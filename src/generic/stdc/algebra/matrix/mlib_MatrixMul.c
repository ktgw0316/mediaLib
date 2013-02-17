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

#pragma ident	"@(#)mlib_MatrixMul.c	9.2	07/10/08 SMI"

/*
 * FUNCTIONS
 *      mlib_MatrixMul_S32_S32_Mod
 *      mlib_MatrixMul_S32_S32_Sat
 *      mlib_MatrixMul_S16_S16_Sat
 *      mlib_MatrixMul_S32_S16_Sat
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_MatrixMul_S32_S32_[Sat|Mod](mlib_s32       * z,
 *                                                   const mlib_s32 * x,
 *                                                   const mlib_s32 * y,
 *                                                   mlib_s32       m,
 *                                                   mlib_s32       l,
 *                                                   mlib_s32       n);
 *
 *      mlib_status mlib_MatrixMul_S16_S16_Sat(mlib_s16       * z,
 *                                             const mlib_s16 * x,
 *                                             const mlib_s16 * y,
 *                                             mlib_s32       m,
 *                                             mlib_s32       l,
 *                                             mlib_s32       n);
 *      mlib_status mlib_MatrixMul_S32_S16_Sat(mlib_s32       * z,
 *                                             const mlib_s16 * x,
 *                                             const mlib_s16 * y,
 *                                             mlib_s32       m,
 *                                             mlib_s32       l,
 *                                             mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result matrix.
 *           The matrix is in row major order
 *      x    pointer to the first element of the first matrix.
 *           The matrix is in row major order
 *      y    pointer to the first element of the second matrix.
 *           The matrix is in row major order
 *      m    number of rows in the first matrix
 *      l    number of columns in the first matrix, and number of rows
 *           in the second matrix
 *      n    number of columns in the second matrix
 *
 * DESCRIPTION
 *                 l
 *      z[i][j] = SUM (x[i][k] * y[k][j])
 *                k = 1
 */

#include <mlib_algebra.h>
#include <mlib_MatrixMul.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_MatrixMul_S32C_S32C_Mod = __mlib_MatrixMul_S32C_S32C_Mod
#pragma weak mlib_MatrixMul_S32C_S32C_Sat = __mlib_MatrixMul_S32C_S32C_Sat
#pragma weak mlib_MatrixMul_S32_S32_Mod = __mlib_MatrixMul_S32_S32_Mod
#pragma weak mlib_MatrixMul_S32_S32_Sat = __mlib_MatrixMul_S32_S32_Sat
#pragma weak mlib_MatrixMul_S32C_S16C_Sat = __mlib_MatrixMul_S32C_S16C_Sat
#pragma weak mlib_MatrixMul_S16C_S16C_Sat = __mlib_MatrixMul_S16C_S16C_Sat
#pragma weak mlib_MatrixMul_S32_S16_Sat = __mlib_MatrixMul_S32_S16_Sat
#pragma weak mlib_MatrixMul_S16_S16_Sat = __mlib_MatrixMul_S16_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_MatrixMul_S32C_S32C_Mod) mlib_MatrixMul_S32C_S32C_Mod
	__attribute__((weak, alias("__mlib_MatrixMul_S32C_S32C_Mod")));
__typeof__(__mlib_MatrixMul_S32C_S32C_Sat) mlib_MatrixMul_S32C_S32C_Sat
	__attribute__((weak, alias("__mlib_MatrixMul_S32C_S32C_Sat")));
__typeof__(__mlib_MatrixMul_S32_S32_Mod) mlib_MatrixMul_S32_S32_Mod
	__attribute__((weak, alias("__mlib_MatrixMul_S32_S32_Mod")));
__typeof__(__mlib_MatrixMul_S32_S32_Sat) mlib_MatrixMul_S32_S32_Sat
	__attribute__((weak, alias("__mlib_MatrixMul_S32_S32_Sat")));
