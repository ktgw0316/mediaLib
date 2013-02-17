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

#pragma ident	"@(#)mlib_s_SignalFFT_1.c	9.5	07/11/05 SMI"

/*
 * FUNCTIONS
 *  NOTE: FFT below means F(ast)F(ourier)T(ransform)
 *      mlib_SignalFFT_1_S16C_S16C_Mod   - FFT of 16-bit format
 *                                         complex array into
 *                                         complex array
 *      mlib_SignalFFT_1_S16_S16_Mod     - FFT of 16-bit format
 *                                         2 arrays as real and
 *                                         imag parts of complex
 *                                         into 2 arrays as
 *                                         real and imag parts of complex
 *      mlib_SignalFFT_1_S16C_S16_Mod    - FFT of 16bit format
 *                                         1 array as real part
 *                                         of( real-valued ) complex
 *                                         into complex array
 *      mlib_SignalIFFT_1_S16C_S16C      - inverse FFT
 *                                         of 16-bit format complex array
 *                                         into complex array
 *      mlib_SignalIFFT_1_S16_S16        - inverse FFT
 *                                         of 16-bit format 2 arrays as real
 *                                         and imag parts of complex
 *                                         into 2 arrays as
 *                                         real and imag parts of complex
 *      mlib_SignalIFFT_1_S16_S16C       - inverse FFT
 *                                         of 16bit format complex array
 *                                         into 1 array as real part
 *                                         of( real-valued ) complex
 *                                         NOTE: this is a semantic pair
 *                                           to mlib_SignalFFT_1_S16C_S16_Mod
 *      mlib_SignalFFT_1_S16C_Mod        - FFT of 16-bit format
 *                                         complex array into complex array
 *                                         (input and output arrays
 *                                         are the same)
 *      mlib_SignalFFT_1_S16_Mod         - FFT of 16-bit format
 *                                         2 arrays as real and
 *                                         imag parts of complex
 *                                         into 2 arrays as
 *                                         real and imag parts of complex
 *                                         (input and output array pairs
 *                                         are the same)
 *      mlib_SignalIFFT_1_S16C           - inverse FFT of 16-bit format
 *                                         complex array into complex array
 *                                         (input and output arrays
 *                                         are the same)
 *      mlib_SignalIFFT_1_S16            - inverse FFT of 16-bit format
 *                                         2 arrays as real
 *                                         and imag parts of complex
 *                                         into 2 arrays as
 *                                         real and imag parts of complex
 *                                         (input and output array pairs
 *                                         are the same)
 *
 * SYNOPSIS
 *
 *
 *      mlib_status mlib_SignalFFT_1_S16C_Mod(mlib_s16 *datac,
 *                                            mlib_s32 order);
 *      mlib_status mlib_SignalFFT_1_S16C_S16C_Mod(mlib_s16       *dstc,
 *                                                 const mlib_s16 *srcc,
 *                                                 mlib_s32       order);
 *      mlib_status mlib_SignalFFT_1_S16C_S16_Mod(mlib_s16       *dstc,
 *                                                const mlib_s16 *srcr,
 *                                                mlib_s32       order);
 *      mlib_status mlib_SignalFFT_1_S16_Mod(mlib_s16 *datar,
 *                                           mlib_s16 *datai,
 *                                           mlib_s32 order);
 *      mlib_status mlib_SignalFFT_1_S16_S16_Mod(mlib_s16       *dstr,
 *                                               mlib_s16       *dsti,
 *                                               const mlib_s16 *srcr,
 *                                               const mlib_s16 *srci,
 *                                               mlib_s32       order);
 *      mlib_status mlib_SignalIFFT_1_S16(mlib_s16 *datar,
 *                                        mlib_s16 *datai,
 *                                        mlib_s32 order);
 *      mlib_status mlib_SignalIFFT_1_S16C(mlib_s16 *datac,
 *                                         mlib_s32 order);
 *      mlib_status mlib_SignalIFFT_1_S16C_S16C(mlib_s16       *dstc,
 *                                              const mlib_s16 *srcc,
 *                                              mlib_s32       order);
 *      mlib_status mlib_SignalIFFT_1_S16_S16(mlib_s16       *dstr,
 *                                            mlib_s16       *dsti,
 *                                            const mlib_s16 *srcr,
 *                                            const mlib_s16 *srci,
 *                                            mlib_s32       order);
 *      mlib_status mlib_SignalIFFT_1_S16_S16C(mlib_s16       *dstr,
 *                                             const mlib_s16 *srcc,
 *                                             mlib_s32       order);
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
 * Note1. Not-windowed FFT functions call dispatcher with it 'window'
 *  parameter set to NULL ( see also: mlib_s_fftDispatcher_1 macro in header )
 * Note2. Functions like mlib*FFT_1_S16 and mlib*FFT_1_S16C are trivially
 *  mapped into mlib*FFT_1_S16_S16 and mlib*FFT_1_S16C_S16C.
 * Note3. All functions ( regarding Note2 mapping ) have "_0_4-shadows":
 *  funcname -> funcname##_0_4 providing processing for orders 0 - 4
 */

#include <mlib_signal.h>
#include <mlib_s_SignalFFTUtil.h>
#include <mlib_s_SignalFFT_x_Macros.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalFFT_1_S16C_Mod = __mlib_SignalFFT_1_S16C_Mod
#pragma weak mlib_SignalFFT_1_S16C_S16C_Mod = \
	__mlib_SignalFFT_1_S16C_S16C_Mod
#pragma weak mlib_SignalFFT_1_S16C_S16_Mod = __mlib_SignalFFT_1_S16C_S16_Mod
#pragma weak mlib_SignalFFT_1_S16_Mod = __mlib_SignalFFT_1_S16_Mod
#pragma weak mlib_SignalFFT_1_S16_S16_Mod = __mlib_SignalFFT_1_S16_S16_Mod
#pragma weak mlib_SignalIFFT_1_S16 = __mlib_SignalIFFT_1_S16
#pragma weak mlib_SignalIFFT_1_S16C = __mlib_SignalIFFT_1_S16C
#pragma weak mlib_SignalIFFT_1_S16C_S16C = __mlib_SignalIFFT_1_S16C_S16C
#pragma weak mlib_SignalIFFT_1_S16_S16 = __mlib_SignalIFFT_1_S16_S16
#pragma weak mlib_SignalIFFT_1_S16_S16C = __mlib_SignalIFFT_1_S16_S16C

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalFFT_1_S16C_Mod) mlib_SignalFFT_1_S16C_Mod
    __attribute__((weak, alias("__mlib_SignalFFT_1_S16C_Mod")));
__typeof__(__mlib_SignalFFT_1_S16C_S16C_Mod) mlib_SignalFFT_1_S16C_S16C_Mod
    __attribute__((weak, alias("__mlib_SignalFFT_1_S16C_S16C_Mod")));
__typeof__(__mlib_SignalFFT_1_S16C_S16_Mod) mlib_SignalFFT_1_S16C_S16_Mod
    __attribute__((weak, alias("__mlib_SignalFFT_1_S16C_S16_Mod")));
__typeof__(__mlib_SignalFFT_1_S16_Mod) mlib_SignalFFT_1_S16_Mod
    __attribute__((weak, alias("__mlib_SignalFFT_1_S16_Mod")));
__typeof__(__mlib_SignalFFT_1_S16_S16_Mod) mlib_SignalFFT_1_S16_S16_Mod
    __attribute__((weak, alias("__mlib_SignalFFT_1_S16_S16_Mod")));
__typeof__(__mlib_SignalIFFT_1_S16) mlib_SignalIFFT_1_S16
    __attribute__((weak, alias("__mlib_SignalIFFT_1_S16")));
__typeof__(__mlib_SignalIFFT_1_S16C) mlib_SignalIFFT_1_S16C
    __attribute__((weak, alias("__mlib_SignalIFFT_1_S16C")));
__typeof__(__mlib_SignalIFFT_1_S16C_S16C) mlib_SignalIFFT_1_S16C_S16C
    __attribute__((weak, alias("__mlib_SignalIFFT_1_S16C_S16C")));
__typeof__(__mlib_SignalIFFT_1_S16_S16) mlib_SignalIFFT_1_S16_S16
    __attribute__((weak, alias("__mlib_SignalIFFT_1_S16_S16")));
__typeof__(__mlib_SignalIFFT_1_S16_S16C) mlib_SignalIFFT_1_S16_S16C
    __attribute__((weak, alias("__mlib_SignalIFFT_1_S16_S16C")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

/* functions codes */

static mlib_status
mlib_s_SignalFFT_S16C_S16C_0_4(
    mlib_s16 *dstc,
    const mlib_s16 *srcc,
    mlib_s32 order)
{
	switch (order) {
	case 0:
		dstc[0] = srcc[0];
		dstc[1] = srcc[1];
		break;

	case 1: {
		    mlib_s32 r0, r1, i0, i1;

		    r0 = srcc[0];
		    i0 = srcc[1];
		    r1 = srcc[2];
		    i1 = srcc[3];
		    dstc[0] = r0 + r1;
		    dstc[1] = i0 + i1;
		    dstc[2] = r0 - r1;
		    dstc[3] = i0 - i1;
	    }

		break;

	case 2: {
		    DCL_2;
		    LOAD_2C;
		    FFT2_KERNEL;

		    dstc[0] = rr0 + rr2;
		    dstc[1] = ii0 + ii2;
		    dstc[2] = rr1 + ii3;
		    dstc[3] = ii1 - rr3;
		    dstc[4] = rr0 - rr2;
		    dstc[5] = ii0 - ii2;
		    dstc[6] = rr1 - ii3;
		    dstc[7] = ii1 + rr3;
	    }

		break;

	case 3: {
		    DCL_3;
		    LOAD_3C;
		    FFT3_KERNEL;

		    dstc[0] = r0 + r2;
		    dstc[1] = i0 + i2;
		    dstc[2] = r4 + rr1;
		    dstc[3] = i5 - rr3;
		    dstc[4] = r1 + i3;
		    dstc[5] = i1 - r3;
		    dstc[6] = r5 - rr0;
		    dstc[7] = i4 - rr2;
		    dstc[8] = r0 - r2;
		    dstc[9] = i0 - i2;
		    dstc[10] = r4 - rr1;
		    dstc[11] = i5 + rr3;
		    dstc[12] = r1 - i3;
		    dstc[13] = i1 + r3;
		    dstc[14] = r5 + rr0;
		    dstc[15] = i4 + rr2;
	    }

		break;

	case 4: {
		    __m128i reim0, reim1, reim2, reim3, reim4, reim5, reim6,
			reim7, tmp;

		    DCL_COMMON;
		    DCL_OUTVARR;

		    PREP_OUTC;

		    LOAD_REIM;

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
mlib_s_SignalFFT_S16_S16_0_4(
    mlib_s16 *dstr,
    mlib_s16 *dsti,
    const mlib_s16 *srcr,
    const mlib_s16 *srci,
    mlib_s32 order)
{
	switch (order) {
	case 0:
		dstr[0] = srcr[0];
		dsti[0] = srci[0];
		break;

	case 1: {
		    mlib_s32 r0, r1, i0, i1;

		    r0 = srcr[0];
		    r1 = srcr[1];
		    i0 = srci[0];
		    i1 = srci[1];
		    dstr[0] = r0 + r1;
		    dstr[1] = r0 - r1;
		    dsti[0] = i0 + i1;
		    dsti[1] = i0 - i1;
	    }

		break;

	case 2: {
		    DCL_2;
		    LOAD_2;
		    FFT2_KERNEL;

		    dstr[0] = rr0 + rr2;
		    dstr[1] = rr1 + ii3;
		    dstr[2] = rr0 - rr2;
		    dstr[3] = rr1 - ii3;
		    dsti[0] = ii0 + ii2;
		    dsti[1] = ii1 - rr3;
		    dsti[2] = ii0 - ii2;
		    dsti[3] = ii1 + rr3;
	    }

		break;

	case 3: {
		    DCL_3;
		    LOAD_3;
		    FFT3_KERNEL;

		    dstr[0] = r0 + r2;
		    dstr[1] = r4 + rr1;
		    dstr[2] = r1 + i3;
		    dstr[3] = r5 - rr0;
		    dstr[4] = r0 - r2;
		    dstr[5] = r4 - rr1;
		    dstr[6] = r1 - i3;
		    dstr[7] = r5 + rr0;

		    dsti[0] = i0 + i2;
		    dsti[1] = i5 - rr3;
		    dsti[2] = i1 - r3;
		    dsti[3] = i4 - rr2;
		    dsti[4] = i0 - i2;
		    dsti[5] = i5 + rr3;
		    dsti[6] = i1 + r3;
		    dsti[7] = i4 + rr2;
	    }

		break;

	case 4: {
		    DCL_COMMON;
		    DCL_OUTVARR;
		    DCL_OUTVARI;

		    PREP_OUTR;
		    PREP_OUTI;

		    LOAD_REAL;
		    LOAD_IMAG;

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
mlib_s_SignalFFT_S16C_S16_0_4(
    mlib_s16 *dstc,
    const mlib_s16 *srcr,
    mlib_s32 order)
{
	switch (order) {
	case 0:
		dstc[0] = srcr[0];
		dstc[1] = 0;
		break;

	case 1: {
		    mlib_s32 r0, r1;

		    r0 = srcr[0];
		    r1 = srcr[1];
		    dstc[0] = r0 + r1;
		    dstc[1] = 0;
		    dstc[2] = r0 - r1;
		    dstc[3] = 0;
	    }

		break;

	case 2: {
		    mlib_s32 r0, r1, r2, r3;
		    mlib_s32 rr0, rr1, rr2, rr3;

		    r0 = srcr[0];
		    r1 = srcr[1];
		    r2 = srcr[2];
		    r3 = srcr[3];
		    rr0 = r0 + r2;
		    rr1 = r0 - r2;
		    rr2 = r1 + r3;
		    rr3 = r1 - r3;
		    dstc[0] = rr0 + rr2;
		    dstc[1] = 0;
		    dstc[2] = rr1;
		    dstc[3] = -rr3;
		    dstc[4] = rr0 - rr2;
		    dstc[5] = 0;
		    dstc[6] = rr1;
		    dstc[7] = rr3;
	    }

		break;

	case 3: {
		    mlib_s32 r0, r1, r2, r3, r4, r5, r6, r7;
		    mlib_s32 rr0, rr1, rr2, rr3, rr4, rr5, rr6, rr7;
		    mlib_d64 coeff = 0.707106781;

		    r0 = srcr[0];
		    r1 = srcr[1];
		    r2 = srcr[2];
		    r3 = srcr[3];
		    r4 = srcr[4];
		    r5 = srcr[5];
		    r6 = srcr[6];
		    r7 = srcr[7];

		    rr0 = r0 + r4;
		    rr1 = r0 - r4;
		    rr2 = r1 + r5;
		    rr3 = r1 - r5;

		    rr4 = r2 + r6;
		    rr5 = r2 - r6;
		    rr6 = r3 + r7;
		    rr7 = r3 - r7;

		    r0 = rr0 + rr4;
		    r1 = rr0 - rr4;
		    r2 = rr2 + rr6;
		    r3 = rr2 - rr6;

		    rr0 = (rr3 - rr7) * coeff;
		    rr2 = (rr3 + rr7) * coeff;

		    r4 = rr1 + rr0;
		    r5 = rr1 - rr0;
		    r6 = rr5 - rr2;
		    r7 = rr5 + rr2;

		    dstc[0] = r0 + r2;
		    dstc[1] = 0;
		    dstc[2] = r4;
		    dstc[3] = -r7;
		    dstc[4] = r1;
		    dstc[5] = -r3;
		    dstc[6] = r5;
		    dstc[7] = r6;
		    dstc[8] = r0 - r2;
		    dstc[9] = 0;
		    dstc[10] = r5;
		    dstc[11] = -r6;
		    dstc[12] = r1;
		    dstc[13] = r3;
		    dstc[14] = r4;
		    dstc[15] = r7;
	    }

		break;

	case 4: {
		    __m128i reim0, reim1, reim2, reim3, reim4, reim5, reim6,
			reim7;

		    DCL_COMMON;
		    DCL_OUTVARR;

		    PREP_OUTC;

		    LOAD_REAL;

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
mlib_s_SignalIFFT_S16C_S16C_0_4(
    mlib_s16 *dstc,
    const mlib_s16 *srcc,
    mlib_s32 order)
{
	switch (order) {
	case 0:
		dstc[0] = srcc[0];
		dstc[1] = srcc[1];
		break;

	case 1: {
		    mlib_s32 r0, r1, i0, i1;

		    r0 = srcc[0];
		    i0 = srcc[1];
		    r1 = srcc[2];
		    i1 = srcc[3];
		    dstc[0] = (r0 + r1) >> 1;
		    dstc[1] = (i0 + i1) >> 1;
		    dstc[2] = (r0 - r1) >> 1;
		    dstc[3] = (i0 - i1) >> 1;
	    }

		break;

	case 2: {
		    DCL_2;
		    LOAD_2C;
		    FFT2_KERNEL;

		    dstc[0] = (rr0 + rr2) >> 2;
		    dstc[1] = (ii0 + ii2) >> 2;
		    dstc[2] = (rr1 - ii3) >> 2;
		    dstc[3] = (ii1 + rr3) >> 2;
		    dstc[4] = (rr0 - rr2) >> 2;
		    dstc[5] = (ii0 - ii2) >> 2;
		    dstc[6] = (rr1 + ii3) >> 2;
		    dstc[7] = (ii1 - rr3) >> 2;
	    }

		break;

	case 3: {
		    DCL_3;
		    LOAD_3C;
		    FFT3_KERNEL;

		    dstc[0] = (r0 + r2) >> 3;
		    dstc[1] = (i0 + i2) >> 3;
		    dstc[2] = (r5 + rr0) >> 3;
		    dstc[3] = (i4 + rr2) >> 3;
		    dstc[4] = (r1 - i3) >> 3;
		    dstc[5] = (i1 + r3) >> 3;
		    dstc[6] = (r4 - rr1) >> 3;
		    dstc[7] = (i5 + rr3) >> 3;
		    dstc[8] = (r0 - r2) >> 3;
		    dstc[9] = (i0 - i2) >> 3;
		    dstc[10] = (r5 - rr0) >> 3;
		    dstc[11] = (i4 - rr2) >> 3;
		    dstc[12] = (r1 + i3) >> 3;
		    dstc[13] = (i1 - r3) >> 3;
		    dstc[14] = (r4 + rr1) >> 3;
		    dstc[15] = (i5 - rr3) >> 3;
	    }

		break;

	case 4: {
		    mlib_s32 divider = 2;
		    DEF_ADDER(divider);
		    __m128i reim0, reim1, reim2, reim3, reim4, reim5, reim6,
			reim7, tmp;

		    DCL_COMMON;
		    DCL_OUTVARR;

		    PREP_OUTC;

		    LOAD_REIM;

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
mlib_s_SignalIFFT_S16_S16_0_4(
    mlib_s16 *dstr,
    mlib_s16 *dsti,
    const mlib_s16 *srcr,
    const mlib_s16 *srci,
    mlib_s32 order)
{
	switch (order) {
	case 0:
		dstr[0] = srcr[0];
		dsti[0] = srci[0];
		break;

	case 1: {
		    mlib_s32 r0, r1, i0, i1;

		    r0 = srcr[0];
		    r1 = srcr[1];
		    i0 = srci[0];
		    i1 = srci[1];
		    dstr[0] = (r0 + r1) >> 1;
		    dstr[1] = (r0 - r1) >> 1;
		    dsti[0] = (i0 + i1) >> 1;
		    dsti[1] = (i0 - i1) >> 1;
	    }

		break;

	case 2: {
		    DCL_2;
		    LOAD_2;
		    FFT2_KERNEL;

		    dstr[0] = (rr0 + rr2) >> 2;
		    dstr[1] = (rr1 - ii3) >> 2;
		    dstr[2] = (rr0 - rr2) >> 2;
		    dstr[3] = (rr1 + ii3) >> 2;
		    dsti[0] = (ii0 + ii2) >> 2;
		    dsti[1] = (ii1 + rr3) >> 2;
		    dsti[2] = (ii0 - ii2) >> 2;
		    dsti[3] = (ii1 - rr3) >> 2;
	    }

		break;

	case 3: {
		    DCL_3;
		    LOAD_3;
		    FFT3_KERNEL;

		    dstr[0] = (r0 + r2) >> 3;
		    dstr[1] = (r5 + rr0) >> 3;
		    dstr[2] = (r1 - i3) >> 3;
		    dstr[3] = (r4 - rr1) >> 3;
		    dstr[4] = (r0 - r2) >> 3;
		    dstr[5] = (r5 - rr0) >> 3;
		    dstr[6] = (r1 + i3) >> 3;
		    dstr[7] = (r4 + rr1) >> 3;

		    dsti[0] = (i0 + i2) >> 3;
		    dsti[1] = (i4 + rr2) >> 3;
		    dsti[2] = (i1 + r3) >> 3;
		    dsti[3] = (i5 + rr3) >> 3;
		    dsti[4] = (i0 - i2) >> 3;
		    dsti[5] = (i4 - rr2) >> 3;
		    dsti[6] = (i1 - r3) >> 3;
		    dsti[7] = (i5 - rr3) >> 3;
	    }

		break;

	case 4: {
		    mlib_s32 divider = 2;
		    DEF_ADDER(divider);
		    __m128i tmp;

		    DCL_COMMON;
		    DCL_OUTVARR;
		    DCL_OUTVARI;

		    PREP_OUTR;
		    PREP_OUTI;

		    LOAD_REAL;
		    LOAD_IMAG;

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
mlib_s_SignalIFFT_S16_S16C_0_4(
    mlib_s16 *dstr,
    const mlib_s16 *srcc,
    mlib_s32 order)
{
	switch (order) {
	case 0:
		dstr[0] = srcc[0];
		break;

	case 1: {
		    mlib_s32 r0, r1;

		    r0 = srcc[0];
		    r1 = srcc[2];
		    dstr[0] = (r0 + r1) >> 1;
		    dstr[1] = (r0 - r1) >> 1;
	    }

		break;

	case 2: {
		    mlib_s32 r0, r1, r2, r3;
		    mlib_s32 i1, i3;
		    mlib_s32 rr0, rr1, rr2, ii3;

		    r0 = srcc[0];
		    r1 = srcc[2];
		    i1 = srcc[3];
		    r2 = srcc[4];
		    r3 = srcc[6];
		    i3 = srcc[7];

		    rr0 = r0 + r2;
		    rr1 = r0 - r2;
		    rr2 = r1 + r3;
		    ii3 = i1 - i3;
		    dstr[0] = (rr0 + rr2) >> 2;
		    dstr[1] = (rr1 - ii3) >> 2;
		    dstr[2] = (rr0 - rr2) >> 2;
		    dstr[3] = (rr1 + ii3) >> 2;
	    }

		break;

	case 3: {
		    mlib_s32 r0, r1, r2, r3, r4, r5, r6, r7;
		    mlib_s32 i1, i2, i3, i5, i6, i7;
		    mlib_s32 rr0, rr1, rr2, rr3, rr4, rr6, rr7;
		    mlib_s32 ii2, ii3, ii5, ii6, ii7;
		    mlib_d64 coeff = 0.707106781;

		    r0 = srcc[0];
		    r1 = srcc[2];
		    i1 = srcc[3];
		    r2 = srcc[4];
		    i2 = srcc[5];
		    r3 = srcc[6];
		    i3 = srcc[7];
		    r4 = srcc[8];
		    r5 = srcc[10];
		    i5 = srcc[11];
		    r6 = srcc[12];
		    i6 = srcc[13];
		    r7 = srcc[14];
		    i7 = srcc[15];

		    rr0 = r0 + r4;
		    rr1 = r0 - r4;
		    rr2 = r1 + r5;
		    rr3 = r1 - r5;

		    rr4 = r2 + r6;
		    rr6 = r3 + r7;
		    rr7 = r3 - r7;

		    ii2 = i1 + i5;
		    ii3 = i1 - i5;
		    ii5 = i2 - i6;
		    ii6 = i3 + i7;
		    ii7 = i3 - i7;

		    r0 = rr0 + rr4;
		    r1 = rr0 - rr4;
		    r2 = rr2 + rr6;

		    r4 = rr1 + ii5;
		    r5 = rr1 - ii5;
		    r6 = rr3 + ii7;
		    r7 = rr3 - ii7;

		    i3 = ii2 - ii6;

		    i6 = ii3 + rr7;
		    i7 = ii3 - rr7;

		    rr0 = (r7 - i6) * coeff;
		    rr1 = (r6 + i7) * coeff;

		    dstr[0] = (r0 + r2) >> 3;
		    dstr[1] = (r5 + rr0) >> 3;
		    dstr[2] = (r1 - i3) >> 3;
		    dstr[3] = (r4 - rr1) >> 3;
		    dstr[4] = (r0 - r2) >> 3;
		    dstr[5] = (r5 - rr0) >> 3;
		    dstr[6] = (r1 + i3) >> 3;
		    dstr[7] = (r4 + rr1) >> 3;
	    }

		break;

	case 4: {
		    mlib_s32 divider = 2;
		    DEF_ADDER(divider);
		    __m128i reim0, reim1, reim2, reim3, reim4, reim5, reim6,
			reim7, tmp;

		    DCL_COMMON;
		    DCL_OUTVARR;

		    PREP_OUTR;

		    LOAD_REIM;

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
__mlib_SignalFFT_1_S16C_S16C_Mod(
    mlib_s16 *dstc,
    const mlib_s16 *srcc,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcc);
	SERVE_BAD_ORDER(order);

	if (order <= 4) {
		return (mlib_s_SignalFFT_S16C_S16C_0_4(dstc, srcc, order));
	} else {
		return mlib_s_fftDispatcher_1(dstc, NULL, (void *)srcc, NULL,
		    order, eSrcCmplx | eDstCmplx);
	}
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_1_S16_S16_Mod(
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
	SERVE_BAD_ORDER(order);

	if (order <= 4)
		return mlib_s_SignalFFT_S16_S16_0_4(dstr, dsti, srcr, srci,
		    order);
	else
		return mlib_s_fftDispatcher_1(dstr, dsti, (void *)srcr,
		    (void *)srci, order, eSrcReIm | eDstReIm);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_1_S16C_S16_Mod(
    mlib_s16 *dstc,
    const mlib_s16 *srcr,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcr);
	SERVE_BAD_ORDER(order);

	if (order <= 4)
		return (mlib_s_SignalFFT_S16C_S16_0_4(dstc, srcr, order));
	else
		return mlib_s_fftDispatcher_1(dstc, NULL, (void *)srcr, NULL,
		    order, eSrcRe | eDstCmplx);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_1_S16C_S16C(
    mlib_s16 *dstc,
    const mlib_s16 *srcc,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcc);
	SERVE_BAD_ORDER(order);

	if (order <= 4)
		return (mlib_s_SignalIFFT_S16C_S16C_0_4(dstc, srcc, order));
	else
		return mlib_s_fftDispatcher_1(dstc, NULL, (void *)srcc, NULL,
		    order, eSrcCmplx | eDstCmplx | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_1_S16_S16(
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
	SERVE_BAD_ORDER(order);

	if (order <= 4)
		return mlib_s_SignalIFFT_S16_S16_0_4(dstr, dsti, srcr, srci,
		    order);
	else
		return mlib_s_fftDispatcher_1(dstr, dsti, (void *)srcr,
		    (void *)srci, order, eSrcReIm | eDstReIm | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_1_S16_S16C(
    mlib_s16 *dstr,
    const mlib_s16 *srcc,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(srcc);
	SERVE_BAD_POINTER(dstr);
	SERVE_BAD_ORDER(order);

	if (order <= 4)
		return (mlib_s_SignalIFFT_S16_S16C_0_4(dstr, srcc, order));
	else
		return mlib_s_fftDispatcher_1(dstr, NULL, (void *)srcc, NULL,
		    order, eSrcCmplx | eDstRe | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_1_S16C_Mod(
    mlib_s16 *datac,
    mlib_s32 order)
{
	return (__mlib_SignalFFT_1_S16C_S16C_Mod(datac, datac, order));
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_1_S16_Mod(
    mlib_s16 *datar,
    mlib_s16 *datai,
    mlib_s32 order)
{
	return __mlib_SignalFFT_1_S16_S16_Mod(datar, datai, datar, datai,
	    order);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_1_S16C(
    mlib_s16 *datac,
    mlib_s32 order)
{
	return (__mlib_SignalIFFT_1_S16C_S16C(datac, datac, order));
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_1_S16(
    mlib_s16 *datar,
    mlib_s16 *datai,
    mlib_s32 order)
{
	return (__mlib_SignalIFFT_1_S16_S16(datar, datai, datar, datai, order));
}

/* *********************************************************** */
