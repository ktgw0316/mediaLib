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

#pragma ident	"@(#)mlib_v_ImageScalarBlendFunc.c	9.3	07/11/05 SMI"

/*
 * Internal functions.
 *
 *
 *     void mlib_v_ImageScalarBlend_U8_124_A8D1X8(mlib_u8  *src1,
 *                                                mlib_u8  *src2,
 *                                                mlib_u8  *dst,
 *                                                mlib_s32 dsize,
 *                                                mlib_d64 ad);
 *
 *     void mlib_v_ImageScalarBlend_U8_124_A8D2(mlib_u8  *src1,
 *                                              mlib_s32 slb1,
 *                                              mlib_u8  *src2,
 *                                              mlib_s32 slb2,
 *                                              mlib_u8  *dst,
 *                                              mlib_s32 dlb,
 *                                              mlib_s32 xsize,
 *                                              mlib_s32 ysize,
 *                                              mlib_d64 ad);
 *
 *     void mlib_v_ImageScalarBlend_U8_124(mlib_u8  *src1,
 *                                         mlib_s32 slb1,
 *                                         mlib_u8  *src2,
 *                                         mlib_s32 slb2,
 *                                         mlib_u8  *dst,
 *                                         mlib_s32 dlb,
 *                                         mlib_s32 xsize,
 *                                         mlib_s32 ysize,
 *                                         mlib_d64 ad0);
 *
 *     void mlib_v_ImageScalarBlend_U8_3_A8D1(mlib_u8  *src1,
 *                                            mlib_u8  *src2,
 *                                            mlib_u8  *dst,
 *                                            mlib_s32 dsize,
 *                                            mlib_d64 a0);
 *
 *     void mlib_v_ImageScalarBlend_U8_3_A8D2(mlib_u8  *src1,
 *                                            mlib_s32 slb1,
 *                                            mlib_u8  *src2,
 *                                            mlib_s32 slb2,
 *                                            mlib_u8  *dst,
 *                                            mlib_s32 dlb,
 *                                            mlib_s32 xsize,
 *                                            mlib_s32 ysize,
 *                                            mlib_d64 a0);
 *
 *     void mlib_v_ImageScalarBlend_U8_3(mlib_u8  *src1,
 *                                       mlib_s32 slb1,
 *                                       mlib_u8  *src2,
 *                                       mlib_s32 slb2,
 *                                       mlib_u8  *dst,
 *                                       mlib_s32 dlb,
 *                                       mlib_s32 xsize,
 *                                       mlib_s32 ysize,
 *                                       mlib_d64 a0);
 *
 *     void mlib_v_ImageScalarBlend_S16_124_A8D1X4(mlib_s16 *src1,
 *                                                 mlib_s16 *src2,
 *                                                 mlib_s16 *dst,
 *                                                 mlib_s32 dsize,
 *                                                 mlib_d64 ad);
 *
 *     void mlib_v_ImageScalarBlend_S16_124_A8D2(mlib_s16 *src1,
 *                                               mlib_s32 slb1,
 *                                               mlib_s16 *src2,
 *                                               mlib_s32 slb2,
 *                                               mlib_s16 *dst,
 *                                               mlib_s32 dlb,
 *                                               mlib_s32 xsize,
 *                                               mlib_s32 ysize,
 *                                               mlib_d64 ad);
 *
 *     void mlib_v_ImageScalarBlend_S16_124(mlib_s16 *src1,
 *                                          mlib_s32 slb1,
 *                                          mlib_s16 *src2,
 *                                          mlib_s32 slb2,
 *                                          mlib_s16 *dst,
 *                                          mlib_s32 dlb,
 *                                          mlib_s32 xsize,
 *                                          mlib_s32 ysize,
 *                                          mlib_d64 ad0);
 *
 *     void mlib_v_ImageScalarBlend_S16_3_A8D1(mlib_s16 *src1,
 *                                             mlib_s16 *src2,
 *                                             mlib_s16 *dst,
 *                                             mlib_s32 dsize,
 *                                             mlib_d64 a0);
 *
 *     void mlib_v_ImageScalarBlend_S16_3_A8D2(mlib_s16 *src1,
 *                                             mlib_s32 slb1,
 *                                             mlib_s16 *src2,
 *                                             mlib_s32 slb2,
 *                                             mlib_s16 *dst,
 *                                             mlib_s32 dlb,
 *                                             mlib_s32 xsize,
 *                                             mlib_s32 ysize,
 *                                             mlib_d64 a0);
 *
 *     void mlib_v_ImageScalarBlend_S16_3(mlib_s16 *src1,
 *                                        mlib_s32 slb1,
 *                                        mlib_s16 *src2,
 *                                        mlib_s32 slb2,
 *                                        mlib_s16 *dst,
 *                                        mlib_s32 dlb,
 *                                        mlib_s32 xsize,
 *                                        mlib_s32 ysize,
 *                                        mlib_d64 a0);
 *
 *     void mlib_v_ImageScalarBlend_S32_1(mlib_s32 *sa1,
 *                                        mlib_s32 slb1,
 *                                        mlib_s32 *sa2,
 *                                        mlib_s32 slb2,
 *                                        mlib_s32 *da,
 *                                        mlib_s32 dlb,
 *                                        mlib_s32 xsize,
 *                                        mlib_s32 ysize,
 *                                        mlib_s32 *alpha);
 *
 *     void mlib_v_ImageScalarBlend_S32_2(mlib_s32 *sa1,
 *                                        mlib_s32 slb1,
 *                                        mlib_s32 *sa2,
 *                                        mlib_s32 slb2,
 *                                        mlib_s32 *da,
 *                                        mlib_s32 dlb,
 *                                        mlib_s32 xsize,
 *                                        mlib_s32 ysize,
 *                                        mlib_s32 *alpha);
 *
 *     void mlib_v_ImageScalarBlend_S32_3(mlib_s32 *sa1,
 *                                        mlib_s32 slb1,
 *                                        mlib_s32 *sa2,
 *                                        mlib_s32 slb2,
 *                                        mlib_s32 *da,
 *                                        mlib_s32 dlb,
 *                                        mlib_s32 xsize,
 *                                        mlib_s32 ysize,
 *                                        mlib_s32 *alpha);
 *
 *     void mlib_v_ImageScalarBlend_S32_4(mlib_s32 *sa1,
 *                                        mlib_s32 slb1,
 *                                        mlib_s32 *sa2,
 *                                        mlib_s32 slb2,
 *                                        mlib_s32 *da,
 *                                        mlib_s32 dlb,
 *                                        mlib_s32 xsize,
 *                                        mlib_s32 ysize,
 *                                        mlib_s32 *alpha);
 *
 *     void mlib_v_ImageScalarBlend_U16_124_A8D1X4(mlib_u16 *src1,
 *                                                 mlib_u16 *src2,
 *                                                 mlib_u16 *dst,
 *                                                 mlib_s32 dsize,
 *                                                 mlib_d64 ad);
 *
 *     void mlib_v_ImageScalarBlend_U16_124_A8D2(mlib_u16 *src1,
 *                                               mlib_s32 slb1,
 *                                               mlib_u16 *src2,
 *                                               mlib_s32 slb2,
 *                                               mlib_u16 *dst,
 *                                               mlib_s32 dlb,
 *                                               mlib_s32 xsize,
 *                                               mlib_s32 ysize,
 *                                               mlib_d64 ad);
 *
 *     void mlib_v_ImageScalarBlend_U16_124(mlib_u16 *src1,
 *                                          mlib_s32 slb1,
 *                                          mlib_u16 *src2,
 *                                          mlib_s32 slb2,
 *                                          mlib_u16 *dst,
 *                                          mlib_s32 dlb,
 *                                          mlib_s32 xsize,
 *                                          mlib_s32 ysize,
 *                                          mlib_d64 ad0);
 *
 *     void mlib_v_ImageScalarBlend_U16_3_A8D1(mlib_u16 *src1,
 *                                             mlib_u16 *src2,
 *                                             mlib_u16 *dst,
 *                                             mlib_s32 dsize,
 *                                             mlib_d64 a0);
 *
 *     void mlib_v_ImageScalarBlend_U16_3_A8D2(mlib_u16 *src1,
 *                                             mlib_s32 slb1,
 *                                             mlib_u16 *src2,
 *                                             mlib_s32 slb2,
 *                                             mlib_u16 *dst,
 *                                             mlib_s32 dlb,
 *                                             mlib_s32 xsize,
 *                                             mlib_s32 ysize,
 *                                             mlib_d64 a0);
 *
 *     void mlib_v_ImageScalarBlend_U16_3(mlib_u16 *src1,
 *                                        mlib_s32 slb1,
 *                                        mlib_u16 *src2,
 *                                        mlib_s32 slb2,
 *                                        mlib_u16 *dst,
 *                                        mlib_s32 dlb,
 *                                        mlib_s32 xsize,
 *                                        mlib_s32 ysize,
 *                                        mlib_d64 a0);
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageScalarBlendFunc.h>

/* *********************************************************** */

#define	MASK	0x7fffffff

/* *********************************************************** */

#define	MLIB_V_IMAGESCALARBLEND_U8(sd1, sd2, adh, bdh, adl, bdl)   \
	rd1h = vis_fmul8x16(vis_read_hi(sd1), adh);                \
	rd2h = vis_fmul8x16(vis_read_hi(sd2), bdh);                \
	rd1l = vis_fmul8x16(vis_read_lo(sd1), adl);                \
	rd2l = vis_fmul8x16(vis_read_lo(sd2), bdl);                \
	rdh = vis_fpadd16(rd1h, rd2h);                             \
	rdl = vis_fpadd16(rd1l, rd2l);                             \
	dd = vis_fpack16_pair(rdh, rdl)

/* *********************************************************** */

void
mlib_v_ImageScalarBlend_U8_124_A8D1X8(
    mlib_u8 *src1,
    mlib_u8 *src2,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_d64 ad)
{
	mlib_d64 *sp1, *sp2, *dp;
	mlib_d64 sd1, sd2, dd;

/* intermediate result */
	mlib_d64 rd1h, rd2h, rdh;

/* intermediate result */
	mlib_d64 rd1l, rd2l, rdl;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_d64 bd;
	mlib_s32 j;

	bd = vis_fpsub16(ones, ad);

	sp1 = (mlib_d64 *)src1;
	sp2 = (mlib_d64 *)src2;
	dp = (mlib_d64 *)dst;

/* 8-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 8); j += 8) {
		sd1 = (*sp1++);
		sd2 = (*sp2++);
		MLIB_V_IMAGESCALARBLEND_U8(sd1, sd2, ad, bd, ad, bd);
		(*dp++) = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageScalarBlend_U8_124_A8D2(
    mlib_u8 *src1,
    mlib_s32 slb1,
    mlib_u8 *src2,
    mlib_s32 slb2,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad)
{
	mlib_d64 *sp1, *sl1;
	mlib_d64 *sp2, *sl2;
	mlib_d64 *dp, *dl;
	mlib_d64 sd1, sd2;
	mlib_d64 dd;

/* intermediate result */
	mlib_d64 rd1h, rd2h, rdh;

/* intermediate result */
	mlib_d64 rd1l, rd2l, rdl;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_d64 bd;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 i, j;

	bd = vis_fpsub16(ones, ad);

	sl1 = sp1 = (mlib_d64 *)src1;
	sl2 = sp2 = (mlib_d64 *)src2;
	dl = dp = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 8); i += 8) {
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_V_IMAGESCALARBLEND_U8(sd1, sd2, ad, bd, ad, bd);
			(*dp++) = dd;
		}

/* last 8-pixel column loop */

		if (i < xsize) {
			sd1 = *sp1;
			sd2 = *sp2;
			MLIB_V_IMAGESCALARBLEND_U8(sd1, sd2, ad, bd, ad, bd);
			emask = vis_edge8((void *)0, (void *)(xsize - i - 1));
			vis_pst_8(dd, dp, emask);
		}

		sl1 = sp1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
		sl2 = sp2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
		dl = dp = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageScalarBlend_U8_124(
    mlib_u8 *src1,
    mlib_s32 slb1,
    mlib_u8 *src2,
    mlib_s32 slb2,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad0)
{
/* pointers to a line in source images */
	mlib_u8 *sl1, *sl2;

/* pointers to source images */
	mlib_u8 *sa1, *sa2;

/* 8-byte aligned pointers to source images */
	mlib_d64 *sp1, *sp2;

/* pointer to destination image */
	mlib_u8 *da;

/* pointer to end of a line in destination image */
	mlib_u8 *dend;

/* 8-byte aligned pointer to destination image */
	mlib_d64 *dp;

/* offset of address alignment in destination */
	mlib_s32 off;

/* edge masks */
	mlib_s32 emask;

/* source data */
	mlib_d64 s11, s10;

/* source data */
	mlib_d64 s21, s20;

/* source data */
	mlib_d64 sd10, sd20;

/* destination data */
	mlib_d64 dd;

/* intermediate result */
	mlib_d64 rd1h, rd2h, rdh;

/* intermediate result */
	mlib_d64 rd1l, rd2l, rdl;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);

/* (1-alpha) data */
	mlib_d64 ad, bd;

/* loop variable */
	mlib_s32 i, j, n;

	sl1 = sa1 = src1;
	sl2 = sa2 = src2;
	da = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)da & (~7));
		off = (mlib_addr)dp - (mlib_addr)da;
		dend = da + xsize - 1;

/* prepare the constant value */
		vis_alignaddr(dp, 2 * off);
		ad = vis_faligndata(ad0, ad0);
		bd = vis_fpsub16(ones, ad);

/* generate edge mask for the start point */
		emask = vis_edge8(da, dend);

		sp2 = (mlib_d64 *)vis_alignaddr(sa2, off);
		s21 = vis_ld_d64_nf(sp2);
		sp1 = (mlib_d64 *)vis_alignaddr(sa1, off);
		s11 = vis_ld_d64_nf(sp1);

/* first 8-pixel column loop */

		if (emask != 0xff) {
			s10 = s11;
			s11 = vis_ld_d64_nf(sp1 + 1);
			sd10 = vis_faligndata(s10, s11);
			sp1++;

			vis_alignaddr(sa2, off);
			s20 = s21;
			s21 = vis_ld_d64_nf(sp2 + 1);
			sd20 = vis_faligndata(s20, s21);
			sp2++;

			MLIB_V_IMAGESCALARBLEND_U8(sd10, sd20, ad, bd, ad, bd);
			vis_pst_8(dd, dp, emask);
			dp++;
			vis_alignaddr(sa1, off);
		}

/* 8-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 1) >> 3;

#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s10 = s11;
			s11 = vis_ld_d64_nf(sp1 + 1);
			sd10 = vis_faligndata(s10, s11);

			vis_alignaddr(sa2, off);
			s20 = s21;
			s21 = vis_ld_d64_nf(sp2 + 1);
			sd20 = vis_faligndata(s20, s21);

			MLIB_V_IMAGESCALARBLEND_U8(sd10, sd20, ad, bd, ad, bd);
			(*dp++) = dd;
			sp1++;
			sp2++;
			vis_alignaddr(sa1, off);
		}

/* last 8-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd10 = vis_faligndata(s11, vis_ld_d64_nf(sp1 + 1));
			vis_alignaddr(sa2, off);
			sd20 = vis_faligndata(s21, vis_ld_d64_nf(sp2 + 1));
			MLIB_V_IMAGESCALARBLEND_U8(sd10, sd20, ad, bd, ad, bd);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
		}

		sl1 = sa1 = sl1 + slb1;
		sl2 = sa2 = sl2 + slb2;
		da += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageScalarBlend_U8_3_A8D1(
    mlib_u8 *src1,
    mlib_u8 *src2,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_d64 a0)
{
	mlib_d64 *sp1, *sp2, *dp;

/* pointer to end of dst */
	mlib_u8 *dend;
	mlib_d64 sd1, sd2;

/* destination data */
	mlib_d64 dd;

/* intermediate result */
	mlib_d64 rd1h, rd2h, rdh;

/* intermediate result */
	mlib_d64 rd1l, rd2l, rdl;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_d64 a1, a2, b0, b1, b2;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 j;

/* prepare the alpha and (1 - alpha) values */
	vis_alignaddr(dst, 2);
	a2 = vis_faligndata(a0, a0);
	a1 = vis_faligndata(a0, a2);
	vis_alignaddr(dst, 4);
	a2 = vis_faligndata(a0, a1);
	b0 = vis_fpsub16(ones, a0);
	b1 = vis_fpsub16(ones, a1);
	b2 = vis_fpsub16(ones, a2);

	sp1 = (mlib_d64 *)src1;
	sp2 = (mlib_d64 *)src2;
	dp = (mlib_d64 *)dst;
	dend = (mlib_u8 *)dp + dsize - 1;

