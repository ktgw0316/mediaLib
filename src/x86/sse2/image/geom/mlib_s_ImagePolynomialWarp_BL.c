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

#pragma ident   "@(#)mlib_s_ImagePolynomialWarp_BL.c	9.2	07/11/05 SMI"

#define	MMX_USE_MODIF_8x16

#include <mlib_image.h>
#include <mlib_s_ImageFilters.h>
#include <mlib_ImagePolynomialWarp.h>
#ifdef  __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	MLIB_POINTER_SHIFT(P)	(P >> (MLIB_SHIFT-2)) &~ 3
#define	FILTER_SHIFT	4

#define	FILTER_MASK	(((1 << 8) - 1) << 3)

#define	LGSTEPS	8

#define	STEPS	(1 << LGSTEPS)

/* *********************************************************** */

#define	DECLAREVAR                                              \
	mlib_s32 *xs = pws->src_x;                              \
	mlib_s32 *ys = pws->src_y;                              \
	mlib_s32 *xd = pws->dst_x;                              \
	mlib_u16 *ycoeff = pws->dsrc_y;                         \
	mlib_u16 *xcoeff = pws->dsrc_x;                         \
	mlib_s32 i;                                             \
	mlib_s32 srcYStride = lineAddr[1] - lineAddr[0];        \
	mlib_s32 filterpos

/* *********************************************************** */

#undef MLIB_TYPE
#define	MLIB_TYPE	mlib_u8

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U8_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	DECLAREVAR;
	mlib_s32 X, Y;
	const MLIB_TYPE *srcPixelPtr;
	__m128i r0;
	__m128i _s_zero = _mm_setzero_si128();
	__m128i xfilter, yfilter, filter;

	for (i = 0; i < n; i++) {
		srcPixelPtr = lineAddr[*ys] + *xs;
		filterpos = (*xcoeff >> FILTER_SHIFT) & FILTER_MASK;
		xfilter = _mm_loadl_epi64((__m128i *)
					((mlib_u8 *)mlib_filters_u8_bl +
					filterpos));
		filterpos = (*ycoeff >> FILTER_SHIFT) & FILTER_MASK;
		yfilter = _mm_loadl_epi64((__m128i *)
					((mlib_u8 *)mlib_filters_u8_bl +
					filterpos + STEPS * 8));
		filter = _mm_mulhi_epi16(xfilter, yfilter);
		r0 = _mm_cvtsi32_si128(*(mlib_u16 *)srcPixelPtr |
			(*(mlib_u16 *)(srcPixelPtr + srcYStride) << 16));
		r0 = _mm_madd_epi16(_mm_unpacklo_epi8(r0, _s_zero), filter);
		r0 = _mm_add_epi32(r0, _mm_srli_si128(r0, 4));

		dstData[xd[0]] = (_mm_cvtsi128_si32(r0) >> 12);
		xs++;
		ys++;
		xd++;
		xcoeff++;
		ycoeff++;
	}
}

/* *********************************************************** */

#undef	MMX_SCALE
#define	MMX_SCALE	3

void
mlib_ImagePolynomialWarp_BL_U8_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	DECLAREVAR;
	mlib_s32 X, Y;
	const MLIB_TYPE *srcPixelPtr;

	__m128i r0, s0, s1;
	__m128i _s_zero = _mm_setzero_si128();
	__m128i k05 = _mm_set1_epi32(0x00040004);
	__m128i xfilter, yfilter, filter;
	mlib_s32 ri;

	for (i = 0; i < n; i++) {
		srcPixelPtr = lineAddr[*ys] + 2 * xs[0];

		filterpos = (*xcoeff >> FILTER_SHIFT) & FILTER_MASK;
		xfilter = _mm_loadl_epi64((__m128i *)
					((mlib_u8 *)mlib_filters_u8_bl +
					filterpos));
		filterpos = (*ycoeff >> FILTER_SHIFT) & FILTER_MASK;
		yfilter = _mm_loadl_epi64((__m128i *)
					((mlib_u8 *)mlib_filters_u8_bl +
					filterpos + STEPS * 8));

		filter = _mm_mulhi_epi16(xfilter, yfilter);
		filter = _mm_unpacklo_epi16(filter, filter);

		s0 = _mm_loadl_epi64((__m128i *)srcPixelPtr);
		s1 = _mm_loadl_epi64((__m128i *)(srcPixelPtr + srcYStride));

		r0 = _mm_unpacklo_epi32(s0, s1);
		r0 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(r0,
				_s_zero), 7), filter);
		r0 = _mm_add_epi16(r0, _mm_srli_si128(r0, 8));
		r0 = _mm_add_epi16(r0, _mm_srli_si128(r0, 4));
		r0 = _mm_add_epi16(r0, k05);

		ri = _mm_cvtsi128_si32(_mm_packus_epi16(
				_mm_srai_epi16(r0, 6 - MMX_SCALE), _s_zero));

		((mlib_s16 *)dstData)[xd[0]] = ri;
		xs++;
		ys++;
		xd++;
		xcoeff++;
		ycoeff++;
	}
}

/* *********************************************************** */

#undef	MMX_SCALE
#define	MMX_SCALE	3

void
mlib_ImagePolynomialWarp_BL_U8_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	DECLAREVAR;
	const MLIB_TYPE *srcPixelPtr;
	__m128i s0, s1, filter0, filter1;
	__m128i _s_zero = _mm_setzero_si128();
	__m128i k05 = _mm_set1_epi32(0x00040004);
	__m128i xfilter, yfilter, filter;
	__m128i r0, r1;
	mlib_s32 ri, xdst;

	for (i = 0; i < n; i++) {
		srcPixelPtr = lineAddr[ys[0]] + 3 * xs[0];

		filterpos = (*xcoeff >> FILTER_SHIFT) & FILTER_MASK;
		xfilter = _mm_loadl_epi64((__m128i *)
					((mlib_u8 *)mlib_filters_u8_bl +
					filterpos));
		filterpos = (*ycoeff >> FILTER_SHIFT) & FILTER_MASK;
		yfilter = _mm_loadl_epi64((__m128i *)
					((mlib_u8 *)mlib_filters_u8_bl +
					filterpos + STEPS * 8));

		filter = _mm_mulhi_epi16(xfilter, yfilter);
		filter = _mm_unpacklo_epi16(filter, filter);
		filter0 = _mm_unpacklo_epi32(filter, filter);
		filter1 = _mm_unpackhi_epi32(filter, filter);

		s0 = _mm_loadl_epi64((__m128i *)srcPixelPtr);
		s0 = _mm_unpacklo_epi32(s0, _mm_srli_si128(s0, 3));
		s1 = _mm_loadl_epi64((__m128i *)(srcPixelPtr + srcYStride));
		s1 = _mm_unpacklo_epi32(s1, _mm_srli_si128(s1, 3));

		r0 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(s0,
				_s_zero), 7), filter0);
		r1 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(s1,
				_s_zero), 7), filter1);

		r0 = _mm_add_epi16(r0, r1);
		r0 = _mm_add_epi16(r0, _mm_srli_si128(r0, 8));
		r0 = _mm_add_epi16(r0, k05);

		ri = _mm_cvtsi128_si32(_mm_packus_epi16(
				_mm_srai_epi16(r0, 6 - MMX_SCALE), _s_zero));

		xdst = xd[0] * 3;
		dstData[xdst] = ri;
		dstData[xdst + 1] = (ri >> 8);
		dstData[xdst + 2] = (ri >> 16);

		xs++;
		ys++;
		xd++;
		xcoeff++;
		ycoeff++;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U8_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	DECLAREVAR;
	const MLIB_TYPE *srcPixelPtr;
	__m128i s0, s1, filter0, filter1;
	__m128i _s_zero = _mm_setzero_si128();
	__m128i k05 = _mm_set1_epi32(0x00040004);
	__m128i xfilter, yfilter, filter;
	__m128i r0, r1;
	mlib_s32 ri;

	for (i = 0; i < n; i++) {
		srcPixelPtr = lineAddr[ys[0]] + 4 * xs[0];

		filterpos = (*xcoeff >> FILTER_SHIFT) & FILTER_MASK;
		xfilter = _mm_loadl_epi64((__m128i *)
					((mlib_u8 *)mlib_filters_u8_bl +
					filterpos));
		filterpos = (*ycoeff >> FILTER_SHIFT) & FILTER_MASK;
		yfilter = _mm_loadl_epi64((__m128i *)
					((mlib_u8 *)mlib_filters_u8_bl +
					filterpos + STEPS * 8));

		filter = _mm_mulhi_epi16(xfilter, yfilter);
		filter = _mm_unpacklo_epi16(filter, filter);
		filter0 = _mm_unpacklo_epi32(filter, filter);
		filter1 = _mm_unpackhi_epi32(filter, filter);

		s0 = _mm_loadl_epi64((__m128i *)srcPixelPtr);
		s1 = _mm_loadl_epi64((__m128i *)(srcPixelPtr + srcYStride));

		r0 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(s0,
				_s_zero), 7), filter0);
		r1 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(s1,
				_s_zero), 7), filter1);

		r0 = _mm_add_epi16(r0, r1);
		r0 = _mm_add_epi16(r0, _mm_srli_si128(r0, 8));
		r0 = _mm_add_epi16(r0, k05);

		ri = _mm_cvtsi128_si32(_mm_packus_epi16(
				_mm_srai_epi16(r0, 6 - MMX_SCALE), _s_zero));

		((mlib_s32 *)dstData)[xd[0]] = ri;

		xs++;
		ys++;
		xd++;
		xcoeff++;
		ycoeff++;
	}
}

/* *********************************************************** */

#undef	MLIB_TYPE
#define	MLIB_TYPE	mlib_s16

/* *********************************************************** */
#define	NEXT_PIXELS_1BL_S16                                        \
	srcPixelPtr0 = (MLIB_TYPE *) (lineAddr[ys[0]]) + xs[0];       \
	srcPixelPtr1 = (MLIB_TYPE *) (lineAddr[ys[1]]) + xs[1];       \
	srcPixelPtr2 = (MLIB_TYPE *) (lineAddr[ys[2]]) + xs[2];       \
	srcPixelPtr3 = (MLIB_TYPE *) (lineAddr[ys[3]]) + xs[3];       \
	xs += 4;                                                      \
	ys += 4

/* *********************************************************** */
#define	NEXT_PIXELS_2BL_S16                                        \
	srcPixelPtr0 = (MLIB_TYPE *) (lineAddr[ys[0]]) + 2 * xs[0];   \
	srcPixelPtr1 = (MLIB_TYPE *) (lineAddr[ys[1]]) + 2 * xs[1];   \
	srcPixelPtr2 = (MLIB_TYPE *) (lineAddr[ys[2]]) + 2 * xs[2];   \
	srcPixelPtr3 = (MLIB_TYPE *) (lineAddr[ys[3]]) + 2 * xs[3];   \
	xs += 4;                                                      \
	ys += 4

/* *********************************************************** */

#define	NEXT_PIXELS_1BL_S16_END(count)                      \
	srcPixelPtr0 = (MLIB_TYPE *)                          \
		(lineAddr[ys[0]]) + xs[0];                        \
	srcPixelPtr3 = srcPixelPtr0;                          \
	if ((count) == 1) {                                   \
	    srcPixelPtr2 = srcPixelPtr0;                      \
	    srcPixelPtr1 = srcPixelPtr0;                      \
	} else {                                              \
	    if ((count) == 2) {                               \
			srcPixelPtr1 = (MLIB_TYPE *)                  \
				(lineAddr[ys[1]]) + xs[1];                \
			srcPixelPtr2 = srcPixelPtr0;                  \
	    } else {                                          \
			if ((count) == 3) {                           \
				srcPixelPtr1 =                            \
					(MLIB_TYPE *)                         \
					(lineAddr[ys[1]]) + xs[1];            \
				srcPixelPtr2 =                            \
					(MLIB_TYPE *)                         \
					(lineAddr[ys[2]]) + xs[2];            \
			}                                             \
	    }                                                 \
	}

/* *********************************************************** */

