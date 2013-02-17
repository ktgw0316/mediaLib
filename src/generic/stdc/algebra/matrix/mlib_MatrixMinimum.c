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

#pragma ident	"@(#)mlib_MatrixMinimum.c	9.2	07/10/08 SMI"

/*
 * FUNCTIONS
 *      mlib_MatrixMinimum - find the minimum
 *
 * SYNOPSIS
 *      mlib_status mlib_MatrixMinimum_U8(mlib_u8       *min,
 *                                        const mlib_u8 *x,
 *                                        mlib_s32      m,
 *                                        mlib_s32      n);
 *      mlib_status mlib_MatrixMinimum_S8(mlib_s8       *min,
 *                                        const mlib_s8 *x,
 *                                        mlib_s32      m,
 *                                        mlib_s32      n);
 *      mlib_status mlib_MatrixMinimum_S16(mlib_s16       *min,
 *                                         const mlib_s16 *x,
 *                                         mlib_s32       m,
 *                                         mlib_s32       n);
 *      mlib_status mlib_MatrixMinimum_S32(mlib_s32       *min,
 *                                         const mlib_s32 *x,
 *                                         mlib_s32       m,
 *                                         mlib_s32       n);
 *      mlib_status mlib_MatrixMinimum_F32(mlib_f32       *min,
 *                                         const mlib_f32 *x,
 *                                         mlib_s32       m,
 *                                         mlib_s32       n);
 *      mlib_status mlib_MatrixMinimum_D64(mlib_d64       *min,
 *                                         const mlib_d64 *x,
 *                                         mlib_s32       m,
 *                                         mlib_s32       n);
 *
 * ARGUMENTS
 *      min    the pointer to minimum value
 *      x      the pointer to the first element of the source matrix
 *      m      the number of rows in the source matrix
 *      n      the number of columns in the source matrix
 *
 * DESCRIPTION
 *            m - 1 n - 1
 *      min = Min Min x[i][j]
 *            i = 0 j = 0
 */

/*
 * FUNCTIONS
 *      mlib_MatrixMinimumMag - find the minimum
 *
 * SYNOPSIS
 *      mlib_status mlib_MatrixMinimumMag_U8C(mlib_u8       *min,
 *                                            const mlib_u8 *x,
 *                                            mlib_s32      m,
 *                                            mlib_s32      n);
 *      mlib_status mlib_MatrixMinimumMag_S8C(mlib_s8       *min,
 *                                            const mlib_s8 *x,
 *                                            mlib_s32      m,
 *                                            mlib_s32      n);
 *      mlib_status mlib_MatrixMinimumMag_S16C(mlib_s16       *min,
 *                                             const mlib_s16 *x,
 *                                             mlib_s32       m,
 *                                             mlib_s32       n);
 *      mlib_status mlib_MatrixMinimumMag_S32C(mlib_s32       *min,
 *                                             const mlib_s32 *x,
 *                                             mlib_s32       m,
 *                                             mlib_s32       n);
 *      mlib_status mlib_MatrixMinimumMag_F32C(mlib_f32       *min,
 *                                             const mlib_f32 *x,
 *                                             mlib_s32       m,
 *                                             mlib_s32       n);
 *      mlib_status mlib_MatrixMinimumMag_D64C(mlib_d64       *min,
 *                                             const mlib_d64 *x,
 *                                             mlib_s32       m,
 *                                             mlib_s32       n);
 *
 * ARGUMENTS
 *      min    the pointer to minimum value
 *      x      the pointer to the first element of the source matrix
 *      m      the number of rows in the source matrix
 *      n      the number of columns in the source matrix
 *
 * DESCRIPTION
 *      This function finds the first element with the minimum
 *      magnitude, then puts the real and imaginary parts of it into
 *      min[0] and min[1], respectively.
 *
 *            m - 1 n - 1
 *      min = Min Min x[i][j]
 *            i = 0 j = 0
 */

#include <mlib_algebra.h>
#include <mlib_AlgebraUtil.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_MatrixMinimum_S8 = __mlib_MatrixMinimum_S8
#pragma weak mlib_MatrixMinimum_U8 = __mlib_MatrixMinimum_U8
#pragma weak mlib_MatrixMinimum_S16 = __mlib_MatrixMinimum_S16
#pragma weak mlib_MatrixMinimum_S32 = __mlib_MatrixMinimum_S32
#pragma weak mlib_MatrixMinimum_F32 = __mlib_MatrixMinimum_F32
#pragma weak mlib_MatrixMinimum_D64 = __mlib_MatrixMinimum_D64
#pragma weak mlib_MatrixMinimumMag_S8C = __mlib_MatrixMinimumMag_S8C
#pragma weak mlib_MatrixMinimumMag_U8C = __mlib_MatrixMinimumMag_U8C
#pragma weak mlib_MatrixMinimumMag_S16C = __mlib_MatrixMinimumMag_S16C
#pragma weak mlib_MatrixMinimumMag_S32C = __mlib_MatrixMinimumMag_S32C
#pragma weak mlib_MatrixMinimumMag_F32C = __mlib_MatrixMinimumMag_F32C
#pragma weak mlib_MatrixMinimumMag_D64C = __mlib_MatrixMinimumMag_D64C

