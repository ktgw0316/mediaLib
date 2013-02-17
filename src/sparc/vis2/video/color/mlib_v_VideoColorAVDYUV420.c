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

#pragma ident	"@(#)mlib_v_VideoColorAVDYUV420.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV420seq_to_ARGBint - color convert YUV sequential
 *                                            to ARGB interleaved
 *
 * SYNOPSIS
 *
 *      void mlib_VideoColorYUV420seq_to_ABGRint(mlib_u32      *abgr,
 *                                               const mlib_u8 *y,
 *                                               const mlib_u8 *u,
 *                                               const mlib_u8 *v,
 *                                               const mlib_u8 *a_array,
 *                                               mlib_u8       a_const,
 *                                               mlib_s32      w,
 *                                               mlib_s32      h,
 *                                               mlib_s32      dlb,
 *                                               mlib_s32      aylb,
 *                                               mlib_s32      uvlb);
 *      argb        pointer to output buffer
 *      y           pointer to Y input buffer
 *      u           pointer to U input buffer
 *      v           pointer to V input buffer
 *      a           pointer to array of alpha values
 *      alpha       constant alpha value
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      ya_stride   linebytes for Y and alpha buffers
 *      uv_stride   linebytes for U and V buffers
 *
 * DESCRIPTION
 *        |R|   |1.1644   0.0000   1.5966|   [|Y|   |16.0000|]
 *        |G| = |1.1644  -0.3920  -0.8132| * [|U| - |128.0000|]
 *        |B|   |1.1644   2.0184   0.0000|   [|V|   |128.0000|]
 *
 *      The Y, U, V pixel streams are converted into a ARGB pixel stream. All
 *      pixel components are 8-bit unsigned integers. The Y buffer has
 *      dimensions width and height. The U and V buffers have dimensions
 *      width/2 and height/2.
 *      Dimensions width and height are assumed to be even. Successive rows
 *      of the output buffer ARGB use successive rows of Y and the same rows
 *      of U and V.
 *      The alpha values for this function work in the following fashion:
 *      - if a pointer is not NULL the values are taken from there.
 *      It has to have the same dimensions as the Y buffer.
 *      - if a pointer is NULL, the alpha values for every pixel are
 *      set to alpha.
 */

/*
 * FUNCTION
 *      mlib_VideoColorYUV420seq_to_ABGRint - color convert YUV sequential
 *                                            to ABGR interleaved
 *
 * SYNOPSIS
 *
 *      void mlib_VideoColorYUV420seq_to_ARGBint(mlib_u32      *argb,
 *                                               const mlib_u8 *y,
 *                                               const mlib_u8 *u,
 *                                               const mlib_u8 *v,
 *                                               const mlib_u8 *a_array,
 *                                               mlib_u8       a_const,
 *                                               mlib_s32      w,
 *                                               mlib_s32      h,
 *                                               mlib_s32      dlb,
 *                                               mlib_s32      aylb,
 *                                               mlib_s32      uvlb);
 * ARGUMENTS
 *
 * ARGUMENTS
 *      abgr        pointer to output buffer
 *      y           pointer to Y input buffer
 *      u           pointer to U input buffer
 *      v           pointer to V input buffer
 *      a           pointer to array of alpha values
 *      alpha       constant alpha value
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      ya_stride   linebytes for Y and alpha buffers
 *      uv_stride   linebytes for U and V buffers
 *
 * DESCRIPTION
 *        |R|   |1.1644   0.0000   1.5966|   [|Y|   |16.0000|]
 *        |G| = |1.1644  -0.3920  -0.8132| * [|U| - |128.0000|]
 *        |B|   |1.1644   2.0184   0.0000|   [|V|   |128.0000|]
 *
 *      The Y, U, V pixel streams are converted into a ABGR pixel stream. All
 *      pixel components are 8-bit unsigned integers. The Y buffer has
 *      dimensions width and height. The U and V buffers have dimensions
 *      width/2 and height/2.
 *      Dimensions width and height are assumed to be even. Successive rows
 *      of the output buffer ABGR use successive rows of Y and the same rows
 *      of U and V.
 *      The alpha values for this function work in the following fashion:
 *      - if a pointer is not NULL the values are taken from there.
 *      It has to have the same dimensions as the Y buffer.
 *      - if a pointer is NULL, the alpha values for every pixel are
 *      set to alpha.
 */

#include <mlib_video.h>
#include <mlib_algebra.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorYUV420seq_to_ABGRint = \
	__mlib_VideoColorYUV420seq_to_ABGRint
#pragma weak mlib_VideoColorYUV420seq_to_ARGBint = \
	__mlib_VideoColorYUV420seq_to_ARGBint

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorYUV420seq_to_ABGRint)
	mlib_VideoColorYUV420seq_to_ABGRint
	__attribute__((weak, alias("__mlib_VideoColorYUV420seq_to_ABGRint")));
__typeof__(__mlib_VideoColorYUV420seq_to_ARGBint)
	mlib_VideoColorYUV420seq_to_ARGBint
	__attribute__((weak, alias("__mlib_VideoColorYUV420seq_to_ARGBint")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	BUF_SIZE	2048

/* *********************************************************** */

static void
mlib_v_VideoYUV2ABGR_alpha_420(
	mlib_u32 *abgr0,
	mlib_u32 *abgr1,
	const mlib_d64 *y0,
	const mlib_d64 *y1,
	const mlib_f32 *u,
	const mlib_f32 *v,
	mlib_d64 a_const,
	mlib_s32 count,
	mlib_s32 left,
	mlib_s32 isrgb)
{
/* all. pointer to dst */
	mlib_d64 *dpp0 = (mlib_d64 *)abgr0;

/* all. pointer to dst */
	mlib_d64 *dpp1 = (mlib_d64 *)abgr1;
	mlib_f32 fu, fv;
	mlib_d64 dy, du, dv, dz;

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
	mlib_d64 blue_red_hi, x_green_hi, blue_red_lo, x_green_lo;
	mlib_d64 dd, dd1, dd2, dd3;

/* loop variables */
	mlib_s32 i;

	if (isrgb) {
/* (1.1644, 1.5966)*8192 */
		k12 = vis_to_float(0x25434097);
/* (-.3920, -.8132)*8192 */
		k34 = vis_to_float(0xe5faf375);
/* 2.0184*8192 */
		k5 = vis_to_float(0x1003317);
		k_276_9856 = vis_to_double(0x1be01be0, 0x1be01be0);
		k_135_6352 = vis_to_double(0x10f410f4, 0x10f410f4);
		k_222_9952 = vis_to_double(0x22a022a0, 0x22a022a0);
	}

	if (((mlib_addr)abgr0 & 7) || ((mlib_addr)abgr1 & 7)) {
		fu = (*u++);
		fv = (*v++);
		dy = (*y0++);
		dz = (*y1++);
		du = vis_fpmerge(fu, fu);
		dv = vis_fpmerge(fv, fv);
#pragma pipeloop(0)
		for (i = 0; i < count; i++) {

/* U*(-0.3920); */
			u_3920_hi = vis_fmul8x16au(vis_read_hi(du), k34);
/* V*(-0.8132); */
			v_8132_hi = vis_fmul8x16al(vis_read_hi(dv), k34);
/* U*(-0.3920); */
			u_3920_lo = vis_fmul8x16au(vis_read_lo(du), k34);
/* V*(-0.8132); */
			v_8132_lo = vis_fmul8x16al(vis_read_lo(dv), k34);

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
			y_11644_hi = vis_fmul8x16au(vis_read_hi(dy), k12);
			b_hi = vis_fpsub16(u_20184_hi, k_276_9856);

/* Y*1.1644 */
			y_11644_lo = vis_fmul8x16au(vis_read_lo(dy), k12);
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
			x_green_hi =
				vis_fpmerge(vis_read_hi(a_const), green_hi);

			red_lo = vis_fpack16(temp_r_lo);
			blue_red_hi = vis_fpmerge(blue_hi, red_hi);

			x_green_lo =
				vis_fpmerge(vis_read_lo(a_const), green_lo);
			blue_red_lo = vis_fpmerge(blue_lo, red_lo);

			dd = vis_fpmerge(vis_read_hi(x_green_hi),
				vis_read_hi(blue_red_hi));
			dd1 = vis_fpmerge(vis_read_lo(x_green_hi),
				vis_read_lo(blue_red_hi));
			dd2 = vis_fpmerge(vis_read_hi(x_green_lo),
				vis_read_hi(blue_red_lo));
			dd3 = vis_fpmerge(vis_read_lo(x_green_lo),
				vis_read_lo(blue_red_lo));

			((mlib_f32 *)dpp0)[8 * i + 0] = vis_read_hi(dd);
			((mlib_f32 *)dpp0)[8 * i + 1] = vis_read_lo(dd);
			((mlib_f32 *)dpp0)[8 * i + 2] = vis_read_hi(dd1);
			((mlib_f32 *)dpp0)[8 * i + 3] = vis_read_lo(dd1);
			((mlib_f32 *)dpp0)[8 * i + 4] = vis_read_hi(dd2);
			((mlib_f32 *)dpp0)[8 * i + 5] = vis_read_lo(dd2);
			((mlib_f32 *)dpp0)[8 * i + 6] = vis_read_hi(dd3);
			((mlib_f32 *)dpp0)[8 * i + 7] = vis_read_lo(dd3);

/* Y*1.1644 */
			z_11644_hi = vis_fmul8x16au(vis_read_hi(dz), k12);
/* Y*1.1644 */
			z_11644_lo = vis_fmul8x16au(vis_read_lo(dz), k12);

			temp_g_hi = vis_fpadd16(g_hi, z_11644_hi);
			temp_b_hi = vis_fpadd16(b_hi, z_11644_hi);

			green_hi = vis_fpack16(temp_g_hi);
			temp_r_hi = vis_fpadd16(r_hi, z_11644_hi);

			blue_hi = vis_fpack16(temp_b_hi);
			temp_g_lo = vis_fpadd16(g_lo, z_11644_lo);

			red_hi = vis_fpack16(temp_r_hi);
			temp_b_lo = vis_fpadd16(b_lo, z_11644_lo);

			green_lo = vis_fpack16(temp_g_lo);
			temp_r_lo = vis_fpadd16(r_lo, z_11644_lo);

			blue_lo = vis_fpack16(temp_b_lo);
			x_green_hi =
				vis_fpmerge(vis_read_hi(a_const), green_hi);

			red_lo = vis_fpack16(temp_r_lo);
			blue_red_hi = vis_fpmerge(blue_hi, red_hi);

			x_green_lo =
				vis_fpmerge(vis_read_lo(a_const), green_lo);
			blue_red_lo = vis_fpmerge(blue_lo, red_lo);

			dd = vis_fpmerge(vis_read_hi(x_green_hi),
				vis_read_hi(blue_red_hi));
			dd1 = vis_fpmerge(vis_read_lo(x_green_hi),
				vis_read_lo(blue_red_hi));
			dd2 = vis_fpmerge(vis_read_hi(x_green_lo),
				vis_read_hi(blue_red_lo));
			dd3 = vis_fpmerge(vis_read_lo(x_green_lo),
				vis_read_lo(blue_red_lo));

			((mlib_f32 *)dpp1)[8 * i + 0] = vis_read_hi(dd);
			((mlib_f32 *)dpp1)[8 * i + 1] = vis_read_lo(dd);
			((mlib_f32 *)dpp1)[8 * i + 2] = vis_read_hi(dd1);
			((mlib_f32 *)dpp1)[8 * i + 3] = vis_read_lo(dd1);
			((mlib_f32 *)dpp1)[8 * i + 4] = vis_read_hi(dd2);
			((mlib_f32 *)dpp1)[8 * i + 5] = vis_read_lo(dd2);
			((mlib_f32 *)dpp1)[8 * i + 6] = vis_read_hi(dd3);
			((mlib_f32 *)dpp1)[8 * i + 7] = vis_read_lo(dd3);

			fu = (*u++);
			fv = (*v++);
			dy = (*y0++);
			dz = (*y1++);
			du = vis_fpmerge(fu, fu);
			dv = vis_fpmerge(fv, fv);
		}
	} else {

		fu = (*u++);
		fv = (*v++);
		dy = (*y0++);
		dz = (*y1++);
		du = vis_fpmerge(fu, fu);
		dv = vis_fpmerge(fv, fv);
#pragma pipeloop(0)
		for (i = 0; i < count; i++) {

/* U*(-0.3920); */
			u_3920_hi = vis_fmul8x16au(vis_read_hi(du), k34);
/* V*(-0.8132); */
			v_8132_hi = vis_fmul8x16al(vis_read_hi(dv), k34);
/* U*(-0.3920); */
			u_3920_lo = vis_fmul8x16au(vis_read_lo(du), k34);
/* V*(-0.8132); */
			v_8132_lo = vis_fmul8x16al(vis_read_lo(dv), k34);

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
			y_11644_hi = vis_fmul8x16au(vis_read_hi(dy), k12);
			b_hi = vis_fpsub16(u_20184_hi, k_276_9856);

/* Y*1.1644 */
			y_11644_lo = vis_fmul8x16au(vis_read_lo(dy), k12);
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
			x_green_hi =
				vis_fpmerge(vis_read_hi(a_const), green_hi);

			red_lo = vis_fpack16(temp_r_lo);
			blue_red_hi = vis_fpmerge(blue_hi, red_hi);

			x_green_lo =
				vis_fpmerge(vis_read_lo(a_const), green_lo);
			blue_red_lo = vis_fpmerge(blue_lo, red_lo);

			dd = vis_fpmerge(vis_read_hi(x_green_hi),
				vis_read_hi(blue_red_hi));
			dd1 = vis_fpmerge(vis_read_lo(x_green_hi),
				vis_read_lo(blue_red_hi));
			dd2 = vis_fpmerge(vis_read_hi(x_green_lo),
				vis_read_hi(blue_red_lo));
			dd3 = vis_fpmerge(vis_read_lo(x_green_lo),
				vis_read_lo(blue_red_lo));

			dpp0[4 * i + 0] = dd;
			dpp0[4 * i + 1] = dd1;
			dpp0[4 * i + 2] = dd2;
			dpp0[4 * i + 3] = dd3;

/* Y*1.1644 */
			z_11644_hi = vis_fmul8x16au(vis_read_hi(dz), k12);
/* Y*1.1644 */
			z_11644_lo = vis_fmul8x16au(vis_read_lo(dz), k12);

			temp_g_hi = vis_fpadd16(g_hi, z_11644_hi);
			temp_b_hi = vis_fpadd16(b_hi, z_11644_hi);

			green_hi = vis_fpack16(temp_g_hi);
			temp_r_hi = vis_fpadd16(r_hi, z_11644_hi);

			blue_hi = vis_fpack16(temp_b_hi);
			temp_g_lo = vis_fpadd16(g_lo, z_11644_lo);

			red_hi = vis_fpack16(temp_r_hi);
			temp_b_lo = vis_fpadd16(b_lo, z_11644_lo);

			green_lo = vis_fpack16(temp_g_lo);
			temp_r_lo = vis_fpadd16(r_lo, z_11644_lo);

			blue_lo = vis_fpack16(temp_b_lo);
			x_green_hi =
				vis_fpmerge(vis_read_hi(a_const), green_hi);

			red_lo = vis_fpack16(temp_r_lo);
			blue_red_hi = vis_fpmerge(blue_hi, red_hi);

			x_green_lo =
				vis_fpmerge(vis_read_lo(a_const), green_lo);
			blue_red_lo = vis_fpmerge(blue_lo, red_lo);

			dd = vis_fpmerge(vis_read_hi(x_green_hi),
				vis_read_hi(blue_red_hi));
			dd1 = vis_fpmerge(vis_read_lo(x_green_hi),
				vis_read_lo(blue_red_hi));
			dd2 = vis_fpmerge(vis_read_hi(x_green_lo),
				vis_read_hi(blue_red_lo));
			dd3 = vis_fpmerge(vis_read_lo(x_green_lo),
				vis_read_lo(blue_red_lo));

			dpp1[4 * i + 0] = dd;
			dpp1[4 * i + 1] = dd1;
			dpp1[4 * i + 2] = dd2;
			dpp1[4 * i + 3] = dd3;

			fu = (*u++);
			fv = (*v++);
			dy = (*y0++);
			dz = (*y1++);
			du = vis_fpmerge(fu, fu);
			dv = vis_fpmerge(fv, fv);
		}
	}

	if (left) {
		mlib_d64 res_buf0[4];
		mlib_d64 res_buf1[4];

/* U*(-0.3920); */
		u_3920_hi = vis_fmul8x16au(vis_read_hi(du), k34);
/* V*(-0.8132); */
		v_8132_hi = vis_fmul8x16al(vis_read_hi(dv), k34);
/* U*(-0.3920); */
		u_3920_lo = vis_fmul8x16au(vis_read_lo(du), k34);
/* V*(-0.8132); */
		v_8132_lo = vis_fmul8x16al(vis_read_lo(dv), k34);

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
		y_11644_hi = vis_fmul8x16au(vis_read_hi(dy), k12);
		b_hi = vis_fpsub16(u_20184_hi, k_276_9856);

/* Y*1.1644 */
		y_11644_lo = vis_fmul8x16au(vis_read_lo(dy), k12);
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

		x_green_hi = vis_fpmerge(vis_read_hi(a_const), green_hi);

		red_lo = vis_fpack16(temp_r_lo);
		blue_red_hi = vis_fpmerge(blue_hi, red_hi);

		x_green_lo = vis_fpmerge(vis_read_lo(a_const), green_lo);
		blue_red_lo = vis_fpmerge(blue_lo, red_lo);

		dd = vis_fpmerge(vis_read_hi(x_green_hi),
			vis_read_hi(blue_red_hi));
		dd1 = vis_fpmerge(vis_read_lo(x_green_hi),
			vis_read_lo(blue_red_hi));
		dd2 = vis_fpmerge(vis_read_hi(x_green_lo),
			vis_read_hi(blue_red_lo));
		dd3 = vis_fpmerge(vis_read_lo(x_green_lo),
			vis_read_lo(blue_red_lo));

		res_buf0[0] = dd;
		res_buf0[1] = dd1;
		res_buf0[2] = dd2;
		res_buf0[3] = dd3;

/* Y*1.1644 */
		z_11644_hi = vis_fmul8x16au(vis_read_hi(dz), k12);
/* Y*1.1644 */
		z_11644_lo = vis_fmul8x16au(vis_read_lo(dz), k12);

		temp_g_hi = vis_fpadd16(g_hi, z_11644_hi);
		temp_b_hi = vis_fpadd16(b_hi, z_11644_hi);

		green_hi = vis_fpack16(temp_g_hi);
		temp_r_hi = vis_fpadd16(r_hi, z_11644_hi);

		blue_hi = vis_fpack16(temp_b_hi);
		temp_g_lo = vis_fpadd16(g_lo, z_11644_lo);

		red_hi = vis_fpack16(temp_r_hi);
		temp_b_lo = vis_fpadd16(b_lo, z_11644_lo);

		green_lo = vis_fpack16(temp_g_lo);
		temp_r_lo = vis_fpadd16(r_lo, z_11644_lo);

		blue_lo = vis_fpack16(temp_b_lo);
		x_green_hi = vis_fpmerge(vis_read_hi(a_const), green_hi);

		red_lo = vis_fpack16(temp_r_lo);
		blue_red_hi = vis_fpmerge(blue_hi, red_hi);

		x_green_lo = vis_fpmerge(vis_read_lo(a_const), green_lo);
		blue_red_lo = vis_fpmerge(blue_lo, red_lo);

		dd = vis_fpmerge(vis_read_hi(x_green_hi),
			vis_read_hi(blue_red_hi));
		dd1 = vis_fpmerge(vis_read_lo(x_green_hi),
			vis_read_lo(blue_red_hi));
		dd2 = vis_fpmerge(vis_read_hi(x_green_lo),
			vis_read_hi(blue_red_lo));
		dd3 = vis_fpmerge(vis_read_lo(x_green_lo),
			vis_read_lo(blue_red_lo));

		res_buf1[0] = dd;
		res_buf1[1] = dd1;
		res_buf1[2] = dd2;
		res_buf1[3] = dd3;

		for (i = 0; i < left; i++) {
			((mlib_f32 *)dpp0)[8 * count + i] =
				((mlib_f32 *)res_buf0)[i];
			((mlib_f32 *)dpp1)[8 * count + i] =
				((mlib_f32 *)res_buf1)[i];
		}
	}
}

/* *********************************************************** */

static void
mlib_v_VideoYUV2ABGR_aarray_420(
	mlib_u32 *abgr0,
	mlib_u32 *abgr1,
	const mlib_d64 *y0,
	const mlib_d64 *y1,
	const mlib_f32 *u,
	const mlib_f32 *v,
	const mlib_d64 *a0,
	const mlib_d64 *a1,
	mlib_s32 count,
	mlib_s32 left,
	mlib_s32 isrgb)
{
/* all. pointer to dst */
	mlib_d64 *dpp0 = (mlib_d64 *)abgr0;

/* all. pointer to dst */
	mlib_d64 *dpp1 = (mlib_d64 *)abgr1;
	mlib_f32 fu, fv;
	mlib_d64 dy, du, dv, da0, da1, dz;

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
	mlib_d64 blue_red_hi, x_green_hi, blue_red_lo, x_green_lo;
	mlib_d64 dd, dd1, dd2, dd3;

/* loop variables */
	mlib_s32 i;

	if (isrgb) {
/* (1.1644, 1.5966)*8192 */
		k12 = vis_to_float(0x25434097);
/* (-.3920, -.8132)*8192 */
		k34 = vis_to_float(0xe5faf375);
/* 2.0184*8192 */
		k5 = vis_to_float(0x1003317);
		k_276_9856 = vis_to_double(0x1be01be0, 0x1be01be0);
		k_135_6352 = vis_to_double(0x10f410f4, 0x10f410f4);
		k_222_9952 = vis_to_double(0x22a022a0, 0x22a022a0);
	}

	if (((mlib_addr)abgr0 & 7) || ((mlib_addr)abgr1 & 7)) {
#pragma pipeloop(0)
		for (i = 0; i < count; i++) {
			fu = (*u++);
			fv = (*v++);
			dy = (*y0++);
			dz = (*y1++);
			da0 = (*a0++);
			da1 = (*a1++);

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
			y_11644_hi = vis_fmul8x16au(vis_read_hi(dy), k12);
			b_hi = vis_fpsub16(u_20184_hi, k_276_9856);

/* Y*1.1644 */
			y_11644_lo = vis_fmul8x16au(vis_read_lo(dy), k12);
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
			x_green_hi = vis_fpmerge(vis_read_hi(da0), green_hi);

			red_lo = vis_fpack16(temp_r_lo);
			blue_red_hi = vis_fpmerge(blue_hi, red_hi);

			x_green_lo = vis_fpmerge(vis_read_lo(da0), green_lo);
			blue_red_lo = vis_fpmerge(blue_lo, red_lo);

			dd = vis_fpmerge(vis_read_hi(x_green_hi),
				vis_read_hi(blue_red_hi));

			dd1 = vis_fpmerge(vis_read_lo(x_green_hi),
				vis_read_lo(blue_red_hi));
			dd2 = vis_fpmerge(vis_read_hi(x_green_lo),
				vis_read_hi(blue_red_lo));

			dd3 = vis_fpmerge(vis_read_lo(x_green_lo),
				vis_read_lo(blue_red_lo));

			((mlib_f32 *)dpp0)[8 * i + 0] = vis_read_hi(dd);
			((mlib_f32 *)dpp0)[8 * i + 1] = vis_read_lo(dd);
			((mlib_f32 *)dpp0)[8 * i + 2] = vis_read_hi(dd1);
			((mlib_f32 *)dpp0)[8 * i + 3] = vis_read_lo(dd1);
			((mlib_f32 *)dpp0)[8 * i + 4] = vis_read_hi(dd2);
			((mlib_f32 *)dpp0)[8 * i + 5] = vis_read_lo(dd2);
			((mlib_f32 *)dpp0)[8 * i + 6] = vis_read_hi(dd3);
			((mlib_f32 *)dpp0)[8 * i + 7] = vis_read_lo(dd3);

/* Y*1.1644 */
			z_11644_hi = vis_fmul8x16au(vis_read_hi(dz), k12);
/* Y*1.1644 */
			z_11644_lo = vis_fmul8x16au(vis_read_lo(dz), k12);

			temp_g_hi = vis_fpadd16(g_hi, z_11644_hi);
			temp_b_hi = vis_fpadd16(b_hi, z_11644_hi);

			green_hi = vis_fpack16(temp_g_hi);
			temp_r_hi = vis_fpadd16(r_hi, z_11644_hi);

			blue_hi = vis_fpack16(temp_b_hi);
			temp_g_lo = vis_fpadd16(g_lo, z_11644_lo);

			red_hi = vis_fpack16(temp_r_hi);
			temp_b_lo = vis_fpadd16(b_lo, z_11644_lo);

			green_lo = vis_fpack16(temp_g_lo);
			temp_r_lo = vis_fpadd16(r_lo, z_11644_lo);

			blue_lo = vis_fpack16(temp_b_lo);
			x_green_hi = vis_fpmerge(vis_read_hi(da1), green_hi);

			red_lo = vis_fpack16(temp_r_lo);
			blue_red_hi = vis_fpmerge(blue_hi, red_hi);

			x_green_lo = vis_fpmerge(vis_read_lo(da1), green_lo);
			blue_red_lo = vis_fpmerge(blue_lo, red_lo);

			dd = vis_fpmerge(vis_read_hi(x_green_hi),
				vis_read_hi(blue_red_hi));
			dd1 = vis_fpmerge(vis_read_lo(x_green_hi),
				vis_read_lo(blue_red_hi));
			dd2 = vis_fpmerge(vis_read_hi(x_green_lo),
				vis_read_hi(blue_red_lo));
			dd3 = vis_fpmerge(vis_read_lo(x_green_lo),
				vis_read_lo(blue_red_lo));

			((mlib_f32 *)dpp1)[8 * i + 0] = vis_read_hi(dd);
			((mlib_f32 *)dpp1)[8 * i + 1] = vis_read_lo(dd);
			((mlib_f32 *)dpp1)[8 * i + 2] = vis_read_hi(dd1);
			((mlib_f32 *)dpp1)[8 * i + 3] = vis_read_lo(dd1);
			((mlib_f32 *)dpp1)[8 * i + 4] = vis_read_hi(dd2);
			((mlib_f32 *)dpp1)[8 * i + 5] = vis_read_lo(dd2);
			((mlib_f32 *)dpp1)[8 * i + 6] = vis_read_hi(dd3);
			((mlib_f32 *)dpp1)[8 * i + 7] = vis_read_lo(dd3);
		}
	} else {
#pragma pipeloop(0)
		for (i = 0; i < count; i++) {
			fu = (*u++);
			fv = (*v++);
			dy = (*y0++);
			dz = (*y1++);
			da0 = (*a0++);
			da1 = (*a1++);

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
			y_11644_hi = vis_fmul8x16au(vis_read_hi(dy), k12);
			b_hi = vis_fpsub16(u_20184_hi, k_276_9856);

/* Y*1.1644 */
			y_11644_lo = vis_fmul8x16au(vis_read_lo(dy), k12);
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
			x_green_hi = vis_fpmerge(vis_read_hi(da0), green_hi);

			red_lo = vis_fpack16(temp_r_lo);
			blue_red_hi = vis_fpmerge(blue_hi, red_hi);

			x_green_lo = vis_fpmerge(vis_read_lo(da0), green_lo);
			blue_red_lo = vis_fpmerge(blue_lo, red_lo);

			dd = vis_fpmerge(vis_read_hi(x_green_hi),
				vis_read_hi(blue_red_hi));

			dd1 = vis_fpmerge(vis_read_lo(x_green_hi),
				vis_read_lo(blue_red_hi));
			dd2 = vis_fpmerge(vis_read_hi(x_green_lo),
				vis_read_hi(blue_red_lo));

			dd3 = vis_fpmerge(vis_read_lo(x_green_lo),
				vis_read_lo(blue_red_lo));

			dpp0[4 * i + 0] = dd;
			dpp0[4 * i + 1] = dd1;
			dpp0[4 * i + 2] = dd2;
			dpp0[4 * i + 3] = dd3;

/* Y*1.1644 */
			z_11644_hi = vis_fmul8x16au(vis_read_hi(dz), k12);
/* Y*1.1644 */
			z_11644_lo = vis_fmul8x16au(vis_read_lo(dz), k12);

			temp_g_hi = vis_fpadd16(g_hi, z_11644_hi);
			temp_b_hi = vis_fpadd16(b_hi, z_11644_hi);

			green_hi = vis_fpack16(temp_g_hi);
			temp_r_hi = vis_fpadd16(r_hi, z_11644_hi);

			blue_hi = vis_fpack16(temp_b_hi);
			temp_g_lo = vis_fpadd16(g_lo, z_11644_lo);

			red_hi = vis_fpack16(temp_r_hi);
			temp_b_lo = vis_fpadd16(b_lo, z_11644_lo);

			green_lo = vis_fpack16(temp_g_lo);
			temp_r_lo = vis_fpadd16(r_lo, z_11644_lo);

			blue_lo = vis_fpack16(temp_b_lo);
			x_green_hi = vis_fpmerge(vis_read_hi(da1), green_hi);

			red_lo = vis_fpack16(temp_r_lo);
			blue_red_hi = vis_fpmerge(blue_hi, red_hi);

			x_green_lo = vis_fpmerge(vis_read_lo(da1), green_lo);
			blue_red_lo = vis_fpmerge(blue_lo, red_lo);

			dd = vis_fpmerge(vis_read_hi(x_green_hi),
				vis_read_hi(blue_red_hi));
			dd1 = vis_fpmerge(vis_read_lo(x_green_hi),
				vis_read_lo(blue_red_hi));
			dd2 = vis_fpmerge(vis_read_hi(x_green_lo),
				vis_read_hi(blue_red_lo));
			dd3 = vis_fpmerge(vis_read_lo(x_green_lo),
				vis_read_lo(blue_red_lo));

			da1 = a1[i];

			dpp1[4 * i + 0] = dd;
			dpp1[4 * i + 1] = dd1;
			dpp1[4 * i + 2] = dd2;
			dpp1[4 * i + 3] = dd3;
		}
	}

	if (left) {
		mlib_d64 res_buf0[4], res_buf1[4];

		fu = (*u++);
		fv = (*v++);
		dy = (*y0++);
		dz = (*y1++);
		da0 = (*a0++);
		da1 = (*a1++);

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
		y_11644_hi = vis_fmul8x16au(vis_read_hi(dy), k12);
		b_hi = vis_fpsub16(u_20184_hi, k_276_9856);

/* Y*1.1644 */
		y_11644_lo = vis_fmul8x16au(vis_read_lo(dy), k12);
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

		x_green_hi = vis_fpmerge(vis_read_hi(da0), green_hi);

		red_lo = vis_fpack16(temp_r_lo);
		blue_red_hi = vis_fpmerge(blue_hi, red_hi);

		x_green_lo = vis_fpmerge(vis_read_lo(da0), green_lo);
		blue_red_lo = vis_fpmerge(blue_lo, red_lo);

		res_buf0[0] =
			vis_fpmerge(vis_read_hi(x_green_hi),
			vis_read_hi(blue_red_hi));
		res_buf0[1] =
			vis_fpmerge(vis_read_lo(x_green_hi),
			vis_read_lo(blue_red_hi));
		res_buf0[2] =
			vis_fpmerge(vis_read_hi(x_green_lo),
			vis_read_hi(blue_red_lo));
		res_buf0[3] =
			vis_fpmerge(vis_read_lo(x_green_lo),
			vis_read_lo(blue_red_lo));

/* Y*1.1644 */
		z_11644_hi = vis_fmul8x16au(vis_read_hi(dz), k12);
/* Y*1.1644 */
		z_11644_lo = vis_fmul8x16au(vis_read_lo(dz), k12);

		temp_g_hi = vis_fpadd16(g_hi, z_11644_hi);
		temp_b_hi = vis_fpadd16(b_hi, z_11644_hi);

		green_hi = vis_fpack16(temp_g_hi);
		temp_r_hi = vis_fpadd16(r_hi, z_11644_hi);

		blue_hi = vis_fpack16(temp_b_hi);
		temp_g_lo = vis_fpadd16(g_lo, z_11644_lo);

		red_hi = vis_fpack16(temp_r_hi);
		temp_b_lo = vis_fpadd16(b_lo, z_11644_lo);

		green_lo = vis_fpack16(temp_g_lo);
		temp_r_lo = vis_fpadd16(r_lo, z_11644_lo);

		blue_lo = vis_fpack16(temp_b_lo);
		x_green_hi = vis_fpmerge(vis_read_hi(da1), green_hi);

		red_lo = vis_fpack16(temp_r_lo);
		blue_red_hi = vis_fpmerge(blue_hi, red_hi);

		x_green_lo = vis_fpmerge(vis_read_lo(da1), green_lo);
		blue_red_lo = vis_fpmerge(blue_lo, red_lo);

		dd = vis_fpmerge(vis_read_hi(x_green_hi),
			vis_read_hi(blue_red_hi));
		dd1 = vis_fpmerge(vis_read_lo(x_green_hi),
			vis_read_lo(blue_red_hi));
		dd2 = vis_fpmerge(vis_read_hi(x_green_lo),
			vis_read_hi(blue_red_lo));
		dd3 = vis_fpmerge(vis_read_lo(x_green_lo),
			vis_read_lo(blue_red_lo));

		res_buf1[0] =
			vis_fpmerge(vis_read_hi(x_green_hi),
			vis_read_hi(blue_red_hi));
		res_buf1[1] =
			vis_fpmerge(vis_read_lo(x_green_hi),
			vis_read_lo(blue_red_hi));
		res_buf1[2] =
			vis_fpmerge(vis_read_hi(x_green_lo),
			vis_read_hi(blue_red_lo));
		res_buf1[3] =
			vis_fpmerge(vis_read_lo(x_green_lo),
			vis_read_lo(blue_red_lo));

		for (i = 0; i < left; i++) {
			((mlib_f32 *)dpp0)[8 * count + i] =
				((mlib_f32 *)res_buf0)[i];
			((mlib_f32 *)dpp1)[8 * count + i] =
				((mlib_f32 *)res_buf1)[i];
		}
	}
}

/* *********************************************************** */

void
__mlib_VideoColorYUV420seq_to_ABGRint(
	mlib_u32 *abgr,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	const mlib_u8 *a_array,
	mlib_u8 a_const,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 aylb,
	mlib_s32 uvlb)
{
	mlib_d64 y_buf[BUF_SIZE / 8], y_buf_[BUF_SIZE / 8];
	mlib_d64 u_buf[BUF_SIZE / 16], v_buf[BUF_SIZE / 16];
	mlib_d64 a_buf[BUF_SIZE / 8], a_buf_[BUF_SIZE / 8];
	mlib_d64 *y0 = y_buf, *y0_ = y_buf_, *u0 = u_buf, *v0 = v_buf;
	mlib_d64 *a0 = a_buf, *a0_ = a_buf_;
	mlib_d64 *y_tmp = NULL, *z_tmp;
	mlib_d64 w_alpha = vis_to_double_dup((a_const << 24) | (a_const << 16) |
		(a_const << 8) | a_const);
	mlib_s32 w_algn = ((w + 7) & ~7), w2 = w_algn >> 1;
	mlib_s32 i, count = w >> 3, left = w & 7;

	dlb >>= 2;
	h >>= 1;

/*
 * initialize GSR scale factor
 */
	vis_write_gsr(2 << 3);

	if (w > BUF_SIZE) {
		y0 = (y_tmp = __mlib_malloc(5 * w_algn));
		if (y_tmp == NULL)
			return;
		y0_ = (z_tmp = y_tmp + w_algn / sizeof (mlib_d64));
		u0 = (z_tmp = z_tmp + w2 / sizeof (mlib_d64));
		v0 = (z_tmp = z_tmp + w2 / sizeof (mlib_d64));
		a0 = (z_tmp = z_tmp + w_algn / sizeof (mlib_d64));
		a0_ = (z_tmp = z_tmp + w_algn / sizeof (mlib_d64));
	}

	for (i = 0; i < h;
		i++, abgr += 2 * dlb, y += 2 * aylb, u += uvlb, v += uvlb) {
		mlib_d64 *y1 = y0, *y1_ = y0_, *u1 = u0, *v1 = v0;

		if (((mlib_addr)y & 7) == 0)
			y1 = (mlib_d64 *)y;
		else
			__mlib_VectorCopy_U8((mlib_u8 *)y1, y, w);

		if (((mlib_addr)(y + aylb) & 7) == 0)
			y1_ = (mlib_d64 *)(y + aylb);
		else
			__mlib_VectorCopy_U8((mlib_u8 *)y1_, (y + aylb),
				w);

		if (((mlib_addr)u & 3) == 0)
			u1 = (mlib_d64 *)u;
		else
			__mlib_VectorCopy_U8((mlib_u8 *)u1, u, w / 2);

		if (((mlib_addr)v & 3) == 0)
			v1 = (mlib_d64 *)v;
		else
			__mlib_VectorCopy_U8((mlib_u8 *)v1, v, w / 2);

		if (a_array) {
			mlib_d64 *a1 = a0, *a1_ = a0_;

			if (((mlib_addr)a_array & 7) == 0)
				a1 = (mlib_d64 *)a_array;
			else
				__mlib_VectorCopy_U8((mlib_u8 *)a1, a_array,
					w);

			if (((mlib_addr)(a_array + aylb) & 7) == 0)
				a1_ = (mlib_d64 *)(a_array + aylb);
			else
				__mlib_VectorCopy_U8((mlib_u8 *)a1_,
					(a_array + aylb), w);

			mlib_v_VideoYUV2ABGR_aarray_420(abgr, abgr + dlb, y1,
				y1_, (mlib_f32 *)u1, (mlib_f32 *)v1,
				(mlib_d64 *)a1, (mlib_d64 *)a1_, count, left,
				0);
			a_array += 2 * aylb;
		} else {
			mlib_v_VideoYUV2ABGR_alpha_420(abgr, abgr + dlb, y1,
				y1_, (mlib_f32 *)u1, (mlib_f32 *)v1, w_alpha,
				count, left, 0);
		}
	}

	if (y_tmp) {
		__mlib_free(y_tmp);
	}
}

/* *********************************************************** */

void
__mlib_VideoColorYUV420seq_to_ARGBint(
	mlib_u32 *argb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	const mlib_u8 *a_array,
	mlib_u8 a_const,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 aylb,
	mlib_s32 uvlb)
{
	mlib_d64 y_buf[BUF_SIZE / 8], y_buf_[BUF_SIZE / 8];
	mlib_d64 u_buf[BUF_SIZE / 16], v_buf[BUF_SIZE / 16];
	mlib_d64 a_buf[BUF_SIZE / 8], a_buf_[BUF_SIZE / 8];
	mlib_d64 *y0 = y_buf, *y0_ = y_buf_, *u0 = u_buf, *v0 = v_buf;
	mlib_d64 *a0 = a_buf, *a0_ = a_buf_;
	mlib_d64 *y_tmp = NULL, *z_tmp;
	mlib_d64 w_alpha = vis_to_double_dup((a_const << 24) | (a_const << 16) |
		(a_const << 8) | a_const);
	mlib_s32 w_algn = ((w + 7) & ~7), w2 = w_algn >> 1;
	mlib_s32 i, count = w >> 3, left = w & 7;

	dlb >>= 2;
	h >>= 1;

/*
 * initialize GSR scale factor
 */
	vis_write_gsr(2 << 3);

	if (w > BUF_SIZE) {
		y0 = (y_tmp = __mlib_malloc(5 * w_algn));
		if (y_tmp == NULL)
			return;
		y0_ = (z_tmp = y_tmp + w_algn / sizeof (mlib_d64));
		u0 = (z_tmp = z_tmp + w2 / sizeof (mlib_d64));
		v0 = (z_tmp = z_tmp + w2 / sizeof (mlib_d64));
		a0 = (z_tmp = z_tmp + w_algn / sizeof (mlib_d64));
		a0_ = (z_tmp = z_tmp + w_algn / sizeof (mlib_d64));
	}

	for (i = 0; i < h;
		i++, argb += 2 * dlb, y += 2 * aylb, u += uvlb, v += uvlb) {
		mlib_d64 *y1 = y0, *y1_ = y0_, *u1 = u0, *v1 = v0;

		if (((mlib_addr)y & 7) == 0)
			y1 = (mlib_d64 *)y;
		else
			__mlib_VectorCopy_U8((mlib_u8 *)y1, y, w);

		if (((mlib_addr)(y + aylb) & 7) == 0)
			y1_ = (mlib_d64 *)(y + aylb);
		else
			__mlib_VectorCopy_U8((mlib_u8 *)y1_, (y + aylb),
				w);

		if (((mlib_addr)u & 3) == 0)
			u1 = (mlib_d64 *)u;
		else
			__mlib_VectorCopy_U8((mlib_u8 *)u1, u, w / 2);

		if (((mlib_addr)v & 3) == 0)
			v1 = (mlib_d64 *)v;
		else
			__mlib_VectorCopy_U8((mlib_u8 *)v1, v, w / 2);

		if (a_array) {
			mlib_d64 *a1 = a0, *a1_ = a0_;

			if (((mlib_addr)a_array & 7) == 0)
				a1 = (mlib_d64 *)a_array;
			else
				__mlib_VectorCopy_U8((mlib_u8 *)a1, a_array,
					w);

			if (((mlib_addr)(a_array + aylb) & 7) == 0)
				a1_ = (mlib_d64 *)(a_array + aylb);
			else
				__mlib_VectorCopy_U8((mlib_u8 *)a1_,
					(a_array + aylb), w);

			mlib_v_VideoYUV2ABGR_aarray_420(argb, argb + dlb, y1,
				y1_, (mlib_f32 *)v1, (mlib_f32 *)u1,
				(mlib_d64 *)a1, (mlib_d64 *)a1_, count, left,
				1);
			a_array += 2 * aylb;
		} else {
			mlib_v_VideoYUV2ABGR_alpha_420(argb, argb + dlb, y1,
				y1_, (mlib_f32 *)v1, (mlib_f32 *)u1, w_alpha,
				count, left, 1);
		}
	}

	if (y_tmp) {
		__mlib_free(y_tmp);
	}
}

/* *********************************************************** */
