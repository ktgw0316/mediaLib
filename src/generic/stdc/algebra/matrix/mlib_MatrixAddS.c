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

#pragma ident	"@(#)mlib_MatrixAddS.c	9.3	07/10/08 SMI"

/*
 * FUNCTIONS
 *      mlib_MatrixAddS_U8_[U8|S16]_[Sat|Mod]   - addition of unsigned 8-bit
 *                                                format element matrix
 *                                                to a scalar
 *      mlib_MatrixAddS_S8_[S8|S16]_[Sat|Mod]   - addition of signed 8-bit
 *                                                format element matrix
 *                                                to a scalar
 *      mlib_MatrixAddS_S16_[S16|S32]_[Sat|Mod] - addition of signed 16-bit
 *                                                format element matrix
 *                                                to a scalar
 *      mlib_MatrixAddS_S32_S32_[Sat|Mod]       - addition of signed 32-bit
 *                                                format element matrix
 *                                                to a scalar
 *
 * SYNOPSIS
 *      mlib_status mlib_MatrixAddS_S16_[Sat|Mod](mlib_s16       * xz,
 *                                                const mlib_s16 * c,
 *                                                mlib_s32       m,
 *                                                mlib_s32       n);
 *      mlib_status mlib_MatrixAddS_S16_S16_[Sat|Mod](mlib_s16       * z,
 *                                                    const mlib_s16 * x,
 *                                                    const mlib_s16 * c,
 *                                                    mlib_s32       m,
 *                                                    mlib_s32       n);
 *      mlib_status mlib_MatrixAddS_S32_[Sat|Mod](mlib_s32       * xz,
 *                                                const mlib_s32 * c,
 *                                                mlib_s32       m,
 *                                                mlib_s32       n);
 *      mlib_status mlib_MatrixAddS_S32_S32_[Sat|Mod](mlib_s32       * z,
 *                                                    const mlib_s32 * x,
 *                                                    const mlib_s32 * c,
 *                                                    mlib_s32       m,
 *                                                    mlib_s32       n);
 *      mlib_status mlib_MatrixAddS_S8_[Sat|Mod](mlib_s8       * xz,
 *                                               const mlib_s8 * c,
 *                                               mlib_s32      m,
 *                                               mlib_s32      n);
 *      mlib_status mlib_MatrixAddS_S8_S8_[Sat|Mod](mlib_s8       * z,
 *                                                  const mlib_s8 * x,
 *                                                  const mlib_s8 * c,
 *                                                  mlib_s32      m,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_MatrixAddS_U8_[Sat|Mod](mlib_u8       * xz,
 *                                               const mlib_u8 * c,
 *                                               mlib_s32      m,
 *                                               mlib_s32      n);
 *      mlib_status mlib_MatrixAddS_U8_U8_[Sat|Mod](mlib_u8       * z,
 *                                                  const mlib_u8 * x,
 *                                                  const mlib_u8 * c,
 *                                                  mlib_s32      m,
 *                                                  mlib_s32      n);
 *
 *      mlib_status mlib_MatrixAddS_S16_S8_Mod(mlib_s16      * z,
 *                                             const mlib_s8 * x,
 *                                             const mlib_s8 * c,
 *                                             mlib_s32      m,
 *                                             mlib_s32      n);
 *      mlib_status mlib_MatrixAddS_S16_U8_Mod(mlib_s16      * z,
 *                                             const mlib_u8 * x,
 *                                             const mlib_u8 * c,
 *                                             mlib_s32      m,
 *                                             mlib_s32      n);
 *      mlib_status mlib_MatrixAddS_S32_S16_Mod(mlib_s32       * z,
 *                                              const mlib_s16 * x,
 *                                              const mlib_s16 * c,
 *                                              mlib_s32       m,
 *                                              mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result matrix.
 *           The matrix is in row major order
 *      x    pointer to the first element of the input matrix.
 *           The matrix is in row major order
 *      c    a scalar
 *      m    number of rows in the matrix
 *      n    number of columns in the matrix
 *
 * DESCRIPTION
 *      z = x + c
 */

