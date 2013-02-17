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

#pragma ident	"@(#)mlib_SignalFFT_D64Disp_32.c	9.4	07/10/09 SMI"

/* *********************************************************** */

/* DISPATCHER */

/* *********************************************************** */

/*
 * FUNCTIONS
 *  NOTE: FFT below means F(ast)F(ourier)T(ransform)
 *      mlib_c_fftDispatcher[_D64|_D64C|_D64_D64|_D64C_D64C|
 *                           _D64C_D64|_D64_D64C]
 *                            - dispatches various FFT functions:
 *                              with double format data: creates
 *                              [ dynamically ] working buffer,
 *                              fills it by source data, calls kernel FFT
 *                              function ( direct or inverse ) and stores
 *                              results in destination array(s)
 *
 *
 * SYNOPSIS
 *      mlib_status mlib_c_fftDispatcher[_D64|_D64C|_D64_D64|_D64C_D64C|
 *                                       _D64C_D64|_D64_D64C]
 *                  (*data_t *CONST  dst_main,
 *                    data_t *CONST  dst_aux,
 *                    data_t *CONST  src_main,
 *                    data_t *CONST  src_aux,
 *                    mlib_s32 CONST order,
 *                    mlib_s32 CONST shiftscale,
 *                    mlib_s32 CONST isign);
 *
 *
 * ARGUMENTS
 *      Note:      dst_main, dst_aux, src_main, src_aux arguments are
 *                   interpreted depending on [_D64|_D64C|_D64_D64|_D64C_D64C|
 *                                             _D64C_D64|_D64_D64C]
 *
 *      dst_main - {
 *                   Complex destination signal array. dst_main[ 2 * i ]
 *                   hold the real parts, and dst_main[ 2 * i + 1 ] hold the
 *                   imaginary parts
 *                 }
 *                 or {
 *                   Destination signal array that hold real parts
 *                 }
 *
 *      dst_aux  - {
 *                   Destination signal array that hold imaginary
 *                 }
 *                 or {
 *                   omitted ( must be NULL )
 *                 }
 *
 *      src_main - {
 *                   Complex source signal array. src_main[ 2 * i ]
 *                   hold the real parts, and src_main[ 2 * i + 1 ] hold the
 *                   imaginary parts
 *                 }
 *                 or {
 *                   Source signal array that hold real parts
 *                 }
 *
 *      src_aux  - {
 *                   Source signal array that hold imaginary
 *                 }
 *                 or {
 *                   omitted ( must be NULL )
 *                 }
 *
 *      order    - Order of the transformation. The base-2 logarithm of the
 *                 number of data samples
 *
 *      isign
 *               - 32bit integer sign for direct/inverse FFT
 *
 *      shiftscale
 *               - 32bit integer passed to mlib_c_fftDispatcher*
 *                  to serve 1, 2, 3 FFT versions
 *
 *
 * DESCRIPTION
 *
 *      dest = kernelFFT( src )
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

#include <mlib_signal.h>
#include <mlib_SignalFFTUtil.h>

/* *********************************************************** */

#define	REV_NBITS	32

/* *********************************************************** */
#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#if REV_NBITS == 16
#pragma weak mlib_fftDispatcher_D64C_D64C = __mlib_fftDispatcher_D64C_D64C
#pragma weak mlib_fftDispatcher_D64_D64 = __mlib_fftDispatcher_D64_D64
#pragma weak mlib_fftDispatcher_D64_D64C = __mlib_fftDispatcher_D64_D64C
#pragma weak mlib_fftDispatcher_D64C_D64 = __mlib_fftDispatcher_D64C_D64
#pragma weak mlib_fftDispatcher_D64C = __mlib_fftDispatcher_D64C
#pragma weak mlib_fftDispatcher_D64 = __mlib_fftDispatcher_D64
#endif /* REV_NBITS == 16 */

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

#if REV_NBITS == 16
__typeof__(__mlib_fftDispatcher_D64C_D64C) mlib_fftDispatcher_D64C_D64C
    __attribute__((weak, alias("__mlib_fftDispatcher_D64C_D64C")));
__typeof__(__mlib_fftDispatcher_D64_D64) mlib_fftDispatcher_D64_D64
    __attribute__((weak, alias("__mlib_fftDispatcher_D64_D64")));
__typeof__(__mlib_fftDispatcher_D64_D64C) mlib_fftDispatcher_D64_D64C
    __attribute__((weak, alias("__mlib_fftDispatcher_D64_D64C")));
__typeof__(__mlib_fftDispatcher_D64C_D64) mlib_fftDispatcher_D64C_D64
    __attribute__((weak, alias("__mlib_fftDispatcher_D64C_D64")));
__typeof__(__mlib_fftDispatcher_D64C) mlib_fftDispatcher_D64C
    __attribute__((weak, alias("__mlib_fftDispatcher_D64C")));
__typeof__(__mlib_fftDispatcher_D64) mlib_fftDispatcher_D64
    __attribute__((weak, alias("__mlib_fftDispatcher_D64")));
#endif /* REV_NBITS == 16 */

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	GLOB_NAME(SUFF, BITS)	__mlib_fftDispatcher##BITS##SUFF
#define	FUNC_NAME(SUFF)	GLOB_NAME(SUFF, _32_)
#define	REV_INDX(indx, shift)	REV_INDX_32(indx, shift)

/*
 *    ATTENTION!
 *    Valid values for 'REV_NBITS' are 8, 16, 24, 32
 *    The less REV_NBITS, the faster it works,
 *    bit reversion performs properly up to (REV_NBITS + 2) order!
 */

/* *********************************************************** */

#define	PARAMS                                                  \
	data_t *CONST dst_main, data_t *CONST dst_aux,          \
	    data_t *CONST src_main, data_t *CONST src_aux,      \
	    mlib_s32 CONST order, mlib_s32 CONST shiftscale,    \
	    mlib_s32 CONST isign

/* *********************************************************** */

/* general error processing */

/* *********************************************************** */

#define	SERVE_BAD_ORDER(order)                                  \
	{                                                       \
	    if ((order) < 0 || (order) > 31)                    \
		return (MLIB_OUTOFRANGE);                       \
	}

/* *********************************************************** */

#define	SERVE_BAD_POINTER(ptr)                                  \
	{                                                       \
	    if (!(ptr))                                         \
		return (MLIB_NULLPOINTER);                      \
	}

#define	ERR_PTR_NULL	NULL

/* *********************************************************** */

/* read into deal buffer macros */
#define	LOAD_C(dst, src_main, src_aux, wnd, i_src, mult)        \
	dst##_r = (deal_t)(src_main[2 * (i_src)]);              \
	dst##_i = (deal_t)(src_main[2 * (i_src) + 1])

/* *********************************************************** */

#define	LOAD_RI(dst, src_main, src_aux, wnd, i_src, mult)       \
	dst##_r = (deal_t)(src_main[i_src]);                    \
	dst##_i = (deal_t)(src_aux[i_src])

/* *********************************************************** */

#define	LOAD_R(dst, src_main, src_aux, wnd, i_src, mult)        \
	dst##_r = (deal_t)(src_main[i_src]);                    \
	dst##_i = (deal_t)0

/* *********************************************************** */

#define	LOAD_C_MULT(dst, src_main, src_aux, wnd, i_src, mult)   \
	dst##_r = (deal_t)(src_main[2 * (i_src)]) * mult;       \
	dst##_i = (deal_t)(src_main[2 * (i_src) + 1]) * mult

/* *********************************************************** */

#define	LOAD_RI_MULT(dst, src_main, src_aux, wnd, i_src, mult)   \
	dst##_r = (deal_t)(src_main[i_src]) * mult;              \
	dst##_i = (deal_t)(src_aux[i_src]) * mult

/* *********************************************************** */

#define	LOAD_R_MULT(dst, src_main, src_aux, wnd, i_src, mult)   \
	dst##_r = (deal_t)(src_main[i_src]) * mult;             \
	dst##_i = (deal_t)0

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

#define	DCL_DST_PTR_C(pbeg, dst, shift)                         \
	dtCmplx *CONST pbeg = (dtCmplx *) (dst) + shift

/* *********************************************************** */

#define	DCL_DST_PTR_RI(pbeg, dst, shift)                        \
	deal_t *pbeg##_r = (dst##_r) + shift, *pbeg##_i =       \
	    (dst##_i) + shift

/* *********************************************************** */

/*
 *    declares and inits FOUR ptrs to dtCmplx from p_beg shifted by 1
 */
#define	DCL_4_CMPLX_PTRS_C(tag, ptr_beg)                        \
	dtCmplx *CONST tag##_0 = ptr_beg, *CONST tag##_1 =      \
	    tag##_0 + 1, *CONST tag##_2 =                       \
	    tag##_0 + 2, *CONST tag##_3 = tag##_0 + 3

/* *********************************************************** */

#define	DCL_8_CMPLX_PTRS_C(tag0, tag1, ptr_beg)                 \
	DCL_4_CMPLX_PTRS_C(tag0, ptr_beg);                      \
	DCL_4_CMPLX_PTRS_C(tag1, tag0##_0 + 4)

/* *********************************************************** */

#undef DCL_4_CMPLX_PTRS_RI
#define	DCL_4_CMPLX_PTRS_RI(tag, ptr_beg)                       \
	deal_t *CONST tag##_0__r =                              \
	    ptr_beg##_r, *CONST tag##_1__r =                    \
	    tag##_0__r + 1, *CONST tag##_2__r =                 \
	    tag##_0__r + 2, *CONST tag##_3__r =                 \
	    tag##_0__r + 3, *CONST tag##_0__i =                 \
	    ptr_beg##_i, *CONST tag##_1__i =                    \
	    tag##_0__i + 1, *CONST tag##_2__i =                 \
	    tag##_0__i + 2, *CONST tag##_3__i =                 \
	    tag##_0__i + 3

/* *********************************************************** */

#define	DCL_8_CMPLX_PTRS_RI(tag0, tag1, ptr_beg)                \
	DCL_4_CMPLX_PTRS_RI(tag0, ptr_beg);                     \
	deal_t *CONST next4_r =                                 \
	    tag0##_0##__r + 4, *CONST next4_i =                 \
	    tag0##_0##__i + 4;                                  \
	DCL_4_CMPLX_PTRS_RI(tag1, next4)

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

/* perform & store simple butterflies */
#undef STORE_SIMPLE_BUTTERFLIES
#define	STORE_SIMPLE_BUTTERFLIES(dst, src, dstag)               \
	BUTTERFLY_CMPLX_CMPLX_##dstag(dst##_0, dst##_2,         \
	    src##_0_pls_1, src##_2_pls_3);                      \
	BUTTERFLY_CMPLX_CMPLX_##dstag(dst##_1, dst##_3,         \
	    src##_0_mns_1, src##_2_mns_3)

/* *********************************************************** */

/* sequences to perform simple butt on 4 complex elems */
#define	DEAL_4_2LAYS(ptr_beg, d0, d1, iSIGNed, dstag)           \
	{                                                       \
	    DCL_CALC_SIMPLE_BUTT(c0, d0, iSIGNed);              \
	    DCL_4_CMPLX_PTRS_##dstag(d0, ptr_beg);              \
	    STORE_SIMPLE_BUTTERFLIES(d0, c0, dstag);            \
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

/* sequences to perform 1st-3layers butt on 8 sequent with step 1 */
#define	DEAL_8_3LAYS(ptr_beg, d0, d1, iSIGNed, dstag)           \
	{                                                       \
	    DCL_CALC_SIMPLE_BUTT(c0, d0, iSIGNed);              \
	    DCL_CALC_SIMPLE_BUTT(c1, d1, iSIGNed);              \
	    CALC_SIMPLE_BUTTS(d0, c0);                          \
	    CALC_SIMPLE_BUTTS(d1, c1); {                        \
		DCL_8_CMPLX_PTRS_##dstag(d0, d1, ptr_beg);      \
		DCL_CALC_3RD_LAY_BUTT(d1, iSIGNed);             \
		STORE_3RD_LAY_BUTTERFLIES(d0, d1, dstag);       \
	    }                                                   \
	}

/* *********************************************************** */

/* reversion at all orders using REV_INDX*, load_macro, deal_macro */
#define	LOOP_REV_3(ord_arr, iSIGNed, dst, src_main, src_aux,             \
	wnd, load_macro, mult, dstag)                                    \
	{                                                                \
	    mlib_s32 CONST nnn = 1 << (ord_arr), o_3 =                   \
		ord_arr - 3, shift = REV_NBITS - o_3;                    \
	    mlib_s32 iii;                                                \
	    mlib_s32 rev_1 = 4 << o_3, rev_2 = 2 << o_3, rev_3 =         \
		6 << o_3, rev_4 = 1 << o_3, rev_5 = 5 << o_3, rev_6 =    \
		3 << o_3, rev_7 = 7 << o_3;                              \
	    for (iii = iii0; iii < iii1; iii += 8) {                     \
		mlib_s32 hgh = iii >> 3;                                 \
		mlib_s32 hgh_rev = REV_INDX(hgh, shift), iii_0_rev =     \
		    hgh_rev, iii_1_rev = hgh_rev | rev_1, iii_2_rev =    \
		    hgh_rev | rev_2, iii_3_rev =                         \
		    hgh_rev | rev_3, iii_4_rev =                         \
		    hgh_rev | rev_4, iii_5_rev =                         \
		    hgh_rev | rev_5, iii_6_rev =                         \
		    hgh_rev | rev_6, iii_7_rev = hgh_rev | rev_7;        \
		deal_t d0_0_r, d0_0_i, d0_1_r, d0_1_i, d0_2_r, d0_2_i,   \
		    d0_3_r, d0_3_i, d1_0_r, d1_0_i, d1_1_r, d1_1_i,      \
		    d1_2_r, d1_2_i, d1_3_r, d1_3_i;                      \
		DCL_DST_PTR_##dstag(pbeg, dst, iii);                     \
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
		DEAL_8_3LAYS(pbeg, d0, d1, iSIGNed, dstag);              \
	    }                                                            \
	}

/* *********************************************************** */

#define	LOOP_REV_2(ord_arr, iSIGNed, dst, src_main, src_aux,             \
	wnd, load_macro, mult, dstag)                                    \
	{                                                                \
	    mlib_s32 CONST nnn = 1 << (ord_arr), o_2 =                   \
		ord_arr - 2, shift = REV_NBITS - o_2;                    \
	    mlib_s32 iii;                                                \
	    mlib_s32 rev_1 = 2 << o_2, rev_2 = 1 << o_2, rev_3 =         \
		3 << o_2;                                                \
	    for (iii = iii0; iii < iii1; iii += 4) {                     \
		mlib_s32 hgh = iii >> 2;                                 \
		mlib_s32 hgh_rev = REV_INDX(hgh, shift), iii_0_rev =     \
		    hgh_rev, iii_1_rev = hgh_rev | rev_1, iii_2_rev =    \
		    hgh_rev | rev_2, iii_3_rev = hgh_rev | rev_3;        \
		deal_t d0_0_r, d0_0_i, d0_1_r, d0_1_i, d0_2_r, d0_2_i,   \
		    d0_3_r, d0_3_i;                                      \
		DCL_DST_PTR_##dstag(pbeg, dst, iii);                     \
		load_macro(d0_0, src_main, src_aux, wnd, iii_0_rev,      \
		    mult);                                               \
		load_macro(d0_1, src_main, src_aux, wnd, iii_1_rev,      \
		    mult);                                               \
		load_macro(d0_2, src_main, src_aux, wnd, iii_2_rev,      \
		    mult);                                               \
		load_macro(d0_3, src_main, src_aux, wnd, iii_3_rev,      \
		    mult);                                               \
		DEAL_4_2LAYS(pbeg, d0, d1, iSIGNed, dstag);              \
	    }                                                            \
	}

/* *********************************************************** */

/* perform bit reversion at order 2 */
#define	DEAL_ALL_ORD_2(iSIGNed, dst, src_main, src_aux, wnd,         \
	load_macro, mult, dstag)                                     \
	{                                                            \
	    deal_t d0_0_r, d0_0_i, d0_1_r, d0_1_i, d0_2_r, d0_2_i,   \
		d0_3_r, d0_3_i;                                      \
	    load_macro(d0_0, src_main, src_aux, wnd, 0, mult);       \
	    load_macro(d0_1, src_main, src_aux, wnd, 2, mult);       \
	    load_macro(d0_2, src_main, src_aux, wnd, 1, mult);       \
	    load_macro(d0_3, src_main, src_aux, wnd, 3, mult); {     \
		DCL_DST_PTR_##dstag(pbeg, dst, 0);                   \
		DCL_4_CMPLX_PTRS_##dstag(d0, pbeg);                  \
		DCL_CALC_SIMPLE_BUTT(c0, d0, iSIGNed);               \
		STORE_SIMPLE_BUTTERFLIES(d0, c0, dstag);             \
	    }                                                        \
	}

/* *********************************************************** */

/* perform bit reversion at order 2 regarding shiftscale */
#define	DEAL_ALL_ORD_2_MULT(iSIGNed, dst, src_main, src_aux,            \
	wnd, load_macro, shiftscale, dstag, scl)                        \
	{                                                               \
	    if (shiftscale == scl) {                                    \
		DEAL_ALL_ORD_2(iSIGNed, dst, src_main, src_aux, 0,      \
		    load_macro, 1997.0, dstag);                         \
	    } else {                                                    \
		mlib_d64 CONST mult = 1. / (1 << (scl - shiftscale));   \
	                                                                \
		DEAL_ALL_ORD_2(iSIGNed, dst, src_main, src_aux, 0,      \
		    load_macro##_MULT, mult, dstag);                    \
	    }                                                           \
	}

/* *********************************************************** */

/* reversion at 3rd orders load_macro, deal_macro */
#define	DEAL_ALL_ORD_3(iSIGNed, dst, src_main, src_aux, wnd,              \
	load_macro, mult, dstag)                                          \
	{                                                                 \
	    deal_t d0_0_r, d0_0_i, d0_1_r, d0_1_i, d0_2_r, d0_2_i,        \
		d0_3_r, d0_3_i, d1_0_r, d1_0_i, d1_1_r, d1_1_i, d1_2_r,   \
		d1_2_i, d1_3_r, d1_3_i;                                   \
	    DCL_DST_PTR_##dstag(pbeg, dst, 0);                            \
	    load_macro(d0_0, src_main, src_aux, wnd, 0, mult);            \
	    load_macro(d0_1, src_main, src_aux, wnd, 4, mult);            \
	    load_macro(d0_2, src_main, src_aux, wnd, 2, mult);            \
	    load_macro(d0_3, src_main, src_aux, wnd, 6, mult);            \
	    load_macro(d1_0, src_main, src_aux, wnd, 1, mult);            \
	    load_macro(d1_1, src_main, src_aux, wnd, 5, mult);            \
	    load_macro(d1_2, src_main, src_aux, wnd, 3, mult);            \
	    load_macro(d1_3, src_main, src_aux, wnd, 7, mult);            \
	    DEAL_8_3LAYS(pbeg, d0, d1, iSIGNed, dstag);                   \
	}

/* *********************************************************** */

/* perform bit reversion at order 3 regarding shiftscale */
#define	DEAL_ALL_ORD_3_MULT(iSIGNed, dst, src_main, src_aux,            \
	wnd, load_macro, shiftscale, dstag, scl)                        \
	{                                                               \
	    if (shiftscale == scl) {                                    \
		DEAL_ALL_ORD_3(iSIGNed, dst, src_main, src_aux, 0,      \
		    load_macro, 1997.0, dstag);                         \
	    } else {                                                    \
		mlib_d64 CONST mult = 1. / (1 << (scl - shiftscale));   \
	                                                                \
		DEAL_ALL_ORD_3(iSIGNed, dst, src_main, src_aux, 0,      \
		    load_macro##_MULT, mult, dstag);                    \
	    }                                                           \
	}

/* *********************************************************** */

#define	GET_NUM_FIRST_C(first3, order)                                    \
	first3 =                                                          \
	    (order >                                                      \
	    CACHE1_ORD ? (((order - CACHE1_ORD - 1) % 3) + CACHE1_ORD +   \
	    1 - 3) : order) & 1

/* *********************************************************** */

#define	GET_NUM_FIRST_RI(first3, order)                         \
	GET_NUM_FIRST_C(first3, order)

/* *********************************************************** */

/* perform bit reversion at all orders regarding ord_arr */
#define	LOAD_BIT_REV(ord_arr, iSIGNed, dst, src_main,                     \
	src_aux, wnd, load_macro, mult, dstag)                            \
	{                                                                 \
	    mlib_s32 first3;                                              \
	                                                                  \
	    GET_NUM_FIRST_##dstag(first3, ord_arr);                       \
	    if (first3) {                                                 \
		LOOP_REV_3(ord_arr, iSIGNed, dst, src_main, src_aux, 0,   \
		    load_macro, mult, dstag);                             \
	    } else {                                                      \
		LOOP_REV_2(ord_arr, iSIGNed, dst, src_main, src_aux, 0,   \
		    load_macro, mult, dstag);                             \
	    }                                                             \
	}

/* *********************************************************** */

/* regarding shiftscale */
#define	LOAD_BIT_REV_MULT(ord_arr, iSIGNed, dst, src_main,               \
	src_aux, wnd, load_macro, shiftscale, dstag, scl)                \
	{                                                                \
	    if (shiftscale == scl) {                                     \
		LOAD_BIT_REV(ord_arr, iSIGNed, dst, src_main, src_aux,   \
		    0, load_macro, 1997.0, dstag);                       \
	    } else {                                                     \
		mlib_d64 CONST mult = 1. / (1 << (scl - shiftscale));    \
	                                                                 \
		LOAD_BIT_REV(ord_arr, iSIGNed, dst, src_main, src_aux,   \
		    0, load_macro##_MULT, mult, dstag);                  \
	    }                                                            \
	}

/* *********************************************************** */

#define	SWAP_LOAD_C(val, src, mult)                             \
	val.r = (deal_t)(src->r);                               \
	val.i = (deal_t)(src->i)

/* *********************************************************** */

#define	SWAP_LOAD_C_MULT(val, src, mult)                        \
	val.r = (deal_t)(src->r) * mult;                        \
	val.i = (deal_t)(src->i) * mult

/* *********************************************************** */

#define	SWAP_LOAD_RI(val, src, mult)                            \
	val##_r = *(src##_r);                                   \
	val##_i = *(src##_i)

/* *********************************************************** */

#define	SWAP_LOAD_RI_MULT(val, src, mult)                       \
	val##_r = *(src##_r) * mult;                            \
	val##_i = *(src##_i) * mult

/* *********************************************************** */

#define	NO_SWAP_LOAD_C(data, indx,  mult)

/* *********************************************************** */

#define	NO_SWAP_LOAD_C_MULT(data, indx, mult)                   \
	((dtCmplx *) (data) + (indx))->r *= mult;               \
	((dtCmplx *) (data) + (indx))->i *= mult

/* *********************************************************** */

#define	NO_SWAP_LOAD_RI(data, indx, mult)

/* *********************************************************** */

#define	NO_SWAP_LOAD_RI_MULT(data, indx, mult)                  \
	(*((data##_r) + (indx))) *= mult;                       \
	(*((data##_i) + (indx))) *= mult

/* *********************************************************** */

#define	SWAP_C(pdata, indx, swapindx, load_macro, mult)         \
	{                                                       \
	    dtCmplx *p0 = (dtCmplx *) (pdata) + (indx);         \
	    dtCmplx *p1 = (dtCmplx *) (pdata) + (swapindx);     \
	    dtCmplx tmp0, tmp1;                                 \
	                                                        \
	    load_macro(tmp0, p0, mult);                         \
	    load_macro(tmp1, p1, mult);                         \
	    p0->r = tmp1.r;                                     \
	    p0->i = tmp1.i;                                     \
	    p1->r = tmp0.r;                                     \
	    p1->i = tmp0.i;                                     \
	}

/* *********************************************************** */

#define	SWAP_RI(pdata, indx, swapindx, load_macro, mult)        \
	{                                                       \
	    deal_t *p0_r = (pdata##_r) + (indx), *p0_i =        \
		(pdata##_i) + (indx), *p1_r =                   \
		(pdata##_r) + (swapindx), *p1_i =               \
		(pdata##_i) + (swapindx);                       \
	    deal_t tmp0_r, tmp0_i, tmp1_r, tmp1_i;              \
	                                                        \
	    load_macro(tmp0, p0, mult);                         \
	    load_macro(tmp1, p1, mult);                         \
	    *p1_r = tmp0_r;                                     \
	    *p1_i = tmp0_i;                                     \
	    *p0_r = tmp1_r;                                     \
	    *p0_i = tmp1_i;                                     \
	}

/* *********************************************************** */

#define	BIT_REV_EVEN(ord_arr, dst, load_macro, mult, dstag)         \
	{                                                           \
	    mlib_s32 CONST ord_half = ord_arr >> 1, maxhalf =       \
		1 << ord_half;                                      \
	    mlib_s32 CONST shift_half = REV_NBITS - ord_half;       \
	    mlib_s32 l_dir;                                         \
	                                                            \
	    NO_SWAP_##load_macro(dst, 0, mult);                     \
	    for (l_dir = 1; l_dir < maxhalf; l_dir++) {             \
		mlib_s32 h_inv =                                    \
		    REV_INDX(l_dir, shift_half) << ord_half;        \
		mlib_s32 l_inv;                                     \
	                                                            \
		for (l_inv = 0; l_inv < l_dir; l_inv++) {           \
		    mlib_s32 h_dir =                                \
			REV_INDX(l_inv, shift_half) << ord_half;    \
		    mlib_s32 indx_dir = h_dir | l_dir, indx_inv =   \
			h_inv | l_inv;                              \
		    SWAP_##dstag(dst, indx_dir, indx_inv,           \
			SWAP_##load_macro, mult);                   \
		}                                                   \
		NO_SWAP_##load_macro(dst, l_dir | h_inv, mult);     \
	    }                                                       \
	}

/* *********************************************************** */

#define	BIT_REV__ODD(ord_arr, dst, load_macro, mult, dstag)            \
	{                                                              \
	    mlib_s32 CONST ord_half = ord_arr >> 1, maxhalf =          \
		1 << ord_half;                                         \
	    mlib_s32 CONST ord_half1 = ord_half + 1, middlebit =       \
		1 << ord_half;                                         \
	    mlib_s32 CONST shift_half = REV_NBITS - ord_half;          \
	    mlib_s32 l_dir;                                            \
	                                                               \
	    NO_SWAP_##load_macro(dst, 0, mult);                        \
	    NO_SWAP_##load_macro(dst, middlebit, mult);                \
	    for (l_dir = 1; l_dir < maxhalf; l_dir++) {                \
		mlib_s32 h_inv =                                       \
		    REV_INDX(l_dir, shift_half) << ord_half1;          \
		mlib_s32 l_inv;                                        \
	                                                               \
		for (l_inv = 0; l_inv < l_dir; l_inv++) {              \
		    mlib_s32 h_dir =                                   \
			REV_INDX(l_inv, shift_half) << ord_half1;      \
		    mlib_s32 indx_dir = h_dir | l_dir, indx_inv =      \
			h_inv | l_inv;                                 \
		    SWAP_##dstag(dst, indx_dir, indx_inv,              \
			SWAP_##load_macro, mult);                      \
		    indx_dir |= middlebit;                             \
		    indx_inv |= middlebit;                             \
		    SWAP_##dstag(dst, indx_dir, indx_inv,              \
			SWAP_##load_macro, mult);                      \
		}                                                      \
		NO_SWAP_##load_macro(dst, l_dir | h_inv, mult);        \
		NO_SWAP_##load_macro(dst, l_dir | h_inv | middlebit,   \
		    mult);                                             \
	    }                                                          \
	}

/* *********************************************************** */

/* inplace beginning */
#define	INPLACE_BEGINNING(ord_arr, dst, load_macro,                        \
	shiftscale, dstag)                                                 \
	{                                                                  \
	    if (ord_arr & 1) {                                             \
		if (shiftscale == ord_arr) {                               \
		    BIT_REV__ODD(ord_arr, dst, load_macro, mult, dstag);   \
		} else {                                                   \
		    mlib_d64 CONST mult =                                  \
			1. / (1 << (ord_arr - shiftscale));                \
		    BIT_REV__ODD(ord_arr, dst, load_macro##_MULT,          \
			mult, dstag);                                      \
		}                                                          \
	    } else {                                                       \
		if (shiftscale == ord_arr) {                               \
		    BIT_REV_EVEN(ord_arr, dst, load_macro, mult, dstag);   \
		} else {                                                   \
		    mlib_d64 CONST mult =                                  \
			1. / (1 << (ord_arr - shiftscale));                \
		    BIT_REV_EVEN(ord_arr, dst, load_macro##_MULT,          \
			mult, dstag);                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	LOOP_3(ord_arr, iSIGNed, dst, src_main, src_aux, wnd,              \
	load_macro, mult, dstag)                                           \
	{                                                                  \
	    mlib_s32 CONST nnn = 1 << (ord_arr);                           \
	    mlib_s32 iii;                                                  \
	                                                                   \
	    for (iii = 0; iii < nnn; iii += 8) {                           \
		deal_t d0_0_r, d0_0_i, d0_1_r, d0_1_i, d0_2_r, d0_2_i,     \
		    d0_3_r, d0_3_i, d1_0_r, d1_0_i, d1_1_r, d1_1_i,        \
		    d1_2_r, d1_2_i, d1_3_r, d1_3_i;                        \
		DCL_DST_PTR_##dstag(pbeg, dst, iii);                       \
		load_macro(d0_0, src_main, src_aux, wnd, iii, mult);       \
		load_macro(d0_1, src_main, src_aux, wnd, iii + 1, mult);   \
		load_macro(d0_2, src_main, src_aux, wnd, iii + 2, mult);   \
		load_macro(d0_3, src_main, src_aux, wnd, iii + 3, mult);   \
		load_macro(d1_0, src_main, src_aux, wnd, iii + 4, mult);   \
		load_macro(d1_1, src_main, src_aux, wnd, iii + 5, mult);   \
		load_macro(d1_2, src_main, src_aux, wnd, iii + 6, mult);   \
		load_macro(d1_3, src_main, src_aux, wnd, iii + 7, mult);   \
		DEAL_8_3LAYS(pbeg, d0, d1, iSIGNed, dstag);                \
	    }                                                              \
	}

/* *********************************************************** */

#define	LOOP_2(ord_arr, iSIGNed, dst, src_main, src_aux, wnd,              \
	load_macro, mult, dstag)                                           \
	{                                                                  \
	    mlib_s32 CONST nnn = 1 << (ord_arr);                           \
	    mlib_s32 iii;                                                  \
	                                                                   \
	    for (iii = 0; iii < nnn; iii += 4) {                           \
		deal_t d0_0_r, d0_0_i, d0_1_r, d0_1_i, d0_2_r, d0_2_i,     \
		    d0_3_r, d0_3_i;                                        \
		DCL_DST_PTR_##dstag(pbeg, dst, iii);                       \
		load_macro(d0_0, src_main, src_aux, wnd, iii, mult);       \
		load_macro(d0_1, src_main, src_aux, wnd, iii + 1, mult);   \
		load_macro(d0_2, src_main, src_aux, wnd, iii + 2, mult);   \
		load_macro(d0_3, src_main, src_aux, wnd, iii + 3, mult);   \
		DEAL_4_2LAYS(pbeg, d0, d1, iSIGNed, dstag);                \
	    }                                                              \
	}

/* *********************************************************** */

#define	LOAD_NOT_BIT_REV(ord_arr, iSIGNed, dst, src_main,             \
	src_aux, wnd, load_macro, mult, dstag)                        \
	{                                                             \
	    mlib_s32 first3;                                          \
	                                                              \
	    GET_NUM_FIRST_##dstag(first3, ord_arr);                   \
	    if (first3) {                                             \
		LOOP_3(ord_arr, iSIGNed, dst, src_main, src_aux, 0,   \
		    load_macro, mult, dstag);                         \
	    } else {                                                  \
		LOOP_2(ord_arr, iSIGNed, dst, src_main, src_aux, 0,   \
		    load_macro, mult, dstag);                         \
	    }                                                         \
	}

/* *********************************************************** */

#define	iii0  0
#define	iii1  nnn

/* *********************************************************** */

mlib_status FUNC_NAME(
    D64C_D64C) (
    data_t *CONST dst_main,
    data_t *CONST dst_aux,
    data_t *CONST src_main,
    data_t *CONST src_aux,
    mlib_s32 CONST order,
    mlib_s32 CONST shiftscale,
    mlib_s32 CONST isign)
{
	deal_t *CONST pal64 = dst_main;

	SERVE_BAD_ORDER(order);

	if (order < 4) {
		switch (order) {
		case 0: {
			    dst_main[0] = src_main[0];
			    dst_main[1] = src_main[1];
		    }

			break;
		case 1: {
			    deal_t buf0, buf1, buf2, buf3;
			    mlib_d64 CONST multscale =
				shiftscale != 1 ? 0.5 : 1.0;

/* initialise deal buffer */
			    buf0 = src_main[0];
			    buf1 = src_main[1];
			    buf2 = src_main[2];
			    buf3 = src_main[3];
/* butterfly */
			    buf2 = (buf0 += buf2) - (2.0 * buf2);
			    buf3 = (buf1 += buf3) - (2.0 * buf3);
/* scale */
/* put results into destination */
			    dst_main[0] = buf0 * multscale;
			    dst_main[1] = buf1 * multscale;
			    dst_main[2] = buf2 * multscale;
			    dst_main[3] = buf3 * multscale;
		    }

			break;
		case 2: {

			    if (isign == INVERS_FFT) {
				    DEAL_ALL_ORD_2_MULT(AS_IS, pal64, src_main,
					0, 0, LOAD_C, shiftscale, C, 2);
			    } else {
				    DEAL_ALL_ORD_2_MULT(MINUS, pal64, src_main,
					0, 0, LOAD_C, shiftscale, C, 2);
			    }
		    }

			break;
		case 3: {

			    if (isign == INVERS_FFT) {
				    DEAL_ALL_ORD_3_MULT(AS_IS, pal64, src_main,
					0, 0, LOAD_C, shiftscale, C, 3);
			    } else {
				    DEAL_ALL_ORD_3_MULT(MINUS, pal64, src_main,
					0, 0, LOAD_C, shiftscale, C, 3);
			    }
		    }

			break;
		}

		return (MLIB_SUCCESS);
	}

/* initialise deal buffer */

	if (isign == INVERS_FFT) {
		LOAD_BIT_REV_MULT(order, AS_IS, pal64, src_main, 0, 0, LOAD_C,
		    shiftscale, C, order);
	} else {
		LOAD_BIT_REV_MULT(order, MINUS, pal64, src_main, 0, 0, LOAD_C,
		    shiftscale, C, order);
	}

/* deal it */
	mlib_c_fftKernel(pal64, order, isign);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    D64_D64) (
    data_t *CONST dst_main,
    data_t *CONST dst_aux,
    data_t *CONST src_main,
    data_t *CONST src_aux,
    mlib_s32 CONST order,
    mlib_s32 CONST shiftscale,
    mlib_s32 CONST isign)
{
	mlib_d64 *pal64_r = dst_main, *pal64_i = dst_aux;

	SERVE_BAD_ORDER(order);

	if (order < 4) {
		switch (order) {
		case 0: {
			    dst_main[0] = src_main[0];
			    dst_aux[0] = src_aux[0];
		    }

			break;
		case 1: {
			    deal_t buf0, buf1, buf2, buf3;
			    mlib_d64 CONST multscale =
				shiftscale != 1 ? 0.5 : 1.0;

/* initialise deal buffer */
			    buf0 = src_main[0];
			    buf1 = src_aux[0];
			    buf2 = src_main[1];
			    buf3 = src_aux[1];
/* butterfly */
			    buf2 = (buf0 += buf2) - (2.0 * buf2);
			    buf3 = (buf1 += buf3) - (2.0 * buf3);
/* scale */
/* put results into destination */
			    dst_main[0] = buf0 * multscale;
			    dst_aux[0] = buf1 * multscale;
			    dst_main[1] = buf2 * multscale;
			    dst_aux[1] = buf3 * multscale;
		    }

			break;
		case 2: {

			    if (isign == INVERS_FFT) {
				    DEAL_ALL_ORD_2_MULT(AS_IS, pal64, src_main,
					src_aux, 0, LOAD_RI, shiftscale, RI, 2);
			    } else {
				    DEAL_ALL_ORD_2_MULT(MINUS, pal64, src_main,
					src_aux, 0, LOAD_RI, shiftscale, RI, 2);
			    }
		    }

			break;
		case 3: {

			    if (isign == INVERS_FFT) {
				    DEAL_ALL_ORD_3_MULT(AS_IS, pal64, src_main,
					src_aux, 0, LOAD_RI, shiftscale, RI, 3);
			    } else {
				    DEAL_ALL_ORD_3_MULT(MINUS, pal64, src_main,
					src_aux, 0, LOAD_RI, shiftscale, RI, 3);
			    }
		    }

			break;
		}

		return (MLIB_SUCCESS);
	}

	if (isign == INVERS_FFT) {
		LOAD_BIT_REV_MULT(order, AS_IS, pal64, src_main, src_aux, 0,
		    LOAD_RI, shiftscale, RI, order);
	} else {
		LOAD_BIT_REV_MULT(order, MINUS, pal64, src_main, src_aux, 0,
		    LOAD_RI, shiftscale, RI, order);
	}

/* deal it */

	mlib_c_fftKernel_RI(pal64_r, pal64_i, order, isign);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    D64C) (
    data_t *CONST dst_main,
    data_t *CONST dst_aux,
    data_t *CONST src_main,
    data_t *CONST src_aux,
    mlib_s32 CONST order,
    mlib_s32 CONST shiftscale,
    mlib_s32 CONST isign)
{
	deal_t *CONST pal64 = dst_main;

	SERVE_BAD_ORDER(order);

	if (order < 4) {
		switch (order) {
		case 0: {
/* nothing to do */
		    }

			break;
		case 1: {
			    deal_t buf0, buf1, buf2, buf3;
			    mlib_d64 CONST multscale =
				shiftscale != 1 ? 0.5 : 1.0;

/* initialise deal buffer */
			    buf0 = dst_main[0];
			    buf1 = dst_main[1];
			    buf2 = dst_main[2];
			    buf3 = dst_main[3];
/* butterfly */
			    buf2 = (buf0 += buf2) - (2.0 * buf2);
			    buf3 = (buf1 += buf3) - (2.0 * buf3);
/* scale */
/* put results into destination */
			    dst_main[0] = buf0 * multscale;
			    dst_main[1] = buf1 * multscale;
			    dst_main[2] = buf2 * multscale;
			    dst_main[3] = buf3 * multscale;
		    }

			break;
		case 2: {

			    if (isign == INVERS_FFT) {
				    DEAL_ALL_ORD_2_MULT(AS_IS, pal64, pal64, 0,
					0, LOAD_C, shiftscale, C, 2);
			    } else {
				    DEAL_ALL_ORD_2_MULT(MINUS, pal64, pal64, 0,
					0, LOAD_C, shiftscale, C, 2);
			    }
		    }

			break;
		case 3: {

			    if (isign == INVERS_FFT) {
				    DEAL_ALL_ORD_3_MULT(AS_IS, pal64, pal64, 0,
					0, LOAD_C, shiftscale, C, 3);
			    } else {
				    DEAL_ALL_ORD_3_MULT(MINUS, pal64, pal64, 0,
					0, LOAD_C, shiftscale, C, 3);
			    }
		    }

			break;
		}

		return (MLIB_SUCCESS);
	}

	INPLACE_BEGINNING(order, pal64, LOAD_C, shiftscale, C);

/* deal it */
	mlib_c_fftKernelInplace(pal64, order, isign);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    D64) (
    data_t *CONST dst_main,
    data_t *CONST dst_aux,
    data_t *CONST src_main,
    data_t *CONST src_aux,
    mlib_s32 CONST order,
    mlib_s32 CONST shiftscale,
    mlib_s32 CONST isign)
{
	deal_t *CONST pal64_r = dst_main, *pal64_i = dst_aux;

	SERVE_BAD_ORDER(order);

	if (order < 4) {
		switch (order) {
		case 0: {
/* nothing to do */
		    }

			break;
		case 1: {
			    deal_t buf0, buf1, buf2, buf3;
			    mlib_d64 CONST multscale =
				shiftscale != 1 ? 0.5 : 1.0;

/* initialise deal buffer */
			    buf0 = dst_main[0];
			    buf1 = dst_aux[0];
			    buf2 = dst_main[1];
			    buf3 = dst_aux[1];
/* butterfly */
			    buf2 = (buf0 += buf2) - (2.0 * buf2);
			    buf3 = (buf1 += buf3) - (2.0 * buf3);
/* scale */
/* put results into destination */
			    dst_main[0] = buf0 * multscale;
			    dst_aux[0] = buf1 * multscale;
			    dst_main[1] = buf2 * multscale;
			    dst_aux[1] = buf3 * multscale;
		    }

			break;
		case 2: {

			    if (isign == INVERS_FFT) {
				    DEAL_ALL_ORD_2_MULT(AS_IS, pal64, pal64_r,
					pal64_i, 0, LOAD_RI, shiftscale, RI, 2);
			    } else {
				    DEAL_ALL_ORD_2_MULT(MINUS, pal64, pal64_r,
					pal64_i, 0, LOAD_RI, shiftscale, RI, 2);
			    }
		    }

			break;
		case 3: {

			    if (isign == INVERS_FFT) {
				    DEAL_ALL_ORD_3_MULT(AS_IS, pal64, pal64_r,
					pal64_i, 0, LOAD_RI, shiftscale, RI, 3);
			    } else {
				    DEAL_ALL_ORD_3_MULT(MINUS, pal64, pal64_r,
					pal64_i, 0, LOAD_RI, shiftscale, RI, 3);
			    }
		    }

			break;
		}

		return (MLIB_SUCCESS);
	}

	INPLACE_BEGINNING(order, pal64, LOAD_RI, shiftscale, RI);

/* deal it */
	mlib_c_fftKernelInplace_RI(pal64_r, pal64_i, order, isign);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    D64_D64C) (
    data_t *CONST dst_main,
    data_t *CONST dst_aux,
    data_t *CONST src_main,
    data_t *CONST src_aux,
    mlib_s32 CONST order,
    mlib_s32 CONST shiftscale,
    mlib_s32 CONST isign)
{
/* no Direct! */
/* useful values */
	size_t CONST size2deal = (size_t)(sizeof (deal_t)<< (order));

/* service things */
	mlib_d64 al64_stack_buffer[1024];
	deal_t *CONST pal64_r = dst_main;
	deal_t *pal64_i = (deal_t *)al64_stack_buffer;
	deal_t *p_malloced = NULL;

	SERVE_BAD_ORDER(order);

	if (order < 4) {
		switch (order) {
		case 0: {
			    dst_main[0] = src_main[0];
		    }

			break;
		case 1: {
			    deal_t buf0, buf2;
			    mlib_d64 CONST multscale =
				shiftscale != 1 ? 0.5 : 1.0;

/* initialise deal buffer */
			    buf0 = src_main[0];
			    buf2 = src_main[2];
/* butterfly */
			    buf2 = (buf0 += buf2) - (2.0 * buf2);
/* scale */
/* put results into destination */
			    dst_main[0] = buf0 * multscale;
			    dst_main[1] = buf2 * multscale;
		    }

			break;
		case 2: {

			    if (isign == INVERS_FFT) {
				    DEAL_ALL_ORD_2_MULT(AS_IS, pal64, src_main,
					src_aux, 0, LOAD_C, shiftscale, RI, 2);
			    }

/*
 * else {
 *          no direct!
 * }
 */
		    }

			break;
		case 3: {

			    if (isign == INVERS_FFT) {
				    DEAL_ALL_ORD_3_MULT(AS_IS, pal64, src_main,
					src_aux, 0, LOAD_C, shiftscale, RI, 3);
			    }

/*
 * else {
 *          no direct!
 * }
 */
		    }

			break;
		}

		return (MLIB_SUCCESS);
	}

/*
 *    determine whether there isn't enough place in stack
 *    and we must 'malloc' our buffer
 */

	if (size2deal > sizeof (al64_stack_buffer)) {
		if (!(p_malloced = (deal_t *)__mlib_malloc(size2deal)))
			return (MLIB_FAILURE);
		pal64_i = p_malloced;
	}

	if (isign == INVERS_FFT) {
		LOAD_BIT_REV_MULT(order, AS_IS, pal64, src_main, src_aux, 0,
		    LOAD_C, shiftscale, RI, order);
	}

/*
 * else {
 *    no direct!
 * }
 */

/* deal it */
	mlib_c_fftKernel_RI(pal64_r, pal64_i, order, INVERS_FFT);

	if (p_malloced)
		__mlib_free(p_malloced);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(
    D64C_D64) (
    data_t *CONST dst_main,
    data_t *CONST dst_aux,
    data_t *CONST src_main,
    data_t *CONST src_aux,
    mlib_s32 CONST order,
    mlib_s32 CONST shiftscale,
    mlib_s32 CONST isign)
{
	deal_t *CONST pal64 = dst_main;
	deal_t rot_r, rot_i, incr_r, incr_i, wtmp;
	int iii, length, quarter;

	SERVE_BAD_ORDER(order);

	if (order < 5) {
		int scaleshift = shiftscale;

		switch (order) {
		case 0: {
			    dst_main[0] = src_main[0];
			    dst_main[1] = 0.0;
		    }

			break;
		case 1: {
			    deal_t buf0, buf2;
			    mlib_d64 CONST multscale =
				shiftscale != 1 ? 0.5 : 1.0;

/* initialise deal buffer */
			    buf0 = src_main[0];
			    buf2 = src_main[1];
/* butterfly */
			    buf2 = (buf0 += buf2) - (2.0 * buf2);
/* scale */
/* put results into destination */
			    dst_main[0] = buf0 * multscale;
			    dst_main[1] = 0.0;
			    dst_main[2] = buf2 * multscale;
			    dst_main[3] = 0.0;
		    }

			break;
		case 2: {
			    deal_t buf0, buf1, buf2, buf3;
			    mlib_d64 CONST multscale =
				.25 * (deal_t)(1 << shiftscale);

/* initialise deal buffer */
			    buf0 = src_main[0];
			    buf1 = src_main[1];
			    buf2 = src_main[2];
			    buf3 = src_main[3];
/* butterfly */
			    buf2 = (buf0 += buf2) - (2.0 * buf2);
			    buf3 = (buf1 += buf3) - (2.0 * buf3);
/* scale */
/* put results into destination */
			    dst_main[0] = buf0 * multscale;
			    dst_main[1] = buf1 * multscale;
			    dst_main[2] = buf2 * multscale;
			    dst_main[3] = buf3 * multscale;
		    }

			dst_main[0] = (wtmp = dst_main[0]) + dst_main[1];
			wtmp -= dst_main[1];
			dst_main[1] = 0;
			dst_main[2 * 1 + 1] = -dst_main[2 * 1 + 1];

			dst_main[2 * 2] = wtmp;
			dst_main[2 * 2 + 1] = 0;

			dst_main[2 * 3] = dst_main[2 * 1];
			dst_main[2 * 3 + 1] = -dst_main[2 * 1 + 1];

			break;
		case 3:
			DEAL_ALL_ORD_2_MULT(AS_IS, pal64, src_main, 0, 0,
			    LOAD_C, shiftscale, C, 3);
			{
				deal_t d1_r = pal64[2 * 1], d1_i =
				    pal64[2 * 1 + 1], d2_r =
				    pal64[2 * 3], d2_i = -pal64[2 * 3 + 1];
				deal_t h1r, h1i, h2r, h2i, h2rot_r, h2rot_i;

				h1r = (d1_r + d2_r);
				h1i = (d1_i + d2_i);
				h2r = (d1_i - d2_i);
				h2i = -(d1_r - d2_r);

				h2rot_r = SIN_PI_BY4 * (h2r - h2i);
				h2rot_i = SIN_PI_BY4 * (h2i + h2r);

				pal64[2 * 1] = .5 * (h1r + h2rot_r);
				pal64[2 * 1 + 1] = -.5 * (h1i + h2rot_i);
				pal64[2 * 3] = .5 * (h1r - h2rot_r);
				pal64[2 * 3 + 1] = -.5 * (-h1i + h2rot_i);
			}

			pal64[0] = (wtmp = pal64[0]) + pal64[1];
			wtmp -= pal64[1];
			pal64[1] = 0;
			pal64[2 * 2 + 1] = -pal64[2 * 2 + 1];

			pal64[2 * 4] = wtmp;
			pal64[2 * 4 + 1] = 0;

			for (iii = 2 * 4 + 2; iii < 2 * 8; iii += 2) {
				pal64[iii] = pal64[2 * 8 - iii];
				pal64[iii + 1] = -pal64[2 * 8 - iii + 1];
			}

			break;
		case 4: {
			    DEAL_ALL_ORD_3_MULT(AS_IS, pal64, src_main, 0, 0,
				LOAD_C, scaleshift, C, 4);

			    rot_r = incr_r = COS_PI_BY8;
			    rot_i = incr_i = SIN_PI_BY8;

			    for (iii = 1; iii < 4; iii++) {
				    int i1 = 2 * iii, i2 = i1 + 1, i3 =
					2 * 8 - i1, i4 = i3 + 1;
				    deal_t d1_r = pal64[i1], d1_i = pal64[i2];
				    deal_t d2_r = pal64[i3], d2_i = -pal64[i4];
				    deal_t h1r, h1i, h2r, h2i, h2rot_r, h2rot_i;

				    h1r = (d1_r + d2_r);
				    h1i = (d1_i + d2_i);
				    h2r = (d1_i - d2_i);
				    h2i = -(d1_r - d2_r);
				    h2rot_r = rot_r * h2r - rot_i * h2i;
				    h2rot_i = rot_r * h2i + rot_i * h2r;
				    pal64[i1] = .5 * (h1r + h2rot_r);
				    pal64[i2] = -.5 * (h1i + h2rot_i);
				    pal64[i3] = .5 * (h1r - h2rot_r);
				    pal64[i4] = -.5 * (-h1i + h2rot_i);
				    ROTATE_CMPLX_TMP(rot, incr, wtmp);
			    }

			    pal64[0] = (wtmp = pal64[0]) + pal64[1];
			    wtmp -= pal64[1];
			    pal64[1] = 0;
			    pal64[2 * 4 + 1] = -pal64[2 * 4 + 1];

			    pal64[2 * 8] = wtmp;
			    pal64[2 * 8 + 1] = 0;

			    for (iii = 2 * 8 + 2; iii < 2 * 16; iii += 2) {
				    pal64[iii] = pal64[2 * 16 - iii];
				    pal64[iii + 1] = -pal64[2 * 16 - iii + 1];
			    }
		    }

			break;
		}

		return (MLIB_SUCCESS);
	}

/* initialise deal buffer */

/*
 * if( isign == INVERS_FFT ) {
 *    no inverse!
 *  }
 *  else
 */
	{
		LOAD_BIT_REV_MULT(order - 1, AS_IS, pal64, src_main, NULL, 0,
		    LOAD_C, shiftscale, C, order);
	}
/* deal it */
	    mlib_c_fftKernel(pal64, order - 1, INVERS_FFT);

	length = 1 << order;
	quarter = length >> 2;

	wtmp = SINUS(order);
	rot_r = incr_r = 1. - 2. * wtmp * wtmp;
	rot_i = incr_i = SINUS(order - 1);

	for (iii = 1; iii < quarter; iii++) {
		int i1 = 2 * iii, i2 = i1 + 1, i3 = length - i1, i4 = i3 + 1;
		deal_t d1_r = pal64[i1], d1_i = pal64[i2];
		deal_t d2_r = pal64[i3], d2_i = -pal64[i4];
		deal_t h1r, h1i, h2r, h2i, h2rot_r, h2rot_i;

		h1r = (d1_r + d2_r);
		h1i = (d1_i + d2_i);
		h2r = (d1_i - d2_i);
		h2i = -(d1_r - d2_r);
		h2rot_r = rot_r * h2r - rot_i * h2i;
		h2rot_i = rot_r * h2i + rot_i * h2r;
		pal64[i1] = .5 * (h1r + h2rot_r);
		pal64[i2] = -.5 * (h1i + h2rot_i);
		pal64[i3] = .5 * (h1r - h2rot_r);
		pal64[i4] = -.5 * (-h1i + h2rot_i);
		ROTATE_CMPLX_TMP(rot, incr, wtmp);
	}

	pal64[0] = (wtmp = pal64[0]) + pal64[1];
	wtmp -= pal64[1];
	pal64[1] = 0;
	pal64[2 * quarter + 1] = -pal64[2 * quarter + 1];

	pal64[length] = wtmp;
	pal64[length + 1] = 0;

/* just filling the dependent part */
	for (iii = length + 2; iii < 2 * length; iii += 2) {
		pal64[iii] = pal64[2 * length - iii];
		pal64[iii + 1] = -pal64[2 * length - iii + 1];
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#if REV_NBITS == 16

/* *********************************************************** */

#define	FUNC_CALL(SUFF)                                             \
	mlib_status GLOB_NAME(SUFF, _) (PARAMS)                     \
	{                                                           \
	    mlib_s32 ord = order - 2;                               \
	                                                            \
	    if (ord < 8) {                                          \
		return GLOB_NAME(SUFF, _8_) (dst_main, dst_aux,     \
		    src_main, src_aux, order, shiftscale, isign);   \
	    } else if (ord < 16) {                                  \
		return GLOB_NAME(SUFF, _16_) (dst_main, dst_aux,    \
		    src_main, src_aux, order, shiftscale, isign);   \
	    } else if (ord < 24) {                                  \
		return GLOB_NAME(SUFF, _24_) (dst_main, dst_aux,    \
		    src_main, src_aux, order, shiftscale, isign);   \
	    } else {                                                \
		return GLOB_NAME(SUFF, _32_) (dst_main, dst_aux,    \
		    src_main, src_aux, order, shiftscale, isign);   \
	    }                                                       \
	}

FUNC_CALL(D64)
FUNC_CALL(D64C)
FUNC_CALL(D64_D64)
FUNC_CALL(D64C_D64C)
FUNC_CALL(D64C_D64)
FUNC_CALL(D64_D64C)

/* *********************************************************** */
/* Codes for mlib_ImageFourierTransform function */
/* *********************************************************** */

#undef GET_NUM_FIRST_C

#define	GET_NUM_FIRST_C(first3, order)                          \
	first3 = order;                                         \
	while (first3 > CACHE1_ORD)                             \
	    first3 -= 3;                                        \
	first3 &= 1

/* *********************************************************** */

void
mlib_SignalFFT_first3_blk_nbr(
    mlib_d64 *dst_src,
    mlib_s32 order,
    mlib_s32 order_main)
{
	mlib_s32 i;

	for (i = 0; i < (1 << (order_main - order)); i++) {

		LOAD_NOT_BIT_REV(order, MINUS, dst_src, dst_src, NULL, 0,
		    LOAD_C, 0., C);

		dst_src += (2 << order);
	}
}

/* *********************************************************** */

void
mlib_SignalIFFT_first3_blk_nbr(
    mlib_d64 *dst_src,
    mlib_s32 order,
    mlib_s32 order_main)
{
	mlib_s32 i;

	for (i = 0; i < (1 << (order_main - order)); i++) {

		LOAD_NOT_BIT_REV(order, AS_IS, dst_src, dst_src, NULL, 0,
		    LOAD_C, 0., C);

		dst_src += (2 << order);
	}
}

/* *********************************************************** */

void
mlib_SignalFFT_first3_blk(
    mlib_d64 *dst_main,
    mlib_d64 *src_main,
    mlib_s32 order,
    mlib_s32 order_main)
{
	deal_t *CONST pal64 = dst_main;
	mlib_s32 i;

	for (i = 0; i < (1 << (order_main - order)); i++) {

		LOAD_BIT_REV(order, MINUS, pal64, src_main, src_aux, 0, LOAD_C,
		    0., C);

		src_main += (2 << order);
		pal64 += (2 << order);
	}
}

/* *********************************************************** */

void
mlib_SignalIFFT_first3_blk(
    mlib_d64 *dst_main,
    mlib_d64 *src_main,
    mlib_s32 order,
    mlib_s32 order_main)
{
	deal_t *CONST pal64 = dst_main;
	mlib_s32 i;

	for (i = 0; i < (1 << (order_main - order)); i++) {

		LOAD_BIT_REV(order, AS_IS, pal64, src_main, src_aux, 0, LOAD_C,
		    0., C);

		src_main += (2 << order);
		pal64 += (2 << order);
	}
}

/* *********************************************************** */

void
mlib_SignalFFT_first3_blk_r(
    mlib_d64 *dst_main,
    mlib_d64 *src_main,
    mlib_s32 order,
    mlib_s32 order_main)
{
	deal_t *CONST pal64 = dst_main;
	mlib_s32 i;

	for (i = 0; i < (1 << (order_main - order)); i++) {

		LOAD_BIT_REV(order, MINUS, pal64, src_main, src_aux, 0, LOAD_R,
		    0., C);

		src_main += (1 << order);
		pal64 += (2 << order);
	}
}

/* *********************************************************** */

void
mlib_SignalIFFT_first3_blk_r(
    mlib_d64 *dst_main,
    mlib_d64 *src_main,
    mlib_s32 order,
    mlib_s32 order_main)
{
	deal_t *CONST pal64 = dst_main;
	mlib_s32 i;

	for (i = 0; i < (1 << (order_main - order)); i++) {

		LOAD_BIT_REV(order, AS_IS, pal64, src_main, src_aux, 0, LOAD_R,
		    0., C);

		src_main += (1 << order);
		pal64 += (2 << order);
	}
}

/* *********************************************************** */

mlib_s32
mlib_SignalFFT_CACHE1_ORD()
{
	return (CACHE1_ORD);
}

/* *********************************************************** */

/* last layer */

void
mlib_fft_D64C_D64_LL(
	data_t *pal64,
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
		pal64[i5] = pal64[i1] = .5 * (h1r + h2rot_r);
		pal64[i6] = -(pal64[i2] = -.5 * (h1i + h2rot_i));
		pal64[i7] = pal64[i3] = .5 * (h1r - h2rot_r);
		pal64[i8] = -(pal64[i4] = -.5 * (-h1i + h2rot_i));
		ROTATE_CMPLX_TMP(rot, incr, wtmp);
	}
}

/* *********************************************************** */

#endif /* REV_NBITS == 16 */
/* *********************************************************** */

#undef iii0
#undef iii1
#define	iii0  _iii0
#define	iii1  _iii1

void
FUNC_NAME(BitRev) (
	data_t *pal64,
	data_t *src_main,
	mlib_s32 CONST order,
	mlib_s32 CONST scl,
	mlib_s32 CONST shiftscale,
	mlib_s32 CONST isign,
	mlib_s32 CONST _iii0,
	mlib_s32 CONST _iii1)
{
	if (isign == INVERS_FFT) {
		LOAD_BIT_REV_MULT(order, AS_IS, pal64, src_main, 0, 0, LOAD_C,
		    shiftscale, C, scl);
	} else {
		LOAD_BIT_REV_MULT(order, MINUS, pal64, src_main, 0, 0, LOAD_C,
		    shiftscale, C, scl);
	}
}

void
FUNC_NAME(BitRev_RI) (
	data_t *pal64_r,
	data_t *pal64_i,
	data_t *src_main,
	data_t *src_aux,
	mlib_s32 CONST order,
	mlib_s32 CONST shiftscale,
	mlib_s32 CONST isign,
	mlib_s32 CONST _iii0,
	mlib_s32 CONST _iii1)
{
	if (isign == INVERS_FFT) {
		LOAD_BIT_REV_MULT(order, AS_IS, pal64, src_main, src_aux, 0,
		    LOAD_RI, shiftscale, RI, order);
	} else {
		LOAD_BIT_REV_MULT(order, MINUS, pal64, src_main, src_aux, 0,
		    LOAD_RI, shiftscale, RI, order);
	}
}

void
FUNC_NAME(BitRev_CRI) (
	data_t *pal64_r,
	data_t *pal64_i,
	data_t *src_main,
	data_t *src_aux,
	mlib_s32 CONST order,
	mlib_s32 CONST shiftscale,
	mlib_s32 CONST isign,
	mlib_s32 CONST _iii0,
	mlib_s32 CONST _iii1)
{
	LOAD_BIT_REV_MULT(order, AS_IS, pal64, src_main, src_aux, 0,
		    LOAD_C, shiftscale, RI, order);
}

void
FUNC_NAME(BitRev_Inp) (
	data_t *dst_main,
	mlib_s32 CONST order,
	mlib_s32 CONST shiftscale)
{
	INPLACE_BEGINNING(order, dst_main, LOAD_C, shiftscale, C);
}

void
FUNC_NAME(BitRev_RI_Inp) (
	data_t *pal64_r,
	data_t *pal64_i,
	mlib_s32 CONST order,
	mlib_s32 CONST shiftscale)
{
	INPLACE_BEGINNING(order, pal64, LOAD_RI, shiftscale, RI);
}

/* *********************************************************** */
