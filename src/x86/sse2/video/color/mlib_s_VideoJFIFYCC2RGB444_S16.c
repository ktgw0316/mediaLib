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

#pragma ident	"@(#)mlib_s_VideoJFIFYCC2RGB444_S16.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorJFIFYCC2RGB444_S16 - color conversion from JFIF YCbCr
 *                                          12-bit space to RGB 12-bit space
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorJFIFYCC2RGB444_S16(mlib_s16       *rgb,
 *                                                    const mlib_s16 *y,
 *                                                    const mlib_s16 *cb,
 *                                                    const mlib_s16 *cr,
 *                                                    mlib_s32       n)
 *
 * ARGUMENTS
 *      rgb       Pointer to RGB multicomponent row, 8-byte aligned
 *      y         Pointer to Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      n         Length of y, cb, cr arrays
 *                Note: length of rgb array must be 3*n
 *
 * DESCRIPTION
 *
 *        R  = Y                         + 1.40200 * (Cr - 2048)
 *        G  = Y - 0.34414 * (Cb - 2048) - 0.71414 * (Cr - 2048)
 *        B  = Y + 1.77200 * (Cb - 2048)
 *
 *        Y  = y[i], Cr = cr[i], Cb = cb[i]
 *        rgb[3*i] = R, rgb[3*i+1] = G, rgb[3*i+2] = B, 0 <= i < n
 *
 * RESTRICTION
 *
 *        YCbCr component values must be in [0, 4095] range.
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

#pragma weak mlib_VideoColorJFIFYCC2RGB444_S16 = \
	__mlib_VideoColorJFIFYCC2RGB444_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorJFIFYCC2RGB444_S16)
	mlib_VideoColorJFIFYCC2RGB444_S16
	__attribute__((weak, alias("__mlib_VideoColorJFIFYCC2RGB444_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	PACK_RGB1(x_rgb)	\
{	\
	x_t1 = _mm_and_si128(x_rgb, x_mask1);	\
	x_t2 = _mm_srli_si128(x_rgb, 2);	\
	x_t2 = _mm_and_si128(x_t2, x_mask2);	\
	x_t1 = _mm_or_si128(x_t1, x_t2);	\
	_mm_storeu_si128((__m128i *)prgb, x_t1);	\
	prgb += 6;	\
}

#define	PACK_RGB1_aligned(x_rgb)	\
{	\
	x_t1 = _mm_and_si128(x_rgb, x_mask1);	\
	x_t2 = _mm_srli_si128(x_rgb, 2);	\
	x_t2 = _mm_and_si128(x_t2, x_mask2);	\
	x_t1 = _mm_or_si128(x_t1, x_t2);	\
	_mm_storeu_si128((__m128i *)prgb, x_t1);	\
	prgb += 6;	\
}

#define	PACK_RGB2(x_rgb)	\
{	\
	mlib_s32 *pirgb = (mlib_s32 *)(&x_rgb);	\
	*((mlib_s32 *)prgb) = *pirgb;	\
	prgb += 2;	\
	pirgb ++;	\
	*prgb = *((mlib_s16 *)pirgb);	\
	prgb ++;	\
	pirgb ++;	\
	*((mlib_s32 *)prgb) = *pirgb;	\
	prgb += 2;	\
	pirgb ++;	\
	*prgb = *((mlib_s16 *)pirgb);	\
	prgb ++;	\
}

#define	SCALE	(mlib_f32)0x100000
#define	SAT	(mlib_f32)0x7FF80000
#define	MLIB_U12_MAX	((1 << 12) - 1)
#define	MLIB_U12_MIN	0

#ifdef MLIB_USE_FTOI_CLAMPING

#define	CLAMP_U12(src)	(((mlib_s32)src >> 20) + 0x800)

#else /* MLIB_USE_FTOI_CLAMPING */

#define	CLAMP_U12(src)                                                    \
	(((mlib_d64)src >=                                                \
		MLIB_S32_MAX) ? MLIB_U12_MAX : (((mlib_d64)src <          \
		MLIB_S32_MIN) ? MLIB_U12_MIN : (((mlib_s32)src >> 20) +   \
		0x800)))
/* (mlib_d64) - krakatoa bug */

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */
mlib_status
mlib_VideoColorJFIFYCC2RGB444_S16_aligned(
	mlib_s16 *rgb,
	const mlib_s16 *y,
	const mlib_s16 *cb,
	const mlib_s16 *cr,
	mlib_s32 n);

mlib_status
mlib_VideoColorJFIFYCC2RGB444_S16_naligned(
	mlib_s16 *rgb,
	const mlib_s16 *y,
	const mlib_s16 *cb,
	const mlib_s16 *cr,
	mlib_s32 n);

mlib_status
__mlib_VideoColorJFIFYCC2RGB444_S16(
	mlib_s16 *rgb,
	const mlib_s16 *y,
	const mlib_s16 *cb,
	const mlib_s16 *cr,
	mlib_s32 n)
{
	if (rgb == NULL || y == NULL || cb == NULL || cr == NULL)
		return (MLIB_NULLPOINTER);

	if (n <= 0)
		return (MLIB_FAILURE);

	if (0 == (0xf & ((mlib_addr)y | (mlib_addr)cb |
		(mlib_addr)cr | (mlib_addr)rgb))) {
	    return mlib_VideoColorJFIFYCC2RGB444_S16_aligned(
		rgb, y, cb, cr, n);
	} else {
	    return mlib_VideoColorJFIFYCC2RGB444_S16_naligned(
		rgb, y, cb, cr, n);
	}
}

mlib_status
mlib_VideoColorJFIFYCC2RGB444_S16_aligned(
	mlib_s16 *rgb,
	const mlib_s16 *y,
	const mlib_s16 *cb,
	const mlib_s16 *cr,
	mlib_s32 n)
{
	/* 0 & 1.402*16384 */
	const __m128i x_c1 = _mm_setr_epi16(0, 22970, 0, 22970,
		0, 22970, 0, 22970);

	/* -0.34414*16384 & -0.71414*16384 */
	const __m128i x_c2 = _mm_setr_epi16(-5638, -11700, -5638, -11700,
		-5638, -11700, -5638, -11700);

	/* 1.772*16384 & 0 */
	const __m128i x_c3 = _mm_setr_epi16(29032, 0, 29032, 0,
		29032, 0, 29032, 0);

	const __m128i x_coff = _mm_set1_epi16(2048);
	const __m128i x_cps1 = _mm_set1_epi32(0x8000);
	const __m128i x_cps2 = _mm_set1_epi16(0x8000);
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask1 = _mm_setr_epi32(0xffffffff, 0xffff, 0, 0);
	const __m128i x_mask2 = _mm_setr_epi32(0, 0xffff0000, 0xffffffff, 0);

	/* __m128i variables */
	__m128i x_y, x_cb, x_cr, x_r, x_g, x_b, x_y1, x_y2;
	__m128i x_r1, x_r2, x_g1, x_g2, x_b1, x_b2, x_t1, x_t2;
	__m128i x_rgbl, x_rgbh, x_rgl, x_rgh, x_bbl, x_bbh;
	__m128i x_cbcr1, x_cbcr2;

	/* pointers */
	__m128i *px_y, *px_cb, *px_cr;
	mlib_s16 *prgb;

	/* other var */
	mlib_d64 fr, fg, fb, fy, fcb, fcr;
	mlib_s32 i;

	px_y = (__m128i *)y;
	px_cb = (__m128i *)cb;
	px_cr = (__m128i *)cr;
	prgb = rgb;
	i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i <= n - 16; i += 8)	{
		x_y = _mm_load_si128(px_y);
		x_y1 = _mm_unpacklo_epi16(x_y, x_zero);
		x_y1 = _mm_slli_epi32(x_y1, 4);
		x_y2 = _mm_unpackhi_epi16(x_y, x_zero);
		x_y2 = _mm_slli_epi32(x_y2, 4);
		px_y++;
		x_cb = _mm_load_si128(px_cb);
		x_cb = _mm_sub_epi16(x_cb, x_coff);
		px_cb++;
		x_cr = _mm_load_si128(px_cr);
		x_cr = _mm_sub_epi16(x_cr, x_coff);
		px_cr++;
		x_cbcr1 = _mm_unpacklo_epi16(x_cb, x_cr);
		x_cbcr2 = _mm_unpackhi_epi16(x_cb, x_cr);

		/* calc r/g/b */
		x_t1 = _mm_madd_epi16(x_cbcr1, x_c1);
		x_t1 = _mm_srai_epi32(x_t1, 10);
		x_r1 = _mm_add_epi32(x_t1, x_y1);
		x_t1 = _mm_madd_epi16(x_cbcr1, x_c2);
		x_t1 = _mm_srai_epi32(x_t1, 10);
		x_g1 = _mm_add_epi32(x_t1, x_y1);
		x_t1 = _mm_madd_epi16(x_cbcr1, x_c3);
		x_t1 = _mm_srai_epi32(x_t1, 10);
		x_b1 = _mm_add_epi32(x_t1, x_y1);

		x_t2 = _mm_madd_epi16(x_cbcr2, x_c1);
		x_t2 = _mm_srai_epi32(x_t2, 10);
		x_r2 = _mm_add_epi32(x_t2, x_y2);
		x_t2 = _mm_madd_epi16(x_cbcr2, x_c2);
		x_t2 = _mm_srai_epi32(x_t2, 10);
		x_g2 = _mm_add_epi32(x_t2, x_y2);
		x_t2 = _mm_madd_epi16(x_cbcr2, x_c3);
		x_t2 = _mm_srai_epi32(x_t2, 10);
		x_b2 = _mm_add_epi32(x_t2, x_y2);

		/* signed pack & shift */
		x_r1 = _mm_sub_epi32(x_r1, x_cps1);
		x_r2 = _mm_sub_epi32(x_r2, x_cps1);
		x_r = _mm_packs_epi32(x_r1, x_r2);
		x_r = _mm_add_epi16(x_r, x_cps2);
		x_r = _mm_srli_epi16(x_r, 4);

		x_g1 = _mm_sub_epi32(x_g1, x_cps1);
		x_g2 = _mm_sub_epi32(x_g2, x_cps1);
		x_g = _mm_packs_epi32(x_g1, x_g2);
		x_g = _mm_add_epi16(x_g, x_cps2);
		x_g = _mm_srli_epi16(x_g, 4);

		x_b1 = _mm_sub_epi32(x_b1, x_cps1);
		x_b2 = _mm_sub_epi32(x_b2, x_cps1);
		x_b = _mm_packs_epi32(x_b1, x_b2);
		x_b = _mm_add_epi16(x_b, x_cps2);
		x_b = _mm_srli_epi16(x_b, 4);

		/* create rgb sequences */
		x_rgl = _mm_unpacklo_epi16(x_r, x_g);
		x_rgh = _mm_unpackhi_epi16(x_r, x_g);
		x_bbl = _mm_unpacklo_epi16(x_b, x_b);
		x_bbh = _mm_unpackhi_epi16(x_b, x_b);

		/* save */
		x_rgbl = _mm_unpacklo_epi32(x_rgl, x_bbl);
		PACK_RGB1_aligned(x_rgbl);

		x_rgbh = _mm_unpackhi_epi32(x_rgl, x_bbl);
		PACK_RGB1(x_rgbh);

		x_rgbl = _mm_unpacklo_epi32(x_rgh, x_bbh);
		PACK_RGB1(x_rgbl);

		x_rgbh = _mm_unpackhi_epi32(x_rgh, x_bbh);
		PACK_RGB1(x_rgbh);
	}

	if (i <= (n - 8)) {
		x_y = _mm_loadu_si128(px_y);
		x_y1 = _mm_unpacklo_epi16(x_y, x_zero);
		x_y1 = _mm_slli_epi32(x_y1, 4);
		x_y2 = _mm_unpackhi_epi16(x_y, x_zero);
		x_y2 = _mm_slli_epi32(x_y2, 4);
		px_y++;
		x_cb = _mm_loadu_si128(px_cb);
		x_cb = _mm_sub_epi16(x_cb, x_coff);
		px_cb++;
		x_cr = _mm_loadu_si128(px_cr);
		x_cr = _mm_sub_epi16(x_cr, x_coff);
		px_cr++;
		x_cbcr1 = _mm_unpacklo_epi16(x_cb, x_cr);
		x_cbcr2 = _mm_unpackhi_epi16(x_cb, x_cr);

		/* calc r/g/b */
		x_t1 = _mm_madd_epi16(x_cbcr1, x_c1);
		x_t1 = _mm_srai_epi32(x_t1, 10);
		x_r1 = _mm_add_epi32(x_t1, x_y1);
		x_t1 = _mm_madd_epi16(x_cbcr1, x_c2);
		x_t1 = _mm_srai_epi32(x_t1, 10);
		x_g1 = _mm_add_epi32(x_t1, x_y1);
		x_t1 = _mm_madd_epi16(x_cbcr1, x_c3);
		x_t1 = _mm_srai_epi32(x_t1, 10);
		x_b1 = _mm_add_epi32(x_t1, x_y1);

		x_t2 = _mm_madd_epi16(x_cbcr2, x_c1);
		x_t2 = _mm_srai_epi32(x_t2, 10);
		x_r2 = _mm_add_epi32(x_t2, x_y2);
		x_t2 = _mm_madd_epi16(x_cbcr2, x_c2);
		x_t2 = _mm_srai_epi32(x_t2, 10);
		x_g2 = _mm_add_epi32(x_t2, x_y2);
		x_t2 = _mm_madd_epi16(x_cbcr2, x_c3);
		x_t2 = _mm_srai_epi32(x_t2, 10);
		x_b2 = _mm_add_epi32(x_t2, x_y2);

		/* signed pack & shift */
		x_r1 = _mm_sub_epi32(x_r1, x_cps1);
		x_r2 = _mm_sub_epi32(x_r2, x_cps1);
		x_r = _mm_packs_epi32(x_r1, x_r2);
		x_r = _mm_add_epi16(x_r, x_cps2);
		x_r = _mm_srli_epi16(x_r, 4);

		x_g1 = _mm_sub_epi32(x_g1, x_cps1);
		x_g2 = _mm_sub_epi32(x_g2, x_cps1);
		x_g = _mm_packs_epi32(x_g1, x_g2);
		x_g = _mm_add_epi16(x_g, x_cps2);
		x_g = _mm_srli_epi16(x_g, 4);

		x_b1 = _mm_sub_epi32(x_b1, x_cps1);
		x_b2 = _mm_sub_epi32(x_b2, x_cps1);
		x_b = _mm_packs_epi32(x_b1, x_b2);
		x_b = _mm_add_epi16(x_b, x_cps2);
		x_b = _mm_srli_epi16(x_b, 4);

		/* create rgb sequences */
		x_rgl = _mm_unpacklo_epi16(x_r, x_g);
		x_rgh = _mm_unpackhi_epi16(x_r, x_g);
		x_bbl = _mm_unpacklo_epi16(x_b, x_b);
		x_bbh = _mm_unpackhi_epi16(x_b, x_b);

		/* save */
		x_rgbl = _mm_unpacklo_epi32(x_rgl, x_bbl);
		PACK_RGB1_aligned(x_rgbl);

		x_rgbh = _mm_unpackhi_epi32(x_rgl, x_bbl);
		PACK_RGB1(x_rgbh);

		x_rgbl = _mm_unpacklo_epi32(x_rgh, x_bbh);
		PACK_RGB1(x_rgbl);

		x_rgbh = _mm_unpackhi_epi32(x_rgh, x_bbh);
		PACK_RGB2(x_rgbh);

		i += 8;
	}

	if (i <= (n - 4)) {
		x_y = _mm_loadl_epi64(px_y);
		x_y1 = _mm_unpacklo_epi16(x_y, x_zero);
		x_y1 = _mm_slli_epi32(x_y1, 4);
		px_y = (__m128i *)(((__m64 *)px_y) + 1);
		x_cb = _mm_loadl_epi64(px_cb);
		x_cb = _mm_sub_epi16(x_cb, x_coff);
		px_cb = (__m128i *)(((__m64 *)px_cb) + 1);
		x_cr = _mm_loadl_epi64(px_cr);
		x_cr = _mm_sub_epi16(x_cr, x_coff);
		px_cr = (__m128i *)(((__m64 *)px_cr) + 1);
		x_cbcr1 = _mm_unpacklo_epi16(x_cb, x_cr);

		/* calc r/g/b */
		x_t1 = _mm_madd_epi16(x_cbcr1, x_c1);
		x_t1 = _mm_srai_epi32(x_t1, 10);
		x_r1 = _mm_add_epi32(x_t1, x_y1);
		x_t1 = _mm_madd_epi16(x_cbcr1, x_c2);
		x_t1 = _mm_srai_epi32(x_t1, 10);
		x_g1 = _mm_add_epi32(x_t1, x_y1);
		x_t1 = _mm_madd_epi16(x_cbcr1, x_c3);
		x_t1 = _mm_srai_epi32(x_t1, 10);
		x_b1 = _mm_add_epi32(x_t1, x_y1);

		/* signed pack & shift */
		x_r1 = _mm_sub_epi32(x_r1, x_cps1);
		x_r = _mm_packs_epi32(x_r1, x_zero);
		x_r = _mm_add_epi16(x_r, x_cps2);
		x_r = _mm_srli_epi16(x_r, 4);

		x_g1 = _mm_sub_epi32(x_g1, x_cps1);
		x_g = _mm_packs_epi32(x_g1, x_zero);
		x_g = _mm_add_epi16(x_g, x_cps2);
		x_g = _mm_srli_epi16(x_g, 4);

		x_b1 = _mm_sub_epi32(x_b1, x_cps1);
		x_b = _mm_packs_epi32(x_b1, x_zero);
		x_b = _mm_add_epi16(x_b, x_cps2);
		x_b = _mm_srli_epi16(x_b, 4);

		/* create rgb sequences */
		x_rgl = _mm_unpacklo_epi16(x_r, x_g);
		x_bbl = _mm_unpacklo_epi16(x_b, x_b);

		/* save */
		x_rgbl = _mm_unpacklo_epi32(x_rgl, x_bbl);
		PACK_RGB1(x_rgbl);

		x_rgbh = _mm_unpackhi_epi32(x_rgl, x_bbl);
		PACK_RGB2(x_rgbh);

		i += 4;
	}

	/* pure C implementation */
	for (; i < n; i++) {
		fy = y[i] * SCALE - SAT;
		fcb = (mlib_d64)((cb[i] - 2048) << 20);
		fcr = (mlib_d64)((cr[i] - 2048) << 20);
		fr = fy + 1.40200f * fcr;
		fg = fy - 0.34414f * fcb - 0.71414f * fcr;
		fb = fy + 1.77200f * fcb;
		rgb[3 * i] = CLAMP_U12(fr);
		rgb[3 * i + 1] = CLAMP_U12(fg);
		rgb[3 * i + 2] = CLAMP_U12(fb);
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoColorJFIFYCC2RGB444_S16_naligned(
	mlib_s16 *rgb,
	const mlib_s16 *y,
	const mlib_s16 *cb,
	const mlib_s16 *cr,
	mlib_s32 n)
{
	/* 0 & 1.402*16384 */
	const __m128i x_c1 = _mm_setr_epi16(0, 22970, 0, 22970,
		0, 22970, 0, 22970);

	/* -0.34414*16384 & -0.71414*16384 */
	const __m128i x_c2 = _mm_setr_epi16(-5638, -11700, -5638, -11700,
		-5638, -11700, -5638, -11700);

	/* 1.772*16384 & 0 */
	const __m128i x_c3 = _mm_setr_epi16(29032, 0, 29032, 0,
		29032, 0, 29032, 0);

	const __m128i x_coff = _mm_set1_epi16(2048);
	const __m128i x_cps1 = _mm_set1_epi32(0x8000);
	const __m128i x_cps2 = _mm_set1_epi16(0x8000);
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask1 = _mm_setr_epi32(0xffffffff, 0xffff, 0, 0);
	const __m128i x_mask2 = _mm_setr_epi32(0, 0xffff0000, 0xffffffff, 0);

	/* __m128i variables */
	__m128i x_y, x_cb, x_cr, x_r, x_g, x_b, x_y1, x_y2;
	__m128i x_r1, x_r2, x_g1, x_g2, x_b1, x_b2, x_t1, x_t2;
	__m128i x_rgbl, x_rgbh, x_rgl, x_rgh, x_bbl, x_bbh;
	__m128i x_cbcr1, x_cbcr2;

	/* pointers */
	__m128i *px_y, *px_cb, *px_cr;
	mlib_s16 *prgb;

	/* other var */
	mlib_d64 fr, fg, fb, fy, fcb, fcr;
	mlib_s32 i;

	px_y = (__m128i *)y;
	px_cb = (__m128i *)cb;
	px_cr = (__m128i *)cr;
	prgb = rgb;
	i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i <= n - 16; i += 8)	{
		x_y = _mm_loadu_si128(px_y);
		x_y1 = _mm_unpacklo_epi16(x_y, x_zero);
		x_y1 = _mm_slli_epi32(x_y1, 4);
		x_y2 = _mm_unpackhi_epi16(x_y, x_zero);
		x_y2 = _mm_slli_epi32(x_y2, 4);
		px_y++;
		x_cb = _mm_loadu_si128(px_cb);
		x_cb = _mm_sub_epi16(x_cb, x_coff);
		px_cb++;
		x_cr = _mm_loadu_si128(px_cr);
		x_cr = _mm_sub_epi16(x_cr, x_coff);
		px_cr++;
		x_cbcr1 = _mm_unpacklo_epi16(x_cb, x_cr);
		x_cbcr2 = _mm_unpackhi_epi16(x_cb, x_cr);

		/* calc r/g/b */
		x_t1 = _mm_madd_epi16(x_cbcr1, x_c1);
		x_t1 = _mm_srai_epi32(x_t1, 10);
		x_r1 = _mm_add_epi32(x_t1, x_y1);
		x_t1 = _mm_madd_epi16(x_cbcr1, x_c2);
		x_t1 = _mm_srai_epi32(x_t1, 10);
		x_g1 = _mm_add_epi32(x_t1, x_y1);
		x_t1 = _mm_madd_epi16(x_cbcr1, x_c3);
		x_t1 = _mm_srai_epi32(x_t1, 10);
		x_b1 = _mm_add_epi32(x_t1, x_y1);

		x_t2 = _mm_madd_epi16(x_cbcr2, x_c1);
		x_t2 = _mm_srai_epi32(x_t2, 10);
		x_r2 = _mm_add_epi32(x_t2, x_y2);
		x_t2 = _mm_madd_epi16(x_cbcr2, x_c2);
		x_t2 = _mm_srai_epi32(x_t2, 10);
		x_g2 = _mm_add_epi32(x_t2, x_y2);
		x_t2 = _mm_madd_epi16(x_cbcr2, x_c3);
		x_t2 = _mm_srai_epi32(x_t2, 10);
		x_b2 = _mm_add_epi32(x_t2, x_y2);

		/* signed pack & shift */
		x_r1 = _mm_sub_epi32(x_r1, x_cps1);
		x_r2 = _mm_sub_epi32(x_r2, x_cps1);
		x_r = _mm_packs_epi32(x_r1, x_r2);
		x_r = _mm_add_epi16(x_r, x_cps2);
		x_r = _mm_srli_epi16(x_r, 4);

		x_g1 = _mm_sub_epi32(x_g1, x_cps1);
		x_g2 = _mm_sub_epi32(x_g2, x_cps1);
		x_g = _mm_packs_epi32(x_g1, x_g2);
		x_g = _mm_add_epi16(x_g, x_cps2);
		x_g = _mm_srli_epi16(x_g, 4);

		x_b1 = _mm_sub_epi32(x_b1, x_cps1);
		x_b2 = _mm_sub_epi32(x_b2, x_cps1);
		x_b = _mm_packs_epi32(x_b1, x_b2);
		x_b = _mm_add_epi16(x_b, x_cps2);
		x_b = _mm_srli_epi16(x_b, 4);

		/* create rgb sequences */
		x_rgl = _mm_unpacklo_epi16(x_r, x_g);
		x_rgh = _mm_unpackhi_epi16(x_r, x_g);
		x_bbl = _mm_unpacklo_epi16(x_b, x_b);
		x_bbh = _mm_unpackhi_epi16(x_b, x_b);

		/* save */
		x_rgbl = _mm_unpacklo_epi32(x_rgl, x_bbl);
		PACK_RGB1(x_rgbl);

		x_rgbh = _mm_unpackhi_epi32(x_rgl, x_bbl);
		PACK_RGB1(x_rgbh);

		x_rgbl = _mm_unpacklo_epi32(x_rgh, x_bbh);
		PACK_RGB1(x_rgbl);

		x_rgbh = _mm_unpackhi_epi32(x_rgh, x_bbh);
		PACK_RGB1(x_rgbh);
	}

	if (i <= (n - 8)) {
		x_y = _mm_loadu_si128(px_y);
		x_y1 = _mm_unpacklo_epi16(x_y, x_zero);
		x_y1 = _mm_slli_epi32(x_y1, 4);
		x_y2 = _mm_unpackhi_epi16(x_y, x_zero);
		x_y2 = _mm_slli_epi32(x_y2, 4);
		px_y++;
		x_cb = _mm_loadu_si128(px_cb);
		x_cb = _mm_sub_epi16(x_cb, x_coff);
		px_cb++;
		x_cr = _mm_loadu_si128(px_cr);
		x_cr = _mm_sub_epi16(x_cr, x_coff);
		px_cr++;
		x_cbcr1 = _mm_unpacklo_epi16(x_cb, x_cr);
		x_cbcr2 = _mm_unpackhi_epi16(x_cb, x_cr);

		/* calc r/g/b */
		x_t1 = _mm_madd_epi16(x_cbcr1, x_c1);
		x_t1 = _mm_srai_epi32(x_t1, 10);
		x_r1 = _mm_add_epi32(x_t1, x_y1);
		x_t1 = _mm_madd_epi16(x_cbcr1, x_c2);
		x_t1 = _mm_srai_epi32(x_t1, 10);
		x_g1 = _mm_add_epi32(x_t1, x_y1);
		x_t1 = _mm_madd_epi16(x_cbcr1, x_c3);
		x_t1 = _mm_srai_epi32(x_t1, 10);
		x_b1 = _mm_add_epi32(x_t1, x_y1);

		x_t2 = _mm_madd_epi16(x_cbcr2, x_c1);
		x_t2 = _mm_srai_epi32(x_t2, 10);
		x_r2 = _mm_add_epi32(x_t2, x_y2);
		x_t2 = _mm_madd_epi16(x_cbcr2, x_c2);
		x_t2 = _mm_srai_epi32(x_t2, 10);
		x_g2 = _mm_add_epi32(x_t2, x_y2);
		x_t2 = _mm_madd_epi16(x_cbcr2, x_c3);
		x_t2 = _mm_srai_epi32(x_t2, 10);
		x_b2 = _mm_add_epi32(x_t2, x_y2);

		/* signed pack & shift */
		x_r1 = _mm_sub_epi32(x_r1, x_cps1);
		x_r2 = _mm_sub_epi32(x_r2, x_cps1);
		x_r = _mm_packs_epi32(x_r1, x_r2);
		x_r = _mm_add_epi16(x_r, x_cps2);
		x_r = _mm_srli_epi16(x_r, 4);

		x_g1 = _mm_sub_epi32(x_g1, x_cps1);
		x_g2 = _mm_sub_epi32(x_g2, x_cps1);
		x_g = _mm_packs_epi32(x_g1, x_g2);
		x_g = _mm_add_epi16(x_g, x_cps2);
		x_g = _mm_srli_epi16(x_g, 4);

		x_b1 = _mm_sub_epi32(x_b1, x_cps1);
		x_b2 = _mm_sub_epi32(x_b2, x_cps1);
		x_b = _mm_packs_epi32(x_b1, x_b2);
		x_b = _mm_add_epi16(x_b, x_cps2);
		x_b = _mm_srli_epi16(x_b, 4);

		/* create rgb sequences */
		x_rgl = _mm_unpacklo_epi16(x_r, x_g);
		x_rgh = _mm_unpackhi_epi16(x_r, x_g);
		x_bbl = _mm_unpacklo_epi16(x_b, x_b);
		x_bbh = _mm_unpackhi_epi16(x_b, x_b);

		/* save */
		x_rgbl = _mm_unpacklo_epi32(x_rgl, x_bbl);
		PACK_RGB1(x_rgbl);

		x_rgbh = _mm_unpackhi_epi32(x_rgl, x_bbl);
		PACK_RGB1(x_rgbh);

		x_rgbl = _mm_unpacklo_epi32(x_rgh, x_bbh);
		PACK_RGB1(x_rgbl);

		x_rgbh = _mm_unpackhi_epi32(x_rgh, x_bbh);
		PACK_RGB2(x_rgbh);

		i += 8;
	}

	if (i <= (n - 4)) {
		x_y = _mm_loadl_epi64(px_y);
		x_y1 = _mm_unpacklo_epi16(x_y, x_zero);
		x_y1 = _mm_slli_epi32(x_y1, 4);
		px_y = (__m128i *)(((__m64 *)px_y) + 1);
		x_cb = _mm_loadl_epi64(px_cb);
		x_cb = _mm_sub_epi16(x_cb, x_coff);
		px_cb = (__m128i *)(((__m64 *)px_cb) + 1);
		x_cr = _mm_loadl_epi64(px_cr);
		x_cr = _mm_sub_epi16(x_cr, x_coff);
		px_cr = (__m128i *)(((__m64 *)px_cr) + 1);
		x_cbcr1 = _mm_unpacklo_epi16(x_cb, x_cr);

		/* calc r/g/b */
		x_t1 = _mm_madd_epi16(x_cbcr1, x_c1);
		x_t1 = _mm_srai_epi32(x_t1, 10);
		x_r1 = _mm_add_epi32(x_t1, x_y1);
		x_t1 = _mm_madd_epi16(x_cbcr1, x_c2);
		x_t1 = _mm_srai_epi32(x_t1, 10);
		x_g1 = _mm_add_epi32(x_t1, x_y1);
		x_t1 = _mm_madd_epi16(x_cbcr1, x_c3);
		x_t1 = _mm_srai_epi32(x_t1, 10);
		x_b1 = _mm_add_epi32(x_t1, x_y1);

		/* signed pack & shift */
		x_r1 = _mm_sub_epi32(x_r1, x_cps1);
		x_r = _mm_packs_epi32(x_r1, x_zero);
		x_r = _mm_add_epi16(x_r, x_cps2);
		x_r = _mm_srli_epi16(x_r, 4);

		x_g1 = _mm_sub_epi32(x_g1, x_cps1);
		x_g = _mm_packs_epi32(x_g1, x_zero);
		x_g = _mm_add_epi16(x_g, x_cps2);
		x_g = _mm_srli_epi16(x_g, 4);

		x_b1 = _mm_sub_epi32(x_b1, x_cps1);
		x_b = _mm_packs_epi32(x_b1, x_zero);
		x_b = _mm_add_epi16(x_b, x_cps2);
		x_b = _mm_srli_epi16(x_b, 4);

		/* create rgb sequences */
		x_rgl = _mm_unpacklo_epi16(x_r, x_g);
		x_bbl = _mm_unpacklo_epi16(x_b, x_b);

		/* save */
		x_rgbl = _mm_unpacklo_epi32(x_rgl, x_bbl);
		PACK_RGB1(x_rgbl);

		x_rgbh = _mm_unpackhi_epi32(x_rgl, x_bbl);
		PACK_RGB2(x_rgbh);

		i += 4;
	}

	/* pure C implementation */
	for (; i < n; i++) {
		fy = y[i] * SCALE - SAT;
		fcb = (mlib_d64)((cb[i] - 2048) << 20);
		fcr = (mlib_d64)((cr[i] - 2048) << 20);
		fr = fy + 1.40200f * fcr;
		fg = fy - 0.34414f * fcb - 0.71414f * fcr;
		fb = fy + 1.77200f * fcb;
		rgb[3 * i] = CLAMP_U12(fr);
		rgb[3 * i + 1] = CLAMP_U12(fg);
		rgb[3 * i + 2] = CLAMP_U12(fb);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
