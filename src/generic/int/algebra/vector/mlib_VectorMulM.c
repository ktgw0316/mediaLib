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

#pragma ident	"@(#)mlib_VectorMulM.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorMulM_U8_U8[Sat, Mod]   - multiplication of unsigned 8-bit
 *                                          format vector by matrix
 *      mlib_VectorMulM_S8_S8[Sat, Mod]   - multiplication of signed 8-bit
 *                                          format vector by matrix
 *      mlib_VectorMulM_S16_S16[Sat, Mod] - multiplication of signed 16-bit
 *                                          format vector by matrix
 *      mlib_VectorMulM_S32_S32[Sat, Mod] - multiplication of signed 32-bit
 *                                          format vector by matrix
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMulM_U8_U8[Sat, Mod](mlib_u8       *z,
 *                                                 const mlib_u8 *x,
 *                                                 const mlib_u8 *y,
 *                                                 mlib_s32      m,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_VectorMulM_S8_S8[Sat, Mod](mlib_s8       *z,
 *                                                 const mlib_s8 *x,
 *                                                 const mlib_s8 *y,
 *                                                 mlib_s32      m,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_VectorMulM_S16_S16[Sat, Mod](mlib_s16       *z,
 *                                                   const mlib_s16 *x,
 *                                                   const mlib_s16 *y,
 *                                                   mlib_s32       m,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_VectorMulM_S32_S32[Sat, Mod](mlib_s32       *z,
 *                                                   const mlib_s32 *x,
 *                                                   const mlib_s32 *y,
 *                                                   mlib_s32       m,
 *                                                   mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      x    pointer to the first element of the input vector
 *      y    pointer to the first element of the input matrix
 *      m    number of rows in the matrix, and number of elements
 *           in the input vector
 *      n    number of columns in the matrix, and number of elements
 *           in the result vector
 *
 * DESCRIPTION
 *              m
 *      z[i] = SUM (x[k] * y[k][i])
 *             k = 1
 */

/*
 * FUNCTIONS
 *      mlib_VectorMulM_U8C_U8C_[Sat|Mod]    - multiplication of unsigned 8-bit
 *                                             format complex vector
 *                                             by complex matrix
 *      mlib_VectorMulM_S8C_S8C_[Sat|Mod]    - multiplication of signed 8-bit
 *                                             format complex vector
 *                                             by complex matrix
 *      mlib_VectorMulM_S16C_S16C_[Sat|Mod]  - multiplication of signed 16-bit
 *                                             format complex vector
 *                                             by complex matrix
 *       mlib_VectorMulM_S32C_S32C_[Sat|Mod] - multiplication of signed 32-bit
 *                                             format complex vector
 *                                             by complex matrix
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorMulM_U8C_U8C[Sat, Mod](mlib_u8       *z,
 *                                                   const mlib_u8 *x,
 *                                                   const mlib_u8 *y,
 *                                                   mlib_s32      m,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_VectorMulM_S8C_S8C[Sat, Mod](mlib_s8       *z,
 *                                                   const mlib_s8 *x,
 *                                                   const mlib_s8 *y,
 *                                                   mlib_s32      m,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_VectorMulM_S16C_S16C[Sat, Mod](mlib_s16       *z,
 *                                                     const mlib_s16 *x,
 *                                                     const mlib_s16 *y,
 *                                                     mlib_s32       m,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_VectorMulM_S32C_S32C[Sat, Mod](mlib_s32       *z,
 *                                                     const mlib_s32 *x,
 *                                                     const mlib_s32 *y,
 *                                                     mlib_s32       m,
 *                                                     mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts
 *      x    pointer to the first element of the input vector
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      y    pointer to the first element of the input matrix
 *           y[2*i] hold the real parts, and y[2*i + 1] hold the imaginary
 *           parts
 *      m    number of rows in the matrix, and number of elements
 *           in the input vector
 *      n    number of columns in the matrix, and number of elements
 *           in the result vector
 *
 * DESCRIPTION
 *              m
 *      z[i] = SUM (x[k] * y[k][i])
 *             k = 1
 */

#include <mlib_algebra.h>
#include <mlib_VectorMulM.h>
#include <mlib_VectorMulM_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorMulM_S32C_S32C_Mod = __mlib_VectorMulM_S32C_S32C_Mod
#pragma weak mlib_VectorMulM_S32C_S32C_Sat = __mlib_VectorMulM_S32C_S32C_Sat
#pragma weak mlib_VectorMulM_S32_S32_Mod = __mlib_VectorMulM_S32_S32_Mod
#pragma weak mlib_VectorMulM_S32_S32_Sat = __mlib_VectorMulM_S32_S32_Sat

#elif defined(__GNUC__)

__typeof__(__mlib_VectorMulM_S32C_S32C_Mod) mlib_VectorMulM_S32C_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S32C_S32C_Mod")));
__typeof__(__mlib_VectorMulM_S32C_S32C_Sat) mlib_VectorMulM_S32C_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S32C_S32C_Sat")));
__typeof__(__mlib_VectorMulM_S32_S32_Mod) mlib_VectorMulM_S32_S32_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S32_S32_Mod")));
__typeof__(__mlib_VectorMulM_S32_S32_Sat) mlib_VectorMulM_S32_S32_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S32_S32_Sat")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

#ifndef _NO_LONGLONG

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_S32_S32_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 i, k;

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);

	for (i = 0; i < n; i++) {
		mlib_s64 sum_h = 0, sum_l = 0, prod;
		for (k = 0; k < m; k++) {
			prod = (mlib_s64)x[k] * y[k * nstride + i];
			sum_h += prod >> 31;
			sum_l += prod & 0x000000007fffffff;
		}
		sum_h += sum_l >> 31;

		if (sum_h > 0)
			z[i] = MLIB_S32_MAX;
		else if (sum_h < -1)
			z[i] = MLIB_S32_MIN;
		else
			z[i] = ((mlib_s32)sum_l & 0x7fffffff)
				| ((mlib_s32)sum_h & 0x80000000);
		}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_S32_S32_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 i, k;

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);

	for (i = 0; i < n; i++) {
		mlib_s32 sum = 0;
		for (k = 0; k < m; k++) {
			sum += (mlib_s32)x[k] * y[k * nstride + i];
		}
		z[i] = (mlib_s32)sum;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_S32C_S32C_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 i, k;

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);

	for (i = 0; i < n; i++) {
		mlib_s64 sum_r_h = 0, sum_r_l = 0;
		mlib_s64 sum_i_h = 0, sum_i_l = 0;
		mlib_s64 prod;
		for (k = 0; k < m; k++) {
			prod = (mlib_s64)x[2 *k]
				* y[2 * (k * nstride + i)]
				- (mlib_s64)x[2 * k + 1]
				* y[2 * (k * nstride + i) + 1];
			sum_r_h += prod >> 31;
			sum_r_l += prod & 0x000000007fffffff;
			prod = (mlib_s64)x[2 * k]
				* y[2 * (k * nstride + i) + 1]
				+ (mlib_s64)x[2 * k + 1]
				* y[2 * (k * nstride + i)];
			sum_i_h += prod >> 31;
			sum_i_l += prod & 0x000000007fffffff;
		}

		sum_r_h += sum_r_l >> 31;
		sum_i_h += sum_i_l >> 31;

		if (sum_r_h > 0)
			z[2 * i] = MLIB_S32_MAX;
		else if (sum_r_h < -1)
			z[2 * i] = MLIB_S32_MIN;
		else
			z[2 * i] = ((mlib_s32)sum_r_l & 0x7fffffff)
				| ((mlib_s32)sum_r_h & 0x80000000);

		if (sum_i_h > 0)
			z[2 * i + 1] = MLIB_S32_MAX;
		else if (sum_i_h < -1)
			z[2 * i + 1] = MLIB_S32_MIN;
		else
			z[2 * i + 1] = ((mlib_s32)sum_i_l & 0x7fffffff)
				| ((mlib_s32)sum_i_h & 0x80000000);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_S32C_S32C_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 i, k;

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);

	for (i = 0; i < n; i++) {
		mlib_s32 sum_r = 0, sum_i = 0;
		for (k = 0; k < m; k++) {
			sum_r += (mlib_s32)x[2 *k]
				* y[2 * (k * nstride + i)]
				- (mlib_s32)x[2 * k + 1]
				* y[2 * (k * nstride + i) + 1];
			sum_i += (mlib_s32)x[2 * k]
				* y[2 * (k * nstride + i) + 1]
				+ (mlib_s32)x[2 * k + 1]
				* y[2 * (k * nstride + i)];
		}
		z[2 * i] = (mlib_s32)sum_r;
		z[2 * i + 1] = (mlib_s32)sum_i;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulM_S32_S32_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S32_S32_Sat(z,
		x, y, m, n, n);
}

mlib_status
__mlib_VectorMulM_S32_S32_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S32_S32_Mod(z,
		x, y, m, n, n);
}

mlib_status
__mlib_VectorMulM_S32C_S32C_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S32C_S32C_Sat(z,
		x, y, m, n, n);
}

