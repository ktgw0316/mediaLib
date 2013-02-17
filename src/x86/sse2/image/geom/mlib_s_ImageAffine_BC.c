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

#pragma ident	"@(#)mlib_s_ImageAffine_BC.c	9.5	07/11/05 SMI"

/*
 * FUNCTION
 *      Image affine transformation with Bicubic filtering
 * SYNOPSIS
 *      mlib_status mlib_ImageAffine_[u8|s16|u16]_?ch_bc(mlib_s32 *leftEdges,
 *                                                       mlib_s32 *rightEdges,
 *                                                       mlib_s32 *xStarts,
 *                                                       mlib_s32 *yStarts,
 *                                                       mlib_s32 *sides,
 *                                                       mlib_u8  *dstData,
 *                                                       mlib_u8  **lineAddr,
 *                                                       mlib_s32 dstYStride,
 *                                                       mlib_s32 is_affine,
 *                                                       mlib_s32 srcYStride,
 *                                                       mlib_filter filter)
 *
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
 *      filter     type of resampling filter
 *
 * DESCRIPTION
 *      The functions step along the lines from xLeft to xRight and apply
 *      the bicubic filtering.
 *
 */

#include <mlib_s_ImageFilters.h>
#include <mlib_ImageAffine.h>
#ifdef	__SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

#define	DTYPE	mlib_u8

#define	FUN_NAME(CHAN)	mlib_ImageAffine_u8_##CHAN##_bc

#define	FILTER_BITS	9

/* *********************************************************** */

#define	SHIFT_X	13
#define	ROUND_X	0	/* (1 << (SHIFT_X - 1)) */

/* *********************************************************** */

#define	SHIFT_Y	(15 + 15 - SHIFT_X)
#define	ROUND_Y	(1 << (SHIFT_Y - 1))

/* *********************************************************** */

/*
 * Test for the presence of any "1" bit in bits
 *   8 to 31 of val. If present, then val is either
 *   negative or >255. If over/underflows of 8 bits
 *   are uncommon, then this technique can be a win,
 *   since only a single test, rather than two, is
 *   necessary to determine if clamping is needed.
 *   On the other hand, if over/underflows are common,
 *   it adds an extra test.
 */
#define	S32_TO_U8_SAT(DST)                                      \
	if (val0 & 0xffffff00) {                                \
	    if (val0 < MLIB_U8_MIN)                             \
		DST = MLIB_U8_MIN;                              \
	    else                                                \
		DST = MLIB_U8_MAX;                              \
	} else {                                                \
	    DST = (mlib_u8)val0;                                \
	}

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
	DECLAREVAR_BC();
	DTYPE *dstLineEnd;
	const mlib_s16 *mlib_filters_table;
	const mlib_s16 *mlib_filters_table_4;
	__m128i zeros = _mm_setzero_si128();

	if (filter == MLIB_BICUBIC) {
		mlib_filters_table = (mlib_s16 *)mlib_filters_s16_bc;
		mlib_filters_table_4 = (mlib_s16 *)mlib_filters_s16_bc_4;
	} else {
		mlib_filters_table = (mlib_s16 *)mlib_filters_s16_bc2;
		mlib_filters_table_4 = (mlib_s16 *)mlib_filters_s16_bc2_4;
	}

	for (j = yStart; j <= yFinish; j++) {
		__m128i xfs;
		__m128i yfs0, yfs1, yfs2, yfs3;
		__m128i d0s, d1s, d2s, d3s;

		mlib_s32 xf0, xf1, xf2, xf3;
		mlib_s32 yf0, yf1, yf2, yf3;
		mlib_s32 c0, c1, c2, c3, val0;
		mlib_s32 filterpos;
		mlib_s16 *fptr;
		mlib_s16 *fxptr0, *fxptr1;
		mlib_s16 *fyptr0, *fyptr1;
		mlib_u8 s0, s1, s2, s3;

		mlib_u8 *srcPtr00, *srcPtr01, *srcPtr02, *srcPtr03;
		mlib_u8 *srcPtr10, *srcPtr11, *srcPtr12, *srcPtr13;

		CLIP(1);
		dstLineEnd = (DTYPE *) dstData + xRight;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; dstPixelPtr <= (dstLineEnd - 1); dstPixelPtr += 2) {
			filterpos = (X >> FILTER_SHIFT) & FILTER_MASK;
			fxptr0 = (mlib_s16 *)((mlib_u8 *)mlib_filters_table +
				filterpos);

			filterpos = (Y >> FILTER_SHIFT) & FILTER_MASK;
			fyptr0 = (mlib_s16 *)((mlib_u8 *)mlib_filters_table_4 +
				4 * filterpos);

			xSrc = (X >> MLIB_SHIFT) - 1;
			ySrc = (Y >> MLIB_SHIFT) - 1;

			srcPtr00 = ((DTYPE **) lineAddr)[ySrc] + xSrc;
			srcPtr01 =
				(DTYPE *) ((mlib_addr)srcPtr00 + srcYStride);
			srcPtr02 = (DTYPE *)
				((mlib_addr)srcPtr00 + 2 * srcYStride);
			srcPtr03 = (DTYPE *)
				((mlib_addr)srcPtr00 + 3 * srcYStride);

			X += dX;
			Y += dY;


			filterpos = (X >> FILTER_SHIFT) & FILTER_MASK;
			fxptr1 = (mlib_s16 *)((mlib_u8 *)mlib_filters_table +
				filterpos);

			filterpos = (Y >> FILTER_SHIFT) & FILTER_MASK;
			fyptr1 = (mlib_s16 *)((mlib_u8 *)mlib_filters_table_4 +
				4 * filterpos);

			xSrc = (X >> MLIB_SHIFT) - 1;
			ySrc = (Y >> MLIB_SHIFT) - 1;

			srcPtr10 = ((DTYPE **) lineAddr)[ySrc] + xSrc;
			srcPtr11 =
				(DTYPE *) ((mlib_addr)srcPtr10 + srcYStride);
			srcPtr12 = (DTYPE *)
				((mlib_addr)srcPtr10 + 2 * srcYStride);
			srcPtr13 = (DTYPE *)
				((mlib_addr)srcPtr10 + 3 * srcYStride);

			X += dX;
			Y += dY;

			LOAD_TWO_64(xfs, fxptr0, fxptr1);
			LOAD_TWO_64(yfs0, fyptr0, fyptr1);
			LOAD_TWO_64(yfs1, (fyptr0+4), (fyptr1+4));
			LOAD_TWO_64(yfs2, (fyptr0+8), (fyptr1+8));
			LOAD_TWO_64(yfs3, (fyptr0+12), (fyptr1+12));

			d0s = _mm_setr_epi32(
				*(mlib_s32 *)srcPtr00,
				*(mlib_s32 *)srcPtr10,
				*(mlib_s32 *)srcPtr01,
				*(mlib_s32 *)srcPtr11);
			d2s = _mm_setr_epi32(
				*(mlib_s32 *)srcPtr02,
				*(mlib_s32 *)srcPtr12,
				*(mlib_s32 *)srcPtr03,
				*(mlib_s32 *)srcPtr13);
			d1s = _mm_unpackhi_epi8(zeros, d0s);
			d0s = _mm_unpacklo_epi8(zeros, d0s);
			d3s = _mm_unpackhi_epi8(zeros, d2s);
			d2s = _mm_unpacklo_epi8(zeros, d2s);

			d0s = _mm_srli_epi16(d0s, 1);
			d1s = _mm_srli_epi16(d1s, 1);
			d2s = _mm_srli_epi16(d2s, 1);
			d3s = _mm_srli_epi16(d3s, 1);

			d0s = _mm_mulhi_epi16(d0s, yfs0);
			d1s = _mm_mulhi_epi16(d1s, yfs1);
			d2s = _mm_mulhi_epi16(d2s, yfs2);
			d3s = _mm_mulhi_epi16(d3s, yfs3);

			d0s = _mm_add_epi16(d0s, d1s);
			d2s = _mm_add_epi16(d2s, d3s);
			d0s = _mm_add_epi16(d0s, d2s);

			d0s = _mm_madd_epi16(d0s, xfs);
			d0s = _mm_add_epi32(d0s, _mm_srli_epi64(d0s, 32));

			d0s = _mm_srai_epi16(d0s, 5);
			d0s = _mm_packus_epi16(d0s, d0s);
			val0 = _mm_cvtsi128_si32(d0s);
			dstPixelPtr[0] = (val0 >> 8);
			d0s = _mm_srli_si128(d0s, 4);
			val0 = _mm_cvtsi128_si32(d0s);
			dstPixelPtr[1] = (val0 >> 8);
		}

		for (; dstPixelPtr <= dstLineEnd; dstPixelPtr++) {
			filterpos = (X >> FILTER_SHIFT) & FILTER_MASK;
			fptr = (mlib_s16 *)((mlib_u8 *)mlib_filters_table +
				filterpos);

			xf0 = fptr[0];
			xf1 = fptr[1];
			xf2 = fptr[2];
			xf3 = fptr[3];

			filterpos = (Y >> FILTER_SHIFT) & FILTER_MASK;
			fptr = (mlib_s16 *)((mlib_u8 *)mlib_filters_table +
				filterpos);

			yf0 = fptr[0];
			yf1 = fptr[1];
			yf2 = fptr[2];
			yf3 = fptr[3];

			xSrc = (X >> MLIB_SHIFT) - 1;
			ySrc = (Y >> MLIB_SHIFT) - 1;

			srcPixelPtr = ((DTYPE **) lineAddr)[ySrc] + xSrc;
			s0 = srcPixelPtr[0];
			s1 = srcPixelPtr[1];
			s2 = srcPixelPtr[2];
			s3 = srcPixelPtr[3];

			c0 = (s0 * xf0 + s1 * xf1 + s2 * xf2 + s3 * xf3 +
				ROUND_X) >> SHIFT_X;
			srcPixelPtr =
				(DTYPE *) ((mlib_addr)srcPixelPtr + srcYStride);
			c1 = (srcPixelPtr[0] * xf0 + srcPixelPtr[1] * xf1 +
				srcPixelPtr[2] * xf2 + srcPixelPtr[3] * xf3 +
				ROUND_X) >> SHIFT_X;
			srcPixelPtr =
				(DTYPE *) ((mlib_addr)srcPixelPtr + srcYStride);
			c2 = (srcPixelPtr[0] * xf0 + srcPixelPtr[1] * xf1 +
				srcPixelPtr[2] * xf2 + srcPixelPtr[3] * xf3 +
				ROUND_X) >> SHIFT_X;
			srcPixelPtr =
				(DTYPE *) ((mlib_addr)srcPixelPtr + srcYStride);
			c3 = (srcPixelPtr[0] * xf0 + srcPixelPtr[1] * xf1 +
				srcPixelPtr[2] * xf2 + srcPixelPtr[3] * xf3 +
				ROUND_X) >> SHIFT_X;
			val0 = (c0 * yf0 + c1 * yf1 + c2 * yf2 + c3 * yf3 +
				ROUND_Y) >> SHIFT_Y;

			S32_TO_U8_SAT(dstPixelPtr[0]);

			X += dX;
			Y += dY;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUN_NAME(
	2ch) (
	mlib_affine_param *param)
{
	DECLAREVAR_BC();
	DTYPE *dstLineEnd;
	const mlib_s16 *mlib_filters_table;
	const mlib_s16 *mlib_filters_table_4;
	__m128i zeros = _mm_setzero_si128();

	if (filter == MLIB_BICUBIC) {
		mlib_filters_table = (mlib_s16 *)mlib_filters_s16_bc;
		mlib_filters_table_4 = (mlib_s16 *)mlib_filters_s16_bc_4;
	} else {
		mlib_filters_table = (mlib_s16 *)mlib_filters_s16_bc2;
		mlib_filters_table_4 = (mlib_s16 *)mlib_filters_s16_bc2_4;
	}

	for (j = yStart; j <= yFinish; j++) {
		__m128i xfs;
		__m128i yfs0, yfs1, yfs2, yfs3;
		__m128i d0s, d1s, d2s, d3s;

		mlib_s32 xf0, xf1, xf2, xf3;
		mlib_s32 yf0, yf1, yf2, yf3;
		mlib_s32 c0, c1, c2, c3, val0;
		mlib_s32 filterpos;
		mlib_s16 *fptr;
		mlib_s16 *fxptr0, *fxptr1;
		mlib_s16 *fyptr0, *fyptr1;
		mlib_u8 s0, s1, s2, s3;

		mlib_u8 *srcPtr00, *srcPtr01, *srcPtr02, *srcPtr03;

		CLIP(2);
		dstLineEnd = (DTYPE *) dstData + 2 * xRight;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; dstPixelPtr <= dstLineEnd; dstPixelPtr += 2) {
			filterpos = (X >> FILTER_SHIFT) & FILTER_MASK;
			fxptr0 = (mlib_s16 *)((mlib_u8 *)mlib_filters_table +
				filterpos);

			filterpos = (Y >> FILTER_SHIFT) & FILTER_MASK;
			fyptr0 = (mlib_s16 *)((mlib_u8 *)mlib_filters_table_4 +
				4 * filterpos);

			xSrc = (X >> MLIB_SHIFT) - 1;
			ySrc = (Y >> MLIB_SHIFT) - 1;

			srcPtr00 = ((DTYPE **) lineAddr)[ySrc] + 2 * xSrc;
			srcPtr01 =
				(DTYPE *) ((mlib_addr)srcPtr00 + srcYStride);
			srcPtr02 = (DTYPE *)
				((mlib_addr)srcPtr00 + 2 * srcYStride);
			srcPtr03 = (DTYPE *)
				((mlib_addr)srcPtr00 + 3 * srcYStride);

			X += dX;
			Y += dY;

			xfs = _mm_loadl_epi64((__m128i *)fxptr0);
			xfs = _mm_unpacklo_epi16(xfs, xfs);

			yfs0 = _mm_loadl_epi64((__m128i *)fyptr0);
			yfs0 = _mm_unpacklo_epi16(yfs0, yfs0);

			yfs1 = _mm_loadl_epi64((__m128i *)(fyptr0 + 4));
			yfs1 = _mm_unpacklo_epi16(yfs1, yfs1);

			yfs2 = _mm_loadl_epi64((__m128i *)(fyptr0 + 8));
			yfs2 = _mm_unpacklo_epi16(yfs2, yfs2);

			yfs3 = _mm_loadl_epi64((__m128i *)(fyptr0 + 12));
			yfs3 = _mm_unpacklo_epi16(yfs3, yfs3);

			d0s = _mm_loadl_epi64((__m128i *)srcPtr00);
			d1s = _mm_loadl_epi64((__m128i *)srcPtr01);
			d2s = _mm_loadl_epi64((__m128i *)srcPtr02);
			d3s = _mm_loadl_epi64((__m128i *)srcPtr03);
			d0s = _mm_unpacklo_epi8(zeros, d0s);
			d1s = _mm_unpacklo_epi8(zeros, d1s);
			d2s = _mm_unpacklo_epi8(zeros, d2s);
			d3s = _mm_unpacklo_epi8(zeros, d3s);

			d0s = _mm_srli_epi16(d0s, 1);
			d1s = _mm_srli_epi16(d1s, 1);
			d2s = _mm_srli_epi16(d2s, 1);
			d3s = _mm_srli_epi16(d3s, 1);

			d0s = _mm_mulhi_epi16(d0s, yfs0);
			d1s = _mm_mulhi_epi16(d1s, yfs1);
			d2s = _mm_mulhi_epi16(d2s, yfs2);
			d3s = _mm_mulhi_epi16(d3s, yfs3);

			d0s = _mm_add_epi16(d0s, d1s);
			d2s = _mm_add_epi16(d2s, d3s);
			d0s = _mm_add_epi16(d0s, d2s);

			d0s = _mm_mulhi_epi16(d0s, xfs);
			d0s = _mm_add_epi16(d0s, _mm_srli_epi64(d0s, 32));
			d0s = _mm_add_epi16(d0s, _mm_srli_si128(d0s, 8));

			d0s = _mm_srai_epi16(d0s, 5);
			d0s = _mm_packus_epi16(d0s, d0s);
			val0 = _mm_cvtsi128_si32(d0s);
			*(mlib_s16 *)dstPixelPtr = val0;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUN_NAME(
	3ch) (
	mlib_affine_param *param)
{
	DECLAREVAR_BC();
	DTYPE *dstLineEnd;
	const mlib_s16 *mlib_filters_table;
	const mlib_s16 *mlib_filters_table_4;
	__m128i zeros = _mm_setzero_si128();

	if (filter == MLIB_BICUBIC) {
		mlib_filters_table = (mlib_s16 *)mlib_filters_s16_bc;
		mlib_filters_table_4 = (mlib_s16 *)mlib_filters_s16_bc_4;
	} else {
		mlib_filters_table = (mlib_s16 *)mlib_filters_s16_bc2;
		mlib_filters_table_4 = (mlib_s16 *)mlib_filters_s16_bc2_4;
	}

	for (j = yStart; j <= yFinish; j++) {
		__m128i xfs, xfs0, xfs1;
		__m128i yfs0, yfs1, yfs2, yfs3;
		__m128i d0s, d1s, d2s, d3s;
		__m128i d4s, d5s, d6s, d7s;

		mlib_s32 xf0, xf1, xf2, xf3;
		mlib_s32 yf0, yf1, yf2, yf3;
		mlib_s32 c0, c1, c2, c3, val0;
		mlib_s32 filterpos;
		mlib_s16 *fptr;
		mlib_s16 *fxptr0, *fxptr1;
		mlib_s16 *fyptr0, *fyptr1;
		mlib_u8 s0, s1, s2, s3;

		mlib_u8 *srcPtr00, *srcPtr01, *srcPtr02, *srcPtr03;

		CLIP(3);
		dstLineEnd = (DTYPE *) dstData + 3 * xRight;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; dstPixelPtr <= dstLineEnd; dstPixelPtr += 3) {
			filterpos = (X >> FILTER_SHIFT) & FILTER_MASK;
			fxptr0 = (mlib_s16 *)((mlib_u8 *)mlib_filters_table +
				filterpos);

			filterpos = (Y >> FILTER_SHIFT) & FILTER_MASK;
			fyptr0 = (mlib_s16 *)((mlib_u8 *)mlib_filters_table_4 +
				4 * filterpos);

			xSrc = (X >> MLIB_SHIFT) - 1;
			ySrc = (Y >> MLIB_SHIFT) - 1;

			srcPtr00 = ((DTYPE **) lineAddr)[ySrc] + 3 * xSrc;
			srcPtr01 =
				(DTYPE *) ((mlib_addr)srcPtr00 + srcYStride);
			srcPtr02 = (DTYPE *)
				((mlib_addr)srcPtr00 + 2 * srcYStride);
			srcPtr03 = (DTYPE *)
				((mlib_addr)srcPtr00 + 3 * srcYStride);

			X += dX;
			Y += dY;

			xfs = _mm_loadl_epi64((__m128i *)fxptr0);
			xfs = _mm_unpacklo_epi16(xfs, xfs);
			xfs0 = _mm_unpacklo_epi32(xfs, xfs);
			xfs1 = _mm_unpackhi_epi32(xfs, xfs);

			yfs0 = _mm_loadl_epi64((__m128i *)fyptr0);
			yfs0 = _mm_unpacklo_epi16(yfs0, yfs0);

			yfs1 = _mm_loadl_epi64((__m128i *)(fyptr0 + 4));
			yfs1 = _mm_unpacklo_epi16(yfs1, yfs1);

			yfs2 = _mm_loadl_epi64((__m128i *)(fyptr0 + 8));
			yfs2 = _mm_unpacklo_epi16(yfs2, yfs2);

			yfs3 = _mm_loadl_epi64((__m128i *)(fyptr0 + 12));
			yfs3 = _mm_unpacklo_epi16(yfs3, yfs3);

			d0s = _mm_loadu_si128((__m128i *)srcPtr00);
			d1s = _mm_loadu_si128((__m128i *)srcPtr01);
			d2s = _mm_loadu_si128((__m128i *)srcPtr02);
			d3s = _mm_loadu_si128((__m128i *)srcPtr03);
			d4s = _mm_unpackhi_epi8(zeros, d0s);
			d5s = _mm_unpackhi_epi8(zeros, d1s);
			d6s = _mm_unpackhi_epi8(zeros, d2s);
			d7s = _mm_unpackhi_epi8(zeros, d3s);
			d0s = _mm_unpacklo_epi8(zeros, d0s);
			d1s = _mm_unpacklo_epi8(zeros, d1s);
			d2s = _mm_unpacklo_epi8(zeros, d2s);
			d3s = _mm_unpacklo_epi8(zeros, d3s);

			d0s = _mm_srli_epi16(d0s, 1);
			d1s = _mm_srli_epi16(d1s, 1);
			d2s = _mm_srli_epi16(d2s, 1);
			d3s = _mm_srli_epi16(d3s, 1);
			d4s = _mm_srli_epi16(d4s, 1);
			d5s = _mm_srli_epi16(d5s, 1);
			d6s = _mm_srli_epi16(d6s, 1);
			d7s = _mm_srli_epi16(d7s, 1);

			d0s = _mm_mulhi_epi16(d0s, yfs0);
			d1s = _mm_mulhi_epi16(d1s, yfs1);
			d2s = _mm_mulhi_epi16(d2s, yfs2);
			d3s = _mm_mulhi_epi16(d3s, yfs3);
			d4s = _mm_mulhi_epi16(d4s, yfs0);
			d5s = _mm_mulhi_epi16(d5s, yfs1);
			d6s = _mm_mulhi_epi16(d6s, yfs2);
			d7s = _mm_mulhi_epi16(d7s, yfs3);

			d0s = _mm_add_epi16(d0s, d1s);
			d2s = _mm_add_epi16(d2s, d3s);
			d0s = _mm_add_epi16(d0s, d2s);
			d4s = _mm_add_epi16(d4s, d5s);
			d6s = _mm_add_epi16(d6s, d7s);
			d4s = _mm_add_epi16(d4s, d6s);

			d1s = _mm_srli_si128(d0s, 6);
			d5s = _mm_srli_si128(d0s, 8);
			d5s = _mm_unpacklo_epi64(d5s, d4s);
			d4s = _mm_srli_si128(d5s, 4);
			d5s = _mm_srli_si128(d5s, 10);
			d0s = _mm_unpacklo_epi64(d0s, d1s);
			d4s = _mm_unpacklo_epi64(d4s, d5s);

			d0s = _mm_mulhi_epi16(d0s, xfs0);
			d4s = _mm_mulhi_epi16(d4s, xfs1);
			d0s = _mm_add_epi16(d0s, d4s);
			d0s = _mm_add_epi16(d0s, _mm_srli_si128(d0s, 8));

			d0s = _mm_srai_epi16(d0s, 5);
			d0s = _mm_packus_epi16(d0s, d0s);
			val0 = _mm_cvtsi128_si32(d0s);
			*(mlib_s16 *)dstPixelPtr = val0;
			dstPixelPtr[2] = (val0 >> 16);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUN_NAME(
	4ch) (
	mlib_affine_param *param)
{
	DECLAREVAR_BC();
	DTYPE *dstLineEnd;
	const mlib_s16 *mlib_filters_table;
	const mlib_s16 *mlib_filters_table_4;
	__m128i zeros = _mm_setzero_si128();

	if (filter == MLIB_BICUBIC) {
		mlib_filters_table = (mlib_s16 *)mlib_filters_s16_bc;
		mlib_filters_table_4 = (mlib_s16 *)mlib_filters_s16_bc_4;
	} else {
		mlib_filters_table = (mlib_s16 *)mlib_filters_s16_bc2;
		mlib_filters_table_4 = (mlib_s16 *)mlib_filters_s16_bc2_4;
	}

	for (j = yStart; j <= yFinish; j++) {
		__m128i xfs, xfs0, xfs1;
		__m128i yfs0, yfs1, yfs2, yfs3;
		__m128i d0s, d1s, d2s, d3s;
		__m128i d4s, d5s, d6s, d7s;

		mlib_s32 xf0, xf1, xf2, xf3;
		mlib_s32 yf0, yf1, yf2, yf3;
		mlib_s32 c0, c1, c2, c3, val0;
		mlib_s32 filterpos;
		mlib_s16 *fptr;
		mlib_s16 *fxptr0, *fxptr1;
		mlib_s16 *fyptr0, *fyptr1;
		mlib_u8 s0, s1, s2, s3;

		mlib_u8 *srcPtr00, *srcPtr01, *srcPtr02, *srcPtr03;

		CLIP(4);
		dstLineEnd = (DTYPE *) dstData + 4 * xRight;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; dstPixelPtr <= dstLineEnd; dstPixelPtr += 4) {
			filterpos = (X >> FILTER_SHIFT) & FILTER_MASK;
			fxptr0 = (mlib_s16 *)((mlib_u8 *)mlib_filters_table +
				filterpos);

			filterpos = (Y >> FILTER_SHIFT) & FILTER_MASK;
			fyptr0 = (mlib_s16 *)((mlib_u8 *)mlib_filters_table_4 +
				4 * filterpos);

			xSrc = (X >> MLIB_SHIFT) - 1;
			ySrc = (Y >> MLIB_SHIFT) - 1;

			srcPtr00 = ((DTYPE **) lineAddr)[ySrc] + 4 * xSrc;
			srcPtr01 =
				(DTYPE *) ((mlib_addr)srcPtr00 + srcYStride);
			srcPtr02 = (DTYPE *)
				((mlib_addr)srcPtr00 + 2 * srcYStride);
			srcPtr03 = (DTYPE *)
				((mlib_addr)srcPtr00 + 3 * srcYStride);

			X += dX;
			Y += dY;

			xfs = _mm_loadl_epi64((__m128i *)fxptr0);
			xfs = _mm_unpacklo_epi16(xfs, xfs);
			xfs0 = _mm_unpacklo_epi32(xfs, xfs);
			xfs1 = _mm_unpackhi_epi32(xfs, xfs);

			yfs0 = _mm_loadl_epi64((__m128i *)fyptr0);
			yfs0 = _mm_unpacklo_epi16(yfs0, yfs0);

			yfs1 = _mm_loadl_epi64((__m128i *)(fyptr0 + 4));
			yfs1 = _mm_unpacklo_epi16(yfs1, yfs1);

			yfs2 = _mm_loadl_epi64((__m128i *)(fyptr0 + 8));
			yfs2 = _mm_unpacklo_epi16(yfs2, yfs2);

			yfs3 = _mm_loadl_epi64((__m128i *)(fyptr0 + 12));
			yfs3 = _mm_unpacklo_epi16(yfs3, yfs3);

			d0s = _mm_loadu_si128((__m128i *)srcPtr00);
			d1s = _mm_loadu_si128((__m128i *)srcPtr01);
			d2s = _mm_loadu_si128((__m128i *)srcPtr02);
			d3s = _mm_loadu_si128((__m128i *)srcPtr03);
			d4s = _mm_unpackhi_epi8(zeros, d0s);
			d5s = _mm_unpackhi_epi8(zeros, d1s);
			d6s = _mm_unpackhi_epi8(zeros, d2s);
			d7s = _mm_unpackhi_epi8(zeros, d3s);
			d0s = _mm_unpacklo_epi8(zeros, d0s);
			d1s = _mm_unpacklo_epi8(zeros, d1s);
			d2s = _mm_unpacklo_epi8(zeros, d2s);
			d3s = _mm_unpacklo_epi8(zeros, d3s);

			d0s = _mm_srli_epi16(d0s, 1);
			d1s = _mm_srli_epi16(d1s, 1);
			d2s = _mm_srli_epi16(d2s, 1);
			d3s = _mm_srli_epi16(d3s, 1);
			d4s = _mm_srli_epi16(d4s, 1);
			d5s = _mm_srli_epi16(d5s, 1);
			d6s = _mm_srli_epi16(d6s, 1);
			d7s = _mm_srli_epi16(d7s, 1);

			d0s = _mm_mulhi_epi16(d0s, yfs0);
			d1s = _mm_mulhi_epi16(d1s, yfs1);
			d2s = _mm_mulhi_epi16(d2s, yfs2);
			d3s = _mm_mulhi_epi16(d3s, yfs3);
			d4s = _mm_mulhi_epi16(d4s, yfs0);
			d5s = _mm_mulhi_epi16(d5s, yfs1);
			d6s = _mm_mulhi_epi16(d6s, yfs2);
			d7s = _mm_mulhi_epi16(d7s, yfs3);

			d0s = _mm_add_epi16(d0s, d1s);
			d2s = _mm_add_epi16(d2s, d3s);
			d0s = _mm_add_epi16(d0s, d2s);
			d4s = _mm_add_epi16(d4s, d5s);
			d6s = _mm_add_epi16(d6s, d7s);
			d4s = _mm_add_epi16(d4s, d6s);

			d0s = _mm_mulhi_epi16(d0s, xfs0);
			d4s = _mm_mulhi_epi16(d4s, xfs1);
			d0s = _mm_add_epi16(d0s, d4s);
			d0s = _mm_add_epi16(d0s, _mm_srli_si128(d0s, 8));

			d0s = _mm_srai_epi16(d0s, 5);
			d0s = _mm_packus_epi16(d0s, d0s);
			val0 = _mm_cvtsi128_si32(d0s);
			*(mlib_s32 *)dstPixelPtr = val0;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
