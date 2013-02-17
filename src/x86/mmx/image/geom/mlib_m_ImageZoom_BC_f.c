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
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_m_ImageZoom_BC_f.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoom - image scaling with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoom(mlib_image  *dst,
 *                                 mlib_image  *src,
 *                                 mlib_s32    zoomx,
 *                                 mlib_s32    zoomy,
 *                                 mlib_filter filter,
 *                                 mlib_edge   edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image
 *      src       Pointer to source image
 *      zoomx     X zoom factor.
 *      zoomy     Y zoom factor.
 *      filter    Type of resampling filter.
 *      edge      Type of edge condition.
 *
 * DESCRIPTION
 *  The center of the source image is mapped to the center of the
 *  destination image.
 *
 *  The upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *  The resampling filter can be one of the following:
 *    MLIB_NEAREST
 *    MLIB_BILINEAR
 *    MLIB_BICUBIC
 *    MLIB_BICUBIC2
 *
 *  The edge condition can be one of the following:
 *    MLIB_EDGE_DST_NO_WRITE  (default)
 *    MLIB_EDGE_DST_FILL_ZERO
 *    MLIB_EDGE_OP_NEAREST
 *    MLIB_EDGE_SRC_EXTEND
 *    MLIB_EDGE_SRC_PADDED
 */

/* *********************************************************** */

#include <mlib_image.h>
#include <mlib_m_ImageFilters.h>
#include <mlib_mmx_utils.h>
#include <mlib_ImageZoom.h>
#include <mlib_mmx_utils.h>

/* *********************************************************** */

#define	USE_MODIF_8x16

/* *********************************************************** */

#define	VARIABLE(FORMAT)                                            \
	FORMAT *sp = GetElemSubStruct(current, sp), *dp =           \
	GetElemSubStruct(current, dp);                              \
	mlib_u8 *filter1 = GetElemStruct(filter1);                  \
	mlib_u8 *filter4 = GetElemStruct(filter4);                  \
	mlib_s32 i, j, cx, dx = GetElemStruct(DX), dy =             \
	GetElemStruct(DY), x =                                      \
	GetElemSubStruct(current, srcX) & MLIB_MASK, y =            \
	GetElemSubStruct(current, srcY) & MLIB_MASK, src_stride =   \
	GetElemStruct(src_stride), dst_stride =                     \
	GetElemStruct(dst_stride), width =                          \
	GetElemSubStruct(current, width), height =                  \
	GetElemSubStruct(current, height);

/* *********************************************************** */

#define	MLIB_XY_SHIFT	5
#define	MLIB_XY_MASK	(((1 << 8) - 1) << 3)

#define	FILTER_SHIFT	5
#define	FILTER_MASK	(((1 << 8) - 1) << 3)

/* *********************************************************** */

#define	COEFF_D64_U8(r)                                         \
	fpos = (r >> MLIB_XY_SHIFT) & MLIB_XY_MASK;             \
	dfPtr = (__m64 *) ((mlib_u8 *)filter4 + fpos * 4);      \
	r##f0 = dfPtr[0];                                       \
	r##f1 = dfPtr[1];                                       \
	r##f2 = dfPtr[2];                                       \
	r##f3 = dfPtr[3];

/* *********************************************************** */

#define	COEFF_D64_U8_2                                          \
	dxx0 =                                                  \
	_mm_or_si64(_mm_andnot_si64(mask_ffff, dxf0),           \
	    _mm_and_si64(mask_ffff, dxf1));                     \
	dxx1 =                                                  \
	_mm_or_si64(_mm_andnot_si64(mask_ffff, dxf2),           \
	    _mm_and_si64(mask_ffff, dxf3));

/* *********************************************************** */

#undef  MMX_SCALE
#define	MMX_SCALE	3

/* *********************************************************** */

mlib_status
mlib_m_ImageZoom_U8_1_Bicubic(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	mlib_u8 *tsp, *tsp0;
	mlib_s32 fpos;
	__m64 *dfPtr, yf0, yf1, yf2, yf3, dxf;
	__m64 r0, r1, r2, r3, d0;
	mlib_s32 s0, s1, s2, s3;

	sp -= src_stride + 1;
	tsp = sp;

	for (j = 0; j < height; j++) {
		COEFF_D64_U8(y)

		    tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		for (i = 0; i < width; i++) {
			fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
			dxf = *(__m64 *) ((mlib_u8 *)filter1 + fpos);
			tsp0 = tsp + (x >> MLIB_SHIFT);
			x += dx;

			s0 = *(mlib_s32 *)(tsp0);
			s1 = *(mlib_s32 *)(tsp0 + src_stride);
			s2 = *(mlib_s32 *)(tsp0 + 2 * src_stride);
			s3 = *(mlib_s32 *)(tsp0 + 3 * src_stride);

			r0 = mmx_fmul8x16(s0, yf0);
			r1 = mmx_fmul8x16(s1, yf1);
			r2 = mmx_fmul8x16(s2, yf2);
			r3 = mmx_fmul8x16(s3, yf3);
			r0 = _mm_add_pi16(r0, r1);
			r2 = _mm_add_pi16(r2, r3);
			d0 = _mm_add_pi16(r0, r2);
			d0 = _mm_mulhi_pi16(d0, dxf);
			d0 = _mm_add_pi16(d0, _mm_srli_si64(d0, 32));
			d0 = _mm_add_pi16(d0, _mm_srli_si64(d0, 16));
			dp[i] = mmx_fpack16(d0);
		}

		y += dy;
		dp += dst_stride;
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_m_ImageZoom_U8_2_Bicubic(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	mlib_u8 *tsp, *srcPixelPtr;
	mlib_s32 fpos;
	__m64 *dfPtr;
	__m64 xf0, xf1, xf2;
	__m64 yf0, yf1, yf2, yf3;
	__m64 a0, a1, a2, a3, b0, b1, b2, b3;

	sp -= src_stride + 2;

	for (j = 0; j < height; j++) {
		COEFF_D64_U8(y)

		    tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		for (i = 0; i < width; i++) {
			fpos = (x >> FILTER_SHIFT) & FILTER_MASK;
			xf0 = *((__m64 *) ((mlib_u8 *)filter1 + fpos));
			xf1 = _mm_unpacklo_pi16(xf0, xf0);
			xf2 = _mm_unpackhi_pi16(xf0, xf0);
			srcPixelPtr = tsp + 2 * (x >> MLIB_SHIFT);
			x += dx;

			a0 = mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr), yf0);
			a1 = mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr +
			    src_stride), yf1);
			a2 = mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr +
			    2 * src_stride), yf2);
			a3 = mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr +
			    3 * src_stride), yf3);
			b0 = mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr + 4), yf0);
			b1 = mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr +
			    src_stride + 4), yf1);
			b2 = mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr +
			    2 * src_stride + 4), yf2);
			b3 = mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr +
			    3 * src_stride + 4), yf3);
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
			((mlib_s16 *)dp)[i] = mmx_fpack16(a0);
		}

		y += dy;
		dp += dst_stride;
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_m_ImageZoom_U8_3_Bicubic(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	mlib_u8 *tsp, *srcPixelPtr, *tdp;
	mlib_s32 fpos, tail;
	__m64 *dfPtr;
	__m64 xf0, xf1, xf2, xf3;
	__m64 yf0, yf1, yf2, yf3;
	__m64 a0, a1, a2, a3, b0, b1, b2, b3;
	__m64 c0, c1, c2, c3, d0, d1, d2, d3;

	sp -= src_stride + 3;

	for (j = 0; j < height; j++) {
		COEFF_D64_U8(y)

		    tdp = dp;
		tail = dp[3 * width];
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		for (i = 0; i < width; i++) {
			COEFF_D64_U8(x)
			    srcPixelPtr = tsp + 3 * (x >> MLIB_SHIFT);
			x += dx;

			a0 = mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr), yf0);
			a1 = mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr + 3), yf0);
			a2 = mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr + 6), yf0);
			a3 = mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr + 9), yf0);
			b0 = mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr +
			    src_stride), yf1);
			b1 = mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr +
			    src_stride + 3), yf1);
			b2 = mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr +
			    src_stride + 6), yf1);
			b3 = mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr +
			    src_stride + 9), yf1);
			c0 = mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr +
			    2 * src_stride), yf2);
			c1 = mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr +
			    2 * src_stride + 3), yf2);
			c2 = mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr +
			    2 * src_stride + 6), yf2);
			c3 = mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr +
			    2 * src_stride + 9), yf2);
			d0 = mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr +
			    3 * src_stride), yf3);
			d1 = mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr +
			    3 * src_stride + 3), yf3);
			d2 = mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr +
			    3 * src_stride + 6), yf3);
			d3 = mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr +
			    3 * src_stride + 9), yf3);
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

			*(mlib_s32 *)tdp = mmx_fpack16(a0);
			tdp += 3;
		}

		dp[3 * width] = tail;

		y += dy;
		dp += dst_stride;
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_m_ImageZoom_U8_4_Bicubic(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	mlib_u8 *tsp, *srcPixelPtr;
	mlib_s32 fpos;
	__m64 *dfPtr;
	__m64 xf0, xf1, xf2, xf3;
	__m64 yf0, yf1, yf2, yf3;
	__m64 a0, a1, a2, a3, b0, b1, b2, b3;
	__m64 c0, c1, c2, c3, d0, d1, d2, d3;

	sp -= src_stride + 4;

	for (j = 0; j < height; j++) {
		COEFF_D64_U8(y)

		    tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		for (i = 0; i < width; i++) {
			COEFF_D64_U8(x)
			    srcPixelPtr = tsp + 4 * (x >> MLIB_SHIFT);
			x += dx;

			a1 = *(__m64 *) (srcPixelPtr);
			a3 = *(__m64 *) (srcPixelPtr + 8);
			a0 = mmx_fmul8x16hi(a1, yf0);
			a1 = mmx_fmul8x16lo(a1, yf0);
			a2 = mmx_fmul8x16hi(a3, yf0);
			a3 = mmx_fmul8x16lo(a3, yf0);
			b1 = *(__m64 *) (srcPixelPtr + src_stride);
			b3 = *(__m64 *) (srcPixelPtr + src_stride + 8);
			b0 = mmx_fmul8x16hi(b1, yf1);
			b1 = mmx_fmul8x16lo(b1, yf1);
			b2 = mmx_fmul8x16hi(b3, yf1);
			b3 = mmx_fmul8x16lo(b3, yf1);
			c1 = *(__m64 *) (srcPixelPtr + 2 * src_stride);
			c3 = *(__m64 *) (srcPixelPtr + 2 * src_stride + 8);
			c0 = mmx_fmul8x16hi(c1, yf2);
			c1 = mmx_fmul8x16lo(c1, yf2);
			c2 = mmx_fmul8x16hi(c3, yf2);
			c3 = mmx_fmul8x16lo(c3, yf2);
			d1 = *(__m64 *) (srcPixelPtr + 3 * src_stride);
			d3 = *(__m64 *) (srcPixelPtr + 3 * src_stride + 8);
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

			((mlib_s32 *)dp)[i] = mmx_fpack16(a0);
		}

		y += dy;
		dp += dst_stride;
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
