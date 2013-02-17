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

#pragma ident	"@(#)mlib_m_ImageZoom_BC_S16.c	9.2	07/11/05 SMI"

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

#include <stdlib.h>
#include <mlib_image.h>
#include <mlib_m_ImageFilters.h>
#include <mlib_mmx_utils.h>
#include <mlib_ImageZoom.h>

/* *********************************************************** */

#define	FILTER_1	filter1
#define	FILTER_3	filter3
#define	FILTER_4	filter4

/* *********************************************************** */

#define	VARIABLE(FORMAT)                                            \
	FORMAT *sp = GetElemSubStruct(current, sp), *dp =           \
	GetElemSubStruct(current, dp);                              \
	mlib_u8 *filter1 = GetElemStruct(filter1);                  \
	mlib_u8 *filter3 = GetElemStruct(filter3);                  \
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

#define	COEFF_D64_S16(r)                                        \
	fpos = (r >> MLIB_XY_SHIFT) & MLIB_XY_MASK;             \
	dfPtr = (__m64 *) ((mlib_u8 *)FILTER_4 + fpos * 4);     \
	d##r##f0 = dfPtr[0];                                    \
	d##r##f1 = dfPtr[1];                                    \
	d##r##f2 = dfPtr[2];                                    \
	d##r##f3 = dfPtr[3];

/* *********************************************************** */

#define	MLIB_XY_SHIFT	4
#define	MLIB_XY_MASK	(((1 << 9) - 1) << 3)

/* *********************************************************** */

#define	ROUND_VALUE	8
/* #define ADD_ROUND(x) x = _mm_add_pi16(x, _rnd); */
#define	ADD_ROUND(x)

/* *********************************************************** */

#undef  MMX_SCALE
#define	MMX_SCALE	10

/* *********************************************************** */