/*
 * FUNCTIONS
 *      mlib_MatrixAddS_U8C_[U8C|S16C]_[Sat|Mod]   - addition of unsigned 8-bit
 *                                                   format complex element
 *                                                   matrix to a scalar
 *      mlib_MatrixAddS_S8C_[S8C|S16C]_[Sat|Mod]   - addition of signed 8-bit
 *                                                   format complex element
 *                                                   matrix to a scalar
 *      mlib_MatrixAddS_S16C_[S16C|S32C]_[Sat|Mod] - addition of signed 16-bit
 *                                                   format complex element
 *                                                   matrix to a scalar
 *      mlib_MatrixAddS_S32C_S32C_[Sat|Mod]        - addition of signed 32-bit
 *                                                   format complex element
 *                                                   matrix to a scalar
 *
 * SYNOPSIS
 *      mlib_status mlib_MatrixAddS_S16C_[Sat|Mod](mlib_s16       * xz,
 *                                                 const mlib_s16 * c,
 *                                                 mlib_s32       m,
 *                                                 mlib_s32       n);
 *      mlib_status mlib_MatrixAddS_S16C_S16C_[Sat|Mod](mlib_s16       * z,
 *                                                      const mlib_s16 * x,
 *                                                      const mlib_s16 * c,
 *                                                      mlib_s32       m,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_MatrixAddS_S32C_[Sat|Mod](mlib_s32       * xz,
 *                                                 const mlib_s32 * c,
 *                                                 mlib_s32       m,
 *                                                 mlib_s32       n);
 *      mlib_status mlib_MatrixAddS_S32C_S32C_[Sat|Mod](mlib_s32       * z,
 *                                                      const mlib_s32 * x,
 *                                                      const mlib_s32 * c,
 *                                                      mlib_s32       m,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_MatrixAddS_S8C_[Sat|Mod](mlib_s8       * xz,
 *                                                const mlib_s8 * c,
 *                                                mlib_s32      m,
 *                                                mlib_s32      n);
 *      mlib_status mlib_MatrixAddS_S8C_S8C_[Sat|Mod](mlib_s8       * z,
 *                                                    const mlib_s8 * x,
 *                                                    const mlib_s8 * c,
 *                                                    mlib_s32      m,
 *                                                    mlib_s32      n);
 *      mlib_status mlib_MatrixAddS_U8C_[Sat|Mod](mlib_u8       * xz,
 *                                                const mlib_u8 * c,
 *                                                mlib_s32      m,
 *                                                mlib_s32      n);
 *      mlib_status mlib_MatrixAddS_U8C_U8C_[Sat|Mod](mlib_u8       * z,
 *                                                    const mlib_u8 * x,
 *                                                    const mlib_u8 * c,
 *                                                    mlib_s32      m,
 *                                                    mlib_s32      n);
 *
 *      mlib_status mlib_MatrixAddS_S16C_S8C_Mod(mlib_s16      * z,
 *                                               const mlib_s8 * x,
 *                                               const mlib_s8 * c,
 *                                               mlib_s32      m,
 *                                               mlib_s32      n);
 *      mlib_status mlib_MatrixAddS_S16C_U8C_Mod(mlib_s16      * z,
 *                                               const mlib_u8 * x,
 *                                               const mlib_u8 * c,
 *                                               mlib_s32      m,
 *                                               mlib_s32      n);
 *      mlib_status mlib_MatrixAddS_S32C_S16C_Mod(mlib_s32       * z,
 *                                                const mlib_s16 * x,
 *                                                const mlib_s16 * c,
 *                                                mlib_s32       m,
 *                                                mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result matrix.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts. The matrix is in row major order
 *      x    pointer to the first complex element of the input matrix.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts. The matrix is in row major order
 *      c    pointer to a complex scalar. c[0] hold the real part,
 *           and c[1] hold the imaginary part
 *      m    number of rows in the matrix
 *      n    number of columns in the matrix
 *
 * DESCRIPTION
 *      z = x + c
 */

