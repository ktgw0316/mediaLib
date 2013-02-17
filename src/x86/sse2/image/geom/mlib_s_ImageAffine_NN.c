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

#pragma ident	"@(#)mlib_s_ImageAffine_NN.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageAffine_u8_1ch_nn
 *      mlib_ImageAffine_u8_2ch_nn
 *      mlib_ImageAffine_u8_3ch_nn
 *      mlib_ImageAffine_u8_4ch_nn
 *      mlib_ImageAffine_s16_1ch_nn
 *      mlib_ImageAffine_s16_2ch_nn
 *      mlib_ImageAffine_s16_3ch_nn
 *      mlib_ImageAffine_s16_4ch_nn
 *        - image affine transformation with Nearest Neighbor filtering
 * SYNOPSIS
 *      mlib_status mlib_ImageAffine_[u8|s16]_?ch_nn(mlib_s32 *leftEdges,
 *                                                   mlib_s32 *rightEdges,
 *                                                   mlib_s32 *xStarts,
 *                                                   mlib_s32 *yStarts,
 *                                                   mlib_s32 *sides,
 *                                                   mlib_u8  *dstData,
 *                                                   mlib_u8  **lineAddr,
 *                                                   mlib_s32 dstYStride,
 *                                                   mlib_s32 is_affine)
 *
 * ARGUMENTS
 *      leftEdges  array[dstHeight] of xLeft coordinates
 *      RightEdges array[dstHeight] of xRight coordinates
 *      xStarts    array[dstHeight] of xStart * 65536 coordinates
 *      yStarts    array[dstHeight] of yStart * 65536 coordinates
 *      sides      output array[4]. sides[0] is yStart, sides[1] is yFinish,
 *                 sides[2] is dx * 65536, sides[3] is dy * 65536
 *      dstData    pointer to the first pixel on (yStart - 1) line
 *      lineAddr   array[srcHeight] of pointers to the first pixel on
 *                 the corresponding lines
 *      dstYStride stride of destination image
 *      is_affine  indicator (Affine - GridWarp)
 *
 * DESCRIPTION
 *      The functions step along the lines from xLeft to xRight and get the
 *      nearest pixel values as being with the following coordinates
 *      ((xStart - (i - xLeft) * dx) >> 16, (yStart - (i - xLeft) * dy) >> 16)
 *
 */

#ifdef	__SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif
#include <mlib_ImageAffine.h>

/* *********************************************************** */

#define	DECLAREVAR_NN1()                                          \
	DECLAREVAR_NN();                                          \
	DTYPE *dstLineEnd;                                        \
	__m128i Stride128;                                        \
	mlib_s32 srcYStride = lineAddr[1] - lineAddr[0];          \
	DTYPE *srcPixelPtr0 = MLIB_POINTER_GET(lineAddr, 0)

/* *********************************************************** */

#undef  DTYPE
#define	DTYPE	mlib_u8

mlib_status
mlib_ImageAffine_u8_1ch_nn(
	mlib_affine_param *param)
{
	DECLAREVAR_NN1();

	if (srcYStride <= 0xFFFF) {

		Stride128 = _mm_set1_epi32(((srcYStride) & 0xFFFF) << 16);

		for (j = yStart; j <= yFinish; j++) {
			mlib_s32 ind0, ind1, ind2, ind3;
			__m128i dx128, dy128, x128, y128;
			__m128i tmp0, tmp1, tmp2;

			CLIP(1);
			dstLineEnd = (DTYPE *) dstData + xRight;

			dx128 = _mm_set1_epi32(dX << 2);
			dy128 = _mm_set1_epi32(dY << 2);
			x128 = _mm_setr_epi32(
					X, X + dX, X + 2 * dX, X + 3 * dX);
			y128 = _mm_setr_epi32(
					Y, Y + dY, Y + 2 * dY, Y + 3 * dY);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; dstPixelPtr <= (dstLineEnd - 4);
					dstPixelPtr += 4) {
				tmp0 = _mm_srai_epi32(x128, MLIB_SHIFT);
				tmp1 = _mm_madd_epi16(y128, Stride128);
				tmp2 = _mm_add_epi32(tmp0, tmp1);

				ind0 = _mm_cvtsi128_si32(tmp2);
				tmp2 = _mm_srli_si128(tmp2, 4);
				ind1 = _mm_cvtsi128_si32(tmp2);
				tmp2 = _mm_srli_si128(tmp2, 4);
				ind2 = _mm_cvtsi128_si32(tmp2);
				tmp2 = _mm_srli_si128(tmp2, 4);
				ind3 = _mm_cvtsi128_si32(tmp2);

				dstPixelPtr[0] = srcPixelPtr0[ind0];
				dstPixelPtr[1] = srcPixelPtr0[ind1];
				dstPixelPtr[2] = srcPixelPtr0[ind2];
				dstPixelPtr[3] = srcPixelPtr0[ind3];

				x128 = _mm_add_epi32(x128, dx128);
				y128 = _mm_add_epi32(y128, dy128);
			}

			X = _mm_cvtsi128_si32(x128);
			Y = _mm_cvtsi128_si32(y128);

			for (; dstPixelPtr <= dstLineEnd; dstPixelPtr++) {
				ySrc = MLIB_POINTER_SHIFT(Y);
				Y += dY;
				srcPixelPtr = MLIB_POINTER_GET(lineAddr, ySrc);
				xSrc = X >> MLIB_SHIFT;
				X += dX;
				dstPixelPtr[0] = srcPixelPtr[xSrc];
			}
		}
	} else {
		for (j = yStart; j <= yFinish; j++) {
			__m128i pix128;
			mlib_s32 xSrc0, xSrc1, xSrc2, xSrc3;
			mlib_s32 xSrc4, xSrc5, xSrc6, xSrc7;
			mlib_s32 xSrc8, xSrc9, xSrc10, xSrc11;
			mlib_s32 xSrc12, xSrc13, xSrc14, xSrc15;
			mlib_u8 *srcPtr0, *srcPtr1, *srcPtr2, *srcPtr3;
			mlib_u8 *srcPtr4, *srcPtr5, *srcPtr6, *srcPtr7;
			mlib_u8 *srcPtr8, *srcPtr9, *srcPtr10, *srcPtr11;
			mlib_u8 *srcPtr12, *srcPtr13, *srcPtr14, *srcPtr15;

			CLIP(1);
			dstLineEnd = (DTYPE *) dstData + xRight;

			while (((mlib_addr)dstPixelPtr & 15) &&
					(dstPixelPtr <= dstLineEnd)) {
				ySrc = MLIB_POINTER_SHIFT(Y);
				Y += dY;
				srcPixelPtr = MLIB_POINTER_GET(lineAddr, ySrc);
				xSrc = X >> MLIB_SHIFT;
				X += dX;
				dstPixelPtr[0] = srcPixelPtr[xSrc];
				dstPixelPtr ++;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; dstPixelPtr <= (dstLineEnd - 16);
					dstPixelPtr += 16) {
				ySrc = MLIB_POINTER_SHIFT(Y);
				srcPtr0 = MLIB_POINTER_GET(lineAddr, ySrc);
				xSrc0 = X >> MLIB_SHIFT;

				ySrc = MLIB_POINTER_SHIFT(Y + dY);
				srcPtr1 = MLIB_POINTER_GET(lineAddr, ySrc);
				xSrc1 = (X + dX) >> MLIB_SHIFT;

				ySrc = MLIB_POINTER_SHIFT(Y + 2 * dY);
				srcPtr2 = MLIB_POINTER_GET(lineAddr, ySrc);
				xSrc2 = (X + 2 * dX) >> MLIB_SHIFT;

				ySrc = MLIB_POINTER_SHIFT(Y + 3 * dY);
				srcPtr3 = MLIB_POINTER_GET(lineAddr, ySrc);
				xSrc3 = (X + 3 * dX) >> MLIB_SHIFT;

				ySrc = MLIB_POINTER_SHIFT(Y + 4 * dY);
				srcPtr4 = MLIB_POINTER_GET(lineAddr, ySrc);
				xSrc4 = (X + 4 * dX) >> MLIB_SHIFT;

				ySrc = MLIB_POINTER_SHIFT(Y + 5 * dY);
				srcPtr5 = MLIB_POINTER_GET(lineAddr, ySrc);
				xSrc5 = (X + 5 * dX) >> MLIB_SHIFT;

				ySrc = MLIB_POINTER_SHIFT(Y + 6 * dY);
				srcPtr6 = MLIB_POINTER_GET(lineAddr, ySrc);
				xSrc6 = (X + 6 * dX) >> MLIB_SHIFT;

				ySrc = MLIB_POINTER_SHIFT(Y + 7 * dY);
				srcPtr7 = MLIB_POINTER_GET(lineAddr, ySrc);
				xSrc7 = (X + 7 * dX) >> MLIB_SHIFT;

				ySrc = MLIB_POINTER_SHIFT(Y + 8 * dY);
				srcPtr8 = MLIB_POINTER_GET(lineAddr, ySrc);
				xSrc8 = (X + 8 * dX) >> MLIB_SHIFT;

				ySrc = MLIB_POINTER_SHIFT(Y + 9 * dY);
				srcPtr9 = MLIB_POINTER_GET(lineAddr, ySrc);
				xSrc9 = (X + 9 * dX) >> MLIB_SHIFT;

				ySrc = MLIB_POINTER_SHIFT(Y + 10 * dY);
				srcPtr10 = MLIB_POINTER_GET(lineAddr, ySrc);
				xSrc10 = (X + 10 * dX) >> MLIB_SHIFT;

				ySrc = MLIB_POINTER_SHIFT(Y + 11 * dY);
				srcPtr11 = MLIB_POINTER_GET(lineAddr, ySrc);
				xSrc11 = (X + 11 * dX) >> MLIB_SHIFT;

				ySrc = MLIB_POINTER_SHIFT(Y + 12 * dY);
				srcPtr12 = MLIB_POINTER_GET(lineAddr, ySrc);
				xSrc12 = (X + 12 * dX) >> MLIB_SHIFT;

				ySrc = MLIB_POINTER_SHIFT(Y + 13 * dY);
				srcPtr13 = MLIB_POINTER_GET(lineAddr, ySrc);
				xSrc13 = (X + 13 * dX) >> MLIB_SHIFT;

				ySrc = MLIB_POINTER_SHIFT(Y + 14 * dY);
				srcPtr14 = MLIB_POINTER_GET(lineAddr, ySrc);
				xSrc14 = (X + 14 * dX) >> MLIB_SHIFT;

				ySrc = MLIB_POINTER_SHIFT(Y + 15 * dY);
				srcPtr15 = MLIB_POINTER_GET(lineAddr, ySrc);
				xSrc15 = (X + 15 * dX) >> MLIB_SHIFT;

				pix128 = _mm_setr_epi8(
					srcPtr0[xSrc0],
					srcPtr1[xSrc1],
					srcPtr2[xSrc2],
					srcPtr3[xSrc3],
					srcPtr4[xSrc4],
					srcPtr5[xSrc5],
					srcPtr6[xSrc6],
					srcPtr7[xSrc7],
					srcPtr8[xSrc8],
					srcPtr9[xSrc9],
					srcPtr10[xSrc10],
					srcPtr11[xSrc11],
					srcPtr12[xSrc12],
					srcPtr13[xSrc13],
					srcPtr14[xSrc14],
					srcPtr15[xSrc15]);
				_mm_store_si128((__m128i *)dstPixelPtr, pix128);
				Y += 16 * dY;
				X += 16 * dX;
			}

			for (; dstPixelPtr <= dstLineEnd; dstPixelPtr++) {
				ySrc = MLIB_POINTER_SHIFT(Y);
				Y += dY;
				srcPixelPtr = MLIB_POINTER_GET(lineAddr, ySrc);
				xSrc = X >> MLIB_SHIFT;
				X += dX;
				dstPixelPtr[0] = srcPixelPtr[xSrc];
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageAffine_u8_2ch_nn(
	mlib_affine_param *param)
{
	return (mlib_ImageAffine_s16_1ch_nn(param));
}
/* *********************************************************** */

mlib_status
mlib_ImageAffine_u8_3ch_nn(
	mlib_affine_param *param)
{
	DECLAREVAR_NN1();
	__m128i three;

	if (srcYStride <= 0xFFFF) {

		Stride128 = _mm_set1_epi32(((srcYStride) & 0xFFFF) << 16);
		three = _mm_set1_epi32(3 << 16);

		for (j = yStart; j <= yFinish; j++) {
			mlib_u8 tail;
			mlib_s32 pix0;
			mlib_s32 ind0, ind1, ind2, ind3;
			__m128i dx128, dy128, x128, y128;
			__m128i tmp0, tmp1, tmp2;

			CLIP(3);
			dstLineEnd = (DTYPE *) dstData + 3 * xRight;

			tail = dstLineEnd[3];

			dx128 = _mm_set1_epi32(dX << 2);
			dy128 = _mm_set1_epi32(dY << 2);
			x128 = _mm_setr_epi32(
					X, X + dX, X + 2 * dX, X + 3 * dX);
			y128 = _mm_setr_epi32(
					Y, Y + dY, Y + 2 * dY, Y + 3 * dY);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; dstPixelPtr <= (dstLineEnd - 12);
					dstPixelPtr += 12) {
				tmp0 = _mm_madd_epi16(x128, three);
				tmp1 = _mm_madd_epi16(y128, Stride128);
				tmp2 = _mm_add_epi32(tmp0, tmp1);

				ind0 = _mm_cvtsi128_si32(tmp2);
				tmp2 = _mm_srli_si128(tmp2, 4);
				ind1 = _mm_cvtsi128_si32(tmp2);
				tmp2 = _mm_srli_si128(tmp2, 4);
				ind2 = _mm_cvtsi128_si32(tmp2);
				tmp2 = _mm_srli_si128(tmp2, 4);
				ind3 = _mm_cvtsi128_si32(tmp2);

				pix0 = *(mlib_s32 *)(srcPixelPtr0 + ind0);
				*(mlib_s32 *)dstPixelPtr = pix0;
				pix0 = *(mlib_s32 *)(srcPixelPtr0 + ind1);
				*(mlib_s32 *)(dstPixelPtr + 3) = pix0;
				pix0 = *(mlib_s32 *)(srcPixelPtr0 + ind2);
				*(mlib_s32 *)(dstPixelPtr + 6) = pix0;
				pix0 = *(mlib_s32 *)(srcPixelPtr0 + ind3);
				*(mlib_s32 *)(dstPixelPtr + 9) = pix0;

				x128 = _mm_add_epi32(x128, dx128);
				y128 = _mm_add_epi32(y128, dy128);
			}

			X = _mm_cvtsi128_si32(x128);
			Y = _mm_cvtsi128_si32(y128);

			for (; dstPixelPtr <= dstLineEnd; dstPixelPtr += 3) {
				ySrc = MLIB_POINTER_SHIFT(Y);
				Y += dY;
				xSrc = X >> MLIB_SHIFT;
				X += dX;
				srcPixelPtr = MLIB_POINTER_GET(lineAddr, ySrc)
					+ 3 * xSrc;
				pix0 = *(mlib_s32 *)srcPixelPtr;
				*(mlib_s32 *)dstPixelPtr = pix0;
			}
			dstPixelPtr[0] = tail;
		}
	} else {
	for (j = yStart; j <= yFinish; j++) {
		__m128i pix128;
		__m128i tmp0, tmp1, tmp2, tmp3;
		DTYPE pix0, pix1, pix2;
		mlib_u8 *srcPtr0, *srcPtr1, *srcPtr2, *srcPtr3, *srcPtr4;

		CLIP(3);
		dstLineEnd = (DTYPE *) dstData + 3 * xRight;

		__m128i c_f000 = _mm_setr_epi32(0x00ffffff, 0x0, 0x0, 0x0);
		__m128i c_0f00 = _mm_slli_si128(c_f000, 4);
		__m128i c_00f0 = _mm_slli_si128(c_f000, 8);
		__m128i c_000f = _mm_slli_si128(c_f000, 12);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; dstPixelPtr <= (dstLineEnd - 13); dstPixelPtr += 12) {
			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			srcPtr0 =
				MLIB_POINTER_GET(lineAddr, ySrc) + 3 * xSrc;

			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			srcPtr1 =
				MLIB_POINTER_GET(lineAddr, ySrc) + 3 * xSrc;

			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			srcPtr2 =
				MLIB_POINTER_GET(lineAddr, ySrc) + 3 * xSrc;

			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			srcPtr3 =
				MLIB_POINTER_GET(lineAddr, ySrc) + 3 * xSrc;

			pix128 = _mm_setr_epi32(
					*(mlib_s32 *)(srcPtr0),
					*(mlib_s32 *)(srcPtr1),
					*(mlib_s32 *)(srcPtr2),
					*(mlib_s32 *)(srcPtr3));

#if 0
			pix128 = _mm_setr_epi8(
					srcPtr0[0],
					srcPtr0[1],
					srcPtr0[2],
					srcPtr0[3],
					srcPtr1[0],
					srcPtr1[1],
					srcPtr1[2],
					srcPtr1[3],
					srcPtr2[0],
					srcPtr2[1],
					srcPtr2[2],
					srcPtr2[3],
					srcPtr3[0],
					srcPtr3[1],
					srcPtr3[2],
					srcPtr3[3]);
#endif

			tmp0 = _mm_and_si128(pix128, c_f000);
			tmp1 = _mm_and_si128(pix128, c_0f00);
			tmp2 = _mm_and_si128(pix128, c_00f0);
			tmp3 = _mm_and_si128(pix128, c_000f);
			tmp1 = _mm_srli_si128(tmp1, 1);
			tmp2 = _mm_srli_si128(tmp2, 2);
			tmp3 = _mm_srli_si128(tmp3, 3);
			tmp0 = _mm_or_si128(tmp0, tmp1);
			tmp2 = _mm_or_si128(tmp2, tmp3);
			pix128 = _mm_or_si128(tmp0, tmp2);
			_mm_storeu_si128((__m128i *)dstPixelPtr, pix128);
		}

		for (; dstPixelPtr <= dstLineEnd; dstPixelPtr += 3) {
			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			srcPixelPtr =
				MLIB_POINTER_GET(lineAddr, ySrc) + 3 * xSrc;
			pix0 = srcPixelPtr[0];
			pix1 = srcPixelPtr[1];
			pix2 = srcPixelPtr[2];
			dstPixelPtr[0] = pix0;
			dstPixelPtr[1] = pix1;
			dstPixelPtr[2] = pix2;
		}
	}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageAffine_u8_4ch_nn(
	mlib_affine_param *param)
{
	return (mlib_ImageAffine_s32_1ch_nn(param));
}

/* *********************************************************** */

#undef  DTYPE
#define	DTYPE	mlib_u16

mlib_status
mlib_ImageAffine_s16_1ch_nn(
	mlib_affine_param *param)
{
	DECLAREVAR_NN1();

	if (((srcYStride & 1) == 0) && (srcYStride <= 0x1FFFE)) {
		srcYStride >>= 1;

		Stride128 = _mm_set1_epi32((srcYStride & 0xFFFF) << 16);

		for (j = yStart; j <= yFinish; j++) {
			mlib_u16 pix0;
			mlib_s32 ind0, ind1, ind2, ind3;
			__m128i dx128, dy128, x128, y128;
			__m128i tmp0, tmp1, tmp2;
			__m128i buf128;
			mlib_s32 *buf32 = (mlib_s32 *)&buf128;

			CLIP(1);
			dstLineEnd = (DTYPE *) dstData + xRight;

			dx128 = _mm_set1_epi32(dX << 2);
			dy128 = _mm_set1_epi32(dY << 2);
			x128 = _mm_setr_epi32(X, X + dX,
				X + 2 * dX, X + 3 * dX);
			y128 = _mm_setr_epi32(Y, Y + dY,
				Y + 2 * dY, Y + 3 * dY);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; dstPixelPtr <= (dstLineEnd - 4); dstPixelPtr += 4) {
			tmp0 = _mm_srai_epi32(x128, MLIB_SHIFT);
			tmp1 = _mm_madd_epi16(y128, Stride128);
			tmp2 = _mm_add_epi32(tmp0, tmp1);

			ind0 = _mm_cvtsi128_si32(tmp2);
			tmp2 = _mm_srli_si128(tmp2, 4);
			ind1 = _mm_cvtsi128_si32(tmp2);
			tmp2 = _mm_srli_si128(tmp2, 4);
			ind2 = _mm_cvtsi128_si32(tmp2);
			tmp2 = _mm_srli_si128(tmp2, 4);
			ind3 = _mm_cvtsi128_si32(tmp2);

			pix0 = srcPixelPtr0[ind0];
			dstPixelPtr[0] = pix0;
			pix0 = srcPixelPtr0[ind1];
			dstPixelPtr[1] = pix0;
			pix0 = srcPixelPtr0[ind2];
			dstPixelPtr[2] = pix0;
			pix0 = srcPixelPtr0[ind3];
			dstPixelPtr[3] = pix0;

			x128 = _mm_add_epi32(x128, dx128);
			y128 = _mm_add_epi32(y128, dy128);
		}

		X = _mm_cvtsi128_si32(x128);
		Y = _mm_cvtsi128_si32(y128);

		for (; dstPixelPtr <= dstLineEnd; dstPixelPtr++) {
			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			srcPixelPtr = MLIB_POINTER_GET(lineAddr, ySrc);
			pix0 = srcPixelPtr[xSrc];
			dstPixelPtr[0] = pix0;
		}
	}
	} else {
	for (j = yStart; j <= yFinish; j++) {
		__m128i pix128;
		mlib_u16 *srcPtr0, *srcPtr1, *srcPtr2, *srcPtr3;
		mlib_u16 *srcPtr4, *srcPtr5, *srcPtr6, *srcPtr7;
		mlib_s32 xSrc0, xSrc1, xSrc2, xSrc3;
		mlib_s32 xSrc4, xSrc5, xSrc6, xSrc7;
		mlib_s32 pix0;

		CLIP(1);
		dstLineEnd = (DTYPE *) dstData + xRight;

		while (((mlib_addr)dstPixelPtr & 15) &&
				(dstPixelPtr <= dstLineEnd)) {
			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			srcPixelPtr = MLIB_POINTER_GET(lineAddr, ySrc);
			pix0 = srcPixelPtr[xSrc];
			dstPixelPtr[0] = pix0;
			dstPixelPtr ++;
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; dstPixelPtr <= (dstLineEnd - 8); dstPixelPtr += 8) {
			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc0 = X >> MLIB_SHIFT;
			X += dX;
			srcPtr0 = MLIB_POINTER_GET(lineAddr, ySrc);

			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc1 = X >> MLIB_SHIFT;
			X += dX;
			srcPtr1 = MLIB_POINTER_GET(lineAddr, ySrc);

			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc2 = X >> MLIB_SHIFT;
			X += dX;
			srcPtr2 = MLIB_POINTER_GET(lineAddr, ySrc);

			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc3 = X >> MLIB_SHIFT;
			X += dX;
			srcPtr3 = MLIB_POINTER_GET(lineAddr, ySrc);

			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc4 = X >> MLIB_SHIFT;
			X += dX;
			srcPtr4 = MLIB_POINTER_GET(lineAddr, ySrc);

			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc5 = X >> MLIB_SHIFT;
			X += dX;
			srcPtr5 = MLIB_POINTER_GET(lineAddr, ySrc);

			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc6 = X >> MLIB_SHIFT;
			X += dX;
			srcPtr6 = MLIB_POINTER_GET(lineAddr, ySrc);

			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc7 = X >> MLIB_SHIFT;
			X += dX;
			srcPtr7 = MLIB_POINTER_GET(lineAddr, ySrc);

			pix128 = _mm_setr_epi16(
					srcPtr0[xSrc0],
					srcPtr1[xSrc1],
					srcPtr2[xSrc2],
					srcPtr3[xSrc3],
					srcPtr4[xSrc4],
					srcPtr5[xSrc5],
					srcPtr6[xSrc6],
					srcPtr7[xSrc7]);
			_mm_store_si128((__m128i *)dstPixelPtr, pix128);
		}

		for (; dstPixelPtr <= dstLineEnd; dstPixelPtr++) {
			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			srcPixelPtr = MLIB_POINTER_GET(lineAddr, ySrc);
			pix0 = srcPixelPtr[xSrc];
			dstPixelPtr[0] = pix0;
		}
	}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageAffine_s16_2ch_nn(
	mlib_affine_param *param)
{
	return (mlib_ImageAffine_s32_1ch_nn(param));
}

/* *********************************************************** */

mlib_status
mlib_ImageAffine_s16_3ch_nn(
	mlib_affine_param *param)
{
	DECLAREVAR_NN1();
	__m128i three;

	if (srcYStride <= 0xFFFF) {

		srcYStride >>= 1;
		Stride128 = _mm_set1_epi32(((srcYStride) & 0xFFFF) << 16);
		three = _mm_set1_epi32(3 << 16);

		for (j = yStart; j <= yFinish; j++) {
			mlib_u16 tail;
			__m128i pix0;
			mlib_s32 ind0, ind1, ind2, ind3;
			__m128i dx128, dy128, x128, y128;
			__m128i tmp0, tmp1, tmp2;

			CLIP(3);
			dstLineEnd = (DTYPE *) dstData + 3 * xRight;

			tail = dstLineEnd[3];

			dx128 = _mm_set1_epi32(dX << 2);
			dy128 = _mm_set1_epi32(dY << 2);
			x128 = _mm_setr_epi32(
					X, X + dX, X + 2 * dX, X + 3 * dX);
			y128 = _mm_setr_epi32(
					Y, Y + dY, Y + 2 * dY, Y + 3 * dY);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; dstPixelPtr <= (dstLineEnd - 12);
					dstPixelPtr += 12) {
				tmp0 = _mm_madd_epi16(x128, three);
				tmp1 = _mm_madd_epi16(y128, Stride128);
				tmp2 = _mm_add_epi32(tmp0, tmp1);

				ind0 = _mm_cvtsi128_si32(tmp2);
				tmp2 = _mm_srli_si128(tmp2, 4);
				ind1 = _mm_cvtsi128_si32(tmp2);
				tmp2 = _mm_srli_si128(tmp2, 4);
				ind2 = _mm_cvtsi128_si32(tmp2);
				tmp2 = _mm_srli_si128(tmp2, 4);
				ind3 = _mm_cvtsi128_si32(tmp2);

				pix0 = _mm_loadl_epi64(
					(__m128i *)(srcPixelPtr0 + ind0));
				_mm_storel_epi64((__m128i *)dstPixelPtr, pix0);
				pix0 = _mm_loadl_epi64(
					(__m128i *)(srcPixelPtr0 + ind1));
				_mm_storel_epi64(
					(__m128i *)(dstPixelPtr+3), pix0);
				pix0 = _mm_loadl_epi64(
					(__m128i *)(srcPixelPtr0 + ind2));
				_mm_storel_epi64(
					(__m128i *)(dstPixelPtr+6), pix0);
				pix0 = _mm_loadl_epi64(
					(__m128i *)(srcPixelPtr0 + ind3));
				_mm_storel_epi64(
					(__m128i *)(dstPixelPtr+9), pix0);

				x128 = _mm_add_epi32(x128, dx128);
				y128 = _mm_add_epi32(y128, dy128);
			}

			X = _mm_cvtsi128_si32(x128);
			Y = _mm_cvtsi128_si32(y128);

			for (; dstPixelPtr <= dstLineEnd; dstPixelPtr += 3) {
				ySrc = MLIB_POINTER_SHIFT(Y);
				Y += dY;
				xSrc = X >> MLIB_SHIFT;
				X += dX;
				srcPixelPtr = MLIB_POINTER_GET(lineAddr, ySrc)
					+ 3 * xSrc;
				pix0 = _mm_loadl_epi64(
					(__m128i *)srcPixelPtr);
				_mm_storel_epi64(
					(__m128i *)dstPixelPtr, pix0);
			}
			dstPixelPtr[0] = tail;
		}
	} else {
		for (j = yStart; j <= yFinish; j++) {
			__m128i pix128;
			__m128i tmp0, tmp1;

			mlib_u16 *srcPtr0, *srcPtr1;
			mlib_s32 pix0, pix1, pix2;

			CLIP(3);
			dstLineEnd = (DTYPE *) dstData + 3 * xRight;

			__m128i c_f0 = _mm_setr_epi32(
					0x0, 0x0, 0xffffffff, 0x0000ffff);
			__m128i c_0f = _mm_srli_si128(c_f0, 8);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; dstPixelPtr <= (dstLineEnd - 8);
					dstPixelPtr += 6) {
				ySrc = MLIB_POINTER_SHIFT(Y);
				Y += dY;
				xSrc = X >> MLIB_SHIFT;
				X += dX;
				srcPtr0 =
					MLIB_POINTER_GET(lineAddr, ySrc)
					+ 3 * xSrc;

				ySrc = MLIB_POINTER_SHIFT(Y);
				Y += dY;
				xSrc = X >> MLIB_SHIFT;
				X += dX;
				srcPtr1 =
					MLIB_POINTER_GET(lineAddr, ySrc)
					+ 3 * xSrc;

				pix128 = _mm_setr_epi32(
					*(mlib_s32 *)(srcPtr0),
					*(mlib_s32 *)(srcPtr0 + 2),
					*(mlib_s32 *)(srcPtr1),
					*(mlib_s32 *)(srcPtr1 + 2));

				tmp0 = _mm_and_si128(pix128, c_f0);
				tmp1 = _mm_and_si128(pix128, c_0f);
				tmp0 = _mm_srli_si128(tmp0, 2);
				pix128 = _mm_or_si128(tmp0, tmp1);
				_mm_storeu_si128(
					(__m128i *)dstPixelPtr, pix128);
			}

			for (; dstPixelPtr <= dstLineEnd; dstPixelPtr += 3) {
				ySrc = MLIB_POINTER_SHIFT(Y);
				Y += dY;
				xSrc = X >> MLIB_SHIFT;
				X += dX;
				srcPixelPtr =
					MLIB_POINTER_GET(lineAddr, ySrc)
					+ 3 * xSrc;
				pix0 = srcPixelPtr[0];
				pix1 = srcPixelPtr[1];
				pix2 = srcPixelPtr[2];
				dstPixelPtr[0] = pix0;
				dstPixelPtr[1] = pix1;
				dstPixelPtr[2] = pix2;
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageAffine_s16_4ch_nn(
	mlib_affine_param *param)
{
	return (mlib_ImageAffine_d64_1ch_nn(param));
}

/* *********************************************************** */
