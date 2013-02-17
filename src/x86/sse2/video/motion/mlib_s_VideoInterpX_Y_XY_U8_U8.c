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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_VideoInterpX_Y_XY_U8_U8.c	9.2	07/11/05 SMI"

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

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

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

	mlib_s32 dw = (width & 0xF);
	sl = image;
	dlX = outputX;
	dlY = outputY;
	dlZ = outputXY;

	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm6, txmm7;
	__m128i *d0, *d1, *d2;
/* main image */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (j = 0; j < height - 1; j++) {
		sp = sl;

		d0 = (__m128i *)dlX;
		d1 = (__m128i *)dlY;
		d2 = (__m128i *)dlZ;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < (width - dw); i += 16) {
			txmm0 = _mm_loadu_si128((__m128i *)sp);
			txmm1 = _mm_loadu_si128((__m128i *)(sp + stride));
			txmm2 = _mm_loadu_si128((__m128i *)(sp + 1));
			txmm3 = _mm_loadu_si128((__m128i *)(sp + stride + 1));
			txmm5 = _mm_avg_epu8(txmm0, txmm2);
			_mm_storeu_si128((__m128i *)d0++, txmm5);
			txmm4 = _mm_avg_epu8(txmm0, txmm1);
			_mm_storeu_si128((__m128i *)d1++, txmm4);
			txmm2 = _mm_avg_epu8(txmm2, txmm3);
			txmm3 = _mm_avg_epu8(txmm2, txmm4);
			_mm_storeu_si128((__m128i *)d2++, txmm3);
			sp += 16;

		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < dw; i += 8) {
			txmm0 = _mm_loadu_si128((__m128i *)sp);
			txmm1 = _mm_loadu_si128((__m128i *)(sp + stride));
			txmm2 = _mm_loadu_si128((__m128i *)(sp + 1));
			txmm3 = _mm_loadu_si128((__m128i *)(sp + stride + 1));
			txmm5 = _mm_avg_epu8(txmm0, txmm2);
			_mm_storel_epi64((__m128i *)d0, txmm5);
			txmm4 = _mm_avg_epu8(txmm0, txmm1);
			_mm_storel_epi64((__m128i *)d1, txmm4);
			txmm2 = _mm_avg_epu8(txmm2, txmm3);
			txmm3 = _mm_avg_epu8(txmm2, txmm4);
			_mm_storel_epi64((__m128i *)d2, txmm3);
		}
/* last pels on each line */
		a0 = sl[width - 2];
		a1 = sl[width - 1];
		b1 = sl[stride + width - 1];

		x0 = a0 + a1 + 1;
		y1 = a1 + b1 + 1;

		dlX[width - 2] = x0 >> 1;
		/* delete for having been written */
		/* dlY[width - 2] = y2 >> 1;  */
		/* dlZ[width - 2] = (y2 + y1) >> 2; */
		dlX[width - 1] = a1;
		dlY[width - 1] =
		dlZ[width - 1] = y1 >> 1;
		dlX += stride;
		dlY += stride;
		dlZ += stride;

		sl += stride;
	}

/* last lines */
		d0 = (__m128i *)dlX;
		d1 = (__m128i *)dlY;
		d2 = (__m128i *)dlZ;
		sp = sl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < (width - dw); i += 16) {
			txmm0 = _mm_loadu_si128((__m128i *)sp);
			_mm_storeu_si128((__m128i *)d1++, txmm0);
			txmm2 = _mm_loadu_si128((__m128i *)(sp + 1));
			txmm5 = _mm_avg_epu8(txmm0, txmm2);
			_mm_storeu_si128((__m128i *)d0++, txmm5);
			_mm_storeu_si128((__m128i *)d2++, txmm5);
			sp += 16;

		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < dw; i += 8) {
			txmm0 = _mm_loadu_si128((__m128i *)sp);
			_mm_storel_epi64((__m128i *)d1, txmm0);
			txmm2 = _mm_loadu_si128((__m128i *)(sp + 1));
			txmm5 = _mm_avg_epu8(txmm0, txmm2);
			_mm_storel_epi64((__m128i *)d0, txmm5);
			_mm_storel_epi64((__m128i *)d2, txmm5);
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
	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm6, txmm7;

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

	txmm7 = _mm_set_epi32(0, 0, 0xFF000000, 0);
	__m128i Mask = _mm_set_epi32(0, 0, 0xFFFFFFFF, 0xFFFFFFFF);
/* main image */
	for (j = 0; j < height - 1; j++) {
	txmm0 = _mm_loadu_si128((__m128i *)sl);
	txmm1 = _mm_loadu_si128((__m128i *)(sl + stride));
	txmm3 = _mm_and_si128(txmm0, txmm7);
	txmm4 = _mm_and_si128(Mask, txmm0);
	txmm4 = _mm_srli_si128(txmm4, 1);
	txmm2 = _mm_or_si128(txmm3, txmm4);
	txmm5 = _mm_avg_epu8(txmm0, txmm2);
	_mm_storel_epi64((__m128i *)dlX, txmm5);
	txmm6 = _mm_avg_epu8(txmm0, txmm1);
	_mm_storel_epi64((__m128i *)dlY, txmm6);
	txmm3 = _mm_and_si128(txmm6, txmm7);
	txmm4 = _mm_and_si128(Mask, txmm6);
	txmm4 = _mm_srli_si128(txmm4, 1);
	txmm2 = _mm_or_si128(txmm3, txmm4);
	txmm0 = _mm_avg_epu8(txmm6, txmm2);
	_mm_storel_epi64((__m128i *)dlZ, txmm0);

	dlX += stride;
	dlY += stride;
	dlZ += stride;

	sl += stride;
	}
/* last line */
	txmm0 = _mm_loadu_si128((__m128i *)sl);
	_mm_storel_epi64((__m128i *)dlY, txmm0);
	txmm3 = _mm_and_si128(txmm0, txmm7);
	txmm4 = _mm_and_si128(Mask, txmm0);
	txmm4 = _mm_srli_si128(txmm4, 1);
	txmm2 = _mm_or_si128(txmm3, txmm4);
	txmm5 = _mm_avg_epu8(txmm0, txmm2);
	_mm_storel_epi64((__m128i *)dlX, txmm5);
	_mm_storel_epi64((__m128i *)dlZ, txmm5);

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
	mlib_s32 j;

	sl = image;
	dlX = outputX;
	dlY = outputY;
	dlZ = outputXY;

	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm6, txmm7;

	txmm7 = _mm_set_epi32(0xFF000000, 0, 0, 0);
/* main image */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (j = 0; j < height - 1; j++) {
		txmm0 = _mm_loadu_si128((__m128i *)sl);
		txmm1 = _mm_loadu_si128((__m128i *)(sl + stride));
		txmm3 = _mm_and_si128(txmm0, txmm7);
		txmm4 = _mm_srli_si128(txmm0, 1);
		txmm2 = _mm_or_si128(txmm3, txmm4);
		txmm5 = _mm_avg_epu8(txmm0, txmm2);
		_mm_storeu_si128((__m128i *)dlX, txmm5);
		txmm6 = _mm_avg_epu8(txmm0, txmm1);
		_mm_storeu_si128((__m128i *)dlY, txmm6);
		txmm3 = _mm_and_si128(txmm6, txmm7);
		txmm4 = _mm_srli_si128(txmm6, 1);
		txmm2 = _mm_or_si128(txmm3, txmm4);
		txmm0 = _mm_avg_epu8(txmm6, txmm2);
		_mm_storeu_si128((__m128i *)dlZ, txmm0);
		dlX += stride;
		dlY += stride;
		dlZ += stride;
		sl += stride;
	}

/* last line */
	txmm0 = _mm_loadu_si128((__m128i *)sl);
	_mm_storeu_si128((__m128i *)dlY, txmm0);
	txmm3 = _mm_and_si128(txmm0, txmm7);
	txmm4 = _mm_srli_si128(txmm0, 1);
	txmm2 = _mm_or_si128(txmm3, txmm4);
	txmm5 = _mm_avg_epu8(txmm0, txmm2);
	_mm_storeu_si128((__m128i *)dlX, txmm5);
	_mm_storeu_si128((__m128i *)dlZ, txmm5);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
