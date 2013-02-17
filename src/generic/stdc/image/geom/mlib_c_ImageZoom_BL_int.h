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

#ifndef _MLIB_C_IMAGEZOOM_BL_INT_H
#define	_MLIB_C_IMAGEZOOM_BL_INT_H

#pragma ident	"@(#)mlib_c_ImageZoom_BL_int.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_ImageDivTables.h>
#include <mlib_c_ImageBlendTable.h>

/* *********************************************************** */

#if IMG_TYPE == 1  /* MLIB_BYTE */

#define	PREC_BITS	11
#define	DTYPE		mlib_u8
#define	TABLE(A)	(A)
#define	FTABLE(A)	(((A) >> (16 - PREC_BITS)) & ((1 << PREC_BITS) - 1))
#define	RINT(x)		(((x) + (1 << (2*PREC_BITS - 1))) >> (2*PREC_BITS))
#define	VARIABLE_EXT(FORMAT, NCHAN)	mlib_s32 ssize = 0; FORMAT *buffs
#define	COPY_LINE(buffs, sp, ssize)	buffs = sp
#define	FUNC_NAME(CC)	mlib_c_ImageZoomBilinear_U8_##CC

#endif /* IMG_TYPE == 1 ( MLIB_BYTE ) */

/* *********************************************************** */

#define	BUFF_SIZE	300

/* *********************************************************** */

#define	VARIABLE(FORMAT)                                          \
	FORMAT  *sp = GetElemSubStruct(current, sp),              \
		*dp = GetElemSubStruct(current, dp);              \
	mlib_s32 i, j, x,                                         \
		y = GetElemSubStruct(current, srcY) & MLIB_MASK,  \
		dx = GetElemStruct(DX),                           \
		dy = GetElemStruct(DY),                           \
		src_stride = GetElemStruct(src_stride),           \
		dst_stride = GetElemStruct(dst_stride),           \
		width  = GetElemSubStruct(current, width),        \
		height = GetElemSubStruct(current, height)

/* *********************************************************** */

#if IMG_TYPE == 1	/* MLIB_BYTE */

#define	VARIABLE_BL(N)                                          \
	FP_TYPE a0##N, a1##N, a2##N, a3##N;                     \
	FP_TYPE r0##N, r1##N;                                   \
	RTYPE res##N

#else /* IMG_TYPE == 1 ( MLIB_BYTE ) */

#define	VARIABLE_BL(N)                                          \
	mlib_s32 a0##N, a1##N, a2##N, a3##N;                    \
	FP_TYPE r0##N, r1##N;                                   \
	RTYPE res##N

#endif /* IMG_TYPE == 1 ( MLIB_BYTE ) */

/* *********************************************************** */

#define	FP_TYPE	mlib_s32

/* *********************************************************** */

#define	RTYPE	mlib_s32

/* *********************************************************** */

#define	LOAD(N, index)                                          \
	a0##N = TABLE(tsp[cx + N]);                             \
	a3##N = TABLE(tsp[cx + src_stride + N]);                \
	a1##N = TABLE(tsp[cx + index]);                         \
	a2##N = TABLE(tsp[cx + src_stride + index])

/* *********************************************************** */

#define	CALC(N)                                                 \
	r0##N = (a0##N << PREC_BITS) + fdy * (a3##N - a0##N);   \
	r1##N = (a1##N << PREC_BITS) + fdy * (a2##N - a1##N)

/* *********************************************************** */

#define	RES(N)	res##N = (RTYPE)((r0##N << PREC_BITS) + fdx * (r1##N - r0##N))

/* *********************************************************** */

#define	STOR(N, index)	dp[index] = RINT(res##N)

/* *********************************************************** */

#ifndef NO_ZOOM_FUNC

#undef  NCHAN
#define	NCHAN	1

