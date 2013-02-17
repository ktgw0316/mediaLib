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

#pragma ident	"@(#)mlib_v_ImageScaleFunc.c	9.3	07/11/05 SMI"

/*
 * Internal functions.
 *
 *     void mlib_v_ImageScale_U8_U8_124_A8D1X8(mlib_u8  *src,
 *                                             mlib_u8  *dst,
 *                                             mlib_s32 dsize,
 *                                             mlib_d64 ad,
 *                                             mlib_d64 bd);
 *
 *     void mlib_v_ImageScale_U8_U8_124_A8D2(mlib_u8  *src,
 *                                           mlib_s32 slb,
 *                                           mlib_u8  *dst,
 *                                           mlib_s32 dlb,
 *                                           mlib_s32 xsize,
 *                                           mlib_s32 ysize,
 *                                           mlib_d64 ad,
 *                                           mlib_d64 bd);
 *
 *     void mlib_v_ImageScale_U8_U8_124(mlib_u8  *src,
 *                                      mlib_s32 slb,
 *                                      mlib_u8  *dst,
 *                                      mlib_s32 dlb,
 *                                      mlib_s32 xsize,
 *                                      mlib_s32 ysize,
 *                                      mlib_d64 ad,
 *                                      mlib_d64 bd);
 *
 *     void mlib_v_ImageScale_U8_U8_3_A8D1(mlib_u8  *src,
 *                                         mlib_u8  *dst,
 *                                         mlib_s32 dsize,
 *                                         mlib_d64 ad,
 *                                         mlib_d64 bd);
 *
 *     void mlib_v_ImageScale_U8_U8_3_A8D2(mlib_u8  *src,
 *                                         mlib_s32 slb,
 *                                         mlib_u8  *dst,
 *                                         mlib_s32 dlb,
 *                                         mlib_s32 xsize,
 *                                         mlib_s32 ysize,
 *                                         mlib_d64 ad,
 *                                         mlib_d64 bd);
 *
 *     void mlib_v_ImageScale_U8_U8_3(mlib_u8  *src,
 *                                    mlib_s32 slb,
 *                                    mlib_u8  *dst,
 *                                    mlib_s32 dlb,
 *                                    mlib_s32 xsize,
 *                                    mlib_s32 ysize,
 *                                    mlib_d64 ad,
 *                                    mlib_d64 bd);
 *
 *     void mlib_v_ImageScale_S16_S16_124_A8D1X4(mlib_s16 *src,
 *                                               mlib_s16 *dst,
 *                                               mlib_s32 dsize,
 *                                               mlib_d64 ad,
 *                                               mlib_d64 bdh,
 *                                               mlib_d64 bdl);
 *
 *     void mlib_v_ImageScale_S16_S16_124_A8D2(mlib_s16 *src,
 *                                             mlib_s32 slb,
 *                                             mlib_s16 *dst,
 *                                             mlib_s32 dlb,
 *                                             mlib_s32 xsize,
 *                                             mlib_s32 ysize,
 *                                             mlib_d64 ad,
 *                                             mlib_d64 bdh,
 *                                             mlib_d64 bdl);
 *
 *     void mlib_v_ImageScale_S16_S16_124(mlib_s16 *src,
 *                                        mlib_s32 slb,
 *                                        mlib_s16 *dst,
 *                                        mlib_s32 dlb,
 *                                        mlib_s32 xsize,
 *                                        mlib_s32 ysize,
 *                                        mlib_d64 ad,
 *                                        mlib_d64 bdh,
 *                                        mlib_d64 bdl);
 *
 *     void mlib_v_ImageScale_S16_S16_3_A8D1(mlib_s16 *src,
 *                                           mlib_s16 *dst,
 *                                           mlib_s32 dsize,
 *                                           mlib_d64 ad,
 *                                           mlib_d64 bdh,
 *                                           mlib_d64 bdl);
 *
 *     void mlib_v_ImageScale_S16_S16_3_A8D2(mlib_s16 *src,
 *                                           mlib_s32 slb,
 *                                           mlib_s16 *dst,
 *                                           mlib_s32 dlb,
 *                                           mlib_s32 xsize,
 *                                           mlib_s32 ysize,
 *                                           mlib_d64 ad,
 *                                           mlib_d64 bdh,
 *                                           mlib_d64 bdl);
 *
 *     void mlib_v_ImageScale_S16_S16_3(mlib_s16 *src,
 *                                      mlib_s32 slb,
 *                                      mlib_s16 *dst,
 *                                      mlib_s32 dlb,
 *                                      mlib_s32 xsize,
 *                                      mlib_s32 ysize,
 *                                      mlib_d64 ad,
 *                                      mlib_d64 bdh,
 *                                      mlib_d64 bdl);
 *
 *     void mlib_v_ImageScale_U8_U8_D_124_A8D1X8(mlib_u8  *src,
 *                                               mlib_u8  *dst,
 *                                               mlib_s32 dsize,
 *                                               mlib_d64 ad,
 *                                               mlib_d64 bdh,
 *                                               mlib_d64 bdl);
 *
 *     void mlib_v_ImageScale_U8_U8_D_124_A8D2(mlib_u8  *src,
 *                                             mlib_s32 slb,
 *                                             mlib_u8  *dst,
 *                                             mlib_s32 dlb,
 *                                             mlib_s32 xsize,
 *                                             mlib_s32 ysize,
 *                                             mlib_d64 ad,
 *                                             mlib_d64 bdh,
 *                                             mlib_d64 bdl);
 *
 *     void mlib_v_ImageScale_U8_U8_D_124(mlib_u8  *src,
 *                                        mlib_s32 slb,
 *                                        mlib_u8  *dst,
 *                                        mlib_s32 dlb,
 *                                        mlib_s32 xsize,
 *                                        mlib_s32 ysize,
 *                                        mlib_d64 ad,
 *                                        mlib_d64 bdh,
 *                                        mlib_d64 bdl);
 *
 *     void mlib_v_ImageScale_U8_U8_D_3_A8D1(mlib_u8  *src,
 *                                           mlib_u8  *dst,
 *                                           mlib_s32 dsize,
 *                                           mlib_d64 ad,
 *                                           mlib_d64 bdh,
 *                                           mlib_d64 bdl);
 *
 *     void mlib_v_ImageScale_U8_U8_D_3_A8D2(mlib_u8  *src,
 *                                           mlib_s32 slb,
 *                                           mlib_u8  *dst,
 *                                           mlib_s32 dlb,
 *                                           mlib_s32 xsize,
 *                                           mlib_s32 ysize,
 *                                           mlib_d64 ad,
 *                                           mlib_d64 bdh,
 *                                           mlib_d64 bdl);
 *
 *     void mlib_v_ImageScale_U8_U8_D_3(mlib_u8  *src,
 *                                      mlib_s32 slb,
 *                                      mlib_u8  *dst,
 *                                      mlib_s32 dlb,
 *                                      mlib_s32 xsize,
 *                                      mlib_s32 ysize,
 *                                      mlib_d64 ad,
 *                                      mlib_d64 bdh,
 *                                      mlib_d64 bdl);
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageScale.h>

/* *********************************************************** */