#include <mlib_algebra.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_MatrixAddS_S8C_Mod = __mlib_MatrixAddS_S8C_Mod
#pragma weak mlib_MatrixAddS_U8C_Mod = __mlib_MatrixAddS_U8C_Mod
#pragma weak mlib_MatrixAddS_S8_Sat = __mlib_MatrixAddS_S8_Sat
#pragma weak mlib_MatrixAddS_U8_Sat = __mlib_MatrixAddS_U8_Sat
#pragma weak mlib_MatrixAddS_S8_S8_Mod = __mlib_MatrixAddS_S8_S8_Mod
#pragma weak mlib_MatrixAddS_S8C_Sat = __mlib_MatrixAddS_S8C_Sat
#pragma weak mlib_MatrixAddS_U8C_Sat = __mlib_MatrixAddS_U8C_Sat
#pragma weak mlib_MatrixAddS_S32_Mod = __mlib_MatrixAddS_S32_Mod
#pragma weak mlib_MatrixAddS_U8_U8_Mod = __mlib_MatrixAddS_U8_U8_Mod
#pragma weak mlib_MatrixAddS_S16_Mod = __mlib_MatrixAddS_S16_Mod
#pragma weak mlib_MatrixAddS_S8C_S8C_Mod = __mlib_MatrixAddS_S8C_S8C_Mod
#pragma weak mlib_MatrixAddS_S8_S8_Sat = __mlib_MatrixAddS_S8_S8_Sat
#pragma weak mlib_MatrixAddS_U8C_U8C_Mod = __mlib_MatrixAddS_U8C_U8C_Mod
#pragma weak mlib_MatrixAddS_S32_Sat = __mlib_MatrixAddS_S32_Sat
#pragma weak mlib_MatrixAddS_U8_U8_Sat = __mlib_MatrixAddS_U8_U8_Sat
#pragma weak mlib_MatrixAddS_S16_Sat = __mlib_MatrixAddS_S16_Sat
#pragma weak mlib_MatrixAddS_S8C_S8C_Sat = __mlib_MatrixAddS_S8C_S8C_Sat
#pragma weak mlib_MatrixAddS_U8C_U8C_Sat = __mlib_MatrixAddS_U8C_U8C_Sat
#pragma weak mlib_MatrixAddS_S32_S32_Mod = __mlib_MatrixAddS_S32_S32_Mod
#pragma weak mlib_MatrixAddS_S32_S16_Mod = __mlib_MatrixAddS_S32_S16_Mod
#pragma weak mlib_MatrixAddS_S16_S16_Mod = __mlib_MatrixAddS_S16_S16_Mod
#pragma weak mlib_MatrixAddS_S32C_S32C_Mod = __mlib_MatrixAddS_S32C_S32C_Mod
#pragma weak mlib_MatrixAddS_S32_S32_Sat = __mlib_MatrixAddS_S32_S32_Sat
#pragma weak mlib_MatrixAddS_S32C_S16C_Mod = __mlib_MatrixAddS_S32C_S16C_Mod
#pragma weak mlib_MatrixAddS_S16C_S16C_Mod = __mlib_MatrixAddS_S16C_S16C_Mod
#pragma weak mlib_MatrixAddS_S32C_Mod = __mlib_MatrixAddS_S32C_Mod
#pragma weak mlib_MatrixAddS_S16_S16_Sat = __mlib_MatrixAddS_S16_S16_Sat
#pragma weak mlib_MatrixAddS_S16C_Mod = __mlib_MatrixAddS_S16C_Mod
#pragma weak mlib_MatrixAddS_S32C_S32C_Sat = __mlib_MatrixAddS_S32C_S32C_Sat
#pragma weak mlib_MatrixAddS_S16C_S16C_Sat = __mlib_MatrixAddS_S16C_S16C_Sat
#pragma weak mlib_MatrixAddS_S32C_Sat = __mlib_MatrixAddS_S32C_Sat
#pragma weak mlib_MatrixAddS_S16_S8_Mod = __mlib_MatrixAddS_S16_S8_Mod
#pragma weak mlib_MatrixAddS_S16C_Sat = __mlib_MatrixAddS_S16C_Sat
#pragma weak mlib_MatrixAddS_S16_U8_Mod = __mlib_MatrixAddS_S16_U8_Mod
#pragma weak mlib_MatrixAddS_S16C_S8C_Mod = __mlib_MatrixAddS_S16C_S8C_Mod
#pragma weak mlib_MatrixAddS_S8_Mod = __mlib_MatrixAddS_S8_Mod
#pragma weak mlib_MatrixAddS_S16C_U8C_Mod = __mlib_MatrixAddS_S16C_U8C_Mod
#pragma weak mlib_MatrixAddS_U8_Mod = __mlib_MatrixAddS_U8_Mod

