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
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_m_ImagePolynomialWarp_BL.c	9.3	07/11/05 SMI"

#define	MMX_USE_MODIF_8x16

#include <mlib_mmx_utils.h>
#include <mlib_image.h>
#include <mlib_m_ImageFilters.h>
#include <mlib_ImagePolynomialWarp.h>

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

#define	NEXT_PIXELS_1BL_U8                                        \
	srcPixelPtr0 = (MLIB_TYPE *) (lineAddr[ys[0]]) + xs[0];   \
	srcPixelPtr1 = (MLIB_TYPE *) (lineAddr[ys[1]]) + xs[1];   \
	srcPixelPtr2 = (MLIB_TYPE *) (lineAddr[ys[2]]) + xs[2];   \
	srcPixelPtr3 = (MLIB_TYPE *) (lineAddr[ys[3]]) + xs[3];   \
	xs += 4;                                                  \
	ys += 4

/* *********************************************************** */

#define	NEXT_PIXELS_1BL_U8_END(count)                                     \
	srcPixelPtr0 = (MLIB_TYPE *) (lineAddr[ys[0]]) + xs[0];           \
	srcPixelPtr3 = srcPixelPtr0;                                      \
	if ((count) == 1) {                                               \
	    srcPixelPtr2 = srcPixelPtr0;                                  \
	    srcPixelPtr1 = srcPixelPtr0;                                  \
	} else {                                                          \
	    if ((count) == 2) {                                           \
		srcPixelPtr1 = (MLIB_TYPE *) (lineAddr[ys[1]]) + xs[1];   \
		srcPixelPtr2 = srcPixelPtr0;                              \
	    } else {                                                      \
		if ((count) == 3) {                                       \
		    srcPixelPtr1 =                                        \
			(MLIB_TYPE *) (lineAddr[ys[1]]) + xs[1];          \
		    srcPixelPtr2 =                                        \
			(MLIB_TYPE *) (lineAddr[ys[2]]) + xs[2];          \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

#define	NEXT_PIXELS_2BL_U8                                      \
	srcPixelPtr0 = lineAddr[ys[0]] + 2 * xs[0];             \
	srcPixelPtr1 = lineAddr[ys[1]] + 2 * xs[1];             \
	srcPixelPtr2 = lineAddr[ys[2]] + 2 * xs[2];             \
	srcPixelPtr3 = lineAddr[ys[3]] + 2 * xs[3];             \
	xs += 4;                                                \
	ys += 4

/* *********************************************************** */

#define	NEXT_PIXELS_2BL_U8_END(count)                             \
	srcPixelPtr0 = lineAddr[ys[0]] + 2 * xs[0];               \
	srcPixelPtr3 = srcPixelPtr0;                              \
	if ((count) == 1) {                                       \
	    srcPixelPtr2 = srcPixelPtr0;                          \
	    srcPixelPtr1 = srcPixelPtr0;                          \
	} else {                                                  \
	    if ((count) == 2) {                                   \
		srcPixelPtr1 = lineAddr[ys[1]] + 2 * xs[1];       \
		srcPixelPtr2 = srcPixelPtr0;                      \
	    } else {                                              \
		if ((count) == 3) {                               \
		    srcPixelPtr1 = lineAddr[ys[1]] + 2 * xs[1];   \
		    srcPixelPtr2 = lineAddr[ys[2]] + 2 * xs[2];   \
		}                                                 \
	    }                                                     \
	}

/* *********************************************************** */

#define	LOAD_2BL_U8_AL1(accum, ind)                                    \
	accum =                                                        \
	MMX_MERGE4_U16(*(mlib_u16 *)((mlib_u8 *)srcPixelPtr0 + ind),   \
	    *(mlib_u16 *)((mlib_u8 *)srcPixelPtr1 + ind),              \
	    *(mlib_u16 *)((mlib_u8 *)srcPixelPtr2 + ind),              \
	    *(mlib_u16 *)((mlib_u8 *)srcPixelPtr3 + ind))

/* *********************************************************** */

#define	LOAD_2BL_U8_AL                                          \
	LOAD_2BL_U8_AL1(ul, 0);                                 \
	LOAD_2BL_U8_AL1(ur, 2);                                 \
	LOAD_2BL_U8_AL1(ll, srcYStride);                        \
	LOAD_2BL_U8_AL1(lr, srcYStride1)

/* *********************************************************** */

#define	mmmx_read_hi(x)	(x)
#define	mmmx_read_lo(x)	_mm_srli_si64(x, 32)

/* *********************************************************** */

#define	RESULT_1BL_U8(pos)                                        \
	pos##0 = mmx_fmul8x16hi(mmmx_read_##pos(ul), delta1_x);   \
	pos##1 = mmx_fmul8x16hi(mmmx_read_##pos(ur), deltax);     \
	pos##4 = _mm_add_pi16(pos##0, pos##1);                    \
	pos##4 = _mm_slli_pi16(pos##4, 1);                         \
	pos##6 = _mm_mulhi_pi16(pos##4, delta1_y);                \
	pos##2 = mmx_fmul8x16hi(mmmx_read_##pos(ll), delta1_x);   \
	pos##3 = mmx_fmul8x16hi(mmmx_read_##pos(lr), deltax);     \
	pos##5 = _mm_add_pi16(pos##2, pos##3);                    \
	pos##5 = _mm_slli_pi16(pos##5, 1);                         \
	pos##7 = _mm_mulhi_pi16(pos##5, deltay);                  \
	pos##8 = _mm_add_pi16(pos##6, pos##7);                    \
	pos##8 = _mm_add_pi16(pos##8, k05);                       \
	deltax = _m_from_int(coeffx[0]);                          \
	deltay = _m_from_int(coeffy[0]);                          \
	deltax = _mm_unpacklo_pi16(deltax, deltax);               \
	deltay = _mm_unpacklo_pi16(deltay, deltay);               \
	delta1_x = _mm_sub_pi16(mask_7fff, deltax);               \
	delta1_y = _mm_sub_pi16(mask_7fff, deltay);               \
	coeffx++;                                                 \
	coeffy++

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
	__m64 hi0;
	__m64 xfilter, yfilter, filter;

	for (i = 0; i < n; i++) {
		srcPixelPtr = lineAddr[*ys] + *xs;
		filterpos = (*xcoeff >> FILTER_SHIFT) & FILTER_MASK;
		xfilter =
		    *((__m64 *) ((mlib_u8 *)mlib_filters_u8_bl + filterpos));
		filterpos = (*ycoeff >> FILTER_SHIFT) & FILTER_MASK;
		yfilter =
		    *((__m64 *) ((mlib_u8 *)mlib_filters_u8_bl + filterpos +
		    STEPS * 8));
		filter = _mm_mulhi_pi16(xfilter, yfilter);
		hi0 =
		    _m_from_int(*(mlib_u16 *)srcPixelPtr | (*(mlib_u16
		    *)(srcPixelPtr + srcYStride) << 16));
		hi0 = _mm_madd_pi16(_mm_unpacklo_pi8(hi0, _m_zero), filter);
		hi0 = _mm_add_pi32(hi0, _mm_srli_si64(hi0, 32));

		dstData[xd[0]] = (_m_to_int(hi0) >> 12);
		xs++;
		ys++;
		xd++;
		xcoeff++;
		ycoeff++;
	}

	_mm_empty();
}

/* *********************************************************** */

#undef	MMX_SCALE
#define	MMX_SCALE	0

void
mlib_ImagePolynomialWarp_BL_U8_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	DECLAREVAR;
	mlib_s32 srcYStride1 = srcYStride + 2;
	const MLIB_TYPE *srcPixelPtr0, *srcPixelPtr1, *srcPixelPtr2,
	    *srcPixelPtr3;
	__m64 mask_7fff = mmx_to_double_dup(0x7FFF7FFF);
	__m64 k05 = mmx_to_double_dup(0x00200020);
	__m64 *dp = (__m64 *) pws->buffer;
	__m64 deltax, deltay, delta1_x, delta1_y;
	__m64 hi0, hi1, hi2, hi3, hi4, hi5, hi6, hi7, hi8;
	__m64 lo0, lo1, lo2, lo3, lo4, lo5, lo6, lo7, lo8;
	__m64 ul, ur, ll, lr;
	mlib_s32 *coeffx = pws->dsrc_x;
	mlib_s32 *coeffy = pws->dsrc_y;
	mlib_u16 *dpu, *dstData1 = (mlib_u16 *)dstData;

	if (n <= 0) {
		_mm_empty();
		return;
	}

	deltax = _m_from_int(coeffx[0]);
	deltay = _m_from_int(coeffy[0]);
	deltax = _mm_unpacklo_pi16(deltax, deltax);
	deltay = _mm_unpacklo_pi16(deltay, deltay);
	delta1_x = _mm_sub_pi16(mask_7fff, deltax);
	delta1_y = _mm_sub_pi16(mask_7fff, deltay);
	coeffx++;
	coeffy++;

	i = 0;

	if (n >= 4) {

		NEXT_PIXELS_2BL_U8;

		for (; i <= n - 8; i += 4) {
			LOAD_2BL_U8_AL;
			NEXT_PIXELS_2BL_U8;
			RESULT_1BL_U8(hi);
			RESULT_1BL_U8(lo);
			(*dp++) = mmx_fpack16_pair(hi8, lo8);
		}

		LOAD_2BL_U8_AL;
		RESULT_1BL_U8(hi);
		RESULT_1BL_U8(lo);
		(*dp++) = mmx_fpack16_pair(hi8, lo8);
		i += 4;
	}

	if (i < n) {
		mlib_s32 tail = n - i;

		NEXT_PIXELS_2BL_U8_END(tail);
		LOAD_2BL_U8_AL;
		RESULT_1BL_U8(hi);
		RESULT_1BL_U8(lo);
		(*dp++) = mmx_fpack16_pair(hi8, lo8);
	}

	dpu = (mlib_u16 *)pws->buffer;

	for (i = 0; i < n; i++) {
		dstData1[xd[i]] = dpu[i];
	}

	_mm_empty();
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
	__m64 filter01, filter23, filter0, filter1, filter2, filter3;
	__m64 k05 = mmx_to_double_dup(0x00040004);
	__m64 _m_mzero[1] = { 0 };
	__m64 filter, xfilter, yfilter;
	__m64 sm0;
	__m64 ulprod, urprod, llprod, lrprod;
	mlib_s32 rr, xdst;
	mlib_s32 hi_top, lo_top, hi_bot, lo_bot;

	for (i = 0; i < n; i++) {
		srcPixelPtr = lineAddr[ys[0]] + 3 * xs[0];

		filterpos = (*xcoeff >> FILTER_SHIFT) & FILTER_MASK;
		xfilter =
		    *((__m64 *) ((mlib_u8 *)mlib_filters_u8_bl + filterpos));
		filterpos = (*ycoeff >> FILTER_SHIFT) & FILTER_MASK;
		yfilter =
		    *((__m64 *) ((mlib_u8 *)mlib_filters_u8_bl + filterpos +
		    STEPS * 8));

		filter = _mm_mulhi_pi16(xfilter, yfilter);
		filter01 = _mm_unpacklo_pi16(filter, filter);
		filter23 = _mm_unpackhi_pi16(filter, filter);
		filter0 = _mm_unpacklo_pi16(filter01, filter01);
		filter1 = _mm_unpackhi_pi16(filter01, filter01);
		filter2 = _mm_unpacklo_pi16(filter23, filter23);
		filter3 = _mm_unpackhi_pi16(filter23, filter23);

		hi_top = *(mlib_s32 *)srcPixelPtr;
		lo_top = *(mlib_s32 *)(srcPixelPtr + 3);
		hi_bot = *(mlib_s32 *)(srcPixelPtr + srcYStride);
		lo_bot = *(mlib_s32 *)(srcPixelPtr + srcYStride + 3);

		ulprod = _mm_mulhi_pi16(_mm_slli_pi16(_mm_unpacklo_pi8
		    (_m_from_int(hi_top), _m_mzero[0]), 7), filter0);
		urprod = _mm_mulhi_pi16(_mm_slli_pi16(_mm_unpacklo_pi8
		    (_m_from_int(lo_top), _m_mzero[0]), 7), filter1);
		llprod = _mm_mulhi_pi16(_mm_slli_pi16(_mm_unpacklo_pi8
		    (_m_from_int(hi_bot), _m_mzero[0]), 7), filter2);
		lrprod = _mm_mulhi_pi16(_mm_slli_pi16(_mm_unpacklo_pi8
		    (_m_from_int(lo_bot), _m_mzero[0]), 7), filter3);

		sm0 = _mm_add_pi16(ulprod, urprod);
		sm0 = _mm_add_pi16(sm0, llprod);
		sm0 = _mm_add_pi16(sm0, lrprod);
		sm0 = _mm_add_pi16(sm0, k05);

		rr = mmx_fpack16(sm0);

		xdst = xd[i] * 3;
		dstData[xdst] = rr;
		dstData[xdst + 1] = (rr >> 8);
		dstData[xdst + 2] = (rr >> 16);

		xs++;
		ys++;
		xcoeff++;
		ycoeff++;
	}

	_mm_empty();
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
	__m64 filter01, filter23, filter0, filter1, filter2, filter3;
	__m64 k05 = mmx_to_double_dup(0x00040004);
	__m64 _m_mzero[1] = { 0 };
	__m64 filter, xfilter, yfilter;
	__m64 sm0;
	__m64 ulprod, urprod, llprod, lrprod;
	mlib_s32 hi_top, lo_top, hi_bot, lo_bot;
	mlib_s32 *dstData1 = (mlib_s32 *)dstData;

	for (i = 0; i < n; i++) {
		srcPixelPtr = lineAddr[ys[0]] + 4 * xs[0];

		filterpos = (*xcoeff >> FILTER_SHIFT) & FILTER_MASK;
		xfilter =
		    *((__m64 *) ((mlib_u8 *)mlib_filters_u8_bl + filterpos));
		filterpos = (*ycoeff >> FILTER_SHIFT) & FILTER_MASK;
		yfilter =
		    *((__m64 *) ((mlib_u8 *)mlib_filters_u8_bl + filterpos +
		    STEPS * 8));

		filter = _mm_mulhi_pi16(xfilter, yfilter);
		filter01 = _mm_unpacklo_pi16(filter, filter);
		filter23 = _mm_unpackhi_pi16(filter, filter);
		filter0 = _mm_unpacklo_pi16(filter01, filter01);
		filter1 = _mm_unpackhi_pi16(filter01, filter01);
		filter2 = _mm_unpacklo_pi16(filter23, filter23);
		filter3 = _mm_unpackhi_pi16(filter23, filter23);

		hi_top = *(mlib_s32 *)srcPixelPtr;
		lo_top = *(mlib_s32 *)(srcPixelPtr + 4);
		hi_bot = *(mlib_s32 *)(srcPixelPtr + srcYStride);
		lo_bot = *(mlib_s32 *)(srcPixelPtr + srcYStride + 4);

		ulprod = _mm_mulhi_pi16(_mm_slli_pi16(_mm_unpacklo_pi8
		    (_m_from_int(hi_top), _m_mzero[0]), 7), filter0);
		urprod = _mm_mulhi_pi16(_mm_slli_pi16(_mm_unpacklo_pi8
		    (_m_from_int(lo_top), _m_mzero[0]), 7), filter1);
		llprod = _mm_mulhi_pi16(_mm_slli_pi16(_mm_unpacklo_pi8
		    (_m_from_int(hi_bot), _m_mzero[0]), 7), filter2);
		lrprod = _mm_mulhi_pi16(_mm_slli_pi16(_mm_unpacklo_pi8
		    (_m_from_int(lo_bot), _m_mzero[0]), 7), filter3);

		sm0 = _mm_add_pi16(ulprod, urprod);
		sm0 = _mm_add_pi16(sm0, llprod);
		sm0 = _mm_add_pi16(sm0, lrprod);
		sm0 = _mm_add_pi16(sm0, k05);

		dstData1[xd[i]] = mmx_fpack16(sm0);

		xs++;
		ys++;
		xcoeff++;
		ycoeff++;
	}

	_mm_empty();
}

/* *********************************************************** */

#undef	MLIB_TYPE
#define	MLIB_TYPE	mlib_s16

/* *********************************************************** */

#define	NEXT_PIXELS_2BL_S16                                           \
	srcPixelPtr0 = (MLIB_TYPE *) (lineAddr[ys[0]]) + 2 * xs[0];   \
	srcPixelPtr1 = (MLIB_TYPE *) (lineAddr[ys[1]]) + 2 * xs[1];   \
	xs += 2;                                                      \
	ys += 2

/* *********************************************************** */

#define	NEXT_PIXELS_3BL_S16                                          \
	srcPixelPtr = (MLIB_TYPE *) (lineAddr[ys[0]] + 6 * xs[0]);   \
	xs++;                                                        \
	ys++

/* *********************************************************** */

#define	NEXT_PIXELS_4BL_S16                                          \
	srcPixelPtr = (MLIB_TYPE *) (lineAddr[ys[0]] + 8 * xs[0]);   \
	xs++;                                                        \
	ys++

/* *********************************************************** */

#define	LOAD_2BL_S16_AL                                                \
	ul =                                                           \
	mmx_freg_pair(*(mlib_s32 *)srcPixelPtr0,                       \
	    *(mlib_s32 *)srcPixelPtr1);                                \
	ur =                                                           \
	mmx_freg_pair(*(mlib_s32 *)((mlib_u8 *)srcPixelPtr0 + 4),      \
	    *(mlib_s32 *)((mlib_u8 *)srcPixelPtr1 + 4));               \
	ll =                                                           \
	mmx_freg_pair(*(mlib_s32 *)((mlib_u8 *)srcPixelPtr0 +          \
	    srcYStride),                                               \
	    *(mlib_s32 *)((mlib_u8 *)srcPixelPtr1 + srcYStride));      \
	lr =                                                           \
	mmx_freg_pair(*(mlib_s32 *)((mlib_u8 *)srcPixelPtr0 +          \
	    srcYStride + 4),                                           \
	    *(mlib_s32 *)((mlib_u8 *)srcPixelPtr1 + srcYStride + 4))

/* *********************************************************** */

#define	LOAD_3BL_S16                                                \
	ul = *(__m64 *) srcPixelPtr;                                \
	ur = *(__m64 *) ((mlib_u8 *)srcPixelPtr + 6);               \
	ll = *(__m64 *) ((mlib_u8 *)srcPixelPtr + srcYStride);      \
	lr = *(__m64 *) ((mlib_u8 *)srcPixelPtr + srcYStride + 6)

/* *********************************************************** */

#define	LOAD_4BL_S16                                                \
	ul = *(__m64 *) srcPixelPtr;                                \
	ur = *(__m64 *) ((mlib_u8 *)srcPixelPtr + 8);               \
	ll = *(__m64 *) ((mlib_u8 *)srcPixelPtr + srcYStride);      \
	lr = *(__m64 *) ((mlib_u8 *)srcPixelPtr + srcYStride + 8)

/* *********************************************************** */

#define	RESULT_1BL_S16                                          \
	ul = _mm_mulhi_pi16(ul, delta1_x);                      \
	ur = _mm_mulhi_pi16(ur, deltax);                        \
	ul = _mm_add_pi16(ul, ur);                              \
	ul = _mm_add_pi16(ul, ul);                              \
	ul = _mm_mulhi_pi16(ul, delta1_y);                      \
	ll = _mm_mulhi_pi16(ll, delta1_x);                      \
	lr = _mm_mulhi_pi16(lr, deltax);                        \
	ll = _mm_add_pi16(ll, lr);                              \
	ll = _mm_add_pi16(ll, ll);                              \
	ll = _mm_mulhi_pi16(ll, deltay);                        \
	hi8 = _mm_add_pi16(ll, ul);                             \
	hi8 = _mm_add_pi16(hi8, hi8)

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
	__m64 mask_7fff = mmx_to_double_dup(0x7FFF7FFF);
	__m64 *dp = (__m64 *) pws->buffer;
	__m64 ul, ur, ll, lr;
	__m64 deltax, deltay, delta1_x, delta1_y;
	__m64 hi8;
	__m64 *coeffx = (__m64 *) pws->dsrc_x;
	__m64 *coeffy = (__m64 *) pws->dsrc_y;
	mlib_u16 *dpu, *dstData1 = (mlib_u16 *)dstData;

	deltax = coeffx[0];
	deltay = coeffy[0];
	delta1_x = _mm_sub_pi16(mask_7fff, deltax);
	delta1_y = _mm_sub_pi16(mask_7fff, deltay);
	coeffx++;
	coeffy++;

	i = 0;

	if (i <= n - 4) {
		NEXT_PIXELS_1BL_U8;

		for (; i <= n - 8; i += 4) {
			LOAD_2BL_U8_AL;
			NEXT_PIXELS_1BL_U8;
			RESULT_1BL_S16;
			deltax = coeffx[0];
			deltay = coeffy[0];
			delta1_x = _mm_sub_pi16(mask_7fff, deltax);
			delta1_y = _mm_sub_pi16(mask_7fff, deltay);
			coeffx++;
			coeffy++;
			(*dp++) = hi8;
		}

		LOAD_2BL_U8_AL;
		RESULT_1BL_S16;
		deltax = coeffx[0];
		deltay = coeffy[0];
		delta1_x = _mm_sub_pi16(mask_7fff, deltax);
		delta1_y = _mm_sub_pi16(mask_7fff, deltay);
		(*dp++) = hi8;
		i += 4;
	}

	if (i < n) {
		mlib_s32 tail = n - i;

		NEXT_PIXELS_1BL_U8_END(tail);
		LOAD_2BL_U8_AL;
		RESULT_1BL_S16;
		(*dp++) = hi8;
	}

	dpu = (mlib_u16 *)pws->buffer;

	for (i = 0; i < n; i++) {
		dstData1[xd[i]] = dpu[i];
	}

	_mm_empty();
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
	MLIB_TYPE *srcPixelPtr0, *srcPixelPtr1;
	__m64 mask_7fff = mmx_to_double_dup(0x7FFF7FFF);
	__m64 ul, ur, ll, lr;
	__m64 deltax, deltay, delta1_x, delta1_y;
	__m64 hi8;
	mlib_s32 *coeffx = (mlib_s32 *)pws->dsrc_x;
	mlib_s32 *coeffy = (mlib_s32 *)pws->dsrc_y;
	mlib_s32 *dstData1 = (mlib_s32 *)dstData;

	deltax = _m_from_int(coeffx[0]);
	deltay = _m_from_int(coeffy[0]);
	deltax = _mm_unpacklo_pi16(deltax, deltax);
	deltay = _mm_unpacklo_pi16(deltay, deltay);
	delta1_x = _mm_sub_pi16(mask_7fff, deltax);
	delta1_y = _mm_sub_pi16(mask_7fff, deltay);
	coeffx++;
	coeffy++;

	i = 0;

	if (i <= n - 2) {
		NEXT_PIXELS_2BL_S16;

		for (; i <= n - 4; i += 2) {
			LOAD_2BL_S16_AL;
			NEXT_PIXELS_2BL_S16;
			RESULT_1BL_S16;
			deltax = _m_from_int(coeffx[0]);
			deltay = _m_from_int(coeffy[0]);
			deltax = _mm_unpacklo_pi16(deltax, deltax);
			deltay = _mm_unpacklo_pi16(deltay, deltay);
			delta1_x = _mm_sub_pi16(mask_7fff, deltax);
			delta1_y = _mm_sub_pi16(mask_7fff, deltay);
			coeffx++;
			coeffy++;
			dstData1[xd[0]] = mmx_read_hi(hi8);
			dstData1[xd[1]] = mmx_read_lo(hi8);
			xd += 2;
		}

		LOAD_2BL_S16_AL;
		RESULT_1BL_S16;
		deltax = _m_from_int(coeffx[0]);
		deltay = _m_from_int(coeffy[0]);
		deltax = _mm_unpacklo_pi16(deltax, deltax);
		deltay = _mm_unpacklo_pi16(deltay, deltay);
		delta1_x = _mm_sub_pi16(mask_7fff, deltax);
		delta1_y = _mm_sub_pi16(mask_7fff, deltay);
		coeffx++;
		coeffy++;
		dstData1[xd[0]] = mmx_read_hi(hi8);
		dstData1[xd[1]] = mmx_read_lo(hi8);
		xd += 2;
		i += 2;
	}

	if (i < n) {
		srcPixelPtr0 = (MLIB_TYPE *) (lineAddr[ys[0]]) + 2 * xs[0];
		srcPixelPtr1 = srcPixelPtr0;
		LOAD_2BL_S16_AL;
		RESULT_1BL_S16;
		dstData1[xd[0]] = mmx_read_hi(hi8);
	}

	_mm_empty();
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
	MLIB_TYPE *srcPixelPtr;
	__m64 mask_7fff = mmx_to_double_dup(0x7FFF7FFF);
	__m64 ul, ur, ll, lr;
	__m64 deltax, deltay, delta1_x, delta1_y;
	__m64 hi8;
	mlib_s32 xdst, rr, xSrc;
	mlib_u16 *coeffx = (mlib_u16 *)pws->dsrc_x;
	mlib_u16 *coeffy = (mlib_u16 *)pws->dsrc_y;
	mlib_u16 *dstData1 = (mlib_u16 *)dstData;

	if (n > 0) {
		deltax = _m_from_int(coeffx[0]);
		deltay = _m_from_int(coeffy[0]);
		deltax = _mm_unpacklo_pi16(deltax, deltax);
		deltay = _mm_unpacklo_pi16(deltay, deltay);
		deltax = _mm_unpacklo_pi16(deltax, deltax);
		deltay = _mm_unpacklo_pi16(deltay, deltay);
		delta1_x = _mm_sub_pi16(mask_7fff, deltax);
		delta1_y = _mm_sub_pi16(mask_7fff, deltay);
		coeffx++;
		coeffy++;

		NEXT_PIXELS_3BL_S16;

		for (i = 0; i < n - 1; i++) {
			LOAD_3BL_S16;
			NEXT_PIXELS_3BL_S16;
			RESULT_1BL_S16;
			deltax = _m_from_int(coeffx[0]);
			deltay = _m_from_int(coeffy[0]);
			deltax = _mm_unpacklo_pi16(deltax, deltax);
			deltay = _mm_unpacklo_pi16(deltay, deltay);
			deltax = _mm_unpacklo_pi16(deltax, deltax);
			deltay = _mm_unpacklo_pi16(deltay, deltay);
			delta1_x = _mm_sub_pi16(mask_7fff, deltax);
			delta1_y = _mm_sub_pi16(mask_7fff, deltay);
			coeffx++;
			coeffy++;
			rr = mmx_read_hi(hi8);
			xdst = xd[i] * 3;
			dstData1[xdst] = rr;
			dstData1[xdst + 1] = (rr >> 16);
			dstData1[xdst + 2] = mmx_read_lo(hi8);
		}

		LOAD_3BL_S16;
		RESULT_1BL_S16;
		rr = mmx_read_hi(hi8);
		xdst = xd[i] * 3;
		dstData1[xdst] = rr;
		dstData1[xdst + 1] = (rr >> 16);
		dstData1[xdst + 2] = mmx_read_lo(hi8);
	}

	_mm_empty();
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
	MLIB_TYPE *srcPixelPtr;
	__m64 mask_7fff = mmx_to_double_dup(0x7FFF7FFF);
	__m64 ul, ur, ll, lr;
	__m64 deltax, deltay, delta1_x, delta1_y;
	__m64 hi8;
	mlib_u16 *coeffx = (mlib_u16 *)pws->dsrc_x;
	mlib_u16 *coeffy = (mlib_u16 *)pws->dsrc_y;
	__m64 *dstData1 = (__m64 *) dstData;

	if (n > 0) {

		deltax = _m_from_int(coeffx[0]);
		deltay = _m_from_int(coeffy[0]);
		deltax = _mm_unpacklo_pi16(deltax, deltax);
		deltay = _mm_unpacklo_pi16(deltay, deltay);
		deltax = _mm_unpacklo_pi16(deltax, deltax);
		deltay = _mm_unpacklo_pi16(deltay, deltay);
		delta1_x = _mm_sub_pi16(mask_7fff, deltax);
		delta1_y = _mm_sub_pi16(mask_7fff, deltay);
		coeffx++;
		coeffy++;

		NEXT_PIXELS_4BL_S16;

		for (i = 0; i < n - 1; i++) {
			LOAD_4BL_S16;
			NEXT_PIXELS_4BL_S16;
			RESULT_1BL_S16;
			deltax = _m_from_int(coeffx[0]);
			deltay = _m_from_int(coeffy[0]);
			deltax = _mm_unpacklo_pi16(deltax, deltax);
			deltay = _mm_unpacklo_pi16(deltay, deltay);
			deltax = _mm_unpacklo_pi16(deltax, deltax);
			deltay = _mm_unpacklo_pi16(deltay, deltay);
			delta1_x = _mm_sub_pi16(mask_7fff, deltax);
			delta1_y = _mm_sub_pi16(mask_7fff, deltay);
			coeffx++;
			coeffy++;
			dstData1[xd[i]] = hi8;
		}

		LOAD_4BL_S16;
		RESULT_1BL_S16;
		dstData1[xd[i]] = hi8;
	}

	_mm_empty();
}

/* *********************************************************** */

#define	RESULT_1BL_U16                                                \
	ul = _mm_mulhi_pi16(_mm_xor_si64(ul, mask_8000), delta1_x);   \
	ur = _mm_mulhi_pi16(_mm_xor_si64(ur, mask_8000), deltax);     \
	ul = _mm_add_pi16(ul, ur);                                    \
	ul = _mm_add_pi16(ul, ul);                                    \
	ul = _mm_mulhi_pi16(ul, delta1_y);                            \
	ll = _mm_mulhi_pi16(_mm_xor_si64(ll, mask_8000), delta1_x);   \
	lr = _mm_mulhi_pi16(_mm_xor_si64(lr, mask_8000), deltax);     \
	ll = _mm_add_pi16(ll, lr);                                    \
	ll = _mm_add_pi16(ll, ll);                                    \
	ll = _mm_mulhi_pi16(ll, deltay);                              \
	hi8 = _mm_add_pi16(ll, ul);                                   \
	hi8 = _mm_add_pi16(hi8, hi8);                                 \
	hi8 = _mm_xor_si64(hi8, mask_8000)

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
	__m64 mask_7fff = mmx_to_double_dup(0x7FFF7FFF);
	__m64 mask_8000 = mmx_to_double_dup(0x80008000);
	__m64 *dp = (__m64 *) pws->buffer;
	__m64 ul, ur, ll, lr;
	__m64 deltax, deltay, delta1_x, delta1_y;
	__m64 hi8;
	__m64 *coeffx = (__m64 *) pws->dsrc_x;
	__m64 *coeffy = (__m64 *) pws->dsrc_y;
	mlib_u16 *dpu, *dstData1 = (mlib_u16 *)dstData;

	deltax = coeffx[0];
	deltay = coeffy[0];
	delta1_x = _mm_sub_pi16(mask_7fff, deltax);
	delta1_y = _mm_sub_pi16(mask_7fff, deltay);
	coeffx++;
	coeffy++;

	i = 0;

	if (i <= n - 4) {
		NEXT_PIXELS_1BL_U8;

		for (; i <= n - 8; i += 4) {
			LOAD_2BL_U8_AL;
			NEXT_PIXELS_1BL_U8;
			RESULT_1BL_U16;
			deltax = coeffx[0];
			deltay = coeffy[0];
			delta1_x = _mm_sub_pi16(mask_7fff, deltax);
			delta1_y = _mm_sub_pi16(mask_7fff, deltay);
			coeffx++;
			coeffy++;
			(*dp++) = hi8;
		}

		LOAD_2BL_U8_AL;
		RESULT_1BL_U16;
		deltax = coeffx[0];
		deltay = coeffy[0];
		delta1_x = _mm_sub_pi16(mask_7fff, deltax);
		delta1_y = _mm_sub_pi16(mask_7fff, deltay);
		(*dp++) = hi8;
		i += 4;
	}

	if (i < n) {
		mlib_s32 tail = n - i;

		NEXT_PIXELS_1BL_U8_END(tail)
		    LOAD_2BL_U8_AL;
		RESULT_1BL_U16;
		(*dp++) = hi8;
	}

	dpu = (mlib_u16 *)pws->buffer;

	for (i = 0; i < n; i++) {
		dstData1[xd[i]] = dpu[i];
	}

	_mm_empty();
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
	MLIB_TYPE *srcPixelPtr0, *srcPixelPtr1;
	__m64 mask_7fff = mmx_to_double_dup(0x7FFF7FFF);
	__m64 mask_8000 = mmx_to_double_dup(0x80008000);
	__m64 ul, ur, ll, lr;
	__m64 deltax, deltay, delta1_x, delta1_y;
	__m64 hi8;
	mlib_s32 *coeffx = (mlib_s32 *)pws->dsrc_x;
	mlib_s32 *coeffy = (mlib_s32 *)pws->dsrc_y;
	mlib_s32 *dstData1 = (mlib_s32 *)dstData;

	deltax = _m_from_int(coeffx[0]);
	deltay = _m_from_int(coeffy[0]);
	deltax = _mm_unpacklo_pi16(deltax, deltax);
	deltay = _mm_unpacklo_pi16(deltay, deltay);
	delta1_x = _mm_sub_pi16(mask_7fff, deltax);
	delta1_y = _mm_sub_pi16(mask_7fff, deltay);
	coeffx++;
	coeffy++;

	i = 0;

	if (i <= n - 2) {
		NEXT_PIXELS_2BL_S16;

		for (; i <= n - 4; i += 2) {
			LOAD_2BL_S16_AL;
			NEXT_PIXELS_2BL_S16;
			RESULT_1BL_U16;
			deltax = _m_from_int(coeffx[0]);
			deltay = _m_from_int(coeffy[0]);
			deltax = _mm_unpacklo_pi16(deltax, deltax);
			deltay = _mm_unpacklo_pi16(deltay, deltay);
			delta1_x = _mm_sub_pi16(mask_7fff, deltax);
			delta1_y = _mm_sub_pi16(mask_7fff, deltay);
			coeffx++;
			coeffy++;
			dstData1[xd[0]] = mmx_read_hi(hi8);
			dstData1[xd[1]] = mmx_read_lo(hi8);
			xd += 2;
		}

		LOAD_2BL_S16_AL;
		RESULT_1BL_U16;
		deltax = _m_from_int(coeffx[0]);
		deltay = _m_from_int(coeffy[0]);
		deltax = _mm_unpacklo_pi16(deltax, deltax);
		deltay = _mm_unpacklo_pi16(deltay, deltay);
		delta1_x = _mm_sub_pi16(mask_7fff, deltax);
		delta1_y = _mm_sub_pi16(mask_7fff, deltay);
		coeffx++;
		coeffy++;
		dstData1[xd[0]] = mmx_read_hi(hi8);
		dstData1[xd[1]] = mmx_read_lo(hi8);
		xd += 2;
		i += 2;
	}

	if (i < n) {
		srcPixelPtr0 = (MLIB_TYPE *) (lineAddr[ys[0]]) + 2 * xs[0];
		srcPixelPtr1 = srcPixelPtr0;
		LOAD_2BL_S16_AL;
		RESULT_1BL_U16;
		dstData1[xd[0]] = mmx_read_hi(hi8);
	}

	_mm_empty();
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
	MLIB_TYPE *srcPixelPtr;
	__m64 mask_7fff = mmx_to_double_dup(0x7FFF7FFF);
	__m64 mask_8000 = mmx_to_double_dup(0x80008000);
	__m64 ul, ur, ll, lr;
	__m64 deltax, deltay, delta1_x, delta1_y;
	__m64 hi8;
	mlib_s32 xdst, rr, xSrc;
	mlib_u16 *coeffx = (mlib_u16 *)pws->dsrc_x;
	mlib_u16 *coeffy = (mlib_u16 *)pws->dsrc_y;
	mlib_u16 *dstData1 = (mlib_u16 *)dstData;

	if (n > 0) {
		deltax = _m_from_int(coeffx[0]);
		deltay = _m_from_int(coeffy[0]);
		deltax = _mm_unpacklo_pi16(deltax, deltax);
		deltay = _mm_unpacklo_pi16(deltay, deltay);
		deltax = _mm_unpacklo_pi16(deltax, deltax);
		deltay = _mm_unpacklo_pi16(deltay, deltay);
		delta1_x = _mm_sub_pi16(mask_7fff, deltax);
		delta1_y = _mm_sub_pi16(mask_7fff, deltay);
		coeffx++;
		coeffy++;

		NEXT_PIXELS_3BL_S16;

		for (i = 0; i < n - 1; i++) {
			LOAD_3BL_S16;
			NEXT_PIXELS_3BL_S16;
			RESULT_1BL_U16;
			deltax = _m_from_int(coeffx[0]);
			deltay = _m_from_int(coeffy[0]);
			deltax = _mm_unpacklo_pi16(deltax, deltax);
			deltay = _mm_unpacklo_pi16(deltay, deltay);
			deltax = _mm_unpacklo_pi16(deltax, deltax);
			deltay = _mm_unpacklo_pi16(deltay, deltay);
			delta1_x = _mm_sub_pi16(mask_7fff, deltax);
			delta1_y = _mm_sub_pi16(mask_7fff, deltay);
			coeffx++;
			coeffy++;
			rr = mmx_read_hi(hi8);
			xdst = xd[i] * 3;
			dstData1[xdst] = rr;
			dstData1[xdst + 1] = (rr >> 16);
			dstData1[xdst + 2] = mmx_read_lo(hi8);
		}

		LOAD_3BL_S16;
		RESULT_1BL_U16;
		rr = mmx_read_hi(hi8);
		xdst = xd[i] * 3;
		dstData1[xdst] = rr;
		dstData1[xdst + 1] = (rr >> 16);
		dstData1[xdst + 2] = mmx_read_lo(hi8);
	}

	_mm_empty();
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
	MLIB_TYPE *srcPixelPtr;
	__m64 mask_7fff = mmx_to_double_dup(0x7FFF7FFF);
	__m64 mask_8000 = mmx_to_double_dup(0x80008000);
	__m64 ul, ur, ll, lr;
	__m64 deltax, deltay, delta1_x, delta1_y;
	__m64 hi8;
	mlib_u16 *coeffx = (mlib_u16 *)pws->dsrc_x;
	mlib_u16 *coeffy = (mlib_u16 *)pws->dsrc_y;
	__m64 *dstData1 = (__m64 *) dstData;

	if (n > 0) {

		deltax = _m_from_int(coeffx[0]);
		deltay = _m_from_int(coeffy[0]);
		deltax = _mm_unpacklo_pi16(deltax, deltax);
		deltay = _mm_unpacklo_pi16(deltay, deltay);
		deltax = _mm_unpacklo_pi16(deltax, deltax);
		deltay = _mm_unpacklo_pi16(deltay, deltay);
		delta1_x = _mm_sub_pi16(mask_7fff, deltax);
		delta1_y = _mm_sub_pi16(mask_7fff, deltay);
		coeffx++;
		coeffy++;

		NEXT_PIXELS_4BL_S16;

		for (i = 0; i < n - 1; i++) {
			LOAD_4BL_S16;
			NEXT_PIXELS_4BL_S16;
			RESULT_1BL_U16;
			deltax = _m_from_int(coeffx[0]);
			deltay = _m_from_int(coeffy[0]);
			deltax = _mm_unpacklo_pi16(deltax, deltax);
			deltay = _mm_unpacklo_pi16(deltay, deltay);
			deltax = _mm_unpacklo_pi16(deltax, deltax);
			deltay = _mm_unpacklo_pi16(deltay, deltay);
			delta1_x = _mm_sub_pi16(mask_7fff, deltax);
			delta1_y = _mm_sub_pi16(mask_7fff, deltay);
			coeffx++;
			coeffy++;
			dstData1[xd[i]] = hi8;
		}

		LOAD_4BL_S16;
		RESULT_1BL_U16;
		dstData1[xd[i]] = hi8;
	}

	_mm_empty();
}

/* *********************************************************** */
