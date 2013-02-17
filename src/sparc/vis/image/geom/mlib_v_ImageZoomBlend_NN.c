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

#pragma ident	"@(#)mlib_v_ImageZoomBlend_NN.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      Internal functions for mlib_ImageZoomTranslateBlend().
 *
 */

#include <mlib_image.h>
#include <vis_proto.h>
#include <mlib_v_ImageChannelExtract.h>

/* *********************************************************** */

#define	_MLIB_VIS_VER_

#include <mlib_ImageZoom.h>

/* *********************************************************** */

#define	VARIABLE(FORMAT)                                             \
	mlib_s32 j,                                                  \
		dx = GetElemStruct(DX),                              \
		dy = GetElemStruct(DY),                              \
		x = GetElemSubStruct(current, srcX),                 \
		y = GetElemSubStruct(current, srcY),                 \
		src_stride = GetElemStruct(src_stride),              \
		dst_stride = GetElemStruct(dst_stride),              \
		width  = GetElemSubStruct(current, width),           \
		height = GetElemSubStruct(current, height);          \
	mlib_s32 alp_ind = param -> alp_ind

/* *********************************************************** */

#define	BLEND                                                   \
	s1 = (*bp++);                                           \
	ss = vis_faligndata(s0, s1);                            \
	s0 = s1;                                                \
	dd = *(mlib_d64 *)dp;                                   \
	d0 =                                                    \
	vis_fpadd16(vis_fmul8x16au(vis_read_hi(ss), aa),        \
	    vis_fmul8x16al(vis_read_hi(dd), aa));               \
	d1 =                                                    \
	vis_fpadd16(vis_fmul8x16au(vis_read_lo(ss), aa),        \
	    vis_fmul8x16al(vis_read_lo(dd), aa));               \
	dd = vis_fpack16_pair(d0, d1)

/* *********************************************************** */