#elif defined(__GNUC__)

__typeof__(__mlib_MatrixAddS_S8C_Mod) mlib_MatrixAddS_S8C_Mod
	__attribute__((weak, alias("__mlib_MatrixAddS_S8C_Mod")));
__typeof__(__mlib_MatrixAddS_U8C_Mod) mlib_MatrixAddS_U8C_Mod
	__attribute__((weak, alias("__mlib_MatrixAddS_U8C_Mod")));
__typeof__(__mlib_MatrixAddS_S8_Sat) mlib_MatrixAddS_S8_Sat
	__attribute__((weak, alias("__mlib_MatrixAddS_S8_Sat")));
__typeof__(__mlib_MatrixAddS_U8_Sat) mlib_MatrixAddS_U8_Sat
	__attribute__((weak, alias("__mlib_MatrixAddS_U8_Sat")));
__typeof__(__mlib_MatrixAddS_S8_S8_Mod) mlib_MatrixAddS_S8_S8_Mod
	__attribute__((weak, alias("__mlib_MatrixAddS_S8_S8_Mod")));
__typeof__(__mlib_MatrixAddS_S8C_Sat) mlib_MatrixAddS_S8C_Sat
	__attribute__((weak, alias("__mlib_MatrixAddS_S8C_Sat")));
__typeof__(__mlib_MatrixAddS_U8C_Sat) mlib_MatrixAddS_U8C_Sat
	__attribute__((weak, alias("__mlib_MatrixAddS_U8C_Sat")));
__typeof__(__mlib_MatrixAddS_S32_Mod) mlib_MatrixAddS_S32_Mod
	__attribute__((weak, alias("__mlib_MatrixAddS_S32_Mod")));
__typeof__(__mlib_MatrixAddS_U8_U8_Mod) mlib_MatrixAddS_U8_U8_Mod
	__attribute__((weak, alias("__mlib_MatrixAddS_U8_U8_Mod")));
__typeof__(__mlib_MatrixAddS_S16_Mod) mlib_MatrixAddS_S16_Mod
	__attribute__((weak, alias("__mlib_MatrixAddS_S16_Mod")));
__typeof__(__mlib_MatrixAddS_S8C_S8C_Mod) mlib_MatrixAddS_S8C_S8C_Mod
	__attribute__((weak, alias("__mlib_MatrixAddS_S8C_S8C_Mod")));
__typeof__(__mlib_MatrixAddS_S8_S8_Sat) mlib_MatrixAddS_S8_S8_Sat
	__attribute__((weak, alias("__mlib_MatrixAddS_S8_S8_Sat")));
__typeof__(__mlib_MatrixAddS_U8C_U8C_Mod) mlib_MatrixAddS_U8C_U8C_Mod
	__attribute__((weak, alias("__mlib_MatrixAddS_U8C_U8C_Mod")));
__typeof__(__mlib_MatrixAddS_S32_Sat) mlib_MatrixAddS_S32_Sat
	__attribute__((weak, alias("__mlib_MatrixAddS_S32_Sat")));
__typeof__(__mlib_MatrixAddS_U8_U8_Sat) mlib_MatrixAddS_U8_U8_Sat
	__attribute__((weak, alias("__mlib_MatrixAddS_U8_U8_Sat")));
__typeof__(__mlib_MatrixAddS_S16_Sat) mlib_MatrixAddS_S16_Sat
	__attribute__((weak, alias("__mlib_MatrixAddS_S16_Sat")));
__typeof__(__mlib_MatrixAddS_S8C_S8C_Sat) mlib_MatrixAddS_S8C_S8C_Sat
	__attribute__((weak, alias("__mlib_MatrixAddS_S8C_S8C_Sat")));
__typeof__(__mlib_MatrixAddS_U8C_U8C_Sat) mlib_MatrixAddS_U8C_U8C_Sat
	__attribute__((weak, alias("__mlib_MatrixAddS_U8C_U8C_Sat")));
__typeof__(__mlib_MatrixAddS_S32_S32_Mod) mlib_MatrixAddS_S32_S32_Mod
	__attribute__((weak, alias("__mlib_MatrixAddS_S32_S32_Mod")));
__typeof__(__mlib_MatrixAddS_S32_S16_Mod) mlib_MatrixAddS_S32_S16_Mod
	__attribute__((weak, alias("__mlib_MatrixAddS_S32_S16_Mod")));
__typeof__(__mlib_MatrixAddS_S16_S16_Mod) mlib_MatrixAddS_S16_S16_Mod
	__attribute__((weak, alias("__mlib_MatrixAddS_S16_S16_Mod")));
__typeof__(__mlib_MatrixAddS_S32C_S32C_Mod) mlib_MatrixAddS_S32C_S32C_Mod
	__attribute__((weak, alias("__mlib_MatrixAddS_S32C_S32C_Mod")));
__typeof__(__mlib_MatrixAddS_S32_S32_Sat) mlib_MatrixAddS_S32_S32_Sat
	__attribute__((weak, alias("__mlib_MatrixAddS_S32_S32_Sat")));
__typeof__(__mlib_MatrixAddS_S32C_S16C_Mod) mlib_MatrixAddS_S32C_S16C_Mod
	__attribute__((weak, alias("__mlib_MatrixAddS_S32C_S16C_Mod")));
__typeof__(__mlib_MatrixAddS_S16C_S16C_Mod) mlib_MatrixAddS_S16C_S16C_Mod
	__attribute__((weak, alias("__mlib_MatrixAddS_S16C_S16C_Mod")));
__typeof__(__mlib_MatrixAddS_S32C_Mod) mlib_MatrixAddS_S32C_Mod
	__attribute__((weak, alias("__mlib_MatrixAddS_S32C_Mod")));
__typeof__(__mlib_MatrixAddS_S16_S16_Sat) mlib_MatrixAddS_S16_S16_Sat
	__attribute__((weak, alias("__mlib_MatrixAddS_S16_S16_Sat")));
__typeof__(__mlib_MatrixAddS_S16C_Mod) mlib_MatrixAddS_S16C_Mod
	__attribute__((weak, alias("__mlib_MatrixAddS_S16C_Mod")));
__typeof__(__mlib_MatrixAddS_S32C_S32C_Sat) mlib_MatrixAddS_S32C_S32C_Sat
	__attribute__((weak, alias("__mlib_MatrixAddS_S32C_S32C_Sat")));
__typeof__(__mlib_MatrixAddS_S16C_S16C_Sat) mlib_MatrixAddS_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_MatrixAddS_S16C_S16C_Sat")));
__typeof__(__mlib_MatrixAddS_S32C_Sat) mlib_MatrixAddS_S32C_Sat
	__attribute__((weak, alias("__mlib_MatrixAddS_S32C_Sat")));
__typeof__(__mlib_MatrixAddS_S16_S8_Mod) mlib_MatrixAddS_S16_S8_Mod
	__attribute__((weak, alias("__mlib_MatrixAddS_S16_S8_Mod")));
__typeof__(__mlib_MatrixAddS_S16C_Sat) mlib_MatrixAddS_S16C_Sat
	__attribute__((weak, alias("__mlib_MatrixAddS_S16C_Sat")));
__typeof__(__mlib_MatrixAddS_S16_U8_Mod) mlib_MatrixAddS_S16_U8_Mod
	__attribute__((weak, alias("__mlib_MatrixAddS_S16_U8_Mod")));
__typeof__(__mlib_MatrixAddS_S16C_S8C_Mod) mlib_MatrixAddS_S16C_S8C_Mod
	__attribute__((weak, alias("__mlib_MatrixAddS_S16C_S8C_Mod")));
__typeof__(__mlib_MatrixAddS_S8_Mod) mlib_MatrixAddS_S8_Mod
	__attribute__((weak, alias("__mlib_MatrixAddS_S8_Mod")));
__typeof__(__mlib_MatrixAddS_S16C_U8C_Mod) mlib_MatrixAddS_S16C_U8C_Mod
	__attribute__((weak, alias("__mlib_MatrixAddS_S16C_U8C_Mod")));
__typeof__(__mlib_MatrixAddS_U8_Mod) mlib_MatrixAddS_U8_Mod
	__attribute__((weak, alias("__mlib_MatrixAddS_U8_Mod")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_U8_U8_Sat(z, x, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_U8_U8_Mod(z, x, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S16_U8_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S16_U8_Mod(z, x, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S8_S8_Sat(z, x, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S8_S8_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S8_S8_Mod(z, x, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S16_S8_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S16_S8_Mod(z, x, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S16_S16_Sat(z, x, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S16_S16_Mod(z, x, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S32_S16_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S32_S16_Mod(z, x, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S32_S32_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S32_S32_Sat(z, x, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S32_S32_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S32_S32_Mod(z, x, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_U8_Sat(
	mlib_u8 *xz,
	const mlib_u8 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_U8_U8_Sat(xz, xz, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_U8_Mod(
	mlib_u8 *xz,
	const mlib_u8 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_U8_U8_Mod(xz, xz, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S8_Sat(
	mlib_s8 *xz,
	const mlib_s8 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S8_S8_Sat(xz, xz, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S8_Mod(
	mlib_s8 *xz,
	const mlib_s8 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S8_S8_Mod(xz, xz, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S16_Sat(
	mlib_s16 *xz,
	const mlib_s16 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S16_S16_Sat(xz, xz, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S16_Mod(
	mlib_s16 *xz,
	const mlib_s16 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S16_S16_Mod(xz, xz, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S32_Sat(
	mlib_s32 *xz,
	const mlib_s32 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S32_S32_Sat(xz, xz, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S32_Mod(
	mlib_s32 *xz,
	const mlib_s32 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S32_S32_Mod(xz, xz, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_U8C_U8C_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_U8C_U8C_Sat(z, x, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_U8C_U8C_Mod(z, x, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S16C_U8C_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S16C_U8C_Mod(z, x, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S8C_S8C_Sat(z, x, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S8C_S8C_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S8C_S8C_Mod(z, x, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S16C_S8C_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S16C_S8C_Mod(z, x, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S16C_S16C_Sat(z, x, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S16C_S16C_Mod(z, x, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S32C_S16C_Mod(z, x, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S32C_S32C_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S32C_S32C_Sat(z, x, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S32C_S32C_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S32C_S32C_Mod(z, x, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_U8C_Sat(
	mlib_u8 *xz,
	const mlib_u8 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_U8C_U8C_Sat(xz, xz, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_U8C_Mod(
	mlib_u8 *xz,
	const mlib_u8 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_U8C_U8C_Mod(xz, xz, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S8C_Sat(
	mlib_s8 *xz,
	const mlib_s8 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S8C_S8C_Sat(xz, xz, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S8C_Mod(
	mlib_s8 *xz,
	const mlib_s8 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S8C_S8C_Mod(xz, xz, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S16C_Sat(
	mlib_s16 *xz,
	const mlib_s16 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S16C_S16C_Sat(xz, xz, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S16C_Mod(
	mlib_s16 *xz,
	const mlib_s16 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S16C_S16C_Mod(xz, xz, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S32C_Sat(
	mlib_s32 *xz,
	const mlib_s32 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S32C_S32C_Sat(xz, xz, c, m * n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixAddS_S32C_Mod(
	mlib_s32 *xz,
	const mlib_s32 *c,
	mlib_s32 m,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S32C_S32C_Mod(xz, xz, c, m * n));
}

/* *********************************************************** */
