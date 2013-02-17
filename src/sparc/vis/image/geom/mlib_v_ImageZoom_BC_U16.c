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

#pragma ident	"@(#)mlib_v_ImageZoom_BC_U16.c	9.2	07/11/05 SMI"

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

#define	_MLIB_VIS_VER_

#include <mlib_ImageZoom.h>
#include <mlib_v_ImageFilters.h>
#include <mlib_v_ImageChannelInsert.h>
#include <mlib_v_ImageChannelExtract.h>

/* *********************************************************** */

#if 0	/* SHORT */

#define	FUNC_NAME(CC)	mlib_v_ImageZoomBicubic_S16_##CC

#define	DEF_MASK

#define	XOR(x)	(x)

/* *********************************************************** */

#define	FPMUL_U16(res, src1, src2)	FPMUL_S16(res, src1, src2)

#else /* 0 ( SHORT ) */

#define	FUNC_NAME(CC)	mlib_v_ImageZoomBicubic_U16_##CC

/* *********************************************************** */

#define	DEF_MASK	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);

#define	XOR(x)	vis_fxor(x, mask8000)

/* *********************************************************** */

#define	FPMUL_U16(res, src1, src2)                              \
	src1 = XOR(src1);                                       \
	FPMUL_S16(res, src1, src2)

#endif /* 0 ( SHORT ) */

/* *********************************************************** */

#define	FPMUL_S16(res, src1, src2)                                     \
	res =                                                          \
	vis_fpadd16(vis_fmul8sux16(src1, src2), vis_fmul8ulx16(src1,   \
	    src2))

/* *********************************************************** */

#define	FILTER_1	filter1
#define	FILTER_3	filter3

#define	FILTER_4	filter4

/* *********************************************************** */

#define	MLIB_XY_SHIFT	4
#define	MLIB_XY_MASK	(((1 << 9) - 1) << 3)

/* *********************************************************** */

#define	VARIABLE(FORMAT)                                             \
	FORMAT *sp = GetElemSubStruct(current, sp),                  \
		*dl = GetElemSubStruct(current, dp);                 \
	DEF_MASK                                                     \
	mlib_u8 *filter4 = GetElemStruct(filter4);                   \
	mlib_s32 row_size = param -> sline_size;                     \
	mlib_s32 x_max = (row_size - 3) << MLIB_SHIFT;               \
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

#define	COEFF_D64_S16(r)                                        \
	fpos = (r >> MLIB_XY_SHIFT) & MLIB_XY_MASK;             \
	dfPtr = (mlib_d64 *)((mlib_u8 *)FILTER_4 + fpos * 4);   \
	d##r##f0 = dfPtr[0];                                    \
	d##r##f1 = dfPtr[1];                                    \
	d##r##f2 = dfPtr[2];                                    \
	d##r##f3 = dfPtr[3]

/* *********************************************************** */

