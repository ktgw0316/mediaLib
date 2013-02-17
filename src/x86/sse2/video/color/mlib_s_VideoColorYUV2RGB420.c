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

#pragma ident	"@(#)mlib_s_VideoColorYUV2RGB420.c	9.5	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV2RGB420 - color conversion from YUV420 to RGB
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorYUV2RGB420(mlib_u8       *rgb,
 *                                            const mlib_u8 *y,
 *                                            const mlib_u8 *u,
 *                                            const mlib_u8 *v,
 *                                            mlib_s32      width,
 *                                            mlib_s32      height,
 *                                            mlib_s32      rgb_stride,
 *                                            mlib_s32      y_stride,
 *                                            mlib_s32      uv_stride)
 *
 * ARGUMENT
 *      rgb         Pointer to the output RGB image
 *      y           Pointer to input Y component
 *      u           Pointer to input U component
 *      v           Pointer to input V component
 *      width       Width of the image. Note that width must be a multiple of 2
 *      height      Height of the image. Note that height
 *                  must be a multiple of 2
 *      rgb_stride Stride in bytes between adjacent rows in output image
 *      y_stride    Stride in bytes between adjacent rows in Y input image
 *      uv_stride   Stride in bytes between adjacent rows in U and V input
 *                  image
 *
 * DESCRIPTION
 *      mlib_ImageColorConvert2()
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

