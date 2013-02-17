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

#pragma ident	"@(#)mlib_v_VideoColorYUV2RGB420.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV2RGB420 - color conversion from YUV420 to RGB
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorYUV2RGB420(mlib_u8 *rgb,
 *                                            const mlib_u8 *y,
 *                                            const mlib_u8 *u,
 *                                            const mlib_u8 *v,
 *                                            mlib_s32 width,
 *                                            mlib_s32 height,
 *                                            mlib_s32 rgb_stride,
 *                                            mlib_s32 y_stride,
 *                                            mlib_s32 uv_stride);
 * ARGUMENT
 *      rgb        Pointer to the output RGB image
 *      y          Pointer to input Y component
 *      u          Pointer to input U component
 *      v          Pointer to input V component
 *      width      Width of the image. Note that width must be a multiple of 2
 *      height     Height of the image. Note that height must be a multiple of 2
 *      rgb_stride Stride in bytes between adjacent rows in output image
 *      y_stride   Stride in bytes between adjacent rows in Y input image
 *      uv_stride  Stride in bytes between adjacent rows in U and V input image
 *
 * DESCRIPTION
 *      mlib_ImageColorConvert2()
 *        |R|   |1.1644   0.0000   1.5966|   [Y   16.0000 ]
 *        |G| = |1.1644  -0.3920  -0.8132| * [U - 128.0000]
 *        |B|   |1.1644   2.0184   0.0000|   [V   128.0000]
 */

