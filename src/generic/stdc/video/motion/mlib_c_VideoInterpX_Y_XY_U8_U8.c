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

#pragma ident	"@(#)mlib_c_VideoInterpX_Y_XY_U8_U8.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoInterpX_Y_XY_U8_U8 - Performs half-pixel interpolation in
 *                                     both X and Y directions
 *                                     for replenishment mode
 *
 * SYNOPSIS
 *    mlib_status mlib_VideoInterpX_Y_XY_U8_U8(mlib_u8       *outputX,
 *                                             mlib_u8       *outputY,
 *                                             mlib_u8       *outputXY,
 *                                             const mlib_u8 *image,
 *                                             mlib_s32      stride,
 *                                             mlib_s32      width,
 *                                             mlib_s32      height)
 * ARGUMENT
 *      outputX    Pointer to output of X-interpolation, must be 8-byte aligned
 *      outputY    Pointer to output of Y-interpolation, must be 8-byte aligned
 *      outputXY   Pointer to output of XY-interpolation, must be 8-byte aligned
 *      image      Pointer to image data, must be 8-byte aligned
 *      stride     Stride in bytes between rows in image,
 *                 must be a multiple of 8
 *      width      Width of image, must be a multiple of 8
 *      height     Height of image, must be a multiple of 2
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoInterpX_Y_XY_U8_U8 = __mlib_VideoInterpX_Y_XY_U8_U8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoInterpX_Y_XY_U8_U8) mlib_VideoInterpX_Y_XY_U8_U8
	__attribute__((weak, alias("__mlib_VideoInterpX_Y_XY_U8_U8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static mlib_status mlib_c_VideoInterpX_Y_XY_U8_U8_w16(
	mlib_u8 *outputX,
	mlib_u8 *outputY,
	mlib_u8 *outputXY,
	const mlib_u8 *image,
	mlib_s32 stride,
	mlib_s32 height);

static mlib_status mlib_c_VideoInterpX_Y_XY_U8_U8_comm(
	mlib_u8 *outputX,
	mlib_u8 *outputY,
	mlib_u8 *outputXY,
	const mlib_u8 *image,
	mlib_s32 stride,
	mlib_s32 width,
	mlib_s32 height);

/* *********************************************************** */

mlib_status
mlib_c_VideoInterpX_Y_XY_U8_U8_comm(
	mlib_u8 *outputX,
	mlib_u8 *outputY,
	mlib_u8 *outputXY,
	const mlib_u8 *image,
	mlib_s32 stride,
	mlib_s32 width,
	mlib_s32 height)
{
	const mlib_u8 *sl, *sp;
	mlib_u8 *dlX, *dlY, *dlZ;
	mlib_s32 a0, a1, a2, b1, b2, x0, x1, y0, y1, y2;
	mlib_s32 i, j;

	sl = image;
	dlX = outputX;
	dlY = outputY;
	dlZ = outputXY;

/* main image */
	for (j = 0; j < height - 1; j++) {
		sp = sl;
		y2 = sp[0] + sp[stride] + 1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i <= (width - 4); i += 2) {
			a0 = sp[0];
			a1 = sp[1];
			a2 = sp[2];
			b1 = sp[stride + 1];
			b2 = sp[stride + 2];

			x0 = a0 + a1 + 1;
			x1 = a1 + a2 + 1;
			y0 = y2;
			y1 = a1 + b1 + 1;
			y2 = a2 + b2 + 1;

/* *(mlib_u16*)(dlX + i) = ((x0 << 7) & 0xFF00) | (x1 >> 1); */
			dlX[i] = x0 >> 1;
			dlX[i + 1] = x1 >> 1;
			dlY[i] = y0 >> 1;
			dlY[i + 1] = y1 >> 1;
			dlZ[i] = (y0 + y1) >> 2;
			dlZ[i + 1] = (y1 + y2) >> 2;

			sp += 2;
		}

/* last pels on each line */
		a0 = sl[width - 2];
		a1 = sl[width - 1];
		b1 = sl[stride + width - 1];

		x0 = a0 + a1 + 1;
		y1 = a1 + b1 + 1;

		dlX[width - 2] = x0 >> 1;
		dlY[width - 2] = y2 >> 1;
		dlZ[width - 2] = (y2 + y1) >> 2;
		dlX[width - 1] = a1;
		dlY[width - 1] =
		dlZ[width - 1] = y1 >> 1;

		dlX += stride;
		dlY += stride;
		dlZ += stride;

		sl += stride;
	}

/* last lines */
	for (i = 0; i < width - 1; i++) {
		dlX[i] = (sl[i] + sl[i + 1] + 1) >> 1;
		dlY[i] = sl[i];
		dlZ[i] = (sl[i] + sl[i + 1] + 1) >> 1;
	}

/* bottom right corner pels */
	dlX[width - 1] = sl[width - 1];
	dlY[width - 1] = sl[width - 1];
	dlZ[width - 1] = sl[width - 1];

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpX_Y_XY_U8_U8(
	mlib_u8 *outputX,
	mlib_u8 *outputY,
	mlib_u8 *outputXY,
	const mlib_u8 *image,
	mlib_s32 stride,
	mlib_s32 width,
	mlib_s32 height)
{
	const mlib_u8 *sl;
	mlib_u8 *dlX, *dlY, *dlZ;
	mlib_s32 a0, a1, a2, a3, a4, a5, a6, a7;
	mlib_s32 b0, b1, b2, b3, b4, b5, b6, b7;
	mlib_s32 x0, x1, x2, x3, x4, x5, x6;
	mlib_s32 y0, y1, y2, y3, y4, y5, y6, y7;
	mlib_s32 j;

	if (width != 8) {
		if (width == 16) {
			return mlib_c_VideoInterpX_Y_XY_U8_U8_w16(outputX,
				outputY, outputXY, image, stride, height);
		}

		return mlib_c_VideoInterpX_Y_XY_U8_U8_comm(outputX, outputY,
			outputXY, image, stride, width, height);
	}

	sl = image;
	dlX = outputX;
	dlY = outputY;
	dlZ = outputXY;

	b0 = sl[0];
	b1 = sl[1];
	b2 = sl[2];
	b3 = sl[3];
	b4 = sl[4];
	b5 = sl[5];
	b6 = sl[6];
	b7 = sl[7];

/* main image */
	for (j = 0; j < height - 1; j++) {
		a0 = b0;
		b0 = sl[stride];
		a1 = b1;
		b1 = sl[stride + 1];
		a2 = b2;
		b2 = sl[stride + 2];
		a3 = b3;
		b3 = sl[stride + 3];
		a4 = b4;
		b4 = sl[stride + 4];
		a5 = b5;
		b5 = sl[stride + 5];
		a6 = b6;
		b6 = sl[stride + 6];
		a7 = b7;
		b7 = sl[stride + 7];

		y0 = a0 + b0 + 1;
		y1 = a1 + b1 + 1;
		y2 = a2 + b2 + 1;
		y3 = a3 + b3 + 1;
		y4 = a4 + b4 + 1;
		y5 = a5 + b5 + 1;
		y6 = a6 + b6 + 1;
		y7 = a7 + b7 + 1;

		dlX[0] = (a0 + a1 + 1) >> 1;
		dlX[1] = (a1 + a2 + 1) >> 1;
		dlX[2] = (a2 + a3 + 1) >> 1;
		dlX[3] = (a3 + a4 + 1) >> 1;
		dlX[4] = (a4 + a5 + 1) >> 1;
		dlX[5] = (a5 + a6 + 1) >> 1;
		dlX[6] = (a6 + a7 + 1) >> 1;
		dlX[7] = a7;
		dlY[0] = y0 >> 1;
		dlY[1] = y1 >> 1;
		dlY[2] = y2 >> 1;
		dlY[3] = y3 >> 1;
		dlY[4] = y4 >> 1;
		dlY[5] = y5 >> 1;
		dlY[6] = y6 >> 1;
		dlY[7] = y7 >> 1;
		dlZ[0] = (y0 + y1) >> 2;
		dlZ[1] = (y1 + y2) >> 2;
		dlZ[2] = (y2 + y3) >> 2;
		dlZ[3] = (y3 + y4) >> 2;
		dlZ[4] = (y4 + y5) >> 2;
		dlZ[5] = (y5 + y6) >> 2;
		dlZ[6] = (y6 + y7) >> 2;
		dlZ[7] = y7 >> 1;

		dlX += stride;
		dlY += stride;
		dlZ += stride;

		sl += stride;
	}

/* last line */
	x0 = (b0 + b1 + 1) >> 1;
	x1 = (b1 + b2 + 1) >> 1;
	x2 = (b2 + b3 + 1) >> 1;
	x3 = (b3 + b4 + 1) >> 1;
	x4 = (b4 + b5 + 1) >> 1;
	x5 = (b5 + b6 + 1) >> 1;
	x6 = (b6 + b7 + 1) >> 1;

	dlX[0] = x0;
	dlX[1] = x1;
	dlX[2] = x2;
	dlX[3] = x3;
	dlX[4] = x4;
	dlX[5] = x5;
	dlX[6] = x6;
	dlX[7] = b7;
	dlY[0] = b0;
	dlY[1] = b1;
	dlY[2] = b2;
	dlY[3] = b3;
	dlY[4] = b4;
	dlY[5] = b5;
	dlY[6] = b6;
	dlY[7] = b7;
	dlZ[0] = x0;
	dlZ[1] = x1;
	dlZ[2] = x2;
	dlZ[3] = x3;
	dlZ[4] = x4;
	dlZ[5] = x5;
	dlZ[6] = x6;
	dlZ[7] = b7;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_VideoInterpX_Y_XY_U8_U8_w16(
	mlib_u8 *outputX,
	mlib_u8 *outputY,
	mlib_u8 *outputXY,
	const mlib_u8 *image,
	mlib_s32 stride,
	mlib_s32 height)
{
	const mlib_u8 *sl;
	mlib_u8 *dlX, *dlY, *dlZ;
	mlib_s32 a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF;
	mlib_s32 b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, bA, bB, bC, bD, bE, bF;
	mlib_s32 x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, xA, xB, xC, xD, xE;
	mlib_s32 y0, y1, y2, y3, y4, y5, y6, y7, y8, y9, yA, yB, yC, yD, yE, yF;
	mlib_s32 j;

	sl = image;
	dlX = outputX;
	dlY = outputY;
	dlZ = outputXY;

/* main image */
	for (j = 0; j < height - 1; j++) {
		a0 = sl[0];
		b0 = sl[stride];
		a1 = sl[1];
		b1 = sl[stride + 1];
		a2 = sl[2];
		b2 = sl[stride + 2];
		a3 = sl[3];
		b3 = sl[stride + 3];
		a4 = sl[4];
		b4 = sl[stride + 4];
		a5 = sl[5];
		b5 = sl[stride + 5];
		a6 = sl[6];
		b6 = sl[stride + 6];
		a7 = sl[7];
		b7 = sl[stride + 7];
		a8 = sl[8];
		b8 = sl[stride + 8];
		a9 = sl[9];
		b9 = sl[stride + 9];
		aA = sl[10];
		bA = sl[stride + 10];
		aB = sl[11];
		bB = sl[stride + 11];
		aC = sl[12];
		bC = sl[stride + 12];
		aD = sl[13];
		bD = sl[stride + 13];
		aE = sl[14];
		bE = sl[stride + 14];
		aF = sl[15];
		bF = sl[stride + 15];

		y0 = a0 + b0 + 1;
		y1 = a1 + b1 + 1;
		y2 = a2 + b2 + 1;
		y3 = a3 + b3 + 1;
		y4 = a4 + b4 + 1;
		y5 = a5 + b5 + 1;
		y6 = a6 + b6 + 1;
		y7 = a7 + b7 + 1;
		y8 = a8 + b8 + 1;
		y9 = a9 + b9 + 1;
		yA = aA + bA + 1;
		yB = aB + bB + 1;
		yC = aC + bC + 1;
		yD = aD + bD + 1;
		yE = aE + bE + 1;
		yF = aF + bF + 1;

		dlX[0] = (a0 + a1 + 1) >> 1;
		dlX[1] = (a1 + a2 + 1) >> 1;
		dlX[2] = (a2 + a3 + 1) >> 1;
		dlX[3] = (a3 + a4 + 1) >> 1;
		dlX[4] = (a4 + a5 + 1) >> 1;
		dlX[5] = (a5 + a6 + 1) >> 1;
		dlX[6] = (a6 + a7 + 1) >> 1;
		dlX[7] = (a7 + a8 + 1) >> 1;
		dlX[8] = (a8 + a9 + 1) >> 1;
		dlX[9] = (a9 + aA + 1) >> 1;
		dlX[10] = (aA + aB + 1) >> 1;
		dlX[11] = (aB + aC + 1) >> 1;
		dlX[12] = (aC + aD + 1) >> 1;
		dlX[13] = (aD + aE + 1) >> 1;
		dlX[14] = (aE + aF + 1) >> 1;
		dlX[15] = aF;
		dlY[0] = y0 >> 1;
		dlY[1] = y1 >> 1;
		dlY[2] = y2 >> 1;
		dlY[3] = y3 >> 1;
		dlY[4] = y4 >> 1;
		dlY[5] = y5 >> 1;
		dlY[6] = y6 >> 1;
		dlY[7] = y7 >> 1;
		dlY[8] = y8 >> 1;
		dlY[9] = y9 >> 1;
		dlY[10] = yA >> 1;
		dlY[11] = yB >> 1;
		dlY[12] = yC >> 1;
		dlY[13] = yD >> 1;
		dlY[14] = yE >> 1;
		dlY[15] = yF >> 1;
		dlZ[0] = (y0 + y1) >> 2;
		dlZ[1] = (y1 + y2) >> 2;
		dlZ[2] = (y2 + y3) >> 2;
		dlZ[3] = (y3 + y4) >> 2;
		dlZ[4] = (y4 + y5) >> 2;
		dlZ[5] = (y5 + y6) >> 2;
		dlZ[6] = (y6 + y7) >> 2;
		dlZ[7] = (y7 + y8) >> 2;
		dlZ[8] = (y8 + y9) >> 2;
		dlZ[9] = (y9 + yA) >> 2;
		dlZ[10] = (yA + yB) >> 2;
		dlZ[11] = (yB + yC) >> 2;
		dlZ[12] = (yC + yD) >> 2;
		dlZ[13] = (yD + yE) >> 2;
		dlZ[14] = (yE + yF) >> 2;
		dlZ[15] = yF >> 1;

		dlX += stride;
		dlY += stride;
		dlZ += stride;

		sl += stride;
	}

/* last line */
	a0 = sl[0];
	a1 = sl[1];
	a2 = sl[2];
	a3 = sl[3];
	a4 = sl[4];
	a5 = sl[5];
	a6 = sl[6];
	a7 = sl[7];
	a8 = sl[8];
	a9 = sl[9];
	aA = sl[10];
	aB = sl[11];
	aC = sl[12];
	aD = sl[13];
	aE = sl[14];
	aF = sl[15];

	x0 = (a0 + a1 + 1) >> 1;
	x1 = (a1 + a2 + 1) >> 1;
	x2 = (a2 + a3 + 1) >> 1;
	x3 = (a3 + a4 + 1) >> 1;
	x4 = (a4 + a5 + 1) >> 1;
	x5 = (a5 + a6 + 1) >> 1;
	x6 = (a6 + a7 + 1) >> 1;
	x7 = (a7 + a8 + 1) >> 1;
	x8 = (a8 + a9 + 1) >> 1;
	x9 = (a9 + aA + 1) >> 1;
	xA = (aA + aB + 1) >> 1;
	xB = (aB + aC + 1) >> 1;
	xC = (aC + aD + 1) >> 1;
	xD = (aD + aE + 1) >> 1;
	xE = (aE + aF + 1) >> 1;

	dlX[0] = x0;
	dlX[1] = x1;
	dlX[2] = x2;
	dlX[3] = x3;
	dlX[4] = x4;
	dlX[5] = x5;
	dlX[6] = x6;
	dlX[7] = x7;
	dlX[8] = x8;
	dlX[9] = x9;
	dlX[10] = xA;
	dlX[11] = xB;
	dlX[12] = xC;
	dlX[13] = xD;
	dlX[14] = xE;
	dlX[15] = aF;

	dlY[0] = a0;
	dlY[1] = a1;
	dlY[2] = a2;
	dlY[3] = a3;
	dlY[4] = a4;
	dlY[5] = a5;
	dlY[6] = a6;
	dlY[7] = a7;
	dlY[8] = a8;
	dlY[9] = a9;
	dlY[10] = aA;
	dlY[11] = aB;
	dlY[12] = aC;
	dlY[13] = aD;
	dlY[14] = aE;
	dlY[15] = aF;

	dlZ[0] = x0;
	dlZ[1] = x1;
	dlZ[2] = x2;
	dlZ[3] = x3;
	dlZ[4] = x4;
	dlZ[5] = x5;
	dlZ[6] = x6;
	dlZ[7] = x7;
	dlZ[8] = x8;
	dlZ[9] = x9;
	dlZ[10] = xA;
	dlZ[11] = xB;
	dlZ[12] = xC;
	dlZ[13] = xD;
	dlZ[14] = xE;
	dlZ[15] = aF;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
