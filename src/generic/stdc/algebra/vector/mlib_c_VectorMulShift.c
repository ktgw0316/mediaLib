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

#pragma ident	"@(#)mlib_c_VectorMulShift.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorMulShift - multiplication of vectors with shifting
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMulShift_U8_U8_Mod(mlib_u8       *z,
 *                                                const mlib_u8 *x,
 *                                                const mlib_u8 *y,
 *                                                mlib_s32      n,
 *                                                mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_U8_U8_Sat(mlib_u8       *z,
 *                                                const mlib_u8 *x,
 *                                                const mlib_u8 *y,
 *                                                mlib_s32      n,
 *                                                mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_U8C_U8C_Mod(mlib_u8       *z,
 *                                                  const mlib_u8 *x,
 *                                                  const mlib_u8 *y,
 *                                                  mlib_s32      n,
 *                                                  mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_U8C_U8C_Sat(mlib_u8       *z,
 *                                                  const mlib_u8 *x,
 *                                                  const mlib_u8 *y,
 *                                                  mlib_s32      n,
 *                                                  mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_S8_S8_Mod(mlib_s8       *z,
 *                                                const mlib_s8 *x,
 *                                                const mlib_s8 *y,
 *                                                mlib_s32      n,
 *                                                mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_S8_S8_Sat(mlib_s8       *z,
 *                                                const mlib_s8 *x,
 *                                                const mlib_s8 *y,
 *                                                mlib_s32      n,
 *                                                mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_S8C_S8C_Mod(mlib_s8       *z,
 *                                                  const mlib_s8 *x,
 *                                                  const mlib_s8 *y,
 *                                                  mlib_s32      n,
 *                                                  mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_S8C_S8C_Sat(mlib_s8       *z,
 *                                                  const mlib_s8 *x,
 *                                                  const mlib_s8 *y,
 *                                                  mlib_s32      n,
 *                                                  mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_S16_S16_Mod(mlib_s16       *z,
 *                                                  const mlib_s16 *x,
 *                                                  const mlib_s16 *y,
 *                                                  mlib_s32       n,
 *                                                  mlib_s32       shift);
 *      mlib_status mlib_VectorMulShift_S16_S16_Sat(mlib_s16       *z,
 *                                                  const mlib_s16 *x,
 *                                                  const mlib_s16 *y,
 *                                                  mlib_s32       n,
 *                                                  mlib_s32       shift);
 *      mlib_status mlib_VectorMulShift_S16C_S16C_Sat(mlib_s16       *z,
 *                                                    const mlib_s16 *x,
 *                                                    const mlib_s16 *y,
 *                                                    mlib_s32       n,
 *                                                    mlib_s32       shift);
 *      mlib_status mlib_VectorMulShift_S16C_S16C_Mod(mlib_s16       *z,
 *                                                    const mlib_s16 *x,
 *                                                    const mlib_s16 *y,
 *                                                    mlib_s32       n,
 *                                                    mlib_s32       shift);
 *
 * ARGUMENTS
 *      z      the pointer to the first element of the result vector
 *      xz     the pointer to the first element of the first source and
 *             result vector
 *      x      the pointer to the first element of the first source vector
 *      y      the pointer to the first element of the second source vector
 *      n      the number of elements in each vector
 *      shift  the right shifting factor
 *
 * DESCRIPTION
 *      For U8, S8, S16, S32 types
 *
 *           z[i] = x[i] * y[i] * 2^(-shift); 0 <= i < n
 *
 *      For U8C, S8C, S16C, S32C types
 *
 *           z[2*i + 0] = (x[2*i + 0] * y[2*i + 0] - x[2*i + 1] * y[2*i + 1]) *
 *                        2^(-shift); 0 <= i < n
 *           z[2*i + 1] = (x[2*i + 1] * y[2*i + 0] + x[2*i + 0] * y[2*i + 1]) *
 *                        2^(-shift); 0 <= i < n
 *
 *      For _Mod mode, modulo operation is performed.  For _Sat mode,
 *      saturated operation is performed.
 *
 * RESTRICTIONS
 *      ranges of valid shift are:
 *           1 <= shift <= 8   for U8, S8, U8C, S8C types
 *           1 <= shift <= 16  for S16, S16C types
 *           1 <= shift <= 31  for S32, S32C types
 */

#include <mlib_algebra.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorMulShift_S16C_S16C_Mod = \
	__mlib_VectorMulShift_S16C_S16C_Mod
#pragma weak mlib_VectorMulShift_S16C_S16C_Sat = \
	__mlib_VectorMulShift_S16C_S16C_Sat
#pragma weak mlib_VectorMulShift_S16_S16_Mod = \
	__mlib_VectorMulShift_S16_S16_Mod
#pragma weak mlib_VectorMulShift_S16_S16_Sat = \
	__mlib_VectorMulShift_S16_S16_Sat
#pragma weak mlib_VectorMulShift_S8C_S8C_Mod = \
	__mlib_VectorMulShift_S8C_S8C_Mod
#pragma weak mlib_VectorMulShift_S8C_S8C_Sat = \
	__mlib_VectorMulShift_S8C_S8C_Sat
#pragma weak mlib_VectorMulShift_S8_S8_Mod = __mlib_VectorMulShift_S8_S8_Mod
#pragma weak mlib_VectorMulShift_S8_S8_Sat = __mlib_VectorMulShift_S8_S8_Sat
#pragma weak mlib_VectorMulShift_U8C_U8C_Mod = \
	__mlib_VectorMulShift_U8C_U8C_Mod
#pragma weak mlib_VectorMulShift_U8C_U8C_Sat = \
	__mlib_VectorMulShift_U8C_U8C_Sat
#pragma weak mlib_VectorMulShift_U8_U8_Mod = __mlib_VectorMulShift_U8_U8_Mod
#pragma weak mlib_VectorMulShift_U8_U8_Sat = __mlib_VectorMulShift_U8_U8_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VectorMulShift_S16C_S16C_Mod)
	mlib_VectorMulShift_S16C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_S16C_S16C_Mod")));
__typeof__(__mlib_VectorMulShift_S16C_S16C_Sat)
	mlib_VectorMulShift_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_S16C_S16C_Sat")));
__typeof__(__mlib_VectorMulShift_S16_S16_Mod) mlib_VectorMulShift_S16_S16_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_S16_S16_Mod")));
__typeof__(__mlib_VectorMulShift_S16_S16_Sat) mlib_VectorMulShift_S16_S16_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_S16_S16_Sat")));
__typeof__(__mlib_VectorMulShift_S8C_S8C_Mod) mlib_VectorMulShift_S8C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_S8C_S8C_Mod")));
__typeof__(__mlib_VectorMulShift_S8C_S8C_Sat) mlib_VectorMulShift_S8C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_S8C_S8C_Sat")));
__typeof__(__mlib_VectorMulShift_S8_S8_Mod) mlib_VectorMulShift_S8_S8_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_S8_S8_Mod")));
__typeof__(__mlib_VectorMulShift_S8_S8_Sat) mlib_VectorMulShift_S8_S8_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_S8_S8_Sat")));
__typeof__(__mlib_VectorMulShift_U8C_U8C_Mod) mlib_VectorMulShift_U8C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_U8C_U8C_Mod")));
__typeof__(__mlib_VectorMulShift_U8C_U8C_Sat) mlib_VectorMulShift_U8C_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_U8C_U8C_Sat")));
__typeof__(__mlib_VectorMulShift_U8_U8_Mod) mlib_VectorMulShift_U8_U8_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_U8_U8_Mod")));
__typeof__(__mlib_VectorMulShift_U8_U8_Sat) mlib_VectorMulShift_U8_U8_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_U8_U8_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(X)	((mlib_s32)(X))

#else /* MLIB_USE_FTOI_CLAMPING */

#define	FLOAT2INT_CLAMP(X)                                       \
	(((X) >= MLIB_S32_MAX) ? MLIB_S32_MAX : (((X) <          \
		MLIB_S32_MIN) ? MLIB_S32_MIN : (mlib_s32)(X)))

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;

#ifdef i386
	mlib_s32 i;
	mlib_s32 mul;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	for (i = 0; i < n; i++) {
		mul = (px[i] * (mlib_s32)py[i]) >> shift;

		if (mul > MLIB_U8_MAX)
			mul = MLIB_U8_MAX;
		else if (mul < MLIB_U8_MIN)
			mul = MLIB_U8_MIN;
		pz[i] = mul;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#else /* i386 */

	mlib_d64 sh = (mlib_d64)(1 << (24 - shift));
	mlib_d64 mul, mul0, mul1, mul2, mul3;
	mlib_s32 i, m;
	mlib_u32 *dpz;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	for (m = n; (m > 0) && (mlib_addr)z & 3; m--) {
		mul = px[0] * (mlib_d64)py[0] * sh;
		pz[0] = (FLOAT2INT_CLAMP(mul + MLIB_S32_MIN)
			-MLIB_S32_MIN) >> 24;
		pz++;
		px++;
		py++;
	}

	dpz = (mlib_u32 *)pz;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < m - 3; i += 4) {
		mul0 = px[i + 0] * (mlib_d64)py[i + 0] * sh;
		mul1 = px[i + 1] * (mlib_d64)py[i + 1] * sh;
		mul2 = px[i + 2] * (mlib_d64)py[i + 2] * sh;
		mul3 = px[i + 3] * (mlib_d64)py[i + 3] * sh;
		(*dpz++) = ((((mlib_u32)(FLOAT2INT_CLAMP(mul0 + MLIB_S32_MIN)
			-MLIB_S32_MIN)) & 0xFF000000) |
			(((mlib_u32)(FLOAT2INT_CLAMP(mul1 + MLIB_S32_MIN)
			-MLIB_S32_MIN) >> 8) & 0xFF0000) |
			(((mlib_u32)(FLOAT2INT_CLAMP(mul2 + MLIB_S32_MIN)
			-MLIB_S32_MIN) >> 16) & 0xFF00) |
			((mlib_u32)(FLOAT2INT_CLAMP(mul3 + MLIB_S32_MIN)
			-MLIB_S32_MIN) >> 24));
	}

	pz = (mlib_u8 *)dpz;
	while ((m > 0) && (i < m)) {
		mul = px[i] * (mlib_d64)py[i] * sh;
		(*pz++) = (FLOAT2INT_CLAMP(mul + MLIB_S32_MIN)
			-MLIB_S32_MIN) >> 24;
		i++;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* i386 */
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;

#ifdef i386

	mlib_s32 i;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	for (i = 0; i < n; i++)
		pz[i] = (px[i] * py[i]) >> shift;
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#else /* i386 */

	mlib_d64 mul, mul0, mul1, mul2, mul3;
	mlib_s32 i, m;
	mlib_s32 *dpz;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	for (m = n; (m > 0) && (mlib_addr)z & 3; m--) {
		mul = px[0] * (mlib_d64)py[0];
		pz[0] = (mlib_s32)mul >> shift;
		pz++;
		px++;
		py++;
	}

	dpz = (mlib_s32 *)pz;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < m - 3; i += 4) {
		mul0 = px[i + 0] * (mlib_d64)py[i + 0];
		mul1 = px[i + 1] * (mlib_d64)py[i + 1];
		mul2 = px[i + 2] * (mlib_d64)py[i + 2];
		mul3 = px[i + 3] * (mlib_d64)py[i + 3];
		(*dpz++) =
			(((mlib_s32)mul0 << (24 - shift)) & 0xFF000000) |
			(((mlib_s32)mul1 << (16 - shift)) & 0xFF0000) |
			(((mlib_s32)mul2 << (8 - shift)) & 0xFF00) |
			(((mlib_s32)mul3 >> shift) & 0xFF);
	}

	pz = (mlib_u8 *)dpz;
	while (i < m) {
		mul = px[i] * (mlib_d64)py[i];
		(*pz++) = (mlib_s32)mul >> shift;
		i++;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* i386 */
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_U8C_U8C_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;

#ifdef i386
	mlib_s32 i;
	mlib_s32 mul_r, mul_i, x_r, x_i, y_r, y_i;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	for (i = 0; i < n; i++) {
		x_r = px[2 * i];
		x_i = px[2 * i + 1];
		y_r = py[2 * i];
		y_i = py[2 * i + 1];
		mul_r = (x_r * y_r - x_i * y_i) >> shift;
		mul_i = (x_i * y_r + x_r * y_i) >> shift;
		pz[2 * i] = (mul_r > MLIB_U8_MAX) ? MLIB_U8_MAX :
			(mul_r < MLIB_U8_MIN ? MLIB_U8_MIN : mul_r);
		pz[2 * i + 1] = (mul_i > MLIB_U8_MAX) ? MLIB_U8_MAX :
			(mul_i < MLIB_U8_MIN ? MLIB_U8_MIN : mul_i);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#else /* i386 */

	mlib_s32 i;
	mlib_d64 mul_r, mul_i;
	mlib_u8 x_r, x_i, y_r, y_i;
	mlib_d64 sh = (mlib_d64)(1 << (24 - shift));

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		x_r = px[2 * i];
		x_i = px[2 * i + 1];
		y_r = py[2 * i];
		y_i = py[2 * i + 1];
		mul_r = (x_r * (mlib_d64)y_r - x_i * (mlib_d64)y_i) * sh;
		mul_i = (x_i * (mlib_d64)y_r + x_r * (mlib_d64)y_i) * sh;
		pz[2 * i] = (FLOAT2INT_CLAMP(mul_r + MLIB_S32_MIN)
			-MLIB_S32_MIN) >> 24;
		pz[2 * i + 1] = (FLOAT2INT_CLAMP(mul_i + MLIB_S32_MIN)
			-MLIB_S32_MIN) >> 24;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* i386 */
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;

#ifdef i386

	mlib_s32 i;
	mlib_s32 mul_r, mul_i, x_r, x_i, y_r, y_i;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	for (i = 0; i < n; i++) {
		x_r = px[2 * i];
		x_i = px[2 * i + 1];
		y_r = py[2 * i];
		y_i = py[2 * i + 1];
		mul_r = (x_r * y_r - x_i * y_i) >> shift;
		mul_i = (x_i * y_r + x_r * y_i) >> shift;
		pz[2 * i] = mul_r;
		pz[2 * i + 1] = mul_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#else /* i386 */

	mlib_u8 x_r, x_i, y_r, y_i;
	mlib_d64 mul_r, mul_i;
	mlib_s32 i;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		x_r = px[2 * i];
		x_i = px[2 * i + 1];
		y_r = py[2 * i];
		y_i = py[2 * i + 1];
		mul_r = (x_r * (mlib_d64)y_r - x_i * (mlib_d64)y_i);
		mul_i = (x_i * (mlib_d64)y_r + x_r * (mlib_d64)y_i);
		pz[2 * i] = ((mlib_s32)(mul_r - 1) + 1) >> shift;
		pz[2 * i + 1] = ((mlib_s32)(mul_i - 1) + 1) >> shift;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* i386 */
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y, *pz = (mlib_s8 *)z;

#ifdef i386
	mlib_s32 i;
	mlib_s32 mul;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	for (i = 0; i < n; i++) {
		mul = (px[i] * (mlib_s32)py[i]) >> shift;

		if (mul > MLIB_S8_MAX)
			mul = MLIB_S8_MAX;
		else if (mul < MLIB_S8_MIN)
			mul = MLIB_S8_MIN;
		pz[i] = mul;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#else /* i386 */

	mlib_d64 sh = (mlib_d64)(1 << (24 - shift));
	mlib_d64 mul, mul0, mul1, mul2, mul3;
	mlib_s32 i, m;
	mlib_u32 *dpz;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	for (m = n; (m > 0) && (mlib_addr)z & 3; m--) {
		mul = px[0] * (mlib_d64)py[0] * sh;
		pz[0] = FLOAT2INT_CLAMP(mul) >> 24;
		pz++;
		px++;
		py++;
	}

	dpz = (mlib_u32 *)pz;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < m - 3; i += 4) {
		mul0 = px[i + 0] * (mlib_d64)py[i + 0] * sh;
		mul1 = px[i + 1] * (mlib_d64)py[i + 1] * sh;
		mul2 = px[i + 2] * (mlib_d64)py[i + 2] * sh;
		mul3 = px[i + 3] * (mlib_d64)py[i + 3] * sh;
		(*dpz++) = ((((mlib_u32)(FLOAT2INT_CLAMP(mul0))) & 0xFF000000) |
			(((mlib_u32)(FLOAT2INT_CLAMP(mul1)) >> 8) & 0xFF0000) |
			(((mlib_u32)(FLOAT2INT_CLAMP(mul2)) >> 16) & 0xFF00) |
			((mlib_u32)(FLOAT2INT_CLAMP(mul3)) >> 24));
	}

	pz = (mlib_s8 *)dpz;
	while ((m > 0) && (i < m)) {
		mul = px[i] * (mlib_d64)py[i] * sh;
		(*pz++) = FLOAT2INT_CLAMP(mul) >> 24;
		i++;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* i386 */
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S8_S8_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y, *pz = (mlib_s8 *)z;

#ifdef i386

	mlib_s32 i;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	for (i = 0; i < n; i++)
		pz[i] = (px[i] * py[i]) >> shift;
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#else /* i386 */

	mlib_d64 mul, mul0, mul1, mul2, mul3;
	mlib_s32 i, m;
	mlib_s32 *dpz;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	for (m = n; (m > 0) && (mlib_addr)z & 3; m--) {
		mul = px[0] * (mlib_d64)py[0];
		pz[0] = (mlib_s32)mul >> shift;
		pz++;
		px++;
		py++;
	}

	dpz = (mlib_s32 *)pz;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < m - 3; i += 4) {
		mul0 = px[i + 0] * (mlib_d64)py[i + 0];
		mul1 = px[i + 1] * (mlib_d64)py[i + 1];
		mul2 = px[i + 2] * (mlib_d64)py[i + 2];
		mul3 = px[i + 3] * (mlib_d64)py[i + 3];
		(*dpz++) =
			(((mlib_s32)mul0 << (24 - shift)) & 0xFF000000) |
			(((mlib_s32)mul1 << (16 - shift)) & 0xFF0000) |
			(((mlib_s32)mul2 << (8 - shift)) & 0xFF00) |
			(((mlib_s32)mul3 >> shift) & 0xFF);
	}

	pz = (mlib_s8 *)dpz;
	while (i < m) {
		mul = px[i] * (mlib_d64)py[i];
		(*pz++) = (mlib_s32)mul >> shift;
		i++;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* i386 */
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y, *pz = (mlib_s8 *)z;

#ifdef i386
	mlib_s32 i;
	mlib_s32 mul_r, mul_i, x_r, x_i, y_r, y_i;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	for (i = 0; i < n; i++) {
		x_r = px[2 * i];
		x_i = px[2 * i + 1];
		y_r = py[2 * i];
		y_i = py[2 * i + 1];
		mul_r = (x_r * y_r - x_i * y_i) >> shift;
		mul_i = (x_i * y_r + x_r * y_i) >> shift;
		pz[2 * i] = (mul_r > MLIB_S8_MAX) ? MLIB_S8_MAX :
			(mul_r < MLIB_S8_MIN ? MLIB_S8_MIN : mul_r);
		pz[2 * i + 1] = (mul_i > MLIB_S8_MAX) ? MLIB_S8_MAX :
			(mul_i < MLIB_S8_MIN ? MLIB_S8_MIN : mul_i);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#else /* i386 */

	mlib_s32 i;
	mlib_d64 mul_r, mul_i;
	mlib_s8 x_r, x_i, y_r, y_i;
	mlib_d64 sh = (mlib_d64)(1 << (24 - shift));

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		x_r = px[2 * i];
		x_i = px[2 * i + 1];
		y_r = py[2 * i];
		y_i = py[2 * i + 1];
		mul_r = (x_r * (mlib_d64)y_r - x_i * (mlib_d64)y_i) * sh;
		mul_i = (x_i * (mlib_d64)y_r + x_r * (mlib_d64)y_i) * sh;
		pz[2 * i] = FLOAT2INT_CLAMP(mul_r)
		>>24;
		pz[2 * i + 1] = FLOAT2INT_CLAMP(mul_i)
		>>24;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* i386 */
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S8C_S8C_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y, *pz = (mlib_s8 *)z;

#ifdef i386

	mlib_s32 i;
	mlib_s32 mul_r, mul_i, x_r, x_i, y_r, y_i;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	for (i = 0; i < n; i++) {
		x_r = px[2 * i];
		x_i = px[2 * i + 1];
		y_r = py[2 * i];
		y_i = py[2 * i + 1];
		mul_r = (x_r * y_r - x_i * y_i) >> shift;
		mul_i = (x_i * y_r + x_r * y_i) >> shift;
		pz[2 * i] = mul_r;
		pz[2 * i + 1] = mul_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#else /* i386 */

	mlib_s8 x_r, x_i, y_r, y_i;
	mlib_d64 mul_r, mul_i;
	mlib_s32 i;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		x_r = px[2 * i];
		x_i = px[2 * i + 1];
		y_r = py[2 * i];
		y_i = py[2 * i + 1];
		mul_r = (x_r * (mlib_d64)y_r - x_i * (mlib_d64)y_i);
		mul_i = (x_i * (mlib_d64)y_r + x_r * (mlib_d64)y_i);
		pz[2 * i] = ((mlib_s32)(mul_r - 1) + 1) >> shift;
		pz[2 * i + 1] = ((mlib_s32)(mul_i - 1) + 1) >> shift;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* i386 */
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;

#ifdef i386
	mlib_s32 i;
	mlib_s32 mul;

	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);

	for (i = 0; i < n; i++) {
		mul = (px[i] * (mlib_s32)py[i]) >> shift;

		if (mul > MLIB_S16_MAX)
			mul = MLIB_S16_MAX;
		else if (mul < MLIB_S16_MIN)
			mul = MLIB_S16_MIN;
		pz[i] = mul;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#else /* i386 */

	mlib_d64 sh = (mlib_d64)(1 << (16 - shift));
	mlib_d64 mul, mul0, mul1;
	mlib_s32 i, m = n;

	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);

	if ((m > 0) && (mlib_addr)z & 2) {
		mul = (mlib_d64)px[0] * (mlib_d64)py[0] * sh;
		pz[0] = FLOAT2INT_CLAMP(mul)
		>>16;
		pz++;
		px++;
		py++;
		m--;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (m >> 1); i++) {
		mul0 = (mlib_d64)px[2 * i + 0] * (mlib_d64)py[2 * i + 0] * sh;
		mul1 = (mlib_d64)px[2 * i + 1] * (mlib_d64)py[2 * i + 1] * sh;
		((mlib_s32 *)pz)[i] = FLOAT2INT_CLAMP(mul0);
		pz[2 * i + 1] = FLOAT2INT_CLAMP(mul1)
		>>16;
	}

	if ((m > 0) && (m & 1)) {
		mul = (mlib_d64)px[m - 1] * (mlib_d64)py[m - 1] * sh;
		pz[m - 1] = FLOAT2INT_CLAMP(mul)
		>>16;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* i386 */
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;

#ifdef i386

	mlib_s32 i;

	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);

	for (i = 0; i < n; i++)
		pz[i] = (px[i] * py[i]) >> shift;
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#else /* i386 */

	mlib_d64 mul, mul0, mul1;
	mlib_s32 i, m = n;

	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);

	if ((m > 0) && (mlib_addr)z & 2) {
		mul = (mlib_d64)px[0] * (mlib_d64)py[0];
		pz[0] = (mlib_s32)mul >> shift;
		pz++;
		px++;
		py++;
		m--;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (m >> 1); i++) {
		mul0 = px[2 * i + 0] * (mlib_d64)py[2 * i + 0];
		mul1 = px[2 * i + 1] * (mlib_d64)py[2 * i + 1];

		pz[2 * i + 0] = (mlib_s32)mul0 >> shift;
		pz[2 * i + 1] = (mlib_s32)mul1 >> shift;
	}

	if ((m > 0) && (m & 1)) {
		mul = (mlib_d64)px[m - 1] * (mlib_d64)py[m - 1];
		pz[m - 1] = (mlib_s32)mul >> shift;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* i386 */
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;

#ifdef i386
	mlib_s32 i;
	mlib_s32 mul_r, mul_i, x_r, x_i, y_r, y_i;

	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);

	for (i = 0; i < n; i++) {
		x_r = px[2 * i];
		x_i = px[2 * i + 1];
		y_r = py[2 * i];
		y_i = py[2 * i + 1];
		mul_r = x_r * y_r - x_i * y_i;

		if (mul_r == MLIB_S32_MIN)
			mul_r = ((mlib_u32)MLIB_S32_MIN) >> shift;
		else
			mul_r = mul_r >> shift;
		mul_i = (x_i * y_r + x_r * y_i) >> shift;
		pz[2 * i] =
			(mul_r > MLIB_S16_MAX) ? MLIB_S16_MAX : (mul_r <
			MLIB_S16_MIN ? MLIB_S16_MIN : mul_r);
		pz[2 * i + 1] =
			(mul_i > MLIB_S16_MAX) ? MLIB_S16_MAX : (mul_i <
			MLIB_S16_MIN ? MLIB_S16_MIN : mul_i);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#else /* i386 */

	mlib_s32 i;
	mlib_d64 mul_r, mul_i;
	mlib_s16 x_r, x_i, y_r, y_i;
	mlib_d64 sh = (mlib_d64)(1 << (16 - shift));

	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		x_r = px[2 * i];
		x_i = px[2 * i + 1];
		y_r = py[2 * i];
		y_i = py[2 * i + 1];
		mul_r = (x_r * (mlib_d64)y_r - x_i * (mlib_d64)y_i) * sh;
		mul_i = (x_i * (mlib_d64)y_r + x_r * (mlib_d64)y_i) * sh;
		pz[2 * i] = FLOAT2INT_CLAMP(mul_r)
		>>16;
		pz[2 * i + 1] = FLOAT2INT_CLAMP(mul_i)
		>>16;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* i386 */
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;

#ifdef i386

	mlib_s32 i;
	mlib_s32 mul_r, mul_i, x_r, x_i, y_r, y_i;

	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);

	for (i = 0; i < n; i++) {
		x_r = px[2 * i];
		x_i = px[2 * i + 1];
		y_r = py[2 * i];
		y_i = py[2 * i + 1];
		mul_r = (x_r * y_r - x_i * y_i) >> shift;
		mul_i = (x_i * y_r + x_r * y_i) >> shift;
		pz[2 * i] = mul_r;
		pz[2 * i + 1] = mul_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#else /* i386 */

	mlib_s16 x_r, x_i, y_r, y_i;
	mlib_d64 mul_r, mul_i;
	mlib_s32 i;

	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		x_r = px[2 * i];
		x_i = px[2 * i + 1];
		y_r = py[2 * i];
		y_i = py[2 * i + 1];
		mul_r = (x_r * (mlib_d64)y_r - x_i * (mlib_d64)y_i);
		mul_i = (x_i * (mlib_d64)y_r + x_r * (mlib_d64)y_i);
		pz[2 * i] = ((mlib_s32)(mul_r - 1) + 1) >> shift;
		pz[2 * i + 1] = ((mlib_s32)(mul_i - 1) + 1) >> shift;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* i386 */
}

/* *********************************************************** */
