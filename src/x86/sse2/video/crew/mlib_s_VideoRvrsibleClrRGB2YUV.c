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

#pragma ident	"@(#)mlib_s_VideoRvrsibleClrRGB2YUV.c	9.5	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VideoReversibleColorRGB2YUV_U8_U8
 *      mlib_VideoReversibleColorRGB2YUV_S16_U8
 *      mlib_VideoReversibleColorRGB2YUV_S16_S16
 *      mlib_VideoReversibleColorRGB2YUV_S32_S16
 *                                    - convert from RGB to YUV
 *
 * SYNOPSIS
 *
 *      mlib_status
 *      mlib_VideoReversibleColorRGB2YUV_S16_S16(mlib_s16       *Y,
 *                                               mlib_s16       *U,
 *                                               mlib_s16       *V,
 *                                               const mlib_s16 *R,
 *                                               const mlib_s16 *G,
 *                                               const mlib_s16 *B,
 *                                               mlib_s32       n,
 *                                               mlib_s32       depth);
 *      mlib_status
 *      mlib_VideoReversibleColorRGB2YUV_S16_U8(mlib_s16      *Y,
 *                                              mlib_s16      *U,
 *                                              mlib_s16      *V,
 *                                              const mlib_u8 *R,
 *                                              const mlib_u8 *G,
 *                                              const mlib_u8 *B,
 *                                              mlib_s32      n,
 *                                              mlib_s32      depth);
 *      mlib_status
 *      mlib_VideoReversibleColorRGB2YUV_S32_S16(mlib_s32       *Y,
 *                                               mlib_s32       *U,
 *                                               mlib_s32       *V,
 *                                               const mlib_s16 *R,
 *                                               const mlib_s16 *G,
 *                                               const mlib_s16 *B,
 *                                               mlib_s32       n,
 *                                               mlib_s32       depth);
 *      mlib_status
 *      mlib_VideoReversibleColorRGB2YUV_U8_U8(mlib_u8       *Y,
 *                                             mlib_u8       *U,
 *                                             mlib_u8       *V,
 *                                             const mlib_u8 *R,
 *                                             const mlib_u8 *G,
 *                                             const mlib_u8 *B,
 *                                             mlib_s32      n,
 *                                             mlib_s32      depth);
 * ARGUMENT
 *      Y     pointer to output Y component
 *      U     pointer to output U component
 *      V     pointer to output V component
 *      R     pointer to input R component
 *      G     pointer to input G component
 *      B     pointer to input B component
 *      n     length components
 *      depth number of bit_planes required to store the original
 *            R, G, and B components
 *
 * RESTRICTION
 *  All components must have the same sizes and a aligned to 8 bytes.
 *  Possible variants are following:
 *      MLIB_U8   -> MLIB_U8           depth = 0 .. 7
 *      MLIB_U8   -> MLIB_S16          depth = 0 .. 8
 *      MLIB_S16  -> MLIB_S16          depth = 0 .. 15
 *      MLIB_S16  -> MLIB_S32          depth = 0 .. 16
 *
 * DESCRIPTION
 *
 *      Y = (R + 2 * G + B) / 4;
 *      U = R - G + 2^depth - 1;
 *      V = B - G + 2^depth - 1;
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

#pragma weak mlib_VideoReversibleColorRGB2YUV_S16_S16 = \
	__mlib_VideoReversibleColorRGB2YUV_S16_S16
#pragma weak mlib_VideoReversibleColorRGB2YUV_S16_U8 = \
	__mlib_VideoReversibleColorRGB2YUV_S16_U8
#pragma weak mlib_VideoReversibleColorRGB2YUV_S32_S16 = \
	__mlib_VideoReversibleColorRGB2YUV_S32_S16
#pragma weak mlib_VideoReversibleColorRGB2YUV_U8_U8 = \
	__mlib_VideoReversibleColorRGB2YUV_U8_U8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoReversibleColorRGB2YUV_S16_S16)
	mlib_VideoReversibleColorRGB2YUV_S16_S16
	__attribute__((weak,
	alias("__mlib_VideoReversibleColorRGB2YUV_S16_S16")));
__typeof__(__mlib_VideoReversibleColorRGB2YUV_S16_U8)
	mlib_VideoReversibleColorRGB2YUV_S16_U8
	__attribute__((weak,
	alias("__mlib_VideoReversibleColorRGB2YUV_S16_U8")));
__typeof__(__mlib_VideoReversibleColorRGB2YUV_S32_S16)
	mlib_VideoReversibleColorRGB2YUV_S32_S16
	__attribute__((weak,
	alias("__mlib_VideoReversibleColorRGB2YUV_S32_S16")));
__typeof__(__mlib_VideoReversibleColorRGB2YUV_U8_U8)
	mlib_VideoReversibleColorRGB2YUV_U8_U8
	__attribute__((weak,
	alias("__mlib_VideoReversibleColorRGB2YUV_U8_U8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
mlib_VideoReversibleColorRGB2YUV_U8_U8_naligned(
	mlib_u8 *Y,
	mlib_u8 *U,
	mlib_u8 *V,
	const mlib_u8 *R,
	const mlib_u8 *G,
	const mlib_u8 *B,
	mlib_s32 n,
	mlib_s32 depth)
{
	int i;
	mlib_s16 nShift, nr, ng, nb;
	__m128i *pY, *pU, *pV, *pR, *pG, *pB;
	__m128i r, g, b, y, u, v;
	__m128i rh, rl, gh, gl, bh, bl, yh, yl, uh, ul, vh, vl;
	__m128i shift, temp, temph, templ;
	const __m128i	zero = _mm_setzero_si128();

	nShift = ((1 << depth) - 1);
	shift = _mm_set1_epi16(nShift);
	pY = (__m128i*)Y;
	pU = (__m128i*)U;
	pV = (__m128i*)V;
	pR = (__m128i*)R;
	pG = (__m128i*)G;
	pB = (__m128i*)B;
	i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < (n >>4); i++) {
		r = _mm_loadu_si128(&pR[i]);
		g = _mm_loadu_si128(&pG[i]);
		b = _mm_loadu_si128(&pB[i]);
		rh = _mm_unpackhi_epi8(r, zero);
		rl = _mm_unpacklo_epi8(r, zero);
		gh = _mm_unpackhi_epi8(g, zero);
		gl = _mm_unpacklo_epi8(g, zero);
		bh = _mm_unpackhi_epi8(b, zero);
		bl = _mm_unpacklo_epi8(b, zero);

		temp = _mm_add_epi16(rh, bh);
		temp = _mm_add_epi16(temp, _mm_slli_epi16(gh, 1));
		yh = _mm_srai_epi16(temp, 2);
		temp = _mm_add_epi16(rl, bl);
		temp = _mm_add_epi16(temp, _mm_slli_epi16(gl, 1));
		yl = _mm_srai_epi16(temp, 2);
		y = _mm_packus_epi16(yl, yh);
		_mm_storeu_si128(&pY[i], y);

		temph = _mm_sub_epi16(shift, gh);
		templ = _mm_sub_epi16(shift, gl);

		uh = _mm_add_epi16(temph, rh);
		ul = _mm_add_epi16(templ, rl);
		u = _mm_packus_epi16(ul, uh);
		_mm_storeu_si128(&pU[i], u);

		vh = _mm_add_epi16(temph, bh);
		vl = _mm_add_epi16(templ, bl);
		v = _mm_packus_epi16(vl, vh);
		_mm_storeu_si128(&pV[i], v);
	}

	for (i <<= 4; i < n; i++) {
		nr = R[i];
		ng = G[i];
		nb = B[i];

		Y[i] = (nr + 2 * ng + nb) >> 2;
		U[i] = nr - ng + nShift;
		V[i] = nb - ng + nShift;
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoReversibleColorRGB2YUV_U8_U8_aligned(
	mlib_u8 *Y,
	mlib_u8 *U,
	mlib_u8 *V,
	const mlib_u8 *R,
	const mlib_u8 *G,
	const mlib_u8 *B,
	mlib_s32 n,
	mlib_s32 depth)
{
	int i;
	mlib_s16 nShift, nr, ng, nb;
	__m128i *pY, *pU, *pV, *pR, *pG, *pB;
	__m128i r, g, b, y, u, v;
	__m128i rh, rl, gh, gl, bh, bl, yh, yl, uh, ul, vh, vl;
	__m128i shift, temp, temph, templ;
	const __m128i	zero = _mm_setzero_si128();

	nShift = ((1 << depth) - 1);
	shift = _mm_set1_epi16(nShift);
	pY = (__m128i*)Y;
	pU = (__m128i*)U;
	pV = (__m128i*)V;
	pR = (__m128i*)R;
	pG = (__m128i*)G;
	pB = (__m128i*)B;
	i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < (n >>4); i++) {
		r = _mm_load_si128(&pR[i]);
		g = _mm_load_si128(&pG[i]);
		b = _mm_load_si128(&pB[i]);
		rh = _mm_unpackhi_epi8(r, zero);
		rl = _mm_unpacklo_epi8(r, zero);
		gh = _mm_unpackhi_epi8(g, zero);
		gl = _mm_unpacklo_epi8(g, zero);
		bh = _mm_unpackhi_epi8(b, zero);
		bl = _mm_unpacklo_epi8(b, zero);

		temp = _mm_add_epi16(rh, bh);
		temp = _mm_add_epi16(temp, _mm_slli_epi16(gh, 1));
		yh = _mm_srai_epi16(temp, 2);
		temp = _mm_add_epi16(rl, bl);
		temp = _mm_add_epi16(temp, _mm_slli_epi16(gl, 1));
		yl = _mm_srai_epi16(temp, 2);
		y = _mm_packus_epi16(yl, yh);
		_mm_store_si128(&pY[i], y);

		temph = _mm_sub_epi16(shift, gh);
		templ = _mm_sub_epi16(shift, gl);

		uh = _mm_add_epi16(temph, rh);
		ul = _mm_add_epi16(templ, rl);
		u = _mm_packus_epi16(ul, uh);
		_mm_store_si128(&pU[i], u);

		vh = _mm_add_epi16(temph, bh);
		vl = _mm_add_epi16(templ, bl);
		v = _mm_packus_epi16(vl, vh);
		_mm_store_si128(&pV[i], v);
	}

	for (i <<= 4; i < n; i++) {
		nr = R[i];
		ng = G[i];
		nb = B[i];

		Y[i] = (nr + 2 * ng + nb) >> 2;
		U[i] = nr - ng + nShift;
		V[i] = nb - ng + nShift;
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_VideoReversibleColorRGB2YUV_U8_U8(
	mlib_u8 *Y,
	mlib_u8 *U,
	mlib_u8 *V,
	const mlib_u8 *R,
	const mlib_u8 *G,
	const mlib_u8 *B,
	mlib_s32 n,
	mlib_s32 depth)
{
	if (0 == (0xf & ((mlib_addr)R | (mlib_addr)G |
		(mlib_addr)B | (mlib_addr)Y |
		(mlib_addr)U | (mlib_addr)V))) {
		return mlib_VideoReversibleColorRGB2YUV_U8_U8_aligned(
				Y, U, V, R, G, B, n, depth);
	} else {
		return mlib_VideoReversibleColorRGB2YUV_U8_U8_naligned(
				Y, U, V, R, G, B, n, depth);
	}
}

/* *********************************************************** */

mlib_status
mlib_VideoReversibleColorRGB2YUV_S16_U8_naligned(
	mlib_s16 *Y,
	mlib_s16 *U,
	mlib_s16 *V,
	const mlib_u8 *R,
	const mlib_u8 *G,
	const mlib_u8 *B,
	mlib_s32 n,
	mlib_s32 depth)
{
	int i;
	mlib_s16 nShift, nr, ng, nb;
	__m128i *pY, *pU, *pV, *pR, *pG, *pB;
	__m128i r, g, b;
	__m128i rh, rl, gh, gl, bh, bl, yh, yl, uh, ul, vh, vl;
	__m128i shift, temph, templ;
	const __m128i zero = _mm_setzero_si128();

	nShift = ((1 << depth) - 1);
	shift = _mm_set1_epi16(nShift);
	pY = (__m128i*)Y;
	pU = (__m128i*)U;
	pV = (__m128i*)V;
	pR = (__m128i*)R;
	pG = (__m128i*)G;
	pB = (__m128i*)B;
	i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < (n >>4); i++) {
		r = _mm_loadu_si128(&pR[i]);
		g = _mm_loadu_si128(&pG[i]);
		b = _mm_loadu_si128(&pB[i]);
		rh = _mm_unpackhi_epi8(r, zero);
		rl = _mm_unpacklo_epi8(r, zero);
		gh = _mm_unpackhi_epi8(g, zero);
		gl = _mm_unpacklo_epi8(g, zero);
		bh = _mm_unpackhi_epi8(b, zero);
		bl = _mm_unpacklo_epi8(b, zero);

		templ = _mm_add_epi16(rl, bl);
		templ = _mm_add_epi16(templ, _mm_slli_epi16(gl, 1));
		yl = _mm_srai_epi16(templ, 2);
		temph = _mm_add_epi16(rh, bh);
		temph = _mm_add_epi16(temph, _mm_slli_epi16(gh, 1));
		yh = _mm_srai_epi16(temph, 2);
		_mm_storeu_si128(&pY[i << 1], yl);
		_mm_storeu_si128(&pY[(i << 1) + 1], yh);

		temph = _mm_sub_epi16(shift, gh);
		templ = _mm_sub_epi16(shift, gl);

		ul = _mm_add_epi16(templ, rl);
		_mm_storeu_si128(&pU[i << 1], ul);
		uh = _mm_add_epi16(temph, rh);
		_mm_storeu_si128(&pU[(i << 1) + 1], uh);

		vl = _mm_add_epi16(templ, bl);
		_mm_storeu_si128(&pV[i << 1], vl);
		vh = _mm_add_epi16(temph, bh);
		_mm_storeu_si128(&pV[(i << 1) + 1], vh);
	}

	for (i <<= 4; i < n; i++) {
		nr = R[i];
		ng = G[i];
		nb = B[i];

		Y[i] = (nr + 2 * ng + nb) >> 2;
		U[i] = nr - ng + nShift;
		V[i] = nb - ng + nShift;
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoReversibleColorRGB2YUV_S16_U8_aligned(
	mlib_s16 *Y,
	mlib_s16 *U,
	mlib_s16 *V,
	const mlib_u8 *R,
	const mlib_u8 *G,
	const mlib_u8 *B,
	mlib_s32 n,
	mlib_s32 depth)
{
	int i;
	mlib_s16 nShift, nr, ng, nb;
	__m128i *pY, *pU, *pV, *pR, *pG, *pB;
	__m128i r, g, b;
	__m128i rh, rl, gh, gl, bh, bl, yh, yl, uh, ul, vh, vl;
	__m128i shift, temph, templ;
	const __m128i zero = _mm_setzero_si128();

	nShift = ((1 << depth) - 1);
	shift = _mm_set1_epi16(nShift);
	pY = (__m128i*)Y;
	pU = (__m128i*)U;
	pV = (__m128i*)V;
	pR = (__m128i*)R;
	pG = (__m128i*)G;
	pB = (__m128i*)B;
	i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < (n >>4); i++) {
		r = _mm_load_si128(&pR[i]);
		g = _mm_load_si128(&pG[i]);
		b = _mm_load_si128(&pB[i]);
		rh = _mm_unpackhi_epi8(r, zero);
		rl = _mm_unpacklo_epi8(r, zero);
		gh = _mm_unpackhi_epi8(g, zero);
		gl = _mm_unpacklo_epi8(g, zero);
		bh = _mm_unpackhi_epi8(b, zero);
		bl = _mm_unpacklo_epi8(b, zero);

		templ = _mm_add_epi16(rl, bl);
		templ = _mm_add_epi16(templ, _mm_slli_epi16(gl, 1));
		yl = _mm_srai_epi16(templ, 2);
		temph = _mm_add_epi16(rh, bh);
		temph = _mm_add_epi16(temph, _mm_slli_epi16(gh, 1));
		yh = _mm_srai_epi16(temph, 2);
		_mm_store_si128(&pY[i << 1], yl);
		_mm_store_si128(&pY[(i << 1) + 1], yh);

		temph = _mm_sub_epi16(shift, gh);
		templ = _mm_sub_epi16(shift, gl);

		ul = _mm_add_epi16(templ, rl);
		_mm_store_si128(&pU[i << 1], ul);
		uh = _mm_add_epi16(temph, rh);
		_mm_store_si128(&pU[(i << 1) + 1], uh);

		vl = _mm_add_epi16(templ, bl);
		_mm_store_si128(&pV[i << 1], vl);
		vh = _mm_add_epi16(temph, bh);
		_mm_store_si128(&pV[(i << 1) + 1], vh);
	}

	for (i <<= 4; i < n; i++) {
		nr = R[i];
		ng = G[i];
		nb = B[i];

		Y[i] = (nr + 2 * ng + nb) >> 2;
		U[i] = nr - ng + nShift;
		V[i] = nb - ng + nShift;
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_VideoReversibleColorRGB2YUV_S16_U8(
	mlib_s16 *Y,
	mlib_s16 *U,
	mlib_s16 *V,
	const mlib_u8 *R,
	const mlib_u8 *G,
	const mlib_u8 *B,
	mlib_s32 n,
	mlib_s32 depth)
{
	if (0 == (0xf & ((mlib_addr)R | (mlib_addr)G |
		(mlib_addr)B | (mlib_addr)Y |
		(mlib_addr)U | (mlib_addr)V))) {
		return mlib_VideoReversibleColorRGB2YUV_S16_U8_aligned(
			Y, U, V, R, G, B, n, depth);
	} else {
		return mlib_VideoReversibleColorRGB2YUV_S16_U8_naligned(
			Y, U, V, R, G, B, n, depth);
	}
}

/* *********************************************************** */

mlib_status
mlib_VideoReversibleColorRGB2YUV_S16_S16_naligned(
	mlib_s16 *Y,
	mlib_s16 *U,
	mlib_s16 *V,
	const mlib_s16 *R,
	const mlib_s16 *G,
	const mlib_s16 *B,
	mlib_s32 n,
	mlib_s32 depth)
{
	int i;
	mlib_s16 nShift;
	__m128i *pY, *pU, *pV, *pR, *pG, *pB;
	mlib_s16 nr, ng, nb;
	__m128i y, u, v, g, r, b;
	__m128i shift, temp;

	nShift = ((1 << depth) - 1);
	shift = _mm_set1_epi16(nShift);
	pY = (__m128i*)Y;
	pU = (__m128i*)U;
	pV = (__m128i*)V;
	pR = (__m128i*)R;
	pG = (__m128i*)G;
	pB = (__m128i*)B;
	i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < (n >> 3); i++) {
		r = _mm_loadu_si128(&pR[i]);
		g = _mm_loadu_si128(&pG[i]);
		b = _mm_loadu_si128(&pB[i]);

		temp = _mm_slli_epi16(g, 1);
		temp = _mm_add_epi16(r, temp);
		temp = _mm_add_epi16(temp, b);
		y = _mm_srai_epi16(temp, 2);
		_mm_storeu_si128(&pY[i], y);

		temp = _mm_sub_epi16(shift, g);

		u = _mm_add_epi16(temp, r);
		_mm_storeu_si128(&pU[i], u);
		v = _mm_add_epi16(temp, b);
		_mm_storeu_si128(&pV[i], v);
	}

	for (i <<= 3; i < n; i++) {
		nr = R[i];
		ng = G[i];
		nb = B[i];

		Y[i] = (nr + 2 * ng + nb) >> 2;
		U[i] = nr - ng + nShift;
		V[i] = nb - ng + nShift;
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoReversibleColorRGB2YUV_S16_S16_aligned(
	mlib_s16 *Y,
	mlib_s16 *U,
	mlib_s16 *V,
	const mlib_s16 *R,
	const mlib_s16 *G,
	const mlib_s16 *B,
	mlib_s32 n,
	mlib_s32 depth)
{
	int i;
	mlib_s16 nShift;
	__m128i *pY, *pU, *pV, *pR, *pG, *pB;
	mlib_s16 nr, ng, nb;
	__m128i y, u, v, g, r, b;
	__m128i shift, temp;

	nShift = ((1 << depth) - 1);
	shift = _mm_set1_epi16(nShift);
	pY = (__m128i*)Y;
	pU = (__m128i*)U;
	pV = (__m128i*)V;
	pR = (__m128i*)R;
	pG = (__m128i*)G;
	pB = (__m128i*)B;
	i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < (n >> 3); i++) {
		r = _mm_load_si128(&pR[i]);
		g = _mm_load_si128(&pG[i]);
		b = _mm_load_si128(&pB[i]);

		temp = _mm_slli_epi16(g, 1);
		temp = _mm_add_epi16(r, temp);
		temp = _mm_add_epi16(temp, b);
		y = _mm_srai_epi16(temp, 2);
		_mm_store_si128(&pY[i], y);

		temp = _mm_sub_epi16(shift, g);

		u = _mm_add_epi16(temp, r);
		_mm_store_si128(&pU[i], u);
		v = _mm_add_epi16(temp, b);
		_mm_store_si128(&pV[i], v);
	}

	for (i <<= 3; i < n; i++) {
		nr = R[i];
		ng = G[i];
		nb = B[i];

		Y[i] = (nr + 2 * ng + nb) >> 2;
		U[i] = nr - ng + nShift;
		V[i] = nb - ng + nShift;
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_VideoReversibleColorRGB2YUV_S16_S16(
	mlib_s16 *Y,
	mlib_s16 *U,
	mlib_s16 *V,
	const mlib_s16 *R,
	const mlib_s16 *G,
	const mlib_s16 *B,
	mlib_s32 n,
	mlib_s32 depth)
{
	if (depth > 13) {
		mlib_s32 ir, ig, ib, iShift, i;

		iShift = ((1 << depth) - 1);
		for (i = 0; i < n; i++) {
			ir = R[i];
			ig = G[i];
			ib = B[i];

			Y[i] = (ir + 2 * ig + ib) >> 2;
			U[i] = ir - ig + iShift;
			V[i] = ib - ig + iShift;
		}
		return (MLIB_SUCCESS);
	}


	if (0 == (0xf & ((mlib_addr)R | (mlib_addr)G |
		(mlib_addr)B | (mlib_addr)Y |
		(mlib_addr)U | (mlib_addr)V))) {
		return mlib_VideoReversibleColorRGB2YUV_S16_S16_aligned(
				Y, U, V, R, G, B, n, depth);
	} else {
		return mlib_VideoReversibleColorRGB2YUV_S16_S16_naligned(
				Y, U, V, R, G, B, n, depth);
	}
}

mlib_status
mlib_VideoReversibleColorRGB2YUV_S32_S16_naligned(
	mlib_s32 *Y,
	mlib_s32 *U,
	mlib_s32 *V,
	const mlib_s16 *R,
	const mlib_s16 *G,
	const mlib_s16 *B,
	mlib_s32 n,
	mlib_s32 depth)
{
	int i;
	mlib_s32 iShift, ir, ig, ib;
	__m128i r, g, b;
	__m128i *pY, *pU, *pV, *pR, *pG, *pB;
	__m128i rh, rl, gh, gl, bh, bl, yh, yl, uh, ul, vh, vl;
	__m128i shift, temp, temph, templ;
	const __m128i zero = _mm_setzero_si128();

	iShift = ((1 << depth) - 1);
	shift = _mm_set1_epi32(iShift);
	pY = (__m128i*)Y;
	pU = (__m128i*)U;
	pV = (__m128i*)V;
	pR = (__m128i*)R;
	pG = (__m128i*)G;
	pB = (__m128i*)B;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (i = 0; i < (n >>3); i++) {
		r = _mm_loadu_si128(&pR[i]);
		g = _mm_loadu_si128(&pG[i]);
		b = _mm_loadu_si128(&pB[i]);
		rh = _mm_unpackhi_epi16(r, zero);
		rh = _mm_slli_epi32(rh, 16);
		rh = _mm_srai_epi32(rh, 16);
		rl = _mm_unpacklo_epi16(r, zero);
		rl = _mm_slli_epi32(rl, 16);
		rl = _mm_srai_epi32(rl, 16);
		gh = _mm_unpackhi_epi16(g, zero);
		gh = _mm_slli_epi32(gh, 16);
		gh = _mm_srai_epi32(gh, 16);
		gl = _mm_unpacklo_epi16(g, zero);
		gl = _mm_slli_epi32(gl, 16);
		gl = _mm_srai_epi32(gl, 16);
		bh = _mm_unpackhi_epi16(b, zero);
		bh = _mm_slli_epi32(bh, 16);
		bh = _mm_srai_epi32(bh, 16);
		bl = _mm_unpacklo_epi16(b, zero);
		bl = _mm_slli_epi32(bl, 16);
		bl = _mm_srai_epi32(bl, 16);

		temp = _mm_slli_epi32(gl, 1);
		temp = _mm_add_epi32(rl, temp);
		temp = _mm_add_epi32(bl, temp);
		yl = _mm_srai_epi32(temp, 2);
		_mm_storeu_si128(&pY[i << 1], yl);
		temp = _mm_slli_epi32(gh, 1);
		temp = _mm_add_epi32(rh, temp);
		temp = _mm_add_epi32(bh, temp);
		yh = _mm_srai_epi32(temp, 2);
		_mm_storeu_si128(&pY[(i << 1) + 1], yh);

		temph = _mm_sub_epi32(shift, gh);
		templ = _mm_sub_epi32(shift, gl);

		ul = _mm_add_epi32(templ, rl);
		_mm_storeu_si128(&pU[i << 1], ul);
		uh = _mm_add_epi32(temph, rh);
		_mm_storeu_si128(&pU[(i << 1) + 1], uh);

		vl = _mm_add_epi32(templ, bl);
		_mm_storeu_si128(&pV[i << 1], vl);
		vh = _mm_add_epi32(temph, bh);
		_mm_storeu_si128(&pV[(i << 1) + 1], vh);
	}

	for (i <<= 3; i < n; i++) {
		ir = R[i];
		ig = G[i];
		ib = B[i];

		Y[i] = (ir + 2 * ig + ib) >> 2;
		U[i] = ir - ig + iShift;
		V[i] = ib - ig + iShift;
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoReversibleColorRGB2YUV_S32_S16_aligned(
	mlib_s32 *Y,
	mlib_s32 *U,
	mlib_s32 *V,
	const mlib_s16 *R,
	const mlib_s16 *G,
	const mlib_s16 *B,
	mlib_s32 n,
	mlib_s32 depth)
{
	int i;
	mlib_s32 iShift, ir, ig, ib;
	__m128i r, g, b;
	__m128i *pY, *pU, *pV, *pR, *pG, *pB;
	__m128i rh, rl, gh, gl, bh, bl, yh, yl, uh, ul, vh, vl;
	__m128i shift, temp, temph, templ;
	const __m128i zero = _mm_setzero_si128();

	iShift = ((1 << depth) - 1);
	shift = _mm_set1_epi32(iShift);
	pY = (__m128i*)Y;
	pU = (__m128i*)U;
	pV = (__m128i*)V;
	pR = (__m128i*)R;
	pG = (__m128i*)G;
	pB = (__m128i*)B;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (i = 0; i < (n >> 3); i++) {
		r = _mm_load_si128(&pR[i]);
		g = _mm_load_si128(&pG[i]);
		b = _mm_load_si128(&pB[i]);
		rh = _mm_unpackhi_epi16(r, zero);
		rh = _mm_slli_epi32(rh, 16);
		rh = _mm_srai_epi32(rh, 16);
		rl = _mm_unpacklo_epi16(r, zero);
		rl = _mm_slli_epi32(rl, 16);
		rl = _mm_srai_epi32(rl, 16);
		gh = _mm_unpackhi_epi16(g, zero);
		gh = _mm_slli_epi32(gh, 16);
		gh = _mm_srai_epi32(gh, 16);
		gl = _mm_unpacklo_epi16(g, zero);
		gl = _mm_slli_epi32(gl, 16);
		gl = _mm_srai_epi32(gl, 16);
		bh = _mm_unpackhi_epi16(b, zero);
		bh = _mm_slli_epi32(bh, 16);
		bh = _mm_srai_epi32(bh, 16);
		bl = _mm_unpacklo_epi16(b, zero);
		bl = _mm_slli_epi32(bl, 16);
		bl = _mm_srai_epi32(bl, 16);

		temp = _mm_slli_epi32(gl, 1);
		temp = _mm_add_epi32(rl, temp);
		temp = _mm_add_epi32(bl, temp);
		yl = _mm_srai_epi32(temp, 2);
		_mm_store_si128(&pY[i << 1], yl);
		temp = _mm_slli_epi32(gh, 1);
		temp = _mm_add_epi32(rh, temp);
		temp = _mm_add_epi32(bh, temp);
		yh = _mm_srai_epi32(temp, 2);
		_mm_store_si128(&pY[(i << 1) + 1], yh);

		temph = _mm_sub_epi32(shift, gh);
		templ = _mm_sub_epi32(shift, gl);

		ul = _mm_add_epi32(templ, rl);
		_mm_store_si128(&pU[i << 1], ul);
		uh = _mm_add_epi32(temph, rh);
		_mm_store_si128(&pU[(i << 1) + 1], uh);

		vl = _mm_add_epi32(templ, bl);
		_mm_store_si128(&pV[i << 1], vl);
		vh = _mm_add_epi32(temph, bh);
		_mm_store_si128(&pV[(i << 1) + 1], vh);
	}

	for (i <<= 3; i < n; i++) {
		ir = R[i];
		ig = G[i];
		ib = B[i];

		Y[i] = (ir + 2 * ig + ib) >> 2;
		U[i] = ir - ig + iShift;
		V[i] = ib - ig + iShift;
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_VideoReversibleColorRGB2YUV_S32_S16(
	mlib_s32 *Y,
	mlib_s32 *U,
	mlib_s32 *V,
	const mlib_s16 *R,
	const mlib_s16 *G,
	const mlib_s16 *B,
	mlib_s32 n,
	mlib_s32 depth)
{
	if (0 == (0xf & ((mlib_addr)R | (mlib_addr)G |
		(mlib_addr)B | (mlib_addr)Y |
		(mlib_addr)U | (mlib_addr)V))) {
		return mlib_VideoReversibleColorRGB2YUV_S32_S16_aligned(
				Y, U, V, R, G, B, n, depth);
	} else {
		return mlib_VideoReversibleColorRGB2YUV_S32_S16_naligned(
				Y, U, V, R, G, B, n, depth);
	}
}

/* *********************************************************** */
