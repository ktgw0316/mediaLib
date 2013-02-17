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

#pragma ident	"@(#)mlib_c_ImageZoom_NN_f.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoom - image scaling with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoom(mlib_image       *dst,
 *                                 const mlib_image *src,
 *                                 mlib_f32         zoomx,
 *                                 mlib_f32         zoomy,
 *                                 mlib_filter      filter,
 *                                 mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image
 *      src       Pointer to source image
 *      zoomx     X zoom factor.
 *      zoomy     Y zoom factor.
 *      filter    Type of resampling filter.
 *      edge      Type of edge condition.
 *
 * DESCRIPTION
 *  The center of the source image is mapped to the center of the
 *  destination image.
 *
 *  The upper-left corner pixel of an image is located at (0.5, 0.5).
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

#include <mlib_image.h>

/* *********************************************************** */

#define	_MLIB_C_VER_

#include <mlib_ImageZoom.h>

/* *********************************************************** */

#define	VARIABLE(FORMAT)                                             \
	mlib_s32 j,                                                  \
		dx = GetElemStruct(DX),                              \
		dy = GetElemStruct(DY),                              \
		x = GetElemSubStruct(current, srcX),                 \
		y = GetElemSubStruct(current, srcY),                 \
		src_stride = GetElemStruct(src_stride),              \
		dst_stride = GetElemStruct(dst_stride),              \
		width  = GetElemSubStruct(current, width),           \
		height = GetElemSubStruct(current, height);          \
	mlib_u8 *tsp;                                                \
	FORMAT *sp = GetElemSubStruct(current, sp),                  \
		*dp = GetElemSubStruct(current, dp)

/* *********************************************************** */

mlib_status
mlib_ImageZoom_U8_1_Nearest(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8);
	mlib_u8 *dl = dp;
	mlib_s32 y0 = -1;

	tsp = sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {

		if ((y0 >> MLIB_SHIFT) == (y >> MLIB_SHIFT)) {
			mlib_ImageCopy_na((mlib_u8 *)dl - dst_stride, dl,
			    width);
		} else {
			mlib_u8 *dp = dl, *dend = dl + width;
			mlib_s32 s0, s1;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			if ((mlib_addr)dp & 1) {
				(*dp++) = tsp[x >> MLIB_SHIFT];
				x += dx;
			}

			for (; dp <= dend - 2; dp += 2) {
				s0 = tsp[x >> MLIB_SHIFT];
				x += dx;
				s1 = tsp[x >> MLIB_SHIFT];
				x += dx;
#ifdef _LITTLE_ENDIAN
				*(mlib_s16 *)dp = (s1 << 8) | s0;
#else /* _LITTLE_ENDIAN */
				*(mlib_s16 *)dp = (s0 << 8) | s1;
#endif /* _LITTLE_ENDIAN */
			}

			if (dp < dend) {
				(*dp++) = tsp[x >> MLIB_SHIFT];
			}
		}

		y0 = y;
		y += dy;
		dl += dst_stride;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_U8_3_Nearest(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8);
	mlib_s32 i;
	mlib_u8 *tdp;
	mlib_u8 tmp0, tmp1, tmp2;
	mlib_s32 cx, y0 = -1;

	tsp = sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {

		if ((y >> MLIB_SHIFT) == (y0 >> MLIB_SHIFT)) {
			mlib_ImageCopy_na((mlib_u8 *)dp - dst_stride, dp,
			    3 * width);
		} else {
			tdp = dp;
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			cx = (x >> MLIB_SHIFT) * 3;
			tmp0 = tsp[cx];
			tmp1 = tsp[cx + 1];
			tmp2 = tsp[cx + 2];

			for (i = 0; i < width - 1; i++, tdp += 3) {

				tdp[0] = tmp0;
				tdp[1] = tmp1;
				tdp[2] = tmp2;
				x += dx;
				cx = (x >> MLIB_SHIFT) * 3;
				tmp0 = tsp[cx];
				tmp1 = tsp[cx + 1];
				tmp2 = tsp[cx + 2];
			}

			tdp[0] = tmp0;
			tdp[1] = tmp1;
			tdp[2] = tmp2;
		}

		y0 = y;
		y += dy;
		dp = (mlib_u8 *)dp + dst_stride;
		tsp = (mlib_u8 *)sp + ((y >> MLIB_SHIFT) * src_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_S16_1_Nearest(
    mlib_work_image * param)
{
	VARIABLE(mlib_u16);
	mlib_s32 i;
	mlib_u8 *tdp;
	mlib_u16 tmp;
	mlib_s32 cx, y0 = -1;

	tsp = (mlib_u8 *)sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {

		if ((y >> MLIB_SHIFT) == (y0 >> MLIB_SHIFT)) {
			mlib_ImageCopy_na((mlib_u8 *)dp - dst_stride,
			    (void *)dp, 2 * width);
		} else {
			tdp = (mlib_u8 *)dp;
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			cx = (x >> (MLIB_SHIFT - 1)) & ~1;
			tmp = *(mlib_u16 *)(tsp + cx);

			for (i = 0; i < width - 1; i++, tdp += 2) {

				*(mlib_u16 *)tdp = tmp;
				x += dx;
				cx = (x >> (MLIB_SHIFT - 1)) & ~1;
				tmp = *(mlib_u16 *)(tsp + cx);
			}

			*(mlib_u16 *)tdp = tmp;
		}

		y0 = y;
		y += dy;
		dp = (void *)((mlib_u8 *)dp + dst_stride);
		tsp = (mlib_u8 *)sp + ((y >> MLIB_SHIFT) * src_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_S16_3_Nearest(
    mlib_work_image * param)
{
	VARIABLE(mlib_u16);
	mlib_s32 i;
	mlib_u8 *tdp;
	mlib_u16 tmp0, tmp1, tmp2;
	mlib_s32 cx, y0 = -1;

	tsp = (mlib_u8 *)sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {

		if ((y >> MLIB_SHIFT) == (y0 >> MLIB_SHIFT)) {
			mlib_ImageCopy_na((mlib_u8 *)dp - dst_stride,
			    (void *)dp, 6 * width);
		} else {
			tdp = (mlib_u8 *)dp;
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			tmp0 = *(mlib_u16 *)tsp;
			tmp1 = *(mlib_u16 *)(tsp + 2);
			tmp2 = *(mlib_u16 *)(tsp + 4);

			for (i = 0; i < width - 1; i++, tdp += 6) {

				*(mlib_u16 *)tdp = tmp0;
				*(mlib_u16 *)(tdp + 2) = tmp1;
				*(mlib_u16 *)(tdp + 4) = tmp2;
				x += dx;
				cx = (x >> MLIB_SHIFT) * 6;
				tmp0 = *(mlib_u16 *)(tsp + cx);
				tmp1 = *(mlib_u16 *)(tsp + cx + 2);
				tmp2 = *(mlib_u16 *)(tsp + cx + 4);
			}

			*(mlib_u16 *)tdp = tmp0;
			*(mlib_u16 *)(tdp + 2) = tmp1;
			*(mlib_u16 *)(tdp + 4) = tmp2;
		}

		y0 = y;
		y += dy;
		dp = (void *)((mlib_u8 *)dp + dst_stride);
		tsp = (mlib_u8 *)sp + ((y >> MLIB_SHIFT) * src_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
