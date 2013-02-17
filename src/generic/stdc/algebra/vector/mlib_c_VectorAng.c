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

#pragma ident	"@(#)mlib_c_VectorAng.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorAng_U8C - phase of unsigned 8-bit format complex vector
 *      mlib_VectorAng_S8C - phase of signed 8-bit format complex vector
 *      mlib_VectorAng_S16C - phase of 16-bit format complex vector
 *      mlib_VectorAng_S32C - phase of 32-bit format complex vector
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorAng_U8C(mlib_d64      * a,
 *                                     const mlib_u8 * x,
 *                                     mlib_s32      n);
 *      mlib_status mlib_VectorAng_S8C(mlib_d64      *a,
 *                                     const mlib_s8 *x,
 *                                     mlib_s32      n);
 *      mlib_status mlib_VectorAng_S16C(mlib_d64       *a,
 *                                      const mlib_s16 *x,
 *                                      mlib_s32       n);
 *      mlib_status mlib_VectorAng_S32C(mlib_d64       *a,
 *                                      const mlib_s32 *x,
 *                                      mlib_s32       n);
 *
 * ARGUMENTS
 *      a    pointer to the output phase vector
 *      x    pointer to the first complex element of the input vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      n    number of complex elements in the x vector
 *
 * DESCRIPTION
 *      a = atan2(x_imag, x_real)
 *      function atan2 return a value in radians, in the range of -pi to pi.
 */

#include <mlib_algebra.h>
#include <mlib_AlgebraUtil.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorAng_S16C = __mlib_VectorAng_S16C
#pragma weak mlib_VectorAng_S8C = __mlib_VectorAng_S8C
#pragma weak mlib_VectorAng_U8C = __mlib_VectorAng_U8C
#pragma weak mlib_VectorAng_S32C = __mlib_VectorAng_S32C

#elif defined(__GNUC__)

__typeof__(__mlib_VectorAng_S16C)
    mlib_VectorAng_S16C __attribute__((weak, alias("__mlib_VectorAng_S16C")));

__typeof__(__mlib_VectorAng_S8C)
    mlib_VectorAng_S8C __attribute__((weak, alias("__mlib_VectorAng_S8C")));

__typeof__(__mlib_VectorAng_U8C)
    mlib_VectorAng_U8C __attribute__((weak, alias("__mlib_VectorAng_U8C")));

__typeof__(__mlib_VectorAng_S32C)
    mlib_VectorAng_S32C __attribute__((weak, alias("__mlib_VectorAng_S32C")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VectorAng_U8C(
	mlib_d64 *a,
	const mlib_u8 *x,
	mlib_s32 n)
{
	mlib_s32 i;

	for (i = 0; i < n; i++) {
		a[i] = mlib_atan2i(((mlib_u8 *)x)[2 * i + 1],
			((mlib_u8 *)x)[2 * i]);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAng_S8C(
	mlib_d64 *a,
	const mlib_s8 *x,
	mlib_s32 n)
{
	mlib_s32 i;

	for (i = 0; i < n; i++) {
		a[i] = mlib_atan2i(((mlib_s8 *)x)[2 * i + 1],
			((mlib_s8 *)x)[2 * i]);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAng_S16C(
	mlib_d64 *a,
	const mlib_s16 *x,
	mlib_s32 n)
{
	mlib_s32 i;

	for (i = 0; i < n; i++) {
		a[i] = mlib_atan2i(((mlib_s16 *)x)[2 * i + 1],
			((mlib_s16 *)x)[2 * i]);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAng_S32C(
	mlib_d64 *a,
	const mlib_s32 *x,
	mlib_s32 n)
{
	mlib_s32 i;

	for (i = 0; i < n; i++) {
		a[i] = mlib_atan2i(((mlib_s32 *)x)[2 * i + 1],
			((mlib_s32 *)x)[2 * i]);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */
