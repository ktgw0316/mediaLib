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

#pragma ident	"@(#)mlib_ImagePolynomialWarp_NN_2.c	9.2	07/10/09 SMI"

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
	ddy = 2 * b2

/* *********************************************************** */

#define	CALL_FUNC(NAME)                                                   \
	NAME(dstData, lineAddr, xCoeffs, yCoeffs, preShiftX, preShiftY,   \
	    srcWidth, srcHeight, dstWidth, dstHeight, dstStride)

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_U8_1)
{
	mlib_u8 buf[8], pix, *dp, *dpt;
	const mlib_u8 *sp;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, a0, a1, a2, y, wy, dy, ddy, b0, b1, b2;

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

			wx += dx;
			dx += ddx;
			wy += dy;
			dy += ddy;

			*dp = pix;
			dpt++;
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_U8_2)
{
	mlib_u8 buf[8], pix1, *dp;
	const mlib_u8 *sp;
	mlib_s32 i, j, pos, iwx, iwy, mask, srcStride =
	    lineAddr[2] - lineAddr[1];
	mlib_d64 x, wx, dx, ddx, a0, a1, a2, y, wy, dy, ddy, b0, b1, b2;

	pos =
	    dstStride | srcStride | (mlib_addr)dstData |
	    (mlib_addr)(lineAddr[0]);

	if ((pos & 1) == 0) {
		CALL_FUNC(mlib_ImagePolynomialWarp_2_NN_S16_1);
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

			wx += dx;
			dx += ddx;
			wy += dy;
			dy += ddy;

			dp[0] = sp[0];
			dp[1] = pix1;
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_U8_3)
{
	mlib_u8 buf[8], pix0, pix1, pix2, *dp, *dpt;
	const mlib_u8 *sp;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, a0, a1, a2, y, wy, dy, ddy, b0, b1, b2;

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

			wx += dx;
			dx += ddx;
			wy += dy;
			dy += ddy;
			dpt += 3;
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_U8_4)
{
	mlib_u8 buf[8], pix0, pix1, pix2, pix3, *dp, *dpt;
	const mlib_u8 *sp;
	mlib_s32 i, j, pos, iwx, iwy, mask, srcStride =
	    lineAddr[2] - lineAddr[1];
	mlib_d64 x, wx, dx, ddx, a0, a1, a2, y, wy, dy, ddy, b0, b1, b2;

	pos =
	    dstStride | srcStride | (mlib_addr)dstData |
	    (mlib_addr)(lineAddr[0]);

	if ((pos & 1) == 0) {
		CALL_FUNC(mlib_ImagePolynomialWarp_2_NN_S16_2);
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
			wx += dx;
			dx += ddx;
			wy += dy;
			dy += ddy;

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

DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_S16_1)
{
	mlib_u16 buf[8], pix, *dp, *dpt, *sp;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, a0, a1, a2, y, wy, dy, ddy, b0, b1, b2;

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
			wx += dx;
			dx += ddx;
			wy += dy;
			dy += ddy;
			*dp = pix;
			dpt++;
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_S16_2)
{
	mlib_u16 buf[8], pix0, pix1, *dp, *dpt, *sp;
	mlib_s32 i, j, pos, iwx, iwy, mask, srcStride =
	    lineAddr[2] - lineAddr[1];
	mlib_d64 x, wx, dx, ddx, a0, a1, a2, y, wy, dy, ddy, b0, b1, b2;

	pos =
	    dstStride | srcStride | (mlib_addr)dstData |
	    (mlib_addr)(lineAddr[0]);

	if ((pos & 3) == 0) {
		CALL_FUNC(mlib_ImagePolynomialWarp_2_NN_S32_1);
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

			wx += dx;
			dx += ddx;
			wy += dy;
			dy += ddy;
			dpt += 2;

			dp[0] = pix0;
			dp[1] = pix1;
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_S16_3)
{
	mlib_u16 buf[8], *dp, *sp;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, a0, a1, a2, y, wy, dy, ddy, b0, b1, b2;

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
			wy += dy;
			dy += ddy;

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

DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_S16_4)
{
	mlib_u16 buf[8], pix0, pix1, pix2, pix3, *dp, *dpt, *sp;
	mlib_s32 i, j, pos, iwx, iwy, mask, srcStride =
	    lineAddr[2] - lineAddr[1];
	mlib_d64 x, wx, dx, ddx, a0, a1, a2, y, wy, dy, ddy, b0, b1, b2;

	pos =
	    dstStride | srcStride | (mlib_addr)dstData |
	    (mlib_addr)(lineAddr[0]);

	if ((pos & 3) == 0) {
		CALL_FUNC(mlib_ImagePolynomialWarp_2_NN_S32_2);
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

			wx += dx;
			dx += ddx;
			wy += dy;
			dy += ddy;
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

DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_S32_1)
{
	mlib_s32 buf[8], pix, *dp, *dpt, *sp;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, a0, a1, a2, y, wy, dy, ddy, b0, b1, b2;

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

			wx += dx;
			dx += ddx;
			wy += dy;
			dy += ddy;
			dpt++;

			*dp = pix;
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_S32_2)
{
	mlib_s32 buf[8], pix0, pix1, *dp, *dpt, *sp;
	mlib_s32 i, j, iwx, iwy, mask, srcStride = lineAddr[2] - lineAddr[1];
	mlib_d64 x, wx, dx, ddx, a0, a1, a2, y, wy, dy, ddy, b0, b1, b2;

#ifndef i386
	mlib_s32 pos;

	pos =
	    dstStride | srcStride | (mlib_addr)dstData |
	    (mlib_addr)(lineAddr[0]);

	if ((pos & 7) == 0) {
		CALL_FUNC(mlib_ImagePolynomialWarp_2_NN_D64_1);
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

			wx += dx;
			dx += ddx;
			wy += dy;
			dy += ddy;
			dpt += 2;

			dp[0] = pix0;
			dp[1] = pix1;
		}

		y += 1.0;
		dstData += dstStride;
	}
}

/* *********************************************************** */

DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_S32_3)
{
	mlib_s32 buf[8], *dp, *sp;
	mlib_s32 i, j, iwx, iwy, mask;
	mlib_d64 x, wx, dx, ddx, a0, a1, a2, y, wy, dy, ddy, b0, b1, b2;

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
			wy += dy;
			dy += ddy;

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

DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_S32_4)
{
	mlib_s32 buf[8], pix0, pix1, pix2, pix3, *dp, *sp;
	mlib_s32 i, j, iwx, iwy, mask, srcStride = lineAddr[2] - lineAddr[1];
	mlib_d64 x, wx, dx, ddx, a0, a1, a2, y, wy, dy, ddy, b0, b1, b2;

#ifndef i386
	mlib_s32 pos;

	pos =
	    dstStride | srcStride | (mlib_addr)dstData |
	    (mlib_addr)(lineAddr[0]);

	if ((pos & 7) == 0) {
		CALL_FUNC(mlib_ImagePolynomialWarp_2_NN_D64_2);
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

			wx += dx;
			dx += ddx;
			wy += dy;
			dy += ddy;

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
