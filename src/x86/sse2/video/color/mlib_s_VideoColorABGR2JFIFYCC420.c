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

#pragma ident	"@(#)mlib_s_VideoColorABGR2JFIFYCC420.c	9.6	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorABGR2JFIFYCC420 - color conversion with
 *                                       420 downsampling with bilinear filter
 *                                       Only Cr and Cb components are
 *                                       under upsampling.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorABGR2JFIFYCC420(mlib_u8        *y0,
 *                                                 mlib_u8        *y1,
 *                                                 mlib_u8        *cb,
 *                                                 mlib_u8        *cr,
 *                                                 const mlib_u8  *abgr0,
 *                                                 const mlib_u8  *abgr1,
 *                                                 mlib_s32        n)
 *
 * ARGUMENTS
 *      y0        Pointer to upper Y component row, 8-byte aligned
 *      y1        Pointer to lower Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      abgr0     Pointer to upper ABGR multicomponent row, 8-byte aligned
 *      abgr1     Pointer to lower ABGR multicomponent row, 8-byte aligned
 *      n         Length of y0 and y1 arrays
 *                Note: n must be even,
 *                      length of cb, cr arrays must be n/2
 *                      length of rgb0, rgb1 arrays must be 4*n
 *
 * DESCRIPTION
 *
 *      mlib_VideoColorABGR2JFIFYCC420:
 *        composition of mlib_VideoColorABGR2JFIFYCC444 with
 *                       mlib_VideoDownSample420
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

#pragma weak mlib_VideoColorABGR2JFIFYCC420 = \
	__mlib_VideoColorABGR2JFIFYCC420

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorABGR2JFIFYCC420) mlib_VideoColorABGR2JFIFYCC420
	__attribute__((weak, alias("__mlib_VideoColorABGR2JFIFYCC420")));

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

/* abgr: 64 bytes */
#define	SeparateABGR64(x_abgr0, x_abgr1, x_abgr2, x_abgr3, x_r, x_g, x_b) \
{	\
	x_t1 = _mm_unpacklo_epi8(x_abgr0, x_abgr1);	\
	x_t2 = _mm_unpackhi_epi8(x_abgr0, x_abgr1);	\
	x_t3 = _mm_unpacklo_epi8(x_abgr2, x_abgr3);	\
	x_t4 = _mm_unpackhi_epi8(x_abgr2, x_abgr3);	\
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
	x_b = _mm_unpackhi_epi64(x_t1, x_t3);		\
	x_g = _mm_unpacklo_epi64(x_t2, x_t4);		\
	x_r = _mm_unpackhi_epi64(x_t2, x_t4);		\
}

/* abgr: 32 bytes */
#define	SeparateABGR32(x_abgr0, x_abgr1, x_r, x_g, x_b) \
{	\
	x_t1 = _mm_unpacklo_epi8(x_abgr0, x_abgr1);	\
	x_t2 = _mm_unpackhi_epi8(x_abgr0, x_abgr1);	\
							\
	x_t5 = _mm_unpacklo_epi8(x_t1, x_t2);		\
	x_t6 = _mm_unpackhi_epi8(x_t1, x_t2);		\
							\
	x_t1 = _mm_unpacklo_epi8(x_t5, x_t6);		\
	x_t2 = _mm_unpackhi_epi8(x_t5, x_t6);		\
							\
	x_b = _mm_unpackhi_epi64(x_t1, x_zero);		\
	x_g = _mm_unpacklo_epi64(x_t2, x_zero);		\
	x_r = _mm_unpackhi_epi64(x_t2, x_zero);		\
}

/* abgr: 16 bytes */
#define	SeparateABGR16(x_abgr0, x_r, x_g, x_b) \
{	\
	x_t1 = _mm_unpacklo_epi8(x_abgr0, x_zero);	\
	x_t2 = _mm_unpackhi_epi8(x_abgr0, x_zero);	\
							\
	x_t5 = _mm_unpacklo_epi8(x_t1, x_t2);		\
	x_t6 = _mm_unpackhi_epi8(x_t1, x_t2);		\
							\
	x_t1 = _mm_unpacklo_epi8(x_t5, x_t6);		\
	x_t2 = _mm_unpackhi_epi8(x_t5, x_t6);		\
							\
	x_b = _mm_unpackhi_epi64(x_t1, x_zero);		\
	x_g = _mm_unpacklo_epi64(x_t2, x_zero);		\
	x_r = _mm_unpackhi_epi64(x_t2, x_zero);		\
}

/* *********************************************************** */
mlib_status
mlib_VideoColorABGR2JFIFYCC420_aligned(
	mlib_u8 *y0,
	mlib_u8 *y1,
	mlib_u8 *cb,
	mlib_u8 *cr,
	const mlib_u8 *abgr0,
	const mlib_u8 *abgr1,
	mlib_s32 n);

mlib_status
mlib_VideoColorABGR2JFIFYCC420_naligned(
	mlib_u8 *y0,
	mlib_u8 *y1,
	mlib_u8 *cb,
	mlib_u8 *cr,
	const mlib_u8 *abgr0,
	const mlib_u8 *abgr1,
	mlib_s32 n);

mlib_status
__mlib_VideoColorABGR2JFIFYCC420(
	mlib_u8 *y0,
	mlib_u8 *y1,
	mlib_u8 *cb,
	mlib_u8 *cr,
	const mlib_u8 *abgr0,
	const mlib_u8 *abgr1,
	mlib_s32 n)
{
	if (abgr0 == NULL || abgr1 == NULL || y0 == NULL || y1 == NULL ||
		cb == NULL || cr == NULL)
			return (MLIB_NULLPOINTER);

	if (n <= 0)
		return (MLIB_FAILURE);

	if (0 == (0xf & ((mlib_addr)abgr0 | (mlib_addr)abgr1 |
			(mlib_addr)y0 | (mlib_addr)y1 |
			(mlib_addr)cb | (mlib_addr)cr))) {
			return mlib_VideoColorABGR2JFIFYCC420_aligned(
				y0, y1, cb, cr, abgr0, abgr1, n);
	} else {
			return mlib_VideoColorABGR2JFIFYCC420_naligned(
				y0, y1, cb, cr, abgr0, abgr1, n);
	}
}

mlib_status
mlib_VideoColorABGR2JFIFYCC420_aligned(
	mlib_u8 *y0,
	mlib_u8 *y1,
	mlib_u8 *cb,
	mlib_u8 *cr,
	const mlib_u8 *abgr0,
	const mlib_u8 *abgr1,
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

	const __m128i x_mask = _mm_set1_epi16(0xff);

	const __m128i x_off1 = _mm_set1_epi32(0x10000);

	__m128i x_t1, x_t2, x_t3, x_t4, x_t5, x_t6, x_t7, x_t8;
	__m128i x_temp;
	mlib_s32 iTemp, ir1, ir2, ig1, ig2, ib1, ib2, i;
	mlib_s32 iy1, iy2, icb1, icb2, icr1, icr2, size;
	mlib_s32 icbbak, icrbak, icb, icr;

	__m128i x_abgr0, x_abgr1, x_abgr2, x_abgr3;
	__m128i x_r, x_g, x_b, x_y, x_cb, x_cr;
	__m128i x_r1, x_r2, x_g1, x_g2, x_b1, x_b2;
	__m128i x_y1, x_y2, x_cb1, x_cb2, x_cr1, x_cr2;
	__m128i x_ybak, x_cbbak, x_crbak;
	__m128i *pabgr0, *pabgr1, *py0, *py1, *pcb, *pcr;

	pabgr0 = (__m128i*)abgr0;
	pabgr1 = (__m128i*)abgr1;
	py0 = (__m128i*)y0;
	py1 = (__m128i*)y1;
	pcb = (__m128i*)cb;
	pcr = (__m128i*)cr;
	size = 4*n;
	i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i <= size - 64; i += 64)	{
		x_abgr0 = _mm_load_si128(pabgr0++);
		x_abgr1 = _mm_load_si128(pabgr0++);
		x_abgr2 = _mm_load_si128(pabgr0++);
		x_abgr3 = _mm_load_si128(pabgr0++);

		SeparateABGR64(x_abgr0, x_abgr1, x_abgr2, x_abgr3,
			x_r, x_g, x_b);
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
		x_ybak = _mm_packus_epi16(x_y1, x_y2);
		x_cb1 = _mm_packus_epi16(x_cb1, x_cb2);
		x_cb2 = _mm_srli_si128(x_cb1, 1);
		x_cb = _mm_avg_epu8(x_cb1, x_cb2);
		x_cbbak = _mm_and_si128(x_cb, x_mask);
		x_cr1 = _mm_packus_epi16(x_cr1, x_cr2);
		x_cr2 = _mm_srli_si128(x_cr1, 1);
		x_cr = _mm_avg_epu8(x_cr1, x_cr2);
		x_crbak = _mm_and_si128(x_cr, x_mask);

		/* load */
		x_abgr0 = _mm_load_si128(pabgr1++);
		x_abgr1 = _mm_load_si128(pabgr1++);
		x_abgr2 = _mm_load_si128(pabgr1++);
		x_abgr3 = _mm_load_si128(pabgr1++);

		SeparateABGR64(x_abgr0, x_abgr1, x_abgr2, x_abgr3,
			x_r, x_g, x_b);
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
		x_y = _mm_packus_epi16(x_y1, x_y2);
		x_cb1 = _mm_packus_epi16(x_cb1, x_cb2);
		x_cb2 = _mm_srli_si128(x_cb1, 1);
		x_cb = _mm_avg_epu8(x_cb1, x_cb2);
		x_cb = _mm_and_si128(x_cb, x_mask);
		x_cr1 = _mm_packus_epi16(x_cr1, x_cr2);
		x_cr2 = _mm_srli_si128(x_cr1, 1);
		x_cr = _mm_avg_epu8(x_cr1, x_cr2);
		x_cr = _mm_and_si128(x_cr, x_mask);

		/* cb & cr */
		x_cb = _mm_add_epi16(x_off1, x_cb);
		x_cb = _mm_avg_epu16(x_cb, x_cbbak);
		x_cb = _mm_packus_epi16(x_cb, x_cb);
		x_cr = _mm_add_epi16(x_off1, x_cr);
		x_cr = _mm_avg_epu16(x_cr, x_crbak);
		x_cr = _mm_packus_epi16(x_cr, x_cr);

		/* save */
		_mm_store_si128(py0, x_ybak);
		_mm_store_si128(py1, x_y);
		_mm_storel_epi64(pcb, x_cb);
		_mm_storel_epi64(pcr, x_cr);

		py0++;
		py1++;
		pcb = (__m128i*)(((__m64*)pcb) + 1);
		pcr = (__m128i*)(((__m64*)pcr) + 1);
	}

	/* r/g/b: 8 bytes */
	if (i <= size - 32)	{
		x_abgr0 = _mm_load_si128(pabgr0++);
		x_abgr1 = _mm_load_si128(pabgr0++);

		SeparateABGR32(x_abgr0, x_abgr1, x_r, x_g, x_b);
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
		x_ybak = _mm_packus_epi16(x_y1, x_y1);
		x_cb1 = _mm_packus_epi16(x_cb1, x_cb1);
		x_cb2 = _mm_srli_si128(x_cb1, 1);
		x_cb = _mm_avg_epu8(x_cb1, x_cb2);
		x_cbbak = _mm_and_si128(x_cb, x_mask);
		x_cr1 = _mm_packus_epi16(x_cr1, x_cr1);
		x_cr2 = _mm_srli_si128(x_cr1, 1);
		x_cr = _mm_avg_epu8(x_cr1, x_cr2);
		x_crbak = _mm_and_si128(x_cr, x_mask);

		/* load */
		x_abgr0 = _mm_load_si128(pabgr1++);
		x_abgr1 = _mm_load_si128(pabgr1++);

		SeparateABGR32(x_abgr0, x_abgr1, x_r, x_g, x_b);
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
		x_y = _mm_packus_epi16(x_y1, x_y1);
		x_cb1 = _mm_packus_epi16(x_cb1, x_cb1);
		x_cb2 = _mm_srli_si128(x_cb1, 1);
		x_cb = _mm_avg_epu8(x_cb1, x_cb2);
		x_cb = _mm_and_si128(x_cb, x_mask);
		x_cr1 = _mm_packus_epi16(x_cr1, x_cr1);
		x_cr2 = _mm_srli_si128(x_cr1, 1);
		x_cr = _mm_avg_epu8(x_cr1, x_cr2);
		x_cr = _mm_and_si128(x_cr, x_mask);

		/* cb & cr */
		x_cb = _mm_add_epi16(x_off1, x_cb);
		x_cb = _mm_avg_epu16(x_cb, x_cbbak);
		x_cb = _mm_packus_epi16(x_cb, x_cb);
		x_cr = _mm_add_epi16(x_off1, x_cr);
		x_cr = _mm_avg_epu16(x_cr, x_crbak);
		x_cr = _mm_packus_epi16(x_cr, x_cr);
		icb1 = _mm_cvtsi128_si32(x_cb);
		icr1 = _mm_cvtsi128_si32(x_cr);

		/* save */
		_mm_storel_epi64(py0, x_ybak);
		_mm_storel_epi64(py1, x_y);
		*((mlib_s32*)pcb) = icb1;
		*((mlib_s32*)pcr) = icr1;

		py0 = (__m128i*) (((__m64*)py0) + 1);
		py1 = (__m128i*) (((__m64*)py1) + 1);
		pcb = (__m128i*) (((mlib_s32*)pcb) + 1);
		pcr = (__m128i*) (((mlib_s32*)pcr) + 1);

		i += 32;
	}

	/* r/g/b: 4 bytes */
	if (i <= size - 16)	{
		x_abgr0 = _mm_load_si128(pabgr0++);

		SeparateABGR16(x_abgr0, x_r, x_g, x_b);
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
		x_ybak = _mm_packus_epi16(x_y1, x_y1);
		x_cb1 = _mm_packus_epi16(x_cb1, x_cb1);
		x_cb2 = _mm_srli_si128(x_cb1, 1);
		x_cb = _mm_avg_epu8(x_cb1, x_cb2);
		x_cbbak = _mm_and_si128(x_cb, x_mask);
		x_cr1 = _mm_packus_epi16(x_cr1, x_cr1);
		x_cr2 = _mm_srli_si128(x_cr1, 1);
		x_cr = _mm_avg_epu8(x_cr1, x_cr2);
		x_crbak = _mm_and_si128(x_cr, x_mask);

		/* load */
		x_abgr0 = _mm_load_si128(pabgr1++);

		SeparateABGR16(x_abgr0, x_r, x_g, x_b);
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
		x_y = _mm_packus_epi16(x_y1, x_y1);
		x_cb1 = _mm_packus_epi16(x_cb1, x_cb1);
		x_cb2 = _mm_srli_si128(x_cb1, 1);
		x_cb = _mm_avg_epu8(x_cb1, x_cb2);
		x_cb = _mm_and_si128(x_cb, x_mask);
		x_cr1 = _mm_packus_epi16(x_cr1, x_cr1);
		x_cr2 = _mm_srli_si128(x_cr1, 1);
		x_cr = _mm_avg_epu8(x_cr1, x_cr2);
		x_cr = _mm_and_si128(x_cr, x_mask);

		/* cb & cr */
		x_cb = _mm_add_epi16(x_off1, x_cb);
		x_cb = _mm_avg_epu16(x_cb, x_cbbak);
		x_cb = _mm_packus_epi16(x_cb, x_cb);
		x_cr = _mm_add_epi16(x_off1, x_cr);
		x_cr = _mm_avg_epu16(x_cr, x_crbak);
		x_cr = _mm_packus_epi16(x_cr, x_cr);

		/* save */
		*((mlib_s32 *)py0) = *((mlib_s32 *)&x_ybak);
		*((mlib_s32 *)py1) = *((mlib_s32 *)&x_y);
		*((mlib_s16 *)pcb) = *((mlib_s16 *)&x_cb);
		*((mlib_s16 *)pcr) = *((mlib_s16 *)&x_cr);

		py0 = (__m128i*) (((mlib_s32*)py0) + 1);
		py1 = (__m128i*) (((mlib_s32*)py1) + 1);
		pcb = (__m128i*) (((mlib_s16*)pcb) + 1);
		pcr = (__m128i*) (((mlib_s16*)pcr) + 1);

		i += 16;
	}

	/* r/g/b: ==2 bytes */
	if (i <= size - 8) {
		/* 1st r/g/b */
		iTemp = *((mlib_s32 *)pabgr0);
		ib1 = (iTemp >> 8) & 0xff;
		ig1 = (iTemp >> 16) & 0xff;
		ir1 = (iTemp >> 24) & 0xff;
		pabgr0 = (__m128i*) (((mlib_u8 *)pabgr0) + 4);

		iy1 = ((c11*ir1 + c12*ig1 + c13*ib1) >> 13);
		icb1 = ((-c21*ir1 - c22*ig1 + c23*ib1) >> 13) + coff;
		icr1 = ((c31*ir1 - c32*ig1 - c33*ib1) >> 13) + coff;

		/* 2nd r/g/b */
		iTemp = *((mlib_s32 *)pabgr0);
		ib2 = (iTemp >> 8) & 0xff;
		ig2 = (iTemp >> 16) & 0xff;
		ir2 = (iTemp >> 24) & 0xff;
		pabgr0 = (__m128i*) (((mlib_u8 *)pabgr0) + 4);

		iy2 = ((c11*ir2 + c12*ig2 + c13*ib2) >> 13);
		icb2 = ((-c21*ir2 - c22*ig2 + c23*ib2) >> 13) + coff;
		icr2 = ((c31*ir2 - c32*ig2 - c33*ib2) >> 13) + coff;
		icbbak = icb1 + icb2;
		icrbak = icr1 + icr2;

		/* save y */
		CLAMP_U8(iy1, *((mlib_u8*)py0));
		CLAMP_U8(iy2, *(((mlib_u8*)py0) + 1));
		py0 = (__m128i*) (((mlib_u8*)py0) + 2);

		/* 1st r/g/b */
		iTemp = *((mlib_s32 *)pabgr1);
		ib1 = (iTemp >> 8) & 0xff;
		ig1 = (iTemp >> 16) & 0xff;
		ir1 = (iTemp >> 24) & 0xff;
		pabgr1 = (__m128i*) (((mlib_u8 *)pabgr1) + 4);

		iy1 = ((c11*ir1 + c12*ig1 + c13*ib1) >> 13);
		icb1 = ((-c21*ir1 - c22*ig1 + c23*ib1) >> 13) + coff;
		icr1 = ((c31*ir1 - c32*ig1 - c33*ib1) >> 13) + coff;

		/* 2nd r/g/b */
		iTemp = *((mlib_s32 *)pabgr1);
		ib2 = (iTemp >> 8) & 0xff;
		ig2 = (iTemp >> 16) & 0xff;
		ir2 = (iTemp >> 24) & 0xff;
		pabgr1 = (__m128i*) (((mlib_u8 *)pabgr1) + 4);

		iy2 = ((c11*ir2 + c12*ig2 + c13*ib2) >> 13);
		icb2 = ((-c21*ir2 - c22*ig2 + c23*ib2) >> 13) + coff;
		icr2 = ((c31*ir2 - c32*ig2 - c33*ib2) >> 13) + coff;
		icb = icb1 + icb2;
		icr = icr1 + icr2;

		/* save y */
		CLAMP_U8(iy1, *((mlib_u8*)py1));
		CLAMP_U8(iy2, *(((mlib_u8*)py1) + 1));
		py1 = (__m128i*) (((mlib_u8*)py1) + 2);

		/* cb & cr */
		CLAMP_U8((icbbak+icb)/4, *((mlib_u8*)pcb));
		CLAMP_U8((icrbak+icr)/4, *((mlib_u8*)pcr));

		pcb = (__m128i*) (((mlib_u8*)pcb) + 1);
		pcr = (__m128i*) (((mlib_u8*)pcr) + 1);
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoColorABGR2JFIFYCC420_naligned(
	mlib_u8 *y0,
	mlib_u8 *y1,
	mlib_u8 *cb,
	mlib_u8 *cr,
	const mlib_u8 *abgr0,
	const mlib_u8 *abgr1,
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

	const __m128i x_mask = _mm_set1_epi16(0xff);

	const __m128i x_off1 = _mm_set1_epi32(0x10000);

	__m128i x_t1, x_t2, x_t3, x_t4, x_t5, x_t6, x_t7, x_t8;
	__m128i x_temp;
	mlib_s32 iTemp, ir1, ir2, ig1, ig2, ib1, ib2, i;
	mlib_s32 iy1, iy2, icb1, icb2, icr1, icr2, size;
	mlib_s32 icbbak, icrbak, icb, icr;

	__m128i x_abgr0, x_abgr1, x_abgr2, x_abgr3;
	__m128i x_r, x_g, x_b, x_y, x_cb, x_cr;
	__m128i x_r1, x_r2, x_g1, x_g2, x_b1, x_b2;
	__m128i x_y1, x_y2, x_cb1, x_cb2, x_cr1, x_cr2;
	__m128i x_ybak, x_cbbak, x_crbak;
	__m128i *pabgr0, *pabgr1, *py0, *py1, *pcb, *pcr;

	pabgr0 = (__m128i*)abgr0;
	pabgr1 = (__m128i*)abgr1;
	py0 = (__m128i*)y0;
	py1 = (__m128i*)y1;
	pcb = (__m128i*)cb;
	pcr = (__m128i*)cr;
	size = 4*n;
	i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i <= size - 64; i += 64)	{
		x_abgr0 = _mm_loadu_si128(pabgr0++);
		x_abgr1 = _mm_loadu_si128(pabgr0++);
		x_abgr2 = _mm_loadu_si128(pabgr0++);
		x_abgr3 = _mm_loadu_si128(pabgr0++);

		SeparateABGR64(x_abgr0, x_abgr1, x_abgr2, x_abgr3,
			x_r, x_g, x_b);
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
		x_ybak = _mm_packus_epi16(x_y1, x_y2);
		x_cb1 = _mm_packus_epi16(x_cb1, x_cb2);
		x_cb2 = _mm_srli_si128(x_cb1, 1);
		x_cb = _mm_avg_epu8(x_cb1, x_cb2);
		x_cbbak = _mm_and_si128(x_cb, x_mask);
		x_cr1 = _mm_packus_epi16(x_cr1, x_cr2);
		x_cr2 = _mm_srli_si128(x_cr1, 1);
		x_cr = _mm_avg_epu8(x_cr1, x_cr2);
		x_crbak = _mm_and_si128(x_cr, x_mask);

		/* load */
		x_abgr0 = _mm_loadu_si128(pabgr1++);
		x_abgr1 = _mm_loadu_si128(pabgr1++);
		x_abgr2 = _mm_loadu_si128(pabgr1++);
		x_abgr3 = _mm_loadu_si128(pabgr1++);

		SeparateABGR64(x_abgr0, x_abgr1, x_abgr2, x_abgr3,
			x_r, x_g, x_b);
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
		x_y = _mm_packus_epi16(x_y1, x_y2);
		x_cb1 = _mm_packus_epi16(x_cb1, x_cb2);
		x_cb2 = _mm_srli_si128(x_cb1, 1);
		x_cb = _mm_avg_epu8(x_cb1, x_cb2);
		x_cb = _mm_and_si128(x_cb, x_mask);
		x_cr1 = _mm_packus_epi16(x_cr1, x_cr2);
		x_cr2 = _mm_srli_si128(x_cr1, 1);
		x_cr = _mm_avg_epu8(x_cr1, x_cr2);
		x_cr = _mm_and_si128(x_cr, x_mask);

		/* cb & cr */
		x_cb = _mm_add_epi16(x_off1, x_cb);
		x_cb = _mm_avg_epu16(x_cb, x_cbbak);
		x_cb = _mm_packus_epi16(x_cb, x_cb);
		x_cr = _mm_add_epi16(x_off1, x_cr);
		x_cr = _mm_avg_epu16(x_cr, x_crbak);
		x_cr = _mm_packus_epi16(x_cr, x_cr);

		/* save */
		_mm_storeu_si128(py0, x_ybak);
		_mm_storeu_si128(py1, x_y);
		_mm_storel_epi64(pcb, x_cb);
		_mm_storel_epi64(pcr, x_cr);

		py0++;
		py1++;
		pcb = (__m128i*)(((__m64*)pcb) + 1);
		pcr = (__m128i*)(((__m64*)pcr) + 1);
	}

	/* r/g/b: 8 bytes */
	if (i <= size - 32)	{
		x_abgr0 = _mm_loadu_si128(pabgr0++);
		x_abgr1 = _mm_loadu_si128(pabgr0++);

		SeparateABGR32(x_abgr0, x_abgr1, x_r, x_g, x_b);
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
		x_ybak = _mm_packus_epi16(x_y1, x_y1);
		x_cb1 = _mm_packus_epi16(x_cb1, x_cb1);
		x_cb2 = _mm_srli_si128(x_cb1, 1);
		x_cb = _mm_avg_epu8(x_cb1, x_cb2);
		x_cbbak = _mm_and_si128(x_cb, x_mask);
		x_cr1 = _mm_packus_epi16(x_cr1, x_cr1);
		x_cr2 = _mm_srli_si128(x_cr1, 1);
		x_cr = _mm_avg_epu8(x_cr1, x_cr2);
		x_crbak = _mm_and_si128(x_cr, x_mask);

		/* load */
		x_abgr0 = _mm_loadu_si128(pabgr1++);
		x_abgr1 = _mm_loadu_si128(pabgr1++);

		SeparateABGR32(x_abgr0, x_abgr1, x_r, x_g, x_b);
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
		x_y = _mm_packus_epi16(x_y1, x_y1);
		x_cb1 = _mm_packus_epi16(x_cb1, x_cb1);
		x_cb2 = _mm_srli_si128(x_cb1, 1);
		x_cb = _mm_avg_epu8(x_cb1, x_cb2);
		x_cb = _mm_and_si128(x_cb, x_mask);
		x_cr1 = _mm_packus_epi16(x_cr1, x_cr1);
		x_cr2 = _mm_srli_si128(x_cr1, 1);
		x_cr = _mm_avg_epu8(x_cr1, x_cr2);
		x_cr = _mm_and_si128(x_cr, x_mask);

		/* cb & cr */
		x_cb = _mm_add_epi16(x_off1, x_cb);
		x_cb = _mm_avg_epu16(x_cb, x_cbbak);
		x_cb = _mm_packus_epi16(x_cb, x_cb);
		x_cr = _mm_add_epi16(x_off1, x_cr);
		x_cr = _mm_avg_epu16(x_cr, x_crbak);
		x_cr = _mm_packus_epi16(x_cr, x_cr);
		icb1 = _mm_cvtsi128_si32(x_cb);
		icr1 = _mm_cvtsi128_si32(x_cr);

		/* save */
		_mm_storel_epi64(py0, x_ybak);
		_mm_storel_epi64(py1, x_y);
		*((mlib_s32*)pcb) = icb1;
		*((mlib_s32*)pcr) = icr1;

		py0 = (__m128i*) (((__m64*)py0) + 1);
		py1 = (__m128i*) (((__m64*)py1) + 1);
		pcb = (__m128i*) (((mlib_s32*)pcb) + 1);
		pcr = (__m128i*) (((mlib_s32*)pcr) + 1);

		i += 32;
	}

	/* r/g/b: 4 bytes */
	if (i <= size - 16)	{
		x_abgr0 = _mm_loadu_si128(pabgr0++);
		x_abgr1 = x_abgr2 = x_abgr3 = x_zero;

		SeparateABGR16(x_abgr0, x_r, x_g, x_b);
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
		x_ybak = _mm_packus_epi16(x_y1, x_y1);
		x_cb1 = _mm_packus_epi16(x_cb1, x_cb1);
		x_cb2 = _mm_srli_si128(x_cb1, 1);
		x_cb = _mm_avg_epu8(x_cb1, x_cb2);
		x_cbbak = _mm_and_si128(x_cb, x_mask);
		x_cr1 = _mm_packus_epi16(x_cr1, x_cr1);
		x_cr2 = _mm_srli_si128(x_cr1, 1);
		x_cr = _mm_avg_epu8(x_cr1, x_cr2);
		x_crbak = _mm_and_si128(x_cr, x_mask);

		/* load */
		x_abgr0 = _mm_loadu_si128(pabgr1++);

		SeparateABGR16(x_abgr0, x_r, x_g, x_b);
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
		x_y = _mm_packus_epi16(x_y1, x_y1);
		x_cb1 = _mm_packus_epi16(x_cb1, x_cb1);
		x_cb2 = _mm_srli_si128(x_cb1, 1);
		x_cb = _mm_avg_epu8(x_cb1, x_cb2);
		x_cb = _mm_and_si128(x_cb, x_mask);
		x_cr1 = _mm_packus_epi16(x_cr1, x_cr1);
		x_cr2 = _mm_srli_si128(x_cr1, 1);
		x_cr = _mm_avg_epu8(x_cr1, x_cr2);
		x_cr = _mm_and_si128(x_cr, x_mask);

		/* cb & cr */
		x_cb = _mm_add_epi16(x_off1, x_cb);
		x_cb = _mm_avg_epu16(x_cb, x_cbbak);
		x_cb = _mm_packus_epi16(x_cb, x_cb);
		x_cr = _mm_add_epi16(x_off1, x_cr);
		x_cr = _mm_avg_epu16(x_cr, x_crbak);
		x_cr = _mm_packus_epi16(x_cr, x_cr);

		/* save */
		*((mlib_s32 *)py0) = *((mlib_s32 *)&x_ybak);
		*((mlib_s32 *)py1) = *((mlib_s32 *)&x_y);
		*((mlib_s16 *)pcb) = *((mlib_s16 *)&x_cb);
		*((mlib_s16 *)pcr) = *((mlib_s16 *)&x_cr);

		py0 = (__m128i*) (((mlib_s32*)py0) + 1);
		py1 = (__m128i*) (((mlib_s32*)py1) + 1);
		pcb = (__m128i*) (((mlib_s16*)pcb) + 1);
		pcr = (__m128i*) (((mlib_s16*)pcr) + 1);

		i += 16;
	}

	/* r/g/b: ==2 bytes */
	if (i <= size - 8) {
		/* 1st r/g/b */
		iTemp = *((mlib_s32 *)pabgr0);
		ib1 = (iTemp >> 8) & 0xff;
		ig1 = (iTemp >> 16) & 0xff;
		ir1 = (iTemp >> 24) & 0xff;
		pabgr0 = (__m128i*) (((mlib_u8 *)pabgr0) + 4);

		iy1 = ((c11*ir1 + c12*ig1 + c13*ib1) >> 13);
		icb1 = ((-c21*ir1 - c22*ig1 + c23*ib1) >> 13) + coff;
		icr1 = ((c31*ir1 - c32*ig1 - c33*ib1) >> 13) + coff;

		/* 2nd r/g/b */
		iTemp = *((mlib_s32 *)pabgr0);
		ib2 = (iTemp >> 8) & 0xff;
		ig2 = (iTemp >> 16) & 0xff;
		ir2 = (iTemp >> 24) & 0xff;
		pabgr0 = (__m128i*) (((mlib_u8 *)pabgr0) + 4);

		iy2 = ((c11*ir2 + c12*ig2 + c13*ib2) >> 13);
		icb2 = ((-c21*ir2 - c22*ig2 + c23*ib2) >> 13) + coff;
		icr2 = ((c31*ir2 - c32*ig2 - c33*ib2) >> 13) + coff;
		icbbak = icb1 + icb2;
		icrbak = icr1 + icr2;

		/* save y */
		CLAMP_U8(iy1, *((mlib_u8*)py0));
		CLAMP_U8(iy2, *(((mlib_u8*)py0) + 1));
		py0 = (__m128i*) (((mlib_u8*)py0) + 2);

		/* 1st r/g/b */
		iTemp = *((mlib_s32 *)pabgr1);
		ib1 = (iTemp >> 8) & 0xff;
		ig1 = (iTemp >> 16) & 0xff;
		ir1 = (iTemp >> 24) & 0xff;
		pabgr1 = (__m128i*) (((mlib_u8 *)pabgr1) + 4);

		iy1 = ((c11*ir1 + c12*ig1 + c13*ib1) >> 13);
		icb1 = ((-c21*ir1 - c22*ig1 + c23*ib1) >> 13) + coff;
		icr1 = ((c31*ir1 - c32*ig1 - c33*ib1) >> 13) + coff;

		/* 2nd r/g/b */
		iTemp = *((mlib_s32 *)pabgr1);
		ib2 = (iTemp >> 8) & 0xff;
		ig2 = (iTemp >> 16) & 0xff;
		ir2 = (iTemp >> 24) & 0xff;
		pabgr1 = (__m128i*) (((mlib_u8 *)pabgr1) + 4);

		iy2 = ((c11*ir2 + c12*ig2 + c13*ib2) >> 13);
		icb2 = ((-c21*ir2 - c22*ig2 + c23*ib2) >> 13) + coff;
		icr2 = ((c31*ir2 - c32*ig2 - c33*ib2) >> 13) + coff;
		icb = icb1 + icb2;
		icr = icr1 + icr2;

		/* save y */
		CLAMP_U8(iy1, *((mlib_u8*)py1));
		CLAMP_U8(iy2, *(((mlib_u8*)py1) + 1));
		py1 = (__m128i*) (((mlib_u8*)py1) + 2);

		/* cb & cr */
		CLAMP_U8((icbbak+icb)/4, *((mlib_u8*)pcb));
		CLAMP_U8((icrbak+icr)/4, *((mlib_u8*)pcr));

		pcb = (__m128i*) (((mlib_u8*)pcb) + 1);
		pcr = (__m128i*) (((mlib_u8*)pcr) + 1);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