mlib_status FUNC_NAME(
    1)  (
    mlib_work_image *param)
{
	mlib_s32 buff_arr[BUFF_SIZE], *buff = buff_arr, *buff0, *buff1;
	VARIABLE(DTYPE);
	VARIABLE_EXT(DTYPE, NCHAN);
	mlib_s32 cx;
	mlib_s32 size, y_step = 2;

	size = 2*width;

	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size * sizeof (mlib_s32));

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + width;

	for (j = 0; j < height; j++) {
		mlib_s32 fdy = FTABLE(y);
		mlib_s32 fry = (1 << PREC_BITS) - fdy;

		if (dy > (1 << MLIB_SHIFT)) {
			mlib_s32 x0, x1, x2, x3, y0, y1, fdx;
			DTYPE *tsp = sp + (y >> MLIB_SHIFT) * src_stride;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = x >> MLIB_SHIFT;
				fdx = FTABLE(x);

				x0 = TABLE(tsp[cx]);
				x1 = TABLE(tsp[cx + NCHAN]);
				x2 = TABLE(tsp[cx + src_stride]);
				x3 = TABLE(tsp[cx + src_stride + NCHAN]);

				y0 = (x0 << PREC_BITS) + fdx * (x1 - x0);
				y1 = (x2 << PREC_BITS) + fdx * (x3 - x2);

				dp[i] = RINT(fry * y0 + fdy * y1);

				x += dx;
			}

		} else if (y_step == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				mlib_s32 y0, y1, rr;

				y0 = buff0[i];
				y1 = buff1[i];

				dp[i] = RINT(fry * y0 + fdy * y1);
			}
		} else if (y_step == 1) {
			DTYPE *tsp =
			    sp + (y >> MLIB_SHIFT) * src_stride + src_stride;
			mlib_s32 *buffT;

/* convert source line to mlib_s32 type */
			COPY_LINE(buffs, tsp, ssize);

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				mlib_s32 cxh;
				mlib_s32 xh2, xh3, fxh;
				mlib_s32 y0, y1, rr;

				cxh = x >> MLIB_SHIFT;
				fxh = FTABLE(x);

				xh2 = (mlib_s32)TABLE(buffs[cxh]);
				xh3 = (mlib_s32)TABLE(buffs[cxh + NCHAN]);

				y1 = (xh2 << PREC_BITS) + fxh * (xh3 - xh2);

				buff1[i] = y1;
				y0 = buff0[i];

				dp[i] = RINT(fry * y0 + fdy * y1);

				x += dx;
			}
		} else {
			mlib_s32 x0, x1, x2, x3, y0, y1, fdx;
			DTYPE *tsp = sp + (y >> MLIB_SHIFT) * src_stride;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = x >> MLIB_SHIFT;
				fdx = FTABLE(x);

				x0 = TABLE(tsp[cx]);
				x1 = TABLE(tsp[cx + NCHAN]);
				x2 = TABLE(tsp[cx + src_stride]);
				x3 = TABLE(tsp[cx + src_stride + NCHAN]);

				y0 = (x0 << PREC_BITS) + fdx * (x1 - x0);
				y1 = (x2 << PREC_BITS) + fdx * (x3 - x2);
				buff0[i] = y0;
				buff1[i] = y1;

				dp[i] = RINT((y0 << PREC_BITS) + fdy * (y1 -
					y0));

				x += dx;
			}
		}

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		y += dy;
		dp += dst_stride;
	}

	if (size > BUFF_SIZE)
		__mlib_free(buff);

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
	mlib_s32 buff_arr[BUFF_SIZE], *buff = buff_arr, *buff0, *buff1;
	VARIABLE(DTYPE);
	VARIABLE_EXT(DTYPE, NCHAN);
	mlib_s32 cx;
	mlib_s32 size, y_step = 2;

	size = 4*width;

	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size * sizeof (mlib_s32));

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + 2*width;

	for (j = 0; j < height; j++) {
		mlib_s32 fdy = FTABLE(y);
		mlib_s32 fry = (1 << PREC_BITS) - fdy;

		if (y_step == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < 2*width; i++) {
				mlib_s32 y0, y1, rr;

				y0 = buff0[i];
				y1 = buff1[i];

				dp[i] = RINT(fry * y0 + fdy * y1);
			}

		} else if (y_step == 1) {
			DTYPE *tsp =
			    sp + (y >> MLIB_SHIFT) * src_stride + src_stride;
			mlib_s32 *buffT;

/* convert source line to mlib_s32 type */
			COPY_LINE(buffs, tsp, ssize);

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				mlib_s32 cx;
				mlib_s32 a2, a3, b2, b3, fdx, frx;
				mlib_s32 y0a, y0b, y1a, y1b, r0, r1;

				cx = MUL_NCHAN(x >> MLIB_SHIFT);
				fdx = FTABLE(x);

				a2 = buffs[cx];
				a3 = buffs[cx + NCHAN];
				b2 = buffs[cx + 1];
				b3 = buffs[cx + NCHAN + 1];

				frx = (1 << PREC_BITS) - fdx;
				y1a = frx * a2 + fdx * a3;
				y1b = frx * b2 + fdx * b3;

				buff1[2*i] = y1a;
				buff1[2*i + 1] = y1b;
				y0a = buff0[2*i];
				y0b = buff0[2*i + 1];

				dp[NCHAN * i] =
				    RINT(fry * y0a + fdy * y1a);
				dp[NCHAN * i + 1] =
				    RINT(fry * y0b + fdy * y1b);

				x += dx;
			}

		} else {
			mlib_s32 a0, a1, a2, a3, ay0, ay1, fdx;
			mlib_s32 b0, b1, b2, b3, by0, by1;
			DTYPE *tsp = sp + (y >> MLIB_SHIFT) * src_stride;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = MUL_NCHAN(x >> MLIB_SHIFT);
				fdx = FTABLE(x);

				a0 = TABLE(tsp[cx]);
				a1 = TABLE(tsp[cx + NCHAN]);
				a2 = TABLE(tsp[cx + src_stride]);
				a3 = TABLE(tsp[cx + src_stride + NCHAN]);

				b0 = TABLE(tsp[cx + 1]);
				b1 = TABLE(tsp[cx + NCHAN + 1]);
				b2 = TABLE(tsp[cx + src_stride + 1]);
				b3 = TABLE(tsp[cx + src_stride + NCHAN + 1]);

				ay0 = (a0 << PREC_BITS) + fdx * (a1 - a0);
				ay1 = (a2 << PREC_BITS) + fdx * (a3 - a2);
				buff0[2 * i] = ay0;
				buff1[2 * i] = ay1;

				by0 = (b0 << PREC_BITS) + fdx * (b1 - b0);
				by1 = (b2 << PREC_BITS) + fdx * (b3 - b2);
				buff0[2 * i + 1] = by0;
				buff1[2 * i + 1] = by1;

				dp[NCHAN * i] = RINT((ay0 << PREC_BITS) +
					fdy * (ay1 - ay0));
				dp[NCHAN * i + 1] = RINT((by0 << PREC_BITS) +
					fdy * (by1 - by0));

				x += dx;
			}
		}

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		y += dy;
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
#define	MUL_NCHAN(x)	(((x) << 1) + (x))

