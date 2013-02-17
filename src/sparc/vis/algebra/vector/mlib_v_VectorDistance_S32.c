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

#pragma ident	"@(#)mlib_v_VectorDistance_S32.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *     mlib_VectorDistance_[U8|S8|S16|S32]_Sat - Euclidean distance
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorDistance_S32_Sat(mlib_d64       *z,
 *                                              const mlib_s32 *x,
 *                                              const mlib_s32 *y,
 *                                              mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the Euclidean distance
 *      x    pointer to the first input vector
 *      y    pointer to the second input vector
 *      n    number of elements in the vector
 *
 * DESCRIPTION
 *                n - 1
 *      z = sqrt( SUM (x[i] - y[i])^2 )
 *                i = 0
 */

#include <mlib_algebra.h>
#include <mlib_SysMath.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorDistance_S32_Sat = __mlib_VectorDistance_S32_Sat

#elif defined(__GNUC__)

__typeof__(__mlib_VectorDistance_S32_Sat) mlib_VectorDistance_S32_Sat
	__attribute__((weak, alias("__mlib_VectorDistance_S32_Sat")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VectorDistance_S32_Sat(
	mlib_d64 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	mlib_d64 acc, item;
	mlib_s32 i;

	if (n <= 0)
		return (MLIB_FAILURE);

	acc = 0.0;
	for (i = 0; i < n; i++) {
		item = x[i] - (mlib_d64)y[i];
		acc += item * item;
	}

	z[0] = mlib_sqrt(acc);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
