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

#pragma ident	"@(#)mlib_v_VideoColorYUV2ARGB411.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV2ARGB411 - color conversion from YUV411 to ARGB
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorYUV2ARGB411(mlib_u8       *argb,
 *                                             const mlib_u8 *y,
 *                                             const mlib_u8 *u,
 *                                             const mlib_u8 *v,
 *                                             mlib_s32      width,
 *                                             mlib_s32      height,
 *                                             mlib_s32      argb_stride,
 *                                             mlib_s32      y_stride,
 *                                             mlib_s32      uv_stride);
 * ARGUMENT
 *      argb            Pointer to the output ARGB image
 *      y               Pointer to input Y component
 *      u               Pointer to input U component
 *      v               Pointer to input V component
 *      width           Width of the image
 *      height          Height of the image
 *      argb_stride     Stride in bytes between adjacent rows in output image
 *      y_stride        Stride in bytes between adjacent rows in Y input image
 *      uv_stride       Stride in bytes between adjacent rows in U and V input
 *                      image
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

#pragma weak mlib_VideoColorYUV2ARGB411 = __mlib_VideoColorYUV2ARGB411

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorYUV2ARGB411) mlib_VideoColorYUV2ARGB411
	__attribute__((weak, alias("__mlib_VideoColorYUV2ARGB411")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static mlib_status mlib_v_VideoColorYUV2ARGB411_dst_align(
	mlib_u8 *argb,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 argb_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride);

static mlib_status mlib_v_VideoColorYUV2ARGB411_dst_nonalign(
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
__mlib_VideoColorYUV2ARGB411(
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

	if ((width & 3) != 0)
		return (MLIB_FAILURE);

	if ((((mlib_addr)argb & 7) == 0) && (((argb_stride & 7) == 0) ||
		(height == 1))) {

		if (y_stride == width && uv_stride == width / 4 &&
			argb_stride == 4 * width) {

			width *= height;
			height = 1;
		}

		return mlib_v_VideoColorYUV2ARGB411_dst_align(argb, y, u, v,
			width, height, argb_stride, y_stride, uv_stride);
	} else {
		return mlib_v_VideoColorYUV2ARGB411_dst_nonalign(argb, y, u, v,
			width, height, argb_stride, y_stride, uv_stride);
	}
}

/* *********************************************************** */

static mlib_status
mlib_v_VideoColorYUV2ARGB411_dst_align(
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
	mlib_u8 *sp1, *sp2, *sp3, *sl1, *sl2, *sl3;

/* pointers to dst address */
	mlib_u8 *dp, *dl;

/* all. pointer to y */
	mlib_d64 *spy;

/* all. pointer to dst */
	mlib_d64 *dpp;

/* u, v data */
	mlib_f32 fu, fv;

/* y data */
	mlib_d64 dy0, dy1, dy2, dy3;
	mlib_d64 ddy1, ddy2, ddy3, ddy4;
	mlib_d64 du0, du1;
	mlib_d64 dv1, dv2;
	mlib_d64 dr, dr1, dr2, dr3, dr4;
	mlib_d64 dg, dg1, dg2, dg3, dg4;
	mlib_d64 db, db1, db2, db3, db4;
	mlib_d64 dd, dtmp;

/* used to load u, v into mlib_f32 */
	mlib_f32 ffu[1], ffv[1];

/* used to load u, v into mlib_f32 */
	mlib_u8 *ufu, *vfu;

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

/* loop variables */
	mlib_s32 i, j;

/* alpha_ch. is not written */
	mlib_s32 emask = 0x77;

	ufu = (mlib_u8 *)ffu;
	vfu = (mlib_u8 *)ffv;

/*
 * initialize GSR scale factor
 */
	vis_write_gsr(3 << 3);

	sp1 = sl1 = (mlib_u8 *)y;
	sp2 = sl2 = (mlib_u8 *)u;
	sp3 = sl3 = (mlib_u8 *)v;

	dl = dp = (mlib_u8 *)argb;

/*
 * row loop
 */
	for (j = 0; j < height; j++) {
		spy = (mlib_d64 *)vis_alignaddr(sp1, 0);
		dpp = (mlib_d64 *)dp;
		ufu[0] = vis_ld_u8_nf(sp2);
		ufu[1] = vis_ld_u8_nf(sp2 + 1);
		ufu[2] = vis_ld_u8_nf(sp2 + 2);
		ufu[3] = vis_ld_u8_nf(sp2 + 3);
		vfu[0] = vis_ld_u8_nf(sp3);
		vfu[1] = vis_ld_u8_nf(sp3 + 1);
		vfu[2] = vis_ld_u8_nf(sp3 + 2);
		vfu[3] = vis_ld_u8_nf(sp3 + 3);
		sp2 += 4;
		sp3 += 4;

		fu = ffu[0];
		fv = ffv[0];

		dy0 = vis_ld_d64_nf(spy); spy++;

/*
 * 16-pixel column loop
 */
#pragma pipeloop(0)
		for (i = 0; i <= width - 16; i += 16) {

			dy3 = vis_ld_d64_nf(spy); spy++;
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
			ufu[0] = vis_ld_u8_nf(sp2);

			ddy3 = vis_fmul8x16al(vis_read_hi(dy2), f0);
			ddy4 = vis_fmul8x16al(vis_read_lo(dy2), f0);
			ufu[1] = vis_ld_u8_nf(sp2 + 1);

			db1 = vis_fmul8x16au(fscale, vis_read_hi(db));
			db1 = vis_fpadd16(ddy1, db1);
			ufu[2] = vis_ld_u8_nf(sp2 + 2);

			db2 = vis_fmul8x16al(fscale, vis_read_hi(db));
			db2 = vis_fpadd16(ddy2, db2);
			ufu[3] = vis_ld_u8_nf(sp2 + 3);

			db3 = vis_fmul8x16au(fscale, vis_read_lo(db));
			db3 = vis_fpadd16(ddy3, db3);
			vfu[0] = vis_ld_u8_nf(sp3);

			db4 = vis_fmul8x16al(fscale, vis_read_lo(db));
			db4 = vis_fpadd16(ddy4, db4);
			vfu[1] = vis_ld_u8_nf(sp3 + 1);

			dg1 = vis_fmul8x16au(fscale, vis_read_hi(dg));
			dg1 = vis_fpadd16(ddy1, dg1);
			vfu[2] = vis_ld_u8_nf(sp3 + 2);

			dg2 = vis_fmul8x16al(fscale, vis_read_hi(dg));
			dg2 = vis_fpadd16(ddy2, dg2);
			vfu[3] = vis_ld_u8_nf(sp3 + 3);

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

			dr = vis_fpack16_pair(dr1, dr2);
			dr1 = vis_fpack16_pair(dr3, dr4);

			dg = vis_fpack16_pair(dg1, dg2);
			dg1 = vis_fpack16_pair(dg3, dg4);

			db = vis_fpack16_pair(db1, db2);
			db1 = vis_fpack16_pair(db3, db4);

			dg2 = vis_fpmerge(vis_read_hi(db), vis_read_hi(dg));
			dg3 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(db));

			dd = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			vis_pst_8(dd, dpp++, emask);
			dd = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));
			vis_pst_8(dd, dpp++, emask);

			dg2 = vis_fpmerge(vis_read_lo(db), vis_read_lo(dg));
			dg3 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(db));

			dd = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			vis_pst_8(dd, dpp++, emask);
			dd = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));
			vis_pst_8(dd, dpp++, emask);

			dg2 = vis_fpmerge(vis_read_hi(db1), vis_read_hi(dg1));
			dg3 = vis_fpmerge(vis_read_hi(dr1), vis_read_hi(db1));

			dd = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			vis_pst_8(dd, dpp++, emask);
			dd = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));
			vis_pst_8(dd, dpp++, emask);

			dg2 = vis_fpmerge(vis_read_lo(db1), vis_read_lo(dg1));
			dg3 = vis_fpmerge(vis_read_lo(dr1), vis_read_lo(db1));

			dd = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			vis_pst_8(dd, dpp++, emask);
			dd = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));
			vis_pst_8(dd, dpp++, emask);
			fu = ffu[0];
			fv = ffv[0];
			sp2 += 4;
			sp3 += 4;
		}

		if (i <= width - 8) {

			dy3 = vis_ld_d64_nf(spy); spy++;
			dy1 = vis_faligndata(dy0, dy3);
			dy0 = dy3;

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
			ufu[0] = ufu[2];

			db1 = vis_fmul8x16au(fscale, vis_read_hi(db));
			db1 = vis_fpadd16(ddy1, db1);
			vfu[0] = vfu[2];

			db2 = vis_fmul8x16al(fscale, vis_read_hi(db));
			db2 = vis_fpadd16(ddy2, db2);

			dg1 = vis_fmul8x16au(fscale, vis_read_hi(dg));
			dg1 = vis_fpadd16(ddy1, dg1);

			dg2 = vis_fmul8x16al(fscale, vis_read_hi(dg));
			dg2 = vis_fpadd16(ddy2, dg2);

			dr1 = vis_fmul8x16au(fscale, vis_read_hi(dr));
			dr1 = vis_fpadd16(ddy1, dr1);

			dr2 = vis_fmul8x16al(fscale, vis_read_hi(dr));
			dr2 = vis_fpadd16(ddy2, dr2);

			dr = vis_fpack16_pair(dr1, dr2);
			dg = vis_fpack16_pair(dg1, dg2);
			db = vis_fpack16_pair(db1, db2);

			dg2 = vis_fpmerge(vis_read_hi(db), vis_read_hi(dg));
			dg3 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(db));

			dd = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			vis_pst_8(dd, dpp++, emask);
			dd = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));
			vis_pst_8(dd, dpp++, emask);

			dg2 = vis_fpmerge(vis_read_lo(db), vis_read_lo(dg));
			dg3 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(db));

			dd = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			vis_pst_8(dd, dpp++, emask);
			dd = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));
			vis_pst_8(dd, dpp++, emask);

			fu = ffu[0];
			fv = ffv[0];
			i += 8;
		}

		if (i < width) {

			dy3 = vis_ld_d64_nf(spy);
			dy1 = vis_faligndata(dy0, dy3);

			du0 = vis_fmul8x16al(fu, f1);
			db = vis_fpadd16(du0, doff0);

			du1 = vis_fmul8x16al(fu, f4);
			dv1 = vis_fmul8x16al(fv, f5);
			dtmp = vis_fpadd16(du1, dv1);
			dg = vis_fpadd16(dtmp, doff1);

			dv2 = vis_fmul8x16al(fv, f8);
			dr = vis_fpadd16(dv2, doff2);

			ddy1 = vis_fmul8x16al(vis_read_hi(dy1), f0);

			db1 = vis_fmul8x16au(fscale, vis_read_hi(db));
			db1 = vis_fpadd16(ddy1, db1);

			dg1 = vis_fmul8x16au(fscale, vis_read_hi(dg));
			dg1 = vis_fpadd16(ddy1, dg1);

			dr1 = vis_fmul8x16au(fscale, vis_read_hi(dr));
			dr1 = vis_fpadd16(ddy1, dr1);

			fu = vis_fpack16(dr1);

			dg2 = vis_fpmerge(fu, vis_fpack16(dg1));
			dg3 = vis_fpmerge(fu, vis_fpack16(db1));

			dd = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			vis_pst_8(dd, dpp++, emask);
			dd = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));
			vis_pst_8(dd, dpp, emask);
		}

		sp1 = sl1 = sl1 + y_stride;
		sp2 = sl2 = sl2 + uv_stride;
		sp3 = sl3 = sl3 + uv_stride;

		dl = dp = dl + argb_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