mlib_status FUNC_NAME(1)  (
    mlib_work_image *param)
{
	VARIABLE(mlib_s16);
	mlib_u8 *filter1 = GetElemStruct(filter1);
	mlib_s16 *tsp, *tt0, *tt1;
	mlib_d64 *tdp;
	mlib_s32 fpos;
	mlib_d64 *dfPtr, dyf0, dyf1, dyf2, dyf3, dxx0, dxx1, fone =
	    vis_to_float(0x01000100);
	mlib_d64 *buff, *buffd, *buff_arr[8], *buff0, *buff1, *buff2, *buff3,
	    *buffx;
	mlib_f32 *pbuff;
	mlib_s32 width2, width4, k, bind = 0, y_step = 4;

	width2 = (width + 1) / 2;
	width4 = (width + 3) / 4;

	buff = __mlib_malloc((width4 * 9 + 2) * sizeof (mlib_d64));

	if (buff == NULL)
		return (MLIB_FAILURE);
	for (k = 0; k < 4; k++)
		buff_arr[k] = buff + k * width4;
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k];
	buffx = buff + 4 * width4;
	buffd = buff + 8 * width4;

	vis_write_gsr((10 << 3) | 6);

	x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	for (i = 0; i < width2; i++) {
		mlib_u8 *p_f0, *p_f1;
		mlib_d64 dx0, dx1;

		fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
		p_f0 = (mlib_u8 *)FILTER_1 + fpos;
		x += dx;
		fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
		p_f1 = (mlib_u8 *)FILTER_1 + fpos;
		x += dx;

		dx0 = vis_faligndata(vis_ld_u16_i(p_f1, 2), dx0);
		dx0 = vis_faligndata(vis_ld_u16_i(p_f0, 2), dx0);
		dx0 = vis_faligndata(vis_ld_u16_i(p_f1, 0), dx0);
		dx0 = vis_faligndata(vis_ld_u16_i(p_f0, 0), dx0);
		dx1 = vis_faligndata(vis_ld_u16_i(p_f1, 6), dx1);
		dx1 = vis_faligndata(vis_ld_u16_i(p_f0, 6), dx1);
		dx1 = vis_faligndata(vis_ld_u16_i(p_f1, 4), dx1);
		dx1 = vis_faligndata(vis_ld_u16_i(p_f0, 4), dx1);

		buffx[2 * i] = dx0;
		buffx[2 * i + 1] = dx1;
	}

	sp -= src_stride + 1;
	tsp = sp;

	for (j = 0; j < height; j++) {
		COEFF_D64_S16(y);

		vis_write_gsr((10 << 3) | 6);

		if (((mlib_addr)dl & 7) || (width & 3)) {
			tdp = (mlib_d64 *)buffd;
		} else {
			tdp = (mlib_d64 *)dl;
		}

		if (y_step >= 4) {
			y_step = 4;
			tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		} else {
			bind += y_step;

			if (bind >= 4)
				bind -= 4;
		}

		for (k = 4 - y_step; k < 4; k++) {
			mlib_d64 s0, s1, r0, r1, r2;
			mlib_s32 cx0, cx1;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			pbuff = (mlib_f32 *)buff_arr[bind + k];

			cx0 = (x >> MLIB_SHIFT) & ((x - x_max) >> 31);
			tt0 = tsp + cx0;
			x += dx;
			cx1 = (x >> MLIB_SHIFT) & ((x - x_max) >> 31);
			tt1 = tsp + cx1;
			x += dx;

			s0 = vis_faligndata(vis_ld_u16_i(tt1, 2), s0);
			s0 = vis_faligndata(vis_ld_u16_i(tt0, 2), s0);
			s0 = vis_faligndata(vis_ld_u16_i(tt1, 0), s0);
			s0 = vis_faligndata(vis_ld_u16_i(tt0, 0), s0);
			s1 = vis_faligndata(vis_ld_u16_i(tt1, 6), s1);
			s1 = vis_faligndata(vis_ld_u16_i(tt0, 6), s1);
			s1 = vis_faligndata(vis_ld_u16_i(tt1, 4), s1);
			s1 = vis_faligndata(vis_ld_u16_i(tt0, 4), s1);

#pragma pipeloop(0)
			for (i = 0; i < width2; i++) {
				dxx0 = buffx[2 * i];
				dxx1 = buffx[2 * i + 1];

				cx0 = (x >> MLIB_SHIFT) & ((x - x_max) >> 31);
				tt0 = tsp + cx0;
				x += dx;
				cx1 = (x >> MLIB_SHIFT) & ((x - x_max) >> 31);
				tt1 = tsp + cx1;
				x += dx;

				FPMUL_U16(r0, s0, dxx0);
				s0 = vis_faligndata(vis_ld_u16_i(tt1, 2), s0);
				s0 = vis_faligndata(vis_ld_u16_i(tt0, 2), s0);
				s0 = vis_faligndata(vis_ld_u16_i(tt1, 0), s0);
				s0 = vis_faligndata(vis_ld_u16_i(tt0, 0), s0);

				FPMUL_U16(r1, s1, dxx1);
				s1 = vis_faligndata(vis_ld_u16_i(tt1, 6), s1);
				s1 = vis_faligndata(vis_ld_u16_i(tt0, 6), s1);
				s1 = vis_faligndata(vis_ld_u16_i(tt1, 4), s1);
				s1 = vis_faligndata(vis_ld_u16_i(tt0, 4), s1);

				r2 = vis_fpadd16(r0, r1);
				pbuff[i] =
				    vis_fpadd16s(vis_read_hi(r2),
				    vis_read_lo(r2));
			}

			tsp += src_stride;
		}

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];

