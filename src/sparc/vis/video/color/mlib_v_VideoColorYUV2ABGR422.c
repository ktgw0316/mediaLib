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

#pragma ident	"@(#)mlib_v_VideoColorYUV2ABGR422.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV2ABGR422 - color conversion from YUV422 to ABGR
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorYUV2ABGR422(mlib_u8       *abgr,
 *                                             const mlib_u8 *y,
 *                                             const mlib_u8 *u,
 *                                             const mlib_u8 *v,
 *                                             mlib_s32      width,
 *                                             mlib_s32      height,
 *                                             mlib_s32      abgr_stride,
 *                                             mlib_s32      y_stride,
 *                                             mlib_s32      uv_stride);
 * ARGUMENT
 *      abgr        Pointer to the output ABGR image
 *      y           Pointer to input Y component
 *      u           Pointer to input U component
 *      v           Pointer to input V component
 *      width       Width of the image. Note that width must be a multiple of 2
 *      height      Height of the image
 *      abgr_stride Stride in bytes between adjacent rows in output image
 *      y_stride    Stride in bytes between adjacent rows in Y input image
 *      uv_stride   Stride in bytes between adjacent rows in U and V input
 *                  image
 *
 * DESCRIPTION
 *      mlib_ImageColorConvert2()
 *        |R|   |1.1644   0.0000   1.5966|   [|Y|   |16.0000| ]
 *        |G| = |1.1644  -0.3920  -0.8132| * [|U| - |128.0000|] =
 *        |B|   |1.1644   2.0184   0.0000|   [|V|   |128.0000|]
 *
 *              |1.1644   0.0000   1.5966|   |Y|   (-222.9952)
 *            = |1.1644  -0.3920  -0.8132| * |U| + (135.6352)
 *              |1.1644   2.0184   0.0000|   |V|   (-276.9856)
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorYUV2ABGR422 = __mlib_VideoColorYUV2ABGR422

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorYUV2ABGR422) mlib_VideoColorYUV2ABGR422
	__attribute__((weak, alias("__mlib_VideoColorYUV2ABGR422")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static mlib_status mlib_v_VideoColorYUV2ABGR422_all_align(
	mlib_u8 *abgr,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 abgr_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride);

static mlib_status mlib_v_VideoColorYUV2ABGR422_nonalign(
	mlib_u8 *abgr,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 abgr_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride);

/* *********************************************************** */

mlib_status
__mlib_VideoColorYUV2ABGR422(
	mlib_u8 *abgr,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 abgr_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride)
{
	if (abgr == NULL || y == NULL || u == NULL || v == NULL)
		return (MLIB_NULLPOINTER);

	if (width & 1)
		return (MLIB_FAILURE);

	if (width <= 0 || height <= 0)
		return (MLIB_FAILURE);

	if (y_stride == width && uv_stride == width / 2 &&
		abgr_stride == 4 * width) {

		width *= height;
		height = 1;
	}

	if ((((mlib_addr)abgr & 7) == 0) && (((mlib_addr)u & 3) == 0) &&
		(((mlib_addr)v & 3) == 0) && ((((uv_stride & 3) == 0) &&
		((abgr_stride & 7) == 0)) || (height == 1))) {

		return mlib_v_VideoColorYUV2ABGR422_all_align(abgr, y, u, v,
			width, height, abgr_stride, y_stride, uv_stride);
	} else {
		return mlib_v_VideoColorYUV2ABGR422_nonalign(abgr, y, u, v,
			width, height, abgr_stride, y_stride, uv_stride);
	}
}

/* *********************************************************** */

static mlib_status
mlib_v_VideoColorYUV2ABGR422_all_align(
	mlib_u8 *abgr,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 abgr_stride,
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

/* all. pointer to dst */
	mlib_d64 *dpp;

/* u, v data */
	mlib_f32 fu, fv;

/* y data */
	mlib_d64 dy0, dy1, dy3;
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
	mlib_f32 red_hi, red_lo, green_hi, green_lo, blue_hi, blue_lo;
	mlib_d64 blue_red_hi, x_green_hi, blue_red_lo, x_green_lo;
	mlib_d64 dd, dd1, dd2;
	mlib_f32 *dfu, *dfv;

/* loop variable */
	mlib_s32 i, j;

/* alpha_ch. is not written */
	mlib_s32 emask = 0x77;

/*
 * initialize GSR scale factor
 */
	vis_write_gsr(2 << 3);

	sp1 = sl1 = (mlib_u8 *)y;
	sp2 = sl2 = (mlib_u8 *)u;
	sp3 = sl3 = (mlib_u8 *)v;

	dl = dp = (mlib_u8 *)abgr;

/*
 * row loop
 */
	for (j = 0; j < height; j++) {
		spy = (mlib_d64 *)vis_alignaddr(sp1, 0);
		dpp = (mlib_d64 *)dp;
		dfu = (mlib_f32 *)sp2;
		dfv = (mlib_f32 *)sp3;

		fu = vis_ld_f32_nf(dfu); dfu++;
		fv = vis_ld_f32_nf(dfv); dfv++;

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

		dy0 = vis_ld_d64_nf(spy); spy++;
		dy3 = vis_ld_d64_nf(spy); spy++;
		dy1 = vis_faligndata(dy0, dy3);
		dy0 = dy3;

/*
 * 16-pixel column loop
 */
#pragma pipeloop(0)
		for (i = 0; i <= width - 8; i += 8) {

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

			r_hi = vis_fpsub16(v_15966_hi, k_222_9952);
			r_lo = vis_fpsub16(v_15966_lo, k_222_9952);

			temp_g_hi = vis_fpadd16(g_hi, y_11644_hi);
			temp_b_hi = vis_fpadd16(b_hi, y_11644_hi);

			fu = vis_ld_f32_nf(dfu); dfu++;
			green_hi = vis_fpack16(temp_g_hi);
			temp_r_hi = vis_fpadd16(r_hi, y_11644_hi);

			fv = vis_ld_f32_nf(dfv); dfv++;
			blue_hi = vis_fpack16(temp_b_hi);
			temp_g_lo = vis_fpadd16(g_lo, y_11644_lo);

			red_hi = vis_fpack16(temp_r_hi);
			temp_b_lo = vis_fpadd16(b_lo, y_11644_lo);

			green_lo = vis_fpack16(temp_g_lo);
			temp_r_lo = vis_fpadd16(r_lo, y_11644_lo);

			blue_lo = vis_fpack16(temp_b_lo);
			du = vis_fpmerge(fu, fu);

			x_green_hi = vis_fmul8x16au(green_hi, k5);
			dv = vis_fpmerge(fv, fv);

			red_lo = vis_fpack16(temp_r_lo);
			blue_red_hi = vis_fpmerge(blue_hi, red_hi);

			x_green_lo = vis_fmul8x16au(green_lo, k5);
			blue_red_lo = vis_fpmerge(blue_lo, red_lo);

/* U*(-0.3920); */
			u_3920_hi = vis_fmul8x16au(vis_read_hi(du), k34);
			dd = vis_fpmerge(vis_read_hi(x_green_hi),
				vis_read_hi(blue_red_hi));
			vis_pst_8(dd, dpp++, emask);

/* V*(-0.8132); */
			v_8132_hi = vis_fmul8x16al(vis_read_hi(dv), k34);
			dd = vis_fpmerge(vis_read_lo(x_green_hi),
				vis_read_lo(blue_red_hi));
			vis_pst_8(dd, dpp++, emask);

			u_3920_lo = vis_fmul8x16au(vis_read_lo(du), k34);
			dd = vis_fpmerge(vis_read_hi(x_green_lo),
				vis_read_hi(blue_red_lo));
			vis_pst_8(dd, dpp++, emask);

			v_8132_lo = vis_fmul8x16al(vis_read_lo(dv), k34);
			dd = vis_fpmerge(vis_read_lo(x_green_lo),
				vis_read_lo(blue_red_lo));

			vis_pst_8(dd, dpp++, emask);
			dy3 = vis_ld_d64_nf(spy); spy++;
			dy1 = vis_faligndata(dy0, dy3);
			dy0 = dy3;
		}

		if (i < width) {

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

			r_hi = vis_fpsub16(v_15966_hi, k_222_9952);
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

			x_green_lo = vis_fmul8x16au(green_lo, k5);
			blue_red_lo = vis_fpmerge(blue_lo, red_lo);

			dd = vis_fpmerge(vis_read_hi(x_green_hi),
				vis_read_hi(blue_red_hi));
			vis_pst_8(dd, dpp++, emask);

			dd1 = vis_fpmerge(vis_read_lo(x_green_hi),
				vis_read_lo(blue_red_hi));

			dd2 = vis_fpmerge(vis_read_hi(x_green_lo),
				vis_read_hi(blue_red_lo));

			i += 2;

			if (i < width) {
				vis_pst_8(dd1, dpp++, emask);
				i += 2;

				if (i < width) {
					vis_pst_8(dd2, dpp, emask);
				}
			}
		}

		sp1 = sl1 = sl1 + y_stride;
		sp2 = sl2 = sl2 + uv_stride;
		sp3 = sl3 = sl3 + uv_stride;

		dl = dp = dl + abgr_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

static mlib_status
mlib_v_VideoColorYUV2ABGR422_nonalign(
	mlib_u8 *abgr,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 abgr_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride)
{
/* pointers to src address */
	mlib_u8 *sp2, *sp3, *sl2, *sl3;

/* pointers to src address */
	mlib_u8 *sp1, *sl1;

/* pointers to dst address */
	mlib_u8 *dp, *dl, *dend;

/* all. pointer to y */
	mlib_d64 *spy;

/* all. pointer to dst */
	mlib_d64 *dpp;

/* u, v data */
	mlib_f32 fu0, fu1, fv0, fv1;

/* y data */
	mlib_d64 dy0, dy1, dy3;
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
	mlib_f32 red_hi, red_lo, green_hi, green_lo, blue_hi, blue_lo;
	mlib_d64 blue_red_hi, x_green_hi, blue_red_lo, x_green_lo;
	mlib_d64 dd, dd0, dd1;

/* loop variable */
	mlib_s32 i, j;

/* alpha_ch. is not written */
	mlib_s32 emask = 0x7777;
	mlib_s32 emask1;
	mlib_s32 off;
	mlib_f32 *dfu, *dfv;
	mlib_d64 du0, du1, dv0, dv1;
	mlib_s32 off2, off3;
	mlib_s32 inc;

/*
 * initialize GSR scale factor
 */
	vis_write_gsr(2 << 3);

	sp1 = sl1 = (mlib_u8 *)y;
	sp2 = sl2 = (mlib_u8 *)u;
	sp3 = sl3 = (mlib_u8 *)v;

	dl = dp = (mlib_u8 *)abgr;

/*
 * row loop
 */
	for (j = 0; j < height; j++) {
		spy = (mlib_d64 *)vis_alignaddr(sp1, 0);
		dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
		dfu = (mlib_f32 *)((mlib_addr)sp2 & ~3);
		off2 = (sp2 - (mlib_u8 *)dfu) * 2;
		dfv = (mlib_f32 *)((mlib_addr)sp3 & ~3);
		off3 = (sp3 - (mlib_u8 *)dfv) * 2;

		dend = dp + width * 4 - 1;
		emask1 = vis_edge8(dp, dend);
		i = dp - (mlib_u8 *)dpp;
		emask >>= i;
		inc = (emask1 != 0xff);
		emask1 &= emask;
		off = 8 - i;

		vis_alignaddr((void *)off2, 0);
		fu0 = vis_ld_f32_nf(dfu); dfu++;
		fu1 = vis_ld_f32_nf(dfu); dfu++;
		du0 = vis_fpmerge(fu0, fu0);
		du1 = vis_fpmerge(fu1, fu1);
		du = vis_faligndata(du0, du1);
		du0 = du1;

		vis_alignaddr((void *)off3, 0);
		fv0 = vis_ld_f32_nf(dfv); dfv++;
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
		dy0 = vis_ld_d64_nf(spy); spy++;
		dy3 = vis_ld_d64_nf(spy); spy++;
		dy1 = vis_faligndata(dy0, dy3);
		dy0 = dy3;

/*
 * 16-pixel column loop
 */
#pragma pipeloop(0)
		for (i = 0; i <= width - 8; i += 8) {

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

			r_hi = vis_fpsub16(v_15966_hi, k_222_9952);
			r_lo = vis_fpsub16(v_15966_lo, k_222_9952);

			temp_g_hi = vis_fpadd16(g_hi, y_11644_hi);
			temp_b_hi = vis_fpadd16(b_hi, y_11644_hi);

			green_hi = vis_fpack16(temp_g_hi);
			temp_r_hi = vis_fpadd16(r_hi, y_11644_hi);

			blue_hi = vis_fpack16(temp_b_hi);
			temp_g_lo = vis_fpadd16(g_lo, y_11644_lo);

			red_hi = vis_fpack16(temp_r_hi);
			temp_b_lo = vis_fpadd16(b_lo, y_11644_lo);

			vis_alignaddr((void *)off2, 0);
			fu1 = vis_ld_f32_nf(dfu); dfu++;
			du1 = vis_fpmerge(fu1, fu1);
			du = vis_faligndata(du0, du1);
			du0 = du1;

			green_lo = vis_fpack16(temp_g_lo);
			temp_r_lo = vis_fpadd16(r_lo, y_11644_lo);

			blue_lo = vis_fpack16(temp_b_lo);
			x_green_hi = vis_fmul8x16au(green_hi, k5);

			red_lo = vis_fpack16(temp_r_lo);
			blue_red_hi = vis_fpmerge(blue_hi, red_hi);

			x_green_lo = vis_fmul8x16au(green_lo, k5);
			blue_red_lo = vis_fpmerge(blue_lo, red_lo);

			vis_alignaddr((void *)off3, 0);

			fv1 = vis_ld_f32_nf(dfv); dfv++;
			dv1 = vis_fpmerge(fv1, fv1);
			dv = vis_faligndata(dv0, dv1);
			dv0 = dv1;

			vis_alignaddr((void *)off, 0);
/* U*(-0.3920); */
			u_3920_hi = vis_fmul8x16au(vis_read_hi(du), k34);
			dd1 = vis_fpmerge(vis_read_hi(x_green_hi),
				vis_read_hi(blue_red_hi));
			dd = vis_faligndata(dd0, dd1);
			vis_pst_8(dd, dpp, emask1);
			dpp += inc;
			inc = 1;

/* V*(-0.8132); */
			v_8132_hi = vis_fmul8x16al(vis_read_hi(dv), k34);
			dd0 = vis_fpmerge(vis_read_lo(x_green_hi),
				vis_read_lo(blue_red_hi));
			dd = vis_faligndata(dd1, dd0);
			vis_pst_8(dd, dpp++, emask);

			u_3920_lo = vis_fmul8x16au(vis_read_lo(du), k34);
			dd1 = vis_fpmerge(vis_read_hi(x_green_lo),
				vis_read_hi(blue_red_lo));
			dd = vis_faligndata(dd0, dd1);
			vis_pst_8(dd, dpp++, emask);

			v_8132_lo = vis_fmul8x16al(vis_read_lo(dv), k34);
			dd0 = vis_fpmerge(vis_read_lo(x_green_lo),
				vis_read_lo(blue_red_lo));
			dd = vis_faligndata(dd1, dd0);
			vis_pst_8(dd, dpp++, emask);

			vis_alignaddr(sp1, 0);
			dy3 = vis_ld_d64_nf(spy); spy++;
			dy1 = vis_faligndata(dy0, dy3);
			dy0 = dy3;
			emask1 = emask;
		}

		if (i < width) {

			vis_alignaddr((void *)off, 0);
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

			r_hi = vis_fpsub16(v_15966_hi, k_222_9952);
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

			x_green_lo = vis_fmul8x16au(green_lo, k5);
			blue_red_lo = vis_fpmerge(blue_lo, red_lo);

			dd1 = vis_fpmerge(vis_read_hi(x_green_hi),
				vis_read_hi(blue_red_hi));
			dd = vis_faligndata(dd0, dd1);
			vis_pst_8(dd, dpp, emask1);
			dd0 = dd1;
			dpp += inc;

			i += 2;

			if (i < width) {

				dd1 = vis_fpmerge(vis_read_lo(x_green_hi),
					vis_read_lo(blue_red_hi));
				dd = vis_faligndata(dd0, dd1);
				vis_pst_8(dd, dpp++, emask);
				dd0 = dd1;
				i += 2;

				if (i < width) {
					dd1 = vis_fpmerge(vis_read_hi
						(x_green_lo),
						vis_read_hi(blue_red_lo));
					dd = vis_faligndata(dd0, dd1);
					vis_pst_8(dd, dpp++, emask);
					dd0 = dd1;
				}
			}
		}

		vis_alignaddr((void *)off, 0);
		emask1 = vis_edge8(dpp, dend);
		emask1 &= emask;
		dd = vis_faligndata(dd0, dd1);
		vis_pst_8(dd, dpp, emask1);

		sp1 = sl1 = sl1 + y_stride;
		sp2 = sl2 = sl2 + uv_stride;
		sp3 = sl3 = sl3 + uv_stride;

		dl = dp = dl + abgr_stride;
		emask = 0x7777;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
