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

#pragma ident	"@(#)mlib_v_ImageBlendFunc.c	9.3	07/11/05 SMI"

/*
 * Internal functions.
 *
 * void mlib_v_ImageBlend_U8_A8D1X8(mlib_u8  *src1,
 *                                  mlib_u8  *src2,
 *                                  mlib_u8  *dst,
 *                                  mlib_u8  *aimg,
 *                                  mlib_s32 dsize);
 *
 * void mlib_v_ImageBlend_U8_A8D2(mlib_u8  *src1,
 *                                mlib_s32 slb1,
 *                                mlib_u8  *src2,
 *                                mlib_s32 slb2,
 *                                mlib_u8  *dst,
 *                                mlib_s32 dlb,
 *                                mlib_u8  *aimg,
 *                                mlib_s32 alb,
 *                                mlib_s32 xsize,
 *                                mlib_s32 ysize);
 *
 * void mlib_v_ImageBlend_U8(mlib_u8  *src1,
 *                           mlib_s32 slb1,
 *                           mlib_u8  *src2,
 *                           mlib_s32 slb2,
 *                           mlib_u8  *dst,
 *                           mlib_s32 dlb,
 *                           mlib_u8  *aimg,
 *                           mlib_s32 alb,
 *                           mlib_s32 xsize,
 *                           mlib_s32 ysize);
 *
 * void mlib_v_ImageBlend_S16_A8D1X4(mlib_s16 *src1,
 *                                   mlib_s16 *src2,
 *                                   mlib_s16 *dst,
 *                                   mlib_s16 *aimg,
 *                                   mlib_s32 dsize);
 *
 * void mlib_v_ImageBlend_S16_A8D2(mlib_s16 *src1,
 *                                 mlib_s32 slb1,
 *                                 mlib_s16 *src2,
 *                                 mlib_s32 slb2,
 *                                 mlib_s16 *dst,
 *                                 mlib_s32 dlb,
 *                                 mlib_s16 *aimg,
 *                                 mlib_s32 alb,
 *                                 mlib_s32 xsize,
 *                                 mlib_s32 ysize);
 *
 * void mlib_v_ImageBlend_S16(mlib_s16 *src1,
 *                            mlib_s32 slb1,
 *                            mlib_s16 *src2,
 *                            mlib_s32 slb2,
 *                            mlib_s16 *dst,
 *                            mlib_s32 dlb,
 *                            mlib_s16 *aimg,
 *                            mlib_s32 alb,
 *                            mlib_s32 xsize,
 *                            mlib_s32 ysize);
 *
 * void mlib_v_ImageBlend_U16_A8D1X4(mlib_u16 *src1,
 *                                   mlib_u16 *src2,
 *                                   mlib_u16 *dst,
 *                                   mlib_u16 *aimg,
 *                                   mlib_s32 dsize);
 *
 * void mlib_v_ImageBlend_U16_A8D2(mlib_u16 *src1,
 *                                 mlib_s32 slb1,
 *                                 mlib_u16 *src2,
 *                                 mlib_s32 slb2,
 *                                 mlib_u16 *dst,
 *                                 mlib_s32 dlb,
 *                                 mlib_u16 *aimg,
 *                                 mlib_s32 alb,
 *                                 mlib_s32 xsize,
 *                                 mlib_s32 ysize);
 *
 * void mlib_v_ImageBlend_U16(mlib_u16 *src1,
 *                            mlib_s32 slb1,
 *                            mlib_u16 *src2,
 *                            mlib_s32 slb2,
 *                            mlib_u16 *dst,
 *                            mlib_s32 dlb,
 *                            mlib_u16 *aimg,
 *                            mlib_s32 alb,
 *                            mlib_s32 xsize,
 *                            mlib_s32 ysize);
 *
 * void mlib_v_ImageBlend_S32(mlib_s32 *sa1,
 *                            mlib_s32 slb1,
 *                            mlib_s32 *sa2,
 *                            mlib_s32 slb2,
 *                            mlib_s32 *da,
 *                            mlib_s32 dlb,
 *                            mlib_s32 *aa,
 *                            mlib_s32 alb,
 *                            mlib_s32 xsize,
 *                            mlib_s32 ysize);
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_v_ImageBlendFunc.h>

/* *********************************************************** */

#define	MLIB_V_IMAGEBLEND_U8(sd1, sd2, ad, dd)                  \
	adh = vis_fexpand_hi(ad);                               \
	adl = vis_fexpand_lo(ad);                               \
	bdh = vis_fpsub16(ones, adh);                           \
	bdl = vis_fpsub16(ones, adl);                           \
	rd1h = vis_fmul8x16(vis_read_hi(sd1), adh);             \
	rd1l = vis_fmul8x16(vis_read_lo(sd1), adl);             \
	rd2h = vis_fmul8x16(vis_read_hi(sd2), bdh);             \
	rd2l = vis_fmul8x16(vis_read_lo(sd2), bdl);             \
	rdh = vis_fpadd16(rd1h, rd2h);                          \
	rdl = vis_fpadd16(rd1l, rd2l);                          \
	dd = vis_fpack16_pair(rdh, rdl)

/* *********************************************************** */

void
mlib_v_ImageBlend_U8_A8D1X8(
    mlib_u8 *src1,
    mlib_u8 *src2,
    mlib_u8 *dst,
    mlib_u8 *aimg,
    mlib_s32 dsize)
{
	mlib_d64 *sp1, *sp2;
	mlib_d64 sd1, sd2;
	mlib_d64 *dp;
	mlib_d64 dd;
	mlib_d64 *ap;
	mlib_d64 ad;
	mlib_d64 adh, adl, bdh, bdl;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rdh, rdl;
	mlib_d64 ones = vis_to_double_dup(0x0ff00ff0);
	mlib_s32 j;

	sp1 = (mlib_d64 *)src1;
	sp2 = (mlib_d64 *)src2;
	dp = (mlib_d64 *)dst;
	ap = (mlib_d64 *)aimg;

/* 8-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 8); j += 8) {
		sd1 = (*sp1++);
		sd2 = (*sp2++);
		ad = (*ap++);
		MLIB_V_IMAGEBLEND_U8(sd1, sd2, ad, dd);
		(*dp++) = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlend_U8_A8D2(
    mlib_u8 *src1,
    mlib_s32 slb1,
    mlib_u8 *src2,
    mlib_s32 slb2,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_u8 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_d64 *sp1, *sl1;
	mlib_d64 *sp2, *sl2;
	mlib_d64 sd1, sd2;
	mlib_d64 *dp, *dl;
	mlib_d64 dd;
	mlib_d64 *ap, *al;
	mlib_d64 ad;
	mlib_d64 adh, adl, bdh, bdl;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rdh, rdl;
	mlib_d64 ones = vis_to_double_dup(0x0ff00ff0);
	mlib_s32 emask;
	mlib_s32 i, j;

	sl1 = sp1 = (mlib_d64 *)src1;
	sl2 = sp2 = (mlib_d64 *)src2;
	dl = dp = (mlib_d64 *)dst;
	al = ap = (mlib_d64 *)aimg;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 8); i += 8) {
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			ad = (*ap++);
			MLIB_V_IMAGEBLEND_U8(sd1, sd2, ad, dd);
			(*dp++) = dd;
		}

/* last 8-pixel column loop */

		if (i < xsize) {
			sd1 = vis_ld_d64_nf(sp1);
			sd2 = vis_ld_d64_nf(sp2);
			ad = vis_ld_d64_nf(ap);
			MLIB_V_IMAGEBLEND_U8(sd1, sd2, ad, dd);
			emask = vis_edge8((void *)0, (void *)(xsize - i - 1));
			vis_pst_8(dd, dp, emask);
		}

		sl1 = sp1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
		sl2 = sp2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
		dl = dp = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		al = ap = (mlib_d64 *)((mlib_u8 *)al + alb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlend_U8(
    mlib_u8 *src1,
    mlib_s32 slb1,
    mlib_u8 *src2,
    mlib_s32 slb2,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_u8 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u8 *sa1, *sl1;
	mlib_u8 *sa2, *sl2;
	mlib_u8 *aa, *al;
	mlib_d64 *sp1, *sp2, *ap;
	mlib_u8 *dl, *dend;
	mlib_d64 *dp;
	mlib_d64 s11, s10;
	mlib_d64 s21, s20;
	mlib_d64 a0, a1;
	mlib_d64 sd10, sd20, ad0;
	mlib_d64 dd;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rdh, rdl;
	mlib_d64 ones = vis_to_double_dup(0x0ff00ff0);
	mlib_d64 adh, adl, bdh, bdl;
	mlib_s32 off;
	mlib_s32 emask;
	mlib_s32 i, j, n;

	sl1 = sa1 = src1;
	sl2 = sa2 = src2;
	al = aa = aimg;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

		sp1 = (mlib_d64 *)vis_alignaddr(sa1, off);
		s10 = vis_ld_d64_nf(sp1);
		sp2 = (mlib_d64 *)vis_alignaddr(sa2, off);
		s20 = vis_ld_d64_nf(sp2);
		ap = (mlib_d64 *)vis_alignaddr(aa, off);
		a0 = vis_ld_d64_nf(ap);

		emask = vis_edge8(dl, dend);

/* first 8-pixel column */

		if (emask != 0xff) {
			a1 = vis_ld_d64_nf(ap + 1);
			ad0 = vis_faligndata(a0, a1);
			a0 = a1;
			ap++;

			vis_alignaddr(sa1, off);
			s11 = vis_ld_d64_nf(sp1 + 1);
			sd10 = vis_faligndata(s10, s11);
			s10 = s11;
			sp1++;

			vis_alignaddr(sa2, off);
			s21 = vis_ld_d64_nf(sp2 + 1);
			sd20 = vis_faligndata(s20, s21);
			s20 = s21;
			sp2++;

			MLIB_V_IMAGEBLEND_U8(sd10, sd20, ad0, dd);
			vis_pst_8(dd, dp, emask);
			dp++;
			vis_alignaddr(aa, off);
		}

/* 8-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 1) >> 3;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			vis_alignaddr(sa1, off);
			s11 = vis_ld_d64_nf(sp1 + 1);
			sd10 = vis_faligndata(s10, s11);
			s10 = s11;
			sp1++;

			vis_alignaddr(sa2, off);
			s21 = vis_ld_d64_nf(sp2 + 1);
			sd20 = vis_faligndata(s20, s21);
			s20 = s21;
			sp2++;

			vis_alignaddr(aa, off);
			a1 = vis_ld_d64_nf(ap + 1);
			ad0 = vis_faligndata(a0, a1);
			a0 = a1;
			ap++;

			MLIB_V_IMAGEBLEND_U8(sd10, sd20, ad0, dd);
			(*dp++) = dd;
		}

/* last 8-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			ad0 = vis_faligndata(a0, vis_ld_d64_nf(ap + 1));

			vis_alignaddr(sa1, off);
			sd10 = vis_faligndata(s10, vis_ld_d64_nf(sp1 + 1));

			vis_alignaddr(sa2, off);
			sd20 = vis_faligndata(s20, vis_ld_d64_nf(sp2 + 1));

			MLIB_V_IMAGEBLEND_U8(sd10, sd20, ad0, dd);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
		}

		sl1 = sa1 = sl1 + slb1;
		sl2 = sa2 = sl2 + slb2;
		al = aa = al + alb;
		dl += dlb;
	}
}

/* *********************************************************** */

#define	MLIB_V_IMAGEBLEND_S16(sd1, sd2, ad, dd)                 \
	ad = vis_fand(ones, ad);                                \
	bd = vis_fpsub16(ones, ad);                             \
	rd1h = vis_fmul8sux16(ad, sd1);                         \
	rd1l = vis_fmul8ulx16(ad, sd1);                         \
	rd1 = vis_fpadd16(rd1h, rd1l);                          \
	rd2h = vis_fmul8sux16(bd, sd2);                         \
	rd2l = vis_fmul8ulx16(bd, sd2);                         \
	rd2 = vis_fpadd16(rd2h, rd2l);                          \
	dd = vis_fpadd16(rd1, rd2);                             \
	dd = vis_fpadd16(dd, dd)

/* *********************************************************** */

void
mlib_v_ImageBlend_S16_A8D1X4(
    mlib_s16 *src1,
    mlib_s16 *src2,
    mlib_s16 *dst,
    mlib_s16 *aimg,
    mlib_s32 dsize)
{
	mlib_d64 *sp1, *sp2;
	mlib_d64 sd1, sd2;
	mlib_d64 *dp;
	mlib_d64 dd;
	mlib_d64 *ap;
	mlib_d64 ad, bd;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rd1, rd2;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_s32 j;

	sp1 = (mlib_d64 *)src1;
	sp2 = (mlib_d64 *)src2;
	dp = (mlib_d64 *)dst;
	ap = (mlib_d64 *)aimg;

/* 4-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j < dsize / 4; j++) {
		sd1 = sp1[j];
		sd2 = sp2[j];
		ad = ap[j];
		MLIB_V_IMAGEBLEND_S16(sd1, sd2, ad, dd);
		dp[j] = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlend_S16_A8D2(
    mlib_s16 *src1,
    mlib_s32 slb1,
    mlib_s16 *src2,
    mlib_s32 slb2,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s16 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_d64 *sp1, *sl1;
	mlib_d64 *sp2, *sl2;
	mlib_d64 sd1, sd2;
	mlib_d64 *dp, *dl;
	mlib_d64 dd;
	mlib_d64 *ap, *al;
	mlib_d64 ad, bd;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rd1, rd2;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);

/* edge mask */
	mlib_s32 emask;
	mlib_s32 i, j;

	sl1 = sp1 = (mlib_d64 *)src1;
	sl2 = sp2 = (mlib_d64 *)src2;
	dl = dp = (mlib_d64 *)dst;
	al = ap = (mlib_d64 *)aimg;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize / 4; i++) {
			sd1 = sp1[i];
			sd2 = sp2[i];
			ad = ap[i];
			MLIB_V_IMAGEBLEND_S16(sd1, sd2, ad, dd);
			dp[i] = dd;
		}

/* last 4-pixel column */

		if (4 * i < xsize) {
			sd1 = vis_ld_d64_nf(sp1 + i);
			sd2 = vis_ld_d64_nf(sp2 + i);
			ad = vis_ld_d64_nf(ap + i);
			MLIB_V_IMAGEBLEND_S16(sd1, sd2, ad, dd);
			emask =
			    vis_edge16((void *)0,
			    (void *)(2 * (xsize - 4 * i - 1)));
			vis_pst_16(dd, dp + i, emask);
		}

		sl1 = sp1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
		sl2 = sp2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
		dl = dp = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		al = ap = (mlib_d64 *)((mlib_u8 *)al + alb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlend_S16(
    mlib_s16 *src1,
    mlib_s32 slb1,
    mlib_s16 *src2,
    mlib_s32 slb2,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s16 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s16 *sa1, *sl1;
	mlib_s16 *sa2, *sl2;
	mlib_s16 *aa, *al;
	mlib_d64 *sp1, *sp2, *ap;
	mlib_s16 *dl, *dend;
	mlib_d64 *dp;
	mlib_d64 s11, s10;
	mlib_d64 s21, s20;
	mlib_d64 a0, a1;
	mlib_d64 sd10, sd20, ad0;
	mlib_d64 bd;
	mlib_d64 dd;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rd1, rd2;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_s32 off;
	mlib_s32 emask;
	mlib_s32 i, j, n;

	sl1 = sa1 = src1;
	sl2 = sa2 = src2;
	al = aa = aimg;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

		sp1 = (mlib_d64 *)vis_alignaddr(sa1, off);
		s10 = vis_ld_d64_nf(sp1);
		sp2 = (mlib_d64 *)vis_alignaddr(sa2, off);
		s20 = vis_ld_d64_nf(sp2);
		ap = (mlib_d64 *)vis_alignaddr(aa, off);
		a0 = vis_ld_d64_nf(ap);

		emask = vis_edge16(dl, dend);

/* first 4-pixel column loop */

		if (emask != 0xf) {
			vis_alignaddr(sa1, off);
			s11 = vis_ld_d64_nf(sp1 + 1);
			sd10 = vis_faligndata(s10, s11);
			s10 = s11;
			sp1++;

			vis_alignaddr(sa2, off);
			s21 = vis_ld_d64_nf(sp2 + 1);
			sd20 = vis_faligndata(s20, s21);
			s20 = s21;
			sp2++;

			vis_alignaddr(aa, off);
			a1 = vis_ld_d64_nf(ap + 1);
			ad0 = vis_faligndata(a0, a1);
			a0 = a1;
			ap++;

			MLIB_V_IMAGEBLEND_S16(sd10, sd20, ad0, dd);
			vis_pst_16(dd, dp, emask);
			dp++;
		}

/* 4-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 2) >> 3;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			vis_alignaddr(sa1, off);
			s11 = vis_ld_d64_nf(sp1 + 1);
			sd10 = vis_faligndata(s10, s11);
			s10 = s11;
			sp1++;

			vis_alignaddr(sa2, off);
			s21 = vis_ld_d64_nf(sp2 + 1);
			sd20 = vis_faligndata(s20, s21);
			s20 = s21;
			sp2++;

			vis_alignaddr(aa, off);
			a1 = vis_ld_d64_nf(ap + 1);
			ad0 = vis_faligndata(a0, a1);
			a0 = a1;
			ap++;

			MLIB_V_IMAGEBLEND_S16(sd10, sd20, ad0, dd);
			(*dp++) = dd;
		}

/* last 4-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			ad0 = vis_faligndata(a0, vis_ld_d64_nf(ap + 1));

			vis_alignaddr(sa1, off);
			sd10 = vis_faligndata(s10, vis_ld_d64_nf(sp1 + 1));

			vis_alignaddr(sa2, off);
			sd20 = vis_faligndata(s20, vis_ld_d64_nf(sp2 + 1));

			MLIB_V_IMAGEBLEND_S16(sd10, sd20, ad0, dd);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
		}

		sl1 = sa1 = (mlib_s16 *)((mlib_u8 *)sl1 + slb1);
		sl2 = sa2 = (mlib_s16 *)((mlib_u8 *)sl2 + slb2);
		al = aa = (mlib_s16 *)((mlib_u8 *)al + alb);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

#define	MLIB_V_IMAGEBLEND_U16(sd1, sd2, ad, dd)                 \
	sd1 = vis_fxnor(ones, sd1);                             \
	sd2 = vis_fxnor(ones, sd2);                             \
	ad = vis_fand(ad, mask);                                \
	ad = vis_fmul8x16(fmin, ad);                            \
	ad = vis_fand(ad, ones);                                \
	bd = vis_fpsub16(ones, ad);                             \
	rd1h = vis_fmul8sux16(ad, sd1);                         \
	rd1l = vis_fmul8ulx16(ad, sd1);                         \
	rd1 = vis_fpadd16(rd1h, rd1l);                          \
	rd2h = vis_fmul8sux16(bd, sd2);                         \
	rd2l = vis_fmul8ulx16(bd, sd2);                         \
	rd2 = vis_fpadd16(rd2h, rd2l);                          \
	dd = vis_fpadd16(rd1, rd2);                             \
	dd = vis_fpadd16(dd, dd);                               \
	dd = vis_fxnor(ones, dd)

/* *********************************************************** */

void
mlib_v_ImageBlend_U16_A8D1X4(
    mlib_u16 *src1,
    mlib_u16 *src2,
    mlib_u16 *dst,
    mlib_u16 *aimg,
    mlib_s32 dsize)
{
	mlib_d64 *sp1, *sp2;
	mlib_d64 sd1, sd2;
	mlib_d64 *dp;
	mlib_d64 dd;
	mlib_d64 *ap;
	mlib_d64 ad, bd;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rd1, rd2;
	mlib_d64 mask = vis_to_double_dup(0xfffefffe);
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_f32 fmin = vis_to_float(0x80808080);
	mlib_s32 j;

	sp1 = (mlib_d64 *)src1;
	sp2 = (mlib_d64 *)src2;
	dp = (mlib_d64 *)dst;
	ap = (mlib_d64 *)aimg;

/* 4-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j < dsize / 4; j++) {
		sd1 = sp1[j];
		sd2 = sp2[j];
		ad = ap[j];
		MLIB_V_IMAGEBLEND_U16(sd1, sd2, ad, dd);
		dp[j] = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlend_U16_A8D2(
    mlib_u16 *src1,
    mlib_s32 slb1,
    mlib_u16 *src2,
    mlib_s32 slb2,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_u16 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_d64 *sp1, *sl1;
	mlib_d64 *sp2, *sl2;
	mlib_d64 sd1, sd2;
	mlib_d64 *dp, *dl;
	mlib_d64 dd;
	mlib_d64 *ap, *al;
	mlib_d64 ad, bd;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rd1, rd2;
	mlib_d64 mask = vis_to_double_dup(0xfffefffe);
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_f32 fmin = vis_to_float(0x80808080);

/* edge mask */
	mlib_s32 emask;
	mlib_s32 i, j;

	sl1 = sp1 = (mlib_d64 *)src1;
	sl2 = sp2 = (mlib_d64 *)src2;
	dl = dp = (mlib_d64 *)dst;
	al = ap = (mlib_d64 *)aimg;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize / 4; i++) {
			sd1 = sp1[i];
			sd2 = sp2[i];
			ad = ap[i];
			MLIB_V_IMAGEBLEND_U16(sd1, sd2, ad, dd);
			dp[i] = dd;
		}

/* last 4-pixel column */

		if (4 * i < xsize) {
			sd1 = vis_ld_d64_nf(sp1 + i);
			sd2 = vis_ld_d64_nf(sp2 + i);
			ad = vis_ld_d64_nf(ap + i);
			MLIB_V_IMAGEBLEND_U16(sd1, sd2, ad, dd);
			emask =
			    vis_edge16((void *)0,
			    (void *)(2 * (xsize - 4 * i - 1)));
			vis_pst_16(dd, dp + i, emask);
		}

		sl1 = sp1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
		sl2 = sp2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
		dl = dp = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		al = ap = (mlib_d64 *)((mlib_u8 *)al + alb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlend_U16(
    mlib_u16 *src1,
    mlib_s32 slb1,
    mlib_u16 *src2,
    mlib_s32 slb2,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_u16 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u16 *sa1, *sl1;
	mlib_u16 *sa2, *sl2;
	mlib_u16 *aa, *al;
	mlib_d64 *sp1, *sp2, *ap;
	mlib_u16 *dl, *dend;
	mlib_d64 *dp;
	mlib_d64 s11, s10;
	mlib_d64 s21, s20;
	mlib_d64 a0, a1;
	mlib_d64 sd10, sd20, ad0;
	mlib_d64 bd;
	mlib_d64 dd;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rd1, rd2;
	mlib_d64 mask = vis_to_double_dup(0xfffefffe);
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_f32 fmin = vis_to_float(0x80808080);
	mlib_s32 off;
	mlib_s32 emask;
	mlib_s32 i, j, n;

	sl1 = sa1 = src1;
	sl2 = sa2 = src2;
	al = aa = aimg;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

		sp1 = (mlib_d64 *)vis_alignaddr(sa1, off);
		s10 = vis_ld_d64_nf(sp1);
		sp2 = (mlib_d64 *)vis_alignaddr(sa2, off);
		s20 = vis_ld_d64_nf(sp2);
		ap = (mlib_d64 *)vis_alignaddr(aa, off);
		a0 = vis_ld_d64_nf(ap);

		emask = vis_edge16(dl, dend);

/* first 4-pixel column loop */

		if (emask != 0xf) {
			vis_alignaddr(sa1, off);
			s11 = vis_ld_d64_nf(sp1 + 1);
			sd10 = vis_faligndata(s10, s11);
			s10 = s11;
			sp1++;

			vis_alignaddr(sa2, off);
			s21 = vis_ld_d64_nf(sp2 + 1);
			sd20 = vis_faligndata(s20, s21);
			s20 = s21;
			sp2++;

			vis_alignaddr(aa, off);
			a1 = vis_ld_d64_nf(ap + 1);
			ad0 = vis_faligndata(a0, a1);
			a0 = a1;
			ap++;

			MLIB_V_IMAGEBLEND_U16(sd10, sd20, ad0, dd);
			vis_pst_16(dd, dp, emask);
			dp++;
		}

/* 4-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 2) >> 3;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			vis_alignaddr(sa1, off);
			s11 = vis_ld_d64_nf(sp1 + 1);
			sd10 = vis_faligndata(s10, s11);
			s10 = s11;
			sp1++;

			vis_alignaddr(sa2, off);
			s21 = vis_ld_d64_nf(sp2 + 1);
			sd20 = vis_faligndata(s20, s21);
			s20 = s21;
			sp2++;

			vis_alignaddr(aa, off);
			a1 = vis_ld_d64_nf(ap + 1);
			ad0 = vis_faligndata(a0, a1);
			a0 = a1;
			ap++;

			MLIB_V_IMAGEBLEND_U16(sd10, sd20, ad0, dd);
			(*dp++) = dd;
		}

/* last 4-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			ad0 = vis_faligndata(a0, vis_ld_d64_nf(ap + 1));

			vis_alignaddr(sa1, off);
			sd10 = vis_faligndata(s10, vis_ld_d64_nf(sp1 + 1));

			vis_alignaddr(sa2, off);
			sd20 = vis_faligndata(s20, vis_ld_d64_nf(sp2 + 1));

			MLIB_V_IMAGEBLEND_U16(sd10, sd20, ad0, dd);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
		}

		sl1 = sa1 = (mlib_u16 *)((mlib_u8 *)sl1 + slb1);
		sl2 = sa2 = (mlib_u16 *)((mlib_u8 *)sl2 + slb2);
		al = aa = (mlib_u16 *)((mlib_u8 *)al + alb);
		dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlend_S32(
    mlib_s32 *sa1,
    mlib_s32 slb1,
    mlib_s32 *sa2,
    mlib_s32 slb2,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 *aa,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* indices for x and y */
	mlib_s32 i, j;

/* variable for alpha coefficients */
	mlib_d64 fa;
	mlib_d64 fshift = -1.0 / MLIB_S32_MIN;

	for (j = 0; j < ysize; j++) {
#pragma pipeloop(0)
		for (i = 0; i < xsize; i++) {
			fa = mlib_fabs((mlib_d64)aa[i]) * fshift;
			da[i] = (mlib_s32)(fa * (mlib_d64)sa1[i]
			    + (1 - fa) * (mlib_d64)sa2[i]);
		}

		sa1 = (mlib_s32 *)((mlib_u8 *)sa1 + slb1);
		sa2 = (mlib_s32 *)((mlib_u8 *)sa2 + slb2);
		da = (mlib_s32 *)((mlib_u8 *)da + dlb);
		aa = (mlib_s32 *)((mlib_u8 *)aa + alb);
	}
}

/* *********************************************************** */
