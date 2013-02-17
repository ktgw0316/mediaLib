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

#pragma ident	"@(#)mlib_s_ImageFilteredSubsample_U8.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *
 *      mlib_ImageFilteredSubsample - Antialias filter and subsample an image
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageFilteredSubsample(mlib_image  *dst,
 *                                              const mlib_image  *src,
 *                                              mlib_s32    scaleX,
 *                                              mlib_s32    scaleY,
 *                                              mlib_s32    transX,
 *                                              mlib_s32    transY,
 *                                              const mlib_d64    *hKernel,
 *                                              const mlib_d64    *vKernel,
 *                                              mlib_s32    hSize,
 *                                              mlib_s32    vSize,
 *                                              mlib_s32    hParity,
 *                                              mlib_s32    vParity,
 *                                              mlib_edge   edge)
 *
 * ARGUMENTS
 *
 *      dst       Pointer to the destination image.
 *      src       Pointer to the source image.
 *      scaleX    The x scale factor of subsampling.
 *      scaleY    The y scale factor of subsampling.
 *      transX    The x translation.
 *      transY    The y translation.
 *      hKernel   Pointer to the compact form of the horizontal kernel.
 *      vKernel   Pointer to the compact form of the vertical kernel.
 *      hSize     Size of the array hKernel.
 *      vSize     Size of the array vKernel.
 *      hParity   Parity of the horizontal kernel (0 => even, 1 => odd).
 *      vParity   Parity of the vertical kernel (0 => even, 1 => odd).
 *      edge      Type of edge condition.
 *
 * DESCRIPTION
 *
 *      An operator to antialias filter and subsample images.  The effect of
 *      this operation on an image is equivalent to performing convolution
 *      (filter) followed by subsample (zoom out) operations.
 *
 *      This operator is similar to the image ZoomTranslate operator.  But
 *      they have different definitions on scale factors and translations,
 *      hence use different coordinate mapping equations.  The scaleX and
 *      scaleY used by FilteredSubsample are the reciprocals of the zoomx
 *      and zoomy, respectively, used by ZoomTranslate.
 *
 *      The FilteredSubsample operator mapping equations are given by
 *
 *          xS = xD*scaleX + transX
 *          yS = yD*scaleY + transY
 *
 *      where, a point (xD, yD) in the destination image is backward mapped
 *      to a point (xS, yS) in the source image.  The arguments transX and
 *      transY are provided to support tiling.
 *
 *      The subsample terms, i.e., the scale factors scaleX and scaleY,
 *      are restricted to positive integral values.  Geometrically, one
 *      destination pixel maps to scaleX by scaleY source pixels.
 *      With odd scale factors, destination pixel centers map directly onto
 *      source pixel centers.  With even scale factors, destination pixels
 *      map squarely between source pixel centers.  Below are examples of
 *      even, odd, and combination cases.
 *
 *          s = source pixel centers
 *          d = destination pixel centers mapped to source
 *
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *            d       d       d
 *          s   s   s   s   s   s           s   d   s   s   d   s
 *
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *            d       d       d
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *
 *          s   s   s   s   s   s           s   d   s   s   d   s
 *            d       d       d
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *
 *          Even scaleX/Y factors            Odd scaleX/Y factors
 *
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *              d           d
 *          s   s   s   s   s   s           s d s   s d s   s d s
 *
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *              d           d
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *
 *          s   s   s   s   s   s           s d s   s d s   s d s
 *              d           d
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *
 *         Odd/even scaleX/Y factors      Even/odd scaleX/Y factors
 *
 *      The applied filter is quadrant symmetric (typically antialias +
 *      resample).  The filter is product-separable, quadrant symmetric,
 *      and is defined by half of its span.  Parity is used to signify
 *      whether the symmetric kernel has a double center (even parity) or
 *      a single center value (odd parity).  For example, if hParity == 0
 *      (even), the horizontal kernel is defined as:
 *
 *      hKernel[hSize-1], ..., hKernel[0], hKernel[0], ..., hKernel[hSize-1]
 *
 *      Otherwise, if hParity == 1 (odd), the horizontal kernel is defined
 *      as:
 *
 *          hKernel[hSize-1], ..., hKernel[0], ..., hKernel[hSize-1]
 *
 *      Horizontal and vertical kernels representing convolved resample
 *      (i.e., the combined separable kernels) can be computed from a
 *      convolution filter (with odd parity), a resample filter, and because
 *      the subsample factors affect resample weights, the subsample scale
 *      factors.  It is the user's responsibility to provide meaningful
 *      combined kernels.
 *
 *      To compute the value of a pixel centered at point (xD, yD) in the
 *      destination image, apply the combined kernel to the source image by
 *      aligning the kernel's geometric center to the backward mapped point
 *      (xS, yS) in the source image.  When this is done in a separable
 *      manner, the centers of horizontal and vertical kernels should match
 *      the xS and yS values, respectively.
 *
 *      The combination of subsampling and filtering has performance
 *      benefits over sequential operator usage in part due to the symmetry
 *      constraints imposed by only allowing integer parameters for scaling
 *      and only allowing separable symmetric filters.
 *
 * RESTRICTION
 *
 *      The src and the dst must be the same type and have same number
 *      of channels (1, 2, 3, or 4).
 *      The images can be of type MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or
 *      MLIB_INT.
 *
 *      The image pixels are assumed to be centered at .5 coordinate points,
 *      and the upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *      scaleX > 0
 *      scaleY > 0
 *
 *      The edge condition can be one of the following:
 *              MLIB_EDGE_DST_NO_WRITE  (default)
 */

#include <stdlib.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#ifdef  __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

#define	IMG_TYPE	1

#define	KBUFF_SIZE	64

/* *********************************************************** */

#define	DSCALE	(1u << (31 - iscale))

#define	STYPE	mlib_u8

#define	FS_FUNC(SUFF)	mlib_s_ImageFilteredSubsample_8nw_##SUFF

/* *********************************************************** */

#define	PARAMS                                                            \
	__m128i *buff, STYPE * sp0, STYPE * sp1, __m128i * pkk,           \
	    mlib_s32 xsize, mlib_s32 off, mlib_s32 step, mlib_s32 nchan

/* *********************************************************** */

#define	PARAMS1	PARAMS, mlib_s32 iscale, STYPE *dp

/* *********************************************************** */

#define	sse2_fmul8x16hi(x, y)                                                \
	_mm_mulhi_epi16(_mm_slli_epi16(_mm_unpacklo_epi8(x, sse2_zero), 7), y)

/* *********************************************************** */

#define	LD_1x8_U8(s0, sp0)                                        \
	t0 = _mm_cvtsi32_si128(sp0[0] | (sp0[step] << 8) |        \
			(sp0[step2] << 16) | (sp0[step3] << 24)); \
	t1 = _mm_cvtsi32_si128(sp0[step4] | (sp0[step5] << 8) |   \
			(sp0[step6] << 16) | (sp0[step7] << 24)); \
	sp0 += 8*step;                                            \
	t2 = _mm_unpacklo_epi32(t0, t1);                          \
	t3 = _mm_unpacklo_epi8(t2, sse2_zero);                    \
	s0 = _mm_slli_epi16(t3, 7)

/* xHxGxFxExDxCxBxA -> 0H0G0F0E0D0C0B0A */
#define	LD_1x8_U8_ST2(s0, sp0)                                    \
	t0 = _mm_loadu_si128((__m128i *)sp0);                     \
	sp0 += 16;                                                \
	s0 = _mm_srli_epi16(_mm_slli_epi16(t0, 8), 1)

/* *********************************************************** */

#define	LD_1x4_U16(s0, sp0)                                       \
	t0 = _mm_cvtsi32_si128(*(mlib_u16 *)sp0 |                 \
			((*(mlib_u16 *)(sp0 + step)) << 16));     \
	t1 = _mm_cvtsi32_si128(*(mlib_u16 *)(sp0 + step2) |       \
			((*(mlib_u16 *)(sp0 + step3)) << 16));    \
	sp0 += 4*step;                                            \
	t2 = _mm_unpacklo_epi32(t0, t1);                          \
	t3 = _mm_unpacklo_epi8(t2, sse2_zero);                    \
	s0 = _mm_slli_epi16(t3, 7)

/* xxHGxxFExxDCxxBA -> 0H0G0F0E0D0C0B0A */
#define	LD_1x4_U16_ST4(s0, sp0)                                   \
	t0 = _mm_loadu_si128((__m128i *)sp0);                     \
	sp0 += 16;                                                \
	t1 = _mm_and_si128(t0, epi32_00f0);                       \
	t2 = _mm_and_si128(t0, epi32_000f);                       \
	t3 = _mm_slli_epi32(t1, 8);                               \
	s0 = _mm_or_si128(t2, t3);                                \
	s0 = _mm_slli_epi16(s0, 7)

/* *********************************************************** */

#define	LD_1x2_U32(s0, sp0)                                       \
	t0 = _mm_cvtsi32_si128(*(mlib_u32 *)sp0);                 \
	sp0 += step;                                              \
	t1 = _mm_cvtsi32_si128(*(mlib_u32 *)sp0);                 \
	sp0 += step;                                              \
	t2 = _mm_unpacklo_epi32(t0, t1);                          \
	t3 = _mm_unpacklo_epi8(t2, sse2_zero);                    \
	s0 = _mm_slli_epi16(t3, 7)

#define	LD_1x2_U32_ST6(s0, sp0)                                   \
	t0 = _mm_cvtsi32_si128(*(mlib_u32 *)sp0);                 \
	sp0 += 6;                                                 \
	t1 = _mm_cvtsi32_si128(*(mlib_u32 *)sp0);                 \
	sp0 += 6;                                                 \
	t2 = _mm_unpacklo_epi32(t0, t1);                          \
	t3 = _mm_unpacklo_epi8(t2, sse2_zero);                    \
	s0 = _mm_slli_epi16(t3, 7)

/* xxxxHGFExxxxDCBA -> 0H0G0F0E0D0C0B0A */
#define	LD_1x2_U32_ST8(s0, sp0)                                   \
	t0 = _mm_cvtsi32_si128(*(mlib_u32 *)sp0);                 \
	sp0 += 8;                                                 \
	t1 = _mm_cvtsi32_si128(*(mlib_u32 *)sp0);                 \
	sp0 += 8;                                                 \
	t2 = _mm_unpacklo_epi32(t0, t1);                          \
	t3 = _mm_unpacklo_epi8(t2, sse2_zero);                    \
	s0 = _mm_slli_epi16(t3, 7)

/* *********************************************************** */

#define	MA_4x8_S16(dd, s0, s1, s2, s3, kk)                        \
	dd = _mm_add_epi16(dd, _mm_mulhi_epi16(s0, kk));          \
	dd = _mm_add_epi16(dd, _mm_mulhi_epi16(s1, kk));          \
	dd = _mm_add_epi16(dd, _mm_mulhi_epi16(s2, kk));          \
	dd = _mm_add_epi16(dd, _mm_mulhi_epi16(s3, kk))

/* *********************************************************** */

#define	ST_1x8_U8(dp, dd)                                         \
	dd = _mm_packus_epi16(_mm_srai_epi16(dd, sh), sse2_zero); \
	_mm_storel_epi64((__m128i *)dp, dd);                      \
	dp += 8

/* *********************************************************** */

#define	ST_1x8_U8_EDGE(dp, dd, N)                                 \
	dd = _mm_packus_epi16(_mm_srai_epi16(dd, sh), sse2_zero); \
	di = _mm_cvtsi128_si32(dd);                               \
	if ((N) > 3) {                                            \
		*(mlib_s32 *)dp = di;                             \
		if ((N) > 4) {                                    \
			dd = _mm_srli_si128(dd, 4);               \
			di = _mm_cvtsi128_si32(dd);               \
			dp[4] = di;                               \
		}                                                 \
		if ((N) > 5)                                      \
			dp[5] = di >> 8;                          \
		if ((N) > 6)                                      \
			dp[6] = di >> 16;                         \
	} else {                                                  \
		dp[0] = di;                                       \
		if ((N) > 1)                                      \
			dp[1] = di >> 8;                          \
		if ((N) > 2)                                      \
			dp[2] = di >> 16;                         \
	}

/* *********************************************************** */

static void FS_FUNC(
    2x2i_1) (
    PARAMS)
{
	__m128i s0, s1, s2, s3, dd;
	__m128i t0, t1, t2, t3;
	__m128i kk = pkk[0];
	__m128i sse2_zero = _mm_setzero_si128();
	mlib_s32 i, step2, step3, step4, step5, step6, step7;
	mlib_u8 *sp0a = sp0 + off;
	mlib_u8 *sp1a = sp1 + off;

	step2 = step + step;
	step3 = step + step2;
	step4 = step + step3;
	step5 = step + step4;
	step6 = step + step5;
	step7 = step + step6;

	switch (step) {
	case 2:
#pragma pipeloop(0)
		for (i = 0; i < (xsize + 7) / 8; i++) {
			dd = _mm_load_si128(&buff[i]);
			LD_1x8_U8_ST2(s0, sp0);
			LD_1x8_U8_ST2(s1, sp0a);
			LD_1x8_U8_ST2(s2, sp1);
			LD_1x8_U8_ST2(s3, sp1a);
			MA_4x8_S16(dd, s0, s1, s2, s3, kk);
			_mm_store_si128(&buff[i], dd);
		}
		break;
	default:
#pragma pipeloop(0)
		for (i = 0; i < (xsize + 7) / 8; i++) {
			dd = _mm_load_si128(&buff[i]);
			LD_1x8_U8(s0, sp0);
			LD_1x8_U8(s1, sp0a);
			LD_1x8_U8(s2, sp1);
			LD_1x8_U8(s3, sp1a);
			MA_4x8_S16(dd, s0, s1, s2, s3, kk);
			_mm_store_si128(&buff[i], dd);
		}
		break;
	}
}

/* *********************************************************** */

static void FS_FUNC(
    2x2l_1) (
    PARAMS1)
{
	__m128i s0, s1, s2, s3, dd;
	__m128i t0, t1, t2, t3;
	__m128i val0 = pkk[4], kk = pkk[0];
	__m128i sse2_zero = _mm_setzero_si128();
	mlib_s32 i, step2, step3, step4, step5, step6, step7;
	mlib_s32 di, sh = 6 - iscale;
	mlib_u8 *sp0a = sp0 + off;
	mlib_u8 *sp1a = sp1 + off;

	step2 = step + step;
	step3 = step + step2;
	step4 = step + step3;
	step5 = step + step4;
	step6 = step + step5;
	step7 = step + step6;

	switch (step) {
	case 2:
#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			dd = _mm_load_si128(&buff[i]);
			LD_1x8_U8_ST2(s0, sp0);
			LD_1x8_U8_ST2(s1, sp0a);
			LD_1x8_U8_ST2(s2, sp1);
			LD_1x8_U8_ST2(s3, sp1a);
			_mm_store_si128(&buff[i], val0);
			MA_4x8_S16(dd, s0, s1, s2, s3, kk);
			ST_1x8_U8(dp, dd);
		}

		if (xsize & 7) {
			dd = _mm_load_si128(&buff[i]);
			LD_1x8_U8_ST2(s0, sp0);
			LD_1x8_U8_ST2(s1, sp0a);
			LD_1x8_U8_ST2(s2, sp1);
			LD_1x8_U8_ST2(s3, sp1a);
			_mm_store_si128(&buff[i], val0);
			MA_4x8_S16(dd, s0, s1, s2, s3, kk);
			ST_1x8_U8_EDGE(dp, dd, xsize & 7);
		}
		break;
	default:
#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			dd = _mm_load_si128(&buff[i]);
			LD_1x8_U8(s0, sp0);
			LD_1x8_U8(s1, sp0a);
			LD_1x8_U8(s2, sp1);
			LD_1x8_U8(s3, sp1a);
			_mm_store_si128(&buff[i], val0);
			MA_4x8_S16(dd, s0, s1, s2, s3, kk);
			ST_1x8_U8(dp, dd);
		}

		if (xsize & 7) {
			dd = _mm_load_si128(&buff[i]);
			LD_1x8_U8(s0, sp0);
			LD_1x8_U8(s1, sp0a);
			LD_1x8_U8(s2, sp1);
			LD_1x8_U8(s3, sp1a);
			_mm_store_si128(&buff[i], val0);
			MA_4x8_S16(dd, s0, s1, s2, s3, kk);
			ST_1x8_U8_EDGE(dp, dd, xsize & 7);
		}
		break;
	}
}

/* *********************************************************** */

static void FS_FUNC(
    1x1l_1) (
    PARAMS1)
{
	__m128i s0, dd;
	__m128i t0, t1, t2, t3;
	__m128i val0 = pkk[4], kk = pkk[0];
	__m128i sse2_zero = _mm_setzero_si128();
	mlib_s32 i, step2, step3, step4, step5, step6, step7;
	mlib_s32 di, sh = 6 - iscale;

	step2 = step + step;
	step3 = step + step2;
	step4 = step + step3;
	step5 = step + step4;
	step6 = step + step5;
	step7 = step + step6;

#pragma pipeloop(0)
	for (i = 0; i < xsize / 8; i++) {
		dd = _mm_load_si128(&buff[i]);
		LD_1x8_U8(s0, sp0);
		_mm_store_si128(&buff[i], val0);
		dd = _mm_add_epi16(dd, _mm_mulhi_epi16(s0, kk));
		ST_1x8_U8(dp, dd);
	}

	if (xsize & 7) {
		dd = _mm_load_si128(&buff[i]);
		LD_1x8_U8(s0, sp0);
		_mm_store_si128(&buff[i], val0);
		dd = _mm_add_epi16(dd, _mm_mulhi_epi16(s0, kk));
		ST_1x8_U8_EDGE(dp, dd, xsize & 7);
	}
}

/* *********************************************************** */

static void FS_FUNC(
    2x2i_2) (
    PARAMS)
{
	__m128i s0, s1, s2, s3, dd;
	__m128i t0, t1, t2, t3;
	__m128i kk = pkk[0];
	__m128i sse2_zero = _mm_setzero_si128();
	__m128i epi32_000f = _mm_set1_epi32(0x000000ff);
	__m128i epi32_00f0 = _mm_slli_epi32(epi32_000f, 8);
	mlib_s32 i, step2, step3;
	mlib_u8 *sp0a = sp0 + off;
	mlib_u8 *sp1a = sp1 + off;

	step2 = step + step;
	step3 = step + step2;

	switch (step) {
	case 4:
#pragma pipeloop(0)
		for (i = 0; i < (xsize + 3) / 4; i++) {
			dd = _mm_load_si128(&buff[i]);
			LD_1x4_U16_ST4(s0, sp0);
			LD_1x4_U16_ST4(s1, sp0a);
			LD_1x4_U16_ST4(s2, sp1);
			LD_1x4_U16_ST4(s3, sp1a);
			MA_4x8_S16(dd, s0, s1, s2, s3, kk);
			_mm_store_si128(&buff[i], dd);
		}
		break;
	default:
#pragma pipeloop(0)
		for (i = 0; i < (xsize + 3) / 4; i++) {
			dd = _mm_load_si128(&buff[i]);
			LD_1x4_U16(s0, sp0);
			LD_1x4_U16(s1, sp0a);
			LD_1x4_U16(s2, sp1);
			LD_1x4_U16(s3, sp1a);
			MA_4x8_S16(dd, s0, s1, s2, s3, kk);
			_mm_store_si128(&buff[i], dd);
		}
		break;
	}
}

/* *********************************************************** */

static void FS_FUNC(
    2x2l_2) (
    PARAMS1)
{
	__m128i s0, s1, s2, s3, dd;
	__m128i t0, t1, t2, t3;
	__m128i val0 = pkk[4], kk = pkk[0];
	__m128i sse2_zero = _mm_setzero_si128();
	__m128i epi32_000f = _mm_set1_epi32(0x000000ff);
	__m128i epi32_00f0 = _mm_slli_epi32(epi32_000f, 8);
	mlib_s32 i, step2, step3;
	mlib_s32 di, sh = 6 - iscale;
	mlib_u8 *sp0a = sp0 + off;
	mlib_u8 *sp1a = sp1 + off;

	step2 = step + step;
	step3 = step + step2;

	switch (step) {
	case 4:
#pragma pipeloop(0)
		for (i = 0; i < xsize / 4; i++) {
			dd = _mm_load_si128(&buff[i]);
			LD_1x4_U16_ST4(s0, sp0);
			LD_1x4_U16_ST4(s1, sp0a);
			LD_1x4_U16_ST4(s2, sp1);
			LD_1x4_U16_ST4(s3, sp1a);
			_mm_store_si128(&buff[i], val0);
			MA_4x8_S16(dd, s0, s1, s2, s3, kk);
			ST_1x8_U8(dp, dd);
		}

		if (xsize & 3) {
			dd = _mm_load_si128(&buff[i]);
			LD_1x4_U16_ST4(s0, sp0);
			LD_1x4_U16_ST4(s1, sp0a);
			LD_1x4_U16_ST4(s2, sp1);
			LD_1x4_U16_ST4(s3, sp1a);
			_mm_store_si128(&buff[i], val0);
			MA_4x8_S16(dd, s0, s1, s2, s3, kk);
			dd = _mm_packus_epi16(_mm_srai_epi16(dd, sh),
						sse2_zero);
			di = _mm_cvtsi128_si32(dd);
			if ((xsize & 3) > 1) {
				*(mlib_s32 *)dp = di;
				dp += 4;
				if ((xsize & 3) > 2) {
					dd = _mm_srli_si128(dd, 4);
					*(mlib_s16 *)dp =
						_mm_cvtsi128_si32(dd);
				}
			} else {
				*(mlib_s16 *)dp = di;
			}
		}
		break;
	default:
#pragma pipeloop(0)
		for (i = 0; i < xsize / 4; i++) {
			dd = _mm_load_si128(&buff[i]);
			LD_1x4_U16(s0, sp0);
			LD_1x4_U16(s1, sp0a);
			LD_1x4_U16(s2, sp1);
			LD_1x4_U16(s3, sp1a);
			_mm_store_si128(&buff[i], val0);
			MA_4x8_S16(dd, s0, s1, s2, s3, kk);
			ST_1x8_U8(dp, dd);
		}

		if (xsize & 3) {
			dd = _mm_load_si128(&buff[i]);
			LD_1x4_U16(s0, sp0);
			LD_1x4_U16(s1, sp0a);
			LD_1x4_U16(s2, sp1);
			LD_1x4_U16(s3, sp1a);
			_mm_store_si128(&buff[i], val0);
			MA_4x8_S16(dd, s0, s1, s2, s3, kk);
			dd = _mm_packus_epi16(_mm_srai_epi16(dd, sh),
						sse2_zero);
			di = _mm_cvtsi128_si32(dd);
			if ((xsize & 3) > 1) {
				*(mlib_s32 *)dp = di;
				dp += 4;
				if ((xsize & 3) > 2) {
					dd = _mm_srli_si128(dd, 4);
					*(mlib_s16 *)dp =
						_mm_cvtsi128_si32(dd);
				}
			} else {
				*(mlib_s16 *)dp = di;
			}
		}
		break;
	}
}

/* *********************************************************** */

static void FS_FUNC(
    1x1l_2) (
    PARAMS1)
{
	__m128i s0, dd;
	__m128i t0, t1, t2, t3;
	__m128i val0 = pkk[4], kk = pkk[0];
	__m128i sse2_zero = _mm_setzero_si128();
	mlib_s32 i, step2, step3;
	mlib_s32 di, sh = 6 - iscale;

	step2 = step + step;
	step3 = step + step2;

#pragma pipeloop(0)
	for (i = 0; i < xsize / 4; i++) {
		dd = _mm_load_si128(&buff[i]);
		LD_1x4_U16(s0, sp0);
		_mm_store_si128(&buff[i], val0);
		dd = _mm_add_epi16(dd, _mm_mulhi_epi16(s0, kk));
		ST_1x8_U8(dp, dd);
	}

	if (xsize & 3) {
		dd = _mm_load_si128(&buff[i]);
		LD_1x4_U16(s0, sp0);
		_mm_store_si128(&buff[i], val0);
		dd = _mm_add_epi16(dd, _mm_mulhi_epi16(s0, kk));
		dd = _mm_packus_epi16(_mm_srai_epi16(dd, sh), sse2_zero);
		di = _mm_cvtsi128_si32(dd);
		if ((xsize & 3) > 1) {
			*(mlib_s32 *)dp = di;
			dp += 4;
			if ((xsize & 3) > 2) {
				dd = _mm_srli_si128(dd, 4);
				*(mlib_s16 *)dp = _mm_cvtsi128_si32(dd);
			}
		} else {
			*(mlib_s16 *)dp = di;
		}
	}
}

/* *********************************************************** */

static void FS_FUNC(
    2x2i_3) (
    PARAMS)
{
	__m128i s0, s1, s2, s3, dd;
	__m128i t0, t1, t2, t3;
	__m128i kk = pkk[0];
	__m128i sse2_zero = _mm_setzero_si128();
	mlib_s32 i;
	mlib_u8 *sp0a = sp0 + off;
	mlib_u8 *sp1a = sp1 + off;

	switch (step) {
	case 6:
#pragma pipeloop(0)
		for (i = 0; i < (xsize + 1)/2; i++) {
			dd = _mm_load_si128(&buff[i]);
			LD_1x2_U32_ST6(s0, sp0);
			LD_1x2_U32_ST6(s1, sp0a);
			LD_1x2_U32_ST6(s2, sp1);
			LD_1x2_U32_ST6(s3, sp1a);
			MA_4x8_S16(dd, s0, s1, s2, s3, kk);
			_mm_store_si128(&buff[i], dd);
		}
		break;
	default:
#pragma pipeloop(0)
		for (i = 0; i < (xsize + 1)/2; i++) {
			dd = _mm_load_si128(&buff[i]);
			LD_1x2_U32(s0, sp0);
			LD_1x2_U32(s1, sp0a);
			LD_1x2_U32(s2, sp1);
			LD_1x2_U32(s3, sp1a);
			MA_4x8_S16(dd, s0, s1, s2, s3, kk);
			_mm_store_si128(&buff[i], dd);
		}
		break;
	}
}

/* *********************************************************** */

static void FS_FUNC(
    2x2l_3) (
    PARAMS1)
{
	__m128i s0, s1, s2, s3, dd;
	__m128i t0, t1, t2, t3;
	__m128i val0 = pkk[4], kk = pkk[0];
	__m128i sse2_zero = _mm_setzero_si128();
	mlib_s32 sh = 6 - iscale;
	mlib_s32 i, di;
	mlib_u8 *sp0a = sp0 + off;
	mlib_u8 *sp1a = sp1 + off;

	xsize--;

	switch (step) {
	case 6:
#pragma pipeloop(0)
		for (i = 0; i < xsize/2; i++) {
			dd = _mm_load_si128(&buff[i]);
			LD_1x2_U32_ST6(s0, sp0);
			LD_1x2_U32_ST6(s1, sp0a);
			LD_1x2_U32_ST6(s2, sp1);
			LD_1x2_U32_ST6(s3, sp1a);
			_mm_store_si128(&buff[i], val0);
			MA_4x8_S16(dd, s0, s1, s2, s3, kk);
			dd = _mm_packus_epi16(_mm_srai_epi16(dd, sh),
						sse2_zero);
			*(mlib_s32 *)dp = _mm_cvtsi128_si32(dd);
			dd = _mm_srli_si128(dd, 4);
			dp += nchan;
			*(mlib_s32 *)dp = _mm_cvtsi128_si32(dd);
			dp += nchan;
		}
		break;
	default:
#pragma pipeloop(0)
		for (i = 0; i < xsize/2; i++) {
			dd = _mm_load_si128(&buff[i]);
			LD_1x2_U32(s0, sp0);
			LD_1x2_U32(s1, sp0a);
			LD_1x2_U32(s2, sp1);
			LD_1x2_U32(s3, sp1a);
			_mm_store_si128(&buff[i], val0);
			MA_4x8_S16(dd, s0, s1, s2, s3, kk);
			dd = _mm_packus_epi16(_mm_srai_epi16(dd, sh),
						sse2_zero);
			*(mlib_s32 *)dp = _mm_cvtsi128_si32(dd);
			dd = _mm_srli_si128(dd, 4);
			dp += nchan;
			*(mlib_s32 *)dp = _mm_cvtsi128_si32(dd);
			dp += nchan;
		}
		break;
	}

	if (xsize & 1) {
		dd = _mm_load_si128(&buff[i]);
		LD_1x2_U32(s0, sp0);
		LD_1x2_U32(s1, sp0a);
		LD_1x2_U32(s2, sp1);
		LD_1x2_U32(s3, sp1a);
		_mm_store_si128(&buff[i], val0);
		MA_4x8_S16(dd, s0, s1, s2, s3, kk);
		dd = _mm_packus_epi16(_mm_srai_epi16(dd, sh), sse2_zero);
		*(mlib_s32 *)dp = _mm_cvtsi128_si32(dd);
		dp += nchan;
		dd = _mm_srli_si128(dd, 4);
		di = _mm_cvtsi128_si32(dd);
		dp[0] = di;
		dp[1] = di >> 8;
		dp[2] = di >> 16;
	} else {
		dd = _mm_load_si128(&buff[i]);
		s0 = _mm_cvtsi32_si128(*(mlib_s32 *)sp0);
		s1 = _mm_cvtsi32_si128(*(mlib_s32 *)sp0a);
		s2 = _mm_cvtsi32_si128(*(mlib_s32 *)sp1);
		s3 = _mm_cvtsi32_si128(*(mlib_s32 *)sp1a);
		_mm_store_si128(&buff[i], val0);
		dd = _mm_add_epi16(dd, sse2_fmul8x16hi(s0, kk));
		dd = _mm_add_epi16(dd, sse2_fmul8x16hi(s1, kk));
		dd = _mm_add_epi16(dd, sse2_fmul8x16hi(s2, kk));
		dd = _mm_add_epi16(dd, sse2_fmul8x16hi(s3, kk));
		dd = _mm_packus_epi16(_mm_srai_epi16(dd, sh), sse2_zero);
		di = _mm_cvtsi128_si32(dd);
		dp[0] = di;
		dp[1] = di >> 8;
		dp[2] = di >> 16;
	}
}

/* *********************************************************** */

static void FS_FUNC(
    1x1l_3) (
    PARAMS1)
{
	__m128i s0, dd;
	__m128i t0, t1, t2, t3;
	__m128i val0 = pkk[4], kk = pkk[0];
	__m128i sse2_zero = _mm_setzero_si128();
	mlib_s32 sh = 6 - iscale;
	mlib_s32 i, di;

	xsize--;

#pragma pipeloop(0)
	for (i = 0; i < xsize/2; i++) {
		dd = _mm_load_si128(&buff[i]);
		LD_1x2_U32(s0, sp0);
		_mm_store_si128(&buff[i], val0);
		dd = _mm_add_epi16(dd, _mm_mulhi_epi16(s0, kk));
		dd = _mm_packus_epi16(_mm_srai_epi16(dd, sh), sse2_zero);
		*(mlib_s32 *)dp = _mm_cvtsi128_si32(dd);
		dp += nchan;
		dd = _mm_srli_si128(dd, 4);
		*(mlib_s32 *)dp = _mm_cvtsi128_si32(dd);
		dp += nchan;
	}

	if (xsize & 1) {
		dd = _mm_load_si128(&buff[i]);
		LD_1x2_U32(s0, sp0);
		_mm_store_si128(&buff[i], val0);
		dd = _mm_add_epi16(dd, _mm_mulhi_epi16(s0, kk));
		dd = _mm_packus_epi16(_mm_srai_epi16(dd, sh), sse2_zero);
		*(mlib_s32 *)dp = _mm_cvtsi128_si32(dd);
		dp += nchan;
		dd = _mm_srli_si128(dd, 4);
		di = _mm_cvtsi128_si32(dd);
		dp[0] = di;
		dp[1] = di >> 8;
		dp[2] = di >> 16;
	} else {
		dd = _mm_load_si128(&buff[i]);
		s0 = _mm_cvtsi32_si128(*(mlib_s32 *)sp0);
		_mm_store_si128(&buff[i], val0);
		dd = _mm_add_epi16(dd, sse2_fmul8x16hi(s0, kk));
		dd = _mm_packus_epi16(_mm_srai_epi16(dd, sh), sse2_zero);
		di = _mm_cvtsi128_si32(dd);
		dp[0] = di;
		dp[1] = di >> 8;
		dp[2] = di >> 16;
	}
}

/* *********************************************************** */

static void FS_FUNC(
    2x2i_4) (
    PARAMS)
{
	__m128i s0, s1, s2, s3, dd;
	__m128i t0, t1, t2, t3;
	__m128i kk = pkk[0];
	__m128i sse2_zero = _mm_setzero_si128();
	mlib_s32 i;
	mlib_u8 *sp0a = sp0 + off;
	mlib_u8 *sp1a = sp1 + off;

	switch (step) {
	case 8:
#pragma pipeloop(0)
		for (i = 0; i < (xsize + 1)/2; i++) {
			dd = _mm_load_si128(&buff[i]);
			LD_1x2_U32_ST8(s0, sp0);
			LD_1x2_U32_ST8(s1, sp0a);
			LD_1x2_U32_ST8(s2, sp1);
			LD_1x2_U32_ST8(s3, sp1a);
			MA_4x8_S16(dd, s0, s1, s2, s3, kk);
			_mm_store_si128(&buff[i], dd);
		}
		break;
	default:
#pragma pipeloop(0)
		for (i = 0; i < (xsize + 1)/2; i++) {
			dd = _mm_load_si128(&buff[i]);
			LD_1x2_U32(s0, sp0);
			LD_1x2_U32(s1, sp0a);
			LD_1x2_U32(s2, sp1);
			LD_1x2_U32(s3, sp1a);
			MA_4x8_S16(dd, s0, s1, s2, s3, kk);
			_mm_store_si128(&buff[i], dd);
		}
		break;
	}
}

/* *********************************************************** */

static void FS_FUNC(
    2x2l_4) (
    PARAMS1)
{
	__m128i s0, s1, s2, s3, dd;
	__m128i t0, t1, t2, t3;
	__m128i val0 = pkk[4], kk = pkk[0];
	__m128i sse2_zero = _mm_setzero_si128();
	mlib_s32 sh = 6 - iscale;
	mlib_s32 i;
	mlib_u8 *sp0a = sp0 + off;
	mlib_u8 *sp1a = sp1 + off;

	switch (step) {
	case 8:
#pragma pipeloop(0)
		for (i = 0; i < xsize/2; i++) {
			dd = _mm_load_si128(&buff[i]);
			LD_1x2_U32_ST8(s0, sp0);
			LD_1x2_U32_ST8(s1, sp0a);
			LD_1x2_U32_ST8(s2, sp1);
			LD_1x2_U32_ST8(s3, sp1a);
			_mm_store_si128(&buff[i], val0);
			MA_4x8_S16(dd, s0, s1, s2, s3, kk);
			ST_1x8_U8(dp, dd);
		}
		break;
	default:
#pragma pipeloop(0)
		for (i = 0; i < xsize/2; i++) {
			dd = _mm_load_si128(&buff[i]);
			LD_1x2_U32(s0, sp0);
			LD_1x2_U32(s1, sp0a);
			LD_1x2_U32(s2, sp1);
			LD_1x2_U32(s3, sp1a);
			_mm_store_si128(&buff[i], val0);
			MA_4x8_S16(dd, s0, s1, s2, s3, kk);
			ST_1x8_U8(dp, dd);
		}
		break;
	}

	if (xsize & 1) {
		dd = _mm_load_si128(&buff[i]);
		s0 = _mm_cvtsi32_si128(*(mlib_s32 *)sp0);
		s1 = _mm_cvtsi32_si128(*(mlib_s32 *)sp0a);
		s2 = _mm_cvtsi32_si128(*(mlib_s32 *)sp1);
		s3 = _mm_cvtsi32_si128(*(mlib_s32 *)sp1a);
		_mm_store_si128(&buff[i], val0);
		dd = _mm_add_epi16(dd, sse2_fmul8x16hi(s0, kk));
		dd = _mm_add_epi16(dd, sse2_fmul8x16hi(s1, kk));
		dd = _mm_add_epi16(dd, sse2_fmul8x16hi(s2, kk));
		dd = _mm_add_epi16(dd, sse2_fmul8x16hi(s3, kk));
		dd = _mm_srai_epi16(dd, sh);
		dd = _mm_packus_epi16(dd, sse2_zero);
		*(mlib_s32 *)dp = _mm_cvtsi128_si32(dd);
	}
}

/* *********************************************************** */

static void FS_FUNC(
    1x1l_4) (
    PARAMS1)
{
	__m128i s0, dd;
	__m128i t0, t1, t2, t3;
	__m128i val0 = pkk[4], kk = pkk[0];
	__m128i sse2_zero = _mm_setzero_si128();
	mlib_s32 sh = 6 - iscale;
	mlib_s32 i;

#pragma pipeloop(0)
	for (i = 0; i < xsize/2; i++) {
		dd = _mm_load_si128(&buff[i]);
		LD_1x2_U32(s0, sp0);
		_mm_store_si128(&buff[i], val0);
		dd = _mm_add_epi16(dd, _mm_mulhi_epi16(s0, kk));
		ST_1x8_U8(dp, dd);
	}

	if (xsize & 1) {
		dd = _mm_load_si128(&buff[i]);
		s0 = _mm_cvtsi32_si128(*(mlib_s32 *)sp0);
		_mm_store_si128(&buff[i], val0);
		dd = _mm_add_epi16(dd, sse2_fmul8x16hi(s0, kk));
		dd = _mm_packus_epi16(_mm_srai_epi16(dd, sh), sse2_zero);
		*(mlib_s32 *)dp = _mm_cvtsi128_si32(dd);
	}
}

/* *********************************************************** */

mlib_status FS_FUNC(
    1)  (
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 scaleX,
    mlib_s32 scaleY,
    mlib_s32 transX,
    mlib_s32 transY,
    mlib_d64 *hKernel,
    mlib_d64 *vKernel,
    mlib_s32 iscale,
    mlib_s32 hSize,
    mlib_s32 vSize,
    mlib_s32 hParity,
    mlib_s32 vParity,
    mlib_edge edge)
{
	STYPE *psrc, *pdst, *sl, *dl;
	mlib_s32 slb, dlb, swidth, sheight, dwidth, dheight, nchan;
	mlib_type type;
	mlib_s32 kw = 2 * hSize - hParity;
	mlib_s32 kh = 2 * vSize - vParity;
	mlib_s32 step, stepRight, stepDown;
	mlib_s32 rnd_val, rnd_val2;
	__m128i *buff, kk[5], val0;
	__m128i keri_lcl[KBUFF_SIZE], *keri = keri_lcl;
	mlib_s32 x_min, x_max, y_min, y_max, xsize, bsize;
	mlib_s32 i, j, k, ix, iy;

	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, nchan, dwidth, dheight, dlb, pdst);
	MLIB_IMAGE_GET_ALL_PARAMS(src, type, nchan, swidth, sheight, slb, psrc);

	slb /= sizeof (STYPE);
	dlb /= sizeof (STYPE);

	step = scaleX * nchan;
	stepRight = (kw - 1) * nchan;
	stepDown = (kh - 1) * slb;

/* 0.5*scaleX - 0.5*(kw-1) */
	transX += (scaleX - (kw - 1)) >> 1;