#pragma weak mlib_VideoColorYUV2RGB420 = __mlib_VideoColorYUV2RGB420

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorYUV2RGB420) mlib_VideoColorYUV2RGB420
	__attribute__((weak, alias("__mlib_VideoColorYUV2RGB420")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
/* save rgb from sequence: rgb* rgb* rgb* rgb* */
#define	PACK_RGB1(x_rgb)	\
{							\
	mlib_u8 *pout = (mlib_u8*)(&x_rgb);		\
	*((mlib_s32*)pd1) = *((mlib_s32*)pout);	\
	pd1 += 3;					\
	pout += 4;					\
	*((mlib_s32*)pd1) = *((mlib_s32*)pout);	\
	pd1 += 3;					\
	pout += 4;					\
	*((mlib_s32*)pd2) = *((mlib_s32*)pout);	\
	pd2 += 3;					\
	pout += 4;					\
	*((mlib_s32*)pd2) = *((mlib_s32*)pout);	\
	pd2 += 3;					\
}

#define	PACK_RGB2(x_rgb)	\
{							\
	mlib_u8 *pout = (mlib_u8*)(&x_rgb);		\
	*((mlib_s32*)pd1) = *((mlib_s32*)pout);	\
	pd1 += 3;					\
	pout += 4;					\
	*((mlib_s16*)pd1) = *((mlib_s16*)pout);	\
	pd1 += 2;					\
	pout += 2;					\
	*((mlib_u8*)pd1) = *((mlib_u8*)pout);		\
	pd1 += 1;					\
	pout += 2;					\
	*((mlib_s32*)pd2) = *((mlib_s32*)pout);	\
	pd2 += 3;					\
	pout += 4;					\
	*((mlib_s16*)pd2) = *((mlib_s16*)pout);	\
	pd2 += 2;					\
	pout += 2;					\
	*((mlib_u8*)pd2) = *((mlib_u8*)pout);		\
	pd2 += 1;					\
}

/* *********************************************************** */
mlib_status
mlib_VideoColorYUV2RGB420_aligned(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 rgb_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride);

mlib_status
mlib_VideoColorYUV2RGB420_naligned(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 rgb_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride);

mlib_status
__mlib_VideoColorYUV2RGB420(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 rgb_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride)
{
	if (rgb == NULL || y == NULL || u == NULL || v == NULL)
		return (MLIB_NULLPOINTER);

	if (width <= 0 || height <= 0)
		return (MLIB_FAILURE);

	if ((width | height) & 1)
		return (MLIB_FAILURE);

	if ((((mlib_addr)y & 0xf) == 0) && ((y_stride & 0xf) == 0)) {
		return mlib_VideoColorYUV2RGB420_aligned(rgb, y, u, v,
			width, height, rgb_stride, y_stride, uv_stride);
	} else {
		return mlib_VideoColorYUV2RGB420_naligned(rgb, y, u, v,
			width, height, rgb_stride, y_stride, uv_stride);
	}
}

mlib_status
mlib_VideoColorYUV2RGB420_aligned(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
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
	__m128i x_rgl, x_rgh, x_bzl, x_bzh, x_rgbh, x_rgbl;
	__m128i *px_y1, *px_y2;
	__m64 *pm_u, *pm_v;
	mlib_u8 *pY1, *pY2, *pU, *pV, *pD1, *pD2, *pd1, *pd2;

	pY1 = (mlib_u8*)y;
	pY2 = (mlib_u8*)y + y_stride;
	pU = (mlib_u8*)u;
	pV = (mlib_u8*)v;
	pD1 = (mlib_u8*)rgb;
	pD2 = (mlib_u8*)rgb + rgb_stride;

	for (jH = 0; jH < (height >> 1); jH++) {
		px_y1 = (__m128i*)pY1;
		px_y2 = (__m128i*)pY2;
		pm_u = (__m64*)pU;
		pm_v = (__m64*)pV;
		pd1 = pD1;
		pd2 = pD2;
		iW = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		/* 32 pixels */
		for (; iW <= width - 16; iW += 16) {
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

			/* create rgb sequences x_[b/r/g] */
			x_rgl = _mm_unpacklo_epi8(x_r, x_g);
			x_rgh = _mm_unpackhi_epi8(x_r, x_g);
			x_bzl = _mm_unpacklo_epi8(x_b, x_b);
			x_bzh = _mm_unpackhi_epi8(x_b, x_b);

			x_rgbl = _mm_unpacklo_epi16(x_rgl, x_bzl);
			PACK_RGB1(x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_rgl, x_bzl);
			PACK_RGB1(x_rgbh);

			x_rgbl = _mm_unpacklo_epi16(x_rgh, x_bzh);
			PACK_RGB1(x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_rgh, x_bzh);
			PACK_RGB1(x_rgbh);

			/* create rgb sequences : x_[b1/r1/g1] */
			x_rgl = _mm_unpacklo_epi8(x_r1, x_g1);
			x_rgh = _mm_unpackhi_epi8(x_r1, x_g1);
			x_bzl = _mm_unpacklo_epi8(x_b1, x_b1);
			x_bzh = _mm_unpackhi_epi8(x_b1, x_b1);

			x_rgbl = _mm_unpacklo_epi16(x_rgl, x_bzl);
			PACK_RGB1(x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_rgl, x_bzl);
			PACK_RGB1(x_rgbh);

			x_rgbl = _mm_unpacklo_epi16(x_rgh, x_bzh);
			PACK_RGB1(x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_rgh, x_bzh);
			PACK_RGB2(x_rgbh);

		}

		/* 16 pixels */
		if (iW <= width - 8) {
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

			/* create rgb sequences x_[b/r/g] */
			x_rgl = _mm_unpacklo_epi8(x_r, x_g);
			x_rgh = _mm_unpackhi_epi8(x_r, x_g);
			x_bzl = _mm_unpacklo_epi8(x_b, x_b);
			x_bzh = _mm_unpackhi_epi8(x_b, x_b);

			x_rgbl = _mm_unpacklo_epi16(x_rgl, x_bzl);
			PACK_RGB1(x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_rgl, x_bzl);
			PACK_RGB1(x_rgbh);

			x_rgbl = _mm_unpacklo_epi16(x_rgh, x_bzh);
			PACK_RGB1(x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_rgh, x_bzh);
			PACK_RGB2(x_rgbh);

			iW += 8;
		}

		/* 8 pixels */
		if (iW <= width - 4)	{
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

			/* x_g[1/2] */
			x_bak = _mm_unpacklo_epi16(x_g, x_g);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_g1 = _mm_add_epi16(x_y1, x_temp);

			/* x_r[1/2] */
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

			/* create rgb sequences x_[b/r/g] */
			x_rgl = _mm_unpacklo_epi8(x_r, x_g);
			x_bzl = _mm_unpacklo_epi8(x_b, x_b);

			x_rgbl = _mm_unpacklo_epi16(x_rgl, x_bzl);
			PACK_RGB1(x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_rgl, x_bzl);
			PACK_RGB2(x_rgbh);

			iW += 4;
		}

		/* 4 pixels */
		if (iW <= width - 2) {
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

			/* create rgb sequences : db/dr/dg */
			/* rgb rgb rgb rgb : 3 mlib_s32 */
			x_rgl = _mm_unpacklo_epi8(x_r, x_g);
			x_bzl = _mm_unpacklo_epi8(x_b, x_b);

			/* lower half of drgl & dbzl */
			x_rgbl = _mm_unpacklo_epi16(x_rgl, x_bzl);
			PACK_RGB2(x_rgbl);

			iW += 2;
		}

		pY1 += (2 * y_stride);
		pY2 += (2 * y_stride);
		pU += uv_stride;
		pV += uv_stride;
		pD1 += (2 * rgb_stride);
		pD2 += (2 * rgb_stride);
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoColorYUV2RGB420_naligned(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
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
	__m128i x_rgl, x_rgh, x_bzl, x_bzh, x_rgbh, x_rgbl;
	__m128i *px_y1, *px_y2;
	__m64 *pm_u, *pm_v;
	mlib_u8 *pY1, *pY2, *pU, *pV, *pD1, *pD2, *pd1, *pd2;

	pY1 = (mlib_u8*)y;
	pY2 = (mlib_u8*)y + y_stride;
	pU = (mlib_u8*)u;
	pV = (mlib_u8*)v;
	pD1 = (mlib_u8*)rgb;
	pD2 = (mlib_u8*)rgb + rgb_stride;

	for (jH = 0; jH < (height >> 1); jH++) {
		px_y1 = (__m128i*)pY1;
		px_y2 = (__m128i*)pY2;
		pm_u = (__m64*)pU;
		pm_v = (__m64*)pV;
		pd1 = pD1;
		pd2 = pD2;
		iW = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		/* 32 pixels */
		for (; iW <= width - 16; iW += 16) {
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

			/* create rgb sequences x_[b/r/g] */
			x_rgl = _mm_unpacklo_epi8(x_r, x_g);
			x_rgh = _mm_unpackhi_epi8(x_r, x_g);
			x_bzl = _mm_unpacklo_epi8(x_b, x_b);
			x_bzh = _mm_unpackhi_epi8(x_b, x_b);

			x_rgbl = _mm_unpacklo_epi16(x_rgl, x_bzl);
			PACK_RGB1(x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_rgl, x_bzl);
			PACK_RGB1(x_rgbh);

			x_rgbl = _mm_unpacklo_epi16(x_rgh, x_bzh);
			PACK_RGB1(x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_rgh, x_bzh);
			PACK_RGB1(x_rgbh);

			/* create rgb sequences : x_[b1/r1/g1] */
			x_rgl = _mm_unpacklo_epi8(x_r1, x_g1);
			x_rgh = _mm_unpackhi_epi8(x_r1, x_g1);
			x_bzl = _mm_unpacklo_epi8(x_b1, x_b1);
			x_bzh = _mm_unpackhi_epi8(x_b1, x_b1);

			x_rgbl = _mm_unpacklo_epi16(x_rgl, x_bzl);
			PACK_RGB1(x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_rgl, x_bzl);
			PACK_RGB1(x_rgbh);

			x_rgbl = _mm_unpacklo_epi16(x_rgh, x_bzh);
			PACK_RGB1(x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_rgh, x_bzh);
			PACK_RGB2(x_rgbh);

		}

		/* 16 pixels */
		if (iW <= width - 8) {
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

			/* create rgb sequences x_[b/r/g] */
			x_rgl = _mm_unpacklo_epi8(x_r, x_g);
			x_rgh = _mm_unpackhi_epi8(x_r, x_g);
			x_bzl = _mm_unpacklo_epi8(x_b, x_b);
			x_bzh = _mm_unpackhi_epi8(x_b, x_b);

			x_rgbl = _mm_unpacklo_epi16(x_rgl, x_bzl);
			PACK_RGB1(x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_rgl, x_bzl);
			PACK_RGB1(x_rgbh);

			x_rgbl = _mm_unpacklo_epi16(x_rgh, x_bzh);
			PACK_RGB1(x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_rgh, x_bzh);
			PACK_RGB2(x_rgbh);

			iW += 8;
		}

		/* 8 pixels */
		if (iW <= width - 4) {
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

			/* x_g[1/2] */
			x_bak = _mm_unpacklo_epi16(x_g, x_g);
			x_temp = _mm_unpacklo_epi32(x_bak, x_bak);
			x_g1 = _mm_add_epi16(x_y1, x_temp);

			/* x_r[1/2] */
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

			/* create rgb sequences x_[b/r/g] */
			x_rgl = _mm_unpacklo_epi8(x_r, x_g);
			x_bzl = _mm_unpacklo_epi8(x_b, x_b);

			x_rgbl = _mm_unpacklo_epi16(x_rgl, x_bzl);
			PACK_RGB1(x_rgbl);

			x_rgbh = _mm_unpackhi_epi16(x_rgl, x_bzl);
			PACK_RGB2(x_rgbh);

			iW += 4;
		}

		/* 4 pixels */
		if (iW <= width - 2) {
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

			/* create rgb sequences : db/dr/dg */
			/* rgb rgb rgb rgb : 3 mlib_s32 */
			x_rgl = _mm_unpacklo_epi8(x_r, x_g);
			x_bzl = _mm_unpacklo_epi8(x_b, x_b);

			/* lower half of drgl & dbzl */
			x_rgbl = _mm_unpacklo_epi16(x_rgl, x_bzl);
			PACK_RGB2(x_rgbl);

			iW += 2;
		}

		pY1 += (2 * y_stride);
		pY2 += (2 * y_stride);
		pU += uv_stride;
		pV += uv_stride;
		pD1 += (2 * rgb_stride);
		pD2 += (2 * rgb_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
