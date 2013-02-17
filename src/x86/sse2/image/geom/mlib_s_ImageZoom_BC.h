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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#ifndef _MLIB_S_IMAGEZOOM_BC_H
#define	_MLIB_S_IMAGEZOOM_BC_H

#pragma ident	"@(#)mlib_s_ImageZoom_BC.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	STORE(mem, x)	mem = x

/* *********************************************************** */

#define	BC_S0(x)	((TTYPE)-0.5*(x)*(x)*(x) + (x)*(x) - (TTYPE)0.5*(x))
#define	BC_S1(x)	((TTYPE) 1.5*(x)*(x)*(x) - (TTYPE)2.5*(x)*(x) + \
			(TTYPE)1.0)
#define	BC_S2(x)	((TTYPE)-1.5*(x)*(x)*(x) + (TTYPE)2.0*(x)*(x) + \
			(TTYPE)0.5*(x))
#define	BC_S3(x)	((TTYPE) 0.5*(x)*(x)*(x) - (TTYPE)0.5*(x)*(x))

/* *********************************************************** */

#define	BC2_S0(x)	(-(x)*(x)*(x) + (TTYPE)2.0*(x)*(x) - (x))
#define	BC2_S1(x)	((x)*(x)*(x) - (TTYPE)2.0*(x)*(x) + (TTYPE)1.0)
#define	BC2_S2(x)	(-(x)*(x)*(x) + (x)*(x) + (x))
#define	BC2_S3(x)	((x)*(x)*(x) - (x)*(x))

/* *********************************************************** */

#define	BC_SUM(NCHAN, k)                                               \
	xf0 *sp[k] + xf1 * sp[k + NCHAN] + xf2 * sp[k + 2 * NCHAN] +   \
	    xf3 * sp[k + 3 * NCHAN]

/* *********************************************************** */

#define	DEF_VARS                                                        \
	DTYPE *sl = GetElemSubStruct(current, sp);                      \
	DTYPE *dl = GetElemSubStruct(current, dp);                      \
	TTYPE xf0, xf1, xf2, xf3;                                       \
	TTYPE yf0, yf1, yf2, yf3;                                       \
	TTYPE ddx, ddy, h0, h1, h2, h3, res;                            \
	TTYPE *buff, *buff_arr[8], *pbuff, *buff0, *buff1, *buff2,      \
	    *buff3, *xbuff;                                             \
	mlib_s32 i, j, k, x, y, y_step = 4,                             \
		filter = GetElemStruct(filter),                         \
		src_stride = GetElemStruct(src_stride),                 \
		dst_stride = GetElemStruct(dst_stride),                 \
		width  = GetElemSubStruct(current, width),              \
		height = GetElemSubStruct(current, height);             \
	mlib_s32 *cbuff, bind = 0;                                      \
	TTYPE xd  = (TTYPE)param->xstart,                               \
		yd  = (TTYPE)param->ystart,                             \
		adx = (TTYPE)param->rzoomx,                             \
		ady = (TTYPE)param->rzoomy

/* *********************************************************** */

#undef  NCHAN
#define	NCHAN	1

