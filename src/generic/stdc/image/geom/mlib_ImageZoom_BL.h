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

#ifndef _MLIB_IMAGEZOOM_BL_H
#define	_MLIB_IMAGEZOOM_BL_H

#pragma ident	"@(#)mlib_ImageZoom_BL.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>

/* *********************************************************** */

#define	BUFF_SIZE	300

/* *********************************************************** */

#define	VARIABLE(FORMAT)                                     \
	FORMAT *sp = GetElemSubStruct(current, sp),          \
		*dp = GetElemSubStruct(current, dp);         \
	FTYPE ddx = (FTYPE)param->rzoomx,                    \
		ddy = (FTYPE)param->rzoomy,                  \
		yd  = (FTYPE)param->ystart,                  \
		xd;                                          \
	mlib_s32 i, j, cx, y,                                \
		src_stride = GetElemStruct(src_stride),      \
		dst_stride = GetElemStruct(dst_stride),      \
		width  = GetElemSubStruct(current, width),   \
		height = GetElemSubStruct(current, height)

/* *********************************************************** */

#define	VARIABLE_BL(FORMAT)                                     \
	FTYPE fdx, fdy, fdx1, fdy1;                             \
	FORMAT *tsp = sp;                                       \
	                                                        \
	y = (mlib_s32)yd

/* *********************************************************** */

#define	BL_SUM4(NCHAN, k)                                         \
	(fdx1 * fdy1 * tsp[cx + k] + fdx * fdy1 * tsp[cx + (k +   \
	    NCHAN)] + fdx1 * fdy * tsp[cx + src_stride + k] +     \
	    fdx * fdy * tsp[cx + src_stride + (k + NCHAN)])

/* *********************************************************** */

#define	BL_SUM(NCHAN, cx)	(frx*tsp[cx] + fdx*tsp[cx + NCHAN])

/* *********************************************************** */

