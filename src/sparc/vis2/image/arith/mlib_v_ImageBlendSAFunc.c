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

#pragma ident	"@(#)mlib_v_ImageBlendSAFunc.c	9.3	07/11/05 SMI"

/*
 * Internal functions.
 *
 * void mlib_v_ImageBlendSA_U8_2_A8D1X8(mlib_u8  *src1,
 *                                      mlib_u8  *src2,
 *                                      mlib_u8  *dst,
 *                                      mlib_u8  *aimg,
 *                                      mlib_s32 dsize);
 *
 * void mlib_v_ImageBlendSA_U8_2_A8D2(mlib_u8  *src1,
 *                                    mlib_s32 slb1,
 *                                    mlib_u8  *src2,
 *                                    mlib_s32 slb2,
 *                                    mlib_u8  *dst,
 *                                    mlib_s32 dlb,
 *                                    mlib_u8  *aimg,
 *                                    mlib_s32 alb,
 *                                    mlib_s32 xsize,
 *                                    mlib_s32 ysize);
 *
 * void mlib_v_ImageBlendSA_U8_2_DstA8D1(mlib_u8  *src1,
 *                                       mlib_u8  *src2,
 *                                       mlib_u8  *dst,
 *                                       mlib_u8  *aimg,
 *                                       mlib_s32 xsize);
 *
 * void mlib_v_ImageBlendSA_U8_2_D1(mlib_u8  *src1,
 *                                  mlib_u8  *src2,
 *                                  mlib_u8  *dst,
 *                                  mlib_u8  *aimg,
 *                                  mlib_s32 xsize);
 *
 * void mlib_v_ImageBlendSA_U8_2(mlib_u8  *src1,
 *                               mlib_s32 slb1,
 *                               mlib_u8  *src2,
 *                               mlib_s32 slb2,
 *                               mlib_u8  *dst,
 *                               mlib_s32 dlb,
 *                               mlib_u8  *aimg,
 *                               mlib_s32 alb,
 *                               mlib_s32 xsize,
 *                               mlib_s32 ysize);
 *
 * void mlib_v_ImageBlendSA_U8_3_A8D1(mlib_u8  *src1,
 *                                    mlib_u8  *src2,
 *                                    mlib_u8  *dst,
 *                                    mlib_u8  *aimg,
 *                                    mlib_s32 dsize);
 *
 * void mlib_v_ImageBlendSA_U8_3_A8D2(mlib_u8  *src1,
 *                                    mlib_s32 slb1,
 *                                    mlib_u8  *src2,
 *                                    mlib_s32 slb2,
 *                                    mlib_u8  *dst,
 *                                    mlib_s32 dlb,
 *                                    mlib_u8  *aimg,
 *                                    mlib_s32 alb,
 *                                    mlib_s32 xsize,
 *                                    mlib_s32 ysize);
 *
 * void mlib_v_ImageBlendSA_U8_3_DstA8D1(mlib_u8  *sa1,
 *                                       mlib_u8  *sa2,
 *                                       mlib_u8  *dst,
 *                                       mlib_u8  *aimg,
 *                                       mlib_s32 xsize);
 *
 * void mlib_v_ImageBlendSA_U8_3(mlib_u8  *src1,
 *                               mlib_s32 slb1,
 *                               mlib_u8  *src2,
 *                               mlib_s32 slb2,
 *                               mlib_u8  *dst,
 *                               mlib_s32 dlb,
 *                               mlib_u8  *aimg,
 *                               mlib_s32 alb,
 *                               mlib_s32 xsize,
 *                               mlib_s32 ysize);
 *
 * void mlib_v_ImageBlendSA_U8_4_A8D1X8(mlib_u8  *src1,
 *                                      mlib_u8  *src2,
 *                                      mlib_u8  *dst,
 *                                      mlib_u8  *aimg,
 *                                      mlib_s32 dsize);
 *
 * void mlib_v_ImageBlendSA_U8_4_A8D2(mlib_u8  *src1,
 *                                    mlib_s32 slb1,
 *                                    mlib_u8  *src2,
 *                                    mlib_s32 slb2,
 *                                    mlib_u8  *dst,
 *                                    mlib_s32 dlb,
 *                                    mlib_u8  *aimg,
 *                                    mlib_s32 alb,
 *                                    mlib_s32 xsize,
 *                                    mlib_s32 ysize);
 *
 * void mlib_v_ImageBlendSA_U8_4(mlib_u8  *src1,
 *                               mlib_s32 slb1,
 *                               mlib_u8  *src2,
 *                               mlib_s32 slb2,
 *                               mlib_u8  *dst,
 *                               mlib_s32 dlb,
 *                               mlib_u8  *aimg,
 *                               mlib_s32 alb,
 *                               mlib_s32 xsize,
 *                               mlib_s32 ysize);
 *
 * void mlib_v_ImageBlendSA_S16_2_A8D1X4(mlib_s16 *src1,
 *                                       mlib_s16 *src2,
 *                                       mlib_s16 *dst,
 *                                       mlib_s16 *aimg,
 *                                       mlib_s32 dsize);
 *
 * void mlib_v_ImageBlendSA_S16_2_A8D2(mlib_s16 *src1,
 *                                     mlib_s32 slb1,
 *                                     mlib_s16 *src2,
 *                                     mlib_s32 slb2,
 *                                     mlib_s16 *dst,
 *                                     mlib_s32 dlb,
 *                                     mlib_s16 *aimg,
 *                                     mlib_s32 alb,
 *                                     mlib_s32 xsize,
 *                                     mlib_s32 ysize);
 *
 * void mlib_v_ImageBlendSA_S16_2_DstA8D1(mlib_s16 *sa1,
 *                                        mlib_s16 *sa2,
 *                                        mlib_s16 *dst,
 *                                        mlib_s16 *aimg,
 *                                        mlib_s32 xsize);
 *
 * void mlib_v_ImageBlendSA_S16_2_D1(mlib_s16 *sa1,
 *                                   mlib_s16 *sa2,
 *                                   mlib_s16 *dst,
 *                                   mlib_s16 *aimg,
 *                                   mlib_s32 xsize);
 *
 * void mlib_v_ImageBlendSA_S16_2(mlib_s16 *src1,
 *                                mlib_s32 slb1,
 *                                mlib_s16 *src2,
 *                                mlib_s32 slb2,
 *                                mlib_s16 *dst,
 *                                mlib_s32 dlb,
 *                                mlib_s16 *aimg,
 *                                mlib_s32 alb,
 *                                mlib_s32 xsize,
 *                                mlib_s32 ysize);
 *
 * void mlib_v_ImageBlendSA_S16_3_A8D1(mlib_s16 *src1,
 *                                     mlib_s16 *src2,
 *                                     mlib_s16 *dst,
 *                                     mlib_s16 *aimg,
 *                                     mlib_s32 dsize);
 *
 * void mlib_v_ImageBlendSA_S16_3_A8D2(mlib_s16 *src1,
 *                                     mlib_s32 slb1,
 *                                     mlib_s16 *src2,
 *                                     mlib_s32 slb2,
 *                                     mlib_s16 *dst,
 *                                     mlib_s32 dlb,
 *                                     mlib_s16 *aimg,
 *                                     mlib_s32 alb,
 *                                     mlib_s32 xsize,
 *                                     mlib_s32 ysize);
 *
 * void mlib_v_ImageBlendSA_S16_3(mlib_s16 *src1,
 *                                mlib_s32 slb1,
 *                                mlib_s16 *src2,
 *                                mlib_s32 slb2,
 *                                mlib_s16 *dst,
 *                                mlib_s32 dlb,
 *                                mlib_s16 *aimg,
 *                                mlib_s32 alb,
 *                                mlib_s32 xsize,
 *                                mlib_s32 ysize);
 *
 * void mlib_v_ImageBlendSA_S16_4_A8D1(mlib_s16 *src1,
 *                                     mlib_s16 *src2,
 *                                     mlib_s16 *dst,
 *                                     mlib_s16 *aimg,
 *                                     mlib_s32 dsize);
 *
 * void mlib_v_ImageBlendSA_S16_4_A8D2(mlib_s16 *src1,
 *                                     mlib_s32 slb1,
 *                                     mlib_s16 *src2,
 *                                     mlib_s32 slb2,
 *                                     mlib_s16 *dst,
 *                                     mlib_s32 dlb,
 *                                     mlib_s16 *aimg,
 *                                     mlib_s32 alb,
 *                                     mlib_s32 xsize,
 *                                     mlib_s32 ysize);
 *
 * void mlib_v_ImageBlendSA_S16_4(mlib_s16 *src1,
 *                                mlib_s32 slb1,
 *                                mlib_s16 *src2,
 *                                mlib_s32 slb2,
 *                                mlib_s16 *dst,
 *                                mlib_s32 dlb,
 *                                mlib_s16 *aimg,
 *                                mlib_s32 alb,
 *                                mlib_s32 xsize,
 *                                mlib_s32 ysize);
 *
 * void mlib_v_ImageBlendSA_U16_2_A8D1X4(mlib_u16 *src1,
 *                                       mlib_u16 *src2,
 *                                       mlib_u16 *dst,
 *                                       mlib_u16 *aimg,
 *                                       mlib_s32 dsize);
 *
 * void mlib_v_ImageBlendSA_U16_2_A8D2(mlib_u16 *src1,
 *                                     mlib_s32 slb1,
 *                                     mlib_u16 *src2,
 *                                     mlib_s32 slb2,
 *                                     mlib_u16 *dst,
 *                                     mlib_s32 dlb,
 *                                     mlib_u16 *aimg,
 *                                     mlib_s32 alb,
 *                                     mlib_s32 xsize,
 *                                     mlib_s32 ysize);
 *
 * void mlib_v_ImageBlendSA_U16_2_DstA8D1(mlib_u16 *sa1,
 *                                        mlib_u16 *sa2,
 *                                        mlib_u16 *dst,
 *                                        mlib_u16 *aimg,
 *                                        mlib_s32 xsize);
 *
 * void mlib_v_ImageBlendSA_U16_2_D1(mlib_u16 *sa1,
 *                                   mlib_u16 *sa2,
 *                                   mlib_u16 *dst,
 *                                   mlib_u16 *aimg,
 *                                   mlib_s32 xsize);
 *
 * void mlib_v_ImageBlendSA_U16_2(mlib_u16 *src1,
 *                                mlib_s32 slb1,
 *                                mlib_u16 *src2,
 *                                mlib_s32 slb2,
 *                                mlib_u16 *dst,
 *                                mlib_s32 dlb,
 *                                mlib_u16 *aimg,
 *                                mlib_s32 alb,
 *                                mlib_s32 xsize,
 *                                mlib_s32 ysize);
 *
 * void mlib_v_ImageBlendSA_U16_3_A8D1(mlib_u16 *src1,
 *                                     mlib_u16 *src2,
 *                                     mlib_u16 *dst,
 *                                     mlib_u16 *aimg,
 *                                     mlib_s32 dsize);
 *
 * void mlib_v_ImageBlendSA_U16_3_A8D2(mlib_u16 *src1,
 *                                     mlib_s32 slb1,
 *                                     mlib_u16 *src2,
 *                                     mlib_s32 slb2,
 *                                     mlib_u16 *dst,
 *                                     mlib_s32 dlb,
 *                                     mlib_u16 *aimg,
 *                                     mlib_s32 alb,
 *                                     mlib_s32 xsize,
 *                                     mlib_s32 ysize);
 *
 * void mlib_v_ImageBlendSA_U16_3(mlib_u16 *src1,
 *                                mlib_s32 slb1,
 *                                mlib_u16 *src2,
 *                                mlib_s32 slb2,
 *                                mlib_u16 *dst,
 *                                mlib_s32 dlb,
 *                                mlib_u16 *aimg,
 *                                mlib_s32 alb,
 *                                mlib_s32 xsize,
 *                                mlib_s32 ysize);
 *
 * void mlib_v_ImageBlendSA_U16_4_A8D1(mlib_u16 *src1,
 *                                     mlib_u16 *src2,
 *                                     mlib_u16 *dst,
 *                                     mlib_u16 *aimg,
 *                                     mlib_s32 dsize);
 *
 * void mlib_v_ImageBlendSA_U16_4_A8D2(mlib_u16 *src1,
 *                                     mlib_s32 slb1,
 *                                     mlib_u16 *src2,
 *                                     mlib_s32 slb2,
 *                                     mlib_u16 *dst,
 *                                     mlib_s32 dlb,
 *                                     mlib_u16 *aimg,
 *                                     mlib_s32 alb,
 *                                     mlib_s32 xsize,
 *                                     mlib_s32 ysize);
 *
 * void mlib_v_ImageBlendSA_U16_4(mlib_u16 *src1,
 *                                mlib_s32 slb1,
 *                                mlib_u16 *src2,
 *                                mlib_s32 slb2,
 *                                mlib_u16 *dst,
 *                                mlib_s32 dlb,
 *                                mlib_u16 *aimg,
 *                                mlib_s32 alb,
 *                                mlib_s32 xsize,
 *                                mlib_s32 ysize);
 *
 * void mlib_v_ImageBlendSA_S32(mlib_s32 *sa1,
 *                              mlib_s32 slb1,
 *                              mlib_s32 *sa2,
 *                              mlib_s32 slb2,
 *                              mlib_s32 *da,
 *                              mlib_s32 dlb,
 *                              mlib_s32 *aa,
 *                              mlib_s32 alb,
 *                              mlib_s32 xsize,
 *                              mlib_s32 ysize,
 *                              mlib_s32 csize);
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_v_ImageBlendFunc.h>
#include <mlib_ImageCopy.h>

/* *********************************************************** */

#define	SIZE	256

/* *********************************************************** */

#define	vis_fpmerge_hi(a, b)	vis_fpmerge(vis_read_hi(a), vis_read_hi(b))

#define	vis_fpmerge_lo(a, b)	vis_fpmerge(vis_read_lo(a), vis_read_lo(b))

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

