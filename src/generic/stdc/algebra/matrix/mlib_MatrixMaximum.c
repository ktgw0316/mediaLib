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

#pragma ident	"@(#)mlib_MatrixMaximum.c	9.2	07/10/08 SMI"

/*
 * FUNCTIONS
 *      mlib_MatrixMaximum - find the maximum
 *
 * SYNOPSIS
 *      mlib_status mlib_MatrixMaximum_U8(mlib_u8       *max,
 *                                        const mlib_u8 *x,
 *                                        mlib_s32      m,
 *                                        mlib_s32      n);
 *      mlib_status mlib_MatrixMaximum_S8(mlib_s8       *max,
 *                                        const mlib_s8 *x,
 *                                        mlib_s32      m,
 *                                        mlib_s32      n);
 *      mlib_status mlib_MatrixMaximum_S16(mlib_s16       *max,
 *                                         const mlib_s16 *x,
 *                                         mlib_s32       m,
 *                                         mlib_s32       n);
 *      mlib_status mlib_MatrixMaximum_S32(mlib_s32       *max,
 *                                         const mlib_s32 *x,
 *                                         mlib_s32       m,
 *                                         mlib_s32       n);
 *      mlib_status mlib_MatrixMaximum_F32(mlib_f32       *max,
 *                                         const mlib_f32 *x,
 *                                         mlib_s32       m,
 *                                         mlib_s32       n);
 *      mlib_status mlib_MatrixMaximum_D64(mlib_d64       *max,
 *                                         const mlib_d64 *x,
 *                                         mlib_s32       m,
 *                                         mlib_s32       n);
 *
 * ARGUMENTS
 *      max    the pointer to maximum value
 *      x      the pointer to the first element of the source matrix
 *      m      the number of rows in the source matrix
 *      n      the number of columns in the source matrix
 *
 * DESCRIPTION
 *            m - 1 n - 1
 *      max = Max Max x[i][j]
 *            i = 0 j = 0
 */

/*
 * FUNCTIONS
 *      mlib_MatrixMaximumMag - find the maximum
 *
 * SYNOPSIS
 *      mlib_status mlib_MatrixMaximumMag_U8C(mlib_u8       *max,
 *                                            const mlib_u8 *x,
 *                                            mlib_s32      m,
 *                                            mlib_s32      n);
 *      mlib_status mlib_MatrixMaximumMag_S8C(mlib_s8       *max,
 *                                            const mlib_s8 *x,
 *                                            mlib_s32      m,
 *                                            mlib_s32      n);
 *      mlib_status mlib_MatrixMaximumMag_S16C(mlib_s16       *max,
 *                                             const mlib_s16 *x,
 *                                             mlib_s32       m,
 *                                             mlib_s32       n);
 *      mlib_status mlib_MatrixMaximumMag_S32C(mlib_s32       *max,
 *                                             const mlib_s32 *x,
 *                                             mlib_s32       m,
 *                                             mlib_s32       n);
 *      mlib_status mlib_MatrixMaximumMag_F32C(mlib_f32       *max,
 *                                             const mlib_f32 *x,
 *                                             mlib_s32       m,
 *                                             mlib_s32       n);
 *      mlib_status mlib_MatrixMaximumMag_D64C(mlib_d64       *max,
 *                                             const mlib_d64 *x,
 *                                             mlib_s32       m,
 *                                             mlib_s32       n);
 *
 * ARGUMENTS
 *      max    the pointer to maximum value
 *      x      the pointer to the first element of the source matrix
 *      m      the number of rows in the source matrix
 *      n      the number of columns in the source matrix
 *
 * DESCRIPTION
 *      This function finds the first element with the maximum
 *      magnitude, then puts the real and imaginary parts of it into
 *      max[0] and max[1], respectively.
 *
 *            m - 1 n - 1
 *      max = Max Max x[i][j]
 *            i = 0 j = 0
 */

#include <mlib_algebra.h>
#include <mlib_AlgebraUtil.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_MatrixMaximum_S8 = __mlib_MatrixMaximum_S8
#pragma weak mlib_MatrixMaximum_U8 = __mlib_MatrixMaximum_U8
#pragma weak mlib_MatrixMaximum_S16 = __mlib_MatrixMaximum_S16
#pragma weak mlib_MatrixMaximum_S32 = __mlib_MatrixMaximum_S32
#pragma weak mlib_MatrixMaximum_F32 = __mlib_MatrixMaximum_F32
#pragma weak mlib_MatrixMaximum_D64 = __mlib_MatrixMaximum_D64
#pragma weak mlib_MatrixMaximumMag_S8C = __mlib_MatrixMaximumMag_S8C
#pragma weak mlib_MatrixMaximumMag_U8C = __mlib_MatrixMaximumMag_U8C
#pragma weak mlib_MatrixMaximumMag_S16C = __mlib_MatrixMaximumMag_S16C
#pragma weak mlib_MatrixMaximumMag_S32C = __mlib_MatrixMaximumMag_S32C
#pragma weak mlib_MatrixMaximumMag_F32C = __mlib_MatrixMaximumMag_F32C
#pragma weak mlib_MatrixMaximumMag_D64C = __mlib_MatrixMaximumMag_D64C

