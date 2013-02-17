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

#pragma ident	"@(#)mlib_v_VideoColorYUV2ARGB420.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV2ARGB420 - color conversion from YUV420 to ARGB
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_v_VideoColorYUV2ARGB420(mlib_u8       *argb,
 *                                               const mlib_u8 *y,
 *                                               const mlib_u8 *u,
 *                                               const mlib_u8 *v,
 *                                               mlib_s32      width,
 *                                               mlib_s32      height,
 *                                               mlib_s32      argb_stride,
 *                                               mlib_s32      y_stride,
 *                                               mlib_s32      uv_stride);
 *
 * ARGUMENT
 *      argb        Pointer to the output ARGB image
 *      y           Pointer to input Y component
 *      u           Pointer to input U component
 *      v           Pointer to input V component
 *      width       Width of the image. Note that width
 *                  must be a multiple of 2
 *      height      Height of the image. Note that height
 *                  must be a multiple of 2
 *      argb_stride Stride in bytes between adjacent rows in output image
 *      y_stride    Stride in bytes between adjacent rows in Y input image
 *      uv_stride   Stride in bytes between adjacent rows
 *                  in U and V input image
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

#include <vis_proto.h>
#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorYUV2ARGB420 = __mlib_VideoColorYUV2ARGB420

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorYUV2ARGB420) mlib_VideoColorYUV2ARGB420
	__attribute__((weak, alias("__mlib_VideoColorYUV2ARGB420")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static mlib_status mlib_v_VideoColorYUV2ARGB420_all_align(
	mlib_u8 *argb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 argb_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride);

static mlib_status mlib_v_VideoColorYUV2ARGB420_nonalign(
	mlib_u8 *argb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 argb_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride);

/* *********************************************************** */

mlib_status
__mlib_VideoColorYUV2ARGB420(
	mlib_u8 *argb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 argb_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride)
{
	if (argb == NULL || y == NULL || u == NULL || v == NULL)
		return (MLIB_NULLPOINTER);

	if (width <= 0 || height <= 0)
		return (MLIB_FAILURE);

	if ((width | height) & 1)
		return (MLIB_FAILURE);

	if ((((mlib_addr)argb & 7) == 0) && (((mlib_addr)u & 3) == 0) &&
		(((mlib_addr)v & 3) == 0) && ((uv_stride & 3) == 0) &&
		((argb_stride & 7) == 0) && ((y_stride & 7) == 0)) {

		return mlib_v_VideoColorYUV2ARGB420_all_align(argb, y, u, v,
			width, height, argb_stride, y_stride, uv_stride);
	} else {
		return mlib_v_VideoColorYUV2ARGB420_nonalign(argb, y, u, v,
			width, height, argb_stride, y_stride, uv_stride);
	}
}

/* *********************************************************** */

static mlib_status
mlib_v_VideoColorYUV2ARGB420_all_align(
	mlib_u8 *argb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 argb_stride,
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

/* all. pointer to dst */
	mlib_d64 *dpp1, *dpp2;

/* u, v data */
	mlib_f32 fu, fv;

/* y data */
	mlib_d64 dy0, dy1, dy2, dy3, dy4, dy5;
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
	mlib_f32 red_hi, red_lo, green_hi, green_lo, blue_hi, blue_lo;
	mlib_f32 red2_lo, green2_lo, blue2_lo;
	mlib_d64 blue_red_hi, x_green_hi, blue_red_lo, x_green_lo;
	mlib_d64 dd, dd1, dd2, dd3, dd4, dd5, dd6;
	mlib_f32 *dfu, *dfv;

/* loop variables */
	mlib_s32 i, j;

/* alpha_ch. is not written */
	mlib_s32 emask = 0x77;
	mlib_s32 y_stride2 = 2 * y_stride;
	mlib_s32 argb_stride2 = 2 * argb_stride;
	mlib_f32 fone = vis_to_float(0x100);

/*
 * initialize GSR scale factor
 */
	vis_write_gsr(2 << 3);

	sp11 = sl11 = (mlib_u8 *)y;
	sp12 = sl12 = (mlib_u8 *)y + y_stride;
	sp2 = sl2 = (mlib_u8 *)u;
	sp3 = sl3 = (mlib_u8 *)v;

	dl1 = dp1 = (mlib_u8 *)argb;
	dl2 = dp2 = (mlib_u8 *)(argb + argb_stride);

/*
 * row loop
 */
	for (j = 0; j < height / 2; j++) {
		spy1 = (mlib_d64 *)vis_alignaddr(sp11, 0);
		spy2 = (mlib_d64 *)vis_alignaddr(sp12, 0);
		dpp1 = (mlib_d64 *)dp1;
		dpp2 = (mlib_d64 *)dp2;
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

		dy0 = vis_ld_d64_nf(spy1); spy1++;
		dy4 = vis_ld_d64_nf(spy2); spy2++;

/*
 * 16-pixel column loop
 */
#pragma pipeloop(0)
		for (i = 0; i <= width - 8; i += 8) {

			dy3 = vis_ld_d64_nf(spy1); spy1++;
			dy1 = vis_faligndata(dy0, dy3);
			dy0 = dy3;

			dy5 = vis_ld_d64_nf(spy2); spy2++;
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
			blue_red_hi = vis_fpmerge(red_hi, blue_hi);

			temp_g_hi = vis_fpadd16(g_hi, z_11644_hi);
			temp_b_hi = vis_fpadd16(b_hi, z_11644_hi);

			green_hi = vis_fpack16(temp_g_hi);
			temp_r_hi = vis_fpadd16(r_hi, z_11644_hi);

			blue_hi = vis_fpack16(temp_b_hi);
			temp_g_lo = vis_fpadd16(g_lo, z_11644_lo);

			red_hi = vis_fpack16(temp_r_hi);
			temp_b_lo = vis_fpadd16(b_lo, z_11644_lo);

			fu = vis_ld_f32_nf(dfu); dfu++;
			green2_lo = vis_fpack16(temp_g_lo);
			temp_r_lo = vis_fpadd16(r_lo, z_11644_lo);

			fv = vis_ld_f32_nf(dfv); dfv++;
			blue2_lo = vis_fpack16(temp_b_lo);
			x_green_lo = vis_fmul8x16au(green_lo, k5);

			red2_lo = vis_fpack16(temp_r_lo);
			blue_red_lo = vis_fpmerge(red_lo, blue_lo);

			dd = vis_fpmerge(vis_read_hi(x_green_hi),
				vis_read_hi(blue_red_hi));
			vis_pst_8(dd, dpp1++, emask);

			dd = vis_fpmerge(vis_read_lo(x_green_hi),
				vis_read_lo(blue_red_hi));
			vis_pst_8(dd, dpp1++, emask);

			dd = vis_fpmerge(vis_read_hi(x_green_lo),
				vis_read_hi(blue_red_lo));
			vis_pst_8(dd, dpp1++, emask);

			dd = vis_fpmerge(vis_read_lo(x_green_lo),
				vis_read_lo(blue_red_lo));
			vis_pst_8(dd, dpp1++, emask);

			x_green_hi = vis_fmul8x16au(green_hi, k5);
			blue_red_hi = vis_fpmerge(red_hi, blue_hi);

			dd = vis_fpmerge(vis_read_hi(x_green_hi),
				vis_read_hi(blue_red_hi));
			vis_pst_8(dd, dpp2++, emask);

			dd = vis_fpmerge(vis_read_lo(x_green_hi),
				vis_read_lo(blue_red_hi));
			vis_pst_8(dd, dpp2++, emask);

			du = vis_fpmerge(fu, fu);
			dv = vis_fpmerge(fv, fv);

/* U*(-0.3920); */
			u_3920_hi = vis_fmul8x16au(vis_read_hi(du), k34);
			x_green_lo = vis_fmul8x16al(green2_lo, fone);

/* V*(-0.8132); */
			v_8132_hi = vis_fmul8x16al(vis_read_hi(dv), k34);
			blue_red_lo = vis_fpmerge(red2_lo, blue2_lo);

			u_3920_lo = vis_fmul8x16au(vis_read_lo(du), k34);
			dd = vis_fpmerge(vis_read_hi(x_green_lo),
				vis_read_hi(blue_red_lo));
			vis_pst_8(dd, dpp2++, emask);

			v_8132_lo = vis_fmul8x16al(vis_read_lo(dv), k34);
			dd = vis_fpmerge(vis_read_lo(x_green_lo),
				vis_read_lo(blue_red_lo));
			vis_pst_8(dd, dpp2++, emask);
		}

		if (i < width) {

			dy3 = vis_ld_d64_nf(spy1);
			dy1 = vis_faligndata(dy0, dy3);

			dy5 = vis_ld_d64_nf(spy2);
			dy2 = vis_faligndata(dy4, dy5);

			u_20184_hi = vis_fmul8x16al(vis_read_hi(du), k5);
			g_hi = vis_fpadd16(u_3920_hi, v_8132_hi);

			u_20184_lo = vis_fmul8x16al(vis_read_lo(du), k5);
			g_hi = vis_fpadd16(g_hi, k_135_6352);

			v_15966_hi = vis_fmul8x16al(vis_read_hi(dv), k12);
			g_lo = vis_fpadd16(u_3920_lo, v_8132_lo);

			v_15966_lo = vis_fmul8x16al(vis_read_lo(dv), k12);
			g_lo = vis_fpadd16(g_lo, k_135_6352);

			y_11644_hi = vis_fmul8x16au(vis_read_hi(dy1), k12);
			b_hi = vis_fpsub16(u_20184_hi, k_276_9856);

			y_11644_lo = vis_fmul8x16au(vis_read_lo(dy1), k12);
			b_lo = vis_fpsub16(u_20184_lo, k_276_9856);

			z_11644_hi = vis_fmul8x16au(vis_read_hi(dy2), k12);
			r_hi = vis_fpsub16(v_15966_hi, k_222_9952);

			z_11644_lo = vis_fmul8x16au(vis_read_lo(dy2), k12);
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
			blue_red_hi = vis_fpmerge(red_hi, blue_hi);

			temp_g_hi = vis_fpadd16(g_hi, z_11644_hi);
			temp_b_hi = vis_fpadd16(b_hi, z_11644_hi);

			green_hi = vis_fpack16(temp_g_hi);
			temp_r_hi = vis_fpadd16(r_hi, z_11644_hi);

			blue_hi = vis_fpack16(temp_b_hi);
			temp_g_lo = vis_fpadd16(g_lo, z_11644_lo);

			red_hi = vis_fpack16(temp_r_hi);
			temp_b_lo = vis_fpadd16(b_lo, z_11644_lo);

			green2_lo = vis_fpack16(temp_g_lo);
			temp_r_lo = vis_fpadd16(r_lo, z_11644_lo);

			blue2_lo = vis_fpack16(temp_b_lo);
			x_green_lo = vis_fmul8x16au(green_lo, k5);

			red2_lo = vis_fpack16(temp_r_lo);
			blue_red_lo = vis_fpmerge(red_lo, blue_lo);

			dd1 = vis_fpmerge(vis_read_hi(x_green_hi),
				vis_read_hi(blue_red_hi));
			dd2 = vis_fpmerge(vis_read_lo(x_green_hi),
				vis_read_lo(blue_red_hi));
			dd3 = vis_fpmerge(vis_read_hi(x_green_lo),
				vis_read_hi(blue_red_lo));

			x_green_hi = vis_fmul8x16au(green_hi, k5);
			blue_red_hi = vis_fpmerge(red_hi, blue_hi);

			dd4 = vis_fpmerge(vis_read_hi(x_green_hi),
				vis_read_hi(blue_red_hi));
			dd5 = vis_fpmerge(vis_read_lo(x_green_hi),
				vis_read_lo(blue_red_hi));
			x_green_lo = vis_fmul8x16al(green2_lo, fone);
			blue_red_lo = vis_fpmerge(red2_lo, blue2_lo);

			dd6 = vis_fpmerge(vis_read_hi(x_green_lo),
				vis_read_hi(blue_red_lo));
			vis_pst_8(dd1, dpp1++, emask);
			vis_pst_8(dd4, dpp2++, emask);
			i += 2;

			if (i < width) {
				vis_pst_8(dd2, dpp1++, emask);
				vis_pst_8(dd5, dpp2++, emask);
				i += 2;

				if (i < width) {
					vis_pst_8(dd3, dpp1, emask);
					vis_pst_8(dd6, dpp2, emask);
				}
			}
		}

		sp11 = sl11 = sl11 + y_stride2;
		sp12 = sl12 = sl12 + y_stride2;
		sp2 = sl2 = sl2 + uv_stride;
		sp3 = sl3 = sl3 + uv_stride;

		dl1 = dp1 = dl1 + argb_stride2;
		dl2 = dp2 = dl2 + argb_stride2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

static mlib_status
mlib_v_VideoColorYUV2ARGB420_nonalign(
	mlib_u8 *argb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 argb_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride)
{
/* pointers to src address */
	mlib_u8 *sp2, *sp3, *sl2, *sl3;

/* pointers to src address */
	mlib_u8 *sp11, *sp12, *sl11, *sl12;

/* pointers to dst address */
	mlib_u8 *dp1, *dl1, *dend1;

/* pointers to dst address */
	mlib_u8 *dp2, *dl2, *dend2;

/* all. pointer to y */
	mlib_d64 *spy1, *spy2;

/* all. pointer to dst */
	mlib_d64 *dpp1, *dpp2;

/* y data */
	mlib_d64 dy0, dy1, dy2, dy3, dy4, dy5;
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
	mlib_f32 red_hi, red_lo, green_hi, green_lo, blue_hi, blue_lo;
	mlib_f32 red2_lo, green2_lo, blue2_lo;
	mlib_d64 blue_red_hi, x_green_hi, blue_red_lo, x_green_lo;
	mlib_d64 dd, dd0, dd1, dd2, dd3;

/* loop variables */
	mlib_s32 i, ii, j;

/* alpha_ch. is not written */
	mlib_s32 emask = 0x7777, emask2 = 0x7777;
	mlib_s32 emask1, emask3;
	mlib_s32 y_stride2 = 2 * y_stride;
	mlib_s32 argb_stride2 = 2 * argb_stride;
	mlib_s32 off1, off2, off3, off4;
	mlib_f32 fone = vis_to_float(0x100);
	mlib_f32 *dfu, *dfv;
	mlib_d64 du0, du1, dv0, dv1;
	mlib_s32 inc1, inc2;

/* u, v data */
	mlib_f32 fu0, fu1, fv0, fv1;

/*
 * initialize GSR scale factor
 */
	vis_write_gsr(2 << 3);

	sp11 = sl11 = (mlib_u8 *)y;
	sp12 = sl12 = (mlib_u8 *)y + y_stride;
	sp2 = sl2 = (mlib_u8 *)u;
	sp3 = sl3 = (mlib_u8 *)v;

	dl1 = dp1 = (mlib_u8 *)argb;
	dl2 = dp2 = (mlib_u8 *)(argb + argb_stride);

/*
 * row loop
 */
	for (j = 0; j < height / 2; j++) {
		spy1 = (mlib_d64 *)vis_alignaddr(sp11, 0);
		spy2 = (mlib_d64 *)vis_alignaddr(sp12, 0);
		dfu = (mlib_f32 *)((mlib_addr)sp2 & ~3);
		off3 = (sp2 - (mlib_u8 *)dfu) * 2;
		dfv = (mlib_f32 *)((mlib_addr)sp3 & ~3);
		off4 = (sp3 - (mlib_u8 *)dfv) * 2;

		dend1 = dp1 + width * 4 - 1;
		emask1 = vis_edge8(dp1, dend1);

		dpp1 = (mlib_d64 *)vis_alignaddr(dp1, 0);
		i = dp1 - (mlib_u8 *)dpp1;
		emask >>= i;
		inc1 = (emask1 != 0xff);
		emask1 &= emask;
		off1 = 8 - i;

		dend2 = dp2 + width * 4 - 1;
		emask3 = vis_edge8(dp2, dend2);

		dpp2 = (mlib_d64 *)vis_alignaddr(dp2, 0);
		i = dp2 - (mlib_u8 *)dpp2;
		emask2 >>= i;
		inc2 = (emask3 != 0xff);
		emask3 &= emask2;
		off2 = 8 - i;

		vis_alignaddr((void *)off3, 0);
		fu0 = vis_ld_f32_nf(dfu); dfu++;
		fu1 = vis_ld_f32_nf(dfu); dfu++;
		du0 = vis_fpmerge(fu0, fu0);
		du1 = vis_fpmerge(fu1, fu1);
		du = vis_faligndata(du0, du1);
		du0 = du1;

		vis_alignaddr((void *)off4, 0);
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

		dy0 = vis_ld_d64_nf(spy1); spy1++;
		dy4 = vis_ld_d64_nf(spy2); spy2++;

/*
 * 16-pixel column loop
 */
#pragma pipeloop(0)
		for (i = 0; i <= width - 8; i += 8) {

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
			blue_red_hi = vis_fpmerge(red_hi, blue_hi);

			temp_g_hi = vis_fpadd16(g_hi, z_11644_hi);
			temp_b_hi = vis_fpadd16(b_hi, z_11644_hi);

			green_hi = vis_fpack16(temp_g_hi);
			temp_r_hi = vis_fpadd16(r_hi, z_11644_hi);

			blue_hi = vis_fpack16(temp_b_hi);
			temp_g_lo = vis_fpadd16(g_lo, z_11644_lo);

			red_hi = vis_fpack16(temp_r_hi);
			temp_b_lo = vis_fpadd16(b_lo, z_11644_lo);

			vis_alignaddr((void *)off3, 0);
			fu1 = vis_ld_f32_nf(dfu); dfu++;
			du1 = vis_fpmerge(fu1, fu1);
			du = vis_faligndata(du0, du1);
			du0 = du1;

			green2_lo = vis_fpack16(temp_g_lo);
			temp_r_lo = vis_fpadd16(r_lo, z_11644_lo);

			vis_alignaddr((void *)off4, 0);
			fv1 = vis_ld_f32_nf(dfv); dfv++;
			dv1 = vis_fpmerge(fv1, fv1);
			dv = vis_faligndata(dv0, dv1);
			dv0 = dv1;

			blue2_lo = vis_fpack16(temp_b_lo);
			x_green_lo = vis_fmul8x16au(green_lo, k5);

			vis_alignaddr((void *)off1, 0);

			red2_lo = vis_fpack16(temp_r_lo);
			blue_red_lo = vis_fpmerge(red_lo, blue_lo);

			dd1 = vis_fpmerge(vis_read_hi(x_green_hi),
				vis_read_hi(blue_red_hi));
			dd = vis_faligndata(dd0, dd1);
			vis_pst_8(dd, dpp1, emask1);
			dpp1 += inc1;
			inc1 = 1;

			dd0 = vis_fpmerge(vis_read_lo(x_green_hi),
				vis_read_lo(blue_red_hi));
			dd = vis_faligndata(dd1, dd0);
			vis_pst_8(dd, dpp1++, emask);

			dd1 = vis_fpmerge(vis_read_hi(x_green_lo),
				vis_read_hi(blue_red_lo));
			dd = vis_faligndata(dd0, dd1);
			vis_pst_8(dd, dpp1++, emask);

			dd0 = vis_fpmerge(vis_read_lo(x_green_lo),
				vis_read_lo(blue_red_lo));
			dd = vis_faligndata(dd1, dd0);
			vis_pst_8(dd, dpp1++, emask);

			vis_alignaddr((void *)off2, 0);
			x_green_hi = vis_fmul8x16au(green_hi, k5);
			blue_red_hi = vis_fpmerge(red_hi, blue_hi);

			dd3 = vis_fpmerge(vis_read_hi(x_green_hi),
				vis_read_hi(blue_red_hi));
			dd = vis_faligndata(dd2, dd3);
			vis_pst_8(dd, dpp2, emask3);
			dpp2 += inc2;
			inc2 = 1;

			dd2 = vis_fpmerge(vis_read_lo(x_green_hi),
				vis_read_lo(blue_red_hi));
			dd = vis_faligndata(dd3, dd2);
			vis_pst_8(dd, dpp2++, emask2);

/* U*(-0.3920); */
			u_3920_hi = vis_fmul8x16au(vis_read_hi(du), k34);
			x_green_lo = vis_fmul8x16al(green2_lo, fone);

/* V*(-0.8132); */
			v_8132_hi = vis_fmul8x16al(vis_read_hi(dv), k34);
			blue_red_lo = vis_fpmerge(red2_lo, blue2_lo);

			u_3920_lo = vis_fmul8x16au(vis_read_lo(du), k34);
			dd3 = vis_fpmerge(vis_read_hi(x_green_lo),
				vis_read_hi(blue_red_lo));
			dd = vis_faligndata(dd2, dd3);
			vis_pst_8(dd, dpp2++, emask2);

			v_8132_lo = vis_fmul8x16al(vis_read_lo(dv), k34);
			dd2 = vis_fpmerge(vis_read_lo(x_green_lo),
				vis_read_lo(blue_red_lo));
			dd = vis_faligndata(dd3, dd2);
			vis_pst_8(dd, dpp2++, emask2);
			emask1 = emask;
			emask3 = emask2;
		}

		if (i < width) {

			ii = i;
			dy3 = vis_ld_d64_nf(spy1);
			vis_alignaddr(sp11, 0);
			dy1 = vis_faligndata(dy0, dy3);

			dy5 = vis_ld_d64_nf(spy2);
			vis_alignaddr(sp12, 0);
			dy2 = vis_faligndata(dy4, dy5);

			u_20184_hi = vis_fmul8x16al(vis_read_hi(du), k5);
			g_hi = vis_fpadd16(u_3920_hi, v_8132_hi);

			u_20184_lo = vis_fmul8x16al(vis_read_lo(du), k5);
			g_hi = vis_fpadd16(g_hi, k_135_6352);

			v_15966_hi = vis_fmul8x16al(vis_read_hi(dv), k12);
			g_lo = vis_fpadd16(u_3920_lo, v_8132_lo);

			v_15966_lo = vis_fmul8x16al(vis_read_lo(dv), k12);
			g_lo = vis_fpadd16(g_lo, k_135_6352);

			y_11644_hi = vis_fmul8x16au(vis_read_hi(dy1), k12);
			b_hi = vis_fpsub16(u_20184_hi, k_276_9856);

			y_11644_lo = vis_fmul8x16au(vis_read_lo(dy1), k12);
			b_lo = vis_fpsub16(u_20184_lo, k_276_9856);

			z_11644_hi = vis_fmul8x16au(vis_read_hi(dy2), k12);
			r_hi = vis_fpsub16(v_15966_hi, k_222_9952);

			z_11644_lo = vis_fmul8x16au(vis_read_lo(dy2), k12);
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
			blue_red_hi = vis_fpmerge(red_hi, blue_hi);

			temp_g_hi = vis_fpadd16(g_hi, z_11644_hi);
			temp_b_hi = vis_fpadd16(b_hi, z_11644_hi);

			green_hi = vis_fpack16(temp_g_hi);
			temp_r_hi = vis_fpadd16(r_hi, z_11644_hi);

			blue_hi = vis_fpack16(temp_b_hi);
			temp_g_lo = vis_fpadd16(g_lo, z_11644_lo);

			red_hi = vis_fpack16(temp_r_hi);
			temp_b_lo = vis_fpadd16(b_lo, z_11644_lo);

			green2_lo = vis_fpack16(temp_g_lo);
			temp_r_lo = vis_fpadd16(r_lo, z_11644_lo);

			blue2_lo = vis_fpack16(temp_b_lo);
			x_green_lo = vis_fmul8x16au(green_lo, k5);

			red2_lo = vis_fpack16(temp_r_lo);
			blue_red_lo = vis_fpmerge(red_lo, blue_lo);

			vis_alignaddr((void *)off1, 0);

			dd1 = vis_fpmerge(vis_read_hi(x_green_hi),
				vis_read_hi(blue_red_hi));
			dd = vis_faligndata(dd0, dd1);
			vis_pst_8(dd, dpp1, emask1);
			dd0 = dd1;
			dpp1 += inc1;
			inc1 = 1;

			i += 2;

			if (i < width) {
				dd1 = vis_fpmerge(vis_read_lo(x_green_hi),
					vis_read_lo(blue_red_hi));
				dd = vis_faligndata(dd0, dd1);
				vis_pst_8(dd, dpp1++, emask);
				dd0 = dd1;
				i += 2;

				if (i < width) {
					dd1 = vis_fpmerge(
						vis_read_hi(x_green_lo),
						vis_read_hi(blue_red_lo));
					dd = vis_faligndata(dd0, dd1);
					vis_pst_8(dd, dpp1++, emask);
					dd0 = dd1;
				}
			}

			vis_alignaddr((void *)off2, 0);

			x_green_hi = vis_fmul8x16au(green_hi, k5);
			blue_red_hi = vis_fpmerge(red_hi, blue_hi);
			x_green_lo = vis_fmul8x16al(green2_lo, fone);
			blue_red_lo = vis_fpmerge(red2_lo, blue2_lo);
			dd3 = vis_fpmerge(vis_read_hi(x_green_hi),
				vis_read_hi(blue_red_hi));
			dd = vis_faligndata(dd2, dd3);
			vis_pst_8(dd, dpp2, emask3);
			dd2 = dd3;
			dpp2 += inc2;
			inc2 = 1;

			ii += 2;

			if (ii < width) {
				dd3 = vis_fpmerge(vis_read_lo(x_green_hi),
					vis_read_lo(blue_red_hi));
				dd = vis_faligndata(dd2, dd3);
				vis_pst_8(dd, dpp2++, emask2);
				dd2 = dd3;
				ii += 2;

				if (ii < width) {
					dd3 = vis_fpmerge(
						vis_read_hi(x_green_lo),
						vis_read_hi(blue_red_lo));
					dd = vis_faligndata(dd2, dd3);
					vis_pst_8(dd, dpp2++, emask2);
					dd2 = dd3;
				}
			}
		}

		emask1 = vis_edge8(dpp1, dend1);
		emask1 &= emask;
		vis_alignaddr((void *)off1, 0);
		dd = vis_faligndata(dd0, dd1);
		vis_pst_8(dd, dpp1, emask1);

		emask3 = vis_edge8(dpp2, dend2);
		emask3 &= emask2;
		vis_alignaddr((void *)off2, 0);
		dd = vis_faligndata(dd2, dd3);
		vis_pst_8(dd, dpp2, emask3);

		sp11 = sl11 = sl11 + y_stride2;
		sp12 = sl12 = sl12 + y_stride2;
		sp2 = sl2 = sl2 + uv_stride;
		sp3 = sl3 = sl3 + uv_stride;

		dl1 = dp1 = dl1 + argb_stride2;
		dl2 = dp2 = dl2 + argb_stride2;
		emask = 0x7777;
		emask2 = 0x7777;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