mlib_status FUNC_NAME(
    3)  (
    mlib_work_image *param)
{
	mlib_s32 buff_arr[BUFF_SIZE], *buff = buff_arr, *buff0, *buff1;
	VARIABLE(DTYPE);
	VARIABLE_EXT(DTYPE, NCHAN);
	mlib_s32 size, y_step = 2, l;

	if (!height)
		return (MLIB_SUCCESS);

	size = 6*width;

	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size * sizeof (mlib_s32));

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + 3*width;

	for (j = 0; j < 3*width; j++) {
		buff1[j] = 0;
	}

	for (j = 0; j < height; j++) {
		mlib_s32 fdy = FTABLE(y);
		mlib_s32 fry = (1 << PREC_BITS) - fdy;

		if (y_step == 0) {
			mlib_s32 k;

			for (k = 0; k < 3; k++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width; i++) {
					mlib_s32 y0, y1;

					y0 = buff0[i];
					y1 = buff1[i];

					dp[3 * i] =
					    RINT(fry * y0 + fdy * y1);
				}

				sp += 1;
				dp += 1;
				buff0 += width;
				buff1 += width;
			}

			sp -= 3;
			dp -= 3;
			buff0 -= 3 * width;
			buff1 -= 3 * width;

		} else {
			for (l = 0; l < y_step; l++) {
				mlib_s32 k;
				mlib_s32 *buffT;

/* convert source line to mlib_s32 type */
				COPY_LINE(buffs, sp, ssize);

				buffT = buff0;
				buff0 = buff1;
				buff1 = buffT;

				for (k = 0; k < 3; k++) {
					x = GetElemSubStruct(current,
					    srcX) & MLIB_MASK;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < width; i++) {
						mlib_s32 cxh;
						mlib_s32 xh2, xh3, fxh;
						mlib_s32 y0, y1, rr;

						cxh =
						    MUL_NCHAN(x >> MLIB_SHIFT);

						fxh = FTABLE(x);

						xh2 =
						    (mlib_s32)TABLE(buffs[cxh]);
						xh3 =
						    (mlib_s32)TABLE(buffs[cxh +
						    NCHAN]);

						y1 = (xh2 << PREC_BITS) + fxh *
							(xh3 - xh2);

						buff1[i] = y1;
						y0 = buff0[i];

						dp[3 * i] =
						    RINT(fry * y0 + fdy * y1);

						x += dx;
					}

					dp += 1;
					buffs += 1;
					buff0 += width;
					buff1 += width;
				}

				dp -= 3;
				buffs -= 3;
				buff0 -= 3 * width;
				buff1 -= 3 * width;
				sp += src_stride;
			}
		}

		if (y_step >= 0) {
			dp += dst_stride;
			y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
			y += dy;
		} else {
			y_step = 1;
		}
	}

	if (buff != buff_arr)
		__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  NCHAN
