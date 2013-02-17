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

#pragma ident	"@(#)mlib_v_SignalFFT_1.c	9.4	07/11/05 SMI"

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
 *  parameter set to NULL ( see also: mlib_v_fftDispatcher_1 macro in header )
 * Note2. Functions like mlib*FFT_1_S16 and mlib*FFT_1_S16C are trivially
 *  mapped into mlib*FFT_1_S16_S16 and mlib*FFT_1_S16C_S16C.
 * Note3. All functions ( regarding Note2 mapping ) have "_0_4-shadows":
 *  funcname -> funcname##_0_4 providing processing for orders 0 - 4
 */

#include <mlib_signal.h>
#include <mlib_SignalFFTUtil.h>
#include <vis_proto.h>

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

/* macros, typedefs */

#define	VIS_ALIGNADDR(PTR, OFF)	vis_alignaddr((void *)(PTR), OFF)

/* *********************************************************** */

/* 4th order macros */

/* *********************************************************** */

#define	TRANSPOSE4X4_REV(i0, i1, i2, i3, p0, p2, p1, p3)               \
	{                                                              \
	    mlib_d64 i02, i13, i0213, k02, k13;                        \
	                                                               \
	    i02 = vis_fpmerge(vis_read_hi(i0), vis_read_hi(i2));       \
	    i13 = vis_fpmerge(vis_read_hi(i1), vis_read_hi(i3));       \
	    i0213 = vis_fpmerge(vis_read_hi(i02), vis_read_hi(i13));   \
	    k02 = vis_fpmerge(vis_read_lo(i0), vis_read_lo(i2));       \
	    k13 = vis_fpmerge(vis_read_lo(i1), vis_read_lo(i3));       \
	    *(mlib_d64 *)(p0) =                                        \
		vis_fpmerge(vis_read_hi(i0213), vis_read_lo(i0213));   \
	    i0213 = vis_fpmerge(vis_read_lo(i02), vis_read_lo(i13));   \
	    *(mlib_d64 *)(p1) =                                        \
		vis_fpmerge(vis_read_hi(i0213), vis_read_lo(i0213));   \
	    i0213 = vis_fpmerge(vis_read_hi(k02), vis_read_hi(k13));   \
	    *(mlib_d64 *)(p2) =                                        \
		vis_fpmerge(vis_read_hi(i0213), vis_read_lo(i0213));   \
	    i0213 = vis_fpmerge(vis_read_lo(k02), vis_read_lo(k13));   \
	    *(mlib_d64 *)(p3) =                                        \
		vis_fpmerge(vis_read_hi(i0213), vis_read_lo(i0213));   \
	}

/* *********************************************************** */

#define	UNMIX_REIM(ri0, ri1, rout, iout)                              \
	{                                                             \
	    mlib_d64 i00, i44, i04, i26;                              \
	                                                              \
	    i00 = vis_fpmerge(vis_read_hi(ri0), vis_read_hi(ri1));    \
	    i44 = vis_fpmerge(vis_read_lo(ri0), vis_read_lo(ri1));    \
	    i04 = vis_fpmerge(vis_read_hi(i00), vis_read_hi(i44));    \
	    i26 = vis_fpmerge(vis_read_lo(i00), vis_read_lo(i44));    \
	    rout = vis_fpmerge(vis_read_hi(i04), vis_read_lo(i04));   \
	    iout = vis_fpmerge(vis_read_hi(i26), vis_read_lo(i26));   \
	}

/* *********************************************************** */

#define	REMIX_REIM(r0, i0, ri0, ri1)                                 \
	{                                                            \
	    mlib_d64 r04, i04, r02, i02;                             \
	                                                             \
	    r04 = vis_fpmerge(vis_read_hi(r0), vis_read_lo(r0));     \
	    i04 = vis_fpmerge(vis_read_hi(i0), vis_read_lo(i0));     \
	    r02 = vis_fpmerge(vis_read_hi(r04), vis_read_lo(r04));   \
	    i02 = vis_fpmerge(vis_read_hi(i04), vis_read_lo(i04));   \
	    r04 = vis_fpmerge(vis_read_hi(r02), vis_read_hi(i02));   \
	    i04 = vis_fpmerge(vis_read_lo(r02), vis_read_lo(i02));   \
	    ri0 = vis_fpmerge(vis_read_hi(r04), vis_read_hi(i04));   \
	    ri1 = vis_fpmerge(vis_read_lo(r04), vis_read_lo(i04));   \
	}

/* *********************************************************** */

#define	LOAD_REAL                                               \
	{                                                       \
	    mlib_d64 tmp, *real64;                              \
	                                                        \
	    real64 = VIS_ALIGNADDR(srcr, 0);                    \
	    real0 = real64[0];                                  \
	    real1 = real64[1];                                  \
	    real2 = real64[2];                                  \
	    real3 = real64[3];                                  \
	    tmp = real64[4];                                    \
	    real0 = vis_faligndata(real0, real1);               \
	    real1 = vis_faligndata(real1, real2);               \
	    real2 = vis_faligndata(real2, real3);               \
	    real3 = vis_faligndata(real3, tmp);                 \
	}

/* *********************************************************** */

#define	LOAD_IMAG                                               \
	{                                                       \
	    mlib_d64 tmp, *imag64;                              \
	                                                        \
	    imag64 = VIS_ALIGNADDR(srci, 0);                    \
	    imag0 = imag64[0];                                  \
	    imag1 = imag64[1];                                  \
	    imag2 = imag64[2];                                  \
	    imag3 = imag64[3];                                  \
	    tmp = imag64[4];                                    \
	    imag0 = vis_faligndata(imag0, imag1);               \
	    imag1 = vis_faligndata(imag1, imag2);               \
	    imag2 = vis_faligndata(imag2, imag3);               \
	    imag3 = vis_faligndata(imag3, tmp);                 \
	}

/* *********************************************************** */

#define	LOAD_REIM                                                      \
	{                                                              \
	    mlib_d64 *reim64 = VIS_ALIGNADDR(srcc, 0);                 \
	                                                               \
	    reim0 = reim64[0];                                         \
	    reim1 = reim64[1];                                         \
	    reim2 = reim64[2];                                         \
	    reim3 = reim64[3];                                         \
	    reim4 = reim64[4];                                         \
	    reim5 = reim64[5];                                         \
	    reim6 = reim64[6];                                         \
	    reim7 = reim64[7];                                         \
	    tmp = reim64[8];                                           \
	    reim0 = vis_faligndata(reim0, reim1);                      \
	    reim1 = vis_faligndata(reim1, reim2);                      \
	    reim2 = vis_faligndata(reim2, reim3);                      \
	    reim3 = vis_faligndata(reim3, reim4);                      \
	    reim4 = vis_faligndata(reim4, reim5);                      \
	    reim5 = vis_faligndata(reim5, reim6);                      \
	    reim6 = vis_faligndata(reim6, reim7);                      \
	    reim7 = vis_faligndata(reim7, tmp);                        \
	UNMIX_REIM(reim0, reim1, real0, imag0) UNMIX_REIM(reim2,       \
		reim3, real1, imag1) UNMIX_REIM(reim4, reim5, real2,   \
		imag2) UNMIX_REIM(reim6, reim7, real3, imag3)}

/* *********************************************************** */

#define	MULT(d0, d1)                                                  \
	vis_fpadd16(vis_fmul8sux16(d0, d1), vis_fmul8ulx16(d0, d1))

/* *********************************************************** */

#define	PROC_ONLYREAL_FIRST_DIR                                 \
	{                                                       \
	    mlib_d64 r0a1, r0s1, r2a3, r2s3;                    \
	                                                        \
	    r0a1 = vis_fpadd16(real0, real2);                   \
	    r2a3 = vis_fpadd16(real1, real3);                   \
	    r0s1 = vis_fpsub16(real0, real2);                   \
	    r2s3 = vis_fpsub16(real1, real3);                   \
	    ar = vis_fpadd16(r0a1, r2a3);                       \
	    cr = vis_fpsub16(r0a1, r2a3);                       \
	    br = dr = r0s1;                                     \
	    ai = ci = vis_fzero();                              \
	    bi = vis_fpsub16(ai, r2s3);                         \
	    di = r2s3;                                          \
	}

/* *********************************************************** */

#define	PROC_FIRST_DIR                                          \
	{                                                       \
	    mlib_d64 r0a1, r0s1, r2a3, r2s3;                    \
	    mlib_d64 i0a1, i0s1, i2a3, i2s3;                    \
	                                                        \
	    r0a1 = vis_fpadd16(real0, real2);                   \
	    r2a3 = vis_fpadd16(real1, real3);                   \
	    r0s1 = vis_fpsub16(real0, real2);                   \
	    r2s3 = vis_fpsub16(real1, real3);                   \
	    ar = vis_fpadd16(r0a1, r2a3);                       \
	    cr = vis_fpsub16(r0a1, r2a3);                       \
	    i0a1 = vis_fpadd16(imag0, imag2);                   \
	    i2a3 = vis_fpadd16(imag1, imag3);                   \
	    i0s1 = vis_fpsub16(imag0, imag2);                   \
	    i2s3 = vis_fpsub16(imag1, imag3);                   \
	    ai = vis_fpadd16(i0a1, i2a3);                       \
	    ci = vis_fpsub16(i0a1, i2a3);                       \
	    bi = vis_fpsub16(i0s1, r2s3);                       \
	    di = vis_fpadd16(i0s1, r2s3);                       \
	    br = vis_fpadd16(r0s1, i2s3);                       \
	    dr = vis_fpsub16(r0s1, i2s3);                       \
	}

/* *********************************************************** */

#define	PROC_FIRST_INV                                          \
	{                                                       \
	    mlib_d64 r0a1, r0s1, r2a3, r2s3;                    \
	    mlib_d64 i0a1, i0s1, i2a3, i2s3;                    \
	                                                        \
	    real0 = vis_fmul8x16(divider, real0);               \
	    real1 = vis_fmul8x16(divider, real1);               \
	    real2 = vis_fmul8x16(divider, real2);               \
	    real3 = vis_fmul8x16(divider, real3);               \
	    r0a1 = vis_fpadd16(real0, real2);                   \
	    r2a3 = vis_fpadd16(real1, real3);                   \
	    r0s1 = vis_fpsub16(real0, real2);                   \
	    r2s3 = vis_fpsub16(real1, real3);                   \
	    ar = vis_fpadd16(r0a1, r2a3);                       \
	    cr = vis_fpsub16(r0a1, r2a3);                       \
	    imag0 = vis_fmul8x16(divider, imag0);               \
	    imag1 = vis_fmul8x16(divider, imag1);               \
	    imag2 = vis_fmul8x16(divider, imag2);               \
	    imag3 = vis_fmul8x16(divider, imag3);               \
	    i0a1 = vis_fpadd16(imag0, imag2);                   \
	    i2a3 = vis_fpadd16(imag1, imag3);                   \
	    i0s1 = vis_fpsub16(imag0, imag2);                   \
	    i2s3 = vis_fpsub16(imag1, imag3);                   \
	    ai = vis_fpadd16(i0a1, i2a3);                       \
	    ci = vis_fpsub16(i0a1, i2a3);                       \
	    bi = vis_fpadd16(i0s1, r2s3);                       \
	    di = vis_fpsub16(i0s1, r2s3);                       \
	    br = vis_fpsub16(r0s1, i2s3);                       \
	    dr = vis_fpadd16(r0s1, i2s3);                       \
	}

/* *********************************************************** */

#pragma align 8(mlib_fft_rot)

static const mlib_u16 mlib_fft_rot[] = {

	0x4040, 0x4040, 0x0000, 0x0000,
	0x4000, 0x2D41, 0x0000, 0xD2BF,
	0x0000, 0x2D41, 0x4000, 0x2D41,
	0x4000, 0x3B21, 0x2D41, 0x187E,
	0x0000, 0xE782, 0xD2BF, 0xC4DF,
	0x4000, 0x187E, 0xD2BF, 0xC4DF,
	0x0000, 0xC4DF, 0xD2BF, 0x187E,
	0x7FFF, 0x5A82, 0x0000, 0xA57E,
	0x0000, 0x5A82, 0x7FFF, 0x5A82,
	0x7FFF, 0x7642, 0x5A82, 0x30FC,
	0x0000, 0xCF04, 0xA57E, 0x89BE,
	0x7FFF, 0x30FC, 0xA57E, 0x89BE,
	0x0000, 0x89BE, 0xA57E, 0x30FC,
};

