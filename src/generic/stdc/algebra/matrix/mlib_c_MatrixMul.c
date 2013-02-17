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

#pragma ident	"@(#)mlib_c_MatrixMul.c	9.3	07/10/08 SMI"

/*
 * FUNCTIONS
 *      mlib_MatrixMul_[U8|S16]_U8_[Sat|Mod] - multiplication of
 *                                             unsigned 8-bit
 *                                             format element matrices
 *      mlib_MatrixMul_[S8|S16]_S8_[Sat|Mod] - multiplication of
 *                                              signed 8-bit
 *                                              format element matrices
 *      mlib_MatrixMul_[S16|S32]_S16_[Sat|Mod] - multiplication of
 *                                               signed 16-bit
 *                                               format element matrices
 *
 * SYNOPSIS
 *      mlib_status mlib_MatrixMul_S16_S8_[Sat|Mod](mlib_s16      * z,
 *                                                  const mlib_s8 * x,
 *                                                  const mlib_s8 * y,
 *                                                  mlib_s32      m,
 *                                                  mlib_s32      l,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_MatrixMul_S16_U8_[Sat|Mod](mlib_s16      * z,
 *                                                  const mlib_u8 * x,
 *                                                  const mlib_u8 * y,
 *                                                  mlib_s32      m,
 *                                                  mlib_s32      l,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_MatrixMul_S8_S8_[Sat|Mod](mlib_s8       * z,
 *                                                 const mlib_s8 * x,
 *                                                 const mlib_s8 * y,
 *                                                 mlib_s32      m,
 *                                                 mlib_s32      l,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_MatrixMul_U8_U8_[Sat|Mod](mlib_u8       * z,
 *                                                 const mlib_u8 * x,
 *                                                 const mlib_u8 * y,
 *                                                 mlib_s32      m,
 *                                                 mlib_s32      l,
 *                                                 mlib_s32      n);
 *
 *      mlib_status mlib_MatrixMul_S16_S16_Mod(mlib_s16       * z,
 *                                             const mlib_s16 * x,
 *                                             const mlib_s16 * y,
 *                                             mlib_s32       m,
 *                                             mlib_s32       l,
 *                                             mlib_s32       n);
 *      mlib_status mlib_MatrixMul_S32_S16_Mod(mlib_s32       * z,
 *                                             const mlib_s16 * x,
 *                                             const mlib_s16 * y,
 *                                             mlib_s32       m,
 *                                             mlib_s32       l,
 *                                             mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result matrix.
 *           The matrix is in row major order
 *      x    pointer to the first element of the first matrix.
 *           The matrix is in row major order
 *      y    pointer to the first element of the second matrix.
 *           The matrix is in row major order
 *      m    number of rows in the first matrix
 *      l    number of columns in the first matrix, and number of rows
 *           in the second matrix
 *      n    number of columns in the second matrix
 *
 * DESCRIPTION
 *                 l
 *      z[i][j] = SUM (x[i][k] * y[k][j])
 *                k = 1
 */

#include <mlib_algebra.h>
#include <mlib_MatrixMul.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_MatrixMul_S16_S8_Sat = __mlib_MatrixMul_S16_S8_Sat
#pragma weak mlib_MatrixMul_S8_S8_Sat = __mlib_MatrixMul_S8_S8_Sat
#pragma weak mlib_MatrixMul_S16_U8_Sat = __mlib_MatrixMul_S16_U8_Sat
#pragma weak mlib_MatrixMul_S32_S16_Mod = __mlib_MatrixMul_S32_S16_Mod
#pragma weak mlib_MatrixMul_U8_U8_Sat = __mlib_MatrixMul_U8_U8_Sat
#pragma weak mlib_MatrixMul_S16_S16_Mod = __mlib_MatrixMul_S16_S16_Mod
#pragma weak mlib_MatrixMul_S16_S8_Mod = __mlib_MatrixMul_S16_S8_Mod
#pragma weak mlib_MatrixMul_S8_S8_Mod = __mlib_MatrixMul_S8_S8_Mod
#pragma weak mlib_MatrixMul_S16_U8_Mod = __mlib_MatrixMul_S16_U8_Mod
#pragma weak mlib_MatrixMul_U8_U8_Mod = __mlib_MatrixMul_U8_U8_Mod

#elif defined(__GNUC__)

__typeof__(__mlib_MatrixMul_S16_S8_Sat) mlib_MatrixMul_S16_S8_Sat
	__attribute__((weak, alias("__mlib_MatrixMul_S16_S8_Sat")));
__typeof__(__mlib_MatrixMul_S8_S8_Sat) mlib_MatrixMul_S8_S8_Sat
	__attribute__((weak, alias("__mlib_MatrixMul_S8_S8_Sat")));
__typeof__(__mlib_MatrixMul_S16_U8_Sat) mlib_MatrixMul_S16_U8_Sat
	__attribute__((weak, alias("__mlib_MatrixMul_S16_U8_Sat")));
__typeof__(__mlib_MatrixMul_S32_S16_Mod) mlib_MatrixMul_S32_S16_Mod
	__attribute__((weak, alias("__mlib_MatrixMul_S32_S16_Mod")));
__typeof__(__mlib_MatrixMul_U8_U8_Sat) mlib_MatrixMul_U8_U8_Sat
	__attribute__((weak, alias("__mlib_MatrixMul_U8_U8_Sat")));
__typeof__(__mlib_MatrixMul_S16_S16_Mod) mlib_MatrixMul_S16_S16_Mod
	__attribute__((weak, alias("__mlib_MatrixMul_S16_S16_Mod")));
__typeof__(__mlib_MatrixMul_S16_S8_Mod) mlib_MatrixMul_S16_S8_Mod
	__attribute__((weak, alias("__mlib_MatrixMul_S16_S8_Mod")));
__typeof__(__mlib_MatrixMul_S8_S8_Mod) mlib_MatrixMul_S8_S8_Mod
	__attribute__((weak, alias("__mlib_MatrixMul_S8_S8_Mod")));
__typeof__(__mlib_MatrixMul_S16_U8_Mod) mlib_MatrixMul_S16_U8_Mod
	__attribute__((weak, alias("__mlib_MatrixMul_S16_U8_Mod")));
__typeof__(__mlib_MatrixMul_U8_U8_Mod) mlib_MatrixMul_U8_U8_Mod
	__attribute__((weak, alias("__mlib_MatrixMul_U8_U8_Mod")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

MATRIX_MUL_FP32(U8, U8, Sat)
	MATRIX_MUL_FP32(S8, S8, Sat)
	MATRIX_MUL_FP32(S16, U8, Sat)
	MATRIX_MUL_FP32(S16, S8, Sat)

	MATRIX_MUL_FP32(U8, U8, Mod)
	MATRIX_MUL_FP32(S8, S8, Mod)
	MATRIX_MUL_FP32(S16, U8, Mod)
	MATRIX_MUL_FP32(S16, S8, Mod)

	MATRIX_MUL_FP64(S16, S16, Mod)
	MATRIX_MUL_FP64(S32, S16, Mod)

/* *********************************************************** */
