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

#pragma ident	"@(#)mlib_v_VideoColorAVDResize_BC.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorResizeABGR - image resize
 *
 * SYNOPSIS
 *     void mlib_VideoColorResizeABGR(mlib_u32    *dst,
 *                                    mlib_u32    *src,
 *                                    mlib_s32    dst_width,
 *                                    mlib_s32    dst_height,
 *                                    mlib_s32    dst_stride,
 *                                    mlib_s32    src_width,
 *                                    mlib_s32    src_height,
 *                                    mlib_s32    src_stride,
 *                                    mlib_filter filter)
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
#include <mlib_v_ImageFilters.h>

/* *********************************************************** */

#define	MLIB_SHIFT	15
#define	MLIB_PREC	(1 << MLIB_SHIFT)
#define	MLIB_PREC2	(1 << (MLIB_SHIFT - 1))
#define	MLIB_MASK	(MLIB_PREC - 1)

/* *********************************************************** */

#define	MLIB_XY_SHIFT	(MLIB_SHIFT - 11)
#define	MLIB_XY_MASK	(((1 << 8) - 1) << 3)

/* *********************************************************** */

#define	FILTER_1	filter1
#define	FILTER_4	filter4

/* *********************************************************** */

#define	FPMUL16(src1, src2)                                     \
	vis_fpadd16(vis_fmul8sux16((src1), (src2)),             \
		vis_fmul8ulx16((src1), (src2)))

/* *********************************************************** */

#define	COEFF_D64_U8(r)                                         \
	fpos = (r >> MLIB_XY_SHIFT) & MLIB_XY_MASK;             \
	dfPtr = (mlib_d64 *)((mlib_u8 *)FILTER_4 + fpos * 4);   \
	d##r##f0 = dfPtr[0];                                    \
	d##r##f1 = dfPtr[1];                                    \
	d##r##f2 = dfPtr[2];                                    \
	d##r##f3 = dfPtr[3]

/* *********************************************************** */

#define	BUF_SIZE	600

/* *********************************************************** */

