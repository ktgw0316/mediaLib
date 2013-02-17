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

#pragma ident	"@(#)mlib_v_ImageZoom_BL.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoom - image scaling with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoom(mlib_image       *dst,
 *                                 const mlib_image *src,
 *                                 mlib_f32         zoomx,
 *                                 mlib_f32         zoomy,
 *                                 mlib_filter      filter,
 *                                 mlib_edge        edge)
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

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageDivTables.h>
#include <mlib_v_ImageChannelExtract.h>

/* *********************************************************** */

#define	_MLIB_VIS_VER_

#include <mlib_ImageZoom.h>

/* *********************************************************** */

#define	FUNC_NAME(CC)	mlib_v_ImageZoomBilinear_U8_##CC

/* *********************************************************** */

#define	BUFF_SIZE	2048

/* *********************************************************** */

#define	LD_4_PIX_U8(a0, a1, tsp0, tsp1)                         \
	a0 = vis_faligndata(vis_ld_u8_i(tsp0, cx3), a0);        \
	a1 = vis_faligndata(vis_ld_u8_i(tsp1, cx3), a1);        \
	a0 = vis_faligndata(vis_ld_u8_i(tsp0, cx2), a0);        \
	a1 = vis_faligndata(vis_ld_u8_i(tsp1, cx2), a1);        \
	a0 = vis_faligndata(vis_ld_u8_i(tsp0, cx1), a0);        \
	a1 = vis_faligndata(vis_ld_u8_i(tsp1, cx1), a1);        \
	a0 = vis_faligndata(vis_ld_u8_i(tsp0, cx0), a0);        \
	a1 = vis_faligndata(vis_ld_u8_i(tsp1, cx0), a1)

/* *********************************************************** */

#define	LD_2_PIX_U16(a0, a1, tsp0, tsp1)                        \
	a0 = vis_faligndata(vis_ld_u16_i(tsp0, cx1), a0);       \
	a1 = vis_faligndata(vis_ld_u16_i(tsp1, cx1), a1);       \
	a0 = vis_faligndata(vis_ld_u16_i(tsp0, cx0), a0);       \
	a1 = vis_faligndata(vis_ld_u16_i(tsp1, cx0), a1)

/* *********************************************************** */

#define	INIT_XLINE()                                                    \
	x = GetElemSubStruct(current, srcX) & MLIB_MASK;                \
	x0 = x;                                                         \
/*                                                                      \
 * x1 = x0 + dx;                                                        \
 */                                                                     \
	yo = (y >> 8) & 0xFF;                                           \
	fdy = vis_to_float((yo << 24) | (yo << 16) | (yo << 8) | yo);   \
	tsp0 = tsp;                                                     \
	tsp2 = tsp0 + src_stride

/* *********************************************************** */

#undef   VARIABLE
#define	VARIABLE(FORMAT)                                             \
	FORMAT *sp = GetElemSubStruct(current, sp),                  \
		*dl = GetElemSubStruct(current, dp),                 \
		*tsp = sp;                                           \
	mlib_s32 i, j,                                               \
		dx = GetElemStruct(DX),                              \
		dy = GetElemStruct(DY),                              \
		x = GetElemSubStruct(current, srcX) & MLIB_MASK,     \
		y = GetElemSubStruct(current, srcY) & MLIB_MASK,     \
		src_stride = GetElemStruct(src_stride),              \
		dst_stride = GetElemStruct(dst_stride),              \
		width  = GetElemSubStruct(current, width),           \
		height = GetElemSubStruct(current, height)

/* *********************************************************** */

