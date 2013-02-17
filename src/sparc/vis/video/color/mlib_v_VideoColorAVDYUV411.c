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

#pragma ident	"@(#)mlib_v_VideoColorAVDYUV411.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV411seq_to_ARGBint - color convert YUV sequential
 *                                            to ARGB interleaved
 *
 * SYNOPSIS
 *
 *      void mlib_VideoColorYUV411seq_to_ARGBint(mlib_u32      *argb,
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
 *
 * ARGUMENTS
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
 *      width/4 and height.
 *      Dimension width is assumed to be a multiple of 4. In each row, every
 *      4 Y values use the same U and V values.
 *      The alpha values for this function work in the following fashion:
 *      - if a pointer is not NULL the values are taken from there.
 *      It has to have the same dimensions as the Y buffer.
 *      - if a pointer is NULL, the alpha values for every pixel are
 *      set to alpha.
 */

/*
 * FUNCTION
 *      mlib_VideoColorYUV411seq_to_ABGRint - color convert YUV sequential
 *                                            to ABGR interleaved
 *
 * SYNOPSIS
 *
 *      void mlib_VideoColorYUV411seq_to_ABGRint(mlib_u32      *abgr,
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
 *      width/4 and height.
 *      Dimension width is assumed to be a multiple of 4. In each row, every
 *      4 Y values use the same U and V values.
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

#pragma weak mlib_VideoColorYUV411seq_to_ABGRint = \
	__mlib_VideoColorYUV411seq_to_ABGRint
#pragma weak mlib_VideoColorYUV411seq_to_ARGBint = \
	__mlib_VideoColorYUV411seq_to_ARGBint

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorYUV411seq_to_ABGRint)
	mlib_VideoColorYUV411seq_to_ABGRint
	__attribute__((weak, alias("__mlib_VideoColorYUV411seq_to_ABGRint")));
__typeof__(__mlib_VideoColorYUV411seq_to_ARGBint)
	mlib_VideoColorYUV411seq_to_ARGBint
	__attribute__((weak, alias("__mlib_VideoColorYUV411seq_to_ARGBint")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	BUF_SIZE	2048

/* *********************************************************** */

static void
mlib_v_VideoYUV2ABGR_alpha_411(
	mlib_u32 *abgr,
	const mlib_d64 *y,
	const mlib_f32 *u,
	const mlib_f32 *v,
	mlib_d64 a_const,
	mlib_s32 count,
	mlib_s32 left,
	mlib_s32 isrgb)
{
/* all. pointer to dst */
	mlib_d64 *dpp = (mlib_d64 *)abgr;

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
	mlib_d64 dd, dtmp;

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
	mlib_s32 i;

	if (isrgb) {
		f0 = vis_to_float(0x12a1);
		f1 = vis_to_float(0x3317);
		f4 = vis_to_float(0xe5fa);
		f5 = vis_to_float(0xf375);
		f8 = vis_to_float(0x4097);
		doff0 = vis_to_double_dup(0xe420e420);
		doff1 = vis_to_double_dup(0x10f410f4);
		doff2 = vis_to_double_dup(0xdd60dd60);
	}

	dy1 = (*y++);
	dy2 = vis_ld_d64_nf((mlib_d64 *)y); y++;
	fu = (*u++);
	fv = (*v++);

	du0 = vis_fmul8x16al(fu, f1);
	du1 = vis_fmul8x16al(fu, f4);
	dv1 = vis_fmul8x16al(fv, f5);
	dv2 = vis_fmul8x16al(fv, f8);

	if ((mlib_addr)abgr & 7) {
#pragma pipeloop(0)
		for (i = 0; i < count; i++) {
			ddy1 = vis_fmul8x16al(vis_read_hi(dy1), f0);
			ddy2 = vis_fmul8x16al(vis_read_lo(dy1), f0);

			ddy3 = vis_fmul8x16al(vis_read_hi(dy2), f0);
			ddy4 = vis_fmul8x16al(vis_read_lo(dy2), f0);

			db = vis_fpadd16(du0, doff0);

			dtmp = vis_fpadd16(du1, dv1);
			dg = vis_fpadd16(dtmp, doff1);

			dr = vis_fpadd16(dv2, doff2);

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

			dr = vis_fpack16_pair(dr1, dr2);
			dr1 = vis_fpack16_pair(dr3, dr4);

			dg = vis_fpack16_pair(dg1, dg2);
			dg1 = vis_fpack16_pair(dg3, dg4);

			db = vis_fpack16_pair(db1, db2);
			db1 = vis_fpack16_pair(db3, db4);

			dg2 = vis_fpmerge(vis_read_hi(a_const),
				vis_read_hi(dg));
			dg3 = vis_fpmerge(vis_read_hi(db), vis_read_hi(dr));

			dy1 = vis_ld_d64_nf((mlib_d64 *)y + 2 * i);
			dy2 = vis_ld_d64_nf((mlib_d64 *)y + 2 * i + 1);
			fu = vis_ld_f32_nf((mlib_f32 *)u + i);
			fv = vis_ld_f32_nf((mlib_f32 *)v + i);

			dd = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			((mlib_f32 *)dpp)[16 * i] = vis_read_hi(dd);
			((mlib_f32 *)dpp)[16 * i + 1] = vis_read_lo(dd);
			dd = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));
			((mlib_f32 *)dpp)[16 * i + 2] = vis_read_hi(dd);
			((mlib_f32 *)dpp)[16 * i + 3] = vis_read_lo(dd);

			dg2 = vis_fpmerge(vis_read_lo(a_const),
				vis_read_lo(dg));
			dg3 = vis_fpmerge(vis_read_lo(db), vis_read_lo(dr));

			dd = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			((mlib_f32 *)dpp)[16 * i + 4] = vis_read_hi(dd);
			((mlib_f32 *)dpp)[16 * i + 5] = vis_read_lo(dd);
			dd = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));
			((mlib_f32 *)dpp)[16 * i + 6] = vis_read_hi(dd);
			((mlib_f32 *)dpp)[16 * i + 7] = vis_read_lo(dd);

			dg2 = vis_fpmerge(vis_read_hi(a_const),
				vis_read_hi(dg1));
			dg3 = vis_fpmerge(vis_read_hi(db1), vis_read_hi(dr1));

			dd = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			((mlib_f32 *)dpp)[16 * i + 8] = vis_read_hi(dd);
			((mlib_f32 *)dpp)[16 * i + 9] = vis_read_lo(dd);
			dd = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));
			((mlib_f32 *)dpp)[16 * i + 10] = vis_read_hi(dd);
			((mlib_f32 *)dpp)[16 * i + 11] = vis_read_lo(dd);

			dg2 = vis_fpmerge(vis_read_lo(a_const),
				vis_read_lo(dg1));
			dg3 = vis_fpmerge(vis_read_lo(db1), vis_read_lo(dr1));

			dd = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			((mlib_f32 *)dpp)[16 * i + 12] = vis_read_hi(dd);
			((mlib_f32 *)dpp)[16 * i + 13] = vis_read_lo(dd);
			dd = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));
			((mlib_f32 *)dpp)[16 * i + 14] = vis_read_hi(dd);
			((mlib_f32 *)dpp)[16 * i + 15] = vis_read_lo(dd);

			du0 = vis_fmul8x16al(fu, f1);
			du1 = vis_fmul8x16al(fu, f4);
			dv1 = vis_fmul8x16al(fv, f5);
			dv2 = vis_fmul8x16al(fv, f8);
		}
	} else {
#pragma pipeloop(0)
		for (i = 0; i < count; i++) {
			ddy1 = vis_fmul8x16al(vis_read_hi(dy1), f0);
			ddy2 = vis_fmul8x16al(vis_read_lo(dy1), f0);

			ddy3 = vis_fmul8x16al(vis_read_hi(dy2), f0);
			ddy4 = vis_fmul8x16al(vis_read_lo(dy2), f0);

			db = vis_fpadd16(du0, doff0);

			dtmp = vis_fpadd16(du1, dv1);
			dg = vis_fpadd16(dtmp, doff1);

			dr = vis_fpadd16(dv2, doff2);

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

			dr = vis_fpack16_pair(dr1, dr2);
			dr1 = vis_fpack16_pair(dr3, dr4);

			dg = vis_fpack16_pair(dg1, dg2);
			dg1 = vis_fpack16_pair(dg3, dg4);

			db = vis_fpack16_pair(db1, db2);
			db1 = vis_fpack16_pair(db3, db4);

			dg2 = vis_fpmerge(vis_read_hi(a_const),
				vis_read_hi(dg));
			dg3 = vis_fpmerge(vis_read_hi(db), vis_read_hi(dr));

			dy1 = vis_ld_d64_nf((mlib_d64 *)y + 2 * i);
			dy2 = vis_ld_d64_nf((mlib_d64 *)y + 2 * i + 1);
			fu = vis_ld_f32_nf((mlib_f32 *)u + i);
			fv = vis_ld_f32_nf((mlib_f32 *)v + i);

			dpp[8 * i] =
				vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			dpp[8 * i + 1] =
				vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));

			dg2 = vis_fpmerge(vis_read_lo(a_const),
				vis_read_lo(dg));
			dg3 = vis_fpmerge(vis_read_lo(db), vis_read_lo(dr));

			dpp[8 * i + 2] =
				vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			dpp[8 * i + 3] =
				vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));

			dg2 = vis_fpmerge(vis_read_hi(a_const),
				vis_read_hi(dg1));
			dg3 = vis_fpmerge(vis_read_hi(db1), vis_read_hi(dr1));

			dpp[8 * i + 4] =
				vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			dpp[8 * i + 5] =
				vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));

			dg2 = vis_fpmerge(vis_read_lo(a_const),
				vis_read_lo(dg1));
			dg3 = vis_fpmerge(vis_read_lo(db1), vis_read_lo(dr1));

			dpp[8 * i + 6] =
				vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			dpp[8 * i + 7] =
				vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));

			du0 = vis_fmul8x16al(fu, f1);
			du1 = vis_fmul8x16al(fu, f4);
			dv1 = vis_fmul8x16al(fv, f5);
			dv2 = vis_fmul8x16al(fv, f8);
		}
	}

	if (left) {
		mlib_d64 res_buf[8];

		ddy1 = vis_fmul8x16al(vis_read_hi(dy1), f0);
		ddy2 = vis_fmul8x16al(vis_read_lo(dy1), f0);

		ddy3 = vis_fmul8x16al(vis_read_hi(dy2), f0);
		ddy4 = vis_fmul8x16al(vis_read_lo(dy2), f0);

		db = vis_fpadd16(du0, doff0);

		dtmp = vis_fpadd16(du1, dv1);
		dg = vis_fpadd16(dtmp, doff1);

		dr = vis_fpadd16(dv2, doff2);

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

		dr = vis_fpack16_pair(dr1, dr2);
		dr1 = vis_fpack16_pair(dr3, dr4);

		dg = vis_fpack16_pair(dg1, dg2);
		dg1 = vis_fpack16_pair(dg3, dg4);

		db = vis_fpack16_pair(db1, db2);
		db1 = vis_fpack16_pair(db3, db4);

		dg2 = vis_fpmerge(vis_read_hi(a_const), vis_read_hi(dg));
		dg3 = vis_fpmerge(vis_read_hi(db), vis_read_hi(dr));

		res_buf[0] = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
		res_buf[1] = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));

		dg2 = vis_fpmerge(vis_read_lo(a_const), vis_read_lo(dg));
		dg3 = vis_fpmerge(vis_read_lo(db), vis_read_lo(dr));

		res_buf[2] = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
		res_buf[3] = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));

		dg2 = vis_fpmerge(vis_read_hi(a_const), vis_read_hi(dg1));
		dg3 = vis_fpmerge(vis_read_hi(db1), vis_read_hi(dr1));

		res_buf[4] = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
		res_buf[5] = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));

		dg2 = vis_fpmerge(vis_read_lo(a_const), vis_read_lo(dg1));
		dg3 = vis_fpmerge(vis_read_lo(db1), vis_read_lo(dr1));

		res_buf[6] = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
		res_buf[7] = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));

		for (i = 0; i < left; i++)
			((mlib_f32 *)dpp)[16 * count + i] =
				((mlib_f32 *)res_buf)[i];
	}
}

