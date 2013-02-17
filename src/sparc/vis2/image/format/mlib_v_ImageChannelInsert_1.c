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

#pragma ident	"@(#)mlib_v_ImageChannelInsert_1.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_v_ImageChannelInsert_U8_12_A8D1X8
 *      mlib_v_ImageChannelInsert_U8_12_D1
 *      mlib_v_ImageChannelInsert_U8_12
 *      mlib_v_ImageChannelInsert_U8_13_A8D1X8
 *      mlib_v_ImageChannelInsert_U8_13_D1
 *      mlib_v_ImageChannelInsert_U8_13
 *      mlib_v_ImageChannelInsert_U8_14_A8D1X8
 *      mlib_v_ImageChannelInsert_U8_14_D1
 *      mlib_v_ImageChannelInsert_U8_14
 *      mlib_v_ImageChannelInsert_S16_12_A8D1X4
 *      mlib_v_ImageChannelInsert_S16_12_D1
 *      mlib_v_ImageChannelInsert_S16_12
 *      mlib_v_ImageChannelInsert_S16_13_A8D1X4
 *      mlib_v_ImageChannelInsert_S16_13_D1
 *      mlib_v_ImageChannelInsert_S16_13
 *      mlib_v_ImageChannelInsert_S16_14_A8D1X4
 *      mlib_v_ImageChannelInsert_S16_14_D1
 *      mlib_v_ImageChannelInsert_S16_14
 *      mlib_v_ImageChannelInsert_S32
 *      mlib_v_ImageChannelInsert_D64
 *
 * ARGUMENT
 *      src     pointer to source image data
 *      dst     pointer to destination image data
 *      slb     source image line stride in bytes
 *      dlb     destination image line stride in bytes
 *      dsize   image data size in pixels
 *      xsize   image width in pixels
 *      ysize   image height in lines
 *      cmask   channel mask
 *
 * DESCRIPTION
 *      Copy the 1-channel source image into the selected channel
 *      of the destination image -- VIS version low level functions.
 *
 * NOTE
 *      These functions are separated from mlib_v_ImageChannelInsert.c
 *      for loop unrolling and structure clarity.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageChannelInsert.h>
#include <mlib_v_ImageChannelInsert_2.h>

/* *********************************************************** */

/* general channel insertion: slower due to the inner loop */
void
mlib_v_ImageChannelInsert_D64(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 channels,
    mlib_s32 channeld,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask)
{
/* pointer for pixel in src */
	mlib_d64 *sp;

/* pointer for line in src */
	mlib_d64 *sl;

/* pointer for pixel in dst */
	mlib_d64 *dp;

/* pointer for line in dst */
	mlib_d64 *dl;

/* indices for x, y, channel */
	mlib_s32 i, j, k;
	mlib_s32 deltac[5] = { 0, 1, 1, 1, 1 };
	mlib_s32 inc0, inc1, inc2;
	mlib_d64 s0, s1, s2;

	deltac[channels] = 1;
	for (i = (channeld - 1), k = 0; i >= 0; i--) {
		if ((cmask & (1 << i)) == 0)
			deltac[k]++;
		else
			k++;
	}

	deltac[channels] = channeld;
	for (i = 1; i < channels; i++) {
		deltac[channels] -= deltac[i];
	}

	sp = sl = (void *)src;
	dp = dl = dst + deltac[0];

	if (channels == 1) {
		for (j = 0; j < height; j++) {

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				dp[0] = s0;
				dp += channeld;
				sp++;
			}

			sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		}
	} else if (channels == 2) {
		inc0 = deltac[1];
		inc1 = deltac[2] + inc0;
		for (j = 0; j < height; j++) {

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				s1 = sp[1];
				dp[0] = s0;
				dp[inc0] = s1;
				dp += inc1;
				sp += 2;
			}

			sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		}
	} else if (channels == 3) {
		inc0 = deltac[1];
		inc1 = deltac[2] + inc0;
		inc2 = deltac[3] + inc1;
		for (j = 0; j < height; j++) {

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				s1 = sp[1];
				s2 = sp[2];
				dp[0] = s0;
				dp[inc0] = s1;
				dp[inc1] = s2;
				dp += inc2;
				sp += 3;
			}

			sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */

/* general channel insertion: slower due to the inner loop */

void
mlib_v_ImageChannelInsert_S32(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 channels,
    mlib_s32 channeld,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask)
{
/* pointer for pixel in src */
	mlib_s32 *sp;

/* pointer for line in src */
	mlib_s32 *sl;

/* pointer for pixel in dst */
	mlib_s32 *dp;

/* pointer for line in dst */
	mlib_s32 *dl;

/* indices for x, y, channel */
	mlib_s32 i, j, k;
	mlib_s32 deltac[5] = { 0, 1, 1, 1, 1 };
	mlib_s32 inc0, inc1, inc2;
	mlib_s32 s0, s1, s2;

	deltac[channels] = 1;
	for (i = (channeld - 1), k = 0; i >= 0; i--) {
		if ((cmask & (1 << i)) == 0)
			deltac[k]++;
		else
			k++;
	}

	deltac[channels] = channeld;
	for (i = 1; i < channels; i++) {
		deltac[channels] -= deltac[i];
	}

	sp = sl = (void *)src;
	dp = dl = dst + deltac[0];

	if (channels == 1) {
		for (j = 0; j < height; j++) {

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				dp[0] = s0;
				dp += channeld;
				sp++;
			}

			sp = sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
		}
	} else if (channels == 2) {
		inc0 = deltac[1];
		inc1 = deltac[2] + inc0;
		for (j = 0; j < height; j++) {

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				s1 = sp[1];
				dp[0] = s0;
				dp[inc0] = s1;
				dp += inc1;
				sp += 2;
			}

			sp = sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
		}
	} else if (channels == 3) {
		inc0 = deltac[1];
		inc1 = deltac[2] + inc0;
		inc2 = deltac[3] + inc1;
		for (j = 0; j < height; j++) {

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				s1 = sp[1];
				s2 = sp[2];
				dp[0] = s0;
				dp[inc0] = s1;
				dp[inc1] = s2;
				dp += inc2;
				sp += 3;
			}

			sp = sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */

#define	INSERT_U8_12(sd0, dd0, dd1)                              \
	                                                         \
/*                                                               \
 * channel duplicate                                             \
 */                                                              \
	dd0 = vis_fpmerge(vis_read_hi(sd0), vis_read_hi(sd0));   \
	dd1 = vis_fpmerge(vis_read_lo(sd0), vis_read_lo(sd0))

/* *********************************************************** */

/*
 * insert one channel to a 2-channel image.
 * both source and destination image data are 8-byte aligned.
 * dsize is multiple of 8.
 */

void
mlib_v_ImageChannelInsert_U8_12_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_d64 sd0;
	mlib_d64 dd0, dd1;
	mlib_s32 bmask;
	mlib_s32 i;

	bmask = cmask | (cmask << 2) | (cmask << 4) | (cmask << 6);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

#pragma pipeloop(0)
	for (i = 0; i < dsize / 8; i++) {
		sd0 = (*sp++);
		INSERT_U8_12(sd0, dd0, dd1);
		vis_pst_8(dd0, dp++, bmask);
		vis_pst_8(dd1, dp++, bmask);
	}
}

/* *********************************************************** */

/*
 * insert one channel to a 2-channel image.
 */

void
mlib_v_ImageChannelInsert_U8_12_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_u8 *sa, *da;

/* end points in dst */
	mlib_u8 *dend, *dend2;

/* 8-byte aligned start points in dst */
	mlib_d64 *dp;

/* 8-byte aligned start point in src */
	mlib_d64 *sp;

/* 8-byte source data */
	mlib_d64 sd0, sd1;

/* 8-byte destination data */
	mlib_d64 dd0, dd1, dd2, dd3;

/* offset of address in src */
	mlib_s32 soff;

/* offset of address in dst */
	mlib_s32 doff;

/* offset of src over dst */
	mlib_s32 off;

/* edge mask */
	mlib_s32 emask;

/* channel mask */
	mlib_s32 bmask;
	mlib_s32 i, n;

	bmask = cmask | (cmask << 2) | (cmask << 4) | (cmask << 6);

	sa = (void *)src;
	da = dst;

/* prepare the source address */
	sp = (mlib_d64 *)((mlib_addr)sa & (~7));
	soff = ((mlib_addr)sa & 7);

/* prepare the destination addresses */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	doff = ((mlib_addr)da & 7);
	dend = da + dsize * 2 - 1;
	dend2 = dend - 15;

/* calculate the src's offset over dst */
	off = soff * 2 - doff;

	if (doff % 2 != 0) {
		bmask = (~bmask) & 0xff;
	}

/* src and dst have same alignment */
	if (off == 0) {

/* load 8 bytes */
		sd0 = vis_ld_d64_nf(sp);
		sp++;

/* insert, including some garbage at the start point */
		INSERT_U8_12(sd0, dd0, dd1);

/* store 16 bytes result */
		emask = vis_edge8(da, dend);
		vis_pst_8(dd0, dp++, emask & bmask);

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd1, dp++, emask & bmask);
		}

		if ((mlib_addr)dp <= (mlib_addr)dend2) {
			n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 16 + 1;

/* 8-pixel column loop, emask not needed */

#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				sd0 = (*sp++);
				INSERT_U8_12(sd0, dd0, dd1);
				vis_pst_8(dd0, dp++, bmask);
				vis_pst_8(dd1, dp++, bmask);
			}
		}

/* end point handling */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd0 = vis_ld_d64_nf(sp);
			INSERT_U8_12(sd0, dd0, dd1);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd0, dp++, emask & bmask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd1, dp, emask & bmask);
			}
		}
	} else if (off < 0) {
		vis_alignaddr((void *)off, 0);

/* generate edge mask for the start point */
		emask = vis_edge8(da, dend);

/* load 8 bytes */
		sd0 = vis_ld_d64_nf(sp);
		sp++;

/* insert and store 16 bytes */
		INSERT_U8_12(sd0, dd0, dd1);
		vis_pst_8(vis_faligndata(dd0, dd0), dp++, emask & bmask);

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			vis_pst_8(vis_faligndata(dd0, dd1), dp++,
			    emask & bmask);
		}

		if ((mlib_addr)dp <= (mlib_addr)dend2) {
			n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 16 + 1;

/* 8-pixel column loop, emask not needed */

#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				dd2 = dd1;
				sd0 = (*sp++);
				INSERT_U8_12(sd0, dd0, dd1);
				vis_pst_8(vis_faligndata(dd2, dd0), dp++,
				    bmask);
				vis_pst_8(vis_faligndata(dd0, dd1), dp++,
				    bmask);
			}
		}

/* end point handling */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			dd2 = dd1;
			sd0 = vis_ld_d64_nf(sp);
			INSERT_U8_12(sd0, dd0, dd1);
			vis_pst_8(vis_faligndata(dd2, dd0), dp++,
			    emask & bmask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				vis_pst_8(vis_faligndata(dd0, dd1), dp,
				    emask & bmask);
			}
		}
	} else if (off < 8) {
		vis_alignaddr((void *)off, 0);

/* generate edge mask for the start point */
		emask = vis_edge8(da, dend);

/* load 16 bytes */
		sd0 = vis_ld_d64_nf(sp);
		sd1 = vis_ld_d64_nf(sp + 1);
		sp += 2;

/* insert and store 16 bytes */
		INSERT_U8_12(sd0, dd0, dd1);
		INSERT_U8_12(sd1, dd2, dd3);
		vis_pst_8(vis_faligndata(dd0, dd1), dp++, emask & bmask);

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			vis_pst_8(vis_faligndata(dd1, dd2), dp++,
			    emask & bmask);
		}

		if ((mlib_addr)dp <= (mlib_addr)dend2) {
			n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 16 + 1;

/* 8-pixel column loop, emask not needed */

#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				dd0 = dd2;
				dd1 = dd3;
				sd1 = (*sp++);
				INSERT_U8_12(sd1, dd2, dd3);
				vis_pst_8(vis_faligndata(dd0, dd1), dp++,
				    bmask);
				vis_pst_8(vis_faligndata(dd1, dd2), dp++,
				    bmask);
			}
		}

