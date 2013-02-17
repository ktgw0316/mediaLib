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

#pragma ident	"@(#)mlib_v_ImageChannelExtract_1.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *  mlib_v_ImageChannelExtract_U8_21_A8D1X8
 *  mlib_v_ImageChannelExtract_U8_21_A8D2X8
 *  mlib_v_ImageChannelExtract_U8_21_D1
 *  mlib_v_ImageChannelExtract_U8_21
 *  mlib_v_ImageChannelExtract_U8_31_A8D1X8
 *  mlib_v_ImageChannelExtract_U8_31_A8D2X8
 *  mlib_v_ImageChannelExtract_U8_31_D1
 *  mlib_v_ImageChannelExtract_U8_31
 *  mlib_v_ImageChannelExtract_U8_41_A8D1X8
 *  mlib_v_ImageChannelExtract_U8_41_A8D2X8
 *  mlib_v_ImageChannelExtract_U8_41_D1
 *  mlib_v_ImageChannelExtract_U8_41
 *  mlib_v_ImageChannelExtract_S16_21_A8D1X4
 *  mlib_v_ImageChannelExtract_S16_21_A8D2X4
 *  mlib_v_ImageChannelExtract_S16_21_D1
 *  mlib_v_ImageChannelExtract_S16_21
 *  mlib_v_ImageChannelExtract_S16_31_A8D1X4
 *  mlib_v_ImageChannelExtract_S16_31_A8D2X4
 *  mlib_v_ImageChannelExtract_S16_31_D1
 *  mlib_v_ImageChannelExtract_S16_31
 *  mlib_v_ImageChannelExtract_S16_41_A8D1X4
 *  mlib_v_ImageChannelExtract_S16_41_A8D2X4
 *  mlib_v_ImageChannelExtract_S16_41_D1
 *  mlib_v_ImageChannelExtract_S16_41
 *
 * ARGUMENT
 *      src    pointer to source image data
 *      dst    pointer to destination image data
 *      slb    source image line stride in bytes
 *      dlb   destination image line stride in bytes
 *      dsize  image data size in pixels
 *      xsize  image width in pixels
 *      ysize  image height in lines
 *      cmask channel mask
 *
 * DESCRIPTION
 *      Extract the one selected channel of the source image into the
 *      1-channel destination image.
 *
 * NOTE
 *      These functions are separated from mlib_ImageChannelExtract.c
 *      for loop unrolling and structure clarity.
 *      I was hope to see you in any case, but it seems you doesn't?:(
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageChannelExtract.h>
#include <mlib_v_ImageChannelExtract_2.h>

/* *********************************************************** */

#define	CHANNELEXTRACT_U8_21(sd0, sd1, dd)                      \
	dd = vis_bshuffle(sd0, sd1)

/* *********************************************************** */

static const mlib_s32 mlib_bmask_U8_21[] = { 0x13579BDF, 0x02468ACE };

/* *********************************************************** */

/*
 * extract one channel from a 2-channel image.
 * both source and destination image data are 8-byte aligned.
 * xsize is multiple of 8.
 */

void
mlib_v_ImageChannelExtract_U8_21_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1;
	mlib_s32 i;

	vis_write_bmask(mlib_bmask_U8_21[cmask - 1], 0);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

#pragma pipeloop(0)
	for (i = 0; i < dsize - 15; i += 16) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		(*dp++) = vis_bshuffle(sd0, sd1);

		sd0 = (*sp++);
		sd1 = (*sp++);
		(*dp++) = vis_bshuffle(sd0, sd1);
	}

	if (i < dsize) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		(*dp++) = vis_bshuffle(sd0, sd1);
	}
}

/* *********************************************************** */

/*
 * extract one channel from a 2-channel image.
 * both source and destination image data are 8-byte aligned.
 * xsize is multiple of 8.
 */

void
mlib_v_ImageChannelExtract_U8_21_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_d64 *sl, *dl;
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1;
	mlib_s32 i, j;

	vis_write_bmask(mlib_bmask_U8_21[cmask - 1], 0);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize - 15; i += 16) {
			sd0 = (*sp++);
			sd1 = (*sp++);
			(*dp++) = vis_bshuffle(sd0, sd1);

			sd0 = (*sp++);
			sd1 = (*sp++);
			(*dp++) = vis_bshuffle(sd0, sd1);
		}

		if (i < xsize) {
			sd0 = (*sp++);
			sd1 = (*sp++);
			(*dp++) = vis_bshuffle(sd0, sd1);
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/*
 * extract one channel from a 2-channel image.
 */

void
mlib_v_ImageChannelExtract_U8_21(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_u8 *sl, *dl, *sa, *da;

/* end points in dst */
	mlib_u8 *dend, *dend2;

/* 8-byte aligned start points in dst */
	mlib_d64 *dp;

/* 8-byte aligned start point in src */
	mlib_d64 *sp;

/* 8-byte source data */
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_d64 dd0, dd1;
	mlib_s32 soff, doff, off, ind;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 i, j, n;

	sa = sl = (void *)src;
	da = dl = dst;

	for (j = 0; j < ysize; j++) {

/* prepare the source address */
		sp = (mlib_d64 *)((mlib_addr)sa & (~7));
		soff = ((mlib_addr)sa & 7);

/* prepare the destination addresses */
		dp = (mlib_d64 *)((mlib_addr)da & (~7));
		doff = ((mlib_addr)da & 7);
		dend = da + xsize - 1;
		dend2 = dend - 7;

/* calculate the src's offset over dst */

		if (cmask == 2) {
			off = soff / 2 - doff;
		} else {
			off = (soff + 1) / 2 - doff;
		}

		ind = cmask;

		if (soff & 1)
			ind ^= 3;
		vis_write_bmask(mlib_bmask_U8_21[ind - 1], 0);

/* src and dst have same alignment */
		if (off == 0) {

/* generate edge mask for the start point */
			emask = vis_edge8(da, dend);

/* load 16 bytes */
			sd0 = vis_ld_d64_nf(sp);
			sd1 = vis_ld_d64_nf(sp + 1);
			sp += 2;

/* extract, including some garbage at the start point */
			CHANNELEXTRACT_U8_21(sd0, sd1, dd0);

/* store 8 bytes result */
			vis_pst_8(dd0, dp++, emask);

			if ((mlib_addr)dp <= (mlib_addr)dend2) {
				n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 8 + 1;

/* 8-pixel column loop, emask not needed */

#pragma pipeloop(0)
				for (i = 0; i < n; i++) {
					sd0 = (*sp++);
					sd1 = (*sp++);
					CHANNELEXTRACT_U8_21(sd0, sd1, dd0);
					(*dp++) = dd0;
				}
			}

/* end point handling */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				sd0 = vis_ld_d64_nf(sp);
				sd1 = vis_ld_d64_nf(sp + 1);
				CHANNELEXTRACT_U8_21(sd0, sd1, dd0);
				vis_pst_8(dd0, dp, emask);
			}
		} else {
			vis_alignaddr((void *)off, 0);

/* generate edge mask for the start point */
			emask = vis_edge8(da, dend);

			if (off < 0) {
/* load 16 bytes */
				sd2 = vis_ld_d64_nf(sp);
				sd3 = vis_ld_d64_nf(sp + 1);
				sp += 2;

/* extract and store 8 bytes */
				CHANNELEXTRACT_U8_21(sd2, sd3, dd1);
				vis_pst_8(vis_faligndata(dd1, dd1), dp++,
				    emask);
			} else {
/* load 32 bytes */
				sd0 = vis_ld_d64_nf(sp);
				sd1 = vis_ld_d64_nf(sp + 1);
				sd2 = vis_ld_d64_nf(sp + 2);
				sd3 = vis_ld_d64_nf(sp + 3);
				sp += 4;

/* extract and store 8 bytes */
				CHANNELEXTRACT_U8_21(sd0, sd1, dd0);
				CHANNELEXTRACT_U8_21(sd2, sd3, dd1);
				vis_pst_8(vis_faligndata(dd0, dd1), dp++,
				    emask);
			}

			if ((mlib_addr)dp <= (mlib_addr)dend2) {
				n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 8 + 1;

/* 8-pixel column loop, emask not needed */

#pragma pipeloop(0)
				for (i = 0; i < n; i++) {
					dd0 = dd1;
					sd2 = (*sp++);
					sd3 = (*sp++);
					CHANNELEXTRACT_U8_21(sd2, sd3, dd1);
					(*dp++) = vis_faligndata(dd0, dd1);
				}
			}

/* end point handling */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				dd0 = dd1;
				sd2 = vis_ld_d64_nf(sp);
				sd3 = vis_ld_d64_nf(sp + 1);
				CHANNELEXTRACT_U8_21(sd2, sd3, dd1);
				vis_pst_8(vis_faligndata(dd0, dd1), dp, emask);
			}
		}

		sa = sl += slb;
		da = dl += dlb;
	}
}

