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

#pragma ident	"@(#)mlib_m_ImagePolynomialWarp_BC.c	9.4	07/11/05 SMI"

/* *********************************************************** */

#define	MMX_USE_MODIF_8x16

#define	MMX_USE_MEM_ZERO

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <mmintrin.h>
#endif

#include <mlib_mmx_utils.h>
#include <mlib_image.h>
#include <mlib_ImagePolynomialWarp.h>

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
	__m64 _m_mzero[1] = { 0 };
	__m64 xf0;
	__m64 yf0, yf1, yf2, yf3;
	__m64 r0, r1, r2, r3, d0;
	__m64 *fptr;
	mlib_s32 s0, s1, s2, s3;
	mlib_s32 ri;

	for (i = 0; i < n; i++) {
		filterpos = (xcoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		xf0 = *((__m64 *) (filter_table_x + filterpos));
		filterpos = (ycoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m64 *) (filter_table_y + 4 * filterpos);
		yf0 = fptr[0];
		yf1 = fptr[1];
		yf2 = fptr[2];
		yf3 = fptr[3];
		srcPixelPtr = (MLIB_TYPE *) (lineAddr[ys[0] - 1]) + xs[0] - 1;
		s0 = *(mlib_s32 *)(srcPixelPtr);
		s1 = *(mlib_s32 *)(srcPixelPtr + srcYStride);
		s2 = *(mlib_s32 *)(srcPixelPtr + 2 * srcYStride);
		s3 = *(mlib_s32 *)(srcPixelPtr + 3 * srcYStride);
		r0 = mmx_fmul8x16hi(_m_from_int(s0), yf0);
		r1 = mmx_fmul8x16hi(_m_from_int(s1), yf1);
		r2 = mmx_fmul8x16hi(_m_from_int(s2), yf2);
		r3 = mmx_fmul8x16hi(_m_from_int(s3), yf3);
		r0 = _mm_add_pi16(r0, r1);
		r2 = _mm_add_pi16(r2, r3);
		d0 = _mm_add_pi16(r0, r2);
		d0 = _mm_madd_pi16(d0, xf0);
		d0 = _mm_add_pi32(d0, _mm_srli_si64(d0, 32));
		ri = mmx_fpack16(d0);
		dstData[xd[0]] = (ri >> 8);
		xs++;
		ys++;
		xd++;
		xcoeff++;
		ycoeff++;
	}

	_mm_empty();
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
	__m64 _m_mzero[1] = { 0 };
	__m64 xf0, xf1, xf2;
	__m64 yf0, yf1, yf2, yf3;
	__m64 a0, a1, a2, a3, b0, b1, b2, b3;
	__m64 *fptr;
	mlib_s32 ri;
	mlib_u16 *dstData1 = (mlib_u16 *)dstData;

	for (i = 0; i < n; i++) {
		filterpos = (xcoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		xf0 = *((__m64 *) (filter_table_x + filterpos));
		xf1 = _mm_unpacklo_pi16(xf0, xf0);
		xf2 = _mm_unpackhi_pi16(xf0, xf0);
		filterpos = (ycoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m64 *) (filter_table_y + 4 * filterpos);
		yf0 = fptr[0];
		yf1 = fptr[1];
		yf2 = fptr[2];
		yf3 = fptr[3];
		srcPixelPtr =
		    (MLIB_TYPE *) (lineAddr[ys[0] - 1]) + 2 * (xs[0] - 1);
		a0 = mmx_fmul8x16hi(*(__m64 *) (srcPixelPtr), yf0);
		a1 = mmx_fmul8x16hi(*(__m64 *) (srcPixelPtr + srcYStride), yf1);
		a2 = mmx_fmul8x16hi(*(__m64 *) (srcPixelPtr + 2 * srcYStride),
		    yf2);
		a3 = mmx_fmul8x16hi(*(__m64 *) (srcPixelPtr + 3 * srcYStride),
		    yf3);
		b0 = mmx_fmul8x16hi(*(__m64 *) (srcPixelPtr + 4), yf0);
		b1 = mmx_fmul8x16hi(*(__m64 *) (srcPixelPtr + srcYStride + 4),
		    yf1);
		b2 = mmx_fmul8x16hi(*(__m64 *) (srcPixelPtr + 2 * srcYStride +
		    4), yf2);
		b3 = mmx_fmul8x16hi(*(__m64 *) (srcPixelPtr + 3 * srcYStride +
		    4), yf3);
		a0 = _mm_add_pi16(a0, a1);
		a2 = _mm_add_pi16(a2, a3);
		a0 = _mm_add_pi16(a0, a2);
		b0 = _mm_add_pi16(b0, b1);
		b2 = _mm_add_pi16(b2, b3);
		b0 = _mm_add_pi16(b0, b2);
		a0 = _mm_mulhi_pi16(a0, xf1);
		b0 = _mm_mulhi_pi16(b0, xf2);
		a0 = _mm_add_pi16(a0, b0);
		a0 = _mm_add_pi16(a0, _mm_srli_si64(a0, 32));
		ri = mmx_fpack16(a0);
		dstData1[xd[0]] = ri;
		xs++;
		ys++;
		xd++;
		xcoeff++;
		ycoeff++;
	}

	_mm_empty();
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
	mlib_u8 *filter_table_y = (mlib_u8 *)pws->filter_table_vis;
	__m64 _m_mzero[1] = { 0 };
	__m64 xf0, xf1, xf2, xf3;
	__m64 yf0, yf1, yf2, yf3;
	__m64 a0, a1, a2, a3, b0, b1, b2, b3;
	__m64 c0, c1, c2, c3, d0, d1, d2, d3;
	__m64 *fptr;
	mlib_s32 ri, xdst;

	for (i = 0; i < n; i++) {
		filterpos = (xcoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m64 *) (filter_table_y + 4 * filterpos);
		xf0 = fptr[0];
		xf1 = fptr[1];
		xf2 = fptr[2];
		xf3 = fptr[3];
		filterpos = (ycoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m64 *) (filter_table_y + 4 * filterpos);
		yf0 = fptr[0];
		yf1 = fptr[1];
		yf2 = fptr[2];
		yf3 = fptr[3];
		srcPixelPtr =
		    (MLIB_TYPE *) (lineAddr[ys[0] - 1]) + 3 * (xs[0] - 1);
		a0 = mmx_fmul8x16hi(*(__m64 *) (srcPixelPtr), yf0);
		a1 = mmx_fmul8x16hi(*(__m64 *) (srcPixelPtr + 3), yf0);
		a2 = mmx_fmul8x16hi(*(__m64 *) (srcPixelPtr + 6), yf0);
		a3 = mmx_fmul8x16hi(*(__m64 *) (srcPixelPtr + 9), yf0);
		b0 = mmx_fmul8x16hi(*(__m64 *) (srcPixelPtr + srcYStride), yf1);
		b1 = mmx_fmul8x16hi(*(__m64 *) (srcPixelPtr + srcYStride + 3),
		    yf1);
		b2 = mmx_fmul8x16hi(*(__m64 *) (srcPixelPtr + srcYStride + 6),
		    yf1);
		b3 = mmx_fmul8x16hi(*(__m64 *) (srcPixelPtr + srcYStride + 9),
		    yf1);
		c0 = mmx_fmul8x16hi(*(__m64 *) (srcPixelPtr + 2 * srcYStride),
		    yf2);
		c1 = mmx_fmul8x16hi(*(__m64 *) (srcPixelPtr + 2 * srcYStride +
		    3), yf2);
		c2 = mmx_fmul8x16hi(*(__m64 *) (srcPixelPtr + 2 * srcYStride +
		    6), yf2);
		c3 = mmx_fmul8x16hi(*(__m64 *) (srcPixelPtr + 2 * srcYStride +
		    9), yf2);
		d0 = mmx_fmul8x16hi(*(__m64 *) (srcPixelPtr + 3 * srcYStride),
		    yf3);
		d1 = mmx_fmul8x16hi(*(__m64 *) (srcPixelPtr + 3 * srcYStride +
		    3), yf3);
		d2 = mmx_fmul8x16hi(*(__m64 *) (srcPixelPtr + 3 * srcYStride +
		    6), yf3);
		d3 = mmx_fmul8x16hi(*(__m64 *) (srcPixelPtr + 3 * srcYStride +
		    9), yf3);
		a0 = _mm_add_pi16(a0, b0);
		a1 = _mm_add_pi16(a1, b1);
		a2 = _mm_add_pi16(a2, b2);
		a3 = _mm_add_pi16(a3, b3);
		c0 = _mm_add_pi16(c0, d0);
		c1 = _mm_add_pi16(c1, d1);
		c2 = _mm_add_pi16(c2, d2);
		c3 = _mm_add_pi16(c3, d3);
		a0 = _mm_add_pi16(a0, c0);
		a1 = _mm_add_pi16(a1, c1);
		a2 = _mm_add_pi16(a2, c2);
		a3 = _mm_add_pi16(a3, c3);
		a0 = _mm_mulhi_pi16(a0, xf0);
		a1 = _mm_mulhi_pi16(a1, xf1);
		a2 = _mm_mulhi_pi16(a2, xf2);
		a3 = _mm_mulhi_pi16(a3, xf3);
		a0 = _mm_add_pi16(a0, a1);
		a2 = _mm_add_pi16(a2, a3);
		a0 = _mm_add_pi16(a0, a2);
		ri = mmx_fpack16(a0);
		xdst = xd[i] * 3;
		dstData[xdst] = ri;
		dstData[xdst + 1] = (ri >> 8);
		dstData[xdst + 2] = (ri >> 16);
		xs++;
		ys++;
		xcoeff++;
		ycoeff++;
	}

	_mm_empty();
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
	__m64 _m_mzero[1] = { 0 };
	__m64 xf0, xf1, xf2, xf3;
	__m64 yf0, yf1, yf2, yf3;
	__m64 a0, a1, a2, a3, b0, b1, b2, b3;
	__m64 c0, c1, c2, c3, d0, d1, d2, d3;
	__m64 *fptr;
	mlib_s32 ri;
	mlib_s32 *dstData1 = (mlib_s32 *)dstData;

	for (i = 0; i < n; i++) {
		filterpos = (xcoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m64 *) (filter_table_y + 4 * filterpos);
		xf0 = fptr[0];
		xf1 = fptr[1];
		xf2 = fptr[2];
		xf3 = fptr[3];
		filterpos = (ycoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m64 *) (filter_table_y + 4 * filterpos);
		yf0 = fptr[0];
		yf1 = fptr[1];
		yf2 = fptr[2];
		yf3 = fptr[3];
		srcPixelPtr =
		    (MLIB_TYPE *) (lineAddr[ys[0] - 1]) + 4 * (xs[0] - 1);
		a1 = *(__m64 *) (srcPixelPtr);
		a3 = *(__m64 *) (srcPixelPtr + 8);
		a0 = mmx_fmul8x16hi(a1, yf0);
		a1 = mmx_fmul8x16lo(a1, yf0);
		a2 = mmx_fmul8x16hi(a3, yf0);
		a3 = mmx_fmul8x16lo(a3, yf0);
		b1 = *(__m64 *) (srcPixelPtr + srcYStride);
		b3 = *(__m64 *) (srcPixelPtr + srcYStride + 8);
		b0 = mmx_fmul8x16hi(b1, yf1);
		b1 = mmx_fmul8x16lo(b1, yf1);
		b2 = mmx_fmul8x16hi(b3, yf1);
		b3 = mmx_fmul8x16lo(b3, yf1);
		c1 = *(__m64 *) (srcPixelPtr + 2 * srcYStride);
		c3 = *(__m64 *) (srcPixelPtr + 2 * srcYStride + 8);
		c0 = mmx_fmul8x16hi(c1, yf2);
		c1 = mmx_fmul8x16lo(c1, yf2);
		c2 = mmx_fmul8x16hi(c3, yf2);
		c3 = mmx_fmul8x16lo(c3, yf2);
		d1 = *(__m64 *) (srcPixelPtr + 3 * srcYStride);
		d3 = *(__m64 *) (srcPixelPtr + 3 * srcYStride + 8);
		d0 = mmx_fmul8x16hi(d1, yf3);
		d1 = mmx_fmul8x16lo(d1, yf3);
		d2 = mmx_fmul8x16hi(d3, yf3);
		d3 = mmx_fmul8x16lo(d3, yf3);
		a0 = _mm_add_pi16(a0, b0);
		a1 = _mm_add_pi16(a1, b1);
		a2 = _mm_add_pi16(a2, b2);
		a3 = _mm_add_pi16(a3, b3);
		c0 = _mm_add_pi16(c0, d0);
		c1 = _mm_add_pi16(c1, d1);
		c2 = _mm_add_pi16(c2, d2);
		c3 = _mm_add_pi16(c3, d3);
		a0 = _mm_add_pi16(a0, c0);
		a1 = _mm_add_pi16(a1, c1);
		a2 = _mm_add_pi16(a2, c2);
		a3 = _mm_add_pi16(a3, c3);
		a0 = _mm_mulhi_pi16(a0, xf0);
		a1 = _mm_mulhi_pi16(a1, xf1);
		a2 = _mm_mulhi_pi16(a2, xf2);
		a3 = _mm_mulhi_pi16(a3, xf3);
		a0 = _mm_add_pi16(a0, a1);
		a2 = _mm_add_pi16(a2, a3);
		a0 = _mm_add_pi16(a0, a2);
		ri = mmx_fpack16(a0);
		dstData1[xd[0]] = ri;
		xs++;
		ys++;
		xd++;
		xcoeff++;
		ycoeff++;
	}

	_mm_empty();
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
	__m64 xf0;
	__m64 yf0, yf1, yf2, yf3;
	__m64 a0, a1, a2, a3;
	__m64 *fptr;
	mlib_u16 *dstData1 = (mlib_u16 *)dstData;

	for (i = 0; i < n; i++) {
		filterpos = (xcoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		xf0 = *((__m64 *) (filter_table_x + filterpos));
		filterpos = (ycoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m64 *) (filter_table_y + 4 * filterpos);
		yf0 = fptr[0];
		yf1 = fptr[1];
		yf2 = fptr[2];
		yf3 = fptr[3];
		srcPixelPtr = (MLIB_TYPE *) (lineAddr[ys[0] - 1]) + (xs[0] - 1);
		a0 = *(__m64 *) (srcPixelPtr);
		a1 = *(__m64 *) ((mlib_u8 *)srcPixelPtr + srcYStride);
		a2 = *(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * srcYStride);
		a3 = *(__m64 *) ((mlib_u8 *)srcPixelPtr + 3 * srcYStride);
		a0 = _mm_mulhi_pi16(a0, yf0);
		a1 = _mm_mulhi_pi16(a1, yf1);
		a2 = _mm_mulhi_pi16(a2, yf2);
		a3 = _mm_mulhi_pi16(a3, yf3);
		a0 = _mm_add_pi16(a0, a1);
		a2 = _mm_add_pi16(a2, a3);
		a0 = _mm_add_pi16(a0, a2);
		a0 = _mm_madd_pi16(a0, xf0);
		a0 = _mm_add_pi32(a0, _mm_srli_si64(a0, 32));
		a0 = _mm_srli_pi32(a0, 15);
		a0 = _mm_adds_pi16(a0, a0);
		dstData1[xd[0]] = _m_to_int(a0);
		xs++;
		ys++;
		xd++;
		xcoeff++;
		ycoeff++;
	}

	_mm_empty();
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
	__m64 xf0, xf1, xf2;
	__m64 yf0, yf1, yf2, yf3;
	__m64 a0, a1, a2, a3, b0, b1, b2, b3;
	__m64 *fptr;
	mlib_s32 *dstData1 = (mlib_s32 *)dstData;

	for (i = 0; i < n; i++) {
		filterpos = (xcoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		xf0 = *((__m64 *) (filter_table_x + filterpos));
		xf1 = _mm_unpacklo_pi16(xf0, xf0);
		xf2 = _mm_unpackhi_pi16(xf0, xf0);
		filterpos = (ycoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m64 *) (filter_table_y + 4 * filterpos);
		yf0 = fptr[0];
		yf1 = fptr[1];
		yf2 = fptr[2];
		yf3 = fptr[3];
		srcPixelPtr =
		    (MLIB_TYPE *) (lineAddr[ys[0] - 1]) + 2 * (xs[0] - 1);
		a0 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr), yf0);
		a1 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    srcYStride), yf1);
		a2 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    2 * srcYStride), yf2);
		a3 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    3 * srcYStride), yf3);
		b0 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr + 8),
		    yf0);
		b1 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    srcYStride + 8), yf1);
		b2 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    2 * srcYStride + 8), yf2);
		b3 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    3 * srcYStride + 8), yf3);
		a0 = _mm_add_pi16(a0, a1);
		a2 = _mm_add_pi16(a2, a3);
		a0 = _mm_add_pi16(a0, a2);
		b0 = _mm_add_pi16(b0, b1);
		b2 = _mm_add_pi16(b2, b3);
		b0 = _mm_add_pi16(b0, b2);
		a0 = _mm_mulhi_pi16(a0, xf1);
		b0 = _mm_mulhi_pi16(b0, xf2);
		a0 = _mm_add_pi16(a0, b0);
		a0 = _mm_add_pi16(a0, _mm_srli_si64(a0, 32));
		a0 = _mm_add_pi16(a0, a0);
		a0 = _mm_adds_pi16(a0, a0);
		dstData1[xd[0]] = *(mlib_s32 *)&a0;
		xs++;
		ys++;
		xd++;
		xcoeff++;
		ycoeff++;
	}

	_mm_empty();
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
	mlib_u8 *filter_table_y = (mlib_u8 *)pws->filter_table_vis;
	__m64 xf0, xf1, xf2, xf3;
	__m64 yf0, yf1, yf2, yf3;
	__m64 a0, a1, a2, a3, b0, b1, b2, b3;
	__m64 c0, c1, c2, c3, d0, d1, d2, d3;
	__m64 *fptr;
	mlib_u16 *dstData1 = (mlib_u16 *)dstData;
	mlib_s32 rr, xdst;

	for (i = 0; i < n; i++) {
		filterpos = (xcoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m64 *) (filter_table_y + 4 * filterpos);
		xf0 = fptr[0];
		xf1 = fptr[1];
		xf2 = fptr[2];
		xf3 = fptr[3];
		filterpos = (ycoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m64 *) (filter_table_y + 4 * filterpos);
		yf0 = fptr[0];
		yf1 = fptr[1];
		yf2 = fptr[2];
		yf3 = fptr[3];
		srcPixelPtr =
		    (MLIB_TYPE *) (lineAddr[ys[0] - 1]) + 3 * (xs[0] - 1);
		a0 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr), yf0);
		a1 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * 3),
		    yf0);
		a2 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * 6),
		    yf0);
		a3 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * 9),
		    yf0);
		b0 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    srcYStride), yf1);
		b1 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    srcYStride + 2 * 3), yf1);
		b2 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    srcYStride + 2 * 6), yf1);
		b3 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    srcYStride + 2 * 9), yf1);
		c0 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    2 * srcYStride), yf2);
		c1 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    2 * srcYStride + 2 * 3), yf2);
		c2 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    2 * srcYStride + 2 * 6), yf2);
		c3 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    2 * srcYStride + 2 * 9), yf2);
		d0 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    3 * srcYStride), yf3);
		d1 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    3 * srcYStride + 2 * 3), yf3);
		d2 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    3 * srcYStride + 2 * 6), yf3);
		d3 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    3 * srcYStride + 2 * 9), yf3);
		a0 = _mm_add_pi16(a0, b0);
		a1 = _mm_add_pi16(a1, b1);
		a2 = _mm_add_pi16(a2, b2);
		a3 = _mm_add_pi16(a3, b3);
		c0 = _mm_add_pi16(c0, d0);
		c1 = _mm_add_pi16(c1, d1);
		c2 = _mm_add_pi16(c2, d2);
		c3 = _mm_add_pi16(c3, d3);
		a0 = _mm_add_pi16(a0, c0);
		a1 = _mm_add_pi16(a1, c1);
		a2 = _mm_add_pi16(a2, c2);
		a3 = _mm_add_pi16(a3, c3);
		a0 = _mm_mulhi_pi16(a0, xf0);
		a1 = _mm_mulhi_pi16(a1, xf1);
		a2 = _mm_mulhi_pi16(a2, xf2);
		a3 = _mm_mulhi_pi16(a3, xf3);
		a0 = _mm_add_pi16(a0, a1);
		a2 = _mm_add_pi16(a2, a3);
		a0 = _mm_add_pi16(a0, a2);
		a0 = _mm_add_pi16(a0, a0);
		a0 = _mm_adds_pi16(a0, a0);
		rr = mmx_read_hi(a0);
		xdst = xd[i] * 3;
		dstData1[xdst] = rr;
		dstData1[xdst + 1] = (rr >> 16);
		dstData1[xdst + 2] = mmx_read_lo(a0);
		xs++;
		ys++;
		xcoeff++;
		ycoeff++;
	}

	_mm_empty();
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
	__m64 xf0, xf1, xf2, xf3;
	__m64 yf0, yf1, yf2, yf3;
	__m64 a0, a1, a2, a3, b0, b1, b2, b3;
	__m64 c0, c1, c2, c3, d0, d1, d2, d3;
	__m64 *fptr;
	__m64 *dstData1 = (__m64 *) dstData;

	for (i = 0; i < n; i++) {
		filterpos = (xcoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m64 *) (filter_table_y + 4 * filterpos);
		xf0 = fptr[0];
		xf1 = fptr[1];
		xf2 = fptr[2];
		xf3 = fptr[3];
		filterpos = (ycoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m64 *) (filter_table_y + 4 * filterpos);
		yf0 = fptr[0];
		yf1 = fptr[1];
		yf2 = fptr[2];
		yf3 = fptr[3];
		srcPixelPtr =
		    (MLIB_TYPE *) (lineAddr[ys[0] - 1]) + 4 * (xs[0] - 1);
		a0 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr), yf0);
		a1 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * 4),
		    yf0);
		a2 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * 8),
		    yf0);
		a3 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    2 * 12), yf0);
		b0 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    srcYStride), yf1);
		b1 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    srcYStride + 2 * 4), yf1);
		b2 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    srcYStride + 2 * 8), yf1);
		b3 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    srcYStride + 2 * 12), yf1);
		c0 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    2 * srcYStride), yf2);
		c1 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    2 * srcYStride + 2 * 4), yf2);
		c2 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    2 * srcYStride + 2 * 8), yf2);
		c3 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    2 * srcYStride + 2 * 12), yf2);
		d0 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    3 * srcYStride), yf3);
		d1 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    3 * srcYStride + 2 * 4), yf3);
		d2 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    3 * srcYStride + 2 * 8), yf3);
		d3 = _mm_mulhi_pi16(*(__m64 *) ((mlib_u8 *)srcPixelPtr +
		    3 * srcYStride + 2 * 12), yf3);
		a0 = _mm_add_pi16(a0, b0);
		a1 = _mm_add_pi16(a1, b1);
		a2 = _mm_add_pi16(a2, b2);
		a3 = _mm_add_pi16(a3, b3);
		c0 = _mm_add_pi16(c0, d0);
		c1 = _mm_add_pi16(c1, d1);
		c2 = _mm_add_pi16(c2, d2);
		c3 = _mm_add_pi16(c3, d3);
		a0 = _mm_add_pi16(a0, c0);
		a1 = _mm_add_pi16(a1, c1);
		a2 = _mm_add_pi16(a2, c2);
		a3 = _mm_add_pi16(a3, c3);
		a0 = _mm_mulhi_pi16(a0, xf0);
		a1 = _mm_mulhi_pi16(a1, xf1);
		a2 = _mm_mulhi_pi16(a2, xf2);
		a3 = _mm_mulhi_pi16(a3, xf3);
		a0 = _mm_add_pi16(a0, a1);
		a2 = _mm_add_pi16(a2, a3);
		a0 = _mm_add_pi16(a0, a2);
		a0 = _mm_add_pi16(a0, a0);
		a0 = _mm_adds_pi16(a0, a0);
		dstData1[xd[0]] = a0;
		xs++;
		ys++;
		xd++;
		xcoeff++;
		ycoeff++;
	}

	_mm_empty();
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
	__m64 xf0;
	__m64 yf0, yf1, yf2, yf3;
	__m64 a0, a1, a2, a3;
	__m64 *fptr;
	__m64 mask_8000 = mmx_to_double_dup(0x80008000);
	mlib_u16 *dstData1 = (mlib_u16 *)dstData;

	for (i = 0; i < n; i++) {
		filterpos = (xcoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		xf0 = *((__m64 *) (filter_table_x + filterpos));
		filterpos = (ycoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m64 *) (filter_table_y + 4 * filterpos);
		yf0 = fptr[0];
		yf1 = fptr[1];
		yf2 = fptr[2];
		yf3 = fptr[3];
		srcPixelPtr = (MLIB_TYPE *) (lineAddr[ys[0] - 1]) + (xs[0] - 1);
		a0 = *(__m64 *) (srcPixelPtr);
		a1 = *(__m64 *) ((mlib_u8 *)srcPixelPtr + srcYStride);
		a2 = *(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * srcYStride);
		a3 = *(__m64 *) ((mlib_u8 *)srcPixelPtr + 3 * srcYStride);
		a0 = _mm_xor_si64(a0, mask_8000);
		a1 = _mm_xor_si64(a1, mask_8000);
		a2 = _mm_xor_si64(a2, mask_8000);
		a3 = _mm_xor_si64(a3, mask_8000);
		a0 = _mm_mulhi_pi16(a0, yf0);
		a1 = _mm_mulhi_pi16(a1, yf1);
		a2 = _mm_mulhi_pi16(a2, yf2);
		a3 = _mm_mulhi_pi16(a3, yf3);
		a0 = _mm_add_pi16(a0, a1);
		a2 = _mm_add_pi16(a2, a3);
		a0 = _mm_add_pi16(a0, a2);
		a0 = _mm_madd_pi16(a0, xf0);
		a0 = _mm_add_pi32(a0, _mm_srli_si64(a0, 32));
		a0 = _mm_srli_pi32(a0, 15);
		a0 = _mm_adds_pi16(a0, a0);
		dstData1[xd[0]] = _m_to_int(a0) ^ 0x8000;
		xs++;
		ys++;
		xd++;
		xcoeff++;
		ycoeff++;
	}

	_mm_empty();
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
	__m64 xf0, xf1, xf2;
	__m64 yf0, yf1, yf2, yf3;
	__m64 a0, a1, a2, a3, b0, b1, b2, b3;
	__m64 *fptr;
	__m64 mask_8000 = mmx_to_double_dup(0x80008000);
	mlib_s32 *dstData1 = (mlib_s32 *)dstData;

	for (i = 0; i < n; i++) {
		filterpos = (xcoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		xf0 = *((__m64 *) (filter_table_x + filterpos));
		xf1 = _mm_unpacklo_pi16(xf0, xf0);
		xf2 = _mm_unpackhi_pi16(xf0, xf0);
		filterpos = (ycoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m64 *) (filter_table_y + 4 * filterpos);
		yf0 = fptr[0];
		yf1 = fptr[1];
		yf2 = fptr[2];
		yf3 = fptr[3];
		srcPixelPtr =
		    (MLIB_TYPE *) (lineAddr[ys[0] - 1]) + 2 * (xs[0] - 1);
		a0 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr)), yf0);
		a1 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + srcYStride)), yf1);
		a2 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * srcYStride)), yf2);
		a3 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 3 * srcYStride)), yf3);
		b0 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 8)), yf0);
		b1 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + srcYStride + 8)), yf1);
		b2 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * srcYStride + 8)),
		    yf2);
		b3 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 3 * srcYStride + 8)),
		    yf3);
		a0 = _mm_add_pi16(a0, a1);
		a2 = _mm_add_pi16(a2, a3);
		a0 = _mm_add_pi16(a0, a2);
		b0 = _mm_add_pi16(b0, b1);
		b2 = _mm_add_pi16(b2, b3);
		b0 = _mm_add_pi16(b0, b2);
		a0 = _mm_mulhi_pi16(a0, xf1);
		b0 = _mm_mulhi_pi16(b0, xf2);
		a0 = _mm_add_pi16(a0, b0);
		a0 = _mm_add_pi16(a0, _mm_srli_si64(a0, 32));
		a0 = _mm_add_pi16(a0, a0);
		a0 = _mm_adds_pi16(a0, a0);
		dstData1[xd[0]] = *(mlib_s32 *)&a0 ^ 0x80008000;
		xs++;
		ys++;
		xd++;
		xcoeff++;
		ycoeff++;
	}

	_mm_empty();
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
	mlib_u8 *filter_table_y = (mlib_u8 *)pws->filter_table_vis;
	__m64 xf0, xf1, xf2, xf3;
	__m64 yf0, yf1, yf2, yf3;
	__m64 a0, a1, a2, a3, b0, b1, b2, b3;
	__m64 c0, c1, c2, c3, d0, d1, d2, d3;
	__m64 *fptr;
	__m64 mask_8000 = mmx_to_double_dup(0x80008000);
	mlib_u16 *dstData1 = (mlib_u16 *)dstData;
	mlib_s32 rr, xdst;

	for (i = 0; i < n; i++) {
		filterpos = (xcoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m64 *) (filter_table_y + 4 * filterpos);
		xf0 = fptr[0];
		xf1 = fptr[1];
		xf2 = fptr[2];
		xf3 = fptr[3];
		filterpos = (ycoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m64 *) (filter_table_y + 4 * filterpos);
		yf0 = fptr[0];
		yf1 = fptr[1];
		yf2 = fptr[2];
		yf3 = fptr[3];
		srcPixelPtr =
		    (MLIB_TYPE *) (lineAddr[ys[0] - 1]) + 3 * (xs[0] - 1);
		a0 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr)), yf0);
		a1 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * 3)), yf0);
		a2 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * 6)), yf0);
		a3 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * 9)), yf0);
		b0 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + srcYStride)), yf1);
		b1 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + srcYStride + 2 * 3)),
		    yf1);
		b2 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + srcYStride + 2 * 6)),
		    yf1);
		b3 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + srcYStride + 2 * 9)),
		    yf1);
		c0 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * srcYStride)), yf2);
		c1 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * srcYStride +
		    2 * 3)), yf2);
		c2 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * srcYStride +
		    2 * 6)), yf2);
		c3 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * srcYStride +
		    2 * 9)), yf2);
		d0 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 3 * srcYStride)), yf3);
		d1 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 3 * srcYStride +
		    2 * 3)), yf3);
		d2 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 3 * srcYStride +
		    2 * 6)), yf3);
		d3 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 3 * srcYStride +
		    2 * 9)), yf3);
		a0 = _mm_add_pi16(a0, b0);
		a1 = _mm_add_pi16(a1, b1);
		a2 = _mm_add_pi16(a2, b2);
		a3 = _mm_add_pi16(a3, b3);
		c0 = _mm_add_pi16(c0, d0);
		c1 = _mm_add_pi16(c1, d1);
		c2 = _mm_add_pi16(c2, d2);
		c3 = _mm_add_pi16(c3, d3);
		a0 = _mm_add_pi16(a0, c0);
		a1 = _mm_add_pi16(a1, c1);
		a2 = _mm_add_pi16(a2, c2);
		a3 = _mm_add_pi16(a3, c3);
		a0 = _mm_mulhi_pi16(a0, xf0);
		a1 = _mm_mulhi_pi16(a1, xf1);
		a2 = _mm_mulhi_pi16(a2, xf2);
		a3 = _mm_mulhi_pi16(a3, xf3);
		a0 = _mm_add_pi16(a0, a1);
		a2 = _mm_add_pi16(a2, a3);
		a0 = _mm_add_pi16(a0, a2);
		a0 = _mm_add_pi16(a0, a0);
		a0 = _mm_adds_pi16(a0, a0);
		a0 = _mm_xor_si64(mask_8000, a0);
		rr = mmx_read_hi(a0);
		xdst = xd[i] * 3;
		dstData1[xdst] = rr;
		dstData1[xdst + 1] = (rr >> 16);
		dstData1[xdst + 2] = mmx_read_lo(a0);
		xs++;
		ys++;
		xcoeff++;
		ycoeff++;
	}

	_mm_empty();
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
	__m64 xf0, xf1, xf2, xf3;
	__m64 yf0, yf1, yf2, yf3;
	__m64 a0, a1, a2, a3, b0, b1, b2, b3;
	__m64 c0, c1, c2, c3, d0, d1, d2, d3;
	__m64 *fptr;
	__m64 mask_8000 = mmx_to_double_dup(0x80008000);
	__m64 *dstData1 = (__m64 *) dstData;

	for (i = 0; i < n; i++) {
		filterpos = (xcoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m64 *) (filter_table_y + 4 * filterpos);
		xf0 = fptr[0];
		xf1 = fptr[1];
		xf2 = fptr[2];
		xf3 = fptr[3];
		filterpos = (ycoeff[0] >> FILTER_SHIFT) & FILTER_MASK;
		fptr = (__m64 *) (filter_table_y + 4 * filterpos);
		yf0 = fptr[0];
		yf1 = fptr[1];
		yf2 = fptr[2];
		yf3 = fptr[3];
		srcPixelPtr =
		    (MLIB_TYPE *) (lineAddr[ys[0] - 1]) + 4 * (xs[0] - 1);
		a0 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr)), yf0);
		a1 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * 4)), yf0);
		a2 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * 8)), yf0);
		a3 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * 12)), yf0);
		b0 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + srcYStride)), yf1);
		b1 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + srcYStride + 2 * 4)),
		    yf1);
		b2 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + srcYStride + 2 * 8)),
		    yf1);
		b3 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + srcYStride + 2 * 12)),
		    yf1);
		c0 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * srcYStride)), yf2);
		c1 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * srcYStride +
		    2 * 4)), yf2);
		c2 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * srcYStride +
		    2 * 8)), yf2);
		c3 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * srcYStride +
		    2 * 12)), yf2);
		d0 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 3 * srcYStride)), yf3);
		d1 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 3 * srcYStride +
		    2 * 4)), yf3);
		d2 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 3 * srcYStride +
		    2 * 8)), yf3);
		d3 = _mm_mulhi_pi16(_mm_xor_si64(mask_8000,
		    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 3 * srcYStride +
		    2 * 12)), yf3);
		a0 = _mm_add_pi16(a0, b0);
		a1 = _mm_add_pi16(a1, b1);
		a2 = _mm_add_pi16(a2, b2);
		a3 = _mm_add_pi16(a3, b3);
		c0 = _mm_add_pi16(c0, d0);
		c1 = _mm_add_pi16(c1, d1);
		c2 = _mm_add_pi16(c2, d2);
		c3 = _mm_add_pi16(c3, d3);
		a0 = _mm_add_pi16(a0, c0);
		a1 = _mm_add_pi16(a1, c1);
		a2 = _mm_add_pi16(a2, c2);
		a3 = _mm_add_pi16(a3, c3);
		a0 = _mm_mulhi_pi16(a0, xf0);
		a1 = _mm_mulhi_pi16(a1, xf1);
		a2 = _mm_mulhi_pi16(a2, xf2);
		a3 = _mm_mulhi_pi16(a3, xf3);
		a0 = _mm_add_pi16(a0, a1);
		a2 = _mm_add_pi16(a2, a3);
		a0 = _mm_add_pi16(a0, a2);
		a0 = _mm_add_pi16(a0, a0);
		a0 = _mm_adds_pi16(a0, a0);
		dstData1[xd[0]] = _mm_xor_si64(mask_8000, a0);
		xs++;
		ys++;
		xd++;
		xcoeff++;
		ycoeff++;
	}

	_mm_empty();
}

/* *********************************************************** */
