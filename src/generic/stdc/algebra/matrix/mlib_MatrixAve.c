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

#pragma ident	"@(#)mlib_MatrixAve.c	9.5	07/10/08 SMI"

/*
 * FUNCTIONS
 *      mlib_MatrixAve_[U8_U8|S8_S8],
 *      mlib_MatrixAve_S16_[U8|S8],
 *      mlib_MatrixAve_[U8|S8] - average of signed/unsigned
 *                               8-bit format element matrices
 *
 *      mlib_MatrixAve_[S16|S32]_S16,
 *      mlib_MatrixAve_S16 - average of signed 16-bit format element matrices
 *
 *      mlib_MatrixAve_S32_S32,
 *      mlib_MatrixAve_S32 - average of signed 32-bit format element matrices
 *
 * SYNOPSIS
 *      mlib_status mlib_MatrixAve_U8_U8(mlib_u8       *z,
 *                                       const mlib_u8 *x,
 *                                       const mlib_u8 *y,
 *                                       mlib_s32      m,
 *                                       mlib_s32      n);
 *      mlib_status mlib_MatrixAve_S8_S8(mlib_s8       *z,
 *                                       const mlib_s8 *x,
 *                                       const mlib_s8 *y,
 *                                       mlib_s32      m,
 *                                       mlib_s32      n);
 *      mlib_status mlib_MatrixAve_S16_U8(mlib_s16      *z,
 *                                        const mlib_u8 *x,
 *                                        const mlib_u8 *y,
 *                                        mlib_s32      m,
 *                                        mlib_s32      n);
 *      mlib_status mlib_MatrixAve_S16_S8(mlib_s16      *z,
 *                                        const mlib_s8 *x,
 *                                        const mlib_s8 *y,
 *                                        mlib_s32      m,
 *                                        mlib_s32      n);
 *      mlib_status mlib_MatrixAve_S16_S16(mlib_s16       *z,
 *                                         const mlib_s16 *x,
 *                                         const mlib_s16 *y,
 *                                         mlib_s32       m,
 *                                         mlib_s32       n);
 *      mlib_status mlib_MatrixAve_S32_S16(mlib_s32       *z,
 *                                         const mlib_s16 *x,
 *                                         const mlib_s16 *y,
 *                                         mlib_s32       m,
 *                                         mlib_s32       n);
 *      mlib_status mlib_MatrixAve_S32_S32(mlib_s32       *z,
 *                                         const mlib_s32 *x,
 *                                         const mlib_s32 *y,
 *                                         mlib_s32       m,
 *                                         mlib_s32       n);
 *
 *      mlib_status mlib_MatrixAve_U8(mlib_u8       *xz,
 *                                    const mlib_u8 *y,
 *                                    mlib_s32      m,
 *                                    mlib_s32      n);
 *      mlib_status mlib_MatrixAve_S8(mlib_s8       *xz,
 *                                    const mlib_s8 *y,
 *                                    mlib_s32      m,
 *                                    mlib_s32      n);
 *      mlib_status mlib_MatrixAve_S16(mlib_s16       *xz,
 *                                     const mlib_s16 *y,
 *                                     mlib_s32       m,
 *                                     mlib_s32       n);
 *      mlib_status mlib_MatrixAve_S32(mlib_s32       *xz,
 *                                     const mlib_s32 *y,
 *                                     mlib_s32       m,
 *                                     mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result matrix.
 *           The matrix is in row major order
 *      xz   pointer to the first element of the first input and
 *           the output matrix. The matrix is in row major order
 *      x    pointer to the first element of the first matrix.
 *           The matrix is in row major order
 *      y    pointer to the first element of the second matrix.
 *           The matrix is in row major order
 *      m    number of rows in the matrices
 *      n    number of columns in the matrices
 *
 * DESCRIPTION
 *      z = (x + y + 1) / 2
 */

/*
 * FUNCTIONS
 *      mlib_MatrixAve_[U8C_U8C|S8C_S8C],
 *      mlib_MatrixAve_S16C_[U8C|S8C],
 *      mlib_MatrixAve_[U8C|S8C] - average of signed/unsigned
 *                                 8-bit format complex element matrices
 *
 *      mlib_MatrixAve_[S16C|S32C]_S16C,
 *      mlib_MatrixAve_S16C - average of signed 16-bit format complex
 *                            element matrices
 *
 *      mlib_MatrixAve_S32C_S32C,
 *      mlib_MatrixAve_S32C - average of signed 32-bit format complex
 *                            element matrices
 *
 * SYNOPSIS
 *      mlib_status mlib_MatrixAve_U8C_U8C(mlib_u8       *z,
 *                                         const mlib_u8 *x,
 *                                         const mlib_u8 *y,
 *                                         mlib_s32      m,
 *                                         mlib_s32      n);
 *      mlib_status mlib_MatrixAve_S8C_S8C(mlib_s8       *z,
 *                                         const mlib_s8 *x,
 *                                         const mlib_s8 *y,
 *                                         mlib_s32      m,
 *                                         mlib_s32      n);
 *      mlib_status mlib_MatrixAve_S16C_U8C(mlib_s16      *z,
 *                                          const mlib_u8 *x,
 *                                          const mlib_u8 *y,
 *                                          mlib_s32      m,
 *                                          mlib_s32      n);
 *      mlib_status mlib_MatrixAve_S16C_S8C(mlib_s16      *z,
 *                                          const mlib_s8 *x,
 *                                          const mlib_s8 *y,
 *                                          mlib_s32      m,
 *                                          mlib_s32      n);
 *      mlib_status mlib_MatrixAve_S16C_S16C(mlib_s16       *z,
 *                                           const mlib_s16 *x,
 *                                           const mlib_s16 *y,
 *                                           mlib_s32       m,
 *                                           mlib_s32       n);
 *      mlib_status mlib_MatrixAve_S32C_S16C(mlib_s32       *z,
 *                                           const mlib_s16 *x,
 *                                           const mlib_s16 *y,
 *                                           mlib_s32       m,
 *                                           mlib_s32       n);
 *      mlib_status mlib_MatrixAve_S32C_S32C(mlib_s32       *z,
 *                                           const mlib_s32 *x,
 *                                           const mlib_s32 *y,
 *                                           mlib_s32       m,
 *                                           mlib_s32       n);
 *
 *      mlib_status mlib_MatrixAve_U8C(mlib_u8       *xz,
 *                                     const mlib_u8 *y,
 *                                     mlib_s32      m,
 *                                     mlib_s32      n);
 *      mlib_status mlib_MatrixAve_S8C(mlib_s8       *xz,
 *                                     const mlib_s8 *y,
 *                                     mlib_s32      m,
 *                                     mlib_s32      n);
 *      mlib_status mlib_MatrixAve_S16C(mlib_s16       *xz,
 *                                      const mlib_s16 *y,
 *                                      mlib_s32       m,
 *                                      mlib_s32       n);
 *      mlib_status mlib_MatrixAve_S32C(mlib_s32       *xz,
 *                                      const mlib_s32 *y,
 *                                      mlib_s32       m,
 *                                      mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result matrix.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts. The matrix is in row major order
 *      xz   pointer to the first element of the first input and
 *           the output matrix.
 *           xz[2*i] hold the real parts, and xz[2*i + 1] hold the imaginary
 *           parts.  The matrix is in row major order
 *      x    pointer to the first complex element of the first matrix.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts. The matrix is in row major order
 *      y    pointer to the first complex element of the second matrix.
 *           y[2*i] hold the real parts, and y[2*i + 1] hold the imaginary
 *           parts. The matrix is in row major order
 *      m    number of rows in the matrix
 *      n    number of columns in the matrix
 *
 * DESCRIPTION
 *      z = (x + y + 1) / 2
 */

/* *********************************************************** */

#include <mlib_algebra.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_MatrixAve_S8C = __mlib_MatrixAve_S8C
#pragma weak mlib_MatrixAve_U8C = __mlib_MatrixAve_U8C
#pragma weak mlib_MatrixAve_S8_S8 = __mlib_MatrixAve_S8_S8
#pragma weak mlib_MatrixAve_S32 = __mlib_MatrixAve_S32
#pragma weak mlib_MatrixAve_U8_U8 = __mlib_MatrixAve_U8_U8
#pragma weak mlib_MatrixAve_S16 = __mlib_MatrixAve_S16
#pragma weak mlib_MatrixAve_S32C = __mlib_MatrixAve_S32C
#pragma weak mlib_MatrixAve_S8C_S8C = __mlib_MatrixAve_S8C_S8C
#pragma weak mlib_MatrixAve_S16C = __mlib_MatrixAve_S16C
#pragma weak mlib_MatrixAve_U8C_U8C = __mlib_MatrixAve_U8C_U8C
#pragma weak mlib_MatrixAve_S16_S8 = __mlib_MatrixAve_S16_S8
#pragma weak mlib_MatrixAve_S16_U8 = __mlib_MatrixAve_S16_U8
#pragma weak mlib_MatrixAve_S16C_S8C = __mlib_MatrixAve_S16C_S8C
#pragma weak mlib_MatrixAve_S8 = __mlib_MatrixAve_S8
#pragma weak mlib_MatrixAve_S16C_U8C = __mlib_MatrixAve_S16C_U8C
#pragma weak mlib_MatrixAve_U8 = __mlib_MatrixAve_U8
#pragma weak mlib_MatrixAve_S32_S32 = __mlib_MatrixAve_S32_S32
#pragma weak mlib_MatrixAve_S32_S16 = __mlib_MatrixAve_S32_S16
#pragma weak mlib_MatrixAve_S16_S16 = __mlib_MatrixAve_S16_S16
#pragma weak mlib_MatrixAve_S32C_S32C = __mlib_MatrixAve_S32C_S32C
#pragma weak mlib_MatrixAve_S32C_S16C = __mlib_MatrixAve_S32C_S16C
#pragma weak mlib_MatrixAve_S16C_S16C = __mlib_MatrixAve_S16C_S16C

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_MatrixAve_S8C) mlib_MatrixAve_S8C
	__attribute__((weak, alias("__mlib_MatrixAve_S8C")));
__typeof__(__mlib_MatrixAve_U8C) mlib_MatrixAve_U8C
	__attribute__((weak, alias("__mlib_MatrixAve_U8C")));
__typeof__(__mlib_MatrixAve_S8_S8) mlib_MatrixAve_S8_S8
	__attribute__((weak, alias("__mlib_MatrixAve_S8_S8")));
__typeof__(__mlib_MatrixAve_S32) mlib_MatrixAve_S32
	__attribute__((weak, alias("__mlib_MatrixAve_S32")));
__typeof__(__mlib_MatrixAve_U8_U8) mlib_MatrixAve_U8_U8
	__attribute__((weak, alias("__mlib_MatrixAve_U8_U8")));
__typeof__(__mlib_MatrixAve_S16) mlib_MatrixAve_S16
	__attribute__((weak, alias("__mlib_MatrixAve_S16")));
__typeof__(__mlib_MatrixAve_S32C) mlib_MatrixAve_S32C
	__attribute__((weak, alias("__mlib_MatrixAve_S32C")));
__typeof__(__mlib_MatrixAve_S8C_S8C) mlib_MatrixAve_S8C_S8C
	__attribute__((weak, alias("__mlib_MatrixAve_S8C_S8C")));
__typeof__(__mlib_MatrixAve_S16C) mlib_MatrixAve_S16C
	__attribute__((weak, alias("__mlib_MatrixAve_S16C")));
__typeof__(__mlib_MatrixAve_U8C_U8C) mlib_MatrixAve_U8C_U8C
	__attribute__((weak, alias("__mlib_MatrixAve_U8C_U8C")));
__typeof__(__mlib_MatrixAve_S16_S8) mlib_MatrixAve_S16_S8
	__attribute__((weak, alias("__mlib_MatrixAve_S16_S8")));
__typeof__(__mlib_MatrixAve_S16_U8) mlib_MatrixAve_S16_U8
	__attribute__((weak, alias("__mlib_MatrixAve_S16_U8")));
__typeof__(__mlib_MatrixAve_S16C_S8C) mlib_MatrixAve_S16C_S8C
	__attribute__((weak, alias("__mlib_MatrixAve_S16C_S8C")));
__typeof__(__mlib_MatrixAve_S8) mlib_MatrixAve_S8
	__attribute__((weak, alias("__mlib_MatrixAve_S8")));
__typeof__(__mlib_MatrixAve_S16C_U8C) mlib_MatrixAve_S16C_U8C
	__attribute__((weak, alias("__mlib_MatrixAve_S16C_U8C")));
__typeof__(__mlib_MatrixAve_U8) mlib_MatrixAve_U8
	__attribute__((weak, alias("__mlib_MatrixAve_U8")));
__typeof__(__mlib_MatrixAve_S32_S32) mlib_MatrixAve_S32_S32
	__attribute__((weak, alias("__mlib_MatrixAve_S32_S32")));
__typeof__(__mlib_MatrixAve_S32_S16) mlib_MatrixAve_S32_S16
	__attribute__((weak, alias("__mlib_MatrixAve_S32_S16")));
__typeof__(__mlib_MatrixAve_S16_S16) mlib_MatrixAve_S16_S16
	__attribute__((weak, alias("__mlib_MatrixAve_S16_S16")));