/* *********************************************************** */

/*
 * extract one channel from a 2-channel image.
 */

void
mlib_v_ImageChannelExtract_U8_21_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_v_ImageChannelExtract_U8_21(src, 0, dst, 0, dsize, 1, cmask);
}

/* *********************************************************** */

static const mlib_s32 mlib_bmask_U8_31[] = {
	0x258B69CF, 0x147A58BE, 0, 0x036947AD,
};

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_U8_31_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1, sd2;
	mlib_d64 dd1, dd2;
	mlib_s32 i;

	vis_write_bmask(mlib_bmask_U8_31[cmask - 1], 0);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

#pragma pipeloop(0)
	for (i = 0; i < dsize - 31; i += 32) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		dd1 = vis_bshuffle(sd0, sd1);
		dd2 = vis_bshuffle(sd1, sd2);
		(*dp++) = vis_freg_pair(vis_read_hi(dd1), vis_read_lo(dd2));

		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		dd1 = vis_bshuffle(sd0, sd1);
		dd2 = vis_bshuffle(sd1, sd2);
		(*dp++) = vis_freg_pair(vis_read_hi(dd1), vis_read_lo(dd2));

		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		dd1 = vis_bshuffle(sd0, sd1);
		dd2 = vis_bshuffle(sd1, sd2);
		(*dp++) = vis_freg_pair(vis_read_hi(dd1), vis_read_lo(dd2));

		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		dd1 = vis_bshuffle(sd0, sd1);
		dd2 = vis_bshuffle(sd1, sd2);
		(*dp++) = vis_freg_pair(vis_read_hi(dd1), vis_read_lo(dd2));
	}

#pragma pipeloop(0)
	for (; i < dsize - 7; i += 8) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		dd1 = vis_bshuffle(sd0, sd1);
		dd2 = vis_bshuffle(sd1, sd2);
		(*dp++) = vis_freg_pair(vis_read_hi(dd1), vis_read_lo(dd2));
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_U8_31_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_d64 *sl, *dl;
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1, sd2;
	mlib_d64 dd1, dd2;
	mlib_s32 i, j;

	vis_write_bmask(mlib_bmask_U8_31[cmask - 1], 0);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize - 31; i += 32) {
			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			dd1 = vis_bshuffle(sd0, sd1);
			dd2 = vis_bshuffle(sd1, sd2);
			(*dp++) =
			    vis_freg_pair(vis_read_hi(dd1), vis_read_lo(dd2));

			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			dd1 = vis_bshuffle(sd0, sd1);
			dd2 = vis_bshuffle(sd1, sd2);
			(*dp++) =
			    vis_freg_pair(vis_read_hi(dd1), vis_read_lo(dd2));

			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			dd1 = vis_bshuffle(sd0, sd1);
			dd2 = vis_bshuffle(sd1, sd2);
			(*dp++) =
			    vis_freg_pair(vis_read_hi(dd1), vis_read_lo(dd2));

			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			dd1 = vis_bshuffle(sd0, sd1);
			dd2 = vis_bshuffle(sd1, sd2);
			(*dp++) =
			    vis_freg_pair(vis_read_hi(dd1), vis_read_lo(dd2));
		}

#pragma pipeloop(0)
		for (; i < xsize - 7; i += 8) {
			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			dd1 = vis_bshuffle(sd0, sd1);
			dd2 = vis_bshuffle(sd1, sd2);
			(*dp++) =
			    vis_freg_pair(vis_read_hi(dd1), vis_read_lo(dd2));
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_U8_31_D1_na(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask,
    mlib_s32 off)
{
	mlib_u8 *dend;
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1, sd2;
	mlib_d64 dd0, dd1, dd2, dd3;
	mlib_s32 i, emask;

	vis_write_bmask(mlib_bmask_U8_31[cmask - 1], 0);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;
	dend = dst + dsize - 1;

/* load 24 bytes */
	sd0 = vis_ld_d64_nf(sp);
	sd1 = vis_ld_d64_nf(sp + 1);
	sd2 = vis_ld_d64_nf(sp + 2);
	sp += 3;
	dd2 = vis_bshuffle(sd0, sd1);
	dd3 = vis_bshuffle(sd1, sd2);
	dd0 = vis_freg_pair(vis_read_hi(dd2), vis_read_lo(dd3));

	vis_alignaddr((void *)off, 0);

#pragma pipeloop(0)
	for (i = 0; i < dsize - 31; i += 32) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		dd2 = vis_bshuffle(sd0, sd1);
		dd3 = vis_bshuffle(sd1, sd2);
		dd1 = vis_freg_pair(vis_read_hi(dd2), vis_read_lo(dd3));
		(*dp++) = vis_faligndata(dd0, dd1);

		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		dd2 = vis_bshuffle(sd0, sd1);
		dd3 = vis_bshuffle(sd1, sd2);
		dd0 = vis_freg_pair(vis_read_hi(dd2), vis_read_lo(dd3));
		(*dp++) = vis_faligndata(dd1, dd0);

		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		dd2 = vis_bshuffle(sd0, sd1);
		dd3 = vis_bshuffle(sd1, sd2);
		dd1 = vis_freg_pair(vis_read_hi(dd2), vis_read_lo(dd3));
		(*dp++) = vis_faligndata(dd0, dd1);

		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		dd2 = vis_bshuffle(sd0, sd1);
		dd3 = vis_bshuffle(sd1, sd2);
		dd0 = vis_freg_pair(vis_read_hi(dd2), vis_read_lo(dd3));
		(*dp++) = vis_faligndata(dd1, dd0);
	}

#pragma pipeloop(0)
	for (; i < dsize - 7; i += 8) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		dd2 = vis_bshuffle(sd0, sd1);
		dd3 = vis_bshuffle(sd1, sd2);
		dd1 = vis_freg_pair(vis_read_hi(dd2), vis_read_lo(dd3));
		(*dp++) = vis_faligndata(dd0, dd1);
		dd0 = dd1;
	}

/* end point handling */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge8(dp, dend);

		if ((off == 2) && ((emask & 3) != 0)) {
			sd0 = vis_ld_d64_nf(sp);
			sd1 = vis_ld_d64_nf(sp + 1);
			sd2 = vis_ld_d64_nf(sp + 2);
			dd2 = vis_bshuffle(sd0, sd1);
			dd3 = vis_bshuffle(sd1, sd2);
			dd1 = vis_freg_pair(vis_read_hi(dd2), vis_read_lo(dd3));
		}

		dd0 = vis_faligndata(dd0, dd1);
		vis_pst_8(dd0, dp, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_U8_31_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_u8 *sa, *da;

/* end points in dst */
	mlib_u8 *dend;

/* 8-byte aligned start points in dst */
	mlib_d64 *dp;

/* 8-byte aligned start point in src  */
	mlib_d64 *sp, *sp1;

/* 8-byte source data */
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_d64 dd0, dd1, dd2, dd3;

/* offset of address in src */
	mlib_s32 soff, off;

/* offset of address in dst */
	mlib_s32 doff;

/* edge mask */
	mlib_s32 emask;

	sa = (void *)src;
	da = dst;
	dend = da + dsize - 1;

	if (cmask == 1)
		sa += 2;
	else if (cmask == 2)
		sa += 1;

	cmask = 0x124;

	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	doff = ((mlib_addr)da & 7);

	sp = vis_alignaddr(sa, -3 * doff);
	soff = (sa - 3 * doff - (mlib_u8 *)sp) & 7;
	cmask >>= soff;
	cmask &= 7;

	if (doff != 0) {
		vis_write_bmask(mlib_bmask_U8_31[3], 0);
		sp1 = (mlib_d64 *)((mlib_addr)sa & (~7));

/* generate edge mask for the start point */
		emask = vis_edge8(da, dend);

		sd0 = vis_ld_d64_nf(sp1);

		if (sp == sp1)
			sp1++;
		sd1 = vis_ld_d64_nf(sp1);

		if (sp + 1 == sp1)
			sp1++;
		sd2 = vis_ld_d64_nf(sp1);

		if (sp + 2 == sp1)
			sp1++;
		sd3 = vis_ld_d64_nf(sp1);

		sd0 = vis_faligndata(sd0, sd1);
		sd1 = vis_faligndata(sd1, sd2);
		sd2 = vis_faligndata(sd2, sd3);

		dd2 = vis_bshuffle(sd0, sd1);
		dd3 = vis_bshuffle(sd1, sd2);
		dd0 = vis_freg_pair(vis_read_hi(dd2), vis_read_lo(dd3));
		vis_pst_8(dd0, dp++, emask);

		dsize -= (8 - doff);

		if (dsize <= 0)
			dsize = 0;
		sp += 3;
	}

	if (soff < 3) {
		dsize = dsize & (~7);

		mlib_v_ImageChannelExtract_U8_31_A8D1X8((mlib_u8 *)sp,
		    (mlib_u8 *)dp, dsize, cmask);

		dp += (dsize >> 3);
		sp += (dsize >> 3) * 3;

/* end point handling */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			sd0 = vis_ld_d64_nf(sp);
			sd1 = vis_ld_d64_nf(sp + 1);
			sd2 = vis_ld_d64_nf(sp + 2);
			dd1 = vis_bshuffle(sd0, sd1);
			dd2 = vis_bshuffle(sd1, sd2);
			dd0 = vis_freg_pair(vis_read_hi(dd1), vis_read_lo(dd2));
			vis_pst_8(dd0, dp, emask);
		}
	} else {
		off = 1;

		if (soff >= 6)
			off = 2;
		mlib_v_ImageChannelExtract_U8_31_D1_na((mlib_u8 *)sp,
		    (mlib_u8 *)dp, dsize, cmask, off);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_U8_31(
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

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageChannelExtract_U8_31_D1(sa, da, xsize, cmask);
		sa = sl += slb;
		da = dl += dlb;
	}
}

/* *********************************************************** */

static const mlib_s32 mlib_bmask_U8_41[] = {
	0x37BF37BF, 0x26AE26AE, 0, 0x159D159D, 0, 0, 0, 0x048C048C
};

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_U8_41_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_d64 dd1, dd2;
	mlib_s32 i;

	vis_write_bmask(mlib_bmask_U8_41[cmask - 1], 0);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

#pragma pipeloop(0)
	for (i = 0; i < dsize - 31; i += 32) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		sd3 = (*sp++);
		dd1 = vis_bshuffle(sd0, sd1);
		dd2 = vis_bshuffle(sd2, sd3);
		(*dp++) = vis_freg_pair(vis_read_hi(dd1), vis_read_hi(dd2));

		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		sd3 = (*sp++);
		dd1 = vis_bshuffle(sd0, sd1);
		dd2 = vis_bshuffle(sd2, sd3);
		(*dp++) = vis_freg_pair(vis_read_hi(dd1), vis_read_hi(dd2));

		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		sd3 = (*sp++);
		dd1 = vis_bshuffle(sd0, sd1);
		dd2 = vis_bshuffle(sd2, sd3);
		(*dp++) = vis_freg_pair(vis_read_hi(dd1), vis_read_hi(dd2));

		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		sd3 = (*sp++);
		dd1 = vis_bshuffle(sd0, sd1);
		dd2 = vis_bshuffle(sd2, sd3);
		(*dp++) = vis_freg_pair(vis_read_hi(dd1), vis_read_hi(dd2));
	}

#pragma pipeloop(0)
	for (; i < dsize - 7; i += 8) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		sd3 = (*sp++);
		dd1 = vis_bshuffle(sd0, sd1);
		dd2 = vis_bshuffle(sd2, sd3);
		(*dp++) = vis_freg_pair(vis_read_hi(dd1), vis_read_hi(dd2));
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_U8_41_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_d64 *sl, *dl;
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_d64 dd1, dd2;
	mlib_s32 i, j;

	vis_write_bmask(mlib_bmask_U8_41[cmask - 1], 0);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize - 31; i += 32) {
			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			sd3 = (*sp++);
			dd1 = vis_bshuffle(sd0, sd1);
			dd2 = vis_bshuffle(sd2, sd3);
			(*dp++) =
			    vis_freg_pair(vis_read_hi(dd1), vis_read_hi(dd2));

			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			sd3 = (*sp++);
			dd1 = vis_bshuffle(sd0, sd1);
			dd2 = vis_bshuffle(sd2, sd3);
			(*dp++) =
			    vis_freg_pair(vis_read_hi(dd1), vis_read_hi(dd2));

			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			sd3 = (*sp++);
			dd1 = vis_bshuffle(sd0, sd1);
			dd2 = vis_bshuffle(sd2, sd3);
			(*dp++) =
			    vis_freg_pair(vis_read_hi(dd1), vis_read_hi(dd2));

			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			sd3 = (*sp++);
			dd1 = vis_bshuffle(sd0, sd1);
			dd2 = vis_bshuffle(sd2, sd3);
			(*dp++) =
			    vis_freg_pair(vis_read_hi(dd1), vis_read_hi(dd2));
		}

#pragma pipeloop(0)
		for (; i < xsize - 7; i += 8) {
			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			sd3 = (*sp++);
			dd1 = vis_bshuffle(sd0, sd1);
			dd2 = vis_bshuffle(sd2, sd3);
			(*dp++) =
			    vis_freg_pair(vis_read_hi(dd1), vis_read_hi(dd2));
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_U8_41_D1_na(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_u8 *dend;
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_d64 dd0, dd1, dd2, dd3;
	mlib_s32 i, emask;

	vis_write_bmask(mlib_bmask_U8_41[cmask - 1], 0);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;
	dend = dst + dsize - 1;

	sd0 = vis_ld_d64_nf(sp);
	sd1 = vis_ld_d64_nf(sp + 1);
	sd2 = vis_ld_d64_nf(sp + 2);
	sd3 = vis_ld_d64_nf(sp + 3);
	sp += 4;
	dd2 = vis_bshuffle(sd0, sd1);
	dd3 = vis_bshuffle(sd2, sd3);
	dd0 = vis_freg_pair(vis_read_hi(dd2), vis_read_hi(dd3));

	vis_alignaddr((void *)0, 1);

#pragma pipeloop(0)
	for (i = 0; i < dsize - 31; i += 32) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		sd3 = (*sp++);
		dd2 = vis_bshuffle(sd0, sd1);
		dd3 = vis_bshuffle(sd2, sd3);
		dd1 = vis_freg_pair(vis_read_hi(dd2), vis_read_hi(dd3));
		(*dp++) = vis_faligndata(dd0, dd1);

		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		sd3 = (*sp++);
		dd2 = vis_bshuffle(sd0, sd1);
		dd3 = vis_bshuffle(sd2, sd3);
		dd0 = vis_freg_pair(vis_read_hi(dd2), vis_read_hi(dd3));
		(*dp++) = vis_faligndata(dd1, dd0);

		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		sd3 = (*sp++);
		dd2 = vis_bshuffle(sd0, sd1);
		dd3 = vis_bshuffle(sd2, sd3);
		dd1 = vis_freg_pair(vis_read_hi(dd2), vis_read_hi(dd3));
		(*dp++) = vis_faligndata(dd0, dd1);

		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		sd3 = (*sp++);
		dd2 = vis_bshuffle(sd0, sd1);
		dd3 = vis_bshuffle(sd2, sd3);
		dd0 = vis_freg_pair(vis_read_hi(dd2), vis_read_hi(dd3));
		(*dp++) = vis_faligndata(dd1, dd0);
	}

#pragma pipeloop(0)
	for (; i < dsize - 7; i += 8) {
		sd0 = vis_ld_d64_nf(sp);
		sd1 = vis_ld_d64_nf(sp + 1);
		sd2 = vis_ld_d64_nf(sp + 2);
		sd3 = vis_ld_d64_nf(sp + 3);
		sp += 4;
		dd2 = vis_bshuffle(sd0, sd1);
		dd3 = vis_bshuffle(sd2, sd3);
		dd1 = vis_freg_pair(vis_read_hi(dd2), vis_read_hi(dd3));
		(*dp++) = vis_faligndata(dd0, dd1);
		dd0 = dd1;
	}

/* end point handling */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge8(dp, dend);
		dd0 = vis_faligndata(dd0, dd0);
		vis_pst_8(dd0, dp, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_U8_41_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_u8 *sa, *da;

/* end points in dst */
	mlib_u8 *dend;

/* 8-byte aligned start points in dst */
	mlib_d64 *dp;

/* 8-byte aligned start point in src  */
	mlib_d64 *sp, *sp1;

/* 8-byte source data */
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_d64 dd0, dd2, dd3;

/* offset of address in src */
	mlib_s32 soff, soff1;

/* offset of address in dst */
	mlib_s32 doff;

/* edge mask */
	mlib_s32 emask;

	sa = (void *)src;
	da = dst;
	dend = da + dsize - 1;

	if (cmask == 1)
		sa += 3;
	else if (cmask == 2)
		sa += 2;
	else if (cmask == 4)
		sa += 1;

	cmask = 8;

	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	doff = ((mlib_addr)da & 7);

	sp = (mlib_d64 *)((mlib_addr)(sa - 4 * doff) & (~7));
	soff = (sa - 4 * doff - (mlib_u8 *)sp) & 7;
	cmask >>= (soff & 3);
	vis_write_bmask(mlib_bmask_U8_41[cmask - 1], 0);

	if (doff != 0) {
		sp1 = (mlib_d64 *)((mlib_addr)sa & (~7));
		soff1 = (sa - (mlib_u8 *)sp1) & 7;

/* generate edge mask for the start point */
		emask = vis_edge8(da, dend);

		if (soff1 >= 4) {
			vis_alignaddr(sp, 9 - doff);
		} else {
			vis_alignaddr(sp, 8 - doff);
		}

		sd0 = vis_ld_d64_nf(sp1);
		sd1 = vis_ld_d64_nf(sp1 + 1);
		sd2 = vis_ld_d64_nf(sp1 + 2);
		sd3 = vis_ld_d64_nf(sp1 + 3);
		sp1 += 4;

		dd2 = vis_bshuffle(sd0, sd1);
		dd3 = vis_bshuffle(sd2, sd3);
		dd0 = vis_freg_pair(vis_read_hi(dd2), vis_read_hi(dd3));
		dd0 = vis_faligndata(dd0, dd0);
		vis_pst_8(dd0, dp++, emask);

		dsize -= (8 - doff);

		if (dsize <= 0)
			dsize = 0;
		sp += 4;
	}

	if (soff < 4) {
		dsize = dsize & (~7);

		mlib_v_ImageChannelExtract_U8_41_A8D1X8((mlib_u8 *)sp,
		    (mlib_u8 *)dp, dsize, cmask);

		dp += (dsize >> 3);
		sp += (dsize >> 3) << 2;

/* end point handling */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			sd0 = vis_ld_d64_nf(sp);
			sd1 = vis_ld_d64_nf(sp + 1);
			sd2 = vis_ld_d64_nf(sp + 2);
			sd3 = vis_ld_d64_nf(sp + 3);
			dd2 = vis_bshuffle(sd0, sd1);
			dd3 = vis_bshuffle(sd2, sd3);
			dd0 = vis_freg_pair(vis_read_hi(dd2), vis_read_hi(dd3));
			vis_pst_8(dd0, dp, emask);
		}
	} else {
		mlib_v_ImageChannelExtract_U8_41_D1_na((mlib_u8 *)sp,
		    (mlib_u8 *)dp, dsize, cmask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_U8_41(
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

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageChannelExtract_U8_41_D1(sa, da, xsize, cmask);
		sa = sl += slb;
		da = dl += dlb;
	}
}

/* *********************************************************** */

/*
 * extract one channel from a 2-channel image.
 * both source and destination image data are 8-byte aligned.
 * dsize is multiple of 4.
 */

void
mlib_v_ImageChannelExtract_S16_21_A8D1X4(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	cmask = 12 >> ((cmask & 1) << 1);
	mlib_v_ImageChannelExtract_U8_42_A8D1X8((mlib_u8 *)src, (mlib_u8 *)dst,
	    dsize, cmask);
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_S16_21_A8D2X4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	cmask = 12 >> ((cmask & 1) << 1);
	mlib_v_ImageChannelExtract_U8_42_A8D2X8((mlib_u8 *)src, slb,
	    (mlib_u8 *)dst, dlb, xsize, ysize, cmask);
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_S16_21_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	cmask = 12 >> ((cmask & 1) << 1);
	mlib_v_ImageChannelExtract_U8_42_D1((mlib_u8 *)src, (mlib_u8 *)dst,
	    dsize, cmask);
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_S16_21(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	cmask = 12 >> ((cmask & 1) << 1);
	mlib_v_ImageChannelExtract_U8_42((mlib_u8 *)src, slb, (mlib_u8 *)dst,
	    dlb, xsize, ysize, cmask);
}

/* *********************************************************** */

#define	CHANNELEXTRACT_S16_31L(sd0, sd1, sd2, dd)                \
	                                                         \
/*                                                               \
 * extract the left channel                                      \
 */                                                              \
	sda = vis_fpmerge(vis_read_hi(sd0), vis_read_lo(sd1));   \
	sdb = vis_fpmerge(vis_read_lo(sd0), vis_read_hi(sd2));   \
	sdc = vis_fpmerge(vis_read_hi(sda), vis_read_lo(sdb));   \
	dd = vis_fpmerge(vis_read_hi(sdc), vis_read_lo(sdc))

/* *********************************************************** */

#define	CHANNELEXTRACT_S16_31M(sd0, sd1, sd2, dd)                \
	                                                         \
/*                                                               \
 * extract the middle channel                                    \
 */                                                              \
	sda = vis_fpmerge(vis_read_hi(sd0), vis_read_lo(sd1));   \
	sdb = vis_fpmerge(vis_read_hi(sd1), vis_read_lo(sd2));   \
	sdc = vis_fpmerge(vis_read_lo(sda), vis_read_hi(sdb));   \
	dd = vis_fpmerge(vis_read_hi(sdc), vis_read_lo(sdc))

/* *********************************************************** */

#define	CHANNELEXTRACT_S16_31R(sd0, sd1, sd2, dd)                \
	                                                         \
/*                                                               \
 * extract the right channel                                     \
 */                                                              \
	sda = vis_fpmerge(vis_read_lo(sd0), vis_read_hi(sd2));   \
	sdb = vis_fpmerge(vis_read_hi(sd1), vis_read_lo(sd2));   \
	sdc = vis_fpmerge(vis_read_hi(sda), vis_read_lo(sdb));   \
	dd = vis_fpmerge(vis_read_hi(sdc), vis_read_lo(sdc))

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_S16_31_A8D1X4(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1, sd2;
	mlib_d64 sda, sdb, sdc;
	mlib_d64 dd;
	mlib_s32 i;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

	if (cmask == 4) {

#pragma pipeloop(0)
		for (i = 0; i < dsize / 4; i++) {
			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			CHANNELEXTRACT_S16_31L(sd0, sd1, sd2, dd);
			(*dp++) = dd;
		}
	} else if (cmask == 2) {

#pragma pipeloop(0)
		for (i = 0; i < dsize / 4; i++) {
			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			CHANNELEXTRACT_S16_31M(sd0, sd1, sd2, dd);
			(*dp++) = dd;
		}
	} else {

#pragma pipeloop(0)
		for (i = 0; i < dsize / 4; i++) {
			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			CHANNELEXTRACT_S16_31R(sd0, sd1, sd2, dd);
			(*dp++) = dd;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_S16_31_A8D2X4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_d64 *sl, *dl;
	mlib_d64 sd0, sd1, sd2;
	mlib_d64 sda, sdb, sdc;
	mlib_d64 dd;
	mlib_s32 i, j;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	if (cmask == 4) {
		for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
			for (i = 0; i < xsize / 4; i++) {
				sd0 = (*sp++);
				sd1 = (*sp++);
				sd2 = (*sp++);
				CHANNELEXTRACT_S16_31L(sd0, sd1, sd2, dd);
				(*dp++) = dd;
			}

			sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 2) {
		for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
			for (i = 0; i < xsize / 4; i++) {
				sd0 = (*sp++);
				sd1 = (*sp++);
				sd2 = (*sp++);
				CHANNELEXTRACT_S16_31M(sd0, sd1, sd2, dd);
				(*dp++) = dd;
			}

			sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		}
	} else {
		for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
			for (i = 0; i < xsize / 4; i++) {
				sd0 = (*sp++);
				sd1 = (*sp++);
				sd2 = (*sp++);
				CHANNELEXTRACT_S16_31R(sd0, sd1, sd2, dd);
				(*dp++) = dd;
			}

			sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_S16_31_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_s16 *sa, *da;

/* end points in dst */
	mlib_s16 *dend, *dend2;

/* 8-byte aligned start points in dst */
	mlib_d64 *dp;

/* 8-byte aligned start point in src */
	mlib_d64 *sp;

/* 8-byte source data */
	mlib_d64 sd0, sd1, sd2;
	mlib_d64 sd3, sd4, sd5;
	mlib_d64 sda, sdb, sdc;
	mlib_d64 dd0, dd1;

/* offset of address in src */
	mlib_s32 soff;

/* offset of address in dst */
	mlib_s32 doff;

/* offset of src over dst */
	mlib_s32 off;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 i, n;

	sa = (void *)src;
	da = dst;

/* prepare the source address */
	sp = (mlib_d64 *)((mlib_addr)sa & (~7));
	soff = ((mlib_addr)sa & 7);

/* prepare the destination addresses */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	doff = ((mlib_addr)da & 7);
	dend = da + dsize - 1;
	dend2 = dend - 3;

/* calculate the src's offset over dst */

	if (cmask == 4) {
		off = (soff / 6) * 2 - doff;
	} else if (cmask == 2) {
		off = ((soff + 2) / 6) * 2 - doff;
	} else {
		off = ((soff + 4) / 6) * 2 - doff;
	}

	if (((cmask == 4) && (soff % 6 == 0)) ||
	    ((cmask == 2) && (soff % 6 == 4)) ||
	    ((cmask == 1) && (soff % 6 == 2))) {
/* extract left channel */

/* src and dst have same alignment */
		if (off == 0) {

/* generate edge mask for the start point */
			emask = vis_edge16(da, dend);

/* load 24 bytes */
			sd0 = vis_ld_d64_nf(sp);
			sd1 = vis_ld_d64_nf(sp + 1);
			sd2 = vis_ld_d64_nf(sp + 2);
			sp += 3;

/* extract, including some garbage at the start point */
			CHANNELEXTRACT_S16_31L(sd0, sd1, sd2, dd0);

/* store 8 bytes result */
			vis_pst_16(dd0, dp++, emask);

			if ((mlib_addr)dp <= (mlib_addr)dend2) {
				n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 8 + 1;

/* 8-pixel column loop, emask not needed */

#pragma pipeloop(0)
				for (i = 0; i < n; i++) {
					sd0 = (*sp++);
					sd1 = (*sp++);
					sd2 = (*sp++);
					CHANNELEXTRACT_S16_31L(sd0, sd1, sd2,
					    dd0);
					(*dp++) = dd0;
				}
			}

/* end point handling */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				sd0 = vis_ld_d64_nf(sp);
				sd1 = vis_ld_d64_nf(sp + 1);
				sd2 = vis_ld_d64_nf(sp + 2);
				CHANNELEXTRACT_S16_31L(sd0, sd1, sd2, dd0);
				vis_pst_16(dd0, dp++, emask);
			}
		} else {
			vis_alignaddr((void *)off, 0);

/* generate edge mask for the start point */
			emask = vis_edge16(da, dend);

			if (off < 0) {
/* load 24 bytes */
				sd3 = vis_ld_d64_nf(sp);
				sd4 = vis_ld_d64_nf(sp + 1);
				sd5 = vis_ld_d64_nf(sp + 2);
				sp += 3;

/* extract and store 8 bytes */
				CHANNELEXTRACT_S16_31L(sd3, sd4, sd5, dd1);
				vis_pst_16(vis_faligndata(dd1, dd1), dp++,
				    emask);
			} else {
/* load 48 bytes */
				sd0 = vis_ld_d64_nf(sp);
				sd1 = vis_ld_d64_nf(sp + 1);
				sd2 = vis_ld_d64_nf(sp + 2);
				sd3 = vis_ld_d64_nf(sp + 3);
				sd4 = vis_ld_d64_nf(sp + 4);
				sd5 = vis_ld_d64_nf(sp + 5);
				sp += 6;

/* extract and store 8 bytes */
				CHANNELEXTRACT_S16_31L(sd0, sd1, sd2, dd0);
				CHANNELEXTRACT_S16_31L(sd3, sd4, sd5, dd1);
				vis_pst_16(vis_faligndata(dd0, dd1), dp++,
				    emask);
			}

			if ((mlib_addr)dp <= (mlib_addr)dend2) {
				n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 8 + 1;

/* 8-pixel column loop, emask not needed */

#pragma pipeloop(0)
				for (i = 0; i < n; i++) {
					dd0 = dd1;
					sd3 = (*sp++);
					sd4 = (*sp++);
					sd5 = (*sp++);
					CHANNELEXTRACT_S16_31L(sd3, sd4, sd5,
					    dd1);
					(*dp++) = vis_faligndata(dd0, dd1);
				}
			}

/* end point handling */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				dd0 = dd1;
				sd3 = vis_ld_d64_nf(sp);
				sd4 = vis_ld_d64_nf(sp + 1);
				sd5 = vis_ld_d64_nf(sp + 2);
				CHANNELEXTRACT_S16_31L(sd3, sd4, sd5, dd1);
				vis_pst_16(vis_faligndata(dd0, dd1), dp, emask);
			}
		}
	} else if (((cmask == 4) && (soff % 6 == 2)) || ((cmask == 2) &&
	    (soff % 6 == 0)) || ((cmask == 1) && (soff % 6 == 4))) {
/* extract middle channel */

/* src and dst have same alignment */
		if (off == 0) {

/* generate edge mask for the start point */
			emask = vis_edge16(da, dend);

/* load 24 bytes */
			sd0 = vis_ld_d64_nf(sp);
			sd1 = vis_ld_d64_nf(sp + 1);
			sd2 = vis_ld_d64_nf(sp + 2);
			sp += 3;

/* extract, including some garbage at the start point */
			CHANNELEXTRACT_S16_31M(sd0, sd1, sd2, dd0);

/* store 8 bytes result */
			vis_pst_16(dd0, dp++, emask);

			if ((mlib_addr)dp <= (mlib_addr)dend2) {
				n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 8 + 1;

/* 8-pixel column loop, emask not needed */

#pragma pipeloop(0)
				for (i = 0; i < n; i++) {
					sd0 = (*sp++);
					sd1 = (*sp++);
					sd2 = (*sp++);
					CHANNELEXTRACT_S16_31M(sd0, sd1, sd2,
					    dd0);
					(*dp++) = dd0;
				}
			}

/* end point handling */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				sd0 = vis_ld_d64_nf(sp);
				sd1 = vis_ld_d64_nf(sp + 1);
				sd2 = vis_ld_d64_nf(sp + 2);
				CHANNELEXTRACT_S16_31M(sd0, sd1, sd2, dd0);
				vis_pst_16(dd0, dp, emask);
			}
		} else {
			vis_alignaddr((void *)off, 0);

/* generate edge mask for the start point */
			emask = vis_edge16(da, dend);

			if (off < 0) {
/* load 24 bytes */
				sd3 = vis_ld_d64_nf(sp);
				sd4 = vis_ld_d64_nf(sp + 1);
				sd5 = vis_ld_d64_nf(sp + 2);
				sp += 3;

/* extract and store 8 bytes */
				CHANNELEXTRACT_S16_31M(sd3, sd4, sd5, dd1);
				vis_pst_16(vis_faligndata(dd1, dd1), dp++,
				    emask);
			} else {
/* load 48 bytes */
				sd0 = vis_ld_d64_nf(sp);
				sd1 = vis_ld_d64_nf(sp + 1);
				sd2 = vis_ld_d64_nf(sp + 2);
				sd3 = vis_ld_d64_nf(sp + 3);
				sd4 = vis_ld_d64_nf(sp + 4);
				sd5 = vis_ld_d64_nf(sp + 5);
				sp += 6;

/* extract and store 8 bytes */
				CHANNELEXTRACT_S16_31M(sd0, sd1, sd2, dd0);
				CHANNELEXTRACT_S16_31M(sd3, sd4, sd5, dd1);
				vis_pst_16(vis_faligndata(dd0, dd1), dp++,
				    emask);
			}

			if ((mlib_addr)dp <= (mlib_addr)dend2) {
				n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 8 + 1;

/* 8-pixel column loop, emask not needed */

#pragma pipeloop(0)
				for (i = 0; i < n; i++) {
					dd0 = dd1;
					sd3 = (*sp++);
					sd4 = (*sp++);
					sd5 = (*sp++);
					CHANNELEXTRACT_S16_31M(sd3, sd4, sd5,
					    dd1);
					(*dp++) = vis_faligndata(dd0, dd1);
				}
			}

/* end point handling */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				dd0 = dd1;
				sd3 = vis_ld_d64_nf(sp);
				sd4 = vis_ld_d64_nf(sp + 1);
				sd5 = vis_ld_d64_nf(sp + 2);
				CHANNELEXTRACT_S16_31M(sd3, sd4, sd5, dd1);
				vis_pst_16(vis_faligndata(dd0, dd1), dp, emask);
			}
		}
	} else {
/* extract right channel */

/* src and dst have same alignment */
		if (off == 0) {

/* generate edge mask for the start point */
			emask = vis_edge16(da, dend);

/* load 24 bytes */
			sd0 = vis_ld_d64_nf(sp);
			sd1 = vis_ld_d64_nf(sp + 1);
			sd2 = vis_ld_d64_nf(sp + 2);
			sp += 3;

/* extract, including some garbage at the start point */
			CHANNELEXTRACT_S16_31R(sd0, sd1, sd2, dd0);

/* store 8 bytes result */
			vis_pst_16(dd0, dp++, emask);

			if ((mlib_addr)dp <= (mlib_addr)dend2) {
				n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 8 + 1;

/* 8-pixel column loop, emask not needed */

#pragma pipeloop(0)
				for (i = 0; i < n; i++) {
					sd0 = (*sp++);
					sd1 = (*sp++);
					sd2 = (*sp++);
					CHANNELEXTRACT_S16_31R(sd0, sd1, sd2,
					    dd0);
					(*dp++) = dd0;
				}
			}

/* end point handling */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				sd0 = vis_ld_d64_nf(sp);
				sd1 = vis_ld_d64_nf(sp + 1);
				sd2 = vis_ld_d64_nf(sp + 2);
				CHANNELEXTRACT_S16_31R(sd0, sd1, sd2, dd0);
				vis_pst_16(dd0, dp, emask);
			}
		} else {
			vis_alignaddr((void *)off, 0);

/* generate edge mask for the start point */
			emask = vis_edge16(da, dend);

			if (off < 0) {
/* load 24 bytes */
				sd3 = vis_ld_d64_nf(sp);
				sd4 = vis_ld_d64_nf(sp + 1);
				sd5 = vis_ld_d64_nf(sp + 2);
				sp += 3;

/* extract and store 8 bytes */
				CHANNELEXTRACT_S16_31R(sd3, sd4, sd5, dd1);
				vis_pst_16(vis_faligndata(dd1, dd1), dp++,
				    emask);
			} else {
/* load 48 bytes */
				sd0 = vis_ld_d64_nf(sp);
				sd1 = vis_ld_d64_nf(sp + 1);
				sd2 = vis_ld_d64_nf(sp + 2);
				sd3 = vis_ld_d64_nf(sp + 3);
				sd4 = vis_ld_d64_nf(sp + 4);
				sd5 = vis_ld_d64_nf(sp + 5);
				sp += 6;

/* extract and store 8 bytes */
				CHANNELEXTRACT_S16_31R(sd0, sd1, sd2, dd0);
				CHANNELEXTRACT_S16_31R(sd3, sd4, sd5, dd1);
				vis_pst_16(vis_faligndata(dd0, dd1), dp++,
				    emask);
			}

			if ((mlib_addr)dp <= (mlib_addr)dend2) {
				n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 8 + 1;

/* 8-pixel column loop, emask not needed */

#pragma pipeloop(0)
				for (i = 0; i < n; i++) {
					dd0 = dd1;
					sd3 = (*sp++);
					sd4 = (*sp++);
					sd5 = (*sp++);
					CHANNELEXTRACT_S16_31R(sd3, sd4, sd5,
					    dd1);
					(*dp++) = vis_faligndata(dd0, dd1);
				}
			}

/* end point handling */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				dd0 = dd1;
				sd3 = vis_ld_d64_nf(sp);
				sd4 = vis_ld_d64_nf(sp + 1);
				sd5 = vis_ld_d64_nf(sp + 2);
				CHANNELEXTRACT_S16_31R(sd3, sd4, sd5, dd1);
				vis_pst_16(vis_faligndata(dd0, dd1), dp, emask);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_S16_31(
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

	for (j = 0; j < ysize; j++) {
		mlib_v_ImageChannelExtract_S16_31_D1(sa, da, xsize, cmask);
		sa = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		da = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

#define	CHANNELEXTRACT_S16_41L(sd0, sd1, sd2, sd3, dd)           \
	                                                         \
/*                                                               \
 * extract the left channel                                      \
 */                                                              \
	sda = vis_fpmerge(vis_read_hi(sd0), vis_read_hi(sd2));   \
	sdb = vis_fpmerge(vis_read_hi(sd1), vis_read_hi(sd3));   \
	sdc = vis_fpmerge(vis_read_hi(sda), vis_read_hi(sdb));   \
	dd = vis_fpmerge(vis_read_hi(sdc), vis_read_lo(sdc))

/* *********************************************************** */

#define	CHANNELEXTRACT_S16_41ML(sd0, sd1, sd2, sd3, dd)          \
	                                                         \
/*                                                               \
 * extract the middle left channel                               \
 */                                                              \
	sda = vis_fpmerge(vis_read_hi(sd0), vis_read_hi(sd2));   \
	sdb = vis_fpmerge(vis_read_hi(sd1), vis_read_hi(sd3));   \
	sdc = vis_fpmerge(vis_read_lo(sda), vis_read_lo(sdb));   \
	dd = vis_fpmerge(vis_read_hi(sdc), vis_read_lo(sdc))

/* *********************************************************** */

#define	CHANNELEXTRACT_S16_41MR(sd0, sd1, sd2, sd3, dd)          \
	                                                         \
/*                                                               \
 * extract the middle right channel                              \
 */                                                              \
	sda = vis_fpmerge(vis_read_lo(sd0), vis_read_lo(sd2));   \
	sdb = vis_fpmerge(vis_read_lo(sd1), vis_read_lo(sd3));   \
	sdc = vis_fpmerge(vis_read_hi(sda), vis_read_hi(sdb));   \
	dd = vis_fpmerge(vis_read_hi(sdc), vis_read_lo(sdc))

/* *********************************************************** */

#define	CHANNELEXTRACT_S16_41R(sd0, sd1, sd2, sd3, dd)           \
	                                                         \
/*                                                               \
 * extract the right channel                                     \
 */                                                              \
	sda = vis_fpmerge(vis_read_lo(sd0), vis_read_lo(sd2));   \
	sdb = vis_fpmerge(vis_read_lo(sd1), vis_read_lo(sd3));   \
	sdc = vis_fpmerge(vis_read_lo(sda), vis_read_lo(sdb));   \
	dd = vis_fpmerge(vis_read_hi(sdc), vis_read_lo(sdc))

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_S16_41_A8D1X4(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_d64 sda, sdb, sdc;
	mlib_d64 dd;
	mlib_s32 i;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

	if (cmask == 8) {

#pragma pipeloop(0)
		for (i = 0; i < dsize / 4; i++) {
			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			sd3 = (*sp++);
			CHANNELEXTRACT_S16_41L(sd0, sd1, sd2, sd3, dd);
			(*dp++) = dd;
		}
	} else if (cmask == 4) {

#pragma pipeloop(0)
		for (i = 0; i < dsize / 4; i++) {
			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			sd3 = (*sp++);
			CHANNELEXTRACT_S16_41ML(sd0, sd1, sd2, sd3, dd);
			(*dp++) = dd;
		}
	} else if (cmask == 2) {

#pragma pipeloop(0)
		for (i = 0; i < dsize / 4; i++) {
			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			sd3 = (*sp++);
			CHANNELEXTRACT_S16_41MR(sd0, sd1, sd2, sd3, dd);
			(*dp++) = dd;
		}
	} else {

#pragma pipeloop(0)
		for (i = 0; i < dsize / 4; i++) {
			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			sd3 = (*sp++);
			CHANNELEXTRACT_S16_41R(sd0, sd1, sd2, sd3, dd);
			(*dp++) = dd;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_S16_41_A8D2X4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_d64 *sl, *dl;
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_d64 sda, sdb, sdc;
	mlib_d64 dd;
	mlib_s32 i, j;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	if (cmask == 8) {
		for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
			for (i = 0; i < xsize / 4; i++) {
				sd0 = (*sp++);
				sd1 = (*sp++);
				sd2 = (*sp++);
				sd3 = (*sp++);
				CHANNELEXTRACT_S16_41L(sd0, sd1, sd2, sd3, dd);
				(*dp++) = dd;
			}

			sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 4) {
		for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
			for (i = 0; i < xsize / 4; i++) {
				sd0 = (*sp++);
				sd1 = (*sp++);
				sd2 = (*sp++);
				sd3 = (*sp++);
				CHANNELEXTRACT_S16_41ML(sd0, sd1, sd2, sd3, dd);
				(*dp++) = dd;
			}

			sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 2) {
		for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
			for (i = 0; i < xsize / 4; i++) {
				sd0 = (*sp++);
				sd1 = (*sp++);
				sd2 = (*sp++);
				sd3 = (*sp++);
				CHANNELEXTRACT_S16_41MR(sd0, sd1, sd2, sd3, dd);
				(*dp++) = dd;
			}

			sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		}
	} else {
		for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
			for (i = 0; i < xsize / 4; i++) {
				sd0 = (*sp++);
				sd1 = (*sp++);
				sd2 = (*sp++);
				sd3 = (*sp++);
				CHANNELEXTRACT_S16_41R(sd0, sd1, sd2, sd3, dd);
				(*dp++) = dd;
			}

			sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_S16_41_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_s16 *sa, *da;

/* end points in dst */
	mlib_s16 *dend, *dend2;

/* 8-byte aligned start points in dst */
	mlib_d64 *dp;

/* 8-byte aligned start point in src */
	mlib_d64 *sp;

/* 8-byte source data */
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_d64 sd4, sd5, sd6, sd7;
	mlib_d64 sda, sdb, sdc;
	mlib_d64 dd0, dd1;

/* offset of address in src */
	mlib_s32 soff;

/* offset of address in dst */
	mlib_s32 doff;

/* offset of src over dst */
	mlib_s32 off;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 i, n;

	sa = (void *)src;
	da = dst;

/* prepare the source address */
	sp = (mlib_d64 *)((mlib_addr)sa & (~7));
	soff = ((mlib_addr)sa & 7);

/* prepare the destination addresses */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	doff = ((mlib_addr)da & 7);
	dend = da + dsize - 1;
	dend2 = dend - 3;

/* calculate the src's offset over dst */

	if (cmask == 8) {
		off = (soff / 8) * 2 - doff;
	} else if (cmask == 4) {
		off = ((soff + 2) / 8) * 2 - doff;
	} else if (cmask == 2) {
		off = ((soff + 4) / 8) * 2 - doff;
	} else {
		off = ((soff + 6) / 8) * 2 - doff;
	}

	if (((cmask == 8) && (soff == 0)) ||
	    ((cmask == 4) && (soff == 6)) ||
	    ((cmask == 2) && (soff == 4)) || ((cmask == 1) && (soff == 2))) {
/* extract left channel */

/* src and dst have same alignment */
		if (off == 0) {

/* generate edge mask for the start point */
			emask = vis_edge16(da, dend);

/* load 32 bytes */
			sd0 = vis_ld_d64_nf(sp);
			sd1 = vis_ld_d64_nf(sp + 1);
			sd2 = vis_ld_d64_nf(sp + 2);
			sd3 = vis_ld_d64_nf(sp + 3);
			sp += 4;

/* extract, including some garbage at the start point */
			CHANNELEXTRACT_S16_41L(sd0, sd1, sd2, sd3, dd0);

/* store 8 bytes result */
			vis_pst_16(dd0, dp++, emask);

			if ((mlib_addr)dp <= (mlib_addr)dend2) {
				n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 8 + 1;

/* 8-pixel column loop, emask not needed */

#pragma pipeloop(0)
				for (i = 0; i < n; i++) {
					sd0 = (*sp++);
					sd1 = (*sp++);
					sd2 = (*sp++);
					sd3 = (*sp++);
					CHANNELEXTRACT_S16_41L(sd0, sd1, sd2,
					    sd3, dd0);
					(*dp++) = dd0;
				}
			}

/* end point handling */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				sd0 = vis_ld_d64_nf(sp);
				sd1 = vis_ld_d64_nf(sp + 1);
				sd2 = vis_ld_d64_nf(sp + 2);
				sd3 = vis_ld_d64_nf(sp + 3);
				CHANNELEXTRACT_S16_41L(sd0, sd1, sd2, sd3, dd0);
				vis_pst_16(dd0, dp++, emask);
			}
		} else {
			vis_alignaddr((void *)off, 0);

/* generate edge mask for the start point */
			emask = vis_edge16(da, dend);

			if (off < 0) {
/* load 32 bytes */
				sd4 = vis_ld_d64_nf(sp);
				sd5 = vis_ld_d64_nf(sp + 1);
				sd6 = vis_ld_d64_nf(sp + 2);
				sd7 = vis_ld_d64_nf(sp + 3);
				sp += 4;

/* extract and store 8 bytes */
				CHANNELEXTRACT_S16_41L(sd4, sd5, sd6, sd7, dd1);
				vis_pst_16(vis_faligndata(dd1, dd1), dp++,
				    emask);
			} else {
/* load 64 bytes */
				sd0 = vis_ld_d64_nf(sp);
				sd1 = vis_ld_d64_nf(sp + 1);
				sd2 = vis_ld_d64_nf(sp + 2);
				sd3 = vis_ld_d64_nf(sp + 3);
				sd4 = vis_ld_d64_nf(sp + 4);
				sd5 = vis_ld_d64_nf(sp + 5);
				sd6 = vis_ld_d64_nf(sp + 6);
				sd7 = vis_ld_d64_nf(sp + 7);
				sp += 8;

/* extract and store 8 bytes */
				CHANNELEXTRACT_S16_41L(sd0, sd1, sd2, sd3, dd0);
				CHANNELEXTRACT_S16_41L(sd4, sd5, sd6, sd7, dd1);
				vis_pst_16(vis_faligndata(dd0, dd1), dp++,
				    emask);
			}

			if ((mlib_addr)dp <= (mlib_addr)dend2) {
				n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 8 + 1;

/* 8-pixel column loop, emask not needed */

#pragma pipeloop(0)
				for (i = 0; i < n; i++) {
					dd0 = dd1;
					sd4 = (*sp++);
					sd5 = (*sp++);
					sd6 = (*sp++);
					sd7 = (*sp++);
					CHANNELEXTRACT_S16_41L(sd4, sd5, sd6,
					    sd7, dd1);
					(*dp++) = vis_faligndata(dd0, dd1);
				}
			}

/* end point handling */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				dd0 = dd1;
				sd4 = vis_ld_d64_nf(sp);
				sd5 = vis_ld_d64_nf(sp + 1);
				sd6 = vis_ld_d64_nf(sp + 2);
				sd7 = vis_ld_d64_nf(sp + 3);
				CHANNELEXTRACT_S16_41L(sd4, sd5, sd6, sd7, dd1);
				vis_pst_16(vis_faligndata(dd0, dd1), dp, emask);
			}
		}
	} else if (((cmask == 8) && (soff == 2)) || ((cmask == 4) &&
	    (soff == 0)) || ((cmask == 2) && (soff == 6)) || ((cmask == 1) &&
	    (soff == 4))) {
/* extract middle left channel */

/* src and dst have same alignment */
		if (off == 0) {

/* generate edge mask for the start point */
			emask = vis_edge16(da, dend);

/* load 32 bytes */
			sd0 = vis_ld_d64_nf(sp);
			sd1 = vis_ld_d64_nf(sp + 1);
			sd2 = vis_ld_d64_nf(sp + 2);
			sd3 = vis_ld_d64_nf(sp + 3);
			sp += 4;

/* extract, including some garbage at the start point */
			CHANNELEXTRACT_S16_41ML(sd0, sd1, sd2, sd3, dd0);

/* store 8 bytes result */
			vis_pst_16(dd0, dp++, emask);

			if ((mlib_addr)dp <= (mlib_addr)dend2) {
				n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 8 + 1;

/* 8-pixel column loop, emask not needed */

#pragma pipeloop(0)
				for (i = 0; i < n; i++) {
					sd0 = (*sp++);
					sd1 = (*sp++);
					sd2 = (*sp++);
					sd3 = (*sp++);
					CHANNELEXTRACT_S16_41ML(sd0, sd1, sd2,
					    sd3, dd0);
					(*dp++) = dd0;
				}
			}

/* end point handling */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				sd0 = vis_ld_d64_nf(sp);
				sd1 = vis_ld_d64_nf(sp + 1);
				sd2 = vis_ld_d64_nf(sp + 2);
				sd3 = vis_ld_d64_nf(sp + 3);
				CHANNELEXTRACT_S16_41ML(sd0, sd1, sd2, sd3,
				    dd0);
				vis_pst_16(dd0, dp++, emask);
			}
		} else {
			vis_alignaddr((void *)off, 0);

/* generate edge mask for the start point */
			emask = vis_edge16(da, dend);

			if (off < 0) {
/* load 32 bytes */
				sd4 = vis_ld_d64_nf(sp);
				sd5 = vis_ld_d64_nf(sp + 1);
				sd6 = vis_ld_d64_nf(sp + 2);
				sd7 = vis_ld_d64_nf(sp + 3);
				sp += 4;

/* extract and store 8 bytes */
				CHANNELEXTRACT_S16_41ML(sd4, sd5, sd6, sd7,
				    dd1);
				vis_pst_16(vis_faligndata(dd1, dd1), dp++,
				    emask);
			} else {
/* load 64 bytes */
				sd0 = vis_ld_d64_nf(sp);
				sd1 = vis_ld_d64_nf(sp + 1);
				sd2 = vis_ld_d64_nf(sp + 2);
				sd3 = vis_ld_d64_nf(sp + 3);
				sd4 = vis_ld_d64_nf(sp + 4);
				sd5 = vis_ld_d64_nf(sp + 5);
				sd6 = vis_ld_d64_nf(sp + 6);
				sd7 = vis_ld_d64_nf(sp + 7);
				sp += 8;

/* extract and store 8 bytes */
				CHANNELEXTRACT_S16_41ML(sd0, sd1, sd2, sd3,
				    dd0);
				CHANNELEXTRACT_S16_41ML(sd4, sd5, sd6, sd7,
				    dd1);
				vis_pst_16(vis_faligndata(dd0, dd1), dp++,
				    emask);
			}

			if ((mlib_addr)dp <= (mlib_addr)dend2) {
				n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 8 + 1;

/* 8-pixel column loop, emask not needed */

#pragma pipeloop(0)
				for (i = 0; i < n; i++) {
					dd0 = dd1;
					sd4 = (*sp++);
					sd5 = (*sp++);
					sd6 = (*sp++);
					sd7 = (*sp++);
					CHANNELEXTRACT_S16_41ML(sd4, sd5, sd6,
					    sd7, dd1);
					(*dp++) = vis_faligndata(dd0, dd1);
				}
			}

/* end point handling */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				dd0 = dd1;
				sd4 = vis_ld_d64_nf(sp);
				sd5 = vis_ld_d64_nf(sp + 1);
				sd6 = vis_ld_d64_nf(sp + 2);
				sd7 = vis_ld_d64_nf(sp + 3);
				CHANNELEXTRACT_S16_41ML(sd4, sd5, sd6, sd7,
				    dd1);
				vis_pst_16(vis_faligndata(dd0, dd1), dp, emask);
			}
		}
	} else if (((cmask == 8) && (soff == 4)) || ((cmask == 4) &&
	    (soff == 2)) || ((cmask == 2) && (soff == 0)) || ((cmask == 1) &&
	    (soff == 6))) {
/* extract middle right channel */

/* src and dst have same alignment */
		if (off == 0) {

/* generate edge mask for the start point */
			emask = vis_edge16(da, dend);

/* load 32 bytes */
			sd0 = vis_ld_d64_nf(sp);
			sd1 = vis_ld_d64_nf(sp + 1);
			sd2 = vis_ld_d64_nf(sp + 2);
			sd3 = vis_ld_d64_nf(sp + 3);
			sp += 4;

/* extract, including some garbage at the start point */
			CHANNELEXTRACT_S16_41MR(sd0, sd1, sd2, sd3, dd0);

/* store 8 bytes result */
			vis_pst_16(dd0, dp++, emask);

			if ((mlib_addr)dp <= (mlib_addr)dend2) {
				n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 8 + 1;

/* 8-pixel column loop, emask not needed */

#pragma pipeloop(0)
				for (i = 0; i < n; i++) {
					sd0 = (*sp++);
					sd1 = (*sp++);
					sd2 = (*sp++);
					sd3 = (*sp++);
					CHANNELEXTRACT_S16_41MR(sd0, sd1, sd2,
					    sd3, dd0);
					(*dp++) = dd0;
				}
			}

/* end point handling */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				sd0 = vis_ld_d64_nf(sp);
				sd1 = vis_ld_d64_nf(sp + 1);
				sd2 = vis_ld_d64_nf(sp + 2);
				sd3 = vis_ld_d64_nf(sp + 3);
				CHANNELEXTRACT_S16_41MR(sd0, sd1, sd2, sd3,
				    dd0);
				vis_pst_16(dd0, dp, emask);
			}
		} else {
			vis_alignaddr((void *)off, 0);

/* generate edge mask for the start point */
			emask = vis_edge16(da, dend);

			if (off < 0) {
/* load 32 bytes */
				sd4 = vis_ld_d64_nf(sp);
				sd5 = vis_ld_d64_nf(sp + 1);
				sd6 = vis_ld_d64_nf(sp + 2);
				sd7 = vis_ld_d64_nf(sp + 3);
				sp += 4;

/* extract and store 8 bytes */
				CHANNELEXTRACT_S16_41MR(sd4, sd5, sd6, sd7,
				    dd1);
				vis_pst_16(vis_faligndata(dd1, dd1), dp++,
				    emask);
			} else {
/* load 64 bytes */
				sd0 = vis_ld_d64_nf(sp);
				sd1 = vis_ld_d64_nf(sp + 1);
				sd2 = vis_ld_d64_nf(sp + 2);
				sd3 = vis_ld_d64_nf(sp + 3);
				sd4 = vis_ld_d64_nf(sp + 4);
				sd5 = vis_ld_d64_nf(sp + 5);
				sd6 = vis_ld_d64_nf(sp + 6);
				sd7 = vis_ld_d64_nf(sp + 7);
				sp += 8;

/* extract and store 8 bytes */
				CHANNELEXTRACT_S16_41MR(sd0, sd1, sd2, sd3,
				    dd0);
				CHANNELEXTRACT_S16_41MR(sd4, sd5, sd6, sd7,
				    dd1);
				vis_pst_16(vis_faligndata(dd0, dd1), dp++,
				    emask);
			}

			if ((mlib_addr)dp <= (mlib_addr)dend2) {
				n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 8 + 1;

/* 8-pixel column loop, emask not needed */

#pragma pipeloop(0)
				for (i = 0; i < n; i++) {
					dd0 = dd1;
					sd4 = (*sp++);
					sd5 = (*sp++);
					sd6 = (*sp++);
					sd7 = (*sp++);
					CHANNELEXTRACT_S16_41MR(sd4, sd5, sd6,
					    sd7, dd1);
					(*dp++) = vis_faligndata(dd0, dd1);
				}
			}

/* end point handling */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				dd0 = dd1;
				sd4 = vis_ld_d64_nf(sp);
				sd5 = vis_ld_d64_nf(sp + 1);
				sd6 = vis_ld_d64_nf(sp + 2);
				sd7 = vis_ld_d64_nf(sp + 3);
				CHANNELEXTRACT_S16_41MR(sd4, sd5, sd6, sd7,
				    dd1);
				vis_pst_16(vis_faligndata(dd0, dd1), dp, emask);
			}
		}
	} else {
/* extract right channel */
/* src and dst have same alignment */
		if (off == 0) {

/* generate edge mask for the start point */
			emask = vis_edge16(da, dend);

/* load 32 bytes */
			sd0 = vis_ld_d64_nf(sp);
			sd1 = vis_ld_d64_nf(sp + 1);
			sd2 = vis_ld_d64_nf(sp + 2);
			sd3 = vis_ld_d64_nf(sp + 3);
			sp += 4;

/* extract, including some garbage at the start point */
			CHANNELEXTRACT_S16_41R(sd0, sd1, sd2, sd3, dd0);

/* store 8 bytes result */
			vis_pst_16(dd0, dp++, emask);

			if ((mlib_addr)dp <= (mlib_addr)dend2) {
				n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 8 + 1;

/* 8-pixel column loop, emask not needed */

#pragma pipeloop(0)
				for (i = 0; i < n; i++) {
					sd0 = (*sp++);
					sd1 = (*sp++);
					sd2 = (*sp++);
					sd3 = (*sp++);
					CHANNELEXTRACT_S16_41R(sd0, sd1, sd2,
					    sd3, dd0);
					(*dp++) = dd0;
				}
			}

/* end point handling */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				sd0 = vis_ld_d64_nf(sp);
				sd1 = vis_ld_d64_nf(sp + 1);
				sd2 = vis_ld_d64_nf(sp + 2);
				sd3 = vis_ld_d64_nf(sp + 3);
				CHANNELEXTRACT_S16_41R(sd0, sd1, sd2, sd3, dd0);
				vis_pst_16(dd0, dp, emask);
			}
		} else {
			vis_alignaddr((void *)off, 0);

/* generate edge mask for the start point */
			emask = vis_edge16(da, dend);

			if (off < 0) {
/* load 32 bytes */
				sd4 = vis_ld_d64_nf(sp);
				sd5 = vis_ld_d64_nf(sp + 1);
				sd6 = vis_ld_d64_nf(sp + 2);
				sd7 = vis_ld_d64_nf(sp + 3);
				sp += 4;

/* extract and store 8 bytes */
				CHANNELEXTRACT_S16_41R(sd4, sd5, sd6, sd7, dd1);
				vis_pst_16(vis_faligndata(dd1, dd1), dp++,
				    emask);
			} else {
/* load 64 bytes */
				sd0 = vis_ld_d64_nf(sp);
				sd1 = vis_ld_d64_nf(sp + 1);
				sd2 = vis_ld_d64_nf(sp + 2);
				sd3 = vis_ld_d64_nf(sp + 3);
				sd4 = vis_ld_d64_nf(sp + 4);
				sd5 = vis_ld_d64_nf(sp + 5);
				sd6 = vis_ld_d64_nf(sp + 6);
				sd7 = vis_ld_d64_nf(sp + 7);
				sp += 8;

/* extract and store 8 bytes */
				CHANNELEXTRACT_S16_41R(sd0, sd1, sd2, sd3, dd0);
				CHANNELEXTRACT_S16_41R(sd4, sd5, sd6, sd7, dd1);
				vis_pst_16(vis_faligndata(dd0, dd1), dp++,
				    emask);
			}

			if ((mlib_addr)dp <= (mlib_addr)dend2) {
				n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 8 + 1;

/* 8-pixel column loop, emask not needed */

#pragma pipeloop(0)
				for (i = 0; i < n; i++) {
					dd0 = dd1;
					sd4 = (*sp++);
					sd5 = (*sp++);
					sd6 = (*sp++);
					sd7 = (*sp++);
					CHANNELEXTRACT_S16_41R(sd4, sd5, sd6,
					    sd7, dd1);
					(*dp++) = vis_faligndata(dd0, dd1);
				}
			}

/* end point handling */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				dd0 = dd1;
				sd4 = vis_ld_d64_nf(sp);
				sd5 = vis_ld_d64_nf(sp + 1);
				sd6 = vis_ld_d64_nf(sp + 2);
				sd7 = vis_ld_d64_nf(sp + 3);
				CHANNELEXTRACT_S16_41R(sd4, sd5, sd6, sd7, dd1);
				vis_pst_16(vis_faligndata(dd0, dd1), dp, emask);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_S16_41(
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
	for (j = 0; j < ysize; j++) {
		mlib_v_ImageChannelExtract_S16_41_D1(sa, da, xsize, cmask);
		sa = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		da = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */
