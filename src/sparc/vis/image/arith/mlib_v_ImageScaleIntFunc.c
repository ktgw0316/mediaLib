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

#pragma ident	"@(#)mlib_v_ImageScaleIntFunc.c	9.3	07/11/05 SMI"

/*
 * Internal functions.
 *
 *     void mlib_v_ImageScale_S32_S16_124_A8D1(mlib_u8  *src,
 *                                             mlib_u8  *dst,
 *                                             mlib_s32 dsize,
 *                                             mlib_d64 ad,
 *                                             mlib_d64 bdh,
 *                                             mlib_d64 bdl);
 *
 *     void mlib_v_ImageScale_S32_S16_3_A8D1(mlib_u8  *src,
 *                                           mlib_u8  *dst,
 *                                           mlib_s32 dsize,
 *                                           mlib_f32 *afh,
 *                                           mlib_f32 *afl,
 *                                           mlib_d64 *bdh,
 *                                           mlib_d64 *bdl);
 *
 *     void mlib_v_ImageScale_S32_S16_124(mlib_u8  *src,
 *                                        mlib_s32 slb,
 *                                        mlib_u8  *dst,
 *                                        mlib_s32 dlb,
 *                                        mlib_s32 xsize,
 *                                        mlib_s32 ysize,
 *                                        mlib_f32 *afh,
 *                                        mlib_f32 *afl,
 *                                        mlib_d64 *bdh,
 *                                        mlib_d64 *bdl);
 *
 *     void mlib_v_ImageScale_S32_S16_3(mlib_u8  *src,
 *                                      mlib_s32 slb,
 *                                      mlib_u8  *dst,
 *                                      mlib_s32 dlb,
 *                                      mlib_s32 xsize,
 *                                      mlib_s32 ysize,
 *                                      mlib_f32 *afh,
 *                                      mlib_f32 *afl,
 *                                      mlib_d64 *bdh,
 *                                      mlib_d64 *bdl);
 *
 *     void mlib_v_ImageScale_S32_U16_124(mlib_u8  *src,
 *                                        mlib_s32 slb,
 *                                        mlib_u8  *dst,
 *                                        mlib_s32 dlb,
 *                                        mlib_s32 xsize,
 *                                        mlib_s32 ysize,
 *                                        mlib_f32 *afh,
 *                                        mlib_f32 *afl,
 *                                        mlib_d64 *bdh,
 *                                        mlib_d64 *bdl);
 *
 *     void mlib_v_ImageScale_S32_U16_3(mlib_u8  *src,
 *                                      mlib_s32 slb,
 *                                      mlib_u8  *dst,
 *                                      mlib_s32 dlb,
 *                                      mlib_s32 xsize,
 *                                      mlib_s32 ysize,
 *                                      mlib_f32 *afh,
 *                                      mlib_f32 *afl,
 *                                      mlib_d64 *bdh,
 *                                      mlib_d64 *bdl);
 *
 *     void mlib_v_ImageScale_S32_U8_124_A8D1(mlib_u8  *src,
 *                                            mlib_u8  *dst,
 *                                            mlib_s32 dsize,
 *                                            mlib_d64 ad,
 *                                            mlib_d64 bdh,
 *                                            mlib_d64 bdl);
 *
 *     void mlib_v_ImageScale_S32_U8_3_A8D1(mlib_u8  *src,
 *                                          mlib_u8  *dst,
 *                                          mlib_s32 dsize,
 *                                          mlib_f32 *afh,
 *                                          mlib_f32 *afl,
 *                                          mlib_d64 *bdh,
 *                                          mlib_d64 *bdl);
 *
 *     void mlib_v_ImageScale_S32_U8_124(mlib_u8  *src,
 *                                       mlib_s32 slb,
 *                                       mlib_u8  *dst,
 *                                       mlib_s32 dlb,
 *                                       mlib_s32 xsize,
 *                                       mlib_s32 ysize,
 *                                       mlib_f32 *afh,
 *                                       mlib_f32 *afl,
 *                                       mlib_d64 *bdh,
 *                                       mlib_d64 *bdl);
 *
 *     void mlib_v_ImageScale_S32_U8_3(mlib_u8  *src,
 *                                     mlib_s32 slb,
 *                                     mlib_u8  *dst,
 *                                     mlib_s32 dlb,
 *                                     mlib_s32 xsize,
 *                                     mlib_s32 ysize,
 *                                     mlib_f32 *afh,
 *                                     mlib_f32 *afl,
 *                                     mlib_d64 *bdh,
 *                                     mlib_d64 *bdl);
 *
 *     void mlib_v_ImageScale_S16_U8_124_A8D1X4(mlib_s16 *src,
 *                                              mlib_u8  *dst,
 *                                              mlib_s32 dsize,
 *                                              mlib_d64 ad,
 *                                              mlib_d64 bdh,
 *                                              mlib_d64 bdl);
 *
 *     void mlib_v_ImageScale_S16_U8_124_A8D2(mlib_s16 *src,
 *                                            mlib_s32 slb,
 *                                            mlib_u8  *dst,
 *                                            mlib_s32 dlb,
 *                                            mlib_s32 xsize,
 *                                            mlib_s32 ysize,
 *                                            mlib_d64 ad,
 *                                            mlib_d64 bdh,
 *                                            mlib_d64 bdl);
 *
 *     void mlib_v_ImageScale_S16_U8_124(mlib_s16 *src,
 *                                       mlib_s32 slb,
 *                                       mlib_u8  *dst,
 *                                       mlib_s32 dlb,
 *                                       mlib_s32 xsize,
 *                                       mlib_s32 ysize,
 *                                       mlib_d64 ad,
 *                                       mlib_d64 bdh,
 *                                       mlib_d64 bdl);
 *
 *     void mlib_v_ImageScale_S16_U8_3_A8D1(mlib_s16 *src,
 *                                          mlib_u8  *dst,
 *                                          mlib_s32 dsize,
 *                                          mlib_d64 ad,
 *                                          mlib_d64 bdh,
 *                                          mlib_d64 bdl);
 *
 *     void mlib_v_ImageScale_S16_U8_3_A8D2(mlib_s16 *src,
 *                                          mlib_s32 slb,
 *                                          mlib_u8  *dst,
 *                                          mlib_s32 dlb,
 *                                          mlib_s32 xsize,
 *                                          mlib_s32 ysize,
 *                                          mlib_d64 ad,
 *                                          mlib_d64 bdh,
 *                                          mlib_d64 bdl);
 *
 *     void mlib_v_ImageScale_S16_U8_3(mlib_s16 *src,
 *                                     mlib_s32 slb,
 *                                     mlib_u8  *dst,
 *                                     mlib_s32 dlb,
 *                                     mlib_s32 xsize,
 *                                     mlib_s32 ysize,
 *                                     mlib_d64 ad,
 *                                     mlib_d64 bdh,
 *                                     mlib_d64 bdl);
 *
 *     void mlib_v_ImageScale_U16_U8_124(mlib_u16 *src,
 *                                       mlib_s32 slb,
 *                                       mlib_u8  *dst,
 *                                       mlib_s32 dlb,
 *                                       mlib_s32 xsize,
 *                                       mlib_s32 ysize,
 *                                       mlib_d64 ad,
 *                                       mlib_d64 bdh,
 *                                       mlib_d64 bdl);
 *
 *     void mlib_v_ImageScale_U16_U8_3(mlib_u16 *src,
 *                                     mlib_s32 slb,
 *                                     mlib_u8  *dst,
 *                                     mlib_s32 dlb,
 *                                     mlib_s32 xsize,
 *                                     mlib_s32 ysize,
 *                                     mlib_d64 ad,
 *                                     mlib_d64 bdh,
 *                                     mlib_d64 bdl);
 *
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageScale.h>

/* *********************************************************** */

#define	MLIB_V_IMAGESCALE_S16_S16(sd, afh, afl, bdh, bdl, dd)   \
	rdhh = vis_fmuld8sux16(vis_read_hi(sd), afh);           \
	rdhl = vis_fmuld8ulx16(vis_read_hi(sd), afh);           \
	rdh = vis_fpadd32(rdhh, rdhl);                          \
	rdh = vis_fpadd32(rdh, bdh);                            \
	rdlh = vis_fmuld8sux16(vis_read_lo(sd), afl);           \
	rdll = vis_fmuld8ulx16(vis_read_lo(sd), afl);           \
	rdl = vis_fpadd32(rdlh, rdll);                          \
	rdl = vis_fpadd32(rdl, bdl);                            \
	dd = vis_fpackfix_pair(rdh, rdl)

/* *********************************************************** */

#define	MLIB_V_SHIFT16_S32_S16(sd, df)                          \
	sd = vis_fand(sd, d32hi16);                             \
	sh = vis_faligndata(dzero, sd);                         \
	df = vis_fors(vis_read_hi(sd), vis_read_lo(sh))

/* *********************************************************** */

#define	MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, afh, afl, bdh,      \
	bdl, dd)                                                \
	MLIB_V_SHIFT16_S32_S16(sd0, sf0);                       \
	MLIB_V_SHIFT16_S32_S16(sd1, sf1);                       \
	sd = vis_freg_pair(sf0, sf1);                           \
	MLIB_V_IMAGESCALE_S16_S16(sd, afh, afl, bdh, bdl, dd)

/* *********************************************************** */

#define	MLIB_V_IMAGESCALE_S16_U16(sd, afh, afl, bdh, bdl, dd)    \
	MLIB_V_IMAGESCALE_S16_S16(sd, afh, afl, bdh, bdl, dd);   \
	dd = vis_fxor(dd, mask_8000)

/* *********************************************************** */

#define	MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, afh, afl, bdh,      \
	bdl, dd)                                                \
	MLIB_V_SHIFT16_S32_S16(sd0, sf0);                       \
	MLIB_V_SHIFT16_S32_S16(sd1, sf1);                       \
	sd = vis_freg_pair(sf0, sf1);                           \
	MLIB_V_IMAGESCALE_S16_U16(sd, afh, afl, bdh, bdl, dd)

