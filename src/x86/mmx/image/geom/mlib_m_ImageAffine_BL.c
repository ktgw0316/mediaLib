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

#pragma ident	"@(#)mlib_m_ImageAffine_BL.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageAffine_u8_1ch_bl
 *      mlib_ImageAffine_u8_2ch_bl
 *      mlib_ImageAffine_u8_3ch_bl
 *      mlib_ImageAffine_u8_4ch_bl
 *      mlib_ImageAffine_s16_1ch_bl
 *      mlib_ImageAffine_s16_2ch_bl
 *      mlib_ImageAffine_s16_3ch_bl
 *      mlib_ImageAffine_s16_4ch_bl
 *      mlib_ImageAffine_s32_1ch_bl
 *      mlib_ImageAffine_s32_2ch_bl
 *      mlib_ImageAffine_s32_3ch_bl
 *      mlib_ImageAffine_s32_4ch_bl
 *
 * DESCRIPTION
 *      The functions step along the lines from xLeft to xRight and apply
 *      the bilinear filtering.
 *
 */

#define	MMX_USE_MODIF_8x16

#include <mlib_mmx_utils.h>
#include <mlib_m_ImageFilters.h>
#include <mlib_ImageAffine.h>

/* *********************************************************** */

#define	FILTER_BITS	8

#define	LGSTEPS	8
#define	STEPS	(1 << LGSTEPS)

#undef  MLIB_POINTER_SHIFT

#ifdef _LP64
#define	MLIB_POINTER_SHIFT(P)   (((P) >> (MLIB_SHIFT - 3)) &~ 7)
#else
#define	MLIB_POINTER_SHIFT(P)	(P >> (MLIB_SHIFT-2)) &~ 3
#endif /* _LP64 */

#undef  MLIB_POINTER_GET
#define	MLIB_POINTER_GET(A, P)	(*(MLIB_TYPE **)((mlib_u8 *)A + P))

/* ******************************************************************* */

#define	DTYPE	MLIB_TYPE
#undef MLIB_TYPE
#define	MLIB_TYPE	mlib_u8

/* ******************************************************************* */
#define	NEXT_PIXELS_1BL_U8(ind)                                  \
	filterpos = MLIB_POINTER_SHIFT(Y);                       \
	srcPixelPtr0##ind =                                      \
	    *(MLIB_TYPE **) ((mlib_u8 *)lineAddr + filterpos);   \
	srcPixelPtr0##ind += X >> MLIB_SHIFT;                    \
	X += dX;                                                 \
	Y += dY;                                                 \
	filterpos = MLIB_POINTER_SHIFT(Y);                       \
	srcPixelPtr1##ind =                                      \
	    *(MLIB_TYPE **) ((mlib_u8 *)lineAddr + filterpos);   \
	srcPixelPtr1##ind += X >> MLIB_SHIFT;                    \
	X += dX;                                                 \
	Y += dY;                                                 \
	filterpos = MLIB_POINTER_SHIFT(Y);                       \
	srcPixelPtr2##ind =                                      \
	    *(MLIB_TYPE **) ((mlib_u8 *)lineAddr + filterpos);   \
	srcPixelPtr2##ind += X >> MLIB_SHIFT;                    \
	X += dX;                                                 \
	Y += dY;                                                 \
	filterpos = MLIB_POINTER_SHIFT(Y);                       \
	srcPixelPtr3##ind =                                      \
	    *(MLIB_TYPE **) ((mlib_u8 *)lineAddr + filterpos);   \
	srcPixelPtr3##ind += X >> MLIB_SHIFT;                    \
	X += dX;                                                 \
	Y += dY;
/* ******************************************************************* */
#define	NEXT_PIXELS_1BL_U8_END(count)                                    \
	filterpos = MLIB_POINTER_SHIFT(Y);                               \
	srcPixelPtr00 =                                                  \
	    *(MLIB_TYPE **) ((mlib_u8 *)lineAddr + filterpos);           \
	srcPixelPtr00 += X >> MLIB_SHIFT;                                \
	X += dX;                                                         \
	Y += dY;                                                         \
	srcPixelPtr30 = srcPixelPtr00;                                   \
	if ((count) == 1) {                                              \
	    srcPixelPtr20 = srcPixelPtr00;                               \
	    srcPixelPtr10 = srcPixelPtr00;                               \
	} else {                                                         \
	    if ((count) == 2) {                                          \
		filterpos = MLIB_POINTER_SHIFT(Y);                       \
		srcPixelPtr10 =                                          \
		    *(MLIB_TYPE **) ((mlib_u8 *)lineAddr + filterpos);   \
		srcPixelPtr10 += X >> MLIB_SHIFT;                        \
		srcPixelPtr20 = srcPixelPtr00;                           \
	    } else {                                                     \
		if ((count) == 3) {                                      \
		    filterpos = MLIB_POINTER_SHIFT(Y);                   \
		    srcPixelPtr10 =                                      \
			*(MLIB_TYPE **) ((mlib_u8 *)lineAddr +           \
			filterpos);                                      \
		    srcPixelPtr10 += X >> MLIB_SHIFT;                    \
		    X += dX;                                             \
		    Y += dY;                                             \
		    filterpos = MLIB_POINTER_SHIFT(Y);                   \
		    srcPixelPtr20 =                                      \
			*(MLIB_TYPE **) ((mlib_u8 *)lineAddr +           \
			filterpos);                                      \
		    srcPixelPtr20 += X >> MLIB_SHIFT;                    \
		}                                                        \
	    }                                                            \
	}
/* ******************************************************************* */
#define	NEXT_PIXELS_2BL_U8(ind)                                  \
	filterpos = MLIB_POINTER_SHIFT(Y);                       \
	srcPixelPtr0##ind =                                      \
	    *(MLIB_TYPE **) ((mlib_u8 *)lineAddr + filterpos);   \
	srcPixelPtr0##ind += (X >> (MLIB_SHIFT - 1)) & ~1;       \
	X += dX;                                                 \
	Y += dY;                                                 \
	filterpos = MLIB_POINTER_SHIFT(Y);                       \
	srcPixelPtr1##ind =                                      \
	    *(MLIB_TYPE **) ((mlib_u8 *)lineAddr + filterpos);   \
	srcPixelPtr1##ind += (X >> (MLIB_SHIFT - 1)) & ~1;       \
	X += dX;                                                 \
	Y += dY;                                                 \
	filterpos = MLIB_POINTER_SHIFT(Y);                       \
	srcPixelPtr2##ind =                                      \
	    *(MLIB_TYPE **) ((mlib_u8 *)lineAddr + filterpos);   \
	srcPixelPtr2##ind += (X >> (MLIB_SHIFT - 1)) & ~1;       \
	X += dX;                                                 \
	Y += dY;                                                 \
	filterpos = MLIB_POINTER_SHIFT(Y);                       \
	srcPixelPtr3##ind =                                      \
	    *(MLIB_TYPE **) ((mlib_u8 *)lineAddr + filterpos);   \
	srcPixelPtr3##ind += (X >> (MLIB_SHIFT - 1)) & ~1;       \
	X += dX;                                                 \
	Y += dY;
/* ******************************************************************* */
#define	NEXT_PIXELS_2BL_U8_END(count)                                    \
	filterpos = MLIB_POINTER_SHIFT(Y);                               \
	srcPixelPtr00 =                                                  \
	    *(MLIB_TYPE **) ((mlib_u8 *)lineAddr + filterpos);           \
	srcPixelPtr00 += (X >> (MLIB_SHIFT - 1)) & ~1;                   \
	X += dX;                                                         \
	Y += dY;                                                         \
	srcPixelPtr30 = srcPixelPtr00;                                   \
	if ((count) == 1) {                                              \
	    srcPixelPtr20 = srcPixelPtr00;                               \
	    srcPixelPtr10 = srcPixelPtr00;                               \
	} else {                                                         \
	    if ((count) == 2) {                                          \
		filterpos = MLIB_POINTER_SHIFT(Y);                       \
		srcPixelPtr10 =                                          \
		    *(MLIB_TYPE **) ((mlib_u8 *)lineAddr + filterpos);   \
		srcPixelPtr10 += (X >> (MLIB_SHIFT - 1)) & ~1;           \
		srcPixelPtr20 = srcPixelPtr00;                           \
	    } else {                                                     \
		if ((count) == 3) {                                      \
		    filterpos = MLIB_POINTER_SHIFT(Y);                   \
		    srcPixelPtr10 =                                      \
			*(MLIB_TYPE **) ((mlib_u8 *)lineAddr +           \
			filterpos);                                      \
		    srcPixelPtr10 += (X >> (MLIB_SHIFT - 1)) & ~1;       \
		    X += dX;                                             \
		    Y += dY;                                             \
		    filterpos = MLIB_POINTER_SHIFT(Y);                   \
		    srcPixelPtr20 =                                      \
			*(MLIB_TYPE **) ((mlib_u8 *)lineAddr +           \
			filterpos);                                      \
		    srcPixelPtr20 += (X >> (MLIB_SHIFT - 1)) & ~1;       \
		}                                                        \
	    }                                                            \
	}
/* ******************************************************************* */
#define	LOAD_2BL_U8_AL1(accum, ind, a)                               \
	accum =                                                      \
	MMX_MERGE4_U16(*(mlib_u16 *)((mlib_u8 *)srcPixelPtr0##a +    \
	    ind), *(mlib_u16 *)((mlib_u8 *)srcPixelPtr1##a + ind),   \
	    *(mlib_u16 *)((mlib_u8 *)srcPixelPtr2##a + ind),         \
	    *(mlib_u16 *)((mlib_u8 *)srcPixelPtr3##a + ind));
/* ******************************************************************* */
#define	LOAD_2BL_U8_AL(ind)                                     \
	LOAD_2BL_U8_AL1(ul##ind, 0,                             \
	    ind) LOAD_2BL_U8_AL1(ur##ind, 2,                    \
	    ind) LOAD_2BL_U8_AL1(ll##ind, srcYStride,           \
	    ind) LOAD_2BL_U8_AL1(lr##ind, srcYStride1, ind)

#define	mmmx_read_hi(x)	(x)
#define	mmmx_read_lo(x)	_mm_srli_si64(x, 32)
/* ******************************************************************* */
#define	RESULT_1BL_U8(pos, ind)                                 \
	pos##0 =                                                \
	mmx_fmul8x16hi(mmmx_read_##pos(ul##ind), delta1_x);     \
	pos##1 =                                                \
	mmx_fmul8x16hi(mmmx_read_##pos(ur##ind), deltax);       \
	pos##4 = _mm_add_pi16(pos##0, pos##1);                  \
	pos##4 = _mm_slli_pi16(pos##4, 1);                      \
	pos##6 = _mm_mulhi_pi16(pos##4, delta1_y);              \
	pos##2 =                                                \
	mmx_fmul8x16hi(mmmx_read_##pos(ll##ind), delta1_x);     \
	pos##3 =                                                \
	mmx_fmul8x16hi(mmmx_read_##pos(lr##ind), deltax);       \
	pos##5 = _mm_add_pi16(pos##2, pos##3);                  \
	pos##5 = _mm_slli_pi16(pos##5, 1);                      \
	pos##7 = _mm_mulhi_pi16(pos##5, deltay);                \
	pos##8 = _mm_add_pi16(pos##6, pos##7);                  \
	pos##8 = _mm_add_pi16(pos##8, k05);                     \
	deltax = _mm_add_pi16(deltax, dx64);                    \
	deltay = _mm_add_pi16(deltay, dy64);                    \
	deltax = _mm_and_si64(deltax, mask_7fff);               \
	deltay = _mm_and_si64(deltay, mask_7fff);               \
	delta1_x = _mm_sub_pi16(mask_7fff, deltax);             \
	delta1_y = _mm_sub_pi16(mask_7fff, deltay);
/* ******************************************************************* */
#define	BL_U8_3CH_MAKE                                                     \
	filterpos = MLIB_POINTER_SHIFT(Y);                                 \
	srcPixelPtr = *(MLIB_TYPE **) ((mlib_u8 *)lineAddr + filterpos);   \
	srcPixelPtr += (X >> MLIB_SHIFT) * 3;                              \
	filterpos = (X >> FILTER_SHIFT) & FILTER_MASK;                     \
	xfilter =                                                          \
	    *((__m64 *) ((mlib_u8 *)mlib_filters_u8_bl + filterpos));      \
	filterpos = (Y >> FILTER_SHIFT) & FILTER_MASK;                     \
	yfilter =                                                          \
	    *((__m64 *) ((mlib_u8 *)mlib_filters_u8_bl + filterpos +       \
	    STEPS * 8));                                                   \
	filter = _mm_mulhi_pi16(xfilter, yfilter);                         \
	filter01 = _mm_unpacklo_pi16(filter, filter);                      \
	filter23 = _mm_unpackhi_pi16(filter, filter);                      \
	filter0 = _mm_unpacklo_pi16(filter01, filter01);                   \
	filter1 = _mm_unpackhi_pi16(filter01, filter01);                   \
	filter2 = _mm_unpacklo_pi16(filter23, filter23);                   \
	filter3 = _mm_unpackhi_pi16(filter23, filter23);                   \
	hi_top = *(mlib_s32 *)srcPixelPtr;                                 \
	lo_top = *(mlib_s32 *)(srcPixelPtr + 3);                           \
	hi_bot = *(mlib_s32 *)(srcPixelPtr + srcYStride);                  \
	lo_bot = *(mlib_s32 *)(srcPixelPtr + srcYStride + 3);              \
	ulprod =                                                           \
	_mm_mulhi_pi16(_mm_slli_pi16(_mm_unpacklo_pi8(_m_from_int          \
	    (hi_top), _m_mzero[0]), 7), filter0);                          \
	urprod =                                                           \
	_mm_mulhi_pi16(_mm_slli_pi16(_mm_unpacklo_pi8(_m_from_int          \
	    (lo_top), _m_mzero[0]), 7), filter1);                          \
	llprod =                                                           \
	_mm_mulhi_pi16(_mm_slli_pi16(_mm_unpacklo_pi8(_m_from_int          \
	    (hi_bot), _m_mzero[0]), 7), filter2);                          \
	lrprod =                                                           \
	_mm_mulhi_pi16(_mm_slli_pi16(_mm_unpacklo_pi8(_m_from_int          \
	    (lo_bot), _m_mzero[0]), 7), filter3);                          \
	sm0 = _mm_add_pi16(ulprod, urprod);                                \
	sm0 = _mm_add_pi16(sm0, llprod);                                   \
	sm0 = _mm_add_pi16(sm0, lrprod);                                   \
	sm0 = _mm_add_pi16(sm0, k05);                                      \
	X += dX;                                                           \
	Y += dY;
/* ******************************************************************* */
#define	BL_U8_4CH_MAKE_AL(index)                                           \
	filterpos = MLIB_POINTER_SHIFT(Y);                                 \
	srcPixelPtr = *(MLIB_TYPE **) ((mlib_u8 *)lineAddr + filterpos);   \
	srcPixelPtr += (X >> (MLIB_SHIFT - 2)) & ~3;                       \
	filterpos = (X >> FILTER_SHIFT) & FILTER_MASK;                     \
	xfilter =                                                          \
	    *((__m64 *) ((mlib_u8 *)mlib_filters_u8_bl + filterpos));      \
	filterpos = (Y >> FILTER_SHIFT) & FILTER_MASK;                     \
	yfilter =                                                          \
	    *((__m64 *) ((mlib_u8 *)mlib_filters_u8_bl + filterpos +       \
	    STEPS * 8));                                                   \
	filter = _mm_mulhi_pi16(xfilter, yfilter);                         \
	filter01 = _mm_unpacklo_pi16(filter, filter);                      \
	filter23 = _mm_unpackhi_pi16(filter, filter);                      \
	filter0 = _mm_unpacklo_pi16(filter01, filter01);                   \
	filter1 = _mm_unpackhi_pi16(filter01, filter01);                   \
	filter2 = _mm_unpacklo_pi16(filter23, filter23);                   \
	filter3 = _mm_unpackhi_pi16(filter23, filter23);                   \
	hi_top = *(mlib_s32 *)srcPixelPtr;                                 \
	lo_top = *(mlib_s32 *)(srcPixelPtr + 4);                           \
	hi_bot = *(mlib_s32 *)(srcPixelPtr + srcYStride);                  \
	lo_bot = *(mlib_s32 *)(srcPixelPtr + srcYStride + 4);              \
	ulprod = mmx_fmul8x16(hi_top, filter0);                            \
	urprod = mmx_fmul8x16(lo_top, filter1);                            \
	llprod = mmx_fmul8x16(hi_bot, filter2);                            \
	lrprod = mmx_fmul8x16(lo_bot, filter3);                            \
	sm##index = _mm_add_pi16(ulprod, urprod);                          \
	sm##index = _mm_add_pi16(sm##index, llprod);                       \
	sm##index = _mm_add_pi16(sm##index, lrprod);                       \
	sm##index = _mm_add_pi16(sm##index, k05);                          \
	X += dX;                                                           \
	Y += dY;
/* ******************************************************************* */
#define	BL_U8_1CH()                                                        \
	filterpos = MLIB_POINTER_SHIFT(Y);                                 \
	srcPixelPtr = *(MLIB_TYPE **) ((mlib_u8 *)lineAddr + filterpos);   \
	srcPixelPtr += (X >> MLIB_SHIFT);                                  \
	filterpos = (X >> FILTER_SHIFT) & FILTER_MASK;                     \
	xfilter =                                                          \
	    *((__m64 *) ((mlib_u8 *)mlib_filters_u8_bl + filterpos));      \
	filterpos = (Y >> FILTER_SHIFT) & FILTER_MASK;                     \
	yfilter =                                                          \
	    *((__m64 *) ((mlib_u8 *)mlib_filters_u8_bl + filterpos +       \
	    STEPS * 8));                                                   \
	filter = _mm_mulhi_pi16(xfilter, yfilter);                         \
	hi0 =                                                              \
	_m_from_int(*(mlib_u16 *)srcPixelPtr | (*(mlib_u16                 \
	    *)(srcPixelPtr + srcYStride) << 16));                          \
	hi0 = _mm_madd_pi16(_mm_unpacklo_pi8(hi0, _m_zero), filter);       \
	hi0 = _mm_add_pi32(hi0, _mm_srli_si64(hi0, 32));                   \
	X += dX;                                                           \
	Y += dY;
/* ******************************************************************* */

#undef  MMX_SCALE
#define	MMX_SCALE	3

/* ******************************************************************* */

mlib_status
mlib_ImageAffine_u8_1ch_bl(
    mlib_affine_param *param)
{
	DECLAREVAR_BL();

	for (j = yStart; j <= yFinish; j++) {
		__m64 hi0;
		__m64 xfilter, yfilter, filter;
		mlib_s32 cols, filterpos, rr, i;

		CLIP(1);

		cols = xRight - xLeft + 1;
		for (i = 0; i < cols; i++) {
			BL_U8_1CH();
			rr = (_m_to_int(hi0) >> 12);
			dstPixelPtr[i] = rr;
		}
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  PREPARE_DELTAS
#define	PREPARE_DELTAS                                                \
	if (warp_tbl != NULL) {                                       \
	    dX = warp_tbl[2 * j];                                     \
	    dY = warp_tbl[2 * j + 1];                                 \
	    dx64 =                                                    \
		mmx_to_double_dup((((dX) & 0xFFFF) << 16) | ((dX) &   \
		0xFFFF));                                             \
	    dy64 =                                                    \
		mmx_to_double_dup((((dY) & 0xFFFF) << 16) | ((dY) &   \
		0xFFFF));                                             \
	}

/* ******************************************************************* */

#undef  MMX_SCALE
#define	MMX_SCALE	0

mlib_status
mlib_ImageAffine_u8_2ch_bl(
    mlib_affine_param *param)
{
	DECLAREVAR_BL();
	mlib_s32 srcYStride1 = srcYStride + 2;
	MLIB_TYPE *srcPixelPtr00, *srcPixelPtr10, *srcPixelPtr20,
	    *srcPixelPtr30;
	__m64 dx64, dy64;
	__m64 mask_7fff = mmx_to_double_dup(0x7FFF7FFF);
	__m64 k05 = mmx_to_double_dup(0x00200020);

	dx64 = mmx_to_double_dup((((dX) & 0xFFFF) << 16) | ((dX) & 0xFFFF));
	dy64 = mmx_to_double_dup((((dY) & 0xFFFF) << 16) | ((dY) & 0xFFFF));

	for (j = yStart; j <= yFinish; j++) {
		__m64 *dp;
		__m64 deltax, deltay, delta1_x, delta1_y;
		__m64 hi0, hi1, hi2, hi3, hi4, hi5, hi6, hi7, hi8;
		__m64 lo0, lo1, lo2, lo3, lo4, lo5, lo6, lo7, lo8;
		__m64 ul0, ur0, ll0, lr0;
		mlib_s32 cols, filterpos;
		mlib_s32 x0, x1, y0, y1;
		mlib_s32 i;

		CLIP(2);

		cols = xRight - xLeft + 1;
		dp = (__m64 *) dstPixelPtr;
		x0 = X;
		y0 = Y;
		x1 = (x0 + dX);
		y1 = (y0 + dY);

		deltax =
		    mmx_to_double((((x0 & 0xFFFE) << 15) | ((x0 & 0xFFFE) >>
		    1)), (((x1 & 0xFFFE) << 15) | ((x1 & 0xFFFE) >> 1)));
		deltay =
		    mmx_to_double((((y0 & 0xFFFE) << 15) | ((y0 & 0xFFFE) >>
		    1)), (((y1 & 0xFFFE) << 15) | ((y1 & 0xFFFE) >> 1)));

		delta1_x = _mm_sub_pi16(mask_7fff, deltax);
		delta1_y = _mm_sub_pi16(mask_7fff, deltay);

		i = 0;
		if (cols >= 4) {

			NEXT_PIXELS_2BL_U8(0)

			    for (; i <= cols - 8; i += 4) {
				LOAD_2BL_U8_AL(0)
				    NEXT_PIXELS_2BL_U8(0)
				    RESULT_1BL_U8(hi, 0)
				    RESULT_1BL_U8(lo, 0)
				    (*dp++) = mmx_fpack16_pair(hi8, lo8);
			}
			LOAD_2BL_U8_AL(0)
			    RESULT_1BL_U8(hi, 0)
			    RESULT_1BL_U8(lo, 0)
			    (*dp++) = mmx_fpack16_pair(hi8, lo8);
			i += 4;
		}

		if (i < cols) {
			mlib_s32 tail = cols - i;

			NEXT_PIXELS_2BL_U8_END(tail)
			    LOAD_2BL_U8_AL(0)
			    RESULT_1BL_U8(hi, 0)
			    RESULT_1BL_U8(lo, 0)
			    hi8 = mmx_fpack16_pair(hi8, lo8);
			mmx_pst_16(hi8, dp, 0xF0 >> tail);
		}
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  MMX_SCALE
#define	MMX_SCALE	3

/* *********************************************************** */

#undef  PREPARE_DELTAS
#define	PREPARE_DELTAS                                          \
	if (warp_tbl != NULL) {                                 \
	    dX = warp_tbl[2 * j];                               \
	    dY = warp_tbl[2 * j + 1];                           \
	}

/* *********************************************************** */

mlib_status
mlib_ImageAffine_u8_3ch_bl(
    mlib_affine_param *param)
{
	DECLAREVAR_BL();
	__m64 filter01, filter23, filter0, filter1, filter2, filter3;
	__m64 k05 = mmx_to_double_dup(0x00040004);
	__m64 _m_mzero[1] = { 0 };

	for (j = yStart; j <= yFinish; j++) {
		__m64 filter, xfilter, yfilter;
		__m64 sm0;
		__m64 ulprod, urprod, llprod, lrprod;
		mlib_s32 col, cols, tail;
		mlib_s32 filterpos;
		mlib_s32 hi_top, lo_top, hi_bot, lo_bot;

		CLIP(3);

		cols = xRight - xLeft + 1;
		tail = dstPixelPtr[3 * cols];

		for (col = 0; col < cols; col++) {
			BL_U8_3CH_MAKE
			    * (mlib_s32 *)dstPixelPtr = mmx_fpack16(sm0);
			dstPixelPtr += 3;
		}

		dstPixelPtr[0] = tail;
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageAffine_u8_4ch_bl(
    mlib_affine_param *param)
{
	DECLAREVAR_BL();
	__m64 filter01, filter23, filter0, filter1, filter2, filter3;
	__m64 k05 = mmx_to_double_dup(0x00040004);

	for (j = yStart; j <= yFinish; j++) {
		__m64 *dpDst;
		__m64 accum, filter, xfilter, yfilter;
		__m64 sm0, sm1;
		__m64 ulprod, urprod, llprod, lrprod;
		mlib_s32 col, cols, times;
		mlib_s32 filterpos;
		mlib_s32 hi_top, lo_top, hi_bot, lo_bot;

		CLIP(4);

		cols = xRight - xLeft + 1;
		times = cols >> 1;
		dpDst = (__m64 *) dstPixelPtr;

		if (times > 0) {
			BL_U8_4CH_MAKE_AL(0)
			    BL_U8_4CH_MAKE_AL(1)

			    for (col = 1; col < times; col++) {
				accum = mmx_fpack16_pair(sm0, sm1);
				BL_U8_4CH_MAKE_AL(0)
				    BL_U8_4CH_MAKE_AL(1)
				    (*dpDst++) = accum;
			}
			accum = mmx_fpack16_pair(sm0, sm1);
			(*dpDst++) = accum;
		}
		cols -= times << 1;

		if (cols == 1) {
			BL_U8_4CH_MAKE_AL(0)
			    * (mlib_s32 *)dpDst = mmx_fpack16(sm0);
		}
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  MLIB_TYPE
#define	MLIB_TYPE	mlib_s16

/* *********************************************************** */
#define	NEXT_PIXELS_2BL_S16                                      \
	filterpos = MLIB_POINTER_SHIFT(Y);                       \
	srcPixelPtr00 =                                          \
	    *(MLIB_TYPE **) ((mlib_u8 *)lineAddr + filterpos);   \
	srcPixelPtr00 += (X >> (MLIB_SHIFT - 1)) & ~1;           \
	X += dX;                                                 \
	Y += dY;                                                 \
	filterpos = MLIB_POINTER_SHIFT(Y);                       \
	srcPixelPtr10 =                                          \
	    *(MLIB_TYPE **) ((mlib_u8 *)lineAddr + filterpos);   \
	srcPixelPtr10 += (X >> (MLIB_SHIFT - 1)) & ~1;           \
	X += dX;                                                 \
	Y += dY;
/* ******************************************************************* */
#define	NEXT_PIXELS_3BL_S16                                                \
	xSrc = X >> MLIB_SHIFT;                                            \
	filterpos = MLIB_POINTER_SHIFT(Y);                                 \
	srcPixelPtr = *(MLIB_TYPE **) ((mlib_u8 *)lineAddr + filterpos);   \
	srcPixelPtr += ((xSrc << 1) + xSrc);                               \
	X += dX;                                                           \
	Y += dY;
/* ******************************************************************* */
#define	NEXT_PIXELS_4BL_S16                                                \
	filterpos = MLIB_POINTER_SHIFT(Y);                                 \
	srcPixelPtr = *(MLIB_TYPE **) ((mlib_u8 *)lineAddr + filterpos);   \
	srcPixelPtr += (X >> (MLIB_SHIFT - 2)) & ~3;                       \
	X += dX;                                                           \
	Y += dY;
/* ******************************************************************* */
#define	LOAD_2BL_S16_AL                                                  \
	ul0 =                                                            \
	mmx_freg_pair(*(mlib_s32 *)srcPixelPtr00,                        \
	    *(mlib_s32 *)srcPixelPtr10);                                 \
	ur0 =                                                            \
	mmx_freg_pair(*(mlib_s32 *)((mlib_u8 *)srcPixelPtr00 + 4),       \
	    *(mlib_s32 *)((mlib_u8 *)srcPixelPtr10 + 4));                \
	ll0 =                                                            \
	mmx_freg_pair(*(mlib_s32 *)((mlib_u8 *)srcPixelPtr00 +           \
	    srcYStride),                                                 \
	    *(mlib_s32 *)((mlib_u8 *)srcPixelPtr10 + srcYStride));       \
	lr0 =                                                            \
	mmx_freg_pair(*(mlib_s32 *)((mlib_u8 *)srcPixelPtr00 +           \
	    srcYStride + 4),                                             \
	    *(mlib_s32 *)((mlib_u8 *)srcPixelPtr10 + srcYStride + 4));
/* ******************************************************************* */
#define	LOAD_3BL_S16                                                  \
	ul0 = *(__m64 *) srcPixelPtr;                                 \
	ur0 = *(__m64 *) ((mlib_u8 *)srcPixelPtr + 6);                \
	ll0 = *(__m64 *) ((mlib_u8 *)srcPixelPtr + srcYStride);       \
	lr0 = *(__m64 *) ((mlib_u8 *)srcPixelPtr + srcYStride + 6);
/* ******************************************************************* */
#define	LOAD_4BL_S16_AL                                               \
	ul0 = *(__m64 *) srcPixelPtr;                                 \
	ur0 = *(__m64 *) ((mlib_u8 *)srcPixelPtr + 8);                \
	ll0 = *(__m64 *) ((mlib_u8 *)srcPixelPtr + srcYStride);       \
	lr0 = *(__m64 *) ((mlib_u8 *)srcPixelPtr + srcYStride + 8);
/* ******************************************************************* */
#define	RESULT_1BL_S16(pos)                                       \
	ul0 = _mm_mulhi_pi16(ul0, pos##_delta1_x);                \
	ur0 = _mm_mulhi_pi16(ur0, pos##_deltax);                  \
	ul0 = _mm_add_pi16(ul0, ur0);                             \
	ul0 = _mm_add_pi16(ul0, ul0);                             \
	ul0 = _mm_mulhi_pi16(ul0, pos##_delta1_y);                \
	ll0 = _mm_mulhi_pi16(ll0, pos##_delta1_x);                \
	lr0 = _mm_mulhi_pi16(lr0, pos##_deltax);                  \
	ll0 = _mm_add_pi16(ll0, lr0);                             \
	ll0 = _mm_add_pi16(ll0, ll0);                             \
	ll0 = _mm_mulhi_pi16(ll0, pos##_deltay);                  \
	hi8 = _mm_add_pi16(ll0, ul0);                             \
	hi8 = _mm_add_pi16(hi8, hi8);                             \
	pos##_deltax = _mm_add_pi16(pos##_deltax, dx64);          \
	pos##_deltay = _mm_add_pi16(pos##_deltay, dy64);          \
	pos##_deltax = _mm_and_si64(pos##_deltax, mask_7fff);     \
	pos##_deltay = _mm_and_si64(pos##_deltay, mask_7fff);     \
	pos##_delta1_x = _mm_sub_pi16(mask_7ffe, pos##_deltax);   \
	pos##_delta1_y = _mm_sub_pi16(mask_7ffe, pos##_deltay);
/* ******************************************************************* */
#undef PREPARE_DELTAS
#define	PREPARE_DELTAS                                                     \
	if (warp_tbl != NULL) {                                            \
	    dX = warp_tbl[2 * j];                                          \
	    dY = warp_tbl[2 * j + 1];                                      \
	    dx64 =                                                         \
		mmx_to_double_dup((((dX << 1) & 0xFFFF) << 16) | ((dX <<   \
		1) & 0xFFFF));                                             \
	    dy64 =                                                         \
		mmx_to_double_dup((((dY << 1) & 0xFFFF) << 16) | ((dY <<   \
		1) & 0xFFFF));                                             \
	}
/* ******************************************************************* */

mlib_status
mlib_ImageAffine_s16_1ch_bl(
    mlib_affine_param *param)
{
	DECLAREVAR_BL();
	mlib_s32 srcYStride1 = srcYStride + 2;
	MLIB_TYPE *srcPixelPtr00, *srcPixelPtr10, *srcPixelPtr20,
	    *srcPixelPtr30;
	__m64 dx64, dy64;
	__m64 mask_7fff = mmx_to_double_dup(0x7FFF7FFF);
	__m64 mask_7ffe = mmx_to_double_dup(0x7FFE7FFE);

	dx64 =
	    mmx_to_double_dup((((dX << 1) & 0xFFFF) << 16) | ((dX << 1) &
	    0xFFFF));
	dy64 =
	    mmx_to_double_dup((((dY << 1) & 0xFFFF) << 16) | ((dY << 1) &
	    0xFFFF));

	for (j = yStart; j <= yFinish; j++) {
		__m64 *dp;
		__m64 ul0, ur0, ll0, lr0;
		__m64 hi_deltax, hi_deltay, hi_delta1_x, hi_delta1_y;
		__m64 hi8;
		mlib_s32 cols, times, filterpos;
		mlib_s32 x0, x1, x2, x3, y0, y1, y2, y3;
		mlib_s32 i;

		CLIP(1);

		cols = xRight - xLeft + 1;
		dp = (__m64 *) dstPixelPtr;
		x0 = X;
		y0 = Y;
		x1 = (x0 + dX);
		y1 = (y0 + dY);
		x2 = (x1 + dX);
		y2 = (y1 + dY);
		x3 = (x2 + dX);
		y3 = (y2 + dY);

		hi_deltax =
		    mmx_to_double((((x0 & 0xFFFE) << 15) | ((x1 & 0xFFFE) >>
		    1)), (((x2 & 0xFFFE) << 15) | ((x3 & 0xFFFE) >> 1)));
		hi_deltay =
		    mmx_to_double((((y0 & 0xFFFE) << 15) | ((y1 & 0xFFFE) >>
		    1)), (((y2 & 0xFFFE) << 15) | ((y3 & 0xFFFE) >> 1)));

		hi_delta1_x = _mm_sub_pi16(mask_7ffe, hi_deltax);
		hi_delta1_y = _mm_sub_pi16(mask_7ffe, hi_deltay);

		i = 0;
		if (i <= cols - 4) {
			NEXT_PIXELS_1BL_U8(0)

			    for (; i <= cols - 8; i += 4) {
				LOAD_2BL_U8_AL(0)
				    NEXT_PIXELS_1BL_U8(0)
				    RESULT_1BL_S16(hi)
				    (*dp++) = hi8;
			}

			LOAD_2BL_U8_AL(0)
			    RESULT_1BL_S16(hi)
			    (*dp++) = hi8;
			i += 4;
		}

		if (i < cols) {
			mlib_s32 tail = cols - i;

			NEXT_PIXELS_1BL_U8_END(tail)
			    LOAD_2BL_U8_AL(0)
			    RESULT_1BL_S16(hi)
			    mmx_pst_16(hi8, dp, 0xF0 >> tail);
		}
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* ******************************************************************* */
#undef PREPARE_DELTAS
#define	PREPARE_DELTAS                                                \
	if (warp_tbl != NULL) {                                       \
	    dX = warp_tbl[2 * j];                                     \
	    dY = warp_tbl[2 * j + 1];                                 \
	    dx64 =                                                    \
		mmx_to_double_dup((((dX) & 0xFFFF) << 16) | ((dX) &   \
		0xFFFF));                                             \
	    dy64 =                                                    \
		mmx_to_double_dup((((dY) & 0xFFFF) << 16) | ((dY) &   \
		0xFFFF));                                             \
	}
/* ******************************************************************* */

mlib_status
mlib_ImageAffine_s16_2ch_bl(
    mlib_affine_param *param)
{
	DECLAREVAR_BL();
	MLIB_TYPE *srcPixelPtr00, *srcPixelPtr10;
	__m64 dx64, dy64;
	__m64 mask_7fff = mmx_to_double_dup(0x7FFF7FFF);
	__m64 mask_7ffe = mmx_to_double_dup(0x7FFE7FFE);

	dx64 = mmx_to_double_dup((((dX) & 0xFFFF) << 16) | ((dX) & 0xFFFF));
	dy64 = mmx_to_double_dup((((dY) & 0xFFFF) << 16) | ((dY) & 0xFFFF));

	for (j = yStart; j <= yFinish; j++) {
		__m64 *dp;
		__m64 ul0, ur0, ll0, lr0;
		__m64 hi_deltax, hi_deltay, hi_delta1_x, hi_delta1_y;
		__m64 hi8;
		mlib_s32 cols, filterpos;
		mlib_s32 x0, x1, y0, y1;
		mlib_s32 i;

		CLIP(2);

		cols = xRight - xLeft + 1;
		dp = (__m64 *) dstPixelPtr;
		x0 = X;
		y0 = Y;
		x1 = (x0 + dX);
		y1 = (y0 + dY);

		hi_deltax =
		    mmx_to_double((((x0 & 0xFFFE) << 15) | ((x0 & 0xFFFE) >>
		    1)), (((x1 & 0xFFFE) << 15) | ((x1 & 0xFFFE) >> 1)));
		hi_deltay =
		    mmx_to_double((((y0 & 0xFFFE) << 15) | ((y0 & 0xFFFE) >>
		    1)), (((y1 & 0xFFFE) << 15) | ((y1 & 0xFFFE) >> 1)));

		hi_delta1_x = _mm_sub_pi16(mask_7ffe, hi_deltax);
		hi_delta1_y = _mm_sub_pi16(mask_7ffe, hi_deltay);

		i = 0;
		if (i <= cols - 2) {
			NEXT_PIXELS_2BL_S16 for (; i <= cols - 4; i += 2) {
				LOAD_2BL_S16_AL
				    NEXT_PIXELS_2BL_S16 RESULT_1BL_S16(hi)
				    (*dp++) = hi8;
			}
			LOAD_2BL_S16_AL RESULT_1BL_S16(hi)
			    (*dp++) = hi8;
			i += 2;
		}

		if (i < cols) {
			filterpos = MLIB_POINTER_SHIFT(Y);
			srcPixelPtr00 =
			    *(MLIB_TYPE **) ((mlib_u8 *)lineAddr + filterpos);
			srcPixelPtr00 += (X >> (MLIB_SHIFT - 1)) & ~1;
			srcPixelPtr10 = srcPixelPtr00;
			LOAD_2BL_S16_AL RESULT_1BL_S16(hi)
			    * (mlib_s32 *)dp = mmx_read_hi(hi8);
		}
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* ******************************************************************* */
#undef PREPARE_DELTAS
#define	PREPARE_DELTAS                                                     \
	if (warp_tbl != NULL) {                                            \
	    dX = warp_tbl[2 * j];                                          \
	    dY = warp_tbl[2 * j + 1];                                      \
	    dX = (dX - (dX >> 31)) & ~1;                                   \
/*                                                                         \
 * rounding towards ZERO                                                   \
 */                                                                        \
	    dY = (dY - (dY >> 31)) & ~1;                                   \
/*                                                                         \
 * rounding towards ZERO                                                   \
 */                                                                        \
	    dx64 =                                                         \
		mmx_to_double_dup(((dX & 0xFFFE) << 15) | ((dX & 0xFFFE)   \
		>> 1));                                                    \
	    dy64 =                                                         \
		mmx_to_double_dup(((dY & 0xFFFE) << 15) | ((dY & 0xFFFE)   \
		>> 1));                                                    \
	}
/* ******************************************************************* */

mlib_status
mlib_ImageAffine_s16_3ch_bl(
    mlib_affine_param *param)
{
	DECLAREVAR_BL();
	__m64 dx64, dy64;
	__m64 mask_7fff = mmx_to_double_dup(0x7FFF7FFF);
	__m64 mask_7ffe = mmx_to_double_dup(0x7FFE7FFE);

/* rounding towards ZERO */
	dX = (dX - (dX >> 31)) & ~1;
/* rounding towards ZERO */
	dY = (dY - (dY >> 31)) & ~1;

	dx64 = mmx_to_double_dup(((dX & 0xFFFE) << 15) | ((dX & 0xFFFE) >> 1));
	dy64 = mmx_to_double_dup(((dY & 0xFFFE) << 15) | ((dY & 0xFFFE) >> 1));

	for (j = yStart; j <= yFinish; j++) {
		__m64 ul0, ur0, ll0, lr0;
		__m64 hi_deltax, hi_deltay, hi_delta1_x, hi_delta1_y;
		__m64 hi8;
		mlib_s32 cols, filterpos, tail;
		mlib_s32 x0, y0;
		mlib_s32 i;

		CLIP(3);

		cols = xRight - xLeft + 1;
		tail = dstPixelPtr[3 * cols];
		x0 = X;
		y0 = Y;

		hi_deltax =
		    mmx_to_double_dup(((x0 & 0xFFFE) << 15) | ((x0 & 0xFFFE) >>
		    1));
		hi_deltay =
		    mmx_to_double_dup(((y0 & 0xFFFE) << 15) | ((y0 & 0xFFFE) >>
		    1));

		hi_delta1_x = _mm_sub_pi16(mask_7ffe, hi_deltax);
		hi_delta1_y = _mm_sub_pi16(mask_7ffe, hi_deltay);

		if (cols != 0) {

			NEXT_PIXELS_3BL_S16 for (i = 0; i < cols - 1; i++) {
				LOAD_3BL_S16
				    NEXT_PIXELS_3BL_S16 RESULT_1BL_S16(hi)
				    * (__m64 *) dstPixelPtr = hi8;
				dstPixelPtr += 3;
			}
			LOAD_3BL_S16 RESULT_1BL_S16(hi)
			    * (__m64 *) dstPixelPtr = hi8;
			dstPixelPtr += 3;
		}

		dstPixelPtr[0] = tail;
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageAffine_s16_4ch_bl(
    mlib_affine_param *param)
{

	DECLAREVAR_BL();
	__m64 dx64, dy64;
	__m64 mask_7fff = mmx_to_double_dup(0x7FFF7FFF);
	__m64 mask_7ffe = mmx_to_double_dup(0x7FFE7FFE);

/* rounding towards ZERO */
	dX = (dX - (dX >> 31)) & ~1;
/* rounding towards ZERO */
	dY = (dY - (dY >> 31)) & ~1;

	dx64 = mmx_to_double_dup(((dX & 0xFFFE) << 15) | ((dX & 0xFFFE) >> 1));
	dy64 = mmx_to_double_dup(((dY & 0xFFFE) << 15) | ((dY & 0xFFFE) >> 1));

	for (j = yStart; j <= yFinish; j++) {
		__m64 *dp;
		__m64 ul0, ur0, ll0, lr0;
		__m64 hi_deltax, hi_deltay, hi_delta1_x, hi_delta1_y;
		__m64 hi8;
		mlib_s32 cols, filterpos;
		mlib_s32 x0, y0;
		mlib_s32 i;

		CLIP(4);

		cols = xRight - xLeft + 1;
		dp = (__m64 *) dstPixelPtr;
		x0 = X;
		y0 = Y;

		hi_deltax =
		    mmx_to_double_dup(((x0 & 0xFFFE) << 15) | ((x0 & 0xFFFE) >>
		    1));
		hi_deltay =
		    mmx_to_double_dup(((y0 & 0xFFFE) << 15) | ((y0 & 0xFFFE) >>
		    1));

		hi_delta1_x = _mm_sub_pi16(mask_7ffe, hi_deltax);
		hi_delta1_y = _mm_sub_pi16(mask_7ffe, hi_deltay);

		if (cols != 0) {

			NEXT_PIXELS_4BL_S16 for (i = 0; i < cols - 1; i++) {
				LOAD_4BL_S16_AL RESULT_1BL_S16(hi)
				    dp[i] = hi8;
			NEXT_PIXELS_4BL_S16}
			LOAD_4BL_S16_AL RESULT_1BL_S16(hi)
			    dp[i] = hi8;
		}
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  MLIB_TYPE
#define	MLIB_TYPE	mlib_u16

/* *********************************************************** */

#define	RESULT_1BL_U16(pos)                                       \
	ul0 = _mm_xor_si64(ul0, mask_8000);                       \
	ur0 = _mm_xor_si64(ur0, mask_8000);                       \
	ll0 = _mm_xor_si64(ll0, mask_8000);                       \
	lr0 = _mm_xor_si64(lr0, mask_8000);                       \
	ul0 = _mm_mulhi_pi16(ul0, pos##_delta1_x);                \
	ur0 = _mm_mulhi_pi16(ur0, pos##_deltax);                  \
	ul0 = _mm_add_pi16(ul0, ur0);                             \
	ul0 = _mm_add_pi16(ul0, ul0);                             \
	ul0 = _mm_mulhi_pi16(ul0, pos##_delta1_y);                \
	ll0 = _mm_mulhi_pi16(ll0, pos##_delta1_x);                \
	lr0 = _mm_mulhi_pi16(lr0, pos##_deltax);                  \
	ll0 = _mm_add_pi16(ll0, lr0);                             \
	ll0 = _mm_add_pi16(ll0, ll0);                             \
	ll0 = _mm_mulhi_pi16(ll0, pos##_deltay);                  \
	hi8 = _mm_add_pi16(ll0, ul0);                             \
	hi8 = _mm_add_pi16(hi8, hi8);                             \
	hi8 = _mm_xor_si64(hi8, mask_8000);                       \
	pos##_deltax = _mm_add_pi16(pos##_deltax, dx64);          \
	pos##_deltay = _mm_add_pi16(pos##_deltay, dy64);          \
	pos##_deltax = _mm_and_si64(pos##_deltax, mask_7fff);     \
	pos##_deltay = _mm_and_si64(pos##_deltay, mask_7fff);     \
	pos##_delta1_x = _mm_sub_pi16(mask_7ffe, pos##_deltax);   \
	pos##_delta1_y = _mm_sub_pi16(mask_7ffe, pos##_deltay);

/* ******************************************************************* */
#undef PREPARE_DELTAS
#define	PREPARE_DELTAS                                                     \
	if (warp_tbl != NULL) {                                            \
	    dX = warp_tbl[2 * j];                                          \
	    dY = warp_tbl[2 * j + 1];                                      \
	    dx64 =                                                         \
		mmx_to_double_dup((((dX << 1) & 0xFFFF) << 16) | ((dX <<   \
		1) & 0xFFFF));                                             \
	    dy64 =                                                         \
		mmx_to_double_dup((((dY << 1) & 0xFFFF) << 16) | ((dY <<   \
		1) & 0xFFFF));                                             \
	}
/* ******************************************************************* */

mlib_status
mlib_ImageAffine_u16_1ch_bl(
    mlib_affine_param *param)
{
	DECLAREVAR_BL();
	mlib_s32 srcYStride1 = srcYStride + 2;
	MLIB_TYPE *srcPixelPtr00, *srcPixelPtr10, *srcPixelPtr20,
	    *srcPixelPtr30;
	__m64 dx64, dy64;
	__m64 mask_7fff = mmx_to_double_dup(0x7FFF7FFF);
	__m64 mask_7ffe = mmx_to_double_dup(0x7FFE7FFE);
	__m64 mask_8000 = mmx_to_double_dup(0x80008000);

	dx64 =
	    mmx_to_double_dup((((dX << 1) & 0xFFFF) << 16) | ((dX << 1) &
	    0xFFFF));
	dy64 =
	    mmx_to_double_dup((((dY << 1) & 0xFFFF) << 16) | ((dY << 1) &
	    0xFFFF));

	for (j = yStart; j <= yFinish; j++) {
		__m64 *dp;
		__m64 ul0, ur0, ll0, lr0;
		__m64 hi_deltax, hi_deltay, hi_delta1_x, hi_delta1_y;
		__m64 hi8;
		mlib_s32 cols, filterpos;
		mlib_s32 x0, x1, x2, x3, y0, y1, y2, y3;
		mlib_s32 i;

		CLIP(1);

		cols = xRight - xLeft + 1;
		dp = (__m64 *) dstPixelPtr;
		x0 = X;
		y0 = Y;
		x1 = (x0 + dX);
		y1 = (y0 + dY);
		x2 = (x1 + dX);
		y2 = (y1 + dY);
		x3 = (x2 + dX);
		y3 = (y2 + dY);

		hi_deltax =
		    mmx_to_double((((x0 & 0xFFFE) << 15) | ((x1 & 0xFFFE) >>
		    1)), (((x2 & 0xFFFE) << 15) | ((x3 & 0xFFFE) >> 1)));
		hi_deltay =
		    mmx_to_double((((y0 & 0xFFFE) << 15) | ((y1 & 0xFFFE) >>
		    1)), (((y2 & 0xFFFE) << 15) | ((y3 & 0xFFFE) >> 1)));

		hi_delta1_x = _mm_sub_pi16(mask_7ffe, hi_deltax);
		hi_delta1_y = _mm_sub_pi16(mask_7ffe, hi_deltay);

		i = 0;
		if (i <= cols - 4) {
			NEXT_PIXELS_1BL_U8(0)

			    for (; i <= cols - 8; i += 4) {
				LOAD_2BL_U8_AL(0)
				    NEXT_PIXELS_1BL_U8(0)
				    RESULT_1BL_U16(hi)
				    (*dp++) = hi8;
			}

			LOAD_2BL_U8_AL(0)
			    RESULT_1BL_U16(hi)
			    (*dp++) = hi8;
			i += 4;
		}

		if (i < cols) {
			mlib_s32 tail = cols - i;

			NEXT_PIXELS_1BL_U8_END(tail)
			    LOAD_2BL_U8_AL(0)
			    RESULT_1BL_U16(hi)
			    mmx_pst_16(hi8, dp, 0xF0 >> tail);
		}
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* ******************************************************************* */
#undef PREPARE_DELTAS
#define	PREPARE_DELTAS                                                \
	if (warp_tbl != NULL) {                                       \
	    dX = warp_tbl[2 * j];                                     \
	    dY = warp_tbl[2 * j + 1];                                 \
	    dx64 =                                                    \
		mmx_to_double_dup((((dX) & 0xFFFF) << 16) | ((dX) &   \
		0xFFFF));                                             \
	    dy64 =                                                    \
		mmx_to_double_dup((((dY) & 0xFFFF) << 16) | ((dY) &   \
		0xFFFF));                                             \
	}
/* ******************************************************************* */

mlib_status
mlib_ImageAffine_u16_2ch_bl(
    mlib_affine_param *param)
{
	DECLAREVAR_BL();
	MLIB_TYPE *srcPixelPtr00, *srcPixelPtr10;
	__m64 dx64, dy64;
	__m64 mask_7fff = mmx_to_double_dup(0x7FFF7FFF);
	__m64 mask_7ffe = mmx_to_double_dup(0x7FFE7FFE);
	__m64 mask_8000 = mmx_to_double_dup(0x80008000);

	dx64 = mmx_to_double_dup((((dX) & 0xFFFF) << 16) | ((dX) & 0xFFFF));
	dy64 = mmx_to_double_dup((((dY) & 0xFFFF) << 16) | ((dY) & 0xFFFF));

	for (j = yStart; j <= yFinish; j++) {
		__m64 *dp;
		__m64 ul0, ur0, ll0, lr0;
		__m64 hi_deltax, hi_deltay, hi_delta1_x, hi_delta1_y;
		__m64 hi8;
		mlib_s32 cols, filterpos;
		mlib_s32 x0, x1, y0, y1;
		mlib_s32 i;

		CLIP(2);

		cols = xRight - xLeft + 1;
		dp = (__m64 *) dstPixelPtr;
		x0 = X;
		y0 = Y;
		x1 = (x0 + dX);
		y1 = (y0 + dY);

		hi_deltax =
		    mmx_to_double((((x0 & 0xFFFE) << 15) | ((x0 & 0xFFFE) >>
		    1)), (((x1 & 0xFFFE) << 15) | ((x1 & 0xFFFE) >> 1)));
		hi_deltay =
		    mmx_to_double((((y0 & 0xFFFE) << 15) | ((y0 & 0xFFFE) >>
		    1)), (((y1 & 0xFFFE) << 15) | ((y1 & 0xFFFE) >> 1)));

		hi_delta1_x = _mm_sub_pi16(mask_7ffe, hi_deltax);
		hi_delta1_y = _mm_sub_pi16(mask_7ffe, hi_deltay);

		i = 0;
		if (i <= cols - 2) {
			NEXT_PIXELS_2BL_S16 for (; i <= cols - 4; i += 2) {
				LOAD_2BL_S16_AL
				    NEXT_PIXELS_2BL_S16 RESULT_1BL_U16(hi)
				    (*dp++) = hi8;
			}
			LOAD_2BL_S16_AL RESULT_1BL_U16(hi)
			    (*dp++) = hi8;
			i += 2;
		}

		if (i < cols) {
			filterpos = MLIB_POINTER_SHIFT(Y);
			srcPixelPtr00 =
			    *(MLIB_TYPE **) ((mlib_u8 *)lineAddr + filterpos);
			srcPixelPtr00 += (X >> (MLIB_SHIFT - 1)) & ~1;
			srcPixelPtr10 = srcPixelPtr00;
			LOAD_2BL_S16_AL RESULT_1BL_U16(hi)
			    * (mlib_s32 *)dp = mmx_read_hi(hi8);
		}
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* ******************************************************************* */
#undef PREPARE_DELTAS
#define	PREPARE_DELTAS                                                     \
	if (warp_tbl != NULL) {                                            \
	    dX = warp_tbl[2 * j];                                          \
	    dY = warp_tbl[2 * j + 1];                                      \
	    dX = (dX - (dX >> 31)) & ~1;                                   \
/*                                                                         \
 * rounding towards ZERO                                                   \
 */                                                                        \
	    dY = (dY - (dY >> 31)) & ~1;                                   \
/*                                                                         \
 * rounding towards ZERO                                                   \
 */                                                                        \
	    dx64 =                                                         \
		mmx_to_double_dup(((dX & 0xFFFE) << 15) | ((dX & 0xFFFE)   \
		>> 1));                                                    \
	    dy64 =                                                         \
		mmx_to_double_dup(((dY & 0xFFFE) << 15) | ((dY & 0xFFFE)   \
		>> 1));                                                    \
	}
/* ******************************************************************* */

mlib_status
mlib_ImageAffine_u16_3ch_bl(
    mlib_affine_param *param)
{
	DECLAREVAR_BL();
	__m64 dx64, dy64;
	__m64 mask_7fff = mmx_to_double_dup(0x7FFF7FFF);
	__m64 mask_7ffe = mmx_to_double_dup(0x7FFE7FFE);
	__m64 mask_8000 = mmx_to_double_dup(0x80008000);

/* rounding towards ZERO */
	dX = (dX - (dX >> 31)) & ~1;
/* rounding towards ZERO */
	dY = (dY - (dY >> 31)) & ~1;

	dx64 = mmx_to_double_dup(((dX & 0xFFFE) << 15) | ((dX & 0xFFFE) >> 1));
	dy64 = mmx_to_double_dup(((dY & 0xFFFE) << 15) | ((dY & 0xFFFE) >> 1));

	for (j = yStart; j <= yFinish; j++) {
		__m64 ul0, ur0, ll0, lr0;
		__m64 hi_deltax, hi_deltay, hi_delta1_x, hi_delta1_y;
		__m64 hi8;
		mlib_s32 cols, filterpos, tail;
		mlib_s32 x0, y0;
		mlib_s32 i;

		CLIP(3);

		cols = xRight - xLeft + 1;
		tail = dstPixelPtr[3 * cols];
		x0 = X;
		y0 = Y;

		hi_deltax =
		    mmx_to_double_dup(((x0 & 0xFFFE) << 15) | ((x0 & 0xFFFE) >>
		    1));
		hi_deltay =
		    mmx_to_double_dup(((y0 & 0xFFFE) << 15) | ((y0 & 0xFFFE) >>
		    1));

		hi_delta1_x = _mm_sub_pi16(mask_7ffe, hi_deltax);
		hi_delta1_y = _mm_sub_pi16(mask_7ffe, hi_deltay);

		if (cols != 0) {

			NEXT_PIXELS_3BL_S16 for (i = 0; i < cols - 1; i++) {
				LOAD_3BL_S16
				    NEXT_PIXELS_3BL_S16 RESULT_1BL_U16(hi)
				    * (__m64 *) dstPixelPtr = hi8;
				dstPixelPtr += 3;
			}
			LOAD_3BL_S16 RESULT_1BL_U16(hi)
			    * (__m64 *) dstPixelPtr = hi8;
			dstPixelPtr += 3;
		}

		dstPixelPtr[0] = tail;
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageAffine_u16_4ch_bl(
    mlib_affine_param *param)
{

	DECLAREVAR_BL();
	__m64 dx64, dy64;
	__m64 mask_7fff = mmx_to_double_dup(0x7FFF7FFF);
	__m64 mask_7ffe = mmx_to_double_dup(0x7FFE7FFE);
	__m64 mask_8000 = mmx_to_double_dup(0x80008000);

/* rounding towards ZERO */
	dX = (dX - (dX >> 31)) & ~1;
/* rounding towards ZERO */
	dY = (dY - (dY >> 31)) & ~1;

	dx64 = mmx_to_double_dup(((dX & 0xFFFE) << 15) | ((dX & 0xFFFE) >> 1));
	dy64 = mmx_to_double_dup(((dY & 0xFFFE) << 15) | ((dY & 0xFFFE) >> 1));

	for (j = yStart; j <= yFinish; j++) {
		__m64 *dp;
		__m64 ul0, ur0, ll0, lr0;
		__m64 hi_deltax, hi_deltay, hi_delta1_x, hi_delta1_y;
		__m64 hi8;
		mlib_s32 cols, filterpos;
		mlib_s32 x0, y0;
		mlib_s32 i;

		CLIP(4);

		cols = xRight - xLeft + 1;
		dp = (__m64 *) dstPixelPtr;
		x0 = X;
		y0 = Y;

		hi_deltax =
		    mmx_to_double_dup(((x0 & 0xFFFE) << 15) | ((x0 & 0xFFFE) >>
		    1));
		hi_deltay =
		    mmx_to_double_dup(((y0 & 0xFFFE) << 15) | ((y0 & 0xFFFE) >>
		    1));

		hi_delta1_x = _mm_sub_pi16(mask_7ffe, hi_deltax);
		hi_delta1_y = _mm_sub_pi16(mask_7ffe, hi_deltay);

		if (cols != 0) {

			NEXT_PIXELS_4BL_S16 for (i = 0; i < cols - 1; i++) {
				LOAD_4BL_S16_AL RESULT_1BL_U16(hi)
				    dp[i] = hi8;
			NEXT_PIXELS_4BL_S16}
			LOAD_4BL_S16_AL RESULT_1BL_U16(hi)
			    dp[i] = hi8;
		}
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  MLIB_TYPE
#define	MLIB_TYPE	mlib_s32

/* *********************************************************** */

#undef  PREPARE_DELTAS
#define	PREPARE_DELTAS                                          \
	if (warp_tbl != NULL) {                                 \
	    dX = warp_tbl[2 * j];                               \
	    dY = warp_tbl[2 * j + 1];                           \
	}

/* *********************************************************** */

mlib_status
mlib_ImageAffine_s32_1ch_bl(
    mlib_affine_param *param)
{
	DECLAREVAR_BL();
	DTYPE *dstLineEnd;
	mlib_d64 scale = 1. / 65536.;
	mlib_s32 srcYStride1 = srcYStride + 4;

	srcYStride >>= 2;
	srcYStride1 >>= 2;
	for (j = yStart; j <= yFinish; j++) {
		mlib_d64 t, u, k0, k1, k2, k3;
		mlib_d64 a00_0, a01_0, a10_0, a11_0;
		mlib_d64 pix0;

		CLIP(1);
		dstLineEnd = (DTYPE *) dstData + xRight;

		t = (X & MLIB_MASK) * scale;
		u = (Y & MLIB_MASK) * scale;
		ySrc = MLIB_POINTER_SHIFT(Y);
		Y += dY;
		xSrc = X >> MLIB_SHIFT;
		X += dX;
		srcPixelPtr = MLIB_POINTER_GET(lineAddr, ySrc) + xSrc;
		k3 = t * u;
		k2 = (1.0 - t) * u;
		k1 = t * (1.0 - u);
		k0 = (1.0 - t) * (1.0 - u);
		a00_0 = srcPixelPtr[0];
		a01_0 = srcPixelPtr[1];
		a10_0 = srcPixelPtr[srcYStride];
		a11_0 = srcPixelPtr[srcYStride1];

		for (; dstPixelPtr < dstLineEnd; dstPixelPtr++) {
			mlib_s32 t0;

			pix0 =
			    k0 * a00_0 + k1 * a01_0 + k2 * a10_0 + k3 * a11_0;
			t = (X & MLIB_MASK) * scale;
			u = (Y & MLIB_MASK) * scale;
			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			srcPixelPtr =
			    *(MLIB_TYPE **) ((mlib_u8 *)lineAddr + ySrc) + xSrc;
			t0 = (mlib_s32)(pix0);
			k3 = t * u;
			k2 = (1.0 - t) * u;
			k1 = t * (1.0 - u);
			k0 = (1.0 - t) * (1.0 - u);
			a00_0 = srcPixelPtr[0];
			a01_0 = srcPixelPtr[1];
			a10_0 = srcPixelPtr[srcYStride];
			a11_0 = srcPixelPtr[srcYStride1];
			dstPixelPtr[0] = t0;
		}
		pix0 = k0 * a00_0 + k1 * a01_0 + k2 * a10_0 + k3 * a11_0;
		dstPixelPtr[0] = (mlib_s32)(pix0);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageAffine_s32_2ch_bl(
    mlib_affine_param *param)
{
	DECLAREVAR_BL();
	DTYPE *dstLineEnd;
	mlib_d64 scale = 1. / 65536.;

	for (j = yStart; j <= yFinish; j++) {
		MLIB_TYPE *srcPixelPtr2;
		mlib_d64 t, u, k0, k1, k2, k3;
		mlib_d64 a00_0, a01_0, a10_0, a11_0;
		mlib_d64 a00_1, a01_1, a10_1, a11_1;
		mlib_d64 pix0, pix1;

		CLIP(2);
		dstLineEnd = (DTYPE *) dstData + 2 * xRight;

		t = (X & MLIB_MASK) * scale;
		u = (Y & MLIB_MASK) * scale;
		ySrc = MLIB_POINTER_SHIFT(Y);
		Y += dY;
		xSrc = X >> MLIB_SHIFT;
		X += dX;
		srcPixelPtr = MLIB_POINTER_GET(lineAddr, ySrc) + 2 * xSrc;
		srcPixelPtr2 =
		    (MLIB_TYPE *) ((mlib_u8 *)srcPixelPtr + srcYStride);
		k3 = t * u;
		k2 = (1.0 - t) * u;
		k1 = t * (1.0 - u);
		k0 = (1.0 - t) * (1.0 - u);
		a00_0 = srcPixelPtr[0];
		a00_1 = srcPixelPtr[1];
		a01_0 = srcPixelPtr[2];
		a01_1 = srcPixelPtr[3];
		a10_0 = srcPixelPtr2[0];
		a10_1 = srcPixelPtr2[1];
		a11_0 = srcPixelPtr2[2];
		a11_1 = srcPixelPtr2[3];

		for (; dstPixelPtr < dstLineEnd; dstPixelPtr += 2) {
			mlib_s32 t0, t1;

			pix0 =
			    k0 * a00_0 + k1 * a01_0 + k2 * a10_0 + k3 * a11_0;
			pix1 =
			    k0 * a00_1 + k1 * a01_1 + k2 * a10_1 + k3 * a11_1;
			t = (X & MLIB_MASK) * scale;
			u = (Y & MLIB_MASK) * scale;
			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			srcPixelPtr =
			    MLIB_POINTER_GET(lineAddr, ySrc) + 2 * xSrc;
			srcPixelPtr2 =
			    (MLIB_TYPE *) ((mlib_u8 *)srcPixelPtr + srcYStride);
			k3 = t * u;
			k2 = (1.0 - t) * u;
			k1 = t * (1.0 - u);
			k0 = (1.0 - t) * (1.0 - u);
			a01_0 = srcPixelPtr[2];
			a01_1 = srcPixelPtr[3];
			t0 = (mlib_s32)(pix0);
			t1 = (mlib_s32)(pix1);
			a00_0 = srcPixelPtr[0];
			a00_1 = srcPixelPtr[1];
			a10_0 = srcPixelPtr2[0];
			a10_1 = srcPixelPtr2[1];
			a11_0 = srcPixelPtr2[2];
			a11_1 = srcPixelPtr2[3];
			dstPixelPtr[0] = t0;
			dstPixelPtr[1] = t1;
		}

		pix0 = k0 * a00_0 + k1 * a01_0 + k2 * a10_0 + k3 * a11_0;
		pix1 = k0 * a00_1 + k1 * a01_1 + k2 * a10_1 + k3 * a11_1;
		dstPixelPtr[0] = (mlib_s32)(pix0);
		dstPixelPtr[1] = (mlib_s32)(pix1);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageAffine_s32_3ch_bl(
    mlib_affine_param *param)
{
	DECLAREVAR_BL();
	DTYPE *dstLineEnd;
	mlib_d64 scale = 1. / 65536.;

	for (j = yStart; j <= yFinish; j++) {
		MLIB_TYPE *srcPixelPtr2;
		mlib_d64 t, u, k0, k1, k2, k3;
		mlib_d64 a00_0, a01_0, a10_0, a11_0;
		mlib_d64 a00_1, a01_1, a10_1, a11_1;
		mlib_d64 a00_2, a01_2, a10_2, a11_2;
		mlib_d64 pix0, pix1, pix2;

		CLIP(3);
		dstLineEnd = (DTYPE *) dstData + 3 * xRight;

		t = (X & MLIB_MASK) * scale;
		u = (Y & MLIB_MASK) * scale;
		ySrc = MLIB_POINTER_SHIFT(Y);
		Y += dY;
		xSrc = X >> MLIB_SHIFT;
		X += dX;
		srcPixelPtr = MLIB_POINTER_GET(lineAddr, ySrc) + 3 * xSrc;
		srcPixelPtr2 =
		    (MLIB_TYPE *) ((mlib_u8 *)srcPixelPtr + srcYStride);
		k3 = t * u;
		k2 = (1.0 - t) * u;
		k1 = t * (1.0 - u);
		k0 = (1.0 - t) * (1.0 - u);
		a00_0 = srcPixelPtr[0];
		a00_1 = srcPixelPtr[1];
		a00_2 = srcPixelPtr[2];
		a01_0 = srcPixelPtr[3];
		a01_1 = srcPixelPtr[4];
		a01_2 = srcPixelPtr[5];
		a10_0 = srcPixelPtr2[0];
		a10_1 = srcPixelPtr2[1];
		a10_2 = srcPixelPtr2[2];
		a11_0 = srcPixelPtr2[3];
		a11_1 = srcPixelPtr2[4];
		a11_2 = srcPixelPtr2[5];

		for (; dstPixelPtr < dstLineEnd; dstPixelPtr += 3) {
			mlib_s32 t0, t1, t2;

			pix0 =
			    k0 * a00_0 + k1 * a01_0 + k2 * a10_0 + k3 * a11_0;
			pix1 =
			    k0 * a00_1 + k1 * a01_1 + k2 * a10_1 + k3 * a11_1;
			pix2 =
			    k0 * a00_2 + k1 * a01_2 + k2 * a10_2 + k3 * a11_2;
			t = (X & MLIB_MASK) * scale;
			u = (Y & MLIB_MASK) * scale;
			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> MLIB_SHIFT;
			X += dX;
			srcPixelPtr =
			    MLIB_POINTER_GET(lineAddr, ySrc) + 3 * xSrc;
			srcPixelPtr2 =
			    (MLIB_TYPE *) ((mlib_u8 *)srcPixelPtr + srcYStride);
			k3 = t * u;
			k2 = (1.0 - t) * u;
			k1 = t * (1.0 - u);
			k0 = (1.0 - t) * (1.0 - u);
			a01_0 = srcPixelPtr[3];
			a01_1 = srcPixelPtr[4];
			a01_2 = srcPixelPtr[5];
			t0 = (mlib_s32)(pix0);
			t1 = (mlib_s32)(pix1);
			t2 = (mlib_s32)(pix2);
			a00_0 = srcPixelPtr[0];
			a00_1 = srcPixelPtr[1];
			a00_2 = srcPixelPtr[2];
			a10_0 = srcPixelPtr2[0];
			a10_1 = srcPixelPtr2[1];
			a10_2 = srcPixelPtr2[2];
			a11_0 = srcPixelPtr2[3];
			a11_1 = srcPixelPtr2[4];
			a11_2 = srcPixelPtr2[5];
			dstPixelPtr[0] = t0;
			dstPixelPtr[1] = t1;
			dstPixelPtr[2] = t2;
		}

		pix0 = k0 * a00_0 + k1 * a01_0 + k2 * a10_0 + k3 * a11_0;
		pix1 = k0 * a00_1 + k1 * a01_1 + k2 * a10_1 + k3 * a11_1;
		pix2 = k0 * a00_2 + k1 * a01_2 + k2 * a10_2 + k3 * a11_2;
		dstPixelPtr[0] = (mlib_s32)(pix0);
		dstPixelPtr[1] = (mlib_s32)(pix1);
		dstPixelPtr[2] = (mlib_s32)(pix2);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageAffine_s32_4ch_bl(
    mlib_affine_param *param)
{
	DECLAREVAR_BL();
	DTYPE *dstLineEnd;
	mlib_d64 scale = 1. / 65536.;

	for (j = yStart; j <= yFinish; j++) {
		MLIB_TYPE *srcPixelPtr2;
		mlib_d64 t, u, k0, k1, k2, k3;
		mlib_d64 a00_0, a01_0, a10_0, a11_0;
		mlib_d64 a00_1, a01_1, a10_1, a11_1;
		mlib_d64 a00_2, a01_2, a10_2, a11_2;
		mlib_d64 a00_3, a01_3, a10_3, a11_3;
		mlib_d64 pix0, pix1, pix2, pix3;

		CLIP(4);
		dstLineEnd = (DTYPE *) dstData + 4 * xRight;

		t = (X & MLIB_MASK) * scale;
		u = (Y & MLIB_MASK) * scale;
		ySrc = MLIB_POINTER_SHIFT(Y);
		Y += dY;
		xSrc = X >> MLIB_SHIFT;
		X += dX;
		srcPixelPtr = MLIB_POINTER_GET(lineAddr, ySrc) + 4 * xSrc;
		srcPixelPtr2 =
		    (MLIB_TYPE *) ((mlib_u8 *)srcPixelPtr + srcYStride);
		k3 = t * u;
		k2 = (1.0 - t) * u;
		k1 = t * (1.0 - u);
		k0 = (1.0 - t) * (1.0 - u);
		a00_0 = srcPixelPtr[0];
		a00_1 = srcPixelPtr[1];
		a00_2 = srcPixelPtr[2];
		a00_3 = srcPixelPtr[3];
		a01_0 = srcPixelPtr[4];
		a01_1 = srcPixelPtr[5];
		a01_2 = srcPixelPtr[6];
		a01_3 = srcPixelPtr[7];
		a10_0 = srcPixelPtr2[0];
		a10_1 = srcPixelPtr2[1];
		a10_2 = srcPixelPtr2[2];
		a10_3 = srcPixelPtr2[3];
		a11_0 = srcPixelPtr2[4];
		a11_1 = srcPixelPtr2[5];
		a11_2 = srcPixelPtr2[6];
		a11_3 = srcPixelPtr2[7];

		for (; dstPixelPtr < dstLineEnd; dstPixelPtr += 4) {
			mlib_s32 t0, t1, t2, t3;

			pix0 =
			    k0 * a00_0 + k1 * a01_0 + k2 * a10_0 + k3 * a11_0;
			pix1 =
			    k0 * a00_1 + k1 * a01_1 + k2 * a10_1 + k3 * a11_1;
			pix2 =
			    k0 * a00_2 + k1 * a01_2 + k2 * a10_2 + k3 * a11_2;
			pix3 =
			    k0 * a00_3 + k1 * a01_3 + k2 * a10_3 + k3 * a11_3;
			t = (X & MLIB_MASK) * scale;
			u = (Y & MLIB_MASK) * scale;
			ySrc = MLIB_POINTER_SHIFT(Y);
			Y += dY;
			xSrc = X >> (MLIB_SHIFT - 2);
			X += dX;
			srcPixelPtr =
			    MLIB_POINTER_GET(lineAddr, ySrc) + (xSrc & ~3);
			srcPixelPtr2 =
			    (MLIB_TYPE *) ((mlib_u8 *)srcPixelPtr + srcYStride);
			k3 = t * u;
			k2 = (1.0 - t) * u;
			k1 = t * (1.0 - u);
			k0 = (1.0 - t) * (1.0 - u);
			a00_3 = srcPixelPtr[3];
			a01_3 = srcPixelPtr[7];
			a10_3 = srcPixelPtr2[3];
			a11_3 = srcPixelPtr2[7];
			t0 = (mlib_s32)(pix0);
			t1 = (mlib_s32)(pix1);
			t2 = (mlib_s32)(pix2);
			t3 = (mlib_s32)(pix3);
			a00_0 = srcPixelPtr[0];
			a00_1 = srcPixelPtr[1];
			a00_2 = srcPixelPtr[2];
			a01_0 = srcPixelPtr[4];
			a01_1 = srcPixelPtr[5];
			a01_2 = srcPixelPtr[6];
			a10_0 = srcPixelPtr2[0];
			a10_1 = srcPixelPtr2[1];
			a10_2 = srcPixelPtr2[2];
			a11_0 = srcPixelPtr2[4];
			a11_1 = srcPixelPtr2[5];
			a11_2 = srcPixelPtr2[6];
			dstPixelPtr[0] = t0;
			dstPixelPtr[1] = t1;
			dstPixelPtr[2] = t2;
			dstPixelPtr[3] = t3;
		}

		pix0 = k0 * a00_0 + k1 * a01_0 + k2 * a10_0 + k3 * a11_0;
		pix1 = k0 * a00_1 + k1 * a01_1 + k2 * a10_1 + k3 * a11_1;
		pix2 = k0 * a00_2 + k1 * a01_2 + k2 * a10_2 + k3 * a11_2;
		pix3 = k0 * a00_3 + k1 * a01_3 + k2 * a10_3 + k3 * a11_3;
		dstPixelPtr[0] = (mlib_s32)pix0;
		dstPixelPtr[1] = (mlib_s32)pix1;
		dstPixelPtr[2] = (mlib_s32)pix2;
		dstPixelPtr[3] = (mlib_s32)pix3;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