/* end point handling */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			dd0 = dd2;
			dd1 = dd3;
			sd1 = vis_ld_d64_nf(sp);
			dd2 = vis_fpmerge(vis_read_hi(sd1), vis_read_hi(sd1));
			vis_pst_8(vis_faligndata(dd0, dd1), dp++,
			    emask & bmask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				vis_pst_8(vis_faligndata(dd1, dd2), dp,
				    emask & bmask);
			}
		}
	} else {
/* (off >= 8) */
		vis_alignaddr((void *)off, 0);

/* generate edge mask for the start point */
		emask = vis_edge8(da, dend);

/* load 16 bytes */
		sd0 = vis_ld_d64_nf(sp);
		sd1 = vis_ld_d64_nf(sp + 1);
		sp += 2;

/* insert and store 16 bytes */
		dd1 = vis_fpmerge(vis_read_lo(sd0), vis_read_lo(sd0));
		INSERT_U8_12(sd1, dd2, dd3);
		vis_pst_8(vis_faligndata(dd1, dd2), dp++, emask & bmask);

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			vis_pst_8(vis_faligndata(dd2, dd3), dp++,
			    emask & bmask);
		}

		if ((mlib_addr)dp <= (mlib_addr)dend2) {
			n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 16 + 1;

/* 8-pixel column loop, emask not needed */

#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				dd1 = dd3;
				sd1 = (*sp++);
				INSERT_U8_12(sd1, dd2, dd3);
				vis_pst_8(vis_faligndata(dd1, dd2), dp++,
				    bmask);
				vis_pst_8(vis_faligndata(dd2, dd3), dp++,
				    bmask);
			}
		}

/* end point handling */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			dd1 = dd3;
			sd1 = vis_ld_d64_nf(sp);
			INSERT_U8_12(sd1, dd2, dd3);
			vis_pst_8(vis_faligndata(dd1, dd2), dp++,
			    emask & bmask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				vis_pst_8(vis_faligndata(dd2, dd3), dp,
				    emask & bmask);
			}
		}
	}
}

/* *********************************************************** */

/*
 * insert one channel to a 2-channel image.
 */

void
mlib_v_ImageChannelInsert_U8_12(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_u8 *sa, *da;
	mlib_u8 *sl, *dl;
	mlib_s32 j;

	sa = sl = (void *)src;
	da = dl = dst;

#pragma pipeloop(0)
	for (j = 0; j < ysize; j++) {
		mlib_v_ImageChannelInsert_U8_12_D1(sa, da, xsize, cmask);
		sa = sl += slb;
		da = dl += dlb;
	}
}

/* *********************************************************** */

static const mlib_s32 mlib_off_U8[] = {
	0x00000000, 0x11111111, 0x22222222, 0x33333333,
	0x44444444, 0x55555555, 0x66666666, 0x77777777
};

/* *********************************************************** */

