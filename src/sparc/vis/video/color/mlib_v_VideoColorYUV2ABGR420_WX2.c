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

#pragma ident	"@(#)mlib_v_VideoColorYUV2ABGR420_WX2.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV2ABGR420_WX2 - color conversion-YUV420 to ABGR
 *                                         inside clipping window with zoom x2
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorYUV2ABGR420_WX2(mlib_u8       *abgr,
 *                                                 const mlib_u8 *y_inp,
 *                                                 const mlib_u8 *u_inp,
 *                                                 const mlib_u8 *v_inp,
 *                                                 mlib_s32      wd,
 *                                                 mlib_s32      ht,
 *                                                 mlib_s32      rgb_stride,
 *                                                 mlib_s32      y_stride,
 *                                                 mlib_s32      uv_stride,
 *                                                 mlib_s32      left,
 *                                                 mlib_s32      top,
 *                                                 mlib_s32      right,
 *                                                 mlib_s32      bottom);
 * ARGUMENT
 *      abgr        Pointer to output ABGR image
 *      y           Pointer to input Y component
 *      u           Pointer to input U component
 *      v           Pointer to input V component
 *      width       Width of the not zoomed image
 *      height      Height of the not zoomed image
 *      abgr_stride Stride in bytes between adjacent rows in output image
 *      y_stride    Stride in bytes between adjacent rows in Y input image
 *      uv_stride   Stride in bytes between adjacent rows in U and V input image
 *      left        left border of the clipping window
 *      top         top border of the clipping window
 *      right       right opened border of the clipping window
 *      bottom      bottom opened border of the clipping window
 *
 * DESCRIPTION
 *        |A| = 0
 *        |R|   |1.1644   0.0000   1.5966|   [Y   16.0000 ]
 *        |G| = |1.1644  -0.3920  -0.8132| * [U - 128.0000]
 *        |B|   |1.1644   2.0184   0.0000|   [V   128.0000]
 *
 * NOTES
 *  abgr and y must be 8-byte aligned
 *  u and v must be 4-byte aligned
 *  width must be a multiple of 8
 *  height must be a multiple of 2
 *  abgr_stride and y_stride must be a multiple of 8
 *  uv_stride must be a multiple of 4
 *  window borders must be: 0 <= left < right <= width*2
 *  and 0 <= top < bottom <= height*2
 */