/* *********************************************************** */

void
mlib_v_ImageScale_S32_S16_124_A8D1(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd, sd0, sd1;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* loop variables */
	mlib_s32 j;
	mlib_d64 sh;
	mlib_f32 sf0, sf1;

	mlib_d64 dzero = vis_fzero();
	mlib_d64 d32hi16 = vis_to_double_dup(0xffff0000);

	vis_alignaddr((void *)0, 6);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

/* 4-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 4); j += 4) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, vis_read_hi(ad),
		    vis_read_lo(ad), bdh, bdl, dd);
		(*dp++) = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_S32_S16_3_A8D1(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_f32 *afh,
    mlib_f32 *afl,
    mlib_d64 *bdh,
    mlib_d64 *bdl)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd, sd0, sd1;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* loop variables */
	mlib_s32 j;
	mlib_d64 sh;
	mlib_f32 sf0, sf1;

	mlib_d64 dzero = vis_fzero();
	mlib_d64 d32hi16 = vis_to_double_dup(0xffff0000);
	mlib_f32 ah, al;
	mlib_d64 bh, bl;
	mlib_s32 emask, n;
	mlib_s16 *dend;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;
	dend = (mlib_s16 *)dp + dsize - 1;

/* prepare align for macro */
	vis_alignaddr((void *)0, 6);

	n = dsize / 12;

/* 4-pixel loop */
	ah = afh[0];
	al = afl[0];
	bh = bdh[0];
	bl = bdl[0];
#pragma pipeloop(0)
	for (j = 0; j < n; j++) {
		sd0 = sp[6 * j];
		sd1 = sp[6 * j + 1];
		MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, ah, al, bh, bl, dd);
		dp[3 * j] = dd;
	}

	ah = afh[1];
	al = afl[1];
	bh = bdh[1];
	bl = bdl[1];
#pragma pipeloop(0)
	for (j = 0; j < n; j++) {
		sd0 = sp[6 * j + 2];
		sd1 = sp[6 * j + 3];
		MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, ah, al, bh, bl, dd);
		dp[3 * j + 1] = dd;
	}

	ah = afh[2];
	al = afl[2];
	bh = bdh[2];
	bl = bdl[2];
#pragma pipeloop(0)
	for (j = 0; j < n; j++) {
		sd0 = sp[6 * j + 4];
		sd1 = sp[6 * j + 5];
		MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, ah, al, bh, bl, dd);
		dp[3 * j + 2] = dd;
	}

	sp += 6 * n;
	dp += 3 * n;

/* last 4-pixel */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, afh[0], afl[0], bdh[0],
		    bdl[0], dd);
		emask = vis_edge16(dp, dend);
		vis_pst_16(dd, dp, emask);
		dp++;

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd0 = vis_ld_d64_nf(sp); sp++;
			sd1 = vis_ld_d64_nf(sp); sp++;
			MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, afh[1], afl[1],
			    bdh[1], bdl[1], dd);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd0 = vis_ld_d64_nf(sp); sp++;
				sd1 = vis_ld_d64_nf(sp); sp++;
				MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, afh[2],
				    afl[2], bdh[2], bdl[2], dd);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
				dp++;
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_S32_S16_124(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_f32 *afh,
    mlib_f32 *afl,
    mlib_d64 *bdh,
    mlib_d64 *bdl)
{
/* aligned dst address */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd, sd0, sd1;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* loop variables */
	mlib_s32 j;
	mlib_d64 sh;
	mlib_f32 sf0, sf1;

	mlib_f32 *sl, *sp;
	mlib_s16 *dl, *dend;
	mlib_d64 bh, bl;
	mlib_f32 ah, al;
	mlib_f32 sf[4];
	mlib_s32 off, i, n, emask;

	mlib_d64 dzero = vis_fzero();
	mlib_d64 d32hi16 = vis_to_double_dup(0xffff0000);

	sl = (mlib_f32 *)src;
	dl = (mlib_s16 *)dst;

/* prepare align for macro */
	vis_alignaddr((void *)0, 6);

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the source and destination address */
		sp = sl;
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_u8 *)dl - (mlib_u8 *)dp;
		dend = dl + xsize - 1;

/* prepare the alpha and beta values */
		off >>= 1;
		ah = afh[off];
		al = afl[off];
		bh = bdh[off];
		bl = bdl[off];

/* first 4-pixel column */
		emask = vis_edge16(dl, dend);

		if (emask != 0xf) {
			for (i = off; i < 4; i++) {
				sf[i] = vis_ld_f32_nf(sp); sp++;
			}
			MLIB_V_IMAGESCALE_S32_S16(*(mlib_d64 *)sf,
			    *((mlib_d64 *)sf + 1), ah, al, bh, bl, dd);
			vis_pst_16(dd, dp, emask);
			dp++;
		}

/* 4-pixel column loop */
		n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) >> 3;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			sd0 = vis_freg_pair(sp[0], sp[1]);
			sd1 = vis_freg_pair(sp[2], sp[3]);
			MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, ah, al, bh, bl, dd);
			*dp = dd;
			dp++;
			sp += 4;
		}

/* last 4-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd0 = vis_freg_pair(vis_ld_f32_nf(sp),
				vis_ld_f32_nf(sp + 1));
			sd1 = vis_freg_pair(vis_ld_f32_nf(sp + 2),
				vis_ld_f32_nf(sp + 3));
			MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, ah, al, bh, bl, dd);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
		}

		sl = (mlib_f32 *)((mlib_u8 *)sl + slb);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_S32_S16_3(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_f32 *afh,
    mlib_f32 *afl,
    mlib_d64 *bdh,
    mlib_d64 *bdl)
{
/* aligned dst address */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd, sd0, sd1;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* loop variables */
	mlib_s32 j;
	mlib_d64 sh;
	mlib_f32 sf0, sf1;

	mlib_f32 *sl, *sp;
	mlib_s16 *dl, *dend;
	mlib_d64 bh, bl;
	mlib_f32 ah, al;
	mlib_f32 sf[4];
	mlib_s32 off, off0, off1, off2, i, n, emask;

	mlib_d64 dzero = vis_fzero();
	mlib_d64 d32hi16 = vis_to_double_dup(0xffff0000);

/* prepare align for macro */
	vis_alignaddr((void *)0, 6);

/* line addresses */
	sl = (mlib_f32 *)src;
	dl = (mlib_s16 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the source and destination address */
		sp = sl;
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_u8 *)dl - (mlib_u8 *)dp;
		dend = dl + xsize - 1;

		off >>= 1;

		if (off)
			off0 = 3 - off;
		else
			off0 = 0;

/* first pixel column */
		emask = vis_edge16(dl, dend);

		if (emask != 0xf) {
			for (i = off; i < 4; i++) {
				sf[i] = vis_ld_f32_nf(sp); sp++;
			}
			MLIB_V_IMAGESCALE_S32_S16(*(mlib_d64 *)sf,
			    *((mlib_d64 *)sf + 1), afh[off0], afl[off0],
			    bdh[off0], bdl[off0], dd);
			vis_pst_16(dd, dp, emask);
			dp++;
			off0++;

			if (off0 == 3)
				off0 = 0;
		}

/* 4-pixel loop */
		n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) / (3 * 8);

		ah = afh[off0];
		al = afl[off0];
		bh = bdh[off0];
		bl = bdl[off0];
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			sd0 = vis_freg_pair(sp[12 * i], sp[12 * i + 1]);
			sd1 = vis_freg_pair(sp[12 * i + 2], sp[12 * i + 3]);
			MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, ah, al, bh, bl, dd);
			dp[3 * i] = dd;
		}

		off1 = off0 + 1;

		if (off1 == 3)
			off1 = 0;
		ah = afh[off1];
		al = afl[off1];
		bh = bdh[off1];
		bl = bdl[off1];
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			sd0 = vis_freg_pair(sp[12 * i + 4], sp[12 * i + 5]);
			sd1 = vis_freg_pair(sp[12 * i + 6], sp[12 * i + 7]);
			MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, ah, al, bh, bl, dd);
			dp[3 * i + 1] = dd;
		}

		off2 = off1 + 1;

		if (off2 == 3)
			off2 = 0;
		ah = afh[off2];
		al = afl[off2];
		bh = bdh[off2];
		bl = bdl[off2];
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			sd0 = vis_freg_pair(sp[12 * i + 8], sp[12 * i + 9]);
			sd1 = vis_freg_pair(sp[12 * i + 10], sp[12 * i + 11]);
			MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, ah, al, bh, bl, dd);
			dp[3 * i + 2] = dd;
		}

		sp += 12 * n;
		dp += 3 * n;

/* last 4-pixel */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd0 = vis_freg_pair(vis_ld_f32_nf(sp),
				vis_ld_f32_nf(sp + 1));
			sd1 = vis_freg_pair(vis_ld_f32_nf(sp + 2),
				vis_ld_f32_nf(sp + 3));
			MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, afh[off0],
			    afl[off0], bdh[off0], bdl[off0], dd);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd0 = vis_freg_pair(vis_ld_f32_nf(sp + 4),
					vis_ld_f32_nf(sp + 5));
				sd1 = vis_freg_pair(vis_ld_f32_nf(sp + 6),
					vis_ld_f32_nf(sp + 7));
				MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, afh[off1],
				    afl[off1], bdh[off1], bdl[off1], dd);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					sd0 = vis_freg_pair(
						vis_ld_f32_nf(sp + 8),
						vis_ld_f32_nf(sp + 9));
					sd1 = vis_freg_pair(
						vis_ld_f32_nf(sp + 10),
						vis_ld_f32_nf(sp + 11));
					MLIB_V_IMAGESCALE_S32_S16(sd0, sd1,
					    afh[off2], afl[off2], bdh[off2],
					    bdl[off2], dd);
					emask = vis_edge16(dp, dend);
					vis_pst_16(dd, dp, emask);
					dp++;
				}
			}
		}

		sl = (mlib_f32 *)((mlib_u8 *)sl + slb);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_S32_U16_124(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_f32 *afh,
    mlib_f32 *afl,
    mlib_d64 *bdh,
    mlib_d64 *bdl)
{
/* aligned dst address */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd, sd0, sd1;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* loop variables */
	mlib_s32 j;
	mlib_d64 sh;
	mlib_f32 sf0, sf1;
	mlib_f32 *sl, *sp;
	mlib_s16 *dl, *dend;
	mlib_d64 bh, bl;
	mlib_f32 ah, al;
	mlib_s32 off, i, n, emask;
	mlib_d64 dzero = vis_fzero();
	mlib_d64 d32hi16 = vis_to_double_dup(0xffff0000);
	mlib_d64 mask_8000 = vis_to_double_dup(0x80008000);

	sl = (mlib_f32 *)src;
	dl = (mlib_s16 *)dst;

/* prepare align for macro */
	vis_alignaddr((void *)0, 6);

	if (((slb >> 1) == dlb) && (dlb == 2 * xsize)) {
		xsize *= ysize;
		ysize = 1;
	}

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the source and destination address */
		sp = sl;
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_u8 *)dl - (mlib_u8 *)dp;
		dend = dl + xsize - 1;

/* prepare the alpha and beta values */
		off >>= 1;
		ah = afh[off];
		al = afl[off];
		bh = bdh[off];
		bl = bdl[off];

/* first 4-pixel column */
		emask = vis_edge16(dl, dend);

		if (emask != 0xf) {
			sd0 = vis_freg_pair(vis_ld_f32_nf(sp - off),
				vis_ld_f32_nf(sp - off + 1));
			sd1 = vis_freg_pair(vis_ld_f32_nf(sp - off + 2),
				vis_ld_f32_nf(sp - off + 3));
			sp += (4 - off);
			MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, ah, al, bh, bl, dd);
			vis_pst_16(dd, dp, emask);
			dp++;
		}

		if (((mlib_addr)sp & 7) == 0) {

/* 4-pixel column loop */
			n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) >> 3;
#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				sd0 = *(mlib_d64 *)sp;
				sd1 = *(mlib_d64 *)(sp + 2);
				MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, ah, al, bh,
				    bl, dd);
				*dp = dd;
				dp++;
				sp += 4;
			}

/* last 4-pixel column */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd0 = vis_ld_d64_nf((mlib_d64 *)sp);
				sd1 = vis_ld_d64_nf((mlib_d64 *)(sp + 2));
				MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, ah, al, bh,
				    bl, dd);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
			}

		} else {

/* 4-pixel column loop */
			n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) >> 3;
#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				sd0 = vis_freg_pair(sp[0], sp[1]);
				sd1 = vis_freg_pair(sp[2], sp[3]);
				MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, ah, al, bh,
				    bl, dd);
				*dp = dd;
				dp++;
				sp += 4;
			}

/* last 4-pixel column */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd0 = vis_freg_pair(vis_ld_f32_nf(sp),
					vis_ld_f32_nf(sp + 1));
				sd1 = vis_freg_pair(vis_ld_f32_nf(sp + 2),
					vis_ld_f32_nf(sp + 3));
				MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, ah, al, bh,
				    bl, dd);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
			}
		}

		sl = (mlib_f32 *)((mlib_u8 *)sl + slb);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_S32_U16_3(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_f32 *afh,
    mlib_f32 *afl,
    mlib_d64 *bdh,
    mlib_d64 *bdl)
{
/* aligned dst address */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd, sd0, sd1;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* loop variables */
	mlib_s32 j;
	mlib_d64 sh;
	mlib_f32 sf0, sf1;
	mlib_f32 *sl, *sp;
	mlib_s16 *dl, *dend;
	mlib_d64 bh, bl;
	mlib_f32 ah, al;
	mlib_s32 off, off0, off1, off2, i, n, emask;
	mlib_d64 dzero = vis_fzero();
	mlib_d64 d32hi16 = vis_to_double_dup(0xffff0000);
	mlib_d64 mask_8000 = vis_to_double_dup(0x80008000);

/* prepare align for macro */
	vis_alignaddr((void *)0, 6);

/* line addresses */
	sl = (mlib_f32 *)src;
	dl = (mlib_s16 *)dst;

	if (((slb >> 1) == dlb) && (dlb == 2 * xsize)) {
		xsize *= ysize;
		ysize = 1;
	}

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the source and destination address */
		sp = sl;
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_u8 *)dl - (mlib_u8 *)dp;
		dend = dl + xsize - 1;

		off >>= 1;

		if (off)
			off0 = 3 - off;
		else
			off0 = 0;

/* first pixel column */
		emask = vis_edge16(dl, dend);

		if (emask != 0xf) {
			sd0 = vis_freg_pair(vis_ld_f32_nf(sp - off),
				vis_ld_f32_nf(sp - off + 1));
			sd1 = vis_freg_pair(vis_ld_f32_nf(sp - off + 2),
				vis_ld_f32_nf(sp - off + 3));
			sp += (4 - off);
			MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, afh[off0],
			    afl[off0], bdh[off0], bdl[off0], dd);
			vis_pst_16(dd, dp, emask);
			dp++;
			off0++;

			if (off0 == 3)
				off0 = 0;
		}

		if (((mlib_addr)sp & 7) == 0) {

/* 4-pixel loop */
			n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) / (3 * 8);

			ah = afh[off0];
			al = afl[off0];
			bh = bdh[off0];
			bl = bdl[off0];
#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				sd0 = *(mlib_d64 *)(sp + 12 * i);
				sd1 = *(mlib_d64 *)(sp + 12 * i + 2);
				MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, ah, al, bh,
				    bl, dd);
				dp[3 * i] = dd;
			}

			off1 = off0 + 1;

			if (off1 == 3)
				off1 = 0;
			ah = afh[off1];
			al = afl[off1];
			bh = bdh[off1];
			bl = bdl[off1];
#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				sd0 = *(mlib_d64 *)(sp + 12 * i + 4);
				sd1 = *(mlib_d64 *)(sp + 12 * i + 6);
				MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, ah, al, bh,
				    bl, dd);
				dp[3 * i + 1] = dd;
			}

			off2 = off1 + 1;

			if (off2 == 3)
				off2 = 0;
			ah = afh[off2];
			al = afl[off2];
			bh = bdh[off2];
			bl = bdl[off2];
#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				sd0 = *(mlib_d64 *)(sp + 12 * i + 8);
				sd1 = *(mlib_d64 *)(sp + 12 * i + 10);
				MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, ah, al, bh,
				    bl, dd);
				dp[3 * i + 2] = dd;
			}

			sp += 12 * n;
			dp += 3 * n;

/* last 4-pixel */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd0 = vis_ld_d64_nf((mlib_d64 *)(sp));
				sd1 = vis_ld_d64_nf((mlib_d64 *)(sp + 2));
				MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, afh[off0],
				    afl[off0], bdh[off0], bdl[off0], dd);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					sd0 = vis_ld_d64_nf(
						(mlib_d64 *)(sp + 4));
					sd1 = vis_ld_d64_nf(
						(mlib_d64 *)(sp + 6));
					MLIB_V_IMAGESCALE_S32_U16(sd0, sd1,
					    afh[off1], afl[off1], bdh[off1],
					    bdl[off1], dd);
					emask = vis_edge16(dp, dend);
					vis_pst_16(dd, dp, emask);
					dp++;

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						sd0 = vis_ld_d64_nf(
							(mlib_d64 *)(sp + 8));
						sd1 = vis_ld_d64_nf(
							(mlib_d64 *)(sp + 10));
						MLIB_V_IMAGESCALE_S32_U16(sd0,
						    sd1, afh[off2], afl[off2],
						    bdh[off2], bdl[off2], dd);
						emask = vis_edge16(dp, dend);
						vis_pst_16(dd, dp, emask);
						dp++;
					}
				}
			}

		} else {

/* 4-pixel loop */
			n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) / (3 * 8);

			ah = afh[off0];
			al = afl[off0];
			bh = bdh[off0];
			bl = bdl[off0];
#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				sd0 = vis_freg_pair(sp[12 * i], sp[12 * i + 1]);
				sd1 =
				    vis_freg_pair(sp[12 * i + 2],
				    sp[12 * i + 3]);
				MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, ah, al, bh,
				    bl, dd);
				dp[3 * i] = dd;
			}

			off1 = off0 + 1;

			if (off1 == 3)
				off1 = 0;
			ah = afh[off1];
			al = afl[off1];
			bh = bdh[off1];
			bl = bdl[off1];
#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				sd0 =
				    vis_freg_pair(sp[12 * i + 4],
				    sp[12 * i + 5]);
				sd1 =
				    vis_freg_pair(sp[12 * i + 6],
				    sp[12 * i + 7]);
				MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, ah, al, bh,
				    bl, dd);
				dp[3 * i + 1] = dd;
			}

			off2 = off1 + 1;

			if (off2 == 3)
				off2 = 0;
			ah = afh[off2];
			al = afl[off2];
			bh = bdh[off2];
			bl = bdl[off2];
#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				sd0 =
				    vis_freg_pair(sp[12 * i + 8],
				    sp[12 * i + 9]);
				sd1 =
				    vis_freg_pair(sp[12 * i + 10],
				    sp[12 * i + 11]);
				MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, ah, al, bh,
				    bl, dd);
				dp[3 * i + 2] = dd;
			}

			sp += 12 * n;
			dp += 3 * n;

/* last 4-pixel */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd0 = vis_freg_pair(vis_ld_f32_nf(sp),
					vis_ld_f32_nf(sp + 1));
				sd1 = vis_freg_pair(vis_ld_f32_nf(sp + 2),
					vis_ld_f32_nf(sp + 3));
				MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, afh[off0],
				    afl[off0], bdh[off0], bdl[off0], dd);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					sd0 = vis_freg_pair(
						vis_ld_f32_nf(sp + 4),
						vis_ld_f32_nf(sp + 5));
					sd1 = vis_freg_pair(
						vis_ld_f32_nf(sp + 6),
						vis_ld_f32_nf(sp + 7));
					MLIB_V_IMAGESCALE_S32_U16(sd0, sd1,
					    afh[off1], afl[off1], bdh[off1],
					    bdl[off1], dd);
					emask = vis_edge16(dp, dend);
					vis_pst_16(dd, dp, emask);
					dp++;

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						sd0 =
						    vis_freg_pair(
							vis_ld_f32_nf(sp + 8),
							vis_ld_f32_nf(sp + 9));
						sd1 =
						    vis_freg_pair(
						    vis_ld_f32_nf(sp + 10),
						    vis_ld_f32_nf(sp + 11));
						MLIB_V_IMAGESCALE_S32_U16(sd0,
						    sd1, afh[off2], afl[off2],
						    bdh[off2], bdl[off2], dd);
						emask = vis_edge16(dp, dend);
						vis_pst_16(dd, dp, emask);
						dp++;
					}
				}
			}
		}

		sl = (mlib_f32 *)((mlib_u8 *)sl + slb);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

#define	MLIB_V_IMAGESCALE_S16_U81(sdhi, sdlo, afh, afl, bdh,     \
	bdl, df)                                                 \
	rdhh = vis_fmuld8sux16(sdhi, afh);                       \
	rdhl = vis_fmuld8ulx16(sdhi, afh);                       \
	rdh = vis_fpadd32(rdhh, rdhl);                           \
	rdh = vis_fpadd32(rdh, bdh);                             \
	ddh = vis_fpack32(ddh, rdh);                             \
	rdlh = vis_fmuld8sux16(sdlo, afl);                       \
	rdll = vis_fmuld8ulx16(sdlo, afl);                       \
	rdl = vis_fpadd32(rdlh, rdll);                           \
	rdl = vis_fpadd32(rdl, bdl);                             \
	ddh = vis_fpack32(ddh, rdl);                             \
	ddh = vis_fpmerge(vis_read_hi(ddh), vis_read_lo(ddh));   \
	df = vis_read_lo(ddh)

/* *********************************************************** */

#define	MLIB_V_IMAGESCALE_S32_U8(sd0, sd1, afh, afl, bdh,             \
	bdl, df)                                                      \
	MLIB_V_SHIFT16_S32_S16(sd0, sf0);                             \
	MLIB_V_SHIFT16_S32_S16(sd1, sf1);                             \
	MLIB_V_IMAGESCALE_S16_U81(sf0, sf1, afh, afl, bdh, bdl, df)

/* *********************************************************** */

#define	MLIB_PST_8_FL(df, dl, dp, dend)                         \
	dpal = (mlib_d64 *)((mlib_addr)dp & (~7));              \
	if (dpal == (mlib_d64 *)dp)                             \
	    dd0 = vis_freg_pair(df, vis_fzeros());              \
	else                                                    \
	    dd0 = vis_freg_pair(vis_fzeros(), df);              \
	emask = vis_edge8(dl, dend);                            \
	vis_pst_8(dd0, dpal, emask)

/* *********************************************************** */

void
mlib_v_ImageScale_S32_U8_124_A8D1(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_f32 *dp;

/* source data */
	mlib_d64 sd0, sd1;

/* destination data */
	mlib_f32 df;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* temporaries used in macro */
	mlib_d64 ddh;

/* loop variables */
	mlib_s32 j;
	mlib_d64 sh;
	mlib_f32 sf0, sf1;

	mlib_d64 dzero = vis_fzero();
	mlib_d64 d32hi16 = vis_to_double_dup(0xffff0000);

	vis_alignaddr((void *)0, 6);

	sp = (mlib_d64 *)src;
	dp = (mlib_f32 *)dst;

/* 4-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 4); j += 4) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		MLIB_V_IMAGESCALE_S32_U8(sd0, sd1, vis_read_hi(ad),
		    vis_read_lo(ad), bdh, bdl, df);
		(*dp++) = df;
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_S32_U8_3_A8D1(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_f32 *afh,
    mlib_f32 *afl,
    mlib_d64 *bdh,
    mlib_d64 *bdl)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_f32 *dp;

/* source data */
	mlib_d64 sd0, sd1;

/* destination data */
	mlib_f32 dd;

/* destination data */
	mlib_d64 dd0;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* temporaries used in macro */
	mlib_d64 ddh;

/* loop variables */
	mlib_s32 j;
	mlib_d64 sh;
	mlib_f32 sf0, sf1;

	mlib_d64 dzero = vis_fzero();
	mlib_d64 d32hi16 = vis_to_double_dup(0xffff0000);
	mlib_f32 ah, al;
	mlib_d64 bh, bl;
	mlib_s32 emask, n;
	mlib_u8 *dend;
	mlib_d64 *dpal;

	sp = (mlib_d64 *)src;
	dp = (mlib_f32 *)dst;
	dend = (mlib_u8 *)dp + dsize - 1;

/* prepare align for macro */
	vis_alignaddr((void *)0, 6);

	n = dsize / 12;

/* 4-pixel loop */
	ah = afh[0];
	al = afl[0];
	bh = bdh[0];
	bl = bdl[0];
#pragma pipeloop(0)
	for (j = 0; j < n; j++) {
		sd0 = sp[6 * j];
		sd1 = sp[6 * j + 1];
		MLIB_V_IMAGESCALE_S32_U8(sd0, sd1, ah, al, bh, bl, dd);
		dp[3 * j] = dd;
	}

	ah = afh[1];
	al = afl[1];
	bh = bdh[1];
	bl = bdl[1];
#pragma pipeloop(0)
	for (j = 0; j < n; j++) {
		sd0 = sp[6 * j + 2];
		sd1 = sp[6 * j + 3];
		MLIB_V_IMAGESCALE_S32_U8(sd0, sd1, ah, al, bh, bl, dd);
		dp[3 * j + 1] = dd;
	}

	ah = afh[2];
	al = afl[2];
	bh = bdh[2];
	bl = bdl[2];
#pragma pipeloop(0)
	for (j = 0; j < n; j++) {
		sd0 = sp[6 * j + 4];
		sd1 = sp[6 * j + 5];
		MLIB_V_IMAGESCALE_S32_U8(sd0, sd1, ah, al, bh, bl, dd);
		dp[3 * j + 2] = dd;
	}

	sp += 6 * n;
	dp += 3 * n;

/* last 4-pixel */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		MLIB_V_IMAGESCALE_S32_U8(sd0, sd1, afh[0], afl[0], bdh[0],
		    bdl[0], dd);
		MLIB_PST_8_FL(dd, dp, dp, dend);
		dp++;

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd0 = vis_ld_d64_nf(sp); sp++;
			sd1 = vis_ld_d64_nf(sp); sp++;
			MLIB_V_IMAGESCALE_S32_U8(sd0, sd1, afh[1], afl[1],
			    bdh[1], bdl[1], dd);
			MLIB_PST_8_FL(dd, dp, dp, dend);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd0 = vis_ld_d64_nf(sp); sp++;
				sd1 = vis_ld_d64_nf(sp); sp++;
				MLIB_V_IMAGESCALE_S32_U8(sd0, sd1, afh[2],
				    afl[2], bdh[2], bdl[2], dd);
				MLIB_PST_8_FL(dd, dp, dp, dend);
				dp++;
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_S32_U8_124(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_f32 *afh,
    mlib_f32 *afl,
    mlib_d64 *bdh,
    mlib_d64 *bdl)
{
/* aligned dst address */
	mlib_f32 *dp;

/* source data */
	mlib_d64 sd0, sd1;

/* destination data */
	mlib_f32 dd;

/* destination data */
	mlib_d64 dd0;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* loop variables */
	mlib_s32 j;
	mlib_d64 sh;
	mlib_f32 sf0, sf1;

	mlib_f32 *sl, *sp;
	mlib_u8 *dl, *dend;
	mlib_d64 bh, bl;
	mlib_f32 ah, al;
	mlib_f32 sf[4];
	mlib_s32 off, i, n, emask;

	mlib_d64 dzero = vis_fzero();
	mlib_d64 d32hi16 = vis_to_double_dup(0xffff0000);
	mlib_d64 ddh, *dpal;

	sl = (mlib_f32 *)src;
	dl = (mlib_u8 *)dst;

/* prepare align for macro */
	vis_alignaddr((void *)0, 6);

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the source and destination address */
		sp = sl;
		dp = (mlib_f32 *)((mlib_addr)dl & (~3));
		off = (mlib_u8 *)dl - (mlib_u8 *)dp;
		dend = dl + xsize - 1;

/* prepare the alpha and beta values */
		ah = afh[off];
		al = afl[off];
		bh = bdh[off];
		bl = bdl[off];

/* first 4-pixel column */
		for (i = off; i < 4; i++) {
			sf[i] = vis_ld_f32_nf(sp); sp++;
		}
		MLIB_V_IMAGESCALE_S32_U8(*(mlib_d64 *)sf, *((mlib_d64 *)sf + 1),
		    ah, al, bh, bl, dd);
		MLIB_PST_8_FL(dd, dl, dp, dend);
		dp++;

/* 4-pixel column loop */
		n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) >> 2;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			sd0 = vis_freg_pair(sp[0], sp[1]);
			sd1 = vis_freg_pair(sp[2], sp[3]);
			MLIB_V_IMAGESCALE_S32_U8(sd0, sd1, ah, al, bh, bl, dd);
			*dp = dd;
			dp++;
			sp += 4;
		}

/* last 4-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd0 = vis_freg_pair(vis_ld_f32_nf(sp),
				vis_ld_f32_nf(sp + 1));
			sd1 = vis_freg_pair(vis_ld_f32_nf(sp + 2),
				vis_ld_f32_nf(sp + 3));
			MLIB_V_IMAGESCALE_S32_U8(sd0, sd1, ah, al, bh, bl, dd);
			MLIB_PST_8_FL(dd, dp, dp, dend);
		}

		sl = (mlib_f32 *)((mlib_u8 *)sl + slb);
		dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_S32_U8_3(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_f32 *afh,
    mlib_f32 *afl,
    mlib_d64 *bdh,
    mlib_d64 *bdl)
{
/* aligned dst address */
	mlib_f32 *dp;

/* source data */
	mlib_d64 sd0, sd1;

/* destination data */
	mlib_f32 dd;

/* destination data */
	mlib_d64 dd0;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* loop variables */
	mlib_s32 j;
	mlib_d64 sh;
	mlib_f32 sf0, sf1;

	mlib_f32 *sl, *sp;
	mlib_u8 *dl, *dend;
	mlib_d64 bh, bl;
	mlib_f32 ah, al;
	mlib_f32 sf[4];
	mlib_s32 off, off0, off1, off2, i, n, emask;

	mlib_d64 dzero = vis_fzero();
	mlib_d64 d32hi16 = vis_to_double_dup(0xffff0000);
	mlib_d64 ddh, *dpal;

/* prepare align for macro */
	vis_alignaddr((void *)0, 6);

/* line addresses */
	sl = (mlib_f32 *)src;
	dl = (mlib_u8 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the source and destination address */
		sp = sl;
		dp = (mlib_f32 *)((mlib_addr)dl & (~3));
		off = (mlib_u8 *)dl - (mlib_u8 *)dp;
		dend = dl + xsize - 1;

		if (off)
			off0 = 3 - off;
		else
			off0 = 0;

/* first pixel column */
		for (i = off; i < 4; i++) {
			sf[i] = vis_ld_f32_nf(sp); sp++;
		}
		MLIB_V_IMAGESCALE_S32_U8(*(mlib_d64 *)sf, *((mlib_d64 *)sf + 1),
		    afh[off0], afl[off0], bdh[off0], bdl[off0], dd);
		MLIB_PST_8_FL(dd, dl, dp, dend);
		dp++;
		off0++;

		if (off0 == 3)
			off0 = 0;

/* 4-pixel loop */
		n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) / (3 * 4);

		ah = afh[off0];
		al = afl[off0];
		bh = bdh[off0];
		bl = bdl[off0];
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			sd0 = vis_freg_pair(sp[12 * i], sp[12 * i + 1]);
			sd1 = vis_freg_pair(sp[12 * i + 2], sp[12 * i + 3]);
			MLIB_V_IMAGESCALE_S32_U8(sd0, sd1, ah, al, bh, bl, dd);
			dp[3 * i] = dd;
		}

		off1 = off0 + 1;

		if (off1 == 3)
			off1 = 0;
		ah = afh[off1];
		al = afl[off1];
		bh = bdh[off1];
		bl = bdl[off1];
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			sd0 = vis_freg_pair(sp[12 * i + 4], sp[12 * i + 5]);
			sd1 = vis_freg_pair(sp[12 * i + 6], sp[12 * i + 7]);
			MLIB_V_IMAGESCALE_S32_U8(sd0, sd1, ah, al, bh, bl, dd);
			dp[3 * i + 1] = dd;
		}

		off2 = off1 + 1;

		if (off2 == 3)
			off2 = 0;
		ah = afh[off2];
		al = afl[off2];
		bh = bdh[off2];
		bl = bdl[off2];
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			sd0 = vis_freg_pair(sp[12 * i + 8], sp[12 * i + 9]);
			sd1 = vis_freg_pair(sp[12 * i + 10], sp[12 * i + 11]);
			MLIB_V_IMAGESCALE_S32_U8(sd0, sd1, ah, al, bh, bl, dd);
			dp[3 * i + 2] = dd;
		}

		sp += 12 * n;
		dp += 3 * n;

/* last 4-pixel */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd0 = vis_freg_pair(vis_ld_f32_nf(sp),
				vis_ld_f32_nf(sp + 1));
			sd1 = vis_freg_pair(vis_ld_f32_nf(sp + 2),
				vis_ld_f32_nf(sp + 3));
			MLIB_V_IMAGESCALE_S32_U8(sd0, sd1, afh[off0], afl[off0],
			    bdh[off0], bdl[off0], dd);
			MLIB_PST_8_FL(dd, dp, dp, dend);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd0 = vis_freg_pair(vis_ld_f32_nf(sp + 4),
					vis_ld_f32_nf(sp + 5));
				sd1 = vis_freg_pair(vis_ld_f32_nf(sp + 6),
					vis_ld_f32_nf(sp + 7));
				MLIB_V_IMAGESCALE_S32_U8(sd0, sd1, afh[off1],
				    afl[off1], bdh[off1], bdl[off1], dd);
				MLIB_PST_8_FL(dd, dp, dp, dend);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					sd0 = vis_freg_pair(
						vis_ld_f32_nf(sp + 8),
						vis_ld_f32_nf(sp + 9));
					sd1 = vis_freg_pair(
						vis_ld_f32_nf(sp + 10),
						vis_ld_f32_nf(sp + 11));
					MLIB_V_IMAGESCALE_S32_U8(sd0, sd1,
					    afh[off2], afl[off2], bdh[off2],
					    bdl[off2], dd);
					MLIB_PST_8_FL(dd, dp, dp, dend);
					dp++;
				}
			}
		}

		sl = (mlib_f32 *)((mlib_u8 *)sl + slb);
		dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

#define	MLIB_V_IMAGESCALE_S16_U8(sd, afh, afl, bdh, bdl, df)     \
	rdhh = vis_fmuld8sux16(vis_read_hi(sd), afh);            \
	rdhl = vis_fmuld8ulx16(vis_read_hi(sd), afh);            \
	rdh = vis_fpadd32(rdhh, rdhl);                           \
	rdh = vis_fpadd32(rdh, bdh);                             \
	ddh = vis_fpack32(ddh, rdh);                             \
	rdlh = vis_fmuld8sux16(vis_read_lo(sd), afl);            \
	rdll = vis_fmuld8ulx16(vis_read_lo(sd), afl);            \
	rdl = vis_fpadd32(rdlh, rdll);                           \
	rdl = vis_fpadd32(rdl, bdl);                             \
	ddh = vis_fpack32(ddh, rdl);                             \
	ddh = vis_fpmerge(vis_read_hi(ddh), vis_read_lo(ddh));   \
	df = vis_read_lo(ddh)

/* *********************************************************** */

void
mlib_v_ImageScale_S16_U8_124_A8D1X4(
    mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_f32 *dp;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_f32 dd;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* temporaries used in macro */
	mlib_d64 ddh;

/* loop variables */
	mlib_s32 j;

	sp = (mlib_d64 *)src;
	dp = (mlib_f32 *)dst;

/* 8-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 4); j += 4) {
		sd = (*sp++);
		MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad), vis_read_lo(ad),
		    bdh, bdl, dd);
		(*dp++) = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_S16_U8_124_A8D2(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl)
{
/* aligned src address */
	mlib_d64 *sp, *sl;

/* aligned dst address */
	mlib_f32 *dp, *dl;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_f32 dfh, dfl;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* temporaries used in macro */
	mlib_d64 ddh;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_f32 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize / 8 * 2; i++) {
			sd = (*sp++);
			MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad),
			    vis_read_lo(ad), bdh, bdl, dfh);
			(*dp++) = dfh;
		}

		if (4 * i < xsize) {
			sd = sp[0];
			MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad),
			    vis_read_lo(ad), bdh, bdl, dfh);
			sd = vis_ld_d64_nf(sp + 1);
			MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad),
			    vis_read_lo(ad), bdh, bdl, dfl);
			dd = vis_freg_pair(dfh, dfl);
			emask = vis_edge8(dp, (mlib_u8 *)dl + xsize - 1);
			vis_pst_8(dd, dp, emask);
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_f32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_S16_U8_124(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_f32 *dp;

/* pointer to begin of a src line */
	mlib_s16 *sl;

/* pointer to begin of a dst line */
	mlib_u8 *dl;

/* pointer to end of a dst line */
	mlib_u8 *dend;

/* source data */
	mlib_d64 sd, s0, s1;

/* destination data */
	mlib_f32 dfh, dfl;

/* destination data */
	mlib_d64 dd;

/* alpha and beta data */
	mlib_d64 ad0, b[8];

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* temporaries used in macro */
	mlib_d64 ddh;

/* edge mask */
	mlib_s32 emask;

/* offset in dst address */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;

/* prepare the beta value */
	b[0] = b[6] = bdh;
	b[2] = b[4] = bdl;
	vis_alignaddr((void *)0, 4);
	b[3] = b[5] = vis_faligndata(bdh, bdl);
	b[1] = b[7] = vis_faligndata(bdl, bdh);

	sl = src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_f32 *)((mlib_addr)dl & (~7));
		off = (mlib_u8 *)dp - (mlib_u8 *)dl;
		dend = dl + xsize - 1;

/* prepare the beta value */
		bdh = b[(-off) & 3];
		bdl = b[((-off) & 3) + 4];

/* prepare the alpha value */
		off *= 2;
		vis_alignaddr(dp, off);
		ad0 = vis_faligndata(ad, ad);

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* first 8-pixel column */
		emask = vis_edge8(dl, dend);

		if (emask != 0xff) {
			s0 = vis_ld_d64_nf(sp);
			s1 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad0),
			    vis_read_lo(ad0), bdh, bdl, dfh);

			s0 = vis_ld_d64_nf(sp + 2);
			sd = vis_faligndata(s1, s0);
			MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad0),
			    vis_read_lo(ad0), bdh, bdl, dfl);

			sp += 2;
			dd = vis_freg_pair(dfh, dfl);
			vis_pst_8(dd, dp, emask);
			dp += 2;
		}

