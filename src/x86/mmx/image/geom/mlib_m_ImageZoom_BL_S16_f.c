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

#pragma ident	"@(#)mlib_m_ImageZoom_BL_S16_f.c	9.2	07/11/05 SMI"

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

/* *********************************************************** */

#define	VARIABLE(FORMAT)                                            \
	FORMAT *sp = GetElemSubStruct(current, sp), *dp =           \
	GetElemSubStruct(current, dp);                              \
	mlib_d64 dzoomx = GetElemStruct(zoomx);                     \
	mlib_s32 i, j, cx, dx = GetElemStruct(DX), dy =             \
	GetElemStruct(DY), x =                                      \
	GetElemSubStruct(current, srcX) & MLIB_MASK, y =            \
	GetElemSubStruct(current, srcY) & MLIB_MASK, src_stride =   \
	    2 * GetElemStruct(src_stride), dst_stride =             \
	    2 * GetElemStruct(dst_stride), width =                  \
	GetElemSubStruct(current, width), height =                  \
	GetElemSubStruct(current, height);

/* *********************************************************** */

#define	INIT_DY()                                               \
	yo = ((y & 0xfffe) >> 1);                               \
	ddy = mmx_to_double_dup((yo << 16) | yo);               \
	yo = 0x7ffe - yo;                                       \
	rdy = mmx_to_double_dup((yo << 16) | yo)

/* *********************************************************** */

static void
mlib_empty_func()
{
}

/* *********************************************************** */