mlib_status
__mlib_VectorMulM_S32C_S32C_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S32C_S32C_Mod(z,
		x, y, m, n, n);
}

/* *********************************************************** */

#else	/* _NO_LONGLONG */

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_S32_S32_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 size, i, k, m_end;
	mlib_d64 array[MAX_SIZE], *dpx;

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = m * 2;

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_S32, type_S32, mode_Sat,
				x, y, 1, m, n, nstride, z);
	}

	for (i = 0; i < m; i++) {
		mlib_s32 xi = ((mlib_s32 *)x)[i];

		dpx[2 * i + 1] = xi & MLIB_U16_MAX;
		dpx[2 * i] = xi - dpx[2 * i + 1];
	}

	for (i = 0; i < n; i++) {
		static const mlib_d64 two26 = 67108864.0, two42 =
			4398046511104.0;
		mlib_d64 mul_l = 0, mul_lo_h = 0, mul_lo_l = 0;
		mlib_d64 mul_h = 0;

		k = 0;
		do {
			mlib_d64 mul = 0, mul_lo = 0;

			m_end = (m < (k + 64)) ? m : k + 64;
			for (; k < m_end; k++) {
				mlib_d64 dy = ((mlib_s32 *)y)[k * nstride + i];
				mlib_d64 dx_hi = dpx[2 * k], dx_lo =
					dpx[2 * k + 1];

				mul += dx_hi * dy;
				mul_lo += dx_lo * dy;
			}

			mul_lo_h += ((mlib_s32)(1.0 / two26 * mul_lo)) * two26;
			mul_lo_l +=
				mul_lo -
				((mlib_s32)(1.0 / two26 * mul_lo)) * two26;
			mul_h += ((mlib_s32)(1.0 / two42 * mul)) * two42;
			mul_l += mul - ((mlib_s32)(1.0 / two42 * mul)) * two42;
/* because of the compiler error */
			k = m_end;
		}

		while (m_end < m);
		mul_h += mul_lo_l + mul_lo_h + mul_l;

#ifndef MLIB_USE_FTOI_CLAMPING

		if (mul_h > MLIB_S32_MAX)
			mul_h = MLIB_S32_MAX;
		else if (mul_h < MLIB_S32_MIN)
			mul_h = MLIB_S32_MIN;

#endif /* MLIB_USE_FTOI_CLAMPING */

		((mlib_s32 *)z)[i] = (mlib_s32)mul_h;
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_S32_S32_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 size, i, k, m_end;
	mlib_d64 array[MAX_SIZE], *dpx;

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = m * 2;

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_S32, type_S32, mode_Mod,
				x, y, 1, m, n, nstride, z);
	}

	for (i = 0; i < m; i++) {
		mlib_s32 xi = ((mlib_s32 *)x)[i];

		dpx[2 * i + 1] = xi & MLIB_U16_MAX;
		dpx[2 * i] = xi - dpx[2 * i + 1];
	}

	for (i = 0; i < n; i++) {
		static const mlib_d64 two26 = 67108864.0;
		static const mlib_d64 two42 = 4398046511104.0;
		mlib_d64 mul_l = 0, mul_lo_h = 0, mul_lo_l = 0;

		k = 0;
		do {
			mlib_d64 mul = 0, mul_lo = 0;

			m_end = (m < (k + 64)) ? m : k + 64;
			for (; k < m_end; k++) {
				mlib_d64 dy = ((mlib_s32 *)y)[k * nstride + i];
				mlib_d64 dx_hi = dpx[2 * k], dx_lo =
					dpx[2 * k + 1];

				mul += dx_hi * dy;
				mul_lo += dx_lo * dy;
			}

			mul_lo_h += ((mlib_s32)(1.0 / two26 * mul_lo)) * two26;
			mul_lo_l +=
				mul_lo -
				((mlib_s32)(1.0 / two26 * mul_lo)) * two26;
			mul_l += mul - ((mlib_s32)(1.0 / two42 * mul)) * two42;
/* because of the compiler error */
			k = m_end;
		}

		while (m_end < m);
		((mlib_s32 *)z)[i] = mlib_D64_to_S32_Mod(mul_lo_l) +
			mlib_D64_to_S32_Mod(mul_lo_h) +
			mlib_D64_to_S32_Mod(mul_l);
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_S32C_S32C_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 size, i, k, m_end;
	mlib_d64 array[MAX_SIZE], *dpx;

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = m * 4;

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_S32C, type_S32C,
				mode_Sat, x, y, 1, m, n, nstride, z);
	}

	for (i = 0; i < m; i++) {
		mlib_s32 xi_r = ((mlib_s32 *)x)[2 * i], xi_i =
			((mlib_s32 *)x)[2 * i + 1];

		dpx[4 * i + 1] = xi_r & MLIB_U16_MAX;
		dpx[4 * i] = xi_r - dpx[4 * i + 1];
		dpx[4 * i + 3] = xi_i & MLIB_U16_MAX;
		dpx[4 * i + 2] = xi_i - dpx[4 * i + 3];
	}

	for (i = 0; i < n; i++) {
		static const mlib_d64 two26 = 67108864.0, two42 =
			4398046511104.0;
		mlib_d64 mul_l_r = 0, mul_lo_h_r = 0, mul_lo_l_r = 0;
		mlib_d64 mul_l_i = 0, mul_lo_h_i = 0, mul_lo_l_i = 0;
		mlib_d64 mul_h_r = 0, mul_h_i = 0;

		k = 0;
		do {
			mlib_d64 mul_r = 0, mul_lo_r = 0, mul_i = 0, mul_lo_i =
				0;

			m_end = (m < (k + 32)) ? m : k + 32;
			for (; k < m_end; k++) {
				mlib_d64 dx_hi_r = dpx[4 * k];
				mlib_d64 dx_lo_r = dpx[4 * k + 1];
				mlib_d64 dx_hi_i = dpx[4 * k + 2];
				mlib_d64 dx_lo_i = dpx[4 * k + 3];
				mlib_d64 dy_r =
					((mlib_s32 *)y)[2 * (k * nstride + i)];
				mlib_d64 dy_i =
					((mlib_s32 *)y)
						[2 * (k * nstride + i) + 1];

				mul_r += dx_hi_r * dy_r - dx_hi_i * dy_i;
				mul_i += dx_hi_i * dy_r + dx_hi_r * dy_i;
				mul_lo_r += dx_lo_r * dy_r - dx_lo_i * dy_i;
				mul_lo_i += dx_lo_i * dy_r + dx_lo_r * dy_i;
			}

			mul_lo_h_r +=
				((mlib_s32)(1.0 / two26 * mul_lo_r)) * two26;
			mul_lo_l_r +=
				mul_lo_r -
				((mlib_s32)(1.0 / two26 * mul_lo_r)) * two26;
			mul_lo_h_i +=
				((mlib_s32)(1.0 / two26 * mul_lo_i)) * two26;
			mul_lo_l_i +=
				mul_lo_i -
				((mlib_s32)(1.0 / two26 * mul_lo_i)) * two26;
			mul_h_r += ((mlib_s32)(1.0 / two42 * mul_r)) * two42;
			mul_h_i += ((mlib_s32)(1.0 / two42 * mul_i)) * two42;
			mul_l_r +=
				mul_r -
				((mlib_s32)(1.0 / two42 * mul_r)) * two42;
			mul_l_i +=
				mul_i -
				((mlib_s32)(1.0 / two42 * mul_i)) * two42;
		}

		while (m_end < m);
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

		((mlib_s32 *)z)[2 * i] = (mlib_s32)mul_h_r;
		((mlib_s32 *)z)[2 * i + 1] = (mlib_s32)mul_h_i;
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_S32C_S32C_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 size, i, k, m_end;
	mlib_d64 array[MAX_SIZE], *dpx;

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = m * 4;

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_S32C, type_S32C,
				mode_Mod, x, y, 1, m, n, nstride, z);
	}

	for (i = 0; i < m; i++) {
		mlib_s32 xi_r = ((mlib_s32 *)x)[2 * i], xi_i =
			((mlib_s32 *)x)[2 * i + 1];

		dpx[4 * i + 1] = xi_r & MLIB_U16_MAX;
		dpx[4 * i] = xi_r - dpx[4 * i + 1];
		dpx[4 * i + 3] = xi_i & MLIB_U16_MAX;
		dpx[4 * i + 2] = xi_i - dpx[4 * i + 3];
	}

	for (i = 0; i < n; i++) {
		static const mlib_d64 two26 = 67108864.0, two42 =
			4398046511104.0;
		mlib_d64 mul_l_r = 0, mul_lo_h_r = 0, mul_lo_l_r = 0;
		mlib_d64 mul_l_i = 0, mul_lo_h_i = 0, mul_lo_l_i = 0;

		k = 0;
		do {
			mlib_d64 mul_r = 0, mul_lo_r = 0, mul_i = 0, mul_lo_i =
				0;

			m_end = (m < (k + 32)) ? m : k + 32;
			for (; k < m_end; k++) {
				mlib_d64 dx_hi_r = dpx[4 * k], dx_lo_r =
					dpx[4 * k + 1];
				mlib_d64 dx_hi_i = dpx[4 * k + 2];
				mlib_d64 dx_lo_i = dpx[4 * k + 3];
				mlib_d64 dy_r =
					((mlib_s32 *)y)[2 * (k * nstride + i)];
				mlib_d64 dy_i =
					((mlib_s32 *)y)
						[2 * (k * nstride + i) + 1];

				mul_r += dx_hi_r * dy_r - dx_hi_i * dy_i;
				mul_i += dx_hi_i * dy_r + dx_hi_r * dy_i;
				mul_lo_r += dx_lo_r * dy_r - dx_lo_i * dy_i;
				mul_lo_i += dx_lo_i * dy_r + dx_lo_r * dy_i;
			}

			mul_lo_h_r +=
				((mlib_s32)(1.0 / two26 * mul_lo_r)) * two26;
			mul_lo_l_r +=
				mul_lo_r -
				((mlib_s32)(1.0 / two26 * mul_lo_r)) * two26;
			mul_lo_h_i +=
				((mlib_s32)(1.0 / two26 * mul_lo_i)) * two26;
			mul_lo_l_i +=
				mul_lo_i -
				((mlib_s32)(1.0 / two26 * mul_lo_i)) * two26;
			mul_l_r +=
				mul_r -
				((mlib_s32)(1.0 / two42 * mul_r)) * two42;
			mul_l_i +=
				mul_i -
				((mlib_s32)(1.0 / two42 * mul_i)) * two42;
		}

		while (m_end < m);
		((mlib_s32 *)z)[2 * i] = mlib_D64_to_S32_Mod(mul_lo_l_r) +
			mlib_D64_to_S32_Mod(mul_lo_h_r) +
			mlib_D64_to_S32_Mod(mul_l_r);
		((mlib_s32 *)z)[2 * i + 1] = mlib_D64_to_S32_Mod(mul_lo_l_i) +
			mlib_D64_to_S32_Mod(mul_lo_h_i) +
			mlib_D64_to_S32_Mod(mul_l_i);
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulM_S32_S32_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S32_S32_Sat(z,
		x, y, m, n, n);
}

mlib_status
__mlib_VectorMulM_S32_S32_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S32_S32_Mod(z,
		x, y, m, n, n);
}

mlib_status
__mlib_VectorMulM_S32C_S32C_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S32C_S32C_Sat(z,
		x, y, m, n, n);
}

mlib_status
__mlib_VectorMulM_S32C_S32C_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S32C_S32C_Mod(z,
		x, y, m, n, n);
}

/* *********************************************************** */

#endif	/* _NO_LONGLONG */
