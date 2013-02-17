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

#pragma ident	"@(#)mlib_v_ImageColorRGB2MonoFunc.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_v_ImageColorRGB2Mono_U8_A8D1X8
 *      mlib_v_ImageColorRGB2Mono_U8_A8D2X8
 *      mlib_v_ImageColorRGB2Mono_U8_D1
 *      mlib_v_ImageColorRGB2Mono_U8
 *      mlib_v_ImageColorRGB2Mono_S32
 *
 * ARGUMENT
 *      src      pointer to source image data
 *      dst      pointer to destination image data
 *      slb      source image line stride in bytes
 *      dlb      destination image line stride in bytes
 *      dsize    image data size in pixels
 *      xsize    image width in pixels
 *      ysize    image height in lines
 *      weight   array of 3 blending coefficients
 *
 * DESCRIPTION
 *  Separate the three channels of the source image into three
 *  1-channel destination images -- VIS version low level functions.
 *
 * NOTE
 *  These functions are separated from mlib_v_ImageChannelColorRGB2Mono.c
 *  for loop unrolling and structure clarity.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageColorRGB2MonoFunc.h>

/* *********************************************************** */

#define	FLOAT2INT_CLAMP(DST, SRC)                               \
	{                                                       \
	    mlib_d64 dsrc = (mlib_d64)(SRC);                    \
	                                                        \
	    if (dsrc > (mlib_d64)MLIB_S32_MAX)                  \
		dsrc = (mlib_d64)MLIB_S32_MAX;                  \
	    if (dsrc < (mlib_d64)MLIB_S32_MIN)                  \
		dsrc = (mlib_d64)MLIB_S32_MIN;                  \
	    (*DST++) = (mlib_s32)dsrc;                          \
	}

/* *********************************************************** */

#define	CHANNELSEPARATE_U8_AL(sd0, sd1, sd2, sd3, rgdd0,        \
	bdd0, rgdd1, bdd1)                                      \
	vis_write_bmask(mask0, 0);                              \
	rgdd0 = vis_bshuffle(sd0, sd1);                         \
	rgdd1 = vis_bshuffle(sd2, sd3);                         \
	vis_write_bmask(mask1, 0);                              \
	bdd0 = vis_bshuffle(sd0, sd1);                          \
	bdd1 = vis_bshuffle(sd2, sd3)

/* *********************************************************** */

#define	CHANNELSEPARATE_U8(sd0, sd1, sd2, rgdd0, bdd0, rgdd1,    \
	bdd1)                                                    \
	vis_write_bmask(mask0, 0);                               \
	rgdd0 = vis_bshuffle(sd0, sd1);                          \
	vis_write_bmask(mask1, 0);                               \
	bdd0 = vis_bshuffle(sd0, sd1);                           \
	vis_write_bmask(mask2, 0);                               \
	rgdd1 = vis_bshuffle(sd1, sd2);                          \
	vis_write_bmask(mask3, 0);                               \
	bdd1 = vis_bshuffle(sd1, sd2)

/* *********************************************************** */

#define	CHANNELWEIGHT_U8(rgdd0, bdd0, rgdd1, bdd1, dd)          \
	dd = vis_fmul8x16al(vis_read_hi(rgdd0), alpha);         \
	ddt = vis_fmul8x16al(vis_read_lo(rgdd0), beta);         \
	dd = vis_fpadd16(dd, ddt);                              \
	ddt = vis_fmul8x16al(vis_read_hi(bdd0), gamma);         \
	dd = vis_fpadd16(dd, ddt);                              \
	d32 = vis_fpack16(dd);                                  \
	dd = vis_fmul8x16al(vis_read_hi(rgdd1), alpha);         \
	ddt = vis_fmul8x16al(vis_read_lo(rgdd1), beta);         \
	dd = vis_fpadd16(dd, ddt);                              \
	ddt = vis_fmul8x16al(vis_read_hi(bdd1), gamma);         \
	ddt = vis_fpadd16(dd, ddt);                             \
	e32 = vis_fpack16(ddt);                                 \
	dd = vis_freg_pair(d32, e32)

/* *********************************************************** */

void
mlib_v_ImageColorRGB2Mono_U8_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_d64 *weight)
{
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_d64 dd;
	mlib_d64 rgdd0, rgdd1, bdd0, bdd1, ddt;
	mlib_f32 d32, e32, alpha, gamma, beta;
	mlib_s32 i;
	mlib_s32 mask0 = 0x0369147a;
	mlib_s32 mask1 = 0x258b258b;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

/* prepare the weight */
	alpha = vis_to_float(weight[0] * 8192);
	beta = vis_to_float(weight[1] * 8192);
	gamma = vis_to_float(weight[2] * 8192);
	vis_write_gsr((2 << 3) + 4);
#pragma pipeloop(0)
	for (i = 0; i < dsize / 8; i++) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		sd3 = vis_faligndata(sd2, sd2);
		sd2 = vis_faligndata(sd1, sd2);
		CHANNELSEPARATE_U8_AL(sd0, sd1, sd2, sd3, rgdd0, bdd0, rgdd1,
		    bdd1);
		CHANNELWEIGHT_U8(rgdd0, bdd0, rgdd1, bdd1, dd);
		(*dp++) = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorRGB2Mono_U8_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight)
{
	mlib_d64 *sp, *dp;
	mlib_d64 *sl, *dl;
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_d64 dd;
	mlib_d64 rgdd0, bdd0, rgdd1, bdd1, ddt;
	mlib_f32 d32, e32, alpha, gamma, beta;
	mlib_s32 i, j;
	mlib_s32 mask0 = 0x0369147a;
	mlib_s32 mask1 = 0x258b258b;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

/* prepare the weight */
	alpha = vis_to_float(weight[0] * 8192);
	beta = vis_to_float(weight[1] * 8192);
	gamma = vis_to_float(weight[2] * 8192);
	vis_write_gsr((2 << 3) + 4);
	for (j = 0; j < ysize; j++) {
#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			sd3 = vis_faligndata(sd2, sd2);
			sd2 = vis_faligndata(sd1, sd2);
			CHANNELSEPARATE_U8_AL(sd0, sd1, sd2, sd3, rgdd0, bdd0,
			    rgdd1, bdd1);
			CHANNELWEIGHT_U8(rgdd0, bdd0, rgdd1, bdd1, dd);
			(*dp++) = dd;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorRGB2Mono_U8_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_d64 *weight)
{
	mlib_u8 *dst_end;
	mlib_d64 dd, d0, d1, d2, d3;
	mlib_d64 rgdd0, bdd0, rgdd1, bdd1, ddt;
	mlib_d64 *src_all, *dp;
	mlib_f32 d32, e32, alpha, gamma, beta;
	mlib_d64 sd0, sd1, sd2;
	mlib_s32 i, emask;
	mlib_s32 off;
	mlib_s32 mask0 = 0x0369147a;
	mlib_s32 mask1 = 0x258b258b;
	mlib_s32 mask2 = 0x47ad58be;
	mlib_s32 mask3 = 0x69cf69cf;

/* prepare the weight */
	alpha = vis_to_float(weight[0] * 8192);
	beta = vis_to_float(weight[1] * 8192);
	gamma = vis_to_float(weight[2] * 8192);
	vis_write_gsr(2 << 3);

	dp = (mlib_d64 *)((mlib_addr)dst & (~7));
	off = (mlib_addr)dp - (mlib_addr)dst;

	dst_end = dst + (dsize - 1);
	emask = vis_edge8(dst, dst_end);
	src_all = vis_alignaddr((void *)src, (3 * off));

	d0 = (*src_all++);
	d1 = (*src_all++);
	d2 = (*src_all++);
	d3 = (*src_all++);

	sd0 = vis_faligndata(d0, d1);
	sd1 = vis_faligndata(d1, d2);
	sd2 = vis_faligndata(d2, d3);

	CHANNELSEPARATE_U8(sd0, sd1, sd2, rgdd0, bdd0, rgdd1, bdd1);
	CHANNELWEIGHT_U8(rgdd0, bdd0, rgdd1, bdd1, dd);
	vis_pst_8(dd, dp, emask);
	dp++;

#pragma pipeloop(0)
	for (i = 8 + off; i <= (dsize - 8); i += 8) {
		d0 = d3;
		d1 = (*src_all++);
		d2 = (*src_all++);
		d3 = (*src_all++);

		sd0 = vis_faligndata(d0, d1);
		sd1 = vis_faligndata(d1, d2);
		sd2 = vis_faligndata(d2, d3);

		CHANNELSEPARATE_U8(sd0, sd1, sd2, rgdd0, bdd0, rgdd1, bdd1);
		CHANNELWEIGHT_U8(rgdd0, bdd0, rgdd1, bdd1, dd);
		(*dp++) = dd;
	}

	if ((mlib_addr)dp <= (mlib_addr)dst_end) {

		emask = vis_edge8(dp, dst_end);
		d0 = d3;
		d1 = (*src_all++);
		d2 = (*src_all++);
		d3 = (*src_all++);
		sd0 = vis_faligndata(d0, d1);
		sd1 = vis_faligndata(d1, d2);
		sd2 = vis_faligndata(d2, d3);

		CHANNELSEPARATE_U8(sd0, sd1, sd2, rgdd0, bdd0, rgdd1, bdd1);
		CHANNELWEIGHT_U8(rgdd0, bdd0, rgdd1, bdd1, dd);
		vis_pst_8(dd, dp, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorRGB2Mono_U8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight)
{
	mlib_u8 *sa, *da;
	mlib_u8 *sl, *dl;
	mlib_s32 j;

	sa = sl = (void *)src;
	da = dl = dst;

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageColorRGB2Mono_U8_D1(sa, da, xsize, weight);
		sa = sl += slb;
		da = dl += dlb;
	}
}

/* *********************************************************** */

mlib_status
mlib_v_ImageColorRGB2Mono_S32(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight)
{
	mlib_s32 *sa, *sl;
	mlib_s32 *da, *dl;
	mlib_d64 sf0, sf1, sf2, df1, w[3];
	mlib_d64 sf3, sf4, sf5, df2;
	mlib_d64 sf6, sf7, sf8, df3;
	mlib_s32 i, j;
	mlib_s32 num_blocks, leftover_cols;

	w[0] = weight[0];
	w[1] = weight[1];
	w[2] = weight[2];

	sl = sa = (void *)src;
	dl = da = dst;

	num_blocks = xsize / 3;
	leftover_cols = xsize - num_blocks * 3;

	for (j = 0; j < ysize; j++) {
		for (i = 0; i < num_blocks; i++) {
			sf0 = (*sa++);
			sf1 = (*sa++);
			sf2 = (*sa++);
			sf3 = (*sa++);
			sf4 = (*sa++);
			sf5 = (*sa++);
			sf6 = (*sa++);
			sf7 = (*sa++);
			sf8 = (*sa++);

			df1 = w[0] * sf0 + w[1] * sf1 + w[2] * sf2;
			df2 = w[0] * sf3 + w[1] * sf4 + w[2] * sf5;
			df3 = w[0] * sf6 + w[1] * sf7 + w[2] * sf8;

			FLOAT2INT_CLAMP(da, df1);
			FLOAT2INT_CLAMP(da, df2);
			FLOAT2INT_CLAMP(da, df3);
		}

		if (leftover_cols) {
			sf0 = (*sa++);
			sf1 = (*sa++);
			sf2 = (*sa++);

			df1 = w[0] * sf0 + w[1] * sf1 + w[2] * sf2;
			FLOAT2INT_CLAMP(da, df1);

			if (leftover_cols > 1) {
				sf0 = (*sa++);
				sf1 = (*sa++);
				sf2 = (*sa++);

				df1 = w[0] * sf0 + w[1] * sf1 + w[2] * sf2;
				FLOAT2INT_CLAMP(da, df1);
			}
		}

		sl = sa = sl + slb;
		dl = da = dl + dlb;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