static const mlib_s32 mlib_bmask_U8_31[] = {
	0x25036147, 0x50361472, 0, 0x03614725
};

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_U8_13(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_u8 *sa, *da, *sl, *dl, *dend;
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1, dd;
	mlib_s32 i, j, soff, doff, n, cmask1, emask;
	mlib_s32 sm0, sm1, sm2, shift;

	if ((xsize == slb) && (3 * slb == dlb)) {
		xsize *= ysize;
		ysize = 1;
	}

	xsize *= 3;

	cmask |= (cmask << 3);
	cmask |= (cmask << 6);
	cmask |= (cmask << 12);

	sa = sl = (void *)src;
	da = dl = dst;

	for (j = 0; j < ysize; j++) {
		n = xsize;
		dend = da + xsize - 1;
		cmask1 = cmask;

		dp = (mlib_d64 *)((mlib_addr)da & (~7));
		doff = (da - (mlib_u8 *)dp) & 7;
		cmask1 >>= doff;
		shift = (cmask1 & 7) - 1;

		if (shift == 3)
			shift = 2;

		sa -= (doff + shift) / 3;

		sp = (mlib_d64 *)((mlib_addr)sa & (~7));
		soff = (sa - (mlib_u8 *)sp) & 7;

		sm0 = cmask1 >> 1;
		sm1 = cmask1 >> 2;
		sm2 = cmask1;

		vis_write_bmask(mlib_bmask_U8_31[(cmask1 & 7) - 1],
		    mlib_off_U8[soff]);

		sd0 = vis_ld_d64_nf(sp);
		sp++;

		if (doff != 0) {
/* generate edge mask for the start point */
			emask = vis_edge8(da, dend);
			sd1 = vis_ld_d64_nf(sp);
			sp++;
			dd = vis_bshuffle(sd0, sd1);
			vis_pst_8(dd, dp++, sm0 & emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp++, sm1 & emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge8(dp, dend);
					vis_pst_8(dd, dp++, sm2 & emask);
				}
			}

			sd0 = sd1;
			n -= (24 - doff);
		}
#pragma pipeloop(0)
		for (i = 0; i < n - 23; i += 24) {
			sd1 = (*sp++);
			dd = vis_bshuffle(sd0, sd1);
			vis_pst_8(dd, dp, sm0);
			vis_pst_8(dd, dp + 1, sm1);
			vis_pst_8(dd, dp + 2, sm2);
			sd0 = sd1;
			dp += 3;
		}

/* end point handling */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			sd1 = vis_ld_d64_nf(sp);
			dd = vis_bshuffle(sd0, sd1);
			vis_pst_8(dd, dp++, sm0 & emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp++, sm1 & emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge8(dp, dend);
					vis_pst_8(dd, dp, sm2 & emask);
				}
			}
		}

		sa = sl += slb;
		da = dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_U8_13_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_v_ImageChannelInsert_U8_13(src, 0, dst, 0, dsize, 1, cmask);
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_U8_13_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_v_ImageChannelInsert_U8_13(src, 0, dst, 0, dsize, 1, cmask);
}

/* *********************************************************** */

static const mlib_s32 mlib_bmask_U8_41[] = {
	0x35702461, 0x57024613, 0, 0x70246135, 0, 0, 0, 0x02461357
};

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_U8_14_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_u8 *dend;
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1;
	mlib_d64 dd0, dd1, dd2, dd3;
	mlib_s32 i, soff, doff, emask;
	mlib_s32 shift;

	dsize *= 4;

	cmask |= (cmask << 4);
	cmask |= (cmask << 8);

	dend = dst + dsize - 1;

	dp = (mlib_d64 *)((mlib_addr)dst & (~7));
	doff = (dst - (mlib_u8 *)dp) & 7;
	cmask >>= doff;
	shift = (cmask & 0xF) - 1;

	if (shift == 3)
		shift = 2;

	if (shift == 7)
		shift = 3;

	src -= ((doff + shift) >> 2);

	sp = (mlib_d64 *)((mlib_addr)src & (~7));
	soff = (src - (mlib_u8 *)sp) & 7;

	vis_write_gsr64((((mlib_u64)mlib_bmask_U8_41[(cmask & 0xF) - 1] +
	    mlib_off_U8[soff]) << 32) + 1);

	sd0 = vis_ld_d64_nf(sp);
	sp++;

	if (doff != 0) {
/* generate edge mask for the start point */
		emask = vis_edge8(dst, dend);
		sd1 = vis_ld_d64_nf(sp);
		sp++;
		dd0 = vis_bshuffle(sd0, sd1);
		vis_pst_8(dd0, dp++, cmask & emask);

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			dd1 = vis_faligndata(dd0, dd0);
			vis_pst_8(dd1, dp++, cmask & emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				dd2 = vis_faligndata(dd1, dd1);
				vis_pst_8(dd2, dp++, cmask & emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge8(dp, dend);
					dd3 = vis_faligndata(dd2, dd2);
					vis_pst_8(dd3, dp++, cmask & emask);
				}
			}
		}

		sd0 = sd1;
		dsize -= (32 - doff);
	}
