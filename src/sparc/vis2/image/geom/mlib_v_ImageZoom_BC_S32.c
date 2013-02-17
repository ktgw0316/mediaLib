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

#pragma ident	"@(#)mlib_v_ImageZoom_BC_S32.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *   Internal functions for mlib_ImageZoom on MLIB_BICUBIC filter
 */

#include <mlib_image.h>
#include <mlib_ImageZoom.h>
#include <mlib_ImageFilters.h>

/* *********************************************************** */

#define	DTYPE	mlib_s32
#define	TTYPE	mlib_d64

/* *********************************************************** */

#define	FUNC_NAME(CC)	mlib_ImageZoomBicubic_S32_##CC

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(DST, SRC)	DST = ((mlib_s32)(SRC))

#else /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	FLOAT2INT_CLAMP(DST, SRC)                               \
	{                                                       \
	    mlib_d64 dsrc = (mlib_d64)(SRC);                    \
	                                                        \
	    if (dsrc > (mlib_d64)MLIB_S32_MAX)                  \
		dsrc = (mlib_d64)MLIB_S32_MAX;                  \
	    if (dsrc < (mlib_d64)MLIB_S32_MIN)                  \
		dsrc = (mlib_d64)MLIB_S32_MIN;                  \
	    DST = (mlib_s32)dsrc;                               \
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

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

#define	BC_SUM(NCHAN, k)                                               \
	xf0 *sp[k] + xf1 * sp[k + NCHAN] + xf2 * sp[k + 2 * NCHAN] +   \
	    xf3 * sp[k + 3 * NCHAN]

/* *********************************************************** */

#undef  NCHAN
#define	NCHAN	1

mlib_status FUNC_NAME(1)  (
    mlib_work_image *param)
{
	DTYPE *sl = GetElemSubStruct(current, sp);
	DTYPE *dl = GetElemSubStruct(current, dp);
	TTYPE xf0, xf1, xf2, xf3;
	TTYPE yf0, yf1, yf2, yf3;
	TTYPE ddx, ddy, h0, h1, h2, h3, res;
	TTYPE *buff, *buff_arr[8], *pbuff, *buff0, *buff1, *buff2, *buff3,
	    *xbuff;
	mlib_s32 i, j, k, x, y, y_step = 4, filter =
	    GetElemStruct(filter), src_stride =
	    GetElemStruct(src_stride), dst_stride =
	    GetElemStruct(dst_stride), width =
	    GetElemSubStruct(current, width), height =
	    GetElemSubStruct(current, height);
	mlib_s32 *cbuff, bind = 0;
	TTYPE xd = GetElemStruct(xstart),
	    yd = GetElemStruct(ystart),
	    adx = 1.0 / GetElemStruct(zoomx), ady = 1.0 / GetElemStruct(zoomy);

	buff =
	    __mlib_malloc(width * (4 * NCHAN + 4) * sizeof (TTYPE) +
	    width * sizeof (mlib_s32));

	if (buff == NULL)
		return (MLIB_FAILURE);
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k] = buff + k * (NCHAN * width);
	xbuff = buff + 4 * NCHAN * width;
	cbuff = (mlib_s32 *)(xbuff + 4 * width);

	if (filter == MLIB_BICUBIC) {
#pragma pipeloop(0)
		for (i = 0; i < width; i++) {
			x = (mlib_s32)xd;
			ddx = xd - x;

			cbuff[i] = NCHAN * x;
			xbuff[4 * i] = BC_S0(ddx);
			xbuff[4 * i + 1] = BC_S1(ddx);
			xbuff[4 * i + 2] = BC_S2(ddx);
			xbuff[4 * i + 3] = BC_S3(ddx);

			xd += adx;
		}

	} else {
#pragma pipeloop(0)
		for (i = 0; i < width; i++) {
			x = (mlib_s32)xd;
			ddx = xd - x;

			cbuff[i] = NCHAN * x;
			xbuff[4 * i] = BC2_S0(ddx);
			xbuff[4 * i + 1] = BC2_S1(ddx);
			xbuff[4 * i + 2] = BC2_S2(ddx);
			xbuff[4 * i + 3] = BC2_S3(ddx);

			xd += adx;
		}
	}

	sl -= src_stride + NCHAN;
	y = (mlib_s32)yd;
	sl += y * src_stride;

	for (j = 0; j < height; j++) {
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

		for (k = 4 - y_step; k < 4; k++) {
			pbuff = buff_arr[bind + k];
#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				DTYPE *sp = sl + cbuff[i];

				xf0 = xbuff[4 * i];
				xf1 = xbuff[4 * i + 1];
				xf2 = xbuff[4 * i + 2];
				xf3 = xbuff[4 * i + 3];

				pbuff[i] = BC_SUM(NCHAN, 0);
			}

			sl += src_stride;
		}

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];
#pragma pipeloop(0)
		for (i = 0; i < NCHAN * width; i++) {
			h0 = buff0[i];
			h1 = buff1[i];
			h2 = buff2[i];
			h3 = buff3[i];

			res = yf0 * h0 + yf1 * h1 + yf2 * h2 + yf3 * h3;
			FLOAT2INT_CLAMP(dl[i], res);
		}

		yd += ady;
		y_step = (mlib_s32)yd - y;
		y = (mlib_s32)yd;
		bind = (bind + y_step) & 3;

		if (y_step > 4) {
			sl += (y_step - 4) * src_stride;
			y_step = 4;
		}

		dl += dst_stride;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  NCHAN
#define	NCHAN	2

mlib_status FUNC_NAME(2)  (
    mlib_work_image *param)
{
	DTYPE *sl = GetElemSubStruct(current, sp);
	DTYPE *dl = GetElemSubStruct(current, dp);
	TTYPE xf0, xf1, xf2, xf3;
	TTYPE yf0, yf1, yf2, yf3;
	TTYPE ddx, ddy, h0, h1, h2, h3, res;
	TTYPE *buff, *buff_arr[8], *pbuff, *buff0, *buff1, *buff2, *buff3,
	    *xbuff;
	mlib_s32 i, j, k, x, y, y_step = 4, filter =
	    GetElemStruct(filter), src_stride =
	    GetElemStruct(src_stride), dst_stride =
	    GetElemStruct(dst_stride), width =
	    GetElemSubStruct(current, width), height =
	    GetElemSubStruct(current, height);
	mlib_s32 *cbuff, bind = 0;
	TTYPE xd = GetElemStruct(xstart),
	    yd = GetElemStruct(ystart),
	    adx = 1.0 / GetElemStruct(zoomx), ady = 1.0 / GetElemStruct(zoomy);

	buff =
	    __mlib_malloc(width * (4 * NCHAN + 4) * sizeof (TTYPE) +
	    width * sizeof (mlib_s32));

	if (buff == NULL)
		return (MLIB_FAILURE);
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k] = buff + k * (NCHAN * width);
	xbuff = buff + 4 * NCHAN * width;
	cbuff = (mlib_s32 *)(xbuff + 4 * width);

	if (filter == MLIB_BICUBIC) {
#pragma pipeloop(0)
		for (i = 0; i < width; i++) {
			x = (mlib_s32)xd;
			ddx = xd - x;

			cbuff[i] = NCHAN * x;
			xbuff[4 * i] = BC_S0(ddx);
			xbuff[4 * i + 1] = BC_S1(ddx);
			xbuff[4 * i + 2] = BC_S2(ddx);
			xbuff[4 * i + 3] = BC_S3(ddx);

			xd += adx;
		}

	} else {
#pragma pipeloop(0)
		for (i = 0; i < width; i++) {
			x = (mlib_s32)xd;
			ddx = xd - x;

			cbuff[i] = NCHAN * x;
			xbuff[4 * i] = BC2_S0(ddx);
			xbuff[4 * i + 1] = BC2_S1(ddx);
			xbuff[4 * i + 2] = BC2_S2(ddx);
			xbuff[4 * i + 3] = BC2_S3(ddx);

			xd += adx;
		}
	}

	sl -= src_stride + NCHAN;
	y = (mlib_s32)yd;
	sl += y * src_stride;

	for (j = 0; j < height; j++) {
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

		for (k = 4 - y_step; k < 4; k++) {
			pbuff = buff_arr[bind + k];
#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				DTYPE *sp = sl + cbuff[i];

				xf0 = xbuff[4 * i];
				xf1 = xbuff[4 * i + 1];
				xf2 = xbuff[4 * i + 2];
				xf3 = xbuff[4 * i + 3];

				pbuff[2 * i] = BC_SUM(NCHAN, 0);
				pbuff[2 * i + 1] = BC_SUM(NCHAN, 1);
			}

			sl += src_stride;
		}

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];
#pragma pipeloop(0)
		for (i = 0; i < NCHAN * width; i++) {
			h0 = buff0[i];
			h1 = buff1[i];
			h2 = buff2[i];
			h3 = buff3[i];

			res = yf0 * h0 + yf1 * h1 + yf2 * h2 + yf3 * h3;
			FLOAT2INT_CLAMP(dl[i], res);
		}

		yd += ady;
		y_step = (mlib_s32)yd - y;
		y = (mlib_s32)yd;
		bind = (bind + y_step) & 3;

		if (y_step > 4) {
			sl += (y_step - 4) * src_stride;
			y_step = 4;
		}

		dl += dst_stride;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  NCHAN
#define	NCHAN	3

mlib_status FUNC_NAME(3)  (
    mlib_work_image *param)
{
	DTYPE *sl = GetElemSubStruct(current, sp);
	DTYPE *dl = GetElemSubStruct(current, dp);
	TTYPE xf0, xf1, xf2, xf3;
	TTYPE yf0, yf1, yf2, yf3;
	TTYPE ddx, ddy, h0, h1, h2, h3, res;
	TTYPE *buff, *buff_arr[8], *pbuff, *buff0, *buff1, *buff2, *buff3,
	    *xbuff;
	mlib_s32 i, j, k, x, y, y_step = 4, filter =
	    GetElemStruct(filter), src_stride =
	    GetElemStruct(src_stride), dst_stride =
	    GetElemStruct(dst_stride), width =
	    GetElemSubStruct(current, width), height =
	    GetElemSubStruct(current, height);
	mlib_s32 *cbuff, bind = 0;
	TTYPE xd = GetElemStruct(xstart),
	    yd = GetElemStruct(ystart),
	    adx = 1.0 / GetElemStruct(zoomx), ady = 1.0 / GetElemStruct(zoomy);

	buff =
	    __mlib_malloc(width * (4 * NCHAN + 4) * sizeof (TTYPE) +
	    width * sizeof (mlib_s32));

	if (buff == NULL)
		return (MLIB_FAILURE);
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k] = buff + k * (NCHAN * width);
	xbuff = buff + 4 * NCHAN * width;
	cbuff = (mlib_s32 *)(xbuff + 4 * width);

	if (filter == MLIB_BICUBIC) {
#pragma pipeloop(0)
		for (i = 0; i < width; i++) {
			x = (mlib_s32)xd;
			ddx = xd - x;

			cbuff[i] = NCHAN * x;
			xbuff[4 * i] = BC_S0(ddx);
			xbuff[4 * i + 1] = BC_S1(ddx);
			xbuff[4 * i + 2] = BC_S2(ddx);
			xbuff[4 * i + 3] = BC_S3(ddx);

			xd += adx;
		}

	} else {
#pragma pipeloop(0)
		for (i = 0; i < width; i++) {
			x = (mlib_s32)xd;
			ddx = xd - x;

			cbuff[i] = NCHAN * x;
			xbuff[4 * i] = BC2_S0(ddx);
			xbuff[4 * i + 1] = BC2_S1(ddx);
			xbuff[4 * i + 2] = BC2_S2(ddx);
			xbuff[4 * i + 3] = BC2_S3(ddx);

			xd += adx;
		}
	}

	sl -= src_stride + NCHAN;
	y = (mlib_s32)yd;
	sl += y * src_stride;

	for (j = 0; j < height; j++) {
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

		for (k = 4 - y_step; k < 4; k++) {
			pbuff = buff_arr[bind + k];
#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				DTYPE *sp = sl + cbuff[i];

				xf0 = xbuff[4 * i];
				xf1 = xbuff[4 * i + 1];
				xf2 = xbuff[4 * i + 2];
				xf3 = xbuff[4 * i + 3];

				pbuff[3 * i] = BC_SUM(NCHAN, 0);
				pbuff[3 * i + 1] = BC_SUM(NCHAN, 1);
				pbuff[3 * i + 2] = BC_SUM(NCHAN, 2);
			}

			sl += src_stride;
		}

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];
#pragma pipeloop(0)
		for (i = 0; i < NCHAN * width; i++) {
			h0 = buff0[i];
			h1 = buff1[i];
			h2 = buff2[i];
			h3 = buff3[i];

			res = yf0 * h0 + yf1 * h1 + yf2 * h2 + yf3 * h3;
			FLOAT2INT_CLAMP(dl[i], res);
		}

		yd += ady;
		y_step = (mlib_s32)yd - y;
		y = (mlib_s32)yd;
		bind = (bind + y_step) & 3;

		if (y_step > 4) {
			sl += (y_step - 4) * src_stride;
			y_step = 4;
		}

		dl += dst_stride;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  NCHAN