/* *********************************************************** */

#define	LOAD_ROT_DIR                                            \
	rot3r = *((mlib_d64 *)mlib_fft_rot + 7);                \
	rot3i = *((mlib_d64 *)mlib_fft_rot + 8);                \
	rotc0r = *((mlib_d64 *)mlib_fft_rot + 9);               \
	rotc1r = *((mlib_d64 *)mlib_fft_rot + 10);              \
	rotd0r = *((mlib_d64 *)mlib_fft_rot + 11);              \
	rotd1r = *((mlib_d64 *)mlib_fft_rot + 12)

/* *********************************************************** */

#define	LOAD_ROT_INV                                            \
	rot3r = *((mlib_d64 *)mlib_fft_rot + 1);                \
	rot3i = *((mlib_d64 *)mlib_fft_rot + 2);                \
	rotc0r = *((mlib_d64 *)mlib_fft_rot + 3);               \
	rotc1r = *((mlib_d64 *)mlib_fft_rot + 4);               \
	rotd0r = *((mlib_d64 *)mlib_fft_rot + 5);               \
	rotd1r = *((mlib_d64 *)mlib_fft_rot + 6)

/* *********************************************************** */

#define	PROC_LAST_INV                                           \
	{                                                       \
	    mlib_d64 c0r, c0i, d0r, d0i;                        \
	                                                        \
	    ar = vis_fmul8x16(divider, ar);                     \
	    ai = vis_fmul8x16(divider, ai);                     \
	    real1 = MULT(rot3r, br);                            \
	    tmp = MULT(rot3i, bi);                              \
	    real1 = vis_fpsub16(real1, tmp);                    \
	    imag1 = MULT(rot3r, bi);                            \
	    tmp = MULT(rot3i, br);                              \
	    imag1 = vis_fpadd16(imag1, tmp);                    \
	    real0 = vis_fpadd16(ar, real1);                     \
	    real1 = vis_fpsub16(ar, real1);                     \
	    imag0 = vis_fpadd16(ai, imag1);                     \
	    imag1 = vis_fpsub16(ai, imag1);                     \
	    c0r = MULT(rotc0r, cr);                             \
	    tmp = MULT(rotc1r, ci);                             \
	    c0r = vis_fpadd16(c0r, tmp);                        \
	    c0i = MULT(rotc1r, cr);                             \
	    tmp = MULT(rotc0r, ci);                             \
	    c0i = vis_fpsub16(tmp, c0i);                        \
	    d0r = MULT(rotd0r, dr);                             \
	    tmp = MULT(rotd1r, di);                             \
	    d0r = vis_fpadd16(d0r, tmp);                        \
	    d0i = MULT(rotd1r, dr);                             \
	    tmp = MULT(rotd0r, di);                             \
	    d0i = vis_fpsub16(tmp, d0i);                        \
	    real2 = vis_fpadd16(c0r, d0r);                      \
	    imag2 = vis_fpadd16(c0i, d0i);                      \
	    real3 = vis_fpsub16(d0i, c0i);                      \
	    imag3 = vis_fpsub16(c0r, d0r);                      \
	}

/* *********************************************************** */

#define	PROC_LAST_END                                           \
	ar = vis_fpadd16(real0, real2);                         \
	br = vis_fpadd16(real1, real3);                         \
	cr = vis_fpsub16(real0, real2);                         \
	dr = vis_fpsub16(real1, real3);                         \
	ai = vis_fpadd16(imag0, imag2);                         \
	bi = vis_fpadd16(imag1, imag3);                         \
	ci = vis_fpsub16(imag0, imag2);                         \
	di = vis_fpsub16(imag1, imag3)

/* *********************************************************** */

#define	PROC_LAST_END_R                                         \
	ar = vis_fpadd16(real0, real2);                         \
	br = vis_fpadd16(real1, real3);                         \
	cr = vis_fpsub16(real0, real2);                         \
	dr = vis_fpsub16(real1, real3)

/* *********************************************************** */

