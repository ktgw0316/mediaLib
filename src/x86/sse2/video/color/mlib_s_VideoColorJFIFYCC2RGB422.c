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

#pragma ident	"@(#)mlib_s_VideoColorJFIFYCC2RGB422.c	9.5	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorJFIFYCC2RGB422 - color conversion with 422 upsampling
 *                                      with bilinear filter.
 *                                      Only Cr and Cb components are
 *                                      under upsampling.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorJFIFYCC2RGB422(mlib_u8       *rgb,
 *                                                const mlib_u8 *y,
 *                                                const mlib_u8 *cb,
 *                                                const mlib_u8 *cr,
 *                                                mlib_s32      n)
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
 *
 *      mlib_VideoColorJFIFYCC2RGB422:
 *        composition of mlib_VideoUpSample422 with
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

#pragma weak mlib_VideoColorJFIFYCC2RGB422 = __mlib_VideoColorJFIFYCC2RGB422

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorJFIFYCC2RGB422) mlib_VideoColorJFIFYCC2RGB422
	__attribute__((weak, alias("__mlib_VideoColorJFIFYCC2RGB422")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
/* save : rgb0 rgb0 rgb0 rgb0 */

/* do not save the first two rgb-s */
#define	PACK_RGB1(rgb, prgb)	\
{							\
	mlib_u8 *pout = ((mlib_u8*)(&rgb)) + 8;		\
	*((mlib_s32*)prgb) = *((mlib_s32*)pout);	\
	prgb += 3;					\
	pout += 4;					\
	*((mlib_s32*)prgb) = *((mlib_s32*)pout);	\
	prgb += 3;					\
}

/* save the 4 rgb(s) */
#define	PACK_RGB2(rgb, prgb)	\
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

/* do not save the last two rgb-s */
#define	PACK_RGB3(rgb, prgb)	\
{							\
	mlib_u8 *pout = (mlib_u8*)(&rgb);		\
	*((mlib_s32*)prgb) = *((mlib_s32*)pout);	\
	prgb += 3;					\
	pout += 4;					\
	*((mlib_s32*)prgb) = *((mlib_s32*)pout);	\
	prgb += 3;					\
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
__mlib_VideoColorJFIFYCC2RGB422(
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
	const __m128i x_one = _mm_set1_epi8(0x1);

	/* __m128i variables */
	__m128i x_y, x_cb, x_cr, x_temp, x_self, x_ls, x_rs;
	__m128i x_y1, x_y2, x_y3, x_y4;
	__m128i x_cb1, x_cb2, x_cb3, x_cb4;
	__m128i x_cr1, x_cr2, x_cr3, x_cr4;
	__m128i x_r1, x_r2, x_r3, x_r4;
	__m128i x_b1, x_b2, x_b3, x_b4;
	__m128i x_g1, x_g2, x_g3, x_g4;
	__m128i x_rgbl, x_rgbh, x_rgl, x_rgh, x_bzl, x_bzh;

	/* pointers */
	mlib_u8 *py, *pcb, *pcr, *prgb;

	/* other var */
	mlib_s32 iTemp, i, iy1, iy2, icr1, icr2, icb1, icb2;
	mlib_s32 ioldcb, ioldcr, icbc, icbs, icrc, icrs;
	mlib_s32 ir1, ig1, ib1, ir2, ig2, ib2;

	py = (mlib_u8 *)y;
	pcb = (mlib_u8 *)cb;
	pcr = (mlib_u8 *)cr;
	prgb = rgb;
	i = 0;
	n >>= 1;

	/* process 1st cb/cr */
	ioldcb = pcb[0] - 1;
	ioldcr = pcr[0] - 1;

	if (n > 1) {
		icbc = pcb[0];
		icbs = icbc * 3;
		icb1 = (icbs + ioldcb + 1) >> 2;
		icb2 = (icbs + pcb[1] + 2) >> 2;

		icrc = pcr[0];
		icrs = icrc * 3;
		icr1 = (icrs + ioldcr + 1) >> 2;
		icr2 = (icrs + pcr[1] + 2) >> 2;
	} else {
		icbc = pcb[0];
		icbs = icbc * 3;
		icb1 = (icbs + ioldcb + 1) >> 2;
		icb2 = icbc;

		icrc = pcr[0];
		icrs = icrc * 3;
		icr1 = (icrs + ioldcr + 1) >> 2;
		icr2 = icrc;
	}

	iy1 = py[0];
	iy2 = py[1];

	iTemp = (icr1 * c13) >> 8;
	ir1 = (iTemp + coff0) >> 5;

	iTemp = (icb1 * c22) >> 8;
	ig1 = (icr1 * c23) >> 8;
	iTemp += ig1;
	ig1 = coff1 - iTemp;
	ig1 = ig1 >> 5;

	iTemp = (icb1 * c32) >> 8;
	ib1 = iTemp + coff2;
	ib1 = ib1 >> 5;

	iTemp = (icr2 * c13) >> 8;
	ir2 = (iTemp + coff0) >> 5;

	iTemp = (icb2 * c22) >> 8;
	ig2 = (icr2 * c23) >> 8;
	iTemp += ig2;
	ig2 = coff1 - iTemp;
	ig2 = ig2 >> 5;

	iTemp = (icb2 * c32) >> 8;
	ib2 = iTemp + coff2;
	ib2 = ib2 >> 5;

	CLAMP_U8(ir1 + iy1, prgb[0]);
	CLAMP_U8(ig1 + iy1, prgb[1]);
	CLAMP_U8(ib1 + iy1, prgb[2]);
	CLAMP_U8(ir2 + iy2, prgb[3]);
	CLAMP_U8(ig2 + iy2, prgb[4]);
	CLAMP_U8(ib2 + iy2, prgb[5]);

	prgb += 6;

	if (1 == n)
		return (MLIB_SUCCESS);

	/* calc 28 rgb */
	for (; i <= n - 16; i += 14) {
		/* load */
		x_y = _mm_loadu_si128((__m128i *)py);
		x_y1 = _mm_unpacklo_epi8(x_y, x_zero);
		x_y2 = _mm_unpackhi_epi8(x_y, x_zero);
		py += 16;
		x_y = _mm_loadu_si128((__m128i *)py);
		x_y3 = _mm_unpacklo_epi8(x_y, x_zero);
		x_y4 = _mm_unpackhi_epi8(x_y, x_zero);
		py += 12;

		x_cb = _mm_loadu_si128((__m128i *)pcb);
		x_self = x_cb;
		x_ls = _mm_slli_si128(x_self, 1);
		x_rs = _mm_srli_si128(x_self, 1);
		x_temp = _mm_adds_epu8(x_self, x_one);
		x_temp = _mm_avg_epu8(x_temp, x_ls);
		x_cb1 = _mm_avg_epu8(x_temp, x_self);
		x_temp = _mm_adds_epu8(x_self, x_one);
		x_cb2 = _mm_avg_epu8(x_self, x_rs);
		x_cb2 = _mm_avg_epu8(x_temp, x_cb2);
		/* cross */
		x_temp = x_cb1;
		x_cb1 = _mm_unpacklo_epi8(x_temp, x_cb2);
		x_cb2 = _mm_unpackhi_epi8(x_temp, x_cb2);
		x_temp = x_cb1;
		x_self = x_cb2;
		x_cb1 = _mm_unpacklo_epi8(x_zero, x_temp);
		x_cb2 = _mm_unpackhi_epi8(x_zero, x_temp);
		x_cb3 = _mm_unpacklo_epi8(x_zero, x_self);
		x_cb4 = _mm_unpackhi_epi8(x_zero, x_self);
		/* pcb */
		pcb += 14;

		x_cr = _mm_loadu_si128((__m128i *)pcr);
		x_self = x_cr;
		x_ls = _mm_slli_si128(x_self, 1);
		x_rs = _mm_srli_si128(x_self, 1);
		x_temp = _mm_adds_epu8(x_self, x_one);
		x_temp = _mm_avg_epu8(x_temp, x_ls);
		x_cr1 = _mm_avg_epu8(x_temp, x_self);
		x_temp = _mm_adds_epu8(x_self, x_one);
		x_cr2 = _mm_avg_epu8(x_self, x_rs);
		x_cr2 = _mm_avg_epu8(x_temp, x_cr2);
		/* cross */
		x_temp = x_cr1;
		x_cr1 = _mm_unpacklo_epi8(x_temp, x_cr2);
		x_cr2 = _mm_unpackhi_epi8(x_temp, x_cr2);
		x_temp = x_cr1;
		x_self = x_cr2;
		x_cr1 = _mm_unpacklo_epi8(x_zero, x_temp);
		x_cr2 = _mm_unpackhi_epi8(x_zero, x_temp);
		x_cr3 = _mm_unpacklo_epi8(x_zero, x_self);
		x_cr4 = _mm_unpackhi_epi8(x_zero, x_self);
		/* pcr */
		pcr += 14;

		/* pre-calc */
		x_temp = _mm_mulhi_epu16(x_cr1, x_c13);
		x_r1 = _mm_add_epi16(x_temp, x_coff0);
		x_r1 = _mm_srai_epi16(x_r1, 5);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c22);
		x_g1 = _mm_mulhi_epu16(x_cr1, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g1);
		x_g1 = _mm_sub_epi16(x_coff1, x_temp);
		x_g1 = _mm_srai_epi16(x_g1, 5);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c32);
		x_b1 = _mm_add_epi16(x_temp, x_coff2);
		x_b1 = _mm_srai_epi16(x_b1, 5);

		x_temp = _mm_mulhi_epu16(x_cr2, x_c13);
		x_r2 = _mm_add_epi16(x_temp, x_coff0);
		x_r2 = _mm_srai_epi16(x_r2, 5);

		x_temp = _mm_mulhi_epu16(x_cb2, x_c22);
		x_g2 = _mm_mulhi_epu16(x_cr2, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g2);
		x_g2 = _mm_sub_epi16(x_coff1, x_temp);
		x_g2 = _mm_srai_epi16(x_g2, 5);

		x_temp = _mm_mulhi_epu16(x_cb2, x_c32);
		x_b2 = _mm_add_epi16(x_temp, x_coff2);
		x_b2 = _mm_srai_epi16(x_b2, 5);

		x_temp = _mm_mulhi_epu16(x_cr3, x_c13);
		x_r3 = _mm_add_epi16(x_temp, x_coff0);
		x_r3 = _mm_srai_epi16(x_r3, 5);

		x_temp = _mm_mulhi_epu16(x_cb3, x_c22);
		x_g3 = _mm_mulhi_epu16(x_cr3, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g3);
		x_g3 = _mm_sub_epi16(x_coff1, x_temp);
		x_g3 = _mm_srai_epi16(x_g3, 5);

		x_temp = _mm_mulhi_epu16(x_cb3, x_c32);
		x_b3 = _mm_add_epi16(x_temp, x_coff2);
		x_b3 = _mm_srai_epi16(x_b3, 5);

		x_temp = _mm_mulhi_epu16(x_cr4, x_c13);
		x_r4 = _mm_add_epi16(x_temp, x_coff0);
		x_r4 = _mm_srai_epi16(x_r4, 5);

		x_temp = _mm_mulhi_epu16(x_cb4, x_c22);
		x_g4 = _mm_mulhi_epu16(x_cr4, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g4);
		x_g4 = _mm_sub_epi16(x_coff1, x_temp);
		x_g4 = _mm_srai_epi16(x_g4, 5);

		x_temp = _mm_mulhi_epu16(x_cb4, x_c32);
		x_b4 = _mm_add_epi16(x_temp, x_coff2);
		x_b4 = _mm_srai_epi16(x_b4, 5);

		/* get x_r/g/b[1/2/3/4] */
		x_r1 = _mm_add_epi16(x_y1, x_r1);
		x_r2 = _mm_add_epi16(x_y2, x_r2);
		x_r3 = _mm_add_epi16(x_y3, x_r3);
		x_r4 = _mm_add_epi16(x_y4, x_r4);

		x_g1 = _mm_add_epi16(x_y1, x_g1);
		x_g2 = _mm_add_epi16(x_y2, x_g2);
		x_g3 = _mm_add_epi16(x_y3, x_g3);
		x_g4 = _mm_add_epi16(x_y4, x_g4);

		x_b1 = _mm_add_epi16(x_y1, x_b1);
		x_b2 = _mm_add_epi16(x_y2, x_b2);
		x_b3 = _mm_add_epi16(x_y3, x_b3);
		x_b4 = _mm_add_epi16(x_y4, x_b4);

		/* pack */
		x_r1 = _mm_packus_epi16(x_r1, x_r2);
		x_g1 = _mm_packus_epi16(x_g1, x_g2);
		x_b1 = _mm_packus_epi16(x_b1, x_b2);
		x_r2 = _mm_packus_epi16(x_r3, x_r4);
		x_g2 = _mm_packus_epi16(x_g3, x_g4);
		x_b2 = _mm_packus_epi16(x_b3, x_b4);

		/* save rgb */
		x_rgl = _mm_unpacklo_epi8(x_r1, x_g1);
		x_rgh = _mm_unpackhi_epi8(x_r1, x_g1);
		x_bzl = _mm_unpacklo_epi8(x_b1, x_zero);
		x_bzh = _mm_unpackhi_epi8(x_b1, x_zero);

		x_rgbl = _mm_unpacklo_epi16(x_rgl, x_bzl);
		PACK_RGB1(x_rgbl, prgb);

		x_rgbh = _mm_unpackhi_epi16(x_rgl, x_bzl);
		PACK_RGB2(x_rgbh, prgb);

		x_rgbl = _mm_unpacklo_epi16(x_rgh, x_bzh);
		PACK_RGB2(x_rgbl, prgb);

		x_rgbh = _mm_unpackhi_epi16(x_rgh, x_bzh);
		PACK_RGB2(x_rgbh, prgb);

		x_rgl = _mm_unpacklo_epi8(x_r2, x_g2);
		x_rgh = _mm_unpackhi_epi8(x_r2, x_g2);
		x_bzl = _mm_unpacklo_epi8(x_b2, x_zero);
		x_bzh = _mm_unpackhi_epi8(x_b2, x_zero);

		x_rgbl = _mm_unpacklo_epi16(x_rgl, x_bzl);
		PACK_RGB2(x_rgbl, prgb);

		x_rgbh = _mm_unpackhi_epi16(x_rgl, x_bzl);
		PACK_RGB2(x_rgbh, prgb);

		x_rgbl = _mm_unpacklo_epi16(x_rgh, x_bzh);
		PACK_RGB2(x_rgbl, prgb);

		x_rgbh = _mm_unpackhi_epi16(x_rgh, x_bzh);
		PACK_RGB3(x_rgbh, prgb);
	}

	/* get 12 rgb */
	while (i <= n - 8) {
		/* load */
		x_y = _mm_loadu_si128((__m128i *)py);
		x_y1 = _mm_unpacklo_epi8(x_y, x_zero);
		x_y2 = _mm_unpackhi_epi8(x_y, x_zero);
		py += 12;

		x_cb = _mm_loadl_epi64((__m128i *)pcb);
		x_self = x_cb;
		x_ls = _mm_slli_si128(x_self, 1);
		x_rs = _mm_srli_si128(x_self, 1);
		x_temp = _mm_adds_epu8(x_self, x_one);
		x_temp = _mm_avg_epu8(x_temp, x_ls);
		x_cb1 = _mm_avg_epu8(x_temp, x_self);
		x_temp = _mm_adds_epu8(x_self, x_one);
		x_cb2 = _mm_avg_epu8(x_self, x_rs);
		x_cb2 = _mm_avg_epu8(x_temp, x_cb2);
		/* cross */
		x_temp = x_cb1;
		x_cb1 = _mm_unpacklo_epi8(x_temp, x_cb2);
		x_temp = x_cb1;
		x_cb1 = _mm_unpacklo_epi8(x_zero, x_temp);
		x_cb2 = _mm_unpackhi_epi8(x_zero, x_temp);
		/* pcb */
		pcb += 6;

		x_cr = _mm_loadu_si128((__m128i *)pcr);
		x_self = x_cr;
		x_ls = _mm_slli_si128(x_self, 1);
		x_rs = _mm_srli_si128(x_self, 1);
		x_temp = _mm_adds_epu8(x_self, x_one);
		x_temp = _mm_avg_epu8(x_temp, x_ls);
		x_cr1 = _mm_avg_epu8(x_temp, x_self);
		x_temp = _mm_adds_epu8(x_self, x_one);
		x_cr2 = _mm_avg_epu8(x_self, x_rs);
		x_cr2 = _mm_avg_epu8(x_temp, x_cr2);
		/* cross */
		x_temp = x_cr1;
		x_cr1 = _mm_unpacklo_epi8(x_temp, x_cr2);
		x_temp = x_cr1;
		x_cr1 = _mm_unpacklo_epi8(x_zero, x_temp);
		x_cr2 = _mm_unpackhi_epi8(x_zero, x_temp);
		/* pcr */
		pcr += 6;

		/* pre-calc */
		x_temp = _mm_mulhi_epu16(x_cr1, x_c13);
		x_r1 = _mm_add_epi16(x_temp, x_coff0);
		x_r1 = _mm_srai_epi16(x_r1, 5);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c22);
		x_g1 = _mm_mulhi_epu16(x_cr1, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g1);
		x_g1 = _mm_sub_epi16(x_coff1, x_temp);
		x_g1 = _mm_srai_epi16(x_g1, 5);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c32);
		x_b1 = _mm_add_epi16(x_temp, x_coff2);
		x_b1 = _mm_srai_epi16(x_b1, 5);

		x_temp = _mm_mulhi_epu16(x_cr2, x_c13);
		x_r2 = _mm_add_epi16(x_temp, x_coff0);
		x_r2 = _mm_srai_epi16(x_r2, 5);

		x_temp = _mm_mulhi_epu16(x_cb2, x_c22);
		x_g2 = _mm_mulhi_epu16(x_cr2, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g2);
		x_g2 = _mm_sub_epi16(x_coff1, x_temp);
		x_g2 = _mm_srai_epi16(x_g2, 5);

		x_temp = _mm_mulhi_epu16(x_cb2, x_c32);
		x_b2 = _mm_add_epi16(x_temp, x_coff2);
		x_b2 = _mm_srai_epi16(x_b2, 5);

		/* get x_r/g/b[1/2] */
		x_r1 = _mm_add_epi16(x_y1, x_r1);
		x_r2 = _mm_add_epi16(x_y2, x_r2);

		x_g1 = _mm_add_epi16(x_y1, x_g1);
		x_g2 = _mm_add_epi16(x_y2, x_g2);

		x_b1 = _mm_add_epi16(x_y1, x_b1);
		x_b2 = _mm_add_epi16(x_y2, x_b2);

		/* pack */
		x_r1 = _mm_packus_epi16(x_r1, x_r2);
		x_g1 = _mm_packus_epi16(x_g1, x_g2);
		x_b1 = _mm_packus_epi16(x_b1, x_b2);

		/* save rgb */
		x_rgl = _mm_unpacklo_epi8(x_r1, x_g1);
		x_rgh = _mm_unpackhi_epi8(x_r1, x_g1);
		x_bzl = _mm_unpacklo_epi8(x_b1, x_zero);
		x_bzh = _mm_unpackhi_epi8(x_b1, x_zero);

		x_rgbl = _mm_unpacklo_epi16(x_rgl, x_bzl);
		PACK_RGB1(x_rgbl, prgb);

		x_rgbh = _mm_unpackhi_epi16(x_rgl, x_bzl);
		PACK_RGB2(x_rgbh, prgb);

		x_rgbl = _mm_unpacklo_epi16(x_rgh, x_bzh);
		PACK_RGB2(x_rgbl, prgb);

		x_rgbh = _mm_unpackhi_epi16(x_rgh, x_bzh);
		PACK_RGB3(x_rgbh, prgb);

		i += 6;
	}

	/* get 4 rgb */
	while (i <= n - 4) {
		/* load */
		x_y = _mm_loadl_epi64((__m128i *)py);
		x_y1 = _mm_unpacklo_epi8(x_y, x_zero);
		py += 4;

		iTemp = *((mlib_s32 *)pcb);
		x_cb = _mm_cvtsi32_si128(iTemp);
		x_self = x_cb;
		x_ls = _mm_slli_si128(x_self, 1);
		x_rs = _mm_srli_si128(x_self, 1);
		x_temp = _mm_adds_epu8(x_self, x_one);
		x_temp = _mm_avg_epu8(x_temp, x_ls);
		x_cb1 = _mm_avg_epu8(x_temp, x_self);
		x_temp = _mm_adds_epu8(x_self, x_one);
		x_cb2 = _mm_avg_epu8(x_self, x_rs);
		x_cb2 = _mm_avg_epu8(x_temp, x_cb2);
		/* cross */
		x_temp = x_cb1;
		x_cb1 = _mm_unpacklo_epi8(x_temp, x_cb2);
		x_temp = x_cb1;
		x_cb1 = _mm_unpacklo_epi8(x_zero, x_temp);
		/* pcb */
		pcb += 2;

		iTemp = *((mlib_s32 *)pcr);
		x_cr = _mm_cvtsi32_si128(iTemp);
		x_self = x_cr;
		x_ls = _mm_slli_si128(x_self, 1);
		x_rs = _mm_srli_si128(x_self, 1);
		x_temp = _mm_adds_epu8(x_self, x_one);
		x_temp = _mm_avg_epu8(x_temp, x_ls);
		x_cr1 = _mm_avg_epu8(x_temp, x_self);
		x_temp = _mm_adds_epu8(x_self, x_one);
		x_cr2 = _mm_avg_epu8(x_self, x_rs);
		x_cr2 = _mm_avg_epu8(x_temp, x_cr2);
		/* cross */
		x_temp = x_cr1;
		x_cr1 = _mm_unpacklo_epi8(x_temp, x_cr2);
		x_temp = x_cr1;
		x_cr1 = _mm_unpacklo_epi8(x_zero, x_temp);
		/* pcr */
		pcr += 2;

		/* pre-calc */
		x_temp = _mm_mulhi_epu16(x_cr1, x_c13);
		x_r1 = _mm_add_epi16(x_temp, x_coff0);
		x_r1 = _mm_srai_epi16(x_r1, 5);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c22);
		x_g1 = _mm_mulhi_epu16(x_cr1, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g1);
		x_g1 = _mm_sub_epi16(x_coff1, x_temp);
		x_g1 = _mm_srai_epi16(x_g1, 5);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c32);
		x_b1 = _mm_add_epi16(x_temp, x_coff2);
		x_b1 = _mm_srai_epi16(x_b1, 5);

		/* get x_r/g/b[1] */
		x_r1 = _mm_add_epi16(x_y1, x_r1);
		x_g1 = _mm_add_epi16(x_y1, x_g1);
		x_b1 = _mm_add_epi16(x_y1, x_b1);

		/* pack */
		x_r1 = _mm_packus_epi16(x_r1, x_zero);
		x_g1 = _mm_packus_epi16(x_g1, x_zero);
		x_b1 = _mm_packus_epi16(x_b1, x_zero);

		/* save rgb */
		x_rgl = _mm_unpacklo_epi8(x_r1, x_g1);
		x_bzl = _mm_unpacklo_epi8(x_b1, x_zero);

		x_rgbl = _mm_unpacklo_epi16(x_rgl, x_bzl);
		PACK_RGB1(x_rgbl, prgb);

		x_rgbh = _mm_unpackhi_epi16(x_rgl, x_bzl);
		PACK_RGB3(x_rgbh, prgb);

		i += 2;
	}

	py += 2;
	pcb++;
	pcr++;
	i++;
	n--;

	/* pure C */
	while (i <= n - 1) {
		ioldcb = *(pcb - 1);
		ioldcr = *(pcr - 1);

		icbc = pcb[0];
		icbs = icbc * 3;
		icb1 = (icbs + ioldcb + 1) >> 2;
		icb2 = (icbs + pcb[1] + 2) >> 2;

		icrc = pcr[0];
		icrs = icrc * 3;
		icr1 = (icrs + ioldcr + 1) >> 2;
		icr2 = (icrs + pcr[1] + 2) >> 2;

		iy1 = py[0];
		iy2 = py[1];

		iTemp = (icr1 * c13) >> 8;
		ir1 = (iTemp + coff0) >> 5;

		iTemp = (icb1 * c22) >> 8;
		ig1 = (icr1 * c23) >> 8;
		iTemp += ig1;
		ig1 = coff1 - iTemp;
		ig1 = ig1 >> 5;

		iTemp = (icb1 * c32) >> 8;
		ib1 = iTemp + coff2;
		ib1 = ib1 >> 5;

		iTemp = (icr2 * c13) >> 8;
		ir2 = (iTemp + coff0) >> 5;

		iTemp = (icb2 * c22) >> 8;
		ig2 = (icr2 * c23) >> 8;
		iTemp += ig2;
		ig2 = coff1 - iTemp;
		ig2 = ig2 >> 5;

		iTemp = (icb2 * c32) >> 8;
		ib2 = iTemp + coff2;
		ib2 = ib2 >> 5;

		CLAMP_U8(ir1 + iy1, prgb[0]);
		CLAMP_U8(ig1 + iy1, prgb[1]);
		CLAMP_U8(ib1 + iy1, prgb[2]);
		CLAMP_U8(ir2 + iy2, prgb[3]);
		CLAMP_U8(ig2 + iy2, prgb[4]);
		CLAMP_U8(ib2 + iy2, prgb[5]);

		py += 2;
		pcb++;
		pcr++;
		prgb += 6;
		i++;
	}

	/* the last one : i = n */
	{
		ioldcb = *(pcb - 1);
		ioldcr = *(pcr - 1);

		icbc = pcb[0];
		icbs = icbc * 3;
		icb1 = (icbs + ioldcb + 1) >> 2;
		icb2 = icbc;

		icrc = pcr[0];
		icrs = icrc * 3;
		icr1 = (icrs + ioldcr + 1) >> 2;
		icr2 = icrc;

		iy1 = py[0];
		iy2 = py[1];

		iTemp = (icr1 * c13) >> 8;
		ir1 = (iTemp + coff0) >> 5;

		iTemp = (icb1 * c22) >> 8;
		ig1 = (icr1 * c23) >> 8;
		iTemp += ig1;
		ig1 = coff1 - iTemp;
		ig1 = ig1 >> 5;

		iTemp = (icb1 * c32) >> 8;
		ib1 = iTemp + coff2;
		ib1 = ib1 >> 5;

		iTemp = (icr2 * c13) >> 8;
		ir2 = (iTemp + coff0) >> 5;

		iTemp = (icb2 * c22) >> 8;
		ig2 = (icr2 * c23) >> 8;
		iTemp += ig2;
		ig2 = coff1 - iTemp;
		ig2 = ig2 >> 5;

		iTemp = (icb2 * c32) >> 8;
		ib2 = iTemp + coff2;
		ib2 = ib2 >> 5;

		CLAMP_U8(ir1 + iy1, prgb[0]);
		CLAMP_U8(ig1 + iy1, prgb[1]);
		CLAMP_U8(ib1 + iy1, prgb[2]);
		CLAMP_U8(ir2 + iy2, prgb[3]);
		CLAMP_U8(ig2 + iy2, prgb[4]);
		CLAMP_U8(ib2 + iy2, prgb[5]);

		py += 2;
		pcb++;
		pcr++;
		prgb += 6;
		i++;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
