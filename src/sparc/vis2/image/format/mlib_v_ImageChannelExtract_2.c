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

#pragma ident	"@(#)mlib_v_ImageChannelExtract_2.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *  mlib_v_ImageChannelExtract_U8_32_A8D1X8
 *  mlib_v_ImageChannelExtract_U8_32_A8D2X8
 *  mlib_v_ImageChannelExtract_U8_32_D1
 *  mlib_v_ImageChannelExtract_U8_32
 *  mlib_v_ImageChannelExtract_U8_42_A8D1X8
 *  mlib_v_ImageChannelExtract_U8_42_A8D2X8
 *  mlib_v_ImageChannelExtract_U8_42_D1
 *  mlib_v_ImageChannelExtract_U8_42
 *  mlib_v_ImageChannelExtract_S16_32_A8D1X4
 *  mlib_v_ImageChannelExtract_S16_32_A8D2X4
 *  mlib_v_ImageChannelExtract_S16_32_D1
 *  mlib_v_ImageChannelExtract_S16_32
 *  mlib_v_ImageChannelExtract_S16_42_A8D1X4
 *  mlib_v_ImageChannelExtract_S16_42_A8D2X4
 *  mlib_v_ImageChannelExtract_S16_42_D1
 *  mlib_v_ImageChannelExtract_S16_42
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
 *      2-channel destination image.
 *
 * NOTE
 *      These functions are separated from mlib_ImageChannelExtract.c
 *      for loop unrolling and structure clarity.
 *      I was hope to see you in any case, but it seems you doesn't?:(
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageChannelExtract_2.h>

/* *********************************************************** */

static const mlib_s32 mlib_bmask_U8_32[] = {
	0x124578AB, 0, 0x0235689B, 0x0134679A,
	0x234589AB, 0, 0x014567AB, 0x01236789
};

static const mlib_s32 mlib_offset_U8_32[] = {
	0x00000000, 0x11111111, 0x22222222, 0x33333333
};

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_U8_32_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1, sd2, sd3, sd4;
	mlib_s32 i;

	vis_write_gsr64(((mlib_u64)mlib_bmask_U8_32[cmask - 3] << 32) + 4);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

#pragma pipeloop(0)
	for (i = 0; i < dsize - 15; i += 16) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		sd3 = vis_faligndata(sd1, sd2);
		sd4 = vis_faligndata(sd2, sd2);
		(*dp++) = vis_bshuffle(sd0, sd1);
		(*dp++) = vis_bshuffle(sd3, sd4);

		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		sd3 = vis_faligndata(sd1, sd2);
		sd4 = vis_faligndata(sd2, sd2);
		(*dp++) = vis_bshuffle(sd0, sd1);
		(*dp++) = vis_bshuffle(sd3, sd4);
	}

#pragma pipeloop(0)
	for (; i < dsize - 7; i += 8) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		sd3 = vis_faligndata(sd1, sd2);
		sd4 = vis_faligndata(sd2, sd2);
		(*dp++) = vis_bshuffle(sd0, sd1);
		(*dp++) = vis_bshuffle(sd3, sd4);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_U8_32_A8D2X8(
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
	mlib_d64 sd0, sd1, sd2, sd3, sd4;
	mlib_s32 i, j;

	vis_write_gsr64(((mlib_u64)mlib_bmask_U8_32[cmask - 3] << 32) + 4);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize - 15; i += 16) {
			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			sd3 = vis_faligndata(sd1, sd2);
			sd4 = vis_faligndata(sd2, sd2);
			(*dp++) = vis_bshuffle(sd0, sd1);
			(*dp++) = vis_bshuffle(sd3, sd4);

			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			sd3 = vis_faligndata(sd1, sd2);
			sd4 = vis_faligndata(sd2, sd2);
			(*dp++) = vis_bshuffle(sd0, sd1);
			(*dp++) = vis_bshuffle(sd3, sd4);
		}

#pragma pipeloop(0)
		for (; i < xsize - 7; i += 8) {
			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			sd3 = vis_faligndata(sd1, sd2);
			sd4 = vis_faligndata(sd2, sd2);
			(*dp++) = vis_bshuffle(sd0, sd1);
			(*dp++) = vis_bshuffle(sd3, sd4);
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_U8_32_D1(
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
	mlib_d64 sd0, sd1, sd2, sd3, sd4, sd5;
	mlib_d64 dd0;
	mlib_s32 shift;

/* offset of address in src */
	mlib_s32 soff;

/* offset of address in dst */
	mlib_s32 doff;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 i;

	dsize *= 2;
	sa = (void *)src;
	da = dst;
	dend = da + dsize - 1;

	if (cmask == 3) {
		sa += 1;
		cmask = 6;
	}

	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	doff = ((mlib_addr)da & 7);

	shift = doff & 1;
	cmask -= shift;

	if (cmask == 4)
		cmask = 3;

	sp = (mlib_d64 *)((mlib_addr)(sa - 3 * (doff >> 1) - 2 * shift) & (~7));
	soff = (sa - 3 * (doff >> 1) - 2 * shift - (mlib_u8 *)sp) & 7;

	vis_write_gsr64((((mlib_u64)mlib_bmask_U8_32[cmask - 3] +
	    mlib_offset_U8_32[soff & 3]) << 32) + 4);

	if (doff != 0) {
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

		if (soff >= 4) {
			sd0 = vis_faligndata(sd0, sd1);
			sd1 = vis_faligndata(sd1, sd2);
		}

		dd0 = vis_bshuffle(sd0, sd1);
		vis_pst_8(dd0, dp++, emask);

		soff = (soff + 4) & 7;

		dsize -= (8 - doff);

		if (dsize <= 0)
			dsize = 0;
		sp += (2 - (soff >> 2));
	}

	if (soff < 4) {
		sd0 = vis_ld_d64_nf(sp);
		sp++;

#pragma pipeloop(0)
		for (i = 0; i < dsize - 31; i += 32) {
			sd1 = (*sp++);
			sd2 = (*sp++);
			sd3 = (*sp++);
			sd4 = vis_faligndata(sd1, sd2);
			sd5 = vis_faligndata(sd2, sd3);
			(*dp++) = vis_bshuffle(sd0, sd1);
			(*dp++) = vis_bshuffle(sd4, sd5);

			sd1 = (*sp++);
			sd2 = (*sp++);
			sd0 = (*sp++);
			sd4 = vis_faligndata(sd1, sd2);
			sd5 = vis_faligndata(sd2, sd0);
			(*dp++) = vis_bshuffle(sd3, sd1);
			(*dp++) = vis_bshuffle(sd4, sd5);
		}

#pragma pipeloop(0)
		for (; i < dsize - 15; i += 16) {
			sd1 = vis_ld_d64_nf(sp);
			sd2 = vis_ld_d64_nf(sp + 1);
			sd3 = vis_ld_d64_nf(sp + 2);
			sp += 3;
			sd4 = vis_faligndata(sd1, sd2);
			sd5 = vis_faligndata(sd2, sd3);
			(*dp++) = vis_bshuffle(sd0, sd1);
			(*dp++) = vis_bshuffle(sd4, sd5);
			sd0 = sd3;
		}

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			sd1 = vis_ld_d64_nf(sp);
			dd0 = vis_bshuffle(sd0, sd1);
			vis_pst_8(dd0, dp++, emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				sd2 = vis_ld_d64_nf(sp + 1);
				sd3 = vis_ld_d64_nf(sp + 2);
				sd4 = vis_faligndata(sd1, sd2);
				sd5 = vis_faligndata(sd2, sd3);
				dd0 = vis_bshuffle(sd4, sd5);
				vis_pst_8(dd0, dp, emask);
			}
		}
	} else {
		sd0 = vis_ld_d64_nf(sp);
		sp++;

#pragma pipeloop(0)
		for (i = 0; i < dsize - 31; i += 32) {
			sd1 = (*sp++);
			sd2 = (*sp++);
			sd4 = vis_faligndata(sd0, sd1);
			sd5 = vis_faligndata(sd1, sd2);
			sd0 = (*sp++);
			(*dp++) = vis_bshuffle(sd4, sd5);
			(*dp++) = vis_bshuffle(sd2, sd0);

			sd1 = (*sp++);
			sd2 = (*sp++);
			sd4 = vis_faligndata(sd0, sd1);
			sd5 = vis_faligndata(sd1, sd2);
			sd0 = (*sp++);
			(*dp++) = vis_bshuffle(sd4, sd5);
			(*dp++) = vis_bshuffle(sd2, sd0);
		}

#pragma pipeloop(0)
		for (; i < dsize - 15; i += 16) {
			sd1 = vis_ld_d64_nf(sp);
			sd2 = vis_ld_d64_nf(sp + 1);
			sd4 = vis_faligndata(sd0, sd1);
			sd5 = vis_faligndata(sd1, sd2);
			sd0 = vis_ld_d64_nf(sp + 2);
			sp += 3;
			(*dp++) = vis_bshuffle(sd4, sd5);
			(*dp++) = vis_bshuffle(sd2, sd0);
		}

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			sd1 = vis_ld_d64_nf(sp);
			sd2 = vis_ld_d64_nf(sp + 1);
			sd4 = vis_faligndata(sd0, sd1);
			sd5 = vis_faligndata(sd1, sd2);
			dd0 = vis_bshuffle(sd4, sd5);
			vis_pst_8(dd0, dp++, emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				sd0 = vis_ld_d64_nf(sp + 2);
				dd0 = vis_bshuffle(sd2, sd0);
				vis_pst_8(dd0, dp, emask);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_U8_32(
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
		mlib_v_ImageChannelExtract_U8_32_D1(sa, da, xsize, cmask);
		sa = sl += slb;
		da = dl += dlb;
	}
}

/* *********************************************************** */

static const mlib_s32 mlib_bmask_U8_42[] = {
	0x2367ABEF, 0x00000000, 0x13579BDF, 0x12569ADE, 0x00000000,
	0x00000000, 0x03478BCF, 0x02468ACE, 0x00000000, 0x014589CD,
	0x4567CDEF, 0x00000000, 0x2367ABEF, 0x2345ABCD, 0x00000000,
	0x00000000, 0x016789EF, 0x014589CD, 0x00000000, 0x012389AB
};

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_U8_42_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_s32 i;

	vis_write_bmask(mlib_bmask_U8_42[cmask - 3], 0);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

#pragma pipeloop(0)
	for (i = 0; i < dsize - 7; i += 8) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		sd3 = (*sp++);
		(*dp++) = vis_bshuffle(sd0, sd1);
		(*dp++) = vis_bshuffle(sd2, sd3);
	}

/* For S16 */

	if (i < dsize) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		(*dp++) = vis_bshuffle(sd0, sd1);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_U8_42_A8D2X8(
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
	mlib_s32 i, j;

	vis_write_bmask(mlib_bmask_U8_42[cmask - 3], 0);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < xsize - 7; i += 8) {
			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			sd3 = (*sp++);
			(*dp++) = vis_bshuffle(sd0, sd1);
			(*dp++) = vis_bshuffle(sd2, sd3);
		}

/* For S16 */

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

void
mlib_v_ImageChannelExtract_U8_42_D1(
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

/* 8-byte source data */
	mlib_d64 sd4, sd5, sd6, sd7, sd8;
	mlib_d64 dd0;
	mlib_s32 shift;

/* offset of address in src */
	mlib_s32 soff;

/* offset of address in dst */
	mlib_s32 doff;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 i;

	dsize *= 2;
	sa = (void *)src;
	da = dst;
	dend = da + dsize - 1;

	if (cmask == 3) {
		sa += 2;
		cmask = 12;
	} else if ((cmask & 8) == 0) {
		sa += 1;
		cmask *= 2;
	}

	cmask += (cmask << 4);

	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	doff = ((mlib_addr)da & 7);

	shift = doff & 1;
	cmask = (cmask >> (4 - shift)) & 0xF;

	sp = (mlib_d64 *)((mlib_addr)(sa - 4 * (doff >> 1) - 3 * shift) & (~7));
	soff = (sa - 4 * (doff >> 1) - 3 * shift - (mlib_u8 *)sp) & 7;

	vis_write_gsr64((((mlib_u64)mlib_bmask_U8_42[cmask - 3]) << 32) + soff);

	if (doff != 0) {
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

		sd0 = vis_faligndata(sd0, sd1);
		sd1 = vis_faligndata(sd1, sd2);

		dd0 = vis_bshuffle(sd0, sd1);
		vis_pst_8(dd0, dp++, emask);

		dsize -= (8 - doff);

		if (dsize <= 0)
			dsize = 0;
		sp += 2;
	}

	if (soff == 0) {

#pragma pipeloop(0)
		for (i = 0; i < dsize - 15; i += 16) {
			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			sd3 = (*sp++);
			(*dp++) = vis_bshuffle(sd0, sd1);
			(*dp++) = vis_bshuffle(sd2, sd3);
		}

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			sd0 = vis_ld_d64_nf(sp);
			sd1 = vis_ld_d64_nf(sp + 1);
			dd0 = vis_bshuffle(sd0, sd1);
			vis_pst_8(dd0, dp++, emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				sd2 = vis_ld_d64_nf(sp + 2);
				sd3 = vis_ld_d64_nf(sp + 3);
				dd0 = vis_bshuffle(sd2, sd3);
				vis_pst_8(dd0, dp, emask);
			}
		}
	} else {
		sd0 = vis_ld_d64_nf(sp);
		sp++;

#pragma pipeloop(0)
		for (i = 0; i < dsize - 15; i += 16) {
			sd1 = (*sp++);
			sd2 = (*sp++);
			sd3 = (*sp++);
			sd4 = (*sp++);
			sd5 = vis_faligndata(sd0, sd1);
			sd6 = vis_faligndata(sd1, sd2);
			sd7 = vis_faligndata(sd2, sd3);
			sd8 = vis_faligndata(sd3, sd4);
			(*dp++) = vis_bshuffle(sd5, sd6);
			(*dp++) = vis_bshuffle(sd7, sd8);
			sd0 = sd4;
		}

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			sd1 = vis_ld_d64_nf(sp);
			sd2 = vis_ld_d64_nf(sp + 1);
			sd5 = vis_faligndata(sd0, sd1);
			sd6 = vis_faligndata(sd1, sd2);
			dd0 = vis_bshuffle(sd5, sd6);
			vis_pst_8(dd0, dp++, emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				sd3 = vis_ld_d64_nf(sp + 2);
				sd4 = vis_ld_d64_nf(sp + 3);
				sd7 = vis_faligndata(sd2, sd3);
				sd8 = vis_faligndata(sd3, sd4);
				dd0 = vis_bshuffle(sd7, sd8);
				vis_pst_8(dd0, dp, emask);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_U8_42(
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
		mlib_v_ImageChannelExtract_U8_42_D1(sa, da, xsize, cmask);
		sa = sl += slb;
		da = dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_S16_32_A8D1X4(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_v_ImageChannelExtract_U8_32_A8D1X8((mlib_u8 *)src, (mlib_u8 *)dst,
	    2 * dsize, cmask + 4);
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_S16_32_A8D2X4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_v_ImageChannelExtract_U8_32_A8D2X8((mlib_u8 *)src, slb,
	    (mlib_u8 *)dst, dlb, 2 * xsize, ysize, cmask + 4);
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_S16_32_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_s16 *sa, *da;

/* end points in dst */
	mlib_s16 *dend;

/* 8-byte aligned start points in dst */
	mlib_d64 *dp;

/* 8-byte aligned start point in src  */
	mlib_d64 *sp, *sp1;

/* 8-byte source data */
	mlib_d64 sd0, sd1, sd2, sd3, sd4, sd5;
	mlib_d64 dd0;
	mlib_s32 shift;

/* offset of address in src */
	mlib_s32 soff;

/* offset of address in dst */
	mlib_s32 doff;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 i;

	dsize *= 2;
	sa = (void *)src;
	da = dst;
	dend = da + dsize - 1;

	if (cmask == 3) {
		sa += 1;
		cmask = 6;
	}

	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	doff = ((mlib_addr)da & 7) >> 1;

	shift = doff & 1;
	cmask -= shift;

	if (cmask == 4)
		cmask = 3;

	sp = (mlib_d64 *)((mlib_addr)(sa - 3 * (doff >> 1) - 2 * shift) & (~7));
	soff = (sa - 3 * (doff >> 1) - 2 * shift - (mlib_s16 *)sp) & 7;
	soff *= 2;

	vis_write_gsr64((((mlib_u64)mlib_bmask_U8_32[cmask + 1] +
	    mlib_offset_U8_32[soff & 3]) << 32) + 4);

	if (doff != 0) {
		sp1 = (mlib_d64 *)((mlib_addr)sa & (~7));

/* generate edge mask for the start point */
		emask = vis_edge16(da, dend);

		sd0 = vis_ld_d64_nf(sp1);

		if (sp == sp1)
			sp1++;
		sd1 = vis_ld_d64_nf(sp1);

		if (sp + 1 == sp1)
			sp1++;
		sd2 = vis_ld_d64_nf(sp1);

		if (soff >= 4) {
			sd0 = vis_faligndata(sd0, sd1);
			sd1 = vis_faligndata(sd1, sd2);
		}

		dd0 = vis_bshuffle(sd0, sd1);
		vis_pst_16(dd0, dp++, emask);

		soff = (soff + 4) & 7;

		dsize -= (4 - doff);

		if (dsize <= 0)
			dsize = 0;
		sp += (2 - (soff >> 2));
	}

	if (soff < 4) {
		sd0 = vis_ld_d64_nf(sp);
		sp++;

#pragma pipeloop(0)
		for (i = 0; i < dsize - 15; i += 16) {
			sd1 = (*sp++);
			sd2 = (*sp++);
			sd3 = (*sp++);
			sd4 = vis_faligndata(sd1, sd2);
			sd5 = vis_faligndata(sd2, sd3);
			(*dp++) = vis_bshuffle(sd0, sd1);
			(*dp++) = vis_bshuffle(sd4, sd5);

			sd1 = (*sp++);
			sd2 = (*sp++);
			sd0 = (*sp++);
			sd4 = vis_faligndata(sd1, sd2);
			sd5 = vis_faligndata(sd2, sd0);
			(*dp++) = vis_bshuffle(sd3, sd1);
			(*dp++) = vis_bshuffle(sd4, sd5);
		}

#pragma pipeloop(0)
		for (; i < dsize - 7; i += 8) {
			sd1 = vis_ld_d64_nf(sp);
			sd2 = vis_ld_d64_nf(sp + 1);
			sd3 = vis_ld_d64_nf(sp + 2);
			sp += 3;
			sd4 = vis_faligndata(sd1, sd2);
			sd5 = vis_faligndata(sd2, sd3);
			(*dp++) = vis_bshuffle(sd0, sd1);
			(*dp++) = vis_bshuffle(sd4, sd5);
			sd0 = sd3;
		}

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge16(dp, dend);
			sd1 = vis_ld_d64_nf(sp);
			dd0 = vis_bshuffle(sd0, sd1);
			vis_pst_16(dd0, dp++, emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				sd2 = vis_ld_d64_nf(sp + 1);
				sd3 = vis_ld_d64_nf(sp + 2);
				sd4 = vis_faligndata(sd1, sd2);
				sd5 = vis_faligndata(sd2, sd3);
				dd0 = vis_bshuffle(sd4, sd5);
				vis_pst_16(dd0, dp, emask);
			}
		}
	} else {
		sd0 = vis_ld_d64_nf(sp);
		sp++;

#pragma pipeloop(0)
		for (i = 0; i < dsize - 15; i += 16) {
			sd1 = (*sp++);
			sd2 = (*sp++);
			sd4 = vis_faligndata(sd0, sd1);
			sd5 = vis_faligndata(sd1, sd2);
			sd0 = (*sp++);
			(*dp++) = vis_bshuffle(sd4, sd5);
			(*dp++) = vis_bshuffle(sd2, sd0);

			sd1 = (*sp++);
			sd2 = (*sp++);
			sd4 = vis_faligndata(sd0, sd1);
			sd5 = vis_faligndata(sd1, sd2);
			sd0 = (*sp++);
			(*dp++) = vis_bshuffle(sd4, sd5);
			(*dp++) = vis_bshuffle(sd2, sd0);
		}

#pragma pipeloop(0)
		for (; i < dsize - 7; i += 8) {
			sd1 = vis_ld_d64_nf(sp);
			sd2 = vis_ld_d64_nf(sp + 1);
			sd4 = vis_faligndata(sd0, sd1);
			sd5 = vis_faligndata(sd1, sd2);
			sd0 = vis_ld_d64_nf(sp + 2);
			sp += 3;
			(*dp++) = vis_bshuffle(sd4, sd5);
			(*dp++) = vis_bshuffle(sd2, sd0);
		}

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge16(dp, dend);
			sd1 = vis_ld_d64_nf(sp);
			sd2 = vis_ld_d64_nf(sp + 1);
			sd4 = vis_faligndata(sd0, sd1);
			sd5 = vis_faligndata(sd1, sd2);
			dd0 = vis_bshuffle(sd4, sd5);
			vis_pst_16(dd0, dp++, emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				sd0 = vis_ld_d64_nf(sp + 2);
				dd0 = vis_bshuffle(sd2, sd0);
				vis_pst_16(dd0, dp, emask);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_S16_32(
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
		mlib_v_ImageChannelExtract_S16_32_D1(sa, da, xsize, cmask);
		sa = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		da = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_S16_42_A8D1X4(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_v_ImageChannelExtract_U8_42_A8D1X8((mlib_u8 *)src, (mlib_u8 *)dst,
	    2 * dsize, cmask + 10);
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_S16_42_A8D2X4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_v_ImageChannelExtract_U8_42_A8D2X8((mlib_u8 *)src, slb,
	    (mlib_u8 *)dst, dlb, 2 * xsize, ysize, cmask + 10);
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_S16_42_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_s16 *sa, *da;

/* end points in dst */
	mlib_s16 *dend;

/* 8-byte aligned start points in dst */
	mlib_d64 *dp;

/* 8-byte aligned start point in src  */
	mlib_d64 *sp, *sp1;

/* 8-byte source data */
	mlib_d64 sd0, sd1, sd2, sd3;

/* 8-byte source data */
	mlib_d64 sd4, sd5, sd6, sd7, sd8;
	mlib_d64 dd0;
	mlib_s32 shift;

/* offset of address in src */
	mlib_s32 soff;

/* offset of address in dst */
	mlib_s32 doff;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 i;

	dsize *= 2;
	sa = (void *)src;
	da = dst;
	dend = da + dsize - 1;

	if (cmask == 3) {
		sa += 2;
		cmask = 12;
	} else if ((cmask & 8) == 0) {
		sa += 1;
		cmask *= 2;
	}

	cmask += (cmask << 4);

	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	doff = ((mlib_addr)da & 7) >> 1;

	shift = doff & 1;
	cmask = (cmask >> (4 - shift)) & 0xF;

	sp = (mlib_d64 *)((mlib_addr)(sa - 4 * (doff >> 1) - 3 * shift) & (~7));
	soff = (sa - 4 * (doff >> 1) - 3 * shift - (mlib_s16 *)sp) & 7;

	vis_write_gsr64((((mlib_u64)mlib_bmask_U8_42[cmask + 7]) << 32) +
	    2 * soff);

	if (doff != 0) {
		sp1 = (mlib_d64 *)((mlib_addr)sa & (~7));

/* generate edge mask for the start point */
		emask = vis_edge16(da, dend);

		sd0 = vis_ld_d64_nf(sp1);

		if (sp == sp1)
			sp1++;
		sd1 = vis_ld_d64_nf(sp1);

		if (sp + 1 == sp1)
			sp1++;
		sd2 = vis_ld_d64_nf(sp1);

		sd0 = vis_faligndata(sd0, sd1);
		sd1 = vis_faligndata(sd1, sd2);

		dd0 = vis_bshuffle(sd0, sd1);
		vis_pst_16(dd0, dp++, emask);

		dsize -= (4 - doff);

		if (dsize <= 0)
			dsize = 0;
		sp += 2;
	}

	if (soff == 0) {

#pragma pipeloop(0)
		for (i = 0; i < dsize - 7; i += 8) {
			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			sd3 = (*sp++);
			(*dp++) = vis_bshuffle(sd0, sd1);
			(*dp++) = vis_bshuffle(sd2, sd3);
		}

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge16(dp, dend);
			sd0 = vis_ld_d64_nf(sp);
			sd1 = vis_ld_d64_nf(sp + 1);
			dd0 = vis_bshuffle(sd0, sd1);
			vis_pst_16(dd0, dp++, emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				sd2 = vis_ld_d64_nf(sp + 2);
				sd3 = vis_ld_d64_nf(sp + 3);
				dd0 = vis_bshuffle(sd2, sd3);
				vis_pst_16(dd0, dp, emask);
			}
		}
	} else {
		sd0 = vis_ld_d64_nf(sp);
		sp++;

#pragma pipeloop(0)
		for (i = 0; i < dsize - 7; i += 8) {
			sd1 = (*sp++);
			sd2 = (*sp++);
			sd3 = (*sp++);
			sd4 = (*sp++);
			sd5 = vis_faligndata(sd0, sd1);
			sd6 = vis_faligndata(sd1, sd2);
			sd7 = vis_faligndata(sd2, sd3);
			sd8 = vis_faligndata(sd3, sd4);
			(*dp++) = vis_bshuffle(sd5, sd6);
			(*dp++) = vis_bshuffle(sd7, sd8);
			sd0 = sd4;
		}

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge16(dp, dend);
			sd1 = vis_ld_d64_nf(sp);
			sd2 = vis_ld_d64_nf(sp + 1);
			sd5 = vis_faligndata(sd0, sd1);
			sd6 = vis_faligndata(sd1, sd2);
			dd0 = vis_bshuffle(sd5, sd6);
			vis_pst_16(dd0, dp++, emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				sd3 = vis_ld_d64_nf(sp + 2);
				sd4 = vis_ld_d64_nf(sp + 3);
				sd7 = vis_faligndata(sd2, sd3);
				sd8 = vis_faligndata(sd3, sd4);
				dd0 = vis_bshuffle(sd7, sd8);
				vis_pst_16(dd0, dp, emask);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_S16_42(
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
		mlib_v_ImageChannelExtract_S16_42_D1(sa, da, xsize, cmask);
		sa = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		da = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */
