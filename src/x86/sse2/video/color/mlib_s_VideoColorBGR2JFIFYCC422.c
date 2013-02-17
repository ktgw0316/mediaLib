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

#pragma ident	"@(#)mlib_s_VideoColorBGR2JFIFYCC422.c	9.7	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorrBGR2JFIFYCC422 - color conversion with
 *                                      422 downsampling with bilinear filter
 *                                      Only Cr and Cb components are
 *                                      under upsampling.
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorBGR2JFIFYCC422(mlib_u8       *y,
 *                                                mlib_u8       *cb,
 *                                                mlib_u8       *cr,
 *                                                const mlib_u8 *bgr,
 *                                                mlib_s32      n)
 *
 * ARGUMENTS
 *      y         Pointer to Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      bgr       Pointer to BGR multicomponent row, 8-byte aligned
 *      n         Length of y array
 *                Note: n must be even,
 *                      length of cb, cr arrays must be n/2
 *                      length of bgr array must be 3*n
 *
 * DESCRIPTION
 *      mlib_VideoColorBGR2JFIFYCC422:
 *        composition of mlib_VideoColorBGR2JFIFYCC444 with
 *                       mlib_VideoDownSample422
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

#pragma weak mlib_VideoColorBGR2JFIFYCC422 = __mlib_VideoColorBGR2JFIFYCC422

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorBGR2JFIFYCC422) mlib_VideoColorBGR2JFIFYCC422
	__attribute__((weak, alias("__mlib_VideoColorBGR2JFIFYCC422")));

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

/* bgr: 48 bytes */
#define	SeparateBGR48(x_bgr0, x_bgr1, x_bgr2, x_b, x_g,  x_r)	\
{	\
	x_t1 = _mm_unpacklo_epi8(x_bgr0, _mm_srli_si128(x_bgr1, 8));	\
	x_t2 = _mm_unpacklo_epi8(_mm_srli_si128(x_bgr0, 8), x_bgr2);	\
	x_t3 = _mm_unpacklo_epi8(x_bgr1, _mm_srli_si128(x_bgr2, 8));	\
	x_t4 = _mm_unpacklo_epi8(x_t1, _mm_srli_si128(x_t2, 8));	\
	x_t5 = _mm_unpacklo_epi8(_mm_srli_si128(x_t1, 8), x_t3);	\
	\
	x_t6 = _mm_unpacklo_epi8(x_t4, _mm_srli_si128(x_t5, 8));	\
	x_t7 = _mm_unpacklo_epi8(x_t2, _mm_srli_si128(x_t3, 8));	\
	x_t8 = _mm_unpacklo_epi8(_mm_srli_si128(x_t4, 8), x_t7);	\
	x_t9 = _mm_unpacklo_epi8(x_t5, _mm_srli_si128(x_t7, 8));	\
	\
	x_b = _mm_unpacklo_epi8(x_t6, _mm_srli_si128(x_t8, 8));	\
	x_g = _mm_unpacklo_epi8(_mm_srli_si128(x_t6, 8), x_t9);	\
	x_r = _mm_unpacklo_epi8(x_t8, _mm_srli_si128(x_t9, 8));	\
}

/* bgr: 24 bytes */
#define	SeparateBGR24(x_bgr0, x_bgr1, x_b, x_g,  x_r)	\
{	\
	x_bgr2 = x_zero;	\
	\
	x_t1 = _mm_unpacklo_epi8(x_bgr0, _mm_srli_si128(x_bgr1, 8));	\
	x_t2 = _mm_unpacklo_epi8(_mm_srli_si128(x_bgr0, 8), x_bgr2);	\
	x_t3 = _mm_unpacklo_epi8(x_bgr1, _mm_srli_si128(x_bgr2, 8));	\
	x_t4 = _mm_unpacklo_epi8(x_t1, _mm_srli_si128(x_t2, 8));	\
	x_t5 = _mm_unpacklo_epi8(_mm_srli_si128(x_t1, 8), x_t3);	\
	\
	x_t6 = _mm_unpacklo_epi8(x_t4, _mm_srli_si128(x_t5, 8));	\
	x_t7 = _mm_unpacklo_epi8(x_t2, _mm_srli_si128(x_t3, 8));	\
	x_t8 = _mm_unpacklo_epi8(_mm_srli_si128(x_t4, 8), x_t7);	\
	x_t9 = _mm_unpacklo_epi8(x_t5, _mm_srli_si128(x_t7, 8));	\
	\
	x_b = _mm_unpacklo_epi8(x_t6, _mm_srli_si128(x_t8, 8));	\
	x_g = _mm_unpacklo_epi8(_mm_srli_si128(x_t6, 8), x_t9);	\
	x_r = _mm_unpacklo_epi8(x_t8, _mm_srli_si128(x_t9, 8));	\
}

/* bgr: 12 bytes */
#define	SeparateBGR12(x_bgr0, x_b, x_g,  x_r)	\
{	\
	x_bgr1 = x_bgr2 = x_zero;	\
	\
	x_t1 = _mm_unpacklo_epi8(x_bgr0, _mm_srli_si128(x_bgr1, 8));	\
	x_t2 = _mm_unpacklo_epi8(_mm_srli_si128(x_bgr0, 8), x_bgr2);	\
	x_t3 = _mm_unpacklo_epi8(x_bgr1, _mm_srli_si128(x_bgr2, 8));	\
	x_t4 = _mm_unpacklo_epi8(x_t1, _mm_srli_si128(x_t2, 8));	\
	x_t5 = _mm_unpacklo_epi8(_mm_srli_si128(x_t1, 8), x_t3);	\
	\
	x_t6 = _mm_unpacklo_epi8(x_t4, _mm_srli_si128(x_t5, 8));	\
	x_t7 = _mm_unpacklo_epi8(x_t2, _mm_srli_si128(x_t3, 8));	\
	x_t8 = _mm_unpacklo_epi8(_mm_srli_si128(x_t4, 8), x_t7);	\
	x_t9 = _mm_unpacklo_epi8(x_t5, _mm_srli_si128(x_t7, 8));	\
	\
	x_b = _mm_unpacklo_epi8(x_t6, _mm_srli_si128(x_t8, 8));	\
	x_g = _mm_unpacklo_epi8(_mm_srli_si128(x_t6, 8), x_t9);	\
	x_r = _mm_unpacklo_epi8(x_t8, _mm_srli_si128(x_t9, 8));	\
}

/* *********************************************************** */
mlib_status
mlib_VideoColorBGR2JFIFYCC422_aligned(
	mlib_u8 *y,
	mlib_u8 *cb,
	mlib_u8 *cr,
	const mlib_u8 *bgr,
	mlib_s32 n);

mlib_status
mlib_VideoColorBGR2JFIFYCC422_naligned(
	mlib_u8 *y,
	mlib_u8 *cb,
	mlib_u8 *cr,
	const mlib_u8 *bgr,
	mlib_s32 n);

mlib_status
__mlib_VideoColorBGR2JFIFYCC422(
	mlib_u8 *y,
	mlib_u8 *cb,
	mlib_u8 *cr,
	const mlib_u8 *bgr,
	mlib_s32 n)
{
	if (bgr == NULL || y == NULL || cb == NULL || cr == NULL)
		return (MLIB_NULLPOINTER);

	if (n <= 0)
		return (MLIB_FAILURE);

	if (0 == (0xf & ((mlib_addr)bgr | (mlib_addr)y |
			(mlib_addr)cb | (mlib_addr)cr)))	{
		return mlib_VideoColorBGR2JFIFYCC422_aligned(
			y, cb, cr, bgr, n);
	} else {
		return mlib_VideoColorBGR2JFIFYCC422_naligned(
			y, cb, cr, bgr, n);
	}
}

mlib_status
mlib_VideoColorBGR2JFIFYCC422_aligned(
	mlib_u8 *y,
	mlib_u8 *cb,
	mlib_u8 *cr,
	const mlib_u8 *bgr,
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

	__m128i x_t1, x_t2, x_t3, x_t4, x_t5, x_t6, x_t7, x_t8, x_t9;
	__m128i x_temp;
	mlib_s32 iTemp, ir1, ir2, ig1, ig2, ib1, ib2, i;
	mlib_s32 iy1, iy2, icb1, icb2, icr1, icr2, size;

	__m128i x_bgr0, x_bgr1, x_bgr2, x_r, x_g, x_b, x_y, x_cb, x_cr;
	__m128i x_r1, x_r2, x_g1, x_g2, x_b1, x_b2;
	__m128i x_y1, x_y2, x_cb1, x_cb2, x_cr1, x_cr2;
	__m128i *pbgr, *py, *pcb, *pcr;

	pbgr = (__m128i*)bgr;
	py = (__m128i*)y;
	pcb = (__m128i*)cb;
	pcr = (__m128i*)cr;
	size = 3*n;
	i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i <= size - 48; i += 48)	{
		x_bgr0 = _mm_load_si128(pbgr++);
		x_bgr1 = _mm_load_si128(pbgr++);
		x_bgr2 = _mm_load_si128(pbgr++);

		SeparateBGR48(x_bgr0, x_bgr1, x_bgr2, x_b, x_g, x_r);
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
		x_cb1 = _mm_and_si128(x_cb1, x_mask);
		x_cb2 = _mm_and_si128(x_cb2, x_mask);
		x_cb1 = _mm_add_epi16(x_cb1, x_off1);
		x_cb = _mm_avg_epu16(x_cb1, x_cb2);
		x_cb = _mm_packus_epi16(x_cb, x_cb);
		x_cr1 = _mm_packus_epi16(x_cr1, x_cr2);
		x_cr2 = _mm_srli_si128(x_cr1, 1);
		x_cr1 = _mm_and_si128(x_cr1, x_mask);
		x_cr2 = _mm_and_si128(x_cr2, x_mask);
		x_cr1 = _mm_add_epi16(x_cr1, x_off1);
		x_cr = _mm_avg_epu16(x_cr1, x_cr2);
		x_cr = _mm_packus_epi16(x_cr, x_cr);

		/* save */
		_mm_store_si128(py, x_y);
		_mm_storel_epi64(pcb, x_cb);
		_mm_storel_epi64(pcr, x_cr);

		py++;
		pcb = (__m128i*)(((__m64*)pcb) + 1);
		pcr = (__m128i*)(((__m64*)pcr) + 1);
	}

	/* r/g/b: 8 bytes */
	if (i <= size - 24) {
		x_bgr0 = _mm_load_si128(pbgr);
		x_bgr1 = _mm_loadl_epi64(pbgr+1);
		pbgr = (__m128i*) (((__m64*)pbgr) + 3);

		SeparateBGR24(x_bgr0, x_bgr1, x_b, x_g, x_r);
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
		x_y = _mm_packus_epi16(x_y1, x_y2);
		x_cb1 = _mm_packus_epi16(x_cb1, x_cb2);
		x_cb2 = _mm_srli_si128(x_cb1, 1);
		x_cb1 = _mm_and_si128(x_cb1, x_mask);
		x_cb2 = _mm_and_si128(x_cb2, x_mask);
		x_cb1 = _mm_add_epi16(x_cb1, x_off1);
		x_cb = _mm_avg_epu16(x_cb1, x_cb2);
		x_cb = _mm_packus_epi16(x_cb, x_cb);
		x_cr1 = _mm_packus_epi16(x_cr1, x_cr2);
		x_cr2 = _mm_srli_si128(x_cr1, 1);
		x_cr1 = _mm_and_si128(x_cr1, x_mask);
		x_cr2 = _mm_and_si128(x_cr2, x_mask);
		x_cr1 = _mm_add_epi16(x_cr1, x_off1);
		x_cr = _mm_avg_epu16(x_cr1, x_cr2);
		x_cr = _mm_packus_epi16(x_cr, x_cr);

		icb1 = _mm_cvtsi128_si32(x_cb);
		icr1 = _mm_cvtsi128_si32(x_cr);

		/* save */
		_mm_storel_epi64(((__m128i *)py), x_y);
		*((mlib_s32*)pcb) = icb1;
		*((mlib_s32*)pcr) = icr1;

		py = (__m128i*) (((__m64*)py) + 1);
		pcb = (__m128i*) (((mlib_s32*)pcb) + 1);
		pcr = (__m128i*) (((mlib_s32*)pcr) + 1);

		i += 24;
	}

	/* r/g/b: 4 bytes */
	if (i <= size - 12) {
		x_bgr0 = _mm_loadl_epi64(pbgr);
		x_temp = _mm_cvtsi32_si128(((mlib_s32*)pbgr)[2]);
		x_temp = _mm_slli_si128(x_temp, 8);
		x_bgr0 = _mm_or_si128(x_temp, x_bgr0);
		pbgr = (__m128i*) (((mlib_s32*)pbgr) + 3);

		SeparateBGR12(x_bgr0, x_b, x_g, x_r);
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
		x_y = _mm_packus_epi16(x_y1, x_y2);
		x_cb1 = _mm_packus_epi16(x_cb1, x_cb2);
		x_cb2 = _mm_srli_si128(x_cb1, 1);
		x_cb1 = _mm_and_si128(x_cb1, x_mask);
		x_cb2 = _mm_and_si128(x_cb2, x_mask);
		x_cb1 = _mm_add_epi16(x_cb1, x_off1);
		x_cb = _mm_avg_epu16(x_cb1, x_cb2);
		x_cb = _mm_packus_epi16(x_cb, x_cb);
		x_cr1 = _mm_packus_epi16(x_cr1, x_cr2);
		x_cr2 = _mm_srli_si128(x_cr1, 1);
		x_cr1 = _mm_and_si128(x_cr1, x_mask);
		x_cr2 = _mm_and_si128(x_cr2, x_mask);
		x_cr1 = _mm_add_epi16(x_cr1, x_off1);
		x_cr = _mm_avg_epu16(x_cr1, x_cr2);
		x_cr = _mm_packus_epi16(x_cr, x_cr);

		/* save */
		*((mlib_s32 *)py) = *((mlib_s32 *)&x_y);
		*((mlib_s16 *)pcb) = *((mlib_s16 *)&x_cb);
		*((mlib_s16 *)pcr) = *((mlib_s16 *)&x_cr);

		py = (__m128i*) (((mlib_s32*)py) + 1);
		pcb = (__m128i*) (((mlib_s16*)pcb) + 1);
		pcr = (__m128i*) (((mlib_s16*)pcr) + 1);

		i += 12;
	}

	/* r/g/b: ==2 bytes */
	if (i <= size - 6) {
		/* 1st r/g/b */
		iTemp = *((mlib_s16 *)pbgr);
		ir1 = ((mlib_u8 *)pbgr)[2];
		pbgr = (__m128i*) (((mlib_u8 *)pbgr) + 3);

		ib1 = iTemp & 0xff;
		ig1 = (iTemp >> 8) & 0xff;

		iy1 = ((c11*ir1 + c12*ig1 + c13*ib1) >> 13);
		icb1 = ((-c21*ir1 - c22*ig1 + c23*ib1) >> 13) + coff;
		icr1 = ((c31*ir1 - c32*ig1 - c33*ib1) >> 13) + coff;

		/* 2nd r/g/b */
		iTemp = *((mlib_s16 *)pbgr);
		ir2 = ((mlib_u8 *)pbgr)[2];
		pbgr = (__m128i*) (((mlib_u8 *)pbgr) + 3);

		ib2 = iTemp & 0xff;
		ig2 = (iTemp >> 8) & 0xff;

		iy2 = ((c11*ir2 + c12*ig2 + c13*ib2) >> 13);
		icb2 = ((-c21*ir2 - c22*ig2 + c23*ib2) >> 13) + coff;
		icr2 = ((c31*ir2 - c32*ig2 - c33*ib2) >> 13) + coff;

		/* save */
		CLAMP_U8(iy1, *((mlib_u8*)py));
		CLAMP_U8(iy2, *(((mlib_u8*)py) + 1));
		CLAMP_U8((icb1+icb2)/2, *((mlib_u8*)pcb));
		CLAMP_U8((icr1+icr2)/2, *((mlib_u8*)pcr));

		py = (__m128i*) (((mlib_u8*)py) + 2);
		pcb = (__m128i*) (((mlib_u8*)pcb) + 1);
		pcr = (__m128i*) (((mlib_u8*)pcr) + 1);
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoColorBGR2JFIFYCC422_naligned(
	mlib_u8 *y,
	mlib_u8 *cb,
	mlib_u8 *cr,
	const mlib_u8 *bgr,
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

	__m128i x_t1, x_t2, x_t3, x_t4, x_t5, x_t6, x_t7, x_t8, x_t9;
	__m128i x_temp;
	mlib_s32 iTemp, ir1, ir2, ig1, ig2, ib1, ib2, i;
	mlib_s32 iy1, iy2, icb1, icb2, icr1, icr2, size;

	__m128i x_bgr0, x_bgr1, x_bgr2, x_r, x_g, x_b, x_y, x_cb, x_cr;
	__m128i x_r1, x_r2, x_g1, x_g2, x_b1, x_b2;
	__m128i x_y1, x_y2, x_cb1, x_cb2, x_cr1, x_cr2;
	__m128i *pbgr, *py, *pcb, *pcr;

	pbgr = (__m128i*)bgr;
	py = (__m128i*)y;
	pcb = (__m128i*)cb;
	pcr = (__m128i*)cr;
	size = 3*n;
	i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i <= size - 48; i += 48)	{
		x_bgr0 = _mm_loadu_si128(pbgr++);
		x_bgr1 = _mm_loadu_si128(pbgr++);
		x_bgr2 = _mm_loadu_si128(pbgr++);

		SeparateBGR48(x_bgr0, x_bgr1, x_bgr2, x_b, x_g, x_r);
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
		x_cb1 = _mm_and_si128(x_cb1, x_mask);
		x_cb2 = _mm_and_si128(x_cb2, x_mask);
		x_cb1 = _mm_add_epi16(x_cb1, x_off1);
		x_cb = _mm_avg_epu16(x_cb1, x_cb2);
		x_cb = _mm_packus_epi16(x_cb, x_cb);
		x_cr1 = _mm_packus_epi16(x_cr1, x_cr2);
		x_cr2 = _mm_srli_si128(x_cr1, 1);
		x_cr1 = _mm_and_si128(x_cr1, x_mask);
		x_cr2 = _mm_and_si128(x_cr2, x_mask);
		x_cr1 = _mm_add_epi16(x_cr1, x_off1);
		x_cr = _mm_avg_epu16(x_cr1, x_cr2);
		x_cr = _mm_packus_epi16(x_cr, x_cr);

		/* save */
		_mm_storeu_si128(py, x_y);
		_mm_storel_epi64(pcb, x_cb);
		_mm_storel_epi64(pcr, x_cr);

		py++;
		pcb = (__m128i*)(((__m64*)pcb) + 1);
		pcr = (__m128i*)(((__m64*)pcr) + 1);
	}

	/* r/g/b: 8 bytes */
	if (i <= size - 24) {
		x_bgr0 = _mm_loadu_si128(pbgr);
		x_bgr1 = _mm_loadl_epi64(pbgr+1);
		pbgr = (__m128i*) (((__m64*)pbgr) + 3);

		SeparateBGR24(x_bgr0, x_bgr1, x_b, x_g, x_r);
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
		x_y = _mm_packus_epi16(x_y1, x_y2);
		x_cb1 = _mm_packus_epi16(x_cb1, x_cb2);
		x_cb2 = _mm_srli_si128(x_cb1, 1);
		x_cb1 = _mm_and_si128(x_cb1, x_mask);
		x_cb2 = _mm_and_si128(x_cb2, x_mask);
		x_cb1 = _mm_add_epi16(x_cb1, x_off1);
		x_cb = _mm_avg_epu16(x_cb1, x_cb2);
		x_cb = _mm_packus_epi16(x_cb, x_cb);
		x_cr1 = _mm_packus_epi16(x_cr1, x_cr2);
		x_cr2 = _mm_srli_si128(x_cr1, 1);
		x_cr1 = _mm_and_si128(x_cr1, x_mask);
		x_cr2 = _mm_and_si128(x_cr2, x_mask);
		x_cr1 = _mm_add_epi16(x_cr1, x_off1);
		x_cr = _mm_avg_epu16(x_cr1, x_cr2);
		x_cr = _mm_packus_epi16(x_cr, x_cr);

		icb1 = _mm_cvtsi128_si32(x_cb);
		icr1 = _mm_cvtsi128_si32(x_cr);

		/* save */
		_mm_storel_epi64(((__m128i *)py), x_y);
		*((mlib_s32*)pcb) = icb1;
		*((mlib_s32*)pcr) = icr1;

		py = (__m128i*) (((__m64*)py) + 1);
		pcb = (__m128i*) (((mlib_s32*)pcb) + 1);
		pcr = (__m128i*) (((mlib_s32*)pcr) + 1);

		i += 24;
	}

	/* r/g/b: 4 bytes */
	if (i <= size - 12) {
		x_bgr0 = _mm_loadl_epi64(pbgr);
		x_temp = _mm_cvtsi32_si128(((mlib_s32*)pbgr)[2]);
		x_temp = _mm_slli_si128(x_temp, 8);
		x_bgr0 = _mm_or_si128(x_temp, x_bgr0);
		pbgr = (__m128i*) (((mlib_s32*)pbgr) + 3);

		SeparateBGR12(x_bgr0, x_b, x_g, x_r);
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
		x_y = _mm_packus_epi16(x_y1, x_y2);
		x_cb1 = _mm_packus_epi16(x_cb1, x_cb2);
		x_cb2 = _mm_srli_si128(x_cb1, 1);
		x_cb1 = _mm_and_si128(x_cb1, x_mask);
		x_cb2 = _mm_and_si128(x_cb2, x_mask);
		x_cb1 = _mm_add_epi16(x_cb1, x_off1);
		x_cb = _mm_avg_epu16(x_cb1, x_cb2);
		x_cb = _mm_packus_epi16(x_cb, x_cb);
		x_cr1 = _mm_packus_epi16(x_cr1, x_cr2);
		x_cr2 = _mm_srli_si128(x_cr1, 1);
		x_cr1 = _mm_and_si128(x_cr1, x_mask);
		x_cr2 = _mm_and_si128(x_cr2, x_mask);
		x_cr1 = _mm_add_epi16(x_cr1, x_off1);
		x_cr = _mm_avg_epu16(x_cr1, x_cr2);
		x_cr = _mm_packus_epi16(x_cr, x_cr);

		/* save */
		*((mlib_s32 *)py) = *((mlib_s32 *)&x_y);
		*((mlib_s16 *)pcb) = *((mlib_s16 *)&x_cb);
		*((mlib_s16 *)pcr) = *((mlib_s16 *)&x_cr);

		py = (__m128i*) (((mlib_s32*)py) + 1);
		pcb = (__m128i*) (((mlib_s16*)pcb) + 1);
		pcr = (__m128i*) (((mlib_s16*)pcr) + 1);

		i += 12;
	}

	/* r/g/b: ==2 bytes */
	if (i <= size - 6) {
		/* 1st r/g/b */
		iTemp = *((mlib_s16 *)pbgr);
		ir1 = ((mlib_u8 *)pbgr)[2];
		pbgr = (__m128i*) (((mlib_u8 *)pbgr) + 3);

		ib1 = iTemp & 0xff;
		ig1 = (iTemp >> 8) & 0xff;

		iy1 = ((c11*ir1 + c12*ig1 + c13*ib1) >> 13);
		icb1 = ((-c21*ir1 - c22*ig1 + c23*ib1) >> 13) + coff;
		icr1 = ((c31*ir1 - c32*ig1 - c33*ib1) >> 13) + coff;

		/* 2nd r/g/b */
		iTemp = *((mlib_s16 *)pbgr);
		ir2 = ((mlib_u8 *)pbgr)[2];
		pbgr = (__m128i*) (((mlib_u8 *)pbgr) + 3);

		ib2 = iTemp & 0xff;
		ig2 = (iTemp >> 8) & 0xff;

		iy2 = ((c11*ir2 + c12*ig2 + c13*ib2) >> 13);
		icb2 = ((-c21*ir2 - c22*ig2 + c23*ib2) >> 13) + coff;
		icr2 = ((c31*ir2 - c32*ig2 - c33*ib2) >> 13) + coff;

		/* save */
		CLAMP_U8(iy1, *((mlib_u8*)py));
		CLAMP_U8(iy2, *(((mlib_u8*)py) + 1));
		CLAMP_U8((icb1+icb2)/2, *((mlib_u8*)pcb));
		CLAMP_U8((icr1+icr2)/2, *((mlib_u8*)pcr));

		py = (__m128i*) (((mlib_u8*)py) + 2);
		pcb = (__m128i*) (((mlib_u8*)pcb) + 1);
		pcr = (__m128i*) (((mlib_u8*)pcr) + 1);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
