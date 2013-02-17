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

#pragma ident	"@(#)mlib_m_ImageZoom_BL_f.c	9.2	07/11/05 SMI"

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

#define	FILTER_1	filter1
#define	FILTER_3	filter3
#define	FILTER_4	filter4

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

#define	FILTER_SHIFT	5
#define	FILTER_MASK	(((1 << 8) - 1) << 3)

/* *********************************************************** */

#define	COEFF_D64_U8(r)                                         \
	fpos = (r >> FILTER_SHIFT) & FILTER_MASK;               \
	dfPtr = (__m64 *) ((mlib_u8 *)FILTER_4 + fpos * 4);     \
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
mlib_m_ImageZoom_U8_1s_Bilinear(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	mlib_u8 *tsp, *srcPixelPtr;
	mlib_u8 *filter_bl2 = (mlib_u8 *)mlib_filters_u8_bl + 8 * 256;
	mlib_s32 fpos, s0;
	__m64 *dfPtr, yf0, xf0, d0;

	for (j = 0; j < height; j++) {
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		fpos = (y >> FILTER_SHIFT) & FILTER_MASK;
		yf0 = *(__m64 *) (filter_bl2 + fpos);

		for (i = 0; i < width; i++) {
			fpos = (x >> FILTER_SHIFT) & FILTER_MASK;
			xf0 = *(__m64 *) ((mlib_u8 *)mlib_filters_u8_bl + fpos);
			srcPixelPtr = tsp + (x >> MLIB_SHIFT);
			x += dx;

			s0 = *(mlib_u16 *)srcPixelPtr | (*(mlib_u16
			    *)(srcPixelPtr + src_stride) << 16);
			xf0 = _mm_mulhi_pi16(xf0, yf0);
			d0 = mmx_fmul8x16(s0, xf0);
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
mlib_m_ImageZoom_U8_2s_Bilinear(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	mlib_u8 *tsp, *srcPixelPtr;
	mlib_u8 *filter_bl2 = (mlib_u8 *)mlib_filters_u8_bl + 8 * 256;
	mlib_s32 fpos;
	__m64 yf0, yf1, xf0, xyf0, xyf1, d0, d1;

	for (j = 0; j < height; j++) {
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		fpos = (y >> FILTER_SHIFT) & FILTER_MASK;
		yf0 = *(__m64 *) (filter_bl2 + fpos);
		yf1 = _mm_unpackhi_pi16(yf0, yf0);
		yf0 = _mm_unpacklo_pi16(yf0, yf0);

		for (i = 0; i < width; i++) {
			fpos = (x >> FILTER_SHIFT) & FILTER_MASK;
			xf0 = *(__m64 *) (filter_bl2 + fpos);
			srcPixelPtr = tsp + 2 * (x >> MLIB_SHIFT);
			x += dx;

			xyf0 = _mm_mulhi_pi16(xf0, yf0);
			xyf1 = _mm_mulhi_pi16(xf0, yf1);

			d0 = mmx_fmul8x16(*(mlib_s32 *)srcPixelPtr, xyf0);
			d1 = mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr +
			    src_stride), xyf1);
			d0 = _mm_add_pi16(d0, d1);
			d0 = _mm_add_pi16(d0, _mm_srli_si64(d0, 32));
			((mlib_s16 *)dp)[i] = mmx_fpack16(d0);
		}

		y += dy;
		dp += dst_stride;
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_m_ImageZoom_U8_3s_Bilinear(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	mlib_u8 *tsp, *srcPixelPtr, *tdp;
	mlib_u8 *filter_bl2 = (mlib_u8 *)mlib_filters_u8_bl + 8 * 256;
	mlib_s32 tail;
	mlib_s32 fpos;
	__m64 yf0, yf1, xf0, xf1, d0, d1;

	for (j = 0; j < height; j++) {
		tdp = dp;
		tail = dp[3 * width];

		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		fpos = (y >> FILTER_SHIFT) & FILTER_MASK;
		yf0 = *(__m64 *) (filter_bl2 + fpos);
		yf1 = _mm_unpackhi_pi16(yf0, yf0);
		yf0 = _mm_unpacklo_pi16(yf0, yf0);

		for (i = 0; i < width; i++) {
			fpos = (x >> FILTER_SHIFT) & FILTER_MASK;
			xf0 = *(__m64 *) (filter_bl2 + fpos);
			srcPixelPtr = tsp + 3 * (x >> MLIB_SHIFT);
			x += dx;

			xf1 = _mm_unpackhi_pi16(xf0, xf0);
			xf0 = _mm_unpacklo_pi16(xf0, xf0);

			d0 = _mm_add_pi16(mmx_fmul8x16(*(mlib_s32
			    *)(srcPixelPtr), xf0),
			    mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr + 3), xf1));
			d1 = _mm_add_pi16(mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr
			    + src_stride), xf0),
			    mmx_fmul8x16(*(mlib_s32 *)(srcPixelPtr +
			    src_stride + 3), xf1));
			d0 = _mm_mulhi_pi16(d0, yf0);
			d1 = _mm_mulhi_pi16(d1, yf1);
			d0 = _mm_add_pi16(d0, d1);
			*(mlib_s32 *)tdp = mmx_fpack16(d0);
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
mlib_m_ImageZoom_U8_4s_Bilinear(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	mlib_u8 *tsp, *srcPixelPtr;
	mlib_u8 *filter_bl2 = (mlib_u8 *)mlib_filters_u8_bl + 8 * 256;
	mlib_s32 fpos;
	__m64 yf0, yf1, xf0, xf1, d0, d1;

	for (j = 0; j < height; j++) {
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		fpos = (y >> FILTER_SHIFT) & FILTER_MASK;
		yf0 = *(__m64 *) (filter_bl2 + fpos);
		yf1 = _mm_unpackhi_pi16(yf0, yf0);
		yf0 = _mm_unpacklo_pi16(yf0, yf0);

		for (i = 0; i < width; i++) {
			fpos = (x >> FILTER_SHIFT) & FILTER_MASK;
			xf0 = *(__m64 *) (filter_bl2 + fpos);
			srcPixelPtr = tsp + 4 * (x >> MLIB_SHIFT);
			x += dx;

			xf1 = _mm_unpackhi_pi16(xf0, xf0);
			xf0 = _mm_unpacklo_pi16(xf0, xf0);

			d0 = *(__m64 *) srcPixelPtr;
			d1 = *(__m64 *) (srcPixelPtr + src_stride);

			d0 = _mm_add_pi16(mmx_fmul8x16hi(d0, xf0),
			    mmx_fmul8x16lo(d0, xf1));
			d1 = _mm_add_pi16(mmx_fmul8x16hi(d1, xf0),
			    mmx_fmul8x16lo(d1, xf1));
			d0 = _mm_mulhi_pi16(d0, yf0);
			d1 = _mm_mulhi_pi16(d1, yf1);
			d0 = _mm_add_pi16(d0, d1);
			((mlib_s32 *)dp)[i] = mmx_fpack16(d0);
		}

		y += dy;
		dp += dst_stride;
	}

	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
