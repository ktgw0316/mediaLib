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

#pragma ident	"@(#)mlib_s_VideoColorYUV2ABGR420_X2.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV2ABGR420_X2 - color conversion-YUV420 to ABGR
 *                                      with zoom x2
 *
 * SYNOPSIS
 *
 * mlib_status mlib_VideoColorYUV2ABGR420_X2(mlib_u8       *abgr,
 *                                           const mlib_u8 *y,
 *                                           const mlib_u8 *u,
 *                                           const mlib_u8 *v,
 *                                           mlib_s32      width,
 *                                           mlib_s32      height,
 *                                           mlib_s32      abgr_stride,
 *                                           mlib_s32      y_stride,
 *                                           mlib_s32      uv_stride)
 *
 * ARGUMENT
 *      abgr            Pointer to output ABGR image
 *      y               Pointer to input Y component
 *      u               Pointer to input U component
 *      v               Pointer to input V component
 *      width           Width of the not zoomed image
 *      height          Height of the not zoomed  image
 *      abgr_stride     Stride in bytes between adjacent rows in output image
 *      y_stride        Stride in bytes between adjacent rows in Y input image
 *      uv_stride       Stride in bytes between adjacent rows in U and
 *                      V input image
 *
 * DESCRIPTION
 *        |A| = 0
 *        |R|   |1.1644   0.0000   1.5966|   [Y   16.0000 ]
 *        |G| = |1.1644  -0.3920  -0.8132| * [U - 128.0000]
 *        |B|   |1.1644   2.0184   0.0000|   [V   128.0000]
 *
 * NOTES
 *  abgr and y must be 8-byte aligned
 *  u and v must be 4-byte aligned
 *  width must be a multiple of 8
 *  height must be a multiple of 2
 *  abgr_stride and y_stride must be a multiple of 8
 *  uv_stride must be a multiple of 4
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

