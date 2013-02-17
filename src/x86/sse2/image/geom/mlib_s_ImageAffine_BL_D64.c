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

#pragma ident	"@(#)mlib_s_ImageAffine_BL_D64.c	9.5	07/11/05 SMI"

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

#define	DTYPE	mlib_d64
#define	FTYPE	DTYPE

#define	FUN_NAME(CHAN)	mlib_ImageAffine_d64_##CHAN##_bl

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
		__m128i Xs, Ys, dXs, dYs, Xs1, Ys1;
		__m128i mask128 = _mm_set1_epi32(MLIB_MASK);
		__m128d scales = _mm_set1_pd(scale);
		__m128d ones = _mm_set1_pd(ONE);
		__m128d ts, us, ts1, us1;
		__m128d k0s, k1s, k2s, k3s;
		__m128d a00s, a01s, a10s, a11s;
		__m128d pix0s, pix1s, pix2s, pix3s;
		__m128d ts_, us_, ts1_, us1_;
		__m128d k0s_, k1s_, k2s_, k3s_;
		__m128d a00s_, a01s_, a10s_, a11s_;
		__m128d pix0s_, pix1s_, pix2s_, pix3s_;

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
			Xs1 = _mm_and_si128(Xs, mask128);
			Ys1 = _mm_and_si128(Ys, mask128);
			ts = _mm_cvtepi32_pd(Xs1);
			us = _mm_cvtepi32_pd(Ys1);
			Xs1 = _mm_srli_si128(Xs1, 8);
			Ys1 = _mm_srli_si128(Ys1, 8);
			ts_ = _mm_cvtepi32_pd(Xs1);
			us_ = _mm_cvtepi32_pd(Ys1);
			ts = _mm_mul_pd(ts, scales);
			us = _mm_mul_pd(us, scales);
			ts_ = _mm_mul_pd(ts_, scales);
			us_ = _mm_mul_pd(us_, scales);
			ts1 = _mm_sub_pd(ones, ts);
			us1 = _mm_sub_pd(ones, us);
			ts1_ = _mm_sub_pd(ones, ts_);
			us1_ = _mm_sub_pd(ones, us_);

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

			k3s = _mm_mul_pd(ts, us);
			k2s = _mm_mul_pd(ts1, us);
			k1s = _mm_mul_pd(ts, us1);
			k0s = _mm_mul_pd(ts1, us1);
			k3s_ = _mm_mul_pd(ts_, us_);
			k2s_ = _mm_mul_pd(ts1_, us_);
			k1s_ = _mm_mul_pd(ts_, us1_);
			k0s_ = _mm_mul_pd(ts1_, us1_);

			a00s = _mm_setr_pd(
				srcPtr0[0],
				srcPtr1[0]);
			a00s_ = _mm_setr_pd(
				srcPtr2[0],
				srcPtr3[0]);
			a01s = _mm_setr_pd(
				srcPtr0[1],
				srcPtr1[1]);
			a01s_ = _mm_setr_pd(
				srcPtr2[1],
				srcPtr3[1]);
			a10s = _mm_setr_pd(
				srcPtr0[srcYStride],
				srcPtr1[srcYStride]);
			a10s_ = _mm_setr_pd(
				srcPtr2[srcYStride],
				srcPtr3[srcYStride]);
			a11s = _mm_setr_pd(
				srcPtr0[srcYStride1],
				srcPtr1[srcYStride1]);
			a11s_ = _mm_setr_pd(
				srcPtr2[srcYStride1],
				srcPtr3[srcYStride1]);

			pix0s = _mm_mul_pd(k0s, a00s);
			pix1s = _mm_mul_pd(k1s, a01s);
			pix2s = _mm_mul_pd(k2s, a10s);
			pix3s = _mm_mul_pd(k3s, a11s);
			pix0s_ = _mm_mul_pd(k0s_, a00s_);
			pix1s_ = _mm_mul_pd(k1s_, a01s_);
			pix2s_ = _mm_mul_pd(k2s_, a10s_);
			pix3s_ = _mm_mul_pd(k3s_, a11s_);

			pix0s = _mm_add_pd(pix0s, pix1s);
			pix2s = _mm_add_pd(pix2s, pix3s);
			pix0s = _mm_add_pd(pix0s, pix2s);
			pix0s_ = _mm_add_pd(pix0s_, pix1s_);
			pix2s_ = _mm_add_pd(pix2s_, pix3s_);
			pix0s_ = _mm_add_pd(pix0s_, pix2s_);

			_mm_storeu_pd((mlib_d64 *)dstPixelPtr, pix0s);
			_mm_storeu_pd((mlib_d64 *)(dstPixelPtr + 2), pix0s_);

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
		__m128i Xs, Ys, dXs, dYs, Xs1, Ys1;
		__m128i mask128 = _mm_set1_epi32(MLIB_MASK);
		__m128d scales = _mm_set1_pd(scale);
		__m128d ones = _mm_set1_pd(ONE);
		__m128d ts, us, ts1, us1;
		__m128d k0s, k1s, k2s, k3s;
		__m128d a00s, a01s, a10s, a11s;
		__m128d pix0s, pix1s, pix2s, pix3s;
		__m128d ts_, us_, ts1_, us1_;
		__m128d k0s_, k1s_, k2s_, k3s_;
		__m128d a00s_, a01s_, a10s_, a11s_;
		__m128d pix0s_, pix1s_, pix2s_, pix3s_;

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
			Xs1 = _mm_and_si128(Xs, mask128);
			Ys1 = _mm_and_si128(Ys, mask128);
			ts = _mm_cvtepi32_pd(Xs1);
			us = _mm_cvtepi32_pd(Ys1);
			Xs1 = _mm_srli_si128(Xs1, 8);
			Ys1 = _mm_srli_si128(Ys1, 8);
			ts_ = _mm_cvtepi32_pd(Xs1);
			us_ = _mm_cvtepi32_pd(Ys1);
			ts = _mm_mul_pd(ts, scales);
			us = _mm_mul_pd(us, scales);
			ts_ = _mm_mul_pd(ts_, scales);
			us_ = _mm_mul_pd(us_, scales);
			ts1 = _mm_sub_pd(ones, ts);
			us1 = _mm_sub_pd(ones, us);
			ts1_ = _mm_sub_pd(ones, ts_);
			us1_ = _mm_sub_pd(ones, us_);

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

			k3s = _mm_mul_pd(ts, us);
			k2s = _mm_mul_pd(ts1, us);
			k1s = _mm_mul_pd(ts, us1);
			k0s = _mm_mul_pd(ts1, us1);
			k3s_ = _mm_mul_pd(ts_, us_);
			k2s_ = _mm_mul_pd(ts1_, us_);
			k1s_ = _mm_mul_pd(ts_, us1_);
			k0s_ = _mm_mul_pd(ts1_, us1_);

			a00s = _mm_loadu_pd((mlib_d64 *)srcPtr0);
			a00s_ = _mm_loadu_pd((mlib_d64 *)srcPtr2);
			a01s = _mm_loadu_pd((mlib_d64 *)(srcPtr0 + 2));
			a01s_ = _mm_loadu_pd((mlib_d64 *)(srcPtr2 + 2));
			a10s = _mm_loadu_pd((mlib_d64 *)srcPtr1);
			a10s_ = _mm_loadu_pd((mlib_d64 *)srcPtr3);
			a11s = _mm_loadu_pd((mlib_d64 *)(srcPtr1 + 2));
			a11s_ = _mm_loadu_pd((mlib_d64 *)(srcPtr3 + 2));

			pix0s = _mm_mul_pd(k0s, a00s);
			pix1s = _mm_mul_pd(k1s, a01s);
			pix2s = _mm_mul_pd(k2s, a10s);
			pix3s = _mm_mul_pd(k3s, a11s);
			pix0s_ = _mm_mul_pd(k0s_, a00s_);
			pix1s_ = _mm_mul_pd(k1s_, a01s_);
			pix2s_ = _mm_mul_pd(k2s_, a10s_);
			pix3s_ = _mm_mul_pd(k3s_, a11s_);

			pix0s = _mm_add_pd(pix0s, pix1s);
			pix2s = _mm_add_pd(pix2s, pix3s);
			pix0s = _mm_add_pd(pix0s, pix2s);
			pix0s_ = _mm_add_pd(pix0s_, pix1s_);
			pix2s_ = _mm_add_pd(pix2s_, pix3s_);
			pix0s_ = _mm_add_pd(pix0s_, pix2s_);

			_mm_storeu_pd((mlib_d64 *)dstPixelPtr, pix0s);
			_mm_storeu_pd((mlib_d64 *)(dstPixelPtr + 2), pix0s_);

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
		__m128i Xs, Ys, dXs, dYs, Xs1, Ys1;
		__m128i mask128 = _mm_set1_epi32(MLIB_MASK);
		__m128d scales = _mm_set1_pd(scale);
		__m128d ones = _mm_set1_pd(ONE);
		__m128d ts, us, ts1, us1;
		__m128d k0s, k1s, k2s, k3s;
		__m128d a00s, a01s, a10s, a11s;
		__m128d pix0s, pix1s, pix2s, pix3s;
		__m128d ts_, us_, ts1_, us1_;
		__m128d k0s_, k1s_, k2s_, k3s_;
		__m128d a00s_, a01s_, a10s_, a11s_;
		__m128d pix0s_, pix1s_, pix2s_, pix3s_;

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
			Xs1 = _mm_and_si128(Xs, mask128);
			Ys1 = _mm_and_si128(Ys, mask128);
			ts = _mm_cvtepi32_pd(Xs1);
			us = _mm_cvtepi32_pd(Ys1);
			Xs1 = _mm_srli_si128(Xs1, 8);
			Ys1 = _mm_srli_si128(Ys1, 8);
			ts_ = _mm_cvtepi32_pd(Xs1);
			us_ = _mm_cvtepi32_pd(Ys1);
			ts = _mm_mul_pd(ts, scales);
			us = _mm_mul_pd(us, scales);
			ts_ = _mm_mul_pd(ts_, scales);
			us_ = _mm_mul_pd(us_, scales);
			ts1 = _mm_sub_pd(ones, ts);
			us1 = _mm_sub_pd(ones, us);
			ts1_ = _mm_sub_pd(ones, ts_);
			us1_ = _mm_sub_pd(ones, us_);

			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			srcPixelPtr =
				MLIB_POINTER_GET(lineAddr, ySrc) + 3 * xSrc;
			srcPixelPtr2 =
				(DTYPE *) ((mlib_u8 *)srcPixelPtr + srcYStride);

			k3s = _mm_mul_pd(ts, us);
			k2s = _mm_mul_pd(ts1, us);
			k1s = _mm_mul_pd(ts, us1);
			k0s = _mm_mul_pd(ts1, us1);
			k3s_ = _mm_mul_pd(ts_, us_);
			k2s_ = _mm_mul_pd(ts1_, us_);
			k1s_ = _mm_mul_pd(ts_, us1_);
			k0s_ = _mm_mul_pd(ts1_, us1_);

			a00s = _mm_loadu_pd((mlib_d64 *)srcPixelPtr);
/*
 * here _mm_loadu_pd() is used instead of _mm_loadl_pd()
 * as a workaround for CR 6549470
 * it may not be completely safe (read out of boundary)
 */
			a00s_ = _mm_loadu_pd(
				(mlib_d64 *)(srcPixelPtr + 2));
			a01s = _mm_loadu_pd((mlib_d64 *)(srcPixelPtr + 3));
			a01s_ = _mm_loadu_pd(
				(mlib_d64 *)(srcPixelPtr + 5));
			a10s = _mm_loadu_pd((mlib_d64 *)srcPixelPtr2);
			a10s_ = _mm_loadu_pd(
				(mlib_d64 *)(srcPixelPtr2 + 2));
			a11s = _mm_loadu_pd((mlib_d64 *)(srcPixelPtr2 + 3));
			a11s_ = _mm_loadu_pd(
				(mlib_d64 *)(srcPixelPtr2 + 5));

			pix0s = _mm_mul_pd(k0s, a00s);
			pix1s = _mm_mul_pd(k1s, a01s);
			pix2s = _mm_mul_pd(k2s, a10s);
			pix3s = _mm_mul_pd(k3s, a11s);
			pix0s_ = _mm_mul_pd(k0s_, a00s_);
			pix1s_ = _mm_mul_pd(k1s_, a01s_);
			pix2s_ = _mm_mul_pd(k2s_, a10s_);
			pix3s_ = _mm_mul_pd(k3s_, a11s_);
			pix0s = _mm_add_pd(pix0s, pix1s);
			pix2s = _mm_add_pd(pix2s, pix3s);
			pix0s = _mm_add_pd(pix0s, pix2s);
			pix0s_ = _mm_add_pd(pix0s_, pix1s_);
			pix2s_ = _mm_add_pd(pix2s_, pix3s_);
			pix0s_ = _mm_add_pd(pix0s_, pix2s_);
			_mm_storeu_pd((mlib_d64 *)dstPixelPtr, pix0s);
			_mm_storel_pd((mlib_d64 *)(dstPixelPtr + 2), pix0s_);

			Xs = _mm_add_epi32(Xs, dXs);
			Ys = _mm_add_epi32(Ys, dYs);
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
	FTYPE scale = ONE / MLIB_PREC;

	for (j = yStart; j <= yFinish; j++) {
		DTYPE *srcPixelPtr2;
		FTYPE t, u, k0, k1, k2, k3;
		FTYPE a00_0, a01_0, a10_0, a11_0;
		FTYPE a00_1, a01_1, a10_1, a11_1;
		FTYPE a00_2, a01_2, a10_2, a11_2;
		FTYPE a00_3, a01_3, a10_3, a11_3;
		FTYPE pix0, pix1, pix2, pix3;
		__m128i Xs, Ys, dXs, dYs, Xs1, Ys1;
		__m128i mask128 = _mm_set1_epi32(MLIB_MASK);
		__m128d scales = _mm_set1_pd(scale);
		__m128d ones = _mm_set1_pd(ONE);
		__m128d ts, us, ts1, us1;
		__m128d k0s, k1s, k2s, k3s;
		__m128d a00s, a01s, a10s, a11s;
		__m128d pix0s, pix1s, pix2s, pix3s;
		__m128d ts_, us_, ts1_, us1_;
		__m128d k0s_, k1s_, k2s_, k3s_;
		__m128d a00s_, a01s_, a10s_, a11s_;
		__m128d pix0s_, pix1s_, pix2s_, pix3s_;

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
			Xs1 = _mm_and_si128(Xs, mask128);
			Ys1 = _mm_and_si128(Ys, mask128);
			ts = _mm_cvtepi32_pd(Xs1);
			us = _mm_cvtepi32_pd(Ys1);
			Xs1 = _mm_srli_si128(Xs1, 8);
			Ys1 = _mm_srli_si128(Ys1, 8);
			ts_ = _mm_cvtepi32_pd(Xs1);
			us_ = _mm_cvtepi32_pd(Ys1);
			ts = _mm_mul_pd(ts, scales);
			us = _mm_mul_pd(us, scales);
			ts_ = _mm_mul_pd(ts_, scales);
			us_ = _mm_mul_pd(us_, scales);
			ts1 = _mm_sub_pd(ones, ts);
			us1 = _mm_sub_pd(ones, us);
			ts1_ = _mm_sub_pd(ones, ts_);
			us1_ = _mm_sub_pd(ones, us_);

			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> (MLIB_SHIFT - 2);
			X += dX;
			srcPixelPtr =
				MLIB_POINTER_GET(lineAddr, ySrc) + (xSrc & ~3);
			srcPixelPtr2 =
				(DTYPE *) ((mlib_u8 *)srcPixelPtr + srcYStride);

			k3s = _mm_mul_pd(ts, us);
			k2s = _mm_mul_pd(ts1, us);
			k1s = _mm_mul_pd(ts, us1);
			k0s = _mm_mul_pd(ts1, us1);
			k3s_ = _mm_mul_pd(ts_, us_);
			k2s_ = _mm_mul_pd(ts1_, us_);
			k1s_ = _mm_mul_pd(ts_, us1_);
			k0s_ = _mm_mul_pd(ts1_, us1_);

			a00s = _mm_loadu_pd((mlib_d64 *)srcPixelPtr);
			a00s_ = _mm_loadu_pd((mlib_d64 *)(srcPixelPtr + 2));
			a01s = _mm_loadu_pd((mlib_d64 *)(srcPixelPtr + 4));
			a01s_ = _mm_loadu_pd((mlib_d64 *)(srcPixelPtr + 6));
			a10s = _mm_loadu_pd((mlib_d64 *)srcPixelPtr2);
			a10s_ = _mm_loadu_pd((mlib_d64 *)(srcPixelPtr2 + 2));
			a11s = _mm_loadu_pd((mlib_d64 *)(srcPixelPtr2 + 4));
			a11s_ = _mm_loadu_pd((mlib_d64 *)(srcPixelPtr2 + 6));

			pix0s = _mm_mul_pd(k0s, a00s);
			pix1s = _mm_mul_pd(k1s, a01s);
			pix2s = _mm_mul_pd(k2s, a10s);
			pix3s = _mm_mul_pd(k3s, a11s);
			pix0s_ = _mm_mul_pd(k0s_, a00s_);
			pix1s_ = _mm_mul_pd(k1s_, a01s_);
			pix2s_ = _mm_mul_pd(k2s_, a10s_);
			pix3s_ = _mm_mul_pd(k3s_, a11s_);
			pix0s = _mm_add_pd(pix0s, pix1s);
			pix2s = _mm_add_pd(pix2s, pix3s);
			pix0s = _mm_add_pd(pix0s, pix2s);
			pix0s_ = _mm_add_pd(pix0s_, pix1s_);
			pix2s_ = _mm_add_pd(pix2s_, pix3s_);
			pix0s_ = _mm_add_pd(pix0s_, pix2s_);
			_mm_storeu_pd((mlib_d64 *)dstPixelPtr, pix0s);
			_mm_storeu_pd((mlib_d64 *)(dstPixelPtr + 2), pix0s_);

			Xs = _mm_add_epi32(Xs, dXs);
			Ys = _mm_add_epi32(Ys, dYs);
		}

	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
