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

#pragma ident	"@(#)mlib_s_VideoColorYUV2ABGR420_W.c	9.5	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV2ABGR420_W - color conversion-YUV420 to ABGR
 *                                     inside clipping window
 *
 * SYNOPSIS
 *
 * mlib_status mlib_VideoColorYUV2ABGR420_W(mlib_u8       *abgr,
 *                                          const mlib_u8 *y,
 *                                          const mlib_u8 *u,
 *                                          const mlib_u8 *v,
 *                                          mlib_s32      width,
 *                                          mlib_s32      height,
 *                                          mlib_s32      abgr_stride,
 *                                          mlib_s32      y_stride,
 *                                          mlib_s32      uv_stride,
 *                                          mlib_s32      left,
 *                                          mlib_s32      top,
 *                                          mlib_s32      right,
 *                                          mlib_s32      bottom)
 *
 * ARGUMENT
 *      abgr        Pointer to output ABGR image
 *      y           Pointer to input Y component
 *      u           Pointer to input U component
 *      v           Pointer to input V component
 *      width       Width of the image
 *      height      Height of the image
 *      abgr_stride Stride in bytes between adjacent rows in output image
 *      y_stride    Stride in bytes between adjacent rows in Y input image
 *      uv_stride   Stride in bytes between adjacent rows in U and V input image
 *      left        left border of the clipping window
 *      top         top border of the clipping window
 *      right       right opened border of the clipping window
 *      bottom      bottom opened border of the clipping window
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
 *  window borders must be: 0 <= left < right <= width
 *  and 0 <= top < bottom <= height
 */

#include <mlib_video.h>
#include <mlib_c_VideoColorYUV.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorYUV2ABGR420_W = __mlib_VideoColorYUV2ABGR420_W

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorYUV2ABGR420_W) mlib_VideoColorYUV2ABGR420_W
	__attribute__((weak, alias("__mlib_VideoColorYUV2ABGR420_W")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	TCLAMP_U8(val, dst)	dst = pClip[val]

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN
#define	TCLAMP_U8E(val, dst)	*((mlib_u16*)&(dst)) = pClip[val]<<8;
#else /* _LITTLE_ENDIAN */
#define	TCLAMP_U8E(val, dst)	*((mlib_u16*)&(dst)) = pClip[val];
#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	CALC1(_dst, _y, _u, _v)                                 \
	sfb = mlib_tBU[_u] - BBi;                               \
	sfg = mlib_tGU[_u] - mlib_tGV[_v];                      \
	sfr = mlib_tRV[_v] - RRi;                               \
	sf01 = mlib_tYY[_y];                                    \
	TCLAMP_U8E(sf01 + sfb, _dst[0]);                        \
	TCLAMP_U8(sf01 + sfg, _dst[2]);                         \
	TCLAMP_U8(sf01 + sfr, _dst[3])

/* *********************************************************** */

#define	CALC2(_dst1, _dst2, _y1, _y2, _u, _v)                   \
	sfb = mlib_tBU[_u] - BBi;                               \
	sfg = mlib_tGU[_u] - mlib_tGV[_v];                      \
	sfr = mlib_tRV[_v] - RRi;                               \
	sf01 = mlib_tYY[_y1];                                   \
	sf02 = mlib_tYY[_y2];                                   \
	TCLAMP_U8E(sf01 + sfb, _dst1[0]);                       \
	TCLAMP_U8(sf01 + sfg, _dst1[2]);                        \
	TCLAMP_U8(sf01 + sfr, _dst1[3]);                        \
	TCLAMP_U8E(sf02 + sfb, _dst2[0]);                       \
	TCLAMP_U8(sf02 + sfg, _dst2[2]);                        \
	TCLAMP_U8(sf02 + sfr, _dst2[3])

/* *********************************************************** */

#define	_mm_storeh_epi64(p, x_a)			\
{							\
	__m128i x_ra = _mm_srli_si128(x_a, 8);		\
	_mm_storel_epi64(p, x_ra);			\
}

#define	SAVE_ABGR2(bgr, pd)				\
{							\
	_mm_storeu_si128((__m128i*)pd, bgr);		\
	pd += 16;					\
}

/* *********************************************************** */

mlib_status
__mlib_VideoColorYUV2ABGR420_W(
	mlib_u8 *abgr,
	const mlib_u8 *y_inp,
	const mlib_u8 *u_inp,
	const mlib_u8 *v_inp,
	mlib_s32 wd,
	mlib_s32 ht,
	mlib_s32 abgr_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride,
	mlib_s32 left,
	mlib_s32 top,
	mlib_s32 right,
	mlib_s32 bottom)
{
	/* variables in C version */
	mlib_s32 i, j;
	const mlib_u8 *say1, *say2, *sau, *sav, *sly1, *sly2, *slu, *slv;
	mlib_u8 *da1, *dl1, *da2, *dl2;

	mlib_s32 sf01, sf02, sfr, sfg, sfb;
	mlib_s32 BBi = 554;
	mlib_s32 RRi = 446;

	mlib_u8 *const pClip = (mlib_u8 *const)mlib_tClip + 288 * 2;

	/* variables for SSE2 version */
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

	mlib_s32 iW;
	mlib_s32 iu, iv, ig, ir, ib, iTemp;
	mlib_s32 iu0, iu1, iv1, iv2;
	__m128i x_u0, x_u1, x_v1, x_v2, x_temp, x_out, x_bak, x_temp1;
	__m128i x_u, x_v, x_y1, x_y2, x_y3, x_y4;
	__m128i x_r1, x_r2, x_r3, x_r4, x_g1, x_g2, x_g3, x_g4;
	__m128i x_b1, x_b2, x_b3, x_b4, x_r, x_g, x_b;
	__m128i x_zbl, x_zbh, x_grl, x_grh, x_bgrh, x_bgrl;
	__m128i *px_y1, *px_y2;
	__m64 *pm_u, *pm_v;
	mlib_u8 *pd1, *pd2;

	/* C implementation */
	if (wd <= 0 || ht <= 0)
		return (MLIB_FAILURE);

	if (wd & 7 || ht & 1 || abgr_stride & 7 || uv_stride & 3 ||
		y_stride & 7)
		return (MLIB_FAILURE);

	if (left < 0 || top < 0 || right <= left || bottom <= top ||
	    wd < right || ht < bottom)
		return (MLIB_FAILURE);

	abgr += left * 4 + top * abgr_stride;
	y_inp += left + top * y_stride;
	u_inp += (left >> 1) + (top >> 1) * uv_stride;
	v_inp += (left >> 1) + (top >> 1) * uv_stride;

	wd = ((right & ~1) - left) >> 1;
	ht = ((bottom & ~1) - top) >> 1;

	if (top & 1) {
		say1 = y_inp;
		sau = u_inp;
		sav = v_inp;
		da1 = abgr;

		if (left & 1) {
			CALC1(abgr, y_inp[0], u_inp[0], v_inp[0]);
			say1++;
			sau++;
			sav++;
			da1 += 4;
		}

		for (i = 0; i < wd; i++) {
			CALC2(da1, (da1 + 4), say1[0], say1[1], sau[0], sav[0]);
			say1 += 2;
			sau++;
			sav++;
			da1 += 8;
		}

		if (right & 1) {
			CALC1(da1, say1[0], sau[0], sav[0]);
		}

		abgr += abgr_stride;
		y_inp += y_stride;
		u_inp += uv_stride;
		v_inp += uv_stride;
	}

	sly1 = say1 = y_inp;
	sly2 = say2 = y_inp + y_stride;
	slu = sau = u_inp;
	slv = sav = v_inp;
	dl1 = da1 = abgr;
	dl2 = da2 = abgr + abgr_stride;

	for (j = 0; j < ht; j++) {
		if (left & 1) {
			CALC2(da1, da2, say1[0], say2[0], sau[0], sav[0]);
			say1++;
			say2++;
			sau++;
			sav++;
			da1 += 4;
			da2 += 4;
		}

		/* SSE2 implementation */
		px_y1 = (__m128i*)say1;
		px_y2 = (__m128i*)say2;
		pm_u = (__m64*)sau;
		pm_v = (__m64*)sav;
		pd1 = da1;
		pd2 = da2;
		iW = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		/* 32 pixels */
		for (; iW <= wd - 8; iW += 8) {
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
			x_temp1 = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			SAVE_ABGR2(x_temp, pd1);
			SAVE_ABGR2(x_temp1, pd2);

			x_bgrl = _mm_unpacklo_epi16(x_zbh, x_grh);
			x_bgrh = _mm_unpackhi_epi16(x_zbh, x_grh);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			SAVE_ABGR2(x_temp, pd1);
			SAVE_ABGR2(x_temp1, pd2);

			/* create bgr sequences : x_[b1/r1/g1] */
			x_zbl = _mm_unpacklo_epi8(x_zero, x_b1);
			x_zbh = _mm_unpackhi_epi8(x_zero, x_b1);
			x_grl = _mm_unpacklo_epi8(x_g1, x_r1);
			x_grh = _mm_unpackhi_epi8(x_g1, x_r1);

			x_bgrl = _mm_unpacklo_epi16(x_zbl, x_grl);
			x_bgrh = _mm_unpackhi_epi16(x_zbl, x_grl);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			SAVE_ABGR2(x_temp, pd1);
			SAVE_ABGR2(x_temp1, pd2);

			x_bgrl = _mm_unpacklo_epi16(x_zbh, x_grh);
			x_bgrh = _mm_unpackhi_epi16(x_zbh, x_grh);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			SAVE_ABGR2(x_temp, pd1);
			SAVE_ABGR2(x_temp1, pd2);
		}

		/* 16 pixels */
		if (iW <= wd - 4) {
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
			x_temp1 = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			SAVE_ABGR2(x_temp, pd1);
			SAVE_ABGR2(x_temp1, pd2);

			x_bgrl = _mm_unpacklo_epi16(x_zbh, x_grh);
			x_bgrh = _mm_unpackhi_epi16(x_zbh, x_grh);
			x_temp = _mm_unpacklo_epi64(x_bgrl, x_bgrh);
			x_temp1 = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			SAVE_ABGR2(x_temp, pd1);
			SAVE_ABGR2(x_temp1, pd2);

			iW += 4;
		}

		/* 8 pixels */
		if (iW <= wd - 2) {
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
			x_temp1 = _mm_unpackhi_epi64(x_bgrl, x_bgrh);
			SAVE_ABGR2(x_temp, pd1);
			SAVE_ABGR2(x_temp1, pd2);

			iW += 2;
		}

		/* 4 pixels */
		if (iW <= wd - 1) {
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

			/* pre-calc d[r/g/b] */
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
			/* bgr bgr bgr bgr */
			x_zbl = _mm_unpacklo_epi8(x_zero, x_b);
			x_grl = _mm_unpacklo_epi8(x_g, x_r);

			/* lower half of dzbl & dgrl */
			x_bgrl = _mm_unpacklo_epi16(x_zbl, x_grl);
			_mm_storel_epi64((__m128i*)pd1, x_bgrl);
			_mm_storeh_epi64((__m128i*)pd2, x_bgrl);

			pd1 += 8;
			pd2 += 8;

			iW += 1;
		}

		/* C implementation */
		say1 += 2 * wd;
		say2 += 2 * wd;
		sau += wd;
		sav += wd;
		da1 += 8 * wd;
		da2 += 8 * wd;
		if (right & 1) {
			CALC2(da1, da2, say1[0], say2[0], sau[0], sav[0]);
		}

		sly1 = say1 = ((mlib_u8 *)sly1 + 2 * y_stride);
		sly2 = say2 = ((mlib_u8 *)sly2 + 2 * y_stride);
		slu = sau = ((mlib_u8 *)slu + uv_stride);
		slv = sav = ((mlib_u8 *)slv + uv_stride);
		dl1 = da1 = ((mlib_u8 *)dl1 + 2 * abgr_stride);
		dl2 = da2 = ((mlib_u8 *)dl2 + 2 * abgr_stride);
	}

	if (bottom & 1) {
		if (left & 1) {
			CALC1(da1, say1[0], sau[0], sav[0]);
			say1++;
			sau++;
			sav++;
			da1 += 4;
		}

		for (i = 0; i < wd; i++) {
			CALC2(da1, (da1 + 4), say1[0], say1[1], sau[0], sav[0]);
			say1 += 2;
			sau++;
			sav++;
			da1 += 8;
		}

		if (right & 1) {
			CALC1(da1, say1[0], sau[0], sav[0]);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