#define	MLIB_V_IMAGESCALE_U8_U8(sd, adh, adl, bdh, bdl, dd)     \
	rdh = vis_fmul8x16(vis_read_hi(sd), adh);               \
	rdh = vis_fpadd16(rdh, bdh);                            \
	rdl = vis_fmul8x16(vis_read_lo(sd), adl);               \
	rdl = vis_fpadd16(rdl, bdl);                            \
	dd = vis_fpack16_pair(rdh, rdl)

/* *********************************************************** */

void
mlib_v_ImageScale_U8_U8_124_A8D1X8(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_d64 ad,
    mlib_d64 bd)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* loop variables */
	mlib_s32 j;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

/* 8-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 8); j += 8) {
		sd = (*sp++);
		MLIB_V_IMAGESCALE_U8_U8(sd, ad, ad, bd, bd, dd);
		(*dp++) = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_U8_U8_124_A8D2(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bd)
{
/* aligned src address */
	mlib_d64 *sp, *sl;

/* aligned dst address */
	mlib_d64 *dp, *dl;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 8); i += 8) {
			sd = (*sp++);
			MLIB_V_IMAGESCALE_U8_U8(sd, ad, ad, bd, bd, dd);
			(*dp++) = dd;
		}

/* last 8-pixel column loop */

		if (i < xsize) {
			sd = (*sp++);
			MLIB_V_IMAGESCALE_U8_U8(sd, ad, ad, bd, bd, dd);
			emask = vis_edge8((void *)0, (void *)(xsize - i - 1));
			vis_pst_8(dd, dp, emask);
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_U8_U8_124(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bd)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to begin of a src line */
	mlib_u8 *sl;

/* pointer to begin of a dst line */
	mlib_u8 *dl;

/* pointer to end of a dst line */
	mlib_u8 *dend;

/* source data */
	mlib_d64 sd, s0, s1;

/* destination data */
	mlib_d64 dd;

/* variables for constants */
	mlib_d64 ad0, bd0;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* edge mask */
	mlib_s32 emask;

/* offset in dst address */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;

	sl = src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

/* prepare the constant value */
		vis_alignaddr(dp, 2 * off);
		ad0 = vis_faligndata(ad, ad);
		bd0 = vis_faligndata(bd, bd);

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* generate edge mask for the start point */
		emask = vis_edge8(dl, dend);

/* first 8-pixel column */
		s0 = vis_ld_d64_nf(sp);
		s1 = vis_ld_d64_nf(sp + 1);
		sd = vis_faligndata(s0, s1);
		sp++;
		MLIB_V_IMAGESCALE_U8_U8(sd, ad0, ad0, bd0, bd0, dd);
		vis_pst_8(dd, dp, emask);
		dp++;

/* 8-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 1) >> 3;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s0 = sp[0];
			s1 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s0, s1);
			sp++;
			MLIB_V_IMAGESCALE_U8_U8(sd, ad0, ad0, bd0, bd0, dd);
			(*dp++) = dd;
		}

/* last 8-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd = vis_faligndata(s1, vis_ld_d64_nf(sp + 1));
			MLIB_V_IMAGESCALE_U8_U8(sd, ad0, ad0, bd0, bd0, dd);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_U8_U8_3_A8D1(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_d64 ad,
    mlib_d64 bd)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to end of dst */
	mlib_u8 *dend;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* alpha data */
	mlib_d64 ad1, ad2;

/* beta data */
	mlib_d64 bd1, bd2;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 j;

/* prepare alpha and beta values */
	vis_alignaddr(dst, 2);
	ad2 = vis_faligndata(ad, ad);
	ad1 = vis_faligndata(ad, ad2);
	bd2 = vis_faligndata(bd, bd);
	bd1 = vis_faligndata(bd, bd2);
	vis_alignaddr(dst, 4);
	ad2 = vis_faligndata(ad, ad1);
	bd2 = vis_faligndata(bd, bd1);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;
	dend = (mlib_u8 *)dp + dsize - 1;

/* 8-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 24); j += 24) {
		sd = (*sp++);
		MLIB_V_IMAGESCALE_U8_U8(sd, ad, ad1, bd, bd1, dd);
		(*dp++) = dd;

		sd = (*sp++);
		MLIB_V_IMAGESCALE_U8_U8(sd, ad2, ad, bd2, bd, dd);
		(*dp++) = dd;

		sd = (*sp++);
		MLIB_V_IMAGESCALE_U8_U8(sd, ad1, ad2, bd1, bd2, dd);
		(*dp++) = dd;
	}

/* last 8-pixel */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		sd = sp[0];
		MLIB_V_IMAGESCALE_U8_U8(sd, ad, ad1, bd, bd1, dd);
		emask = vis_edge8(dp, dend);
		vis_pst_8(dd, dp, emask);
		dp++;

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd = sp[1];
			MLIB_V_IMAGESCALE_U8_U8(sd, ad2, ad, bd2, bd, dd);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd = sp[2];
				MLIB_V_IMAGESCALE_U8_U8(sd, ad1, ad2, bd1, bd2,
				    dd);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_U8_U8_3_A8D2(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bd)
{
/* aligned src address */
	mlib_d64 *sp, *sl;

/* aligned dst address */
	mlib_d64 *dp, *dl;

/* pointer to end of dst */
	mlib_u8 *dend;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* alpha data */
	mlib_d64 ad1, ad2;

/* beta data */
	mlib_d64 bd1, bd2;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;

/* prepare alpha and beta values */
	vis_alignaddr(dst, 2);
	ad2 = vis_faligndata(ad, ad);
	ad1 = vis_faligndata(ad, ad2);
	bd2 = vis_faligndata(bd, bd);
	bd1 = vis_faligndata(bd, bd2);
	vis_alignaddr(dst, 4);
	ad2 = vis_faligndata(ad, ad1);
	bd2 = vis_faligndata(bd, bd1);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

		dend = (mlib_u8 *)dl + xsize - 1;

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 24); i += 24) {
			sd = (*sp++);
			MLIB_V_IMAGESCALE_U8_U8(sd, ad, ad1, bd, bd1, dd);
			(*dp++) = dd;

			sd = (*sp++);
			MLIB_V_IMAGESCALE_U8_U8(sd, ad2, ad, bd2, bd, dd);
			(*dp++) = dd;

			sd = (*sp++);
			MLIB_V_IMAGESCALE_U8_U8(sd, ad1, ad2, bd1, bd2, dd);
			(*dp++) = dd;
		}

/* last 8-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd = sp[0];
			MLIB_V_IMAGESCALE_U8_U8(sd, ad, ad1, bd, bd1, dd);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd = sp[1];
				MLIB_V_IMAGESCALE_U8_U8(sd, ad2, ad, bd2, bd,
				    dd);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					sd = sp[2];
					MLIB_V_IMAGESCALE_U8_U8(sd, ad1, ad2,
					    bd1, bd2, dd);
					emask = vis_edge8(dp, dend);
					vis_pst_8(dd, dp, emask);
				}
			}
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_U8_U8_3(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bd)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to src line */
	mlib_u8 *sl;

/* pointer to dst line */
	mlib_u8 *dl;

/* pointer to end of dst */
	mlib_u8 *dend;

/* source data */
	mlib_d64 sd, s0, s1;

/* destination data */
	mlib_d64 dd;

/* alpha data */
	mlib_d64 ad1, ad2, a[10];

/* beta data */
	mlib_d64 bd1, bd2, b[10];

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* edge masks */
	mlib_s32 emask;

/* offset of address in dst */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;

/* prepare alpha and beta values */
	a[1] = a[4] = a[7] = ad;
	b[1] = b[4] = b[7] = bd;
	vis_alignaddr((void *)0, 2);
	ad2 = vis_faligndata(ad, ad);
	bd2 = vis_faligndata(bd, bd);
	a[2] = a[5] = a[8] = ad1 = vis_faligndata(ad, ad2);
	b[2] = b[5] = b[8] = bd1 = vis_faligndata(bd, bd2);
	vis_alignaddr((void *)0, 4);
	a[0] = a[3] = a[6] = a[9] = vis_faligndata(ad, ad1);
	b[0] = b[3] = b[6] = b[9] = vis_faligndata(bd, bd1);

	sl = src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

/* prepare alpha and beta values */
		ad = a[7 + off];
		ad1 = a[8 + off];
		ad2 = a[9 + off];
		bd = b[7 + off];
		bd1 = b[8 + off];
		bd2 = b[9 + off];

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* generate edge mask for the start point */
		emask = vis_edge8(dl, dend);

