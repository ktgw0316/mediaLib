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

#pragma ident	"@(#)mlib_c_VectorConj.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorConj_S8C_S8C_Sat,
 *      mlib_VectorConj_S8C_Sat      - computation of 8-bit format
 *                                     complex conjugated vector
 *      mlib_VectorConj_S16C_S16C_Sat,
 *      mlib_VectorConj_S16C_Sat      - computation of 16-bit format
 *                                      complex conjugated vector
 *      mlib_VectorConj_S32C_S32C_Sat,
 *      mlib_VectorConj_S32C_Sat      - computation of 32-bit format
 *                                      complex conjugated vector
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorConj_S8C_S8C_Sat(mlib_s8       *z,
 *                                              const mlib_s8 *x,
 *                                              mlib_s32      n);
 *      mlib_status mlib_VectorConj_S8C_Sat(mlib_s8  *xz,
 *                                          mlib_s32 n);
 *
 *      mlib_status mlib_VectorConj_S16C_S16C_Sat(mlib_s16      *z,
 *                                                constmlib_s16 *x,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorConj_S16C_Sat(mlib_s16 *xz,
 *                                           mlib_s32 n);
 *      mlib_status mlib_VectorConj_S32C_S32C_Sat(mlib_s32 *x,
 *                                                mlib_s32 n);
 *      mlib_status mlib_VectorConj_S32C_Sat(mlib_s32 *xz,
 *                                           mlib_s32 n);
 * ARGUMENTS
 *      z    pointer to the first complex element of the result vector.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts
 *      x    pointer to the first complex element of the input vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      n    number of complex elements in the vectors
 *
 * DESCRIPTION
 *           *
 *      z = x
 */

#include <mlib_algebra.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorConj_S32C_Sat = __mlib_VectorConj_S32C_Sat
#pragma weak mlib_VectorConj_S8C_S8C_Sat = __mlib_VectorConj_S8C_S8C_Sat
#pragma weak mlib_VectorConj_S32C_S32C_Sat = __mlib_VectorConj_S32C_S32C_Sat
#pragma weak mlib_VectorConj_S16C_Sat = __mlib_VectorConj_S16C_Sat
#pragma weak mlib_VectorConj_S8C_Sat = __mlib_VectorConj_S8C_Sat
#pragma weak mlib_VectorConj_S16C_S16C_Sat = __mlib_VectorConj_S16C_S16C_Sat

#elif defined(__GNUC__)

__typeof__(__mlib_VectorConj_S32C_Sat) mlib_VectorConj_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorConj_S32C_Sat")));
__typeof__(__mlib_VectorConj_S8C_S8C_Sat) mlib_VectorConj_S8C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorConj_S8C_S8C_Sat")));
__typeof__(__mlib_VectorConj_S32C_S32C_Sat) mlib_VectorConj_S32C_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorConj_S32C_S32C_Sat")));
__typeof__(__mlib_VectorConj_S16C_Sat) mlib_VectorConj_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorConj_S16C_Sat")));
__typeof__(__mlib_VectorConj_S8C_Sat) mlib_VectorConj_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorConj_S8C_Sat")));
__typeof__(__mlib_VectorConj_S16C_S16C_Sat) mlib_VectorConj_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorConj_S16C_S16C_Sat")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CHECK                                                   \
	if ((xx == NULL) || (zz == NULL) || (n <= 0))           \
	    return MLIB_FAILURE

#define	CHECK_INP                                               \
	if ((xz == NULL) || (n <= 0))                           \
	    return MLIB_FAILURE

#define	CONTROL(MAX, MIN, obj)	(((c = obj) == MIN) ? MAX : - c)

#define	CONJC(form, MAX, MIN)                                   \
	{                                                       \
	    const form *src = x;                                \
	    form *dst = z, c;                                   \
	    mlib_s32 i;                                         \
	                                                        \
	    if (i = (n & 1) * 2) {                              \
		dst[0] = src[0];                                \
		dst[1] = CONTROL(MAX, MIN, src[1]);             \
	    }                                                   \
	    for (; i < (mlib_s32)n * 2; ) {                      \
		dst[i] = src[i];                                \
		i++;                                            \
		dst[i] = CONTROL(MAX, MIN, src[i]);             \
		i++;                                            \
		dst[i] = src[i];                                \
		i++;                                            \
		dst[i] = CONTROL(MAX, MIN, src[i]);             \
		i++;                                            \
	    }                                                   \
	}                                                       \
	                                                        \
	return MLIB_SUCCESS

/* *********************************************************** */

#define	CONJC_IN_THE_SAME_PLACE(form, MAX, MIN)                 \
	{                                                       \
	    form *src = x, c;                                   \
	    mlib_s32 i;                                         \
	                                                        \
	    if (i = (n & 1) * 2) {                              \
		src[1] = CONTROL(MAX, MIN, src[1]);             \
	    }                                                   \
	    for (i++; i < (mlib_s32)n * 2; i += 4) {            \
		src[i] = CONTROL(MAX, MIN, src[i]);             \
		src[i + 2] = CONTROL(MAX, MIN, src[i + 2]);     \
	    }                                                   \
	}                                                       \
	                                                        \
	return MLIB_SUCCESS

/* *********************************************************** */

mlib_status
__mlib_VectorConj_S8C_S8C_Sat(
	mlib_s8 *zz,
	const mlib_s8 *xx,
	mlib_s32 n)
{
	const mlib_s8 *x = xx;
	mlib_s8 *z = zz;

	CHECK;

	CONJC(mlib_s8,
		MLIB_S8_MAX,
		MLIB_S8_MIN);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConj_S8C_Sat(
	mlib_s8 *xz,
	mlib_s32 n)
{
	mlib_s8 *x = xz;

	CHECK_INP;

	CONJC_IN_THE_SAME_PLACE(mlib_s8,
		MLIB_S8_MAX,
		MLIB_S8_MIN);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConj_S16C_S16C_Sat(
	mlib_s16 *zz,
	const mlib_s16 *xx,
	mlib_s32 n)
{
	const mlib_s16 *x = xx;
	mlib_s16 *z = zz;

	CHECK;

	CONJC(mlib_s16,
		MLIB_S16_MAX,
		MLIB_S16_MIN);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConj_S16C_Sat(
	mlib_s16 *xz,
	mlib_s32 n)
{
	mlib_s16 *x = xz;

	CHECK_INP;

	CONJC_IN_THE_SAME_PLACE(mlib_s16,
		MLIB_S16_MAX,
		MLIB_S16_MIN);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConj_S32C_S32C_Sat(
	mlib_s32 *zz,
	const mlib_s32 *xx,
	mlib_s32 n)
{
	const mlib_s32 *x = xx;
	mlib_s32 *z = zz;

	CHECK;

	CONJC(mlib_s32,
		MLIB_S32_MAX,
		MLIB_S32_MIN);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConj_S32C_Sat(
	mlib_s32 *xz,
	mlib_s32 n)
{
	mlib_s32 *x = xz;

	CHECK_INP;

	CONJC_IN_THE_SAME_PLACE(mlib_s32,
		MLIB_S32_MAX,
		MLIB_S32_MIN);
}

/* *********************************************************** */