#include <vis_proto.h>
#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorYUV2ABGR420_WX2 = \
	__mlib_VideoColorYUV2ABGR420_WX2

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorYUV2ABGR420_WX2) mlib_VideoColorYUV2ABGR420_WX2
	__attribute__((weak, alias("__mlib_VideoColorYUV2ABGR420_WX2")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoColorYUV2ABGR420_WX2(
	mlib_u8 *abgr,
	const mlib_u8 *y_inp,
	const mlib_u8 *u_inp,
	const mlib_u8 *v_inp,
	mlib_s32 wd,
	mlib_s32 ht,
	mlib_s32 rgb_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride,
	mlib_s32 left,
	mlib_s32 top,
	mlib_s32 right,
	mlib_s32 bottom)
{
	mlib_d64 *Y;
	mlib_f32 *U;
	mlib_f32 *V;
	mlib_d64 *Z;
	mlib_d64 *rgb0, *rgb1, *rgb2, *rgb3;

/* (1.1644, 1.5966)*8192 */
	mlib_f32 k12 = vis_to_float(0x25423317);

/* (-.3920, -.8132)*8192 */
	mlib_f32 k34 = vis_to_float(0xf374e5fa);

/* 2.0184*8192 */
	mlib_f32 k5 = vis_to_float(0x1004097);

	mlib_d64 k_222_9952 = vis_to_double_dup(0x1be01be0);
	mlib_d64 k_135_6352 = vis_to_double_dup(0x10f410f4);
	mlib_d64 k_276_9856 = vis_to_double_dup(0x229f229f);

	mlib_d64 u_3920_hi, u_20184_hi, v_15966_hi, v_8132_hi;
	mlib_d64 u_3920_lo, u_20184_lo, v_15966_lo, v_8132_lo;
	mlib_d64 y_11644_hi, y_11644_lo;
	mlib_d64 z_11644_hi, z_11644_lo;
	mlib_d64 r_hi, r_lo, g_hi, g_lo, b_hi, b_lo;
	mlib_d64 temp_r_hi, temp_r_lo, temp_g_hi, temp_g_lo, temp_b_hi,
		temp_b_lo;
	mlib_f32 red_hi, red_lo, green_hi, green_lo, blue_hi, blue_lo;
	mlib_f32 red2_lo, green2_lo, blue2_lo;
	mlib_d64 blue_red_hi, x_green_hi, blue_red_lo, x_green_lo;

	mlib_s32 i, j;
	mlib_d64 u2, v2, y, z, val, val1, val2;
	mlib_f32 u, v;

	mlib_s32 mask, maskt1, maskb1, maskt2, maskb2;

	if (wd <= 0 || ht <= 0)
		return (MLIB_FAILURE);

	if (wd & 7 || ht & 1 || rgb_stride & 7 || uv_stride & 3 || y_stride & 7)
		return (MLIB_FAILURE);

	if (left < 0 || top < 0 || right <= left || bottom <= top ||
		2 * wd < right || 2 * ht < bottom)
		return (MLIB_FAILURE);

/*
 * expand subwindow to normal aligned ( 8*wd x 2*ht pixels )
 */
	abgr += (left & ~15) * 4 + (top & ~3) * rgb_stride;
	y_inp += ((left & ~15) >> 1) + ((top & ~3) >> 1) * y_stride;
	u_inp += ((left & ~15) >> 2) + (top >> 2) * uv_stride;
	v_inp += ((left & ~15) >> 2) + (top >> 2) * uv_stride;

	ht = ((bottom + 3) & ~3) - (top & ~3);
	wd = ((right + 15) & ~15) - (left & ~15);
	bottom *= 2;

	vis_write_gsr((2 << 3) + 4);

	Y = (mlib_d64 *)y_inp;
	U = (mlib_f32 *)u_inp;
	V = (mlib_f32 *)v_inp;

/*
 * Load u, v only for the first time
 */
	u = (*U++);
	v = (*V++);
	Z = (mlib_d64 *)((mlib_u8 *)Y + y_stride);

/* First scan line. */
	y = (*Y++);
/* Second scan line. */
	z = (*Z++);

	rgb0 = (mlib_d64 *)abgr;
	rgb1 = (mlib_d64 *)((mlib_u8 *)rgb0 + rgb_stride);
	rgb2 = (mlib_d64 *)((mlib_u8 *)rgb1 + rgb_stride);
	rgb3 = (mlib_d64 *)((mlib_u8 *)rgb2 + rgb_stride);

	uv_stride = uv_stride - (wd >> 2) - 4;
	y_stride = 2 * y_stride - (wd >> 1) - 8;
	rgb_stride = (4 * rgb_stride) - (wd << 2);

	wd >>= 4;
	j = ht >> 2;

	maskb2 = vis_edge16((void *)(top * 2), (void *)(bottom - 2));
	maskt1 = ((maskb2 << 28) >> 31);
	maskt2 = ((maskb2 << 29) >> 31);
	maskb1 = ((maskb2 << 30) >> 31);
	maskb2 = ((maskb2 << 31) >> 31);

	left = (0xffff >> (left & 15));
	right = (0xffff << ((-right) & 15));
	do {
		u2 = vis_fpmerge(u, u);
		v2 = vis_fpmerge(v, v);
/* U*(-0.3920); */
		u_3920_hi = vis_fmul8x16au(vis_read_hi(u2), k34);
/* V*(-0.8132); */
		v_8132_hi = vis_fmul8x16al(vis_read_hi(v2), k34);
		u_3920_lo = vis_fmul8x16au(vis_read_lo(u2), k34);
		v_8132_lo = vis_fmul8x16al(vis_read_lo(v2), k34);

		mask = left & (right | ~((wd - 2) >> 31));

/* U*2.0184 */
		u_20184_hi = vis_fmul8x16al(vis_read_hi(u2), k5);
		g_hi = vis_fpadd16(u_3920_hi, v_8132_hi);

/* V*1.5966 */
		v_15966_hi = vis_fmul8x16al(vis_read_hi(v2), k12);
		g_hi = vis_fpadd16(g_hi, k_135_6352);

#pragma pipeloop(0)
		for (i = 0; i < wd; i++) {
			u_20184_lo = vis_fmul8x16al(vis_read_lo(u2), k5);
			g_lo = vis_fpadd16(u_3920_lo, v_8132_lo);

			v_15966_lo = vis_fmul8x16al(vis_read_lo(v2), k12);
			g_lo = vis_fpadd16(g_lo, k_135_6352);

/* Y*1.1644 */
			y_11644_hi = vis_fmul8x16au(vis_read_hi(y), k12);
			b_hi = vis_fpsub16(u_20184_hi, k_276_9856);

/* Y*1.1644 */
			y_11644_lo = vis_fmul8x16au(vis_read_lo(y), k12);
			b_lo = vis_fpsub16(u_20184_lo, k_276_9856);

/* Z*1.1644 */
			z_11644_hi = vis_fmul8x16au(vis_read_hi(z), k12);
/* take care of  U - 128 */
			r_hi = vis_fpsub16(v_15966_hi, k_222_9952);

/* Z*1.1644 */
			z_11644_lo = vis_fmul8x16au(vis_read_lo(z), k12);
			r_lo = vis_fpsub16(v_15966_lo, k_222_9952);

			temp_g_hi = vis_fpadd16(g_hi, y_11644_hi);

			temp_b_hi = vis_fpadd16(b_hi, y_11644_hi);

			green_hi = vis_fpack16(temp_g_hi);
			temp_r_hi = vis_fpadd16(r_hi, y_11644_hi);

			blue_hi = vis_fpack16(temp_b_hi);
			temp_g_lo = vis_fpadd16(g_lo, y_11644_lo);

			red_hi = vis_fpack16(temp_r_hi);
			temp_b_lo = vis_fpadd16(b_lo, y_11644_lo);

			green_lo = vis_fpack16(temp_g_lo);
			temp_r_lo = vis_fpadd16(r_lo, y_11644_lo);

			blue_lo = vis_fpack16(temp_b_lo);
			x_green_hi = vis_fmul8x16au(green_hi, k5);

			red_lo = vis_fpack16(temp_r_lo);
			blue_red_hi = vis_fpmerge(blue_hi, red_hi);

			temp_g_hi = vis_fpadd16(g_hi, z_11644_hi);

			temp_b_hi = vis_fpadd16(b_hi, z_11644_hi);

			green_hi = vis_fpack16(temp_g_hi);
			temp_r_hi = vis_fpadd16(r_hi, z_11644_hi);

			blue_hi = vis_fpack16(temp_b_hi);
			temp_g_lo = vis_fpadd16(g_lo, z_11644_lo);

			red_hi = vis_fpack16(temp_r_hi);
			temp_b_lo = vis_fpadd16(b_lo, z_11644_lo);

			u = vis_ld_f32_nf(U); U++;
			green2_lo = vis_fpack16(temp_g_lo);
			temp_r_lo = vis_fpadd16(r_lo, z_11644_lo);

			v = vis_ld_f32_nf(V); V++;
			blue2_lo = vis_fpack16(temp_b_lo);
			x_green_lo = vis_fmul8x16au(green_lo, k5);

/* First scan line. */
			y = (*Y++);
			red2_lo = vis_fpack16(temp_r_lo);
			blue_red_lo = vis_fpmerge(blue_lo, red_lo);
/* Second scan line. */
			z = vis_ld_d64_nf(Z); Z++;

			val1 = vis_fpmerge(vis_read_hi(x_green_hi),
				vis_read_hi(blue_red_hi));
			val2 = vis_faligndata(val1, val1);
			val = vis_faligndata(val2, val1);
			vis_pst_32(val, rgb0, (mask & maskt1) >> 14);
			vis_pst_32(val, rgb1, (mask & maskt2) >> 14);
			val = vis_faligndata(val1, val2);
			vis_pst_32(val, rgb0 + 1, (mask & maskt1) >> 12);
			vis_pst_32(val, rgb1 + 1, (mask & maskt2) >> 12);

			val1 = vis_fpmerge(vis_read_lo(x_green_hi),
				vis_read_lo(blue_red_hi));
			val2 = vis_faligndata(val1, val1);
			val = vis_faligndata(val2, val1);
			vis_pst_32(val, rgb0 + 2, (mask & maskt1) >> 10);
			vis_pst_32(val, rgb1 + 2, (mask & maskt2) >> 10);
			val = vis_faligndata(val1, val2);
			vis_pst_32(val, rgb0 + 3, (mask & maskt1) >> 8);
			vis_pst_32(val, rgb1 + 3, (mask & maskt2) >> 8);

			x_green_hi = vis_fmul8x16au(green_hi, k5);

			val1 = vis_fpmerge(vis_read_hi(x_green_lo),
				vis_read_hi(blue_red_lo));
			val2 = vis_faligndata(val1, val1);
			val = vis_faligndata(val2, val1);
			vis_pst_32(val, rgb0 + 4, (mask & maskt1) >> 6);
			vis_pst_32(val, rgb1 + 4, (mask & maskt2) >> 6);
			val = vis_faligndata(val1, val2);
			vis_pst_32(val, rgb0 + 5, (mask & maskt1) >> 4);
			vis_pst_32(val, rgb1 + 5, (mask & maskt2) >> 4);

			val1 = vis_fpmerge(vis_read_lo(x_green_lo),
				vis_read_lo(blue_red_lo));
			val2 = vis_faligndata(val1, val1);
			val = vis_faligndata(val2, val1);
			vis_pst_32(val, rgb0 + 6, (mask & maskt1) >> 2);
			vis_pst_32(val, rgb1 + 6, (mask & maskt2) >> 2);
			val = vis_faligndata(val1, val2);
			vis_pst_32(val, rgb0 + 7, (mask & maskt1));
			vis_pst_32(val, rgb1 + 7, (mask & maskt2));

			blue_red_hi = vis_fpmerge(blue_hi, red_hi);

			val1 = vis_fpmerge(vis_read_hi(x_green_hi),
				vis_read_hi(blue_red_hi));
			val2 = vis_faligndata(val1, val1);
			val = vis_faligndata(val2, val1);
			vis_pst_32(val, rgb2, (mask & maskb1) >> 14);
			vis_pst_32(val, rgb3, (mask & maskb2) >> 14);
			val = vis_faligndata(val1, val2);
			vis_pst_32(val, rgb2 + 1, (mask & maskb1) >> 12);
			vis_pst_32(val, rgb3 + 1, (mask & maskb2) >> 12);

			val1 = vis_fpmerge(vis_read_lo(x_green_hi),
				vis_read_lo(blue_red_hi));
			val2 = vis_faligndata(val1, val1);
			val = vis_faligndata(val2, val1);
			vis_pst_32(val, rgb2 + 2, (mask & maskb1) >> 10);
			vis_pst_32(val, rgb3 + 2, (mask & maskb2) >> 10);
			val = vis_faligndata(val1, val2);
			vis_pst_32(val, rgb2 + 3, (mask & maskb1) >> 8);
			vis_pst_32(val, rgb3 + 3, (mask & maskb2) >> 8);

			u2 = vis_fpmerge(u, u);
			v2 = vis_fpmerge(v, v);

			x_green_lo = vis_fmul8x16au(green2_lo, k5);
/* U*(-0.3920); */
			u_3920_hi = vis_fmul8x16au(vis_read_hi(u2), k34);

			blue_red_lo = vis_fpmerge(blue2_lo, red2_lo);
/* V*(-0.8132); */
			v_8132_hi = vis_fmul8x16al(vis_read_hi(v2), k34);

			val1 = vis_fpmerge(vis_read_hi(x_green_lo),
				vis_read_hi(blue_red_lo));
			val2 = vis_faligndata(val1, val1);
			val = vis_faligndata(val2, val1);
			vis_pst_32(val, rgb2 + 4, (mask & maskb1) >> 6);
			vis_pst_32(val, rgb3 + 4, (mask & maskb2) >> 6);
			val = vis_faligndata(val1, val2);
			vis_pst_32(val, rgb2 + 5, (mask & maskb1) >> 4);
			vis_pst_32(val, rgb3 + 5, (mask & maskb2) >> 4);

			u_3920_lo = vis_fmul8x16au(vis_read_lo(u2), k34);
			v_8132_lo = vis_fmul8x16al(vis_read_lo(v2), k34);

			val1 = vis_fpmerge(vis_read_lo(x_green_lo),
				vis_read_lo(blue_red_lo));
			val2 = vis_faligndata(val1, val1);
			val = vis_faligndata(val2, val1);
			vis_pst_32(val, rgb2 + 6, (mask & maskb1) >> 2);
			vis_pst_32(val, rgb3 + 6, (mask & maskb2) >> 2);
			val = vis_faligndata(val1, val2);
			vis_pst_32(val, rgb2 + 7, (mask & maskb1));
			vis_pst_32(val, rgb3 + 7, (mask & maskb2));

			mask = (right | ~((wd - 3 - i) >> 31));

/* U*2.0184 */
			u_20184_hi = vis_fmul8x16al(vis_read_hi(u2), k5);
			g_hi = vis_fpadd16(u_3920_hi, v_8132_hi);

/* V*1.5966 */
			v_15966_hi = vis_fmul8x16al(vis_read_hi(v2), k12);
			g_hi = vis_fpadd16(g_hi, k_135_6352);

			rgb0 += 8;
			rgb1 += 8;
			rgb2 += 8;
			rgb3 += 8;
		}

		Y = (mlib_d64 *)((mlib_u8 *)Y + y_stride);
		Z = (mlib_d64 *)((mlib_u8 *)Z + y_stride);
		rgb0 = (mlib_d64 *)((mlib_u8 *)rgb0 + rgb_stride);
		rgb1 = (mlib_d64 *)((mlib_u8 *)rgb1 + rgb_stride);
		rgb2 = (mlib_d64 *)((mlib_u8 *)rgb2 + rgb_stride);
		rgb3 = (mlib_d64 *)((mlib_u8 *)rgb3 + rgb_stride);
		U = (mlib_f32 *)((mlib_u8 *)U + uv_stride);
		V = (mlib_f32 *)((mlib_u8 *)V + uv_stride);

		maskb2 = vis_edge16((void *)((bottom - (j - 1) * 8 + 7) & ~7),
			(void *)(bottom - 2));
		maskt1 = -1;
		maskt2 = ((maskb2 << 29) >> 31);
		maskb1 = ((maskb2 << 30) >> 31);
		maskb2 = ((maskb2 << 31) >> 31);

		y = vis_ld_d64_nf(Y); Y++;
		z = vis_ld_d64_nf(Z); Z++;
		u = vis_ld_f32_nf(U); U++;
		v = vis_ld_f32_nf(V); V++;
	} while (--j);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