/* first 8-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s0 = vis_ld_d64_nf(sp);
			s1 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s0, s1);
			sp++;
			MLIB_V_IMAGESCALE_U8_U8(sd, ad, ad1, bd, bd1, dd);
			vis_pst_8(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s0 = s1;
				s1 = vis_ld_d64_nf(sp + 1);
				sd = vis_faligndata(s0, s1);
				sp++;
				MLIB_V_IMAGESCALE_U8_U8(sd, ad2, ad, bd2, bd,
				    dd);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					s0 = s1;
					s1 = vis_ld_d64_nf(sp + 1);
					sd = vis_faligndata(s0, s1);
					sp++;
					MLIB_V_IMAGESCALE_U8_U8(sd, ad1, ad2,
					    bd1, bd2, dd);
					emask = vis_edge8(dp, dend);
					vis_pst_8(dd, dp, emask);
					dp++;
				}
			}
		}

/* 8-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 1) / 24;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s0 = sp[0];
			s1 = sp[1];
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGESCALE_U8_U8(sd, ad, ad1, bd, bd1, dd);
			(*dp++) = dd;

			s0 = sp[2];
			sd = vis_faligndata(s1, s0);
			MLIB_V_IMAGESCALE_U8_U8(sd, ad2, ad, bd2, bd, dd);
			(*dp++) = dd;

			s1 = vis_ld_d64_nf(sp + 3);
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGESCALE_U8_U8(sd, ad1, ad2, bd1, bd2, dd);
			(*dp++) = dd;
			sp += 3;
		}

/* last 8-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s0 = vis_ld_d64_nf(sp);
			s1 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGESCALE_U8_U8(sd, ad, ad1, bd, bd1, dd);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s0 = s1;
				s1 = vis_ld_d64_nf(sp + 2);
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_U8_U8(sd, ad2, ad, bd2, bd,
				    dd);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					s0 = s1;
					s1 = vis_ld_d64_nf(sp + 3);
					sd = vis_faligndata(s0, s1);
					MLIB_V_IMAGESCALE_U8_U8(sd, ad1, ad2,
					    bd1, bd2, dd);
					emask = vis_edge8(dp, dend);
					vis_pst_8(dd, dp, emask);
				}
			}
		}

		sl += slb;
		dl += dlb;
	}
}

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

void
mlib_v_ImageScale_S16_S16_124_A8D1X4(
    mlib_s16 *src,
    mlib_s16 *dst,
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
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* loop variables */
	mlib_s32 j;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

/* 4-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 4); j += 4) {
		sd = (*sp++);
		MLIB_V_IMAGESCALE_S16_S16(sd, vis_read_hi(ad), vis_read_lo(ad),
		    bdh, bdl, dd);
		(*dp++) = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_S16_S16_124_A8D2(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
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
	mlib_d64 *dp, *dl;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 4); i += 4) {
			sd = (*sp++);
			MLIB_V_IMAGESCALE_S16_S16(sd, vis_read_hi(ad),
			    vis_read_lo(ad), bdh, bdl, dd);
			(*dp++) = dd;
		}

/* last 4-pixel column */

		if (i < xsize) {
			sd = *sp;
			MLIB_V_IMAGESCALE_S16_S16(sd, vis_read_hi(ad),
			    vis_read_lo(ad), bdh, bdl, dd);
			emask =
			    vis_edge16((void *)0,
			    (void *)(2 * (xsize - i - 1)));
			vis_pst_16(dd, dp, emask);
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_S16_S16_124(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
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
	mlib_d64 *dp;

/* pointer to begin of a src line */
	mlib_s16 *sl;

/* pointer to begin of a dst line */
	mlib_s16 *dl;

/* pointer to end of a dst line */
	mlib_s16 *dend;

/* source data */
	mlib_d64 sd, s0, s1;

/* destination data */
	mlib_d64 dd;

/* constant variables */
	mlib_d64 ad0, b[8];

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

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
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

/* prepare the alpha value */
		vis_alignaddr(dp, off);
		ad0 = vis_faligndata(ad, ad);

/* prepare the beta value */
		bdh = b[i = (-off) >> 1];
		bdl = b[i + 4];

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* generate edge mask for the start point */
		emask = vis_edge16(dl, dend);

/* first 4-pixel column */

		if (emask != 0xf) {
			s0 = vis_ld_d64_nf(sp);
			s1 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s0, s1);
			sp++;
			MLIB_V_IMAGESCALE_S16_S16(sd, vis_read_hi(ad0),
			    vis_read_lo(ad0), bdh, bdl, dd);
			vis_pst_16(dd, dp, emask);
			dp++;
		}

/* 4-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 2) >> 3;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s0 = (*sp++);
			s1 = vis_ld_d64_nf(sp);
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGESCALE_S16_S16(sd, vis_read_hi(ad0),
			    vis_read_lo(ad0), bdh, bdl, dd);
			(*dp++) = dd;
		}

/* 4-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd = vis_faligndata(vis_ld_d64_nf(sp),
				vis_ld_d64_nf(sp + 1));
			MLIB_V_IMAGESCALE_S16_S16(sd, vis_read_hi(ad0),
			    vis_read_lo(ad0), bdh, bdl, dd);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
		}

		sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_S16_S16_3_A8D1(
    mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to end of dst */
	mlib_s16 *dend;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* alpha and beta data */
	mlib_d64 adm, bdm;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* edge mask */
	mlib_s32 emask;

/* loop variable */
	mlib_s32 j;

/* prepare alpha and beta values */
	vis_alignaddr(dst, 2);
	adm = vis_faligndata(ad, ad);
	vis_alignaddr(dst, 4);
	bdm = vis_faligndata(bdh, bdl);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;
	dend = (mlib_s16 *)dp + dsize - 1;

/* 4-pixel column loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 12); j += 12) {
		sd = sp[0];
		MLIB_V_IMAGESCALE_S16_S16(sd, vis_read_hi(ad), vis_read_lo(ad),
		    bdh, bdl, dd);
		(*dp++) = dd;
		sd = sp[1];
		MLIB_V_IMAGESCALE_S16_S16(sd, vis_read_hi(adm), vis_read_hi(ad),
		    bdm, bdh, dd);
		(*dp++) = dd;
		sd = sp[2];
		MLIB_V_IMAGESCALE_S16_S16(sd, vis_read_lo(ad), vis_read_hi(adm),
		    bdl, bdm, dd);
		(*dp++) = dd;
		sp += 3;
	}

/* last 4-pixel */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		sd = sp[0];
		MLIB_V_IMAGESCALE_S16_S16(sd, vis_read_hi(ad), vis_read_lo(ad),
		    bdh, bdl, dd);
		emask = vis_edge16(dp, dend);
		vis_pst_16(dd, dp, emask);
		dp++;

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd = sp[1];
			MLIB_V_IMAGESCALE_S16_S16(sd, vis_read_hi(adm),
			    vis_read_hi(ad), bdm, bdh, dd);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd = sp[2];
				MLIB_V_IMAGESCALE_S16_S16(sd, vis_read_lo(ad),
				    vis_read_hi(adm), bdl, bdm, dd);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_S16_S16_3_A8D2(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
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
	mlib_d64 *dp, *dl;

/* pointer to end of dst */
	mlib_s16 *dend;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* alpha and beta data */
	mlib_d64 adm, bdm;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;

/* prepare alpha and beta values */
	vis_alignaddr(dst, 2);
	adm = vis_faligndata(ad, ad);
	vis_alignaddr(dst, 4);
	bdm = vis_faligndata(bdh, bdl);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

		dend = (mlib_s16 *)dl + xsize - 1;

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 12); i += 12) {
			sd = sp[0];
			MLIB_V_IMAGESCALE_S16_S16(sd, vis_read_hi(ad),
			    vis_read_lo(ad), bdh, bdl, dd);
			(*dp++) = dd;
			sd = sp[1];
			MLIB_V_IMAGESCALE_S16_S16(sd, vis_read_hi(adm),
			    vis_read_hi(ad), bdm, bdh, dd);
			(*dp++) = dd;
			sd = sp[2];
			MLIB_V_IMAGESCALE_S16_S16(sd, vis_read_lo(ad),
			    vis_read_hi(adm), bdl, bdm, dd);
			(*dp++) = dd;
			sp += 3;
		}

/* last 4-pixel */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd = sp[0];
			MLIB_V_IMAGESCALE_S16_S16(sd, vis_read_hi(ad),
			    vis_read_lo(ad), bdh, bdl, dd);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd = sp[1];
				MLIB_V_IMAGESCALE_S16_S16(sd, vis_read_hi(adm),
				    vis_read_hi(ad), bdm, bdh, dd);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					sd = sp[2];
					MLIB_V_IMAGESCALE_S16_S16(sd,
					    vis_read_lo(ad), vis_read_hi(adm),
					    bdl, bdm, dd);
					emask = vis_edge16(dp, dend);
					vis_pst_16(dd, dp, emask);
				}
			}
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_S16_S16_3(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
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
	mlib_d64 *dp;

/* pointer to src line */
	mlib_s16 *sl;

/* pointer to dst line */
	mlib_s16 *dl;

/* pointer to end of dst */
	mlib_s16 *dend;

/* source data */
	mlib_d64 sd, s0, s1;

/* destination data */
	mlib_d64 dd;

/* alpha and beta data */
	mlib_d64 adm, bdm, a[5], b[6];

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* edge masks */
	mlib_s32 emask;

/* offset of address in dst */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;

/* prepare alpha and beta values */
	a[0] = a[3] = ad;
	vis_alignaddr((void *)0, 2);
	adm = vis_faligndata(ad, ad);
	a[1] = a[4] = vis_faligndata(ad, adm);
	b[0] = b[3] = bdh;
	b[1] = b[4] = bdl;
	vis_alignaddr((void *)0, 4);
	a[2] = vis_faligndata(ad, adm);
	b[2] = b[5] = vis_faligndata(bdh, bdl);

	sl = src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

/* prepare the alpha and beta */
		bdh = b[i = (-off) >> 1];
		bdl = b[i + 1];
		bdm = b[i + 2];
		ad = a[3 - i];
		adm = a[4 - i];

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* first 4-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s0 = vis_ld_d64_nf(sp);
			s1 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s0, s1);
			sp++;
			MLIB_V_IMAGESCALE_S16_S16(sd, vis_read_hi(ad),
			    vis_read_lo(ad), bdh, bdl, dd);
			emask = vis_edge16(dl, dend);
			vis_pst_16(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s0 = s1;
				s1 = vis_ld_d64_nf(sp + 1);
				sd = vis_faligndata(s0, s1);
				sp++;
				MLIB_V_IMAGESCALE_S16_S16(sd, vis_read_hi(adm),
				    vis_read_hi(ad), bdm, bdh, dd);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					s0 = s1;
					s1 = vis_ld_d64_nf(sp + 1);
					sd = vis_faligndata(s0, s1);
					sp++;
					MLIB_V_IMAGESCALE_S16_S16(sd,
					    vis_read_lo(ad), vis_read_hi(adm),
					    bdl, bdm, dd);
					emask = vis_edge16(dp, dend);
					vis_pst_16(dd, dp, emask);
					dp++;
				}
			}
		}

/* 4-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 2) / 24;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			mlib_d64 s2, s3;

			s0 = sp[0];
			s1 = sp[1];
			s2 = sp[2];
			s3 = vis_ld_d64_nf(sp + 3);
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGESCALE_S16_S16(sd, vis_read_hi(ad),
			    vis_read_lo(ad), bdh, bdl, dd);
			(*dp++) = dd;

			sd = vis_faligndata(s1, s2);
			MLIB_V_IMAGESCALE_S16_S16(sd, vis_read_hi(adm),
			    vis_read_hi(ad), bdm, bdh, dd);
			(*dp++) = dd;

			sd = vis_faligndata(s2, s3);
			MLIB_V_IMAGESCALE_S16_S16(sd, vis_read_lo(ad),
			    vis_read_hi(adm), bdl, bdm, dd);
			(*dp++) = dd;
			sp += 3;
		}

/* last 4-pixel */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s0 = vis_ld_d64_nf(sp);
			s1 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGESCALE_S16_S16(sd, vis_read_hi(ad),
			    vis_read_lo(ad), bdh, bdl, dd);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s0 = s1;
				s1 = vis_ld_d64_nf(sp + 2);
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_S16_S16(sd, vis_read_hi(adm),
				    vis_read_hi(ad), bdm, bdh, dd);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					sd = vis_faligndata(s1,
						vis_ld_d64_nf(sp + 3));
					MLIB_V_IMAGESCALE_S16_S16(sd,
					    vis_read_lo(ad), vis_read_hi(adm),
					    bdl, bdm, dd);
					emask = vis_edge16(dp, dend);
					vis_pst_16(dd, dp, emask);
				}
			}
		}

		sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

#define	MLIB_V_IMAGESCALE_U8_U8_D(sd, adh, adl, bd0, bd1,                \
	bd2, bd3, dd)                                                    \
	{                                                                \
	    mlib_d64 d01, d23, d45, d67;                                 \
	                                                                 \
	    d23 = vis_fpmerge(vis_read_hi(sd), vis_read_hi(sd));         \
	    d67 = vis_fpmerge(vis_read_lo(sd), vis_read_lo(sd));         \
	    d01 = vis_fmuld8ulx16(vis_read_hi(d23), vis_read_hi(adh));   \
	    d23 = vis_fmuld8ulx16(vis_read_lo(d23), vis_read_lo(adh));   \
	    d45 = vis_fmuld8ulx16(vis_read_hi(d67), vis_read_hi(adl));   \
	    d67 = vis_fmuld8ulx16(vis_read_lo(d67), vis_read_lo(adl));   \
	    d01 = vis_fpadd32(d01, bd0);                                 \
	    d23 = vis_fpadd32(d23, bd1);                                 \
	    d45 = vis_fpadd32(d45, bd2);                                 \
	    d67 = vis_fpadd32(d67, bd3);                                 \
	    dd = vis_fpack32(dd, d01);                                   \
	    dd = vis_fpack32(dd, d23);                                   \
	    dd = vis_fpack32(dd, d45);                                   \
	    dd = vis_fpack32(dd, d67);                                   \
	    dd = vis_fpmerge(vis_read_hi(dd), vis_read_lo(dd));          \
	}

/* *********************************************************** */

void
mlib_v_ImageScale_U8_U8_D_124_A8D1X8(
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
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* loop variables */
	mlib_s32 j;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

/* 8-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 8); j += 8) {
		sd = (*sp++);
		MLIB_V_IMAGESCALE_U8_U8_D(sd, ad, ad, bdh, bdl, bdh, bdl, dd);
		(*dp++) = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_U8_U8_D_124_A8D2(
    mlib_u8 *src,
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
	mlib_d64 *dp, *dl;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 8); i += 8) {
			sd = (*sp++);
			MLIB_V_IMAGESCALE_U8_U8_D(sd, ad, ad, bdh, bdl, bdh,
			    bdl, dd);
			(*dp++) = dd;
		}

/* last 8-pixel column loop */

		if (i < xsize) {
			sd = *sp;
			MLIB_V_IMAGESCALE_U8_U8_D(sd, ad, ad, bdh, bdl, bdh,
			    bdl, dd);
			emask = vis_edge8((void *)0, (void *)(xsize - i - 1));
			vis_pst_8(dd, dp, emask);
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_U8_U8_D_124(
    mlib_u8 *src,
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
	mlib_d64 *dp;

/* pointer to begin of a src line */
	mlib_u8 *sl;

/* pointer to begin of a dst line */
	mlib_u8 *dl;

/* pointer to end of a dst line */
	mlib_u8 *dend;

/* source data */
	mlib_d64 sd, s0, s1;

/* destination data */
	mlib_d64 dd;

/* alpha and beta data */
	mlib_d64 ad0, b[8];

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
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

/* prepare the alpha value */
		vis_alignaddr(dp, 2 * off);
		ad0 = vis_faligndata(ad, ad);

/* prepare the beta value */
		bdh = b[(-off) & 3];
		bdl = b[((-off) & 3) + 4];

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* first 8-pixel column */
		emask = vis_edge8(dl, dend);

		if (emask != 0xff) {
			s0 = vis_ld_d64_nf(sp);
			s1 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s0, s1);
			sp++;
			MLIB_V_IMAGESCALE_U8_U8_D(sd, ad0, ad0, bdh, bdl, bdh,
			    bdl, dd);
			vis_pst_8(dd, dp, emask);
			dp++;
		}

/* 8-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 1) >> 3;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s0 = sp[0];
			s1 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s0, s1);
			sp++;
			MLIB_V_IMAGESCALE_U8_U8_D(sd, ad0, ad0, bdh, bdl, bdh,
			    bdl, dd);
			(*dp++) = dd;
		}

/* last 8-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd = vis_faligndata(sp[0], vis_ld_d64_nf(sp + 1));
			MLIB_V_IMAGESCALE_U8_U8_D(sd, ad0, ad0, bdh, bdl, bdh,
			    bdl, dd);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_U8_U8_D_3_A8D1(
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

/* pointer to end of dst */
	mlib_u8 *dend;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* alpha data */
	mlib_d64 ad1, ad2;

/* beta data */
	mlib_d64 bdm;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 j;

/* prepare alpha and beta values */
	vis_alignaddr(dst, 2);
	ad2 = vis_faligndata(ad, ad);
	ad1 = vis_faligndata(ad, ad2);
	vis_alignaddr(dst, 4);
	ad2 = vis_faligndata(ad, ad1);
	bdm = vis_faligndata(bdh, bdl);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;
	dend = (mlib_u8 *)dp + dsize - 1;

/* 8-pixel loop */
/*
 * bad performance for loop - it's better to use 3 small loops
 *
 *  for (j = 0; j <= (dsize-24); j += 24) {
 *    sd = (*sp++);
 *    MLIB_V_IMAGESCALE_U8_U8_D(sd, ad, ad1, bdh, bdl, bdm, bdh, dd);
 *    (*dp++) = dd;
 *
 *    sd = (*sp++);
 *    MLIB_V_IMAGESCALE_U8_U8_D(sd, ad2, ad, bdl, bdm, bdh, bdl, dd);
 *    (*dp++) = dd;
 *
 *    sd = (*sp++);
 *    MLIB_V_IMAGESCALE_U8_U8_D(sd, ad1, ad2, bdm, bdh, bdl, bdm, dd);
 *    (*dp++) = dd;
 *  }
 *
 */

#pragma pipeloop(0)
	for (j = 0; j < dsize / 24; j++) {
		sd = sp[3 * j];
		MLIB_V_IMAGESCALE_U8_U8_D(sd, ad, ad1, bdh, bdl, bdm, bdh, dd);
		dp[3 * j] = dd;
	}

#pragma pipeloop(0)
	for (j = 0; j < dsize / 24; j++) {
		sd = sp[3 * j + 1];
		MLIB_V_IMAGESCALE_U8_U8_D(sd, ad2, ad, bdl, bdm, bdh, bdl, dd);
		dp[3 * j + 1] = dd;
	}

#pragma pipeloop(0)
	for (j = 0; j < dsize / 24; j++) {
		sd = sp[3 * j + 2];
		MLIB_V_IMAGESCALE_U8_U8_D(sd, ad1, ad2, bdm, bdh, bdl, bdm, dd);
		dp[3 * j + 2] = dd;
	}

	sp += 3 * j;
	dp += 3 * j;

/* last 8-pixel */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		sd = sp[0];
		MLIB_V_IMAGESCALE_U8_U8_D(sd, ad, ad1, bdh, bdl, bdm, bdh, dd);
		emask = vis_edge8(dp, dend);
		vis_pst_8(dd, dp, emask);
		dp++;

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd = sp[1];
			MLIB_V_IMAGESCALE_U8_U8_D(sd, ad2, ad, bdl, bdm, bdh,
			    bdl, dd);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd = sp[2];
				MLIB_V_IMAGESCALE_U8_U8_D(sd, ad1, ad2, bdm,
				    bdh, bdl, bdm, dd);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_U8_U8_D_3_A8D2(
    mlib_u8 *src,
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
	mlib_d64 *dp, *dl;

/* pointer to end of dst */
	mlib_u8 *dend;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* alpha data */
	mlib_d64 ad1, ad2;

/* beta data */
	mlib_d64 bdm;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;

/* prepare alpha and beta values */
	vis_alignaddr(dst, 2);
	ad2 = vis_faligndata(ad, ad);
	ad1 = vis_faligndata(ad, ad2);
	vis_alignaddr(dst, 4);
	ad2 = vis_faligndata(ad, ad1);
	bdm = vis_faligndata(bdh, bdl);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (i = 0; i < ysize; i++) {

		dend = (mlib_u8 *)dl + xsize - 1;

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (j = 0; j < xsize / 24; j++) {
			sd = sp[3 * j];
			MLIB_V_IMAGESCALE_U8_U8_D(sd, ad, ad1, bdh, bdl, bdm,
			    bdh, dd);
			dp[3 * j] = dd;
		}

#pragma pipeloop(0)
		for (j = 0; j < xsize / 24; j++) {
			sd = sp[3 * j + 1];
			MLIB_V_IMAGESCALE_U8_U8_D(sd, ad2, ad, bdl, bdm, bdh,
			    bdl, dd);
			dp[3 * j + 1] = dd;
		}

#pragma pipeloop(0)
		for (j = 0; j < xsize / 24; j++) {
			sd = sp[3 * j + 2];
			MLIB_V_IMAGESCALE_U8_U8_D(sd, ad1, ad2, bdm, bdh, bdl,
			    bdm, dd);
			dp[3 * j + 2] = dd;
		}

		sp += 3 * j;
		dp += 3 * j;

/* last 8-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd = sp[0];
			MLIB_V_IMAGESCALE_U8_U8_D(sd, ad, ad1, bdh, bdl, bdm,
			    bdh, dd);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd = sp[1];
				MLIB_V_IMAGESCALE_U8_U8_D(sd, ad2, ad, bdl, bdm,
				    bdh, bdl, dd);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					sd = sp[2];
					MLIB_V_IMAGESCALE_U8_U8_D(sd, ad1, ad2,
					    bdm, bdh, bdl, bdm, dd);
					emask = vis_edge8(dp, dend);
					vis_pst_8(dd, dp, emask);
				}
			}
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_U8_U8_D_3(
    mlib_u8 *src,
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
	mlib_d64 *dp;

/* pointer to src line */
	mlib_u8 *sl;

/* pointer to dst line */
	mlib_u8 *dl;

/* pointer to end of dst */
	mlib_u8 *dend;

/* source data */
	mlib_d64 sd, s0, s1;

/* destination data */
	mlib_d64 dd;

/* alpha data */
	mlib_d64 ad1, ad2, a[10];

/* beta data */
	mlib_d64 bdm, b[10];

/* edge masks */
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
	for (i = 0; i < ysize; i++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
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
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* first 8-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s0 = vis_ld_d64_nf(sp);
			s1 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s0, s1);
			sp++;
			MLIB_V_IMAGESCALE_U8_U8_D(sd, ad, ad1, bdh, bdl, bdm,
			    bdh, dd);
			emask = vis_edge8(dl, dend);
			vis_pst_8(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s0 = s1;
				s1 = vis_ld_d64_nf(sp + 1);
				sd = vis_faligndata(s0, s1);
				sp++;
				MLIB_V_IMAGESCALE_U8_U8_D(sd, ad2, ad, bdl, bdm,
				    bdh, bdl, dd);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					s0 = s1;
					s1 = vis_ld_d64_nf(sp + 1);
					sd = vis_faligndata(s0, s1);
					sp++;
					MLIB_V_IMAGESCALE_U8_U8_D(sd, ad1, ad2,
					    bdm, bdh, bdl, bdm, dd);
					emask = vis_edge8(dp, dend);
					vis_pst_8(dd, dp, emask);
					dp++;
				}
			}
		}

/* 8-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 1) / 24;
#pragma pipeloop(0)
		for (j = 0; j < n; j++) {
			s0 = sp[3 * j];
			s1 = sp[3 * j + 1];
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGESCALE_U8_U8_D(sd, ad, ad1, bdh, bdl, bdm,
			    bdh, dd);
			dp[3 * j] = dd;
		}

#pragma pipeloop(0)
		for (j = 0; j < n; j++) {
			s0 = sp[3 * j + 1];
			s1 = sp[3 * j + 2];
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGESCALE_U8_U8_D(sd, ad2, ad, bdl, bdm, bdh,
			    bdl, dd);
			dp[3 * j + 1] = dd;
		}

#pragma pipeloop(0)
		for (j = 0; j < n; j++) {
			s0 = sp[3 * j + 2];
			s1 = sp[3 * j + 3];
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGESCALE_U8_U8_D(sd, ad1, ad2, bdm, bdh, bdl,
			    bdm, dd);
			dp[3 * j + 2] = dd;
		}

		sp += 3 * n;
		dp += 3 * n;

/* last 8-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s0 = vis_ld_d64_nf(sp);
			s1 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGESCALE_U8_U8_D(sd, ad, ad1, bdh, bdl, bdm,
			    bdh, dd);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s0 = s1;
				s1 = vis_ld_d64_nf(sp + 2);
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_U8_U8_D(sd, ad2, ad, bdl, bdm,
				    bdh, bdl, dd);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					sd = vis_faligndata(s1,
						vis_ld_d64_nf(sp + 3));
					MLIB_V_IMAGESCALE_U8_U8_D(sd, ad1, ad2,
					    bdm, bdh, bdl, bdm, dd);
					emask = vis_edge8(dp, dend);
					vis_pst_8(dd, dp, emask);
				}
			}
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */
