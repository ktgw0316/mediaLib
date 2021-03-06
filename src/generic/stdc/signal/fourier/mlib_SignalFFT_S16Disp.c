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

#pragma ident	"@(#)mlib_SignalFFT_S16Disp.c	9.3	07/10/09 SMI"

/* *********************************************************** */

#include <mlib_signal.h>
#include <mlib_SysMath.h>	/* for fabs */

#define	FFT_UTIL_C

#include <mlib_SignalFFTUtil.h>

/* *********************************************************** */

/* scale to transform ( -7fff, 7fff ) -> ( -.999..., .999... ) */
#define	Q15SCALE	(- MLIB_S16_MIN)

/* *********************************************************** */

/* windowing macro */
#define	MULT_WIN(src, wnd)	(((deal_t)(src)) * ((deal_t)(wnd)))

/* *********************************************************** */

/* read into deal buffer macros */
#define	LOAD_C_W(dst, src_main, src_aux, wnd, i_src, mult)                \
	{                                                                 \
	    mlib_s32 tmp = wnd[i_src];                                    \
	                                                                  \
	    dst##_r =                                                     \
		mult * (deal_t)(MULT_WIN(tmp, src_main[2 * (i_src)]));    \
	    dst##_i =                                                     \
		mult * (deal_t)(MULT_WIN(tmp,                             \
		src_main[2 * (i_src) + 1]));                              \
	}

/* *********************************************************** */

#define	LOAD_C(dst, src_main, src_aux, wnd, i_src, mult)        \
	dst##_r = (deal_t)(src_main[2 * (i_src)]);              \
	dst##_i = (deal_t)(src_main[2 * (i_src) + 1])

/* *********************************************************** */

#define	LOAD_RI_W(dst, src_main, src_aux, wnd, i_src, mult)         \
	{                                                           \
	    mlib_s32 tmp = wnd[i_src];                              \
	                                                            \
	    dst##_r =                                               \
		mult * (deal_t)(MULT_WIN(tmp, src_main[i_src]));    \
	    dst##_i =                                               \
		mult * (deal_t)(MULT_WIN(tmp, src_aux[i_src]));     \
	}

/* *********************************************************** */

#define	LOAD_RI(dst, src_main, src_aux, wnd, i_src, mult)       \
	dst##_r = (deal_t)(src_main[i_src]);                    \
	dst##_i = (deal_t)(src_aux[i_src])

/* *********************************************************** */

#define	LOAD_R_W(dst, src_main, src_aux, wnd, i_src, mult)      \
	dst##_r =                                               \
	    mult * (deal_t)(MULT_WIN(wnd[2 * (i_src)],          \
	    src_main[2 * (i_src)]));                            \
	dst##_i =                                               \
	    mult * (deal_t)(MULT_WIN(wnd[2 * (i_src) + 1],      \
	    src_main[2 * (i_src) + 1]))

/* *********************************************************** */

#define	LOAD_R(dst, src_main, src_aux, wnd, i_src, mult)        \
	LOAD_C(dst, src_main, src_aux, wnd, i_src, mult)

/* *********************************************************** */

#define	LOAD_C_4_W(dst, src_main, src_aux, wnd, i_src, mult)    \
	LOAD_C_W(dst, src_main, src_aux, wnd, i_src, mult)

/* *********************************************************** */

#define	LOAD_C_4(dst, src_main, src_aux, wnd, i_src, mult)      \
	dst##_r = mult * src_main[2 * (i_src)];                 \
	dst##_i = mult * src_main[2 * (i_src) + 1]

/* *********************************************************** */

#define	LOAD_RI_4_W(dst, src_main, src_aux, wnd, i_src, mult)   \
	LOAD_RI_W(dst, src_main, src_aux, wnd, i_src, mult)

/* *********************************************************** */

#define	LOAD_RI_4(dst, src_main, src_aux, wnd, i_src, mult)     \
	dst##_r = mult * src_main[i_src];                       \
	dst##_i = mult * src_aux[i_src]

/* *********************************************************** */

#define	LOAD_R_4_W(dst, src_main, src_aux, wnd, i_src, mult)    \
	LOAD_R_W(dst, src_main, src_aux, wnd, i_src, mult)

/* *********************************************************** */

#define	LOAD_R_4(dst, src_main, src_aux, wnd, i_src, mult)      \
	LOAD_C_4(dst, src_main, src_aux, wnd, i_src, mult)

/* *********************************************************** */

/* special for 1st-3layers */

/* *********************************************************** */

#define	BUTTERFLY_CMPLX_CMPLX_ROT(dest1, dest2, rot2)           \
	BUTTERFLY_CMPLX_CMPLX(dest1, dest2, dest1, rot2)

/* *********************************************************** */

/*
 *    declares and inits FOUR ptrs to dtCmplx from p_beg shifted by 1
 */
#undef DCL_4_CMPLX_PTRS
#define	DCL_4_CMPLX_PTRS(tag, ptr_beg)                          \
	dtCmplx *CONST tag##_0 = ptr_beg, *CONST tag##_1 =      \
	    tag##_0 + 1, *CONST tag##_2 =                       \
	    tag##_0 + 2, *CONST tag##_3 = tag##_0 + 3

/* *********************************************************** */

/* declare/define components for simple butterflies */
#undef DCL_CALC_SIMPLE_BUTT
#define	DCL_CALC_SIMPLE_BUTT(dst, src, iSIGNed)                 \
	CONST deal_t dst##_0_pls_1_r =                          \
	    src##_0_r + src##_1_r, dst##_0_pls_1_i =            \
	    src##_0_i + src##_1_i, dst##_0_mns_1_r =            \
	    src##_0_r - src##_1_r, dst##_0_mns_1_i =            \
	    src##_0_i - src##_1_i, dst##_2_pls_3_r =            \
	    src##_2_r + src##_3_r, dst##_2_pls_3_i =            \
	    src##_2_i + src##_3_i, dst##_2_mns_3_r =            \
	iSIGNed(src##_3_i - src##_2_i), dst##_2_mns_3_i =       \
	iSIGNed(src##_2_r - src##_3_r)

/* *********************************************************** */

/* sequences to perform simple butt on 4 complex elems */
#define	DEAL_4_2LAYS(ptr_beg, d0, d1, iSIGNed)                  \
	{                                                       \
	    DCL_CALC_SIMPLE_BUTT(c0, d0, iSIGNed);              \
	    DCL_4_CMPLX_PTRS(d0, ptr_beg);                      \
	    STORE_SIMPLE_BUTTERFLIES(d0, c0);                   \
	}

/* *********************************************************** */

/* calc components of simple butterfly for 1st-3-layers */
#define	CALC_INTERMED_BUTT(dest1, dest2, src1, src2)            \
	dest1##_r = src1##_r + src2##_r, dest1##_i =            \
	    src1##_i + src2##_i, dest2##_r =                    \
	    src1##_r - src2##_r, dest2##_i =                    \
	    src1##_i - src2##_i

/* *********************************************************** */

/* calc components of 2 simple butterflies for 1st-3-layers */
#define	CALC_SIMPLE_BUTTS(dst, src)                             \
	CALC_INTERMED_BUTT(dst##_0, dst##_2, src##_0_pls_1,     \
	    src##_2_pls_3);                                     \
	CALC_INTERMED_BUTT(dst##_1, dst##_3, src##_0_mns_1,     \
	    src##_2_mns_3)

/* *********************************************************** */

/* declare/define components for 1st-3-layers butterflies */
#define	DCL_CALC_3RD_LAY_BUTT(src, iSIGNed)                     \
	CONST deal_t src##_0r_r = src##_0_r, src##_0r_i =       \
	    src##_0_i, src##_1r_r =                             \
	    SIN_PI_BY4 * (src##_1_r - iSIGNed(src##_1_i)),      \
	    src##_1r_i =                                        \
	    SIN_PI_BY4 * (src##_1_i + iSIGNed(src##_1_r)),      \
	    src##_2r_r = -iSIGNed(src##_2_i), src##_2r_i =      \
	iSIGNed(src##_2_r), src##_3r_r =                        \
	    (-SIN_PI_BY4) * (iSIGNed(src##_3_i) + src##_3_r),   \
	    src##_3r_i =                                        \
	    SIN_PI_BY4 * (iSIGNed(src##_3_r) - src##_3_i)

/* *********************************************************** */

/* perform & store 1st-3-layers butterflies */
#define	STORE_3RD_LAY_BUTTERFLIES(src0, src1)                   \
	BUTTERFLY_CMPLX_CMPLX_ROT(src0##_0, src1##_0,           \
	    src1##_0r);                                         \
	BUTTERFLY_CMPLX_CMPLX_ROT(src0##_1, src1##_1,           \
	    src1##_1r);                                         \
	BUTTERFLY_CMPLX_CMPLX_ROT(src0##_2, src1##_2,           \
	    src1##_2r);                                         \
	BUTTERFLY_CMPLX_CMPLX_ROT(src0##_3, src1##_3,           \
	    src1##_3r)

/* *********************************************************** */

/* sequences to perform 1st-3layers butt on 8 sequent with step 1 */
#define	DEAL_8_3LAYS(ptr_beg, d0, d1, iSIGNed)                  \
	{                                                       \
	    DCL_CALC_SIMPLE_BUTT(c0, d0, iSIGNed);              \
	    DCL_CALC_SIMPLE_BUTT(c1, d1, iSIGNed);              \
	    CALC_SIMPLE_BUTTS(d0, c0);                          \
	    CALC_SIMPLE_BUTTS(d1, c1); {                        \
		DCL_4_CMPLX_PTRS(d0, ptr_beg);                  \
		DCL_4_CMPLX_PTRS(d1, d0_0 + 4);                 \
		DCL_CALC_3RD_LAY_BUTT(d1, iSIGNed);             \
		STORE_3RD_LAY_BUTTERFLIES(d0, d1);              \
	    }                                                   \
	}

/* *********************************************************** */

/* reversion at all orders using REV_INDX*, load_macro, deal_macro */
#define	LOOP_REV_3(leng, iSIGNed, load_macro)                            \
	{                                                                \
	    mlib_s32 CONST o_3 = order - 3, shift = leng - o_3;          \
	    mlib_s32 iii;                                                \
	    mlib_s32 rev_1 = 4 << o_3, rev_2 = 2 << o_3, rev_3 =         \
		6 << o_3, rev_4 = 1 << o_3, rev_5 = 5 << o_3, rev_6 =    \
		3 << o_3, rev_7 = 7 << o_3;                              \
	    for (iii = iii0; iii < iii1; iii += 8) {                     \
		mlib_s32 hgh = iii >> 3;                                 \
		mlib_s32 hgh_rev =                                       \
		    REV_INDX_##leng(hgh, shift), iii_0_rev =             \
		    hgh_rev, iii_1_rev = hgh_rev | rev_1, iii_2_rev =    \
		    hgh_rev | rev_2, iii_3_rev =                         \
		    hgh_rev | rev_3, iii_4_rev =                         \
		    hgh_rev | rev_4, iii_5_rev =                         \
		    hgh_rev | rev_5, iii_6_rev =                         \
		    hgh_rev | rev_6, iii_7_rev = hgh_rev | rev_7;        \
		deal_t d0_0_r, d0_0_i, d0_1_r, d0_1_i, d0_2_r, d0_2_i,   \
		    d0_3_r, d0_3_i, d1_0_r, d1_0_i, d1_1_r, d1_1_i,      \
		    d1_2_r, d1_2_i, d1_3_r, d1_3_i;                      \
		dtCmplx *CONST pbeg = (dtCmplx *) (dst) + iii;           \
	                                                                 \
		load_macro(d0_0, src_main, src_aux, wnd, iii_0_rev,      \
		    mult);                                               \
		load_macro(d0_1, src_main, src_aux, wnd, iii_1_rev,      \
		    mult);                                               \
		load_macro(d0_2, src_main, src_aux, wnd, iii_2_rev,      \
		    mult);                                               \
		load_macro(d0_3, src_main, src_aux, wnd, iii_3_rev,      \
		    mult);                                               \
		load_macro(d1_0, src_main, src_aux, wnd, iii_4_rev,      \
		    mult);                                               \
		load_macro(d1_1, src_main, src_aux, wnd, iii_5_rev,      \
		    mult);                                               \
		load_macro(d1_2, src_main, src_aux, wnd, iii_6_rev,      \
		    mult);                                               \
		load_macro(d1_3, src_main, src_aux, wnd, iii_7_rev,      \
		    mult);                                               \
		DEAL_8_3LAYS(pbeg, d0, d1, iSIGNed);                     \
	    }                                                            \
	}

/* *********************************************************** */

#define	LOOP_REV_2(leng, iSIGNed, load_macro)                            \
	{                                                                \
	    mlib_s32 CONST o_2 = order - 2, shift = leng - o_2;          \
	    mlib_s32 iii;                                                \
	    mlib_s32 rev_1 = 2 << o_2, rev_2 = 1 << o_2, rev_3 =         \
		3 << o_2;                                                \
	    for (iii = iii0; iii < iii1; iii += 4) {                     \
		mlib_s32 hgh = iii >> 2;                                 \
		mlib_s32 hgh_rev =                                       \
		    REV_INDX_##leng(hgh, shift), iii_0_rev =             \
		    hgh_rev, iii_1_rev = hgh_rev | rev_1, iii_2_rev =    \
		    hgh_rev | rev_2, iii_3_rev = hgh_rev | rev_3;        \
		deal_t d0_0_r, d0_0_i, d0_1_r, d0_1_i, d0_2_r, d0_2_i,   \
		    d0_3_r, d0_3_i;                                      \
		dtCmplx *CONST pbeg = (dtCmplx *) (dst) + iii;           \
	                                                                 \
		load_macro(d0_0, src_main, src_aux, wnd, iii_0_rev,      \
		    mult);                                               \
		load_macro(d0_1, src_main, src_aux, wnd, iii_1_rev,      \
		    mult);                                               \
		load_macro(d0_2, src_main, src_aux, wnd, iii_2_rev,      \
		    mult);                                               \
		load_macro(d0_3, src_main, src_aux, wnd, iii_3_rev,      \
		    mult);                                               \
		DEAL_4_2LAYS(pbeg, d0, d1, iSIGNed);                     \
	    }                                                            \
	}

/* *********************************************************** */

/* perform bit reversion at order 2 */
#define	DEAL_ALL_ORD_2(iSIGNed, load_macro)                          \
	{                                                            \
	    deal_t d0_0_r, d0_0_i, d0_1_r, d0_1_i, d0_2_r, d0_2_i,   \
		d0_3_r, d0_3_i;                                      \
	    load_macro(d0_0, src_main, src_aux, wnd, 0, mult);       \
	    load_macro(d0_1, src_main, src_aux, wnd, 2, mult);       \
	    load_macro(d0_2, src_main, src_aux, wnd, 1, mult);       \
	    load_macro(d0_3, src_main, src_aux, wnd, 3, mult); {     \
		dtCmplx *CONST pbeg = (dtCmplx *) (dst);             \
	                                                             \
		DCL_4_CMPLX_PTRS(d0, pbeg);                          \
		DCL_CALC_SIMPLE_BUTT(c0, d0, iSIGNed);               \
		STORE_SIMPLE_BUTTERFLIES(d0, c0);                    \
	    }                                                        \
	}

/* *********************************************************** */

#define	GET_NUM_FIRST(first3, order)                            \
	first3 = order;                                         \
	while (first3 > CACHE1_ORD)                             \
	    first3 -= 3;                                        \
	first3 &= 1

/* *********************************************************** */

#define	GET_NUM_FIRST_4(first3, order)                                    \
	GET_NUM_FIRST(first3, (order > CACHE1_ORD ? order - 2 : order))

/* *********************************************************** */

/* perform bit reversion at all orders regarding order */
#define	LOAD_BIT_REV_1(leng, iSIGNed, load_macro)               \
	{                                                       \
	    if (order == 2) {                                   \
		DEAL_ALL_ORD_2(iSIGNed, load_macro);            \
	    } else if (first3) {                                \
		LOOP_REV_3(leng, iSIGNed, load_macro);          \
	    } else {                                            \
		LOOP_REV_2(leng, iSIGNed, load_macro);          \
	    }                                                   \
	}

/* *********************************************************** */

/* perform bit reversion at all orders regarding wnd */
#define	LOAD_BIT_REV(leng, iSIGNed, load_macro)                  \
	{                                                        \
	    if (wnd) {                                           \
		LOAD_BIT_REV_1(leng, iSIGNed, load_macro##_W);   \
	    } else {                                             \
		LOAD_BIT_REV_1(leng, iSIGNed, load_macro);       \
	    }                                                    \
	}

/* *********************************************************** */

/* things for *_4_S16C_S16 */
#define	ST_ORS(smth)	(all_ors |= (mlib_s32)mlib_fabs(smth))

/* *********************************************************** */

#define	GET_FUNC(REV_NBITS)                                                \
	void                                                               \
	mlib_c_fftDW_GetData1_##REV_NBITS##_4(deal_t *CONST dst,           \
	    mlib_s16 *CONST src_main, mlib_s16 *CONST src_aux,             \
	    mlib_s16 *CONST wnd, mlib_s32 CONST order,                     \
	    mlib_s32 CONST data_info_flags, mlib_s32 iii0, mlib_s32 iii1)  \
	{                                                                  \
	                                                                   \
/*                                                                         \
 * perform division by NN                                                  \
 */                                                                        \
	    CONST mlib_d64 mult =                                          \
		(1. / (1 << order)) / (wnd ? (mlib_d64)(Q15SCALE) : 1.);   \
	    mlib_s32 first3;                                               \
	                                                                   \
	    GET_NUM_FIRST_4(first3, order);                                \
/*                                                                         \
 * initialise deal buffer & serve window                                   \
 */                                                                        \
	    if (data_info_flags & eInversFFT) {                            \
		if (data_info_flags & eSrcCmplx) {                         \
		    LOAD_BIT_REV(REV_NBITS, AS_IS, LOAD_C_4);              \
		} else if (data_info_flags & eSrcReIm) {                   \
		    LOAD_BIT_REV(REV_NBITS, AS_IS, LOAD_RI_4);             \
		}                                                          \
	                                                                   \
/*                                                                         \
 * if( data_info_flags & eSrcRe ) => no Inverse!                           \
 */                                                                        \
	    } else {                                                       \
		if (data_info_flags & eSrcCmplx) {                         \
		    LOAD_BIT_REV(REV_NBITS, MINUS, LOAD_C_4);              \
		} else if (data_info_flags & eSrcReIm) {                   \
		    LOAD_BIT_REV(REV_NBITS, MINUS, LOAD_RI_4);             \
		} else {                                                   \
	                                                                   \
/*                                                                         \
 * if( data_info_flags & eSrcRe )                                          \
 */                                                                        \
		    order--;                                               \
		    GET_NUM_FIRST(first3, order);                          \
		    LOAD_BIT_REV(REV_NBITS, AS_IS, LOAD_R_4);              \
		}                                                          \
	    }                                                              \
	}                                                                  \
	                                                                   \
/*                                                                         \
 * ************************************************************            \
 */                                                                        \
	void                                                               \
	mlib_c_fftDW_GetData1_##REV_NBITS(deal_t *CONST dst,               \
	    mlib_s16 *CONST src_main, mlib_s16 *CONST src_aux,             \
	    mlib_s16 *CONST wnd, mlib_s32 CONST order,                     \
	    mlib_s32 CONST scale, mlib_s32 CONST data_info_flags,          \
		mlib_s32 iii0, mlib_s32 iii1)                              \
	{                                                                  \
	    CONST mlib_d64 mult =                                          \
		wnd ? 1. / ((1 << (order -                                 \
		scale)) * (mlib_d64)(Q15SCALE)) : 1997.0;                  \
	    mlib_s32 first3;                                               \
	                                                                   \
	    GET_NUM_FIRST(first3, order);                                  \
/*                                                                         \
 * initialise deal buffer & serve window                                   \
 */                                                                        \
	    if (data_info_flags & eInversFFT) {                            \
		if (data_info_flags & eSrcCmplx) {                         \
		    LOAD_BIT_REV(REV_NBITS, AS_IS, LOAD_C);                \
		} else if (data_info_flags & eSrcReIm) {                   \
		    LOAD_BIT_REV(REV_NBITS, AS_IS, LOAD_RI);               \
		}                                                          \
	                                                                   \
/*                                                                         \
 * if( data_info_flags & eSrcRe ) => no Inverse!                           \
 */                                                                        \
	    } else {                                                       \
		if (data_info_flags & eSrcCmplx) {                         \
		    LOAD_BIT_REV(REV_NBITS, MINUS, LOAD_C);                \
		} else if (data_info_flags & eSrcReIm) {                   \
		    LOAD_BIT_REV(REV_NBITS, MINUS, LOAD_RI);               \
		} else {                                                   \
	                                                                   \
/*                                                                         \
 * if( data_info_flags & eSrcRe )                                          \
 */                                                                        \
		    order--;                                               \
		    GET_NUM_FIRST(first3, order);                          \
		    LOAD_BIT_REV(REV_NBITS, AS_IS, LOAD_R);                \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

GET_FUNC(8)
GET_FUNC(16)
GET_FUNC(24)
GET_FUNC(32)

/* *********************************************************** */

void
mlib_fft_S16C_S16_4_WR(
	mlib_s16 *dst_main,
	mlib_d64 *pal64,
	mlib_d64 mult,
	mlib_s32 length,
	mlib_s32 i0,
	mlib_s32 i1)
{
	mlib_s32 iii;
	for (iii = i0; iii < i1; iii += 2) {
		dst_main[2 * length - iii] = (dst_main[iii] =
			mult * pal64[iii]);
		dst_main[2 * length - iii + 1] = -(dst_main[iii + 1] =
			mult * pal64[iii + 1]);
	}
}

void
mlib_fft_S16C_S16_LL(
	mlib_s16 *dst_main,
	mlib_d64 *pal64,
	mlib_d64 mult,
	mlib_s32 order,
	mlib_s32 i0,
	mlib_s32 i1,
	mlib_d64 rot_r,
	mlib_d64 rot_i)
{
	mlib_s32 iii;
	mlib_s32 length = 1 << order;
	deal_t incr_r, incr_i, wtmp;
	wtmp = SINUS(order);
	incr_r = 1. - 2. * wtmp * wtmp;
	incr_i = SINUS(order - 1);
	for (iii = i0; iii < i1; iii++) {
		mlib_s32 i1 = 2 * iii, i2 = i1 + 1;
		mlib_s32 i3 = length - i1, i4 = i3 + 1;
		mlib_s32 i5 = 2 * length - 2 * iii,
		i6 = 2 * length - 2 * iii + 1,
		i7 = 2 * length - length + 2 * iii,
		i8 = 2 * length - (length - 2 * iii) + 1;
		deal_t d1_r = pal64[i1], d1_i = pal64[i2];
		deal_t d2_r = pal64[i3], d2_i = -pal64[i4];
		deal_t h1r, h1i, h2r, h2i, h2rot_r, h2rot_i;
		h1r = (d1_r + d2_r);
		h1i = (d1_i + d2_i);
		h2r = (d1_i - d2_i);
		h2i = -(d1_r - d2_r);
		h2rot_r = rot_r * h2r - rot_i * h2i;
		h2rot_i = rot_r * h2i + rot_i * h2r;
		dst_main[i5] = dst_main[i1] = mult * .5 * (h1r + h2rot_r);
		dst_main[i6] = -(dst_main[i2] = mult * -.5 * (h1i + h2rot_i));
		dst_main[i7] = dst_main[i3] = mult * .5 * (h1r - h2rot_r);
		dst_main[i8] = -(dst_main[i4] = mult * -.5 * (-h1i + h2rot_i));
		ROTATE_CMPLX_TMP(rot, incr, wtmp);
	}
}

mlib_s32
mlib_fft_S16C_S16_4_LL(
	mlib_d64 *pal64,
	mlib_s32 order,
	mlib_s32 i0,
	mlib_s32 i1,
	mlib_d64 rot_r,
	mlib_d64 rot_i)
{
	mlib_s32 iii;
	mlib_s32 length = 1 << order;
	mlib_s32 all_ors = 0;
	deal_t incr_r, incr_i, wtmp;

	wtmp = SINUS(order);
	incr_r = 1. - 2. * wtmp * wtmp;
	incr_i = SINUS(order - 1);

	for (iii = i0; iii < i1; iii++) {
		mlib_s32 i1 = 2 * iii, i2 = i1 + 1;
		mlib_s32 i3 = length - i1, i4 = i3 + 1;
		deal_t d1_r = pal64[i1], d1_i = pal64[i2];
		deal_t d2_r = pal64[i3], d2_i = -pal64[i4];
		deal_t h1r, h1i, h2r, h2i, h2rot_r, h2rot_i;
		h1r = (d1_r + d2_r);
		h1i = (d1_i + d2_i);
		h2r = (d1_i - d2_i);
		h2i = -(d1_r - d2_r);
		h2rot_r = rot_r * h2r - rot_i * h2i;
		h2rot_i = rot_r * h2i + rot_i * h2r;
		ST_ORS(pal64[i1] = .5 * (h1r + h2rot_r));
		ST_ORS(pal64[i2] = -.5 * (h1i + h2rot_i));
		ST_ORS(pal64[i3] = .5 * (h1r - h2rot_r));
		ST_ORS(pal64[i4] = -.5 * (-h1i + h2rot_i));
		ROTATE_CMPLX_TMP(rot, incr, wtmp);
	}
	return (all_ors);
}

void
mlib_fftDW_PutData_S16(
    deal_t *CONST pal64data,
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s32 CONST data_info_flags,
    mlib_s32 CONST bool_is_windowed,
    mlib_d64 scalemult,
    mlib_s32 i0,
    mlib_s32 i1)
{
	mlib_s32 i;

	/* saturate & put results into destination */

	if (bool_is_windowed) {
		if (data_info_flags & eDstCmplx) {
			for (i = i0; i < i1; i++) {
				dst_main[2 * i] = pal64data[2 * i];
				dst_main[2 * i + 1] =
				pal64data[2 * i + 1];
			}
		} else if (data_info_flags & eDstReIm) {
			for (i = i0; i < i1; i++) {
				dst_main[i] = pal64data[2 * i];
				dst_aux[i] = pal64data[2 * i + 1];
			}
		} else {
			for (i = i0; i < i1; i++)
				dst_main[i] = pal64data[2 * i];
		}
	} else {
		/* need scaling */
		if (data_info_flags & eDstCmplx) {
			for (i = i0; i < i1; i++) {
				dst_main[2 * i] =
					pal64data[2 * i] * scalemult;
				dst_main[2 * i + 1] =
					pal64data[2 * i + 1] *
					scalemult;
			}
		} else if (data_info_flags & eDstReIm) {
			for (i = i0; i < i1; i++) {
				dst_main[i] =
					pal64data[2 * i] * scalemult;
				dst_aux[i] =
					pal64data[2 * i + 1] *
					scalemult;
			}
		} else {
			for (i = i0; i < i1; i++)
				dst_main[i] =
					pal64data[2 * i] * scalemult;
		}
	}
}

/* *********************************************************** */