#define	MLIB_V_IMAGEBLEND_U8_1(sd1, sd2, ad, dd)                \
	adh = vis_fmul8x16(vis_read_hi(ad), scale);             \
	adl = vis_fmul8x16(vis_read_lo(ad), scale);             \
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
mlib_v_ImageBlendSA_U8_2_A8D1X8(
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
	mlib_d64 ad, ad0, ad1;
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
	for (j = 0; j < (dsize / 8); j++) {
		ad = (*ap++);
		ad0 = vis_fpmerge_hi(ad, ad);
		ad1 = vis_fpmerge_lo(ad, ad);

		sd1 = (*sp1++);
		sd2 = (*sp2++);
		MLIB_V_IMAGEBLEND_U8(sd1, sd2, ad0, dd);
		(*dp++) = dd;

		sd1 = (*sp1++);
		sd2 = (*sp2++);
		MLIB_V_IMAGEBLEND_U8(sd1, sd2, ad1, dd);
		(*dp++) = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_U8_2_A8D2(
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
	mlib_d64 ad, ad0, ad1;
	mlib_d64 adh, adl, bdh, bdl;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rdh, rdl;
	mlib_d64 ones = vis_to_double_dup(0x0ff00ff0);
	mlib_u8 *dend;
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
			ad = (*ap++);
			ad0 = vis_fpmerge_hi(ad, ad);
			ad1 = vis_fpmerge_lo(ad, ad);

			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_V_IMAGEBLEND_U8(sd1, sd2, ad0, dd);
			(*dp++) = dd;

			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_V_IMAGEBLEND_U8(sd1, sd2, ad1, dd);
			(*dp++) = dd;
		}

		if (i < xsize) {
			ad = vis_ld_d64_nf(ap);
			ad0 = vis_fpmerge_hi(ad, ad);

			sd1 = vis_ld_d64_nf(sp1);
			sp1++;
			sd2 = vis_ld_d64_nf(sp2);
			sp2++;
			MLIB_V_IMAGEBLEND_U8(sd1, sd2, ad0, dd);
			dend = (mlib_u8 *)dl + 2 * xsize - 1;
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd1 = vis_ld_d64_nf(sp1);
				sd2 = vis_ld_d64_nf(sp2);
				ad1 = vis_fpmerge_lo(ad, ad);
				MLIB_V_IMAGEBLEND_U8(sd1, sd2, ad1, dd);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
			}
		}

		sl1 = sp1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
		sl2 = sp2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
		dl = dp = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		al = ap = (mlib_d64 *)((mlib_u8 *)al + alb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_U8_2_DstA8D1(
    mlib_u8 *src1,
    mlib_u8 *src2,
    mlib_u8 *dst,
    mlib_u8 *aimg,
    mlib_s32 xsize)
{
	mlib_u8 *sa1;
	mlib_u8 *sa2;
	mlib_u8 *aa;
	mlib_d64 *sp1, *sp2, *ap;
	mlib_u8 *dend;
	mlib_d64 *dp;
	mlib_d64 s11, s10;
	mlib_d64 s21, s20;
	mlib_d64 a0, a1;
	mlib_d64 sd10, sd20, ad0;
	mlib_d64 sd11, sd21, ad1;
	mlib_d64 ad, dd;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rdh, rdl;
	mlib_d64 ones = vis_to_double_dup(0x0ff00ff0);
	mlib_d64 scale = vis_to_double_dup(0x10001000);
	mlib_d64 adh, adl, bdh, bdl;
	mlib_s32 emask, mask1, mask2;
	mlib_s32 off;
	mlib_s32 i;

	sa1 = src1;
	sa2 = src2;
	aa = aimg;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)dst & (~7));
	off = (mlib_addr)dp - (mlib_addr)dst;
	dend = dst + 2 * xsize - 1;

	sp1 = (mlib_d64 *)vis_alignaddr(sa1, off);
	s10 = vis_ld_d64_nf(sp1);
	sp2 = (mlib_d64 *)vis_alignaddr(sa2, off);
	s20 = vis_ld_d64_nf(sp2);
	ap = (mlib_d64 *)vis_alignaddr(aa, (off >> 1));
	a0 = vis_ld_d64_nf(ap);

	mask1 = ((mlib_addr)(sa1 + off) & 7) * 0x11111111;
	mask2 = ((mlib_addr)(sa2 + off) & 7) * 0x11111111;
	emask = vis_edge8(dst, dend);

/* first 8-pixel column */

	if (emask != 0xff) {
		a1 = vis_ld_d64_nf(ap + 1);
		ad = vis_faligndata(a0, a1);
		ad0 = vis_fpmerge_hi(ad, ad);
		ap = (mlib_d64 *)vis_alignaddr(aa, ((off >> 1) + 4));
		a0 = vis_ld_d64_nf(ap);

		vis_write_bmask(mask1, 0x01234567);
		s11 = vis_ld_d64_nf(sp1 + 1);
		sd10 = vis_bshuffle(s10, s11);
		s10 = s11;
		sp1++;

		vis_write_bmask(mask2, 0x01234567);
		s21 = vis_ld_d64_nf(sp2 + 1);
		sd20 = vis_bshuffle(s20, s21);
		s20 = s21;
		sp2++;

		MLIB_V_IMAGEBLEND_U8_1(sd10, sd20, ad0, dd);
		vis_pst_8(dd, dp, emask);
		dp++;
	}

	xsize = ((mlib_u8 *)dend - (mlib_u8 *)dp + 1) >> 4;

#pragma pipeloop(0)
	for (i = 0; i < xsize; i++) {
		vis_write_bmask(mask1, 0x01234567);
		s11 = sp1[1];
		sd10 = vis_bshuffle(s10, s11);
		s10 = vis_ld_d64_nf(sp1 + 2);
		sd11 = vis_bshuffle(s11, s10);
		sp1 += 2;
		vis_write_bmask(mask2, 0x01234567);
		s21 = sp2[1];
		sd20 = vis_bshuffle(s20, s21);
		s20 = vis_ld_d64_nf(sp2 + 2);
		sd21 = vis_bshuffle(s21, s20);
		sp2 += 2;

		a1 = vis_ld_d64_nf(ap + 1);
		ad = vis_faligndata(a0, a1);
		ad0 = vis_fpmerge_hi(ad, ad);
		ad1 = vis_fpmerge_lo(ad, ad);
		a0 = a1;
		ap++;

		MLIB_V_IMAGEBLEND_U8_1(sd10, sd20, ad0, dd);
		(*dp++) = dd;

		MLIB_V_IMAGEBLEND_U8_1(sd11, sd21, ad1, dd);
		(*dp++) = dd;
	}

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		a1 = vis_ld_d64_nf(ap + 1);
		ad = vis_faligndata(a0, a1);
		ad0 = vis_fpmerge_hi(ad, ad);

		vis_write_bmask(mask1, 0x01234567);
		s11 = vis_ld_d64_nf(sp1 + 1);
		sd10 = vis_bshuffle(s10, s11);
		s10 = vis_ld_d64_nf(sp1 + 2);
		sd11 = vis_bshuffle(s11, s10);

		vis_write_bmask(mask2, 0x01234567);
		s21 = vis_ld_d64_nf(sp2 + 1);
		sd20 = vis_bshuffle(s20, s21);
		s20 = vis_ld_d64_nf(sp2 + 2);
		sd21 = vis_bshuffle(s21, s20);

		MLIB_V_IMAGEBLEND_U8_1(sd10, sd20, ad0, dd);
		emask = vis_edge8(dp, dend);
		vis_pst_8(dd, dp, emask);
		dp++;

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			ad1 = vis_fpmerge_lo(ad, ad);
			MLIB_V_IMAGEBLEND_U8_1(sd11, sd21, ad1, dd);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_U8_2_D1(
    mlib_u8 *src1,
    mlib_u8 *src2,
    mlib_u8 *dst,
    mlib_u8 *aimg,
    mlib_s32 xsize)
{
	mlib_u8 *sa1;
	mlib_u8 *sa2;
	mlib_u8 *aa;
	mlib_d64 *sp1, *sp2;
	mlib_f32 *ap;
	mlib_u8 *dend;
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
	mlib_d64 scale = vis_to_double_dup(0x10001000);
	mlib_d64 adh, adl, bdh, bdl;
	mlib_s32 off;
	mlib_s32 emask, mask1, mask2;
	mlib_s32 i;

	sa1 = src1;
	sa2 = src2;
	aa = aimg;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)dst & (~7));
	off = (mlib_addr)dp - (mlib_addr)dst;
	dend = dst + 2 * xsize - 1;

	sp1 = (mlib_d64 *)vis_alignaddr(sa1, off);
	s10 = vis_ld_d64_nf(sp1);
	sp2 = (mlib_d64 *)vis_alignaddr(sa2, off);
	s20 = vis_ld_d64_nf(sp2);
	ap = (mlib_f32 *)((mlib_addr)(aa + (off >> 1)) & (~3));
	vis_alignaddr(dp,
	    (((mlib_addr)(aa + (off >> 1)) - (mlib_addr)ap) * 2 + 1) & 7);

	a0 = vis_fpmerge(vis_ld_f32_nf(ap), vis_ld_f32_nf(ap));

	mask1 = ((mlib_addr)(sa1 + off) & 7) * 0x11111111;
	mask2 = ((mlib_addr)(sa2 + off) & 7) * 0x11111111;
	emask = vis_edge8(dst, dend);

/* first 8-pixel column */
	a1 = vis_fpmerge(vis_ld_f32_nf(ap + 1), vis_ld_f32_nf(ap + 1));
	ad0 = vis_faligndata(a0, a1);
	a0 = a1;
	ap++;

	vis_write_bmask(mask1, 0x01234567);
	s11 = vis_ld_d64_nf(sp1 + 1);
	sd10 = vis_bshuffle(s10, s11);
	s10 = s11;
	sp1++;

	vis_write_bmask(mask2, 0x01234567);
	s21 = vis_ld_d64_nf(sp2 + 1);
	sd20 = vis_bshuffle(s20, s21);
	s20 = s21;
	sp2++;

	MLIB_V_IMAGEBLEND_U8_1(sd10, sd20, ad0, dd);
	vis_pst_8(dd, dp, emask);
	dp++;

	xsize = ((mlib_u8 *)dend - (mlib_u8 *)dp + 1) >> 3;

