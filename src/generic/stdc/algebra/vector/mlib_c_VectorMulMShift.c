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

#pragma ident	"@(#)mlib_c_VectorMulMShift.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *     mlib_VectorMulMShift_[S16_S16|S16C_S16C]_[Sat|Mod] -
 *          Multiplication of a signed 16-bit vector by a matrix with shifting
 *
 * SYNOPSIS
 *     mlib_status mlib_VectorMulMShift_S16_S16_Sat(mlib_s16       *z,
 *                                                  const mlib_s16 *x,
 *                                                  const mlib_s16 *y,
 *                                                  mlib_s32       m,
 *                                                  mlib_s32       n,
 *                                                  mlib_s32       shift)
 *     mlib_status mlib_VectorMulMShift_S16_S16_Mod(mlib_s16       *z,
 *                                                  const mlib_s16 *x,
 *                                                  const mlib_s16 *y,
 *                                                  mlib_s32       m,
 *                                                  mlib_s32       n,
 *                                                  mlib_s32       shift)
 *     mlib_status mlib_VectorMulMShift_S16C_S16C_Sat(mlib_s16       *z,
 *                                                    const mlib_s16 *x,
 *                                                    const mlib_s16 *y,
 *                                                    mlib_s32       m,
 *                                                    mlib_s32       n,
 *                                                    mlib_s32       shift)
 *     mlib_status mlib_VectorMulMShift_S16C_S16C_Mod(mlib_s16       *z,
 *                                                    const mlib_s16 *x,
 *                                                    const mlib_s16 *y,
 *                                                    mlib_s32       m,
 *                                                    mlib_s32       n,
 *                                                    mlib_s32       shift)
 *
 * ARGUMENTS
 *     z          the pointer to the first element of the result vector
 *     x          the pointer to the first element of the input vector
 *     y          the pointer to the first element of the input matrix in
 *                row - major order
 *     m          the number of rows in the matrix, and number of elements
 *                in the input vector
 *     n          the number of columns in the matrix, and number of elements
 *                in the result vector
 *     shift      the right shifting factor
 *
 * DESCRIPTION
 *                  m
 *          z[i] = SUM (x[k] * y[k][i]) * 2^(-shift)
 *                 k = 1
 */

#include <mlib_algebra.h>
#include <mlib_AlgebraUtil.h>
#include <mlib_VectorMulM_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorMulMShift_S16_S16_Mod = \
	__mlib_VectorMulMShift_S16_S16_Mod
#pragma weak mlib_VectorMulMShift_S16_S16_Sat = \
	__mlib_VectorMulMShift_S16_S16_Sat
#pragma weak mlib_VectorMulMShift_S16C_S16C_Mod = \
	__mlib_VectorMulMShift_S16C_S16C_Mod
#pragma weak mlib_VectorMulMShift_S16C_S16C_Sat = \
	__mlib_VectorMulMShift_S16C_S16C_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorMulMShift_S16_S16_Mod) mlib_VectorMulMShift_S16_S16_Mod
	__attribute__((weak, alias("__mlib_VectorMulMShift_S16_S16_Mod")));
__typeof__(__mlib_VectorMulMShift_S16_S16_Sat) mlib_VectorMulMShift_S16_S16_Sat
	__attribute__((weak, alias("__mlib_VectorMulMShift_S16_S16_Sat")));
__typeof__(__mlib_VectorMulMShift_S16C_S16C_Mod)
	mlib_VectorMulMShift_S16C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMulMShift_S16C_S16C_Mod")));
__typeof__(__mlib_VectorMulMShift_S16C_S16C_Sat)
	mlib_VectorMulMShift_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorMulMShift_S16C_S16C_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifndef MLIB_USE_FTOI_CLAMPING

#define	shift_round_Sat(out, in, divisor)                       \
	{                                                       \
	    mlib_d64 val = in * divisor;                        \
	                                                        \
	    if (val > MLIB_S32_MAX)                             \
		out = MLIB_S16_MAX;                             \
	    else if (val < MLIB_S32_MIN)                        \
		out = MLIB_S16_MIN;                             \
	    else                                                \
		out = ((mlib_s32)val) >> 16;                    \
	}

#else /* MLIB_USE_FTOI_CLAMPING */

#define	shift_round_Sat(out, in, divisor)                       \
	out = ((mlib_s32)(in * divisor)) >> 16

#endif /* MLIB_USE_FTOI_CLAMPING */

#define	shift_round_Mod(out, in, shift)                         \
	out = mlib_D64_to_S32_Mod(in) >> shift

/* *********************************************************** */

mlib_status
mlib_VectorMulMShift_S_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ns,
	mlib_s32 shift)
{
	mlib_s32 i, j;
	mlib_d64 divisor = 1 << (16 - shift);

	if (m <= 0 || n <= 0)
		return (MLIB_FAILURE);

	if (shift < 1 || shift > 16)
		return (MLIB_OUTOFRANGE);

	for (i = 0; i <= (n - 4); i += 4) {
		mlib_d64 dz0 = 0, dz1 = 0, dz2 = 0, dz3 = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < m; j++) {
			mlib_d64 dxj = (mlib_d64)x[j];

			dz0 += dxj * (mlib_d64)y[j * ns + i];
			dz1 += dxj * (mlib_d64)y[j * ns + i + 1];
			dz2 += dxj * (mlib_d64)y[j * ns + i + 2];
			dz3 += dxj * (mlib_d64)y[j * ns + i + 3];
		}

		shift_round_Sat(z[i], dz0, divisor);
		shift_round_Sat(z[i + 1], dz1, divisor);
		shift_round_Sat(z[i + 2], dz2, divisor);
		shift_round_Sat(z[i + 3], dz3, divisor);
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n; i++) {
		mlib_d64 dz = 0;

		for (j = 0; j <= (m - 4); j += 4) {
			dz += ((mlib_d64)x[j]) * ((mlib_d64)y[j * ns + i]);
			dz += ((mlib_d64)x[j + 1]) * ((mlib_d64)y[(j + 1) * ns +
				i]);
			dz += ((mlib_d64)x[j + 2]) * ((mlib_d64)y[(j + 2) * ns +
				i]);
			dz += ((mlib_d64)x[j + 3]) * ((mlib_d64)y[(j + 3) * ns +
				i]);
		}

		for (; j < m; j++)
			dz += (mlib_d64)x[j] * (mlib_d64)y[j * ns + i];

		shift_round_Sat(z[i], dz, divisor);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_VectorMulMShift_S_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ns,
	mlib_s32 shift)
{
	mlib_s32 i, j;

	if (m <= 0 || n <= 0)
		return (MLIB_FAILURE);

	if (shift < 1 || shift > 16)
		return (MLIB_OUTOFRANGE);

	for (i = 0; i <= (n - 4); i += 4) {
		mlib_d64 dz0 = 0, dz1 = 0, dz2 = 0, dz3 = 0;

		for (j = 0; j < m; j++) {
			mlib_d64 dxj = (mlib_d64)x[j];

			dz0 += dxj * (mlib_d64)y[j * ns + i];
			dz1 += dxj * (mlib_d64)y[j * ns + i + 1];
			dz2 += dxj * (mlib_d64)y[j * ns + i + 2];
			dz3 += dxj * (mlib_d64)y[j * ns + i + 3];
		}

		shift_round_Mod(z[i], dz0, shift);
		shift_round_Mod(z[i + 1], dz1, shift);
		shift_round_Mod(z[i + 2], dz2, shift);
		shift_round_Mod(z[i + 3], dz3, shift);
	}

	for (; i < n; i++) {
		mlib_d64 dz = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (m - 4); j += 4) {
			dz += ((mlib_d64)x[j]) * ((mlib_d64)y[j * ns + i]);
			dz += ((mlib_d64)x[j + 1]) * ((mlib_d64)y[(j + 1) * ns +
				i]);
			dz += ((mlib_d64)x[j + 2]) * ((mlib_d64)y[(j + 2) * ns +
				i]);
			dz += ((mlib_d64)x[j + 3]) * ((mlib_d64)y[(j + 3) * ns +
				i]);
		}

		for (; j < m; j++)
			dz += (mlib_d64)x[j] * (mlib_d64)y[j * ns + i];

		shift_round_Mod(z[i], dz, shift);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_VectorMulMShift_S_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ns,
	mlib_s32 shift)
{
	mlib_s32 i, j;
	mlib_d64 divisor = 1 << (16 - shift);
	mlib_s32 ns2;

	if (m <= 0 || n <= 0)
		return (MLIB_FAILURE);

	if (shift < 1 || shift > 16)
		return (MLIB_OUTOFRANGE);

	ns2 = ns << 1;
	for (i = 0; i <= (n - 4); i += 4) {
		mlib_d64 dz0_r = 0, dz0_i = 0, dz1_r = 0, dz1_i = 0;
		mlib_d64 dz2_r = 0, dz2_i = 0, dz3_r = 0, dz3_i = 0;
		const mlib_s16 *addr = y + (i << 1);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < m; j++) {
			mlib_d64 dxj_r = (mlib_d64)x[j * 2];
			mlib_d64 dxj_i = (mlib_d64)x[j * 2 + 1];

			mlib_d64 dy0_r = (mlib_d64)addr[0];
			mlib_d64 dy0_i = (mlib_d64)addr[1];
			mlib_d64 dy1_r = (mlib_d64)addr[2];
			mlib_d64 dy1_i = (mlib_d64)addr[3];

			mlib_d64 dy2_r = (mlib_d64)addr[4];
			mlib_d64 dy2_i = (mlib_d64)addr[5];
			mlib_d64 dy3_r = (mlib_d64)addr[6];
			mlib_d64 dy3_i = (mlib_d64)addr[7];

			addr += ns2;

			dz0_r += dxj_r * dy0_r - dxj_i * dy0_i;
			dz0_i += dxj_r * dy0_i + dxj_i * dy0_r;
			dz1_r += dxj_r * dy1_r - dxj_i * dy1_i;
			dz1_i += dxj_r * dy1_i + dxj_i * dy1_r;
			dz2_r += dxj_r * dy2_r - dxj_i * dy2_i;
			dz2_i += dxj_r * dy2_i + dxj_i * dy2_r;
			dz3_r += dxj_r * dy3_r - dxj_i * dy3_i;
			dz3_i += dxj_r * dy3_i + dxj_i * dy3_r;
		}

		shift_round_Sat(z[i * 2 + 0], dz0_r, divisor);
		shift_round_Sat(z[i * 2 + 1], dz0_i, divisor);
		shift_round_Sat(z[i * 2 + 2], dz1_r, divisor);
		shift_round_Sat(z[i * 2 + 3], dz1_i, divisor);
		shift_round_Sat(z[i * 2 + 4], dz2_r, divisor);
		shift_round_Sat(z[i * 2 + 5], dz2_i, divisor);
		shift_round_Sat(z[i * 2 + 6], dz3_r, divisor);
		shift_round_Sat(z[i * 2 + 7], dz3_i, divisor);
	}

	for (; i < n; i++) {
		mlib_d64 dz_r = 0, dz_i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < m; j++) {
			mlib_s32 a = (j * ns + i) * 2;

			dz_r += ((mlib_d64)x[j * 2]) * ((mlib_d64)y[a]) -
				((mlib_d64)x[j * 2 + 1]) * ((mlib_d64)y[a + 1]);
			dz_i += ((mlib_d64)x[j * 2]) * ((mlib_d64)y[a + 1]) +
				((mlib_d64)x[j * 2 + 1]) * ((mlib_d64)y[a]);
		}

		shift_round_Sat(z[i * 2], dz_r, divisor);
		shift_round_Sat(z[i * 2 + 1], dz_i, divisor);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_VectorMulMShift_S_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ns,
	mlib_s32 shift)
{
	mlib_s32 i, j;
	mlib_s32 ns2;

	if (m <= 0 || n <= 0)
		return (MLIB_FAILURE);

	if (shift < 1 || shift > 16)
		return (MLIB_OUTOFRANGE);

	ns2 = ns << 1;

	for (i = 0; i <= (n - 4); i += 4) {
		mlib_d64 dz0_r = 0, dz0_i = 0, dz1_r = 0, dz1_i = 0;
		mlib_d64 dz2_r = 0, dz2_i = 0, dz3_r = 0, dz3_i = 0;
		const mlib_s16 *addr = y + (i << 1);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < m; j++) {
			mlib_d64 dxj_r = (mlib_d64)x[j * 2];
			mlib_d64 dxj_i = (mlib_d64)x[j * 2 + 1];

			mlib_d64 dy0_r = (mlib_d64)addr[0];
			mlib_d64 dy0_i = (mlib_d64)addr[1];
			mlib_d64 dy1_r = (mlib_d64)addr[2];
			mlib_d64 dy1_i = (mlib_d64)addr[3];

			mlib_d64 dy2_r = (mlib_d64)addr[4];
			mlib_d64 dy2_i = (mlib_d64)addr[5];
			mlib_d64 dy3_r = (mlib_d64)addr[6];
			mlib_d64 dy3_i = (mlib_d64)addr[7];

			addr += ns2;

			dz0_r += dxj_r * dy0_r - dxj_i * dy0_i;
			dz0_i += dxj_r * dy0_i + dxj_i * dy0_r;
			dz1_r += dxj_r * dy1_r - dxj_i * dy1_i;
			dz1_i += dxj_r * dy1_i + dxj_i * dy1_r;
			dz2_r += dxj_r * dy2_r - dxj_i * dy2_i;
			dz2_i += dxj_r * dy2_i + dxj_i * dy2_r;
			dz3_r += dxj_r * dy3_r - dxj_i * dy3_i;
			dz3_i += dxj_r * dy3_i + dxj_i * dy3_r;
		}

		shift_round_Mod(z[i * 2 + 0], dz0_r, shift);
		shift_round_Mod(z[i * 2 + 1], dz0_i, shift);
		shift_round_Mod(z[i * 2 + 2], dz1_r, shift);
		shift_round_Mod(z[i * 2 + 3], dz1_i, shift);
		shift_round_Mod(z[i * 2 + 4], dz2_r, shift);
		shift_round_Mod(z[i * 2 + 5], dz2_i, shift);
		shift_round_Mod(z[i * 2 + 6], dz3_r, shift);
		shift_round_Mod(z[i * 2 + 7], dz3_i, shift);
	}

	for (; i < n; i++) {
		mlib_d64 dz_r = 0, dz_i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < m; j++) {
			mlib_s32 a = (j * ns + i) * 2;

			dz_r += ((mlib_d64)x[j * 2]) * ((mlib_d64)y[a]) -
				((mlib_d64)x[j * 2 + 1]) * ((mlib_d64)y[a + 1]);
			dz_i += ((mlib_d64)x[j * 2]) * ((mlib_d64)y[a + 1]) +
				((mlib_d64)x[j * 2 + 1]) * ((mlib_d64)y[a]);
		}

		shift_round_Mod(z[i * 2], dz_r, shift);
		shift_round_Mod(z[i * 2 + 1], dz_i, shift);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulMShift_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 shift)
{
	return mlib_VectorMulMShift_S_S16_S16_Sat(z,
		x, y, m, n, n, shift);
}

mlib_status
__mlib_VectorMulMShift_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 shift)
{
	return mlib_VectorMulMShift_S_S16_S16_Mod(z,
		x, y, m, n, n, shift);
}

mlib_status
__mlib_VectorMulMShift_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 shift)
{
	return mlib_VectorMulMShift_S_S16C_S16C_Sat(z,
		x, y, m, n, n, shift);
}

mlib_status
__mlib_VectorMulMShift_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 shift)
{
	return mlib_VectorMulMShift_S_S16C_S16C_Mod(z,
		x, y, m, n, n, shift);
}

/* *********************************************************** */
