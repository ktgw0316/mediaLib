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

#pragma ident	"@(#)mlib_s_VideoColorRGB2ABGR.c	9.6	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorRGB2ABGR - RGB to ABGR conversion
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorRGB2ABGR(mlib_u8       *abgr,
 *                                          const mlib_u8 *rgb,
 *                                          mlib_s32      n)
 *
 * ARGUMENTS
 *      abgr      Pointer to ABGR multicomponent row, 8-byte aligned
 *      rgb       Pointer to  RGB multicomponent row, 8-byte aligned
 *      n         Number of pixels
 *
 *      abgr[4*i] - undefined
 *      abgr[4*i+1] = rgb[3*i+2]
 *      abgr[4*i+2] = rgb[3*i+1]
 *      abgr[4*i+3] = rgb[3*i]
 *
 *      0 <= i < n
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

#pragma weak mlib_VideoColorRGB2ABGR = __mlib_VideoColorRGB2ABGR

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorRGB2ABGR) mlib_VideoColorRGB2ABGR
	__attribute__((weak, alias("__mlib_VideoColorRGB2ABGR")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
/* rgb: 48 bytes */
#define	SeparateRGB48(x_rgb0, x_rgb1, x_rgb2, x_r, x_g,  x_b)	\
{	\
	x_t1 = _mm_unpacklo_epi8(x_rgb0, _mm_srli_si128(x_rgb1, 8));	\
	x_t2 = _mm_unpacklo_epi8(_mm_srli_si128(x_rgb0, 8), x_rgb2);	\
	x_t3 = _mm_unpacklo_epi8(x_rgb1, _mm_srli_si128(x_rgb2, 8));	\
	x_t4 = _mm_unpacklo_epi8(x_t1, _mm_srli_si128(x_t2, 8));	\
	x_t5 = _mm_unpacklo_epi8(_mm_srli_si128(x_t1, 8), x_t3);	\
	\
	x_t6 = _mm_unpacklo_epi8(x_t4, _mm_srli_si128(x_t5, 8));	\
	x_t7 = _mm_unpacklo_epi8(x_t2, _mm_srli_si128(x_t3, 8));	\
	x_t8 = _mm_unpacklo_epi8(_mm_srli_si128(x_t4, 8), x_t7);	\
	x_t9 = _mm_unpacklo_epi8(x_t5, _mm_srli_si128(x_t7, 8));	\
	\
	x_r = _mm_unpacklo_epi8(x_t6, _mm_srli_si128(x_t8, 8));	\
	x_g = _mm_unpacklo_epi8(_mm_srli_si128(x_t6, 8), x_t9);	\
	x_b = _mm_unpacklo_epi8(x_t8, _mm_srli_si128(x_t9, 8));	\
}

/* rgb: 24 bytes */
#define	SeparateRGB24(x_rgb0, x_rgb1, x_r, x_g,  x_b)	\
{	\
	x_rgb2 = x_zero;	\
	\
	x_t1 = _mm_unpacklo_epi8(x_rgb0, _mm_srli_si128(x_rgb1, 8));	\
	x_t2 = _mm_unpacklo_epi8(_mm_srli_si128(x_rgb0, 8), x_rgb2);	\
	x_t3 = _mm_unpacklo_epi8(x_rgb1, _mm_srli_si128(x_rgb2, 8));	\
	x_t4 = _mm_unpacklo_epi8(x_t1, _mm_srli_si128(x_t2, 8));	\
	x_t5 = _mm_unpacklo_epi8(_mm_srli_si128(x_t1, 8), x_t3);	\
	\
	x_t6 = _mm_unpacklo_epi8(x_t4, _mm_srli_si128(x_t5, 8));	\
	x_t7 = _mm_unpacklo_epi8(x_t2, _mm_srli_si128(x_t3, 8));	\
	x_t8 = _mm_unpacklo_epi8(_mm_srli_si128(x_t4, 8), x_t7);	\
	x_t9 = _mm_unpacklo_epi8(x_t5, _mm_srli_si128(x_t7, 8));	\
	\
	x_r = _mm_unpacklo_epi8(x_t6, _mm_srli_si128(x_t8, 8));	\
	x_g = _mm_unpacklo_epi8(_mm_srli_si128(x_t6, 8), x_t9);	\
	x_b = _mm_unpacklo_epi8(x_t8, _mm_srli_si128(x_t9, 8));	\
}

/* rgb: 12 bytes */
#define	SeparateRGB12(x_rgb0, x_r, x_g,  x_b)	\
{	\
	x_rgb1 = x_rgb2 = x_zero;	\
	\
	x_t1 = _mm_unpacklo_epi8(x_rgb0, _mm_srli_si128(x_rgb1, 8));	\
	x_t2 = _mm_unpacklo_epi8(_mm_srli_si128(x_rgb0, 8), x_rgb2);	\
	x_t3 = _mm_unpacklo_epi8(x_rgb1, _mm_srli_si128(x_rgb2, 8));	\
	x_t4 = _mm_unpacklo_epi8(x_t1, _mm_srli_si128(x_t2, 8));	\
	x_t5 = _mm_unpacklo_epi8(_mm_srli_si128(x_t1, 8), x_t3);	\
	\
	x_t6 = _mm_unpacklo_epi8(x_t4, _mm_srli_si128(x_t5, 8));	\
	x_t7 = _mm_unpacklo_epi8(x_t2, _mm_srli_si128(x_t3, 8));	\
	x_t8 = _mm_unpacklo_epi8(_mm_srli_si128(x_t4, 8), x_t7);	\
	x_t9 = _mm_unpacklo_epi8(x_t5, _mm_srli_si128(x_t7, 8));	\
	\
	x_r = _mm_unpacklo_epi8(x_t6, _mm_srli_si128(x_t8, 8));	\
	x_g = _mm_unpacklo_epi8(_mm_srli_si128(x_t6, 8), x_t9);	\
	x_b = _mm_unpacklo_epi8(x_t8, _mm_srli_si128(x_t9, 8));	\
}


/* *********************************************************** */
mlib_status
mlib_VideoColorRGB2ABGR_aligned(
	mlib_u8 *abgr,
	const mlib_u8 *rgb,
	mlib_s32 n);

mlib_status
mlib_VideoColorRGB2ABGR_naligned(
	mlib_u8 *abgr,
	const mlib_u8 *rgb,
	mlib_s32 n);

mlib_status
__mlib_VideoColorRGB2ABGR(
	mlib_u8 *abgr,
	const mlib_u8 *rgb,
	mlib_s32 n)
{
	if (rgb == NULL || abgr == NULL)
		return (MLIB_NULLPOINTER);

	if (n <= 0)
		return (MLIB_FAILURE);

	if (0 == (0xf & ((mlib_addr)abgr | (mlib_addr)rgb))) {
		return mlib_VideoColorRGB2ABGR_aligned(
			abgr, rgb, n);
	} else {
		return mlib_VideoColorRGB2ABGR_naligned(
			abgr, rgb, n);
	}

}

mlib_status
mlib_VideoColorRGB2ABGR_aligned(
	mlib_u8 *abgr,
	const mlib_u8 *rgb,
	mlib_s32 n)
{
	__m128i x_t1, x_t2, x_t3, x_t4, x_t5, x_t6, x_t7, x_t8, x_t9;
	__m128i x_rgb0, x_rgb1, x_rgb2, x_r, x_g, x_b;
	__m128i x_zb, x_gr, x_temp;
	const __m128i x_zero = _mm_setzero_si128();
	__m128i *px_abgr, *px_rgb;
	mlib_s32 i, ir, ig, ib, iTemp;

	px_abgr = (__m128i *)abgr;
	px_rgb = (__m128i *)rgb;

	i = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	/* 16 pixels */
	for (; i <= (n - 16); i += 16) {
		x_rgb0 = _mm_load_si128(px_rgb++);
		x_rgb1 = _mm_load_si128(px_rgb++);
		x_rgb2 = _mm_load_si128(px_rgb++);
		SeparateRGB48(x_rgb0, x_rgb1, x_rgb2, x_r, x_g, x_b);
		x_zb = _mm_unpacklo_epi8(x_b, x_b);
		x_gr = _mm_unpacklo_epi8(x_g, x_r);
		x_temp = _mm_unpacklo_epi16(x_zb, x_gr);
		_mm_store_si128(px_abgr++, x_temp);
		x_temp = _mm_unpackhi_epi16(x_zb, x_gr);
		_mm_store_si128(px_abgr++, x_temp);
		x_zb = _mm_unpackhi_epi8(x_b, x_b);
		x_gr = _mm_unpackhi_epi8(x_g, x_r);
		x_temp = _mm_unpacklo_epi16(x_zb, x_gr);
		_mm_store_si128(px_abgr++, x_temp);
		x_temp = _mm_unpackhi_epi16(x_zb, x_gr);
		_mm_store_si128(px_abgr++, x_temp);
	}

	/* 8 pixels */
	if (i <= (n - 8)) {
		x_rgb0 = _mm_load_si128(px_rgb);
		px_rgb++;
		x_rgb1 = _mm_loadl_epi64(px_rgb);
		px_rgb = (__m128i *)(((__m64 *)px_rgb) + 1);
		SeparateRGB24(x_rgb0, x_rgb1, x_r, x_g, x_b);
		x_zb = _mm_unpacklo_epi8(x_b, x_b);
		x_gr = _mm_unpacklo_epi8(x_g, x_r);
		x_temp = _mm_unpacklo_epi16(x_zb, x_gr);
		_mm_store_si128(px_abgr++, x_temp);
		x_temp = _mm_unpackhi_epi16(x_zb, x_gr);
		_mm_store_si128(px_abgr++, x_temp);

		i += 8;
	}

	/* 4 pixels */
	if (i <= (n - 4)) {
		x_rgb0 = _mm_loadl_epi64(px_rgb);
		x_temp = _mm_cvtsi32_si128(((mlib_s32*)px_rgb)[2]);
		x_temp = _mm_slli_si128(x_temp, 8);
		x_rgb0 = _mm_or_si128(x_temp, x_rgb0);
		px_rgb = (__m128i *) (((mlib_s32 *)px_rgb) + 3);
		SeparateRGB12(x_rgb0, x_r, x_g, x_b);
		x_zb = _mm_unpacklo_epi8(x_b, x_b);
		x_gr = _mm_unpacklo_epi8(x_g, x_r);
		x_temp = _mm_unpacklo_epi16(x_zb, x_gr);
		_mm_store_si128(px_abgr++, x_temp);

		i += 4;
	}

	/* 2 pixels or less */
	for (; i <= (n - 1); i++) {
		ir = ((mlib_u8 *)px_rgb)[0];
		ir <<= 24;
		ig = ((mlib_u8 *)px_rgb)[1];
		ig <<= 16;
		ib = ((mlib_u8 *)px_rgb)[2];
		ib <<= 8;
		iTemp = ir | ig | ib;
		px_rgb = (__m128i *)(((mlib_u8 *)px_rgb) + 3);
		((mlib_s32*)px_abgr)[0] = iTemp;
		px_abgr = (__m128i *)(((mlib_s16*)px_abgr) + 2);
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoColorRGB2ABGR_naligned(
	mlib_u8 *abgr,
	const mlib_u8 *rgb,
	mlib_s32 n)
{
	__m128i x_t1, x_t2, x_t3, x_t4, x_t5, x_t6, x_t7, x_t8, x_t9;
	__m128i x_rgb0, x_rgb1, x_rgb2, x_r, x_g, x_b;
	__m128i x_zb, x_gr, x_temp;
	const __m128i x_zero = _mm_setzero_si128();
	__m128i *px_abgr, *px_rgb;
	mlib_s32 i, ir, ig, ib, iTemp;

	px_abgr = (__m128i *)abgr;
	px_rgb = (__m128i *)rgb;

	i = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	/* 16 pixels */
	for (; i <= (n - 16); i += 16) {
		x_rgb0 = _mm_loadu_si128(px_rgb++);
		x_rgb1 = _mm_loadu_si128(px_rgb++);
		x_rgb2 = _mm_loadu_si128(px_rgb++);
		SeparateRGB48(x_rgb0, x_rgb1, x_rgb2, x_r, x_g, x_b);
		x_zb = _mm_unpacklo_epi8(x_b, x_b);
		x_gr = _mm_unpacklo_epi8(x_g, x_r);
		x_temp = _mm_unpacklo_epi16(x_zb, x_gr);
		_mm_storeu_si128(px_abgr++, x_temp);
		x_temp = _mm_unpackhi_epi16(x_zb, x_gr);
		_mm_storeu_si128(px_abgr++, x_temp);
		x_zb = _mm_unpackhi_epi8(x_b, x_b);
		x_gr = _mm_unpackhi_epi8(x_g, x_r);
		x_temp = _mm_unpacklo_epi16(x_zb, x_gr);
		_mm_storeu_si128(px_abgr++, x_temp);
		x_temp = _mm_unpackhi_epi16(x_zb, x_gr);
		_mm_storeu_si128(px_abgr++, x_temp);
	}

	/* 8 pixels */
	if (i <= (n - 8)) {
		x_rgb0 = _mm_loadu_si128(px_rgb);
		px_rgb++;
		x_rgb1 = _mm_loadl_epi64(px_rgb);
		px_rgb = (__m128i *)(((__m64 *)px_rgb) + 1);
		SeparateRGB24(x_rgb0, x_rgb1, x_r, x_g, x_b);
		x_zb = _mm_unpacklo_epi8(x_b, x_b);
		x_gr = _mm_unpacklo_epi8(x_g, x_r);
		x_temp = _mm_unpacklo_epi16(x_zb, x_gr);
		_mm_storeu_si128(px_abgr++, x_temp);
		x_temp = _mm_unpackhi_epi16(x_zb, x_gr);
		_mm_storeu_si128(px_abgr++, x_temp);

		i += 8;
	}

	/* 4 pixels */
	if (i <= (n - 4)) {
		x_rgb0 = _mm_loadl_epi64(px_rgb);
		x_temp = _mm_cvtsi32_si128(((mlib_s32*)px_rgb)[2]);
		x_temp = _mm_slli_si128(x_temp, 8);
		x_rgb0 = _mm_or_si128(x_temp, x_rgb0);
		px_rgb = (__m128i *) (((mlib_s32 *)px_rgb) + 3);
		SeparateRGB12(x_rgb0, x_r, x_g, x_b);
		x_zb = _mm_unpacklo_epi8(x_b, x_b);
		x_gr = _mm_unpacklo_epi8(x_g, x_r);
		x_temp = _mm_unpacklo_epi16(x_zb, x_gr);
		_mm_storeu_si128(px_abgr++, x_temp);

		i += 4;
	}

	/* 2 pixels or less */
	for (; i <= (n - 1); i++) {
		ir = ((mlib_u8 *)px_rgb)[0];
		ir <<= 24;
		ig = ((mlib_u8 *)px_rgb)[1];
		ig <<= 16;
		ib = ((mlib_u8 *)px_rgb)[2];
		ib <<= 8;
		iTemp = ir | ig | ib;
		px_rgb = (__m128i *)(((mlib_u8 *)px_rgb) + 3);
		((mlib_s32*)px_abgr)[0] = iTemp;
		px_abgr = (__m128i *)(((mlib_s16*)px_abgr) + 2);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
