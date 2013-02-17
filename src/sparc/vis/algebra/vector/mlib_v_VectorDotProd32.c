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

#pragma ident	"@(#)mlib_v_VectorDotProd32.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorDotProd_S32_Sat - dot product of signed 32-bit format vectors
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorDotProd_S32_Sat(mlib_d64       *z,
 *                                             const mlib_s32 *x,
 *                                             const mlib_s32 *y,
 *                                             mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the dot product of the vectors
 *      x    pointer to the first element of the first vector
 *      y    pointer to the first element of the second vector
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *           n
 *      z = SUM (x[i] * y[i])
 *          i = 1
 */

#include <mlib_algebra.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorDotProd_S32_Sat = __mlib_VectorDotProd_S32_Sat

#elif defined(__GNUC__)

__typeof__(__mlib_VectorDotProd_S32_Sat) mlib_VectorDotProd_S32_Sat
	__attribute__((weak, alias("__mlib_VectorDotProd_S32_Sat")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VectorDotProd_S32_Sat(
	mlib_d64 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 dot = 0;
	mlib_s32 *px = (mlib_s32 *)x, *py = (mlib_s32 *)y;

	if (n <= 0)
		return (MLIB_FAILURE);

	for (i = 0; i < n; i++)
		dot += px[i] * (mlib_d64)py[i];
	z[0] = dot;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
