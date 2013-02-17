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

#pragma ident	"@(#)mlib_s_VideoClrJFIFYCCK2CMYK444.c	9.5	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorJFIFYCCK2CMYK444 - color conversion from JFIF YCbCrK
 *                                        to CMYK space
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorJFIFYCCK2CMYK444(mlib_u8       *cmyk,
 *                                                  const mlib_u8 *y,
 *                                                  const mlib_u8 *cb,
 *                                                  const mlib_u8 *cr,
 *                                                  const mlib_u8 *k,
 *                                                  mlib_s32      n)
 *
 * ARGUMENTS
 *      cmyk      Pointer to CMYK multicomponent row, 8-byte aligned
 *      y         Pointer to Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      k         Pointer to K component row, 8-byte aligned
 *      n         Length of y, cb, cr, k arrays
 *                Note: length of cmyk array must be 4*n
 *
 * DESCRIPTION
 *      mlib_VideoColorJFIFYCCK2CMYK444
 *        R  = Y                        + 1.40200 * (Cr - 128)
 *        G  = Y - 0.34414 * (Cb - 128) - 0.71414 * (Cr - 128)
 *        B  = Y + 1.77200 * (Cb - 128)
 *
 *        and CMYK: C = (255 - R),  M = (255 - G),  Y = (255 - B)
 *
 *        Y = y[i], Cb = cb[i], Cr = cr[i]
 *        C = cmyk[4*i], M = cmyk[4*i+1], Y = cmyk[4*i+2]
 *
 *        K = cmyk[4*i+3] = k[i] - should be passed unchanged
 *
 *        0 <= i < n
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

