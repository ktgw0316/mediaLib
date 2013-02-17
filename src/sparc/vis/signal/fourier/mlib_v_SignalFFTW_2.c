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

#pragma ident	"@(#)mlib_v_SignalFFTW_2.c	9.4	07/11/05 SMI"

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
#include <mlib_SignalFFTUtil.h>
#include <vis_proto.h>

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

/* *********************************************************** */

#define	VIS_ALIGNADDR(PTR, OFF)	vis_alignaddr((void *)(PTR), OFF)
#define	VIS_LD_U16(PTR)	vis_ld_u16((void *)(PTR))

/* *********************************************************** */

#define	VIS_LDF(dsrc, psrc)                                     \
	dsrc = vis_faligndata(VIS_LD_U16(psrc + 1), dsrc);      \
	dsrc = vis_faligndata(VIS_LD_U16(psrc), dsrc)

/* *********************************************************** */

#define	VIS_LDD(dsrc0, psrc)                                    \
	{                                                       \
	    mlib_d64 *dpsrc, dsrc1;                             \
	                                                        \
	    dpsrc = (mlib_d64 *)VIS_ALIGNADDR(psrc, 0);         \
	    dsrc0 = dpsrc[0];                                   \
	    dsrc1 = dpsrc[1];                                   \
	    dsrc0 = vis_faligndata(dsrc0, dsrc1);               \
	}

/* *********************************************************** */

#define	VIS_LDD_2(dsrc0, dsrc1, psrc)                           \
	{                                                       \
	    mlib_d64 *dpsrc, dsrc2;                             \
	                                                        \
	    dpsrc = (mlib_d64 *)VIS_ALIGNADDR(psrc, 0);         \
	    dsrc0 = dpsrc[0];                                   \
	    dsrc1 = dpsrc[1];                                   \
	    dsrc2 = dpsrc[2];                                   \
	    dsrc0 = vis_faligndata(dsrc0, dsrc1);               \
	    dsrc1 = vis_faligndata(dsrc1, dsrc2);               \
	}

/* *********************************************************** */

#define	VIS_LDD_4(dsrc0, dsrc1, dsrc2, dsrc3, psrc)             \
	{                                                       \
	    mlib_d64 *dpsrc, dsrc4;                             \
	                                                        \
	    dpsrc = (mlib_d64 *)VIS_ALIGNADDR(psrc, 0);         \
	    dsrc0 = dpsrc[0];                                   \
	    dsrc1 = dpsrc[1];                                   \
	    dsrc2 = dpsrc[2];                                   \
	    dsrc3 = dpsrc[3];                                   \
	    dsrc4 = dpsrc[4];                                   \
	    dsrc0 = vis_faligndata(dsrc0, dsrc1);               \
	    dsrc1 = vis_faligndata(dsrc1, dsrc2);               \
	    dsrc2 = vis_faligndata(dsrc2, dsrc3);               \
	    dsrc3 = vis_faligndata(dsrc3, dsrc4);               \
	}

/* *********************************************************** */

#define	VIS_STF(pdst, fdst)                                            \
	{                                                              \
	    mlib_d64 tem_dst;                                          \
	                                                               \
	    tem_dst = vis_write_lo(tem_dst, fdst);                     \
	    vis_st_u16_i(tem_dst, pdst, 2);                            \
	    vis_st_u16_i(vis_faligndata(tem_dst, tem_dst), pdst, 0);   \
	}

/* *********************************************************** */

#define	VIS_STD(pdst, dst)                                      \
	{                                                       \
	    mlib_d64 *dpdst, tem_dst;                           \
	    mlib_s32 emask;                                     \
	                                                        \
	    dpdst = (mlib_d64 *)VIS_ALIGNADDR(pdst, 8);         \
	    emask = vis_edge8(pdst, dpdst);                     \
	    VIS_ALIGNADDR((void *)~(mlib_addr)pdst, 1);         \
	    tem_dst = vis_faligndata(dst, dst);                 \
	    vis_pst_8(tem_dst, dpdst - 1, emask);               \
	    vis_pst_8(tem_dst, dpdst, ~emask);                  \
	}

/* *********************************************************** */

#define	VIS_STD_2(pdst, dst0, dst1)                                     \
	{                                                               \
	    mlib_d64 *dpdst = (mlib_d64 *)VIS_ALIGNADDR(pdst, -1);      \
	    mlib_s32 emask =                                            \
		0xff >> ((mlib_addr)pdst - (mlib_addr)dpdst);           \
	    (void *)VIS_ALIGNADDR((void *)~(mlib_addr)pdst, 1);         \
	    vis_pst_8(vis_faligndata(dst0, dst0), dpdst, emask);        \
	    dpdst[1] = vis_faligndata(dst0, dst1);                      \
	    vis_pst_8(vis_faligndata(dst1, dst1), dpdst + 2, ~emask);   \
	}

/* *********************************************************** */

#define	VIS_STD_4(pdst, dst0, dst1, dst2, dst3)                         \
	{                                                               \
	    mlib_d64 *dpdst = (mlib_d64 *)VIS_ALIGNADDR(pdst, -1);      \
	    mlib_s32 emask =                                            \
		0xff >> ((mlib_addr)pdst - (mlib_addr)dpdst);           \
	    (void *)VIS_ALIGNADDR((void *)~(mlib_addr)pdst, 1);         \
	    vis_pst_8(vis_faligndata(dst0, dst0), dpdst, emask);        \
	    dpdst[1] = vis_faligndata(dst0, dst1);                      \
	    dpdst[2] = vis_faligndata(dst1, dst2);                      \
	    dpdst[3] = vis_faligndata(dst2, dst3);                      \
	    vis_pst_8(vis_faligndata(dst3, dst3), dpdst + 4, ~emask);   \
	}

/* *********************************************************** */

#define	DBL_WIND(dwin, dwin1)                                           \
	{                                                               \
	    mlib_d64 dr;                                                \
	                                                                \
	    dr = vis_fpmerge(vis_read_hi(dwin), vis_read_lo(dwin));     \
	    dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));         \
	    dwin1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr));      \
	    dr = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr));         \
	    dwin = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dwin1));    \
	    dwin1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dwin1));   \
	}

/* *********************************************************** */

#define	VIS_LDF_DBL(dwin, window)                                  \
	{                                                          \
	    mlib_d64 dr = VIS_LD_U16(window + 1);                  \
	                                                           \
	    dwin = vis_faligndata(dr, vis_faligndata(dr, dr));     \
	    dr = VIS_LD_U16(window);                               \
	    dwin = vis_faligndata(dr, vis_faligndata(dr, dwin));   \
	}

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

#define	DCL_WINDOW	mlib_d64 win0, win1, win2, win3

/* *********************************************************** */

#define	LOAD_WINDOW                                             \
	{                                                       \
	    mlib_d64 tmp, *win64;                               \
	                                                        \
	    win64 = VIS_ALIGNADDR(window, 0);                   \
	    win0 = win64[0];                                    \
	    win1 = win64[1];                                    \
	    win2 = win64[2];                                    \
	    win3 = win64[3];                                    \
	    tmp = win64[4];                                     \
	    win0 = vis_faligndata(win0, win1);                  \
	    win1 = vis_faligndata(win1, win2);                  \
	    win2 = vis_faligndata(win2, win3);                  \
	    win3 = vis_faligndata(win3, tmp);                   \
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

#define	DUP_R                                                   \
	real0 = vis_fpadd16(real0, real0);                      \
	real1 = vis_fpadd16(real1, real1);                      \
	real2 = vis_fpadd16(real2, real2);                      \
	real3 = vis_fpadd16(real3, real3)

/* *********************************************************** */

#define	DUP_I                                                   \
	imag0 = vis_fpadd16(imag0, imag0);                      \
	imag1 = vis_fpadd16(imag1, imag1);                      \
	imag2 = vis_fpadd16(imag2, imag2);                      \
	imag3 = vis_fpadd16(imag3, imag3)

/* *********************************************************** */

#define	MULT(d0, d1)                                                  \
	vis_fpadd16(vis_fmul8sux16(d0, d1), vis_fmul8ulx16(d0, d1))

/* *********************************************************** */

#define	MUL_RWIN                                                \
	real0 = MULT(win0, real0);                              \
	real1 = MULT(win1, real1);                              \
	real2 = MULT(win2, real2);                              \
	real3 = MULT(win3, real3)

/* *********************************************************** */

#define	MUL_IWIN                                                \
	imag0 = MULT(win0, imag0);                              \
	imag1 = MULT(win1, imag1);                              \
	imag2 = MULT(win2, imag2);                              \
	imag3 = MULT(win3, imag3)

/* *********************************************************** */

#define	PROC_ONLYREAL_FIRST_DIR                                 \
	{                                                       \
	    mlib_d64 r0a1, r0s1, r2a3, r2s3;                    \
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
	    bi = vis_fpadd16(i0s1, r2s3);                       \
	    di = vis_fpsub16(i0s1, r2s3);                       \
	    br = vis_fpsub16(r0s1, i2s3);                       \
	    dr = vis_fpadd16(r0s1, i2s3);                       \
	}

/* *********************************************************** */

/* rotator values */
#pragma align 8(mlib_fft_rot)

static const mlib_u16 mlib_fft_rot[] = {
	0x4040, 0x4040, 0x8080, 0x8080,
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

#define	LOAD_ROT_INV                                            \
	rot3r = *((mlib_d64 *)mlib_fft_rot + 7);                \
	rot3i = *((mlib_d64 *)mlib_fft_rot + 8);                \
	rotc0r = *((mlib_d64 *)mlib_fft_rot + 9);               \
	rotc1r = *((mlib_d64 *)mlib_fft_rot + 10);              \
	rotd0r = *((mlib_d64 *)mlib_fft_rot + 11);              \
	rotd1r = *((mlib_d64 *)mlib_fft_rot + 12)

/* *********************************************************** */

#define	LOAD_ROT_DIR                                            \
	rot3r = *((mlib_d64 *)mlib_fft_rot + 1);                \
	rot3i = *((mlib_d64 *)mlib_fft_rot + 2);                \
	rotc0r = *((mlib_d64 *)mlib_fft_rot + 3);               \
	rotc1r = *((mlib_d64 *)mlib_fft_rot + 4);               \
	rotd0r = *((mlib_d64 *)mlib_fft_rot + 5);               \
	rotd1r = *((mlib_d64 *)mlib_fft_rot + 6);               \
	divider = ((mlib_f32 *)mlib_fft_rot)[0]

/* *********************************************************** */

#define	PROC_LAST_INV                                           \
	{                                                       \
	    mlib_d64 c0r, c0i, d0r, d0i;                        \
	                                                        \
	    br = vis_fpadd16(br, br);                           \
	    bi = vis_fpadd16(bi, bi);                           \
	    cr = vis_fpadd16(cr, cr);                           \
	    ci = vis_fpadd16(ci, ci);                           \
	    dr = vis_fpadd16(dr, dr);                           \
	    di = vis_fpadd16(di, di);                           \
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
	    ar = vis_fmul8x16(divider, ar);                     \
	    ai = vis_fmul8x16(divider, ai);                     \
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

/* special multiplyings */
#define	MUL32F(f_src0, f_src1)                                  \
	vis_fpadd32(vis_fmuld8sux16(f_src0, f_src1),            \
	    vis_fmuld8ulx16(f_src0, f_src1))

/* *********************************************************** */

#define	MUL32_HL(dst0, dst1, f_sin_pi_on4, d_src)               \
	dst0 = MUL32F(f_sin_pi_on4, vis_read_hi(d_src));        \
	dst1 = MUL32F(f_sin_pi_on4, vis_read_lo(d_src))

/* *********************************************************** */

#define	MUL32_WIN(dst0, dst1, d_win, d_src)                      \
	dst0 = MUL32F(vis_read_hi(d_win), vis_read_hi(d_src));   \
	dst1 = MUL32F(vis_read_lo(d_win), vis_read_lo(d_src))

/* *********************************************************** */

/* flexibility placeholders */
#define	vis_fp00032s(val0, val1)	(val0)
#define	vis_read_00(val)	(val)

/* *********************************************************** */

/* adds or subs highs or lows read */
#define	AS_HL(addsub, hilo0, d0, hilo1, d1)                     \
	vis_fp##addsub##32s(vis_read_##hilo0(d0),               \
	    vis_read_##hilo1(d1))

/* *********************************************************** */

/* parametrized vis_freg_pair with AS_HL's */
#define	FREG(addsub0, hilo0, d0, hilo1, d1, addsub2, hilo2,     \
	d2, hilo3, d3)                                          \
	vis_freg_pair(AS_HL(addsub0, hilo0, d0, hilo1, d1),     \
	    AS_HL(addsub2, hilo2, d2, hilo3, d3))

/* *********************************************************** */

/* FREG with fzero as AS_HL( 000, 00, fzero, 00, 00 ) */
#define	FREG_FZERO(addsub0, hilo0, d0, hilo1, d1)                     \
	FREG(addsub0, hilo0, d0, hilo1, d1, 000, 00, fzero, 00, 00)

/* *********************************************************** */

/* FFT butterflies */
/* simple */
#define	BUTT(dst0, dst1, src0, src1)                            \
	dst0 = vis_fpadd32(src0, src1);                         \
	dst1 = vis_fpsub32(src0, src1)

/* *********************************************************** */

/* on FREG's */
#define	BUTT_FREG(dst0, dst1, hilo0, d0, hilo1, d1, hilo2,            \
	d2, hilo3, d3)                                                \
	dst0 =                                                        \
	FREG(add, hilo0, d0, hilo1, d1, sub, hilo2, d2, hilo3, d3);   \
	dst1 =                                                        \
	FREG(sub, hilo0, d0, hilo1, d1, add, hilo2, d2, hilo3, d3)

/* *********************************************************** */

/* on AS_HL's */
#define	BUTT_AS_HL(dst0, dst1, hilo0, d0, hilo1, d1)            \
	dst0 = AS_HL(add, hilo0, d0, hilo1, d1);                \
	dst1 = AS_HL(sub, hilo0, d0, hilo1, d1)

/* *********************************************************** */

/* FFT butterfly on vis_fpackfix_pair's */
#define	BUTT_FPFX_PR(dst0, dst1, dsrc0, dsrc1, dsrc2, dsrc3)              \
	dst0 =                                                            \
	vis_fpackfix_pair(vis_fpadd32(dsrc0, dsrc1), vis_fpadd32(dsrc2,   \
	    dsrc3));                                                      \
	dst1 =                                                            \
	vis_fpackfix_pair(vis_fpsub32(dsrc0, dsrc1), vis_fpsub32(dsrc2,   \
	    dsrc3))

/* *********************************************************** */

/* FFT butterfly on vis_fpackfix_pair's cross-like */
#define	BUTT_FPFX_PR_CROSS(dst0, dst1, dsrc0, dsrc1, dsrc2,               \
	dsrc3)                                                            \
	dst0 =                                                            \
	vis_fpackfix_pair(vis_fpadd32(dsrc0, dsrc1), vis_fpsub32(dsrc2,   \
	    dsrc3));                                                      \
	dst1 =                                                            \
	vis_fpackfix_pair(vis_fpsub32(dsrc0, dsrc1), vis_fpadd32(dsrc2,   \
	    dsrc3))

/* *********************************************************** */

/* simple FFT butterfly on vis_fpackfix_pair */
#define	BUTT_FPFX_PR_SIMPLE(dsrc0, dsrc1)                                 \
	vis_fpackfix_pair(vis_fpadd32(dsrc0, dsrc1), vis_fpsub32(dsrc0,   \
	    dsrc1))

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
mlib_v_SignalFFTW_S16C_S16C_0_4(
    mlib_s16 *dstc,
    const mlib_s16 *srcc,
    const mlib_s16 *window,
    mlib_s32 order)
{
	mlib_d64 dsrc, dsrc0, dsrc1, dsrc2, dsrc3, dst, dst1;
	mlib_d64 dsrc4, dsrc5, dsrc6, dsrc7, dst2, dst3;
	mlib_d64 dwin, dwin1, dwin2, dwin3;
	mlib_f32 foneby8 = vis_to_float(0x20202020);
	mlib_f32 fonebytwo = vis_to_float(0x80808080);
	mlib_f32 fsinpion4 = vis_to_float(0x5A825A82);

	switch (order) {
	case 0:
		dstc[0] = (srcc[0] * window[0]) >> 15;
		dstc[1] = (srcc[1] * window[0]) >> 15;
		break;

	case 1:
		vis_write_gsr((0 << 3) | 6);
		VIS_LDF_DBL(dwin, window);
		VIS_LDD(dsrc, srcc);
		MUL32_WIN(dsrc0, dsrc1, dwin, dsrc);
		dst = BUTT_FPFX_PR_SIMPLE(dsrc0, dsrc1);
		VIS_STD(dstc, dst);
		break;

	case 2:
		vis_write_gsr((0 << 3) | 6);
		VIS_LDD(dwin, window);
		DBL_WIND(dwin, dwin1);
		VIS_LDD_2(dsrc, dsrc3, srcc);
		dsrc = vis_fmul8x16(fonebytwo, dsrc);
		dsrc3 = vis_fmul8x16(fonebytwo, dsrc3);
		MUL32_WIN(dsrc0, dsrc1, dwin, dsrc);
		MUL32_WIN(dsrc2, dsrc3, dwin1, dsrc3);
		BUTT(dsrc, dsrc0, dsrc0, dsrc2);
		dst1 = vis_fpadd32(dsrc1, dsrc3);
		dsrc1 =
		    FREG(sub, lo, dsrc1, lo, dsrc3, sub, hi, dsrc3, hi, dsrc1);
		BUTT_FPFX_PR(dst, dst1, dsrc, dst1, dsrc0, dsrc1);
		VIS_STD_2(dstc, dst, dst1);
		break;

	case 3:
		vis_write_gsr(1 << 3);
		VIS_LDD_2(dwin, dwin2, window);
		dwin = vis_fmul8x16(foneby8, dwin);
		dwin2 = vis_fmul8x16(foneby8, dwin2);
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
		    vis_fpackfix_pair(FREG(add, hi, dst1, lo, dst1, sub, lo,
		    dst1, hi, dst1), FREG(sub, hi, dst3, lo, dst3, add, hi,
		    dst3, lo, dst3));
		MUL32_HL(dsrc5, dsrc7, fsinpion4, dst1);
		BUTT_FPFX_PR(dst3, dsrc5, dsrc0, dst, dsrc1, dsrc5);
		dsrc4 =
		    vis_fpackfix_pair(FREG(add, hi, dsrc2, lo, dst2, sub, lo,
		    dsrc2, hi, dst2), vis_fpsub32(dsrc3, dsrc7));
		dsrc6 =
		    vis_fpackfix_pair(FREG(sub, hi, dsrc2, lo, dst2, add, lo,
		    dsrc2, hi, dst2), vis_fpadd32(dsrc3, dsrc7));
		VIS_STD_4(dstc, dst3, dsrc4, dsrc5, dsrc6);
		break;

	case 4: {
		    mlib_f32 divider = ((mlib_f32 *)mlib_fft_rot)[1];
		    mlib_d64 reim0, reim1, reim2, reim3, reim4, reim5, reim6,
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
mlib_v_SignalFFTW_S16_S16_0_4(
    mlib_s16 *dstr,
    mlib_s16 *dsti,
    const mlib_s16 *srcr,
    const mlib_s16 *srci,
    const mlib_s16 *window,
    mlib_s32 order)
{
	mlib_d64 dsrc, dsrc0, dsrc1, dsrc2, dsrc3, dst, dst1;
	mlib_d64 dsrc4, dsrc5, dsrc6, dsrc7, dst2, dst3, dwin, dwin1;
	mlib_f32 fdst0, fdst1;
	mlib_f32 foneby8 = vis_to_float(0x20202020);
	mlib_f32 fonebytwo = vis_to_float(0x80808080);
	mlib_f32 fsinpion4 = vis_to_float(0x5A825A82);

	switch (order) {
	case 0:
		dstr[0] = (srcr[0] * window[0]) >> 15;
		dsti[0] = (srci[0] * window[0]) >> 15;
		break;

	case 1:
		vis_write_gsr((0 << 3) | 6);
		VIS_LDF(dwin, window);
		VIS_LDF(dsrc0, srcr);
		VIS_LDF(dsrc1, srci);
		dsrc0 = MUL32F(vis_read_hi(dwin), vis_read_hi(dsrc0));
		dsrc1 = MUL32F(vis_read_hi(dwin), vis_read_hi(dsrc1));
		fdst0 =
		    vis_fpackfix(FREG(add, hi, dsrc0, lo, dsrc0, sub, hi, dsrc0,
		    lo, dsrc0));
		fdst1 =
		    vis_fpackfix(FREG(add, hi, dsrc1, lo, dsrc1, sub, hi, dsrc1,
		    lo, dsrc1));
		VIS_STF(dstr, fdst0);
		VIS_STF(dsti, fdst1);
		break;

	case 2:
		vis_write_gsr((0 << 3) | 6);
		VIS_LDD(dwin, window);
		dwin = vis_fmul8x16(fonebytwo, dwin);
		VIS_LDD(dsrc, srcr);
		VIS_LDD(dsrc3, srci);
		MUL32_WIN(dsrc0, dsrc1, dwin, dsrc);
		MUL32_WIN(dsrc2, dsrc3, dwin, dsrc3);
		BUTT(dsrc, dsrc0, dsrc0, dsrc1);
		BUTT(dst1, dsrc2, dsrc2, dsrc3);
		dst =
		    vis_fpackfix_pair(FREG(add, hi, dsrc, lo, dsrc, add, hi,
		    dsrc0, lo, dsrc2), FREG(sub, hi, dsrc, lo, dsrc, sub, hi,
		    dsrc0, lo, dsrc2));
		dst1 =
		    vis_fpackfix_pair(FREG(add, hi, dst1, lo, dst1, sub, hi,
		    dsrc2, lo, dsrc0), FREG(sub, hi, dst1, lo, dst1, add, hi,
		    dsrc2, lo, dsrc0));
		VIS_STD(dstr, dst);
		VIS_STD(dsti, dst1);
		break;

	case 3:
		vis_write_gsr(1 << 3);
		VIS_LDD_2(dwin, dwin1, window);
		dwin = vis_fmul8x16(foneby8, dwin);
		dwin1 = vis_fmul8x16(foneby8, dwin1);
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
		    vis_fpackfix_pair(FREG(sub, lo, dsrc6, lo, dst2, add, lo,
		    dsrc6, lo, dst2), FREG(add, lo, dsrc7, lo, dst3, sub, lo,
		    dsrc7, lo, dst3));
		MUL32_HL(dsrc0, dsrc1, fsinpion4, dsrc1);
		dsrc2 =
		    vis_fpackfix_pair(FREG(add, hi, dsrc4, lo, dsrc4, add, hi,
		    dsrc7, hi, dsrc1), FREG(add, hi, dsrc5, lo, dst1, sub, hi,
		    dsrc6, hi, dsrc0));
		dsrc3 =
		    vis_fpackfix_pair(FREG(sub, hi, dsrc4, lo, dsrc4, sub, hi,
		    dsrc7, hi, dsrc1), FREG(sub, hi, dsrc5, lo, dst1, add, hi,
		    dsrc6, hi, dsrc0));
		dsrc4 =
		    vis_fpackfix_pair(FREG(add, hi, dst, lo, dst, sub, hi, dst3,
		    lo, dsrc1), FREG(sub, hi, dst1, lo, dsrc5, sub, hi, dst2,
		    lo, dsrc0));
		dsrc5 =
		    vis_fpackfix_pair(FREG(sub, hi, dst, lo, dst, add, hi, dst3,
		    lo, dsrc1), FREG(add, hi, dst1, lo, dsrc5, add, hi, dst2,
		    lo, dsrc0));
		VIS_STD_2(dstr, dsrc2, dsrc3);
		VIS_STD_2(dsti, dsrc4, dsrc5);
		break;

	case 4: {
		    mlib_f32 divider = ((mlib_f32 *)mlib_fft_rot)[1];

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
mlib_v_SignalFFTW_S16C_S16_0_4(
    mlib_s16 *dstc,
    const mlib_s16 *srcr,
    const mlib_s16 *window,
    mlib_s32 order)
{
	mlib_d64 dsrc, dsrc0, dsrc1, dsrc2, dsrc3, dst, dst1;
	mlib_d64 dsrc4, dsrc5, dst2, dst3, dwin, dwin1;
	mlib_f32 fdst0;
	mlib_f32 fzero = vis_fzeros();
	mlib_f32 foneby8 = vis_to_float(0x20202020);
	mlib_f32 fonebytwo = vis_to_float(0x80808080);
	mlib_f32 fsinpion4 = vis_to_float(0x5A825A82);

	switch (order) {
	case 0:
		dstc[0] = (srcr[0] * window[0]) >> 15;
		dstc[1] = 0;
		break;

	case 1:
		vis_write_gsr((0 << 3) | 6);
		VIS_LDF(dwin, window);
		VIS_LDF(dsrc0, srcr);
		dsrc0 = MUL32F(vis_read_hi(dwin), vis_read_hi(dsrc0));
		dst =
		    vis_fpackfix_pair(FREG_FZERO(add, hi, dsrc0, lo, dsrc0),
		    FREG_FZERO(sub, hi, dsrc0, lo, dsrc0));
		VIS_STD(dstc, dst);
		break;

	case 2:
		vis_write_gsr((0 << 3) | 6);
		VIS_LDD(dwin, window);
		dwin = vis_fmul8x16(fonebytwo, dwin);
		VIS_LDD(dsrc, srcr);
		MUL32_WIN(dsrc0, dsrc1, dwin, dsrc);
		BUTT(dsrc, dsrc0, dsrc0, dsrc1);
		dst =
		    vis_fpackfix_pair(FREG_FZERO(add, hi, dsrc, lo, dsrc),
		    FREG(000, hi, dsrc0, 00, 00, sub, 00, fzero, lo, dsrc0));
		dst1 =
		    vis_fpackfix_pair(FREG_FZERO(sub, hi, dsrc, lo, dsrc),
		    dsrc0);
		VIS_STD_2(dstc, dst, dst1);
		break;

	case 3:
		vis_write_gsr(1 << 3);
		VIS_LDD_2(dwin, dwin1, window);
		dwin = vis_fmul8x16(foneby8, dwin);
		dwin1 = vis_fmul8x16(foneby8, dwin1);
		VIS_LDD_2(dsrc1, dsrc3, srcr);
		MUL32_WIN(dsrc0, dsrc1, dwin, dsrc1);
		MUL32_WIN(dsrc2, dsrc3, dwin1, dsrc3);
		BUTT(dst, dst1, dsrc0, dsrc2);
		BUTT(dst2, dst3, dsrc1, dsrc3);

		BUTT(dsrc4, dsrc5, dst, dst2);

		fdst0 =
		    vis_fpackfix(FREG(sub, lo, dst1, lo, dst3, add, lo, dst1,
		    lo, dst3));
		dsrc0 = MUL32F(fsinpion4, fdst0);
		dsrc2 =
		    vis_fpackfix_pair(FREG_FZERO(add, hi, dsrc4, lo, dsrc4),
		    FREG(add, hi, dst1, hi, dsrc0, sub, 00,
		    AS_HL(sub, 00, fzero, hi, dst3), lo, dsrc0));
		dsrc3 =
		    vis_fpackfix_pair(FREG(000, hi, dsrc5, 00, 00, sub, lo,
		    dst2, lo, dst), FREG(sub, hi, dst1, hi, dsrc0, sub, hi,
		    dst3, lo, dsrc0));
		dsrc4 =
		    vis_fpackfix_pair(FREG_FZERO(sub, hi, dsrc4, lo, dsrc4),
		    FREG(sub, hi, dst1, hi, dsrc0, sub, lo, dsrc0, hi, dst3));
		dsrc5 =
		    vis_fpackfix_pair(dsrc5, FREG(add, hi, dst1, hi, dsrc0, add,
		    hi, dst3, lo, dsrc0));
		VIS_STD_4(dstc, dsrc2, dsrc3, dsrc4, dsrc5);
		break;

	case 4: {
		    mlib_f32 divider = ((mlib_f32 *)mlib_fft_rot)[1];
		    mlib_d64 reim0, reim1, reim2, reim3, reim4, reim5, reim6,
			reim7;

		    DCL_COMMON;
		    DCL_OUTVARR;
		    DCL_WINDOW;

		    PREP_OUTC;

		    LOAD_WINDOW;
		    LOAD_REAL;

		    MUL_RWIN;

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
mlib_v_SignalIFFTW_S16C_S16C_0_4(
    mlib_s16 *dstc,
    const mlib_s16 *srcc,
    const mlib_s16 *window,
    mlib_s32 order)
{
	mlib_d64 dsrc, dsrc0, dsrc1, dsrc2, dsrc3, dst, dst1;
	mlib_d64 dsrc4, dsrc5, dsrc6, dsrc7, dst2, dst3;
	mlib_d64 dwin, dwin1, dwin2, dwin3;
	mlib_f32 foneby8 = vis_to_float(0x20202020);
	mlib_f32 fonebytwo = vis_to_float(0x80808080);
	mlib_f32 fsinpion4 = vis_to_float(0x5A825A82);

	vis_write_gsr((order << 3) | 6);

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
		dwin = vis_fmul8x16(fonebytwo, dwin);
		DBL_WIND(dwin, dwin1);
		VIS_LDD_2(dsrc, dsrc3, srcc);
		MUL32_WIN(dsrc0, dsrc1, dwin, dsrc);
		MUL32_WIN(dsrc2, dsrc3, dwin1, dsrc3);
		BUTT(dsrc, dsrc0, dsrc0, dsrc2);
		dst1 = vis_fpadd32(dsrc1, dsrc3);
		dsrc1 =
		    FREG(sub, lo, dsrc1, lo, dsrc3, sub, hi, dsrc3, hi, dsrc1);
		BUTT_FPFX_PR_CROSS(dst, dst1, dsrc, dst1, dsrc0, dsrc1);
		VIS_STD_2(dstc, dst, dst1);
		break;

	case 3:
		vis_write_gsr(1 << 3);
		VIS_LDD_2(dwin, dwin2, window);
		dwin = vis_fmul8x16(foneby8, dwin);
		dwin2 = vis_fmul8x16(foneby8, dwin2);
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
		    vis_fpackfix_pair(FREG(sub, hi, dst1, lo, dst1, add, hi,
		    dst1, lo, dst1), FREG(add, hi, dst3, lo, dst3, sub, lo,
		    dst3, hi, dst3));
		vis_write_gsr(4 << 3);
		MUL32_HL(dsrc5, dsrc7, fsinpion4, dst1);
		BUTT_FPFX_PR(dst3, dsrc5, dsrc0, dst, dsrc1, dsrc5);
		dsrc4 =
		    vis_fpackfix_pair(FREG(sub, hi, dsrc2, lo, dst2, add, lo,
		    dsrc2, hi, dst2), vis_fpsub32(dsrc3, dsrc7));
		dsrc6 =
		    vis_fpackfix_pair(FREG(add, hi, dsrc2, lo, dst2, sub, lo,
		    dsrc2, hi, dst2), vis_fpadd32(dsrc3, dsrc7));
		VIS_STD_4(dstc, dst3, dsrc4, dsrc5, dsrc6);
		break;

	case 4: {
		    mlib_d64 reim0, reim1, reim2, reim3, reim4, reim5, reim6,
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
mlib_v_SignalIFFTW_S16_S16_0_4(
    mlib_s16 *dstr,
    mlib_s16 *dsti,
    const mlib_s16 *srcr,
    const mlib_s16 *srci,
    const mlib_s16 *window,
    mlib_s32 order)
{
	mlib_d64 dsrc, dsrc0, dsrc1, dsrc2, dsrc3, dst, dst1;
	mlib_d64 dsrc4, dsrc5, dsrc6, dsrc7, dst2, dst3, dwin, dwin1;
	mlib_f32 fdst0, fdst1;
	mlib_f32 foneby8 = vis_to_float(0x20202020);
	mlib_f32 fonebytwo = vis_to_float(0x80808080);
	mlib_f32 fsinpion4 = vis_to_float(0x5A825A82);

	vis_write_gsr((order << 3) | 6);

	switch (order) {
	case 0:
		dstr[0] = (srcr[0] * window[0]) >> 15;
		dsti[0] = (srci[0] * window[0]) >> 15;
		break;

	case 1:
		VIS_LDF(dwin, window);
		VIS_LDF(dsrc0, srcr);
		VIS_LDF(dsrc1, srci);
		dsrc0 = MUL32F(vis_read_hi(dwin), vis_read_hi(dsrc0));
		dsrc1 = MUL32F(vis_read_hi(dwin), vis_read_hi(dsrc1));
		fdst0 =
		    vis_fpackfix(FREG(add, hi, dsrc0, lo, dsrc0, sub, hi, dsrc0,
		    lo, dsrc0));
		fdst1 =
		    vis_fpackfix(FREG(add, hi, dsrc1, lo, dsrc1, sub, hi, dsrc1,
		    lo, dsrc1));
		VIS_STF(dstr, fdst0);
		VIS_STF(dsti, fdst1);
		break;

	case 2:
		VIS_LDD(dwin, window);
		dwin = vis_fmul8x16(fonebytwo, dwin);
		VIS_LDD(dsrc, srcr);
		VIS_LDD(dsrc3, srci);
		MUL32_WIN(dsrc0, dsrc1, dwin, dsrc);
		MUL32_WIN(dsrc2, dsrc3, dwin, dsrc3);
		BUTT(dsrc, dsrc0, dsrc0, dsrc1);
		BUTT(dst1, dsrc2, dsrc2, dsrc3);
		dst =
		    vis_fpackfix_pair(FREG(add, hi, dsrc, lo, dsrc, sub, hi,
		    dsrc0, lo, dsrc2), FREG(sub, hi, dsrc, lo, dsrc, add, hi,
		    dsrc0, lo, dsrc2));
		dst1 =
		    vis_fpackfix_pair(FREG(add, hi, dst1, lo, dst1, add, hi,
		    dsrc2, lo, dsrc0), FREG(sub, hi, dst1, lo, dst1, sub, hi,
		    dsrc2, lo, dsrc0));
		VIS_STD(dstr, dst);
		VIS_STD(dsti, dst1);
		break;

	case 3:
		vis_write_gsr(1 << 3);
		VIS_LDD_2(dwin, dwin1, window);
		dwin = vis_fmul8x16(foneby8, dwin);
		dwin1 = vis_fmul8x16(foneby8, dwin1);
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
		    vis_fpackfix_pair(FREG(sub, lo, dsrc6, lo, dst2, add, lo,
		    dsrc6, lo, dst2), FREG(add, lo, dsrc7, lo, dst3, sub, lo,
		    dsrc7, lo, dst3));
		vis_write_gsr(4 << 3);
		MUL32_HL(dsrc0, dsrc1, fsinpion4, dsrc1);
		dsrc2 =
		    vis_fpackfix_pair(FREG(add, hi, dsrc4, lo, dsrc4, add, hi,
		    dsrc6, hi, dsrc0), FREG(sub, hi, dsrc5, lo, dst1, sub, hi,
		    dsrc7, hi, dsrc1));
		dsrc3 =
		    vis_fpackfix_pair(FREG(sub, hi, dsrc4, lo, dsrc4, sub, hi,
		    dsrc6, hi, dsrc0), FREG(add, hi, dsrc5, lo, dst1, add, hi,
		    dsrc7, hi, dsrc1));
		dsrc4 =
		    vis_fpackfix_pair(FREG(add, hi, dst, lo, dst, add, hi, dst2,
		    lo, dsrc0), FREG(add, hi, dst1, lo, dsrc5, add, hi, dst3,
		    lo, dsrc1));
		dsrc5 =
		    vis_fpackfix_pair(FREG(sub, hi, dst, lo, dst, sub, hi, dst2,
		    lo, dsrc0), FREG(sub, hi, dst1, lo, dsrc5, sub, hi, dst3,
		    lo, dsrc1));
		VIS_STD_2(dstr, dsrc2, dsrc3);
		VIS_STD_2(dsti, dsrc4, dsrc5);
		break;

	case 4: {
		    mlib_d64 tmp;

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
mlib_v_SignalIFFTW_S16_S16C_0_4(
    mlib_s16 *dstr,
    const mlib_s16 *srcc,
    const mlib_s16 *window,
    mlib_s32 order)
{
	mlib_d64 dsrc, dsrc0, dsrc1, dsrc2, dsrc3, dst, dst1;
	mlib_d64 dsrc4, dsrc5, dsrc6, dsrc7, dst2, dst3;
	mlib_d64 dwin, dwin1, dwin2, dwin3;
	mlib_f32 fdst0, fdst1, fdst2;
	mlib_f32 foneby8 = vis_to_float(0x20202020);
	mlib_f32 fonebytwo = vis_to_float(0x80808080);
	mlib_f32 fsinpion4 = vis_to_float(0x5A825A82);

	vis_write_gsr((order << 3) | 6);

	switch (order) {
	case 0:
		dstr[0] = (srcc[0] * window[0]) >> 15;
		break;

	case 1:
		VIS_LDF_DBL(dwin, window);
		VIS_LDD(dsrc, srcc);
		MUL32_WIN(dsrc0, dsrc1, dwin, dsrc);
		VIS_ALIGNADDR((void *)0, 6);
		fdst0 =
		    vis_fpackfix(FREG(add, hi, dsrc0, hi, dsrc1, sub, hi, dsrc0,
		    hi, dsrc1));

		VIS_STF(dstr, fdst0);
		break;

	case 2:
		VIS_LDD(dwin, window);
		dwin = vis_fmul8x16(fonebytwo, dwin);
		DBL_WIND(dwin, dwin1);
		VIS_LDD_2(dsrc, dsrc3, srcc);
		MUL32_WIN(dsrc0, dsrc1, dwin, dsrc);
		MUL32_WIN(dsrc2, dsrc3, dwin1, dsrc3);
		BUTT(dsrc, dsrc0, dsrc0, dsrc2);
		BUTT(dst, dsrc1, dsrc1, dsrc3);
		dst =
		    vis_fpackfix_pair(FREG(add, hi, dsrc, hi, dst, sub, hi,
		    dsrc0, lo, dsrc1), FREG(sub, hi, dsrc, hi, dst, add, hi,
		    dsrc0, lo, dsrc1));
		VIS_STD(dstr, dst);
		break;

	case 3:
		vis_write_gsr(1 << 3);
		VIS_LDD_2(dwin, dwin2, window);
		dwin = vis_fmul8x16(foneby8, dwin);
		dwin2 = vis_fmul8x16(foneby8, dwin2);
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
		    vis_fpackfix(FREG(sub, hi, dst1, lo, dst1, add, hi, dst3,
		    lo, dst3));
		dsrc5 = MUL32F(fsinpion4, fdst0);
		vis_write_gsr(4 << 3);
		dsrc4 =
		    vis_fpackfix_pair(FREG(add, hi, dsrc0, hi, dst, add, 00,
		    fdst2, hi, dsrc5), FREG(sub, hi, dsrc2, lo, dst2, sub, 00,
		    fdst1, lo, dsrc5));
		dsrc5 =
		    vis_fpackfix_pair(FREG(sub, hi, dsrc0, hi, dst, sub, 00,
		    fdst2, hi, dsrc5), FREG(add, hi, dsrc2, lo, dst2, add, 00,
		    fdst1, lo, dsrc5));
		VIS_STD_2(dstr, dsrc4, dsrc5);
		break;

	case 4: {
		    mlib_d64 reim0, reim1, reim2, reim3, reim4, reim5, reim6,
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
		return mlib_v_SignalFFTW_S16C_S16C_0_4(dstc, srcc, window,
		    order);
	else
		return mlib_v_fftDispatcherW_2(dstc, NULL, (void *)srcc, NULL,
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
		return mlib_v_SignalFFTW_S16_S16_0_4(dstr, dsti, srcr, srci,
		    window, order);
	else
		return mlib_v_fftDispatcherW_2(dstr, dsti, (void *)srcr,
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
		return mlib_v_SignalFFTW_S16C_S16_0_4(dstc, srcr, window,
		    order);
	else
		return mlib_v_fftDispatcherW_2(dstc, NULL, (void *)srcr, NULL,
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
		return mlib_v_SignalIFFTW_S16C_S16C_0_4(dstc, srcc, window,
		    order);
	else
		return mlib_v_fftDispatcherW_2(dstc, NULL, (void *)srcc, NULL,
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
		return mlib_v_SignalIFFTW_S16_S16_0_4(dstr, dsti, srcr, srci,
		    window, order);
	else
		return mlib_v_fftDispatcherW_2(dstr, dsti, (void *)srcr,
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
		return mlib_v_SignalIFFTW_S16_S16C_0_4(dstr, srcc, window,
		    order);
	else
		return mlib_v_fftDispatcherW_2(dstr, NULL, (void *)srcc, NULL,
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