/* 8-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 24); j += 24) {
		sd1 = (*sp1++);
		sd2 = (*sp2++);
		MLIB_V_IMAGESCALARBLEND_U8(sd1, sd2, a0, b0, a1, b1);
		(*dp++) = dd;

		sd1 = (*sp1++);
		sd2 = (*sp2++);
		MLIB_V_IMAGESCALARBLEND_U8(sd1, sd2, a2, b2, a0, b0);
		(*dp++) = dd;

		sd1 = (*sp1++);
		sd2 = (*sp2++);
		MLIB_V_IMAGESCALARBLEND_U8(sd1, sd2, a1, b1, a2, b2);
		(*dp++) = dd;
	}

/* last 8-pixel column */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		sd1 = sp1[0];
		sd2 = sp2[0];
		MLIB_V_IMAGESCALARBLEND_U8(sd1, sd2, a0, b0, a1, b1);
		emask = vis_edge8(dp, dend);
		vis_pst_8(dd, dp, emask);
		dp++;

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd1 = sp1[1];
			sd2 = sp2[1];
			MLIB_V_IMAGESCALARBLEND_U8(sd1, sd2, a2, b2, a0, b0);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd1 = sp1[2];
				sd2 = sp2[2];
				MLIB_V_IMAGESCALARBLEND_U8(sd1, sd2, a1, b1, a2,
				    b2);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageScalarBlend_U8_3_A8D2(
    mlib_u8 *src1,
    mlib_s32 slb1,
    mlib_u8 *src2,
    mlib_s32 slb2,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 a0)
{
	mlib_d64 *sp1, *sl1;
	mlib_d64 *sp2, *sl2;
	mlib_d64 *dp, *dl;

/* pointer to end of dst */
	mlib_u8 *dend;
	mlib_d64 sd1, sd2;

/* destination data */
	mlib_d64 dd;

/* intermediate result */
	mlib_d64 rd1h, rd2h, rdh;

/* intermediate result */
	mlib_d64 rd1l, rd2l, rdl;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_d64 a1, a2, b0, b1, b2;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;

/* prepare the alpha and (1 - alpha) values */
	vis_alignaddr(dst, 2);
	a2 = vis_faligndata(a0, a0);
	a1 = vis_faligndata(a0, a2);
	vis_alignaddr(dst, 4);
	a2 = vis_faligndata(a0, a1);
	b0 = vis_fpsub16(ones, a0);
	b1 = vis_fpsub16(ones, a1);
	b2 = vis_fpsub16(ones, a2);

	sl1 = sp1 = (mlib_d64 *)src1;
	sl2 = sp2 = (mlib_d64 *)src2;
	dl = dp = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

		dend = (mlib_u8 *)dp + xsize - 1;

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 24); i += 24) {
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_V_IMAGESCALARBLEND_U8(sd1, sd2, a0, b0, a1, b1);
			(*dp++) = dd;

			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_V_IMAGESCALARBLEND_U8(sd1, sd2, a2, b2, a0, b0);
			(*dp++) = dd;

			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_V_IMAGESCALARBLEND_U8(sd1, sd2, a1, b1, a2, b2);
			(*dp++) = dd;
		}

/* last 8-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd1 = sp1[0];
			sd2 = sp2[0];
			MLIB_V_IMAGESCALARBLEND_U8(sd1, sd2, a0, b0, a1, b1);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd1 = sp1[1];
				sd2 = sp2[1];
				MLIB_V_IMAGESCALARBLEND_U8(sd1, sd2, a2, b2, a0,
				    b0);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					sd1 = sp1[2];
					sd2 = sp2[2];
					MLIB_V_IMAGESCALARBLEND_U8(sd1, sd2, a1,
					    b1, a2, b2);
					emask = vis_edge8(dp, dend);
					vis_pst_8(dd, dp, emask);
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
mlib_v_ImageScalarBlend_U8_3(
    mlib_u8 *src1,
    mlib_s32 slb1,
    mlib_u8 *src2,
    mlib_s32 slb2,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 a0)
{
/* pointers to a line in source images */
	mlib_u8 *sl1, *sl2;

/* pointers to source images */
	mlib_u8 *sa1, *sa2;

/* 8-byte aligned pointers to source images */
	mlib_d64 *sp1, *sp2;

/* pointer to destination image */
	mlib_u8 *da;

/* pointer to end of a line in destination image */
	mlib_u8 *dend;

/* 8-byte aligned pointer to destination image */
	mlib_d64 *dp;

/* offset of address alignment in destination */
	mlib_s32 off;

/* edge masks */
	mlib_s32 emask;

/* source data */
	mlib_d64 s11, s10;

/* source data */
	mlib_d64 s21, s20;

/* source data */
	mlib_d64 sd10, sd20;

/* source data */
	mlib_d64 sd11, sd21;

/* source data */
	mlib_d64 sd12, sd22;

/* destination data */
	mlib_d64 dd;

/* intermediate result */
	mlib_d64 rd1h, rd2h, rdh;

/* intermediate result */
	mlib_d64 rd1l, rd2l, rdl;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);

/* alpha data */
	mlib_d64 a[10], a1, a2;

/* (1-alpha) data */
	mlib_d64 b0, b1, b2;

/* loop variable */
	mlib_s32 i, j, n;

/* prepare the alpha and (1 - alpha) values */
	a[1] = a[4] = a[7] = a0;
	vis_alignaddr((void *)0, 2);
	a2 = vis_faligndata(a0, a0);
	a[2] = a[5] = a[8] = a1 = vis_faligndata(a0, a2);
	vis_alignaddr((void *)0, 4);
	a[0] = a[3] = a[6] = a[9] = vis_faligndata(a0, a1);

	sl1 = sa1 = src1;
	sl2 = sa2 = src2;
	da = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)da & (~7));
		off = (mlib_addr)dp - (mlib_addr)da;
		dend = da + xsize - 1;

/* prepare alpha and (1-alpha) */
		a0 = a[7 + off];
		b0 = vis_fpsub16(ones, a0);
		a1 = a[8 + off];
		b1 = vis_fpsub16(ones, a1);
		a2 = a[9 + off];
		b2 = vis_fpsub16(ones, a2);

/* generate edge mask for the start point */
		emask = vis_edge8(da, dend);

		sp1 = (mlib_d64 *)vis_alignaddr(sa1, off);
		s11 = vis_ld_d64_nf(sp1);
		sp2 = (mlib_d64 *)vis_alignaddr(sa2, off);
		s21 = vis_ld_d64_nf(sp2);

/* first 8-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s20 = s21;
			s21 = vis_ld_d64_nf(sp2 + 1);
			sd20 = vis_faligndata(s20, s21);
			sp2++;
			vis_alignaddr(sa1, off);
			s10 = s11;
			s11 = vis_ld_d64_nf(sp1 + 1);
			sd10 = vis_faligndata(s10, s11);
			sp1++;
			MLIB_V_IMAGESCALARBLEND_U8(sd10, sd20, a0, b0, a1, b1);
			vis_pst_8(dd, dp, emask);
			dp++;
			vis_alignaddr(sa2, off);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s20 = s21;
				s21 = vis_ld_d64_nf(sp2 + 1);
				sd20 = vis_faligndata(s20, s21);
				sp2++;
				vis_alignaddr(sa1, off);
				s10 = s11;
				s11 = vis_ld_d64_nf(sp1 + 1);
				sd10 = vis_faligndata(s10, s11);
				sp1++;
				MLIB_V_IMAGESCALARBLEND_U8(sd10, sd20, a2, b2,
				    a0, b0);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
				dp++;
				vis_alignaddr(sa2, off);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					s20 = s21;
					s21 = vis_ld_d64_nf(sp2 + 1);
					sd20 = vis_faligndata(s20, s21);
					sp2++;
					vis_alignaddr(sa1, off);
					s10 = s11;
					s11 = vis_ld_d64_nf(sp1 + 1);
					sd10 = vis_faligndata(s10, s11);
					sp1++;
					MLIB_V_IMAGESCALARBLEND_U8(sd10, sd20,
					    a1, b1, a2, b2);
					emask = vis_edge8(dp, dend);
					vis_pst_8(dd, dp, emask);
					dp++;
					vis_alignaddr(sa2, off);
				}
			}
		}

/* 8-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 1) / 24;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s20 = s21;
			s21 = sp2[1];
			sd20 = vis_faligndata(s20, s21);
			s20 = sp2[2];
			sd21 = vis_faligndata(s21, s20);
			s21 = vis_ld_d64_nf(sp2 + 3);
			sd22 = vis_faligndata(s20, s21);
			sp2 += 3;

			vis_alignaddr(sa1, off);
			s10 = s11;
			s11 = sp1[1];
			sd10 = vis_faligndata(s10, s11);
			s10 = sp1[2];
			sd11 = vis_faligndata(s11, s10);
			s11 = vis_ld_d64_nf(sp1 + 3);
			sd12 = vis_faligndata(s10, s11);
			sp1 += 3;

			MLIB_V_IMAGESCALARBLEND_U8(sd10, sd20, a0, b0, a1, b1);
			(*dp++) = dd;
			MLIB_V_IMAGESCALARBLEND_U8(sd11, sd21, a2, b2, a0, b0);
			(*dp++) = dd;
			MLIB_V_IMAGESCALARBLEND_U8(sd12, sd22, a1, b1, a2, b2);
			(*dp++) = dd;
			vis_alignaddr(sa2, off);
		}

/* last 8-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s20 = s21;
			s21 = vis_ld_d64_nf(sp2 + 1);
			sd20 = vis_faligndata(s20, s21);
			vis_alignaddr(sa1, off);
			s10 = s11;
			s11 = vis_ld_d64_nf(sp1 + 1);
			sd10 = vis_faligndata(s10, s11);
			MLIB_V_IMAGESCALARBLEND_U8(sd10, sd20, a0, b0, a1, b1);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s10 = s11;
				s11 = vis_ld_d64_nf(sp1 + 2);
				sd10 = vis_faligndata(s10, s11);
				vis_alignaddr(sa2, off);
				s20 = s21;
				s21 = vis_ld_d64_nf(sp2 + 2);
				sd20 = vis_faligndata(s20, s21);
				MLIB_V_IMAGESCALARBLEND_U8(sd10, sd20, a2, b2,
				    a0, b0);
				emask = vis_edge8((mlib_u8 *)dp, dend);
				vis_pst_8(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					sd20 = vis_faligndata(s21,
						vis_ld_d64_nf(sp2 + 3));
					vis_alignaddr(sa1, off);
					sd10 = vis_faligndata(s11,
						vis_ld_d64_nf(sp1 + 3));
					MLIB_V_IMAGESCALARBLEND_U8(sd10, sd20,
					    a1, b1, a2, b2);
					emask = vis_edge8((mlib_u8 *)dp, dend);
					vis_pst_8(dd, dp, emask);
				}
			}
		}

		sl1 = sa1 = sl1 + slb1;
		sl2 = sa2 = sl2 + slb2;
		da += dlb;
	}
}

/* *********************************************************** */

#define	MLIB_V_IMAGESCALARBLEND_S16(sd1, sd2, ad, bd)           \
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
mlib_v_ImageScalarBlend_S16_124_A8D1X4(
    mlib_s16 *src1,
    mlib_s16 *src2,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_d64 ad)
{
	mlib_d64 *sp1, *sp2;

/* aligned dst address */
	mlib_d64 *dp;
	mlib_d64 sd1, sd2;

/* destination data */
	mlib_d64 dd;

/* intermediate result */
	mlib_d64 rd1h, rd2h, rd1;

/* intermediate result */
	mlib_d64 rd1l, rd2l, rd2;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_d64 bd;

/* loop variables */
	mlib_s32 j;

	bd = vis_fpsub16(ones, ad);

	sp1 = (mlib_d64 *)src1;
	sp2 = (mlib_d64 *)src2;
	dp = (mlib_d64 *)dst;

/* 4-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 4); j += 4) {
		sd1 = (*sp1++);
		sd2 = (*sp2++);
		MLIB_V_IMAGESCALARBLEND_S16(sd1, sd2, ad, bd);
		(*dp++) = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageScalarBlend_S16_124_A8D2(
    mlib_s16 *src1,
    mlib_s32 slb1,
    mlib_s16 *src2,
    mlib_s32 slb2,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad)
{
	mlib_d64 *sp1, *sl1;
	mlib_d64 *sp2, *sl2;

/* aligned dst address */
	mlib_d64 *dp, *dl;
	mlib_d64 sd1, sd2;

/* destination data */
	mlib_d64 dd;

/* intermediate result */
	mlib_d64 rd1h, rd2h, rd1;

/* intermediate result */
	mlib_d64 rd1l, rd2l, rd2;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_d64 bd;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;

	bd = vis_fpsub16(ones, ad);

	sl1 = sp1 = (mlib_d64 *)src1;
	sl2 = sp2 = (mlib_d64 *)src2;
	dl = dp = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 4); i += 4) {
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_V_IMAGESCALARBLEND_S16(sd1, sd2, ad, bd);
			(*dp++) = dd;
		}

/* last 4-pixel column */

		if (i < xsize) {
			sd1 = *sp1;
			sd2 = *sp2;
			MLIB_V_IMAGESCALARBLEND_S16(sd1, sd2, ad, bd);
			emask =
			    vis_edge16((void *)0,
			    (void *)(2 * (xsize - i - 1)));
			vis_pst_16(dd, dp, emask);
		}

		sl1 = sp1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
		sl2 = sp2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
		dl = dp = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageScalarBlend_S16_124(
    mlib_s16 *src1,
    mlib_s32 slb1,
    mlib_s16 *src2,
    mlib_s32 slb2,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad0)
{
/* pointers to a line in source images */
	mlib_s16 *sl1, *sl2;

/* pointers to source images */
	mlib_s16 *sa1, *sa2;

/* 8-byte aligned pointers to source images */
	mlib_d64 *sp1, *sp2;

/* pointers to destination */
	mlib_s16 *da;

/* pointer to end of a line in destination */
	mlib_s16 *dend;

/* 8-byte aligned pointer to destination */
	mlib_d64 *dp;

/* offset of address alignment in destination */
	mlib_s32 off;

/* edge masks */
	mlib_s32 emask;

/* source data */
	mlib_d64 s11, s10, s12;

/* source data */
	mlib_d64 s21, s20, s22;

/* source data */
	mlib_d64 sd10, sd20;

/* source data */
	mlib_d64 sd11, sd21;

/* destination data */
	mlib_d64 dd;

/* intermediate result */
	mlib_d64 rd1h, rd2h, rd1;

/* intermediate result */
	mlib_d64 rd1l, rd2l, rd2;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);

/* alpha and (1-alpha) data */
	mlib_d64 ad, bd;
	mlib_s32 i, j, n;

	sl1 = sa1 = src1;
	sl2 = sa2 = src2;
	da = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)da & (~7));
		off = (mlib_addr)dp - (mlib_addr)da;
		dend = da + xsize - 1;

/* prepare the alpha and (1 - alpha) value */
		vis_alignaddr(dp, off);
		ad = vis_faligndata(ad0, ad0);
		bd = vis_fpsub16(ones, ad);

/* generate edge mask for the start point */
		emask = vis_edge16(da, dend);

		sp1 = (mlib_d64 *)vis_alignaddr(sa1, off);
		s11 = vis_ld_d64_nf(sp1);
		sp2 = (mlib_d64 *)vis_alignaddr(sa2, off);
		s21 = vis_ld_d64_nf(sp2);

/* first 4-pixel column loop */

		if (emask != 0xf) {
			s20 = s21;
			s21 = vis_ld_d64_nf(sp2 + 1);
			sd20 = vis_faligndata(s20, s21);
			sp2++;

			vis_alignaddr(sa1, off);
			s10 = s11;
			s11 = vis_ld_d64_nf(sp1 + 1);
			sd10 = vis_faligndata(s10, s11);
			sp1++;

			MLIB_V_IMAGESCALARBLEND_S16(sd10, sd20, ad, bd);
			vis_pst_16(dd, dp, emask);
			dp++;
			vis_alignaddr(sa2, off);
		}

/* 8-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 2) >> 4;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s20 = s21;
			s22 = sp2[1];
			sd20 = vis_faligndata(s20, s22);
			s21 = vis_ld_d64_nf(sp2 + 2);
			sd21 = vis_faligndata(s22, s21);

			vis_alignaddr(sa1, off);
			s10 = s11;
			s12 = sp1[1];
			sd10 = vis_faligndata(s10, s12);
			s11 = vis_ld_d64_nf(sp1 + 2);
			sd11 = vis_faligndata(s12, s11);

			MLIB_V_IMAGESCALARBLEND_S16(sd10, sd20, ad, bd);
			(*dp++) = dd;
			MLIB_V_IMAGESCALARBLEND_S16(sd11, sd21, ad, bd);
			(*dp++) = dd;
			sp1 += 2;
			sp2 += 2;
			vis_alignaddr(sa2, off);
		}

/* 4-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 2) >> 3;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s20 = s21;
			s21 = vis_ld_d64_nf(sp2 + 1);
			sd20 = vis_faligndata(s20, s21);

			vis_alignaddr(sa1, off);
			s10 = s11;
			s11 = vis_ld_d64_nf(sp1 + 1);
			sd10 = vis_faligndata(s10, s11);

			vis_alignaddr(sa2, off);
			MLIB_V_IMAGESCALARBLEND_S16(sd10, sd20, ad, bd);
			(*dp++) = dd;
			sp1++;
			sp2++;
		}

/* last 4-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd20 = vis_faligndata(s21, vis_ld_d64_nf(sp2 + 1));
			vis_alignaddr(sa1, off);
			sd10 = vis_faligndata(s11, vis_ld_d64_nf(sp1 + 1));

			MLIB_V_IMAGESCALARBLEND_S16(sd10, sd20, ad, bd);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
		}

		sl1 = sa1 = (mlib_s16 *)((mlib_u8 *)sl1 + slb1);
		sl2 = sa2 = (mlib_s16 *)((mlib_u8 *)sl2 + slb2);
		da = (mlib_s16 *)((mlib_u8 *)da + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageScalarBlend_S16_3_A8D1(
    mlib_s16 *src1,
    mlib_s16 *src2,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_d64 a0)
{
	mlib_d64 *sp1, *sp2;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to end of dst */
	mlib_s16 *dend;
	mlib_d64 sd1, sd2;

/* destination data */
	mlib_d64 dd;

/* intermediate result */
	mlib_d64 rd1h, rd2h, rd1;

/* intermediate result */
	mlib_d64 rd1l, rd2l, rd2;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_d64 a1, a2, b0, b1, b2;

/* edge mask */
	mlib_s32 emask;

/* loop variable */
	mlib_s32 j;

/* prepare the alpha and (1 - alpha) values */
	vis_alignaddr(dst, 2);
	a2 = vis_faligndata(a0, a0);
	a1 = vis_faligndata(a0, a2);
	vis_alignaddr(dst, 4);
	a2 = vis_faligndata(a0, a1);
	b0 = vis_fpsub16(ones, a0);
	b1 = vis_fpsub16(ones, a1);
	b2 = vis_fpsub16(ones, a2);

	sp1 = (mlib_d64 *)src1;
	sp2 = (mlib_d64 *)src2;
	dp = (mlib_d64 *)dst;

	dend = (mlib_s16 *)dp + dsize - 1;

/* 4-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 12); j += 12) {
		sd1 = (*sp1++);
		sd2 = (*sp2++);
		MLIB_V_IMAGESCALARBLEND_S16(sd1, sd2, a0, b0);
		(*dp++) = dd;

		sd1 = (*sp1++);
		sd2 = (*sp2++);
		MLIB_V_IMAGESCALARBLEND_S16(sd1, sd2, a1, b1);
		(*dp++) = dd;

		sd1 = (*sp1++);
		sd2 = (*sp2++);
		MLIB_V_IMAGESCALARBLEND_S16(sd1, sd2, a2, b2);
		(*dp++) = dd;
	}

/* last 4-pixel */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		sd1 = sp1[0];
		sd2 = sp2[0];
		MLIB_V_IMAGESCALARBLEND_S16(sd1, sd2, a0, b0);
		emask = vis_edge16(dp, dend);
		vis_pst_16(dd, dp, emask);
		dp++;

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd1 = sp1[1];
			sd2 = sp2[1];
			MLIB_V_IMAGESCALARBLEND_S16(sd1, sd2, a1, b1);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd1 = sp1[2];
				sd2 = sp2[2];
				MLIB_V_IMAGESCALARBLEND_S16(sd1, sd2, a2, b2);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageScalarBlend_S16_3_A8D2(
    mlib_s16 *src1,
    mlib_s32 slb1,
    mlib_s16 *src2,
    mlib_s32 slb2,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 a0)
{
	mlib_d64 *sp1, *sl1;
	mlib_d64 *sp2, *sl2;

/* aligned dst address */
	mlib_d64 *dp, *dl;

/* pointer to end of dst */
	mlib_s16 *dend;
	mlib_d64 sd1, sd2;

/* destination data */
	mlib_d64 dd;

/* intermediate result */
	mlib_d64 rd1h, rd2h, rd1;

/* intermediate result */
	mlib_d64 rd1l, rd2l, rd2;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_d64 a1, a2, b0, b1, b2;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;

/* prepare the alpha and (1 - alpha) values */
	vis_alignaddr(dst, 2);
	a2 = vis_faligndata(a0, a0);
	a1 = vis_faligndata(a0, a2);
	vis_alignaddr(dst, 4);
	a2 = vis_faligndata(a0, a1);
	b0 = vis_fpsub16(ones, a0);
	b1 = vis_fpsub16(ones, a1);
	b2 = vis_fpsub16(ones, a2);

	sl1 = sp1 = (mlib_d64 *)src1;
	sl2 = sp2 = (mlib_d64 *)src2;
	dl = dp = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

		dend = (mlib_s16 *)dp + xsize - 1;

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 12); i += 12) {
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_V_IMAGESCALARBLEND_S16(sd1, sd2, a0, b0);
			(*dp++) = dd;

			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_V_IMAGESCALARBLEND_S16(sd1, sd2, a1, b1);
			(*dp++) = dd;

			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_V_IMAGESCALARBLEND_S16(sd1, sd2, a2, b2);
			(*dp++) = dd;
		}

/* lsat 4-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd1 = sp1[0];
			sd2 = sp2[0];
			MLIB_V_IMAGESCALARBLEND_S16(sd1, sd2, a0, b0);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd1 = sp1[1];
				sd2 = sp2[1];
				MLIB_V_IMAGESCALARBLEND_S16(sd1, sd2, a1, b1);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					sd1 = sp1[2];
					sd2 = sp2[2];
					MLIB_V_IMAGESCALARBLEND_S16(sd1, sd2,
					    a2, b2);
					emask = vis_edge16(dp, dend);
					vis_pst_16(dd, dp, emask);
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
mlib_v_ImageScalarBlend_S16_3(
    mlib_s16 *src1,
    mlib_s32 slb1,
    mlib_s16 *src2,
    mlib_s32 slb2,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 a0)
{
/* pointers to a line in source images */
	mlib_s16 *sl1, *sl2;

/* pointers to source images */
	mlib_s16 *sa1, *sa2;

/* 8-byte aligned pointers to source images */
	mlib_d64 *sp1, *sp2;

/* pointers to destination */
	mlib_s16 *da;

/* pointer to end of a line in destination */
	mlib_s16 *dend;

/* 8-byte aligned pointer to destination */
	mlib_d64 *dp;

/* source data */
	mlib_d64 s11, s10;

/* source data */
	mlib_d64 s21, s20;

/* source data */
	mlib_d64 sd10, sd20;

/* source data */
	mlib_d64 sd11, sd21;

/* source data */
	mlib_d64 sd12, sd22;

/* destination data */
	mlib_d64 dd;

/* intermediate result */
	mlib_d64 rd1h, rd2h, rd1;

/* intermediate result */
	mlib_d64 rd1l, rd2l, rd2;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);

/* alpha data */
	mlib_d64 a[6], a1, a2;

/* (1-alpha) data */
	mlib_d64 b0, b1, b2;

/* edge masks */
	mlib_s32 emask;

/* offset of address in dst */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;

/* prepare the alpha and (1 - alpha) values */
	a[0] = a[3] = a0;
	vis_alignaddr((void *)0, 2);
	a2 = vis_faligndata(a0, a0);
	a[1] = a[4] = a1 = vis_faligndata(a0, a2);
	vis_alignaddr((void *)0, 4);
	a[2] = a[5] = vis_faligndata(a0, a1);

	sl1 = sa1 = src1;
	sl2 = sa2 = src2;
	da = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)da & (~7));
		off = (mlib_addr)dp - (mlib_addr)da;
		dend = da + xsize - 1;

/* prepare alpha and (1-alpha) */
		a0 = a[3 + (off >> 1)];
		b0 = vis_fpsub16(ones, a0);
		a1 = a[4 + (off >> 1)];
		b1 = vis_fpsub16(ones, a1);
		a2 = a[5 + (off >> 1)];
		b2 = vis_fpsub16(ones, a2);

/* generate edge mask for the start point */
		emask = vis_edge16(da, dend);
		sp2 = (mlib_d64 *)vis_alignaddr(sa2, off);
		s21 = vis_ld_d64_nf(sp2);
		sp1 = (mlib_d64 *)vis_alignaddr(sa1, off);
		s11 = vis_ld_d64_nf(sp1);

/* first 4-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s10 = s11;
			s11 = vis_ld_d64_nf(sp1 + 1);
			sd10 = vis_faligndata(s10, s11);
			sp1++;
			vis_alignaddr(sa2, off);
			s20 = s21;
			s21 = vis_ld_d64_nf(sp2 + 1);
			sd20 = vis_faligndata(s20, s21);
			sp2++;
			MLIB_V_IMAGESCALARBLEND_S16(sd10, sd20, a0, b0);
			vis_pst_16(dd, dp, emask);
			dp++;
			vis_alignaddr(sa1, off);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s10 = s11;
				s11 = vis_ld_d64_nf(sp1 + 1);
				sd10 = vis_faligndata(s10, s11);
				sp1++;
				vis_alignaddr(sa2, off);
				s20 = s21;
				s21 = vis_ld_d64_nf(sp2 + 1);
				sd20 = vis_faligndata(s20, s21);
				sp2++;
				MLIB_V_IMAGESCALARBLEND_S16(sd10, sd20, a1, b1);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
				dp++;
				vis_alignaddr(sa1, off);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					s10 = s11;
					s11 = vis_ld_d64_nf(sp1 + 1);
					sd10 = vis_faligndata(s10, s11);
					sp1++;
					vis_alignaddr(sa2, off);
					s20 = s21;
					s21 = vis_ld_d64_nf(sp2 + 1);
					sd20 = vis_faligndata(s20, s21);
					sp2++;
					MLIB_V_IMAGESCALARBLEND_S16(sd10, sd20,
					    a2, b2);
					emask = vis_edge16(dp, dend);
					vis_pst_16(dd, dp, emask);
					dp++;
					vis_alignaddr(sa1, off);
				}
			}
		}

/* 4-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 2) / 24;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s10 = s11;
			s11 = sp1[1];
			sd10 = vis_faligndata(s10, s11);
			s10 = sp1[2];
			sd11 = vis_faligndata(s11, s10);
			s11 = vis_ld_d64_nf(sp1 + 3);
			sd12 = vis_faligndata(s10, s11);
			sp1 += 3;

			vis_alignaddr(sa2, off);
			s20 = s21;
			s21 = sp2[1];
			sd20 = vis_faligndata(s20, s21);
			s20 = sp2[2];
			sd21 = vis_faligndata(s21, s20);
			s21 = vis_ld_d64_nf(sp2 + 3);
			sd22 = vis_faligndata(s20, s21);
			sp2 += 3;

			MLIB_V_IMAGESCALARBLEND_S16(sd10, sd20, a0, b0);
			(*dp++) = dd;
			MLIB_V_IMAGESCALARBLEND_S16(sd11, sd21, a1, b1);
			(*dp++) = dd;
			MLIB_V_IMAGESCALARBLEND_S16(sd12, sd22, a2, b2);
			(*dp++) = dd;
			vis_alignaddr(sa1, off);
		}

/* last 4-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s10 = s11;
			s11 = vis_ld_d64_nf(sp1 + 1);
			sd10 = vis_faligndata(s10, s11);
			vis_alignaddr(sa2, off);
			s20 = s21;
			s21 = vis_ld_d64_nf(sp2 + 1);
			sd20 = vis_faligndata(s20, s21);
			MLIB_V_IMAGESCALARBLEND_S16(sd10, sd20, a0, b0);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s20 = s21;
				s21 = vis_ld_d64_nf(sp2 + 2);
				sd20 = vis_faligndata(s20, s21);
				vis_alignaddr(sa1, off);
				s10 = s11;
				s11 = vis_ld_d64_nf(sp1 + 2);
				sd10 = vis_faligndata(s10, s11);
				MLIB_V_IMAGESCALARBLEND_S16(sd10, sd20, a1, b1);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					sd10 = vis_faligndata(s11,
						vis_ld_d64_nf(sp1 + 3));
					vis_alignaddr(sa2, off);
					sd20 = vis_faligndata(s21,
						vis_ld_d64_nf(sp2 + 3));
					MLIB_V_IMAGESCALARBLEND_S16(sd10, sd20,
					    a2, b2);
					emask = vis_edge16(dp, dend);
					vis_pst_16(dd, dp, emask);
				}
			}
		}

		sl1 = sa1 = (mlib_s16 *)((mlib_u8 *)sl1 + slb1);
		sl2 = sa2 = (mlib_s16 *)((mlib_u8 *)sl2 + slb2);
		da = (mlib_s16 *)((mlib_u8 *)da + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageScalarBlend_S32_1(
    mlib_s32 *sa1,
    mlib_s32 slb1,
    mlib_s32 *sa2,
    mlib_s32 slb2,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 *alpha)
{
/* pointer for line of source */
	mlib_s32 *sl1 = sa1;

/* pointer for line of source */
	mlib_s32 *sl2 = sa2;

/* pointer for line of destination */
	mlib_s32 *dl = da;

/* indices for x and y */
	mlib_s32 i, j;
	mlib_d64 a0 = -(alpha[0] & MASK) / (mlib_d64)MLIB_S32_MIN;

	for (j = 0; j < ysize; j++) {
		for (i = 0; i < xsize; i++) {
#pragma pipeloop(0)
			dl[i] = (mlib_s32)(a0 * (mlib_d64)sl1[i] +
			    (1 - a0) * (mlib_d64)sl2[i]);
		}

		sl1 = (mlib_s32 *)((unsigned char *)sl1 + slb1);
		sl2 = (mlib_s32 *)((unsigned char *)sl2 + slb2);
		dl = (mlib_s32 *)((unsigned char *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageScalarBlend_S32_2(
    mlib_s32 *sa1,
    mlib_s32 slb1,
    mlib_s32 *sa2,
    mlib_s32 slb2,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 *alpha)
{
/* pointer for line of source */
	mlib_s32 *sl1 = sa1;

/* pointer for line of source */
	mlib_s32 *sl2 = sa2;

/* pointer for line of destination */
	mlib_s32 *dl = da;

/* indices for x and y */
	mlib_s32 i, j;
	mlib_d64 a0 = -(alpha[0] & MASK) / (mlib_d64)MLIB_S32_MIN;
	mlib_d64 a1 = -(alpha[1] & MASK) / (mlib_d64)MLIB_S32_MIN;

	for (j = 0; j < ysize; j++) {
		for (i = 0; i < xsize; i++) {
#pragma pipeloop(0)
			dl[2 * i] = (mlib_s32)(a0 * (mlib_d64)sl1[2 * i] +
			    (1 - a0) * (mlib_d64)sl2[2 * i]);
			dl[2 * i + 1] =
			    (mlib_s32)(a1 * (mlib_d64)sl1[2 * i + 1] + (1 -
			    a1) * (mlib_d64)sl2[2 * i + 1]);
		}

		sl1 = (mlib_s32 *)((unsigned char *)sl1 + slb1);
		sl2 = (mlib_s32 *)((unsigned char *)sl2 + slb2);
		dl = (mlib_s32 *)((unsigned char *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageScalarBlend_S32_3(
    mlib_s32 *sa1,
    mlib_s32 slb1,
    mlib_s32 *sa2,
    mlib_s32 slb2,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 *alpha)
{
/* pointer for line of source */
	mlib_s32 *sl1 = sa1;

/* pointer for line of source */
	mlib_s32 *sl2 = sa2;

/* pointer for line of destination */
	mlib_s32 *dl = da;

/* indices for x and y */
	mlib_s32 i, j;
	mlib_d64 a0 = -(alpha[0] & MASK) / (mlib_d64)MLIB_S32_MIN;
	mlib_d64 a1 = -(alpha[1] & MASK) / (mlib_d64)MLIB_S32_MIN;
	mlib_d64 a2 = -(alpha[2] & MASK) / (mlib_d64)MLIB_S32_MIN;

	for (j = 0; j < ysize; j++) {
		for (i = 0; i < xsize; i++) {
#pragma pipeloop(0)
			dl[3 * i] = (mlib_s32)(a0 * (mlib_d64)sl1[3 * i] +
			    (1 - a0) * (mlib_d64)sl2[3 * i]);
			dl[3 * i + 1] =
			    (mlib_s32)(a1 * (mlib_d64)sl1[3 * i + 1] + (1 -
			    a1) * (mlib_d64)sl2[3 * i + 1]);
			dl[3 * i + 2] =
			    (mlib_s32)(a2 * (mlib_d64)sl1[3 * i + 2] + (1 -
			    a2) * (mlib_d64)sl2[3 * i + 2]);
		}

		sl1 = (mlib_s32 *)((unsigned char *)sl1 + slb1);
		sl2 = (mlib_s32 *)((unsigned char *)sl2 + slb2);
		dl = (mlib_s32 *)((unsigned char *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageScalarBlend_S32_4(
    mlib_s32 *sa1,
    mlib_s32 slb1,
    mlib_s32 *sa2,
    mlib_s32 slb2,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 *alpha)
{
/* pointer for line of source */
	mlib_s32 *sl1 = sa1;

/* pointer for line of source */
	mlib_s32 *sl2 = sa2;

/* pointer for line of destination */
	mlib_s32 *dl = da;

/* indices for x and y */
	mlib_s32 i, j;
	mlib_d64 a0 = -(alpha[0] & MASK) / (mlib_d64)MLIB_S32_MIN;
	mlib_d64 a1 = -(alpha[1] & MASK) / (mlib_d64)MLIB_S32_MIN;
	mlib_d64 a2 = -(alpha[2] & MASK) / (mlib_d64)MLIB_S32_MIN;
	mlib_d64 a3 = -(alpha[3] & MASK) / (mlib_d64)MLIB_S32_MIN;

	for (j = 0; j < ysize; j++) {
		for (i = 0; i < xsize; i++) {
#pragma pipeloop(0)
			dl[4 * i] = (mlib_s32)(a0 * (mlib_d64)sl1[4 * i] +
			    (1 - a0) * (mlib_d64)sl2[4 * i]);
			dl[4 * i + 1] =
			    (mlib_s32)(a1 * (mlib_d64)sl1[4 * i + 1] + (1 -
			    a1) * (mlib_d64)sl2[4 * i + 1]);
			dl[4 * i + 2] =
			    (mlib_s32)(a2 * (mlib_d64)sl1[4 * i + 2] + (1 -
			    a2) * (mlib_d64)sl2[4 * i + 2]);
			dl[4 * i + 3] =
			    (mlib_s32)(a3 * (mlib_d64)sl1[4 * i + 3] + (1 -
			    a3) * (mlib_d64)sl2[4 * i + 3]);
		}

		sl1 = (mlib_s32 *)((unsigned char *)sl1 + slb1);
		sl2 = (mlib_s32 *)((unsigned char *)sl2 + slb2);
		dl = (mlib_s32 *)((unsigned char *)dl + dlb);
	}
}

/* *********************************************************** */

#define	MLIB_V_IMAGESCALARBLEND_U16(sd1, sd2, ad, bd)           \
	sd1 = vis_fxnor(ones, sd1);                             \
	sd2 = vis_fxnor(ones, sd2);                             \
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
mlib_v_ImageScalarBlend_U16_124_A8D1X4(
    mlib_u16 *src1,
    mlib_u16 *src2,
    mlib_u16 *dst,
    mlib_s32 dsize,
    mlib_d64 ad)
{
	mlib_d64 *sp1, *sp2;

/* aligned dst address */
	mlib_d64 *dp;
	mlib_d64 sd1, sd2;

/* destination data */
	mlib_d64 dd;

/* intermediate result */
	mlib_d64 rd1h, rd2h, rd1;

/* intermediate result */
	mlib_d64 rd1l, rd2l, rd2;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_d64 bd;

/* loop variables */
	mlib_s32 j;

	bd = vis_fpsub16(ones, ad);

	sp1 = (mlib_d64 *)src1;
	sp2 = (mlib_d64 *)src2;
	dp = (mlib_d64 *)dst;

/* 4-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 4); j += 4) {
		sd1 = (*sp1++);
		sd2 = (*sp2++);
		MLIB_V_IMAGESCALARBLEND_U16(sd1, sd2, ad, bd);
		(*dp++) = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageScalarBlend_U16_124_A8D2(
    mlib_u16 *src1,
    mlib_s32 slb1,
    mlib_u16 *src2,
    mlib_s32 slb2,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad)
{
	mlib_d64 *sp1, *sl1;
	mlib_d64 *sp2, *sl2;

/* aligned dst address */
	mlib_d64 *dp, *dl;
	mlib_d64 sd1, sd2;

/* destination data */
	mlib_d64 dd;

/* intermediate result */
	mlib_d64 rd1h, rd2h, rd1;

/* intermediate result */
	mlib_d64 rd1l, rd2l, rd2;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_d64 bd;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;

	bd = vis_fpsub16(ones, ad);

	sl1 = sp1 = (mlib_d64 *)src1;
	sl2 = sp2 = (mlib_d64 *)src2;
	dl = dp = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 4); i += 4) {
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_V_IMAGESCALARBLEND_U16(sd1, sd2, ad, bd);
			(*dp++) = dd;
		}

/* last 4-pixel column */

		if (i < xsize) {
			sd1 = *sp1;
			sd2 = *sp2;
			MLIB_V_IMAGESCALARBLEND_U16(sd1, sd2, ad, bd);
			emask =
			    vis_edge16((void *)0,
			    (void *)(2 * (xsize - i - 1)));
			vis_pst_16(dd, dp, emask);
		}

		sl1 = sp1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
		sl2 = sp2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
		dl = dp = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageScalarBlend_U16_124(
    mlib_u16 *src1,
    mlib_s32 slb1,
    mlib_u16 *src2,
    mlib_s32 slb2,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad0)
{
/* pointers to a line in source images */
	mlib_u16 *sl1, *sl2;

/* pointers to source images */
	mlib_u16 *sa1, *sa2;

/* 8-byte aligned pointers to source images */
	mlib_d64 *sp1, *sp2;

/* pointers to destination */
	mlib_u16 *da;

/* pointer to end of a line in destination */
	mlib_u16 *dend;

/* 8-byte aligned pointer to destination */
	mlib_d64 *dp;

/* offset of address alignment in destination */
	mlib_s32 off;

/* edge masks */
	mlib_s32 emask;

/* source data */
	mlib_d64 s11, s10, s12;

/* source data */
	mlib_d64 s21, s20, s22;

/* source data */
	mlib_d64 sd10, sd20;

/* source data */
	mlib_d64 sd11, sd21;

/* destination data */
	mlib_d64 dd;

/* intermediate result */
	mlib_d64 rd1h, rd2h, rd1;

/* intermediate result */
	mlib_d64 rd1l, rd2l, rd2;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);

/* alpha and (1-alpha) data */
	mlib_d64 ad, bd;
	mlib_s32 i, j, n;

	sl1 = sa1 = src1;
	sl2 = sa2 = src2;
	da = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)da & (~7));
		off = (mlib_addr)dp - (mlib_addr)da;
		dend = da + xsize - 1;

/* prepare the alpha and (1 - alpha) value */
		vis_alignaddr(dp, off);
		ad = vis_faligndata(ad0, ad0);
		bd = vis_fpsub16(ones, ad);

/* generate edge mask for the start point */
		emask = vis_edge16(da, dend);

		sp1 = (mlib_d64 *)vis_alignaddr(sa1, off);
		s11 = vis_ld_d64_nf(sp1);
		sp2 = (mlib_d64 *)vis_alignaddr(sa2, off);
		s21 = vis_ld_d64_nf(sp2);

/* first 4-pixel column loop */

		if (emask != 0xf) {
			s20 = s21;
			s21 = vis_ld_d64_nf(sp2 + 1);
			sd20 = vis_faligndata(s20, s21);
			sp2++;

			vis_alignaddr(sa1, off);
			s10 = s11;
			s11 = vis_ld_d64_nf(sp1 + 1);
			sd10 = vis_faligndata(s10, s11);
			sp1++;

			MLIB_V_IMAGESCALARBLEND_U16(sd10, sd20, ad, bd);
			vis_pst_16(dd, dp, emask);
			dp++;
			vis_alignaddr(sa2, off);
		}

/* 8-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 2) >> 4;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s20 = s21;
			s22 = sp2[1];
			sd20 = vis_faligndata(s20, s22);
			s21 = vis_ld_d64_nf(sp2 + 2);
			sd21 = vis_faligndata(s22, s21);

			vis_alignaddr(sa1, off);
			s10 = s11;
			s12 = sp1[1];
			sd10 = vis_faligndata(s10, s12);
			s11 = vis_ld_d64_nf(sp1 + 2);
			sd11 = vis_faligndata(s12, s11);

			MLIB_V_IMAGESCALARBLEND_U16(sd10, sd20, ad, bd);
			(*dp++) = dd;
			MLIB_V_IMAGESCALARBLEND_U16(sd11, sd21, ad, bd);
			(*dp++) = dd;
			sp1 += 2;
			sp2 += 2;
			vis_alignaddr(sa2, off);
		}

/* 4-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 2) >> 3;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s20 = s21;
			s21 = vis_ld_d64_nf(sp2 + 1);
			sd20 = vis_faligndata(s20, s21);

			vis_alignaddr(sa1, off);
			s10 = s11;
			s11 = vis_ld_d64_nf(sp1 + 1);
			sd10 = vis_faligndata(s10, s11);

			vis_alignaddr(sa2, off);
			MLIB_V_IMAGESCALARBLEND_U16(sd10, sd20, ad, bd);
			(*dp++) = dd;
			sp1++;
			sp2++;
		}

/* last 4-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd20 = vis_faligndata(s21, vis_ld_d64_nf(sp2 + 1));
			vis_alignaddr(sa1, off);
			sd10 = vis_faligndata(s11, vis_ld_d64_nf(sp1 + 1));

			MLIB_V_IMAGESCALARBLEND_U16(sd10, sd20, ad, bd);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
		}

		sl1 = sa1 = (mlib_u16 *)((mlib_u8 *)sl1 + slb1);
		sl2 = sa2 = (mlib_u16 *)((mlib_u8 *)sl2 + slb2);
		da = (mlib_u16 *)((mlib_u8 *)da + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageScalarBlend_U16_3_A8D1(
    mlib_u16 *src1,
    mlib_u16 *src2,
    mlib_u16 *dst,
    mlib_s32 dsize,
    mlib_d64 a0)
{
	mlib_d64 *sp1, *sp2;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to end of dst */
	mlib_u16 *dend;
	mlib_d64 sd1, sd2;

/* destination data */
	mlib_d64 dd;

/* intermediate result */
	mlib_d64 rd1h, rd2h, rd1;

/* intermediate result */
	mlib_d64 rd1l, rd2l, rd2;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_d64 a1, a2, b0, b1, b2;

/* edge mask */
	mlib_s32 emask;

/* loop variable */
	mlib_s32 j;

/* prepare the alpha and (1 - alpha) values */
	vis_alignaddr(dst, 2);
	a2 = vis_faligndata(a0, a0);
	a1 = vis_faligndata(a0, a2);
	vis_alignaddr(dst, 4);
	a2 = vis_faligndata(a0, a1);
	b0 = vis_fpsub16(ones, a0);
	b1 = vis_fpsub16(ones, a1);
	b2 = vis_fpsub16(ones, a2);

	sp1 = (mlib_d64 *)src1;
	sp2 = (mlib_d64 *)src2;
	dp = (mlib_d64 *)dst;

	dend = (mlib_u16 *)dp + dsize - 1;

/* 4-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 12); j += 12) {
		sd1 = (*sp1++);
		sd2 = (*sp2++);
		MLIB_V_IMAGESCALARBLEND_U16(sd1, sd2, a0, b0);
		(*dp++) = dd;

		sd1 = (*sp1++);
		sd2 = (*sp2++);
		MLIB_V_IMAGESCALARBLEND_U16(sd1, sd2, a1, b1);
		(*dp++) = dd;

		sd1 = (*sp1++);
		sd2 = (*sp2++);
		MLIB_V_IMAGESCALARBLEND_U16(sd1, sd2, a2, b2);
		(*dp++) = dd;
	}

/* last 4-pixel */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		sd1 = sp1[0];
		sd2 = sp2[0];
		MLIB_V_IMAGESCALARBLEND_U16(sd1, sd2, a0, b0);
		emask = vis_edge16(dp, dend);
		vis_pst_16(dd, dp, emask);
		dp++;

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd1 = sp1[1];
			sd2 = sp2[1];
			MLIB_V_IMAGESCALARBLEND_U16(sd1, sd2, a1, b1);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd1 = sp1[2];
				sd2 = sp2[2];
				MLIB_V_IMAGESCALARBLEND_U16(sd1, sd2, a2, b2);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageScalarBlend_U16_3_A8D2(
    mlib_u16 *src1,
    mlib_s32 slb1,
    mlib_u16 *src2,
    mlib_s32 slb2,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 a0)
{
	mlib_d64 *sp1, *sl1;
	mlib_d64 *sp2, *sl2;

/* aligned dst address */
	mlib_d64 *dp, *dl;

/* pointer to end of dst */
	mlib_u16 *dend;
	mlib_d64 sd1, sd2;

/* destination data */
	mlib_d64 dd;

/* intermediate result */
	mlib_d64 rd1h, rd2h, rd1;

/* intermediate result */
	mlib_d64 rd1l, rd2l, rd2;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_d64 a1, a2, b0, b1, b2;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;

/* prepare the alpha and (1 - alpha) values */
	vis_alignaddr(dst, 2);
	a2 = vis_faligndata(a0, a0);
	a1 = vis_faligndata(a0, a2);
	vis_alignaddr(dst, 4);
	a2 = vis_faligndata(a0, a1);
	b0 = vis_fpsub16(ones, a0);
	b1 = vis_fpsub16(ones, a1);
	b2 = vis_fpsub16(ones, a2);

	sl1 = sp1 = (mlib_d64 *)src1;
	sl2 = sp2 = (mlib_d64 *)src2;
	dl = dp = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

		dend = (mlib_u16 *)dp + xsize - 1;

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 12); i += 12) {
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_V_IMAGESCALARBLEND_U16(sd1, sd2, a0, b0);
			(*dp++) = dd;

			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_V_IMAGESCALARBLEND_U16(sd1, sd2, a1, b1);
			(*dp++) = dd;

			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_V_IMAGESCALARBLEND_U16(sd1, sd2, a2, b2);
			(*dp++) = dd;
		}

/* lsat 4-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd1 = sp1[0];
			sd2 = sp2[0];
			MLIB_V_IMAGESCALARBLEND_U16(sd1, sd2, a0, b0);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd1 = sp1[1];
				sd2 = sp2[1];
				MLIB_V_IMAGESCALARBLEND_U16(sd1, sd2, a1, b1);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					sd1 = sp1[2];
					sd2 = sp2[2];
					MLIB_V_IMAGESCALARBLEND_U16(sd1, sd2,
					    a2, b2);
					emask = vis_edge16(dp, dend);
					vis_pst_16(dd, dp, emask);
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
mlib_v_ImageScalarBlend_U16_3(
    mlib_u16 *src1,
    mlib_s32 slb1,
    mlib_u16 *src2,
    mlib_s32 slb2,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 a0)
{
/* pointers to a line in source images */
	mlib_u16 *sl1, *sl2;

/* pointers to source images */
	mlib_u16 *sa1, *sa2;

/* 8-byte aligned pointers to source images */
	mlib_d64 *sp1, *sp2;

/* pointers to destination */
	mlib_u16 *da;

/* pointer to end of a line in destination */
	mlib_u16 *dend;

/* 8-byte aligned pointer to destination */
	mlib_d64 *dp;

/* source data */
	mlib_d64 s11, s10;

/* source data */
	mlib_d64 s21, s20;

/* source data */
	mlib_d64 sd10, sd20;

/* source data */
	mlib_d64 sd11, sd21;

/* source data */
	mlib_d64 sd12, sd22;

/* destination data */
	mlib_d64 dd;

/* intermediate result */
	mlib_d64 rd1h, rd2h, rd1;

/* intermediate result */
	mlib_d64 rd1l, rd2l, rd2;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);

/* alpha data */
	mlib_d64 a[6], a1, a2;

/* (1-alpha) data */
	mlib_d64 b0, b1, b2;

/* edge masks */
	mlib_s32 emask;

/* offset of address in dst */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;

/* prepare the alpha and (1 - alpha) values */
	a[0] = a[3] = a0;
	vis_alignaddr((void *)0, 2);
	a2 = vis_faligndata(a0, a0);
	a[1] = a[4] = a1 = vis_faligndata(a0, a2);
	vis_alignaddr((void *)0, 4);
	a[2] = a[5] = vis_faligndata(a0, a1);

	sl1 = sa1 = src1;
	sl2 = sa2 = src2;
	da = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)da & (~7));
		off = (mlib_addr)dp - (mlib_addr)da;
		dend = da + xsize - 1;

/* prepare alpha and (1-alpha) */
		a0 = a[3 + (off >> 1)];
		b0 = vis_fpsub16(ones, a0);
		a1 = a[4 + (off >> 1)];
		b1 = vis_fpsub16(ones, a1);
		a2 = a[5 + (off >> 1)];
		b2 = vis_fpsub16(ones, a2);

/* generate edge mask for the start point */
		emask = vis_edge16(da, dend);
		sp2 = (mlib_d64 *)vis_alignaddr(sa2, off);
		s21 = vis_ld_d64_nf(sp2);
		sp1 = (mlib_d64 *)vis_alignaddr(sa1, off);
		s11 = vis_ld_d64_nf(sp1);

/* first 4-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s10 = s11;
			s11 = vis_ld_d64_nf(sp1 + 1);
			sd10 = vis_faligndata(s10, s11);
			sp1++;
			vis_alignaddr(sa2, off);
			s20 = s21;
			s21 = vis_ld_d64_nf(sp2 + 1);
			sd20 = vis_faligndata(s20, s21);
			sp2++;
			MLIB_V_IMAGESCALARBLEND_U16(sd10, sd20, a0, b0);
			vis_pst_16(dd, dp, emask);
			dp++;
			vis_alignaddr(sa1, off);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s10 = s11;
				s11 = vis_ld_d64_nf(sp1 + 1);
				sd10 = vis_faligndata(s10, s11);
				sp1++;
				vis_alignaddr(sa2, off);
				s20 = s21;
				s21 = vis_ld_d64_nf(sp2 + 1);
				sd20 = vis_faligndata(s20, s21);
				sp2++;
				MLIB_V_IMAGESCALARBLEND_U16(sd10, sd20, a1, b1);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
				dp++;
				vis_alignaddr(sa1, off);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					s10 = s11;
					s11 = vis_ld_d64_nf(sp1 + 1);
					sd10 = vis_faligndata(s10, s11);
					sp1++;
					vis_alignaddr(sa2, off);
					s20 = s21;
					s21 = vis_ld_d64_nf(sp2 + 1);
					sd20 = vis_faligndata(s20, s21);
					sp2++;
					MLIB_V_IMAGESCALARBLEND_U16(sd10, sd20,
					    a2, b2);
					emask = vis_edge16(dp, dend);
					vis_pst_16(dd, dp, emask);
					dp++;
					vis_alignaddr(sa1, off);
				}
			}
		}

/* 4-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 2) / 24;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s10 = s11;
			s11 = sp1[1];
			sd10 = vis_faligndata(s10, s11);
			s10 = sp1[2];
			sd11 = vis_faligndata(s11, s10);
			s11 = vis_ld_d64_nf(sp1 + 3);
			sd12 = vis_faligndata(s10, s11);
			sp1 += 3;

			vis_alignaddr(sa2, off);
			s20 = s21;
			s21 = sp2[1];
			sd20 = vis_faligndata(s20, s21);
			s20 = sp2[2];
			sd21 = vis_faligndata(s21, s20);
			s21 = vis_ld_d64_nf(sp2 + 3);
			sd22 = vis_faligndata(s20, s21);
			sp2 += 3;

			MLIB_V_IMAGESCALARBLEND_U16(sd10, sd20, a0, b0);
			(*dp++) = dd;
			MLIB_V_IMAGESCALARBLEND_U16(sd11, sd21, a1, b1);
			(*dp++) = dd;
			MLIB_V_IMAGESCALARBLEND_U16(sd12, sd22, a2, b2);
			(*dp++) = dd;
			vis_alignaddr(sa1, off);
		}

/* last 4-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s10 = s11;
			s11 = vis_ld_d64_nf(sp1 + 1);
			sd10 = vis_faligndata(s10, s11);
			vis_alignaddr(sa2, off);
			s20 = s21;
			s21 = vis_ld_d64_nf(sp2 + 1);
			sd20 = vis_faligndata(s20, s21);
			MLIB_V_IMAGESCALARBLEND_U16(sd10, sd20, a0, b0);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s20 = s21;
				s21 = vis_ld_d64_nf(sp2 + 2);
				sd20 = vis_faligndata(s20, s21);
				vis_alignaddr(sa1, off);
				s10 = s11;
				s11 = vis_ld_d64_nf(sp1 + 2);
				sd10 = vis_faligndata(s10, s11);
				MLIB_V_IMAGESCALARBLEND_U16(sd10, sd20, a1, b1);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					sd10 = vis_faligndata(s11,
						vis_ld_d64_nf(sp1 + 3));
					vis_alignaddr(sa2, off);
					sd20 = vis_faligndata(s21,
						vis_ld_d64_nf(sp2 + 3));
					MLIB_V_IMAGESCALARBLEND_U16(sd10, sd20,
					    a2, b2);
					emask = vis_edge16(dp, dend);
					vis_pst_16(dd, dp, emask);
				}
			}
		}

		sl1 = sa1 = (mlib_u16 *)((mlib_u8 *)sl1 + slb1);
		sl2 = sa2 = (mlib_u16 *)((mlib_u8 *)sl2 + slb2);
		da = (mlib_u16 *)((mlib_u8 *)da + dlb);
	}
}

/* *********************************************************** */