mlib_status
mlib_m_ImageZoom_S16_1_1_Bicubic(
    mlib_work_image * param)
{
	VARIABLE(mlib_u16)
	mlib_u16 *tsp, *tt0, *tt1;
	__m64 *tdp, tail;
	mlib_s32 fpos;
	__m64 *dfPtr, dyf0, dyf1, dyf2, dyf3, dxx0, dxx1;
	__m64 *buff, *buff_arr[8], *buff0, *buff1, *buff2, *buff3, *buffx;
	mlib_s32 *pbuff;
	mlib_s32 width2, width4, k, bind = 0, y_step = 4;

	width2 = (width + 1) / 2;
	width4 = (width + 3) / 4;

	buff = mlib_malloc(width4 * 9 * sizeof (__m64));
	if (buff == NULL)
		return (MLIB_FAILURE);
	for (k = 0; k < 4; k++)
		buff_arr[k] = buff + k * width4;
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k];
	buffx = buff + 4 * width4;

	x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	for (i = 0; i < width2; i++) {
		mlib_u16 *p_f0, *p_f1;

		fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
		p_f0 = (mlib_u16 *)((mlib_u8 *)FILTER_1 + fpos);
		x += dx;
		fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
		p_f1 = (mlib_u16 *)((mlib_u8 *)FILTER_1 + fpos);
		x += dx;

		((mlib_s32 *)buffx)[4 * i] = p_f0[0] | (p_f1[0] << 16);
		((mlib_s32 *)buffx)[4 * i + 1] = p_f0[1] | (p_f1[1] << 16);
		((mlib_s32 *)buffx)[4 * i + 2] = p_f0[2] | (p_f1[2] << 16);
		((mlib_s32 *)buffx)[4 * i + 3] = p_f0[3] | (p_f1[3] << 16);
	}

	sp -= src_stride + 1;
	tsp = sp;

	for (j = 0; j < height; j++) {
		COEFF_D64_S16(y)

		    if (y_step >= 4) {
			y_step = 4;
			tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		} else {
			bind += y_step;
			if (bind >= 4)
				bind -= 4;
		}

		for (k = 4 - y_step; k < 4; k++) {
			__m64 s0, s1, r0, r1, r2;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			pbuff = (mlib_s32 *)buff_arr[bind + k];

			for (i = 0; i < width2; i++) {

				tt0 = tsp + (x >> MLIB_SHIFT);
				x += dx;
				tt1 = tsp + (x >> MLIB_SHIFT);
				x += dx;

				s0 = MMX_MERGE4_U16(tt0[0], tt1[0], tt0[1],
				    tt1[1]);
				s1 = MMX_MERGE4_U16(tt0[2], tt1[2], tt0[3],
				    tt1[3]);

				dxx0 = buffx[2 * i];
				dxx1 = buffx[2 * i + 1];

				r0 = _mm_mulhi_pi16(s0, dxx0);
				r1 = _mm_mulhi_pi16(s1, dxx1);

				r2 = _mm_add_pi16(r0, r1);
				pbuff[i] =
				    mmx_fpadd16s(mmx_read_hi(r2),
				    mmx_read_lo(r2));
			}

			tsp += src_stride;
		}

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];

		tdp = (__m64 *) dp;
		tail = tdp[width4 - 1];

		for (i = 0; i < width4; i++) {
			__m64 s0, s1, s2, s3, r0, r1, r2, r3, r4, r5, r6, r7,
			    r8;
			s0 = buff0[i];
			s1 = buff1[i];
			s2 = buff2[i];
			s3 = buff3[i];

			r0 = _mm_mulhi_pi16(s0, dyf0);
			r1 = _mm_mulhi_pi16(s1, dyf1);
			r2 = _mm_mulhi_pi16(s2, dyf2);
			r3 = _mm_mulhi_pi16(s3, dyf3);

			r4 = _mm_add_pi16(r0, r1);
			r5 = _mm_add_pi16(r2, r3);
			r6 = _mm_add_pi16(r4, r5);

			ADD_ROUND(r6);
			r7 = _mm_unpacklo_pi16(_m_zero, r6);
			r8 = _mm_unpackhi_pi16(_m_zero, r6);
			r7 = _mm_srai_pi32(r7, 14);
			r8 = _mm_srai_pi32(r8, 14);

			tdp[i] = _mm_packs_pi32(r7, r8);
		}

		if (width & 3) {
			__m64 mask =
			    *(__m64 *) (mlib_mask64_arr + 2 * (width & 3));

			tdp[width4 - 1] =
			    _mm_or_si64(_mm_and_si64(mask, tdp[width4 - 1]),
			    _mm_andnot_si64(mask, tail));
		}

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		y += dy;
		dp += dst_stride;
	}

	_mm_empty();

	mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_m_ImageZoom_S16_2_1_Bicubic(
    mlib_work_image * param)
{
	VARIABLE(mlib_u16)
	mlib_u16 *tsp;
	__m64 *tdp, tail;
	mlib_s32 *prow, *tt0, *tt1;
	mlib_s32 fpos;
	__m64 *dfPtr,
	    dyf0, dyf1, dyf2, dyf3,
	    dxf0, dxf1, dxf2, dxf3, dxx0, dxx1, dxx2, dxx3, mask_ffff;
	__m64 *buff, *buffd, *buff_arr[8], *buff0, *buff1, *buff2, *buff3,
	    *buffx;
	__m64 *pbuff;
	mlib_s32 width2 = (width + 1) / 2, row_size, k, bind = 0, y_step = 4;

	row_size = 4 * GetElemStruct(width);
	buff = mlib_malloc(width2 * 9 * sizeof (__m64) + row_size);
	if (buff == NULL)
		return (MLIB_FAILURE);
	for (k = 0; k < 4; k++)
		buff_arr[k] = buff + k * width2;
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k];
	buffx = buff + 4 * width2;
	buffd = buff + 8 * width2;

	mask_ffff = mmx_freg_pair(0, 0xFFFFFFFF);

	x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	for (i = 0; i < width2; i++) {
		mlib_s32 xx;
		__m64 dxxf0, dxxf1, dxxf2, dxxf3;

		COEFF_D64_S16(x);
		xx = x + dx;
		COEFF_D64_S16(xx);

		buffx[4 * i] =
		    _mm_or_si64(_mm_andnot_si64(mask_ffff, dxf0),
		    _mm_and_si64(mask_ffff, dxxf0));
		buffx[4 * i + 1] =
		    _mm_or_si64(_mm_andnot_si64(mask_ffff, dxf1),
		    _mm_and_si64(mask_ffff, dxxf1));
		buffx[4 * i + 2] =
		    _mm_or_si64(_mm_andnot_si64(mask_ffff, dxf2),
		    _mm_and_si64(mask_ffff, dxxf2));
		buffx[4 * i + 3] =
		    _mm_or_si64(_mm_andnot_si64(mask_ffff, dxf3),
		    _mm_and_si64(mask_ffff, dxxf3));

		x += 2 * dx;
	}

	sp -= src_stride + 2;
	tsp = sp;

	for (j = 0; j < height; j++) {
		COEFF_D64_S16(y)

		    if (y_step >= 4) {
			y_step = 4;
			tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		} else {
			bind += y_step;
			if (bind >= 4)
				bind -= 4;
		}

		for (k = 4 - y_step; k < 3; k++) {
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			pbuff = buff_arr[bind + k];
			prow = (mlib_s32 *)tsp;

			for (i = 0; i < width2; i++) {
				mlib_s32 a0, a1, a2, a3, b0, b1, b2, b3;
				__m64 c0, c1, c2, c3, d0, d1, d2, d3, d4, d5,
				    d6;

				tt0 = prow + (x >> MLIB_SHIFT);
				x += dx;
				tt1 = prow + (x >> MLIB_SHIFT);
				x += dx;

				a0 = tt0[0];
				b0 = tt1[0];
				a1 = tt0[1];
				b1 = tt1[1];
				a2 = tt0[2];
				b2 = tt1[2];
				a3 = tt0[3];
				b3 = tt1[3];

				dxx0 = buffx[4 * i];
				dxx1 = buffx[4 * i + 1];
				dxx2 = buffx[4 * i + 2];
				dxx3 = buffx[4 * i + 3];

				c0 = mmx_freg_pair(a0, b0);
				c1 = mmx_freg_pair(a1, b1);
				c2 = mmx_freg_pair(a2, b2);
				c3 = mmx_freg_pair(a3, b3);

				d0 = _mm_mulhi_pi16(c0, dxx0);
				d1 = _mm_mulhi_pi16(c1, dxx1);
				d2 = _mm_mulhi_pi16(c2, dxx2);
				d3 = _mm_mulhi_pi16(c3, dxx3);

				d4 = _mm_add_pi16(d0, d1);
				d5 = _mm_add_pi16(d2, d3);
				d6 = _mm_add_pi16(d4, d5);

				pbuff[i] = d6;
			}

			tsp += src_stride;
		}

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];

		tdp = (__m64 *) dp;
		tail = tdp[width2 - 1];

		if (k < 4) {
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			prow = (mlib_s32 *)tsp;

			for (i = 0; i < width2; i++) {
				mlib_s32 a0, a1, a2, a3, b0, b1, b2, b3;
				__m64 c0, c1, c2, c3, d0, d1, d2, d3, d4, d5;
				__m64 s0, s1, s2, s3, r0, r1, r2, r3, r4, r5,
				    r6, r7, r8;

				tt0 = prow + (x >> MLIB_SHIFT);
				x += dx;
				tt1 = prow + (x >> MLIB_SHIFT);
				x += dx;

				a0 = tt0[0];
				b0 = tt1[0];
				a1 = tt0[1];
				b1 = tt1[1];
				a2 = tt0[2];
				b2 = tt1[2];
				a3 = tt0[3];
				b3 = tt1[3];

				dxx0 = buffx[4 * i];
				dxx1 = buffx[4 * i + 1];
				dxx2 = buffx[4 * i + 2];
				dxx3 = buffx[4 * i + 3];

				s0 = buff0[i];
				s1 = buff1[i];
				s2 = buff2[i];

				c0 = mmx_freg_pair(a0, b0);
				c1 = mmx_freg_pair(a1, b1);
				c2 = mmx_freg_pair(a2, b2);
				c3 = mmx_freg_pair(a3, b3);

				d0 = _mm_mulhi_pi16(c0, dxx0);
				d1 = _mm_mulhi_pi16(c1, dxx1);
				d2 = _mm_mulhi_pi16(c2, dxx2);
				d3 = _mm_mulhi_pi16(c3, dxx3);

				d4 = _mm_add_pi16(d0, d1);
				d5 = _mm_add_pi16(d2, d3);
				s3 = _mm_add_pi16(d4, d5);
				buff3[i] = s3;

				r0 = _mm_mulhi_pi16(s0, dyf0);
				r1 = _mm_mulhi_pi16(s1, dyf1);
				r2 = _mm_mulhi_pi16(s2, dyf2);
				r3 = _mm_mulhi_pi16(s3, dyf3);

				r4 = _mm_add_pi16(r0, r1);
				r5 = _mm_add_pi16(r2, r3);
				r6 = _mm_add_pi16(r4, r5);

				ADD_ROUND(r6);
				r7 = _mm_unpacklo_pi16(_m_zero, r6);
				r8 = _mm_unpackhi_pi16(_m_zero, r6);
				r7 = _mm_srai_pi32(r7, 14);
				r8 = _mm_srai_pi32(r8, 14);

				tdp[i] = _mm_packs_pi32(r7, r8);
			}

			tsp += src_stride;

		} else {
			for (i = 0; i < width2; i++) {
				__m64 s0, s1, s2, s3, r0, r1, r2, r3, r4, r5,
				    r6, r7, r8;
				s0 = buff0[i];
				s1 = buff1[i];
				s2 = buff2[i];
				s3 = buff3[i];

				r0 = _mm_mulhi_pi16(s0, dyf0);
				r1 = _mm_mulhi_pi16(s1, dyf1);
				r2 = _mm_mulhi_pi16(s2, dyf2);
				r3 = _mm_mulhi_pi16(s3, dyf3);

				r4 = _mm_add_pi16(r0, r1);
				r5 = _mm_add_pi16(r2, r3);
				r6 = _mm_add_pi16(r4, r5);

				ADD_ROUND(r6);
				r7 = _mm_unpacklo_pi16(_m_zero, r6);
				r8 = _mm_unpackhi_pi16(_m_zero, r6);
				r7 = _mm_srai_pi32(r7, 14);
				r8 = _mm_srai_pi32(r8, 14);

				tdp[i] = _mm_packs_pi32(r7, r8);
			}
		}

		if (width & 1) {
			__m64 mask = *(__m64 *) (mlib_mask64_arr + 4);

			tdp[width2 - 1] =
			    _mm_or_si64(_mm_and_si64(mask, tdp[width2 - 1]),
			    _mm_andnot_si64(mask, tail));
		}

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		y += dy;
		dp += dst_stride;
	}

	_mm_empty();

	mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_m_ImageZoom_S16_4_1_Bicubic(
    mlib_work_image * param)
{
	VARIABLE(mlib_u16)
	mlib_u16 *tsp;
	__m64 *prow, *tdp;
	mlib_s32 fpos;
	__m64 *dfPtr, dyf0, dyf1, dyf2, dyf3, dxf0, dxf1, dxf2, dxf3;
	__m64 *buff, *buffd, *buff_arr[8], *buff0, *buff1, *buff2, *buff3,
	    *pbuff;
	mlib_s32 row_size, k, bind = 0, y_step = 4;

	row_size = 8 * GetElemStruct(width);
	buff = mlib_malloc(width * 5 * sizeof (__m64) + row_size);
	if (buff == NULL)
		return (MLIB_FAILURE);
	for (k = 0; k < 4; k++)
		buff_arr[k] = buff + k * width;
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k];
	buffd = buff + 4 * width;

	sp -= src_stride + 4;
	tsp = sp;

	for (j = 0; j < height; j++) {
		fpos = (y >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
		dfPtr = (__m64 *) ((mlib_u8 *)FILTER_4 + fpos * 4);
		dyf0 = dfPtr[0];
		dyf1 = dfPtr[1];
		dyf2 = dfPtr[2];
		dyf3 = dfPtr[3];

		if (y_step >= 4) {
			y_step = 4;
			tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		} else {
			bind += y_step;
			if (bind >= 4)
				bind -= 4;
		}

		for (k = 4 - y_step; k < 3; k++) {
			__m64 s0, s1, s2, s3;
			__m64 r0, r1, r2, r3, r4, r5, r6;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			pbuff = buff_arr[bind + k];
			prow = (__m64 *) tsp;

			for (i = 0; i < width; i++) {
				cx = x >> MLIB_SHIFT;
				fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
				dfPtr =
				    (__m64 *) ((mlib_u8 *)FILTER_4 + fpos * 4);
				dxf0 = dfPtr[0];
				dxf1 = dfPtr[1];
				dxf2 = dfPtr[2];
				dxf3 = dfPtr[3];

				s0 = prow[cx];
				s1 = prow[cx + 1];
				s2 = prow[cx + 2];
				s3 = prow[cx + 3];

				r0 = _mm_mulhi_pi16(s0, dxf0);
				r1 = _mm_mulhi_pi16(s1, dxf1);
				r2 = _mm_mulhi_pi16(s2, dxf2);
				r3 = _mm_mulhi_pi16(s3, dxf3);

				r4 = _mm_add_pi16(r0, r1);
				r5 = _mm_add_pi16(r2, r3);
				r6 = _mm_add_pi16(r4, r5);
				pbuff[i] = r6;

				x += dx;
			}

			tsp += src_stride;
		}

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];

		tdp = (__m64 *) dp;

		if (k < 4) {
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			prow = (__m64 *) tsp;

			for (i = 0; i < width; i++) {
				__m64 a0, a1, a2, a3, b0, b1, b2, b3, b4, b5;
				__m64 s0, s1, s2, s3, r0, r1, r2, r3, r4, r5,
				    r6, r7, r8, r9;

				cx = x >> MLIB_SHIFT;
				fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
				dfPtr =
				    (__m64 *) ((mlib_u8 *)FILTER_4 + fpos * 4);
				dxf0 = dfPtr[0];
				dxf1 = dfPtr[1];
				dxf2 = dfPtr[2];
				dxf3 = dfPtr[3];

				a0 = prow[cx];
				a1 = prow[cx + 1];
				a2 = prow[cx + 2];
				a3 = prow[cx + 3];

				s0 = buff0[i];
				s1 = buff1[i];
				s2 = buff2[i];

				b0 = _mm_mulhi_pi16(a0, dxf0);
				b1 = _mm_mulhi_pi16(a1, dxf1);
				b2 = _mm_mulhi_pi16(a2, dxf2);
				b3 = _mm_mulhi_pi16(a3, dxf3);

				b4 = _mm_add_pi16(b0, b1);
				b5 = _mm_add_pi16(b2, b3);
				s3 = _mm_add_pi16(b4, b5);

				r0 = _mm_mulhi_pi16(s0, dyf0);
				r1 = _mm_mulhi_pi16(s1, dyf1);
				r2 = _mm_mulhi_pi16(s2, dyf2);
				r3 = _mm_mulhi_pi16(s3, dyf3);

				r4 = _mm_add_pi16(r0, r1);
				r5 = _mm_add_pi16(r2, r3);
				r6 = _mm_add_pi16(r4, r5);

				ADD_ROUND(r6);
				r7 = _mm_unpacklo_pi16(_m_zero, r6);
				r8 = _mm_unpackhi_pi16(_m_zero, r6);
				r7 = _mm_srai_pi32(r7, 14);
				r8 = _mm_srai_pi32(r8, 14);
				r9 = _mm_packs_pi32(r7, r8);

				buff3[i] = s3;
				tdp[i] = r9;
				x += dx;
			}

			tsp += src_stride;

		} else {

			for (i = 0; i < width; i++) {
				__m64 s0, s1, s2, s3, r0, r1, r2, r3, r4, r5,
				    r6, r7, r8;
				s0 = buff0[i];
				s1 = buff1[i];
				s2 = buff2[i];
				s3 = buff3[i];

				r0 = _mm_mulhi_pi16(s0, dyf0);
				r1 = _mm_mulhi_pi16(s1, dyf1);
				r2 = _mm_mulhi_pi16(s2, dyf2);
				r3 = _mm_mulhi_pi16(s3, dyf3);

				r4 = _mm_add_pi16(r0, r1);
				r5 = _mm_add_pi16(r2, r3);
				r6 = _mm_add_pi16(r4, r5);

				ADD_ROUND(r6);
				r7 = _mm_unpacklo_pi16(_m_zero, r6);
				r8 = _mm_unpackhi_pi16(_m_zero, r6);
				r7 = _mm_srai_pi32(r7, 14);
				r8 = _mm_srai_pi32(r8, 14);

				tdp[i] = _mm_packs_pi32(r7, r8);
			}
		}

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		y += dy;
		dp += dst_stride;
	}

	_mm_empty();

	mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_m_ImageZoom_S16_3_1_Bicubic(
    mlib_work_image * param)
{
	VARIABLE(mlib_u16)
	mlib_u16 *tsp, *tdp, tail;
	mlib_s32 fpos;
	__m64 *dfPtr, dyf0, dyf1, dyf2, dyf3, dxf0, dxf1, dxf2, dxf3;
	__m64 *buff, *buffd, *buff_arr[8], *buff0, *buff1, *buff2, *buff3,
	    *pbuff;
	mlib_s32 src_width, row_size, k, bind = 0, y_step = 4;

	src_width = GetElemStruct(width);
	row_size = 8 * src_width;

	buff = mlib_malloc(width * 5 * sizeof (__m64) + row_size);
	if (buff == NULL)
		return (MLIB_FAILURE);
	for (k = 0; k < 4; k++)
		buff_arr[k] = buff + k * width;
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k];
	buffd = buff + 4 * width;

	sp -= src_stride + 3;
	tsp = sp;

	for (j = 0; j < height; j++) {
		fpos = (y >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
		dfPtr = (__m64 *) ((mlib_u8 *)FILTER_4 + fpos * 4);
		dyf0 = dfPtr[0];
		dyf1 = dfPtr[1];
		dyf2 = dfPtr[2];
		dyf3 = dfPtr[3];

		if (y_step >= 4) {
			y_step = 4;
			tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		} else {
			bind += y_step;
			if (bind >= 4)
				bind -= 4;
		}

		for (k = 4 - y_step; k < 3; k++) {
			__m64 s0, s1, s2, s3;
			__m64 r0, r1, r2, r3, r4, r5, r6;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			pbuff = buff_arr[bind + k];

			for (i = 0; i < width; i++) {
				cx = x >> MLIB_SHIFT;
				cx += 2 * cx;
				fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
				dfPtr =
				    (__m64 *) ((mlib_u8 *)FILTER_4 + fpos * 4);
				dxf0 = dfPtr[0];
				dxf1 = dfPtr[1];
				dxf2 = dfPtr[2];
				dxf3 = dfPtr[3];

				s0 = *(__m64 *) (tsp + cx);
				s1 = *(__m64 *) (tsp + cx + 3);
				s2 = *(__m64 *) (tsp + cx + 6);
				s3 = *(__m64 *) (tsp + cx + 9);

				r0 = _mm_mulhi_pi16(s0, dxf0);
				r1 = _mm_mulhi_pi16(s1, dxf1);
				r2 = _mm_mulhi_pi16(s2, dxf2);
				r3 = _mm_mulhi_pi16(s3, dxf3);

				r4 = _mm_add_pi16(r0, r1);
				r5 = _mm_add_pi16(r2, r3);
				r6 = _mm_add_pi16(r4, r5);
				pbuff[i] = r6;

				x += dx;
			}

			tsp += src_stride;
		}

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];

		tdp = dp;
		tail = dp[3 * width];

		if (k < 4) {
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			for (i = 0; i < width; i++) {
				__m64 a0, a1, a2, a3, b0, b1, b2, b3, b4, b5;
				__m64 s0, s1, s2, s3, r0, r1, r2, r3, r4, r5,
				    r6, r7, r8;

				cx = x >> MLIB_SHIFT;
				cx += 2 * cx;
				fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
				dfPtr =
				    (__m64 *) ((mlib_u8 *)FILTER_4 + fpos * 4);
				dxf0 = dfPtr[0];
				dxf1 = dfPtr[1];
				dxf2 = dfPtr[2];
				dxf3 = dfPtr[3];

				a0 = *(__m64 *) (tsp + cx);
				a1 = *(__m64 *) (tsp + cx + 3);
				a2 = *(__m64 *) (tsp + cx + 6);
				a3 = *(__m64 *) (tsp + cx + 9);

				s0 = buff0[i];
				s1 = buff1[i];
				s2 = buff2[i];

				b0 = _mm_mulhi_pi16(a0, dxf0);
				b1 = _mm_mulhi_pi16(a1, dxf1);
				b2 = _mm_mulhi_pi16(a2, dxf2);
				b3 = _mm_mulhi_pi16(a3, dxf3);

				b4 = _mm_add_pi16(b0, b1);
				b5 = _mm_add_pi16(b2, b3);
				s3 = _mm_add_pi16(b4, b5);
				buff3[i] = s3;

				r0 = _mm_mulhi_pi16(s0, dyf0);
				r1 = _mm_mulhi_pi16(s1, dyf1);
				r2 = _mm_mulhi_pi16(s2, dyf2);
				r3 = _mm_mulhi_pi16(s3, dyf3);

				r4 = _mm_add_pi16(r0, r1);
				r5 = _mm_add_pi16(r2, r3);
				r6 = _mm_add_pi16(r4, r5);

				ADD_ROUND(r6);
				r7 = _mm_unpacklo_pi16(_m_zero, r6);
				r8 = _mm_unpackhi_pi16(_m_zero, r6);
				r7 = _mm_srai_pi32(r7, 14);
				r8 = _mm_srai_pi32(r8, 14);

				*(__m64 *) tdp = _mm_packs_pi32(r7, r8);
				tdp += 3;
				x += dx;
			}

			tsp += src_stride;

		} else {

			for (i = 0; i < width; i++) {
				__m64 s0, s1, s2, s3, r0, r1, r2, r3, r4, r5,
				    r6, r7, r8;
				s0 = buff0[i];
				s1 = buff1[i];
				s2 = buff2[i];
				s3 = buff3[i];

				r0 = _mm_mulhi_pi16(s0, dyf0);
				r1 = _mm_mulhi_pi16(s1, dyf1);
				r2 = _mm_mulhi_pi16(s2, dyf2);
				r3 = _mm_mulhi_pi16(s3, dyf3);

				r4 = _mm_add_pi16(r0, r1);
				r5 = _mm_add_pi16(r2, r3);
				r6 = _mm_add_pi16(r4, r5);

				ADD_ROUND(r6);
				r7 = _mm_unpacklo_pi16(_m_zero, r6);
				r8 = _mm_unpackhi_pi16(_m_zero, r6);
				r7 = _mm_srai_pi32(r7, 14);
				r8 = _mm_srai_pi32(r8, 14);

				*(__m64 *) tdp = _mm_packs_pi32(r7, r8);
				tdp += 3;
			}
		}

		tdp[0] = tail;

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		y += dy;
		dp += dst_stride;
	}

	_mm_empty();

	mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
