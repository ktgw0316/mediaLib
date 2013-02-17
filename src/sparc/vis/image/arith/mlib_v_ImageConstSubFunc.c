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

#pragma ident	"@(#)mlib_v_ImageConstSubFunc.c	9.3	07/11/05 SMI"

/*
 * Internal function.
 *
 * void mlib_v_ImageConstSub_U8_124_A8D1X8(mlib_u8  *src,
 *                                         mlib_u8  *dst,
 *                                         mlib_s32 dsize,
 *                                         mlib_d64 dc);
 *
 * void mlib_v_ImageConstSub_U8_124_A8D2(mlib_u8  *src,
 *                                       mlib_s32 slb,
 *                                       mlib_u8  *dst,
 *                                       mlib_s32 dlb,
 *                                       mlib_s32 xsize,
 *                                       mlib_s32 ysize,
 *                                       mlib_d64 dc);
 *
 * void mlib_v_ImageConstSub_U8_124(mlib_u8  *src,
 *                                  mlib_s32 slb,
 *                                  mlib_u8  *dst,
 *                                  mlib_s32 dlb,
 *                                  mlib_s32 xsize,
 *                                  mlib_s32 ysize,
 *                                  mlib_d64 dc);
 *
 * void mlib_v_ImageConstSub_U8_3_A8D1(mlib_u8  *src,
 *                                     mlib_u8  *dst,
 *                                     mlib_s32 dsize,
 *                                     mlib_d64 dc);
 *
 * void mlib_v_ImageConstSub_U8_3_A8D2(mlib_u8  *src,
 *                                     mlib_s32 slb,
 *                                     mlib_u8  *dst,
 *                                     mlib_s32 dlb,
 *                                     mlib_s32 xsize,
 *                                     mlib_s32 ysize,
 *                                     mlib_d64 dc);
 *
 * void mlib_v_ImageConstSub_U8_3(mlib_u8  *src,
 *                                mlib_s32 slb,
 *                                mlib_u8  *dst,
 *                                mlib_s32 dlb,
 *                                mlib_s32 xsize,
 *                                mlib_s32 ysize,
 *                                mlib_d64 dc);
 *
 * void mlib_v_ImageConstSub_S16_124_A8D1X4(mlib_s16 *src,
 *                                          mlib_s16 *dst,
 *                                          mlib_s32 dsize,
 *                                          mlib_d64 dc);
 *
 * void mlib_v_ImageConstSub_S16_124_A8D2(mlib_s16 *src,
 *                                        mlib_s32 slb,
 *                                        mlib_s16 *dst,
 *                                        mlib_s32 dlb,
 *                                        mlib_s32 xsize,
 *                                        mlib_s32 ysize,
 *                                        mlib_d64 dc);
 *
 * void mlib_v_ImageConstSub_S16_124(mlib_s16 *src,
 *                                   mlib_s32 slb,
 *                                   mlib_s16 *dst,
 *                                   mlib_s32 dlb,
 *                                   mlib_s32 xsize,
 *                                   mlib_s32 ysize,
 *                                   mlib_d64 dc);
 *
 * void mlib_v_ImageConstSub_S16_3_A8D1(mlib_s16 *src,
 *                                      mlib_s16 *dst,
 *                                      mlib_s32 dsize,
 *                                      mlib_d64 dc);
 *
 * void mlib_v_ImageConstSub_S16_3_A8D2(mlib_s16 *src,
 *                                      mlib_s32 slb,
 *                                      mlib_s16 *dst,
 *                                      mlib_s32 dlb,
 *                                      mlib_s32 xsize,
 *                                      mlib_s32 ysize,
 *                                      mlib_d64 dc);
 *
 * void mlib_v_ImageConstSub_S16_3(mlib_s16 *src,
 *                                 mlib_s32 slb,
 *                                 mlib_s16 *dst,
 *                                 mlib_s32 dlb,
 *                                 mlib_s32 xsize,
 *                                 mlib_s32 ysize,
 *                                 mlib_d64 dc);
 *
 * void mlib_v_ImageConstSub_U16_124(mlib_s16 *src,
 *                                   mlib_s32 slb,
 *                                   mlib_s16 *dst,
 *                                   mlib_s32 dlb,
 *                                   mlib_s32 xsize,
 *                                   mlib_s32 ysize,
 *                                   mlib_d64 dc1,
 *                                   mlib_d64 dc2);
 *
 * void mlib_v_ImageConstSub_U16_3(mlib_s16 *src,
 *                                 mlib_s32 slb,
 *                                 mlib_s16 *dst,
 *                                 mlib_s32 dlb,
 *                                 mlib_s32 xsize,
 *                                 mlib_s32 ysize,
 *                                 mlib_d64 dc1,
 *                                 mlib_d64 dc2);
 *
 * void mlib_v_ImageConstSub_S32_124_A8D1X4(mlib_s32 *src,
 *                                          mlib_s32 *dst,
 *                                          mlib_s32 dsize,
 *                                          mlib_d64 dc1,
 *                                          mlib_d64 dc2);
 *
 * void mlib_v_ImageConstSub_S32_124_A8D2(mlib_s32 *src,
 *                                        mlib_s32 slb,
 *                                        mlib_s32 *dst,
 *                                        mlib_s32 dlb,
 *                                        mlib_s32 xsize,
 *                                        mlib_s32 ysize,
 *                                        mlib_d64 dc1,
 *                                        mlib_d64 dc2);
 *
 * void mlib_v_ImageConstSub_S32_124(mlib_s32 *src,
 *                                   mlib_s32 slb,
 *                                   mlib_s32 *dst,
 *                                   mlib_s32 dlb,
 *                                   mlib_s32 xsize,
 *                                   mlib_s32 ysize,
 *                                   mlib_d64 dc1,
 *                                   mlib_d64 dc2);
 *
 * void mlib_v_ImageConstSub_S32_3_A8D1(mlib_s32 *src,
 *                                      mlib_s32 *dst,
 *                                      mlib_s32 dsize,
 *                                      mlib_d64 dc1,
 *                                      mlib_d64 dc2);
 *
 * void mlib_v_ImageConstSub_S32_3_A8D2(mlib_s32 *src,
 *                                      mlib_s32 slb,
 *                                      mlib_s32 *dst,
 *                                      mlib_s32 dlb,
 *                                      mlib_s32 xsize,
 *                                      mlib_s32 ysize,
 *                                      mlib_d64 dc1,
 *                                      mlib_d64 dc2);
 *
 * void mlib_v_ImageConstSub_S32_3(mlib_s32 *src,
 *                                 mlib_s32 slb,
 *                                 mlib_s32 *dst,
 *                                 mlib_s32 dlb,
 *                                 mlib_s32 xsize,
 *                                 mlib_s32 ysize,
 *                                 mlib_d64 dc1,
 *                                 mlib_d64 dc2);
 *
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageConstSubFunc.h>

/* *********************************************************** */

#define	MLIB_V_IMAGECONSTSUB_U8(sd, cdh, cdl, dd)               \
	sdh = vis_fpmerge(fzero, vis_read_hi(sd));              \
	sdl = vis_fmul8x16al(vis_read_lo(sd), fone);            \
	rdh = vis_fpsub16(cdh, sdh);                            \
	rdl = vis_fpsub16(cdl, sdl);                            \
	dd = vis_fpack16_pair(rdh, rdl)

/* *********************************************************** */

void
mlib_v_ImageConstSub_U8_124_A8D1X8(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_d64 dc)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* source data used in macro */
	mlib_d64 sdh, sdl;

/* intermediate used in macro */
	mlib_d64 rdh, rdl;

/* loop variables */
	mlib_s32 j;
	mlib_f32 fone = vis_to_float(0x100), fzero = vis_to_float(0);

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

/* 8-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 8); j += 8) {
		sd = (*sp++);
		MLIB_V_IMAGECONSTSUB_U8(sd, dc, dc, dd);
		(*dp++) = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageConstSub_U8_124_A8D2(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dc)
{
/* aligned src address */
	mlib_d64 *sp, *sl;

/* aligned dst address */
	mlib_d64 *dp, *dl;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* source data used in macro */
	mlib_d64 sdh, sdl;

/* intermediate used in macro */
	mlib_d64 rdh, rdl;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;
	mlib_f32 fone = vis_to_float(0x100), fzero = vis_to_float(0);

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 8); i += 8) {
			sd = (*sp++);
			MLIB_V_IMAGECONSTSUB_U8(sd, dc, dc, dd);
			(*dp++) = dd;
		}

/* last 8-pixel column loop */

		if (i < xsize) {
			sd = (*sp++);
			MLIB_V_IMAGECONSTSUB_U8(sd, dc, dc, dd);
			emask = vis_edge8((void *)0, (void *)(xsize - i - 1));
			vis_pst_8(dd, dp, emask);
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageConstSub_U8_124(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dc)
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

/* variable for constant */
	mlib_d64 dc0;

/* source data used in macro */
	mlib_d64 sdh, sdl;

/* intermediate used in macro */
	mlib_d64 rdh, rdl;

/* edge mask */
	mlib_s32 emask;

/* offset in dst address */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;
	mlib_f32 fone = vis_to_float(0x100), fzero = vis_to_float(0);

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);

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
		dc0 = vis_faligndata(dc, dc);

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* generate edge mask for the start point */
		emask = vis_edge8(dl, dend);

/* first 8-pixel column */
		s0 = vis_ld_d64_nf(sp);
		s1 = vis_ld_d64_nf(sp + 1);
		sd = vis_faligndata(s0, s1);
		sp++;
		MLIB_V_IMAGECONSTSUB_U8(sd, dc0, dc0, dd);
		vis_pst_8(dd, dp, emask);
		dp++;

/* 8-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 1) >> 3;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s0 = vis_ld_d64_nf(sp);
			s1 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s0, s1);
			sp++;
			MLIB_V_IMAGECONSTSUB_U8(sd, dc0, dc0, dd);
			(*dp++) = dd;
		}

/* generate edge mask for the end point */

/* last 8-pixel column loop */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd = vis_faligndata(s1, vis_ld_d64_nf(sp + 1));
			MLIB_V_IMAGECONSTSUB_U8(sd, dc0, dc0, dd);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageConstSub_U8_3_A8D1(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_d64 dc)
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

/* variables for constant */
	mlib_d64 dc1, dc2;

/* source data used in macro */
	mlib_d64 sdh, sdl;

/* intermediate used in macro */
	mlib_d64 rdh, rdl;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 j;
	mlib_f32 fone = vis_to_float(0x100), fzero = vis_to_float(0);

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);

/* prepare the constant value */
	vis_alignaddr(dst, 2);
	dc2 = vis_faligndata(dc, dc);
	dc1 = vis_faligndata(dc, dc2);
	vis_alignaddr(dst, 4);
	dc2 = vis_faligndata(dc, dc1);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;
	dend = (mlib_u8 *)dp + dsize - 1;

/* 8-pixel column loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 24); j += 24) {
		sd = (*sp++);
		MLIB_V_IMAGECONSTSUB_U8(sd, dc, dc1, dd);
		(*dp++) = dd;

		sd = (*sp++);
		MLIB_V_IMAGECONSTSUB_U8(sd, dc2, dc, dd);
		(*dp++) = dd;

		sd = (*sp++);
		MLIB_V_IMAGECONSTSUB_U8(sd, dc1, dc2, dd);
		(*dp++) = dd;
	}

/* last 8-pixel */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		sd = sp[0];
		MLIB_V_IMAGECONSTSUB_U8(sd, dc, dc1, dd);
		emask = vis_edge8(dp, dend);
		vis_pst_8(dd, dp, emask);
		dp++;

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd = sp[1];
			MLIB_V_IMAGECONSTSUB_U8(sd, dc2, dc, dd);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd = sp[2];
				MLIB_V_IMAGECONSTSUB_U8(sd, dc1, dc2, dd);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageConstSub_U8_3_A8D2(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dc)
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

/* variables for constant */
	mlib_d64 dc1, dc2;

/* source data used in macro */
	mlib_d64 sdh, sdl;

/* intermediate used in macro */
	mlib_d64 rdh, rdl;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;
	mlib_f32 fone = vis_to_float(0x100), fzero = vis_to_float(0);

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);

/* prepare the constant value */
	vis_alignaddr(dst, 2);
	dc2 = vis_faligndata(dc, dc);
	dc1 = vis_faligndata(dc, dc2);
	vis_alignaddr(dst, 4);
	dc2 = vis_faligndata(dc, dc1);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

		dend = (mlib_u8 *)dl + xsize - 1;

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 24); i += 24) {
			sd = (*sp++);
			MLIB_V_IMAGECONSTSUB_U8(sd, dc, dc1, dd);
			(*dp++) = dd;

			sd = (*sp++);
			MLIB_V_IMAGECONSTSUB_U8(sd, dc2, dc, dd);
			(*dp++) = dd;

			sd = (*sp++);
			MLIB_V_IMAGECONSTSUB_U8(sd, dc1, dc2, dd);
			(*dp++) = dd;
		}

/* last 8-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd = sp[0];
			MLIB_V_IMAGECONSTSUB_U8(sd, dc, dc1, dd);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd = sp[1];
				MLIB_V_IMAGECONSTSUB_U8(sd, dc2, dc, dd);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					sd = sp[2];
					MLIB_V_IMAGECONSTSUB_U8(sd, dc1, dc2,
					    dd);
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
mlib_v_ImageConstSub_U8_3(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dc)
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

/* variables for constant */
	mlib_d64 dc1, dc2, cds[3];

/* source data used in macro */
	mlib_d64 sdh, sdl;

/* intermediate used in macro */
	mlib_d64 rdh, rdl;

/* edge mask */
	mlib_s32 emask;

/* offset in dst address */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;
	mlib_f32 fone = vis_to_float(0x100), fzero = vis_to_float(0);

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);

/* prepare the constant value */
	vis_alignaddr((void *)0, 2);
	dc2 = vis_faligndata(dc, dc);
	cds[1] = dc1 = vis_faligndata(dc, dc2);
	vis_alignaddr((void *)0, 4);
	cds[0] = dc;
	cds[2] = vis_faligndata(dc, dc1);

	sl = src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

/* prepare the constants */
		vis_alignaddr(dp, 6 + 2 * off);
		dc = vis_faligndata(cds[0], cds[1]);
		dc1 = vis_faligndata(cds[1], cds[2]);
		dc2 = vis_faligndata(cds[2], cds[0]);

		if (off <= -4) {
			dd = dc;
			dc = dc2;
			dc2 = dc1;
			dc1 = dd;
		}

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
			MLIB_V_IMAGECONSTSUB_U8(sd, dc, dc1, dd);
			vis_pst_8(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s0 = s1;
				s1 = vis_ld_d64_nf(sp + 1);
				sd = vis_faligndata(s0, s1);
				sp++;
				MLIB_V_IMAGECONSTSUB_U8(sd, dc2, dc, dd);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					s0 = s1;
					s1 = vis_ld_d64_nf(sp + 1);
					sd = vis_faligndata(s0, s1);
					sp++;
					MLIB_V_IMAGECONSTSUB_U8(sd, dc1, dc2,
					    dd);
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
			MLIB_V_IMAGECONSTSUB_U8(sd, dc, dc1, dp[0]);

			s0 = s1;
			s1 = sp[2];
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGECONSTSUB_U8(sd, dc2, dc, dp[1]);

			s0 = s1;
			s1 = vis_ld_d64_nf(sp + 3);
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGECONSTSUB_U8(sd, dc1, dc2, dp[2]);
			sp += 3;
			dp += 3;
		}

