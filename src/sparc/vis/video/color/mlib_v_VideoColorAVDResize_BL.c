/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_v_VideoColorAVDResize_BL.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorResizeABGR - image resize
 *
 * SYNOPSIS
 *     void mlib_VideoColorResizeABGR(mlib_u32       *dst,
 *                                    const mlib_u32 *src,
 *                                    mlib_s32       dst_width,
 *                                    mlib_s32       dst_height,
 *                                    mlib_s32       dst_stride,
 *                                    mlib_s32       src_width,
 *                                    mlib_s32       src_height,
 *                                    mlib_s32       src_stride,
 *                                    mlib_filter    filter)
 *
 * ARGUMENTS
 *      dst         pointer to output image
 *      src         pointer to input image
 *      dst_stride  output image width in pixels
 *      dst_height  output image height in rows
 *      dst_stride  linebytes for output image
 *      src_width   input image width in pixels
 *      src_height  input image height in lines
 *      src_stride  linebytes for input image
 *      filter      type of interpolation filter
 *
 * DESCRIPTION
 *      Function performs the resize source image with dimensions src_width,
 *      src_height into the destination image with dimensions dst_width,
 *      dst_height using nearest-neighbor, bilinear interpolation, or bicubic
 *      interpolation. Edge conditions are handled according to "source extend"
 *      handling scheme. The interpolation filter can be one of the following:
 *        MLIB_NEAREST
 *        MLIB_BILINEAR
 *        MLIB_BICUBIC
 */

#include <mlib_video.h>
#include <vis_proto.h>
#include <mlib_VideoColorAVDResize.h>

/* *********************************************************** */

#define	MLIB_SHIFT	15
#define	MLIB_PREC	(1 << MLIB_SHIFT)
#define	MLIB_PREC2	(1 << (MLIB_SHIFT - 1))
#define	MLIB_MASK	(MLIB_PREC - 1)

/* *********************************************************** */