#include <vis_proto.h>
#include <mlib_video.h>
#include <mlib_algebra.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorYUV2RGB420 = __mlib_VideoColorYUV2RGB420

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorYUV2RGB420) mlib_VideoColorYUV2RGB420
	__attribute__((weak, alias("__mlib_VideoColorYUV2RGB420")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	STORE_PIXEL1(ind1, ind2, ind3)                          \
	vis_st_u8_i(blue1, dp1, ind3);                          \
	blue1 = vis_faligndata(blue1, blue1);                   \
	vis_st_u8_i(green1, dp1, ind2);                         \
	green1 = vis_faligndata(green1, green1);                \
	vis_st_u8_i(red1, dp1, ind1);                           \
	red1 = vis_faligndata(red1, red1)

/* *********************************************************** */

#define	STORE_PIXEL2(ind1, ind2, ind3)                          \
	vis_st_u8_i(blue2, dp2, ind3);                          \
	blue2 = vis_faligndata(blue2, blue2);                   \
	vis_st_u8_i(green2, dp2, ind2);                         \
	green2 = vis_faligndata(green2, green2);                \
	vis_st_u8_i(red2, dp2, ind1);                           \
	red2 = vis_faligndata(red2, red2)

/* *********************************************************** */

static mlib_status mlib_v_VideoColorYUV2RGB420_all_align(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 rgb_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride);

static mlib_status mlib_v_VideoColorYUV2RGB420_nonalign(
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
__mlib_VideoColorYUV2RGB420(
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

	if ((width | height) & 1)
		return (MLIB_FAILURE);

	if ((((mlib_addr)y & 7) == 0) && (((mlib_addr)u & 3) == 0) &&
		(((mlib_addr)v & 3) == 0) && (((mlib_addr)rgb & 7) == 0) &&
		((uv_stride & 3) == 0) && ((y_stride & 7) == 0) &&
		((rgb_stride & 7) == 0)) {

		return mlib_v_VideoColorYUV2RGB420_all_align(rgb, y, u, v,
			width, height, rgb_stride, y_stride, uv_stride);
	} else {
		return mlib_v_VideoColorYUV2RGB420_nonalign(rgb, y, u, v,
			width, height, rgb_stride, y_stride, uv_stride);
	}
}

/* *********************************************************** */

static mlib_status
mlib_v_VideoColorYUV2RGB420_all_align(
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
	mlib_u8 *sp11, *sp12, *sl11, *sl12;

/* pointers to dst address */
	mlib_u8 *dp1, *dl1, *dp2, *dl2;

/* all. pointer to y */
	mlib_d64 *spy1, *spy2;

/* all. pointers to u, v */
	mlib_f32 *dfu, *dfv;

/* u, v data */
	mlib_f32 fu, fv;

/* y data */
	mlib_d64 dy1, dy2;
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
	mlib_d64 z_11644_hi, z_11644_lo;
	mlib_d64 r_hi, r_lo, g_hi, g_lo, b_hi, b_lo;
	mlib_d64 temp_r_hi, temp_r_lo, temp_g_hi, temp_g_lo, temp_b_hi,
		temp_b_lo;
/* loop variables */
	mlib_s32 i, j;
	mlib_s32 y_stride2 = 2 * y_stride;
	mlib_s32 rgb_stride2 = 2 * rgb_stride;
	mlib_d64 red1, green1, blue1, *ddp1, dd01, dd11, dd21;
	mlib_d64 red2, green2, blue2, *ddp2, dd02, dd12, dd22;

/*
 * initialize GSR scale factor
 */
	vis_write_gsr((2 << 3) + 7);

	sp11 = sl11 = (mlib_u8 *)y;
	sp12 = sl12 = (mlib_u8 *)y + y_stride;
	sp2 = sl2 = (mlib_u8 *)u;
	sp3 = sl3 = (mlib_u8 *)v;

	dl1 = dp1 = (mlib_u8 *)rgb;
	dl2 = dp2 = (mlib_u8 *)(rgb + rgb_stride);
	ddp1 = (mlib_d64 *)dp1;
	ddp2 = (mlib_d64 *)dp2;

/*
 * row loop
 */
	for (j = 0; j < height / 2; j++) {
		spy1 = (mlib_d64 *)sp11;
		spy2 = (mlib_d64 *)sp12;
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

		dy1 = (*spy1++);
		dy2 = (*spy2++);

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

/* Y*1.1644 */
		y_11644_hi = vis_fmul8x16au(vis_read_hi(dy1), k12);
		b_hi = vis_fpsub16(u_20184_hi, k_276_9856);

/* Y*1.1644 */
		y_11644_lo = vis_fmul8x16au(vis_read_lo(dy1), k12);
		b_lo = vis_fpsub16(u_20184_lo, k_276_9856);

/* Z*1.1644 */
		z_11644_hi = vis_fmul8x16au(vis_read_hi(dy2), k12);
		r_hi = vis_fpsub16(v_15966_hi, k_222_9952);

/* Z*1.1644 */
		z_11644_lo = vis_fmul8x16au(vis_read_lo(dy2), k12);
		r_lo = vis_fpsub16(v_15966_lo, k_222_9952);

		temp_g_hi = vis_fpadd16(g_hi, y_11644_hi);
		temp_b_hi = vis_fpadd16(b_hi, y_11644_hi);

		green1 = vis_fpack16_to_hi(green1, temp_g_hi);
		temp_r_hi = vis_fpadd16(r_hi, y_11644_hi);

		blue1 = vis_fpack16_to_hi(blue1, temp_b_hi);
		temp_g_lo = vis_fpadd16(g_lo, y_11644_lo);
		fu = vis_ld_f32_nf(dfu); dfu++;

		red1 = vis_fpack16_to_hi(red1, temp_r_hi);
		temp_b_lo = vis_fpadd16(b_lo, y_11644_lo);
		fv = vis_ld_f32_nf(dfv); dfv++;

		green1 = vis_fpack16_to_lo(green1, temp_g_lo);
		temp_r_lo = vis_fpadd16(r_lo, y_11644_lo);

		blue1 = vis_fpack16_to_lo(blue1, temp_b_lo);
		du = vis_fpmerge(fu, fu);

		red1 = vis_fpack16_to_lo(red1, temp_r_lo);
		dv = vis_fpmerge(fv, fv);

/* U*(-0.3920); */
		u_3920_hi = vis_fmul8x16au(vis_read_hi(du), k34);
		temp_g_hi = vis_fpadd16(g_hi, z_11644_hi);

/* V*(-0.8132); */
		v_8132_hi = vis_fmul8x16al(vis_read_hi(dv), k34);
		temp_b_hi = vis_fpadd16(b_hi, z_11644_hi);

		green2 = vis_fpack16_to_hi(green2, temp_g_hi);
		temp_r_hi = vis_fpadd16(r_hi, z_11644_hi);

		blue2 = vis_fpack16_to_hi(blue2, temp_b_hi);
		temp_g_lo = vis_fpadd16(g_lo, z_11644_lo);

		red2 = vis_fpack16_to_hi(red2, temp_r_hi);
		temp_b_lo = vis_fpadd16(b_lo, z_11644_lo);

		green2 = vis_fpack16_to_lo(green2, temp_g_lo);
		temp_r_lo = vis_fpadd16(r_lo, z_11644_lo);

		blue2 = vis_fpack16_to_lo(blue2, temp_b_lo);
		red2 = vis_fpack16_to_lo(red2, temp_r_lo);

		u_3920_lo = vis_fmul8x16au(vis_read_lo(du), k34);
		v_8132_lo = vis_fmul8x16al(vis_read_lo(dv), k34);

		dy1 = vis_ld_d64_nf(spy1); spy1++;
		dy2 = vis_ld_d64_nf(spy2); spy2++;

/*
 * 16-pixel column loop
 */
#pragma pipeloop(0)
		for (i = 0; i <= width - 8; i += 8) {

			vis_write_bmask(0x0801902A, 0);
			dd01 = vis_bshuffle(red1, green1);
			dd02 = vis_bshuffle(red2, green2);
			vis_write_bmask(0x03B04C05, 0);
			dd11 = vis_bshuffle(red1, green1);
			dd12 = vis_bshuffle(red2, green2);
			vis_write_bmask(0xD06E07F0, 0);
			dd21 = vis_bshuffle(red1, green1);
			dd22 = vis_bshuffle(red2, green2);
			vis_write_bmask(0x01834967, 0);
			ddp1[0] = vis_bshuffle(dd01, blue1);
			ddp2[0] = vis_bshuffle(dd02, blue2);
			vis_write_bmask(0xA12B45C7, 0);
			ddp1[1] = vis_bshuffle(dd11, blue1);
			ddp2[1] = vis_bshuffle(dd12, blue2);
			vis_write_bmask(0x0D23E56F, 0);
			ddp1[2] = vis_bshuffle(dd21, blue1);
			ddp2[2] = vis_bshuffle(dd22, blue2);

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

/* Y*1.1644 */
			y_11644_hi = vis_fmul8x16au(vis_read_hi(dy1), k12);
			b_hi = vis_fpsub16(u_20184_hi, k_276_9856);

/* Y*1.1644 */
			y_11644_lo = vis_fmul8x16au(vis_read_lo(dy1), k12);
			b_lo = vis_fpsub16(u_20184_lo, k_276_9856);

/* Z*1.1644 */
			z_11644_hi = vis_fmul8x16au(vis_read_hi(dy2), k12);
			r_hi = vis_fpsub16(v_15966_hi, k_222_9952);

/* Z*1.1644 */
			z_11644_lo = vis_fmul8x16au(vis_read_lo(dy2), k12);
			r_lo = vis_fpsub16(v_15966_lo, k_222_9952);

			temp_g_hi = vis_fpadd16(g_hi, y_11644_hi);
			temp_b_hi = vis_fpadd16(b_hi, y_11644_hi);

			green1 = vis_fpack16_to_hi(green1, temp_g_hi);
			temp_r_hi = vis_fpadd16(r_hi, y_11644_hi);

			blue1 = vis_fpack16_to_hi(blue1, temp_b_hi);
			temp_g_lo = vis_fpadd16(g_lo, y_11644_lo);
			fu = vis_ld_f32_nf(dfu); dfu++;

			red1 = vis_fpack16_to_hi(red1, temp_r_hi);
			temp_b_lo = vis_fpadd16(b_lo, y_11644_lo);
			fv = vis_ld_f32_nf(dfv); dfv++;

			green1 = vis_fpack16_to_lo(green1, temp_g_lo);
			temp_r_lo = vis_fpadd16(r_lo, y_11644_lo);

			blue1 = vis_fpack16_to_lo(blue1, temp_b_lo);
			du = vis_fpmerge(fu, fu);

			red1 = vis_fpack16_to_lo(red1, temp_r_lo);
			dv = vis_fpmerge(fv, fv);

/* U*(-0.3920); */
			u_3920_hi = vis_fmul8x16au(vis_read_hi(du), k34);
			temp_g_hi = vis_fpadd16(g_hi, z_11644_hi);

/* V*(-0.8132); */
			v_8132_hi = vis_fmul8x16al(vis_read_hi(dv), k34);
			temp_b_hi = vis_fpadd16(b_hi, z_11644_hi);

			green2 = vis_fpack16_to_hi(green2, temp_g_hi);
			temp_r_hi = vis_fpadd16(r_hi, z_11644_hi);

			blue2 = vis_fpack16_to_hi(blue2, temp_b_hi);
			temp_g_lo = vis_fpadd16(g_lo, z_11644_lo);

			red2 = vis_fpack16_to_hi(red2, temp_r_hi);
			temp_b_lo = vis_fpadd16(b_lo, z_11644_lo);

			green2 = vis_fpack16_to_lo(green2, temp_g_lo);
			temp_r_lo = vis_fpadd16(r_lo, z_11644_lo);

			blue2 = vis_fpack16_to_lo(blue2, temp_b_lo);
			red2 = vis_fpack16_to_lo(red2, temp_r_lo);

			u_3920_lo = vis_fmul8x16au(vis_read_lo(du), k34);
			v_8132_lo = vis_fmul8x16al(vis_read_lo(dv), k34);

			dy1 = vis_ld_d64_nf(spy1); spy1++;
			dy2 = vis_ld_d64_nf(spy2); spy2++;

			ddp1 += 3;
			ddp2 += 3;
		}

		dp1 = (mlib_u8 *)ddp1;
		dp2 = (mlib_u8 *)ddp2;

		vis_alignaddr((void *)(width - i), 0);
		blue1 = vis_faligndata(blue1, blue1);
		green1 = vis_faligndata(green1, green1);
		red1 = vis_faligndata(red1, red1);
		dp1 += ((width - i - 1) * 3);

		blue2 = vis_faligndata(blue2, blue2);
		green2 = vis_faligndata(green2, green2);
		red2 = vis_faligndata(red2, red2);
		dp2 += ((width - i - 1) * 3);

		vis_alignaddr((void *)7, 0);
		for (; i < width; i++) {
			STORE_PIXEL1(0, 1, 2);
			STORE_PIXEL2(0, 1, 2);
			dp1 -= 3;
			dp2 -= 3;
		}

		sp11 = sl11 = sl11 + y_stride2;
		sp12 = sl12 = sl12 + y_stride2;
		sp2 = sl2 = sl2 + uv_stride;
		sp3 = sl3 = sl3 + uv_stride;

		dl1 = dp1 = dl1 + rgb_stride2;
		dl2 = dp2 = dl2 + rgb_stride2;
		ddp1 = (mlib_d64 *)dp1;
		ddp2 = (mlib_d64 *)dp2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

static mlib_status
mlib_v_VideoColorYUV2RGB420_nonalign(
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
	mlib_u8 *sp11, *sp12, *sl11, *sl12;

/* pointers to dst address */
	mlib_u8 *dp1, *dl1;

/* pointers to dst address */
	mlib_u8 *dp2, *dl2;

/* all. pointer to y */
	mlib_d64 *spy1, *spy2;

/* all. pointers to u, v */
	mlib_f32 *dfu, *dfv;

/* y data */
	mlib_d64 dy0, dy1, dy2, dy3, dy4, dy5;

/* u, v data */
	mlib_f32 fu0, fu1, fv0, fv1;
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
	mlib_d64 z_11644_hi, z_11644_lo;
	mlib_d64 r_hi, r_lo, g_hi, g_lo, b_hi, b_lo;
	mlib_d64 temp_r_hi, temp_r_lo, temp_g_hi, temp_g_lo, temp_b_hi,
		temp_b_lo;
/* loop variables */
	mlib_s32 i, j;
	mlib_s32 y_stride2 = 2 * y_stride;
	mlib_s32 rgb_stride2 = 2 * rgb_stride;
	mlib_s32 off2, off3;
	mlib_d64 red1, green1, blue1, *ddp1, dd01, dd11, dd21;
	mlib_d64 red2, green2, blue2, *ddp2, dd02, dd12, dd22;
	mlib_d64 *buf1, BUFF1[16 * 1024];
	mlib_d64 *buf2, BUFF2[16 * 1024];
	mlib_u8 *tmp1, *tmp2;

	if (width * 3 > 16 * 1024) {
		tmp1 = __mlib_malloc(width * 3 * sizeof (mlib_u8) + 7);
		tmp2 = __mlib_malloc(width * 3 * sizeof (mlib_u8) + 7);
		buf1 = (mlib_d64 *)((mlib_addr)(tmp1 + 7) & ~7);
		buf2 = (mlib_d64 *)((mlib_addr)(tmp2 + 7) & ~7);
	} else {
		buf1 = (mlib_d64 *)BUFF1;
		buf2 = (mlib_d64 *)BUFF2;
	}

/*
 * initialize GSR scale factor
 */
	vis_write_gsr(2 << 3);

	sp11 = sl11 = (mlib_u8 *)y;
	sp12 = sl12 = (mlib_u8 *)y + y_stride;
	sp2 = sl2 = (mlib_u8 *)u;
	sp3 = sl3 = (mlib_u8 *)v;

	dp1 = (mlib_u8 *)buf1;
	dp2 = (mlib_u8 *)buf2;
	dl1 = (mlib_u8 *)rgb;
	dl2 = (mlib_u8 *)(rgb + rgb_stride);
	ddp1 = (mlib_d64 *)dp1;
	ddp2 = (mlib_d64 *)dp2;

/*
 * row loop
 */
	for (j = 0; j < height / 2; j++) {
		spy1 = (mlib_d64 *)vis_alignaddr(sp11, 0);
		spy2 = (mlib_d64 *)vis_alignaddr(sp12, 0);

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

		dy0 = (*spy1++);
		dy4 = (*spy2++);

		dy3 = vis_ld_d64_nf(spy1); spy1++;
		vis_alignaddr(sp11, 0);
		dy1 = vis_faligndata(dy0, dy3);
		dy0 = dy3;

		dy5 = vis_ld_d64_nf(spy2); spy2++;
		vis_alignaddr(sp12, 0);
		dy2 = vis_faligndata(dy4, dy5);
		dy4 = dy5;

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

/* Y*1.1644 */
		y_11644_hi = vis_fmul8x16au(vis_read_hi(dy1), k12);
		b_hi = vis_fpsub16(u_20184_hi, k_276_9856);

/* Y*1.1644 */
		y_11644_lo = vis_fmul8x16au(vis_read_lo(dy1), k12);
		b_lo = vis_fpsub16(u_20184_lo, k_276_9856);

/* Z*1.1644 */
		z_11644_hi = vis_fmul8x16au(vis_read_hi(dy2), k12);
		r_hi = vis_fpsub16(v_15966_hi, k_222_9952);

/* Z*1.1644 */
		z_11644_lo = vis_fmul8x16au(vis_read_lo(dy2), k12);
		r_lo = vis_fpsub16(v_15966_lo, k_222_9952);

		temp_g_hi = vis_fpadd16(g_hi, y_11644_hi);
		temp_b_hi = vis_fpadd16(b_hi, y_11644_hi);

		green1 = vis_fpack16_to_hi(green1, temp_g_hi);
		temp_r_hi = vis_fpadd16(r_hi, y_11644_hi);

		blue1 = vis_fpack16_to_hi(blue1, temp_b_hi);
		temp_g_lo = vis_fpadd16(g_lo, y_11644_lo);
		vis_alignaddr((void *)off2, 0);
		fu1 = vis_ld_f32_nf(dfu); dfu++;
		du1 = vis_fpmerge(fu1, fu1);
		du = vis_faligndata(du0, du1);
		du0 = du1;

		red1 = vis_fpack16_to_hi(red1, temp_r_hi);
		temp_b_lo = vis_fpadd16(b_lo, y_11644_lo);
		vis_alignaddr((void *)off3, 0);
		fv1 = vis_ld_f32_nf(dfv); dfv++;
		dv1 = vis_fpmerge(fv1, fv1);
		dv = vis_faligndata(dv0, dv1);
		dv0 = dv1;

		green1 = vis_fpack16_to_lo(green1, temp_g_lo);
		temp_r_lo = vis_fpadd16(r_lo, y_11644_lo);

		blue1 = vis_fpack16_to_lo(blue1, temp_b_lo);
		red1 = vis_fpack16_to_lo(red1, temp_r_lo);

/* U*(-0.3920); */
		u_3920_hi = vis_fmul8x16au(vis_read_hi(du), k34);
		temp_g_hi = vis_fpadd16(g_hi, z_11644_hi);

/* V*(-0.8132); */
		v_8132_hi = vis_fmul8x16al(vis_read_hi(dv), k34);
		temp_b_hi = vis_fpadd16(b_hi, z_11644_hi);

		green2 = vis_fpack16_to_hi(green2, temp_g_hi);
		temp_r_hi = vis_fpadd16(r_hi, z_11644_hi);

		blue2 = vis_fpack16_to_hi(blue2, temp_b_hi);
		temp_g_lo = vis_fpadd16(g_lo, z_11644_lo);

		red2 = vis_fpack16_to_hi(red2, temp_r_hi);
		temp_b_lo = vis_fpadd16(b_lo, z_11644_lo);

		green2 = vis_fpack16_to_lo(green2, temp_g_lo);
		temp_r_lo = vis_fpadd16(r_lo, z_11644_lo);

		blue2 = vis_fpack16_to_lo(blue2, temp_b_lo);
		red2 = vis_fpack16_to_lo(red2, temp_r_lo);

		u_3920_lo = vis_fmul8x16au(vis_read_lo(du), k34);
		v_8132_lo = vis_fmul8x16al(vis_read_lo(dv), k34);

/*
 * 16-pixel column loop
 */
#pragma pipeloop(0)
		for (i = 0; i <= width - 8; i += 8) {

			vis_write_bmask(0x0801902A, 0);
			dd01 = vis_bshuffle(red1, green1);
			dd02 = vis_bshuffle(red2, green2);
			vis_write_bmask(0x03B04C05, 0);
			dd11 = vis_bshuffle(red1, green1);
			dd12 = vis_bshuffle(red2, green2);
			vis_write_bmask(0xD06E07F0, 0);
			dd21 = vis_bshuffle(red1, green1);
			dd22 = vis_bshuffle(red2, green2);
			vis_write_bmask(0x01834967, 0);
			ddp1[0] = vis_bshuffle(dd01, blue1);
			ddp2[0] = vis_bshuffle(dd02, blue2);
			vis_write_bmask(0xA12B45C7, 0);
			ddp1[1] = vis_bshuffle(dd11, blue1);
			ddp2[1] = vis_bshuffle(dd12, blue2);
			vis_write_bmask(0x0D23E56F, 0);
			ddp1[2] = vis_bshuffle(dd21, blue1);
			ddp2[2] = vis_bshuffle(dd22, blue2);

			dy3 = vis_ld_d64_nf(spy1); spy1++;
			vis_alignaddr(sp11, 0);
			dy1 = vis_faligndata(dy0, dy3);
			dy0 = dy3;

			dy5 = vis_ld_d64_nf(spy2); spy2++;
			vis_alignaddr(sp12, 0);
			dy2 = vis_faligndata(dy4, dy5);
			dy4 = dy5;

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

/* Y*1.1644 */
			y_11644_hi = vis_fmul8x16au(vis_read_hi(dy1), k12);
			b_hi = vis_fpsub16(u_20184_hi, k_276_9856);

/* Y*1.1644 */
			y_11644_lo = vis_fmul8x16au(vis_read_lo(dy1), k12);
			b_lo = vis_fpsub16(u_20184_lo, k_276_9856);

/* Z*1.1644 */
			z_11644_hi = vis_fmul8x16au(vis_read_hi(dy2), k12);
			r_hi = vis_fpsub16(v_15966_hi, k_222_9952);

/* Z*1.1644 */
			z_11644_lo = vis_fmul8x16au(vis_read_lo(dy2), k12);
			r_lo = vis_fpsub16(v_15966_lo, k_222_9952);

			temp_g_hi = vis_fpadd16(g_hi, y_11644_hi);
			temp_b_hi = vis_fpadd16(b_hi, y_11644_hi);

			green1 = vis_fpack16_to_hi(green1, temp_g_hi);
			temp_r_hi = vis_fpadd16(r_hi, y_11644_hi);

			blue1 = vis_fpack16_to_hi(blue1, temp_b_hi);
			temp_g_lo = vis_fpadd16(g_lo, y_11644_lo);
			vis_alignaddr((void *)off2, 0);
			fu1 = vis_ld_f32_nf(dfu); dfu++;
			du1 = vis_fpmerge(fu1, fu1);
			du = vis_faligndata(du0, du1);
			du0 = du1;

			red1 = vis_fpack16_to_hi(red1, temp_r_hi);
			temp_b_lo = vis_fpadd16(b_lo, y_11644_lo);
			vis_alignaddr((void *)off3, 0);
			fv1 = vis_ld_f32_nf(dfv); dfv++;
			dv1 = vis_fpmerge(fv1, fv1);
			dv = vis_faligndata(dv0, dv1);
			dv0 = dv1;

			green1 = vis_fpack16_to_lo(green1, temp_g_lo);
			temp_r_lo = vis_fpadd16(r_lo, y_11644_lo);

			blue1 = vis_fpack16_to_lo(blue1, temp_b_lo);
			red1 = vis_fpack16_to_lo(red1, temp_r_lo);

/* U*(-0.3920); */
			u_3920_hi = vis_fmul8x16au(vis_read_hi(du), k34);
			temp_g_hi = vis_fpadd16(g_hi, z_11644_hi);

/* V*(-0.8132); */
			v_8132_hi = vis_fmul8x16al(vis_read_hi(dv), k34);
			temp_b_hi = vis_fpadd16(b_hi, z_11644_hi);

			green2 = vis_fpack16_to_hi(green2, temp_g_hi);
			temp_r_hi = vis_fpadd16(r_hi, z_11644_hi);

			blue2 = vis_fpack16_to_hi(blue2, temp_b_hi);
			temp_g_lo = vis_fpadd16(g_lo, z_11644_lo);

			red2 = vis_fpack16_to_hi(red2, temp_r_hi);
			temp_b_lo = vis_fpadd16(b_lo, z_11644_lo);

			green2 = vis_fpack16_to_lo(green2, temp_g_lo);
			temp_r_lo = vis_fpadd16(r_lo, z_11644_lo);

			blue2 = vis_fpack16_to_lo(blue2, temp_b_lo);
			red2 = vis_fpack16_to_lo(red2, temp_r_lo);

			u_3920_lo = vis_fmul8x16au(vis_read_lo(du), k34);
			v_8132_lo = vis_fmul8x16al(vis_read_lo(dv), k34);

			ddp1 += 3;
			ddp2 += 3;
		}

		dp1 = (mlib_u8 *)ddp1;
		dp2 = (mlib_u8 *)ddp2;

		vis_alignaddr((void *)(width - i), 0);
		blue1 = vis_faligndata(blue1, blue1);
		green1 = vis_faligndata(green1, green1);
		red1 = vis_faligndata(red1, red1);
		dp1 += ((width - i - 1) * 3);

		blue2 = vis_faligndata(blue2, blue2);
		green2 = vis_faligndata(green2, green2);
		red2 = vis_faligndata(red2, red2);
		dp2 += ((width - i - 1) * 3);

		vis_alignaddr((void *)7, 0);
		for (; i < width; i++) {
			STORE_PIXEL1(0, 1, 2);
			STORE_PIXEL2(0, 1, 2);
			dp1 -= 3;
			dp2 -= 3;
		}

		sp11 = sl11 = sl11 + y_stride2;
		sp12 = sl12 = sl12 + y_stride2;
		sp2 = sl2 = sl2 + uv_stride;
		sp3 = sl3 = sl3 + uv_stride;
		__mlib_VectorCopy_U8(dl1, (mlib_u8 *)buf1, width * 3);
		__mlib_VectorCopy_U8(dl2, (mlib_u8 *)buf2, width * 3);

		dl1 = dp1 = dl1 + rgb_stride2;
		dl2 = dp2 = dl2 + rgb_stride2;
		dp1 = (mlib_u8 *)buf1;
		dp2 = (mlib_u8 *)buf2;
		ddp1 = (mlib_d64 *)dp1;
		ddp2 = (mlib_d64 *)dp2;
	}

	if (width * 3 > 16 * 1024) {
		__mlib_free(tmp1);
		__mlib_free(tmp2);
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
