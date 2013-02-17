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

#pragma ident	"@(#)mlib_s_VectorZero.c	9.5	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorZero_U8  - zeroing of 8-bit format vector
 *      mlib_VectorZero_S8  - zeroing of 8-bit format vector
 *      mlib_VectorZero_S16 - zeroing of 16-bit format vector
 *      mlib_VectorZero_S32 - zeroing of 32-bit format vector
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorZero_U8(mlib_u8  *z,
 *                                     mlib_s32 n)
 *      mlib_status mlib_VectorZero_S8(mlib_s8  *z,
 *                                     mlib_s32 n)
 *      mlib_status mlib_VectorZero_S16(mlib_s16 *z,
 *                                      mlib_s32 n)
 *      mlib_status mlib_VectorZero_S32(mlib_s32 *z,
 *                                      mlib_s32 n)
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      n    number of elements in the vector
 *
 * DESCRIPTION
 *      z = 0
 *
 */

/*
 * FUNCTIONS
 *      mlib_VectorZero_U8C - zeroing of 8-bit format complex vector
 *      mlib_VectorZero_S8C - zeroing of 8-bit format complex vector
 *      mlib_VectorZero_S16C - zeroing of 16-bit format complex vector
 *      mlib_VectorZero_S32C - zeroing of 32-bit format complex vector
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorZero_U8C(mlib_u8  *z,
 *                                      mlib_s32 n)
 *      mlib_status mlib_VectorZero_S8C(mlib_s8  *z,
 *                                      mlib_s32 n)
 *      mlib_status mlib_VectorZero_S16C(mlib_s16 *z,
 *                                       mlib_s32 n)
 *      mlib_status mlib_VectorZero_S32C(mlib_s32 *z,
 *                                       mlib_s32 n)
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result vector.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts
 *      n    number of complex elements in the vector
 *
 * DESCRIPTION
 *      z = 0
 *
 * IMPLEMENTATION
 *
 *      Because very often using the base of all sources are
 *      copied from mlib_VectorSet*.
 */

#include <mlib_algebra.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorZero_S32C = __mlib_VectorZero_S32C
#pragma weak mlib_VectorZero_S8 = __mlib_VectorZero_S8
#pragma weak mlib_VectorZero_S16C = __mlib_VectorZero_S16C
#pragma weak mlib_VectorZero_U8 = __mlib_VectorZero_U8
#pragma weak mlib_VectorZero_S8C = __mlib_VectorZero_S8C
#pragma weak mlib_VectorZero_U8C = __mlib_VectorZero_U8C
#pragma weak mlib_VectorZero_S32 = __mlib_VectorZero_S32
#pragma weak mlib_VectorZero_S16 = __mlib_VectorZero_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorZero_S32C) mlib_VectorZero_S32C
	__attribute__((weak, alias("__mlib_VectorZero_S32C")));
__typeof__(__mlib_VectorZero_S8) mlib_VectorZero_S8
	__attribute__((weak, alias("__mlib_VectorZero_S8")));
__typeof__(__mlib_VectorZero_S16C) mlib_VectorZero_S16C
	__attribute__((weak, alias("__mlib_VectorZero_S16C")));
__typeof__(__mlib_VectorZero_U8) mlib_VectorZero_U8
	__attribute__((weak, alias("__mlib_VectorZero_U8")));
__typeof__(__mlib_VectorZero_S8C) mlib_VectorZero_S8C
	__attribute__((weak, alias("__mlib_VectorZero_S8C")));
__typeof__(__mlib_VectorZero_U8C) mlib_VectorZero_U8C
	__attribute__((weak, alias("__mlib_VectorZero_U8C")));
__typeof__(__mlib_VectorZero_S32) mlib_VectorZero_S32
	__attribute__((weak, alias("__mlib_VectorZero_S32")));
__typeof__(__mlib_VectorZero_S16) mlib_VectorZero_S16
	__attribute__((weak, alias("__mlib_VectorZero_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SET_ZERO(TYPE)							\
	mlib_s32 i, nstep, n1, n2, n3;					\
	TYPE *pdst = z;							\
	__m128i zero = _mm_setzero_si128();				\
									\
	if (n < 1)							\
		return (MLIB_FAILURE);					\
									\
	nstep = 16 / sizeof (TYPE);					\
	n1 = ((16 - ((mlib_addr)z & 15)) & 15) / sizeof (TYPE);		\
	n2 = (n - n1) / nstep;						\
	n3 = n - n1 - n2 * nstep;					\
									\
	if (n2 < 1) { 							\
		for (i = 0; i < n; i++)					\
			*pdst++ = 0;					\
	} else {							\
		for (i = 0; i < n1; i++)				\
			*pdst++ = 0;					\
		for (i = 0; i < n2; i++) {				\
			_mm_store_si128((__m128i *)pdst, zero);		\
			pdst += nstep;					\
		}							\
		for (i = 0; i < n3; i++)				\
			*pdst++ = 0;					\
	}								\
									\
	return (MLIB_SUCCESS)

/* *********************************************************** */

mlib_status
__mlib_VectorZero_U8(
	mlib_u8 *z,
	mlib_s32 n)
{
	SET_ZERO(mlib_u8);
}

/* *********************************************************** */

mlib_status
__mlib_VectorZero_S8(
	mlib_s8 *z,
	mlib_s32 n)
{
	SET_ZERO(mlib_s8);
}

/* *********************************************************** */

mlib_status
__mlib_VectorZero_S16(
	mlib_s16 *z,
	mlib_s32 n)
{
	SET_ZERO(mlib_s16);
}

/* *********************************************************** */

mlib_status
__mlib_VectorZero_S32(
	mlib_s32 *z,
	mlib_s32 n)
{
	SET_ZERO(mlib_s32);
}

/* *********************************************************** */

mlib_status
__mlib_VectorZero_U8C(
	mlib_u8 *z,
	mlib_s32 n)
{
	return (__mlib_VectorZero_U8((mlib_u8 *)z, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorZero_S8C(
	mlib_s8 *z,
	mlib_s32 n)
{
	return (__mlib_VectorZero_U8((mlib_u8 *)z, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorZero_S16C(
	mlib_s16 *z,
	mlib_s32 n)
{
	return (__mlib_VectorZero_S16((mlib_s16 *)z, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorZero_S32C(
	mlib_s32 *z,
	mlib_s32 n)
{
	return (__mlib_VectorZero_S32((mlib_s32 *)z, n + n));
}

/* *********************************************************** */
