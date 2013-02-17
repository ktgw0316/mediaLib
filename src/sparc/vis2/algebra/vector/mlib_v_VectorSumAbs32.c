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

#pragma ident	"@(#)mlib_v_VectorSumAbs32.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorSumAbs_S32 - sum of the absolute values
 *                             of signed 32-bit format vector
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorSumAbs_S32_Sat(mlib_d64       * z,
 *                                            const mlib_s32 * x,
 *                                            mlib_s32       n);
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
#include <mlib_SysMath.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorSumAbs_S32_Sat = __mlib_VectorSumAbs_S32_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorSumAbs_S32_Sat) mlib_VectorSumAbs_S32_Sat
	__attribute__((weak, alias("__mlib_VectorSumAbs_S32_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VectorSumAbs_S32_Sat(
	mlib_d64 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 sum0 = 0, src0, res0;
	mlib_s64 sum1 = 0, sr1, src1, res1;
	mlib_d64 sum2 = 0, src2, res2;
	mlib_s64 sum3 = 0, sr3, src3, res3;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (i = n & 1) {
		src0 = x[0];
		sum0 += mlib_fabs(src0);
	}
#pragma pipeloop(0)
	for (; i < n - 3; i += 4) {

		src0 = x[i + 0];
		src1 = x[i + 1];
		src2 = x[i + 2];
		src3 = x[i + 3];

		res0 = mlib_fabs(src0);
		sr1 = (src1 >> 31);
		res1 = (src1 ^ sr1) - sr1;

		res2 = mlib_fabs(src2);
		sr3 = (src3 >> 31);
		res3 = (src3 ^ sr3) - sr3;

		sum0 += res0;
		sum1 += res1;
		sum2 += res2;
		sum3 += res3;
	}

	if (i < n) {
		src0 = x[i + 0];
		src1 = x[i + 1];

		res0 = mlib_fabs(src0);
		sr1 = (src1 >> 31);
		res1 = (src1 ^ sr1) - sr1;

		sum0 += res0;
		sum1 += res1;
	}

	z[0] = sum0 + sum1 + sum2 + sum3;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