#define	NCHAN	4

/* *********************************************************** */

#undef  MUL_NCHAN
#define	MUL_NCHAN(x)	((x) << 2)

mlib_status FUNC_NAME(
    4)  (
    mlib_work_image *param)
{
	mlib_s32 buff_arr[BUFF_SIZE], *buff = buff_arr, *buff0, *buff1;
	VARIABLE(DTYPE);
	VARIABLE_EXT(DTYPE, NCHAN);
	mlib_s32 cx;
	mlib_s32 size, y_step = -1;

	if (!height)
		return (MLIB_SUCCESS);

	size = 8*width;

	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size * sizeof (mlib_s32));

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + 4 * width;

	for (j = 0; j < 4 * width; j++) {
		buff1[j] = 0;
	}

	for (j = 0; j < height + 1; j++) {
		mlib_s32 fdy = FTABLE(y);
		mlib_s32 fry = (1 << PREC_BITS) - fdy;

		if (y_step == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < 4*width; i++) {
				mlib_s32 y0, y1, rr;

				y0 = buff0[i];
				y1 = buff1[i];

				dp[i] =
				    RINT(fry * y0 + fdy * y1);
			}

		} else {
			mlib_s32 k;
			mlib_s32 *buffT;

/* convert source line to mlib_s32 type */
			COPY_LINE(buffs, sp, ssize);

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			for (k = 0; k < 4; k++) {
				x = GetElemSubStruct(current, srcX) & MLIB_MASK;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < width; i++) {
					mlib_s32 fdx, frx, a2, a3, y0, y1, rr;

					cx = MUL_NCHAN(x >> MLIB_SHIFT);
					fdx = FTABLE(x);

					a2 = (mlib_s32)TABLE(buffs[cx]);
					a3 = (mlib_s32)TABLE(buffs[cx + NCHAN]);

					frx = (1 << PREC_BITS) - fdx;
					y1 = frx * a2 + fdx * a3;

					buff1[4*i] = y1;
					y0 = buff0[4*i];

					dp[NCHAN * i] =
					    RINT(fry * y0 + fdy * y1);

					x += dx;
				}

				dp += 1;
				buffs += 1;
				buff0 += 1;
				buff1 += 1;
			}

			dp -= 4;
			buffs -= 4;
			buff0 -= 4;
			buff1 -= 4;
			sp += src_stride;
		}

		if (y_step >= 0) {
			dp += dst_stride;
			y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
			y += dy;
		} else {
			y_step = 1;
		}
	}

	if (size > BUFF_SIZE)
		__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    1s) (
    mlib_work_image *param)
{
	VARIABLE(DTYPE);
	mlib_s32 cx;

	VARIABLE_BL(0);
	DTYPE *tsp = sp;
	FP_TYPE fdx, fdy;

	for (j = 0; j < height; j++) {
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		fdy = FTABLE(y);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = x >> MLIB_SHIFT;
			fdx = FTABLE(x);

			LOAD(0, 1);
			CALC(0);
			RES(0);
			STOR(0, i);

			x += dx;
		}

		y += dy;
		dp += dst_stride;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    2s) (
    mlib_work_image *param)
{
	VARIABLE(DTYPE);
	mlib_s32 cx;

	VARIABLE_BL(0);
	VARIABLE_BL(1);
	DTYPE *tsp = sp;
	FP_TYPE fdx, fdy;

	for (j = 0; j < height; j++) {
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		fdy = FTABLE(y);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = (x >> MLIB_SHIFT) * 2;
			fdx = FTABLE(x);

			LOAD(0, 2);
			LOAD(1, 3);
			CALC(0);
			CALC(1);
			RES(0);
			RES(1);
			STOR(0, 2 * i);
			STOR(1, 2 * i + 1);

			x += dx;
		}

		y += dy;
		dp += dst_stride;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    3s) (
    mlib_work_image *param)
{
	VARIABLE(DTYPE);
	mlib_s32 cx;

	VARIABLE_BL(0);
	VARIABLE_BL(1);
	VARIABLE_BL(2);
	DTYPE *tsp = sp;
	FP_TYPE fdx, fdy;

	for (j = 0; j < height; j++) {
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		fdy = FTABLE(y);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = (x >> MLIB_SHIFT) * 3;
			fdx = FTABLE(x);

			LOAD(0, 3);
			LOAD(1, 4);
			LOAD(2, 5);

			CALC(0);
			CALC(1);
			CALC(2);
			RES(0);
			RES(1);
			RES(2);

			STOR(0, 3 * i);
			STOR(1, 3 * i + 1);
			STOR(2, 3 * i + 2);

			x += dx;
		}

		y += dy;
		dp += dst_stride;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    4s) (
    mlib_work_image *param)
{
	VARIABLE(DTYPE);
	mlib_s32 cx;

	VARIABLE_BL(0);
	VARIABLE_BL(1);
	VARIABLE_BL(2);
	VARIABLE_BL(3);
	DTYPE *tsp = sp;
	FP_TYPE fdx, fdy;

	for (j = 0; j < height; j++) {
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		fdy = FTABLE(y);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = (x >> MLIB_SHIFT) * 4;
			fdx = FTABLE(x);

			LOAD(0, 4);
			LOAD(1, 5);
			LOAD(2, 6);
			LOAD(3, 7);

			CALC(0);
			CALC(1);
			CALC(2);
			CALC(3);

			RES(0);
			RES(1);
			RES(2);
			RES(3);

			STOR(0, 4 * i);
			STOR(1, 4 * i + 1);
			STOR(2, 4 * i + 2);
			STOR(3, 4 * i + 3);

			x += dx;
		}

		y += dy;
		dp += dst_stride;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#endif /* NO_ZOOM_FUNC */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_C_IMAGEZOOM_BL_INT_H */