mlib_status
mlib_m_ImageZoom_S16_2s_Bilinear(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	__m64 *buffx;
	mlib_u8 *tsp, *srcPixelPtr;
	mlib_d64 cdx, zdx;
	mlib_s32 yo;
	__m64 rdy, ddy, ddx, d0, d1;

	buffx = mlib_malloc(width * 4 * sizeof (mlib_s16));
	if (buffx == NULL)
		return (MLIB_FAILURE);

/* calculate dx for all row */
	cdx = x * 0.5;
	zdx = (1 << 15) / dzoomx;
	for (i = 0; i < width; i++) {
		mlib_s32 ires;

		ires = ((mlib_s32)cdx) - ((x >> MLIB_SHIFT) << 15);
		if (ires >= (1 << 15))
			ires = (1 << 15) - 1;
		if (ires < 0)
			ires = 0;
		((mlib_s16 *)buffx)[4 * i] = 0x7ffe - ires;
		((mlib_s16 *)buffx)[4 * i + 1] = 0x7ffe - ires;
		((mlib_s16 *)buffx)[4 * i + 2] = ires;
		((mlib_s16 *)buffx)[4 * i + 3] = ires;

		cdx += zdx;
		x += dx;
		mlib_empty_func();
	}

	for (j = 0; j < height; j++) {
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		INIT_DY();

		for (i = 0; i < width; i++) {
			srcPixelPtr = tsp + 4 * (x >> MLIB_SHIFT);
			x += dx;

			ddx = buffx[i];

			d0 = *(__m64 *) (srcPixelPtr);
			d1 = *(__m64 *) (srcPixelPtr + src_stride);
			d0 = _mm_mulhi_pi16(d0, ddx);
			d1 = _mm_mulhi_pi16(d1, ddx);
			d0 = _mm_add_pi16(d0, d0);
			d1 = _mm_add_pi16(d1, d1);
			d0 = _mm_mulhi_pi16(d0, rdy);
			d1 = _mm_mulhi_pi16(d1, ddy);
			d0 = _mm_add_pi16(d0, d1);
			d0 = _mm_add_pi16(d0, d0);
			d0 = _mm_add_pi16(d0, _mm_srli_si64(d0, 32));
			((mlib_s32 *)dp)[i] = _m_to_int(d0);
		}

		y += dy;
		dp += dst_stride;
	}

	_mm_empty();

	mlib_free(buffx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_m_ImageZoom_S16_3s_Bilinear(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	__m64 *buffx;
	mlib_u8 *tsp, *srcPixelPtr, *tdp;
	mlib_d64 cdx, zdx;
	mlib_s32 yo, tail;
	__m64 rdy, ddy, rdx, ddx, d0, d1, d2, d3;

	buffx = mlib_malloc(width * 8 * sizeof (mlib_s16));
	if (buffx == NULL)
		return (MLIB_FAILURE);

/* calculate dx for all row */
	cdx = x * 0.5;
	zdx = (1 << 15) / dzoomx;
	for (i = 0; i < width; i++) {
		mlib_s32 ires;

		ires = ((mlib_s32)cdx) - ((x >> MLIB_SHIFT) << 15);
		if (ires >= (1 << 15))
			ires = (1 << 15) - 1;
		if (ires < 0)
			ires = 0;
		((mlib_s16 *)buffx)[8 * i] = 0x7ffe - ires;
		((mlib_s16 *)buffx)[8 * i + 1] = 0x7ffe - ires;
		((mlib_s16 *)buffx)[8 * i + 2] = 0x7ffe - ires;
		((mlib_s16 *)buffx)[8 * i + 3] = 0x7ffe - ires;
		((mlib_s16 *)buffx)[8 * i + 4] = ires;
		((mlib_s16 *)buffx)[8 * i + 5] = ires;
		((mlib_s16 *)buffx)[8 * i + 6] = ires;
		((mlib_s16 *)buffx)[8 * i + 7] = ires;

		cdx += zdx;
		x += dx;
		mlib_empty_func();
	}

	for (j = 0; j < height; j++) {
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		INIT_DY();

		tdp = dp;
		tail = ((mlib_s16 *)dp)[3 * width];

		for (i = 0; i < width; i++) {
			srcPixelPtr = tsp + 6 * (x >> MLIB_SHIFT);
			x += dx;

			rdx = buffx[2 * i];
			ddx = buffx[2 * i + 1];

			d0 = *(__m64 *) (srcPixelPtr);
			d1 = *(__m64 *) (srcPixelPtr + 6);
			d2 = *(__m64 *) (srcPixelPtr + src_stride);
			d3 = *(__m64 *) (srcPixelPtr + src_stride + 6);
			d0 = _mm_mulhi_pi16(d0, rdx);
			d1 = _mm_mulhi_pi16(d1, ddx);
			d2 = _mm_mulhi_pi16(d2, rdx);
			d3 = _mm_mulhi_pi16(d3, ddx);
			d0 = _mm_add_pi16(d0, d1);
			d2 = _mm_add_pi16(d2, d3);
			d0 = _mm_mulhi_pi16(d0, rdy);
			d2 = _mm_mulhi_pi16(d2, ddy);
			d0 = _mm_add_pi16(d0, d2);
			d0 = _mm_add_pi16(d0, d0);
			d0 = _mm_add_pi16(d0, d0);
			*(__m64 *) tdp = d0;
			tdp += 6;
		}

		((mlib_s16 *)dp)[3 * width] = tail;

		y += dy;
		dp += dst_stride;
	}

	_mm_empty();

	mlib_free(buffx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_m_ImageZoom_S16_4s_Bilinear(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	__m64 *buffx;
	mlib_u8 *tsp, *srcPixelPtr;
	mlib_d64 cdx, zdx;
	mlib_s32 yo;
	__m64 rdy, ddy, rdx, ddx, d0, d1, d2, d3;

	buffx = mlib_malloc(width * 8 * sizeof (mlib_s16));
	if (buffx == NULL)
		return (MLIB_FAILURE);

/* calculate dx for all row */
	cdx = x * 0.5;
	zdx = (1 << 15) / dzoomx;
	for (i = 0; i < width; i++) {
		mlib_s32 ires;

		ires = ((mlib_s32)cdx) - ((x >> MLIB_SHIFT) << 15);
		if (ires >= (1 << 15))
			ires = (1 << 15) - 1;
		if (ires < 0)
			ires = 0;
		((mlib_s16 *)buffx)[8 * i] = 0x7ffe - ires;
		((mlib_s16 *)buffx)[8 * i + 1] = 0x7ffe - ires;
		((mlib_s16 *)buffx)[8 * i + 2] = 0x7ffe - ires;
		((mlib_s16 *)buffx)[8 * i + 3] = 0x7ffe - ires;
		((mlib_s16 *)buffx)[8 * i + 4] = ires;
		((mlib_s16 *)buffx)[8 * i + 5] = ires;
		((mlib_s16 *)buffx)[8 * i + 6] = ires;
		((mlib_s16 *)buffx)[8 * i + 7] = ires;

		cdx += zdx;
		x += dx;
		mlib_empty_func();
	}

	for (j = 0; j < height; j++) {
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		INIT_DY();

		for (i = 0; i < width; i++) {
			srcPixelPtr = tsp + 8 * (x >> MLIB_SHIFT);
			x += dx;

			rdx = buffx[2 * i];
			ddx = buffx[2 * i + 1];

			d0 = *(__m64 *) (srcPixelPtr);
			d1 = *(__m64 *) (srcPixelPtr + 8);
			d2 = *(__m64 *) (srcPixelPtr + src_stride);
			d3 = *(__m64 *) (srcPixelPtr + src_stride + 8);
			d0 = _mm_mulhi_pi16(d0, rdx);
			d1 = _mm_mulhi_pi16(d1, ddx);
			d2 = _mm_mulhi_pi16(d2, rdx);
			d3 = _mm_mulhi_pi16(d3, ddx);
			d0 = _mm_add_pi16(d0, d1);
			d2 = _mm_add_pi16(d2, d3);
			d0 = _mm_mulhi_pi16(d0, rdy);
			d2 = _mm_mulhi_pi16(d2, ddy);
			d0 = _mm_add_pi16(d0, d2);
			d0 = _mm_add_pi16(d0, d0);
			d0 = _mm_add_pi16(d0, d0);
			((__m64 *) dp)[i] = d0;
		}

		y += dy;
		dp += dst_stride;
	}

	_mm_empty();

	mlib_free(buffx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
