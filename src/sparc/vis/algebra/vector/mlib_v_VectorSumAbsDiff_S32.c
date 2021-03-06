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

#pragma ident	"@(#)mlib_v_VectorSumAbsDiff_S32.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *     mlib_VectorSumAbsDiff_[U8|S8|S16|S32]_Sat - sum of absolute difference
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorSumAbsDiff_S32_Sat(mlib_d64       *z,
 *                                                const mlib_s32 *x,
 *                                                const mlib_s32 *y,
 *                                                mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the sum of the absolute difference
 *      x    pointer to the first input vector
 *      y    pointer to the second input vector
 *      n    number of elements in the vector
 *
 * DESCRIPTION
 *          n - 1
 *      z = SUM |x[i] - y[i]|
 *          i = 0
 */

#include <mlib_algebra.h>
#include <mlib_SysMath.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorSumAbsDiff_S32_Sat = __mlib_VectorSumAbsDiff_S32_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorSumAbsDiff_S32_Sat) mlib_VectorSumAbsDiff_S32_Sat
	__attribute__((weak, alias("__mlib_VectorSumAbsDiff_S32_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VectorSumAbsDiff_S32_Sat(
	mlib_d64 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 sum0 = 0, src0, res0;
	mlib_s64 sum1 = 0, sr1, src1, res1;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (i = n & 1) {
		src0 = x[0] - (mlib_d64)y[0];
		sum0 += mlib_fabs(src0);
	}
#pragma pipeloop(0)
	for (; i < n - 1; i += 2) {

		src0 = x[i + 0] - (mlib_d64)y[i + 0];
		src1 = x[i + 1] - (mlib_s64)y[i + 1];

		res0 = mlib_fabs(src0);
		sr1 = (src1 >> 63);
		res1 = (src1 ^ sr1) - sr1;

		sum0 += res0;
		sum1 += res1;
	}

	z[0] = sum0 + sum1;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