#pragma pipeloop(0)
	for (i = 0; i < dsize - 31; i += 32) {
		sd1 = (*sp++);
		dd0 = vis_bshuffle(sd0, sd1);
		dd1 = vis_faligndata(dd0, dd0);
		dd2 = vis_faligndata(dd1, dd1);
		dd3 = vis_faligndata(dd2, dd2);
		vis_pst_8(dd0, dp++, cmask);
		vis_pst_8(dd1, dp++, cmask);
		vis_pst_8(dd2, dp++, cmask);
		vis_pst_8(dd3, dp++, cmask);
		sd0 = sd1;
	}

/* end point handling */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge8(dp, dend);
		sd1 = vis_ld_d64_nf(sp);
		dd0 = vis_bshuffle(sd0, sd1);
		vis_pst_8(dd0, dp++, cmask & emask);

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			dd1 = vis_faligndata(dd0, dd0);
			vis_pst_8(dd1, dp++, cmask & emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				dd2 = vis_faligndata(dd1, dd1);
				vis_pst_8(dd2, dp++, cmask & emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge8(dp, dend);
					dd3 = vis_faligndata(dd2, dd2);
					vis_pst_8(dd3, dp, cmask & emask);
				}
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_U8_14_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_v_ImageChannelInsert_U8_14_D1(src, dst, dsize, cmask);
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_U8_14(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_u8 *sa, *da;
	mlib_u8 *sl, *dl;
	mlib_s32 j;

	sa = sl = (void *)src;
	da = dl = dst;

#pragma pipeloop(0)
	for (j = 0; j < ysize; j++) {
		mlib_v_ImageChannelInsert_U8_14_D1(sa, da, xsize, cmask);
		sa = sl += slb;
		da = dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_S16_12_A8D1X4(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	cmask = 12 >> ((cmask & 1) << 1);
	mlib_v_ImageChannelInsert_U8_24_A8D1X8((mlib_u8 *)src, (mlib_u8 *)dst,
	    dsize, cmask);
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_S16_12_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	cmask = 12 >> ((cmask & 1) << 1);
	mlib_v_ImageChannelInsert_U8_24_D1((mlib_u8 *)src, (mlib_u8 *)dst,
	    dsize, cmask);
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_S16_12(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	cmask = 12 >> ((cmask & 1) << 1);
	mlib_v_ImageChannelInsert_U8_24((mlib_u8 *)src, slb, (mlib_u8 *)dst,
	    dlb, xsize, ysize, cmask);
}

/* *********************************************************** */

static const mlib_s32 mlib_bmask_S16_31[] = {
	0x45230167, 0x23016745, 0, 0x01674523
};

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_S16_13_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_s16 *sa, *da, *dend;
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1, dd;
	mlib_s32 i, soff, doff, emask;
	mlib_s32 sm0, sm1, sm2, shift;

	dsize *= 3;

	cmask |= (cmask << 3);
	cmask |= (cmask << 6);
	cmask |= (cmask << 12);

	sa = (void *)src;
	da = dst;

	dend = da + dsize - 1;

	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	doff = (da - (mlib_s16 *)dp) & 3;
	cmask >>= doff;
	shift = (cmask & 7) - 1;

	if (shift == 3)
		shift = 2;

	sa -= (doff + shift) / 3;

	sp = (mlib_d64 *)((mlib_addr)sa & (~7));
	soff = (sa - (mlib_s16 *)sp) & 3;

	sm0 = cmask >> 2;
	sm1 = cmask >> 1;
	sm2 = cmask;

	vis_write_bmask(mlib_bmask_S16_31[(cmask & 7) - 1],
	    mlib_off_U8[2 * soff]);

	sd0 = vis_ld_d64_nf(sp);
	sp++;

	if (doff != 0) {
/* generate edge mask for the start point */
		emask = vis_edge16(da, dend);
		sd1 = vis_ld_d64_nf(sp);
		sp++;
		dd = vis_bshuffle(sd0, sd1);
		vis_pst_16(dd, dp++, sm0 & emask);

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp++, sm1 & emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp++, sm2 & emask);
			}
		}

		sd0 = sd1;
		dsize -= (12 - doff);
	}
#pragma pipeloop(0)
	for (i = 0; i < dsize - 11; i += 12) {
		sd1 = (*sp++);
		dd = vis_bshuffle(sd0, sd1);
		vis_pst_16(dd, dp, sm0);
		vis_pst_16(dd, dp + 1, sm1);
		vis_pst_16(dd, dp + 2, sm2);
		sd0 = sd1;
		dp += 3;
	}

/* end point handling */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge16(dp, dend);
		sd1 = vis_ld_d64_nf(sp);
		dd = vis_bshuffle(sd0, sd1);
		vis_pst_16(dd, dp++, sm0 & emask);

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp++, sm1 & emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, sm2 & emask);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_S16_13_A8D1X4(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_v_ImageChannelInsert_S16_13_D1(src, dst, dsize, cmask);
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_S16_13(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_s16 *sa, *da;
	mlib_s16 *sl, *dl;
	mlib_s32 j;

	sa = sl = (void *)src;
	da = dl = dst;

#pragma pipeloop(0)
	for (j = 0; j < ysize; j++) {
		mlib_v_ImageChannelInsert_S16_13_D1(sa, da, xsize, cmask);
		sa = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		da = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_S16_14_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_s16 *sa, *da;

/* end point in destination */
	mlib_s16 *dend;

/* 8-byte aligned start points in src */
	mlib_d64 *sp;

/* 8-byte registers for source data */
	mlib_d64 sd0, sd1, sd;

/* offset of address alignment in src */
	mlib_s32 off;
	mlib_s32 bmask;
	mlib_s32 i;

	sa = (void *)src;
/* 8, 4,2, 1 -> 0, 1,2, 3 */
	da = dst + (6 / cmask + 1) / 2;

/* prepare the src address */
	sp = (mlib_d64 *)((mlib_addr)sa & (~7));
	off = (mlib_addr)sa & 7;

	bmask = 0x23456701 + mlib_off_U8[off];

	dend = da + dsize * 4 - 1;

	sd0 = vis_ld_d64_nf(sp);
	sp++;

	vis_write_gsr64((((mlib_u64)bmask) << 32) + 2);

#pragma pipeloop(0)
	for (i = 0; i < dsize - 3; i += 4) {
		sd1 = (*sp++);
		sd = vis_bshuffle(sd0, sd1);
		vis_st_u16(sd, da);
		da += 4;
		vis_st_u16(sd = vis_faligndata(sd, sd), da);
		da += 4;
		vis_st_u16(sd = vis_faligndata(sd, sd), da);
		da += 4;
		vis_st_u16(sd = vis_faligndata(sd, sd), da);
		da += 4;
		sd0 = sd1;
	}

/* right end handling */

	if ((mlib_addr)da <= (mlib_addr)dend) {
		sd1 = vis_ld_d64_nf(sp);
		sd = vis_bshuffle(sd0, sd1);
		vis_st_u16(sd, da);
		da += 4;

		if ((mlib_addr)da <= (mlib_addr)dend) {
			vis_st_u16(sd = vis_faligndata(sd, sd), da);
			da += 4;

			if ((mlib_addr)da <= (mlib_addr)dend) {
				vis_st_u16(sd = vis_faligndata(sd, sd), da);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_S16_14_A8D1X4(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_v_ImageChannelInsert_S16_14_D1(src, dst, dsize, cmask);
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_S16_14(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_s16 *sa, *da;
	mlib_s16 *sl, *dl;
	mlib_s32 j;

	sa = sl = (void *)src;
	da = dl = dst;

#pragma pipeloop(0)
	for (j = 0; j < ysize; j++) {
		mlib_v_ImageChannelInsert_S16_14_D1(sa, da, xsize, cmask);
		sa = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		da = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */
