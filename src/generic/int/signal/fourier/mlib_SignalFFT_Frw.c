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

#pragma ident	"@(#)mlib_SignalFFT_Frw.c	9.3	07/11/05 SMI"

/* *********************************************************** */

/* ifdef _NO_LONGLONG, use stdc source codes, else use int source codes */
#ifdef _NO_LONGLONG

/*
 * FUNCTIONS
 *   Internal functions for FFT layer processing
 *
 */

#include <mlib_signal.h>
#include <mlib_i_SignalFFTUtil.h>
#define	ISIGN DIRECT_FFT

/* *********************************************************** */

#if ISIGN == INVERS_FFT

#define	FUNC(NAME)	mlib_fft_##NAME##_I
#define	SSIGN(x)	AS_IS(x)

#else /* ISIGN == INVERS_FFT */

#define	FUNC(NAME)	mlib_fft_##NAME##_F
#define	SSIGN(x)	MINUS(x)

#endif /* ISIGN == INVERS_FFT */

/* *********************************************************** */

#define	NUM_SPECIAL_LAST	3

/* *********************************************************** */

/* sequence to perform simple butt on 8 complex elems sequent */
#define	DEAL_8_SIMPLE_SEQUENT(ptr_beg, tagstep, iSIGNed)        \
	DCL_4_CMPLX_PTRS(d0, ptr_beg, tagstep);                 \
	DCL_4_CMPLX_PTRS(d1, d0_0 + four_##tagstep, tagstep);   \
	DCL_CALC_SIMPLE_BUTT(c0, d0, iSIGNed);                  \
	DCL_CALC_SIMPLE_BUTT(c1, d1, iSIGNed);                  \
	STORE_SIMPLE_BUTTERFLIES(d0, c0);                       \
	STORE_SIMPLE_BUTTERFLIES(d1, c1)

/* *********************************************************** */

/* sequence to perform simple butt on 8+8 complex elems sequent */
#define	DEAL_8_8_SIMPLE_SEQUENT(ptr_beg, tagstep, iSIGNed)      \
	DEAL_8_SIMPLE_SEQUENT(ptr_beg, tagstep, iSIGNed)        \
	{                                                       \
	    DCL_4_CMPLX_PTRS(d2, d0_0 + two_four_##tagstep,     \
		tagstep);                                       \
	    DCL_4_CMPLX_PTRS(d3, d0_0 + three_four_##tagstep,   \
		tagstep);                                       \
	    DCL_CALC_SIMPLE_BUTT(c2, d2, iSIGNed);              \
	    DCL_CALC_SIMPLE_BUTT(c3, d3, iSIGNed);              \
	    STORE_SIMPLE_BUTTERFLIES(d2, c2);                   \
	    STORE_SIMPLE_BUTTERFLIES(d3, c3);                   \
	}

/* *********************************************************** */

/* sequence to perform normal butt on 4+4 complex elems sequent */
#define	DEAL_8_NORMAL_SEQUENT(ptr_beg, tagstep, rot,                 \
	rot_half, r_rh, iSIGNed)                                     \
	DCL_4_CMPLX_PTRS(d0, ptr_beg, tagstep);                      \
	DCL_4_CMPLX_PTRS(d1, d0_0 + four_##tagstep, tagstep);        \
	DCL_CALC_NORMAL_BUTT(d0, 0, rot, rot_half, r_rh, iSIGNed);   \
	DCL_CALC_NORMAL_BUTT(d1, 1, rot, rot_half, r_rh, iSIGNed);   \
	STORE_USUAL_BUTTERFLIES(d0, 0);                              \
	STORE_USUAL_BUTTERFLIES(d1, 1)

/* *********************************************************** */

/* sequence to continue DEAL_8_NORMAL_SEQUENT on next 8 elems */
#define	DEAL_8_8_NORMAL_SEQUENT(ptr_beg, tagstep, rot,                   \
	rot_half, r_rh, iSIGNed)                                         \
	DEAL_8_NORMAL_SEQUENT(ptr_beg, tagstep, rot, rot_half, r_rh,     \
	    iSIGNed); {                                                  \
	    DCL_4_CMPLX_PTRS(d2, d0_0 + two_four_##tagstep,              \
		tagstep);                                                \
	    DCL_4_CMPLX_PTRS(d3, d0_0 + three_four_##tagstep,            \
		tagstep);                                                \
	    DCL_CALC_NORMAL_BUTT(d2, 2, rot, rot_half, r_rh, iSIGNed);   \
	    DCL_CALC_NORMAL_BUTT(d3, 3, rot, rot_half, r_rh, iSIGNed);   \
	    STORE_USUAL_BUTTERFLIES(d2, 2);                              \
	    STORE_USUAL_BUTTERFLIES(d3, 3);                              \
	}

/* *********************************************************** */

/* sequence to perform simple butt on 8 complex elems sequent */
#define	DEAL_8_SIMPLE_SEQUENT_RI(ptr_beg, tagstep, iSIGNed)         \
	DCL_4_CMPLX_PTRS_RI(d0, ptr_beg, tagstep);                  \
	DCL_4_NEXT_PTRS_RI(d1, ptr_beg, four_##tagstep, tagstep);   \
	DCL_CALC_SIMPLE_BUTT_RI(c0, d0, iSIGNed);                   \
	DCL_CALC_SIMPLE_BUTT_RI(c1, d1, iSIGNed);                   \
	STORE_SIMPLE_BUTTERFLIES_RI(d0, c0);                        \
	STORE_SIMPLE_BUTTERFLIES_RI(d1, c1)

/* *********************************************************** */

/* sequence to perform simple butt on 8+8 complex elems sequent */
#define	DEAL_8_8_SIMPLE_SEQUENT_RI(ptr_beg, tagstep, iSIGNed)       \
	DEAL_8_SIMPLE_SEQUENT_RI(ptr_beg, tagstep, iSIGNed); {      \
	    DCL_4_NEXT_PTRS_RI(d2, ptr_beg, two_four_##tagstep,     \
		tagstep);                                           \
	    DCL_4_NEXT_PTRS_RI(d3, ptr_beg, three_four_##tagstep,   \
		tagstep);                                           \
	    DCL_CALC_SIMPLE_BUTT_RI(c2, d2, iSIGNed);               \
	    DCL_CALC_SIMPLE_BUTT_RI(c3, d3, iSIGNed);               \
	    STORE_SIMPLE_BUTTERFLIES_RI(d2, c2);                    \
	    STORE_SIMPLE_BUTTERFLIES_RI(d3, c3);                    \
	}

/* *********************************************************** */

/* sequence to perform normal butt on 4+4 complex elems sequent */
#define	DEAL_8_NORMAL_SEQUENT_RI(ptr_beg, tagstep, rot,                 \
	rot_half, r_rh, iSIGNed)                                        \
	DCL_4_CMPLX_PTRS_RI(d0, ptr_beg, tagstep);                      \
	DCL_4_NEXT_PTRS_RI(d1, ptr_beg, four_##tagstep, tagstep);       \
	DCL_CALC_NORMAL_BUTT_RI(d0, 0, rot, rot_half, r_rh, iSIGNed);   \
	DCL_CALC_NORMAL_BUTT_RI(d1, 1, rot, rot_half, r_rh, iSIGNed);   \
	STORE_USUAL_BUTTERFLIES_RI(d0, 0);                              \
	STORE_USUAL_BUTTERFLIES_RI(d1, 1)

/* *********************************************************** */

/* sequence to continue DEAL_8_NORMAL_SEQUENT on next 8 elems */
#define	DEAL_8_8_NORMAL_SEQUENT_RI(ptr_beg, tagstep, rot,                 \
	rot_half, r_rh, iSIGNed)                                          \
	DEAL_8_NORMAL_SEQUENT_RI(ptr_beg, tagstep, rot, rot_half, r_rh,   \
	    iSIGNed); {                                                   \
	    DCL_4_NEXT_PTRS_RI(d2, ptr_beg, two_four_##tagstep,           \
		tagstep);                                                 \
	    DCL_4_NEXT_PTRS_RI(d3, ptr_beg, three_four_##tagstep,         \
		tagstep);                                                 \
	    DCL_CALC_NORMAL_BUTT_RI(d2, 2, rot, rot_half, r_rh,           \
		iSIGNed);                                                 \
	    DCL_CALC_NORMAL_BUTT_RI(d3, 3, rot, rot_half, r_rh,           \
		iSIGNed);                                                 \
	    STORE_USUAL_BUTTERFLIES_RI(d2, 2);                            \
	    STORE_USUAL_BUTTERFLIES_RI(d3, 3);                            \
	}

/* *********************************************************** */

#define	CACHE1_ELEMS	(1 << CACHE1_ORD)

#define	MSIN_PI_BY4                                                           \
	    ((precise_t)-0.70710678118654752440084436210484903928483593768846)

/* *********************************************************** */

#define	DEF_CVAR(x)	mlib_d64 x##_r, x##_i

/* *********************************************************** */

#define	DEF_CVAR4(x)                                            \
	mlib_d64 x##0_r, x##0_i, x##1_r, x##1_i,                \
	    x##2_r, x##2_i, x##3_r, x##3_i

/* *********************************************************** */

#define	DEF_CVAR4x(x)                                           \
	mlib_d64 x##1_r, x##1_i, x##2_r, x##2_i,                \
	    x##3_r, x##3_i

/* *********************************************************** */

#define	DEF_CVAR8(x)                                            \
	mlib_d64 x##0_r, x##0_i, x##1_r, x##1_i,                \
	    x##2_r, x##2_i, x##3_r, x##3_i, x##4_r,             \
	    x##4_i, x##5_r, x##5_i, x##6_r, x##6_i,             \
	    x##7_r, x##7_i

/* *********************************************************** */

#define	DEF_CVAR8x(x)                                           \
	mlib_d64 x##1_r, x##1_i, x##2_r, x##2_i,                \
	    x##3_r, x##3_i, x##4_r, x##4_i, x##5_r,             \
	    x##5_i, x##6_r, x##6_i, x##7_r, x##7_i

/* *********************************************************** */

#define	INI_CVAR(x, R, I)	mlib_d64 x##_r = R, x##_i = I

/* *********************************************************** */

#define	C_LD(x, dat)	x##_r = (dat)[0]; x##_i = (dat)[1]

/* *********************************************************** */

#define	C_ST(x, dat)	(dat)[0] = x##_r; (dat)[1] = x##_i

/* *********************************************************** */

#define	C_LD_RI(x, dat, indx)                                   \
	x##_r = (dat##_r)[indx];                                \
	x##_i = (dat##_i)[indx]

/* *********************************************************** */

#define	C_ST_RI(x, dat, indx)                                   \
	(dat##_r)[indx] = x##_r;                                \
	(dat##_i)[indx] = x##_i

/* *********************************************************** */

#define	CMUL(z, x, y)                                           \
	z##_r = x##_r * y##_r - x##_i * y##_i;                  \
	z##_i = x##_r * y##_i + x##_i * y##_r

/* *********************************************************** */

#define	CROT(y, x)                                              \
	{                                                       \
	    mlib_d64 y_r = y##_r;                               \
	                                                        \
	    y##_r = x##_r * y##_r - x##_i * y##_i;              \
	    y##_i = x##_r * y##_i + x##_i * y_r;                \
	}

/* *********************************************************** */

#if ISIGN == INVERS_FFT

#define	CROT_P1(y, x)                                           \
	y##_r = SIN_PI_BY4 * (x##_r - x##_i);                   \
	y##_i = SIN_PI_BY4 * (x##_i + x##_r)

#define	CROT_P3(y, x)                                           \
	y##_r = MSIN_PI_BY4 * (x##_i + x##_r);                  \
	y##_i = SIN_PI_BY4 * (x##_r - x##_i)

#else /* ISIGN == INVERS_FFT */

#define	CROT_P1(y, x)                                           \
	y##_r = SIN_PI_BY4 * (x##_r + x##_i);                   \
	y##_i = SIN_PI_BY4 * (x##_i - x##_r)

#define	CROT_P3(y, x)                                           \
	y##_r = SIN_PI_BY4 * (x##_i - x##_r);                   \
	y##_i = MSIN_PI_BY4 * (x##_i + x##_r)

#endif /* ISIGN == INVERS_FFT */

/* *********************************************************** */

#define	ROTATE_CMPLX_TMP(dest, incr, tmp)                       \
	{                                                       \
	    dest##_r = ((tmp =                                  \
		dest##_r) * incr##_r -                          \
		dest##_i * incr##_i);                           \
	    dest##_i =                                          \
		tmp * incr##_i + dest##_i * incr##_r;           \
	}

/* *********************************************************** */

#define	CADD(z, x, y)	z##_r = x##_r + y##_r; z##_i = x##_i + y##_i

/* *********************************************************** */

#define	CSUB(z, x, y)	z##_r = x##_r - y##_r; z##_i = x##_i - y##_i

/* *********************************************************** */

#if ISIGN == INVERS_FFT

#define	RSUB(z, x, y)	z##_i = x##_r - y##_r; z##_r = y##_i - x##_i

#else /* ISIGN == INVERS_FFT */

#define	RSUB(z, x, y)	z##_i = y##_r - x##_r; z##_r = x##_i - y##_i

#endif /* ISIGN == INVERS_FFT */

/* *********************************************************** */

#define	CBUT(a, b, N0, N1)                                      \
	CADD(b##N0, a##N0, a##N1);                              \
	CSUB(b##N1, a##N0, a##N1)

/* *********************************************************** */

#define	RBUT(a, b, N0, N1)                                      \
	CADD(b##N0, a##N0, a##N1);                              \
	RSUB(b##N1, a##N0, a##N1)

/* *********************************************************** */

#define	INI_ROT(rot, ord_lay)                                   \
	mlib_d64 sin_half##rot = dsign * SINUS(ord_lay + 1);    \
	mlib_d64 rot##_r =                                      \
	    ONE - TWO * sin_half##rot * sin_half##rot;          \
	mlib_d64 rot##_i = dsign * SINUS(ord_lay)

/* *********************************************************** */

#define	DCL_DST_PTR_RI(pbeg, dst, shift)                        \
	deal_t *pbeg##_r = (dst##_r) + shift, *pbeg##_i =       \
	    (dst##_i) + shift

/* *********************************************************** */

#define	DCL_DST_PTR_C(pbeg, dst, shift)                         \
	dtCmplx *CONST pbeg = (dtCmplx *) (dst) + shift

/* *********************************************************** */

#define	DCL_4_CMPLX_PTRS_C(tag, ptr_beg, tagstp)                \
	DCL_4_CMPLX_PTRS(tag, ptr_beg, tagstp)

/* *********************************************************** */

#define	DCL_4_NEXT_PTRS_C(tag, ptr_beg, shift, tagstp)          \
	DCL_4_CMPLX_PTRS_C(tag, ptr_beg + shift, tagstp)

/* *********************************************************** */

#define	BUTTERFLY_CMPLX_CMPLX_C(dest1, dest2, src1, src2)       \
	{                                                       \
	    dest1->r = src1##_r + src2##_r;                     \
	    dest1->i = src1##_i + src2##_i;                     \
	    dest2->r = src1##_r - src2##_r;                     \
	    dest2->i = src1##_i - src2##_i;                     \
	}

/* *********************************************************** */

/* special for 1st-3layers */
#define	BUTTERFLY_CMPLX_CMPLX_ROT(dest1, dest2, rot2, dstag)       \
	BUTTERFLY_CMPLX_CMPLX_##dstag(dest1, dest2, dest1, rot2)

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

/* sequence to perform simple butt on 4 complex elems */
#define	DEAL_4_SIMPLE_C(ptr_beg, tagstep, iSIGNed)              \
	DEAL_4_SIMPLE(ptr_beg, tagstep, iSIGNed)

/* *********************************************************** */

/* declare/define components for simple butterflies */
#define	DCL_CALC_SIMPLE_BUTT_C(dst, src, iSIGNed)               \
	DCL_CALC_SIMPLE_BUTT(dst, src, iSIGNed)

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
#define	STORE_3RD_LAY_BUTTERFLIES(src0, src1, dstag)            \
	BUTTERFLY_CMPLX_CMPLX_ROT(src0##_0, src1##_0,           \
	    src1##_0r, dstag);                                  \
	BUTTERFLY_CMPLX_CMPLX_ROT(src0##_1, src1##_1,           \
	    src1##_1r, dstag);                                  \
	BUTTERFLY_CMPLX_CMPLX_ROT(src0##_2, src1##_2,           \
	    src1##_2r, dstag);                                  \
	BUTTERFLY_CMPLX_CMPLX_ROT(src0##_3, src1##_3,           \
	    src1##_3r, dstag)

/* *********************************************************** */

#define	FIRST_3_LAYERS(ord_arr, iSIGNed, dst, dstag)                      \
	for (iii = 0; iii < nnn; iii += 8) {                              \
	    deal_t d0_0_r, d0_0_i, d0_1_r, d0_1_i, d0_2_r, d0_2_i,        \
		d0_3_r, d0_3_i, d1_0_r, d1_0_i, d1_1_r, d1_1_i, d1_2_r,   \
		d1_2_i, d1_3_r, d1_3_i;                                   \
	    DCL_DST_PTR_##dstag(pbeg, dst, iii);                          \
	    DCL_4_CMPLX_PTRS_##dstag(d0, pbeg, 1);                        \
	    DCL_4_NEXT_PTRS_##dstag(d1, pbeg, 4, 1);                      \
	    DCL_CALC_SIMPLE_BUTT_##dstag(c0, d0, iSIGNed);                \
	    DCL_CALC_SIMPLE_BUTT_##dstag(c1, d1, iSIGNed);                \
	    CALC_SIMPLE_BUTTS(d0, c0);                                    \
	    CALC_SIMPLE_BUTTS(d1, c1); {                                  \
		DCL_CALC_3RD_LAY_BUTT(d1, iSIGNed);                       \
		STORE_3RD_LAY_BUTTERFLIES(d0, d1, dstag);                 \
	    }                                                             \
	}

/* *********************************************************** */

#define	FIRST_2_LAYERS(ord_arr, iSIGNed, dst, dstag)            \
	for (iii = 0; iii < nnn; iii += 4) {                    \
	    DCL_DST_PTR_##dstag(pbeg, dst, iii);                \
	    DEAL_4_SIMPLE_##dstag(pbeg, 1, iSIGNed);            \
	}

/* *********************************************************** */

#ifdef _MSC_VER
#pragma optimize("", off)
#endif /* _MSC_VER */

/* *********************************************************** */

void FUNC(
	order4) (
	mlib_d64 *pal64data,
	mlib_d64 *data_i,
	mlib_s32 order)
{
	dtCmplx *CONST dat = (dtCmplx *) pal64data;

	DEAL_ALL_ORD4(isign, dat, SSIGN);
}

/* *********************************************************** */

void FUNC(
	first2) (
	mlib_d64 *pal64data,
	mlib_d64 *data_i,
	mlib_s32 n_ord_arr)
{
	mlib_s32 CONST nnn = 1 << n_ord_arr;
	mlib_s32 iii;

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
	FIRST_2_LAYERS(n_ord_arr, SSIGN, pal64data, C);
}

/* *********************************************************** */

void FUNC(
	first3) (
	mlib_d64 *pal64data,
	mlib_d64 *data_i,
	mlib_s32 n_ord_arr)
{
	mlib_s32 CONST nnn = 1 << n_ord_arr;
	mlib_s32 iii;

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
	FIRST_3_LAYERS(n_ord_arr, SSIGN, pal64data, C);
}

/* *********************************************************** */

#if NUM_SPECIAL_LAST >= 2

void FUNC(
	2LayersLast) (
	dtCmplx * dat,
	dtCmplx * data_i,
	mlib_s32 n_ord_arr)
{
	mlib_s32 isign = ISIGN;
	mlib_s32 n_ord_lay = n_ord_arr - 2;
	mlib_s32 n_len_arr = 1 << n_ord_arr;
	mlib_s32 n_step_half = n_len_arr >> 3;

	DCL_CALC_STEPS1234(n_step, n_ord_lay);
	DCL_CALC_INCR(incr_half, n_ord_lay + 1, 0);
	DCL_CALC_INCR(incr, n_ord_lay, 1);
	DCL_INIT_ROT(rot, incr);
	DCL_INIT_ROT(rot_half, incr_half);
	precise_t wtmp;
	mlib_s32 iii;

	DEAL_4_SIMPLE(dat, n_step, SSIGN);
	{
		DEAL_4_HALFWAY(dat + n_step_half, n_step, SSIGN);
	}

	for (iii = 1; iii < n_step_half; iii++) {
		DCL_MULT_ROT_ROT_HALF(rot, rot_half, r_rh);
		DEAL_8_USUAL_MIRR(dat + iii, dat + n_step - iii,
		    n_step, rot, rot_half, r_rh, SSIGN);
		ROTATE_CMPLX_TMP(rot, incr, wtmp);
		ROTATE_CMPLX_TMP(rot_half, incr_half, wtmp);
	}
}

#endif /* NUM_SPECIAL_LAST >= 2 */

/* *********************************************************** */

#if NUM_SPECIAL_LAST >= 3

void FUNC(
	2LayersLastM1) (
	dtCmplx * dat,
	dtCmplx * data_i,
	mlib_s32 n_ord_arr)
{
	mlib_s32 isign = ISIGN;
	mlib_s32 n_ord_lay = n_ord_arr - 3;

	DCL_CALC_STEPS1234(n_step, n_ord_lay);
	DCL_CALC_INCR(incr_half, n_ord_lay + 1, 0);
	DCL_CALC_INCR(incr, n_ord_lay, 1);
	DCL_INIT_ROT(rot, incr);
	DCL_INIT_ROT(rot_half, incr_half);
	precise_t wtmp;
	mlib_s32 iii;

	DEAL_8_SIMPLE_SEQUENT(dat, n_step, SSIGN);
	for (iii = 1; iii < n_step; iii++) {
		DCL_MULT_ROT_ROT_HALF(rot, rot_half, r_rh);
		DEAL_8_NORMAL_SEQUENT(dat + iii, n_step, rot, rot_half, r_rh,
		    SSIGN);
		ROTATE_CMPLX_TMP(rot, incr, wtmp);
		ROTATE_CMPLX_TMP(rot_half, incr_half, wtmp);
	}
}

#endif /* NUM_SPECIAL_LAST >= 3 */

/* *********************************************************** */

#if NUM_SPECIAL_LAST >= 4

void FUNC(
	2LayersLastM2) (
	dtCmplx * dat,
	dtCmplx * data_i,
	mlib_s32 n_ord_arr)
{
	mlib_s32 isign = ISIGN;
	mlib_s32 n_ord_lay = n_ord_arr - 4;

	DCL_CALC_STEPS1234(n_step, n_ord_lay);
	DCL_CALC_INCR(incr_half, n_ord_lay + 1, 0);
	DCL_CALC_INCR(incr, n_ord_lay, 1);
	DCL_INIT_ROT(rot, incr);
	DCL_INIT_ROT(rot_half, incr_half);
	CONST mlib_s32 two_four_n_step = n_step * 8;
	CONST mlib_s32 three_four_n_step = n_step * 12;
	precise_t wtmp;
	mlib_s32 iii;

	DEAL_8_8_SIMPLE_SEQUENT(dat, n_step, SSIGN);
	for (iii = 1; iii < n_step; iii++) {
		DCL_MULT_ROT_ROT_HALF(rot, rot_half, r_rh);
		DEAL_8_8_NORMAL_SEQUENT(dat + iii, n_step, rot, rot_half, r_rh,
		    SSIGN);
		ROTATE_CMPLX_TMP(rot, incr, wtmp);
		ROTATE_CMPLX_TMP(rot_half, incr_half, wtmp);
	}
}

#endif /* NUM_SPECIAL_LAST >= 4 */

/* *********************************************************** */

void FUNC(
	2Layers) (
	mlib_d64 *dat,
	mlib_d64 *data_i,
	mlib_s32 n_ord_arr,
	mlib_s32 n_ord_lay)
{
	mlib_d64 dsign = ISIGN;
	mlib_s32 i, j;
	mlib_s32 dstep = 2 * (1 << n_ord_lay);
	mlib_s32 bsize = 2 * (1 << (n_ord_lay + 2));
	mlib_s32 len_arr = 2 * (1 << n_ord_arr);
	mlib_s32 n_left;

	INI_ROT(rot, n_ord_lay + 1);
	INI_CVAR(r1, 1, 0);
	INI_CVAR(r2, 1, 0);
	INI_CVAR(r3, 1, 0);

	n_left = n_ord_arr - n_ord_lay;

	if (n_left <= NUM_SPECIAL_LAST) {
#if NUM_SPECIAL_LAST >= 2

		if (n_left == 2) {
			FUNC(2LayersLast) ((void *)dat, (void *)data_i,
			    n_ord_arr);
		}
#endif /* NUM_SPECIAL_LAST >= 2 */
#if NUM_SPECIAL_LAST >= 3
		else if (n_left == 3) {
			FUNC(2LayersLastM1) ((void *)dat, (void *)data_i,
			    n_ord_arr);
		}
#endif /* NUM_SPECIAL_LAST >= 3 */
#if NUM_SPECIAL_LAST >= 4
		else if (n_left == 4) {
			FUNC(2LayersLastM2) ((void *)dat, (void *)data_i,
			    n_ord_arr);
		}
#endif /* NUM_SPECIAL_LAST >= 4 */
		return;
	}

	for (j = 0; j < dstep; j += 2) {
		mlib_d64 *dat0 = dat + j, *dat1 = dat0 + dstep,
		    *dat2 = dat1 + dstep, *dat3 = dat2 + dstep;
		DEF_CVAR4x(x);
		DEF_CVAR4(a);
		DEF_CVAR4(b);
		DEF_CVAR4(c);

/* load sources */
		C_LD(a0, dat0);
		C_LD(x1, dat1);
		C_LD(x2, dat2);
		C_LD(x3, dat3);

/* rotate sources */
		CMUL(a1, x1, r2);
		CMUL(a2, x2, r1);
		CMUL(a3, x3, r3);

/* first stage */
		CADD(b0, a0, a1);
		CSUB(b1, a0, a1);
		CADD(b2, a2, a3);
		RSUB(b3, a2, a3);

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
		for (i = bsize; i < len_arr; i += bsize) {
/* load sources */
			C_LD(a0, dat0 + i);
			C_LD(x1, dat1 + i);
			C_LD(x2, dat2 + i);
			C_LD(x3, dat3 + i);

/* rotate sources */
			CMUL(a1, x1, r2);
			CMUL(a2, x2, r1);
			CMUL(a3, x3, r3);

/* second stage */
			CADD(c0, b0, b2);
			CSUB(c2, b0, b2);
			CADD(c1, b1, b3);
			CSUB(c3, b1, b3);

/* first stage */
			CADD(b0, a0, a1);
			CSUB(b1, a0, a1);
			CADD(b2, a2, a3);
			RSUB(b3, a2, a3);

/* store */
			C_ST(c0, dat0 + i - bsize);
			C_ST(c1, dat1 + i - bsize);
			C_ST(c2, dat2 + i - bsize);
			C_ST(c3, dat3 + i - bsize);
		}

/* second stage */
		CADD(c0, b0, b2);
		CSUB(c2, b0, b2);
		CADD(c1, b1, b3);
		CSUB(c3, b1, b3);

/* store last */
		C_ST(c0, dat0 + len_arr - bsize);
		C_ST(c1, dat1 + len_arr - bsize);
		C_ST(c2, dat2 + len_arr - bsize);
		C_ST(c3, dat3 + len_arr - bsize);

/* rotate */
		CROT(r1, rot);
		CMUL(r2, r1, r1);
		CMUL(r3, r1, r2);
	}
}

/* *********************************************************** */

void FUNC(
	3LayersBlk) (
	mlib_d64 *dat,
	mlib_d64 *data_i,
	mlib_s32 n_ord_lay,
	mlib_s32 i0,
	mlib_s32 i1,
	mlib_d64 r1__r,
	mlib_d64 r1__i)
{
	mlib_d64 dsign = ISIGN;
	mlib_s32 n_ord_arr = n_ord_lay + 3;
	mlib_s32 i;
	mlib_s32 dstep = 2 * (1 << n_ord_lay);
	mlib_d64 *dat0 = dat, *dat1 = dat0 + dstep;
	mlib_d64 *dat2 = dat1 + dstep, *dat3 = dat2 + dstep;
	mlib_d64 *dat4 = dat3 + dstep, *dat5 = dat4 + dstep;
	mlib_d64 *dat6 = dat5 + dstep, *dat7 = dat6 + dstep;

	INI_ROT(rot, n_ord_lay + 2);
	{
		DEF_CVAR8x(x);
		DEF_CVAR8(a);
		DEF_CVAR8(b);
		DEF_CVAR8(c);
		DEF_CVAR8(d);
		DEF_CVAR(z5);
		DEF_CVAR(z7);
		INI_CVAR(r1, 1, 0);
		INI_CVAR(r2, 1, 0);
		INI_CVAR(r3, 1, 0);
		INI_CVAR(r4, 1, 0);
		INI_CVAR(r5, 1, 0);
		INI_CVAR(r6, 1, 0);
		INI_CVAR(r7, 1, 0);

		r1_r = r1__r;
		r1_i = r1__i;

		CMUL(r2, r1, r1);
		CMUL(r3, r1, r2);
		CMUL(r4, r2, r2);
		CMUL(r5, r2, r3);
		CMUL(r6, r3, r3);
		CMUL(r7, r3, r4);
#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
		for (i = 2*i0; i < 2*i1; i += 2) {
			C_LD(a0, dat0 + i);
			C_LD(x1, dat1 + i);
			C_LD(x2, dat2 + i);
			C_LD(x3, dat3 + i);
			C_LD(x4, dat4 + i);
			C_LD(x5, dat5 + i);
			C_LD(x6, dat6 + i);
			C_LD(x7, dat7 + i);

/* first stage */
			CMUL(a1, x1, r4);
			CBUT(a, b, 0, 1);

			CMUL(a2, x2, r2);
			CMUL(a3, x3, r6);
			RBUT(a, b, 2, 3);

			CMUL(a4, x4, r1);
			CMUL(a5, x5, r5);
			CBUT(a, b, 4, 5);

			CMUL(a6, x6, r3);
			CMUL(a7, x7, r7);
			RBUT(a, b, 6, 7);

/* second stage */
			CBUT(b, c, 0, 2);
			CBUT(b, c, 1, 3);
			RBUT(b, c, 4, 6);
			CBUT(b, z, 5, 7);

			CROT_P1(c5, z5);
			CROT_P3(c7, z7);

/* third stage */
			CBUT(c, d, 0, 4);
			CBUT(c, d, 1, 5);
			CBUT(c, d, 2, 6);
			CBUT(c, d, 3, 7);

/* rotate */
			CROT(r1, rot);
			CMUL(r2, r1, r1);
			CMUL(r3, r1, r2);
			CMUL(r4, r2, r2);
			CMUL(r5, r2, r3);
			CMUL(r6, r3, r3);
			CMUL(r7, r3, r4);

/* store */
			C_ST(d0, dat0 + i);
			C_ST(d1, dat1 + i);
			C_ST(d2, dat2 + i);
			C_ST(d3, dat3 + i);
			C_ST(d4, dat4 + i);
			C_ST(d5, dat5 + i);
			C_ST(d6, dat6 + i);
			C_ST(d7, dat7 + i);
		}
	}
}

/* *********************************************************** */

/*
 * Functions (*_RI) with separate real/image arrays
 *
 */

void FUNC(
	order4_RI) (
	deal_t *CONST pal64data_r,
	deal_t *CONST pal64data_i,
	mlib_s32 CONST order)
	{
	deal_t *CONST dat_r = (deal_t *)pal64data_r;
	deal_t *CONST dat_i = (deal_t *)pal64data_i;

	DEAL_ALL_ORD4_RI(isign, dat, SSIGN);
}

/* *********************************************************** */

void FUNC(
	first2_RI) (
	deal_t *pal64data_r,
	deal_t *pal64data_i,
	mlib_s32 n_ord_arr)
{
	mlib_s32 CONST nnn = 1 << n_ord_arr;
	mlib_s32 iii;

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
	FIRST_2_LAYERS(n_ord_arr, SSIGN, pal64data, RI);
}

/* *********************************************************** */

void FUNC(
	first3_RI) (
	deal_t *pal64data_r,
	deal_t *pal64data_i,
	mlib_s32 n_ord_arr)
{
	mlib_s32 CONST nnn = 1 << n_ord_arr;
	mlib_s32 iii;

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
	FIRST_3_LAYERS(n_ord_arr, SSIGN, pal64data, RI);
}

/* *********************************************************** */

#if NUM_SPECIAL_LAST >= 2

void FUNC(
	2LayersLast_RI) (
	deal_t *CONST dat_r,
	deal_t *CONST dat_i,
	mlib_s32 CONST n_ord_arr)
{
	mlib_s32 isign = ISIGN;
	CONST mlib_s32 n_ord_lay = n_ord_arr - 2;
	CONST mlib_s32 n_len_arr = 1 << n_ord_arr;
	CONST mlib_s32 n_step_half = n_len_arr >> 3;

	DCL_CALC_STEPS1234(n_step, n_ord_lay);
	DCL_CALC_INCR(incr_half, n_ord_lay + 1, 0);
	DCL_CALC_INCR(incr, n_ord_lay, 1);
	DCL_INIT_ROT(rot, incr);
	DCL_INIT_ROT(rot_half, incr_half);
	precise_t wtmp;
	mlib_s32 iii;

	deal_t *dat_mmm_r = dat_r, *dat_mmm_i = dat_i;

	DEAL_4_SIMPLE_RI(dat_mmm, n_step, SSIGN);
	{
		deal_t *dat_mmm_r = dat_r + n_step_half, *dat_mmm_i =
		    dat_i + n_step_half;

		DEAL_4_HALFWAY_RI(dat_mmm, n_step, SSIGN);
	}

	for (iii = 1; iii < n_step_half; iii++) {
		DCL_MULT_ROT_ROT_HALF(rot, rot_half, r_rh);
		{
			deal_t *dat_mmm_r = dat_r + iii, *dat_mmm_i =
			    dat_i + iii;
			deal_t *dat_mirr_r = dat_r + n_step - iii, *dat_mirr_i =
			    dat_i + n_step - iii;
			DEAL_8_USUAL_MIRR_RI(dat_mmm, dat_mirr, n_step, rot,
			    rot_half, r_rh, SSIGN);
		}

		ROTATE_CMPLX_TMP(rot, incr, wtmp);
		ROTATE_CMPLX_TMP(rot_half, incr_half, wtmp);
	}
}

#endif /* NUM_SPECIAL_LAST >= 2 */

/* *********************************************************** */

#if NUM_SPECIAL_LAST >= 2

void FUNC(
	2LayersLastM1_RI) (
	deal_t *CONST dat_r,
	deal_t *CONST dat_i,
	mlib_s32 CONST n_ord_arr)
{
	mlib_s32 isign = ISIGN;
	CONST mlib_s32 n_ord_lay = n_ord_arr - 3;

	DCL_CALC_STEPS1234(n_step, n_ord_lay);
	DCL_CALC_INCR(incr_half, n_ord_lay + 1, 0);
	DCL_CALC_INCR(incr, n_ord_lay, 1);
	DCL_INIT_ROT(rot, incr);
	DCL_INIT_ROT(rot_half, incr_half);
	precise_t wtmp;
	mlib_s32 iii;

	DEAL_8_SIMPLE_SEQUENT_RI(dat, n_step, SSIGN);
	for (iii = 1; iii < n_step; iii++) {
		DCL_MULT_ROT_ROT_HALF(rot, rot_half, r_rh);
		deal_t *dat_iii_r = dat_r + iii, *dat_iii_i = dat_i + iii;

		DEAL_8_NORMAL_SEQUENT_RI(dat_iii, n_step, rot, rot_half, r_rh,
		    SSIGN);
		ROTATE_CMPLX_TMP(rot, incr, wtmp);
		ROTATE_CMPLX_TMP(rot_half, incr_half, wtmp);
	}
}

#endif /* NUM_SPECIAL_LAST >= 2 */

/* *********************************************************** */

#if NUM_SPECIAL_LAST >= 2

void FUNC(
	2LayersLastM2_RI) (
	deal_t *CONST dat_r,
	deal_t *CONST dat_i,
	mlib_s32 CONST n_ord_arr)
{
	mlib_s32 isign = ISIGN;
	CONST mlib_s32 n_ord_lay = n_ord_arr - 4;

	DCL_CALC_STEPS1234(n_step, n_ord_lay);
	DCL_CALC_INCR(incr_half, n_ord_lay + 1, 0);
	DCL_CALC_INCR(incr, n_ord_lay, 1);
	DCL_INIT_ROT(rot, incr);
	DCL_INIT_ROT(rot_half, incr_half);
	CONST mlib_s32 two_four_n_step = n_step * 8;
	CONST mlib_s32 three_four_n_step = n_step * 12;
	precise_t wtmp;
	mlib_s32 iii;

	DEAL_8_8_SIMPLE_SEQUENT_RI(dat, n_step, SSIGN);
	for (iii = 1; iii < n_step; iii++) {
		DCL_MULT_ROT_ROT_HALF(rot, rot_half, r_rh);
		deal_t *dat_iii_r = dat_r + iii, *dat_iii_i = dat_i + iii;

		DEAL_8_8_NORMAL_SEQUENT_RI(dat_iii, n_step, rot, rot_half, r_rh,
		    SSIGN);
		ROTATE_CMPLX_TMP(rot, incr, wtmp);
		ROTATE_CMPLX_TMP(rot_half, incr_half, wtmp);
	}
}

#endif /* NUM_SPECIAL_LAST >= 2 */

/* *********************************************************** */

void FUNC(
	2Layers_RI) (
	mlib_d64 *dat_r,
	mlib_d64 *dat_i,
	mlib_s32 n_ord_arr,
	mlib_s32 n_ord_lay)
{
	mlib_d64 dsign = ISIGN;
	mlib_s32 i, j;
	mlib_s32 dstep = (1 << n_ord_lay);
	mlib_s32 bsize = (1 << (n_ord_lay + 2));
	mlib_s32 len_arr = (1 << n_ord_arr);
	mlib_s32 n_left;

	INI_ROT(rot, n_ord_lay + 1);
	INI_CVAR(r1, 1, 0);
	INI_CVAR(r2, 1, 0);
	INI_CVAR(r3, 1, 0);

	n_left = n_ord_arr - n_ord_lay;

	if (n_left <= NUM_SPECIAL_LAST) {
#if NUM_SPECIAL_LAST >= 2

		if (n_left == 2) {
			FUNC(2LayersLast_RI) ((void *)dat_r, (void *)dat_i,
			    n_ord_arr);
		}
#endif /* NUM_SPECIAL_LAST >= 2 */
#if NUM_SPECIAL_LAST >= 3
		else if (n_left == 3) {
			FUNC(2LayersLastM1_RI) ((void *)dat_r, (void *)dat_i,
			    n_ord_arr);
		}
#endif /* NUM_SPECIAL_LAST >= 3 */
#if NUM_SPECIAL_LAST >= 4
		else if (n_left == 4) {
			FUNC(2LayersLastM2_RI) ((void *)dat_r, (void *)dat_i,
			    n_ord_arr);
		}
#endif /* NUM_SPECIAL_LAST >= 4 */
		return;
	}

	for (j = 0; j < dstep; j++) {
		mlib_d64 *dat0_r = dat_r + j, *dat1_r = dat0_r + dstep;
		mlib_d64 *dat2_r = dat1_r + dstep, *dat3_r = dat2_r + dstep;
		mlib_d64 *dat0_i = dat_i + j, *dat1_i = dat0_i + dstep;
		mlib_d64 *dat2_i = dat1_i + dstep, *dat3_i = dat2_i + dstep;

		DEF_CVAR4x(x);
		DEF_CVAR4(a);
		DEF_CVAR4(b);
		DEF_CVAR4(c);

/* load sources */
		C_LD_RI(a0, dat0, 0);
		C_LD_RI(x1, dat1, 0);
		C_LD_RI(x2, dat2, 0);
		C_LD_RI(x3, dat3, 0);

/* rotate sources */
		CMUL(a1, x1, r2);
		CMUL(a2, x2, r1);
		CMUL(a3, x3, r3);

/* first stage */
		CADD(b0, a0, a1);
		CSUB(b1, a0, a1);
		CADD(b2, a2, a3);
		RSUB(b3, a2, a3);

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
		for (i = bsize; i < len_arr; i += bsize) {
/* load sources */
			C_LD_RI(a0, dat0, i);
			C_LD_RI(x1, dat1, i);
			C_LD_RI(x2, dat2, i);
			C_LD_RI(x3, dat3, i);

/* rotate sources */
			CMUL(a1, x1, r2);
			CMUL(a2, x2, r1);
			CMUL(a3, x3, r3);

/* second stage */
			CADD(c0, b0, b2);
			CSUB(c2, b0, b2);
			CADD(c1, b1, b3);
			CSUB(c3, b1, b3);

/* first stage */
			CADD(b0, a0, a1);
			CSUB(b1, a0, a1);
			CADD(b2, a2, a3);
			RSUB(b3, a2, a3);

/* store */
			C_ST_RI(c0, dat0, i - bsize);
			C_ST_RI(c1, dat1, i - bsize);
			C_ST_RI(c2, dat2, i - bsize);
			C_ST_RI(c3, dat3, i - bsize);
		}

/* second stage */
		CADD(c0, b0, b2);
		CSUB(c2, b0, b2);
		CADD(c1, b1, b3);
		CSUB(c3, b1, b3);

/* store last */
		C_ST_RI(c0, dat0, len_arr - bsize);
		C_ST_RI(c1, dat1, len_arr - bsize);
		C_ST_RI(c2, dat2, len_arr - bsize);
		C_ST_RI(c3, dat3, len_arr - bsize);

/* rotate */
		CROT(r1, rot);
		CMUL(r2, r1, r1);
		CMUL(r3, r1, r2);
	}
}

/* *********************************************************** */

void FUNC(
	3LayersBlk_RI) (
	mlib_d64 *dat_r,
	mlib_d64 *dat_i,
	mlib_s32 n_ord_lay,
	mlib_s32 i0,
	mlib_s32 i1,
	mlib_d64 r1__r,
	mlib_d64 r1__i)
{
	mlib_d64 dsign = ISIGN;
	mlib_s32 n_ord_arr = n_ord_lay + 3;
	mlib_s32 i;
	mlib_s32 dstep = (1 << n_ord_lay);
	mlib_d64 *dat0_r = dat_r, *dat1_r = dat0_r + dstep;
	mlib_d64 *dat2_r = dat1_r + dstep, *dat3_r = dat2_r + dstep;
	mlib_d64 *dat4_r = dat3_r + dstep, *dat5_r = dat4_r + dstep;
	mlib_d64 *dat6_r = dat5_r + dstep, *dat7_r = dat6_r + dstep;
	mlib_d64 *dat0_i = dat_i, *dat1_i = dat0_i + dstep;
	mlib_d64 *dat2_i = dat1_i + dstep, *dat3_i = dat2_i + dstep;
	mlib_d64 *dat4_i = dat3_i + dstep, *dat5_i = dat4_i + dstep;
	mlib_d64 *dat6_i = dat5_i + dstep, *dat7_i = dat6_i + dstep;

	INI_ROT(rot, n_ord_lay + 2);
	{
		DEF_CVAR8x(x);
		DEF_CVAR8(a);
		DEF_CVAR8(b);
		DEF_CVAR8(c);
		DEF_CVAR8(d);
		DEF_CVAR(z5);
		DEF_CVAR(z7);
		INI_CVAR(r1, 1, 0);
		INI_CVAR(r2, 1, 0);
		INI_CVAR(r3, 1, 0);
		INI_CVAR(r4, 1, 0);
		INI_CVAR(r5, 1, 0);
		INI_CVAR(r6, 1, 0);
		INI_CVAR(r7, 1, 0);

		r1_r = r1__r;
		r1_i = r1__i;

		CMUL(r2, r1, r1);
		CMUL(r3, r1, r2);
		CMUL(r4, r2, r2);
		CMUL(r5, r2, r3);
		CMUL(r6, r3, r3);
		CMUL(r7, r3, r4);

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
		for (i = i0; i < i1; i++) {
			C_LD_RI(a0, dat0, i);
			C_LD_RI(x1, dat1, i);
			C_LD_RI(x2, dat2, i);
			C_LD_RI(x3, dat3, i);
			C_LD_RI(x4, dat4, i);
			C_LD_RI(x5, dat5, i);
			C_LD_RI(x6, dat6, i);
			C_LD_RI(x7, dat7, i);

/* first stage */
			CMUL(a1, x1, r4);
			CBUT(a, b, 0, 1);

			CMUL(a2, x2, r2);
			CMUL(a3, x3, r6);
			RBUT(a, b, 2, 3);

			CMUL(a4, x4, r1);
			CMUL(a5, x5, r5);
			CBUT(a, b, 4, 5);

			CMUL(a6, x6, r3);
			CMUL(a7, x7, r7);
			RBUT(a, b, 6, 7);

/* second stage */
			CBUT(b, c, 0, 2);
			CBUT(b, c, 1, 3);
			RBUT(b, c, 4, 6);
			CBUT(b, z, 5, 7);

			CROT_P1(c5, z5);
			CROT_P3(c7, z7);

/* third stage */
			CBUT(c, d, 0, 4);
			CBUT(c, d, 1, 5);
			CBUT(c, d, 2, 6);
			CBUT(c, d, 3, 7);

/* rotate */
			CROT(r1, rot);
			CMUL(r2, r1, r1);
			CMUL(r3, r1, r2);
			CMUL(r4, r2, r2);
			CMUL(r5, r2, r3);
			CMUL(r6, r3, r3);
			CMUL(r7, r3, r4);

/* store */
			C_ST_RI(d0, dat0, i);
			C_ST_RI(d1, dat1, i);
			C_ST_RI(d2, dat2, i);
			C_ST_RI(d3, dat3, i);
			C_ST_RI(d4, dat4, i);
			C_ST_RI(d5, dat5, i);
			C_ST_RI(d6, dat6, i);
			C_ST_RI(d7, dat7, i);
		}
	}
}

/* *********************************************************** */

void FUNC(
	3Layers) (
	mlib_d64 *data,
	mlib_d64 *data_i,
	mlib_s32 n_ord_arr,
	mlib_s32 n_ord_lay)
{
	mlib_d64 dsign = ISIGN;
	INI_ROT(r, 3);

	mlib_s32 length_lay = 1 << (n_ord_lay + 3);
	mlib_s32 length_arr = 1 << n_ord_arr, i;
	mlib_s32 step = 1 << (n_ord_lay - 1);

	for (i = 0; i < length_arr; i += length_lay) {
		FUNC(3LayersBlk)(data + 2 * i, NULL,
			n_ord_lay, 0, 2*step, 1, 0);
	}
}

/* *********************************************************** */

void FUNC(
	3Layers_RI) (
	mlib_d64 *data_r,
	mlib_d64 *data_i,
	mlib_s32 n_ord_arr,
	mlib_s32 n_ord_lay)
{
	mlib_s32 length_lay = 1 << (n_ord_lay + 3), length_arr =
	    1 << n_ord_arr, i;
	mlib_s32 step = 1 << n_ord_lay;

	for (i = 0; i < length_arr; i += length_lay) {
		FUNC(3LayersBlk_RI) (data_r + i, data_i + i, n_ord_lay,
		0, step, 1, 0);
	}
}

/* *********************************************************** */

fft_param
FUNC(func)[] =
{
	    ISIGN,
	    0,
	    FUNC(order4),
	    FUNC(first2),
	    FUNC(first3),
	    FUNC(2Layers),
	    FUNC(3Layers),
};

fft_param
FUNC(func_RI)[] =
{
	    ISIGN,
	    1,
	    FUNC(order4_RI),
	    FUNC(first2_RI),
	    FUNC(first3_RI),
	    FUNC(2Layers_RI),
	    FUNC(3Layers_RI),
};

/* ifdef _NO_LONGLONG, use stdc source codes, else use int source codes */
#else

/*
 * FUNCTIONS
 *   Internal functions for FFT layer processing
 *
 */

#include <mlib_signal.h>
#include <mlib_i_SignalFFTUtil.h>
#define	ISIGN DIRECT_FFT

/* *********************************************************** */

#if ISIGN == INVERS_FFT

#define	FUNC(NAME)	mlib_fft_##NAME##_I
#define	SSIGN(x)	AS_IS(x)

#define	FUNC_i(NAME)	mlib_fft_##NAME##_I_i
#define	SSIGN_i(x)	AS_IS_i(x)

#else /* ISIGN == INVERS_FFT */

#define	FUNC(NAME)	mlib_fft_##NAME##_F
#define	SSIGN(x)	MINUS(x)

#define	FUNC_i(NAME)	mlib_fft_##NAME##_F_i
#define	SSIGN_i(x)	MINUS_i(x)

#endif /* ISIGN == INVERS_FFT */

/* *********************************************************** */

#define	NUM_SPECIAL_LAST	3

/* *********************************************************** */

/* sequence to perform simple butt on 8 complex elems sequent */
#define	DEAL_8_SIMPLE_SEQUENT(ptr_beg, tagstep, iSIGNed)        \
	DCL_4_CMPLX_PTRS(d0, ptr_beg, tagstep);                 \
	DCL_4_CMPLX_PTRS(d1, d0_0 + four_##tagstep, tagstep);   \
	DCL_CALC_SIMPLE_BUTT(c0, d0, iSIGNed);                  \
	DCL_CALC_SIMPLE_BUTT(c1, d1, iSIGNed);                  \
	STORE_SIMPLE_BUTTERFLIES(d0, c0);                       \
	STORE_SIMPLE_BUTTERFLIES(d1, c1)

#define	DEAL_8_SIMPLE_SEQUENT_i(ptr_beg, tagstep, iSIGNed)        \
	DCL_4_CMPLX_PTRS_i(d0, ptr_beg, tagstep);                 \
	DCL_4_CMPLX_PTRS_i(d1, d0_0 + four_##tagstep, tagstep);   \
	DCL_CALC_SIMPLE_BUTT_i(c0, d0, iSIGNed);                  \
	DCL_CALC_SIMPLE_BUTT_i(c1, d1, iSIGNed);                  \
	STORE_SIMPLE_BUTTERFLIES_i(d0, c0);                       \
	STORE_SIMPLE_BUTTERFLIES_i(d1, c1)

/* *********************************************************** */

/* sequence to perform simple butt on 8+8 complex elems sequent */
#define	DEAL_8_8_SIMPLE_SEQUENT(ptr_beg, tagstep, iSIGNed)      \
	DEAL_8_SIMPLE_SEQUENT(ptr_beg, tagstep, iSIGNed)        \
	{                                                       \
	    DCL_4_CMPLX_PTRS(d2, d0_0 + two_four_##tagstep,     \
		tagstep);                                       \
	    DCL_4_CMPLX_PTRS(d3, d0_0 + three_four_##tagstep,   \
		tagstep);                                       \
	    DCL_CALC_SIMPLE_BUTT(c2, d2, iSIGNed);              \
	    DCL_CALC_SIMPLE_BUTT(c3, d3, iSIGNed);              \
	    STORE_SIMPLE_BUTTERFLIES(d2, c2);                   \
	    STORE_SIMPLE_BUTTERFLIES(d3, c3);                   \
	}

#define	DEAL_8_8_SIMPLE_SEQUENT_i(ptr_beg, tagstep, iSIGNed)      \
	DEAL_8_SIMPLE_SEQUENT_i(ptr_beg, tagstep, iSIGNed)        \
	{                                                       \
	    DCL_4_CMPLX_PTRS_i(d2, d0_0 + two_four_##tagstep,     \
		tagstep);                                       \
	    DCL_4_CMPLX_PTRS_i(d3, d0_0 + three_four_##tagstep,   \
		tagstep);                                       \
	    DCL_CALC_SIMPLE_BUTT_i(c2, d2, iSIGNed);              \
	    DCL_CALC_SIMPLE_BUTT_i(c3, d3, iSIGNed);              \
	    STORE_SIMPLE_BUTTERFLIES_i(d2, c2);                   \
	    STORE_SIMPLE_BUTTERFLIES_i(d3, c3);                   \
	}

/* *********************************************************** */

/* sequence to perform normal butt on 4+4 complex elems sequent */
#define	DEAL_8_NORMAL_SEQUENT(ptr_beg, tagstep, rot,                 \
	rot_half, r_rh, iSIGNed)                                     \
	DCL_4_CMPLX_PTRS(d0, ptr_beg, tagstep);                      \
	DCL_4_CMPLX_PTRS(d1, d0_0 + four_##tagstep, tagstep);        \
	DCL_CALC_NORMAL_BUTT(d0, 0, rot, rot_half, r_rh, iSIGNed);   \
	DCL_CALC_NORMAL_BUTT(d1, 1, rot, rot_half, r_rh, iSIGNed);   \
	STORE_USUAL_BUTTERFLIES(d0, 0);                              \
	STORE_USUAL_BUTTERFLIES(d1, 1)

#define	DEAL_8_NORMAL_SEQUENT_i(ptr_beg, tagstep, rot,                 \
	rot_half, r_rh, iSIGNed)                                     \
	DCL_4_CMPLX_PTRS_i(d0, ptr_beg, tagstep);                      \
	DCL_4_CMPLX_PTRS_i(d1, d0_0 + four_##tagstep, tagstep);        \
	DCL_CALC_NORMAL_BUTT_i(d0, 0, rot, rot_half, r_rh, iSIGNed);   \
	DCL_CALC_NORMAL_BUTT_i(d1, 1, rot, rot_half, r_rh, iSIGNed);   \
	STORE_USUAL_BUTTERFLIES_i(d0, 0);                              \
	STORE_USUAL_BUTTERFLIES_i(d1, 1)

/* *********************************************************** */

/* sequence to continue DEAL_8_NORMAL_SEQUENT on next 8 elems */
#define	DEAL_8_8_NORMAL_SEQUENT(ptr_beg, tagstep, rot,                   \
	rot_half, r_rh, iSIGNed)                                         \
	DEAL_8_NORMAL_SEQUENT(ptr_beg, tagstep, rot, rot_half, r_rh,     \
	    iSIGNed); {                                                  \
	    DCL_4_CMPLX_PTRS(d2, d0_0 + two_four_##tagstep,              \
		tagstep);                                                \
	    DCL_4_CMPLX_PTRS(d3, d0_0 + three_four_##tagstep,            \
		tagstep);                                                \
	    DCL_CALC_NORMAL_BUTT(d2, 2, rot, rot_half, r_rh, iSIGNed);   \
	    DCL_CALC_NORMAL_BUTT(d3, 3, rot, rot_half, r_rh, iSIGNed);   \
	    STORE_USUAL_BUTTERFLIES(d2, 2);                              \
	    STORE_USUAL_BUTTERFLIES(d3, 3);                              \
	}

#define	DEAL_8_8_NORMAL_SEQUENT_i(ptr_beg, tagstep, rot,                   \
	rot_half, r_rh, iSIGNed)                                         \
	DEAL_8_NORMAL_SEQUENT_i(ptr_beg, tagstep, rot, rot_half, r_rh,     \
	    iSIGNed); {                                                  \
	    DCL_4_CMPLX_PTRS_i(d2, d0_0 + two_four_##tagstep,              \
		tagstep);                                                \
	    DCL_4_CMPLX_PTRS_i(d3, d0_0 + three_four_##tagstep,            \
		tagstep);                                                \
	    DCL_CALC_NORMAL_BUTT_i(d2, 2, rot, rot_half, r_rh, iSIGNed);   \
	    DCL_CALC_NORMAL_BUTT_i(d3, 3, rot, rot_half, r_rh, iSIGNed);   \
	    STORE_USUAL_BUTTERFLIES_i(d2, 2);                              \
	    STORE_USUAL_BUTTERFLIES_i(d3, 3);                              \
	}

/* *********************************************************** */

/* sequence to perform simple butt on 8 complex elems sequent */
#define	DEAL_8_SIMPLE_SEQUENT_RI(ptr_beg, tagstep, iSIGNed)         \
	DCL_4_CMPLX_PTRS_RI(d0, ptr_beg, tagstep);                  \
	DCL_4_NEXT_PTRS_RI(d1, ptr_beg, four_##tagstep, tagstep);   \
	DCL_CALC_SIMPLE_BUTT_RI(c0, d0, iSIGNed);                   \
	DCL_CALC_SIMPLE_BUTT_RI(c1, d1, iSIGNed);                   \
	STORE_SIMPLE_BUTTERFLIES_RI(d0, c0);                        \
	STORE_SIMPLE_BUTTERFLIES_RI(d1, c1)

#define	DEAL_8_SIMPLE_SEQUENT_RI_i(ptr_beg, tagstep, iSIGNed)         \
	DCL_4_CMPLX_PTRS_RI_i(d0, ptr_beg, tagstep);                  \
	DCL_4_NEXT_PTRS_RI_i(d1, ptr_beg, four_##tagstep, tagstep);   \
	DCL_CALC_SIMPLE_BUTT_RI_i(c0, d0, iSIGNed);                   \
	DCL_CALC_SIMPLE_BUTT_RI_i(c1, d1, iSIGNed);                   \
	STORE_SIMPLE_BUTTERFLIES_RI_i(d0, c0);                        \
	STORE_SIMPLE_BUTTERFLIES_RI_i(d1, c1)

/* *********************************************************** */

/* sequence to perform simple butt on 8+8 complex elems sequent */
#define	DEAL_8_8_SIMPLE_SEQUENT_RI(ptr_beg, tagstep, iSIGNed)       \
	DEAL_8_SIMPLE_SEQUENT_RI(ptr_beg, tagstep, iSIGNed); {      \
	    DCL_4_NEXT_PTRS_RI(d2, ptr_beg, two_four_##tagstep,     \
		tagstep);                                           \
	    DCL_4_NEXT_PTRS_RI(d3, ptr_beg, three_four_##tagstep,   \
		tagstep);                                           \
	    DCL_CALC_SIMPLE_BUTT_RI(c2, d2, iSIGNed);               \
	    DCL_CALC_SIMPLE_BUTT_RI(c3, d3, iSIGNed);               \
	    STORE_SIMPLE_BUTTERFLIES_RI(d2, c2);                    \
	    STORE_SIMPLE_BUTTERFLIES_RI(d3, c3);                    \
	}

#define	DEAL_8_8_SIMPLE_SEQUENT_RI_i(ptr_beg, tagstep, iSIGNed)       \
	DEAL_8_SIMPLE_SEQUENT_RI_i(ptr_beg, tagstep, iSIGNed); {      \
	    DCL_4_NEXT_PTRS_RI_i(d2, ptr_beg, two_four_##tagstep,     \
		tagstep);                                           \
	    DCL_4_NEXT_PTRS_RI_i(d3, ptr_beg, three_four_##tagstep,   \
		tagstep);                                           \
	    DCL_CALC_SIMPLE_BUTT_RI_i(c2, d2, iSIGNed);               \
	    DCL_CALC_SIMPLE_BUTT_RI_i(c3, d3, iSIGNed);               \
	    STORE_SIMPLE_BUTTERFLIES_RI_i(d2, c2);                    \
	    STORE_SIMPLE_BUTTERFLIES_RI_i(d3, c3);                    \
	}

/* *********************************************************** */

/* sequence to perform normal butt on 4+4 complex elems sequent */
#define	DEAL_8_NORMAL_SEQUENT_RI(ptr_beg, tagstep, rot,                 \
	rot_half, r_rh, iSIGNed)                                        \
	DCL_4_CMPLX_PTRS_RI(d0, ptr_beg, tagstep);                      \
	DCL_4_NEXT_PTRS_RI(d1, ptr_beg, four_##tagstep, tagstep);       \
	DCL_CALC_NORMAL_BUTT_RI(d0, 0, rot, rot_half, r_rh, iSIGNed);   \
	DCL_CALC_NORMAL_BUTT_RI(d1, 1, rot, rot_half, r_rh, iSIGNed);   \
	STORE_USUAL_BUTTERFLIES_RI(d0, 0);                              \
	STORE_USUAL_BUTTERFLIES_RI(d1, 1)

#define	DEAL_8_NORMAL_SEQUENT_RI_i(ptr_beg, tagstep, rot,                 \
	rot_half, r_rh, iSIGNed)                                        \
	DCL_4_CMPLX_PTRS_RI_i(d0, ptr_beg, tagstep);                      \
	DCL_4_NEXT_PTRS_RI_i(d1, ptr_beg, four_##tagstep, tagstep);       \
	DCL_CALC_NORMAL_BUTT_RI_i(d0, 0, rot, rot_half, r_rh, iSIGNed);   \
	DCL_CALC_NORMAL_BUTT_RI_i(d1, 1, rot, rot_half, r_rh, iSIGNed);   \
	STORE_USUAL_BUTTERFLIES_RI_i(d0, 0);                              \
	STORE_USUAL_BUTTERFLIES_RI_i(d1, 1)

/* *********************************************************** */

/* sequence to continue DEAL_8_NORMAL_SEQUENT on next 8 elems */
#define	DEAL_8_8_NORMAL_SEQUENT_RI(ptr_beg, tagstep, rot,                 \
	rot_half, r_rh, iSIGNed)                                          \
	DEAL_8_NORMAL_SEQUENT_RI(ptr_beg, tagstep, rot, rot_half, r_rh,   \
	    iSIGNed); {                                                   \
	    DCL_4_NEXT_PTRS_RI(d2, ptr_beg, two_four_##tagstep,           \
		tagstep);                                                 \
	    DCL_4_NEXT_PTRS_RI(d3, ptr_beg, three_four_##tagstep,         \
		tagstep);                                                 \
	    DCL_CALC_NORMAL_BUTT_RI(d2, 2, rot, rot_half, r_rh,           \
		iSIGNed);                                                 \
	    DCL_CALC_NORMAL_BUTT_RI(d3, 3, rot, rot_half, r_rh,           \
		iSIGNed);                                                 \
	    STORE_USUAL_BUTTERFLIES_RI(d2, 2);                            \
	    STORE_USUAL_BUTTERFLIES_RI(d3, 3);                            \
	}

#define	DEAL_8_8_NORMAL_SEQUENT_RI_i(ptr_beg, tagstep, rot,                 \
	rot_half, r_rh, iSIGNed)                                          \
	DEAL_8_NORMAL_SEQUENT_RI_i(ptr_beg, tagstep, rot, rot_half, r_rh,   \
	    iSIGNed); {                                                   \
	    DCL_4_NEXT_PTRS_RI_i(d2, ptr_beg, two_four_##tagstep,           \
		tagstep);                                                 \
	    DCL_4_NEXT_PTRS_RI_i(d3, ptr_beg, three_four_##tagstep,         \
		tagstep);                                                 \
	    DCL_CALC_NORMAL_BUTT_RI_i(d2, 2, rot, rot_half, r_rh,           \
		iSIGNed);                                                 \
	    DCL_CALC_NORMAL_BUTT_RI_i(d3, 3, rot, rot_half, r_rh,           \
		iSIGNed);                                                 \
	    STORE_USUAL_BUTTERFLIES_RI_i(d2, 2);                            \
	    STORE_USUAL_BUTTERFLIES_RI_i(d3, 3);                            \
	}

/* *********************************************************** */

#define	CACHE1_ELEMS	(1 << CACHE1_ORD)

#define	MSIN_PI_BY4                                                           \
	    ((precise_t)-0.70710678118654752440084436210484903928483593768846)

#define	MSIN_PI_BY4_i  ((mlib_s64)-23170)

/* Q30 */
#define	MSIN_PI_BY4_i_Q    ((mlib_s64)(-SIN_PI_BY4_i_Q))

/* *********************************************************** */

#define	DEF_CVAR(x)	mlib_d64 x##_r, x##_i

#define	DEF_CVAR_i(x)	mlib_s64 x##_r, x##_i

/* *********************************************************** */

#define	DEF_CVAR4(x)                                            \
	mlib_d64 x##0_r, x##0_i, x##1_r, x##1_i,                \
	    x##2_r, x##2_i, x##3_r, x##3_i

#define	DEF_CVAR4_i(x)                                            \
	mlib_s64 x##0_r, x##0_i, x##1_r, x##1_i,                \
	    x##2_r, x##2_i, x##3_r, x##3_i

/* *********************************************************** */

#define	DEF_CVAR4x(x)                                           \
	mlib_d64 x##1_r, x##1_i, x##2_r, x##2_i,                \
	    x##3_r, x##3_i

#define	DEF_CVAR4x_i(x)                                           \
	mlib_s64 x##1_r, x##1_i, x##2_r, x##2_i,                \
	    x##3_r, x##3_i

/* *********************************************************** */

#define	DEF_CVAR8(x)                                            \
	mlib_d64 x##0_r, x##0_i, x##1_r, x##1_i,                \
	    x##2_r, x##2_i, x##3_r, x##3_i, x##4_r,             \
	    x##4_i, x##5_r, x##5_i, x##6_r, x##6_i,             \
	    x##7_r, x##7_i

#define	DEF_CVAR8_i(x)                                            \
	mlib_s64 x##0_r, x##0_i, x##1_r, x##1_i,                \
	    x##2_r, x##2_i, x##3_r, x##3_i, x##4_r,             \
	    x##4_i, x##5_r, x##5_i, x##6_r, x##6_i,             \
	    x##7_r, x##7_i

/* *********************************************************** */

#define	DEF_CVAR8x(x)                                           \
	mlib_d64 x##1_r, x##1_i, x##2_r, x##2_i,                \
	    x##3_r, x##3_i, x##4_r, x##4_i, x##5_r,             \
	    x##5_i, x##6_r, x##6_i, x##7_r, x##7_i

#define	DEF_CVAR8x_i(x)                                           \
	mlib_s64 x##1_r, x##1_i, x##2_r, x##2_i,                \
	    x##3_r, x##3_i, x##4_r, x##4_i, x##5_r,             \
	    x##5_i, x##6_r, x##6_i, x##7_r, x##7_i

/* *********************************************************** */

#define	INI_CVAR(x, R, I)	mlib_d64 x##_r = R, x##_i = I

#define	INI_CVAR_i(x, R, I)	mlib_s64 x##_r = R, x##_i = I

/* *********************************************************** */

#define	C_LD(x, dat)	x##_r = (dat)[0]; x##_i = (dat)[1]

#define	C_LD_i(x, dat)	x##_r = (dat)[0]; x##_i = (dat)[1]

/* *********************************************************** */

#define	C_ST(x, dat)	(dat)[0] = x##_r; (dat)[1] = x##_i

#define	C_ST_i(x, dat)	(dat)[0] = x##_r; (dat)[1] = x##_i

/* *********************************************************** */

#define	C_LD_RI(x, dat, indx)                                   \
	x##_r = (dat##_r)[indx];                                \
	x##_i = (dat##_i)[indx]

#define	C_LD_RI_i(x, dat, indx)                                   \
	x##_r = (dat##_r)[indx];                                \
	x##_i = (dat##_i)[indx]

/* *********************************************************** */

#define	C_ST_RI(x, dat, indx)                                   \
	(dat##_r)[indx] = x##_r;                                \
	(dat##_i)[indx] = x##_i

#define	C_ST_RI_i(x, dat, indx)                                   \
	(dat##_r)[indx] = x##_r;                                \
	(dat##_i)[indx] = x##_i

/* *********************************************************** */

#define	CMUL(z, x, y)                                           \
	z##_r = x##_r * y##_r - x##_i * y##_i;                  \
	z##_i = x##_r * y##_i + x##_i * y##_r

#define	CMUL_i_old(z, x, y)                                           \
	z##_r = MULT_Q_i(x##_r, y##_r) - MULT_Q_i(x##_i, y##_i);  \
	z##_i = MULT_Q_i(x##_r, y##_i) + MULT_Q_i(x##_i, y##_r)

#define	CMUL_i(z, x, y)                                           \
	z##_r = MULT_ED_Q_i(x##_r * y##_r - x##_i * y##_i);       \
	z##_i = MULT_ED_Q_i(x##_r * y##_i + x##_i * y##_r)

/* *********************************************************** */

#define	CROT(y, x)                                              \
	{                                                       \
	    mlib_d64 y_r = y##_r;                               \
	                                                        \
	    y##_r = x##_r * y##_r - x##_i * y##_i;              \
	    y##_i = x##_r * y##_i + x##_i * y_r;                \
	}

#define	CROT_i_old(y, x)                                              \
	{                                                       \
	    mlib_s64 y_r = y##_r;                               \
	                                                        \
	    y##_r = MULT_Q_i(x##_r, y##_r) - MULT_Q_i(x##_i, y##_i); \
	    y##_i = MULT_Q_i(x##_r, y##_i) + MULT_Q_i(x##_i, y_r);   \
	}

#define	CROT_i(y, x)                                              \
	{                                                       \
	    mlib_s64 y_r = y##_r;                               \
	                                                        \
	    y##_r = MULT_ED_Q_i(x##_r * y##_r - x##_i * y##_i);              \
	    y##_i = MULT_ED_Q_i(x##_r * y##_i + x##_i * y_r);                \
	}

/* *********************************************************** */

#if ISIGN == INVERS_FFT

#define	CROT_P1(y, x)                                           \
	y##_r = SIN_PI_BY4 * (x##_r - x##_i);                   \
	y##_i = SIN_PI_BY4 * (x##_i + x##_r)

#define	CROT_P1_i(y, x)                                           \
	y##_r = MULT_Q_i(SIN_PI_BY4_i_Q, (x##_r - x##_i));                   \
	y##_i = MULT_Q_i(SIN_PI_BY4_i_Q, (x##_i + x##_r))

#define	CROT_P3(y, x)                                           \
	y##_r = MSIN_PI_BY4 * (x##_i + x##_r);                  \
	y##_i = SIN_PI_BY4 * (x##_r - x##_i)

#define	CROT_P3_i(y, x)                                           \
	y##_r = MULT_Q_i(MSIN_PI_BY4_i_Q, (x##_i + x##_r));                  \
	y##_i = MULT_Q_i(SIN_PI_BY4_i_Q, (x##_r - x##_i))

#else /* ISIGN == INVERS_FFT */

#define	CROT_P1(y, x)                                           \
	y##_r = SIN_PI_BY4 * (x##_r + x##_i);                   \
	y##_i = SIN_PI_BY4 * (x##_i - x##_r)

#define	CROT_P1_i(y, x)                                           \
	y##_r = MULT_Q_i(SIN_PI_BY4_i_Q, (x##_r + x##_i));                   \
	y##_i = MULT_Q_i(SIN_PI_BY4_i_Q, (x##_i - x##_r))

#define	CROT_P3(y, x)                                           \
	y##_r = SIN_PI_BY4 * (x##_i - x##_r);                   \
	y##_i = MSIN_PI_BY4 * (x##_i + x##_r)

#define	CROT_P3_i(y, x)                                           \
	y##_r = MULT_Q_i(SIN_PI_BY4_i_Q, (x##_i - x##_r));                   \
	y##_i = MULT_Q_i(MSIN_PI_BY4_i_Q, (x##_i + x##_r))

#endif /* ISIGN == INVERS_FFT */

/* *********************************************************** */

#define	ROTATE_CMPLX_TMP(dest, incr, tmp)                       \
	{                                                       \
	    dest##_r = ((tmp =                                  \
		dest##_r) * incr##_r -                          \
		dest##_i * incr##_i);                           \
	    dest##_i =                                          \
		tmp * incr##_i + dest##_i * incr##_r;           \
	}

#define	ROTATE_CMPLX_TMP_i(dest, incr, tmp)                       \
	{                                                       \
	    dest##_r = (MULT_ED_Q_i((tmp =                                  \
		dest##_r) * incr##_r -                          \
		dest##_i * incr##_i));                           \
	    dest##_i =                                          \
		MULT_ED_Q_i(tmp * incr##_i + dest##_i * incr##_r);           \
	}

#define	ROTATE_CMPLX_TMP_i_old(dest, incr, tmp)                       \
	{                                                       \
	    dest##_r = (MULT_Q_i((tmp =                                  \
		dest##_r), incr##_r) -                          \
		MULT_Q_i(dest##_i, incr##_i));                           \
	    dest##_i =                                          \
		MULT_Q_i(tmp, incr##_i) + MULT_Q_i(dest##_i, incr##_r); \
	}

/* *********************************************************** */

#define	CADD(z, x, y)	z##_r = x##_r + y##_r; z##_i = x##_i + y##_i

#define	CADD_i(z, x, y)	z##_r = x##_r + y##_r; z##_i = x##_i + y##_i

/* *********************************************************** */

#define	CSUB(z, x, y)	z##_r = x##_r - y##_r; z##_i = x##_i - y##_i

#define	CSUB_i(z, x, y)	z##_r = x##_r - y##_r; z##_i = x##_i - y##_i

/* *********************************************************** */

#if ISIGN == INVERS_FFT

#define	RSUB(z, x, y)	z##_i = x##_r - y##_r; z##_r = y##_i - x##_i

#define	RSUB_i(z, x, y)	z##_i = x##_r - y##_r; z##_r = y##_i - x##_i

#else /* ISIGN == INVERS_FFT */

#define	RSUB(z, x, y)   z##_i = y##_r - x##_r; z##_r = x##_i - y##_i

#define	RSUB_i(z, x, y)	z##_i = y##_r - x##_r; z##_r = x##_i - y##_i

#endif /* ISIGN == INVERS_FFT */

/* *********************************************************** */

#define	CBUT(a, b, N0, N1)                                      \
	CADD(b##N0, a##N0, a##N1);                              \
	CSUB(b##N1, a##N0, a##N1)

#define	CBUT_i(a, b, N0, N1)                                      \
	CADD_i(b##N0, a##N0, a##N1);                              \
	CSUB_i(b##N1, a##N0, a##N1)

/* *********************************************************** */

#define	RBUT(a, b, N0, N1)                                      \
	CADD(b##N0, a##N0, a##N1);                              \
	RSUB(b##N1, a##N0, a##N1)

#define	RBUT_i(a, b, N0, N1)                                      \
	CADD_i(b##N0, a##N0, a##N1);                              \
	RSUB_i(b##N1, a##N0, a##N1)

/* *********************************************************** */

#define	INI_ROT(rot, ord_lay)                                   \
	mlib_d64 sin_half##rot = dsign * SINUS(ord_lay + 1);    \
	mlib_d64 rot##_r =                                      \
	    ONE - TWO * sin_half##rot * sin_half##rot;          \
	mlib_d64 rot##_i = dsign * SINUS(ord_lay)

#define	INI_ROT_i(rot, ord_lay)                                   \
	mlib_s64 sin_half##rot = dsign * SINUS_i(ord_lay + 1);    \
	mlib_s64 rot##_r =                                      \
	    ONE_i_Q - MULT_Q_i(TWO_i_Q0 * sin_half##rot, sin_half##rot); \
	mlib_s64 rot##_i = dsign * SINUS_i(ord_lay)

/* *********************************************************** */

#define	DCL_DST_PTR_RI(pbeg, dst, shift)                        \
	deal_t *pbeg##_r = (dst##_r) + shift, *pbeg##_i =       \
	    (dst##_i) + shift

#define	DCL_DST_PTR_RI_i(pbeg, dst, shift)                        \
	mlib_s64 *pbeg##_r = (dst##_r) + shift, *pbeg##_i =       \
	    (dst##_i) + shift

/* *********************************************************** */

#define	DCL_DST_PTR_C(pbeg, dst, shift)                         \
	dtCmplx *CONST pbeg = (dtCmplx *) (dst) + shift

#define	DCL_DST_PTR_C_i(pbeg, dst, shift)                         \
	dtCmplx_i *CONST pbeg = (dtCmplx_i *) (dst) + shift

/* *********************************************************** */

#define	DCL_4_CMPLX_PTRS_C(tag, ptr_beg, tagstp)                \
	DCL_4_CMPLX_PTRS(tag, ptr_beg, tagstp)

#define	DCL_4_CMPLX_PTRS_C_i(tag, ptr_beg, tagstp)                \
	DCL_4_CMPLX_PTRS_i(tag, ptr_beg, tagstp)

/* *********************************************************** */

#define	DCL_4_NEXT_PTRS_C(tag, ptr_beg, shift, tagstp)          \
	DCL_4_CMPLX_PTRS_C(tag, ptr_beg + shift, tagstp)

#define	DCL_4_NEXT_PTRS_C_i(tag, ptr_beg, shift, tagstp)          \
	DCL_4_CMPLX_PTRS_C_i(tag, ptr_beg + shift, tagstp)

/* *********************************************************** */

#define	BUTTERFLY_CMPLX_CMPLX_C(dest1, dest2, src1, src2)       \
	{                                                       \
	    dest1->r = src1##_r + src2##_r;                     \
	    dest1->i = src1##_i + src2##_i;                     \
	    dest2->r = src1##_r - src2##_r;                     \
	    dest2->i = src1##_i - src2##_i;                     \
	}

#define	BUTTERFLY_CMPLX_CMPLX_C_i(dest1, dest2, src1, src2)       \
	{                                                       \
	    dest1->r = src1##_r + src2##_r;                     \
	    dest1->i = src1##_i + src2##_i;                     \
	    dest2->r = src1##_r - src2##_r;                     \
	    dest2->i = src1##_i - src2##_i;                     \
	}

/* *********************************************************** */

/* special for 1st-3layers */
#define	BUTTERFLY_CMPLX_CMPLX_ROT(dest1, dest2, rot2, dstag)       \
	BUTTERFLY_CMPLX_CMPLX_##dstag(dest1, dest2, dest1, rot2)

#define	BUTTERFLY_CMPLX_CMPLX_ROT_i(dest1, dest2, rot2, dstag)       \
	BUTTERFLY_CMPLX_CMPLX_##dstag##_i(dest1, dest2, dest1, rot2)

/* *********************************************************** */

/* calc components of simple butterfly for 1st-3-layers */
#define	CALC_INTERMED_BUTT(dest1, dest2, src1, src2)            \
	dest1##_r = src1##_r + src2##_r, dest1##_i =            \
	    src1##_i + src2##_i, dest2##_r =                    \
	    src1##_r - src2##_r, dest2##_i =                    \
	    src1##_i - src2##_i

#define	CALC_INTERMED_BUTT_i(dest1, dest2, src1, src2)            \
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

#define	CALC_SIMPLE_BUTTS_i(dst, src)                             \
	CALC_INTERMED_BUTT_i(dst##_0, dst##_2, src##_0_pls_1,     \
	    src##_2_pls_3);                                     \
	CALC_INTERMED_BUTT_i(dst##_1, dst##_3, src##_0_mns_1,     \
	    src##_2_mns_3)

/* *********************************************************** */

/* sequence to perform simple butt on 4 complex elems */
#define	DEAL_4_SIMPLE_C(ptr_beg, tagstep, iSIGNed)              \
	DEAL_4_SIMPLE(ptr_beg, tagstep, iSIGNed)

#define	DEAL_4_SIMPLE_C_i(ptr_beg, tagstep, iSIGNed)              \
	DEAL_4_SIMPLE_i(ptr_beg, tagstep, iSIGNed)

/* *********************************************************** */

/* declare/define components for simple butterflies */
#define	DCL_CALC_SIMPLE_BUTT_C(dst, src, iSIGNed)               \
	DCL_CALC_SIMPLE_BUTT(dst, src, iSIGNed)

#define	DCL_CALC_SIMPLE_BUTT_C_i(dst, src, iSIGNed)               \
	DCL_CALC_SIMPLE_BUTT_i(dst, src, iSIGNed)

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

#define	DCL_CALC_3RD_LAY_BUTT_i(src, iSIGNed)                     \
	CONST mlib_s64 src##_0r_r = src##_0_r, src##_0r_i =       \
	    src##_0_i, src##_1r_r =                             \
	    MULT_Q_i(SIN_PI_BY4_i_Q, (src##_1_r - iSIGNed(src##_1_i))),      \
	    src##_1r_i =                                        \
	    MULT_Q_i(SIN_PI_BY4_i_Q, (src##_1_i + iSIGNed(src##_1_r))),      \
	    src##_2r_r = -iSIGNed(src##_2_i), src##_2r_i =      \
	iSIGNed(src##_2_r), src##_3r_r =                        \
	    MULT_Q_i((-SIN_PI_BY4_i_Q), (iSIGNed(src##_3_i) + src##_3_r)),   \
	    src##_3r_i =                                        \
	    MULT_Q_i(SIN_PI_BY4_i_Q, (iSIGNed(src##_3_r) - src##_3_i))

/* *********************************************************** */

/* perform & store 1st-3-layers butterflies */
#define	STORE_3RD_LAY_BUTTERFLIES(src0, src1, dstag)            \
	BUTTERFLY_CMPLX_CMPLX_ROT(src0##_0, src1##_0,           \
	    src1##_0r, dstag);                                  \
	BUTTERFLY_CMPLX_CMPLX_ROT(src0##_1, src1##_1,           \
	    src1##_1r, dstag);                                  \
	BUTTERFLY_CMPLX_CMPLX_ROT(src0##_2, src1##_2,           \
	    src1##_2r, dstag);                                  \
	BUTTERFLY_CMPLX_CMPLX_ROT(src0##_3, src1##_3,           \
	    src1##_3r, dstag)

#define	STORE_3RD_LAY_BUTTERFLIES_i(src0, src1, dstag)            \
	BUTTERFLY_CMPLX_CMPLX_ROT_i(src0##_0, src1##_0,           \
	    src1##_0r, dstag);                                  \
	BUTTERFLY_CMPLX_CMPLX_ROT_i(src0##_1, src1##_1,           \
	    src1##_1r, dstag);                                  \
	BUTTERFLY_CMPLX_CMPLX_ROT_i(src0##_2, src1##_2,           \
	    src1##_2r, dstag);                                  \
	BUTTERFLY_CMPLX_CMPLX_ROT_i(src0##_3, src1##_3,           \
	    src1##_3r, dstag)

/* *********************************************************** */

#define	FIRST_3_LAYERS(ord_arr, iSIGNed, dst, dstag)                      \
	for (iii = 0; iii < nnn; iii += 8) {                              \
	    deal_t d0_0_r, d0_0_i, d0_1_r, d0_1_i, d0_2_r, d0_2_i,        \
		d0_3_r, d0_3_i, d1_0_r, d1_0_i, d1_1_r, d1_1_i, d1_2_r,   \
		d1_2_i, d1_3_r, d1_3_i;                                   \
	    DCL_DST_PTR_##dstag(pbeg, dst, iii);                          \
	    DCL_4_CMPLX_PTRS_##dstag(d0, pbeg, 1);                        \
	    DCL_4_NEXT_PTRS_##dstag(d1, pbeg, 4, 1);                      \
	    DCL_CALC_SIMPLE_BUTT_##dstag(c0, d0, iSIGNed);                \
	    DCL_CALC_SIMPLE_BUTT_##dstag(c1, d1, iSIGNed);                \
	    CALC_SIMPLE_BUTTS(d0, c0);                                    \
	    CALC_SIMPLE_BUTTS(d1, c1); {                                  \
		DCL_CALC_3RD_LAY_BUTT(d1, iSIGNed);                       \
		STORE_3RD_LAY_BUTTERFLIES(d0, d1, dstag);                 \
	    }                                                             \
	}

#define	FIRST_3_LAYERS_i(ord_arr, iSIGNed, dst, dstag)                      \
	for (iii = 0; iii < nnn; iii += 8) {                              \
	    mlib_s64 d0_0_r, d0_0_i, d0_1_r, d0_1_i, d0_2_r, d0_2_i,        \
		d0_3_r, d0_3_i, d1_0_r, d1_0_i, d1_1_r, d1_1_i, d1_2_r,   \
		d1_2_i, d1_3_r, d1_3_i;                                   \
	    DCL_DST_PTR_##dstag##_i(pbeg, dst, iii);                          \
	    DCL_4_CMPLX_PTRS_##dstag##_i(d0, pbeg, 1);                        \
	    DCL_4_NEXT_PTRS_##dstag##_i(d1, pbeg, 4, 1);                      \
	    DCL_CALC_SIMPLE_BUTT_##dstag##_i(c0, d0, iSIGNed);                \
	    DCL_CALC_SIMPLE_BUTT_##dstag##_i(c1, d1, iSIGNed);                \
	    CALC_SIMPLE_BUTTS_i(d0, c0);                                    \
	    CALC_SIMPLE_BUTTS_i(d1, c1); {                                  \
		DCL_CALC_3RD_LAY_BUTT_i(d1, iSIGNed);                       \
		STORE_3RD_LAY_BUTTERFLIES_i(d0, d1, dstag);                 \
	    }                                                             \
	}

/* *********************************************************** */

#define	FIRST_2_LAYERS(ord_arr, iSIGNed, dst, dstag)            \
	for (iii = 0; iii < nnn; iii += 4) {                    \
	    DCL_DST_PTR_##dstag(pbeg, dst, iii);                \
	    DEAL_4_SIMPLE_##dstag(pbeg, 1, iSIGNed);            \
	}

#define	FIRST_2_LAYERS_i(ord_arr, iSIGNed, dst, dstag)            \
	for (iii = 0; iii < nnn; iii += 4) {                    \
	    DCL_DST_PTR_##dstag##_i(pbeg, dst, iii);                \
	    DEAL_4_SIMPLE_##dstag##_i(pbeg, 1, iSIGNed);            \
	}

/* *********************************************************** */

#ifdef _MSC_VER
#pragma optimize("", off)
#endif /* _MSC_VER */

/* *********************************************************** */

void FUNC(
	order4) (
	mlib_d64 *pal64data,
	mlib_d64 *data_i,
	mlib_s32 order)
{
	dtCmplx *CONST dat = (dtCmplx *) pal64data;

	DEAL_ALL_ORD4(isign, dat, SSIGN);
}

void FUNC_i(
	order4) (
	mlib_s64 *pal64data,
	mlib_s64 *data_i,
	mlib_s32 order)
{
	dtCmplx_i *CONST dat = (dtCmplx_i *) pal64data;

	DEAL_ALL_ORD4_i(isign, dat, SSIGN_i);
}

/* *********************************************************** */

void FUNC(
	first2) (
	mlib_d64 *pal64data,
	mlib_d64 *data_i,
	mlib_s32 n_ord_arr)
{
	mlib_s32 CONST nnn = 1 << n_ord_arr;
	mlib_s32 iii;

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
	FIRST_2_LAYERS(n_ord_arr, SSIGN, pal64data, C);
}

void FUNC_i(
	first2) (
	mlib_s64 *pal64data,
	mlib_s64 *data_i,
	mlib_s32 n_ord_arr)
{
	mlib_s32 CONST nnn = 1 << n_ord_arr;
	mlib_s32 iii;

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
	FIRST_2_LAYERS_i(n_ord_arr, SSIGN_i, pal64data, C);
}

/* *********************************************************** */

void FUNC(
	first3) (
	mlib_d64 *pal64data,
	mlib_d64 *data_i,
	mlib_s32 n_ord_arr)
{
	mlib_s32 CONST nnn = 1 << n_ord_arr;
	mlib_s32 iii;

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
	FIRST_3_LAYERS(n_ord_arr, SSIGN, pal64data, C);
}

void FUNC_i(
	first3) (
	mlib_s64 *pal64data,
	mlib_s64 *data_i,
	mlib_s32 n_ord_arr)
{
	mlib_s32 CONST nnn = 1 << n_ord_arr;
	mlib_s32 iii;

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
	FIRST_3_LAYERS_i(n_ord_arr, SSIGN_i, pal64data, C);
}

/* *********************************************************** */

#if NUM_SPECIAL_LAST >= 2

void FUNC(
	2LayersLast) (
	dtCmplx * dat,
	dtCmplx * data_i,
	mlib_s32 n_ord_arr)
{
	mlib_s32 isign = ISIGN;
	mlib_s32 n_ord_lay = n_ord_arr - 2;
	mlib_s32 n_len_arr = 1 << n_ord_arr;
	mlib_s32 n_step_half = n_len_arr >> 3;

	DCL_CALC_STEPS1234(n_step, n_ord_lay);
	DCL_CALC_INCR(incr_half, n_ord_lay + 1, 0);
	DCL_CALC_INCR(incr, n_ord_lay, 1);
	DCL_INIT_ROT(rot, incr);
	DCL_INIT_ROT(rot_half, incr_half);
	precise_t wtmp;
	mlib_s32 iii;

	DEAL_4_SIMPLE(dat, n_step, SSIGN);
	{
		DEAL_4_HALFWAY(dat + n_step_half, n_step, SSIGN);
	}

	for (iii = 1; iii < n_step_half; iii++) {
		DCL_MULT_ROT_ROT_HALF(rot, rot_half, r_rh);
		DEAL_8_USUAL_MIRR(dat + iii, dat + n_step - iii,
		    n_step, rot, rot_half, r_rh, SSIGN);
		ROTATE_CMPLX_TMP(rot, incr, wtmp);
		ROTATE_CMPLX_TMP(rot_half, incr_half, wtmp);
	}
}

void FUNC_i(
	2LayersLast) (
	dtCmplx_i * dat,
	dtCmplx_i * data_i,
	mlib_s32 n_ord_arr)
{
	mlib_s32 isign = ISIGN;
	mlib_s32 n_ord_lay = n_ord_arr - 2;
	mlib_s32 n_len_arr = 1 << n_ord_arr;
	mlib_s32 n_step_half = n_len_arr >> 3;

	DCL_CALC_STEPS1234_i(n_step, n_ord_lay);
	DCL_CALC_INCR_i(incr_half, n_ord_lay + 1, 0);
	DCL_CALC_INCR_i(incr, n_ord_lay, 1);
	DCL_INIT_ROT_i(rot, incr);
	DCL_INIT_ROT_i(rot_half, incr_half);
	mlib_s64 wtmp;
	mlib_s32 iii;

	DEAL_4_SIMPLE_i(dat, n_step, SSIGN_i);
	{
		DEAL_4_HALFWAY_i(dat + n_step_half, n_step, SSIGN_i);
	}

	for (iii = 1; iii < n_step_half; iii++) {
		DCL_MULT_ROT_ROT_HALF_i(rot, rot_half, r_rh);
		DEAL_8_USUAL_MIRR_i(dat + iii, dat + n_step - iii,
		    n_step, rot, rot_half, r_rh, SSIGN_i);
		ROTATE_CMPLX_TMP_i(rot, incr, wtmp);
		ROTATE_CMPLX_TMP_i(rot_half, incr_half, wtmp);
	}
}

#endif /* NUM_SPECIAL_LAST >= 2 */

/* *********************************************************** */

#if NUM_SPECIAL_LAST >= 3

void FUNC(
    2LayersLastM1) (
    dtCmplx * dat,
    dtCmplx * data_i,
    mlib_s32 n_ord_arr)
{
	mlib_s32 isign = ISIGN;
	mlib_s32 n_ord_lay = n_ord_arr - 3;

	DCL_CALC_STEPS1234(n_step, n_ord_lay);
	DCL_CALC_INCR(incr_half, n_ord_lay + 1, 0);
	DCL_CALC_INCR(incr, n_ord_lay, 1);
	DCL_INIT_ROT(rot, incr);
	DCL_INIT_ROT(rot_half, incr_half);
	precise_t wtmp;
	mlib_s32 iii;

	DEAL_8_SIMPLE_SEQUENT(dat, n_step, SSIGN);
	for (iii = 1; iii < n_step; iii++) {
		DCL_MULT_ROT_ROT_HALF(rot, rot_half, r_rh);
		DEAL_8_NORMAL_SEQUENT(dat + iii, n_step, rot, rot_half, r_rh,
			SSIGN);
		ROTATE_CMPLX_TMP(rot, incr, wtmp);
		ROTATE_CMPLX_TMP(rot_half, incr_half, wtmp);
	}
}

void FUNC_i(
	2LayersLastM1) (
	dtCmplx_i * dat,
	dtCmplx_i * data_i,
	mlib_s32 n_ord_arr)
{
	mlib_s32 isign = ISIGN;
	mlib_s32 n_ord_lay = n_ord_arr - 3;

	DCL_CALC_STEPS1234_i(n_step, n_ord_lay);
	DCL_CALC_INCR_i(incr_half, n_ord_lay + 1, 0);
	DCL_CALC_INCR_i(incr, n_ord_lay, 1);
	DCL_INIT_ROT_i(rot, incr);
	DCL_INIT_ROT_i(rot_half, incr_half);
	mlib_s64 wtmp;
	mlib_s32 iii;

	DEAL_8_SIMPLE_SEQUENT_i(dat, n_step, SSIGN_i);
	for (iii = 1; iii < n_step; iii++) {
		DCL_MULT_ROT_ROT_HALF_i(rot, rot_half, r_rh);
		DEAL_8_NORMAL_SEQUENT_i(dat + iii, n_step, rot, rot_half, r_rh,
		    SSIGN_i);
		ROTATE_CMPLX_TMP_i(rot, incr, wtmp);
		ROTATE_CMPLX_TMP_i(rot_half, incr_half, wtmp);
	}
}

#endif /* NUM_SPECIAL_LAST >= 3 */

/* *********************************************************** */

#if NUM_SPECIAL_LAST >= 4

void FUNC(
    2LayersLastM2) (
    dtCmplx * dat,
    dtCmplx * data_i,
    mlib_s32 n_ord_arr)
{
	mlib_s32 isign = ISIGN;
	mlib_s32 n_ord_lay = n_ord_arr - 4;

	DCL_CALC_STEPS1234(n_step, n_ord_lay);
	DCL_CALC_INCR(incr_half, n_ord_lay + 1, 0);
	DCL_CALC_INCR(incr, n_ord_lay, 1);
	DCL_INIT_ROT(rot, incr);
	DCL_INIT_ROT(rot_half, incr_half);
	CONST mlib_s32 two_four_n_step = n_step * 8;
	CONST mlib_s32 three_four_n_step = n_step * 12;
	precise_t wtmp;
	mlib_s32 iii;

	DEAL_8_8_SIMPLE_SEQUENT(dat, n_step, SSIGN);
	for (iii = 1; iii < n_step; iii++) {
		DCL_MULT_ROT_ROT_HALF(rot, rot_half, r_rh);
		DEAL_8_8_NORMAL_SEQUENT(dat + iii, n_step, rot, rot_half, r_rh,
			SSIGN);
		ROTATE_CMPLX_TMP(rot, incr, wtmp);
		ROTATE_CMPLX_TMP(rot_half, incr_half, wtmp);
	}
}

void FUNC_i(
	2LayersLastM2) (
	dtCmplx_i * dat,
	dtCmplx_i * data_i,
	mlib_s32 n_ord_arr)
{
	mlib_s32 isign = ISIGN;
	mlib_s32 n_ord_lay = n_ord_arr - 4;

	DCL_CALC_STEPS1234_i(n_step, n_ord_lay);
	DCL_CALC_INCR_i(incr_half, n_ord_lay + 1, 0);
	DCL_CALC_INCR_i(incr, n_ord_lay, 1);
	DCL_INIT_ROT_i(rot, incr);
	DCL_INIT_ROT_i(rot_half, incr_half);
	CONST mlib_s32 two_four_n_step = n_step * 8;
	CONST mlib_s32 three_four_n_step = n_step * 12;
	mlib_s64 wtmp;
	mlib_s32 iii;

	DEAL_8_8_SIMPLE_SEQUENT_i(dat, n_step, SSIGN_i);
	for (iii = 1; iii < n_step; iii++) {
		DCL_MULT_ROT_ROT_HALF_i(rot, rot_half, r_rh);
		DEAL_8_8_NORMAL_SEQUENT_i(dat + iii, n_step, rot, rot_half,
			r_rh, SSIGN_i);
		ROTATE_CMPLX_TMP_i(rot, incr, wtmp);
		ROTATE_CMPLX_TMP_i(rot_half, incr_half, wtmp);
	}
}

#endif /* NUM_SPECIAL_LAST >= 4 */

/* *********************************************************** */

void FUNC(
	2Layers) (
	mlib_d64 *dat,
	mlib_d64 *data_i,
	mlib_s32 n_ord_arr,
	mlib_s32 n_ord_lay)
{
	mlib_d64 dsign = ISIGN;
	mlib_s32 i, j;
	mlib_s32 dstep = 2 * (1 << n_ord_lay);
	mlib_s32 bsize = 2 * (1 << (n_ord_lay + 2));
	mlib_s32 len_arr = 2 * (1 << n_ord_arr);
	mlib_s32 n_left;

	INI_ROT(rot, n_ord_lay + 1);
	INI_CVAR(r1, 1, 0);
	INI_CVAR(r2, 1, 0);
	INI_CVAR(r3, 1, 0);

	n_left = n_ord_arr - n_ord_lay;

	if (n_left <= NUM_SPECIAL_LAST) {
#if NUM_SPECIAL_LAST >= 2

		if (n_left == 2) {
			FUNC(2LayersLast) ((void *)dat, (void *)data_i,
			    n_ord_arr);
		}
#endif /* NUM_SPECIAL_LAST >= 2 */
#if NUM_SPECIAL_LAST >= 3
		else if (n_left == 3) {
			FUNC(2LayersLastM1) ((void *)dat, (void *)data_i,
			    n_ord_arr);
		}
#endif /* NUM_SPECIAL_LAST >= 3 */
#if NUM_SPECIAL_LAST >= 4
		else if (n_left == 4) {
			FUNC(2LayersLastM2) ((void *)dat, (void *)data_i,
			    n_ord_arr);
		}
#endif /* NUM_SPECIAL_LAST >= 4 */
		return;
	}

	for (j = 0; j < dstep; j += 2) {
		mlib_d64 *dat0 = dat + j, *dat1 = dat0 + dstep,
		    *dat2 = dat1 + dstep, *dat3 = dat2 + dstep;
		DEF_CVAR4x(x);
		DEF_CVAR4(a);
		DEF_CVAR4(b);
		DEF_CVAR4(c);

/* load sources */
		C_LD(a0, dat0);
		C_LD(x1, dat1);
		C_LD(x2, dat2);
		C_LD(x3, dat3);

/* rotate sources */
		CMUL(a1, x1, r2);
		CMUL(a2, x2, r1);
		CMUL(a3, x3, r3);

/* first stage */
		CADD(b0, a0, a1);
		CSUB(b1, a0, a1);
		CADD(b2, a2, a3);
		RSUB(b3, a2, a3);

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
		for (i = bsize; i < len_arr; i += bsize) {
/* load sources */
			C_LD(a0, dat0 + i);
			C_LD(x1, dat1 + i);
			C_LD(x2, dat2 + i);
			C_LD(x3, dat3 + i);

/* rotate sources */
			CMUL(a1, x1, r2);
			CMUL(a2, x2, r1);
			CMUL(a3, x3, r3);

/* second stage */
			CADD(c0, b0, b2);
			CSUB(c2, b0, b2);
			CADD(c1, b1, b3);
			CSUB(c3, b1, b3);

/* first stage */
			CADD(b0, a0, a1);
			CSUB(b1, a0, a1);
			CADD(b2, a2, a3);
			RSUB(b3, a2, a3);

/* store */
			C_ST(c0, dat0 + i - bsize);
			C_ST(c1, dat1 + i - bsize);
			C_ST(c2, dat2 + i - bsize);
			C_ST(c3, dat3 + i - bsize);
		}

/* second stage */
		CADD(c0, b0, b2);
		CSUB(c2, b0, b2);
		CADD(c1, b1, b3);
		CSUB(c3, b1, b3);

/* store last */
		C_ST(c0, dat0 + len_arr - bsize);
		C_ST(c1, dat1 + len_arr - bsize);
		C_ST(c2, dat2 + len_arr - bsize);
		C_ST(c3, dat3 + len_arr - bsize);

/* rotate */
		CROT(r1, rot);
		CMUL(r2, r1, r1);
		CMUL(r3, r1, r2);
	}
}

void FUNC_i(
	2Layers) (
	mlib_s64 *dat,
	mlib_s64 *data_i,
	mlib_s32 n_ord_arr,
	mlib_s32 n_ord_lay)
{
	mlib_s64 dsign = ISIGN;
	mlib_s32 i, j;
	mlib_s32 dstep = 2 * (1 << n_ord_lay);
	mlib_s32 bsize = 2 * (1 << (n_ord_lay + 2));
	mlib_s32 len_arr = 2 * (1 << n_ord_arr);
	mlib_s32 n_left;

	INI_ROT_i(rot, n_ord_lay + 1);
	INI_CVAR_i(r1, ((((mlib_s64)1) << SCQ)-1), 0);
	INI_CVAR_i(r2, ((((mlib_s64)1) << SCQ)-1), 0);
	INI_CVAR_i(r3, ((((mlib_s64)1) << SCQ)-1), 0);

	n_left = n_ord_arr - n_ord_lay;

	if (n_left <= NUM_SPECIAL_LAST) {
#if NUM_SPECIAL_LAST >= 2

		if (n_left == 2) {
			FUNC_i(2LayersLast) ((void *)dat, (void *)data_i,
			    n_ord_arr);
		}
#endif /* NUM_SPECIAL_LAST >= 2 */
#if NUM_SPECIAL_LAST >= 3
		else if (n_left == 3) {
			FUNC_i(2LayersLastM1) ((void *)dat, (void *)data_i,
			    n_ord_arr);
		}
#endif /* NUM_SPECIAL_LAST >= 3 */
#if NUM_SPECIAL_LAST >= 4
		else if (n_left == 4) {
			FUNC_i(2LayersLastM2) ((void *)dat, (void *)data_i,
			    n_ord_arr);
		}
#endif /* NUM_SPECIAL_LAST >= 4 */
		return;
	}

	for (j = 0; j < dstep; j += 2) {
		mlib_s64 *dat0 = dat + j, *dat1 = dat0 + dstep,
		    *dat2 = dat1 + dstep, *dat3 = dat2 + dstep;
		DEF_CVAR4x_i(x);
		DEF_CVAR4_i(a);
		DEF_CVAR4_i(b);
		DEF_CVAR4_i(c);

/* load sources */
		C_LD_i(a0, dat0);
		C_LD_i(x1, dat1);
		C_LD_i(x2, dat2);
		C_LD_i(x3, dat3);

/* rotate sources */
		CMUL_i(a1, x1, r2);
		CMUL_i(a2, x2, r1);
		CMUL_i(a3, x3, r3);

/* first stage */
		CADD_i(b0, a0, a1);
		CSUB_i(b1, a0, a1);
		CADD_i(b2, a2, a3);
		RSUB_i(b3, a2, a3);

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
		for (i = bsize; i < len_arr; i += bsize) {
/* load sources */
			C_LD_i(a0, dat0 + i);
			C_LD_i(x1, dat1 + i);
			C_LD_i(x2, dat2 + i);
			C_LD_i(x3, dat3 + i);

/* rotate sources */
			CMUL_i(a1, x1, r2);
			CMUL_i(a2, x2, r1);
			CMUL_i(a3, x3, r3);

/* second stage */
			CADD_i(c0, b0, b2);
			CSUB_i(c2, b0, b2);
			CADD_i(c1, b1, b3);
			CSUB_i(c3, b1, b3);

/* first stage */
			CADD_i(b0, a0, a1);
			CSUB_i(b1, a0, a1);
			CADD_i(b2, a2, a3);
			RSUB_i(b3, a2, a3);

/* store */
			C_ST_i(c0, dat0 + i - bsize);
			C_ST_i(c1, dat1 + i - bsize);
			C_ST_i(c2, dat2 + i - bsize);
			C_ST_i(c3, dat3 + i - bsize);
		}

/* second stage */
		CADD_i(c0, b0, b2);
		CSUB_i(c2, b0, b2);
		CADD_i(c1, b1, b3);
		CSUB_i(c3, b1, b3);

/* store last */
		C_ST_i(c0, dat0 + len_arr - bsize);
		C_ST_i(c1, dat1 + len_arr - bsize);
		C_ST_i(c2, dat2 + len_arr - bsize);
		C_ST_i(c3, dat3 + len_arr - bsize);

/* rotate */
		CROT_i(r1, rot);
		CMUL_i(r2, r1, r1);
		CMUL_i(r3, r1, r2);
	}
}

/* *********************************************************** */

void FUNC(
	3LayersBlk) (
	mlib_d64 *dat,
	mlib_d64 *data_i,
	mlib_s32 n_ord_lay,
	mlib_s32 i0,
	mlib_s32 i1,
	mlib_d64 r1__r,
	mlib_d64 r1__i)
{
	mlib_d64 dsign = ISIGN;
	mlib_s32 n_ord_arr = n_ord_lay + 3;
	mlib_s32 i;
	mlib_s32 dstep = 2 * (1 << n_ord_lay);
	mlib_d64 *dat0 = dat, *dat1 = dat0 + dstep;
	mlib_d64 *dat2 = dat1 + dstep, *dat3 = dat2 + dstep;
	mlib_d64 *dat4 = dat3 + dstep, *dat5 = dat4 + dstep;
	mlib_d64 *dat6 = dat5 + dstep, *dat7 = dat6 + dstep;

	INI_ROT(rot, n_ord_lay + 2);
	{
		DEF_CVAR8x(x);
		DEF_CVAR8(a);
		DEF_CVAR8(b);
		DEF_CVAR8(c);
		DEF_CVAR8(d);
		DEF_CVAR(z5);
		DEF_CVAR(z7);
		INI_CVAR(r1, 1, 0);
		INI_CVAR(r2, 1, 0);
		INI_CVAR(r3, 1, 0);
		INI_CVAR(r4, 1, 0);
		INI_CVAR(r5, 1, 0);
		INI_CVAR(r6, 1, 0);
		INI_CVAR(r7, 1, 0);

		r1_r = r1__r;
		r1_i = r1__i;

		CMUL(r2, r1, r1);
		CMUL(r3, r1, r2);
		CMUL(r4, r2, r2);
		CMUL(r5, r2, r3);
		CMUL(r6, r3, r3);
		CMUL(r7, r3, r4);
#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
		for (i = 2*i0; i < 2*i1; i += 2) {
			C_LD(a0, dat0 + i);
			C_LD(x1, dat1 + i);
			C_LD(x2, dat2 + i);
			C_LD(x3, dat3 + i);
			C_LD(x4, dat4 + i);
			C_LD(x5, dat5 + i);
			C_LD(x6, dat6 + i);
			C_LD(x7, dat7 + i);

/* first stage */
			CMUL(a1, x1, r4);
			CBUT(a, b, 0, 1);

			CMUL(a2, x2, r2);
			CMUL(a3, x3, r6);
			RBUT(a, b, 2, 3);

			CMUL(a4, x4, r1);
			CMUL(a5, x5, r5);
			CBUT(a, b, 4, 5);

			CMUL(a6, x6, r3);
			CMUL(a7, x7, r7);
			RBUT(a, b, 6, 7);

/* second stage */
			CBUT(b, c, 0, 2);
			CBUT(b, c, 1, 3);
			RBUT(b, c, 4, 6);
			CBUT(b, z, 5, 7);

			CROT_P1(c5, z5);
			CROT_P3(c7, z7);

/* third stage */
			CBUT(c, d, 0, 4);
			CBUT(c, d, 1, 5);
			CBUT(c, d, 2, 6);
			CBUT(c, d, 3, 7);

/* rotate */
			CROT(r1, rot);
			CMUL(r2, r1, r1);
			CMUL(r3, r1, r2);
			CMUL(r4, r2, r2);
			CMUL(r5, r2, r3);
			CMUL(r6, r3, r3);
			CMUL(r7, r3, r4);

/* store */
			C_ST(d0, dat0 + i);
			C_ST(d1, dat1 + i);
			C_ST(d2, dat2 + i);
			C_ST(d3, dat3 + i);
			C_ST(d4, dat4 + i);
			C_ST(d5, dat5 + i);
			C_ST(d6, dat6 + i);
			C_ST(d7, dat7 + i);
		}
	}
}

void FUNC_i(
	3LayersBlk) (
	mlib_s64 *dat,
	mlib_s64 *data_i,
	mlib_s32 n_ord_lay,
	mlib_s32 i0,
	mlib_s32 i1,
	mlib_s64 r1__r,
	mlib_s64 r1__i)
{
	mlib_s64 dsign = ISIGN;
	mlib_s32 n_ord_arr = n_ord_lay + 3;
	mlib_s32 i;
	mlib_s32 dstep = 2 * (1 << n_ord_lay);
	mlib_s64 *dat0 = dat, *dat1 = dat0 + dstep;
	mlib_s64 *dat2 = dat1 + dstep, *dat3 = dat2 + dstep;
	mlib_s64 *dat4 = dat3 + dstep, *dat5 = dat4 + dstep;
	mlib_s64 *dat6 = dat5 + dstep, *dat7 = dat6 + dstep;

	INI_ROT_i(rot, n_ord_lay + 2);
	{
		DEF_CVAR8x_i(x);
		DEF_CVAR8_i(a);
		DEF_CVAR8_i(b);
		DEF_CVAR8_i(c);
		DEF_CVAR8_i(d);
		DEF_CVAR_i(z5);
		DEF_CVAR_i(z7);
		INI_CVAR_i(r1, ((((mlib_s64)1) << SCQ)-1), 0);
		INI_CVAR_i(r2, ((((mlib_s64)1) << SCQ)-1), 0);
		INI_CVAR_i(r3, ((((mlib_s64)1) << SCQ)-1), 0);
		INI_CVAR_i(r4, ((((mlib_s64)1) << SCQ)-1), 0);
		INI_CVAR_i(r5, ((((mlib_s64)1) << SCQ)-1), 0);
		INI_CVAR_i(r6, ((((mlib_s64)1) << SCQ)-1), 0);
		INI_CVAR_i(r7, ((((mlib_s64)1) << SCQ)-1), 0);

		r1_r = r1__r;
		r1_i = r1__i;

		CMUL_i(r2, r1, r1);
		CMUL_i(r3, r1, r2);
		CMUL_i(r4, r2, r2);
		CMUL_i(r5, r2, r3);
		CMUL_i(r6, r3, r3);
		CMUL_i(r7, r3, r4);
#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
		for (i = 2*i0; i < 2*i1; i += 2) {
			C_LD_i(a0, dat0 + i);
			C_LD_i(x1, dat1 + i);
			C_LD_i(x2, dat2 + i);
			C_LD_i(x3, dat3 + i);
			C_LD_i(x4, dat4 + i);
			C_LD_i(x5, dat5 + i);
			C_LD_i(x6, dat6 + i);
			C_LD_i(x7, dat7 + i);

/* first stage */
			CMUL_i(a1, x1, r4);
			CBUT_i(a, b, 0, 1);

			CMUL_i(a2, x2, r2);
			CMUL_i(a3, x3, r6);
			RBUT_i(a, b, 2, 3);

			CMUL_i(a4, x4, r1);
			CMUL_i(a5, x5, r5);
			CBUT_i(a, b, 4, 5);

			CMUL_i(a6, x6, r3);
			CMUL_i(a7, x7, r7);
			RBUT_i(a, b, 6, 7);

/* second stage */
			CBUT_i(b, c, 0, 2);
			CBUT_i(b, c, 1, 3);
			RBUT_i(b, c, 4, 6);
			CBUT_i(b, z, 5, 7);

			CROT_P1_i(c5, z5);
			CROT_P3_i(c7, z7);

/* third stage */
			CBUT_i(c, d, 0, 4);
			CBUT_i(c, d, 1, 5);
			CBUT_i(c, d, 2, 6);
			CBUT_i(c, d, 3, 7);

/* rotate */
			CROT_i(r1, rot);
			CMUL_i(r2, r1, r1);
			CMUL_i(r3, r1, r2);
			CMUL_i(r4, r2, r2);
			CMUL_i(r5, r2, r3);
			CMUL_i(r6, r3, r3);
			CMUL_i(r7, r3, r4);

/* store */
			C_ST_i(d0, dat0 + i);
			C_ST_i(d1, dat1 + i);
			C_ST_i(d2, dat2 + i);
			C_ST_i(d3, dat3 + i);
			C_ST_i(d4, dat4 + i);
			C_ST_i(d5, dat5 + i);
			C_ST_i(d6, dat6 + i);
			C_ST_i(d7, dat7 + i);
		}
	}
}

/* *********************************************************** */

/*
 * Functions (*_RI) with separate real/image arrays
 *
 */

void FUNC(
	order4_RI) (
	deal_t *CONST pal64data_r,
	deal_t *CONST pal64data_i,
	mlib_s32 CONST order)
	{
	deal_t *CONST dat_r = (deal_t *)pal64data_r;
	deal_t *CONST dat_i = (deal_t *)pal64data_i;

	DEAL_ALL_ORD4_RI(isign, dat, SSIGN);
}

/* *********************************************************** */

void FUNC(
	first2_RI) (
	deal_t *pal64data_r,
	deal_t *pal64data_i,
	mlib_s32 n_ord_arr)
{
	mlib_s32 CONST nnn = 1 << n_ord_arr;
	mlib_s32 iii;

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
	FIRST_2_LAYERS(n_ord_arr, SSIGN, pal64data, RI);
}

/* *********************************************************** */

void FUNC(
	first3_RI) (
	deal_t *pal64data_r,
	deal_t *pal64data_i,
	mlib_s32 n_ord_arr)
{
	mlib_s32 CONST nnn = 1 << n_ord_arr;
	mlib_s32 iii;

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
	FIRST_3_LAYERS(n_ord_arr, SSIGN, pal64data, RI);
}

/* *********************************************************** */

#if NUM_SPECIAL_LAST >= 2

void FUNC(
	2LayersLast_RI) (
	deal_t *CONST dat_r,
	deal_t *CONST dat_i,
	mlib_s32 CONST n_ord_arr)
{
	mlib_s32 isign = ISIGN;
	CONST mlib_s32 n_ord_lay = n_ord_arr - 2;
	CONST mlib_s32 n_len_arr = 1 << n_ord_arr;
	CONST mlib_s32 n_step_half = n_len_arr >> 3;

	DCL_CALC_STEPS1234(n_step, n_ord_lay);
	DCL_CALC_INCR(incr_half, n_ord_lay + 1, 0);
	DCL_CALC_INCR(incr, n_ord_lay, 1);
	DCL_INIT_ROT(rot, incr);
	DCL_INIT_ROT(rot_half, incr_half);
	precise_t wtmp;
	mlib_s32 iii;

	deal_t *dat_mmm_r = dat_r, *dat_mmm_i = dat_i;

	DEAL_4_SIMPLE_RI(dat_mmm, n_step, SSIGN);
	{
		deal_t *dat_mmm_r = dat_r + n_step_half, *dat_mmm_i =
		    dat_i + n_step_half;

		DEAL_4_HALFWAY_RI(dat_mmm, n_step, SSIGN);
	}

	for (iii = 1; iii < n_step_half; iii++) {
		DCL_MULT_ROT_ROT_HALF(rot, rot_half, r_rh);
		{
			deal_t *dat_mmm_r = dat_r + iii, *dat_mmm_i =
			    dat_i + iii;
			deal_t *dat_mirr_r = dat_r + n_step - iii, *dat_mirr_i =
			    dat_i + n_step - iii;
			DEAL_8_USUAL_MIRR_RI(dat_mmm, dat_mirr, n_step, rot,
			    rot_half, r_rh, SSIGN);
		}

		ROTATE_CMPLX_TMP(rot, incr, wtmp);
		ROTATE_CMPLX_TMP(rot_half, incr_half, wtmp);
	}
}

#endif /* NUM_SPECIAL_LAST >= 2 */

/* *********************************************************** */

#if NUM_SPECIAL_LAST >= 2

void FUNC(
	2LayersLastM1_RI) (
	deal_t *CONST dat_r,
	deal_t *CONST dat_i,
	mlib_s32 CONST n_ord_arr)
{
	mlib_s32 isign = ISIGN;
	CONST mlib_s32 n_ord_lay = n_ord_arr - 3;

	DCL_CALC_STEPS1234(n_step, n_ord_lay);
	DCL_CALC_INCR(incr_half, n_ord_lay + 1, 0);
	DCL_CALC_INCR(incr, n_ord_lay, 1);
	DCL_INIT_ROT(rot, incr);
	DCL_INIT_ROT(rot_half, incr_half);
	precise_t wtmp;
	mlib_s32 iii;

	DEAL_8_SIMPLE_SEQUENT_RI(dat, n_step, SSIGN);
	for (iii = 1; iii < n_step; iii++) {
		DCL_MULT_ROT_ROT_HALF(rot, rot_half, r_rh);
		deal_t *dat_iii_r = dat_r + iii, *dat_iii_i = dat_i + iii;

		DEAL_8_NORMAL_SEQUENT_RI(dat_iii, n_step, rot, rot_half, r_rh,
		    SSIGN);
		ROTATE_CMPLX_TMP(rot, incr, wtmp);
		ROTATE_CMPLX_TMP(rot_half, incr_half, wtmp);
	}
}

#endif /* NUM_SPECIAL_LAST >= 2 */

/* *********************************************************** */

#if NUM_SPECIAL_LAST >= 2

void FUNC(
	2LayersLastM2_RI) (
	deal_t *CONST dat_r,
	deal_t *CONST dat_i,
	mlib_s32 CONST n_ord_arr)
{
	mlib_s32 isign = ISIGN;
	CONST mlib_s32 n_ord_lay = n_ord_arr - 4;

	DCL_CALC_STEPS1234(n_step, n_ord_lay);
	DCL_CALC_INCR(incr_half, n_ord_lay + 1, 0);
	DCL_CALC_INCR(incr, n_ord_lay, 1);
	DCL_INIT_ROT(rot, incr);
	DCL_INIT_ROT(rot_half, incr_half);
	CONST mlib_s32 two_four_n_step = n_step * 8;
	CONST mlib_s32 three_four_n_step = n_step * 12;
	precise_t wtmp;
	mlib_s32 iii;

	DEAL_8_8_SIMPLE_SEQUENT_RI(dat, n_step, SSIGN);
	for (iii = 1; iii < n_step; iii++) {
		DCL_MULT_ROT_ROT_HALF(rot, rot_half, r_rh);
		deal_t *dat_iii_r = dat_r + iii, *dat_iii_i = dat_i + iii;

		DEAL_8_8_NORMAL_SEQUENT_RI(dat_iii, n_step, rot, rot_half, r_rh,
		    SSIGN);
		ROTATE_CMPLX_TMP(rot, incr, wtmp);
		ROTATE_CMPLX_TMP(rot_half, incr_half, wtmp);
	}
}

#endif /* NUM_SPECIAL_LAST >= 2 */

/* *********************************************************** */

void FUNC(
	2Layers_RI) (
	mlib_d64 *dat_r,
	mlib_d64 *dat_i,
	mlib_s32 n_ord_arr,
	mlib_s32 n_ord_lay)
{
	mlib_d64 dsign = ISIGN;
	mlib_s32 i, j;
	mlib_s32 dstep = (1 << n_ord_lay);
	mlib_s32 bsize = (1 << (n_ord_lay + 2));
	mlib_s32 len_arr = (1 << n_ord_arr);
	mlib_s32 n_left;

	INI_ROT(rot, n_ord_lay + 1);
	INI_CVAR(r1, 1, 0);
	INI_CVAR(r2, 1, 0);
	INI_CVAR(r3, 1, 0);

	n_left = n_ord_arr - n_ord_lay;

	if (n_left <= NUM_SPECIAL_LAST) {
#if NUM_SPECIAL_LAST >= 2

		if (n_left == 2) {
			FUNC(2LayersLast_RI) ((void *)dat_r, (void *)dat_i,
			    n_ord_arr);
		}
#endif /* NUM_SPECIAL_LAST >= 2 */
#if NUM_SPECIAL_LAST >= 3
		else if (n_left == 3) {
			FUNC(2LayersLastM1_RI) ((void *)dat_r, (void *)dat_i,
			    n_ord_arr);
		}
#endif /* NUM_SPECIAL_LAST >= 3 */
#if NUM_SPECIAL_LAST >= 4
		else if (n_left == 4) {
			FUNC(2LayersLastM2_RI) ((void *)dat_r, (void *)dat_i,
			    n_ord_arr);
		}
#endif /* NUM_SPECIAL_LAST >= 4 */
		return;
	}

	for (j = 0; j < dstep; j++) {
		mlib_d64 *dat0_r = dat_r + j, *dat1_r = dat0_r + dstep;
		mlib_d64 *dat2_r = dat1_r + dstep, *dat3_r = dat2_r + dstep;
		mlib_d64 *dat0_i = dat_i + j, *dat1_i = dat0_i + dstep;
		mlib_d64 *dat2_i = dat1_i + dstep, *dat3_i = dat2_i + dstep;

		DEF_CVAR4x(x);
		DEF_CVAR4(a);
		DEF_CVAR4(b);
		DEF_CVAR4(c);

/* load sources */
		C_LD_RI(a0, dat0, 0);
		C_LD_RI(x1, dat1, 0);
		C_LD_RI(x2, dat2, 0);
		C_LD_RI(x3, dat3, 0);

/* rotate sources */
		CMUL(a1, x1, r2);
		CMUL(a2, x2, r1);
		CMUL(a3, x3, r3);

/* first stage */
		CADD(b0, a0, a1);
		CSUB(b1, a0, a1);
		CADD(b2, a2, a3);
		RSUB(b3, a2, a3);

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
		for (i = bsize; i < len_arr; i += bsize) {
/* load sources */
			C_LD_RI(a0, dat0, i);
			C_LD_RI(x1, dat1, i);
			C_LD_RI(x2, dat2, i);
			C_LD_RI(x3, dat3, i);

/* rotate sources */
			CMUL(a1, x1, r2);
			CMUL(a2, x2, r1);
			CMUL(a3, x3, r3);

/* second stage */
			CADD(c0, b0, b2);
			CSUB(c2, b0, b2);
			CADD(c1, b1, b3);
			CSUB(c3, b1, b3);

/* first stage */
			CADD(b0, a0, a1);
			CSUB(b1, a0, a1);
			CADD(b2, a2, a3);
			RSUB(b3, a2, a3);

/* store */
			C_ST_RI(c0, dat0, i - bsize);
			C_ST_RI(c1, dat1, i - bsize);
			C_ST_RI(c2, dat2, i - bsize);
			C_ST_RI(c3, dat3, i - bsize);
		}

/* second stage */
		CADD(c0, b0, b2);
		CSUB(c2, b0, b2);
		CADD(c1, b1, b3);
		CSUB(c3, b1, b3);

/* store last */
		C_ST_RI(c0, dat0, len_arr - bsize);
		C_ST_RI(c1, dat1, len_arr - bsize);
		C_ST_RI(c2, dat2, len_arr - bsize);
		C_ST_RI(c3, dat3, len_arr - bsize);

/* rotate */
		CROT(r1, rot);
		CMUL(r2, r1, r1);
		CMUL(r3, r1, r2);
	}
}

/* *********************************************************** */

void FUNC(
	3LayersBlk_RI) (
	mlib_d64 *dat_r,
	mlib_d64 *dat_i,
	mlib_s32 n_ord_lay,
	mlib_s32 i0,
	mlib_s32 i1,
	mlib_d64 r1__r,
	mlib_d64 r1__i)
{
	mlib_d64 dsign = ISIGN;
	mlib_s32 n_ord_arr = n_ord_lay + 3;
	mlib_s32 i;
	mlib_s32 dstep = (1 << n_ord_lay);
	mlib_d64 *dat0_r = dat_r, *dat1_r = dat0_r + dstep;
	mlib_d64 *dat2_r = dat1_r + dstep, *dat3_r = dat2_r + dstep;
	mlib_d64 *dat4_r = dat3_r + dstep, *dat5_r = dat4_r + dstep;
	mlib_d64 *dat6_r = dat5_r + dstep, *dat7_r = dat6_r + dstep;
	mlib_d64 *dat0_i = dat_i, *dat1_i = dat0_i + dstep;
	mlib_d64 *dat2_i = dat1_i + dstep, *dat3_i = dat2_i + dstep;
	mlib_d64 *dat4_i = dat3_i + dstep, *dat5_i = dat4_i + dstep;
	mlib_d64 *dat6_i = dat5_i + dstep, *dat7_i = dat6_i + dstep;

	INI_ROT(rot, n_ord_lay + 2);
	{
		DEF_CVAR8x(x);
		DEF_CVAR8(a);
		DEF_CVAR8(b);
		DEF_CVAR8(c);
		DEF_CVAR8(d);
		DEF_CVAR(z5);
		DEF_CVAR(z7);
		INI_CVAR(r1, 1, 0);
		INI_CVAR(r2, 1, 0);
		INI_CVAR(r3, 1, 0);
		INI_CVAR(r4, 1, 0);
		INI_CVAR(r5, 1, 0);
		INI_CVAR(r6, 1, 0);
		INI_CVAR(r7, 1, 0);

		r1_r = r1__r;
		r1_i = r1__i;

		CMUL(r2, r1, r1);
		CMUL(r3, r1, r2);
		CMUL(r4, r2, r2);
		CMUL(r5, r2, r3);
		CMUL(r6, r3, r3);
		CMUL(r7, r3, r4);

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
		for (i = i0; i < i1; i++) {
			C_LD_RI(a0, dat0, i);
			C_LD_RI(x1, dat1, i);
			C_LD_RI(x2, dat2, i);
			C_LD_RI(x3, dat3, i);
			C_LD_RI(x4, dat4, i);
			C_LD_RI(x5, dat5, i);
			C_LD_RI(x6, dat6, i);
			C_LD_RI(x7, dat7, i);

/* first stage */
			CMUL(a1, x1, r4);
			CBUT(a, b, 0, 1);

			CMUL(a2, x2, r2);
			CMUL(a3, x3, r6);
			RBUT(a, b, 2, 3);

			CMUL(a4, x4, r1);
			CMUL(a5, x5, r5);
			CBUT(a, b, 4, 5);

			CMUL(a6, x6, r3);
			CMUL(a7, x7, r7);
			RBUT(a, b, 6, 7);

/* second stage */
			CBUT(b, c, 0, 2);
			CBUT(b, c, 1, 3);
			RBUT(b, c, 4, 6);
			CBUT(b, z, 5, 7);

			CROT_P1(c5, z5);
			CROT_P3(c7, z7);

/* third stage */
			CBUT(c, d, 0, 4);
			CBUT(c, d, 1, 5);
			CBUT(c, d, 2, 6);
			CBUT(c, d, 3, 7);

/* rotate */
			CROT(r1, rot);
			CMUL(r2, r1, r1);
			CMUL(r3, r1, r2);
			CMUL(r4, r2, r2);
			CMUL(r5, r2, r3);
			CMUL(r6, r3, r3);
			CMUL(r7, r3, r4);

/* store */
			C_ST_RI(d0, dat0, i);
			C_ST_RI(d1, dat1, i);
			C_ST_RI(d2, dat2, i);
			C_ST_RI(d3, dat3, i);
			C_ST_RI(d4, dat4, i);
			C_ST_RI(d5, dat5, i);
			C_ST_RI(d6, dat6, i);
			C_ST_RI(d7, dat7, i);
		}
	}
}

/* *********************************************************** */

void FUNC(
	3Layers) (
	mlib_d64 *data,
	mlib_d64 *data_i,
	mlib_s32 n_ord_arr,
	mlib_s32 n_ord_lay)
{
	mlib_d64 dsign = ISIGN;
	INI_ROT(r, 3);

	mlib_s32 length_lay = 1 << (n_ord_lay + 3);
	mlib_s32 length_arr = 1 << n_ord_arr, i;
	mlib_s32 step = 1 << (n_ord_lay - 1);

	for (i = 0; i < length_arr; i += length_lay) {
		FUNC(3LayersBlk)(data + 2 * i, NULL,
			n_ord_lay, 0, 2*step, 1, 0);
	}
}

void FUNC_i(
	3Layers) (
	mlib_s64 *data,
	mlib_s64 *data_i,
	mlib_s32 n_ord_arr,
	mlib_s32 n_ord_lay)
{
	mlib_s64 dsign = ISIGN;
	INI_ROT_i(r, 3);

	mlib_s32 length_lay = 1 << (n_ord_lay + 3);
	mlib_s32 length_arr = 1 << n_ord_arr, i;
	mlib_s32 step = 1 << (n_ord_lay - 1);

	for (i = 0; i < length_arr; i += length_lay) {
		FUNC_i(3LayersBlk)(data + 2 * i, NULL,
			n_ord_lay, 0, 2*step, ((((mlib_s64)1) << SCQ)-1), 0);
	}
}

/* *********************************************************** */

void FUNC(
	3Layers_RI) (
	mlib_d64 *data_r,
	mlib_d64 *data_i,
	mlib_s32 n_ord_arr,
	mlib_s32 n_ord_lay)
{
	mlib_s32 length_lay = 1 << (n_ord_lay + 3), length_arr =
	    1 << n_ord_arr, i;
	mlib_s32 step = 1 << n_ord_lay;

	for (i = 0; i < length_arr; i += length_lay) {
		FUNC(3LayersBlk_RI) (data_r + i, data_i + i, n_ord_lay,
		0, step, 1, 0);
	}
}

/* *********************************************************** */

fft_param
FUNC(func)[] =
{
	    ISIGN,
	    0,
	    FUNC(order4),
	    FUNC(first2),
	    FUNC(first3),
	    FUNC(2Layers),
	    FUNC(3Layers),
};

fft_param_i
FUNC_i(func)[] =
{
	    ISIGN,
	    0,
	    FUNC_i(order4),
	    FUNC_i(first2),
	    FUNC_i(first3),
	    FUNC_i(2Layers),
	    FUNC_i(3Layers),
};

fft_param
FUNC(func_RI)[] =
{
	    ISIGN,
	    1,
	    FUNC(order4_RI),
	    FUNC(first2_RI),
	    FUNC(first3_RI),
	    FUNC(2Layers_RI),
	    FUNC(3Layers_RI),
};

/* ifdef _NO_LONGLONG, use stdc source codes, else use int source codes */
#endif

/* *********************************************************** */