mlib_status
mlib_ImageZoom_U8_33_Nearest(
    mlib_work_image *param,
    mlib_u8 *sp,
    mlib_u8 *dp)
{
	VARIABLE(mlib_u8);
	mlib_u8 *dl = dp, *tsp, *tt;
	mlib_s32 cx, y0 = -1, dx7 = 7 * dx, dx15 = 8 * dx + dx7;
	mlib_s32 off, i, alp;
	mlib_f32 aa;
	mlib_u8 *buffd;

	alp = (param->alpha) * (16384.0 / 255);
	((mlib_s32 *)&aa)[0] = (alp << 16) | (16384 - alp);

	tsp = sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	buffd = __mlib_malloc(3 * width + 24);

	for (j = 0; j < height; j++) {
		mlib_u8 *dp = buffd, *dend = dp + 3 * width;
		mlib_d64 s0, s1, ss, d0, d1, dd, *bp;

		vis_write_gsr((1 << 3) | 7);
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		x += dx7;

#pragma pipeloop(0)
		for (; dp <= dend - 24; dp += 24) {
			mlib_d64 s0, s1, s2;

			cx = x >> MLIB_SHIFT;
			tt = tsp + 2 * cx + cx;
			x -= dx;
			s2 = vis_faligndata(vis_ld_u8_i(tt, 2), s2);
			s2 = vis_faligndata(vis_ld_u8_i(tt, 1), s2);
			s2 = vis_faligndata(vis_ld_u8_i(tt, 0), s2);

			cx = x >> MLIB_SHIFT;
			tt = tsp + 2 * cx + cx;
			x -= dx;
			s2 = vis_faligndata(vis_ld_u8_i(tt, 2), s2);
			s2 = vis_faligndata(vis_ld_u8_i(tt, 1), s2);
			s2 = vis_faligndata(vis_ld_u8_i(tt, 0), s2);

			cx = x >> MLIB_SHIFT;
			tt = tsp + 2 * cx + cx;
			x -= dx;
			s2 = vis_faligndata(vis_ld_u8_i(tt, 2), s2);
			s2 = vis_faligndata(vis_ld_u8_i(tt, 1), s2);
			s1 = vis_faligndata(vis_ld_u8_i(tt, 0), s1);

			cx = x >> MLIB_SHIFT;
			tt = tsp + 2 * cx + cx;
			x -= dx;
			s1 = vis_faligndata(vis_ld_u8_i(tt, 2), s1);
			s1 = vis_faligndata(vis_ld_u8_i(tt, 1), s1);
			s1 = vis_faligndata(vis_ld_u8_i(tt, 0), s1);

			cx = x >> MLIB_SHIFT;
			tt = tsp + 2 * cx + cx;
			x -= dx;
			s1 = vis_faligndata(vis_ld_u8_i(tt, 2), s1);
			s1 = vis_faligndata(vis_ld_u8_i(tt, 1), s1);
			s1 = vis_faligndata(vis_ld_u8_i(tt, 0), s1);

			cx = x >> MLIB_SHIFT;
			tt = tsp + 2 * cx + cx;
			x -= dx;
			s1 = vis_faligndata(vis_ld_u8_i(tt, 2), s1);
			s0 = vis_faligndata(vis_ld_u8_i(tt, 1), s0);
			s0 = vis_faligndata(vis_ld_u8_i(tt, 0), s0);

			cx = x >> MLIB_SHIFT;
			tt = tsp + 2 * cx + cx;
			x -= dx;
			s0 = vis_faligndata(vis_ld_u8_i(tt, 2), s0);
			s0 = vis_faligndata(vis_ld_u8_i(tt, 1), s0);
			s0 = vis_faligndata(vis_ld_u8_i(tt, 0), s0);

			cx = x >> MLIB_SHIFT;
			tt = tsp + 2 * cx + cx;
			x += dx15;
			s0 = vis_faligndata(vis_ld_u8_i(tt, 2), s0);
			s0 = vis_faligndata(vis_ld_u8_i(tt, 1), s0);
			s0 = vis_faligndata(vis_ld_u8_i(tt, 0), s0);

			((mlib_d64 *)dp)[0] = s0;
			((mlib_d64 *)dp)[1] = s1;
			((mlib_d64 *)dp)[2] = s2;
		}

		x -= dx7;

		while (dp < dend) {
			cx = x >> MLIB_SHIFT;
			tt = tsp + 2 * cx + cx;
			dp[0] = tt[0];
			dp[1] = tt[1];
			dp[2] = tt[2];
			x += dx;
			dp += 3;
		}

		off = (mlib_s32)dl & 7;
		dp = dl - off;
		dend = dl + 3 * width;
		bp = vis_alignaddr(buffd - off, 0);
		s0 = (*bp++);

		if (off) {
			BLEND;
			vis_pst_8(dd, dp, vis_edge8(dl, dend - 1));
			dp += 8;
		}
#pragma pipeloop(0)
		for (; dp <= dend - 8; dp += 8) {
			BLEND;
			*(mlib_d64 *)dp = dd;
		}

		if (dp < dend) {
			BLEND;
			vis_pst_8(dd, dp, vis_edge8(dp, dend - 1));
		}

		y0 = y;
		y += dy;
		dl = (void *)((mlib_u8 *)dl + dst_stride);
		tsp = (void *)((mlib_u8 *)sp + (y >> MLIB_SHIFT) * src_stride);
	}

	__mlib_free(buffd);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_U8_43_Nearest(
    mlib_work_image *param,
    mlib_u8 *sp,
    mlib_u8 *dp)
{
	VARIABLE(mlib_u8);
	mlib_u8 *dl = dp, *tsp, *tt;
	mlib_s32 cx, y0 = -1, dx7 = 7 * dx, dx15 = 8 * dx + dx7;
	mlib_s32 off, i;
	mlib_d64 a0, a1, done = vis_to_double(0, 16384);
	mlib_u8 *pp;
	mlib_f32 aa;
	mlib_f32 *buffd;
	mlib_blend blend = param->blend;

	buffd = __mlib_malloc(width * sizeof (mlib_f32) + 8 * 8);

	if (buffd == NULL) {
		return (MLIB_FAILURE);
	}

	if (blend != MLIB_BLEND_GTK_SRC) {
		mlib_s32 alp = (param->alpha) * (16384.0 / 255) * (256.0 / 255);

		aa = vis_to_float(alp);
	} else {
		aa = vis_to_float(16384.0 * (256.0 / 255));
	}

	tsp = sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {
		mlib_u8 *dp = dl;
		mlib_d64 ss, dd;

		vis_write_gsr((1 << 3) | 7);
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		if (blend != MLIB_BLEND_GTK_SRC) {
#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				mlib_d64 s0, s1, s2;

				cx = x >> MLIB_SHIFT;
				tt = tsp + 4 * cx;
				x += dx;
				ss = vis_faligndata(vis_ld_u8_i(tt, 2), ss);
				ss = vis_faligndata(vis_ld_u8_i(tt, 1), ss);
				ss = vis_faligndata(vis_ld_u8_i(tt, 0), ss);

				dd = vis_faligndata(vis_ld_u8_i(dp, 2), dd);
				dd = vis_faligndata(vis_ld_u8_i(dp, 1), dd);
				dd = vis_faligndata(vis_ld_u8_i(dp, 0), dd);

				a0 = vis_ld_u8_i(tt, alp_ind);
				a0 = vis_fmul8x16al(vis_read_lo(a0), aa);
				a1 = vis_fpsub16(done, a0);

				dd = vis_fpadd16(vis_fmul8x16al(vis_read_hi(ss),
				    vis_read_lo(a0)),
				    vis_fmul8x16al(vis_read_hi(dd),
				    vis_read_lo(a1)));

				buffd[i] = vis_fpack16(dd);

				dp += 3;
			}
		} else {
#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				mlib_d64 s0, s1, s2;

				cx = x >> MLIB_SHIFT;
				tt = tsp + 4 * cx;
				x += dx;
				ss = vis_faligndata(vis_ld_u8_i(tt, 2), ss);
				ss = vis_faligndata(vis_ld_u8_i(tt, 1), ss);
				ss = vis_faligndata(vis_ld_u8_i(tt, 0), ss);

				a0 = vis_ld_u8_i(tt, alp_ind);
				a0 = vis_fmul8x16al(vis_read_lo(a0), aa);

				dd = vis_fmul8x16al(vis_read_hi(ss),
				    vis_read_lo(a0));

				buffd[i] = vis_fpack16(dd);

				dp += 3;
			}
		}
		mlib_v_ImageChannelExtract_U8_43L_D1((void *)buffd, dl, width);

		y0 = y;
		y += dy;
		dl = (void *)((mlib_u8 *)dl + dst_stride);
		tsp = (void *)((mlib_u8 *)sp + (y >> MLIB_SHIFT) * src_stride);
	}

	__mlib_free(buffd);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