__typeof__(__mlib_MatrixAve_S32C_S32C) mlib_MatrixAve_S32C_S32C
	__attribute__((weak, alias("__mlib_MatrixAve_S32C_S32C")));
__typeof__(__mlib_MatrixAve_S32C_S16C) mlib_MatrixAve_S32C_S16C
	__attribute__((weak, alias("__mlib_MatrixAve_S32C_S16C")));
__typeof__(__mlib_MatrixAve_S16C_S16C) mlib_MatrixAve_S16C_S16C
	__attribute__((weak, alias("__mlib_MatrixAve_S16C_S16C")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_MatrixAve_U8_U8(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m < 1) {
		return (MLIB_FAILURE);
	}
	return (__mlib_VectorAve_U8_U8(z, x, y, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAve_S16_U8(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m < 1) {
		return (MLIB_FAILURE);
	}
	return (__mlib_VectorAve_S16_U8(z, x, y, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAve_S8_S8(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m < 1) {
		return (MLIB_FAILURE);
	}
	return (__mlib_VectorAve_S8_S8(z, x, y, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAve_S16_S8(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m < 1) {
		return (MLIB_FAILURE);
	}
	return (__mlib_VectorAve_S16_S8(z, x, y, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAve_S16_S16(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m < 1) {
		return (MLIB_FAILURE);
	}
	return (__mlib_VectorAve_S16_S16(z, x, y, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAve_S32_S16(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m < 1) {
		return (MLIB_FAILURE);
	}
	return (__mlib_VectorAve_S32_S16(z, x, y, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAve_S32_S32(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m < 1) {
		return (MLIB_FAILURE);
	}
	return (__mlib_VectorAve_S32_S32(z, x, y, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAve_U8(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m < 1) {
		return (MLIB_FAILURE);
	}
	return (__mlib_VectorAve_U8_U8(xz, xz, y, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAve_S8(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m < 1) {
		return (MLIB_FAILURE);
	}
	return (__mlib_VectorAve_S8_S8(xz, xz, y, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAve_S16(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m < 1) {
		return (MLIB_FAILURE);
	}
	return (__mlib_VectorAve_S16_S16(xz, xz, y, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAve_S32(
	mlib_s32 *xz,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m < 1) {
		return (MLIB_FAILURE);
	}
	return (__mlib_VectorAve_S32_S32(xz, xz, y, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAve_U8C_U8C(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m < 1) {
		return (MLIB_FAILURE);
	}
	return (__mlib_VectorAve_U8_U8(z, x, y, 2 * m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAve_S16C_U8C(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m < 1) {
		return (MLIB_FAILURE);
	}
	return (__mlib_VectorAve_S16_U8(z, x, y, 2 * m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAve_S8C_S8C(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m < 1) {
		return (MLIB_FAILURE);
	}
	return (__mlib_VectorAve_S8_S8(z, x, y, 2 * m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAve_S16C_S8C(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m < 1) {
		return (MLIB_FAILURE);
	}
	return (__mlib_VectorAve_S16_S8(z, x, y, 2 * m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAve_S16C_S16C(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m < 1) {
		return (MLIB_FAILURE);
	}
	return (__mlib_VectorAve_S16_S16(z, x, y, 2 * m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAve_S32C_S16C(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m < 1) {
		return (MLIB_FAILURE);
	}
	return (__mlib_VectorAve_S32_S16(z, x, y, 2 * m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAve_S32C_S32C(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m < 1) {
		return (MLIB_FAILURE);
	}
	return (__mlib_VectorAve_S32_S32(z, x, y, 2 * m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAve_U8C(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m < 1) {
		return (MLIB_FAILURE);
	}
	return (__mlib_VectorAve_U8_U8(xz, xz, y, 2 * m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAve_S8C(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m < 1) {
		return (MLIB_FAILURE);
	}
	return (__mlib_VectorAve_S8_S8(xz, xz, y, 2 * m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAve_S16C(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m < 1) {
		return (MLIB_FAILURE);
	}
	return (__mlib_VectorAve_S16_S16(xz, xz, y, 2 * m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAve_S32C(
	mlib_s32 *xz,
	const mlib_s32 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	if (m < 1) {
		return (MLIB_FAILURE);
	}
	return (__mlib_VectorAve_S32_S32(xz, xz, y, 2 * m * n));
}

/* *********************************************************** */