/* last 8-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s0 = sp[0];
			s1 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGECONSTSUB_U8(sd, dc, dc1, dd);
			emask = vis_edge8(dp, dend);
			vis_pst_8(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s0 = s1;
				s1 = vis_ld_d64_nf(sp + 2);
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGECONSTSUB_U8(sd, dc2, dc, dd);
				emask = vis_edge8(dp, dend);
				vis_pst_8(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					s0 = s1;
					s1 = vis_ld_d64_nf(sp + 3);
					sd = vis_faligndata(s0, s1);
					MLIB_V_IMAGECONSTSUB_U8(sd, dc1, dc2,
					    dd);
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

#define	MLIB_V_IMAGECONSTSUB_S16(sd, cd, dd)	dd = vis_fpsub16(cd, sd)

/* *********************************************************** */

void
mlib_v_ImageConstSub_S16_124_A8D1X4(
    mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_d64 dc)
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

/* 4-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 4); j += 4) {
		sd = (*sp++);
		MLIB_V_IMAGECONSTSUB_S16(sd, dc, dd);
		(*dp++) = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageConstSub_S16_124_A8D2(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dc)
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

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 4); i += 4) {
			sd = (*sp++);
			MLIB_V_IMAGECONSTSUB_S16(sd, dc, dd);
			(*dp++) = dd;
		}

/* last 4-pixel column */

		if (i < xsize) {
			sd = (*sp++);
			MLIB_V_IMAGECONSTSUB_S16(sd, dc, dd);
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
mlib_v_ImageConstSub_S16_124(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dc)
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

/* constant variable */
	mlib_d64 dc0;

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
		vis_alignaddr(dp, off);
		dc0 = vis_faligndata(dc, dc);

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* generate edge mask for the start point */
		emask = vis_edge16(dl, dend);

/* first 4-pixel column */
		s0 = vis_ld_d64_nf(sp);
		s1 = vis_ld_d64_nf(sp + 1);
		sd = vis_faligndata(s0, s1);
		sp++;
		MLIB_V_IMAGECONSTSUB_S16(sd, dc0, dd);
		vis_pst_16(dd, dp, emask);
		dp++;

/* 4-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 2) >> 3;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s0 = s1;
			s1 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s0, s1);
			sp++;
			MLIB_V_IMAGECONSTSUB_S16(sd, dc0, dd);
			(*dp++) = dd;
		}

/* last 4-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd = vis_faligndata(s1, vis_ld_d64_nf(sp + 1));
			MLIB_V_IMAGECONSTSUB_S16(sd, dc0, dd);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
		}

		sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageConstSub_S16_3_A8D1(
    mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_d64 dc)
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

/* constant variables */
	mlib_d64 dc1, dc2;

/* edge mask */
	mlib_s32 emask;

/* loop variable */
	mlib_s32 j;

/* prepare the constant value */
	vis_alignaddr(dst, 2);
	dc2 = vis_faligndata(dc, dc);
	dc1 = vis_faligndata(dc, dc2);
	vis_alignaddr(dst, 4);
	dc2 = vis_faligndata(dc, dc1);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

	dend = (mlib_s16 *)dp + dsize - 1;

/* 4-pixel column loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 12); j += 12) {
		sd = sp[0];
		MLIB_V_IMAGECONSTSUB_S16(sd, dc, dp[0]);
		sd = sp[1];
		MLIB_V_IMAGECONSTSUB_S16(sd, dc1, dp[1]);
		sd = sp[2];
		MLIB_V_IMAGECONSTSUB_S16(sd, dc2, dp[2]);
		sp += 3;
		dp += 3;
	}

/* last 4-pixel column */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		sd = sp[0];
		MLIB_V_IMAGECONSTSUB_S16(sd, dc, dd);
		emask = vis_edge16(dp, dend);
		vis_pst_16(dd, dp, emask);
		dp++;

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd = sp[1];
			MLIB_V_IMAGECONSTSUB_S16(sd, dc1, dd);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd = sp[2];
				MLIB_V_IMAGECONSTSUB_S16(sd, dc2, dd);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageConstSub_S16_3_A8D2(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dc)
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

/* constant variables */
	mlib_d64 dc1, dc2;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;

/* prepare the constant value */
	vis_alignaddr(dst, 2);
	dc2 = vis_faligndata(dc, dc);
	dc1 = vis_faligndata(dc, dc2);
	vis_alignaddr(dst, 4);
	dc2 = vis_faligndata(dc, dc1);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

		dend = (mlib_s16 *)dl + xsize - 1;

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 12); i += 12) {
			sd = sp[0];
			MLIB_V_IMAGECONSTSUB_S16(sd, dc, dp[0]);
			sd = sp[1];
			MLIB_V_IMAGECONSTSUB_S16(sd, dc1, dp[1]);
			sd = sp[2];
			MLIB_V_IMAGECONSTSUB_S16(sd, dc2, dp[2]);
			sp += 3;
			dp += 3;
		}

/* last 4-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd = sp[0];
			MLIB_V_IMAGECONSTSUB_S16(sd, dc, dd);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd = sp[1];
				MLIB_V_IMAGECONSTSUB_S16(sd, dc1, dd);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					sd = sp[2];
					MLIB_V_IMAGECONSTSUB_S16(sd, dc2, dd);
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
mlib_v_ImageConstSub_S16_3(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dc)
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

/* constant variables */
	mlib_d64 dc1, dc2, cds[3];

/* edge masks */
	mlib_s32 emask;

/* offset of address in dst */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;

/* prepare the constant value */
	vis_alignaddr((void *)0, 2);
	dc2 = vis_faligndata(dc, dc);
	cds[1] = dc1 = vis_faligndata(dc, dc2);
	vis_alignaddr((void *)0, 4);
	cds[0] = dc;
	cds[2] = vis_faligndata(dc, dc1);

	sl = src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

