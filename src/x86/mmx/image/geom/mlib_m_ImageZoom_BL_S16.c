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

#pragma ident	"@(#)mlib_m_ImageZoom_BL_S16.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *   Internal functions for mlib_ImageZoomTranslate on image of S16 data type
 */

/* *********************************************************** */

#include <stdlib.h>
#include <mlib_image.h>
#include <mlib_mmx_utils.h>
#include <mlib_ImageZoom.h>

/* *********************************************************** */

/* #define TYPE_U16 */

#ifdef TYPE_U16

#define	FUNC(SUFF)	mlib_m_ImageZoom_U16_##SUFF
#define	DEF_MASK_8000	__m64 mask8000;
#define	XOR_8000(x)	x = _mm_xor_si64(x, mask8000);

#else

#define	FUNC(SUFF)	mlib_m_ImageZoom_S16_##SUFF
#define	DEF_MASK_8000

#define	XOR_8000(x)

#endif

/* *********************************************************** */

#define	BUFF_SIZE	2048

/* *********************************************************** */

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
	GetElemSubStruct(current, height);                          \
	    mlib_d64 dzoomx = GetElemStruct(zoomx);                 \
	                                                            \
	DEF_MASK_8000

/* *********************************************************** */

#define	LD_4_PIX_U16(a0, a1, tsp0, tsp1)                                   \
	a0 = MMX_MERGE4_U16(tsp0[cx0], tsp0[cx1], tsp0[cx2], tsp0[cx3]);   \
	a1 = MMX_MERGE4_U16(tsp1[cx0], tsp1[cx1], tsp1[cx2], tsp1[cx3]);   \
	XOR_8000(a0) XOR_8000(a1)

#define	MUL2(x)	_mm_slli_pi16(x, 1)

/* *********************************************************** */

#define	INIT_XLINE()                                            \
	x = GetElemSubStruct(current, srcX) & MLIB_MASK;        \
	x0 = x;                                                 \
	x1 = x0 + dx;                                           \
	yo = ((y & 0xfffe) >> 1);                               \
	ddy = mmx_to_double_dup((yo << 16) | yo);               \
	rdy = _mm_sub_pi16(mask_7fff, ddy)

/* *********************************************************** */

#define	FMUL_16x16(z, x, y)	z = _mm_mulhi_pi16(x, y)

#undef  MMX_SCALE
#define	MMX_SCALE	0

/* *********************************************************** */

#define	VAL_U16_ONE	0x7FFE7FFE

/* *********************************************************** */

static void
mlib_empty_func()
{
}

/* *********************************************************** */

mlib_status FUNC(
    1_Bilinear) (
    mlib_work_image * param)
{
	VARIABLE(mlib_u16)
	__m64 buff_arr[BUFF_SIZE / 8], *buff0, *buff1, *buffx;
	void *buff = buff_arr, *buffd;
	__m64 *tdp, tail;
	mlib_u16 *tsp0, *tsp1, *tsp2, *tsp3;
	mlib_s32 x0, x1, x2, x3, cx0, cx1, cx2, cx3, yo;
	mlib_s32 width4, size, y_step = 2, dx4 = dx * 4;
	mlib_d64 cdx, zdx;
	__m64 ddy, rdy;
	__m64 mask_7fff;

	width4 = (width + 3) / 4;

	size = 8 * 4 * width4 + 8;
	if (size > BUFF_SIZE) {
		buff = mlib_malloc(size);
		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + width4;
	buffx = buff1 + width4;
	buffd = buffx + width4;

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
		((mlib_s16 *)buffx)[i] = ires;

		cdx += zdx;
		x += dx;
		mlib_empty_func();
	}

	mask_7fff = mmx_to_double_dup(VAL_U16_ONE);
#ifdef TYPE_U16
	mask8000 = mmx_to_double_dup(0x80008000);
#endif

	for (j = 0; j < height; j++) {
		tdp = (__m64 *) dp;
		tail = tdp[width4 - 1];

		if (y_step == 0) {
			yo = ((y & 0xfffe) >> 1);
			ddy = mmx_to_double_dup((yo << 16) | yo);
			rdy = _mm_sub_pi16(mask_7fff, ddy);

			for (i = 0; i < width4; i++) {
				__m64 y0, y1, z0, z1, dd;

				y0 = buff0[i];
				y1 = buff1[i];

				FMUL_16x16(z0, y0, rdy);
				FMUL_16x16(z1, y1, ddy);
				dd = _mm_add_pi16(z0, z1);
				dd = MUL2(dd);

				XOR_8000(dd)
				    tdp[i] = dd;
			}

		} else if (y_step == 1) {
			__m64 *buffT;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			INIT_XLINE();
			x2 = x0 + (dx << 1);
			x3 = x1 + (dx << 1);
			tsp2 = tsp + src_stride;
			tsp3 = tsp2 + 1;

			for (i = 0; i < width4; i++) {
				__m64 ddx, rdx, sd2, sd3, v2, v3, y0, y1, z0,
				    z1, dd;

				cx0 = (x0 >> MLIB_SHIFT);
				cx1 = (x1 >> MLIB_SHIFT);
				cx2 = (x2 >> MLIB_SHIFT);
				cx3 = (x3 >> MLIB_SHIFT);

				ddx = buffx[i];
				rdx = _mm_sub_pi16(mask_7fff, ddx);

				LD_4_PIX_U16(sd2, sd3, tsp2, tsp3);
				FMUL_16x16(v2, sd2, rdx);
				FMUL_16x16(v3, sd3, ddx);

				y0 = buff0[i];
				y1 = _mm_add_pi16(v2, v3);
				y1 = MUL2(y1);

				FMUL_16x16(z0, y0, rdy);
				FMUL_16x16(z1, y1, ddy);
				dd = _mm_add_pi16(z0, z1);
				dd = MUL2(dd);

				x0 += dx4;
				x1 += dx4;
				x2 += dx4;
				x3 += dx4;

				buff1[i] = y1;
				XOR_8000(dd)
				    tdp[i] = dd;
			}

		} else {
			INIT_XLINE();
			x2 = x0 + (dx << 1);
			x3 = x1 + (dx << 1);
			tsp0 = tsp;
			tsp1 = tsp0 + 1;
			tsp2 = tsp0 + src_stride;
			tsp3 = tsp2 + 1;

			for (i = 0; i < width4; i++) {
				__m64 ddx, rdx, sd0, sd1, sd2, sd3, v0, v1, v2,
				    v3, y0, y1, z0, z1, dd;

				cx0 = (x0 >> MLIB_SHIFT);
				cx1 = (x1 >> MLIB_SHIFT);
				cx2 = (x2 >> MLIB_SHIFT);
				cx3 = (x3 >> MLIB_SHIFT);

				ddx = buffx[i];
				rdx = _mm_sub_pi16(mask_7fff, ddx);

				LD_4_PIX_U16(sd0, sd1, tsp0, tsp1);
				LD_4_PIX_U16(sd2, sd3, tsp2, tsp3);

				FMUL_16x16(v0, sd0, rdx);
				FMUL_16x16(v1, sd1, ddx);
				y0 = _mm_add_pi16(v0, v1);
				y0 = MUL2(y0);

				FMUL_16x16(v2, sd2, rdx);
				FMUL_16x16(v3, sd3, ddx);
				y1 = _mm_add_pi16(v2, v3);
				y1 = MUL2(y1);

				FMUL_16x16(z0, y0, rdy);
				FMUL_16x16(z1, y1, ddy);
				dd = _mm_add_pi16(z0, z1);
				dd = MUL2(dd);

				x0 += dx4;
				x1 += dx4;
				x2 += dx4;
				x3 += dx4;

				buff0[i] = y0;
				buff1[i] = y1;
				XOR_8000(dd)
				    tdp[i] = dd;
			}
		}

		if (width & 3) {
			__m64 mask =
			    ((__m64 *) mlib_mask64_arr)[2 * (width & 3)];

			tdp[width4 - 1] =
			    _mm_or_si64(_mm_and_si64(mask, tdp[width4 - 1]),
			    _mm_andnot_si64(mask, tail));
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

mlib_status FUNC(
    2_Bilinear) (
    mlib_work_image * param)
{
	VARIABLE(mlib_s16)
	__m64 buff_arr[BUFF_SIZE / 8], *buff0, *buff1, *buffx;
	void *buff = buff_arr, *buffd, *row_0;
	__m64 *tdp, tail;
	mlib_s32 *tsp0, *tsp1, *tsp2, *tsp3;
	mlib_s32 x0, x1, cx0, cx1, yo;
	mlib_s32 width2, size, row_size, y_step = 2, dx2 = dx * 2;
	mlib_d64 cdx, zdx;
	__m64 ddy, rdy;
	__m64 mask_7fff;

	width2 = (width + 1) / 2;

	row_size = 4 * ((width * dx) >> MLIB_SHIFT) + 32;
	size = 8 * 4 * width2 + 2 * row_size;
	if (size > BUFF_SIZE) {
		buff = mlib_malloc(size);
		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + width2;
	buffx = buff1 + width2;
	buffd = buffx + width2;
	row_0 = (__m64 *) buffd + width2;

/* calculate dx for all row */
	cdx = x * 0.5;
	zdx = (1 << 15) / dzoomx;
	for (i = 0; i < width; i++) {
		mlib_s32 ires = ((mlib_s32)cdx) - ((x >> MLIB_SHIFT) << 15);

		if (ires >= (1 << 15))
			ires = (1 << 15) - 1;
		if (ires < 0)
			ires = 0;
		ires = (ires << 16) | ires;
		((mlib_s32 *)buffx)[i] = ires;
		cdx += zdx;
		x += dx;
		mlib_empty_func();
	}

	mask_7fff = mmx_to_double_dup(VAL_U16_ONE);
#ifdef TYPE_U16
	mask8000 = mmx_to_double_dup(0x80008000);
#endif

	for (j = 0; j < height; j++) {
		tdp = (__m64 *) dp;
		tail = tdp[width2 - 1];

		if (y_step == 0) {
			yo = ((y & 0xfffe) >> 1);
			ddy = mmx_to_double_dup((yo << 16) | yo);
			rdy = _mm_sub_pi16(mask_7fff, ddy);

			for (i = 0; i < width2; i++) {
				__m64 y0, y1, z0, z1, dd;

				y0 = buff0[i];
				y1 = buff1[i];

				FMUL_16x16(z0, y0, rdy);
				FMUL_16x16(z1, y1, ddy);
				dd = _mm_add_pi16(z0, z1);
				dd = MUL2(dd);

				XOR_8000(dd)
				    tdp[i] = dd;
			}

		} else if (y_step == 1) {
			__m64 *buffT;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			INIT_XLINE();
			tsp2 = (mlib_s32 *)(tsp + src_stride);
			tsp3 = tsp2 + 1;

			for (i = 0; i < width2; i++) {
				__m64 ddx, rdx, sd2, sd3, v2, v3, y0, y1, z0,
				    z1, dd;
				mlib_s32 sf2, sf3, se2, se3;

				cx0 = (x0 >> MLIB_SHIFT);
				cx1 = (x1 >> MLIB_SHIFT);

				ddx = buffx[i];
				rdx = _mm_sub_pi16(mask_7fff, ddx);

				sf2 = tsp2[cx0];
				se2 = tsp2[cx1];
				sf3 = tsp3[cx0];
				se3 = tsp3[cx1];

				sd2 = mmx_freg_pair(sf2, se2);
				sd3 = mmx_freg_pair(sf3, se3);
				XOR_8000(sd2);
				XOR_8000(sd3);

				FMUL_16x16(v2, sd2, rdx);
				FMUL_16x16(v3, sd3, ddx);

				y0 = buff0[i];
				y1 = _mm_add_pi16(v2, v3);
				y1 = MUL2(y1);

				FMUL_16x16(z0, y0, rdy);
				FMUL_16x16(z1, y1, ddy);
				dd = _mm_add_pi16(z0, z1);
				dd = MUL2(dd);

				x0 += dx2;
				x1 += dx2;

				buff1[i] = y1;
				XOR_8000(dd)
				    tdp[i] = dd;
			}

		} else {

			INIT_XLINE();
			tsp0 = (mlib_s32 *)tsp;
			tsp2 = (mlib_s32 *)(tsp + src_stride);
			tsp1 = tsp0 + 1;
			tsp3 = tsp2 + 1;

			for (i = 0; i < width2; i++) {
				__m64 ddx, rdx, sd0, sd1, sd2, sd3, v0, v1, v2,
				    v3, y0, y1, z0, z1, dd;
				mlib_s32 sf0, sf1, sf2, sf3, se0, se1, se2, se3;

				cx0 = (x0 >> MLIB_SHIFT);
				cx1 = (x1 >> MLIB_SHIFT);

				ddx = buffx[i];
				rdx = _mm_sub_pi16(mask_7fff, ddx);

				sf0 = tsp0[cx0];
				se0 = tsp0[cx1];
				sf1 = tsp1[cx0];
				se1 = tsp1[cx1];
				sf2 = tsp2[cx0];
				se2 = tsp2[cx1];
				sf3 = tsp3[cx0];
				se3 = tsp3[cx1];

				sd0 = mmx_freg_pair(sf0, se0);
				sd1 = mmx_freg_pair(sf1, se1);
				sd2 = mmx_freg_pair(sf2, se2);
				sd3 = mmx_freg_pair(sf3, se3);

				XOR_8000(sd0);
				XOR_8000(sd1);
				XOR_8000(sd2);
				XOR_8000(sd3);

				FMUL_16x16(v0, sd0, rdx);
				FMUL_16x16(v1, sd1, ddx);
				y0 = _mm_add_pi16(v0, v1);
				y0 = MUL2(y0);

				FMUL_16x16(v2, sd2, rdx);
				FMUL_16x16(v3, sd3, ddx);
				y1 = _mm_add_pi16(v2, v3);
				y1 = MUL2(y1);

				FMUL_16x16(z0, y0, rdy);
				FMUL_16x16(z1, y1, ddy);
				dd = _mm_add_pi16(z0, z1);
				dd = MUL2(dd);

				x0 += dx2;
				x1 += dx2;

				buff0[i] = y0;
				buff1[i] = y1;
				XOR_8000(dd)
				    tdp[i] = dd;
			}
		}

		if (width & 1) {
			__m64 mask = ((__m64 *) mlib_mask64_arr)[4];

			tdp[width2 - 1] =
			    _mm_or_si64(_mm_and_si64(mask, tdp[width2 - 1]),
			    _mm_andnot_si64(mask, tail));
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

mlib_status FUNC(
    3_Bilinear) (
    mlib_work_image * param)
{
	VARIABLE(mlib_u16)
	__m64 buff_arr[BUFF_SIZE / 8], *buff0, *buff1, *buffx;
	void *buff = buff_arr, *buffd;
	__m64 *tdp, tail;
	mlib_u16 *tsp0, *tsp1, *tsp2, *tsp3;
	mlib_s32 x0, x1, yo;
	mlib_s32 width4, width3_4, size, y_step = 2;
	mlib_d64 cdx, zdx;
	__m64 ddy, rdy;
	__m64 mask_7fff;
	mlib_s32 *buffc;
	mlib_u16 *buffcc;

	width4 = (width + 3) / 4;
	width3_4 = (3 * width + 3) / 4;

	size = 10 * 12 * width4 + 32;
	if (size > BUFF_SIZE) {
		buff = mlib_malloc(size);
		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buffx = buff;
	buff0 = buffx + 3 * width4;
	buff1 = buff0 + 3 * width4;
	buffd = buff1 + 3 * width4;
	buffc = (mlib_s32 *)buffd + 6 * width4;

/* calculate dx for all row */
	cdx = x * 0.5;
	zdx = (1 << 15) / dzoomx;
	for (i = 0; i < (width + 1) / 2; i++) {
		mlib_s32 ires0, ires1;

		ires0 = ((mlib_s32)cdx) - ((x >> MLIB_SHIFT) << 15);
		if (ires0 >= (1 << 15))
			ires0 = (1 << 15) - 1;
		if (ires0 < 0)
			ires0 = 0;
		cdx += zdx;
		x += dx;

		ires1 = ((mlib_s32)cdx) - ((x >> MLIB_SHIFT) << 15);
		if (ires1 >= (1 << 15))
			ires1 = (1 << 15) - 1;
		if (ires1 < 0)
			ires1 = 0;
		cdx += zdx;
		x += dx;

		((mlib_s32 *)buffx)[3 * i] = (ires0 << 16) | ires0;
		((mlib_s32 *)buffx)[3 * i + 1] = (ires1 << 16) | ires0;
		((mlib_s32 *)buffx)[3 * i + 2] = (ires1 << 16) | ires1;

		mlib_empty_func();
	}

/* calculate x for all row */
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

	mask_7fff = mmx_to_double_dup(VAL_U16_ONE);
#ifdef TYPE_U16
	mask8000 = mmx_to_double_dup(0x80008000);
#endif

	for (j = 0; j < height; j++) {
		tdp = (__m64 *) dp;
		tail = tdp[width3_4 - 1];

		if (y_step == 0) {
			yo = ((y & 0xfffe) >> 1);
			ddy = mmx_to_double_dup((yo << 16) | yo);
			rdy = _mm_sub_pi16(mask_7fff, ddy);

			for (i = 0; i < width3_4; i++) {
				__m64 y0, y1, z0, z1, dd;

				y0 = buff0[i];
				y1 = buff1[i];

				FMUL_16x16(z0, y0, rdy);
				FMUL_16x16(z1, y1, ddy);
				dd = _mm_add_pi16(z0, z1);
				dd = MUL2(dd);

				XOR_8000(dd)
				    tdp[i] = dd;
			}

		} else if (y_step == 1) {
			__m64 *buffT;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			INIT_XLINE();
			tsp2 = tsp + src_stride;
			tsp3 = tsp2 + 3;

			for (i = 0; i < width3_4; i++) {
				__m64 ddx, rdx, sd2, sd3, v2, v3, y0, y1, z0,
				    z1, dd;
				mlib_s32 cx01, cx23, cx0, cx1, cx2, cx3;

				cx01 = buffc[2 * i];
				cx23 = buffc[2 * i + 1];
				cx1 = cx01 >> 16;
				cx0 = cx01 & 0xFFFF;
				cx3 = cx23 >> 16;
				cx2 = cx23 & 0xFFFF;

				ddx = buffx[i];
				rdx = _mm_sub_pi16(mask_7fff, ddx);

				LD_4_PIX_U16(sd2, sd3, tsp2, tsp3);

				FMUL_16x16(v2, sd2, rdx);
				FMUL_16x16(v3, sd3, ddx);

				y0 = buff0[i];
				y1 = _mm_add_pi16(v2, v3);
				y1 = MUL2(y1);

				FMUL_16x16(z0, y0, rdy);
				FMUL_16x16(z1, y1, ddy);
				dd = _mm_add_pi16(z0, z1);
				dd = MUL2(dd);

				buff1[i] = y1;
				XOR_8000(dd)
				    tdp[i] = dd;
			}

		} else {
			INIT_XLINE();
			tsp0 = tsp;
			tsp1 = tsp0 + 3;
			tsp2 = tsp0 + src_stride;
			tsp3 = tsp2 + 3;

			for (i = 0; i < width3_4; i++) {
				__m64 ddx, rdx, sd0, sd1, sd2, sd3, v0, v1, v2,
				    v3, y0, y1, z0, z1, dd;
				mlib_s32 cx01, cx23, cx0, cx1, cx2, cx3;

				cx01 = buffc[2 * i];
				cx23 = buffc[2 * i + 1];
				cx1 = cx01 >> 16;
				cx0 = cx01 & 0xFFFF;
				cx3 = cx23 >> 16;
				cx2 = cx23 & 0xFFFF;

				ddx = buffx[i];
				rdx = _mm_sub_pi16(mask_7fff, ddx);

				LD_4_PIX_U16(sd0, sd1, tsp0, tsp1);
				FMUL_16x16(v0, sd0, rdx);
				FMUL_16x16(v1, sd1, ddx);
				y0 = _mm_add_pi16(v0, v1);
				y0 = MUL2(y0);

				LD_4_PIX_U16(sd2, sd3, tsp2, tsp3);
				FMUL_16x16(v2, sd2, rdx);
				FMUL_16x16(v3, sd3, ddx);
				y1 = _mm_add_pi16(v2, v3);
				y1 = MUL2(y1);

				FMUL_16x16(z0, y0, rdy);
				FMUL_16x16(z1, y1, ddy);
				dd = _mm_add_pi16(z0, z1);
				dd = MUL2(dd);

				buff0[i] = y0;
				buff1[i] = y1;
				XOR_8000(dd)
				    tdp[i] = dd;
			}
		}

		if ((3 * width) & 3) {
			__m64 mask =
			    ((__m64 *) mlib_mask64_arr)[2 * ((3 * width) & 3)];

			tdp[width3_4 - 1] =
			    _mm_or_si64(_mm_and_si64(mask, tdp[width3_4 - 1]),
			    _mm_andnot_si64(mask, tail));
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

mlib_status FUNC(
    4_Bilinear) (
    mlib_work_image * param)
{
	VARIABLE(mlib_s16)
	__m64 buff_arr[BUFF_SIZE / 8], *buff0, *buff1, *buffx;
	void *buff = buff_arr, *buffd, *row_0;
	__m64 *tdp;
	__m64 *tsp0, *tsp1, *tsp2, *tsp3;
	mlib_s32 x0, x1, cx0, yo;
	mlib_s32 size, row_size, y_step = 2;
	__m64 ddy, rdy;
	mlib_d64 cdx, zdx;
	__m64 mask_7fff;

	row_size = 8 * ((width * dx) >> MLIB_SHIFT) + 64;
	size = 8 * 4 * width + 2 * row_size;
	if (size > BUFF_SIZE) {
		buff = mlib_malloc(size);
		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + width;
	buffx = buff1 + width;
	buffd = buffx + width;
	row_0 = (__m64 *) buffd + width;

	cdx = x * 0.5;
	zdx = (1 << 15) / dzoomx;
	for (i = 0; i < width; i++) {
		mlib_s32 ires = ((mlib_s32)cdx) - ((x >> MLIB_SHIFT) << 15);

		if (ires >= (1 << 15))
			ires = (1 << 15) - 1;
		if (ires < 0)
			ires = 0;

		ires = (ires << 16) | ires;
		((mlib_s32 *)buffx)[2 * i] = ires;
		((mlib_s32 *)buffx)[2 * i + 1] = ires;
		cdx += zdx;
		x += dx;
		mlib_empty_func();
	}

	mask_7fff = mmx_to_double_dup(VAL_U16_ONE);
#ifdef TYPE_U16
	mask8000 = mmx_to_double_dup(0x80008000);
#endif

	for (j = 0; j < height; j++) {
		tdp = (__m64 *) dp;

		if (y_step == 0) {
			yo = ((y & 0xfffe) >> 1),
			    ddy = mmx_to_double_dup((yo << 16) | yo);
			rdy = _mm_sub_pi16(mask_7fff, ddy);

			for (i = 0; i < width; i++) {
				__m64 y0, y1, z0, z1, dd;

				y0 = buff0[i];
				y1 = buff1[i];

				FMUL_16x16(z0, y0, rdy);
				FMUL_16x16(z1, y1, ddy);
				dd = _mm_add_pi16(z0, z1);
				dd = MUL2(dd);

				XOR_8000(dd)
				    tdp[i] = dd;
			}

		} else if (y_step == 1) {
			__m64 *buffT;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			INIT_XLINE();
			tsp2 = (__m64 *) (tsp + src_stride);
			tsp3 = tsp2 + 1;

			for (i = 0; i < width; i++) {
				__m64 sd2, sd3, ddx, rdx, v2, v3, y0, y1, z0,
				    z1, dd;

				cx0 = (x0 >> MLIB_SHIFT);

				ddx = buffx[i];
				rdx = _mm_sub_pi16(mask_7fff, ddx);

				sd2 = tsp2[cx0];
				sd3 = tsp3[cx0];
				XOR_8000(sd2);
				XOR_8000(sd3);
				FMUL_16x16(v2, sd2, rdx);
				FMUL_16x16(v3, sd3, ddx);

				y0 = buff0[i];
				y1 = _mm_add_pi16(v2, v3);
				y1 = MUL2(y1);

				FMUL_16x16(z0, y0, rdy);
				FMUL_16x16(z1, y1, ddy);
				dd = _mm_add_pi16(z0, z1);
				dd = MUL2(dd);

				x0 += dx;

				buff1[i] = y1;
				XOR_8000(dd)
				    tdp[i] = dd;
			}

		} else {
			INIT_XLINE();
			tsp0 = (__m64 *) tsp;
			tsp2 = (__m64 *) (tsp + src_stride);
			tsp1 = tsp0 + 1;
			tsp3 = tsp2 + 1;

			for (i = 0; i < width; i++) {
				__m64 sd0, sd1, sd2, sd3, ddx, rdx;
				__m64 v0, v1, v2, v3, y0, y1, z0, z1, dd;

				cx0 = (x0 >> MLIB_SHIFT);

				ddx = buffx[i];
				rdx = _mm_sub_pi16(mask_7fff, ddx);

				sd0 = tsp0[cx0];
				sd1 = tsp1[cx0];
				sd2 = tsp2[cx0];
				sd3 = tsp3[cx0];
				XOR_8000(sd0);
				XOR_8000(sd1);
				XOR_8000(sd2);
				XOR_8000(sd3);

				FMUL_16x16(v0, sd0, rdx);
				FMUL_16x16(v1, sd1, ddx);
				y0 = _mm_add_pi16(v0, v1);
				y0 = MUL2(y0);

				FMUL_16x16(v2, sd2, rdx);
				FMUL_16x16(v3, sd3, ddx);
				y1 = _mm_add_pi16(v2, v3);
				y1 = MUL2(y1);

				FMUL_16x16(z0, y0, rdy);
				FMUL_16x16(z1, y1, ddy);
				dd = _mm_add_pi16(z0, z1);
				dd = MUL2(dd);

				x0 += dx;

				buff0[i] = y0;
				buff1[i] = y1;
				XOR_8000(dd)
				    tdp[i] = dd;
			}
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
