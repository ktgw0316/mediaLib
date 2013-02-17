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

#pragma ident	"@(#)mlib_v_ImageZoomIndex_BL.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomIndex - indexed color image scaling
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoomIndex(mlib_image       *dst,
 *                                      const mlib_image *src,
 *                                      mlib_d64         zoomx,
 *                                      mlib_d64         zoomy,
 *                                      mlib_filter      filter,
 *                                      mlib_edge        edge,
 *                                      const void       *colormap)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image
 *      src       Pointer to source image
 *      zoomx     X zoom factor.
 *      zoomy     Y zoom factor.
 *      filter    Type of resampling filter.
 *      edge      Type of edge conditions.
 *      colormap  Internal structure.
 *
 * DESCRIPTION
 *  The center of the source image is mapped to the center of the
 *  destination image.
 *
 *  The resampling filter can be one of the following:
 *    MLIB_NEAREST
 *    MLIB_BILINEAR
 *    MLIB_BICUBIC
 *    MLIB_BICUBIC2
 *
 *  The edge condition can be one of the following:
 *    MLIB_EDGE_DST_NO_WRITE  (default)
 *    MLIB_EDGE_DST_FILL_ZERO
 *    MLIB_EDGE_OP_NEAREST
 *    MLIB_EDGE_SRC_EXTEND
 *    MLIB_EDGE_SRC_PADDED
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageColormap.h>

/* *********************************************************** */

#define	_MLIB_VIS_VER_

#include <mlib_ImageZoom.h>

/* *********************************************************** */

/*			Bilinear				*/

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIndex_U8_U8_Bilinear(
    mlib_work_image *param)
{
	void *colormap = GetElemStruct(colormap);
	mlib_f32 a0, a1, a2, a3, fy64;
	mlib_f32 *tdp;
	mlib_f32 *colortable =
	    (mlib_f32 *)mlib_ImageGetLutNormalTable(colormap) -
	    mlib_ImageGetLutOffset(colormap);
	mlib_u8 *dpbuf = GetElemStruct(buffer_dp), *tsp;
	mlib_u8 *sp = GetElemSubStruct(current, sp);
	mlib_u8 *dp = GetElemSubStruct(current, dp);
	mlib_s32 i, j, cx, next_step = 2;
	mlib_s32 dx = GetElemStruct(DX);
	mlib_s32 dy = GetElemStruct(DY);
	mlib_s32 channels = mlib_ImageGetLutChannels(colormap);
	mlib_s32 x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	mlib_s32 y = GetElemSubStruct(current, srcY) & MLIB_MASK;
	mlib_s32 src_stride = GetElemStruct(src_stride);
	mlib_s32 dst_stride = GetElemStruct(dst_stride);
	mlib_s32 width = GetElemSubStruct(current, width);
	mlib_s32 height = GetElemSubStruct(current, height);
	mlib_d64 *buff, *buf0, *buf1, *bufT;
	mlib_d64 buffer[2048];
	mlib_d64 x64, rx64, xdx, r0, r1, r2, r3, r4, r5, r6, r7, r8;
	mlib_d64 mask_7fff = vis_to_double_dup(0x7FFF7FFF);
	mlib_pack_func func;

	tsp = sp;
	vis_write_gsr(7);

	func =
	    (channels ==
	    3) ? (mlib_pack_func) mlib_ImageColorTrue2IndexLine_U8_U8_3_in_4
	    : (mlib_pack_func) mlib_ImageColorTrue2IndexLine_U8_U8_4;

	if (width * 2 > 2048) {
		buff = __mlib_malloc(sizeof (mlib_d64) * width * 2);

		if (buff == NULL)
			return (MLIB_FAILURE);
	} else
		buff = buffer;
	buf0 = buff;
	buf1 = buf0 + width;
	xdx = vis_to_double_dup(((dx & 0xFFFE) << 15) | ((dx & 0xFFFE) >> 1));

	for (j = 0; j < height; j++) {

		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		tdp = (void *)dpbuf;

		cx = (y >> 8) & 0xFF;
		fy64 = vis_to_float((cx << 24) | (cx << 16) | (cx << 8) | cx);
		x64 =
		    vis_to_double_dup(((x & 0xFFFE) << 15) | ((x & 0xFFFE) >>
		    1));
		rx64 = vis_fpsub16(mask_7fff, x64);

		cx = next_step;
		next_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);

		if (cx == 0) {
#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				r4 = buf0[i];
				r5 = buf1[i];
				r6 = vis_fpsub16(r5, r4);
				r7 = vis_fmul8x16(fy64, r6);
				r8 = vis_fpadd16(r7, r4);
				tdp[i] = vis_fpack16(r8);
			}
		} else {
			if (cx == 1) {
				bufT = buf0;
				buf0 = buf1;
				buf1 = bufT;
				tsp += src_stride;
#pragma pipeloop(0)
				for (i = 0; i < width; i++) {
					cx = x >> MLIB_SHIFT;
					r4 = buf0[i];
					a2 = colortable[tsp[cx + 1]];
					a3 = colortable[tsp[cx]];
					r2 = vis_fmul8x16(a2, x64);
					r3 = vis_fmul8x16(a3, rx64);
					r5 = vis_fpadd16(r2, r3);
					r6 = vis_fpsub16(r5, r4);
					r7 = vis_fmul8x16(fy64, r6);
					r8 = vis_fpadd16(r7, r4);
					buf1[i] = r5;
					x += dx;
					tdp[i] = vis_fpack16(r8);
					x64 =
					    vis_fand(vis_fpadd16(x64, xdx),
					    mask_7fff);
					rx64 = vis_fpsub16(mask_7fff, x64);
				}
			} else {
#pragma pipeloop(0)
				for (i = 0; i < width; i++) {
					cx = x >> MLIB_SHIFT;
					a0 = colortable[tsp[cx]];
					a1 = colortable[tsp[cx + 1]];
					a2 = colortable[tsp[cx + 1 +
					    src_stride]];
					a3 = colortable[tsp[cx + src_stride]];
					r0 = vis_fmul8x16(a0, rx64);
					r1 = vis_fmul8x16(a1, x64);
					r2 = vis_fmul8x16(a2, x64);
					r3 = vis_fmul8x16(a3, rx64);
					r4 = vis_fpadd16(r0, r1);
					r5 = vis_fpadd16(r2, r3);
					r6 = vis_fpsub16(r5, r4);
					r7 = vis_fmul8x16(fy64, r6);
					r8 = vis_fpadd16(r7, r4);
					buf0[i] = r4;
					buf1[i] = r5;
					x += dx;
					tdp[i] = vis_fpack16(r8);
					x64 =
					    vis_fand(vis_fpadd16(x64, xdx),
					    mask_7fff);
					rx64 = vis_fpsub16(mask_7fff, x64);
				}
			}
		}

		func(dpbuf, dp, width, colormap);

		y += dy;
		dp += dst_stride;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	if (buffer != buff)
		__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIndex_U8_S16_Bilinear(
    mlib_work_image *param)
{
	void *colormap = GetElemStruct(colormap);
	mlib_d64 *tdp;
	mlib_d64 *colortable =
	    (mlib_d64 *)mlib_ImageGetLutNormalTable(colormap) -
	    mlib_ImageGetLutOffset(colormap);
	mlib_s16 *dpbuf = GetElemStruct(buffer_dp);
	mlib_u8 *tsp, *sp = GetElemSubStruct(current, sp);
	mlib_u8 *dp = GetElemSubStruct(current, dp);
	mlib_s32 i, j, cx, next_step = 2;
	mlib_s32 dx = GetElemStruct(DX);
	mlib_s32 dy = GetElemStruct(DY);
	mlib_s32 x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	mlib_s32 y = GetElemSubStruct(current, srcY) & MLIB_MASK;
	mlib_s32 channels = mlib_ImageGetLutChannels(colormap);
	mlib_s32 src_stride = GetElemStruct(src_stride);
	mlib_s32 dst_stride = GetElemStruct(dst_stride);
	mlib_s32 width = GetElemSubStruct(current, width);
	mlib_s32 height = GetElemSubStruct(current, height);
	mlib_d64 mask_7fff = vis_to_double_dup(0x7FFF7FFF);
	mlib_d64 *buf0, *buf1, *buff, *bufT, *coeff;
	mlib_d64 buffer[3072];
	mlib_d64 a0, a1, a2, a3, r0, r1, r2, r3, r4, r5, r6, r7, r8;
	mlib_d64 dx64, dy64, rdx64, rdy64, xdx;
	mlib_pack_func func;

	func =
	    (channels ==
	    3) ? (mlib_pack_func) mlib_ImageColorTrue2IndexLine_S16_U8_3_in_4
	    : (mlib_pack_func) mlib_ImageColorTrue2IndexLine_S16_U8_4;

	tsp = (void *)sp;

	if (width * 3 > 3072) {
		buff = __mlib_malloc(sizeof (mlib_d64) * width * 3);

		if (buff == NULL)
			return (MLIB_FAILURE);
	} else
		buff = buffer;

	buf0 = buff;
	buf1 = buf0 + width;
	coeff = buf1 + width;

	dx64 = vis_to_double_dup(((x & 0xFFFE) << 15) | ((x & 0xFFFE) >> 1));
	xdx = vis_to_double_dup(((dx & 0xFFFE) >> 1) | ((dx & 0xFFFE) << 15));
	for (i = 0; i < width; i++) {
		coeff[i] = dx64;
		dx64 = vis_fand(vis_fpadd16(dx64, xdx), mask_7fff);
	}

	for (j = 0; j < height; j++) {

		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		tdp = (void *)dpbuf;

		dy64 =
		    vis_to_double_dup(((y & 0xFFFE) >> 1) | ((y & 0xFFFE) <<
		    15));
		rdy64 = vis_fpsub16(mask_7fff, dy64);

		cx = next_step;
		next_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);

		if (cx == 0) {
#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				r2 = buf0[i];
				r5 = buf1[i];

				r6 = vis_fpadd16(vis_fmul8sux16(rdy64, r2),
				    vis_fmul8ulx16(rdy64, r2));
				r7 = vis_fpadd16(vis_fmul8sux16(dy64, r5),
				    vis_fmul8ulx16(dy64, r5));
				r8 = vis_fpadd16(r6, r7);
				r8 = vis_fpadd16(r8, r8);

				tdp[i] = r8;
			}
		} else {
			if (cx == 1) {
				bufT = buf0;
				buf0 = buf1;
				buf1 = bufT;
				tsp += src_stride;
#pragma pipeloop(0)
				for (i = 0; i < width; i++) {
					cx = x >> MLIB_SHIFT;

					dx64 = coeff[i];
					rdx64 = vis_fpsub16(mask_7fff, dx64);

					a2 = colortable[tsp[cx + 1]];
					a3 = colortable[tsp[cx]];
					r2 = buf0[i];

					r3 = vis_fpadd16(vis_fmul8sux16(rdx64,
					    a3), vis_fmul8ulx16(rdx64, a3));
					r4 = vis_fpadd16(vis_fmul8sux16(dx64,
					    a2), vis_fmul8ulx16(dx64, a2));
					r5 = vis_fpadd16(r3, r4);
					r5 = vis_fpadd16(r5, r5);

					r6 = vis_fpadd16(vis_fmul8sux16(rdy64,
					    r2), vis_fmul8ulx16(rdy64, r2));
					r7 = vis_fpadd16(vis_fmul8sux16(dy64,
					    r5), vis_fmul8ulx16(dy64, r5));
					r8 = vis_fpadd16(r6, r7);
					r8 = vis_fpadd16(r8, r8);

					buf1[i] = r5;
					tdp[i] = r8;
					x += dx;
				}
			} else {
#pragma pipeloop(0)
				for (i = 0; i < width; i++) {
					cx = x >> MLIB_SHIFT;

					dx64 = coeff[i];
					rdx64 = vis_fpsub16(mask_7fff, dx64);

					a0 = colortable[tsp[cx]];
					a1 = colortable[tsp[cx + 1]];
					a2 = colortable[tsp[cx + src_stride +
					    1]];
					a3 = colortable[tsp[cx + src_stride]];

					r0 = vis_fpadd16(vis_fmul8sux16(rdx64,
					    a0), vis_fmul8ulx16(rdx64, a0));
					r1 = vis_fpadd16(vis_fmul8sux16(dx64,
					    a1), vis_fmul8ulx16(dx64, a1));
					r2 = vis_fpadd16(r0, r1);
					r2 = vis_fpadd16(r2, r2);

					r3 = vis_fpadd16(vis_fmul8sux16(rdx64,
					    a3), vis_fmul8ulx16(rdx64, a3));
					r4 = vis_fpadd16(vis_fmul8sux16(dx64,
					    a2), vis_fmul8ulx16(dx64, a2));
					r5 = vis_fpadd16(r3, r4);
					r5 = vis_fpadd16(r5, r5);

					r6 = vis_fpadd16(vis_fmul8sux16(rdy64,
					    r2), vis_fmul8ulx16(rdy64, r2));
					r7 = vis_fpadd16(vis_fmul8sux16(dy64,
					    r5), vis_fmul8ulx16(dy64, r5));
					r8 = vis_fpadd16(r6, r7);
					r8 = vis_fpadd16(r8, r8);

					buf0[i] = r2;
					buf1[i] = r5;
					tdp[i] = r8;
					x += dx;
				}
			}
		}

		func(dpbuf, dp, width, colormap);

		y += dy;
		dp += dst_stride;
		tsp = (void *)(sp + (y >> MLIB_SHIFT) * src_stride);
	}

	if (buff != buffer)
		__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIndex_S16_S16_Bilinear(
    mlib_work_image *param)
{
	void *colormap = GetElemStruct(colormap);
	mlib_d64 *tdp,
	    *colortable =
		(mlib_d64 *)mlib_ImageGetLutNormalTable(colormap) -
		mlib_ImageGetLutOffset(colormap);
	mlib_u8 *tsp;
	mlib_s16 *dpbuf = GetElemStruct(buffer_dp);
	mlib_s16 *sp = GetElemSubStruct(current, sp);
	mlib_s16 *dp = GetElemSubStruct(current, dp);
	mlib_s32 i, j, cx, next_step = 2;
	mlib_s32 dx = GetElemStruct(DX);
	mlib_s32 dy = GetElemStruct(DY);
	mlib_s32 channels = mlib_ImageGetLutChannels(colormap);
	mlib_s32 x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	mlib_s32 y = GetElemSubStruct(current, srcY) & MLIB_MASK;
	mlib_s32 src_stride = GetElemStruct(src_stride);
	mlib_s32 dst_stride = GetElemStruct(dst_stride);
	mlib_s32 width = GetElemSubStruct(current, width);
	mlib_s32 height = GetElemSubStruct(current, height);
	mlib_d64 mask_7fff = vis_to_double_dup(0x7FFF7FFF);
	mlib_d64 *buf0, *buf1, *buff, *bufT, *coeff;
	mlib_d64 buffer[3072];
	mlib_d64 a0, a1, a2, a3, r0, r1, r2, r3, r4, r5, r6, r7, r8;
	mlib_d64 dx64, dy64, rdx64, rdy64, xdx;
	mlib_pack_func func;

	func =
	    (channels ==
	    3) ? (mlib_pack_func) mlib_ImageColorTrue2IndexLine_S16_S16_3_in_4
	    : (mlib_pack_func) mlib_ImageColorTrue2IndexLine_S16_S16_4;

	tsp = (void *)sp;

	if (width * 3 > 3072) {
		buff = __mlib_malloc(sizeof (mlib_d64) * width * 3);

		if (buff == NULL)
			return (MLIB_FAILURE);
	} else
		buff = buffer;

	buf0 = buff;
	buf1 = buf0 + width;
	coeff = buf1 + width;

	dx64 = vis_to_double_dup(((x & 0xFFFE) << 15) | ((x & 0xFFFE) >> 1));
	xdx = vis_to_double_dup(((dx & 0xFFFE) >> 1) | ((dx & 0xFFFE) << 15));
	for (i = 0; i < width; i++) {
		coeff[i] = dx64;
		dx64 = vis_fand(vis_fpadd16(dx64, xdx), mask_7fff);
	}

	for (j = 0; j < height; j++) {

		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		tdp = (void *)dpbuf;

		dy64 =
		    vis_to_double_dup(((y & 0xFFFE) >> 1) | ((y & 0xFFFE) <<
		    15));
		rdy64 = vis_fpsub16(mask_7fff, dy64);

		cx = next_step;
		next_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);

		if (cx == 0) {
#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				r2 = buf0[i];
				r5 = buf1[i];

				r6 = vis_fpadd16(vis_fmul8sux16(rdy64, r2),
				    vis_fmul8ulx16(rdy64, r2));
				r7 = vis_fpadd16(vis_fmul8sux16(dy64, r5),
				    vis_fmul8ulx16(dy64, r5));
				r8 = vis_fpadd16(r6, r7);
				r8 = vis_fpadd16(r8, r8);

				tdp[i] = r8;
			}
		} else {
			if (cx == 1) {
				bufT = buf0;
				buf0 = buf1;
				buf1 = bufT;
				tsp += 2 * src_stride;
#pragma pipeloop(0)
				for (i = 0; i < width; i++) {
					cx = (x >> (MLIB_SHIFT - 1)) & ~1;

					dx64 = coeff[i];
					rdx64 = vis_fpsub16(mask_7fff, dx64);

					a2 = colortable[*(mlib_s16 *)(tsp + cx +
					    2)];
					a3 = colortable[*(mlib_s16 *)(tsp +
					    cx)];
					r2 = buf0[i];

					r3 = vis_fpadd16(vis_fmul8sux16(rdx64,
					    a3), vis_fmul8ulx16(rdx64, a3));
					r4 = vis_fpadd16(vis_fmul8sux16(dx64,
					    a2), vis_fmul8ulx16(dx64, a2));
					r5 = vis_fpadd16(r3, r4);
					r5 = vis_fpadd16(r5, r5);

					r6 = vis_fpadd16(vis_fmul8sux16(rdy64,
					    r2), vis_fmul8ulx16(rdy64, r2));
					r7 = vis_fpadd16(vis_fmul8sux16(dy64,
					    r5), vis_fmul8ulx16(dy64, r5));
					r8 = vis_fpadd16(r6, r7);
					r8 = vis_fpadd16(r8, r8);

					buf1[i] = r5;
					tdp[i] = r8;
					x += dx;
				}
			} else {
#pragma pipeloop(0)
				for (i = 0; i < width; i++) {
					cx = (x >> (MLIB_SHIFT - 1)) & ~1;

					dx64 = coeff[i];
					rdx64 = vis_fpsub16(mask_7fff, dx64);

					a0 = colortable[*(mlib_s16 *)(tsp +
					    cx)];
					a1 = colortable[*(mlib_s16 *)(tsp + cx +
					    2)];
					a2 = colortable[*(mlib_s16 *)(tsp + cx +
					    2 * src_stride + 2)];
					a3 = colortable[*(mlib_s16 *)(tsp + cx +
					    2 * src_stride)];

					r0 = vis_fpadd16(vis_fmul8sux16(rdx64,
					    a0), vis_fmul8ulx16(rdx64, a0));
					r1 = vis_fpadd16(vis_fmul8sux16(dx64,
					    a1), vis_fmul8ulx16(dx64, a1));
					r2 = vis_fpadd16(r0, r1);
					r2 = vis_fpadd16(r2, r2);

					r3 = vis_fpadd16(vis_fmul8sux16(rdx64,
					    a3), vis_fmul8ulx16(rdx64, a3));
					r4 = vis_fpadd16(vis_fmul8sux16(dx64,
					    a2), vis_fmul8ulx16(dx64, a2));
					r5 = vis_fpadd16(r3, r4);
					r5 = vis_fpadd16(r5, r5);

					r6 = vis_fpadd16(vis_fmul8sux16(rdy64,
					    r2), vis_fmul8ulx16(rdy64, r2));
					r7 = vis_fpadd16(vis_fmul8sux16(dy64,
					    r5), vis_fmul8ulx16(dy64, r5));
					r8 = vis_fpadd16(r6, r7);
					r8 = vis_fpadd16(r8, r8);

					buf0[i] = r2;
					buf1[i] = r5;
					tdp[i] = r8;
					x += dx;
				}
			}
		}

		func(dpbuf, dp, width, colormap);

		y += dy;
		dp += dst_stride;
		tsp = (void *)(sp + (y >> MLIB_SHIFT) * src_stride);
	}

	if (buff != buffer)
		__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIndex_S16_U8_Bilinear(
    mlib_work_image *param)
{
	void *colormap = GetElemStruct(colormap);
	mlib_f32 a0, a1, a2, a3, fy64, *tdp;
	mlib_f32 *colortable =
	    (mlib_f32 *)mlib_ImageGetLutNormalTable(colormap) -
	    mlib_ImageGetLutOffset(colormap);
	mlib_u8 *tsp, *dpbuf = GetElemStruct(buffer_dp);
	mlib_s16 *sp = GetElemSubStruct(current, sp);
	mlib_s16 *dp = GetElemSubStruct(current, dp);
	mlib_s32 i, j, cx, next_step = 2;
	mlib_s32 dx = GetElemStruct(DX);
	mlib_s32 dy = GetElemStruct(DY);
	mlib_s32 x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	mlib_s32 y = GetElemSubStruct(current, srcY) & MLIB_MASK;
	mlib_s32 channels = mlib_ImageGetLutChannels(colormap);
	mlib_s32 src_stride = GetElemStruct(src_stride);
	mlib_s32 dst_stride = GetElemStruct(dst_stride);
	mlib_s32 width = GetElemSubStruct(current, width);
	mlib_s32 height = GetElemSubStruct(current, height);
	mlib_d64 *buff, *buf0, *buf1, *bufT;
	mlib_d64 buffer[2048];
	mlib_d64 x64, rx64, xdx;
	mlib_d64 r0, r1, r2, r3, r4, r5, r6, r7, r8;
	mlib_d64 mask_7fff = vis_to_double_dup(0x7FFF7FFF);
	mlib_pack_func func;

	tsp = (void *)sp;
	vis_write_gsr(7);

	func =
	    (channels ==
	    3) ? (mlib_pack_func) mlib_ImageColorTrue2IndexLine_U8_S16_3_in_4
	    : (mlib_pack_func) mlib_ImageColorTrue2IndexLine_U8_S16_4;

	if (width * 2 > 2048) {
		buff = __mlib_malloc(sizeof (mlib_d64) * width * 2);

		if (buff == NULL)
			return (MLIB_FAILURE);
	} else
		buff = buffer;
	buf0 = buff;
	buf1 = buf0 + width;
	xdx = vis_to_double_dup(((dx & 0xFFFE) << 15) | ((dx & 0xFFFE) >> 1));

	for (j = 0; j < height; j++) {

		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		tdp = (void *)dpbuf;

		cx = (y >> 8) & 0xFF;
		fy64 = vis_to_float((cx << 24) | (cx << 16) | (cx << 8) | cx);
		x64 =
		    vis_to_double_dup(((x & 0xFFFE) << 15) | ((x & 0xFFFE) >>
		    1));
		rx64 = vis_fpsub16(mask_7fff, x64);

		cx = next_step;
		next_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);

		if (cx == 0) {
#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				r4 = buf0[i];
				r5 = buf1[i];
				r6 = vis_fpsub16(r5, r4);
				r7 = vis_fmul8x16(fy64, r6);
				r8 = vis_fpadd16(r7, r4);
				tdp[i] = vis_fpack16(r8);
			}
		} else {
			if (cx == 1) {
				bufT = buf0;
				buf0 = buf1;
				buf1 = bufT;
				tsp += 2 * src_stride;
#pragma pipeloop(0)
				for (i = 0; i < width; i++) {
					cx = (x >> (MLIB_SHIFT - 1)) & ~1;
					r4 = buf0[i];
					a2 = colortable[*(mlib_s16 *)(tsp + cx +
					    2)];
					a3 = colortable[*(mlib_s16 *)(tsp +
					    cx)];
					r2 = vis_fmul8x16(a2, x64);
					r3 = vis_fmul8x16(a3, rx64);
					r5 = vis_fpadd16(r2, r3);
					r6 = vis_fpsub16(r5, r4);
					r7 = vis_fmul8x16(fy64, r6);
					r8 = vis_fpadd16(r7, r4);
					buf1[i] = r5;
					x += dx;
					tdp[i] = vis_fpack16(r8);
					x64 =
					    vis_fand(vis_fpadd16(x64, xdx),
					    mask_7fff);
					rx64 = vis_fpsub16(mask_7fff, x64);
				}
			} else {
#pragma pipeloop(0)
				for (i = 0; i < width; i++) {
					cx = (x >> (MLIB_SHIFT - 1)) & ~1;
					a0 = colortable[*(mlib_s16 *)(tsp +
					    cx)];
					a1 = colortable[*(mlib_s16 *)(tsp + cx +
					    2)];
					a2 = colortable[*(mlib_s16 *)(tsp + cx +
					    2 * src_stride + 2)];
					a3 = colortable[*(mlib_s16 *)(tsp + cx +
					    2 * src_stride)];
					r0 = vis_fmul8x16(a0, rx64);
					r1 = vis_fmul8x16(a1, x64);
					r2 = vis_fmul8x16(a2, x64);
					r3 = vis_fmul8x16(a3, rx64);
					r4 = vis_fpadd16(r0, r1);
					r5 = vis_fpadd16(r2, r3);
					r6 = vis_fpsub16(r5, r4);
					r7 = vis_fmul8x16(fy64, r6);
					r8 = vis_fpadd16(r7, r4);
					buf0[i] = r4;
					buf1[i] = r5;
					x += dx;
					tdp[i] = vis_fpack16(r8);
					x64 =
					    vis_fand(vis_fpadd16(x64, xdx),
					    mask_7fff);
					rx64 = vis_fpsub16(mask_7fff, x64);
				}
			}
		}

		func(dpbuf, dp, width, colormap);

		y += dy;
		dp += dst_stride;
		tsp = (void *)(sp + (y >> MLIB_SHIFT) * src_stride);
	}

	if (buffer != buff)
		__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