__typeof__(__mlib_MatrixMul_S32C_S16C_Sat) mlib_MatrixMul_S32C_S16C_Sat
	__attribute__((weak, alias("__mlib_MatrixMul_S32C_S16C_Sat")));
__typeof__(__mlib_MatrixMul_S16C_S16C_Sat) mlib_MatrixMul_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_MatrixMul_S16C_S16C_Sat")));
__typeof__(__mlib_MatrixMul_S32_S16_Sat) mlib_MatrixMul_S32_S16_Sat
	__attribute__((weak, alias("__mlib_MatrixMul_S32_S16_Sat")));
__typeof__(__mlib_MatrixMul_S16_S16_Sat) mlib_MatrixMul_S16_S16_Sat
	__attribute__((weak, alias("__mlib_MatrixMul_S16_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MAX_SIZE	400

/* *********************************************************** */

MATRIX_MUL_FP64(S16, S16, Sat)
MATRIX_MUL_FP64(S32, S16, Sat)

MATRIX_MULC_FP64(S16, S16, Sat)
MATRIX_MULC_FP64(S32, S16, Sat)

/* *********************************************************** */

mlib_status
__mlib_MatrixMul_S32_S32_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 l,
	mlib_s32 n)
{
	mlib_s32 size, i, j, k, l_end;
	mlib_d64 *dpx, *dpy;

	if (!((m > 0) && (l > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = l * (2 * m + n) + 2;

	dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

	if (dpx == NULL) {
		return mlib_MatrixMul_type(type_S32, type_S32, mode_Sat, x, y,
			m, l, n, n, z);
	}

	dpy = dpx + 2 * l * m;
	for (i = 0; i < (l * m); i++) {
		mlib_s32 xi = ((mlib_s32 *)x)[i];

		dpx[2 * i + 1] = xi & MLIB_U16_MAX;
		dpx[2 * i] = xi - dpx[2 * i + 1];
	}

	for (j = 0; j < l; j++)
		for (i = 0; i < n; i++)
			dpy[i * l + j] = ((mlib_s32 *)y)[j * n + i];

/* for purify not to throw UMR warning */
	dpy[n * l] = dpy[n * l + 1] = 0;

	for (j = 0; j < m; j++) {
		for (i = 0; i < n; i++) {
			static const mlib_d64 two26 = 67108864.0, two42 =
				4398046511104.0;
			mlib_d64 mul_l = 0, mul_lo_h = 0, mul_lo_l = 0;
			mlib_d64 mul_h = 0;

			k = 0;
			do {
				mlib_d64 mul_hi = 0, mul_lo = 0;
				mlib_d64 dx_hi = dpx[2 * (j * l + k)];
				mlib_d64 dx_lo = dpx[2 * (j * l + k) + 1];
				mlib_d64 dy = dpy[i * l + k];

				l_end = (l < (k + 64)) ? l : k + 64;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; k < l_end; k++) {
					mul_hi += dx_hi * dy;
					mul_lo += dx_lo * dy;
					dx_hi = dpx[2 * (j * l + k + 1)];
					dx_lo = dpx[2 * (j * l + k + 1) + 1];
					dy = dpy[i * l + k + 1];
				}

				mul_lo_h +=
					((mlib_s32)(1.0 / two26 * mul_lo)) *
					two26;
				mul_lo_l +=
					mul_lo -
					((mlib_s32)(1.0 / two26 * mul_lo)) *
					two26;
				mul_h += ((mlib_s32)(1.0 / two42 * mul_hi)) *
					two42;
				mul_l += mul_hi -
					((mlib_s32)(1.0 / two42 * mul_hi)) *
					two42;
/* because of the compiler error */
				k = l_end;
			}

			while (l_end < l);

			mul_h += mul_lo_l + mul_lo_h + mul_l;
#ifndef MLIB_USE_FTOI_CLAMPING

			if (mul_h > MLIB_S32_MAX)
				mul_h = MLIB_S32_MAX;
			else if (mul_h < MLIB_S32_MIN)
				mul_h = MLIB_S32_MIN;
#endif /* MLIB_USE_FTOI_CLAMPING */
			((mlib_s32 *)z)[j * n + i] = (mlib_s32)mul_h;
		}
	}

	__mlib_free(dpx);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMul_S32_S32_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 l,
	mlib_s32 n)
{
	mlib_s32 size, i, j, k, l_end;
	mlib_d64 *dpx, *dpy;

	if (!((m > 0) && (l > 0) && (n > 0)))
		return (MLIB_FAILURE);

	size = l * (2 * m + n) + 2;
	dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

	if (dpx == NULL) {
		return mlib_MatrixMul_type(type_S32, type_S32, mode_Mod, x, y,
			m, l, n, n, z);
	}

	dpy = dpx + 2 * l * m;

	for (i = 0; i < (l * m); i++) {
		mlib_s32 xi = ((mlib_s32 *)x)[i];

		dpx[2 * i + 1] = xi & MLIB_U16_MAX;
		dpx[2 * i] = xi - dpx[2 * i + 1];
	}

	for (j = 0; j < l; j++)
		for (i = 0; i < n; i++)
			dpy[i * l + j] = ((mlib_s32 *)y)[j * n + i];

/* for purify not to throw UMR warning */
	dpy[n * l] = dpy[n * l + 1] = 0;

	for (j = 0; j < m; j++) {
		for (i = 0; i < n; i++) {
			static const mlib_d64 two26 = 67108864.0, two42 =
				4398046511104.0;
			mlib_d64 mul_l = 0, mul_lo_h = 0, mul_lo_l = 0;

			k = 0;
			do {
				mlib_d64 mul_hi = 0, mul_lo = 0;
				mlib_d64 dx_hi = dpx[2 * (j * l + k)];
				mlib_d64 dx_lo = dpx[2 * (j * l + k) + 1];
				mlib_d64 dy = dpy[i * l + k];

				l_end = (l < (k + 64)) ? l : k + 64;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; k < l_end; k++) {
					mul_hi += dx_hi * dy;
					mul_lo += dx_lo * dy;
					dx_hi = dpx[2 * (j * l + k + 1)];
					dx_lo = dpx[2 * (j * l + k + 1) + 1];
					dy = dpy[i * l + k + 1];
				}

				mul_lo_h +=
					((mlib_s32)(1.0 / two26 * mul_lo)) *
					two26;
				mul_lo_l +=
					mul_lo -
					((mlib_s32)(1.0 / two26 * mul_lo)) *
					two26;
				mul_l += mul_hi -
					((mlib_s32)(1.0 / two42 * mul_hi)) *
					two42;
/* because of the compiler error */
				k = l_end;
			}

			while (l_end < l);

			((mlib_s32 *)z)[j * n + i] =
				mlib_D64_to_S32_Mod(mul_lo_l) +
				mlib_D64_to_S32_Mod(mul_lo_h) +
				mlib_D64_to_S32_Mod(mul_l);
		}
	}

	__mlib_free(dpx);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMul_S32C_S32C_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 l,
	mlib_s32 n)
{
	mlib_s32 size, i, j, k, l_end;
	mlib_d64 array[MAX_SIZE], *dpx, *dpy;

	if (!((m > 0) && (l > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = l * 2 * (2 * m + n);

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_S32C, type_S32C,
				mode_Sat, x, y, m, l, n, n, z);
	}

	dpy = dpx + 4 * l * m;
	for (i = 0; i < (2 * l * m); i++) {
		mlib_s32 xi = ((mlib_s32 *)x)[i];

		dpx[2 * i + 1] = xi & MLIB_U16_MAX;
		dpx[2 * i] = xi - dpx[2 * i + 1];
	}

	for (j = 0; j < l; j++)
		for (i = 0; i < n; i++) {
			dpy[2 * (i * l + j)] = ((mlib_s32 *)y)[2 * (j * n + i)];
			dpy[2 * (i * l + j) + 1] =
				((mlib_s32 *)y)[2 * (j * n + i) + 1];
		}

	for (j = 0; j < m; j++)
		for (i = 0; i < n; i++) {
			static const mlib_d64 two26 = 67108864.0, two42 =
				4398046511104.0;
			mlib_d64 mul_l_r = 0, mul_lo_h_r = 0, mul_lo_l_r = 0;
			mlib_d64 mul_l_i = 0, mul_lo_h_i = 0, mul_lo_l_i = 0;
			mlib_d64 mul_h_r = 0, mul_h_i = 0;

			k = 0;
			do {
				mlib_d64 mul_r = 0, mul_lo_r = 0, mul_i =
					0, mul_lo_i = 0;

				l_end = (l < (k + 32)) ? l : k + 32;
				for (; k < l_end; k++) {
					mlib_d64 dx_hi_r = dpx[4 * (j * l + k)];
					mlib_d64 dx_lo_r =
						dpx[4 * (j * l + k) + 1];
					mlib_d64 dx_hi_i =
						dpx[4 * (j * l + k) + 2];
					mlib_d64 dx_lo_i =
						dpx[4 * (j * l + k) + 3];
					mlib_d64 dy_r = dpy[2 * (i * l + k)];
					mlib_d64 dy_i =
						dpy[2 * (i * l + k) + 1];

					mul_r += dx_hi_r * dy_r -
						dx_hi_i * dy_i;
					mul_i += dx_hi_i * dy_r +
						dx_hi_r * dy_i;
					mul_lo_r +=
						dx_lo_r * dy_r - dx_lo_i * dy_i;
					mul_lo_i +=
						dx_lo_i * dy_r + dx_lo_r * dy_i;
				}

				mul_lo_h_r +=
					((mlib_s32)(1.0 / two26 * mul_lo_r)) *
					two26;
				mul_lo_l_r +=
					mul_lo_r -
					((mlib_s32)(1.0 / two26 * mul_lo_r)) *
					two26;
				mul_lo_h_i +=
					((mlib_s32)(1.0 / two26 * mul_lo_i)) *
					two26;
				mul_lo_l_i +=
					mul_lo_i -
					((mlib_s32)(1.0 / two26 * mul_lo_i)) *
					two26;
				mul_h_r +=
					((mlib_s32)(1.0 / two42 * mul_r)) *
					two42;
				mul_h_i +=
					((mlib_s32)(1.0 / two42 * mul_i)) *
					two42;
				mul_l_r +=
					mul_r -
					((mlib_s32)(1.0 / two42 * mul_r)) *
					two42;
				mul_l_i +=
					mul_i -
					((mlib_s32)(1.0 / two42 * mul_i)) *
					two42;
			}

			while (l_end < l);
			mul_h_r += mul_lo_l_r + mul_lo_h_r + mul_l_r;
			mul_h_i += mul_lo_l_i + mul_lo_h_i + mul_l_i;
#ifndef MLIB_USE_FTOI_CLAMPING

			if (mul_h_r > MLIB_S32_MAX)
				mul_h_r = MLIB_S32_MAX;
			else if (mul_h_r < MLIB_S32_MIN)
				mul_h_r = MLIB_S32_MIN;

			if (mul_h_i > MLIB_S32_MAX)
				mul_h_i = MLIB_S32_MAX;
			else if (mul_h_i < MLIB_S32_MIN)
				mul_h_i = MLIB_S32_MIN;
#endif /* MLIB_USE_FTOI_CLAMPING */
			((mlib_s32 *)z)[2 * (j * n + i)] = (mlib_s32)mul_h_r;
			((mlib_s32 *)z)[2 * (j * n + i) + 1] =
				(mlib_s32)mul_h_i;
		}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMul_S32C_S32C_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 l,
	mlib_s32 n)
{
	mlib_s32 size, i, j, k, l_end;
	mlib_d64 array[MAX_SIZE], *dpx, *dpy;

	if (!((m > 0) && (l > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = l * 2 * (2 * m + n);

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_S32C, type_S32C,
				mode_Mod, x, y, m, l, n, n, z);
	}

	dpy = dpx + 4 * l * m;
	for (i = 0; i < (2 * l * m); i++) {
		mlib_s32 xi = ((mlib_s32 *)x)[i];

		dpx[2 * i + 1] = xi & MLIB_U16_MAX;
		dpx[2 * i] = xi - dpx[2 * i + 1];
	}

	for (j = 0; j < l; j++)
		for (i = 0; i < n; i++) {
			dpy[2 * (i * l + j)] = ((mlib_s32 *)y)[2 * (j * n + i)];
			dpy[2 * (i * l + j) + 1] =
				((mlib_s32 *)y)[2 * (j * n + i) + 1];
		}

	for (j = 0; j < m; j++) {
		for (i = 0; i < n; i++) {
			static const mlib_d64 two26 = 67108864.0, two42 =
				4398046511104.0;
			mlib_d64 mul_l_r = 0, mul_lo_h_r = 0, mul_lo_l_r =
				0, mul_l_i = 0, mul_lo_h_i = 0, mul_lo_l_i = 0;

			k = 0;
			do {
				mlib_d64 mul_r = 0, mul_lo_r = 0, mul_i =
					0, mul_lo_i = 0;

				l_end = (l < (k + 32)) ? l : k + 32;
				for (; k < l_end; k++) {
					mlib_d64 dx_hi_r = dpx[4 * (j * l + k)];
					mlib_d64 dx_lo_r =
						dpx[4 * (j * l + k) + 1];
					mlib_d64 dx_hi_i =
						dpx[4 * (j * l + k) + 2];
					mlib_d64 dx_lo_i =
						dpx[4 * (j * l + k) + 3];
					mlib_d64 dy_r = dpy[2 * (i * l + k)];
					mlib_d64 dy_i =
						dpy[2 * (i * l + k) + 1];

					mul_r += dx_hi_r * dy_r -
						dx_hi_i * dy_i;
					mul_i += dx_hi_i * dy_r +
						dx_hi_r * dy_i;
					mul_lo_r +=
						dx_lo_r * dy_r - dx_lo_i * dy_i;
					mul_lo_i +=
						dx_lo_i * dy_r + dx_lo_r * dy_i;
				}

				mul_lo_h_r +=
					((mlib_s32)(1.0 / two26 * mul_lo_r)) *
					two26;
				mul_lo_l_r +=
					mul_lo_r -
					((mlib_s32)(1.0 / two26 * mul_lo_r)) *
					two26;
				mul_lo_h_i +=
					((mlib_s32)(1.0 / two26 * mul_lo_i)) *
					two26;
				mul_lo_l_i +=
					mul_lo_i -
					((mlib_s32)(1.0 / two26 * mul_lo_i)) *
					two26;
				mul_l_r +=
					mul_r -
					((mlib_s32)(1.0 / two42 * mul_r)) *
					two42;
				mul_l_i +=
					mul_i -
					((mlib_s32)(1.0 / two42 * mul_i)) *
					two42;
			}

			while (l_end < l);
			((mlib_s32 *)z)[2 * (j * n + i)] =
				mlib_D64_to_S32_Mod(mul_lo_l_r) +
				mlib_D64_to_S32_Mod(mul_lo_h_r) +
				mlib_D64_to_S32_Mod(mul_l_r);
			((mlib_s32 *)z)[2 * (j * n + i) + 1] =
				mlib_D64_to_S32_Mod(mul_lo_l_i) +
				mlib_D64_to_S32_Mod(mul_lo_h_i) +
				mlib_D64_to_S32_Mod(mul_l_i);
		}
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