#undef  NCHAN
#define	NCHAN	1
mlib_status FUNC_NAME(
    1s) (
    mlib_work_image *param)
{
	VARIABLE(DTYPE);
	VARIABLE_BL(DTYPE);

	for (j = 0; j < height; j++) {
		fdy = yd - y;
		fdy1 = 1 - fdy;

		xd = (FTYPE) GetElemStruct(xstart);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = (mlib_s32)xd;
			fdx = xd - cx;
			fdx1 = 1 - fdx;

			dp[NCHAN * i] = (DTYPE) BL_SUM4(NCHAN, 0);

			xd += ddx;
		}

		yd += ddy;
		y = (mlib_s32)yd;

		dp += dst_stride;
		tsp = sp + y * src_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  NCHAN
#define	NCHAN	2

mlib_status FUNC_NAME(
    2s) (
    mlib_work_image *param)
{
	VARIABLE(DTYPE);
	VARIABLE_BL(DTYPE);

	for (j = 0; j < height; j++) {
		fdy = yd - y;
		fdy1 = 1 - fdy;

		xd = (FTYPE) GetElemStruct(xstart);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = (mlib_s32)xd;
			fdx = xd - cx;
			fdx1 = 1 - fdx;
			cx *= NCHAN;

			dp[NCHAN * i] = (DTYPE) BL_SUM4(NCHAN, 0);
			dp[NCHAN * i + 1] = (DTYPE) BL_SUM4(NCHAN, 1);

			xd += ddx;
		}

		yd += ddy;
		y = (mlib_s32)yd;

		dp += dst_stride;
		tsp = sp + y * src_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  NCHAN
#define	NCHAN	3

mlib_status FUNC_NAME(
    3s) (
    mlib_work_image *param)
{
	VARIABLE(DTYPE);
	FTYPE r0, r1, r2;

	VARIABLE_BL(DTYPE);

	for (j = 0; j < height; j++) {
		fdy = yd - y;
		fdy1 = 1 - fdy;

		xd = (FTYPE) GetElemStruct(xstart);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = (mlib_s32)xd;
			fdx = xd - cx;
			fdx1 = 1 - fdx;
			cx *= NCHAN;

			r0 = BL_SUM4(NCHAN, 0);
			r1 = BL_SUM4(NCHAN, 1);
			r2 = BL_SUM4(NCHAN, 2);

			dp[NCHAN * i] = (DTYPE) r0;
			dp[NCHAN * i + 1] = (DTYPE) r1;
			dp[NCHAN * i + 2] = (DTYPE) r2;

			xd += ddx;
		}

		yd += ddy;
		y = (mlib_s32)yd;

		dp += dst_stride;
		tsp = sp + y * src_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  NCHAN
#define	NCHAN	4

mlib_status FUNC_NAME(
    4s) (
    mlib_work_image *param)
{
	VARIABLE(DTYPE);
	VARIABLE_BL(DTYPE);

	for (j = 0; j < height; j++) {
		fdy = yd - y;
		fdy1 = 1 - fdy;

		xd = (FTYPE) GetElemStruct(xstart);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = (mlib_s32)xd;
			fdx = xd - cx;
			fdx1 = 1 - fdx;
			cx *= NCHAN;

			dp[NCHAN * i] = (DTYPE) BL_SUM4(NCHAN, 0);
			dp[NCHAN * i + 1] = (DTYPE) BL_SUM4(NCHAN, 1);
			dp[NCHAN * i + 2] = (DTYPE) BL_SUM4(NCHAN, 2);
			dp[NCHAN * i + 3] = (DTYPE) BL_SUM4(NCHAN, 3);

			xd += ddx;
		}

		yd += ddy;
		y = (mlib_s32)yd;

		dp += dst_stride;
		tsp = sp + y * src_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    1)  (
    mlib_work_image *param)
{
	FTYPE buff_arr[BUFF_SIZE], *buff = buff_arr, *buff0, *buff1;

	VARIABLE(DTYPE);
	mlib_s32 dy = GetElemStruct(DY);
	mlib_s32 size, y_step = 2;

	size = 2 * width;

	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size * sizeof (FTYPE));

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + width;

	for (j = 0; j < height; j++) {
		FTYPE fdy, fry;

		y = (mlib_s32)yd;
		fdy = yd - y;
		fry = 1 - fdy;

		xd = (FTYPE) GetElemStruct(xstart);

		if (dy > (1 << MLIB_SHIFT)) {
			FTYPE x0, x1, x2, x3, y0, y1, fdx, frx;
			DTYPE *tsp = sp + y * src_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = (mlib_s32)xd;
				fdx = xd - cx;
				frx = 1 - fdx;

				x0 = tsp[cx];
				x1 = tsp[cx + 1];
				x2 = tsp[cx + src_stride];
				x3 = tsp[cx + src_stride + 1];

				y0 = frx * x0 + fdx * x1;
				y1 = frx * x2 + fdx * x3;

				dp[i] = (DTYPE) (fry * y0 + fdy * y1);

				xd += ddx;
			}
		} else if (y_step == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				FTYPE y0, y1;

				y0 = buff0[i];
				y1 = buff1[i];

				dp[i] = (DTYPE) (fry * y0 + fdy * y1);
			}
		} else if (y_step == 1) {
			DTYPE *tsp = sp + y * src_stride + src_stride;
			FTYPE *buffT;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				FTYPE x2, x3, fdx, frx, y0, y1;

				cx = (mlib_s32)xd;
				fdx = xd - cx;
				frx = 1 - fdx;

				x2 = tsp[cx];
				x3 = tsp[cx + 1];

				y1 = frx * x2 + fdx * x3;

				buff1[i] = y1;
				y0 = buff0[i];

				dp[i] = (DTYPE) (fry * y0 + fdy * y1);
				xd += ddx;
			}
		} else {
			FTYPE x0, x1, x2, x3, y0, y1, fdx, frx;
			DTYPE *tsp = sp + y * src_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = (mlib_s32)xd;
				fdx = xd - cx;
				frx = 1 - fdx;

				x0 = tsp[cx];
				x1 = tsp[cx + 1];
				x2 = tsp[cx + src_stride];
				x3 = tsp[cx + src_stride + 1];

				y0 = frx * x0 + fdx * x1;
				y1 = frx * x2 + fdx * x3;
				buff0[i] = y0;
				buff1[i] = y1;

				dp[i] = (DTYPE) (fry * y0 + fdy * y1);
				xd += ddx;
			}
		}

		y_step = (mlib_s32)(yd + ddy) - y;
		yd += ddy;
		dp += dst_stride;
	}

	if (size > BUFF_SIZE) {
		__mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  NCHAN
#define	NCHAN	2

/* *********************************************************** */

#undef  MUL_NCHAN
#define	MUL_NCHAN(x)	((x) << 1)

mlib_status FUNC_NAME(
    2)  (
    mlib_work_image *param)
{
	FTYPE buff_arr[BUFF_SIZE], *buff = buff_arr, *buff0, *buff1;

	VARIABLE(DTYPE);
	mlib_s32 size, y_step = 2;

	size = 2 * NCHAN * width;

	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size * sizeof (FTYPE));

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + NCHAN * width;

	for (j = 0; j < height; j++) {
		FTYPE fdy, fry;

		y = (mlib_s32)yd;
		fdy = yd - y;
		fry = 1 - fdy;

		xd = (FTYPE) GetElemStruct(xstart);

		if (y_step == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < NCHAN * width; i++) {
				FTYPE y0, y1;

				y0 = buff0[i];
				y1 = buff1[i];

				dp[i] = (DTYPE) (fry * y0 + fdy * y1);
			}
		} else if (y_step == 1) {
			DTYPE *tsp = sp + y * src_stride + src_stride;
			FTYPE *buffT;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				FTYPE fdx, frx, y0a, y1a, y0b, y1b;

				cx = (mlib_s32)xd;
				fdx = xd - cx;
				frx = 1 - fdx;
				cx = MUL_NCHAN(cx);

				y1a = BL_SUM(NCHAN, cx);
				y1b = BL_SUM(NCHAN, cx + 1);

				buff1[NCHAN * i] = y1a;
				buff1[NCHAN * i + 1] = y1b;
				y0a = buff0[NCHAN * i];
				y0b = buff0[NCHAN * i + 1];

				dp[NCHAN * i] = (DTYPE) (fry * y0a + fdy * y1a);
				dp[NCHAN * i + 1] =
				    (DTYPE) (fry * y0b + fdy * y1b);
				xd += ddx;
			}
		} else {
			FTYPE fdx, frx, y0a, y1a, y0b, y1b;
			DTYPE *tsp = sp + y * src_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = (mlib_s32)xd;
				fdx = xd - cx;
				frx = 1 - fdx;
				cx = MUL_NCHAN(cx);

				y0a = BL_SUM(NCHAN, cx);
				y0b = BL_SUM(NCHAN, cx + 1);
				y1a = BL_SUM(NCHAN, cx + src_stride);
				y1b = BL_SUM(NCHAN, cx + src_stride + 1);
				buff0[NCHAN * i] = y0a;
				buff1[NCHAN * i] = y1a;
				buff0[NCHAN * i + 1] = y0b;
				buff1[NCHAN * i + 1] = y1b;

				dp[NCHAN * i] = (DTYPE) (fry * y0a + fdy * y1a);
				dp[NCHAN * i + 1] =
				    (DTYPE) (fry * y0b + fdy * y1b);
				xd += ddx;
			}
		}

		y_step = (mlib_s32)(yd + ddy) - y;
		yd += ddy;
		dp += dst_stride;
	}

	if (size > BUFF_SIZE)
		__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  NCHAN
#define	NCHAN	3

/* *********************************************************** */

#undef  MUL_NCHAN
#define	MUL_NCHAN(x)	(NCHAN*(x))

mlib_status FUNC_NAME(
    3)  (
    mlib_work_image *param)
{
	FTYPE buff_arr[BUFF_SIZE], *buff = buff_arr, *buff0, *buff1;

	VARIABLE(DTYPE);
	mlib_s32 size, y_step = 2;

	size = 2 * NCHAN * width;

	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size * sizeof (FTYPE));

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + NCHAN * width;

	for (j = 0; j < height; j++) {
		FTYPE fdy, fry;

		y = (mlib_s32)yd;
		fdy = yd - y;
		fry = 1 - fdy;

		xd = (FTYPE) GetElemStruct(xstart);

		if (y_step == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < NCHAN * width; i++) {
				FTYPE y0, y1;

				y0 = buff0[i];
				y1 = buff1[i];

				dp[i] = (DTYPE) (fry * y0 + fdy * y1);
			}
		} else if (y_step == 1) {
			DTYPE *tsp = sp + y * src_stride + src_stride;
			FTYPE *buffT;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				FTYPE fdx, frx, y0a, y1a, y0b, y1b, y0c, y1c;

				cx = (mlib_s32)xd;
				fdx = xd - cx;
				frx = 1 - fdx;
				cx = MUL_NCHAN(cx);

				y1a = BL_SUM(NCHAN, cx);
				y1b = BL_SUM(NCHAN, cx + 1);
				y1c = BL_SUM(NCHAN, cx + 2);

				buff1[NCHAN * i] = y1a;
				buff1[NCHAN * i + 1] = y1b;
				buff1[NCHAN * i + 2] = y1c;
				y0a = buff0[NCHAN * i];
				y0b = buff0[NCHAN * i + 1];
				y0c = buff0[NCHAN * i + 2];

				dp[NCHAN * i] = (DTYPE) (fry * y0a + fdy * y1a);
				dp[NCHAN * i + 1] =
				    (DTYPE) (fry * y0b + fdy * y1b);
				dp[NCHAN * i + 2] =
				    (DTYPE) (fry * y0c + fdy * y1c);
				xd += ddx;
			}
		} else {
			FTYPE fdx, frx, y0a, y1a, y0b, y1b, y0c, y1c;
			DTYPE *tsp = sp + y * src_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = (mlib_s32)xd;
				fdx = xd - cx;
				frx = 1 - fdx;
				cx = MUL_NCHAN(cx);

				y0a = BL_SUM(NCHAN, cx);
				y0b = BL_SUM(NCHAN, cx + 1);
				y0c = BL_SUM(NCHAN, cx + 2);
				y1a = BL_SUM(NCHAN, cx + src_stride);
				y1b = BL_SUM(NCHAN, cx + src_stride + 1);
				y1c = BL_SUM(NCHAN, cx + src_stride + 2);
				buff0[NCHAN * i] = y0a;
				buff1[NCHAN * i] = y1a;
				buff0[NCHAN * i + 1] = y0b;
				buff1[NCHAN * i + 1] = y1b;
				buff0[NCHAN * i + 2] = y0c;
				buff1[NCHAN * i + 2] = y1c;

				dp[NCHAN * i] = (DTYPE) (fry * y0a + fdy * y1a);
				dp[NCHAN * i + 1] =
				    (DTYPE) (fry * y0b + fdy * y1b);
				dp[NCHAN * i + 2] =
				    (DTYPE) (fry * y0c + fdy * y1c);
				xd += ddx;
			}
		}

		y_step = (mlib_s32)(yd + ddy) - y;
		yd += ddy;
		dp += dst_stride;
	}

	if (size > BUFF_SIZE)
		__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  NCHAN
#define	NCHAN	4

/* *********************************************************** */

#undef  MUL_NCHAN
#define	MUL_NCHAN(x)	(NCHAN*(x))

mlib_status FUNC_NAME(
    4)  (
    mlib_work_image *param)
{
	FTYPE buff_arr[BUFF_SIZE], *buff = buff_arr, *buff0, *buff1;

	VARIABLE(DTYPE);
	mlib_s32 size, y_step = 2;

	size = 2 * NCHAN * width;

	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size * sizeof (FTYPE));

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + NCHAN * width;

	for (j = 0; j < height; j++) {
		FTYPE fdy, fry;

		y = (mlib_s32)yd;
		fdy = yd - y;
		fry = 1 - fdy;

		xd = (FTYPE) GetElemStruct(xstart);

		if (y_step == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < NCHAN * width; i++) {
				FTYPE y0, y1;

				y0 = buff0[i];
				y1 = buff1[i];

				dp[i] = (DTYPE) (fry * y0 + fdy * y1);
			}
		} else if (y_step == 1) {
			DTYPE *tsp = sp + y * src_stride + src_stride;
			FTYPE *buffT;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				FTYPE fdx, frx, y0a, y1a, y0b, y1b, y0c, y1c,
				    y0d, y1d;

				cx = (mlib_s32)xd;
				fdx = xd - cx;
				frx = 1 - fdx;
				cx = MUL_NCHAN(cx);

				y1a = BL_SUM(NCHAN, cx);
				y1b = BL_SUM(NCHAN, cx + 1);
				y1c = BL_SUM(NCHAN, cx + 2);
				y1d = BL_SUM(NCHAN, cx + 3);

				buff1[NCHAN * i] = y1a;
				buff1[NCHAN * i + 1] = y1b;
				buff1[NCHAN * i + 2] = y1c;
				buff1[NCHAN * i + 3] = y1d;
				y0a = buff0[NCHAN * i];
				y0b = buff0[NCHAN * i + 1];
				y0c = buff0[NCHAN * i + 2];
				y0d = buff0[NCHAN * i + 3];

				dp[NCHAN * i] = (DTYPE) (fry * y0a + fdy * y1a);
				dp[NCHAN * i + 1] =
				    (DTYPE) (fry * y0b + fdy * y1b);
				dp[NCHAN * i + 2] =
				    (DTYPE) (fry * y0c + fdy * y1c);
				dp[NCHAN * i + 3] =
				    (DTYPE) (fry * y0d + fdy * y1d);
				xd += ddx;
			}
		} else {
			FTYPE fdx, frx, y0a, y1a, y0b, y1b, y0c, y1c, y0d, y1d;
			DTYPE *tsp = sp + y * src_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = (mlib_s32)xd;
				fdx = xd - cx;
				frx = 1 - fdx;
				cx = MUL_NCHAN(cx);

				y0a = BL_SUM(NCHAN, cx);
				y0b = BL_SUM(NCHAN, cx + 1);
				y0c = BL_SUM(NCHAN, cx + 2);
				y0d = BL_SUM(NCHAN, cx + 3);
				y1a = BL_SUM(NCHAN, cx + src_stride);
				y1b = BL_SUM(NCHAN, cx + src_stride + 1);
				y1c = BL_SUM(NCHAN, cx + src_stride + 2);
				y1d = BL_SUM(NCHAN, cx + src_stride + 3);
				buff0[NCHAN * i] = y0a;
				buff1[NCHAN * i] = y1a;
				buff0[NCHAN * i + 1] = y0b;
				buff1[NCHAN * i + 1] = y1b;
				buff0[NCHAN * i + 2] = y0c;
				buff1[NCHAN * i + 2] = y1c;
				buff0[NCHAN * i + 3] = y0d;
				buff1[NCHAN * i + 3] = y1d;

				dp[NCHAN * i] = (DTYPE) (fry * y0a + fdy * y1a);
				dp[NCHAN * i + 1] =
				    (DTYPE) (fry * y0b + fdy * y1b);
				dp[NCHAN * i + 2] =
				    (DTYPE) (fry * y0c + fdy * y1c);
				dp[NCHAN * i + 3] =
				    (DTYPE) (fry * y0d + fdy * y1d);
				xd += ddx;
			}
		}

		y_step = (mlib_s32)(yd + ddy) - y;
		yd += ddy;
		dp += dst_stride;
	}

	if (size > BUFF_SIZE)
		__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGEZOOM_BL_H */
