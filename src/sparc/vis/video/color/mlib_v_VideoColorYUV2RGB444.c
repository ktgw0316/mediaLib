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

#pragma ident	"@(#)mlib_v_VideoColorYUV2RGB444.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV2RGB444 - color conversion from YUV444 to RGB
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorYUV2RGB444(mlib_u8       *rgb,
 *                                            const mlib_u8 *y,
 *                                            const mlib_u8 *u,
 *                                            const mlib_u8 *v,
 *                                            mlib_s32      width,
 *                                            mlib_s32      height,
 *                                            mlib_s32      rgb_stride,
 *                                            mlib_s32      yuv_stride);
 * ARGUMENT
 *      rgb             Pointer to the output RGB image
 *      y               Pointer to input Y component
 *      u               Pointer to input U component
 *      v               Pointer to input V component
 *      width           Width of the image
 *      height          Height of the image
 *      rgb_stride      Stride in bytes between adjacent rows in output image
 *      yuv_stride      Stride in bytes between adjacent rows in input image
 *
 * DESCRIPTION
 *        |R|   |1.1644   0.0000   1.5966|   [|Y|   | 16.0000|]
 *        |G| = |1.1644  -0.3920  -0.8132| * [|U| - |128.0000|]
 *        |B|   |1.1644   2.0184   0.0000|   [|V|   |128.0000|]
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorYUV2RGB444 = __mlib_VideoColorYUV2RGB444

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorYUV2RGB444) mlib_VideoColorYUV2RGB444
	__attribute__((weak, alias("__mlib_VideoColorYUV2RGB444")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	BUFF_SIZE	256

/* *********************************************************** */

#define	STORE_PIXEL(ind1, ind2, ind3)                           \
	vis_st_u8_i(blue, rgb, ind3);                           \
	blue = vis_faligndata(blue, blue);                      \
	vis_st_u8_i(green, rgb, ind2);                          \
	green = vis_faligndata(green, green);                   \
	vis_st_u8_i(red, rgb, ind1);                            \
	red = vis_faligndata(red, red)

/* *********************************************************** */

static void mlib_v_VideoColorYUV2RGB444_all_align(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 size);

static void mlib_v_VideoColorYUV2RGB444_nonalign(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width);

/* *********************************************************** */

mlib_status
__mlib_VideoColorYUV2RGB444(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 rgb_stride,
	mlib_s32 yuv_stride)
{
	mlib_s32 j;

	if (rgb == NULL || y == NULL || u == NULL || v == NULL)
		return (MLIB_NULLPOINTER);

	if (width <= 0 || height <= 0)
		return (MLIB_FAILURE);

	if (yuv_stride == width && rgb_stride == 3 * width) {
		width *= height;
		height = 1;
	}

/*
 * initialize GSR scale factor
 */
	vis_write_gsr((2 << 3) + 2);

	if (((((mlib_addr)y | (mlib_addr)u | (mlib_addr)v | (mlib_addr)rgb) & 3)
		== 0) && ((((yuv_stride | rgb_stride) & 3) == 0) ||
		(height == 1))) {

/* line loop */
		for (j = 0; j < height; j++) {
			mlib_v_VideoColorYUV2RGB444_all_align(rgb, y, u, v,
				width);
			rgb += rgb_stride;
			y += yuv_stride;
			u += yuv_stride;
			v += yuv_stride;
		}
	} else {

/* line loop */
		for (j = 0; j < height; j++) {
			mlib_v_VideoColorYUV2RGB444_nonalign(rgb, y, u, v,
				width);
			rgb += rgb_stride;
			y += yuv_stride;
			u += yuv_stride;
			v += yuv_stride;
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

static void
mlib_v_VideoColorYUV2RGB444_all_align(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 size)
{
	mlib_u8 *dend;
	mlib_f32 *sf0, *sf1, *sf2, *pfd, fzero = vis_fzeros();
	mlib_s32 i, n, m, emask;
	mlib_d64 *buff2, pbuff_arr2[BUFF_SIZE + 4];
	mlib_d64 tmp_arr64[2];
	mlib_d64 k01 = vis_to_double_dup(0x0000f375);
	mlib_d64 k02 = vis_to_double_dup(0x3317e5fa);
	mlib_d64 k11 = vis_to_double_dup(0xf3754097);
	mlib_d64 k12 = vis_to_double_dup(0xe5fa0000);
	mlib_d64 k21 = vis_to_double_dup(0x40970000);
	mlib_d64 k22 = vis_to_double_dup(0x00003317);
	mlib_d64 c_0 = vis_to_double_dup(0xe42010f4);
	mlib_d64 c_1 = vis_to_double_dup(0x10f4dd60);
	mlib_d64 c_2 = vis_to_double_dup(0xdd60e420);
	mlib_d64 k_0 = vis_to_double_dup(0x25432543);
	mlib_d64 dmask1001 = vis_to_double(0xffff0000, 0x0000ffff);

	do {
/* loop on buffer size */

		if (size > 2 * BUFF_SIZE) {
			n = 2 * BUFF_SIZE;
		} else {
			n = size;
		}

		m = (n - 1) >> 2;
		buff2 = pbuff_arr2;
		sf0 = (mlib_f32 *)y;
		sf1 = (mlib_f32 *)u;
		sf2 = (mlib_f32 *)v;
		dend = rgb + 3 * n - 1;

#pragma pipeloop(0)
		for (i = 0; i < (m + 1); i++) {
			mlib_d64 s00, s01, s02, s10, s11, s12, s20, s21, s22,
				s_0;
			mlib_f32 x0, x1, x2;

			x0 = (*sf0++);
			x1 = (*sf1++);
			x2 = (*sf2++);

			s_0 = vis_fmul8x16(x0, k_0);
			s01 = vis_fmul8x16(x1, k01);
			s11 = vis_fmul8x16(x1, k11);
			s21 = vis_fmul8x16(x1, k21);
			s02 = vis_fmul8x16(x2, k02);
			s12 = vis_fmul8x16(x2, k12);
			s22 = vis_fmul8x16(x2, k22);

			s00 = vis_fpadd16(s_0, s01);
			s10 = vis_fpadd16(s_0, s11);
			s20 = vis_fpadd16(s_0, s21);

			s02 = vis_fpadd16(s02, c_0);
			s12 = vis_fpadd16(s12, c_1);
			s22 = vis_fpadd16(s22, c_2);

			s00 = vis_fpadd16(s00, s02);
			s10 = vis_fpadd16(s10, s12);
			s20 = vis_fpadd16(s20, s22);

			((mlib_f32 *)buff2)[0] = vis_fpack16(s00);
			((mlib_f32 *)buff2)[1] = vis_fpack16(s10);
			((mlib_f32 *)buff2)[2] = fzero;
			((mlib_f32 *)buff2)[3] = vis_fpack16(s20);

			buff2 += 2;
		}

/*
 * merge buff values to 3-channel array
 */
		buff2 = pbuff_arr2;
		pfd = (mlib_f32 *)rgb;
		m = n >> 2;

#pragma pipeloop(0)
		for (i = 0; i < m; i++) {
			mlib_d64 d_0235, d_xx14, d_0xx5, d_x14x, d_23xx, d_0145;

			d_0235 = buff2[0];
			d_xx14 = buff2[1];

			d_0235 = vis_fpmerge(vis_read_hi(d_0235),
				vis_read_lo(d_0235));
			d_23xx = vis_faligndata(d_0235, d_0235);
			d_x14x = vis_faligndata(d_xx14, d_xx14);
			d_0xx5 = vis_fand(dmask1001, d_0235);
			d_0145 = vis_for(d_0xx5, d_x14x);

			pfd[0] = vis_read_hi(d_0145);
			pfd[1] = vis_read_hi(d_23xx);
			pfd[2] = vis_read_lo(d_0145);

			buff2 += 2;
			pfd += 3;
		}

/*
 * last pixels
 */

		if ((mlib_u8 *)pfd <= dend) {
			mlib_d64 d_0235, d_xx14, d_0xx5, d_x14x, d_23xx, d_0145;
			mlib_f32 *tmp_arr32 = (mlib_f32 *)tmp_arr64;

			d_0235 = buff2[0];
			d_xx14 = buff2[1];

			d_0235 = vis_fpmerge(vis_read_hi(d_0235),
				vis_read_lo(d_0235));
			d_23xx = vis_faligndata(d_0235, d_0235);
			d_x14x = vis_faligndata(d_xx14, d_xx14);
			d_0xx5 = vis_fand(dmask1001, d_0235);
			d_0145 = vis_for(d_0xx5, d_x14x);

			emask = vis_edge8(pfd, dend);

			if ((mlib_addr)pfd & 7) {
				pfd--;
				tmp_arr32++;
			}

			tmp_arr32[0] = vis_read_hi(d_0145);
			tmp_arr32[1] = vis_read_hi(d_23xx);
			tmp_arr32[2] = vis_read_lo(d_0145);

			vis_pst_8(tmp_arr64[0], pfd, emask);

			pfd += 2;
			emask = vis_edge8(pfd, dend);

			if ((mlib_u8 *)pfd <= dend)
				vis_pst_8(tmp_arr64[1], pfd, emask);
		}

		y += n;
		u += n;
		v += n;
		rgb += 3 * n;
		size -= n;

	} while (size);
}

/* *********************************************************** */

static void
mlib_v_VideoColorYUV2RGB444_nonalign(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width)
{
/* all. pointer to y, u, v */
	mlib_d64 *spy, *dfu, *dfv;

/* y data */
	mlib_d64 dy0, dy1, dy3;
	mlib_d64 du, dv, du0, du1, dv0, dv1;

/* (1.1644, 1.5966)*8192 */
	mlib_f32 k12 = vis_to_float(0x25433317);

/* (-.3920, -.8132)*8192 */
	mlib_f32 k34 = vis_to_float(0xf375e5fa);

/* 2.0184*8192 */
	mlib_f32 k5 = vis_to_float(0x1004097);
	mlib_d64 k_222_9952 = vis_to_double_dup(0x1be01be0);
	mlib_d64 k_135_6352 = vis_to_double_dup(0x10f410f4);
	mlib_d64 k_276_9856 = vis_to_double_dup(0x22a022a0);
	mlib_d64 u_3920_hi, u_20184_hi, v_15966_hi, v_8132_hi;
	mlib_d64 u_3920_lo, u_20184_lo, v_15966_lo, v_8132_lo;
	mlib_d64 y_11644_hi, y_11644_lo;
	mlib_d64 r_hi, r_lo, g_hi, g_lo, b_hi, b_lo;
	mlib_d64 red, green, blue;

/* loop variable */
	mlib_s32 i;

	dfu = (mlib_d64 *)vis_alignaddr((void *)u, 0);
	du0 = (*dfu++);
	du1 = vis_ld_d64_nf(dfu); dfu++;
	du = vis_faligndata(du0, du1);
	du0 = du1;

	dfv = (mlib_d64 *)vis_alignaddr((void *)v, 0);
	dv0 = (*dfv++);
	dv1 = vis_ld_d64_nf(dfv); dfv++;
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

	spy = (mlib_d64 *)vis_alignaddr((void *)y, 0);
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

	vis_alignaddr((void *)u, 0);
	du1 = vis_ld_d64_nf(dfu); dfu++;
	du = vis_faligndata(du0, du1);
	du0 = du1;

/* Y*1.1644 */
	y_11644_hi = vis_fmul8x16au(vis_read_hi(dy1), k12);
	b_hi = vis_fpsub16(u_20184_hi, k_276_9856);

	vis_alignaddr((void *)v, 0);
	dv1 = vis_ld_d64_nf(dfv); dfv++;
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
	g_hi = vis_fpadd16(g_hi, y_11644_hi);

	v_8132_lo = vis_fmul8x16al(vis_read_lo(dv), k34);
	g_lo = vis_fpadd16(g_lo, y_11644_lo);

	green = vis_fpack16_pair(g_hi, g_lo);
	b_hi = vis_fpadd16(b_hi, y_11644_hi);
	b_lo = vis_fpadd16(b_lo, y_11644_lo);

	blue = vis_fpack16_pair(b_hi, b_lo);
	r_hi = vis_fpadd16(r_hi, y_11644_hi);
	r_lo = vis_fpadd16(r_lo, y_11644_lo);

	red = vis_fpack16_pair(r_hi, r_lo);

	vis_alignaddr((void *)y, 0);
	dy3 = vis_ld_d64_nf(spy); spy++;
	dy1 = vis_faligndata(dy0, dy3);
	dy0 = dy3;

	vis_alignaddr(spy, 7);

/*
 * 16-pixel column loop
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

		vis_st_u8_i(blue, rgb, 2);
		vis_st_u8_i(green, rgb, 1);
		vis_st_u8_i(red, rgb, 0);

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
		vis_alignaddr((void *)u, 0);
		du1 = vis_ld_d64_nf(dfu); dfu++;
		du = vis_faligndata(du0, du1);
		du0 = du1;

/* Y*1.1644 */
		y_11644_hi = vis_fmul8x16au(vis_read_hi(dy1), k12);
		b_hi = vis_fpsub16(u_20184_hi, k_276_9856);
		vis_alignaddr((void *)v, 0);
		dv1 = vis_ld_d64_nf(dfv); dfv++;
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
		g_hi = vis_fpadd16(g_hi, y_11644_hi);

		v_8132_lo = vis_fmul8x16al(vis_read_lo(dv), k34);
		g_lo = vis_fpadd16(g_lo, y_11644_lo);

		green = vis_fpack16_pair(g_hi, g_lo);
		b_hi = vis_fpadd16(b_hi, y_11644_hi);
		b_lo = vis_fpadd16(b_lo, y_11644_lo);

		blue = vis_fpack16_pair(b_hi, b_lo);
		r_hi = vis_fpadd16(r_hi, y_11644_hi);
		r_lo = vis_fpadd16(r_lo, y_11644_lo);

		red = vis_fpack16_pair(r_hi, r_lo);

		vis_alignaddr((void *)y, 0);
		dy3 = vis_ld_d64_nf(spy); spy++;
		dy1 = vis_faligndata(dy0, dy3);
		dy0 = dy3;

		vis_alignaddr(spy, 7);

		rgb += 24;
	}

	vis_alignaddr((void *)(width - i), 0);
	blue = vis_faligndata(blue, blue);
	green = vis_faligndata(green, green);
	red = vis_faligndata(red, red);
	rgb += ((width - i - 1) * 3);

	vis_alignaddr(spy, 7);
	for (; i < width; i++) {
		STORE_PIXEL(0, 1, 2);
		rgb -= 3;
	}
}

/* *********************************************************** */
