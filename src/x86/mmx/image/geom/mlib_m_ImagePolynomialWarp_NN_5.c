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

#pragma ident	"@(#)mlib_m_ImagePolynomialWarp_NN_5.c	9.2	07/11/05 SMI"

/* *********************************************************** */
#include <mlib_image.h>
#include <mlib_ImagePolynomialWarp.h>

/* *********************************************************** */
#define	INIT_COEFFS                                                   \
	a0 = xCoeffs[0] + xCoeffs[6] * y + xCoeffs[11] * y * y +      \
	    xCoeffs[15] * y * y * y + xCoeffs[18] * y * y * y * y +   \
	    xCoeffs[20] * y * y * y * y * y;                          \
	a1 = xCoeffs[1] + xCoeffs[7] * y + xCoeffs[12] * y * y +      \
	    xCoeffs[16] * y * y * y + xCoeffs[19] * y * y * y * y;    \
	a2 = xCoeffs[2] + xCoeffs[8] * y + xCoeffs[13] * y * y +      \
	    xCoeffs[17] * y * y * y;                                  \
	a3 = xCoeffs[3] + xCoeffs[9] * y + xCoeffs[14] * y * y;       \
	a4 = xCoeffs[4] + xCoeffs[10] * y;                            \
	a5 = xCoeffs[5];                                              \
	wx = a0 + a1 * x + a2 * x * x + a3 * x * x * x +              \
	    a4 * x * x * x * x + a5 * x * x * x * x * x;              \
	wx += 1;                                                      \
	a0 = 1 * a5 + 1 * a4 + 1 * a3 + 1 * a2 + 1 * a1;              \
	a1 = 5 * a5 + 4 * a4 + 3 * a3 + 2 * a2;                       \
	a2 = 10 * a5 + 6 * a4 + 3 * a3;                               \
	a3 = 10 * a5 + 4 * a4;                                        \
	a4 = 5 * a5;                                                  \
	dx = a0 + a1 * x + a2 * x * x + a3 * x * x * x +              \
	    a4 * x * x * x * x;                                       \
	a0 = 1 * a4 + 1 * a3 + 1 * a2 + 1 * a1;                       \
	a1 = 4 * a4 + 3 * a3 + 2 * a2;                                \
	a2 = 6 * a4 + 3 * a3;                                         \
	a3 = 4 * a4;                                                  \
	ddx = a0 + a1 * x + a2 * x * x + a3 * x * x * x;              \
	a0 = 1 * a3 + 1 * a2 + 1 * a1;                                \
	a1 = 3 * a3 + 2 * a2;                                         \
	a2 = 3 * a3;                                                  \
	dddx = a0 + a1 * x + a2 * x * x;                              \
	a0 = 1 * a2 + 1 * a1;                                         \
	a1 = 2 * a2;                                                  \
	ddddx = a0 + a1 * x;                                          \
	dddddx = a1;                                                  \
	b0 = yCoeffs[0] + yCoeffs[6] * y + yCoeffs[11] * y * y +      \
	    yCoeffs[15] * y * y * y + yCoeffs[18] * y * y * y * y +   \
	    yCoeffs[20] * y * y * y * y * y;                          \
	b1 = yCoeffs[1] + yCoeffs[7] * y + yCoeffs[12] * y * y +      \
	    yCoeffs[16] * y * y * y + yCoeffs[19] * y * y * y * y;    \
	b2 = yCoeffs[2] + yCoeffs[8] * y + yCoeffs[13] * y * y +      \
	    yCoeffs[17] * y * y * y;                                  \
	b3 = yCoeffs[3] + yCoeffs[9] * y + yCoeffs[14] * y * y;       \
	b4 = yCoeffs[4] + yCoeffs[10] * y;                            \
	b5 = yCoeffs[5];                                              \
	wy = b0 + b1 * x + b2 * x * x + b3 * x * x * x +              \
	    b4 * x * x * x * x + b5 * x * x * x * x * x;              \
	wy += 1;                                                      \
	b0 = 1 * b5 + 1 * b4 + 1 * b3 + 1 * b2 + 1 * b1;              \
	b1 = 5 * b5 + 4 * b4 + 3 * b3 + 2 * b2;                       \
	b2 = 10 * b5 + 6 * b4 + 3 * b3;                               \
	b3 = 10 * b5 + 4 * b4;                                        \
	b4 = 5 * b5;                                                  \
	dy = b0 + b1 * x + b2 * x * x + b3 * x * x * x +              \
	    b4 * x * x * x * x;                                       \
	b0 = 1 * b4 + 1 * b3 + 1 * b2 + 1 * b1;                       \
	b1 = 4 * b4 + 3 * b3 + 2 * b2;                                \
	b2 = 6 * b4 + 3 * b3;                                         \
	b3 = 4 * b4;                                                  \
	ddy = b0 + b1 * x + b2 * x * x + b3 * x * x * x;              \
	b0 = 1 * b3 + 1 * b2 + 1 * b1;                                \
	b1 = 3 * b3 + 2 * b2;                                         \
	b2 = 3 * b3;                                                  \
	dddy = b0 + b1 * x + b2 * x * x;                              \
	b0 = 1 * b2 + 1 * b1;                                         \
	b1 = 2 * b2;                                                  \
	ddddy = b0 + b1 * x;                                          \
	dddddy = b1;

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
	ddddx += dddddx;                                        \
	wy += dy;                                               \
	dy += ddy;                                              \
	ddy += dddy;                                            \
	dddy += ddddy;                                          \
	ddddy += dddddy;

/* *********************************************************** */
#define	VARS                                                               \
	mlib_d64 x, wx, dx, ddx, dddx, ddddx, dddddx, a0, a1, a2, a3,      \
	    a4, a5, y, wy, dy, ddy, dddy, ddddy, dddddy, b0, b1, b2, b3,   \
	    b4, b5;
/* *********************************************************** */
#define	NAME_U8_1	mlib_ImagePolynomialWarp_5_NN_U8_1
#define	NAME_U8_2	mlib_ImagePolynomialWarp_5_NN_U8_2
#define	NAME_U8_3	mlib_ImagePolynomialWarp_5_NN_U8_3
#define	NAME_U8_4	mlib_ImagePolynomialWarp_5_NN_U8_4

#define	NAME_S16_1	mlib_ImagePolynomialWarp_5_NN_S16_1
#define	NAME_S16_2	mlib_ImagePolynomialWarp_5_NN_S16_2
#define	NAME_S16_3	mlib_ImagePolynomialWarp_5_NN_S16_3
#define	NAME_S16_4	mlib_ImagePolynomialWarp_5_NN_S16_4

#define	NAME_S32_1	mlib_ImagePolynomialWarp_5_NN_S32_1
#define	NAME_S32_2	mlib_ImagePolynomialWarp_5_NN_S32_2
#define	NAME_S32_3	mlib_ImagePolynomialWarp_5_NN_S32_3
#define	NAME_S32_4	mlib_ImagePolynomialWarp_5_NN_S32_4
/* *********************************************************** */
#include <mlib_m_ImagePolynomialWarp_NN.h>
/* *********************************************************** */
