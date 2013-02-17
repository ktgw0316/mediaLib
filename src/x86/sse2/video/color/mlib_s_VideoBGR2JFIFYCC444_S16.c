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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_VideoBGR2JFIFYCC444_S16.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorBGR2JFIFYCC444_S16 - color conversion from BGR 12-bit
 *                                          space to JFIF YCbCr 12-bit space
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorBGR2JFIFYCC444_S16(mlib_s16       *y,
 *                                                    mlib_s16       cb,
 *                                                    mlib_s16       *cr,
 *                                                    const mlib_s16 *bgr,
 *                                                    mlib_s32       n)
 *
 * ARGUMENTS
 *      y         Pointer to Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      bgr       Pointer to BGR multicomponent row, 8-byte aligned
 *      n         Length of the y, cb, cr arrays
 *                Note: Length of the bgr array must be 3*n
 *
 * DESCRIPTION
 *
 *        Y  =  0.29900 * R + 0.58700 * G + 0.11400 * B
 *        Cb = -0.16874 * R - 0.33126 * G + 0.50000 * B  + 2048
 *        Cr =  0.50000 * R - 0.41869 * G - 0.08131 * B  + 2048
 *
 *        B  = bgr[3*i], G = bgr[3*i+1], R = bgr[3*i+2]
 *        y[i] = Y, cr[i] = Cr, cb[i] = Cb, 0 <= i < n
 *
 * RESTRICTION
 *
 *        BGR component values must be in [0, 4095] range.
 *        The resulting YCbCr components are saturating to
 *        the same range.
 */

#include <mlib_video.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorBGR2JFIFYCC444_S16 = \
	__mlib_VideoColorBGR2JFIFYCC444_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorBGR2JFIFYCC444_S16)
	mlib_VideoColorBGR2JFIFYCC444_S16
	__attribute__((weak, alias("__mlib_VideoColorBGR2JFIFYCC444_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
#define	SeparateBGR48_S16			\
{								\
	x_t0 = _mm_unpacklo_epi16(x_bgr0, _mm_srli_si128(x_bgr1, 8));	\
	x_t1 = _mm_unpacklo_epi16(_mm_srli_si128(x_bgr0, 8), x_bgr2);	\
	x_t2 = _mm_unpacklo_epi16(x_bgr1, _mm_srli_si128(x_bgr2, 8));	\
								\
	x_t3 = _mm_unpacklo_epi16(x_t0, _mm_srli_si128(x_t1, 8));	\
	x_t4 = _mm_unpacklo_epi16(_mm_srli_si128(x_t0, 8), x_t2);	\
	x_t5 = _mm_unpacklo_epi16(x_t1, _mm_srli_si128(x_t2, 8));	\
								\
	x_b = _mm_unpacklo_epi16(x_t3, _mm_srli_si128(x_t4, 8));	\
	x_g = _mm_unpacklo_epi16(_mm_srli_si128(x_t3, 8), x_t5);	\
	x_r = _mm_unpacklo_epi16(x_t4, _mm_srli_si128(x_t5, 8));	\
}

#define	SeparateBGR24_S16			\
{								\
	x_t0 = _mm_unpacklo_epi16(x_bgr0, x_zero);			\
	x_t1 = _mm_unpacklo_epi16(_mm_srli_si128(x_bgr0, 8), x_zero);	\
	x_t2 = _mm_unpacklo_epi16(x_bgr1, x_zero);			\
								\
	x_t3 = _mm_unpacklo_epi16(x_t0, _mm_srli_si128(x_t1, 8));	\
	x_t4 = _mm_unpacklo_epi16(_mm_srli_si128(x_t0, 8), x_t2);	\
	x_t5 = _mm_unpacklo_epi16(x_t1, _mm_srli_si128(x_t2, 8));	\
								\
	x_b = _mm_unpacklo_epi16(x_t3, _mm_srli_si128(x_t4, 8));	\
	x_g = _mm_unpacklo_epi16(_mm_srli_si128(x_t3, 8), x_t5);	\
	x_r = _mm_unpacklo_epi16(x_t4, _mm_srli_si128(x_t5, 8));	\
}

/* *********************************************************** */
mlib_status
mlib_VideoColorBGR2JFIFYCC444_S16_aligned(
	mlib_s16 *y,
	mlib_s16 *cb,
	mlib_s16 *cr,
	const mlib_s16 *bgr,
	mlib_s32 n);

mlib_status
mlib_VideoColorBGR2JFIFYCC444_S16_naligned(
	mlib_s16 *y,
	mlib_s16 *cb,
	mlib_s16 *cr,
	const mlib_s16 *bgr,
	mlib_s32 n);

mlib_status
__mlib_VideoColorBGR2JFIFYCC444_S16(
	mlib_s16 *y,
	mlib_s16 *cb,
	mlib_s16 *cr,
	const mlib_s16 *bgr,
	mlib_s32 n)
{
	if (bgr == NULL || y == NULL || cb == NULL || cr == NULL)
		return (MLIB_NULLPOINTER);

	if (n <= 0)
		return (MLIB_FAILURE);

	if (0 == (0xf & ((mlib_addr)y | (mlib_addr)cb |
		(mlib_addr)cr | (mlib_addr)bgr))) {
	    return mlib_VideoColorBGR2JFIFYCC444_S16_aligned(
		y, cb, cr, bgr, n);
	} else {
	    return mlib_VideoColorBGR2JFIFYCC444_S16_naligned(
		y, cb, cr, bgr, n);
	}
}

mlib_status
mlib_VideoColorBGR2JFIFYCC444_S16_aligned(
	mlib_s16 *y,
	mlib_s16 *cb,
	mlib_s16 *cr,
	const mlib_s16 *bgr,
	mlib_s32 n)
{
	/* 0.299*32768 */
	const __m128i x_c11 = _mm_set1_epi16(9798);

	/* 0.587*32768 */
	const __m128i x_c12 = _mm_set1_epi16(19235);

	/* 0.114*32768 */
	const __m128i x_c13 = _mm_set1_epi16(3735);

	/* -0.16874*32768 */
	const __m128i x_c21 = _mm_set1_epi16(-5529);

	/* -0.33126*32768 */
	const __m128i x_c22 = _mm_set1_epi16(-10855);

	/* 0.5*32768 */
	const __m128i x_c23 = _mm_set1_epi16(16384);

	/* 0.5*32768 */
	const __m128i x_c31 = x_c23;

	/* -0.41869*32768 */
	const __m128i x_c32 = _mm_set1_epi16(-13720);

	/* -0.08131*32768 */
	const __m128i x_c33 = _mm_set1_epi16(-2664);

	/* 2048 */
	const __m128i x_coff = _mm_set1_epi16(2048 << 2);

	const __m128i x_zero = _mm_setzero_si128();

	__m128i x_bgr0, x_bgr1, x_bgr2, x_r, x_g, x_b;
	__m128i x_y, x_cb, x_cr;
	__m128i x_t0, x_t1, x_t2, x_t3, x_t4, x_t5;
	__m128i *px_y, *px_cb, *px_cr, *px_bgr;
	mlib_d64 fr, fg, fb, fy, fcb, fcr;
	mlib_s32 i;

	px_y = (__m128i *)y;
	px_cb = (__m128i *)cb;
	px_cr = (__m128i *)cr;
	px_bgr = (__m128i *)bgr;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= (n - 8); i += 8) {
		x_bgr0 = _mm_load_si128(px_bgr++);
		x_bgr0 = _mm_slli_epi16(x_bgr0, 3);
		x_bgr1 = _mm_load_si128(px_bgr++);
		x_bgr1 = _mm_slli_epi16(x_bgr1, 3);
		x_bgr2 = _mm_load_si128(px_bgr++);
		x_bgr2 = _mm_slli_epi16(x_bgr2, 3);
		SeparateBGR48_S16;

		x_t0 = _mm_mulhi_epi16(x_r, x_c11);
		x_t1 = _mm_mulhi_epi16(x_g, x_c12);
		x_t2 = _mm_mulhi_epi16(x_b, x_c13);
		x_y = _mm_add_epi16(x_t0, x_t1);
		x_y = _mm_add_epi16(x_y, x_t2);

		x_t0 = _mm_mulhi_epi16(x_r, x_c21);
		x_t1 = _mm_mulhi_epi16(x_g, x_c22);
		x_t2 = _mm_mulhi_epi16(x_b, x_c23);
		x_cb = _mm_add_epi16(x_t0, x_t1);
		x_cb = _mm_add_epi16(x_cb, x_coff);
		x_cb = _mm_add_epi16(x_cb, x_t2);

		x_t0 = _mm_mulhi_epi16(x_r, x_c31);
		x_t1 = _mm_mulhi_epi16(x_g, x_c32);
		x_t2 = _mm_mulhi_epi16(x_b, x_c33);
		x_cr = _mm_add_epi16(x_t0, x_t1);
		x_cr = _mm_add_epi16(x_cr, x_coff);
		x_cr = _mm_add_epi16(x_cr, x_t2);

		/* save */
		x_y = _mm_srli_epi16(x_y, 2);
		x_cb = _mm_srli_epi16(x_cb, 2);
		x_cr = _mm_srli_epi16(x_cr, 2);
		_mm_store_si128(px_y++, x_y);
		_mm_store_si128(px_cb++, x_cb);
		_mm_store_si128(px_cr++, x_cr);
	}

	if (i <= (n - 4)) {
		x_bgr0 = _mm_load_si128(px_bgr++);
		x_bgr0 = _mm_slli_epi16(x_bgr0, 3);
		x_bgr1 = _mm_loadl_epi64(px_bgr);
		x_bgr1 = _mm_slli_epi16(x_bgr1, 3);
		px_bgr = (__m128i *)((__m64 *)px_bgr + 1);
		SeparateBGR24_S16;

		x_t0 = _mm_mulhi_epi16(x_r, x_c11);
		x_t1 = _mm_mulhi_epi16(x_g, x_c12);
		x_t2 = _mm_mulhi_epi16(x_b, x_c13);
		x_y = _mm_add_epi16(x_t0, x_t1);
		x_y = _mm_add_epi16(x_y, x_t2);

		x_t0 = _mm_mulhi_epi16(x_r, x_c21);
		x_t1 = _mm_mulhi_epi16(x_g, x_c22);
		x_t2 = _mm_mulhi_epi16(x_b, x_c23);
		x_cb = _mm_add_epi16(x_t0, x_t1);
		x_cb = _mm_add_epi16(x_cb, x_coff);
		x_cb = _mm_add_epi16(x_cb, x_t2);

		x_t0 = _mm_mulhi_epi16(x_r, x_c31);
		x_t1 = _mm_mulhi_epi16(x_g, x_c32);
		x_t2 = _mm_mulhi_epi16(x_b, x_c33);
		x_cr = _mm_add_epi16(x_t0, x_t1);
		x_cr = _mm_add_epi16(x_cr, x_coff);
		x_cr = _mm_add_epi16(x_cr, x_t2);

		/* save */
		x_y = _mm_srli_epi16(x_y, 2);
		x_cb = _mm_srli_epi16(x_cb, 2);
		x_cr = _mm_srli_epi16(x_cr, 2);
		_mm_storel_epi64(px_y, x_y);
		px_y = (__m128i *)((__m64 *)px_y + 1);
		_mm_storel_epi64(px_cb, x_cb);
		px_cb = (__m128i *)((__m64 *)px_cb + 1);
		_mm_storel_epi64(px_cr, x_cr);
		px_cr = (__m128i *)((__m64 *)px_cr + 1);

		i += 4;
	}

	for (; i <= (n - 1); i++) {
		fb = bgr[3 * i];
		fg = bgr[3 * i + 1];
		fr = bgr[3 * i + 2];

		fy = 0.29900f * fr + 0.58700f * fg + 0.11400f * fb;
		fcb = -0.16874f * fr - 0.33126f * fg + 0.50000f * fb + 2048;
		fcr = 0.50000f * fr - 0.41869f * fg - 0.08131f * fb + 2048;

		y[i] = (mlib_s16)fy;
		cb[i] = (mlib_s16)fcb;
		cr[i] = (mlib_s16)fcr;
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoColorBGR2JFIFYCC444_S16_naligned(
	mlib_s16 *y,
	mlib_s16 *cb,
	mlib_s16 *cr,
	const mlib_s16 *bgr,
	mlib_s32 n)
{
	/* 0.299*32768 */
	const __m128i x_c11 = _mm_set1_epi16(9798);

	/* 0.587*32768 */
	const __m128i x_c12 = _mm_set1_epi16(19235);

	/* 0.114*32768 */
	const __m128i x_c13 = _mm_set1_epi16(3735);

	/* -0.16874*32768 */
	const __m128i x_c21 = _mm_set1_epi16(-5529);

	/* -0.33126*32768 */
	const __m128i x_c22 = _mm_set1_epi16(-10855);

	/* 0.5*32768 */
	const __m128i x_c23 = _mm_set1_epi16(16384);

	/* 0.5*32768 */
	const __m128i x_c31 = x_c23;

	/* -0.41869*32768 */
	const __m128i x_c32 = _mm_set1_epi16(-13720);

	/* -0.08131*32768 */
	const __m128i x_c33 = _mm_set1_epi16(-2664);

	/* 2048 */
	const __m128i x_coff = _mm_set1_epi16(2048 << 2);

	const __m128i x_zero = _mm_setzero_si128();

	__m128i x_bgr0, x_bgr1, x_bgr2, x_r, x_g, x_b;
	__m128i x_y, x_cb, x_cr;
	__m128i x_t0, x_t1, x_t2, x_t3, x_t4, x_t5;
	__m128i *px_y, *px_cb, *px_cr, *px_bgr;
	mlib_d64 fr, fg, fb, fy, fcb, fcr;
	mlib_s32 i;

	px_y = (__m128i *)y;
	px_cb = (__m128i *)cb;
	px_cr = (__m128i *)cr;
	px_bgr = (__m128i *)bgr;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= (n - 8); i += 8) {
		x_bgr0 = _mm_loadu_si128(px_bgr++);
		x_bgr0 = _mm_slli_epi16(x_bgr0, 3);
		x_bgr1 = _mm_loadu_si128(px_bgr++);
		x_bgr1 = _mm_slli_epi16(x_bgr1, 3);
		x_bgr2 = _mm_loadu_si128(px_bgr++);
		x_bgr2 = _mm_slli_epi16(x_bgr2, 3);
		SeparateBGR48_S16;

		x_t0 = _mm_mulhi_epi16(x_r, x_c11);
		x_t1 = _mm_mulhi_epi16(x_g, x_c12);
		x_t2 = _mm_mulhi_epi16(x_b, x_c13);
		x_y = _mm_add_epi16(x_t0, x_t1);
		x_y = _mm_add_epi16(x_y, x_t2);

		x_t0 = _mm_mulhi_epi16(x_r, x_c21);
		x_t1 = _mm_mulhi_epi16(x_g, x_c22);
		x_t2 = _mm_mulhi_epi16(x_b, x_c23);
		x_cb = _mm_add_epi16(x_t0, x_t1);
		x_cb = _mm_add_epi16(x_cb, x_coff);
		x_cb = _mm_add_epi16(x_cb, x_t2);

		x_t0 = _mm_mulhi_epi16(x_r, x_c31);
		x_t1 = _mm_mulhi_epi16(x_g, x_c32);
		x_t2 = _mm_mulhi_epi16(x_b, x_c33);
		x_cr = _mm_add_epi16(x_t0, x_t1);
		x_cr = _mm_add_epi16(x_cr, x_coff);
		x_cr = _mm_add_epi16(x_cr, x_t2);

		/* save */
		x_y = _mm_srli_epi16(x_y, 2);
		x_cb = _mm_srli_epi16(x_cb, 2);
		x_cr = _mm_srli_epi16(x_cr, 2);
		_mm_storeu_si128(px_y++, x_y);
		_mm_storeu_si128(px_cb++, x_cb);
		_mm_storeu_si128(px_cr++, x_cr);
	}

	if (i <= (n - 4)) {
		x_bgr0 = _mm_loadu_si128(px_bgr++);
		x_bgr0 = _mm_slli_epi16(x_bgr0, 3);
		x_bgr1 = _mm_loadl_epi64(px_bgr);
		x_bgr1 = _mm_slli_epi16(x_bgr1, 3);
		px_bgr = (__m128i *)((__m64 *)px_bgr + 1);
		SeparateBGR24_S16;

		x_t0 = _mm_mulhi_epi16(x_r, x_c11);
		x_t1 = _mm_mulhi_epi16(x_g, x_c12);
		x_t2 = _mm_mulhi_epi16(x_b, x_c13);
		x_y = _mm_add_epi16(x_t0, x_t1);
		x_y = _mm_add_epi16(x_y, x_t2);

		x_t0 = _mm_mulhi_epi16(x_r, x_c21);
		x_t1 = _mm_mulhi_epi16(x_g, x_c22);
		x_t2 = _mm_mulhi_epi16(x_b, x_c23);
		x_cb = _mm_add_epi16(x_t0, x_t1);
		x_cb = _mm_add_epi16(x_cb, x_coff);
		x_cb = _mm_add_epi16(x_cb, x_t2);

		x_t0 = _mm_mulhi_epi16(x_r, x_c31);
		x_t1 = _mm_mulhi_epi16(x_g, x_c32);
		x_t2 = _mm_mulhi_epi16(x_b, x_c33);
		x_cr = _mm_add_epi16(x_t0, x_t1);
		x_cr = _mm_add_epi16(x_cr, x_coff);
		x_cr = _mm_add_epi16(x_cr, x_t2);

		/* save */
		x_y = _mm_srli_epi16(x_y, 2);
		x_cb = _mm_srli_epi16(x_cb, 2);
		x_cr = _mm_srli_epi16(x_cr, 2);
		_mm_storel_epi64(px_y, x_y);
		px_y = (__m128i *)((__m64 *)px_y + 1);
		_mm_storel_epi64(px_cb, x_cb);
		px_cb = (__m128i *)((__m64 *)px_cb + 1);
		_mm_storel_epi64(px_cr, x_cr);
		px_cr = (__m128i *)((__m64 *)px_cr + 1);

		i += 4;
	}

	for (; i <= (n - 1); i++) {
		fb = bgr[3 * i];
		fg = bgr[3 * i + 1];
		fr = bgr[3 * i + 2];

		fy = 0.29900f * fr + 0.58700f * fg + 0.11400f * fb;
		fcb = -0.16874f * fr - 0.33126f * fg + 0.50000f * fb + 2048;
		fcr = 0.50000f * fr - 0.41869f * fg - 0.08131f * fb + 2048;

		y[i] = (mlib_s16)fy;
		cb[i] = (mlib_s16)fcb;
		cr[i] = (mlib_s16)fcr;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
