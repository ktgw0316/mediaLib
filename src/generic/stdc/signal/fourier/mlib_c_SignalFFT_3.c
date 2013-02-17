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

#pragma ident	"@(#)mlib_c_SignalFFT_3.c	9.4	07/10/09 SMI"

/*
 * FUNCTIONS
 *  NOTE: FFT below means F(ast)F(ourier)T(ransform)
 *      mlib_SignalFFT_3_S16C_S16C_Mod   - FFT of 16-bit
 *                                         format complex array into
 *                                         complex array
 *      mlib_SignalFFT_3_S16_S16_Mod     - FFT of 16-bit
 *                                         format 2 arrays as real and
 *                                         imag parts of complex
 *                                         into 2 arrays as
 *                                         real and imag parts of complex
 *      mlib_SignalFFT_3_S16C_S16_Mod    - FFT of 16-bit
 *                                         format 1 array as real part
 *                                         of( real-valued ) complex
 *                                         into complex array
 *      mlib_SignalIFFT_3_S16C_S16C_Mod  - inverse FFT of 16-bit
 *                                         format complex array
 *                                         into complex array
 *      mlib_SignalIFFT_3_S16_S16_Mod    - inverse FFT of 16-bit
 *                                         format 2 arrays as real
 *                                         and imag parts of complex
 *                                         into 2 arrays as
 *                                         real and imag parts of complex
 *      mlib_SignalIFFT_3_S16_S16C_Mod   - inverse FFT of 16-bit
 *                                         format complex array
 *                                         into 1 array as real part
 *                                         of( real-valued ) complex
 *                                         NOTE: this is a semantic pair
 *                                           to mlib_SignalFFT_3_S16C_S16_Mod
 *      mlib_SignalFFT_3_S16C_Mod        - FFT of 16-bit
 *                                         format complex array into
 *                                         complex array
 *                                         (input and output arrays
 *                                         are the same)
 *      mlib_SignalFFT_3_S16_Mod         - FFT of 16-bit
 *                                         format 2 arrays as real and
 *                                         imag parts of complex
 *                                         into 2 arrays as
 *                                         real and imag parts of complex
 *                                         (input and output array pairs
 *                                         are the same)
 *      mlib_SignalIFFT_3_S16C_Mod       - inverse FFT of 16-bit
 *                                         format complex array
 *                                         into complex array
 *                                         (input and output arrays
 *                                         are the same)
 *      mlib_SignalIFFT_3_S16_Mod        - inverse FFT of 16-bit
 *                                         format 2 arrays as real
 *                                         and imag parts of complex
 *                                         into 2 arrays as
 *                                         real and imag parts of complex
 *                                         (input and output array pairs
 *                                         are the same)
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalFFT_3_S16C_Mod(mlib_s16 *datac,
 *                                            mlib_s32 order);
 *      mlib_status mlib_SignalFFT_3_S16C_S16C_Mod(mlib_s16       *dstc,
 *                                                 const mlib_s16 *srcc,
 *                                                 mlib_s32       order);
 *      mlib_status mlib_SignalFFT_3_S16C_S16_Mod(mlib_s16       *dstc,
 *                                                const mlib_s16 *srcr,
 *                                                mlib_s32       order);
 *      mlib_status mlib_SignalFFT_3_S16_Mod(mlib_s16 *datar,
 *                                           mlib_s16 *datai,
 *                                           mlib_s32 order);
 *      mlib_status mlib_SignalFFT_3_S16_S16_Mod(mlib_s16       *dstr,
 *                                               mlib_s16       *dsti,
 *                                               const mlib_s16 *srcr,
 *                                               const mlib_s16 *srci,
 *                                               mlib_s32       order);
 *      mlib_status mlib_SignalIFFT_3_S16C_Mod(mlib_s16 *datac,
 *                                             mlib_s32 order);
 *      mlib_status mlib_SignalIFFT_3_S16C_S16C_Mod(mlib_s16       *dstc,
 *                                                  const mlib_s16 *srcc,
 *                                                  mlib_s32       order);
 *      mlib_status mlib_SignalIFFT_3_S16_Mod(mlib_s16 *datar,
 *                                            mlib_s16 *datai,
 *                                            mlib_s32 order);
 *      mlib_status mlib_SignalIFFT_3_S16_S16C_Mod(mlib_s16       *dstr,
 *                                                 const mlib_s16 *srcc,
 *                                                 mlib_s32       order);
 *      mlib_status mlib_SignalIFFT_3_S16_S16_Mod(mlib_s16       *dstr,
 *                                                mlib_s16       *dsti,
 *                                                const mlib_s16 *srcr,
 *                                                const mlib_s16 *srci,
 *                                                mlib_s32       order);
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
 *      order   - Order of the transformation. The base-2 logarithm of the
 *                number of data samples
 *
 * DESCRIPTION
 *
 *      dest = FFT|InverseFFT( src )
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
 * Note. Not-windowed FFT functions call dispatcher with it 'window'
 *  parameter set to NULL ( see also: mlib_c_fftDispatcher_3 macro below )
 *
 */

#include <mlib_signal.h>

#define	FFT_UTIL_C

#include <mlib_SignalFFTUtil.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalFFT_3_S16C_Mod = __mlib_SignalFFT_3_S16C_Mod
#pragma weak mlib_SignalFFT_3_S16C_S16C_Mod = \
	__mlib_SignalFFT_3_S16C_S16C_Mod
#pragma weak mlib_SignalFFT_3_S16C_S16_Mod = __mlib_SignalFFT_3_S16C_S16_Mod
#pragma weak mlib_SignalFFT_3_S16_Mod = __mlib_SignalFFT_3_S16_Mod
#pragma weak mlib_SignalFFT_3_S16_S16_Mod = __mlib_SignalFFT_3_S16_S16_Mod
#pragma weak mlib_SignalIFFT_3_S16C_Mod = __mlib_SignalIFFT_3_S16C_Mod
#pragma weak mlib_SignalIFFT_3_S16C_S16C_Mod = \
	__mlib_SignalIFFT_3_S16C_S16C_Mod
#pragma weak mlib_SignalIFFT_3_S16_Mod = __mlib_SignalIFFT_3_S16_Mod
#pragma weak mlib_SignalIFFT_3_S16_S16C_Mod = \
	__mlib_SignalIFFT_3_S16_S16C_Mod
#pragma weak mlib_SignalIFFT_3_S16_S16_Mod = __mlib_SignalIFFT_3_S16_S16_Mod

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalFFT_3_S16C_Mod) mlib_SignalFFT_3_S16C_Mod
    __attribute__((weak, alias("__mlib_SignalFFT_3_S16C_Mod")));
__typeof__(__mlib_SignalFFT_3_S16C_S16C_Mod) mlib_SignalFFT_3_S16C_S16C_Mod
    __attribute__((weak, alias("__mlib_SignalFFT_3_S16C_S16C_Mod")));
__typeof__(__mlib_SignalFFT_3_S16C_S16_Mod) mlib_SignalFFT_3_S16C_S16_Mod
    __attribute__((weak, alias("__mlib_SignalFFT_3_S16C_S16_Mod")));
__typeof__(__mlib_SignalFFT_3_S16_Mod) mlib_SignalFFT_3_S16_Mod
    __attribute__((weak, alias("__mlib_SignalFFT_3_S16_Mod")));
__typeof__(__mlib_SignalFFT_3_S16_S16_Mod) mlib_SignalFFT_3_S16_S16_Mod
    __attribute__((weak, alias("__mlib_SignalFFT_3_S16_S16_Mod")));
__typeof__(__mlib_SignalIFFT_3_S16C_Mod) mlib_SignalIFFT_3_S16C_Mod
    __attribute__((weak, alias("__mlib_SignalIFFT_3_S16C_Mod")));
__typeof__(__mlib_SignalIFFT_3_S16C_S16C_Mod) mlib_SignalIFFT_3_S16C_S16C_Mod
    __attribute__((weak, alias("__mlib_SignalIFFT_3_S16C_S16C_Mod")));
__typeof__(__mlib_SignalIFFT_3_S16_Mod) mlib_SignalIFFT_3_S16_Mod
    __attribute__((weak, alias("__mlib_SignalIFFT_3_S16_Mod")));
__typeof__(__mlib_SignalIFFT_3_S16_S16C_Mod) mlib_SignalIFFT_3_S16_S16C_Mod
    __attribute__((weak, alias("__mlib_SignalIFFT_3_S16_S16C_Mod")));
__typeof__(__mlib_SignalIFFT_3_S16_S16_Mod) mlib_SignalIFFT_3_S16_S16_Mod
    __attribute__((weak, alias("__mlib_SignalIFFT_3_S16_S16_Mod")));

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

#define	SERVE_BAD_POINTER(ptr)                                  \
		if (!(ptr))                                     \
			return MLIB_NULLPOINTER

/* *********************************************************** */

/* functions codes */

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_3_S16C_S16C_Mod(
    mlib_s16 *dstc,
    const mlib_s16 *srcc,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcc);
	return mlib_c_fftDispatcher_3(dstc, NULL, (void *)srcc, NULL, order,
	    eSrcCmplx | eDstCmplx);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_3_S16_S16_Mod(
    mlib_s16 *dstr,
    mlib_s16 *dsti,
    const mlib_s16 *srcr,
    const mlib_s16 *srci,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstr);
	SERVE_BAD_POINTER(dsti);
	SERVE_BAD_POINTER(srcr);
	SERVE_BAD_POINTER(srci);
	return mlib_c_fftDispatcher_3(dstr, dsti, (void *)srcr, (void *)srci,
	    order, eSrcReIm | eDstReIm);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_3_S16C_S16_Mod(
    mlib_s16 *dstc,
    const mlib_s16 *srcr,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcr);
	return mlib_c_fftDispatcherW_S16C_S16(dstc, NULL, (void *)srcr, NULL,
	    NULL, order, order / 2, eSrcRe | eDstCmplx);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_3_S16C_S16C_Mod(
    mlib_s16 *dstc,
    const mlib_s16 *srcc,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcc);
	return mlib_c_fftDispatcher_3(dstc, NULL, (void *)srcc, NULL, order,
	    eSrcCmplx | eDstCmplx | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_3_S16_S16_Mod(
    mlib_s16 *dstr,
    mlib_s16 *dsti,
    const mlib_s16 *srcr,
    const mlib_s16 *srci,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstr);
	SERVE_BAD_POINTER(dsti);
	SERVE_BAD_POINTER(srcr);
	SERVE_BAD_POINTER(srci);
	return mlib_c_fftDispatcher_3(dstr, dsti, (void *)srcr, (void *)srci,
	    order, eSrcReIm | eDstReIm | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_3_S16_S16C_Mod(
    mlib_s16 *dstr,
    const mlib_s16 *srcc,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(srcc);
	SERVE_BAD_POINTER(dstr);
	return mlib_c_fftDispatcher_3(dstr, NULL, (void *)srcc, NULL, order,
	    eSrcCmplx | eDstRe | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_3_S16C_Mod(
    mlib_s16 *datac,
    mlib_s32 order)
{
	return (__mlib_SignalFFT_3_S16C_S16C_Mod(datac, datac, order));
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_3_S16_Mod(
    mlib_s16 *datar,
    mlib_s16 *datai,
    mlib_s32 order)
{
	return __mlib_SignalFFT_3_S16_S16_Mod(datar, datai, datar, datai,
	    order);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_3_S16C_Mod(
    mlib_s16 *datac,
    mlib_s32 order)
{
	return (__mlib_SignalIFFT_3_S16C_S16C_Mod(datac, datac, order));
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_3_S16_Mod(
    mlib_s16 *datar,
    mlib_s16 *datai,
    mlib_s32 order)
{
	return __mlib_SignalIFFT_3_S16_S16_Mod(datar, datai, datar, datai,
	    order);
}

/* *********************************************************** */