#pragma pipeloop(0)
	for (i = 0; i < xsize; i++) {
		a1 = vis_fpmerge(vis_ld_f32_nf(ap + 1), vis_ld_f32_nf(ap + 1));
		ad0 = vis_faligndata(a0, a1);
		a0 = a1;
		ap++;

		vis_write_bmask(mask1, 0x01234567);
		s11 = vis_ld_d64_nf(sp1 + 1);
		sd10 = vis_bshuffle(s10, s11);
		s10 = s11;
		sp1++;

		vis_write_bmask(mask2, 0x01234567);
		s21 = vis_ld_d64_nf(sp2 + 1);
		sd20 = vis_bshuffle(s20, s21);
		s20 = s21;
		sp2++;

		MLIB_V_IMAGEBLEND_U8_1(sd10, sd20, ad0, dd);
		(*dp++) = dd;
	}

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		a1 = vis_fpmerge(vis_ld_f32_nf(ap + 1), vis_ld_f32_nf(ap + 1));
		ad0 = vis_faligndata(a0, a1);

		vis_write_bmask(mask1, 0x01234567);
		s11 = vis_ld_d64_nf(sp1 + 1);
		sd10 = vis_bshuffle(s10, s11);

		vis_write_bmask(mask2, 0x01234567);
		s21 = vis_ld_d64_nf(sp2 + 1);
		sd20 = vis_bshuffle(s20, s21);

		MLIB_V_IMAGEBLEND_U8_1(sd10, sd20, ad0, dd);
		emask = vis_edge8(dp, dend);
		vis_pst_8(dd, dp, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_U8_2(
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
	mlib_u8 *sl1, *sl2, *al;
	mlib_u8 *dl;
	mlib_d64 *dp;
	mlib_s32 off;
	mlib_s32 j;

	sl1 = src1;
	sl2 = src2;
	dl = dst;
	al = aimg;

/* row loop */
	for (j = 0; j < ysize; j++) {

		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;

		if ((off & 1) == 0) {
			mlib_v_ImageBlendSA_U8_2_DstA8D1(sl1, sl2, dl, al,
			    xsize);
		} else {
			mlib_v_ImageBlendSA_U8_2_D1(sl1, sl2, dl, al, xsize);
		}

		sl1 += slb1;
		sl2 += slb2;
		dl += dlb;
		al += alb;
	}
}

/* *********************************************************** */

#define	MLIB_V_IMAGEBLEND_U8_3(sd1, sd2, ad0, ad1, dd)          \
	adh = vis_fmul8x16(ad0, scale);                         \
	adl = vis_fmul8x16(ad1, scale);                         \
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

#define	MLIB_V_IMAGEBLEND_U8_3_1(sd1, sd2, ad0, ad1, dd)        \
	adh = vis_fmul8x16(ad0, scale);                         \
	adl = vis_fexpand(ad1);                                 \
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
mlib_v_ImageBlendSA_U8_3_A8D1(
    mlib_u8 *src1,
    mlib_u8 *src2,
    mlib_u8 *dst,
    mlib_u8 *aimg,
    mlib_s32 dsize)
{
	mlib_d64 *sp1, *sp2;
	mlib_d64 *aa;
	mlib_d64 *dp;
	mlib_u8 *dend;
	mlib_d64 sd10, sd20, ad0;
	mlib_d64 sd11, sd21, ad1;
	mlib_d64 sd12, sd22, ad2;
	mlib_d64 ad3, ad4, ad5;
	mlib_d64 ad, dd;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rdh, rdl;
	mlib_d64 ones = vis_to_double_dup(0x0ff00ff0);
	mlib_d64 scale = vis_to_double_dup(0x10001000);
	mlib_d64 adh, adl, bdh, bdl;
	mlib_s32 emask;
	mlib_s32 j;

	sp1 = (mlib_d64 *)src1;
	sp2 = (mlib_d64 *)src2;
	dp = (mlib_d64 *)dst;
	aa = (mlib_d64 *)aimg;

	dend = (mlib_u8 *)dp + 3 * dsize - 1;
	vis_write_bmask(0x01123345, 0);

/* 8-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 8); j += 8) {
		sd10 = (*sp1++);
		sd11 = (*sp1++);
		sd12 = (*sp1++);

		sd20 = (*sp2++);
		sd21 = (*sp2++);
		sd22 = (*sp2++);

		ad = (*aa++);

/* 00112233 */
		ad0 = vis_fpmerge(vis_read_hi(ad), vis_read_hi(ad));
/* 00011122 */
		ad1 = vis_bshuffle(ad0, ad0);
/* xxxx2333 */
		ad2 = vis_fpmerge(vis_read_hi(ad), vis_read_lo(ad0));
/* 44556677 */
		ad5 = vis_fpmerge(vis_read_lo(ad), vis_read_lo(ad));
/* 44455566 */
		ad3 = vis_bshuffle(ad5, ad5);
/* xxxx6777 */
		ad4 = vis_fpmerge(vis_read_lo(ad), vis_read_lo(ad5));

		MLIB_V_IMAGEBLEND_U8_3_1(sd10, sd20, vis_read_hi(ad1),
		    vis_read_lo(ad1), dd);
		(*dp++) = dd;

		MLIB_V_IMAGEBLEND_U8_3_1(sd11, sd21, vis_read_lo(ad2),
		    vis_read_hi(ad3), dd);
		(*dp++) = dd;

		MLIB_V_IMAGEBLEND_U8_3(sd12, sd22, vis_read_lo(ad3),
		    vis_read_lo(ad4), dd);
		(*dp++) = dd;
	}

	if (j < dsize) {
		sd10 = vis_ld_d64_nf(sp1);
		sd20 = vis_ld_d64_nf(sp2);

		ad = vis_ld_d64_nf(aa);
/* 00112233 */
		ad0 = vis_fpmerge(vis_read_hi(ad), vis_read_hi(ad));
/* 00011122 */
		ad1 = vis_bshuffle(ad0, ad0);

		MLIB_V_IMAGEBLEND_U8_3_1(sd10, sd20, vis_read_hi(ad1),
		    vis_read_lo(ad1), dd);
		emask = vis_edge8(dp, dend);
		vis_pst_8(dd, dp, emask);
		dp++;

		if ((mlib_addr)dp <= (mlib_addr)dend) {

			sd11 = vis_ld_d64_nf(sp1 + 1);
			sd21 = vis_ld_d64_nf(sp2 + 1);
/* xxxx2333 */
			ad2 = vis_fpmerge(vis_read_hi(ad), vis_read_lo(ad0));
/* 44556677 */
			ad5 = vis_fpmerge(vis_read_lo(ad), vis_read_lo(ad));
/* 44455566 */
			ad3 = vis_bshuffle(ad5, ad5);

			MLIB_V_IMAGEBLEND_U8_3_1(sd11, sd21, vis_read_lo(ad2),
			    vis_read_hi(ad3), dd);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {

				sd12 = vis_ld_d64_nf(sp1 + 2);
				sd22 = vis_ld_d64_nf(sp2 + 2);
/* xxxx6777 */
				ad4 =
				    vis_fpmerge(vis_read_lo(ad),
				    vis_read_lo(ad5));
				MLIB_V_IMAGEBLEND_U8_3(sd12, sd22,
				    vis_read_lo(ad3), vis_read_lo(ad4), dd);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_U8_3_A8D2(
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
	mlib_d64 *aa, *al;
	mlib_d64 *dp, *dl;
	mlib_s32 emask;
	mlib_u8 *dend;
	mlib_d64 sd10, sd20, ad0;
	mlib_d64 sd11, sd21, ad1;
	mlib_d64 sd12, sd22, ad2;
	mlib_d64 ad3, ad4, ad5;
	mlib_d64 ad, dd;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rdh, rdl;
	mlib_d64 ones = vis_to_double_dup(0x0ff00ff0);
	mlib_d64 scale = vis_to_double_dup(0x10001000);
	mlib_d64 adh, adl, bdh, bdl;
	mlib_s32 i, j;

	vis_write_bmask(0x01123345, 0);

	sl1 = sp1 = (mlib_d64 *)src1;
	sl2 = sp2 = (mlib_d64 *)src2;
	dl = dp = (mlib_d64 *)dst;
	al = aa = (mlib_d64 *)aimg;

/* row loop */
	for (j = 0; j < ysize; j++) {

		dend = (mlib_u8 *)dp + 3 * xsize - 1;

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 8); i += 8) {
			sd10 = (*sp1++);
			sd11 = (*sp1++);
			sd12 = (*sp1++);

			sd20 = (*sp2++);
			sd21 = (*sp2++);
			sd22 = (*sp2++);

			ad = (*aa++);

/* 00112233 */
			ad0 = vis_fpmerge(vis_read_hi(ad), vis_read_hi(ad));
/* 00011122 */
			ad1 = vis_bshuffle(ad0, ad0);
/* xxxx2333 */
			ad2 = vis_fpmerge(vis_read_hi(ad), vis_read_lo(ad0));
/* 44556677 */
			ad5 = vis_fpmerge(vis_read_lo(ad), vis_read_lo(ad));
/* 44455566 */
			ad3 = vis_bshuffle(ad5, ad5);
/* xxxx6777 */
			ad4 = vis_fpmerge(vis_read_lo(ad), vis_read_lo(ad5));

			MLIB_V_IMAGEBLEND_U8_3_1(sd10, sd20, vis_read_hi(ad1),
			    vis_read_lo(ad1), dd);
			(*dp++) = dd;

			MLIB_V_IMAGEBLEND_U8_3_1(sd11, sd21, vis_read_lo(ad2),
			    vis_read_hi(ad3), dd);
			(*dp++) = dd;

			MLIB_V_IMAGEBLEND_U8_3(sd12, sd22, vis_read_lo(ad3),
			    vis_read_lo(ad4), dd);
			(*dp++) = dd;
		}

		if (i < xsize) {
			sd10 = vis_ld_d64_nf(sp1);
			sd20 = vis_ld_d64_nf(sp2);

			ad = vis_ld_d64_nf(aa);
/* 00112233 */
			ad0 = vis_fpmerge(vis_read_hi(ad), vis_read_hi(ad));
/* 00011122 */
			ad1 = vis_bshuffle(ad0, ad0);

			MLIB_V_IMAGEBLEND_U8_3_1(sd10, sd20, vis_read_hi(ad1),
			    vis_read_lo(ad1), dd);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {

				sd11 = vis_ld_d64_nf(sp1 + 1);
				sd21 = vis_ld_d64_nf(sp2 + 1);
/* xxxx2333 */
				ad2 =
				    vis_fpmerge(vis_read_hi(ad),
				    vis_read_lo(ad0));
/* 44556677 */
				ad5 =
				    vis_fpmerge(vis_read_lo(ad),
				    vis_read_lo(ad));
/* 44455566 */
				ad3 = vis_bshuffle(ad5, ad5);

				MLIB_V_IMAGEBLEND_U8_3_1(sd11, sd21,
				    vis_read_lo(ad2), vis_read_hi(ad3), dd);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {

					sd12 = vis_ld_d64_nf(sp1 + 2);
					sd22 = vis_ld_d64_nf(sp2 + 2);
/* xxxx6777 */
					ad4 =
					    vis_fpmerge(vis_read_lo(ad),
					    vis_read_lo(ad5));
					MLIB_V_IMAGEBLEND_U8_3(sd12, sd22,
					    vis_read_lo(ad3), vis_read_lo(ad4),
					    dd);
					emask = vis_edge8(dp, dend);
					vis_pst_8(dd, dp, emask);
				}
			}
		}

		sl1 = sp1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
		sl2 = sp2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
		dl = dp = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		al = aa = (mlib_d64 *)((mlib_u8 *)al + alb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_U8_3_DstA8D1(
    mlib_u8 *sa1,
    mlib_u8 *sa2,
    mlib_u8 *dst,
    mlib_u8 *aimg,
    mlib_s32 xsize)
{
	mlib_d64 *sp1, *sp2;
	mlib_d64 *dp, dd;
	mlib_u8 *dend;
	mlib_d64 sd10, sd20, ad0;
	mlib_d64 sd11, sd21, ad1;
	mlib_d64 sd12, sd22, ad2;
	mlib_d64 s10, s11, s12, s20, s21, s22;
	mlib_d64 *ap, ad, a0, a1, ad3, ad4, ad5;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rdh, rdl;
	mlib_d64 ones = vis_to_double_dup(0x0ff00ff0);
	mlib_d64 scale = vis_to_double_dup(0x10001000);
	mlib_d64 adh, adl, bdh, bdl;
	mlib_s32 emask;
	mlib_s32 i;

	vis_write_bmask(0x01123345, 0);

/* prepare the destination address */
	dp = (mlib_d64 *)dst;
	dend = dst + 3 * xsize - 1;

	sp1 = (mlib_d64 *)vis_alignaddr(sa1, 0);
	s10 = vis_ld_d64_nf(sp1);
	sp2 = (mlib_d64 *)vis_alignaddr(sa2, 0);
	s20 = vis_ld_d64_nf(sp2);
	ap = (mlib_d64 *)vis_alignaddr(aimg, 0);
	a0 = vis_ld_d64_nf(ap);

/* 8-pixel column loop */
#pragma pipeloop(0)
	for (i = 0; i < xsize / 8; i++) {
		vis_alignaddr(sa1, 0);
		s11 = sp1[1];
		sd10 = vis_faligndata(s10, s11);
		s12 = sp1[2];
		sd11 = vis_faligndata(s11, s12);
		s10 = vis_ld_d64_nf(sp1 + 3);
		sd12 = vis_faligndata(s12, s10);
		sp1 += 3;

		vis_alignaddr(sa2, 0);
		s21 = sp2[1];
		sd20 = vis_faligndata(s20, s21);
		s22 = sp2[2];
		sd21 = vis_faligndata(s21, s22);
		s20 = vis_ld_d64_nf(sp2 + 3);
		sd22 = vis_faligndata(s22, s20);
		sp2 += 3;

		vis_alignaddr(aimg, 0);
		a1 = vis_ld_d64_nf(ap + 1);
		ad = vis_faligndata(a0, a1);
		a0 = a1;
		ap++;

/* 00112233 */
		ad0 = vis_fpmerge(vis_read_hi(ad), vis_read_hi(ad));
/* 00011122 */
		ad1 = vis_bshuffle(ad0, ad0);
/* xxxx2333 */
		ad2 = vis_fpmerge(vis_read_hi(ad), vis_read_lo(ad0));
/* 44556677 */
		ad5 = vis_fpmerge(vis_read_lo(ad), vis_read_lo(ad));
/* 44455566 */
		ad3 = vis_bshuffle(ad5, ad5);
/* xxxx6777 */
		ad4 = vis_fpmerge(vis_read_lo(ad), vis_read_lo(ad5));

		MLIB_V_IMAGEBLEND_U8_3(sd10, sd20, vis_read_hi(ad1),
		    vis_read_lo(ad1), dd);
		(*dp++) = dd;

		MLIB_V_IMAGEBLEND_U8_3(sd11, sd21, vis_read_lo(ad2),
		    vis_read_hi(ad3), dd);
		(*dp++) = dd;

		MLIB_V_IMAGEBLEND_U8_3(sd12, sd22, vis_read_lo(ad3),
		    vis_read_lo(ad4), dd);
		(*dp++) = dd;
	}

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		vis_alignaddr(sa1, 0);
		s11 = vis_ld_d64_nf(sp1 + 1);
		sd10 = vis_faligndata(s10, s11);
		s12 = vis_ld_d64_nf(sp1 + 2);
		sd11 = vis_faligndata(s11, s12);
		s10 = vis_ld_d64_nf(sp1 + 3);
		sd12 = vis_faligndata(s12, s10);

		vis_alignaddr(sa2, 0);
		s21 = vis_ld_d64_nf(sp2 + 1);
		sd20 = vis_faligndata(s20, s21);
		s22 = vis_ld_d64_nf(sp2 + 2);
		sd21 = vis_faligndata(s21, s22);
		s20 = vis_ld_d64_nf(sp2 + 3);
		sd22 = vis_faligndata(s22, s20);

		vis_alignaddr(aimg, 0);
		a1 = vis_ld_d64_nf(ap + 1);
		ad = vis_faligndata(a0, a1);

/* 00112233 */
		ad0 = vis_fpmerge(vis_read_hi(ad), vis_read_hi(ad));
/* 00011122 */
		ad1 = vis_bshuffle(ad0, ad0);

		MLIB_V_IMAGEBLEND_U8_3(sd10, sd20, vis_read_hi(ad1),
		    vis_read_lo(ad1), dd);
		emask = vis_edge8(dp, dend);
		vis_pst_8(dd, dp, emask);
		dp++;

		if ((mlib_addr)dp <= (mlib_addr)dend) {

/* xxxx2333 */
			ad2 = vis_fpmerge(vis_read_hi(ad), vis_read_lo(ad0));
/* 44556677 */
			ad5 = vis_fpmerge(vis_read_lo(ad), vis_read_lo(ad));
/* 44455566 */
			ad3 = vis_bshuffle(ad5, ad5);

			MLIB_V_IMAGEBLEND_U8_3(sd11, sd21, vis_read_lo(ad2),
			    vis_read_hi(ad3), dd);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {

/* xxxx6777 */
				ad4 =
				    vis_fpmerge(vis_read_lo(ad),
				    vis_read_lo(ad5));
				MLIB_V_IMAGEBLEND_U8_3(sd12, sd22,
				    vis_read_lo(ad3), vis_read_lo(ad4), dd);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_U8_3(
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
	mlib_u8 *sl1, *sl2, *al, *dl;
	mlib_s32 i, j, beg, a1;

	sl1 = src1;
	sl2 = src2;
	dl = dst;
	al = aimg;

/* row loop */
	for (j = 0; j < ysize; j++) {

		beg = ((mlib_s32)dl * 5) & 7;

		if (beg > xsize)
			beg = xsize;
		for (i = 0; i < beg; i++) {
			a1 = al[0];
			dl[0] = ((a1 * (sl1[0] - sl2[0])) >> 8) + sl2[0];
			dl[1] = ((a1 * (sl1[1] - sl2[1])) >> 8) + sl2[1];
			dl[2] = ((a1 * (sl1[2] - sl2[2])) >> 8) + sl2[2];
			al++;
			sl1 += 3;
			sl2 += 3;
			dl += 3;
		}

		mlib_v_ImageBlendSA_U8_3_DstA8D1(sl1, sl2, dl, al, xsize - beg);

		sl1 = src1 = src1 + slb1;
		sl2 = src2 = src2 + slb2;
		dl = dst = dst + dlb;
		al = aimg = aimg + alb;
	}
}

/* *********************************************************** */

#define	MLIB_V_IMAGEBLEND_U8_4(sd1, sd2, adh, adl, dd)               \
	adh = vis_fexpand_lo(adh);                                   \
	adl = vis_fexpand_lo(adl);                                   \
	bdh = vis_fpsub16(ones, adh);                                \
	bdl = vis_fpsub16(ones, adl);                                \
	rd1h = vis_fmul8x16al(vis_read_hi(sd1), vis_read_lo(adh));   \
	rd1l = vis_fmul8x16al(vis_read_lo(sd1), vis_read_lo(adl));   \
	rd2h = vis_fmul8x16al(vis_read_hi(sd2), vis_read_lo(bdh));   \
	rd2l = vis_fmul8x16al(vis_read_lo(sd2), vis_read_lo(bdl));   \
	rdh = vis_fpadd16(rd1h, rd2h);                               \
	rdl = vis_fpadd16(rd1l, rd2l);                               \
	dd = vis_fpack16_pair(rdh, rdl)

/* *********************************************************** */

#define	MLIB_V_IMAGEBLEND_U8_4_1(sd1, sd2, adh, adl, dd)             \
	adh = vis_fmul8x16(vis_read_lo(adh), scale);                 \
	adl = vis_fexpand_lo(adl);                                   \
	bdh = vis_fpsub16(ones, adh);                                \
	bdl = vis_fpsub16(ones, adl);                                \
	rd1h = vis_fmul8x16al(vis_read_hi(sd1), vis_read_lo(adh));   \
	rd1l = vis_fmul8x16al(vis_read_lo(sd1), vis_read_lo(adl));   \
	rd2h = vis_fmul8x16al(vis_read_hi(sd2), vis_read_lo(bdh));   \
	rd2l = vis_fmul8x16al(vis_read_lo(sd2), vis_read_lo(bdl));   \
	rdh = vis_fpadd16(rd1h, rd2h);                               \
	rdl = vis_fpadd16(rd1l, rd2l);                               \
	dd = vis_fpack16_pair(rdh, rdl)

/* *********************************************************** */

void
mlib_v_ImageBlendSA_U8_4_A8D1X8(
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
	mlib_d64 ad0, ad1, bdh, bdl;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rdh, rdl;
	mlib_d64 ones = vis_to_double_dup(0x0ff00ff0);
	mlib_s32 j;

	sp1 = (mlib_d64 *)src1;
	sp2 = (mlib_d64 *)src2;
	dp = (mlib_d64 *)dst;

#pragma pipeloop(0)
	for (j = 0; j < dsize; j += 2) {
		ad0 = vis_ld_u8(aimg);
		ad1 = vis_ld_u8(aimg + 1);
		aimg += 2;
		sd1 = (*sp1++);
		sd2 = (*sp2++);
		MLIB_V_IMAGEBLEND_U8_4(sd1, sd2, ad0, ad1, dd);
		(*dp++) = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_U8_4_A8D2(
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
	mlib_u8 *ap, *al;
	mlib_d64 ad0, ad1, bdh, bdl;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rdh, rdl;
	mlib_d64 ones = vis_to_double_dup(0x0ff00ff0);
	mlib_s32 i, j;

	sl1 = sp1 = (mlib_d64 *)src1;
	sl2 = sp2 = (mlib_d64 *)src2;
	dl = dp = (mlib_d64 *)dst;
	al = ap = (mlib_u8 *)aimg;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 2); i += 2) {
			ad0 = vis_ld_u8(ap);
			ad1 = vis_ld_u8(ap + 1);
			ap += 2;
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_V_IMAGEBLEND_U8_4(sd1, sd2, ad0, ad1, dd);
			(*dp++) = dd;
		}

		if (i < xsize) {
			ad0 = vis_ld_u8(ap);
			sd1 = vis_ld_d64_nf(sp1);
			sd2 = vis_ld_d64_nf(sp2);
			MLIB_V_IMAGEBLEND_U8_4(sd1, sd2, ad0, ad1, dd);
			*((mlib_f32 *)dp) = vis_read_hi(dd);
		}

		sl1 = sp1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
		sl2 = sp2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
		dl = dp = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		al = ap = al + alb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_U8_4(
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
	mlib_u8 *sl1, *sl2, *al;
	mlib_u8 *dl, *pdst;
	mlib_d64 *sp1, *sp2;
	mlib_d64 *spp1, *spp2;
	mlib_d64 *dp;
	mlib_d64 sd1, sd2;
	mlib_d64 sd10, sd11, sd20, sd21;
	mlib_d64 dd;
	mlib_d64 ad0, ad1, bdh, bdl;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rdh, rdl;
	mlib_d64 ones = vis_to_double_dup(0x0ff00ff0);
	mlib_d64 scale = vis_to_double_dup(0x10001000);
	mlib_d64 bufd[SIZE / 2];
	mlib_s32 i, j, num, col, mask;

	sl1 = src1;
	sl2 = src2;
	dl = dst;
	al = aimg;

/* row loop */
	for (j = 0; j < ysize; j++) {
		spp1 = (mlib_d64 *)((mlib_addr)(sl1) & ~7);
		spp2 = vis_alignaddr(sl2, 0);
		mask = ((mlib_addr)(sl1) & 7) * 0x11111111;

		for (col = 0; col < xsize; col += num) {
			num = xsize - col;

			if (num > SIZE)
				num = SIZE;

			vis_write_bmask(mask, 0x01234567);
			sp1 = spp1 + col / 2;
			sp2 = spp2 + col / 2;
			pdst = dl + 4 * col;

			vis_alignaddr(sl2, 0);

			dp = ((mlib_addr)pdst & 7) ? (mlib_d64 *)bufd
			    : (mlib_d64 *)pdst;
			sd10 = vis_ld_d64_nf(sp1);
			sp1++;
			sd20 = vis_ld_d64_nf(sp2);
			sp2++;
#pragma pipeloop(0)
			for (i = 0; i <= (num - 2); i += 2) {
				ad0 = vis_ld_u8(al + i);
				ad1 = vis_ld_u8(al + i + 1);
				sd11 = vis_ld_d64_nf(sp1);
				sp1++;
				sd21 = vis_ld_d64_nf(sp2);
				sp2++;
				sd1 = vis_bshuffle(sd10, sd11);
				sd2 = vis_faligndata(sd20, sd21);
				MLIB_V_IMAGEBLEND_U8_4_1(sd1, sd2, ad0, ad1,
				    dd);
				(*dp++) = dd;
				sd10 = sd11;
				sd20 = sd21;
			}

			if (i < num) {
				ad0 = vis_ld_u8(al + i);
				sd11 = vis_ld_d64_nf(sp1);
				sd21 = vis_ld_d64_nf(sp2);
				sd1 = vis_bshuffle(sd10, sd11);
				sd2 = vis_faligndata(sd20, sd21);
				MLIB_V_IMAGEBLEND_U8_4_1(sd1, sd2, ad0, ad1,
				    dd);
				*((mlib_f32 *)dp) = vis_read_hi(dd);
			}

			if ((mlib_addr)pdst & 7) {
				mlib_ImageCopy_na((mlib_u8 *)bufd, pdst,
				    num * 4);
			}
		}

		sl1 += slb1;
		sl2 += slb2;
		dl += dlb;
		al += alb;
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
mlib_v_ImageBlendSA_S16_2_A8D1X4(
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
	mlib_d64 a0, ad0, ad1;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rd1, rd2;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_s32 j;

	vis_alignaddr((void *)0, 4);

	sp1 = (mlib_d64 *)src1;
	sp2 = (mlib_d64 *)src2;
	dp = (mlib_d64 *)dst;
	ap = (mlib_d64 *)aimg;

	vis_write_bmask(0x01012323, 0);

/* 4-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j < (dsize / 4); j++) {
		ad = (*ap++);
		a0 = vis_faligndata(ad, ad);
		ad0 = vis_bshuffle(ad, ad);
		ad1 = vis_bshuffle(a0, a0);

		sd1 = (*sp1++);
		sd2 = (*sp2++);
		MLIB_V_IMAGEBLEND_S16(sd1, sd2, ad0, dd);
		(*dp++) = dd;

		sd1 = (*sp1++);
		sd2 = (*sp2++);
		MLIB_V_IMAGEBLEND_S16(sd1, sd2, ad1, dd);
		(*dp++) = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_S16_2_A8D2(
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
	mlib_s16 *dend;
	mlib_d64 dd;
	mlib_d64 *ap, *al;
	mlib_d64 ad, bd;
	mlib_d64 a0, ad0, ad1;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rd1, rd2;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_s32 emask;
	mlib_s32 i, j;

	vis_alignaddr((void *)0, 4);

	sl1 = sp1 = (mlib_d64 *)src1;
	sl2 = sp2 = (mlib_d64 *)src2;
	dl = dp = (mlib_d64 *)dst;
	al = ap = (mlib_d64 *)aimg;

	vis_write_bmask(0x01012323, 0);

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 4); i += 4) {
			ad = (*ap++);
			a0 = vis_faligndata(ad, ad);
			ad0 = vis_bshuffle(ad, ad);
			ad1 = vis_bshuffle(a0, a0);

			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_V_IMAGEBLEND_S16(sd1, sd2, ad0, dd);
			(*dp++) = dd;

			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_V_IMAGEBLEND_S16(sd1, sd2, ad1, dd);
			(*dp++) = dd;
		}

/* last 4-pixel column */

		if (i < xsize) {
			dend = (mlib_s16 *)dl + 2 * xsize - 1;
			ad = vis_ld_d64_nf(ap);
			ad0 = vis_bshuffle(ad, ad);

			sd1 = vis_ld_d64_nf(sp1);
			sd2 = vis_ld_d64_nf(sp2);

			MLIB_V_IMAGEBLEND_S16(sd1, sd2, ad0, dd);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd1 = vis_ld_d64_nf(sp1 + 1);
				sd2 = vis_ld_d64_nf(sp2 + 1);
				a0 = vis_faligndata(ad, ad);
				ad1 = vis_bshuffle(a0, a0);
				MLIB_V_IMAGEBLEND_S16(sd1, sd2, ad1, dd);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
			}
		}

		sl1 = sp1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
		sl2 = sp2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
		dl = dp = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		al = ap = (mlib_d64 *)((mlib_u8 *)al + alb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_S16_2_D1(
    mlib_s16 *sa1,
    mlib_s16 *sa2,
    mlib_s16 *dst,
    mlib_s16 *aimg,
    mlib_s32 xsize)
{
	mlib_d64 *sp1, *sp2;
	mlib_f32 *ap;
	mlib_s16 *dend;
	mlib_d64 *dp;
	mlib_d64 s11, s10;
	mlib_d64 s21, s20;
	mlib_d64 sd10, sd20, ad0;
	mlib_d64 sd11, sd21, ad1;
	mlib_d64 sd12, sd22, ad2;
	mlib_d64 sd13, sd23, ad3;
	mlib_d64 ah, al, bd, dd;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rd1, rd2;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_s32 emask, mask, off, off1, n;
	mlib_s32 i;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)dst & (~7));
	off = (mlib_addr)dp - (mlib_addr)dst;
	off1 = (off >> 1);
	dend = dst + 2 * xsize - 1;

	sp1 = (mlib_d64 *)vis_alignaddr(sa1, off);
	s10 = vis_ld_d64_nf(sp1);
	sp2 = (mlib_d64 *)vis_alignaddr(sa2, off);
	s20 = vis_ld_d64_nf(sp2);

	ap = (mlib_f32 *)((mlib_addr)(aimg + (off1 >> 1)) & (~3));
	off1 =
	    (((mlib_addr)(aimg + (off1 >> 1)) - (mlib_addr)ap) +
	    (off1 & 1)) * 8;
	mask = (mlib_s32)((((mlib_u64)0x021346578A9BCEDF) << off1) >> 32);
	vis_write_bmask(mask, 0);
	al = vis_fpmerge(vis_ld_f32_nf(ap), vis_ld_f32_nf(ap));

	emask = vis_edge16(dst, dend);

/* first 4-pixel column loop */

	if (emask != 0xf) {
		ah = vis_fpmerge(vis_ld_f32_nf(ap + 1), vis_ld_f32_nf(ap + 1));
		ad0 = vis_bshuffle(al, ah);
		ap++;
		al = ah;

		vis_alignaddr(sa1, off);
		s11 = vis_ld_d64_nf(sp1 + 1);
		sd10 = vis_faligndata(s10, s11);
		sp1++;
		s10 = s11;

		vis_alignaddr(sa2, off);
		s21 = vis_ld_d64_nf(sp2 + 1);
		sd20 = vis_faligndata(s20, s21);
		sp2++;
		s20 = s21;

		MLIB_V_IMAGEBLEND_S16(sd10, sd20, ad0, dd);

		vis_pst_16(dd, dp, emask);
		dp++;
	}

	n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 2) >> 5;

/* 8-pixel column loop */
#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		ah = vis_fpmerge(ap[1], ap[1]);
		ad0 = vis_bshuffle(al, ah);
		al = vis_fpmerge(ap[2], ap[2]);
		ad1 = vis_bshuffle(ah, al);
		ah = vis_fpmerge(ap[3], ap[3]);
		ad2 = vis_bshuffle(al, ah);
		al = vis_fpmerge(vis_ld_f32_nf(ap + 4), vis_ld_f32_nf(ap + 4));
		ad3 = vis_bshuffle(ah, al);
		ap += 4;

		vis_alignaddr(sa1, off);
		s11 = sp1[1];
		sd10 = vis_faligndata(s10, s11);
		s10 = sp1[2];
		sd11 = vis_faligndata(s11, s10);
		s11 = sp1[3];
		sd12 = vis_faligndata(s10, s11);
		s10 = vis_ld_d64_nf(sp1 + 4);
		sd13 = vis_faligndata(s11, s10);
		sp1 += 4;

		vis_alignaddr(sa2, off);
		s21 = sp2[1];
		sd20 = vis_faligndata(s20, s21);
		s20 = sp2[2];
		sd21 = vis_faligndata(s21, s20);
		s21 = sp2[3];
		sd22 = vis_faligndata(s20, s21);
		s20 = vis_ld_d64_nf(sp2 + 4);
		sd23 = vis_faligndata(s21, s20);
		sp2 += 4;

		MLIB_V_IMAGEBLEND_S16(sd10, sd20, ad0, dd);
		(*dp++) = dd;

		MLIB_V_IMAGEBLEND_S16(sd11, sd21, ad1, dd);
		(*dp++) = dd;

		MLIB_V_IMAGEBLEND_S16(sd12, sd22, ad2, dd);
		(*dp++) = dd;

		MLIB_V_IMAGEBLEND_S16(sd13, sd23, ad3, dd);
		(*dp++) = dd;
	}

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		ah = vis_fpmerge(vis_ld_f32_nf(ap + 1), vis_ld_f32_nf(ap + 1));
		ad0 = vis_bshuffle(al, ah);

		vis_alignaddr(sa1, off);
		s11 = vis_ld_d64_nf(sp1 + 1);
		sd10 = vis_faligndata(s10, s11);

		vis_alignaddr(sa2, off);
		s21 = vis_ld_d64_nf(sp2 + 1);
		sd20 = vis_faligndata(s20, s21);

		MLIB_V_IMAGEBLEND_S16(sd10, sd20, ad0, dd);
		emask = vis_edge16(dp, dend);
		vis_pst_16(dd, dp, emask);
		dp++;

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			al = vis_fpmerge(vis_ld_f32_nf(ap + 2),
			    vis_ld_f32_nf(ap + 2));
			ad1 = vis_bshuffle(ah, al);

			vis_alignaddr(sa1, off);
			s10 = vis_ld_d64_nf(sp1 + 2);
			sd11 = vis_faligndata(s11, s10);

			vis_alignaddr(sa2, off);
			s20 = vis_ld_d64_nf(sp2 + 2);
			sd21 = vis_faligndata(s21, s20);

			MLIB_V_IMAGEBLEND_S16(sd11, sd21, ad1, dd);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				ah = vis_fpmerge(vis_ld_f32_nf(ap + 3),
				    vis_ld_f32_nf(ap + 3));
				ad2 = vis_bshuffle(al, ah);

				vis_alignaddr(sa1, off);
				s11 = vis_ld_d64_nf(sp1 + 3);
				sd12 = vis_faligndata(s10, s11);

				vis_alignaddr(sa2, off);
				s21 = vis_ld_d64_nf(sp2 + 3);
				sd22 = vis_faligndata(s20, s21);

				MLIB_V_IMAGEBLEND_S16(sd12, sd22, ad2, dd);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					al = vis_fpmerge(vis_ld_f32_nf(ap + 4),
					    vis_ld_f32_nf(ap + 4));
					ad3 = vis_bshuffle(ah, al);

					vis_alignaddr(sa1, off);
					s10 = vis_ld_d64_nf(sp1 + 4);
					sd13 = vis_faligndata(s11, s10);

					vis_alignaddr(sa2, off);
					s20 = vis_ld_d64_nf(sp2 + 4);
					sd23 = vis_faligndata(s21, s20);

					MLIB_V_IMAGEBLEND_S16(sd13, sd23, ad3,
					    dd);
					emask = vis_edge16(dp, dend);
					vis_pst_16(dd, dp, emask);
				}
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_S16_2(
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
	mlib_s16 *sl1, *sl2, *al;
	mlib_s16 *dl;
	mlib_s32 j;

	sl1 = src1;
	sl2 = src2;
	dl = dst;
	al = aimg;

/* row loop */
	for (j = 0; j < ysize; j++) {

		mlib_v_ImageBlendSA_S16_2_D1(sl1, sl2, dl, al, xsize);

		sl1 = (mlib_s16 *)((mlib_u8 *)sl1 + slb1);
		sl2 = (mlib_s16 *)((mlib_u8 *)sl2 + slb2);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
		al = (mlib_s16 *)((mlib_u8 *)al + alb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_S16_3_A8D1(
    mlib_s16 *src1,
    mlib_s16 *src2,
    mlib_s16 *dst,
    mlib_s16 *aimg,
    mlib_s32 dsize)
{
	mlib_d64 *sp1, *sp2;
	mlib_d64 *dp;
	mlib_u8 *dend;
	mlib_d64 sd10, sd20, ad0;
	mlib_d64 sd11, sd21, ad1;
	mlib_d64 sd12, sd22, ad2;
	mlib_d64 ad3, ad4, ad5;
	mlib_d64 bd, dd;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rd1, rd2;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_s32 emask;
	mlib_s32 j;

/* initialize GSR offset */
	vis_alignaddr(dst, 6);

	sp1 = (mlib_d64 *)src1;
	sp2 = (mlib_d64 *)src2;
	dp = (mlib_d64 *)dst;

	dend = (mlib_u8 *)((mlib_s16 *)dp + 3 * dsize) - 1;

	vis_write_bmask(0x67EFEFEF, 0);

/* 4-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j < dsize / 4; j++) {
/* xxxxxx00 */
		ad0 = vis_ld_u16_i(aimg, 0);
/* xxxxxx11 */
		ad1 = vis_ld_u16_i(aimg, 2);
/* xxxxxx22 */
		ad2 = vis_ld_u16_i(aimg, 4);
/* xxxxxx33 */
		ad3 = vis_ld_u16_i(aimg, 6);

		sd10 = sp1[0];
		sd11 = sp1[1];
		sd12 = sp1[2];
		sp1 += 3;

		sd20 = sp2[0];
		sd21 = sp2[1];
		sd22 = sp2[2];
		sp2 += 3;

/* 00111111 */
		ad4 = vis_bshuffle(ad0, ad1);
/* 11222222 */
		ad5 = vis_bshuffle(ad1, ad2);
/* 22333333 */
		ad2 = vis_bshuffle(ad2, ad3);

/* 00001111 */
		ad3 = vis_faligndata(ad0, ad4);
/* 00000011 */
		ad0 = vis_faligndata(ad0, ad3);
/* 11112222 */
		ad1 = vis_faligndata(ad1, ad5);

		aimg += 4;

		MLIB_V_IMAGEBLEND_S16(sd10, sd20, ad0, dd);
		(*dp++) = dd;
		MLIB_V_IMAGEBLEND_S16(sd11, sd21, ad1, dd);
		(*dp++) = dd;
		MLIB_V_IMAGEBLEND_S16(sd12, sd22, ad2, dd);
		(*dp++) = dd;
	}

	if ((mlib_addr)dp <= (mlib_addr)dend) {

/* xxxxxx00 */
		ad0 = vis_ld_u16_i(aimg, 0);

		if ((6 + (mlib_addr)dp) <= (mlib_addr)dend) {
/* xxxxxx11 */
			ad1 = vis_ld_u16_i(aimg, 2);
		}

/* 00111111 */
		ad4 = vis_bshuffle(ad0, ad1);
/* 00001111 */
		ad3 = vis_faligndata(ad0, ad4);
/* 00000011 */
		ad0 = vis_faligndata(ad0, ad3);

		sd10 = vis_ld_d64_nf(sp1);
		sd20 = vis_ld_d64_nf(sp2);

		MLIB_V_IMAGEBLEND_S16(sd10, sd20, ad0, dd);
		sd11 = vis_ld_d64_nf(sp1 + 1);
		sd21 = vis_ld_d64_nf(sp2 + 1);
		emask = vis_edge8(dp, dend);
		vis_pst_8(dd, dp, emask);
		dp++;

		if ((mlib_addr)dp <= (mlib_addr)dend) {

			if ((4 + (mlib_addr)dp) <= (mlib_addr)dend) {
/* xxxxxx22 */
				ad2 = vis_ld_u16_i(aimg, 4);
			}

/* 11222222 */
			ad5 = vis_bshuffle(ad1, ad2);
/* 11112222 */
			ad1 = vis_faligndata(ad1, ad5);

			MLIB_V_IMAGEBLEND_S16(sd11, sd21, ad1, dd);
			sd12 = vis_ld_d64_nf(sp1 + 2);
			sd22 = vis_ld_d64_nf(sp2 + 2);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {

				if ((2 + (mlib_addr)dp) <= (mlib_addr)dend) {
/* xxxxxx33 */
					ad3 = vis_ld_u16_i(aimg, 6);
				}

/* 22333333 */
				ad2 = vis_bshuffle(ad2, ad3);

				MLIB_V_IMAGEBLEND_S16(sd12, sd22, ad2, dd);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_S16_3_A8D2(
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
	mlib_s16 *aa, *al;
	mlib_d64 *dp, *dl;
	mlib_u8 *dend;
	mlib_d64 sd10, sd20, ad0;
	mlib_d64 sd11, sd21, ad1;
	mlib_d64 sd12, sd22, ad2;
	mlib_d64 ad3, ad4, ad5;
	mlib_d64 bd, dd;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rd1, rd2;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_s32 emask;
	mlib_s32 i, j;

/* initialize GSR offset */
	vis_alignaddr(dst, 6);

	sl1 = sp1 = (mlib_d64 *)src1;
	sl2 = sp2 = (mlib_d64 *)src2;
	dl = dp = (mlib_d64 *)dst;
	al = aa = aimg;

	vis_write_bmask(0x67EFEFEF, 0);

/* row loop */
	for (j = 0; j < ysize; j++) {

		dend = (mlib_u8 *)((mlib_s16 *)dp + 3 * xsize) - 1;

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize / 4; i++) {
/* xxxxxx00 */
			ad0 = vis_ld_u16_i(aa, 0);
/* xxxxxx11 */
			ad1 = vis_ld_u16_i(aa, 2);
/* xxxxxx22 */
			ad2 = vis_ld_u16_i(aa, 4);
/* xxxxxx33 */
			ad3 = vis_ld_u16_i(aa, 6);

			sd10 = sp1[0];
			sd11 = sp1[1];
			sd12 = sp1[2];
			sp1 += 3;

			sd20 = sp2[0];
			sd21 = sp2[1];
			sd22 = sp2[2];
			sp2 += 3;

/* 00111111 */
			ad4 = vis_bshuffle(ad0, ad1);
/* 11222222 */
			ad5 = vis_bshuffle(ad1, ad2);
/* 22333333 */
			ad2 = vis_bshuffle(ad2, ad3);

/* 00001111 */
			ad3 = vis_faligndata(ad0, ad4);
/* 00000011 */
			ad0 = vis_faligndata(ad0, ad3);
/* 11112222 */
			ad1 = vis_faligndata(ad1, ad5);

			aa += 4;

			MLIB_V_IMAGEBLEND_S16(sd10, sd20, ad0, dd);
			(*dp++) = dd;
			MLIB_V_IMAGEBLEND_S16(sd11, sd21, ad1, dd);
			(*dp++) = dd;
			MLIB_V_IMAGEBLEND_S16(sd12, sd22, ad2, dd);
			(*dp++) = dd;
		}

		if ((mlib_addr)dp <= (mlib_addr)dend) {

/* xxxxxx00 */
			ad0 = vis_ld_u16_i(aa, 0);

			if ((6 + (mlib_addr)dp) <= (mlib_addr)dend) {
/* xxxxxx11 */
				ad1 = vis_ld_u16_i(aa, 2);
			}

/* 00111111 */
			ad4 = vis_bshuffle(ad0, ad1);
/* 00001111 */
			ad3 = vis_faligndata(ad0, ad4);
/* 00000011 */
			ad0 = vis_faligndata(ad0, ad3);

			sd10 = vis_ld_d64_nf(sp1);
			sd20 = vis_ld_d64_nf(sp2);

			MLIB_V_IMAGEBLEND_S16(sd10, sd20, ad0, dd);
			sd11 = vis_ld_d64_nf(sp1 + 1);
			sd21 = vis_ld_d64_nf(sp2 + 1);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {

				if ((4 + (mlib_addr)dp) <= (mlib_addr)dend) {
/* xxxxxx22 */
					ad2 = vis_ld_u16_i(aa, 4);
				}

/* 11222222 */
				ad5 = vis_bshuffle(ad1, ad2);
/* 11112222 */
				ad1 = vis_faligndata(ad1, ad5);

				MLIB_V_IMAGEBLEND_S16(sd11, sd21, ad1, dd);
				sd12 = vis_ld_d64_nf(sp1 + 2);
				sd22 = vis_ld_d64_nf(sp2 + 2);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {

					if ((2 + (mlib_addr)dp) <=
					    (mlib_addr)dend) {
/* xxxxxx33 */
						ad3 = vis_ld_u16_i(aa, 6);
					}

/* 22333333 */
					ad2 = vis_bshuffle(ad2, ad3);

					MLIB_V_IMAGEBLEND_S16(sd12, sd22, ad2,
					    dd);
					emask = vis_edge8(dp, dend);
					vis_pst_8(dd, dp, emask);
				}
			}
		}

		sl1 = sp1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
		sl2 = sp2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
		dl = dp = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		al = aa = (mlib_s16 *)((mlib_u8 *)al + alb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_S16_3_DstA8D1(
    mlib_s16 *src1,
    mlib_s16 *src2,
    mlib_s16 *dst,
    mlib_s16 *aimg,
    mlib_s32 xsize)
{
	mlib_d64 *sp1, *sp2;
	mlib_s16 *sa1, *sa2;
	mlib_s16 *aa;
	mlib_d64 *dp, dd;
	mlib_s16 *dl;
	mlib_u8 *dend;
	mlib_d64 sd10, sd20, ad0;
	mlib_d64 sd11, sd21, ad1;
	mlib_d64 sd12, sd22, ad2;
	mlib_d64 ad3;
	mlib_d64 s10, s11, s12, s20, s21, s22;
	mlib_d64 bd;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rd1, rd2;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_s32 emask, mask0, mask1, mask2;
	mlib_s32 i;

	sa1 = src1;
	sa2 = src2;
	aa = aimg;
	dl = dst;

	mask0 = 0x676767EF;
	mask1 = 0x6767EFEF;
	mask2 = 0x67EFEFEF;

/* prepare the destination address */
	dp = (mlib_d64 *)dl;
	dend = (mlib_u8 *)((mlib_s16 *)dl + 3 * xsize) - 1;

	sp1 = (mlib_d64 *)vis_alignaddr(sa1, 0);
	s10 = vis_ld_d64_nf(sp1);
	sp2 = (mlib_d64 *)vis_alignaddr(sa2, 0);
	s20 = vis_ld_d64_nf(sp2);

/* 4-pixel column loop */
#pragma pipeloop(0)
	for (i = 0; i < xsize / 4; i++) {
		vis_alignaddr(sa1, 0);
		s11 = sp1[1];
		sd10 = vis_faligndata(s10, s11);
		s12 = sp1[2];
		sd11 = vis_faligndata(s11, s12);
		s10 = vis_ld_d64_nf(sp1 + 3);
		sd12 = vis_faligndata(s12, s10);
		sp1 += 3;

		vis_alignaddr(sa2, 0);
		s21 = sp2[1];
		sd20 = vis_faligndata(s20, s21);
		s22 = sp2[2];
		sd21 = vis_faligndata(s21, s22);
		s20 = vis_ld_d64_nf(sp2 + 3);
		sd22 = vis_faligndata(s22, s20);
		sp2 += 3;

/* xxxxxx00 */
		ad0 = vis_ld_u16_i(aa, 0);
/* xxxxxx11 */
		ad1 = vis_ld_u16_i(aa, 2);
/* xxxxxx22 */
		ad2 = vis_ld_u16_i(aa, 4);
/* xxxxxx33 */
		ad3 = vis_ld_u16_i(aa, 6);

		vis_write_bmask(mask0, 0);
/* 00111111 */
		ad0 = vis_bshuffle(ad0, ad1);
		vis_write_bmask(mask1, 0);
/* 11222222 */
		ad1 = vis_bshuffle(ad1, ad2);
		vis_write_bmask(mask2, 0);
/* 22333333 */
		ad2 = vis_bshuffle(ad2, ad3);

		aa += 4;

		MLIB_V_IMAGEBLEND_S16(sd10, sd20, ad0, dd);
		(*dp++) = dd;
		MLIB_V_IMAGEBLEND_S16(sd11, sd21, ad1, dd);
		(*dp++) = dd;
		MLIB_V_IMAGEBLEND_S16(sd12, sd22, ad2, dd);
		(*dp++) = dd;
	}

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		vis_alignaddr(sa1, 0);
		s11 = vis_ld_d64_nf(sp1 + 1);
		sd10 = vis_faligndata(s10, s11);

		vis_alignaddr(sa2, 0);
		s21 = vis_ld_d64_nf(sp2 + 1);
		sd20 = vis_faligndata(s20, s21);

/* xxxxxx00 */
		ad0 = vis_ld_u16_i(aa, 0);

		if ((6 + (mlib_addr)dp) <= (mlib_addr)dend) {
/* xxxxxx11 */
			ad1 = vis_ld_u16_i(aa, 2);
		}

		vis_write_bmask(mask0, 0);
/* 00111111 */
		ad0 = vis_bshuffle(ad0, ad1);

		MLIB_V_IMAGEBLEND_S16(sd10, sd20, ad0, dd);

		emask = vis_edge8(dp, dend);
		vis_pst_8(dd, dp, emask);
		dp++;

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			vis_alignaddr(sa1, 0);
			s10 = vis_ld_d64_nf(sp1 + 2);
			sd11 = vis_faligndata(s11, s10);

			vis_alignaddr(sa2, 0);
			s20 = vis_ld_d64_nf(sp2 + 2);
			sd21 = vis_faligndata(s21, s20);

			if ((4 + (mlib_addr)dp) <= (mlib_addr)dend) {
/* xxxxxx22 */
				ad2 = vis_ld_u16_i(aa, 4);
			}

			vis_write_bmask(mask1, 0);
/* 11222222 */
			ad1 = vis_bshuffle(ad1, ad2);

			MLIB_V_IMAGEBLEND_S16(sd11, sd21, ad1, dd);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				vis_alignaddr(sa1, 0);
				s11 = vis_ld_d64_nf(sp1 + 3);
				sd12 = vis_faligndata(s10, s11);

				vis_alignaddr(sa2, 0);
				s21 = vis_ld_d64_nf(sp2 + 3);
				sd22 = vis_faligndata(s20, s21);

				if ((2 + (mlib_addr)dp) <= (mlib_addr)dend) {
/* xxxxxx33 */
					ad3 = vis_ld_u16_i(aa, 6);
				}

				vis_write_bmask(mask2, 0);
/* 22333333 */
				ad2 = vis_bshuffle(ad2, ad3);

				MLIB_V_IMAGEBLEND_S16(sd12, sd22, ad2, dd);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_S16_3(
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
	mlib_s16 *sl1, *sl2, *al, *dl;
	mlib_s32 i, j, beg;

/* variable for alpha coefficients */
	mlib_f32 fa;

#define	ONE_S16	0x7fff

	sl1 = src1;
	sl2 = src2;
	dl = dst;
	al = aimg;

/* row loop */
	for (j = 0; j < ysize; j++) {

		beg = ((mlib_s32)dl & 7) >> 1;

		if (beg > xsize)
			beg = xsize;
		for (i = 0; i < beg; i++) {
			fa = (mlib_f32)(al[0] & ONE_S16);
			dl[0] =
			    ((mlib_s32)(fa * (mlib_f32)(sl1[0] -
			    sl2[0])) >> 15) + sl2[0];
			dl[1] =
			    ((mlib_s32)(fa * (mlib_f32)(sl1[1] -
			    sl2[1])) >> 15) + sl2[1];
			dl[2] =
			    ((mlib_s32)(fa * (mlib_f32)(sl1[2] -
			    sl2[2])) >> 15) + sl2[2];
			al++;
			sl1 += 3;
			sl2 += 3;
			dl += 3;
		}

		mlib_v_ImageBlendSA_S16_3_DstA8D1(sl1, sl2, dl, al,
		    xsize - beg);

		sl1 = src1 = (mlib_s16 *)((mlib_u8 *)src1 + slb1);
		sl2 = src2 = (mlib_s16 *)((mlib_u8 *)src2 + slb2);
		dl = dst = (mlib_s16 *)((mlib_u8 *)dst + dlb);
		al = aimg = (mlib_s16 *)((mlib_u8 *)aimg + alb);
	}
}

/* *********************************************************** */

#define	MLIB_V_IMAGEBLEND_S16_4(sd1, sd2, ad, dd)               \
	ad = vis_fand(ones, ad);                                \
	ad = vis_fmul8x16al(k05, vis_read_lo(ad));              \
	ad = vis_fpadd16(ad, ad);                               \
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
mlib_v_ImageBlendSA_S16_4_A8D1(
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
	mlib_d64 ad, bd;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rd1, rd2;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_s32 j;

	sp1 = (mlib_d64 *)src1;
	sp2 = (mlib_d64 *)src2;
	dp = (mlib_d64 *)dst;

	vis_write_bmask(0x67676767, 0);

/* 4-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j < dsize; j++) {
		ad = vis_ld_u16(aimg);
		ad = vis_bshuffle(ad, ad);
		aimg++;
		sd1 = sp1[j];
		sd2 = sp2[j];
		MLIB_V_IMAGEBLEND_S16(sd1, sd2, ad, dd);
		dp[j] = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_S16_4_A8D2(
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
	mlib_s16 *ap, *al;
	mlib_d64 ad, bd;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rd1, rd2;
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_s32 i, j;

	sl1 = sp1 = (mlib_d64 *)src1;
	sl2 = sp2 = (mlib_d64 *)src2;
	dl = dp = (mlib_d64 *)dst;
	al = ap = (mlib_s16 *)aimg;

	vis_write_bmask(0x67676767, 0);

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize; i++) {
			ad = vis_ld_u16(ap);
			ad = vis_bshuffle(ad, ad);
			ap++;
			sd1 = sp1[i];
			sd2 = sp2[i];
			MLIB_V_IMAGEBLEND_S16(sd1, sd2, ad, dd);
			dp[i] = dd;
		}

		sl1 = sp1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
		sl2 = sp2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
		dl = dp = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		al = ap = (mlib_s16 *)((mlib_u8 *)al + alb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_S16_4(
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
	mlib_s16 *dend;
	mlib_d64 *sp1, *sp2;
	mlib_d64 *dp;
	mlib_d64 s11, s10;
	mlib_d64 s21, s20;
	mlib_d64 sd10, sd20, ad0;
	mlib_d64 adl, adh;
	mlib_d64 bd, dd;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rd1, rd2;
	mlib_d64 ones = vis_to_double_dup(0x7ffe7ffe);
	mlib_f32 k05 = vis_to_float(0x80808080);
	mlib_s32 off;
	mlib_s32 emask, mask;
	mlib_s32 i, j;

/* row loop */
	for (j = 0; j < ysize; j++) {

		dp = (mlib_d64 *)((mlib_addr)dst & (~7));
		off = (mlib_addr)dp - (mlib_addr)dst;
		dend = dst + 4 * xsize - 1;

		sp1 = (mlib_d64 *)vis_alignaddr(src1, off);
		s10 = vis_ld_d64_nf(sp1);
		sp2 = (mlib_d64 *)((mlib_addr)((mlib_u8 *)src2 + off) & ~7);
		s20 = vis_ld_d64_nf(sp2);

		if (off == 0) {
			vis_write_bmask(((mlib_addr)(src2) & 7) * 0x11111111,
			    0x01234567);

#pragma pipeloop(0)
			for (i = 0; i < xsize; i++) {
				s11 = vis_ld_d64_nf(sp1 + 1);
				sd10 = vis_faligndata(s10, s11);
				s10 = s11;
				sp1++;

				s21 = vis_ld_d64_nf(sp2 + 1);
				sd20 = vis_bshuffle(s20, s21);
				s20 = s21;
				sp2++;
				ad0 = vis_ld_u16(aimg + i);
				MLIB_V_IMAGEBLEND_S16_4(sd10, sd20, ad0, dd);
				(*dp++) = dd;
			}
		} else {

/* prepare edge mask */
			emask = vis_edge16(dst, dend);

			mask =
			    (mlib_s32)(((mlib_u64)0x67676767EFEFEFEF) >> (-4 *
			    off));
			vis_write_bmask(mask, 0);

			vis_alignaddr(src1, off);
			s11 = vis_ld_d64_nf(sp1 + 1);
			sd10 = vis_faligndata(s10, s11);
			s10 = s11;
			sp1++;

			vis_alignaddr(src2, off);
			s21 = vis_ld_d64_nf(sp2 + 1);
			sd20 = vis_faligndata(s20, s21);
			s20 = s21;
			sp2++;
			adh = vis_ld_u16(aimg);
			ad0 = vis_bshuffle(adh, adh);

			MLIB_V_IMAGEBLEND_S16(sd10, sd20, ad0, dd);
			vis_pst_16(dd, dp, emask);
			dp++;

#pragma pipeloop(0)
			for (i = 1; i < xsize; i++) {
				vis_alignaddr(src1, off);
				s11 = vis_ld_d64_nf(sp1 + 1);
				sd10 = vis_faligndata(s10, s11);
				s10 = s11;
				sp1++;

				vis_alignaddr(src2, off);
				s21 = vis_ld_d64_nf(sp2 + 1);
				sd20 = vis_faligndata(s20, s21);
				s20 = s21;
				sp2++;
				adl = vis_ld_u16(aimg + i);
				ad0 = vis_bshuffle(adh, adl);

				adh = adl;

				MLIB_V_IMAGEBLEND_S16(sd10, sd20, ad0, dd);
				(*dp++) = dd;
			}

			vis_alignaddr(src1, off);
			s11 = vis_ld_d64_nf(sp1 + 1);
			sd10 = vis_faligndata(s10, s11);

			vis_alignaddr(src2, off);
			s21 = vis_ld_d64_nf(sp2 + 1);

			sd20 = vis_faligndata(s20, s21);
			ad0 = vis_bshuffle(adh, adl);

			MLIB_V_IMAGEBLEND_S16(sd10, sd20, ad0, dd);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
		}

		src1 = (mlib_s16 *)((mlib_u8 *)src1 + slb1);
		src2 = (mlib_s16 *)((mlib_u8 *)src2 + slb2);
		dst = (mlib_s16 *)((mlib_u8 *)dst + dlb);
		aimg = (mlib_s16 *)((mlib_u8 *)aimg + alb);
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
mlib_v_ImageBlendSA_U16_2_A8D1X4(
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
	mlib_d64 a0, ad0, ad1;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rd1, rd2;
	mlib_d64 mask = vis_to_double_dup(0xfffefffe);
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_f32 fmin = vis_to_float(0x80808080);
	mlib_s32 j;

	vis_alignaddr((void *)0, 4);

	sp1 = (mlib_d64 *)src1;
	sp2 = (mlib_d64 *)src2;
	dp = (mlib_d64 *)dst;
	ap = (mlib_d64 *)aimg;

	vis_write_bmask(0x01012323, 0);

/* 4-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j < (dsize / 4); j++) {
		ad = (*ap++);
		a0 = vis_faligndata(ad, ad);
		ad0 = vis_bshuffle(ad, ad);
		ad1 = vis_bshuffle(a0, a0);

		sd1 = (*sp1++);
		sd2 = (*sp2++);
		MLIB_V_IMAGEBLEND_U16(sd1, sd2, ad0, dd);
		(*dp++) = dd;

		sd1 = (*sp1++);
		sd2 = (*sp2++);
		MLIB_V_IMAGEBLEND_U16(sd1, sd2, ad1, dd);
		(*dp++) = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_U16_2_A8D2(
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
	mlib_u16 *dend;
	mlib_d64 dd;
	mlib_d64 *ap, *al;
	mlib_d64 ad, bd;
	mlib_d64 a0, ad0, ad1;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rd1, rd2;
	mlib_d64 mask = vis_to_double_dup(0xfffefffe);
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_f32 fmin = vis_to_float(0x80808080);
	mlib_s32 emask;
	mlib_s32 i, j;

	vis_alignaddr((void *)0, 4);

	sl1 = sp1 = (mlib_d64 *)src1;
	sl2 = sp2 = (mlib_d64 *)src2;
	dl = dp = (mlib_d64 *)dst;
	al = ap = (mlib_d64 *)aimg;

	vis_write_bmask(0x01012323, 0);

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 4); i += 4) {
			ad = (*ap++);
			a0 = vis_faligndata(ad, ad);
			ad0 = vis_bshuffle(ad, ad);
			ad1 = vis_bshuffle(a0, a0);

			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_V_IMAGEBLEND_U16(sd1, sd2, ad0, dd);
			(*dp++) = dd;

			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_V_IMAGEBLEND_U16(sd1, sd2, ad1, dd);
			(*dp++) = dd;
		}

/* last 4-pixel column */

		if (i < xsize) {
			dend = (mlib_u16 *)dl + 2 * xsize - 1;
			ad = vis_ld_d64_nf(ap);
			ad0 = vis_bshuffle(ad, ad);

			sd1 = vis_ld_d64_nf(sp1);
			sd2 = vis_ld_d64_nf(sp2);

			MLIB_V_IMAGEBLEND_U16(sd1, sd2, ad0, dd);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd1 = vis_ld_d64_nf(sp1 + 1);
				sd2 = vis_ld_d64_nf(sp2 + 1);
				a0 = vis_faligndata(ad, ad);
				ad1 = vis_bshuffle(a0, a0);
				MLIB_V_IMAGEBLEND_U16(sd1, sd2, ad1, dd);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
			}
		}

		sl1 = sp1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
		sl2 = sp2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
		dl = dp = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		al = ap = (mlib_d64 *)((mlib_u8 *)al + alb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_U16_2_D1(
    mlib_u16 *sa1,
    mlib_u16 *sa2,
    mlib_u16 *dst,
    mlib_u16 *aimg,
    mlib_s32 xsize)
{
	mlib_d64 *sp1, *sp2;
	mlib_f32 *ap;
	mlib_u16 *dend;
	mlib_d64 *dp;
	mlib_d64 s11, s10;
	mlib_d64 s21, s20;
	mlib_d64 sd10, sd20, ad0;
	mlib_d64 sd11, sd21, ad1;
	mlib_d64 sd12, sd22, ad2;
	mlib_d64 sd13, sd23, ad3;
	mlib_d64 ah, al, bd, dd;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rd1, rd2;
	mlib_d64 mask = vis_to_double_dup(0xfffefffe);
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_f32 fmin = vis_to_float(0x80808080);
	mlib_s32 emask, mask1, off, off1, n;
	mlib_s32 i;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)dst & (~7));
	off = (mlib_addr)dp - (mlib_addr)dst;
	off1 = (off >> 1);
	dend = dst + 2 * xsize - 1;

	sp1 = (mlib_d64 *)vis_alignaddr(sa1, off);
	s10 = vis_ld_d64_nf(sp1);
	sp2 = (mlib_d64 *)vis_alignaddr(sa2, off);
	s20 = vis_ld_d64_nf(sp2);

	ap = (mlib_f32 *)((mlib_addr)(aimg + (off1 >> 1)) & (~3));
	off1 =
	    (((mlib_addr)(aimg + (off1 >> 1)) - (mlib_addr)ap) +
	    (off1 & 1)) * 8;
	mask1 = (mlib_s32)((((mlib_u64)0x021346578A9BCEDF) << off1) >> 32);
	vis_write_bmask(mask1, 0);
	al = vis_fpmerge(vis_ld_f32_nf(ap), vis_ld_f32_nf(ap));

	emask = vis_edge16(dst, dend);

/* first 4-pixel column loop */

	if (emask != 0xf) {
		ah = vis_fpmerge(vis_ld_f32_nf(ap + 1), vis_ld_f32_nf(ap + 1));
		ad0 = vis_bshuffle(al, ah);
		ap++;
		al = ah;

		vis_alignaddr(sa1, off);
		s11 = vis_ld_d64_nf(sp1 + 1);
		sd10 = vis_faligndata(s10, s11);
		sp1++;
		s10 = s11;

		vis_alignaddr(sa2, off);
		s21 = vis_ld_d64_nf(sp2 + 1);
		sd20 = vis_faligndata(s20, s21);
		sp2++;
		s20 = s21;

		MLIB_V_IMAGEBLEND_U16(sd10, sd20, ad0, dd);

		vis_pst_16(dd, dp, emask);
		dp++;
	}

	n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 2) >> 5;

/* 8-pixel column loop */
#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		ah = vis_fpmerge(ap[1], ap[1]);
		ad0 = vis_bshuffle(al, ah);
		al = vis_fpmerge(ap[2], ap[2]);
		ad1 = vis_bshuffle(ah, al);
		ah = vis_fpmerge(ap[3], ap[3]);
		ad2 = vis_bshuffle(al, ah);
		al = vis_fpmerge(vis_ld_f32_nf(ap + 4), vis_ld_f32_nf(ap + 4));
		ad3 = vis_bshuffle(ah, al);
		ap += 4;

		vis_alignaddr(sa1, off);
		s11 = sp1[1];
		sd10 = vis_faligndata(s10, s11);
		s10 = sp1[2];
		sd11 = vis_faligndata(s11, s10);
		s11 = sp1[3];
		sd12 = vis_faligndata(s10, s11);
		s10 = vis_ld_d64_nf(sp1 + 4);
		sd13 = vis_faligndata(s11, s10);
		sp1 += 4;

		vis_alignaddr(sa2, off);
		s21 = sp2[1];
		sd20 = vis_faligndata(s20, s21);
		s20 = sp2[2];
		sd21 = vis_faligndata(s21, s20);
		s21 = sp2[3];
		sd22 = vis_faligndata(s20, s21);
		s20 = vis_ld_d64_nf(sp2 + 4);
		sd23 = vis_faligndata(s21, s20);
		sp2 += 4;

		MLIB_V_IMAGEBLEND_U16(sd10, sd20, ad0, dd);
		(*dp++) = dd;

		MLIB_V_IMAGEBLEND_U16(sd11, sd21, ad1, dd);
		(*dp++) = dd;

		MLIB_V_IMAGEBLEND_U16(sd12, sd22, ad2, dd);
		(*dp++) = dd;

		MLIB_V_IMAGEBLEND_U16(sd13, sd23, ad3, dd);
		(*dp++) = dd;
	}

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		ah = vis_fpmerge(vis_ld_f32_nf(ap + 1), vis_ld_f32_nf(ap + 1));
		ad0 = vis_bshuffle(al, ah);

		vis_alignaddr(sa1, off);
		s11 = vis_ld_d64_nf(sp1 + 1);
		sd10 = vis_faligndata(s10, s11);

		vis_alignaddr(sa2, off);
		s21 = vis_ld_d64_nf(sp2 + 1);
		sd20 = vis_faligndata(s20, s21);

		MLIB_V_IMAGEBLEND_U16(sd10, sd20, ad0, dd);
		emask = vis_edge16(dp, dend);
		vis_pst_16(dd, dp, emask);
		dp++;

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			al = vis_fpmerge(vis_ld_f32_nf(ap + 2),
			    vis_ld_f32_nf(ap + 2));
			ad1 = vis_bshuffle(ah, al);

			vis_alignaddr(sa1, off);
			s10 = vis_ld_d64_nf(sp1 + 2);
			sd11 = vis_faligndata(s11, s10);

			vis_alignaddr(sa2, off);
			s20 = vis_ld_d64_nf(sp2 + 2);
			sd21 = vis_faligndata(s21, s20);

			MLIB_V_IMAGEBLEND_U16(sd11, sd21, ad1, dd);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				ah = vis_fpmerge(vis_ld_f32_nf(ap + 3),
				    vis_ld_f32_nf(ap + 3));
				ad2 = vis_bshuffle(al, ah);

				vis_alignaddr(sa1, off);
				s11 = vis_ld_d64_nf(sp1 + 3);
				sd12 = vis_faligndata(s10, s11);

				vis_alignaddr(sa2, off);
				s21 = vis_ld_d64_nf(sp2 + 3);
				sd22 = vis_faligndata(s20, s21);

				MLIB_V_IMAGEBLEND_U16(sd12, sd22, ad2, dd);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					al = vis_fpmerge(vis_ld_f32_nf(ap + 4),
					    vis_ld_f32_nf(ap + 4));
					ad3 = vis_bshuffle(ah, al);

					vis_alignaddr(sa1, off);
					s10 = vis_ld_d64_nf(sp1 + 4);
					sd13 = vis_faligndata(s11, s10);

					vis_alignaddr(sa2, off);
					s20 = vis_ld_d64_nf(sp2 + 4);
					sd23 = vis_faligndata(s21, s20);

					MLIB_V_IMAGEBLEND_U16(sd13, sd23, ad3,
					    dd);
					emask = vis_edge16(dp, dend);
					vis_pst_16(dd, dp, emask);
				}
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_U16_2(
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
	mlib_u16 *sl1, *sl2, *al;
	mlib_u16 *dl;
	mlib_s32 j;

	sl1 = src1;
	sl2 = src2;
	dl = dst;
	al = aimg;

/* row loop */
	for (j = 0; j < ysize; j++) {

		mlib_v_ImageBlendSA_U16_2_D1(sl1, sl2, dl, al, xsize);

		sl1 = (mlib_u16 *)((mlib_u8 *)sl1 + slb1);
		sl2 = (mlib_u16 *)((mlib_u8 *)sl2 + slb2);
		dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
		al = (mlib_u16 *)((mlib_u8 *)al + alb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_U16_3_A8D1(
    mlib_u16 *src1,
    mlib_u16 *src2,
    mlib_u16 *dst,
    mlib_u16 *aimg,
    mlib_s32 dsize)
{
	mlib_d64 *sp1, *sp2;
	mlib_d64 *dp;
	mlib_u8 *dend;
	mlib_d64 sd10, sd20, ad0;
	mlib_d64 sd11, sd21, ad1;
	mlib_d64 sd12, sd22, ad2;
	mlib_d64 ad3, ad4, ad5;
	mlib_d64 bd, dd;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rd1, rd2;
	mlib_d64 mask = vis_to_double_dup(0xfffefffe);
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_f32 fmin = vis_to_float(0x80808080);
	mlib_s32 emask;
	mlib_s32 j;

/* initialize GSR offset */
	vis_alignaddr(dst, 6);

	sp1 = (mlib_d64 *)src1;
	sp2 = (mlib_d64 *)src2;
	dp = (mlib_d64 *)dst;

	dend = (mlib_u8 *)((mlib_u16 *)dp + 3 * dsize) - 1;

	vis_write_bmask(0x67EFEFEF, 0);

/* 4-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j < dsize / 4; j++) {
/* xxxxxx00 */
		ad0 = vis_ld_u16_i(aimg, 0);
/* xxxxxx11 */
		ad1 = vis_ld_u16_i(aimg, 2);
/* xxxxxx22 */
		ad2 = vis_ld_u16_i(aimg, 4);
/* xxxxxx33 */
		ad3 = vis_ld_u16_i(aimg, 6);

		sd10 = sp1[0];
		sd11 = sp1[1];
		sd12 = sp1[2];
		sp1 += 3;

		sd20 = sp2[0];
		sd21 = sp2[1];
		sd22 = sp2[2];
		sp2 += 3;

/* 00111111 */
		ad4 = vis_bshuffle(ad0, ad1);
/* 11222222 */
		ad5 = vis_bshuffle(ad1, ad2);
/* 22333333 */
		ad2 = vis_bshuffle(ad2, ad3);

/* 00001111 */
		ad3 = vis_faligndata(ad0, ad4);
/* 00000011 */
		ad0 = vis_faligndata(ad0, ad3);
/* 11112222 */
		ad1 = vis_faligndata(ad1, ad5);

		aimg += 4;

		MLIB_V_IMAGEBLEND_U16(sd10, sd20, ad0, dd);
		(*dp++) = dd;
		MLIB_V_IMAGEBLEND_U16(sd11, sd21, ad1, dd);
		(*dp++) = dd;
		MLIB_V_IMAGEBLEND_U16(sd12, sd22, ad2, dd);
		(*dp++) = dd;
	}

	if ((mlib_addr)dp <= (mlib_addr)dend) {

/* xxxxxx00 */
		ad0 = vis_ld_u16_i(aimg, 0);

		if ((6 + (mlib_addr)dp) <= (mlib_addr)dend) {
/* xxxxxx11 */
			ad1 = vis_ld_u16_i(aimg, 2);
		}

/* 00111111 */
		ad4 = vis_bshuffle(ad0, ad1);
/* 00001111 */
		ad3 = vis_faligndata(ad0, ad4);
/* 00000011 */
		ad0 = vis_faligndata(ad0, ad3);

		sd10 = vis_ld_d64_nf(sp1);
		sd20 = vis_ld_d64_nf(sp2);

		MLIB_V_IMAGEBLEND_U16(sd10, sd20, ad0, dd);
		sd11 = vis_ld_d64_nf(sp1 + 1);
		sd21 = vis_ld_d64_nf(sp2 + 1);
		emask = vis_edge8(dp, dend);
		vis_pst_8(dd, dp, emask);
		dp++;

		if ((mlib_addr)dp <= (mlib_addr)dend) {

			if ((4 + (mlib_addr)dp) <= (mlib_addr)dend) {
/* xxxxxx22 */
				ad2 = vis_ld_u16_i(aimg, 4);
			}

/* 11222222 */
			ad5 = vis_bshuffle(ad1, ad2);
/* 11112222 */
			ad1 = vis_faligndata(ad1, ad5);

			MLIB_V_IMAGEBLEND_U16(sd11, sd21, ad1, dd);
			sd12 = vis_ld_d64_nf(sp1 + 2);
			sd22 = vis_ld_d64_nf(sp2 + 2);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {

				if ((2 + (mlib_addr)dp) <= (mlib_addr)dend) {
/* xxxxxx33 */
					ad3 = vis_ld_u16_i(aimg, 6);
				}

/* 22333333 */
				ad2 = vis_bshuffle(ad2, ad3);

				MLIB_V_IMAGEBLEND_U16(sd12, sd22, ad2, dd);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_U16_3_A8D2(
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
	mlib_u16 *aa, *al;
	mlib_d64 *dp, *dl;
	mlib_u8 *dend;
	mlib_d64 sd10, sd20, ad0;
	mlib_d64 sd11, sd21, ad1;
	mlib_d64 sd12, sd22, ad2;
	mlib_d64 ad3, ad4, ad5;
	mlib_d64 bd, dd;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rd1, rd2;
	mlib_d64 mask = vis_to_double_dup(0xfffefffe);
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_f32 fmin = vis_to_float(0x80808080);
	mlib_s32 emask;
	mlib_s32 i, j;

/* initialize GSR offset */
	vis_alignaddr(dst, 6);

	sl1 = sp1 = (mlib_d64 *)src1;
	sl2 = sp2 = (mlib_d64 *)src2;
	dl = dp = (mlib_d64 *)dst;
	al = aa = aimg;

	vis_write_bmask(0x67EFEFEF, 0);

/* row loop */
	for (j = 0; j < ysize; j++) {

		dend = (mlib_u8 *)((mlib_u16 *)dp + 3 * xsize) - 1;

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize / 4; i++) {
/* xxxxxx00 */
			ad0 = vis_ld_u16_i(aa, 0);
/* xxxxxx11 */
			ad1 = vis_ld_u16_i(aa, 2);
/* xxxxxx22 */
			ad2 = vis_ld_u16_i(aa, 4);
/* xxxxxx33 */
			ad3 = vis_ld_u16_i(aa, 6);

			sd10 = sp1[0];
			sd11 = sp1[1];
			sd12 = sp1[2];
			sp1 += 3;

			sd20 = sp2[0];
			sd21 = sp2[1];
			sd22 = sp2[2];
			sp2 += 3;

/* 00111111 */
			ad4 = vis_bshuffle(ad0, ad1);
/* 11222222 */
			ad5 = vis_bshuffle(ad1, ad2);
/* 22333333 */
			ad2 = vis_bshuffle(ad2, ad3);

/* 00001111 */
			ad3 = vis_faligndata(ad0, ad4);
/* 00000011 */
			ad0 = vis_faligndata(ad0, ad3);
/* 11112222 */
			ad1 = vis_faligndata(ad1, ad5);

			aa += 4;

			MLIB_V_IMAGEBLEND_U16(sd10, sd20, ad0, dd);
			(*dp++) = dd;
			MLIB_V_IMAGEBLEND_U16(sd11, sd21, ad1, dd);
			(*dp++) = dd;
			MLIB_V_IMAGEBLEND_U16(sd12, sd22, ad2, dd);
			(*dp++) = dd;
		}

		if ((mlib_addr)dp <= (mlib_addr)dend) {

/* xxxxxx00 */
			ad0 = vis_ld_u16_i(aa, 0);

			if ((6 + (mlib_addr)dp) <= (mlib_addr)dend) {
/* xxxxxx11 */
				ad1 = vis_ld_u16_i(aa, 2);
			}

/* 00111111 */
			ad4 = vis_bshuffle(ad0, ad1);
/* 00001111 */
			ad3 = vis_faligndata(ad0, ad4);
/* 00000011 */
			ad0 = vis_faligndata(ad0, ad3);

			sd10 = vis_ld_d64_nf(sp1);
			sd20 = vis_ld_d64_nf(sp2);

			MLIB_V_IMAGEBLEND_U16(sd10, sd20, ad0, dd);
			sd11 = vis_ld_d64_nf(sp1 + 1);
			sd21 = vis_ld_d64_nf(sp2 + 1);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {

				if ((4 + (mlib_addr)dp) <= (mlib_addr)dend) {
/* xxxxxx22 */
					ad2 = vis_ld_u16_i(aa, 4);
				}

/* 11222222 */
				ad5 = vis_bshuffle(ad1, ad2);
/* 11112222 */
				ad1 = vis_faligndata(ad1, ad5);

				MLIB_V_IMAGEBLEND_U16(sd11, sd21, ad1, dd);
				sd12 = vis_ld_d64_nf(sp1 + 2);
				sd22 = vis_ld_d64_nf(sp2 + 2);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {

					if ((2 + (mlib_addr)dp) <=
					    (mlib_addr)dend) {
/* xxxxxx33 */
						ad3 = vis_ld_u16_i(aa, 6);
					}

/* 22333333 */
					ad2 = vis_bshuffle(ad2, ad3);

					MLIB_V_IMAGEBLEND_U16(sd12, sd22, ad2,
					    dd);
					emask = vis_edge8(dp, dend);
					vis_pst_8(dd, dp, emask);
				}
			}
		}

		sl1 = sp1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
		sl2 = sp2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
		dl = dp = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		al = aa = (mlib_u16 *)((mlib_u8 *)al + alb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_U16_3_DstA8D1(
    mlib_u16 *src1,
    mlib_u16 *src2,
    mlib_u16 *dst,
    mlib_u16 *aimg,
    mlib_s32 xsize)
{
	mlib_d64 *sp1, *sp2;
	mlib_u16 *sa1, *sa2;
	mlib_u16 *aa;
	mlib_d64 *dp, dd;
	mlib_u16 *dl;
	mlib_u8 *dend;
	mlib_d64 sd10, sd20, ad0;
	mlib_d64 sd11, sd21, ad1;
	mlib_d64 sd12, sd22, ad2;
	mlib_d64 ad3;
	mlib_d64 s10, s11, s12, s20, s21, s22;
	mlib_d64 bd;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rd1, rd2;
	mlib_d64 mask = vis_to_double_dup(0xfffefffe);
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_f32 fmin = vis_to_float(0x80808080);
	mlib_s32 emask, mask0, mask1, mask2;
	mlib_s32 i;

	sa1 = src1;
	sa2 = src2;
	aa = aimg;
	dl = dst;

	mask0 = 0x676767EF;
	mask1 = 0x6767EFEF;
	mask2 = 0x67EFEFEF;

/* prepare the destination address */
	dp = (mlib_d64 *)dl;
	dend = (mlib_u8 *)((mlib_u16 *)dl + 3 * xsize) - 1;

	sp1 = (mlib_d64 *)vis_alignaddr(sa1, 0);
	s10 = vis_ld_d64_nf(sp1);
	sp2 = (mlib_d64 *)vis_alignaddr(sa2, 0);
	s20 = vis_ld_d64_nf(sp2);

/* 4-pixel column loop */
#pragma pipeloop(0)
	for (i = 0; i < xsize / 4; i++) {
		vis_alignaddr(sa1, 0);
		s11 = sp1[1];
		sd10 = vis_faligndata(s10, s11);
		s12 = sp1[2];
		sd11 = vis_faligndata(s11, s12);
		s10 = vis_ld_d64_nf(sp1 + 3);
		sd12 = vis_faligndata(s12, s10);
		sp1 += 3;

		vis_alignaddr(sa2, 0);
		s21 = sp2[1];
		sd20 = vis_faligndata(s20, s21);
		s22 = sp2[2];
		sd21 = vis_faligndata(s21, s22);
		s20 = vis_ld_d64_nf(sp2 + 3);
		sd22 = vis_faligndata(s22, s20);
		sp2 += 3;

/* xxxxxx00 */
		ad0 = vis_ld_u16_i(aa, 0);
/* xxxxxx11 */
		ad1 = vis_ld_u16_i(aa, 2);
/* xxxxxx22 */
		ad2 = vis_ld_u16_i(aa, 4);
/* xxxxxx33 */
		ad3 = vis_ld_u16_i(aa, 6);

		vis_write_bmask(mask0, 0);
/* 00111111 */
		ad0 = vis_bshuffle(ad0, ad1);
		vis_write_bmask(mask1, 0);
/* 11222222 */
		ad1 = vis_bshuffle(ad1, ad2);
		vis_write_bmask(mask2, 0);
/* 22333333 */
		ad2 = vis_bshuffle(ad2, ad3);

		aa += 4;

		MLIB_V_IMAGEBLEND_U16(sd10, sd20, ad0, dd);
		(*dp++) = dd;
		MLIB_V_IMAGEBLEND_U16(sd11, sd21, ad1, dd);
		(*dp++) = dd;
		MLIB_V_IMAGEBLEND_U16(sd12, sd22, ad2, dd);
		(*dp++) = dd;
	}

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		vis_alignaddr(sa1, 0);
		s11 = vis_ld_d64_nf(sp1 + 1);
		sd10 = vis_faligndata(s10, s11);

		vis_alignaddr(sa2, 0);
		s21 = vis_ld_d64_nf(sp2 + 1);
		sd20 = vis_faligndata(s20, s21);

/* xxxxxx00 */
		ad0 = vis_ld_u16_i(aa, 0);

		if ((6 + (mlib_addr)dp) <= (mlib_addr)dend) {
/* xxxxxx11 */
			ad1 = vis_ld_u16_i(aa, 2);
		}

		vis_write_bmask(mask0, 0);
/* 00111111 */
		ad0 = vis_bshuffle(ad0, ad1);

		MLIB_V_IMAGEBLEND_U16(sd10, sd20, ad0, dd);

		emask = vis_edge8(dp, dend);
		vis_pst_8(dd, dp, emask);
		dp++;

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			vis_alignaddr(sa1, 0);
			s10 = vis_ld_d64_nf(sp1 + 2);
			sd11 = vis_faligndata(s11, s10);

			vis_alignaddr(sa2, 0);
			s20 = vis_ld_d64_nf(sp2 + 2);
			sd21 = vis_faligndata(s21, s20);

			if ((4 + (mlib_addr)dp) <= (mlib_addr)dend) {
/* xxxxxx22 */
				ad2 = vis_ld_u16_i(aa, 4);
			}

			vis_write_bmask(mask1, 0);
/* 11222222 */
			ad1 = vis_bshuffle(ad1, ad2);

			MLIB_V_IMAGEBLEND_U16(sd11, sd21, ad1, dd);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				vis_alignaddr(sa1, 0);
				s11 = vis_ld_d64_nf(sp1 + 3);
				sd12 = vis_faligndata(s10, s11);

				vis_alignaddr(sa2, 0);
				s21 = vis_ld_d64_nf(sp2 + 3);
				sd22 = vis_faligndata(s20, s21);

				if ((2 + (mlib_addr)dp) <= (mlib_addr)dend) {
/* xxxxxx33 */
					ad3 = vis_ld_u16_i(aa, 6);
				}

				vis_write_bmask(mask2, 0);
/* 22333333 */
				ad2 = vis_bshuffle(ad2, ad3);

				MLIB_V_IMAGEBLEND_U16(sd12, sd22, ad2, dd);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_U16_3(
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
	mlib_u16 *sl1, *sl2, *al, *dl;
	mlib_s32 i, j, beg;

/* variable for alpha coefficients */
	mlib_f32 fa;

	sl1 = src1;
	sl2 = src2;
	dl = dst;
	al = aimg;

/* row loop */
	for (j = 0; j < ysize; j++) {

		beg = ((mlib_s32)dl & 7) >> 1;

		if (beg > xsize)
			beg = xsize;
		for (i = 0; i < beg; i++) {
			fa = (mlib_f32)al[0] * (mlib_f32)0.5;
			dl[0] =
			    ((mlib_s32)(fa * (mlib_f32)(sl1[0] -
			    sl2[0])) >> 15) + sl2[0];
			dl[1] =
			    ((mlib_s32)(fa * (mlib_f32)(sl1[1] -
			    sl2[1])) >> 15) + sl2[1];
			dl[2] =
			    ((mlib_s32)(fa * (mlib_f32)(sl1[2] -
			    sl2[2])) >> 15) + sl2[2];
			al++;
			sl1 += 3;
			sl2 += 3;
			dl += 3;
		}

		mlib_v_ImageBlendSA_U16_3_DstA8D1(sl1, sl2, dl, al,
		    xsize - beg);

		sl1 = src1 = (mlib_u16 *)((mlib_u8 *)src1 + slb1);
		sl2 = src2 = (mlib_u16 *)((mlib_u8 *)src2 + slb2);
		dl = dst = (mlib_u16 *)((mlib_u8 *)dst + dlb);
		al = aimg = (mlib_u16 *)((mlib_u8 *)aimg + alb);
	}
}

/* *********************************************************** */

#define	MLIB_V_IMAGEBLEND_U16_4(sd1, sd2, ad, dd)               \
	sd1 = vis_fxnor(ones, sd1);                             \
	sd2 = vis_fxnor(ones, sd2);                             \
	ad = vis_fand(ad, mask);                                \
	ad = vis_fmul8x16al(fmin, vis_read_lo(ad));             \
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
mlib_v_ImageBlendSA_U16_4_A8D1(
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

/* 4-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j < dsize; j++) {
		ad = vis_ld_u16(aimg);
		aimg++;
		sd1 = sp1[j];
		sd2 = sp2[j];
		MLIB_V_IMAGEBLEND_U16_4(sd1, sd2, ad, dd);
		dp[j] = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_U16_4_A8D2(
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
	mlib_u16 *ap, *al;
	mlib_d64 ad, bd;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rd1, rd2;
	mlib_d64 mask = vis_to_double_dup(0xfffefffe);
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_f32 fmin = vis_to_float(0x80808080);
	mlib_s32 i, j;

	sl1 = sp1 = (mlib_d64 *)src1;
	sl2 = sp2 = (mlib_d64 *)src2;
	dl = dp = (mlib_d64 *)dst;
	al = ap = (mlib_u16 *)aimg;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize; i++) {
			ad = vis_ld_u16(ap);
			ap++;
			sd1 = sp1[i];
			sd2 = sp2[i];
			MLIB_V_IMAGEBLEND_U16_4(sd1, sd2, ad, dd);
			dp[i] = dd;
		}

		sl1 = sp1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
		sl2 = sp2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
		dl = dp = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		al = ap = (mlib_u16 *)((mlib_u8 *)al + alb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_U16_4(
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
	mlib_u16 *dend;
	mlib_d64 *sp1, *sp2;
	mlib_d64 *dp;
	mlib_d64 s11, s10;
	mlib_d64 s21, s20;
	mlib_d64 sd10, sd20, ad0;
	mlib_d64 bd, dd;
	mlib_d64 rd1h, rd2h;
	mlib_d64 rd1l, rd2l;
	mlib_d64 rd1, rd2;
	mlib_d64 adl, adh;
	mlib_d64 mask = vis_to_double_dup(0xfffefffe);
	mlib_d64 ones = vis_to_double_dup(0x7fff7fff);
	mlib_f32 fmin = vis_to_float(0x80808080);
	mlib_s32 off;
	mlib_s32 emask, mask1;
	mlib_s32 i, j;

/* row loop */
	for (j = 0; j < ysize; j++) {

		dp = (mlib_d64 *)((mlib_addr)dst & (~7));
		off = (mlib_addr)dp - (mlib_addr)dst;
		dend = dst + 4 * xsize - 1;

		sp1 = (mlib_d64 *)vis_alignaddr(src1, off);
		s10 = vis_ld_d64_nf(sp1);
		sp2 = (mlib_d64 *)((mlib_addr)((mlib_u8 *)src2 + off) & ~7);
		s20 = vis_ld_d64_nf(sp2);

		if (off == 0) {
			vis_write_bmask(((mlib_addr)(src2) & 7) * 0x11111111,
			    0x01234567);

#pragma pipeloop(0)
			for (i = 0; i < xsize; i++) {
				s11 = vis_ld_d64_nf(sp1 + 1);
				sd10 = vis_faligndata(s10, s11);
				s10 = s11;
				sp1++;

				s21 = vis_ld_d64_nf(sp2 + 1);
				sd20 = vis_bshuffle(s20, s21);
				s20 = s21;
				sp2++;
				ad0 = vis_ld_u16(aimg + i);
				MLIB_V_IMAGEBLEND_U16_4(sd10, sd20, ad0, dd);
				(*dp++) = dd;
			}
		} else {

/* prepare edge mask */
			emask = vis_edge16(dst, dend);

			mask1 =
			    (mlib_s32)(((mlib_u64)0x67676767EFEFEFEF) >> (-4 *
			    off));
			vis_write_bmask(mask1, 0);

			vis_alignaddr(src1, off);
			s11 = vis_ld_d64_nf(sp1 + 1);
			sd10 = vis_faligndata(s10, s11);
			s10 = s11;
			sp1++;

			vis_alignaddr(src2, off);
			s21 = vis_ld_d64_nf(sp2 + 1);
			sd20 = vis_faligndata(s20, s21);
			s20 = s21;
			sp2++;
			adh = vis_ld_u16(aimg);
			ad0 = vis_bshuffle(adh, adh);

			MLIB_V_IMAGEBLEND_U16(sd10, sd20, ad0, dd);
			vis_pst_16(dd, dp, emask);
			dp++;

#pragma pipeloop(0)
			for (i = 1; i < xsize; i++) {
				vis_alignaddr(src1, off);
				s11 = vis_ld_d64_nf(sp1 + 1);
				sd10 = vis_faligndata(s10, s11);
				s10 = s11;
				sp1++;

				vis_alignaddr(src2, off);
				s21 = vis_ld_d64_nf(sp2 + 1);
				sd20 = vis_faligndata(s20, s21);
				s20 = s21;
				sp2++;
				adl = vis_ld_u16(aimg + i);
				ad0 = vis_bshuffle(adh, adl);

				adh = adl;

				MLIB_V_IMAGEBLEND_U16(sd10, sd20, ad0, dd);
				(*dp++) = dd;
			}

			vis_alignaddr(src1, off);
			s11 = vis_ld_d64_nf(sp1 + 1);
			sd10 = vis_faligndata(s10, s11);

			vis_alignaddr(src2, off);
			s21 = vis_ld_d64_nf(sp2 + 1);

			sd20 = vis_faligndata(s20, s21);
			ad0 = vis_bshuffle(adh, adl);

			MLIB_V_IMAGEBLEND_U16(sd10, sd20, ad0, dd);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
		}

		src1 = (mlib_u16 *)((mlib_u8 *)src1 + slb1);
		src2 = (mlib_u16 *)((mlib_u8 *)src2 + slb2);
		dst = (mlib_u16 *)((mlib_u8 *)dst + dlb);
		aimg = (mlib_u16 *)((mlib_u8 *)aimg + alb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageBlendSA_S32(
    mlib_s32 *sa1,
    mlib_s32 slb1,
    mlib_s32 *sa2,
    mlib_s32 slb2,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 *aa,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize)
{
/* indices for x and y */
	mlib_s32 i, j;

/* variable for alpha coefficients */
	mlib_d64 fa;
	mlib_d64 fshift = -1.0 / MLIB_S32_MIN;

	for (j = 0; j < ysize; j++) {
		if (csize == 2) {
#pragma pipeloop(0)
			for (i = 0; i < xsize; i++) {
				fa = mlib_fabs((mlib_d64)aa[i]) * fshift;
				da[2 * i] =
				    (mlib_s32)(fa * (mlib_d64)sa1[2 * i] + (1 -
				    fa) * (mlib_d64)sa2[2 * i]);
				da[2 * i + 1] =
				    (mlib_s32)(fa * (mlib_d64)sa1[2 * i + 1] +
				    (1 - fa) * (mlib_d64)sa2[2 * i + 1]);
			}
		} else if (csize == 3) {
#pragma pipeloop(0)
			for (i = 0; i < xsize; i++) {
				fa = mlib_fabs((mlib_d64)aa[i]) * fshift;
				da[3 * i] =
				    (mlib_s32)(fa * (mlib_d64)sa1[3 * i] + (1 -
				    fa) * (mlib_d64)sa2[3 * i]);
				da[3 * i + 1] =
				    (mlib_s32)(fa * (mlib_d64)sa1[3 * i + 1] +
				    (1 - fa) * (mlib_d64)sa2[3 * i + 1]);
				da[3 * i + 2] =
				    (mlib_s32)(fa * (mlib_d64)sa1[3 * i + 2] +
				    (1 - fa) * (mlib_d64)sa2[3 * i + 2]);
			}
		} else {
#pragma pipeloop(0)
			for (i = 0; i < xsize; i++) {
				fa = mlib_fabs((mlib_d64)aa[i]) * fshift;
				da[4 * i] =
				    (mlib_s32)(fa * (mlib_d64)sa1[4 * i] + (1 -
				    fa) * (mlib_d64)sa2[4 * i]);
				da[4 * i + 1] =
				    (mlib_s32)(fa * (mlib_d64)sa1[4 * i + 1] +
				    (1 - fa) * (mlib_d64)sa2[4 * i + 1]);
				da[4 * i + 2] =
				    (mlib_s32)(fa * (mlib_d64)sa1[4 * i + 2] +
				    (1 - fa) * (mlib_d64)sa2[4 * i + 2]);
				da[4 * i + 3] =
				    (mlib_s32)(fa * (mlib_d64)sa1[4 * i + 3] +
				    (1 - fa) * (mlib_d64)sa2[4 * i + 3]);
			}
		}

		sa1 = (mlib_s32 *)((mlib_u8 *)sa1 + slb1);
		sa2 = (mlib_s32 *)((mlib_u8 *)sa2 + slb2);
		da = (mlib_s32 *)((mlib_u8 *)da + dlb);
		aa = (mlib_s32 *)((mlib_u8 *)aa + alb);
	}
}

/* *********************************************************** */
