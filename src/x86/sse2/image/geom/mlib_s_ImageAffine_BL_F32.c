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

#pragma ident	"@(#)mlib_s_ImageAffine_BL_F32.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *   Internal functions for mlib_ImageAffine with bilinear filtering.
 */

#include <mlib_ImageAffine.h>
#ifdef	__SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	DTYPE	mlib_f32
#define	FTYPE	mlib_f32

#define	FUN_NAME(CHAN)	mlib_ImageAffine_f32_##CHAN##_bl

/* *********************************************************** */

mlib_status FUN_NAME(
	1ch) (
	mlib_affine_param *param)
{
	DECLAREVAR_BL();
	DTYPE *dstLineEnd;
	FTYPE scale = ONE / MLIB_PREC;
	mlib_s32 srcYStride1;

	srcYStride /= sizeof (DTYPE);
	srcYStride1 = srcYStride + 1;

	for (j = yStart; j <= yFinish; j++) {
		FTYPE t, u, k0, k1, k2, k3;
		FTYPE a00_0, a01_0, a10_0, a11_0;
		FTYPE pix0;
		DTYPE *srcPtr0, *srcPtr1, *srcPtr2, *srcPtr3;
		__m128i Xs, Ys, dXs, dYs;
		__m128i mask128 = _mm_set1_epi32(MLIB_MASK);
		__m128 scales = _mm_set1_ps(scale);
		__m128 ones = _mm_set1_ps(ONE);
		__m128 ts, us, ts1, us1;
		__m128 k0s, k1s, k2s, k3s;
		__m128 a00s, a01s, a10s, a11s;
		__m128 pix0s, pix1s, pix2s, pix3s;

		CLIP(1);
		dstLineEnd = (DTYPE *) dstData + xRight;

		Xs = _mm_setr_epi32(X, X + dX, X + 2 * dX, X + 3 * dX);
		Ys = _mm_setr_epi32(Y, Y + dY, Y + 2 * dY, Y + 3 * dY);
		dXs = _mm_set1_epi32(4 * dX);
		dYs = _mm_set1_epi32(4 * dY);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; dstPixelPtr <= (dstLineEnd - 3); dstPixelPtr += 4) {
			ts = _mm_cvtepi32_ps(_mm_and_si128(Xs, mask128));
			us = _mm_cvtepi32_ps(_mm_and_si128(Ys, mask128));
			ts = _mm_mul_ps(ts, scales);
			us = _mm_mul_ps(us, scales);
			ts1 = _mm_sub_ps(ones, ts);
			us1 = _mm_sub_ps(ones, us);

			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			srcPtr0 =
				*(DTYPE **) ((mlib_u8 *)lineAddr + ySrc) + xSrc;
			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			srcPtr1 =
				*(DTYPE **) ((mlib_u8 *)lineAddr + ySrc) + xSrc;
			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			srcPtr2 =
				*(DTYPE **) ((mlib_u8 *)lineAddr + ySrc) + xSrc;
			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			srcPtr3 =
				*(DTYPE **) ((mlib_u8 *)lineAddr + ySrc) + xSrc;

			k3s = _mm_mul_ps(ts, us);
			k2s = _mm_mul_ps(ts1, us);
			k1s = _mm_mul_ps(ts, us1);
			k0s = _mm_mul_ps(ts1, us1);

			a00s = _mm_setr_ps(
				srcPtr0[0],
				srcPtr1[0],
				srcPtr2[0],
				srcPtr3[0]);
			a01s = _mm_setr_ps(
				srcPtr0[1],
				srcPtr1[1],
				srcPtr2[1],
				srcPtr3[1]);
			a10s = _mm_setr_ps(
				srcPtr0[srcYStride],
				srcPtr1[srcYStride],
				srcPtr2[srcYStride],
				srcPtr3[srcYStride]);
			a11s = _mm_setr_ps(
				srcPtr0[srcYStride1],
				srcPtr1[srcYStride1],
				srcPtr2[srcYStride1],
				srcPtr3[srcYStride1]);

			pix0s = _mm_mul_ps(k0s, a00s);
			pix1s = _mm_mul_ps(k1s, a01s);
			pix2s = _mm_mul_ps(k2s, a10s);
			pix3s = _mm_mul_ps(k3s, a11s);
			pix0s = _mm_add_ps(pix0s, pix1s);
			pix2s = _mm_add_ps(pix2s, pix3s);
			pix0s = _mm_add_ps(pix0s, pix2s);
			_mm_storeu_ps((mlib_f32 *)dstPixelPtr, pix0s);

			Xs = _mm_add_epi32(Xs, dXs);
			Ys = _mm_add_epi32(Ys, dYs);
		}

		for (; dstPixelPtr <= dstLineEnd; dstPixelPtr++) {
			t = (X & MLIB_MASK) * scale;
			u = (Y & MLIB_MASK) * scale;
			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			srcPixelPtr =
				*(DTYPE **) ((mlib_u8 *)lineAddr + ySrc) + xSrc;
			k3 = t * u;
			k2 = (ONE - t) * u;
			k1 = t * (ONE - u);
			k0 = (ONE - t) * (ONE - u);
			a00_0 = srcPixelPtr[0];
			a01_0 = srcPixelPtr[1];
			a10_0 = srcPixelPtr[srcYStride];
			a11_0 = srcPixelPtr[srcYStride1];
			pix0 = k0 * a00_0 + k1 * a01_0 + k2 * a10_0 +
				k3 * a11_0;
			dstPixelPtr[0] = (DTYPE) pix0;
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
	FTYPE scale = ONE / MLIB_PREC;

	for (j = yStart; j <= yFinish; j++) {
		DTYPE *srcPixelPtr2;
		FTYPE t, u, k0, k1, k2, k3;
		FTYPE a00_0, a01_0, a10_0, a11_0;
		FTYPE a00_1, a01_1, a10_1, a11_1;
		FTYPE pix0, pix1;
		DTYPE *srcPtr0, *srcPtr1, *srcPtr2, *srcPtr3;
		__m128i Xs, Ys, dXs, dYs;
		__m128i mask128 = _mm_set1_epi32(MLIB_MASK);
		__m128 scales = _mm_set1_ps(scale);
		__m128 ones = _mm_set1_ps(ONE);
		__m128 ts, us, ts1, us1;
		__m128 k0s, k1s, k2s, k3s;
		__m128 a00s, a01s, a10s, a11s;
		__m128 pix0s, pix1s, pix2s, pix3s;

		CLIP(2);
		dstLineEnd = (DTYPE *) dstData + 2 * xRight;

		Xs = _mm_setr_epi32(X, X, X + dX, X + dX);
		Ys = _mm_setr_epi32(Y, Y, Y + dY, Y + dY);
		dXs = _mm_set1_epi32(2 * dX);
		dYs = _mm_set1_epi32(2 * dY);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; dstPixelPtr <= (dstLineEnd - 3); dstPixelPtr += 4) {
			ts = _mm_cvtepi32_ps(_mm_and_si128(Xs, mask128));
			us = _mm_cvtepi32_ps(_mm_and_si128(Ys, mask128));
			ts = _mm_mul_ps(ts, scales);
			us = _mm_mul_ps(us, scales);
			ts1 = _mm_sub_ps(ones, ts);
			us1 = _mm_sub_ps(ones, us);

			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			srcPtr0 =
				MLIB_POINTER_GET(lineAddr, ySrc) + 2 * xSrc;
			srcPtr1 =
				(DTYPE *) ((mlib_u8 *)srcPtr0 + srcYStride);

			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			srcPtr2 =
				MLIB_POINTER_GET(lineAddr, ySrc) + 2 * xSrc;
			srcPtr3 =
				(DTYPE *) ((mlib_u8 *)srcPtr2 + srcYStride);

			k3s = _mm_mul_ps(ts, us);
			k2s = _mm_mul_ps(ts1, us);
			k1s = _mm_mul_ps(ts, us1);
			k0s = _mm_mul_ps(ts1, us1);

			a00s = _mm_loadl_pi(a00s, (__m64 *)srcPtr0);
			a00s = _mm_loadh_pi(a00s, (__m64 *)srcPtr2);
			a01s = _mm_loadl_pi(a01s, (__m64 *)(srcPtr0 + 2));
			a01s = _mm_loadh_pi(a01s, (__m64 *)(srcPtr2 + 2));
			a10s = _mm_loadl_pi(a10s, (__m64 *)srcPtr1);
			a10s = _mm_loadh_pi(a10s, (__m64 *)srcPtr3);
			a11s = _mm_loadl_pi(a11s, (__m64 *)(srcPtr1 + 2));
			a11s = _mm_loadh_pi(a11s, (__m64 *)(srcPtr3 + 2));

			pix0s = _mm_mul_ps(k0s, a00s);
			pix1s = _mm_mul_ps(k1s, a01s);
			pix2s = _mm_mul_ps(k2s, a10s);
			pix3s = _mm_mul_ps(k3s, a11s);
			pix0s = _mm_add_ps(pix0s, pix1s);
			pix2s = _mm_add_ps(pix2s, pix3s);
			pix0s = _mm_add_ps(pix0s, pix2s);
			_mm_storeu_ps((mlib_f32 *)dstPixelPtr, pix0s);

			Xs = _mm_add_epi32(Xs, dXs);
			Ys = _mm_add_epi32(Ys, dYs);
		}

		for (; dstPixelPtr <= dstLineEnd; dstPixelPtr += 2) {
			t = (X & MLIB_MASK) * scale;
			u = (Y & MLIB_MASK) * scale;
			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			srcPixelPtr =
				MLIB_POINTER_GET(lineAddr, ySrc) + 2 * xSrc;
			srcPixelPtr2 =
				(DTYPE *) ((mlib_u8 *)srcPixelPtr + srcYStride);
			k3 = t * u;
			k2 = (ONE - t) * u;
			k1 = t * (ONE - u);
			k0 = (ONE - t) * (ONE - u);
			a01_0 = srcPixelPtr[2];
			a01_1 = srcPixelPtr[3];
			a00_0 = srcPixelPtr[0];
			a00_1 = srcPixelPtr[1];
			a10_0 = srcPixelPtr2[0];
			a10_1 = srcPixelPtr2[1];
			a11_0 = srcPixelPtr2[2];
			a11_1 = srcPixelPtr2[3];
			pix0 = k0 * a00_0 + k1 * a01_0 + k2 * a10_0 +
				k3 * a11_0;
			pix1 = k0 * a00_1 + k1 * a01_1 + k2 * a10_1 +
				k3 * a11_1;
			dstPixelPtr[0] = (DTYPE) pix0;
			dstPixelPtr[1] = (DTYPE) pix1;
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
	FTYPE scale = ONE / MLIB_PREC;

	for (j = yStart; j <= yFinish; j++) {
		DTYPE *srcPixelPtr2;
		FTYPE t, u, k0, k1, k2, k3;
		FTYPE a00_0, a01_0, a10_0, a11_0;
		FTYPE a00_1, a01_1, a10_1, a11_1;
		FTYPE a00_2, a01_2, a10_2, a11_2;
		FTYPE pix0, pix1, pix2;
		__m128i Xs, Ys, dXs, dYs;
		__m128i mask128 = _mm_set1_epi32(MLIB_MASK);
		__m128 scales = _mm_set1_ps(scale);
		__m128 ones = _mm_set1_ps(ONE);
		__m128 ts, us, ts1, us1;
		__m128 k0s, k1s, k2s, k3s;
		__m128 a00s, a01s, a10s, a11s;
		__m128 pix0s, pix1s, pix2s, pix3s;

		CLIP(3);
		dstLineEnd = (DTYPE *) dstData + 3 * xRight;

		Xs = _mm_set1_epi32(X);
		Ys = _mm_set1_epi32(Y);
		dXs = _mm_set1_epi32(dX);
		dYs = _mm_set1_epi32(dY);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; dstPixelPtr <= dstLineEnd; dstPixelPtr += 3) {
			ts = _mm_cvtepi32_ps(_mm_and_si128(Xs, mask128));
			us = _mm_cvtepi32_ps(_mm_and_si128(Ys, mask128));
			ts = _mm_mul_ps(ts, scales);
			us = _mm_mul_ps(us, scales);
			ts1 = _mm_sub_ps(ones, ts);
			us1 = _mm_sub_ps(ones, us);

			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			srcPixelPtr =
				MLIB_POINTER_GET(lineAddr, ySrc) + 3 * xSrc;
			srcPixelPtr2 =
				(DTYPE *) ((mlib_u8 *)srcPixelPtr + srcYStride);

			k3s = _mm_mul_ps(ts, us);
			k2s = _mm_mul_ps(ts1, us);
			k1s = _mm_mul_ps(ts, us1);
			k0s = _mm_mul_ps(ts1, us1);

#if 1
			a00s = _mm_loadu_ps((mlib_f32 *)srcPixelPtr);
/* for better performance but not completely safe here ! */
			a01s = _mm_loadu_ps((mlib_f32 *)&srcPixelPtr[3]);
			a10s = _mm_loadu_ps((mlib_f32 *)srcPixelPtr2);
			a11s = _mm_loadu_ps((mlib_f32 *)&srcPixelPtr2[3]);
#else
			a00s = _mm_loadu_si128((__m128i *)srcPixelPtr);
			a01s = _mm_setr_epi32(
				srcPixelPtr[3],
				srcPixelPtr[4],
				srcPixelPtr[5],
				srcPixelPtr[5]);
			a10s = _mm_loadu_si128((__m128i *)srcPixelPtr2);
			a11s = _mm_setr_epi32(
				srcPixelPtr2[3],
				srcPixelPtr2[4],
				srcPixelPtr2[5],
				srcPixelPtr2[5]);
#endif

			pix0s = _mm_mul_ps(k0s, a00s);
			pix1s = _mm_mul_ps(k1s, a01s);
			pix2s = _mm_mul_ps(k2s, a10s);
			pix3s = _mm_mul_ps(k3s, a11s);
			pix0s = _mm_add_ps(pix0s, pix1s);
			pix2s = _mm_add_ps(pix2s, pix3s);
			pix0s = _mm_add_ps(pix0s, pix2s);
			_mm_storel_pi((__m64 *)dstPixelPtr, pix0s);
			pix2s = _mm_unpackhi_ps(pix0s, pix0s);
			_mm_store_ss((mlib_f32 *)(dstPixelPtr + 2), pix2s);

			Xs = _mm_add_epi32(Xs, dXs);
			Ys = _mm_add_epi32(Ys, dYs);
		}

	}

	_mm_empty();

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUN_NAME(
	4ch) (
	mlib_affine_param *param)
{
	DECLAREVAR_BL();
	DTYPE *dstLineEnd;
	FTYPE scale = ONE / MLIB_PREC;

	for (j = yStart; j <= yFinish; j++) {
		DTYPE *srcPixelPtr2;
		FTYPE t, u, k0, k1, k2, k3;
		FTYPE a00_0, a01_0, a10_0, a11_0;
		FTYPE a00_1, a01_1, a10_1, a11_1;
		FTYPE a00_2, a01_2, a10_2, a11_2;
		FTYPE a00_3, a01_3, a10_3, a11_3;
		FTYPE pix0, pix1, pix2, pix3;
		__m128i Xs, Ys, dXs, dYs;
		__m128i mask128 = _mm_set1_epi32(MLIB_MASK);
		__m128 scales = _mm_set1_ps(scale);
		__m128 ones = _mm_set1_ps(ONE);
		__m128 ts, us, ts1, us1;
		__m128 k0s, k1s, k2s, k3s;
		__m128 a00s, a01s, a10s, a11s;
		__m128 pix0s, pix1s, pix2s, pix3s;

		CLIP(4);
		dstLineEnd = (DTYPE *) dstData + 4 * xRight;

		Xs = _mm_set1_epi32(X);
		Ys = _mm_set1_epi32(Y);
		dXs = _mm_set1_epi32(dX);
		dYs = _mm_set1_epi32(dY);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; dstPixelPtr <= dstLineEnd; dstPixelPtr += 4) {
			ts = _mm_cvtepi32_ps(_mm_and_si128(Xs, mask128));
			us = _mm_cvtepi32_ps(_mm_and_si128(Ys, mask128));
			ts = _mm_mul_ps(ts, scales);
			us = _mm_mul_ps(us, scales);
			ts1 = _mm_sub_ps(ones, ts);
			us1 = _mm_sub_ps(ones, us);

			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> (MLIB_SHIFT - 2);
			X += dX;
			srcPixelPtr =
				MLIB_POINTER_GET(lineAddr, ySrc) + (xSrc & ~3);
			srcPixelPtr2 =
				(DTYPE *) ((mlib_u8 *)srcPixelPtr + srcYStride);

			k3s = _mm_mul_ps(ts, us);
			k2s = _mm_mul_ps(ts1, us);
			k1s = _mm_mul_ps(ts, us1);
			k0s = _mm_mul_ps(ts1, us1);

			a00s = _mm_loadu_ps((mlib_f32 *)srcPixelPtr);
			a01s = _mm_loadu_ps((mlib_f32 *)&srcPixelPtr[4]);
			a10s = _mm_loadu_ps((mlib_f32 *)srcPixelPtr2);
			a11s = _mm_loadu_ps((mlib_f32 *)&srcPixelPtr2[4]);

			pix0s = _mm_mul_ps(k0s, a00s);
			pix1s = _mm_mul_ps(k1s, a01s);
			pix2s = _mm_mul_ps(k2s, a10s);
			pix3s = _mm_mul_ps(k3s, a11s);
			pix0s = _mm_add_ps(pix0s, pix1s);
			pix2s = _mm_add_ps(pix2s, pix3s);
			pix0s = _mm_add_ps(pix0s, pix2s);
			_mm_storeu_ps((mlib_f32 *)dstPixelPtr, pix0s);

			Xs = _mm_add_epi32(Xs, dXs);
			Ys = _mm_add_epi32(Ys, dYs);
		}

	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
