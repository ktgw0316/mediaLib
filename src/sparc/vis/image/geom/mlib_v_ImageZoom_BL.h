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

#ifndef _MLIB_V_IMAGEZOOM_BL_H
#define	_MLIB_V_IMAGEZOOM_BL_H

#pragma ident	"@(#)mlib_v_ImageZoom_BL.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

/* *********************************************************** */

#if IMG_TYPE == 2  /* MLIB_SHORT */

#define	FUNC_NAME(CC)	mlib_v_ImageZoomBilinear_S16_##CC
#define	DEF_MASK

#elif IMG_TYPE == 3	/* IMG_TYPE == 2 ( MLIB_SHORT ) */

#define	FUNC_NAME(CC)	mlib_v_ImageZoomBilinear_U16_##CC
#define	DEF_MASK	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);

#endif		   /* IMG_TYPE == 2 ( MLIB_SHORT ) */

/* *********************************************************** */

#define	BUFF_SIZE	2048

/* *********************************************************** */

#define	VARIABLE(FORMAT)                                             \
	FORMAT *sl = GetElemSubStruct(current, sp),                  \
		*dl = GetElemSubStruct(current, dp),                 \
		*sp = sl;                                            \
	mlib_s32 row_size = param -> sline_size;                     \
	mlib_s32 x_max = (row_size - 1) << MLIB_SHIFT;               \
	mlib_s32 i, j,                                               \
		x = GetElemSubStruct(current, srcX) & MLIB_MASK,     \
		y = GetElemSubStruct(current, srcY) & MLIB_MASK,     \
		src_stride = GetElemStruct(src_stride),              \
		dst_stride = GetElemStruct(dst_stride),              \
		width  = GetElemSubStruct(current, width),           \
		height = GetElemSubStruct(current, height);          \
	mlib_d64 mask_7FFF = vis_to_double_dup(0x7FFF7FFF);          \
	DEF_MASK

/* *********************************************************** */

#define	LD_4_PIX_U16(a0, a1, sp, NCHAN)                          \
	a0 = vis_faligndata(vis_ld_u16(sp + cx3), a0);           \
	a1 = vis_faligndata(vis_ld_u16(sp + NCHAN + cx3), a1);   \
	a0 = vis_faligndata(vis_ld_u16(sp + cx2), a0);           \
	a1 = vis_faligndata(vis_ld_u16(sp + NCHAN + cx2), a1);   \
	a0 = vis_faligndata(vis_ld_u16(sp + cx1), a0);           \
	a1 = vis_faligndata(vis_ld_u16(sp + NCHAN + cx1), a1);   \
	a0 = vis_faligndata(vis_ld_u16(sp + cx0), a0);           \
	a1 = vis_faligndata(vis_ld_u16(sp + NCHAN + cx0), a1)

/* *********************************************************** */

#define	GET_POS_X() x = GetElemSubStruct(current, srcX) & MLIB_MASK

/* *********************************************************** */

#define	GET_POS_Y()                                             \
	y0 = ((mlib_s32)d_pos) & 0x7FFF;                        \
	ddy = vis_to_double_dup((y0 << 16) | y0);               \
	rdy = vis_fpsub16(mask_7FFF, ddy)

/* *********************************************************** */

#define	NEXT_LINE()                                             \
	y_step =                                                \
	    ((mlib_s32)(d_pos + d_step) >> 15) -                \
	    ((mlib_s32)d_pos >> 15);                            \
	d_pos += d_step;                                        \
	dl += dst_stride;                                       \
	sl += y_step * src_stride;                              \
	sp = sl

/* *********************************************************** */

/* arguments (x, y) are swapped to prevent overflow */
#define	FMULY_16x16(z, x, y)                                          \
	z = vis_fpadd16(vis_fmul8sux16(y, x), vis_fmul8ulx16(y, x))

/* *********************************************************** */

#if IMG_TYPE == 2  /* MLIB_SHORT */

#define	XOR(x)	x
#define	FMULX_16x16(z, x, y)	FMULY_16x16(z, x, y)

#else		   /* IMG_TYPE == 2 ( MLIB_SHORT ) */

#define	XOR(x)	vis_fxor(x, mask8000)
#define	FMULX_16x16(z, x, y)	x = XOR(x); FMULY_16x16(z, x, y)

#endif		   /* IMG_TYPE == 2 ( MLIB_SHORT ) */

/* *********************************************************** */

mlib_status FUNC_NAME(1)  (
    mlib_work_image *param)
{
	VARIABLE(mlib_s16)
	mlib_s32 dx = GetElemStruct(DX);
	mlib_d64 buff_arr[BUFF_SIZE / 8], *buff0, *buff1, *buffx;
	void *buff = buff_arr, *buffd;
	mlib_d64 *dp;
	mlib_s16 *sp2;
	mlib_s32 x0, x1, x2, x3, cx0, cx1, cx2, cx3, y0;
	mlib_s32 width4, size, y_step = 2, dx4 = dx * 4;
	mlib_d64 d_pos, d_step, ddy, rdy;

	width4 = (width + 3) / 4;
	size = 8 * 4 * width4 + 8;

	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + width4;
	buffx = buff1 + width4;
	buffd = buffx + width4;

/* calculate dx for all row */
	d_pos = (mlib_d64)x *0.5;

	d_step = (1 << 15) / GetElemStruct(zoomx);
#pragma pipeloop(0)
	for (i = 0; i < width; i++) {
		mlib_s32 ires = ((mlib_s32)d_pos) - ((x >> MLIB_SHIFT) << 15);

		if (ires > MLIB_S16_MAX)
			ires = MLIB_S16_MAX;
		if (ires < 0)
			ires = 0;
		((mlib_s16 *)buffx)[i] = ires;

		d_pos += d_step;
		x += dx;
	}

	d_pos = (mlib_d64)y *0.5;

	d_step = (1 << 15) / GetElemStruct(zoomy);

	for (j = 0; j < height; j++) {
		vis_write_gsr(6);

		if (y_step > 1) {
			GET_POS_X();
			x0 = x;
			x1 = x0 + dx;
			x2 = x0 + 2 * dx;
			x3 = x1 + 2 * dx;

#pragma pipeloop(0)
			for (i = 0; i < width4; i++) {
				mlib_d64 ddx, rdx, sd2, sd3, v2, v3, y1;

				cx0 = (x0 >> MLIB_SHIFT);
				cx1 = (x1 >> MLIB_SHIFT);
				cx2 = (x2 >> MLIB_SHIFT);
				cx3 = (x3 >> MLIB_SHIFT);

				cx1 &= ((x1 - x_max) >> 31);
				cx2 &= ((x2 - x_max) >> 31);
				cx3 &= ((x3 - x_max) >> 31);

				x0 += dx4;
				x1 += dx4;
				x2 += dx4;
				x3 += dx4;

				ddx = buffx[i];
				rdx = vis_fpsub16(mask_7FFF, ddx);

				LD_4_PIX_U16(sd2, sd3, sp, 1);
				FMULX_16x16(v2, sd2, rdx);
				FMULX_16x16(v3, sd3, ddx);
				y1 = vis_fpadd16(v2, v3);
				y1 = vis_fpadd16(y1, y1);

				buff1[i] = y1;
			}
		}

		if (((mlib_addr)dl & 7) || (width & 3)) {
			dp = (mlib_d64 *)buffd;
		} else {
			dp = (mlib_d64 *)dl;
		}

		GET_POS_Y();

		if (y_step == 0) {
#pragma pipeloop(0)
			for (i = 0; i < width4; i++) {
				mlib_d64 y0, y1, z0, z1, dd;

				y0 = buff0[i];
				y1 = buff1[i];

				FMULY_16x16(z0, y0, rdy);
				FMULY_16x16(z1, y1, ddy);
				dd = vis_fpadd16(z0, z1);
				dd = vis_fpadd16(dd, dd);

				dp[i] = XOR(dd);
			}

		} else {
			mlib_d64 *buffT;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			GET_POS_X();
			x0 = x;
			x1 = x0 + dx;
			x2 = x0 + 2 * dx;
			x3 = x1 + 2 * dx;

			sp2 = sp + src_stride;

#pragma pipeloop(0)
			for (i = 0; i < width4; i++) {
				mlib_d64 ddx, rdx, sd2, sd3, v2, v3, y0, y1, z0,
				    z1, dd;

				cx0 = (x0 >> MLIB_SHIFT);
				cx1 = (x1 >> MLIB_SHIFT);
				cx2 = (x2 >> MLIB_SHIFT);
				cx3 = (x3 >> MLIB_SHIFT);

				cx1 &= ((x1 - x_max) >> 31);
				cx2 &= ((x2 - x_max) >> 31);
				cx3 &= ((x3 - x_max) >> 31);

				ddx = buffx[i];
				rdx = vis_fpsub16(mask_7FFF, ddx);

				LD_4_PIX_U16(sd2, sd3, sp2, 1);
				FMULX_16x16(v2, sd2, rdx);
				FMULX_16x16(v3, sd3, ddx);

				y0 = buff0[i];
				y1 = vis_fpadd16(v2, v3);
				y1 = vis_fpadd16(y1, y1);

				FMULY_16x16(z0, y0, rdy);
				FMULY_16x16(z1, y1, ddy);
				dd = vis_fpadd16(z0, z1);
				dd = vis_fpadd16(dd, dd);

				x0 += dx4;
				x1 += dx4;
				x2 += dx4;
				x3 += dx4;

				buff1[i] = y1;
				dp[i] = XOR(dd);
			}
		}

		if ((mlib_s16 *)dp != dl)
			mlib_ImageCopy_na((void *)dp, (void *)dl, 2 * width);

		NEXT_LINE();
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
	VARIABLE(mlib_s16)
	mlib_s32 dx = GetElemStruct(DX);
	mlib_d64 buff_arr[BUFF_SIZE / 8], *buff0, *buff1, *buffx;
	void *buff = buff_arr, *buffd, *row_0, *row_1;
	mlib_d64 *dp;
	mlib_f32 *sp0, *sp1, *sp2, *sp3;
	mlib_s32 x0, x1, cx0, cx1, y0;
	mlib_s32 width2, size, y_step = 2, dx2 = dx * 2;
	mlib_d64 d_pos, d_step, ddy, rdy;

	width2 = (width + 1) / 2;
	row_size *= 4;
	size = 8 * 4 * width2 + 2 * row_size;

	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + width2;
	buffx = buff1 + width2;
	buffd = buffx + width2;
	row_0 = (mlib_d64 *)buffd + width2;
	row_1 = (mlib_u8 *)row_0 + row_size;

/* calculate dx for all row */
	d_pos = (mlib_d64)x *0.5;

	d_step = (1 << 15) / GetElemStruct(zoomx);
#pragma pipeloop(0)
	for (i = 0; i < width; i++) {
		mlib_s32 ires = ((mlib_s32)d_pos) - ((x >> MLIB_SHIFT) << 15);

		if (ires > MLIB_S16_MAX)
			ires = MLIB_S16_MAX;
		if (ires < 0)
			ires = 0;
		ires = (ires << 16) | ires;
		((mlib_s32 *)buffx)[i] = ires;
		d_pos += d_step;
		x += dx;
	}

	d_pos = (mlib_d64)y *0.5;

	d_step = (1 << 15) / GetElemStruct(zoomy);

	for (j = 0; j < height; j++) {

		if (((mlib_addr)dl & 7) || (width & 1)) {
			dp = (mlib_d64 *)buffd;
		} else {
			dp = (mlib_d64 *)dl;
		}

		GET_POS_Y();

		if (y_step == 0) {
#pragma pipeloop(0)
			for (i = 0; i < width2; i++) {
				mlib_d64 y0, y1, z0, z1, dd;

				y0 = buff0[i];
				y1 = buff1[i];

				FMULY_16x16(z0, y0, rdy);
				FMULY_16x16(z1, y1, ddy);
				dd = vis_fpadd16(z0, z1);
				dd = vis_fpadd16(dd, dd);

				dp[i] = XOR(dd);
			}

		} else if (y_step == 1) {
			mlib_d64 *buffT;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			GET_POS_X();
			x0 = x;
			x1 = x0 + dx;
			sp2 = (mlib_f32 *)(sp + src_stride);

			if ((mlib_addr)sp2 & 3) {
				mlib_ImageCopy_na((void *)sp2, row_1, row_size);
				sp2 = row_1;
			}

			sp3 = sp2 + 1;

#pragma pipeloop(0)
			for (i = 0; i < width2; i++) {
				mlib_d64 ddx, rdx, sd2, sd3, v2, v3, y0, y1, z0,
				    z1, dd;
				mlib_f32 sf2, sf3, se2, se3;

				cx0 = (x0 >> MLIB_SHIFT);
				cx1 = (x1 >> MLIB_SHIFT) & ((x1 - x_max) >> 31);

				ddx = buffx[i];
				rdx = vis_fpsub16(mask_7FFF, ddx);

				sf2 = sp2[cx0];
				se2 = sp2[cx1];
				sf3 = sp3[cx0];
				se3 = sp3[cx1];

				sd2 = vis_freg_pair(sf2, se2);
				sd3 = vis_freg_pair(sf3, se3);
				FMULX_16x16(v2, sd2, rdx);
				FMULX_16x16(v3, sd3, ddx);

				y0 = buff0[i];
				y1 = vis_fpadd16(v2, v3);
				y1 = vis_fpadd16(y1, y1);

				FMULY_16x16(z0, y0, rdy);
				FMULY_16x16(z1, y1, ddy);
				dd = vis_fpadd16(z0, z1);
				dd = vis_fpadd16(dd, dd);

				x0 += dx2;
				x1 += dx2;

				buff1[i] = y1;
				dp[i] = XOR(dd);
			}

		} else {

			GET_POS_X();
			x0 = x;
			x1 = x0 + dx;
			sp0 = (mlib_f32 *)sp;
			sp2 = (mlib_f32 *)(sp + src_stride);

			if ((mlib_addr)sp0 & 3) {
				mlib_ImageCopy_na((void *)sp0, row_0, row_size);
				sp0 = row_0;
			}

			sp1 = sp0 + 1;

			if ((mlib_addr)sp2 & 3) {
				mlib_ImageCopy_na((void *)sp2, row_1, row_size);
				sp2 = row_1;
			}

			sp3 = sp2 + 1;

#pragma pipeloop(0)
			for (i = 0; i < width2; i++) {
				mlib_d64 ddx, rdx, sd0, sd1, sd2, sd3, v0, v1,
				    v2, v3, y0, y1, z0, z1, dd;
				mlib_f32 sf0, sf1, sf2, sf3, se0, se1, se2, se3;

				cx0 = (x0 >> MLIB_SHIFT);
				cx1 = (x1 >> MLIB_SHIFT) & ((x1 - x_max) >> 31);

				ddx = buffx[i];
				rdx = vis_fpsub16(mask_7FFF, ddx);

				sf0 = sp0[cx0];
				se0 = sp0[cx1];
				sf1 = sp1[cx0];
				se1 = sp1[cx1];
				sf2 = sp2[cx0];
				se2 = sp2[cx1];
				sf3 = sp3[cx0];
				se3 = sp3[cx1];

				sd0 = vis_freg_pair(sf0, se0);
				sd1 = vis_freg_pair(sf1, se1);
				sd2 = vis_freg_pair(sf2, se2);
				sd3 = vis_freg_pair(sf3, se3);

				FMULX_16x16(v0, sd0, rdx);
				FMULX_16x16(v1, sd1, ddx);
				y0 = vis_fpadd16(v0, v1);
				y0 = vis_fpadd16(y0, y0);

				FMULX_16x16(v2, sd2, rdx);
				FMULX_16x16(v3, sd3, ddx);
				y1 = vis_fpadd16(v2, v3);
				y1 = vis_fpadd16(y1, y1);

				FMULY_16x16(z0, y0, rdy);
				FMULY_16x16(z1, y1, ddy);
				dd = vis_fpadd16(z0, z1);
				dd = vis_fpadd16(dd, dd);

				x0 += dx2;
				x1 += dx2;

				buff0[i] = y0;
				buff1[i] = y1;
				dp[i] = XOR(dd);
			}
		}

		if ((mlib_s16 *)dp != dl)
			mlib_ImageCopy_na((void *)dp, (void *)dl, 4 * width);

		NEXT_LINE();
	}

	if (size > BUFF_SIZE) {
		__mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(3)  (
    mlib_work_image *param)
{
	VARIABLE(mlib_s16)
	mlib_d64 buff_arr[BUFF_SIZE / 8], *buff0, *buff1, *buffx;
	void *buff = buff_arr, *buffd;
	mlib_d64 *dp;
	mlib_s16 *sp2;
	mlib_s32 y0;
	mlib_s32 width4, size, y_step = 2;
	mlib_d64 d_pos, d_step, ddy, rdy;
	mlib_s32 *buffc;
	mlib_u16 *buffcc;
	mlib_s32 cx, ires0;

	width4 = (width + 3) / 4;

	size = 10 * 12 * width4 + 32;

	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buffx = buff;
	buff0 = buffx + 3 * width4;
	buff1 = buff0 + 3 * width4;
	buffd = buff1 + 3 * width4;
	buffc = (mlib_s32 *)buffd + 6 * width4;

/* calculate x, dx for all row */
	d_pos = (mlib_d64)x *0.5;

	d_step = (1 << 15) / GetElemStruct(zoomx);
	buffcc = (mlib_u16 *)buffc;
#pragma pipeloop(0)
	for (i = 0; i < width; i++) {
		ires0 = (mlib_s32)d_pos & 0x7FFF;
		cx = (mlib_s32)d_pos >> 15;
		cx += 2 * cx;
		buffcc[3 * i] = cx;
		buffcc[3 * i + 1] = cx + 1;
		buffcc[3 * i + 2] = cx + 2;
		d_pos += d_step;

		((mlib_s16 *)buffx)[3 * i] = ires0;
		((mlib_s16 *)buffx)[3 * i + 1] = ires0;
		((mlib_s16 *)buffx)[3 * i + 2] = ires0;
	}
	for (; i < 4 * width4; i++) {
		buffcc[3 * i] = cx;
		buffcc[3 * i + 1] = cx + 1;
		buffcc[3 * i + 2] = cx + 2;

		((mlib_s16 *)buffx)[3 * i] = ires0;
		((mlib_s16 *)buffx)[3 * i + 1] = ires0;
		((mlib_s16 *)buffx)[3 * i + 2] = ires0;
	}

	d_pos = (mlib_d64)y *0.5;

	d_step = (1 << 15) / GetElemStruct(zoomy);

	for (j = 0; j < height; j++) {
		vis_write_gsr(6);

		if (y_step > 1) {
			GET_POS_X();

#pragma pipeloop(0)
			for (i = 0; i < 3 * width4; i++) {
				mlib_d64 ddx, rdx, sd2, sd3, v2, v3, y1;
				mlib_s32 cx01, cx23, cx0, cx1, cx2, cx3;

				cx01 = buffc[2 * i];
				cx23 = buffc[2 * i + 1];
				cx0 = cx01 >> 16;
				cx1 = cx01 & 0xFFFF;
				cx2 = cx23 >> 16;
				cx3 = cx23 & 0xFFFF;

				ddx = buffx[i];
				rdx = vis_fpsub16(mask_7FFF, ddx);

				LD_4_PIX_U16(sd2, sd3, sp, 3);

				FMULX_16x16(v2, sd2, rdx);
				FMULX_16x16(v3, sd3, ddx);
				y1 = vis_fpadd16(v2, v3);
				y1 = vis_fpadd16(y1, y1);

				buff1[i] = y1;
			}
		}

		if (((mlib_addr)dl & 7) || (width & 3)) {
			dp = (mlib_d64 *)buffd;
		} else {
			dp = (mlib_d64 *)dl;
		}

		GET_POS_Y();

		if (y_step == 0) {
#pragma pipeloop(0)
			for (i = 0; i < 3 * width4; i++) {
				mlib_d64 y0, y1, z0, z1, dd;

				y0 = buff0[i];
				y1 = buff1[i];

				FMULY_16x16(z0, y0, rdy);
				FMULY_16x16(z1, y1, ddy);
				dd = vis_fpadd16(z0, z1);
				dd = vis_fpadd16(dd, dd);

				dp[i] = XOR(dd);
			}

		} else {
			mlib_d64 *buffT;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			GET_POS_X();
			sp2 = sp + src_stride;

#pragma pipeloop(0)
			for (i = 0; i < 3 * width4; i++) {
				mlib_d64 ddx, rdx, sd2, sd3, v2, v3, y0, y1, z0,
				    z1, dd;
				mlib_s32 cx01, cx23, cx0, cx1, cx2, cx3;

				cx01 = buffc[2 * i];
				cx23 = buffc[2 * i + 1];
				cx0 = cx01 >> 16;
				cx1 = cx01 & 0xFFFF;
				cx2 = cx23 >> 16;
				cx3 = cx23 & 0xFFFF;

				ddx = buffx[i];
				rdx = vis_fpsub16(mask_7FFF, ddx);

				LD_4_PIX_U16(sd2, sd3, sp2, 3);

				FMULX_16x16(v2, sd2, rdx);
				FMULX_16x16(v3, sd3, ddx);

				y0 = buff0[i];
				y1 = vis_fpadd16(v2, v3);
				y1 = vis_fpadd16(y1, y1);

				FMULY_16x16(z0, y0, rdy);
				FMULY_16x16(z1, y1, ddy);
				dd = vis_fpadd16(z0, z1);
				dd = vis_fpadd16(dd, dd);

				buff1[i] = y1;
				dp[i] = XOR(dd);
			}
		}

		if ((mlib_s16 *)dp != dl)
			mlib_ImageCopy_na((void *)dp, (void *)dl, 6 * width);

		NEXT_LINE();
	}

	if (size > BUFF_SIZE) {
		__mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(4)  (
    mlib_work_image *param)
{
	VARIABLE(mlib_s16)
	mlib_s32 dx = GetElemStruct(DX);
	mlib_d64 buff_arr[BUFF_SIZE / 8], *buff0, *buff1, *buffx;
	void *buff = buff_arr, *buffd, *row_0, *row_1;
	mlib_d64 *dp;
	mlib_d64 *sp0, *sp1, *sp2, *sp3;
	mlib_s32 x0, cx0, y0;
	mlib_s32 size, y_step = 2;
	mlib_d64 ddy, rdy, d_pos, d_step;

	row_size *= 8;
	size = 8 * 4 * width + 2 * row_size;

	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + width;
	buffx = buff1 + width;
	buffd = buffx + width;
	row_0 = (mlib_d64 *)buffd + width;
	row_1 = (mlib_u8 *)row_0 + row_size;

/* calculate dx for all row */
	d_pos = (mlib_d64)x *0.5;

	d_step = (1 << 15) / GetElemStruct(zoomx);
#pragma pipeloop(0)
	for (i = 0; i < width; i++) {
		mlib_s32 ires = ((mlib_s32)d_pos) - ((x >> MLIB_SHIFT) << 15);

		if (ires > MLIB_S16_MAX)
			ires = MLIB_S16_MAX;
		if (ires < 0)
			ires = 0;

		ires = (ires << 16) | ires;
		((mlib_s32 *)buffx)[2 * i] = ires;
		((mlib_s32 *)buffx)[2 * i + 1] = ires;
		d_pos += d_step;
		x += dx;
	}

	d_pos = (mlib_d64)y *0.5;

	d_step = (1 << 15) / GetElemStruct(zoomy);

	for (j = 0; j < height; j++) {

		if ((mlib_addr)dl & 7) {
			dp = (mlib_d64 *)buffd;
		} else {
			dp = (mlib_d64 *)dl;
		}

		GET_POS_Y();

		if (y_step == 0) {
#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				mlib_d64 y0, y1, z0, z1, dd;

				y0 = buff0[i];
				y1 = buff1[i];

				FMULY_16x16(z0, y0, rdy);
				FMULY_16x16(z1, y1, ddy);
				dd = vis_fpadd16(z0, z1);
				dd = vis_fpadd16(dd, dd);

				dp[i] = XOR(dd);
			}

		} else if (y_step == 1) {
			mlib_d64 *buffT;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			GET_POS_X();
			x0 = x;
			sp2 = (mlib_d64 *)(sp + src_stride);

			if ((mlib_addr)sp2 & 7) {
				mlib_ImageCopy_na((void *)sp2, row_1, row_size);
				sp2 = row_1;
			}

			sp3 = sp2 + 1;

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				mlib_d64 sd2, sd3, ddx, rdx, v2, v3, y0, y1, z0,
				    z1, dd;

				cx0 = (x0 >> MLIB_SHIFT);

				ddx = buffx[i];
				rdx = vis_fpsub16(mask_7FFF, ddx);

				sd2 = sp2[cx0];
				sd3 = sp3[cx0];
				FMULX_16x16(v2, sd2, rdx);
				FMULX_16x16(v3, sd3, ddx);

				y0 = buff0[i];
				y1 = vis_fpadd16(v2, v3);
				y1 = vis_fpadd16(y1, y1);

				FMULY_16x16(z0, y0, rdy);
				FMULY_16x16(z1, y1, ddy);
				dd = vis_fpadd16(z0, z1);
				dd = vis_fpadd16(dd, dd);

				x0 += dx;

				buff1[i] = y1;
				dp[i] = XOR(dd);
			}

		} else {
			GET_POS_X();
			x0 = x;
			sp0 = (mlib_d64 *)sp;
			sp2 = (mlib_d64 *)(sp + src_stride);

			if ((mlib_addr)sp0 & 7) {
				mlib_ImageCopy_na((void *)sp0, row_0, row_size);
				sp0 = row_0;
			}

			sp1 = sp0 + 1;

			if ((mlib_addr)sp2 & 7) {
				mlib_ImageCopy_na((void *)sp2, row_1, row_size);
				sp2 = row_1;
			}

			sp3 = sp2 + 1;

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				mlib_d64 sd0, sd1, sd2, sd3, ddx, rdx;
				mlib_d64 v0, v1, v2, v3, y0, y1, z0, z1, dd;

				cx0 = (x0 >> MLIB_SHIFT);

				ddx = buffx[i];
				rdx = vis_fpsub16(mask_7FFF, ddx);

				sd0 = sp0[cx0];
				sd1 = sp1[cx0];
				sd2 = sp2[cx0];
				sd3 = sp3[cx0];

				FMULX_16x16(v0, sd0, rdx);
				FMULX_16x16(v1, sd1, ddx);
				y0 = vis_fpadd16(v0, v1);
				y0 = vis_fpadd16(y0, y0);

				FMULX_16x16(v2, sd2, rdx);
				FMULX_16x16(v3, sd3, ddx);
				y1 = vis_fpadd16(v2, v3);
				y1 = vis_fpadd16(y1, y1);

				FMULY_16x16(z0, y0, rdy);
				FMULY_16x16(z1, y1, ddy);
				dd = vis_fpadd16(z0, z1);
				dd = vis_fpadd16(dd, dd);

				x0 += dx;

				buff0[i] = y0;
				buff1[i] = y1;
				dp[i] = XOR(dd);
			}
		}

		if ((mlib_s16 *)dp != dl)
			mlib_ImageCopy_na((void *)dp, (void *)dl, 8 * width);

		NEXT_LINE();
	}

	if (size > BUFF_SIZE) {
		__mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_IMAGEZOOM_BL_H */
