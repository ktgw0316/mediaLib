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

#pragma ident	"@(#)mlib_v_ImageZoom_BC_f.c	9.3	07/11/05 SMI"

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

/* *********************************************************** */

#define	_MLIB_VIS_VER_

#include <mlib_ImageZoom.h>
#include <mlib_ImageDivTables.h>
#include <mlib_v_ImageFilters.h>
#include <mlib_v_ImageChannelInsert.h>
#include <mlib_v_ImageChannelExtract.h>
#include <mlib_Utils.h>

/* *********************************************************** */

#define	FUNC_NAME(CC)	mlib_v_ImageZoomBicubic_##CC

/* *********************************************************** */

#define	FILTER_1	filter1
#define	FILTER_3	filter3

#define	FILTER_4	filter4

/* *********************************************************** */

#define	VARIABLE(FORMAT)                                             \
	FORMAT *sp = GetElemSubStruct(current, sp),                  \
		*dl = GetElemSubStruct(current, dp);                 \
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

#define	MLIB_XY_SHIFT	5
#define	MLIB_XY_MASK	(((1 << 8) - 1) << 3)

/* *********************************************************** */

#define	FPMUL16(src1, src2)                                     \
	vis_fpadd16(vis_fmul8sux16((src1), (src2)),             \
	    vis_fmul8ulx16((src1), (src2)))

/* *********************************************************** */

#define	COEFF_D64_U8(r)                                         \
	fpos = (r >> MLIB_XY_SHIFT) & MLIB_XY_MASK;             \
	dfPtr = (mlib_d64 *)((mlib_u8 *)FILTER_4 + fpos * 4);   \
	d##r##f0 = dfPtr[0];                                    \
	d##r##f1 = dfPtr[1];                                    \
	d##r##f2 = dfPtr[2];                                    \
	d##r##f3 = dfPtr[3]

/* *********************************************************** */

#define	LOAD_U8_2_BC                                            \
	cx = (x >> MLIB_SHIFT) * 2;                             \
	x += dx;                                                \
	dsp = (mlib_d64 *)vis_alignaddr(tsp + cx, 0);           \
	dtmp0 = dsp[0];                                         \
	dtmp1 = dsp[1];                                         \
	A0 = vis_faligndata(dtmp0, dtmp1);                      \
	cx += src_stride;                                       \
	dsp = (mlib_d64 *)vis_alignaddr(tsp + cx, 0);           \
	dtmp0 = dsp[0];                                         \
	dtmp1 = dsp[1];                                         \
	B0 = vis_faligndata(dtmp0, dtmp1);                      \
	cx += src_stride;                                       \
	dsp = (mlib_d64 *)vis_alignaddr(tsp + cx, 0);           \
	dtmp0 = dsp[0];                                         \
	dtmp1 = dsp[1];                                         \
	C0 = vis_faligndata(dtmp0, dtmp1);                      \
	cx += src_stride;                                       \
	dsp = (mlib_d64 *)vis_alignaddr(tsp + cx, 0);           \
	dtmp0 = dsp[0];                                         \
	dtmp1 = dsp[1];                                         \
	D0 = vis_faligndata(dtmp0, dtmp1)

/* *********************************************************** */

#define	FMUL_U8_BC_2                                            \
	r00 = vis_fmul8x16(vis_read_hi(A0), dyf0);              \
	r02 = vis_fmul8x16(vis_read_lo(A0), dyf0);              \
	r10 = vis_fmul8x16(vis_read_hi(B0), dyf1);              \
	r12 = vis_fmul8x16(vis_read_lo(B0), dyf1);              \
	r20 = vis_fmul8x16(vis_read_hi(C0), dyf2);              \
	r22 = vis_fmul8x16(vis_read_lo(C0), dyf2);              \
	r30 = vis_fmul8x16(vis_read_hi(D0), dyf3);              \
	r32 = vis_fmul8x16(vis_read_lo(D0), dyf3)

/* *********************************************************** */

#define	FADD_U8_BC_2                                            \
	r40 = vis_fpadd16(r00, r10);                            \
	r42 = vis_fpadd16(r02, r12);                            \
	r50 = vis_fpadd16(r20, r30);                            \
	r52 = vis_fpadd16(r22, r32);                            \
	r60 = vis_fpadd16(r40, r50);                            \
	r62 = vis_fpadd16(r42, r52);                            \
	r70 = FPMUL16(r60, dxx0);                               \
	r72 = FPMUL16(r62, dxx1);                               \
	r80 = vis_fpadd16(r70, r72);                            \
	res =                                                   \
	vis_fmuld8ulx16(fone, vis_fpadd16s(vis_read_lo(r80),    \
	    vis_read_hi(r80)))

/* *********************************************************** */

#define	COEFF_D64_U8_2                                                     \
	dxx0 =                                                             \
	vis_for(vis_fandnot(mask_ffff, dxf0), vis_fand(mask_ffff,          \
	    dxf1));                                                        \
	dxx1 =                                                             \
	vis_for(vis_fandnot(mask_ffff, dxf2), vis_fand(mask_ffff, dxf3))

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

#define	BLEND44(dst, fres)                                      \
	{                                                       \
	    mlib_s32 res =                                      \
		dst + (mlib_s32)((dalpha0 * I2D(fres) -         \
		w0 * I2D(dst)) * rw);                           \
	    dst = (res & ~(res >> 31)) | ((255 - res) >> 31);   \
	}

#define	BLEND44z(dst, fres)                                              \
	{                                                                \
	    mlib_s32 res = (dalpha0 * I2D(fres) + w1 * I2D(dst)) * rw;   \
	                                                                 \
	    dst = (res & ~(res >> 31)) | ((255 - res) >> 31);            \
	}

/* *********************************************************** */

#define	DIV_ALPHA(ss, alpha)                                            \
	vis_fmul8x16(ss, ((mlib_d64 *)mlib_v_tbl_255DivAlpha)[alpha])

/* *********************************************************** */

mlib_status FUNC_NAME(U8_1) (
    mlib_work_image *param)
{
	VARIABLE(mlib_u8);
	mlib_u8 *filter1 = GetElemStruct(filter1);
	mlib_u8 *tsp, *tt0, *tt1;
	mlib_f32 *tdp;
	mlib_s32 fpos;
	mlib_d64 *dfPtr, dyf0, dyf1, dyf2, dyf3, dxx0, dxx1;
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

	vis_write_gsr((3 << 3) | 6);

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
		COEFF_D64_U8(y);

		if (((mlib_addr)dl & 3) || (width & 3)) {
			tdp = (mlib_f32 *)buffd;
		} else {
			tdp = (mlib_f32 *)dl;
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
			mlib_d64 s0, s1, s2, s3, s4, s5, r0, r1, r2;
			mlib_s32 cx0, cx1;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			pbuff = (mlib_f32 *)buff_arr[bind + k];

			cx0 = (x >> MLIB_SHIFT);
			tt0 = tsp + cx0;
			x += dx;
			cx1 = (x >> MLIB_SHIFT) & ((x - x_max) >> 31);
			tt1 = tsp + cx1;
			x += dx;

			s0 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(tt0, 0)),
			    vis_read_lo(vis_ld_u8_i(tt0, 1)));
			s1 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(tt1, 0)),
			    vis_read_lo(vis_ld_u8_i(tt1, 1)));
			s2 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(tt0, 2)),
			    vis_read_lo(vis_ld_u8_i(tt0, 3)));
			s3 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(tt1, 2)),
			    vis_read_lo(vis_ld_u8_i(tt1, 3)));
			s4 = vis_fpmerge(vis_read_lo(s0), vis_read_lo(s1));
			s5 = vis_fpmerge(vis_read_lo(s2), vis_read_lo(s3));

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

				s0 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(tt0,
				    0)), vis_read_lo(vis_ld_u8_i(tt0, 1)));
				s1 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(tt1,
				    0)), vis_read_lo(vis_ld_u8_i(tt1, 1)));
				s2 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(tt0,
				    2)), vis_read_lo(vis_ld_u8_i(tt0, 3)));
				s3 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(tt1,
				    2)), vis_read_lo(vis_ld_u8_i(tt1, 3)));

				r0 = vis_fmul8x16(vis_read_lo(s4), dxx0);
				r1 = vis_fmul8x16(vis_read_lo(s5), dxx1);

				s4 = vis_fpmerge(vis_read_lo(s0),
				    vis_read_lo(s1));
				s5 = vis_fpmerge(vis_read_lo(s2),
				    vis_read_lo(s3));

				r2 = vis_fpadd16(r0, r1);
				pbuff[i] =
				    vis_fpadd16s(vis_read_lo(r2),
				    vis_read_hi(r2));
			}

			tsp += src_stride;
		}

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];