mlib_status FUNC_NAME(1)  (
    mlib_work_image *param)
{
	VARIABLE(mlib_u8);
	mlib_d64 buff_arr[BUFF_SIZE / 8], *buff0, *buff1, *buffx;
	void *buff = buff_arr, *buffd;
	mlib_f32 *tdp, fdy;
	mlib_u8 *tsp0, *tsp1, *tsp2, *tsp3;
	mlib_s32 x0, x1, x2, x3, cx0, cx1, cx2, cx3, yo, x_max, dx4 = dx * 4;
	mlib_s32 width4, size, y_step = 2;
	mlib_d64 ddx, dx64;
	mlib_d64 mask_7fff = vis_to_double_dup(0x7FFF7FFF);
	mlib_d64 k05 = vis_to_double_dup(0x00400040);

	width4 = (width + 3) / 4;

	size = 7 * 4 * width4 + 8;

	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + width4;
	buffx = buff1 + width4;
	buffd = buffx + width4;

	x0 = GetElemSubStruct(current, srcX) & MLIB_MASK;
	x1 = x0 + dx;
	x2 = x0 + (dx << 1);
	x3 = x1 + (dx << 1);

/* calculate dx for all row */
	dx64 =
	    vis_to_double_dup(((dx4 & 0xFFFE) << 15) | ((dx4 & 0xFFFE) >> 1));
	ddx =
	    vis_to_double(((x0 & 0xFFFE) << 15) | ((x1 & 0xFFFE) >> 1),
	    ((x2 & 0xFFFE) << 15) | ((x3 & 0xFFFE) >> 1));
#pragma pipeloop(0)
	for (i = 0; i < width4; i++) {
		buffx[i] = ddx;
		ddx = vis_fand(vis_fpadd16(ddx, dx64), mask_7fff);
	}

	x_max = (param->sline_size - 1) << MLIB_SHIFT;

	for (j = 0; j < height; j++) {

		vis_write_gsr(7);

		if (((mlib_addr)dl & 3) || (width & 3)) {
			tdp = (mlib_f32 *)buffd;
		} else {
			tdp = (mlib_f32 *)dl;
		}

		if (y_step == 0) {
			mlib_d64 ddy;

			yo = (y >> 8) & 0xFF;
			ddy =
			    vis_to_double_dup((yo << 24) | (yo << 16) | (yo <<
			    8) | yo);

#pragma pipeloop(0)
			for (i = 0; i < width4; i++) {
				mlib_d64 y0, y1, y10, z10, dd;

				y0 = buff0[i];
				y1 = buff1[i];

				y10 = vis_fpsub16(y1, y0);
				z10 = vis_fmul8x16(vis_read_hi(ddy), y10);
				dd = vis_fpadd16(y0, z10);
				dd = vis_fpadd16(dd, k05);

				tdp[i] = vis_fpack16(dd);
			}

		} else if (y_step == 1) {
			mlib_d64 *buffT;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			INIT_XLINE();
			x1 = x0 + dx;
			x2 = x0 + (dx << 1);
			x3 = x1 + (dx << 1);
			tsp3 = tsp2 + 1;

#pragma pipeloop(0)
			for (i = 0; i < width4; i++) {
				mlib_d64 ddx, rdx, sd2, sd3, v2, v3, y0, y1,
				    y10, z10, dd;

				cx0 = (x0 >> MLIB_SHIFT);
				cx1 = (x1 >> MLIB_SHIFT);
				cx2 = (x2 >> MLIB_SHIFT);
				cx3 = (x3 >> MLIB_SHIFT);

				cx1 &= ((x1 - x_max) >> 31);
				cx2 &= ((x2 - x_max) >> 31);
				cx3 &= ((x3 - x_max) >> 31);

				ddx = buffx[i];
				rdx = vis_fpsub16(mask_7fff, ddx);

				LD_4_PIX_U8(sd2, sd3, tsp2, tsp3);
				v2 = vis_fmul8x16(vis_read_hi(sd2), rdx);
				v3 = vis_fmul8x16(vis_read_hi(sd3), ddx);

				y0 = buff0[i];
				y1 = vis_fpadd16(v2, v3);

				y10 = vis_fpsub16(y1, y0);
				z10 = vis_fmul8x16(fdy, y10);
				dd = vis_fpadd16(y0, z10);
				dd = vis_fpadd16(dd, k05);

				x0 += dx4;
				x1 += dx4;
				x2 += dx4;
				x3 += dx4;

				buff1[i] = y1;
				tdp[i] = vis_fpack16(dd);
			}

		} else {

			INIT_XLINE();
			x1 = x0 + dx;
			x2 = x0 + (dx << 1);
			x3 = x1 + (dx << 1);
			tsp1 = tsp0 + 1;
			tsp3 = tsp2 + 1;

#pragma pipeloop(0)
			for (i = 0; i < width4; i++) {
				mlib_d64 ddx, rdx, sd0, sd1, sd2, sd3, v0, v1,
				    v2, v3, y0, y1, y10, z10, dd;

				cx0 = (x0 >> MLIB_SHIFT);
				cx1 = (x1 >> MLIB_SHIFT);
				cx2 = (x2 >> MLIB_SHIFT);
				cx3 = (x3 >> MLIB_SHIFT);

				cx1 &= ((x1 - x_max) >> 31);
				cx2 &= ((x2 - x_max) >> 31);
				cx3 &= ((x3 - x_max) >> 31);

				ddx = buffx[i];
				rdx = vis_fpsub16(mask_7fff, ddx);

				LD_4_PIX_U8(sd0, sd1, tsp0, tsp1);
				v0 = vis_fmul8x16(vis_read_hi(sd0), rdx);
				v1 = vis_fmul8x16(vis_read_hi(sd1), ddx);

				LD_4_PIX_U8(sd2, sd3, tsp2, tsp3);
				v2 = vis_fmul8x16(vis_read_hi(sd2), rdx);
				v3 = vis_fmul8x16(vis_read_hi(sd3), ddx);

				y0 = vis_fpadd16(v0, v1);
				y1 = vis_fpadd16(v2, v3);

				y10 = vis_fpsub16(y1, y0);
				z10 = vis_fmul8x16(fdy, y10);
				dd = vis_fpadd16(y0, z10);
				dd = vis_fpadd16(dd, k05);

				x0 += dx4;
				x1 += dx4;
				x2 += dx4;
				x3 += dx4;

				buff0[i] = y0;
				buff1[i] = y1;
				tdp[i] = vis_fpack16(dd);
			}
		}

		if ((mlib_u8 *)tdp != dl)
			mlib_ImageCopy_na((mlib_u8 *)tdp, dl, width);

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		dl += dst_stride;
		y += dy;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	if (size > BUFF_SIZE) {
		__mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(2)  (
    mlib_work_image *param)
{
	VARIABLE(mlib_u8);
	mlib_d64 buff_arr[BUFF_SIZE / 8], *buff0, *buff1, *buffx;
	void *buff = buff_arr, *buffd, *row_0, *row_1;
	mlib_f32 *tdp, fdy;
	mlib_u8 *tsp0, *tsp1, *tsp2, *tsp3;
	mlib_s32 x0, x1, cx0, cx1, yo, x_max, dx2 = dx * 2;
	mlib_s32 width2, size, row_size, y_step = 2;
	mlib_d64 ddx, dx64;
	mlib_d64 mask_7fff = vis_to_double_dup(0x7FFF7FFF);
	mlib_d64 k05 = vis_to_double_dup(0x00400040);

	width2 = (width + 1) / 2;

	row_size = 2 * param->sline_size;

	size = 7 * 4 * width2 + 2 * row_size;

	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + width2;
	buffx = buff1 + width2;
	buffd = buffx + width2;
	row_0 = (mlib_f32 *)buffd + width2;
	row_1 = (mlib_u8 *)row_0 + row_size;

	x0 = GetElemSubStruct(current, srcX) & MLIB_MASK;
	x1 = x0 + dx;

/* calculate dx for all row */
	dx64 =
	    vis_to_double_dup(((dx2 & 0xFFFE) << 15) | ((dx2 & 0xFFFE) >> 1));
	ddx =
	    vis_to_double(((x0 & 0xFFFE) << 15) | ((x0 & 0xFFFE) >> 1),
	    ((x1 & 0xFFFE) << 15) | ((x1 & 0xFFFE) >> 1));
#pragma pipeloop(0)
	for (i = 0; i < width2; i++) {
		buffx[i] = ddx;
		ddx = vis_fand(vis_fpadd16(ddx, dx64), mask_7fff);
	}

	x_max = (param->sline_size - 1) << MLIB_SHIFT;

	for (j = 0; j < height; j++) {

		if (((mlib_addr)dl & 3) || (width & 1)) {
			tdp = (mlib_f32 *)buffd;
		} else {
			tdp = (mlib_f32 *)dl;
		}

		if (y_step == 0) {
			mlib_d64 ddy;

			vis_write_gsr(6);

			yo = (y >> 8) & 0xFF;
			ddy =
			    vis_to_double_dup((yo << 24) | (yo << 16) | (yo <<
			    8) | yo);

#pragma pipeloop(0)
			for (i = 0; i < width2; i++) {
				mlib_d64 y0, y1, y10, z10, dd;

				y0 = buff0[i];
				y1 = buff1[i];

				y10 = vis_fpsub16(y1, y0);
				z10 = vis_fmul8x16(vis_read_hi(ddy), y10);
				dd = vis_fpadd16(y0, z10);
				dd = vis_fpadd16(dd, k05);

				tdp[i] = vis_fpack16(dd);
			}

		} else if (y_step == 1) {
			mlib_d64 *buffT;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			INIT_XLINE();
			x1 = x0 + dx;

			if ((mlib_addr)tsp2 & 1) {
				mlib_ImageCopy_na((void *)tsp2, row_1,
				    row_size);
				tsp2 = row_1;
			}

			tsp3 = tsp2 + 2;

			vis_write_gsr(6);

#pragma pipeloop(0)
			for (i = 0; i < width2; i++) {
				mlib_d64 ddx, rdx, sd2, sd3, v2, v3, y0, y1,
				    y10, z10, dd;

				cx0 = (x0 >> MLIB_SHIFT);
				cx1 = (x1 >> MLIB_SHIFT);
				cx1 &= (x1 - x_max) >> 31;
				cx0 += cx0;
				cx1 += cx1;

				ddx = buffx[i];
				rdx = vis_fpsub16(mask_7fff, ddx);

				LD_2_PIX_U16(sd2, sd3, tsp2, tsp3);
				v2 = vis_fmul8x16(vis_read_hi(sd2), rdx);
				v3 = vis_fmul8x16(vis_read_hi(sd3), ddx);

				y0 = buff0[i];
				y1 = vis_fpadd16(v2, v3);

				y10 = vis_fpsub16(y1, y0);
				z10 = vis_fmul8x16(fdy, y10);
				dd = vis_fpadd16(y0, z10);
				dd = vis_fpadd16(dd, k05);

				x0 += dx2;
				x1 += dx2;

				buff1[i] = y1;
				tdp[i] = vis_fpack16(dd);
			}

		} else {

			INIT_XLINE();
			x1 = x0 + dx;

			if ((mlib_addr)tsp0 & 1) {
				mlib_ImageCopy_na((void *)tsp0, row_0,
				    row_size);
				tsp0 = row_0;
			}

			tsp1 = tsp0 + 2;

			if ((mlib_addr)tsp2 & 1) {
				mlib_ImageCopy_na((void *)tsp2, row_1,
				    row_size);
				tsp2 = row_1;
			}

			tsp3 = tsp2 + 2;

			vis_write_gsr(6);

#pragma pipeloop(0)
			for (i = 0; i < width2; i++) {
				mlib_d64 ddx, rdx, sd0, sd1, sd2, sd3, v0, v1,
				    v2, v3, y0, y1, y10, z10, dd;

				cx0 = (x0 >> MLIB_SHIFT);
				cx1 = (x1 >> MLIB_SHIFT);
				cx1 &= (x1 - x_max) >> 31;
				cx0 += cx0;
				cx1 += cx1;

				ddx = buffx[i];
				rdx = vis_fpsub16(mask_7fff, ddx);

				LD_2_PIX_U16(sd0, sd1, tsp0, tsp1);
				v0 = vis_fmul8x16(vis_read_hi(sd0), rdx);
				v1 = vis_fmul8x16(vis_read_hi(sd1), ddx);

				LD_2_PIX_U16(sd2, sd3, tsp2, tsp3);
				v2 = vis_fmul8x16(vis_read_hi(sd2), rdx);
				v3 = vis_fmul8x16(vis_read_hi(sd3), ddx);

				y0 = vis_fpadd16(v0, v1);
				y1 = vis_fpadd16(v2, v3);

				y10 = vis_fpsub16(y1, y0);
				z10 = vis_fmul8x16(fdy, y10);
				dd = vis_fpadd16(y0, z10);
				dd = vis_fpadd16(dd, k05);

				x0 += dx2;
				x1 += dx2;

				buff0[i] = y0;
				buff1[i] = y1;
				tdp[i] = vis_fpack16(dd);
			}
		}

		if ((mlib_u8 *)tdp != dl)
			mlib_ImageCopy_na((mlib_u8 *)tdp, dl, 2 * width);

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		dl += dst_stride;
		y += dy;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	if (size > BUFF_SIZE) {
		__mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	BLEND33                                                 \
	s1 = (*bp++);                                           \
	ss = vis_faligndata(s0, s1);                            \
	s0 = s1;                                                \
	dd = *(mlib_d64 *)dp;                                   \
	d0 =                                                    \
	vis_fpadd16(vis_fmul8x16au(vis_read_hi(ss), aa),        \
	    vis_fmul8x16al(vis_read_hi(dd), aa));               \
	d1 =                                                    \
	vis_fpadd16(vis_fmul8x16au(vis_read_lo(ss), aa),        \
	    vis_fmul8x16al(vis_read_lo(dd), aa));               \
	dd = vis_fpack16_pair(d0, d1)

/* *********************************************************** */

#define	BLEND43(hh1, hh2)                                         \
	ss = ((mlib_f32 *)buffd)[i];                              \
	a1 = vis_fpsub16(done, vis_fmul8x16al(ss, aa));           \
	dd =                                                      \
	vis_fmul8x16##hh2(vis_read_hi(dd), vis_read_##hh1(a1));   \
	dd = vis_fpadd16(dd, vis_fmul8x16au(ss, aa));             \
	((mlib_f32 *)buffd)[i] = vis_fpack16(dd);                 \
	dp += 3

/* *********************************************************** */

#define	FP_TYPE	mlib_d64
#define	F_ONE	((FP_TYPE)1)
#define	I2D(x)	mlib_U82D64[x]

/* *********************************************************** */

#define	BLEND34(dst, fres)                                      \
	dst += (mlib_s32)((I2D(fres) - I2D(dst)) * rw)

#define	BLEND34z(dst, fres)	dst = (w0*I2D(fres) + w1*I2D(dst))*rw

/* *********************************************************** */

#if MLIB_VIS >= 0x200

#define	BLEND44(dst, fres)                                              \
	dst += (mlib_s32)((dalpha0 * I2D(fres) - w0 * I2D(dst)) * rw)

#else

#define	BLEND44(dst, fres)                                        \
	dst += (mlib_s32)((dalpha0 * (fres) - w0 * (dst)) * rw)

#endif

#define	BLEND44z(dst, fres)                                     \
	dst = (dalpha0 * I2D(fres) + w1 * I2D(dst)) * rw

/* *********************************************************** */

#define	DIV_ALPHA(ss, alpha)                                            \
	vis_fmul8x16(ss, ((mlib_d64 *)mlib_v_tbl_255DivAlpha)[alpha])

/* *********************************************************** */

mlib_status FUNC_NAME(3)  (
    mlib_work_image *param)
{
	VARIABLE(mlib_u8);
	mlib_d64 buff_arr[BUFF_SIZE / 8], *buff0, *buff1, *buffx;
	void *buff = buff_arr, *buffd;
	mlib_f32 *tdp, fdy;
	mlib_u8 *tsp0, *tsp1, *tsp2, *tsp3;
	mlib_u32 cx, x0, x1, x2, x3, yo, dx4 = dx * 4;
	mlib_s32 alp_ind, chan_d;
	mlib_s32 width4, size, y_step = 2;
	mlib_d64 dx_0, dx_1, dx_2, dx64;
	mlib_d64 mask_7fff = vis_to_double_dup(0x7FFF7FFF);
	mlib_d64 k05 = vis_to_double_dup(0x00400040);
	mlib_s32 *buffc;
	mlib_u16 *buffcc;
	mlib_f32 aa;
	mlib_blend blend;

	alp_ind = param->alp_ind;

	if (alp_ind) {
		mlib_s32 alp = (param->alpha) * (16384.0 / 255);

		((mlib_s32 *)&aa)[0] = (alp << 16) | (16384 - alp);
		chan_d = param->chan_d;
		blend = param->blend;
	}

	width4 = (width + 3) / 4;

	size = 9 * 12 * width4 + 32;

	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	x0 = GetElemSubStruct(current, srcX) & MLIB_MASK;
	x1 = x0 + dx;
	x2 = x0 + (dx << 1);
	x3 = x1 + (dx << 1);

/* calculate dx for all row */
	dx64 =
	    vis_to_double_dup(((dx4 & 0xFFFE) << 15) | ((dx4 & 0xFFFE) >> 1));
	dx_0 =
	    vis_to_double(((x0 & 0xFFFE) << 15) | ((x0 & 0xFFFE) >> 1),
	    ((x0 & 0xFFFE) << 15) | ((x1 & 0xFFFE) >> 1));
	dx_1 =
	    vis_to_double(((x1 & 0xFFFE) << 15) | ((x1 & 0xFFFE) >> 1),
	    ((x2 & 0xFFFE) << 15) | ((x2 & 0xFFFE) >> 1));
	dx_2 =
	    vis_to_double(((x2 & 0xFFFE) << 15) | ((x3 & 0xFFFE) >> 1),
	    ((x3 & 0xFFFE) << 15) | ((x3 & 0xFFFE) >> 1));

	buffx = buff;
	buff0 = buffx + 3 * width4;
	buff1 = buff0 + 3 * width4;
	buffd = buff1 + 3 * width4;
	buffc = (mlib_s32 *)buffd + 3 * width4;

#pragma pipeloop(0)
	for (i = 0; i < width4; i++) {
		buffx[3 * i] = dx_0;
		buffx[3 * i + 1] = dx_1;
		buffx[3 * i + 2] = dx_2;
		dx_0 = vis_fand(vis_fpadd16(dx_0, dx64), mask_7fff);
		dx_1 = vis_fand(vis_fpadd16(dx_1, dx64), mask_7fff);
		dx_2 = vis_fand(vis_fpadd16(dx_2, dx64), mask_7fff);
	}

	x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	buffcc = (mlib_u16 *)buffc;
#pragma pipeloop(0)
	for (i = 0; i < width; i++) {
		cx = (x >> MLIB_SHIFT);
		cx += 2 * cx;
		buffcc[3 * i] = cx;
		buffcc[3 * i + 1] = cx + 1;
		buffcc[3 * i + 2] = cx + 2;
		x += dx;
	}

	for (; i < 4 * width4; i++) {
		buffcc[3 * i] = cx;
		buffcc[3 * i + 1] = cx + 1;
		buffcc[3 * i + 2] = cx + 2;
		x += dx;
	}

	for (j = 0; j < height; j++) {

		vis_write_gsr(7);

		if (((mlib_addr)dl & 3) || (width & 3) || alp_ind) {
			tdp = (mlib_f32 *)buffd;
		} else {
			tdp = (mlib_f32 *)dl;
		}

		if (y_step == 0) {
			mlib_d64 ddy;

			yo = (y >> 8) & 0xFF;
			ddy =
			    vis_to_double_dup((yo << 24) | (yo << 16) | (yo <<
			    8) | yo);

#pragma pipeloop(0)
			for (i = 0; i < 3 * width4; i++) {
				mlib_d64 y0, y1, y10, z10, dd;

				y0 = buff0[i];
				y1 = buff1[i];

				y10 = vis_fpsub16(y1, y0);
				z10 = vis_fmul8x16(vis_read_hi(ddy), y10);
				dd = vis_fpadd16(y0, z10);
				dd = vis_fpadd16(dd, k05);

				tdp[i] = vis_fpack16(dd);
			}

		} else if (y_step == 1) {
			mlib_d64 *buffT;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			yo = (y >> 8) & 0xFF;
			fdy =
			    vis_to_float((yo << 24) | (yo << 16) | (yo << 8) |
			    yo);

			tsp2 = tsp + src_stride;
			tsp3 = tsp2 + 3;

#pragma pipeloop(0)
			for (i = 0; i < 3 * width4; i++) {
				mlib_d64 ddx, rdx, sd2, sd3, v2, v3, y0, y1,
				    y10, z10, dd;
				mlib_u32 cx01, cx23, cx0, cx1, cx2, cx3;

				cx01 = buffc[2 * i];
				cx23 = buffc[2 * i + 1];
				cx0 = cx01 >> 16;
				cx1 = cx01 & 0xFFFF;
				cx2 = cx23 >> 16;
				cx3 = cx23 & 0xFFFF;

				ddx = buffx[i];
				rdx = vis_fpsub16(mask_7fff, ddx);

				LD_4_PIX_U8(sd2, sd3, tsp2, tsp3);
				v2 = vis_fmul8x16(vis_read_hi(sd2), rdx);
				v3 = vis_fmul8x16(vis_read_hi(sd3), ddx);

				y0 = buff0[i];
				y1 = vis_fpadd16(v2, v3);

				y10 = vis_fpsub16(y1, y0);
				z10 = vis_fmul8x16(fdy, y10);
				dd = vis_fpadd16(y0, z10);
				dd = vis_fpadd16(dd, k05);

				buff1[i] = y1;
				tdp[i] = vis_fpack16(dd);
			}

		} else {
			yo = (y >> 8) & 0xFF;
			fdy =
			    vis_to_float((yo << 24) | (yo << 16) | (yo << 8) |
			    yo);

			tsp0 = tsp;
			tsp1 = tsp0 + 3;
			tsp2 = tsp0 + src_stride;
			tsp3 = tsp2 + 3;

#pragma pipeloop(0)
			for (i = 0; i < 3 * width4; i++) {
				mlib_d64 ddx, rdx, sd0, sd1, sd2, sd3, v0, v1,
				    v2, v3, y0, y1, y10, z10, dd;
				mlib_u32 cx01, cx23, cx0, cx1, cx2, cx3;

				cx01 = buffc[2 * i];
				cx23 = buffc[2 * i + 1];
				cx0 = cx01 >> 16;
				cx1 = cx01 & 0xFFFF;
				cx2 = cx23 >> 16;
				cx3 = cx23 & 0xFFFF;

				ddx = buffx[i];
				rdx = vis_fpsub16(mask_7fff, ddx);

				LD_4_PIX_U8(sd0, sd1, tsp0, tsp1);
				v0 = vis_fmul8x16(vis_read_hi(sd0), rdx);
				v1 = vis_fmul8x16(vis_read_hi(sd1), ddx);

				LD_4_PIX_U8(sd2, sd3, tsp2, tsp3);
				v2 = vis_fmul8x16(vis_read_hi(sd2), rdx);
				v3 = vis_fmul8x16(vis_read_hi(sd3), ddx);

				y0 = vis_fpadd16(v0, v1);
				y1 = vis_fpadd16(v2, v3);

				y10 = vis_fpsub16(y1, y0);
				z10 = vis_fmul8x16(fdy, y10);
				dd = vis_fpadd16(y0, z10);
				dd = vis_fpadd16(dd, k05);

				buff0[i] = y0;
				buff1[i] = y1;
				tdp[i] = vis_fpack16(dd);
			}
		}

		if (!alp_ind) {
			if ((mlib_u8 *)tdp != dl) {
				mlib_ImageCopy_na((mlib_u8 *)tdp, dl,
				    3 * width);
			}

		} else if (chan_d == 3) {	/* ZoomBlend */
			mlib_u8 *dp, *dend = dl + 3 * width;
			mlib_s32 off = (mlib_s32)dl & 7;
			mlib_d64 s0, s1, ss, d0, d1, dd, *bp;

			vis_write_gsr(1 << 3);

			dp = dl - off;
			bp = vis_alignaddr((mlib_u8 *)buffd - off, 0);
			s0 = (*bp++);

			if (off) {
				BLEND33;
				vis_pst_8(dd, dp, vis_edge8(dl, dend - 1));
				dp += 8;
			}
#pragma pipeloop(0)
			for (; dp <= dend - 8; dp += 8) {
				BLEND33;
				*(mlib_d64 *)dp = dd;
			}

			if (dp < dend) {
				BLEND33;
				vis_pst_8(dd, dp, vis_edge8(dp, dend - 1));
			}

		} else {	/* ZoomBlend */
			mlib_u8 *sp = buffd;
			FP_TYPE w0 = param->alpha;
			FP_TYPE w1s = F_ONE - w0 * (F_ONE / 255);
			FP_TYPE w1, w, rw;

			if (blend == MLIB_BLEND_GTK_SRC) {
#pragma pipeloop(0)
				for (i = 0; i < width; i++) {
					dl[4 * i] = sp[3 * i];
					dl[4 * i + 1] = sp[3 * i + 1];
					dl[4 * i + 2] = sp[3 * i + 2];
					dl[4 * i + alp_ind] = 255;
				}
			} else if (blend == MLIB_BLEND_GTK_SRC_OVER2) {
#pragma pipeloop(0)
				for (i = 0; i < width; i++) {
					w1 = w1s * I2D(dl[4 * i + alp_ind]);
					w = w0 + w1;
					FP_DIV(rw, w0, w);

					BLEND34(dl[4 * i], sp[3 * i]);
					BLEND34(dl[4 * i + 1], sp[3 * i + 1]);
					BLEND34(dl[4 * i + 2], sp[3 * i + 2]);
					dl[4 * i + alp_ind] = w;
				}
			} else {
#pragma pipeloop(0)
				for (i = 0; i < width; i++) {
					w1 = w1s * I2D(dl[4 * i + alp_ind]);
					w = w0 + w1;
					FP_INV(rw, w);

					BLEND34z(dl[4 * i], sp[3 * i]);
					BLEND34z(dl[4 * i + 1], sp[3 * i + 1]);
					BLEND34z(dl[4 * i + 2], sp[3 * i + 2]);
					dl[4 * i + alp_ind] = w;
				}
			}
		}

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		dl += dst_stride;
		y += dy;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	if (size > BUFF_SIZE) {
		__mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	EXPAND0(x)	vis_fpmerge(fzeros, x)

#define	MUL_ALPHA_4CH(sp, dp, h1, h2)                           \
	{                                                       \
	    mlib_d64 ss, a0, a1, d0, d1;                        \
	                                                        \
	    ss = vis_freg_pair(((mlib_f32 *)sp)[2 * i],         \
		((mlib_f32 *)sp)[2 * i + 1]);                   \
	    a0 = EXPAND0(vis_read_hi(ss));                      \
	    a1 = EXPAND0(vis_read_lo(ss));                      \
	    d0 = vis_fmul8x16##h2(vis_read_hi(ss),              \
		vis_read_##h1(a0));                             \
	    d1 = vis_fmul8x16##h2(vis_read_lo(ss),              \
		vis_read_##h1(a1));                             \
	    d0 = vis_fpack16_pair(d0, d1);                      \
	    vis_pst_8(d0, (mlib_d64 *)dp + i, ~cmask);          \
	    vis_pst_8(ss, (mlib_d64 *)dp + i, cmask);           \
	}

/* *********************************************************** */

mlib_status FUNC_NAME(4)  (
    mlib_work_image *param)
{
	VARIABLE(mlib_u8);
	mlib_d64 buff_arr[BUFF_SIZE / 8], *buff0, *buff1, *buffx;
	void *buff = buff_arr, *buffd, *row_0, *row_1;
	mlib_f32 *tdp, fdy;
	mlib_u8 *tsp0, *tsp1, *tsp2, *tsp3;
	mlib_s32 x0, cx0, yo, x_max;
	mlib_s32 alp_ind, chan_d;
	mlib_s32 size, row_size, row_size8, y_step, next_step = 2;
	mlib_d64 dx64, dx0, dx1;
	mlib_d64 mask_7fff = vis_to_double_dup(0x7FFF7FFF);
	mlib_d64 k05 = vis_to_double_dup(0x00400040);
	mlib_d64 done = vis_to_double_dup((16384 << 16) | 16384);
	mlib_f32 aa, fzeros = vis_fzeros();
	mlib_blend blend = param->blend;

	alp_ind = param->alp_ind;

	if (alp_ind) {
		mlib_s32 dalpha = (param->alpha) * (16384.0 / 255);

		aa = vis_to_float(((mlib_s32)dalpha << 16) | (mlib_s32)(dalpha *
		    (256.0 / 255)));
		chan_d = param->chan_d;

		if (alp_ind == -1) {
			tsp = sp -= 1;
			if (blend == MLIB_BLEND_GTK_SRC && chan_d == 4)
				dl--;
		}
	}

	row_size = 4 * param->sline_size;
	row_size8 = (row_size + 7) / 8;
	size = 2 * 8 * row_size8 + (3 * 8 + 4) * width + 4 * 8;

	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	row_0 = buff;
	row_1 = (mlib_d64 *)row_0 + row_size8;
	buff0 = (mlib_d64 *)row_1 + row_size8;
	buff1 = buff0 + width;
	buffx = buff1 + width;
	buffd = buffx + width;

	x0 = GetElemSubStruct(current, srcX) & MLIB_MASK;

/* calculate dx for all row */
	dx64 = vis_to_double_dup(((dx & 0x7FFF) << 16) | (dx & 0x7FFF));
	dx0 = vis_to_double(((x0 & 0xFFFE) << 15) | ((x0 & 0xFFFE) >> 1),
	    ((x0 & 0xFFFE) << 15) | ((x0 & 0xFFFE) >> 1));
	x0 += dx;
	dx1 = vis_to_double(((x0 & 0xFFFE) << 15) | ((x0 & 0xFFFE) >> 1),
	    ((x0 & 0xFFFE) << 15) | ((x0 & 0xFFFE) >> 1));

#pragma pipeloop(0)
	for (i = 0; i <= width - 2; i += 2) {
		buffx[i] = dx0;
		buffx[i + 1] = dx1;
		dx0 = vis_fand(vis_fpadd16(dx0, dx64), mask_7fff);
		dx1 = vis_fand(vis_fpadd16(dx1, dx64), mask_7fff);
	}

	if (i < width) {
		buffx[i] = dx0;
	}

	for (j = 0; j < height; j++) {

		if ((mlib_addr)dl & 3 || alp_ind) {
			tdp = (mlib_f32 *)buffd;
		} else {
			tdp = (mlib_f32 *)dl;
		}

		y_step = next_step;
		next_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);

		if (y_step == 0) {
			mlib_d64 ddy;

			yo = (y >> 8) & 0xFF;
			ddy =
			    vis_to_double_dup((yo << 24) | (yo << 16) | (yo <<
			    8) | yo);

			vis_write_gsr(0);

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				mlib_d64 y0, y1, y10, z10, dd;

				y0 = buff0[i];
				y1 = buff1[i];

				y10 = vis_fpsub16(y1, y0);
				z10 = vis_fmul8x16(vis_read_hi(ddy), y10);
				dd = vis_fpadd16(y0, z10);
				dd = vis_fpadd16(dd, k05);

				tdp[i] = vis_fpack16(dd);
			}

		} else if (y_step == 1) {
			mlib_d64 *buffT;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			INIT_XLINE();

			vis_write_gsr(7 << 3);

			if ((mlib_addr)tsp2 & 3) {
				mlib_ImageCopy_na((void *)tsp2, row_1,
				    row_size);
				tsp2 = row_1;
			}
			if (alp_ind) {
				if (alp_ind == 3) {
					mlib_s32 cmask = 1 | (1 << 4);

#pragma pipeloop(0)
					for (i = 0; i < row_size8; i++) {
						MUL_ALPHA_4CH(tsp2, row_1, lo,
						    al);
					}
				} else {
					mlib_s32 cmask = (1 << 3) | (1 << 7);

#pragma pipeloop(0)
					for (i = 0; i < row_size8; i++) {
						MUL_ALPHA_4CH(tsp2, row_1, hi,
						    au);
					}
				}
				tsp2 = row_1;
			}
			tsp3 = tsp2 + 4;

			vis_write_gsr(0);

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				mlib_d64 ddx, rdx, v2, v3, y0, y1, y10, z10, dd;
				mlib_f32 sf2, sf3;

				cx0 = (x0 >> MLIB_SHIFT);

				ddx = buffx[i];
				rdx = vis_fpsub16(mask_7fff, ddx);

				sf2 = ((mlib_f32 *)tsp2)[cx0];
				sf3 = ((mlib_f32 *)tsp3)[cx0];
				v2 = vis_fmul8x16(sf2, rdx);
				v3 = vis_fmul8x16(sf3, ddx);

				y0 = buff0[i];
				y1 = vis_fpadd16(v2, v3);

				y10 = vis_fpsub16(y1, y0);
				z10 = vis_fmul8x16(fdy, y10);
				dd = vis_fpadd16(y0, z10);
				dd = vis_fpadd16(dd, k05);

				x0 += dx;

				buff1[i] = y1;
				tdp[i] = vis_fpack16(dd);
			}

		} else {
			INIT_XLINE();

			vis_write_gsr(7 << 3);

			if ((mlib_addr)tsp0 & 3) {
				mlib_ImageCopy_na((void *)tsp0, row_0,
				    row_size);
				tsp0 = row_0;
			}
			if (alp_ind) {
				if (alp_ind == 3) {
					mlib_s32 cmask = 1 | (1 << 4);

#pragma pipeloop(0)
					for (i = 0; i < row_size8; i++) {
						MUL_ALPHA_4CH(tsp0, row_0, lo,
						    al);
					}
				} else {
					mlib_s32 cmask = (1 << 3) | (1 << 7);

#pragma pipeloop(0)
					for (i = 0; i < row_size8; i++) {
						MUL_ALPHA_4CH(tsp0, row_0, hi,
						    au);
					}
				}
				tsp0 = row_0;
			}
			tsp1 = tsp0 + 4;

			if ((mlib_addr)tsp2 & 3) {
				mlib_ImageCopy_na((void *)tsp2, row_1,
				    row_size);
				tsp2 = row_1;
			}
			if (alp_ind) {
				if (alp_ind == 3) {
					mlib_s32 cmask = 1 | (1 << 4);

#pragma pipeloop(0)
					for (i = 0; i < row_size8; i++) {
						MUL_ALPHA_4CH(tsp2, row_1, lo,
						    al);
					}
				} else {
					mlib_s32 cmask = (1 << 3) | (1 << 7);

#pragma pipeloop(0)
					for (i = 0; i < row_size8; i++) {
						MUL_ALPHA_4CH(tsp2, row_1, hi,
						    au);
					}
				}
				tsp2 = row_1;
			}
			tsp3 = tsp2 + 4;

			vis_write_gsr(0);

/* without write to buff0 and buff1 */
			if (next_step >= 2) {
#pragma pipeloop(0)
				for (i = 0; i < width; i++) {
					mlib_d64 ddx, rdx;
					mlib_d64 v0, v1, v2, v3, y0, y1, y10,
					    z10, dd;
					mlib_f32 sf0, sf1, sf2, sf3;

					cx0 = (x0 >> MLIB_SHIFT);

					sf0 = ((mlib_f32 *)tsp0)[cx0];
					sf1 = ((mlib_f32 *)tsp1)[cx0];
					sf2 = ((mlib_f32 *)tsp2)[cx0];
					sf3 = ((mlib_f32 *)tsp3)[cx0];

					ddx = buffx[i];
					rdx = vis_fpsub16(mask_7fff, ddx);

					v0 = vis_fmul8x16(sf0, rdx);
					v1 = vis_fmul8x16(sf1, ddx);
					v2 = vis_fmul8x16(sf2, rdx);
					v3 = vis_fmul8x16(sf3, ddx);

					y0 = vis_fpadd16(v0, v1);
					y1 = vis_fpadd16(v2, v3);

					y10 = vis_fpsub16(y1, y0);
					z10 = vis_fmul8x16(fdy, y10);
					dd = vis_fpadd16(y0, z10);
					dd = vis_fpadd16(dd, k05);

					x0 += dx;
					tdp[i] = vis_fpack16(dd);
				}

			} else {
				mlib_d64 dx64, ddx, rdx;

				dx64 =
				    vis_to_double_dup(((dx & 0xFFFE) << 15) |
				    ((dx & 0xFFFE) >> 1));
				ddx =
				    vis_to_double_dup(((x0 & 0xFFFE) << 15) |
				    ((x0 & 0xFFFE) >> 1));
				rdx = vis_fpsub16(mask_7fff, ddx);

#pragma pipeloop(0)
				for (i = 0; i < width; i++) {
					mlib_d64 v0, v1, v2, v3, y0, y1, y10,
					    z10, dd;
					mlib_f32 sf0, sf1, sf2, sf3;

					cx0 = (x0 >> MLIB_SHIFT);

					sf0 = ((mlib_f32 *)tsp0)[cx0];
					sf1 = ((mlib_f32 *)tsp1)[cx0];
					sf2 = ((mlib_f32 *)tsp2)[cx0];
					sf3 = ((mlib_f32 *)tsp3)[cx0];

					rdx = vis_fpsub16(mask_7fff, ddx);

					v0 = vis_fmul8x16(sf0, rdx);
					v1 = vis_fmul8x16(sf1, ddx);
					v2 = vis_fmul8x16(sf2, rdx);
					v3 = vis_fmul8x16(sf3, ddx);

					y0 = vis_fpadd16(v0, v1);
					y1 = vis_fpadd16(v2, v3);

					y10 = vis_fpsub16(y1, y0);
					z10 = vis_fmul8x16(fdy, y10);
					dd = vis_fpadd16(y0, z10);
					dd = vis_fpadd16(dd, k05);

					x0 += dx;
					ddx =
					    vis_fand(vis_fpadd16(ddx, dx64),
					    mask_7fff);
					rdx = vis_fpsub16(mask_7fff, ddx);

					buff0[i] = y0;
					buff1[i] = y1;
					tdp[i] = vis_fpack16(dd);
				}
			}
		}

		if (!alp_ind) {
			if ((mlib_u8 *)tdp != dl) {
				mlib_ImageCopy_na((mlib_u8 *)tdp, dl,
				    4 * width);
			}
		} else /* ZoomBlend */ if (chan_d == 3) {
			mlib_f32 ss;
			mlib_d64 dd, a0, a1;
			mlib_u8 *dp = dl;

			vis_write_gsr((1 << 3) | 7);

			if (alp_ind == 3) {
				if (blend != MLIB_BLEND_GTK_SRC) {
#pragma pipeloop(0)
					for (i = 0; i < width; i++) {
						dd = vis_faligndata(vis_ld_u8_i
						    (dp, 2), dd);
						dd = vis_faligndata(vis_ld_u8_i
						    (dp, 1), dd);
						dd = vis_faligndata(vis_ld_u8_i
						    (dp, 0), dd);

						BLEND43(lo, al);
					}
				}

				mlib_v_ImageChannelExtract_U8_43L_D1((void *)
				    buffd, dl, width);

			} else {
				if (blend != MLIB_BLEND_GTK_SRC) {
#pragma pipeloop(0)
					for (i = 0; i < width; i++) {
						dd = vis_faligndata(vis_ld_u8_i
						    (dp, 2), dd);
						dd = vis_faligndata(vis_ld_u8_i
						    (dp, 1), dd);
						dd = vis_faligndata(vis_ld_u8_i
						    (dp, 0), dd);
						dd = vis_faligndata(dd, dd);

						BLEND43(hi, au);
					}
				}

				mlib_v_ImageChannelExtract_U8_43R_D1((void *)
				    buffd, dl, width);
			}

		} else {
			FP_TYPE dalpha0 = param->alpha;
			FP_TYPE dalpha = dalpha0 * (F_ONE / 255);
			FP_TYPE w0, w1, w, rw;
			mlib_u8 *sp = buffd;

			if (alp_ind == -1)
				sp++;

			if (blend == MLIB_BLEND_GTK_SRC) {
				mlib_u8 *dp = buffd;
				mlib_s32 alp_ind0 = alp_ind, cmask;

				if (alp_ind == -1)
					alp_ind0 = 0;
				cmask = 0x777 >> alp_ind0;

				vis_write_gsr(7 << 3);

#pragma pipeloop(0)
				for (i = 0; i < (width + 1) / 2; i++) {
					mlib_d64 ss = ((mlib_d64 *)dp)[i];
					mlib_d64 d0, d1;

					d0 = DIV_ALPHA(vis_read_hi(ss),
					    dp[8 * i + alp_ind0]);
					d1 = DIV_ALPHA(vis_read_lo(ss),
					    dp[8 * i + alp_ind0 + 4]);

					d0 = vis_fpack16_pair(d0, d1);

					vis_pst_8(d0, (mlib_d64 *)dp + i,
					    cmask);
				}

				mlib_ImageCopy_na(dp, dl, 4 * width);
			} else if (blend == MLIB_BLEND_GTK_SRC_OVER2) {
#pragma pipeloop(0)
				for (i = 0; i < width; i++) {
					w0 = dalpha * sp[4 * i + alp_ind];
					w1 = (F_ONE -
					    w0 * (F_ONE / 255)) * I2D(dl[4 * i +
					    alp_ind]);
					w = w0 + w1;
					FP_INV(rw, w);

					BLEND44(dl[4 * i], sp[4 * i]);
					BLEND44(dl[4 * i + 1], sp[4 * i + 1]);
					BLEND44(dl[4 * i + 2], sp[4 * i + 2]);
					dl[4 * i + alp_ind] = w;
				}
			} else {
#pragma pipeloop(0)
				for (i = 0; i < width; i++) {
					w0 = dalpha * sp[4 * i + alp_ind];
					w1 = (F_ONE -
					    w0 * (F_ONE / 255)) * I2D(dl[4 * i +
					    alp_ind]);
					w = w0 + w1;
					FP_INV(rw, w);

					BLEND44z(dl[4 * i], sp[4 * i]);
					BLEND44z(dl[4 * i + 1], sp[4 * i + 1]);
					BLEND44z(dl[4 * i + 2], sp[4 * i + 2]);
					dl[4 * i + alp_ind] = w;
				}
			}
		}

		dl += dst_stride;
		y += dy;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	if (size > BUFF_SIZE) {
		__mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
