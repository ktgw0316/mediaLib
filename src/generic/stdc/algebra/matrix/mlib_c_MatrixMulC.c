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

#pragma ident	"@(#)mlib_c_MatrixMulC.c	9.3	07/10/08 SMI"

/*
 * FUNCTIONS
 *      mlib_MatrixMul_U8C_[U8C|S16C]_[Sat|Mod]   - multiplication of
 *                                                  unsigned 8-bit format
 *                                                  element complex matrices
 *      mlib_MatrixMul_S8C_[S8C|S16C]_[Sat|Mod]   - multiplication of
 *                                                  signed 8-bit format
 *                                                  element complex matrices
 *      mlib_MatrixMul_S16C_[S16C|S32C]_[Sat|Mod] - multiplication of
 *                                                  signed 16-bit format
 *                                                  element complex matrices
 *      mlib_MatrixMul_S32C_S32C_[Sat|Mod]        - multiplication of
 *                                                  signed 32-bit format
 *                                                  element complex matrices
 *
 * SYNOPSIS
 *      mlib_status mlib_MatrixMul_S16C_S16C_[Sat|Mod](mlib_s16       *z,
 *                                                     const mlib_s16 *x,
 *                                                     const mlib_s16 *y,
 *                                                     mlib_s32       m,
 *                                                     mlib_s32       l,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_MatrixMul_S16C_S8C_[Sat|Mod](mlib_s16      *z,
 *                                                    const mlib_s8 *x,
 *                                                    const mlib_s8 *y,
 *                                                    mlib_s32      m,
 *                                                    mlib_s32      l,
 *                                                    mlib_s32      n);
 *      mlib_status mlib_MatrixMul_S16C_U8C_[Sat|Mod](mlib_s16      *z,
 *                                                    const mlib_u8 *x,
 *                                                    const mlib_u8 *y,
 *                                                    mlib_s32      m,
 *                                                    mlib_s32      l,
 *                                                    mlib_s32      n);
 *      mlib_status mlib_MatrixMul_S32C_S16C_[Sat|Mod](mlib_s32       *z,
 *                                                     const mlib_s16 *x,
 *                                                     const mlib_s16 *y,
 *                                                     mlib_s32       m,
 *                                                     mlib_s32       l,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_MatrixMul_S32C_S32C_[Sat|Mod](mlib_s32       *z,
 *                                                     const mlib_s32 *x,
 *                                                     const mlib_s32 *y,
 *                                                     mlib_s32       m,
 *                                                     mlib_s32       l,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_MatrixMul_S8C_S8C_[Sat|Mod](mlib_s8       *z,
 *                                                   const mlib_s8 *x,
 *                                                   const mlib_s8 *y,
 *                                                   mlib_s32      m,
 *                                                   mlib_s32      l,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_MatrixMul_U8C_U8C_[Sat|Mod](mlib_u8       *z,
 *                                                   const mlib_u8 *x,
 *                                                   const mlib_u8 *y,
 *                                                   mlib_s32      m,
 *                                                   mlib_s32      l,
 *                                                   mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result matrix.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts. The matrix is in row major order
 *      x    pointer to the first complex element of the first matrix.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts. The matrix is in row major order
 *      y    pointer to the first complex element of the second matrix.
 *           y[2*i] hold the real parts, and y[2*i + 1] hold the imaginary
 *           parts. The matrix is in row major order
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

#pragma weak mlib_MatrixMul_S16C_U8C_Mod = __mlib_MatrixMul_S16C_U8C_Mod
#pragma weak mlib_MatrixMul_U8C_U8C_Mod = __mlib_MatrixMul_U8C_U8C_Mod
#pragma weak mlib_MatrixMul_S16C_S8C_Sat = __mlib_MatrixMul_S16C_S8C_Sat
#pragma weak mlib_MatrixMul_S8C_S8C_Sat = __mlib_MatrixMul_S8C_S8C_Sat
#pragma weak mlib_MatrixMul_S16C_U8C_Sat = __mlib_MatrixMul_S16C_U8C_Sat
#pragma weak mlib_MatrixMul_S32C_S16C_Mod = __mlib_MatrixMul_S32C_S16C_Mod
#pragma weak mlib_MatrixMul_U8C_U8C_Sat = __mlib_MatrixMul_U8C_U8C_Sat
#pragma weak mlib_MatrixMul_S16C_S16C_Mod = __mlib_MatrixMul_S16C_S16C_Mod
#pragma weak mlib_MatrixMul_S16C_S8C_Mod = __mlib_MatrixMul_S16C_S8C_Mod
#pragma weak mlib_MatrixMul_S8C_S8C_Mod = __mlib_MatrixMul_S8C_S8C_Mod

#elif defined(__GNUC__)

__typeof__(__mlib_MatrixMul_S16C_U8C_Mod) mlib_MatrixMul_S16C_U8C_Mod
	__attribute__((weak, alias("__mlib_MatrixMul_S16C_U8C_Mod")));
__typeof__(__mlib_MatrixMul_U8C_U8C_Mod) mlib_MatrixMul_U8C_U8C_Mod
	__attribute__((weak, alias("__mlib_MatrixMul_U8C_U8C_Mod")));
__typeof__(__mlib_MatrixMul_S16C_S8C_Sat) mlib_MatrixMul_S16C_S8C_Sat
	__attribute__((weak, alias("__mlib_MatrixMul_S16C_S8C_Sat")));
__typeof__(__mlib_MatrixMul_S8C_S8C_Sat) mlib_MatrixMul_S8C_S8C_Sat
	__attribute__((weak, alias("__mlib_MatrixMul_S8C_S8C_Sat")));
__typeof__(__mlib_MatrixMul_S16C_U8C_Sat) mlib_MatrixMul_S16C_U8C_Sat
	__attribute__((weak, alias("__mlib_MatrixMul_S16C_U8C_Sat")));
__typeof__(__mlib_MatrixMul_S32C_S16C_Mod) mlib_MatrixMul_S32C_S16C_Mod
	__attribute__((weak, alias("__mlib_MatrixMul_S32C_S16C_Mod")));
__typeof__(__mlib_MatrixMul_U8C_U8C_Sat) mlib_MatrixMul_U8C_U8C_Sat
	__attribute__((weak, alias("__mlib_MatrixMul_U8C_U8C_Sat")));
__typeof__(__mlib_MatrixMul_S16C_S16C_Mod) mlib_MatrixMul_S16C_S16C_Mod
	__attribute__((weak, alias("__mlib_MatrixMul_S16C_S16C_Mod")));
__typeof__(__mlib_MatrixMul_S16C_S8C_Mod) mlib_MatrixMul_S16C_S8C_Mod
	__attribute__((weak, alias("__mlib_MatrixMul_S16C_S8C_Mod")));
__typeof__(__mlib_MatrixMul_S8C_S8C_Mod) mlib_MatrixMul_S8C_S8C_Mod
	__attribute__((weak, alias("__mlib_MatrixMul_S8C_S8C_Mod")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

MATRIX_MULC_FP32(U8, U8, Sat)
	MATRIX_MULC_FP32(S8, S8, Sat)
	MATRIX_MULC_FP32(S16, U8, Sat)
	MATRIX_MULC_FP32(S16, S8, Sat)

	MATRIX_MULC_FP32(U8, U8, Mod)
	MATRIX_MULC_FP32(S8, S8, Mod)
	MATRIX_MULC_FP32(S16, U8, Mod)
	MATRIX_MULC_FP32(S16, S8, Mod)

	MATRIX_MULC_FP64(S16, S16, Mod)
	MATRIX_MULC_FP64(S32, S16, Mod)

/* *********************************************************** */
