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

#ifndef	_MLIB_S_SIGNALFFT_3_MACROS_H
#define	_MLIB_S_SIGNALFFT_3_MACROS_H

#pragma ident	"@(#)mlib_s_SignalFFT_3_Macros.h	9.6	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_s_SignalFFT_x_Macros.h>

/* *********************************************************** */
/*
 * Mask out those macros defined in mlib_s_SignalFFT_x_Macros.h
 * but redefined in this file
 *
 * This file is only included by mlib_s_SignalFFT_3.c and
 * mlib_s_SignalFFTW_3.c
 */
/* *********************************************************** */

#undef	DCL_COMMON
#undef	PROC_FIRST_INV
#undef  LOAD_ROT_DIR
#undef  LOAD_ROT_INV
#undef  PROC_LAST_DIR

/* *********************************************************** */

/* 4th order macros */

/* *********************************************************** */

#define	PROC_FIRST_INV                                            \
	{                                                         \
	    __m128i r0a1, r0s1, r2a3, r2s3;                       \
	    __m128i i0a1, i0s1, i2a3, i2s3;                       \
	                                                          \
	    r0a1 = _mm_add_epi16(real0, real2);                   \
	    r2a3 = _mm_add_epi16(real1, real3);                   \
	    r0s1 = _mm_sub_epi16(real0, real2);                   \
	    r2s3 = _mm_sub_epi16(real1, real3);                   \
	    ar = _mm_add_epi16(r0a1, r2a3);                       \
	    cr = _mm_sub_epi16(r0a1, r2a3);                       \
	    i0a1 = _mm_add_epi16(imag0, imag2);                   \
	    i2a3 = _mm_add_epi16(imag1, imag3);                   \
	    i0s1 = _mm_sub_epi16(imag0, imag2);                   \
	    i2s3 = _mm_sub_epi16(imag1, imag3);                   \
	    ai = _mm_add_epi16(i0a1, i2a3);                       \
	    ci = _mm_sub_epi16(i0a1, i2a3);                       \
	    bi = _mm_add_epi16(i0s1, r2s3);                       \
	    di = _mm_sub_epi16(i0s1, r2s3);                       \
	    br = _mm_sub_epi16(r0s1, i2s3);                       \
	    dr = _mm_add_epi16(r0s1, i2s3);                       \
	}

/* *********************************************************** */

#define	LOAD_ROT                                                               \
	rot3r = _mm_loadl_epi64((__m128i *)((mlib_d64 *)mlib_fft_rot + 1));    \
	rot3i = _mm_loadl_epi64((__m128i *)((mlib_d64 *)mlib_fft_rot + 2));    \
	rotc0r = _mm_loadl_epi64((__m128i *)((mlib_d64 *)mlib_fft_rot + 3));   \
	rotc1r = _mm_loadl_epi64((__m128i *)((mlib_d64 *)mlib_fft_rot + 4));   \
	rotd0r = _mm_loadl_epi64((__m128i *)((mlib_d64 *)mlib_fft_rot + 5));   \
	rotd1r = _mm_loadl_epi64((__m128i *)((mlib_d64 *)mlib_fft_rot + 6))

/* *********************************************************** */

#define	PROC_LAST_DIR                                             \
	{                                                         \
	    __m128i c0r, c0i, d0r, d0i, tmp;                      \
	                                                          \
	    ar = _mm_fmul8x16_vis(ar, divider);                   \
	    ai = _mm_fmul8x16_vis(ai, divider);                   \
	    real1 = MULT(rot3r, br);                              \
	    tmp = MULT(rot3i, bi);                                \
	    real1 = _mm_add_epi16(real1, tmp);                    \
	    imag1 = MULT(rot3r, bi);                              \
	    tmp = MULT(rot3i, br);                                \
	    imag1 = _mm_sub_epi16(imag1, tmp);                    \
	    real0 = _mm_add_epi16(ar, real1);                     \
	    real1 = _mm_sub_epi16(ar, real1);                     \
	    imag0 = _mm_add_epi16(ai, imag1);                     \
	    imag1 = _mm_sub_epi16(ai, imag1);                     \
	    c0r = MULT(rotc0r, cr);                               \
	    tmp = MULT(rotc1r, ci);                               \
	    c0r = _mm_sub_epi16(c0r, tmp);                        \
	    c0i = MULT(rotc1r, cr);                               \
	    tmp = MULT(rotc0r, ci);                               \
	    c0i = _mm_add_epi16(tmp, c0i);                        \
	    d0r = MULT(rotd0r, dr);                               \
	    tmp = MULT(rotd1r, di);                               \
	    d0r = _mm_sub_epi16(d0r, tmp);                        \
	    d0i = MULT(rotd1r, dr);                               \
	    tmp = MULT(rotd0r, di);                               \
	    d0i = _mm_add_epi16(tmp, d0i);                        \
	    real2 = _mm_add_epi16(c0r, d0r);                      \
	    imag2 = _mm_add_epi16(c0i, d0i);                      \
	    real3 = _mm_sub_epi16(c0i, d0i);                      \
	    imag3 = _mm_sub_epi16(d0r, c0r);                      \
	}

/* *********************************************************** */

#define	DCL_COMMON                                                \
	__m128i ar, br, cr, dr, ai, bi, ci, di;                   \
	mlib_s32 divider = 2;                                     \
	DEF_ADDER(divider);                                       \
	__m128i real0, real1, real2, real3;                       \
	__m128i imag0, imag1, imag2, imag3;                       \
	__m128i rot3r, rot3i, rotc0r, rotc1r, rotd0r, rotd1r

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif	/* _MLIB_S_SIGNALFFT_3_MACROS_H */
