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

#pragma ident	"@(#)mlib_VectorMaximum.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorMaximum - find the maximum
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMaximum_S32(mlib_s32       *max,
 *                                         const mlib_s32 *x,
 *                                         mlib_s32       n);
 *      mlib_status mlib_VectorMaximum_F32(mlib_f32       *max,
 *                                         const mlib_f32 *x,
 *                                         mlib_s32       n);
 *      mlib_status mlib_VectorMaximum_D64(mlib_d64       *max,
 *                                         const mlib_d64 *x,
 *                                         mlib_s32       n);
 *
 * ARGUMENTS
 *      max    the pointer to maximum value
 *      x      the pointer to the first element of the source vector
 *      n      the number of elements in the source vector
 *
 * DESCRIPTION
 *            n - 1
 *      max = Max x[i]
 *            i = 0
 */

/*
 * FUNCTIONS
 *      mlib_VectorMaximumMag - find the maximum
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMaximumMag_S32C(mlib_s32       *max,
 *                                             const mlib_s32 *x,
 *                                             mlib_s32       n);
 *      mlib_status mlib_VectorMaximumMag_F32C(mlib_f32       *max,
 *                                             const mlib_f32 *x,
 *                                             mlib_s32       n);
 *      mlib_status mlib_VectorMaximumMag_D64C(mlib_d64       *max,
 *                                             const mlib_d64 *x,
 *                                             mlib_s32       n);
 *
 * ARGUMENTS
 *      max    the pointer to maximum value
 *      x      the pointer to the first element of the source vector
 *      n      the number of elements in the source vector
 *
 * DESCRIPTION
 *      This function finds the first element with the maximum
 *      magnitude, then puts the real and imaginary parts of it into
 *      max[0] and max[1], respectively.
 *
 *            n - 1
 *      max = Max x[i]
 *            i = 0
 */

#include <mlib_algebra.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorMaximum_S32 = __mlib_VectorMaximum_S32
#pragma weak mlib_VectorMaximum_F32 = __mlib_VectorMaximum_F32
#pragma weak mlib_VectorMaximum_D64 = __mlib_VectorMaximum_D64
#pragma weak mlib_VectorMaximumMag_S32C = __mlib_VectorMaximumMag_S32C
#pragma weak mlib_VectorMaximumMag_F32C = __mlib_VectorMaximumMag_F32C
#pragma weak mlib_VectorMaximumMag_D64C = __mlib_VectorMaximumMag_D64C

#elif defined(__GNUC__)

__typeof__(__mlib_VectorMaximum_S32) mlib_VectorMaximum_S32
	__attribute__((weak, alias("__mlib_VectorMaximum_S32")));
__typeof__(__mlib_VectorMaximum_F32) mlib_VectorMaximum_F32
	__attribute__((weak, alias("__mlib_VectorMaximum_F32")));
__typeof__(__mlib_VectorMaximum_D64) mlib_VectorMaximum_D64
	__attribute__((weak, alias("__mlib_VectorMaximum_D64")));
__typeof__(__mlib_VectorMaximumMag_S32C) mlib_VectorMaximumMag_S32C
	__attribute__((weak, alias("__mlib_VectorMaximumMag_S32C")));
__typeof__(__mlib_VectorMaximumMag_F32C) mlib_VectorMaximumMag_F32C
	__attribute__((weak, alias("__mlib_VectorMaximumMag_F32C")));