/* *********************************************************** */

static void
mlib_v_VideoYUV2ABGR_aarray_411(
	mlib_u32 *abgr,
	const mlib_d64 *y,
	const mlib_f32 *u,
	const mlib_f32 *v,
	const mlib_d64 *a_array,
	mlib_s32 count,
	mlib_s32 left,
	mlib_s32 isrgb)
{
/* all. pointer to dst */
	mlib_d64 *dpp = (mlib_d64 *)abgr;

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
	mlib_d64 *dpa, da0, da1, da2, da3, da4;
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

/* loop variables */
	mlib_s32 i;

	if (isrgb) {
		f0 = vis_to_float(0x12a1);
		f1 = vis_to_float(0x3317);
		f4 = vis_to_float(0xe5fa);
		f5 = vis_to_float(0xf375);
		f8 = vis_to_float(0x4097);
		doff0 = vis_to_double_dup(0xe420e420);
		doff1 = vis_to_double_dup(0x10f410f4);
		doff2 = vis_to_double_dup(0xdd60dd60);
	}

	dpa = vis_alignaddr((void *)a_array, 0);

	dy1 = (*y++);
	dy2 = vis_ld_d64_nf((mlib_d64 *)y); y++;
	fu = (*u++);
	fv = (*v++);
	da2 = (*dpa++);
	da3 = vis_ld_d64_nf(dpa); dpa++;
	da4 = vis_ld_d64_nf(dpa); dpa++;

	du0 = vis_fmul8x16al(fu, f1);
	du1 = vis_fmul8x16al(fu, f4);
	dv1 = vis_fmul8x16al(fv, f5);
	dv2 = vis_fmul8x16al(fv, f8);

	if (!((mlib_addr)abgr & 7)) {
#pragma pipeloop(0)
		for (i = 0; i < count; i++) {
			da0 = vis_faligndata(da2, da3);
			da1 = vis_faligndata(da3, da4);

			ddy1 = vis_fmul8x16al(vis_read_hi(dy1), f0);
			ddy2 = vis_fmul8x16al(vis_read_lo(dy1), f0);

			ddy3 = vis_fmul8x16al(vis_read_hi(dy2), f0);
			ddy4 = vis_fmul8x16al(vis_read_lo(dy2), f0);

			db = vis_fpadd16(du0, doff0);

			dtmp = vis_fpadd16(du1, dv1);
			dg = vis_fpadd16(dtmp, doff1);

			dr = vis_fpadd16(dv2, doff2);

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

			dr = vis_fpack16_pair(dr1, dr2);
			dr1 = vis_fpack16_pair(dr3, dr4);

			dg = vis_fpack16_pair(dg1, dg2);
			dg1 = vis_fpack16_pair(dg3, dg4);

			db = vis_fpack16_pair(db1, db2);
			db1 = vis_fpack16_pair(db3, db4);

			dg2 = vis_fpmerge(vis_read_hi(da0), vis_read_hi(dg));
			dg3 = vis_fpmerge(vis_read_hi(db), vis_read_hi(dr));

			dy1 = vis_ld_d64_nf((mlib_d64 *)y + 2 * i);
			dy2 = vis_ld_d64_nf((mlib_d64 *)y + 2 * i + 1);
			fu = vis_ld_f32_nf((mlib_f32 *)u + i);
			fv = vis_ld_f32_nf((mlib_f32 *)v + i);
			da2 = da4;
			da3 = vis_ld_d64_nf(dpa + 2 * i);
			da4 = vis_ld_d64_nf(dpa + 2 * i + 1);

			dpp[8 * i] =
				vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			dpp[8 * i + 1] =
				vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));

			dg2 = vis_fpmerge(vis_read_lo(da0), vis_read_lo(dg));
			dg3 = vis_fpmerge(vis_read_lo(db), vis_read_lo(dr));

			dpp[8 * i + 2] =
				vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			dpp[8 * i + 3] =
				vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));

			dg2 = vis_fpmerge(vis_read_hi(da1), vis_read_hi(dg1));
			dg3 = vis_fpmerge(vis_read_hi(db1), vis_read_hi(dr1));

			dpp[8 * i + 4] =
				vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			dpp[8 * i + 5] =
				vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));

			dg2 = vis_fpmerge(vis_read_lo(da1), vis_read_lo(dg1));
			dg3 = vis_fpmerge(vis_read_lo(db1), vis_read_lo(dr1));

			dpp[8 * i + 6] =
				vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			dpp[8 * i + 7] =
				vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));

			du0 = vis_fmul8x16al(fu, f1);
			du1 = vis_fmul8x16al(fu, f4);
			dv1 = vis_fmul8x16al(fv, f5);
			dv2 = vis_fmul8x16al(fv, f8);
		}
	} else {
		mlib_d64 dd;

#pragma pipeloop(0)
		for (i = 0; i < count; i++) {
			da0 = vis_faligndata(da2, da3);
			da1 = vis_faligndata(da3, da4);

			ddy1 = vis_fmul8x16al(vis_read_hi(dy1), f0);
			ddy2 = vis_fmul8x16al(vis_read_lo(dy1), f0);

			ddy3 = vis_fmul8x16al(vis_read_hi(dy2), f0);
			ddy4 = vis_fmul8x16al(vis_read_lo(dy2), f0);

			db = vis_fpadd16(du0, doff0);

			dtmp = vis_fpadd16(du1, dv1);
			dg = vis_fpadd16(dtmp, doff1);

			dr = vis_fpadd16(dv2, doff2);

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

			dr = vis_fpack16_pair(dr1, dr2);
			dr1 = vis_fpack16_pair(dr3, dr4);

			dg = vis_fpack16_pair(dg1, dg2);
			dg1 = vis_fpack16_pair(dg3, dg4);

			db = vis_fpack16_pair(db1, db2);
			db1 = vis_fpack16_pair(db3, db4);

			dg2 = vis_fpmerge(vis_read_hi(da0), vis_read_hi(dg));
			dg3 = vis_fpmerge(vis_read_hi(db), vis_read_hi(dr));

			dy1 = vis_ld_d64_nf((mlib_d64 *)y + 2 * i);
			dy2 = vis_ld_d64_nf((mlib_d64 *)y + 2 * i + 1);
			fu = vis_ld_f32_nf((mlib_f32 *)u + i);
			fv = vis_ld_f32_nf((mlib_f32 *)v + i);
			da2 = da4;
			da3 = vis_ld_d64_nf(dpa + 2 * i);
			da4 = vis_ld_d64_nf(dpa + 2 * i + 1);

			dd = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			((mlib_f32 *)dpp)[16 * i] = vis_read_hi(dd);
			((mlib_f32 *)dpp)[16 * i + 1] = vis_read_lo(dd);
			dd = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));
			((mlib_f32 *)dpp)[16 * i + 2] = vis_read_hi(dd);
			((mlib_f32 *)dpp)[16 * i + 3] = vis_read_lo(dd);

			dg2 = vis_fpmerge(vis_read_lo(da0), vis_read_lo(dg));
			dg3 = vis_fpmerge(vis_read_lo(db), vis_read_lo(dr));

			dd = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			((mlib_f32 *)dpp)[16 * i + 4] = vis_read_hi(dd);
			((mlib_f32 *)dpp)[16 * i + 5] = vis_read_lo(dd);
			dd = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));
			((mlib_f32 *)dpp)[16 * i + 6] = vis_read_hi(dd);
			((mlib_f32 *)dpp)[16 * i + 7] = vis_read_lo(dd);

			dg2 = vis_fpmerge(vis_read_hi(da1), vis_read_hi(dg1));
			dg3 = vis_fpmerge(vis_read_hi(db1), vis_read_hi(dr1));

			dd = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			((mlib_f32 *)dpp)[16 * i + 8] = vis_read_hi(dd);
			((mlib_f32 *)dpp)[16 * i + 9] = vis_read_lo(dd);
			dd = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));
			((mlib_f32 *)dpp)[16 * i + 10] = vis_read_hi(dd);
			((mlib_f32 *)dpp)[16 * i + 11] = vis_read_lo(dd);

			dg2 = vis_fpmerge(vis_read_lo(da1), vis_read_lo(dg1));
			dg3 = vis_fpmerge(vis_read_lo(db1), vis_read_lo(dr1));

			dd = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
			((mlib_f32 *)dpp)[16 * i + 12] = vis_read_hi(dd);
			((mlib_f32 *)dpp)[16 * i + 13] = vis_read_lo(dd);
			dd = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));
			((mlib_f32 *)dpp)[16 * i + 14] = vis_read_hi(dd);
			((mlib_f32 *)dpp)[16 * i + 15] = vis_read_lo(dd);

			du0 = vis_fmul8x16al(fu, f1);
			du1 = vis_fmul8x16al(fu, f4);
			dv1 = vis_fmul8x16al(fv, f5);
			dv2 = vis_fmul8x16al(fv, f8);
		}
	}

	if (left) {
		mlib_d64 res_buf[8];

		da0 = vis_faligndata(da2, da3);
		da1 = vis_faligndata(da3, da4);

		ddy1 = vis_fmul8x16al(vis_read_hi(dy1), f0);
		ddy2 = vis_fmul8x16al(vis_read_lo(dy1), f0);

		ddy3 = vis_fmul8x16al(vis_read_hi(dy2), f0);
		ddy4 = vis_fmul8x16al(vis_read_lo(dy2), f0);

		db = vis_fpadd16(du0, doff0);

		dtmp = vis_fpadd16(du1, dv1);
		dg = vis_fpadd16(dtmp, doff1);

		dr = vis_fpadd16(dv2, doff2);

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

		dr = vis_fpack16_pair(dr1, dr2);
		dr1 = vis_fpack16_pair(dr3, dr4);

		dg = vis_fpack16_pair(dg1, dg2);
		dg1 = vis_fpack16_pair(dg3, dg4);

		db = vis_fpack16_pair(db1, db2);
		db1 = vis_fpack16_pair(db3, db4);

		dg2 = vis_fpmerge(vis_read_hi(da0), vis_read_hi(dg));
		dg3 = vis_fpmerge(vis_read_hi(db), vis_read_hi(dr));

		res_buf[0] = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
		res_buf[1] = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));

		dg2 = vis_fpmerge(vis_read_lo(da0), vis_read_lo(dg));
		dg3 = vis_fpmerge(vis_read_lo(db), vis_read_lo(dr));

		res_buf[2] = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
		res_buf[3] = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));

		dg2 = vis_fpmerge(vis_read_hi(da1), vis_read_hi(dg1));
		dg3 = vis_fpmerge(vis_read_hi(db1), vis_read_hi(dr1));

		res_buf[4] = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
		res_buf[5] = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));

		dg2 = vis_fpmerge(vis_read_lo(da1), vis_read_lo(dg1));
		dg3 = vis_fpmerge(vis_read_lo(db1), vis_read_lo(dr1));

		res_buf[6] = vis_fpmerge(vis_read_hi(dg2), vis_read_hi(dg3));
		res_buf[7] = vis_fpmerge(vis_read_lo(dg2), vis_read_lo(dg3));

		for (i = 0; i < left; i++)
			((mlib_f32 *)dpp)[16 * count + i] =
				((mlib_f32 *)res_buf)[i];
	}
}