#define	NCHAN	4

mlib_status FUNC_NAME(4)  (
    mlib_work_image *param)
{
	DTYPE *sl = GetElemSubStruct(current, sp);
	DTYPE *dl = GetElemSubStruct(current, dp);
	TTYPE xf0, xf1, xf2, xf3;
	TTYPE yf0, yf1, yf2, yf3;
	TTYPE ddx, ddy, h0, h1, h2, h3, res;
	TTYPE *buff, *buff_arr[8], *pbuff, *buff0, *buff1, *buff2, *buff3,
	    *xbuff;
	mlib_s32 i, j, k, x, y, y_step = 4, filter =
	    GetElemStruct(filter), src_stride =
	    GetElemStruct(src_stride), dst_stride =
	    GetElemStruct(dst_stride), width =
	    GetElemSubStruct(current, width), height =
	    GetElemSubStruct(current, height);
	mlib_s32 *cbuff, bind = 0;
	TTYPE xd = GetElemStruct(xstart),
	    yd = GetElemStruct(ystart),
	    adx = 1.0 / GetElemStruct(zoomx), ady = 1.0 / GetElemStruct(zoomy);

	buff =
	    __mlib_malloc(width * (4 * NCHAN + 4) * sizeof (TTYPE) +
	    width * sizeof (mlib_s32));

	if (buff == NULL)
		return (MLIB_FAILURE);
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k] = buff + k * (NCHAN * width);
	xbuff = buff + 4 * NCHAN * width;
	cbuff = (mlib_s32 *)(xbuff + 4 * width);

	if (filter == MLIB_BICUBIC) {
#pragma pipeloop(0)
		for (i = 0; i < width; i++) {
			x = (mlib_s32)xd;
			ddx = xd - x;

			cbuff[i] = NCHAN * x;
			xbuff[4 * i] = BC_S0(ddx);
			xbuff[4 * i + 1] = BC_S1(ddx);
			xbuff[4 * i + 2] = BC_S2(ddx);
			xbuff[4 * i + 3] = BC_S3(ddx);

			xd += adx;
		}

	} else {
#pragma pipeloop(0)
		for (i = 0; i < width; i++) {
			x = (mlib_s32)xd;
			ddx = xd - x;

			cbuff[i] = NCHAN * x;
			xbuff[4 * i] = BC2_S0(ddx);
			xbuff[4 * i + 1] = BC2_S1(ddx);
			xbuff[4 * i + 2] = BC2_S2(ddx);
			xbuff[4 * i + 3] = BC2_S3(ddx);

			xd += adx;
		}
	}

	sl -= src_stride + NCHAN;
	y = (mlib_s32)yd;
	sl += y * src_stride;

	for (j = 0; j < height; j++) {
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

		for (k = 4 - y_step; k < 4; k++) {
			pbuff = buff_arr[bind + k];
#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				DTYPE *sp = sl + cbuff[i];

				xf0 = xbuff[4 * i];
				xf1 = xbuff[4 * i + 1];
				xf2 = xbuff[4 * i + 2];
				xf3 = xbuff[4 * i + 3];

				pbuff[4 * i] = BC_SUM(NCHAN, 0);
				pbuff[4 * i + 1] = BC_SUM(NCHAN, 1);
				pbuff[4 * i + 2] = BC_SUM(NCHAN, 2);
				pbuff[4 * i + 3] = BC_SUM(NCHAN, 3);
			}

			sl += src_stride;
		}

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];
#pragma pipeloop(0)
		for (i = 0; i < NCHAN * width; i++) {
			h0 = buff0[i];
			h1 = buff1[i];
			h2 = buff2[i];
			h3 = buff3[i];

			res = yf0 * h0 + yf1 * h1 + yf2 * h2 + yf3 * h3;
			FLOAT2INT_CLAMP(dl[i], res);
		}

		yd += ady;
		y_step = (mlib_s32)yd - y;
		y = (mlib_s32)yd;
		bind = (bind + y_step) & 3;

		if (y_step > 4) {
			sl += (y_step - 4) * src_stride;
			y_step = 4;
		}

		dl += dst_stride;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
