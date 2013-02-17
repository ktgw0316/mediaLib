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

#pragma ident	"@(#)mlib_VectorMinimum.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorMinimum - find the minimum
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMinimum_S32(mlib_s32       *min,
 *                                         const mlib_s32 *x,
 *                                         mlib_s32       n);
 *      mlib_status mlib_VectorMinimum_F32(mlib_f32       *min,
 *                                         const mlib_f32 *x,
 *                                         mlib_s32       n);
 *      mlib_status mlib_VectorMinimum_D64(mlib_d64       *min,
 *                                         const mlib_d64 *x,
 *                                         mlib_s32       n);
 *
 * ARGUMENTS
 *      min    the pointer to minimum value
 *      x      the pointer to the first element of the source vector
 *      n      the number of elements in the source vector
 *
 * DESCRIPTION
 *            n - 1
 *      min = Min x[i]
 *            i = 0
 */

/*
 * FUNCTIONS
 *      mlib_VectorMinimumMag - find the minimum
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMinimumMag_S32C(mlib_s32       *min,
 *                                             const mlib_s32 *x,
 *                                             mlib_s32       n);
 *      mlib_status mlib_VectorMinimumMag_F32C(mlib_f32       *min,
 *                                             const mlib_f32 *x,
 *                                             mlib_s32       n);
 *      mlib_status mlib_VectorMinimumMag_D64C(mlib_d64       *min,
 *                                             const mlib_d64 *x,
 *                                             mlib_s32       n);
 *
 * ARGUMENTS
 *      min    the pointer to minimum value
 *      x      the pointer to the first element of the source vector
 *      n      the number of elements in the source vector
 *
 * DESCRIPTION
 *      This function finds the first element with the minimum
 *      magnitude, then puts the real and imaginary parts of it into
 *      min[0] and min[1], respectively.
 *
 *            n - 1
 *      min = Min x[i]
 *            i = 0
 */

#include <mlib_algebra.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorMinimum_S32 = __mlib_VectorMinimum_S32
#pragma weak mlib_VectorMinimum_F32 = __mlib_VectorMinimum_F32
#pragma weak mlib_VectorMinimum_D64 = __mlib_VectorMinimum_D64
#pragma weak mlib_VectorMinimumMag_S32C = __mlib_VectorMinimumMag_S32C
#pragma weak mlib_VectorMinimumMag_F32C = __mlib_VectorMinimumMag_F32C
#pragma weak mlib_VectorMinimumMag_D64C = __mlib_VectorMinimumMag_D64C

#elif defined(__GNUC__)

__typeof__(__mlib_VectorMinimum_S32) mlib_VectorMinimum_S32
	__attribute__((weak, alias("__mlib_VectorMinimum_S32")));
__typeof__(__mlib_VectorMinimum_F32) mlib_VectorMinimum_F32
	__attribute__((weak, alias("__mlib_VectorMinimum_F32")));
__typeof__(__mlib_VectorMinimum_D64) mlib_VectorMinimum_D64
	__attribute__((weak, alias("__mlib_VectorMinimum_D64")));
__typeof__(__mlib_VectorMinimumMag_S32C) mlib_VectorMinimumMag_S32C
	__attribute__((weak, alias("__mlib_VectorMinimumMag_S32C")));
__typeof__(__mlib_VectorMinimumMag_F32C) mlib_VectorMinimumMag_F32C
	__attribute__((weak, alias("__mlib_VectorMinimumMag_F32C")));
