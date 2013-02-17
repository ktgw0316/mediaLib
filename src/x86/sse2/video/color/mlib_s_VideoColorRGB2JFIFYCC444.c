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

#pragma ident	"@(#)mlib_s_VideoColorRGB2JFIFYCC444.c	9.6	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorRGB2JFIFYCC444 - color conversion from RGB to JFIF
 *                                      YCrCb space
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorRGB2JFIFYCC444(mlib_u8       *y,
 *                                                mlib_u8       *cb,
 *                                                mlib_u8       *cr,
 *                                                const mlib_u8 *rgb,
 *                                                mlib_s32      n)
 *
 * ARGUMENTS
 *      y         Pointer to Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      rgb       Pointer to RGB multicomponent row, 8-byte aligned
 *      n         Length of the y, cb, cr arrays
 *                Note: Length of the rgb array must be 3*n
 *
 * DESCRIPTION
 *
 *      mlib_VideoColorRGB2JFIFYCC444
 *        Y  =  0.29900 * R + 0.58700 * G + 0.11400 * B
 *        Cb = -0.16874 * R - 0.33126 * G + 0.50000 * B  + 128
 *        Cr =  0.50000 * R - 0.41869 * G - 0.08131 * B  + 128
 *
 *        R  = rgb[3*i], G = rgb[3*i+1], B = rgb[3*i+2]
 *        y[i] = Y, cr[i] = Cr, cb[i] = Cb, 0 <= i < n
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

#pragma weak mlib_VideoColorRGB2JFIFYCC444 = __mlib_VideoColorRGB2JFIFYCC444

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorRGB2JFIFYCC444) mlib_VideoColorRGB2JFIFYCC444
	__attribute__((weak, alias("__mlib_VideoColorRGB2JFIFYCC444")));

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

/* *********************************************************** */
mlib_status
mlib_VideoColorRGB2JFIFYCC444_aligned(
	mlib_u8 *y,
	mlib_u8 *cb,
	mlib_u8 *cr,
	const mlib_u8 *rgb,
	mlib_s32 n);

mlib_status
mlib_VideoColorRGB2JFIFYCC444_naligned(
	mlib_u8 *y,
	mlib_u8 *cb,
	mlib_u8 *cr,
	const mlib_u8 *rgb,
	mlib_s32 n);

mlib_status
__mlib_VideoColorRGB2JFIFYCC444(
	mlib_u8 *y,
	mlib_u8 *cb,
	mlib_u8 *cr,
	const mlib_u8 *rgb,
	mlib_s32 n)
{
	if (rgb == NULL || y == NULL || cb == NULL || cr == NULL)
		return (MLIB_NULLPOINTER);

	if (n <= 0)
		return (MLIB_FAILURE);

	if (0 == (0xf & ((mlib_addr)rgb | (mlib_addr)y |
			(mlib_addr)cb | (mlib_addr)cr))) {
		return mlib_VideoColorRGB2JFIFYCC444_aligned(
			y, cb, cr, rgb, n);
	} else {
		return mlib_VideoColorRGB2JFIFYCC444_naligned(
			y, cb, cr, rgb, n);
	}
}

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

/* rgb: 6 bytes */
#define	SeparateRGB6(is, ns, nr, ng, nb)	\
{	\
	nr = (is&0xff) | ((is>>16)&0xff00);	\
	ng = ((is>>8)&0xff) | ((ns<<8)&0xff00);	\
	nb = ((is>>16)&0xff) | (ns&0xff00);	\
}

mlib_status
mlib_VideoColorRGB2JFIFYCC444_aligned(
	mlib_u8 *y,
	mlib_u8 *cb,
	mlib_u8 *cr,
	const mlib_u8 *rgb,
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

	__m128i x_t1, x_t2, x_t3, x_t4, x_t5, x_t6, x_t7, x_t8, x_t9;
	__m128i x_temp;
	mlib_s32 iTemp, ir1, ig1, ib1, i;
	mlib_s32 iy1, icb1, icr1, size;

	__m128i x_rgb0, x_rgb1, x_rgb2, x_r, x_g, x_b, x_y, x_cb, x_cr;
	__m128i x_r1, x_r2, x_g1, x_g2, x_b1, x_b2;
	__m128i x_y1, x_y2, x_cb1, x_cb2, x_cr1, x_cr2;
	__m128i *prgb, *py, *pcb, *pcr;

	prgb = (__m128i *)rgb;
	py = (__m128i *)y;
	pcb = (__m128i *)cb;
	pcr = (__m128i *)cr;
	size = 3*n;
	i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i <= size - 48; i += 48) {
		x_rgb0 = _mm_load_si128(prgb++);
		x_rgb1 = _mm_load_si128(prgb++);
		x_rgb2 = _mm_load_si128(prgb++);

		SeparateRGB48(x_rgb0, x_rgb1, x_rgb2, x_r, x_g, x_b);
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

		/* pack & save */
		x_y1 = _mm_packus_epi16(x_y1, x_y2);
		x_cb1 = _mm_packus_epi16(x_cb1, x_cb2);
		x_cr1 = _mm_packus_epi16(x_cr1, x_cr2);
		_mm_store_si128(py, x_y1);
		_mm_store_si128(pcb, x_cb1);
		_mm_store_si128(pcr, x_cr1);

		py++;
		pcb++;
		pcr++;
	}

	/* r/g/b: 8 bytes */
	if (i <= size - 24)	{
		x_rgb0 = _mm_load_si128(prgb);
		x_rgb1 = _mm_loadl_epi64(prgb+1);
		prgb = (__m128i*) (((__m64*)prgb) + 3);

		SeparateRGB24(x_rgb0, x_rgb1, x_r, x_g, x_b);
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
		_mm_storel_epi64(py, x_y);
		_mm_storel_epi64(pcb, x_cb);
		_mm_storel_epi64(pcr, x_cr);

		py = (__m128i*) (((__m64*)py) + 1);
		pcb = (__m128i*) (((__m64*)pcb) + 1);
		pcr = (__m128i*) (((__m64*)pcr) + 1);

		i += 24;
	}

	/* r/g/b: 4 bytes */
	if (i <= size - 12)	{
		x_rgb0 = _mm_loadl_epi64(prgb);
		x_temp = _mm_cvtsi32_si128(((mlib_s32*)prgb)[2]);
		x_temp = _mm_slli_si128(x_temp, 8);
		x_rgb0 = _mm_or_si128(x_temp, x_rgb0);
		prgb = (__m128i*) (((mlib_s32*)prgb) + 3);

		SeparateRGB12(x_rgb0, x_r, x_g, x_b);
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
		*((mlib_s32 *)py) = *((mlib_s32 *)&x_y);
		*((mlib_s32 *)pcb) = *((mlib_s32 *)&x_cb);
		*((mlib_s32 *)pcr) = *((mlib_s32 *)&x_cr);

		py = (__m128i*) (((mlib_s32*)py) + 1);
		pcb = (__m128i*) (((mlib_s32*)pcb) + 1);
		pcr = (__m128i*) (((mlib_s32*)pcr) + 1);

		i += 12;
	}

	/* r/g/b: <=3 bytes */
	for (; i < size; i += 3) {
		iTemp = *((mlib_s16 *)prgb);
		ib1 = ((mlib_u8 *)prgb)[2];
		prgb = (__m128i*) (((mlib_u8 *)prgb) + 3);

		ir1 = iTemp & 0xff;
		ig1 = (iTemp >> 8) & 0xff;

		iy1 = ((c11*ir1 + c12*ig1 + c13*ib1) >> 13);
		icb1 = ((-c21*ir1 - c22*ig1 + c23*ib1) >> 13) + coff;
		icr1 = ((c31*ir1 - c32*ig1 - c33*ib1) >> 13) + coff;

		CLAMP_U8(iy1, *((mlib_u8*)py));
		CLAMP_U8(icb1, *((mlib_u8*)pcb));
		CLAMP_U8(icr1, *((mlib_u8*)pcr));

		py = (__m128i*) (((mlib_u8*)py) + 1);
		pcb = (__m128i*) (((mlib_u8*)pcb) + 1);
		pcr = (__m128i*) (((mlib_u8*)pcr) + 1);

	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoColorRGB2JFIFYCC444_naligned(
	mlib_u8 *y,
	mlib_u8 *cb,
	mlib_u8 *cr,
	const mlib_u8 *rgb,
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

	__m128i x_t1, x_t2, x_t3, x_t4, x_t5, x_t6, x_t7, x_t8, x_t9;
	__m128i x_temp;
	mlib_s32 iTemp, ir1, ig1, ib1, i;
	mlib_s32 iy1, icb1, icr1, size;

	__m128i x_rgb0, x_rgb1, x_rgb2, x_r, x_g, x_b, x_y, x_cb, x_cr;
	__m128i x_r1, x_r2, x_g1, x_g2, x_b1, x_b2;
	__m128i x_y1, x_y2, x_cb1, x_cb2, x_cr1, x_cr2;
	__m128i *prgb, *py, *pcb, *pcr;

	prgb = (__m128i*)rgb;
	py = (__m128i*)y;
	pcb = (__m128i*)cb;
	pcr = (__m128i*)cr;
	size = 3*n;
	i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i <= size - 48; i += 48)	{
		x_rgb0 = _mm_loadu_si128(prgb++);
		x_rgb1 = _mm_loadu_si128(prgb++);
		x_rgb2 = _mm_loadu_si128(prgb++);

		SeparateRGB48(x_rgb0, x_rgb1, x_rgb2, x_r, x_g, x_b);
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
		_mm_storeu_si128(py, x_y1);
		_mm_storeu_si128(pcb, x_cb1);
		_mm_storeu_si128(pcr, x_cr1);

		py++;
		pcb++;
		pcr++;
	}

	/* r/g/b: 8 bytes */
	if (i <= size - 24)	{
		x_rgb0 = _mm_loadu_si128(prgb);
		x_rgb1 = _mm_loadl_epi64(prgb+1);
		prgb = (__m128i*) (((__m64*)prgb) + 3);

		SeparateRGB24(x_rgb0, x_rgb1, x_r, x_g, x_b);
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
		_mm_storel_epi64(((__m128i *)py), x_y);
		_mm_storel_epi64(((__m128i *)pcb), x_cb);
		_mm_storel_epi64(((__m128i *)pcr), x_cr);

		py = (__m128i*) (((__m64*)py) + 1);
		pcb = (__m128i*) (((__m64*)pcb) + 1);
		pcr = (__m128i*) (((__m64*)pcr) + 1);

		i += 24;
	}

	/* r/g/b: 4 bytes */
	if (i <= size - 12)	{
		x_rgb0 = _mm_loadl_epi64(prgb);
		x_temp = _mm_cvtsi32_si128(((mlib_s32*)prgb)[2]);
		x_temp = _mm_slli_si128(x_temp, 8);
		x_rgb0 = _mm_or_si128(x_temp, x_rgb0);
		prgb = (__m128i*) (((mlib_s32*)prgb) + 3);

		SeparateRGB12(x_rgb0, x_r, x_g, x_b);
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
		*((mlib_s32 *)py) = *((mlib_s32 *)&x_y);
		*((mlib_s32 *)pcb) = *((mlib_s32 *)&x_cb);
		*((mlib_s32 *)pcr) = *((mlib_s32 *)&x_cr);

		py = (__m128i*) (((mlib_s32*)py) + 1);
		pcb = (__m128i*) (((mlib_s32*)pcb) + 1);
		pcr = (__m128i*) (((mlib_s32*)pcr) + 1);

		i += 12;
	}

	/* r/g/b: <=3 bytes */
	for (; i < size; i += 3) {
		iTemp = *((mlib_s16 *)prgb);
		ib1 = ((mlib_u8 *)prgb)[2];
		prgb = (__m128i*) (((mlib_u8 *)prgb) + 3);

		ir1 = iTemp & 0xff;
		ig1 = (iTemp >> 8) & 0xff;

		iy1 = ((c11*ir1 + c12*ig1 + c13*ib1) >> 13);
		icb1 = ((-c21*ir1 - c22*ig1 + c23*ib1) >> 13) + coff;
		icr1 = ((c31*ir1 - c32*ig1 - c33*ib1) >> 13) + coff;

		CLAMP_U8(iy1, *((mlib_u8*)py));
		CLAMP_U8(icb1, *((mlib_u8*)pcb));
		CLAMP_U8(icr1, *((mlib_u8*)pcr));

		py = (__m128i*) (((mlib_u8*)py) + 1);
		pcb = (__m128i*) (((mlib_u8*)pcb) + 1);
		pcr = (__m128i*) (((mlib_u8*)pcr) + 1);

	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
