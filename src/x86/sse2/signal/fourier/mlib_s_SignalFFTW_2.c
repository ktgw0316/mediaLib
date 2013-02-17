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

#pragma ident	"@(#)mlib_s_SignalFFTW_2.c	9.5	07/11/05 SMI"

/*
 * FUNCTIONS
 *  NOTE: FFT below means F(ast)F(ourier)T(ransform)
 *      mlib_SignalFFTW_2_S16C_S16C      - windowed FFT of 16-bit format complex
 *                                         array into complex array
 *      mlib_SignalFFTW_2_S16_S16        - windowed FFT
 *                                         of 16-bit format 2 arrays
 *                                         as real and imag parts
 *                                         of complex into 2
 *                                         arrays as real and
 *                                         imag parts of complex
 *      mlib_SignalFFTW_2_S16C_S16       - windowed FFT of 16bit format 1 array
 *                                         as real part of( real-valued )
 *                                         complex into complex array
 *      mlib_SignalIFFTW_2_S16C_S16C_Mod - inverse windowed FFT
 *                                         of 16-bit format
 *                                         complex array into complex array
 *      mlib_SignalIFFTW_2_S16_S16_Mod   - inverse windowed FFT
 *                                         of 16-bit format
 *                                         2 arrays as real and imag parts of
 *                                         complex into 2 arrays as real and
 *                                         imag parts of complex
 *      mlib_SignalIFFTW_2_S16_S16C_Mod  - inverse windowed FFT of 16bit format
 *                                         complex array into 1 array as real
 *                                         part of( real-valued ) complex
 *                                         NOTE: this is a semantic pair
 *                                           to mlib_SignalFFTW_2_S16C_S16
 *      mlib_SignalFFTW_2_S16C           - windowed FFT of 16-bit format complex
 *                                         array into complex array
 *                                         (input and output arrays
 *                                         are the same)
 *      mlib_SignalFFTW_2_S16            - windowed FFT
 *                                         of 16-bit format 2 arrays
 *                                         as real and imag parts
 *                                         of complex into
 *                                         2 arrays as real and imag parts of
 *                                         complex
 *                                         (input and output array pairs
 *                                         are the same)
 *      mlib_SignalIFFTW_2_S16C_Mod      - inverse windowed FFT
 *                                         of 16-bit format complex
 *                                         array into complex array
 *                                         (input and output arrays
 *                                         are the same)
 *      mlib_SignalIFFTW_2_S16_Mod       - inverse windowed FFT of 16-bit format
 *                                         2 arrays as real and imag parts of
 *                                         complex into 2 arrays as real and
 *                                         imag parts of complex
 *                                         (input and output array pairs
 *                                         are the same)
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalFFTW_2_S16(mlib_s16       *datar,
 *                                        mlib_s16       *datai,
 *                                        const mlib_s16 *window,
 *                                        mlib_s32       order);
 *      mlib_status mlib_SignalFFTW_2_S16C(mlib_s16       *datac,
 *                                         const mlib_s16 *window,
 *                                         mlib_s32       order);
 *      mlib_status mlib_SignalFFTW_2_S16C_S16(mlib_s16       *dstc,
 *                                             const mlib_s16 *srcr,
 *                                             const mlib_s16 *window,
 *                                             mlib_s32       order);
 *      mlib_status mlib_SignalFFTW_2_S16C_S16C(mlib_s16       *dstc,
 *                                              const mlib_s16 *srcc,
 *                                              const mlib_s16 *window,
 *                                              mlib_s32       order);
 *      mlib_status mlib_SignalFFTW_2_S16_S16(mlib_s16       *dstr,
 *                                            mlib_s16       *dsti,
 *                                            const mlib_s16 *srcr,
 *                                            const mlib_s16 *srci,
 *                                            const mlib_s16 *window,
 *                                            mlib_s32       order);
 *      mlib_status mlib_SignalIFFTW_2_S16C_Mod(mlib_s16       *datac,
 *                                              const mlib_s16 *window,
 *                                              mlib_s32       order);
 *      mlib_status mlib_SignalIFFTW_2_S16C_S16C_Mod(mlib_s16       *dstc,
 *                                                   const mlib_s16 *srcc,
 *                                                   const mlib_s16 *window,
 *                                                   mlib_s32       order);
 *      mlib_status mlib_SignalIFFTW_2_S16_Mod(mlib_s16       *datar,
 *                                             mlib_s16       *datai,
 *                                             const mlib_s16 *window,
 *                                             mlib_s32       order);
 *      mlib_status mlib_SignalIFFTW_2_S16_S16C_Mod(mlib_s16       *dstr,
 *                                                  const mlib_s16 *srcc,
 *                                                  const mlib_s16 *window,
 *                                                  mlib_s32       order);
 *      mlib_status mlib_SignalIFFTW_2_S16_S16_Mod(mlib_s16       *dstr,
 *                                                 mlib_s16       *dsti,
 *                                                 const mlib_s16 *srcr,
 *                                                 const mlib_s16 *srci,
 *                                                 const mlib_s16 *window,
 *                                                 mlib_s32       order);
 *
 * ARGUMENTS
 *      datac   - Complex source and destination signal array. datac[ 2 * i ]
 *                hold the real parts, and datac[ 2 * i + 1 ] hold the
 *                imaginary parts
 *
 *      datar   - Source and destination signal array that hold real parts
 *
 *      datai   - Source and destination signal array that hold imaginary
 *
 *      dstc    - Complex destination signal array. dstc[ 2 * i ] hold
 *                the real parts, and dstc[ 2 * i + 1 ] hold the
 *                imaginary parts
 *
 *      dstr    - Destination signal array that hold real parts
 *
 *      dsti    - Destination signal array that hold imaginary
 *
 *      srcc    - Complex source signal array. srcc[ 2 * i ] hold the real
 *                parts, and srcc[ 2 * i + 1 ] hold the imaginary parts
 *
 *      srcr    - Source signal array that hold real parts
 *
 *      srci    - Source signal array that hold imaginary
 *
 *      window  - Window coefficient array. The window coefficients are in
 *                Q15 format. Note the data type of the window coefficient
 *                array is S16 for both S16 and S16C input signal
 *
 *      order   - Order of the transformation. The base-2 logarithm of the
 *                number of data samples
 *
 * DESCRIPTION
 *
 *      dest = FFT|InverseFFT( src, window )
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
 * Note1. Functions like mlib*FFTW_2_S16 and mlib*FFTW_2_S16C are trivially
 *  mapped into mlib*FFTW_2_S16_S16 and mlib*FFTW_2_S16C_S16C.
 * Note2. All functions ( regarding Note1 mapping ) have "_0_4-shadows":
 *  funcname -> funcname##_0_4 providing processing for orders 0 - 4
 */

