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

#pragma ident	"@(#)mlib_MatrixMulShiftC.c	9.3	07/10/08 SMI"

/*
 * FUNCTIONS
 *      mlib_MatrixMulShift_S16C_S16C_Sat
 *      multiplication of signed 16-bit matrices with shifting
 *
 * SYNOPSIS
 *      mlib_status mlib_MatrixMulShift_S16C_S16C_Sat(mlib_s16       * z,
 *                                                    const mlib_s16 * x,
 *                                                    const mlib_s16 * y,
 *                                                    mlib_s32       m,
 *                                                    mlib_s32       l,
 *                                                    mlib_s32       n,
 *                                                    mlib_s32       shift);
 *
 * ARGUMENTS
 *      z      pointer to the first element of the result matrix.
 *             The matrix is in row major order
 *      x      pointer to the first element of the first matrix.
 *             The matrix is in row major order
 *      y      pointer to the first element of the second matrix.
 *             The matrix is in row major order
 *      m      number of rows in the first matrix
 *      l      number of columns in the first matrix, and number of rows
 *             in the second matrix
 *      n      number of columns in the second matrix
 *  shift      the right shifting factor
 *
 * DESCRIPTION
 *                 l
 *      z[i][j] = SUM (x[i][k] * y[k][j]) * 2^(-shift)
 *                k = 1
 */

#include <mlib_algebra.h>
#include <mlib_MatrixMul.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_MatrixMulShift_S16C_S16C_Sat = \
	__mlib_MatrixMulShift_S16C_S16C_Sat

#elif defined(__GNUC__)

__typeof__(__mlib_MatrixMulShift_S16C_S16C_Sat)
	mlib_MatrixMulShift_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_MatrixMulShift_S16C_S16C_Sat")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

MATRIX_MULC_SHIFT(S16, S16, Sat, mlib_d64)

/* *********************************************************** */