/* 8-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 1) >> 3;
		s1 = vis_ld_d64_nf(sp);
#pragma pipeloop(0)
		for (i = 0; i < 2 * n; i++) {
			s0 = s1;
			s1 = vis_ld_d64_nf(sp + 1);
			sp++;
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad0),
			    vis_read_lo(ad0), bdh, bdl, dfh);
			(*dp++) = dfh;
		}

/* last 8-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s0 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s1, s0);
			MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad0),
			    vis_read_lo(ad0), bdh, bdl, dfh);
			s1 = vis_ld_d64_nf(sp + 2);
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad0),
			    vis_read_lo(ad0), bdh, bdl, dfl);

			dd = vis_freg_pair(dfh, dfl);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
		}

		sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_S16_U8_3_A8D1(
    mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_f32 *dp;

/* pointer to end of dst */
	mlib_u8 *dend;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_f32 dfh, dfl;

/* destination data */
	mlib_d64 dd;

/* alpha and beta data */
	mlib_d64 ad1, bdm;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* temporaries used in macro */
	mlib_d64 ddh;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 j;

/* prepare alpha and beta values */
	vis_alignaddr(dst, 2);
	ad1 = vis_faligndata(ad, ad);
	vis_alignaddr(dst, 4);
	bdm = vis_faligndata(bdh, bdl);

	sp = (mlib_d64 *)src;
	dp = (mlib_f32 *)dst;
	dend = (mlib_u8 *)dp + dsize - 1;

/* 8-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j < (dsize / 24 * 2); j++) {
		sd = sp[3 * j];
		MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad), vis_read_lo(ad),
		    bdh, bdl, dfh);
		dp[3 * j] = dfh;
	}

#pragma pipeloop(0)
	for (j = 0; j < (dsize / 24 * 2); j++) {
		sd = sp[3 * j + 1];
		MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad1), vis_read_hi(ad),
		    bdm, bdh, dfh);
		dp[3 * j + 1] = dfh;
	}

#pragma pipeloop(0)
	for (j = 0; j < (dsize / 24 * 2); j++) {
		sd = sp[3 * j + 2];
		MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_lo(ad), vis_read_hi(ad1),
		    bdl, bdm, dfh);
		dp[3 * j + 2] = dfh;
	}

	sp += 3 * j;
	dp += 3 * j;

/* last 8-pixel */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		sd = sp[0];
		MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad), vis_read_lo(ad),
		    bdh, bdl, dfh);
		sd = vis_ld_d64_nf(sp + 1);
		MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad1), vis_read_hi(ad),
		    bdm, bdh, dfl);
		dd = vis_freg_pair(dfh, dfl);
		emask = vis_edge8(dp, dend);
		vis_pst_8(dd, dp, emask);
		dp += 2;

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd = sp[2];
			MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_lo(ad),
			    vis_read_hi(ad1), bdl, bdm, dfh);
			sd = vis_ld_d64_nf(sp + 3);
			MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad),
			    vis_read_lo(ad), bdh, bdl, dfl);
			dd = vis_freg_pair(dfh, dfl);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
			dp += 2;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd = sp[4];
				MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad1),
				    vis_read_hi(ad), bdm, bdh, dfh);
				sd = vis_ld_d64_nf(sp + 5);
				MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_lo(ad),
				    vis_read_hi(ad1), bdl, bdm, dfl);
				dd = vis_freg_pair(dfh, dfl);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_S16_U8_3_A8D2(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl)
{
/* aligned src address */
	mlib_d64 *sp, *sl;

/* aligned dst address */
	mlib_f32 *dp, *dl;

/* pointer to end of dst */
	mlib_u8 *dend;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_f32 dfh, dfl;

/* destination data */
	mlib_d64 dd;

/* alpha and beta data */
	mlib_d64 ad1, bdm;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* temporaries used in macro */
	mlib_d64 ddh;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;

/* prepare alpha and beta values */
	vis_alignaddr(dst, 2);
	ad1 = vis_faligndata(ad, ad);
	vis_alignaddr(dst, 4);
	bdm = vis_faligndata(bdh, bdl);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_f32 *)dst;

/* row loop */
	for (i = 0; i < ysize; i++) {

		dend = (mlib_u8 *)dl + xsize - 1;

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (j = 0; j < (xsize / 24 * 2); j++) {
			sd = sp[3 * j];
			MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad),
			    vis_read_lo(ad), bdh, bdl, dfh);
			dp[3 * j] = dfh;
		}

#pragma pipeloop(0)
		for (j = 0; j < (xsize / 24 * 2); j++) {
			sd = sp[3 * j + 1];
			MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad1),
			    vis_read_hi(ad), bdm, bdh, dfh);
			dp[3 * j + 1] = dfh;
		}

#pragma pipeloop(0)
		for (j = 0; j < (xsize / 24 * 2); j++) {
			sd = sp[3 * j + 2];
			MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_lo(ad),
			    vis_read_hi(ad1), bdl, bdm, dfh);
			dp[3 * j + 2] = dfh;
		}

		sp += 3 * j;
		dp += 3 * j;

/* last 8-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd = sp[0];
			MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad),
			    vis_read_lo(ad), bdh, bdl, dfh);
			sd = vis_ld_d64_nf(sp + 1);
			MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad1),
			    vis_read_hi(ad), bdm, bdh, dfl);
			dd = vis_freg_pair(dfh, dfl);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
			dp += 2;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd = sp[2];
				MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_lo(ad),
				    vis_read_hi(ad1), bdl, bdm, dfh);
				sd = sp[3];
				MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad),
				    vis_read_lo(ad), bdh, bdl, dfl);
				dd = vis_freg_pair(dfh, dfl);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
				dp += 2;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					sd = sp[4];
					MLIB_V_IMAGESCALE_S16_U8(sd,
					    vis_read_hi(ad1), vis_read_hi(ad),
					    bdm, bdh, dfh);
					sd = vis_ld_d64_nf(sp + 5);
					MLIB_V_IMAGESCALE_S16_U8(sd,
					    vis_read_lo(ad), vis_read_hi(ad1),
					    bdl, bdm, dfl);
					dd = vis_freg_pair(dfh, dfl);
					emask = vis_edge8(dp, dend);
					vis_pst_8(dd, dp, emask);
				}
			}
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_f32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_S16_U8_3(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_f32 *dp;

/* pointer to src line */
	mlib_s16 *sl;

/* pointer to dst line */
	mlib_u8 *dl;

/* pointer to end of dst */
	mlib_u8 *dend;

/* source data */
	mlib_d64 sd, s0, s1;

/* destination data */
	mlib_f32 dfh, dfl;

/* destination data */
	mlib_d64 dd;

/* alpha data */
	mlib_d64 ad1, ad2, a[10];

/* beta data */
	mlib_d64 bdm, b[10];

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* temporaries used in macro */
	mlib_d64 ddh;

/* edge mask */
	mlib_s32 emask;

/* offset of address in dst */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;

/* prepare the alpha value */
	a[1] = a[4] = a[7] = ad;
	vis_alignaddr((void *)0, 2);
	ad2 = vis_faligndata(ad, ad);
	a[2] = a[5] = a[8] = ad1 = vis_faligndata(ad, ad2);
	vis_alignaddr((void *)0, 4);
	a[0] = a[3] = a[6] = a[9] = vis_faligndata(ad, ad1);

/* prepare the beta value */
	b[0] = b[3] = b[6] = b[9] = bdh;
	b[1] = b[4] = b[7] = bdl;
	b[2] = b[5] = b[8] = vis_faligndata(bdh, bdl);

	sl = src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_f32 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

/* prepare the alpha and beta */
		bdh = b[-off];
		bdl = b[1 - off];
		bdm = b[2 - off];
		ad = a[7 + off];
		ad1 = a[8 + off];
		ad2 = a[9 + off];

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, 2 * off);

/* first 8-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s0 = vis_ld_d64_nf(sp);
			s1 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad),
			    vis_read_lo(ad), bdh, bdl, dfh);
			s0 = vis_ld_d64_nf(sp + 2);
			sd = vis_faligndata(s1, s0);
			MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad1),
			    vis_read_hi(ad), bdm, bdh, dfl);
			sp += 2;
			dd = vis_freg_pair(dfh, dfl);
			emask = vis_edge8(dl, dend);
			vis_pst_8(dd, dp, emask);
			dp += 2;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s1 = vis_ld_d64_nf(sp + 1);
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_lo(ad),
				    vis_read_hi(ad1), bdl, bdm, dfh);
				s0 = sp[2];
				sd = vis_faligndata(s1, s0);
				MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad),
				    vis_read_lo(ad), bdh, bdl, dfl);
				sp += 2;
				dd = vis_freg_pair(dfh, dfl);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
				dp += 2;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					s1 = vis_ld_d64_nf(sp + 1);
					sd = vis_faligndata(s0, s1);
					MLIB_V_IMAGESCALE_S16_U8(sd,
					    vis_read_hi(ad1), vis_read_hi(ad),
					    bdm, bdh, dfh);
					s0 = sp[2];
					sd = vis_faligndata(s1, s0);
					MLIB_V_IMAGESCALE_S16_U8(sd,
					    vis_read_lo(ad), vis_read_hi(ad1),
					    bdl, bdm, dfl);
					sp += 2;
					dd = vis_freg_pair(dfh, dfl);
					emask = vis_edge8(dp, dend);
					vis_pst_8(dd, dp, emask);
					dp += 2;
				}
			}
		}

/* 8-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 1) / 24;
		s0 = vis_ld_d64_nf(sp);
		s1 = vis_ld_d64_nf(sp + 1);
		sd = vis_faligndata(s0, s1);
#pragma pipeloop(0)
		for (i = 0; i < 2 * n; i++) {
			MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad),
			    vis_read_lo(ad), bdh, bdl, dfh);
			s0 = sp[3 * i + 3];
			s1 = vis_ld_d64_nf(sp + 3 * i + 4);
			sd = vis_faligndata(s0, s1);
			dp[3 * i] = dfh;
		}

		s0 = vis_ld_d64_nf(sp + 1);
		s1 = vis_ld_d64_nf(sp + 2);
		sd = vis_faligndata(s0, s1);
#pragma pipeloop(0)
		for (i = 0; i < 2 * n; i++) {
			MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad1),
			    vis_read_hi(ad), bdm, bdh, dfh);
			s0 = vis_ld_d64_nf(sp + 3 * i + 4);
			s1 = vis_ld_d64_nf(sp + 3 * i + 5);
			sd = vis_faligndata(s0, s1);
			dp[3 * i + 1] = dfh;
		}

		s0 = vis_ld_d64_nf(sp + 2);
		s1 = vis_ld_d64_nf(sp + 3);
		sd = vis_faligndata(s0, s1);
#pragma pipeloop(0)
		for (i = 0; i < 2 * n; i++) {
			MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_lo(ad),
			    vis_read_hi(ad1), bdl, bdm, dfh);
			s0 = vis_ld_d64_nf(sp + 3 * i + 5);
			s1 = vis_ld_d64_nf(sp + 3 * i + 6);
			sd = vis_faligndata(s0, s1);
			dp[3 * i + 2] = dfh;
		}

		sp += 3 * i;
		dp += 3 * i;

/* last 8-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s0 = vis_ld_d64_nf(sp);
			s1 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad),
			    vis_read_lo(ad), bdh, bdl, dfh);
			s0 = vis_ld_d64_nf(sp + 2);
			sd = vis_faligndata(s1, s0);
			MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad1),
			    vis_read_hi(ad), bdm, bdh, dfl);
			dd = vis_freg_pair(dfh, dfl);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
			dp += 2;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s1 = sp[3];
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_lo(ad),
				    vis_read_hi(ad1), bdl, bdm, dfh);
				s0 = sp[4];
				sd = vis_faligndata(s1, s0);
				MLIB_V_IMAGESCALE_S16_U8(sd, vis_read_hi(ad),
				    vis_read_lo(ad), bdh, bdl, dfl);
				dd = vis_freg_pair(dfh, dfl);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
				dp += 2;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					s1 = vis_ld_d64_nf(sp + 5);
					sd = vis_faligndata(s0, s1);
					MLIB_V_IMAGESCALE_S16_U8(sd,
					    vis_read_hi(ad1), vis_read_hi(ad),
					    bdm, bdh, dfh);
					sd = vis_faligndata(s1,
						vis_ld_d64_nf(sp + 6));
					MLIB_V_IMAGESCALE_S16_U8(sd,
					    vis_read_lo(ad), vis_read_hi(ad1),
					    bdl, bdm, dfl);
					dd = vis_freg_pair(dfh, dfl);
					emask = vis_edge8(dp, dend);
					vis_pst_8(dd, dp, emask);
				}
			}
		}

		sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dl += dlb;
	}
}

/* *********************************************************** */

#define	MLIB_V_IMAGESCALE_U16_U8(sd, afh, afl, bdh, bdl, df)    \
	sd = vis_fpsub16(sd, mask_8000);                        \
	MLIB_V_IMAGESCALE_S16_U8(sd, afh, afl, bdh, bdl, df)

/* *********************************************************** */

void
mlib_v_ImageScale_U16_U8_124(
    mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_f32 *dp;

/* pointer to begin of a src line */
	mlib_s16 *sl;

/* pointer to begin of a dst line */
	mlib_u8 *dl;

/* pointer to end of a dst line */
	mlib_u8 *dend;

/* source data */
	mlib_d64 sd, s0, s1;

/* destination data */
	mlib_f32 dfh, dfl;

/* destination data */
	mlib_d64 dd;

/* alpha and beta data */
	mlib_d64 ad0, b[8];

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* temporaries used in macro */
	mlib_d64 ddh;

/* edge mask */
	mlib_s32 emask;

/* offset in dst address */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;
	mlib_d64 mask_8000 = vis_to_double_dup(0x80008000);

/* prepare the beta value */
	b[0] = b[6] = bdh;
	b[2] = b[4] = bdl;
	vis_alignaddr((void *)0, 4);
	b[3] = b[5] = vis_faligndata(bdh, bdl);
	b[1] = b[7] = vis_faligndata(bdl, bdh);

	sl = (mlib_s16 *)src;
	dl = dst;

	if (((slb >> 1) == dlb) && (dlb == xsize)) {
		xsize *= ysize;
		ysize = 1;
	}

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_f32 *)((mlib_addr)dl & (~7));
		off = (mlib_u8 *)dp - (mlib_u8 *)dl;
		dend = dl + xsize - 1;

/* prepare the beta value */
		bdh = b[(-off) & 3];
		bdl = b[((-off) & 3) + 4];

/* prepare the alpha value */
		off *= 2;
		vis_alignaddr(dp, off);
		ad0 = vis_faligndata(ad, ad);

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

		if (((mlib_addr)((mlib_u8 *)sl + off) & 7) == 0) {

/* first 8-pixel column */
			emask = vis_edge8(dl, dend);

			if (emask != 0xff) {
				sd = sp[0];
				MLIB_V_IMAGESCALE_U16_U8(sd, vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl, dfh);

				sd = vis_ld_d64_nf(sp + 1);
				MLIB_V_IMAGESCALE_U16_U8(sd, vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl, dfl);

				sp += 2;
				dd = vis_freg_pair(dfh, dfl);
				vis_pst_8(dd, dp, emask);
				dp += 2;
			}

/* 8-pixel column loop */
			n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 1) >> 3;
			s1 = vis_ld_d64_nf(sp);
#pragma pipeloop(0)
			for (i = 0; i < 2 * n; i++) {
				sd = sp[0];
				sp++;
				MLIB_V_IMAGESCALE_U16_U8(sd, vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl, dfh);
				(*dp++) = dfh;
			}

/* last 8-pixel column */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd = sp[0];
				MLIB_V_IMAGESCALE_U16_U8(sd, vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl, dfh);
				sd = vis_ld_d64_nf(sp + 1);
				MLIB_V_IMAGESCALE_U16_U8(sd, vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl, dfl);

				dd = vis_freg_pair(dfh, dfl);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
			}

		} else {

/* first 8-pixel column */
			emask = vis_edge8(dl, dend);

			if (emask != 0xff) {
				s0 = vis_ld_d64_nf(sp);
				s1 = vis_ld_d64_nf(sp + 1);
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_U16_U8(sd, vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl, dfh);

				s0 = vis_ld_d64_nf(sp + 2);
				sd = vis_faligndata(s1, s0);
				MLIB_V_IMAGESCALE_U16_U8(sd, vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl, dfl);

				sp += 2;
				dd = vis_freg_pair(dfh, dfl);
				vis_pst_8(dd, dp, emask);
				dp += 2;
			}

/* 8-pixel column loop */
			n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 1) >> 3;
			s1 = vis_ld_d64_nf(sp);
#pragma pipeloop(0)
			for (i = 0; i < 2 * n; i++) {
				s0 = s1;
				s1 = sp[1];
				sp++;
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_U16_U8(sd, vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl, dfh);
				(*dp++) = dfh;
			}

/* last 8-pixel column */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s0 = vis_ld_d64_nf(sp + 1);
				sd = vis_faligndata(s1, s0);
				MLIB_V_IMAGESCALE_U16_U8(sd, vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl, dfh);
				s1 = vis_ld_d64_nf(sp + 2);
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_U16_U8(sd, vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl, dfl);

				dd = vis_freg_pair(dfh, dfl);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
			}
		}

		sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_U16_U8_3(
    mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_f32 *dp;

/* pointer to src line */
	mlib_s16 *sl;

/* pointer to dst line */
	mlib_u8 *dl;

/* pointer to end of dst */
	mlib_u8 *dend;

/* source data */
	mlib_d64 sd, s0, s1;

/* destination data */
	mlib_f32 dfh, dfl;

/* destination data */
	mlib_d64 dd;

/* alpha data */
	mlib_d64 ad1, ad2, a[10];

/* beta data */
	mlib_d64 bdm, b[10];

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* temporaries used in macro */
	mlib_d64 ddh;

/* edge mask */
	mlib_s32 emask;

/* offset of address in dst */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;
	mlib_d64 mask_8000 = vis_to_double_dup(0x80008000);

/* prepare the alpha value */
	a[1] = a[4] = a[7] = ad;
	vis_alignaddr((void *)0, 2);
	ad2 = vis_faligndata(ad, ad);
	a[2] = a[5] = a[8] = ad1 = vis_faligndata(ad, ad2);
	vis_alignaddr((void *)0, 4);
	a[0] = a[3] = a[6] = a[9] = vis_faligndata(ad, ad1);

/* prepare the beta value */
	b[0] = b[3] = b[6] = b[9] = bdh;
	b[1] = b[4] = b[7] = bdl;
	b[2] = b[5] = b[8] = vis_faligndata(bdh, bdl);

	sl = (mlib_s16 *)src;
	dl = dst;

	if (((slb >> 1) == dlb) && (dlb == xsize)) {
		xsize *= ysize;
		ysize = 1;
	}

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_f32 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

/* prepare the alpha and beta */
		bdh = b[-off];
		bdl = b[1 - off];
		bdm = b[2 - off];
		ad = a[7 + off];
		ad1 = a[8 + off];
		ad2 = a[9 + off];

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, 2 * off);

		if (((mlib_addr)(sl + off) & 7) == 0) {

/* first 8-pixel column */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd = sp[0];
				MLIB_V_IMAGESCALE_U16_U8(sd, vis_read_hi(ad),
				    vis_read_lo(ad), bdh, bdl, dfh);
				sd = vis_ld_d64_nf(sp + 1);
				MLIB_V_IMAGESCALE_U16_U8(sd, vis_read_hi(ad1),
				    vis_read_hi(ad), bdm, bdh, dfl);
				sp += 2;
				dd = vis_freg_pair(dfh, dfl);
				emask = vis_edge8(dl, dend);
				vis_pst_8(dd, dp, emask);
				dp += 2;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					sd = sp[0];
					MLIB_V_IMAGESCALE_U16_U8(sd,
					    vis_read_lo(ad), vis_read_hi(ad1),
					    bdl, bdm, dfh);
					sd = vis_ld_d64_nf(sp + 1);
					MLIB_V_IMAGESCALE_U16_U8(sd,
					    vis_read_hi(ad), vis_read_lo(ad),
					    bdh, bdl, dfl);
					sp += 2;
					dd = vis_freg_pair(dfh, dfl);
					emask = vis_edge8(dp, dend);
					vis_pst_8(dd, dp, emask);
					dp += 2;

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						sd = sp[0];
						MLIB_V_IMAGESCALE_U16_U8(sd,
						    vis_read_hi(ad1),
						    vis_read_hi(ad), bdm, bdh,
						    dfh);
						sd = vis_ld_d64_nf(sp + 1);
						MLIB_V_IMAGESCALE_U16_U8(sd,
						    vis_read_lo(ad),
						    vis_read_hi(ad1), bdl, bdm,
						    dfl);
						sp += 2;
						dd = vis_freg_pair(dfh, dfl);
						emask = vis_edge8(dp, dend);
						vis_pst_8(dd, dp, emask);
						dp += 2;
					}
				}
			}

/* 8-pixel column loop */
			n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 1) / 24;
#pragma pipeloop(0)
			for (i = 0; i < 2 * n; i++) {
				sd = sp[3 * i];
				MLIB_V_IMAGESCALE_U16_U8(sd, vis_read_hi(ad),
				    vis_read_lo(ad), bdh, bdl, dfh);
				dp[3 * i] = dfh;
			}

#pragma pipeloop(0)
			for (i = 0; i < 2 * n; i++) {
				sd = sp[3 * i + 1];
				MLIB_V_IMAGESCALE_U16_U8(sd, vis_read_hi(ad1),
				    vis_read_hi(ad), bdm, bdh, dfh);
				dp[3 * i + 1] = dfh;
			}

#pragma pipeloop(0)
			for (i = 0; i < 2 * n; i++) {
				sd = sp[3 * i + 2];
				MLIB_V_IMAGESCALE_U16_U8(sd, vis_read_lo(ad),
				    vis_read_hi(ad1), bdl, bdm, dfh);
				dp[3 * i + 2] = dfh;
			}

			sp += 3 * i;
			dp += 3 * i;

/* last 8-pixel column */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd = sp[0];
				MLIB_V_IMAGESCALE_U16_U8(sd, vis_read_hi(ad),
				    vis_read_lo(ad), bdh, bdl, dfh);
				sd = vis_ld_d64_nf(sp + 1);
				MLIB_V_IMAGESCALE_U16_U8(sd, vis_read_hi(ad1),
				    vis_read_hi(ad), bdm, bdh, dfl);
				dd = vis_freg_pair(dfh, dfl);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
				dp += 2;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					sd = sp[2];
					MLIB_V_IMAGESCALE_U16_U8(sd,
					    vis_read_lo(ad), vis_read_hi(ad1),
					    bdl, bdm, dfh);
					sd = vis_ld_d64_nf(sp + 3);
					MLIB_V_IMAGESCALE_U16_U8(sd,
					    vis_read_hi(ad), vis_read_lo(ad),
					    bdh, bdl, dfl);
					dd = vis_freg_pair(dfh, dfl);
					emask = vis_edge8(dp, dend);
					vis_pst_8(dd, dp, emask);
					dp += 2;

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						sd = sp[4];
						MLIB_V_IMAGESCALE_U16_U8(sd,
						    vis_read_hi(ad1),
						    vis_read_hi(ad), bdm, bdh,
						    dfh);
						sd = vis_ld_d64_nf(sp + 5);
						MLIB_V_IMAGESCALE_U16_U8(sd,
						    vis_read_lo(ad),
						    vis_read_hi(ad1), bdl, bdm,
						    dfl);
						dd = vis_freg_pair(dfh, dfl);
						emask = vis_edge8(dp, dend);
						vis_pst_8(dd, dp, emask);
					}
				}
			}

		} else {

/* first 8-pixel column */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s0 = vis_ld_d64_nf(sp);
				s1 = vis_ld_d64_nf(sp + 1);
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_U16_U8(sd, vis_read_hi(ad),
				    vis_read_lo(ad), bdh, bdl, dfh);
				s0 = vis_ld_d64_nf(sp + 2);
				sd = vis_faligndata(s1, s0);
				MLIB_V_IMAGESCALE_U16_U8(sd, vis_read_hi(ad1),
				    vis_read_hi(ad), bdm, bdh, dfl);
				sp += 2;
				dd = vis_freg_pair(dfh, dfl);
				emask = vis_edge8(dl, dend);
				vis_pst_8(dd, dp, emask);
				dp += 2;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					s1 = vis_ld_d64_nf(sp + 1);
					sd = vis_faligndata(s0, s1);
					MLIB_V_IMAGESCALE_U16_U8(sd,
					    vis_read_lo(ad), vis_read_hi(ad1),
					    bdl, bdm, dfh);
					s0 = sp[2];
					sd = vis_faligndata(s1, s0);
					MLIB_V_IMAGESCALE_U16_U8(sd,
					    vis_read_hi(ad), vis_read_lo(ad),
					    bdh, bdl, dfl);
					sp += 2;
					dd = vis_freg_pair(dfh, dfl);
					emask = vis_edge8(dp, dend);
					vis_pst_8(dd, dp, emask);
					dp += 2;

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						s1 = vis_ld_d64_nf(sp + 1);
						sd = vis_faligndata(s0, s1);
						MLIB_V_IMAGESCALE_U16_U8(sd,
						    vis_read_hi(ad1),
						    vis_read_hi(ad), bdm, bdh,
						    dfh);
						s0 = sp[2];
						sd = vis_faligndata(s1, s0);
						MLIB_V_IMAGESCALE_U16_U8(sd,
						    vis_read_lo(ad),
						    vis_read_hi(ad1), bdl, bdm,
						    dfl);
						sp += 2;
						dd = vis_freg_pair(dfh, dfl);
						emask = vis_edge8(dp, dend);
						vis_pst_8(dd, dp, emask);
						dp += 2;
					}
				}
			}

/* 8-pixel column loop */
			n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 1) / 24;
			s0 = vis_ld_d64_nf(sp);
			s1 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s0, s1);
#pragma pipeloop(0)
			for (i = 0; i < 2 * n; i++) {
				MLIB_V_IMAGESCALE_U16_U8(sd, vis_read_hi(ad),
				    vis_read_lo(ad), bdh, bdl, dfh);
				s0 = sp[3 * i + 3];
				s1 = vis_ld_d64_nf(sp + 3 * i + 4);
				sd = vis_faligndata(s0, s1);
				dp[3 * i] = dfh;
			}

			s0 = vis_ld_d64_nf(sp + 1);
			s1 = vis_ld_d64_nf(sp + 2);
			sd = vis_faligndata(s0, s1);
#pragma pipeloop(0)
			for (i = 0; i < 2 * n; i++) {
				MLIB_V_IMAGESCALE_U16_U8(sd, vis_read_hi(ad1),
				    vis_read_hi(ad), bdm, bdh, dfh);
				s0 = vis_ld_d64_nf(sp + 3 * i + 4);
				s1 = vis_ld_d64_nf(sp + 3 * i + 5);
				sd = vis_faligndata(s0, s1);
				dp[3 * i + 1] = dfh;
			}

			s0 = vis_ld_d64_nf(sp + 2);
			s1 = vis_ld_d64_nf(sp + 3);
			sd = vis_faligndata(s0, s1);
#pragma pipeloop(0)
			for (i = 0; i < 2 * n; i++) {
				MLIB_V_IMAGESCALE_U16_U8(sd, vis_read_lo(ad),
				    vis_read_hi(ad1), bdl, bdm, dfh);
				s0 = vis_ld_d64_nf(sp + 3 * i + 5);
				s1 = vis_ld_d64_nf(sp + 3 * i + 6);
				sd = vis_faligndata(s0, s1);
				dp[3 * i + 2] = dfh;
			}

			sp += 3 * i;
			dp += 3 * i;

/* last 8-pixel column */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s0 = vis_ld_d64_nf(sp);
				s1 = vis_ld_d64_nf(sp + 1);
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_U16_U8(sd, vis_read_hi(ad),
				    vis_read_lo(ad), bdh, bdl, dfh);
				s0 = vis_ld_d64_nf(sp + 2);
				sd = vis_faligndata(s1, s0);
				MLIB_V_IMAGESCALE_U16_U8(sd, vis_read_hi(ad1),
				    vis_read_hi(ad), bdm, bdh, dfl);
				dd = vis_freg_pair(dfh, dfl);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
				dp += 2;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					s1 = vis_ld_d64_nf(sp + 3);
					sd = vis_faligndata(s0, s1);
					MLIB_V_IMAGESCALE_U16_U8(sd,
					    vis_read_lo(ad), vis_read_hi(ad1),
					    bdl, bdm, dfh);
					s0 = vis_ld_d64_nf(sp + 4);
					sd = vis_faligndata(s1, s0);
					MLIB_V_IMAGESCALE_U16_U8(sd,
					    vis_read_hi(ad), vis_read_lo(ad),
					    bdh, bdl, dfl);
					dd = vis_freg_pair(dfh, dfl);
					emask = vis_edge8(dp, dend);
					vis_pst_8(dd, dp, emask);
					dp += 2;

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						s1 = vis_ld_d64_nf(sp + 5);
						sd = vis_faligndata(s0, s1);
						MLIB_V_IMAGESCALE_U16_U8(sd,
						    vis_read_hi(ad1),
						    vis_read_hi(ad), bdm, bdh,
						    dfh);
						sd = vis_faligndata(s1,
							vis_ld_d64_nf(sp + 6));
						MLIB_V_IMAGESCALE_U16_U8(sd,
						    vis_read_lo(ad),
						    vis_read_hi(ad1), bdl, bdm,
						    dfl);
						dd = vis_freg_pair(dfh, dfl);
						emask = vis_edge8(dp, dend);
						vis_pst_8(dd, dp, emask);
					}
				}
			}
		}

		sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dl += dlb;
	}
}

/* *********************************************************** */