/* *********************************************************** */

void
__mlib_VideoColorYUV411seq_to_ABGRint(
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
	mlib_d64 y_buf[BUF_SIZE / 8], u_buf[BUF_SIZE / 8], v_buf[BUF_SIZE / 8];
	mlib_d64 *y0 = y_buf, *u0 = u_buf, *v0 = v_buf;
	mlib_d64 *y_tmp = NULL, *z_tmp;
	mlib_d64 w_alpha =
		vis_to_double_dup((a_const << 24) | (a_const << 16) | (a_const
		<< 8) | a_const);
	mlib_s32 w_algn = ((w + 7) & ~7), w4 = w >> 2;
	mlib_s32 i, count = w >> 4, left = w & 15;

	dlb >>= 2;

/*
 * initialize GSR scale factor
 */
	vis_write_gsr(3 << 3);

	if (w > BUF_SIZE) {
		y0 = (y_tmp = __mlib_malloc(3 * w_algn));
		if (y_tmp == NULL)
			return;
		u0 = (z_tmp = y_tmp + w_algn / sizeof (mlib_d64));
		v0 = (z_tmp = z_tmp + w_algn / sizeof (mlib_d64));
	}

	for (i = 0; i < h; i++, abgr += dlb, y += aylb, u += uvlb, v += uvlb) {
		mlib_d64 *y1 = y0, *u1 = u0, *v1 = v0;

		if (((mlib_addr)y & 7) == 0)
			y1 = (mlib_d64 *)y;
		else
			__mlib_VectorCopy_U8((mlib_u8 *)y1, y, w);

		if (((mlib_addr)u & 3) == 0)
			u1 = (mlib_d64 *)u;
		else
			__mlib_VectorCopy_U8((mlib_u8 *)u1, u, w4);

		if (((mlib_addr)v & 3) == 0)
			v1 = (mlib_d64 *)v;
		else
			__mlib_VectorCopy_U8((mlib_u8 *)v1, v, w4);

		if (a_array) {
			mlib_v_VideoYUV2ABGR_aarray_411(abgr, y1,
				(mlib_f32 *)u1, (mlib_f32 *)v1,
				(mlib_d64 *)a_array, count, left, 0);
			a_array += aylb;
		} else {
			mlib_v_VideoYUV2ABGR_alpha_411(abgr, y1, (mlib_f32 *)u1,
				(mlib_f32 *)v1, w_alpha, count, left, 0);
		}
	}

	if (y_tmp) {
		__mlib_free(y_tmp);
	}
}

/* *********************************************************** */

void
__mlib_VideoColorYUV411seq_to_ARGBint(
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
	mlib_d64 y_buf[BUF_SIZE / 8], u_buf[BUF_SIZE / 8], v_buf[BUF_SIZE / 8];
	mlib_d64 *y0 = y_buf, *u0 = u_buf, *v0 = v_buf;
	mlib_d64 *y_tmp = NULL, *z_tmp;
	mlib_d64 w_alpha =
		vis_to_double_dup((a_const << 24) | (a_const << 16) | (a_const
		<< 8) | a_const);
	mlib_s32 w_algn = ((w + 7) & ~7), w4 = w >> 2;
	mlib_s32 i, count = w >> 4, left = w & 15;

	dlb >>= 2;

/*
 * initialize GSR scale factor
 */
	vis_write_gsr(3 << 3);

	if (w > BUF_SIZE) {
		y0 = (y_tmp = __mlib_malloc(3 * w_algn));
		if (y_tmp == NULL)
			return;
		u0 = (z_tmp = y_tmp + w_algn / sizeof (mlib_d64));
		v0 = (z_tmp = z_tmp + w_algn / sizeof (mlib_d64));
	}

	for (i = 0; i < h; i++, argb += dlb, y += aylb, u += uvlb, v += uvlb) {
		mlib_d64 *y1 = y0, *u1 = u0, *v1 = v0;

		if (((mlib_addr)y & 7) == 0)
			y1 = (mlib_d64 *)y;
		else
			__mlib_VectorCopy_U8((mlib_u8 *)y1, y, w);

		if (((mlib_addr)u & 3) == 0)
			u1 = (mlib_d64 *)u;
		else
			__mlib_VectorCopy_U8((mlib_u8 *)u1, u, w4);

		if (((mlib_addr)v & 3) == 0)
			v1 = (mlib_d64 *)v;
		else
			__mlib_VectorCopy_U8((mlib_u8 *)v1, v, w4);

		if (a_array) {
			mlib_v_VideoYUV2ABGR_aarray_411(argb, y1,
				(mlib_f32 *)v1, (mlib_f32 *)u1,
				(mlib_d64 *)a_array, count, left, 1);
			a_array += aylb;
		} else {
			mlib_v_VideoYUV2ABGR_alpha_411(argb, y1, (mlib_f32 *)v1,
				(mlib_f32 *)u1, w_alpha, count, left, 1);
		}
	}

	if (y_tmp) {
		__mlib_free(y_tmp);
	}
}

/* *********************************************************** */