#define	NEXT_PIXELS_2BL_S16_END(count)                       \
	srcPixelPtr0 = (MLIB_TYPE *)                          \
		(lineAddr[ys[0]]) + 2 * xs[0];                    \
	srcPixelPtr3 = srcPixelPtr0;                          \
	if ((count) == 1) {                                   \
	    srcPixelPtr2 = srcPixelPtr0;                      \
	    srcPixelPtr1 = srcPixelPtr0;                      \
	} else {                                              \
	    if ((count) == 2) {                               \
			srcPixelPtr1 = (MLIB_TYPE *)                  \
				(lineAddr[ys[1]]) + 2 * xs[1];            \
			srcPixelPtr2 = srcPixelPtr0;                  \
	    } else {                                          \
			if ((count) == 3) {                           \
				srcPixelPtr1 =                            \
					(MLIB_TYPE *)                         \
					(lineAddr[ys[1]]) +                   \
					2 * xs[1];                            \
				srcPixelPtr2 =                            \
					(MLIB_TYPE *)                         \
					(lineAddr[ys[2]]) +                   \
					2 * xs[2];                            \
			}                                             \
	    }                                                 \
	}


/* *********************************************************** */

#define	NEXT_PIXELS_3BL_S16                                         \
	srcPixelPtr0 = (MLIB_TYPE *) (lineAddr[ys[0]]) + 3 * xs[0];   \
	srcPixelPtr1 = (MLIB_TYPE *) (lineAddr[ys[1]]) + 3 * xs[1];   \
	xs += 2;                                                      \
	ys += 2

/* *********************************************************** */

#define	NEXT_PIXELS_4BL_S16                                         \
	srcPixelPtr0 = (MLIB_TYPE *) (lineAddr[ys[0]]) + 4 * xs[0];   \
	srcPixelPtr1 = (MLIB_TYPE *) (lineAddr[ys[1]]) + 4 * xs[1];   \
	xs += 2;                                                      \
	ys += 2

/* *********************************************************** */
#define	sse2_freg_pair(x, y)                                      \
	_mm_unpacklo_epi32(_mm_cvtsi32_si128(x), _mm_cvtsi32_si128(y))

#define	SSE2_MERGE4_U16(a, b, c, d)                               \
	sse2_freg_pair((a) | ((b) << 16), (c) | ((d) << 16))

#define	SSE2_MERGE4_S32(a, b, c, d)                               \
	_mm_setr_epi32(a, b, c, d);

/* *********************************************************** */
#define	LOAD_1BL_S16_AL1(accum, ind)                                \
	accum =                                                       \
	SSE2_MERGE4_U16(*(mlib_u16 *)((mlib_u8 *)srcPixelPtr0 + ind), \
	    *(mlib_u16 *)((mlib_u8 *)srcPixelPtr1 + ind),             \
	    *(mlib_u16 *)((mlib_u8 *)srcPixelPtr2 + ind),             \
	    *(mlib_u16 *)((mlib_u8 *)srcPixelPtr3 + ind))

/* *********************************************************** */

#define	LOAD_2BL_S16_AL1(accum, ind)                                \
	accum =                                                       \
	SSE2_MERGE4_S32(*(mlib_s32 *)((mlib_u8 *)srcPixelPtr0 + ind), \
	    *(mlib_s32 *)((mlib_u8 *)srcPixelPtr1 + ind),             \
	    *(mlib_s32 *)((mlib_u8 *)srcPixelPtr2 + ind),             \
	    *(mlib_s32 *)((mlib_u8 *)srcPixelPtr3 + ind))

/* *********************************************************** */

#define	LOAD_1BL_S16_AL                                          \
	LOAD_1BL_S16_AL1(ul, 0);                                     \
	LOAD_1BL_S16_AL1(ur, 2);                                     \
	LOAD_1BL_S16_AL1(ll, srcYStride);                            \
	LOAD_1BL_S16_AL1(lr, srcYStride1)

/* *********************************************************** */
#define	LOAD_2BL_S16_AL                                          \
	LOAD_2BL_S16_AL1(ul, 0);                                     \
	LOAD_2BL_S16_AL1(ur, 4);                                     \
	LOAD_2BL_S16_AL1(ll, srcYStride);                            \
	LOAD_2BL_S16_AL1(lr, srcYStride1)

/* *********************************************************** */

#define	LOAD_3BL_S16_AL                                            \
	ul = _mm_unpacklo_epi64(                                     \
		_mm_loadl_epi64((__m128i *)srcPixelPtr0),                \
	    _mm_loadl_epi64((__m128i *)srcPixelPtr1));               \
	ur = _mm_unpacklo_epi64(                                     \
		_mm_loadl_epi64((__m128i *)                              \
		((mlib_u8 *)srcPixelPtr0 + 6)),                          \
	    _mm_loadl_epi64((__m128i *)                              \
		((mlib_u8 *)srcPixelPtr1 + 6)));                         \
	ll = _mm_unpacklo_epi64(                                     \
		_mm_loadl_epi64((__m128i *)                              \
		((mlib_u8 *)srcPixelPtr0 + srcYStride)),                 \
	    _mm_loadl_epi64((__m128i *)                              \
		((mlib_u8 *)srcPixelPtr1 + srcYStride)));                \
	lr = _mm_unpacklo_epi64(                                     \
		_mm_loadl_epi64((__m128i *)                              \
		((mlib_u8 *)srcPixelPtr0 + srcYStride + 6)),             \
	    _mm_loadl_epi64((__m128i *)                              \
		((mlib_u8 *)srcPixelPtr1 + srcYStride + 6)))

/* *********************************************************** */

#define	LOAD_4BL_S16_AL                                            \
	ul = _mm_unpacklo_epi64(                                     \
		_mm_loadl_epi64((__m128i *)srcPixelPtr0),                \
	    _mm_loadl_epi64((__m128i *)srcPixelPtr1));               \
	ur = _mm_unpacklo_epi64(                                     \
		_mm_loadl_epi64((__m128i *)                              \
		((mlib_u8 *)srcPixelPtr0 + 8)),                          \
	    _mm_loadl_epi64((__m128i *)                              \
		((mlib_u8 *)srcPixelPtr1 + 8)));                         \
	ll = _mm_unpacklo_epi64(                                     \
		_mm_loadl_epi64((__m128i *)                              \
		((mlib_u8 *)srcPixelPtr0 + srcYStride)),                 \
	    _mm_loadl_epi64((__m128i *)                              \
		((mlib_u8 *)srcPixelPtr1 + srcYStride)));                \
	lr = _mm_unpacklo_epi64(                                     \
		_mm_loadl_epi64((__m128i *)                              \
		((mlib_u8 *)srcPixelPtr0 + srcYStride + 8)),             \
	    _mm_loadl_epi64((__m128i *)                              \
		((mlib_u8 *)srcPixelPtr1 + srcYStride + 8)))


