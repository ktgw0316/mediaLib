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

#pragma ident	"@(#)mlib_i_VectorMulShift.c	9.2	07/11/05 SMI"

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
	mlib_s32 mul0, mul1;
	mlib_s32 i, m = n;
	mlib_s32 mul;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	if ((mlib_addr)z & 2) {
		mul0 = ((mlib_s32)px[0] * py[0]) >> shift;
		pz[0] = ((MLIB_U8_MAX - mul0) >> 31) | mul0;
		pz++;
		px++;
		py++;
		m--;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < m - 1; i += 2) {
		mul0 = ((mlib_s32)px[i] * py[i]) >> shift;
		mul1 = ((mlib_s32)px[i + 1] * py[i + 1]) >> shift;
		pz[i] = ((MLIB_U8_MAX - mul0) >> 31) | mul0;
		pz[i + 1] = ((MLIB_U8_MAX - mul1) >> 31) | mul1;
	}

	if (m & 1) {
		mul0 = ((mlib_s32)px[m - 1] * py[m - 1]) >> shift;
		pz[m - 1] = ((MLIB_U8_MAX - mul0) >> 31) | mul0;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
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

	mlib_s32 i, m = n;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	if ((mlib_addr)z & 2) {
		pz[0] = ((mlib_s32)px[0] * py[0]) >> shift;
		pz++;
		px++;
		py++;
		m--;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < m - 1; i += 2) {
		pz[i] = ((mlib_s32)px[i] * py[i]) >> shift;
		pz[i + 1] = ((mlib_s32)px[i + 1] * py[i + 1]) >> shift;
	}

	if (m & 1) {
		pz[m - 1] = ((mlib_s32)px[m - 1] * py[m - 1]) >> shift;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
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
	mlib_s32 n2 = (n >> 1) << 2, i_0, i_1, i_2, i_3, z_im, z_im_1;
	mlib_s32 x_re, x_im, x_re_1, x_im_1;
	mlib_s32 c_re, c_im;
	mlib_s32 z_re, z_re_1;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1, i_2 = 2, i_3 = 3; i_0 < n2;
		i_0 += 4, i_1 += 4, i_2 += 4, i_3 += 4) {
		x_re = x[i_0];
		x_im = x[i_1];
		c_re = y[i_0];
		c_im = y[i_1];

		z_re = (x_re * c_re - x_im * c_im) >> shift;
		z_im = (x_re * c_im + x_im * c_re) >> shift;
		z[i_0] = (((MLIB_U8_MAX - z_re) >> 31) | z_re) &
					(~(z_re >> 31));
		z[i_1] = ((MLIB_U8_MAX - z_im) >> 31) | z_im;

		x_im_1 = x[i_3];
		x_re_1 = x[i_2];
		c_re = y[i_2];
		c_im = y[i_3];

		z_re_1 = (x_re_1 * c_re - x_im_1 * c_im) >> shift;
		z_im_1 = (x_re_1 * c_im + x_im_1 * c_re) >> shift;
		z[i_2] = (((MLIB_U8_MAX - z_re_1) >> 31) | z_re_1)
				& (~(z_re_1 >> 31));
		z[i_3] = ((MLIB_U8_MAX - z_im_1) >> 31) | z_im_1;
	}

	if ((n << 1) != n2) {
		mlib_s32 index1 = (n - 1) << 1;
		mlib_s32 index2 = index1 + 1;
		x_re = x[index1];
		x_im = x[index2];
		c_re = y[index1];
		c_im = y[index2];

		z_re = (x_re * c_re - x_im * c_im) >> shift;
		z_im = (x_re * c_im + x_im * c_re) >> shift;
		z[index1] = (((MLIB_U8_MAX - z_re) >> 31) | z_re)
					& (~(z_re >> 31));
		z[index2] = ((MLIB_U8_MAX - z_im) >> 31) | z_im;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
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
	mlib_s32 n2 = (n >> 1) << 2, i_0, i_1, i_2, i_3;
	mlib_s32 x_re, x_im, x_re_1, x_im_1;
	mlib_s32 c_re, c_im;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1, i_2 = 2, i_3 = 3; i_0 < n2;
		i_0 += 4, i_1 += 4, i_2 += 4, i_3 += 4) {

		x_re = x[i_0];
		x_im = x[i_1];
		x_im_1 = x[i_3];
		x_re_1 = x[i_2];
		c_re = y[i_0];
		c_im = y[i_1];
		z[i_0] = (x_re * c_re - x_im * c_im) >> shift;
		z[i_1] = (x_re * c_im + x_im * c_re) >> shift;
		c_re = y[i_2];
		c_im = y[i_3];
		z[i_2] = (x_re_1 * c_re - x_im_1 * c_im) >> shift;
		z[i_3] = (x_re_1 * c_im + x_im_1 * c_re) >> shift;
	}

	if ((n << 1) != n2) {
		mlib_s32 index1 = (n - 1) << 1;
		mlib_s32 index2 = index1 + 1;
		mlib_s32 im, re;
		c_re = y[index1];
		c_im = y[index2];
		re = (x[index1] * c_re - x[index2] * c_im) >> shift;
		im = (x[index1] * c_im + x[index2] * c_re) >> shift;
		z[index1] = re;
		z[index2] = im;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
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
	mlib_s32 i;
	mlib_s32 mul;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mul = ((mlib_s32)x[i] * y[i]) >> shift;
		if (mul > MLIB_S8_MAX)
			z[i] = MLIB_S8_MAX;
		else if (mul < MLIB_S8_MIN)
			z[i] = MLIB_S8_MIN;
		else
			z[i] = mul;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
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

	mlib_s32 i, m = n;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	if ((mlib_addr)z & 2) {
		pz[0] = ((mlib_s32)px[0] * py[0]) >> shift;
		pz++;
		px++;
		py++;
		m--;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < m - 1; i += 2) {
		pz[i] = ((mlib_s32)px[i] * py[i]) >> shift;
		pz[i + 1] = ((mlib_s32)px[i + 1] * py[i + 1]) >> shift;
	}

	if (m & 1) {
		pz[m - 1] = ((mlib_s32)px[m - 1] * py[m - 1]) >> shift;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
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

	mlib_s32 i_0, i_1, i_2, i_3, n2 = (n >> 1) << 2;
	mlib_s32 z_re, z_im, z_re_1, z_im_1, x_re, x_im, c_re, c_im;
	mlib_s32 x_re_1, x_im_1;
	mlib_s32 index1, index2;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1, i_2 = 2, i_3 = 3; i_0 < n2;
		i_0 += 4, i_1 += 4, i_2 += 4, i_3 += 4) {
		x_re = x[i_0];
		x_im = x[i_1];
		c_re = y[i_0];
		c_im = y[i_1];

		z_re = (x_re * c_re - x_im * c_im) >> shift;
		z_im = (x_re * c_im + x_im * c_re) >> shift;

		if (z_re > MLIB_S8_MAX)
			pz[i_0] = MLIB_S8_MAX;
		else if (z_re < MLIB_S8_MIN)
			pz[i_0] = MLIB_S8_MIN;
		else
			pz[i_0] = z_re;

		if (z_im > MLIB_S8_MAX)
			pz[i_1] = MLIB_S8_MAX;
		else if (z_im < MLIB_S8_MIN)
			pz[i_1] = MLIB_S8_MIN;
		else
			pz[i_1] = z_im;
		x_im_1 = x[i_3];
		x_re_1 = x[i_2];
		c_re = y[i_2];
		c_im = y[i_3];

		z_re_1 = (x_re_1 * c_re - x_im_1 * c_im) >> shift;
		z_im_1 = (x_re_1 * c_im + x_im_1 * c_re) >> shift;
		if (z_re_1 > MLIB_S8_MAX)
			pz[i_2] = MLIB_S8_MAX;
		else if (z_re_1 < MLIB_S8_MIN)
			pz[i_2] = MLIB_S8_MIN;
		else
			pz[i_2] = z_re_1;
		if (z_im_1 > MLIB_S8_MAX)
			pz[i_3] = MLIB_S8_MAX;
		else if (z_im_1 < MLIB_S8_MIN)
			pz[i_3] = MLIB_S8_MIN;
		else
			pz[i_3] = z_im_1;
	}

	if ((n << 1) != n2) {
		mlib_s32 index1 = (n - 1) << 1;
		mlib_s32 index2 = index1 + 1;
		x_re = x[index1];
		x_im = x[index2];
		c_re = y[index1];
		c_im = y[index2];

		z_re = (x_re * c_re - x_im * c_im) >> shift;
		z_im = (x_re * c_im + x_im * c_re) >> shift;
		if (z_re > MLIB_S8_MAX)
			pz[index1] = MLIB_S8_MAX;
		else if (z_re < MLIB_S8_MIN)
			pz[index1] = MLIB_S8_MIN;
		else
			pz[index1] = z_re;
		if (z_im > MLIB_S8_MAX)
			pz[index2] = MLIB_S8_MAX;
		else if (z_im < MLIB_S8_MIN)
			pz[index2] = MLIB_S8_MIN;
		else
			pz[index2] = z_im;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
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
	mlib_s32 n2 = (n >> 1) << 2, i_0, i_1, i_2, i_3;
	mlib_s32 x_re, x_im, x_re_1, x_im_1;
	mlib_s32 c_re, c_im;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1, i_2 = 2, i_3 = 3; i_0 < n2;
		i_0 += 4, i_1 += 4, i_2 += 4, i_3 += 4) {

		x_re = x[i_0];
		x_im = x[i_1];
		x_im_1 = x[i_3];
		x_re_1 = x[i_2];
		c_re = y[i_0];
		c_im = y[i_1];
		z[i_0] = (x_re * c_re - x_im * c_im) >> shift;
		z[i_1] = (x_re * c_im + x_im * c_re) >> shift;
		c_re = y[i_2];
		c_im = y[i_3];
		z[i_2] = (x_re_1 * c_re - x_im_1 * c_im) >> shift;
		z[i_3] = (x_re_1 * c_im + x_im_1 * c_re) >> shift;
	}

	if ((n << 1) != n2) {
		mlib_s32 index1 = (n - 1) << 1;
		mlib_s32 index2 = index1 + 1;
		mlib_s32 im, re;
		c_re = y[index1];
		c_im = y[index2];
		re = (x[index1] * c_re - x[index2] * c_im) >> shift;
		im = (x[index1] * c_im + x[index2] * c_re) >> shift;
		z[index1] = re;
		z[index2] = im;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
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
	mlib_s32 mul1;
	mlib_s32 i;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;

	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mul1 = ((mlib_s32)px[i] * py[i]) >> shift;
		if (mul1 > MLIB_S16_MAX)
			pz[i] = MLIB_S16_MAX;
		else if (mul1 < MLIB_S16_MIN)
			pz[i] = MLIB_S16_MIN;
		else
			pz[i] = mul1;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
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
	mlib_s32 i;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;

	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		pz[i] = ((mlib_s32)px[i] * py[i]) >> shift;
	}
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}
#ifdef _NO_LONGLONG
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
#else
mlib_status
__mlib_VectorMulShift_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;

	mlib_s64 mul_r, mul_i;
	mlib_s64 x_r, x_i, y_r, y_i;
	mlib_s32 i, index1, index2;

	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		index1 = i << 1;
		index2 = index1 + 1;
		x_r = px[index1];
		x_i = px[index2];
		y_r = py[index1];
		y_i = py[index2];
		mul_r = (x_r * y_r - x_i * y_i) >> shift;
		mul_i = (x_i * y_r + x_r * y_i) >> shift;

		if (mul_r > MLIB_S16_MAX)
			pz[index1] = MLIB_S16_MAX;
		else if (mul_r < MLIB_S16_MIN)
			pz[index1] = MLIB_S16_MIN;
		else
			pz[index1] = mul_r;

		if (mul_i > MLIB_S16_MAX)
			pz[index2] = MLIB_S16_MAX;
		else if (mul_i < MLIB_S16_MIN)
			pz[index2] = MLIB_S16_MIN;
		else
			pz[index2] = mul_i;
	}
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}
#endif
/* *********************************************************** */
#ifdef _NO_LONGLONG
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
#else
mlib_status
__mlib_VectorMulShift_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;
	mlib_s64 mul_r, mul_i, x_r, x_i, y_r, y_i;
	mlib_s32 i;
	mlib_s32 index1, index2;
	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		index1 = i << 1;
		index2 = index1 + 1;
		x_r = px[index1];
		x_i = px[index2];
		y_r = py[index1];
		y_i = py[index2];
		pz[index1] = (x_r * y_r - x_i * y_i) >> shift;
		pz[index2] = (x_i * y_r + x_r * y_i) >> shift;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}
#endif
/* *********************************************************** */