/* prepare the constants */
		vis_alignaddr(dp, 6 + off);
		dc = vis_faligndata(cds[0], cds[1]);
		dc1 = vis_faligndata(cds[1], cds[2]);
		dc2 = vis_faligndata(cds[2], cds[0]);

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* generate edge mask for the start point */
		emask = vis_edge16(dl, dend);

/* first 4-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s0 = vis_ld_d64_nf(sp);
			s1 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s0, s1);
			sp++;
			MLIB_V_IMAGECONSTSUB_S16(sd, dc, dd);
			vis_pst_16(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s0 = s1;
				s1 = vis_ld_d64_nf(sp + 1);
				sd = vis_faligndata(s0, s1);
				sp++;
				MLIB_V_IMAGECONSTSUB_S16(sd, dc1, dd);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					s0 = s1;
					s1 = vis_ld_d64_nf(sp + 1);
					sd = vis_faligndata(s0, s1);
					sp++;
					MLIB_V_IMAGECONSTSUB_S16(sd, dc2, dd);
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
			s0 = sp[0];
			s1 = sp[1];
			sp++;
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGECONSTSUB_S16(sd, dc, dp[0]);

			s0 = s1;
			s1 = sp[1];
			sp++;
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGECONSTSUB_S16(sd, dc1, dp[1]);

			s0 = s1;
			s1 = vis_ld_d64_nf(sp + 1);
			sp++;
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGECONSTSUB_S16(sd, dc2, dp[2]);
			dp += 3;
		}

/* last 4-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s0 = sp[0];
			s1 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGECONSTSUB_S16(sd, dc, dd);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s0 = s1;
				s1 = vis_ld_d64_nf(sp + 2);
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGECONSTSUB_S16(sd, dc1, dd);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					sd = vis_faligndata(s1,
						vis_ld_d64_nf(sp + 3));
					MLIB_V_IMAGECONSTSUB_S16(sd, dc2, dd);
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

#define	MLIB_V_IMAGECONSTSUB_U16(sd, dc1, dc2, dd)              \
	sd = vis_fxor(sd, mask_8000);                           \
	tmp = vis_fmuld8ulx16(ftwo, vis_read_hi(sd));           \
	rdh = vis_fpsub32(dc1, tmp);                            \
	tmp = vis_fmuld8ulx16(ftwo, vis_read_lo(sd));           \
	rdl = vis_fpsub32(dc2, tmp);                            \
	dd = vis_fpackfix_pair(rdh, rdl);                       \
	dd = vis_fxor(dd, mask_8000)

/* *********************************************************** */

void
mlib_v_ImageConstSub_U16_124(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dc1,
    mlib_d64 dc2)
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
	mlib_d64 dc[8];

/* temporaries used in macro */
	mlib_d64 rdh, rdl, tmp;

/* edge mask */
	mlib_s32 emask;

/* offset in dst address */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;
	mlib_d64 mask_8000 = vis_to_double_dup(0x80008000);
	mlib_f32 ftwo = vis_to_float(0x00020002);

/* initialize GSR scale factor */
	vis_write_gsr(15 << 3);

/* prepare the const values */
	dc[0] = dc[6] = dc1;
	dc[2] = dc[4] = dc2;
	vis_alignaddr((void *)0, 4);
	dc[3] = dc[5] = vis_faligndata(dc1, dc2);
	dc[1] = dc[7] = vis_faligndata(dc2, dc1);

	if ((slb == dlb) && (dlb == 2 * xsize)) {
		xsize *= ysize;
		ysize = 1;
	}

	sl = src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

		dc1 = dc[i = (-off) >> 1];
		dc2 = dc[i + 4];

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

		if (((mlib_addr)((mlib_u8 *)sl + off) & 7) == 0) {

/* generate edge mask for the start point */
			emask = vis_edge16(dl, dend);

/* first 4-pixel column */

			if (emask != 0xf) {
				sd = sp[0];
				sp++;
				MLIB_V_IMAGECONSTSUB_U16(sd, dc1, dc2, dd);
				vis_pst_16(dd, dp, emask);
				dp++;
			}

/* 4-pixel column loop */
			n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 2) >> 3;
#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				sd = sp[0];
				sp++;
				MLIB_V_IMAGECONSTSUB_U16(sd, dc1, dc2, dd);
				*dp = dd;
				dp++;
			}

/* 4-pixel column */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd = sp[0];
				MLIB_V_IMAGECONSTSUB_U16(sd, dc1, dc2, dd);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
			}

		} else {

/* generate edge mask for the start point */
			emask = vis_edge16(dl, dend);

/* first 4-pixel column */

			if (emask != 0xf) {
				s0 = vis_ld_d64_nf(sp);
				s1 = vis_ld_d64_nf(sp + 1);
				sd = vis_faligndata(s0, s1);
				sp++;
				MLIB_V_IMAGECONSTSUB_U16(sd, dc1, dc2, dd);
				vis_pst_16(dd, dp, emask);
				dp++;
			}

/* 4-pixel column loop */
			n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 2) >> 3;
			s0 = vis_ld_d64_nf(sp);
			sp++;
#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				s1 = *sp;
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGECONSTSUB_U16(sd, dc1, dc2, dd);
				s0 = s1;
				*dp = dd;
				dp++;
				sp++;
			}

