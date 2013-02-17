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

#pragma ident	"@(#)mlib_v_ImageChannelInsert_34.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_v_ImageChannelInsert_U8_34_A8D1X8
 *      mlib_v_ImageChannelInsert_U8_34_D1
 *      mlib_v_ImageChannelInsert_U8_34
 *      mlib_v_ImageChannelInsert_U8_34R_A8D1X8
 *      mlib_v_ImageChannelInsert_U8_34R_D1
 *      mlib_v_ImageChannelInsert_U8_34R
 *      mlib_v_ImageChannelInsert_U8_34L_A8D1X8
 *      mlib_v_ImageChannelInsert_U8_34L_D1
 *      mlib_v_ImageChannelInsert_U8_34L
 *      mlib_v_ImageChannelInsert_S16_34_A8D1X4
 *      mlib_v_ImageChannelInsert_S16_34_D1
 *      mlib_v_ImageChannelInsert_S16_34
 *      mlib_v_ImageChannelInsert_S16_34R_A8D1X4
 *      mlib_v_ImageChannelInsert_S16_34R_D1
 *      mlib_v_ImageChannelInsert_S16_34R
 *      mlib_v_ImageChannelInsert_S16_34L_A8D1X4
 *      mlib_v_ImageChannelInsert_S16_34L_D1
 *      mlib_v_ImageChannelInsert_S16_34L
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
 *      Copy the 3-channel source image into the selected channels
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

static const mlib_s32 mlib_one_count[] = {
	0, 1, 1, 2, 1, 2, 2, 3
};

static const mlib_s32 mlib_off_U8[] = {
	0x00000000, 0x11111111, 0x22222222, 0x33333333,
	0x44444444, 0x55555555, 0x66666666, 0x77777777
};

static const mlib_s32 mlib_bmask_U8_43[] = {
	0x00123345, 0x667899AB, 0x44567789, 0x22345567,
	0x01223455, 0x67889ABB, 0x45667899, 0x23445677
};

/* *********************************************************** */

#define	INSERT_STORE_34_1(sd0, sd1, sd2, sd3, off)              \
	vis_write_bmask(bm0, off);                              \
	dd0 = vis_bshuffle(sd0, sd1);                           \
	vis_write_bmask(bm1, off);                              \
	dd1 = vis_bshuffle(sd0, sd1);                           \
	vis_write_bmask(bm2, off);                              \
	dd2 = vis_bshuffle(sd1, sd2);                           \
	vis_write_bmask(bm3, off);                              \
	dd3 = vis_bshuffle(sd2, sd3)

/* *********************************************************** */

#define	INSERT_STORE_34_2(sd0, sd1, sd2, sd3, off, off1)        \
	vis_write_bmask(bm0, off);                              \
	dd0 = vis_bshuffle(sd0, sd1);                           \
	vis_write_bmask(bm1, off1);                             \
	dd1 = vis_bshuffle(sd1, sd2);                           \
	vis_write_bmask(bm2, off1);                             \
	dd2 = vis_bshuffle(sd2, sd3);                           \
	vis_write_bmask(bm3, off);                              \
	dd3 = vis_bshuffle(sd2, sd3)

/* *********************************************************** */

#define	STORE_U8                                                \
	vis_pst_8(dd0, dp++, cmask);                            \
	vis_pst_8(dd1, dp++, cmask);                            \
	vis_pst_8(dd2, dp++, cmask);                            \
	vis_pst_8(dd3, dp++, cmask)

/* *********************************************************** */

#define	INSERT_STORE_U8_34_1(sd0, sd1, sd2, sd3, off)           \
	INSERT_STORE_34_1(sd0, sd1, sd2, sd3, off);             \
	STORE_U8

/* *********************************************************** */

#define	INSERT_STORE_U8_34_2(sd0, sd1, sd2, sd3, off, off1)     \
	INSERT_STORE_34_2(sd0, sd1, sd2, sd3, off, off1);       \
	STORE_U8

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_U8_34_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1, sd2;
	mlib_d64 dd0, dd1, dd2, dd3;
	mlib_s32 bm0, bm1, bm2, bm3;
	mlib_s32 i;

	if (cmask < 12) {
		bm0 = mlib_bmask_U8_43[0];
		bm1 = mlib_bmask_U8_43[1];
		bm2 = mlib_bmask_U8_43[2];
		bm3 = mlib_bmask_U8_43[3];
	} else {
		bm0 = mlib_bmask_U8_43[4];
		bm1 = mlib_bmask_U8_43[5];
		bm2 = mlib_bmask_U8_43[6];
		bm3 = mlib_bmask_U8_43[7];
	}

	cmask |= (cmask << 4);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

#pragma pipeloop(0)
	for (i = 0; i < dsize / 8; i++) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		INSERT_STORE_U8_34_1(sd0, sd1, sd2, sd2, 0);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_U8_34_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_u8 *dend;
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_d64 dd0, dd1, dd2, dd3;
	mlib_s32 bm0, bm1, bm2, bm3;
	mlib_s32 i, soff, doff, off, off1, emask;
	mlib_s32 shift;

	dsize *= 4;

	cmask |= (cmask << 4);
	cmask |= (cmask << 8);

	dend = dst + dsize - 1;

	dp = (mlib_d64 *)((mlib_addr)dst & (~7));
	doff = (dst - (mlib_u8 *)dp) & 7;

	cmask >>= doff;

	shift = ((cmask & 0xFF) >> ((8 - doff) + (doff & ~3)));
	src -= (((doff >> 2) * 3) + mlib_one_count[shift]);

	sp = (mlib_d64 *)((mlib_addr)src & (~7));
	soff = (src - (mlib_u8 *)sp) & 7;

	off = mlib_off_U8[soff];

	if ((cmask & 0xF) < 12) {
		bm0 = mlib_bmask_U8_43[0];
		bm1 = mlib_bmask_U8_43[1];
		bm2 = mlib_bmask_U8_43[2];
		bm3 = mlib_bmask_U8_43[3];
	} else {
		bm0 = mlib_bmask_U8_43[4];
		bm1 = mlib_bmask_U8_43[5];
		bm2 = mlib_bmask_U8_43[6];
		bm3 = mlib_bmask_U8_43[7];
	}

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
			vis_pst_8(dd0, dp++, cmask & emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				vis_write_bmask(bm1, off);
				dd1 = vis_bshuffle(sd0, sd1);
				vis_pst_8(dd1, dp++, cmask & emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge8(dp, dend);
					sd2 = vis_ld_d64_nf(sp);
					sp++;
					vis_write_bmask(bm2, off);
					dd2 = vis_bshuffle(sd1, sd2);
					vis_pst_8(dd2, dp++, cmask & emask);

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge8(dp, dend);
						sd3 = vis_ld_d64_nf(sp);
						sp++;
						vis_write_bmask(bm3, off);
						dd3 = vis_bshuffle(sd2, sd3);
						vis_pst_8(dd3, dp++,
						    cmask & emask);
					}
				}
			}

			sd0 = sd3;
			dsize -= (32 - doff);
		}

		if (soff != 0) {

#pragma pipeloop(0)
			for (i = 0; i < dsize - 31; i += 32) {
				sd1 = (*sp++);
				sd2 = (*sp++);
				sd3 = (*sp++);
				INSERT_STORE_U8_34_1(sd0, sd1, sd2, sd3, off);
				sd0 = sd3;
			}
		} else {
/* src is 8-bytes aligned */

			sp--;

#pragma pipeloop(0)
			for (i = 0; i < dsize - 31; i += 32) {
				sd0 = (*sp++);
				sd1 = (*sp++);
				sd2 = (*sp++);
				INSERT_STORE_U8_34_1(sd0, sd1, sd2, sd2, 0);
			}

			sd0 = vis_ld_d64_nf(sp);
			sp++;
		}

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			sd1 = vis_ld_d64_nf(sp);
			vis_write_bmask(bm0, off);
			dd0 = vis_bshuffle(sd0, sd1);
			vis_pst_8(dd0, dp++, cmask & emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				vis_write_bmask(bm1, off);
				dd1 = vis_bshuffle(sd0, sd1);
				vis_pst_8(dd1, dp++, cmask & emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge8(dp, dend);
					sd2 = vis_ld_d64_nf(sp + 1);
					vis_write_bmask(bm2, off);
					dd2 = vis_bshuffle(sd1, sd2);
					vis_pst_8(dd2, dp++, cmask & emask);

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge8(dp, dend);
						sd3 = vis_ld_d64_nf(sp + 2);
						vis_write_bmask(bm3, off);
						dd3 = vis_bshuffle(sd2, sd3);
						vis_pst_8(dd3, dp,
						    cmask & emask);
					}
				}
			}
		}
	} else {
		off1 = mlib_off_U8[soff - 4];

		bm1 -= 0x44444444;
		bm2 -= 0x44444444;

		sd0 = vis_ld_d64_nf(sp);
		sp++;

		if (doff != 0) {
/* generate edge mask for the start point */
			emask = vis_edge8(dst, dend);
			sd1 = vis_ld_d64_nf(sp);
			sp++;
			vis_write_bmask(bm0, off);
			dd0 = vis_bshuffle(sd0, sd1);
			vis_pst_8(dd0, dp++, cmask & emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				sd2 = vis_ld_d64_nf(sp);
				sp++;
				vis_write_bmask(bm1, off1);
				dd1 = vis_bshuffle(sd1, sd2);
				vis_pst_8(dd1, dp++, cmask & emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge8(dp, dend);
					sd3 = vis_ld_d64_nf(sp);
					sp++;
					vis_write_bmask(bm2, off1);
					dd2 = vis_bshuffle(sd2, sd3);
					vis_pst_8(dd2, dp++, cmask & emask);

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge8(dp, dend);
						vis_write_bmask(bm3, off);
						dd3 = vis_bshuffle(sd2, sd3);
						vis_pst_8(dd3, dp++,
						    cmask & emask);
					}
				}
			}

			sd0 = sd3;
			dsize -= (32 - doff);
		}
#pragma pipeloop(0)
		for (i = 0; i < dsize - 31; i += 32) {
			sd1 = (*sp++);
			sd2 = (*sp++);
			sd3 = (*sp++);
			INSERT_STORE_U8_34_2(sd0, sd1, sd2, sd3, off, off1);
			sd0 = sd3;
		}

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			sd1 = vis_ld_d64_nf(sp);
			vis_write_bmask(bm0, off);
			dd0 = vis_bshuffle(sd0, sd1);
			vis_pst_8(dd0, dp++, cmask & emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				sd2 = vis_ld_d64_nf(sp + 1);
				vis_write_bmask(bm1, off1);
				dd1 = vis_bshuffle(sd1, sd2);
				vis_pst_8(dd1, dp++, cmask & emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge8(dp, dend);
					sd3 = vis_ld_d64_nf(sp + 2);
					vis_write_bmask(bm2, off1);
					dd2 = vis_bshuffle(sd2, sd3);
					vis_pst_8(dd2, dp++, cmask & emask);

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge8(dp, dend);
						vis_write_bmask(bm3, off);
						dd3 = vis_bshuffle(sd2, sd3);
						vis_pst_8(dd3, dp,
						    cmask & emask);
					}
				}
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_U8_34(
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
		mlib_v_ImageChannelInsert_U8_34_D1(sa, da, xsize, cmask);
		sa = sl += slb;
		da = dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_U8_34R_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
	mlib_v_ImageChannelInsert_U8_34_A8D1X8(src, dst, dsize, 7);
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_U8_34R_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
	mlib_v_ImageChannelInsert_U8_34_D1(src, dst, dsize, 7);
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_U8_34R(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_v_ImageChannelInsert_U8_34(src, slb, dst, dlb, xsize, ysize, 7);
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_U8_34L_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
	mlib_v_ImageChannelInsert_U8_34_A8D1X8(src, dst, dsize, 14);
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_U8_34L_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
	mlib_v_ImageChannelInsert_U8_34_D1(src, dst, dsize, 14);
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_U8_34L(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_v_ImageChannelInsert_U8_34(src, slb, dst, dlb, xsize, ysize, 14);
}

/* *********************************************************** */

static const mlib_s32 mlib_bmask_S16_43[] = {
	0x01012345, 0x676789AB, 0x45456789, 0x23234567,
	0x01234545, 0x6789ABAB, 0x45678989, 0x23456767
};

/* *********************************************************** */

#define	STORE_S16                                               \
	vis_pst_16(dd0, dp++, cmask);                           \
	vis_pst_16(dd1, dp++, cmask);                           \
	vis_pst_16(dd2, dp++, cmask);                           \
	vis_pst_16(dd3, dp++, cmask)

/* *********************************************************** */

#define	INSERT_STORE_S16_34_1(sd0, sd1, sd2, sd3, off)          \
	INSERT_STORE_34_1(sd0, sd1, sd2, sd3, off);             \
	STORE_S16

/* *********************************************************** */

#define	INSERT_STORE_S16_34_2(sd0, sd1, sd2, sd3, off, off1)    \
	INSERT_STORE_34_2(sd0, sd1, sd2, sd3, off, off1);       \
	STORE_S16

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_S16_34_A8D1X4(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1, sd2;
	mlib_d64 dd0, dd1, dd2, dd3;
	mlib_s32 bm0, bm1, bm2, bm3;
	mlib_s32 i;

	if (cmask < 12) {
		bm0 = mlib_bmask_S16_43[0];
		bm1 = mlib_bmask_S16_43[1];
		bm2 = mlib_bmask_S16_43[2];
		bm3 = mlib_bmask_S16_43[3];
	} else {
		bm0 = mlib_bmask_S16_43[4];
		bm1 = mlib_bmask_S16_43[5];
		bm2 = mlib_bmask_S16_43[6];
		bm3 = mlib_bmask_S16_43[7];
	}

	cmask |= (cmask << 4);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

#pragma pipeloop(0)
	for (i = 0; i < dsize / 4; i++) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		INSERT_STORE_S16_34_1(sd0, sd1, sd2, sd2, 0);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_S16_34_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_s16 *dend;
	mlib_d64 *sp, *dp;
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_d64 dd0, dd1, dd2, dd3;
	mlib_s32 bm0, bm1, bm2, bm3;
	mlib_s32 i, soff, doff, off, off1, emask;
	mlib_s32 shift;

	dsize *= 4;

	cmask |= (cmask << 4);
	cmask |= (cmask << 8);

	dend = dst + dsize - 1;

	dp = (mlib_d64 *)((mlib_addr)dst & (~7));
	doff = (dst - (mlib_s16 *)dp) & 7;

	cmask >>= doff;
	cmask &= 0xF;

	shift = (cmask >> ((4 - doff) + (doff & ~3)));
	src -= (((doff >> 2) * 3) + mlib_one_count[shift]);

	sp = (mlib_d64 *)((mlib_addr)src & (~7));
	soff = (mlib_addr)src & 7;

	off = mlib_off_U8[soff];

	if (cmask < 12) {
		bm0 = mlib_bmask_S16_43[0];
		bm1 = mlib_bmask_S16_43[1];
		bm2 = mlib_bmask_S16_43[2];
		bm3 = mlib_bmask_S16_43[3];
	} else {
		bm0 = mlib_bmask_S16_43[4];
		bm1 = mlib_bmask_S16_43[5];
		bm2 = mlib_bmask_S16_43[6];
		bm3 = mlib_bmask_S16_43[7];
	}

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
			vis_pst_16(dd0, dp++, cmask & emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				vis_write_bmask(bm1, off);
				dd1 = vis_bshuffle(sd0, sd1);
				vis_pst_16(dd1, dp++, cmask & emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge16(dp, dend);
					sd2 = vis_ld_d64_nf(sp);
					sp++;
					vis_write_bmask(bm2, off);
					dd2 = vis_bshuffle(sd1, sd2);
					vis_pst_16(dd2, dp++, cmask & emask);

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge16(dp, dend);
						sd3 = vis_ld_d64_nf(sp);
						sp++;
						vis_write_bmask(bm3, off);
						dd3 = vis_bshuffle(sd2, sd3);
						vis_pst_16(dd3, dp++,
						    cmask & emask);
					}
				}
			}

			sd0 = sd3;
			dsize -= (16 - doff);
		}

		if (soff != 0) {

#pragma pipeloop(0)
			for (i = 0; i < dsize - 15; i += 16) {
				sd1 = (*sp++);
				sd2 = (*sp++);
				sd3 = (*sp++);
				INSERT_STORE_S16_34_1(sd0, sd1, sd2, sd3, off);
				sd0 = sd3;
			}
		} else {
/* src is 8-bytes aligned */

			sp--;

#pragma pipeloop(0)
			for (i = 0; i < dsize - 15; i += 16) {
				sd0 = (*sp++);
				sd1 = (*sp++);
				sd2 = (*sp++);
				INSERT_STORE_S16_34_1(sd0, sd1, sd2, sd2, 0);
			}

			sd0 = vis_ld_d64_nf(sp);
			sp++;
		}

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge16(dp, dend);
			sd1 = vis_ld_d64_nf(sp);
			vis_write_bmask(bm0, off);
			dd0 = vis_bshuffle(sd0, sd1);
			vis_pst_16(dd0, dp++, cmask & emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				vis_write_bmask(bm1, off);
				dd1 = vis_bshuffle(sd0, sd1);
				vis_pst_16(dd1, dp++, cmask & emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge16(dp, dend);
					sd2 = vis_ld_d64_nf(sp + 1);
					vis_write_bmask(bm2, off);
					dd2 = vis_bshuffle(sd1, sd2);
					vis_pst_16(dd2, dp++, cmask & emask);

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge16(dp, dend);
						sd3 = vis_ld_d64_nf(sp + 2);
						vis_write_bmask(bm3, off);
						dd3 = vis_bshuffle(sd2, sd3);
						vis_pst_16(dd3, dp,
						    cmask & emask);
					}
				}
			}
		}
	} else {
		off1 = mlib_off_U8[soff - 4];

		bm1 -= 0x44444444;
		bm2 -= 0x44444444;

		sd0 = vis_ld_d64_nf(sp);
		sp++;

		if (doff != 0) {
/* generate edge mask for the start point */
			emask = vis_edge16(dst, dend);
			sd1 = vis_ld_d64_nf(sp);
			sp++;
			vis_write_bmask(bm0, off);
			dd0 = vis_bshuffle(sd0, sd1);
			vis_pst_16(dd0, dp++, cmask & emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				sd2 = vis_ld_d64_nf(sp);
				sp++;
				vis_write_bmask(bm1, off1);
				dd1 = vis_bshuffle(sd1, sd2);
				vis_pst_16(dd1, dp++, cmask & emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge16(dp, dend);
					sd3 = vis_ld_d64_nf(sp);
					sp++;
					vis_write_bmask(bm2, off1);
					dd2 = vis_bshuffle(sd2, sd3);
					vis_pst_16(dd2, dp++, cmask & emask);

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge16(dp, dend);
						vis_write_bmask(bm3, off);
						dd3 = vis_bshuffle(sd2, sd3);
						vis_pst_16(dd3, dp++,
						    cmask & emask);
					}
				}
			}

			sd0 = sd3;
			dsize -= (16 - doff);
		}
#pragma pipeloop(0)
		for (i = 0; i < dsize - 15; i += 16) {
			sd1 = (*sp++);
			sd2 = (*sp++);
			sd3 = (*sp++);
			INSERT_STORE_S16_34_2(sd0, sd1, sd2, sd3, off, off1);
			sd0 = sd3;
		}

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge16(dp, dend);
			sd1 = vis_ld_d64_nf(sp);
			vis_write_bmask(bm0, off);
			dd0 = vis_bshuffle(sd0, sd1);
			vis_pst_16(dd0, dp++, cmask & emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				sd2 = vis_ld_d64_nf(sp + 1);
				vis_write_bmask(bm1, off1);
				dd1 = vis_bshuffle(sd1, sd2);
				vis_pst_16(dd1, dp++, cmask & emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge16(dp, dend);
					sd3 = vis_ld_d64_nf(sp + 2);
					vis_write_bmask(bm2, off1);
					dd2 = vis_bshuffle(sd2, sd3);
					vis_pst_16(dd2, dp++, cmask & emask);

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge16(dp, dend);
						vis_write_bmask(bm3, off);
						dd3 = vis_bshuffle(sd2, sd3);
						vis_pst_16(dd3, dp,
						    cmask & emask);
					}
				}
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_S16_34(
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
		mlib_v_ImageChannelInsert_S16_34_D1(sa, da, xsize, cmask);
		sa = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		da = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_S16_34R_A8D1X4(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
	mlib_v_ImageChannelInsert_S16_34_A8D1X4(src, dst, dsize, 7);
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_S16_34R_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
	mlib_v_ImageChannelInsert_S16_34_D1(src, dst, dsize, 7);
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_S16_34R(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_v_ImageChannelInsert_S16_34(src, slb, dst, dlb, xsize, ysize, 7);
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_S16_34L_A8D1X4(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
	mlib_v_ImageChannelInsert_S16_34_A8D1X4(src, dst, dsize, 14);
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_S16_34L_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
	mlib_v_ImageChannelInsert_S16_34_D1(src, dst, dsize, 14);
}

/* *********************************************************** */

void
mlib_v_ImageChannelInsert_S16_34L(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_v_ImageChannelInsert_S16_34(src, slb, dst, dlb, xsize, ysize, 14);
}

/* *********************************************************** */
