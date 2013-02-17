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

#pragma ident	"@(#)mlib_c_ImageZoom_BC_S32.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *   Internal functions for mlib_ImageZoom on MLIB_BICUBIC filter
 *   and S32 data type
 */

#include <mlib_image.h>
#include <mlib_ImageZoom.h>
#include <mlib_ImageFilters.h>

/* *********************************************************** */

#define	FUNC_NAME(CC)	mlib_ImageZoomBicubic_S32_##CC

static mlib_status mlib_ImageZoomBicubic_S32_2c(
    mlib_work_image * param);
static mlib_status mlib_ImageZoomBicubic_S32_3c(
    mlib_work_image * param);
static mlib_status mlib_ImageZoomBicubic_S32_4c(
    mlib_work_image * param);

/* *********************************************************** */

#define	BC_S0(x)	(-0.5 * (x) * (x) * (x) + (x) * (x) - 0.5 * (x))
#define	BC_S1(x)	(1.5 * (x) * (x) * (x) - 2.5 * (x) * (x) + 1.0)

#define	BC_S2(x)	(-1.5 * (x) * (x) * (x) + 2.0 * (x) * (x) + 0.5 * (x))

#define	BC_S3(x)	(0.5 * (x) * (x) * (x) - 0.5 * (x) * (x))

/* *********************************************************** */

#define	BC2_S0(x)	(- (x) * (x) * (x) + 2.0 * (x) * (x) - (x))
#define	BC2_S1(x)	((x) * (x) * (x) - 2.0 * (x) * (x) + 1.0)

#define	BC2_S2(x)	(- (x) * (x) * (x) + (x) * (x) + (x))

#define	BC2_S3(x)	((x) * (x) * (x) - (x) * (x))

/* *********************************************************** */

#define	CALCULATION(a0, a1, a2, a3)                                \
	xf0 *tt[a0] + xf1 * tt[a1] + xf2 * tt[a2] + xf3 * tt[a3]

/* *********************************************************** */

#ifndef MLIB_USE_FTOI_CLAMPING

/* *********************************************************** */

#define	STORE(N)                                                \
	if (res##N >= MLIB_S32_MAX)                             \
	    res##N = MLIB_S32_MAX;                              \
	if (res##N <= MLIB_S32_MIN)                             \
	    res##N = MLIB_S32_MIN;                              \
	tdp[N] = (mlib_s32)res##N

#else /* MLIB_USE_FTOI_CLAMPING */

#define	STORE(N)	tdp[N] = (mlib_s32) res##N

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

mlib_status FUNC_NAME(
    1)  (
    mlib_work_image * param)
{
	mlib_s32 *sp = GetElemSubStruct(current, sp),
	    *dp = GetElemSubStruct(current, dp);
	mlib_d64 ddx, ddy, xf0, xf1, xf2, xf3, yf0, yf1, yf2, yf3;
	mlib_s32 i, j, cx,
	    filter = GetElemStruct(filter),
	    y,
	    src_stride = GetElemStruct(src_stride),
	    dst_stride = GetElemStruct(dst_stride),
	    width = GetElemSubStruct(current, width),
	    height = GetElemSubStruct(current, height);
	mlib_d64 k00, k10, k20, k30, res0;
	mlib_s32 *tsp = NULL, *tdp, *tt;
	mlib_d64 *buff, *buff_arr[8], *pbuff, *buff0, *buff1, *buff2, *buff3,
	    *xbuff;
	mlib_s32 *cbuff, k, bind = 0, y_step = 4;
	mlib_d64 xd = GetElemStruct(xstart),
	    yd = GetElemStruct(ystart),
	    adx = 1.0 / GetElemStruct(zoomx), ady = 1.0 / GetElemStruct(zoomy);

	buff = __mlib_malloc(width * 9 * sizeof (mlib_d64));

	if (buff == NULL)
		return (MLIB_FAILURE);
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k] = buff + k * width;
	xbuff = buff + 4 * width;
	cbuff = (mlib_s32 *)(xbuff + 4 * width);

	if (filter == MLIB_BICUBIC) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = (mlib_s32)xd;
			ddx = xd - cx;

			cbuff[i] = cx;
			xbuff[4 * i] = BC_S0(ddx);
			xbuff[4 * i + 1] = BC_S1(ddx);
			xbuff[4 * i + 2] = BC_S2(ddx);
			xbuff[4 * i + 3] = BC_S3(ddx);

			xd += adx;
		}

	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = (mlib_s32)xd;
			ddx = xd - cx;

			cbuff[i] = cx;
			xbuff[4 * i] = BC2_S0(ddx);
			xbuff[4 * i + 1] = BC2_S1(ddx);
			xbuff[4 * i + 2] = BC2_S2(ddx);
			xbuff[4 * i + 3] = BC2_S3(ddx);

			xd += adx;
		}
	}

	sp -= src_stride + 1;

	for (j = 0; j < height; j++) {
		y = (mlib_s32)yd;
		ddy = yd - y;

		if (filter == MLIB_BICUBIC) {
			yf0 = BC_S0(ddy);
			yf1 = BC_S1(ddy);
			yf2 = BC_S2(ddy);
			yf3 = BC_S3(ddy);
		} else {
			yf0 = BC2_S0(ddy);
			yf1 = BC2_S1(ddy);
			yf2 = BC2_S2(ddy);
			yf3 = BC2_S3(ddy);
		}

		if (y_step >= 4) {
			y_step = 4;
			tsp = sp + y * src_stride;
		} else {
			bind += y_step;

			if (bind >= 4)
				bind -= 4;
		}

		for (k = 4 - y_step; k < 4; k++) {
			pbuff = buff_arr[bind + k];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				xf0 = xbuff[4 * i];
				xf1 = xbuff[4 * i + 1];
				xf2 = xbuff[4 * i + 2];
				xf3 = xbuff[4 * i + 3];

				tt = tsp + cbuff[i];

				pbuff[i] = CALCULATION(0, 1, 2, 3);
			}

			tsp += src_stride;
		}

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];
		tdp = dp;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			k00 = buff0[i];
			k10 = buff1[i];
			k20 = buff2[i];
			k30 = buff3[i];

			res0 = k00 * yf0 + k10 * yf1 + k20 * yf2 + k30 * yf3;
			STORE(0);

			tdp++;
		}

		y_step = (mlib_s32)(yd + ady) - y;
		yd += ady;
		dp += dst_stride;
	}

	__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    2c) (
    mlib_work_image * param)
{
	mlib_s32 *sp = GetElemSubStruct(current, sp),
	    *dp = GetElemSubStruct(current, dp);
	mlib_d64 ddx, ddy, xf0, xf1, xf2, xf3, yf0, yf1, yf2, yf3;
	mlib_s32 i, j, cx,
	    filter = GetElemStruct(filter),
	    y,
	    src_stride = GetElemStruct(src_stride),
	    dst_stride = GetElemStruct(dst_stride),
	    width = GetElemSubStruct(current, width),
	    height = GetElemSubStruct(current, height);
	mlib_d64 k00, k10, k20, k30, res0;
	mlib_s32 *tsp = NULL, *tdp, *tt;
	mlib_d64 *buff, *buff_arr[8], *pbuff, *buff0, *buff1, *buff2, *buff3,
	    *xbuff;
	mlib_s32 *cbuff, k, bind = 0, y_step = 4;
	mlib_d64 xd = GetElemStruct(xstart),
	    yd = GetElemStruct(ystart),
	    adx = 1.0 / GetElemStruct(zoomx), ady = 1.0 / GetElemStruct(zoomy);

	buff = __mlib_malloc(width * 9 * sizeof (mlib_d64));

	if (buff == NULL)
		return (MLIB_FAILURE);
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k] = buff + k * width;
	xbuff = buff + 4 * width;
	cbuff = (mlib_s32 *)(xbuff + 4 * width);

	if (filter == MLIB_BICUBIC) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = (mlib_s32)xd;
			ddx = xd - cx;

			cbuff[i] = 2 * cx;
			xbuff[4 * i] = BC_S0(ddx);
			xbuff[4 * i + 1] = BC_S1(ddx);
			xbuff[4 * i + 2] = BC_S2(ddx);
			xbuff[4 * i + 3] = BC_S3(ddx);

			xd += adx;
		}

	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = (mlib_s32)xd;
			ddx = xd - cx;

			cbuff[i] = 2 * cx;
			xbuff[4 * i] = BC2_S0(ddx);
			xbuff[4 * i + 1] = BC2_S1(ddx);
			xbuff[4 * i + 2] = BC2_S2(ddx);
			xbuff[4 * i + 3] = BC2_S3(ddx);

			xd += adx;
		}
	}

	sp -= src_stride + 2;

	for (j = 0; j < height; j++) {
		y = (mlib_s32)yd;
		ddy = yd - y;

		if (filter == MLIB_BICUBIC) {
			yf0 = BC_S0(ddy);
			yf1 = BC_S1(ddy);
			yf2 = BC_S2(ddy);
			yf3 = BC_S3(ddy);
		} else {
			yf0 = BC2_S0(ddy);
			yf1 = BC2_S1(ddy);
			yf2 = BC2_S2(ddy);
			yf3 = BC2_S3(ddy);
		}

		if (y_step >= 4) {
			y_step = 4;
			tsp = sp + y * src_stride;
		} else {
			bind += y_step;

			if (bind >= 4)
				bind -= 4;
		}

		for (k = 4 - y_step; k < 4; k++) {
			pbuff = buff_arr[bind + k];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				xf0 = xbuff[4 * i];
				xf1 = xbuff[4 * i + 1];
				xf2 = xbuff[4 * i + 2];
				xf3 = xbuff[4 * i + 3];

				tt = tsp + cbuff[i];

				pbuff[i] = CALCULATION(0, 2, 4, 6);
			}

			tsp += src_stride;
		}

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];
		tdp = dp;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			k00 = buff0[i];
			k10 = buff1[i];
			k20 = buff2[i];
			k30 = buff3[i];

			res0 = k00 * yf0 + k10 * yf1 + k20 * yf2 + k30 * yf3;
			STORE(0);

			tdp += 2;
		}

		y_step = (mlib_s32)(yd + ady) - y;
		yd += ady;
		dp += dst_stride;
	}

	__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#ifdef _MSC_VER
