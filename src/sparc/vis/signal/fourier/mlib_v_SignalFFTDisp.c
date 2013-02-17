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

#pragma ident	"@(#)mlib_v_SignalFFTDisp.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *  NOTE: FFT below means F(ast)F(ourier)T(ransform)
 *     mlib_v_fftDispatcherW_? - dispatches various FFT functions: creates
 *                               ( dynamically ) main working buffer,
 *                               fills it by source data, calls kernel FFT
 *                               function ( direct or inverse ) and stores
 *                               results in destination array(s)
 *      mlib_v_fftDW_GetData   - static function performing filling of
 *                               the working buffer by source data
 *      mlib_v_fftDW_PutData   - static function performing filling of
 *                               destination array by the data from
 *                               working buffer
 *
 * SYNOPSIS
 *      mlib_status mlib_v_fftDispatcherW_[1|2|3]
 *                  (mlib_s16 *CONST dst_main,
 *                   mlib_s16 *CONST dst_aux,
 *                   mlib_s16 *CONST src_main,
 *                   mlib_s16 *CONST src_aux,
 *                   mlib_s16 *CONST window,
 *                   mlib_s32 CONST  order,
 *                   mlib_s32 CONST  data_info_flags);
 *      static void mlib_v_fftDW_GetData
 *                  (deal_v_t *CONST pal64data,
 *                   mlib_s16 *CONST src_main,
 *                   mlib_s16 *CONST src_aux,
 *                   mlib_s16 *CONST window,
 *                   mlib_s32 CONST  order,
 *                   mlib_s32 CONST  scaleup,
 *                   mlib_s32 CONST  data_info_flags);
 *      static void mlib_v_fftDW_PutData
 *                  (deal_v_t *CONST pal64data,
 *                   mlib_s16 *CONST dst_main,
 *                   mlib_s16 *CONST dst_aux,
 *                   mlib_s16 *CONST window,
 *                   mlib_s32 CONST  order,
 *                   mlib_s32 CONST  data_info_flags);
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
 *      pal64data
 *               - 64bit-aligned complex array. Created dynamically inside
 *                 mlib_v_fftDispatcherW ( from stack or __mlib_malloc ).
 *                 Destination for mlib_v_fftDW_GetData.
 *                 Source for mlib_v_fftDW_PutData.
 *                 Source and destination for kernel FFT function.
 *                 pal64data[ 2 * i ] hold the real parts, and
 *                 pal64data[ 2 * i + 1 ] hold the imaginary parts
 *
 * DESCRIPTION
 *
 *      dest = PutData(( buffer = kernelFFT(
 *                       buffer = GetData( src[, window] )) ))
 *
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

/*  include headers  */

#include <stdlib.h>
#include <mlib_signal.h>
#include <vis_proto.h>
#include <mlib_SignalFFTUtil.h>
#include <mlib_v_SignalFFT_Tbl.h>
#include <mlib_v_SignalFFTDisp.h>

/* *********************************************************** */

/* max possible order 24 + 5 = 29 */
#define	REV_NBITS	24
#define	REV_INDX(indx, shift)	REV_INDX_24(indx, shift)

/* *********************************************************** */

/*  windowing macro  */
#define	MULT_WIN(dat, win)                                                 \
	dat =                                                              \
	vis_fpadd16(vis_fmul8sux16(win, dat), vis_fmul8ulx16(win, dat));   \
	dat = vis_fpadd16(dat, dat);

/* *********************************************************** */

/*  windowing macro for scaled's  */
#define	MULD_WIN(fdat, fwin)                                             \
	vis_fpadd32(vis_fmuld8sux16(fwin, fdat), vis_fmuld8ulx16(fwin,   \
	    fdat))

/* *********************************************************** */

/*  scaled's  */
#define	MULD_SHL(shl6, fdat)	vis_fmuld8sux16(shl6, fdat)

/* *********************************************************** */

/*  load data into deal buffers for various sources  */
#define	SC_LOAD_C_W(dst, src_main, src_aux, wnd, i_dst, fzeroVal)         \
	{                                                                 \
	    mlib_d64 win, dr, di, tmp0, tmp1, tmp2, tmp3, tmp4, tmp5,     \
		tmp6, tmp7, tmp8, tmp9, tmpa, tmpb, rhr, rlr, rhi, rli;   \
	    tmp4 = vis_ld_u16_i((void *)wnd, (i_dst##_3_rev) * 2);        \
	    win = vis_faligndata(tmp4, tmp4);                             \
	    tmp5 = vis_ld_u16_i((void *)wnd, (i_dst##_2_rev) * 2);        \
	    win = vis_faligndata(tmp5, win);                              \
	    tmp0 =                                                        \
		vis_ld_u16_i((void *)src_main, (i_dst##_3_rev) * 4);      \
	    dr = vis_faligndata(tmp0, tmp0);                              \
	    tmp1 =                                                        \
		vis_ld_u16_i((void *)src_main, (i_dst##_2_rev) * 4);      \
	    dr = vis_faligndata(tmp1, dr);                                \
	    rlr = MULD_WIN(vis_read_hi(win), vis_read_hi(dr));            \
	    tmp8 =                                                        \
		vis_ld_u16_i((void *)(src_main + 1),                      \
		(i_dst##_3_rev) * 4);                                     \
	    di = vis_faligndata(tmp8, tmp8);                              \
	    tmp9 =                                                        \
		vis_ld_u16_i((void *)(src_main + 1),                      \
		(i_dst##_2_rev) * 4);                                     \
	    di = vis_faligndata(tmp9, di);                                \
	    rli = MULD_WIN(vis_read_hi(win), vis_read_hi(di));            \
	    tmp6 = vis_ld_u16_i((void *)wnd, (i_dst##_1_rev) * 2);        \
	    win = vis_faligndata(tmp6, tmp6);                             \
	    tmp7 = vis_ld_u16_i((void *)wnd, (i_dst##_0_rev) * 2);        \
	    win = vis_faligndata(tmp7, win);                              \
	    tmp2 =                                                        \
		vis_ld_u16_i((void *)src_main, (i_dst##_1_rev) * 4);      \
	    dr = vis_faligndata(tmp2, tmp2);                              \
	    tmp3 =                                                        \
		vis_ld_u16_i((void *)src_main, (i_dst##_0_rev) * 4);      \
	    dr = vis_faligndata(tmp3, dr);                                \
	    rhr = MULD_WIN(vis_read_hi(win), vis_read_hi(dr));            \
	    dr = vis_fpackfix_pair(rhr, rlr);                             \
	    tmpa =                                                        \
		vis_ld_u16_i((void *)(src_main + 1),                      \
		(i_dst##_1_rev) * 4);                                     \
	    di = vis_faligndata(tmpa, tmpa);                              \
	    tmpb =                                                        \
		vis_ld_u16_i((void *)(src_main + 1),                      \
		(i_dst##_0_rev) * 4);                                     \
	    di = vis_faligndata(tmpb, di);                                \
	    rhi = MULD_WIN(vis_read_hi(win), vis_read_hi(di));            \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr;                          \
	    di = vis_fpackfix_pair(rhi, rli);                             \
	    *(mlib_d64 *)(dst##_i + i_dst) = di;                          \
	}

/* *********************************************************** */

#define	SC_LOAD_RI_W(dst, src_main, src_aux, wnd, i_dst, fzeroVal)        \
	{                                                                 \
	    mlib_d64 win, dr, di, tmp0, tmp1, tmp2, tmp3, tmp4, tmp5,     \
		tmp6, tmp7, tmp8, tmp9, tmpa, tmpb, rhr, rlr, rhi, rli;   \
	    tmp4 = vis_ld_u16_i((void *)wnd, (i_dst##_3_rev) * 2);        \
	    win = vis_faligndata(tmp4, tmp4);                             \
	    tmp5 = vis_ld_u16_i((void *)wnd, (i_dst##_2_rev) * 2);        \
	    win = vis_faligndata(tmp5, win);                              \
	    tmp0 =                                                        \
		vis_ld_u16_i((void *)src_main, (i_dst##_3_rev) * 2);      \
	    dr = vis_faligndata(tmp0, tmp0);                              \
	    tmp1 =                                                        \
		vis_ld_u16_i((void *)src_main, (i_dst##_2_rev) * 2);      \
	    dr = vis_faligndata(tmp1, dr);                                \
	    rlr = MULD_WIN(vis_read_hi(win), vis_read_hi(dr));            \
	    tmp8 =                                                        \
		vis_ld_u16_i((void *)(src_aux),                           \
		(i_dst##_3_rev) * 2);                                     \
	    di = vis_faligndata(tmp8, tmp8);                              \
	    tmp9 =                                                        \
		vis_ld_u16_i((void *)(src_aux),                           \
		(i_dst##_2_rev) * 2);                                     \
	    di = vis_faligndata(tmp9, di);                                \
	    rli = MULD_WIN(vis_read_hi(win), vis_read_hi(di));            \
	    tmp6 = vis_ld_u16_i((void *)wnd, (i_dst##_1_rev) * 2);        \
	    win = vis_faligndata(tmp6, tmp6);                             \
	    tmp7 = vis_ld_u16_i((void *)wnd, (i_dst##_0_rev) * 2);        \
	    win = vis_faligndata(tmp7, win);                              \
	    tmp2 =                                                        \
		vis_ld_u16_i((void *)src_main, (i_dst##_1_rev) * 2);      \
	    dr = vis_faligndata(tmp2, tmp2);                              \
	    tmp3 =                                                        \
		vis_ld_u16_i((void *)src_main, (i_dst##_0_rev) * 2);      \
	    dr = vis_faligndata(tmp3, dr);                                \
	    rhr = MULD_WIN(vis_read_hi(win), vis_read_hi(dr));            \
	    dr = vis_fpackfix_pair(rhr, rlr);                             \
	    tmpa =                                                        \
		vis_ld_u16_i((void *)(src_aux),                           \
		(i_dst##_1_rev) * 2);                                     \
	    di = vis_faligndata(tmpa, tmpa);                              \
	    tmpb =                                                        \
		vis_ld_u16_i((void *)(src_aux),                           \
		(i_dst##_0_rev) * 2);                                     \
	    di = vis_faligndata(tmpb, di);                                \
	    rhi = MULD_WIN(vis_read_hi(win), vis_read_hi(di));            \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr;                          \
	    di = vis_fpackfix_pair(rhi, rli);                             \
	    *(mlib_d64 *)(dst##_i + i_dst) = di;                          \
	}

/* *********************************************************** */

#define	SC_LOAD_R_W(dst, src_main, src_aux, wnd, i_dst, fzeroVal)         \
	{                                                                 \
	    mlib_d64 win, dr, tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6,   \
		tmp7, rhr, rlr;                                           \
	    tmp4 = vis_ld_u16_i((void *)wnd, (i_dst##_3_rev) * 2);        \
	    win = vis_faligndata(tmp4, tmp4);                             \
	    tmp5 = vis_ld_u16_i((void *)wnd, (i_dst##_2_rev) * 2);        \
	    win = vis_faligndata(tmp5, win);                              \
	    tmp0 =                                                        \
		vis_ld_u16_i((void *)src_main, (i_dst##_3_rev) * 2);      \
	    dr = vis_faligndata(tmp0, tmp0);                              \
	    tmp1 =                                                        \
		vis_ld_u16_i((void *)src_main, (i_dst##_2_rev) * 2);      \
	    dr = vis_faligndata(tmp1, dr);                                \
	    rlr = MULD_WIN(vis_read_hi(win), vis_read_hi(dr));            \
	    tmp6 = vis_ld_u16_i((void *)wnd, (i_dst##_1_rev) * 2);        \
	    win = vis_faligndata(tmp6, tmp6);                             \
	    tmp7 = vis_ld_u16_i((void *)wnd, (i_dst##_0_rev) * 2);        \
	    win = vis_faligndata(tmp7, win);                              \
	    tmp2 =                                                        \
		vis_ld_u16_i((void *)src_main, (i_dst##_1_rev) * 2);      \
	    dr = vis_faligndata(tmp2, tmp2);                              \
	    tmp3 =                                                        \
		vis_ld_u16_i((void *)src_main, (i_dst##_0_rev) * 2);      \
	    dr = vis_faligndata(tmp3, dr);                                \
	    rhr = MULD_WIN(vis_read_hi(win), vis_read_hi(dr));            \
	    *(mlib_d64 *)(dst##_i + i_dst) = fzeroVal;                    \
	    dr = vis_fpackfix_pair(rhr, rlr);                             \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr;                          \
	}

/* *********************************************************** */

#define	SC_LOAD_C(dst, src_main, src_aux, wnd, i_dst, fzeroVal)          \
	{                                                                \
	    mlib_d64 dr, di, tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6,   \
		tmp7, rhr, rlr, rhi, rli;                                \
	    tmp0 =                                                       \
		vis_ld_u16_i((void *)src_main, (i_dst##_3_rev) * 4);     \
	    dr = vis_faligndata(tmp0, tmp0);                             \
	    tmp1 =                                                       \
		vis_ld_u16_i((void *)src_main, (i_dst##_2_rev) * 4);     \
	    dr = vis_faligndata(tmp1, dr);                               \
	    rlr = MULD_SHL(shl6, vis_read_hi(dr));                       \
	    tmp4 =                                                       \
		vis_ld_u16_i((void *)(src_main + 1),                     \
		(i_dst##_3_rev) * 4);                                    \
	    di = vis_faligndata(tmp4, tmp4);                             \
	    tmp5 =                                                       \
		vis_ld_u16_i((void *)(src_main + 1),                     \
		(i_dst##_2_rev) * 4);                                    \
	    di = vis_faligndata(tmp5, di);                               \
	    rli = MULD_SHL(shl6, vis_read_hi(di));                       \
	    tmp2 =                                                       \
		vis_ld_u16_i((void *)src_main, (i_dst##_1_rev) * 4);     \
	    dr = vis_faligndata(tmp2, tmp2);                             \
	    tmp3 =                                                       \
		vis_ld_u16_i((void *)src_main, (i_dst##_0_rev) * 4);     \
	    dr = vis_faligndata(tmp3, dr);                               \
	    rhr = MULD_SHL(shl6, vis_read_hi(dr));                       \
	    dr = vis_fpackfix_pair(rhr, rlr);                            \
	    tmp6 =                                                       \
		vis_ld_u16_i((void *)(src_main + 1),                     \
		(i_dst##_1_rev) * 4);                                    \
	    di = vis_faligndata(tmp6, tmp6);                             \
	    tmp7 =                                                       \
		vis_ld_u16_i((void *)(src_main + 1),                     \
		(i_dst##_0_rev) * 4);                                    \
	    di = vis_faligndata(tmp7, di);                               \
	    rhi = MULD_SHL(shl6, vis_read_hi(di));                       \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr;                         \
	    di = vis_fpackfix_pair(rhi, rli);                            \
	    *(mlib_d64 *)(dst##_i + i_dst) = di;                         \
	}

/* *********************************************************** */

#define	SC_LOAD_C_OPT(dst, src_main, src_aux, wnd, i_dst, fzeroVal)        \
	{                                                                  \
	    mlib_f32 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;       \
	    mlib_d64 r0, r1, r2, r3, dr0, di0, r4, r5, r6, r7, dr1, di1;   \
	                                                                   \
	    tmp0 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_0_rev        \
		* 4];                                                      \
	    tmp1 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_1_rev * 4];  \
	    tmp2 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_2_rev * 4];  \
	    tmp3 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_3_rev * 4];  \
	    tmp4 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_0_rev        \
		* 4 + 4];                                                  \
	    tmp5 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_1_rev        \
		* 4 + 4];                                                  \
	    tmp6 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_2_rev        \
		* 4 + 4];                                                  \
	    tmp7 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_3_rev        \
		* 4 + 4];                                                  \
	    r0 = MULD_SHL(shl6, tmp0);                                     \
	    r1 = MULD_SHL(shl6, tmp1);                                     \
	    r2 = MULD_SHL(shl6, tmp2);                                     \
	    r3 = MULD_SHL(shl6, tmp3);                                     \
	    r4 = MULD_SHL(shl6, tmp4);                                     \
	    r5 = MULD_SHL(shl6, tmp5);                                     \
	    r6 = MULD_SHL(shl6, tmp6);                                     \
	    r7 = MULD_SHL(shl6, tmp7);                                     \
	    dr0 = vis_fpackfix_pair(r0, r2);                               \
	    di0 = vis_fpackfix_pair(r1, r3);                               \
	    dr1 = vis_fpackfix_pair(r4, r6);                               \
	    di1 = vis_fpackfix_pair(r5, r7);                               \
	    r0 = vis_fpmerge(vis_read_hi(dr0), vis_read_lo(dr0));          \
	    r1 = vis_fpmerge(vis_read_hi(di0), vis_read_lo(di0));          \
	    r2 = vis_fpmerge(vis_read_hi(r0), vis_read_hi(r1));            \
	    r3 = vis_fpmerge(vis_read_lo(r0), vis_read_lo(r1));            \
	    dr0 = vis_fpmerge(vis_read_hi(r2), vis_read_lo(r2));           \
	    di0 = vis_fpmerge(vis_read_hi(r3), vis_read_lo(r3));           \
	    r4 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));          \
	    r5 = vis_fpmerge(vis_read_hi(di1), vis_read_lo(di1));          \
	    r6 = vis_fpmerge(vis_read_hi(r4), vis_read_hi(r5));            \
	    r7 = vis_fpmerge(vis_read_lo(r4), vis_read_lo(r5));            \
	    dr1 = vis_fpmerge(vis_read_hi(r6), vis_read_lo(r6));           \
	    di1 = vis_fpmerge(vis_read_hi(r7), vis_read_lo(r7));           \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr0;                          \
	    *(mlib_d64 *)(dst##_i + i_dst) = di0;                          \
	    *(mlib_d64 *)(dst##_r + i_dst + offset) = dr1;                 \
	    *(mlib_d64 *)(dst##_i + i_dst + offset) = di1;                 \
	}

/* *********************************************************** */

#define	SC_LOAD_C_OPT2(dst, src_main, src_aux, wnd, i_dst, fzeroVal)       \
	{                                                                  \
	    mlib_d64 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;       \
	    mlib_d64 r0, r1, r2, r3, dr0, di0, r4, r5, r6, r7, dr1, di1;   \
	    mlib_d64 ra, rb, rc, rd, re, rf, rg, rh, dra, drb, drc, drd;   \
	                                                                   \
	    tmp0 =                                                         \
		*(mlib_d64 *)&((mlib_u8 *)(src_main))[i_dst##_0_rev * 4];  \
	    tmp1 =                                                         \
		*(mlib_d64 *)&((mlib_u8 *)(src_main))[i_dst##_1_rev * 4];  \
	    tmp2 =                                                         \
		*(mlib_d64 *)&((mlib_u8 *)(src_main))[i_dst##_2_rev * 4];  \
	    tmp3 =                                                         \
		*(mlib_d64 *)&((mlib_u8 *)(src_main))[i_dst##_3_rev * 4];  \
	    r0 = MULD_SHL(shl6, vis_read_hi(tmp0));                        \
	    r1 = MULD_SHL(shl6, vis_read_hi(tmp1));                        \
	    r2 = MULD_SHL(shl6, vis_read_hi(tmp2));                        \
	    r3 = MULD_SHL(shl6, vis_read_hi(tmp3));                        \
	    r4 = MULD_SHL(shl6, vis_read_lo(tmp0));                        \
	    r5 = MULD_SHL(shl6, vis_read_lo(tmp1));                        \
	    r6 = MULD_SHL(shl6, vis_read_lo(tmp2));                        \
	    r7 = MULD_SHL(shl6, vis_read_lo(tmp3));                        \
	    tmp4 =                                                         \
		*(mlib_d64 *)&((mlib_u8 *)(src_main))[i_dst##_0_rev        \
		* 4 + 8];                                                  \
	    tmp5 =                                                         \
		*(mlib_d64 *)&((mlib_u8 *)(src_main))[i_dst##_1_rev        \
		* 4 + 8];                                                  \
	    tmp6 =                                                         \
		*(mlib_d64 *)&((mlib_u8 *)(src_main))[i_dst##_2_rev        \
		* 4 + 8];                                                  \
	    tmp7 =                                                         \
		*(mlib_d64 *)&((mlib_u8 *)(src_main))[i_dst##_3_rev        \
		* 4 + 8];                                                  \
	    dr0 = vis_fpackfix_pair(r0, r2);                               \
	    di0 = vis_fpackfix_pair(r1, r3);                               \
	    dr1 = vis_fpackfix_pair(r4, r6);                               \
	    di1 = vis_fpackfix_pair(r5, r7);                               \
	    r0 = vis_fpmerge(vis_read_hi(dr0), vis_read_lo(dr0));          \
	    r1 = vis_fpmerge(vis_read_hi(di0), vis_read_lo(di0));          \
	    r2 = vis_fpmerge(vis_read_hi(r0), vis_read_hi(r1));            \
	    r3 = vis_fpmerge(vis_read_lo(r0), vis_read_lo(r1));            \
	    dr0 = vis_fpmerge(vis_read_hi(r2), vis_read_lo(r2));           \
	    di0 = vis_fpmerge(vis_read_hi(r3), vis_read_lo(r3));           \
	    ra = MULD_SHL(shl6, vis_read_hi(tmp4));                        \
	    rb = MULD_SHL(shl6, vis_read_hi(tmp5));                        \
	    rc = MULD_SHL(shl6, vis_read_hi(tmp6));                        \
	    rd = MULD_SHL(shl6, vis_read_hi(tmp7));                        \
	    re = MULD_SHL(shl6, vis_read_lo(tmp4));                        \
	    rf = MULD_SHL(shl6, vis_read_lo(tmp5));                        \
	    rg = MULD_SHL(shl6, vis_read_lo(tmp6));                        \
	    rh = MULD_SHL(shl6, vis_read_lo(tmp7));                        \
	    r4 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));          \
	    r5 = vis_fpmerge(vis_read_hi(di1), vis_read_lo(di1));          \
	    r6 = vis_fpmerge(vis_read_hi(r4), vis_read_hi(r5));            \
	    r7 = vis_fpmerge(vis_read_lo(r4), vis_read_lo(r5));            \
	    dr1 = vis_fpmerge(vis_read_hi(r6), vis_read_lo(r6));           \
	    di1 = vis_fpmerge(vis_read_hi(r7), vis_read_lo(r7));           \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr0;                          \
	    *(mlib_d64 *)(dst##_i + i_dst) = di0;                          \
	    *(mlib_d64 *)(dst##_r + i_dst + offset0) = dr1;                \
	    *(mlib_d64 *)(dst##_i + i_dst + offset0) = di1;                \
	    dr0 = vis_fpackfix_pair(ra, rc);                               \
	    di0 = vis_fpackfix_pair(rb, rd);                               \
	    dr1 = vis_fpackfix_pair(re, rg);                               \
	    di1 = vis_fpackfix_pair(rf, rh);                               \
	    r0 = vis_fpmerge(vis_read_hi(dr0), vis_read_lo(dr0));          \
	    r1 = vis_fpmerge(vis_read_hi(di0), vis_read_lo(di0));          \
	    r2 = vis_fpmerge(vis_read_hi(r0), vis_read_hi(r1));            \
	    r3 = vis_fpmerge(vis_read_lo(r0), vis_read_lo(r1));            \
	    dr0 = vis_fpmerge(vis_read_hi(r2), vis_read_lo(r2));           \
	    di0 = vis_fpmerge(vis_read_hi(r3), vis_read_lo(r3));           \
	    r4 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));          \
	    r5 = vis_fpmerge(vis_read_hi(di1), vis_read_lo(di1));          \
	    r6 = vis_fpmerge(vis_read_hi(r4), vis_read_hi(r5));            \
	    r7 = vis_fpmerge(vis_read_lo(r4), vis_read_lo(r5));            \
	    dr1 = vis_fpmerge(vis_read_hi(r6), vis_read_lo(r6));           \
	    di1 = vis_fpmerge(vis_read_hi(r7), vis_read_lo(r7));           \
	    *(mlib_d64 *)(dst##_r + i_dst + offset1) = dr0;                \
	    *(mlib_d64 *)(dst##_i + i_dst + offset1) = di0;                \
	    *(mlib_d64 *)(dst##_r + i_dst + offset2) = dr1;                \
	    *(mlib_d64 *)(dst##_i + i_dst + offset2) = di1;                \
	}

/* *********************************************************** */

#define	SC_LOAD_RI(dst, src_main, src_aux, wnd, i_dst, fzeroVal)         \
	{                                                                \
	    mlib_d64 dr, di, tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6,   \
		tmp7, rhr, rlr, rhi, rli;                                \
	    tmp0 =                                                       \
		vis_ld_u16_i((void *)src_main, (i_dst##_3_rev) * 2);     \
	    dr = vis_faligndata(tmp0, tmp0);                             \
	    tmp1 =                                                       \
		vis_ld_u16_i((void *)src_main, (i_dst##_2_rev) * 2);     \
	    dr = vis_faligndata(tmp1, dr);                               \
	    rlr = MULD_SHL(shl6, vis_read_hi(dr));                       \
	    tmp4 =                                                       \
		vis_ld_u16_i((void *)(src_aux),                          \
		(i_dst##_3_rev) * 2);                                    \
	    di = vis_faligndata(tmp4, tmp4);                             \
	    tmp5 =                                                       \
		vis_ld_u16_i((void *)(src_aux),                          \
		(i_dst##_2_rev) * 2);                                    \
	    di = vis_faligndata(tmp5, di);                               \
	    rli = MULD_SHL(shl6, vis_read_hi(di));                       \
	    tmp2 =                                                       \
		vis_ld_u16_i((void *)src_main, (i_dst##_1_rev) * 2);     \
	    dr = vis_faligndata(tmp2, tmp2);                             \
	    tmp3 =                                                       \
		vis_ld_u16_i((void *)src_main, (i_dst##_0_rev) * 2);     \
	    dr = vis_faligndata(tmp3, dr);                               \
	    rhr = MULD_SHL(shl6, vis_read_hi(dr));                       \
	    dr = vis_fpackfix_pair(rhr, rlr);                            \
	    tmp6 =                                                       \
		vis_ld_u16_i((void *)(src_aux),                          \
		(i_dst##_1_rev) * 2);                                    \
	    di = vis_faligndata(tmp6, tmp6);                             \
	    tmp7 =                                                       \
		vis_ld_u16_i((void *)(src_aux),                          \
		(i_dst##_0_rev) * 2);                                    \
	    di = vis_faligndata(tmp7, di);                               \
	    rhi = MULD_SHL(shl6, vis_read_hi(di));                       \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr;                         \
	    di = vis_fpackfix_pair(rhi, rli);                            \
	    *(mlib_d64 *)(dst##_i + i_dst) = di;                         \
	}

/* *********************************************************** */

#define	SC_LOAD_RI_OPT(dst, src_main, src_aux, wnd, i_dst, fzeroVal)       \
	{                                                                  \
	    mlib_f32 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;       \
	    mlib_d64 r0, r1, r2, r3, r4, r5, r6, r7, dr0, dr1, di0, di1;   \
	                                                                   \
	    tmp0 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_0_rev * 2];  \
	    tmp1 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_1_rev * 2];  \
	    tmp2 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_2_rev * 2];  \
	    tmp3 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_3_rev * 2];  \
	    tmp4 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_aux))[i_dst##_0_rev * 2];   \
	    tmp5 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_aux))[i_dst##_1_rev * 2];   \
	    tmp6 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_aux))[i_dst##_2_rev * 2];   \
	    tmp7 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_aux))[i_dst##_3_rev * 2];   \
	    r0 = MULD_SHL(shl6, tmp0);                                     \
	    r1 = MULD_SHL(shl6, tmp1);                                     \
	    r2 = MULD_SHL(shl6, tmp2);                                     \
	    r3 = MULD_SHL(shl6, tmp3);                                     \
	    r4 = MULD_SHL(shl6, tmp4);                                     \
	    r5 = MULD_SHL(shl6, tmp5);                                     \
	    r6 = MULD_SHL(shl6, tmp6);                                     \
	    r7 = MULD_SHL(shl6, tmp7);                                     \
	    dr0 = vis_fpackfix_pair(r0, r2);                               \
	    dr1 = vis_fpackfix_pair(r1, r3);                               \
	    di0 = vis_fpackfix_pair(r4, r6);                               \
	    di1 = vis_fpackfix_pair(r5, r7);                               \
	    r0 = vis_fpmerge(vis_read_hi(dr0), vis_read_lo(dr0));          \
	    r1 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));          \
	    r2 = vis_fpmerge(vis_read_hi(r0), vis_read_hi(r1));            \
	    r3 = vis_fpmerge(vis_read_lo(r0), vis_read_lo(r1));            \
	    dr0 = vis_fpmerge(vis_read_hi(r2), vis_read_lo(r2));           \
	    dr1 = vis_fpmerge(vis_read_hi(r3), vis_read_lo(r3));           \
	    r4 = vis_fpmerge(vis_read_hi(di0), vis_read_lo(di0));          \
	    r5 = vis_fpmerge(vis_read_hi(di1), vis_read_lo(di1));          \
	    r6 = vis_fpmerge(vis_read_hi(r4), vis_read_hi(r5));            \
	    r7 = vis_fpmerge(vis_read_lo(r4), vis_read_lo(r5));            \
	    di0 = vis_fpmerge(vis_read_hi(r6), vis_read_lo(r6));           \
	    di1 = vis_fpmerge(vis_read_hi(r7), vis_read_lo(r7));           \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr0;                          \
	    *(mlib_d64 *)(dst##_i + i_dst) = di0;                          \
	    *(mlib_d64 *)(dst##_r + i_dst + offset) = dr1;                 \
	    *(mlib_d64 *)(dst##_i + i_dst + offset) = di1;                 \
	}

/* *********************************************************** */

#define	SC_LOAD_R(dst, src_main, src_aux, wnd, i_dst, fzeroVal)        \
	{                                                              \
	    mlib_d64 dr, di, tmp0, tmp1, tmp2, tmp3, rhr, rlr;         \
	                                                               \
	    tmp0 =                                                     \
		vis_ld_u16_i((void *)src_main, (i_dst##_3_rev) * 2);   \
	    dr = vis_faligndata(tmp0, tmp0);                           \
	    tmp1 =                                                     \
		vis_ld_u16_i((void *)src_main, (i_dst##_2_rev) * 2);   \
	    dr = vis_faligndata(tmp1, dr);                             \
	    rlr = MULD_SHL(shl6, vis_read_hi(dr));                     \
	    tmp2 =                                                     \
		vis_ld_u16_i((void *)src_main, (i_dst##_1_rev) * 2);   \
	    di = vis_faligndata(tmp2, tmp2);                           \
	    tmp3 =                                                     \
		vis_ld_u16_i((void *)src_main, (i_dst##_0_rev) * 2);   \
	    di = vis_faligndata(tmp3, di);                             \
	    rhr = MULD_SHL(shl6, vis_read_hi(di));                     \
	    *(mlib_d64 *)(dst##_i + i_dst) = fzeroVal;                 \
	    dr = vis_fpackfix_pair(rhr, rlr);                          \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr;                       \
	}

/* *********************************************************** */

#define	SC_LOAD_R_OPT(dst, src_main, src_aux, wnd, i_dst, fzeroVal)   \
	{                                                             \
	    mlib_f32 tmp0, tmp1, tmp2, tmp3;                          \
	    mlib_d64 dr0, dr1, r0, r1, r2, r3;                        \
	                                                              \
	    tmp0 =                                                    \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_0_rev   \
		* 2];                                                 \
	    tmp1 =                                                    \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_1_rev   \
		* 2];                                                 \
	    tmp2 =                                                    \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_2_rev   \
		* 2];                                                 \
	    tmp3 =                                                    \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_3_rev   \
		* 2];                                                 \
	    r0 = MULD_SHL(shl6, tmp0);                                \
	    r1 = MULD_SHL(shl6, tmp1);                                \
	    r2 = MULD_SHL(shl6, tmp2);                                \
	    r3 = MULD_SHL(shl6, tmp3);                                \
	    dr0 = vis_fpackfix_pair(r0, r2);                          \
	    dr1 = vis_fpackfix_pair(r1, r3);                          \
	    r0 = vis_fpmerge(vis_read_hi(dr0), vis_read_lo(dr0));     \
	    r1 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));     \
	    r2 = vis_fpmerge(vis_read_hi(r0), vis_read_hi(r1));       \
	    r3 = vis_fpmerge(vis_read_lo(r0), vis_read_lo(r1));       \
	    dr0 = vis_fpmerge(vis_read_hi(r2), vis_read_lo(r2));      \
	    dr1 = vis_fpmerge(vis_read_hi(r3), vis_read_lo(r3));      \
	    *(mlib_d64 *)(dst##_i + i_dst) = fzeroVal;                \
	    *(mlib_d64 *)(dst##_i + i_dst + offset) = fzeroVal;       \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr0;                     \
	    *(mlib_d64 *)(dst##_r + i_dst + offset) = dr1;            \
	}

/* *********************************************************** */

#define	SC_LOAD_R_OPT2(dst, src_main, src_aux, wnd, i_dst, fzeroVal)  \
	{                                                             \
	    mlib_d64 dr0, dr1, dr2, dr3;                              \
	    mlib_d64 r0, r1, r2, r3, r4, r5, r6, r7;                  \
	    mlib_d64 tmp0, tmp1, tmp2, tmp3;                          \
	                                                              \
	    tmp0 =                                                    \
		*(mlib_d64 *)&((mlib_u8 *)(src_main))[i_dst##_0_rev   \
		* 2];                                                 \
	    tmp1 =                                                    \
		*(mlib_d64 *)&((mlib_u8 *)(src_main))[i_dst##_1_rev   \
		* 2];                                                 \
	    tmp2 =                                                    \
		*(mlib_d64 *)&((mlib_u8 *)(src_main))[i_dst##_2_rev   \
		* 2];                                                 \
	    tmp3 =                                                    \
		*(mlib_d64 *)&((mlib_u8 *)(src_main))[i_dst##_3_rev   \
		* 2];                                                 \
	    r0 = MULD_SHL(shl6, vis_read_hi(tmp0));                   \
	    r1 = MULD_SHL(shl6, vis_read_lo(tmp0));                   \
	    r2 = MULD_SHL(shl6, vis_read_hi(tmp1));                   \
	    r3 = MULD_SHL(shl6, vis_read_lo(tmp1));                   \
	    r4 = MULD_SHL(shl6, vis_read_hi(tmp2));                   \
	    r5 = MULD_SHL(shl6, vis_read_lo(tmp2));                   \
	    r6 = MULD_SHL(shl6, vis_read_hi(tmp3));                   \
	    r7 = MULD_SHL(shl6, vis_read_lo(tmp3));                   \
	    tmp0 = vis_fpackfix_pair(r0, r1);                         \
	    tmp1 = vis_fpackfix_pair(r2, r3);                         \
	    tmp2 = vis_fpackfix_pair(r4, r5);                         \
	    tmp3 = vis_fpackfix_pair(r6, r7);                         \
	    r0 = vis_fpmerge(vis_read_hi(tmp0), vis_read_hi(tmp2));   \
	    r1 = vis_fpmerge(vis_read_hi(tmp1), vis_read_hi(tmp3));   \
	    r2 = vis_fpmerge(vis_read_hi(r0), vis_read_hi(r1));       \
	    dr0 = vis_fpmerge(vis_read_hi(r2), vis_read_lo(r2));      \
	    r3 = vis_fpmerge(vis_read_lo(r0), vis_read_lo(r1));       \
	    dr1 = vis_fpmerge(vis_read_hi(r3), vis_read_lo(r3));      \
	    r0 = vis_fpmerge(vis_read_lo(tmp0), vis_read_lo(tmp2));   \
	    r1 = vis_fpmerge(vis_read_lo(tmp1), vis_read_lo(tmp3));   \
	    r2 = vis_fpmerge(vis_read_hi(r0), vis_read_hi(r1));       \
	    dr2 = vis_fpmerge(vis_read_hi(r2), vis_read_lo(r2));      \
	    r3 = vis_fpmerge(vis_read_lo(r0), vis_read_lo(r1));       \
	    dr3 = vis_fpmerge(vis_read_hi(r3), vis_read_lo(r3));      \
	    *(mlib_d64 *)(dst##_i + i_dst) = fzeroVal;                \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr0;                     \
	    *(mlib_d64 *)(dst##_i + i_dst + offset0) = fzeroVal;      \
	    *(mlib_d64 *)(dst##_r + i_dst + offset0) = dr1;           \
	    *(mlib_d64 *)(dst##_i + i_dst + offset1) = fzeroVal;      \
	    *(mlib_d64 *)(dst##_r + i_dst + offset1) = dr2;           \
	    *(mlib_d64 *)(dst##_i + i_dst + offset2) = fzeroVal;      \
	    *(mlib_d64 *)(dst##_r + i_dst + offset2) = dr3;           \
	}

/* *********************************************************** */

/* load data into deal buffers for various sources */
#define	NS_LOAD_C_W(dst, src_main, src_aux, wnd, i_dst, fzeroVal)       \
	{                                                               \
	    mlib_d64 win, dr, di, tmp0, tmp1, tmp2, tmp3, tmp4, tmp5,   \
		tmp6, tmp7, tmp8, tmp9, tmpa, tmpb;                     \
	    tmp4 = vis_ld_u16_i((void *)wnd, (i_dst##_3_rev) * 2);      \
	    win = vis_faligndata(tmp4, win);                            \
	    tmp5 = vis_ld_u16_i((void *)wnd, (i_dst##_2_rev) * 2);      \
	    win = vis_faligndata(tmp5, win);                            \
	    tmp6 = vis_ld_u16_i((void *)wnd, (i_dst##_1_rev) * 2);      \
	    win = vis_faligndata(tmp6, win);                            \
	    tmp7 = vis_ld_u16_i((void *)wnd, (i_dst##_0_rev) * 2);      \
	    win = vis_faligndata(tmp7, win);                            \
	    tmp0 =                                                      \
		vis_ld_u16_i((void *)src_main, (i_dst##_3_rev) * 4);    \
	    dr = vis_faligndata(tmp0, dr);                              \
	    tmp1 =                                                      \
		vis_ld_u16_i((void *)src_main, (i_dst##_2_rev) * 4);    \
	    dr = vis_faligndata(tmp1, dr);                              \
	    tmp2 =                                                      \
		vis_ld_u16_i((void *)src_main, (i_dst##_1_rev) * 4);    \
	    dr = vis_faligndata(tmp2, dr);                              \
	    tmp3 =                                                      \
		vis_ld_u16_i((void *)src_main, (i_dst##_0_rev) * 4);    \
	    dr = vis_faligndata(tmp3, dr);                              \
	    MULT_WIN(dr, win);                                          \
	    tmp8 =                                                      \
		vis_ld_u16_i((void *)(src_main + 1),                    \
		(i_dst##_3_rev) * 4);                                   \
	    di = vis_faligndata(tmp8, di);                              \
	    tmp9 =                                                      \
		vis_ld_u16_i((void *)(src_main + 1),                    \
		(i_dst##_2_rev) * 4);                                   \
	    di = vis_faligndata(tmp9, di);                              \
	    tmpa =                                                      \
		vis_ld_u16_i((void *)(src_main + 1),                    \
		(i_dst##_1_rev) * 4);                                   \
	    di = vis_faligndata(tmpa, di);                              \
	    tmpb =                                                      \
		vis_ld_u16_i((void *)(src_main + 1),                    \
		(i_dst##_0_rev) * 4);                                   \
	    di = vis_faligndata(tmpb, di);                              \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr;                        \
	    MULT_WIN(di, win);                                          \
	    *(mlib_d64 *)(dst##_i + i_dst) = di;                        \
	}

/* *********************************************************** */

#define	NS_LOAD_RI_W(dst, src_main, src_aux, wnd, i_dst, fzeroVal)     \
	{                                                              \
	    mlib_d64 win, dr, di, tmp;                                 \
	                                                               \
	    tmp = vis_ld_u16_i((void *)wnd, (i_dst##_3_rev) * 2);      \
	    win = vis_faligndata(tmp, win);                            \
	    tmp = vis_ld_u16_i((void *)wnd, (i_dst##_2_rev) * 2);      \
	    win = vis_faligndata(tmp, win);                            \
	    tmp = vis_ld_u16_i((void *)wnd, (i_dst##_1_rev) * 2);      \
	    win = vis_faligndata(tmp, win);                            \
	    tmp = vis_ld_u16_i((void *)wnd, (i_dst##_0_rev) * 2);      \
	    win = vis_faligndata(tmp, win);                            \
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
	    dr = vis_faligndata(tmp, dr);                              \
	    MULT_WIN(dr, win);                                         \
	    tmp =                                                      \
		vis_ld_u16_i((void *)src_aux, (i_dst##_3_rev) * 2);    \
	    di = vis_faligndata(tmp, di);                              \
	    tmp =                                                      \
		vis_ld_u16_i((void *)src_aux, (i_dst##_2_rev) * 2);    \
	    di = vis_faligndata(tmp, di);                              \
	    tmp =                                                      \
		vis_ld_u16_i((void *)src_aux, (i_dst##_1_rev) * 2);    \
	    di = vis_faligndata(tmp, di);                              \
	    tmp =                                                      \
		vis_ld_u16_i((void *)src_aux, (i_dst##_0_rev) * 2);    \
	    di = vis_faligndata(tmp, di);                              \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr;                       \
	    MULT_WIN(di, win);                                         \
	    *(mlib_d64 *)(dst##_i + i_dst) = di;                       \
	}

/* *********************************************************** */

#define	NS_LOAD_R_W(dst, src_main, src_aux, wnd, i_dst, fzeroVal)         \
	{                                                                 \
	    mlib_d64 win, dr, tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6,   \
		tmp7;                                                     \
	    tmp0 =                                                        \
		vis_ld_u16_i((void *)src_main, (i_dst##_3_rev) * 2);      \
	    dr = vis_faligndata(tmp0, dr);                                \
	    tmp1 =                                                        \
		vis_ld_u16_i((void *)src_main, (i_dst##_2_rev) * 2);      \
	    dr = vis_faligndata(tmp1, dr);                                \
	    tmp2 =                                                        \
		vis_ld_u16_i((void *)src_main, (i_dst##_1_rev) * 2);      \
	    dr = vis_faligndata(tmp2, dr);                                \
	    tmp3 =                                                        \
		vis_ld_u16_i((void *)src_main, (i_dst##_0_rev) * 2);      \
	    dr = vis_faligndata(tmp3, dr);                                \
	    tmp4 = vis_ld_u16_i((void *)wnd, (i_dst##_3_rev) * 2);        \
	    win = vis_faligndata(tmp4, win);                              \
	    tmp5 = vis_ld_u16_i((void *)wnd, (i_dst##_2_rev) * 2);        \
	    win = vis_faligndata(tmp5, win);                              \
	    tmp6 = vis_ld_u16_i((void *)wnd, (i_dst##_1_rev) * 2);        \
	    win = vis_faligndata(tmp6, win);                              \
	    tmp7 = vis_ld_u16_i((void *)wnd, (i_dst##_0_rev) * 2);        \
	    win = vis_faligndata(tmp7, win);                              \
	    *(mlib_d64 *)(dst##_i + i_dst) = fzeroVal;                    \
	    MULT_WIN(dr, win);                                            \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr;                          \
	}

/* *********************************************************** */

#define	NS_LOAD_C(dst, src_main, src_aux, wnd, i_dst, fzeroVal)        \
	{                                                              \
	    mlib_d64 dr, di, tmp;                                      \
	                                                               \
	    tmp =                                                      \
		vis_ld_u16_i((void *)src_main, (i_dst##_3_rev) * 4);   \
	    dr = vis_faligndata(tmp, dr);                              \
	    tmp =                                                      \
		vis_ld_u16_i((void *)src_main, (i_dst##_2_rev) * 4);   \
	    dr = vis_faligndata(tmp, dr);                              \
	    tmp =                                                      \
		vis_ld_u16_i((void *)src_main, (i_dst##_1_rev) * 4);   \
	    dr = vis_faligndata(tmp, dr);                              \
	    tmp =                                                      \
		vis_ld_u16_i((void *)src_main, (i_dst##_0_rev) * 4);   \
	    dr = vis_faligndata(tmp, dr);                              \
	    tmp =                                                      \
		vis_ld_u16_i((void *)(src_main + 1),                   \
		(i_dst##_3_rev) * 4);                                  \
	    di = vis_faligndata(tmp, di);                              \
	    tmp =                                                      \
		vis_ld_u16_i((void *)(src_main + 1),                   \
		(i_dst##_2_rev) * 4);                                  \
	    di = vis_faligndata(tmp, di);                              \
	    tmp =                                                      \
		vis_ld_u16_i((void *)(src_main + 1),                   \
		(i_dst##_1_rev) * 4);                                  \
	    di = vis_faligndata(tmp, di);                              \
	    tmp =                                                      \
		vis_ld_u16_i((void *)(src_main + 1),                   \
		(i_dst##_0_rev) * 4);                                  \
	    di = vis_faligndata(tmp, di);                              \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr;                       \
	    *(mlib_d64 *)(dst##_i + i_dst) = di;                       \
	}

/* *********************************************************** */

#define	NS_LOAD_C_OPT(dst, src_main, src_aux, wnd, i_dst, fzeroVal)        \
	{                                                                  \
	    mlib_f32 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;       \
	    mlib_d64 r0, r1, r2, r3, dr0, di0, r4, r5, r6, r7, dr1, di1;   \
	                                                                   \
	    tmp0 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_0_rev * 4];  \
	    tmp1 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_1_rev * 4];  \
	    tmp2 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_2_rev * 4];  \
	    tmp3 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_3_rev * 4];  \
	    tmp4 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_0_rev        \
		* 4 + 4];                                                  \
	    tmp5 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_1_rev        \
		* 4 + 4];                                                  \
	    tmp6 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_2_rev        \
		* 4 + 4];                                                  \
	    tmp7 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_3_rev        \
		* 4 + 4];                                                  \
	    r0 = vis_fpmerge(tmp0, tmp2);                                  \
	    r1 = vis_fpmerge(tmp1, tmp3);                                  \
	    r2 = vis_fpmerge(vis_read_hi(r0), vis_read_hi(r1));            \
	    r3 = vis_fpmerge(vis_read_lo(r0), vis_read_lo(r1));            \
	    dr0 = vis_fpmerge(vis_read_hi(r2), vis_read_lo(r2));           \
	    di0 = vis_fpmerge(vis_read_hi(r3), vis_read_lo(r3));           \
	    r4 = vis_fpmerge(tmp4, tmp6);                                  \
	    r5 = vis_fpmerge(tmp5, tmp7);                                  \
	    r6 = vis_fpmerge(vis_read_hi(r4), vis_read_hi(r5));            \
	    r7 = vis_fpmerge(vis_read_lo(r4), vis_read_lo(r5));            \
	    dr1 = vis_fpmerge(vis_read_hi(r6), vis_read_lo(r6));           \
	    di1 = vis_fpmerge(vis_read_hi(r7), vis_read_lo(r7));           \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr0;                          \
	    *(mlib_d64 *)(dst##_i + i_dst) = di0;                          \
	    *(mlib_d64 *)(dst##_r + i_dst + offset) = dr1;                 \
	    *(mlib_d64 *)(dst##_i + i_dst + offset) = di1;                 \
	}

/* *********************************************************** */

#define	NS_LOAD_C_OPT2(dst, src_main, src_aux, wnd, i_dst, fzeroVal)       \
	{                                                                  \
	    mlib_d64 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;       \
	    mlib_d64 r0, r1, r2, r3, dr0, di0, r4, r5, r6, r7, dr1, di1;   \
	                                                                   \
	    tmp0 =                                                         \
		*(mlib_d64 *)&((mlib_u8 *)(src_main))[i_dst##_0_rev * 4];  \
	    tmp1 =                                                         \
		*(mlib_d64 *)&((mlib_u8 *)(src_main))[i_dst##_1_rev * 4];  \
	    tmp2 =                                                         \
		*(mlib_d64 *)&((mlib_u8 *)(src_main))[i_dst##_2_rev * 4];  \
	    tmp3 =                                                         \
		*(mlib_d64 *)&((mlib_u8 *)(src_main))[i_dst##_3_rev * 4];  \
	    tmp4 =                                                         \
		*(mlib_d64 *)&((mlib_u8 *)(src_main))[i_dst##_0_rev        \
		* 4 + 8];                                                  \
	    tmp5 =                                                         \
		*(mlib_d64 *)&((mlib_u8 *)(src_main))[i_dst##_1_rev        \
		* 4 + 8];                                                  \
	    tmp6 =                                                         \
		*(mlib_d64 *)&((mlib_u8 *)(src_main))[i_dst##_2_rev        \
		* 4 + 8];                                                  \
	    tmp7 =                                                         \
		*(mlib_d64 *)&((mlib_u8 *)(src_main))[i_dst##_3_rev        \
		* 4 + 8];                                                  \
	    r0 = vis_fpmerge(vis_read_hi(tmp0), vis_read_hi(tmp2));        \
	    r1 = vis_fpmerge(vis_read_hi(tmp1), vis_read_hi(tmp3));        \
	    r2 = vis_fpmerge(vis_read_hi(r0), vis_read_hi(r1));            \
	    r3 = vis_fpmerge(vis_read_lo(r0), vis_read_lo(r1));            \
	    dr0 = vis_fpmerge(vis_read_hi(r2), vis_read_lo(r2));           \
	    di0 = vis_fpmerge(vis_read_hi(r3), vis_read_lo(r3));           \
	    r4 = vis_fpmerge(vis_read_lo(tmp0), vis_read_lo(tmp2));        \
	    r5 = vis_fpmerge(vis_read_lo(tmp1), vis_read_lo(tmp3));        \
	    r6 = vis_fpmerge(vis_read_hi(r4), vis_read_hi(r5));            \
	    r7 = vis_fpmerge(vis_read_lo(r4), vis_read_lo(r5));            \
	    dr1 = vis_fpmerge(vis_read_hi(r6), vis_read_lo(r6));           \
	    di1 = vis_fpmerge(vis_read_hi(r7), vis_read_lo(r7));           \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr0;                          \
	    *(mlib_d64 *)(dst##_i + i_dst) = di0;                          \
	    *(mlib_d64 *)(dst##_r + i_dst + offset0) = dr1;                \
	    *(mlib_d64 *)(dst##_i + i_dst + offset0) = di1;                \
	    r0 = vis_fpmerge(vis_read_hi(tmp4), vis_read_hi(tmp6));        \
	    r1 = vis_fpmerge(vis_read_hi(tmp5), vis_read_hi(tmp7));        \
	    r2 = vis_fpmerge(vis_read_hi(r0), vis_read_hi(r1));            \
	    r3 = vis_fpmerge(vis_read_lo(r0), vis_read_lo(r1));            \
	    dr0 = vis_fpmerge(vis_read_hi(r2), vis_read_lo(r2));           \
	    di0 = vis_fpmerge(vis_read_hi(r3), vis_read_lo(r3));           \
	    r4 = vis_fpmerge(vis_read_lo(tmp4), vis_read_lo(tmp6));        \
	    r5 = vis_fpmerge(vis_read_lo(tmp5), vis_read_lo(tmp7));        \
	    r6 = vis_fpmerge(vis_read_hi(r4), vis_read_hi(r5));            \
	    r7 = vis_fpmerge(vis_read_lo(r4), vis_read_lo(r5));            \
	    dr1 = vis_fpmerge(vis_read_hi(r6), vis_read_lo(r6));           \
	    di1 = vis_fpmerge(vis_read_hi(r7), vis_read_lo(r7));           \
	    *(mlib_d64 *)(dst##_r + i_dst + offset1) = dr0;                \
	    *(mlib_d64 *)(dst##_i + i_dst + offset1) = di0;                \
	    *(mlib_d64 *)(dst##_r + i_dst + offset2) = dr1;                \
	    *(mlib_d64 *)(dst##_i + i_dst + offset2) = di1;                \
	}

/* *********************************************************** */

#define	NS_LOAD_RI(dst, src_main, src_aux, wnd, i_dst, fzeroVal)       \
	{                                                              \
	    mlib_d64 dr, di, tmp;                                      \
	                                                               \
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
	    dr = vis_faligndata(tmp, dr);                              \
	    tmp =                                                      \
		vis_ld_u16_i((void *)src_aux, (i_dst##_3_rev) * 2);    \
	    di = vis_faligndata(tmp, di);                              \
	    tmp =                                                      \
		vis_ld_u16_i((void *)src_aux, (i_dst##_2_rev) * 2);    \
	    di = vis_faligndata(tmp, di);                              \
	    tmp =                                                      \
		vis_ld_u16_i((void *)src_aux, (i_dst##_1_rev) * 2);    \
	    di = vis_faligndata(tmp, di);                              \
	    tmp =                                                      \
		vis_ld_u16_i((void *)src_aux, (i_dst##_0_rev) * 2);    \
	    di = vis_faligndata(tmp, di);                              \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr;                       \
	    *(mlib_d64 *)(dst##_i + i_dst) = di;                       \
	}

/* *********************************************************** */

#define	NS_LOAD_RI_OPT(dst, src_main, src_aux, wnd, i_dst, fzeroVal)       \
	{                                                                  \
	    mlib_f32 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;       \
	    mlib_d64 r0, r1, r2, r3, r4, r5, r6, r7, dr0, dr1, di0, di1;   \
	                                                                   \
	    tmp0 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_0_rev * 2];  \
	    tmp1 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_1_rev * 2];  \
	    tmp2 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_2_rev * 2];  \
	    tmp3 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_3_rev * 2];  \
	    tmp4 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_aux))[i_dst##_0_rev * 2];   \
	    tmp5 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_aux))[i_dst##_1_rev * 2];   \
	    tmp6 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_aux))[i_dst##_2_rev * 2];   \
	    tmp7 =                                                         \
		*(mlib_f32 *)&((mlib_u8 *)(src_aux))[i_dst##_3_rev * 2];   \
	    r0 = vis_fpmerge(tmp0, tmp2);                                  \
	    r1 = vis_fpmerge(tmp1, tmp3);                                  \
	    r2 = vis_fpmerge(vis_read_hi(r0), vis_read_hi(r1));            \
	    r3 = vis_fpmerge(vis_read_lo(r0), vis_read_lo(r1));            \
	    dr0 = vis_fpmerge(vis_read_hi(r2), vis_read_lo(r2));           \
	    dr1 = vis_fpmerge(vis_read_hi(r3), vis_read_lo(r3));           \
	    r4 = vis_fpmerge(tmp4, tmp6);                                  \
	    r5 = vis_fpmerge(tmp5, tmp7);                                  \
	    r6 = vis_fpmerge(vis_read_hi(r4), vis_read_hi(r5));            \
	    r7 = vis_fpmerge(vis_read_lo(r4), vis_read_lo(r5));            \
	    di0 = vis_fpmerge(vis_read_hi(r6), vis_read_lo(r6));           \
	    di1 = vis_fpmerge(vis_read_hi(r7), vis_read_lo(r7));           \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr0;                          \
	    *(mlib_d64 *)(dst##_i + i_dst) = di0;                          \
	    *(mlib_d64 *)(dst##_r + i_dst + offset) = dr1;                 \
	    *(mlib_d64 *)(dst##_i + i_dst + offset) = di1;                 \
	}

/* *********************************************************** */

#define	NS_LOAD_R(dst, src_main, src_aux, wnd, i_dst, fzeroVal)        \
	{                                                              \
	    mlib_d64 dr, tmp;                                          \
	                                                               \
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
	    dr = vis_faligndata(tmp, dr);                              \
	    *(mlib_d64 *)(dst##_i + i_dst) = fzeroVal;                 \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr;                       \
	}

/* *********************************************************** */

#define	NS_LOAD_R_OPT(dst, src_main, src_aux, wnd, i_dst, fzeroVal)   \
	{                                                             \
	    mlib_f32 tmp0, tmp1, tmp2, tmp3;                          \
	    mlib_d64 dr0, dr1, r0, r1, r2, r3;                        \
	                                                              \
	    tmp0 =                                                    \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_0_rev   \
		* 2];                                                 \
	    tmp1 =                                                    \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_1_rev   \
		* 2];                                                 \
	    tmp2 =                                                    \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_2_rev   \
		* 2];                                                 \
	    tmp3 =                                                    \
		*(mlib_f32 *)&((mlib_u8 *)(src_main))[i_dst##_3_rev   \
		* 2];                                                 \
	    r0 = vis_fpmerge(tmp0, tmp2);                             \
	    r1 = vis_fpmerge(tmp1, tmp3);                             \
	    r2 = vis_fpmerge(vis_read_hi(r0), vis_read_hi(r1));       \
	    r3 = vis_fpmerge(vis_read_lo(r0), vis_read_lo(r1));       \
	    dr0 = vis_fpmerge(vis_read_hi(r2), vis_read_lo(r2));      \
	    dr1 = vis_fpmerge(vis_read_hi(r3), vis_read_lo(r3));      \
	    *(mlib_d64 *)(dst##_i + i_dst) = fzeroVal;                \
	    *(mlib_d64 *)(dst##_i + i_dst + offset) = fzeroVal;       \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr0;                     \
	    *(mlib_d64 *)(dst##_r + i_dst + offset) = dr1;            \
	}

/* *********************************************************** */

#define	NS_LOAD_R_OPT2(dst, src_main, src_aux, wnd, i_dst, fzeroVal)  \
	{                                                             \
	    mlib_d64 dr0, dr1, dr2, dr3;                              \
	    mlib_d64 r0, r1, r2, r3, r4, r5, r6, r7;                  \
	    mlib_d64 tmp0, tmp1, tmp2, tmp3;                          \
	                                                              \
	    tmp0 =                                                    \
		*(mlib_d64 *)&((mlib_u8 *)(src_main))[i_dst##_0_rev   \
		* 2];                                                 \
	    tmp1 =                                                    \
		*(mlib_d64 *)&((mlib_u8 *)(src_main))[i_dst##_1_rev   \
		* 2];                                                 \
	    tmp2 =                                                    \
		*(mlib_d64 *)&((mlib_u8 *)(src_main))[i_dst##_2_rev   \
		* 2];                                                 \
	    tmp3 =                                                    \
		*(mlib_d64 *)&((mlib_u8 *)(src_main))[i_dst##_3_rev   \
		* 2];                                                 \
	    r0 = vis_fpmerge(vis_read_hi(tmp0), vis_read_hi(tmp2));   \
	    r1 = vis_fpmerge(vis_read_hi(tmp1), vis_read_hi(tmp3));   \
	    r2 = vis_fpmerge(vis_read_hi(r0), vis_read_hi(r1));       \
	    dr0 = vis_fpmerge(vis_read_hi(r2), vis_read_lo(r2));      \
	    r3 = vis_fpmerge(vis_read_lo(r0), vis_read_lo(r1));       \
	    dr1 = vis_fpmerge(vis_read_hi(r3), vis_read_lo(r3));      \
	    r0 = vis_fpmerge(vis_read_lo(tmp0), vis_read_lo(tmp2));   \
	    r1 = vis_fpmerge(vis_read_lo(tmp1), vis_read_lo(tmp3));   \
	    r2 = vis_fpmerge(vis_read_hi(r0), vis_read_hi(r1));       \
	    dr2 = vis_fpmerge(vis_read_hi(r2), vis_read_lo(r2));      \
	    r3 = vis_fpmerge(vis_read_lo(r0), vis_read_lo(r1));       \
	    dr3 = vis_fpmerge(vis_read_hi(r3), vis_read_lo(r3));      \
	    *(mlib_d64 *)(dst##_i + i_dst) = fzeroVal;                \
	    *(mlib_d64 *)(dst##_r + i_dst) = dr0;                     \
	    *(mlib_d64 *)(dst##_i + i_dst + offset0) = fzeroVal;      \
	    *(mlib_d64 *)(dst##_r + i_dst + offset0) = dr1;           \
	    *(mlib_d64 *)(dst##_i + i_dst + offset1) = fzeroVal;      \
	    *(mlib_d64 *)(dst##_r + i_dst + offset1) = dr2;           \
	    *(mlib_d64 *)(dst##_i + i_dst + offset2) = fzeroVal;      \
	    *(mlib_d64 *)(dst##_r + i_dst + offset2) = dr3;           \
	}

/* *********************************************************** */

#define	LOOP_REV(leng, ord_arr, nnn, ppp, dst, src_main,                  \
	src_aux, wnd, load_macro, sc)                                     \
	{                                                                 \
	    mlib_s32 kkk, hgh, low, shift = 5 + leng - ord_arr, o_5 =     \
		ord_arr - 5;                                              \
	    mlib_s32 rev_1 = 1 << o_5, rev_2 = 2 << o_5, rev_3 =          \
		3 << o_5;                                                 \
	    for (kkk = 0; kkk < 8; )                                      \
		ppp[kkk++] <<= o_5;                                       \
	    for (kkk = hgh = 0; kkk < nnn; hgh++) {                       \
		mlib_s32 hgh_rev = REV_INDX(hgh, shift);                  \
		mlib_s32 kkk_0_rev = hgh_rev;                             \
	                                                                  \
		for (low = 0; low < 8; kkk += 4) {                        \
		    mlib_s32 kkk_3_rev = kkk_0_rev | rev_3, kkk_2_rev =   \
			kkk_0_rev | rev_2, kkk_1_rev =                    \
			kkk_0_rev | rev_1;                                \
		    sc##load_macro(dst, src_main, src_aux, wnd, kkk,      \
			fzeroVal);                                        \
		    kkk_0_rev = ppp[++low] | hgh_rev;                     \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

#define	LOOP_REV_OPT(leng, ord_arr, nnn, ppp, dst, src_main,              \
	src_aux, wnd, load_macro, sc)                                     \
	{                                                                 \
	    mlib_s32 kkk, hgh, low, shift = 5 + leng - ord_arr, o_5 =     \
		ord_arr - 5;                                              \
	    mlib_s32 rev_1 = 1 << o_5, rev_2 = 2 << o_5, rev_3 =          \
		3 << o_5;                                                 \
	    mlib_s32 offset = nnn / 2;                                    \
	                                                                  \
	    for (kkk = 0; kkk < 8; )                                      \
		ppp[kkk++] <<= o_5;                                       \
	    for (kkk = hgh = 0; kkk < nnn / 2; hgh++) {                   \
		mlib_s32 hgh_rev = REV_INDX(hgh, shift);                  \
		mlib_s32 kkk_0_rev = hgh_rev;                             \
	                                                                  \
		for (low = 0; low < 8; kkk += 4) {                        \
		    mlib_s32 kkk_3_rev = kkk_0_rev | rev_3, kkk_2_rev =   \
			kkk_0_rev | rev_2, kkk_1_rev =                    \
			kkk_0_rev | rev_1;                                \
		    sc##load_macro##_OPT(dst, src_main, src_aux,          \
			wnd, kkk, fzeroVal);                              \
		    kkk_0_rev = ppp[++low] | hgh_rev;                     \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

#define	LOOP_REV_OPT2(leng, ord_arr, nnn, ppp, dst, src_main,             \
	src_aux, wnd, load_macro, sc)                                     \
	{                                                                 \
	    mlib_s32 kkk, hgh, low, shift = 5 + leng - ord_arr, o_5 =     \
		ord_arr - 5;                                              \
	    mlib_s32 rev_1 = 1 << o_5, rev_2 = 2 << o_5, rev_3 =          \
		3 << o_5;                                                 \
	    mlib_s32 offset = nnn / 2, offset0 = nnn / 2, offset1 =       \
		nnn / 4, offset2 = offset + offset1;                      \
	    for (kkk = 0; kkk < 8; )                                      \
		ppp[kkk++] <<= o_5;                                       \
	    for (kkk = hgh = 0; kkk < nnn / 4; hgh++) {                   \
		mlib_s32 hgh_rev = REV_INDX(hgh, shift);                  \
		mlib_s32 kkk_0_rev = hgh_rev;                             \
	                                                                  \
		for (low = 0; low < 8; kkk += 4) {                        \
		    mlib_s32 kkk_3_rev = kkk_0_rev | rev_3, kkk_2_rev =   \
			kkk_0_rev | rev_2, kkk_1_rev =                    \
			kkk_0_rev | rev_1;                                \
		    sc##load_macro##_OPT2(dst, src_main,                  \
			src_aux, wnd, kkk, fzeroVal);                     \
		    kkk_0_rev = ppp[++low] | hgh_rev;                     \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

/*  code to perform bit reversion at all orders regarding load_macro  */
/*  if( ord_arr > 5 + REV_NBITS ) hope never come here  */
#define	LOAD_BIT_REV(ord_arr, dst, src_main, src_aux, wnd,          \
	load_macro, sc)                                             \
	{                                                           \
	    mlib_s32 CONST nnn = 1 << ord_arr;                      \
	    deal_v_t *CONST dst##_r = dst, *CONST dst##_i =         \
		dst + nnn;                                          \
	    mlib_s32 ppp[9] = {                                     \
		0, 16, 8, 24, 4, 20, 12, 28                         \
	    };                                                      \
	    LOOP_REV(REV_NBITS, ord_arr, nnn, ppp, dst, src_main,   \
		src_aux, wnd, load_macro, sc);                      \
	}

/* *********************************************************** */

#define	LOAD_BIT_REV_OPT(ord_arr, dst, src_main, src_aux,               \
	wnd, load_macro, sc)                                            \
	{                                                               \
	    mlib_s32 CONST nnn = 1 << ord_arr;                          \
	    deal_v_t *CONST dst##_r = dst, *CONST dst##_i =             \
		dst + nnn;                                              \
	    mlib_s32 ppp[9] = {                                         \
		0, 16, 8, 24, 4, 20, 12, 28                             \
	    };                                                          \
	    LOOP_REV_OPT(REV_NBITS, ord_arr, nnn, ppp, dst, src_main,   \
		src_aux, wnd, load_macro, sc);                          \
	}

/* *********************************************************** */

#define	LOAD_BIT_REV_OPT2(ord_arr, dst, src_main, src_aux,               \
	wnd, load_macro, sc)                                             \
	{                                                                \
	    mlib_s32 CONST nnn = 1 << ord_arr;                           \
	    deal_v_t *CONST dst##_r = dst, *CONST dst##_i =              \
		dst + nnn;                                               \
	    mlib_s32 ppp[9] = {                                          \
		0, 16, 8, 24, 4, 20, 12, 28                              \
	    };                                                           \
	    LOOP_REV_OPT2(REV_NBITS, ord_arr, nnn, ppp, dst, src_main,   \
		src_aux, wnd, load_macro, sc);                           \
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

static void
mlib_v_fftDW_LdBr_C_W(
    mlib_s32 CONST order,
    mlib_s32 CONST scaleup,
    deal_v_t *CONST pal64data,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window)
{
	if (scaleup > 0) {
		vis_write_gsr((scaleup << 3) + (1 << 3) + 6);
		LOAD_BIT_REV(order, pal64data, src_main, src_aux, window,
		    LOAD_C_W, SC_);
	} else {
		vis_write_gsr(6);
		LOAD_BIT_REV(order, pal64data, src_main, src_aux, window,
		    LOAD_C_W, NS_);
	}
}

/* *********************************************************** */

static void
mlib_v_fftDW_LdBr_C(
    mlib_s32 CONST order,
    mlib_s32 CONST scaleup,
    deal_v_t *CONST pal64data,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window)
{
	if (scaleup > 0) {
		const mlib_f32 shl6 = vis_to_float(0x40004000);

		vis_write_gsr((scaleup << 3) + (2 << 3) + 6);
		if (order < 6 || (((mlib_addr)(src_main) & 0x7) != 0) ||
		    (((mlib_addr)(src_aux) & 0x7) != 0)) {
			LOAD_BIT_REV(order, pal64data, src_main, src_aux,
			    window, LOAD_C, SC_);
		} else {
			LOAD_BIT_REV_OPT(order, pal64data, src_main, src_aux,
			    window, LOAD_C, SC_);
		}
	} else {
		vis_write_gsr(6);
		if (order < 6 || (((mlib_addr)(src_main) & 0x7) != 0) ||
		    (((mlib_addr)(src_aux) & 0x7) != 0)) {
			LOAD_BIT_REV(order, pal64data, src_main, src_aux,
			    window, LOAD_C, NS_);
		} else {
			vis_write_gsr(6);
			LOAD_BIT_REV_OPT(order, pal64data, src_main, src_aux,
			    window, LOAD_C, NS_);
		}
	}
}

/* *********************************************************** */

static void
mlib_v_fftDW_LdBr_RI_W(
    mlib_s32 CONST order,
    mlib_s32 CONST scaleup,
    deal_v_t *CONST pal64data,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window)
{
	if (scaleup > 0) {
		vis_write_gsr((scaleup << 3) + (1 << 3) + 6);
		LOAD_BIT_REV(order, pal64data, src_main, src_aux, window,
		    LOAD_RI_W, SC_);
	} else {
		vis_write_gsr(6);
		LOAD_BIT_REV(order, pal64data, src_main, src_aux, window,
		    LOAD_RI_W, NS_);
	}
}

/* *********************************************************** */

static void
mlib_v_fftDW_LdBr_RI(
    mlib_s32 CONST order,
    mlib_s32 CONST scaleup,
    deal_v_t *CONST pal64data,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window)
{
	if (scaleup > 0) {
		const mlib_f32 shl6 = vis_to_float(0x40004000);

		vis_write_gsr((scaleup << 3) + (2 << 3) + 6);
		if (order < 6 || (((mlib_addr)(src_main) & 0x7) != 0) ||
		    (((mlib_addr)(src_aux) & 0x7) != 0)) {
			LOAD_BIT_REV(order, pal64data, src_main, src_aux,
			    window, LOAD_RI, SC_);
		} else {
			LOAD_BIT_REV_OPT(order, pal64data, src_main, src_aux,
			    window, LOAD_RI, SC_);
		}
	} else {
		vis_write_gsr(6);
		if (order < 6 || (((mlib_addr)(src_main) & 0x7) != 0) ||
		    (((mlib_addr)(src_aux) & 0x7) != 0)) {
			LOAD_BIT_REV(order, pal64data, src_main, src_aux,
			    window, LOAD_RI, NS_);
		} else {
			LOAD_BIT_REV_OPT(order, pal64data, src_main, src_aux,
			    window, LOAD_RI, NS_);
		}
	}
}

/* *********************************************************** */

static void
mlib_v_fftDW_LdBr_R_W(
    mlib_s32 CONST order,
    mlib_s32 CONST scaleup,
    deal_v_t *CONST pal64data,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window)
{
	mlib_d64 fzeroVal = vis_fzero();

	if (scaleup > 0) {
		vis_write_gsr((scaleup << 3) + (1 << 3) + 6);
		LOAD_BIT_REV(order, pal64data, src_main, src_aux, window,
		    LOAD_R_W, SC_);
	} else {
		vis_write_gsr(6);
		LOAD_BIT_REV(order, pal64data, src_main, src_aux, window,
		    LOAD_R_W, NS_);
	}
}

/* *********************************************************** */

static void
mlib_v_fftDW_LdBr_R(
    mlib_s32 CONST order,
    mlib_s32 CONST scaleup,
    deal_v_t *CONST pal64data,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window)
{
	mlib_d64 fzeroVal = vis_fzero();

	if (scaleup > 0) {
		const mlib_f32 shl6 = vis_to_float(0x40004000);

		vis_write_gsr((scaleup << 3) + (2 << 3) + 6);
		if (order < 6 || (((mlib_addr)(src_main) & 0x7) != 0) ||
		    (((mlib_addr)(src_aux) & 0x7) != 0)) {
			LOAD_BIT_REV(order, pal64data, src_main, src_aux,
			    window, LOAD_R, SC_);
		} else {
			if (order == 6) {
				LOAD_BIT_REV_OPT(order, pal64data, src_main,
				    src_aux, window, LOAD_R, SC_);
			} else {
				LOAD_BIT_REV_OPT2(order, pal64data, src_main,
				    src_aux, window, LOAD_R, SC_);
			}
		}
	} else {
		vis_write_gsr(6);
		if (order < 6 || (((mlib_addr)(src_main) & 0x7) != 0) ||
		    (((mlib_addr)(src_aux) & 0x7) != 0)) {
			LOAD_BIT_REV(order, pal64data, src_main, src_aux,
			    window, LOAD_R, NS_);
		} else {
			LOAD_BIT_REV_OPT(order, pal64data, src_main, src_aux,
			    window, LOAD_R, NS_);
		}
	}
}

/* *********************************************************** */

static void
mlib_v_fftDW_GetData(
    deal_v_t *CONST pal64data,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST scaleup,
    mlib_s32 CONST data_info_flags)
{
/*  initialize deal buffer & serve window  */
	if (data_info_flags & eSrcCmplx) {
		if (window) {
			mlib_v_fftDW_LdBr_C_W(order, scaleup, pal64data,
			    src_main, src_aux, window);
		} else {
			mlib_v_fftDW_LdBr_C(order, scaleup, pal64data, src_main,
			    src_aux, window);
		}
	} else if (data_info_flags & eSrcReIm) {
		if (window) {
			mlib_v_fftDW_LdBr_RI_W(order, scaleup, pal64data,
			    src_main, src_aux, window);
		} else {
			mlib_v_fftDW_LdBr_RI(order, scaleup, pal64data,
			    src_main, src_aux, window);
		}
	} else {
		if (window) {
			mlib_v_fftDW_LdBr_R_W(order, scaleup, pal64data,
			    src_main, src_aux, window);
		} else {
			mlib_v_fftDW_LdBr_R(order, scaleup, pal64data, src_main,
			    src_aux, window);
		}
	}
}

/* *********************************************************** */

static mlib_status
mlib_SignalMerge2_S16(
    mlib_s16 *colors,
    mlib_s16 *color1,
    mlib_s16 *color2,
    mlib_s32 n)
{
	mlib_d64 *dp = (mlib_d64 *)colors;
	mlib_d64 *sp0 = (mlib_d64 *)color1;
	mlib_d64 *sp1 = (mlib_d64 *)color2;
	mlib_d64 sd0, sd1, sd2, sd3, dd0, dd1, dr, dr1;
	mlib_d64 dmask = vis_to_double_dup(0xFFFF);
	mlib_f32 f11 = vis_to_float(0x10001), f1010 = vis_to_float(0x1000100);
	mlib_s32 i;

	vis_write_gsr(0);

/* 8-pixels loop */
#pragma pipeloop(0)
	for (i = 0; i <= (n - 8); i += 8) {
		sd0 = sp0[0];
		sd1 = sp1[0];
		sd2 = sp0[1];
		sd3 = sp1[1];
		dd0 = vis_fpmerge(vis_read_hi(sd0), vis_read_lo(sd0));
		dd0 = vis_fpmerge(vis_read_hi(dd0), vis_read_lo(dd0));
		dd1 = vis_fpmerge(vis_read_hi(sd1), vis_read_lo(sd1));
		dd1 = vis_fpmerge(vis_read_hi(dd1), vis_read_lo(dd1));
		dr = vis_fpmerge(vis_read_hi(dd0), vis_read_hi(dd1));
		dr1 = vis_fpmerge(vis_read_lo(dd0), vis_read_lo(dd1));
		dp[0] = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));
		dp[1] = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr1));

		dd0 = vis_fmuld8sux16(f1010, vis_read_hi(sd2));
		dd1 = vis_fmuld8sux16(f1010, vis_read_lo(sd2));
		dr = vis_fmuld8ulx16(f11, vis_read_hi(sd3));
		dr1 = vis_fmuld8ulx16(f11, vis_read_lo(sd3));
		dd0 = vis_fpack32(dd0, dmask);
		dd1 = vis_fpack32(dd1, dmask);
		dp[2] = vis_for(vis_fand(dmask, dr), dd0);
		dp[3] = dr1;
		vis_pst_16(dd1, dp + 3, 0xa);
		sp0 += 2;
		sp1 += 2;
		dp += 4;
	}

/*
 *  FOR ORDER <= 2 ONLY. NOW THIS CASE is USED FOR ORDER > 4 *
 *
 *  if (i <= (n - 4)) {
 *    sd0 = (*sp0++);
 *    sd1 = (*sp1++);
 *    dd0 = vis_fpmerge(vis_read_hi(sd0), vis_read_lo(sd0));
 *    dd0 = vis_fpmerge(vis_read_hi(dd0), vis_read_lo(dd0));
 *    dd1 = vis_fpmerge(vis_read_hi(sd1), vis_read_lo(sd1));
 *    dd1 = vis_fpmerge(vis_read_hi(dd1), vis_read_lo(dd1));
 *    dr  = vis_fpmerge(vis_read_hi(dd0), vis_read_hi(dd1));
 *    dr1 = vis_fpmerge(vis_read_lo(dd0), vis_read_lo(dd1));
 *    dp[0] = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));
 *    dp[1] = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr1));
 *    dp += 2;
 *    i += 4;
 *  }
 *
 *  if (i < n) {
 *    mlib_s32 emask = 0xF0 >> (n & 3);
 *    sd0 = sp0[0];
 *    sd1 = sp1[0];
 *    dd0 = vis_fpmerge(vis_read_hi(sd0), vis_read_lo(sd0));
 *    dd0 = vis_fpmerge(vis_read_hi(dd0), vis_read_lo(dd0));
 *    dd1 = vis_fpmerge(vis_read_hi(sd1), vis_read_lo(sd1));
 *    dd1 = vis_fpmerge(vis_read_hi(dd1), vis_read_lo(dd1));
 *    dr  = vis_fpmerge(vis_read_hi(dd0), vis_read_hi(dd1));
 *    dr1 = vis_fpmerge(vis_read_lo(dd0), vis_read_lo(dd1));
 *    dd0 = vis_fpmerge(vis_read_hi(dr),  vis_read_hi(dr1));
 *    vis_pst_32(dd0, dp, emask >> 2);
 *    if (emask & 0x3) {
 *      dd1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr1));
 *      vis_pst_32(dd1, dp+1, emask);
 *    }
 *  }
 *
 */
	return (MLIB_SUCCESS);
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

/*  saturate & put results into destination  */
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
			mlib_SignalMerge2_S16((void *)dst_main,
			    (void *)srcvar_r, (void *)srcvar_i, nn);
		} else {

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
				mlib_d64 ir1, ii1, mix0, mix1, mix2, out1, out2,
				    out3, out4;

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
		}
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

typedef void (*type_fftK_2Layers) (
    mlib_s16 *data_r,
    mlib_s16 *data_i,
    mlib_s32 order,
    mlib_s32 layer,
    mlib_s32 div_flag);

/* *********************************************************** */

#define	MAX_ORDER	32

static void
mlib_v_fftKernel(
    mlib_s16 *pdata,
    mlib_s32 order,
    mlib_s32 isign,
    mlib_s32 DIVtype)
{
	mlib_s16 *data_r = pdata;
	mlib_s16 *data_i = data_r + (1 << order);
	void *tbl_high = data_i + (1 << order);
	mlib_d64 *tbl;
	mlib_s32 layer, slayer;
	mlib_u8 div_flag[MAX_ORDER];
	type_fftK_2Layers mlib_v_fftK_2Layers_Div;

	mlib_v_fftK_2Layers_Div = (isign > 0) ? mlib_v_fftK_2Layers_P_Div
	    : mlib_v_fftK_2Layers_M_Div;

	vis_write_gsr(0);

	switch (DIVtype) {
	case 0:
		if (order < 7) {
			mlib_v_fftK_3LayersFirst_nod1(data_r, data_i, order,
			    isign);
			slayer = order / 2 + 1;
		} else {
			mlib_v_fftK_3LayersFirst_nod3(data_r, data_i, order,
			    isign);
			slayer = order / 2;
			if (order == 9) {
				slayer++;
			}
		}
		for (layer = 4; layer <= order; layer++) {
/* nonzero if (layer > slayer) */
			div_flag[layer] = (slayer - layer) >> 31;
		}
		break;

	case 1:
		mlib_v_fftK_3LayersFirst(data_r, data_i, order, isign);
		for (layer = 4; layer <= order; layer++) {
			div_flag[layer] = 1;
		}
		break;

	case 2:
		mlib_v_fftK_3LayersFirst(data_r, data_i, order, isign);
		for (layer = 4; layer <= (order - 2); layer++) {
			div_flag[layer] = layer & 1;
		}
		div_flag[order - 1] = 1;
		div_flag[order] = 1;
		break;

	}

	tbl = (isign > 0) ? (void *)mlib_v_fftK_WTable_1_P
	    : (void *)mlib_v_fftK_WTable_1_M;

	if (order > FFT_VIS_TBL_ORDER) {
		mlib_v_fftK_CalcTbl(tbl_high, order, isign);
	}

	for (layer = 4; layer <= order; ) {
		mlib_s32 dflag = div_flag[layer];
		mlib_s32 dflag1 = div_flag[layer + 1];

/* 2 layers */
		if (layer < FFT_VIS_TBL_ORDER && layer < order && dflag1) {
			mlib_s32 ddflag =
			    (dflag) ? BOTH_LAYERS_DIV : ONE_LAYER_DIV;
			mlib_v_fftK_2Layers_Div(data_r, data_i, order, layer,
			    ddflag);
			layer += 2;
		} else {
/* 1 layer */
			if (layer > FFT_VIS_TBL_ORDER) {
				tbl = tbl_high;
			}
			if (dflag) {
				mlib_v_fftK_Layer_Div2(data_r, data_i, order,
				    layer, tbl);
			} else {
				mlib_v_fftK_Layer(data_r, data_i, order, layer,
				    tbl);
			}
			layer++;
		}
	}
}

/* *********************************************************** */

mlib_status
mlib_v_fftDispatcherW(
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST data_info_flags,
    mlib_s32 FFTtype)
{
	mlib_d64 al64_stack_buffer[512];
	deal_v_t *pal64 = (deal_v_t *)al64_stack_buffer;
	deal_v_t *p_malloced = NULL;
	size_t size2deal = sizeof (deal_v_t) << (order + 1);
	mlib_s32 isign =
	    (data_info_flags & eInversFFT) ? INVERS_FFT : DIRECT_FFT;
	mlib_s32 DIVtype, scaleup;

	if (order > FFT_VIS_TBL_ORDER) {
		size2deal += (4 << order);
	}

	if (size2deal > sizeof (al64_stack_buffer)) {
		p_malloced = (deal_v_t *)__mlib_malloc(size2deal);
		if (p_malloced == NULL) {
			return (MLIB_FAILURE);
		}
		pal64 = p_malloced;
	}

/* 1, 2, 3 -> 0, 1, 2 */
	DIVtype = FFTtype - 1;
/* 0 <-> 1 */
	if (DIVtype < 2 && isign == INVERS_FFT)
		DIVtype ^= 1;

	switch (DIVtype) {
	case 0:
		scaleup = (order < 10) ? 4 : ((order + 1) / 2);
		break;

	case 1:
		scaleup = 0;
		break;

	case 2:
		scaleup = ((order & 1) && (isign == INVERS_FFT)) ? 3 : 2;
		break;
	}

	mlib_v_fftDW_GetData(pal64, src_main, src_aux, window, order, scaleup,
	    data_info_flags);

	mlib_v_fftKernel(pal64, order, isign, DIVtype);

	mlib_v_fftDW_PutData(pal64, dst_main, dst_aux, order, data_info_flags);

	if (p_malloced) {
		__mlib_free(p_malloced);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
