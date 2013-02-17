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

#pragma ident	"@(#)mlib_m_ImageZoom_BC.c	9.2	07/11/05 SMI"

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

#define	USE_MODIF_8x16

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

#define	MLIB_XY_SHIFT	5
#define	MLIB_XY_MASK	(((1 << 8) - 1) << 3)

/* *********************************************************** */

#define	COEFF_D64_U8(r)                                         \
	fpos = (r >> MLIB_XY_SHIFT) & MLIB_XY_MASK;             \
	dfPtr = (__m64 *) ((mlib_u8 *)FILTER_4 + fpos * 4);     \
	d##r##f0 = dfPtr[0];                                    \
	d##r##f1 = dfPtr[1];                                    \
	d##r##f2 = dfPtr[2];                                    \
	d##r##f3 = dfPtr[3];

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
mlib_m_ImageZoom_U8_1_1_Bicubic(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	mlib_u8 *tsp, *tt0, *tt1;
	mlib_s32 *tdp, tail;
	mlib_s32 fpos;
	__m64 *dfPtr, dyf0, dyf1, dyf2, dyf3, dxx0, dxx1;
	__m64 *buff, *buffd, *buff_arr[8], *buff0, *buff1, *buff2, *buff3,
	    *buffx;
	mlib_s32 *pbuff;
	mlib_s32 width2, width4, k, bind = 0, y_step = 4;

	width2 = (width + 1) / 2;
	width4 = (width + 3) / 4;

	buff = mlib_malloc(width4 * 9 * sizeof (__m64));
	if (buff == NULL) {
		_mm_empty();
		return (MLIB_FAILURE);
	}
	for (k = 0; k < 4; k++)
		buff_arr[k] = buff + k * width4;
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k];
	buffx = buff + 4 * width4;
	buffd = buff + 8 * width4;

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
		COEFF_D64_U8(y)

		    tdp = (mlib_s32 *)dp;
		tail = tdp[width4 - 1];

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

				s0 = _m_from_int(*(mlib_s32 *)tt0);
				s1 = _m_from_int(*(mlib_s32 *)tt1);
				s0 = _mm_unpacklo_pi8(s0, s1);

				dxx0 = buffx[2 * i];
				dxx1 = buffx[2 * i + 1];

				r0 = mmx_fmul8x16hi(s0, dxx0);
				r1 = mmx_fmul8x16lo(s0, dxx1);
				r2 = _mm_add_pi16(r0, r1);

				pbuff[i] =
				    _m_to_int(_mm_add_pi16(_mm_srli_si64(r2,
				    32), r2));
			}

			tsp += src_stride;
		}

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];

		for (i = 0; i < width4; i++) {
			__m64 s0, s1, s2, s3, r0, r1, r2, r3, r4, r5, r6;

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

			tdp[i] = mmx_fpack16(r6);
		}

		if (width & 3) {
			mlib_s32 mask =
			    *(mlib_s32 *)(mlib_mask64_arr + (width & 3));

			tdp[width4 - 1] =
			    (tdp[width4 - 1] & mask) | (tail & ~mask);
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
mlib_m_ImageZoom_U8_2_1_Bicubic(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	mlib_u8 *tsp;
	mlib_u16 *prow, *tt;
	mlib_s32 *tdp, tail;
	mlib_s32 fpos;
	__m64 *dfPtr,
	    dyf0, dyf1, dyf2, dyf3,
	    dxf0, dxf1, dxf2, dxf3, dxx0, dxx1, mask_ffff;
	__m64 *buff, *buffd, *buff_arr[8], *buff0, *buff1, *buff2, *buff3,
	    *buffx;
	mlib_s32 *pbuff;
	mlib_s32 width2 = (width + 1) / 2, row_size, k, bind = 0, y_step = 4;

	row_size = 2 * GetElemStruct(width);
	buff = mlib_malloc(width2 * 9 * sizeof (__m64) + row_size);
	if (buff == NULL) {
		_mm_empty();
		return (MLIB_FAILURE);
	}
	for (k = 0; k < 4; k++)
		buff_arr[k] = buff + k * width2;
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k];
	buffx = buff + 4 * width2;
	buffd = buff + 8 * width2;

	mask_ffff = mmx_to_double(0, 0xFFFFFFFF);

	x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	for (i = 0; i < width; i++) {
		COEFF_D64_U8(x);
		COEFF_D64_U8_2;
		buffx[2 * i] = dxx0;
		buffx[2 * i + 1] = dxx1;
		x += dx;
	}

	sp -= src_stride + 2;
	tsp = sp;

	for (j = 0; j < height; j++) {
		COEFF_D64_U8(y)

		    tdp = (mlib_s32 *)dp;
		tail = tdp[width2 - 1];

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

			prow = (mlib_u16 *)tsp;

			for (i = 0; i < width; i++) {
				tt = prow + (x >> MLIB_SHIFT);
				x += dx;

				s0 = _m_from_int(tt[0] | (tt[1] << 16));
				s1 = _m_from_int(tt[2] | (tt[3] << 16));

				dxx0 = buffx[2 * i];
				dxx1 = buffx[2 * i + 1];

				r0 = mmx_fmul8x16hi((s0), dxx0);
				r1 = mmx_fmul8x16hi((s1), dxx1);
				r2 = _mm_add_pi16(r0, r1);

				pbuff[i] =
				    mmx_fpadd16s(mmx_read_lo(r2),
				    mmx_read_hi(r2));
			}

			tsp += src_stride;
		}

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];

		for (i = 0; i < width2; i++) {
			__m64 s0, s1, s2, s3, r0, r1, r2, r3, r4, r5, r6;

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

			tdp[i] = mmx_fpack16(r6);
		}

		if (width & 1) {
			mlib_s32 mask = *(mlib_s32 *)(mlib_mask64_arr + 2);

			tdp[width2 - 1] =
			    (tdp[width2 - 1] & mask) | (tail & ~mask);
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
mlib_m_ImageZoom_U8_4_1_Bicubic(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	mlib_u8 *tsp;
	mlib_s32 *prow;
	mlib_s32 *tdp;
	mlib_s32 fpos;
	__m64 *dfPtr, dyf0, dyf1, dyf2, dyf3;
	__m64 *buff, *buff_arr[8], *buff0, *buff1, *buff2, *buff3, *buffx,
	    *pbuff;
	mlib_s32 k, bind = 0, y_step = 4;

	buff = mlib_malloc(width * 8 * sizeof (__m64));
	if (buff == NULL) {
		_mm_empty();
		return (MLIB_FAILURE);
	}
	for (k = 0; k < 4; k++)
		buff_arr[k] = buff + k * width;
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k];
	buffx = buff + 4 * width;

	x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	for (i = 0; i < width; i++) {
		__m64 *p_flt;

		fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
		p_flt = (__m64 *) ((mlib_u8 *)FILTER_4 + 4 * fpos);
		buffx[4 * i] = p_flt[0];
		buffx[4 * i + 1] = p_flt[1];
		buffx[4 * i + 2] = p_flt[2];
		buffx[4 * i + 3] = p_flt[3];
		x += dx;
	}

	sp -= src_stride + 4;
	tsp = sp;

	for (j = 0; j < height; j++) {
		COEFF_D64_U8(y)

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

			prow = (mlib_s32 *)tsp;

			for (i = 0; i < width; i++) {
				cx = x >> MLIB_SHIFT;

				s0 = *(__m64 *) (prow + cx);
				s1 = *(__m64 *) (prow + cx + 1);
				s2 = *(__m64 *) (prow + cx + 2);
				s3 = *(__m64 *) (prow + cx + 3);

				r0 = mmx_fmul8x16hi(s0, buffx[4 * i]);
				r1 = mmx_fmul8x16hi(s1, buffx[4 * i + 1]);
				r2 = mmx_fmul8x16hi(s2, buffx[4 * i + 2]);
				r3 = mmx_fmul8x16hi(s3, buffx[4 * i + 3]);

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

		tdp = (mlib_s32 *)dp;

		if (k < 4) {
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			prow = (mlib_s32 *)tsp;

			for (i = 0; i < width; i++) {
				__m64 a0, a1, a2, a3;
				__m64 b0, b1, b2, b3, b4, b5;
				__m64 s0, s1, s2, s3, r0, r1, r2, r3, r4, r5,
				    r6;

				cx = x >> MLIB_SHIFT;

				a0 = *(__m64 *) (prow + cx);
				a1 = *(__m64 *) (prow + cx + 1);
				a2 = *(__m64 *) (prow + cx + 2);
				a3 = *(__m64 *) (prow + cx + 3);

				s0 = buff0[i];
				s1 = buff1[i];
				s2 = buff2[i];

				b0 = mmx_fmul8x16hi(a0, buffx[4 * i]);
				b1 = mmx_fmul8x16hi(a1, buffx[4 * i + 1]);
				b2 = mmx_fmul8x16hi(a2, buffx[4 * i + 2]);
				b3 = mmx_fmul8x16hi(a3, buffx[4 * i + 3]);

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

				buff3[i] = s3;
				tdp[i] = mmx_fpack16(r6);
				x += dx;
			}

			tsp += src_stride;

		} else {

			for (i = 0; i < width; i++) {
				__m64 s0, s1, s2, s3, r0, r1, r2, r3, r4, r5,
				    r6;
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

				tdp[i] = mmx_fpack16(r6);
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
mlib_m_ImageZoom_U8_3_1_Bicubic(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	mlib_u8 *tsp, *tdp, tail;
	mlib_s32 fpos;
	__m64 *dfPtr, dyf0, dyf1, dyf2, dyf3;
	__m64 *buff, *buff_arr[8], *buff0, *buff1, *buff2, *buff3, *buffx,
	    *pbuff;
	mlib_s32 k, bind = 0, y_step = 4;

	buff = mlib_malloc(width * 8 * sizeof (__m64));
	if (buff == NULL) {
		_mm_empty();
		return (MLIB_FAILURE);
	}
	for (k = 0; k < 4; k++)
		buff_arr[k] = buff + k * width;
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k];
	buffx = buff + 4 * width;

	x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	for (i = 0; i < width; i++) {
		__m64 *p_flt;

		fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
		p_flt = (__m64 *) ((mlib_u8 *)FILTER_4 + 4 * fpos);
		buffx[4 * i] = p_flt[0];
		buffx[4 * i + 1] = p_flt[1];
		buffx[4 * i + 2] = p_flt[2];
		buffx[4 * i + 3] = p_flt[3];
		x += dx;
	}

	sp -= src_stride + 3;
	tsp = sp;

	for (j = 0; j < height; j++) {
		COEFF_D64_U8(y)

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

				s0 = *(__m64 *) (tsp + cx);
				s1 = *(__m64 *) (tsp + cx + 3);
				s2 = *(__m64 *) (tsp + cx + 6);
				s3 = *(__m64 *) (tsp + cx + 9);

				r0 = mmx_fmul8x16hi(s0, buffx[4 * i]);
				r1 = mmx_fmul8x16hi(s1, buffx[4 * i + 1]);
				r2 = mmx_fmul8x16hi(s2, buffx[4 * i + 2]);
				r3 = mmx_fmul8x16hi(s3, buffx[4 * i + 3]);

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
				__m64 a0, a1, a2, a3;
				__m64 b0, b1, b2, b3, b4, b5;
				__m64 s0, s1, s2, s3, r0, r1, r2, r3, r4, r5,
				    r6;

				cx = x >> MLIB_SHIFT;
				cx += 2 * cx;

				a0 = *(__m64 *) (tsp + cx);
				a1 = *(__m64 *) (tsp + cx + 3);
				a2 = *(__m64 *) (tsp + cx + 6);
				a3 = *(__m64 *) (tsp + cx + 9);

				s0 = buff0[i];
				s1 = buff1[i];
				s2 = buff2[i];

				b0 = mmx_fmul8x16hi(a0, buffx[4 * i]);
				b1 = mmx_fmul8x16hi(a1, buffx[4 * i + 1]);
				b2 = mmx_fmul8x16hi(a2, buffx[4 * i + 2]);
				b3 = mmx_fmul8x16hi(a3, buffx[4 * i + 3]);

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

				buff3[i] = s3;
				*(mlib_s32 *)tdp = mmx_fpack16(r6);
				tdp += 3;
				x += dx;
			}

			tsp += src_stride;

		} else {

			for (i = 0; i < width; i++) {
				__m64 s0, s1, s2, s3, r0, r1, r2, r3, r4, r5,
				    r6;
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

				*(mlib_s32 *)tdp = mmx_fpack16(r6);
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
