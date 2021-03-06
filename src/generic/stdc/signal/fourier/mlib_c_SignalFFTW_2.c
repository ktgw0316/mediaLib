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

#pragma ident	"@(#)mlib_c_SignalFFTW_2.c	9.4	07/10/09 SMI"

/*
 * FUNCTIONS
 *  NOTE: FFT below means F(ast)F(ourier)T(ransform)
 *      mlib_SignalFFTW_2_S16C_S16C      - windowed FFT
 *                                         of 16-bit format complex
 *                                         array into complex array
 *      mlib_SignalFFTW_2_S16_S16        - windowed FFT
 *                                         of 16-bit format 2 arrays
 *                                         as real and imag parts
 *                                         of complex into 2 arrays as real
 *                                         and imag parts of complex
 *      mlib_SignalFFTW_2_S16C_S16       - windowed FFT
 *                                         of 16bit format 1 array
 *                                         as real part of( real-valued )
 *                                         complex into complex array
 *      mlib_SignalIFFTW_2_S16C_S16C_Mod - inverse windowed FFT
 *                                         of 16-bit format complex array
 *                                         into complex array
 *      mlib_SignalIFFTW_2_S16_S16_Mod   - inverse windowed FFT
 *                                         of 16-bit format
 *                                         2 arrays as real and imag parts of
 *                                         complex into 2 arrays as real and
 *                                         imag parts of complex
 *      mlib_SignalIFFTW_2_S16_S16C_Mod  - inverse windowed FFT
 *                                         of 16bit format
 *                                         complex array into 1 array as real
 *                                         part of( real-valued ) complex
 *                                         NOTE: this is a semantic pair
 *                                           to mlib_SignalFFTW_2_S16C_S16
 *      mlib_SignalFFTW_2_S16C           - windowed FFT
 *                                         of 16-bit format complex
 *                                         array into complex array
 *                                         (input and output arrays
 *                                         are the same)
 *      mlib_SignalFFTW_2_S16            - windowed FFT
 *                                         of 16-bit format 2 arrays
 *                                         as real and imag
 *                                         parts of complex into
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
 *      mlib_status mlib_SignalFFTW_2_S16(mlib_s16 *datar,
 *                                        mlib_s16 *datai,
 *                                        const mlib_s16 *window,
 *                                        mlib_s32 order);
 *      mlib_status mlib_SignalFFTW_2_S16C(mlib_s16 *datac,
 *                                         const mlib_s16 *window,
 *                                         mlib_s32 order);
 *      mlib_status mlib_SignalFFTW_2_S16C_S16(mlib_s16 *dstc,
 *                                             const mlib_s16 *srcr,
 *                                             const mlib_s16 *window,
 *                                             mlib_s32 order);
 *      mlib_status mlib_SignalFFTW_2_S16C_S16C(mlib_s16 *dstc,
 *                                              const mlib_s16 *srcc,
 *                                              const mlib_s16 *window,
 *                                              mlib_s32 order);
 *      mlib_status mlib_SignalFFTW_2_S16_S16(mlib_s16 *dstr,
 *                                            mlib_s16 *dsti,
 *                                            const mlib_s16 *srcr,
 *                                            const mlib_s16 *srci,
 *                                            const mlib_s16 *window,
 *                                            mlib_s32 order);
 *      mlib_status mlib_SignalIFFTW_2_S16C_Mod(mlib_s16 *datac,
 *                                              const mlib_s16 *window,
 *                                              mlib_s32 order);
 *      mlib_status mlib_SignalIFFTW_2_S16C_S16C_Mod(mlib_s16 *dstc,
 *                                                   const mlib_s16 *srcc,
 *                                                   const mlib_s16 *window,
 *                                                   mlib_s32 order);
 *      mlib_status mlib_SignalIFFTW_2_S16_Mod(mlib_s16 *datar,
 *                                             mlib_s16 *datai,
 *                                             const mlib_s16 *window,
 *                                             mlib_s32 order);
 *      mlib_status mlib_SignalIFFTW_2_S16_S16C_Mod(mlib_s16 *dstr,
 *                                                  const mlib_s16 *srcc,
 *                                                  const mlib_s16 *window,
 *                                                  mlib_s32 order);
 *      mlib_status mlib_SignalIFFTW_2_S16_S16_Mod(mlib_s16 *dstr,
 *                                                 mlib_s16 *dsti,
 *                                                 const mlib_s16 *srcr,
 *                                                 const mlib_s16 *srci,
 *                                                 const mlib_s16 *window,
 *                                                 mlib_s32 order);
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
 *
 */

#include <mlib_signal.h>

#define	FFT_UTIL_C

#include <mlib_SignalFFTUtil.h>

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

/* macros, typedefs */

/* general error processing */

/* *********************************************************** */

#define	SERVE_BAD_ORDER(order)                                  \
	if ((order) < 0 || (order) > 31)                        \
	    return MLIB_OUTOFRANGE

/* *********************************************************** */

#define	SERVE_BAD_POINTER(ptr)                                  \
	if (!(ptr))                                             \
	    return MLIB_NULLPOINTER

/* *********************************************************** */

/* functions codes */

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
	return mlib_c_fftDispatcherW_2(dstc, NULL, (void *)srcc, NULL,
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
	return mlib_c_fftDispatcherW_2(dstr, dsti, (void *)srcr, (void *)srci,
	    (void *)window, order, eSrcReIm | eDstReIm);
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
	return mlib_c_fftDispatcherW_S16C_S16(dstc, NULL, (void *)srcr, NULL,
	    (void *)window, order, 0, eSrcRe | eDstCmplx);
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
	return mlib_c_fftDispatcherW_2(dstc, NULL, (void *)srcc, NULL,
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
	return mlib_c_fftDispatcherW_2(dstr, dsti, (void *)srcr, (void *)srci,
	    (void *)window, order, eSrcReIm | eDstReIm | eInversFFT);
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
	return mlib_c_fftDispatcherW_2(dstr, NULL, (void *)srcc, NULL,
	    (void *)window, order, eSrcCmplx | eDstRe | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFTW_2_S16C(
    mlib_s16 *datac,
    const mlib_s16 *window,
    mlib_s32 order)
{
	return __mlib_SignalFFTW_2_S16C_S16C(datac, datac, (void *)window,
	    order);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFTW_2_S16(
    mlib_s16 *datar,
    mlib_s16 *datai,
    const mlib_s16 *window,
    mlib_s32 order)
{
	return __mlib_SignalFFTW_2_S16_S16(datar, datai, datar, datai,
	    (void *)window, order);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFTW_2_S16C_Mod(
    mlib_s16 *datac,
    const mlib_s16 *window,
    mlib_s32 order)
{
	return __mlib_SignalIFFTW_2_S16C_S16C_Mod(datac, datac, (void *)window,
	    order);
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
	    (void *)window, order);
}

/* *********************************************************** */