#elif defined(__GNUC__)

__typeof__(__mlib_MatrixMaximum_S8) mlib_MatrixMaximum_S8
	__attribute__((weak, alias("__mlib_MatrixMaximum_S8")));
__typeof__(__mlib_MatrixMaximum_U8) mlib_MatrixMaximum_U8
	__attribute__((weak, alias("__mlib_MatrixMaximum_U8")));
__typeof__(__mlib_MatrixMaximum_S16) mlib_MatrixMaximum_S16
	__attribute__((weak, alias("__mlib_MatrixMaximum_S16")));
__typeof__(__mlib_MatrixMaximum_S32) mlib_MatrixMaximum_S32
	__attribute__((weak, alias("__mlib_MatrixMaximum_S32")));
__typeof__(__mlib_MatrixMaximum_F32) mlib_MatrixMaximum_F32
	__attribute__((weak, alias("__mlib_MatrixMaximum_F32")));
__typeof__(__mlib_MatrixMaximum_D64) mlib_MatrixMaximum_D64
	__attribute__((weak, alias("__mlib_MatrixMaximum_D64")));
__typeof__(__mlib_MatrixMaximumMag_S8C) mlib_MatrixMaximumMag_S8C
	__attribute__((weak, alias("__mlib_MatrixMaximumMag_S8C")));
__typeof__(__mlib_MatrixMaximumMag_U8C) mlib_MatrixMaximumMag_U8C
	__attribute__((weak, alias("__mlib_MatrixMaximumMag_U8C")));
__typeof__(__mlib_MatrixMaximumMag_S16C) mlib_MatrixMaximumMag_S16C
	__attribute__((weak, alias("__mlib_MatrixMaximumMag_S16C")));
__typeof__(__mlib_MatrixMaximumMag_S32C) mlib_MatrixMaximumMag_S32C
	__attribute__((weak, alias("__mlib_MatrixMaximumMag_S32C")));
__typeof__(__mlib_MatrixMaximumMag_F32C) mlib_MatrixMaximumMag_F32C
	__attribute__((weak, alias("__mlib_MatrixMaximumMag_F32C")));
__typeof__(__mlib_MatrixMaximumMag_D64C) mlib_MatrixMaximumMag_D64C
	__attribute__((weak, alias("__mlib_MatrixMaximumMag_D64C")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_MatrixMaximum_U8(
	mlib_u8 *max,
	const mlib_u8 *x,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m <= 0)
		return (MLIB_FAILURE);

	return (__mlib_VectorMaximum_U8(max, x, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMaximum_S8(
	mlib_s8 *max,
	const mlib_s8 *x,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m <= 0)
		return (MLIB_FAILURE);

	return (__mlib_VectorMaximum_S8(max, x, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMaximum_S16(
	mlib_s16 *max,
	const mlib_s16 *x,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m <= 0)
		return (MLIB_FAILURE);

	return (__mlib_VectorMaximum_S16(max, x, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMaximum_S32(
	mlib_s32 *max,
	const mlib_s32 *x,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m <= 0)
		return (MLIB_FAILURE);

	return (__mlib_VectorMaximum_S32(max, x, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMaximum_F32(
	mlib_f32 *max,
	const mlib_f32 *x,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m <= 0)
		return (MLIB_FAILURE);

	return (__mlib_VectorMaximum_F32(max, x, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMaximum_D64(
	mlib_d64 *max,
	const mlib_d64 *x,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m <= 0)
		return (MLIB_FAILURE);

	return (__mlib_VectorMaximum_D64(max, x, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMaximumMag_U8C(
	mlib_u8 *max,
	const mlib_u8 *x,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m <= 0)
		return (MLIB_FAILURE);

	return (__mlib_VectorMaximumMag_U8C(max, x, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMaximumMag_S8C(
	mlib_s8 *max,
	const mlib_s8 *x,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m <= 0)
		return (MLIB_FAILURE);

	return (__mlib_VectorMaximumMag_S8C(max, x, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMaximumMag_S16C(
	mlib_s16 *max,
	const mlib_s16 *x,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m <= 0)
		return (MLIB_FAILURE);

	return (__mlib_VectorMaximumMag_S16C(max, x, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMaximumMag_S32C(
	mlib_s32 *max,
	const mlib_s32 *x,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m <= 0)
		return (MLIB_FAILURE);

	return (__mlib_VectorMaximumMag_S32C(max, x, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMaximumMag_F32C(
	mlib_f32 *max,
	const mlib_f32 *x,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m <= 0)
		return (MLIB_FAILURE);

	return (__mlib_VectorMaximumMag_F32C(max, x, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMaximumMag_D64C(
	mlib_d64 *max,
	const mlib_d64 *x,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m <= 0)
		return (MLIB_FAILURE);

	return (__mlib_VectorMaximumMag_D64C(max, x, m * n));
}

/* *********************************************************** */