#elif defined(__GNUC__)

__typeof__(__mlib_MatrixMinimum_S8) mlib_MatrixMinimum_S8
	__attribute__((weak, alias("__mlib_MatrixMinimum_S8")));
__typeof__(__mlib_MatrixMinimum_U8) mlib_MatrixMinimum_U8
	__attribute__((weak, alias("__mlib_MatrixMinimum_U8")));
__typeof__(__mlib_MatrixMinimum_S16) mlib_MatrixMinimum_S16
	__attribute__((weak, alias("__mlib_MatrixMinimum_S16")));
__typeof__(__mlib_MatrixMinimum_S32) mlib_MatrixMinimum_S32
	__attribute__((weak, alias("__mlib_MatrixMinimum_S32")));
__typeof__(__mlib_MatrixMinimum_F32) mlib_MatrixMinimum_F32
	__attribute__((weak, alias("__mlib_MatrixMinimum_F32")));
__typeof__(__mlib_MatrixMinimum_D64) mlib_MatrixMinimum_D64
	__attribute__((weak, alias("__mlib_MatrixMinimum_D64")));
__typeof__(__mlib_MatrixMinimumMag_S8C) mlib_MatrixMinimumMag_S8C
	__attribute__((weak, alias("__mlib_MatrixMinimumMag_S8C")));
__typeof__(__mlib_MatrixMinimumMag_U8C) mlib_MatrixMinimumMag_U8C
	__attribute__((weak, alias("__mlib_MatrixMinimumMag_U8C")));
__typeof__(__mlib_MatrixMinimumMag_S16C) mlib_MatrixMinimumMag_S16C
	__attribute__((weak, alias("__mlib_MatrixMinimumMag_S16C")));
__typeof__(__mlib_MatrixMinimumMag_S32C) mlib_MatrixMinimumMag_S32C
	__attribute__((weak, alias("__mlib_MatrixMinimumMag_S32C")));
__typeof__(__mlib_MatrixMinimumMag_F32C) mlib_MatrixMinimumMag_F32C
	__attribute__((weak, alias("__mlib_MatrixMinimumMag_F32C")));
__typeof__(__mlib_MatrixMinimumMag_D64C) mlib_MatrixMinimumMag_D64C
	__attribute__((weak, alias("__mlib_MatrixMinimumMag_D64C")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_MatrixMinimum_U8(
	mlib_u8 *min,
	const mlib_u8 *x,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m <= 0)
		return (MLIB_FAILURE);

	return (__mlib_VectorMinimum_U8(min, x, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMinimum_S8(
	mlib_s8 *min,
	const mlib_s8 *x,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m <= 0)
		return (MLIB_FAILURE);

	return (__mlib_VectorMinimum_S8(min, x, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMinimum_S16(
	mlib_s16 *min,
	const mlib_s16 *x,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m <= 0)
		return (MLIB_FAILURE);

	return (__mlib_VectorMinimum_S16(min, x, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMinimum_S32(
	mlib_s32 *min,
	const mlib_s32 *x,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m <= 0)
		return (MLIB_FAILURE);

	return (__mlib_VectorMinimum_S32(min, x, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMinimum_F32(
	mlib_f32 *min,
	const mlib_f32 *x,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m <= 0)
		return (MLIB_FAILURE);

	return (__mlib_VectorMinimum_F32(min, x, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMinimum_D64(
	mlib_d64 *min,
	const mlib_d64 *x,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m <= 0)
		return (MLIB_FAILURE);

	return (__mlib_VectorMinimum_D64(min, x, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMinimumMag_U8C(
	mlib_u8 *min,
	const mlib_u8 *x,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m <= 0)
		return (MLIB_FAILURE);

	return (__mlib_VectorMinimumMag_U8C(min, x, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMinimumMag_S8C(
	mlib_s8 *min,
	const mlib_s8 *x,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m <= 0)
		return (MLIB_FAILURE);

	return (__mlib_VectorMinimumMag_S8C(min, x, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMinimumMag_S16C(
	mlib_s16 *min,
	const mlib_s16 *x,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m <= 0)
		return (MLIB_FAILURE);

	return (__mlib_VectorMinimumMag_S16C(min, x, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMinimumMag_S32C(
	mlib_s32 *min,
	const mlib_s32 *x,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m <= 0)
		return (MLIB_FAILURE);

	return (__mlib_VectorMinimumMag_S32C(min, x, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMinimumMag_F32C(
	mlib_f32 *min,
	const mlib_f32 *x,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m <= 0)
		return (MLIB_FAILURE);

	return (__mlib_VectorMinimumMag_F32C(min, x, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMinimumMag_D64C(
	mlib_d64 *min,
	const mlib_d64 *x,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m <= 0)
		return (MLIB_FAILURE);

	return (__mlib_VectorMinimumMag_D64C(min, x, m * n));
}

/* *********************************************************** */