static mlib_status
mlib_v_VideoColorYUV2ARGB411_dst_nonalign(
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
	mlib_u8 *sp1, *sp2, *sp3, *sl1, *sl2, *sl3;

/* pointers to dst address */
	mlib_u8 *dp, *dl, *dend;

/* all. pointer to y */
	mlib_d64 *spy;

/* all. pointer to dst */
	mlib_d64 *dpp;

/* u, v data */
	mlib_f32 fu, fv;

/* y data */
	mlib_d64 dy0, dy1, dy2;
	mlib_d64 ddy1, ddy2, ddy3, ddy4;
	mlib_d64 du0, du1;
	mlib_d64 dv1, dv2;
	mlib_d64 dr, dr1, dr2, dr3, dr4;
	mlib_d64 dg, dg1, dg2, dg3, dg4;
	mlib_d64 db, db1, db2, db3, db4;
	mlib_d64 dd, dd0, dd1, dtmp;

/* used to load u, v into mlib_f32 */
	mlib_f32 ffu[1], ffv[1];

/* used to load u, v into mlib_f32 */
	mlib_u8 *ufu, *vfu;

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

/* loop variables */
	mlib_s32 i, j;

/* alpha_ch. is not written */
	mlib_s32 emask = 0x7777;
	mlib_s32 emask1;
	mlib_d64 *buf;
	mlib_s32 inc;

	ufu = (mlib_u8 *)ffu;
	vfu = (mlib_u8 *)ffv;

/*
 * initialize GSR scale factor
 */
	vis_write_gsr(3 << 3);

	buf = (mlib_d64 *)__mlib_malloc((width / 8 + 1) * sizeof (mlib_d64));

	if (buf == NULL)
		return (MLIB_FAILURE);

	sp1 = sl1 = (mlib_u8 *)y;
	sp2 = sl2 = (mlib_u8 *)u;
	sp3 = sl3 = (mlib_u8 *)v;

	dl = dp = (mlib_u8 *)argb;

/*
 * row loop
 */
	for (j = 0; j < height; j++) {
		spy = (mlib_d64 *)vis_alignaddr(sp1, 0);
		dpp = buf;
		dy0 = vis_ld_d64_nf(spy); spy++;

#pragma pipeloop(0)
		for (i = 0; i < width; i += 8) {
			dy1 = vis_ld_d64_nf(spy); spy++;
			(*dpp++) = vis_faligndata(dy0, dy1);
			dy0 = dy1;
		}

		spy = buf;

		dend = dp + width * 4 - 1;
		emask1 = vis_edge8(dp, dend);

		dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
		i = dp - (mlib_u8 *)dpp;
		emask >>= i;
		vis_alignaddr((void *)(8 - i), 0);
		inc = (emask1 != 0xff);
		emask1 &= emask;

		ufu[0] = vis_ld_u8_nf(sp2);
		ufu[1] = vis_ld_u8_nf(sp2 + 1);
		ufu[2] = vis_ld_u8_nf(sp2 + 2);
		ufu[3] = vis_ld_u8_nf(sp2 + 3);
		vfu[0] = vis_ld_u8_nf(sp3);
		vfu[1] = vis_ld_u8_nf(sp3 + 1);
		vfu[2] = vis_ld_u8_nf(sp3 + 2);
		vfu[3] = vis_ld_u8_nf(sp3 + 3);
		sp2 += 4;
		sp3 += 4;

		fu = ffu[0];
		fv = ffv[0];

/*
 * 16-pixel column loop
 */
#pragma pipeloop(0)
		for (i = 0; i <= width - 16; i += 16) {

			dy1 = (*spy++);
			dy2 = (*spy++);

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
			ufu[0] = vis_ld_u8_nf(sp2);

			ddy3 = vis_fmul8x16al(vis_read_hi(dy2), f0);
			ddy4 = vis_fmul8x16al(vis_read_lo(dy2), f0);
			ufu[1] = vis_ld_u8_nf(sp2 + 1);

			db1 = vis_fmul8x16au(fscale, vis_read_hi(db));
			db1 = vis_fpadd16(ddy1, db1);
			ufu[2] = vis_ld_u8_nf(sp2 + 2);

			db2 = vis_fmul8x16al(fscale, vis_read_hi(db));
			db2 = vis_fpadd16(ddy2, db2);
			ufu[3] = vis_ld_u8_nf(sp2 + 3);

			db3 = vis_fmul8x16au(fscale, vis_read_lo(db));
			db3 = vis_fpadd16(ddy3, db3);
			vfu[0] = vis_ld_u8_nf(sp3);

			db4 = vis_fmul8x16al(fscale, vis_read_lo(db));
			db4 = vis_fpadd16(ddy4, db4);
			vfu[1] = vis_ld_u8_nf(sp3 + 1);

			dg1 = vis_fmul8x16au(fscale, vis_read_hi(dg));
			dg1 = vis_fpadd16(ddy1, dg1);
			vfu[2] = vis_ld_u8_nf(sp3 + 2);

			dg2 = vis_fmul8x16al(fscale, vis_read_hi(dg));
			dg2 = vis_fpadd16(ddy2, dg2);
			vfu[3] = vis_ld_u8_nf(sp3 + 3);

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

			dr = vis_fpack16_pair(dr1, dr2);
			dr1 = vis_fpack16_pair(dr3, dr4);

			dg = vis_fpack16_pair(dg1, dg2);
			dg1 = vis_fpack16_pair(dg3, dg4);

			db = vis_fpack16_pair(db1, db2);
			db1 = vis_fpack16_pair(db3, db4);

			dg2 = vis_fpmerge(vis_read_hi(db), vis_read_hi(dg));
			dg3 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(db));

			dd1 = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			dd = vis_faligndata(dd0, dd1);
			vis_pst_8(dd, dpp, emask1);
			dpp += inc;
			inc = 1;

			dd0 = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));
			dd = vis_faligndata(dd1, dd0);
			vis_pst_8(dd, dpp++, emask);

			dg2 = vis_fpmerge(vis_read_lo(db), vis_read_lo(dg));
			dg3 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(db));

			dd1 = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			dd = vis_faligndata(dd0, dd1);
			vis_pst_8(dd, dpp++, emask);
			dd0 = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));
			dd = vis_faligndata(dd1, dd0);
			vis_pst_8(dd, dpp++, emask);

			dg2 = vis_fpmerge(vis_read_hi(db1), vis_read_hi(dg1));
			dg3 = vis_fpmerge(vis_read_hi(dr1), vis_read_hi(db1));

			dd1 = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			dd = vis_faligndata(dd0, dd1);
			vis_pst_8(dd, dpp++, emask);
			dd0 = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));
			dd = vis_faligndata(dd1, dd0);
			vis_pst_8(dd, dpp++, emask);

			dg2 = vis_fpmerge(vis_read_lo(db1), vis_read_lo(dg1));
			dg3 = vis_fpmerge(vis_read_lo(dr1), vis_read_lo(db1));

			dd1 = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			dd = vis_faligndata(dd0, dd1);
			vis_pst_8(dd, dpp++, emask);
			dd0 = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));
			dd = vis_faligndata(dd1, dd0);
			vis_pst_8(dd, dpp++, emask);
			fu = ffu[0];
			fv = ffv[0];
			sp2 += 4;
			sp3 += 4;
			emask1 = emask;
		}

		if (i <= width - 8) {

			dy1 = (*spy++);

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
			ufu[0] = ufu[2];

			db1 = vis_fmul8x16au(fscale, vis_read_hi(db));
			db1 = vis_fpadd16(ddy1, db1);
			vfu[0] = vfu[2];

			db2 = vis_fmul8x16al(fscale, vis_read_hi(db));
			db2 = vis_fpadd16(ddy2, db2);

			dg1 = vis_fmul8x16au(fscale, vis_read_hi(dg));
			dg1 = vis_fpadd16(ddy1, dg1);

			dg2 = vis_fmul8x16al(fscale, vis_read_hi(dg));
			dg2 = vis_fpadd16(ddy2, dg2);

			dr1 = vis_fmul8x16au(fscale, vis_read_hi(dr));
			dr1 = vis_fpadd16(ddy1, dr1);

			dr2 = vis_fmul8x16al(fscale, vis_read_hi(dr));
			dr2 = vis_fpadd16(ddy2, dr2);

			dr = vis_fpack16_pair(dr1, dr2);
			dg = vis_fpack16_pair(dg1, dg2);
			db = vis_fpack16_pair(db1, db2);

			dg2 = vis_fpmerge(vis_read_hi(db), vis_read_hi(dg));
			dg3 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(db));

			dd1 = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			dd = vis_faligndata(dd0, dd1);
			vis_pst_8(dd, dpp, emask1);
			dpp += inc;
			inc = 1;

			dd0 = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));
			dd = vis_faligndata(dd1, dd0);
			vis_pst_8(dd, dpp++, emask);

			dg2 = vis_fpmerge(vis_read_lo(db), vis_read_lo(dg));
			dg3 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(db));

			dd1 = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			dd = vis_faligndata(dd0, dd1);
			vis_pst_8(dd, dpp++, emask);
			dd0 = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));
			dd = vis_faligndata(dd1, dd0);
			vis_pst_8(dd, dpp++, emask);

			fu = ffu[0];
			fv = ffv[0];

			i += 8;
			emask1 = emask;
		}

		if (i < width) {

			dy1 = vis_ld_d64_nf(spy);

			du0 = vis_fmul8x16al(fu, f1);
			db = vis_fpadd16(du0, doff0);

			du1 = vis_fmul8x16al(fu, f4);
			dv1 = vis_fmul8x16al(fv, f5);
			dtmp = vis_fpadd16(du1, dv1);
			dg = vis_fpadd16(dtmp, doff1);

			dv2 = vis_fmul8x16al(fv, f8);
			dr = vis_fpadd16(dv2, doff2);

			ddy1 = vis_fmul8x16al(vis_read_hi(dy1), f0);

			db1 = vis_fmul8x16au(fscale, vis_read_hi(db));
			db1 = vis_fpadd16(ddy1, db1);

			dg1 = vis_fmul8x16au(fscale, vis_read_hi(dg));
			dg1 = vis_fpadd16(ddy1, dg1);

			dr1 = vis_fmul8x16au(fscale, vis_read_hi(dr));
			dr1 = vis_fpadd16(ddy1, dr1);

			fu = vis_fpack16(dr1);

			dg2 = vis_fpmerge(fu, vis_fpack16(dg1));
			dg3 = vis_fpmerge(fu, vis_fpack16(db1));

			dd1 = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			dd = vis_faligndata(dd0, dd1);
			vis_pst_8(dd, dpp, emask1);
			dpp += inc;

			dd0 = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));
			dd = vis_faligndata(dd1, dd0);
			vis_pst_8(dd, dpp++, emask);
		}

		emask1 = vis_edge8(dpp, dend);
		emask1 &= emask;
		dd = vis_faligndata(dd0, dd1);
		vis_pst_8(dd, dpp, emask1);

		sp1 = sl1 = sl1 + y_stride;
		sp2 = sl2 = sl2 + uv_stride;
		sp3 = sl3 = sl3 + uv_stride;

		dl = dp = dl + argb_stride;
		emask = 0x7777;
	}

	__mlib_free(buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
