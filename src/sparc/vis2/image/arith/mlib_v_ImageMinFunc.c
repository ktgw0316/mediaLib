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

#pragma ident	"@(#)mlib_v_ImageMinFunc.c	9.3	07/11/05 SMI"

/*
 * Internal function for ImageMin function.
 *
 *    void mlib_v_ImageMin_U8_D1(mlib_u8  *src1,
 *                               mlib_u8  *src2,
 *                               mlib_u8  *dst,
 *                               mlib_s32 dsize);
 *
 *    void mlib_v_ImageMin_U8_A8D2(mlib_u8  *src1,
 *                                 mlib_s32 slb1,
 *                                 mlib_u8  *src2,
 *                                 mlib_s32 slb2,
 *                                 mlib_u8  *dst,
 *                                 mlib_s32 dlb,
 *                                 mlib_s32 xsize,
 *                                 mlib_s32 ysize);
 *
 *    void mlib_v_ImageMin_U8_1D(mlib_u8  *src1,
 *                               mlib_u8  *src2,
 *                               mlib_u8  *dst,
 *                               mlib_s32 dsize);
 *
 *    void mlib_v_ImageMin_U8(mlib_u8  *src1,
 *                            mlib_s32 slb1,
 *                            mlib_u8  *src2,
 *                            mlib_s32 slb2,
 *                            mlib_u8  *dst,
 *                            mlib_s32 dlb,
 *                            mlib_s32 xsize,
 *                            mlib_s32 ysize);
 *
 *    void mlib_v_ImageMin_S16_D1(mlib_s16 *src1,
 *                                mlib_s16 *src2,
 *                                mlib_s16 *dst,
 *                                mlib_s32 dsize);
 *
 *    void mlib_v_ImageMin_S16_A8D2(mlib_s16 *src1,
 *                                  mlib_s32 slb1,
 *                                  mlib_s16 *src2,
 *                                  mlib_s32 slb2,
 *                                  mlib_s16 *dst,
 *                                  mlib_s32 dlb,
 *                                  mlib_s32 xsize,
 *                                  mlib_s32 ysize);
 *
 *    void mlib_v_ImageMin_S16_1D(mlib_s16 *src1,
 *                                mlib_s16 *src2,
 *                                mlib_s16 *dst,
 *                                mlib_s32 dsize);
 *
 *    void mlib_v_ImageMin_S16(mlib_s16 *src1,
 *                             mlib_s32 slb1,
 *                             mlib_s16 *src2,
 *                             mlib_s32 slb2,
 *                             mlib_s16 *dst,
 *                             mlib_s32 dlb,
 *                             mlib_s32 xsize,
 *                             mlib_s32 ysize);
 *
 *    void mlib_v_ImageMin_U16_D1(mlib_u16 *src1,
 *                                mlib_u16 *src2,
 *                                mlib_u16 *dst,
 *                                mlib_s32 dsize);
 *
 *    void mlib_v_ImageMin_U16_A8D2(mlib_u16 *src1,
 *                                  mlib_s32 slb1,
 *                                  mlib_u16 *src2,
 *                                  mlib_s32 slb2,
 *                                  mlib_u16 *dst,
 *                                  mlib_s32 dlb,
 *                                  mlib_s32 xsize,
 *                                  mlib_s32 ysize);
 *
 *    void mlib_v_ImageMin_U16_1D(mlib_u16 *src1,
 *                                mlib_u16 *src2,
 *                                mlib_u16 *dst,
 *                                mlib_s32 dsize);
 *
 *    void mlib_v_ImageMin_U16(mlib_u16 *src1,
 *                             mlib_s32 slb1,
 *                             mlib_u16 *src2,
 *                             mlib_s32 slb2,
 *                             mlib_u16 *dst,
 *                             mlib_s32 dlb,
 *                             mlib_s32 xsize,
 *                             mlib_s32 ysize);
 *
 *    void mlib_v_ImageMin_S32_D1(mlib_s32 *src1,
 *                                mlib_s32 *src2,
 *                                mlib_s32 *dst,
 *                                mlib_s32 dsize);
 *
 *    void mlib_v_ImageMin_S32_A8D2(mlib_s32 *src1,
 *                                  mlib_s32 slb1,
 *                                  mlib_s32 *src2,
 *                                  mlib_s32 slb2,
 *                                  mlib_s32 *dst,
 *                                  mlib_s32 dlb,
 *                                  mlib_s32 xsize,
 *                                  mlib_s32 ysize);
 *
 *    void mlib_v_ImageMin_S32_1D(mlib_s32 *src1,
 *                                mlib_s32 *src2,
 *                                mlib_s32 *dst,
 *                                mlib_s32 dsize);
 *
 *    void mlib_v_ImageMin_S32(mlib_s32 *src1,
 *                             mlib_s32 slb1,
 *                             mlib_s32 *src2,
 *                             mlib_s32 slb2,
 *                             mlib_s32 *dst,
 *                             mlib_s32 dlb,
 *                             mlib_s32 xsize,
 *                             mlib_s32 ysize);
 *
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageMinFunc.h>

/* *********************************************************** */

#define	MLIB_V_MINIMAGE_U8(sd1, sd2, mask)                      \
	sd1h = vis_fexpand_hi(sd1);                             \
	sd1l = vis_fexpand_lo(sd1);                             \
	sd2h = vis_fmul8x16al(vis_read_hi(sd2), fone);          \
	sd2l = vis_fmul8x16al(vis_read_lo(sd2), fone);          \
	cmaskh = vis_fcmpgt16(sd1h, sd2h);                      \
	cmaskl = vis_fcmpgt16(sd1l, sd2l);                      \
	mask = (cmaskh << 4) | cmaskl

/* *********************************************************** */

void
mlib_v_ImageMin_U8_D1(
    mlib_u8 *src1,
    mlib_u8 *src2,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
/* pointers to source images */
	mlib_u8 *sa1, *sa2;

/* 8-byte aligned pointers to source images */
	mlib_d64 *sp1, *sp2;

/* pointer to destination image */
	mlib_u8 *dl;

/* 8-byte aligned pointer to destination image */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd1, sd2;

/* pointer to end of a line in destination image */
	mlib_u8 *dend;

/* temporaries */
	mlib_d64 sd1h, sd1l, sd2h, sd2l;

/*  masks */
	mlib_s32 cmask, cmaskh, cmaskl, emask;

/* offset of address alignment in destination */
	mlib_s32 off;

/* loop variable */
	mlib_s32 j;
	mlib_f32 fone = vis_to_float(0x1000);

	sa1 = src1;
	sa2 = src2;
	dl = dst;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)dl & (~7));
	off = (mlib_addr)dp - (mlib_addr)dl;
	dend = dl + dsize - 1;

/* generate edge mask for the start point */
	emask = vis_edge8(dl, dend);

/* first 8-pixel column */
	sp1 = (mlib_d64 *)vis_alignaddr(sa1, 0);
	sd1 = (*sp1++);
	sp2 = (mlib_d64 *)vis_alignaddr(sa2, 0);
	sd2 = (*sp2++);
	MLIB_V_MINIMAGE_U8(sd1, sd2, cmask);
	vis_pst_8(sd1, dp, emask);
	vis_pst_8(sd2, dp, emask & cmask);
	dp++;

#pragma pipeloop(0)
	for (j = 8 + off; j <= (dsize - 8); j += 8) {
		sd1 = (*sp1++);
		sd2 = (*sp2++);
		MLIB_V_MINIMAGE_U8(sd1, sd2, cmask);
		*dp = sd1;
		vis_pst_8(sd2, dp, cmask);
		dp++;
	}

/* last 8-pixel column */

	if ((mlib_u8 *)dp <= dend) {
		emask = vis_edge8(dp, dend);
		sd1 = *sp1;
		sd2 = *sp2;
		MLIB_V_MINIMAGE_U8(sd1, sd2, cmask);
		vis_pst_8(sd1, dp, emask);
		vis_pst_8(sd2, dp, emask & cmask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageMin_U8_A8D2(
    mlib_u8 *src1,
    mlib_s32 slb1,
    mlib_u8 *src2,
    mlib_s32 slb2,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* 8-byte aligned pointers to source images */
	mlib_d64 *sp1, *sp2;

/* pointers to a line in source */
	mlib_d64 *sl1, *sl2;

/* pointer to a line in destination */
	mlib_d64 *dl;

/* 8-byte aligned pointer to destination image */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd1, sd2;

/* pointer to end of a line in destination image */
	mlib_u8 *dend;

/* temporaries */
	mlib_d64 sd1h, sd1l, sd2h, sd2l;

/*  masks */
	mlib_s32 cmask, cmaskh, cmaskl, emask;

/* loop variable */
	mlib_s32 i, j;
	mlib_f32 fone = vis_to_float(0x1000);

	sl1 = sp1 = (mlib_d64 *)src1;
	sl2 = sp2 = (mlib_d64 *)src2;
	dl = dp = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {
		dend = (mlib_u8 *)dl + xsize - 1;

#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 8); i += 8) {
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_V_MINIMAGE_U8(sd1, sd2, cmask);
			*dp = sd1;
			vis_pst_8(sd2, dp, cmask);
			dp++;
		}

/* last 8-pixel column */

		if ((mlib_u8 *)dp <= dend) {
			emask = vis_edge8(dp, dend);
			sd1 = *sp1;
			sd2 = *sp2;
			MLIB_V_MINIMAGE_U8(sd1, sd2, cmask);
			vis_pst_8(sd1, dp, emask);
			vis_pst_8(sd2, dp, emask & cmask);
		}

		sl1 = sp1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
		sl2 = sp2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
		dl = dp = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

static const mlib_s32 bmask_min_table[8] = {
	0x01234567,
	0x12345678,
	0x23456789,
	0x3456789A,
	0x456789AB,
	0x56789ABC,
	0x6789ABCD,
	0x789ABCDE,
};

/* *********************************************************** */

void
mlib_v_ImageMin_U8_1D(
    mlib_u8 *src1,
    mlib_u8 *src2,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
/* pointers to source images */
	mlib_u8 *sa1, *sa2;

/* 8-byte aligned pointers to source images */
	mlib_d64 *sp1, *sp2;

/* pointer to destination image */
	mlib_u8 *dl;

/* 8-byte aligned pointer to destination image */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd1, sd2;

/* pointer to end of a line in destination image */
	mlib_u8 *dend;

/*  masks */
	mlib_s32 cmask, cmaskh, cmaskl, emask;

/* temporaries */
	mlib_d64 sd1h, sd1l, sd2h, sd2l;

/* source data */
	mlib_d64 s11, s10;

/* source data */
	mlib_d64 s21, s20;

/* offset of address alignment in destination */
	mlib_s32 off;

/* loop variable */
	mlib_s32 j;
	mlib_f32 fone = vis_to_float(0x1000);

	sa1 = src1;
	sa2 = src2;
	dl = dst;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)dl & (~7));
	off = (mlib_addr)dp - (mlib_addr)dl;
	dend = dl + dsize - 1;

/* generate edge mask for the start point */
	emask = vis_edge8(dl, dend);

/* first 8-pixel column loop */
	sp1 = (mlib_d64 *)vis_alignaddr(sa1, off);
	s10 = sp1[0];
	s11 = sp1[1];
	sd1 = vis_faligndata(s10, s11);
	sp1++;

	sp2 = (mlib_d64 *)vis_alignaddr(sa2, off);
	s20 = sp2[0];
	s21 = sp2[1];
	sd2 = vis_faligndata(s20, s21);
	sp2++;
	MLIB_V_MINIMAGE_U8(sd1, sd2, cmask);
	vis_pst_8(sd1, dp, emask);
	vis_pst_8(sd2, dp, emask & cmask);
	dp++;

	if (sa1 != dl) {

#pragma pipeloop(0)
		for (j = 8 + off; j <= (dsize - 8); j += 8) {
			s10 = s11;
			s11 = sp1[1];
			vis_alignaddr(sa1, off);
			sd1 = vis_faligndata(s10, s11);
			sp1++;

			s20 = s21;
			s21 = sp2[1];
			vis_alignaddr(sa2, off);
			sd2 = vis_faligndata(s20, s21);
			sp2++;

			MLIB_V_MINIMAGE_U8(sd1, sd2, cmask);
			*dp = sd1;
			vis_pst_8(sd2, dp, cmask);
			dp++;
		}
	} else {
		sp1 += ((dsize - off) >> 3) - 1;
		s11 = sp1[0];

#pragma pipeloop(0)
		for (j = 8 + off; j <= (dsize - 8); j += 8) {
			s20 = s21;
			s21 = sp2[1];
			sd2 = vis_faligndata(s20, s21);
			sp2++;

			sd1 = dp[0];
			MLIB_V_MINIMAGE_U8(sd1, sd2, cmask);
			vis_pst_8(sd2, dp, cmask);
			dp++;
		}
	}

/* last 8-pixel column */

	if ((mlib_u8 *)dp <= dend) {
		s20 = s21;
		s21 = sp2[1];
		sd2 = vis_faligndata(s20, s21);

		vis_alignaddr(sa1, off);
		s10 = s11;
		s11 = sp1[1];
		sd1 = vis_faligndata(s10, s11);

		MLIB_V_MINIMAGE_U8(sd1, sd2, cmask);
		emask = vis_edge8(dp, dend);
		vis_pst_8(sd1, dp, emask);
		vis_pst_8(sd2, dp, emask & cmask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageMin_U8(
    mlib_u8 *src1,
    mlib_s32 slb1,
    mlib_u8 *src2,
    mlib_s32 slb2,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* pointers to source images */
	mlib_u8 *sa1, *sa2;

/* pointer to destination image */
	mlib_u8 *dl;

/* loop variable */
	mlib_s32 j;

	sa1 = src1;
	sa2 = src2;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

		mlib_v_ImageMin_U8_1D(sa1, sa2, dl, xsize);

		sa1 += slb1;
		sa2 += slb2;
		dl += dlb;
	}
}

/* *********************************************************** */

#define	MLIB_V_MINIMAGE_S16(sd1, sd2, cmask)                    \
	cmask = vis_fcmplt16(sd1, sd2)

/* *********************************************************** */

void
mlib_v_ImageMin_S16_D1(
    mlib_s16 *src1,
    mlib_s16 *src2,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
/* pointers to source images */
	mlib_s16 *sa1, *sa2;

/* 8-byte aligned pointers to source images */
	mlib_d64 *sp1, *sp2;

/* pointer to destination image */
	mlib_s16 *dl;

/* 8-byte aligned pointer to destination image */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd1, sd2;

/* source data */
	mlib_d64 sd11, sd21;

/* source data */
	mlib_d64 sd12, sd22;

/* source data */
	mlib_d64 sd13, sd23;

/* source data */
	mlib_d64 sd_1, sd_2;

/* source data */
	mlib_d64 sd_11, sd_21;

/* source data */
	mlib_d64 sd_12, sd_22;

/* source data */
	mlib_d64 sd_13, sd_23;

/* pointer to end of a line in destination image */
	mlib_s16 *dend;

/*  masks */
	mlib_s32 cmask, emask, cmaskk;

/*  masks */
	mlib_s32 cmask1, cmaskk1;

/*  masks */
	mlib_s32 cmask2, cmaskk2;

/*  masks */
	mlib_s32 cmask3, cmaskk3;

/* offset of address alignment in destination */
	mlib_s32 off;

/* loop variable */
	mlib_s32 j;

	sa1 = src1;
	sa2 = src2;
	dl = dst;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)dl & (~7));
	off = (mlib_addr)dp - (mlib_addr)dl;
	dend = dl + dsize - 1;

/* generate edge mask for the start point */
	emask = vis_edge16(dl, dend);

/* first 4-pixel column loop */
	sp1 = (mlib_d64 *)vis_alignaddr(sa1, 0);
	sd1 = (*sp1++);
	sp2 = (mlib_d64 *)vis_alignaddr(sa2, 0);
	sd2 = (*sp2++);
	MLIB_V_MINIMAGE_S16(sd1, sd2, cmask);
	vis_pst_16(sd1, dp, emask & cmask);
	vis_pst_16(sd2, dp, emask & ~cmask);
	dp++;

	sd_1 = sp1[0];
	sd_2 = sp2[0];
	sd_11 = sp1[1];
	sd_21 = sp2[1];
	sd_12 = sp1[2];
	sd_22 = sp2[2];
	sd_13 = sp1[3];
	sd_23 = sp2[3];

	MLIB_V_MINIMAGE_S16(sd_1, sd_2, cmaskk);
	MLIB_V_MINIMAGE_S16(sd_11, sd_21, cmaskk1);
	MLIB_V_MINIMAGE_S16(sd_12, sd_22, cmaskk2);
	MLIB_V_MINIMAGE_S16(sd_13, sd_23, cmaskk3);

	sd1 = sp1[4];
	sd2 = sp2[4];
	sd11 = sp1[5];
	sd21 = sp2[5];
	sd12 = sp1[6];
	sd22 = sp2[6];
	sd13 = sp1[7];
	sd23 = sp2[7];

	sp1 += 8;
	sp2 += 8;

	if (src1 != dst) {
#pragma pipeloop(0)
		for (j = 4 + (off >> 1); j <= (dsize - 16); j += 16) {

			MLIB_V_MINIMAGE_S16(sd1, sd2, cmask);
			MLIB_V_MINIMAGE_S16(sd11, sd21, cmask1);
			MLIB_V_MINIMAGE_S16(sd12, sd22, cmask2);
			MLIB_V_MINIMAGE_S16(sd13, sd23, cmask3);

			vis_pst_16(sd_1, dp, cmaskk);
			vis_pst_16(sd_2, dp, ~cmaskk);
			vis_pst_16(sd_11, dp + 1, cmaskk1);
			vis_pst_16(sd_21, dp + 1, ~cmaskk1);
			vis_pst_16(sd_12, dp + 2, cmaskk2);
			vis_pst_16(sd_22, dp + 2, ~cmaskk2);
			vis_pst_16(sd_13, dp + 3, cmaskk3);
			vis_pst_16(sd_23, dp + 3, ~cmaskk3);

			cmaskk = cmask;
			cmaskk1 = cmask1;
			cmaskk2 = cmask2;
			cmaskk3 = cmask3;

			sd_1 = sd1;
			sd_2 = sd2;
			sd_11 = sd11;
			sd_21 = sd21;
			sd_12 = sd12;
			sd_22 = sd22;
			sd_13 = sd13;
			sd_23 = sd23;

			sd1 = sp1[0];
			sd2 = sp2[0];
			sd11 = sp1[1];
			sd21 = sp2[1];
			sd12 = sp1[2];
			sd22 = sp2[2];
			sd13 = sp1[3];
			sd23 = sp2[3];

			sp1 += 4;
			sp2 += 4;
			dp += 4;
		}
	} else {
#pragma pipeloop(0)
		for (j = 4 + (off >> 1); j <= (dsize - 16); j += 16) {

			MLIB_V_MINIMAGE_S16(sd1, sd2, cmask);
			MLIB_V_MINIMAGE_S16(sd11, sd21, cmask1);
			MLIB_V_MINIMAGE_S16(sd12, sd22, cmask2);
			MLIB_V_MINIMAGE_S16(sd13, sd23, cmask3);

			vis_pst_16(sd_2, dp, ~cmaskk);
			vis_pst_16(sd_21, dp + 1, ~cmaskk1);
			vis_pst_16(sd_22, dp + 2, ~cmaskk2);
			vis_pst_16(sd_23, dp + 3, ~cmaskk3);

			cmaskk = cmask;
			cmaskk1 = cmask1;
			cmaskk2 = cmask2;
			cmaskk3 = cmask3;

			sd_1 = sd1;
			sd_2 = sd2;
			sd_11 = sd11;
			sd_21 = sd21;
			sd_12 = sd12;
			sd_22 = sd22;
			sd_13 = sd13;
			sd_23 = sd23;

			sd1 = dp[8];
			sd2 = sp2[0];
			sd11 = dp[9];
			sd21 = sp2[1];
			sd12 = dp[10];
			sd22 = sp2[2];
			sd13 = dp[11];
			sd23 = sp2[3];

			sp2 += 4;
			dp += 4;
		}
	}

/* last 16-pixels */

	if (dend >= (mlib_s16 *)dp) {
		emask = vis_edge16(dp, dend);
		vis_pst_16(sd_1, dp, cmaskk & emask);
		vis_pst_16(sd_2, dp, ~cmaskk & emask);
		dp++;

		if (dend >= (mlib_s16 *)dp) {
			emask = vis_edge16(dp, dend);
			vis_pst_16(sd_11, dp, cmaskk1 & emask);
			vis_pst_16(sd_21, dp, ~cmaskk1 & emask);
			dp++;

			if (dend >= (mlib_s16 *)dp) {
				emask = vis_edge16(dp, dend);
				vis_pst_16(sd_12, dp, cmaskk2 & emask);
				vis_pst_16(sd_22, dp, ~cmaskk2 & emask);
				dp++;

				if (dend >= (mlib_s16 *)dp) {
					emask = vis_edge16(dp, dend);
					vis_pst_16(sd_13, dp, cmaskk3 & emask);
					vis_pst_16(sd_23, dp, ~cmaskk3 & emask);
				}
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageMin_S16_A8D2(
    mlib_s16 *src1,
    mlib_s32 slb1,
    mlib_s16 *src2,
    mlib_s32 slb2,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* 8-byte aligned pointers to source images */
	mlib_d64 *sp1, *sp2;

/* pointers to a line in source */
	mlib_d64 *sl1, *sl2;

/* pointer to a line in destination */
	mlib_d64 *dl;

/* 8-byte aligned pointer to destination image */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd1, sd2;

/* source data */
	mlib_d64 sd11, sd21;

/* source data */
	mlib_d64 sd12, sd22;

/* source data */
	mlib_d64 sd13, sd23;

/* source data */
	mlib_d64 sd_1, sd_2;

/* source data */
	mlib_d64 sd_11, sd_21;

/* source data */
	mlib_d64 sd_12, sd_22;

/* source data */
	mlib_d64 sd_13, sd_23;

/* pointer to end of a line in destination image */
	mlib_s16 *dend;

/*  masks */
	mlib_s32 cmask, emask, cmaskk;

/*  masks */
	mlib_s32 cmask1, cmaskk1;

/*  masks */
	mlib_s32 cmask2, cmaskk2;

/*  masks */
	mlib_s32 cmask3, cmaskk3;

/* loop variable */
	mlib_s32 i, j;

	sl1 = sp1 = (mlib_d64 *)src1;
	sl2 = sp2 = (mlib_d64 *)src2;
	dl = dp = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {
		dend = (mlib_s16 *)dl + xsize - 1;

/* 16-pixel loop */
		sd_1 = sp1[0];
		sd_2 = sp2[0];
		sd_11 = sp1[1];
		sd_21 = sp2[1];
		sd_12 = sp1[2];
		sd_22 = sp2[2];
		sd_13 = sp1[3];
		sd_23 = sp2[3];

		MLIB_V_MINIMAGE_S16(sd_1, sd_2, cmaskk);
		MLIB_V_MINIMAGE_S16(sd_11, sd_21, cmaskk1);
		MLIB_V_MINIMAGE_S16(sd_12, sd_22, cmaskk2);
		MLIB_V_MINIMAGE_S16(sd_13, sd_23, cmaskk3);

		sd1 = sp1[4];
		sd2 = sp2[4];
		sd11 = sp1[5];
		sd21 = sp2[5];
		sd12 = sp1[6];
		sd22 = sp2[6];
		sd13 = sp1[7];
		sd23 = sp2[7];

		sp1 += 8;
		sp2 += 8;

		if (src1 != dst) {
#pragma pipeloop(0)
			for (i = 0; i <= (xsize - 16); i += 16) {

				MLIB_V_MINIMAGE_S16(sd1, sd2, cmask);
				MLIB_V_MINIMAGE_S16(sd11, sd21, cmask1);
				MLIB_V_MINIMAGE_S16(sd12, sd22, cmask2);
				MLIB_V_MINIMAGE_S16(sd13, sd23, cmask3);

				vis_pst_16(sd_1, dp, cmaskk);
				vis_pst_16(sd_2, dp, ~cmaskk);
				vis_pst_16(sd_11, dp + 1, cmaskk1);
				vis_pst_16(sd_21, dp + 1, ~cmaskk1);
				vis_pst_16(sd_12, dp + 2, cmaskk2);
				vis_pst_16(sd_22, dp + 2, ~cmaskk2);
				vis_pst_16(sd_13, dp + 3, cmaskk3);
				vis_pst_16(sd_23, dp + 3, ~cmaskk3);

				cmaskk = cmask;
				cmaskk1 = cmask1;
				cmaskk2 = cmask2;
				cmaskk3 = cmask3;

				sd_1 = sd1;
				sd_2 = sd2;
				sd_11 = sd11;
				sd_21 = sd21;
				sd_12 = sd12;
				sd_22 = sd22;
				sd_13 = sd13;
				sd_23 = sd23;

				sd1 = sp1[0];
				sd2 = sp2[0];
				sd11 = sp1[1];
				sd21 = sp2[1];
				sd12 = sp1[2];
				sd22 = sp2[2];
				sd13 = sp1[3];
				sd23 = sp2[3];

				sp1 += 4;
				sp2 += 4;
				dp += 4;
			}
		} else {
#pragma pipeloop(0)
			for (i = 0; i <= (xsize - 16); i += 16) {

				MLIB_V_MINIMAGE_S16(sd1, sd2, cmask);
				MLIB_V_MINIMAGE_S16(sd11, sd21, cmask1);
				MLIB_V_MINIMAGE_S16(sd12, sd22, cmask2);
				MLIB_V_MINIMAGE_S16(sd13, sd23, cmask3);

				vis_pst_16(sd_2, dp, ~cmaskk);
				vis_pst_16(sd_21, dp + 1, ~cmaskk1);
				vis_pst_16(sd_22, dp + 2, ~cmaskk2);
				vis_pst_16(sd_23, dp + 3, ~cmaskk3);

				cmaskk = cmask;
				cmaskk1 = cmask1;
				cmaskk2 = cmask2;
				cmaskk3 = cmask3;

				sd_1 = sd1;
				sd_2 = sd2;
				sd_11 = sd11;
				sd_21 = sd21;
				sd_12 = sd12;
				sd_22 = sd22;
				sd_13 = sd13;
				sd_23 = sd23;

				sd1 = dp[8];
				sd2 = sp2[0];
				sd11 = dp[9];
				sd21 = sp2[1];
				sd12 = dp[10];
				sd22 = sp2[2];
				sd13 = dp[11];
				sd23 = sp2[3];

				sp2 += 4;
				dp += 4;
			}
		}

/* last 16-pixels */

		if (dend >= (mlib_s16 *)dp) {
			emask = vis_edge16(dp, dend);
			vis_pst_16(sd_1, dp, cmaskk & emask);
			vis_pst_16(sd_2, dp, ~cmaskk & emask);
			dp++;

			if (dend >= (mlib_s16 *)dp) {
				emask = vis_edge16(dp, dend);
				vis_pst_16(sd_11, dp, cmaskk1 & emask);
				vis_pst_16(sd_21, dp, ~cmaskk1 & emask);
				dp++;

				if (dend >= (mlib_s16 *)dp) {
					emask = vis_edge16(dp, dend);
					vis_pst_16(sd_12, dp, cmaskk2 & emask);
					vis_pst_16(sd_22, dp, ~cmaskk2 & emask);
					dp++;

					if (dend >= (mlib_s16 *)dp) {
						emask = vis_edge16(dp, dend);
						vis_pst_16(sd_13, dp,
						    cmaskk3 & emask);
						vis_pst_16(sd_23, dp,
						    ~cmaskk3 & emask);
					}
				}
			}
		}

		sl1 = sp1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
		sl2 = sp2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
		dl = dp = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageMin_S16_1D(
    mlib_s16 *src1,
    mlib_s16 *src2,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
/* pointers to source images */
	mlib_s16 *sa1, *sa2;

/* 8-byte aligned pointers to source images */
	mlib_d64 *sp1, *sp2;

/* pointer to destination image */
	mlib_s16 *dl;

/* 8-byte aligned pointer to destination image */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd1, sd2;

/* source data */
	mlib_d64 sd11, sd21;

/* source data */
	mlib_d64 sd12, sd22;

/* source data */
	mlib_d64 sd13, sd23;

/* source data */
	mlib_d64 sd_1, sd_2;

/* source data */
	mlib_d64 sd_11, sd_21;

/* source data */
	mlib_d64 sd_12, sd_22;

/* source data */
	mlib_d64 sd_13, sd_23;

/* pointer to end of a line in destination image */
	mlib_s16 *dend;

/*  masks */
	mlib_s32 cmask, emask, cmaskk;

/*  masks */
	mlib_s32 cmask1, cmaskk1;

/*  masks */
	mlib_s32 cmask2, cmaskk2;

/*  masks */
	mlib_s32 cmask3, cmaskk3;

/* source data */
	mlib_d64 s11, s10;

/* source data */
	mlib_d64 s21, s20;

/* offset of address alignment in destination */
	mlib_s32 off;

/* loop variable */
	mlib_s32 j;

	sa1 = src1;
	sa2 = src2;
	dl = dst;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)dl & (~7));
	off = (mlib_addr)dp - (mlib_addr)dl;
	dend = dl + dsize - 1;

/* generate edge mask for the start point */
	emask = vis_edge16(dl, dend);

/* first 4-pixel column loop */
	sp1 = (mlib_d64 *)vis_alignaddr(sa1, off);
	s10 = sp1[0];
	s11 = sp1[1];
	sd1 = vis_faligndata(s10, s11);
	sp1++;

	sp2 = (mlib_d64 *)vis_alignaddr(sa2, off);
	s20 = sp2[0];
	s21 = sp2[1];
	sd2 = vis_faligndata(s20, s21);
	sp2++;
	MLIB_V_MINIMAGE_S16(sd1, sd2, cmask);
	vis_pst_16(sd1, dp, emask & cmask);
	vis_pst_16(sd2, dp, emask & ~cmask);
	dp++;

	s20 = sp2[1];
	sd_2 = vis_faligndata(s21, s20);
	s21 = sp2[2];
	sd_21 = vis_faligndata(s20, s21);
	s20 = sp2[3];
	sd_22 = vis_faligndata(s21, s20);
	s21 = sp2[4];
	sd_23 = vis_faligndata(s20, s21);

	vis_alignaddr(sa1, off);
	s10 = sp1[1];
	sd_1 = vis_faligndata(s11, s10);
	s11 = sp1[2];
	sd_11 = vis_faligndata(s10, s11);
	s10 = sp1[3];
	sd_12 = vis_faligndata(s11, s10);
	s11 = sp1[4];
	sd_13 = vis_faligndata(s10, s11);

	MLIB_V_MINIMAGE_S16(sd_1, sd_2, cmaskk);
	MLIB_V_MINIMAGE_S16(sd_11, sd_21, cmaskk1);
	MLIB_V_MINIMAGE_S16(sd_12, sd_22, cmaskk2);
	MLIB_V_MINIMAGE_S16(sd_13, sd_23, cmaskk3);

	s10 = sp1[5];
	sd1 = vis_faligndata(s11, s10);
	s11 = sp1[6];
	sd11 = vis_faligndata(s10, s11);
	s10 = sp1[7];
	sd12 = vis_faligndata(s11, s10);
	s11 = sp1[8];
	sd13 = vis_faligndata(s10, s11);

	vis_alignaddr(sa2, off);
	s20 = sp2[5];
	sd2 = vis_faligndata(s21, s20);
	s21 = sp2[6];
	sd21 = vis_faligndata(s20, s21);
	s20 = sp2[7];
	sd22 = vis_faligndata(s21, s20);
	s21 = sp2[8];
	sd23 = vis_faligndata(s20, s21);

	sp1 += 8;
	sp2 += 8;

	if (src1 != dst) {

		mlib_s32 byte_off = (mlib_s32)((((mlib_addr)sa1) + off) & 7);
		mlib_s32 mask;

		vis_alignaddr(sa2, off);
		mask = bmask_min_table[byte_off];
		vis_write_bmask(mask, 0);

#pragma pipeloop(0)
		for (j = 4 + (off >> 1); j <= (dsize - 16); j += 16) {

			MLIB_V_MINIMAGE_S16(sd1, sd2, cmask);
			MLIB_V_MINIMAGE_S16(sd11, sd21, cmask1);
			MLIB_V_MINIMAGE_S16(sd12, sd22, cmask2);
			MLIB_V_MINIMAGE_S16(sd13, sd23, cmask3);

			vis_pst_16(sd_1, dp, cmaskk);
			vis_pst_16(sd_2, dp, ~cmaskk);
			vis_pst_16(sd_11, dp + 1, cmaskk1);
			vis_pst_16(sd_21, dp + 1, ~cmaskk1);
			vis_pst_16(sd_12, dp + 2, cmaskk2);
			vis_pst_16(sd_22, dp + 2, ~cmaskk2);
			vis_pst_16(sd_13, dp + 3, cmaskk3);
			vis_pst_16(sd_23, dp + 3, ~cmaskk3);

			cmaskk = cmask;
			cmaskk1 = cmask1;
			cmaskk2 = cmask2;
			cmaskk3 = cmask3;

			sd_1 = sd1;
			sd_2 = sd2;
			sd_11 = sd11;
			sd_21 = sd21;
			sd_12 = sd12;
			sd_22 = sd22;
			sd_13 = sd13;
			sd_23 = sd23;

			s10 = sp1[1];
			sd1 = vis_bshuffle(s11, s10);
			s11 = sp1[2];
			sd11 = vis_bshuffle(s10, s11);
			s10 = sp1[3];
			sd12 = vis_bshuffle(s11, s10);
			s11 = sp1[4];
			sd13 = vis_bshuffle(s10, s11);

			s20 = sp2[1];
			sd2 = vis_faligndata(s21, s20);
			s21 = sp2[2];
			sd21 = vis_faligndata(s20, s21);
			s20 = sp2[3];
			sd22 = vis_faligndata(s21, s20);
			s21 = sp2[4];
			sd23 = vis_faligndata(s20, s21);

			sp1 += 4;
			sp2 += 4;
			dp += 4;
		}
	} else {
#pragma pipeloop(0)
		for (j = 4 + (off >> 1); j <= (dsize - 16); j += 16) {

			MLIB_V_MINIMAGE_S16(sd1, sd2, cmask);
			MLIB_V_MINIMAGE_S16(sd11, sd21, cmask1);
			MLIB_V_MINIMAGE_S16(sd12, sd22, cmask2);
			MLIB_V_MINIMAGE_S16(sd13, sd23, cmask3);

			vis_pst_16(sd_2, dp, ~cmaskk);
			vis_pst_16(sd_21, dp + 1, ~cmaskk1);
			vis_pst_16(sd_22, dp + 2, ~cmaskk2);
			vis_pst_16(sd_23, dp + 3, ~cmaskk3);

			cmaskk = cmask;
			cmaskk1 = cmask1;
			cmaskk2 = cmask2;
			cmaskk3 = cmask3;

			sd_1 = sd1;
			sd_2 = sd2;
			sd_11 = sd11;
			sd_21 = sd21;
			sd_12 = sd12;
			sd_22 = sd22;
			sd_13 = sd13;
			sd_23 = sd23;

			sd1 = dp[8];
			sd11 = dp[9];
			sd12 = dp[10];
			sd13 = dp[11];

			s20 = sp2[1];
			sd2 = vis_faligndata(s21, s20);
			s21 = sp2[2];
			sd21 = vis_faligndata(s20, s21);
			s20 = sp2[3];
			sd22 = vis_faligndata(s21, s20);
			s21 = sp2[4];
			sd23 = vis_faligndata(s20, s21);

			sp2 += 4;
			dp += 4;
		}
	}

/* last 16-pixels */

	if (dend >= (mlib_s16 *)dp) {
		emask = vis_edge16(dp, dend);
		vis_pst_16(sd_1, dp, cmaskk & emask);
		vis_pst_16(sd_2, dp, ~cmaskk & emask);
		dp++;

		if (dend >= (mlib_s16 *)dp) {
			emask = vis_edge16(dp, dend);
			vis_pst_16(sd_11, dp, cmaskk1 & emask);
			vis_pst_16(sd_21, dp, ~cmaskk1 & emask);
			dp++;

			if (dend >= (mlib_s16 *)dp) {
				emask = vis_edge16(dp, dend);
				vis_pst_16(sd_12, dp, cmaskk2 & emask);
				vis_pst_16(sd_22, dp, ~cmaskk2 & emask);
				dp++;

				if (dend >= (mlib_s16 *)dp) {
					emask = vis_edge16(dp, dend);
					vis_pst_16(sd_13, dp, cmaskk3 & emask);
					vis_pst_16(sd_23, dp, ~cmaskk3 & emask);
				}
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageMin_S16(
    mlib_s16 *src1,
    mlib_s32 slb1,
    mlib_s16 *src2,
    mlib_s32 slb2,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* pointers to source images */
	mlib_s16 *sa1, *sa2;

/* pointer to destination image */
	mlib_s16 *dl;

/* loop variable */
	mlib_s32 j;

	sa1 = src1;
	sa2 = src2;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

		mlib_v_ImageMin_S16_1D(sa1, sa2, dl, xsize);

		sa1 = (mlib_s16 *)((mlib_u8 *)sa1 + slb1);
		sa2 = (mlib_s16 *)((mlib_u8 *)sa2 + slb2);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

#pragma align 64(mlib_32768x4)

static const mlib_u16 mlib_32768x4[] = { 0x8000, 0x8000, 0x8000, 0x8000 };

/* *********************************************************** */

#define	MLIB_V_MINIMAGE_U16(sd1, sd2, cmask)                        \
	cmask =                                                     \
	vis_fcmplt16(vis_fpsub16(sd1, *(mlib_d64 *)mlib_32768x4),   \
	    vis_fpsub16(sd2, *(mlib_d64 *)mlib_32768x4))

/* *********************************************************** */

void
mlib_v_ImageMin_U16_D1(
    mlib_u16 *src1,
    mlib_u16 *src2,
    mlib_u16 *dst,
    mlib_s32 dsize)
{
/* pointers to source images */
	mlib_u16 *sa1, *sa2;

/* 8-byte aligned pointers to source images */
	mlib_d64 *sp1, *sp2;

/* pointer to destination image */
	mlib_u16 *dl;

/* 8-byte aligned pointer to destination image */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd1, sd2;

/* source data */
	mlib_d64 sd11, sd21;

/* source data */
	mlib_d64 sd12, sd22;

/* source data */
	mlib_d64 sd13, sd23;

/* source data */
	mlib_d64 sd_1, sd_2;

/* source data */
	mlib_d64 sd_11, sd_21;

/* source data */
	mlib_d64 sd_12, sd_22;

/* source data */
	mlib_d64 sd_13, sd_23;

/* pointer to end of a line in destination image */
	mlib_u16 *dend;

/*  masks */
	mlib_s32 cmask, emask, cmaskk;

/*  masks */
	mlib_s32 cmask1, cmaskk1;

/*  masks */
	mlib_s32 cmask2, cmaskk2;

/*  masks */
	mlib_s32 cmask3, cmaskk3;

/* offset of address alignment in destination */
	mlib_s32 off;

/* loop variable */
	mlib_s32 j;

	sa1 = src1;
	sa2 = src2;
	dl = dst;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)dl & (~7));
	off = (mlib_addr)dp - (mlib_addr)dl;
	dend = dl + dsize - 1;

/* generate edge mask for the start point */
	emask = vis_edge16(dl, dend);

/* first 4-pixel column loop */
	sp1 = (mlib_d64 *)vis_alignaddr(sa1, 0);
	sd1 = (*sp1++);
	sp2 = (mlib_d64 *)vis_alignaddr(sa2, 0);
	sd2 = (*sp2++);
	MLIB_V_MINIMAGE_U16(sd1, sd2, cmask);
	vis_pst_16(sd1, dp, emask & cmask);
	vis_pst_16(sd2, dp, emask & ~cmask);
	dp++;

	sd_1 = sp1[0];
	sd_2 = sp2[0];
	sd_11 = sp1[1];
	sd_21 = sp2[1];
	sd_12 = sp1[2];
	sd_22 = sp2[2];
	sd_13 = sp1[3];
	sd_23 = sp2[3];

	MLIB_V_MINIMAGE_U16(sd_1, sd_2, cmaskk);
	MLIB_V_MINIMAGE_U16(sd_11, sd_21, cmaskk1);
	MLIB_V_MINIMAGE_U16(sd_12, sd_22, cmaskk2);
	MLIB_V_MINIMAGE_U16(sd_13, sd_23, cmaskk3);

	sd1 = sp1[4];
	sd2 = sp2[4];
	sd11 = sp1[5];
	sd21 = sp2[5];
	sd12 = sp1[6];
	sd22 = sp2[6];
	sd13 = sp1[7];
	sd23 = sp2[7];

	sp1 += 8;
	sp2 += 8;

	if (src1 != dst) {
#pragma pipeloop(0)
		for (j = 4 + (off >> 1); j <= (dsize - 16); j += 16) {

			MLIB_V_MINIMAGE_U16(sd1, sd2, cmask);
			MLIB_V_MINIMAGE_U16(sd11, sd21, cmask1);
			MLIB_V_MINIMAGE_U16(sd12, sd22, cmask2);
			MLIB_V_MINIMAGE_U16(sd13, sd23, cmask3);

			vis_pst_16(sd_1, dp, cmaskk);
			vis_pst_16(sd_2, dp, ~cmaskk);
			vis_pst_16(sd_11, dp + 1, cmaskk1);
			vis_pst_16(sd_21, dp + 1, ~cmaskk1);
			vis_pst_16(sd_12, dp + 2, cmaskk2);
			vis_pst_16(sd_22, dp + 2, ~cmaskk2);
			vis_pst_16(sd_13, dp + 3, cmaskk3);
			vis_pst_16(sd_23, dp + 3, ~cmaskk3);

			cmaskk = cmask;
			cmaskk1 = cmask1;
			cmaskk2 = cmask2;
			cmaskk3 = cmask3;

			sd_1 = sd1;
			sd_2 = sd2;
			sd_11 = sd11;
			sd_21 = sd21;
			sd_12 = sd12;
			sd_22 = sd22;
			sd_13 = sd13;
			sd_23 = sd23;

			sd1 = sp1[0];
			sd2 = sp2[0];
			sd11 = sp1[1];
			sd21 = sp2[1];
			sd12 = sp1[2];
			sd22 = sp2[2];
			sd13 = sp1[3];
			sd23 = sp2[3];

			sp1 += 4;
			sp2 += 4;
			dp += 4;
		}
	} else {
#pragma pipeloop(0)
		for (j = 4 + (off >> 1); j <= (dsize - 16); j += 16) {

			MLIB_V_MINIMAGE_U16(sd1, sd2, cmask);
			MLIB_V_MINIMAGE_U16(sd11, sd21, cmask1);
			MLIB_V_MINIMAGE_U16(sd12, sd22, cmask2);
			MLIB_V_MINIMAGE_U16(sd13, sd23, cmask3);

			vis_pst_16(sd_2, dp, ~cmaskk);
			vis_pst_16(sd_21, dp + 1, ~cmaskk1);
			vis_pst_16(sd_22, dp + 2, ~cmaskk2);
			vis_pst_16(sd_23, dp + 3, ~cmaskk3);

			cmaskk = cmask;
			cmaskk1 = cmask1;
			cmaskk2 = cmask2;
			cmaskk3 = cmask3;

			sd_1 = sd1;
			sd_2 = sd2;
			sd_11 = sd11;
			sd_21 = sd21;
			sd_12 = sd12;
			sd_22 = sd22;
			sd_13 = sd13;
			sd_23 = sd23;

			sd1 = dp[8];
			sd2 = sp2[0];
			sd11 = dp[9];
			sd21 = sp2[1];
			sd12 = dp[10];
			sd22 = sp2[2];
			sd13 = dp[11];
			sd23 = sp2[3];

			sp2 += 4;
			dp += 4;
		}
	}

/* last 16-pixels */

	if (dend >= (mlib_u16 *)dp) {
		emask = vis_edge16(dp, dend);
		vis_pst_16(sd_1, dp, cmaskk & emask);
		vis_pst_16(sd_2, dp, ~cmaskk & emask);
		dp++;

		if (dend >= (mlib_u16 *)dp) {
			emask = vis_edge16(dp, dend);
			vis_pst_16(sd_11, dp, cmaskk1 & emask);
			vis_pst_16(sd_21, dp, ~cmaskk1 & emask);
			dp++;

			if (dend >= (mlib_u16 *)dp) {
				emask = vis_edge16(dp, dend);
				vis_pst_16(sd_12, dp, cmaskk2 & emask);
				vis_pst_16(sd_22, dp, ~cmaskk2 & emask);
				dp++;

				if (dend >= (mlib_u16 *)dp) {
					emask = vis_edge16(dp, dend);
					vis_pst_16(sd_13, dp, cmaskk3 & emask);
					vis_pst_16(sd_23, dp, ~cmaskk3 & emask);
				}
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageMin_U16_A8D2(
    mlib_u16 *src1,
    mlib_s32 slb1,
    mlib_u16 *src2,
    mlib_s32 slb2,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* 8-byte aligned pointers to source images */
	mlib_d64 *sp1, *sp2;

/* pointers to a line in source */
	mlib_d64 *sl1, *sl2;

/* pointer to a line in destination */
	mlib_d64 *dl;

/* 8-byte aligned pointer to destination image */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd1, sd2;

/* source data */
	mlib_d64 sd11, sd21;

/* source data */
	mlib_d64 sd12, sd22;

/* source data */
	mlib_d64 sd13, sd23;

/* source data */
	mlib_d64 sd_1, sd_2;

/* source data */
	mlib_d64 sd_11, sd_21;

/* source data */
	mlib_d64 sd_12, sd_22;

/* source data */
	mlib_d64 sd_13, sd_23;

/* pointer to end of a line in destination image */
	mlib_u16 *dend;

/*  masks */
	mlib_s32 cmask, emask, cmaskk;

/*  masks */
	mlib_s32 cmask1, cmaskk1;

/*  masks */
	mlib_s32 cmask2, cmaskk2;

/*  masks */
	mlib_s32 cmask3, cmaskk3;

/* loop variable */
	mlib_s32 i, j;

	sl1 = sp1 = (mlib_d64 *)src1;
	sl2 = sp2 = (mlib_d64 *)src2;
	dl = dp = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {
		dend = (mlib_u16 *)dl + xsize - 1;

/* 16-pixel loop */
		sd_1 = sp1[0];
		sd_2 = sp2[0];
		sd_11 = sp1[1];
		sd_21 = sp2[1];
		sd_12 = sp1[2];
		sd_22 = sp2[2];
		sd_13 = sp1[3];
		sd_23 = sp2[3];

		MLIB_V_MINIMAGE_U16(sd_1, sd_2, cmaskk);
		MLIB_V_MINIMAGE_U16(sd_11, sd_21, cmaskk1);
		MLIB_V_MINIMAGE_U16(sd_12, sd_22, cmaskk2);
		MLIB_V_MINIMAGE_U16(sd_13, sd_23, cmaskk3);

		sd1 = sp1[4];
		sd2 = sp2[4];
		sd11 = sp1[5];
		sd21 = sp2[5];
		sd12 = sp1[6];
		sd22 = sp2[6];
		sd13 = sp1[7];
		sd23 = sp2[7];

		sp1 += 8;
		sp2 += 8;

		if (src1 != dst) {
#pragma pipeloop(0)
			for (i = 0; i <= (xsize - 16); i += 16) {

				MLIB_V_MINIMAGE_U16(sd1, sd2, cmask);
				MLIB_V_MINIMAGE_U16(sd11, sd21, cmask1);
				MLIB_V_MINIMAGE_U16(sd12, sd22, cmask2);
				MLIB_V_MINIMAGE_U16(sd13, sd23, cmask3);

				vis_pst_16(sd_1, dp, cmaskk);
				vis_pst_16(sd_2, dp, ~cmaskk);
				vis_pst_16(sd_11, dp + 1, cmaskk1);
				vis_pst_16(sd_21, dp + 1, ~cmaskk1);
				vis_pst_16(sd_12, dp + 2, cmaskk2);
				vis_pst_16(sd_22, dp + 2, ~cmaskk2);
				vis_pst_16(sd_13, dp + 3, cmaskk3);
				vis_pst_16(sd_23, dp + 3, ~cmaskk3);

				cmaskk = cmask;
				cmaskk1 = cmask1;
				cmaskk2 = cmask2;
				cmaskk3 = cmask3;

				sd_1 = sd1;
				sd_2 = sd2;
				sd_11 = sd11;
				sd_21 = sd21;
				sd_12 = sd12;
				sd_22 = sd22;
				sd_13 = sd13;
				sd_23 = sd23;

				sd1 = sp1[0];
				sd2 = sp2[0];
				sd11 = sp1[1];
				sd21 = sp2[1];
				sd12 = sp1[2];
				sd22 = sp2[2];
				sd13 = sp1[3];
				sd23 = sp2[3];

				sp1 += 4;
				sp2 += 4;
				dp += 4;
			}
		} else {
#pragma pipeloop(0)
			for (i = 0; i <= (xsize - 16); i += 16) {

				MLIB_V_MINIMAGE_U16(sd1, sd2, cmask);
				MLIB_V_MINIMAGE_U16(sd11, sd21, cmask1);
				MLIB_V_MINIMAGE_U16(sd12, sd22, cmask2);
				MLIB_V_MINIMAGE_U16(sd13, sd23, cmask3);

				vis_pst_16(sd_2, dp, ~cmaskk);
				vis_pst_16(sd_21, dp + 1, ~cmaskk1);
				vis_pst_16(sd_22, dp + 2, ~cmaskk2);
				vis_pst_16(sd_23, dp + 3, ~cmaskk3);

				cmaskk = cmask;
				cmaskk1 = cmask1;
				cmaskk2 = cmask2;
				cmaskk3 = cmask3;

				sd_1 = sd1;
				sd_2 = sd2;
				sd_11 = sd11;
				sd_21 = sd21;
				sd_12 = sd12;
				sd_22 = sd22;
				sd_13 = sd13;
				sd_23 = sd23;

				sd1 = dp[8];
				sd2 = sp2[0];
				sd11 = dp[9];
				sd21 = sp2[1];
				sd12 = dp[10];
				sd22 = sp2[2];
				sd13 = dp[11];
				sd23 = sp2[3];

				sp2 += 4;
				dp += 4;
			}
		}

/* last 16-pixels */

		if (dend >= (mlib_u16 *)dp) {
			emask = vis_edge16(dp, dend);
			vis_pst_16(sd_1, dp, cmaskk & emask);
			vis_pst_16(sd_2, dp, ~cmaskk & emask);
			dp++;

			if (dend >= (mlib_u16 *)dp) {
				emask = vis_edge16(dp, dend);
				vis_pst_16(sd_11, dp, cmaskk1 & emask);
				vis_pst_16(sd_21, dp, ~cmaskk1 & emask);
				dp++;

				if (dend >= (mlib_u16 *)dp) {
					emask = vis_edge16(dp, dend);
					vis_pst_16(sd_12, dp, cmaskk2 & emask);
					vis_pst_16(sd_22, dp, ~cmaskk2 & emask);
					dp++;

					if (dend >= (mlib_u16 *)dp) {
						emask = vis_edge16(dp, dend);
						vis_pst_16(sd_13, dp,
						    cmaskk3 & emask);
						vis_pst_16(sd_23, dp,
						    ~cmaskk3 & emask);
					}
				}
			}
		}

		sl1 = sp1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
		sl2 = sp2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
		dl = dp = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageMin_U16_1D(
    mlib_u16 *src1,
    mlib_u16 *src2,
    mlib_u16 *dst,
    mlib_s32 dsize)
{
/* pointers to source images */
	mlib_u16 *sa1, *sa2;

/* 8-byte aligned pointers to source images */
	mlib_d64 *sp1, *sp2;

/* pointer to destination image */
	mlib_u16 *dl;

/* 8-byte aligned pointer to destination image */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd1, sd2;

/* source data */
	mlib_d64 sd11, sd21;

/* source data */
	mlib_d64 sd12, sd22;

/* source data */
	mlib_d64 sd13, sd23;

/* source data */
	mlib_d64 sd_1, sd_2;

/* source data */
	mlib_d64 sd_11, sd_21;

/* source data */
	mlib_d64 sd_12, sd_22;

/* source data */
	mlib_d64 sd_13, sd_23;

/* pointer to end of a line in destination image */
	mlib_u16 *dend;

/*  masks */
	mlib_s32 cmask, emask, cmaskk;

/*  masks */
	mlib_s32 cmask1, cmaskk1;

/*  masks */
	mlib_s32 cmask2, cmaskk2;

/*  masks */
	mlib_s32 cmask3, cmaskk3;

/* source data */
	mlib_d64 s11, s10;

/* source data */
	mlib_d64 s21, s20;

/* offset of address alignment in destination */
	mlib_s32 off;

/* loop variable */
	mlib_s32 j;

	sa1 = src1;
	sa2 = src2;
	dl = dst;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)dl & (~7));
	off = (mlib_addr)dp - (mlib_addr)dl;
	dend = dl + dsize - 1;

/* generate edge mask for the start point */
	emask = vis_edge16(dl, dend);

/* first 4-pixel column loop */
	sp1 = (mlib_d64 *)vis_alignaddr(sa1, off);
	s10 = sp1[0];
	s11 = sp1[1];
	sd1 = vis_faligndata(s10, s11);
	sp1++;

	sp2 = (mlib_d64 *)vis_alignaddr(sa2, off);
	s20 = sp2[0];
	s21 = sp2[1];
	sd2 = vis_faligndata(s20, s21);
	sp2++;
	MLIB_V_MINIMAGE_U16(sd1, sd2, cmask);
	vis_pst_16(sd1, dp, emask & cmask);
	vis_pst_16(sd2, dp, emask & ~cmask);
	dp++;

	s20 = sp2[1];
	sd_2 = vis_faligndata(s21, s20);
	s21 = sp2[2];
	sd_21 = vis_faligndata(s20, s21);
	s20 = sp2[3];
	sd_22 = vis_faligndata(s21, s20);
	s21 = sp2[4];
	sd_23 = vis_faligndata(s20, s21);

	vis_alignaddr(sa1, off);
	s10 = sp1[1];
	sd_1 = vis_faligndata(s11, s10);
	s11 = sp1[2];
	sd_11 = vis_faligndata(s10, s11);
	s10 = sp1[3];
	sd_12 = vis_faligndata(s11, s10);
	s11 = sp1[4];
	sd_13 = vis_faligndata(s10, s11);

	MLIB_V_MINIMAGE_U16(sd_1, sd_2, cmaskk);
	MLIB_V_MINIMAGE_U16(sd_11, sd_21, cmaskk1);
	MLIB_V_MINIMAGE_U16(sd_12, sd_22, cmaskk2);
	MLIB_V_MINIMAGE_U16(sd_13, sd_23, cmaskk3);

	s10 = sp1[5];
	sd1 = vis_faligndata(s11, s10);
	s11 = sp1[6];
	sd11 = vis_faligndata(s10, s11);
	s10 = sp1[7];
	sd12 = vis_faligndata(s11, s10);
	s11 = sp1[8];
	sd13 = vis_faligndata(s10, s11);

	vis_alignaddr(sa2, off);
	s20 = sp2[5];
	sd2 = vis_faligndata(s21, s20);
	s21 = sp2[6];
	sd21 = vis_faligndata(s20, s21);
	s20 = sp2[7];
	sd22 = vis_faligndata(s21, s20);
	s21 = sp2[8];
	sd23 = vis_faligndata(s20, s21);

	sp1 += 8;
	sp2 += 8;

	if (src1 != dst) {
		mlib_s32 byte_off = (mlib_s32)((((mlib_addr)sa1) + off) & 7);
		mlib_s32 mask;

		vis_alignaddr(sa2, off);
		mask = bmask_min_table[byte_off];
		vis_write_bmask(mask, 0);

#pragma pipeloop(0)
		for (j = 4 + (off >> 1); j <= (dsize - 16); j += 16) {

			MLIB_V_MINIMAGE_U16(sd1, sd2, cmask);
			MLIB_V_MINIMAGE_U16(sd11, sd21, cmask1);
			MLIB_V_MINIMAGE_U16(sd12, sd22, cmask2);
			MLIB_V_MINIMAGE_U16(sd13, sd23, cmask3);

			vis_pst_16(sd_1, dp, cmaskk);
			vis_pst_16(sd_2, dp, ~cmaskk);
			vis_pst_16(sd_11, dp + 1, cmaskk1);
			vis_pst_16(sd_21, dp + 1, ~cmaskk1);
			vis_pst_16(sd_12, dp + 2, cmaskk2);
			vis_pst_16(sd_22, dp + 2, ~cmaskk2);
			vis_pst_16(sd_13, dp + 3, cmaskk3);
			vis_pst_16(sd_23, dp + 3, ~cmaskk3);

			cmaskk = cmask;
			cmaskk1 = cmask1;
			cmaskk2 = cmask2;
			cmaskk3 = cmask3;

			sd_1 = sd1;
			sd_2 = sd2;
			sd_11 = sd11;
			sd_21 = sd21;
			sd_12 = sd12;
			sd_22 = sd22;
			sd_13 = sd13;
			sd_23 = sd23;

			s10 = sp1[1];
			sd1 = vis_bshuffle(s11, s10);
			s11 = sp1[2];
			sd11 = vis_bshuffle(s10, s11);
			s10 = sp1[3];
			sd12 = vis_bshuffle(s11, s10);
			s11 = sp1[4];
			sd13 = vis_bshuffle(s10, s11);

			s20 = sp2[1];
			sd2 = vis_faligndata(s21, s20);
			s21 = sp2[2];
			sd21 = vis_faligndata(s20, s21);
			s20 = sp2[3];
			sd22 = vis_faligndata(s21, s20);
			s21 = sp2[4];
			sd23 = vis_faligndata(s20, s21);

			sp1 += 4;
			sp2 += 4;
			dp += 4;
		}
	} else {
#pragma pipeloop(0)
		for (j = 4 + (off >> 1); j <= (dsize - 16); j += 16) {

			MLIB_V_MINIMAGE_U16(sd1, sd2, cmask);
			MLIB_V_MINIMAGE_U16(sd11, sd21, cmask1);
			MLIB_V_MINIMAGE_U16(sd12, sd22, cmask2);
			MLIB_V_MINIMAGE_U16(sd13, sd23, cmask3);

			vis_pst_16(sd_2, dp, ~cmaskk);
			vis_pst_16(sd_21, dp + 1, ~cmaskk1);
			vis_pst_16(sd_22, dp + 2, ~cmaskk2);
			vis_pst_16(sd_23, dp + 3, ~cmaskk3);

			cmaskk = cmask;
			cmaskk1 = cmask1;
			cmaskk2 = cmask2;
			cmaskk3 = cmask3;

			sd_1 = sd1;
			sd_2 = sd2;
			sd_11 = sd11;
			sd_21 = sd21;
			sd_12 = sd12;
			sd_22 = sd22;
			sd_13 = sd13;
			sd_23 = sd23;

			sd1 = dp[8];
			sd11 = dp[9];
			sd12 = dp[10];
			sd13 = dp[11];

			s20 = sp2[1];
			sd2 = vis_faligndata(s21, s20);
			s21 = sp2[2];
			sd21 = vis_faligndata(s20, s21);
			s20 = sp2[3];
			sd22 = vis_faligndata(s21, s20);
			s21 = sp2[4];
			sd23 = vis_faligndata(s20, s21);

			sp2 += 4;
			dp += 4;
		}
	}

/* last 16-pixels */

	if (dend >= (mlib_u16 *)dp) {
		emask = vis_edge16(dp, dend);
		vis_pst_16(sd_1, dp, cmaskk & emask);
		vis_pst_16(sd_2, dp, ~cmaskk & emask);
		dp++;

		if (dend >= (mlib_u16 *)dp) {
			emask = vis_edge16(dp, dend);
			vis_pst_16(sd_11, dp, cmaskk1 & emask);
			vis_pst_16(sd_21, dp, ~cmaskk1 & emask);
			dp++;

			if (dend >= (mlib_u16 *)dp) {
				emask = vis_edge16(dp, dend);
				vis_pst_16(sd_12, dp, cmaskk2 & emask);
				vis_pst_16(sd_22, dp, ~cmaskk2 & emask);
				dp++;

				if (dend >= (mlib_u16 *)dp) {
					emask = vis_edge16(dp, dend);
					vis_pst_16(sd_13, dp, cmaskk3 & emask);
					vis_pst_16(sd_23, dp, ~cmaskk3 & emask);
				}
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageMin_U16(
    mlib_u16 *src1,
    mlib_s32 slb1,
    mlib_u16 *src2,
    mlib_s32 slb2,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* pointers to source images */
	mlib_u16 *sa1, *sa2;

/* pointer to destination image */
	mlib_u16 *dl;

/* loop variable */
	mlib_s32 j;

	sa1 = src1;
	sa2 = src2;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

		mlib_v_ImageMin_U16_1D(sa1, sa2, dl, xsize);

		sa1 = (mlib_u16 *)((mlib_u8 *)sa1 + slb1);
		sa2 = (mlib_u16 *)((mlib_u8 *)sa2 + slb2);
		dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

#define	MLIB_V_MINIMAGE_S32(sd1, sd2, cmask)                    \
	cmask = vis_fcmplt32(sd1, sd2)

/* *********************************************************** */

void
mlib_v_ImageMin_S32_D1(
    mlib_s32 *src1,
    mlib_s32 *src2,
    mlib_s32 *dst,
    mlib_s32 dsize)
{
/* pointers to source images */
	mlib_s32 *sa1, *sa2;

/* 8-byte aligned pointers to source images */
	mlib_d64 *sp1, *sp2;

/* pointer to destination image */
	mlib_s32 *dl;

/* 8-byte aligned pointer to destination image */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd1, sd2;

/* source data */
	mlib_d64 sd11, sd21;

/* source data */
	mlib_d64 sd12, sd22;

/* source data */
	mlib_d64 sd13, sd23;

/* source data */
	mlib_d64 sd_1, sd_2;

/* source data */
	mlib_d64 sd_11, sd_21;

/* source data */
	mlib_d64 sd_12, sd_22;

/* source data */
	mlib_d64 sd_13, sd_23;

/* pointer to end of a line in destination image */
	mlib_s32 *dend;

/*  masks */
	mlib_s32 cmask, emask, cmaskk;

/*  masks */
	mlib_s32 cmask1, cmaskk1;

/*  masks */
	mlib_s32 cmask2, cmaskk2;

/*  masks */
	mlib_s32 cmask3, cmaskk3;

/* offset of address alignment in destination */
	mlib_s32 off;

/* loop variable */
	mlib_s32 j;

	sa1 = src1;
	sa2 = src2;
	dl = dst;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)dl & (~7));
	off = (mlib_addr)dp - (mlib_addr)dl;
	dend = dl + dsize - 1;

/* generate edge mask for the start point */
	emask = vis_edge32(dl, dend);

/* first 2-pixel column loop */
	sp1 = (mlib_d64 *)vis_alignaddr(sa1, 0);
	sd1 = (*sp1++);
	sp2 = (mlib_d64 *)vis_alignaddr(sa2, 0);
	sd2 = (*sp2++);
	MLIB_V_MINIMAGE_S32(sd1, sd2, cmask);
	vis_pst_32(sd1, dp, emask & cmask);
	vis_pst_32(sd2, dp, emask & ~cmask);
	dp++;

	sd_1 = sp1[0];
	sd_2 = sp2[0];
	sd_11 = sp1[1];
	sd_21 = sp2[1];
	sd_12 = sp1[2];
	sd_22 = sp2[2];
	sd_13 = sp1[3];
	sd_23 = sp2[3];

	MLIB_V_MINIMAGE_S32(sd_1, sd_2, cmaskk);
	MLIB_V_MINIMAGE_S32(sd_11, sd_21, cmaskk1);
	MLIB_V_MINIMAGE_S32(sd_12, sd_22, cmaskk2);
	MLIB_V_MINIMAGE_S32(sd_13, sd_23, cmaskk3);

	sd1 = sp1[4];
	sd2 = sp2[4];
	sd11 = sp1[5];
	sd21 = sp2[5];
	sd12 = sp1[6];
	sd22 = sp2[6];
	sd13 = sp1[7];
	sd23 = sp2[7];

	sp1 += 8;
	sp2 += 8;

	if (src1 != dst) {
#pragma pipeloop(0)
		for (j = 2 + (off >> 2); j <= (dsize - 8); j += 8) {

			MLIB_V_MINIMAGE_S32(sd1, sd2, cmask);
			MLIB_V_MINIMAGE_S32(sd11, sd21, cmask1);
			MLIB_V_MINIMAGE_S32(sd12, sd22, cmask2);
			MLIB_V_MINIMAGE_S32(sd13, sd23, cmask3);

			vis_pst_32(sd_1, dp, cmaskk);
			vis_pst_32(sd_2, dp, ~cmaskk);
			vis_pst_32(sd_11, dp + 1, cmaskk1);
			vis_pst_32(sd_21, dp + 1, ~cmaskk1);
			vis_pst_32(sd_12, dp + 2, cmaskk2);
			vis_pst_32(sd_22, dp + 2, ~cmaskk2);
			vis_pst_32(sd_13, dp + 3, cmaskk3);
			vis_pst_32(sd_23, dp + 3, ~cmaskk3);

			cmaskk = cmask;
			cmaskk1 = cmask1;
			cmaskk2 = cmask2;
			cmaskk3 = cmask3;

			sd_1 = sd1;
			sd_2 = sd2;
			sd_11 = sd11;
			sd_21 = sd21;
			sd_12 = sd12;
			sd_22 = sd22;
			sd_13 = sd13;
			sd_23 = sd23;

			sd1 = sp1[0];
			sd2 = sp2[0];
			sd11 = sp1[1];
			sd21 = sp2[1];
			sd12 = sp1[2];
			sd22 = sp2[2];
			sd13 = sp1[3];
			sd23 = sp2[3];

			sp1 += 4;
			sp2 += 4;
			dp += 4;
		}
	} else {
#pragma pipeloop(0)
		for (j = 2 + (off >> 2); j <= (dsize - 8); j += 8) {

			MLIB_V_MINIMAGE_S32(sd1, sd2, cmask);
			MLIB_V_MINIMAGE_S32(sd11, sd21, cmask1);
			MLIB_V_MINIMAGE_S32(sd12, sd22, cmask2);
			MLIB_V_MINIMAGE_S32(sd13, sd23, cmask3);

			vis_pst_32(sd_2, dp, ~cmaskk);
			vis_pst_32(sd_21, dp + 1, ~cmaskk1);
			vis_pst_32(sd_22, dp + 2, ~cmaskk2);
			vis_pst_32(sd_23, dp + 3, ~cmaskk3);

			cmaskk = cmask;
			cmaskk1 = cmask1;
			cmaskk2 = cmask2;
			cmaskk3 = cmask3;

			sd_1 = sd1;
			sd_2 = sd2;
			sd_11 = sd11;
			sd_21 = sd21;
			sd_12 = sd12;
			sd_22 = sd22;
			sd_13 = sd13;
			sd_23 = sd23;

			sd1 = dp[8];
			sd2 = sp2[0];
			sd11 = dp[9];
			sd21 = sp2[1];
			sd12 = dp[10];
			sd22 = sp2[2];
			sd13 = dp[11];
			sd23 = sp2[3];

			sp2 += 4;
			dp += 4;
		}
	}

/* last 8-pixels */

	if (dend >= (mlib_s32 *)dp) {
		emask = vis_edge32(dp, dend);
		vis_pst_32(sd_1, dp, cmaskk & emask);
		vis_pst_32(sd_2, dp, ~cmaskk & emask);
		dp++;

		if (dend >= (mlib_s32 *)dp) {
			emask = vis_edge32(dp, dend);
			vis_pst_32(sd_11, dp, cmaskk1 & emask);
			vis_pst_32(sd_21, dp, ~cmaskk1 & emask);
			dp++;

			if (dend >= (mlib_s32 *)dp) {
				emask = vis_edge32(dp, dend);
				vis_pst_32(sd_12, dp, cmaskk2 & emask);
				vis_pst_32(sd_22, dp, ~cmaskk2 & emask);
				dp++;

				if (dend >= (mlib_s32 *)dp) {
					emask = vis_edge32(dp, dend);
					vis_pst_32(sd_13, dp, cmaskk3 & emask);
					vis_pst_32(sd_23, dp, ~cmaskk3 & emask);
				}
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageMin_S32_A8D2(
    mlib_s32 *src1,
    mlib_s32 slb1,
    mlib_s32 *src2,
    mlib_s32 slb2,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* 8-byte aligned pointers to source images */
	mlib_d64 *sp1, *sp2;

/* pointers to a line in source */
	mlib_d64 *sl1, *sl2;

/* pointer to a line in destination */
	mlib_d64 *dl;

/* 8-byte aligned pointer to destination image */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd1, sd2;

/* source data */
	mlib_d64 sd11, sd21;

/* source data */
	mlib_d64 sd12, sd22;

/* source data */
	mlib_d64 sd13, sd23;

/* source data */
	mlib_d64 sd_1, sd_2;

/* source data */
	mlib_d64 sd_11, sd_21;

/* source data */
	mlib_d64 sd_12, sd_22;

/* source data */
	mlib_d64 sd_13, sd_23;

/* pointer to end of a line in destination image */
	mlib_s32 *dend;

/*  masks */
	mlib_s32 cmask, emask, cmaskk;

/*  masks */
	mlib_s32 cmask1, cmaskk1;

/*  masks */
	mlib_s32 cmask2, cmaskk2;

/*  masks */
	mlib_s32 cmask3, cmaskk3;

/* loop variable */
	mlib_s32 i, j;

	sl1 = sp1 = (mlib_d64 *)src1;
	sl2 = sp2 = (mlib_d64 *)src2;
	dl = dp = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {
		dend = (mlib_s32 *)dl + xsize - 1;

/* 8-pixel loop */
		sd_1 = sp1[0];
		sd_2 = sp2[0];
		sd_11 = sp1[1];
		sd_21 = sp2[1];
		sd_12 = sp1[2];
		sd_22 = sp2[2];
		sd_13 = sp1[3];
		sd_23 = sp2[3];

		MLIB_V_MINIMAGE_S32(sd_1, sd_2, cmaskk);
		MLIB_V_MINIMAGE_S32(sd_11, sd_21, cmaskk1);
		MLIB_V_MINIMAGE_S32(sd_12, sd_22, cmaskk2);
		MLIB_V_MINIMAGE_S32(sd_13, sd_23, cmaskk3);

		sd1 = sp1[4];
		sd2 = sp2[4];
		sd11 = sp1[5];
		sd21 = sp2[5];
		sd12 = sp1[6];
		sd22 = sp2[6];
		sd13 = sp1[7];
		sd23 = sp2[7];

		sp1 += 8;
		sp2 += 8;

		if (src1 != dst) {
#pragma pipeloop(0)
			for (i = 0; i <= (xsize - 8); i += 8) {

				MLIB_V_MINIMAGE_S32(sd1, sd2, cmask);
				MLIB_V_MINIMAGE_S32(sd11, sd21, cmask1);
				MLIB_V_MINIMAGE_S32(sd12, sd22, cmask2);
				MLIB_V_MINIMAGE_S32(sd13, sd23, cmask3);

				vis_pst_32(sd_1, dp, cmaskk);
				vis_pst_32(sd_2, dp, ~cmaskk);
				vis_pst_32(sd_11, dp + 1, cmaskk1);
				vis_pst_32(sd_21, dp + 1, ~cmaskk1);
				vis_pst_32(sd_12, dp + 2, cmaskk2);
				vis_pst_32(sd_22, dp + 2, ~cmaskk2);
				vis_pst_32(sd_13, dp + 3, cmaskk3);
				vis_pst_32(sd_23, dp + 3, ~cmaskk3);

				cmaskk = cmask;
				cmaskk1 = cmask1;
				cmaskk2 = cmask2;
				cmaskk3 = cmask3;

				sd_1 = sd1;
				sd_2 = sd2;
				sd_11 = sd11;
				sd_21 = sd21;
				sd_12 = sd12;
				sd_22 = sd22;
				sd_13 = sd13;
				sd_23 = sd23;

				sd1 = sp1[0];
				sd2 = sp2[0];
				sd11 = sp1[1];
				sd21 = sp2[1];
				sd12 = sp1[2];
				sd22 = sp2[2];
				sd13 = sp1[3];
				sd23 = sp2[3];

				sp1 += 4;
				sp2 += 4;
				dp += 4;
			}
		} else {
#pragma pipeloop(0)
			for (i = 0; i <= (xsize - 8); i += 8) {

				MLIB_V_MINIMAGE_S32(sd1, sd2, cmask);
				MLIB_V_MINIMAGE_S32(sd11, sd21, cmask1);
				MLIB_V_MINIMAGE_S32(sd12, sd22, cmask2);
				MLIB_V_MINIMAGE_S32(sd13, sd23, cmask3);

				vis_pst_32(sd_2, dp, ~cmaskk);
				vis_pst_32(sd_21, dp + 1, ~cmaskk1);
				vis_pst_32(sd_22, dp + 2, ~cmaskk2);
				vis_pst_32(sd_23, dp + 3, ~cmaskk3);

				cmaskk = cmask;
				cmaskk1 = cmask1;
				cmaskk2 = cmask2;
				cmaskk3 = cmask3;

				sd_1 = sd1;
				sd_2 = sd2;
				sd_11 = sd11;
				sd_21 = sd21;
				sd_12 = sd12;
				sd_22 = sd22;
				sd_13 = sd13;
				sd_23 = sd23;

				sd1 = dp[8];
				sd2 = sp2[0];
				sd11 = dp[9];
				sd21 = sp2[1];
				sd12 = dp[10];
				sd22 = sp2[2];
				sd13 = dp[11];
				sd23 = sp2[3];

				sp2 += 4;
				dp += 4;
			}
		}

/* last 8-pixels */

		if (dend >= (mlib_s32 *)dp) {
			emask = vis_edge32(dp, dend);
			vis_pst_32(sd_1, dp, cmaskk & emask);
			vis_pst_32(sd_2, dp, ~cmaskk & emask);
			dp++;

			if (dend >= (mlib_s32 *)dp) {
				emask = vis_edge32(dp, dend);
				vis_pst_32(sd_11, dp, cmaskk1 & emask);
				vis_pst_32(sd_21, dp, ~cmaskk1 & emask);
				dp++;

				if (dend >= (mlib_s32 *)dp) {
					emask = vis_edge32(dp, dend);
					vis_pst_32(sd_12, dp, cmaskk2 & emask);
					vis_pst_32(sd_22, dp, ~cmaskk2 & emask);
					dp++;

					if (dend >= (mlib_s32 *)dp) {
						emask = vis_edge32(dp, dend);
						vis_pst_32(sd_13, dp,
						    cmaskk3 & emask);
						vis_pst_32(sd_23, dp,
						    ~cmaskk3 & emask);
					}
				}
			}
		}

		sl1 = sp1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
		sl2 = sp2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
		dl = dp = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageMin_S32_1D(
    mlib_s32 *src1,
    mlib_s32 *src2,
    mlib_s32 *dst,
    mlib_s32 dsize)
{
/* pointers to source images */
	mlib_s32 *sa1, *sa2;

/* 8-byte aligned pointers to source images */
	mlib_d64 *sp1, *sp2;

/* pointer to destination image */
	mlib_s32 *dl;

/* 8-byte aligned pointer to destination image */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd1, sd2;

/* source data */
	mlib_d64 sd11, sd21;

/* source data */
	mlib_d64 sd12, sd22;

/* source data */
	mlib_d64 sd13, sd23;

/* source data */
	mlib_d64 sd_1, sd_2;

/* source data */
	mlib_d64 sd_11, sd_21;

/* source data */
	mlib_d64 sd_12, sd_22;

/* source data */
	mlib_d64 sd_13, sd_23;

/* pointer to end of a line in destination image */
	mlib_s32 *dend;

/*  masks */
	mlib_s32 cmask, emask, cmaskk;

/*  masks */
	mlib_s32 cmask1, cmaskk1;

/*  masks */
	mlib_s32 cmask2, cmaskk2;

/*  masks */
	mlib_s32 cmask3, cmaskk3;

/* source data */
	mlib_d64 s11, s10;

/* source data */
	mlib_d64 s21, s20;

/* offset of address alignment in destination */
	mlib_s32 off;

/* loop variable */
	mlib_s32 j;

	sa1 = src1;
	sa2 = src2;
	dl = dst;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)dl & (~7));
	off = (mlib_addr)dp - (mlib_addr)dl;
	dend = dl + dsize - 1;

/* generate edge mask for the start point */
	emask = vis_edge32(dl, dend);

/* first 2-pixel column loop */
	sp1 = (mlib_d64 *)vis_alignaddr(sa1, off);
	s10 = sp1[0];
	s11 = sp1[1];
	sd1 = vis_faligndata(s10, s11);
	sp1++;

	sp2 = (mlib_d64 *)vis_alignaddr(sa2, off);
	s20 = sp2[0];
	s21 = sp2[1];
	sd2 = vis_faligndata(s20, s21);
	sp2++;
	MLIB_V_MINIMAGE_S32(sd1, sd2, cmask);
	vis_pst_32(sd1, dp, emask & cmask);
	vis_pst_32(sd2, dp, emask & ~cmask);
	dp++;

	s20 = sp2[1];
	sd_2 = vis_faligndata(s21, s20);
	s21 = sp2[2];
	sd_21 = vis_faligndata(s20, s21);
	s20 = sp2[3];
	sd_22 = vis_faligndata(s21, s20);
	s21 = sp2[4];
	sd_23 = vis_faligndata(s20, s21);

	vis_alignaddr(sa1, off);
	s10 = sp1[1];
	sd_1 = vis_faligndata(s11, s10);
	s11 = sp1[2];
	sd_11 = vis_faligndata(s10, s11);
	s10 = sp1[3];
	sd_12 = vis_faligndata(s11, s10);
	s11 = sp1[4];
	sd_13 = vis_faligndata(s10, s11);

	MLIB_V_MINIMAGE_S32(sd_1, sd_2, cmaskk);
	MLIB_V_MINIMAGE_S32(sd_11, sd_21, cmaskk1);
	MLIB_V_MINIMAGE_S32(sd_12, sd_22, cmaskk2);
	MLIB_V_MINIMAGE_S32(sd_13, sd_23, cmaskk3);

	s10 = sp1[5];
	sd1 = vis_faligndata(s11, s10);
	s11 = sp1[6];
	sd11 = vis_faligndata(s10, s11);
	s10 = sp1[7];
	sd12 = vis_faligndata(s11, s10);
	s11 = sp1[8];
	sd13 = vis_faligndata(s10, s11);

	vis_alignaddr(sa2, off);
	s20 = sp2[5];
	sd2 = vis_faligndata(s21, s20);
	s21 = sp2[6];
	sd21 = vis_faligndata(s20, s21);
	s20 = sp2[7];
	sd22 = vis_faligndata(s21, s20);
	s21 = sp2[8];
	sd23 = vis_faligndata(s20, s21);

	sp1 += 8;
	sp2 += 8;

	if (src1 != dst) {
		mlib_s32 byte_off = (mlib_s32)((((mlib_addr)sa1) + off) & 7);
		mlib_s32 mask;

		vis_alignaddr(sa2, off);
		mask = bmask_min_table[byte_off];
		vis_write_bmask(mask, 0);

#pragma pipeloop(0)
		for (j = 2 + (off >> 2); j <= (dsize - 8); j += 8) {

			MLIB_V_MINIMAGE_S32(sd1, sd2, cmask);
			MLIB_V_MINIMAGE_S32(sd11, sd21, cmask1);
			MLIB_V_MINIMAGE_S32(sd12, sd22, cmask2);
			MLIB_V_MINIMAGE_S32(sd13, sd23, cmask3);

			vis_pst_32(sd_1, dp, cmaskk);
			vis_pst_32(sd_2, dp, ~cmaskk);
			vis_pst_32(sd_11, dp + 1, cmaskk1);
			vis_pst_32(sd_21, dp + 1, ~cmaskk1);
			vis_pst_32(sd_12, dp + 2, cmaskk2);
			vis_pst_32(sd_22, dp + 2, ~cmaskk2);
			vis_pst_32(sd_13, dp + 3, cmaskk3);
			vis_pst_32(sd_23, dp + 3, ~cmaskk3);

			cmaskk = cmask;
			cmaskk1 = cmask1;
			cmaskk2 = cmask2;
			cmaskk3 = cmask3;

			sd_1 = sd1;
			sd_2 = sd2;
			sd_11 = sd11;
			sd_21 = sd21;
			sd_12 = sd12;
			sd_22 = sd22;
			sd_13 = sd13;
			sd_23 = sd23;

			s10 = sp1[1];
			sd1 = vis_bshuffle(s11, s10);
			s11 = sp1[2];
			sd11 = vis_bshuffle(s10, s11);
			s10 = sp1[3];
			sd12 = vis_bshuffle(s11, s10);
			s11 = sp1[4];
			sd13 = vis_bshuffle(s10, s11);

			s20 = sp2[1];
			sd2 = vis_faligndata(s21, s20);
			s21 = sp2[2];
			sd21 = vis_faligndata(s20, s21);
			s20 = sp2[3];
			sd22 = vis_faligndata(s21, s20);
			s21 = sp2[4];
			sd23 = vis_faligndata(s20, s21);

			sp1 += 4;
			sp2 += 4;
			dp += 4;
		}
	} else {
#pragma pipeloop(0)
		for (j = 2 + (off >> 2); j <= (dsize - 8); j += 8) {

			MLIB_V_MINIMAGE_S32(sd1, sd2, cmask);
			MLIB_V_MINIMAGE_S32(sd11, sd21, cmask1);
			MLIB_V_MINIMAGE_S32(sd12, sd22, cmask2);
			MLIB_V_MINIMAGE_S32(sd13, sd23, cmask3);

			vis_pst_32(sd_2, dp, ~cmaskk);
			vis_pst_32(sd_21, dp + 1, ~cmaskk1);
			vis_pst_32(sd_22, dp + 2, ~cmaskk2);
			vis_pst_32(sd_23, dp + 3, ~cmaskk3);

			cmaskk = cmask;
			cmaskk1 = cmask1;
			cmaskk2 = cmask2;
			cmaskk3 = cmask3;

			sd_1 = sd1;
			sd_2 = sd2;
			sd_11 = sd11;
			sd_21 = sd21;
			sd_12 = sd12;
			sd_22 = sd22;
			sd_13 = sd13;
			sd_23 = sd23;

			sd1 = dp[8];
			sd11 = dp[9];
			sd12 = dp[10];
			sd13 = dp[11];

			s20 = sp2[1];
			sd2 = vis_faligndata(s21, s20);
			s21 = sp2[2];
			sd21 = vis_faligndata(s20, s21);
			s20 = sp2[3];
			sd22 = vis_faligndata(s21, s20);
			s21 = sp2[4];
			sd23 = vis_faligndata(s20, s21);

			sp2 += 4;
			dp += 4;
		}
	}

/* last 8-pixels */

	if (dend >= (mlib_s32 *)dp) {
		emask = vis_edge32(dp, dend);
		vis_pst_32(sd_1, dp, cmaskk & emask);
		vis_pst_32(sd_2, dp, ~cmaskk & emask);
		dp++;

		if (dend >= (mlib_s32 *)dp) {
			emask = vis_edge32(dp, dend);
			vis_pst_32(sd_11, dp, cmaskk1 & emask);
			vis_pst_32(sd_21, dp, ~cmaskk1 & emask);
			dp++;

			if (dend >= (mlib_s32 *)dp) {
				emask = vis_edge32(dp, dend);
				vis_pst_32(sd_12, dp, cmaskk2 & emask);
				vis_pst_32(sd_22, dp, ~cmaskk2 & emask);
				dp++;

				if (dend >= (mlib_s32 *)dp) {
					emask = vis_edge32(dp, dend);
					vis_pst_32(sd_13, dp, cmaskk3 & emask);
					vis_pst_32(sd_23, dp, ~cmaskk3 & emask);
				}
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageMin_S32(
    mlib_s32 *src1,
    mlib_s32 slb1,
    mlib_s32 *src2,
    mlib_s32 slb2,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* pointers to source images */
	mlib_s32 *sa1, *sa2;

/* pointer to destination image */
	mlib_s32 *dl;

/* loop variable */
	mlib_s32 j;

	sa1 = src1;
	sa2 = src2;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

		mlib_v_ImageMin_S32_1D(sa1, sa2, dl, xsize);

		sa1 = (mlib_s32 *)((mlib_u8 *)sa1 + slb1);
		sa2 = (mlib_s32 *)((mlib_u8 *)sa2 + slb2);
		dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */
