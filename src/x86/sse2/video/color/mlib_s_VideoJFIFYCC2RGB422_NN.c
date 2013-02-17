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

#pragma ident	"@(#)mlib_s_VideoJFIFYCC2RGB422_NN.c	9.7	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorJFIFYCC2RGB422_Nearest - color conversion with
 *                                              422 upsampling
 *                                              with nearest filter.
 *                                              Only Cr and Cb components are
 *                                              under upsampling
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorJFIFYCC2RGB422_Nearest(mlib_u8       *rgb,
 *                                                        const mlib_u8 *y,
 *                                                        const mlib_u8 *cb,
 *                                                        const mlib_u8 *cr,
 *                                                        mlib_s32      n)
 *
 * ARGUMENTS
 *      rgb       Pointer to RGB multicomponent row, 8-byte aligned
 *      y         Pointer to Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      n         Length of y array
 *                Note: n must be even,
 *                      length of cb, cr arrays must be n/2
 *                      length of rgb array must be 3*n
 *
 * DESCRIPTION
 *      mlib_VideoColorJFIFYCC2RGB422_Nearest:
 *        composition of mlib_VideoUpSample422_Nearest with
 *                       mlib_VideoColorJFIFYCC2RGB444
 *
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

#pragma weak mlib_VideoColorJFIFYCC2RGB422_Nearest = \
	__mlib_VideoColorJFIFYCC2RGB422_Nearest

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorJFIFYCC2RGB422_Nearest)
	mlib_VideoColorJFIFYCC2RGB422_Nearest
	__attribute__((weak,
	alias("__mlib_VideoColorJFIFYCC2RGB422_Nearest")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
/* save rgb from sequence: rgb* rgb* rgb* rgb* */
#define	PACK_RGB1(rgb)	\
{							\
	mlib_u8 *pout = (mlib_u8*)(&rgb);		\
	*((mlib_s32*)prgb) = *((mlib_s32*)pout);	\
	prgb += 3;					\
	pout += 4;					\
	*((mlib_s32*)prgb) = *((mlib_s32*)pout);	\
	prgb += 3;					\
	pout += 4;					\
	*((mlib_s32*)prgb) = *((mlib_s32*)pout);	\
	prgb += 3;					\
	pout += 4;					\
	*((mlib_s32*)prgb) = *((mlib_s32*)pout);	\
	prgb += 3;					\
}

#define	PACK_RGB2(rgb)	\
{							\
	mlib_u8 *pout = (mlib_u8*)(&rgb);		\
	*((mlib_s32*)prgb) = *((mlib_s32*)pout);	\
	prgb += 3;					\
	pout += 4;					\
	*((mlib_s32*)prgb) = *((mlib_s32*)pout);	\
	prgb += 3;					\
	pout += 4;					\
	*((mlib_s32*)prgb) = *((mlib_s32*)pout);	\
	prgb += 3;					\
	pout += 4;					\
	*((mlib_s16*)prgb) = *((mlib_s16*)pout);	\
	prgb += 2;					\
	pout += 2;					\
	*((mlib_u8*)prgb) = *((mlib_u8*)pout);		\
	prgb += 1;					\
}

#define	CLAMP_U8(f, c)		\
	{	\
		if ((f) < 0)	\
		{	\
			c = 0;	\
		}	\
		else if ((f) < 255)	\
		{	\
			c = (f);	\
		}	\
		else	\
			c = 255;	\
	}

/* *********************************************************** */
mlib_status
mlib_VideoColorJFIFYCC2RGB422_Nearest_aligned(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *cb,
	const mlib_u8 *cr,
	mlib_s32 n);

mlib_status
mlib_VideoColorJFIFYCC2RGB422_Nearest_naligned(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *cb,
	const mlib_u8 *cr,
	mlib_s32 n);

mlib_status
__mlib_VideoColorJFIFYCC2RGB422_Nearest(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *cb,
	const mlib_u8 *cr,
	mlib_s32 n)
{
	if (rgb == NULL || y == NULL || cb == NULL || cr == NULL)
		return (MLIB_NULLPOINTER);

	if (n <= 0)
		return (MLIB_FAILURE);

	if (0 == (0xf & ((mlib_addr)y | (mlib_addr)cb | (mlib_addr)cr))) {
	    return mlib_VideoColorJFIFYCC2RGB422_Nearest_aligned(
		rgb, y, cb, cr, n);
	} else {
	    return mlib_VideoColorJFIFYCC2RGB422_Nearest_naligned(
		rgb, y, cb, cr, n);
	}
}

mlib_status
mlib_VideoColorJFIFYCC2RGB422_Nearest_aligned(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *cb,
	const mlib_u8 *cr,
	mlib_s32 n)
{
	/* 1.402 * 8192 */
	const __m128i x_c13 = _mm_set1_epi16(0x2cdd);
	const mlib_s32 c13 = 0x2cdd;

	/* abs(-0.34414) * 8192 */
	const __m128i x_c22 = _mm_set1_epi16(0xb03);
	const mlib_s32 c22 = 0xb03;

	/* abs(-0.71414) * 8192 */
	const __m128i x_c23 = _mm_set1_epi16(0x16da);
	const mlib_s32 c23 = 0x16da;

	/* 1.772 * 8192 */
	const __m128i x_c32 = _mm_set1_epi16(0x38b4);
	const mlib_s32 c32 = 0x38b4;

	/* -179.456 * 32 */
	const __m128i x_coff0 = _mm_set1_epi16(0xe991);
	const mlib_s32 coff0 = (mlib_s32)0xffffe991;

	/* 135.45984 * 32 */
	const __m128i x_coff1 = _mm_set1_epi16(0x10ef);
	const mlib_s32 coff1 = 0x10ef;

	/* -226.816 * 32 */
	const __m128i x_coff2 = _mm_set1_epi16(0xe3a6);
	const mlib_s32 coff2 = (mlib_s32)0xffffe3a6;

	const __m128i x_zero = _mm_setzero_si128();

	/* __m128i variables */
	__m128i x_y, x_cb, x_cr, x_temp;
	__m128i x_rh, x_rl, x_gh, x_gl, x_bh, x_bl;
	__m128i x_y1, x_cb1, x_cr1, x_y2, x_cb2, x_cr2, x_y3, x_y4;
	__m128i x_r1, x_r2, x_g1, x_g2, x_b1, x_b2;
	__m128i x_r3, x_r4, x_g3, x_g4, x_b3, x_b4;
	__m128i x_rgbl, x_rgbh, x_rgl, x_rgh, x_bzl, x_bzh;

	/* pointers */
	__m128i *px_y, *px_cb, *px_cr;
	mlib_u8 *prgb;

	/* other var */
	mlib_s32 i, iTemp, iy1, iy2, icb1, icr1;
	mlib_s32 ir, ig, ib, ir1, ig1, ib1, ir2, ig2, ib2;

	px_y = (__m128i *)y;
	px_cb = (__m128i *)cb;
	px_cr = (__m128i *)cr;
	prgb = rgb;
	i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i <= n - 64; i += 32) {
		x_y = _mm_load_si128(px_y);
		px_y++;
		x_y1 = _mm_unpacklo_epi8(x_y, x_zero);
		x_y2 = _mm_unpackhi_epi8(x_y, x_zero);
		x_y = _mm_load_si128(px_y);
		px_y++;
		x_y3 = _mm_unpacklo_epi8(x_y, x_zero);
		x_y4 = _mm_unpackhi_epi8(x_y, x_zero);
		x_cb = _mm_load_si128(px_cb);
		px_cb++;
		x_cb1 = _mm_unpacklo_epi8(x_zero, x_cb);
		x_cb2 = _mm_unpackhi_epi8(x_zero, x_cb);
		x_cr = _mm_load_si128(px_cr);
		px_cr++;
		x_cr1 = _mm_unpacklo_epi8(x_zero, x_cr);
		x_cr2 = _mm_unpackhi_epi8(x_zero, x_cr);

		/* lower half */
		x_temp = _mm_mulhi_epu16(x_cr1, x_c13);
		x_r1 = _mm_add_epi16(x_temp, x_coff0);
		x_rl = _mm_srai_epi16(x_r1, 5);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c22);
		x_g1 = _mm_mulhi_epu16(x_cr1, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g1);
		x_g1 = _mm_sub_epi16(x_coff1, x_temp);
		x_gl = _mm_srai_epi16(x_g1, 5);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c32);
		x_b1 = _mm_add_epi16(x_temp, x_coff2);
		x_bl = _mm_srai_epi16(x_b1, 5);

		/* upper half */
		x_temp = _mm_mulhi_epu16(x_cr2, x_c13);
		x_r2 = _mm_add_epi16(x_temp, x_coff0);
		x_rh = _mm_srai_epi16(x_r2, 5);

		x_temp = _mm_mulhi_epu16(x_cb2, x_c22);
		x_g2 = _mm_mulhi_epu16(x_cr2, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g2);
		x_g2 = _mm_sub_epi16(x_coff1, x_temp);
		x_gh = _mm_srai_epi16(x_g2, 5);

		x_temp = _mm_mulhi_epu16(x_cb2, x_c32);
		x_b2 = _mm_add_epi16(x_temp, x_coff2);
		x_bh = _mm_srai_epi16(x_b2, 5);

		/* get x_r/g/b[1/2/3/4] */
		x_temp = _mm_unpacklo_epi16(x_rl, x_rl);
		x_r1 = _mm_add_epi16(x_y1, x_temp);
		x_temp = _mm_unpackhi_epi16(x_rl, x_rl);
		x_r2 = _mm_add_epi16(x_y2, x_temp);
		x_temp = _mm_unpacklo_epi16(x_rh, x_rh);
		x_r3 = _mm_add_epi16(x_y3, x_temp);
		x_temp = _mm_unpackhi_epi16(x_rh, x_rh);
		x_r4 = _mm_add_epi16(x_y4, x_temp);

		x_temp = _mm_unpacklo_epi16(x_gl, x_gl);
		x_g1 = _mm_add_epi16(x_y1, x_temp);
		x_temp = _mm_unpackhi_epi16(x_gl, x_gl);
		x_g2 = _mm_add_epi16(x_y2, x_temp);
		x_temp = _mm_unpacklo_epi16(x_gh, x_gh);
		x_g3 = _mm_add_epi16(x_y3, x_temp);
		x_temp = _mm_unpackhi_epi16(x_gh, x_gh);
		x_g4 = _mm_add_epi16(x_y4, x_temp);

		x_temp = _mm_unpacklo_epi16(x_bl, x_bl);
		x_b1 = _mm_add_epi16(x_y1, x_temp);
		x_temp = _mm_unpackhi_epi16(x_bl, x_bl);
		x_b2 = _mm_add_epi16(x_y2, x_temp);
		x_temp = _mm_unpacklo_epi16(x_bh, x_bh);
		x_b3 = _mm_add_epi16(x_y3, x_temp);
		x_temp = _mm_unpackhi_epi16(x_bh, x_bh);
		x_b4 = _mm_add_epi16(x_y4, x_temp);

		/* pack */
		x_b1 = _mm_packus_epi16(x_b1, x_b2);
		x_b2 = _mm_packus_epi16(x_b3, x_b4);
		x_r1 = _mm_packus_epi16(x_r1, x_r2);
		x_r2 = _mm_packus_epi16(x_r3, x_r4);
		x_g1 = _mm_packus_epi16(x_g1, x_g2);
		x_g2 = _mm_packus_epi16(x_g3, x_g4);

		/* save */
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
		PACK_RGB1(x_rgbh);

		x_rgl = _mm_unpacklo_epi8(x_r2, x_g2);
		x_rgh = _mm_unpackhi_epi8(x_r2, x_g2);
		x_bzl = _mm_unpacklo_epi8(x_b2, x_b2);
		x_bzh = _mm_unpackhi_epi8(x_b2, x_b2);

		x_rgbl = _mm_unpacklo_epi16(x_rgl, x_bzl);
		PACK_RGB1(x_rgbl);

		x_rgbh = _mm_unpackhi_epi16(x_rgl, x_bzl);
		PACK_RGB1(x_rgbh);

		x_rgbl = _mm_unpacklo_epi16(x_rgh, x_bzh);
		PACK_RGB1(x_rgbl);

		x_rgbh = _mm_unpackhi_epi16(x_rgh, x_bzh);
		PACK_RGB1(x_rgbh);
	}

	if (i <= n - 32) {
		x_y = _mm_load_si128(px_y);
		px_y++;
		x_y1 = _mm_unpacklo_epi8(x_y, x_zero);
		x_y2 = _mm_unpackhi_epi8(x_y, x_zero);
		x_y = _mm_load_si128(px_y);
		px_y++;
		x_y3 = _mm_unpacklo_epi8(x_y, x_zero);
		x_y4 = _mm_unpackhi_epi8(x_y, x_zero);
		x_cb = _mm_load_si128(px_cb);
		px_cb++;
		x_cb1 = _mm_unpacklo_epi8(x_zero, x_cb);
		x_cb2 = _mm_unpackhi_epi8(x_zero, x_cb);
		x_cr = _mm_load_si128(px_cr);
		px_cr++;
		x_cr1 = _mm_unpacklo_epi8(x_zero, x_cr);
		x_cr2 = _mm_unpackhi_epi8(x_zero, x_cr);

		/* lower half */
		x_temp = _mm_mulhi_epu16(x_cr1, x_c13);
		x_r1 = _mm_add_epi16(x_temp, x_coff0);
		x_rl = _mm_srai_epi16(x_r1, 5);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c22);
		x_g1 = _mm_mulhi_epu16(x_cr1, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g1);
		x_g1 = _mm_sub_epi16(x_coff1, x_temp);
		x_gl = _mm_srai_epi16(x_g1, 5);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c32);
		x_b1 = _mm_add_epi16(x_temp, x_coff2);
		x_bl = _mm_srai_epi16(x_b1, 5);

		/* upper half */
		x_temp = _mm_mulhi_epu16(x_cr2, x_c13);
		x_r2 = _mm_add_epi16(x_temp, x_coff0);
		x_rh = _mm_srai_epi16(x_r2, 5);

		x_temp = _mm_mulhi_epu16(x_cb2, x_c22);
		x_g2 = _mm_mulhi_epu16(x_cr2, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g2);
		x_g2 = _mm_sub_epi16(x_coff1, x_temp);
		x_gh = _mm_srai_epi16(x_g2, 5);

		x_temp = _mm_mulhi_epu16(x_cb2, x_c32);
		x_b2 = _mm_add_epi16(x_temp, x_coff2);
		x_bh = _mm_srai_epi16(x_b2, 5);

		/* get x_r/g/b[1/2/3/4] */
		x_temp = _mm_unpacklo_epi16(x_rl, x_rl);
		x_r1 = _mm_add_epi16(x_y1, x_temp);
		x_temp = _mm_unpackhi_epi16(x_rl, x_rl);
		x_r2 = _mm_add_epi16(x_y2, x_temp);
		x_temp = _mm_unpacklo_epi16(x_rh, x_rh);
		x_r3 = _mm_add_epi16(x_y3, x_temp);
		x_temp = _mm_unpackhi_epi16(x_rh, x_rh);
		x_r4 = _mm_add_epi16(x_y4, x_temp);

		x_temp = _mm_unpacklo_epi16(x_gl, x_gl);
		x_g1 = _mm_add_epi16(x_y1, x_temp);
		x_temp = _mm_unpackhi_epi16(x_gl, x_gl);
		x_g2 = _mm_add_epi16(x_y2, x_temp);
		x_temp = _mm_unpacklo_epi16(x_gh, x_gh);
		x_g3 = _mm_add_epi16(x_y3, x_temp);
		x_temp = _mm_unpackhi_epi16(x_gh, x_gh);
		x_g4 = _mm_add_epi16(x_y4, x_temp);

		x_temp = _mm_unpacklo_epi16(x_bl, x_bl);
		x_b1 = _mm_add_epi16(x_y1, x_temp);
		x_temp = _mm_unpackhi_epi16(x_bl, x_bl);
		x_b2 = _mm_add_epi16(x_y2, x_temp);
		x_temp = _mm_unpacklo_epi16(x_bh, x_bh);
		x_b3 = _mm_add_epi16(x_y3, x_temp);
		x_temp = _mm_unpackhi_epi16(x_bh, x_bh);
		x_b4 = _mm_add_epi16(x_y4, x_temp);

		/* pack */
		x_b1 = _mm_packus_epi16(x_b1, x_b2);
		x_b2 = _mm_packus_epi16(x_b3, x_b4);
		x_r1 = _mm_packus_epi16(x_r1, x_r2);
		x_r2 = _mm_packus_epi16(x_r3, x_r4);
		x_g1 = _mm_packus_epi16(x_g1, x_g2);
		x_g2 = _mm_packus_epi16(x_g3, x_g4);

		/* save */
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
		PACK_RGB1(x_rgbh);

		x_rgl = _mm_unpacklo_epi8(x_r2, x_g2);
		x_rgh = _mm_unpackhi_epi8(x_r2, x_g2);
		x_bzl = _mm_unpacklo_epi8(x_b2, x_b2);
		x_bzh = _mm_unpackhi_epi8(x_b2, x_b2);

		x_rgbl = _mm_unpacklo_epi16(x_rgl, x_bzl);
		PACK_RGB1(x_rgbl);

		x_rgbh = _mm_unpackhi_epi16(x_rgl, x_bzl);
		PACK_RGB1(x_rgbh);

		x_rgbl = _mm_unpacklo_epi16(x_rgh, x_bzh);
		PACK_RGB1(x_rgbl);

		x_rgbh = _mm_unpackhi_epi16(x_rgh, x_bzh);
		PACK_RGB2(x_rgbh);

		i += 32;
	}

	if (i <= n - 16) {
		x_y = _mm_load_si128(px_y);
		px_y++;
		x_y1 = _mm_unpacklo_epi8(x_y, x_zero);
		x_y2 = _mm_unpackhi_epi8(x_y, x_zero);
		x_cb = _mm_loadl_epi64(px_cb);
		px_cb = (__m128i *)(((__m64 *)px_cb) + 1);
		x_cb1 = _mm_unpacklo_epi8(x_zero, x_cb);
		x_cr = _mm_loadl_epi64(px_cr);
		px_cr = (__m128i *)(((__m64 *)px_cr) + 1);
		x_cr1 = _mm_unpacklo_epi8(x_zero, x_cr);

		/* lower half */
		x_temp = _mm_mulhi_epu16(x_cr1, x_c13);
		x_r1 = _mm_add_epi16(x_temp, x_coff0);
		x_rl = _mm_srai_epi16(x_r1, 5);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c22);
		x_g1 = _mm_mulhi_epu16(x_cr1, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g1);
		x_g1 = _mm_sub_epi16(x_coff1, x_temp);
		x_gl = _mm_srai_epi16(x_g1, 5);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c32);
		x_b1 = _mm_add_epi16(x_temp, x_coff2);
		x_bl = _mm_srai_epi16(x_b1, 5);

		/* get x_r/g/b[1/2] */
		x_temp = _mm_unpacklo_epi16(x_rl, x_rl);
		x_r1 = _mm_add_epi16(x_y1, x_temp);
		x_temp = _mm_unpackhi_epi16(x_rl, x_rl);
		x_r2 = _mm_add_epi16(x_y2, x_temp);

		x_temp = _mm_unpacklo_epi16(x_gl, x_gl);
		x_g1 = _mm_add_epi16(x_y1, x_temp);
		x_temp = _mm_unpackhi_epi16(x_gl, x_gl);
		x_g2 = _mm_add_epi16(x_y2, x_temp);

		x_temp = _mm_unpacklo_epi16(x_bl, x_bl);
		x_b1 = _mm_add_epi16(x_y1, x_temp);
		x_temp = _mm_unpackhi_epi16(x_bl, x_bl);
		x_b2 = _mm_add_epi16(x_y2, x_temp);

		/* pack */
		x_b1 = _mm_packus_epi16(x_b1, x_b2);
		x_r1 = _mm_packus_epi16(x_r1, x_r2);
		x_g1 = _mm_packus_epi16(x_g1, x_g2);

		/* save */
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

		i += 16;
	}

	if (i <= n - 8) {
		x_y = _mm_loadl_epi64(px_y);
		px_y = (__m128i *)(((__m64 *)px_y) + 1);
		x_y1 = _mm_unpacklo_epi8(x_y, x_zero);
		iTemp = *((mlib_s32 *)px_cb);
		x_cb = _mm_cvtsi32_si128(iTemp);
		px_cb = (__m128i *) (((mlib_s32 *)px_cb) + 1);
		x_cb1 = _mm_unpacklo_epi8(x_zero, x_cb);
		iTemp = *((mlib_s32 *)px_cr);
		x_cr = _mm_cvtsi32_si128(iTemp);
		px_cr = (__m128i *) (((mlib_s32 *)px_cr) + 1);
		x_cr1 = _mm_unpacklo_epi8(x_zero, x_cr);

		/* lower half */
		x_temp = _mm_mulhi_epu16(x_cr1, x_c13);
		x_r1 = _mm_add_epi16(x_temp, x_coff0);
		x_rl = _mm_srai_epi16(x_r1, 5);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c22);
		x_g1 = _mm_mulhi_epu16(x_cr1, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g1);
		x_g1 = _mm_sub_epi16(x_coff1, x_temp);
		x_gl = _mm_srai_epi16(x_g1, 5);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c32);
		x_b1 = _mm_add_epi16(x_temp, x_coff2);
		x_bl = _mm_srai_epi16(x_b1, 5);

		/* get x_r/g/b[1] */
		x_temp = _mm_unpacklo_epi16(x_rl, x_rl);
		x_r1 = _mm_add_epi16(x_y1, x_temp);

		x_temp = _mm_unpacklo_epi16(x_gl, x_gl);
		x_g1 = _mm_add_epi16(x_y1, x_temp);

		x_temp = _mm_unpacklo_epi16(x_bl, x_bl);
		x_b1 = _mm_add_epi16(x_y1, x_temp);

		/* pack */
		x_b1 = _mm_packus_epi16(x_b1, x_zero);
		x_r1 = _mm_packus_epi16(x_r1, x_zero);
		x_g1 = _mm_packus_epi16(x_g1, x_zero);

		/* save */
		x_rgl = _mm_unpacklo_epi8(x_r1, x_g1);
		x_bzl = _mm_unpacklo_epi8(x_b1, x_b1);

		x_rgbl = _mm_unpacklo_epi16(x_rgl, x_bzl);
		PACK_RGB1(x_rgbl);

		x_rgbh = _mm_unpackhi_epi16(x_rgl, x_bzl);
		PACK_RGB2(x_rgbh);

		i += 8;
	}

	if (i <= n - 4) {
		iTemp = *((mlib_s32 *)px_y);
		px_y = (__m128i *)(((mlib_s32 *)px_y) + 1);
		x_y = _mm_cvtsi32_si128(iTemp);
		x_y1 = _mm_unpacklo_epi8(x_y, x_zero);
		iTemp = *((mlib_s16 *)px_cb);
		x_cb = _mm_cvtsi32_si128(iTemp);
		px_cb = (__m128i *) (((mlib_s16 *)px_cb) + 1);
		x_cb1 = _mm_unpacklo_epi8(x_zero, x_cb);
		iTemp = *((mlib_s16 *)px_cr);
		x_cr = _mm_cvtsi32_si128(iTemp);
		px_cr = (__m128i *) (((mlib_s16 *)px_cr) + 1);
		x_cr1 = _mm_unpacklo_epi8(x_zero, x_cr);

		/* lower half */
		x_temp = _mm_mulhi_epu16(x_cr1, x_c13);
		x_r1 = _mm_add_epi16(x_temp, x_coff0);
		x_rl = _mm_srai_epi16(x_r1, 5);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c22);
		x_g1 = _mm_mulhi_epu16(x_cr1, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g1);
		x_g1 = _mm_sub_epi16(x_coff1, x_temp);
		x_gl = _mm_srai_epi16(x_g1, 5);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c32);
		x_b1 = _mm_add_epi16(x_temp, x_coff2);
		x_bl = _mm_srai_epi16(x_b1, 5);

		/* get x_r/g/b[1] */
		x_temp = _mm_unpacklo_epi16(x_rl, x_rl);
		x_r1 = _mm_add_epi16(x_y1, x_temp);

		x_temp = _mm_unpacklo_epi16(x_gl, x_gl);
		x_g1 = _mm_add_epi16(x_y1, x_temp);

		x_temp = _mm_unpacklo_epi16(x_bl, x_bl);
		x_b1 = _mm_add_epi16(x_y1, x_temp);

		/* pack */
		x_b1 = _mm_packus_epi16(x_b1, x_zero);
		x_r1 = _mm_packus_epi16(x_r1, x_zero);
		x_g1 = _mm_packus_epi16(x_g1, x_zero);

		/* save */
		x_rgl = _mm_unpacklo_epi8(x_r1, x_g1);
		x_bzl = _mm_unpacklo_epi8(x_b1, x_b1);

		x_rgbl = _mm_unpacklo_epi16(x_rgl, x_bzl);
		PACK_RGB2(x_rgbl);

		i += 4;
	}

	/* pure C implementation */
	if (i <= n - 2) {
		iy1 = ((mlib_u8 *)px_y)[0];
		iy2 = ((mlib_u8 *)px_y)[1];
		icb1 = ((mlib_u8 *)px_cb)[0];
		icr1 = ((mlib_u8 *)px_cr)[0];

		iTemp = (icr1 * c13) >> 8;
		ir = (iTemp + coff0) >> 5;

		iTemp = (icb1 * c22) >> 8;
		ig1 = (icr1 * c23) >> 8;
		iTemp += ig1;
		ig1 = coff1 - iTemp;
		ig = ig1 >> 5;

		iTemp = (icb1 * c32) >> 8;
		ib1 = iTemp + coff2;
		ib = ib1 >> 5;

		ir1 = ir + iy1;
		ir2 = ir + iy2;
		ig1 = ig + iy1;
		ig2 = ig + iy2;
		ib1 = ib + iy1;
		ib2 = ib + iy2;
		CLAMP_U8(ir1, prgb[0]);
		CLAMP_U8(ig1, prgb[1]);
		CLAMP_U8(ib1, prgb[2]);
		CLAMP_U8(ir2, prgb[3]);
		CLAMP_U8(ig2, prgb[4]);
		CLAMP_U8(ib2, prgb[5]);
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoColorJFIFYCC2RGB422_Nearest_naligned(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *cb,
	const mlib_u8 *cr,
	mlib_s32 n)
{
	/* 1.402 * 8192 */
	const __m128i x_c13 = _mm_set1_epi16(0x2cdd);
	const mlib_s32 c13 = 0x2cdd;

	/* abs(-0.34414) * 8192 */
	const __m128i x_c22 = _mm_set1_epi16(0xb03);
	const mlib_s32 c22 = 0xb03;

	/* abs(-0.71414) * 8192 */
	const __m128i x_c23 = _mm_set1_epi16(0x16da);
	const mlib_s32 c23 = 0x16da;

	/* 1.772 * 8192 */
	const __m128i x_c32 = _mm_set1_epi16(0x38b4);
	const mlib_s32 c32 = 0x38b4;

	/* -179.456 * 32 */
	const __m128i x_coff0 = _mm_set1_epi16(0xe991);
	const mlib_s32 coff0 = (mlib_s32)0xffffe991;

	/* 135.45984 * 32 */
	const __m128i x_coff1 = _mm_set1_epi16(0x10ef);
	const mlib_s32 coff1 = 0x10ef;

	/* -226.816 * 32 */
	const __m128i x_coff2 = _mm_set1_epi16(0xe3a6);
	const mlib_s32 coff2 = (mlib_s32)0xffffe3a6;

	const __m128i x_zero = _mm_setzero_si128();

	/* __m128i variables */
	__m128i x_y, x_cb, x_cr, x_temp;
	__m128i x_rh, x_rl, x_gh, x_gl, x_bh, x_bl;
	__m128i x_y1, x_cb1, x_cr1, x_y2, x_cb2, x_cr2, x_y3, x_y4;
	__m128i x_r1, x_r2, x_g1, x_g2, x_b1, x_b2;
	__m128i x_r3, x_r4, x_g3, x_g4, x_b3, x_b4;
	__m128i x_rgbl, x_rgbh, x_rgl, x_rgh, x_bzl, x_bzh;

	/* pointers */
	__m128i *px_y, *px_cb, *px_cr;
	mlib_u8 *prgb;

	/* other var */
	mlib_s32 i, iTemp, iy1, iy2, icb1, icr1;
	mlib_s32 ir, ig, ib, ir1, ig1, ib1, ir2, ig2, ib2;

	px_y = (__m128i *)y;
	px_cb = (__m128i *)cb;
	px_cr = (__m128i *)cr;
	prgb = rgb;
	i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i <= n - 64; i += 32) {
		x_y = _mm_loadu_si128(px_y);
		px_y++;
		x_y1 = _mm_unpacklo_epi8(x_y, x_zero);
		x_y2 = _mm_unpackhi_epi8(x_y, x_zero);
		x_y = _mm_loadu_si128(px_y);
		px_y++;
		x_y3 = _mm_unpacklo_epi8(x_y, x_zero);
		x_y4 = _mm_unpackhi_epi8(x_y, x_zero);
		x_cb = _mm_loadu_si128(px_cb);
		px_cb++;
		x_cb1 = _mm_unpacklo_epi8(x_zero, x_cb);
		x_cb2 = _mm_unpackhi_epi8(x_zero, x_cb);
		x_cr = _mm_loadu_si128(px_cr);
		px_cr++;
		x_cr1 = _mm_unpacklo_epi8(x_zero, x_cr);
		x_cr2 = _mm_unpackhi_epi8(x_zero, x_cr);

		/* lower half */
		x_temp = _mm_mulhi_epu16(x_cr1, x_c13);
		x_r1 = _mm_add_epi16(x_temp, x_coff0);
		x_rl = _mm_srai_epi16(x_r1, 5);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c22);
		x_g1 = _mm_mulhi_epu16(x_cr1, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g1);
		x_g1 = _mm_sub_epi16(x_coff1, x_temp);
		x_gl = _mm_srai_epi16(x_g1, 5);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c32);
		x_b1 = _mm_add_epi16(x_temp, x_coff2);
		x_bl = _mm_srai_epi16(x_b1, 5);

		/* upper half */
		x_temp = _mm_mulhi_epu16(x_cr2, x_c13);
		x_r2 = _mm_add_epi16(x_temp, x_coff0);
		x_rh = _mm_srai_epi16(x_r2, 5);

		x_temp = _mm_mulhi_epu16(x_cb2, x_c22);
		x_g2 = _mm_mulhi_epu16(x_cr2, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g2);
		x_g2 = _mm_sub_epi16(x_coff1, x_temp);
		x_gh = _mm_srai_epi16(x_g2, 5);

		x_temp = _mm_mulhi_epu16(x_cb2, x_c32);
		x_b2 = _mm_add_epi16(x_temp, x_coff2);
		x_bh = _mm_srai_epi16(x_b2, 5);

		/* get x_r/g/b[1/2/3/4] */
		x_temp = _mm_unpacklo_epi16(x_rl, x_rl);
		x_r1 = _mm_add_epi16(x_y1, x_temp);
		x_temp = _mm_unpackhi_epi16(x_rl, x_rl);
		x_r2 = _mm_add_epi16(x_y2, x_temp);
		x_temp = _mm_unpacklo_epi16(x_rh, x_rh);
		x_r3 = _mm_add_epi16(x_y3, x_temp);
		x_temp = _mm_unpackhi_epi16(x_rh, x_rh);
		x_r4 = _mm_add_epi16(x_y4, x_temp);

		x_temp = _mm_unpacklo_epi16(x_gl, x_gl);
		x_g1 = _mm_add_epi16(x_y1, x_temp);
		x_temp = _mm_unpackhi_epi16(x_gl, x_gl);
		x_g2 = _mm_add_epi16(x_y2, x_temp);
		x_temp = _mm_unpacklo_epi16(x_gh, x_gh);
		x_g3 = _mm_add_epi16(x_y3, x_temp);
		x_temp = _mm_unpackhi_epi16(x_gh, x_gh);
		x_g4 = _mm_add_epi16(x_y4, x_temp);

		x_temp = _mm_unpacklo_epi16(x_bl, x_bl);
		x_b1 = _mm_add_epi16(x_y1, x_temp);
		x_temp = _mm_unpackhi_epi16(x_bl, x_bl);
		x_b2 = _mm_add_epi16(x_y2, x_temp);
		x_temp = _mm_unpacklo_epi16(x_bh, x_bh);
		x_b3 = _mm_add_epi16(x_y3, x_temp);
		x_temp = _mm_unpackhi_epi16(x_bh, x_bh);
		x_b4 = _mm_add_epi16(x_y4, x_temp);

		/* pack */
		x_b1 = _mm_packus_epi16(x_b1, x_b2);
		x_b2 = _mm_packus_epi16(x_b3, x_b4);
		x_r1 = _mm_packus_epi16(x_r1, x_r2);
		x_r2 = _mm_packus_epi16(x_r3, x_r4);
		x_g1 = _mm_packus_epi16(x_g1, x_g2);
		x_g2 = _mm_packus_epi16(x_g3, x_g4);

		/* save */
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
		PACK_RGB1(x_rgbh);

		x_rgl = _mm_unpacklo_epi8(x_r2, x_g2);
		x_rgh = _mm_unpackhi_epi8(x_r2, x_g2);
		x_bzl = _mm_unpacklo_epi8(x_b2, x_b2);
		x_bzh = _mm_unpackhi_epi8(x_b2, x_b2);

		x_rgbl = _mm_unpacklo_epi16(x_rgl, x_bzl);
		PACK_RGB1(x_rgbl);

		x_rgbh = _mm_unpackhi_epi16(x_rgl, x_bzl);
		PACK_RGB1(x_rgbh);

		x_rgbl = _mm_unpacklo_epi16(x_rgh, x_bzh);
		PACK_RGB1(x_rgbl);

		x_rgbh = _mm_unpackhi_epi16(x_rgh, x_bzh);
		PACK_RGB1(x_rgbh);
	}

	if (i <= n - 32) {
		x_y = _mm_loadu_si128(px_y);
		px_y++;
		x_y1 = _mm_unpacklo_epi8(x_y, x_zero);
		x_y2 = _mm_unpackhi_epi8(x_y, x_zero);
		x_y = _mm_loadu_si128(px_y);
		px_y++;
		x_y3 = _mm_unpacklo_epi8(x_y, x_zero);
		x_y4 = _mm_unpackhi_epi8(x_y, x_zero);
		x_cb = _mm_loadu_si128(px_cb);
		px_cb++;
		x_cb1 = _mm_unpacklo_epi8(x_zero, x_cb);
		x_cb2 = _mm_unpackhi_epi8(x_zero, x_cb);
		x_cr = _mm_loadu_si128(px_cr);
		px_cr++;
		x_cr1 = _mm_unpacklo_epi8(x_zero, x_cr);
		x_cr2 = _mm_unpackhi_epi8(x_zero, x_cr);

		/* lower half */
		x_temp = _mm_mulhi_epu16(x_cr1, x_c13);
		x_r1 = _mm_add_epi16(x_temp, x_coff0);
		x_rl = _mm_srai_epi16(x_r1, 5);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c22);
		x_g1 = _mm_mulhi_epu16(x_cr1, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g1);
		x_g1 = _mm_sub_epi16(x_coff1, x_temp);
		x_gl = _mm_srai_epi16(x_g1, 5);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c32);
		x_b1 = _mm_add_epi16(x_temp, x_coff2);
		x_bl = _mm_srai_epi16(x_b1, 5);

		/* upper half */
		x_temp = _mm_mulhi_epu16(x_cr2, x_c13);
		x_r2 = _mm_add_epi16(x_temp, x_coff0);
		x_rh = _mm_srai_epi16(x_r2, 5);

		x_temp = _mm_mulhi_epu16(x_cb2, x_c22);
		x_g2 = _mm_mulhi_epu16(x_cr2, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g2);
		x_g2 = _mm_sub_epi16(x_coff1, x_temp);
		x_gh = _mm_srai_epi16(x_g2, 5);

		x_temp = _mm_mulhi_epu16(x_cb2, x_c32);
		x_b2 = _mm_add_epi16(x_temp, x_coff2);
		x_bh = _mm_srai_epi16(x_b2, 5);

		/* get x_r/g/b[1/2/3/4] */
		x_temp = _mm_unpacklo_epi16(x_rl, x_rl);
		x_r1 = _mm_add_epi16(x_y1, x_temp);
		x_temp = _mm_unpackhi_epi16(x_rl, x_rl);
		x_r2 = _mm_add_epi16(x_y2, x_temp);
		x_temp = _mm_unpacklo_epi16(x_rh, x_rh);
		x_r3 = _mm_add_epi16(x_y3, x_temp);
		x_temp = _mm_unpackhi_epi16(x_rh, x_rh);
		x_r4 = _mm_add_epi16(x_y4, x_temp);

		x_temp = _mm_unpacklo_epi16(x_gl, x_gl);
		x_g1 = _mm_add_epi16(x_y1, x_temp);
		x_temp = _mm_unpackhi_epi16(x_gl, x_gl);
		x_g2 = _mm_add_epi16(x_y2, x_temp);
		x_temp = _mm_unpacklo_epi16(x_gh, x_gh);
		x_g3 = _mm_add_epi16(x_y3, x_temp);
		x_temp = _mm_unpackhi_epi16(x_gh, x_gh);
		x_g4 = _mm_add_epi16(x_y4, x_temp);

		x_temp = _mm_unpacklo_epi16(x_bl, x_bl);
		x_b1 = _mm_add_epi16(x_y1, x_temp);
		x_temp = _mm_unpackhi_epi16(x_bl, x_bl);
		x_b2 = _mm_add_epi16(x_y2, x_temp);
		x_temp = _mm_unpacklo_epi16(x_bh, x_bh);
		x_b3 = _mm_add_epi16(x_y3, x_temp);
		x_temp = _mm_unpackhi_epi16(x_bh, x_bh);
		x_b4 = _mm_add_epi16(x_y4, x_temp);

		/* pack */
		x_b1 = _mm_packus_epi16(x_b1, x_b2);
		x_b2 = _mm_packus_epi16(x_b3, x_b4);
		x_r1 = _mm_packus_epi16(x_r1, x_r2);
		x_r2 = _mm_packus_epi16(x_r3, x_r4);
		x_g1 = _mm_packus_epi16(x_g1, x_g2);
		x_g2 = _mm_packus_epi16(x_g3, x_g4);

		/* save */
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
		PACK_RGB1(x_rgbh);

		x_rgl = _mm_unpacklo_epi8(x_r2, x_g2);
		x_rgh = _mm_unpackhi_epi8(x_r2, x_g2);
		x_bzl = _mm_unpacklo_epi8(x_b2, x_b2);
		x_bzh = _mm_unpackhi_epi8(x_b2, x_b2);

		x_rgbl = _mm_unpacklo_epi16(x_rgl, x_bzl);
		PACK_RGB1(x_rgbl);

		x_rgbh = _mm_unpackhi_epi16(x_rgl, x_bzl);
		PACK_RGB1(x_rgbh);

		x_rgbl = _mm_unpacklo_epi16(x_rgh, x_bzh);
		PACK_RGB1(x_rgbl);

		x_rgbh = _mm_unpackhi_epi16(x_rgh, x_bzh);
		PACK_RGB2(x_rgbh);

		i += 32;
	}

	if (i <= n - 16) {
		x_y = _mm_loadu_si128(px_y);
		px_y++;
		x_y1 = _mm_unpacklo_epi8(x_y, x_zero);
		x_y2 = _mm_unpackhi_epi8(x_y, x_zero);
		x_cb = _mm_loadl_epi64(px_cb);
		px_cb = (__m128i *)(((__m64 *)px_cb) + 1);
		x_cb1 = _mm_unpacklo_epi8(x_zero, x_cb);
		x_cr = _mm_loadl_epi64(px_cr);
		px_cr = (__m128i *)(((__m64 *)px_cr) + 1);
		x_cr1 = _mm_unpacklo_epi8(x_zero, x_cr);

		/* lower half */
		x_temp = _mm_mulhi_epu16(x_cr1, x_c13);
		x_r1 = _mm_add_epi16(x_temp, x_coff0);
		x_rl = _mm_srai_epi16(x_r1, 5);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c22);
		x_g1 = _mm_mulhi_epu16(x_cr1, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g1);
		x_g1 = _mm_sub_epi16(x_coff1, x_temp);
		x_gl = _mm_srai_epi16(x_g1, 5);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c32);
		x_b1 = _mm_add_epi16(x_temp, x_coff2);
		x_bl = _mm_srai_epi16(x_b1, 5);

		/* get x_r/g/b[1/2] */
		x_temp = _mm_unpacklo_epi16(x_rl, x_rl);
		x_r1 = _mm_add_epi16(x_y1, x_temp);
		x_temp = _mm_unpackhi_epi16(x_rl, x_rl);
		x_r2 = _mm_add_epi16(x_y2, x_temp);

		x_temp = _mm_unpacklo_epi16(x_gl, x_gl);
		x_g1 = _mm_add_epi16(x_y1, x_temp);
		x_temp = _mm_unpackhi_epi16(x_gl, x_gl);
		x_g2 = _mm_add_epi16(x_y2, x_temp);

		x_temp = _mm_unpacklo_epi16(x_bl, x_bl);
		x_b1 = _mm_add_epi16(x_y1, x_temp);
		x_temp = _mm_unpackhi_epi16(x_bl, x_bl);
		x_b2 = _mm_add_epi16(x_y2, x_temp);

		/* pack */
		x_b1 = _mm_packus_epi16(x_b1, x_b2);
		x_r1 = _mm_packus_epi16(x_r1, x_r2);
		x_g1 = _mm_packus_epi16(x_g1, x_g2);

		/* save */
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

		i += 16;
	}

	if (i <= n - 8) {
		x_y = _mm_loadl_epi64(px_y);
		px_y = (__m128i *)(((__m64 *)px_y) + 1);
		x_y1 = _mm_unpacklo_epi8(x_y, x_zero);
		iTemp = *((mlib_s32 *)px_cb);
		x_cb = _mm_cvtsi32_si128(iTemp);
		px_cb = (__m128i *) (((mlib_s32 *)px_cb) + 1);
		x_cb1 = _mm_unpacklo_epi8(x_zero, x_cb);
		iTemp = *((mlib_s32 *)px_cr);
		x_cr = _mm_cvtsi32_si128(iTemp);
		px_cr = (__m128i *) (((mlib_s32 *)px_cr) + 1);
		x_cr1 = _mm_unpacklo_epi8(x_zero, x_cr);

		/* lower half */
		x_temp = _mm_mulhi_epu16(x_cr1, x_c13);
		x_r1 = _mm_add_epi16(x_temp, x_coff0);
		x_rl = _mm_srai_epi16(x_r1, 5);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c22);
		x_g1 = _mm_mulhi_epu16(x_cr1, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g1);
		x_g1 = _mm_sub_epi16(x_coff1, x_temp);
		x_gl = _mm_srai_epi16(x_g1, 5);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c32);
		x_b1 = _mm_add_epi16(x_temp, x_coff2);
		x_bl = _mm_srai_epi16(x_b1, 5);

		/* get x_r/g/b[1] */
		x_temp = _mm_unpacklo_epi16(x_rl, x_rl);
		x_r1 = _mm_add_epi16(x_y1, x_temp);

		x_temp = _mm_unpacklo_epi16(x_gl, x_gl);
		x_g1 = _mm_add_epi16(x_y1, x_temp);

		x_temp = _mm_unpacklo_epi16(x_bl, x_bl);
		x_b1 = _mm_add_epi16(x_y1, x_temp);

		/* pack */
		x_b1 = _mm_packus_epi16(x_b1, x_zero);
		x_r1 = _mm_packus_epi16(x_r1, x_zero);
		x_g1 = _mm_packus_epi16(x_g1, x_zero);

		/* save */
		x_rgl = _mm_unpacklo_epi8(x_r1, x_g1);
		x_bzl = _mm_unpacklo_epi8(x_b1, x_b1);

		x_rgbl = _mm_unpacklo_epi16(x_rgl, x_bzl);
		PACK_RGB1(x_rgbl);

		x_rgbh = _mm_unpackhi_epi16(x_rgl, x_bzl);
		PACK_RGB2(x_rgbh);

		i += 8;
	}

	if (i <= n - 4) {
		iTemp = *((mlib_s32 *)px_y);
		px_y = (__m128i *)(((mlib_s32 *)px_y) + 1);
		x_y = _mm_cvtsi32_si128(iTemp);
		x_y1 = _mm_unpacklo_epi8(x_y, x_zero);
		iTemp = *((mlib_s16 *)px_cb);
		x_cb = _mm_cvtsi32_si128(iTemp);
		px_cb = (__m128i *) (((mlib_s16 *)px_cb) + 1);
		x_cb1 = _mm_unpacklo_epi8(x_zero, x_cb);
		iTemp = *((mlib_s16 *)px_cr);
		x_cr = _mm_cvtsi32_si128(iTemp);
		px_cr = (__m128i *) (((mlib_s16 *)px_cr) + 1);
		x_cr1 = _mm_unpacklo_epi8(x_zero, x_cr);

		/* lower half */
		x_temp = _mm_mulhi_epu16(x_cr1, x_c13);
		x_r1 = _mm_add_epi16(x_temp, x_coff0);
		x_rl = _mm_srai_epi16(x_r1, 5);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c22);
		x_g1 = _mm_mulhi_epu16(x_cr1, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g1);
		x_g1 = _mm_sub_epi16(x_coff1, x_temp);
		x_gl = _mm_srai_epi16(x_g1, 5);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c32);
		x_b1 = _mm_add_epi16(x_temp, x_coff2);
		x_bl = _mm_srai_epi16(x_b1, 5);

		/* get x_r/g/b[1] */
		x_temp = _mm_unpacklo_epi16(x_rl, x_rl);
		x_r1 = _mm_add_epi16(x_y1, x_temp);

		x_temp = _mm_unpacklo_epi16(x_gl, x_gl);
		x_g1 = _mm_add_epi16(x_y1, x_temp);

		x_temp = _mm_unpacklo_epi16(x_bl, x_bl);
		x_b1 = _mm_add_epi16(x_y1, x_temp);

		/* pack */
		x_b1 = _mm_packus_epi16(x_b1, x_zero);
		x_r1 = _mm_packus_epi16(x_r1, x_zero);
		x_g1 = _mm_packus_epi16(x_g1, x_zero);

		/* save */
		x_rgl = _mm_unpacklo_epi8(x_r1, x_g1);
		x_bzl = _mm_unpacklo_epi8(x_b1, x_b1);

		x_rgbl = _mm_unpacklo_epi16(x_rgl, x_bzl);
		PACK_RGB2(x_rgbl);

		i += 4;
	}

	/* pure C implementation */
	if (i <= n - 2) {
		iy1 = ((mlib_u8 *)px_y)[0];
		iy2 = ((mlib_u8 *)px_y)[1];
		icb1 = ((mlib_u8 *)px_cb)[0];
		icr1 = ((mlib_u8 *)px_cr)[0];

		iTemp = (icr1 * c13) >> 8;
		ir = (iTemp + coff0) >> 5;

		iTemp = (icb1 * c22) >> 8;
		ig1 = (icr1 * c23) >> 8;
		iTemp += ig1;
		ig1 = coff1 - iTemp;
		ig = ig1 >> 5;

		iTemp = (icb1 * c32) >> 8;
		ib1 = iTemp + coff2;
		ib = ib1 >> 5;

		ir1 = ir + iy1;
		ir2 = ir + iy2;
		ig1 = ig + iy1;
		ig2 = ig + iy2;
		ib1 = ib + iy1;
		ib2 = ib + iy2;
		CLAMP_U8(ir1, prgb[0]);
		CLAMP_U8(ig1, prgb[1]);
		CLAMP_U8(ib1, prgb[2]);
		CLAMP_U8(ir2, prgb[3]);
		CLAMP_U8(ig2, prgb[4]);
		CLAMP_U8(ib2, prgb[5]);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
