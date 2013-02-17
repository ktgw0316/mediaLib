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

#pragma ident	"@(#)mlib_s_ImageAffine_BL_S16.c	9.7	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageAffine_s16_1ch_bl
 *      mlib_ImageAffine_s16_2ch_bl
 *      mlib_ImageAffine_s16_3ch_bl
 *      mlib_ImageAffine_s16_4ch_bl
 *        - image affine transformation with Bilinear filtering
 * SYNOPSIS
 *      mlib_status mlib_ImageAffine_s16_?ch_bl(mlib_s32 *leftEdges,
 *                                              mlib_s32 *rightEdges,
 *                                              mlib_s32 *xStarts,
 *                                              mlib_s32 *yStarts,
 *                                              mlib_s32 *sides,
 *                                              mlib_u8  *dstData,
 *                                              mlib_u8  **lineAddr,
 *                                              mlib_s32 dstYStride,
 *                                              mlib_s32 is_affine,
 *                                              mlib_s32 srcYStride)
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
 *      srcYStride stride of source image
 *
 * DESCRIPTION
 *      The functions step along the lines from xLeft to xRight and apply
 *      the bilinear filtering.
 *
 */

#include <mlib_ImageAffine.h>
#ifdef	__SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	DTYPE	mlib_s16

/* *********************************************************** */

#define	TTYPE	mlib_s32

#define	FUN_NAME(CHAN)	mlib_ImageAffine_s16_##CHAN##_bl

/* *********************************************************** */

/*
 * for SHORT/USHORT decrease MLIB_SHIFT due to * overflow in
 * multiplies like fdy * (a10 - a00)
 */

#undef  MLIB_SHIFT
#define	MLIB_SHIFT	15

#define	MLIB_ROUND	(1 << (MLIB_SHIFT - 1))

#define	MLIB_SHIFT_SSE2	(MLIB_SHIFT - 8)
#define	MLIB_ROUND_SSE2	(1 << (MLIB_SHIFT_SSE2 - 1))

/* *********************************************************** */

#define	GET_POINTERS(ind)                                                 \
	fdx = X & MLIB_MASK;                                              \
	fdy = Y & MLIB_MASK;                                              \
	ySrc = MLIB_POINTER_SHIFT(Y);                                     \
	xSrc = X >> MLIB_SHIFT;                                           \
	srcPixelPtr = MLIB_POINTER_GET(lineAddr, ySrc) + ind * xSrc;      \
	srcPixelPtr2 = (DTYPE *) ((mlib_u8 *)srcPixelPtr + srcYStride);   \
	X += dX;                                                          \
	Y += dY

#define	GET_POINTERS_SSE2(ind, ptr_ind)                                   \
	ySrc = MLIB_POINTER_SHIFT(Y);                                     \
	xSrc = X >> MLIB_SHIFT;                                           \
	srcPtr_##ptr_ind = MLIB_POINTER_GET(lineAddr, ySrc) + ind * xSrc; \
	srcPtr2_##ptr_ind =                                               \
		(DTYPE *) ((mlib_u8 *)srcPtr_##ptr_ind + srcYStride);     \
	X += dX;                                                          \
	Y += dY

#define	GET_POINTERS_SSE2_2CH(ind, ptr_ind)                               \
	ySrc = MLIB_POINTER_SHIFT(Y);                                     \
	xSrc = (X >> (MLIB_SHIFT - 1)) & ~1;                              \
	srcPtr_##ptr_ind = MLIB_POINTER_GET(lineAddr, ySrc) + xSrc;       \
	X += dX;                                                          \
	Y += dY


/* *********************************************************** */

#define	COUNT(ind)                                              \
	pix0_##ind =                                            \
		a00_##ind + ((fdy * (a10_##ind -                \
		a00_##ind) + MLIB_ROUND) >> MLIB_SHIFT);        \
	pix1_##ind =                                            \
		a01_##ind + ((fdy * (a11_##ind -                \
		a01_##ind) + MLIB_ROUND) >> MLIB_SHIFT);        \
	res##ind =                                              \
		pix0_##ind + ((fdx * (pix1_##ind -              \
		pix0_##ind) + MLIB_ROUND) >> MLIB_SHIFT); \

/* *********************************************************** */

#define	LOAD(ind, ind1, ind2)                                   \
	a00_##ind = srcPixelPtr[ind1];                          \
	a01_##ind = srcPixelPtr[ind2];                          \
	a10_##ind = srcPixelPtr2[ind1];                         \
	a11_##ind = srcPixelPtr2[ind2]

/* *********************************************************** */

#define	LOAD_TWO_64(dst128, add64_0, add64_1)                       \
	{                                                           \
		__m128i l128, h128;                                 \
		l128 = _mm_loadl_epi64((__m128i *)add64_0);         \
		h128 = _mm_loadl_epi64((__m128i *)add64_1);         \
		dst128 = _mm_unpacklo_epi64(l128, h128);            \
	}

/* *********************************************************** */

mlib_status FUN_NAME(
	1ch) (
	mlib_affine_param *param)
{
	DECLAREVAR_BL();
	DTYPE *dstLineEnd;
	DTYPE *srcPixelPtr2;

#if MLIB_SHIFT == 15
	dX = (dX + 1) >> 1;
	dY = (dY + 1) >> 1;
#endif /* MLIB_SHIFT == 15 */

	__m128i const_7fff = _mm_set1_epi16(0x7fff);
	__m128i masks = _mm_set1_epi16(MLIB_MASK);

	for (j = yStart; j <= yFinish; j++) {
		__m128i a00_0s, a01_0s, a10_0s, a11_0s;
		__m128i pix0_0s, pix0_1s, pix1_0s, pix1_1s, res0s;
		__m128i pix0s, pix1s;
		__m128i fdxs, fdys, deltax, deltay;
		__m128i fdx2s, fdy2s;

		mlib_s32 fdx, fdy;
		mlib_s32 a00_0, a01_0, a10_0, a11_0;
		mlib_s32 pix0_0, pix1_0, res0;

		mlib_s16 *srcPtr_0, *srcPtr_1, *srcPtr_2, *srcPtr_3;
		mlib_s16 *srcPtr_4, *srcPtr_5, *srcPtr_6, *srcPtr_7;
		mlib_s16 *srcPtr2_0, *srcPtr2_1, *srcPtr2_2, *srcPtr2_3;
		mlib_s16 *srcPtr2_4, *srcPtr2_5, *srcPtr2_6, *srcPtr2_7;

		CLIP(1);
		dstLineEnd = (DTYPE *) dstData + xRight;
#if MLIB_SHIFT == 15
		X = X >> 1;
		Y = Y >> 1;

		if (warp_tbl != NULL) {
			dX = (dX + 1) >> 1;
			dY = (dY + 1) >> 1;
		}
#endif /* MLIB_SHIFT == 15 */

		fdxs = _mm_setr_epi16(
			X,
			(X + dX),
			(X + 2 * dX),
			(X + 3 * dX),
			(X + 4 * dX),
			(X + 5 * dX),
			(X + 6 * dX),
			(X + 7 * dX));

		fdys = _mm_setr_epi16(
			Y,
			(Y + dY),
			(Y + 2 * dY),
			(Y + 3 * dY),
			(Y + 4 * dY),
			(Y + 5 * dY),
			(Y + 6 * dY),
			(Y + 7 * dY));

		deltax = _mm_set1_epi16(8 * dX);
		deltay = _mm_set1_epi16(8 * dY);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; dstPixelPtr <= (dstLineEnd - 7); dstPixelPtr += 8) {

			fdxs = _mm_and_si128(fdxs, masks);
			fdys = _mm_and_si128(fdys, masks);

			GET_POINTERS_SSE2(1, 0);
			GET_POINTERS_SSE2(1, 1);
			GET_POINTERS_SSE2(1, 2);
			GET_POINTERS_SSE2(1, 3);
			GET_POINTERS_SSE2(1, 4);
			GET_POINTERS_SSE2(1, 5);
			GET_POINTERS_SSE2(1, 6);
			GET_POINTERS_SSE2(1, 7);

#if 0

			a00_0s = _mm_setr_epi16(
				srcPtr_0[0],
				srcPtr_1[0],
				srcPtr_2[0],
				srcPtr_3[0],
				srcPtr_4[0],
				srcPtr_5[0],
				srcPtr_6[0],
				srcPtr_7[0]);
			a01_0s = _mm_setr_epi16(
				srcPtr_0[1],
				srcPtr_1[1],
				srcPtr_2[1],
				srcPtr_3[1],
				srcPtr_4[1],
				srcPtr_5[1],
				srcPtr_6[1],
				srcPtr_7[1]);

			a10_0s = _mm_setr_epi16(
				srcPtr2_0[0],
				srcPtr2_1[0],
				srcPtr2_2[0],
				srcPtr2_3[0],
				srcPtr2_4[0],
				srcPtr2_5[0],
				srcPtr2_6[0],
				srcPtr2_7[0]);
			a11_0s = _mm_setr_epi16(
				srcPtr2_0[1],
				srcPtr2_1[1],
				srcPtr2_2[1],
				srcPtr2_3[1],
				srcPtr2_4[1],
				srcPtr2_5[1],
				srcPtr2_6[1],
				srcPtr2_7[1]);

#else

			pix0s = _mm_setr_epi32(
				*(mlib_s32 *)srcPtr_0,
				*(mlib_s32 *)srcPtr_4,
				*(mlib_s32 *)srcPtr_2,
				*(mlib_s32 *)srcPtr_6);

			pix1s = _mm_setr_epi32(
				*(mlib_s32 *)srcPtr_1,
				*(mlib_s32 *)srcPtr_5,
				*(mlib_s32 *)srcPtr_3,
				*(mlib_s32 *)srcPtr_7);

			a00_0s = _mm_unpacklo_epi16(pix0s, pix1s);
			a01_0s = _mm_unpackhi_epi16(pix0s, pix1s);
			pix0s = _mm_unpacklo_epi32(a00_0s, a01_0s);
			pix1s = _mm_unpackhi_epi32(a00_0s, a01_0s);
			a00_0s = _mm_unpacklo_epi64(pix0s, pix1s);
			a01_0s = _mm_unpackhi_epi64(pix0s, pix1s);

			pix0s = _mm_setr_epi32(
				*(mlib_s32 *)srcPtr2_0,
				*(mlib_s32 *)srcPtr2_4,
				*(mlib_s32 *)srcPtr2_2,
				*(mlib_s32 *)srcPtr2_6);

			pix1s = _mm_setr_epi32(
				*(mlib_s32 *)srcPtr2_1,
				*(mlib_s32 *)srcPtr2_5,
				*(mlib_s32 *)srcPtr2_3,
				*(mlib_s32 *)srcPtr2_7);

			a10_0s = _mm_unpacklo_epi16(pix0s, pix1s);
			a11_0s = _mm_unpackhi_epi16(pix0s, pix1s);
			pix0s = _mm_unpacklo_epi32(a10_0s, a11_0s);
			pix1s = _mm_unpackhi_epi32(a10_0s, a11_0s);
			a10_0s = _mm_unpacklo_epi64(pix0s, pix1s);
			a11_0s = _mm_unpackhi_epi64(pix0s, pix1s);

#endif

			fdx2s = _mm_sub_epi16(const_7fff, fdxs);
			fdy2s = _mm_sub_epi16(const_7fff, fdys);

			pix0_0s = _mm_mulhi_epi16(a10_0s, fdys);
			pix0_1s = _mm_mulhi_epi16(a00_0s, fdy2s);
			pix0_0s = _mm_add_epi16(pix0_0s, pix0_1s);
			pix0_0s = _mm_add_epi16(pix0_0s, pix0_0s);

			pix1_0s = _mm_mulhi_epi16(a11_0s, fdys);
			pix1_1s = _mm_mulhi_epi16(a01_0s, fdy2s);
			pix1_0s = _mm_add_epi16(pix1_0s, pix1_1s);
			pix1_0s = _mm_add_epi16(pix1_0s, pix1_0s);

			pix1s = _mm_mulhi_epi16(pix1_0s, fdxs);
			pix0s = _mm_mulhi_epi16(pix0_0s, fdx2s);
			res0s = _mm_add_epi16(pix1s, pix0s);
			res0s = _mm_add_epi16(res0s, res0s);

			_mm_storeu_si128((__m128i *)dstPixelPtr, res0s);

			fdxs = _mm_add_epi16(fdxs, deltax);
			fdys = _mm_add_epi16(fdys, deltay);
		}

		for (; dstPixelPtr <= dstLineEnd; dstPixelPtr++) {
			GET_POINTERS(1);
			LOAD(0, 0, 1);
			COUNT(0);
			dstPixelPtr[0] = (DTYPE) res0;
		}

	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUN_NAME(
	2ch) (
	mlib_affine_param *param)
{
	DECLAREVAR_BL();
	DTYPE *dstLineEnd;
	DTYPE *srcPixelPtr2;

	__m128i const_7fff = _mm_set1_epi16(0x7fff);
	__m128i masks = _mm_set1_epi16(MLIB_MASK);

#if MLIB_SHIFT == 15
	dX = (dX + 1) >> 1;
	dY = (dY + 1) >> 1;
#endif /* MLIB_SHIFT == 15 */

	for (j = yStart; j <= yFinish; j++) {
		__m128i a00_0s, a01_0s, a10_0s, a11_0s;
		__m128i pix0_0s, pix0_1s, pix1_0s, pix1_1s, res0s;
		__m128i pix0s, pix1s, pix2s, pix3s;
		__m128i fdxs, fdys, deltax, deltay;
		__m128i fdx2s, fdy2s;

		mlib_s32 fdx, fdy;
		mlib_s32 a00_0, a01_0, a10_0, a11_0;
		mlib_s32 a00_1, a01_1, a10_1, a11_1;
		mlib_s32 pix0_0, pix1_0, res0;
		mlib_s32 pix0_1, pix1_1, res1;

		mlib_s16 *srcPtr_0, *srcPtr_1, *srcPtr_2, *srcPtr_3;

		CLIP(2);
		dstLineEnd = (DTYPE *) dstData + 2 * xRight;
#if MLIB_SHIFT == 15
		X = X >> 1;
		Y = Y >> 1;

		if (warp_tbl != NULL) {
			dX = (dX + 1) >> 1;
			dY = (dY + 1) >> 1;
		}
#endif /* MLIB_SHIFT == 15 */

		fdxs = _mm_setr_epi16(
			X,
			X,
			(X + dX),
			(X + dX),
			(X + 2 * dX),
			(X + 2 * dX),
			(X + 3 * dX),
			(X + 3 * dX));

		fdys = _mm_setr_epi16(
			Y,
			Y,
			(Y + dY),
			(Y + dY),
			(Y + 2 * dY),
			(Y + 2 * dY),
			(Y + 3 * dY),
			(Y + 3 * dY));

		deltax = _mm_set1_epi16(4 * dX);
		deltay = _mm_set1_epi16(4 * dY);

		if (dstPixelPtr <= (dstLineEnd - 8)) {
			GET_POINTERS_SSE2_2CH(2, 0);
			GET_POINTERS_SSE2_2CH(2, 1);
			GET_POINTERS_SSE2_2CH(2, 2);
			GET_POINTERS_SSE2_2CH(2, 3);

			fdxs = _mm_and_si128(fdxs, masks);
			fdys = _mm_and_si128(fdys, masks);

			fdx2s = _mm_sub_epi16(const_7fff, fdxs);
			fdy2s = _mm_sub_epi16(const_7fff, fdys);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; dstPixelPtr <= (dstLineEnd - 16);
					dstPixelPtr += 8) {

				pix0s = _mm_loadl_epi64((__m128i *)srcPtr_0);
				pix1s = _mm_loadl_epi64((__m128i *)srcPtr_1);
				pix2s = _mm_loadl_epi64((__m128i *)srcPtr_2);
				pix3s = _mm_loadl_epi64((__m128i *)srcPtr_3);
				pix0s = _mm_unpacklo_epi32(pix0s, pix1s);
				pix2s = _mm_unpacklo_epi32(pix2s, pix3s);
				a00_0s = _mm_unpacklo_epi64(pix0s, pix2s);
				a01_0s = _mm_unpackhi_epi64(pix0s, pix2s);

				pix0s = _mm_loadl_epi64((__m128i *)
					((mlib_u8 *)srcPtr_0 + srcYStride));
				pix1s = _mm_loadl_epi64((__m128i *)
					((mlib_u8 *)srcPtr_1 + srcYStride));
				pix2s = _mm_loadl_epi64((__m128i *)
					((mlib_u8 *)srcPtr_2 + srcYStride));
				pix3s = _mm_loadl_epi64((__m128i *)
					((mlib_u8 *)srcPtr_3 + srcYStride));
				pix0s = _mm_unpacklo_epi32(pix0s, pix1s);
				pix2s = _mm_unpacklo_epi32(pix2s, pix3s);
				a10_0s = _mm_unpacklo_epi64(pix0s, pix2s);
				a11_0s = _mm_unpackhi_epi64(pix0s, pix2s);

				GET_POINTERS_SSE2_2CH(2, 0);
				GET_POINTERS_SSE2_2CH(2, 1);
				GET_POINTERS_SSE2_2CH(2, 2);
				GET_POINTERS_SSE2_2CH(2, 3);

				pix0_0s = _mm_mulhi_epi16(a10_0s, fdys);
				pix0_1s = _mm_mulhi_epi16(a00_0s, fdy2s);
				pix0_0s = _mm_add_epi16(pix0_0s, pix0_1s);
				pix0_0s = _mm_add_epi16(pix0_0s, pix0_0s);

				pix1_0s = _mm_mulhi_epi16(a11_0s, fdys);
				pix1_1s = _mm_mulhi_epi16(a01_0s, fdy2s);
				pix1_0s = _mm_add_epi16(pix1_0s, pix1_1s);
				pix1_0s = _mm_add_epi16(pix1_0s, pix1_0s);

				pix1s = _mm_mulhi_epi16(pix1_0s, fdxs);
				pix0s = _mm_mulhi_epi16(pix0_0s, fdx2s);
				res0s = _mm_add_epi16(pix1s, pix0s);
				res0s = _mm_add_epi16(res0s, res0s);

				fdxs = _mm_add_epi16(fdxs, deltax);
				fdys = _mm_add_epi16(fdys, deltay);

				fdxs = _mm_and_si128(fdxs, masks);
				fdys = _mm_and_si128(fdys, masks);

				fdx2s = _mm_sub_epi16(const_7fff, fdxs);
				fdy2s = _mm_sub_epi16(const_7fff, fdys);
				_mm_storeu_si128((__m128i *)dstPixelPtr, res0s);
			}

			pix0s = _mm_loadl_epi64((__m128i *)srcPtr_0);
			pix1s = _mm_loadl_epi64((__m128i *)srcPtr_1);
			pix2s = _mm_loadl_epi64((__m128i *)srcPtr_2);
			pix3s = _mm_loadl_epi64((__m128i *)srcPtr_3);
			pix0s = _mm_unpacklo_epi32(pix0s, pix1s);
			pix2s = _mm_unpacklo_epi32(pix2s, pix3s);
			a00_0s = _mm_unpacklo_epi64(pix0s, pix2s);
			a01_0s = _mm_unpackhi_epi64(pix0s, pix2s);

			pix0s = _mm_loadl_epi64((__m128i *)
				((mlib_u8 *)srcPtr_0 + srcYStride));
			pix1s = _mm_loadl_epi64((__m128i *)
				((mlib_u8 *)srcPtr_1 + srcYStride));
			pix2s = _mm_loadl_epi64((__m128i *)
				((mlib_u8 *)srcPtr_2 + srcYStride));
			pix3s = _mm_loadl_epi64((__m128i *)
				((mlib_u8 *)srcPtr_3 + srcYStride));
			pix0s = _mm_unpacklo_epi32(pix0s, pix1s);
			pix2s = _mm_unpacklo_epi32(pix2s, pix3s);
			a10_0s = _mm_unpacklo_epi64(pix0s, pix2s);
			a11_0s = _mm_unpackhi_epi64(pix0s, pix2s);

			pix0_0s = _mm_mulhi_epi16(a10_0s, fdys);
			pix0_1s = _mm_mulhi_epi16(a00_0s, fdy2s);
			pix0_0s = _mm_add_epi16(pix0_0s, pix0_1s);
			pix0_0s = _mm_add_epi16(pix0_0s, pix0_0s);

			pix1_0s = _mm_mulhi_epi16(a11_0s, fdys);
			pix1_1s = _mm_mulhi_epi16(a01_0s, fdy2s);
			pix1_0s = _mm_add_epi16(pix1_0s, pix1_1s);
			pix1_0s = _mm_add_epi16(pix1_0s, pix1_0s);

			pix1s = _mm_mulhi_epi16(pix1_0s, fdxs);
			pix0s = _mm_mulhi_epi16(pix0_0s, fdx2s);
			res0s = _mm_add_epi16(pix1s, pix0s);
			res0s = _mm_add_epi16(res0s, res0s);

			fdxs = _mm_add_epi16(fdxs, deltax);
			fdys = _mm_add_epi16(fdys, deltay);

			fdxs = _mm_and_si128(fdxs, masks);
			fdys = _mm_and_si128(fdys, masks);

			fdx2s = _mm_sub_epi16(const_7fff, fdxs);
			fdy2s = _mm_sub_epi16(const_7fff, fdys);
			_mm_storeu_si128((__m128i *)dstPixelPtr, res0s);

			dstPixelPtr += 8;
		}


		for (; dstPixelPtr <= dstLineEnd; dstPixelPtr += 2) {
			GET_POINTERS(2);
			LOAD(0, 0, 2);
			LOAD(1, 1, 3);
			COUNT(0);
			COUNT(1);
			dstPixelPtr[0] = (DTYPE) res0;
			dstPixelPtr[1] = (DTYPE) res1;
		}
	}

	_mm_empty();

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUN_NAME(
	3ch) (
	mlib_affine_param *param)
{
	DECLAREVAR_BL();
	DTYPE *dstLineEnd;
	DTYPE *srcPixelPtr2;

	__m128i const_7fff = _mm_set1_epi16(0x7fff);
	__m128i masks = _mm_set1_epi16(MLIB_MASK);

#if MLIB_SHIFT == 15
	dX = (dX + 1) >> 1;
	dY = (dY + 1) >> 1;
#endif /* MLIB_SHIFT == 15 */

	for (j = yStart; j <= yFinish; j++) {
		__m128i a00_0s, a01_0s, a10_0s, a11_0s;
		__m128i pix0_0s, pix0_1s, pix1_0s, pix1_1s, res0s;
		__m128i pix0s, pix1s;
		__m128i fdxs, fdys, deltax, deltay;
		__m128i fdx2s, fdy2s;
		__m128i c_0f =
			_mm_setr_epi16(0xffff, 0xffff, 0xffff, 0, 0, 0, 0, 0);
		__m128i c_f0 = _mm_slli_si128(c_0f, 8);

		mlib_s32 fdx, fdy;
		mlib_s32 a00_0, a01_0, a10_0, a11_0;
		mlib_s32 a00_1, a01_1, a10_1, a11_1;
		mlib_s32 a00_2, a01_2, a10_2, a11_2;
		mlib_s32 pix0_0, pix1_0, res0;
		mlib_s32 pix0_1, pix1_1, res1;
		mlib_s32 pix0_2, pix1_2, res2;

		mlib_s16 *srcPtr_0, *srcPtr_1;
		mlib_s16 *srcPtr2_0, *srcPtr2_1;

		CLIP(3);
		dstLineEnd = (DTYPE *) dstData + 3 * xRight;
#if MLIB_SHIFT == 15
		X = X >> 1;
		Y = Y >> 1;

		if (warp_tbl != NULL) {
			dX = (dX + 1) >> 1;
			dY = (dY + 1) >> 1;
		}
#endif /* MLIB_SHIFT == 15 */

		fdxs = _mm_setr_epi16(
			X,
			(X + 0 *dX),
			(X + 0 * dX),
			(X + 0 * dX),
			(X + 1 * dX),
			(X + 1 * dX),
			(X + 1 * dX),
			(X + 1 * dX));

		fdys = _mm_setr_epi16(
			Y,
			(Y + 0 * dY),
			(Y + 0 * dY),
			(Y + 0 * dY),
			(Y + 1 * dY),
			(Y + 1 * dY),
			(Y + 1 * dY),
			(Y + 1 * dY));

		deltax = _mm_set1_epi16(2 * dX);
		deltay = _mm_set1_epi16(2 * dY);


#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; dstPixelPtr <= (dstLineEnd - 6); dstPixelPtr += 6) {
			fdxs = _mm_and_si128(fdxs, masks);
			fdys = _mm_and_si128(fdys, masks);

			GET_POINTERS_SSE2(3, 0);
			GET_POINTERS_SSE2(3, 1);

			pix0s = _mm_loadu_si128((__m128i *)(srcPtr_0 - 1));
			pix1s = _mm_loadu_si128((__m128i *)(srcPtr_1 - 1));
			a00_0s = _mm_unpacklo_epi64(pix0s, pix1s);
			a01_0s = _mm_unpackhi_epi64(pix0s, pix1s);
			a00_0s = _mm_srli_epi64(a00_0s, 16);

			pix0s = _mm_loadu_si128((__m128i *)(srcPtr2_0 - 1));
			pix1s = _mm_loadu_si128((__m128i *)(srcPtr2_1 - 1));
			a10_0s = _mm_unpacklo_epi64(pix0s, pix1s);
			a11_0s = _mm_unpackhi_epi64(pix0s, pix1s);
			a10_0s = _mm_srli_epi64(a10_0s, 16);

			fdx2s = _mm_sub_epi16(const_7fff, fdxs);
			fdy2s = _mm_sub_epi16(const_7fff, fdys);

			pix0_0s = _mm_mulhi_epi16(a10_0s, fdys);
			pix0_1s = _mm_mulhi_epi16(a00_0s, fdy2s);
			pix0_0s = _mm_add_epi16(pix0_0s, pix0_1s);
			pix0_0s = _mm_add_epi16(pix0_0s, pix0_0s);

			pix1_0s = _mm_mulhi_epi16(a11_0s, fdys);
			pix1_1s = _mm_mulhi_epi16(a01_0s, fdy2s);
			pix1_0s = _mm_add_epi16(pix1_0s, pix1_1s);
			pix1_0s = _mm_add_epi16(pix1_0s, pix1_0s);

			pix1s = _mm_mulhi_epi16(pix1_0s, fdxs);
			pix0s = _mm_mulhi_epi16(pix0_0s, fdx2s);
			res0s = _mm_add_epi16(pix1s, pix0s);
			res0s = _mm_add_epi16(res0s, res0s);

			pix0s = _mm_and_si128(res0s, c_0f);
			pix1s = _mm_and_si128(res0s, c_f0);
			pix1s = _mm_srli_si128(pix1s, 2);
			res0s = _mm_or_si128(pix0s, pix1s);

			_mm_storeu_si128((__m128i *)dstPixelPtr, res0s);

			fdxs = _mm_add_epi16(fdxs, deltax);
			fdys = _mm_add_epi16(fdys, deltay);
		}

		for (; dstPixelPtr <= dstLineEnd; dstPixelPtr += 3) {
			GET_POINTERS(3);
			LOAD(0, 0, 3);
			LOAD(1, 1, 4);
			LOAD(2, 2, 5);
			COUNT(0);
			COUNT(1);
			COUNT(2);
			dstPixelPtr[0] = (DTYPE) res0;
			dstPixelPtr[1] = (DTYPE) res1;
			dstPixelPtr[2] = (DTYPE) res2;
		}

	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUN_NAME(
	4ch) (
	mlib_affine_param *param)
{
	DECLAREVAR_BL();
	DTYPE *dstLineEnd;
	DTYPE *srcPixelPtr2;

	__m128i const_7fff = _mm_set1_epi16(0x7fff);
	__m128i masks = _mm_set1_epi16(MLIB_MASK);

#if MLIB_SHIFT == 15
	dX = (dX + 1) >> 1;
	dY = (dY + 1) >> 1;
#endif /* MLIB_SHIFT == 15 */

	for (j = yStart; j <= yFinish; j++) {
		__m128i a00_0s, a01_0s, a10_0s, a11_0s;
		__m128i pix0_0s, pix0_1s, pix1_0s, pix1_1s, res0s;
		__m128i pix0s, pix1s;
		__m128i fdxs, fdys, deltax, deltay;
		__m128i fdx2s, fdy2s;

		mlib_s32 fdx, fdy;
		mlib_s32 a00_0, a01_0, a10_0, a11_0;
		mlib_s32 a00_1, a01_1, a10_1, a11_1;
		mlib_s32 a00_2, a01_2, a10_2, a11_2;
		mlib_s32 a00_3, a01_3, a10_3, a11_3;
		mlib_s32 pix0_0, pix1_0, res0;
		mlib_s32 pix0_1, pix1_1, res1;
		mlib_s32 pix0_2, pix1_2, res2;
		mlib_s32 pix0_3, pix1_3, res3;

		mlib_s16 *srcPtr_0, *srcPtr_1;
		mlib_s16 *srcPtr2_0, *srcPtr2_1;

		CLIP(4);
		dstLineEnd = (DTYPE *) dstData + 4 * xRight;
#if MLIB_SHIFT == 15
		X = X >> 1;
		Y = Y >> 1;

		if (warp_tbl != NULL) {
			dX = (dX + 1) >> 1;
			dY = (dY + 1) >> 1;
		}
#endif /* MLIB_SHIFT == 15 */

		fdxs = _mm_setr_epi16(
			X,
			(X + 0 *dX),
			(X + 0 * dX),
			(X + 0 * dX),
			(X + 1 * dX),
			(X + 1 * dX),
			(X + 1 * dX),
			(X + 1 * dX));

		fdys = _mm_setr_epi16(
			Y,
			(Y + 0 * dY),
			(Y + 0 * dY),
			(Y + 0 * dY),
			(Y + 1 * dY),
			(Y + 1 * dY),
			(Y + 1 * dY),
			(Y + 1 * dY));

		deltax = _mm_set1_epi16(2 * dX);
		deltay = _mm_set1_epi16(2 * dY);


#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; dstPixelPtr <= (dstLineEnd - 7); dstPixelPtr += 8) {
			fdxs = _mm_and_si128(fdxs, masks);
			fdys = _mm_and_si128(fdys, masks);

			GET_POINTERS_SSE2(4, 0);
			GET_POINTERS_SSE2(4, 1);

			pix0s = _mm_loadu_si128((__m128i *)srcPtr_0);
			pix1s = _mm_loadu_si128((__m128i *)srcPtr_1);
			a00_0s = _mm_unpacklo_epi64(pix0s, pix1s);
			a01_0s = _mm_unpackhi_epi64(pix0s, pix1s);

			pix0s = _mm_loadu_si128((__m128i *)srcPtr2_0);
			pix1s = _mm_loadu_si128((__m128i *)srcPtr2_1);
			a10_0s = _mm_unpacklo_epi64(pix0s, pix1s);
			a11_0s = _mm_unpackhi_epi64(pix0s, pix1s);

			fdx2s = _mm_sub_epi16(const_7fff, fdxs);
			fdy2s = _mm_sub_epi16(const_7fff, fdys);

			pix0_0s = _mm_mulhi_epi16(a10_0s, fdys);
			pix0_1s = _mm_mulhi_epi16(a00_0s, fdy2s);
			pix0_0s = _mm_add_epi16(pix0_0s, pix0_1s);
			pix0_0s = _mm_add_epi16(pix0_0s, pix0_0s);

			pix1_0s = _mm_mulhi_epi16(a11_0s, fdys);
			pix1_1s = _mm_mulhi_epi16(a01_0s, fdy2s);
			pix1_0s = _mm_add_epi16(pix1_0s, pix1_1s);
			pix1_0s = _mm_add_epi16(pix1_0s, pix1_0s);

			pix1s = _mm_mulhi_epi16(pix1_0s, fdxs);
			pix0s = _mm_mulhi_epi16(pix0_0s, fdx2s);
			res0s = _mm_add_epi16(pix1s, pix0s);
			res0s = _mm_add_epi16(res0s, res0s);

			_mm_storeu_si128((__m128i *)dstPixelPtr, res0s);

			fdxs = _mm_add_epi16(fdxs, deltax);
			fdys = _mm_add_epi16(fdys, deltay);
		}

		for (; dstPixelPtr <= dstLineEnd; dstPixelPtr += 4) {
			GET_POINTERS(4);
			LOAD(0, 0, 4);
			LOAD(1, 1, 5);
			LOAD(2, 2, 6);
			LOAD(3, 3, 7);
			COUNT(0);
			COUNT(1);
			COUNT(2);
			COUNT(3);
			dstPixelPtr[0] = (DTYPE) res0;
			dstPixelPtr[1] = (DTYPE) res1;
			dstPixelPtr[2] = (DTYPE) res2;
			dstPixelPtr[3] = (DTYPE) res3;
		}

	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
