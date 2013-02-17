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

#pragma ident	"@(#)mlib_m_ImagePolynomialWarp_NN_2.c	9.2	07/11/05 SMI"

/* *********************************************************** */
#include <mlib_image.h>
#include <mlib_ImagePolynomialWarp.h>

/* *********************************************************** */
#define	INIT_COEFFS                                              \
	a0 = xCoeffs[0] + xCoeffs[3] * y + xCoeffs[5] * y * y;   \
	a1 = xCoeffs[1] + xCoeffs[4] * y;                        \
	a2 = xCoeffs[2];                                         \
	wx = a0 + a1 * x + a2 * x * x + 1;                       \
	dx = 2 * a2 * x + a2 + a1;                               \
	ddx = 2 * a2;                                            \
	b0 = yCoeffs[0] + yCoeffs[3] * y + yCoeffs[5] * y * y;   \
	b1 = yCoeffs[1] + yCoeffs[4] * y;                        \
	b2 = yCoeffs[2];                                         \
	wy = b0 + b1 * x + b2 * x * x + 1;                       \
	dy = 2 * b2 * x + b2 + b1;                               \
	ddy = 2 * b2;

/* *********************************************************** */
#define	CALL_FUNC(NAME)                                                   \
	NAME(dstData, lineAddr, xCoeffs, yCoeffs, preShiftX, preShiftY,   \
	    srcWidth, srcHeight, dstWidth, dstHeight, dstStride)

/* *********************************************************** */
#define	DELTA	wx += dx; dx += ddx; wy += dy; dy += ddy;

/* *********************************************************** */
#define	VARS                                                               \
	mlib_d64 x, wx, dx, ddx, a0, a1, a2, y, wy, dy, ddy, b0, b1, b2;
/* *********************************************************** */
#define	NAME_U8_1	mlib_ImagePolynomialWarp_2_NN_U8_1
#define	NAME_U8_2	mlib_ImagePolynomialWarp_2_NN_U8_2
#define	NAME_U8_3	mlib_ImagePolynomialWarp_2_NN_U8_3
#define	NAME_U8_4	mlib_ImagePolynomialWarp_2_NN_U8_4

#define	NAME_S16_1	mlib_ImagePolynomialWarp_2_NN_S16_1
#define	NAME_S16_2	mlib_ImagePolynomialWarp_2_NN_S16_2
#define	NAME_S16_3	mlib_ImagePolynomialWarp_2_NN_S16_3
#define	NAME_S16_4	mlib_ImagePolynomialWarp_2_NN_S16_4

#define	NAME_S32_1	mlib_ImagePolynomialWarp_2_NN_S32_1
#define	NAME_S32_2	mlib_ImagePolynomialWarp_2_NN_S32_2
#define	NAME_S32_3	mlib_ImagePolynomialWarp_2_NN_S32_3
#define	NAME_S32_4	mlib_ImagePolynomialWarp_2_NN_S32_4
/* *********************************************************** */
#include <mlib_m_ImagePolynomialWarp_NN.h>
/* *********************************************************** */
