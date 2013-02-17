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

#pragma ident	"@(#)mlib_s_VideoClrCMYK2JFIFYCCK444.c	9.5	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorCMYK2JFIFYCCK444 - color conversion from CMYK to JFIF
 *                                        YCrCbK space
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorCMYK2JFIFYCCK444(mlib_u8       *y,
 *                                                  mlib_u8       *cb,
 *                                                  mlib_u8       *cr,
 *                                                  mlib_u8       *k,
 *                                                  const mlib_u8 *cmyk,
 *                                                  mlib_s32      n)
 *
 * ARGUMENTS
 *      y         Pointer to Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      k         Pointer to K component row, 8-byte aligned
 *      cmyk      Pointer to CMYK multicomponent row, 8-byte aligned
 *      n         Length of y, cb, cr, k arrays
 *                Note: length of cmyk array must be 4*n
 *
 * DESCRIPTION
 *      mlib_VideoColorCMYK2JFIFYCCK444
 *        R = (255 - C),  G = (255 - M),  B = (255 - Y)
 *
 *        Y  =  0.29900 * R + 0.58700 * G + 0.11400 * B
 *        Cb = -0.16874 * R - 0.33126 * G + 0.50000 * B  + 128
 *        Cr =  0.50000 * R - 0.41869 * G - 0.08131 * B  + 128
 *
 *        Y = y[i], Cb = cb[i], Cr = cr[i]
 *        C = cmyk[4*i], M = cmyk[4*i+1], Y = cmyk[4*i+2]
 *
 *        K = k[i] = cmyk[4*i+3] - should be passed unchanged
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

#pragma weak mlib_VideoColorCMYK2JFIFYCCK444 = \
	__mlib_VideoColorCMYK2JFIFYCCK444

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorCMYK2JFIFYCCK444) mlib_VideoColorCMYK2JFIFYCCK444
	__attribute__((weak, alias("__mlib_VideoColorCMYK2JFIFYCCK444")));

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
	}	\

/* cmyk: 64 bytes */
#define	SeparateCMYK64(x_cmyk0, x_cmyk1, x_cmyk2,	\
x_cmyk3, x_c, x_m, x_yy, x_k) 				\
{							\
	x_t1 = _mm_unpacklo_epi8(x_cmyk0, x_cmyk1);	\
	x_t2 = _mm_unpackhi_epi8(x_cmyk0, x_cmyk1);	\
	x_t3 = _mm_unpacklo_epi8(x_cmyk2, x_cmyk3);	\
	x_t4 = _mm_unpackhi_epi8(x_cmyk2, x_cmyk3);	\
							\
	x_t5 = _mm_unpacklo_epi8(x_t1, x_t2);		\
	x_t6 = _mm_unpackhi_epi8(x_t1, x_t2);		\
	x_t7 = _mm_unpacklo_epi8(x_t3, x_t4);		\
	x_t8 = _mm_unpackhi_epi8(x_t3, x_t4);		\
							\
	x_t1 = _mm_unpacklo_epi8(x_t5, x_t6);		\
	x_t2 = _mm_unpackhi_epi8(x_t5, x_t6);		\
	x_t3 = _mm_unpacklo_epi8(x_t7, x_t8);		\
	x_t4 = _mm_unpackhi_epi8(x_t7, x_t8);		\
							\
	x_c = _mm_unpacklo_epi64(x_t1, x_t3);		\
	x_m = _mm_unpackhi_epi64(x_t1, x_t3);		\
	x_yy = _mm_unpacklo_epi64(x_t2, x_t4);		\
	x_k = _mm_unpackhi_epi64(x_t2, x_t4);		\
}

/* cmyk: 32 bytes */
#define	SeparateCMYK32(x_cmyk0, x_cmyk1, x_c, x_m, x_yy, x_k) \
{							\
	x_t1 = _mm_unpacklo_epi8(x_cmyk0, x_cmyk1);	\
	x_t2 = _mm_unpackhi_epi8(x_cmyk0, x_cmyk1);	\
							\
	x_t5 = _mm_unpacklo_epi8(x_t1, x_t2);		\
	x_t6 = _mm_unpackhi_epi8(x_t1, x_t2);		\
							\
	x_t1 = _mm_unpacklo_epi8(x_t5, x_t6);		\
	x_t2 = _mm_unpackhi_epi8(x_t5, x_t6);		\
							\
	x_c = _mm_unpacklo_epi64(x_t1, x_zero);		\
	x_m = _mm_unpackhi_epi64(x_t1, x_zero);		\
	x_yy = _mm_unpacklo_epi64(x_t2, x_zero);	\
	x_k = _mm_unpackhi_epi64(x_t2, x_zero);		\
}

/* cmyk: 16 bytes */
#define	SeparateCMYK16(x_cmyk0, x_c, x_m, x_yy, x_k) \
{							\
	x_t1 = _mm_unpacklo_epi8(x_cmyk0, x_zero);	\
	x_t2 = _mm_unpackhi_epi8(x_cmyk0, x_zero);	\
							\
	x_t5 = _mm_unpacklo_epi8(x_t1, x_t2);		\
	x_t6 = _mm_unpackhi_epi8(x_t1, x_t2);		\
							\
	x_t1 = _mm_unpacklo_epi8(x_t5, x_t6);		\
	x_t2 = _mm_unpackhi_epi8(x_t5, x_t6);		\
							\
	x_c = _mm_unpacklo_epi64(x_t1, x_zero);		\
	x_m = _mm_unpackhi_epi64(x_t1, x_zero);		\
	x_yy = _mm_unpacklo_epi64(x_t2, x_zero);	\
	x_k = _mm_unpackhi_epi64(x_t2, x_zero);		\
}

/* *********************************************************** */

mlib_status
mlib_VideoColorCMYK2JFIFYCCK444_aligned(
	mlib_u8 *y,
	mlib_u8 *cb,
	mlib_u8 *cr,
	mlib_u8 *k,
	const mlib_u8 *cmyk,
	mlib_s32 n);

mlib_status
mlib_VideoColorCMYK2JFIFYCCK444_naligned(
	mlib_u8 *y,
	mlib_u8 *cb,
	mlib_u8 *cr,
	mlib_u8 *k,
	const mlib_u8 *cmyk,
	mlib_s32 n);

mlib_status
__mlib_VideoColorCMYK2JFIFYCCK444(
	mlib_u8 *y,
	mlib_u8 *cb,
	mlib_u8 *cr,
	mlib_u8 *k,
	const mlib_u8 *cmyk,
	mlib_s32 n)
{
	if (cmyk == NULL || y == NULL || cb == NULL || cr == NULL || k == NULL)
		return (MLIB_NULLPOINTER);

	if (n <= 0)
		return (MLIB_FAILURE);

	if (0 == (0xf & ((mlib_addr)cmyk | (mlib_addr)y | (mlib_addr)cb |
			(mlib_addr)cr | (mlib_addr)k))) {
		return mlib_VideoColorCMYK2JFIFYCCK444_aligned(
			y, cb, cr, k, cmyk, n);
	} else {
		return mlib_VideoColorCMYK2JFIFYCCK444_naligned(
			y, cb, cr, k, cmyk, n);
	}
}

mlib_status
mlib_VideoColorCMYK2JFIFYCCK444_aligned(
	mlib_u8 *y,
	mlib_u8 *cb,
	mlib_u8 *cr,
	mlib_u8 *k,
	const mlib_u8 *cmyk,
	mlib_s32 n)
{
	/* 0.299*8192 */
	const __m128i x_c11 = _mm_set1_epi16(2449);
	const mlib_s32 c11 = 2449;

	/* 0.587*8192 */
	const __m128i x_c12 = _mm_set1_epi16(4809);
	const mlib_s32 c12 = 4809;

	/* 0.114*8192 */
	const __m128i x_c13 = _mm_set1_epi16(934);
	const mlib_s32 c13 = 934;

	/* abs(-0.16874)*8192 */
	const __m128i x_c21 = _mm_set1_epi16(1382);
	const mlib_s32 c21 = 1382;

	/* abs(-0.33126)*8192 */
	const __m128i x_c22 = _mm_set1_epi16(2714);
	const mlib_s32 c22 = 2714;

	/* 0.5*8192 */
	const __m128i x_c23 = _mm_set1_epi16(4096);
	const mlib_s32 c23 = 4096;

	/* 0.5*8192 */
	const __m128i x_c31 = x_c23;
	const mlib_s32 c31 = c23;

	/* abs(-0.41869)*8192 */
	const __m128i x_c32 = _mm_set1_epi16(3430);
	const mlib_s32 c32 = 3430;

	/* abs(-0.08131)*8192 */
	const __m128i x_c33 = _mm_set1_epi16(666);
	const mlib_s32 c33 = 666;

	/* 128 */
	const __m128i x_coff = _mm_set1_epi16(128);
	const mlib_s32 coff = 128;

	/* zero */
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_full = _mm_set1_epi16(0xffff);

	__m128i x_t1, x_t2, x_t3, x_t4, x_t5, x_t6, x_t7, x_t8;
	__m128i x_temp;
	mlib_s32 iTemp, ir1, ig1, ib1, i, ik1;
	mlib_s32 iy1, icb1, icr1, size;

	__m128i x_cmyk0, x_cmyk1, x_cmyk2, x_cmyk3;
	__m128i x_c, x_m, x_yy, x_k, x_y, x_cb, x_cr, x_r, x_g, x_b;
	__m128i x_r1, x_r2, x_g1, x_g2, x_b1, x_b2;
	__m128i x_y1, x_y2, x_cb1, x_cb2, x_cr1, x_cr2;
	__m128i *px_cmyk, *px_y, *px_cb, *px_cr, *px_k;

	px_cmyk = (__m128i *)cmyk;
	px_y = (__m128i *)y;
	px_cb = (__m128i *)cb;
	px_cr = (__m128i *)cr;
	px_k = (__m128i *)k;
	size = n;
	i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i <= size - 16; i += 16)	{
		x_cmyk0 = _mm_load_si128(px_cmyk++);
		x_cmyk1 = _mm_load_si128(px_cmyk++);
		x_cmyk2 = _mm_load_si128(px_cmyk++);
		x_cmyk3 = _mm_load_si128(px_cmyk++);

		SeparateCMYK64(x_cmyk0, x_cmyk1, x_cmyk2, x_cmyk3,
			x_c, x_m, x_yy, x_k);
		_mm_store_si128(px_k, x_k);
		px_k++;
		x_r = _mm_sub_epi8(x_full, x_c);
		x_g = _mm_sub_epi8(x_full, x_m);
		x_b = _mm_sub_epi8(x_full, x_yy);

		x_r1 = _mm_unpacklo_epi8(x_zero, x_r);
		x_r2 = _mm_unpackhi_epi8(x_zero, x_r);
		x_g1 = _mm_unpacklo_epi8(x_zero, x_g);
		x_g2 = _mm_unpackhi_epi8(x_zero, x_g);
		x_b1 = _mm_unpacklo_epi8(x_zero, x_b);
		x_b2 = _mm_unpackhi_epi8(x_zero, x_b);

		/* lower half */
		x_temp = _mm_mulhi_epu16(x_r1, x_c11);
		x_y1 = _mm_mulhi_epu16(x_g1, x_c12);
		x_y1 = _mm_add_epi16(x_y1, x_temp);
		x_temp = _mm_mulhi_epu16(x_b1, x_c13);
		x_temp = _mm_add_epi16(x_y1, x_temp);
		x_y1 = _mm_srai_epi16(x_temp, 5);

		x_temp = _mm_mulhi_epu16(x_r1, x_c21);
		x_cb1 = _mm_mulhi_epu16(x_g1, x_c22);
		x_cb1 = _mm_add_epi16(x_cb1, x_temp);
		x_temp = _mm_mulhi_epu16(x_b1, x_c23);
		x_cb1 = _mm_sub_epi16(x_temp, x_cb1);
		x_temp = _mm_srai_epi16(x_cb1, 5);
		x_cb1 = _mm_add_epi16(x_temp, x_coff);

		x_temp = _mm_mulhi_epu16(x_r1, x_c31);
		x_cr1 = _mm_mulhi_epu16(x_g1, x_c32);
		x_cr1 = _mm_sub_epi16(x_temp, x_cr1);
		x_temp = _mm_mulhi_epu16(x_b1, x_c33);
		x_cr1 = _mm_sub_epi16(x_cr1, x_temp);
		x_temp = _mm_srai_epi16(x_cr1, 5);
		x_cr1 = _mm_add_epi16(x_temp, x_coff);

		/* upper half */
		x_temp = _mm_mulhi_epu16(x_r2, x_c11);
		x_y2 = _mm_mulhi_epu16(x_g2, x_c12);
		x_y2 = _mm_add_epi16(x_y2, x_temp);
		x_temp = _mm_mulhi_epu16(x_b2, x_c13);
		x_temp = _mm_add_epi16(x_y2, x_temp);
		x_y2 = _mm_srai_epi16(x_temp, 5);

		x_temp = _mm_mulhi_epu16(x_r2, x_c21);
		x_cb2 = _mm_mulhi_epu16(x_g2, x_c22);
		x_cb2 = _mm_add_epi16(x_cb2, x_temp);
		x_temp = _mm_mulhi_epu16(x_b2, x_c23);
		x_cb2 = _mm_sub_epi16(x_temp, x_cb2);
		x_temp = _mm_srai_epi16(x_cb2, 5);
		x_cb2 = _mm_add_epi16(x_temp, x_coff);

		x_temp = _mm_mulhi_epu16(x_r2, x_c31);
		x_cr2 = _mm_mulhi_epu16(x_g2, x_c32);
		x_cr2 = _mm_sub_epi16(x_temp, x_cr2);
		x_temp = _mm_mulhi_epu16(x_b2, x_c33);
		x_cr2 = _mm_sub_epi16(x_cr2, x_temp);
		x_temp = _mm_srai_epi16(x_cr2, 5);
		x_cr2 = _mm_add_epi16(x_temp, x_coff);

		/* pack */
		x_y1 = _mm_packus_epi16(x_y1, x_y2);
		x_cb1 = _mm_packus_epi16(x_cb1, x_cb2);
		x_cr1 = _mm_packus_epi16(x_cr1, x_cr2);

		/* save */
		_mm_store_si128(px_y, x_y1);
		_mm_store_si128(px_cb, x_cb1);
		_mm_store_si128(px_cr, x_cr1);

		px_y++;
		px_cb++;
		px_cr++;
	}

	/* r/g/b: 8 bytes */
	if (i <= size - 8)	{
		x_cmyk0 = _mm_load_si128(px_cmyk);
		x_cmyk1 = _mm_load_si128(px_cmyk+1);
		px_cmyk += 2;

		SeparateCMYK32(x_cmyk0, x_cmyk1, x_c, x_m, x_yy, x_k);
		_mm_storel_epi64(px_k, x_k);
		px_k = (__m128i *) (((__m64 *)px_k) + 1);
		x_r = _mm_sub_epi8(x_full, x_c);
		x_g = _mm_sub_epi8(x_full, x_m);
		x_b = _mm_sub_epi8(x_full, x_yy);

		x_r1 = _mm_unpacklo_epi8(x_zero, x_r);
		x_g1 = _mm_unpacklo_epi8(x_zero, x_g);
		x_b1 = _mm_unpacklo_epi8(x_zero, x_b);

		/* lower half only */
		x_temp = _mm_mulhi_epu16(x_r1, x_c11);
		x_y1 = _mm_mulhi_epu16(x_g1, x_c12);
		x_y1 = _mm_add_epi16(x_y1, x_temp);
		x_temp = _mm_mulhi_epu16(x_b1, x_c13);
		x_temp = _mm_add_epi16(x_y1, x_temp);
		x_y1 = _mm_srai_epi16(x_temp, 5);

		x_temp = _mm_mulhi_epu16(x_r1, x_c21);
		x_cb1 = _mm_mulhi_epu16(x_g1, x_c22);
		x_cb1 = _mm_add_epi16(x_cb1, x_temp);
		x_temp = _mm_mulhi_epu16(x_b1, x_c23);
		x_cb1 = _mm_sub_epi16(x_temp, x_cb1);
		x_temp = _mm_srai_epi16(x_cb1, 5);
		x_cb1 = _mm_add_epi16(x_temp, x_coff);

		x_temp = _mm_mulhi_epu16(x_r1, x_c31);
		x_cr1 = _mm_mulhi_epu16(x_g1, x_c32);
		x_cr1 = _mm_sub_epi16(x_temp, x_cr1);
		x_temp = _mm_mulhi_epu16(x_b1, x_c33);
		x_cr1 = _mm_sub_epi16(x_cr1, x_temp);
		x_temp = _mm_srai_epi16(x_cr1, 5);
		x_cr1 = _mm_add_epi16(x_temp, x_coff);

		/* pack */
		x_y = _mm_packus_epi16(x_y1, x_zero);
		x_cb = _mm_packus_epi16(x_cb1, x_zero);
		x_cr = _mm_packus_epi16(x_cr1, x_zero);

		/* save */
		_mm_storel_epi64(px_y, x_y);
		_mm_storel_epi64(px_cb, x_cb);
		_mm_storel_epi64(px_cr, x_cr);

		px_y = (__m128i *) (((__m64 *)px_y) + 1);
		px_cb = (__m128i *) (((__m64 *)px_cb) + 1);
		px_cr = (__m128i *) (((__m64 *)px_cr) + 1);

		i += 8;
	}

	/* r/g/b: 4 bytes */
	if (i <= size - 4)	{
		x_cmyk0 = _mm_load_si128(px_cmyk);
		px_cmyk++;

		SeparateCMYK16(x_cmyk0, x_c, x_m, x_yy, x_k);
		*((mlib_s32 *)px_k) = *((mlib_s32 *)&x_k);
		px_k = (__m128i *) (((mlib_s32 *)px_k) + 1);
		x_r = _mm_sub_epi8(x_full, x_c);
		x_g = _mm_sub_epi8(x_full, x_m);
		x_b = _mm_sub_epi8(x_full, x_yy);

		x_r1 = _mm_unpacklo_epi8(x_zero, x_r);
		x_g1 = _mm_unpacklo_epi8(x_zero, x_g);
		x_b1 = _mm_unpacklo_epi8(x_zero, x_b);

		/* lower half only */
		x_temp = _mm_mulhi_epu16(x_r1, x_c11);
		x_y1 = _mm_mulhi_epu16(x_g1, x_c12);
		x_y1 = _mm_add_epi16(x_y1, x_temp);
		x_temp = _mm_mulhi_epu16(x_b1, x_c13);
		x_temp = _mm_add_epi16(x_y1, x_temp);
		x_y1 = _mm_srai_epi16(x_temp, 5);

		x_temp = _mm_mulhi_epu16(x_r1, x_c21);
		x_cb1 = _mm_mulhi_epu16(x_g1, x_c22);
		x_cb1 = _mm_add_epi16(x_cb1, x_temp);
		x_temp = _mm_mulhi_epu16(x_b1, x_c23);
		x_cb1 = _mm_sub_epi16(x_temp, x_cb1);
		x_temp = _mm_srai_epi16(x_cb1, 5);
		x_cb1 = _mm_add_epi16(x_temp, x_coff);

		x_temp = _mm_mulhi_epu16(x_r1, x_c31);
		x_cr1 = _mm_mulhi_epu16(x_g1, x_c32);
		x_cr1 = _mm_sub_epi16(x_temp, x_cr1);
		x_temp = _mm_mulhi_epu16(x_b1, x_c33);
		x_cr1 = _mm_sub_epi16(x_cr1, x_temp);
		x_temp = _mm_srai_epi16(x_cr1, 5);
		x_cr1 = _mm_add_epi16(x_temp, x_coff);

		/* pack */
		x_y = _mm_packus_epi16(x_y1, x_zero);
		x_cb = _mm_packus_epi16(x_cb1, x_zero);
		x_cr = _mm_packus_epi16(x_cr1, x_zero);

		/* save */
		*((mlib_s32 *)px_y) = *((mlib_s32 *)&x_y);
		*((mlib_s32 *)px_cb) = *((mlib_s32 *)&x_cb);
		*((mlib_s32 *)px_cr) = *((mlib_s32 *)&x_cr);

		px_y = (__m128i *) (((mlib_s32 *)px_y) + 1);
		px_cb = (__m128i *) (((mlib_s32 *)px_cb) + 1);
		px_cr = (__m128i *) (((mlib_s32 *)px_cr) + 1);

		i += 4;
	}

	/* r/g/b: <=3 bytes */
	for (; i < size; i ++) {
		iTemp = *((mlib_s16 *)px_cmyk);
		ir1 = 0xff - (iTemp & 0xff);
		ig1 = 0xff - ((iTemp >> 8) & 0xff);
		iTemp = *(((mlib_s16 *)px_cmyk) + 1);
		ib1 = 0xff - (iTemp & 0xff);
		ik1 = ((iTemp >> 8) & 0xff);
		px_cmyk = (__m128i *) (((mlib_u8 *)px_cmyk) + 4);

		iy1 = ((c11*ir1 + c12*ig1 + c13*ib1) >> 13);
		icb1 = ((-c21*ir1 - c22*ig1 + c23*ib1) >> 13) + coff;
		icr1 = ((c31*ir1 - c32*ig1 - c33*ib1) >> 13) + coff;

		CLAMP_U8(iy1, *((mlib_u8 *)px_y));
		CLAMP_U8(icb1, *((mlib_u8 *)px_cb));
		CLAMP_U8(icr1, *((mlib_u8 *)px_cr));
		*((mlib_u8 *)px_k) = ik1;

		px_y = (__m128i *) (((mlib_u8 *)px_y) + 1);
		px_cb = (__m128i *) (((mlib_u8 *)px_cb) + 1);
		px_cr = (__m128i *) (((mlib_u8 *)px_cr) + 1);
		px_k = (__m128i *) (((mlib_u8 *)px_k) + 1);

	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoColorCMYK2JFIFYCCK444_naligned(
	mlib_u8 *y,
	mlib_u8 *cb,
	mlib_u8 *cr,
	mlib_u8 *k,
	const mlib_u8 *cmyk,
	mlib_s32 n)
{
	/* 0.299*8192 */
	const __m128i x_c11 = _mm_set1_epi16(2449);
	const mlib_s32 c11 = 2449;

	/* 0.587*8192 */
	const __m128i x_c12 = _mm_set1_epi16(4809);
	const mlib_s32 c12 = 4809;

	/* 0.114*8192 */
	const __m128i x_c13 = _mm_set1_epi16(934);
	const mlib_s32 c13 = 934;

	/* abs(-0.16874)*8192 */
	const __m128i x_c21 = _mm_set1_epi16(1382);
	const mlib_s32 c21 = 1382;

	/* abs(-0.33126)*8192 */
	const __m128i x_c22 = _mm_set1_epi16(2714);
	const mlib_s32 c22 = 2714;

	/* 0.5*8192 */
	const __m128i x_c23 = _mm_set1_epi16(4096);
	const mlib_s32 c23 = 4096;

	/* 0.5*8192 */
	const __m128i x_c31 = x_c23;
	const mlib_s32 c31 = c23;

	/* abs(-0.41869)*8192 */
	const __m128i x_c32 = _mm_set1_epi16(3430);
	const mlib_s32 c32 = 3430;

	/* abs(-0.08131)*8192 */
	const __m128i x_c33 = _mm_set1_epi16(666);
	const mlib_s32 c33 = 666;

	/* 128 */
	const __m128i x_coff = _mm_set1_epi16(128);
	const mlib_s32 coff = 128;

	/* zero */
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_full = _mm_set1_epi16(0xffff);

	__m128i x_t1, x_t2, x_t3, x_t4, x_t5, x_t6, x_t7, x_t8;
	__m128i x_temp;
	mlib_s32 iTemp, ir1, ig1, ib1, i, ik1;
	mlib_s32 iy1, icb1, icr1, size;

	__m128i x_cmyk0, x_cmyk1, x_cmyk2, x_cmyk3;
	__m128i x_c, x_m, x_yy, x_k, x_y, x_cb, x_cr, x_r, x_g, x_b;
	__m128i x_r1, x_r2, x_g1, x_g2, x_b1, x_b2;
	__m128i x_y1, x_y2, x_cb1, x_cb2, x_cr1, x_cr2;
	__m128i *px_cmyk, *px_y, *px_cb, *px_cr, *px_k;

	px_cmyk = (__m128i *)cmyk;
	px_y = (__m128i *)y;
	px_cb = (__m128i *)cb;
	px_cr = (__m128i *)cr;
	px_k = (__m128i *)k;
	size = n;
	i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i <= size - 16; i += 16)	{
		x_cmyk0 = _mm_loadu_si128(px_cmyk++);
		x_cmyk1 = _mm_loadu_si128(px_cmyk++);
		x_cmyk2 = _mm_loadu_si128(px_cmyk++);
		x_cmyk3 = _mm_loadu_si128(px_cmyk++);

		SeparateCMYK64(x_cmyk0, x_cmyk1, x_cmyk2, x_cmyk3,
			x_c, x_m, x_yy, x_k);
		_mm_storeu_si128(px_k, x_k);
		px_k++;
		x_r = _mm_sub_epi8(x_full, x_c);
		x_g = _mm_sub_epi8(x_full, x_m);
		x_b = _mm_sub_epi8(x_full, x_yy);

		x_r1 = _mm_unpacklo_epi8(x_zero, x_r);
		x_r2 = _mm_unpackhi_epi8(x_zero, x_r);
		x_g1 = _mm_unpacklo_epi8(x_zero, x_g);
		x_g2 = _mm_unpackhi_epi8(x_zero, x_g);
		x_b1 = _mm_unpacklo_epi8(x_zero, x_b);
		x_b2 = _mm_unpackhi_epi8(x_zero, x_b);

		/* lower half */
		x_temp = _mm_mulhi_epu16(x_r1, x_c11);
		x_y1 = _mm_mulhi_epu16(x_g1, x_c12);
		x_y1 = _mm_add_epi16(x_y1, x_temp);
		x_temp = _mm_mulhi_epu16(x_b1, x_c13);
		x_temp = _mm_add_epi16(x_y1, x_temp);
		x_y1 = _mm_srai_epi16(x_temp, 5);

		x_temp = _mm_mulhi_epu16(x_r1, x_c21);
		x_cb1 = _mm_mulhi_epu16(x_g1, x_c22);
		x_cb1 = _mm_add_epi16(x_cb1, x_temp);
		x_temp = _mm_mulhi_epu16(x_b1, x_c23);
		x_cb1 = _mm_sub_epi16(x_temp, x_cb1);
		x_temp = _mm_srai_epi16(x_cb1, 5);
		x_cb1 = _mm_add_epi16(x_temp, x_coff);

		x_temp = _mm_mulhi_epu16(x_r1, x_c31);
		x_cr1 = _mm_mulhi_epu16(x_g1, x_c32);
		x_cr1 = _mm_sub_epi16(x_temp, x_cr1);
		x_temp = _mm_mulhi_epu16(x_b1, x_c33);
		x_cr1 = _mm_sub_epi16(x_cr1, x_temp);
		x_temp = _mm_srai_epi16(x_cr1, 5);
		x_cr1 = _mm_add_epi16(x_temp, x_coff);

		/* upper half */
		x_temp = _mm_mulhi_epu16(x_r2, x_c11);
		x_y2 = _mm_mulhi_epu16(x_g2, x_c12);
		x_y2 = _mm_add_epi16(x_y2, x_temp);
		x_temp = _mm_mulhi_epu16(x_b2, x_c13);
		x_temp = _mm_add_epi16(x_y2, x_temp);
		x_y2 = _mm_srai_epi16(x_temp, 5);

		x_temp = _mm_mulhi_epu16(x_r2, x_c21);
		x_cb2 = _mm_mulhi_epu16(x_g2, x_c22);
		x_cb2 = _mm_add_epi16(x_cb2, x_temp);
		x_temp = _mm_mulhi_epu16(x_b2, x_c23);
		x_cb2 = _mm_sub_epi16(x_temp, x_cb2);
		x_temp = _mm_srai_epi16(x_cb2, 5);
		x_cb2 = _mm_add_epi16(x_temp, x_coff);

		x_temp = _mm_mulhi_epu16(x_r2, x_c31);
		x_cr2 = _mm_mulhi_epu16(x_g2, x_c32);
		x_cr2 = _mm_sub_epi16(x_temp, x_cr2);
		x_temp = _mm_mulhi_epu16(x_b2, x_c33);
		x_cr2 = _mm_sub_epi16(x_cr2, x_temp);
		x_temp = _mm_srai_epi16(x_cr2, 5);
		x_cr2 = _mm_add_epi16(x_temp, x_coff);

		/* pack */
		x_y1 = _mm_packus_epi16(x_y1, x_y2);
		x_cb1 = _mm_packus_epi16(x_cb1, x_cb2);
		x_cr1 = _mm_packus_epi16(x_cr1, x_cr2);

		/* save */
		_mm_storeu_si128(px_y, x_y1);
		_mm_storeu_si128(px_cb, x_cb1);
		_mm_storeu_si128(px_cr, x_cr1);

		px_y++;
		px_cb++;
		px_cr++;
	}

	/* r/g/b: 8 bytes */
	if (i <= size - 8)	{
		x_cmyk0 = _mm_loadu_si128(px_cmyk);
		x_cmyk1 = _mm_loadu_si128(px_cmyk+1);
		px_cmyk += 2;

		SeparateCMYK32(x_cmyk0, x_cmyk1, x_c, x_m, x_yy, x_k);
		_mm_storel_epi64(px_k, x_k);
		px_k = (__m128i *) (((__m64 *)px_k) + 1);
		x_r = _mm_sub_epi8(x_full, x_c);
		x_g = _mm_sub_epi8(x_full, x_m);
		x_b = _mm_sub_epi8(x_full, x_yy);

		x_r1 = _mm_unpacklo_epi8(x_zero, x_r);
		x_g1 = _mm_unpacklo_epi8(x_zero, x_g);
		x_b1 = _mm_unpacklo_epi8(x_zero, x_b);

		/* lower half only */
		x_temp = _mm_mulhi_epu16(x_r1, x_c11);
		x_y1 = _mm_mulhi_epu16(x_g1, x_c12);
		x_y1 = _mm_add_epi16(x_y1, x_temp);
		x_temp = _mm_mulhi_epu16(x_b1, x_c13);
		x_temp = _mm_add_epi16(x_y1, x_temp);
		x_y1 = _mm_srai_epi16(x_temp, 5);

		x_temp = _mm_mulhi_epu16(x_r1, x_c21);
		x_cb1 = _mm_mulhi_epu16(x_g1, x_c22);
		x_cb1 = _mm_add_epi16(x_cb1, x_temp);
		x_temp = _mm_mulhi_epu16(x_b1, x_c23);
		x_cb1 = _mm_sub_epi16(x_temp, x_cb1);
		x_temp = _mm_srai_epi16(x_cb1, 5);
		x_cb1 = _mm_add_epi16(x_temp, x_coff);

		x_temp = _mm_mulhi_epu16(x_r1, x_c31);
		x_cr1 = _mm_mulhi_epu16(x_g1, x_c32);
		x_cr1 = _mm_sub_epi16(x_temp, x_cr1);
		x_temp = _mm_mulhi_epu16(x_b1, x_c33);
		x_cr1 = _mm_sub_epi16(x_cr1, x_temp);
		x_temp = _mm_srai_epi16(x_cr1, 5);
		x_cr1 = _mm_add_epi16(x_temp, x_coff);

		/* pack */
		x_y = _mm_packus_epi16(x_y1, x_zero);
		x_cb = _mm_packus_epi16(x_cb1, x_zero);
		x_cr = _mm_packus_epi16(x_cr1, x_zero);

		/* save */
		_mm_storel_epi64(px_y, x_y);
		_mm_storel_epi64(px_cb, x_cb);
		_mm_storel_epi64(px_cr, x_cr);

		px_y = (__m128i *) (((__m64 *)px_y) + 1);
		px_cb = (__m128i *) (((__m64 *)px_cb) + 1);
		px_cr = (__m128i *) (((__m64 *)px_cr) + 1);

		i += 8;
	}

	/* r/g/b: 4 bytes */
	if (i <= size - 4)	{
		x_cmyk0 = _mm_loadu_si128(px_cmyk);
		px_cmyk++;

		SeparateCMYK16(x_cmyk0, x_c, x_m, x_yy, x_k);
		*((mlib_s32 *)px_k) = *((mlib_s32 *)&x_k);
		px_k = (__m128i *) (((mlib_s32 *)px_k) + 1);
		x_r = _mm_sub_epi8(x_full, x_c);
		x_g = _mm_sub_epi8(x_full, x_m);
		x_b = _mm_sub_epi8(x_full, x_yy);

		x_r1 = _mm_unpacklo_epi8(x_zero, x_r);
		x_g1 = _mm_unpacklo_epi8(x_zero, x_g);
		x_b1 = _mm_unpacklo_epi8(x_zero, x_b);

		/* lower half only */
		x_temp = _mm_mulhi_epu16(x_r1, x_c11);
		x_y1 = _mm_mulhi_epu16(x_g1, x_c12);
		x_y1 = _mm_add_epi16(x_y1, x_temp);
		x_temp = _mm_mulhi_epu16(x_b1, x_c13);
		x_temp = _mm_add_epi16(x_y1, x_temp);
		x_y1 = _mm_srai_epi16(x_temp, 5);

		x_temp = _mm_mulhi_epu16(x_r1, x_c21);
		x_cb1 = _mm_mulhi_epu16(x_g1, x_c22);
		x_cb1 = _mm_add_epi16(x_cb1, x_temp);
		x_temp = _mm_mulhi_epu16(x_b1, x_c23);
		x_cb1 = _mm_sub_epi16(x_temp, x_cb1);
		x_temp = _mm_srai_epi16(x_cb1, 5);
		x_cb1 = _mm_add_epi16(x_temp, x_coff);

		x_temp = _mm_mulhi_epu16(x_r1, x_c31);
		x_cr1 = _mm_mulhi_epu16(x_g1, x_c32);
		x_cr1 = _mm_sub_epi16(x_temp, x_cr1);
		x_temp = _mm_mulhi_epu16(x_b1, x_c33);
		x_cr1 = _mm_sub_epi16(x_cr1, x_temp);
		x_temp = _mm_srai_epi16(x_cr1, 5);
		x_cr1 = _mm_add_epi16(x_temp, x_coff);

		/* pack */
		x_y = _mm_packus_epi16(x_y1, x_zero);
		x_cb = _mm_packus_epi16(x_cb1, x_zero);
		x_cr = _mm_packus_epi16(x_cr1, x_zero);

		/* save */
		*((mlib_s32 *)px_y) = *((mlib_s32 *)&x_y);
		*((mlib_s32 *)px_cb) = *((mlib_s32 *)&x_cb);
		*((mlib_s32 *)px_cr) = *((mlib_s32 *)&x_cr);

		px_y = (__m128i *) (((mlib_s32 *)px_y) + 1);
		px_cb = (__m128i *) (((mlib_s32 *)px_cb) + 1);
		px_cr = (__m128i *) (((mlib_s32 *)px_cr) + 1);

		i += 4;
	}

	/* r/g/b: <=3 bytes */
	for (; i < size; i ++) {
		iTemp = *((mlib_s16 *)px_cmyk);
		ir1 = 0xff - (iTemp & 0xff);
		ig1 = 0xff - ((iTemp >> 8) & 0xff);
		iTemp = *(((mlib_s16 *)px_cmyk) + 1);
		ib1 = 0xff - (iTemp & 0xff);
		ik1 = ((iTemp >> 8) & 0xff);
		px_cmyk = (__m128i *) (((mlib_u8 *)px_cmyk) + 4);

		iy1 = ((c11*ir1 + c12*ig1 + c13*ib1) >> 13);
		icb1 = ((-c21*ir1 - c22*ig1 + c23*ib1) >> 13) + coff;
		icr1 = ((c31*ir1 - c32*ig1 - c33*ib1) >> 13) + coff;

		CLAMP_U8(iy1, *((mlib_u8 *)px_y));
		CLAMP_U8(icb1, *((mlib_u8 *)px_cb));
		CLAMP_U8(icr1, *((mlib_u8 *)px_cr));
		*((mlib_u8 *)px_k) = ik1;

		px_y = (__m128i *) (((mlib_u8 *)px_y) + 1);
		px_cb = (__m128i *) (((mlib_u8 *)px_cb) + 1);
		px_cr = (__m128i *) (((mlib_u8 *)px_cr) + 1);
		px_k = (__m128i *) (((mlib_u8 *)px_k) + 1);

	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
