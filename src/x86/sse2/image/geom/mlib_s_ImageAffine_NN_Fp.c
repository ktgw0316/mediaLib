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

#pragma ident	"@(#)mlib_s_ImageAffine_NN_Fp.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      Internal functions for mlib_ImageAffine with Nearest Neighbor filtering.
 */

#ifdef	__SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif
#include <mlib_ImageAffine.h>

/* *********************************************************** */

#define	LOAD_TWO_64(dst128, add64_0, add64_1)                       \
	{                                                           \
		__m128i l128, h128;                                 \
		l128 = _mm_loadl_epi64((__m128i *)add64_0);         \
		h128 = _mm_loadl_epi64((__m128i *)add64_1);         \
		dst128 = _mm_unpacklo_epi64(l128, h128);            \
	}

/* *********************************************************** */

#define	sp	srcPixelPtr
#define	dp	dstPixelPtr

/* *********************************************************** */

#undef  DTYPE
#define	DTYPE	mlib_s32

mlib_status
mlib_ImageAffine_s32_1ch_nn(
	mlib_affine_param *param)
{
	DECLAREVAR();
	DTYPE *srcPixelPtr;
	DTYPE *dstLineEnd;

	for (j = yStart; j <= yFinish; j++) {
		__m128i pix128;
		mlib_s32 *sp0, *sp1, *sp2, *sp3;

		CLIP(1);
		dstLineEnd = (DTYPE *) dstData + xRight;

		while (((mlib_addr)dp & 15) && (dp <= dstLineEnd)) {
			sp = S_PTR(Y) + (X >> MLIB_SHIFT);
			dp[0] = sp[0];

			X += dX;
			Y += dY;

			dp ++;
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; dp <= (dstLineEnd - 4); dp += 4) {
			sp0 = S_PTR(Y) + (X >> MLIB_SHIFT);
			X += dX;
			Y += dY;

			sp1 = S_PTR(Y) + (X >> MLIB_SHIFT);
			X += dX;
			Y += dY;

			sp2 = S_PTR(Y) + (X >> MLIB_SHIFT);
			X += dX;
			Y += dY;

			sp3 = S_PTR(Y) + (X >> MLIB_SHIFT);
			X += dX;
			Y += dY;

			pix128 = _mm_setr_epi32(
					sp0[0],
					sp1[0],
					sp2[0],
					sp3[0]);
			_mm_store_si128((__m128i *)dp, pix128);
		}

		for (; dp <= dstLineEnd; dp++) {
			sp = S_PTR(Y) + (X >> MLIB_SHIFT);
			dp[0] = sp[0];

			X += dX;
			Y += dY;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageAffine_s32_2ch_nn(
	mlib_affine_param *param)
{
	return (mlib_ImageAffine_d64_1ch_nn(param));
}

/* *********************************************************** */

mlib_status
mlib_ImageAffine_s32_3ch_nn(
	mlib_affine_param *param)
{
	DECLAREVAR();
	DTYPE *srcPixelPtr;
	DTYPE *dstLineEnd;

	for (j = yStart; j <= yFinish; j++) {
		__m128i pix128_0, pix128_1, pix128_2;
		mlib_s32 *sp0, *sp1, *sp2, *sp3;

		CLIP(3);
		dstLineEnd = (DTYPE *) dstData + 3 * xRight;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; dp <= (dstLineEnd - 12); dp += 12) {
			sp0 = S_PTR(Y) + 3 * (X >> MLIB_SHIFT);
			X += dX;
			Y += dY;

			sp1 = S_PTR(Y) + 3 * (X >> MLIB_SHIFT);
			X += dX;
			Y += dY;

			sp2 = S_PTR(Y) + 3 * (X >> MLIB_SHIFT);
			X += dX;
			Y += dY;

			sp3 = S_PTR(Y) + 3 * (X >> MLIB_SHIFT);
			X += dX;
			Y += dY;

			pix128_0 = _mm_setr_epi32(
					sp0[0],
					sp0[1],
					sp0[2],
					sp1[0]);

			pix128_1 = _mm_setr_epi32(
					sp1[1],
					sp1[2],
					sp2[0],
					sp2[1]);

			pix128_2 = _mm_setr_epi32(
					sp2[2],
					sp3[0],
					sp3[1],
					sp3[2]);

			_mm_storeu_si128((__m128i *)dp, pix128_0);
			_mm_storeu_si128((__m128i *)(dp + 4), pix128_1);
			_mm_storeu_si128((__m128i *)(dp + 8), pix128_2);
		}

		for (; dp <= dstLineEnd; dp += 3) {
			sp = S_PTR(Y) + 3 * (X >> MLIB_SHIFT);
			dp[0] = sp[0];
			dp[1] = sp[1];
			dp[2] = sp[2];

			X += dX;
			Y += dY;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageAffine_s32_4ch_nn(
	mlib_affine_param *param)
{
	return (mlib_ImageAffine_d64_2ch_nn(param));
}

/* *********************************************************** */

#undef  DTYPE
#define	DTYPE	mlib_d64

mlib_status
mlib_ImageAffine_d64_1ch_nn(
	mlib_affine_param *param)
{
	DECLAREVAR();
	DTYPE *srcPixelPtr;
	DTYPE *dstLineEnd;

	for (j = yStart; j <= yFinish; j++) {
		__m128i pix128;
		mlib_d64 *sp0, *sp1;
		mlib_s32 xSrc0, xSrc1;

		CLIP(1);
		dstLineEnd = (DTYPE *) dstData + xRight;

		if (!(mlib_addr)dp & 15) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; dp <= (dstLineEnd - 2); dp += 2) {
				sp0 = S_PTR(Y);
				xSrc0 = X >> MLIB_SHIFT;
				X += dX;
				Y += dY;

				sp1 = S_PTR(Y);
				xSrc1 = X >> MLIB_SHIFT;
				X += dX;
				Y += dY;

				LOAD_TWO_64(pix128, (sp0+xSrc0), (sp1+xSrc1));

				_mm_store_si128((__m128i *)dp, pix128);
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; dp <= (dstLineEnd - 2); dp += 2) {
				sp0 = S_PTR(Y);
				xSrc0 = X >> MLIB_SHIFT;
				X += dX;
				Y += dY;

				sp1 = S_PTR(Y);
				xSrc1 = X >> MLIB_SHIFT;
				X += dX;
				Y += dY;

				LOAD_TWO_64(pix128, (sp0+xSrc0), (sp1+xSrc1));

				_mm_storeu_si128((__m128i *)dp, pix128);
			}
		}

		for (; dp <= dstLineEnd; dp++) {
			sp = S_PTR(Y);
			dp[0] = sp[X >> MLIB_SHIFT];

			X += dX;
			Y += dY;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageAffine_d64_2ch_nn(
	mlib_affine_param *param)
{
	DECLAREVAR();
	DTYPE *srcPixelPtr;
	DTYPE *dstLineEnd;

	for (j = yStart; j <= yFinish; j++) {
		__m128i pix128;

		CLIP(2);
		dstLineEnd = (DTYPE *) dstData + 2 * xRight;

		if (!(mlib_addr)dp & 15) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; dp <= dstLineEnd; dp += 2) {
				sp = S_PTR(Y) + 2 * (X >> MLIB_SHIFT);
				X += dX;
				Y += dY;

				LOAD_TWO_64(pix128, sp, (sp+1));

				_mm_store_si128((__m128i *)dp, pix128);
			}
		} else {
			for (; dp <= dstLineEnd; dp += 2) {
				sp = S_PTR(Y) + 2 * (X >> MLIB_SHIFT);
				X += dX;
				Y += dY;

				LOAD_TWO_64(pix128, sp, (sp+1));

				_mm_storeu_si128((__m128i *)dp, pix128);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageAffine_d64_3ch_nn(
	mlib_affine_param *param)
{
	DECLAREVAR();
	DTYPE *srcPixelPtr;
	DTYPE *dstLineEnd;

	for (j = yStart; j <= yFinish; j++) {
		__m128i pix128_0, pix128_1, pix128_2;
		mlib_d64 *sp0, *sp1;

		CLIP(3);
		dstLineEnd = (DTYPE *) dstData + 3 * xRight;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; dp <= (dstLineEnd - 6); dp += 6) {
			sp0 = S_PTR(Y) + 3 * (X >> MLIB_SHIFT);
			X += dX;
			Y += dY;

			sp1 = S_PTR(Y) + 3 * (X >> MLIB_SHIFT);
			X += dX;
			Y += dY;

			LOAD_TWO_64(pix128_0, sp0, (sp0+1));
			LOAD_TWO_64(pix128_1, (sp0+2), sp1);
			LOAD_TWO_64(pix128_2, (sp1+1), (sp1+2));

			_mm_storeu_si128((__m128i *)dp, pix128_0);
			_mm_storeu_si128((__m128i *)(dp + 2), pix128_1);
			_mm_storeu_si128((__m128i *)(dp + 4), pix128_2);
		}

		for (; dp <= dstLineEnd; dp += 3) {
			sp = S_PTR(Y) + 3 * (X >> MLIB_SHIFT);
			dp[0] = sp[0];
			dp[1] = sp[1];
			dp[2] = sp[2];

			X += dX;
			Y += dY;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageAffine_d64_4ch_nn(
	mlib_affine_param *param)
{
	DECLAREVAR();
	DTYPE *srcPixelPtr;
	DTYPE *dstLineEnd;

	for (j = yStart; j <= yFinish; j++) {
		__m128i pix128_0, pix128_1;

		CLIP(4);
		dstLineEnd = (DTYPE *) dstData + 4 * xRight;

		if (!(mlib_addr)dp & 15) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; dp <= dstLineEnd; dp += 4) {
				sp = S_PTR(Y) + 4 * (X >> MLIB_SHIFT);
				X += dX;
				Y += dY;

				LOAD_TWO_64(pix128_0, sp, (sp+1));
				LOAD_TWO_64(pix128_1, (sp+2), (sp+3));

				_mm_store_si128((__m128i *)dp, pix128_0);
				_mm_store_si128((__m128i *)(dp + 2), pix128_1);
			}
		} else {
			for (; dp <= dstLineEnd; dp += 4) {
				sp = S_PTR(Y) + 4 * (X >> MLIB_SHIFT);
				X += dX;
				Y += dY;

				LOAD_TWO_64(pix128_0, sp, (sp+1));
				LOAD_TWO_64(pix128_1, (sp+2), (sp+3));

				_mm_storeu_si128((__m128i *)dp, pix128_0);
				_mm_storeu_si128((__m128i *)(dp + 2), pix128_1);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
