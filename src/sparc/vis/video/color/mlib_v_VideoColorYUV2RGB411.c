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

#pragma ident	"@(#)mlib_v_VideoColorYUV2RGB411.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV2RGB411 - color conversion from YUV411 to RGB
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorYUV2RGB411(mlib_u8       *rgb,
 *                                            const mlib_u8 *y,
 *                                            const mlib_u8 *u,
 *                                            const mlib_u8 *v,
 *                                            mlib_s32      width,
 *                                            mlib_s32      height,
 *                                            mlib_s32      rgb_stride,
 *                                            mlib_s32      y_stride,
 *                                            mlib_s32      uv_stride);
 * ARGUMENT
 *      rgb             Pointer to the output RGB image
 *      y               Pointer to input Y component
 *      u               Pointer to input U component
 *      v               Pointer to input V component
 *      width           Width of the image
 *      height          Height of the image
 *      rgb_stride      Stride in bytes between adjacent rows
 *                      in output image
 *      y_stride        Stride in bytes between adjacent rows
 *                      in Y input image
 *      uv_stride       Stride in bytes between adjacent rows
 *                      in U and V input image
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

#pragma weak mlib_VideoColorYUV2RGB411 = __mlib_VideoColorYUV2RGB411

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorYUV2RGB411) mlib_VideoColorYUV2RGB411
	__attribute__((weak, alias("__mlib_VideoColorYUV2RGB411")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	STORE_PIXEL1(ind1, ind2, ind3)                          \
	vis_st_u8_i(db1, dp, ind3);                             \
	db1 = vis_faligndata(db1, db1);                         \
	vis_st_u8_i(dg1, dp, ind2);                             \
	dg1 = vis_faligndata(dg1, dg1);                         \
	vis_st_u8_i(dr1, dp, ind1);                             \
	dr1 = vis_faligndata(dr1, dr1)

/* *********************************************************** */

#define	STORE_PIXEL2(ind1, ind2, ind3)                          \
	vis_st_u8_i(db, dp, ind3);                              \
	db = vis_faligndata(db, db);                            \
	vis_st_u8_i(dg, dp, ind2);                              \
	dg = vis_faligndata(dg, dg);                            \
	vis_st_u8_i(dr, dp, ind1);                              \
	dr = vis_faligndata(dr, dr)

/* *********************************************************** */

static mlib_status mlib_v_VideoColorYUV2RGB411_all_align(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 rgb_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride);

static mlib_status mlib_v_VideoColorYUV2RGB411_nonalign(
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
__mlib_VideoColorYUV2RGB411(
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

	if (y_stride == width && uv_stride == width / 4 &&
		rgb_stride == 3 * width) {

		width *= height;
		height = 1;
	}

	if ((width & 3) != 0)
		return (MLIB_FAILURE);

	if ((((mlib_addr)u & 3) == 0) && (((mlib_addr)v & 3) == 0) &&
		(((mlib_addr)y & 7) == 0) && ((((uv_stride & 3) == 0) &&
		((y_stride & 7) == 0)) || (height == 1))) {

		return mlib_v_VideoColorYUV2RGB411_all_align(rgb, y, u, v,
			width, height, rgb_stride, y_stride, uv_stride);
	} else {
		return mlib_v_VideoColorYUV2RGB411_nonalign(rgb, y, u, v,
			width, height, rgb_stride, y_stride, uv_stride);
	}
}

/* *********************************************************** */

static mlib_status
mlib_v_VideoColorYUV2RGB411_all_align(
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
	mlib_u8 *sp1, *sp2, *sp3, *sl1, *sl2, *sl3;

/* pointers to dst address */
	mlib_u8 *dp, *dl;

/* all. pointer to y */
	mlib_d64 *spy;

/* all. pointers to u, v */
	mlib_f32 *dfu, *dfv;

/* u, v data */
	mlib_f32 fu, fv;

/* y data */
	mlib_d64 dy1, dy2;
	mlib_d64 ddy1, ddy2, ddy3, ddy4;
	mlib_d64 du0, du1;
	mlib_d64 dv1, dv2;
	mlib_d64 dr, dr1, dr2, dr3, dr4;
	mlib_d64 dg, dg1, dg2, dg3, dg4;
	mlib_d64 db, db1, db2, db3, db4;
	mlib_d64 dtmp;

/* 1.1644  * 4096 */
	mlib_f32 f0 = vis_to_float(0x12a1);

/* 2.0184  * 8192 */
	mlib_f32 f1 = vis_to_float(0x4097);

/* -0.3920 * 8192 */
	mlib_f32 f4 = vis_to_float(0xf375);

/* -0.8132 * 8192 */
	mlib_f32 f5 = vis_to_float(0xe5fa);

/* 1.5966  * 8192 */
	mlib_f32 f8 = vis_to_float(0x3317);

/* -276.9856 * 32 */
	mlib_d64 doff0 = vis_to_double_dup(0xdd60dd60);

/* 135.6352  * 32 */
	mlib_d64 doff1 = vis_to_double_dup(0x10f410f4);

/* -222.9952 * 32 */
	mlib_d64 doff2 = vis_to_double_dup(0xe420e420);
	mlib_f32 fscale = vis_to_float(0x80808080);

/* loop variable */
	mlib_s32 i, j;

/*
 * initialize GSR scale factor
 */
	vis_write_gsr((3 << 3) + 7);

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

		dy1 = (*spy++);
		dy2 = vis_ld_d64_nf(spy); spy++;

		du0 = vis_fmul8x16al(fu, f1);
		db = vis_fpadd16(du0, doff0);

		du1 = vis_fmul8x16al(fu, f4);
		dv1 = vis_fmul8x16al(fv, f5);
		dtmp = vis_fpadd16(du1, dv1);
		dg = vis_fpadd16(dtmp, doff1);

		dv2 = vis_fmul8x16al(fv, f8);
		dr = vis_fpadd16(dv2, doff2);

		ddy1 = vis_fmul8x16al(vis_read_hi(dy1), f0);
		ddy2 = vis_fmul8x16al(vis_read_lo(dy1), f0);

		ddy3 = vis_fmul8x16al(vis_read_hi(dy2), f0);
		ddy4 = vis_fmul8x16al(vis_read_lo(dy2), f0);

		db1 = vis_fmul8x16au(fscale, vis_read_hi(db));
		db1 = vis_fpadd16(ddy1, db1);

		db2 = vis_fmul8x16al(fscale, vis_read_hi(db));
		db2 = vis_fpadd16(ddy2, db2);

		db3 = vis_fmul8x16au(fscale, vis_read_lo(db));
		db3 = vis_fpadd16(ddy3, db3);

		db4 = vis_fmul8x16al(fscale, vis_read_lo(db));
		db4 = vis_fpadd16(ddy4, db4);

		dg1 = vis_fmul8x16au(fscale, vis_read_hi(dg));
		dg1 = vis_fpadd16(ddy1, dg1);

		dg2 = vis_fmul8x16al(fscale, vis_read_hi(dg));
		dg2 = vis_fpadd16(ddy2, dg2);

		dg3 = vis_fmul8x16au(fscale, vis_read_lo(dg));
		dg3 = vis_fpadd16(ddy3, dg3);

		dg4 = vis_fmul8x16al(fscale, vis_read_lo(dg));
		dg4 = vis_fpadd16(ddy4, dg4);

		dr1 = vis_fmul8x16au(fscale, vis_read_hi(dr));
		dr1 = vis_fpadd16(ddy1, dr1);

		dr2 = vis_fmul8x16al(fscale, vis_read_hi(dr));
		dr2 = vis_fpadd16(ddy2, dr2);

		dr3 = vis_fmul8x16au(fscale, vis_read_lo(dr));
		dr3 = vis_fpadd16(ddy3, dr3);

		dr4 = vis_fmul8x16al(fscale, vis_read_lo(dr));
		dr4 = vis_fpadd16(ddy4, dr4);

		db = vis_fpack16_pair(db1, db2);
		db1 = vis_fpack16_pair(db3, db4);

		dr = vis_fpack16_pair(dr1, dr2);
		dr1 = vis_fpack16_pair(dr3, dr4);

		dg = vis_fpack16_pair(dg1, dg2);
		dg1 = vis_fpack16_pair(dg3, dg4);

		fu = vis_ld_f32_nf(dfu); dfu++;
		fv = vis_ld_f32_nf(dfv); dfv++;

/*
 * 16-pixel column loop
 */
#pragma pipeloop(0)
		for (i = 0; i <= width - 16; i += 16) {

			STORE_PIXEL1(45, 46, 47);
			STORE_PIXEL1(42, 43, 44);
			STORE_PIXEL1(39, 40, 41);
			STORE_PIXEL1(36, 37, 38);
			STORE_PIXEL1(33, 34, 35);
			STORE_PIXEL1(30, 31, 32);
			STORE_PIXEL1(27, 28, 29);

			vis_st_u8_i(db1, dp, 26);
			vis_st_u8_i(dg1, dp, 25);
			vis_st_u8_i(dr1, dp, 24);

			STORE_PIXEL2(21, 22, 23);
			STORE_PIXEL2(18, 19, 20);
			STORE_PIXEL2(15, 16, 17);
			STORE_PIXEL2(12, 13, 14);
			STORE_PIXEL2(9, 10, 11);
			STORE_PIXEL2(6, 7, 8);
			STORE_PIXEL2(3, 4, 5);

			vis_st_u8_i(db, dp, 2);
			vis_st_u8_i(dg, dp, 1);
			vis_st_u8_i(dr, dp, 0);

			dy1 = vis_ld_d64_nf(spy); spy++;
			dy2 = vis_ld_d64_nf(spy); spy++;

			du0 = vis_fmul8x16al(fu, f1);
			db = vis_fpadd16(du0, doff0);

			du1 = vis_fmul8x16al(fu, f4);
			dv1 = vis_fmul8x16al(fv, f5);
			dtmp = vis_fpadd16(du1, dv1);
			dg = vis_fpadd16(dtmp, doff1);

			dv2 = vis_fmul8x16al(fv, f8);
			dr = vis_fpadd16(dv2, doff2);

			ddy1 = vis_fmul8x16al(vis_read_hi(dy1), f0);
			ddy2 = vis_fmul8x16al(vis_read_lo(dy1), f0);

			ddy3 = vis_fmul8x16al(vis_read_hi(dy2), f0);
			ddy4 = vis_fmul8x16al(vis_read_lo(dy2), f0);

			db1 = vis_fmul8x16au(fscale, vis_read_hi(db));
			db1 = vis_fpadd16(ddy1, db1);

			db2 = vis_fmul8x16al(fscale, vis_read_hi(db));
			db2 = vis_fpadd16(ddy2, db2);

			db3 = vis_fmul8x16au(fscale, vis_read_lo(db));
			db3 = vis_fpadd16(ddy3, db3);

			db4 = vis_fmul8x16al(fscale, vis_read_lo(db));
			db4 = vis_fpadd16(ddy4, db4);

			dg1 = vis_fmul8x16au(fscale, vis_read_hi(dg));
			dg1 = vis_fpadd16(ddy1, dg1);

			dg2 = vis_fmul8x16al(fscale, vis_read_hi(dg));
			dg2 = vis_fpadd16(ddy2, dg2);

			dg3 = vis_fmul8x16au(fscale, vis_read_lo(dg));
			dg3 = vis_fpadd16(ddy3, dg3);

			dg4 = vis_fmul8x16al(fscale, vis_read_lo(dg));
			dg4 = vis_fpadd16(ddy4, dg4);

			dr1 = vis_fmul8x16au(fscale, vis_read_hi(dr));
			dr1 = vis_fpadd16(ddy1, dr1);

			dr2 = vis_fmul8x16al(fscale, vis_read_hi(dr));
			dr2 = vis_fpadd16(ddy2, dr2);

			dr3 = vis_fmul8x16au(fscale, vis_read_lo(dr));
			dr3 = vis_fpadd16(ddy3, dr3);

			dr4 = vis_fmul8x16al(fscale, vis_read_lo(dr));
			dr4 = vis_fpadd16(ddy4, dr4);

			db = vis_fpack16_pair(db1, db2);
			db1 = vis_fpack16_pair(db3, db4);

			dr = vis_fpack16_pair(dr1, dr2);
			dr1 = vis_fpack16_pair(dr3, dr4);

			dg = vis_fpack16_pair(dg1, dg2);
			dg1 = vis_fpack16_pair(dg3, dg4);

			fu = vis_ld_f32_nf(dfu); dfu++;
			fv = vis_ld_f32_nf(dfv); dfv++;

			dp += 48;
		}

		if (i <= width - 8) {

			STORE_PIXEL2(21, 22, 23);
			STORE_PIXEL2(18, 19, 20);
			STORE_PIXEL2(15, 16, 17);
			STORE_PIXEL2(12, 13, 14);
			STORE_PIXEL2(9, 10, 11);
			STORE_PIXEL2(6, 7, 8);
			STORE_PIXEL2(3, 4, 5);

			vis_st_u8_i(db, dp, 2);
			vis_st_u8_i(dg, dp, 1);
			vis_st_u8_i(dr, dp, 0);
			db = db1;
			dr = dr1;
			dg = dg1;
			dp += 24;
			i += 8;
		}

		vis_alignaddr((void *)(width - i), 0);
		db = vis_faligndata(db, db);
		dg = vis_faligndata(dg, dg);
		dr = vis_faligndata(dr, dr);
		dp += ((width - i - 1) * 3);

		vis_alignaddr((void *)7, 0);
		for (; i < width; i++) {
			STORE_PIXEL2(0, 1, 2);
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
mlib_v_VideoColorYUV2RGB411_nonalign(
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
	mlib_u8 *sp1, *sp2, *sp3, *sl1, *sl2, *sl3;

/* pointers to dst address */
	mlib_u8 *dp, *dl;

/* all. pointer to y */
	mlib_d64 *spy;

/* all. pointers to u, v */
	mlib_d64 *dfu, *dfv;

/* u, v data */
	mlib_f32 fu, fv;

/* y data */
	mlib_d64 dy0, dy1, dy2, dy3;
	mlib_d64 ddy1, ddy2, ddy3, ddy4;
	mlib_d64 du0, du1, fu0, fu1;
	mlib_d64 dv1, dv2, fv0, fv1;
	mlib_d64 dr, dr1, dr2, dr3, dr4;
	mlib_d64 dg, dg1, dg2, dg3, dg4;
	mlib_d64 db, db1, db2, db3, db4;
	mlib_d64 dtmp;

/* 1.1644  * 4096 */
	mlib_f32 f0 = vis_to_float(0x12a1);

/* 2.0184  * 8192 */
	mlib_f32 f1 = vis_to_float(0x4097);

/* -0.3920 * 8192 */
	mlib_f32 f4 = vis_to_float(0xf375);

/* -0.8132 * 8192 */
	mlib_f32 f5 = vis_to_float(0xe5fa);

/* 1.5966  * 8192 */
	mlib_f32 f8 = vis_to_float(0x3317);

/* -276.9856 * 32 */
	mlib_d64 doff0 = vis_to_double_dup(0xdd60dd60);

/* 135.6352  * 32 */
	mlib_d64 doff1 = vis_to_double_dup(0x10f410f4);

/* -222.9952 * 32 */
	mlib_d64 doff2 = vis_to_double_dup(0xe420e420);
	mlib_f32 fscale = vis_to_float(0x80808080);

/* loop variable */
	mlib_s32 i, j;

/*
 * initialize GSR scale factor
 */
	vis_write_gsr(3 << 3);

	sp1 = sl1 = (mlib_u8 *)y;
	sp2 = sl2 = (mlib_u8 *)u;
	sp3 = sl3 = (mlib_u8 *)v;

	dl = dp = (mlib_u8 *)rgb;

/*
 * row loop
 */
	for (j = 0; j < height; j++) {
		spy = (mlib_d64 *)vis_alignaddr(sp1, 0);

		dfu = (mlib_d64 *)vis_alignaddr(sp2, 0);
		fu0 = (*dfu++);
		fu1 = vis_ld_d64_nf(dfu); dfu++;
		fu = vis_read_hi(vis_faligndata(fu0, fu1));
		sp2 += 4;

		dfv = (mlib_d64 *)vis_alignaddr(sp3, 0);
		fv0 = (*dfv++);
		fv1 = vis_ld_d64_nf(dfv); dfv++;
		fv = vis_read_hi(vis_faligndata(fv0, fv1));
		sp3 += 4;

		dy0 = (*spy++);
		dy3 = vis_ld_d64_nf(spy); spy++;
		vis_alignaddr(sp1, 0);
		dy1 = vis_faligndata(dy0, dy3);
		dy0 = vis_ld_d64_nf(spy); spy++;
		dy2 = vis_faligndata(dy3, dy0);

		du0 = vis_fmul8x16al(fu, f1);
		db = vis_fpadd16(du0, doff0);

		du1 = vis_fmul8x16al(fu, f4);
		dv1 = vis_fmul8x16al(fv, f5);
		dtmp = vis_fpadd16(du1, dv1);
		dg = vis_fpadd16(dtmp, doff1);

		dv2 = vis_fmul8x16al(fv, f8);
		dr = vis_fpadd16(dv2, doff2);

		ddy1 = vis_fmul8x16al(vis_read_hi(dy1), f0);
		ddy2 = vis_fmul8x16al(vis_read_lo(dy1), f0);

		ddy3 = vis_fmul8x16al(vis_read_hi(dy2), f0);
		ddy4 = vis_fmul8x16al(vis_read_lo(dy2), f0);

		db1 = vis_fmul8x16au(fscale, vis_read_hi(db));
		db1 = vis_fpadd16(ddy1, db1);

		db2 = vis_fmul8x16al(fscale, vis_read_hi(db));
		db2 = vis_fpadd16(ddy2, db2);

		db3 = vis_fmul8x16au(fscale, vis_read_lo(db));
		db3 = vis_fpadd16(ddy3, db3);

		db4 = vis_fmul8x16al(fscale, vis_read_lo(db));
		db4 = vis_fpadd16(ddy4, db4);

		dg1 = vis_fmul8x16au(fscale, vis_read_hi(dg));
		dg1 = vis_fpadd16(ddy1, dg1);

		dg2 = vis_fmul8x16al(fscale, vis_read_hi(dg));
		dg2 = vis_fpadd16(ddy2, dg2);

		dg3 = vis_fmul8x16au(fscale, vis_read_lo(dg));
		dg3 = vis_fpadd16(ddy3, dg3);

		dg4 = vis_fmul8x16al(fscale, vis_read_lo(dg));
		dg4 = vis_fpadd16(ddy4, dg4);

		dr1 = vis_fmul8x16au(fscale, vis_read_hi(dr));
		dr1 = vis_fpadd16(ddy1, dr1);

		dr2 = vis_fmul8x16al(fscale, vis_read_hi(dr));
		dr2 = vis_fpadd16(ddy2, dr2);

		dr3 = vis_fmul8x16au(fscale, vis_read_lo(dr));
		dr3 = vis_fpadd16(ddy3, dr3);

		dr4 = vis_fmul8x16al(fscale, vis_read_lo(dr));
		dr4 = vis_fpadd16(ddy4, dr4);

		db = vis_fpack16_pair(db1, db2);
		db1 = vis_fpack16_pair(db3, db4);

		dr = vis_fpack16_pair(dr1, dr2);
		dr1 = vis_fpack16_pair(dr3, dr4);

		dg = vis_fpack16_pair(dg1, dg2);
		dg1 = vis_fpack16_pair(dg3, dg4);

		dfu = (mlib_d64 *)vis_alignaddr(sp2, 0);
		fu0 = vis_ld_d64_nf(dfu); dfu++;
		fu1 = vis_ld_d64_nf(dfu); dfu++;
		fu = vis_read_hi(vis_faligndata(fu0, fu1));
		sp2 += 4;

		dfv = (mlib_d64 *)vis_alignaddr(sp3, 0);
		fv0 = vis_ld_d64_nf(dfv); dfv++;
		fv1 = vis_ld_d64_nf(dfv); dfv++;
		fv = vis_read_hi(vis_faligndata(fv0, fv1));
		sp3 += 4;

/*
 * 16-pixel column loop
 */
#pragma pipeloop(0)
		for (i = 0; i <= width - 16; i += 16) {

			vis_alignaddr((void *)7, 0);
			STORE_PIXEL1(45, 46, 47);
			STORE_PIXEL1(42, 43, 44);
			STORE_PIXEL1(39, 40, 41);
			STORE_PIXEL1(36, 37, 38);
			STORE_PIXEL1(33, 34, 35);
			STORE_PIXEL1(30, 31, 32);
			STORE_PIXEL1(27, 28, 29);

			vis_st_u8_i(db1, dp, 26);
			vis_st_u8_i(dg1, dp, 25);
			vis_st_u8_i(dr1, dp, 24);

			STORE_PIXEL2(21, 22, 23);
			STORE_PIXEL2(18, 19, 20);
			STORE_PIXEL2(15, 16, 17);
			STORE_PIXEL2(12, 13, 14);
			STORE_PIXEL2(9, 10, 11);
			STORE_PIXEL2(6, 7, 8);
			STORE_PIXEL2(3, 4, 5);

			vis_st_u8_i(db, dp, 2);
			vis_st_u8_i(dg, dp, 1);
			vis_st_u8_i(dr, dp, 0);

			dy3 = vis_ld_d64_nf(spy); spy++;
			vis_alignaddr(sp1, 0);
			dy1 = vis_faligndata(dy0, dy3);
			dy0 = vis_ld_d64_nf(spy); spy++;
			dy2 = vis_faligndata(dy3, dy0);

			du0 = vis_fmul8x16al(fu, f1);
			db = vis_fpadd16(du0, doff0);

			du1 = vis_fmul8x16al(fu, f4);
			dv1 = vis_fmul8x16al(fv, f5);
			dtmp = vis_fpadd16(du1, dv1);
			dg = vis_fpadd16(dtmp, doff1);

			dv2 = vis_fmul8x16al(fv, f8);
			dr = vis_fpadd16(dv2, doff2);

			ddy1 = vis_fmul8x16al(vis_read_hi(dy1), f0);
			ddy2 = vis_fmul8x16al(vis_read_lo(dy1), f0);

			ddy3 = vis_fmul8x16al(vis_read_hi(dy2), f0);
			ddy4 = vis_fmul8x16al(vis_read_lo(dy2), f0);

			db1 = vis_fmul8x16au(fscale, vis_read_hi(db));
			db1 = vis_fpadd16(ddy1, db1);

			db2 = vis_fmul8x16al(fscale, vis_read_hi(db));
			db2 = vis_fpadd16(ddy2, db2);

			db3 = vis_fmul8x16au(fscale, vis_read_lo(db));
			db3 = vis_fpadd16(ddy3, db3);

			db4 = vis_fmul8x16al(fscale, vis_read_lo(db));
			db4 = vis_fpadd16(ddy4, db4);

			dg1 = vis_fmul8x16au(fscale, vis_read_hi(dg));
			dg1 = vis_fpadd16(ddy1, dg1);

			dg2 = vis_fmul8x16al(fscale, vis_read_hi(dg));
			dg2 = vis_fpadd16(ddy2, dg2);

			dg3 = vis_fmul8x16au(fscale, vis_read_lo(dg));
			dg3 = vis_fpadd16(ddy3, dg3);

			dg4 = vis_fmul8x16al(fscale, vis_read_lo(dg));
			dg4 = vis_fpadd16(ddy4, dg4);

			dr1 = vis_fmul8x16au(fscale, vis_read_hi(dr));
			dr1 = vis_fpadd16(ddy1, dr1);

			dr2 = vis_fmul8x16al(fscale, vis_read_hi(dr));
			dr2 = vis_fpadd16(ddy2, dr2);

			dr3 = vis_fmul8x16au(fscale, vis_read_lo(dr));
			dr3 = vis_fpadd16(ddy3, dr3);

			dr4 = vis_fmul8x16al(fscale, vis_read_lo(dr));
			dr4 = vis_fpadd16(ddy4, dr4);

			db = vis_fpack16_pair(db1, db2);
			db1 = vis_fpack16_pair(db3, db4);

			dr = vis_fpack16_pair(dr1, dr2);
			dr1 = vis_fpack16_pair(dr3, dr4);

			dg = vis_fpack16_pair(dg1, dg2);
			dg1 = vis_fpack16_pair(dg3, dg4);

			dfu = (mlib_d64 *)vis_alignaddr(sp2, 0);
			fu0 = vis_ld_d64_nf(dfu); dfu++;
			fu1 = vis_ld_d64_nf(dfu); dfu++;
			fu = vis_read_hi(vis_faligndata(fu0, fu1));
			sp2 += 4;

			dfv = (mlib_d64 *)vis_alignaddr(sp3, 0);
			fv0 = vis_ld_d64_nf(dfv); dfv++;
			fv1 = vis_ld_d64_nf(dfv); dfv++;
			fv = vis_read_hi(vis_faligndata(fv0, fv1));
			sp3 += 4;

			dp += 48;
		}

		if (i <= width - 8) {
			vis_alignaddr((void *)7, 0);
			STORE_PIXEL2(21, 22, 23);
			STORE_PIXEL2(18, 19, 20);
			STORE_PIXEL2(15, 16, 17);
			STORE_PIXEL2(12, 13, 14);
			STORE_PIXEL2(9, 10, 11);
			STORE_PIXEL2(6, 7, 8);
			STORE_PIXEL2(3, 4, 5);

			vis_st_u8_i(db, dp, 2);
			vis_st_u8_i(dg, dp, 1);
			vis_st_u8_i(dr, dp, 0);
			db = db1;
			dr = dr1;
			dg = dg1;
			dp += 24;
			i += 8;
		}

		vis_alignaddr((void *)(width - i), 0);
		db = vis_faligndata(db, db);
		dg = vis_faligndata(dg, dg);
		dr = vis_faligndata(dr, dr);
		dp += ((width - i - 1) * 3);

		vis_alignaddr((void *)7, 0);
		for (; i < width; i++) {
			STORE_PIXEL2(0, 1, 2);
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
