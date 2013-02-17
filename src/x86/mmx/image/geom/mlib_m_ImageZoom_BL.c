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

#pragma ident	"@(#)mlib_m_ImageZoom_BL.c	9.2	07/11/05 SMI"

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

#define	MMX_USE_MODIF_8x16

#include <stdlib.h>
#include <mlib_image.h>
#include <mlib_mmx_utils.h>
#include <mlib_ImageZoom.h>

/* *********************************************************** */

#define	BUFF_SIZE	2048

#define	GET_MASK(off)	(mlib_u32)(-1) >> (32 - 8*(off))

/* *********************************************************** */

#define	LD_4_PIX_U8(a0, a1, tsp0, tsp1)                                   \
	a0 = MMX_MERGE4_U8(tsp0[cx0], tsp0[cx1], tsp0[cx2], tsp0[cx3]);   \
	a1 = MMX_MERGE4_U8(tsp1[cx0], tsp1[cx1], tsp1[cx2], tsp1[cx3])

#define	LD_2_PIX_U16(a0, a1, tsp0, tsp1)                                \
	a0 =                                                            \
	_m_from_int(*(mlib_u16 *)(tsp0 + cx0) | (*(mlib_u16 *)(tsp0 +   \
	    cx1) << 16));                                               \
	a1 =                                                            \
	_m_from_int(*(mlib_u16 *)(tsp1 + cx0) | (*(mlib_u16 *)(tsp1 +   \
	    cx1) << 16))

/* *********************************************************** */

#define	INIT_XLINE()                                            \
	x = GetElemSubStruct(current, srcX) & MLIB_MASK;        \
	x0 = x;                                                 \
	x1 = x0 + dx;                                           \
	x2 = x0 + (dx << 1);                                    \
	x3 = x1 + (dx << 1);                                    \
	yo = (y >> 1) & 0x7FFF;                                 \
	ddy = mmx_from_int_dup((yo << 16) | yo);                \
	rdy = _mm_sub_pi16(mask_7fff, ddy);                     \
	tsp0 = tsp;                                             \
	tsp2 = tsp0 + src_stride

/* *********************************************************** */

#undef   VARIABLE
#define	VARIABLE(FORMAT)                                            \
	FORMAT *sp = GetElemSubStruct(current, sp), *dp =           \
	GetElemSubStruct(current, dp), *tsp = sp;                   \
	mlib_s32 i, j, dx = GetElemStruct(DX), dy =                 \
	GetElemStruct(DY), x =                                      \
	GetElemSubStruct(current, srcX) & MLIB_MASK, y =            \
	GetElemSubStruct(current, srcY) & MLIB_MASK, src_stride =   \
	GetElemStruct(src_stride), dst_stride =                     \
	GetElemStruct(dst_stride), width =                          \
	GetElemSubStruct(current, width), height =                  \
	GetElemSubStruct(current, height);

/* *********************************************************** */

#define	ZOOM_Y()                                                \
	y10 = _mm_mulhi_pi16(rdy, y0);                          \
	z10 = _mm_mulhi_pi16(ddy, y1);                          \
	dd = _mm_add_pi16(y10, z10);                            \
	dd = _mm_add_pi16(dd, k05);                             \
	tdp[i] = mmx_fpack16(dd)

/* *********************************************************** */

#undef  MMX_SCALE
#define	MMX_SCALE	1

/* *********************************************************** */

mlib_status
mlib_m_ImageZoom_U8_1_Bilinear(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	__m64 buff_arr[BUFF_SIZE / 8], *buff0, *buff1, *buffx;
	void *buff = buff_arr, *buffd;
	mlib_s32 *tdp, tail;
	mlib_u8 *tsp0, *tsp1, *tsp2, *tsp3;
	mlib_s32 x0, x1, x2, x3, cx0, cx1, cx2, cx3, yo;
	mlib_s32 width4, size, y_step = 2, dx4 = dx * 4;
	__m64 ddx, ddy, rdy, dx64;
	__m64 mask_7fff, k05;

	width4 = (width + 3) / 4;

	size = 7 * 4 * width4 + 8;
	if (size > BUFF_SIZE) {
		buff = mlib_malloc(size);
		if (buff == NULL) {
			_mm_empty();
			return (MLIB_FAILURE);
		}
	}

	buff0 = buff;
	buff1 = buff0 + width4;
	buffx = buff1 + width4;
	buffd = buffx + width4;

	mask_7fff = mmx_to_double_dup(0x7FFF7FFF);
	k05 = mmx_to_double_dup(0x00400040);

	x0 = GetElemSubStruct(current, srcX) & MLIB_MASK;
	x1 = x0 + dx;
	x2 = x0 + (dx << 1);
	x3 = x1 + (dx << 1);

/* calculate dx for all row */
	dx64 = mmx_from_int_dup(((dx4 & 0xFFFE) << 15) | ((dx4 & 0xFFFE) >> 1));
	ddx = mmx_to_double(((x0 & 0xFFFE) << 15) | ((x1 & 0xFFFE) >> 1),
	    ((x2 & 0xFFFE) << 15) | ((x3 & 0xFFFE) >> 1));
	for (i = 0; i < width4; i++) {
		buffx[i] = ddx;
		ddx = _mm_and_si64(_mm_add_pi16(ddx, dx64), mask_7fff);
	}

	for (j = 0; j < height; j++) {
		tdp = (mlib_s32 *)dp;
		tail = tdp[width4 - 1];

		if (y_step == 0) {
			yo = (y >> 1) & 0x7FFF;
			ddy = mmx_from_int_dup((yo << 16) | yo);
			rdy = _mm_sub_pi16(mask_7fff, ddy);

			for (i = 0; i < width4; i++) {
				__m64 y0, y1, y10, z10, dd;

				y0 = buff0[i];
				y1 = buff1[i];

				ZOOM_Y();
			}

		} else if (y_step == 1) {
			__m64 *buffT;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			INIT_XLINE();
			tsp3 = tsp2 + 1;

			for (i = 0; i < width4; i++) {
				__m64 ddx, rdx, sd2, sd3, v2, v3, y0, y1, y10,
				    z0, z1, z10, dd;

				cx0 = (x0 >> MLIB_SHIFT);
				cx1 = (x1 >> MLIB_SHIFT);
				cx2 = (x2 >> MLIB_SHIFT);
				cx3 = (x3 >> MLIB_SHIFT);
				LD_4_PIX_U8(sd2, sd3, tsp2, tsp3);

				ddx = buffx[i];
				rdx = _mm_sub_pi16(mask_7fff, ddx);

				v2 = mmx_fmul8x16hi((sd2), rdx);
				v3 = mmx_fmul8x16hi((sd3), ddx);

				y1 = _mm_add_pi16(v2, v3);
				buff1[i] = y1;
				y0 = buff0[i];

				ZOOM_Y();

				x0 += dx4;
				x1 += dx4;
				x2 += dx4;
				x3 += dx4;
			}
		} else {
			INIT_XLINE();
			tsp1 = tsp0 + 1;
			tsp3 = tsp2 + 1;

			for (i = 0; i < width4; i++) {
				__m64 ddx, rdx, sd0, sd1, sd2, sd3, v0, v1, v2,
				    v3, y0, y1, y10, z10, dd;

				cx0 = (x0 >> MLIB_SHIFT);
				cx1 = (x1 >> MLIB_SHIFT);
				cx2 = (x2 >> MLIB_SHIFT);
				cx3 = (x3 >> MLIB_SHIFT);

				ddx = buffx[i];
				rdx = _mm_sub_pi16(mask_7fff, ddx);

				LD_4_PIX_U8(sd0, sd1, tsp0, tsp1);
				v0 = mmx_fmul8x16hi((sd0), rdx);
				v1 = mmx_fmul8x16hi((sd1), ddx);

				LD_4_PIX_U8(sd2, sd3, tsp2, tsp3);
				v2 = mmx_fmul8x16hi((sd2), rdx);
				v3 = mmx_fmul8x16hi((sd3), ddx);

				y0 = _mm_add_pi16(v0, v1);
				y1 = _mm_add_pi16(v2, v3);
				buff0[i] = y0;
				buff1[i] = y1;

				ZOOM_Y();

				x0 += dx4;
				x1 += dx4;
				x2 += dx4;
				x3 += dx4;
			}
		}

		if (width & 3) {
			mlib_s32 mask = GET_MASK(width & 3);

			tdp[width4 - 1] =
			    (tdp[width4 - 1] & mask) | (tail & ~mask);
		}

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		dp += dst_stride;
		y += dy;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	_mm_empty();

	if (size > BUFF_SIZE) {
		mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_m_ImageZoom_U8_2_Bilinear(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	__m64 buff_arr[BUFF_SIZE / 8], *buff0, *buff1, *buffx;
	void *buff = buff_arr, *buffd, *row_0, *row_1;
	mlib_s32 *tdp, tail;
	mlib_u8 *tsp0, *tsp1, *tsp2, *tsp3;
	mlib_u32 mask;
	mlib_s32 x0, x1, x2, x3, cx0, cx1, yo;
	mlib_s32 width2, size, row_size, y_step = 2, dx2 = dx * 2;
	__m64 ddx, ddy, rdy, dx64;
	__m64 mask_7fff, k05;

	width2 = (width + 1) / 2;

	row_size = 2 * ((width * dx) >> MLIB_SHIFT) + 8;
	size = 7 * 4 * width2 + 2 * row_size;
	if (size > BUFF_SIZE) {
		buff = mlib_malloc(size);
		if (buff == NULL) {
			_mm_empty();
			return (MLIB_FAILURE);
		}
	}

	buff0 = buff;
	buff1 = buff0 + width2;
	buffx = buff1 + width2;
	buffd = buffx + width2;
	row_0 = (mlib_s32 *)buffd + width2;
	row_1 = (mlib_u8 *)row_0 + row_size;

	mask_7fff = mmx_to_double_dup(0x7FFF7FFF);
	k05 = mmx_to_double_dup(0x00400040);

	x0 = GetElemSubStruct(current, srcX) & MLIB_MASK;
	x1 = x0 + dx;

/* calculate dx for all row */
	dx64 =
	    mmx_to_double_dup(((dx2 & 0xFFFE) << 15) | ((dx2 & 0xFFFE) >> 1));
	ddx =
	    mmx_to_double(((x0 & 0xFFFE) << 15) | ((x0 & 0xFFFE) >> 1),
	    ((x1 & 0xFFFE) << 15) | ((x1 & 0xFFFE) >> 1));
	for (i = 0; i < width2; i++) {
		buffx[i] = ddx;
		ddx = _mm_and_si64(_mm_add_pi16(ddx, dx64), mask_7fff);
	}

	for (j = 0; j < height; j++) {
		tdp = (mlib_s32 *)dp;
		tail = tdp[width2 - 1];

		if (y_step == 0) {
			yo = (y >> 1) & 0x7FFF;
			ddy = mmx_from_int_dup((yo << 16) | yo);
			rdy = _mm_sub_pi16(mask_7fff, ddy);

			for (i = 0; i < width2; i++) {
				__m64 y0, y1, y10, z10, dd;

				y0 = buff0[i];
				y1 = buff1[i];

				ZOOM_Y();
			}

		} else if (y_step == 1) {
			__m64 *buffT;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			INIT_XLINE();

			tsp3 = tsp2 + 2;

			for (i = 0; i < width2; i++) {
				__m64 ddx, rdx, sd2, sd3, v2, v3, y0, y1, y10,
				    z10, dd;

				cx0 = (x0 >> MLIB_SHIFT);
				cx1 = (x1 >> MLIB_SHIFT);
				cx0 += cx0;
				cx1 += cx1;

				ddx = buffx[i];
				rdx = _mm_sub_pi16(mask_7fff, ddx);

				LD_2_PIX_U16(sd2, sd3, tsp2, tsp3);
				v2 = mmx_fmul8x16hi((sd2), rdx);
				v3 = mmx_fmul8x16hi((sd3), ddx);

				y0 = buff0[i];
				y1 = _mm_add_pi16(v2, v3);

				ZOOM_Y();

				x0 += dx2;
				x1 += dx2;

				buff1[i] = y1;
			}

		} else {

			INIT_XLINE();

			tsp1 = tsp0 + 2;
			tsp3 = tsp2 + 2;

			for (i = 0; i < width2; i++) {
				__m64 ddx, rdx, sd0, sd1, sd2, sd3, v0, v1, v2,
				    v3, y0, y1, y10, z10, dd;

				cx0 = (x0 >> MLIB_SHIFT);
				cx1 = (x1 >> MLIB_SHIFT);
				cx0 += cx0;
				cx1 += cx1;

				ddx = buffx[i];
				rdx = _mm_sub_pi16(mask_7fff, ddx);

				LD_2_PIX_U16(sd0, sd1, tsp0, tsp1);
				v0 = mmx_fmul8x16hi((sd0), rdx);
				v1 = mmx_fmul8x16hi((sd1), ddx);

				LD_2_PIX_U16(sd2, sd3, tsp2, tsp3);
				v2 = mmx_fmul8x16hi((sd2), rdx);
				v3 = mmx_fmul8x16hi((sd3), ddx);

				y0 = _mm_add_pi16(v0, v1);
				y1 = _mm_add_pi16(v2, v3);

				ZOOM_Y();

				x0 += dx2;
				x1 += dx2;

				buff0[i] = y0;
				buff1[i] = y1;
			}
		}

		if (width & 1) {
			mlib_s32 mask = GET_MASK(2);

			tdp[width2 - 1] =
			    (tdp[width2 - 1] & mask) | (tail & ~mask);
		}

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		dp += dst_stride;
		y += dy;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	_mm_empty();

	if (size > BUFF_SIZE) {
		mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_m_ImageZoom_U8_3_Bilinear(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	__m64 buff_arr[BUFF_SIZE / 8], *buff0, *buff1, *buffx;
	void *buff = buff_arr, *buffd;
	mlib_s32 *tdp, tail;
	mlib_u8 *tsp0, *tsp1, *tsp2, *tsp3;
	mlib_s32 x0, x1, x2, x3, yo;
	mlib_s32 width4, width3_4, size, y_step = 2, dx4 = dx * 4;
	__m64 ddy, rdy, dx_0, dx_1, dx_2, dx64;
	__m64 mask_7fff, k05;
	mlib_s32 *buffc;
	mlib_u16 *buffcc;

	width4 = (width + 3) / 4;
	width3_4 = (3 * width + 3) / 4;

	size = 9 * 12 * width4 + 32;
	if (size > BUFF_SIZE) {
		buff = mlib_malloc(size);
		if (buff == NULL) {
			_mm_empty();
			return (MLIB_FAILURE);
		}
	}

	x0 = GetElemSubStruct(current, srcX) & MLIB_MASK;
	x1 = x0 + dx;
	x2 = x0 + (dx << 1);
	x3 = x1 + (dx << 1);

/* calculate dx for all row */
	dx64 =
	    mmx_to_double_dup(((dx4 & 0xFFFE) << 15) | ((dx4 & 0xFFFE) >> 1));
	dx_0 =
	    mmx_to_double(((x0 & 0xFFFE) << 15) | ((x0 & 0xFFFE) >> 1),
	    ((x0 & 0xFFFE) << 15) | ((x1 & 0xFFFE) >> 1));
	dx_1 =
	    mmx_to_double(((x1 & 0xFFFE) << 15) | ((x1 & 0xFFFE) >> 1),
	    ((x2 & 0xFFFE) << 15) | ((x2 & 0xFFFE) >> 1));
	dx_2 =
	    mmx_to_double(((x2 & 0xFFFE) << 15) | ((x3 & 0xFFFE) >> 1),
	    ((x3 & 0xFFFE) << 15) | ((x3 & 0xFFFE) >> 1));

	buffx = buff;
	buff0 = buffx + 3 * width4;
	buff1 = buff0 + 3 * width4;
	buffd = buff1 + 3 * width4;
	buffc = (mlib_s32 *)buffd + 3 * width4;

	mask_7fff = mmx_to_double_dup(0x7FFF7FFF);
	k05 = mmx_to_double_dup(0x00400040);

	for (i = 0; i < width4; i++) {
		buffx[3 * i] = dx_0;
		buffx[3 * i + 1] = dx_1;
		buffx[3 * i + 2] = dx_2;
		dx_0 = _mm_and_si64(_mm_add_pi16(dx_0, dx64), mask_7fff);
		dx_1 = _mm_and_si64(_mm_add_pi16(dx_1, dx64), mask_7fff);
		dx_2 = _mm_and_si64(_mm_add_pi16(dx_2, dx64), mask_7fff);
	}

	x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	buffcc = (mlib_u16 *)buffc;
	for (i = 0; i < 4 * width4; i++) {
		mlib_s32 cx = (x >> MLIB_SHIFT);

		cx += 2 * cx;
		buffcc[3 * i] = cx;
		buffcc[3 * i + 1] = cx + 1;
		buffcc[3 * i + 2] = cx + 2;
		x += dx;
	}

	for (j = 0; j < height; j++) {
		tdp = (mlib_s32 *)dp;
		tail = tdp[width3_4 - 1];

		if (y_step == 0) {
			yo = (y >> 1) & 0x7FFF;
			ddy = mmx_from_int_dup((yo << 16) | yo);
			rdy = _mm_sub_pi16(mask_7fff, ddy);

			for (i = 0; i < width3_4; i++) {
				__m64 y0, y1, y10, z10, dd;

				y0 = buff0[i];
				y1 = buff1[i];

				ZOOM_Y();
			}

		} else if (y_step == 1) {
			__m64 *buffT;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			yo = (y >> 1) & 0x7FFF;
			ddy = mmx_from_int_dup((yo << 16) | yo);
			rdy = _mm_sub_pi16(mask_7fff, ddy);

			tsp2 = tsp + src_stride;
			tsp3 = tsp2 + 3;

			for (i = 0; i < width3_4; i++) {
				__m64 ddx, rdx, sd2, sd3, v2, v3, y0, y1, y10,
				    z10, dd;
				mlib_s32 cx01, cx23, cx0, cx1, cx2, cx3;

				cx01 = buffc[2 * i];
				cx23 = buffc[2 * i + 1];
				cx1 = cx01 >> 16;
				cx0 = cx01 & 0xFFFF;
				cx3 = cx23 >> 16;
				cx2 = cx23 & 0xFFFF;

				ddx = buffx[i];
				rdx = _mm_sub_pi16(mask_7fff, ddx);

				LD_4_PIX_U8(sd2, sd3, tsp2, tsp3);
				v2 = mmx_fmul8x16hi((sd2), rdx);
				v3 = mmx_fmul8x16hi((sd3), ddx);

				y0 = buff0[i];
				y1 = _mm_add_pi16(v2, v3);

				ZOOM_Y();

				buff1[i] = y1;
			}

		} else {
			yo = (y >> 1) & 0x7FFF;
			ddy = mmx_from_int_dup((yo << 16) | yo);
			rdy = _mm_sub_pi16(mask_7fff, ddy);

			tsp0 = tsp;
			tsp1 = tsp0 + 3;
			tsp2 = tsp0 + src_stride;
			tsp3 = tsp2 + 3;

			for (i = 0; i < width3_4; i++) {
				__m64 ddx, rdx, sd0, sd1, sd2, sd3, v0, v1, v2,
				    v3, y0, y1, y10, z10, dd;
				mlib_s32 cx01, cx23, cx0, cx1, cx2, cx3;

				cx01 = buffc[2 * i];
				cx23 = buffc[2 * i + 1];
				cx1 = cx01 >> 16;
				cx0 = cx01 & 0xFFFF;
				cx3 = cx23 >> 16;
				cx2 = cx23 & 0xFFFF;

				ddx = buffx[i];
				rdx = _mm_sub_pi16(mask_7fff, ddx);

				LD_4_PIX_U8(sd0, sd1, tsp0, tsp1);
				v0 = mmx_fmul8x16hi((sd0), rdx);
				v1 = mmx_fmul8x16hi((sd1), ddx);

				LD_4_PIX_U8(sd2, sd3, tsp2, tsp3);
				v2 = mmx_fmul8x16hi((sd2), rdx);
				v3 = mmx_fmul8x16hi((sd3), ddx);

				y0 = _mm_add_pi16(v0, v1);
				y1 = _mm_add_pi16(v2, v3);

				ZOOM_Y();

				buff0[i] = y0;
				buff1[i] = y1;
			}
		}

		if ((3 * width) & 3) {
			mlib_s32 mask = GET_MASK((3 * width) & 3);

			tdp[width3_4 - 1] =
			    (tdp[width3_4 - 1] & mask) | (tail & ~mask);
		}

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		dp += dst_stride;
		y += dy;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	_mm_empty();

	if (size > BUFF_SIZE) {
		mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_m_ImageZoom_U8_4_Bilinear(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	__m64 buff_arr[BUFF_SIZE / 8], *buff0, *buff1, *buffx;
	void *buff = buff_arr, *buffd, *row_0, *row_1;
	mlib_s32 *tdp;
	mlib_u8 *tsp0, *tsp1, *tsp2, *tsp3;
	mlib_s32 x0, x1, x2, x3, cx0, yo;
	mlib_s32 size, row_size, y_step, next_step = 2;
	__m64 ddx, ddy, rdy, dx64;
	__m64 mask_7fff, k05;

	row_size = 4 * ((width * dx) >> MLIB_SHIFT) + 16;
	size = 7 * 4 * width + 2 * row_size;
	if (size > BUFF_SIZE) {
		buff = mlib_malloc(size);
		if (buff == NULL) {
			_mm_empty();
			return (MLIB_FAILURE);
		}
	}

	buff0 = buff;
	buff1 = buff0 + width;
	buffx = buff1 + width;
	buffd = buffx + width;
	row_0 = (mlib_s32 *)buffd + width;
	row_1 = (mlib_u8 *)row_0 + row_size;

	mask_7fff = mmx_to_double_dup(0x7FFF7FFF);
	k05 = mmx_to_double_dup(0x00400040);

	x0 = GetElemSubStruct(current, srcX) & MLIB_MASK;

/* calculate dx for all row */
	dx64 = mmx_to_double_dup(((dx & 0xFFFE) << 15) | ((dx & 0xFFFE) >> 1));
	ddx = mmx_to_double(((x0 & 0xFFFE) << 15) | ((x0 & 0xFFFE) >> 1),
	    ((x0 & 0xFFFE) << 15) | ((x0 & 0xFFFE) >> 1));
	for (i = 0; i < width; i++) {
		buffx[i] = ddx;
		ddx = _mm_and_si64(_mm_add_pi16(ddx, dx64), mask_7fff);
	}

	for (j = 0; j < height; j++) {
		tdp = (mlib_s32 *)dp;

		y_step = next_step;
		next_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);

		if (y_step == 0) {
			yo = (y >> 1) & 0x7FFF;
			ddy = mmx_from_int_dup((yo << 16) | yo);
			rdy = _mm_sub_pi16(mask_7fff, ddy);

			for (i = 0; i < width; i++) {
				__m64 y0, y1, y10, z10, dd;

				y0 = buff0[i];
				y1 = buff1[i];

				ZOOM_Y();
			}

		} else if (y_step == 1) {
			__m64 *buffT;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			INIT_XLINE();

			tsp3 = tsp2 + 4;

			for (i = 0; i < width; i++) {
				__m64 ddx, rdx, v2, v3, y0, y1, y10, z10, dd;
				mlib_s32 sf2, sf3;

				cx0 = (x0 >> MLIB_SHIFT);

				ddx = buffx[i];
				rdx = _mm_sub_pi16(mask_7fff, ddx);

				sf2 = ((mlib_s32 *)tsp2)[cx0];
				sf3 = ((mlib_s32 *)tsp3)[cx0];
				v2 = mmx_fmul8x16(sf2, rdx);
				v3 = mmx_fmul8x16(sf3, ddx);

				y0 = buff0[i];
				y1 = _mm_add_pi16(v2, v3);

				ZOOM_Y();

				x0 += dx;

				buff1[i] = y1;
			}

		} else {
			INIT_XLINE();

			tsp1 = tsp0 + 4;
			tsp3 = tsp2 + 4;

/* without write to buff0 and buff1 */
			if (next_step >= 2) {
				for (i = 0; i < width; i++) {
					__m64 ddx, rdx;
					__m64 v0, v1, v2, v3, y0, y1, y10, z10,
					    dd;
					mlib_s32 sf0, sf1, sf2, sf3;

					cx0 = (x0 >> MLIB_SHIFT);

					sf0 = ((mlib_s32 *)tsp0)[cx0];
					sf1 = ((mlib_s32 *)tsp1)[cx0];
					sf2 = ((mlib_s32 *)tsp2)[cx0];
					sf3 = ((mlib_s32 *)tsp3)[cx0];

					ddx = buffx[i];
					rdx = _mm_sub_pi16(mask_7fff, ddx);

					v0 = mmx_fmul8x16(sf0, rdx);
					v1 = mmx_fmul8x16(sf1, ddx);
					v2 = mmx_fmul8x16(sf2, rdx);
					v3 = mmx_fmul8x16(sf3, ddx);

					y0 = _mm_add_pi16(v0, v1);
					y1 = _mm_add_pi16(v2, v3);

					ZOOM_Y();

					x0 += dx;
				}
			} else {
				__m64 dx64, ddx, rdx;

				dx64 =
				    mmx_to_double_dup(((dx & 0xFFFE) << 15) |
				    ((dx & 0xFFFE) >> 1));
				ddx =
				    mmx_to_double_dup(((x0 & 0xFFFE) << 15) |
				    ((x0 & 0xFFFE) >> 1));
				rdx = _mm_sub_pi16(mask_7fff, ddx);

				for (i = 0; i < width; i++) {
					__m64 v0, v1, v2, v3, y0, y1, y10, z10,
					    dd;
					mlib_s32 sf0, sf1, sf2, sf3;

					cx0 = (x0 >> MLIB_SHIFT);

					sf0 = ((mlib_s32 *)tsp0)[cx0];
					sf1 = ((mlib_s32 *)tsp1)[cx0];
					sf2 = ((mlib_s32 *)tsp2)[cx0];
					sf3 = ((mlib_s32 *)tsp3)[cx0];

					rdx = _mm_sub_pi16(mask_7fff, ddx);

					v0 = mmx_fmul8x16(sf0, rdx);
					v1 = mmx_fmul8x16(sf1, ddx);
					v2 = mmx_fmul8x16(sf2, rdx);
					v3 = mmx_fmul8x16(sf3, ddx);

					y0 = _mm_add_pi16(v0, v1);
					y1 = _mm_add_pi16(v2, v3);

					ZOOM_Y();

					x0 += dx;
					ddx =
					    _mm_and_si64(_mm_add_pi16(ddx,
					    dx64), mask_7fff);
					rdx = _mm_sub_pi16(mask_7fff, ddx);

					buff0[i] = y0;
					buff1[i] = y1;
				}
			}
		}

		dp += dst_stride;
		y += dy;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	_mm_empty();

	if (size > BUFF_SIZE) {
		mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
