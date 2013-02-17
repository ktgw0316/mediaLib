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

#pragma ident	"@(#)mlib_v_ImageZoomOut2X_NN.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomOut2X -  image 0.5X scaling with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoomOut2X(mlib_image       *dst,
 *                                      const mlib_image *src,
 *                                      mlib_filter      filter,
 *                                      mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      filter    Type of resampling filter.
 *      edge      Type of edge condition.
 *
 * DESCRIPTION
 *  The center of the source image is mapped to the center of the
 *  destination image.
 *
 *  The upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *  The resampling filter can be one of the following:
 *    MLIB_NEAREST
 *    MLIB_BILINEAR
 *    MLIB_BICUBIC
 *
 *  The edge condition can be one of the following:
 *    MLIB_EDGE_DST_NO_WRITE  (default)
 *    MLIB_EDGE_DST_FILL_ZERO
 *    MLIB_EDGE_OP_NEAREST
 *    MLIB_EDGE_SRC_EXTEND
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageZoomOut2X.h>

/* *********************************************************** */

static void mlib_ImageZoomOut2X_Nearest_1(
    NEAREST_PARAMETERS(mlib_u8),
    mlib_s32 pix_size,
    const mlib_s32 *bmask);

static void mlib_ImageZoomOut2X_Nearest_3(
    NEAREST_PARAMETERS(mlib_u8),
    mlib_s32 pix_size,
    const mlib_s32 *bmask);

/* *********************************************************** */

void
mlib_ImageZoomOut2X_Nearest_1(
    NEAREST_PARAMETERS(mlib_u8),
    mlib_s32 pix_size,
    const mlib_s32 *bmask)
{
	mlib_u8 *sl, *dl, *dend;
	mlib_d64 *sp, *dp, s0, s1, s2, dd;
	mlib_s32 i, off, soff, emask, pix_mask;

	dst_width = pix_size * dst_width;
	pix_mask = pix_size - 1;

	for (i = 0; i < dst_height; i++) {
		sl = p_src + 2 * i * src_stride;
		dl = p_dst + i * dst_stride;
		dend = dl + dst_width - 1;

		off = (int)dl & 7;
		dp = (mlib_d64 *)(dl - off);
		soff = (off + pix_mask) & ~pix_mask;

		off = soff - off;
		vis_write_bmask(bmask[off], 0);

		sl -= 2 * soff;
		sl += off;
		sp = vis_alignaddr(sl, 0);

/* source aligned */
		if ((mlib_u8 *)sp == sl) {
			emask = vis_edge8(dl, dend);
			dd = vis_bshuffle(sp[0], sp[1]);
			vis_pst_8(dd, dp, emask);
			dp++;
			sp += 2;

#pragma pipeloop(0)
			for (; (mlib_u8 *)dp <= (dend + 1 - 8); dp++) {
				*dp = vis_bshuffle(sp[0], sp[1]);
				sp += 2;
			}

			if ((mlib_u8 *)dp <= dend) {
				emask = vis_edge8(dp, dend);
				dd = vis_bshuffle(sp[0], sp[1]);
				vis_pst_8(dd, dp, emask);
			}

		} else {
/* source not aligned */
			emask = vis_edge8(dl, dend);
			s0 = sp[0];
			s1 = sp[1];
			s2 = sp[2];
			s0 = vis_faligndata(s0, s1);
			s1 = vis_faligndata(s1, s2);
			dd = vis_bshuffle(s0, s1);
			vis_pst_8(dd, dp, emask);
			dp++;
			sp += 2;
#pragma pipeloop(0)
			for (; (mlib_u8 *)dp <= (dend + 1 - 8); dp++) {
				s0 = s2;
				s1 = sp[1];
				s2 = sp[2];
				s0 = vis_faligndata(s0, s1);
				s1 = vis_faligndata(s1, s2);
				*dp = vis_bshuffle(s0, s1);
				sp += 2;
			}

			if ((mlib_u8 *)dp <= dend) {
				emask = vis_edge8(dp, dend);
				s0 = s2;
				s1 = sp[1];
				s2 = sp[2];
				s0 = vis_faligndata(s0, s1);
				s1 = vis_faligndata(s1, s2);
				dd = vis_bshuffle(s0, s1);
				vis_pst_8(dd, dp, emask);
			}
		}
	}
}

/* *********************************************************** */

static const mlib_s32 mlib_bmask_u8[] = {
	0,
	0x02468ACE,
	0x014589CD,
	0x03478BCF,
	0x012389AB,
	0x012789AF,
	0x016789EF,
	0x05678DEF
};

void
mlib_ImageZoomOut2X_U8_1_Nearest(
    NEAREST_PARAMETERS(mlib_u8))
{
	mlib_ImageZoomOut2X_Nearest_1(NEAREST_P, 1, mlib_bmask_u8 + 1);
}

void
mlib_ImageZoomOut2X_U8_2_Nearest(
    NEAREST_PARAMETERS(mlib_u8))
{
	mlib_ImageZoomOut2X_Nearest_1(NEAREST_P, 2, mlib_bmask_u8 + 2);
}

void
mlib_ImageZoomOut2X_U8_4_Nearest(
    NEAREST_PARAMETERS(mlib_u8))
{
	mlib_ImageZoomOut2X_Nearest_1(NEAREST_P, 4, mlib_bmask_u8 + 4);
}

/* *********************************************************** */

static const mlib_s32 mlib_bmask_s16[] = {
	0, 0,
	0x014589CD, 0,
	0x012389AB, 0,
	0x016789EF, 0,
	0x01234567, 0,
	0x012345EF, 0,
	0x0123CDEF, 0,
	0x01ABCDEF, 0,
};

void
mlib_ImageZoomOut2X_S16_1_Nearest(
    NEAREST_PARAMETERS(mlib_s16))
{
	src_stride *= 2;
	dst_stride *= 2;
	mlib_ImageZoomOut2X_Nearest_1(NEAREST_P, 2, mlib_bmask_s16 + 2);
}

void
mlib_ImageZoomOut2X_S16_2_Nearest(
    NEAREST_PARAMETERS(mlib_s16))
{
	src_stride *= 2;
	dst_stride *= 2;
	mlib_ImageZoomOut2X_Nearest_1(NEAREST_P, 4, mlib_bmask_s16 + 4);
}

void
mlib_ImageZoomOut2X_S16_4_Nearest(
    NEAREST_PARAMETERS(mlib_s16))
{
	src_stride *= 2;
	dst_stride *= 2;
	mlib_ImageZoomOut2X_Nearest_1(NEAREST_P, 8, mlib_bmask_s16 + 8);
}

/* *********************************************************** */

#define	ZOOMOUT2X_3CH()                                         \
	vis_write_bmask(bm0, 0);                                \
	d0 = vis_bshuffle(s0, s1);                              \
	d2 = vis_bshuffle(s3, s4);                              \
	vis_write_bmask(bm1, 0);                                \
	d1 = vis_bshuffle(s1, s2);                              \
	d3 = vis_bshuffle(s4, s5);                              \
	dp[0] = vis_read_hi(d0);                                \
	dp[1] = vis_read_lo(d0);                                \
	dp[2] = vis_read_hi(d1);                                \
	dp[3] = vis_read_hi(d2);                                \
	dp[4] = vis_read_lo(d2);                                \
	dp[5] = vis_read_hi(d3)

/* *********************************************************** */

void
mlib_ImageZoomOut2X_Nearest_3(
    NEAREST_PARAMETERS(mlib_u8),
    mlib_s32 pix_size,
    const mlib_s32 *bmask)
{
	mlib_u8 *sl, *dl;
	mlib_d64 *sp, s0, s1, s2, s3, s4, s5, s6, d0, d1, d2, d3;
	mlib_f32 *dp;
	mlib_s32 bm0 = bmask[0];
	mlib_s32 bm1 = bmask[1];
	mlib_s32 i, j, n;

	if (pix_size == 6)
		dst_width *= 2;

	for (j = 0; j < dst_height; j++) {
		sl = p_src + j * 2 * src_stride;
		dl = p_dst + j * dst_stride;
		n = dst_width;

		if (pix_size == 3) {
			for (; ((int)dl & 3) && n; n--) {
				dl[0] = sl[0];
				dl[1] = sl[1];
				dl[2] = sl[2];
				dl += 3;
				sl += 6;
			}

		} else {
			for (; ((int)dl & 3) && n; n -= 2) {
				((mlib_s16 *)dl)[0] = ((mlib_s16 *)sl)[0];
				((mlib_s16 *)dl)[1] = ((mlib_s16 *)sl)[1];
				((mlib_s16 *)dl)[2] = ((mlib_s16 *)sl)[2];
				dl += 6;
				sl += 12;
			}
		}

		dp = (mlib_f32 *)dl;
		sp = vis_alignaddr(sl, 0);

		if ((mlib_u8 *)sp == sl) {
#pragma pipeloop(0)
			s0 = sp[0];
			s1 = sp[1];
			s2 = sp[2];
			s3 = sp[3];
			s4 = sp[4];
			s5 = sp[5];
			sp += 6;
			for (i = 0; i <= (n - 8); i += 8) {
				ZOOMOUT2X_3CH();
				s0 = sp[0];
				s1 = sp[1];
				s2 = sp[2];
				s3 = sp[3];
				s4 = sp[4];
				s5 = sp[5];
				sp += 6;
				dp += 6;
			}

		} else {
			s6 = sp[0];
#pragma pipeloop(0)
			for (i = 0; i <= (n - 8); i += 8) {
				s0 = s6;
				s1 = sp[1];
				s2 = sp[2];
				s3 = sp[3];
				s4 = sp[4];
				s5 = sp[5];
				s6 = sp[6];
				sp += 6;
				s0 = vis_faligndata(s0, s1);
				s1 = vis_faligndata(s1, s2);
				s2 = vis_faligndata(s2, s3);
				s3 = vis_faligndata(s3, s4);
				s4 = vis_faligndata(s4, s5);
				s5 = vis_faligndata(s5, s6);
				ZOOMOUT2X_3CH();
				dp += 6;
			}
		}

		sl += 6 * i;
		dl = (mlib_u8 *)dp;

		if (pix_size == 3) {
			for (; i < n; i++) {
				dl[0] = sl[0];
				dl[1] = sl[1];
				dl[2] = sl[2];
				dl += 3;
				sl += 6;
			}

		} else {
			for (; i < n; i += 2) {
				((mlib_s16 *)dl)[0] = ((mlib_s16 *)sl)[0];
				((mlib_s16 *)dl)[1] = ((mlib_s16 *)sl)[1];
				((mlib_s16 *)dl)[2] = ((mlib_s16 *)sl)[2];
				dl += 6;
				sl += 12;
			}
		}
	}
}

/* *********************************************************** */

static const mlib_s32 mlib_bmask_u8_3[] = {
	0x012678cd, 0x6abc0000
};

void
mlib_ImageZoomOut2X_U8_3_Nearest(
    NEAREST_PARAMETERS(mlib_u8))
{
	mlib_ImageZoomOut2X_Nearest_3(NEAREST_P, 3, mlib_bmask_u8_3);
}

/* *********************************************************** */

static const mlib_s32 mlib_bmask_s16_3[] = {
	0x012345cd, 0x67890000
};

void
mlib_ImageZoomOut2X_S16_3_Nearest(
    NEAREST_PARAMETERS(mlib_s16))
{
	src_stride *= 2;
	dst_stride *= 2;
	mlib_ImageZoomOut2X_Nearest_3(NEAREST_P, 6, mlib_bmask_s16_3);
}

/* *********************************************************** */
