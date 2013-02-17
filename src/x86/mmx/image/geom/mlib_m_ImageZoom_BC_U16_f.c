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

#pragma ident	"@(#)mlib_m_ImageZoom_BC_U16_f.c	9.2	07/11/05 SMI"

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

#include <stdio.h>
#include <mlib_image.h>
#include <mlib_m_ImageFilters.h>
#include <mlib_mmx_utils.h>
#include <mlib_ImageZoom.h>

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
	    2 * GetElemStruct(src_stride), dst_stride =             \
	    2 * GetElemStruct(dst_stride), width =                  \
	GetElemSubStruct(current, width), height =                  \
	GetElemSubStruct(current, height);                          \
	    __m64 mask8000 = mmx_to_double_dup(0x80008000);

/* *********************************************************** */

#define	FILTER_SHIFT	4
#define	FILTER_MASK	(((1 << 9) - 1) << 3)

/* *********************************************************** */

#define	COEFF_D64_S16(r)                                        \
	fpos = (r >> FILTER_SHIFT) & FILTER_MASK;               \
	dfPtr = (__m64 *) ((mlib_u8 *)filter4 + fpos * 4);      \
	r##f0 = dfPtr[0];                                       \
	r##f1 = dfPtr[1];                                       \
	r##f2 = dfPtr[2];                                       \
	r##f3 = dfPtr[3];

/* *********************************************************** */

mlib_status
mlib_m_ImageZoom_U16_1_Bicubic(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	mlib_u8 *tsp, *srcPixelPtr;
	mlib_s32 fpos;
	__m64 *dfPtr;
	__m64 xf0, yf0, yf1, yf2, yf3;
	__m64 a0, a1, a2, a3;

	sp -= src_stride + 2 * 1;
	tsp = sp;

	for (j = 0; j < height; j++) {
		COEFF_D64_S16(y)

		    tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		for (i = 0; i < width; i++) {
			fpos = (x >> FILTER_SHIFT) & FILTER_MASK;
			xf0 = *(__m64 *) ((mlib_u8 *)filter1 + fpos);
			srcPixelPtr = tsp + 2 * (x >> MLIB_SHIFT);
			x += dx;

			a0 = *(__m64 *) (srcPixelPtr);
			a1 = *(__m64 *) ((mlib_u8 *)srcPixelPtr + src_stride);
			a2 = *(__m64 *) ((mlib_u8 *)srcPixelPtr +
			    2 * src_stride);
			a3 = *(__m64 *) ((mlib_u8 *)srcPixelPtr +
			    3 * src_stride);
			a0 = _mm_xor_si64(a0, mask8000);
			a1 = _mm_xor_si64(a1, mask8000);
			a2 = _mm_xor_si64(a2, mask8000);
			a3 = _mm_xor_si64(a3, mask8000);
			a0 = _mm_mulhi_pi16(a0, yf0);
			a1 = _mm_mulhi_pi16(a1, yf1);
			a2 = _mm_mulhi_pi16(a2, yf2);
			a3 = _mm_mulhi_pi16(a3, yf3);
			a0 = _mm_add_pi16(a0, a1);
			a2 = _mm_add_pi16(a2, a3);
			a0 = _mm_add_pi16(a0, a2);
			a0 = _mm_mulhi_pi16(a0, xf0);
			a0 = _mm_add_pi16(a0, _mm_srli_si64(a0, 32));
			a0 = _mm_add_pi16(a0, _mm_srli_si64(a0, 16));
			a0 = _mm_add_pi16(a0, a0);
			a0 = _mm_adds_pi16(a0, a0);
			((mlib_s16 *)dp)[i] = *(mlib_s16 *)&a0 ^ 0x8000;
		}

		y += dy;
		dp += dst_stride;
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_m_ImageZoom_U16_2_Bicubic(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	mlib_u8 *tsp, *srcPixelPtr;
	mlib_s32 fpos;
	__m64 *dfPtr;
	__m64 xf0, xf1, xf2;
	__m64 yf0, yf1, yf2, yf3;
	__m64 a0, a1, a2, a3, b0, b1, b2, b3;

	sp -= src_stride + 2 * 2;

	for (j = 0; j < height; j++) {
		COEFF_D64_S16(y)

		    tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		for (i = 0; i < width; i++) {
			fpos = (x >> FILTER_SHIFT) & FILTER_MASK;
			xf0 = *((__m64 *) ((mlib_u8 *)filter1 + fpos));
			xf1 = _mm_unpacklo_pi16(xf0, xf0);
			xf2 = _mm_unpackhi_pi16(xf0, xf0);
			srcPixelPtr = tsp + 4 * (x >> MLIB_SHIFT);
			x += dx;

			a0 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr)), yf0);
			a1 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr + src_stride)),
			    yf1);
			a2 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr +
			    2 * src_stride)), yf2);
			a3 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr +
			    3 * src_stride)), yf3);
			b0 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 8)), yf0);
			b1 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr + src_stride +
			    8)), yf1);
			b2 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr +
			    2 * src_stride + 8)), yf2);
			b3 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr +
			    3 * src_stride + 8)), yf3);
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
			((mlib_s32 *)dp)[i] = *(mlib_s32 *)&a0 ^ 0x80008000;
		}

		y += dy;
		dp += dst_stride;
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_m_ImageZoom_U16_3_Bicubic(
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

	sp -= src_stride + 2 * 3;

	for (j = 0; j < height; j++) {
		COEFF_D64_S16(y)

		    tdp = dp;
		tail = ((mlib_s16 *)dp)[3 * width];
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		for (i = 0; i < width; i++) {
			COEFF_D64_S16(x)
			    srcPixelPtr = tsp + 6 * (x >> MLIB_SHIFT);
			x += dx;

			a0 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr)), yf0);
			a1 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * 3)), yf0);
			a2 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * 6)), yf0);
			a3 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * 9)), yf0);
			b0 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr + src_stride)),
			    yf1);
			b1 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr + src_stride +
			    2 * 3)), yf1);
			b2 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr + src_stride +
			    2 * 6)), yf1);
			b3 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr + src_stride +
			    2 * 9)), yf1);
			c0 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr +
			    2 * src_stride)), yf2);
			c1 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr +
			    2 * src_stride + 2 * 3)), yf2);
			c2 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr +
			    2 * src_stride + 2 * 6)), yf2);
			c3 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr +
			    2 * src_stride + 2 * 9)), yf2);
			d0 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr +
			    3 * src_stride)), yf3);
			d1 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr +
			    3 * src_stride + 2 * 3)), yf3);
			d2 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr +
			    3 * src_stride + 2 * 6)), yf3);
			d3 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr +
			    3 * src_stride + 2 * 9)), yf3);
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

			*(__m64 *) tdp = _mm_xor_si64(mask8000, a0);
			tdp += 6;
		}

		((mlib_s16 *)dp)[3 * width] = tail;

		y += dy;
		dp += dst_stride;
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_m_ImageZoom_U16_4_Bicubic(
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

	sp -= src_stride + 2 * 4;

	for (j = 0; j < height; j++) {
		COEFF_D64_S16(y)

		    tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		for (i = 0; i < width; i++) {
			COEFF_D64_S16(x)
			    srcPixelPtr = tsp + 8 * (x >> MLIB_SHIFT);
			x += dx;

			a0 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr)), yf0);
			a1 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * 4)), yf0);
			a2 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * 8)), yf0);
			a3 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr + 2 * 12)), yf0);
			b0 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr + src_stride)),
			    yf1);
			b1 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr + src_stride +
			    2 * 4)), yf1);
			b2 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr + src_stride +
			    2 * 8)), yf1);
			b3 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr + src_stride +
			    2 * 12)), yf1);
			c0 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr +
			    2 * src_stride)), yf2);
			c1 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr +
			    2 * src_stride + 2 * 4)), yf2);
			c2 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr +
			    2 * src_stride + 2 * 8)), yf2);
			c3 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr +
			    2 * src_stride + 2 * 12)), yf2);
			d0 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr +
			    3 * src_stride)), yf3);
			d1 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr +
			    3 * src_stride + 2 * 4)), yf3);
			d2 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr +
			    3 * src_stride + 2 * 8)), yf3);
			d3 = _mm_mulhi_pi16(_mm_xor_si64(mask8000,
			    *(__m64 *) ((mlib_u8 *)srcPixelPtr +
			    3 * src_stride + 2 * 12)), yf3);
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
			((__m64 *) dp)[i] = _mm_xor_si64(mask8000, a0);
		}

		y += dy;
		dp += dst_stride;
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