__typeof__(__mlib_VectorMaximumMag_D64C) mlib_VectorMaximumMag_D64C
	__attribute__((weak, alias("__mlib_VectorMaximumMag_D64C")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MAX(max, arg)	max = (arg > max) ? arg : max
#define	MAX_LOG(max, arg)	max -= ((max - arg) & ((max - arg) >> 61))

/* *********************************************************** */

#define	COMPUTE(TYPE, TYPE_S)                                   \
	TYPE *psrc = (TYPE *) x;                                \
	TYPE max1, max2, max3, max4;                            \
	mlib_s32 i;                                             \
	                                                        \
	if (n <= 0)                                             \
	    return (MLIB_FAILURE);                              \
	max1 = max2 = max3 = max4 = MLIB_##TYPE_S##_MIN;        \
	for (i = 0; i <= (n - 4); i += 4) {                     \
	    MAX(max1, psrc[i]);                                 \
	    MAX(max2, psrc[i + 1]);                             \
	    MAX(max3, psrc[i + 2]);                             \
	    MAX(max4, psrc[i + 3]);                             \
	}                                                       \
	                                                        \
	if (i < n)                                              \
	    MAX(max1, psrc[i]);                                 \
	i++;                                                    \
	if (i < n)                                              \
	    MAX(max2, psrc[i]);                                 \
	i++;                                                    \
	if (i < n)                                              \
	    MAX(max3, psrc[i]);                                 \
	MAX(max1, max2);                                        \
	MAX(max3, max4);                                        \
	MAX(max1, max3);                                        \
	max[0] = max1;                                          \
	return MLIB_SUCCESS


/* *********************************************************** */

#define	COMPUTE_C_INT(TYPE, TYPE_S)                                  \
	TYPE *psrc = (TYPE *) x;                                 \
	mlib_s32 __max_re, __max_im;                             \
	mlib_s32 i, index1, index2;                              \
	mlib_s32 __val_re;                                       \
	mlib_s32 __val_im;                                       \
	mlib_u64 __val;                                          \
	mlib_u64 __val_max;                                      \
	                                                         \
	if (n <= 0)                                              \
	    return (MLIB_FAILURE);                               \
	__max_re = __max_im = __val_max = 0;                     \
	for (i = 0; i < n; i++) {                                \
		index1 = i << 1;                                      \
		index2 = index1 + 1;                                      \
	    __val_re = psrc[index1];                              \
	    __val_im = psrc[index2];                          \
	    __val = (mlib_u64)__val_im * __val_im + \
		(mlib_u64)__val_re * __val_re;   \
	    if (__val > __val_max) {                             \
		__max_re = __val_re;                             \
		__max_im = __val_im;                             \
		__val_max = __val;                               \
	    }                                                    \
	}                                                        \
	                                                         \
	max[0] = __max_re;                                       \
	max[1] = __max_im;                                       \
	return MLIB_SUCCESS

/* *********************************************************** */

#define	COMPUTE_C(TYPE, TYPE_S)                                  \
	TYPE *psrc = (TYPE *) x;                                 \
	mlib_d64 __max_re, __max_im;                             \
	mlib_s32 i, index1, index2;                              \
	mlib_d64 __val_re;                                       \
	mlib_d64 __val_im;                                       \
	mlib_d64 __val;                                          \
	mlib_d64 __val_max;                                      \
	                                                         \
	if (n <= 0)                                              \
	    return (MLIB_FAILURE);                               \
	__max_re = __max_im = __val_max = 0;                     \
	for (i = 0; i < n; i++) {                                \
		index1 = i << 1;                                      \
		index2 = index1 + 1;                                      \
	    __val_re = psrc[index1];                              \
	    __val_im = psrc[index2];                          \
	    __val = __val_im * __val_im + __val_re * __val_re;   \
	    if (__val > __val_max) {                             \
		__max_re = __val_re;                             \
		__max_im = __val_im;                             \
		__val_max = __val;                               \
	    }                                                    \
	}                                                        \
	                                                         \
	max[0] = __max_re;                                       \
	max[1] = __max_im;                                       \
	return MLIB_SUCCESS

/* *********************************************************** */

mlib_status
__mlib_VectorMaximum_S32(
	mlib_s32 *max,
	const mlib_s32 *x,
	mlib_s32 n)
{
	mlib_s32 *psrc = (mlib_s32 *)x;
#ifdef _NO_LONGLONG
	mlib_d64 max1, max2, max3, max4, max5, max6, max7, max8;
#else
	mlib_s64 max1, max2, max3, max4, max5, max6, max7, max8;
#endif
	mlib_s32 i;

	if (n <= 0)
		return (MLIB_FAILURE);

	max1 = max2 = max3 = max4 = MLIB_S32_MIN;
	max5 = max6 = max7 = max8 = MLIB_S32_MIN;

	for (i = 0; i <= (n - 8); i += 8) {
#ifdef _NO_LONGLONG
		MAX(max1, psrc[i]);
		MAX(max2, psrc[i + 1]);
		MAX(max3, psrc[i + 2]);
		MAX(max4, psrc[i + 3]);
		MAX(max5, psrc[i + 4]);
		MAX(max6, psrc[i + 5]);
		MAX(max7, psrc[i + 6]);
		MAX(max8, psrc[i + 7]);
#else
		MAX_LOG(max1, psrc[i]);
		MAX_LOG(max2, psrc[i + 1]);
		MAX_LOG(max3, psrc[i + 2]);
		MAX_LOG(max4, psrc[i + 3]);
		MAX_LOG(max5, psrc[i + 4]);
		MAX_LOG(max6, psrc[i + 5]);
		MAX_LOG(max7, psrc[i + 6]);
		MAX_LOG(max8, psrc[i + 7]);
#endif
	}

	if (i < n) {
#ifdef _NO_LONGLONG
		MAX(max1, psrc[i]);
#else
		MAX_LOG(max1, psrc[i]);
#endif
	}
	i++;

	if (i < n) {
#ifdef _NO_LONGLONG
		MAX(max2, psrc[i]);
#else
		MAX_LOG(max2, psrc[i]);
#endif
	}
	i++;

	if (i < n) {
#ifdef _NO_LONGLONG
		MAX(max3, psrc[i]);
#else
		MAX_LOG(max3, psrc[i]);
#endif
	}
	i++;

	if (i < n) {
#ifdef _NO_LONGLONG
		MAX(max4, psrc[i]);
#else
		MAX_LOG(max4, psrc[i]);
#endif
	}
	i++;

	if (i < n) {
#ifdef _NO_LONGLONG
		MAX(max5, psrc[i]);
#else
		MAX_LOG(max5, psrc[i]);
#endif
	}
	i++;

	if (i < n) {
#ifdef _NO_LONGLONG
		MAX(max6, psrc[i]);
#else
		MAX_LOG(max6, psrc[i]);
#endif
	}
	i++;

	if (i < n) {
#ifdef _NO_LONGLONG
		MAX(max7, psrc[i]);
#else
		MAX_LOG(max7, psrc[i]);
#endif
	}

#ifdef _NO_LONGLONG
	MAX(max1, max2);
	MAX(max3, max4);
	MAX(max5, max6);
	MAX(max7, max8);
	MAX(max1, max3);
	MAX(max5, max7);
	MAX(max1, max5);
	max[0] = (mlib_s32) max1;
#else
	MAX_LOG(max1, max2);
	MAX_LOG(max3, max4);
	MAX_LOG(max5, max6);
	MAX_LOG(max7, max8);
	MAX_LOG(max1, max3);
	MAX_LOG(max5, max7);
	MAX_LOG(max1, max5);
	max[0] = max1;
#endif

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMaximum_F32(
	mlib_f32 *max,
	const mlib_f32 *x,
	mlib_s32 n)
{
	COMPUTE(mlib_f32,
		F32);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMaximum_D64(
	mlib_d64 *max,
	const mlib_d64 *x,
	mlib_s32 n)
{
	COMPUTE(mlib_d64,
		D64);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMaximumMag_S32C(
	mlib_s32 *max,
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
__mlib_VectorMaximumMag_F32C(
	mlib_f32 *max,
	const mlib_f32 *x,
	mlib_s32 n)
{
	COMPUTE_C(mlib_f32,
		F32);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMaximumMag_D64C(
	mlib_d64 *max,
	const mlib_d64 *x,
	mlib_s32 n)
{
	COMPUTE_C(mlib_d64,
		D64);
}

/* *********************************************************** */