/* *********************************************************** */

#define	RESULT_1BL_S16                                          \
	ul = _mm_mulhi_epi16(ul, delta1_x);                      \
	ur = _mm_mulhi_epi16(ur, deltax);                        \
	ul = _mm_add_epi16(ul, ur);                              \
	ul = _mm_add_epi16(ul, ul);                              \
	ul = _mm_mulhi_epi16(ul, delta1_y);                      \
	ll = _mm_mulhi_epi16(ll, delta1_x);                      \
	lr = _mm_mulhi_epi16(lr, deltax);                        \
	ll = _mm_add_epi16(ll, lr);                              \
	ll = _mm_add_epi16(ll, ll);                              \
	ll = _mm_mulhi_epi16(ll, deltay);                        \
	hi8 = _mm_add_epi16(ll, ul);                             \
	hi8 = _mm_add_epi16(hi8, hi8)

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_S16_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	DECLAREVAR;
	mlib_s32 srcYStride1 = srcYStride + 2;
	MLIB_TYPE *srcPixelPtr0, *srcPixelPtr1, *srcPixelPtr2, *srcPixelPtr3;

	__m128i mask_7fff = _mm_set1_epi32(0x7FFF7FFF);
	__m128i ul, ur, ll, lr;
	__m128i deltax, deltay, delta1_x, delta1_y;
	__m128i hi8;
	mlib_d64 *dp = (mlib_d64 *) pws->buffer;
	mlib_d64 *coeffx = (mlib_d64 *) pws->dsrc_x;
	mlib_d64 *coeffy = (mlib_d64 *) pws->dsrc_y;
	mlib_u16 *dpu, *dstData1 = (mlib_u16 *)dstData;

	deltax = _mm_loadl_epi64((__m128i *)coeffx);
	deltay = _mm_loadl_epi64((__m128i *)coeffy);
	delta1_x = _mm_sub_epi16(mask_7fff, deltax);
	delta1_y = _mm_sub_epi16(mask_7fff, deltay);
	coeffx++;
	coeffy++;

	i = 0;

	if (i <= n - 4) {
		NEXT_PIXELS_1BL_S16;

		for (; i <= n - 8; i += 4) {
			LOAD_1BL_S16_AL;
			NEXT_PIXELS_1BL_S16;
			RESULT_1BL_S16;
			deltax = _mm_loadl_epi64((__m128i *)coeffx);
			deltay = _mm_loadl_epi64((__m128i *)coeffy);
			delta1_x = _mm_sub_epi16(mask_7fff, deltax);
			delta1_y = _mm_sub_epi16(mask_7fff, deltay);
			_mm_storel_epi64((__m128i *)dp, hi8);
			coeffx++;
			coeffy++;
			dp++;
		}

		LOAD_1BL_S16_AL;
		RESULT_1BL_S16;
		deltax = _mm_loadl_epi64((__m128i *)coeffx);
		deltay = _mm_loadl_epi64((__m128i *)coeffy);
		delta1_x = _mm_sub_epi16(mask_7fff, deltax);
		delta1_y = _mm_sub_epi16(mask_7fff, deltay);
		_mm_storel_epi64((__m128i *)dp, hi8);
		dp++;
		i += 4;
	}

	if (i < n) {
		mlib_s32 tail = n - i;

		NEXT_PIXELS_1BL_S16_END(tail);
		LOAD_1BL_S16_AL;
		RESULT_1BL_S16;
		_mm_storel_epi64((__m128i *)dp, hi8);
		dp++;
	}

	dpu = (mlib_u16 *)pws->buffer;

	for (i = 0; i < n; i++) {
		dstData1[xd[i]] = dpu[i];
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_S16_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	DECLAREVAR;
	mlib_s32 srcYStride1 = srcYStride + 4;
	MLIB_TYPE *srcPixelPtr0, *srcPixelPtr1, *srcPixelPtr2, *srcPixelPtr3;

	__m128i mask_7fff = _mm_set1_epi32(0x7FFF7FFF);
	__m128i ul, ur, ll, lr;
	__m128i deltax, deltay, delta1_x, delta1_y;
	__m128i hi8;
	__m128i *dp = (__m128i *) pws->buffer;
	mlib_d64 *coeffx = (mlib_d64 *) pws->dsrc_x;
	mlib_d64 *coeffy = (mlib_d64 *) pws->dsrc_y;
	mlib_s32 *dpu, *dstData1 = (mlib_s32 *)dstData;

	deltax = _mm_loadl_epi64((__m128i *)coeffx);
	deltay = _mm_loadl_epi64((__m128i *)coeffy);
	deltax = _mm_unpacklo_epi16(deltax, deltax);
	deltay = _mm_unpacklo_epi16(deltay, deltay);
	delta1_x = _mm_sub_epi16(mask_7fff, deltax);
	delta1_y = _mm_sub_epi16(mask_7fff, deltay);
	coeffx++;
	coeffy++;

	i = 0;

	if (i <= n - 4) {
		NEXT_PIXELS_2BL_S16;

		for (; i <= n - 8; i += 4) {
			LOAD_2BL_S16_AL;
			NEXT_PIXELS_2BL_S16;
			RESULT_1BL_S16;
			deltax = _mm_loadl_epi64((__m128i *)coeffx);
			deltay = _mm_loadl_epi64((__m128i *)coeffy);
			deltax = _mm_unpacklo_epi16(deltax, deltax);
			deltay = _mm_unpacklo_epi16(deltay, deltay);
			delta1_x = _mm_sub_epi16(mask_7fff, deltax);
			delta1_y = _mm_sub_epi16(mask_7fff, deltay);
			_mm_storeu_si128(dp, hi8);
			coeffx++;
			coeffy++;
			dp++;
		}

		LOAD_2BL_S16_AL;
		RESULT_1BL_S16;
		deltax = _mm_loadl_epi64((__m128i *)coeffx);
		deltay = _mm_loadl_epi64((__m128i *)coeffy);
		deltax = _mm_unpacklo_epi16(deltax, deltax);
		deltay = _mm_unpacklo_epi16(deltay, deltay);
		delta1_x = _mm_sub_epi16(mask_7fff, deltax);
		delta1_y = _mm_sub_epi16(mask_7fff, deltay);
		_mm_storeu_si128(dp, hi8);
		dp++;
		i += 4;
	}

	if (i < n) {
		mlib_s32 tail = n - i;

		NEXT_PIXELS_2BL_S16_END(tail);
		LOAD_2BL_S16_AL;
		RESULT_1BL_S16;
		_mm_storeu_si128(dp, hi8);
		dp++;
	}

	dpu = (mlib_s32 *)pws->buffer;

	for (i = 0; i < n; i++) {
		dstData1[xd[i]] = dpu[i];
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_S16_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	DECLAREVAR;
	MLIB_TYPE *srcPixelPtr0, *srcPixelPtr1;
	__m128i mask_7fff = _mm_set1_epi32(0x7FFF7FFF);
	__m128i ul, ur, ll, lr;
	__m128i deltax, deltay, delta1_x, delta1_y;
	__m128i hi8;
	mlib_s32 ri0, ri1, xdst;

	mlib_s32 *coeffx = (mlib_s32 *)pws->dsrc_x;
	mlib_s32 *coeffy = (mlib_s32 *)pws->dsrc_y;
	mlib_u16 *dstData1 = (mlib_u16 *)dstData;

	deltax = _mm_cvtsi32_si128(coeffx[0]);
	deltay = _mm_cvtsi32_si128(coeffy[0]);
	deltax = _mm_unpacklo_epi16(deltax, deltax);
	deltay = _mm_unpacklo_epi16(deltay, deltay);
	deltax = _mm_unpacklo_epi32(deltax, deltax);
	deltay = _mm_unpacklo_epi32(deltay, deltay);
	delta1_x = _mm_sub_epi16(mask_7fff, deltax);
	delta1_y = _mm_sub_epi16(mask_7fff, deltay);
	coeffx++;
	coeffy++;

	i = 0;

	if (i <= n - 2) {
		NEXT_PIXELS_3BL_S16;

		for (; i <= n - 4; i += 2) {
			LOAD_3BL_S16_AL;
			NEXT_PIXELS_3BL_S16;
			RESULT_1BL_S16;

			deltax = _mm_cvtsi32_si128(coeffx[0]);
			deltay = _mm_cvtsi32_si128(coeffy[0]);
			deltax = _mm_unpacklo_epi16(deltax, deltax);
			deltay = _mm_unpacklo_epi16(deltay, deltay);
			deltax = _mm_unpacklo_epi32(deltax, deltax);
			deltay = _mm_unpacklo_epi32(deltay, deltay);
			delta1_x = _mm_sub_epi16(mask_7fff, deltax);
			delta1_y = _mm_sub_epi16(mask_7fff, deltay);
			coeffx++;
			coeffy++;

			xdst = xd[0] * 3;
			dstData1[xdst] = ((mlib_u16 *)&hi8)[0];
			dstData1[xdst + 1] = ((mlib_u16 *)&hi8)[1];
			dstData1[xdst + 2] = ((mlib_u16 *)&hi8)[2];
			xdst = xd[1] * 3;
			dstData1[xdst] = ((mlib_u16 *)&hi8)[4];
			dstData1[xdst + 1] = ((mlib_u16 *)&hi8)[5];
			dstData1[xdst + 2] = ((mlib_u16 *)&hi8)[6];

			xd += 2;
		}

		LOAD_3BL_S16_AL;
		RESULT_1BL_S16;
		deltax = _mm_cvtsi32_si128(coeffx[0]);
		deltay = _mm_cvtsi32_si128(coeffy[0]);
		deltax = _mm_unpacklo_epi16(deltax, deltax);
		deltay = _mm_unpacklo_epi16(deltay, deltay);
		deltax = _mm_unpacklo_epi32(deltax, deltax);
		deltay = _mm_unpacklo_epi32(deltay, deltay);
		delta1_x = _mm_sub_epi16(mask_7fff, deltax);
		delta1_y = _mm_sub_epi16(mask_7fff, deltay);
		coeffx++;
		coeffy++;

		xdst = xd[0] * 3;
		dstData1[xdst] = ((mlib_u16 *)&hi8)[0];
		dstData1[xdst + 1] = ((mlib_u16 *)&hi8)[1];
		dstData1[xdst + 2] = ((mlib_u16 *)&hi8)[2];
		xdst = xd[1] * 3;
		dstData1[xdst] = ((mlib_u16 *)&hi8)[4];
		dstData1[xdst + 1] = ((mlib_u16 *)&hi8)[5];
		dstData1[xdst + 2] = ((mlib_u16 *)&hi8)[6];

		xd += 2;
		i += 2;
	}

	if (i < n) {
		srcPixelPtr0 = (MLIB_TYPE *) (lineAddr[ys[0]]) + 3 * xs[0];
		srcPixelPtr1 = srcPixelPtr0;
		LOAD_3BL_S16_AL;
		RESULT_1BL_S16;

		xdst = xd[0] * 3;
		dstData1[xdst] = ((mlib_u16 *)&hi8)[0];
		dstData1[xdst + 1] = ((mlib_u16 *)&hi8)[1];
		dstData1[xdst + 2] = ((mlib_u16 *)&hi8)[2];
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_S16_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)

{
	DECLAREVAR;
	MLIB_TYPE *srcPixelPtr0, *srcPixelPtr1;
	__m128i mask_7fff = _mm_set1_epi32(0x7FFF7FFF);
	__m128i ul, ur, ll, lr;
	__m128i deltax, deltay, delta1_x, delta1_y;
	__m128i hi8;
	mlib_s32 ri0, ri1, xdst;

	mlib_s32 *coeffx = (mlib_s32 *)pws->dsrc_x;
	mlib_s32 *coeffy = (mlib_s32 *)pws->dsrc_y;
	mlib_u16 *dstData1 = (mlib_u16 *)dstData;

	deltax = _mm_cvtsi32_si128(coeffx[0]);
	deltay = _mm_cvtsi32_si128(coeffy[0]);
	deltax = _mm_unpacklo_epi16(deltax, deltax);
	deltay = _mm_unpacklo_epi16(deltay, deltay);
	deltax = _mm_unpacklo_epi32(deltax, deltax);
	deltay = _mm_unpacklo_epi32(deltay, deltay);
	delta1_x = _mm_sub_epi16(mask_7fff, deltax);
	delta1_y = _mm_sub_epi16(mask_7fff, deltay);
	coeffx++;
	coeffy++;

	i = 0;

	if (i <= n - 2) {
		NEXT_PIXELS_4BL_S16;

		for (; i <= n - 4; i += 2) {
			LOAD_4BL_S16_AL;
			NEXT_PIXELS_4BL_S16;
			RESULT_1BL_S16;

			deltax = _mm_cvtsi32_si128(coeffx[0]);
			deltay = _mm_cvtsi32_si128(coeffy[0]);
			deltax = _mm_unpacklo_epi16(deltax, deltax);
			deltay = _mm_unpacklo_epi16(deltay, deltay);
			deltax = _mm_unpacklo_epi32(deltax, deltax);
			deltay = _mm_unpacklo_epi32(deltay, deltay);
			delta1_x = _mm_sub_epi16(mask_7fff, deltax);
			delta1_y = _mm_sub_epi16(mask_7fff, deltay);
			coeffx++;
			coeffy++;

			_mm_storel_epi64((__m128i *)
				(dstData1 + xd[0] * 4), hi8);
			_mm_storel_epi64((__m128i *)
				(dstData1 + xd[1] * 4), _mm_srli_si128(hi8, 8));
			xd += 2;
		}

		LOAD_4BL_S16_AL;
		RESULT_1BL_S16;
		deltax = _mm_cvtsi32_si128(coeffx[0]);
		deltay = _mm_cvtsi32_si128(coeffy[0]);
		deltax = _mm_unpacklo_epi16(deltax, deltax);
		deltay = _mm_unpacklo_epi16(deltay, deltay);
		deltax = _mm_unpacklo_epi32(deltax, deltax);
		deltay = _mm_unpacklo_epi32(deltay, deltay);
		delta1_x = _mm_sub_epi16(mask_7fff, deltax);
		delta1_y = _mm_sub_epi16(mask_7fff, deltay);
		coeffx++;
		coeffy++;

		_mm_storel_epi64((__m128i *)
			(dstData1 + xd[0] * 4), hi8);
		_mm_storel_epi64((__m128i *)
			(dstData1 + xd[1] * 4), _mm_srli_si128(hi8, 8));

		xd += 2;
		i += 2;
	}

	if (i < n) {
		srcPixelPtr0 = (MLIB_TYPE *) (lineAddr[ys[0]]) + 4 * xs[0];
		srcPixelPtr1 = srcPixelPtr0;
		LOAD_4BL_S16_AL;
		RESULT_1BL_S16;

		_mm_storel_epi64((__m128i *)
			(dstData1 + xd[0] * 4), hi8);
	}
}

/* *********************************************************** */
#define	RESULT_1BL_U16                                             \
	ul = _mm_mulhi_epi16(_mm_xor_si128(ul, mask_8000), delta1_x);  \
	ur = _mm_mulhi_epi16(_mm_xor_si128(ur, mask_8000), deltax);    \
	ul = _mm_add_epi16(ul, ur);                                    \
	ul = _mm_add_epi16(ul, ul);                                    \
	ul = _mm_mulhi_epi16(ul, delta1_y);                            \
	ll = _mm_mulhi_epi16(_mm_xor_si128(ll, mask_8000), delta1_x);  \
	lr = _mm_mulhi_epi16(_mm_xor_si128(lr, mask_8000), deltax);    \
	ll = _mm_add_epi16(ll, lr);                                    \
	ll = _mm_add_epi16(ll, ll);                                    \
	ll = _mm_mulhi_epi16(ll, deltay);                              \
	hi8 = _mm_add_epi16(ll, ul);                                   \
	hi8 = _mm_add_epi16(hi8, hi8);                                 \
	hi8 = _mm_xor_si128(hi8, mask_8000)

/* *********************************************************** */
void
mlib_ImagePolynomialWarp_BL_U16_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	DECLAREVAR;
	mlib_s32 srcYStride1 = srcYStride + 2;
	MLIB_TYPE *srcPixelPtr0, *srcPixelPtr1, *srcPixelPtr2, *srcPixelPtr3;

	__m128i mask_7fff = _mm_set1_epi32(0x7FFF7FFF);
	__m128i mask_8000 = _mm_set1_epi32(0x80008000);
	__m128i ul, ur, ll, lr;
	__m128i deltax, deltay, delta1_x, delta1_y;
	__m128i hi8;
	mlib_d64 *dp = (mlib_d64 *) pws->buffer;
	mlib_d64 *coeffx = (mlib_d64 *) pws->dsrc_x;
	mlib_d64 *coeffy = (mlib_d64 *) pws->dsrc_y;
	mlib_u16 *dpu, *dstData1 = (mlib_u16 *)dstData;

	deltax = _mm_loadl_epi64((__m128i *)coeffx);
	deltay = _mm_loadl_epi64((__m128i *)coeffy);
	delta1_x = _mm_sub_epi16(mask_7fff, deltax);
	delta1_y = _mm_sub_epi16(mask_7fff, deltay);
	coeffx++;
	coeffy++;

	i = 0;

	if (i <= n - 4) {
		NEXT_PIXELS_1BL_S16;

		for (; i <= n - 8; i += 4) {
			LOAD_1BL_S16_AL;
			NEXT_PIXELS_1BL_S16;
			RESULT_1BL_U16;
			deltax = _mm_loadl_epi64((__m128i *)coeffx);
			deltay = _mm_loadl_epi64((__m128i *)coeffy);
			delta1_x = _mm_sub_epi16(mask_7fff, deltax);
			delta1_y = _mm_sub_epi16(mask_7fff, deltay);
			_mm_storel_epi64((__m128i *)dp, hi8);
			coeffx++;
			coeffy++;
			dp++;
		}

		LOAD_1BL_S16_AL;
		RESULT_1BL_U16;
		deltax = _mm_loadl_epi64((__m128i *)coeffx);
		deltay = _mm_loadl_epi64((__m128i *)coeffy);
		delta1_x = _mm_sub_epi16(mask_7fff, deltax);
		delta1_y = _mm_sub_epi16(mask_7fff, deltay);
		_mm_storel_epi64((__m128i *)dp, hi8);
		dp++;
		i += 4;
	}

	if (i < n) {
		mlib_s32 tail = n - i;

		NEXT_PIXELS_1BL_S16_END(tail);
		LOAD_1BL_S16_AL;
		RESULT_1BL_U16;
		_mm_storel_epi64((__m128i *)dp, hi8);
		dp++;
	}

	dpu = (mlib_u16 *)pws->buffer;

	for (i = 0; i < n; i++) {
		dstData1[xd[i]] = dpu[i];
	}
}
/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U16_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	DECLAREVAR;
	mlib_s32 srcYStride1 = srcYStride + 4;
	MLIB_TYPE *srcPixelPtr0, *srcPixelPtr1, *srcPixelPtr2, *srcPixelPtr3;

	__m128i mask_7fff = _mm_set1_epi32(0x7FFF7FFF);
	__m128i mask_8000 = _mm_set1_epi32(0x80008000);
	__m128i ul, ur, ll, lr;
	__m128i deltax, deltay, delta1_x, delta1_y;
	__m128i hi8;
	__m128i *dp = (__m128i *) pws->buffer;
	mlib_d64 *coeffx = (mlib_d64 *) pws->dsrc_x;
	mlib_d64 *coeffy = (mlib_d64 *) pws->dsrc_y;
	mlib_s32 *dpu, *dstData1 = (mlib_s32 *)dstData;

	deltax = _mm_loadl_epi64((__m128i *)coeffx);
	deltay = _mm_loadl_epi64((__m128i *)coeffy);
	deltax = _mm_unpacklo_epi16(deltax, deltax);
	deltay = _mm_unpacklo_epi16(deltay, deltay);
	delta1_x = _mm_sub_epi16(mask_7fff, deltax);
	delta1_y = _mm_sub_epi16(mask_7fff, deltay);
	coeffx++;
	coeffy++;

	i = 0;

	if (i <= n - 4) {
		NEXT_PIXELS_2BL_S16;

		for (; i <= n - 8; i += 4) {
			LOAD_2BL_S16_AL;
			NEXT_PIXELS_2BL_S16;
			RESULT_1BL_U16;
			deltax = _mm_loadl_epi64((__m128i *)coeffx);
			deltay = _mm_loadl_epi64((__m128i *)coeffy);
			deltax = _mm_unpacklo_epi16(deltax, deltax);
			deltay = _mm_unpacklo_epi16(deltay, deltay);
			delta1_x = _mm_sub_epi16(mask_7fff, deltax);
			delta1_y = _mm_sub_epi16(mask_7fff, deltay);
			_mm_storeu_si128(dp, hi8);
			coeffx++;
			coeffy++;
			dp++;
		}

		LOAD_2BL_S16_AL;
		RESULT_1BL_U16;
		deltax = _mm_loadl_epi64((__m128i *)coeffx);
		deltay = _mm_loadl_epi64((__m128i *)coeffy);
		deltax = _mm_unpacklo_epi16(deltax, deltax);
		deltay = _mm_unpacklo_epi16(deltay, deltay);
		delta1_x = _mm_sub_epi16(mask_7fff, deltax);
		delta1_y = _mm_sub_epi16(mask_7fff, deltay);
		_mm_storeu_si128(dp, hi8);
		dp++;
		i += 4;
	}

	if (i < n) {
		mlib_s32 tail = n - i;

		NEXT_PIXELS_2BL_S16_END(tail);
		LOAD_2BL_S16_AL;
		RESULT_1BL_U16;
		_mm_storeu_si128(dp, hi8);
		dp++;
	}

	dpu = (mlib_s32 *)pws->buffer;

	for (i = 0; i < n; i++) {
		dstData1[xd[i]] = dpu[i];
	}
}
/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U16_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	DECLAREVAR;
	MLIB_TYPE *srcPixelPtr0, *srcPixelPtr1;
	__m128i mask_7fff = _mm_set1_epi32(0x7FFF7FFF);
	__m128i mask_8000 = _mm_set1_epi32(0x80008000);
	__m128i ul, ur, ll, lr;
	__m128i deltax, deltay, delta1_x, delta1_y;
	__m128i hi8;
	mlib_s32 ri0, ri1, xdst;

	mlib_s32 *coeffx = (mlib_s32 *)pws->dsrc_x;
	mlib_s32 *coeffy = (mlib_s32 *)pws->dsrc_y;
	mlib_u16 *dstData1 = (mlib_u16 *)dstData;

	deltax = _mm_cvtsi32_si128(coeffx[0]);
	deltay = _mm_cvtsi32_si128(coeffy[0]);
	deltax = _mm_unpacklo_epi16(deltax, deltax);
	deltay = _mm_unpacklo_epi16(deltay, deltay);
	deltax = _mm_unpacklo_epi32(deltax, deltax);
	deltay = _mm_unpacklo_epi32(deltay, deltay);
	delta1_x = _mm_sub_epi16(mask_7fff, deltax);
	delta1_y = _mm_sub_epi16(mask_7fff, deltay);
	coeffx++;
	coeffy++;

	i = 0;

	if (i <= n - 2) {
		NEXT_PIXELS_3BL_S16;

		for (; i <= n - 4; i += 2) {
			LOAD_3BL_S16_AL;
			NEXT_PIXELS_3BL_S16;
			RESULT_1BL_U16;

			deltax = _mm_cvtsi32_si128(coeffx[0]);
			deltay = _mm_cvtsi32_si128(coeffy[0]);
			deltax = _mm_unpacklo_epi16(deltax, deltax);
			deltay = _mm_unpacklo_epi16(deltay, deltay);
			deltax = _mm_unpacklo_epi32(deltax, deltax);
			deltay = _mm_unpacklo_epi32(deltay, deltay);
			delta1_x = _mm_sub_epi16(mask_7fff, deltax);
			delta1_y = _mm_sub_epi16(mask_7fff, deltay);
			coeffx++;
			coeffy++;

			xdst = xd[0] * 3;
			dstData1[xdst] = ((mlib_u16 *)&hi8)[0];
			dstData1[xdst + 1] = ((mlib_u16 *)&hi8)[1];
			dstData1[xdst + 2] = ((mlib_u16 *)&hi8)[2];
			xdst = xd[1] * 3;
			dstData1[xdst] = ((mlib_u16 *)&hi8)[4];
			dstData1[xdst + 1] = ((mlib_u16 *)&hi8)[5];
			dstData1[xdst + 2] = ((mlib_u16 *)&hi8)[6];

			xd += 2;
		}

		LOAD_3BL_S16_AL;
		RESULT_1BL_U16;
		deltax = _mm_cvtsi32_si128(coeffx[0]);
		deltay = _mm_cvtsi32_si128(coeffy[0]);
		deltax = _mm_unpacklo_epi16(deltax, deltax);
		deltay = _mm_unpacklo_epi16(deltay, deltay);
		deltax = _mm_unpacklo_epi32(deltax, deltax);
		deltay = _mm_unpacklo_epi32(deltay, deltay);
		delta1_x = _mm_sub_epi16(mask_7fff, deltax);
		delta1_y = _mm_sub_epi16(mask_7fff, deltay);
		coeffx++;
		coeffy++;

		xdst = xd[0] * 3;
		dstData1[xdst] = ((mlib_u16 *)&hi8)[0];
		dstData1[xdst + 1] = ((mlib_u16 *)&hi8)[1];
		dstData1[xdst + 2] = ((mlib_u16 *)&hi8)[2];
		xdst = xd[1] * 3;
		dstData1[xdst] = ((mlib_u16 *)&hi8)[4];
		dstData1[xdst + 1] = ((mlib_u16 *)&hi8)[5];
		dstData1[xdst + 2] = ((mlib_u16 *)&hi8)[6];

		xd += 2;
		i += 2;
	}

	if (i < n) {
		srcPixelPtr0 = (MLIB_TYPE *) (lineAddr[ys[0]]) + 3 * xs[0];
		srcPixelPtr1 = srcPixelPtr0;
		LOAD_3BL_S16_AL;
		RESULT_1BL_U16;

		xdst = xd[0] * 3;
		dstData1[xdst] = ((mlib_u16 *)&hi8)[0];
		dstData1[xdst + 1] = ((mlib_u16 *)&hi8)[1];
		dstData1[xdst + 2] = ((mlib_u16 *)&hi8)[2];
	}
}
/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U16_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	DECLAREVAR;
	MLIB_TYPE *srcPixelPtr0, *srcPixelPtr1;
	__m128i mask_7fff = _mm_set1_epi32(0x7FFF7FFF);
	__m128i mask_8000 = _mm_set1_epi32(0x80008000);
	__m128i ul, ur, ll, lr;
	__m128i deltax, deltay, delta1_x, delta1_y;
	__m128i hi8;
	mlib_s32 ri0, ri1, xdst;

	mlib_s32 *coeffx = (mlib_s32 *)pws->dsrc_x;
	mlib_s32 *coeffy = (mlib_s32 *)pws->dsrc_y;
	mlib_u16 *dstData1 = (mlib_u16 *)dstData;

	deltax = _mm_cvtsi32_si128(coeffx[0]);
	deltay = _mm_cvtsi32_si128(coeffy[0]);
	deltax = _mm_unpacklo_epi16(deltax, deltax);
	deltay = _mm_unpacklo_epi16(deltay, deltay);
	deltax = _mm_unpacklo_epi32(deltax, deltax);
	deltay = _mm_unpacklo_epi32(deltay, deltay);
	delta1_x = _mm_sub_epi16(mask_7fff, deltax);
	delta1_y = _mm_sub_epi16(mask_7fff, deltay);
	coeffx++;
	coeffy++;

	i = 0;

	if (i <= n - 2) {
		NEXT_PIXELS_4BL_S16;

		for (; i <= n - 4; i += 2) {
			LOAD_4BL_S16_AL;
			NEXT_PIXELS_4BL_S16;
			RESULT_1BL_U16;

			deltax = _mm_cvtsi32_si128(coeffx[0]);
			deltay = _mm_cvtsi32_si128(coeffy[0]);
			deltax = _mm_unpacklo_epi16(deltax, deltax);
			deltay = _mm_unpacklo_epi16(deltay, deltay);
			deltax = _mm_unpacklo_epi32(deltax, deltax);
			deltay = _mm_unpacklo_epi32(deltay, deltay);
			delta1_x = _mm_sub_epi16(mask_7fff, deltax);
			delta1_y = _mm_sub_epi16(mask_7fff, deltay);
			coeffx++;
			coeffy++;

			_mm_storel_epi64((__m128i *)
				(dstData1 + xd[0] * 4), hi8);
			_mm_storel_epi64((__m128i *)
				(dstData1 + xd[1] * 4), _mm_srli_si128(hi8, 8));
			xd += 2;
		}

		LOAD_4BL_S16_AL;
		RESULT_1BL_U16;
		deltax = _mm_cvtsi32_si128(coeffx[0]);
		deltay = _mm_cvtsi32_si128(coeffy[0]);
		deltax = _mm_unpacklo_epi16(deltax, deltax);
		deltay = _mm_unpacklo_epi16(deltay, deltay);
		deltax = _mm_unpacklo_epi32(deltax, deltax);
		deltay = _mm_unpacklo_epi32(deltay, deltay);
		delta1_x = _mm_sub_epi16(mask_7fff, deltax);
		delta1_y = _mm_sub_epi16(mask_7fff, deltay);
		coeffx++;
		coeffy++;

		_mm_storel_epi64((__m128i *)
			(dstData1 + xd[0] * 4), hi8);
		_mm_storel_epi64((__m128i *)
			(dstData1 + xd[1] * 4), _mm_srli_si128(hi8, 8));

		xd += 2;
		i += 2;
	}

	if (i < n) {
		srcPixelPtr0 = (MLIB_TYPE *) (lineAddr[ys[0]]) + 4 * xs[0];
		srcPixelPtr1 = srcPixelPtr0;
		LOAD_4BL_S16_AL;
		RESULT_1BL_U16;

		_mm_storel_epi64((__m128i *)
			(dstData1 + xd[0] * 4), hi8);
	}
}
/* *********************************************************** */
