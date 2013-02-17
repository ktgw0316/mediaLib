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

#pragma ident	"@(#)mlib_SignalFFT_D64_1.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *  NOTE: FFT below means F(ast)F(ourier)T(ransform)
 *      mlib_SignalFFT_1_D64C_D64C   -     FFT of mlib_d64 format
 *                                         complex array into
 *                                         complex array
 *      mlib_SignalFFT_1_D64_D64     -     FFT of mlib_d64 format
 *                                         2 arrays as real and
 *                                         imag parts of complex
 *                                         into 2 arrays as
 *                                         real and imag parts of complex
 *      mlib_SignalFFT_1_D64C_D64    -     FFT of mlib_d64 format
 *                                         1 array as real part
 *                                         of( real-valued )
 *                                         complex into complex array
 *      mlib_SignalIFFT_1_D64C_D64C      - inverse FFT
 *                                         of mlib_d64 format complex array
 *                                         into complex array
 *      mlib_SignalIFFT_1_D64_D64        - inverse FFT
 *                                         of mlib_d64 format 2 arrays as real
 *                                         and imag parts of complex
 *                                         into 2 arrays as
 *                                         real and imag parts of complex
 *      mlib_SignalIFFT_1_D64_D64C       - inverse FFT
 *                                         of mlib_d64 format complex array
 *                                         into 1 array as real part
 *                                         of( real-valued ) complex
 *                                         NOTE: this is a semantic pair
 *                                           to mlib_SignalFFT_1_D64C_D64
 *      mlib_SignalFFT_1_D64C        -     FFT of mlib_d64 format complex array
 *                                         into complex array
 *                                         (input and output arrays
 *                                         are the same)
 *      mlib_SignalFFT_1_D64         -     FFT of mlib_d64 format
 *                                         2 arrays as real and
 *                                         imag parts of complex
 *                                         into 2 arrays as
 *                                         real and imag parts of complex
 *                                         (input and output array pairs
 *                                         are the same)
 *      mlib_SignalIFFT_1_D64C           - inverse FFT
 *                                         of mlib_d64 format complex array
 *                                         into complex array
 *                                         (input and output arrays
 *                                         are the same)
 *      mlib_SignalIFFT_1_D64            - inverse FFT of mlib_d64 format
 *                                         2 arrays as real
 *                                         and imag parts of complex
 *                                         into 2 arrays as
 *                                         real and imag parts of complex
 *                                         (input and output array pairs
 *                                         are the same)
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalFFT_1_D64(data_t *datar,
 *                                       data_t *datai,
 *                                       mlib_s32 order);
 *      mlib_status mlib_SignalFFT_1_D64C(data_t *datac,
 *                                        mlib_s32 order);
 *      mlib_status mlib_SignalFFT_1_D64C_D64(data_t *dstc,
 *                                            data_t *srcr,
 *                                            mlib_s32 order);
 *      mlib_status mlib_SignalFFT_1_D64C_D64C(data_t *dstc,
 *                                             data_t *srcc,
 *                                             mlib_s32 order);
 *      mlib_status mlib_SignalFFT_1_D64_D64(data_t *dstr,
 *                                           data_t *dsti,
 *                                           data_t *srcr,
 *                                           data_t *srci,
 *                                           mlib_s32 order);
 *      mlib_status mlib_SignalIFFT_1_D64(data_t *datar,
 *                                        data_t *datai,
 *                                        mlib_s32 order);
 *      mlib_status mlib_SignalIFFT_1_D64C(data_t *datac,
 *                                         mlib_s32 order);
 *      mlib_status mlib_SignalIFFT_1_D64C_D64C(data_t *dstc,
 *                                              data_t *srcc,
 *                                              mlib_s32 order);
 *      mlib_status mlib_SignalIFFT_1_D64_D64(data_t *dstr,
 *                                            data_t *dsti,
 *                                            data_t *srcr,
 *                                            data_t *srci,
 *                                            mlib_s32 order);
 *      mlib_status mlib_SignalIFFT_1_D64_D64C(data_t *dstr,
 *                                             data_t *srcc,
 *                                             mlib_s32 order);
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
 *  parameter set to NULL ( see also: mlib_c_fftDispatcher_1 macro below )
 *
 */

#include <mlib_signal.h>

#define	FFT_UTIL_C

#include <mlib_SignalFFTUtil.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalFFT_1_D64 = __mlib_SignalFFT_1_D64
#pragma weak mlib_SignalFFT_1_D64C = __mlib_SignalFFT_1_D64C
#pragma weak mlib_SignalFFT_1_D64C_D64 = __mlib_SignalFFT_1_D64C_D64
#pragma weak mlib_SignalFFT_1_D64C_D64C = __mlib_SignalFFT_1_D64C_D64C
#pragma weak mlib_SignalFFT_1_D64_D64 = __mlib_SignalFFT_1_D64_D64
#pragma weak mlib_SignalIFFT_1_D64 = __mlib_SignalIFFT_1_D64
#pragma weak mlib_SignalIFFT_1_D64C = __mlib_SignalIFFT_1_D64C
#pragma weak mlib_SignalIFFT_1_D64C_D64C = __mlib_SignalIFFT_1_D64C_D64C
#pragma weak mlib_SignalIFFT_1_D64_D64 = __mlib_SignalIFFT_1_D64_D64
#pragma weak mlib_SignalIFFT_1_D64_D64C = __mlib_SignalIFFT_1_D64_D64C

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalFFT_1_D64) mlib_SignalFFT_1_D64
    __attribute__((weak, alias("__mlib_SignalFFT_1_D64")));
__typeof__(__mlib_SignalFFT_1_D64C) mlib_SignalFFT_1_D64C
    __attribute__((weak, alias("__mlib_SignalFFT_1_D64C")));
__typeof__(__mlib_SignalFFT_1_D64C_D64) mlib_SignalFFT_1_D64C_D64
    __attribute__((weak, alias("__mlib_SignalFFT_1_D64C_D64")));
__typeof__(__mlib_SignalFFT_1_D64C_D64C) mlib_SignalFFT_1_D64C_D64C
    __attribute__((weak, alias("__mlib_SignalFFT_1_D64C_D64C")));
__typeof__(__mlib_SignalFFT_1_D64_D64) mlib_SignalFFT_1_D64_D64
    __attribute__((weak, alias("__mlib_SignalFFT_1_D64_D64")));
__typeof__(__mlib_SignalIFFT_1_D64) mlib_SignalIFFT_1_D64
    __attribute__((weak, alias("__mlib_SignalIFFT_1_D64")));
__typeof__(__mlib_SignalIFFT_1_D64C) mlib_SignalIFFT_1_D64C
    __attribute__((weak, alias("__mlib_SignalIFFT_1_D64C")));
__typeof__(__mlib_SignalIFFT_1_D64C_D64C) mlib_SignalIFFT_1_D64C_D64C
    __attribute__((weak, alias("__mlib_SignalIFFT_1_D64C_D64C")));
__typeof__(__mlib_SignalIFFT_1_D64_D64) mlib_SignalIFFT_1_D64_D64
    __attribute__((weak, alias("__mlib_SignalIFFT_1_D64_D64")));
__typeof__(__mlib_SignalIFFT_1_D64_D64C) mlib_SignalIFFT_1_D64_D64C
    __attribute__((weak, alias("__mlib_SignalIFFT_1_D64_D64C")));

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

/* scaling coeffs */

/* *********************************************************** */

#define	DCL_CALC_SCALE_1_INV(shiftscale, order)                 \
	mlib_s32 shiftscale = 0

/* *********************************************************** */

#define	DCL_CALC_SCALE_1_DIR(shiftscale, order)                 \
	mlib_s32 shiftscale = (order)

/* *********************************************************** */

/* functions codes */

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_1_D64C_D64C(
    data_t *dstc,
    const data_t *srcc,
    mlib_s32 order)
{
	DCL_CALC_SCALE_1_DIR(shiftscale, order);
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcc);
	return mlib_fftDispatcher_D64C_D64C(dstc, NULL, (void *)srcc, NULL,
	    order, shiftscale, DIRECT_FFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_1_D64_D64(
    data_t *dstr,
    data_t *dsti,
    const data_t *srcr,
    const data_t *srci,
    mlib_s32 order)
{
	DCL_CALC_SCALE_1_DIR(shiftscale, order);
	SERVE_BAD_POINTER(dstr);
	SERVE_BAD_POINTER(dsti);
	SERVE_BAD_POINTER(srcr);
	SERVE_BAD_POINTER(srci);
	return mlib_fftDispatcher_D64_D64(dstr, dsti, (void *)srcr,
	    (void *)srci, order, shiftscale, DIRECT_FFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_1_D64C_D64(
    data_t *dstc,
    const data_t *srcr,
    mlib_s32 order)
{
	DCL_CALC_SCALE_1_DIR(shiftscale, order);
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcr);
	return mlib_fftDispatcher_D64C_D64(dstc, NULL, (void *)srcr, NULL,
	    order, shiftscale, DIRECT_FFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_1_D64C_D64C(
    data_t *dstc,
    const data_t *srcc,
    mlib_s32 order)
{
	DCL_CALC_SCALE_1_INV(shiftscale, order);
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcc);
	return mlib_fftDispatcher_D64C_D64C(dstc, NULL, (void *)srcc, NULL,
	    order, shiftscale, INVERS_FFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_1_D64_D64(
    data_t *dstr,
    data_t *dsti,
    const data_t *srcr,
    const data_t *srci,
    mlib_s32 order)
{
	DCL_CALC_SCALE_1_INV(shiftscale, order);
	SERVE_BAD_POINTER(dstr);
	SERVE_BAD_POINTER(dsti);
	SERVE_BAD_POINTER(srcr);
	SERVE_BAD_POINTER(srci);
	return mlib_fftDispatcher_D64_D64(dstr, dsti, (void *)srcr,
	    (void *)srci, order, shiftscale, INVERS_FFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_1_D64_D64C(
    data_t *dstr,
    const data_t *srcc,
    mlib_s32 order)
{
	DCL_CALC_SCALE_1_INV(shiftscale, order);
	SERVE_BAD_POINTER(srcc);
	SERVE_BAD_POINTER(dstr);
	return mlib_fftDispatcher_D64_D64C(dstr, NULL, (void *)srcc, NULL,
	    order, shiftscale, INVERS_FFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_1_D64C(
    data_t *datac,
    mlib_s32 order)
{
	DCL_CALC_SCALE_1_DIR(shiftscale, order);
	SERVE_BAD_POINTER(datac);
	return mlib_fftDispatcher_D64C(datac, NULL, NULL, NULL, order,
	    shiftscale, DIRECT_FFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_1_D64(
    data_t *datar,
    data_t *datai,
    mlib_s32 order)
{
	DCL_CALC_SCALE_1_DIR(shiftscale, order);
	SERVE_BAD_POINTER(datar);
	SERVE_BAD_POINTER(datai);
	return mlib_fftDispatcher_D64(datar, datai, NULL, NULL, order,
	    shiftscale, DIRECT_FFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_1_D64C(
    data_t *datac,
    mlib_s32 order)
{
	DCL_CALC_SCALE_1_INV(shiftscale, order);
	SERVE_BAD_POINTER(datac);
	return mlib_fftDispatcher_D64C(datac, NULL, NULL, NULL, order,
	    shiftscale, INVERS_FFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_1_D64(
    data_t *datar,
    data_t *datai,
    mlib_s32 order)
{
	DCL_CALC_SCALE_1_INV(shiftscale, order);
	SERVE_BAD_POINTER(datar);
	SERVE_BAD_POINTER(datai);
	return mlib_fftDispatcher_D64(datar, datai, NULL, NULL, order,
	    shiftscale, INVERS_FFT);
}

/* *********************************************************** */