#define	PROC_LAST_DIR                                           \
	{                                                       \
	    mlib_d64 c0r, c0i, d0r, d0i, tmp;                   \
	                                                        \
	    br = vis_fpadd16(br, br);                           \
	    bi = vis_fpadd16(bi, bi);                           \
	    cr = vis_fpadd16(cr, cr);                           \
	    ci = vis_fpadd16(ci, ci);                           \
	    dr = vis_fpadd16(dr, dr);                           \
	    di = vis_fpadd16(di, di);                           \
	    real1 = MULT(rot3r, br);                            \
	    tmp = MULT(rot3i, bi);                              \
	    real1 = vis_fpadd16(real1, tmp);                    \
	    imag1 = MULT(rot3r, bi);                            \
	    tmp = MULT(rot3i, br);                              \
	    imag1 = vis_fpsub16(imag1, tmp);                    \
	    real0 = vis_fpadd16(ar, real1);                     \
	    real1 = vis_fpsub16(ar, real1);                     \
	    imag0 = vis_fpadd16(ai, imag1);                     \
	    imag1 = vis_fpsub16(ai, imag1);                     \
	    c0r = MULT(rotc0r, cr);                             \
	    tmp = MULT(rotc1r, ci);                             \
	    c0r = vis_fpsub16(c0r, tmp);                        \
	    c0i = MULT(rotc1r, cr);                             \
	    tmp = MULT(rotc0r, ci);                             \
	    c0i = vis_fpadd16(tmp, c0i);                        \
	    d0r = MULT(rotd0r, dr);                             \
	    tmp = MULT(rotd1r, di);                             \
	    d0r = vis_fpsub16(d0r, tmp);                        \
	    d0i = MULT(rotd1r, dr);                             \
	    tmp = MULT(rotd0r, di);                             \
	    d0i = vis_fpadd16(tmp, d0i);                        \
	    real2 = vis_fpadd16(c0r, d0r);                      \
	    imag2 = vis_fpadd16(c0i, d0i);                      \
	    real3 = vis_fpsub16(c0i, d0i);                      \
	    imag3 = vis_fpsub16(d0r, c0r);                      \
	}

/* *********************************************************** */

#define	STORE_REIM                                                       \
	VIS_ALIGNADDR(pout64, (8 - (mlib_addr)dstc) & 7);                \
	REMIX_REIM(ar, ai, reim0, reim1) REMIX_REIM(br, bi, reim2,       \
	    reim3) REMIX_REIM(cr, ci, reim4, reim5) REMIX_REIM(dr, di,   \
	    reim6, reim7) pout64[1] = vis_faligndata(reim0, reim1);      \
	pout64[2] = vis_faligndata(reim1, reim2);                        \
	pout64[3] = vis_faligndata(reim2, reim3);                        \
	pout64[4] = vis_faligndata(reim3, reim4);                        \
	pout64[5] = vis_faligndata(reim4, reim5);                        \
	pout64[6] = vis_faligndata(reim5, reim6);                        \
	pout64[7] = vis_faligndata(reim6, reim7);                        \
	reim7 = vis_faligndata(reim7, reim0);                            \
	vis_pst_16(reim7, pout64, mask1);                                \
	vis_pst_16(reim7, pout64 + 8, mask2)

/* *********************************************************** */

#define	STORE_RE                                                \
	VIS_ALIGNADDR(pout64, (8 - (mlib_addr)dstr) & 7);       \
	pout64[1] = vis_faligndata(ar, br);                     \
	pout64[2] = vis_faligndata(br, cr);                     \
	pout64[3] = vis_faligndata(cr, dr);                     \
	dr = vis_faligndata(dr, ar);                            \
	vis_pst_16(dr, pout64, mask1);                          \
	vis_pst_16(dr, pout64 + 4, mask2)

/* *********************************************************** */

#define	STORE_IM                                                \
	VIS_ALIGNADDR(pouti64, (8 - (mlib_addr)dsti) & 7);      \
	pouti64[1] = vis_faligndata(ai, bi);                    \
	pouti64[2] = vis_faligndata(bi, ci);                    \
	pouti64[3] = vis_faligndata(ci, di);                    \
	di = vis_faligndata(di, ai);                            \
	vis_pst_16(di, pouti64, maski1);                        \
	vis_pst_16(di, pouti64 + 4, maski2)

/* *********************************************************** */

#define	PREP_OUTC                                               \
	pout64 = (mlib_d64 *)((mlib_addr)dstc & ~7);            \
	mask1 = vis_edge16(dstc, dstc + 8);                     \
	mask2 = ~mask1;                                         \
	if (!((mlib_addr)dstc & 7)) {                           \
	    --pout64;                                           \
	    mask1 = 0;                                          \
	    mask2 = 0xf;                                        \
	}

/* *********************************************************** */

#define	PREP_OUTR                                               \
	pout64 = (mlib_d64 *)((mlib_addr)dstr & ~7);            \
	mask1 = vis_edge16(dstr, dstr + 8);                     \
	mask2 = ~mask1;                                         \
	if (!((mlib_addr)dstr & 7)) {                           \
	    --pout64;                                           \
	    mask1 = 0;                                          \
	    mask2 = 0xf;                                        \
	}

/* *********************************************************** */

#define	PREP_OUTI                                               \
	pouti64 = (mlib_d64 *)((mlib_addr)dsti & ~7);           \
	maski1 = vis_edge16(dsti, dsti + 8);                    \
	maski2 = ~maski1;                                       \
	if (!((mlib_addr)dsti & 7)) {                           \
	    --pouti64;                                          \
	    maski1 = 0;                                         \
	    maski2 = 0xf;                                       \
	}

/* *********************************************************** */

#define	DCL_COMMON                                              \
	mlib_d64 ar, br, cr, dr, ai, bi, ci, di;                \
	mlib_d64 real0, real1, real2, real3;                    \
	mlib_d64 imag0, imag1, imag2, imag3;                    \
	mlib_d64 rot3r, rot3i, rotc0r, rotc1r, rotd0r, rotd1r

/* *********************************************************** */

#define	DCL_OUTVARR	mlib_d64* pout64; mlib_s32 mask1, mask2

/* *********************************************************** */

#define	DCL_OUTVARI	mlib_d64* pouti64; mlib_s32 maski1, maski2

/* *********************************************************** */

#define	DCL_2                                                   \
	mlib_s32 r0, r1, r2, r3;                                \
	mlib_s32 i0, i1, i2, i3;                                \
	mlib_s32 rr0, rr1, rr2, rr3;                            \
	mlib_s32 ii0, ii1, ii2, ii3

/* *********************************************************** */

#define	DCL_3                                                   \
	mlib_s32 r0, r1, r2, r3, r4, r5, r6, r7;                \
	mlib_s32 i0, i1, i2, i3, i4, i5, i6, i7;                \
	mlib_s32 rr0, rr1, rr2, rr3, rr4, rr5, rr6, rr7;        \
	mlib_s32 ii0, ii1, ii2, ii3, ii4, ii5, ii6, ii7;        \
	mlib_d64 coeff = 0.707106781

/* *********************************************************** */

#define	LOAD_2C                                                 \
	r0 = srcc[0];                                           \
	i0 = srcc[1];                                           \
	r1 = srcc[2];                                           \
	i1 = srcc[3];                                           \
	r2 = srcc[4];                                           \
	i2 = srcc[5];                                           \
	r3 = srcc[6];                                           \
	i3 = srcc[7]

/* *********************************************************** */

#define	LOAD_2                                                  \
	r0 = srcr[0];                                           \
	r1 = srcr[1];                                           \
	r2 = srcr[2];                                           \
	r3 = srcr[3];                                           \
	i0 = srci[0];                                           \
	i1 = srci[1];                                           \
	i2 = srci[2];                                           \
	i3 = srci[3]

/* *********************************************************** */

#define	LOAD_3C                                                 \
	r0 = srcc[0];                                           \
	i0 = srcc[1];                                           \
	r1 = srcc[2];                                           \
	i1 = srcc[3];                                           \
	r2 = srcc[4];                                           \
	i2 = srcc[5];                                           \
	r3 = srcc[6];                                           \
	i3 = srcc[7];                                           \
	r4 = srcc[8];                                           \
	i4 = srcc[9];                                           \
	r5 = srcc[10];                                          \
	i5 = srcc[11];                                          \
	r6 = srcc[12];                                          \
	i6 = srcc[13];                                          \
	r7 = srcc[14];                                          \
	i7 = srcc[15]

/* *********************************************************** */

#define	LOAD_3                                                  \
	r0 = srcr[0];                                           \
	r1 = srcr[1];                                           \
	r2 = srcr[2];                                           \
	r3 = srcr[3];                                           \
	r4 = srcr[4];                                           \
	r5 = srcr[5];                                           \
	r6 = srcr[6];                                           \
	r7 = srcr[7];                                           \
	i0 = srci[0];                                           \
	i1 = srci[1];                                           \
	i2 = srci[2];                                           \
	i3 = srci[3];                                           \
	i4 = srci[4];                                           \
	i5 = srci[5];                                           \
	i6 = srci[6];                                           \
	i7 = srci[7]

/* *********************************************************** */

#define	FFT2_KERNEL                                             \
	rr0 = r0 + r2;                                          \
	rr1 = r0 - r2;                                          \
	rr2 = r1 + r3;                                          \
	rr3 = r1 - r3;                                          \
	ii0 = i0 + i2;                                          \
	ii1 = i0 - i2;                                          \
	ii2 = i1 + i3;                                          \
	ii3 = i1 - i3

/* *********************************************************** */

#define	FFT3_KERNEL                                             \
	rr0 = r0 + r4;                                          \
	rr1 = r0 - r4;                                          \
	rr2 = r1 + r5;                                          \
	rr3 = r1 - r5;                                          \
	rr4 = r2 + r6;                                          \
	rr5 = r2 - r6;                                          \
	rr6 = r3 + r7;                                          \
	rr7 = r3 - r7;                                          \
	ii0 = i0 + i4;                                          \
	ii1 = i0 - i4;                                          \
	ii2 = i1 + i5;                                          \
	ii3 = i1 - i5;                                          \
	ii4 = i2 + i6;                                          \
	ii5 = i2 - i6;                                          \
	ii6 = i3 + i7;                                          \
	ii7 = i3 - i7;                                          \
	r0 = rr0 + rr4;                                         \
	r1 = rr0 - rr4;                                         \
	r2 = rr2 + rr6;                                         \
	r3 = rr2 - rr6;                                         \
	r4 = rr1 + ii5;                                         \
	r5 = rr1 - ii5;                                         \
	r6 = rr3 + ii7;                                         \
	r7 = rr3 - ii7;                                         \
	i0 = ii0 + ii4;                                         \
	i1 = ii0 - ii4;                                         \
	i2 = ii2 + ii6;                                         \
	i3 = ii2 - ii6;                                         \
	i4 = ii1 + rr5;                                         \
	i5 = ii1 - rr5;                                         \
	i6 = ii3 + rr7;                                         \
	i7 = ii3 - rr7;                                         \
	rr0 = (r7 - i6) * coeff;                                \
	rr2 = (r7 + i6) * coeff;                                \
	rr1 = (r6 + i7) * coeff;                                \
	rr3 = (r6 - i7) * coeff

/* *********************************************************** */

/* general error processing */

/* *********************************************************** */

#define	SERVE_BAD_ORDER(order)                                  \
	if ((order) < 0 || (order) > 31)                        \
	    return MLIB_OUTOFRANGE

#define	SERVE_BAD_POINTER(ptr)	if (!(ptr)) return MLIB_NULLPOINTER

/* *********************************************************** */

/* functions codes */

static mlib_status
mlib_v_SignalFFT_S16C_S16C_0_4(
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
		    mlib_d64 reim0, reim1, reim2, reim3, reim4, reim5, reim6,
			reim7, tmp;

		    DCL_COMMON;
		    DCL_OUTVARR;

		    PREP_OUTC;

		    LOAD_REIM;

		    PROC_FIRST_DIR;

		    TRANSPOSE4X4_REV(ar, br, cr, dr, &ar, &br, &cr, &dr);
		    TRANSPOSE4X4_REV(ai, bi, ci, di, &ai, &bi, &ci, &di);

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
mlib_v_SignalFFT_S16_S16_0_4(
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

		    TRANSPOSE4X4_REV(ar, br, cr, dr, &ar, &br, &cr, &dr);
		    TRANSPOSE4X4_REV(ai, bi, ci, di, &ai, &bi, &ci, &di);

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
mlib_v_SignalFFT_S16C_S16_0_4(
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
		    mlib_d64 reim0, reim1, reim2, reim3, reim4, reim5, reim6,
			reim7;

		    DCL_COMMON;
		    DCL_OUTVARR;

		    PREP_OUTC;

		    LOAD_REAL;

		    PROC_ONLYREAL_FIRST_DIR;

		    TRANSPOSE4X4_REV(ar, br, cr, dr, &ar, &br, &cr, &dr);
		    TRANSPOSE4X4_REV(ai, bi, ci, di, &ai, &bi, &ci, &di);

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
mlib_v_SignalIFFT_S16C_S16C_0_4(
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
		    mlib_f32 divider = *((mlib_f32 *)mlib_fft_rot);
		    mlib_d64 reim0, reim1, reim2, reim3, reim4, reim5, reim6,
			reim7, tmp;

		    DCL_COMMON;
		    DCL_OUTVARR;

		    PREP_OUTC;

		    LOAD_REIM;

		    PROC_FIRST_INV;

		    TRANSPOSE4X4_REV(ar, br, cr, dr, &ar, &br, &cr, &dr);
		    TRANSPOSE4X4_REV(ai, bi, ci, di, &ai, &bi, &ci, &di);

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
mlib_v_SignalIFFT_S16_S16_0_4(
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
		    mlib_f32 divider = *((mlib_f32 *)mlib_fft_rot);
		    mlib_d64 tmp;

		    DCL_COMMON;
		    DCL_OUTVARR;
		    DCL_OUTVARI;

		    PREP_OUTR;
		    PREP_OUTI;

		    LOAD_REAL;
		    LOAD_IMAG;

		    PROC_FIRST_INV;

		    TRANSPOSE4X4_REV(ar, br, cr, dr, &ar, &br, &cr, &dr);
		    TRANSPOSE4X4_REV(ai, bi, ci, di, &ai, &bi, &ci, &di);

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
mlib_v_SignalIFFT_S16_S16C_0_4(
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
		    mlib_f32 divider = *((mlib_f32 *)mlib_fft_rot);
		    mlib_d64 reim0, reim1, reim2, reim3, reim4, reim5, reim6,
			reim7, tmp;

		    DCL_COMMON;
		    DCL_OUTVARR;

		    PREP_OUTR;

		    LOAD_REIM;

		    PROC_FIRST_INV;

		    TRANSPOSE4X4_REV(ar, br, cr, dr, &ar, &br, &cr, &dr);
		    TRANSPOSE4X4_REV(ai, bi, ci, di, &ai, &bi, &ci, &di);

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

	if (order <= 4)
		return (mlib_v_SignalFFT_S16C_S16C_0_4(dstc, srcc, order));
	else
		return mlib_v_fftDispatcher_1(dstc, NULL, (void *)srcc, NULL,
		    order, eSrcCmplx | eDstCmplx);
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
		return mlib_v_SignalFFT_S16_S16_0_4(dstr, dsti, srcr, srci,
		    order);
	else
		return mlib_v_fftDispatcher_1(dstr, dsti, (void *)srcr,
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
		return (mlib_v_SignalFFT_S16C_S16_0_4(dstc, srcr, order));
	else
		return mlib_v_fftDispatcher_1(dstc, NULL, (void *)srcr, NULL,
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
		return (mlib_v_SignalIFFT_S16C_S16C_0_4(dstc, srcc, order));
	else
		return mlib_v_fftDispatcher_1(dstc, NULL, (void *)srcc, NULL,
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
		return mlib_v_SignalIFFT_S16_S16_0_4(dstr, dsti, srcr, srci,
		    order);
	else
		return mlib_v_fftDispatcher_1(dstr, dsti, (void *)srcr,
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
		return (mlib_v_SignalIFFT_S16_S16C_0_4(dstr, srcc, order));
	else
		return mlib_v_fftDispatcher_1(dstr, NULL, (void *)srcc, NULL,
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
