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

#pragma ident	"@(#)mlib_s_VideoRvrsibleClrYUV2RGB.c	9.5	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VideoReversibleColorYUV2RGB_U8_U8
 *      mlib_VideoReversibleColorYUV2RGB_U8_S16
 *      mlib_VideoReversibleColorYUV2RGB_S16_S16
 *      mlib_VideoReversibleColorYUV2RGB_S16_S32
 *                                    - convert from YUV to RGB
 *
 * SYNOPSIS
 *
 *      mlib_status
 *      mlib_c_VideoReversibleColorYUV2RGB_S16_S16(mlib_s16       *R,
 *                                                 mlib_s16       *G,
 *                                                 mlib_s16       *B,
 *                                                 const mlib_s16 *Y,
 *                                                 const mlib_s16 *U,
 *                                                 const mlib_s16 *V,
 *                                                 mlib_s32       n,
 *                                                 mlib_s32       depth);
 *      mlib_status
 *      mlib_c_VideoReversibleColorYUV2RGB_S16_S32(mlib_s16       *R,
 *                                                 mlib_s16       *G,
 *                                                 mlib_s16       *B,
 *                                                 const mlib_s32 *Y,
 *                                                 const mlib_s32 *U,
 *                                                 const mlib_s32 *V,
 *                                                 mlib_s32       n,
 *                                                 mlib_s32       depth);
 *      mlib_status
 *      mlib_c_VideoReversibleColorYUV2RGB_U8_S16(mlib_u8        *R,
 *                                                mlib_u8        *G,
 *                                                mlib_u8        *B,
 *                                                const mlib_s16 *Y,
 *                                                const mlib_s16 *U,
 *                                                const mlib_s16 *V,
 *                                                mlib_s32       n,
 *                                                mlib_s32       depth);
 *      mlib_status
 *      mlib_c_VideoReversibleColorYUV2RGB_U8_U8(mlib_u8       *R,
 *                                               mlib_u8       *G,
 *                                               mlib_u8       *B,
 *                                               const mlib_u8 *Y,
 *                                               const mlib_u8 *U,
 *                                               const mlib_u8 *V,
 *                                               mlib_s32      n,
 *                                               mlib_s32      depth);
 *
 * ARGUMENT
 *      R     pointer to output R component
 *      G     pointer to output G component
 *      B     pointer to output B component
 *      Y     pointer to input Y component
 *      U     pointer to input U component
 *      V     pointer to input V component
 *      n     length components
 *      depth number of bit_planes required to store the original
 *            R, G, and B components
 *
 * RESTRICTION
 *  All components must have the same sizes and a aligned to 8 bytes.
 *  Possible variants are following:
 *      MLIB_U8    -> MLIB_U8            depth = 0 .. 7
 *      MLIB_S16   -> MLIB_U8            depth = 0 .. 8
 *      MLIB_S16   -> MLIB_S16           depth = 0 .. 15
 *      MLIB_S32   -> MLIB_S16           depth = 0 .. 16
 *
 * DESCRIPTION
 *
 *  G = Y - (U - 2*(2^depth - 1) + V) / 4;
 *  R = U + G - 2^depth + 1;
 *  B = V + G - 2^depth + 1;
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

#pragma weak mlib_VideoReversibleColorYUV2RGB_S16_S16 = \
	__mlib_VideoReversibleColorYUV2RGB_S16_S16
#pragma weak mlib_VideoReversibleColorYUV2RGB_S16_S32 = \
	__mlib_VideoReversibleColorYUV2RGB_S16_S32
#pragma weak mlib_VideoReversibleColorYUV2RGB_U8_S16 = \
	__mlib_VideoReversibleColorYUV2RGB_U8_S16
#pragma weak mlib_VideoReversibleColorYUV2RGB_U8_U8 = \
	__mlib_VideoReversibleColorYUV2RGB_U8_U8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoReversibleColorYUV2RGB_S16_S16)
	mlib_VideoReversibleColorYUV2RGB_S16_S16
	__attribute__((weak,
	alias("__mlib_VideoReversibleColorYUV2RGB_S16_S16")));
__typeof__(__mlib_VideoReversibleColorYUV2RGB_S16_S32)
	mlib_VideoReversibleColorYUV2RGB_S16_S32
	__attribute__((weak,
	alias("__mlib_VideoReversibleColorYUV2RGB_S16_S32")));
__typeof__(__mlib_VideoReversibleColorYUV2RGB_U8_S16)
	mlib_VideoReversibleColorYUV2RGB_U8_S16
	__attribute__((weak,
	alias("__mlib_VideoReversibleColorYUV2RGB_U8_S16")));
__typeof__(__mlib_VideoReversibleColorYUV2RGB_U8_U8)
	mlib_VideoReversibleColorYUV2RGB_U8_U8
	__attribute__((weak,
	alias("__mlib_VideoReversibleColorYUV2RGB_U8_U8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
mlib_VideoReversibleColorYUV2RGB_U8_U8_naligned(
	mlib_u8 *R,
	mlib_u8 *G,
	mlib_u8 *B,
	const mlib_u8 *Y,
	const mlib_u8 *U,
	const mlib_u8 *V,
	mlib_s32 n,
	mlib_s32 depth)
{
	int i;
	mlib_s16 nShift;
	mlib_s16 ny, nu, nv, nr, ng, nb;

	__m128i y, u, v, g, r, b;
	__m128i yh, yl, uh, ul, vh, vl, gh, gl, rh, rl, bh, bl;
	__m128i temp, temph, templ;
	__m128i *pY, *pU, *pV, *pR, *pG, *pB;
	__m128i shift, shift2;
	const __m128i zero = _mm_setzero_si128();

	nShift = ((1 << depth) - 1);
	shift = _mm_set1_epi16(nShift);
	shift2 = _mm_slli_epi16(shift, 1);
	pY = (__m128i*)Y;
	pU = (__m128i*)U;
	pV = (__m128i*)V;
	pR = (__m128i*)R;
	pG = (__m128i*)G;
	pB = (__m128i*)B;
	i = 0;
	for (; i < (n >> 4); i++) {
		y = _mm_loadu_si128(&pY[i]);
		u = _mm_loadu_si128(&pU[i]);
		v = _mm_loadu_si128(&pV[i]);
		yh = _mm_unpackhi_epi8(y, zero);
		yl = _mm_unpacklo_epi8(y, zero);
		uh = _mm_unpackhi_epi8(u, zero);
		ul = _mm_unpacklo_epi8(u, zero);
		vh = _mm_unpackhi_epi8(v, zero);
		vl = _mm_unpacklo_epi8(v, zero);

		temp = _mm_add_epi16(uh, vh);
		temp = _mm_sub_epi16(temp, shift2);
		temp = _mm_srai_epi16(temp, 2);
		gh = _mm_sub_epi16(yh, temp);
		temp = _mm_add_epi16(ul, vl);
		temp = _mm_sub_epi16(temp, shift2);
		temp = _mm_srai_epi16(temp, 2);
		gl = _mm_sub_epi16(yl, temp);
		g = _mm_packus_epi16(gl, gh);
		_mm_storeu_si128(&pG[i], g);

		temph = _mm_sub_epi16(gh, shift);
		templ = _mm_sub_epi16(gl, shift);

		rh = _mm_add_epi16(temph, uh);
		rl = _mm_add_epi16(templ, ul);
		r = _mm_packus_epi16(rl, rh);
		_mm_storeu_si128(&pR[i], r);

		bh = _mm_add_epi16(temph, vh);
		bl = _mm_add_epi16(templ, vl);
		b = _mm_packus_epi16(bl, bh);
		_mm_storeu_si128(&pB[i], b);
	}

	/* the rest */
	for (i <<= 4; i < n; i++) {
		ny = Y[i];
		nu = U[i];
		nv = V[i];

		ng = ny - ((nu - 2 * nShift + nv) >> 2);
		nr = nu + ng - nShift;
		nb = nv + ng - nShift;

		R[i] = nr;
		G[i] = ng;
		B[i] = nb;
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoReversibleColorYUV2RGB_U8_U8_aligned(
	mlib_u8 *R,
	mlib_u8 *G,
	mlib_u8 *B,
	const mlib_u8 *Y,
	const mlib_u8 *U,
	const mlib_u8 *V,
	mlib_s32 n,
	mlib_s32 depth)
{
	int i;
	mlib_s16 nShift;
	mlib_s16 ny, nu, nv, nr, ng, nb;

	__m128i y, u, v, g, r, b;
	__m128i yh, yl, uh, ul, vh, vl, gh, gl, rh, rl, bh, bl;
	__m128i temp, temph, templ;
	__m128i *pY, *pU, *pV, *pR, *pG, *pB;
	__m128i shift, shift2;
	const __m128i zero = _mm_setzero_si128();

	nShift = ((1 << depth) - 1);
	shift = _mm_set1_epi16(nShift);
	shift2 = _mm_slli_epi16(shift, 1);
	pY = (__m128i*)Y;
	pU = (__m128i*)U;
	pV = (__m128i*)V;
	pR = (__m128i*)R;
	pG = (__m128i*)G;
	pB = (__m128i*)B;
	i = 0;
	for (; i < (n >> 4); i++) {
		y = _mm_load_si128(&pY[i]);
		u = _mm_load_si128(&pU[i]);
		v = _mm_load_si128(&pV[i]);
		yh = _mm_unpackhi_epi8(y, zero);
		yl = _mm_unpacklo_epi8(y, zero);
		uh = _mm_unpackhi_epi8(u, zero);
		ul = _mm_unpacklo_epi8(u, zero);
		vh = _mm_unpackhi_epi8(v, zero);
		vl = _mm_unpacklo_epi8(v, zero);

		temp = _mm_add_epi16(uh, vh);
		temp = _mm_sub_epi16(temp, shift2);
		temp = _mm_srai_epi16(temp, 2);
		gh = _mm_sub_epi16(yh, temp);
		temp = _mm_add_epi16(ul, vl);
		temp = _mm_sub_epi16(temp, shift2);
		temp = _mm_srai_epi16(temp, 2);
		gl = _mm_sub_epi16(yl, temp);
		g = _mm_packus_epi16(gl, gh);
		_mm_store_si128(&pG[i], g);

		temph = _mm_sub_epi16(gh, shift);
		templ = _mm_sub_epi16(gl, shift);

		rh = _mm_add_epi16(temph, uh);
		rl = _mm_add_epi16(templ, ul);
		r = _mm_packus_epi16(rl, rh);
		_mm_store_si128(&pR[i], r);

		bh = _mm_add_epi16(temph, vh);
		bl = _mm_add_epi16(templ, vl);
		b = _mm_packus_epi16(bl, bh);
		_mm_store_si128(&pB[i], b);
	}

	/* the rest */
	for (i <<= 4; i < n; i++) {
		ny = Y[i];
		nu = U[i];
		nv = V[i];

		ng = ny - ((nu - 2 * nShift + nv) >> 2);
		nr = nu + ng - nShift;
		nb = nv + ng - nShift;

		R[i] = nr;
		G[i] = ng;
		B[i] = nb;
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_VideoReversibleColorYUV2RGB_U8_U8(
	mlib_u8 *R,
	mlib_u8 *G,
	mlib_u8 *B,
	const mlib_u8 *Y,
	const mlib_u8 *U,
	const mlib_u8 *V,
	mlib_s32 n,
	mlib_s32 depth)
{
	if (0 == (0xf & ((mlib_addr)R | (mlib_addr)G |
		(mlib_addr)B | (mlib_addr)Y | (mlib_addr)U |
		(mlib_addr)V))) {
		return mlib_VideoReversibleColorYUV2RGB_U8_U8_aligned(
				R, G, B, Y, U, V, n, depth);
	} else {
		return mlib_VideoReversibleColorYUV2RGB_U8_U8_naligned(
				R, G, B, Y, U, V, n, depth);
	}

}
/* *********************************************************** */

mlib_status
mlib_VideoReversibleColorYUV2RGB_U8_S16_naligned(
	mlib_u8 *R,
	mlib_u8 *G,
	mlib_u8 *B,
	const mlib_s16 *Y,
	const mlib_s16 *U,
	const mlib_s16 *V,
	mlib_s32 n,
	mlib_s32 depth)
{
	int i;
	mlib_s16 nShift;
	mlib_s16 ny, nu, nv, nr, ng, nb;

	__m128i y, u, v, g, r, b, temp;
	__m128i *pY, *pU, *pV;
	__m64 *pR, *pG, *pB;
	__m128i shift, shift2;
	const __m128i zero = _mm_setzero_si128();

	nShift = ((1 << depth) - 1);
	shift = _mm_set1_epi16(nShift);
	shift2 = _mm_slli_epi16(shift, 1);
	pY = (__m128i*)Y;
	pU = (__m128i*)U;
	pV = (__m128i*)V;
	pR = (__m64*)R;
	pG = (__m64*)G;
	pB = (__m64*)B;
	i = 0;
	for (; i < (n >> 3); i++) {
		y = _mm_loadu_si128(&pY[i]);
		u = _mm_loadu_si128(&pU[i]);
		v = _mm_loadu_si128(&pV[i]);

		temp = _mm_add_epi16(u, v);
		temp = _mm_sub_epi16(temp, shift2);
		temp = _mm_srai_epi16(temp, 2);
		g = _mm_sub_epi16(y, temp);
		temp = _mm_packus_epi16(g, zero);
		_mm_storel_epi64((__m128i*)&pG[i], temp);

		temp = _mm_sub_epi16(g, shift);

		r = _mm_add_epi16(u, temp);
		r = _mm_packus_epi16(r, zero);
		_mm_storel_epi64((__m128i*)&pR[i], r);

		b = _mm_add_epi16(v, temp);
		b = _mm_packus_epi16(b, zero);
		_mm_storel_epi64((__m128i*)&pB[i], b);
	}

	/* the rest */
	for (i <<= 3; i < n; i++) {
		ny = Y[i];
		nu = U[i];
		nv = V[i];

		ng = ny - ((nu - 2 * nShift + nv) >> 2);
		nr = nu + ng - nShift;
		nb = nv + ng - nShift;

		R[i] = nr;
		G[i] = ng;
		B[i] = nb;
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoReversibleColorYUV2RGB_U8_S16_aligned(
	mlib_u8 *R,
	mlib_u8 *G,
	mlib_u8 *B,
	const mlib_s16 *Y,
	const mlib_s16 *U,
	const mlib_s16 *V,
	mlib_s32 n,
	mlib_s32 depth)
{
	int i;
	mlib_s16 nShift;
	mlib_s16 ny, nu, nv, nr, ng, nb;

	__m128i y, u, v, g, r, b, temp;
	__m128i *pY, *pU, *pV;
	__m64 *pR, *pG, *pB;
	__m128i shift, shift2;
	const __m128i zero = _mm_setzero_si128();

	nShift = ((1 << depth) - 1);
	shift = _mm_set1_epi16(nShift);
	shift2 = _mm_slli_epi16(shift, 1);
	pY = (__m128i*)Y;
	pU = (__m128i*)U;
	pV = (__m128i*)V;
	pR = (__m64*)R;
	pG = (__m64*)G;
	pB = (__m64*)B;
	i = 0;
	for (; i < (n >> 3); i++) {
		y = _mm_load_si128(&pY[i]);
		u = _mm_load_si128(&pU[i]);
		v = _mm_load_si128(&pV[i]);

		temp = _mm_add_epi16(u, v);
		temp = _mm_sub_epi16(temp, shift2);
		temp = _mm_srai_epi16(temp, 2);
		g = _mm_sub_epi16(y, temp);
		temp = _mm_packus_epi16(g, zero);
		_mm_storel_epi64((__m128i*)&pG[i], temp);

		temp = _mm_sub_epi16(g, shift);

		r = _mm_add_epi16(u, temp);
		r = _mm_packus_epi16(r, zero);
		_mm_storel_epi64((__m128i*)&pR[i], r);

		b = _mm_add_epi16(v, temp);
		b = _mm_packus_epi16(b, zero);
		_mm_storel_epi64((__m128i*)&pB[i], b);
	}

	/* the rest */
	for (i <<= 3; i < n; i++) {
		ny = Y[i];
		nu = U[i];
		nv = V[i];

		ng = ny - ((nu - 2 * nShift + nv) >> 2);
		nr = nu + ng - nShift;
		nb = nv + ng - nShift;

		R[i] = nr;
		G[i] = ng;
		B[i] = nb;
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_VideoReversibleColorYUV2RGB_U8_S16(
	mlib_u8 *R,
	mlib_u8 *G,
	mlib_u8 *B,
	const mlib_s16 *Y,
	const mlib_s16 *U,
	const mlib_s16 *V,
	mlib_s32 n,
	mlib_s32 depth)
{
	if (0 == (0xf & ((mlib_addr)Y | (mlib_addr)U |
		(mlib_addr)V))) {
		return mlib_VideoReversibleColorYUV2RGB_U8_S16_aligned(
				R, G, B, Y, U, V, n, depth);
	} else {
		return mlib_VideoReversibleColorYUV2RGB_U8_S16_naligned(
				R, G, B, Y, U, V, n, depth);
	}
}

/* *********************************************************** */

mlib_status
mlib_VideoReversibleColorYUV2RGB_S16_S16_naligned(
	mlib_s16 *R,
	mlib_s16 *G,
	mlib_s16 *B,
	const mlib_s16 *Y,
	const mlib_s16 *U,
	const mlib_s16 *V,
	mlib_s32 n,
	mlib_s32 depth)
{
	int i;
	mlib_s16 nShift;
	mlib_s16 ny, nu, nv, nr, ng, nb;

	__m128i y, u, v, g, r, b, temp;
	__m128i *pY, *pU, *pV;
	__m128i *pR, *pG, *pB;
	__m128i shift, shift2;

	nShift = ((1 << depth) - 1);
	shift = _mm_set1_epi16(nShift);
	shift2 = _mm_slli_epi16(shift, 1);
	pY = (__m128i*)Y;
	pU = (__m128i*)U;
	pV = (__m128i*)V;
	pR = (__m128i*)R;
	pG = (__m128i*)G;
	pB = (__m128i*)B;
	i = 0;
	for (; i < (n >> 3); i++) {
		y = _mm_loadu_si128(&pY[i]);
		u = _mm_loadu_si128(&pU[i]);
		v = _mm_loadu_si128(&pV[i]);

		temp = _mm_add_epi16(u, v);
		temp = _mm_sub_epi16(temp, shift2);
		temp = _mm_srai_epi16(temp, 2);
		g = _mm_sub_epi16(y, temp);
		_mm_storeu_si128(&pG[i], g);

		temp = _mm_sub_epi16(g, shift);

		r = _mm_add_epi16(u, temp);
		_mm_storeu_si128(&pR[i], r);

		b = _mm_add_epi16(v, temp);
		_mm_storeu_si128(&pB[i], b);
	}

	/* the rest */
	for (i <<= 3; i < n; i++) {
		ny = Y[i];
		nu = U[i];
		nv = V[i];

		ng = ny - ((nu - 2 * nShift + nv) >> 2);
		nr = nu + ng - nShift;
		nb = nv + ng - nShift;

		R[i] = nr;
		G[i] = ng;
		B[i] = nb;
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoReversibleColorYUV2RGB_S16_S16_aligned(
	mlib_s16 *R,
	mlib_s16 *G,
	mlib_s16 *B,
	const mlib_s16 *Y,
	const mlib_s16 *U,
	const mlib_s16 *V,
	mlib_s32 n,
	mlib_s32 depth)
{
	int i;
	mlib_s16 nShift;
	mlib_s16 ny, nu, nv, nr, ng, nb;

	__m128i y, u, v, g, r, b, temp;
	__m128i *pY, *pU, *pV;
	__m128i *pR, *pG, *pB;
	__m128i shift, shift2;

	nShift = ((1 << depth) - 1);
	shift = _mm_set1_epi16(nShift);
	shift2 = _mm_slli_epi16(shift, 1);
	pY = (__m128i*)Y;
	pU = (__m128i*)U;
	pV = (__m128i*)V;
	pR = (__m128i*)R;
	pG = (__m128i*)G;
	pB = (__m128i*)B;
	i = 0;
	for (; i < (n >> 3); i++) {
		y = _mm_load_si128(&pY[i]);
		u = _mm_load_si128(&pU[i]);
		v = _mm_load_si128(&pV[i]);

		temp = _mm_add_epi16(u, v);
		temp = _mm_sub_epi16(temp, shift2);
		temp = _mm_srai_epi16(temp, 2);
		g = _mm_sub_epi16(y, temp);
		_mm_store_si128(&pG[i], g);

		temp = _mm_sub_epi16(g, shift);

		r = _mm_add_epi16(u, temp);
		_mm_store_si128(&pR[i], r);

		b = _mm_add_epi16(v, temp);
		_mm_store_si128(&pB[i], b);
	}

	/* the rest */
	for (i <<= 3; i < n; i++) {
		ny = Y[i];
		nu = U[i];
		nv = V[i];

		ng = ny - ((nu - 2 * nShift + nv) >> 2);
		nr = nu + ng - nShift;
		nb = nv + ng - nShift;

		R[i] = nr;
		G[i] = ng;
		B[i] = nb;
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_VideoReversibleColorYUV2RGB_S16_S16(
	mlib_s16 *R,
	mlib_s16 *G,
	mlib_s16 *B,
	const mlib_s16 *Y,
	const mlib_s16 *U,
	const mlib_s16 *V,
	mlib_s32 n,
	mlib_s32 depth)
{
	if (depth > 13) {
		int i;
		mlib_s32 iShift;
		mlib_s32 iy, iu, iv, ir, ig, ib;

		iShift = ((1 << depth) - 1);

		for (i = 0; i < n; i++) {
			iy = Y[i];
			iu = U[i];
			iv = V[i];

			ig = iy - ((iu - 2 * iShift + iv) >> 2);
			ir = iu + ig - iShift;
			ib = iv + ig - iShift;

			R[i] = ir;
			G[i] = ig;
			B[i] = ib;
		}

		return (MLIB_SUCCESS);
	}

	if (0 == (0xf & ((mlib_addr)R | (mlib_addr)G |
		(mlib_addr)B | (mlib_addr)Y | (mlib_addr)U |
		(mlib_addr)V))) {
		return mlib_VideoReversibleColorYUV2RGB_S16_S16_aligned(
				R, G, B, Y, U, V, n, depth);
	} else {
		return mlib_VideoReversibleColorYUV2RGB_S16_S16_naligned(
				R, G, B, Y, U, V, n, depth);
	}
}

/* *********************************************************** */

mlib_status
mlib_VideoReversibleColorYUV2RGB_S16_S32_naligned(
	mlib_s16 *R,
	mlib_s16 *G,
	mlib_s16 *B,
	const mlib_s32 *Y,
	const mlib_s32 *U,
	const mlib_s32 *V,
	mlib_s32 n,
	mlib_s32 depth)
{
	int i;
	mlib_s32 iShift;
	mlib_s32 iy, iu, iv, ir, ig, ib;

	__m128i y, u, v, g, r, b, temp;
	__m128i *pY, *pU, *pV;
	__m64 *pR, *pG, *pB;
	__m128i shift, shift2;
	const __m128i zero = _mm_setzero_si128();

	iShift = ((1 << depth) - 1);
	shift = _mm_set1_epi32(iShift);
	shift2 = _mm_slli_epi32(shift, 1);
	pY = (__m128i*)Y;
	pU = (__m128i*)U;
	pV = (__m128i*)V;
	pR = (__m64*)R;
	pG = (__m64*)G;
	pB = (__m64*)B;
	for (i = 0; i < (n >> 2); i++) {
		y = _mm_loadu_si128(&pY[i]);
		u = _mm_loadu_si128(&pU[i]);
		v = _mm_loadu_si128(&pV[i]);

		temp = _mm_add_epi32(u, v);
		temp = _mm_sub_epi32(temp, shift2);
		temp = _mm_srai_epi32(temp, 2);
		g = _mm_sub_epi32(y, temp);
		temp = _mm_slli_epi32(g, 16);
		temp = _mm_srai_epi32(temp, 16);
		temp = _mm_packs_epi32(temp, zero);
		_mm_storel_epi64((__m128i*)&pG[i], temp);

		temp = _mm_sub_epi32(g, shift);

		r = _mm_add_epi32(u, temp);
		r = _mm_slli_epi32(r, 16);
		r = _mm_srai_epi32(r, 16);
		r = _mm_packs_epi32(r, zero);
		_mm_storel_epi64((__m128i*)&pR[i], r);

		b = _mm_add_epi32(v, temp);
		b = _mm_slli_epi32(b, 16);
		b = _mm_srai_epi32(b, 16);
		b = _mm_packs_epi32(b, zero);
		_mm_storel_epi64((__m128i*)&pB[i], b);
	}

	for (i <<= 2; i < n; i++) {
		iy = Y[i];
		iu = U[i];
		iv = V[i];

		ig = iy - ((iu - 2 * iShift + iv) >> 2);
		ir = iu + ig - iShift;
		ib = iv + ig - iShift;

		R[i] = ir;
		G[i] = ig;
		B[i] = ib;
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoReversibleColorYUV2RGB_S16_S32_aligned(
	mlib_s16 *R,
	mlib_s16 *G,
	mlib_s16 *B,
	const mlib_s32 *Y,
	const mlib_s32 *U,
	const mlib_s32 *V,
	mlib_s32 n,
	mlib_s32 depth)
{
	int i;
	mlib_s32 iShift;
	mlib_s32 iy, iu, iv, ir, ig, ib;

	__m128i y, u, v, g, r, b, temp;
	__m128i *pY, *pU, *pV;
	__m64 *pR, *pG, *pB;
	__m128i shift, shift2;
	const __m128i zero = _mm_setzero_si128();

	iShift = ((1 << depth) - 1);
	shift = _mm_set1_epi32(iShift);
	shift2 = _mm_slli_epi32(shift, 1);
	pY = (__m128i*)Y;
	pU = (__m128i*)U;
	pV = (__m128i*)V;
	pR = (__m64*)R;
	pG = (__m64*)G;
	pB = (__m64*)B;
	for (i = 0; i < (n >> 2); i++) {
		y = _mm_load_si128(&pY[i]);
		u = _mm_load_si128(&pU[i]);
		v = _mm_load_si128(&pV[i]);

		temp = _mm_add_epi32(u, v);
		temp = _mm_sub_epi32(temp, shift2);
		temp = _mm_srai_epi32(temp, 2);
		g = _mm_sub_epi32(y, temp);
		temp = _mm_slli_epi32(g, 16);
		temp = _mm_srai_epi32(temp, 16);
		temp = _mm_packs_epi32(temp, zero);
		_mm_storel_epi64((__m128i*)&pG[i], temp);

		temp = _mm_sub_epi32(g, shift);

		r = _mm_add_epi32(u, temp);
		r = _mm_slli_epi32(r, 16);
		r = _mm_srai_epi32(r, 16);
		r = _mm_packs_epi32(r, zero);
		_mm_storel_epi64((__m128i*)&pR[i], r);

		b = _mm_add_epi32(v, temp);
		b = _mm_slli_epi32(b, 16);
		b = _mm_srai_epi32(b, 16);
		b = _mm_packs_epi32(b, zero);
		_mm_storel_epi64((__m128i*)&pB[i], b);
	}

	for (i <<= 2; i < n; i++) {
		iy = Y[i];
		iu = U[i];
		iv = V[i];

		ig = iy - ((iu - 2 * iShift + iv) >> 2);
		ir = iu + ig - iShift;
		ib = iv + ig - iShift;

		R[i] = ir;
		G[i] = ig;
		B[i] = ib;
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_VideoReversibleColorYUV2RGB_S16_S32(
	mlib_s16 *R,
	mlib_s16 *G,
	mlib_s16 *B,
	const mlib_s32 *Y,
	const mlib_s32 *U,
	const mlib_s32 *V,
	mlib_s32 n,
	mlib_s32 depth)
{

	if (0 == (0xf & ((mlib_addr)Y | (mlib_addr)U |
		(mlib_addr)V))) {
		return mlib_VideoReversibleColorYUV2RGB_S16_S32_aligned(
				R, G, B, Y, U, V, n, depth);
	} else {
		return mlib_VideoReversibleColorYUV2RGB_S16_S32_naligned(
				R, G, B, Y, U, V, n, depth);
	}
}

/* *********************************************************** */