__typeof__(__mlib_VectorMinimumMag_D64C) mlib_VectorMinimumMag_D64C
	__attribute__((weak, alias("__mlib_VectorMinimumMag_D64C")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
#define	MIN_LOG(min, arg)	min += ((arg - min) & ((arg - min) >> 61))
#define	MIN(min, arg)	min = (arg < min) ? arg : min

/* *********************************************************** */

#define	COMPUTE(TYPE, TYPE_S)                                   \
	TYPE *psrc = (TYPE *) x;                                \
	TYPE min1, min2, min3, min4;                            \
	mlib_s32 i;                                             \
	                                                        \
	if (n <= 0)                                             \
	    return (MLIB_FAILURE);                              \
	min1 = min2 = min3 = min4 = MLIB_##TYPE_S##_MAX;        \
	for (i = 0; i <= (n - 4); i += 4) {                     \
	    MIN(min1, psrc[i]);                                 \
	    MIN(min2, psrc[i + 1]);                             \
	    MIN(min3, psrc[i + 2]);                             \
	    MIN(min4, psrc[i + 3]);                             \
	}                                                       \
	                                                        \
	if (i < n)                                              \
	    MIN(min1, psrc[i]);                                 \
	i++;                                                    \
	if (i < n)                                              \
	    MIN(min2, psrc[i]);                                 \
	i++;                                                    \
	if (i < n)                                              \
	    MIN(min3, psrc[i]);                                 \
	MIN(min1, min2);                                        \
	MIN(min3, min4);                                        \
	MIN(min1, min3);                                        \
	min[0] = min1;                                          \
	return MLIB_SUCCESS


/* *********************************************************** */

#define	COMPUTE_C_INT(TYPE, TYPE_S)                                  \
	TYPE *psrc = (TYPE *) x;                                 \
	mlib_s32 __min_re, __min_im;                             \
	mlib_s32 i;                                              \
	mlib_s32 __val_re;                                       \
	mlib_s32 __val_im;                                       \
	mlib_u64 __val;                                          \
	mlib_u64 __val_min;                                      \
	                                                         \
	if (n <= 0)                                              \
	    return (MLIB_FAILURE);                               \
	__min_re = psrc[0];                                      \
	__min_im = psrc[1];                                      \
	__val_min = (mlib_u64)__min_im * __min_im   \
	+ (mlib_u64)__min_re * __min_re;   \
	for (i = 1; i < n; i++) {                                \
	    __val_re = psrc[2 * i];                              \
	    __val_im = psrc[2 * i + 1];                          \
	    __val = (mlib_u64)__val_im * __val_im   \
		+ (mlib_u64)__val_re * __val_re;   \
	    if (__val < __val_min) {                             \
		__min_re = __val_re;                             \
		__min_im = __val_im;                             \
		__val_min = __val;                               \
	    }                                                    \
	}                                                        \
	                                                         \
	min[0] = __min_re;                                       \
	min[1] = __min_im;                                       \
	return MLIB_SUCCESS

/* *********************************************************** */

#define	COMPUTE_C(TYPE, TYPE_S)                                  \
	TYPE *psrc = (TYPE *) x;                                 \
	mlib_d64 __min_re, __min_im;                             \
	mlib_s32 i;                                              \
	mlib_d64 __val_re;                                       \
	mlib_d64 __val_im;                                       \
	mlib_d64 __val;                                          \
	mlib_d64 __val_min;                                      \
	                                                         \
	if (n <= 0)                                              \
	    return (MLIB_FAILURE);                               \
	__min_re = psrc[0];                                      \
	__min_im = psrc[1];                                      \
	__val_min = __min_im * __min_im + __min_re * __min_re;   \
	for (i = 1; i < n; i++) {                                \
	    __val_re = psrc[2 * i];                              \
	    __val_im = psrc[2 * i + 1];                          \
	    __val = __val_im * __val_im + __val_re * __val_re;   \
	    if (__val < __val_min) {                             \
		__min_re = __val_re;                             \
		__min_im = __val_im;                             \
		__val_min = __val;                               \
	    }                                                    \
	}                                                        \
	                                                         \
	min[0] = __min_re;                                       \
	min[1] = __min_im;                                       \
	return MLIB_SUCCESS

/* *********************************************************** */

mlib_status
__mlib_VectorMinimum_S32(
	mlib_s32 *min,
	const mlib_s32 *x,
	mlib_s32 n)
{
	mlib_s32 *psrc = (mlib_s32 *)x;
#ifdef _NO_LONGLONG
	mlib_d64 min1, min2, min3, min4, min5, min6, min7, min8;
#else
	mlib_s64 min1, min2, min3, min4, min5, min6, min7, min8;
#endif
	mlib_s32 i;

	if (n <= 0)
		return (MLIB_FAILURE);

	min1 = min2 = min3 = min4 = MLIB_S32_MAX;
	min5 = min6 = min7 = min8 = MLIB_S32_MAX;

	for (i = 0; i <= (n - 8); i += 8) {
#ifdef _NO_LONGLONG
		MIN(min1, psrc[i]);
		MIN(min2, psrc[i + 1]);
		MIN(min3, psrc[i + 2]);
		MIN(min4, psrc[i + 3]);
		MIN(min5, psrc[i + 4]);
		MIN(min6, psrc[i + 5]);
		MIN(min7, psrc[i + 6]);
		MIN(min8, psrc[i + 7]);
#else
		MIN_LOG(min1, psrc[i]);
		MIN_LOG(min2, psrc[i + 1]);
		MIN_LOG(min3, psrc[i + 2]);
		MIN_LOG(min4, psrc[i + 3]);
		MIN_LOG(min5, psrc[i + 4]);
		MIN_LOG(min6, psrc[i + 5]);
		MIN_LOG(min7, psrc[i + 6]);
		MIN_LOG(min8, psrc[i + 7]);
#endif
	}

	if (i < n) {
#ifdef _NO_LONGLONG
		MIN(min1, psrc[i]);
#else
		MIN_LOG(min1, psrc[i]);
#endif
	}
	i++;

	if (i < n) {
#ifdef _NO_LONGLONG
		MIN(min2, psrc[i]);
#else
		MIN_LOG(min2, psrc[i]);
#endif
	}
	i++;

	if (i < n) {
#ifdef _NO_LONGLONG
		MIN(min3, psrc[i]);
#else
		MIN_LOG(min3, psrc[i]);
#endif
	}
	i++;

	if (i < n) {
#ifdef _NO_LONGLONG
		MIN(min4, psrc[i]);
#else
		MIN_LOG(min4, psrc[i]);
#endif
	}
	i++;

	if (i < n) {
#ifdef _NO_LONGLONG
		MIN(min5, psrc[i]);
#else
		MIN_LOG(min5, psrc[i]);
#endif
	}
	i++;

	if (i < n) {
#ifdef _NO_LONGLONG
		MIN(min6, psrc[i]);
#else
		MIN_LOG(min6, psrc[i]);
#endif
	}
	i++;

	if (i < n) {
#ifdef _NO_LONGLONG
		MIN(min7, psrc[i]);
#else
		MIN_LOG(min7, psrc[i]);
#endif
	}

#ifdef _NO_LONGLONG
	MIN(min1, min2);
	MIN(min3, min4);
	MIN(min5, min6);
	MIN(min7, min8);
	MIN(min1, min3);
	MIN(min5, min7);
	MIN(min1, min5);

	min[0] = (mlib_s32) min1;
#else
	MIN_LOG(min1, min2);
	MIN_LOG(min3, min4);
	MIN_LOG(min5, min6);
	MIN_LOG(min7, min8);
	MIN_LOG(min1, min3);
	MIN_LOG(min5, min7);
	MIN_LOG(min1, min5);

	min[0] = min1;
#endif

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMinimum_F32(
	mlib_f32 *min,
	const mlib_f32 *x,
	mlib_s32 n)
{
	COMPUTE(mlib_f32,
		F32);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMinimum_D64(
	mlib_d64 *min,
	const mlib_d64 *x,
	mlib_s32 n)
{
	COMPUTE(mlib_d64,
		D64);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMinimumMag_S32C(
	mlib_s32 *min,
	const mlib_s32 *x,
	mlib_s32 n)
{
#ifdef _NO_LONGLONG
	COMPUTE_C(mlib_s32,
		S32);
#else
	COMPUTE_C_INT(mlib_s32,
		S32);
#endif
}

/* *********************************************************** */

mlib_status
__mlib_VectorMinimumMag_F32C(
	mlib_f32 *min,
	const mlib_f32 *x,
	mlib_s32 n)
{
	COMPUTE_C(mlib_f32,
		F32);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMinimumMag_D64C(
	mlib_d64 *min,
	const mlib_d64 *x,
	mlib_s32 n)
{
	COMPUTE_C(mlib_d64,
		D64);
}

/* *********************************************************** */
