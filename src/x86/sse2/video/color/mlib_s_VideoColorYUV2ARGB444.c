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

#pragma ident	"@(#)mlib_s_VideoColorYUV2ARGB444.c	9.5	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV2ARGB444 - color conversion from YUV444 to ARGB
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorYUV2ARGB444(mlib_u8       *argb,
 *                                             const mlib_u8 *y,
 *                                             const mlib_u8 *u,
 *                                             const mlib_u8 *v,
 *                                             mlib_s32      width,
 *                                             mlib_s32      height,
 *                                             mlib_s32      argb_stride,
 *                                             mlib_s32      yuv_stride)
 *
 * ARGUMENT
 *      argb        Pointer to the output ARGB image
 *      y           Pointer to input Y component
 *      u           Pointer to input U component
 *      v           Pointer to input V component
 *      width       Width of the image
 *      height      Height of the image
 *      argb_stride Stride in bytes between adjacent rows in output image
 *      yuv_stride  Stride in bytes between adjacent rows input image
 *
 * DESCRIPTION
 *        |R|   |1.1644   0.0000   1.5966|   [|Y|   |16.0000|]
 *        |G| = |1.1644  -0.3920  -0.8132| * [|U| - |128.0000|]
 *        |B|   |1.1644   2.0184   0.0000|   [|V|   |128.0000|]
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

#pragma weak mlib_VideoColorYUV2ARGB444 = __mlib_VideoColorYUV2ARGB444

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorYUV2ARGB444) mlib_VideoColorYUV2ARGB444
	__attribute__((weak, alias("__mlib_VideoColorYUV2ARGB444")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
/* not write alpha data */
#define	SAVE_ARGB1(x_rgb)				\
{							\
	x_temp = _mm_load_si128(px_d);	\
	x_temp = _mm_and_si128(x_temp, x_mask);		\
	x_temp = _mm_or_si128(x_temp, x_rgb);		\
	_mm_store_si128(px_d, x_temp);		\
	px_d += 1;					\
}

#define	SAVE_ARGB2(x_rgb)				\
{							\
	x_temp = _mm_loadu_si128(px_d);	\
	x_temp = _mm_and_si128(x_temp, x_mask);		\
	x_temp = _mm_or_si128(x_temp, x_rgb);		\
	_mm_storeu_si128(px_d, x_temp);	\
	px_d += 1;					\
}

#define	CLAMP_U8(f, c)		\
	{	\
		if ((f) > 255)	\
		{	\
			c = 255;	\
		}	\
		else if ((f) < 0)	\
		{	\
			c = 0;	\
		}	\
		else	\
			c = (f);	\
	}
/* *********************************************************** */

mlib_status
mlib_VideoColorYUV2ARGB444_aligned(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 rgb_stride,
	mlib_s32 yuv_stride);
mlib_status
mlib_VideoColorYUV2ARGB444_naligned(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 rgb_stride,
	mlib_s32 yuv_stride);

mlib_status
__mlib_VideoColorYUV2ARGB444(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 rgb_stride,
	mlib_s32 yuv_stride)
{
	if (rgb == NULL || y == NULL || u == NULL || v == NULL)
		return (MLIB_NULLPOINTER);

	if (xsize <= 0 || ysize <= 0)
		return (MLIB_FAILURE);

	if (yuv_stride == xsize && rgb_stride == 3 * xsize) {
		xsize *= ysize;
		ysize = 1;
	}

	if (((((mlib_addr)y | (mlib_addr)u | (mlib_addr)v | (mlib_addr)rgb) &
				0xf) == 0) &&
		((((yuv_stride | rgb_stride) & 0xf) == 0) ||(ysize == 1))) {
		return mlib_VideoColorYUV2ARGB444_aligned(
			rgb, y, u, v, xsize, ysize, rgb_stride, yuv_stride);
	} else {
		return mlib_VideoColorYUV2ARGB444_naligned(
			rgb, y, u, v, xsize, ysize, rgb_stride, yuv_stride);
	}
}

mlib_status
mlib_VideoColorYUV2ARGB444_aligned(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 rgb_stride,
	mlib_s32 yuv_stride)
{
/* 1.1644  * 8192 */
	const __m128i x_c0 = _mm_set1_epi16(0x2543);
	const mlib_s32 ic0 = 0x2543;

/* 2.0184  * 8192 */
	const __m128i x_c1 = _mm_set1_epi16(0x4097);
	const mlib_s32 ic1 = 0x4097;

/* abs( -0.3920 * 8192 ) */
	const __m128i x_c4 = _mm_set1_epi16(0xc8b);
	const mlib_s32 ic4 = 0xc8b;

/* abs( -0.8132 * 8192 ) */
	const __m128i x_c5 = _mm_set1_epi16(0x1a06);
	const mlib_s32 ic5 = 0x1a06;

/* 1.5966  * 8192 */
	const __m128i x_c8 = _mm_set1_epi16(0x3317);
	const mlib_s32 ic8 = 0x3317;

/* -276.9856 * 32 */
	const __m128i x_coff0 = _mm_set1_epi16(0xdd60);
	const mlib_s32 icoff0 = (mlib_s32)0xffffdd60;

/* 135.6352  * 32 */
	const __m128i x_coff1 = _mm_set1_epi16(0x10f4);
	const mlib_s32 icoff1 = 0x10f4;

/* -222.9952 * 32 */
	const __m128i x_coff2 = _mm_set1_epi16(0xe420);
	const mlib_s32 icoff2 = (mlib_s32)0xffffe420;

	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask = _mm_set1_epi32(0xff);

	mlib_s32 jH, iW;
	__m128i x_u0, x_u1, x_v1, x_v2, x_temp;
	__m128i x_uh, x_ul, x_vh, x_vl, x_yh, x_yl;
	__m128i x_rh, x_rl, x_gh, x_gl, x_bh, x_bl, x_r, x_g, x_b;
	__m128i x_zrl, x_zrh, x_gbl, x_gbh, x_rgbh, x_rgbl;
	__m128i *px_y, *px_u, *px_v, *px_d;
	mlib_u8 *pY, *pU, *pV, *pD;
	mlib_s32 iu, iv, iy, iu0, iu1, iv1, iv2, ir, ig, ib, iTemp;

	pY = (mlib_u8*)y;
	pU = (mlib_u8*)u;
	pV = (mlib_u8*)v;
	pD = (mlib_u8*)rgb;

	for (jH = 0; jH < ysize; jH++) {
		px_y = (__m128i*)pY;
		px_u = (__m128i*)pU;
		px_v = (__m128i*)pV;
		px_d = (__m128i*)pD;
		iW = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; iW <= xsize - 16; iW += 16) {
			x_temp = _mm_load_si128(px_u);
			x_ul = _mm_unpacklo_epi8(x_zero, x_temp);
			x_uh = _mm_unpackhi_epi8(x_zero, x_temp);
			px_u++;
			x_temp = _mm_load_si128(px_v);
			x_vl = _mm_unpacklo_epi8(x_zero, x_temp);
			x_vh = _mm_unpackhi_epi8(x_zero, x_temp);
			px_v++;
			x_temp = _mm_load_si128(px_y);
			x_yl = _mm_unpacklo_epi8(x_zero, x_temp);
			x_yh = _mm_unpackhi_epi8(x_zero, x_temp);
			px_y++;

			/* calc x_[r/g/b][h/l] */
			x_yh = _mm_mulhi_epu16(x_yh, x_c0);
			x_u0 = _mm_mulhi_epu16(x_uh, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bh = _mm_add_epi16(x_temp, x_yh);
			x_u1 = _mm_mulhi_epu16(x_uh, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vh, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gh = _mm_add_epi16(x_temp, x_yh);
			x_v2 = _mm_mulhi_epu16(x_vh, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rh = _mm_add_epi16(x_temp, x_yh);

			x_yl = _mm_mulhi_epu16(x_yl, x_c0);
			x_u0 = _mm_mulhi_epu16(x_ul, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bl = _mm_add_epi16(x_temp, x_yl);
			x_u1 = _mm_mulhi_epu16(x_ul, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vl, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gl = _mm_add_epi16(x_temp, x_yl);
			x_v2 = _mm_mulhi_epu16(x_vl, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rl = _mm_add_epi16(x_temp, x_yl);

			/* shift */
			x_bh = _mm_srai_epi16(x_bh, 5);
			x_bl = _mm_srai_epi16(x_bl, 5);
			x_gh = _mm_srai_epi16(x_gh, 5);
			x_gl = _mm_srai_epi16(x_gl, 5);
			x_rh = _mm_srai_epi16(x_rh, 5);
			x_rl = _mm_srai_epi16(x_rl, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_bl, x_bh);
			x_r = _mm_packus_epi16(x_rl, x_rh);
			x_g = _mm_packus_epi16(x_gl, x_gh);

			/* create rgb sequences x_[b/r/g] */
			x_zrl = _mm_unpacklo_epi8(x_zero, x_r);
			x_zrh = _mm_unpackhi_epi8(x_zero, x_r);
			x_gbl = _mm_unpacklo_epi8(x_g, x_b);
			x_gbh = _mm_unpackhi_epi8(x_g, x_b);

			x_rgbl = _mm_unpacklo_epi16(x_zrl, x_gbl);
			SAVE_ARGB1(x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zrl, x_gbl);
			SAVE_ARGB1(x_rgbh);

			x_rgbl = _mm_unpacklo_epi16(x_zrh, x_gbh);
			SAVE_ARGB1(x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zrh, x_gbh);
			SAVE_ARGB1(x_rgbh);
		}

		if (iW <= xsize - 8) {
			x_temp = _mm_loadl_epi64(px_u);
			x_ul = _mm_unpacklo_epi8(x_zero, x_temp);
			px_u = (__m128i *) (((__m64 *)px_u) + 1);
			x_temp = _mm_loadl_epi64(px_v);
			x_vl = _mm_unpacklo_epi8(x_zero, x_temp);
			px_v = (__m128i *) (((__m64 *)px_v) + 1);
			x_temp = _mm_loadl_epi64(px_y);
			x_yl = _mm_unpacklo_epi8(x_zero, x_temp);
			px_y = (__m128i *) (((__m64 *)px_y) + 1);

			/* calc x_[r/g/b][l] */
			x_yl = _mm_mulhi_epu16(x_yl, x_c0);
			x_u0 = _mm_mulhi_epu16(x_ul, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bl = _mm_add_epi16(x_temp, x_yl);
			x_u1 = _mm_mulhi_epu16(x_ul, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vl, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gl = _mm_add_epi16(x_temp, x_yl);
			x_v2 = _mm_mulhi_epu16(x_vl, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rl = _mm_add_epi16(x_temp, x_yl);

			/* shift */
			x_bl = _mm_srai_epi16(x_bl, 5);
			x_gl = _mm_srai_epi16(x_gl, 5);
			x_rl = _mm_srai_epi16(x_rl, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_bl, x_zero);
			x_r = _mm_packus_epi16(x_rl, x_zero);
			x_g = _mm_packus_epi16(x_gl, x_zero);

			/* create rgb sequences x_[b/r/g] */
			x_zrl = _mm_unpacklo_epi8(x_zero, x_r);
			x_gbl = _mm_unpacklo_epi8(x_g, x_b);

			x_rgbl = _mm_unpacklo_epi16(x_zrl, x_gbl);
			SAVE_ARGB1(x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zrl, x_gbl);
			SAVE_ARGB1(x_rgbh);

			iW += 8;
		}

		if (iW <= xsize - 4) {
			iTemp = *((mlib_s32 *)px_u);
			x_temp = _mm_cvtsi32_si128(iTemp);
			x_ul = _mm_unpacklo_epi8(x_zero, x_temp);
			px_u = (__m128i *) (((mlib_s32 *)px_u) + 1);
			iTemp = *((mlib_s32 *)px_v);
			x_temp = _mm_cvtsi32_si128(iTemp);
			x_vl = _mm_unpacklo_epi8(x_zero, x_temp);
			px_v = (__m128i *) (((mlib_s32 *)px_v) + 1);
			iTemp = *((mlib_s32 *)px_y);
			x_temp = _mm_cvtsi32_si128(iTemp);
			x_yl = _mm_unpacklo_epi8(x_zero, x_temp);
			px_y = (__m128i *) (((mlib_s32 *)px_y) + 1);

			/* calc x_[r/g/b][l] */
			x_yl = _mm_mulhi_epu16(x_yl, x_c0);
			x_u0 = _mm_mulhi_epu16(x_ul, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bl = _mm_add_epi16(x_temp, x_yl);
			x_u1 = _mm_mulhi_epu16(x_ul, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vl, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gl = _mm_add_epi16(x_temp, x_yl);
			x_v2 = _mm_mulhi_epu16(x_vl, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rl = _mm_add_epi16(x_temp, x_yl);

			/* shift */
			x_bl = _mm_srai_epi16(x_bl, 5);
			x_gl = _mm_srai_epi16(x_gl, 5);
			x_rl = _mm_srai_epi16(x_rl, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_bl, x_zero);
			x_r = _mm_packus_epi16(x_rl, x_zero);
			x_g = _mm_packus_epi16(x_gl, x_zero);

			/* create rgb sequences x_[b/r/g] */
			x_zrl = _mm_unpacklo_epi8(x_zero, x_r);
			x_gbl = _mm_unpacklo_epi8(x_g, x_b);

			x_rgbl = _mm_unpacklo_epi16(x_zrl, x_gbl);
			SAVE_ARGB1(x_rgbl);

			iW += 4;
		}

		for (; iW <= xsize - 1; iW++) {
			iu = *((mlib_u8 *)px_u);
			px_u = (__m128i *) (((mlib_u8 *)px_u) + 1);
			iv = *((mlib_u8 *)px_v);
			px_v = (__m128i *) (((mlib_u8 *)px_v) + 1);
			iy = *((mlib_u8 *)px_y);
			px_y = (__m128i *) (((mlib_u8 *)px_y) + 1);

			iy = (iy * ic0) >> 8;
			iu0 = (iu * ic1) >> 8;
			ib = icoff0 + iu0 + iy;
			iu1 = (iu * ic4) >> 8;
			iv1 = (iv * ic5) >> 8;
			iTemp = iu1 + iv1;
			ig = icoff1 - iTemp + iy;
			iv2 = (iv * ic8) >> 8;
			ir = iv2 + icoff2 + iy;

			ir = ir >> 5;
			ig = ig >> 5;
			ib = ib >> 5;

			CLAMP_U8(ir, ((mlib_u8*)px_d)[1]);
			CLAMP_U8(ig, ((mlib_u8*)px_d)[2]);
			CLAMP_U8(ib, ((mlib_u8*)px_d)[3]);

			px_d = (__m128i*) (((mlib_u8*)px_d) + 4);
		}

		pY += yuv_stride;
		pU += yuv_stride;
		pV += yuv_stride;
		pD += rgb_stride;
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoColorYUV2ARGB444_naligned(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 rgb_stride,
	mlib_s32 yuv_stride)
{
/* 1.1644  * 8192 */
	const __m128i x_c0 = _mm_set1_epi16(0x2543);
	const mlib_s32 ic0 = 0x2543;

/* 2.0184  * 8192 */
	const __m128i x_c1 = _mm_set1_epi16(0x4097);
	const mlib_s32 ic1 = 0x4097;

/* abs( -0.3920 * 8192 ) */
	const __m128i x_c4 = _mm_set1_epi16(0xc8b);
	const mlib_s32 ic4 = 0xc8b;

/* abs( -0.8132 * 8192 ) */
	const __m128i x_c5 = _mm_set1_epi16(0x1a06);
	const mlib_s32 ic5 = 0x1a06;

/* 1.5966  * 8192 */
	const __m128i x_c8 = _mm_set1_epi16(0x3317);
	const mlib_s32 ic8 = 0x3317;

/* -276.9856 * 32 */
	const __m128i x_coff0 = _mm_set1_epi16(0xdd60);
	const mlib_s32 icoff0 = (mlib_s32)0xffffdd60;

/* 135.6352  * 32 */
	const __m128i x_coff1 = _mm_set1_epi16(0x10f4);
	const mlib_s32 icoff1 = 0x10f4;

/* -222.9952 * 32 */
	const __m128i x_coff2 = _mm_set1_epi16(0xe420);
	const mlib_s32 icoff2 = (mlib_s32)0xffffe420;

	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask = _mm_set1_epi32(0xff);

	mlib_s32 jH, iW;
	__m128i x_u0, x_u1, x_v1, x_v2, x_temp;
	__m128i x_uh, x_ul, x_vh, x_vl, x_yh, x_yl;
	__m128i x_rh, x_rl, x_gh, x_gl, x_bh, x_bl, x_r, x_g, x_b;
	__m128i x_zrl, x_zrh, x_gbl, x_gbh, x_rgbh, x_rgbl;
	__m128i *px_y, *px_u, *px_v, *px_d;
	mlib_u8 *pY, *pU, *pV, *pD;
	mlib_s32 iu, iv, iy, iu0, iu1, iv1, iv2, ir, ig, ib, iTemp;

	pY = (mlib_u8*)y;
	pU = (mlib_u8*)u;
	pV = (mlib_u8*)v;
	pD = (mlib_u8*)rgb;

	for (jH = 0; jH < ysize; jH++) {
		px_y = (__m128i*)pY;
		px_u = (__m128i*)pU;
		px_v = (__m128i*)pV;
		px_d = (__m128i*)pD;
		iW = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; iW <= xsize - 16; iW += 16) {
			x_temp = _mm_loadu_si128(px_u);
			x_ul = _mm_unpacklo_epi8(x_zero, x_temp);
			x_uh = _mm_unpackhi_epi8(x_zero, x_temp);
			px_u++;
			x_temp = _mm_loadu_si128(px_v);
			x_vl = _mm_unpacklo_epi8(x_zero, x_temp);
			x_vh = _mm_unpackhi_epi8(x_zero, x_temp);
			px_v++;
			x_temp = _mm_loadu_si128(px_y);
			x_yl = _mm_unpacklo_epi8(x_zero, x_temp);
			x_yh = _mm_unpackhi_epi8(x_zero, x_temp);
			px_y++;

			/* calc x_[r/g/b][h/l] */
			x_yh = _mm_mulhi_epu16(x_yh, x_c0);
			x_u0 = _mm_mulhi_epu16(x_uh, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bh = _mm_add_epi16(x_temp, x_yh);
			x_u1 = _mm_mulhi_epu16(x_uh, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vh, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gh = _mm_add_epi16(x_temp, x_yh);
			x_v2 = _mm_mulhi_epu16(x_vh, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rh = _mm_add_epi16(x_temp, x_yh);

			x_yl = _mm_mulhi_epu16(x_yl, x_c0);
			x_u0 = _mm_mulhi_epu16(x_ul, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bl = _mm_add_epi16(x_temp, x_yl);
			x_u1 = _mm_mulhi_epu16(x_ul, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vl, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gl = _mm_add_epi16(x_temp, x_yl);
			x_v2 = _mm_mulhi_epu16(x_vl, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rl = _mm_add_epi16(x_temp, x_yl);

			/* shift */
			x_bh = _mm_srai_epi16(x_bh, 5);
			x_bl = _mm_srai_epi16(x_bl, 5);
			x_gh = _mm_srai_epi16(x_gh, 5);
			x_gl = _mm_srai_epi16(x_gl, 5);
			x_rh = _mm_srai_epi16(x_rh, 5);
			x_rl = _mm_srai_epi16(x_rl, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_bl, x_bh);
			x_r = _mm_packus_epi16(x_rl, x_rh);
			x_g = _mm_packus_epi16(x_gl, x_gh);

			/* create rgb sequences x_[b/r/g] */
			x_zrl = _mm_unpacklo_epi8(x_zero, x_r);
			x_zrh = _mm_unpackhi_epi8(x_zero, x_r);
			x_gbl = _mm_unpacklo_epi8(x_g, x_b);
			x_gbh = _mm_unpackhi_epi8(x_g, x_b);

			x_rgbl = _mm_unpacklo_epi16(x_zrl, x_gbl);
			SAVE_ARGB2(x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zrl, x_gbl);
			SAVE_ARGB2(x_rgbh);

			x_rgbl = _mm_unpacklo_epi16(x_zrh, x_gbh);
			SAVE_ARGB2(x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zrh, x_gbh);
			SAVE_ARGB2(x_rgbh);
		}

		if (iW <= xsize - 8) {
			x_temp = _mm_loadl_epi64(px_u);
			x_ul = _mm_unpacklo_epi8(x_zero, x_temp);
			px_u = (__m128i *) (((__m64 *)px_u) + 1);
			x_temp = _mm_loadl_epi64(px_v);
			x_vl = _mm_unpacklo_epi8(x_zero, x_temp);
			px_v = (__m128i *) (((__m64 *)px_v) + 1);
			x_temp = _mm_loadl_epi64(px_y);
			x_yl = _mm_unpacklo_epi8(x_zero, x_temp);
			px_y = (__m128i *) (((__m64 *)px_y) + 1);

			/* calc x_[r/g/b][l] */
			x_yl = _mm_mulhi_epu16(x_yl, x_c0);
			x_u0 = _mm_mulhi_epu16(x_ul, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bl = _mm_add_epi16(x_temp, x_yl);
			x_u1 = _mm_mulhi_epu16(x_ul, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vl, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gl = _mm_add_epi16(x_temp, x_yl);
			x_v2 = _mm_mulhi_epu16(x_vl, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rl = _mm_add_epi16(x_temp, x_yl);

			/* shift */
			x_bl = _mm_srai_epi16(x_bl, 5);
			x_gl = _mm_srai_epi16(x_gl, 5);
			x_rl = _mm_srai_epi16(x_rl, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_bl, x_zero);
			x_r = _mm_packus_epi16(x_rl, x_zero);
			x_g = _mm_packus_epi16(x_gl, x_zero);

			/* create rgb sequences x_[b/r/g] */
			x_zrl = _mm_unpacklo_epi8(x_zero, x_r);
			x_gbl = _mm_unpacklo_epi8(x_g, x_b);

			x_rgbl = _mm_unpacklo_epi16(x_zrl, x_gbl);
			SAVE_ARGB2(x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_zrl, x_gbl);
			SAVE_ARGB2(x_rgbh);

			iW += 8;
		}

		if (iW <= xsize - 4) {
			iTemp = *((mlib_s32 *)px_u);
			x_temp = _mm_cvtsi32_si128(iTemp);
			x_ul = _mm_unpacklo_epi8(x_zero, x_temp);
			px_u = (__m128i *) (((mlib_s32 *)px_u) + 1);
			iTemp = *((mlib_s32 *)px_v);
			x_temp = _mm_cvtsi32_si128(iTemp);
			x_vl = _mm_unpacklo_epi8(x_zero, x_temp);
			px_v = (__m128i *) (((mlib_s32 *)px_v) + 1);
			iTemp = *((mlib_s32 *)px_y);
			x_temp = _mm_cvtsi32_si128(iTemp);
			x_yl = _mm_unpacklo_epi8(x_zero, x_temp);
			px_y = (__m128i *) (((mlib_s32 *)px_y) + 1);

			/* calc x_[r/g/b][l] */
			x_yl = _mm_mulhi_epu16(x_yl, x_c0);
			x_u0 = _mm_mulhi_epu16(x_ul, x_c1);
			x_temp = _mm_add_epi16(x_u0, x_coff0);
			x_bl = _mm_add_epi16(x_temp, x_yl);
			x_u1 = _mm_mulhi_epu16(x_ul, x_c4);
			x_v1 = _mm_mulhi_epu16(x_vl, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_temp = _mm_sub_epi16(x_coff1, x_temp);
			x_gl = _mm_add_epi16(x_temp, x_yl);
			x_v2 = _mm_mulhi_epu16(x_vl, x_c8);
			x_temp = _mm_add_epi16(x_v2, x_coff2);
			x_rl = _mm_add_epi16(x_temp, x_yl);

			/* shift */
			x_bl = _mm_srai_epi16(x_bl, 5);
			x_gl = _mm_srai_epi16(x_gl, 5);
			x_rl = _mm_srai_epi16(x_rl, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_bl, x_zero);
			x_r = _mm_packus_epi16(x_rl, x_zero);
			x_g = _mm_packus_epi16(x_gl, x_zero);

			/* create rgb sequences x_[b/r/g] */
			x_zrl = _mm_unpacklo_epi8(x_zero, x_r);
			x_gbl = _mm_unpacklo_epi8(x_g, x_b);

			x_rgbl = _mm_unpacklo_epi16(x_zrl, x_gbl);
			SAVE_ARGB2(x_rgbl);

			iW += 4;
		}

		for (; iW <= xsize - 1; iW++) {
			iu = *((mlib_u8 *)px_u);
			px_u = (__m128i *) (((mlib_u8 *)px_u) + 1);
			iv = *((mlib_u8 *)px_v);
			px_v = (__m128i *) (((mlib_u8 *)px_v) + 1);
			iy = *((mlib_u8 *)px_y);
			px_y = (__m128i *) (((mlib_u8 *)px_y) + 1);

			iy = (iy * ic0) >> 8;
			iu0 = (iu * ic1) >> 8;
			ib = icoff0 + iu0 + iy;
			iu1 = (iu * ic4) >> 8;
			iv1 = (iv * ic5) >> 8;
			iTemp = iu1 + iv1;
			ig = icoff1 - iTemp + iy;
			iv2 = (iv * ic8) >> 8;
			ir = iv2 + icoff2 + iy;

			ir = ir >> 5;
			ig = ig >> 5;
			ib = ib >> 5;

			CLAMP_U8(ir, ((mlib_u8*)px_d)[1]);
			CLAMP_U8(ig, ((mlib_u8*)px_d)[2]);
			CLAMP_U8(ib, ((mlib_u8*)px_d)[3]);

			px_d = (__m128i*) (((mlib_u8*)px_d) + 4);
		}

		pY += yuv_stride;
		pU += yuv_stride;
		pV += yuv_stride;
		pD += rgb_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
