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

#pragma ident   "@(#)mlib_s_ImagePolynomialWarp_BC.c	9.2	07/11/05 SMI"

/* *********************************************************** */

#define	MMX_USE_MODIF_8x16

#define	MMX_USE_MEM_ZERO

#include <mlib_image.h>
#include <mlib_ImagePolynomialWarp.h>
#ifdef  __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	DECLAREVAR                                              \
	mlib_s32 *xs = pws->src_x;                              \
	mlib_s32 *ys = pws->src_y;                              \
	mlib_s32 *xd = pws->dst_x;                              \
	mlib_u16 *xcoeff = pws->dsrc_x;                         \
	mlib_u16 *ycoeff = pws->dsrc_y;                         \
	MLIB_TYPE *srcPixelPtr;                                 \
	mlib_s32 i;                                             \
	mlib_s32 srcYStride = lineAddr[1] - lineAddr[0];        \
	mlib_s32 filterpos

/* *********************************************************** */

#undef MLIB_TYPE
#define	MLIB_TYPE	mlib_u8
#define	FILTER_SHIFT	3

#define	FILTER_MASK	(((1 << 9) - 1) << 3)

/* *********************************************************** */

#undef  MMX_SCALE
#define	MMX_SCALE	1

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_U8_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	DECLAREVAR;
	mlib_u8 *filter_table_x = (mlib_u8 *)pws->filter_table;
	mlib_u8 *filter_table_y = (mlib_u8 *)pws->filter_table_vis;
	__m128i _m_mzero = _mm_setzero_si128();
	__m128i xf0;
	__m128i yf0, yf1, yf2, yf3;
	__m128i r0, r1, r2, r3, d0;
	__m128i *fptr;
	mlib_s32 s0, s1, s2, s3;
	mlib_s32 ri;

	for (i = 0; i < n; i++) {
		filterpos = (xcoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		xf0 = _mm_loadl_epi64((__m128i *)(filter_table_x + filterpos));
		filterpos = (ycoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m128i *) (filter_table_y + 4 * filterpos);
		yf0 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 0));
		yf1 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 1));
		yf2 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 2));
		yf3 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 3));
		srcPixelPtr = (MLIB_TYPE *) (lineAddr[ys[0] - 1]) + xs[0] - 1;
		s0 = *(mlib_s32 *)(srcPixelPtr);
		s1 = *(mlib_s32 *)(srcPixelPtr + srcYStride);
		s2 = *(mlib_s32 *)(srcPixelPtr + 2 * srcYStride);
		s3 = *(mlib_s32 *)(srcPixelPtr + 3 * srcYStride);

		r0 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(_mm_cvtsi32_si128(s0),
				_m_mzero), 7), yf0);
		r1 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(_mm_cvtsi32_si128(s1),
				_m_mzero), 7), yf1);
		r2 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(_mm_cvtsi32_si128(s2),
				_m_mzero), 7), yf2);
		r3 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(_mm_cvtsi32_si128(s3),
				_m_mzero), 7), yf3);

		r0 = _mm_add_epi16(r0, r1);
		r2 = _mm_add_epi16(r2, r3);

		d0 = _mm_add_epi16(r0, r2);
		d0 = _mm_madd_epi16(d0, xf0);
		d0 = _mm_add_epi32(d0, _mm_srli_si128(d0, 4));

		ri = _mm_cvtsi128_si32(_mm_packus_epi16(
				_mm_srai_epi16(d0, 6 - MMX_SCALE), _m_mzero));
		dstData[xd[0]] = (ri >> 8);
		xs++;
		ys++;
		xd++;
		xcoeff++;
		ycoeff++;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_U8_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	DECLAREVAR;
	mlib_u8 *filter_table_x = (mlib_u8 *)pws->filter_table;
	mlib_u8 *filter_table_y = (mlib_u8 *)pws->filter_table_vis;

	__m128i _m_mzero = _mm_setzero_si128();
	__m128i xf0;
	__m128i yf0, yf1, yf2, yf3;
	__m128i r0, r1, r2, r3, d0;
	__m128i s0, s1, s2, s3;
	__m128i *fptr;

	mlib_s32 ri;
	mlib_u16 *dstData1 = (mlib_u16 *)dstData;

	for (i = 0; i < n; i++) {
		filterpos = (xcoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		xf0 = _mm_loadl_epi64((__m128i *) (filter_table_x + filterpos));
		xf0 = _mm_unpacklo_epi16(xf0, xf0);
		filterpos = (ycoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m128i *) (filter_table_y + 4 * filterpos);
		yf0 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 0));
		yf0 = _mm_unpacklo_epi64(yf0, yf0);
		yf1 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 1));
		yf1 = _mm_unpacklo_epi64(yf1, yf1);
		yf2 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 2));
		yf2 = _mm_unpacklo_epi64(yf2, yf2);
		yf3 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 3));
		yf3 = _mm_unpacklo_epi64(yf3, yf3);
		srcPixelPtr =
		    (MLIB_TYPE *) (lineAddr[ys[0] - 1]) + 2 * (xs[0] - 1);

		s0 = _mm_loadl_epi64((__m128i *)(srcPixelPtr));
		s1 = _mm_loadl_epi64((__m128i *)(srcPixelPtr + srcYStride));
		s2 = _mm_loadl_epi64((__m128i *)(srcPixelPtr + 2 * srcYStride));
		s3 = _mm_loadl_epi64((__m128i *)(srcPixelPtr + 3 * srcYStride));

		r0 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(s0,
				_m_mzero), 7), yf0);
		r1 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(s1,
				_m_mzero), 7), yf1);
		r2 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(s2,
				_m_mzero), 7), yf2);
		r3 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(s3,
				_m_mzero), 7), yf3);

		r0 = _mm_add_epi16(r0, r1);
		r2 = _mm_add_epi16(r2, r3);

		d0 = _mm_add_epi16(r0, r2);
		d0 = _mm_mulhi_epi16(d0, xf0);
		d0 = _mm_add_epi16(d0, _mm_srli_si128(d0, 8));
		d0 = _mm_add_epi16(d0, _mm_srli_si128(d0, 4));
		ri = _mm_cvtsi128_si32(_mm_packus_epi16(
				_mm_srai_epi16(d0, 6 - MMX_SCALE), _m_mzero));


		dstData1[xd[0]] = ri;
		xs++;
		ys++;
		xd++;
		xcoeff++;
		ycoeff++;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_U8_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	DECLAREVAR;
	mlib_u8 *filter_table_x = (mlib_u8 *)pws->filter_table;
	mlib_u8 *filter_table_y = (mlib_u8 *)pws->filter_table_vis;
	__m128i _m_mzero = _mm_setzero_si128();
	__m128i xf0, xf1, xf2, xf3;
	__m128i yf0, yf1, yf2, yf3;
	__m128i a0, a1, a2, a3, b0, b1, b2, b3;
	__m128i c0, c1, c2, c3, d0, d1, d2, d3;
	__m128i *fptr;
	mlib_s32 ri, xdst;

	for (i = 0; i < n; i++) {
		filterpos = (xcoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m128i *) (filter_table_x + 3 * filterpos);
		xf0 = _mm_loadu_si128(fptr);
		xf1 = _mm_loadu_si128((__m128i *)((mlib_u8 *)fptr + 12));
		filterpos = (ycoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m128i *) (filter_table_y + 4 * filterpos);
		yf0 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 0));
		yf0 = _mm_unpacklo_epi64(yf0, yf0);
		yf1 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 1));
		yf1 = _mm_unpacklo_epi64(yf1, yf1);
		yf2 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 2));
		yf2 = _mm_unpacklo_epi64(yf2, yf2);
		yf3 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 3));
		yf3 = _mm_unpacklo_epi64(yf3, yf3);

		srcPixelPtr =
		    (MLIB_TYPE *) (lineAddr[ys[0] - 1]) + 3 * (xs[0] - 1);

		a0 = _mm_loadl_epi64((__m128i *)(srcPixelPtr));
		a1 = _mm_loadl_epi64((__m128i *)
			(srcPixelPtr + 6));
		b0 = _mm_loadl_epi64((__m128i *)
			(srcPixelPtr + srcYStride));
		b1 = _mm_loadl_epi64((__m128i *)
			(srcPixelPtr + srcYStride + 6));
		c0 = _mm_loadl_epi64((__m128i *)
			(srcPixelPtr + 2 * srcYStride));
		c1 = _mm_loadl_epi64((__m128i *)
			(srcPixelPtr + 2 * srcYStride + 6));
		d0 = _mm_loadl_epi64((__m128i *)
			(srcPixelPtr + 3 * srcYStride));
		d1 = _mm_loadl_epi64((__m128i *)
			(srcPixelPtr + 3 * srcYStride + 6));

		a0 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(a0,
				_m_mzero), 7), yf0);
		a1 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(a1,
				_m_mzero), 7), yf0);
		b0 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(b0,
				_m_mzero), 7), yf1);
		b1 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(b1,
				_m_mzero), 7), yf1);
		c0 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(c0,
				_m_mzero), 7), yf2);
		c1 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(c1,
				_m_mzero), 7), yf2);
		d0 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(d0,
				_m_mzero), 7), yf3);
		d1 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(d1,
				_m_mzero), 7), yf3);

		a0 = _mm_add_epi16(a0, b0);
		a1 = _mm_add_epi16(a1, b1);
		c0 = _mm_add_epi16(c0, d0);
		c1 = _mm_add_epi16(c1, d1);
		a0 = _mm_add_epi16(a0, c0);
		a1 = _mm_add_epi16(a1, c1);
		a0 = _mm_mulhi_epi16(a0, xf0);
		a1 = _mm_mulhi_epi16(a1, xf1);
		a0 = _mm_add_epi16(a0, a1);
		a0 = _mm_add_epi16(a0, _mm_srli_si128(a0, 6));
		ri = _mm_cvtsi128_si32(_mm_packus_epi16(
				_mm_srai_epi16(a0, 6 - MMX_SCALE), _m_mzero));

		xdst = xd[i] * 3;
		dstData[xdst] = ri;
		dstData[xdst + 1] = (ri >> 8);
		dstData[xdst + 2] = (ri >> 16);
		xs++;
		ys++;
		xcoeff++;
		ycoeff++;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_U8_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	DECLAREVAR;
	mlib_u8 *filter_table_y = (mlib_u8 *)pws->filter_table_vis;
	__m128i _m_mzero = _mm_setzero_si128();
	__m128i xf0, xf1, xf2, xf3;
	__m128i yf0, yf1, yf2, yf3;
	__m128i a0, a1, a2, a3, b0, b1, b2, b3;
	__m128i c0, c1, c2, c3, d0, d1, d2, d3;
	__m128i *fptr;

	mlib_s32 ri;
	mlib_s32 *dstData1 = (mlib_s32 *)dstData;

	for (i = 0; i < n; i++) {
		filterpos = (xcoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m128i *) (filter_table_y + 4 * filterpos);
		xf0 = _mm_loadu_si128(fptr);
		xf1 = _mm_loadu_si128(fptr + 1);
		filterpos = (ycoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m128i *) (filter_table_y + 4 * filterpos);
		yf0 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 0));
		yf0 = _mm_unpacklo_epi64(yf0, yf0);
		yf1 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 1));
		yf1 = _mm_unpacklo_epi64(yf1, yf1);
		yf2 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 2));
		yf2 = _mm_unpacklo_epi64(yf2, yf2);
		yf3 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 3));
		yf3 = _mm_unpacklo_epi64(yf3, yf3);

		srcPixelPtr =
		    (MLIB_TYPE *) (lineAddr[ys[0] - 1]) + 4 * (xs[0] - 1);

		a0 = _mm_loadl_epi64((__m128i *)(srcPixelPtr));
		a1 = _mm_loadl_epi64((__m128i *)
			(srcPixelPtr + 8));
		b0 = _mm_loadl_epi64((__m128i *)
			(srcPixelPtr + srcYStride));
		b1 = _mm_loadl_epi64((__m128i *)
			(srcPixelPtr + srcYStride + 8));
		c0 = _mm_loadl_epi64((__m128i *)
			(srcPixelPtr + 2 * srcYStride));
		c1 = _mm_loadl_epi64((__m128i *)
			(srcPixelPtr + 2 * srcYStride + 8));
		d0 = _mm_loadl_epi64((__m128i *)
			(srcPixelPtr + 3 * srcYStride));
		d1 = _mm_loadl_epi64((__m128i *)
			(srcPixelPtr + 3 * srcYStride + 8));

		a0 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(a0,
				_m_mzero), 7), yf0);
		a1 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(a1,
				_m_mzero), 7), yf0);
		b0 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(b0,
				_m_mzero), 7), yf1);
		b1 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(b1,
				_m_mzero), 7), yf1);
		c0 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(c0,
				_m_mzero), 7), yf2);
		c1 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(c1,
				_m_mzero), 7), yf2);
		d0 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(d0,
				_m_mzero), 7), yf3);
		d1 = _mm_mulhi_epi16(_mm_slli_epi16(
				_mm_unpacklo_epi8(d1,
				_m_mzero), 7), yf3);

		a0 = _mm_add_epi16(a0, b0);
		a1 = _mm_add_epi16(a1, b1);
		c0 = _mm_add_epi16(c0, d0);
		c1 = _mm_add_epi16(c1, d1);
		a0 = _mm_add_epi16(a0, c0);
		a1 = _mm_add_epi16(a1, c1);
		a0 = _mm_mulhi_epi16(a0, xf0);
		a1 = _mm_mulhi_epi16(a1, xf1);
		a0 = _mm_add_epi16(a0, a1);
		a0 = _mm_add_epi16(a0, _mm_srli_si128(a0, 8));
		ri = _mm_cvtsi128_si32(_mm_packus_epi16(
				_mm_srai_epi16(a0, 6 - MMX_SCALE), _m_mzero));
		dstData1[xd[0]] = ri;
		xs++;
		ys++;
		xd++;
		xcoeff++;
		ycoeff++;
	}
}

/* *********************************************************** */

#undef  MLIB_TYPE
#define	MLIB_TYPE	mlib_s16

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_S16_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	DECLAREVAR;
	mlib_u8 *filter_table_x = (mlib_u8 *)pws->filter_table;
	mlib_u8 *filter_table_y = (mlib_u8 *)pws->filter_table_vis;
	__m128i xf0;
	__m128i yf0, yf1, yf2, yf3;
	__m128i r0, r1, r2, r3, d0;
	__m128i s0, s1, s2, s3;
	__m128i *fptr;
	mlib_u16 *dstData1 = (mlib_u16 *)dstData;

	for (i = 0; i < n; i++) {
		filterpos = (xcoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		xf0 = _mm_loadl_epi64((__m128i *)(filter_table_x + filterpos));
		filterpos = (ycoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m128i *) (filter_table_y + 4 * filterpos);
		yf0 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 0));
		yf1 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 1));
		yf2 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 2));
		yf3 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 3));
		srcPixelPtr = (MLIB_TYPE *) (lineAddr[ys[0] - 1]) + (xs[0] - 1);

		s0 = _mm_loadl_epi64((__m128i *)(srcPixelPtr));
		s1 = _mm_loadl_epi64((__m128i *)
			((mlib_u8 *)srcPixelPtr + srcYStride));
		s2 = _mm_loadl_epi64((__m128i *)
			((mlib_u8 *)srcPixelPtr + 2 * srcYStride));
		s3 = _mm_loadl_epi64((__m128i *)
			((mlib_u8 *)srcPixelPtr + 3 * srcYStride));

		r0 = _mm_mulhi_epi16(s0, yf0);
		r1 = _mm_mulhi_epi16(s1, yf1);
		r2 = _mm_mulhi_epi16(s2, yf2);
		r3 = _mm_mulhi_epi16(s3, yf3);

		r0 = _mm_add_epi16(r0, r1);
		r2 = _mm_add_epi16(r2, r3);

		d0 = _mm_add_epi16(r0, r2);
		d0 = _mm_madd_epi16(d0, xf0);
		d0 = _mm_add_epi32(d0, _mm_srli_si128(d0, 4));
		d0 = _mm_srli_epi32(d0, 15);

		dstData1[xd[0]] = _mm_cvtsi128_si32(_mm_adds_epi16(d0, d0));
		xs++;
		ys++;
		xd++;
		xcoeff++;
		ycoeff++;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_S16_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	DECLAREVAR;
	mlib_u8 *filter_table_x = (mlib_u8 *)pws->filter_table;
	mlib_u8 *filter_table_y = (mlib_u8 *)pws->filter_table_vis;

	__m128i xf0, xf1, xf2;
	__m128i yf0, yf1, yf2, yf3;
	__m128i r0, r1, r2, r3, d0;
	__m128i s0, s1, s2, s3;
	__m128i a0, a1, a2, a3, b0, b1, b2, b3;
	__m128i *fptr;

	mlib_s32 *dstData1 = (mlib_s32 *)dstData;

	for (i = 0; i < n; i++) {
		filterpos = (xcoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		xf0 = _mm_loadl_epi64((__m128i *)(filter_table_x + filterpos));
		xf0 = _mm_unpacklo_epi16(xf0, xf0);
		filterpos = (ycoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m128i *) (filter_table_y + 4 * filterpos);
		yf0 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 0));
		yf0 = _mm_unpacklo_epi64(yf0, yf0);
		yf1 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 1));
		yf1 = _mm_unpacklo_epi64(yf1, yf1);
		yf2 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 2));
		yf2 = _mm_unpacklo_epi64(yf2, yf2);
		yf3 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 3));
		yf3 = _mm_unpacklo_epi64(yf3, yf3);
		srcPixelPtr =
		    (MLIB_TYPE *) (lineAddr[ys[0] - 1]) + 2 * (xs[0] - 1);

		s0 = _mm_loadu_si128((__m128i *)(srcPixelPtr));
		s1 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + srcYStride));
		s2 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + 2 * srcYStride));
		s3 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + 3 * srcYStride));

		r0 = _mm_mulhi_epi16(s0, yf0);
		r1 = _mm_mulhi_epi16(s1, yf1);
		r2 = _mm_mulhi_epi16(s2, yf2);
		r3 = _mm_mulhi_epi16(s3, yf3);

		r0 = _mm_add_epi16(r0, r1);
		r2 = _mm_add_epi16(r2, r3);

		d0 = _mm_add_epi16(r0, r2);
		d0 = _mm_mulhi_epi16(d0, xf0);
		d0 = _mm_add_epi16(d0, _mm_srli_si128(d0, 8));
		d0 = _mm_add_epi16(d0, _mm_srli_si128(d0, 4));
		d0 = _mm_add_epi16(d0, d0);
		d0 = _mm_adds_epi16(d0, d0);
		dstData1[xd[0]] = *(mlib_s32 *)&d0;
		xs++;
		ys++;
		xd++;
		xcoeff++;
		ycoeff++;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_S16_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	DECLAREVAR;
	mlib_u8 *filter_table_x = (mlib_u8 *)pws->filter_table;
	mlib_u8 *filter_table_y = (mlib_u8 *)pws->filter_table_vis;

	__m128i xf0, xf1, xf2, xf3;
	__m128i yf0, yf1, yf2, yf3;
	__m128i a0, a1, a2, a3, b0, b1, b2, b3;
	__m128i c0, c1, c2, c3, d0, d1, d2, d3;
	__m128i *fptr;
	mlib_u16 *dstData1 = (mlib_u16 *)dstData;
	mlib_s32 rr, xdst;

	for (i = 0; i < n; i++) {
		filterpos = (xcoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m128i *) (filter_table_x + 3 * filterpos);
		xf0 = _mm_loadu_si128(fptr);
		xf1 = _mm_loadu_si128((__m128i *)((mlib_u8 *)fptr + 12));
		filterpos = (ycoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m128i *) (filter_table_y + 4 * filterpos);
		yf0 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 0));
		yf0 = _mm_unpacklo_epi64(yf0, yf0);
		yf1 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 1));
		yf1 = _mm_unpacklo_epi64(yf1, yf1);
		yf2 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 2));
		yf2 = _mm_unpacklo_epi64(yf2, yf2);
		yf3 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 3));
		yf3 = _mm_unpacklo_epi64(yf3, yf3);
		srcPixelPtr =
		    (MLIB_TYPE *) (lineAddr[ys[0] - 1]) + 3 * (xs[0] - 1);

		a0 = _mm_loadu_si128((__m128i *)(srcPixelPtr));
		a1 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + 12));
		b0 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + srcYStride));
		b1 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + srcYStride + 12));
		c0 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + 2 * srcYStride));
		c1 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + 2 * srcYStride + 12));
		d0 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + 3 * srcYStride));
		d1 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + 3 * srcYStride + 12));

		a0 = _mm_mulhi_epi16(a0, yf0);
		a1 = _mm_mulhi_epi16(a1, yf0);
		b0 = _mm_mulhi_epi16(b0, yf1);
		b1 = _mm_mulhi_epi16(b1, yf1);
		c0 = _mm_mulhi_epi16(c0, yf2);
		c1 = _mm_mulhi_epi16(c1, yf2);
		d0 = _mm_mulhi_epi16(d0, yf3);
		d1 = _mm_mulhi_epi16(d1, yf3);

		a0 = _mm_add_epi16(a0, b0);
		a1 = _mm_add_epi16(a1, b1);
		c0 = _mm_add_epi16(c0, d0);
		c1 = _mm_add_epi16(c1, d1);
		a0 = _mm_add_epi16(a0, c0);
		a1 = _mm_add_epi16(a1, c1);
		a0 = _mm_mulhi_epi16(a0, xf0);
		a1 = _mm_mulhi_epi16(a1, xf1);
		a0 = _mm_add_epi16(a0, a1);
		a0 = _mm_add_epi16(a0, _mm_srli_si128(a0, 6));
		a0 = _mm_add_epi16(a0, a0);
		a0 = _mm_adds_epi16(a0, a0);

		rr = _mm_cvtsi128_si32(a0);

		xdst = xd[i] * 3;
		dstData1[xdst] = rr;
		dstData1[xdst + 1] = (rr >> 16);
		dstData1[xdst + 2] = _mm_cvtsi128_si32(_mm_srli_si128(a0, 4));
		xs++;
		ys++;
		xcoeff++;
		ycoeff++;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_S16_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	DECLAREVAR;
	mlib_u8 *filter_table_y = (mlib_u8 *)pws->filter_table_vis;
	__m128i xf0, xf1, xf2, xf3;
	__m128i yf0, yf1, yf2, yf3;
	__m128i a0, a1, a2, a3, b0, b1, b2, b3;
	__m128i c0, c1, c2, c3, d0, d1, d2, d3;
	__m128i *fptr;
	mlib_d64 *dstData1 = (mlib_d64 *) dstData;


	for (i = 0; i < n; i++) {
		filterpos = (xcoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m128i *) (filter_table_y + 4 * filterpos);
		xf0 = _mm_loadu_si128(fptr);
		xf1 = _mm_loadu_si128(fptr + 1);

		filterpos = (ycoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m128i *) (filter_table_y + 4 * filterpos);
		yf0 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 0));
		yf0 = _mm_unpacklo_epi64(yf0, yf0);
		yf1 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 1));
		yf1 = _mm_unpacklo_epi64(yf1, yf1);
		yf2 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 2));
		yf2 = _mm_unpacklo_epi64(yf2, yf2);
		yf3 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 3));
		yf3 = _mm_unpacklo_epi64(yf3, yf3);

		srcPixelPtr =
		    (MLIB_TYPE *) (lineAddr[ys[0] - 1]) + 4 * (xs[0] - 1);

		a0 = _mm_loadu_si128((__m128i *)(srcPixelPtr));
		a1 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + 16));
		b0 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + srcYStride));
		b1 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + srcYStride + 16));
		c0 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + 2 * srcYStride));
		c1 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + 2 * srcYStride + 16));
		d0 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + 3 * srcYStride));
		d1 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + 3 * srcYStride + 16));

		a0 = _mm_mulhi_epi16(a0, yf0);
		a1 = _mm_mulhi_epi16(a1, yf0);
		b0 = _mm_mulhi_epi16(b0, yf1);
		b1 = _mm_mulhi_epi16(b1, yf1);
		c0 = _mm_mulhi_epi16(c0, yf2);
		c1 = _mm_mulhi_epi16(c1, yf2);
		d0 = _mm_mulhi_epi16(d0, yf3);
		d1 = _mm_mulhi_epi16(d1, yf3);

		a0 = _mm_add_epi16(a0, b0);
		a1 = _mm_add_epi16(a1, b1);
		c0 = _mm_add_epi16(c0, d0);
		c1 = _mm_add_epi16(c1, d1);
		a0 = _mm_add_epi16(a0, c0);
		a1 = _mm_add_epi16(a1, c1);
		a0 = _mm_mulhi_epi16(a0, xf0);
		a1 = _mm_mulhi_epi16(a1, xf1);
		a0 = _mm_add_epi16(a0, a1);
		a0 = _mm_add_epi16(a0, _mm_srli_si128(a0, 8));
		a0 = _mm_add_epi16(a0, a0);
		a0 = _mm_adds_epi16(a0, a0);

		dstData1[xd[0]] = *((mlib_d64 *)&a0);
		xs++;
		ys++;
		xd++;
		xcoeff++;
		ycoeff++;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_U16_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	DECLAREVAR;
	mlib_u8 *filter_table_x = (mlib_u8 *)pws->filter_table;
	mlib_u8 *filter_table_y = (mlib_u8 *)pws->filter_table_vis;
	__m128i xf0;
	__m128i yf0, yf1, yf2, yf3;
	__m128i r0, r1, r2, r3, d0;
	__m128i s0, s1, s2, s3;
	__m128i *fptr;
	__m128i mask_8000 = _mm_set1_epi32(0x80008000);
	mlib_u16 *dstData1 = (mlib_u16 *)dstData;

	for (i = 0; i < n; i++) {
		filterpos = (xcoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		xf0 = _mm_loadl_epi64((__m128i *)(filter_table_x + filterpos));
		filterpos = (ycoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m128i *) (filter_table_y + 4 * filterpos);
		yf0 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 0));
		yf1 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 1));
		yf2 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 2));
		yf3 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 3));
		srcPixelPtr = (MLIB_TYPE *) (lineAddr[ys[0] - 1]) + (xs[0] - 1);

		s0 = _mm_loadl_epi64((__m128i *)(srcPixelPtr));
		s1 = _mm_loadl_epi64((__m128i *)
			((mlib_u8 *)srcPixelPtr + srcYStride));
		s2 = _mm_loadl_epi64((__m128i *)
			((mlib_u8 *)srcPixelPtr + 2 * srcYStride));
		s3 = _mm_loadl_epi64((__m128i *)
			((mlib_u8 *)srcPixelPtr + 3 * srcYStride));

		s0 = _mm_xor_si128(s0, mask_8000);
		s1 = _mm_xor_si128(s1, mask_8000);
		s2 = _mm_xor_si128(s2, mask_8000);
		s3 = _mm_xor_si128(s3, mask_8000);

		r0 = _mm_mulhi_epi16(s0, yf0);
		r1 = _mm_mulhi_epi16(s1, yf1);
		r2 = _mm_mulhi_epi16(s2, yf2);
		r3 = _mm_mulhi_epi16(s3, yf3);

		r0 = _mm_add_epi16(r0, r1);
		r2 = _mm_add_epi16(r2, r3);

		d0 = _mm_add_epi16(r0, r2);
		d0 = _mm_madd_epi16(d0, xf0);
		d0 = _mm_add_epi32(d0, _mm_srli_si128(d0, 4));
		d0 = _mm_srli_epi32(d0, 15);

		dstData1[xd[0]] =
			_mm_cvtsi128_si32(
			_mm_adds_epi16(d0, d0)) ^ 0x8000;
		xs++;
		ys++;
		xd++;
		xcoeff++;
		ycoeff++;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_U16_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	DECLAREVAR;
	mlib_u8 *filter_table_x = (mlib_u8 *)pws->filter_table;
	mlib_u8 *filter_table_y = (mlib_u8 *)pws->filter_table_vis;

	__m128i xf0, xf1, xf2;
	__m128i yf0, yf1, yf2, yf3;
	__m128i r0, r1, r2, r3, d0;
	__m128i s0, s1, s2, s3;
	__m128i a0, a1, a2, a3, b0, b1, b2, b3;
	__m128i *fptr;
	__m128i mask_8000 = _mm_set1_epi32(0x80008000);

	mlib_s32 *dstData1 = (mlib_s32 *)dstData;

	for (i = 0; i < n; i++) {
		filterpos = (xcoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		xf0 = _mm_loadl_epi64((__m128i *)(filter_table_x + filterpos));
		xf0 = _mm_unpacklo_epi16(xf0, xf0);
		filterpos = (ycoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m128i *) (filter_table_y + 4 * filterpos);
		yf0 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 0));
		yf0 = _mm_unpacklo_epi64(yf0, yf0);
		yf1 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 1));
		yf1 = _mm_unpacklo_epi64(yf1, yf1);
		yf2 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 2));
		yf2 = _mm_unpacklo_epi64(yf2, yf2);
		yf3 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 3));
		yf3 = _mm_unpacklo_epi64(yf3, yf3);
		srcPixelPtr =
		    (MLIB_TYPE *) (lineAddr[ys[0] - 1]) + 2 * (xs[0] - 1);

		s0 = _mm_loadu_si128((__m128i *)(srcPixelPtr));
		s1 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + srcYStride));
		s2 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + 2 * srcYStride));
		s3 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + 3 * srcYStride));

		s0 = _mm_xor_si128(s0, mask_8000);
		s1 = _mm_xor_si128(s1, mask_8000);
		s2 = _mm_xor_si128(s2, mask_8000);
		s3 = _mm_xor_si128(s3, mask_8000);

		r0 = _mm_mulhi_epi16(s0, yf0);
		r1 = _mm_mulhi_epi16(s1, yf1);
		r2 = _mm_mulhi_epi16(s2, yf2);
		r3 = _mm_mulhi_epi16(s3, yf3);

		r0 = _mm_add_epi16(r0, r1);
		r2 = _mm_add_epi16(r2, r3);

		d0 = _mm_add_epi16(r0, r2);
		d0 = _mm_mulhi_epi16(d0, xf0);
		d0 = _mm_add_epi16(d0, _mm_srli_si128(d0, 8));
		d0 = _mm_add_epi16(d0, _mm_srli_si128(d0, 4));
		d0 = _mm_add_epi16(d0, d0);
		d0 = _mm_adds_epi16(d0, d0);
		dstData1[xd[0]] = *(mlib_s32 *)&d0 ^ 0x80008000;
		xs++;
		ys++;
		xd++;
		xcoeff++;
		ycoeff++;
	}
}
/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_U16_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	DECLAREVAR;
	mlib_u8 *filter_table_x = (mlib_u8 *)pws->filter_table;
	mlib_u8 *filter_table_y = (mlib_u8 *)pws->filter_table_vis;

	__m128i xf0, xf1, xf2, xf3;
	__m128i yf0, yf1, yf2, yf3;
	__m128i a0, a1, a2, a3, b0, b1, b2, b3;
	__m128i c0, c1, c2, c3, d0, d1, d2, d3;
	__m128i *fptr;
	__m128i mask_8000 = _mm_set1_epi32(0x80008000);

	mlib_u16 *dstData1 = (mlib_u16 *)dstData;
	mlib_s32 rr, xdst;

	for (i = 0; i < n; i++) {
		filterpos = (xcoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m128i *) (filter_table_x + 3 * filterpos);
		xf0 = _mm_loadu_si128(fptr);
		xf1 = _mm_loadu_si128((__m128i *)((mlib_u8 *)fptr + 12));
		filterpos = (ycoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m128i *) (filter_table_y + 4 * filterpos);
		yf0 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 0));
		yf0 = _mm_unpacklo_epi64(yf0, yf0);
		yf1 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 1));
		yf1 = _mm_unpacklo_epi64(yf1, yf1);
		yf2 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 2));
		yf2 = _mm_unpacklo_epi64(yf2, yf2);
		yf3 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 3));
		yf3 = _mm_unpacklo_epi64(yf3, yf3);
		srcPixelPtr =
		    (MLIB_TYPE *) (lineAddr[ys[0] - 1]) + 3 * (xs[0] - 1);

		a0 = _mm_loadu_si128((__m128i *)
			(srcPixelPtr));
		a1 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + 12));
		b0 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + srcYStride));
		b1 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + srcYStride + 12));
		c0 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + 2 * srcYStride));
		c1 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + 2 * srcYStride + 12));
		d0 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + 3 * srcYStride));
		d1 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + 3 * srcYStride + 12));

		a0 = _mm_xor_si128(a0, mask_8000);
		a1 = _mm_xor_si128(a1, mask_8000);
		b0 = _mm_xor_si128(b0, mask_8000);
		b1 = _mm_xor_si128(b1, mask_8000);
		c0 = _mm_xor_si128(c0, mask_8000);
		c1 = _mm_xor_si128(c1, mask_8000);
		d0 = _mm_xor_si128(d0, mask_8000);
		d1 = _mm_xor_si128(d1, mask_8000);

		a0 = _mm_mulhi_epi16(a0, yf0);
		a1 = _mm_mulhi_epi16(a1, yf0);
		b0 = _mm_mulhi_epi16(b0, yf1);
		b1 = _mm_mulhi_epi16(b1, yf1);
		c0 = _mm_mulhi_epi16(c0, yf2);
		c1 = _mm_mulhi_epi16(c1, yf2);
		d0 = _mm_mulhi_epi16(d0, yf3);
		d1 = _mm_mulhi_epi16(d1, yf3);

		a0 = _mm_add_epi16(a0, b0);
		a1 = _mm_add_epi16(a1, b1);
		c0 = _mm_add_epi16(c0, d0);
		c1 = _mm_add_epi16(c1, d1);
		a0 = _mm_add_epi16(a0, c0);
		a1 = _mm_add_epi16(a1, c1);
		a0 = _mm_mulhi_epi16(a0, xf0);
		a1 = _mm_mulhi_epi16(a1, xf1);
		a0 = _mm_add_epi16(a0, a1);
		a0 = _mm_add_epi16(a0, _mm_srli_si128(a0, 6));
		a0 = _mm_add_epi16(a0, a0);
		a0 = _mm_adds_epi16(a0, a0);
		a0 = _mm_xor_si128(mask_8000, a0);

		rr = _mm_cvtsi128_si32(a0);

		xdst = xd[i] * 3;
		dstData1[xdst] = rr;
		dstData1[xdst + 1] = (rr >> 16);
		dstData1[xdst + 2] = _mm_cvtsi128_si32(_mm_srli_si128(a0, 4));
		xs++;
		ys++;
		xcoeff++;
		ycoeff++;
	}
}
/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_U16_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	DECLAREVAR;
	mlib_u8 *filter_table_y = (mlib_u8 *)pws->filter_table_vis;
	__m128i xf0, xf1, xf2, xf3;
	__m128i yf0, yf1, yf2, yf3;
	__m128i a0, a1, a2, a3, b0, b1, b2, b3;
	__m128i c0, c1, c2, c3, d0, d1, d2, d3;
	__m128i *fptr;
	__m128i mask_8000 = _mm_set1_epi32(0x80008000);
	mlib_d64 *dstData1 = (mlib_d64 *) dstData;


	for (i = 0; i < n; i++) {
		filterpos = (xcoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m128i *) (filter_table_y + 4 * filterpos);
		xf0 = _mm_loadu_si128(fptr);
		xf1 = _mm_loadu_si128(fptr + 1);

		filterpos = (ycoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m128i *) (filter_table_y + 4 * filterpos);
		yf0 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 0));
		yf0 = _mm_unpacklo_epi64(yf0, yf0);
		yf1 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 1));
		yf1 = _mm_unpacklo_epi64(yf1, yf1);
		yf2 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 2));
		yf2 = _mm_unpacklo_epi64(yf2, yf2);
		yf3 = _mm_loadl_epi64((__m128i *)((mlib_d64 *)fptr + 3));
		yf3 = _mm_unpacklo_epi64(yf3, yf3);

		srcPixelPtr =
		    (MLIB_TYPE *) (lineAddr[ys[0] - 1]) + 4 * (xs[0] - 1);

		a0 = _mm_loadu_si128((__m128i *)
			(srcPixelPtr));
		a1 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + 16));
		b0 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + srcYStride));
		b1 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + srcYStride + 16));
		c0 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + 2 * srcYStride));
		c1 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + 2 * srcYStride + 16));
		d0 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + 3 * srcYStride));
		d1 = _mm_loadu_si128((__m128i *)
			((mlib_u8 *)srcPixelPtr + 3 * srcYStride + 16));

		a0 = _mm_xor_si128(a0, mask_8000);
		a1 = _mm_xor_si128(a1, mask_8000);
		b0 = _mm_xor_si128(b0, mask_8000);
		b1 = _mm_xor_si128(b1, mask_8000);
		c0 = _mm_xor_si128(c0, mask_8000);
		c1 = _mm_xor_si128(c1, mask_8000);
		d0 = _mm_xor_si128(d0, mask_8000);
		d1 = _mm_xor_si128(d1, mask_8000);

		a0 = _mm_mulhi_epi16(a0, yf0);
		a1 = _mm_mulhi_epi16(a1, yf0);
		b0 = _mm_mulhi_epi16(b0, yf1);
		b1 = _mm_mulhi_epi16(b1, yf1);
		c0 = _mm_mulhi_epi16(c0, yf2);
		c1 = _mm_mulhi_epi16(c1, yf2);
		d0 = _mm_mulhi_epi16(d0, yf3);
		d1 = _mm_mulhi_epi16(d1, yf3);

		a0 = _mm_add_epi16(a0, b0);
		a1 = _mm_add_epi16(a1, b1);
		c0 = _mm_add_epi16(c0, d0);
		c1 = _mm_add_epi16(c1, d1);
		a0 = _mm_add_epi16(a0, c0);
		a1 = _mm_add_epi16(a1, c1);
		a0 = _mm_mulhi_epi16(a0, xf0);
		a1 = _mm_mulhi_epi16(a1, xf1);
		a0 = _mm_add_epi16(a0, a1);
		a0 = _mm_add_epi16(a0, _mm_srli_si128(a0, 8));
		a0 = _mm_add_epi16(a0, a0);
		a0 = _mm_adds_epi16(a0, a0);
		a0 = _mm_xor_si128(mask_8000, a0);

		dstData1[xd[0]] = *((mlib_d64 *)&a0);
		xs++;
		ys++;
		xd++;
		xcoeff++;
		ycoeff++;
	}
}
/* *********************************************************** */