#pragma optimize("", off)
#endif /* _MSC_VER */

mlib_status FUNC_NAME(
    3c) (
    mlib_work_image * param)
{
	mlib_s32 *sp = GetElemSubStruct(current, sp),
	    *dp = GetElemSubStruct(current, dp);
	mlib_d64 ddx, ddy, xf0, xf1, xf2, xf3, yf0, yf1, yf2, yf3;
	mlib_s32 i, j, cx,
	    filter = GetElemStruct(filter),
	    y,
	    src_stride = GetElemStruct(src_stride),
	    dst_stride = GetElemStruct(dst_stride),
	    width = GetElemSubStruct(current, width),
	    height = GetElemSubStruct(current, height);
	mlib_d64 k00, k10, k20, k30, res0;
	mlib_s32 *tsp = NULL, *tdp, *tt;

	mlib_d64 *buff, *buff_arr[8], *pbuff, *buff0, *buff1, *buff2, *buff3,
	    *xbuff;
	mlib_s32 *cbuff, k, bind = 0, y_step = 4;
	mlib_d64 xd = GetElemStruct(xstart),
	    yd = GetElemStruct(ystart),
	    adx = 1.0 / GetElemStruct(zoomx), ady = 1.0 / GetElemStruct(zoomy);

	buff = __mlib_malloc(width * 9 * sizeof (mlib_d64));

	if (buff == NULL)
		return (MLIB_FAILURE);
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k] = buff + k * width;
	xbuff = buff + 4 * width;
	cbuff = (mlib_s32 *)(xbuff + 4 * width);

	if (filter == MLIB_BICUBIC) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = (mlib_s32)xd;
			ddx = xd - cx;

			cbuff[i] = 2 * cx + cx;
			xbuff[4 * i] = BC_S0(ddx);
			xbuff[4 * i + 1] = BC_S1(ddx);
			xbuff[4 * i + 2] = BC_S2(ddx);
			xbuff[4 * i + 3] = BC_S3(ddx);

			xd += adx;
		}

	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = (mlib_s32)xd;
			ddx = xd - cx;

			cbuff[i] = 2 * cx + cx;
			xbuff[4 * i] = BC2_S0(ddx);
			xbuff[4 * i + 1] = BC2_S1(ddx);
			xbuff[4 * i + 2] = BC2_S2(ddx);
			xbuff[4 * i + 3] = BC2_S3(ddx);

			xd += adx;
		}
	}

	sp -= src_stride + 3;

	for (j = 0; j < height; j++) {
		y = (mlib_s32)yd;
		ddy = yd - y;

		if (filter == MLIB_BICUBIC) {
			yf0 = BC_S0(ddy);
			yf1 = BC_S1(ddy);
			yf2 = BC_S2(ddy);
			yf3 = BC_S3(ddy);
		} else {
			yf0 = BC2_S0(ddy);
			yf1 = BC2_S1(ddy);
			yf2 = BC2_S2(ddy);
			yf3 = BC2_S3(ddy);
		}

		if (y_step >= 4) {
			y_step = 4;
			tsp = sp + y * src_stride;
		} else {
			bind += y_step;

			if (bind >= 4)
				bind -= 4;
		}

		for (k = 4 - y_step; k < 4; k++) {
			pbuff = buff_arr[bind + k];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				xf0 = xbuff[4 * i];
				xf1 = xbuff[4 * i + 1];
				xf2 = xbuff[4 * i + 2];
				xf3 = xbuff[4 * i + 3];

				tt = tsp + cbuff[i];

				pbuff[i] = CALCULATION(0, 3, 6, 9);
			}

			tsp += src_stride;
		}

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];
		tdp = dp;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			k00 = buff0[i];
			k10 = buff1[i];
			k20 = buff2[i];
			k30 = buff3[i];

			res0 = k00 * yf0 + k10 * yf1 + k20 * yf2 + k30 * yf3;
			STORE(0);

			tdp += 3;
		}

		y_step = (mlib_s32)(yd + ady) - y;
		yd += ady;
		dp += dst_stride;
	}

	__mlib_free(buff);
	return (MLIB_SUCCESS);
}

