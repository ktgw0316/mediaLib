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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_v_VideoColorYUV2RGB422.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV2RGB422 - color conversion from YUV422 to RGB
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorYUV2RGB422(mlib_u8       *rgb,
 *                                            const mlib_u8 *y,
 *                                            const mlib_u8 *u,
 *                                            const mlib_u8 *v,
 *                                            mlib_s32      width,
 *                                            mlib_s32      height,
 *                                            mlib_s32      rgb_stride,
 *                                            mlib_s32      y_stride,
 *                                            mlib_s32      uv_stride);
 * ARGUMENT
 *  rgb        Pointer to the output RGB image
 *  y          Pointer to input Y component
 *  u          Pointer to input U component
 *  v          Pointer to input V component
 *  width      Width of the image. Note that width must be a multiple of 2
 *  height     Height of the image
 *  rgb_stride Stride in bytes between adjacent rows in output image
 *  y_stride   Stride in bytes between adjacent rows in Y input image
 *  uv_stride  Stride in bytes between adjacent rows in U and V input
 *             image
 *
 * DESCRIPTION
 *      mlib_ImageColorConvert2()
 *        |R|   |1.1644   0.0000   1.5966|   |Y|   |16.0000|
 *        |G| = |1.1644  -0.3920  -0.8132| * |U| - |128.0000|
 *        |B|   |1.1644   2.0184   0.0000|   |V|   |128.0000|
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorYUV2RGB422 = __mlib_VideoColorYUV2RGB422

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorYUV2RGB422) mlib_VideoColorYUV2RGB422
	__attribute__((weak, alias("__mlib_VideoColorYUV2RGB422")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	STORE_PIXEL(ind1, ind2, ind3)                           \
	vis_st_u8_i(blue, dp, ind3);                            \
	blue = vis_faligndata(blue, blue);                      \
	vis_st_u8_i(green, dp, ind2);                           \
	green = vis_faligndata(green, green);                   \
	vis_st_u8_i(red, dp, ind1);                             \
	red = vis_faligndata(red, red)

/* *********************************************************** */

static mlib_status mlib_v_VideoColorYUV2RGB422_all_align(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 rgb_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride);

static mlib_status mlib_v_VideoColorYUV2RGB422_nonalign(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 rgb_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride);

/* *********************************************************** */

mlib_status
__mlib_VideoColorYUV2RGB422(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 rgb_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride)
{
	if (rgb == NULL || y == NULL || u == NULL || v == NULL)
		return (MLIB_NULLPOINTER);

	if (width <= 0 || height <= 0)
		return (MLIB_FAILURE);

	if (width & 1)
		return (MLIB_FAILURE);

	if (y_stride == width && uv_stride == width / 2 &&
		rgb_stride == 3 * width) {

		width *= height;
		height = 1;
	}

	if ((((mlib_addr)u & 3) == 0) && (((mlib_addr)v & 3) == 0) &&
		(((mlib_addr)y & 7) == 0) && ((((uv_stride & 3) == 0) &&
		((y_stride & 7) == 0)) || (height == 1))) {

		return mlib_v_VideoColorYUV2RGB422_all_align(rgb, y, u, v,
			width, height, rgb_stride, y_stride, uv_stride);
	} else {
		return mlib_v_VideoColorYUV2RGB422_nonalign(rgb, y, u, v,
			width, height, rgb_stride, y_stride, uv_stride);
	}
}

/* *********************************************************** */

static mlib_status
mlib_v_VideoColorYUV2RGB422_all_align(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 rgb_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride)
{
/* pointers to src address */
	mlib_u8 *sp2, *sp3, *sl2, *sl3;

/* pointers to src address */
	mlib_u8 *sp1, *sl1;

/* pointers to dst address */
	mlib_u8 *dp, *dl;

/* all. pointer to y */
	mlib_d64 *spy;

/* all. pointers to u, v */
	mlib_f32 *dfu, *dfv;

/* u, v data */
	mlib_f32 fu, fv;

/* y data */
	mlib_d64 dy1;
	mlib_d64 du, dv;

/* (1.1644, 1.5966)*8192 */
	mlib_f32 k12 = vis_to_float(0x25433317);

/* (-.3920, -.8132)*8192 */
	mlib_f32 k34 = vis_to_float(0xf375e5fa);

/* 2.0184*8192 */
	mlib_f32 k5 = vis_to_float(0x1004097);
	mlib_d64 k_222_9952 = vis_to_double(0x1be01be0, 0x1be01be0);
	mlib_d64 k_135_6352 = vis_to_double(0x10f410f4, 0x10f410f4);
	mlib_d64 k_276_9856 = vis_to_double(0x22a022a0, 0x22a022a0);
	mlib_d64 u_3920_hi, u_20184_hi, v_15966_hi, v_8132_hi;
	mlib_d64 u_3920_lo, u_20184_lo, v_15966_lo, v_8132_lo;
	mlib_d64 y_11644_hi, y_11644_lo;
	mlib_d64 r_hi, r_lo, g_hi, g_lo, b_hi, b_lo;
	mlib_d64 temp_r_hi, temp_r_lo, temp_g_hi, temp_g_lo, temp_b_hi,
		temp_b_lo;
	mlib_d64 red, green, blue;

/* loop variables */
	mlib_s32 i, j;

/*
 * initialize GSR scale factor
 */
	vis_write_gsr((2 << 3) + 7);

	sp1 = sl1 = (mlib_u8 *)y;
	sp2 = sl2 = (mlib_u8 *)u;
	sp3 = sl3 = (mlib_u8 *)v;

	dl = dp = (mlib_u8 *)rgb;

/*
 * row loop
 */
	for (j = 0; j < height; j++) {
		spy = (mlib_d64 *)sp1;
		dfu = (mlib_f32 *)sp2;
		dfv = (mlib_f32 *)sp3;

		fu = (*dfu++);
		fv = (*dfv++);

		du = vis_fpmerge(fu, fu);
		dv = vis_fpmerge(fv, fv);

/* U*(-0.3920); */
		u_3920_hi = vis_fmul8x16au(vis_read_hi(du), k34);
/* V*(-0.8132); */
		v_8132_hi = vis_fmul8x16al(vis_read_hi(dv), k34);
/* U*(-0.3920); */
		u_3920_lo = vis_fmul8x16au(vis_read_lo(du), k34);
/* V*(-0.8132); */
		v_8132_lo = vis_fmul8x16al(vis_read_lo(dv), k34);

		dy1 = (*spy++);

/* U*2.0184 */
		u_20184_hi = vis_fmul8x16al(vis_read_hi(du), k5);
		g_hi = vis_fpadd16(u_3920_hi, v_8132_hi);

		u_20184_lo = vis_fmul8x16al(vis_read_lo(du), k5);
		g_hi = vis_fpadd16(g_hi, k_135_6352);

/* V*1.5966 */
		v_15966_hi = vis_fmul8x16al(vis_read_hi(dv), k12);
		g_lo = vis_fpadd16(u_3920_lo, v_8132_lo);

		v_15966_lo = vis_fmul8x16al(vis_read_lo(dv), k12);
		g_lo = vis_fpadd16(g_lo, k_135_6352);
		fu = vis_ld_f32_nf(dfu); dfu++;

/* Y*1.1644 */
		y_11644_hi = vis_fmul8x16au(vis_read_hi(dy1), k12);
		b_hi = vis_fpsub16(u_20184_hi, k_276_9856);
		fv = vis_ld_f32_nf(dfv); dfv++;

		du = vis_fpmerge(fu, fu);
		dv = vis_fpmerge(fv, fv);

/* Y*1.1644 */
		y_11644_lo = vis_fmul8x16au(vis_read_lo(dy1), k12);
		b_lo = vis_fpsub16(u_20184_lo, k_276_9856);

/* U*(-0.3920); */
		u_3920_hi = vis_fmul8x16au(vis_read_hi(du), k34);
		r_hi = vis_fpsub16(v_15966_hi, k_222_9952);

/* V*(-0.8132); */
		v_8132_hi = vis_fmul8x16al(vis_read_hi(dv), k34);
		r_lo = vis_fpsub16(v_15966_lo, k_222_9952);

		u_3920_lo = vis_fmul8x16au(vis_read_lo(du), k34);
		temp_g_hi = vis_fpadd16(g_hi, y_11644_hi);

		v_8132_lo = vis_fmul8x16al(vis_read_lo(dv), k34);
		temp_g_lo = vis_fpadd16(g_lo, y_11644_lo);

		green = vis_fpack16_pair(temp_g_hi, temp_g_lo);
		temp_b_hi = vis_fpadd16(b_hi, y_11644_hi);
		temp_b_lo = vis_fpadd16(b_lo, y_11644_lo);

		blue = vis_fpack16_pair(temp_b_hi, temp_b_lo);
		temp_r_hi = vis_fpadd16(r_hi, y_11644_hi);
		temp_r_lo = vis_fpadd16(r_lo, y_11644_lo);

		red = vis_fpack16_pair(temp_r_hi, temp_r_lo);

		dy1 = vis_ld_d64_nf(spy); spy++;

/*
 * 8-pixel column loop
 */
#pragma pipeloop(0)
		for (i = 0; i <= width - 8; i += 8) {

			STORE_PIXEL(21, 22, 23);
			STORE_PIXEL(18, 19, 20);
			STORE_PIXEL(15, 16, 17);
			STORE_PIXEL(12, 13, 14);
			STORE_PIXEL(9, 10, 11);
			STORE_PIXEL(6, 7, 8);
			STORE_PIXEL(3, 4, 5);

			vis_st_u8_i(blue, dp, 2);
			vis_st_u8_i(green, dp, 1);
			vis_st_u8_i(red, dp, 0);

/* U*2.0184 */
			u_20184_hi = vis_fmul8x16al(vis_read_hi(du), k5);
			g_hi = vis_fpadd16(u_3920_hi, v_8132_hi);

			u_20184_lo = vis_fmul8x16al(vis_read_lo(du), k5);
			g_hi = vis_fpadd16(g_hi, k_135_6352);

/* V*1.5966 */
			v_15966_hi = vis_fmul8x16al(vis_read_hi(dv), k12);
			g_lo = vis_fpadd16(u_3920_lo, v_8132_lo);

			v_15966_lo = vis_fmul8x16al(vis_read_lo(dv), k12);
			g_lo = vis_fpadd16(g_lo, k_135_6352);
			fu = vis_ld_f32_nf(dfu); dfu++;

/* Y*1.1644 */
			y_11644_hi = vis_fmul8x16au(vis_read_hi(dy1), k12);
			b_hi = vis_fpsub16(u_20184_hi, k_276_9856);
			fv = vis_ld_f32_nf(dfv); dfv++;

			du = vis_fpmerge(fu, fu);
			dv = vis_fpmerge(fv, fv);

/* Y*1.1644 */
			y_11644_lo = vis_fmul8x16au(vis_read_lo(dy1), k12);
			b_lo = vis_fpsub16(u_20184_lo, k_276_9856);

/* U*(-0.3920); */
			u_3920_hi = vis_fmul8x16au(vis_read_hi(du), k34);
			r_hi = vis_fpsub16(v_15966_hi, k_222_9952);

/* V*(-0.8132); */
			v_8132_hi = vis_fmul8x16al(vis_read_hi(dv), k34);
			r_lo = vis_fpsub16(v_15966_lo, k_222_9952);

			u_3920_lo = vis_fmul8x16au(vis_read_lo(du), k34);
			temp_g_hi = vis_fpadd16(g_hi, y_11644_hi);

			v_8132_lo = vis_fmul8x16al(vis_read_lo(dv), k34);
			temp_g_lo = vis_fpadd16(g_lo, y_11644_lo);

			green = vis_fpack16_pair(temp_g_hi, temp_g_lo);
			temp_b_hi = vis_fpadd16(b_hi, y_11644_hi);
			temp_b_lo = vis_fpadd16(b_lo, y_11644_lo);

			blue = vis_fpack16_pair(temp_b_hi, temp_b_lo);
			temp_r_hi = vis_fpadd16(r_hi, y_11644_hi);
			temp_r_lo = vis_fpadd16(r_lo, y_11644_lo);

			red = vis_fpack16_pair(temp_r_hi, temp_r_lo);
			dy1 = vis_ld_d64_nf(spy); spy++;
			dp += 24;
		}

		vis_alignaddr((void *)(width - i), 0);
		blue = vis_faligndata(blue, blue);
		green = vis_faligndata(green, green);
		red = vis_faligndata(red, red);
		dp += ((width - i - 1) * 3);

		vis_alignaddr((void *)7, 0);
		for (; i < width; i++) {
			STORE_PIXEL(0, 1, 2);
			dp -= 3;
		}

		sp1 = sl1 = sl1 + y_stride;
		sp2 = sl2 = sl2 + uv_stride;
		sp3 = sl3 = sl3 + uv_stride;

		dl = dp = dl + rgb_stride;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

static mlib_status
mlib_v_VideoColorYUV2RGB422_nonalign(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 rgb_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride)
{
/* pointers to src address */
	mlib_u8 *sp2, *sp3, *sl2, *sl3;

/* pointers to src address */
	mlib_u8 *sp1, *sl1;

/* pointers to dst address */
	mlib_u8 *dp, *dl;

/* all. pointer to y */
	mlib_d64 *spy;

/* all. pointers to u, v */
	mlib_f32 *dfu, *dfv;

/* u, v data */
	mlib_f32 fu0, fu1, fv0, fv1;

/* y data */
	mlib_d64 dy0, dy1, dy3;
	mlib_d64 du, dv, du0, du1, dv0, dv1;

/* (1.1644, 1.5966)*8192 */
	mlib_f32 k12 = vis_to_float(0x25433317);

/* (-.3920, -.8132)*8192 */
	mlib_f32 k34 = vis_to_float(0xf375e5fa);

/* 2.0184*8192 */
	mlib_f32 k5 = vis_to_float(0x1004097);
	mlib_d64 k_222_9952 = vis_to_double(0x1be01be0, 0x1be01be0);
	mlib_d64 k_135_6352 = vis_to_double(0x10f410f4, 0x10f410f4);
	mlib_d64 k_276_9856 = vis_to_double(0x22a022a0, 0x22a022a0);
	mlib_d64 u_3920_hi, u_20184_hi, v_15966_hi, v_8132_hi;
	mlib_d64 u_3920_lo, u_20184_lo, v_15966_lo, v_8132_lo;
	mlib_d64 y_11644_hi, y_11644_lo;
	mlib_d64 r_hi, r_lo, g_hi, g_lo, b_hi, b_lo;
	mlib_d64 temp_r_hi, temp_r_lo, temp_g_hi, temp_g_lo, temp_b_hi,
		temp_b_lo;
/* loop variables */
	mlib_s32 i, j;
	mlib_d64 red, green, blue;
	mlib_s32 off2, off3;

/*
 * initialize GSR scale factor
 */
	vis_write_gsr(2 << 3);

	sp1 = sl1 = (mlib_u8 *)y;
	sp2 = sl2 = (mlib_u8 *)u;
	sp3 = sl3 = (mlib_u8 *)v;

	dl = dp = (mlib_u8 *)rgb;

/*
 * row loop
 */
	for (j = 0; j < height; j++) {
		spy = (mlib_d64 *)vis_alignaddr(sp1, 0);
		dfu = (mlib_f32 *)((mlib_addr)sp2 & ~3);
		off2 = (sp2 - (mlib_u8 *)dfu) * 2;
		dfv = (mlib_f32 *)((mlib_addr)sp3 & ~3);
		off3 = (sp3 - (mlib_u8 *)dfv) * 2;

		vis_alignaddr((void *)off2, 0);
		fu0 = (*dfu++);
		fu1 = vis_ld_f32_nf(dfu); dfu++;
		du0 = vis_fpmerge(fu0, fu0);
		du1 = vis_fpmerge(fu1, fu1);
		du = vis_faligndata(du0, du1);
		du0 = du1;

		vis_alignaddr((void *)off3, 0);
		fv0 = (*dfv++);
		fv1 = vis_ld_f32_nf(dfv); dfv++;
		dv0 = vis_fpmerge(fv0, fv0);
		dv1 = vis_fpmerge(fv1, fv1);
		dv = vis_faligndata(dv0, dv1);
		dv0 = dv1;

/* U*(-0.3920); */
		u_3920_hi = vis_fmul8x16au(vis_read_hi(du), k34);
/* V*(-0.8132); */
		v_8132_hi = vis_fmul8x16al(vis_read_hi(dv), k34);
/* U*(-0.3920); */
		u_3920_lo = vis_fmul8x16au(vis_read_lo(du), k34);
/* V*(-0.8132); */
		v_8132_lo = vis_fmul8x16al(vis_read_lo(dv), k34);

		vis_alignaddr(sp1, 0);
		dy0 = (*spy++);
		dy3 = vis_ld_d64_nf(spy); spy++;
		dy1 = vis_faligndata(dy0, dy3);
		dy0 = dy3;

/* U*2.0184 */
		u_20184_hi = vis_fmul8x16al(vis_read_hi(du), k5);
		g_hi = vis_fpadd16(u_3920_hi, v_8132_hi);

		u_20184_lo = vis_fmul8x16al(vis_read_lo(du), k5);
		g_hi = vis_fpadd16(g_hi, k_135_6352);

/* V*1.5966 */
		v_15966_hi = vis_fmul8x16al(vis_read_hi(dv), k12);
		g_lo = vis_fpadd16(u_3920_lo, v_8132_lo);

		v_15966_lo = vis_fmul8x16al(vis_read_lo(dv), k12);
		g_lo = vis_fpadd16(g_lo, k_135_6352);

		vis_alignaddr((void *)off2, 0);
		fu1 = vis_ld_f32_nf(dfu); dfu++;
		du1 = vis_fpmerge(fu1, fu1);
		du = vis_faligndata(du0, du1);
		du0 = du1;

/* Y*1.1644 */
		y_11644_hi = vis_fmul8x16au(vis_read_hi(dy1), k12);
		b_hi = vis_fpsub16(u_20184_hi, k_276_9856);

		vis_alignaddr((void *)off3, 0);
		fv1 = vis_ld_f32_nf(dfv); dfv++;
		dv1 = vis_fpmerge(fv1, fv1);
		dv = vis_faligndata(dv0, dv1);
		dv0 = dv1;

/* Y*1.1644 */
		y_11644_lo = vis_fmul8x16au(vis_read_lo(dy1), k12);
		b_lo = vis_fpsub16(u_20184_lo, k_276_9856);

/* U*(-0.3920); */
		u_3920_hi = vis_fmul8x16au(vis_read_hi(du), k34);
		r_hi = vis_fpsub16(v_15966_hi, k_222_9952);

/* V*(-0.8132); */
		v_8132_hi = vis_fmul8x16al(vis_read_hi(dv), k34);
		r_lo = vis_fpsub16(v_15966_lo, k_222_9952);

		u_3920_lo = vis_fmul8x16au(vis_read_lo(du), k34);
		temp_g_hi = vis_fpadd16(g_hi, y_11644_hi);

		v_8132_lo = vis_fmul8x16al(vis_read_lo(dv), k34);
		temp_g_lo = vis_fpadd16(g_lo, y_11644_lo);

		green = vis_fpack16_pair(temp_g_hi, temp_g_lo);
		temp_b_hi = vis_fpadd16(b_hi, y_11644_hi);
		temp_b_lo = vis_fpadd16(b_lo, y_11644_lo);

		blue = vis_fpack16_pair(temp_b_hi, temp_b_lo);
		temp_r_hi = vis_fpadd16(r_hi, y_11644_hi);
		temp_r_lo = vis_fpadd16(r_lo, y_11644_lo);

		red = vis_fpack16_pair(temp_r_hi, temp_r_lo);

		vis_alignaddr(sp1, 0);
		dy3 = vis_ld_d64_nf(spy); spy++;
		dy1 = vis_faligndata(dy0, dy3);
		dy0 = dy3;

/*
 * 8-pixel column loop
 */
#pragma pipeloop(0)
		for (i = 0; i <= width - 8; i += 8) {

			vis_alignaddr((void *)7, 0);
			STORE_PIXEL(21, 22, 23);
			STORE_PIXEL(18, 19, 20);
			STORE_PIXEL(15, 16, 17);
			STORE_PIXEL(12, 13, 14);
			STORE_PIXEL(9, 10, 11);
			STORE_PIXEL(6, 7, 8);
			STORE_PIXEL(3, 4, 5);

			vis_st_u8_i(blue, dp, 2);
			vis_st_u8_i(green, dp, 1);
			vis_st_u8_i(red, dp, 0);

/* U*2.0184 */
			u_20184_hi = vis_fmul8x16al(vis_read_hi(du), k5);
			g_hi = vis_fpadd16(u_3920_hi, v_8132_hi);

			u_20184_lo = vis_fmul8x16al(vis_read_lo(du), k5);
			g_hi = vis_fpadd16(g_hi, k_135_6352);

/* V*1.5966 */
			v_15966_hi = vis_fmul8x16al(vis_read_hi(dv), k12);
			g_lo = vis_fpadd16(u_3920_lo, v_8132_lo);

			v_15966_lo = vis_fmul8x16al(vis_read_lo(dv), k12);
			g_lo = vis_fpadd16(g_lo, k_135_6352);

			vis_alignaddr((void *)off2, 0);
			fu1 = vis_ld_f32_nf(dfu); dfu++;
			du1 = vis_fpmerge(fu1, fu1);
			du = vis_faligndata(du0, du1);
			du0 = du1;

/* Y*1.1644 */
			y_11644_hi = vis_fmul8x16au(vis_read_hi(dy1), k12);
			b_hi = vis_fpsub16(u_20184_hi, k_276_9856);

			vis_alignaddr((void *)off3, 0);
			fv1 = vis_ld_f32_nf(dfv); dfv++;
			dv1 = vis_fpmerge(fv1, fv1);
			dv = vis_faligndata(dv0, dv1);
			dv0 = dv1;

/* Y*1.1644 */
			y_11644_lo = vis_fmul8x16au(vis_read_lo(dy1), k12);
			b_lo = vis_fpsub16(u_20184_lo, k_276_9856);

/* U*(-0.3920); */
			u_3920_hi = vis_fmul8x16au(vis_read_hi(du), k34);
			r_hi = vis_fpsub16(v_15966_hi, k_222_9952);

/* V*(-0.8132); */
			v_8132_hi = vis_fmul8x16al(vis_read_hi(dv), k34);
			r_lo = vis_fpsub16(v_15966_lo, k_222_9952);

			u_3920_lo = vis_fmul8x16au(vis_read_lo(du), k34);
			temp_g_hi = vis_fpadd16(g_hi, y_11644_hi);

			v_8132_lo = vis_fmul8x16al(vis_read_lo(dv), k34);
			temp_g_lo = vis_fpadd16(g_lo, y_11644_lo);

			green = vis_fpack16_pair(temp_g_hi, temp_g_lo);
			temp_b_hi = vis_fpadd16(b_hi, y_11644_hi);
			temp_b_lo = vis_fpadd16(b_lo, y_11644_lo);

			blue = vis_fpack16_pair(temp_b_hi, temp_b_lo);
			temp_r_hi = vis_fpadd16(r_hi, y_11644_hi);
			temp_r_lo = vis_fpadd16(r_lo, y_11644_lo);

			red = vis_fpack16_pair(temp_r_hi, temp_r_lo);

			vis_alignaddr(sp1, 0);
			dy3 = vis_ld_d64_nf(spy); spy++;
			dy1 = vis_faligndata(dy0, dy3);
			dy0 = dy3;
			dp += 24;
		}

		vis_alignaddr((void *)(width - i), 0);
		blue = vis_faligndata(blue, blue);
		green = vis_faligndata(green, green);
		red = vis_faligndata(red, red);
		dp += ((width - i - 1) * 3);

		vis_alignaddr((void *)7, 0);
		for (; i < width; i++) {
			STORE_PIXEL(0, 1, 2);
			dp -= 3;
		}

		sp1 = sl1 = sl1 + y_stride;
		sp2 = sl2 = sl2 + uv_stride;
		sp3 = sl3 = sl3 + uv_stride;

		dl = dp = dl + rgb_stride;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
