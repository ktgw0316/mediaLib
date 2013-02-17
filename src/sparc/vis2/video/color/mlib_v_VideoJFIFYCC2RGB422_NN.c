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

#pragma ident	"@(#)mlib_v_VideoJFIFYCC2RGB422_NN.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorJFIFYCC2RGB422_Nearest - color conversion with
 *                                              422 upsampling
 *                                              with nearest filter.
 *                                              Only Cr and Cb components are
 *                                              under upsampling
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorJFIFYCC2RGB422_Nearest(mlib_u8       *rgb,
 *                                                        const mlib_u8 *y,
 *                                                        const mlib_u8 *cb,
 *                                                        const mlib_u8 *cr,
 *                                                        mlib_s32      n);
 * ARGUMENTS
 *      rgb       Pointer to RGB multicomponent row, 8-byte aligned
 *      y         Pointer to Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      n         Length of y array
 *                Note: n must be even,
 *                      length of cb, cr arrays must be n/2
 *                      length of rgb array must be 3*n
 *
 * DESCRIPTION
 *      mlib_VideoColorJFIFYCC2RGB422_Nearest:
 *        composition of mlib_VideoUpSample422_Nearest with
 *                       mlib_VideoColorJFIFYCC2RGB444
 *
 */

#include <vis_proto.h>
#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorJFIFYCC2RGB422_Nearest = \
	__mlib_VideoColorJFIFYCC2RGB422_Nearest

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorJFIFYCC2RGB422_Nearest)
	mlib_VideoColorJFIFYCC2RGB422_Nearest
	__attribute__((weak,
	alias("__mlib_VideoColorJFIFYCC2RGB422_Nearest")));

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

mlib_status
__mlib_VideoColorJFIFYCC2RGB422_Nearest(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *cb,
	const mlib_u8 *cr,
	mlib_s32 n)
{
/* pointers to dst address */
	mlib_u8 *dp;

/* all. pointer to y */
	mlib_d64 *spy;

/* all. pointers to u, v */
	mlib_f32 *dfu, *dfv;

/* u, v data */
	mlib_f32 fu, fv;

/* y data */
	mlib_d64 dy1;
	mlib_d64 du, dv;

/* (1.00000, 1.40200)*8192 */
	mlib_f32 k12 = vis_to_float(0x20002cdd);

/* (-.34414, -.71414)*8192 */
	mlib_f32 k34 = vis_to_float(0xf4fde926);

/* 1.77200*8192 */
	mlib_f32 k5 = vis_to_float(0x10038b4);

/* (179.45600 - 0.5)*32 */
	mlib_d64 k_179_456 = vis_to_double(0x165f165f, 0x165f165f);

/* (135.45984 + 0.5)*32 */
	mlib_d64 k_135_45984 = vis_to_double(0x10ff10ff, 0x10ff10ff);

/* (226.81600 - 0.5)*32 */
	mlib_d64 k_226_816 = vis_to_double(0x1c4a1c4a, 0x1c4a1c4a);

	mlib_d64 u_3920_hi, u_20184_hi, v_15966_hi, v_8132_hi;
	mlib_d64 u_3920_lo, u_20184_lo, v_15966_lo, v_8132_lo;
	mlib_d64 y_11644_hi, y_11644_lo;
	mlib_d64 r_hi, r_lo, g_hi, g_lo, b_hi, b_lo;
	mlib_d64 temp_r_hi, temp_r_lo, temp_g_hi, temp_g_lo, temp_b_hi,
		temp_b_lo;
	mlib_d64 red, green, blue, *ddp, dd0, dd1, dd2;

/* loop variable */
	mlib_s32 i;

	if (n <= 0)
		return (MLIB_FAILURE);

/*
 * initialize GSR scale factor
 */
	vis_write_gsr((2 << 3) + 7);

	dp = (mlib_u8 *)rgb;
	ddp = (mlib_d64 *)dp;

	spy = (mlib_d64 *)y;
	dfu = (mlib_f32 *)cb;
	dfv = (mlib_f32 *)cr;

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
	g_hi = vis_fpadd16(g_hi, k_135_45984);

/* V*1.5966 */
	v_15966_hi = vis_fmul8x16al(vis_read_hi(dv), k12);
	g_lo = vis_fpadd16(u_3920_lo, v_8132_lo);

	v_15966_lo = vis_fmul8x16al(vis_read_lo(dv), k12);
	g_lo = vis_fpadd16(g_lo, k_135_45984);
	fu = (*dfu++);

/* Y*1.1644 */
	y_11644_hi = vis_fmul8x16au(vis_read_hi(dy1), k12);
	b_hi = vis_fpsub16(u_20184_hi, k_226_816);
	fv = (*dfv++);

	du = vis_fpmerge(fu, fu);
	dv = vis_fpmerge(fv, fv);

/* Y*1.1644 */
	y_11644_lo = vis_fmul8x16au(vis_read_lo(dy1), k12);
	b_lo = vis_fpsub16(u_20184_lo, k_226_816);

/* U*(-0.3920); */
	u_3920_hi = vis_fmul8x16au(vis_read_hi(du), k34);
	r_hi = vis_fpsub16(v_15966_hi, k_179_456);

/* V*(-0.8132); */
	v_8132_hi = vis_fmul8x16al(vis_read_hi(dv), k34);
	r_lo = vis_fpsub16(v_15966_lo, k_179_456);

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
	for (i = 0; i <= n - 8; i += 8) {

		vis_write_bmask(0x0801902A, 0);
		dd0 = vis_bshuffle(red, green);
		vis_write_bmask(0x03B04C05, 0);
		dd1 = vis_bshuffle(red, green);
		vis_write_bmask(0xD06E07F0, 0);
		dd2 = vis_bshuffle(red, green);
		vis_write_bmask(0x01834967, 0);
		ddp[0] = vis_bshuffle(dd0, blue);
		vis_write_bmask(0xA12B45C7, 0);
		ddp[1] = vis_bshuffle(dd1, blue);
		vis_write_bmask(0x0D23E56F, 0);
		ddp[2] = vis_bshuffle(dd2, blue);

/* U*2.0184 */
		u_20184_hi = vis_fmul8x16al(vis_read_hi(du), k5);
		g_hi = vis_fpadd16(u_3920_hi, v_8132_hi);

		u_20184_lo = vis_fmul8x16al(vis_read_lo(du), k5);
		g_hi = vis_fpadd16(g_hi, k_135_45984);

/* V*1.5966 */
		v_15966_hi = vis_fmul8x16al(vis_read_hi(dv), k12);
		g_lo = vis_fpadd16(u_3920_lo, v_8132_lo);

		v_15966_lo = vis_fmul8x16al(vis_read_lo(dv), k12);
		g_lo = vis_fpadd16(g_lo, k_135_45984);
		fu = vis_ld_f32_nf(dfu); dfu++;

/* Y*1.1644 */
		y_11644_hi = vis_fmul8x16au(vis_read_hi(dy1), k12);
		b_hi = vis_fpsub16(u_20184_hi, k_226_816);
		fv = vis_ld_f32_nf(dfv); dfv++;

		du = vis_fpmerge(fu, fu);
		dv = vis_fpmerge(fv, fv);

/* Y*1.1644 */
		y_11644_lo = vis_fmul8x16au(vis_read_lo(dy1), k12);
		b_lo = vis_fpsub16(u_20184_lo, k_226_816);

/* U*(-0.3920); */
		u_3920_hi = vis_fmul8x16au(vis_read_hi(du), k34);
		r_hi = vis_fpsub16(v_15966_hi, k_179_456);

/* V*(-0.8132); */
		v_8132_hi = vis_fmul8x16al(vis_read_hi(dv), k34);
		r_lo = vis_fpsub16(v_15966_lo, k_179_456);

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
		ddp += 3;
	}

	dp = (mlib_u8 *)ddp;

	vis_alignaddr((void *)(n - i), 0);
	blue = vis_faligndata(blue, blue);
	green = vis_faligndata(green, green);
	red = vis_faligndata(red, red);
	dp += ((n - i - 1) * 3);

	vis_alignaddr((void *)7, 0);
	for (; i < n; i++) {
		STORE_PIXEL(0, 1, 2);
		dp -= 3;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
