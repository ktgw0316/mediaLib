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

#pragma ident	"@(#)mlib_i_VectorMulM.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorMulM_U8_U8[Sat, Mod]   - multiplication of unsigned 8-bit
 *                                          format vector by matrix
 *      mlib_VectorMulM_S8_S8[Sat, Mod]   - multiplication of signed 8-bit
 *                                          format vector by matrix
 *      mlib_VectorMulM_S16_S16[Sat, Mod] - multiplication of signed 16-bit
 *                                          format vector by matrix
 *      mlib_VectorMulM_S32_S32[Sat, Mod] - multiplication of signed 32-bit
 *                                          format vector by matrix
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMulM_U8_U8[Sat, Mod](mlib_u8       *z,
 *                                                 const mlib_u8 *x,
 *                                                 const mlib_u8 *y,
 *                                                 mlib_s32      m,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_VectorMulM_S8_S8[Sat, Mod](mlib_s8       *z,
 *                                                 const mlib_s8 *x,
 *                                                 const mlib_s8 *y,
 *                                                 mlib_s32      m,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_VectorMulM_S16_S16[Sat, Mod](mlib_s16       *z,
 *                                                   const mlib_s16 *x,
 *                                                   const mlib_s16 *y,
 *                                                   mlib_s32       m,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_VectorMulM_S32_S32[Sat, Mod](mlib_s32       *z,
 *                                                   const mlib_s32 *x,
 *                                                   const mlib_s32 *y,
 *                                                   mlib_s32       m,
 *                                                   mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      x    pointer to the first element of the input vector
 *      y    pointer to the first element of the input matrix
 *      m    number of rows in the matrix, and number of elements
 *           in the input vector
 *      n    number of columns in the matrix, and number of elements
 *           in the result vector
 *
 * DESCRIPTION
 *              m
 *      z[i] = SUM (x[k] * y[k][i])
 *             k = 1
 */

/*
 * FUNCTIONS
 *      mlib_VectorMulM_U8C_U8C_[Sat|Mod]    - multiplication of unsigned 8-bit
 *                                             format
 *                                             complex vector by complex matrix
 *      mlib_VectorMulM_S8C_S8C_[Sat|Mod]    - multiplication of signed 8-bit
 *                                             format
 *                                             complex vector by complex matrix
 *      mlib_VectorMulM_S16C_S16C_[Sat|Mod]  - multiplication of signed 16-bit
 *                                             format
 *                                             complex vector by complex matrix
 *       mlib_VectorMulM_S32C_S32C_[Sat|Mod] - multiplication of signed 32-bit
 *                                             format
 *                                             complex vector by complex matrix
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMulM_U8C_U8C[Sat, Mod](mlib_u8       *z,
 *                                                   const mlib_u8 *x,
 *                                                   const mlib_u8 *y,
 *                                                   mlib_s32      m,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_VectorMulM_S8C_S8C[Sat, Mod](mlib_s8       *z,
 *                                                   const mlib_s8 *x,
 *                                                   const mlib_s8 *y,
 *                                                   mlib_s32      m,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_VectorMulM_S16C_S16C[Sat, Mod](mlib_s16       *z,
 *                                                     const mlib_s16 *x,
 *                                                     const mlib_s16 *y,
 *                                                     mlib_s32       m,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_VectorMulM_S32C_S32C[Sat, Mod](mlib_s32       *z,
 *                                                     const mlib_s32 *x,
 *                                                     const mlib_s32 *y,
 *                                                     mlib_s32       m,
 *                                                     mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts
 *      x    pointer to the first element of the input vector
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      y    pointer to the first element of the input matrix
 *           y[2*i] hold the real parts, and y[2*i + 1] hold the imaginary
 *           parts
 *      m    number of rows in the matrix, and number of elements
 *           in the input vector
 *      n    number of columns in the matrix, and number of elements
 *           in the result vector
 *
 * DESCRIPTION
 *              m
 *      z[i] = SUM (x[k] * y[k][i])
 *             k = 1
 */

#include <stdio.h>
#include <mlib_algebra.h>
#include <mlib_VectorMulM.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorMulM_S32_S16_Mod = __mlib_VectorMulM_S32_S16_Mod
#pragma weak mlib_VectorMulM_S16C_S8C_Sat = __mlib_VectorMulM_S16C_S8C_Sat
#pragma weak mlib_VectorMulM_S16_S16_Mod = __mlib_VectorMulM_S16_S16_Mod
#pragma weak mlib_VectorMulM_S16C_U8C_Sat = __mlib_VectorMulM_S16C_U8C_Sat
#pragma weak mlib_VectorMulM_S8_S8_Mod = __mlib_VectorMulM_S8_S8_Mod
#pragma weak mlib_VectorMulM_S32C_S16C_Mod = __mlib_VectorMulM_S32C_S16C_Mod
#pragma weak mlib_VectorMulM_S32_S16_Sat = __mlib_VectorMulM_S32_S16_Sat
#pragma weak mlib_VectorMulM_S16C_S16C_Mod = __mlib_VectorMulM_S16C_S16C_Mod
#pragma weak mlib_VectorMulM_U8_U8_Mod = __mlib_VectorMulM_U8_U8_Mod
#pragma weak mlib_VectorMulM_S16_S16_Sat = __mlib_VectorMulM_S16_S16_Sat
#pragma weak mlib_VectorMulM_S8C_S8C_Mod = __mlib_VectorMulM_S8C_S8C_Mod
#pragma weak mlib_VectorMulM_S8_S8_Sat = __mlib_VectorMulM_S8_S8_Sat
#pragma weak mlib_VectorMulM_S32C_S16C_Sat = __mlib_VectorMulM_S32C_S16C_Sat
#pragma weak mlib_VectorMulM_U8C_U8C_Mod = __mlib_VectorMulM_U8C_U8C_Mod
#pragma weak mlib_VectorMulM_S16C_S16C_Sat = __mlib_VectorMulM_S16C_S16C_Sat
#pragma weak mlib_VectorMulM_U8_U8_Sat = __mlib_VectorMulM_U8_U8_Sat
#pragma weak mlib_VectorMulM_S8C_S8C_Sat = __mlib_VectorMulM_S8C_S8C_Sat
#pragma weak mlib_VectorMulM_S16_S8_Mod = __mlib_VectorMulM_S16_S8_Mod
#pragma weak mlib_VectorMulM_S16_U8_Mod = __mlib_VectorMulM_S16_U8_Mod
#pragma weak mlib_VectorMulM_U8C_U8C_Sat = __mlib_VectorMulM_U8C_U8C_Sat
#pragma weak mlib_VectorMulM_S16C_S8C_Mod = __mlib_VectorMulM_S16C_S8C_Mod
#pragma weak mlib_VectorMulM_S16C_U8C_Mod = __mlib_VectorMulM_S16C_U8C_Mod
#pragma weak mlib_VectorMulM_S16_S8_Sat = __mlib_VectorMulM_S16_S8_Sat
#pragma weak mlib_VectorMulM_S16_U8_Sat = __mlib_VectorMulM_S16_U8_Sat

#elif defined(__GNUC__)

__typeof__(__mlib_VectorMulM_S32_S16_Mod) mlib_VectorMulM_S32_S16_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S32_S16_Mod")));
__typeof__(__mlib_VectorMulM_S16C_S8C_Sat) mlib_VectorMulM_S16C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S16C_S8C_Sat")));
__typeof__(__mlib_VectorMulM_S16_S16_Mod) mlib_VectorMulM_S16_S16_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S16_S16_Mod")));
__typeof__(__mlib_VectorMulM_S16C_U8C_Sat) mlib_VectorMulM_S16C_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S16C_U8C_Sat")));
__typeof__(__mlib_VectorMulM_S8_S8_Mod) mlib_VectorMulM_S8_S8_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S8_S8_Mod")));
__typeof__(__mlib_VectorMulM_S32C_S16C_Mod) mlib_VectorMulM_S32C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S32C_S16C_Mod")));
__typeof__(__mlib_VectorMulM_S32_S16_Sat) mlib_VectorMulM_S32_S16_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S32_S16_Sat")));
__typeof__(__mlib_VectorMulM_S16C_S16C_Mod) mlib_VectorMulM_S16C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S16C_S16C_Mod")));
__typeof__(__mlib_VectorMulM_U8_U8_Mod) mlib_VectorMulM_U8_U8_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_U8_U8_Mod")));
__typeof__(__mlib_VectorMulM_S16_S16_Sat) mlib_VectorMulM_S16_S16_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S16_S16_Sat")));
__typeof__(__mlib_VectorMulM_S8C_S8C_Mod) mlib_VectorMulM_S8C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S8C_S8C_Mod")));
__typeof__(__mlib_VectorMulM_S8_S8_Sat) mlib_VectorMulM_S8_S8_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S8_S8_Sat")));
__typeof__(__mlib_VectorMulM_S32C_S16C_Sat) mlib_VectorMulM_S32C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S32C_S16C_Sat")));
__typeof__(__mlib_VectorMulM_U8C_U8C_Mod) mlib_VectorMulM_U8C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_U8C_U8C_Mod")));
__typeof__(__mlib_VectorMulM_S16C_S16C_Sat) mlib_VectorMulM_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S16C_S16C_Sat")));
__typeof__(__mlib_VectorMulM_U8_U8_Sat) mlib_VectorMulM_U8_U8_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_U8_U8_Sat")));
__typeof__(__mlib_VectorMulM_S8C_S8C_Sat) mlib_VectorMulM_S8C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S8C_S8C_Sat")));
__typeof__(__mlib_VectorMulM_S16_S8_Mod) mlib_VectorMulM_S16_S8_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S16_S8_Mod")));
__typeof__(__mlib_VectorMulM_S16_U8_Mod) mlib_VectorMulM_S16_U8_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S16_U8_Mod")));
__typeof__(__mlib_VectorMulM_U8C_U8C_Sat) mlib_VectorMulM_U8C_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_U8C_U8C_Sat")));
__typeof__(__mlib_VectorMulM_S16C_S8C_Mod) mlib_VectorMulM_S16C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S16C_S8C_Mod")));
__typeof__(__mlib_VectorMulM_S16C_U8C_Mod) mlib_VectorMulM_S16C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S16C_U8C_Mod")));
__typeof__(__mlib_VectorMulM_S16_S8_Sat) mlib_VectorMulM_S16_S8_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S16_S8_Sat")));
__typeof__(__mlib_VectorMulM_S16_U8_Sat) mlib_VectorMulM_S16_U8_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S16_U8_Sat")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

VECTOR_MULM_FP(U8, U8, Sat)
	VECTOR_MULM_FP(S8, S8, Sat)
	VECTOR_MULM_FP(S16, U8, Sat)
	VECTOR_MULM_FP(S16, S8, Sat)

	VECTOR_MULM_FP(U8, U8, Mod)
	VECTOR_MULM_FP(S8, S8, Mod)
	VECTOR_MULM_FP(S16, U8, Mod)
	VECTOR_MULM_FP(S16, S8, Mod)
	VECTOR_MULM_FP(S16, S16, Mod)
	VECTOR_MULM_FP(S32, S16, Mod)

/* *********************************************************** */
	VECTORC_MULM_FP(U8, U8, Sat)
	VECTORC_MULM_FP(S8, S8, Sat)
	VECTORC_MULM_FP(S16, U8, Sat)
	VECTORC_MULM_FP(S16, S8, Sat)

	VECTORC_MULM_FP(U8, U8, Mod)
	VECTORC_MULM_FP(S8, S8, Mod)
	VECTORC_MULM_FP(S16, U8, Mod)
	VECTORC_MULM_FP(S16, S8, Mod)
	VECTORC_MULM_FP(S16, S16, Mod)
	VECTORC_MULM_FP(S32, S16, Mod)

/* *********************************************************** */
	VECTOR_MULM_FP(S16, S16, Sat)
	VECTOR_MULM_FP(S32, S16, Sat)

	VECTORC_MULM_FP(S16, S16, Sat)
	VECTORC_MULM_FP(S32, S16, Sat)

/* *********************************************************** */
