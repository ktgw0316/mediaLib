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

#ifndef	_MLIB_S_SIGNALFFT_X_MACROS_H
#define	_MLIB_S_SIGNALFFT_X_MACROS_H

#pragma ident	"@(#)mlib_s_SignalFFT_x_Macros.h	9.7	07/11/05 SMI"

/*
 * Macros used by the functions in mlib_s_SignalFFT_[1|2|3].c
 * and mlib_s_SignalFFTW_[1|2|3].c
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_s_sse_int.h>
#include <mlib_s_SignalFFT_Tbl.h>

/* *********************************************************** */

/* macros, typedefs */

#define	VIS_ALIGNADDR(PTR, OFF)	vis_alignaddr((void *)(PTR), OFF)

/* *********************************************************** */

/* 4th order macros */

/* *********************************************************** */

#define	TRANSPOSE4X4_REV(i0, i1, i2, i3, p0, p2, p1, p3)        \
	{                                                       \
	    __m128i i01, i23, d0, d1, d2, d3;                   \
	                                                        \
	    i01 = _mm_unpacklo_epi16(i0, i1);                   \
	    i23 = _mm_unpacklo_epi16(i2, i3);                   \
	    d1 = _mm_unpackhi_epi32(i01, i23);                  \
	    d0 = _mm_unpackhi_epi64(d1, d1);                    \
	    d3 = _mm_unpacklo_epi32(i01, i23);                  \
	    d2 = _mm_unpackhi_epi64(d3, d3);                    \
		                                                \
	    _mm_storel_epi64((__m128i *)(p0), d0);              \
	    _mm_storel_epi64((__m128i *)(p1), d1);              \
	    _mm_storel_epi64((__m128i *)(p2), d2);              \
	    _mm_storel_epi64((__m128i *)(p3), d3);              \
	}

#define	TRANSPOSE4X4_REV_NEW(i0, i1, i2, i3, p0, p2, p1, p3)    \
	{                                                       \
	    __m128i i01, i23, d0, d1, d2, d3;                   \
	                                                        \
	    i01 = _mm_unpacklo_epi16(i0, i1);                   \
	    i23 = _mm_unpacklo_epi16(i2, i3);                   \
	    d0 = _mm_unpacklo_epi32(i01, i23);                  \
	    d1 = _mm_unpackhi_epi64(d0, d0);                    \
	    d2 = _mm_unpackhi_epi32(i01, i23);                  \
	    d3 = _mm_unpackhi_epi64(d2, d2);                    \
		                                                \
	    p0 = d0;                                            \
	    p1 = d1;                                            \
	    p2 = d2;                                            \
	    p3 = d3;                                            \
	}

/* *********************************************************** */

#define	UNMIX_REIM(ri0, ri1, rout, iout)                              \
	{                                                             \
	    __m128i r0, r1, i0, i1;                                   \
	                                                              \
	    i0 = _mm_slli_epi32(ri0, 16);                             \
	    i1 = _mm_slli_epi32(ri1, 16);                             \
	    r0 = _mm_srai_epi32(ri0, 16);                             \
	    r1 = _mm_srai_epi32(ri1, 16);                             \
	    i0 = _mm_srai_epi32(i0, 16);                              \
	    i1 = _mm_srai_epi32(i1, 16);                              \
	    iout = _mm_packs_epi32(r1, r0);                           \
	    rout = _mm_packs_epi32(i1, i0);                           \
	    rout = _mm_unpackhi_epi32(rout, _mm_slli_si128(rout, 8)); \
	    iout = _mm_unpackhi_epi32(iout, _mm_slli_si128(iout, 8)); \
	}

#define	UNMIX_REIM_SSE2(ri0, ri1, rout, iout)                         \
	{                                                             \
	    __m128i r0, r1, i0, i1;                                   \
	                                                              \
	    i0 = _mm_slli_epi32(ri0, 16);                             \
	    i1 = _mm_slli_epi32(ri1, 16);                             \
	    r0 = _mm_srai_epi32(ri0, 16);                             \
	    r1 = _mm_srai_epi32(ri1, 16);                             \
	    i0 = _mm_srai_epi32(i0, 16);                              \
	    i1 = _mm_srai_epi32(i1, 16);                              \
	    rout = _mm_packs_epi32(r0, r1);                           \
	    iout = _mm_packs_epi32(i0, i1);                           \
	}

/* *********************************************************** */

#define	REMIX_REIM(r0, i0, ri0, ri1)                            \
	{                                                       \
	    ri0 = _mm_unpacklo_epi16(r0, i0);                   \
	    ri1 = _mm_unpackhi_epi64(ri0, ri0);                 \
	}

/* *********************************************************** */

#define	LOAD_REAL                                               \
	{                                                       \
	    mlib_d64 *real64;                                   \
	                                                        \
	    real64 = (mlib_d64 *)srcr;                          \
	    real0 = _mm_loadl_epi64((__m128i *)(real64));       \
	    real1 = _mm_loadl_epi64((__m128i *)(real64 + 1));   \
	    real2 = _mm_loadl_epi64((__m128i *)(real64 + 2));   \
	    real3 = _mm_loadl_epi64((__m128i *)(real64 + 3));   \
	}

/* *********************************************************** */

#define	LOAD_IMAG                                               \
	{                                                       \
	    mlib_d64 *imag64;                                   \
	                                                        \
	    imag64 = (mlib_d64 *)srci;                          \
	    imag0 = _mm_loadl_epi64((__m128i *)(imag64));       \
	    imag1 = _mm_loadl_epi64((__m128i *)(imag64 + 1));   \
	    imag2 = _mm_loadl_epi64((__m128i *)(imag64 + 2));   \
	    imag3 = _mm_loadl_epi64((__m128i *)(imag64 + 3));   \
	}

/* *********************************************************** */

#define	LOAD_REIM                                               \
	{                                                       \
	    mlib_d64 *reim64 = (mlib_d64 *)srcc;                \
	                                                        \
	    reim0 = _mm_loadl_epi64((__m128i *)reim64);         \
	    reim1 = _mm_loadl_epi64((__m128i *)(reim64 + 1));   \
	    reim2 = _mm_loadl_epi64((__m128i *)(reim64 + 2));   \
	    reim3 = _mm_loadl_epi64((__m128i *)(reim64 + 3));   \
	    reim4 = _mm_loadl_epi64((__m128i *)(reim64 + 4));   \
	    reim5 = _mm_loadl_epi64((__m128i *)(reim64 + 5));   \
	    reim6 = _mm_loadl_epi64((__m128i *)(reim64 + 6));   \
	    reim7 = _mm_loadl_epi64((__m128i *)(reim64 + 7));   \
	    UNMIX_REIM(reim0, reim1, real0, imag0)              \
	    UNMIX_REIM(reim2, reim3, real1, imag1)              \
	    UNMIX_REIM(reim4, reim5, real2, imag2)              \
	    UNMIX_REIM(reim6, reim7, real3, imag3)              \
	}

#define	LOAD_REIM_SSE2                                          \
	{                                                       \
		/* reminder: can be improved */                 \
	    __m128i *reim128 = (__m128i *)srcc;                 \
	                                                        \
	    reim0 = _mm_loadu_si128(reim128);                   \
	    reim1 = _mm_loadu_si128(reim128 + 1);               \
	    reim2 = _mm_loadu_si128(reim128 + 2);               \
	    reim3 = _mm_loadu_si128(reim128 + 3);               \
	    UNMIX_REIM_SSE2(reim0, reim1, real0, imag0)         \
	    UNMIX_REIM_SSE2(reim2, reim3, real2, imag2)         \
	}

/* *********************************************************** */

#define	MULT(d0, d1)                                            \
	_mm_mulhi_epi16_vis(d0, d1)

/* *********************************************************** */

#define	PROC_ONLYREAL_FIRST_DIR                                 \
	{                                                       \
	    __m128i r0a1, r0s1, r2a3, r2s3;                     \
	                                                        \
	    r0a1 = _mm_add_epi16(real0, real2);                 \
	    r2a3 = _mm_add_epi16(real1, real3);                 \
	    r0s1 = _mm_sub_epi16(real0, real2);                 \
	    r2s3 = _mm_sub_epi16(real1, real3);                 \
	    ar = _mm_add_epi16(r0a1, r2a3);                     \
	    cr = _mm_sub_epi16(r0a1, r2a3);                     \
	    br = dr = r0s1;                                     \
	    ai = ci = _mm_setzero_si128();                      \
	    bi = _mm_sub_epi16(ai, r2s3);                       \
	    di = r2s3;                                          \
	}

/* *********************************************************** */

#define	PROC_FIRST_DIR                                          \
	{                                                       \
	    __m128i r0a1, r0s1, r2a3, r2s3;                     \
	    __m128i i0a1, i0s1, i2a3, i2s3;                     \
	                                                        \
	    r0a1 = _mm_add_epi16(real0, real2);                 \
	    r2a3 = _mm_add_epi16(real1, real3);                 \
	    r0s1 = _mm_sub_epi16(real0, real2);                 \
	    r2s3 = _mm_sub_epi16(real1, real3);                 \
	    ar = _mm_add_epi16(r0a1, r2a3);                     \
	    cr = _mm_sub_epi16(r0a1, r2a3);                     \
	    i0a1 = _mm_add_epi16(imag0, imag2);                 \
	    i2a3 = _mm_add_epi16(imag1, imag3);                 \
	    i0s1 = _mm_sub_epi16(imag0, imag2);                 \
	    i2s3 = _mm_sub_epi16(imag1, imag3);                 \
	    ai = _mm_add_epi16(i0a1, i2a3);                     \
	    ci = _mm_sub_epi16(i0a1, i2a3);                     \
	    bi = _mm_sub_epi16(i0s1, r2s3);                     \
	    di = _mm_add_epi16(i0s1, r2s3);                     \
	    br = _mm_add_epi16(r0s1, i2s3);                     \
	    dr = _mm_sub_epi16(r0s1, i2s3);                     \
	}

/*
 * real0 = (real0, real1)
 * real2 = (real2, real3)
 * imag0 = (imag0, imag1)
 * imag2 = (imag2, imag3)
 * r0a1 = (r0a1, r2a3)
 * r0s1 = (r0s1, r2s3)
 * i0a1 = (i0a1, i2a3)
 * i0s1 = (i0s1, i2s3)
 * ar = (ar, ai)
 * cr = (cr, ci)
 * br = (br, di)
 * dr = (dr, bi)
 */
#define	PROC_FIRST_DIR_SSE2                                     \
	{                                                       \
	    __m128i r0a1, r0s1, d0, d1;                         \
	    __m128i i0a1, i0s1, d2, d3;                         \
	                                                        \
	    r0a1 = _mm_add_epi16(real0, real2);                 \
	    r0s1 = _mm_sub_epi16(real0, real2);                 \
	    i0a1 = _mm_add_epi16(imag0, imag2);                 \
	    i0s1 = _mm_sub_epi16(imag0, imag2);                 \
	    d0 = _mm_unpackhi_epi64(r0a1, i0a1);                \
	    d1 = _mm_unpacklo_epi64(r0a1, i0a1);                \
	    d2 = _mm_unpackhi_epi64(r0s1, i0s1);                \
	    d3 = _mm_unpacklo_epi64(i0s1, r0s1);                \
	    ar = _mm_add_epi16(d0, d1);                         \
	    cr = _mm_sub_epi16(d0, d1);                         \
	    br = _mm_add_epi16(d2, d3);                         \
	    dr = _mm_sub_epi16(d2, d3);                         \
	}

/* *********************************************************** */

#define	PROC_FIRST_INV                                          \
	{                                                       \
	    __m128i r0a1, r0s1, r2a3, r2s3;                     \
	    __m128i i0a1, i0s1, i2a3, i2s3;                     \
	                                                        \
	    real0 = _mm_fmul8x16_vis(real0, divider);           \
	    real1 = _mm_fmul8x16_vis(real1, divider);           \
	    real2 = _mm_fmul8x16_vis(real2, divider);           \
	    real3 = _mm_fmul8x16_vis(real3, divider);           \
	    r0a1 = _mm_add_epi16(real0, real2);                 \
	    r2a3 = _mm_add_epi16(real1, real3);                 \
	    r0s1 = _mm_sub_epi16(real0, real2);                 \
	    r2s3 = _mm_sub_epi16(real1, real3);                 \
	    ar = _mm_add_epi16(r0a1, r2a3);                     \
	    cr = _mm_sub_epi16(r0a1, r2a3);                     \
	    imag0 = _mm_fmul8x16_vis(imag0, divider);           \
	    imag1 = _mm_fmul8x16_vis(imag1, divider);           \
	    imag2 = _mm_fmul8x16_vis(imag2, divider);           \
	    imag3 = _mm_fmul8x16_vis(imag3, divider);           \
	    i0a1 = _mm_add_epi16(imag0, imag2);                 \
	    i2a3 = _mm_add_epi16(imag1, imag3);                 \
	    i0s1 = _mm_sub_epi16(imag0, imag2);                 \
	    i2s3 = _mm_sub_epi16(imag1, imag3);                 \
	    ai = _mm_add_epi16(i0a1, i2a3);                     \
	    ci = _mm_sub_epi16(i0a1, i2a3);                     \
	    bi = _mm_add_epi16(i0s1, r2s3);                     \
	    di = _mm_sub_epi16(i0s1, r2s3);                     \
	    br = _mm_sub_epi16(r0s1, i2s3);                     \
	    dr = _mm_add_epi16(r0s1, i2s3);                     \
	}

/* *********************************************************** */

#define	LOAD_ROT_DIR                                                           \
	rot3r = _mm_loadl_epi64((__m128i *)((mlib_d64 *)mlib_fft_rot + 7));    \
	rot3i = _mm_loadl_epi64((__m128i *)((mlib_d64 *)mlib_fft_rot + 8));    \
	rotc0r = _mm_loadl_epi64((__m128i *)((mlib_d64 *)mlib_fft_rot + 9));   \
	rotc1r =  _mm_loadl_epi64((__m128i *)((mlib_d64 *)mlib_fft_rot + 10)); \
	rotd0r = _mm_loadl_epi64((__m128i *)((mlib_d64 *)mlib_fft_rot + 11));  \
	rotd1r = _mm_loadl_epi64((__m128i *)((mlib_d64 *)mlib_fft_rot + 12))

/* *********************************************************** */

#define	LOAD_ROT_INV                                                           \
	rot3r = _mm_loadl_epi64((__m128i *)((mlib_d64 *)mlib_fft_rot + 1));    \
	rot3i = _mm_loadl_epi64((__m128i *)((mlib_d64 *)mlib_fft_rot + 2));    \
	rotc0r = _mm_loadl_epi64((__m128i *)((mlib_d64 *)mlib_fft_rot + 3));   \
	rotc1r = _mm_loadl_epi64((__m128i *)((mlib_d64 *)mlib_fft_rot + 4));   \
	rotd0r = _mm_loadl_epi64((__m128i *)((mlib_d64 *)mlib_fft_rot + 5));   \
	rotd1r = _mm_loadl_epi64((__m128i *)((mlib_d64 *)mlib_fft_rot + 6))

/* *********************************************************** */

#define	PROC_LAST_INV                                           \
	{                                                       \
	    __m128i c0r, c0i, d0r, d0i;                         \
	                                                        \
	    ar = _mm_fmul8x16_vis(ar, divider);                 \
	    ai = _mm_fmul8x16_vis(ai, divider);                 \
	    real1 = MULT(rot3r, br);                            \
	    tmp = MULT(rot3i, bi);                              \
	    real1 = _mm_sub_epi16(real1, tmp);                  \
	    imag1 = MULT(rot3r, bi);                            \
	    tmp = MULT(rot3i, br);                              \
	    imag1 = _mm_add_epi16(imag1, tmp);                  \
	    real0 = _mm_add_epi16(ar, real1);                   \
	    real1 = _mm_sub_epi16(ar, real1);                   \
	    imag0 = _mm_add_epi16(ai, imag1);                   \
	    imag1 = _mm_sub_epi16(ai, imag1);                   \
	    c0r = MULT(rotc0r, cr);                             \
	    tmp = MULT(rotc1r, ci);                             \
	    c0r = _mm_add_epi16(c0r, tmp);                      \
	    c0i = MULT(rotc1r, cr);                             \
	    tmp = MULT(rotc0r, ci);                             \
	    c0i = _mm_sub_epi16(tmp, c0i);                      \
	    d0r = MULT(rotd0r, dr);                             \
	    tmp = MULT(rotd1r, di);                             \
	    d0r = _mm_add_epi16(d0r, tmp);                      \
	    d0i = MULT(rotd1r, dr);                             \
	    tmp = MULT(rotd0r, di);                             \
	    d0i = _mm_sub_epi16(tmp, d0i);                      \
	    real2 = _mm_add_epi16(c0r, d0r);                    \
	    imag2 = _mm_add_epi16(c0i, d0i);                    \
	    real3 = _mm_sub_epi16(d0i, c0i);                    \
	    imag3 = _mm_sub_epi16(c0r, d0r);                    \
	}

/* *********************************************************** */

#define	PROC_LAST_END                                           \
	ar = _mm_add_epi16(real0, real2);                       \
	br = _mm_add_epi16(real1, real3);                       \
	cr = _mm_sub_epi16(real0, real2);                       \
	dr = _mm_sub_epi16(real1, real3);                       \
	ai = _mm_add_epi16(imag0, imag2);                       \
	bi = _mm_add_epi16(imag1, imag3);                       \
	ci = _mm_sub_epi16(imag0, imag2);                       \
	di = _mm_sub_epi16(imag1, imag3)

/* *********************************************************** */

#define	PROC_LAST_END_R                                         \
	ar = _mm_add_epi16(real0, real2);                       \
	br = _mm_add_epi16(real1, real3);                       \
	cr = _mm_sub_epi16(real0, real2);                       \
	dr = _mm_sub_epi16(real1, real3)

/* *********************************************************** */

#define	PROC_LAST_DIR                                           \
	{                                                       \
	    __m128i c0r, c0i, d0r, d0i, tmp;                    \
	                                                        \
	    br = _mm_add_epi16(br, br);                         \
	    bi = _mm_add_epi16(bi, bi);                         \
	    cr = _mm_add_epi16(cr, cr);                         \
	    ci = _mm_add_epi16(ci, ci);                         \
	    dr = _mm_add_epi16(dr, dr);                         \
	    di = _mm_add_epi16(di, di);                         \
	    real1 = MULT(rot3r, br);                            \
	    tmp = MULT(rot3i, bi);                              \
	    real1 = _mm_add_epi16(real1, tmp);                  \
	    imag1 = MULT(rot3r, bi);                            \
	    tmp = MULT(rot3i, br);                              \
	    imag1 = _mm_sub_epi16(imag1, tmp);                  \
	    real0 = _mm_add_epi16(ar, real1);                   \
	    real1 = _mm_sub_epi16(ar, real1);                   \
	    imag0 = _mm_add_epi16(ai, imag1);                   \
	    imag1 = _mm_sub_epi16(ai, imag1);                   \
	    c0r = MULT(rotc0r, cr);                             \
	    tmp = MULT(rotc1r, ci);                             \
	    c0r = _mm_sub_epi16(c0r, tmp);                      \
	    c0i = MULT(rotc1r, cr);                             \
	    tmp = MULT(rotc0r, ci);                             \
	    c0i = _mm_add_epi16(tmp, c0i);                      \
	    d0r = MULT(rotd0r, dr);                             \
	    tmp = MULT(rotd1r, di);                             \
	    d0r = _mm_sub_epi16(d0r, tmp);                      \
	    d0i = MULT(rotd1r, dr);                             \
	    tmp = MULT(rotd0r, di);                             \
	    d0i = _mm_add_epi16(tmp, d0i);                      \
	    real2 = _mm_add_epi16(c0r, d0r);                    \
	    imag2 = _mm_add_epi16(c0i, d0i);                    \
	    real3 = _mm_sub_epi16(c0i, d0i);                    \
	    imag3 = _mm_sub_epi16(d0r, c0r);                    \
	}

/* *********************************************************** */

#define	STORE_REIM                                              \
	REMIX_REIM(ar, ai, reim0, reim1)                        \
	REMIX_REIM(br, bi, reim2, reim3)                        \
	REMIX_REIM(cr, ci, reim4, reim5)                        \
	REMIX_REIM(dr, di, reim6, reim7)                        \
	_mm_storel_epi64((__m128i *)(pout64), reim0);           \
	_mm_storel_epi64((__m128i *)(pout64 + 1), reim1);       \
	_mm_storel_epi64((__m128i *)(pout64 + 2), reim2);       \
	_mm_storel_epi64((__m128i *)(pout64 + 3), reim3);       \
	_mm_storel_epi64((__m128i *)(pout64 + 4), reim4);       \
	_mm_storel_epi64((__m128i *)(pout64 + 5), reim5);       \
	_mm_storel_epi64((__m128i *)(pout64 + 6), reim6);       \
	_mm_storel_epi64((__m128i *)(pout64 + 7), reim7);

/* *********************************************************** */

#define	STORE_RE                                                \
	pout64 = (mlib_d64 *)dstr;                              \
	_mm_storel_epi64((__m128i *)(pout64), ar);              \
	_mm_storel_epi64((__m128i *)(pout64 + 1), br);          \
	_mm_storel_epi64((__m128i *)(pout64 + 2), cr);          \
	_mm_storel_epi64((__m128i *)(pout64 + 3), dr);

/* *********************************************************** */

#define	STORE_IM                                                \
	pouti64 = (mlib_d64 *)dsti;                             \
	_mm_storel_epi64((__m128i *)(pouti64), ai);             \
	_mm_storel_epi64((__m128i *)(pouti64 + 1), bi);         \
	_mm_storel_epi64((__m128i *)(pouti64 + 2), ci);         \
	_mm_storel_epi64((__m128i *)(pouti64 + 3), di);

/* *********************************************************** */

#define	PREP_OUTC                                               \
	pout64 = (mlib_d64 *)dstc;

#define	PREP_OUTC_SSE2                                          \
	pout64 = (__m128i *)((mlib_addr)dstc & ~7);             \
	mask1 = vis_edge16(dstc, dstc + 8);                     \
	mask2 = ~mask1;                                         \
	if (!((mlib_addr)dstc & 7)) {                           \
	    --pout64;                                           \
	    mask1 = 0;                                          \
	    mask2 = 0xf;                                        \
	}

/* *********************************************************** */

#define	PREP_OUTR                                               \
	pout64 = (mlib_d64 *)dstr;

#define	PREP_OUTR_SSE2                                          \
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
	pouti64 = (mlib_d64 *)dsti;

#define	PREP_OUTI_SSE2                                          \
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
	__m128i ar, br, cr, dr, ai, bi, ci, di;                 \
	__m128i real0, real1, real2, real3;                     \
	__m128i imag0, imag1, imag2, imag3;                     \
	__m128i rot3r, rot3i, rotc0r, rotc1r, rotd0r, rotd1r

#define	DCL_COMMON_SSE2                                         \
	__m128i ar, br, cr, dr, ai, bi, ci, di;                 \
	__m128i real0, real1, real2, real3;                     \
	__m128i imag0, imag1, imag2, imag3;                     \
	__m128i rot3r, rot3i, rotc0r, rotc1r, rotd0r, rotd1r

/* *********************************************************** */

#define	DCL_OUTVARR	mlib_d64 *pout64; mlib_s32 mask1, mask2

#define	DCL_OUTVARR_SSE2	__m128i *pout64; mlib_s32 mask1, mask2

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

#define	VIS_LDF(dsrc, psrc)                                       \
	dsrc = _mm_set_epi16(0, 0, 0, 0, 0, 0, psrc[1], psrc[0]); \

/* *********************************************************** */

#define	VIS_LDD(dsrc0, psrc)                                    \
	{                                                       \
	    dsrc0 = _mm_loadl_epi64((__m128i *)(psrc));         \
	}

/* *********************************************************** */

#define	VIS_LDD_2(dsrc0, dsrc1, psrc)                           \
	{                                                       \
	    mlib_d64 *dpsrc;                                    \
	                                                        \
	    dpsrc = (mlib_d64 *)(psrc);                         \
	    dsrc0 = _mm_loadl_epi64((__m128i *)(dpsrc));        \
	    dsrc1 = _mm_loadl_epi64((__m128i *)(dpsrc + 1));    \
	}

/* *********************************************************** */

#define	VIS_LDD_4(dsrc0, dsrc1, dsrc2, dsrc3, psrc)             \
	{                                                       \
	    mlib_d64 *dpsrc;                                    \
	                                                        \
	    dpsrc = (mlib_d64 *)(psrc);                         \
	    dsrc0 = _mm_loadl_epi64((__m128i *)(dpsrc));        \
	    dsrc1 = _mm_loadl_epi64((__m128i *)(dpsrc + 1));    \
	    dsrc2 = _mm_loadl_epi64((__m128i *)(dpsrc + 2));    \
	    dsrc3 = _mm_loadl_epi64((__m128i *)(dpsrc + 3));    \
	}

/* *********************************************************** */

/* reminder: can be improved here ! */
#define	VIS_STF(pdst, fdst)                                     \
	{                                                       \
	    __m128i tmp128;                                     \
	    mlib_s16 *tmp16 = (mlib_s16 *)&tmp128;              \
	                                                        \
	    _mm_store_si128(&tmp128, fdst);                     \
	    pdst[0] = tmp16[4];                                 \
	    pdst[1] = tmp16[5];                                 \
	}

/* *********************************************************** */

#define	VIS_STD(pdst, dst)                                      \
	{                                                       \
	    _mm_storel_epi64((__m128i *)(pdst), dst);           \
	}

/* *********************************************************** */

#define	VIS_STD_2(pdst, dst0, dst1)                             \
	{                                                       \
	    mlib_d64 *dpdst = (mlib_d64 *)(pdst);               \
	    _mm_storel_epi64((__m128i *)(dpdst), dst0);         \
	    _mm_storel_epi64((__m128i *)(dpdst + 1), dst1);     \
	}

/* *********************************************************** */

#define	VIS_STD_4(pdst, dst0, dst1, dst2, dst3)                 \
	{                                                       \
	    mlib_d64 *dpdst = (mlib_d64 *)(pdst);               \
	    _mm_storel_epi64((__m128i *)(dpdst), dst0);         \
	    _mm_storel_epi64((__m128i *)(dpdst + 1), dst1);     \
	    _mm_storel_epi64((__m128i *)(dpdst + 2), dst2);     \
	    _mm_storel_epi64((__m128i *)(dpdst + 3), dst3);     \
	}

/* *********************************************************** */

#define	DBL_WIND(dwin, dwin1)                                   \
	{                                                       \
	    dwin = _mm_unpacklo_epi16(dwin, dwin);              \
	    dwin1 = _mm_unpackhi_epi64(dwin, dwin);             \
	}

/* *********************************************************** */

#define	VIS_LDF_DBL(dwin, window)                                          \
	    dwin = _mm_setr_epi16(window[0], window[1], 0, 0, 0, 0, 0, 0); \
	    dwin = _mm_unpacklo_epi16(dwin, dwin);

/* *********************************************************** */

#define	DCL_WINDOW	__m128i win0, win1, win2, win3

/* *********************************************************** */

#define	LOAD_WINDOW                                             \
	{                                                       \
	    mlib_d64 *win64;                                    \
	                                                        \
	    win64 = (mlib_d64 *)(window);                       \
	    win0 = _mm_loadl_epi64((__m128i *)(win64));         \
	    win1 = _mm_loadl_epi64((__m128i *)(win64 + 1));     \
	    win2 = _mm_loadl_epi64((__m128i *)(win64 + 2));     \
	    win3 = _mm_loadl_epi64((__m128i *)(win64 + 3));     \
	}

/* *********************************************************** */

#define	DUP_R                                                   \
	real0 = _mm_add_epi16(real0, real0);                    \
	real1 = _mm_add_epi16(real1, real1);                    \
	real2 = _mm_add_epi16(real2, real2);                    \
	real3 = _mm_add_epi16(real3, real3)

/* *********************************************************** */

#define	DUP_I                                                   \
	imag0 = _mm_add_epi16(imag0, imag0);                    \
	imag1 = _mm_add_epi16(imag1, imag1);                    \
	imag2 = _mm_add_epi16(imag2, imag2);                    \
	imag3 = _mm_add_epi16(imag3, imag3)

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

#define	MUL32F(f_src0, f_src1)                                  \
	_mm_unpacklo_epi16(_mm_mullo_epi16(f_src0, f_src1),     \
	    _mm_mulhi_epi16(f_src0, f_src1))

/* *********************************************************** */

#define	MUL32_HL(dst0, dst1, f_sin_pi_on4, d_src)               \
	dst1 = (MUL32F(f_sin_pi_on4, _MM_READ32_hi(d_src)));    \
	dst0 = _mm_unpackhi_epi64(dst1, dst1);

/* *********************************************************** */

#define	MUL32_WIN(dst0, dst1, d_win, d_src)                     \
	{                                                       \
		__m128i d0, d1;                                 \
		d0 = _mm_mulhi_epi16(d_win, d_src);             \
		d1 = _mm_mullo_epi16(d_win, d_src);             \
		dst0 = _mm_unpacklo_epi16(d1, d0);              \
		dst1 = _mm_unpackhi_epi64(dst0, dst0);          \
	}

/* *********************************************************** */

/* flexibility placeholders */
#define	_mm_000_epi32(val0, val1)	(val0)
#define	_MM_READ32_00(val)	(val)

/* *********************************************************** */

#define	_MM_READ32_lo(d)	_mm_srli_epi64(d, 32)

#define	_MM_READ32_hi(d) (d)

/* adds or subs highs or lows read */

#define	AS_HL(addsub, hilo0, d0, hilo1, d1)                     \
	_mm_##addsub##_epi32(_MM_READ32_##hilo0(d0),            \
	    _MM_READ32_##hilo1(d1))

/* *********************************************************** */

/* parametrized vis_freg_pair with AS_HL's */

#define	FREG(addsub0, hilo0, d0, hilo1, d1, addsub2, hilo2,     \
	d2, hilo3, d3)                                          \
	_mm_unpacklo_epi32(AS_HL(addsub0, hilo0, d0, hilo1, d1),\
	    AS_HL(addsub2, hilo2, d2, hilo3, d3))

/* *********************************************************** */

/* FREG with fzero as AS_HL( 000, 00, fzero, 00, 00 ) */

#define	FREG_FZERO(addsub0, hilo0, d0, hilo1, d1)                     \
	FREG(addsub0, hilo0, d0, hilo1, d1, 000, 00, fzero, 00, 00)

/* *********************************************************** */

/* FFT butterflies */
/* simple */

#define	BUTT(dst0, dst1, src0, src1)                            \
	dst0 = _mm_add_epi32(src0, src1);                       \
	dst1 = _mm_sub_epi32(src0, src1)

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

#define	BUTT_FPFX_PR(dst0, dst1, dsrc0, dsrc1, dsrc2, dsrc3)    \
	{                                                       \
		__m128i d0, d1, d2, d3;                         \
		d0 = _mm_add_epi32(dsrc0, dsrc1);               \
		d1 = _mm_add_epi32(dsrc2, dsrc3);               \
		d0 = _mm_unpacklo_epi64(d0, d1);                \
		dst0 = _mm_srai_epi32(d0, 16 - gsr_shift);      \
		dst0 = _mm_packs_epi32(dst0, d1);               \
		d0 = _mm_sub_epi32(dsrc0, dsrc1);               \
		d1 = _mm_sub_epi32(dsrc2, dsrc3);               \
		d0 = _mm_unpacklo_epi64(d0, d1);                \
		dst1 = _mm_srai_epi32(d0, 16 - gsr_shift);      \
		dst1 = _mm_packs_epi32(dst1, d1);               \
	}

/* *********************************************************** */

/* FFT butterfly on vis_fpackfix_pair's cross-like */

#define	BUTT_FPFX_PR_CROSS(dst0, dst1, dsrc0, dsrc1, dsrc2, dsrc3)             \
	dst0 =                                                                 \
	SSE2_fpackfix_pair_r(_mm_add_epi32(dsrc0, dsrc1), _mm_sub_epi32(dsrc2, \
	    dsrc3));                                                           \
	dst1 =                                                                 \
	SSE2_fpackfix_pair_r(_mm_sub_epi32(dsrc0, dsrc1), _mm_add_epi32(dsrc2, \
	    dsrc3))

/* *********************************************************** */

/* simple FFT butterfly on vis_fpackfix_pair */

#define	BUTT_FPFX_PR_SIMPLE(dsrc0, dsrc1)                                      \
		_mm_packs_epi32(_mm_srai_epi32(_mm_unpacklo_epi64(             \
		    _mm_add_epi32(dsrc0, dsrc1), _mm_sub_epi32(dsrc0, dsrc1)), \
		    16-gsr_shift), dsrc0);

/* *********************************************************** */

#define	SSE2_fpackfix_pair(d0, d1) \
		_mm_packs_epi32(_mm_srai_epi32(_mm_unpacklo_epi64(d1, d0),   \
			16-gsr_shift), d0)

#define	SSE2_fpackfix_pair_r(d0, d1)	SSE2_fpackfix_pair(d1, d0)

/* reminder: note dwin is completely useless here !!! */
#define	SSE2_fpackfix(d) \
		_mm_packs_epi32(_mm_srai_epi32(d, 16-gsr_shift), dwin)

#define	SSE2_fpackfix_r(d) \
		_mm_packs_epi32(dwin, _mm_srai_epi32(d, 16-gsr_shift))

#define	SSE2_fmul8x16(shift, dsrc)	_mm_fmul8x16_vis(dsrc, shift);

/* *********************************************************** */

/* general error processing */

/* *********************************************************** */

#define	SERVE_BAD_ORDER(order)                                  \
	if ((order) < 0 || (order) > 31)                        \
	    return MLIB_OUTOFRANGE

#define	SERVE_BAD_POINTER(ptr)	if (!(ptr)) return MLIB_NULLPOINTER

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif	/* _MLIB_S_SIGNALFFT_X_MACROS_H */
