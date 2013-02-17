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

#pragma ident	"@(#)mlib_SignalFFT_F32.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *  NOTE: FFT below means F(ast)F(ourier)T(ransform)
 *      mlib_SignalFFT_1_F32C_F32C       - FFT of mlib_f32 format complex array
 *                                         into complex array
 *      mlib_SignalFFT_1_F32_F32         - FFT of mlib_f32 format 2 arrays as
 *                                         real and imag parts of complex into
 *                                         2 arrays as real and
 *                                         imag parts of complex
 *      mlib_SignalFFT_1_F32C_F32        - FFT of mlib_f32 format
 *                                         1 array as real part
 *                                         of( real-valued ) complex
 *                                         into complex array
 *      mlib_SignalIFFT_1_F32C_F32C      - inverse FFT of mlib_f32 format
 *                                         complex array
 *                                         into complex array
 *      mlib_SignalIFFT_1_F32_F32        - inverse FFT of mlib_f32 format
 *                                         2 arrays as real and imag parts
 *                                         of complex into 2 arrays as
 *                                         real and imag parts of complex
 *      mlib_SignalIFFT_1_F32_F32C       - inverse FFT of mlib_f32
 *                                         format complex array
 *                                         into 1 array as real part
 *                                         of( real-valued ) complex
 *                                         NOTE: this is a semantic pair
 *                                           to mlib_SignalFFT_1_F32C_F32
 *      mlib_SignalFFT_1_F32C            - FFT of mlib_f32 format complex array
 *                                         into complex array
 *                                         (input and output arrays
 *                                         are the same)
 *      mlib_SignalFFT_1_F32             - FFT of mlib_f32 format 2 arrays as
 *                                         real and imag parts of complex into
 *                                         2 arrays as real and
 *                                         imag parts of complex
 *                                         (input and output array pairs
 *                                         are the same)
 *      mlib_SignalIFFT_1_F32C           - inverse FFT of mlib_f32 format
 *                                         complex array into complex array
 *                                         (input and output arrays
 *                                         are the same)
 *      mlib_SignalIFFT_1_F32            - inverse FFT of mlib_f32 format
 *                                         2 arrays as real and imag parts
 *                                         of complex into 2 arrays as
 *                                         real and imag parts of complex
 *                                         (input and output array pairs
 *                                         are the same)
 *      mlib_SignalFFTW_1_F32C_F32C      - windowed FFT of mlib_f32
 *                                         format complex array
 *                                         into complex array
 *      mlib_SignalFFTW_1_F32_F32        - windowed FFT of mlib_f32
 *                                         format 2 arrays as real
 *                                         and imag parts of complex
 *                                         into 2 arrays as real
 *                                         and imag parts of complex
 *      mlib_SignalFFTW_1_F32C_F32       - windowed FFT of mlib_f32
 *                                         format 1 array as real part
 *                                         of( real-valued ) complex
 *                                         into complex array
 *      mlib_SignalIFFTW_1_F32C_F32C     - inverse windowed FFT
 *                                         of mlib_f32 format
 *                                         complex array into complex array
 *      mlib_SignalIFFTW_1_F32_F32       - inverse windowed FFT
 *                                         of mlib_f32 format
 *                                         2 arrays as real and imag parts of
 *                                         complex into 2 arrays as real and
 *                                         imag parts of complex
 *      mlib_SignalIFFTW_1_F32_F32C      - inverse windowed FFT
 *                                         of mlib_f32 format
 *                                         complex array into 1 array as real
 *                                         part of( real-valued ) complex
 *                                         NOTE: this is a semantic pair
 *                                           to mlib_SignalFFTW_1_F32C_F32
 *      mlib_SignalFFTW_1_F32C           - windowed FFT of mlib_f32
 *                                         format complex
 *                                         array into complex array
 *                                         (input and output arrays
 *                                         are the same)
 *      mlib_SignalFFTW_1_F32            - windowed FFT
 *                                         of mlib_f32 format 2 arrays
 *                                         as real and imag parts
 *                                         of complex into
 *                                         2 arrays as real and imag parts of
 *                                         complex
 *                                         (input and output array pairs
 *                                         are the same)
 *      mlib_SignalIFFTW_1_F32C          - inverse windowed FFT
 *                                         of mlib_f32 format complex
 *                                         array into complex array
 *                                         (input and output arrays
 *                                         are the same)
 *      mlib_SignalIFFTW_1_F32           - inverse windowed FFT
 *                                         of mlib_f32 format
 *                                         2 arrays as real and imag parts of
 *                                         complex into 2 arrays as real and
 *                                         imag parts of complex
 *                                         (input and output array pairs
 *                                         are the same)
 *
 *      mlib_SignalFFT_2_F32C_F32C       - FFT of mlib_f32 format
 *                                         complex array into
 *                                         complex array
 *      mlib_SignalFFT_2_F32_F32         - FFT of mlib_f32 format
 *                                         2 arrays as real and
 *                                         imag parts of complex
 *                                         into 2 arrays as
 *                                         real and imag parts of complex
 *      mlib_SignalFFT_2_F32C_F32        - FFT of mlib_f32 format 1 array
 *                                         as real part
 *                                         of( real-valued ) complex
 *                                         into complex array
 *      mlib_SignalIFFT_2_F32C_F32C      - inverse FFT of mlib_f32
 *                                         format complex array
 *                                         into complex array
 *      mlib_SignalIFFT_2_F32_F32        - inverse FFT of mlib_f32
 *                                         format 2 arrays as real
 *                                         and imag parts of complex
 *                                         into 2 arrays as
 *                                         real and imag parts of complex
 *      mlib_SignalIFFT_2_F32_F32C       - inverse FFT of mlib_f32
 *                                         format complex array
 *                                         into 1 array as real part
 *                                         of( real-valued ) complex
 *                                         NOTE: this is a semantic pair
 *                                           to mlib_SignalFFT_2_F32C_F32
 *      mlib_SignalFFT_2_F32C            - FFT of mlib_f32
 *                                         format complex array into
 *                                         complex array
 *                                         (input and output arrays
 *                                         are the same)
 *      mlib_SignalFFT_2_F32             - FFT of mlib_f32 format
 *                                         2 arrays as real and
 *                                         imag parts of complex
 *                                         into 2 arrays as
 *                                         real and imag parts of complex
 *                                         (input and output array pairs
 *                                         are the same)
 *      mlib_SignalIFFT_2_F32C           - inverse FFT of mlib_f32
 *                                         format complex array
 *                                         into complex array
 *                                         (input and output arrays
 *                                         are the same)
 *      mlib_SignalIFFT_2_F32            - inverse FFT of mlib_f32
 *                                         format 2 arrays as real
 *                                         and imag parts of complex
 *                                         into 2 arrays as
 *                                         real and imag parts of complex
 *                                         (input and output array pairs
 *                                         are the same)
 *      mlib_SignalFFTW_2_F32C_F32C      - windowed FFT of mlib_f32
 *                                         format complex
 *                                         array into complex array
 *      mlib_SignalFFTW_2_F32_F32        - windowed FFT of mlib_f32
 *                                         format 2 arrays
 *                                         as real and imag parts
 *                                         of complex into 2
 *                                         arrays as real and imag parts
 *                                         of complex
 *      mlib_SignalFFTW_2_F32C_F32       - windowed FFT of mlib_f32
 *                                         format 1 array
 *                                         as real part of( real-valued )
 *                                         complex into complex array
 *      mlib_SignalIFFTW_2_F32C_F32C     - inverse windowed FFT
 *                                         of mlib_f32 format
 *                                         complex array into complex array
 *      mlib_SignalIFFTW_2_F32_F32       - inverse windowed FFT
 *                                         of mlib_f32 format
 *                                         2 arrays as real and imag parts of
 *                                         complex into 2 arrays as real and
 *                                         imag parts of complex
 *      mlib_SignalIFFTW_2_F32_F32C      - inverse windowed FFT
 *                                         of mlib_f32 format
 *                                         complex array into 1 array as real
 *                                         part of( real-valued ) complex
 *                                         NOTE: this is a semantic pair
 *                                           to mlib_SignalFFTW_2_F32C_F32
 *      mlib_SignalFFTW_2_F32C           - windowed FFT of mlib_f32
 *                                         format complex
 *                                         array into complex array
 *                                         (input and output arrays
 *                                         are the same)
 *      mlib_SignalFFTW_2_F32            - windowed FFT
 *                                         of mlib_f32 format 2 arrays
 *                                         as real and imag parts
 *                                         of complex into
 *                                         2 arrays as real and imag parts of
 *                                         complex
 *                                         (input and output array pairs
 *                                         are the same)
 *      mlib_SignalIFFTW_2_F32C          - inverse windowed FFT
 *                                         of mlib_f32 format complex
 *                                         array into complex array
 *                                         (input and output arrays
 *                                         are the same)
 *      mlib_SignalIFFTW_2_F32           - inverse windowed FFT
 *                                         of mlib_f32 format
 *                                         2 arrays as real and imag parts of
 *                                         complex into 2 arrays as real and
 *                                         imag parts of complex
 *                                         (input and output array pairs
 *                                         are the same)
 *
 *      mlib_SignalFFT_3_F32C_F32C       - FFT of mlib_f32 format
 *                                         complex array into
 *                                         complex array
 *      mlib_SignalFFT_3_F32_F32         - FFT of mlib_f32 format
 *                                         2 arrays as real and
 *                                         imag parts of complex
 *                                         into 2 arrays as
 *                                         real and imag parts of complex
 *      mlib_SignalFFT_3_F32C_F32        - FFT of mlib_f32
 *                                         format 1 array as real part
 *                                         of( real-valued ) complex
 *                                         into complex array
 *      mlib_SignalIFFT_3_F32C_F32C      - inverse FFT
 *                                         of mlib_f32 format complex array
 *                                         into complex array
 *      mlib_SignalIFFT_3_F32_F32        - inverse FFT
 *                                         of mlib_f32 format 2 arrays as real
 *                                         and imag parts of complex
 *                                         into 2 arrays as
 *                                         real and imag parts of complex
 *      mlib_SignalIFFT_3_F32_F32C       - inverse FFT
 *                                         of mlib_f32 format complex array
 *                                         into 1 array as real part
 *                                         of( real-valued ) complex
 *                                         NOTE: this is a semantic pair
 *                                         to mlib_SignalFFT_3_F32C_F32
 *      mlib_SignalFFT_3_F32C            - FFT of mlib_f32
 *                                         format complex array into
 *                                         complex array
 *                                         (input and output arrays
 *                                         are the same)
 *      mlib_SignalFFT_3_F32             - FFT of mlib_f32
 *                                         format 2 arrays as real and
 *                                         imag parts of complex
 *                                         into 2 arrays as
 *                                         real and imag parts of complex
 *                                         (input and output array pairs
 *                                         are the same)
 *      mlib_SignalIFFT_3_F32C           - inverse FFT
 *                                         of mlib_f32 format complex array
 *                                         into complex array
 *                                         (input and output arrays
 *                                         are the same)
 *      mlib_SignalIFFT_3_F32            - inverse FFT of mlib_f32
 *                                         format 2 arrays as real
 *                                         and imag parts of complex
 *                                         into 2 arrays as
 *                                         real and imag parts of complex
 *                                         (input and output array pairs
 *                                         are the same)
 *      mlib_SignalFFTW_3_F32C_F32C      - windowed FFT
 *                                         of mlib_f32 format complex
 *                                         array into complex array
 *      mlib_SignalFFTW_3_F32_F32        - windowed FFT
 *                                         of mlib_f32 format 2 arrays
 *                                         as real and imag parts
 *                                         of complex into 2
 *                                         arrays as real and
 *                                         imag parts of complex
 *      mlib_SignalFFTW_3_F32C_F32       - windowed FFT
 *                                         of mlib_f32 format 1 array
 *                                         as real part of( real-valued )
 *                                         complex into complex array
 *      mlib_SignalIFFTW_3_F32C_F32C     - inverse windowed FFT
 *                                         of mlib_f32 format
 *                                         complex array into complex array
 *      mlib_SignalIFFTW_3_F32_F32       - inverse windowed FFT
 *                                         of mlib_f32 format
 *                                         2 arrays as real and imag parts of
 *                                         complex into 2 arrays as real and
 *                                         imag parts of complex
 *      mlib_SignalIFFTW_3_F32_F32C      - inverse windowed FFT
 *                                         of mlib_f32 format
 *                                         complex array into 1 array as real
 *                                         part of( real-valued ) complex
 *                                         NOTE: this is a semantic pair
 *                                           to mlib_SignalFFTW_3_F32C_F32
 *      mlib_SignalFFTW_3_F32C           - windowed FFT
 *                                         of mlib_f32 format complex
 *                                         array into complex array
 *                                         (input and output arrays
 *                                         are the same)
 *      mlib_SignalFFTW_3_F32            - windowed FFT
 *                                         of mlib_f32 format 2 arrays
 *                                         as real and imag parts
 *                                         of complex into
 *                                         2 arrays as real and imag parts of
 *                                         complex
 *                                         (input and output array pairs
 *                                         are the same)
 *      mlib_SignalIFFTW_3_F32C          - inverse windowed FFT
 *                                         of mlib_f32 format complex
 *                                         array into complex array
 *                                         (input and output arrays
 *                                         are the same)
 *      mlib_SignalIFFTW_3_F32           - inverse windowed FFT
 *                                         of mlib_f32 format
 *                                         2 arrays as real and imag parts of
 *                                         complex into 2 arrays as real and
 *                                         imag parts of complex
 *                                         (input and output array pairs
 *                                         are the same)
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalFFTW_1_F32(mlib_f32 *datar,
 *                                        mlib_f32 *datai,
 *                                        const mlib_f32 *window,
 *                                        mlib_s32 order);
 *      mlib_status mlib_SignalFFTW_1_F32C(mlib_f32 *datac,
 *                                         const mlib_f32 *window,
 *                                         mlib_s32 order);
 *      mlib_status mlib_SignalFFTW_1_F32C_F32(mlib_f32 *dstc,
 *                                             const mlib_f32 *srcr,
 *                                             const mlib_f32 *window,
 *                                             mlib_s32 order);
 *      mlib_status mlib_SignalFFTW_1_F32C_F32C(mlib_f32 *dstc,
 *                                              const mlib_f32 *srcc,
 *                                              const mlib_f32 *window,
 *                                              mlib_s32 order);
 *      mlib_status mlib_SignalFFTW_1_F32_F32(mlib_f32 *dstr,
 *                                            mlib_f32 *dsti,
 *                                            const mlib_f32 *srcr,
 *                                            const mlib_f32 *srci,
 *                                            const mlib_f32 *window,
 *                                            mlib_s32 order);
 *      mlib_status mlib_SignalFFTW_2_F32(mlib_f32 *datar,
 *                                        mlib_f32 *datai,
 *                                        const mlib_f32 *window,
 *                                        mlib_s32 order);
 *      mlib_status mlib_SignalFFTW_2_F32C(mlib_f32 *datac,
 *                                         const mlib_f32 *window,
 *                                         mlib_s32 order);
 *      mlib_status mlib_SignalFFTW_2_F32C_F32(mlib_f32 *dstc,
 *                                             const mlib_f32 *srcr,
 *                                             const mlib_f32 *window,
 *                                             mlib_s32 order);
 *      mlib_status mlib_SignalFFTW_2_F32C_F32C(mlib_f32 *dstc,
 *                                              const mlib_f32 *srcc,
 *                                              const mlib_f32 *window,
 *                                              mlib_s32 order);
 *      mlib_status mlib_SignalFFTW_2_F32_F32(mlib_f32 *dstr,
 *                                            mlib_f32 *dsti,
 *                                            const mlib_f32 *srcr,
 *                                            const mlib_f32 *srci,
 *                                            const mlib_f32 *window,
 *                                            mlib_s32 order);
 *      mlib_status mlib_SignalFFTW_3_F32(mlib_f32 *datar,
 *                                        mlib_f32 *datai,
 *                                        const mlib_f32 *window,
 *                                        mlib_s32 order);
 *      mlib_status mlib_SignalFFTW_3_F32C(mlib_f32 *datac,
 *                                         const mlib_f32 *window,
 *                                         mlib_s32 order);
 *      mlib_status mlib_SignalFFTW_3_F32C_F32(mlib_f32 *dstc,
 *                                             const mlib_f32 *srcr,
 *                                             const mlib_f32 *window,
 *                                             mlib_s32 order);
 *      mlib_status mlib_SignalFFTW_3_F32C_F32C(mlib_f32 *dstc,
 *                                              const mlib_f32 *srcc,
 *                                              const mlib_f32 *window,
 *                                              mlib_s32 order);
 *      mlib_status mlib_SignalFFTW_3_F32_F32(mlib_f32 *dstr,
 *                                            mlib_f32 *dsti,
 *                                            const mlib_f32 *srcr,
 *                                            const mlib_f32 *srci,
 *                                            const mlib_f32 *window,
 *                                            mlib_s32 order);
 *      mlib_status mlib_SignalFFT_1_F32(mlib_f32 *datar,
 *                                       mlib_f32 *datai,
 *                                       mlib_s32 order);
 *      mlib_status mlib_SignalFFT_1_F32C(mlib_f32 *datac,
 *                                        mlib_s32 order);
 *      mlib_status mlib_SignalFFT_1_F32C_F32(mlib_f32 *dstc,
 *                                            const mlib_f32 *srcr,
 *                                            mlib_s32 order);
 *      mlib_status mlib_SignalFFT_1_F32C_F32C(mlib_f32 *dstc,
 *                                             const mlib_f32 *srcc,
 *                                             mlib_s32 order);
 *      mlib_status mlib_SignalFFT_1_F32_F32(mlib_f32 *dstr,
 *                                           mlib_f32 *dsti,
 *                                           const mlib_f32 *srcr,
 *                                           const mlib_f32 *srci,
 *                                           mlib_s32 order);
 *      mlib_status mlib_SignalFFT_2_F32(mlib_f32 *datar,
 *                                       mlib_f32 *datai,
 *                                       mlib_s32 order);
 *      mlib_status mlib_SignalFFT_2_F32C(mlib_f32 *datac,
 *                                        mlib_s32 order);
 *      mlib_status mlib_SignalFFT_2_F32C_F32(mlib_f32 *dstc,
 *                                            const mlib_f32 *srcr,
 *                                            mlib_s32 order);
 *      mlib_status mlib_SignalFFT_2_F32C_F32C(mlib_f32 *dstc,
 *                                             const mlib_f32 *srcc,
 *                                             mlib_s32 order);
 *      mlib_status mlib_SignalFFT_2_F32_F32(mlib_f32 *dstr,
 *                                           mlib_f32 *dsti,
 *                                           const mlib_f32 *srcr,
 *                                           const mlib_f32 *srci,
 *                                           mlib_s32 order);
 *      mlib_status mlib_SignalFFT_3_F32(mlib_f32 *datar,
 *                                       mlib_f32 *datai,
 *                                       mlib_s32 order);
 *      mlib_status mlib_SignalFFT_3_F32C(mlib_f32 *datac,
 *                                        mlib_s32 order);
 *      mlib_status mlib_SignalFFT_3_F32C_F32(mlib_f32 *dstc,
 *                                            const mlib_f32 *srcr,
 *                                            mlib_s32 order);
 *      mlib_status mlib_SignalFFT_3_F32C_F32C(mlib_f32 *dstc,
 *                                             const mlib_f32 *srcc,
 *                                             mlib_s32 order);
 *      mlib_status mlib_SignalFFT_3_F32_F32(mlib_f32 *dstr,
 *                                           mlib_f32 *dsti,
 *                                           const mlib_f32 *srcr,
 *                                           const mlib_f32 *srci,
 *                                           mlib_s32 order);
 *      mlib_status mlib_SignalIFFTW_1_F32(mlib_f32 *datar,
 *                                         mlib_f32 *datai,
 *                                         const mlib_f32 *window,
 *                                         mlib_s32 order);
 *      mlib_status mlib_SignalIFFTW_1_F32C(mlib_f32 *datac,
 *                                          const mlib_f32 *window,
 *                                          mlib_s32 order);
 *      mlib_status mlib_SignalIFFTW_1_F32C_F32C(mlib_f32 *dstc,
 *                                               const mlib_f32 *srcc,
 *                                               const mlib_f32 *window,
 *                                               mlib_s32 order);
 *      mlib_status mlib_SignalIFFTW_1_F32_F32(mlib_f32 *dstr,
 *                                             mlib_f32 *dsti,
 *                                             const mlib_f32 *srcr,
 *                                             const mlib_f32 *srci,
 *                                             const mlib_f32 *window,
 *                                             mlib_s32 order);
 *      mlib_status mlib_SignalIFFTW_1_F32_F32C(mlib_f32 *dstr,
 *                                              const mlib_f32 *srcc,
 *                                              const mlib_f32 *window,
 *                                              mlib_s32 order);
 *      mlib_status mlib_SignalIFFTW_2_F32(mlib_f32 *datar,
 *                                         mlib_f32 *datai,
 *                                         const mlib_f32 *window,
 *                                         mlib_s32 order);
 *      mlib_status mlib_SignalIFFTW_2_F32C(mlib_f32 *datac,
 *                                          const mlib_f32 *window,
 *                                          mlib_s32 order);
 *      mlib_status mlib_SignalIFFTW_2_F32C_F32C(mlib_f32 *dstc,
 *                                               const mlib_f32 *srcc,
 *                                               const mlib_f32 *window,
 *                                               mlib_s32 order);
 *      mlib_status mlib_SignalIFFTW_2_F32_F32(mlib_f32 *dstr,
 *                                             mlib_f32 *dsti,
 *                                             const mlib_f32 *srcr,
 *                                             const mlib_f32 *srci,
 *                                             const mlib_f32 *window,
 *                                             mlib_s32 order);
 *      mlib_status mlib_SignalIFFTW_2_F32_F32C(mlib_f32 *dstr,
 *                                              const mlib_f32 *srcc,
 *                                              const mlib_f32 *window,
 *                                              mlib_s32 order);
 *      mlib_status mlib_SignalIFFTW_3_F32(mlib_f32 *datar,
 *                                         mlib_f32 *datai,
 *                                         const mlib_f32 *window,
 *                                         mlib_s32 order);
 *      mlib_status mlib_SignalIFFTW_3_F32C(mlib_f32 *datac,
 *                                          const mlib_f32 *window,
 *                                          mlib_s32 order);
 *      mlib_status mlib_SignalIFFTW_3_F32C_F32C(mlib_f32 *dstc,
 *                                               const mlib_f32 *srcc,
 *                                               const mlib_f32 *window,
 *                                               mlib_s32 order);
 *      mlib_status mlib_SignalIFFTW_3_F32_F32(mlib_f32 *dstr,
 *                                             mlib_f32 *dsti,
 *                                             const mlib_f32 *srcr,
 *                                             const mlib_f32 *srci,
 *                                             const mlib_f32 *window,
 *                                             mlib_s32 order);
 *      mlib_status mlib_SignalIFFTW_3_F32_F32C(mlib_f32 *dstr,
 *                                              const mlib_f32 *srcc,
 *                                              const mlib_f32 *window,
 *                                              mlib_s32 order);
 *      mlib_status mlib_SignalIFFT_1_F32(mlib_f32 *datar,
 *                                        mlib_f32 *datai,
 *                                        mlib_s32 order);
 *      mlib_status mlib_SignalIFFT_1_F32C(mlib_f32 *datac,
 *                                         mlib_s32 order);
 *      mlib_status mlib_SignalIFFT_1_F32C_F32C(mlib_f32 *dstc,
 *                                              const mlib_f32 *srcc,
 *                                              mlib_s32 order);
 *      mlib_status mlib_SignalIFFT_1_F32_F32(mlib_f32 *dstr,
 *                                            mlib_f32 *dsti,
 *                                            const mlib_f32 *srcr,
 *                                            const mlib_f32 *srci,
 *                                            mlib_s32 order);
 *      mlib_status mlib_SignalIFFT_1_F32_F32C(mlib_f32 *dstr,
 *                                             const mlib_f32 *srcc,
 *                                             mlib_s32 order);
 *      mlib_status mlib_SignalIFFT_2_F32(mlib_f32 *datar,
 *                                        mlib_f32 *datai,
 *                                        mlib_s32 order);
 *      mlib_status mlib_SignalIFFT_2_F32C(mlib_f32 *datac,
 *                                         mlib_s32 order);
 *      mlib_status mlib_SignalIFFT_2_F32C_F32C(mlib_f32 *dstc,
 *                                              const mlib_f32 *srcc,
 *                                              mlib_s32 order);
 *      mlib_status mlib_SignalIFFT_2_F32_F32(mlib_f32 *dstr,
 *                                            mlib_f32 *dsti,
 *                                            const mlib_f32 *srcr,
 *                                            const mlib_f32 *srci,
 *                                            mlib_s32 order);
 *      mlib_status mlib_SignalIFFT_2_F32_F32C(mlib_f32 *dstr,
 *                                             const mlib_f32 *srcc,
 *                                             mlib_s32 order);
 *      mlib_status mlib_SignalIFFT_3_F32(mlib_f32 *datar,
 *                                        mlib_f32 *datai,
 *                                        mlib_s32 order);
 *      mlib_status mlib_SignalIFFT_3_F32C(mlib_f32 *datac,
 *                                         mlib_s32 order);
 *      mlib_status mlib_SignalIFFT_3_F32C_F32C(mlib_f32 *dstc,
 *                                              const mlib_f32 *srcc,
 *                                              mlib_s32 order);
 *      mlib_status mlib_SignalIFFT_3_F32_F32(mlib_f32 *dstr,
 *                                            mlib_f32 *dsti,
 *                                            const mlib_f32 *srcr,
 *                                            const mlib_f32 *srci,
 *                                            mlib_s32 order);
 *      mlib_status mlib_SignalIFFT_3_F32_F32C(mlib_f32 *dstr,
 *                                             const mlib_f32 *srcc,
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
 *      window  - Window coefficient array. Note the data type of the window
 *                coefficient array is mlib_f32
 *                for both F32 and F32C input signal
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
 */

#include <mlib_signal.h>

#define	FFT_UTIL_C

#include <mlib_SignalFFTUtil.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalFFTW_1_F32 = __mlib_SignalFFTW_1_F32
#pragma weak mlib_SignalFFTW_1_F32C = __mlib_SignalFFTW_1_F32C
#pragma weak mlib_SignalFFTW_1_F32C_F32 = __mlib_SignalFFTW_1_F32C_F32
#pragma weak mlib_SignalFFTW_1_F32C_F32C = __mlib_SignalFFTW_1_F32C_F32C
#pragma weak mlib_SignalFFTW_1_F32_F32 = __mlib_SignalFFTW_1_F32_F32
#pragma weak mlib_SignalFFTW_2_F32 = __mlib_SignalFFTW_2_F32
#pragma weak mlib_SignalFFTW_2_F32C = __mlib_SignalFFTW_2_F32C
#pragma weak mlib_SignalFFTW_2_F32C_F32 = __mlib_SignalFFTW_2_F32C_F32
#pragma weak mlib_SignalFFTW_2_F32C_F32C = __mlib_SignalFFTW_2_F32C_F32C
#pragma weak mlib_SignalFFTW_2_F32_F32 = __mlib_SignalFFTW_2_F32_F32
#pragma weak mlib_SignalFFTW_3_F32 = __mlib_SignalFFTW_3_F32
#pragma weak mlib_SignalFFTW_3_F32C = __mlib_SignalFFTW_3_F32C
#pragma weak mlib_SignalFFTW_3_F32C_F32 = __mlib_SignalFFTW_3_F32C_F32
#pragma weak mlib_SignalFFTW_3_F32C_F32C = __mlib_SignalFFTW_3_F32C_F32C
#pragma weak mlib_SignalFFTW_3_F32_F32 = __mlib_SignalFFTW_3_F32_F32
#pragma weak mlib_SignalFFT_1_F32 = __mlib_SignalFFT_1_F32
#pragma weak mlib_SignalFFT_1_F32C = __mlib_SignalFFT_1_F32C
#pragma weak mlib_SignalFFT_1_F32C_F32 = __mlib_SignalFFT_1_F32C_F32
#pragma weak mlib_SignalFFT_1_F32C_F32C = __mlib_SignalFFT_1_F32C_F32C
#pragma weak mlib_SignalFFT_1_F32_F32 = __mlib_SignalFFT_1_F32_F32
#pragma weak mlib_SignalFFT_2_F32 = __mlib_SignalFFT_2_F32
#pragma weak mlib_SignalFFT_2_F32C = __mlib_SignalFFT_2_F32C
#pragma weak mlib_SignalFFT_2_F32C_F32 = __mlib_SignalFFT_2_F32C_F32
#pragma weak mlib_SignalFFT_2_F32C_F32C = __mlib_SignalFFT_2_F32C_F32C
#pragma weak mlib_SignalFFT_2_F32_F32 = __mlib_SignalFFT_2_F32_F32
#pragma weak mlib_SignalFFT_3_F32 = __mlib_SignalFFT_3_F32
#pragma weak mlib_SignalFFT_3_F32C = __mlib_SignalFFT_3_F32C
#pragma weak mlib_SignalFFT_3_F32C_F32 = __mlib_SignalFFT_3_F32C_F32
#pragma weak mlib_SignalFFT_3_F32C_F32C = __mlib_SignalFFT_3_F32C_F32C
#pragma weak mlib_SignalFFT_3_F32_F32 = __mlib_SignalFFT_3_F32_F32
#pragma weak mlib_SignalIFFTW_1_F32 = __mlib_SignalIFFTW_1_F32
#pragma weak mlib_SignalIFFTW_1_F32C = __mlib_SignalIFFTW_1_F32C
#pragma weak mlib_SignalIFFTW_1_F32C_F32C = __mlib_SignalIFFTW_1_F32C_F32C
#pragma weak mlib_SignalIFFTW_1_F32_F32 = __mlib_SignalIFFTW_1_F32_F32
#pragma weak mlib_SignalIFFTW_1_F32_F32C = __mlib_SignalIFFTW_1_F32_F32C
#pragma weak mlib_SignalIFFTW_2_F32 = __mlib_SignalIFFTW_2_F32
#pragma weak mlib_SignalIFFTW_2_F32C = __mlib_SignalIFFTW_2_F32C
#pragma weak mlib_SignalIFFTW_2_F32C_F32C = __mlib_SignalIFFTW_2_F32C_F32C
#pragma weak mlib_SignalIFFTW_2_F32_F32 = __mlib_SignalIFFTW_2_F32_F32
#pragma weak mlib_SignalIFFTW_2_F32_F32C = __mlib_SignalIFFTW_2_F32_F32C
#pragma weak mlib_SignalIFFTW_3_F32 = __mlib_SignalIFFTW_3_F32
#pragma weak mlib_SignalIFFTW_3_F32C = __mlib_SignalIFFTW_3_F32C
#pragma weak mlib_SignalIFFTW_3_F32C_F32C = __mlib_SignalIFFTW_3_F32C_F32C
#pragma weak mlib_SignalIFFTW_3_F32_F32 = __mlib_SignalIFFTW_3_F32_F32
#pragma weak mlib_SignalIFFTW_3_F32_F32C = __mlib_SignalIFFTW_3_F32_F32C
#pragma weak mlib_SignalIFFT_1_F32 = __mlib_SignalIFFT_1_F32
#pragma weak mlib_SignalIFFT_1_F32C = __mlib_SignalIFFT_1_F32C
#pragma weak mlib_SignalIFFT_1_F32C_F32C = __mlib_SignalIFFT_1_F32C_F32C
#pragma weak mlib_SignalIFFT_1_F32_F32 = __mlib_SignalIFFT_1_F32_F32
#pragma weak mlib_SignalIFFT_1_F32_F32C = __mlib_SignalIFFT_1_F32_F32C
#pragma weak mlib_SignalIFFT_2_F32 = __mlib_SignalIFFT_2_F32
#pragma weak mlib_SignalIFFT_2_F32C = __mlib_SignalIFFT_2_F32C
#pragma weak mlib_SignalIFFT_2_F32C_F32C = __mlib_SignalIFFT_2_F32C_F32C
#pragma weak mlib_SignalIFFT_2_F32_F32 = __mlib_SignalIFFT_2_F32_F32
#pragma weak mlib_SignalIFFT_2_F32_F32C = __mlib_SignalIFFT_2_F32_F32C
#pragma weak mlib_SignalIFFT_3_F32 = __mlib_SignalIFFT_3_F32
#pragma weak mlib_SignalIFFT_3_F32C = __mlib_SignalIFFT_3_F32C
#pragma weak mlib_SignalIFFT_3_F32C_F32C = __mlib_SignalIFFT_3_F32C_F32C
#pragma weak mlib_SignalIFFT_3_F32_F32 = __mlib_SignalIFFT_3_F32_F32
#pragma weak mlib_SignalIFFT_3_F32_F32C = __mlib_SignalIFFT_3_F32_F32C

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalFFTW_1_F32) mlib_SignalFFTW_1_F32
    __attribute__((weak, alias("__mlib_SignalFFTW_1_F32")));
