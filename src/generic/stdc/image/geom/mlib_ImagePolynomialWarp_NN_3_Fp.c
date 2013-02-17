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

#pragma ident	"@(#)mlib_ImagePolynomialWarp_NN_3_Fp.c	9.2	07/10/09 SMI"

#include <mlib_image.h>
#include <mlib_ImagePolynomialWarp.h>

/* *********************************************************** */

#define	INIT_COEFFS                                                     \
	a0 = xCoeffs[0] + xCoeffs[4] * y + xCoeffs[7] * y * y +         \
	    xCoeffs[9] * y * y * y;                                     \
	a1 = xCoeffs[1] + xCoeffs[5] * y + xCoeffs[8] * y * y;          \
	a2 = xCoeffs[2] + xCoeffs[6] * y;                               \
	a3 = xCoeffs[3];                                                \
	wx = a0 + a1 * x + a2 * x * x + a3 * x * x * x + 1;             \
	dx = 3 * a3 * x * x + (3 * a3 + 2 * a2) * x + (a3 + a2 + a1);   \
	ddx = 6 * a3 * x + 6 * a3 + 2 * a2;                             \
	dddx = 6 * a3;                                                  \
	b0 = yCoeffs[0] + yCoeffs[4] * y + yCoeffs[7] * y * y +         \
	    yCoeffs[9] * y * y * y;                                     \
	b1 = yCoeffs[1] + yCoeffs[5] * y + yCoeffs[8] * y * y;          \
	b2 = yCoeffs[2] + yCoeffs[6] * y;                               \
	b3 = yCoeffs[3];                                                \
	wy = b0 + b1 * x + b2 * x * x + b3 * x * x * x + 1;             \
	dy = 3 * b3 * x * x + (3 * b3 + 2 * b2) * x + (b3 + b2 + b1);   \
	ddy = 6 * b3 * x + 6 * b3 + 2 * b2;                             \
	dddy = 6 * b3

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_3_NN_D64_1)
{
	mlib_d64 buf[8], pix0, *dp, *sp;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, dddx, a0, a1, a2, a3, y, wy, dy, ddy, dddy, b0,
	    b1, b2, b3;

	y = 0.5 + preShiftY;
	srcHeight++;
	srcWidth++;

	for (j = 0; j < dstHeight; j++) {

		x = 0.5 + preShiftX;

		INIT_COEFFS;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (i = 0; i < dstWidth; i++) {

			iwx = (mlib_s32)wx;
			iwy = (mlib_s32)wy;

			mask =
			    (~(iwx - 1) & (iwx - srcWidth) & ~(iwy - 1) & (iwy -
			    srcHeight)) >> 31;

			sp = (void *)(lineAddr[iwy & mask] +
			    ((iwx * 8) & mask));

			pix0 = sp[0];

			wx += dx;
			dx += ddx;
			ddx += dddx;
			wy += dy;
			dy += ddy;
			ddy += dddy;

			dp = (void *)(((mlib_addr)(dstData +
			    8 * i) & mask) | ((mlib_addr)buf & ~mask));
			dp[0] = pix0;
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_3_NN_D64_2)
{
	mlib_d64 buf[8], pix0, pix1, *dp, *sp;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, dddx, a0, a1, a2, a3, y, wy, dy, ddy, dddy, b0,
	    b1, b2, b3;

	y = 0.5 + preShiftY;
	srcHeight++;
	srcWidth++;

	for (j = 0; j < dstHeight; j++) {

		x = 0.5 + preShiftX;

		INIT_COEFFS;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (i = 0; i < dstWidth; i++) {

			iwx = (mlib_s32)wx;
			iwy = (mlib_s32)wy;

			mask =
			    (~(iwx - 1) & (iwx - srcWidth) & ~(iwy - 1) & (iwy -
			    srcHeight)) >> 31;

			sp = (void *)(lineAddr[iwy & mask] +
			    ((iwx * 16) & mask));

			pix0 = sp[0];
			pix1 = sp[1];

			wx += dx;
			dx += ddx;
			ddx += dddx;
			wy += dy;
			dy += ddy;
			ddy += dddy;

			dp = (void *)(((mlib_addr)(dstData +
			    16 * i) & mask) | ((mlib_addr)buf & ~mask));
			dp[0] = pix0;
			dp[1] = pix1;
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_3_NN_D64_3)
{
	mlib_d64 buf[8], *dp, *sp;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, dddx, a0, a1, a2, a3, y, wy, dy, ddy, dddy, b0,
	    b1, b2, b3;

	y = 0.5 + preShiftY;
	srcHeight++;
	srcWidth++;

	for (j = 0; j < dstHeight; j++) {

		x = 0.5 + preShiftX;

		INIT_COEFFS;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (i = 0; i < dstWidth; i++) {

			iwx = (mlib_s32)wx;
			iwy = (mlib_s32)wy;

			mask =
			    (~(iwx - 1) & (iwx - srcWidth) & ~(iwy - 1) & (iwy -
			    srcHeight)) >> 31;

			wx += dx;
			dx += ddx;
			ddx += dddx;
			wy += dy;
			dy += ddy;
			ddy += dddy;

			sp = (void *)(lineAddr[iwy & mask] +
			    ((iwx * 24) & mask));
			dp = (void *)(((mlib_addr)(dstData +
			    24 * i) & mask) | ((mlib_addr)buf & ~mask));
			dp[0] = sp[0];
			dp[1] = sp[1];
			dp[2] = sp[2];
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_3_NN_D64_4)
{
	mlib_d64 buf[8], pix0, pix1, pix2, pix3, *dp, *sp;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, dddx, a0, a1, a2, a3, y, wy, dy, ddy, dddy, b0,
	    b1, b2, b3;

	y = 0.5 + preShiftY;
	srcHeight++;
	srcWidth++;

	for (j = 0; j < dstHeight; j++) {

		x = 0.5 + preShiftX;

		INIT_COEFFS;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (i = 0; i < dstWidth; i++) {

			iwx = (mlib_s32)wx;
			iwy = (mlib_s32)wy;

			mask =
			    (~(iwx - 1) & (iwx - srcWidth) & ~(iwy - 1) & (iwy -
			    srcHeight)) >> 31;

			sp = (void *)(lineAddr[iwy & mask] +
			    ((iwx * 32) & mask));

			pix0 = sp[0];
			pix1 = sp[1];
			pix2 = sp[2];
			pix3 = sp[3];

			wx += dx;
			dx += ddx;
			ddx += dddx;
			wy += dy;
			dy += ddy;
			ddy += dddy;

			dp = (void *)(((mlib_addr)(dstData +
			    32 * i) & mask) | ((mlib_addr)buf & ~mask));
			dp[0] = pix0;
			dp[1] = pix1;
			dp[2] = pix2;
			dp[3] = pix3;
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */
