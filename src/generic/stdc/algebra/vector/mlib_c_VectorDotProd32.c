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

#pragma ident	"@(#)mlib_c_VectorDotProd32.c	9.4	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorDotProd_S32_Sat - dot product of signed 32-bit format
 *                                   vectors
 *
 * SYNOPSIS
 *     mlib_status mlib_VectorDotProd_S32_Sat(mlib_d64       *z,
 *                                            const mlib_s32 *x,
 *                                            const mlib_s32 *y,
 *                                            mlib_s32       n);
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

/*
 * FUNCTIONS
 *      mlib_VectorDotProd_S32C_Sat - dot product of signed 32-bit format
 *                                    complex vectors
 *
 * SYNOPSIS
 *     mlib_status mlib_VectorDotProd_S32C_Sat(mlib_d64       *z,
 *                                             const mlib_s32 *x,
 *                                             const mlib_s32 *y,
 *                                             mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the dot product of the vectors. z[0] hold the
 *           real parts, and z[1] hold the imaginary parts
 *      x    pointer to the first complex element of the first vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      y    pointer to the first complex element of the second vector.
 *           y[2*i] hold the real parts, and y[2*i + 1] hold the imaginary
 *           parts
 *      n    number of complex elements in the vectors
 *
 * DESCRIPTION
 *           n          *
 *      z = SUM (x[i] * y[i] )
 *          i = 1
 */

#include <mlib_algebra.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorDotProd_S32_Sat = __mlib_VectorDotProd_S32_Sat
#pragma weak mlib_VectorDotProd_S32C_Sat = __mlib_VectorDotProd_S32C_Sat

#elif defined(__GNUC__)

__typeof__(__mlib_VectorDotProd_S32_Sat) mlib_VectorDotProd_S32_Sat
	__attribute__((weak, alias("__mlib_VectorDotProd_S32_Sat")));
__typeof__(__mlib_VectorDotProd_S32C_Sat) mlib_VectorDotProd_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorDotProd_S32C_Sat")));

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
	mlib_s32 *px, *py;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s32 *)x;
	py = (mlib_s32 *)y;

	for (i = 0; i < n; i++)
		dot += px[i] * (mlib_d64)py[i];
	z[0] = dot;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorDotProd_S32C_Sat(
	mlib_d64 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 dot_r = 0, dot_i = 0;
	mlib_s32 *px, *py;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s32 *)x;
	py = (mlib_s32 *)y;

/* (Xr + jXi) * (Yr - jYi) = (XrYr + XiYi) + j(XiYr - XrYi) */

	for (i = 0; i < n; i++) {
		dot_r = dot_r + px[2 * i] * (mlib_d64)py[2 * i] +
			px[2 * i + 1] * (mlib_d64)py[2 * i + 1];
		dot_i = dot_i + px[2 * i + 1] * (mlib_d64)py[2 * i] -
			px[2 * i] * (mlib_d64)py[2 * i + 1];
	}

	z[0] = dot_r;
	z[1] = dot_i;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
