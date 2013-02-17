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

#pragma ident	"@(#)mlib_ImagePolynomialWarp_NN_5.c	9.2	07/10/09 SMI"

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
	dddddy = b1

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
	ddddy += dddddy

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_U8_1)
{
	mlib_u8 buf[8], pix, *dp, *dpt;
	const mlib_u8 *sp;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, dddx, ddddx, dddddx;
	mlib_d64 a0, a1, a2, a3, a4, a5;
	mlib_d64 y, wy, dy, ddy, dddy, ddddy, dddddy;
	mlib_d64 b0, b1, b2, b3, b4, b5;

	y = 0.5 + preShiftY;
	srcHeight++;
	srcWidth++;

	for (j = 0; j < dstHeight; j++) {

		x = 0.5 + preShiftX;
		dpt = dstData;

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

			sp = lineAddr[(mlib_s32)wy & mask] +
			    ((mlib_s32)wx & mask);
			pix = *sp;

			dp = (void *)(((mlib_addr)dpt & mask) | ((mlib_addr)buf
			    & ~mask));

			DELTA;

			*dp = pix;
			dpt++;
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_U8_2)
{
	mlib_u8 buf[8], pix1, *dp;
	const mlib_u8 *sp;
	mlib_s32 i, j, pos, iwx, iwy, mask, srcStride =
	    lineAddr[2] - lineAddr[1];
	mlib_d64 x, wx, dx, ddx, dddx, ddddx, dddddx;
	mlib_d64 a0, a1, a2, a3, a4, a5;
	mlib_d64 y, wy, dy, ddy, dddy, ddddy, dddddy;
	mlib_d64 b0, b1, b2, b3, b4, b5;

	pos =
	    dstStride | srcStride | (mlib_addr)dstData |
	    (mlib_addr)(lineAddr[0]);

	if ((pos & 1) == 0) {
		CALL_FUNC(mlib_ImagePolynomialWarp_5_NN_S16_1);
		return;
	}

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

			sp = lineAddr[iwy & mask] + ((iwx * 2) & mask);

			pix1 = sp[1];
			dp = (void *)(((mlib_addr)(dstData +
			    2 * i) & mask) | ((mlib_addr)buf & ~mask));

			DELTA;

			dp[0] = sp[0];
			dp[1] = pix1;
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_U8_3)
{
	mlib_u8 buf[8], pix0, pix1, pix2, *dp, *dpt;
	const mlib_u8 *sp;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, dddx, ddddx, dddddx;
	mlib_d64 a0, a1, a2, a3, a4, a5;
	mlib_d64 y, wy, dy, ddy, dddy, ddddy, dddddy;
	mlib_d64 b0, b1, b2, b3, b4, b5;

	y = 0.5 + preShiftY;
	srcHeight++;
	srcWidth = (srcWidth + 1) * 3;

	for (j = 0; j < dstHeight; j++) {

		x = 0.5 + preShiftX;
		dpt = dstData;

		INIT_COEFFS;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (i = 0; i < dstWidth; i++) {

			iwx = (mlib_s32)wx *3;

			iwy = (mlib_s32)wy;

			mask =
			    (~(iwx - 3) & (iwx - srcWidth) & ~(iwy - 1) & (iwy -
			    srcHeight)) >> 31;

			sp = lineAddr[iwy & mask] + (iwx & mask);

			pix0 = sp[0];
			pix1 = sp[1];
			pix2 = sp[2];

			dp = (void *)(((mlib_addr)dpt & mask) | ((mlib_addr)buf
			    & ~mask));

			dp[0] = pix0;
			dp[1] = pix1;
			dp[2] = pix2;

			DELTA;
			dpt += 3;
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_U8_4)
{
	mlib_u8 buf[8], pix0, pix1, pix2, pix3, *dp, *dpt;
	const mlib_u8 *sp;
	mlib_s32 i, j, pos, iwx, iwy, mask, srcStride =
	    lineAddr[2] - lineAddr[1];
	mlib_d64 x, wx, dx, ddx, dddx, ddddx, dddddx;
	mlib_d64 a0, a1, a2, a3, a4, a5;
	mlib_d64 y, wy, dy, ddy, dddy, ddddy, dddddy;
	mlib_d64 b0, b1, b2, b3, b4, b5;

	pos =
	    dstStride | srcStride | (mlib_addr)dstData |
	    (mlib_addr)(lineAddr[0]);

	if ((pos & 1) == 0) {
		CALL_FUNC(mlib_ImagePolynomialWarp_5_NN_S16_2);
		return;
	}

	y = 0.5 + preShiftY;
	srcHeight++;
	srcWidth++;

	for (j = 0; j < dstHeight; j++) {

		x = 0.5 + preShiftX;
		dpt = dstData;

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

			sp = lineAddr[iwy & mask] + ((iwx * 4) & mask);

			pix0 = sp[0];
			pix1 = sp[1];
			pix2 = sp[2];
			pix3 = sp[3];

			dp = (void *)(((mlib_addr)dpt & mask) | ((mlib_addr)buf
			    & ~mask));
			DELTA;

			dp[0] = pix0;
			dp[1] = pix1;
			dp[2] = pix2;
			dp[3] = pix3;
			dpt += 4;
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_S16_1)
{
	mlib_u16 buf[8], pix, *dp, *dpt, *sp;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, dddx, ddddx, dddddx;
	mlib_d64 a0, a1, a2, a3, a4, a5;
	mlib_d64 y, wy, dy, ddy, dddy, ddddy, dddddy;
	mlib_d64 b0, b1, b2, b3, b4, b5;

	y = 0.5 + preShiftY;
	srcHeight++;
	srcWidth++;

	for (j = 0; j < dstHeight; j++) {

		x = 0.5 + preShiftX;
		dpt = (void *)dstData;

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
			    ((iwx * 2) & mask));
			dp = (void *)(((mlib_addr)dpt & mask) | ((mlib_addr)buf
			    & ~mask));

			pix = *sp;
			DELTA;
			*dp = pix;
			dpt++;
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_S16_2)
{
	mlib_u16 buf[8], pix0, pix1, *dp, *dpt, *sp;
	mlib_s32 i, j, pos, iwx, iwy, mask, srcStride =
	    lineAddr[2] - lineAddr[1];
	mlib_d64 x, wx, dx, ddx, dddx, ddddx, dddddx;
	mlib_d64 a0, a1, a2, a3, a4, a5;
	mlib_d64 y, wy, dy, ddy, dddy, ddddy, dddddy;
	mlib_d64 b0, b1, b2, b3, b4, b5;

	pos =
	    dstStride | srcStride | (mlib_addr)dstData |
	    (mlib_addr)(lineAddr[0]);

	if ((pos & 3) == 0) {
		CALL_FUNC(mlib_ImagePolynomialWarp_5_NN_S32_1);
		return;
	}

	y = 0.5 + preShiftY;
	srcHeight++;
	srcWidth++;

	for (j = 0; j < dstHeight; j++) {

		x = 0.5 + preShiftX;
		dpt = (void *)dstData;

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
			    ((iwx * 4) & mask));
			dp = (void *)(((mlib_addr)dpt & mask) | ((mlib_addr)buf
			    & ~mask));

			pix0 = sp[0];
			pix1 = sp[1];

			DELTA;
			dpt += 2;

			dp[0] = pix0;
			dp[1] = pix1;
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_S16_3)
{
	mlib_u16 buf[8], *dp, *sp;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, dddx, ddddx, dddddx;
	mlib_d64 a0, a1, a2, a3, a4, a5;
	mlib_d64 y, wy, dy, ddy, dddy, ddddy, dddddy;
	mlib_d64 b0, b1, b2, b3, b4, b5;

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

			DELTA;

			sp = (void *)(lineAddr[iwy & mask] +
			    ((iwx * 6) & mask));
			dp = (void *)(((mlib_addr)(dstData +
			    6 * i) & mask) | ((mlib_addr)buf & ~mask));
			dp[0] = sp[0];
			dp[1] = sp[1];
			dp[2] = sp[2];
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_S16_4)
{
	mlib_u16 buf[8], pix0, pix1, pix2, pix3, *dp, *dpt, *sp;
	mlib_s32 i, j, pos, iwx, iwy, mask, srcStride =
	    lineAddr[2] - lineAddr[1];
	mlib_d64 x, wx, dx, ddx, dddx, ddddx, dddddx;
	mlib_d64 a0, a1, a2, a3, a4, a5;
	mlib_d64 y, wy, dy, ddy, dddy, ddddy, dddddy;
	mlib_d64 b0, b1, b2, b3, b4, b5;

	pos =
	    dstStride | srcStride | (mlib_addr)dstData |
	    (mlib_addr)(lineAddr[0]);

	if ((pos & 3) == 0) {
		CALL_FUNC(mlib_ImagePolynomialWarp_5_NN_S32_2);
		return;
	}

	y = 0.5 + preShiftY;
	srcHeight++;
	srcWidth++;

	for (j = 0; j < dstHeight; j++) {

		x = 0.5 + preShiftX;
		dpt = (void *)dstData;

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
			dp = (void *)(((mlib_addr)dpt & mask) | ((mlib_addr)buf
			    & ~mask));

			pix0 = sp[0];
			pix1 = sp[1];
			pix2 = sp[2];
			pix3 = sp[3];

			DELTA;
			dpt += 4;

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

DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_S32_1)
{
	mlib_s32 buf[8], pix, *dp, *dpt, *sp;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, dddx, ddddx, dddddx;
	mlib_d64 a0, a1, a2, a3, a4, a5;
	mlib_d64 y, wy, dy, ddy, dddy, ddddy, dddddy;
	mlib_d64 b0, b1, b2, b3, b4, b5;

	y = 0.5 + preShiftY;
	srcHeight++;
	srcWidth++;

	for (j = 0; j < dstHeight; j++) {

		x = 0.5 + preShiftX;
		dpt = (void *)dstData;

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
			    ((iwx * 4) & mask));
			dp = (void *)(((mlib_addr)dpt & mask) | ((mlib_addr)buf
			    & ~mask));

			pix = *sp;

			DELTA;
			dpt++;

			*dp = pix;
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_S32_2)
{
	mlib_s32 buf[8], pix0, pix1, *dp, *dpt, *sp;
	mlib_s32 i, j, iwx, iwy, mask, srcStride = lineAddr[2] - lineAddr[1];
	mlib_d64 x, wx, dx, ddx, dddx, ddddx, dddddx;
	mlib_d64 a0, a1, a2, a3, a4, a5;
	mlib_d64 y, wy, dy, ddy, dddy, ddddy, dddddy;
	mlib_d64 b0, b1, b2, b3, b4, b5;

#ifndef i386
	mlib_s32 pos;

	pos =
	    dstStride | srcStride | (mlib_addr)dstData |
	    (mlib_addr)(lineAddr[0]);

	if ((pos & 7) == 0) {
		CALL_FUNC(mlib_ImagePolynomialWarp_5_NN_D64_1);
		return;
	}
#endif /* i386 */

	y = 0.5 + preShiftY;
	srcHeight++;
	srcWidth++;

	for (j = 0; j < dstHeight; j++) {

		x = 0.5 + preShiftX;
		dpt = (void *)dstData;

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
			dp = (void *)(((mlib_addr)dpt & mask) | ((mlib_addr)buf
			    & ~mask));

			pix0 = sp[0];
			pix1 = sp[1];

			DELTA;
			dpt += 2;

			dp[0] = pix0;
			dp[1] = pix1;
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_S32_3)
{
	mlib_s32 buf[8], *dp, *sp;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, dddx, ddddx, dddddx;
	mlib_d64 a0, a1, a2, a3, a4, a5;
	mlib_d64 y, wy, dy, ddy, dddy, ddddy, dddddy;
	mlib_d64 b0, b1, b2, b3, b4, b5;

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

			DELTA;

			sp = (void *)(lineAddr[iwy & mask] +
			    ((iwx * 12) & mask));
			dp = (void *)(((mlib_addr)(dstData +
			    12 * i) & mask) | ((mlib_addr)buf & ~mask));
			dp[0] = sp[0];
			dp[1] = sp[1];
			dp[2] = sp[2];
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_S32_4)
{
	mlib_s32 buf[8], pix0, pix1, pix2, pix3, *dp, *sp;
	mlib_s32 i, j, iwx, iwy, mask, srcStride = lineAddr[2] - lineAddr[1];
	mlib_d64 x, wx, dx, ddx, dddx, ddddx, dddddx;
	mlib_d64 a0, a1, a2, a3, a4, a5;
	mlib_d64 y, wy, dy, ddy, dddy, ddddy, dddddy;
	mlib_d64 b0, b1, b2, b3, b4, b5;

#ifndef i386
	mlib_s32 pos;

	pos =
	    dstStride | srcStride | (mlib_addr)dstData |
	    (mlib_addr)(lineAddr[0]);

	if ((pos & 7) == 0) {
		CALL_FUNC(mlib_ImagePolynomialWarp_5_NN_D64_2);
		return;
	}
#endif /* i386 */

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
			pix2 = sp[2];
			pix3 = sp[3];

			DELTA;

			dp = (void *)(((mlib_addr)(dstData +
			    16 * i) & mask) | ((mlib_addr)buf & ~mask));
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
