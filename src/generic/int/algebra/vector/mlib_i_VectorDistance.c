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

#pragma ident	"@(#)mlib_i_VectorDistance.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *     mlib_VectorDistance_[U8|S8|S16|S32]_Sat - Euclidean distance
 *
 * SYNOPSIS
 *     mlib_status mlib_VectorDistance_U8_Sat(mlib_d64      *z,
 *                                            const mlib_u8 *x,
 *                                            const mlib_u8 *y,
 *                                            mlib_s32      n);
 *     mlib_status mlib_VectorDistance_S8_Sat(mlib_d64      *z,
 *                                            const mlib_s8 *x,
 *                                            const mlib_s8 *y,
 *                                            mlib_s32      n);
 *     mlib_status mlib_VectorDistance_S16_Sat(mlib_d64       *z,
 *                                             const mlib_s16 *x,
 *                                             const mlib_s16 *y,
 *                                             mlib_s32       n);
 *     mlib_status mlib_VectorDistance_S32_Sat(mlib_d64       *z,
 *                                             const mlib_s32 *x,
 *                                             const mlib_s32 *y,
 *                                             mlib_s32       n);
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

#pragma weak mlib_VectorDistance_S16_Sat = __mlib_VectorDistance_S16_Sat
#pragma weak mlib_VectorDistance_S8_Sat = __mlib_VectorDistance_S8_Sat
#pragma weak mlib_VectorDistance_U8_Sat = __mlib_VectorDistance_U8_Sat
#pragma weak mlib_VectorDistance_S32_Sat = __mlib_VectorDistance_S32_Sat

#elif defined(__GNUC__)

__typeof__(__mlib_VectorDistance_S16_Sat) mlib_VectorDistance_S16_Sat
	__attribute__((weak, alias("__mlib_VectorDistance_S16_Sat")));
__typeof__(__mlib_VectorDistance_S8_Sat) mlib_VectorDistance_S8_Sat
	__attribute__((weak, alias("__mlib_VectorDistance_S8_Sat")));
__typeof__(__mlib_VectorDistance_U8_Sat) mlib_VectorDistance_U8_Sat
	__attribute__((weak, alias("__mlib_VectorDistance_U8_Sat")));
__typeof__(__mlib_VectorDistance_S32_Sat) mlib_VectorDistance_S32_Sat
	__attribute__((weak, alias("__mlib_VectorDistance_S32_Sat")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef	_NO_LONGLONG

typedef mlib_d64 LONGTYPE;

#else

typedef mlib_s64 LONGTYPE;

#endif

/* *********************************************************** */

#define	MLIB_C_VECTORDISTANCE(DTYPE, Z, X, Y, N)                \
	DTYPE *px, *py;                                         \
	LONGTYPE acc, item;                                     \
	mlib_s32 i;                                             \
	                                                        \
	if (N <= 0)                                             \
	    return (MLIB_FAILURE);                              \
	px = (DTYPE *) X;                                       \
	py = (DTYPE *) Y;                                       \
	acc = 0;                                                \
	for (i = 0; i < N; i++) {                               \
	    item = (LONGTYPE)(px[i] - py[i]);                   \
	    acc += item * item;                                 \
	}                                                       \
	                                                        \
	Z[0] = mlib_sqrt((mlib_d64)acc);                        \
	                                                        \
	return MLIB_SUCCESS

/* *********************************************************** */

mlib_status
__mlib_VectorDistance_U8_Sat(
	mlib_d64 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	MLIB_C_VECTORDISTANCE(mlib_u8,
		z,
		x,
		y,
		n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorDistance_S8_Sat(
	mlib_d64 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	MLIB_C_VECTORDISTANCE(mlib_s8,
		z,
		x,
		y,
		n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorDistance_S16_Sat(
	mlib_d64 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	MLIB_C_VECTORDISTANCE(mlib_s16,
		z,
		x,
		y,
		n);
}

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
