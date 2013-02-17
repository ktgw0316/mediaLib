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

#pragma ident	"@(#)mlib_s_SignalFFTDisp.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *  NOTE: FFT below means F(ast)F(ourier)T(ransform)
 *     mlib_v_fftDispatcherW_? - dispatches various FFT functions: creates
 *                               ( dynamically ) main working buffer,
 *                               fills it by source data, calls kernel FFT
 *                               function ( direct or inverse ) and stores
 *                               results in destination array(s)
 *      mlib_s_fftDW_GetData   - static function performing filling of
 *                               the working buffer by source data
 *      mlib_s_fftDW_PutData   - static function performing filling of
 *                               destination array by the data from
 *                               working buffer
 *
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_v_fftDispatcherW_[1|2|3]
 *                  (mlib_s16 *CONST dst_main,
 *                   mlib_s16 *CONST dst_aux,
 *                   mlib_s16 *CONST src_main,
 *                   mlib_s16 *CONST src_aux,
 *                   mlib_s16 *CONST window,
 *                   mlib_s32 CONST  order,
 *                   mlib_s32 CONST  data_info_flags);
 *      static void mlib_s_fftDW_GetData
 *                  (deal_v_t *CONST pal128data,
 *                   mlib_s16 *CONST src_main,
 *                   mlib_s16 *CONST src_aux,
 *                   mlib_s16 *CONST window,
 *                   mlib_s32 CONST  order,
 *                   mlib_s32 CONST  scaleup,
 *                   mlib_s32 CONST  data_info_flags);
 *      static void mlib_s_fftDW_PutData
 *                  (deal_v_t *CONST pal128data,
 *                   mlib_s16 *CONST dst_main,
 *                   mlib_s16 *CONST dst_aux,
 *                   mlib_s16 *CONST window,
 *                   mlib_s32 CONST  order,
 *                   mlib_s32 CONST  data_info_flags);
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
 *      pal128data
 *               - 128bit-aligned complex array. Created dynamically inside
 *                 mlib_v_fftDispatcherW ( from stack or __mlib_malloc ).
 *                 Destination for mlib_s_fftDW_GetData.
 *                 Source for mlib_s_fftDW_PutData.
 *                 Source and destination for kernel FFT function.
 *                 pal128data[ 2 * i ] hold the real parts, and
 *                 pal128data[ 2 * i + 1 ] hold the imaginary parts
 *
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
#include <mlib_s_sse_int.h>
#include <mlib_s_SignalFFTUtil.h>
#include <mlib_s_SignalFFT_Tbl.h>
#include <mlib_s_SignalFFTDisp.h>

/* *********************************************************** */

/* max possible order 24 + 5 = 29 */
#define	REV_NBITS	24
#define	REV_INDX(indx, shift)	REV_INDX_24(indx, shift)

/* *********************************************************** */

/*  load data into deal buffers for various sources  */
#define	SC_LOAD_C_W_SSE2(dst, src_main, src_aux, wnd, i_dst, fzeroVal)    \
	{                                                                 \
	    mlib_s32 shift = 16 - 1 - scaleup; \
	    __m128i win, dr, di, tmp0, tmp1, tmp0h, tmp0l, tmp1h, tmp1l,  \
		tmp2h, tmp2l, tmp3h, tmp3l, rhr, rlr, rhi, rli;   \
	    win = _mm_setr_epi16(                                         \
			    wnd[i_dst##_0_rev0],                          \
			    wnd[i_dst##_1_rev0],                          \
			    wnd[i_dst##_2_rev0],                          \
			    wnd[i_dst##_3_rev0],                          \
			    wnd[i_dst##_0_rev1],                          \
			    wnd[i_dst##_1_rev1],                          \
			    wnd[i_dst##_2_rev1],                          \
			    wnd[i_dst##_3_rev1]);                         \
	    dr = _mm_setr_epi16(                                        \
			    src_main[(i_dst##_0_rev0) * 2],               \
			    src_main[(i_dst##_1_rev0) * 2],               \
			    src_main[(i_dst##_2_rev0) * 2],               \
			    src_main[(i_dst##_3_rev0) * 2],               \
			    src_main[(i_dst##_0_rev1) * 2],               \
			    src_main[(i_dst##_1_rev1) * 2],               \
			    src_main[(i_dst##_2_rev1) * 2],               \
			    src_main[(i_dst##_3_rev1) * 2]);              \
	    di = _mm_setr_epi16(                                        \
			    src_main[(i_dst##_0_rev0) * 2 + 1],           \
			    src_main[(i_dst##_1_rev0) * 2 + 1],           \
			    src_main[(i_dst##_2_rev0) * 2 + 1],           \
			    src_main[(i_dst##_3_rev0) * 2 + 1],           \
			    src_main[(i_dst##_0_rev1) * 2 + 1],           \
			    src_main[(i_dst##_1_rev1) * 2 + 1],           \
			    src_main[(i_dst##_2_rev1) * 2 + 1],           \
			    src_main[(i_dst##_3_rev1) * 2 + 1]);          \
	    tmp0h = _mm_mulhi_epi16(win, dr);                           \
	    tmp0l = _mm_mullo_epi16(win, dr);                           \
	    tmp1h = _mm_mulhi_epi16(win, di);                           \
	    tmp1l = _mm_mullo_epi16(win, di);                           \
	    rhr = _mm_unpackhi_epi16(tmp0l, tmp0h);                     \
	    rlr = _mm_unpacklo_epi16(tmp0l, tmp0h);                     \
	    rhi = _mm_unpackhi_epi16(tmp1l, tmp1h);                     \
	    rli = _mm_unpacklo_epi16(tmp1l, tmp1h);                     \
	    rhr = _mm_srai_epi32(rhr, shift); \
	    rlr = _mm_srai_epi32(rlr, shift); \
	    rhi = _mm_srai_epi32(rhi, shift); \
	    rli = _mm_srai_epi32(rli, shift); \
	    dr = _mm_packs_epi32(rlr, rhr);                               \
	    di = _mm_packs_epi32(rli, rhi);                               \
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst), dr);     \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst), di);     \
	}

/* *********************************************************** */

#define	SC_LOAD_RI_W_SSE2(dst, src_main, src_aux, wnd, i_dst, fzeroVal)   \
	{                                                                 \
	    mlib_s32 shift = 16 - 1 - scaleup; \
	    __m128i win, dr, di, tmp0, tmp1, tmp0h, tmp0l, tmp1h, tmp1l,  \
		tmp2h, tmp2l, tmp3h, tmp3l, rhr, rlr, rhi, rli;   \
	    win = _mm_setr_epi16(                                         \
			    wnd[i_dst##_0_rev0],                          \
			    wnd[i_dst##_1_rev0],                          \
			    wnd[i_dst##_2_rev0],                          \
			    wnd[i_dst##_3_rev0],                          \
			    wnd[i_dst##_0_rev1],                          \
			    wnd[i_dst##_1_rev1],                          \
			    wnd[i_dst##_2_rev1],                          \
			    wnd[i_dst##_3_rev1]);                         \
	    dr = _mm_setr_epi16(                                        \
			    src_main[(i_dst##_0_rev0)],               \
			    src_main[(i_dst##_1_rev0)],               \
			    src_main[(i_dst##_2_rev0)],               \
			    src_main[(i_dst##_3_rev0)],               \
			    src_main[(i_dst##_0_rev1)],               \
			    src_main[(i_dst##_1_rev1)],               \
			    src_main[(i_dst##_2_rev1)],               \
			    src_main[(i_dst##_3_rev1)]);              \
	    di = _mm_setr_epi16(                                        \
			    src_aux[(i_dst##_0_rev0)],           \
			    src_aux[(i_dst##_1_rev0)],           \
			    src_aux[(i_dst##_2_rev0)],           \
			    src_aux[(i_dst##_3_rev0)],           \
			    src_aux[(i_dst##_0_rev1)],           \
			    src_aux[(i_dst##_1_rev1)],           \
			    src_aux[(i_dst##_2_rev1)],           \
			    src_aux[(i_dst##_3_rev1)]);          \
	    tmp0h = _mm_mulhi_epi16(win, dr);                           \
	    tmp0l = _mm_mullo_epi16(win, dr);                           \
	    tmp1h = _mm_mulhi_epi16(win, di);                           \
	    tmp1l = _mm_mullo_epi16(win, di);                           \
	    rhr = _mm_unpackhi_epi16(tmp0l, tmp0h);                     \
	    rlr = _mm_unpacklo_epi16(tmp0l, tmp0h);                     \
	    rhi = _mm_unpackhi_epi16(tmp1l, tmp1h);                     \
	    rli = _mm_unpacklo_epi16(tmp1l, tmp1h);                     \
	    rhr = _mm_srai_epi32(rhr, shift); \
	    rlr = _mm_srai_epi32(rlr, shift); \
	    rhi = _mm_srai_epi32(rhi, shift); \
	    rli = _mm_srai_epi32(rli, shift); \
	    dr = _mm_packs_epi32(rlr, rhr);                               \
	    di = _mm_packs_epi32(rli, rhi);                               \
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst), dr);     \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst), di);     \
	}

/* *********************************************************** */

#define	SC_LOAD_R_W_SSE2(dst, src_main, src_aux, wnd, i_dst, fzeroVal)         \
	{                                                                 \
	    mlib_s32 shift = 16 - 1 - scaleup; \
	    __m128i win, dr, di, tmp0, tmp1, tmp0h, tmp0l, tmp1h, tmp1l,  \
		tmp2h, tmp2l, tmp3h, tmp3l, rhr, rlr, rhi, rli;   \
	    win = _mm_setr_epi16(                                         \
			    wnd[i_dst##_0_rev0],                          \
			    wnd[i_dst##_1_rev0],                          \
			    wnd[i_dst##_2_rev0],                          \
			    wnd[i_dst##_3_rev0],                          \
			    wnd[i_dst##_0_rev1],                          \
			    wnd[i_dst##_1_rev1],                          \
			    wnd[i_dst##_2_rev1],                          \
			    wnd[i_dst##_3_rev1]);                         \
	    dr = _mm_setr_epi16(                                        \
			    src_main[(i_dst##_0_rev0)],               \
			    src_main[(i_dst##_1_rev0)],               \
			    src_main[(i_dst##_2_rev0)],               \
			    src_main[(i_dst##_3_rev0)],               \
			    src_main[(i_dst##_0_rev1)],               \
			    src_main[(i_dst##_1_rev1)],               \
			    src_main[(i_dst##_2_rev1)],               \
			    src_main[(i_dst##_3_rev1)]);              \
	    tmp0h = _mm_mulhi_epi16(win, dr);                           \
	    tmp0l = _mm_mullo_epi16(win, dr);                           \
	    rhr = _mm_unpackhi_epi16(tmp0l, tmp0h);                     \
	    rlr = _mm_unpacklo_epi16(tmp0l, tmp0h);                     \
	    rhr = _mm_srai_epi32(rhr, shift); \
	    rlr = _mm_srai_epi32(rlr, shift); \
	    dr = _mm_packs_epi32(rlr, rhr);                               \
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst), dr);     \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst),          \
			    fzeroVal_SSE2);           \
	}

/* *********************************************************** */

#define	SC_LOAD_C_SSE2(dst, src_main, src_aux, wnd, i_dst, fzeroVal)          \
	{                                                                \
	    mlib_s32 shift = 16 - 2 - scaleup; \
	    mlib_s32 right_shift = 16 - (6 + 8); \
	    __m128i zeros = _mm_setzero_si128(); \
	    __m128i dr, di, rhr, rlr, rhi, rli;   \
	    dr = _mm_set_epi16(                                        \
			    src_main[(i_dst##_3_rev0) * 2],               \
			    src_main[(i_dst##_2_rev0) * 2],               \
			    src_main[(i_dst##_1_rev0) * 2],               \
			    src_main[(i_dst##_0_rev0) * 2],               \
			    src_main[(i_dst##_3_rev1) * 2],               \
			    src_main[(i_dst##_2_rev1) * 2],               \
			    src_main[(i_dst##_1_rev1) * 2],               \
			    src_main[(i_dst##_0_rev1) * 2]);              \
	    di = _mm_set_epi16(                                        \
			    src_main[(i_dst##_3_rev0) * 2 + 1],           \
			    src_main[(i_dst##_2_rev0) * 2 + 1],           \
			    src_main[(i_dst##_1_rev0) * 2 + 1],           \
			    src_main[(i_dst##_0_rev0) * 2 + 1],           \
			    src_main[(i_dst##_3_rev1) * 2 + 1],           \
			    src_main[(i_dst##_2_rev1) * 2 + 1],           \
			    src_main[(i_dst##_1_rev1) * 2 + 1],           \
			    src_main[(i_dst##_0_rev1) * 2 + 1]);          \
	    rhr = _mm_unpackhi_epi16(zeros, dr); \
	    rlr = _mm_unpacklo_epi16(zeros, dr); \
	    rhi = _mm_unpackhi_epi16(zeros, di); \
	    rli = _mm_unpacklo_epi16(zeros, di); \
	    /* reminder reminder: the two sets of shifting can be combined */ \
	    rhr = _mm_srai_epi32(rhr, right_shift); \
	    rlr = _mm_srai_epi32(rlr, right_shift); \
	    rhi = _mm_srai_epi32(rhi, right_shift); \
	    rli = _mm_srai_epi32(rli, right_shift); \
		\
	    rhr = _mm_srai_epi32(rhr, shift); \
	    rlr = _mm_srai_epi32(rlr, shift); \
	    rhi = _mm_srai_epi32(rhi, shift); \
	    rli = _mm_srai_epi32(rli, shift); \
	    dr = _mm_packs_epi32(rhr, rlr);                               \
	    di = _mm_packs_epi32(rhi, rli);                               \
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst), dr);     \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst), di);     \
	}

/* *********************************************************** */

#define	SC_LOAD_C_OPT_SSE2(dst, src_main, src_aux, wnd, i_dst, fzeroVal)   \
	{                                                                  \
	    __m128i tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;       \
	    __m128i r0, r1, r2, r3, dr0, di0, r4, r5, r6, r7, dr1, di1;   \
	    mlib_s32 *psrc = (mlib_s32 *)src_main; \
	    __m128i zeros = _mm_setzero_si128(); \
	    mlib_s32 shift = 16 - 2 - scaleup; \
	    mlib_s32 right_shift = 16 - (6 + 8); \
	                                                                   \
	    tmp0 = _mm_setr_epi32( \
			    psrc[i_dst##_0_rev0], \
			    psrc[i_dst##_0_rev1], \
			    psrc[i_dst##_1_rev0], \
			    psrc[i_dst##_1_rev1]); \
	    tmp1 = _mm_setr_epi32( \
			    psrc[i_dst##_2_rev0], \
			    psrc[i_dst##_2_rev1], \
			    psrc[i_dst##_3_rev0], \
			    psrc[i_dst##_3_rev1]); \
	    tmp2 = _mm_setr_epi32( \
			    psrc[i_dst##_0_rev0 + 1], \
			    psrc[i_dst##_0_rev1 + 1], \
			    psrc[i_dst##_1_rev0 + 1], \
			    psrc[i_dst##_1_rev1 + 1]); \
	    tmp3 = _mm_setr_epi32( \
			    psrc[i_dst##_2_rev0 + 1], \
			    psrc[i_dst##_2_rev1 + 1], \
			    psrc[i_dst##_3_rev0 + 1], \
			    psrc[i_dst##_3_rev1 + 1]); \
	    r0 = _mm_unpackhi_epi16(zeros, tmp0); \
	    r1 = _mm_unpacklo_epi16(zeros, tmp0); \
	    r2 = _mm_unpackhi_epi16(zeros, tmp1); \
	    r3 = _mm_unpacklo_epi16(zeros, tmp1); \
	    r4 = _mm_unpackhi_epi16(zeros, tmp2); \
	    r5 = _mm_unpacklo_epi16(zeros, tmp2); \
	    r6 = _mm_unpackhi_epi16(zeros, tmp3); \
	    r7 = _mm_unpacklo_epi16(zeros, tmp3); \
	/* reminder : following two sets of shifting can be combined */ \
	    r0 = _mm_srai_epi32(r0, right_shift); \
	    r1 = _mm_srai_epi32(r1, right_shift); \
	    r2 = _mm_srai_epi32(r2, right_shift); \
	    r3 = _mm_srai_epi32(r3, right_shift); \
	    r4 = _mm_srai_epi32(r4, right_shift); \
	    r5 = _mm_srai_epi32(r5, right_shift); \
	    r6 = _mm_srai_epi32(r6, right_shift); \
	    r7 = _mm_srai_epi32(r7, right_shift); \
		\
	    r0 = _mm_srai_epi32(r0, shift); \
	    r1 = _mm_srai_epi32(r1, shift); \
	    r2 = _mm_srai_epi32(r2, shift); \
	    r3 = _mm_srai_epi32(r3, shift); \
	    r4 = _mm_srai_epi32(r4, shift); \
	    r5 = _mm_srai_epi32(r5, shift); \
	    r6 = _mm_srai_epi32(r6, shift); \
	    r7 = _mm_srai_epi32(r7, shift); \
		\
	    dr0 = _mm_packs_epi32(r0, r2); \
	    di0 = _mm_packs_epi32(r1, r3); \
	    dr1 = _mm_packs_epi32(r4, r6); \
	    di1 = _mm_packs_epi32(r5, r7); \
		\
	    r0 = _mm_unpackhi_epi16(di0, dr0); \
	    r1 = _mm_unpacklo_epi16(di0, dr0); \
	    r2 = _mm_unpackhi_epi32(r1, r0); \
	    r3 = _mm_unpacklo_epi32(r1, r0); \
	    di0 = _mm_unpackhi_epi64(r3, r2); \
	    dr0 = _mm_unpacklo_epi64(r3, r2); \
	    r0 = _mm_unpackhi_epi16(di1, dr1); \
	    r1 = _mm_unpacklo_epi16(di1, dr1); \
	    r2 = _mm_unpackhi_epi32(r1, r0); \
	    r3 = _mm_unpacklo_epi32(r1, r0); \
	    di1 = _mm_unpackhi_epi64(r3, r2); \
	    dr1 = _mm_unpacklo_epi64(r3, r2); \
		\
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst), dr0);    \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst), di0);    \
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst + offset), dr1);\
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst + offset), di1);\
	}

#define	SC_LOAD_C_OPT_SSE2_64(dst, src_main, src_aux, wnd, i_dst, fzeroVal)  \
	{                                                                  \
	    __m128i tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;       \
	    __m128i r0, r1, r2, r3, dr0, di0, r4, r5, r6, r7, dr1, di1;   \
	    mlib_s32 *psrc = (mlib_s32 *)src_main; \
	    __m128i zeros = _mm_setzero_si128(); \
	    mlib_s32 shift = 16 - 2 - scaleup; \
	    mlib_s32 right_shift = 16 - (6 + 8); \
	                                                                   \
	    tmp0 = _mm_setr_epi32( \
			    psrc[i_dst##_0_rev], \
			    psrc[i_dst##_0_rev + 1], \
			    psrc[i_dst##_1_rev], \
			    psrc[i_dst##_1_rev + 1]); \
	    tmp1 = _mm_setr_epi32( \
			    psrc[i_dst##_2_rev], \
			    psrc[i_dst##_2_rev + 1], \
			    psrc[i_dst##_3_rev], \
			    psrc[i_dst##_3_rev + 1]); \
	    r0 = _mm_unpackhi_epi16(zeros, tmp0); \
	    r1 = _mm_unpacklo_epi16(zeros, tmp0); \
	    r2 = _mm_unpackhi_epi16(zeros, tmp1); \
	    r3 = _mm_unpacklo_epi16(zeros, tmp1); \
	/* reminder : following two sets of shifting can be combined */ \
	    r0 = _mm_srai_epi32(r0, right_shift); \
	    r1 = _mm_srai_epi32(r1, right_shift); \
	    r2 = _mm_srai_epi32(r2, right_shift); \
	    r3 = _mm_srai_epi32(r3, right_shift); \
		\
	    r0 = _mm_srai_epi32(r0, shift); \
	    r1 = _mm_srai_epi32(r1, shift); \
	    r2 = _mm_srai_epi32(r2, shift); \
	    r3 = _mm_srai_epi32(r3, shift); \
		\
	    di0 = _mm_packs_epi32(r0, r2); \
	    dr0 = _mm_packs_epi32(r1, r3); \
		\
	    r0 = _mm_unpackhi_epi16(dr0, di0); \
	    r1 = _mm_unpacklo_epi16(dr0, di0); \
	    r2 = _mm_unpackhi_epi32(r1, r0); \
	    r3 = _mm_unpacklo_epi32(r1, r0); \
	    di0 = _mm_unpackhi_epi64(r3, r2); \
	    dr0 = _mm_unpacklo_epi64(r3, r2); \
		\
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst), dr0);    \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst), di0);    \
	}

/* *********************************************************** */

#define	SC_LOAD_RI_SSE2(dst, src_main, src_aux, wnd, i_dst, fzeroVal)         \
	{                                                                \
	    mlib_s32 shift = 16 - 2 - scaleup; \
	    mlib_s32 right_shift = 16 - (6 + 8); \
	    __m128i zeros = _mm_setzero_si128(); \
	    __m128i dr, di, rhr, rlr, rhi, rli;   \
	    dr = _mm_setr_epi16(                                        \
			    src_main[(i_dst##_0_rev0)],               \
			    src_main[(i_dst##_1_rev0)],               \
			    src_main[(i_dst##_2_rev0)],               \
			    src_main[(i_dst##_3_rev0)],               \
			    src_main[(i_dst##_0_rev1)],               \
			    src_main[(i_dst##_1_rev1)],               \
			    src_main[(i_dst##_2_rev1)],               \
			    src_main[(i_dst##_3_rev1)]);              \
	    di = _mm_setr_epi16(                                        \
			    src_aux[(i_dst##_0_rev0)],           \
			    src_aux[(i_dst##_1_rev0)],           \
			    src_aux[(i_dst##_2_rev0)],           \
			    src_aux[(i_dst##_3_rev0)],           \
			    src_aux[(i_dst##_0_rev1)],           \
			    src_aux[(i_dst##_1_rev1)],           \
			    src_aux[(i_dst##_2_rev1)],           \
			    src_aux[(i_dst##_3_rev1)]);          \
	    rhr = _mm_unpackhi_epi16(zeros, dr); \
	    rlr = _mm_unpacklo_epi16(zeros, dr); \
	    rhi = _mm_unpackhi_epi16(zeros, di); \
	    rli = _mm_unpacklo_epi16(zeros, di); \
	    /* reminder reminder: the two sets of shifting can be combined */ \
	    rhr = _mm_srai_epi32(rhr, right_shift); \
	    rlr = _mm_srai_epi32(rlr, right_shift); \
	    rhi = _mm_srai_epi32(rhi, right_shift); \
	    rli = _mm_srai_epi32(rli, right_shift); \
		\
	    rhr = _mm_srai_epi32(rhr, shift); \
	    rlr = _mm_srai_epi32(rlr, shift); \
	    rhi = _mm_srai_epi32(rhi, shift); \
	    rli = _mm_srai_epi32(rli, shift); \
	    dr = _mm_packs_epi32(rlr, rhr);                               \
	    di = _mm_packs_epi32(rli, rhi);                               \
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst), dr);     \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst), di);     \
	}

/* *********************************************************** */

#define	SC_LOAD_RI_OPT_SSE2(dst, src_main, src_aux, wnd, i_dst, fzeroVal)  \
	{                                                                  \
	    __m128i tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;       \
	    __m128i r0, r1, r2, r3, dr0, di0, r4, r5, r6, r7, dr1, di1;   \
	    __m128i zeros = _mm_setzero_si128(); \
	    mlib_s32 shift = 16 - 2 - scaleup; \
	    mlib_s32 right_shift = 16 - (6 + 8); \
	                                                                   \
	    tmp0 = _mm_setr_epi32( \
			    *(mlib_s32 *)(src_main + i_dst##_0_rev0), \
			    *(mlib_s32 *)(src_main + i_dst##_0_rev1), \
			    *(mlib_s32 *)(src_main + i_dst##_1_rev0), \
			    *(mlib_s32 *)(src_main + i_dst##_1_rev1)); \
	    tmp1 = _mm_setr_epi32( \
			    *(mlib_s32 *)(src_main + i_dst##_2_rev0), \
			    *(mlib_s32 *)(src_main + i_dst##_2_rev1), \
			    *(mlib_s32 *)(src_main + i_dst##_3_rev0), \
			    *(mlib_s32 *)(src_main + i_dst##_3_rev1)); \
	    tmp2 = _mm_setr_epi32( \
			    *(mlib_s32 *)(src_aux + i_dst##_0_rev0), \
			    *(mlib_s32 *)(src_aux + i_dst##_0_rev1), \
			    *(mlib_s32 *)(src_aux + i_dst##_1_rev0), \
			    *(mlib_s32 *)(src_aux + i_dst##_1_rev1)); \
	    tmp3 = _mm_setr_epi32( \
			    *(mlib_s32 *)(src_aux + i_dst##_2_rev0), \
			    *(mlib_s32 *)(src_aux + i_dst##_2_rev1), \
			    *(mlib_s32 *)(src_aux + i_dst##_3_rev0), \
			    *(mlib_s32 *)(src_aux + i_dst##_3_rev1)); \
	    r0 = _mm_unpackhi_epi16(zeros, tmp0); \
	    r1 = _mm_unpacklo_epi16(zeros, tmp0); \
	    r2 = _mm_unpackhi_epi16(zeros, tmp1); \
	    r3 = _mm_unpacklo_epi16(zeros, tmp1); \
	    r4 = _mm_unpackhi_epi16(zeros, tmp2); \
	    r5 = _mm_unpacklo_epi16(zeros, tmp2); \
	    r6 = _mm_unpackhi_epi16(zeros, tmp3); \
	    r7 = _mm_unpacklo_epi16(zeros, tmp3); \
	/* reminder : following two sets of shifting can be combined */ \
	    r0 = _mm_srai_epi32(r0, right_shift); \
	    r1 = _mm_srai_epi32(r1, right_shift); \
	    r2 = _mm_srai_epi32(r2, right_shift); \
	    r3 = _mm_srai_epi32(r3, right_shift); \
	    r4 = _mm_srai_epi32(r4, right_shift); \
	    r5 = _mm_srai_epi32(r5, right_shift); \
	    r6 = _mm_srai_epi32(r6, right_shift); \
	    r7 = _mm_srai_epi32(r7, right_shift); \
		\
	    r0 = _mm_srai_epi32(r0, shift); \
	    r1 = _mm_srai_epi32(r1, shift); \
	    r2 = _mm_srai_epi32(r2, shift); \
	    r3 = _mm_srai_epi32(r3, shift); \
	    r4 = _mm_srai_epi32(r4, shift); \
	    r5 = _mm_srai_epi32(r5, shift); \
	    r6 = _mm_srai_epi32(r6, shift); \
	    r7 = _mm_srai_epi32(r7, shift); \
		\
	    dr0 = _mm_packs_epi32(r0, r2); \
	    dr1 = _mm_packs_epi32(r1, r3); \
	    di0 = _mm_packs_epi32(r4, r6); \
	    di1 = _mm_packs_epi32(r5, r7); \
		\
	    r0 = _mm_unpackhi_epi16(dr1, dr0); \
	    r1 = _mm_unpacklo_epi16(dr1, dr0); \
	    r2 = _mm_unpackhi_epi32(r1, r0); \
	    r3 = _mm_unpacklo_epi32(r1, r0); \
	    dr1 = _mm_unpackhi_epi64(r3, r2); \
	    dr0 = _mm_unpacklo_epi64(r3, r2); \
	    r0 = _mm_unpackhi_epi16(di1, di0); \
	    r1 = _mm_unpacklo_epi16(di1, di0); \
	    r2 = _mm_unpackhi_epi32(r1, r0); \
	    r3 = _mm_unpacklo_epi32(r1, r0); \
	    di1 = _mm_unpackhi_epi64(r3, r2); \
	    di0 = _mm_unpacklo_epi64(r3, r2); \
		\
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst), dr0);    \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst), di0);    \
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst + offset), dr1);\
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst + offset), di1);\
	}

#define	SC_LOAD_RI_OPT_SSE2_64(dst, src_main, src_aux, wnd, i_dst, fzeroVal)  \
	{                                                                  \
	    __m128i tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;       \
	    __m128i r0, r1, r2, r3, dr0, di0, r4, r5, r6, r7, dr1, di1;   \
	    mlib_s32 *psrc = (mlib_s32 *)src_main; \
	    mlib_s32 *paux = (mlib_s32 *)src_aux; \
	    __m128i zeros = _mm_setzero_si128(); \
	    mlib_s32 shift = 16 - 2 - scaleup; \
	    mlib_s32 right_shift = 16 - (6 + 8); \
	                                                                   \
	    tmp0 = _mm_setr_epi32( \
			    *(mlib_s32 *)(src_main + i_dst##_0_rev), \
			    *(mlib_s32 *)(src_aux + i_dst##_0_rev), \
			    *(mlib_s32 *)(src_main + i_dst##_1_rev), \
			    *(mlib_s32 *)(src_aux + i_dst##_1_rev)); \
	    tmp1 = _mm_setr_epi32( \
			    *(mlib_s32 *)(src_main + i_dst##_2_rev), \
			    *(mlib_s32 *)(src_aux + i_dst##_2_rev), \
			    *(mlib_s32 *)(src_main + i_dst##_3_rev), \
			    *(mlib_s32 *)(src_aux + i_dst##_3_rev)); \
	    r0 = _mm_unpackhi_epi16(zeros, tmp0); \
	    r1 = _mm_unpacklo_epi16(zeros, tmp0); \
	    r2 = _mm_unpackhi_epi16(zeros, tmp1); \
	    r3 = _mm_unpacklo_epi16(zeros, tmp1); \
	/* reminder : following two sets of shifting can be combined */ \
	    r0 = _mm_srai_epi32(r0, right_shift); \
	    r1 = _mm_srai_epi32(r1, right_shift); \
	    r2 = _mm_srai_epi32(r2, right_shift); \
	    r3 = _mm_srai_epi32(r3, right_shift); \
		\
	    r0 = _mm_srai_epi32(r0, shift); \
	    r1 = _mm_srai_epi32(r1, shift); \
	    r2 = _mm_srai_epi32(r2, shift); \
	    r3 = _mm_srai_epi32(r3, shift); \
		\
	    dr0 = _mm_packs_epi32(r0, r2); \
	    di0 = _mm_packs_epi32(r1, r3); \
		\
	    r0 = _mm_unpackhi_epi16(di0, dr0); \
	    r1 = _mm_unpacklo_epi16(di0, dr0); \
	    di0 = _mm_unpackhi_epi32(r1, r0); \
	    dr0 = _mm_unpacklo_epi32(r1, r0); \
		\
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst), dr0);    \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst), di0);    \
	}

/* *********************************************************** */

#define	SC_LOAD_R_SSE2(dst, src_main, src_aux, wnd, i_dst, fzeroVal)        \
	{                                                              \
	    mlib_s32 shift = 16 - 2 - scaleup; \
	    mlib_s32 right_shift = 16 - (6 + 8); \
	    __m128i zeros = _mm_setzero_si128(); \
	    __m128i dr, di, rhr, rlr, rhi, rli;   \
	    dr = _mm_setr_epi16(                                        \
			    src_main[(i_dst##_0_rev0)],               \
			    src_main[(i_dst##_1_rev0)],               \
			    src_main[(i_dst##_2_rev0)],               \
			    src_main[(i_dst##_3_rev0)],               \
			    src_main[(i_dst##_0_rev1)],               \
			    src_main[(i_dst##_1_rev1)],               \
			    src_main[(i_dst##_2_rev1)],               \
			    src_main[(i_dst##_3_rev1)]);              \
	    rhr = _mm_unpackhi_epi16(zeros, dr); \
	    rlr = _mm_unpacklo_epi16(zeros, dr); \
	    /* reminder reminder: the two sets of shifting can be combined */ \
	    rhr = _mm_srai_epi32(rhr, right_shift); \
	    rlr = _mm_srai_epi32(rlr, right_shift); \
		\
	    rhr = _mm_srai_epi32(rhr, shift); \
	    rlr = _mm_srai_epi32(rlr, shift); \
	    dr = _mm_packs_epi32(rlr, rhr);                               \
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst), dr);     \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst),          \
			    fzeroVal_SSE2);           \
	}

/* *********************************************************** */

#define	SC_LOAD_R_OPT_SSE2(dst, src_main, src_aux, wnd, i_dst, fzeroVal)   \
	{                                                             \
	    __m128i tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;       \
	    __m128i r0, r1, r2, r3, dr0, di0, r4, r5, r6, r7, dr1, di1;   \
	    __m128i zeros = _mm_setzero_si128(); \
	    mlib_s32 shift = 16 - 2 - scaleup; \
	    mlib_s32 right_shift = 16 - (6 + 8); \
	                                                                   \
	    tmp0 = _mm_setr_epi32( \
			    *(mlib_s32 *)(src_main + i_dst##_0_rev0), \
			    *(mlib_s32 *)(src_main + i_dst##_0_rev1), \
			    *(mlib_s32 *)(src_main + i_dst##_1_rev0), \
			    *(mlib_s32 *)(src_main + i_dst##_1_rev1)); \
	    tmp1 = _mm_setr_epi32( \
			    *(mlib_s32 *)(src_main + i_dst##_2_rev0), \
			    *(mlib_s32 *)(src_main + i_dst##_2_rev1), \
			    *(mlib_s32 *)(src_main + i_dst##_3_rev0), \
			    *(mlib_s32 *)(src_main + i_dst##_3_rev1)); \
	    r0 = _mm_unpackhi_epi16(zeros, tmp0); \
	    r1 = _mm_unpacklo_epi16(zeros, tmp0); \
	    r2 = _mm_unpackhi_epi16(zeros, tmp1); \
	    r3 = _mm_unpacklo_epi16(zeros, tmp1); \
	/* reminder : following two sets of shifting can be combined */ \
	    r0 = _mm_srai_epi32(r0, right_shift); \
	    r1 = _mm_srai_epi32(r1, right_shift); \
	    r2 = _mm_srai_epi32(r2, right_shift); \
	    r3 = _mm_srai_epi32(r3, right_shift); \
		\
	    r0 = _mm_srai_epi32(r0, shift); \
	    r1 = _mm_srai_epi32(r1, shift); \
	    r2 = _mm_srai_epi32(r2, shift); \
	    r3 = _mm_srai_epi32(r3, shift); \
		\
	    dr0 = _mm_packs_epi32(r0, r2); \
	    dr1 = _mm_packs_epi32(r1, r3); \
		\
	    r0 = _mm_unpackhi_epi16(dr0, dr1); \
	    r1 = _mm_unpacklo_epi16(dr0, dr1); \
	    r2 = _mm_unpackhi_epi32(r0, r1); \
	    r3 = _mm_unpacklo_epi32(r0, r1); \
	    dr0 = _mm_unpackhi_epi64(r3, r2); \
	    dr1 = _mm_unpacklo_epi64(r3, r2); \
		\
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst), dr0);    \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst), \
			    fzeroVal_SSE2);    \
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst + offset), \
			    dr1);    \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst + offset), \
			    fzeroVal_SSE2);    \
	}

#define	SC_LOAD_R_OPT_SSE2_64(dst, src_main, src_aux, wnd, i_dst, fzeroVal)   \
	{                                                             \
	    __m128i tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;       \
	    __m128i r0, r1, r2, r3, dr0, di0, r4, r5, r6, r7, dr1, di1;   \
	    __m128i zeros = _mm_setzero_si128(); \
	    mlib_s32 shift = 16 - 2 - scaleup; \
	    mlib_s32 right_shift = 16 - (6 + 8); \
	                                                                   \
	    tmp0 = _mm_setr_epi32( \
			    *(mlib_s32 *)(src_main + i_dst##_0_rev), \
			    *(mlib_s32 *)(src_main + i_dst##_2_rev), \
			    *(mlib_s32 *)(src_main + i_dst##_1_rev), \
			    *(mlib_s32 *)(src_main + i_dst##_3_rev)); \
	    r0 = _mm_unpackhi_epi16(zeros, tmp0); \
	    r1 = _mm_unpacklo_epi16(zeros, tmp0); \
	/* reminder : following two sets of shifting can be combined */ \
	    r0 = _mm_srai_epi32(r0, right_shift); \
	    r1 = _mm_srai_epi32(r1, right_shift); \
		\
	    r0 = _mm_srai_epi32(r0, shift); \
	    r1 = _mm_srai_epi32(r1, shift); \
		\
	    dr0 = _mm_packs_epi32(r0, r1); \
		\
	    r0 = _mm_unpackhi_epi16(dr0, _mm_slli_si128(dr0, 8)); \
	    dr0 = _mm_unpackhi_epi32(_mm_slli_si128(r0, 8), r0); \
		\
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst), dr0);    \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst), \
			    fzeroVal_SSE2);    \
	}

/* *********************************************************** */

#define	SC_LOAD_R_OPT2_SSE2(dst, src_main, src_aux, wnd, i_dst, fzeroVal)  \
	{                                                             \
	    __m128i tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;       \
	    __m128i r0, r1, r2, r3, dr0, dr1, r4, r5, r6, r7, dr2, dr3;   \
	    __m128i zeros = _mm_setzero_si128(); \
	    mlib_s32 shift = 16 - 2 - scaleup; \
	    mlib_s32 right_shift = 16 - (6 + 8); \
	                                                                   \
	    tmp0 = _mm_loadl_epi64((__m128i *)(src_main + i_dst##_0_rev0)); \
	    tmp1 = _mm_loadl_epi64((__m128i *)(src_main + i_dst##_0_rev1)); \
	    tmp2 = _mm_loadl_epi64((__m128i *)(src_main + i_dst##_1_rev0)); \
	    tmp3 = _mm_loadl_epi64((__m128i *)(src_main + i_dst##_1_rev1)); \
	    tmp4 = _mm_loadl_epi64((__m128i *)(src_main + i_dst##_2_rev0)); \
	    tmp5 = _mm_loadl_epi64((__m128i *)(src_main + i_dst##_2_rev1)); \
	    tmp6 = _mm_loadl_epi64((__m128i *)(src_main + i_dst##_3_rev0)); \
	    tmp7 = _mm_loadl_epi64((__m128i *)(src_main + i_dst##_3_rev1)); \
		\
	    r0 = _mm_unpacklo_epi16(zeros, tmp0); \
	    r1 = _mm_unpacklo_epi16(zeros, tmp1); \
	    r2 = _mm_unpacklo_epi16(zeros, tmp2); \
	    r3 = _mm_unpacklo_epi16(zeros, tmp3); \
	    r4 = _mm_unpacklo_epi16(zeros, tmp4); \
	    r5 = _mm_unpacklo_epi16(zeros, tmp5); \
	    r6 = _mm_unpacklo_epi16(zeros, tmp6); \
	    r7 = _mm_unpacklo_epi16(zeros, tmp7); \
	/* reminder : following two sets of shifting can be combined */ \
	    r0 = _mm_srai_epi32(r0, right_shift); \
	    r1 = _mm_srai_epi32(r1, right_shift); \
	    r2 = _mm_srai_epi32(r2, right_shift); \
	    r3 = _mm_srai_epi32(r3, right_shift); \
	    r4 = _mm_srai_epi32(r4, right_shift); \
	    r5 = _mm_srai_epi32(r5, right_shift); \
	    r6 = _mm_srai_epi32(r6, right_shift); \
	    r7 = _mm_srai_epi32(r7, right_shift); \
		\
	    r0 = _mm_srai_epi32(r0, shift); \
	    r1 = _mm_srai_epi32(r1, shift); \
	    r2 = _mm_srai_epi32(r2, shift); \
	    r3 = _mm_srai_epi32(r3, shift); \
	    r4 = _mm_srai_epi32(r4, shift); \
	    r5 = _mm_srai_epi32(r5, shift); \
	    r6 = _mm_srai_epi32(r6, shift); \
	    r7 = _mm_srai_epi32(r7, shift); \
		\
	    dr0 = _mm_packs_epi32(r0, r1); \
	    dr1 = _mm_packs_epi32(r2, r3); \
	    dr2 = _mm_packs_epi32(r4, r5); \
	    dr3 = _mm_packs_epi32(r6, r7); \
		\
	    /* reminder reminder: following code might be optimized more */ \
	    r0 = _mm_unpackhi_epi16(dr0, dr1); \
	    r1 = _mm_unpacklo_epi16(dr0, dr1); \
	    r2 = _mm_unpackhi_epi16(dr2, dr3); \
	    r3 = _mm_unpacklo_epi16(dr2, dr3); \
	    r4 = _mm_unpackhi_epi32(r0, r2); \
	    r5 = _mm_unpacklo_epi32(r0, r2); \
	    r6 = _mm_unpackhi_epi32(r1, r3); \
	    r7 = _mm_unpacklo_epi32(r1, r3); \
	    r0 = _mm_unpackhi_epi64(r5, r4); \
	    r1 = _mm_unpacklo_epi64(r5, r4); \
	    r2 = _mm_unpackhi_epi64(r7, r6); \
	    r3 = _mm_unpacklo_epi64(r7, r6); \
		\
	    dr3 = _mm_unpackhi_epi64(r2, r0); \
	    dr1 = _mm_unpacklo_epi64(r2, r0); \
	    dr2 = _mm_unpackhi_epi64(r3, r1); \
	    dr0 = _mm_unpacklo_epi64(r3, r1); \
		\
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst), dr0);    \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst), \
			    fzeroVal_SSE2);    \
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst + offset0), \
			    dr1);    \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst + offset0), \
			    fzeroVal_SSE2);    \
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst + offset1), \
			    dr2);    \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst + offset1), \
			    fzeroVal_SSE2);    \
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst + offset2), \
			    dr3);    \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst + offset2), \
			    fzeroVal_SSE2);    \
	}

#define	SC_LOAD_R_OPT2_SSE2_64(dst, src_main, src_aux, wnd, i_dst, fzeroVal)  \
	{                                                             \
	    __m128i tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;       \
	    __m128i r0, r1, r2, r3, dr0, dr1, r4, r5, r6, r7, dr2, dr3;   \
	    __m128i zeros = _mm_setzero_si128(); \
	    mlib_s32 shift = 16 - 2 - scaleup; \
	    mlib_s32 right_shift = 16 - (6 + 8); \
	                                                                   \
	    tmp0 = _mm_loadl_epi64((__m128i *)(src_main + i_dst##_0_rev)); \
	    tmp1 = _mm_loadl_epi64((__m128i *)(src_main + i_dst##_1_rev)); \
	    tmp2 = _mm_loadl_epi64((__m128i *)(src_main + i_dst##_2_rev)); \
	    tmp3 = _mm_loadl_epi64((__m128i *)(src_main + i_dst##_3_rev)); \
		\
	    r0 = _mm_unpacklo_epi16(zeros, tmp0); \
	    r1 = _mm_unpacklo_epi16(zeros, tmp1); \
	    r2 = _mm_unpacklo_epi16(zeros, tmp2); \
	    r3 = _mm_unpacklo_epi16(zeros, tmp3); \
	/* reminder : following two sets of shifting can be combined */ \
	    r0 = _mm_srai_epi32(r0, right_shift); \
	    r1 = _mm_srai_epi32(r1, right_shift); \
	    r2 = _mm_srai_epi32(r2, right_shift); \
	    r3 = _mm_srai_epi32(r3, right_shift); \
		\
	    r0 = _mm_srai_epi32(r0, shift); \
	    r1 = _mm_srai_epi32(r1, shift); \
	    r2 = _mm_srai_epi32(r2, shift); \
	    r3 = _mm_srai_epi32(r3, shift); \
		\
	    dr0 = _mm_packs_epi32(r0, r2); \
	    dr1 = _mm_packs_epi32(r1, r3); \
		\
	    r0 = _mm_unpacklo_epi16(dr0, dr1); \
	    r1 = _mm_unpackhi_epi16(dr0, dr1); \
	    r2 = _mm_unpackhi_epi32(r0, r1); \
	    r3 = _mm_unpacklo_epi32(r0, r1); \
	    dr1 = _mm_unpackhi_epi64(r3, r2); \
	    dr0 = _mm_unpacklo_epi64(r3, r2); \
		\
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst), dr0);    \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst),          \
			    fzeroVal_SSE2);    \
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst + offset0),   \
			    dr1);    \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst + offset0),   \
			    fzeroVal_SSE2);    \
	}

/* *********************************************************** */

/* load data into deal buffers for various sources */
#define	NS_LOAD_C_W_SSE2(dst, src_main, src_aux, wnd, i_dst, fzeroVal)       \
	{                                                               \
	    __m128i win, dr, di;   \
	    win = _mm_setr_epi16(                                         \
			    wnd[i_dst##_0_rev0],                          \
			    wnd[i_dst##_1_rev0],                          \
			    wnd[i_dst##_2_rev0],                          \
			    wnd[i_dst##_3_rev0],                          \
			    wnd[i_dst##_0_rev1],                          \
			    wnd[i_dst##_1_rev1],                          \
			    wnd[i_dst##_2_rev1],                          \
			    wnd[i_dst##_3_rev1]);                         \
	    dr = _mm_setr_epi16(                                        \
			    src_main[(i_dst##_0_rev0) * 2],               \
			    src_main[(i_dst##_1_rev0) * 2],               \
			    src_main[(i_dst##_2_rev0) * 2],               \
			    src_main[(i_dst##_3_rev0) * 2],               \
			    src_main[(i_dst##_0_rev1) * 2],               \
			    src_main[(i_dst##_1_rev1) * 2],               \
			    src_main[(i_dst##_2_rev1) * 2],               \
			    src_main[(i_dst##_3_rev1) * 2]);              \
	    di = _mm_setr_epi16(                                        \
			    src_main[(i_dst##_0_rev0) * 2 + 1],           \
			    src_main[(i_dst##_1_rev0) * 2 + 1],           \
			    src_main[(i_dst##_2_rev0) * 2 + 1],           \
			    src_main[(i_dst##_3_rev0) * 2 + 1],           \
			    src_main[(i_dst##_0_rev1) * 2 + 1],           \
			    src_main[(i_dst##_1_rev1) * 2 + 1],           \
			    src_main[(i_dst##_2_rev1) * 2 + 1],           \
			    src_main[(i_dst##_3_rev1) * 2 + 1]);          \
	    dr = _mm_mulhi_epi16_vis(win, dr);                           \
	    di = _mm_mulhi_epi16_vis(win, di);                           \
	    dr = _mm_add_epi16(dr, dr); \
	    di = _mm_add_epi16(di, di); \
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst), dr);        \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst), di);        \
	}

/* *********************************************************** */

#define	NS_LOAD_RI_W_SSE2(dst, src_main, src_aux, wnd, i_dst, fzeroVal)     \
	{                                                              \
	    __m128i win, dr, di;   \
	    win = _mm_setr_epi16(                                         \
			    wnd[i_dst##_0_rev0],                          \
			    wnd[i_dst##_1_rev0],                          \
			    wnd[i_dst##_2_rev0],                          \
			    wnd[i_dst##_3_rev0],                          \
			    wnd[i_dst##_0_rev1],                          \
			    wnd[i_dst##_1_rev1],                          \
			    wnd[i_dst##_2_rev1],                          \
			    wnd[i_dst##_3_rev1]);                         \
	    dr = _mm_setr_epi16(                                        \
			    src_main[(i_dst##_0_rev0)],               \
			    src_main[(i_dst##_1_rev0)],               \
			    src_main[(i_dst##_2_rev0)],               \
			    src_main[(i_dst##_3_rev0)],               \
			    src_main[(i_dst##_0_rev1)],               \
			    src_main[(i_dst##_1_rev1)],               \
			    src_main[(i_dst##_2_rev1)],               \
			    src_main[(i_dst##_3_rev1)]);              \
	    di = _mm_setr_epi16(                                        \
			    src_aux[(i_dst##_0_rev0)],           \
			    src_aux[(i_dst##_1_rev0)],           \
			    src_aux[(i_dst##_2_rev0)],           \
			    src_aux[(i_dst##_3_rev0)],           \
			    src_aux[(i_dst##_0_rev1)],           \
			    src_aux[(i_dst##_1_rev1)],           \
			    src_aux[(i_dst##_2_rev1)],           \
			    src_aux[(i_dst##_3_rev1)]);          \
	    dr = _mm_mulhi_epi16_vis(win, dr);                           \
	    di = _mm_mulhi_epi16_vis(win, di);                           \
	    dr = _mm_add_epi16(dr, dr); \
	    di = _mm_add_epi16(di, di); \
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst), dr);        \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst), di);        \
	}

/* *********************************************************** */

#define	NS_LOAD_R_W_SSE2(dst, src_main, src_aux, wnd, i_dst, fzeroVal)         \
	{                                                                 \
	    mlib_s32 shift = 16 - 1 - scaleup; \
	    __m128i win, dr, di, tmp0, tmp1, tmp0h, tmp0l, tmp1h, tmp1l,  \
		tmp2h, tmp2l, tmp3h, tmp3l, rhr, rlr, rhi, rli;   \
	    win = _mm_setr_epi16(                                         \
			    wnd[i_dst##_0_rev0],                          \
			    wnd[i_dst##_1_rev0],                          \
			    wnd[i_dst##_2_rev0],                          \
			    wnd[i_dst##_3_rev0],                          \
			    wnd[i_dst##_0_rev1],                          \
			    wnd[i_dst##_1_rev1],                          \
			    wnd[i_dst##_2_rev1],                          \
			    wnd[i_dst##_3_rev1]);                         \
	    dr = _mm_setr_epi16(                                        \
			    src_main[(i_dst##_0_rev0)],               \
			    src_main[(i_dst##_1_rev0)],               \
			    src_main[(i_dst##_2_rev0)],               \
			    src_main[(i_dst##_3_rev0)],               \
			    src_main[(i_dst##_0_rev1)],               \
			    src_main[(i_dst##_1_rev1)],               \
			    src_main[(i_dst##_2_rev1)],               \
			    src_main[(i_dst##_3_rev1)]);              \
	    dr = _mm_mulhi_epi16_vis(win, dr);                           \
	    dr = _mm_add_epi16(dr, dr); \
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst), dr);       \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst),     \
			    fzeroVal_SSE2);           \
	}

/* *********************************************************** */

#define	NS_LOAD_C_SSE2(dst, src_main, src_aux, wnd, i_dst, fzeroVal)        \
	{                                                              \
	    __m128i dr, di;                                      \
	                                                               \
	    dr = _mm_setr_epi16(                                       \
			    src_main[(i_dst##_0_rev0) * 2],            \
			    src_main[(i_dst##_1_rev0) * 2],            \
			    src_main[(i_dst##_2_rev0) * 2],            \
			    src_main[(i_dst##_3_rev0) * 2],            \
			    src_main[(i_dst##_0_rev1) * 2],            \
			    src_main[(i_dst##_1_rev1) * 2],            \
			    src_main[(i_dst##_2_rev1) * 2],            \
			    src_main[(i_dst##_3_rev1) * 2]);           \
	    di = _mm_setr_epi16(                                       \
			    src_main[(i_dst##_0_rev0) * 2 + 1],        \
			    src_main[(i_dst##_1_rev0) * 2 + 1],        \
			    src_main[(i_dst##_2_rev0) * 2 + 1],        \
			    src_main[(i_dst##_3_rev0) * 2 + 1],        \
			    src_main[(i_dst##_0_rev1) * 2 + 1],        \
			    src_main[(i_dst##_1_rev1) * 2 + 1],        \
			    src_main[(i_dst##_2_rev1) * 2 + 1],        \
			    src_main[(i_dst##_3_rev1) * 2 + 1]);       \
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst), dr);  \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst), di);  \
	}

/* *********************************************************** */

#define	NS_LOAD_C_OPT_SSE2(dst, src_main, src_aux, wnd, i_dst, fzeroVal)   \
	{                                                                  \
	    __m128i r0, r1, r2, r3, dr0, di0, r4, r5, r6, r7, dr1, di1;   \
	    mlib_s32 *psrc = (mlib_s32 *)src_main; \
	                                                                   \
	    dr0 = _mm_setr_epi32( \
			    psrc[i_dst##_0_rev0], \
			    psrc[i_dst##_0_rev1], \
			    psrc[i_dst##_2_rev0], \
			    psrc[i_dst##_2_rev1]); \
	    di0 = _mm_setr_epi32( \
			    psrc[i_dst##_1_rev0], \
			    psrc[i_dst##_1_rev1], \
			    psrc[i_dst##_3_rev0], \
			    psrc[i_dst##_3_rev1]); \
	    dr1 = _mm_setr_epi32( \
			    psrc[i_dst##_0_rev0 + 1], \
			    psrc[i_dst##_0_rev1 + 1], \
			    psrc[i_dst##_2_rev0 + 1], \
			    psrc[i_dst##_2_rev1 + 1]); \
	    di1 = _mm_setr_epi32( \
			    psrc[i_dst##_1_rev0 + 1], \
			    psrc[i_dst##_1_rev1 + 1], \
			    psrc[i_dst##_3_rev0 + 1], \
			    psrc[i_dst##_3_rev1 + 1]); \
		\
	    r0 = _mm_unpackhi_epi16(dr0, di0); \
	    r1 = _mm_unpacklo_epi16(dr0, di0); \
	    r2 = _mm_unpackhi_epi32(r1, r0); \
	    r3 = _mm_unpacklo_epi32(r1, r0); \
	    di0 = _mm_unpackhi_epi64(r3, r2); \
	    dr0 = _mm_unpacklo_epi64(r3, r2); \
	    r0 = _mm_unpackhi_epi16(dr1, di1); \
	    r1 = _mm_unpacklo_epi16(dr1, di1); \
	    r2 = _mm_unpackhi_epi32(r1, r0); \
	    r3 = _mm_unpacklo_epi32(r1, r0); \
	    di1 = _mm_unpackhi_epi64(r3, r2); \
	    dr1 = _mm_unpacklo_epi64(r3, r2); \
		\
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst), dr0);    \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst), di0);    \
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst + offset), dr1);\
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst + offset), di1);\
	}

#define	NS_LOAD_C_OPT_SSE2_64(dst, src_main, src_aux, wnd, i_dst, fzeroVal)   \
	{                                                                  \
	    __m128i r0, r1, r2, r3, dr0, di0, r4, r5, r6, r7, dr1, di1;   \
	    mlib_s32 *psrc = (mlib_s32 *)src_main; \
	                                                                   \
	    dr0 = _mm_setr_epi32( \
			    psrc[i_dst##_0_rev], \
			    psrc[i_dst##_0_rev + 1], \
			    psrc[i_dst##_2_rev], \
			    psrc[i_dst##_2_rev + 1]); \
	    di0 = _mm_setr_epi32( \
			    psrc[i_dst##_1_rev], \
			    psrc[i_dst##_1_rev + 1], \
			    psrc[i_dst##_3_rev], \
			    psrc[i_dst##_3_rev + 1]); \
		\
	    r0 = _mm_unpackhi_epi16(dr0, di0); \
	    r1 = _mm_unpacklo_epi16(dr0, di0); \
	    r2 = _mm_unpackhi_epi32(r1, r0); \
	    r3 = _mm_unpacklo_epi32(r1, r0); \
	    di0 = _mm_unpackhi_epi64(r3, r2); \
	    dr0 = _mm_unpacklo_epi64(r3, r2); \
		\
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst), dr0);    \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst), di0);    \
	}

/* *********************************************************** */

#define	NS_LOAD_RI_SSE2(dst, src_main, src_aux, wnd, i_dst, fzeroVal)       \
	{                                                              \
	    __m128i dr, di, rhr, rlr, rhi, rli;   \
	    dr = _mm_setr_epi16(                                        \
			    src_main[(i_dst##_0_rev0)],               \
			    src_main[(i_dst##_1_rev0)],               \
			    src_main[(i_dst##_2_rev0)],               \
			    src_main[(i_dst##_3_rev0)],               \
			    src_main[(i_dst##_0_rev1)],               \
			    src_main[(i_dst##_1_rev1)],               \
			    src_main[(i_dst##_2_rev1)],               \
			    src_main[(i_dst##_3_rev1)]);              \
	    di = _mm_setr_epi16(                                        \
			    src_aux[(i_dst##_0_rev0)],           \
			    src_aux[(i_dst##_1_rev0)],           \
			    src_aux[(i_dst##_2_rev0)],           \
			    src_aux[(i_dst##_3_rev0)],           \
			    src_aux[(i_dst##_0_rev1)],           \
			    src_aux[(i_dst##_1_rev1)],           \
			    src_aux[(i_dst##_2_rev1)],           \
			    src_aux[(i_dst##_3_rev1)]);          \
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst), dr);      \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst), di);      \
	}

/* *********************************************************** */

#define	NS_LOAD_RI_OPT_SSE2(dst, src_main, src_aux, wnd, i_dst, fzeroVal)  \
	{                                                                  \
	    __m128i tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;       \
	    __m128i r0, r1, r2, r3, dr0, di0, r4, r5, r6, r7, dr1, di1;   \
	                                                                   \
	    tmp0 = _mm_setr_epi32( \
			    *(mlib_s32 *)(src_main + i_dst##_0_rev0), \
			    *(mlib_s32 *)(src_main + i_dst##_0_rev1), \
			    *(mlib_s32 *)(src_main + i_dst##_2_rev0), \
			    *(mlib_s32 *)(src_main + i_dst##_2_rev1)); \
	    tmp1 = _mm_setr_epi32( \
			    *(mlib_s32 *)(src_main + i_dst##_1_rev0), \
			    *(mlib_s32 *)(src_main + i_dst##_1_rev1), \
			    *(mlib_s32 *)(src_main + i_dst##_3_rev0), \
			    *(mlib_s32 *)(src_main + i_dst##_3_rev1)); \
	    tmp2 = _mm_setr_epi32( \
			    *(mlib_s32 *)(src_aux + i_dst##_0_rev0), \
			    *(mlib_s32 *)(src_aux + i_dst##_0_rev1), \
			    *(mlib_s32 *)(src_aux + i_dst##_2_rev0), \
			    *(mlib_s32 *)(src_aux + i_dst##_2_rev1)); \
	    tmp3 = _mm_setr_epi32( \
			    *(mlib_s32 *)(src_aux + i_dst##_1_rev0), \
			    *(mlib_s32 *)(src_aux + i_dst##_1_rev1), \
			    *(mlib_s32 *)(src_aux + i_dst##_3_rev0), \
			    *(mlib_s32 *)(src_aux + i_dst##_3_rev1)); \
		\
	    r0 = _mm_unpackhi_epi16(tmp0, tmp1); \
	    r1 = _mm_unpacklo_epi16(tmp0, tmp1); \
	    r3 = _mm_unpackhi_epi32(r1, r0); \
	    r2 = _mm_unpacklo_epi32(r1, r0); \
	    dr0 = _mm_unpacklo_epi64(r2, r3); \
	    dr1 = _mm_unpackhi_epi64(r2, r3); \
	    r0 = _mm_unpackhi_epi16(tmp2, tmp3); \
	    r1 = _mm_unpacklo_epi16(tmp2, tmp3); \
	    r3 = _mm_unpackhi_epi32(r1, r0); \
	    r2 = _mm_unpacklo_epi32(r1, r0); \
	    di0 = _mm_unpacklo_epi64(r2, r3); \
	    di1 = _mm_unpackhi_epi64(r2, r3); \
		\
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst), dr0);    \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst), di0);    \
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst + offset), dr1);\
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst + offset), di1);\
	}

#define	NS_LOAD_RI_OPT_SSE2_64(dst, src_main, src_aux, wnd, i_dst, fzeroVal)  \
	{                                                                  \
	    __m128i tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;       \
	    __m128i r0, r1, r2, r3, dr0, di0, r4, r5, r6, r7, dr1, di1;   \
	                                                                   \
	    tmp0 = _mm_setr_epi32( \
			    *(mlib_s32 *)(src_main + i_dst##_0_rev), \
			    *(mlib_s32 *)(src_aux + i_dst##_0_rev), \
			    *(mlib_s32 *)(src_main + i_dst##_2_rev), \
			    *(mlib_s32 *)(src_aux + i_dst##_2_rev)); \
	    tmp1 = _mm_setr_epi32( \
			    *(mlib_s32 *)(src_main + i_dst##_1_rev), \
			    *(mlib_s32 *)(src_aux + i_dst##_1_rev), \
			    *(mlib_s32 *)(src_main + i_dst##_3_rev), \
			    *(mlib_s32 *)(src_aux + i_dst##_3_rev)); \
		\
	    r0 = _mm_unpackhi_epi16(tmp0, tmp1); \
	    r1 = _mm_unpacklo_epi16(tmp0, tmp1); \
	    dr0 = _mm_unpacklo_epi32(r1, r0); \
	    di0 = _mm_unpackhi_epi32(r1, r0); \
		\
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst), dr0);    \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst), di0);    \
	}

/* *********************************************************** */

#define	NS_LOAD_R_SSE2(dst, src_main, src_aux, wnd, i_dst, fzeroVal)        \
	{                                                              \
	    __m128i dr; \
	                                                                   \
	    dr = _mm_setr_epi16( \
			    src_main[i_dst##_0_rev0], \
			    src_main[i_dst##_1_rev0], \
			    src_main[i_dst##_2_rev0], \
			    src_main[i_dst##_3_rev0], \
			    src_main[i_dst##_0_rev1], \
			    src_main[i_dst##_1_rev1], \
			    src_main[i_dst##_2_rev1], \
			    src_main[i_dst##_3_rev1]); \
		\
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst), dr);    \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst),         \
			    fzeroVal_SSE2);    \
	}

/* *********************************************************** */

#define	NS_LOAD_R_OPT_SSE2(dst, src_main, src_aux, wnd, i_dst, fzeroVal)   \
	{                                                             \
	    __m128i tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;       \
	    __m128i r0, r1, r2, r3, dr0, di0, r4, r5, r6, r7, dr1, di1;   \
	    __m128i zeros = _mm_setzero_si128(); \
	    mlib_s32 shift = 16 - 2 - scaleup; \
	    mlib_s32 right_shift = 16 - (6 + 8); \
	                                                                   \
	    tmp0 = _mm_setr_epi32( \
			    *(mlib_s32 *)(src_main + i_dst##_0_rev0), \
			    *(mlib_s32 *)(src_main + i_dst##_0_rev1), \
			    *(mlib_s32 *)(src_main + i_dst##_1_rev0), \
			    *(mlib_s32 *)(src_main + i_dst##_1_rev1)); \
	    tmp1 = _mm_setr_epi32( \
			    *(mlib_s32 *)(src_main + i_dst##_2_rev0), \
			    *(mlib_s32 *)(src_main + i_dst##_2_rev1), \
			    *(mlib_s32 *)(src_main + i_dst##_3_rev0), \
			    *(mlib_s32 *)(src_main + i_dst##_3_rev1)); \
	    r0 = _mm_unpackhi_epi16(zeros, tmp0); \
	    r1 = _mm_unpacklo_epi16(zeros, tmp0); \
	    r2 = _mm_unpackhi_epi16(zeros, tmp1); \
	    r3 = _mm_unpacklo_epi16(zeros, tmp1); \
	/* reminder : following two sets of shifting can be combined */ \
	    r0 = _mm_srai_epi32(r0, right_shift); \
	    r1 = _mm_srai_epi32(r1, right_shift); \
	    r2 = _mm_srai_epi32(r2, right_shift); \
	    r3 = _mm_srai_epi32(r3, right_shift); \
		\
	    r0 = _mm_srai_epi32(r0, shift); \
	    r1 = _mm_srai_epi32(r1, shift); \
	    r2 = _mm_srai_epi32(r2, shift); \
	    r3 = _mm_srai_epi32(r3, shift); \
		\
	    dr1 = _mm_packs_epi32(r0, r2); \
	    dr0 = _mm_packs_epi32(r1, r3); \
		\
	    r0 = _mm_unpackhi_epi16(dr0, dr1); \
	    r1 = _mm_unpacklo_epi16(dr0, dr1); \
	    r2 = _mm_unpackhi_epi32(r1, r0); \
	    r3 = _mm_unpacklo_epi32(r1, r0); \
	    dr1 = _mm_unpackhi_epi64(r3, r2); \
	    dr0 = _mm_unpacklo_epi64(r3, r2); \
		\
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst), dr0);    \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst),          \
			    fzeroVal_SSE2);    \
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst + offset), dr1);\
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst + offset),    \
			    fzeroVal_SSE2);    \
	}

#define	NS_LOAD_R_OPT_SSE2_64(dst, src_main, src_aux, wnd, i_dst, fzeroVal)   \
	{                                                             \
	    __m128i tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;       \
	    __m128i r0, r1, r2, r3, dr0, di0, r4, r5, r6, r7, dr1, di1;   \
	    __m128i zeros = _mm_setzero_si128(); \
	    mlib_s32 shift = 16 - 2 - scaleup; \
	    mlib_s32 right_shift = 16 - (6 + 8); \
	                                                                   \
	    tmp0 = _mm_setr_epi32( \
			    *(mlib_s32 *)(src_main + i_dst##_0_rev), \
			    *(mlib_s32 *)(src_main + i_dst##_2_rev), \
			    *(mlib_s32 *)(src_main + i_dst##_1_rev), \
			    *(mlib_s32 *)(src_main + i_dst##_3_rev)); \
	    r0 = _mm_unpackhi_epi16(zeros, tmp0); \
	    r1 = _mm_unpacklo_epi16(zeros, tmp0); \
	/* reminder : following two sets of shifting can be combined */ \
	    r0 = _mm_srai_epi32(r0, right_shift); \
	    r1 = _mm_srai_epi32(r1, right_shift); \
		\
	    r0 = _mm_srai_epi32(r0, shift); \
	    r1 = _mm_srai_epi32(r1, shift); \
		\
	    dr0 = _mm_packs_epi32(r0, r1); \
		\
	    r0 = _mm_unpackhi_epi16(dr0, _mm_slli_si128(dr0, 8)); \
	    dr0 = _mm_unpackhi_epi32(_mm_slli_si128(r0, 8), r0); \
		\
	    _mm_storeu_si128((__m128i *)(dst##_r + i_dst), dr0);    \
	    _mm_storeu_si128((__m128i *)(dst##_i + i_dst),          \
			    fzeroVal_SSE2);    \
	}

/* *********************************************************** */

#define	LOOP_REV_SSE2(leng, ord_arr, nnn, ppp, dst, src_main,                  \
	src_aux, wnd, load_macro, sc)                                     \
	{                                                                 \
	    mlib_s32 kkk, hgh, low, shift = 5 + leng - ord_arr, o_5 =     \
		ord_arr - 5;                                              \
	    mlib_s32 rev_1 = 1 << o_5, rev_2 = 2 << o_5, rev_3 =          \
		3 << o_5;                                                 \
	    for (kkk = 0; kkk < 8; )                                      \
		ppp[kkk++] <<= o_5;                                       \
	    for (kkk = hgh = 0; kkk < nnn; hgh++) {                    \
		mlib_s32 hgh_rev0 = REV_INDX(2 * hgh, shift);                  \
		mlib_s32 hgh_rev1 = REV_INDX(2 * hgh + 1, shift);              \
		mlib_s32 kkk_0_rev0 = hgh_rev0;                             \
		mlib_s32 kkk_0_rev1 = hgh_rev1;                             \
	                                                                  \
		for (low = 0; low < 8; kkk += 8) {                        \
		    mlib_s32 kkk_3_rev0 = kkk_0_rev0 | rev_3, kkk_2_rev0 =   \
			kkk_0_rev0 | rev_2, kkk_1_rev0 =                    \
			kkk_0_rev0 | rev_1;                                \
		    mlib_s32 kkk_3_rev1 = kkk_0_rev1 | rev_3, kkk_2_rev1 =   \
			kkk_0_rev1 | rev_2, kkk_1_rev1 =                    \
			kkk_0_rev1 | rev_1;                                \
		    sc##load_macro##_SSE2(dst, src_main, src_aux, wnd, kkk,   \
			fzeroVal);                                        \
		    low ++; \
		    kkk_0_rev0 = ppp[low] | hgh_rev0;                     \
		    kkk_0_rev1 = ppp[low] | hgh_rev1;                     \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

#define	LOOP_REV_OPT_SSE2(leng, ord_arr, nnn, ppp, dst, src_main,              \
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
	if (nnn != 64) { \
	    for (kkk = hgh = 0; kkk < (nnn / 2); hgh++) {                   \
		mlib_s32 hgh_rev0 = REV_INDX(2 * hgh, shift);                  \
		mlib_s32 hgh_rev1 = REV_INDX(2 * hgh + 1, shift);              \
		mlib_s32 kkk_0_rev0 = hgh_rev0;                             \
		mlib_s32 kkk_0_rev1 = hgh_rev1;                             \
	                                                                  \
		for (low = 0; low < 8; kkk += 8) {                        \
		    mlib_s32 kkk_3_rev0 = kkk_0_rev0 | rev_3, kkk_2_rev0 =   \
			kkk_0_rev0 | rev_2, kkk_1_rev0 =                    \
			kkk_0_rev0 | rev_1;                                \
		    mlib_s32 kkk_3_rev1 = kkk_0_rev1 | rev_3, kkk_2_rev1 =   \
			kkk_0_rev1 | rev_2, kkk_1_rev1 =                    \
			kkk_0_rev1 | rev_1;                                \
		    sc##load_macro##_OPT_SSE2(dst, src_main, src_aux,          \
			wnd, kkk, fzeroVal);                              \
		    low ++; \
		    kkk_0_rev0 = ppp[low] | hgh_rev0;                     \
		    kkk_0_rev1 = ppp[low] | hgh_rev1;                     \
		}                                                         \
	    }                                                             \
	} else { \
		kkk = hgh = 0; \
		mlib_s32 hgh_rev = REV_INDX(hgh, shift);                  \
		mlib_s32 kkk_0_rev = hgh_rev;                             \
	                                                                  \
		for (low = 0; low < 8; kkk += 8) {                        \
		    mlib_s32 kkk_3_rev = kkk_0_rev | rev_3, kkk_2_rev =   \
			kkk_0_rev | rev_2, kkk_1_rev =                    \
			kkk_0_rev | rev_1;                                \
		    sc##load_macro##_OPT_SSE2_64(dst, src_main, src_aux,  \
			wnd, kkk, fzeroVal);                              \
		    kkk_0_rev = ppp[++low] | hgh_rev;                     \
		}                                                         \
	} \
	}

/* *********************************************************** */

#define	LOOP_REV_OPT2_SSE2(leng, ord_arr, nnn, ppp, dst, src_main,             \
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
	if ((nnn / 4) > 32) { \
	    for (kkk = hgh = 0; kkk < nnn / 4; hgh++) {                   \
		mlib_s32 hgh_rev0 = REV_INDX(2 * hgh, shift);                  \
		mlib_s32 hgh_rev1 = REV_INDX(2 * hgh + 1, shift);             \
		mlib_s32 kkk_0_rev0 = hgh_rev0;                             \
		mlib_s32 kkk_0_rev1 = hgh_rev1;                             \
	                                                                  \
		for (low = 0; low < 8; kkk += 8) {                        \
		    mlib_s32 kkk_3_rev0 = kkk_0_rev0 | rev_3, kkk_2_rev0 =   \
			kkk_0_rev0 | rev_2, kkk_1_rev0 =                    \
			kkk_0_rev0 | rev_1;                                \
		    mlib_s32 kkk_3_rev1 = kkk_0_rev1 | rev_3, kkk_2_rev1 =   \
			kkk_0_rev1 | rev_2, kkk_1_rev1 =                    \
			kkk_0_rev1 | rev_1;                                \
		    sc##load_macro##_OPT2_SSE2(dst, src_main,                  \
			src_aux, wnd, kkk, fzeroVal);                     \
		    low ++; \
		    kkk_0_rev0 = ppp[low] | hgh_rev0;                     \
		    kkk_0_rev1 = ppp[low] | hgh_rev1;                     \
		}                                                         \
	    }                                                             \
	} else { \
		kkk = hgh = 0; \
		mlib_s32 hgh_rev = REV_INDX(hgh, shift);                  \
		mlib_s32 kkk_0_rev = hgh_rev;                             \
	                                                                  \
		for (low = 0; low < 8; kkk += 8) {                        \
		    mlib_s32 kkk_3_rev = kkk_0_rev | rev_3, kkk_2_rev =   \
			kkk_0_rev | rev_2, kkk_1_rev =                    \
			kkk_0_rev | rev_1;                                \
		    sc##load_macro##_OPT2_SSE2_64(dst, src_main,          \
				    src_aux, wnd, kkk, fzeroVal);         \
		    kkk_0_rev = ppp[++low] | hgh_rev;                     \
		}                                                         \
	} \
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

/*  code to perform bit reversion at all orders regarding load_macro  */
/*  if( ord_arr > 5 + REV_NBITS ) hope never come here  */
#define	LOAD_BIT_REV_SSE2(ord_arr, dst, src_main, src_aux, wnd,          \
	load_macro, sc)                                             \
	{                                                           \
	    mlib_s32 CONST nnn = 1 << ord_arr;                      \
	    deal_v_t *CONST dst##_r = dst, *CONST dst##_i =         \
		dst + nnn;                                          \
	    mlib_s32 ppp[9] = {                                     \
		0, 16, 8, 24, 4, 20, 12, 28                         \
	    };                                                      \
	    LOOP_REV_SSE2(REV_NBITS, ord_arr, nnn, ppp, dst, src_main,   \
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

#define	LOAD_BIT_REV_OPT_SSE2(ord_arr, dst, src_main, src_aux,               \
	wnd, load_macro, sc)                                            \
	{                                                               \
	    mlib_s32 CONST nnn = 1 << ord_arr;                          \
	    deal_v_t *CONST dst##_r = dst, *CONST dst##_i =             \
		dst + nnn;                                              \
	    mlib_s32 ppp[9] = {                                         \
		0, 16, 8, 24, 4, 20, 12, 28                             \
	    };                                                          \
	    LOOP_REV_OPT_SSE2(REV_NBITS, ord_arr, nnn, ppp, dst, src_main,   \
		src_aux, wnd, load_macro, sc);                          \
	}

/* *********************************************************** */

#define	LOAD_BIT_REV_OPT2_SSE2(ord_arr, dst, src_main, src_aux,               \
	wnd, load_macro, sc)                                             \
	{                                                                \
	    mlib_s32 CONST nnn = 1 << ord_arr;                           \
	    deal_v_t *CONST dst##_r = dst, *CONST dst##_i =              \
		dst + nnn;                                               \
	    mlib_s32 ppp[9] = {                                          \
		0, 16, 8, 24, 4, 20, 12, 28                              \
	    };                                                           \
	    LOOP_REV_OPT2_SSE2(REV_NBITS, ord_arr, nnn, ppp, dst, src_main,   \
		src_aux, wnd, load_macro, sc);                           \
	}

/* *********************************************************** */

static void
mlib_s_fftDW_LdBr_C_W(
    mlib_s32 CONST order,
    mlib_s32 CONST scaleup,
    deal_v_t *CONST pal128data,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window)
{
	if (scaleup > 0) {
		LOAD_BIT_REV_SSE2(order, pal128data, src_main, src_aux, window,
		    LOAD_C_W, SC_);
	} else {
		LOAD_BIT_REV_SSE2(order, pal128data, src_main, src_aux, window,
		    LOAD_C_W, NS_);
	}
}
/* *********************************************************** */

static void
mlib_s_fftDW_LdBr_C(
    mlib_s32 CONST order,
    mlib_s32 CONST scaleup,
    deal_v_t *CONST pal128data,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window)
{
	if (scaleup > 0) {
		if (order < 6 || (((mlib_addr)(src_main) & 0x7) != 0) ||
		    (((mlib_addr)(src_aux) & 0x7) != 0)) {
			LOAD_BIT_REV_SSE2(order, pal128data, src_main, src_aux,
			    window, LOAD_C, SC_);
		} else {
			LOAD_BIT_REV_OPT_SSE2(order, pal128data, src_main,
					src_aux, window, LOAD_C, SC_);
		}
	} else {
		if (order < 6 || (((mlib_addr)(src_main) & 0x7) != 0) ||
		    (((mlib_addr)(src_aux) & 0x7) != 0)) {
			LOAD_BIT_REV_SSE2(order, pal128data, src_main, src_aux,
			    window, LOAD_C, NS_);
		} else {
			LOAD_BIT_REV_OPT_SSE2(order, pal128data, src_main,
					src_aux, window, LOAD_C, NS_);
		}
	}
}

/* *********************************************************** */

static void
mlib_s_fftDW_LdBr_RI_W(
    mlib_s32 CONST order,
    mlib_s32 CONST scaleup,
    deal_v_t *CONST pal128data,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window)
{
	if (scaleup > 0) {
		LOAD_BIT_REV_SSE2(order, pal128data, src_main, src_aux, window,
		    LOAD_RI_W, SC_);
	} else {
		LOAD_BIT_REV_SSE2(order, pal128data, src_main, src_aux, window,
		    LOAD_RI_W, NS_);
	}
}

/* *********************************************************** */

static void
mlib_s_fftDW_LdBr_RI(
    mlib_s32 CONST order,
    mlib_s32 CONST scaleup,
    deal_v_t *CONST pal128data,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window)
{
	if (scaleup > 0) {
		if (order < 6 || (((mlib_addr)(src_main) & 0x7) != 0) ||
		    (((mlib_addr)(src_aux) & 0x7) != 0)) {
			LOAD_BIT_REV_SSE2(order, pal128data, src_main, src_aux,
			    window, LOAD_RI, SC_);
		} else {
			LOAD_BIT_REV_OPT_SSE2(order, pal128data, src_main,
					src_aux, window, LOAD_RI, SC_);
		}
	} else {
		if (order < 6 || (((mlib_addr)(src_main) & 0x7) != 0) ||
		    (((mlib_addr)(src_aux) & 0x7) != 0)) {
			LOAD_BIT_REV_SSE2(order, pal128data, src_main, src_aux,
			    window, LOAD_RI, NS_);
		} else {
			LOAD_BIT_REV_OPT_SSE2(order, pal128data, src_main,
					src_aux, window, LOAD_RI, NS_);
		}
	}
}

/* *********************************************************** */

static void
mlib_s_fftDW_LdBr_R_W(
    mlib_s32 CONST order,
    mlib_s32 CONST scaleup,
    deal_v_t *CONST pal128data,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window)
{
	__m128i fzeroVal_SSE2 = _mm_setzero_si128();

	if (scaleup > 0) {
		LOAD_BIT_REV_SSE2(order, pal128data, src_main, src_aux, window,
		    LOAD_R_W, SC_);
	} else {
		LOAD_BIT_REV_SSE2(order, pal128data, src_main, src_aux, window,
		    LOAD_R_W, NS_);
	}
}

/* *********************************************************** */

static void
mlib_s_fftDW_LdBr_R(
    mlib_s32 CONST order,
    mlib_s32 CONST scaleup,
    deal_v_t *CONST pal128data,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window)
{
	__m128i fzeroVal_SSE2 = _mm_setzero_si128();

	if (scaleup > 0) {
		if (order < 6 || (((mlib_addr)(src_main) & 0x7) != 0) ||
		    (((mlib_addr)(src_aux) & 0x7) != 0)) {
			LOAD_BIT_REV_SSE2(order, pal128data, src_main, src_aux,
			    window, LOAD_R, SC_);
		} else {
			if (order == 6) {
				LOAD_BIT_REV_OPT_SSE2(order, pal128data,
					src_main, src_aux, window, LOAD_R, SC_);
			} else {
				LOAD_BIT_REV_OPT2_SSE2(order, pal128data,
					src_main, src_aux, window, LOAD_R, SC_);
			}
		}
	} else {
		if (order < 6 || (((mlib_addr)(src_main) & 0x7) != 0) ||
		    (((mlib_addr)(src_aux) & 0x7) != 0)) {
			LOAD_BIT_REV_SSE2(order, pal128data, src_main, src_aux,
					window, LOAD_R, NS_);
		} else {
			LOAD_BIT_REV_OPT_SSE2(order, pal128data, src_main,
					src_aux, window, LOAD_R, NS_);
		}
	}
}

/* *********************************************************** */

static void
mlib_s_fftDW_GetData(
    deal_v_t *CONST pal128data,
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
			mlib_s_fftDW_LdBr_C_W(order, scaleup, pal128data,
			    src_main, src_aux, window);
		} else {
			mlib_s_fftDW_LdBr_C(order, scaleup,
				pal128data, src_main, src_aux, window);
		}
	} else if (data_info_flags & eSrcReIm) {
		if (window) {
			mlib_s_fftDW_LdBr_RI_W(order, scaleup, pal128data,
			    src_main, src_aux, window);
		} else {
			mlib_s_fftDW_LdBr_RI(order, scaleup, pal128data,
			    src_main, src_aux, window);
		}
	} else {
		if (window) {
			mlib_s_fftDW_LdBr_R_W(order, scaleup, pal128data,
			    src_main, src_aux, window);
		} else {
			mlib_s_fftDW_LdBr_R(order, scaleup,
				pal128data, src_main, src_aux, window);
		}
	}
}

#define	RIGHT_SHIFT(x)	(((x) + adder) >> shift)

#define	LEFT_SHIFT(x)	((x) << scaleup)

static void
mlib_s_fftDW_GetData_order5(
    deal_v_t *CONST pal128data,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST scaleup,
    mlib_s32 CONST data_info_flags)
{
	mlib_s32 shift = 15 - scaleup;
	mlib_s32 adder = (1 << (shift - 1));
/*  initialize deal buffer & serve window  */
	if (data_info_flags & eSrcCmplx) {
		if (window) {
			for (int i = 0; i < 32; i++) {
				pal128data[i] = RIGHT_SHIFT(
					src_main[2 * i] * window[i]);
				pal128data[i + 32] = RIGHT_SHIFT(
					src_main[2 * i + 1] * window[i]);
			}
		} else {
			for (int i = 0; i < 32; i++) {
				pal128data[i] = LEFT_SHIFT(src_main[2 * i]);
				pal128data[i + 32] =
					LEFT_SHIFT(src_main[2 * i + 1]);
			}
		}
	} else if (data_info_flags & eSrcReIm) {
		if (window) {
			for (int i = 0; i < 32; i++) {
				pal128data[i] =
					RIGHT_SHIFT(src_main[i] * window[i]);
				pal128data[i + 32] =
					RIGHT_SHIFT(src_aux[i] * window[i]);
			}
		} else {
			for (int i = 0; i < 32; i++) {
				pal128data[i] = LEFT_SHIFT(src_main[i]);
				pal128data[i + 32] = LEFT_SHIFT(src_aux[i]);
			}
		}
	} else {
		if (window) {
			/* reminder: do we need set imaginary to zero ? */
			for (int i = 0; i < 32; i++) {
				pal128data[i] =
					RIGHT_SHIFT(src_main[i] * window[i]);
				pal128data[i + 32] = 0;
			}
		} else {
			/* reminder: do we need set imaginary to zero ? */
			for (int i = 0; i < 32; i++) {
				pal128data[i] = LEFT_SHIFT(src_main[i]);
				pal128data[i + 32] = 0;
			}
		}
	}

	mlib_d64 *p = (mlib_d64 *)pal128data;
	mlib_d64 tmp;
	tmp = p[1];
	p[1] = p[4];
	p[4] = tmp;
	tmp = p[3];
	p[3] = p[6];
	p[6] = tmp;
	p += 8;
	tmp = p[1];
	p[1] = p[4];
	p[4] = tmp;
	tmp = p[3];
	p[3] = p[6];
	p[6] = tmp;
}


/* *********************************************************** */

static mlib_status
mlib_s_SignalMerge2_S16(
    mlib_s16 *colors,
    mlib_s16 *color1,
    mlib_s16 *color2,
    mlib_s32 n)
{
	/* reminder: need to accelerate by SSE2 */
	for (int i = 0; i < n; i++) {
		colors[2 * i] = color1[i];
		colors[2 * i + 1] = color2[i];
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

static mlib_status
mlib_s_SignalCopy_S16(
	mlib_s16 *dst,
	mlib_s16 *src,
	int nn)
{
	/* reminder: need to be accelerated by SSE2 */
	for (int i = 0; i < nn; i++) {
		dst[i] = src[i];
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

static void
mlib_s_fftDW_PutData(
    deal_v_t *CONST pal128data,
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s32 CONST order,
    mlib_s32 CONST data_info_flags)
{
	mlib_s32 CONST nn = 1 << order;
	deal_v_t *CONST pal128data_r = pal128data;
	deal_v_t *CONST pal128data_i = pal128data + nn;

/*  saturate & put results into destination  */
	if (data_info_flags & eDstCmplx) {
		mlib_s_SignalMerge2_S16((void *)dst_main,
		    (void *)pal128data_r, (void *)pal128data_i, nn);
	} else {
		mlib_s_SignalCopy_S16((void *)dst_main,
				(void *)pal128data_r, nn);

		if (data_info_flags & eDstReIm) {
			mlib_s_SignalCopy_S16((void *)dst_aux,
					(void *)pal128data_i, nn);
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
mlib_s_fftKernel(
    mlib_s16 *pdata,
    mlib_s32 order,
    mlib_s32 isign,
    mlib_s32 DIVtype)
{
	if (order == 5) {
		order = 6;
	}

	mlib_s16 *data_r = pdata;
	mlib_s16 *data_i = data_r + (1 << order);
	void *tbl_high = data_i + (1 << order);
	__m128i *tbl;
	mlib_s32 layer, slayer;
	mlib_u8 div_flag[MAX_ORDER];
	type_fftK_2Layers mlib_s_fftK_2Layers_Div;

	mlib_s_fftK_2Layers_Div = (isign > 0) ? mlib_s_fftK_2Layers_P_Div
	    : mlib_s_fftK_2Layers_M_Div;

	switch (DIVtype) {
	case 0:
		if (order < 7) {
			mlib_s_fftK_3LayersFirst_nod1(data_r, data_i, order,
			    isign);
			slayer = order / 2 + 1;
		} else {
			mlib_s_fftK_3LayersFirst_nod3(data_r, data_i, order,
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
		mlib_s_fftK_3LayersFirst(data_r, data_i, order, isign);
		for (layer = 4; layer <= order; layer++) {
			div_flag[layer] = 1;
		}
		break;

	case 2:
		mlib_s_fftK_3LayersFirst(data_r, data_i, order, isign);
		for (layer = 4; layer <= (order - 2); layer++) {
			div_flag[layer] = layer & 1;
		}
		div_flag[order - 1] = 1;
		div_flag[order] = 1;
		break;

	}

	tbl = (isign > 0) ? (void *)mlib_s_fftK_WTable_1_P
	    : (void *)mlib_s_fftK_WTable_1_M;

	if (order > FFT_VIS_TBL_ORDER) {
		mlib_s_fftK_CalcTbl(tbl_high, order, isign);
	}

	for (layer = 4; layer <= order; ) {
		mlib_s32 dflag = div_flag[layer];
		mlib_s32 dflag1 = div_flag[layer + 1];

/* 2 layers */
		if (layer < FFT_VIS_TBL_ORDER && layer < order && dflag1) {
			mlib_s32 ddflag =
			    (dflag) ? BOTH_LAYERS_DIV : ONE_LAYER_DIV;
			mlib_s_fftK_2Layers_Div(data_r, data_i, order, layer,
			    ddflag);
			layer += 2;
		} else {
/* 1 layer */
			if (layer > FFT_VIS_TBL_ORDER) {
				tbl = tbl_high;
			}
			if (dflag) {
				mlib_s_fftK_Layer_Div2(data_r, data_i, order,
				    layer, tbl);
			} else {
				mlib_s_fftK_Layer(data_r, data_i, order, layer,
				    tbl);
			}
			layer++;
		}
	}
}

/* *********************************************************** */

mlib_status
mlib_s_fftDispatcherW(
    mlib_s16 *CONST dst_main,
    mlib_s16 *CONST dst_aux,
    mlib_s16 *CONST src_main,
    mlib_s16 *CONST src_aux,
    mlib_s16 *CONST window,
    mlib_s32 CONST order,
    mlib_s32 CONST data_info_flags,
    mlib_s32 FFTtype)
{
	__m128i al128_stack_buffer[256];
	deal_v_t *pal128 = (deal_v_t *)al128_stack_buffer;
	deal_v_t *p_malloced = NULL;
	size_t size2deal = sizeof (deal_v_t) << (order + 1);
	mlib_s32 isign =
	    (data_info_flags & eInversFFT) ? INVERS_FFT : DIRECT_FFT;
	mlib_s32 DIVtype, scaleup;

	if (order > FFT_VIS_TBL_ORDER) {
		size2deal += (4 << order);
	}

	if (size2deal > sizeof (al128_stack_buffer)) {
		p_malloced = (deal_v_t *)__mlib_malloc(size2deal);
		if (p_malloced == NULL) {
			return (MLIB_FAILURE);
		}
		pal128 = p_malloced;
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

	if (order != 5) {
		mlib_s_fftDW_GetData(pal128, src_main, src_aux, window,
			order, scaleup, data_info_flags);
	} else {
		mlib_s_fftDW_GetData_order5(pal128, src_main, src_aux,
			window, order, scaleup, data_info_flags);
		mlib_d64 *pal64 = (mlib_d64 *)pal128;
		mlib_d64 tmp64[16];
		for (int k = 0; k < 16; k++) {
			tmp64[k] = pal64[k];
		}
		for (int i = 0; i < 2; i++) {
			for (int k = 0; k < 8; k++) {
				pal64[2 * k] = tmp64[k + i * 8];
				pal64[2 * k + 1] = 0.0;
			}
			pal64 += 16;
		}
	}

	mlib_s_fftKernel(pal128, order, isign, DIVtype);

	if (order == 5) {
		mlib_d64 *pal64 = (mlib_d64 *)pal128;
		for (int k = 8; k < 16; k++) {
			pal64[k] = pal64[k + 8];
		}

		if (DIVtype == 1) {
			for (int k = 0; k < 128; k++) {
				pal128[k] <<= 1;
			}
		}
	}

	mlib_s_fftDW_PutData(pal128, dst_main, dst_aux, order, data_info_flags);

	if (p_malloced) {
		__mlib_free(p_malloced);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