#pragma weak mlib_VideoColorYUV2ABGR420_X2 = __mlib_VideoColorYUV2ABGR420_X2

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorYUV2ABGR420_X2) mlib_VideoColorYUV2ABGR420_X2
	__attribute__((weak, alias("__mlib_VideoColorYUV2ABGR420_X2")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SAVE_ABGR1(x_bgr, pd)				\
{							\
	_mm_store_si128((__m128i *)pd, x_bgr);		\
	pd += 16;					\
}

#define	SAVE_ABGR2(x_bgr, pd)				\
{							\
	_mm_storeu_si128((__m128i *)pd, x_bgr);		\
	pd += 16;					\
}

/* *********************************************************** */
mlib_status
mlib_VideoColorYUV2ABGR420_X2_aligned(
	mlib_u8 *abgr,
	const mlib_u8 *y_inp,
	const mlib_u8 *u_inp,
	const mlib_u8 *v_inp,
	mlib_s32 wd,
	mlib_s32 ht,
	mlib_s32 rgb_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride);

mlib_status
mlib_VideoColorYUV2ABGR420_X2_naligned(
	mlib_u8 *abgr,
	const mlib_u8 *y_inp,
	const mlib_u8 *u_inp,
	const mlib_u8 *v_inp,
	mlib_s32 wd,
	mlib_s32 ht,
	mlib_s32 rgb_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride);

mlib_status
__mlib_VideoColorYUV2ABGR420_X2(
	mlib_u8 *abgr,
	const mlib_u8 *y_inp,
	const mlib_u8 *u_inp,
	const mlib_u8 *v_inp,
	mlib_s32 wd,
	mlib_s32 ht,
	mlib_s32 rgb_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride)
{
	if (abgr == NULL || y_inp == NULL || u_inp == NULL || v_inp == NULL)
		return (MLIB_NULLPOINTER);

	if (wd <= 0 || ht <= 0)
		return (MLIB_FAILURE);

	if ((wd | ht) & 1)
		return (MLIB_FAILURE);

	if (rgb_stride <= 0 || y_stride <= 0 || uv_stride <= 0)
		return (MLIB_FAILURE);

	if ((0 == (0xf & ((mlib_addr)y_inp | (mlib_addr)abgr))) &&
		(0 == (0xf & (y_stride | rgb_stride)))) {
		return mlib_VideoColorYUV2ABGR420_X2_aligned(abgr, y_inp,
			u_inp, v_inp, wd, ht, rgb_stride, y_stride, uv_stride);
	} else {
		return mlib_VideoColorYUV2ABGR420_X2_naligned(abgr, y_inp,
			u_inp, v_inp, wd, ht, rgb_stride, y_stride, uv_stride);
	}
}

mlib_status
mlib_VideoColorYUV2ABGR420_X2_aligned(
	mlib_u8 *abgr,
	const mlib_u8 *y_inp,
	const mlib_u8 *u_inp,
	const mlib_u8 *v_inp,
	mlib_s32 wd,
	mlib_s32 ht,
	mlib_s32 rgb_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride)
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

	mlib_s32 jH, iW;
	mlib_s32 iu, iv, ig, ir, ib, iTemp;
	mlib_s32 iu0, iu1, iv1, iv2;
	__m128i x_u0, x_u1, x_v1, x_v2, x_temp, x_out, x_bak, x_temp1;
	__m128i x_u, x_v, x_y1, x_y2, x_y3, x_y4;
	__m128i x_r1, x_r2, x_r3, x_r4, x_g1, x_g2, x_g3, x_g4;
	__m128i x_b1, x_b2, x_b3, x_b4, x_r, x_g, x_b;
	__m128i x_zbl, x_zbh, x_grl, x_grh, x_bgrh, x_bgrl;
	__m128i *px_y1, *px_y2;
	__m64 *pm_u, *pm_v;
	mlib_u8 *pY1, *pY2, *pU, *pV;
	mlib_u8 *pD1, *pD2, *pD3, *pD4, *pd1, *pd2, *pd3, *pd4;

	pY1 = (mlib_u8*)y_inp;
	pY2 = (mlib_u8*)y_inp + y_stride;
	pU = (mlib_u8*)u_inp;
	pV = (mlib_u8*)v_inp;
	pD1 = abgr;
	pD2 = pD1 + rgb_stride;
	pD3 = pD2 + rgb_stride;
	pD4 = pD3 + rgb_stride;

	for (jH = 0; jH < (ht >> 1); jH++) {
		px_y1 = (__m128i*)pY1;
		px_y2 = (__m128i*)pY2;
		pm_u = (__m64*)pU;
		pm_v = (__m64*)pV;
		pd1 = pD1;
		pd2 = pD2;
		pd3 = pD3;
		pd4 = pD4;
		iW = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; iW <= wd - 16; iW += 16) {
			x_temp = _mm_loadl_epi64((__m128i*)pm_u);
			x_u = _mm_unpacklo_epi8(x_zero, x_temp);
			pm_u++;
			x_temp = _mm_loadl_epi64((__m128i*)pm_v);
			x_v = _mm_unpacklo_epi8(x_zero, x_temp);
			pm_v++;
			x_temp = _mm_load_si128(px_y1);
			px_y1++;
			x_temp1 = _mm_load_si128(px_y2);
			px_y2++;
			x_out = _mm_unpacklo_epi16(x_temp, x_temp1);
			x_y1 = _mm_unpacklo_epi8(x_zero, x_out);
			x_y2 = _mm_unpackhi_epi8(x_zero, x_out);
			x_out = _mm_unpackhi_epi16(x_temp, x_temp1);
			x_y3 = _mm_unpacklo_epi8(x_zero, x_out);
			x_y4 = _mm_unpackhi_epi8(x_zero, x_out);

			/* pre calc x_[r/g/b][12] */
			x_u0 = _mm_mulhi_epu16(x_u, x_c1);
			x_b = _mm_add_epi16(x_u0, x_coff0);

			x_u1 = _mm_mulhi_epu16(x_u, x_c4);
			x_v1 = _mm_mulhi_epu16(x_v, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_g = _mm_sub_epi16(x_coff1, x_temp);

			x_v2 = _mm_mulhi_epu16(x_v, x_c8);
			x_r = _mm_add_epi16(x_v2, x_coff2);

			x_y1 = _mm_mulhi_epu16(x_y1, x_c0);
			x_y2 = _mm_mulhi_epu16(x_y2, x_c0);
			x_y3 = _mm_mulhi_epu16(x_y3, x_c0);
			x_y4 = _mm_mulhi_epu16(x_y4, x_c0);

			/* x_b[1/2/3/4] */
			x_bak = _mm_unpacklo_epi16(x_b, x_b);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_b1 = _mm_add_epi16(x_y1, x_temp);
			x_temp = _mm_unpackhi_epi32(x_bak, x_bak);
			x_b2 = _mm_add_epi16(x_y2, x_temp);

			x_bak = _mm_unpackhi_epi16(x_b, x_b);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_b3 = _mm_add_epi16(x_y3, x_temp);
			x_temp = _mm_unpackhi_epi32(x_bak, x_bak);
			x_b4 = _mm_add_epi16(x_y4, x_temp);

			/* x_g[1/2/3/4] */
			x_bak = _mm_unpacklo_epi16(x_g, x_g);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_g1 = _mm_add_epi16(x_y1, x_temp);
			x_temp = _mm_unpackhi_epi32(x_bak, x_bak);
			x_g2 = _mm_add_epi16(x_y2, x_temp);

			x_bak = _mm_unpackhi_epi16(x_g, x_g);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_g3 = _mm_add_epi16(x_y3, x_temp);
			x_temp = _mm_unpackhi_epi32(x_bak, x_bak);
			x_g4 = _mm_add_epi16(x_y4, x_temp);

			/* x_r[1/2/3/4] */
			x_bak = _mm_unpacklo_epi16(x_r, x_r);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_r1 = _mm_add_epi16(x_y1, x_temp);
			x_temp = _mm_unpackhi_epi32(x_bak, x_bak);
			x_r2 = _mm_add_epi16(x_y2, x_temp);

			x_bak = _mm_unpackhi_epi16(x_r, x_r);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_r3 = _mm_add_epi16(x_y3, x_temp);
			x_temp = _mm_unpackhi_epi32(x_bak, x_bak);
			x_r4 = _mm_add_epi16(x_y4, x_temp);

			x_b1 = _mm_srai_epi16(x_b1, 5);
			x_b2 = _mm_srai_epi16(x_b2, 5);
			x_b3 = _mm_srai_epi16(x_b3, 5);
			x_b4 = _mm_srai_epi16(x_b4, 5);
			x_g1 = _mm_srai_epi16(x_g1, 5);
			x_g2 = _mm_srai_epi16(x_g2, 5);
			x_g3 = _mm_srai_epi16(x_g3, 5);
			x_g4 = _mm_srai_epi16(x_g4, 5);
			x_r1 = _mm_srai_epi16(x_r1, 5);
			x_r2 = _mm_srai_epi16(x_r2, 5);
			x_r3 = _mm_srai_epi16(x_r3, 5);
			x_r4 = _mm_srai_epi16(x_r4, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_b1, x_b2);
			x_b1 = _mm_packus_epi16(x_b3, x_b4);
			x_r = _mm_packus_epi16(x_r1, x_r2);
			x_r1 = _mm_packus_epi16(x_r3, x_r4);
			x_g = _mm_packus_epi16(x_g1, x_g2);
			x_g1 = _mm_packus_epi16(x_g3, x_g4);

			/* create bgr sequences x_[b/r/g] */
			x_zbl = _mm_unpacklo_epi8(x_zero, x_b);
			x_zbh = _mm_unpackhi_epi8(x_zero, x_b);
			x_grl = _mm_unpacklo_epi8(x_g, x_r);
			x_grh = _mm_unpackhi_epi8(x_g, x_r);

			x_bgrl = _mm_unpacklo_epi16(x_zbl, x_grl);
			x_bgrh = _mm_unpackhi_epi16(x_zbl, x_grl);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd1);
			SAVE_ABGR1(x_temp1, pd2);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd1);
			SAVE_ABGR1(x_temp1, pd2);
			x_temp = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd3);
			SAVE_ABGR1(x_temp1, pd4);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd3);
			SAVE_ABGR1(x_temp1, pd4);

			x_bgrl = _mm_unpacklo_epi16(x_zbh, x_grh);
			x_bgrh = _mm_unpackhi_epi16(x_zbh, x_grh);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd1);
			SAVE_ABGR1(x_temp1, pd2);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd1);
			SAVE_ABGR1(x_temp1, pd2);
			x_temp = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd3);
			SAVE_ABGR1(x_temp1, pd4);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd3);
			SAVE_ABGR1(x_temp1, pd4);

			/* create bgr sequences : x_[b1/r1/g1] */
			x_zbl = _mm_unpacklo_epi8(x_zero, x_b1);
			x_zbh = _mm_unpackhi_epi8(x_zero, x_b1);
			x_grl = _mm_unpacklo_epi8(x_g1, x_r1);
			x_grh = _mm_unpackhi_epi8(x_g1, x_r1);

			x_bgrl = _mm_unpacklo_epi16(x_zbl, x_grl);
			x_bgrh = _mm_unpackhi_epi16(x_zbl, x_grl);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd1);
			SAVE_ABGR1(x_temp1, pd2);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd1);
			SAVE_ABGR1(x_temp1, pd2);
			x_temp = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd3);
			SAVE_ABGR1(x_temp1, pd4);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd3);
			SAVE_ABGR1(x_temp1, pd4);

			x_bgrl = _mm_unpacklo_epi16(x_zbh, x_grh);
			x_bgrh = _mm_unpackhi_epi16(x_zbh, x_grh);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd1);
			SAVE_ABGR1(x_temp1, pd2);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd1);
			SAVE_ABGR1(x_temp1, pd2);
			x_temp = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd3);
			SAVE_ABGR1(x_temp1, pd4);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd3);
			SAVE_ABGR1(x_temp1, pd4);
		}

		if (iW <= wd - 8) {
			iTemp = *((mlib_s32 *)pm_u);
			x_temp = _mm_cvtsi32_si128(iTemp);
			x_u = _mm_unpacklo_epi8(x_zero, x_temp);
			pm_u = (__m64 *) (((mlib_s32 *)pm_u) + 1);
			iTemp = *((mlib_s32 *)pm_v);
			x_temp = _mm_cvtsi32_si128(iTemp);
			x_v = _mm_unpacklo_epi8(x_zero, x_temp);
			pm_v = (__m64 *) (((mlib_s32 *)pm_v) + 1);
			x_temp = _mm_loadl_epi64(px_y1);
			px_y1 = (__m128i *) (((__m64 *)px_y1) + 1);
			x_temp1 = _mm_loadl_epi64(px_y2);
			px_y2 = (__m128i *) (((__m64 *)px_y2) + 1);
			x_out = _mm_unpacklo_epi16(x_temp, x_temp1);
			x_y1 = _mm_unpacklo_epi8(x_zero, x_out);
			x_y2 = _mm_unpackhi_epi8(x_zero, x_out);

			/* pre calc x_[r/g/b][12] */
			x_u0 = _mm_mulhi_epu16(x_u, x_c1);
			x_b = _mm_add_epi16(x_u0, x_coff0);

			x_u1 = _mm_mulhi_epu16(x_u, x_c4);
			x_v1 = _mm_mulhi_epu16(x_v, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_g = _mm_sub_epi16(x_coff1, x_temp);

			x_v2 = _mm_mulhi_epu16(x_v, x_c8);
			x_r = _mm_add_epi16(x_v2, x_coff2);

			x_y1 = _mm_mulhi_epu16(x_y1, x_c0);
			x_y2 = _mm_mulhi_epu16(x_y2, x_c0);

			/* x_b[1/2] */
			x_bak = _mm_unpacklo_epi16(x_b, x_b);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_b1 = _mm_add_epi16(x_y1, x_temp);
			x_temp = _mm_unpackhi_epi32(x_bak, x_bak);
			x_b2 = _mm_add_epi16(x_y2, x_temp);

			/* x_g[1/2] */
			x_bak = _mm_unpacklo_epi16(x_g, x_g);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_g1 = _mm_add_epi16(x_y1, x_temp);
			x_temp = _mm_unpackhi_epi32(x_bak, x_bak);
			x_g2 = _mm_add_epi16(x_y2, x_temp);

			/* x_r[1/2] */
			x_bak = _mm_unpacklo_epi16(x_r, x_r);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_r1 = _mm_add_epi16(x_y1, x_temp);
			x_temp = _mm_unpackhi_epi32(x_bak, x_bak);
			x_r2 = _mm_add_epi16(x_y2, x_temp);

			x_b1 = _mm_srai_epi16(x_b1, 5);
			x_b2 = _mm_srai_epi16(x_b2, 5);
			x_g1 = _mm_srai_epi16(x_g1, 5);
			x_g2 = _mm_srai_epi16(x_g2, 5);
			x_r1 = _mm_srai_epi16(x_r1, 5);
			x_r2 = _mm_srai_epi16(x_r2, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_b1, x_b2);
			x_r = _mm_packus_epi16(x_r1, x_r2);
			x_g = _mm_packus_epi16(x_g1, x_g2);

			/* create bgr sequences x_[b/r/g] */
			x_zbl = _mm_unpacklo_epi8(x_zero, x_b);
			x_zbh = _mm_unpackhi_epi8(x_zero, x_b);
			x_grl = _mm_unpacklo_epi8(x_g, x_r);
			x_grh = _mm_unpackhi_epi8(x_g, x_r);

			x_bgrl = _mm_unpacklo_epi16(x_zbl, x_grl);
			x_bgrh = _mm_unpackhi_epi16(x_zbl, x_grl);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd1);
			SAVE_ABGR1(x_temp1, pd2);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd1);
			SAVE_ABGR1(x_temp1, pd2);
			x_temp = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd3);
			SAVE_ABGR1(x_temp1, pd4);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd3);
			SAVE_ABGR1(x_temp1, pd4);

			x_bgrl = _mm_unpacklo_epi16(x_zbh, x_grh);
			x_bgrh = _mm_unpackhi_epi16(x_zbh, x_grh);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd1);
			SAVE_ABGR1(x_temp1, pd2);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd1);
			SAVE_ABGR1(x_temp1, pd2);
			x_temp = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd3);
			SAVE_ABGR1(x_temp1, pd4);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd3);
			SAVE_ABGR1(x_temp1, pd4);

			iW += 8;
		}

		if (iW <= wd - 4) {
			iTemp = *((mlib_s16 *)pm_u);
			x_temp = _mm_cvtsi32_si128(iTemp);
			x_u = _mm_unpacklo_epi8(x_zero, x_temp);
			pm_u = (__m64 *) (((mlib_s16 *)pm_u) + 1);
			iTemp = *((mlib_s16 *)pm_v);
			x_temp = _mm_cvtsi32_si128(iTemp);
			x_v = _mm_unpacklo_epi8(x_zero, x_temp);
			pm_v = (__m64 *) (((mlib_s16 *)pm_v) + 1);

			iTemp = *((mlib_s32 *)px_y1);
			px_y1 = (__m128i *) (((mlib_s32 *)px_y1) + 1);
			x_temp = _mm_cvtsi32_si128(iTemp);
			iTemp = *((mlib_s32 *)px_y2);
			px_y2 = (__m128i *) (((mlib_s32 *)px_y2) + 1);
			x_temp1 = _mm_cvtsi32_si128(iTemp);
			x_out = _mm_unpacklo_epi16(x_temp, x_temp1);
			x_y1 = _mm_unpacklo_epi8(x_zero, x_out);

			/* pre calc x_[r/g/b][1] */
			x_u0 = _mm_mulhi_epu16(x_u, x_c1);
			x_b = _mm_add_epi16(x_u0, x_coff0);

			x_u1 = _mm_mulhi_epu16(x_u, x_c4);
			x_v1 = _mm_mulhi_epu16(x_v, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_g = _mm_sub_epi16(x_coff1, x_temp);

			x_v2 = _mm_mulhi_epu16(x_v, x_c8);
			x_r = _mm_add_epi16(x_v2, x_coff2);

			x_y1 = _mm_mulhi_epu16(x_y1, x_c0);
			x_y2 = _mm_mulhi_epu16(x_y2, x_c0);

			/* x_b[1] */
			x_bak = _mm_unpacklo_epi16(x_b, x_b);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_b1 = _mm_add_epi16(x_y1, x_temp);

			/* x_g[1] */
			x_bak = _mm_unpacklo_epi16(x_g, x_g);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_g1 = _mm_add_epi16(x_y1, x_temp);

			/* x_r[1] */
			x_bak = _mm_unpacklo_epi16(x_r, x_r);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_r1 = _mm_add_epi16(x_y1, x_temp);

			x_b1 = _mm_srai_epi16(x_b1, 5);
			x_g1 = _mm_srai_epi16(x_g1, 5);
			x_r1 = _mm_srai_epi16(x_r1, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_b1, x_zero);
			x_r = _mm_packus_epi16(x_r1, x_zero);
			x_g = _mm_packus_epi16(x_g1, x_zero);

			/* create bgr sequences x_[b/r/g] */
			x_zbl = _mm_unpacklo_epi8(x_zero, x_b);
			x_grl = _mm_unpacklo_epi8(x_g, x_r);

			x_bgrl = _mm_unpacklo_epi16(x_zbl, x_grl);
			x_bgrh = _mm_unpackhi_epi16(x_zbl, x_grl);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd1);
			SAVE_ABGR1(x_temp1, pd2);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd1);
			SAVE_ABGR1(x_temp1, pd2);
			x_temp = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd3);
			SAVE_ABGR1(x_temp1, pd4);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd3);
			SAVE_ABGR1(x_temp1, pd4);

			iW += 4;
		}

		if (iW <= wd - 2) {
			/* load y u v, and expand */
			iu = *((mlib_u8 *)pm_u);
			pm_u = (__m64 *) (((mlib_u8 *)pm_u) + 1);
			iv = *((mlib_u8 *)pm_v);
			pm_v = (__m64 *) (((mlib_u8 *)pm_v) + 1);
			iTemp = (*((mlib_s16 *)px_y1)  & 0xffff) |
				(*((mlib_s16 *)px_y2)<<16);
			x_temp = _mm_cvtsi32_si128(iTemp);
			x_y1 = _mm_unpacklo_epi8(x_zero, x_temp);
			px_y1 = (__m128i *) (((mlib_s16 *)px_y1) + 1);
			px_y2 = (__m128i *) (((mlib_s16 *)px_y2) + 1);

			/* pre-calc d[r/g/b][1] */
			iu0 = (iu * ic1) >> 8;
			ib = icoff0 + iu0;

			iu1 = (iu * ic4) >> 8;
			iv1 = (iv * ic5) >> 8;
			iTemp = iu1 + iv1;
			ig = icoff1 - iTemp;

			iv2 = (iv * ic8) >> 8;
			ir = iv2 + icoff2;

			x_y1 = _mm_mulhi_epu16(x_y1, x_c0);

			/* db1 */
			x_temp = _mm_set1_epi16(ib);
			x_b1 = _mm_add_epi16(x_y1, x_temp);

			/* dg1 */
			x_temp = _mm_set1_epi16(ig);
			x_g1 = _mm_add_epi16(x_y1, x_temp);

			/* dr1 */
			x_temp = _mm_set1_epi16(ir);
			x_r1 = _mm_add_epi16(x_y1, x_temp);

			x_b1 = _mm_srai_epi16(x_b1, 5);
			x_g1 = _mm_srai_epi16(x_g1, 5);
			x_r1 = _mm_srai_epi16(x_r1, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_b1, x_zero);
			x_r = _mm_packus_epi16(x_r1, x_zero);
			x_g = _mm_packus_epi16(x_g1, x_zero);

			/* create bgr sequences : db/dr/dg */
			/* bgr0 bgr0 bgr0 bgr0 */
			x_zbl = _mm_unpacklo_epi8(x_zero, x_b);
			x_grl = _mm_unpacklo_epi8(x_g, x_r);

			/* lower half of zbl & grl */
			x_bgrl = _mm_unpacklo_epi16(x_zbl, x_grl);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_zero);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd1);
			SAVE_ABGR1(x_temp1, pd2);
			x_temp = _mm_unpackhi_epi64(x_bgrl, x_zero);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR1(x_temp1, pd3);
			SAVE_ABGR1(x_temp1, pd4);

			iW += 2;
		}

		pY1 += (2 * y_stride);
		pY2 += (2 * y_stride);
		pU += uv_stride;
		pV += uv_stride;
		pD1 += (4 * rgb_stride);
		pD2 += (4 * rgb_stride);
		pD3 += (4 * rgb_stride);
		pD4 += (4 * rgb_stride);
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoColorYUV2ABGR420_X2_naligned(
	mlib_u8 *abgr,
	const mlib_u8 *y_inp,
	const mlib_u8 *u_inp,
	const mlib_u8 *v_inp,
	mlib_s32 wd,
	mlib_s32 ht,
	mlib_s32 rgb_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride)
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

	mlib_s32 jH, iW;
	mlib_s32 iu, iv, ig, ir, ib, iTemp;
	mlib_s32 iu0, iu1, iv1, iv2;
	__m128i x_u0, x_u1, x_v1, x_v2, x_temp, x_out, x_bak, x_temp1;
	__m128i x_u, x_v, x_y1, x_y2, x_y3, x_y4;
	__m128i x_r1, x_r2, x_r3, x_r4, x_g1, x_g2, x_g3, x_g4;
	__m128i x_b1, x_b2, x_b3, x_b4, x_r, x_g, x_b;
	__m128i x_zbl, x_zbh, x_grl, x_grh, x_bgrh, x_bgrl;
	__m128i *px_y1, *px_y2;
	__m64 *pm_u, *pm_v;
	mlib_u8 *pY1, *pY2, *pU, *pV;
	mlib_u8 *pD1, *pD2, *pD3, *pD4, *pd1, *pd2, *pd3, *pd4;

	pY1 = (mlib_u8*)y_inp;
	pY2 = (mlib_u8*)y_inp + y_stride;
	pU = (mlib_u8*)u_inp;
	pV = (mlib_u8*)v_inp;
	pD1 = abgr;
	pD2 = pD1 + rgb_stride;
	pD3 = pD2 + rgb_stride;
	pD4 = pD3 + rgb_stride;

	for (jH = 0; jH < (ht >> 1); jH++) {
		px_y1 = (__m128i*)pY1;
		px_y2 = (__m128i*)pY2;
		pm_u = (__m64*)pU;
		pm_v = (__m64*)pV;
		pd1 = pD1;
		pd2 = pD2;
		pd3 = pD3;
		pd4 = pD4;
		iW = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; iW <= wd - 16; iW += 16) {
			x_temp = _mm_loadl_epi64((__m128i*)pm_u);
			x_u = _mm_unpacklo_epi8(x_zero, x_temp);
			pm_u++;
			x_temp = _mm_loadl_epi64((__m128i*)pm_v);
			x_v = _mm_unpacklo_epi8(x_zero, x_temp);
			pm_v++;
			x_temp = _mm_loadu_si128(px_y1);
			px_y1++;
			x_temp1 = _mm_loadu_si128(px_y2);
			px_y2++;
			x_out = _mm_unpacklo_epi16(x_temp, x_temp1);
			x_y1 = _mm_unpacklo_epi8(x_zero, x_out);
			x_y2 = _mm_unpackhi_epi8(x_zero, x_out);
			x_out = _mm_unpackhi_epi16(x_temp, x_temp1);
			x_y3 = _mm_unpacklo_epi8(x_zero, x_out);
			x_y4 = _mm_unpackhi_epi8(x_zero, x_out);

			/* pre calc x_[r/g/b][12] */
			x_u0 = _mm_mulhi_epu16(x_u, x_c1);
			x_b = _mm_add_epi16(x_u0, x_coff0);

			x_u1 = _mm_mulhi_epu16(x_u, x_c4);
			x_v1 = _mm_mulhi_epu16(x_v, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_g = _mm_sub_epi16(x_coff1, x_temp);

			x_v2 = _mm_mulhi_epu16(x_v, x_c8);
			x_r = _mm_add_epi16(x_v2, x_coff2);

			x_y1 = _mm_mulhi_epu16(x_y1, x_c0);
			x_y2 = _mm_mulhi_epu16(x_y2, x_c0);
			x_y3 = _mm_mulhi_epu16(x_y3, x_c0);
			x_y4 = _mm_mulhi_epu16(x_y4, x_c0);

			/* x_b[1/2/3/4] */
			x_bak = _mm_unpacklo_epi16(x_b, x_b);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_b1 = _mm_add_epi16(x_y1, x_temp);
			x_temp = _mm_unpackhi_epi32(x_bak, x_bak);
			x_b2 = _mm_add_epi16(x_y2, x_temp);

			x_bak = _mm_unpackhi_epi16(x_b, x_b);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_b3 = _mm_add_epi16(x_y3, x_temp);
			x_temp = _mm_unpackhi_epi32(x_bak, x_bak);
			x_b4 = _mm_add_epi16(x_y4, x_temp);

			/* x_g[1/2/3/4] */
			x_bak = _mm_unpacklo_epi16(x_g, x_g);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_g1 = _mm_add_epi16(x_y1, x_temp);
			x_temp = _mm_unpackhi_epi32(x_bak, x_bak);
			x_g2 = _mm_add_epi16(x_y2, x_temp);

			x_bak = _mm_unpackhi_epi16(x_g, x_g);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_g3 = _mm_add_epi16(x_y3, x_temp);
			x_temp = _mm_unpackhi_epi32(x_bak, x_bak);
			x_g4 = _mm_add_epi16(x_y4, x_temp);

			/* x_r[1/2/3/4] */
			x_bak = _mm_unpacklo_epi16(x_r, x_r);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_r1 = _mm_add_epi16(x_y1, x_temp);
			x_temp = _mm_unpackhi_epi32(x_bak, x_bak);
			x_r2 = _mm_add_epi16(x_y2, x_temp);

			x_bak = _mm_unpackhi_epi16(x_r, x_r);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_r3 = _mm_add_epi16(x_y3, x_temp);
			x_temp = _mm_unpackhi_epi32(x_bak, x_bak);
			x_r4 = _mm_add_epi16(x_y4, x_temp);

			x_b1 = _mm_srai_epi16(x_b1, 5);
			x_b2 = _mm_srai_epi16(x_b2, 5);
			x_b3 = _mm_srai_epi16(x_b3, 5);
			x_b4 = _mm_srai_epi16(x_b4, 5);
			x_g1 = _mm_srai_epi16(x_g1, 5);
			x_g2 = _mm_srai_epi16(x_g2, 5);
			x_g3 = _mm_srai_epi16(x_g3, 5);
			x_g4 = _mm_srai_epi16(x_g4, 5);
			x_r1 = _mm_srai_epi16(x_r1, 5);
			x_r2 = _mm_srai_epi16(x_r2, 5);
			x_r3 = _mm_srai_epi16(x_r3, 5);
			x_r4 = _mm_srai_epi16(x_r4, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_b1, x_b2);
			x_b1 = _mm_packus_epi16(x_b3, x_b4);
			x_r = _mm_packus_epi16(x_r1, x_r2);
			x_r1 = _mm_packus_epi16(x_r3, x_r4);
			x_g = _mm_packus_epi16(x_g1, x_g2);
			x_g1 = _mm_packus_epi16(x_g3, x_g4);

			/* create bgr sequences x_[b/r/g] */
			x_zbl = _mm_unpacklo_epi8(x_zero, x_b);
			x_zbh = _mm_unpackhi_epi8(x_zero, x_b);
			x_grl = _mm_unpacklo_epi8(x_g, x_r);
			x_grh = _mm_unpackhi_epi8(x_g, x_r);

			x_bgrl = _mm_unpacklo_epi16(x_zbl, x_grl);
			x_bgrh = _mm_unpackhi_epi16(x_zbl, x_grl);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);

			x_bgrl = _mm_unpacklo_epi16(x_zbh, x_grh);
			x_bgrh = _mm_unpackhi_epi16(x_zbh, x_grh);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);

			/* create bgr sequences : x_[b1/r1/g1] */
			x_zbl = _mm_unpacklo_epi8(x_zero, x_b1);
			x_zbh = _mm_unpackhi_epi8(x_zero, x_b1);
			x_grl = _mm_unpacklo_epi8(x_g1, x_r1);
			x_grh = _mm_unpackhi_epi8(x_g1, x_r1);

			x_bgrl = _mm_unpacklo_epi16(x_zbl, x_grl);
			x_bgrh = _mm_unpackhi_epi16(x_zbl, x_grl);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);

			x_bgrl = _mm_unpacklo_epi16(x_zbh, x_grh);
			x_bgrh = _mm_unpackhi_epi16(x_zbh, x_grh);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);
		}

		if (iW <= wd - 8) {
			iTemp = *((mlib_s32 *)pm_u);
			x_temp = _mm_cvtsi32_si128(iTemp);
			x_u = _mm_unpacklo_epi8(x_zero, x_temp);
			pm_u = (__m64 *) (((mlib_s32 *)pm_u) + 1);
			iTemp = *((mlib_s32 *)pm_v);
			x_temp = _mm_cvtsi32_si128(iTemp);
			x_v = _mm_unpacklo_epi8(x_zero, x_temp);
			pm_v = (__m64 *) (((mlib_s32 *)pm_v) + 1);
			x_temp = _mm_loadl_epi64(px_y1);
			px_y1 = (__m128i *) (((__m64 *)px_y1) + 1);
			x_temp1 = _mm_loadl_epi64(px_y2);
			px_y2 = (__m128i *) (((__m64 *)px_y2) + 1);
			x_out = _mm_unpacklo_epi16(x_temp, x_temp1);
			x_y1 = _mm_unpacklo_epi8(x_zero, x_out);
			x_y2 = _mm_unpackhi_epi8(x_zero, x_out);

			/* pre calc x_[r/g/b][12] */
			x_u0 = _mm_mulhi_epu16(x_u, x_c1);
			x_b = _mm_add_epi16(x_u0, x_coff0);

			x_u1 = _mm_mulhi_epu16(x_u, x_c4);
			x_v1 = _mm_mulhi_epu16(x_v, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_g = _mm_sub_epi16(x_coff1, x_temp);

			x_v2 = _mm_mulhi_epu16(x_v, x_c8);
			x_r = _mm_add_epi16(x_v2, x_coff2);

			x_y1 = _mm_mulhi_epu16(x_y1, x_c0);
			x_y2 = _mm_mulhi_epu16(x_y2, x_c0);

			/* x_b[1/2] */
			x_bak = _mm_unpacklo_epi16(x_b, x_b);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_b1 = _mm_add_epi16(x_y1, x_temp);
			x_temp = _mm_unpackhi_epi32(x_bak, x_bak);
			x_b2 = _mm_add_epi16(x_y2, x_temp);

			/* x_g[1/2] */
			x_bak = _mm_unpacklo_epi16(x_g, x_g);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_g1 = _mm_add_epi16(x_y1, x_temp);
			x_temp = _mm_unpackhi_epi32(x_bak, x_bak);
			x_g2 = _mm_add_epi16(x_y2, x_temp);

			/* x_r[1/2] */
			x_bak = _mm_unpacklo_epi16(x_r, x_r);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_r1 = _mm_add_epi16(x_y1, x_temp);
			x_temp = _mm_unpackhi_epi32(x_bak, x_bak);
			x_r2 = _mm_add_epi16(x_y2, x_temp);

			x_b1 = _mm_srai_epi16(x_b1, 5);
			x_b2 = _mm_srai_epi16(x_b2, 5);
			x_g1 = _mm_srai_epi16(x_g1, 5);
			x_g2 = _mm_srai_epi16(x_g2, 5);
			x_r1 = _mm_srai_epi16(x_r1, 5);
			x_r2 = _mm_srai_epi16(x_r2, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_b1, x_b2);
			x_r = _mm_packus_epi16(x_r1, x_r2);
			x_g = _mm_packus_epi16(x_g1, x_g2);

			/* create bgr sequences x_[b/r/g] */
			x_zbl = _mm_unpacklo_epi8(x_zero, x_b);
			x_zbh = _mm_unpackhi_epi8(x_zero, x_b);
			x_grl = _mm_unpacklo_epi8(x_g, x_r);
			x_grh = _mm_unpackhi_epi8(x_g, x_r);

			x_bgrl = _mm_unpacklo_epi16(x_zbl, x_grl);
			x_bgrh = _mm_unpackhi_epi16(x_zbl, x_grl);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);

			x_bgrl = _mm_unpacklo_epi16(x_zbh, x_grh);
			x_bgrh = _mm_unpackhi_epi16(x_zbh, x_grh);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);

			iW += 8;
		}

		if (iW <= wd - 4) {
			iTemp = *((mlib_s16 *)pm_u);
			x_temp = _mm_cvtsi32_si128(iTemp);
			x_u = _mm_unpacklo_epi8(x_zero, x_temp);
			pm_u = (__m64 *) (((mlib_s16 *)pm_u) + 1);
			iTemp = *((mlib_s16 *)pm_v);
			x_temp = _mm_cvtsi32_si128(iTemp);
			x_v = _mm_unpacklo_epi8(x_zero, x_temp);
			pm_v = (__m64 *) (((mlib_s16 *)pm_v) + 1);

			iTemp = *((mlib_s32 *)px_y1);
			px_y1 = (__m128i *) (((mlib_s32 *)px_y1) + 1);
			x_temp = _mm_cvtsi32_si128(iTemp);
			iTemp = *((mlib_s32 *)px_y2);
			px_y2 = (__m128i *) (((mlib_s32 *)px_y2) + 1);
			x_temp1 = _mm_cvtsi32_si128(iTemp);
			x_out = _mm_unpacklo_epi16(x_temp, x_temp1);
			x_y1 = _mm_unpacklo_epi8(x_zero, x_out);

			/* pre calc x_[r/g/b][1] */
			x_u0 = _mm_mulhi_epu16(x_u, x_c1);
			x_b = _mm_add_epi16(x_u0, x_coff0);

			x_u1 = _mm_mulhi_epu16(x_u, x_c4);
			x_v1 = _mm_mulhi_epu16(x_v, x_c5);
			x_temp = _mm_add_epi16(x_u1, x_v1);
			x_g = _mm_sub_epi16(x_coff1, x_temp);

			x_v2 = _mm_mulhi_epu16(x_v, x_c8);
			x_r = _mm_add_epi16(x_v2, x_coff2);

			x_y1 = _mm_mulhi_epu16(x_y1, x_c0);
			x_y2 = _mm_mulhi_epu16(x_y2, x_c0);

			/* x_b[1] */
			x_bak = _mm_unpacklo_epi16(x_b, x_b);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_b1 = _mm_add_epi16(x_y1, x_temp);

			/* x_g[1] */
			x_bak = _mm_unpacklo_epi16(x_g, x_g);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_g1 = _mm_add_epi16(x_y1, x_temp);

			/* x_r[1] */
			x_bak = _mm_unpacklo_epi16(x_r, x_r);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_r1 = _mm_add_epi16(x_y1, x_temp);

			x_b1 = _mm_srai_epi16(x_b1, 5);
			x_g1 = _mm_srai_epi16(x_g1, 5);
			x_r1 = _mm_srai_epi16(x_r1, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_b1, x_zero);
			x_r = _mm_packus_epi16(x_r1, x_zero);
			x_g = _mm_packus_epi16(x_g1, x_zero);

			/* create bgr sequences x_[b/r/g] */
			x_zbl = _mm_unpacklo_epi8(x_zero, x_b);
			x_grl = _mm_unpacklo_epi8(x_g, x_r);

			x_bgrl = _mm_unpacklo_epi16(x_zbl, x_grl);
			x_bgrh = _mm_unpackhi_epi16(x_zbl, x_grl);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);
			x_temp1 = _mm_unpackhi_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);

			iW += 4;
		}

		if (iW <= wd - 2) {
			/* load y u v, and expand */
			iu = *((mlib_u8 *)pm_u);
			pm_u = (__m64 *) (((mlib_u8 *)pm_u) + 1);
			iv = *((mlib_u8 *)pm_v);
			pm_v = (__m64 *) (((mlib_u8 *)pm_v) + 1);
			iTemp = (*((mlib_s16 *)px_y1)  & 0xffff) |
				(*((mlib_s16 *)px_y2)<<16);
			x_temp = _mm_cvtsi32_si128(iTemp);
			x_y1 = _mm_unpacklo_epi8(x_zero, x_temp);
			px_y1 = (__m128i *) (((mlib_s16 *)px_y1) + 1);
			px_y2 = (__m128i *) (((mlib_s16 *)px_y2) + 1);

			/* pre-calc d[r/g/b][1] */
			iu0 = (iu * ic1) >> 8;
			ib = icoff0 + iu0;

			iu1 = (iu * ic4) >> 8;
			iv1 = (iv * ic5) >> 8;
			iTemp = iu1 + iv1;
			ig = icoff1 - iTemp;

			iv2 = (iv * ic8) >> 8;
			ir = iv2 + icoff2;

			x_y1 = _mm_mulhi_epu16(x_y1, x_c0);

			/* db1 */
			x_temp = _mm_set1_epi16(ib);
			x_b1 = _mm_add_epi16(x_y1, x_temp);

			/* dg1 */
			x_temp = _mm_set1_epi16(ig);
			x_g1 = _mm_add_epi16(x_y1, x_temp);

			/* dr1 */
			x_temp = _mm_set1_epi16(ir);
			x_r1 = _mm_add_epi16(x_y1, x_temp);

			x_b1 = _mm_srai_epi16(x_b1, 5);
			x_g1 = _mm_srai_epi16(x_g1, 5);
			x_r1 = _mm_srai_epi16(x_r1, 5);

			/* pack: 16=>8 */
			x_b = _mm_packus_epi16(x_b1, x_zero);
			x_r = _mm_packus_epi16(x_r1, x_zero);
			x_g = _mm_packus_epi16(x_g1, x_zero);

			/* create bgr sequences : db/dr/dg */
			/* bgr0 bgr0 bgr0 bgr0 */
			x_zbl = _mm_unpacklo_epi8(x_zero, x_b);
			x_grl = _mm_unpacklo_epi8(x_g, x_r);

			/* lower half of zbl & grl */
			x_bgrl = _mm_unpacklo_epi16(x_zbl, x_grl);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_zero);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd1);
			SAVE_ABGR2(x_temp1, pd2);
			x_temp = _mm_unpackhi_epi64(x_bgrl, x_zero);
			x_temp1 = _mm_unpacklo_epi32(x_temp, x_temp);
			SAVE_ABGR2(x_temp1, pd3);
			SAVE_ABGR2(x_temp1, pd4);

			iW += 2;
		}

		pY1 += (2 * y_stride);
		pY2 += (2 * y_stride);
		pU += uv_stride;
		pV += uv_stride;
		pD1 += (4 * rgb_stride);
		pD2 += (4 * rgb_stride);
		pD3 += (4 * rgb_stride);
		pD4 += (4 * rgb_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
