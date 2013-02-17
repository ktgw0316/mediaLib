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

#pragma ident	"@(#)mlib_i_VectorSumAbs.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorSumAbs_U8 -  sum of the absolute values
 *                              of unsigned 8-bit format vector
 *      mlib_VectorSumAbs_S8 -  sum of the absolute values
 *                              of signed 8-bit format vector
 *      mlib_VectorSumAbs_S16 - sum of the absolute values
 *                              of signed 16-bit format vector
 *      mlib_VectorSumAbs_S32 - sum of the absolute values
 *                              of signed 32-bit format vector
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorSumAbs_S16_Sat(mlib_d64       * z,
 *                                            const mlib_s16 * x,
 *                                            mlib_s32       n);
 *      mlib_status mlib_VectorSumAbs_S32_Sat(mlib_d64       * z,
 *                                            const mlib_s32 * x,
 *                                            mlib_s32       n);
 *      mlib_status mlib_VectorSumAbs_S8_Sat(mlib_d64      * z,
 *                                           const mlib_s8 * x,
 *                                           mlib_s32      n);
 *      mlib_status mlib_VectorSumAbs_U8_Sat(mlib_d64      * z,
 *                                           const mlib_u8 * x,
 *                                           mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the sum of the absolute values of the vector
 *      x    pointer to the first element of the input vector
 *      n    number of elements in the vector
 *
 * DESCRIPTION
 *           n
 *      z = SUM |x[i]|
 *          i = 1
 */

#include <mlib_algebra.h>
#include <mlib_AlgebraUtil.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorSumAbs_S16_Sat = __mlib_VectorSumAbs_S16_Sat
#pragma weak mlib_VectorSumAbs_S8_Sat = __mlib_VectorSumAbs_S8_Sat
#pragma weak mlib_VectorSumAbs_U8_Sat = __mlib_VectorSumAbs_U8_Sat
#pragma weak mlib_VectorSumAbs_S32_Sat = __mlib_VectorSumAbs_S32_Sat

#elif defined(__GNUC__)

__typeof__(__mlib_VectorSumAbs_S16_Sat) mlib_VectorSumAbs_S16_Sat
	__attribute__((weak, alias("__mlib_VectorSumAbs_S16_Sat")));
__typeof__(__mlib_VectorSumAbs_S8_Sat) mlib_VectorSumAbs_S8_Sat
	__attribute__((weak, alias("__mlib_VectorSumAbs_S8_Sat")));
__typeof__(__mlib_VectorSumAbs_U8_Sat) mlib_VectorSumAbs_U8_Sat
	__attribute__((weak, alias("__mlib_VectorSumAbs_U8_Sat")));
__typeof__(__mlib_VectorSumAbs_S32_Sat) mlib_VectorSumAbs_S32_Sat
	__attribute__((weak, alias("__mlib_VectorSumAbs_S32_Sat")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VectorSumAbs_U8_Sat(
	mlib_d64 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_s32 sum = 0;
	mlib_u8 *px = (mlib_u8 *)x;

	if (n <= 0)
		return (MLIB_FAILURE);

	for (i = 0; i < n; i++)
		sum += px[i];
	z[0] = (mlib_d64)sum;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSumAbs_S8_Sat(
	mlib_d64 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_s32 sum = 0;
	mlib_s8 *px = (mlib_s8 *)x;
	mlib_s32 sr, src;

	if (n <= 0)
		return (MLIB_FAILURE);

	for (i = 0; i < n; i++) {
		src = (mlib_s32)px[i];
		sr = (src >> 31);
		sum += (src ^ sr) - sr;
	}

	z[0] = (mlib_d64)sum;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSumAbs_S16_Sat(
	mlib_d64 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_s32 sum = 0;
	mlib_s16 *px = (mlib_s16 *)x;
	mlib_s32 sr, src;

	if (n <= 0)
		return (MLIB_FAILURE);

	for (i = 0; i < n; i++) {
		src = (mlib_s32)px[i];
		sr = (src >> 31);
		sum += (src ^ sr) - sr;
	}

	z[0] = (mlib_d64)sum;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSumAbs_S32_Sat(
	mlib_d64 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	mlib_s32 i;

#if defined(_NO_LONGLONG)

	mlib_d64 sum0 = 0, src0;
	if (n <= 0)
		return (MLIB_FAILURE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		src0 = x[i + 0];
		sum0 += mlib_fabs(src0);
	}

	z[0] = sum0;

#else /* defined(_NO_LONGLONG) */

	mlib_s64 sum0 = 0, sr0, src0, res0;
	mlib_s64 sum1 = 0, sr1, src1, res1;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (i = n & 1) {
		src0 = x[0];
		sr0 = (src0 >> 63);
		res0 = (src0 ^ sr0) - sr0;
		sum0 += res0;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n - 1; i += 2) {

		src0 = x[i + 0];
		src1 = x[i + 1];

		sr0 = (src0 >> 63);
		res0 = (src0 ^ sr0) - sr0;

		sr1 = (src1 >> 63);
		res1 = (src1 ^ sr1) - sr1;

		sum0 += res0;
		sum1 += res1;
	}

	z[0] = (mlib_d64)(sum0 + sum1);

#endif /* defined(_NO_LONGLONG) */

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