void
mlib_VideoZoom_U8_4_Bicubic(
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
	mlib_s32 srcX = ((dx + 1) >> 1) - MLIB_PREC2 - MLIB_PREC;
	mlib_s32 srcY = ((dy + 1) >> 1) - MLIB_PREC2 - MLIB_PREC;
	mlib_u8 *sp = (mlib_u8 *)src;
	mlib_u8 *dp = (mlib_u8 *)dst;
	mlib_u8 *filter1 = (mlib_u8 *)mlib_filters_u8_bc;
	mlib_u8 *filter4 = (mlib_u8 *)mlib_filters_u8_bc_4;
	mlib_s32 i, j, k, cx;
	mlib_s32 fpos;
	mlib_s32 x, y;
	mlib_s32 sh1 = sh - 1, sw1 = 4 * (sw - 1);
	mlib_d64 *buffx, *buf_arr[4], *buff0, *buff1, *buff2, *buff3;
	mlib_s32 poi_arr[4];
	mlib_d64 buf_space[5 * BUF_SIZE];

	if (dw > BUF_SIZE) {
		buffx = __mlib_malloc(5 * dw * sizeof (mlib_d64));

		if (!buffx)
			return;
	} else
		buffx = buf_space;

	for (i = 0; i < 4; i++) {
		buf_arr[i] = buffx + (i + 1) * dw;
		poi_arr[i] = -1;
	}

	vis_write_gsr(3 << 3);

	{
		x = srcX;
		fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
		x += dx;

#pragma pipeloop(0)
		for (i = 0; i < dw; i++) {
			mlib_d64 val =
				*(mlib_d64 *)((mlib_u8 *)FILTER_1 + fpos);

			fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
			x += dx;
			buffx[i] = val;
		}
	}

	sw = (sw - 3) << MLIB_SHIFT;
	y = srcY;

	for (j = 0; j < dh; j++) {
		mlib_d64 *dfPtr, dyf0, dyf1, dyf2, dyf3, dxf0;
		mlib_u8 *tsp, *tsp1, *tsp2, *tsp3;
		mlib_s32 srcys[4];

		for (i = 0; i < 4; i++) {
			srcys[i] = (y >> MLIB_SHIFT) + i;
			srcys[i] =
				(srcys[i] < 0) ? 0 : (srcys[i] >
				sh1) ? sh1 : srcys[i];
		}

		for (k = 0; k < 3; k++) {
			mlib_s32 poi = srcys[k] & 3;

			if (poi_arr[poi] == srcys[k])
				continue;
			poi_arr[poi] = srcys[k];
			buff3 = buf_arr[poi];

			i = 0;
			x = srcX;
			tsp = sp + srcys[k] * src_stride;
			tsp1 = tsp + 4;
			tsp2 = tsp + 8;
			tsp3 = tsp + 12;

			while (x < 0 && i < dw) {
				mlib_f32 s0, s1, s2, s3;
				mlib_d64 r0, r1, r2, r3, r4, r5, r6;
				mlib_s32 srcx0 = (x >> (MLIB_SHIFT - 2)) & ~3;
				mlib_s32 srcx1 = srcx0 + 4, srcx2 =
					srcx0 + 8, srcx3 = srcx0 + 12;

				srcx0 = 0;
				srcx1 = (srcx1 & ~(srcx1 >> 31));
				srcx1 = sw1 - srcx1;
				srcx1 = sw1 - (srcx1 & ~(srcx1 >> 31));
				srcx2 = (srcx2 & ~(srcx2 >> 31));
				srcx2 = sw1 - srcx2;
				srcx2 = sw1 - (srcx2 & ~(srcx2 >> 31));
				srcx3 = (srcx3 & ~(srcx3 >> 31));
				srcx3 = sw1 - srcx3;
				srcx3 = sw1 - (srcx3 & ~(srcx3 >> 31));

				dxf0 = buffx[i];

				s0 = *(mlib_f32 *)(tsp + srcx0);
				s1 = *(mlib_f32 *)(tsp + srcx1);
				s2 = *(mlib_f32 *)(tsp + srcx2);
				s3 = *(mlib_f32 *)(tsp + srcx3);

				r0 = vis_fmul8x16au(s0, vis_read_hi(dxf0));
				r1 = vis_fmul8x16al(s1, vis_read_hi(dxf0));
				r2 = vis_fmul8x16au(s2, vis_read_lo(dxf0));
				r3 = vis_fmul8x16al(s3, vis_read_lo(dxf0));

				r4 = vis_fpadd16(r0, r1);
				r5 = vis_fpadd16(r2, r3);
				r6 = vis_fpadd16(r4, r5);
				buff3[i] = r6;

				x += dx;
				i++;
			}

#pragma pipeloop(0)
			for (; x < sw; i++, x += dx) {
				mlib_f32 s0, s1, s2, s3;
				mlib_d64 r0, r1, r2, r3, r4, r5, r6;

				cx = (x >> (MLIB_SHIFT - 2)) & ~3;
				dxf0 = buffx[i];

				s0 = *(mlib_f32 *)(tsp + cx);
				s1 = *(mlib_f32 *)(tsp1 + cx);
				s2 = *(mlib_f32 *)(tsp2 + cx);
				s3 = *(mlib_f32 *)(tsp3 + cx);

				r0 = vis_fmul8x16au(s0, vis_read_hi(dxf0));
				r1 = vis_fmul8x16al(s1, vis_read_hi(dxf0));
				r2 = vis_fmul8x16au(s2, vis_read_lo(dxf0));
				r3 = vis_fmul8x16al(s3, vis_read_lo(dxf0));

				r4 = vis_fpadd16(r0, r1);
				r5 = vis_fpadd16(r2, r3);
				r6 = vis_fpadd16(r4, r5);
				buff3[i] = r6;
			}

			while (i < dw) {
				mlib_f32 s0, s1, s2, s3;
				mlib_d64 r0, r1, r2, r3, r4, r5, r6;
				mlib_s32 srcx0 = (x >> (MLIB_SHIFT - 2)) & ~3;
				mlib_s32 srcx1 = srcx0 + 4, srcx2 =
					srcx0 + 8, srcx3 = srcx0 + 12;

				srcx0 = (srcx0 & ~(srcx0 >> 31));
				srcx0 = sw1 - srcx0;
				srcx0 = sw1 - (srcx0 & ~(srcx0 >> 31));
				srcx1 = (srcx1 & ~(srcx1 >> 31));
				srcx1 = sw1 - srcx1;
				srcx1 = sw1 - (srcx1 & ~(srcx1 >> 31));
				srcx2 = (srcx2 & ~(srcx2 >> 31));
				srcx2 = sw1 - srcx2;
				srcx2 = sw1 - (srcx2 & ~(srcx2 >> 31));
				srcx3 = sw1;

				dxf0 = buffx[i];

				s0 = *(mlib_f32 *)(tsp + srcx0);
				s1 = *(mlib_f32 *)(tsp + srcx1);
				s2 = *(mlib_f32 *)(tsp + srcx2);
				s3 = *(mlib_f32 *)(tsp + srcx3);

				r0 = vis_fmul8x16au(s0, vis_read_hi(dxf0));
				r1 = vis_fmul8x16al(s1, vis_read_hi(dxf0));
				r2 = vis_fmul8x16au(s2, vis_read_lo(dxf0));
				r3 = vis_fmul8x16al(s3, vis_read_lo(dxf0));

				r4 = vis_fpadd16(r0, r1);
				r5 = vis_fpadd16(r2, r3);
				r6 = vis_fpadd16(r4, r5);
				buff3[i] = r6;

				x += dx;
				i++;
			}
		}

		{
			mlib_s32 poi = srcys[3] & 3;

			COEFF_D64_U8(y);

			buff0 = buf_arr[srcys[0] & 3];
			buff1 = buf_arr[srcys[1] & 3];
			buff2 = buf_arr[srcys[2] & 3];
			buff3 = buf_arr[srcys[3] & 3];

			if (poi_arr[poi] == srcys[3]) {
#pragma pipeloop(0)
				for (i = 0; i < dw; i++) {
					mlib_d64 p0, p1, p2, p3, t0, t1, t2, t3,
						t4, t5, t6;

					p0 = buff0[i];
					p1 = buff1[i];
					p2 = buff2[i];
					p3 = buff3[i];

					t0 = FPMUL16(p0, dyf0);
					t1 = FPMUL16(p1, dyf1);
					t2 = FPMUL16(p2, dyf2);
					t3 = FPMUL16(p3, dyf3);

					t4 = vis_fpadd16(t0, t1);
					t5 = vis_fpadd16(t2, t3);
					t6 = vis_fpadd16(t4, t5);

					*((mlib_f32 *)dp + i) = vis_fpack16(t6);
				}
			} else {
				poi_arr[poi] = srcys[3];

				i = 0;
				x = srcX;
				tsp = sp + srcys[k] * src_stride;
				tsp1 = tsp + 4;
				tsp2 = tsp + 8;
				tsp3 = tsp + 12;

				while (x < 0 && i < dw) {
					mlib_f32 s0, s1, s2, s3;
					mlib_d64 r0, r1, r2, r3, r4, r5, r6;
					mlib_d64 p0, p1, p2, p3, t0, t1, t2, t3,
						t4, t5, t6;
					mlib_s32 srcx0 =
						(x >> (MLIB_SHIFT - 2)) & ~3;
					mlib_s32 srcx1 = srcx0 + 4, srcx2 =
						srcx0 + 8, srcx3 = srcx0 + 12;

					srcx0 = 0;
					srcx1 = (srcx1 & ~(srcx1 >> 31));
					srcx1 = sw1 - srcx1;
					srcx1 = sw1 - (srcx1 & ~(srcx1 >> 31));
					srcx2 = (srcx2 & ~(srcx2 >> 31));
					srcx2 = sw1 - srcx2;
					srcx2 = sw1 - (srcx2 & ~(srcx2 >> 31));
					srcx3 = (srcx3 & ~(srcx3 >> 31));
					srcx3 = sw1 - srcx3;
					srcx3 = sw1 - (srcx3 & ~(srcx3 >> 31));

					dxf0 = buffx[i];

					s0 = *(mlib_f32 *)(tsp + srcx0);
					s1 = *(mlib_f32 *)(tsp + srcx1);
					s2 = *(mlib_f32 *)(tsp + srcx2);
					s3 = *(mlib_f32 *)(tsp + srcx3);

					r0 = vis_fmul8x16au(s0,
						vis_read_hi(dxf0));
					r1 = vis_fmul8x16al(s1,
						vis_read_hi(dxf0));
					r2 = vis_fmul8x16au(s2,
						vis_read_lo(dxf0));
					r3 = vis_fmul8x16al(s3,
						vis_read_lo(dxf0));

					r4 = vis_fpadd16(r0, r1);
					r5 = vis_fpadd16(r2, r3);
					r6 = vis_fpadd16(r4, r5);
					buff3[i] = r6;

					p0 = buff0[i];
					p1 = buff1[i];
					p2 = buff2[i];
					p3 = buff3[i];

					t0 = FPMUL16(p0, dyf0);
					t1 = FPMUL16(p1, dyf1);
					t2 = FPMUL16(p2, dyf2);
					t3 = FPMUL16(p3, dyf3);

					t4 = vis_fpadd16(t0, t1);
					t5 = vis_fpadd16(t2, t3);
					t6 = vis_fpadd16(t4, t5);

					*((mlib_f32 *)dp + i) = vis_fpack16(t6);

					x += dx;
					i++;
				}

#pragma pipeloop(0)
				for (; x < sw; i++, x += dx) {
					mlib_f32 s0, s1, s2, s3;
					mlib_d64 r0, r1, r2, r3, r4, r5, r6;
					mlib_d64 p0, p1, p2, p3, t0, t1, t2, t3,
						t4, t5, t6;

					cx = (x >> (MLIB_SHIFT - 2)) & ~3;
					dxf0 = buffx[i];
					p0 = buff0[i];
					p1 = buff1[i];
					p2 = buff2[i];

					t0 = FPMUL16(p0, dyf0);
					t1 = FPMUL16(p1, dyf1);
					t2 = FPMUL16(p2, dyf2);

					s0 = *(mlib_f32 *)(tsp + cx);
					s1 = *(mlib_f32 *)(tsp1 + cx);
					s2 = *(mlib_f32 *)(tsp2 + cx);
					s3 = *(mlib_f32 *)(tsp3 + cx);

					r0 = vis_fmul8x16au(s0,
						vis_read_hi(dxf0));
					r1 = vis_fmul8x16al(s1,
						vis_read_hi(dxf0));
					r2 = vis_fmul8x16au(s2,
						vis_read_lo(dxf0));
					r3 = vis_fmul8x16al(s3,
						vis_read_lo(dxf0));

					r4 = vis_fpadd16(r0, r1);
					r5 = vis_fpadd16(r2, r3);
					r6 = vis_fpadd16(r4, r5);
					p3 = r6;
					t3 = FPMUL16(p3, dyf3);

					t4 = vis_fpadd16(t0, t1);
					t5 = vis_fpadd16(t2, t3);
					t6 = vis_fpadd16(t4, t5);

					buff3[i] = r6;
					*((mlib_f32 *)dp + i) = vis_fpack16(t6);
				}

				while (i < dw) {
					mlib_f32 s0, s1, s2, s3;
					mlib_d64 r0, r1, r2, r3, r4, r5, r6;
					mlib_d64 p0, p1, p2, p3, t0, t1, t2, t3,
						t4, t5, t6;
					mlib_s32 srcx0 =
						(x >> (MLIB_SHIFT - 2)) & ~3;
					mlib_s32 srcx1 = srcx0 + 4, srcx2 =
						srcx0 + 8, srcx3 = srcx0 + 12;

					srcx0 = (srcx0 & ~(srcx0 >> 31));
					srcx0 = sw1 - srcx0;
					srcx0 = sw1 - (srcx0 & ~(srcx0 >> 31));
					srcx1 = (srcx1 & ~(srcx1 >> 31));
					srcx1 = sw1 - srcx1;
					srcx1 = sw1 - (srcx1 & ~(srcx1 >> 31));
					srcx2 = (srcx2 & ~(srcx2 >> 31));
					srcx2 = sw1 - srcx2;
					srcx2 = sw1 - (srcx2 & ~(srcx2 >> 31));
					srcx3 = sw1;

					dxf0 = buffx[i];

					s0 = *(mlib_f32 *)(tsp + srcx0);
					s1 = *(mlib_f32 *)(tsp + srcx1);
					s2 = *(mlib_f32 *)(tsp + srcx2);
					s3 = *(mlib_f32 *)(tsp + srcx3);

					r0 = vis_fmul8x16au(s0,
						vis_read_hi(dxf0));
					r1 = vis_fmul8x16al(s1,
						vis_read_hi(dxf0));
					r2 = vis_fmul8x16au(s2,
						vis_read_lo(dxf0));
					r3 = vis_fmul8x16al(s3,
						vis_read_lo(dxf0));

					r4 = vis_fpadd16(r0, r1);
					r5 = vis_fpadd16(r2, r3);
					r6 = vis_fpadd16(r4, r5);
					buff3[i] = r6;

					p0 = buff0[i];
					p1 = buff1[i];
					p2 = buff2[i];
					p3 = buff3[i];

					t0 = FPMUL16(p0, dyf0);
					t1 = FPMUL16(p1, dyf1);
					t2 = FPMUL16(p2, dyf2);
					t3 = FPMUL16(p3, dyf3);

					t4 = vis_fpadd16(t0, t1);
					t5 = vis_fpadd16(t2, t3);
					t6 = vis_fpadd16(t4, t5);

					*((mlib_f32 *)dp + i) = vis_fpack16(t6);
					x += dx;
					i++;
				}
			}
		}

		y += dy;
		dp += dst_stride;
	}

	if (buffx != buf_space)
		__mlib_free(buffx);
}

/* *********************************************************** */
