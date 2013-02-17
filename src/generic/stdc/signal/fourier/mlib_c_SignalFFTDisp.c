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

#pragma ident	"@(#)mlib_c_SignalFFTDisp.c	9.3	07/10/09 SMI"

/* *********************************************************** */

/* DISPATCHER */

/* *********************************************************** */

/*
 * FUNCTIONS
 *  NOTE: FFT below means F(ast)F(ourier)T(ransform)
 *      mlib_c_fftDispatcherW[_1|_2|_3|_4|_Common]
 *                            - dispatches various FFT functions: creates
 *                              ( dynamically ) main working buffer,
 *                              fills it by source data, calls kernel FFT
 *                              function ( direct or inverse ) and stores
 *                              results in destination array(s)
 *      mlib_c_fftDW_01       - static function providing 'simpliest'
 *                              orders ( 0, 1 ) dispatching
 *      mlib_c_fftDW_GetData  - static function performing filling of
 *                              the working buffer by source data
 *      mlib_c_fftDW_PutData  - static function performing filling of
 *                              destination array by the data from
 *                              working buffer
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_c_fftDispatcherW(mlib_s16 *CONST dst_main,
 *                                        mlib_s16 *CONST dst_aux,
 *                                        mlib_s16 *CONST src_main,
 *                                        mlib_s16 *CONST src_aux,
 *                                        mlib_s16 *CONST window,
 *                                        mlib_s32 CONST  order,
 *                                        mlib_s32 *CONST scale,
 *                                        mlib_s32 CONST  data_info_flags);
 *
 *      static mlib_status mlib_c_fftDW_01(mlib_s16 *CONST dst_main,
 *                                         mlib_s16 *CONST dst_aux,
 *                                         mlib_s16 *CONST src_main,
 *                                         mlib_s16 *CONST src_aux,
 *                                         mlib_s16 *CONST window,
 *                                         mlib_s32 CONST  order,
 *                                         mlib_s32 CONST  shiftscale,
 *                                         mlib_s32 CONST  data_info_flags);
 *
 *      static void mlib_c_fftDW_GetData[_4](deal_t *CONST   pal64data,
 *                                           mlib_s16 *CONST src_main,
 *                                           mlib_s16 *CONST src_aux,
 *                                           mlib_s16 *CONST window,
 *                                           mlib_s32 CONST  order,
 *                                           mlib_s32 CONST  scale,
 *                                           mlib_s32 CONST  data_info_flag);
 *
 *      static void mlib_c_fftDW_PutData[_4](deal_t *CONST   pal64data,
 *                                           mlib_s16 *CONST dst_main,
 *                                           mlib_s16 *CONST dst_aux,
 *                                           mlib_s16 *CONST window,
 *                                           mlib_s32 CONST  order,
 *                                           mlib_s32 CONST  scale,
 *                                           mlib_s32 CONST  data_info_flags,
 *                                           mlib_s32 CONST  bool_is_windowed);
 *
 *
 * ARGUMENTS
 *      Note:      dst_main, dst_aux, src_main, src_aux arguments are
 *                   interpreted depending on data_info_flags argument
 *
 *      dst_main - if( data_info_flags & eDstCmplx ) {
 *                   Complex destination signal array. dst_main[ 2 * i ]
 *                   hold the real parts, and dst_main[ 2 * i + 1 ] hold the
 *                   imaginary parts
 *                 }
 *                 else {
 *                   Destination signal array that hold real parts
 *                 }
 *
 *      dst_aux  - if( data_info_flags & eDstReIm ) {
 *                   Destination signal array that hold imaginary
 *                 }
 *                 else {
 *                   omitted ( must be NULL )
 *                 }
 *
 *      src_main - if( data_info_flags & eSrcCmplx ) {
 *                   Complex source signal array. src_main[ 2 * i ]
 *                   hold the real parts, and src_main[ 2 * i + 1 ] hold the
 *                   imaginary parts
 *                 }
 *                 else {
 *                   Source signal array that hold real parts
 *                 }
 *
 *      src_aux  - if( data_info_flags & eSrcReIm ) {
 *                   Source signal array that hold imaginary
 *                 }
 *                 else {
 *                   omitted ( must be NULL )
 *                 }
 *
 *      window   - Window coefficient array. The window coefficients are in
 *                 Q15 format. Note the data type of the window coefficient
 *                 array is S16 for both S16 and S16C input signal
 *                 Note: it is NULL for 'not-windowed' FFT's
 *
 *      order    - Order of the transformation. The base-2 logarithm of the
 *                 number of data samples
 *
 *      data_info_flags
 *               - 32bit integer composed of three sets of bit flags, holding:
 *                 1)eDstCmplx excluded_or eDstReIm excluded_or eDstRe
 *                   determining destination processing
 *                 2)eSrcCmplx excluded_or eSrcReIm excluded_or eSrcRe
 *                   determining source processing
 *                 3)eInversFFT determining whether to perform direct or
 *                   inverse FFT processing
 *
 *      bool_is_windowed
 *               - 32bit integer passed to mlib_c_fftDW_PutData
 *                  to smart saturation depending on windowing presence
 *                  set to 0, if no windowing was performed, to 1 else
 *
 *      pal64data
 *               - 64bit-aligned complex array. Created dynamically inside
 *                 mlib_c_fftDispatcherW ( from stack or __mlib_malloc ).
 *                 Destination for mlib_c_fftDW_GetData.
 *                 Source for mlib_c_fftDW_PutData.
 *                 Source and destination for kernel FFT function.
 *                 pal64data[ 2 * i ] hold the real parts, and
 *                 pal64data[ 2 * i + 1 ] hold the imaginary parts
 *
 *      shiftscale
 *               - 32bit integer passed to mlib_c_fftDispatcherW_Common
 *                  to serve 1, 2, 3, 4 FFT versions
 *
 * DESCRIPTION
 *
 *      dest = PutData( ( buffer =
 *                        kernelFFT( buffer = GetData( src[, window] ) ) ) )
 *
 * IMPLEMENTATION
 *
 *      We don't process all the cases 'as they are'. Instead, we have one
 *      'dispatcher' function and call it from all the cases, passing it
 *      all necessary array pointers together with ( bit-flags composed )
 *      additional parameter describing each case specifics and pointers
 *      interpretation.
 *      This way we avoid unnecessary code duplication and increase reliability
 *      ( indirectly, due to processing features incapsulation ).
 *      'Dispatcher' function creates 'unified-and-aligned' working buffer,
 *      fills it using source data ( and possibly window ), calls 'kernel'
 *      FFT function, passing this buffer as source and destination array,
 *      and then fills destination array(s) by data got from buffer
 */

#include <mlib_signal.h>
#include <mlib_SysMath.h>	/* for fabs */

#define	FFT_UTIL_C

#include <mlib_SignalFFTUtil.h>

/* *********************************************************** */

/* function prototypes */

static mlib_status mlib_c_fftDispatcherW(
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 *CONST scale,
    mlib_s32 CONST data_info_flags);

static mlib_status mlib_c_fftDW_01(
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST shiftscale,
    mlib_s32 CONST data_info_flags);

static void mlib_c_fftDW_GetData(
    deal_t *CONST pal64data,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST scale,
    mlib_s32 CONST data_info_flags);

static void mlib_c_fftDW_GetData_4(
    deal_t *CONST pal64data,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST data_info_flags);

static void mlib_c_fftDW_PutData(
    deal_t *CONST pal64data,
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s32 CONST order,
    mlib_s32 CONST scale,
    mlib_s32 CONST data_info_flags,
    mlib_s32 CONST bool_is_windowed);

static void mlib_c_fftDW_PutData_4(
    deal_t *CONST pal64data,
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s32 CONST order,
    mlib_s32 CONST scale,
    mlib_s32 CONST data_info_flags);

/* *********************************************************** */

/* macros, typedefs */

/* general error processing */

/* *********************************************************** */

#define	SERVE_BAD_ORDER(order)                                  \
	{                                                       \
	    if ((order) < 0 || (order) > 31)                    \
		return (MLIB_FAILURE);                          \
	}

/* *********************************************************** */

#define	SERVE_BAD_POINTER(ptr)                                  \
	{                                                       \
	    if (!(ptr))                                         \
		return (MLIB_FAILURE);                          \
	}

/* *********************************************************** */

/* marks error ptr situations in dispatcher */
#define	ERR_PTR_NULL	NULL

/* *********************************************************** */

/* scale to transform ( -7fff, 7fff ) -> ( -.999..., .999... ) */
#define	Q15SCALE	(- MLIB_S16_MIN)

#define	SAT_MULT(n)	(1. / (n))

/* *********************************************************** */

/* patched scale to avoid multiplying in saturation/inverseFFT */
#define	WIN_MULT(n)	(SAT_MULT(n) / ((mlib_d64)(Q15SCALE)))

/* *********************************************************** */

/* due to WIN_MULT we don't need any other mults in *W case */
#define	SAT_D64_TO_S16W(val, mult)	(val)

/* *********************************************************** */

/* fast saturation macro */
#define	SAT_D64_TO_S16(val, mult)	SAT_D64_TO_S16W((val) * (mult), 1997)

/* *********************************************************** */

/* windowing macro for 0, 1 orders */
#define	MULT_WIN_01(src, wnd)                                   \
	(((mlib_s32)(src) * (mlib_s32)(wnd)) >> 15)

/* *********************************************************** */

/* windowing macro */
#define	MULT_WIN(src, wnd)	(((deal_t)(src)) * ((deal_t)(wnd)))

/* *********************************************************** */

/* value to mark scale usage */
#define	SCALE_SIGNIFICANT	33

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
	    mlib_s32 CONST nnn = 1 << order, o_3 = order - 3, shift =    \
		leng - o_3;                                              \
	    mlib_s32 iii, hgh;                                           \
	    mlib_s32 rev_1 = 4 << o_3, rev_2 = 2 << o_3, rev_3 =         \
		6 << o_3, rev_4 = 1 << o_3, rev_5 = 5 << o_3, rev_6 =    \
		3 << o_3, rev_7 = 7 << o_3;                              \
	    for (iii = hgh = 0; iii < nnn; hgh++, iii += 8) {            \
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
	    mlib_s32 CONST nnn = 1 << order, o_2 = order - 2, shift =    \
		leng - o_2;                                              \
	    mlib_s32 iii, hgh;                                           \
	    mlib_s32 rev_1 = 2 << o_2, rev_2 = 1 << o_2, rev_3 =         \
		3 << o_2;                                                \
	    for (iii = hgh = 0; iii < nnn; hgh++, iii += 4) {            \
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

#define	FIND_SCALEF(result, all_ors)                            \
	{                                                       \
	    mlib_s32 cntr = 30;                                 \
	                                                        \
	    while (cntr)                                        \
		if (all_ors & (1 << cntr))                      \
		    break;                                      \
		else                                            \
		    cntr--;                                     \
	    result = 14 - cntr;                                 \
	}

/* *********************************************************** */

#define	GET_FUNC(REV_NBITS)                                                \
	static void                                                        \
	mlib_c_fftDW_GetData##REV_NBITS##_4(deal_t *CONST dst,             \
	    mlib_s16 *CONST src_main, mlib_s16 *CONST src_aux,             \
	    mlib_s16 *CONST wnd, mlib_s32 CONST order,                     \
	    mlib_s32 CONST data_info_flags)                                \
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
	static void                                                        \
	mlib_c_fftDW_GetData##REV_NBITS(deal_t *CONST dst,                 \
	    mlib_s16 *CONST src_main, mlib_s16 *CONST src_aux,             \
	    mlib_s16 *CONST wnd, mlib_s32 CONST order,                     \
	    mlib_s32 CONST scale, mlib_s32 CONST data_info_flags)          \
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

    static void mlib_c_fftDW_GetData_4(
    deal_t *CONST dst,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST wnd,
    mlib_s32 CONST order,
    mlib_s32 CONST data_info_flags)
{
	mlib_s32 ord = order - 2;

	if (ord < 8) {
		mlib_c_fftDW_GetData8_4(dst, src_main, src_aux, wnd, order,
		    data_info_flags);
	} else if (ord < 16) {
		mlib_c_fftDW_GetData16_4(dst, src_main, src_aux, wnd, order,
		    data_info_flags);
	} else if (ord < 24) {
		mlib_c_fftDW_GetData24_4(dst, src_main, src_aux, wnd, order,
		    data_info_flags);
	} else {
		mlib_c_fftDW_GetData32_4(dst, src_main, src_aux, wnd, order,
		    data_info_flags);
	}
}

static void
mlib_c_fftDW_GetData(
    deal_t *CONST dst,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST wnd,
    mlib_s32 CONST order,
    mlib_s32 CONST scale,
    mlib_s32 CONST data_info_flags)
{
	mlib_s32 ord = order - 2;

	if (ord < 8) {
		mlib_c_fftDW_GetData8(dst, src_main, src_aux, wnd, order, scale,
		    data_info_flags);
	} else if (ord < 16) {
		mlib_c_fftDW_GetData16(dst, src_main, src_aux, wnd, order,
		    scale, data_info_flags);
	} else if (ord < 24) {
		mlib_c_fftDW_GetData24(dst, src_main, src_aux, wnd, order,
		    scale, data_info_flags);
	} else {
		mlib_c_fftDW_GetData32(dst, src_main, src_aux, wnd, order,
		    scale, data_info_flags);
	}
}

/* *********************************************************** */

static void
mlib_c_fftDW_PutData_4(
    deal_t *CONST pal64data,
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s32 CONST order,
    mlib_s32 CONST scale,
    mlib_s32 CONST data_info_flags)
{
	mlib_s32 CONST nn = 1 << order;
	mlib_s32 i = 0;
	CONST mlib_d64 mult = scale < 0 ? 1. / (1 << -scale) : 1 << scale;

/*
 *    scale results to get 15 bits significant guiding by scale
 *    no saturation needed
 */

/* saturate & put results into destination */

	if (data_info_flags & eDstCmplx) {
		for (i = 0; i < nn; i++) {
			dst_main[2 * i] = pal64data[2 * i] * mult;
			dst_main[2 * i + 1] = pal64data[2 * i + 1] * mult;
		}
	} else if (data_info_flags & eDstReIm) {
		for (i = 0; i < nn; i++) {
			dst_main[i] = pal64data[2 * i] * mult;
			dst_aux[i] = pal64data[2 * i + 1] * mult;
		}
	} else {
		for (i = 0; i < nn; i++) {
			dst_main[i] = pal64data[2 * i] * mult;
		}
	}
}

/* *********************************************************** */

static void
mlib_c_fftDW_PutData(
    deal_t *CONST pal64data,
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s32 CONST order,
    mlib_s32 CONST scale,
    mlib_s32 CONST data_info_flags,
    mlib_s32 CONST bool_is_windowed)
{
	mlib_s32 CONST nn = 1 << order;
	mlib_s32 i = 0;
	CONST mlib_d64 mult = 1. / (1 << (order - scale));

/* saturate & put results into destination */

	if (bool_is_windowed) {
		if (data_info_flags & eDstCmplx) {
			for (i = 0; i < nn; i++) {
				dst_main[2 * i] = pal64data[2 * i];
				dst_main[2 * i + 1] = pal64data[2 * i + 1];
			}
		} else if (data_info_flags & eDstReIm) {
			for (i = 0; i < nn; i++) {
				dst_main[i] = pal64data[2 * i];
				dst_aux[i] = pal64data[2 * i + 1];
			}
		} else {
			for (i = 0; i < nn; i++) {
				dst_main[i] = pal64data[2 * i];
			}
		}
	} else {
/* if( !bool_is_windowed ) */
		if (data_info_flags & eDstCmplx) {
			for (i = 0; i < nn; i++) {
				dst_main[2 * i] = pal64data[2 * i] * mult;
				dst_main[2 * i + 1] =
				    pal64data[2 * i + 1] * mult;
			}
		} else if (data_info_flags & eDstReIm) {
			for (i = 0; i < nn; i++) {
				dst_main[i] = pal64data[2 * i] * mult;
				dst_aux[i] = pal64data[2 * i + 1] * mult;
			}
		} else {
			for (i = 0; i < nn; i++) {
				dst_main[i] = pal64data[2 * i] * mult;
			}
		}
	}
}

/* *********************************************************** */

static mlib_status
mlib_c_fftDW_01(
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST shiftscale,
    mlib_s32 CONST data_info_flags)
{
/* serves lowest ( 0 & 1 ) orders, to easy unrolling in higher ones */
	deal_t buf[4] = { 0.0, 0.0, 0.0, 0.0 };
	mlib_d64 CONST multscale = shiftscale != 1 ? 0.5 : 1.0;

	if (order == 0) {
/* initialise deal buffer */

		if (window) {
			buf[0] = MULT_WIN_01(src_main[0], window[0]);

			if (data_info_flags & eSrcCmplx) {
				buf[1] = MULT_WIN_01(src_main[1], window[0]);
			} else if (data_info_flags & eSrcReIm) {
				buf[1] = MULT_WIN_01(src_aux[0], window[0]);
			}
		} else {
			buf[0] = src_main[0];

			if (data_info_flags & eSrcCmplx) {
				buf[1] = src_main[1];
			} else if (data_info_flags & eSrcReIm) {
				buf[1] = src_aux[0];
			}
		}

/* put results into destination */
		dst_main[0] = buf[0];

		if (data_info_flags & eDstCmplx)
			dst_main[1] = buf[1];
		else if (data_info_flags & eDstReIm)
			dst_aux[0] = buf[1];
		return (MLIB_SUCCESS);
	}

/* initialise deal buffer */

	if (window) {
		if (data_info_flags & eSrcCmplx) {
			buf[0] = MULT_WIN_01(src_main[0], window[0]);
			buf[1] = MULT_WIN_01(src_main[1], window[0]);
			buf[2] = MULT_WIN_01(src_main[2], window[1]);
			buf[3] = MULT_WIN_01(src_main[3], window[1]);
		} else if (data_info_flags & eSrcReIm) {
			buf[0] = MULT_WIN_01(src_main[0], window[0]);
			buf[1] = MULT_WIN_01(src_aux[0], window[0]);
			buf[2] = MULT_WIN_01(src_main[1], window[1]);
			buf[3] = MULT_WIN_01(src_aux[1], window[1]);
		} else {
			buf[0] = MULT_WIN_01(src_main[0], window[0]);
			buf[2] = MULT_WIN_01(src_main[1], window[1]);
		}
	} else {
		if (data_info_flags & eSrcCmplx) {
			buf[0] = src_main[0];
			buf[1] = src_main[1];
			buf[2] = src_main[2];
			buf[3] = src_main[3];
		} else if (data_info_flags & eSrcReIm) {
			buf[0] = src_main[0];
			buf[1] = src_aux[0];
			buf[2] = src_main[1];
			buf[3] = src_aux[1];
		} else {
			buf[0] = src_main[0];
			buf[2] = src_main[1];
		}
	}

/* butterfly */
	buf[2] = (buf[0] += buf[2]) - (2.0 * buf[2]);
	buf[3] = (buf[1] += buf[3]) - (2.0 * buf[3]);
/* scale */
	buf[0] *= multscale;
	buf[1] *= multscale;
	buf[2] *= multscale;
	buf[3] *= multscale;

/* put results into destination */

	if (data_info_flags & eDstCmplx) {
		dst_main[0] = buf[0];
		dst_main[1] = buf[1];
		dst_main[2] = buf[2];
		dst_main[3] = buf[3];
	} else if (data_info_flags & eDstReIm) {
		dst_main[0] = buf[0];
		dst_aux[0] = buf[1];
		dst_main[1] = buf[2];
		dst_aux[1] = buf[3];
	} else {
		dst_main[0] = buf[0];
		dst_main[1] = buf[2];
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_fftDispatcherW_S16C_S16(
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST shiftscale,
    mlib_s32 CONST data_info_flags)
{
/* useful values */
	size_t CONST size2deal = sizeof (deal_t) << (order + 1);

/* service things */
	mlib_d64 al64_stack_buffer[512];
	deal_t *pal64 = (deal_t *)al64_stack_buffer;
	deal_t *p_malloced = NULL;
	deal_t rot_r, rot_i, incr_r, incr_i, wtmp;
	mlib_s32 iii, length, quarter;
	CONST deal_t mult = window ? 1. : 1. / (1 << (order - shiftscale));

	SERVE_BAD_ORDER(order);

	if (order < 2) {
		return mlib_c_fftDW_01(dst_main, dst_aux, src_main, src_aux,
		    window, order, shiftscale, data_info_flags);
	}

	if (order == 2) {
		deal_t buf0, buf1, buf2, buf3;
		deal_t CONST multscale = .25 * (deal_t)(1 << shiftscale);

/* initialise deal buffer */

		if (!window) {
			buf0 = src_main[0];
			buf1 = src_main[1];
			buf2 = src_main[2];
			buf3 = src_main[3];
		} else {
			buf0 = MULT_WIN_01(src_main[0], window[0]);
			buf1 = MULT_WIN_01(src_main[1], window[1]);
			buf2 = MULT_WIN_01(src_main[2], window[2]);
			buf3 = MULT_WIN_01(src_main[3], window[3]);
		}

/* butterfly */
		buf2 = (buf0 += buf2) - (2.0 * buf2);
		buf3 = (buf1 += buf3) - (2.0 * buf3);
/* scale */
/* put results into destination */
		buf0 *= multscale;
		buf1 *= multscale;
		buf2 *= multscale;
		buf3 *= multscale;

		dst_main[2 * 0] = buf0 + buf1;
		dst_main[2 * 0 + 1] = dst_main[2 * 2 + 1] = 0;
		dst_main[2 * 1] = dst_main[2 * 3] = buf2;
		dst_main[2 * 1 + 1] = -(dst_main[2 * 3 + 1] = buf3);
		dst_main[2 * 2] = buf0 - buf1;
		return (MLIB_SUCCESS);
	}

/*
 *    determine whether there isn't enough place in stack
 *    and we must 'malloc' our buffer
 */

	if (size2deal > sizeof (al64_stack_buffer)) {
		if (!(p_malloced = (deal_t *)__mlib_malloc(size2deal)))
			return (MLIB_FAILURE);
		pal64 = p_malloced;
	}

	mlib_c_fftDW_GetData(pal64, src_main, src_aux, window, order,
	    shiftscale, data_info_flags);

/* deal it */
	mlib_c_fftKernel(pal64, order - 1, INVERS_FFT);

	length = 1 << order;
	quarter = length >> 2;

	wtmp = SINUS(order);
	rot_r = incr_r = 1. - 2. * wtmp * wtmp;
	rot_i = incr_i = SINUS(order - 1);

	for (iii = 1; iii < quarter; iii++) {
		mlib_s32 i1 = 2 * iii, i2 = i1 + 1, i3 = length - i1, i4 =
		    i3 + 1;
		mlib_s32 j1 = 2 * length - i1, j2 = j1 + 1, j3 =
		    2 * length - i3, j4 = j3 + 1;
		deal_t d1_r = pal64[i1], d1_i = pal64[i2];
		deal_t d2_r = pal64[i3], d2_i = -pal64[i4];
		deal_t h1r, h1i, h2r, h2i, h2rot_r, h2rot_i;

		h1r = (d1_r + d2_r);
		h1i = (d1_i + d2_i);
		h2r = (d1_i - d2_i);
		h2i = -(d1_r - d2_r);
		h2rot_r = rot_r * h2r - rot_i * h2i;
		h2rot_i = rot_r * h2i + rot_i * h2r;
		dst_main[i1] = mult * (pal64[i1] = .5 * (h1r + h2rot_r));
		dst_main[i2] = mult * (pal64[i2] = -.5 * (h1i + h2rot_i));
		dst_main[i3] = mult * (pal64[i3] = .5 * (h1r - h2rot_r));
		dst_main[i4] = mult * (pal64[i4] = -.5 * (-h1i + h2rot_i));
		dst_main[j1] = dst_main[i1];
		dst_main[j2] = -dst_main[i2];
		dst_main[j3] = dst_main[i3];
		dst_main[j4] = -dst_main[i4];
		ROTATE_CMPLX_TMP(rot, incr, wtmp);
	}

	wtmp = pal64[0] - pal64[1];

	dst_main[0] = mult * (pal64[0] += pal64[1]);
	dst_main[1] = mult * (pal64[1] = 0);

	dst_main[2 * quarter + 0] = mult * pal64[2 * quarter + 0];
	dst_main[2 * quarter + 1] = mult * (pal64[2 * quarter + 1] =
	    -pal64[2 * quarter + 1]);

	dst_main[2 * length - 2 * quarter] = dst_main[2 * quarter + 0];
	dst_main[2 * length - 2 * quarter + 1] = -dst_main[2 * quarter + 1];

	dst_main[length] = mult * (pal64[length] = wtmp);
	dst_main[length + 1] = mult * (pal64[length + 1] = 0);

	if (p_malloced)
		__mlib_free(p_malloced);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_fftDispatcherW_Common(
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST shiftscale,
    mlib_s32 CONST data_info_flags)
{
/* useful values */
	size_t CONST size2deal = sizeof (deal_t) << (order + 1);

/* service things */
	mlib_d64 al64_stack_buffer[512];
	deal_t *pal64 = (deal_t *)al64_stack_buffer;
	deal_t *p_malloced = NULL;

	SERVE_BAD_ORDER(order);

	if (order < 2) {
		return mlib_c_fftDW_01(dst_main, dst_aux, src_main, src_aux,
		    window, order, shiftscale, data_info_flags);
	}

/*
 *    determine whether there isn't enough place in stack
 *    and we must 'malloc' our buffer
 */

	if (size2deal > sizeof (al64_stack_buffer)) {
		if (!(p_malloced = (deal_t *)__mlib_malloc(size2deal)))
			return (MLIB_FAILURE);
		pal64 = p_malloced;
	}

	mlib_c_fftDW_GetData(pal64, src_main, src_aux, window, order,
	    shiftscale, data_info_flags);

/* deal it */
	mlib_c_fftKernel(pal64, order,
	    data_info_flags & eInversFFT ? INVERS_FFT : DIRECT_FFT);

	mlib_c_fftDW_PutData(pal64, dst_main, dst_aux, order, shiftscale,
	    data_info_flags, !!window);

	if (p_malloced)
		__mlib_free(p_malloced);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_fftDispatcherW_1(
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST data_info_flags)
{
	mlib_s32 shiftscale = data_info_flags & eInversFFT ? 0 : order;

	return mlib_c_fftDispatcherW_Common(dst_main, dst_aux, src_main,
	    src_aux, window, order, shiftscale, data_info_flags);
}

/* *********************************************************** */

mlib_status
mlib_c_fftDispatcherW_2(
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST data_info_flags)
{
	mlib_s32 shiftscale = data_info_flags & eInversFFT ? order : 0;

	return mlib_c_fftDispatcherW_Common(dst_main, dst_aux, src_main,
	    src_aux, window, order, shiftscale, data_info_flags);
}

/* *********************************************************** */

mlib_status
mlib_c_fftDispatcherW_3(
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST data_info_flags)
{
	mlib_s32 shiftscale =
	    data_info_flags & eInversFFT ? (order + 1) / 2 : order / 2;

	return mlib_c_fftDispatcherW_Common(dst_main, dst_aux, src_main,
	    src_aux, window, order, shiftscale, data_info_flags);
}

/* *********************************************************** */

mlib_status
mlib_c_fftDispatcherW_4_S16C_S16(
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 *CONST scale,
    mlib_s32 CONST data_info_flags)
{
/* useful values */
	size_t CONST size2deal = sizeof (deal_t) << (order + 1);

/* service things */
	mlib_d64 al64_stack_buffer[512];
	deal_t *pal64 = (deal_t *)al64_stack_buffer;
	deal_t *p_malloced = NULL;
	mlib_s32 retscale = 0;
	deal_t rot_r, rot_i, incr_r, incr_i, wtmp;
	mlib_s32 iii, length, quarter;
	deal_t mult = 1.;
	mlib_s32 all_ors = 0;

/* init scale factor */
	*scale = order;

	SERVE_BAD_ORDER(order);

	if (order < 2) {
		return mlib_c_fftDW_01(dst_main, dst_aux, src_main, src_aux,
		    window, order, SCALE_SIGNIFICANT, data_info_flags);
	}

	if (order == 2) {
		deal_t buf0, buf1, buf2, buf3;
		deal_t CONST multscale = .25;

/* initialise deal buffer */

		if (!window) {
			buf0 = src_main[0];
			buf1 = src_main[1];
			buf2 = src_main[2];
			buf3 = src_main[3];
		} else {
			buf0 = MULT_WIN_01(src_main[0], window[0]);
			buf1 = MULT_WIN_01(src_main[1], window[1]);
			buf2 = MULT_WIN_01(src_main[2], window[2]);
			buf3 = MULT_WIN_01(src_main[3], window[3]);
		}

/* butterfly */
		buf2 = (buf0 += buf2) - (2.0 * buf2);
		buf3 = (buf1 += buf3) - (2.0 * buf3);
/* scale */
/* put results into destination */
		buf0 *= multscale;
		buf1 *= multscale;
		buf2 *= multscale;
		buf3 *= multscale;

		dst_main[2 * 0] = buf0 + buf1;
		dst_main[2 * 0 + 1] = dst_main[2 * 2 + 1] = 0;
		dst_main[2 * 1] = dst_main[2 * 3] = buf2;
		dst_main[2 * 1 + 1] = -(dst_main[2 * 3 + 1] = buf3);
		dst_main[2 * 2] = buf0 - buf1;
		return (MLIB_SUCCESS);
	}

/*
 *    determine whether there isn't enough place in stack
 *    and we must 'malloc' our buffer
 */

	if (size2deal > sizeof (al64_stack_buffer)) {
		if (!(p_malloced = (deal_t *)__mlib_malloc(size2deal)))
			return (MLIB_FAILURE);
		pal64 = p_malloced;
	}

	mlib_c_fftDW_GetData_4(pal64, src_main, src_aux, window, order,
	    data_info_flags);

/* deal it */
	mlib_c_fftKernel(pal64, order - 1, INVERS_FFT);

	length = 1 << order;
	quarter = length >> 2;

	wtmp = SINUS(order);
	rot_r = incr_r = 1. - 2. * wtmp * wtmp;
	rot_i = incr_i = SINUS(order - 1);

	for (iii = 1; iii < quarter; iii++) {
		mlib_s32 i1 = 2 * iii, i2 = i1 + 1, i3 = length - i1, i4 =
		    i3 + 1;
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

	ST_ORS(wtmp = pal64[0] - pal64[1]);

	ST_ORS(pal64[0] += pal64[1]);

	ST_ORS(pal64[2 * quarter + 0] = pal64[2 * quarter + 0]);
	ST_ORS(pal64[2 * quarter + 1] = -pal64[2 * quarter + 1]);

	FIND_SCALEF(retscale, all_ors);

	mult = retscale < 0 ? 1. / (1 << -retscale) : 1 << retscale;

	dst_main[0] = mult * (pal64[0]);
	dst_main[0 + 1] = 0;

/* just filling the dependent part */
	for (iii = 2; iii < length; iii += 2) {
		dst_main[2 * length - iii] = (dst_main[iii] =
		    mult * pal64[iii]);
		dst_main[2 * length - iii + 1] = -(dst_main[iii + 1] =
		    mult * pal64[iii + 1]);
	}

	dst_main[length] = mult * wtmp;
	dst_main[length + 1] = 0;

	if (scale)
		*scale = order - retscale;

	if (p_malloced)
		__mlib_free(p_malloced);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_fftDispatcherW_4(
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 *CONST scale,
    mlib_s32 CONST data_info_flags)
{
/* useful values */
	size_t CONST size2deal = sizeof (deal_t) << (order + 1);

/* service things */
	mlib_d64 al64_stack_buffer[512];
	deal_t *pal64 = (deal_t *)al64_stack_buffer;
	deal_t *p_malloced = NULL;
	mlib_s32 retscale = 0;

/* init scale factor */
	*scale = order;

	SERVE_BAD_ORDER(order);

	if (order < 2) {
		return mlib_c_fftDW_01(dst_main, dst_aux, src_main, src_aux,
		    window, order, SCALE_SIGNIFICANT, data_info_flags);
	}

/*
 *    determine whether there isn't enough place in stack
 *    and we must 'malloc' our buffer
 */

	if (size2deal > sizeof (al64_stack_buffer)) {
		if (!(p_malloced = (deal_t *)__mlib_malloc(size2deal)))
			return (MLIB_FAILURE);
		pal64 = p_malloced;
	}

	mlib_c_fftDW_GetData_4(pal64, src_main, src_aux, window, order,
	    data_info_flags);

/* deal it */
	retscale =
	    mlib_c_fftKernel_4(pal64, order,
	    data_info_flags & eInversFFT ? INVERS_FFT : DIRECT_FFT);

	if (scale)
		*scale = order - retscale;

	mlib_c_fftDW_PutData_4(pal64, dst_main, dst_aux, order, retscale,
	    data_info_flags);

	if (p_malloced)
		__mlib_free(p_malloced);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