__typeof__(__mlib_SignalFFTW_1_F32C) mlib_SignalFFTW_1_F32C
    __attribute__((weak, alias("__mlib_SignalFFTW_1_F32C")));
__typeof__(__mlib_SignalFFTW_1_F32C_F32) mlib_SignalFFTW_1_F32C_F32
    __attribute__((weak, alias("__mlib_SignalFFTW_1_F32C_F32")));
__typeof__(__mlib_SignalFFTW_1_F32C_F32C) mlib_SignalFFTW_1_F32C_F32C
    __attribute__((weak, alias("__mlib_SignalFFTW_1_F32C_F32C")));
__typeof__(__mlib_SignalFFTW_1_F32_F32) mlib_SignalFFTW_1_F32_F32
    __attribute__((weak, alias("__mlib_SignalFFTW_1_F32_F32")));
__typeof__(__mlib_SignalFFTW_2_F32) mlib_SignalFFTW_2_F32
    __attribute__((weak, alias("__mlib_SignalFFTW_2_F32")));
__typeof__(__mlib_SignalFFTW_2_F32C) mlib_SignalFFTW_2_F32C
    __attribute__((weak, alias("__mlib_SignalFFTW_2_F32C")));
__typeof__(__mlib_SignalFFTW_2_F32C_F32) mlib_SignalFFTW_2_F32C_F32
    __attribute__((weak, alias("__mlib_SignalFFTW_2_F32C_F32")));
