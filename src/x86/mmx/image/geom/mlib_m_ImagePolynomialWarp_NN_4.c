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

#pragma ident	"@(#)mlib_m_ImagePolynomialWarp_NN_4.c	9.2	07/11/05 SMI"

/* *********************************************************** */
#include <mlib_image.h>
#include <mlib_ImagePolynomialWarp.h>

/* *********************************************************** */
#define	INIT_COEFFS                                                    \
	a0 = xCoeffs[0] + (xCoeffs[5] + (xCoeffs[9] + (xCoeffs[12] +   \
	    xCoeffs[14] * y) * y) * y) * y;                            \
	a1 = xCoeffs[1] + (xCoeffs[6] + (xCoeffs[10] +                 \
	    xCoeffs[13] * y) * y) * y;                                 \
	a2 = xCoeffs[2] + (xCoeffs[7] + xCoeffs[11] * y) * y;          \
	a3 = xCoeffs[3] + xCoeffs[8] * y;                              \
	a4 = xCoeffs[4];                                               \
	wx = a0 + (a1 + (a2 + (a3 + a4 * x) * x) * x) * x + 1;         \
	a0 = 1 * a4 + 1 * a3 + 1 * a2 + 1 * a1;                        \
	a1 = 4 * a4 + 3 * a3 + 2 * a2;                                 \
	a2 = 6 * a4 + 3 * a3;                                          \
	a3 = 4 * a4;                                                   \
	dx = a0 + (a1 + (a2 + a3 * x) * x) * x;                        \
	a0 = 1 * a3 + 1 * a2 + 1 * a1;                                 \
	a1 = 3 * a3 + 2 * a2;                                          \
	a2 = 3 * a3;                                                   \
	ddx = a0 + (a1 + a2 * x) * x;                                  \
	a0 = 1 * a2 + 1 * a1;                                          \
	a1 = 2 * a2;                                                   \
	dddx = a0 + a1 * x;                                            \
	ddddx = a1;                                                    \
	b0 = yCoeffs[0] + (yCoeffs[5] + (yCoeffs[9] + (yCoeffs[12] +   \
	    yCoeffs[14] * y) * y) * y) * y;                            \
	b1 = yCoeffs[1] + (yCoeffs[6] + (yCoeffs[10] +                 \
	    yCoeffs[13] * y) * y) * y;                                 \
	b2 = yCoeffs[2] + (yCoeffs[7] + yCoeffs[11] * y) * y;          \
	b3 = yCoeffs[3] + yCoeffs[8] * y;                              \
	b4 = yCoeffs[4];                                               \
	wy = b0 + (b1 + (b2 + (b3 + b4 * x) * x) * x) * x + 1;         \
	b0 = 1 * b4 + 1 * b3 + 1 * b2 + 1 * b1;                        \
	b1 = 4 * b4 + 3 * b3 + 2 * b2;                                 \
	b2 = 6 * b4 + 3 * b3;                                          \
	b3 = 4 * b4;                                                   \
	dy = b0 + (b1 + (b2 + b3 * x) * x) * x;                        \
	b0 = 1 * b3 + 1 * b2 + 1 * b1;                                 \
	b1 = 3 * b3 + 2 * b2;                                          \
	b2 = 3 * b3;                                                   \
	ddy = b0 + (b1 + b2 * x) * x;                                  \
	b0 = 1 * b2 + 1 * b1;                                          \
	b1 = 2 * b2;                                                   \
	dddy = b0 + b1 * x;                                            \
	ddddy = b1;

/* *********************************************************** */
#define	CALL_FUNC(NAME)                                                   \
	NAME(dstData, lineAddr, xCoeffs, yCoeffs, preShiftX, preShiftY,   \
	    srcWidth, srcHeight, dstWidth, dstHeight, dstStride)

/* *********************************************************** */
#define	DELTA                                                   \
	wx += dx;                                               \
	dx += ddx;                                              \
	ddx += dddx;                                            \
	dddx += ddddx;                                          \
	wy += dy;                                               \
	dy += ddy;                                              \
	ddy += dddy;                                            \
	dddy += ddddy;

/* *********************************************************** */
#define	VARS                                                               \
	mlib_d64 x, wx, dx, ddx, dddx, ddddx, a0, a1, a2, a3, a4, y, wy,   \
	    dy, ddy, dddy, ddddy, b0, b1, b2, b3, b4;
/* *********************************************************** */
#define	NAME_U8_1	mlib_ImagePolynomialWarp_4_NN_U8_1
#define	NAME_U8_2	mlib_ImagePolynomialWarp_4_NN_U8_2
#define	NAME_U8_3	mlib_ImagePolynomialWarp_4_NN_U8_3
#define	NAME_U8_4	mlib_ImagePolynomialWarp_4_NN_U8_4

#define	NAME_S16_1	mlib_ImagePolynomialWarp_4_NN_S16_1
#define	NAME_S16_2	mlib_ImagePolynomialWarp_4_NN_S16_2
#define	NAME_S16_3	mlib_ImagePolynomialWarp_4_NN_S16_3
#define	NAME_S16_4	mlib_ImagePolynomialWarp_4_NN_S16_4

#define	NAME_S32_1	mlib_ImagePolynomialWarp_4_NN_S32_1
#define	NAME_S32_2	mlib_ImagePolynomialWarp_4_NN_S32_2
#define	NAME_S32_3	mlib_ImagePolynomialWarp_4_NN_S32_3
#define	NAME_S32_4	mlib_ImagePolynomialWarp_4_NN_S32_4
/* *********************************************************** */
#include <mlib_m_ImagePolynomialWarp_NN.h>
/* *********************************************************** */