/* 4-pixel column */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s1 = vis_ld_d64_nf(sp);
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGECONSTSUB_U16(sd, dc1, dc2, dd);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
			}
		}

		sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageConstSub_U16_3(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dc1,
    mlib_d64 dc2)
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
	mlib_d64 dc[6], dc3;

/* temporaries used in macro */
	mlib_d64 rdh, rdl, tmp;

/* edge mask */
	mlib_s32 emask;

/* offset in dst address */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;
	mlib_d64 mask_8000 = vis_to_double_dup(0x80008000);
	mlib_f32 ftwo = vis_to_float(0x00020002);

/* initialize GSR scale factor */
	vis_write_gsr(15 << 3);

/* prepare the const values */
	dc[0] = dc[3] = dc1;
	dc[1] = dc[4] = dc2;
	vis_alignaddr((void *)0, 4);
	dc[2] = dc[5] = vis_faligndata(dc1, dc2);

	sl = src;
	dl = dst;

	if ((slb == dlb) && (dlb == 2 * xsize)) {
		xsize *= ysize;
		ysize = 1;
	}

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

		dc1 = dc[i = (-off) >> 1];
		dc2 = dc[i + 1];
		dc3 = dc[i + 2];

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

		if (((mlib_addr)((mlib_u8 *)sl + off) & 7) == 0) {

/* first 4-pixel column */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd = sp[0];
				sp++;
				MLIB_V_IMAGECONSTSUB_U16(sd, dc1, dc2, dd);
				emask = vis_edge16(dl, dend);
				vis_pst_16(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					sd = sp[0];
					sp++;
					MLIB_V_IMAGECONSTSUB_U16(sd, dc3, dc1,
					    dd);
					emask = vis_edge16(dp, dend);
					vis_pst_16(dd, dp, emask);
					dp++;

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						sd = sp[0];
						sp++;
						MLIB_V_IMAGECONSTSUB_U16(sd,
						    dc2, dc3, dd);
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
				sd = sp[0];
				MLIB_V_IMAGECONSTSUB_U16(sd, dc1, dc2, dd);
				*dp = dd;
				dp++;

				sd = sp[1];
				MLIB_V_IMAGECONSTSUB_U16(sd, dc3, dc1, dd);
				*dp = dd;
				dp++;

				sd = sp[2];
				MLIB_V_IMAGECONSTSUB_U16(sd, dc2, dc3, dd);
				*dp = dd;
				dp++;
				sp += 3;
			}

/* last 4-pixel */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd = sp[0];
				MLIB_V_IMAGECONSTSUB_U16(sd, dc1, dc2, dd);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					sd = sp[1];
					MLIB_V_IMAGECONSTSUB_U16(sd, dc3, dc1,
					    dd);
					emask = vis_edge16(dp, dend);
					vis_pst_16(dd, dp, emask);
					dp++;

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						sd = sp[2];
						MLIB_V_IMAGECONSTSUB_U16(sd,
						    dc2, dc3, dd);
						emask = vis_edge16(dp, dend);
						vis_pst_16(dd, dp, emask);
					}
				}
			}

		} else {

/* first 4-pixel column */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s0 = vis_ld_d64_nf(sp);
				s1 = vis_ld_d64_nf(sp + 1);
				sd = vis_faligndata(s0, s1);
				sp++;
				MLIB_V_IMAGECONSTSUB_U16(sd, dc1, dc2, dd);
				emask = vis_edge16(dl, dend);
				vis_pst_16(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					s0 = s1;
					s1 = vis_ld_d64_nf(sp + 1);
					sd = vis_faligndata(s0, s1);
					sp++;
					MLIB_V_IMAGECONSTSUB_U16(sd, dc3, dc1,
					    dd);
					emask = vis_edge16(dp, dend);
					vis_pst_16(dd, dp, emask);
					dp++;

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						s0 = s1;
						s1 = vis_ld_d64_nf(sp + 1);
						sd = vis_faligndata(s0, s1);
						sp++;
						MLIB_V_IMAGECONSTSUB_U16(sd,
						    dc2, dc3, dd);
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
				s0 = sp[3 * i];
				s1 = sp[3 * i + 1];
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGECONSTSUB_U16(sd, dc1, dc2, dd);
				dp[3 * i] = dd;
			}

#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				s0 = sp[3 * i + 1];
				s1 = sp[3 * i + 2];
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGECONSTSUB_U16(sd, dc3, dc1, dd);
				dp[3 * i + 1] = dd;
			}

#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				s0 = sp[3 * i + 2];
				s1 = sp[3 * i + 3];
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGECONSTSUB_U16(sd, dc2, dc3, dd);
				dp[3 * i + 2] = dd;
			}

			sp += 3 * i;
			dp += 3 * i;

/* last 4-pixel */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s0 = sp[0];
				s1 = vis_ld_d64_nf(sp + 1);
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGECONSTSUB_U16(sd, dc1, dc2, dd);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					s0 = s1;
					s1 = vis_ld_d64_nf(sp + 2);
					sd = vis_faligndata(s0, s1);
					MLIB_V_IMAGECONSTSUB_U16(sd, dc3, dc1,
					    dd);
					emask = vis_edge16(dp, dend);
					vis_pst_16(dd, dp, emask);
					dp++;

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						sd = vis_faligndata(s1,
							vis_ld_d64_nf(sp + 3));
						MLIB_V_IMAGECONSTSUB_U16(sd,
						    dc2, dc3, dd);
						emask = vis_edge16(dp, dend);
						vis_pst_16(dd, dp, emask);
					}
				}
			}
		}

		sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