__typeof__(__mlib_SignalFFTW_2_F32C_F32C) mlib_SignalFFTW_2_F32C_F32C
    __attribute__((weak, alias("__mlib_SignalFFTW_2_F32C_F32C")));
__typeof__(__mlib_SignalFFTW_2_F32_F32) mlib_SignalFFTW_2_F32_F32
    __attribute__((weak, alias("__mlib_SignalFFTW_2_F32_F32")));
__typeof__(__mlib_SignalFFTW_3_F32) mlib_SignalFFTW_3_F32
    __attribute__((weak, alias("__mlib_SignalFFTW_3_F32")));
__typeof__(__mlib_SignalFFTW_3_F32C) mlib_SignalFFTW_3_F32C
    __attribute__((weak, alias("__mlib_SignalFFTW_3_F32C")));
__typeof__(__mlib_SignalFFTW_3_F32C_F32) mlib_SignalFFTW_3_F32C_F32
    __attribute__((weak, alias("__mlib_SignalFFTW_3_F32C_F32")));
__typeof__(__mlib_SignalFFTW_3_F32C_F32C) mlib_SignalFFTW_3_F32C_F32C
    __attribute__((weak, alias("__mlib_SignalFFTW_3_F32C_F32C")));
__typeof__(__mlib_SignalFFTW_3_F32_F32) mlib_SignalFFTW_3_F32_F32
    __attribute__((weak, alias("__mlib_SignalFFTW_3_F32_F32")));