#pragma pipeloop(0)
		for (i = 0; i < width4; i++) {
			mlib_d64 s0, s1, s2, s3, r0, r1, r2, r3, r4, r5, r6, r7,
			    r8;
			s0 = buff0[i];
			s1 = buff1[i];
			s2 = buff2[i];
			s3 = buff3[i];

			FPMUL_S16(r0, s0, dyf0);
			FPMUL_S16(r1, s1, dyf1);
			FPMUL_S16(r2, s2, dyf2);
			FPMUL_S16(r3, s3, dyf3);

			r4 = vis_fpadd16(r0, r1);
			r5 = vis_fpadd16(r2, r3);
			r6 = vis_fpadd16(r4, r5);

			r7 = vis_fmuld8sux16(fone, vis_read_hi(r6));
			r8 = vis_fmuld8sux16(fone, vis_read_lo(r6));

			tdp[i] = XOR(vis_fpackfix_pair(r7, r8));
		}

		if ((mlib_s16 *)tdp != dl)
			mlib_ImageCopy_na((void *)tdp, (void *)dl, 2 * width);

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		y += dy;
		dl += dst_stride;
	}

	__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(2)  (
    mlib_work_image *param)
{
	VARIABLE(mlib_s16);
	mlib_s16 *tsp;
	mlib_d64 *tdp;
	mlib_f32 *prow, *tt0, *tt1;
	mlib_s32 fpos;
	mlib_d64 *dfPtr;
	mlib_d64 dyf0, dyf1, dyf2, dyf3;
	mlib_d64 dxf0, dxf1, dxf2, dxf3, dxx0, dxx1, dxx2, dxx3;
	mlib_d64 mask_ffff = vis_to_double(0, 0xFFFFFFFF), fone =
	    vis_to_float(0x01000100);
	mlib_d64 *buff, *buffs, *buffd, *buff_arr[8], *buff0, *buff1, *buff2,
	    *buff3, *buffx;
	mlib_d64 *pbuff;
	mlib_s32 width2 = (width + 1) / 2, k, bind = 0, y_step = 4;

	row_size *= 4;
	buff = __mlib_malloc(width2 * 9 * sizeof (mlib_d64) + row_size);

	if (buff == NULL)
		return (MLIB_FAILURE);
	for (k = 0; k < 4; k++)
		buff_arr[k] = buff + k * width2;
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k];
	buffx = buff + 4 * width2;
	buffd = buff + 8 * width2;
	buffs = buffd + width2;

	x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	for (i = 0; i < width2; i++) {
		mlib_s32 xx;
		mlib_d64 dxxf0, dxxf1, dxxf2, dxxf3;

		COEFF_D64_S16(x);
		xx = x + dx;
		COEFF_D64_S16(xx);

		buffx[4 * i] =
		    vis_for(vis_fandnot(mask_ffff, dxf0), vis_fand(mask_ffff,
		    dxxf0));
		buffx[4 * i + 1] =
		    vis_for(vis_fandnot(mask_ffff, dxf1), vis_fand(mask_ffff,
		    dxxf1));
		buffx[4 * i + 2] =
		    vis_for(vis_fandnot(mask_ffff, dxf2), vis_fand(mask_ffff,
		    dxxf2));
		buffx[4 * i + 3] =
		    vis_for(vis_fandnot(mask_ffff, dxf3), vis_fand(mask_ffff,
		    dxxf3));

		x += 2 * dx;
	}

	sp -= src_stride + 2;
	tsp = sp;

	for (j = 0; j < height; j++) {
		COEFF_D64_S16(y);

		if (((mlib_addr)dl & 7) || (width & 1)) {
			tdp = (mlib_d64 *)buffd;
		} else {
			tdp = (mlib_d64 *)dl;
		}

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

			if ((mlib_addr)tsp & 3) {
				prow = (mlib_f32 *)buffs;
				mlib_ImageCopy_na((void *)tsp, (void *)prow,
				    row_size);
			} else {
				prow = (mlib_f32 *)tsp;
			}

#pragma pipeloop(0)
			for (i = 0; i < width2; i++) {
				mlib_f32 a0, a1, a2, a3, b0, b1, b2, b3;
				mlib_d64 c0, c1, c2, c3, d0, d1, d2, d3, d4, d5,
				    d6;
				mlib_s32 cx0, cx1;

				cx0 = (x >> MLIB_SHIFT);
				tt0 = prow + cx0;
				x += dx;
				cx1 = (x >> MLIB_SHIFT) & ((x - x_max) >> 31);
				tt1 = prow + cx1;
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

				c0 = vis_freg_pair(a0, b0);
				c1 = vis_freg_pair(a1, b1);
				c2 = vis_freg_pair(a2, b2);
				c3 = vis_freg_pair(a3, b3);

				FPMUL_U16(d0, c0, dxx0);
				FPMUL_U16(d1, c1, dxx1);
				FPMUL_U16(d2, c2, dxx2);
				FPMUL_U16(d3, c3, dxx3);

				d4 = vis_fpadd16(d0, d1);
				d5 = vis_fpadd16(d2, d3);
				d6 = vis_fpadd16(d4, d5);

				pbuff[i] = d6;
			}

			tsp += src_stride;
		}

		vis_write_gsr(10 << 3);

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];

		if (k < 4) {
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			if ((mlib_addr)tsp & 3) {
				prow = (mlib_f32 *)buffs;
				mlib_ImageCopy_na((void *)tsp, (void *)prow,
				    row_size);
			} else {
				prow = (mlib_f32 *)tsp;
			}

#pragma pipeloop(0)
			for (i = 0; i < width2; i++) {
				mlib_f32 a0, a1, a2, a3, b0, b1, b2, b3;
				mlib_d64 c0, c1, c2, c3, d0, d1, d2, d3, d4, d5;
				mlib_d64 s0, s1, s2, s3, r0, r1, r2, r3, r4, r5,
				    r6, r7, r8;
				mlib_s32 cx0, cx1;

				cx0 = (x >> MLIB_SHIFT);
				tt0 = prow + cx0;
				x += dx;
				cx1 = (x >> MLIB_SHIFT) & ((x - x_max) >> 31);
				tt1 = prow + cx1;
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

				c0 = vis_freg_pair(a0, b0);
				c1 = vis_freg_pair(a1, b1);
				c2 = vis_freg_pair(a2, b2);
				c3 = vis_freg_pair(a3, b3);

				FPMUL_U16(d0, c0, dxx0);
				FPMUL_U16(d1, c1, dxx1);
				FPMUL_U16(d2, c2, dxx2);
				FPMUL_U16(d3, c3, dxx3);

				d4 = vis_fpadd16(d0, d1);
				d5 = vis_fpadd16(d2, d3);
				s3 = vis_fpadd16(d4, d5);

				FPMUL_S16(r0, s0, dyf0);
				FPMUL_S16(r1, s1, dyf1);
				FPMUL_S16(r2, s2, dyf2);
				FPMUL_S16(r3, s3, dyf3);

				r4 = vis_fpadd16(r0, r1);
				r5 = vis_fpadd16(r2, r3);
				r6 = vis_fpadd16(r4, r5);

				r7 = vis_fmuld8sux16(fone, vis_read_hi(r6));
				r8 = vis_fmuld8sux16(fone, vis_read_lo(r6));

				buff3[i] = s3;
				tdp[i] = XOR(vis_fpackfix_pair(r7, r8));
			}

			tsp += src_stride;
		} else {
#pragma pipeloop(0)
			for (i = 0; i < width2; i++) {
				mlib_d64 s0, s1, s2, s3, r0, r1, r2, r3, r4, r5,
				    r6, r7, r8;
				s0 = buff0[i];
				s1 = buff1[i];
				s2 = buff2[i];
				s3 = buff3[i];

				FPMUL_S16(r0, s0, dyf0);
				FPMUL_S16(r1, s1, dyf1);
				FPMUL_S16(r2, s2, dyf2);
				FPMUL_S16(r3, s3, dyf3);

				r4 = vis_fpadd16(r0, r1);
				r5 = vis_fpadd16(r2, r3);
				r6 = vis_fpadd16(r4, r5);

				r7 = vis_fmuld8sux16(fone, vis_read_hi(r6));
				r8 = vis_fmuld8sux16(fone, vis_read_lo(r6));

				tdp[i] = XOR(vis_fpackfix_pair(r7, r8));
			}
		}

		if ((mlib_s16 *)tdp != dl)
			mlib_ImageCopy_na((void *)tdp, (void *)dl, 4 * width);

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		y += dy;
		dl += dst_stride;
	}

	__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(4)  (
    mlib_work_image *param)
{
	VARIABLE(mlib_s16);
	mlib_s32 cx;
	mlib_s16 *tsp;
	mlib_d64 *prow, *tdp;
	mlib_s32 fpos;
	mlib_d64 *dfPtr;
	mlib_d64 dyf0, dyf1, dyf2, dyf3, dxf0, dxf1, dxf2, dxf3;
	mlib_d64 fone = vis_to_float(0x01000100);
	mlib_d64 *buff, *buffs, *buffd, *buff_arr[8], *buff0, *buff1, *buff2,
	    *buff3, *pbuff;
	mlib_s32 k, bind = 0, y_step = 4;

	row_size *= 8;
	buff = __mlib_malloc(width * 5 * sizeof (mlib_d64) + row_size);

	if (buff == NULL)
		return (MLIB_FAILURE);
	for (k = 0; k < 4; k++)
		buff_arr[k] = buff + k * width;
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k];
	buffd = buff + 4 * width;
	buffs = buffd + width;

	sp -= src_stride + 4;
	tsp = sp;

	for (j = 0; j < height; j++) {
		fpos = (y >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
		dfPtr = (mlib_d64 *)((mlib_u8 *)FILTER_4 + fpos * 4);
		dyf0 = dfPtr[0];
		dyf1 = dfPtr[1];
		dyf2 = dfPtr[2];
		dyf3 = dfPtr[3];

		if (((mlib_addr)dl & 7)) {
			tdp = buffd;
		} else {
			tdp = (mlib_d64 *)dl;
		}

		if (y_step >= 4) {
			y_step = 4;
			tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		} else {
			bind += y_step;

			if (bind >= 4)
				bind -= 4;
		}

		for (k = 4 - y_step; k < 3; k++) {
			mlib_d64 s0, s1, s2, s3;
			mlib_d64 r0, r1, r2, r3, r4, r5, r6;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			pbuff = buff_arr[bind + k];

			if ((mlib_addr)tsp & 7) {
				prow = (mlib_d64 *)buffs;
				mlib_ImageCopy_na((void *)tsp, (void *)prow,
				    row_size);
			} else {
				prow = (mlib_d64 *)tsp;
			}

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				cx = x >> MLIB_SHIFT;
				fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
				dfPtr =
				    (mlib_d64 *)((mlib_u8 *)FILTER_4 +
				    fpos * 4);
				dxf0 = dfPtr[0];
				dxf1 = dfPtr[1];
				dxf2 = dfPtr[2];
				dxf3 = dfPtr[3];

				s0 = prow[cx];
				s1 = prow[cx + 1];
				s2 = prow[cx + 2];
				s3 = prow[cx + 3];

				FPMUL_U16(r0, s0, dxf0);
				FPMUL_U16(r1, s1, dxf1);
				FPMUL_U16(r2, s2, dxf2);
				FPMUL_U16(r3, s3, dxf3);

				r4 = vis_fpadd16(r0, r1);
				r5 = vis_fpadd16(r2, r3);
				r6 = vis_fpadd16(r4, r5);
				pbuff[i] = r6;

				x += dx;
			}

			tsp += src_stride;
		}

		vis_write_gsr(10 << 3);

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];

		if (k < 4) {
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			if ((mlib_addr)tsp & 7) {
				prow = (mlib_d64 *)buffs;
				mlib_ImageCopy_na((void *)tsp, (void *)prow,
				    row_size);
			} else {
				prow = (mlib_d64 *)tsp;
			}

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				mlib_d64 a0, a1, a2, a3, b0, b1, b2, b3, b4, b5;
				mlib_d64 s0, s1, s2, s3, r0, r1, r2, r3, r4, r5,
				    r6, r7, r8, r9;

				cx = x >> MLIB_SHIFT;
				fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
				dfPtr =
				    (mlib_d64 *)((mlib_u8 *)FILTER_4 +
				    fpos * 4);
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

				FPMUL_U16(b0, a0, dxf0);
				FPMUL_U16(b1, a1, dxf1);
				FPMUL_U16(b2, a2, dxf2);
				FPMUL_U16(b3, a3, dxf3);

				b4 = vis_fpadd16(b0, b1);
				b5 = vis_fpadd16(b2, b3);
				s3 = vis_fpadd16(b4, b5);

				FPMUL_S16(r0, s0, dyf0);
				FPMUL_S16(r1, s1, dyf1);
				FPMUL_S16(r2, s2, dyf2);
				FPMUL_S16(r3, s3, dyf3);

				r4 = vis_fpadd16(r0, r1);
				r5 = vis_fpadd16(r2, r3);
				r6 = vis_fpadd16(r4, r5);

				r7 = vis_fmuld8sux16(fone, vis_read_hi(r6));
				r8 = vis_fmuld8sux16(fone, vis_read_lo(r6));
				r9 = XOR(vis_fpackfix_pair(r7, r8));

				buff3[i] = s3;
				tdp[i] = r9;
				x += dx;
			}

			tsp += src_stride;
		} else {

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				mlib_d64 s0, s1, s2, s3, r0, r1, r2, r3, r4, r5,
				    r6, r7, r8;
				s0 = buff0[i];
				s1 = buff1[i];
				s2 = buff2[i];
				s3 = buff3[i];

				FPMUL_S16(r0, s0, dyf0);
				FPMUL_S16(r1, s1, dyf1);
				FPMUL_S16(r2, s2, dyf2);
				FPMUL_S16(r3, s3, dyf3);

				r4 = vis_fpadd16(r0, r1);
				r5 = vis_fpadd16(r2, r3);
				r6 = vis_fpadd16(r4, r5);

				r7 = vis_fmuld8sux16(fone, vis_read_hi(r6));
				r8 = vis_fmuld8sux16(fone, vis_read_lo(r6));

				tdp[i] = XOR(vis_fpackfix_pair(r7, r8));
			}
		}

		if ((mlib_s16 *)tdp != dl)
			mlib_ImageCopy_na((void *)tdp, (void *)dl, 8 * width);

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		y += dy;
		dl += dst_stride;
	}

	__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(3)  (
    mlib_work_image *param)
{
	VARIABLE(mlib_s16);
	mlib_s32 cx;
	mlib_s16 *tsp;
	mlib_d64 *prow, *tdp;
	mlib_s32 fpos;
	mlib_d64 *dfPtr;
	mlib_d64 dyf0, dyf1, dyf2, dyf3, dxf0, dxf1, dxf2, dxf3;
	mlib_d64 fone = vis_to_float(0x01000100);
	mlib_d64 *buff, *buffs, *buffd, *buff_arr[8], *buff0, *buff1, *buff2,
	    *buff3, *pbuff;
	mlib_s32 k, bind = 0, y_step = 4;

	buff = __mlib_malloc(width * 5 * sizeof (mlib_d64) + 8 * row_size);

	if (buff == NULL)
		return (MLIB_FAILURE);
	for (k = 0; k < 4; k++)
		buff_arr[k] = buff + k * width;
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k];
	buffd = buff + 4 * width;
	buffs = buffd + width;

	sp -= src_stride + 3;
	tsp = sp;

	for (j = 0; j < height; j++) {
		fpos = (y >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
		dfPtr = (mlib_d64 *)((mlib_u8 *)FILTER_4 + fpos * 4);
		dyf0 = dfPtr[0];
		dyf1 = dfPtr[1];
		dyf2 = dfPtr[2];
		dyf3 = dfPtr[3];

		tdp = buffd;

		if (y_step >= 4) {
			y_step = 4;
			tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		} else {
			bind += y_step;

			if (bind >= 4)
				bind -= 4;
		}

		for (k = 4 - y_step; k < 3; k++) {
			mlib_d64 s0, s1, s2, s3;
			mlib_d64 r0, r1, r2, r3, r4, r5, r6;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			pbuff = buff_arr[bind + k];

			prow = (mlib_d64 *)buffs;
			mlib_v_ImageChannelInsert_S16_34R_D1((void *)tsp,
			    (void *)prow, row_size);

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				cx = x >> MLIB_SHIFT;
				fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
				dfPtr =
				    (mlib_d64 *)((mlib_u8 *)FILTER_4 +
				    fpos * 4);
				dxf0 = dfPtr[0];
				dxf1 = dfPtr[1];
				dxf2 = dfPtr[2];
				dxf3 = dfPtr[3];

				s0 = prow[cx];
				s1 = prow[cx + 1];
				s2 = prow[cx + 2];
				s3 = prow[cx + 3];

				FPMUL_U16(r0, s0, dxf0);
				FPMUL_U16(r1, s1, dxf1);
				FPMUL_U16(r2, s2, dxf2);
				FPMUL_U16(r3, s3, dxf3);

				r4 = vis_fpadd16(r0, r1);
				r5 = vis_fpadd16(r2, r3);
				r6 = vis_fpadd16(r4, r5);
				pbuff[i] = r6;

				x += dx;
			}

			tsp += src_stride;
		}

		vis_write_gsr(10 << 3);

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];

		if (k < 4) {
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			prow = (mlib_d64 *)buffs;
			mlib_v_ImageChannelInsert_S16_34R_D1((void *)tsp,
			    (void *)prow, row_size);

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				mlib_d64 a0, a1, a2, a3, b0, b1, b2, b3, b4, b5;
				mlib_d64 s0, s1, s2, s3, r0, r1, r2, r3, r4, r5,
				    r6, r7, r8, r9;

				cx = x >> MLIB_SHIFT;
				fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
				dfPtr =
				    (mlib_d64 *)((mlib_u8 *)FILTER_4 +
				    fpos * 4);
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

				FPMUL_U16(b0, a0, dxf0);
				FPMUL_U16(b1, a1, dxf1);
				FPMUL_U16(b2, a2, dxf2);
				FPMUL_U16(b3, a3, dxf3);

				b4 = vis_fpadd16(b0, b1);
				b5 = vis_fpadd16(b2, b3);
				s3 = vis_fpadd16(b4, b5);

				FPMUL_S16(r0, s0, dyf0);
				FPMUL_S16(r1, s1, dyf1);
				FPMUL_S16(r2, s2, dyf2);
				FPMUL_S16(r3, s3, dyf3);

				r4 = vis_fpadd16(r0, r1);
				r5 = vis_fpadd16(r2, r3);
				r6 = vis_fpadd16(r4, r5);

				r7 = vis_fmuld8sux16(fone, vis_read_hi(r6));
				r8 = vis_fmuld8sux16(fone, vis_read_lo(r6));
				r9 = XOR(vis_fpackfix_pair(r7, r8));

				buff3[i] = s3;
				tdp[i] = r9;
				x += dx;
			}

			tsp += src_stride;
		} else {

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				mlib_d64 s0, s1, s2, s3, r0, r1, r2, r3, r4, r5,
				    r6, r7, r8;
				s0 = buff0[i];
				s1 = buff1[i];
				s2 = buff2[i];
				s3 = buff3[i];

				FPMUL_S16(r0, s0, dyf0);
				FPMUL_S16(r1, s1, dyf1);
				FPMUL_S16(r2, s2, dyf2);
				FPMUL_S16(r3, s3, dyf3);

				r4 = vis_fpadd16(r0, r1);
				r5 = vis_fpadd16(r2, r3);
				r6 = vis_fpadd16(r4, r5);

				r7 = vis_fmuld8sux16(fone, vis_read_hi(r6));
				r8 = vis_fmuld8sux16(fone, vis_read_lo(r6));

				tdp[i] = XOR(vis_fpackfix_pair(r7, r8));
			}
		}

		mlib_v_ImageChannelExtract_S16_43R_D1((void *)tdp, (void *)dl,
		    width);

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		y += dy;
		dl += dst_stride;
	}

	__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