#define	MLIB_V_IMAGECONSTSUB_S32(sd, cd, dd)	dd = vis_fpsub32(cd, sd)

/* *********************************************************** */

void
mlib_v_ImageConstSub_S32_124_A8D1X4(
    mlib_s32 *src,
    mlib_s32 *dst,
    mlib_s32 dsize,
    mlib_d64 dc1,
    mlib_d64 dc2)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd1, sd2;

/* destination data */
	mlib_d64 dd1, dd2;

/* loop variables */
	mlib_s32 j;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

/* 4-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 4); j += 4) {
		sd1 = (*sp++);
		sd2 = (*sp++);
		MLIB_V_IMAGECONSTSUB_S32(sd1, dc1, dd1);
		MLIB_V_IMAGECONSTSUB_S32(sd2, dc2, dd2);
		(*dp++) = dd1;
		(*dp++) = dd2;
	}
}

/* *********************************************************** */

void
mlib_v_ImageConstSub_S32_124_A8D2(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dc1,
    mlib_d64 dc2)
{
/* aligned src address */
	mlib_d64 *sp, *sl;

/* aligned dst address */
	mlib_d64 *dp, *dl;

/* source data */
	mlib_d64 sd1, sd2;

/* destination data */
	mlib_d64 dd1, dd2;

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
			sd1 = (*sp++);
			sd2 = (*sp++);
			MLIB_V_IMAGECONSTSUB_S32(sd1, dc1, dd1);
			MLIB_V_IMAGECONSTSUB_S32(sd2, dc2, dd2);
			(*dp++) = dd1;
			(*dp++) = dd2;
		}