__typeof__(__mlib_SignalFFT_1_F32) mlib_SignalFFT_1_F32
    __attribute__((weak, alias("__mlib_SignalFFT_1_F32")));
__typeof__(__mlib_SignalFFT_1_F32C) mlib_SignalFFT_1_F32C
    __attribute__((weak, alias("__mlib_SignalFFT_1_F32C")));
__typeof__(__mlib_SignalFFT_1_F32C_F32) mlib_SignalFFT_1_F32C_F32
    __attribute__((weak, alias("__mlib_SignalFFT_1_F32C_F32")));
__typeof__(__mlib_SignalFFT_1_F32C_F32C) mlib_SignalFFT_1_F32C_F32C
    __attribute__((weak, alias("__mlib_SignalFFT_1_F32C_F32C")));
__typeof__(__mlib_SignalFFT_1_F32_F32) mlib_SignalFFT_1_F32_F32
    __attribute__((weak, alias("__mlib_SignalFFT_1_F32_F32")));
__typeof__(__mlib_SignalFFT_2_F32) mlib_SignalFFT_2_F32
    __attribute__((weak, alias("__mlib_SignalFFT_2_F32")));
__typeof__(__mlib_SignalFFT_2_F32C) mlib_SignalFFT_2_F32C
    __attribute__((weak, alias("__mlib_SignalFFT_2_F32C")));
__typeof__(__mlib_SignalFFT_2_F32C_F32) mlib_SignalFFT_2_F32C_F32
    __attribute__((weak, alias("__mlib_SignalFFT_2_F32C_F32")));
__typeof__(__mlib_SignalFFT_2_F32C_F32C) mlib_SignalFFT_2_F32C_F32C
    __attribute__((weak, alias("__mlib_SignalFFT_2_F32C_F32C")));
__typeof__(__mlib_SignalFFT_2_F32_F32) mlib_SignalFFT_2_F32_F32
    __attribute__((weak, alias("__mlib_SignalFFT_2_F32_F32")));
__typeof__(__mlib_SignalFFT_3_F32) mlib_SignalFFT_3_F32
    __attribute__((weak, alias("__mlib_SignalFFT_3_F32")));
__typeof__(__mlib_SignalFFT_3_F32C) mlib_SignalFFT_3_F32C
    __attribute__((weak, alias("__mlib_SignalFFT_3_F32C")));
__typeof__(__mlib_SignalFFT_3_F32C_F32) mlib_SignalFFT_3_F32C_F32
    __attribute__((weak, alias("__mlib_SignalFFT_3_F32C_F32")));
__typeof__(__mlib_SignalFFT_3_F32C_F32C) mlib_SignalFFT_3_F32C_F32C
    __attribute__((weak, alias("__mlib_SignalFFT_3_F32C_F32C")));
__typeof__(__mlib_SignalFFT_3_F32_F32) mlib_SignalFFT_3_F32_F32
    __attribute__((weak, alias("__mlib_SignalFFT_3_F32_F32")));
__typeof__(__mlib_SignalIFFTW_1_F32) mlib_SignalIFFTW_1_F32
    __attribute__((weak, alias("__mlib_SignalIFFTW_1_F32")));
__typeof__(__mlib_SignalIFFTW_1_F32C) mlib_SignalIFFTW_1_F32C
    __attribute__((weak, alias("__mlib_SignalIFFTW_1_F32C")));
__typeof__(__mlib_SignalIFFTW_1_F32C_F32C) mlib_SignalIFFTW_1_F32C_F32C
    __attribute__((weak, alias("__mlib_SignalIFFTW_1_F32C_F32C")));
__typeof__(__mlib_SignalIFFTW_1_F32_F32) mlib_SignalIFFTW_1_F32_F32
    __attribute__((weak, alias("__mlib_SignalIFFTW_1_F32_F32")));
__typeof__(__mlib_SignalIFFTW_1_F32_F32C) mlib_SignalIFFTW_1_F32_F32C
    __attribute__((weak, alias("__mlib_SignalIFFTW_1_F32_F32C")));
__typeof__(__mlib_SignalIFFTW_2_F32) mlib_SignalIFFTW_2_F32
    __attribute__((weak, alias("__mlib_SignalIFFTW_2_F32")));
__typeof__(__mlib_SignalIFFTW_2_F32C) mlib_SignalIFFTW_2_F32C
    __attribute__((weak, alias("__mlib_SignalIFFTW_2_F32C")));
__typeof__(__mlib_SignalIFFTW_2_F32C_F32C) mlib_SignalIFFTW_2_F32C_F32C
    __attribute__((weak, alias("__mlib_SignalIFFTW_2_F32C_F32C")));
__typeof__(__mlib_SignalIFFTW_2_F32_F32) mlib_SignalIFFTW_2_F32_F32
    __attribute__((weak, alias("__mlib_SignalIFFTW_2_F32_F32")));
__typeof__(__mlib_SignalIFFTW_2_F32_F32C) mlib_SignalIFFTW_2_F32_F32C
    __attribute__((weak, alias("__mlib_SignalIFFTW_2_F32_F32C")));
__typeof__(__mlib_SignalIFFTW_3_F32) mlib_SignalIFFTW_3_F32
    __attribute__((weak, alias("__mlib_SignalIFFTW_3_F32")));
__typeof__(__mlib_SignalIFFTW_3_F32C) mlib_SignalIFFTW_3_F32C
    __attribute__((weak, alias("__mlib_SignalIFFTW_3_F32C")));
__typeof__(__mlib_SignalIFFTW_3_F32C_F32C) mlib_SignalIFFTW_3_F32C_F32C
    __attribute__((weak, alias("__mlib_SignalIFFTW_3_F32C_F32C")));
__typeof__(__mlib_SignalIFFTW_3_F32_F32) mlib_SignalIFFTW_3_F32_F32
    __attribute__((weak, alias("__mlib_SignalIFFTW_3_F32_F32")));
__typeof__(__mlib_SignalIFFTW_3_F32_F32C) mlib_SignalIFFTW_3_F32_F32C
    __attribute__((weak, alias("__mlib_SignalIFFTW_3_F32_F32C")));
__typeof__(__mlib_SignalIFFT_1_F32) mlib_SignalIFFT_1_F32
    __attribute__((weak, alias("__mlib_SignalIFFT_1_F32")));
__typeof__(__mlib_SignalIFFT_1_F32C) mlib_SignalIFFT_1_F32C
    __attribute__((weak, alias("__mlib_SignalIFFT_1_F32C")));
__typeof__(__mlib_SignalIFFT_1_F32C_F32C) mlib_SignalIFFT_1_F32C_F32C
    __attribute__((weak, alias("__mlib_SignalIFFT_1_F32C_F32C")));
__typeof__(__mlib_SignalIFFT_1_F32_F32) mlib_SignalIFFT_1_F32_F32
    __attribute__((weak, alias("__mlib_SignalIFFT_1_F32_F32")));
__typeof__(__mlib_SignalIFFT_1_F32_F32C) mlib_SignalIFFT_1_F32_F32C
    __attribute__((weak, alias("__mlib_SignalIFFT_1_F32_F32C")));
__typeof__(__mlib_SignalIFFT_2_F32) mlib_SignalIFFT_2_F32
    __attribute__((weak, alias("__mlib_SignalIFFT_2_F32")));
__typeof__(__mlib_SignalIFFT_2_F32C) mlib_SignalIFFT_2_F32C
    __attribute__((weak, alias("__mlib_SignalIFFT_2_F32C")));
__typeof__(__mlib_SignalIFFT_2_F32C_F32C) mlib_SignalIFFT_2_F32C_F32C
    __attribute__((weak, alias("__mlib_SignalIFFT_2_F32C_F32C")));
__typeof__(__mlib_SignalIFFT_2_F32_F32) mlib_SignalIFFT_2_F32_F32
    __attribute__((weak, alias("__mlib_SignalIFFT_2_F32_F32")));
__typeof__(__mlib_SignalIFFT_2_F32_F32C) mlib_SignalIFFT_2_F32_F32C
    __attribute__((weak, alias("__mlib_SignalIFFT_2_F32_F32C")));
__typeof__(__mlib_SignalIFFT_3_F32) mlib_SignalIFFT_3_F32
    __attribute__((weak, alias("__mlib_SignalIFFT_3_F32")));
__typeof__(__mlib_SignalIFFT_3_F32C) mlib_SignalIFFT_3_F32C
    __attribute__((weak, alias("__mlib_SignalIFFT_3_F32C")));
__typeof__(__mlib_SignalIFFT_3_F32C_F32C) mlib_SignalIFFT_3_F32C_F32C
    __attribute__((weak, alias("__mlib_SignalIFFT_3_F32C_F32C")));
__typeof__(__mlib_SignalIFFT_3_F32_F32) mlib_SignalIFFT_3_F32_F32
    __attribute__((weak, alias("__mlib_SignalIFFT_3_F32_F32")));
__typeof__(__mlib_SignalIFFT_3_F32_F32C) mlib_SignalIFFT_3_F32_F32C
    __attribute__((weak, alias("__mlib_SignalIFFT_3_F32_F32C")));

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
__mlib_SignalFFT_1_F32C_F32C(
    mlib_f32 *dstc,
    const mlib_f32 *srcc,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcc);
	return mlib_fftDispatcher_F32_1(dstc, NULL, (void *)srcc, NULL, order,
	    eSrcCmplx | eDstCmplx);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_1_F32_F32(
    mlib_f32 *dstr,
    mlib_f32 *dsti,
    const mlib_f32 *srcr,
    const mlib_f32 *srci,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstr);
	SERVE_BAD_POINTER(dsti);
	SERVE_BAD_POINTER(srcr);
	SERVE_BAD_POINTER(srci);
	return mlib_fftDispatcher_F32_1(dstr, dsti, (void *)srcr, (void *)srci,
	    order, eSrcReIm | eDstReIm);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_1_F32C_F32(
    mlib_f32 *dstc,
    const mlib_f32 *srcr,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcr);
	return mlib_fftDispatcherW_F32C_F32(dstc, NULL, (void *)srcr, NULL,
	    NULL, order, order, eSrcRe | eDstCmplx);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_1_F32C_F32C(
    mlib_f32 *dstc,
    const mlib_f32 *srcc,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcc);
	return mlib_fftDispatcher_F32_1(dstc, NULL, (void *)srcc, NULL, order,
	    eSrcCmplx | eDstCmplx | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_1_F32_F32(
    mlib_f32 *dstr,
    mlib_f32 *dsti,
    const mlib_f32 *srcr,
    const mlib_f32 *srci,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstr);
	SERVE_BAD_POINTER(dsti);
	SERVE_BAD_POINTER(srcr);
	SERVE_BAD_POINTER(srci);
	return mlib_fftDispatcher_F32_1(dstr, dsti, (void *)srcr, (void *)srci,
	    order, eSrcReIm | eDstReIm | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_1_F32_F32C(
    mlib_f32 *dstr,
    const mlib_f32 *srcc,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(srcc);
	SERVE_BAD_POINTER(dstr);
	return mlib_fftDispatcher_F32_1(dstr, NULL, (void *)srcc, NULL, order,
	    eSrcCmplx | eDstRe | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_1_F32C(
    mlib_f32 *datac,
    mlib_s32 order)
{
	return (__mlib_SignalFFT_1_F32C_F32C(datac, datac, order));
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_1_F32(
    mlib_f32 *datar,
    mlib_f32 *datai,
    mlib_s32 order)
{
	return (__mlib_SignalFFT_1_F32_F32(datar, datai, datar, datai, order));
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_1_F32C(
    mlib_f32 *datac,
    mlib_s32 order)
{
	return (__mlib_SignalIFFT_1_F32C_F32C(datac, datac, order));
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_1_F32(
    mlib_f32 *datar,
    mlib_f32 *datai,
    mlib_s32 order)
{
	return (__mlib_SignalIFFT_1_F32_F32(datar, datai, datar, datai, order));
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFTW_1_F32C_F32C(
    mlib_f32 *dstc,
    const mlib_f32 *srcc,
    const mlib_f32 *window,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcc);
	SERVE_BAD_POINTER(window);
	return mlib_fftDispatcherW_F32_1(dstc, NULL, (void *)srcc, NULL,
	    (void *)window, order, eSrcCmplx | eDstCmplx);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFTW_1_F32_F32(
    mlib_f32 *dstr,
    mlib_f32 *dsti,
    const mlib_f32 *srcr,
    const mlib_f32 *srci,
    const mlib_f32 *window,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstr);
	SERVE_BAD_POINTER(dsti);
	SERVE_BAD_POINTER(srcr);
	SERVE_BAD_POINTER(srci);
	SERVE_BAD_POINTER(window);
	return mlib_fftDispatcherW_F32_1(dstr, dsti, (void *)srcr, (void *)srci,
	    (void *)window, order, eSrcReIm | eDstReIm);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFTW_1_F32C_F32(
    mlib_f32 *dstc,
    const mlib_f32 *srcr,
    const mlib_f32 *window,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcr);
	SERVE_BAD_POINTER(window);
	return mlib_fftDispatcherW_F32C_F32(dstc, NULL, (void *)srcr, NULL,
	    (void *)window, order, order, eSrcRe | eDstCmplx);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFTW_1_F32C_F32C(
    mlib_f32 *dstc,
    const mlib_f32 *srcc,
    const mlib_f32 *window,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcc);
	SERVE_BAD_POINTER(window);
	return mlib_fftDispatcherW_F32_1(dstc, NULL, (void *)srcc, NULL,
	    (void *)window, order, eSrcCmplx | eDstCmplx | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFTW_1_F32_F32(
    mlib_f32 *dstr,
    mlib_f32 *dsti,
    const mlib_f32 *srcr,
    const mlib_f32 *srci,
    const mlib_f32 *window,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstr);
	SERVE_BAD_POINTER(dsti);
	SERVE_BAD_POINTER(srcr);
	SERVE_BAD_POINTER(srci);
	SERVE_BAD_POINTER(window);
	return mlib_fftDispatcherW_F32_1(dstr, dsti, (void *)srcr, (void *)srci,
	    (void *)window, order, eSrcReIm | eDstReIm | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFTW_1_F32_F32C(
    mlib_f32 *dstr,
    const mlib_f32 *srcc,
    const mlib_f32 *window,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(srcc);
	SERVE_BAD_POINTER(dstr);
	SERVE_BAD_POINTER(window);
	return mlib_fftDispatcherW_F32_1(dstr, NULL, (void *)srcc, NULL,
	    (void *)window, order, eSrcCmplx | eDstRe | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFTW_1_F32C(
    mlib_f32 *datac,
    const mlib_f32 *window,
    mlib_s32 order)
{
	return (__mlib_SignalFFTW_1_F32C_F32C(datac, datac, window, order));
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFTW_1_F32(
    mlib_f32 *datar,
    mlib_f32 *datai,
    const mlib_f32 *window,
    mlib_s32 order)
{
	return __mlib_SignalFFTW_1_F32_F32(datar, datai, datar, datai, window,
	    order);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFTW_1_F32C(
    mlib_f32 *datac,
    const mlib_f32 *window,
    mlib_s32 order)
{
	return (__mlib_SignalIFFTW_1_F32C_F32C(datac, datac, window, order));
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFTW_1_F32(
    mlib_f32 *datar,
    mlib_f32 *datai,
    const mlib_f32 *window,
    mlib_s32 order)
{
	return __mlib_SignalIFFTW_1_F32_F32(datar, datai, datar, datai, window,
	    order);
}

/* *********************************************************** */

/* functions codes */

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_2_F32C_F32C(
    mlib_f32 *dstc,
    const mlib_f32 *srcc,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcc);
	return mlib_fftDispatcher_F32_2(dstc, NULL, (void *)srcc, NULL, order,
	    eSrcCmplx | eDstCmplx);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_2_F32_F32(
    mlib_f32 *dstr,
    mlib_f32 *dsti,
    const mlib_f32 *srcr,
    const mlib_f32 *srci,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstr);
	SERVE_BAD_POINTER(dsti);
	SERVE_BAD_POINTER(srcr);
	SERVE_BAD_POINTER(srci);
	return mlib_fftDispatcher_F32_2(dstr, dsti, (void *)srcr, (void *)srci,
	    order, eSrcReIm | eDstReIm);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_2_F32C_F32(
    mlib_f32 *dstc,
    const mlib_f32 *srcr,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcr);
	return mlib_fftDispatcherW_F32C_F32(dstc, NULL, (void *)srcr, NULL,
	    NULL, order, 0, eSrcRe | eDstCmplx);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_2_F32C_F32C(
    mlib_f32 *dstc,
    const mlib_f32 *srcc,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcc);
	return mlib_fftDispatcher_F32_2(dstc, NULL, (void *)srcc, NULL, order,
	    eSrcCmplx | eDstCmplx | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_2_F32_F32(
    mlib_f32 *dstr,
    mlib_f32 *dsti,
    const mlib_f32 *srcr,
    const mlib_f32 *srci,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstr);
	SERVE_BAD_POINTER(dsti);
	SERVE_BAD_POINTER(srcr);
	SERVE_BAD_POINTER(srci);
	return mlib_fftDispatcher_F32_2(dstr, dsti, (void *)srcr, (void *)srci,
	    order, eSrcReIm | eDstReIm | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_2_F32_F32C(
    mlib_f32 *dstr,
    const mlib_f32 *srcc,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(srcc);
	SERVE_BAD_POINTER(dstr);
	return mlib_fftDispatcher_F32_2(dstr, NULL, (void *)srcc, NULL, order,
	    eSrcCmplx | eDstRe | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_2_F32C(
    mlib_f32 *datac,
    mlib_s32 order)
{
	return (__mlib_SignalFFT_2_F32C_F32C(datac, datac, order));
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_2_F32(
    mlib_f32 *datar,
    mlib_f32 *datai,
    mlib_s32 order)
{
	return (__mlib_SignalFFT_2_F32_F32(datar, datai, datar, datai, order));
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_2_F32C(
    mlib_f32 *datac,
    mlib_s32 order)
{
	return (__mlib_SignalIFFT_2_F32C_F32C(datac, datac, order));
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_2_F32(
    mlib_f32 *datar,
    mlib_f32 *datai,
    mlib_s32 order)
{
	return (__mlib_SignalIFFT_2_F32_F32(datar, datai, datar, datai, order));
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFTW_2_F32C_F32C(
    mlib_f32 *dstc,
    const mlib_f32 *srcc,
    const mlib_f32 *window,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcc);
	SERVE_BAD_POINTER(window);
	return mlib_fftDispatcherW_F32_2(dstc, NULL, (void *)srcc, NULL,
	    (void *)window, order, eSrcCmplx | eDstCmplx);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFTW_2_F32_F32(
    mlib_f32 *dstr,
    mlib_f32 *dsti,
    const mlib_f32 *srcr,
    const mlib_f32 *srci,
    const mlib_f32 *window,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstr);
	SERVE_BAD_POINTER(dsti);
	SERVE_BAD_POINTER(srcr);
	SERVE_BAD_POINTER(srci);
	SERVE_BAD_POINTER(window);
	return mlib_fftDispatcherW_F32_2(dstr, dsti, (void *)srcr, (void *)srci,
	    (void *)window, order, eSrcReIm | eDstReIm);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFTW_2_F32C_F32(
    mlib_f32 *dstc,
    const mlib_f32 *srcr,
    const mlib_f32 *window,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcr);
	SERVE_BAD_POINTER(window);
	return mlib_fftDispatcherW_F32C_F32(dstc, NULL, (void *)srcr, NULL,
	    (void *)window, order, 0, eSrcRe | eDstCmplx);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFTW_2_F32C_F32C(
    mlib_f32 *dstc,
    const mlib_f32 *srcc,
    const mlib_f32 *window,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcc);
	SERVE_BAD_POINTER(window);
	return mlib_fftDispatcherW_F32_2(dstc, NULL, (void *)srcc, NULL,
	    (void *)window, order, eSrcCmplx | eDstCmplx | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFTW_2_F32_F32(
    mlib_f32 *dstr,
    mlib_f32 *dsti,
    const mlib_f32 *srcr,
    const mlib_f32 *srci,
    const mlib_f32 *window,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstr);
	SERVE_BAD_POINTER(dsti);
	SERVE_BAD_POINTER(srcr);
	SERVE_BAD_POINTER(srci);
	SERVE_BAD_POINTER(window);
	return mlib_fftDispatcherW_F32_2(dstr, dsti, (void *)srcr, (void *)srci,
	    (void *)window, order, eSrcReIm | eDstReIm | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFTW_2_F32_F32C(
    mlib_f32 *dstr,
    const mlib_f32 *srcc,
    const mlib_f32 *window,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(srcc);
	SERVE_BAD_POINTER(dstr);
	SERVE_BAD_POINTER(window);
	return mlib_fftDispatcherW_F32_2(dstr, NULL, (void *)srcc, NULL,
	    (void *)window, order, eSrcCmplx | eDstRe | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFTW_2_F32C(
    mlib_f32 *datac,
    const mlib_f32 *window,
    mlib_s32 order)
{
	return (__mlib_SignalFFTW_2_F32C_F32C(datac, datac, window, order));
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFTW_2_F32(
    mlib_f32 *datar,
    mlib_f32 *datai,
    const mlib_f32 *window,
    mlib_s32 order)
{
	return __mlib_SignalFFTW_2_F32_F32(datar, datai, datar, datai, window,
	    order);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFTW_2_F32C(
    mlib_f32 *datac,
    const mlib_f32 *window,
    mlib_s32 order)
{
	return (__mlib_SignalIFFTW_2_F32C_F32C(datac, datac, window, order));
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFTW_2_F32(
    mlib_f32 *datar,
    mlib_f32 *datai,
    const mlib_f32 *window,
    mlib_s32 order)
{
	return __mlib_SignalIFFTW_2_F32_F32(datar, datai, datar, datai, window,
	    order);
}

/* *********************************************************** */

/* functions codes */

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_3_F32C_F32C(
    mlib_f32 *dstc,
    const mlib_f32 *srcc,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcc);
	return mlib_fftDispatcher_F32_3(dstc, NULL, (void *)srcc, NULL, order,
	    eSrcCmplx | eDstCmplx);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_3_F32_F32(
    mlib_f32 *dstr,
    mlib_f32 *dsti,
    const mlib_f32 *srcr,
    const mlib_f32 *srci,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstr);
	SERVE_BAD_POINTER(dsti);
	SERVE_BAD_POINTER(srcr);
	SERVE_BAD_POINTER(srci);
	return mlib_fftDispatcher_F32_3(dstr, dsti, (void *)srcr, (void *)srci,
	    order, eSrcReIm | eDstReIm);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_3_F32C_F32(
    mlib_f32 *dstc,
    const mlib_f32 *srcr,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcr);
	return mlib_fftDispatcherW_F32C_F32(dstc, NULL, (void *)srcr, NULL,
	    NULL, order, order / 2, eSrcRe | eDstCmplx);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_3_F32C_F32C(
    mlib_f32 *dstc,
    const mlib_f32 *srcc,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcc);
	return mlib_fftDispatcher_F32_3(dstc, NULL, (void *)srcc, NULL, order,
	    eSrcCmplx | eDstCmplx | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_3_F32_F32(
    mlib_f32 *dstr,
    mlib_f32 *dsti,
    const mlib_f32 *srcr,
    const mlib_f32 *srci,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstr);
	SERVE_BAD_POINTER(dsti);
	SERVE_BAD_POINTER(srcr);
	SERVE_BAD_POINTER(srci);
	return mlib_fftDispatcher_F32_3(dstr, dsti, (void *)srcr, (void *)srci,
	    order, eSrcReIm | eDstReIm | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_3_F32_F32C(
    mlib_f32 *dstr,
    const mlib_f32 *srcc,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(srcc);
	SERVE_BAD_POINTER(dstr);
	return mlib_fftDispatcher_F32_3(dstr, NULL, (void *)srcc, NULL, order,
	    eSrcCmplx | eDstRe | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_3_F32C(
    mlib_f32 *datac,
    mlib_s32 order)
{
	return (__mlib_SignalFFT_3_F32C_F32C(datac, datac, order));
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFT_3_F32(
    mlib_f32 *datar,
    mlib_f32 *datai,
    mlib_s32 order)
{
	return (__mlib_SignalFFT_3_F32_F32(datar, datai, datar, datai, order));
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_3_F32C(
    mlib_f32 *datac,
    mlib_s32 order)
{
	return (__mlib_SignalIFFT_3_F32C_F32C(datac, datac, order));
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFT_3_F32(
    mlib_f32 *datar,
    mlib_f32 *datai,
    mlib_s32 order)
{
	return (__mlib_SignalIFFT_3_F32_F32(datar, datai, datar, datai, order));
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFTW_3_F32C_F32C(
    mlib_f32 *dstc,
    const mlib_f32 *srcc,
    const mlib_f32 *window,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcc);
	SERVE_BAD_POINTER(window);
	return mlib_fftDispatcherW_F32_3(dstc, NULL, (void *)srcc, NULL,
	    (void *)window, order, eSrcCmplx | eDstCmplx);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFTW_3_F32_F32(
    mlib_f32 *dstr,
    mlib_f32 *dsti,
    const mlib_f32 *srcr,
    const mlib_f32 *srci,
    const mlib_f32 *window,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstr);
	SERVE_BAD_POINTER(dsti);
	SERVE_BAD_POINTER(srcr);
	SERVE_BAD_POINTER(srci);
	SERVE_BAD_POINTER(window);
	return mlib_fftDispatcherW_F32_3(dstr, dsti, (void *)srcr, (void *)srci,
	    (void *)window, order, eSrcReIm | eDstReIm);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFTW_3_F32C_F32(
    mlib_f32 *dstc,
    const mlib_f32 *srcr,
    const mlib_f32 *window,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcr);
	SERVE_BAD_POINTER(window);
	return mlib_fftDispatcherW_F32C_F32(dstc, NULL, (void *)srcr, NULL,
	    (void *)window, order, order / 2, eSrcRe | eDstCmplx);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFTW_3_F32C_F32C(
    mlib_f32 *dstc,
    const mlib_f32 *srcc,
    const mlib_f32 *window,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstc);
	SERVE_BAD_POINTER(srcc);
	SERVE_BAD_POINTER(window);
	return mlib_fftDispatcherW_F32_3(dstc, NULL, (void *)srcc, NULL,
	    (void *)window, order, eSrcCmplx | eDstCmplx | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFTW_3_F32_F32(
    mlib_f32 *dstr,
    mlib_f32 *dsti,
    const mlib_f32 *srcr,
    const mlib_f32 *srci,
    const mlib_f32 *window,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(dstr);
	SERVE_BAD_POINTER(dsti);
	SERVE_BAD_POINTER(srcr);
	SERVE_BAD_POINTER(srci);
	SERVE_BAD_POINTER(window);
	return mlib_fftDispatcherW_F32_3(dstr, dsti, (void *)srcr, (void *)srci,
	    (void *)window, order, eSrcReIm | eDstReIm | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFTW_3_F32_F32C(
    mlib_f32 *dstr,
    const mlib_f32 *srcc,
    const mlib_f32 *window,
    mlib_s32 order)
{
	SERVE_BAD_POINTER(srcc);
	SERVE_BAD_POINTER(dstr);
	SERVE_BAD_POINTER(window);
	return mlib_fftDispatcherW_F32_3(dstr, NULL, (void *)srcc, NULL,
	    (void *)window, order, eSrcCmplx | eDstRe | eInversFFT);
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFTW_3_F32C(
    mlib_f32 *datac,
    const mlib_f32 *window,
    mlib_s32 order)
{
	return (__mlib_SignalFFTW_3_F32C_F32C(datac, datac, window, order));
}

/* *********************************************************** */

mlib_status
__mlib_SignalFFTW_3_F32(
    mlib_f32 *datar,
    mlib_f32 *datai,
    const mlib_f32 *window,
    mlib_s32 order)
{
	return __mlib_SignalFFTW_3_F32_F32(datar, datai, datar, datai, window,
	    order);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFTW_3_F32C(
    mlib_f32 *datac,
    const mlib_f32 *window,
    mlib_s32 order)
{
	return (__mlib_SignalIFFTW_3_F32C_F32C(datac, datac, window, order));
}

/* *********************************************************** */

mlib_status
__mlib_SignalIFFTW_3_F32(
    mlib_f32 *datar,
    mlib_f32 *datai,
    const mlib_f32 *window,
    mlib_s32 order)
{
	return __mlib_SignalIFFTW_3_F32_F32(datar, datai, datar, datai, window,
	    order);
}

/* *********************************************************** */
