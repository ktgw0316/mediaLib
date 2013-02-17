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

#pragma ident	"@(#)mlib_v_SignalFFTDisp_4.c	9.3	07/11/05 SMI"

/* *********************************************************** */
/* DISPATCHER */

/* *********************************************************** */
/*
 * FUNCTIONS
 *  NOTE: FFT below means F(ast)F(ourier)T(ransform)
 *      mlib_v_fftDispatcherW_4 - dispatches various FFT functions: creates
 *                                ( dynamically ) main working buffer,
 *                                fills it by source data, calls kernel FFT
 *                                function ( direct or inverse ) and stores
 *                                results in destination array(s)
 *      mlib_v_fftDW_GetData    - static function performing filling of
 *                                the working buffer by source data
 *      mlib_v_fftDW_PutData    - static function performing filling of
 *                                destination array by the data from
 *                                working buffer
 *
 * SYNOPSIS
 *      mlib_status mlib_v_fftDispatcherW_4
 *                  ( mlib_s16 *CONST dst_main,
 *                    mlib_s16 *CONST dst_aux,
 *                    mlib_s16 *CONST src_main,
 *                    mlib_s16 *CONST src_aux,
 *                    mlib_s16 *CONST window,
 *                    mlib_s32 CONST  order,
 *                    mlib_s32 *CONST scalef,
 *                    mlib_s32 CONST  data_info_flags);
 *      static mlib_s32 mlib_v_fftDW_GetData
 *                  ( deal_v_t *CONST pal64data,
 *                    mlib_s16 *CONST src_main,
 *                    mlib_s16 *CONST src_aux,
 *                    mlib_s16 *CONST window,
 *                    mlib_s32 CONST  order,
 *                    mlib_s32 CONST  data_info_flags);
 *      static void mlib_v_fftDW_PutData
 *                  ( deal_v_t *CONST pal64data,
 *                    mlib_s16 *CONST dst_main,
 *                    mlib_s16 *CONST dst_aux,
 *                    mlib_s16 *CONST window,
 *                    mlib_s32 CONST  order,
 *                    mlib_s32 CONST  data_info_flags);
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
 *      scale    - Place to store information of how destination was scaled
 *                 to keep significant bits.
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
 *      pal64data
 *               - 64bit-aligned complex array. Created dynamically inside
 *                 mlib_v_fftDispatcherW_4 ( from stack or __mlib_malloc ).
 *                 Destination for mlib_v_fftDW_GetData.
 *                 Source for mlib_v_fftDW_PutData.
 *                 Source and destination for kernel FFT function.
 *                 pal64data[ 2 * i ] hold the real parts, and
 *                 pal64data[ 2 * i + 1 ] hold the imaginary parts
 *
 * DESCRIPTION
 *
 *      dest = PutData( ( buffer = kernelFFT(
 *                        buffer = GetData( src[, window] ) ) ) )
 */

/* *********************************************************** */
/*
 * IMPLEMENTATION
 *
 * We don't process all the cases 'as they are'. Instead, we have one
 *  'dispatcher' function and call it from all the cases, passing it
 *  all necessary array pointers together with ( bit-flags composed )
 *  additional parameter describing each case specifics and pointers
 *  interpretation.
 * This way we avoid unnecessary code duplication and increase reliability
 *  ( indirectly, due to processing features incapsulation ).
 * 'Dispatcher' function creates 'unified-and-aligned' working buffer,
 *  fills it using source data ( and possibly window ), calls 'kernel'
 *  FFT function, passing this buffer as source and destination array,
 *  and then fills destination array(s) by data got from buffer
 */

/* include headers */

#include <stdlib.h>
#include <mlib_signal.h>
#include <vis_proto.h>
#include <mlib_SignalFFTUtil.h>

/* *********************************************************** */

/* max possible order 24 + 5 = 29 */
#define	REV_NBITS	24
#define	REV_INDX(indx, shift)	REV_INDX_24(indx, shift)

/* *********************************************************** */
/* function prototypes */

static mlib_s32 mlib_v_fftDW_GetData(
    deal_v_t *CONST pal64data,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST data_info_flags);

static void mlib_v_fftDW_PutData(
    deal_v_t *CONST pal64data,
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s32 CONST order,
    mlib_s32 CONST data_info_flags);

/* *********************************************************** */
/* macros, typedefs */

/* windowing macro */
#define	MULT_WIN(dat, win)                                              \
	(dat =                                                          \
	    vis_fpadd16(vis_fmul8sux16(win, dat), vis_fmul8ulx16(win,   \
	    dat)))

/* *********************************************************** */
/* accumulate 4 upper bits OR'ing abs'es of values */
#define	AND_NORM(dval)                                          \
	dnorm =                                                 \
	vis_for(dnorm, vis_fxor(dval, vis_fmul8sux16(dscale,    \
	    vis_fand(rounder, dval))));

/* *********************************************************** */
/* load data into deal buffers for various sources */
#define	LOAD_C_W(dst, src_main, src_aux, wnd, i_dst, fzeroVal)         \
	{                                                              \
	    mlib_d64 win, tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6,    \
		tmp7, tmp8, tmp9, tmpa, tmpb;                          \
	    AND_NORM(dr);                                              \
	    AND_NORM(di);                                              \
	    tmp4 = vis_ld_u16_i((void *)wnd, (i_dst##_3_rev) * 2);     \
	    win = vis_faligndata(tmp4, win);                           \
	    tmp5 = vis_ld_u16_i((void *)wnd, (i_dst##_2_rev) * 2);     \
	    win = vis_faligndata(tmp5, win);                           \
	    tmp6 = vis_ld_u16_i((void *)wnd, (i_dst##_1_rev) * 2);     \
	    win = vis_faligndata(tmp6, win);                           \
	    tmp7 = vis_ld_u16_i((void *)wnd, (i_dst##_0_rev) * 2);     \
	    win = vis_faligndata(tmp7, win);                           \
	    tmp0 =                                                     \
		vis_ld_u16_i((void *)src_main, (i_dst##_3_rev) * 4);   \
	    dr = vis_faligndata(tmp0, dr);                             \
	    tmp1 =                                                     \
		vis_ld_u16_i((void *)src_main, (i_dst##_2_rev) * 4);   \
	    dr = vis_faligndata(tmp1, dr);                             \
	    tmp2 =                                                     \
		vis_ld_u16_i((void *)src_main, (i_dst##_1_rev) * 4);   \
	    dr = vis_faligndata(tmp2, dr);                             \
	    tmp3 =                                                     \
		vis_ld_u16_i((void *)src_main, (i_dst##_0_rev) * 4);   \
	    dr = vis_faligndata(tmp3, dr);                             \
	    MULT_WIN(dr, win);                                         \
	    tmp8 =                                                     \
		vis_ld_u16_i((void *)(src_main + 1),                   \
		(i_dst##_3_rev) * 4);                                  \
	    di = vis_faligndata(tmp8, di);                             \
	    tmp9 =                                                     \
		vis_ld_u16_i((void *)(src_main + 1),                   \
		(i_dst##_2_rev) * 4);                                  \
	    di = vis_faligndata(tmp9, di);                             \
	    tmpa =                                                     \
		vis_ld_u16_i((void *)(src_main + 1),                   \
		(i_dst##_1_rev) * 4);                                  \
	    di = vis_faligndata(tmpa, di);                             \
	    tmpb =                                                     \
		vis_ld_u16_i((void *)(src_main + 1),                   \
		(i_dst##_0_rev) * 4);                                  \
	    di = vis_faligndata(tmpb, di);                             \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr;                       \
	    MULT_WIN(di, win);                                         \
	    *(mlib_d64 *)(dst##_i + i_dst) = di;                       \
	}

/* *********************************************************** */

#define	LOAD_RI_W(dst, src_main, src_aux, wnd, i_dst, fzeroVal)        \
	{                                                              \
	    mlib_d64 win, tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6,    \
		tmp7, tmp8, tmp9, tmpa, tmpb;                          \
	    AND_NORM(dr);                                              \
	    AND_NORM(di);                                              \
	    tmp0 = vis_ld_u16_i((void *)wnd, (i_dst##_3_rev) * 2);     \
	    win = vis_faligndata(tmp0, win);                           \
	    tmp1 = vis_ld_u16_i((void *)wnd, (i_dst##_2_rev) * 2);     \
	    win = vis_faligndata(tmp1, win);                           \
	    tmp2 = vis_ld_u16_i((void *)wnd, (i_dst##_1_rev) * 2);     \
	    win = vis_faligndata(tmp2, win);                           \
	    tmp3 = vis_ld_u16_i((void *)wnd, (i_dst##_0_rev) * 2);     \
	    win = vis_faligndata(tmp3, win);                           \
	    tmp4 =                                                     \
		vis_ld_u16_i((void *)src_main, (i_dst##_3_rev) * 2);   \
	    dr = vis_faligndata(tmp4, dr);                             \
	    tmp5 =                                                     \
		vis_ld_u16_i((void *)src_main, (i_dst##_2_rev) * 2);   \
	    dr = vis_faligndata(tmp5, dr);                             \
	    tmp6 =                                                     \
		vis_ld_u16_i((void *)src_main, (i_dst##_1_rev) * 2);   \
	    dr = vis_faligndata(tmp6, dr);                             \
	    tmp7 =                                                     \
		vis_ld_u16_i((void *)src_main, (i_dst##_0_rev) * 2);   \
	    dr = vis_faligndata(tmp7, dr);                             \
	    MULT_WIN(dr, win);                                         \
	    tmp8 =                                                     \
		vis_ld_u16_i((void *)src_aux, (i_dst##_3_rev) * 2);    \
	    di = vis_faligndata(tmp8, di);                             \
	    tmp9 =                                                     \
		vis_ld_u16_i((void *)src_aux, (i_dst##_2_rev) * 2);    \
	    di = vis_faligndata(tmp9, di);                             \
	    tmpa =                                                     \
		vis_ld_u16_i((void *)src_aux, (i_dst##_1_rev) * 2);    \
	    di = vis_faligndata(tmpa, di);                             \
	    tmpb =                                                     \
		vis_ld_u16_i((void *)src_aux, (i_dst##_0_rev) * 2);    \
	    di = vis_faligndata(tmpb, di);                             \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr;                       \
	    MULT_WIN(di, win);                                         \
	    *(mlib_d64 *)(dst##_i + i_dst) = di;                       \
	}

/* *********************************************************** */

#define	LOAD_R_W(dst, src_main, src_aux, wnd, i_dst, fzeroVal)         \
	{                                                              \
	    mlib_d64 win, tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6,    \
		tmp7;                                                  \
	    AND_NORM(dr);                                              \
	    tmp4 = vis_ld_u16_i((void *)wnd, (i_dst##_3_rev) * 2);     \
	    win = vis_faligndata(tmp4, win);                           \
	    tmp5 = vis_ld_u16_i((void *)wnd, (i_dst##_2_rev) * 2);     \
	    win = vis_faligndata(tmp5, win);                           \
	    tmp6 = vis_ld_u16_i((void *)wnd, (i_dst##_1_rev) * 2);     \
	    win = vis_faligndata(tmp6, win);                           \
	    tmp7 = vis_ld_u16_i((void *)wnd, (i_dst##_0_rev) * 2);     \
	    win = vis_faligndata(tmp7, win);                           \
	    tmp0 =                                                     \
		vis_ld_u16_i((void *)src_main, (i_dst##_3_rev) * 2);   \
	    dr = vis_faligndata(tmp0, dr);                             \
	    tmp1 =                                                     \
		vis_ld_u16_i((void *)src_main, (i_dst##_2_rev) * 2);   \
	    dr = vis_faligndata(tmp1, dr);                             \
	    tmp2 =                                                     \
		vis_ld_u16_i((void *)src_main, (i_dst##_1_rev) * 2);   \
	    dr = vis_faligndata(tmp2, dr);                             \
	    tmp3 =                                                     \
		vis_ld_u16_i((void *)src_main, (i_dst##_0_rev) * 2);   \
	    dr = vis_faligndata(tmp3, dr);                             \
	    *(mlib_d64 *)(dst##_i + i_dst) = fzeroVal;                 \
	    MULT_WIN(dr, win);                                         \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr;                       \
	}

/* *********************************************************** */

#define	LOAD_C(dst, src_main, src_aux, wnd, i_dst, fzeroVal)           \
	{                                                              \
	    mlib_d64 tmpr, tmpi;                                       \
	                                                               \
	    AND_NORM(dr);                                              \
	    AND_NORM(di);                                              \
	    tmpr =                                                     \
		vis_ld_u16_i((void *)src_main, (i_dst##_3_rev) * 4);   \
	    dr = vis_faligndata(tmpr, dr);                             \
	    tmpi =                                                     \
		vis_ld_u16_i((void *)(src_main + 1),                   \
		(i_dst##_3_rev) * 4);                                  \
	    di = vis_faligndata(tmpi, di);                             \
	    tmpr =                                                     \
		vis_ld_u16_i((void *)src_main, (i_dst##_2_rev) * 4);   \
	    dr = vis_faligndata(tmpr, dr);                             \
	    tmpi =                                                     \
		vis_ld_u16_i((void *)(src_main + 1),                   \
		(i_dst##_2_rev) * 4);                                  \
	    di = vis_faligndata(tmpi, di);                             \
	    tmpr =                                                     \
		vis_ld_u16_i((void *)src_main, (i_dst##_1_rev) * 4);   \
	    dr = vis_faligndata(tmpr, dr);                             \
	    tmpi =                                                     \
		vis_ld_u16_i((void *)(src_main + 1),                   \
		(i_dst##_1_rev) * 4);                                  \
	    di = vis_faligndata(tmpi, di);                             \
	    tmpr =                                                     \
		vis_ld_u16_i((void *)src_main, (i_dst##_0_rev) * 4);   \
	    dr = vis_faligndata(tmpr, dr);                             \
	    tmpi =                                                     \
		vis_ld_u16_i((void *)(src_main + 1),                   \
		(i_dst##_0_rev) * 4);                                  \
	    di = vis_faligndata(tmpi, di);                             \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr;                       \
	    *(mlib_d64 *)(dst##_i + i_dst) = di;                       \
	}

/* *********************************************************** */

#define	LOAD_RI(dst, src_main, src_aux, wnd, i_dst, fzeroVal)          \
	{                                                              \
	    mlib_d64 tmpr, tmpi;                                       \
	                                                               \
	    AND_NORM(dr);                                              \
	    AND_NORM(di);                                              \
	    tmpr =                                                     \
		vis_ld_u16_i((void *)src_main, (i_dst##_3_rev) * 2);   \
	    dr = vis_faligndata(tmpr, dr);                             \
	    tmpr =                                                     \
		vis_ld_u16_i((void *)src_main, (i_dst##_2_rev) * 2);   \
	    dr = vis_faligndata(tmpr, dr);                             \
	    tmpr =                                                     \
		vis_ld_u16_i((void *)src_main, (i_dst##_1_rev) * 2);   \
	    dr = vis_faligndata(tmpr, dr);                             \
	    tmpr =                                                     \
		vis_ld_u16_i((void *)src_main, (i_dst##_0_rev) * 2);   \
	    dr = vis_faligndata(tmpr, dr);                             \
	    tmpi =                                                     \
		vis_ld_u16_i((void *)src_aux, (i_dst##_3_rev) * 2);    \
	    di = vis_faligndata(tmpi, di);                             \
	    tmpi =                                                     \
		vis_ld_u16_i((void *)src_aux, (i_dst##_2_rev) * 2);    \
	    di = vis_faligndata(tmpi, di);                             \
	    tmpi =                                                     \
		vis_ld_u16_i((void *)src_aux, (i_dst##_1_rev) * 2);    \
	    di = vis_faligndata(tmpi, di);                             \
	    tmpi =                                                     \
		vis_ld_u16_i((void *)src_aux, (i_dst##_0_rev) * 2);    \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr;                       \
	    di = vis_faligndata(tmpi, di);                             \
	    *(mlib_d64 *)(dst##_i + i_dst) = di;                       \
	}

/* *********************************************************** */

#define	LOAD_R(dst, src_main, src_aux, wnd, i_dst, fzeroVal)           \
	{                                                              \
	    mlib_d64 tmp;                                              \
	                                                               \
	    AND_NORM(dr);                                              \
	    tmp =                                                      \
		vis_ld_u16_i((void *)src_main, (i_dst##_3_rev) * 2);   \
	    dr = vis_faligndata(tmp, dr);                              \
	    tmp =                                                      \
		vis_ld_u16_i((void *)src_main, (i_dst##_2_rev) * 2);   \
	    dr = vis_faligndata(tmp, dr);                              \
	    tmp =                                                      \
		vis_ld_u16_i((void *)src_main, (i_dst##_1_rev) * 2);   \
	    dr = vis_faligndata(tmp, dr);                              \
	    tmp =                                                      \
		vis_ld_u16_i((void *)src_main, (i_dst##_0_rev) * 2);   \
	    *(mlib_d64 *)(dst##_i + i_dst) = fzeroVal;                 \
	    dr = vis_faligndata(tmp, dr);                              \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr;                       \
	}

/* *********************************************************** */

#define	LOOP_REV(leng, ord_arr, nnn, ppp, dst, src_main,                  \
	src_aux, wnd, load_macro)                                         \
	{                                                                 \
	    mlib_s32 kkk, hgh, low, shift = 5 + leng - ord_arr, o_5 =     \
		ord_arr - 5;                                              \
	    mlib_s32 rev_1 = 1 << o_5, rev_2 = 2 << o_5, rev_3 =          \
		3 << o_5;                                                 \
	    mlib_d64 fzeroVal = vis_fzero();                              \
	    mlib_d64 dr = fzeroVal, di = fzeroVal;                        \
	                                                                  \
	    vis_write_gsr(6);                                             \
	    for (kkk = 0; kkk < 8; )                                       \
		ppp[kkk++] <<= o_5;                                       \
	    for (kkk = hgh = 0; kkk < nnn; hgh++) {                       \
		mlib_s32 hgh_rev = REV_INDX(hgh, shift);                  \
		mlib_s32 kkk_0_rev = hgh_rev;                             \
	                                                                  \
		for (low = 0; low < 8; kkk += 4) {                        \
		    mlib_s32 kkk_3_rev = kkk_0_rev | rev_3, kkk_2_rev =   \
			kkk_0_rev | rev_2, kkk_1_rev =                    \
			kkk_0_rev | rev_1;                                \
		    load_macro(dst, src_main, src_aux, wnd, kkk,          \
			fzeroVal);                                        \
		    kkk_0_rev = ppp[++low] | hgh_rev;                     \
		}                                                         \
	    }                                                             \
	    AND_NORM(dr);                                                 \
	    AND_NORM(di);                                                 \
	}

/* *********************************************************** */
/* code to perform bit reversion at all orders regarding load_macro */
#define	LOAD_BIT_REV(ord_arr, dst, src_main, src_aux, wnd,          \
	load_macro)                                                 \
	{                                                           \
	    mlib_s32 CONST nnn = 1 << ord_arr;                      \
	    deal_v_t *CONST dst##_r = dst, *CONST dst##_i =         \
		dst + nnn;                                          \
	    mlib_s32 ppp[9] = {                                     \
		0, 16, 8, 24, 4, 20, 12, 28                         \
	    };                                                      \
	    LOOP_REV(REV_NBITS, ord_arr, nnn, ppp, dst, src_main,   \
		src_aux, wnd, load_macro);                          \
	}

/* ************************************************************ */

#define	COND(i, dn, dz)	(!vis_fcmplt16(dn = vis_fpadd16(dn, dn), dz) && i < 6)

/* *********************************************************** */
/* find scale */
#define	FIND_SCALE(i, dn, dz)                                            \
	{                                                                \
	    for (i = 0; COND(i, dn, dz); )                               \
		i++;                                                     \
	    if (i > 0)                                                   \
		i--;                                                     \
	    if (i > 3)                                                   \
		i = 3;                                                   \
	}

/* *********************************************************** */

#define	MIX_REIM(re, im, mx1, mx2)                                   \
	{                                                            \
	    mlib_d64 rr, ii, ri1, ri2;                               \
	                                                             \
	    rr = vis_fpmerge(vis_read_hi(re), vis_read_lo(re));      \
	    rr = vis_fpmerge(vis_read_hi(rr), vis_read_lo(rr));      \
	    ii = vis_fpmerge(vis_read_hi(im), vis_read_lo(im));      \
	    ii = vis_fpmerge(vis_read_hi(ii), vis_read_lo(ii));      \
	    ri1 = vis_fpmerge(vis_read_hi(rr), vis_read_hi(ii));     \
	    ri2 = vis_fpmerge(vis_read_lo(rr), vis_read_lo(ii));     \
	    mx1 = vis_fpmerge(vis_read_hi(ri1), vis_read_hi(ri2));   \
	    mx2 = vis_fpmerge(vis_read_lo(ri1), vis_read_lo(ri2));   \
	}

/* *********************************************************** */
/* functions codes */

/* *********************************************************** */

static mlib_s32
mlib_v_fftDW_LdBr_4_C_W(
    mlib_s32 CONST order,
    deal_v_t *CONST pal64data,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window)
{
	mlib_d64 const dscale = vis_to_double_dup(0x08000800), dzero =
	    vis_fzero();
	mlib_d64 const rounder = vis_to_double_dup(0xff00ff00);
	mlib_d64 dnorm = vis_fzero();
	mlib_s32 scale = 0;

	LOAD_BIT_REV(order, pal64data, src_main, src_aux, window, LOAD_C_W);
	FIND_SCALE(scale, dnorm, dzero);
	return (scale);
}

/* *********************************************************** */

static mlib_s32
mlib_v_fftDW_LdBr_4_C(
    mlib_s32 CONST order,
    deal_v_t *CONST pal64data,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window)
{
	mlib_d64 const dscale = vis_to_double_dup(0x08000800), dzero =
	    vis_fzero();
	mlib_d64 const rounder = vis_to_double_dup(0xff00ff00);
	mlib_d64 dnorm = vis_fzero();
	mlib_s32 scale = 0;

	LOAD_BIT_REV(order, pal64data, src_main, src_aux, window, LOAD_C);
	FIND_SCALE(scale, dnorm, dzero);
	return (scale);
}

/* *********************************************************** */

static mlib_s32
mlib_v_fftDW_LdBr_4_RI_W(
    mlib_s32 CONST order,
    deal_v_t *CONST pal64data,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window)
{
	mlib_d64 const dscale = vis_to_double_dup(0x08000800), dzero =
	    vis_fzero();
	mlib_d64 const rounder = vis_to_double_dup(0xff00ff00);
	mlib_d64 dnorm = vis_fzero();
	mlib_s32 scale = 0;

	LOAD_BIT_REV(order, pal64data, src_main, src_aux, window, LOAD_RI_W);
	FIND_SCALE(scale, dnorm, dzero);
	return (scale);
}

/* *********************************************************** */

static mlib_s32
mlib_v_fftDW_LdBr_4_RI(
    mlib_s32 CONST order,
    deal_v_t *CONST pal64data,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window)
{
	mlib_d64 const dscale = vis_to_double_dup(0x08000800), dzero =
	    vis_fzero();
	mlib_d64 const rounder = vis_to_double_dup(0xff00ff00);
	mlib_d64 dnorm = vis_fzero();
	mlib_s32 scale = 0;

	LOAD_BIT_REV(order, pal64data, src_main, src_aux, window, LOAD_RI);
	FIND_SCALE(scale, dnorm, dzero);
	return (scale);
}

/* *********************************************************** */

static mlib_s32
mlib_v_fftDW_LdBr_4_R_W(
    mlib_s32 CONST order,
    deal_v_t *CONST pal64data,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window)
{
	mlib_d64 const dscale = vis_to_double_dup(0x08000800), dzero =
	    vis_fzero();
	mlib_d64 const rounder = vis_to_double_dup(0xff00ff00);
	mlib_d64 dnorm = vis_fzero();
	mlib_s32 scale = 0;

	LOAD_BIT_REV(order, pal64data, src_main, src_aux, window, LOAD_R_W);
	FIND_SCALE(scale, dnorm, dzero);
	return (scale);
}

/* *********************************************************** */

static mlib_s32
mlib_v_fftDW_LdBr_4_R(
    mlib_s32 CONST order,
    deal_v_t *CONST pal64data,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window)
{
	mlib_d64 const dscale = vis_to_double_dup(0x08000800), dzero =
	    vis_fzero();
	mlib_d64 const rounder = vis_to_double_dup(0xff00ff00);
	mlib_d64 dnorm = vis_fzero();
	mlib_s32 scale = 0;

	LOAD_BIT_REV(order, pal64data, src_main, src_aux, window, LOAD_R);
	FIND_SCALE(scale, dnorm, dzero);
	return (scale);
}

/* *********************************************************** */

static mlib_s32
mlib_v_fftDW_GetData(
    deal_v_t *CONST pal64data,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST data_info_flags)
{
	mlib_s32 scale = 0;

/* initialize deal buffer & serve window */
	if (data_info_flags & eSrcCmplx) {
		if (window) {
			scale =
			    mlib_v_fftDW_LdBr_4_C_W(order, pal64data, src_main,
			    src_aux, window);
		} else {
			scale =
			    mlib_v_fftDW_LdBr_4_C(order, pal64data, src_main,
			    src_aux, window);
		}
	} else if (data_info_flags & eSrcReIm) {
		if (window) {
			scale =
			    mlib_v_fftDW_LdBr_4_RI_W(order, pal64data, src_main,
			    src_aux, window);
		} else {
			scale =
			    mlib_v_fftDW_LdBr_4_RI(order, pal64data, src_main,
			    src_aux, window);
		}
	} else {
		if (window) {
			scale =
			    mlib_v_fftDW_LdBr_4_R_W(order, pal64data, src_main,
			    src_aux, window);
		} else {
			scale =
			    mlib_v_fftDW_LdBr_4_R(order, pal64data, src_main,
			    src_aux, window);
		}
	}

	return (scale);
}

/* *********************************************************** */

static void
mlib_v_fftDW_PutData(
    deal_v_t *CONST pal64data,
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s32 CONST order,
    mlib_s32 CONST data_info_flags)
{
	mlib_s32 CONST nn = 1 << order;
	deal_v_t *CONST pal64data_r = pal64data;
	deal_v_t *CONST pal64data_i = pal64data + nn;

/* saturate & put results into destination */
	if (data_info_flags & eDstCmplx) {
		mlib_s32 i = 0, count = nn >> 3;
		mlib_d64 *dst64 = (mlib_d64 *)((mlib_addr)dst_main & ~7);
		mlib_d64 *dstvar64 = dst64;
		mlib_d64 *srcvar_r = (mlib_d64 *)pal64data_r;
		mlib_d64 *srcvar_i = (mlib_d64 *)pal64data_i;

/* loc and pub */
		mlib_d64 ir0, ii0, mix0, mix1, mix3, out1, out2, mixlast;
		mlib_s32 mask1 = vis_edge16(dst_main, dst_main + 8);
		mlib_s32 mask2 = 0xf ^ mask1;

		if (!((mlib_addr)dst_main & 7)) {
			--dstvar64;
			mask1 = 0;
			mask2 = 0xf;
		}

		vis_alignaddr(dst64, (8 - (mlib_addr)dst_main) & 7);

		ir0 = ((mlib_d64 *)srcvar_r)[0];
		ii0 = ((mlib_d64 *)srcvar_i)[0];
		MIX_REIM(ir0, ii0, mix0, mix1);
		out2 = vis_faligndata(mix0, mix1);
		dstvar64[1] = out2;
		out1 = vis_faligndata(mix0, mix0);
		vis_pst_16(out1, dstvar64, mask1);

		mix3 = mix1;
		dstvar64 += 2;
		ir0 = srcvar_r[1];
		ii0 = srcvar_i[1];
		srcvar_r += 2;
		srcvar_i += 2;

#pragma pipeloop(0)
		for (i = 1; i < count; i++) {
			mlib_d64 ir1, ii1, mix0, mix1, mix2, out1, out2, out3,
			    out4;
			ir1 = srcvar_r[0];
			MIX_REIM(ir0, ii0, mix0, mix1);
			ii1 = srcvar_i[0];
			mixlast = mix3;
			MIX_REIM(ir1, ii1, mix2, mix3);
			ir0 = srcvar_r[1];
			ii0 = srcvar_i[1];
			srcvar_r += 2;
			srcvar_i += 2;

			out1 = vis_faligndata(mixlast, mix0);
			dstvar64[0] = out1;

			out2 = vis_faligndata(mix0, mix1);
			dstvar64[1] = out2;

			out3 = vis_faligndata(mix1, mix2);
			dstvar64[2] = out3;

			out4 = vis_faligndata(mix2, mix3);
			dstvar64[3] = out4;
			dstvar64 += 4;
		}

#pragma pipeloop(1)
		MIX_REIM(ir0, ii0, mix0, mix1);
		out1 = vis_faligndata(mix3, mix0);
		dstvar64[0] = out1;

		out2 = vis_faligndata(mix0, mix1);
		dstvar64[1] = out2;

		out1 = vis_faligndata(mix1, mix1);
		vis_pst_16(out1, dstvar64 + 2, mask2);
	} else {
		mlib_s32 i, count = nn >> 3;
		mlib_d64 *dst64 = (mlib_d64 *)((mlib_addr)dst_main & ~7);
		mlib_d64 *dstvar64 = dst64 + 1;
		mlib_d64 *srcvar_r = (mlib_d64 *)pal64data_r;
		mlib_d64 ir0, ir1, out1, out2, irfirst;
		mlib_s32 mask1 = vis_edge16(dst_main, dst_main + 8);
		mlib_s32 mask2 = 0xf ^ mask1;

		if (!((mlib_addr)dst_main & 7)) {
			--dstvar64;
			mask1 = 0;
			mask2 = 0xf;
		}

		vis_alignaddr(dst64, (8 - (mlib_addr)dst_main) & 7);

		ir0 = irfirst = ((mlib_d64 *)srcvar_r)[0];
#pragma pipeloop(0)
		for (i = 1; i < count; i++) {
			ir1 = ((mlib_d64 *)srcvar_r)[1];
			srcvar_r += 2;
			out1 = vis_faligndata(ir0, ir1);
			ir0 = ((mlib_d64 *)srcvar_r)[0];
			dstvar64[0] = out1;

			out2 = vis_faligndata(ir1, ir0);
			dstvar64[1] = out2;
			dstvar64 += 2;
		}

#pragma pipeloop(1)
		ir1 = ((mlib_d64 *)srcvar_r)[1];
		out1 = vis_faligndata(ir0, ir1);
		dstvar64[0] = out1;

		out2 = vis_faligndata(ir1, irfirst);
		vis_pst_16(out2, dst64, mask1);
		vis_pst_16(out2, dstvar64 + 1, mask2);

		if (data_info_flags & eDstReIm) {

			mlib_d64 *srcvar_i = (mlib_d64 *)pal64data_i;

			dst64 = (mlib_d64 *)((mlib_addr)dst_aux & ~7);
			dstvar64 = dst64 + 1;
			mask1 = vis_edge16(dst_aux, dst_aux + 8);
			mask2 = 0xf ^ mask1;

			if (!((mlib_addr)dst_aux & 7)) {
				--dstvar64;
				mask1 = 0;
				mask2 = 0xf;
			}

			vis_alignaddr(dst64, (8 - (mlib_addr)dst_aux) & 7);

			ir0 = irfirst = ((mlib_d64 *)srcvar_i)[0];

#pragma pipeloop(0)
			for (i = 1; i < count; i++) {
				ir1 = ((mlib_d64 *)srcvar_i)[1];
				srcvar_i += 2;
				out1 = vis_faligndata(ir0, ir1);
				ir0 = ((mlib_d64 *)srcvar_i)[0];
				dstvar64[0] = out1;

				out2 = vis_faligndata(ir1, ir0);
				dstvar64[1] = out2;

				dstvar64 += 2;
			}

#pragma pipeloop(1)
			ir1 = ((mlib_d64 *)srcvar_i)[1];
			out1 = vis_faligndata(ir0, ir1);
			dstvar64[0] = out1;

			out2 = vis_faligndata(ir1, irfirst);
			vis_pst_16(out2, dst64, mask1);
			vis_pst_16(out2, dstvar64 + 1, mask2);
		}
	}
}

/* *********************************************************** */

mlib_status
mlib_v_fftDispatcherW_4(
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 *CONST scalef,
    mlib_s32 CONST data_info_flags)
{
/*
 *    NOTE. Thanks a lot to VLog for his hint to hide
 *      'inverse-fft-scaling' inside of windowing/saturation
 */
/* useful values */
	size_t CONST size2deal = sizeof (deal_v_t) << (order + 1);
	mlib_s32 retscale = 0;
	mlib_s32 scaleGet = 0;

/* service things */
	mlib_d64 al64_stack_buffer[512];
	deal_v_t *pal64 = (deal_v_t *)al64_stack_buffer;
	deal_v_t *p_malloced = NULL;

	*scalef = 0;

/*
 *    determine whether there isn't enough place in stack
 *    and we must 'malloc' our buffer
 */
	if (size2deal > sizeof (al64_stack_buffer)) {
		if (!(p_malloced = (deal_v_t *)__mlib_malloc(size2deal)))
			return (MLIB_FAILURE);
		pal64 = p_malloced;
	}

	scaleGet = mlib_v_fftDW_GetData(pal64,
	    src_main, src_aux, window, order, data_info_flags);

/* deal it */
	retscale = mlib_v_fftKernel_4(pal64, pal64 + (1 << order), order,
	    scaleGet, data_info_flags & eInversFFT ? INVERS_FFT : DIRECT_FFT);
	*scalef = window ? retscale + 1 : retscale;

	mlib_v_fftDW_PutData(pal64, dst_main, dst_aux, order, data_info_flags);

	if (p_malloced)
		__mlib_free(p_malloced);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