#pragma pipeloop(0)
		for (i = 0; i < width4; i++) {
			mlib_d64 s0, s1, s2, s3, r0, r1, r2, r3, r4, r5, r6;

			s0 = buff0[i];
			s1 = buff1[i];
			s2 = buff2[i];
			s3 = buff3[i];

			r0 = FPMUL16(s0, dyf0);
			r1 = FPMUL16(s1, dyf1);
			r2 = FPMUL16(s2, dyf2);
			r3 = FPMUL16(s3, dyf3);

			r4 = vis_fpadd16(r0, r1);
			r5 = vis_fpadd16(r2, r3);
			r6 = vis_fpadd16(r4, r5);

			tdp[i] = vis_fpack16(r6);
		}

		if ((mlib_u8 *)tdp != dl)
			mlib_ImageCopy_na((mlib_u8 *)tdp, dl, width);

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		y += dy;
		dl += dst_stride;
	}

	__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(U8_2) (
    mlib_work_image *param)
{
	VARIABLE(mlib_u8);
	mlib_s32 cx;
	mlib_u8 *tsp, *prow, *tt;
	mlib_f32 *tdp;
	mlib_s32 fpos;
	mlib_d64 *dfPtr;
	mlib_d64 dyf0, dyf1, dyf2, dyf3, dxf0, dxf1, dxf2, dxf3;
	mlib_d64 dxx0, dxx1, mask_ffff = vis_to_double(0, 0xFFFFFFFF);
	mlib_d64 *buff, *buffs, *buffd, *buff_arr[8], *buff0, *buff1, *buff2,
	    *buff3, *buffx;
	mlib_f32 *pbuff;
	mlib_s32 width2 = (width + 1) / 2, k, bind = 0, y_step = 4;

	row_size *= 2;
	buff = __mlib_malloc(width2 * 9 * sizeof (mlib_d64) + row_size + 4 * 8);

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
		COEFF_D64_U8(y);

		if (((mlib_addr)dl & 3) || (width & 1)) {
			tdp = (mlib_f32 *)buffd;
		} else {
			tdp = (mlib_f32 *)dl;
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

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			pbuff = (mlib_f32 *)buff_arr[bind + k];

			if ((mlib_addr)tsp & 1) {
				prow = (mlib_u8 *)buffs;
				mlib_ImageCopy_na((void *)tsp, prow, row_size);
			} else {
				prow = tsp;
			}

			vis_write_gsr((3 << 3) | 6);

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				cx = 2 * (x >> MLIB_SHIFT);
				tt = prow + cx;
				x += dx;

				s0 = vis_faligndata(vis_ld_u16_i(tt, 2), s0);
				s0 = vis_faligndata(vis_ld_u16_i(tt, 0), s0);
				s1 = vis_faligndata(vis_ld_u16_i(tt, 6), s1);
				s1 = vis_faligndata(vis_ld_u16_i(tt, 4), s1);

				dxx0 = buffx[2 * i];
				dxx1 = buffx[2 * i + 1];

				r0 = vis_fmul8x16(vis_read_hi(s0), dxx0);
				r1 = vis_fmul8x16(vis_read_hi(s1), dxx1);
				r2 = vis_fpadd16(r0, r1);
				pbuff[i] =
				    vis_fpadd16s(vis_read_lo(r2),
				    vis_read_hi(r2));
			}

			tsp += src_stride;
		}

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];

#pragma pipeloop(0)
		for (i = 0; i < width2; i++) {
			mlib_d64 s0, s1, s2, s3, r0, r1, r2, r3, r4, r5, r6;

			s0 = buff0[i];
			s1 = buff1[i];
			s2 = buff2[i];
			s3 = buff3[i];

			r0 = FPMUL16(s0, dyf0);
			r1 = FPMUL16(s1, dyf1);
			r2 = FPMUL16(s2, dyf2);
			r3 = FPMUL16(s3, dyf3);

/*
 * r00 = vis_fmul8x16(dy0, buff0[i]);
 * r10 = vis_fmul8x16(dy0, buff1[i]);
 * r20 = vis_fmul8x16(dy0, buff2[i]);
 * r30 = vis_fmul8x16(dy0, buff3[i]);
 */

			r4 = vis_fpadd16(r0, r1);
			r5 = vis_fpadd16(r2, r3);
			r6 = vis_fpadd16(r4, r5);

			tdp[i] = vis_fpack16(r6);
		}

		if ((mlib_u8 *)tdp != dl)
			mlib_ImageCopy_na((mlib_u8 *)tdp, dl, 2 * width);

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		y += dy;
		dl += dst_stride;
	}

	__mlib_free(buff);
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

mlib_status FUNC_NAME(U8_4) (
    mlib_work_image *param)
{
	VARIABLE(mlib_u8);
	mlib_u8 *filter1 = GetElemStruct(filter1);
	mlib_s32 cx;
	mlib_u8 *tsp;
	mlib_f32 *prow;
	mlib_f32 *tdp;
	mlib_s32 fpos;
	mlib_d64 *dfPtr, dyf0, dyf1, dyf2, dyf3, dxf0;
	mlib_d64 *buff, *buffs, *buffd;
	mlib_d64 *buff_arr[8];
	mlib_d64 *buff0, *buff1, *buff2, *buff3, *buffx, *pbuff;
	mlib_s32 k, bind = 0, y_step = 4;
	mlib_d64 done = vis_to_double_dup((16384 << 16) | 16384);
	mlib_s32 alp_ind, chan_d;
	mlib_f32 aa, fzeros = vis_fzeros();
	mlib_blend blend;

	alp_ind = param->alp_ind;

	if (alp_ind) {
		mlib_s32 dalpha = (param->alpha) * (16384.0 / 255);

		aa = vis_to_float(((mlib_s32)dalpha << 16) | (mlib_s32)(dalpha *
		    (256.0 / 255)));
		chan_d = param->chan_d;
		blend = param->blend;

		if (alp_ind == -1) {
			tsp = sp -= 1;
			if (blend == MLIB_BLEND_GTK_SRC && chan_d == 4)
				dl--;
		}
	}

	row_size *= 4;
	buff = __mlib_malloc(width * 6 * sizeof (mlib_d64) + row_size + 8 * 8);

	if (buff == NULL)
		return (MLIB_FAILURE);
	for (k = 0; k < 4; k++)
		buff_arr[k] = buff + k * width;
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k];
	buffx = buff + 4 * width;
	buffd = buffx + width;
	buffs = buffd + width;

	x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	for (i = 0; i < width; i++) {
		fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
		buffx[i] = *(mlib_d64 *)((mlib_u8 *)FILTER_1 + fpos);
		x += dx;
	}

	sp -= src_stride + 4;
	tsp = sp;

	for (j = 0; j < height; j++) {
		COEFF_D64_U8(y);

		if (((mlib_addr)dl & 3) || alp_ind) {
			tdp = (mlib_f32 *)buffd;
		} else {
			tdp = (mlib_f32 *)dl;
		}

		if (y_step >= 4) {
			y_step = 4;
			tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		} else {
			bind += y_step;

			if (bind >= 4)
				bind -= 4;
		}

		vis_write_gsr(7 << 3);

		for (k = 4 - y_step; k < 3; k++) {
			mlib_f32 s0, s1, s2, s3;
			mlib_d64 r0, r1, r2, r3, r4, r5, r6;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			pbuff = buff_arr[bind + k];

			if ((mlib_addr)tsp & 3) {
				prow = (mlib_f32 *)buffs;
				mlib_ImageCopy_na((void *)tsp, (void *)prow,
				    row_size);
			} else {
				prow = (mlib_f32 *)tsp;
			}

			if (alp_ind) {
				if (alp_ind == 3) {
				    mlib_s32 cmask = 1 | (1 << 4);

#pragma pipeloop(0)
				    for (i = 0; i < (row_size + 7) / 8; i++) {
					MUL_ALPHA_4CH(prow, buffs, lo, al);
				    }
				} else {
				    mlib_s32 cmask = (1 << 3) | (1 << 7);

#pragma pipeloop(0)
				    for (i = 0; i < (row_size + 7) / 8; i++) {
					MUL_ALPHA_4CH(prow, buffs, hi, au);
				    }
				}
				prow = (mlib_f32 *)buffs;
			}
#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				dxf0 = buffx[i];
				cx = x >> MLIB_SHIFT;

				s0 = prow[cx];
				s1 = prow[cx + 1];
				s2 = prow[cx + 2];
				s3 = prow[cx + 3];

				r0 = vis_fmul8x16au(s0, vis_read_hi(dxf0));
				r1 = vis_fmul8x16al(s1, vis_read_hi(dxf0));
				r2 = vis_fmul8x16au(s2, vis_read_lo(dxf0));
				r3 = vis_fmul8x16al(s3, vis_read_lo(dxf0));

				r4 = vis_fpadd16(r0, r1);
				r5 = vis_fpadd16(r2, r3);
				r6 = vis_fpadd16(r4, r5);
				pbuff[i] = r6;

				x += dx;
			}

			tsp += src_stride;
		}

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

			if (alp_ind) {
				vis_write_gsr(7 << 3);
				if (alp_ind == 3) {
				    mlib_s32 cmask = 1 | (1 << 4);

#pragma pipeloop(0)
				    for (i = 0; i < (row_size + 7) / 8; i++) {
					MUL_ALPHA_4CH(prow, buffs, lo, al);
				    }
				} else {
				    mlib_s32 cmask = (1 << 3) | (1 << 7);

#pragma pipeloop(0)
				    for (i = 0; i < (row_size + 7) / 8; i++) {
					MUL_ALPHA_4CH(prow, buffs, hi, au);
				    }
				}
				prow = (mlib_f32 *)buffs;
			}

			vis_write_gsr(3 << 3);

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				mlib_f32 a0, a1, a2, a3;
				mlib_d64 b0, b1, b2, b3, b4, b5;
				mlib_d64 s0, s1, s2, s3, r0, r1, r2, r3, r4, r5,
				    r6;

				dxf0 = buffx[i];
				cx = x >> MLIB_SHIFT;

				a0 = prow[cx];
				a1 = prow[cx + 1];
				a2 = prow[cx + 2];
				a3 = prow[cx + 3];

				s0 = buff0[i];
				s1 = buff1[i];
				s2 = buff2[i];

				b0 = vis_fmul8x16au(a0, vis_read_hi(dxf0));
				b1 = vis_fmul8x16al(a1, vis_read_hi(dxf0));
				b2 = vis_fmul8x16au(a2, vis_read_lo(dxf0));
				b3 = vis_fmul8x16al(a3, vis_read_lo(dxf0));

				b4 = vis_fpadd16(b0, b1);
				b5 = vis_fpadd16(b2, b3);
				s3 = vis_fpadd16(b4, b5);

				r0 = FPMUL16(s0, dyf0);
				r1 = FPMUL16(s1, dyf1);
				r2 = FPMUL16(s2, dyf2);
				r3 = FPMUL16(s3, dyf3);

				r4 = vis_fpadd16(r0, r1);
				r5 = vis_fpadd16(r2, r3);
				r6 = vis_fpadd16(r4, r5);

				buff3[i] = s3;
				tdp[i] = vis_fpack16(r6);
				x += dx;
			}

			tsp += src_stride;
		} else {
			vis_write_gsr(3 << 3);

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				mlib_d64 s0, s1, s2, s3, r0, r1, r2, r3, r4, r5,
				    r6;
				s0 = buff0[i];
				s1 = buff1[i];
				s2 = buff2[i];
				s3 = buff3[i];

				r0 = FPMUL16(s0, dyf0);
				r1 = FPMUL16(s1, dyf1);
				r2 = FPMUL16(s2, dyf2);
				r3 = FPMUL16(s3, dyf3);

				r4 = vis_fpadd16(r0, r1);
				r5 = vis_fpadd16(r2, r3);
				r6 = vis_fpadd16(r4, r5);

				tdp[i] = vis_fpack16(r6);
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
				if (param->alpha > 0) {
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
				if (param->alpha > 0) {
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

		} else {	/* if (chan_d == 4) */

			FP_TYPE dalpha0 = param->alpha;
			FP_TYPE dalpha = dalpha0 * (F_ONE / 255);
			FP_TYPE w0, w1, w, rw;
			mlib_u8 *sp = (void *)buffd;

			if (alp_ind == -1)
				sp++;

			if (blend == MLIB_BLEND_GTK_SRC) {
				mlib_u8 *dp = (void *)buffd;
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
					    w0 * (F_ONE / 255)) * dl[4 * i +
					    alp_ind];
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
					    w0 * (F_ONE / 255)) * dl[4 * i +
					    alp_ind];
					w = w0 + w1;
					FP_INV(rw, w);

					BLEND44z(dl[4 * i], sp[4 * i]);
					BLEND44z(dl[4 * i + 1], sp[4 * i + 1]);
					BLEND44z(dl[4 * i + 2], sp[4 * i + 2]);
					dl[4 * i + alp_ind] = w;
				}
			}
		}

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		y += dy;
		dl += dst_stride;
	}

	__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(U8_3) (
    mlib_work_image *param)
{
	VARIABLE(mlib_u8);
	mlib_u8 *filter1 = GetElemStruct(filter1);
	mlib_s32 cx;
	mlib_u8 *tsp;
	mlib_f32 *prow;
	mlib_f32 *tdp;
	mlib_s32 fpos;
	mlib_d64 *dfPtr, dyf0, dyf1, dyf2, dyf3, dxf0;
	mlib_d64 *buff, *buffs, *buffd, *buffe, *buff_arr[8];
	mlib_d64 *buff0, *buff1, *buff2, *buff3, *buffx, *pbuff;
	mlib_s32 k, bind = 0, y_step = 4;
	mlib_s32 alp_ind, chan_d;
	mlib_f32 aa;
	mlib_blend blend;

	alp_ind = param->alp_ind;

	if (alp_ind) {
		mlib_s32 alp = (param->alpha) * (16384.0 / 255);

		((mlib_s32 *)&aa)[0] = (alp << 16) | (16384 - alp);
		chan_d = param->chan_d;
		blend = param->blend;
	}

	buff =
	    __mlib_malloc(width * 7 * sizeof (mlib_d64) + 4 * row_size + 8 * 8);

	if (buff == NULL)
		return (MLIB_FAILURE);
	for (k = 0; k < 4; k++)
		buff_arr[k] = buff + k * width;
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k];
	buffx = buff + 4 * width;
	buffd = buffx + width;
	buffe = buffd + width;
	buffs = buffe + width;

	x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	for (i = 0; i < width; i++) {
		fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
		buffx[i] = *(mlib_d64 *)((mlib_u8 *)FILTER_1 + fpos);
		x += dx;
	}

	sp -= src_stride + 3;
	tsp = sp;

	for (j = 0; j < height; j++) {
		COEFF_D64_U8(y);

		tdp = (mlib_f32 *)buffd;

		if (y_step >= 4) {
			y_step = 4;
			tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		} else {
			bind += y_step;

			if (bind >= 4)
				bind -= 4;
		}

		for (k = 4 - y_step; k < 3; k++) {
			mlib_f32 s0, s1, s2, s3;
			mlib_d64 r0, r1, r2, r3, r4, r5, r6;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			pbuff = buff_arr[bind + k];

			prow = (mlib_f32 *)buffs;
			mlib_v_ImageChannelInsert_U8_34R_D1((void *)tsp,
			    (void *)prow, row_size);

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				dxf0 = buffx[i];
				cx = x >> MLIB_SHIFT;

				s0 = prow[cx];
				s1 = prow[cx + 1];
				s2 = prow[cx + 2];
				s3 = prow[cx + 3];

				r0 = vis_fmul8x16au(s0, vis_read_hi(dxf0));
				r1 = vis_fmul8x16al(s1, vis_read_hi(dxf0));
				r2 = vis_fmul8x16au(s2, vis_read_lo(dxf0));
				r3 = vis_fmul8x16al(s3, vis_read_lo(dxf0));

				r4 = vis_fpadd16(r0, r1);
				r5 = vis_fpadd16(r2, r3);
				r6 = vis_fpadd16(r4, r5);
				pbuff[i] = r6;

				x += dx;
			}

			tsp += src_stride;
		}

		vis_write_gsr(3 << 3);

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];

		if (k < 4) {
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			prow = (mlib_f32 *)buffs;
			mlib_v_ImageChannelInsert_U8_34R_D1((void *)tsp,
			    (void *)prow, row_size);

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				mlib_f32 a0, a1, a2, a3;
				mlib_d64 b0, b1, b2, b3, b4, b5;
				mlib_d64 s0, s1, s2, s3, r0, r1, r2, r3, r4, r5,
				    r6;

				dxf0 = buffx[i];
				cx = x >> MLIB_SHIFT;

				a0 = prow[cx];
				a1 = prow[cx + 1];
				a2 = prow[cx + 2];
				a3 = prow[cx + 3];

				s0 = buff0[i];
				s1 = buff1[i];
				s2 = buff2[i];

				b0 = vis_fmul8x16au(a0, vis_read_hi(dxf0));
				b1 = vis_fmul8x16al(a1, vis_read_hi(dxf0));
				b2 = vis_fmul8x16au(a2, vis_read_lo(dxf0));
				b3 = vis_fmul8x16al(a3, vis_read_lo(dxf0));

				b4 = vis_fpadd16(b0, b1);
				b5 = vis_fpadd16(b2, b3);
				s3 = vis_fpadd16(b4, b5);

				r0 = FPMUL16(s0, dyf0);
				r1 = FPMUL16(s1, dyf1);
				r2 = FPMUL16(s2, dyf2);
				r3 = FPMUL16(s3, dyf3);

				r4 = vis_fpadd16(r0, r1);
				r5 = vis_fpadd16(r2, r3);
				r6 = vis_fpadd16(r4, r5);

				buff3[i] = s3;
				tdp[i] = vis_fpack16(r6);
				x += dx;
			}

			tsp += src_stride;
		} else {

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				mlib_d64 s0, s1, s2, s3, r0, r1, r2, r3, r4, r5,
				    r6;
				s0 = buff0[i];
				s1 = buff1[i];
				s2 = buff2[i];
				s3 = buff3[i];

				r0 = FPMUL16(s0, dyf0);
				r1 = FPMUL16(s1, dyf1);
				r2 = FPMUL16(s2, dyf2);
				r3 = FPMUL16(s3, dyf3);

				r4 = vis_fpadd16(r0, r1);
				r5 = vis_fpadd16(r2, r3);
				r6 = vis_fpadd16(r4, r5);

				tdp[i] = vis_fpack16(r6);
			}
		}

		if (!alp_ind) {
			mlib_v_ImageChannelExtract_U8_43R_D1((void *)tdp,
			    (void *)dl, width);
		} else if (chan_d == 3) {	/* ZoomBlend */
			mlib_u8 *dp, *dend = dl + 3 * width;
			mlib_s32 off = (mlib_s32)dl & 7;
			mlib_d64 s0, s1, ss, d0, d1, dd, *bp;

			mlib_v_ImageChannelExtract_U8_43R_D1((void *)buffd,
			    (void *)buffe, width);

			vis_write_gsr(1 << 3);

			dp = dl - off;
			bp = vis_alignaddr((mlib_u8 *)buffe - off, 0);
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
			mlib_u8 *sp = (void *)buffd;
			FP_TYPE w0 = param->alpha;
			FP_TYPE w1s = F_ONE - w0 * (F_ONE / 255);
			FP_TYPE w1, w, rw;

			if (blend == MLIB_BLEND_GTK_SRC) {
				if (alp_ind == -1) {
					mlib_ImageCopy_na(sp, dl - 1,
					    4 * width);
				} else {
					mlib_ImageCopy_na(sp + 1, dl,
					    4 * width);
				}
#pragma pipeloop(0)
				for (i = 0; i < width; i++) {
					dl[4 * i + alp_ind] = 255;
				}
			} else if (blend == MLIB_BLEND_GTK_SRC_OVER2) {
#pragma pipeloop(0)
				for (i = 0; i < width; i++) {
					w1 = w1s * dl[4 * i + alp_ind];
					w = w0 + w1;
					FP_DIV(rw, w0, w);

					BLEND34(dl[4 * i], sp[4 * i + 1]);
					BLEND34(dl[4 * i + 1], sp[4 * i + 2]);
					BLEND34(dl[4 * i + 2], sp[4 * i + 3]);
					dl[4 * i + alp_ind] = w;
				}
			} else {
#pragma pipeloop(0)
				for (i = 0; i < width; i++) {
					w1 = w1s * dl[4 * i + alp_ind];
					w = w0 + w1;
					FP_INV(rw, w);

					BLEND34z(dl[4 * i], sp[4 * i + 1]);
					BLEND34z(dl[4 * i + 1], sp[4 * i + 2]);
					BLEND34z(dl[4 * i + 2], sp[4 * i + 3]);
					dl[4 * i + alp_ind] = w;
				}
			}
		}

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		y += dy;
		dl += dst_stride;
	}

	__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