/* last 4-pixel column */

		if (i < xsize) {
			sd1 = sp[0];
			MLIB_V_IMAGECONSTSUB_S32(sd1, dc1, dd1);

			if (i <= (xsize - 3)) {
				sd2 = sp[1];
				MLIB_V_IMAGECONSTSUB_S32(sd2, dc2, dd2);
				(*dp++) = dd1;
				vis_pst_32(dd2, dp, 2);
			} else {
				emask =
				    vis_edge32((void *)0,
				    (void *)(4 * (xsize - i - 1)));
				vis_pst_32(dd1, dp, emask);
			}
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageConstSub_S32_124(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dc1,
    mlib_d64 dc2)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to begin of a src line */
	mlib_s32 *sl;

/* pointer to begin of a dst line */
	mlib_s32 *dl;

/* pointer to end of a dst line */
	mlib_s32 *dend;

/* source data */
	mlib_d64 sd1, sd2, s0, s1, s2;

/* destination data */
	mlib_d64 dd1, dd2;

/* constant variable */
	mlib_d64 dc_0, dc_1;

/* edge mask */
	mlib_s32 emask;

/* offset in dst address */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;

	sl = src;
	dl = dst;

/* prepare the constant value */
	vis_alignaddr((void *)0, 4);
	dc_0 = vis_faligndata(dc2, dc1);
	dc_1 = vis_faligndata(dc1, dc2);

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

/* prepare the constant value */
		vis_alignaddr(dp, 4 + off);
		dc1 = vis_faligndata(dc_0, dc_1);
		dc2 = vis_faligndata(dc_1, dc_0);

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* generate edge mask for the start point */
		emask = vis_edge32(dl, dend);

/* first 4-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s0 = vis_ld_d64_nf(sp);
			s1 = vis_ld_d64_nf(sp + 1);
			sd1 = vis_faligndata(s0, s1);
			sp++;
			MLIB_V_IMAGECONSTSUB_S32(sd1, dc1, dd1);
			vis_pst_32(dd1, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s2 = vis_ld_d64_nf(sp + 1);
				sd2 = vis_faligndata(s1, s2);
				sp++;
				MLIB_V_IMAGECONSTSUB_S32(sd2, dc2, dd2);
				emask = vis_edge32(dp, dend);
				vis_pst_32(dd2, dp, emask);
				dp++;
			}
		}

/* 4-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 4) >> 4;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s0 = s2;
			s1 = sp[1];
			s2 = vis_ld_d64_nf(sp + 2);
			sd1 = vis_faligndata(s0, s1);
			sd2 = vis_faligndata(s1, s2);
			sp += 2;
			MLIB_V_IMAGECONSTSUB_S32(sd1, dc1, dd1);
			MLIB_V_IMAGECONSTSUB_S32(sd2, dc2, dd2);
			(*dp++) = dd1;
			(*dp++) = dd2;
		}

/* last 4-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s1 = vis_ld_d64_nf(sp + 1);
			sd1 = vis_faligndata(s2, s1);
			MLIB_V_IMAGECONSTSUB_S32(sd1, dc1, dd1);
			emask = vis_edge32(dp, dend);
			vis_pst_32(dd1, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s2 = vis_ld_d64_nf(sp + 2);
				sd2 = vis_faligndata(s1, s2);
				MLIB_V_IMAGECONSTSUB_S32(sd2, dc2, dd2);
				emask = vis_edge32(dp, dend);
				vis_pst_32(dd2, dp, emask);
			}
		}

		sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
		dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageConstSub_S32_3_A8D1(
    mlib_s32 *src,
    mlib_s32 *dst,
    mlib_s32 dsize,
    mlib_d64 dc1,
    mlib_d64 dc2)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to end of dst */
	mlib_s32 *dend;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* constant variables */
	mlib_d64 dc3;

/* edge mask */
	mlib_s32 emask;

/* loop variable */
	mlib_s32 j;

/* prepare the constant value */
	vis_alignaddr(dst, 4);
	dc3 = vis_faligndata(dc1, dc2);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

	dend = (mlib_s32 *)dp + dsize - 1;

/* 6-pixel column loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 6); j += 6) {
		sd = sp[0];
		MLIB_V_IMAGECONSTSUB_S32(sd, dc1, dp[0]);
		sd = sp[1];
		MLIB_V_IMAGECONSTSUB_S32(sd, dc2, dp[1]);
		sd = sp[2];
		MLIB_V_IMAGECONSTSUB_S32(sd, dc3, dp[2]);
		sp += 3;
		dp += 3;
	}

/* last 3-pixel column */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		sd = sp[0];
		MLIB_V_IMAGECONSTSUB_S32(sd, dc1, dp[0]);
		sd = sp[1];
		dp++;
		MLIB_V_IMAGECONSTSUB_S32(sd, dc2, dd);
		emask = vis_edge32(dp, dend);
		vis_pst_32(dd, dp, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageConstSub_S32_3_A8D2(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dc1,
    mlib_d64 dc2)
{
/* aligned src address */
	mlib_d64 *sp, *sl;

/* aligned dst address */
	mlib_d64 *dp, *dl;

/* pointer to end of dst */
	mlib_s32 *dend;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* constant variables */
	mlib_d64 dc3;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;

/* prepare the constant value */
	vis_alignaddr(dst, 4);
	dc3 = vis_faligndata(dc1, dc2);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

		dend = (mlib_s32 *)dl + xsize - 1;

/* 6-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 6); i += 6) {
			sd = sp[0];
			MLIB_V_IMAGECONSTSUB_S32(sd, dc1, dp[0]);
			sd = sp[1];
			MLIB_V_IMAGECONSTSUB_S32(sd, dc2, dp[1]);
			sd = sp[2];
			MLIB_V_IMAGECONSTSUB_S32(sd, dc3, dp[2]);
			sp += 3;
			dp += 3;
		}

/* last 3-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd = sp[0];
			MLIB_V_IMAGECONSTSUB_S32(sd, dc1, dp[0]);
			dp++;
			sd = sp[1];
			MLIB_V_IMAGECONSTSUB_S32(sd, dc2, dd);
			emask = vis_edge32(dp, dend);
			vis_pst_32(dd, dp, emask);
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageConstSub_S32_3(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dc1,
    mlib_d64 dc2)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to src line */
	mlib_s32 *sl;

/* pointer to dst line */
	mlib_s32 *dl;

/* pointer to end of dst */
	mlib_s32 *dend;

/* source data */
	mlib_d64 sd, s0, s1;

/* destination data */
	mlib_d64 dd;

/* constant variables */
	mlib_d64 dc, cds[3];

/* edge masks */
	mlib_s32 emask;

/* offset of address in dst */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;

/* prepare the constant value */
	cds[0] = dc1;
	cds[1] = dc2;
	vis_alignaddr((void *)0, 4);
	cds[2] = vis_faligndata(dc1, dc2);

	sl = src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

/* prepare the constants */
		vis_alignaddr(dp, 4 + off);
		dc = vis_faligndata(cds[1], cds[2]);
		dc1 = vis_faligndata(cds[2], cds[0]);
		dc2 = vis_faligndata(cds[0], cds[1]);

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* generate edge mask for the start point */
		emask = vis_edge32(dl, dend);

/* first 4-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s0 = vis_ld_d64_nf(sp);
			s1 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s0, s1);
			sp++;
			MLIB_V_IMAGECONSTSUB_S32(sd, dc, dd);
			vis_pst_32(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s0 = s1;
				s1 = vis_ld_d64_nf(sp + 1);
				sd = vis_faligndata(s0, s1);
				sp++;
				MLIB_V_IMAGECONSTSUB_S32(sd, dc1, dd);
				emask = vis_edge32(dp, dend);
				vis_pst_32(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					s0 = s1;
					s1 = vis_ld_d64_nf(sp + 1);
					sd = vis_faligndata(s0, s1);
					sp++;
					MLIB_V_IMAGECONSTSUB_S32(sd, dc2, dd);
					emask = vis_edge32(dp, dend);
					vis_pst_32(dd, dp, emask);
					dp++;
				}
			}
		}

/* 6-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 4) / 24;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s0 = sp[0];
			s1 = sp[1];
			sp++;
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGECONSTSUB_S32(sd, dc, dp[0]);

			s0 = s1;
			s1 = sp[1];
			sp++;
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGECONSTSUB_S32(sd, dc1, dp[1]);

			s0 = s1;
			s1 = vis_ld_d64_nf(sp + 1);
			sp++;
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGECONSTSUB_S32(sd, dc2, dp[2]);
			dp += 3;
		}

/* last 3-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s0 = sp[0];
			s1 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGECONSTSUB_S32(sd, dc, dd);
			emask = vis_edge32(dp, dend);
			vis_pst_32(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s0 = s1;
				s1 = vis_ld_d64_nf(sp + 2);
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGECONSTSUB_S32(sd, dc1, dd);
				emask = vis_edge32(dp, dend);
				vis_pst_32(dd, dp, emask);
			}
		}

		sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
		dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */
