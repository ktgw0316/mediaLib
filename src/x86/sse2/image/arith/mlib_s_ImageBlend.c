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

#pragma ident	"@(#)mlib_s_ImageBlend.c	9.5	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageBlend - blend two images
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageBlend(mlib_image *dst,
 *                                  const mlib_image *src1,
 *                                  const mlib_image *src2,
 *                                  const mlib_image *alpha);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src1    pointer to first input image
 *      src2    pointer to second input image
 *      alpha   pointer to alpha image
 *
 * RESTRICTION
 *      src1, src2, dst, and alpha must be the same type, the same size and
 *      the same number of channels.
 *      They can have one to four channels. They can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type. Optionally, alpha can be
 *      single-channel image while src1, src2, and dst are multi-channel images.
 *      MLIB_BYTE alpha coefficients are in Q8 format.
 *      MLIB_SHORT alpha coefficients are in Q15 format and must be positive.
 *      MLIB_USHORT alpha coefficients are in Q16 format.
 *      MLIB_INT alpha coefficients are in Q31 format and must be positive.
 *
 * DESCRIPTION
 *      Blend two image with an alpha coefficient for each channel.
 *      dst = alpha*src1 + (1-alpha)*src2
 */

#include <mlib_image.h>

/* *************************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageBlend = __mlib_ImageBlend

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageBlend) mlib_ImageBlend
    __attribute__((weak, alias("__mlib_ImageBlend")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	FUNCTION_NAME	__mlib_ImageBlend

/* *********************************************************** */

#define	EXTRA_VARS_U8                                           \
	const __m128i ones = _mm_set1_epi8(0xff);               \
	const __m128i zero = _mm_set1_epi8(0x00);               \
	const __m128i rand = _mm_set1_epi16(0x80);

#define	ARITH_OPER_U8(dd1, sd1, sd2, sda)                                 \
	{                                                                 \
	    __m128i a00 = _mm_sub_epi8(ones, sda);                        \
	    __m128i a10 = _mm_unpackhi_epi8(sda, zero);                   \
	    __m128i a11 = _mm_unpacklo_epi8(sda, zero);                   \
	    __m128i a20 = _mm_unpackhi_epi8(a00, zero);                   \
	    __m128i a21 = _mm_unpacklo_epi8(a00, zero);                   \
	    __m128i r10 = _mm_unpackhi_epi8(sd1, zero);                   \
	    __m128i r11 = _mm_unpacklo_epi8(sd1, zero);                   \
	    __m128i r20 = _mm_unpackhi_epi8(sd2, zero);                   \
	    __m128i r21 = _mm_unpacklo_epi8(sd2, zero);                   \
	    __m128i r30 = _mm_mullo_epi16(r10, a10);                      \
	    __m128i r31 = _mm_mullo_epi16(r11, a11);                      \
	    __m128i r40 = _mm_mullo_epi16(r20, a20);                      \
	    __m128i r41 = _mm_mullo_epi16(r21, a21);                      \
	    __m128i r50 = _mm_add_epi16(_mm_add_epi16(r30, r40), rand);   \
	    __m128i r51 = _mm_add_epi16(_mm_add_epi16(r31, r41), rand);   \
	                                                                  \
	    dd1 =                                                         \
		_mm_packus_epi16(_mm_srli_epi16(r51, 8),                  \
		_mm_srli_epi16(r50, 8));                                  \
	}

/* *********************************************************** */

#define	EXTRA_VARS_S16	const __m128i ones = _mm_set1_epi16(0x7fff);

#define	ARITH_OPER_S16(dd1, sd1, sd2, sda)                          \
	{                                                           \
	    __m128i rd1 = _mm_and_si128(ones, sda);                 \
	    __m128i rd2 = _mm_mulhi_epi16(sd1, rd1);                \
	                                                            \
	    rd1 = _mm_mulhi_epi16(sd2, _mm_sub_epi16(ones, rd1));   \
	    dd1 = _mm_add_epi16(rd2, rd1);                          \
	    dd1 = _mm_add_epi16(dd1, dd1);                          \
	}

/* *********************************************************** */

#define	EXTRA_VARS_U16                                          \
	const __m128i ones = _mm_set1_epi16(0x7fff);            \
	const __m128i fmax = _mm_set1_epi16(0x8000);

#define	ARITH_OPER_U16(dd1, sd1, sd2, sda)                        \
	{                                                         \
	    __m128i rd1 = _mm_sub_epi16(sd1, fmax);               \
	    __m128i rd2 = _mm_sub_epi16(sd2, fmax);               \
	    __m128i ad = _mm_srli_epi16(sda, 1);                  \
	    __m128i bd = _mm_sub_epi16(ones, ad);                 \
	                                                          \
	    rd1 = _mm_mulhi_epi16(rd1, ad);                       \
	    rd2 = _mm_mulhi_epi16(rd2, bd);                       \
	    dd1 = _mm_add_epi16(rd1, rd2);                        \
	    dd1 = _mm_add_epi16(_mm_add_epi16(dd1, dd1), fmax);   \
	}

/* *********************************************************** */

#define	EXTRA_VARS_S32                                              \
	const __m128d neg_bit = _mm_set1_pd(-0.0);                  \
	const __m128d f_scale = _mm_set1_pd(-1.0 / MLIB_S32_MIN);   \
	const __m128d f_one = _mm_set1_pd(1.0);

#define	ARITH_OPER_S32h(d, s1, s2, s3)                          \
	{                                                       \
	    __m128d dres, d1, d2, d3;                           \
	                                                        \
	    d1 = _mm_cvtepi32_pd(s1);                           \
	    d2 = _mm_cvtepi32_pd(s2);                           \
	    d3 = _mm_cvtepi32_pd(s3);                           \
	    d3 = _mm_andnot_pd(neg_bit, d3);                    \
	    d3 = _mm_mul_pd(d3, f_scale);                       \
	    dres =                                              \
		_mm_add_pd(_mm_mul_pd(d1, d3), _mm_mul_pd(d2,   \
		_mm_sub_pd(f_one, d3)));                        \
	    d = _mm_cvtpd_epi32(dres);                          \
	}

#define	ARITH_OPER_S32(d, s1, s2, s3)                           \
	{                                                       \
	    __m128i dh, dl;                                     \
	                                                        \
	    ARITH_OPER_S32h(dl, s1, s2, s3);                    \
	    s1 = _mm_unpackhi_epi64(s1, s1);                    \
	    s2 = _mm_unpackhi_epi64(s2, s2);                    \
	    s3 = _mm_unpackhi_epi64(s3, s3);                    \
	    ARITH_OPER_S32h(dh, s1, s2, s3);                    \
	    d = _mm_unpacklo_epi64(dl, dh);                     \
	}

/* *********************************************************** */

#define	EXTRA_VARS_F32	__m128 f_one = _mm_set1_ps(1.0f);

#define	ARITH_OPER_F32(d, s1, s2, s3)                                     \
	d =                                                               \
	_mm_add_ps(_mm_mul_ps(s1, s3), _mm_mul_ps(s2, _mm_sub_ps(f_one,   \
	    s3)))

/* *********************************************************** */

#define	EXTRA_VARS_D64	__m128d f_one = _mm_set1_pd(1.0);

#define	ARITH_OPER_D64(d, s1, s2, s3)                                     \
	d =                                                               \
	_mm_add_pd(_mm_mul_pd(s1, s3), _mm_mul_pd(s2, _mm_sub_pd(f_one,   \
	    s3)))

/* *********************************************************** */

#include <mlib_s_ImageArith3.h>

/* *********************************************************** */