#ifdef _MSC_VER
#pragma optimize("", on)
#endif /* _MSC_VER */

/* *********************************************************** */

mlib_status FUNC_NAME(
    4c) (
    mlib_work_image * param)
{
	mlib_s32 *sp = GetElemSubStruct(current, sp),
	    *dp = GetElemSubStruct(current, dp);
	mlib_d64 ddx, ddy, xf0, xf1, xf2, xf3, yf0, yf1, yf2, yf3;
	mlib_s32 i, j, cx,
	    filter = GetElemStruct(filter),
	    y,
	    src_stride = GetElemStruct(src_stride),
	    dst_stride = GetElemStruct(dst_stride),
	    width = GetElemSubStruct(current, width),
	    height = GetElemSubStruct(current, height);
	mlib_d64 k00, k10, k20, k30, res0;
	mlib_s32 *tsp = NULL, *tdp, *tt;
	mlib_d64 *buff, *buff_arr[8], *pbuff, *buff0, *buff1, *buff2, *buff3,
	    *xbuff;
	mlib_s32 *cbuff, k, bind = 0, y_step = 4;
	mlib_d64 xd = GetElemStruct(xstart),
	    yd = GetElemStruct(ystart),
	    adx = 1.0 / GetElemStruct(zoomx), ady = 1.0 / GetElemStruct(zoomy);

	buff = __mlib_malloc(width * 9 * sizeof (mlib_d64));

	if (buff == NULL)
		return (MLIB_FAILURE);
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k] = buff + k * width;
	xbuff = buff + 4 * width;
	cbuff = (mlib_s32 *)(xbuff + 4 * width);

	if (filter == MLIB_BICUBIC) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = (mlib_s32)xd;
			ddx = xd - cx;

			cbuff[i] = 4 * cx;
			xbuff[4 * i] = BC_S0(ddx);
			xbuff[4 * i + 1] = BC_S1(ddx);
			xbuff[4 * i + 2] = BC_S2(ddx);
			xbuff[4 * i + 3] = BC_S3(ddx);

			xd += adx;
		}

	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = (mlib_s32)xd;
			ddx = xd - cx;

			cbuff[i] = 4 * cx;
			xbuff[4 * i] = BC2_S0(ddx);
			xbuff[4 * i + 1] = BC2_S1(ddx);
			xbuff[4 * i + 2] = BC2_S2(ddx);
			xbuff[4 * i + 3] = BC2_S3(ddx);

			xd += adx;
		}
	}

	sp -= src_stride + 4;

	for (j = 0; j < height; j++) {
		y = (mlib_s32)yd;
		ddy = yd - y;

		if (filter == MLIB_BICUBIC) {
			yf0 = BC_S0(ddy);
			yf1 = BC_S1(ddy);
			yf2 = BC_S2(ddy);
			yf3 = BC_S3(ddy);
		} else {
			yf0 = BC2_S0(ddy);
			yf1 = BC2_S1(ddy);
			yf2 = BC2_S2(ddy);
			yf3 = BC2_S3(ddy);
		}

		if (y_step >= 4) {
			y_step = 4;
			tsp = sp + y * src_stride;
		} else {
			bind += y_step;

			if (bind >= 4)
				bind -= 4;
		}

		for (k = 4 - y_step; k < 4; k++) {
			pbuff = buff_arr[bind + k];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				xf0 = xbuff[4 * i];
				xf1 = xbuff[4 * i + 1];
				xf2 = xbuff[4 * i + 2];
				xf3 = xbuff[4 * i + 3];

				tt = tsp + cbuff[i];

				pbuff[i] = CALCULATION(0, 4, 8, 12);
			}

			tsp += src_stride;
		}

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];
		tdp = dp;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			k00 = buff0[i];
			k10 = buff1[i];
			k20 = buff2[i];
			k30 = buff3[i];

			res0 = k00 * yf0 + k10 * yf1 + k20 * yf2 + k30 * yf3;
			STORE(0);

			tdp += 4;
		}

		y_step = (mlib_s32)(yd + ady) - y;
		yd += ady;
		dp += dst_stride;
	}

	__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    2)  (
    mlib_work_image * param)
{
	mlib_status res;
	mlib_clipping *current = param->current;

	res = FUNC_NAME(2c) (param);

	if (res != MLIB_SUCCESS)
		return (res);
	current->sp = (mlib_s32 *)current->sp + 1;
	current->dp = (mlib_s32 *)current->dp + 1;
	res = FUNC_NAME(2c) (param);
	current->sp = (mlib_s32 *)current->sp - 1;
	current->dp = (mlib_s32 *)current->dp - 1;
	return (res);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    3)  (
    mlib_work_image * param)
{
	mlib_status res;
	mlib_clipping *current = param->current;

	res = FUNC_NAME(3c) (param);

	if (res != MLIB_SUCCESS)
		return (res);
	current->sp = (mlib_s32 *)current->sp + 1;
	current->dp = (mlib_s32 *)current->dp + 1;
	res = FUNC_NAME(3c) (param);

	if (res != MLIB_SUCCESS)
		return (res);
	current->sp = (mlib_s32 *)current->sp + 1;
	current->dp = (mlib_s32 *)current->dp + 1;
	res = FUNC_NAME(3c) (param);
	current->sp = (mlib_s32 *)current->sp - 2;
	current->dp = (mlib_s32 *)current->dp - 2;
	return (res);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    4)  (
    mlib_work_image * param)
{
	mlib_status res;
	mlib_clipping *current = param->current;

	res = FUNC_NAME(4c) (param);

	if (res != MLIB_SUCCESS)
		return (res);
	current->sp = (mlib_s32 *)current->sp + 1;
	current->dp = (mlib_s32 *)current->dp + 1;
	res = FUNC_NAME(4c) (param);

	if (res != MLIB_SUCCESS)
		return (res);
	current->sp = (mlib_s32 *)current->sp + 1;
	current->dp = (mlib_s32 *)current->dp + 1;
	res = FUNC_NAME(4c) (param);

	if (res != MLIB_SUCCESS)
		return (res);
	current->sp = (mlib_s32 *)current->sp + 1;
	current->dp = (mlib_s32 *)current->dp + 1;
	res = FUNC_NAME(4c) (param);
	current->sp = (mlib_s32 *)current->sp - 3;
	current->dp = (mlib_s32 *)current->dp - 3;
	return (res);
}

/* *********************************************************** */