/* 0.5*scaleY - 0.5*(kh-1) */
	transY += (scaleY - (kh - 1)) >> 1;

	x_min = scaleX - 1 - transX;
	y_min = scaleY - 1 - transY;
	x_max = swidth - kw - transX;
	y_max = sheight - kh - transY;

	if (x_min >= 0)
		x_min /= scaleX;
	else
		x_min = 0;
	if (y_min >= 0)
		y_min /= scaleY;
	else
		y_min = 0;
	if (x_max >= 0)
		x_max /= scaleX;
	else
		x_max = -1;
	if (y_max >= 0)
		y_max /= scaleY;
	else
		y_max = -1;
	if (x_max >= dwidth)
		x_max = dwidth - 1;
	if (y_max >= dheight)
		y_max = dheight - 1;

	xsize = x_max - x_min + 1;
	if (xsize <= 0)
		return (MLIB_SUCCESS);

	bsize =
	    (nchan == 1) ? ((xsize + 7) / 8) : (nchan ==
	    2) ? ((xsize + 3) / 4) : xsize;
	buff = __mlib_malloc(sizeof (__m128i) * bsize);
	if (buff == NULL)
		return (MLIB_FAILURE);

	if (vSize * hSize > KBUFF_SIZE) {
		keri = __mlib_malloc(vSize * hSize * sizeof (__m128i));
		if (keri == NULL) {
			__mlib_free(buff);
			return (MLIB_FAILURE);
		}
	}

	for (iy = 0; iy < vSize; iy++) {
		mlib_s32 flagy = (iy == 0 && vParity) ? 1 : 0;
		mlib_s32 ki, flagx;
		mlib_d64 kd;

		for (ix = 0; ix < hSize; ix++) {
			flagx = (ix == 0 && hParity) ? 1 : 0;
			kd = hKernel[ix] * vKernel[iy] * DSCALE;
			if (flagx ^ flagy)
				kd *= 0.5;
			ki = (kd > 0.0) ?
				(mlib_s32)(kd + 0.5) : (mlib_s32)(kd - 0.5);
			ki = (ki & 0xFFFF0000) | ((ki >> 16) & 0xFFFF);
			((mlib_s32 *)(keri + iy * hSize + ix))[0] = ki;
			((mlib_s32 *)(keri + iy * hSize + ix))[1] = ki;
			((mlib_s32 *)(keri + iy * hSize + ix))[2] = ki;
			((mlib_s32 *)(keri + iy * hSize + ix))[3] = ki;
		}
	}

	if (iscale > 5) {
		rnd_val = 0;
	} else {
		rnd_val = 1 << (5 - iscale);
	}
	rnd_val2 = rnd_val | (rnd_val << 16);
	val0 = _mm_set1_epi32(rnd_val2);
	kk[4] = val0;
	for (i = 0; i < bsize; i++) {
		buff[i] = val0;
	}

	sl = psrc + (y_min * scaleY + transY) * slb + (x_min * scaleX +
	    transX) * nchan;
	dl = pdst + y_min * dlb + x_min * nchan;

	for (j = y_min; j <= y_max; j++) {
		STYPE *sl0 = sl;
		STYPE *sl1 = sl + stepDown;

		for (iy = vSize - 1; iy >= 0; iy--) {
			mlib_s32 off = stepRight;
			STYPE *sp0 = sl0;
			STYPE *sp1 = sl1;

			for (ix = hSize - 1; ix >= 0; ix--) {
				mlib_s32 flagl = (iy == 0 && ix == 0);

				kk[0] = keri[iy * hSize + ix];

				if (!flagl) {
					if (nchan == 1)
						FS_FUNC(2x2i_1) (buff, sp0,
						    sp1, kk, xsize, off, step,
						    nchan);
					else if (nchan == 2)
						FS_FUNC(2x2i_2) (buff, sp0,
						    sp1, kk, xsize, off, step,
						    nchan);
					else if (nchan == 3)
						FS_FUNC(2x2i_3) (buff, sp0,
						    sp1, kk, xsize, off, step,
						    nchan);
					else
						FS_FUNC(2x2i_4) (buff, sp0,
						    sp1, kk, xsize, off, step,
						    nchan);
				} else if (off == 0 && sp0 == sp1) {
					if (nchan == 1)
						FS_FUNC(1x1l_1) (buff, sp0,
						    sp1, kk, xsize, off, step,
						    nchan, iscale, dl + k);
					else if (nchan == 2)
						FS_FUNC(1x1l_2) (buff, sp0,
						    sp1, kk, xsize, off, step,
						    nchan, iscale, dl + k);
					else if (nchan == 3)
						FS_FUNC(1x1l_3) (buff, sp0,
						    sp1, kk, xsize, off, step,
						    nchan, iscale, dl + k);
					else
						FS_FUNC(1x1l_4) (buff, sp0,
						    sp1, kk, xsize, off, step,
						    nchan, iscale, dl + k);
				} else {
					if (nchan == 1)
						FS_FUNC(2x2l_1) (buff, sp0,
						    sp1, kk, xsize, off, step,
						    nchan, iscale, dl + k);
					else if (nchan == 2)
						FS_FUNC(2x2l_2) (buff, sp0,
						    sp1, kk, xsize, off, step,
						    nchan, iscale, dl + k);
					else if (nchan == 3)
						FS_FUNC(2x2l_3) (buff, sp0,
						    sp1, kk, xsize, off, step,
						    nchan, iscale, dl + k);
					else
						FS_FUNC(2x2l_4) (buff, sp0,
						    sp1, kk, xsize, off, step,
						    nchan, iscale, dl + k);
				}

				sp0 += nchan;
				sp1 += nchan;
				off -= 2 * nchan;
			}

			sl0 += slb;
			sl1 -= slb;
		}

		sl += scaleY * slb;
		dl += dlb;
	}

	if (keri != keri_lcl)
		__mlib_free(keri);
	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