void
mlib_VideoZoom_U8_4_Bilinear(
	mlib_u32 *dst,
	const mlib_u32 *src,
	mlib_s32 dx,
	mlib_s32 dy,
	mlib_s32 dw,
	mlib_s32 dh,
	mlib_s32 dst_stride,
	mlib_s32 sw,
	mlib_s32 sh,
	mlib_s32 src_stride)
{
	mlib_d64 w_7FFF = vis_to_double_dup(0x7FFF7FFF);
	mlib_d64 w_srcx, w_dx, w_dx2;
	mlib_s32 srcX = ((dx + 1) >> 1) - MLIB_PREC2;
	mlib_s32 srcY = ((dy + 1) >> 1) - MLIB_PREC2;
	mlib_s32 sw2 = 4 * (sw - 1), sw_dx, dx2;
	mlib_u8 *sp = (mlib_u8 *)src, *dp = (mlib_u8 *)dst;
	mlib_s32 i, j, cx, y = srcY;

	sw = (sw - 1) << MLIB_SHIFT;
	dx2 = dx + dx;
	sw_dx = sw - dx;
	dw <<= 2;

	vis_write_gsr(0);

	{
		mlib_s32 cut_z = dx & MLIB_MASK, not_z =
			(0x8000 - cut_z) & 0x7FFF;
		mlib_s32 cut_z1, not_z1;

		w_dx = vis_to_double_dup(not_z << 16 | cut_z);
		w_dx2 = vis_fpadd16(w_dx, w_dx);
		cut_z = srcX & MLIB_MASK;
		not_z = 0x7FFF - cut_z;
		cut_z1 = (srcX + dx) & MLIB_MASK;
		not_z1 = 0x7FFF - cut_z1;
		w_srcx = vis_to_double((not_z << 16) | cut_z,
			(not_z1 << 16) | cut_z1);
	}

	for (j = 0; j < dh; j++) {
		mlib_f32 v_x0, v_x1, v_x2, v_x3, v_y;
		mlib_d64 w_x0, w_x1, w_x2, w_x3, w_res, w_res2;
		mlib_d64 w_x = w_srcx, w_scale0, w_scale1;
		mlib_s32 row = (y >> MLIB_SHIFT), x = srcX, cx2, cut_y;
		mlib_u8 *tsp0, *tsp1, *tsp2, *tsp3;

		if (row < 0) {
			tsp0 = sp;
			tsp2 = tsp0;
		} else if (row >= sh - 1) {
			tsp0 = sp + (sh - 1) * src_stride;
			tsp2 = tsp0;
		} else {
			tsp0 = sp + row * src_stride;
			tsp2 = tsp0 + src_stride;
		}

		tsp1 = tsp0 + 4;
		tsp3 = tsp2 + 4;

		{
			mlib_s32 cut_z;

			cut_y = (y >> (MLIB_SHIFT - 8)) & 0xFF;
			cut_z = (cut_y << 8) | cut_y;
			v_y = vis_to_float((cut_z << 16) | cut_z);
		}

		cx = 0;
		i = 0;

		w_scale1 = vis_fmul8x16(v_y, w_x);
		w_scale0 = vis_fpsub16(w_x, w_scale1);

		while (x < 0 && i < dw) {
			w_x = vis_fpadd16(w_x, w_dx);
			w_x = vis_fand(w_x, w_7FFF);
			v_x0 = *(mlib_f32 *)(tsp0 + cx);
			v_x2 = *(mlib_f32 *)(tsp2 + cx);
			w_x0 = vis_fmul8x16au(v_x0, vis_read_hi(w_scale0));
			w_x1 = vis_fmul8x16al(v_x0, vis_read_hi(w_scale0));
			w_x2 = vis_fmul8x16au(v_x2, vis_read_hi(w_scale1));
			w_x3 = vis_fmul8x16al(v_x2, vis_read_hi(w_scale1));
			w_scale1 = vis_fmul8x16(v_y, w_x);
			w_scale0 = vis_fpsub16(w_x, w_scale1);
			w_res = vis_fpadd16(w_x0, w_x1);
			w_res = vis_fpadd16(w_res, w_x2);
			w_res = vis_fpadd16(w_res, w_x3);
			*(mlib_f32 *)(dp + i) = vis_fpack16(w_res);
			i += 4;
			x += dx;
		}

		if ((((mlib_addr)dp + i) & 4) && (x < sw)) {
			w_x = vis_fpadd16(w_x, w_dx);
			w_x = vis_fand(w_x, w_7FFF);
			cx = (x >> (MLIB_SHIFT - 2)) & ~3;
			v_x0 = *(mlib_f32 *)(tsp0 + cx);
			v_x1 = *(mlib_f32 *)(tsp1 + cx);
			v_x2 = *(mlib_f32 *)(tsp2 + cx);
			v_x3 = *(mlib_f32 *)(tsp3 + cx);
			w_x0 = vis_fmul8x16au(v_x0, vis_read_hi(w_scale0));
			w_x1 = vis_fmul8x16al(v_x1, vis_read_hi(w_scale0));
			w_x2 = vis_fmul8x16au(v_x2, vis_read_hi(w_scale1));
			w_x3 = vis_fmul8x16al(v_x3, vis_read_hi(w_scale1));
			w_scale1 = vis_fmul8x16(v_y, w_x);
			w_scale0 = vis_fpsub16(w_x, w_scale1);
			w_res = vis_fpadd16(w_x0, w_x1);
			w_res = vis_fpadd16(w_res, w_x2);
			w_res = vis_fpadd16(w_res, w_x3);
			*(mlib_f32 *)(dp + i) = vis_fpack16(w_res);
			i += 4;
			x += dx;
		}

		if (tsp0 != tsp2 && cut_y != 0) {
			cx = (x >> (MLIB_SHIFT - 2)) & ~3;
			cx2 = ((x + dx) >> (MLIB_SHIFT - 2)) & ~3;

#pragma pipeloop(0)
			for (; x < sw_dx; i += 8) {
				x += dx2;
				w_x = vis_fpadd16(w_x, w_dx2);
				w_x = vis_fand(w_x, w_7FFF);
				v_x0 = *(mlib_f32 *)(tsp0 + cx);
				v_x1 = *(mlib_f32 *)(tsp1 + cx);
				v_x2 = *(mlib_f32 *)(tsp2 + cx);
				v_x3 = *(mlib_f32 *)(tsp3 + cx);
				w_x0 = vis_fmul8x16au(v_x0,
					vis_read_hi(w_scale0));
				w_x1 = vis_fmul8x16al(v_x1,
					vis_read_hi(w_scale0));
				w_x2 = vis_fmul8x16au(v_x2,
					vis_read_hi(w_scale1));
				w_x3 = vis_fmul8x16al(v_x3,
					vis_read_hi(w_scale1));
				w_res = vis_fpadd16(w_x0, w_x1);
				w_res = vis_fpadd16(w_res, w_x2);
				w_res = vis_fpadd16(w_res, w_x3);
				v_x0 = *(mlib_f32 *)(tsp0 + cx2);
				v_x1 = *(mlib_f32 *)(tsp1 + cx2);
				v_x2 = *(mlib_f32 *)(tsp2 + cx2);
				v_x3 = *(mlib_f32 *)(tsp3 + cx2);
				w_x0 = vis_fmul8x16au(v_x0,
					vis_read_lo(w_scale0));
				w_x1 = vis_fmul8x16al(v_x1,
					vis_read_lo(w_scale0));
				w_x2 = vis_fmul8x16au(v_x2,
					vis_read_lo(w_scale1));
				w_x3 = vis_fmul8x16al(v_x3,
					vis_read_lo(w_scale1));
				w_scale1 = vis_fmul8x16(v_y, w_x);
				w_scale0 = vis_fpsub16(w_x, w_scale1);
				w_res2 = vis_fpadd16(w_x0, w_x1);
				w_res2 = vis_fpadd16(w_res2, w_x2);
				w_res2 = vis_fpadd16(w_res2, w_x3);
				cx = ((x) >> (MLIB_SHIFT - 2)) & ~3;
				cx2 = ((x + dx) >> (MLIB_SHIFT - 2)) & ~3;
				*(mlib_d64 *)(dp + i) =
					vis_fpack16_pair(w_res, w_res2);
			}
		} else {
			cx = (x >> (MLIB_SHIFT - 2)) & ~3;
			cx2 = ((x + dx) >> (MLIB_SHIFT - 2)) & ~3;

#pragma pipeloop(0)
			for (; x < sw_dx; i += 8) {
				x += dx2;
				v_x0 = *(mlib_f32 *)(tsp0 + cx);
				v_x1 = *(mlib_f32 *)(tsp1 + cx);
				w_x0 = vis_fmul8x16au(v_x0, vis_read_hi(w_x));
				w_x1 = vis_fmul8x16al(v_x1, vis_read_hi(w_x));
				w_res = vis_fpadd16(w_x0, w_x1);
				v_x0 = *(mlib_f32 *)(tsp0 + cx2);
				v_x1 = *(mlib_f32 *)(tsp1 + cx2);
				w_x0 = vis_fmul8x16au(v_x0, vis_read_lo(w_x));
				w_x1 = vis_fmul8x16al(v_x1, vis_read_lo(w_x));
				w_x = vis_fpadd16(w_x, w_dx2);
				w_x = vis_fand(w_x, w_7FFF);
				w_res2 = vis_fpadd16(w_x0, w_x1);
				cx = ((x) >> (MLIB_SHIFT - 2)) & ~3;
				cx2 = ((x + dx) >> (MLIB_SHIFT - 2)) & ~3;
				*(mlib_d64 *)(dp + i) =
					vis_fpack16_pair(w_res, w_res2);
			}

			w_scale1 = vis_fmul8x16(v_y, w_x);
			w_scale0 = vis_fpsub16(w_x, w_scale1);
		}

		if (x < sw) {
			w_x = vis_fpadd16(w_x, w_dx);
			w_x = vis_fand(w_x, w_7FFF);
			cx = (x >> (MLIB_SHIFT - 2)) & ~3;
			v_x0 = *(mlib_f32 *)(tsp0 + cx);
			v_x1 = *(mlib_f32 *)(tsp1 + cx);
			v_x2 = *(mlib_f32 *)(tsp2 + cx);
			v_x3 = *(mlib_f32 *)(tsp3 + cx);
			w_x0 = vis_fmul8x16au(v_x0, vis_read_hi(w_scale0));
			w_x1 = vis_fmul8x16al(v_x1, vis_read_hi(w_scale0));
			w_x2 = vis_fmul8x16au(v_x2, vis_read_hi(w_scale1));
			w_x3 = vis_fmul8x16al(v_x3, vis_read_hi(w_scale1));
			w_scale1 = vis_fmul8x16(v_y, w_x);
			w_scale0 = vis_fpsub16(w_x, w_scale1);
			w_res = vis_fpadd16(w_x0, w_x1);
			w_res = vis_fpadd16(w_res, w_x2);
			w_res = vis_fpadd16(w_res, w_x3);
			*(mlib_f32 *)(dp + i) = vis_fpack16(w_res);
			i += 4;
			x += dx;
		}

		cx = sw2;

		while (i < dw) {
			w_x = vis_fpadd16(w_x, w_dx);
			w_x = vis_fand(w_x, w_7FFF);
			v_x0 = *(mlib_f32 *)(tsp0 + cx);
			v_x2 = *(mlib_f32 *)(tsp2 + cx);
			w_x0 = vis_fmul8x16au(v_x0, vis_read_hi(w_scale0));
			w_x1 = vis_fmul8x16al(v_x0, vis_read_hi(w_scale0));
			w_x2 = vis_fmul8x16au(v_x2, vis_read_hi(w_scale1));
			w_x3 = vis_fmul8x16al(v_x2, vis_read_hi(w_scale1));
			w_scale1 = vis_fmul8x16(v_y, w_x);
			w_scale0 = vis_fpsub16(w_x, w_scale1);
			w_res = vis_fpadd16(w_x0, w_x1);
			w_res = vis_fpadd16(w_res, w_x2);
			w_res = vis_fpadd16(w_res, w_x3);
			*(mlib_f32 *)(dp + i) = vis_fpack16(w_res);
			i += 4;
			x += dx;
		}

		y += dy;
		dp += dst_stride;
	}
}

/* *********************************************************** */