#pragma weak mlib_VideoColorJFIFYCCK2CMYK444 = \
	__mlib_VideoColorJFIFYCCK2CMYK444

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorJFIFYCCK2CMYK444) mlib_VideoColorJFIFYCCK2CMYK444
	__attribute__((weak, alias("__mlib_VideoColorJFIFYCCK2CMYK444")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
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
mlib_VideoColorJFIFYCCK2CMYK444_aligned(
	mlib_u8 *cmyk,
	const mlib_u8 *y,
	const mlib_u8 *cb,
	const mlib_u8 *cr,
	const mlib_u8 *k,
	mlib_s32 n);

mlib_status
mlib_VideoColorJFIFYCCK2CMYK444_naligned(
	mlib_u8 *cmyk,
	const mlib_u8 *y,
	const mlib_u8 *cb,
	const mlib_u8 *cr,
	const mlib_u8 *k,
	mlib_s32 n);

mlib_status
__mlib_VideoColorJFIFYCCK2CMYK444(
	mlib_u8 *cmyk,
	const mlib_u8 *y,
	const mlib_u8 *cb,
	const mlib_u8 *cr,
	const mlib_u8 *k,
	mlib_s32 n)
{
	if (cmyk == NULL || y == NULL || cb == NULL || cr == NULL || k == NULL)
		return (MLIB_NULLPOINTER);

	if (n <= 0)
		return (MLIB_FAILURE);

	if (0 == (0xf & ((mlib_addr)cmyk | (mlib_addr)y |
		(mlib_addr)cb | (mlib_addr)cr | (mlib_addr)k))) {
		return mlib_VideoColorJFIFYCCK2CMYK444_aligned(
			cmyk, y, cb, cr, k, n);
	} else {
		return mlib_VideoColorJFIFYCCK2CMYK444_naligned(
			cmyk, y, cb, cr, k, n);
	}
}

mlib_status
mlib_VideoColorJFIFYCCK2CMYK444_aligned(
	mlib_u8 *cmyk,
	const mlib_u8 *y,
	const mlib_u8 *cb,
	const mlib_u8 *cr,
	const mlib_u8 *k,
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

	const __m128i x_full = _mm_set1_epi16(0xffff);
	const __m128i x_zero = _mm_setzero_si128();

	/* __m128i variables */
	__m128i x_y, x_cb, x_cr, x_r, x_g, x_b, x_temp;
	__m128i x_y1, x_cb1, x_cr1, x_y2, x_cb2, x_cr2;
	__m128i x_r1, x_r2, x_g1, x_g2, x_b1, x_b2;
	__m128i x_cmykl, x_cmykh, x_cml, x_cmh, x_ykl, x_ykh;
	__m128i x_c, x_m, x_yy, x_k;

	/* pointers */
	__m128i *px_y, *px_cb, *px_cr, *px_k, *px_cmyk;
	mlib_u8 *pcmyk;

	/* other var */
	mlib_s32 i, iTemp, iy1, icb1, icr1;
	mlib_s32 ir1, ig1, ib1, ic1, im1, iyy1, ik1;

	px_y = (__m128i *)y;
	px_cb = (__m128i *)cb;
	px_cr = (__m128i *)cr;
	px_k = (__m128i *)k;
	px_cmyk = (__m128i *)cmyk;
	i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i <= n - 16; i += 16)	{
		x_y = _mm_load_si128(px_y);
		px_y++;
		x_y1 = _mm_unpacklo_epi8(x_y, x_zero);
		x_y2 = _mm_unpackhi_epi8(x_y, x_zero);
		x_cb = _mm_load_si128(px_cb);
		px_cb++;
		x_cb1 = _mm_unpacklo_epi8(x_zero, x_cb);
		x_cb2 = _mm_unpackhi_epi8(x_zero, x_cb);
		x_cr = _mm_load_si128(px_cr);
		px_cr++;
		x_cr1 = _mm_unpacklo_epi8(x_zero, x_cr);
		x_cr2 = _mm_unpackhi_epi8(x_zero, x_cr);
		x_k = _mm_load_si128(px_k);
		px_k++;

		/* lower half */
		x_temp = _mm_mulhi_epu16(x_cr1, x_c13);
		x_r1 = _mm_add_epi16(x_temp, x_coff0);
		x_temp = _mm_srai_epi16(x_r1, 5);
		x_r1 = _mm_add_epi16(x_temp, x_y1);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c22);
		x_g1 = _mm_mulhi_epu16(x_cr1, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g1);
		x_g1 = _mm_sub_epi16(x_coff1, x_temp);
		x_temp = _mm_srai_epi16(x_g1, 5);
		x_g1 = _mm_add_epi16(x_temp, x_y1);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c32);
		x_b1 = _mm_add_epi16(x_temp, x_coff2);
		x_temp = _mm_srai_epi16(x_b1, 5);
		x_b1 = _mm_add_epi16(x_temp, x_y1);

		/* upper half */
		x_temp = _mm_mulhi_epu16(x_cr2, x_c13);
		x_r2 = _mm_add_epi16(x_temp, x_coff0);
		x_temp = _mm_srai_epi16(x_r2, 5);
		x_r2 = _mm_add_epi16(x_temp, x_y2);

		x_temp = _mm_mulhi_epu16(x_cb2, x_c22);
		x_g2 = _mm_mulhi_epu16(x_cr2, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g2);
		x_g2 = _mm_sub_epi16(x_coff1, x_temp);
		x_temp = _mm_srai_epi16(x_g2, 5);
		x_g2 = _mm_add_epi16(x_temp, x_y2);

		x_temp = _mm_mulhi_epu16(x_cb2, x_c32);
		x_b2 = _mm_add_epi16(x_temp, x_coff2);
		x_temp = _mm_srai_epi16(x_b2, 5);
		x_b2 = _mm_add_epi16(x_temp, x_y2);

		/* pack */
		x_b = _mm_packus_epi16(x_b1, x_b2);
		x_r = _mm_packus_epi16(x_r1, x_r2);
		x_g = _mm_packus_epi16(x_g1, x_g2);

		x_c = _mm_sub_epi8(x_full, x_r);
		x_m = _mm_sub_epi8(x_full, x_g);
		x_yy = _mm_sub_epi8(x_full, x_b);

		/* create cmyk sequences */
		x_cml = _mm_unpacklo_epi8(x_c, x_m);
		x_cmh = _mm_unpackhi_epi8(x_c, x_m);
		x_ykl = _mm_unpacklo_epi8(x_yy, x_k);
		x_ykh = _mm_unpackhi_epi8(x_yy, x_k);

		/* save */
		x_cmykl = _mm_unpacklo_epi16(x_cml, x_ykl);
		_mm_store_si128(px_cmyk++, x_cmykl);

		x_cmykh = _mm_unpackhi_epi16(x_cml, x_ykl);
		_mm_store_si128(px_cmyk++, x_cmykh);

		x_cmykl = _mm_unpacklo_epi16(x_cmh, x_ykh);
		_mm_store_si128(px_cmyk++, x_cmykl);

		x_cmykh = _mm_unpackhi_epi16(x_cmh, x_ykh);
		_mm_store_si128(px_cmyk++, x_cmykh);
	}

	if (i <= n - 8) {
		x_y = _mm_loadl_epi64(px_y);
		px_y = (__m128i *) (((__m64 *)px_y) + 1);
		x_y1 = _mm_unpacklo_epi8(x_y, x_zero);
		x_cb = _mm_loadl_epi64(px_cb);
		px_cb = (__m128i *) (((__m64 *)px_cb) + 1);
		x_cb1 = _mm_unpacklo_epi8(x_zero, x_cb);
		x_cr = _mm_loadl_epi64(px_cr);
		px_cr = (__m128i *) (((__m64 *)px_cr) + 1);
		x_cr1 = _mm_unpacklo_epi8(x_zero, x_cr);
		x_k = _mm_loadl_epi64(px_k);
		px_k = (__m128i *) (((__m64 *)px_k) + 1);

		/* lower half only */
		x_temp = _mm_mulhi_epu16(x_cr1, x_c13);
		x_r1 = _mm_add_epi16(x_temp, x_coff0);
		x_temp = _mm_srai_epi16(x_r1, 5);
		x_r1 = _mm_add_epi16(x_temp, x_y1);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c22);
		x_g1 = _mm_mulhi_epu16(x_cr1, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g1);
		x_g1 = _mm_sub_epi16(x_coff1, x_temp);
		x_temp = _mm_srai_epi16(x_g1, 5);
		x_g1 = _mm_add_epi16(x_temp, x_y1);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c32);
		x_b1 = _mm_add_epi16(x_temp, x_coff2);
		x_temp = _mm_srai_epi16(x_b1, 5);
		x_b1 = _mm_add_epi16(x_temp, x_y1);

		/* pack */
		x_b = _mm_packus_epi16(x_b1, x_zero);
		x_r = _mm_packus_epi16(x_r1, x_zero);
		x_g = _mm_packus_epi16(x_g1, x_zero);

		x_c = _mm_sub_epi8(x_full, x_r);
		x_m = _mm_sub_epi8(x_full, x_g);
		x_yy = _mm_sub_epi8(x_full, x_b);

		/* create cmyk sequences */
		x_cml = _mm_unpacklo_epi8(x_c, x_m);
		x_ykl = _mm_unpacklo_epi8(x_yy, x_k);

		/* save */
		x_cmykl = _mm_unpacklo_epi16(x_cml, x_ykl);
		_mm_store_si128(px_cmyk++, x_cmykl);

		x_cmykh = _mm_unpackhi_epi16(x_cml, x_ykl);
		_mm_store_si128(px_cmyk++, x_cmykh);

		i += 8;
	}

	if (i <= n - 4) {
		iTemp = *((mlib_s32 *)px_y);
		x_y = _mm_cvtsi32_si128(iTemp);
		px_y = (__m128i *) (((mlib_s32 *)px_y) + 1);
		x_y1 = _mm_unpacklo_epi8(x_y, x_zero);

		iTemp = *((mlib_s32 *)px_cb);
		x_cb = _mm_cvtsi32_si128(iTemp);
		px_cb = (__m128i *) (((mlib_s32 *)px_cb) + 1);
		x_cb1 = _mm_unpacklo_epi8(x_zero, x_cb);

		iTemp = *((mlib_s32 *)px_cr);
		x_cr = _mm_cvtsi32_si128(iTemp);
		px_cr = (__m128i *) (((mlib_s32 *)px_cr) + 1);
		x_cr1 = _mm_unpacklo_epi8(x_zero, x_cr);

		iTemp = *((mlib_s32 *)px_k);
		x_k = _mm_cvtsi32_si128(iTemp);
		px_k = (__m128i *) (((mlib_s32 *)px_k) + 1);

		/* 64 0f lower half only */
		x_temp = _mm_mulhi_epu16(x_cr1, x_c13);
		x_r1 = _mm_add_epi16(x_temp, x_coff0);
		x_temp = _mm_srai_epi16(x_r1, 5);
		x_r1 = _mm_add_epi16(x_temp, x_y1);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c22);
		x_g1 = _mm_mulhi_epu16(x_cr1, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g1);
		x_g1 = _mm_sub_epi16(x_coff1, x_temp);
		x_temp = _mm_srai_epi16(x_g1, 5);
		x_g1 = _mm_add_epi16(x_temp, x_y1);
		x_temp = _mm_mulhi_epu16(x_cb1, x_c32);
		x_b1 = _mm_add_epi16(x_temp, x_coff2);
		x_temp = _mm_srai_epi16(x_b1, 5);
		x_b1 = _mm_add_epi16(x_temp, x_y1);

		/* pack */
		x_b = _mm_packus_epi16(x_b1, x_zero);
		x_r = _mm_packus_epi16(x_r1, x_zero);
		x_g = _mm_packus_epi16(x_g1, x_zero);

		x_c = _mm_sub_epi8(x_full, x_r);
		x_m = _mm_sub_epi8(x_full, x_g);
		x_yy = _mm_sub_epi8(x_full, x_b);

		/* create cmyk sequences */
		x_cml = _mm_unpacklo_epi8(x_c, x_m);
		x_ykl = _mm_unpacklo_epi8(x_yy, x_k);

		/* save */
		x_cmykl = _mm_unpacklo_epi16(x_cml, x_ykl);
		_mm_store_si128(px_cmyk++, x_cmykl);

		i += 4;
	}

	/* pure C implementation */
	pcmyk = (mlib_u8 *)px_cmyk;
	for (; i < n; i++) {
		iy1 = y[i];
		icb1 = cb[i];
		icr1 = cr[i];
		ik1 = k[i];

		iTemp = (icr1 * c13) >> 8;
		ir1 = (iTemp + coff0) >> 5;
		ir1 += iy1;

		iTemp = (icb1 * c22) >> 8;
		ig1 = (icr1 * c23) >> 8;
		iTemp += ig1;
		ig1 = coff1 - iTemp;
		iTemp = ig1 >> 5;
		ig1 = iTemp + iy1;

		iTemp = (icb1 * c32) >> 8;
		ib1 = iTemp + coff2;
		iTemp = ib1 >> 5;
		ib1 = iTemp + iy1;

		ic1 = 0xff - ir1;
		im1 = 0xff - ig1;
		iyy1 = 0xff - ib1;
		CLAMP_U8(ic1, pcmyk[0]);
		CLAMP_U8(im1, pcmyk[1]);
		CLAMP_U8(iyy1, pcmyk[2]);
		pcmyk[3] = ik1;

		pcmyk += 4;
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoColorJFIFYCCK2CMYK444_naligned(
	mlib_u8 *cmyk,
	const mlib_u8 *y,
	const mlib_u8 *cb,
	const mlib_u8 *cr,
	const mlib_u8 *k,
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

	const __m128i x_full = _mm_set1_epi16(0xffff);
	const __m128i x_zero = _mm_setzero_si128();

	/* __m128i variables */
	__m128i x_y, x_cb, x_cr, x_r, x_g, x_b, x_temp;
	__m128i x_y1, x_cb1, x_cr1, x_y2, x_cb2, x_cr2;
	__m128i x_r1, x_r2, x_g1, x_g2, x_b1, x_b2;
	__m128i x_cmykl, x_cmykh, x_cml, x_cmh, x_ykl, x_ykh;
	__m128i x_c, x_m, x_yy, x_k;

	/* pointers */
	__m128i *px_y, *px_cb, *px_cr, *px_k, *px_cmyk;
	mlib_u8 *pcmyk;

	/* other var */
	mlib_s32 i, iTemp, iy1, icb1, icr1;
	mlib_s32 ir1, ig1, ib1, ic1, im1, iyy1, ik1;

	px_y = (__m128i *)y;
	px_cb = (__m128i *)cb;
	px_cr = (__m128i *)cr;
	px_k = (__m128i *)k;
	px_cmyk = (__m128i *)cmyk;
	i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i <= n - 16; i += 16)	{
		x_y = _mm_loadu_si128(px_y);
		px_y++;
		x_y1 = _mm_unpacklo_epi8(x_y, x_zero);
		x_y2 = _mm_unpackhi_epi8(x_y, x_zero);
		x_cb = _mm_loadu_si128(px_cb);
		px_cb++;
		x_cb1 = _mm_unpacklo_epi8(x_zero, x_cb);
		x_cb2 = _mm_unpackhi_epi8(x_zero, x_cb);
		x_cr = _mm_loadu_si128(px_cr);
		px_cr++;
		x_cr1 = _mm_unpacklo_epi8(x_zero, x_cr);
		x_cr2 = _mm_unpackhi_epi8(x_zero, x_cr);
		x_k = _mm_loadu_si128(px_k);
		px_k++;

		/* lower half */
		x_temp = _mm_mulhi_epu16(x_cr1, x_c13);
		x_r1 = _mm_add_epi16(x_temp, x_coff0);
		x_temp = _mm_srai_epi16(x_r1, 5);
		x_r1 = _mm_add_epi16(x_temp, x_y1);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c22);
		x_g1 = _mm_mulhi_epu16(x_cr1, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g1);
		x_g1 = _mm_sub_epi16(x_coff1, x_temp);
		x_temp = _mm_srai_epi16(x_g1, 5);
		x_g1 = _mm_add_epi16(x_temp, x_y1);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c32);
		x_b1 = _mm_add_epi16(x_temp, x_coff2);
		x_temp = _mm_srai_epi16(x_b1, 5);
		x_b1 = _mm_add_epi16(x_temp, x_y1);

		/* upper half */
		x_temp = _mm_mulhi_epu16(x_cr2, x_c13);
		x_r2 = _mm_add_epi16(x_temp, x_coff0);
		x_temp = _mm_srai_epi16(x_r2, 5);
		x_r2 = _mm_add_epi16(x_temp, x_y2);

		x_temp = _mm_mulhi_epu16(x_cb2, x_c22);
		x_g2 = _mm_mulhi_epu16(x_cr2, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g2);
		x_g2 = _mm_sub_epi16(x_coff1, x_temp);
		x_temp = _mm_srai_epi16(x_g2, 5);
		x_g2 = _mm_add_epi16(x_temp, x_y2);

		x_temp = _mm_mulhi_epu16(x_cb2, x_c32);
		x_b2 = _mm_add_epi16(x_temp, x_coff2);
		x_temp = _mm_srai_epi16(x_b2, 5);
		x_b2 = _mm_add_epi16(x_temp, x_y2);

		/* pack */
		x_b = _mm_packus_epi16(x_b1, x_b2);
		x_r = _mm_packus_epi16(x_r1, x_r2);
		x_g = _mm_packus_epi16(x_g1, x_g2);

		x_c = _mm_sub_epi8(x_full, x_r);
		x_m = _mm_sub_epi8(x_full, x_g);
		x_yy = _mm_sub_epi8(x_full, x_b);

		/* create cmyk sequences */
		x_cml = _mm_unpacklo_epi8(x_c, x_m);
		x_cmh = _mm_unpackhi_epi8(x_c, x_m);
		x_ykl = _mm_unpacklo_epi8(x_yy, x_k);
		x_ykh = _mm_unpackhi_epi8(x_yy, x_k);

		/* save */
		x_cmykl = _mm_unpacklo_epi16(x_cml, x_ykl);
		_mm_storeu_si128(px_cmyk++, x_cmykl);

		x_cmykh = _mm_unpackhi_epi16(x_cml, x_ykl);
		_mm_storeu_si128(px_cmyk++, x_cmykh);

		x_cmykl = _mm_unpacklo_epi16(x_cmh, x_ykh);
		_mm_storeu_si128(px_cmyk++, x_cmykl);

		x_cmykh = _mm_unpackhi_epi16(x_cmh, x_ykh);
		_mm_storeu_si128(px_cmyk++, x_cmykh);
	}

	if (i <= n - 8) {
		x_y = _mm_loadl_epi64(px_y);
		px_y = (__m128i *) (((__m64 *)px_y) + 1);
		x_y1 = _mm_unpacklo_epi8(x_y, x_zero);
		x_cb = _mm_loadl_epi64(px_cb);
		px_cb = (__m128i *) (((__m64 *)px_cb) + 1);
		x_cb1 = _mm_unpacklo_epi8(x_zero, x_cb);
		x_cr = _mm_loadl_epi64(px_cr);
		px_cr = (__m128i *) (((__m64 *)px_cr) + 1);
		x_cr1 = _mm_unpacklo_epi8(x_zero, x_cr);
		x_k = _mm_loadl_epi64(px_k);
		px_k = (__m128i *) (((__m64 *)px_k) + 1);

		/* lower half only */
		x_temp = _mm_mulhi_epu16(x_cr1, x_c13);
		x_r1 = _mm_add_epi16(x_temp, x_coff0);
		x_temp = _mm_srai_epi16(x_r1, 5);
		x_r1 = _mm_add_epi16(x_temp, x_y1);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c22);
		x_g1 = _mm_mulhi_epu16(x_cr1, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g1);
		x_g1 = _mm_sub_epi16(x_coff1, x_temp);
		x_temp = _mm_srai_epi16(x_g1, 5);
		x_g1 = _mm_add_epi16(x_temp, x_y1);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c32);
		x_b1 = _mm_add_epi16(x_temp, x_coff2);
		x_temp = _mm_srai_epi16(x_b1, 5);
		x_b1 = _mm_add_epi16(x_temp, x_y1);

		/* pack */
		x_b = _mm_packus_epi16(x_b1, x_zero);
		x_r = _mm_packus_epi16(x_r1, x_zero);
		x_g = _mm_packus_epi16(x_g1, x_zero);

		x_c = _mm_sub_epi8(x_full, x_r);
		x_m = _mm_sub_epi8(x_full, x_g);
		x_yy = _mm_sub_epi8(x_full, x_b);

		/* create cmyk sequences */
		x_cml = _mm_unpacklo_epi8(x_c, x_m);
		x_ykl = _mm_unpacklo_epi8(x_yy, x_k);

		/* save */
		x_cmykl = _mm_unpacklo_epi16(x_cml, x_ykl);
		_mm_storeu_si128(px_cmyk++, x_cmykl);

		x_cmykh = _mm_unpackhi_epi16(x_cml, x_ykl);
		_mm_storeu_si128(px_cmyk++, x_cmykh);

		i += 8;
	}

	if (i <= n - 4) {
		iTemp = *((mlib_s32 *)px_y);
		x_y = _mm_cvtsi32_si128(iTemp);
		px_y = (__m128i *) (((mlib_s32 *)px_y) + 1);
		x_y1 = _mm_unpacklo_epi8(x_y, x_zero);

		iTemp = *((mlib_s32 *)px_cb);
		x_cb = _mm_cvtsi32_si128(iTemp);
		px_cb = (__m128i *) (((mlib_s32 *)px_cb) + 1);
		x_cb1 = _mm_unpacklo_epi8(x_zero, x_cb);

		iTemp = *((mlib_s32 *)px_cr);
		x_cr = _mm_cvtsi32_si128(iTemp);
		px_cr = (__m128i *) (((mlib_s32 *)px_cr) + 1);
		x_cr1 = _mm_unpacklo_epi8(x_zero, x_cr);

		iTemp = *((mlib_s32 *)px_k);
		x_k = _mm_cvtsi32_si128(iTemp);
		px_k = (__m128i *) (((mlib_s32 *)px_k) + 1);

		/* 64 0f lower half only */
		x_temp = _mm_mulhi_epu16(x_cr1, x_c13);
		x_r1 = _mm_add_epi16(x_temp, x_coff0);
		x_temp = _mm_srai_epi16(x_r1, 5);
		x_r1 = _mm_add_epi16(x_temp, x_y1);

		x_temp = _mm_mulhi_epu16(x_cb1, x_c22);
		x_g1 = _mm_mulhi_epu16(x_cr1, x_c23);
		x_temp = _mm_add_epi16(x_temp, x_g1);
		x_g1 = _mm_sub_epi16(x_coff1, x_temp);
		x_temp = _mm_srai_epi16(x_g1, 5);
		x_g1 = _mm_add_epi16(x_temp, x_y1);
		x_temp = _mm_mulhi_epu16(x_cb1, x_c32);
		x_b1 = _mm_add_epi16(x_temp, x_coff2);
		x_temp = _mm_srai_epi16(x_b1, 5);
		x_b1 = _mm_add_epi16(x_temp, x_y1);

		/* pack */
		x_b = _mm_packus_epi16(x_b1, x_zero);
		x_r = _mm_packus_epi16(x_r1, x_zero);
		x_g = _mm_packus_epi16(x_g1, x_zero);

		x_c = _mm_sub_epi8(x_full, x_r);
		x_m = _mm_sub_epi8(x_full, x_g);
		x_yy = _mm_sub_epi8(x_full, x_b);

		/* create cmyk sequences */
		x_cml = _mm_unpacklo_epi8(x_c, x_m);
		x_ykl = _mm_unpacklo_epi8(x_yy, x_k);

		/* save */
		x_cmykl = _mm_unpacklo_epi16(x_cml, x_ykl);
		_mm_storeu_si128(px_cmyk++, x_cmykl);

		i += 4;
	}

	/* pure C implementation */
	pcmyk = (mlib_u8 *)px_cmyk;
	for (; i < n; i++) {
		iy1 = y[i];
		icb1 = cb[i];
		icr1 = cr[i];
		ik1 = k[i];

		iTemp = (icr1 * c13) >> 8;
		ir1 = (iTemp + coff0) >> 5;
		ir1 += iy1;

		iTemp = (icb1 * c22) >> 8;
		ig1 = (icr1 * c23) >> 8;
		iTemp += ig1;
		ig1 = coff1 - iTemp;
		iTemp = ig1 >> 5;
		ig1 = iTemp + iy1;

		iTemp = (icb1 * c32) >> 8;
		ib1 = iTemp + coff2;
		iTemp = ib1 >> 5;
		ib1 = iTemp + iy1;

		ic1 = 0xff - ir1;
		im1 = 0xff - ig1;
		iyy1 = 0xff - ib1;
		CLAMP_U8(ic1, pcmyk[0]);
		CLAMP_U8(im1, pcmyk[1]);
		CLAMP_U8(iyy1, pcmyk[2]);
		pcmyk[3] = ik1;

		pcmyk += 4;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