#include <mlib_signal.h>
#include <mlib_s_SignalFFTUtil.h>
#include <mlib_s_SignalFFT_2_Macros.h>

/* *********************************************************** */
#undef	LOAD_ROT_DIR
/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalFFTW_2_S16 = __mlib_SignalFFTW_2_S16
#pragma weak mlib_SignalFFTW_2_S16C = __mlib_SignalFFTW_2_S16C
#pragma weak mlib_SignalFFTW_2_S16C_S16 = __mlib_SignalFFTW_2_S16C_S16
#pragma weak mlib_SignalFFTW_2_S16C_S16C = __mlib_SignalFFTW_2_S16C_S16C
#pragma weak mlib_SignalFFTW_2_S16_S16 = __mlib_SignalFFTW_2_S16_S16
#pragma weak mlib_SignalIFFTW_2_S16C_Mod = __mlib_SignalIFFTW_2_S16C_Mod
#pragma weak mlib_SignalIFFTW_2_S16C_S16C_Mod = \
	__mlib_SignalIFFTW_2_S16C_S16C_Mod
#pragma weak mlib_SignalIFFTW_2_S16_Mod = __mlib_SignalIFFTW_2_S16_Mod
#pragma weak mlib_SignalIFFTW_2_S16_S16C_Mod = \
	__mlib_SignalIFFTW_2_S16_S16C_Mod
#pragma weak mlib_SignalIFFTW_2_S16_S16_Mod = \
	__mlib_SignalIFFTW_2_S16_S16_Mod

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalFFTW_2_S16) mlib_SignalFFTW_2_S16
    __attribute__((weak, alias("__mlib_SignalFFTW_2_S16")));
__typeof__(__mlib_SignalFFTW_2_S16C) mlib_SignalFFTW_2_S16C
    __attribute__((weak, alias("__mlib_SignalFFTW_2_S16C")));
__typeof__(__mlib_SignalFFTW_2_S16C_S16) mlib_SignalFFTW_2_S16C_S16
    __attribute__((weak, alias("__mlib_SignalFFTW_2_S16C_S16")));
__typeof__(__mlib_SignalFFTW_2_S16C_S16C) mlib_SignalFFTW_2_S16C_S16C
    __attribute__((weak, alias("__mlib_SignalFFTW_2_S16C_S16C")));
__typeof__(__mlib_SignalFFTW_2_S16_S16) mlib_SignalFFTW_2_S16_S16
    __attribute__((weak, alias("__mlib_SignalFFTW_2_S16_S16")));
__typeof__(__mlib_SignalIFFTW_2_S16C_Mod) mlib_SignalIFFTW_2_S16C_Mod
    __attribute__((weak, alias("__mlib_SignalIFFTW_2_S16C_Mod")));
__typeof__(__mlib_SignalIFFTW_2_S16C_S16C_Mod) mlib_SignalIFFTW_2_S16C_S16C_Mod
    __attribute__((weak, alias("__mlib_SignalIFFTW_2_S16C_S16C_Mod")));
__typeof__(__mlib_SignalIFFTW_2_S16_Mod) mlib_SignalIFFTW_2_S16_Mod
    __attribute__((weak, alias("__mlib_SignalIFFTW_2_S16_Mod")));
__typeof__(__mlib_SignalIFFTW_2_S16_S16C_Mod) mlib_SignalIFFTW_2_S16_S16C_Mod
    __attribute__((weak, alias("__mlib_SignalIFFTW_2_S16_S16C_Mod")));
__typeof__(__mlib_SignalIFFTW_2_S16_S16_Mod) mlib_SignalIFFTW_2_S16_S16_Mod
    __attribute__((weak, alias("__mlib_SignalIFFTW_2_S16_S16_Mod")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	LOAD_ROT_DIR                                                         \
	rot3r = _mm_loadl_epi64((__m128i *)((mlib_d64 *)mlib_fft_rot + 1));  \
	rot3i = _mm_loadl_epi64((__m128i *)((mlib_d64 *)mlib_fft_rot + 2));  \
	rotc0r = _mm_loadl_epi64((__m128i *)((mlib_d64 *)mlib_fft_rot + 3)); \
	rotc1r = _mm_loadl_epi64((__m128i *)((mlib_d64 *)mlib_fft_rot + 4)); \
	rotd0r = _mm_loadl_epi64((__m128i *)((mlib_d64 *)mlib_fft_rot + 5)); \
	rotd1r = _mm_loadl_epi64((__m128i *)((mlib_d64 *)mlib_fft_rot + 6)); \
	divider = 2;                                                         \
	REDEF_ADDER(divider);

/* *********************************************************** */

/* functions codes */

static mlib_status
mlib_s_SignalFFTW_S16C_S16C_0_4(
    mlib_s16 *dstc,
    const mlib_s16 *srcc,
    const mlib_s16 *window,
    mlib_s32 order)
{
	__m128i dsrc, dsrc0, dsrc1, dsrc2, dsrc3, dst, dst1;
	__m128i dsrc4, dsrc5, dsrc6, dsrc7, dst2, dst3;
	__m128i dwin, dwin1, dwin2, dwin3;
	mlib_s32 fonebytwo = 1;
	DEF_ADDER(fonebytwo);
	mlib_s32 foneby8 = 3;
	DEF_ADDER(foneby8);
	__m128i fsinpion4 = _mm_set1_epi32(0x5A825A82);
	mlib_s32 gsr_shift;

	switch (order) {
	case 0:
		dstc[0] = (srcc[0] * window[0]) >> 15;
		dstc[1] = (srcc[1] * window[0]) >> 15;
		break;

	case 1:
		gsr_shift = 0;
		VIS_LDF_DBL(dwin, window);
		VIS_LDD(dsrc, srcc);
		MUL32_WIN(dsrc0, dsrc1, dwin, dsrc);
		dst = BUTT_FPFX_PR_SIMPLE(dsrc0, dsrc1);
		VIS_STD(dstc, dst);
		break;

	case 2:
		gsr_shift = 0;
		VIS_LDD(dwin, window);
		DBL_WIND(dwin, dwin1);
		VIS_LDD_2(dsrc, dsrc3, srcc);
		dsrc = SSE2_fmul8x16(fonebytwo, dsrc);
		dsrc3 = SSE2_fmul8x16(fonebytwo, dsrc3);
		MUL32_WIN(dsrc0, dsrc1, dwin, dsrc);
		MUL32_WIN(dsrc2, dsrc3, dwin1, dsrc3);
		BUTT(dsrc, dsrc0, dsrc0, dsrc2);
		dst1 = _mm_add_epi32(dsrc1, dsrc3);
		dsrc1 =
		    FREG(sub, lo, dsrc1, lo, dsrc3, sub, hi, dsrc3, hi, dsrc1);
		BUTT_FPFX_PR(dst, dst1, dsrc, dst1, dsrc0, dsrc1);
		VIS_STD_2(dstc, dst, dst1);
		break;

	case 3:
		gsr_shift = 1;
		VIS_LDD_2(dwin, dwin2, window);
		dwin = SSE2_fmul8x16(foneby8, dwin);
		dwin2 = SSE2_fmul8x16(foneby8, dwin2);
		DBL_WIND(dwin, dwin1);
		DBL_WIND(dwin2, dwin3);
		VIS_LDD_4(dsrc1, dsrc3, dsrc5, dsrc7, srcc);
		MUL32_WIN(dsrc0, dsrc1, dwin, dsrc1);
		MUL32_WIN(dsrc2, dsrc3, dwin1, dsrc3);
		MUL32_WIN(dsrc4, dsrc5, dwin2, dsrc5);
		MUL32_WIN(dsrc6, dsrc7, dwin3, dsrc7);
		BUTT(dst, dst1, dsrc0, dsrc4);
		BUTT(dst2, dst3, dsrc2, dsrc6);
		BUTT(dsrc4, dsrc5, dsrc1, dsrc5);
		BUTT(dsrc6, dsrc7, dsrc3, dsrc7);

		BUTT(dsrc0, dsrc2, dst, dst2);
		BUTT_FREG(dsrc1, dsrc3, hi, dst1, lo, dst3, lo, dst1, hi, dst3);
		BUTT(dst, dst2, dsrc4, dsrc6);
		BUTT_FREG(dst1, dst3, hi, dsrc5, lo, dsrc7, lo, dsrc5, hi,
		    dsrc7);

		dst1 =
		    SSE2_fpackfix_pair(FREG(add, hi, dst1, lo, dst1, sub, lo,
		    dst1, hi, dst1), FREG(sub, hi, dst3, lo, dst3, add, hi,
		    dst3, lo, dst3));
		MUL32_HL(dsrc5, dsrc7, fsinpion4, dst1);
		BUTT_FPFX_PR(dst3, dsrc5, dsrc0, dst, dsrc1, dsrc5);
		dsrc4 =
		    SSE2_fpackfix_pair_r(FREG(add, hi, dsrc2, lo, dst2, sub, lo,
		    dsrc2, hi, dst2), _mm_sub_epi32(dsrc3, dsrc7));
		dsrc6 =
		    SSE2_fpackfix_pair_r(FREG(sub, hi, dsrc2, lo, dst2, add, lo,
		    dsrc2, hi, dst2), _mm_add_epi32(dsrc3, dsrc7));
		VIS_STD_4(dstc, dst3, dsrc4, dsrc5, dsrc6);
		break;

	case 4: {
		    mlib_s32 divider = 1;
		    DEF_ADDER(divider);
		    __m128i reim0, reim1, reim2, reim3, reim4, reim5, reim6,
			reim7, tmp;

		    DCL_COMMON;
		    DCL_OUTVARR;
		    DCL_WINDOW;

		    PREP_OUTC;

		    LOAD_WINDOW;
		    LOAD_REIM;

		    MUL_RWIN;
		    MUL_IWIN;

		    PROC_FIRST_DIR;

		    TRANSPOSE4X4_REV_NEW(ar, br, cr, dr, ar, br, cr, dr);
		    TRANSPOSE4X4_REV_NEW(ai, bi, ci, di, ai, bi, ci, di);

		    LOAD_ROT_DIR;

		    PROC_LAST_DIR;
		    PROC_LAST_END;

		    STORE_REIM;
	    }

		break;

	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

static mlib_status
mlib_s_SignalFFTW_S16_S16_0_4(
    mlib_s16 *dstr,
    mlib_s16 *dsti,
    const mlib_s16 *srcr,
    const mlib_s16 *srci,
    const mlib_s16 *window,
    mlib_s32 order)
{
	__m128i dsrc, dsrc0, dsrc1, dsrc2, dsrc3, dst, dst1;
	__m128i dsrc4, dsrc5, dsrc6, dsrc7, dst2, dst3, dwin, dwin1;
	__m128i fdst0, fdst1;
	mlib_s32 fonebytwo = 1;
	DEF_ADDER(fonebytwo);
	mlib_s32 foneby8 = 3;
	DEF_ADDER(foneby8);
	__m128i fsinpion4 = _mm_set1_epi32(0x5A825A82);
	mlib_s32 gsr_shift;

	switch (order) {
	case 0:
		dstr[0] = (srcr[0] * window[0]) >> 15;
		dsti[0] = (srci[0] * window[0]) >> 15;
		break;

	case 1:
		gsr_shift = 0;
		VIS_LDF(dwin, window);
		VIS_LDF(dsrc0, srcr);
		VIS_LDF(dsrc1, srci);
		dsrc0 = MUL32F(_MM_READ32_hi(dwin), _MM_READ32_hi(dsrc0));
		dsrc1 = MUL32F(_MM_READ32_hi(dwin), _MM_READ32_hi(dsrc1));
		fdst0 =
		    SSE2_fpackfix_r(
		    FREG(add, hi, dsrc0, lo, dsrc0, sub, hi, dsrc0, lo, dsrc0));
		fdst1 =
		    SSE2_fpackfix_r(
		    FREG(add, hi, dsrc1, lo, dsrc1, sub, hi, dsrc1, lo, dsrc1));
		VIS_STF(dstr, fdst0);
		VIS_STF(dsti, fdst1);
		break;

	case 2:
		gsr_shift = 0;
		VIS_LDD(dwin, window);
		dwin = SSE2_fmul8x16(fonebytwo, dwin);
		VIS_LDD(dsrc, srcr);
		VIS_LDD(dsrc3, srci);
		MUL32_WIN(dsrc0, dsrc1, dwin, dsrc);
		MUL32_WIN(dsrc2, dsrc3, dwin, dsrc3);
		BUTT(dsrc, dsrc0, dsrc0, dsrc1);
		BUTT(dst1, dsrc2, dsrc2, dsrc3);
		dst =
		    SSE2_fpackfix_pair_r(FREG(add, hi, dsrc, lo, dsrc, add, hi,
		    dsrc0, lo, dsrc2), FREG(sub, hi, dsrc, lo, dsrc, sub, hi,
		    dsrc0, lo, dsrc2));
		dst1 =
		    SSE2_fpackfix_pair_r(FREG(add, hi, dst1, lo, dst1, sub, hi,
		    dsrc2, lo, dsrc0), FREG(sub, hi, dst1, lo, dst1, add, hi,
		    dsrc2, lo, dsrc0));
		VIS_STD(dstr, dst);
		VIS_STD(dsti, dst1);
		break;

	case 3:
		gsr_shift = 1;
		VIS_LDD_2(dwin, dwin1, window);
		dwin = SSE2_fmul8x16(foneby8, dwin);
		dwin1 = SSE2_fmul8x16(foneby8, dwin1);
		VIS_LDD_2(dsrc1, dsrc3, srcr);
		VIS_LDD_2(dsrc5, dsrc7, srci);
		MUL32_WIN(dsrc0, dsrc1, dwin, dsrc1);
		MUL32_WIN(dsrc2, dsrc3, dwin1, dsrc3);
		MUL32_WIN(dsrc4, dsrc5, dwin, dsrc5);
		MUL32_WIN(dsrc6, dsrc7, dwin1, dsrc7);
		BUTT(dst, dst1, dsrc0, dsrc2);
		BUTT(dst2, dst3, dsrc1, dsrc3);
		BUTT(dsrc0, dsrc1, dsrc4, dsrc6);
		BUTT(dsrc2, dsrc3, dsrc5, dsrc7);

		BUTT(dsrc4, dsrc5, dst, dst2);
		BUTT(dsrc7, dsrc6, dst1, dsrc3);
		BUTT(dst, dst1, dsrc0, dsrc2);
		BUTT(dst2, dst3, dsrc1, dst3);

		dsrc1 =
		    SSE2_fpackfix_pair(FREG(sub, lo, dsrc6, lo, dst2, add, lo,
		    dsrc6, lo, dst2), FREG(add, lo, dsrc7, lo, dst3, sub, lo,
		    dsrc7, lo, dst3));
		MUL32_HL(dsrc0, dsrc1, fsinpion4, dsrc1);
		dsrc2 =
		    SSE2_fpackfix_pair_r(
		    FREG(add, hi, dsrc4, lo, dsrc4, add, hi, dsrc7, hi, dsrc1),
		    FREG(add, hi, dsrc5, lo, dst1, sub, hi, dsrc6, hi, dsrc0));
		dsrc3 =
		    SSE2_fpackfix_pair_r(
		    FREG(sub, hi, dsrc4, lo, dsrc4, sub, hi, dsrc7, hi, dsrc1),
		    FREG(sub, hi, dsrc5, lo, dst1, add, hi, dsrc6, hi, dsrc0));
		dsrc4 =
		    SSE2_fpackfix_pair_r(
		    FREG(add, hi, dst, lo, dst, sub, hi, dst3, lo, dsrc1),
		    FREG(sub, hi, dst1, lo, dsrc5, sub, hi, dst2, lo, dsrc0));
		dsrc5 =
		    SSE2_fpackfix_pair_r(
		    FREG(sub, hi, dst, lo, dst, add, hi, dst3, lo, dsrc1),
		    FREG(add, hi, dst1, lo, dsrc5, add, hi, dst2, lo, dsrc0));
		VIS_STD_2(dstr, dsrc2, dsrc3);
		VIS_STD_2(dsti, dsrc4, dsrc5);
		break;

	case 4: {
		    mlib_s32 divider = 1;
		    DEF_ADDER(divider);

		    DCL_COMMON;
		    DCL_OUTVARR;
		    DCL_OUTVARI;
		    DCL_WINDOW;

		    PREP_OUTR;
		    PREP_OUTI;

		    LOAD_WINDOW;
		    LOAD_REAL;
		    LOAD_IMAG;

		    MUL_RWIN;
		    MUL_IWIN;

		    PROC_FIRST_DIR;

		    TRANSPOSE4X4_REV_NEW(ar, br, cr, dr, ar, br, cr, dr);
		    TRANSPOSE4X4_REV_NEW(ai, bi, ci, di, ai, bi, ci, di);

		    LOAD_ROT_DIR;

		    PROC_LAST_DIR;
		    PROC_LAST_END;

		    STORE_RE;
		    STORE_IM;
	    }

		break;

	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

static mlib_status
mlib_s_SignalFFTW_S16C_S16_0_4(
    mlib_s16 *dstc,
    const mlib_s16 *srcr,
    const mlib_s16 *window,
    mlib_s32 order)
{
	__m128i dsrc, dsrc0, dsrc1, dsrc2, dsrc3, dst, dst1;
	__m128i dsrc4, dsrc5, dst2, dst3, dwin, dwin1;
	__m128i fdst0;
	mlib_s32 fonebytwo = 1;
	DEF_ADDER(fonebytwo);
	mlib_s32 foneby8 = 3;
	DEF_ADDER(foneby8);
	__m128i fzero = _mm_setzero_si128();
	__m128i fsinpion4 = _mm_set1_epi32(0x5A825A82);
	mlib_s32 gsr_shift;

	switch (order) {
	case 0:
		dstc[0] = (srcr[0] * window[0]) >> 15;
		dstc[1] = 0;
		break;

	case 1:
		gsr_shift = 0;
		VIS_LDF(dwin, window);
		VIS_LDF(dsrc0, srcr);
		dsrc0 = MUL32F(_MM_READ32_hi(dwin), _MM_READ32_hi(dsrc0));
		dst =
		    SSE2_fpackfix_pair_r(FREG_FZERO(add, hi, dsrc0, lo, dsrc0),
		    FREG_FZERO(sub, hi, dsrc0, lo, dsrc0));
		VIS_STD(dstc, dst);
		break;

	case 2:
		gsr_shift = 0;
		VIS_LDD(dwin, window);
		dwin = SSE2_fmul8x16(fonebytwo, dwin);
		VIS_LDD(dsrc, srcr);
		MUL32_WIN(dsrc0, dsrc1, dwin, dsrc);
		BUTT(dsrc, dsrc0, dsrc0, dsrc1);
		dst =
		    SSE2_fpackfix_pair_r(FREG_FZERO(add, hi, dsrc, lo, dsrc),
		    FREG(000, hi, dsrc0, 00, 00, sub, 00, fzero, lo, dsrc0));
		dst1 =
		    SSE2_fpackfix_pair_r(FREG_FZERO(sub, hi, dsrc, lo, dsrc),
		    dsrc0);
		VIS_STD_2(dstc, dst, dst1);
		break;

	case 3:
		gsr_shift = 1;
		VIS_LDD_2(dwin, dwin1, window);
		dwin = SSE2_fmul8x16(foneby8, dwin);
		dwin1 = SSE2_fmul8x16(foneby8, dwin1);
		VIS_LDD_2(dsrc1, dsrc3, srcr);
		MUL32_WIN(dsrc0, dsrc1, dwin, dsrc1);
		MUL32_WIN(dsrc2, dsrc3, dwin1, dsrc3);
		BUTT(dst, dst1, dsrc0, dsrc2);
		BUTT(dst2, dst3, dsrc1, dsrc3);

		BUTT(dsrc4, dsrc5, dst, dst2);

		fdst0 =
		    SSE2_fpackfix(FREG(sub, lo, dst1, lo, dst3, add, lo, dst1,
		    lo, dst3));
		dsrc0 = MUL32F(fsinpion4, fdst0);
		dsrc2 =
		    SSE2_fpackfix_pair_r(FREG_FZERO(add, hi, dsrc4, lo, dsrc4),
		    FREG(add, hi, dst1, hi, dsrc0, sub, 00,
		    AS_HL(sub, 00, fzero, hi, dst3), lo, dsrc0));
		dsrc3 =
		    SSE2_fpackfix_pair_r(FREG(000, hi, dsrc5, 00, 00, sub, lo,
		    dst2, lo, dst), FREG(sub, hi, dst1, hi, dsrc0, sub, hi,
		    dst3, lo, dsrc0));
		dsrc4 =
		    SSE2_fpackfix_pair_r(FREG_FZERO(sub, hi, dsrc4, lo, dsrc4),
		    FREG(sub, hi, dst1, hi, dsrc0, sub, lo, dsrc0, hi, dst3));
		dsrc5 =
		    SSE2_fpackfix_pair_r(dsrc5,
		    FREG(add, hi, dst1, hi, dsrc0, add, hi, dst3, lo, dsrc0));
		VIS_STD_4(dstc, dsrc2, dsrc3, dsrc4, dsrc5);
		break;

	case 4: {
		    mlib_s32 divider = 1;
		    DEF_ADDER(divider);
		    __m128i reim0, reim1, reim2, reim3, reim4, reim5, reim6,
			reim7;

		    DCL_COMMON;
		    DCL_OUTVARR;
		    DCL_WINDOW;

		    PREP_OUTC;

		    LOAD_WINDOW;
		    LOAD_REAL;

		    MUL_RWIN;

		    PROC_ONLYREAL_FIRST_DIR;

		    TRANSPOSE4X4_REV_NEW(ar, br, cr, dr, ar, br, cr, dr);
		    TRANSPOSE4X4_REV_NEW(ai, bi, ci, di, ai, bi, ci, di);

		    LOAD_ROT_DIR;

		    PROC_LAST_DIR;
		    PROC_LAST_END;

		    STORE_REIM;
	    }

		break;

	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

static mlib_status
mlib_s_SignalIFFTW_S16C_S16C_0_4(
    mlib_s16 *dstc,
    const mlib_s16 *srcc,
    const mlib_s16 *window,
    mlib_s32 order)
{
	__m128i dsrc, dsrc0, dsrc1, dsrc2, dsrc3, dst, dst1;
	__m128i dsrc4, dsrc5, dsrc6, dsrc7, dst2, dst3;
	__m128i dwin, dwin1, dwin2, dwin3;
	mlib_s32 fonebytwo = 1;
	DEF_ADDER(fonebytwo);
	mlib_s32 foneby8 = 3;
	DEF_ADDER(foneby8);
	__m128i fsinpion4 = _mm_set1_epi32(0x5A825A82);
	mlib_s32 gsr_shift;

	gsr_shift = order;

	switch (order) {
	case 0:
		dstc[0] = (srcc[0] * window[0]) >> 15;
		dstc[1] = (srcc[1] * window[0]) >> 15;
		break;

	case 1:
		VIS_LDF_DBL(dwin, window);
		VIS_LDD(dsrc, srcc);
		MUL32_WIN(dsrc0, dsrc1, dwin, dsrc);
		dst = BUTT_FPFX_PR_SIMPLE(dsrc0, dsrc1);
		VIS_STD(dstc, dst);
		break;

	case 2:
		VIS_LDD(dwin, window);
		dwin = SSE2_fmul8x16(fonebytwo, dwin);
		DBL_WIND(dwin, dwin1);
		VIS_LDD_2(dsrc, dsrc3, srcc);
		MUL32_WIN(dsrc0, dsrc1, dwin, dsrc);
		MUL32_WIN(dsrc2, dsrc3, dwin1, dsrc3);
		BUTT(dsrc, dsrc0, dsrc0, dsrc2);
		dst1 = _mm_add_epi32(dsrc1, dsrc3);
		dsrc1 =
		    FREG(sub, lo, dsrc1, lo, dsrc3, sub, hi, dsrc3, hi, dsrc1);
		BUTT_FPFX_PR_CROSS(dst, dst1, dsrc, dst1, dsrc0, dsrc1);
		VIS_STD_2(dstc, dst, dst1);
		break;

	case 3:
		gsr_shift = 1;
		VIS_LDD_2(dwin, dwin2, window);
		dwin = SSE2_fmul8x16(foneby8, dwin);
		dwin2 = SSE2_fmul8x16(foneby8, dwin2);
		DBL_WIND(dwin, dwin1);
		DBL_WIND(dwin2, dwin3);
		VIS_LDD_4(dsrc1, dsrc3, dsrc5, dsrc7, srcc);
		MUL32_WIN(dsrc0, dsrc1, dwin, dsrc1);
		MUL32_WIN(dsrc2, dsrc3, dwin1, dsrc3);
		MUL32_WIN(dsrc4, dsrc5, dwin2, dsrc5);
		MUL32_WIN(dsrc6, dsrc7, dwin3, dsrc7);
		BUTT(dst, dst1, dsrc0, dsrc4);
		BUTT(dst2, dst3, dsrc2, dsrc6);
		BUTT(dsrc4, dsrc5, dsrc1, dsrc5);
		BUTT(dsrc6, dsrc7, dsrc3, dsrc7);

		BUTT(dsrc0, dsrc2, dst, dst2);
		BUTT_FREG(dsrc3, dsrc1, hi, dst1, lo, dst3, lo, dst1, hi, dst3);
		BUTT(dst, dst2, dsrc4, dsrc6);
		BUTT_FREG(dst3, dst1, hi, dsrc5, lo, dsrc7, lo, dsrc5, hi,
		    dsrc7);

		dst1 =
		    SSE2_fpackfix_pair(FREG(sub, hi, dst1, lo, dst1, add, hi,
		    dst1, lo, dst1), FREG(add, hi, dst3, lo, dst3, sub, lo,
		    dst3, hi, dst3));
		gsr_shift = 4;
		MUL32_HL(dsrc5, dsrc7, fsinpion4, dst1);
		BUTT_FPFX_PR(dst3, dsrc5, dsrc0, dst, dsrc1, dsrc5);
		dsrc4 =
		    SSE2_fpackfix_pair_r(FREG(sub, hi, dsrc2, lo, dst2, add, lo,
		    dsrc2, hi, dst2), _mm_sub_epi32(dsrc3, dsrc7));
		dsrc6 =
		    SSE2_fpackfix_pair_r(FREG(add, hi, dsrc2, lo, dst2, sub, lo,
		    dsrc2, hi, dst2), _mm_add_epi32(dsrc3, dsrc7));
		VIS_STD_4(dstc, dst3, dsrc4, dsrc5, dsrc6);
		break;

	case 4: {
		    __m128i reim0, reim1, reim2, reim3, reim4, reim5, reim6,
			reim7, tmp;

		    DCL_COMMON;
		    DCL_OUTVARR;
		    DCL_WINDOW;

		    PREP_OUTC;

		    LOAD_WINDOW;
		    LOAD_REIM;
		    DUP_R;
		    MUL_RWIN;
		    DUP_I;
		    MUL_IWIN;

		    PROC_FIRST_INV;

		    TRANSPOSE4X4_REV_NEW(ar, br, cr, dr, ar, br, cr, dr);
		    TRANSPOSE4X4_REV_NEW(ai, bi, ci, di, ai, bi, ci, di);

		    LOAD_ROT_INV;

		    PROC_LAST_INV;
		    PROC_LAST_END;

		    STORE_REIM;
	    }

		break;

	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

static mlib_status
mlib_s_SignalIFFTW_S16_S16_0_4(
    mlib_s16 *dstr,
    mlib_s16 *dsti,
    const mlib_s16 *srcr,
    const mlib_s16 *srci,
    const mlib_s16 *window,
    mlib_s32 order)
{
	__m128i dsrc, dsrc0, dsrc1, dsrc2, dsrc3, dst, dst1;
	__m128i dsrc4, dsrc5, dsrc6, dsrc7, dst2, dst3, dwin, dwin1;
	__m128i fdst0, fdst1;
	mlib_s32 fonebytwo = 1;
	DEF_ADDER(fonebytwo);
	mlib_s32 foneby8 = 3;
	DEF_ADDER(foneby8);
	__m128i fsinpion4 = _mm_set1_epi32(0x5A825A82);
	mlib_s32 gsr_shift;

	gsr_shift = order;

	switch (order) {
	case 0:
		dstr[0] = (srcr[0] * window[0]) >> 15;
		dsti[0] = (srci[0] * window[0]) >> 15;
		break;

	case 1:
		VIS_LDF(dwin, window);
		VIS_LDF(dsrc0, srcr);
		VIS_LDF(dsrc1, srci);
		dsrc0 = MUL32F(_MM_READ32_hi(dwin), _MM_READ32_hi(dsrc0));
		dsrc1 = MUL32F(_MM_READ32_hi(dwin), _MM_READ32_hi(dsrc1));
		fdst0 =
		    SSE2_fpackfix_r(
		    FREG(add, hi, dsrc0, lo, dsrc0, sub, hi, dsrc0, lo, dsrc0));
		fdst1 =
		    SSE2_fpackfix_r(
		    FREG(add, hi, dsrc1, lo, dsrc1, sub, hi, dsrc1, lo, dsrc1));
		VIS_STF(dstr, fdst0);
		VIS_STF(dsti, fdst1);
		break;

	case 2:
		VIS_LDD(dwin, window);
		dwin = SSE2_fmul8x16(fonebytwo, dwin);
		VIS_LDD(dsrc, srcr);
		VIS_LDD(dsrc3, srci);
		MUL32_WIN(dsrc0, dsrc1, dwin, dsrc);
		MUL32_WIN(dsrc2, dsrc3, dwin, dsrc3);
		BUTT(dsrc, dsrc0, dsrc0, dsrc1);
		BUTT(dst1, dsrc2, dsrc2, dsrc3);
		dst =
		    SSE2_fpackfix_pair_r(FREG(add, hi, dsrc, lo, dsrc, sub, hi,
		    dsrc0, lo, dsrc2), FREG(sub, hi, dsrc, lo, dsrc, add, hi,
		    dsrc0, lo, dsrc2));
		dst1 =
		    SSE2_fpackfix_pair_r(FREG(add, hi, dst1, lo, dst1, add, hi,
		    dsrc2, lo, dsrc0), FREG(sub, hi, dst1, lo, dst1, sub, hi,
		    dsrc2, lo, dsrc0));
		VIS_STD(dstr, dst);
		VIS_STD(dsti, dst1);
		break;

	case 3:
		gsr_shift = 1;
		VIS_LDD_2(dwin, dwin1, window);
		dwin = SSE2_fmul8x16(foneby8, dwin);
		dwin1 = SSE2_fmul8x16(foneby8, dwin1);
		VIS_LDD_2(dsrc1, dsrc3, srcr);
		VIS_LDD_2(dsrc5, dsrc7, srci);
		MUL32_WIN(dsrc0, dsrc1, dwin, dsrc1);
		MUL32_WIN(dsrc2, dsrc3, dwin1, dsrc3);
		MUL32_WIN(dsrc4, dsrc5, dwin, dsrc5);
		MUL32_WIN(dsrc6, dsrc7, dwin1, dsrc7);
		BUTT(dst, dst1, dsrc0, dsrc2);
		BUTT(dst2, dst3, dsrc1, dsrc3);
		BUTT(dsrc0, dsrc1, dsrc4, dsrc6);
		BUTT(dsrc2, dsrc3, dsrc5, dsrc7);

		BUTT(dsrc4, dsrc5, dst, dst2);
		BUTT(dsrc7, dsrc6, dst1, dsrc3);
		BUTT(dst, dst1, dsrc0, dsrc2);
		BUTT(dst2, dst3, dsrc1, dst3);

		dsrc1 =
		    SSE2_fpackfix_pair(
		    FREG(sub, lo, dsrc6, lo, dst2, add, lo, dsrc6, lo, dst2),
		    FREG(add, lo, dsrc7, lo, dst3, sub, lo, dsrc7, lo, dst3));
		gsr_shift = 4;
		MUL32_HL(dsrc0, dsrc1, fsinpion4, dsrc1);
		dsrc2 =
		    SSE2_fpackfix_pair_r(
		    FREG(add, hi, dsrc4, lo, dsrc4, add, hi, dsrc6, hi, dsrc0),
		    FREG(sub, hi, dsrc5, lo, dst1, sub, hi, dsrc7, hi, dsrc1));
		dsrc3 =
		    SSE2_fpackfix_pair_r(
		    FREG(sub, hi, dsrc4, lo, dsrc4, sub, hi, dsrc6, hi, dsrc0),
		    FREG(add, hi, dsrc5, lo, dst1, add, hi, dsrc7, hi, dsrc1));
		dsrc4 =
		    SSE2_fpackfix_pair_r(
		    FREG(add, hi, dst, lo, dst, add, hi, dst2, lo, dsrc0),
		    FREG(add, hi, dst1, lo, dsrc5, add, hi, dst3, lo, dsrc1));
		dsrc5 =
		    SSE2_fpackfix_pair_r(
		    FREG(sub, hi, dst, lo, dst, sub, hi, dst2, lo, dsrc0),
		    FREG(sub, hi, dst1, lo, dsrc5, sub, hi, dst3, lo, dsrc1));
		VIS_STD_2(dstr, dsrc2, dsrc3);
		VIS_STD_2(dsti, dsrc4, dsrc5);
		break;

	case 4: {
		    __m128i tmp;

		    DCL_COMMON;
		    DCL_OUTVARR;
		    DCL_OUTVARI;
		    DCL_WINDOW;

		    PREP_OUTR;
		    PREP_OUTI;

		    LOAD_WINDOW;
		    LOAD_REAL;
		    LOAD_IMAG;
		    DUP_R;
		    MUL_RWIN;
		    DUP_I;
		    MUL_IWIN;

		    PROC_FIRST_INV;

		    TRANSPOSE4X4_REV_NEW(ar, br, cr, dr, ar, br, cr, dr);
		    TRANSPOSE4X4_REV_NEW(ai, bi, ci, di, ai, bi, ci, di);

		    LOAD_ROT_INV;

		    PROC_LAST_INV;
		    PROC_LAST_END;

		    STORE_RE;
		    STORE_IM;
	    }

		break;

	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

static mlib_status
mlib_s_SignalIFFTW_S16_S16C_0_4(
    mlib_s16 *dstr,
    const mlib_s16 *srcc,
    const mlib_s16 *window,
    mlib_s32 order)
{
	__m128i dsrc, dsrc0, dsrc1, dsrc2, dsrc3, dst, dst1;
	__m128i dsrc4, dsrc5, dsrc6, dsrc7, dst2, dst3;
	__m128i dwin, dwin1, dwin2, dwin3;
	__m128i fdst0, fdst1, fdst2;
	mlib_s32 fonebytwo = 1;
	DEF_ADDER(fonebytwo);
	mlib_s32 foneby8 = 3;
	DEF_ADDER(foneby8);
	__m128i fsinpion4 = _mm_set1_epi32(0x5A825A82);
	mlib_s32 gsr_shift;

	gsr_shift = order;

	switch (order) {
	case 0:
		dstr[0] = (srcc[0] * window[0]) >> 15;
		break;

	case 1:
		VIS_LDF_DBL(dwin, window);
		VIS_LDD(dsrc, srcc);
		MUL32_WIN(dsrc0, dsrc1, dwin, dsrc);
		fdst0 =
		    SSE2_fpackfix_r(
		    FREG(add, hi, dsrc0, hi, dsrc1, sub, hi, dsrc0, hi, dsrc1));
		VIS_STF(dstr, fdst0);
		break;

	case 2:
		VIS_LDD(dwin, window);
		dwin = SSE2_fmul8x16(fonebytwo, dwin);
		DBL_WIND(dwin, dwin1);
		VIS_LDD_2(dsrc, dsrc3, srcc);
		MUL32_WIN(dsrc0, dsrc1, dwin, dsrc);
		MUL32_WIN(dsrc2, dsrc3, dwin1, dsrc3);
		BUTT(dsrc, dsrc0, dsrc0, dsrc2);
		BUTT(dst, dsrc1, dsrc1, dsrc3);
		dst =
		    SSE2_fpackfix_pair_r(FREG(add, hi, dsrc, hi, dst, sub, hi,
		    dsrc0, lo, dsrc1), FREG(sub, hi, dsrc, hi, dst, add, hi,
		    dsrc0, lo, dsrc1));
		VIS_STD(dstr, dst);
		break;

	case 3:
		gsr_shift = 1;
		VIS_LDD_2(dwin, dwin2, window);
		dwin = SSE2_fmul8x16(foneby8, dwin);
		dwin2 = SSE2_fmul8x16(foneby8, dwin2);
		DBL_WIND(dwin, dwin1);
		DBL_WIND(dwin2, dwin3);
		VIS_LDD_4(dsrc1, dsrc3, dsrc5, dsrc7, srcc);
		MUL32_WIN(dsrc0, dsrc1, dwin, dsrc1);
		MUL32_WIN(dsrc2, dsrc3, dwin1, dsrc3);
		MUL32_WIN(dsrc4, dsrc5, dwin2, dsrc5);
		MUL32_WIN(dsrc6, dsrc7, dwin3, dsrc7);
		BUTT(dst, dst1, dsrc0, dsrc4);
		BUTT(dst2, dst3, dsrc2, dsrc6);
		BUTT(dsrc4, dsrc5, dsrc1, dsrc5);
		BUTT(dsrc6, dsrc7, dsrc3, dsrc7);

		BUTT(dsrc0, dsrc2, dst, dst2);
		BUTT_AS_HL(fdst1, fdst2, hi, dst1, lo, dst3);
		BUTT(dst, dst2, dsrc4, dsrc6);
		BUTT_FREG(dst3, dst1, hi, dsrc5, lo, dsrc7, lo, dsrc5, hi,
		    dsrc7);

		fdst0 =
		    SSE2_fpackfix(FREG(sub, hi, dst1, lo, dst1, add, hi, dst3,
		    lo, dst3));
		dsrc5 = MUL32F(fsinpion4, fdst0);
		gsr_shift = 4;
		dsrc4 =
		    SSE2_fpackfix_pair_r(FREG(add, hi, dsrc0, hi, dst, add, 00,
		    fdst2, hi, dsrc5), FREG(sub, hi, dsrc2, lo, dst2, sub, 00,
		    fdst1, lo, dsrc5));
		dsrc5 =
		    SSE2_fpackfix_pair_r(FREG(sub, hi, dsrc0, hi, dst, sub, 00,
		    fdst2, hi, dsrc5), FREG(add, hi, dsrc2, lo, dst2, add, 00,
		    fdst1, lo, dsrc5));
		VIS_STD_2(dstr, dsrc4, dsrc5);
		break;

	case 4: {
		    __m128i reim0, reim1, reim2, reim3, reim4, reim5, reim6,
			reim7, tmp;

		    DCL_COMMON;
		    DCL_OUTVARR;
		    DCL_WINDOW;

		    PREP_OUTR;

		    LOAD_WINDOW;
		    LOAD_REIM;
		    DUP_R;
		    MUL_RWIN;
		    DUP_I;
		    MUL_IWIN;

		    PROC_FIRST_INV;

		    TRANSPOSE4X4_REV_NEW(ar, br, cr, dr, ar, br, cr, dr);
		    TRANSPOSE4X4_REV_NEW(ai, bi, ci, di, ai, bi, ci, di);

		    LOAD_ROT_INV;

		    PROC_LAST_INV;
		    PROC_LAST_END_R;

		    STORE_RE;
	    }

		break;

	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFTW_2_S16C_S16C(
    mlib_s16 *dstc,
    const mlib_s16 *srcc,
    const mlib_s16 *window,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcc);
	SERVE_BAD_POINTER(window);
	SERVE_BAD_ORDER(order);

	if (order <= 4)
		return mlib_s_SignalFFTW_S16C_S16C_0_4(dstc, srcc, window,
		    order);
	else
		return mlib_s_fftDispatcherW_2(dstc, NULL, (void *)srcc, NULL,
		    (void *)window, order, eSrcCmplx | eDstCmplx);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFTW_2_S16_S16(
    mlib_s16 *dstr,
    mlib_s16 *dsti,
    const mlib_s16 *srcr,
    const mlib_s16 *srci,
    const mlib_s16 *window,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstr);
	SERVE_BAD_POINTER(dsti);
	SERVE_BAD_POINTER(srcr);
	SERVE_BAD_POINTER(srci);
	SERVE_BAD_POINTER(window);
	SERVE_BAD_ORDER(order);

	if (order <= 4)
		return mlib_s_SignalFFTW_S16_S16_0_4(dstr, dsti, srcr, srci,
		    window, order);
	else
		return mlib_s_fftDispatcherW_2(dstr, dsti, (void *)srcr,
		    (void *)srci, (void *)window, order, eSrcReIm | eDstReIm);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFTW_2_S16C_S16(
    mlib_s16 *dstc,
    const mlib_s16 *srcr,
    const mlib_s16 *window,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcr);
	SERVE_BAD_POINTER(window);
	SERVE_BAD_ORDER(order);

	if (order <= 4)
		return mlib_s_SignalFFTW_S16C_S16_0_4(dstc, srcr, window,
		    order);
	else
		return mlib_s_fftDispatcherW_2(dstc, NULL, (void *)srcr, NULL,
		    (void *)window, order, eSrcRe | eDstCmplx);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFTW_2_S16C_S16C_Mod(
    mlib_s16 *dstc,
    const mlib_s16 *srcc,
    const mlib_s16 *window,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcc);
	SERVE_BAD_POINTER(window);
	SERVE_BAD_ORDER(order);

	if (order <= 4)
		return mlib_s_SignalIFFTW_S16C_S16C_0_4(dstc, srcc, window,
		    order);
	else
		return mlib_s_fftDispatcherW_2(dstc, NULL, (void *)srcc, NULL,
		    (void *)window, order, eSrcCmplx | eDstCmplx | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFTW_2_S16_S16_Mod(
    mlib_s16 *dstr,
    mlib_s16 *dsti,
    const mlib_s16 *srcr,
    const mlib_s16 *srci,
    const mlib_s16 *window,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstr);
	SERVE_BAD_POINTER(dsti);
	SERVE_BAD_POINTER(srcr);
	SERVE_BAD_POINTER(srci);
	SERVE_BAD_POINTER(window);
	SERVE_BAD_ORDER(order);

	if (order <= 4)
		return mlib_s_SignalIFFTW_S16_S16_0_4(dstr, dsti, srcr, srci,
		    window, order);
	else
		return mlib_s_fftDispatcherW_2(dstr, dsti, (void *)srcr,
		    (void *)srci, (void *)window, order,
		    eSrcReIm | eDstReIm | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFTW_2_S16_S16C_Mod(
    mlib_s16 *dstr,
    const mlib_s16 *srcc,
    const mlib_s16 *window,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(srcc);
	SERVE_BAD_POINTER(dstr);
	SERVE_BAD_POINTER(window);
	SERVE_BAD_ORDER(order);

	if (order <= 4)
		return mlib_s_SignalIFFTW_S16_S16C_0_4(dstr, srcc, window,
		    order);
	else
		return mlib_s_fftDispatcherW_2(dstr, NULL, (void *)srcc, NULL,
		    (void *)window, order, eSrcCmplx | eDstRe | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFTW_2_S16C(
    mlib_s16 *datac,
    const mlib_s16 *window,
    mlib_s32 order)
{
	return (__mlib_SignalFFTW_2_S16C_S16C(datac, datac, window, order));
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFTW_2_S16(
    mlib_s16 *datar,
    mlib_s16 *datai,
    const mlib_s16 *window,
    mlib_s32 order)
{
	return __mlib_SignalFFTW_2_S16_S16(datar, datai, datar, datai, window,
	    order);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFTW_2_S16C_Mod(
    mlib_s16 *datac,
    const mlib_s16 *window,
    mlib_s32 order)
{
	return (__mlib_SignalIFFTW_2_S16C_S16C_Mod
			(datac, datac, window, order));
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFTW_2_S16_Mod(
    mlib_s16 *datar,
    mlib_s16 *datai,
    const mlib_s16 *window,
    mlib_s32 order)
{
	return __mlib_SignalIFFTW_2_S16_S16_Mod(datar, datai, datar, datai,
	    window, order);
}

/* *********************************************************** */