mlib_status FUNC_NAME(
    1)  (
    mlib_work_image *param)
{
	DEF_VARS;

	buff =
	    __mlib_malloc((width * (4 * NCHAN + 4)) * sizeof (TTYPE) +
	    width * sizeof (mlib_s32));

	if (buff == NULL)
		return (MLIB_FAILURE);

	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k] = buff + k * (NCHAN * width);
	xbuff = buff + 4 * NCHAN * width;
	cbuff = (mlib_s32 *)(xbuff + 4 * width);

	if (filter == MLIB_BICUBIC) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			mlib_s32 cx = (mlib_s32)xd;
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
			mlib_s32 cx = (mlib_s32)xd;
			ddx = xd - cx;

			cbuff[i] = cx;
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

		for (k = 4 - y_step; k < 4; k++) {
			pbuff = buff_arr[bind + k];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
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

		STYPE sxf0, sxf1, sxf2, sxf3;
		sxf0 = S_SET(yf0);
		sxf1 = S_SET(yf1);
		sxf2 = S_SET(yf2);
		sxf3 = S_SET(yf3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < NCHAN * width - SNUM + 1; i += SNUM) {
			STYPE sk00, sk10, sk20, sk30;
			sk00 = S_LD((buff0 + i));
			sk10 = S_LD((buff1 + i));
			sk20 = S_LD((buff2 + i));
			sk30 = S_LD((buff3 + i));

			sk00 = S_MUL(sk00, sxf0);
			sk10 = S_MUL(sk10, sxf1);
			sk20 = S_MUL(sk20, sxf2);
			sk30 = S_MUL(sk30, sxf3);

			sk00 = S_ADD(sk00, sk10);
			sk20 = S_ADD(sk20, sk30);
			sk00 = S_ADD(sk00, sk20);
			S_STO((dl + i), sk00);
		}

		for (; i < NCHAN * width; i++) {
			h0 = buff0[i];
			h1 = buff1[i];
			h2 = buff2[i];
			h3 = buff3[i];
			res = yf0 * h0 + yf1 * h1 + yf2 * h2 + yf3 * h3;
			STORE(dl[i], res);
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

mlib_status FUNC_NAME(
    2)  (
    mlib_work_image *param)
{
	DEF_VARS;

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
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
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
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
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

		for (k = 4 - y_step; k < 4; k++) {
			pbuff = buff_arr[bind + k];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
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

		STYPE sxf0, sxf1, sxf2, sxf3;
		sxf0 = S_SET(yf0);
		sxf1 = S_SET(yf1);
		sxf2 = S_SET(yf2);
		sxf3 = S_SET(yf3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < NCHAN * width - SNUM + 1; i += SNUM) {
			STYPE sk00, sk10, sk20, sk30;
			sk00 = S_LD((buff0 + i));
			sk10 = S_LD((buff1 + i));
			sk20 = S_LD((buff2 + i));
			sk30 = S_LD((buff3 + i));

			sk00 = S_MUL(sk00, sxf0);
			sk10 = S_MUL(sk10, sxf1);
			sk20 = S_MUL(sk20, sxf2);
			sk30 = S_MUL(sk30, sxf3);

			sk00 = S_ADD(sk00, sk10);
			sk20 = S_ADD(sk20, sk30);
			sk00 = S_ADD(sk00, sk20);
			S_STO((dl + i), sk00);
		}

		for (; i < NCHAN * width; i++) {
			h0 = buff0[i];
			h1 = buff1[i];
			h2 = buff2[i];
			h3 = buff3[i];

			res = yf0 * h0 + yf1 * h1 + yf2 * h2 + yf3 * h3;
			STORE(dl[i], res);
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

mlib_status FUNC_NAME(
    3)  (
    mlib_work_image *param)
{
	DEF_VARS;

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
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
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
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
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

		for (k = 4 - y_step; k < 4; k++) {
			pbuff = buff_arr[bind + k];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
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
		STYPE sxf0, sxf1, sxf2, sxf3;
		sxf0 = S_SET(yf0);
		sxf1 = S_SET(yf1);
		sxf2 = S_SET(yf2);
		sxf3 = S_SET(yf3);

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < NCHAN * width - SNUM + 1; i += SNUM) {
			STYPE sk00, sk10, sk20, sk30;
			sk00 = S_LD((buff0 + i));
			sk10 = S_LD((buff1 + i));
			sk20 = S_LD((buff2 + i));
			sk30 = S_LD((buff3 + i));

			sk00 = S_MUL(sk00, sxf0);
			sk10 = S_MUL(sk10, sxf1);
			sk20 = S_MUL(sk20, sxf2);
			sk30 = S_MUL(sk30, sxf3);

			sk00 = S_ADD(sk00, sk10);
			sk20 = S_ADD(sk20, sk30);
			sk00 = S_ADD(sk00, sk20);
			S_STO((dl + i), sk00);
		}

		for (; i < NCHAN * width; i++) {
			h0 = buff0[i];
			h1 = buff1[i];
			h2 = buff2[i];
			h3 = buff3[i];

			res = yf0 * h0 + yf1 * h1 + yf2 * h2 + yf3 * h3;
			STORE(dl[i], res);
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

mlib_status FUNC_NAME(
    4)  (
    mlib_work_image *param)
{
	DEF_VARS;

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
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
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
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
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

		for (k = 4 - y_step; k < 4; k++) {
			pbuff = buff_arr[bind + k];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
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

		STYPE sxf0, sxf1, sxf2, sxf3;
		sxf0 = S_SET(yf0);
		sxf1 = S_SET(yf1);
		sxf2 = S_SET(yf2);
		sxf3 = S_SET(yf3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < NCHAN * width - SNUM + 1; i += SNUM) {
			STYPE sk00, sk10, sk20, sk30;
			sk00 = S_LD((buff0 + i));
			sk10 = S_LD((buff1 + i));
			sk20 = S_LD((buff2 + i));
			sk30 = S_LD((buff3 + i));

			sk00 = S_MUL(sk00, sxf0);
			sk10 = S_MUL(sk10, sxf1);
			sk20 = S_MUL(sk20, sxf2);
			sk30 = S_MUL(sk30, sxf3);

			sk00 = S_ADD(sk00, sk10);
			sk20 = S_ADD(sk20, sk30);
			sk00 = S_ADD(sk00, sk20);
			S_STO((dl + i), sk00);
		}

		for (; i < NCHAN * width; i++) {
			h0 = buff0[i];
			h1 = buff1[i];
			h2 = buff2[i];
			h3 = buff3[i];

			res = yf0 * h0 + yf1 * h1 + yf2 * h2 + yf3 * h3;
			STORE(dl[i], res);
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

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_IMAGEZOOM_BC_H */
