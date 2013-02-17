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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_m_ImageZoom_NN_f.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoom - image scaling with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoom(mlib_image  *dst,
 *                                 mlib_image  *src,
 *                                 mlib_f32    zoomx,
 *                                 mlib_f32    zoomy,
 *                                 mlib_filter filter,
 *                                 mlib_edge   edge)
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

/* *********************************************************** */

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <mmintrin.h>
#endif

#include <mlib_image.h>
#define	_MLIB_C_VER_

#include <mlib_ImageZoom.h>
#include <mlib_ImageCopy.h>
#include <mlib_mmx_utils.h>

/* *********************************************************** */

#define	VARIABLE(FORMAT)                                              \
	mlib_s32 buff_loc[BUFF_SIZE], *buff = buff_loc;               \
	mlib_s32 i, j, dx = GetElemStruct(DX), dy =                   \
	GetElemStruct(DY), x = GetElemSubStruct(current, srcX), y =   \
	GetElemSubStruct(current, srcY), src_stride =                 \
	GetElemStruct(src_stride), dst_stride =                       \
	GetElemStruct(dst_stride), width =                            \
	GetElemSubStruct(current, width), height =                    \
	GetElemSubStruct(current, height);                            \
	mlib_u8 *tsp;                                                 \
	FORMAT *sp = GetElemSubStruct(current, sp), *dp =             \
	GetElemSubStruct(current, dp);

/* *********************************************************** */

#define	GET_COORD(N)                                            \
	if (width > BUFF_SIZE) {                                \
	    buff = mlib_malloc(width * sizeof (mlib_s32));      \
	    if (buff == NULL)                                   \
		return (MLIB_FAILURE);                          \
	}                                                       \
	x = GetElemSubStruct(current, srcX) & MLIB_MASK;        \
	for (i = 0; i < width; i++)                             \
	{                                                       \
	buff[i] = (N) * (x >> MLIB_SHIFT);                      \
	x += dx;                                                \
	}                                                       \
	                                                        \
	tsp = (mlib_u8 *)sp;                                    \
	y = GetElemSubStruct(current, srcY) & MLIB_MASK

/* *********************************************************** */

#define	BUFF_SIZE	1024

/* *********************************************************** */

mlib_status
mlib_ImageZoom_U8_1_Nearest(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	mlib_u8 *dl = dp;
	mlib_s32 y0 = -1;

	GET_COORD(1);

	for (j = 0; j < height; j++) {

		if ((y0 >> MLIB_SHIFT) == (y >> MLIB_SHIFT)) {
			mlib_ImageCopy_na((mlib_u8 *)dl - dst_stride, dl,
			    width);
		} else {
			mlib_u8 *dp = dl;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			i = 0;
			if ((mlib_addr)dp & 1) {
				(*dp++) = tsp[buff[i++]];
			}

			for (; i <= width - 4; i += 4) {
				*(mlib_s16 *)dp =
				    (tsp[buff[i + 1]] << 8) | tsp[buff[i]];
				*(mlib_s16 *)(dp + 2) =
				    (tsp[buff[i + 3]] << 8) | tsp[buff[i + 2]];
				dp += 4;
			}

			for (; i < width; i++) {
				(*dp++) = tsp[buff[i]];
			}
		}

		y0 = y;
		y += dy;
		dl += dst_stride;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	if (buff != buff_loc) {
		mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_U8_3_Nearest(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	mlib_u8 *tdp;
	mlib_s32 tail, y0 = -1;

	GET_COORD(3);

	for (j = 0; j < height; j++) {

		if ((y >> MLIB_SHIFT) == (y0 >> MLIB_SHIFT)) {
			mlib_ImageCopy_na((mlib_u8 *)dp - dst_stride, dp,
			    3 * width);
		} else {
			tdp = dp;
			tail = tdp[3 * width];

			for (i = 0; i < width; i++) {
				*(mlib_s32 *)tdp = *(mlib_s32 *)(tsp + buff[i]);
				tdp += 3;
			}

			tdp[0] = tail;
		}

		y0 = y;
		y += dy;
		dp = (mlib_u8 *)dp + dst_stride;
		tsp = (mlib_u8 *)sp + ((y >> MLIB_SHIFT) * src_stride);
	}

	if (buff != buff_loc) {
		mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_S16_1_Nearest(
    mlib_work_image * param)
{
	VARIABLE(mlib_u16)
	mlib_s32 y0 = -1;

	GET_COORD(2);

	for (j = 0; j < height; j++) {

		if ((y >> MLIB_SHIFT) == (y0 >> MLIB_SHIFT)) {
			mlib_ImageCopy_na((mlib_u8 *)dp - dst_stride,
			    (void *)dp, 2 * width);
		} else {

			for (i = 0; i < width; i++) {
				dp[i] = *(mlib_u16 *)(tsp + buff[i]);
			}
		}

		y0 = y;
		y += dy;
		dp = (void *)((mlib_u8 *)dp + dst_stride);
		tsp = (mlib_u8 *)sp + ((y >> MLIB_SHIFT) * src_stride);
	}

	if (buff != buff_loc) {
		mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_S16_3_Nearest(
    mlib_work_image * param)
{
	VARIABLE(mlib_u16)
	mlib_u8 *tdp;
	mlib_s32 tail, y0 = -1;

	GET_COORD(6);

	for (j = 0; j < height; j++) {

		if ((y >> MLIB_SHIFT) == (y0 >> MLIB_SHIFT)) {
			mlib_ImageCopy_na((mlib_u8 *)dp - dst_stride,
			    (void *)dp, 6 * width);
		} else {
			tdp = (mlib_u8 *)dp;
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			tail = *(mlib_s16 *)(tdp + 6 * width);

			for (i = 0; i < width; i++) {
				*(__m64 *) tdp = *(__m64 *) (tsp + buff[i]);
				tdp += 6;
			}

			*(mlib_s16 *)tdp = tail;
		}

		y0 = y;
		y += dy;
		dp = (void *)((mlib_u8 *)dp + dst_stride);
		tsp = (mlib_u8 *)sp + ((y >> MLIB_SHIFT) * src_stride);
	}

	_mm_empty();

	if (buff != buff_loc) {
		mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_S32_1_Nearest(
    mlib_work_image * param)
{
	VARIABLE(mlib_s32)
	mlib_s32 s0, s1, y0 = -1;

	GET_COORD(4);

	for (j = 0; j < height; j++) {

		if ((y >> MLIB_SHIFT) == (y0 >> MLIB_SHIFT)) {
			mlib_ImageCopy_na((mlib_u8 *)dp - dst_stride,
			    (void *)dp, 4 * width);
		} else {

			for (i = 0; i <= width - 2; i += 2) {
				s0 = *(mlib_s32 *)(tsp + buff[i]);
				s1 = *(mlib_s32 *)(tsp + buff[i + 1]);
				dp[i] = s0;
				dp[i + 1] = s1;
			}

			if (i < width) {
				dp[i] = *(mlib_u32 *)(tsp + buff[i]);
			}
		}

		y0 = y;
		y += dy;
		dp = (void *)((mlib_u8 *)dp + dst_stride);
		tsp = (mlib_u8 *)sp + ((y >> MLIB_SHIFT) * src_stride);
	}

	if (buff != buff_loc) {
		mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_S32_3_Nearest(
    mlib_work_image * param)
{
	VARIABLE(mlib_s32)
	mlib_s32 cx, y_step = -1, tmp0, tmp1, tmp2;

	GET_COORD(12);

	for (j = 0; j < height; j++) {

		if (!y_step) {
			mlib_ImageCopy_na((mlib_u8 *)dp - dst_stride,
			    (void *)dp, 12 * width);
		} else {
			cx = buff[0];
			tmp0 = *(mlib_s32 *)((mlib_u8 *)sp + cx);
			tmp1 = *(mlib_s32 *)((mlib_u8 *)sp + cx + 4);
			tmp2 = *(mlib_s32 *)((mlib_u8 *)sp + cx + 8);

			cx = buff[1];

			for (i = 0; i < width - 1; i++) {
				dp[3 * i + 0] = tmp0;
				dp[3 * i + 1] = tmp1;
				dp[3 * i + 2] = tmp2;

				tmp0 = *(mlib_s32 *)((mlib_u8 *)sp + cx);
				tmp1 = *(mlib_s32 *)((mlib_u8 *)sp + cx + 4);
				tmp2 = *(mlib_s32 *)((mlib_u8 *)sp + cx + 8);

				cx = buff[i + 2];
			}

			dp[3 * i + 0] = tmp0;
			dp[3 * i + 1] = tmp1;
			dp[3 * i + 2] = tmp2;
		}

		y_step = ((y + dy) - (y & ~MLIB_MASK)) >> MLIB_SHIFT;
		y += dy;

		dp = (void *)((mlib_u8 *)dp + dst_stride);
		sp = (void *)((mlib_u8 *)sp + y_step * src_stride);
	}

	if (buff != buff_loc) {
		mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_D64_1_Nearest(
    mlib_work_image * param)
{
	VARIABLE(__m64)
	mlib_s32 y_step;

	GET_COORD(8);

	for (j = 0; j < height; j++) {
		for (i = 0; i <= width - 2; i += 2) {
			dp[i] = *(__m64 *) ((mlib_u8 *)sp + buff[i]);
			dp[i + 1] = *(__m64 *) ((mlib_u8 *)sp + buff[i + 1]);
		}

		if (i < width) {
			dp[i] = *(__m64 *) ((mlib_u8 *)sp + buff[i]);
		}

		y_step = ((y + dy) - (y & ~MLIB_MASK)) >> MLIB_SHIFT;
		y += dy;

		dp = (void *)((mlib_u8 *)dp + dst_stride);
		sp = (void *)((mlib_u8 *)sp + y_step * src_stride);
	}

	_m_empty();

	if (buff != buff_loc) {
		mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_D64_2_Nearest(
    mlib_work_image * param)
{
	VARIABLE(__m64)
	__m64 tmp, tmp1;
	mlib_s32 cx, y_step;

	GET_COORD(2 * 8);

	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			cx = buff[i];
			tmp = *(__m64 *) ((mlib_u8 *)sp + cx);
			tmp1 = *(__m64 *) ((mlib_u8 *)sp + cx + 8);
			dp[2 * i] = tmp;
			dp[2 * i + 1] = tmp1;
		}

		y_step = ((y + dy) - (y & ~MLIB_MASK)) >> MLIB_SHIFT;
		y += dy;

		dp = (void *)((mlib_u8 *)dp + dst_stride);
		sp = (void *)((mlib_u8 *)sp + y_step * src_stride);
	}

	_m_empty();

	if (buff != buff_loc) {
		mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_D64_3_Nearest(
    mlib_work_image * param)
{
	VARIABLE(__m64)
	__m64 tmp, tmp1, tmp2;
	mlib_s32 cx, y_step;

	GET_COORD(3 * 8);

	for (j = 0; j < height; j++) {

		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		cx = buff[0];
		tmp = *(__m64 *) ((mlib_u8 *)sp + cx);
		tmp1 = *(__m64 *) ((mlib_u8 *)sp + cx + 8);
		tmp2 = *(__m64 *) ((mlib_u8 *)sp + cx + 16);

		cx = buff[1];

		for (i = 0; i < width - 1; i++) {
			dp[3 * i] = tmp;
			dp[3 * i + 1] = tmp1;
			dp[3 * i + 2] = tmp2;
			tmp = *(__m64 *) ((mlib_u8 *)sp + cx);
			tmp1 = *(__m64 *) ((mlib_u8 *)sp + cx + 8);
			tmp2 = *(__m64 *) ((mlib_u8 *)sp + cx + 16);
			cx = buff[i + 2];
		}

		dp[3 * i] = tmp;
		dp[3 * i + 1] = tmp1;
		dp[3 * i + 2] = tmp2;

		y_step = ((y + dy) - (y & ~MLIB_MASK)) >> MLIB_SHIFT;
		y += dy;

		dp = (void *)((mlib_u8 *)dp + dst_stride);
		sp = (void *)((mlib_u8 *)sp + y_step * src_stride);
	}

	_m_empty();

	if (buff != buff_loc) {
		mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_D64_4_Nearest(
    mlib_work_image * param)
{
	VARIABLE(__m64)
	__m64 tmp, tmp1, tmp2, tmp3;
	mlib_s32 cx, y_step;

	GET_COORD(4 * 8);

	for (j = 0; j < height; j++) {

		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		cx = buff[0];
		tmp = *(__m64 *) ((mlib_u8 *)sp + cx);
		tmp1 = *(__m64 *) ((mlib_u8 *)sp + cx + 8);
		tmp2 = *(__m64 *) ((mlib_u8 *)sp + cx + 16);
		tmp3 = *(__m64 *) ((mlib_u8 *)sp + cx + 24);

		cx = buff[1];

		for (i = 0; i < width - 1; i++) {

			dp[4 * i] = tmp;
			dp[4 * i + 1] = tmp1;
			dp[4 * i + 2] = tmp2;
			dp[4 * i + 3] = tmp3;
			tmp = *(__m64 *) ((mlib_u8 *)sp + cx);
			tmp1 = *(__m64 *) ((mlib_u8 *)sp + cx + 8);
			tmp2 = *(__m64 *) ((mlib_u8 *)sp + cx + 16);
			tmp3 = *(__m64 *) ((mlib_u8 *)sp + cx + 24);
			cx = buff[i + 2];
		}

		dp[4 * i] = tmp;
		dp[4 * i + 1] = tmp1;
		dp[4 * i + 2] = tmp2;
		dp[4 * i + 3] = tmp3;

		y_step = ((y + dy) - (y & ~MLIB_MASK)) >> MLIB_SHIFT;
		y += dy;

		dp = (void *)((mlib_u8 *)dp + dst_stride);
		sp = (void *)((mlib_u8 *)sp + y_step * src_stride);
	}

	_m_empty();

	if (buff != buff_loc) {
		mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
