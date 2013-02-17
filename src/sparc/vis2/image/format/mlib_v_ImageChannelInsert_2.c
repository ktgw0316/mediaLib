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

#pragma ident	"@(#)mlib_v_ImageChannelInsert_2.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_v_ImageChannelInsert_U8_23_A8D1X8
 *      mlib_v_ImageChannelInsert_U8_23_D1
 *      mlib_v_ImageChannelInsert_U8_23
 *      mlib_v_ImageChannelInsert_U8_24_A8D1X8
 *      mlib_v_ImageChannelInsert_U8_24_D1
 *      mlib_v_ImageChannelInsert_U8_24
 *      mlib_v_ImageChannelInsert_S16_23_A8D1X4
 *      mlib_v_ImageChannelInsert_S16_23_D1
 *      mlib_v_ImageChannelInsert_S16_23
 *      mlib_v_ImageChannelInsert_S16_24_A8D1X4
 *      mlib_v_ImageChannelInsert_S16_24_D1
 *      mlib_v_ImageChannelInsert_S16_24
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
 *      Copy the 2-channel source image into the selected channels
 *      of the destination image -- VIS version low level functions.
 *
 * NOTE
 *      These functions are separated from mlib_v_ImageChannelInsert.c
 *      for loop unrolling and structure clarity.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageChannelInsert_2.h>

/* *********************************************************** */

static const mlib_s32 mlib_one_count[] = {
	0, 1, 1, 2, 1, 2, 2, 3
};

static const mlib_s32 mlib_off_U8[] = {
	0x00000000, 0x11111111, 0x22222222, 0x33333333,
	0x44444444, 0x55555555, 0x66666666, 0x77777777
};

/* *********************************************************** */

static const mlib_s32 mlib_bmask_U8_23[] = {
	0x00122344, 0x5667889A, 0x23445667,
	0x01123345, 0x5677899A, 0x33455677
};

/* *********************************************************** */

#define	INSERT_23_1(sd0, sd1, sd2, off)                         \
	vis_write_bmask(bm0, off);                              \
	dd0 = vis_bshuffle(sd0, sd1);                           \
	vis_write_bmask(bm1, off);                              \
	dd1 = vis_bshuffle(sd0, sd1);                           \
	vis_write_bmask(bm2, off);                              \
	dd2 = vis_bshuffle(sd1, sd2)

/* *********************************************************** */

#define	INSERT_23_2(sd0, sd1, sd2, off, off1)                   \
	vis_write_bmask(bm0, off);                              \
	dd0 = vis_bshuffle(sd0, sd1);                           \
	vis_write_bmask(bm1, off1);                             \
	dd1 = vis_bshuffle(sd1, sd2);                           \
	vis_write_bmask(bm2, off);                              \
	dd2 = vis_bshuffle(sd1, sd2)

/* *********************************************************** */

#define	STORE_U8                                                \
	vis_pst_8(dd0, dp++, sm0);                              \
	vis_pst_8(dd1, dp++, sm1);                              \
	vis_pst_8(dd2, dp++, sm2)

/* *********************************************************** */

#define	INSERT_STORE_U8_23_1(sd0, sd1, sd2, off)                \
	INSERT_23_1(sd0, sd1, sd2, off);                        \
	STORE_U8

/* *********************************************************** */

#define	INSERT_STORE_U8_23_2(sd0, sd1, sd2, off, off1)          \
	INSERT_23_2(sd0, sd1, sd2, off, off1);                  \
	STORE_U8

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_U8_23_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;

/* source data */
	mlib_d64 sd0, sd1;

/* dst data */
	mlib_d64 dd0, dd1, dd2;
	mlib_s32 bm0, bm1, bm2;
	mlib_s32 sm0, sm1, sm2;
	mlib_s32 i;

	if (cmask == 6) {
		bm0 = mlib_bmask_U8_23[3];
		bm1 = mlib_bmask_U8_23[4];
		bm2 = mlib_bmask_U8_23[5];
	} else {
		bm0 = mlib_bmask_U8_23[0];
		bm1 = mlib_bmask_U8_23[1];
		bm2 = mlib_bmask_U8_23[2];
	}

	cmask |= (cmask << 3);
	cmask |= (cmask << 6);

	sm0 = cmask >> 1;
	sm1 = cmask >> 2;
	sm2 = cmask;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

#pragma pipeloop(0)
	for (i = 0; i < dsize / 8; i++) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		INSERT_STORE_U8_23_1(sd0, sd1, sd1, 0);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_U8_23_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_u8 *dend;
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1, sd2;
	mlib_d64 dd0, dd1, dd2;
	mlib_s32 bm0, bm1, bm2;
	mlib_s32 i, soff, doff, off, off1, emask;
	mlib_s32 sm0, sm1, sm2, shift;

	dsize *= 3;

	cmask |= (cmask << 3);
	cmask |= (cmask << 6);
	cmask |= (cmask << 12);

	dend = dst + dsize - 1;

	dp = (mlib_d64 *)((mlib_addr)dst & (~7));
	doff = (dst - (mlib_u8 *)dp) & 7;

	cmask >>= doff;

	sm0 = cmask >> 1;
	sm1 = cmask >> 2;
	sm2 = cmask;

	shift = (((sm0 & 0xFF) >> (8 - doff)) >> ((doff / 3) * 3));

	src -= (((doff / 3) * 2) + mlib_one_count[shift]);

	sp = (mlib_d64 *)((mlib_addr)src & (~7));
	soff = (src - (mlib_u8 *)sp) & 7;

	if ((cmask & 7) == 6) {
		bm0 = mlib_bmask_U8_23[3];
		bm1 = mlib_bmask_U8_23[4];
		bm2 = mlib_bmask_U8_23[5];
	} else {
		bm0 = mlib_bmask_U8_23[0];
		bm1 = mlib_bmask_U8_23[1];
		bm2 = mlib_bmask_U8_23[2];
	}

	off = mlib_off_U8[soff];

	if (soff < 4) {

		sd0 = vis_ld_d64_nf(sp);
		sp++;

		if (doff != 0) {
/* generate edge mask for the start point */
			emask = vis_edge8(dst, dend);
			sd1 = vis_ld_d64_nf(sp);
			sp++;
			vis_write_bmask(bm0, off);
			dd0 = vis_bshuffle(sd0, sd1);
			vis_pst_8(dd0, dp++, sm0 & emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				vis_write_bmask(bm1, off);
				dd1 = vis_bshuffle(sd0, sd1);
				vis_pst_8(dd1, dp++, sm1 & emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge8(dp, dend);
					sd2 = (*sp++);
					vis_write_bmask(bm2, off);
					dd2 = vis_bshuffle(sd1, sd2);
					vis_pst_8(dd2, dp++, sm2 & emask);
				}
			}

			sd0 = sd2;
			dsize -= (24 - doff);
		}
#pragma pipeloop(0)
		for (i = 0; i < dsize - 23; i += 24) {
			sd1 = (*sp++);
			sd2 = (*sp++);
			INSERT_STORE_U8_23_1(sd0, sd1, sd2, off);
			sd0 = sd2;
		}

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			sd1 = vis_ld_d64_nf(sp);
			sp++;
			vis_write_bmask(bm0, off);
			dd0 = vis_bshuffle(sd0, sd1);
			vis_pst_8(dd0, dp++, sm0 & emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				vis_write_bmask(bm1, off);
				dd1 = vis_bshuffle(sd0, sd1);
				vis_pst_8(dd1, dp++, sm1 & emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge8(dp, dend);
					sd2 = vis_ld_d64_nf(sp);
					vis_write_bmask(bm2, off);
					dd2 = vis_bshuffle(sd1, sd2);
					vis_pst_8(dd2, dp, sm2 & emask);
				}
			}
		}
	} else {
		off1 = mlib_off_U8[soff - 4];

		bm1 -= 0x44444444;

		sd0 = vis_ld_d64_nf(sp);
		sp++;

		if (doff != 0) {
/* generate edge mask for the start point */
			emask = vis_edge8(dst, dend);
			sd1 = vis_ld_d64_nf(sp);
			sp++;
			vis_write_bmask(bm0, off);
			dd0 = vis_bshuffle(sd0, sd1);
			vis_pst_8(dd0, dp++, sm0 & emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				sd2 = vis_ld_d64_nf(sp);
				sp++;
				vis_write_bmask(bm1, off1);
				dd1 = vis_bshuffle(sd1, sd2);
				vis_pst_8(dd1, dp++, sm1 & emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge8(dp, dend);
					vis_write_bmask(bm2, off);
					dd2 = vis_bshuffle(sd1, sd2);
					vis_pst_8(dd2, dp++, sm2 & emask);
				}
			}

			sd0 = sd2;
			dsize -= (24 - doff);
		}
#pragma pipeloop(0)
		for (i = 0; i < dsize - 23; i += 24) {
			sd1 = (*sp++);
			sd2 = (*sp++);
			INSERT_STORE_U8_23_2(sd0, sd1, sd2, off, off1);
			sd0 = sd2;
		}

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			sd1 = vis_ld_d64_nf(sp);
			sp++;
			vis_write_bmask(bm0, off);
			dd0 = vis_bshuffle(sd0, sd1);
			vis_pst_8(dd0, dp++, sm0 & emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				sd2 = vis_ld_d64_nf(sp);
				vis_write_bmask(bm1, off1);
				dd1 = vis_bshuffle(sd1, sd2);
				vis_pst_8(dd1, dp++, sm1 & emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge8(dp, dend);
					vis_write_bmask(bm2, off);
					dd2 = vis_bshuffle(sd1, sd2);
					vis_pst_8(dd2, dp, sm2 & emask);
				}
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_U8_23(
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
		mlib_v_ImageChannelInsert_U8_23_D1(sa, da, xsize, cmask);
		sa = sl += slb;
		da = dl += dlb;
	}
}

/* *********************************************************** */

static const mlib_s32 mlib_bmask_U8_24[] = {
	0x67014523, 0x00000000, 0x70415263, 0x70145236, 0x00000000,
	0x00000000, 0x07412563, 0x04152637, 0x00000000, 0x01452367
};

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_U8_24_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_d64 sd0;
	mlib_d64 dd0, dd1;
	mlib_s32 bmask, off;
	mlib_s32 i;

	if ((cmask == 5) || (cmask == 10))
		off = 1;
	else
		off = 2;

	vis_write_gsr64((((mlib_u64)mlib_bmask_U8_24[cmask - 3]) << 32) + off);

	bmask = cmask | (cmask << 4);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

#pragma pipeloop(0)
	for (i = 0; i < dsize - 3; i += 4) {
		sd0 = (*sp++);
		dd0 = vis_bshuffle(sd0, sd0);
		dd1 = vis_faligndata(dd0, dd0);
		vis_pst_8(dd0, dp++, bmask);
		vis_pst_8(dd1, dp++, bmask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_U8_24_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_u8 *dend;
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1;
	mlib_d64 dd0, dd1;
	mlib_s32 i, soff, doff, off, emask;
	mlib_s32 shift;

	dsize *= 4;

	cmask |= (cmask << 4);
	cmask |= (cmask << 8);

	dend = dst + dsize - 1;

	dp = (mlib_d64 *)((mlib_addr)dst & (~7));
	doff = (dst - (mlib_u8 *)dp) & 7;

	cmask >>= doff;

	shift = ((cmask & 0xFF) >> ((8 - doff) + (doff & ~3)));
	src -= (((doff >> 2) << 1) + mlib_one_count[shift]);

	sp = (mlib_d64 *)((mlib_addr)src & (~7));
	soff = (src - (mlib_u8 *)sp) & 7;

	if (((cmask & 0xF) == 5) || ((cmask & 0xF) == 10))
		off = 1;
	else
		off = 2;

	vis_write_gsr64((((mlib_u64)mlib_bmask_U8_24[(cmask & 0xF) - 3] +
	    mlib_off_U8[soff]) << 32) + off);

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
		}

		sd0 = sd1;
		dsize -= (16 - doff);
	}
#pragma pipeloop(0)
	for (i = 0; i < dsize - 15; i += 16) {
		sd1 = (*sp++);
		dd0 = vis_bshuffle(sd0, sd1);
		dd1 = vis_faligndata(dd0, dd0);
		vis_pst_8(dd0, dp++, cmask);
		vis_pst_8(dd1, dp++, cmask);
		sd0 = sd1;
	}

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge8(dp, dend);
		sd1 = vis_ld_d64_nf(sp);
		dd0 = vis_bshuffle(sd0, sd1);
		vis_pst_8(dd0, dp++, cmask & emask);

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			dd1 = vis_faligndata(dd0, dd0);
			vis_pst_8(dd1, dp, cmask & emask);
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_U8_24(
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
		mlib_v_ImageChannelInsert_U8_24_D1(sa, da, xsize, cmask);
		sa = sl += slb;
		da = dl += dlb;
	}
}

/* *********************************************************** */

static const mlib_s32 mlib_bmask_S16_23[] = {
	0x01012345, 0x45678989, 0x23454567,
	0x01232345, 0x676789AB, 0x45456767
};

/* *********************************************************** */

#define	STORE_S16                                               \
	vis_pst_16(dd0, dp++, sm0);                             \
	vis_pst_16(dd1, dp++, sm1);                             \
	vis_pst_16(dd2, dp++, sm2)

/* *********************************************************** */

#define	INSERT_STORE_S16_23_1(sd0, sd1, sd2, off)               \
	INSERT_23_1(sd0, sd1, sd2, off);                        \
	STORE_S16

/* *********************************************************** */

#define	INSERT_STORE_S16_23_2(sd0, sd1, sd2, off, off1)         \
	INSERT_23_2(sd0, sd1, sd2, off, off1);                  \
	STORE_S16

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_S16_23_A8D1X4(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;

/* source data */
	mlib_d64 sd0, sd1;

/* dst data */
	mlib_d64 dd0, dd1, dd2;
	mlib_s32 bm0, bm1, bm2;
	mlib_s32 sm0, sm1, sm2;
	mlib_s32 i;

	if (cmask == 6) {
		bm0 = mlib_bmask_S16_23[3];
		bm1 = mlib_bmask_S16_23[4];
		bm2 = mlib_bmask_S16_23[5];
	} else {
		bm0 = mlib_bmask_S16_23[0];
		bm1 = mlib_bmask_S16_23[1];
		bm2 = mlib_bmask_S16_23[2];
	}

	cmask |= (cmask << 3);
	cmask |= (cmask << 6);

	sm0 = cmask >> 2;
	sm1 = cmask >> 1;
	sm2 = cmask;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

#pragma pipeloop(0)
	for (i = 0; i < dsize / 4; i++) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		INSERT_STORE_S16_23_1(sd0, sd1, sd1, 0);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_S16_23_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_s16 *dend;
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1, sd2;
	mlib_d64 dd0, dd1, dd2;
	mlib_s32 bm0, bm1, bm2;
	mlib_s32 i, soff, doff, off, off1, emask;
	mlib_s32 sm0, sm1, sm2, shift;

	dsize *= 3;

	cmask |= (cmask << 3);
	cmask |= (cmask << 6);
	cmask |= (cmask << 12);

	dend = dst + dsize - 1;

	dp = (mlib_d64 *)((mlib_addr)dst & (~7));
	doff = (dst - (mlib_s16 *)dp) & 3;

	cmask >>= doff;

	sm0 = cmask >> 2;
	sm1 = cmask >> 1;
	sm2 = cmask;

	shift = (((sm0 & 0xF) >> (4 - doff)) >> ((doff / 3) * 3));

	src -= (((doff / 3) * 2) + mlib_one_count[shift]);

	sp = (mlib_d64 *)((mlib_addr)src & (~7));
	soff = (mlib_addr)src & 7;

	if ((cmask & 7) == 6) {
		bm0 = mlib_bmask_S16_23[3];
		bm1 = mlib_bmask_S16_23[4];
		bm2 = mlib_bmask_S16_23[5];
	} else {
		bm0 = mlib_bmask_S16_23[0];
		bm1 = mlib_bmask_S16_23[1];
		bm2 = mlib_bmask_S16_23[2];
	}

	off = mlib_off_U8[soff];

	if (soff < 4) {

		sd0 = vis_ld_d64_nf(sp);
		sp++;

		if (doff != 0) {
/* generate edge mask for the start point */
			emask = vis_edge16(dst, dend);
			sd1 = vis_ld_d64_nf(sp);
			sp++;
			vis_write_bmask(bm0, off);
			dd0 = vis_bshuffle(sd0, sd1);
			vis_pst_16(dd0, dp++, sm0 & emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				vis_write_bmask(bm1, off);
				dd1 = vis_bshuffle(sd0, sd1);
				vis_pst_16(dd1, dp++, sm1 & emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge16(dp, dend);
					sd2 = vis_ld_d64_nf(sp);
					sp++;
					vis_write_bmask(bm2, off);
					dd2 = vis_bshuffle(sd1, sd2);
					vis_pst_16(dd2, dp++, sm2 & emask);
				}
			}

			sd0 = sd2;
			dsize -= (12 - doff);
		}
#pragma pipeloop(0)
		for (i = 0; i < dsize - 11; i += 12) {
			sd1 = (*sp++);
			sd2 = (*sp++);
			INSERT_STORE_S16_23_1(sd0, sd1, sd2, off);
			sd0 = sd2;
		}

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge16(dp, dend);
			sd1 = vis_ld_d64_nf(sp);
			sp++;
			vis_write_bmask(bm0, off);
			dd0 = vis_bshuffle(sd0, sd1);
			vis_pst_16(dd0, dp++, sm0 & emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				vis_write_bmask(bm1, off);
				dd1 = vis_bshuffle(sd0, sd1);
				vis_pst_16(dd1, dp++, sm1 & emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge16(dp, dend);
					sd2 = vis_ld_d64_nf(sp);
					vis_write_bmask(bm2, off);
					dd2 = vis_bshuffle(sd1, sd2);
					vis_pst_16(dd2, dp, sm2 & emask);
				}
			}
		}
	} else {
		off1 = mlib_off_U8[soff - 4];

		bm1 -= 0x44444444;

		sd0 = vis_ld_d64_nf(sp);
		sp++;

		if (doff != 0) {
/* generate edge mask for the start point */
			emask = vis_edge16(dst, dend);
			sd1 = vis_ld_d64_nf(sp);
			sp++;
			vis_write_bmask(bm0, off);
			dd0 = vis_bshuffle(sd0, sd1);
			vis_pst_16(dd0, dp++, sm0 & emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				sd2 = vis_ld_d64_nf(sp);
				sp++;
				vis_write_bmask(bm1, off1);
				dd1 = vis_bshuffle(sd1, sd2);
				vis_pst_16(dd1, dp++, sm1 & emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge16(dp, dend);
					vis_write_bmask(bm2, off);
					dd2 = vis_bshuffle(sd1, sd2);
					vis_pst_16(dd2, dp++, sm2 & emask);
				}
			}

			sd0 = sd2;
			dsize -= (12 - doff);
		}
#pragma pipeloop(0)
		for (i = 0; i < dsize - 11; i += 12) {
			sd1 = (*sp++);
			sd2 = (*sp++);
			INSERT_STORE_S16_23_2(sd0, sd1, sd2, off, off1);
			sd0 = sd2;
		}

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge16(dp, dend);
			sd1 = vis_ld_d64_nf(sp);
			sp++;
			vis_write_bmask(bm0, off);
			dd0 = vis_bshuffle(sd0, sd1);
			vis_pst_16(dd0, dp++, sm0 & emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				sd2 = vis_ld_d64_nf(sp);
				vis_write_bmask(bm1, off1);
				dd1 = vis_bshuffle(sd1, sd2);
				vis_pst_16(dd1, dp++, sm1 & emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge16(dp, dend);
					vis_write_bmask(bm2, off);
					dd2 = vis_bshuffle(sd1, sd2);
					vis_pst_16(dd2, dp, sm2 & emask);
				}
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_S16_23(
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
		mlib_v_ImageChannelInsert_S16_23_D1(sa, da, xsize, cmask);
		sa = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		da = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

static const mlib_s32 mlib_bmask_S16_24[] = {
	0x45670123, 0x00000000, 0x67014523, 0x67012345, 0x00000000,
	0x00000000, 0x01674523, 0x01452367, 0x00000000, 0x01234567
};

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_S16_24_A8D1X4(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_d64 sd0;
	mlib_d64 dd0, dd1;
	mlib_s32 bmask, off;
	mlib_s32 i;

	if ((cmask == 5) || (cmask == 10))
		off = 2;
	else
		off = 4;

	vis_write_gsr64((((mlib_u64)mlib_bmask_S16_24[cmask - 3]) << 32) + off);

	bmask = cmask | (cmask << 4);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

#pragma pipeloop(0)
	for (i = 0; i < dsize - 1; i += 2) {
		sd0 = (*sp++);
		dd0 = vis_bshuffle(sd0, sd0);
		dd1 = vis_faligndata(dd0, dd0);
		vis_pst_16(dd0, dp++, bmask);
		vis_pst_16(dd1, dp++, bmask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_S16_24_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_s16 *dend;
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1;
	mlib_d64 dd0, dd1;
	mlib_s32 i, soff, doff, off, emask;
	mlib_s32 shift;

	dsize *= 4;

	cmask |= (cmask << 4);
	cmask |= (cmask << 8);

	dend = dst + dsize - 1;

	dp = (mlib_d64 *)((mlib_addr)dst & (~7));
	doff = (dst - (mlib_s16 *)dp) & 3;

	cmask >>= doff;
	cmask &= 0xF;

	shift = (cmask >> ((4 - doff) + (doff & ~3)));
	src -= (((doff >> 2) << 1) + mlib_one_count[shift]);

	sp = (mlib_d64 *)((mlib_addr)src & (~7));
	soff = (mlib_addr)src & 7;

	if ((cmask == 5) || (cmask == 10))
		off = 2;
	else
		off = 4;

	vis_write_gsr64((((mlib_u64)mlib_bmask_S16_24[cmask - 3] +
	    mlib_off_U8[soff]) << 32) + off);

	sd0 = vis_ld_d64_nf(sp);
	sp++;

	if (doff != 0) {
/* generate edge mask for the start point */
		emask = vis_edge16(dst, dend);
		sd1 = vis_ld_d64_nf(sp);
		sp++;
		dd0 = vis_bshuffle(sd0, sd1);
		vis_pst_16(dd0, dp++, cmask & emask);

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge16(dp, dend);
			dd1 = vis_faligndata(dd0, dd0);
			vis_pst_16(dd1, dp++, cmask & emask);
		}

		sd0 = sd1;
		dsize -= (8 - doff);
	}
#pragma pipeloop(0)
	for (i = 0; i < dsize - 7; i += 8) {
		sd1 = (*sp++);
		dd0 = vis_bshuffle(sd0, sd1);
		dd1 = vis_faligndata(dd0, dd0);
		vis_pst_16(dd0, dp++, cmask);
		vis_pst_16(dd1, dp++, cmask);
		sd0 = sd1;
	}

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge16(dp, dend);
		sd1 = vis_ld_d64_nf(sp);
		dd0 = vis_bshuffle(sd0, sd1);
		vis_pst_16(dd0, dp++, cmask & emask);

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge16(dp, dend);
			dd1 = vis_faligndata(dd0, dd0);
			vis_pst_16(dd1, dp, cmask & emask);
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_S16_24(
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
		mlib_v_ImageChannelInsert_S16_24_D1(sa, da, xsize, cmask);
		sa = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		da = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */
