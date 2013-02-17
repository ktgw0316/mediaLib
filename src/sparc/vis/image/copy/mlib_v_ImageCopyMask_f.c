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

#pragma ident	"@(#)mlib_v_ImageCopyMask_f.c	9.3	07/11/05 SMI"

/*
 * DESCRIPTION
 *      Copy one image to another via a mask.
 *        dst(i, j) = src(i, j) when mask(i, j) <= *thresh, or
 *                   dst(i, j) when mask(i, j) >  *thresh
 *        -- VIS version low level functions.
 *
 * NOTE
 *        These functions are separated from mlib_v_ImageCopyMask.c for
 *        loop unrolling and structure clarity.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageCopyMask_f.h>

/* *********************************************************** */

#define	MLIB_V_IMAGECOPYMASK_U8(md, tdh, tdl)                   \
	mdh = vis_fpmerge(fzero, vis_read_hi(md));              \
	mdl = vis_fpmerge(fzero, vis_read_lo(md));              \
	cmaskh = vis_fcmplt16(mdh, tdh);                        \
	cmaskl = vis_fcmplt16(mdl, tdl);                        \
	cmask = (cmaskh << 4) | cmaskl

/* *********************************************************** */

void
mlib_v_ImageCopyMask_U8_124_A8D1X8(
    mlib_u8 *src,
    mlib_u8 *mask,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_d64 dthresh)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned mask address */
	mlib_d64 *mp;

/* aligned dst address */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd;

/* mask data */
	mlib_d64 md, mdh, mdl;

/* comparison masks */
	mlib_s32 cmask, cmaskh, cmaskl;

/* loop variables */
	mlib_s32 j;
	mlib_f32 fzero = vis_fzeros();

	sp = (mlib_d64 *)src;
	mp = (mlib_d64 *)mask;
	dp = (mlib_d64 *)dst;

/* 8-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 8); j += 8) {
		sd = (*sp++);
		md = (*mp++);
		MLIB_V_IMAGECOPYMASK_U8(md, dthresh, dthresh);
		vis_pst_8(sd, dp, cmask);
		dp++;
	}
}

/* *********************************************************** */

void
mlib_v_ImageCopyMask_U8_124_A8D2(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *mask,
    mlib_s32 mlb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh)
{
/* aligned src address */
	mlib_d64 *sp, *sl;

/* aligned mask address */
	mlib_d64 *mp, *ml;

/* aligned dst address */
	mlib_d64 *dp, *dl;

/* source data */
	mlib_d64 sd;

/* mask data */
	mlib_d64 md, mdh, mdl;

/* comparison masks */
	mlib_s32 cmask, cmaskh, cmaskl;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;
	mlib_f32 fzero = vis_fzeros();

	sp = sl = (mlib_d64 *)src;
	mp = ml = (mlib_d64 *)mask;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 8); i += 8) {
			sd = (*sp++);
			md = (*mp++);
			MLIB_V_IMAGECOPYMASK_U8(md, dthresh, dthresh);
			vis_pst_8(sd, dp, cmask);
			dp++;
		}

/* last 8-pixel column loop */

		if (i < xsize) {
			sd = vis_ld_d64_nf(sp);
			md = vis_ld_d64_nf(mp);
			MLIB_V_IMAGECOPYMASK_U8(md, dthresh, dthresh);
			emask = vis_edge8((void *)0, (void *)(xsize - i - 1));
			vis_pst_8(sd, dp, emask & cmask);
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		mp = ml = (mlib_d64 *)((mlib_u8 *)ml + mlb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageCopyMask_U8_124(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *mask,
    mlib_s32 mlb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh0)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned mask address */
	mlib_d64 *mp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to begin of a src line */
	mlib_u8 *sl;

/* pointer to begin of a mask line */
	mlib_u8 *ml;

/* pointer to begin of a dst line */
	mlib_u8 *dl;

/* pointer to end of a dst line */
	mlib_u8 *dend;

/* source data */
	mlib_d64 sd, s0, s1;

/* mask data */
	mlib_d64 md, mdh, mdl, m0, m1;

/* variable for thresh */
	mlib_d64 dthresh;

/* comparison masks */
	mlib_s32 cmask, cmaskh, cmaskl;

/* edge mask */
	mlib_s32 emask;

/* offset in dst address */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;
	mlib_f32 fzero = vis_fzeros();

	sl = src;
	ml = mask;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

/* prepare the constant value */
		vis_alignaddr(dp, 2 * off);
		dthresh = vis_faligndata(dthresh0, dthresh0);

/* generate edge mask for the start point */
		emask = vis_edge8(dl, dend);

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);
		s1 = vis_ld_d64_nf(sp);

/* prepare the mask address */
		mp = (mlib_d64 *)vis_alignaddr(ml, off);
		m1 = vis_ld_d64_nf(mp);

/* first-8-byte (8 pixels) */

		if (emask != 0xff) {
			vis_alignaddr(sl, off);
			s0 = s1;
			s1 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s0, s1);
			sp++;

			vis_alignaddr(ml, off);
			m0 = m1;
			m1 = vis_ld_d64_nf(mp + 1);
			md = vis_faligndata(m0, m1);
			mp++;
			MLIB_V_IMAGECOPYMASK_U8(md, dthresh, dthresh);
			vis_pst_8(sd, dp, emask & cmask);
			dp++;
		}

		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 1) >> 3;

/* 8-pixel column loop, emask not needed */
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			vis_alignaddr(sl, off);
			s0 = s1;
			s1 = vis_ld_d64_nf(sp + 1);
			m0 = m1;
			m1 = vis_ld_d64_nf(mp + 1);

			sd = vis_faligndata(s0, s1);
			vis_alignaddr(ml, off);
			md = vis_faligndata(m0, m1);

			MLIB_V_IMAGECOPYMASK_U8(md, dthresh, dthresh);

			vis_pst_8(sd, dp, cmask);
			sp++;
			mp++;
			dp++;
		}

/* last 8-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			md = vis_faligndata(m1, vis_ld_d64_nf(mp + 1));
			vis_alignaddr(sl, off);
			sd = vis_faligndata(s1, vis_ld_d64_nf(sp + 1));
			emask = vis_edge8(dp, dend);
			MLIB_V_IMAGECOPYMASK_U8(md, dthresh, dthresh);
			vis_pst_8(sd, dp, emask & cmask);
		}

		sl += slb;
		ml += mlb;
		dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageCopyMask_U8_3_A8D1(
    mlib_u8 *src,
    mlib_u8 *mask,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_d64 dthresh0)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned mask address */
	mlib_d64 *mp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to end of dst */
	mlib_u8 *dend;

/* source data */
	mlib_d64 sd0, sd1, sd2;

/* mask data */
	mlib_d64 md0, md1, md2, mdh, mdl;

/* variables for threshold */
	mlib_d64 dthresh1, dthresh2;

/* comparison masks */
	mlib_s32 cmask, cmaskh, cmaskl;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 j;
	mlib_f32 fzero = vis_fzeros();

/* prepare the threshold */
	vis_alignaddr(dst, 2);
	dthresh2 = vis_faligndata(dthresh0, dthresh0);
	dthresh1 = vis_faligndata(dthresh0, dthresh2);
	vis_alignaddr(dst, 4);
	dthresh2 = vis_faligndata(dthresh0, dthresh1);

	sp = (mlib_d64 *)src;
	mp = (mlib_d64 *)mask;
	dp = (mlib_d64 *)dst;
	dend = (mlib_u8 *)dp + dsize - 1;

/* 8-pixel column loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 24); j += 24) {
		sd0 = sp[0];
		md0 = mp[0];
		sd1 = sp[1];
		md1 = mp[1];
		sd2 = sp[2];
		md2 = mp[2];

		MLIB_V_IMAGECOPYMASK_U8(md0, dthresh0, dthresh1);
		vis_pst_8(sd0, dp, cmask);
		dp++;
		MLIB_V_IMAGECOPYMASK_U8(md1, dthresh2, dthresh0);
		vis_pst_8(sd1, dp, cmask);
		dp++;
		MLIB_V_IMAGECOPYMASK_U8(md2, dthresh1, dthresh2);
		vis_pst_8(sd2, dp, cmask);
		dp++;
		sp += 3;
		mp += 3;
	}

/* last 8-pixel column */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		sd0 = vis_ld_d64_nf(sp);
		md0 = vis_ld_d64_nf(mp);
		MLIB_V_IMAGECOPYMASK_U8(md0, dthresh0, dthresh1);
		emask = vis_edge8(dp, dend);
		vis_pst_8(sd0, dp, emask & cmask);
		dp++;

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd1 = vis_ld_d64_nf(sp + 1);
			md1 = vis_ld_d64_nf(mp + 1);
			MLIB_V_IMAGECOPYMASK_U8(md1, dthresh2, dthresh0);
			emask = vis_edge8(dp, dend);
			vis_pst_8(sd1, dp, emask & cmask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd2 = vis_ld_d64_nf(sp + 2);
				md2 = vis_ld_d64_nf(mp + 2);
				MLIB_V_IMAGECOPYMASK_U8(md2, dthresh1,
				    dthresh2);
				emask = vis_edge8(dp, dend);
				vis_pst_8(sd2, dp, emask & cmask);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageCopyMask_U8_3_A8D2(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *mask,
    mlib_s32 mlb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh0)
{
/* aligned src address */
	mlib_d64 *sp, *sl;

/* aligned mask address */
	mlib_d64 *mp, *ml;

/* aligned dst address */
	mlib_d64 *dp, *dl;

/* pointer to end of dst */
	mlib_u8 *dend;

/* source data */
	mlib_d64 sd;

/* mask data */
	mlib_d64 md, mdh, mdl;

/* variables for threshold */
	mlib_d64 dthresh1, dthresh2;

/* comparison masks */
	mlib_s32 cmask, cmaskh, cmaskl;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;
	mlib_f32 fzero = vis_fzeros();

/* prepare the threshold */
	vis_alignaddr(dst, 2);
	dthresh2 = vis_faligndata(dthresh0, dthresh0);
	dthresh1 = vis_faligndata(dthresh0, dthresh2);
	vis_alignaddr(dst, 4);
	dthresh2 = vis_faligndata(dthresh0, dthresh1);

	sp = sl = (mlib_d64 *)src;
	mp = ml = (mlib_d64 *)mask;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

		dend = (mlib_u8 *)dp + xsize - 1;

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 24); i += 24) {
			sd = (*sp++);
			md = (*mp++);
			MLIB_V_IMAGECOPYMASK_U8(md, dthresh0, dthresh1);
			vis_pst_8(sd, dp, cmask);
			dp++;

			sd = (*sp++);
			md = (*mp++);
			MLIB_V_IMAGECOPYMASK_U8(md, dthresh2, dthresh0);
			vis_pst_8(sd, dp, cmask);
			dp++;

			sd = (*sp++);
			md = (*mp++);
			MLIB_V_IMAGECOPYMASK_U8(md, dthresh1, dthresh2);
			vis_pst_8(sd, dp, cmask);
			dp++;
		}

/* last 8-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd = vis_ld_d64_nf(sp);
			md = vis_ld_d64_nf(mp);
			MLIB_V_IMAGECOPYMASK_U8(md, dthresh0, dthresh1);
			emask = vis_edge8(dp, dend);
			vis_pst_8(sd, dp, emask & cmask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd = vis_ld_d64_nf(sp + 1);
				md = vis_ld_d64_nf(mp + 1);
				MLIB_V_IMAGECOPYMASK_U8(md, dthresh2, dthresh0);
				emask = vis_edge8(dp, dend);
				vis_pst_8(sd, dp, emask & cmask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					sd = vis_ld_d64_nf(sp + 2);
					md = vis_ld_d64_nf(mp + 2);
					MLIB_V_IMAGECOPYMASK_U8(md, dthresh1,
					    dthresh2);
					emask = vis_edge8(dp, dend);
					vis_pst_8(sd, dp, emask & cmask);
				}
			}
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		mp = ml = (mlib_d64 *)((mlib_u8 *)ml + mlb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageCopyMask_U8_3(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *mask,
    mlib_s32 mlb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh0)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned mask address */
	mlib_d64 *mp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to src line */
	mlib_u8 *sl;

/* pointer to begin of a mask line */
	mlib_u8 *ml;

/* pointer to dst line */
	mlib_u8 *dl;

/* pointer to end of dst */
	mlib_u8 *dend;

/* source data */
	mlib_d64 sd0, sd1, sd2, s0, s1;

/* mask data */
	mlib_d64 md0, md1, md2, mdh, mdl, m0, m1;

/* variables for threshold */
	mlib_d64 dthresh[10], dthresh1, dthresh2;

/* comparison masks */
	mlib_s32 cmask, cmaskh, cmaskl;

/* edge masks */
	mlib_s32 emask;

/* offset of address in dst */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;
	mlib_f32 fzero = vis_fzeros();

/* prepare the threshold */
	dthresh[1] = dthresh[4] = dthresh[7] = dthresh0;
	vis_alignaddr((void *)0, 2);
	dthresh2 = vis_faligndata(dthresh0, dthresh0);
	dthresh[2] = dthresh[5] = dthresh[8] =
	    dthresh1 = vis_faligndata(dthresh0, dthresh2);
	vis_alignaddr((void *)0, 4);
	dthresh[0] = dthresh[3] = dthresh[6] =
	    dthresh[9] = vis_faligndata(dthresh0, dthresh1);

	sl = src;
	ml = mask;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

/* prepare the threshold */
		dthresh0 = dthresh[7 + off];
		dthresh1 = dthresh[8 + off];
		dthresh2 = dthresh[9 + off];

/* generate edge mask for the start point */
		emask = vis_edge8(dl, dend);

/* prepare the mask address */
		mp = (mlib_d64 *)vis_alignaddr(ml, off);
		m1 = vis_ld_d64_nf(mp);

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);
		s1 = vis_ld_d64_nf(sp);

/* first-8-byte */
		s0 = s1;
		s1 = vis_ld_d64_nf(sp + 1);
		sd0 = vis_faligndata(s0, s1);
		sp++;

		vis_alignaddr(ml, off);
		m0 = m1;
		m1 = vis_ld_d64_nf(mp + 1);
		md0 = vis_faligndata(m0, m1);
		mp++;
		MLIB_V_IMAGECOPYMASK_U8(md0, dthresh0, dthresh1);
		vis_pst_8(sd0, dp, emask & cmask);
		dp++;
		vis_alignaddr(sl, off);

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			s0 = s1;
			s1 = vis_ld_d64_nf(sp + 1);
			sd0 = vis_faligndata(s0, s1);
			sp++;

			vis_alignaddr(ml, off);
			m0 = m1;
			m1 = vis_ld_d64_nf(mp + 1);
			md0 = vis_faligndata(m0, m1);
			mp++;
			MLIB_V_IMAGECOPYMASK_U8(md0, dthresh2, dthresh0);
			vis_pst_8(sd0, dp, emask & cmask);
			dp++;
			vis_alignaddr(sl, off);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				s0 = s1;
				s1 = vis_ld_d64_nf(sp + 1);
				sd0 = vis_faligndata(s0, s1);
				sp++;

				vis_alignaddr(ml, off);
				m0 = m1;
				m1 = vis_ld_d64_nf(mp + 1);
				md0 = vis_faligndata(m0, m1);
				mp++;
				MLIB_V_IMAGECOPYMASK_U8(md0, dthresh1,
				    dthresh2);
				vis_pst_8(sd0, dp, emask & cmask);
				dp++;
				vis_alignaddr(sl, off);
			}
		}

/* 8-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 1) / 24;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s0 = s1;
			s1 = sp[1];
			sd0 = vis_faligndata(s0, s1);
			s0 = sp[2];
			sd1 = vis_faligndata(s1, s0);
			s1 = vis_ld_d64_nf(sp + 3);
			sd2 = vis_faligndata(s0, s1);
			sp += 3;

			vis_alignaddr(ml, off);
			m0 = m1;
			m1 = mp[1];
			md0 = vis_faligndata(m0, m1);
			m0 = mp[2];
			md1 = vis_faligndata(m1, m0);
			m1 = vis_ld_d64_nf(mp + 3);
			md2 = vis_faligndata(m0, m1);
			mp += 3;

			MLIB_V_IMAGECOPYMASK_U8(md0, dthresh0, dthresh1);
			vis_pst_8(sd0, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_U8(md1, dthresh2, dthresh0);
			vis_pst_8(sd1, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_U8(md2, dthresh1, dthresh2);
			vis_pst_8(sd2, dp, cmask);
			dp++;
			vis_alignaddr(sl, off);
		}

/* end point handling */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			s0 = s1;
			s1 = vis_ld_d64_nf(sp + 1);
			sd0 = vis_faligndata(s0, s1);

			vis_alignaddr(ml, off);
			m0 = m1;
			m1 = vis_ld_d64_nf(mp + 1);
			md0 = vis_faligndata(m0, m1);
			MLIB_V_IMAGECOPYMASK_U8(md0, dthresh0, dthresh1);
			vis_pst_8(sd0, dp, emask & cmask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				m0 = m1;
				m1 = vis_ld_d64_nf(mp + 2);
				md0 = vis_faligndata(m0, m1);

				vis_alignaddr(sl, off);
				s0 = s1;
				s1 = vis_ld_d64_nf(sp + 2);
				sd0 = vis_faligndata(s0, s1);
				MLIB_V_IMAGECOPYMASK_U8(md0, dthresh2,
				    dthresh0);
				vis_pst_8(sd0, dp, emask & cmask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge8(dp, dend);
					sd0 =
					    vis_faligndata(s1,
					    vis_ld_d64_nf(sp + 3));

					vis_alignaddr(ml, off);
					md0 =
					    vis_faligndata(m1,
					    vis_ld_d64_nf(mp + 3));
					MLIB_V_IMAGECOPYMASK_U8(md0, dthresh1,
					    dthresh2);
					vis_pst_8(sd0, dp, emask & cmask);
				}
			}
		}

		sl += slb;
		ml += mlb;
		dl += dlb;
	}
}

/* *********************************************************** */

#define	MLIB_V_IMAGECOPYMASK_S16(md, td)	cmask = vis_fcmplt16(md, td)

/* *********************************************************** */

void
mlib_v_ImageCopyMask_S16_124_A8D1X4(
    mlib_s16 *src,
    mlib_s16 *mask,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_d64 dthresh)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned mask address */
	mlib_d64 *mp;

/* aligned dst address */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd0, sd1, sd2, sd3;

/* mask data */
	mlib_d64 md0, md1, md2, md3;

/* comparison masks */
	mlib_s32 cmask;

/* loop variables */
	mlib_s32 j;

	sp = (mlib_d64 *)src;
	mp = (mlib_d64 *)mask;
	dp = (mlib_d64 *)dst;

/* 4-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 16); j += 16) {
		sd0 = sp[0];
		md0 = mp[0];
		sd1 = sp[1];
		md1 = mp[1];
		sd2 = sp[2];
		md2 = mp[2];
		sd3 = sp[3];
		md3 = mp[3];

		MLIB_V_IMAGECOPYMASK_S16(md0, dthresh);
		vis_pst_16(sd0, dp, cmask);
		dp++;
		MLIB_V_IMAGECOPYMASK_S16(md1, dthresh);
		vis_pst_16(sd1, dp, cmask);
		dp++;
		MLIB_V_IMAGECOPYMASK_S16(md2, dthresh);
		vis_pst_16(sd2, dp, cmask);
		dp++;
		MLIB_V_IMAGECOPYMASK_S16(md3, dthresh);
		vis_pst_16(sd3, dp, cmask);
		dp++;
		sp += 4;
		mp += 4;
	}

	for (; j <= (dsize - 4); j += 4) {
		sd0 = (*sp++);
		md0 = (*mp++);
		MLIB_V_IMAGECOPYMASK_S16(md0, dthresh);
		vis_pst_16(sd0, dp, cmask);
		dp++;
	}
}

/* *********************************************************** */

void
mlib_v_ImageCopyMask_S16_124_A8D2(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *mask,
    mlib_s32 mlb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh)
{
/* aligned src address */
	mlib_d64 *sp, *sl;

/* aligned mask address */
	mlib_d64 *mp, *ml;

/* aligned dst address */
	mlib_d64 *dp, *dl;

/* source data */
	mlib_d64 sd0, sd1, sd2, sd3;

/* mask data */
	mlib_d64 md0, md1, md2, md3;

/* comparison masks */
	mlib_s32 cmask;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;

	sp = sl = (mlib_d64 *)src;
	mp = ml = (mlib_d64 *)mask;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 16); i += 16) {
			sd0 = sp[0];
			md0 = mp[0];
			sd1 = sp[1];
			md1 = mp[1];
			sd2 = sp[2];
			md2 = mp[2];
			sd3 = sp[3];
			md3 = mp[3];

			MLIB_V_IMAGECOPYMASK_S16(md0, dthresh);
			vis_pst_16(sd0, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_S16(md1, dthresh);
			vis_pst_16(sd1, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_S16(md2, dthresh);
			vis_pst_16(sd2, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_S16(md3, dthresh);
			vis_pst_16(sd3, dp, cmask);
			dp++;
			sp += 4;
			mp += 4;
		}

		for (; i <= (xsize - 4); i += 4) {
			sd0 = (*sp++);
			md0 = (*mp++);
			MLIB_V_IMAGECOPYMASK_S16(md0, dthresh);
			vis_pst_16(sd0, dp, cmask);
			dp++;
		}

/* last 4-pixel column */

		if (i < xsize) {
			sd0 = vis_ld_d64_nf(sp);
			md0 = vis_ld_d64_nf(mp);
			MLIB_V_IMAGECOPYMASK_S16(md0, dthresh);
			emask =
			    vis_edge16((void *)0,
			    (void *)(2 * (xsize - i - 1)));
			vis_pst_16(sd0, dp, emask & cmask);
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		mp = ml = (mlib_d64 *)((mlib_u8 *)ml + mlb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageCopyMask_S16_124(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *mask,
    mlib_s32 mlb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh0)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned mask address */
	mlib_d64 *mp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to begin of a src line */
	mlib_s16 *sl;

/* pointer to begin of a mask line */
	mlib_s16 *ml;

/* pointer to begin of a dst line */
	mlib_s16 *dl;

/* pointer to end of a dst line */
	mlib_s16 *dend;

/* source data */
	mlib_d64 sd0, sd1, sd2, sd3, s0, s1;

/* mask data */
	mlib_d64 md0, md1, md2, md3, m0, m1;

/* intermediate variables */
	mlib_d64 dthresh;

/* comparison masks */
	mlib_s32 cmask;

/* edge mask */
	mlib_s32 emask;

/* offset in dst address */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;

	sl = src;
	ml = mask;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

/* prepare the threshold */
		vis_alignaddr(dp, off);
		dthresh = vis_faligndata(dthresh0, dthresh0);

/* generate edge mask for the start point */
		emask = vis_edge16(dl, dend);

/* prepare the mask address */
		mp = (mlib_d64 *)vis_alignaddr(ml, off);
		m0 = vis_ld_d64_nf(mp);

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);
		s0 = vis_ld_d64_nf(sp);

/* first-8-byte (4 pixels) */

		if (emask != 0xf) {
			s1 = s0;
			s0 = vis_ld_d64_nf(sp + 1);
			sd0 = vis_faligndata(s1, s0);
			sp++;

			vis_alignaddr(ml, off);
			m1 = m0;
			m0 = vis_ld_d64_nf(mp + 1);
			md0 = vis_faligndata(m1, m0);
			mp++;
			MLIB_V_IMAGECOPYMASK_S16(md0, dthresh);
			vis_pst_16(sd0, dp, emask & cmask);
			dp++;
			vis_alignaddr(sl, off);
		}

/* 4-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 2) >> 3;
#pragma pipeloop(0)
		for (i = 0; i <= (n - 4); i += 4) {
			s1 = sp[1];
			sd0 = vis_faligndata(s0, s1);
			s0 = sp[2];
			sd1 = vis_faligndata(s1, s0);
			s1 = sp[3];
			sd2 = vis_faligndata(s0, s1);
			s0 = vis_ld_d64_nf(sp + 4);
			sd3 = vis_faligndata(s1, s0);
			sp += 4;

			vis_alignaddr(ml, off);
			m1 = mp[1];
			md0 = vis_faligndata(m0, m1);
			m0 = mp[2];
			md1 = vis_faligndata(m1, m0);
			m1 = mp[3];
			md2 = vis_faligndata(m0, m1);
			m0 = vis_ld_d64_nf(mp + 4);
			md3 = vis_faligndata(m1, m0);
			mp += 4;

			MLIB_V_IMAGECOPYMASK_S16(md0, dthresh);
			vis_pst_16(sd0, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_S16(md1, dthresh);
			vis_pst_16(sd1, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_S16(md2, dthresh);
			vis_pst_16(sd2, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_S16(md3, dthresh);
			vis_pst_16(sd3, dp, cmask);
			dp++;
			vis_alignaddr(sl, off);
		}

		for (; i < n; i++) {
			s1 = s0;
			s0 = vis_ld_d64_nf(sp + 1);
			sd0 = vis_faligndata(s1, s0);
			sp++;

			vis_alignaddr(ml, off);
			m1 = m0;
			m0 = vis_ld_d64_nf(mp + 1);
			md0 = vis_faligndata(m1, m0);
			mp++;
			MLIB_V_IMAGECOPYMASK_S16(md0, dthresh);
			vis_pst_16(sd0, dp, cmask);
			dp++;
			vis_alignaddr(sl, off);
		}

/* last 4-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd0 = vis_faligndata(s0, vis_ld_d64_nf(sp + 1));

			vis_alignaddr(ml, off);
			md0 = vis_faligndata(m0, vis_ld_d64_nf(mp + 1));
			MLIB_V_IMAGECOPYMASK_S16(md0, dthresh);
			emask = vis_edge16(dp, dend);
			vis_pst_16(sd0, dp, emask & cmask);
		}

		sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		ml = (mlib_s16 *)((mlib_u8 *)ml + mlb);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageCopyMask_S16_3_A8D1(
    mlib_s16 *src,
    mlib_s16 *mask,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_d64 dthresh0)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned mask address */
	mlib_d64 *mp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to end of dst */
	mlib_s16 *dend;

/* source data */
	mlib_d64 sd0, sd1, sd2;

/* mask data */
	mlib_d64 md0, md1, md2;

/* variables for threshold */
	mlib_d64 dthresh1, dthresh2;

/* comparison masks */
	mlib_s32 cmask;

/* edge mask */
	mlib_s32 emask;

/* loop variable */
	mlib_s32 j;

/* prepare the threshold */
	vis_alignaddr(dst, 2);
	dthresh2 = vis_faligndata(dthresh0, dthresh0);
	dthresh1 = vis_faligndata(dthresh0, dthresh2);
	vis_alignaddr(dst, 4);
	dthresh2 = vis_faligndata(dthresh0, dthresh1);

	sp = (mlib_d64 *)src;
	mp = (mlib_d64 *)mask;
	dp = (mlib_d64 *)dst;

	dend = (mlib_s16 *)dp + dsize - 1;

/* 4-pixel column loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 12); j += 12) {
		sd0 = sp[0];
		md0 = mp[0];
		sd1 = sp[1];
		md1 = mp[1];
		sd2 = sp[2];
		md2 = mp[2];

		MLIB_V_IMAGECOPYMASK_S16(md0, dthresh0);
		vis_pst_16(sd0, dp, cmask);
		dp++;
		MLIB_V_IMAGECOPYMASK_S16(md1, dthresh1);
		vis_pst_16(sd1, dp, cmask);
		dp++;
		MLIB_V_IMAGECOPYMASK_S16(md2, dthresh2);
		vis_pst_16(sd2, dp, cmask);
		dp++;
		sp += 3;
		mp += 3;
	}

/* last 4-pixel */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		sd0 = vis_ld_d64_nf(sp);
		md0 = vis_ld_d64_nf(mp);
		MLIB_V_IMAGECOPYMASK_S16(md0, dthresh0);
		emask = vis_edge16(dp, dend);
		vis_pst_16(sd0, dp, emask & cmask);
		dp++;

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd1 = vis_ld_d64_nf(sp + 1);
			md1 = vis_ld_d64_nf(mp + 1);
			MLIB_V_IMAGECOPYMASK_S16(md1, dthresh1);
			emask = vis_edge16(dp, dend);
			vis_pst_16(sd1, dp, emask & cmask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd2 = vis_ld_d64_nf(sp + 2);
				md2 = vis_ld_d64_nf(mp + 2);
				MLIB_V_IMAGECOPYMASK_S16(md2, dthresh2);
				emask = vis_edge16(dp, dend);
				vis_pst_16(sd2, dp, emask & cmask);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageCopyMask_S16_3_A8D2(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *mask,
    mlib_s32 mlb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh0)
{
/* aligned src address */
	mlib_d64 *sp, *sl;

/* aligned mask address */
	mlib_d64 *mp, *ml;

/* aligned dst address */
	mlib_d64 *dp, *dl;

/* pointer to end of dst */
	mlib_s16 *dend;

/* source data */
	mlib_d64 sd0, sd1, sd2;

/* mask data */
	mlib_d64 md0, md1, md2;

/* variables for threshold */
	mlib_d64 dthresh1, dthresh2;

/* comparison masks */
	mlib_s32 cmask;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;

/* prepare the threshold */
	vis_alignaddr(dst, 2);
	dthresh2 = vis_faligndata(dthresh0, dthresh0);
	dthresh1 = vis_faligndata(dthresh0, dthresh2);
	vis_alignaddr(dst, 4);
	dthresh2 = vis_faligndata(dthresh0, dthresh1);

	sp = sl = (mlib_d64 *)src;
	mp = ml = (mlib_d64 *)mask;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

		dend = (mlib_s16 *)dp + xsize - 1;

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 12); i += 12) {
			sd0 = sp[0];
			md0 = mp[0];
			sd1 = sp[1];
			md1 = mp[1];
			sd2 = sp[2];
			md2 = mp[2];

			MLIB_V_IMAGECOPYMASK_S16(md0, dthresh0);
			vis_pst_16(sd0, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_S16(md1, dthresh1);
			vis_pst_16(sd1, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_S16(md2, dthresh2);
			vis_pst_16(sd2, dp, cmask);
			dp++;
			sp += 3;
			mp += 3;
		}

/* last 4-pixel */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd0 = vis_ld_d64_nf(sp);
			md0 = vis_ld_d64_nf(mp);
			MLIB_V_IMAGECOPYMASK_S16(md0, dthresh0);
			emask = vis_edge16(dp, dend);
			vis_pst_16(sd0, dp, emask & cmask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd1 = vis_ld_d64_nf(sp + 1);
				md1 = vis_ld_d64_nf(mp + 1);
				MLIB_V_IMAGECOPYMASK_S16(md1, dthresh1);
				emask = vis_edge16(dp, dend);
				vis_pst_16(sd1, dp, emask & cmask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					sd2 = vis_ld_d64_nf(sp + 2);
					md2 = vis_ld_d64_nf(mp + 2);
					MLIB_V_IMAGECOPYMASK_S16(md2, dthresh2);
					emask = vis_edge16(dp, dend);
					vis_pst_16(sd2, dp, emask & cmask);
				}
			}
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		mp = ml = (mlib_d64 *)((mlib_u8 *)ml + mlb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageCopyMask_S16_3(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *mask,
    mlib_s32 mlb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh0)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned mask address */
	mlib_d64 *mp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to src line */
	mlib_s16 *sl;

/* pointer to begin of a mask line */
	mlib_s16 *ml;

/* pointer to dst line */
	mlib_s16 *dl;

/* pointer to end of dst */
	mlib_s16 *dend;

/* source data */
	mlib_d64 sd0, sd1, sd2;

/* temporaries */
	mlib_d64 s0, s1;

/* mask data */
	mlib_d64 md0, md1, md2, m0, m1;

/* variables for threshold */
	mlib_d64 dt[6], dthresh1, dthresh2;

/* comparison masks */
	mlib_s32 cmask;

/* edge masks */
	mlib_s32 emask;

/* offset of address in dst */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;

/* prepare the threshold */
	dt[0] = dt[3] = dthresh0;
	vis_alignaddr((void *)0, 2);
	dthresh2 = vis_faligndata(dthresh0, dthresh0);
	dt[1] = dt[4] = dthresh1 = vis_faligndata(dthresh0, dthresh2);
	vis_alignaddr((void *)0, 4);
	dt[2] = dt[5] = vis_faligndata(dthresh0, dthresh1);

	sl = src;
	ml = mask;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

/* prepare the threshold */
		dthresh0 = dt[3 + (off >> 1)];
		dthresh1 = dt[4 + (off >> 1)];
		dthresh2 = dt[5 + (off >> 1)];

/* generate edge mask for the start point */
		emask = vis_edge16(dl, dend);

		mp = (mlib_d64 *)vis_alignaddr(ml, off);
		m1 = vis_ld_d64_nf(mp);

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);
		s1 = vis_ld_d64_nf(sp);

/* first-8-byte */
		s0 = s1;
		s1 = vis_ld_d64_nf(sp + 1);
		sd0 = vis_faligndata(s0, s1);
		sp++;

		vis_alignaddr(ml, off);
		m0 = m1;
		m1 = vis_ld_d64_nf(mp + 1);
		md0 = vis_faligndata(m0, m1);
		mp++;
		MLIB_V_IMAGECOPYMASK_S16(md0, dthresh0);
		vis_pst_16(sd0, dp, emask & cmask);
		dp++;
		vis_alignaddr(sl, off);

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s0 = s1;
			s1 = vis_ld_d64_nf(sp + 1);
			sd0 = vis_faligndata(s0, s1);
			sp++;

			vis_alignaddr(ml, off);
			m0 = m1;
			m1 = vis_ld_d64_nf(mp + 1);
			md0 = vis_faligndata(m0, m1);
			mp++;
			MLIB_V_IMAGECOPYMASK_S16(md0, dthresh1);
			emask = vis_edge16(dp, dend);
			vis_pst_16(sd0, dp, emask & cmask);
			dp++;
			vis_alignaddr(sl, off);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s0 = s1;
				s1 = vis_ld_d64_nf(sp + 1);
				sd0 = vis_faligndata(s0, s1);
				sp++;

				vis_alignaddr(ml, off);
				m0 = m1;
				m1 = vis_ld_d64_nf(mp + 1);
				md0 = vis_faligndata(m0, m1);
				mp++;
				MLIB_V_IMAGECOPYMASK_S16(md0, dthresh2);
				emask = vis_edge16(dp, dend);
				vis_pst_16(sd0, dp, emask & cmask);
				dp++;
				vis_alignaddr(sl, off);
			}
		}

/* 4-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 2) / 24;

/* 4-pixel column loop, no emask needed */
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s0 = s1;
			s1 = sp[1];
			sd0 = vis_faligndata(s0, s1);
			s0 = sp[2];
			sd1 = vis_faligndata(s1, s0);
			s1 = vis_ld_d64_nf(sp + 3);
			sd2 = vis_faligndata(s0, s1);
			sp += 3;

			vis_alignaddr(ml, off);
			m0 = m1;
			m1 = mp[1];
			md0 = vis_faligndata(m0, m1);
			m0 = mp[2];
			md1 = vis_faligndata(m1, m0);
			m1 = vis_ld_d64_nf(mp + 3);
			md2 = vis_faligndata(m0, m1);
			mp += 3;

			MLIB_V_IMAGECOPYMASK_S16(md0, dthresh0);
			vis_pst_16(sd0, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_S16(md1, dthresh1);
			vis_pst_16(sd1, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_S16(md2, dthresh2);
			vis_pst_16(sd2, dp, cmask);
			dp++;
			vis_alignaddr(sl, off);
		}

/* end point handling */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s0 = s1;
			s1 = vis_ld_d64_nf(sp + 1);
			sd0 = vis_faligndata(s0, s1);

			vis_alignaddr(ml, off);
			m0 = m1;
			m1 = vis_ld_d64_nf(mp + 1);
			md0 = vis_faligndata(m0, m1);
			MLIB_V_IMAGECOPYMASK_S16(md0, dthresh0);
			emask = vis_edge16(dp, dend);
			vis_pst_16(sd0, dp, emask & cmask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				m0 = m1;
				m1 = vis_ld_d64_nf(mp + 2);
				md0 = vis_faligndata(m0, m1);

				vis_alignaddr(sl, off);
				s0 = s1;
				s1 = vis_ld_d64_nf(sp + 2);
				sd0 = vis_faligndata(s0, s1);
				MLIB_V_IMAGECOPYMASK_S16(md0, dthresh1);
				emask = vis_edge16(dp, dend);
				vis_pst_16(sd0, dp, emask & cmask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					s0 = s1;
					s1 = vis_ld_d64_nf(sp + 3);
					sd0 = vis_faligndata(s0, s1);

					vis_alignaddr(ml, off);
					m0 = m1;
					m1 = vis_ld_d64_nf(mp + 3);
					md0 = vis_faligndata(m0, m1);
					MLIB_V_IMAGECOPYMASK_S16(md0, dthresh2);
					emask = vis_edge16(dp, dend);
					vis_pst_16(sd0, dp, emask & cmask);
				}
			}
		}

		sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		ml = (mlib_s16 *)((mlib_u8 *)ml + mlb);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

#define	MLIB_V_IMAGECOPYMASK_U16(md, td)                        \
	md = vis_fxor(md, xor_mask);                            \
	cmask = vis_fcmplt16(md, td)

/* *********************************************************** */

void
mlib_v_ImageCopyMask_U16_124_A8D1X4(
    mlib_u16 *src,
    mlib_u16 *mask,
    mlib_u16 *dst,
    mlib_s32 dsize,
    mlib_d64 dthresh)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned mask address */
	mlib_d64 *mp;

/* aligned dst address */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd0, sd1, sd2, sd3;

/* mask data */
	mlib_d64 md0, md1, md2, md3;

/* comparison masks */
	mlib_s32 cmask;

/* loop variables */
	mlib_s32 j;
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);

	sp = (mlib_d64 *)src;
	mp = (mlib_d64 *)mask;
	dp = (mlib_d64 *)dst;

/* 4-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 16); j += 16) {
		sd0 = sp[0];
		md0 = mp[0];
		sd1 = sp[1];
		md1 = mp[1];
		sd2 = sp[2];
		md2 = mp[2];
		sd3 = sp[3];
		md3 = mp[3];

		MLIB_V_IMAGECOPYMASK_U16(md0, dthresh);
		vis_pst_16(sd0, dp, cmask);
		dp++;
		MLIB_V_IMAGECOPYMASK_U16(md1, dthresh);
		vis_pst_16(sd1, dp, cmask);
		dp++;
		MLIB_V_IMAGECOPYMASK_U16(md2, dthresh);
		vis_pst_16(sd2, dp, cmask);
		dp++;
		MLIB_V_IMAGECOPYMASK_U16(md3, dthresh);
		vis_pst_16(sd3, dp, cmask);
		dp++;
		sp += 4;
		mp += 4;
	}

	for (; j <= (dsize - 4); j += 4) {
		sd0 = (*sp++);
		md0 = (*mp++);
		MLIB_V_IMAGECOPYMASK_U16(md0, dthresh);
		vis_pst_16(sd0, dp, cmask);
		dp++;
	}
}

/* *********************************************************** */

void
mlib_v_ImageCopyMask_U16_124_A8D2(
    mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *mask,
    mlib_s32 mlb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh)
{
/* aligned src address */
	mlib_d64 *sp, *sl;

/* aligned mask address */
	mlib_d64 *mp, *ml;

/* aligned dst address */
	mlib_d64 *dp, *dl;

/* source data */
	mlib_d64 sd0, sd1, sd2, sd3;

/* mask data */
	mlib_d64 md0, md1, md2, md3;

/* comparison masks */
	mlib_s32 cmask;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);

	sp = sl = (mlib_d64 *)src;
	mp = ml = (mlib_d64 *)mask;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 4-pixel column loop */

#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 16); i += 16) {
			sd0 = sp[0];
			md0 = mp[0];
			sd1 = sp[1];
			md1 = mp[1];
			sd2 = sp[2];
			md2 = mp[2];
			sd3 = sp[3];
			md3 = mp[3];

			MLIB_V_IMAGECOPYMASK_U16(md0, dthresh);
			vis_pst_16(sd0, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_U16(md1, dthresh);
			vis_pst_16(sd1, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_U16(md2, dthresh);
			vis_pst_16(sd2, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_U16(md3, dthresh);
			vis_pst_16(sd3, dp, cmask);
			dp++;
			sp += 4;
			mp += 4;
		}

		for (; i <= (xsize - 4); i += 4) {
			sd0 = (*sp++);
			md0 = (*mp++);
			MLIB_V_IMAGECOPYMASK_U16(md0, dthresh);
			vis_pst_16(sd0, dp, cmask);
			dp++;
		}

/* last 4-pixel column */

		if (i < xsize) {
			sd0 = vis_ld_d64_nf(sp);
			md0 = vis_ld_d64_nf(mp);
			MLIB_V_IMAGECOPYMASK_U16(md0, dthresh);
			emask =
			    vis_edge16((void *)0,
			    (void *)(2 * (xsize - i - 1)));
			vis_pst_16(sd0, dp, emask & cmask);
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		mp = ml = (mlib_d64 *)((mlib_u8 *)ml + mlb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageCopyMask_U16_124(
    mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *mask,
    mlib_s32 mlb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh0)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned mask address */
	mlib_d64 *mp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to begin of a src line */
	mlib_u16 *sl;

/* pointer to begin of a mask line */
	mlib_u16 *ml;

/* pointer to begin of a dst line */
	mlib_u16 *dl;

/* pointer to end of a dst line */
	mlib_u16 *dend;

/* source data */
	mlib_d64 sd0, sd1, sd2, sd3, s0, s1;

/* mask data */
	mlib_d64 md0, md1, md2, md3, m0, m1;

/* intermediate variables */
	mlib_d64 dthresh;

/* comparison masks */
	mlib_s32 cmask;

/* edge mask */
	mlib_s32 emask;

/* offset in dst address */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);

	sl = src;
	ml = mask;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

/* prepare the threshold */
		vis_alignaddr(dp, off);
		dthresh = vis_faligndata(dthresh0, dthresh0);

/* generate edge mask for the start point */
		emask = vis_edge16(dl, dend);

/* prepare the mask address */
		mp = (mlib_d64 *)vis_alignaddr(ml, off);
		m0 = vis_ld_d64_nf(mp);

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);
		s0 = vis_ld_d64_nf(sp);

/* first-8-byte (4 pixels) */

		if (emask != 0xf) {
			s1 = s0;
			s0 = vis_ld_d64_nf(sp + 1);
			sd0 = vis_faligndata(s1, s0);
			sp++;

			vis_alignaddr(ml, off);
			m1 = m0;
			m0 = vis_ld_d64_nf(mp + 1);
			md0 = vis_faligndata(m1, m0);
			mp++;
			MLIB_V_IMAGECOPYMASK_U16(md0, dthresh);
			vis_pst_16(sd0, dp, emask & cmask);
			dp++;
			vis_alignaddr(sl, off);
		}

/* 4-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 2) >> 3;
#pragma pipeloop(0)
		for (i = 0; i <= (n - 4); i += 4) {
			s1 = sp[1];
			sd0 = vis_faligndata(s0, s1);
			s0 = sp[2];
			sd1 = vis_faligndata(s1, s0);
			s1 = sp[3];
			sd2 = vis_faligndata(s0, s1);
			s0 = vis_ld_d64_nf(sp + 4);
			sd3 = vis_faligndata(s1, s0);
			sp += 4;

			vis_alignaddr(ml, off);
			m1 = mp[1];
			md0 = vis_faligndata(m0, m1);
			m0 = mp[2];
			md1 = vis_faligndata(m1, m0);
			m1 = mp[3];
			md2 = vis_faligndata(m0, m1);
			m0 = vis_ld_d64_nf(mp + 4);
			md3 = vis_faligndata(m1, m0);
			mp += 4;

			MLIB_V_IMAGECOPYMASK_U16(md0, dthresh);
			vis_pst_16(sd0, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_U16(md1, dthresh);
			vis_pst_16(sd1, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_U16(md2, dthresh);
			vis_pst_16(sd2, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_U16(md3, dthresh);
			vis_pst_16(sd3, dp, cmask);
			dp++;
			vis_alignaddr(sl, off);
		}

		for (; i < n; i++) {
			s1 = s0;
			s0 = vis_ld_d64_nf(sp + 1);
			sd0 = vis_faligndata(s1, s0);
			sp++;

			vis_alignaddr(ml, off);
			m1 = m0;
			m0 = vis_ld_d64_nf(mp + 1);
			md0 = vis_faligndata(m1, m0);
			mp++;
			MLIB_V_IMAGECOPYMASK_U16(md0, dthresh);
			vis_pst_16(sd0, dp, cmask);
			dp++;
			vis_alignaddr(sl, off);
		}

/* last 4-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd0 = vis_faligndata(s0, vis_ld_d64_nf(sp + 1));

			vis_alignaddr(ml, off);
			md0 = vis_faligndata(m0, vis_ld_d64_nf(mp + 1));
			MLIB_V_IMAGECOPYMASK_U16(md0, dthresh);
			emask = vis_edge16(dp, dend);
			vis_pst_16(sd0, dp, emask & cmask);
		}

		sl = (mlib_u16 *)((mlib_u8 *)sl + slb);
		ml = (mlib_u16 *)((mlib_u8 *)ml + mlb);
		dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageCopyMask_U16_3_A8D1(
    mlib_u16 *src,
    mlib_u16 *mask,
    mlib_u16 *dst,
    mlib_s32 dsize,
    mlib_d64 dthresh0)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned mask address */
	mlib_d64 *mp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to end of dst */
	mlib_s16 *dend;

/* source data */
	mlib_d64 sd0, sd1, sd2;

/* mask data */
	mlib_d64 md0, md1, md2;

/* variables for threshold */
	mlib_d64 dthresh1, dthresh2;

/* comparison masks */
	mlib_s32 cmask;

/* edge mask */
	mlib_s32 emask;

/* loop variable */
	mlib_s32 j;
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);

/* prepare the threshold */
	vis_alignaddr(dst, 2);
	dthresh2 = vis_faligndata(dthresh0, dthresh0);
	dthresh1 = vis_faligndata(dthresh0, dthresh2);
	vis_alignaddr(dst, 4);
	dthresh2 = vis_faligndata(dthresh0, dthresh1);

	sp = (mlib_d64 *)src;
	mp = (mlib_d64 *)mask;
	dp = (mlib_d64 *)dst;

	dend = (mlib_s16 *)dp + dsize - 1;

/* 4-pixel column loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 12); j += 12) {
		sd0 = sp[0];
		md0 = mp[0];
		sd1 = sp[1];
		md1 = mp[1];
		sd2 = sp[2];
		md2 = mp[2];

		MLIB_V_IMAGECOPYMASK_U16(md0, dthresh0);
		vis_pst_16(sd0, dp, cmask);
		dp++;
		MLIB_V_IMAGECOPYMASK_U16(md1, dthresh1);
		vis_pst_16(sd1, dp, cmask);
		dp++;
		MLIB_V_IMAGECOPYMASK_U16(md2, dthresh2);
		vis_pst_16(sd2, dp, cmask);
		dp++;
		sp += 3;
		mp += 3;
	}

/* last 4-pixel */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		sd0 = vis_ld_d64_nf(sp);
		md0 = vis_ld_d64_nf(mp);
		MLIB_V_IMAGECOPYMASK_U16(md0, dthresh0);
		emask = vis_edge16(dp, dend);
		vis_pst_16(sd0, dp, emask & cmask);
		dp++;

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd1 = vis_ld_d64_nf(sp + 1);
			md1 = vis_ld_d64_nf(mp + 1);
			MLIB_V_IMAGECOPYMASK_U16(md1, dthresh1);
			emask = vis_edge16(dp, dend);
			vis_pst_16(sd1, dp, emask & cmask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd2 = vis_ld_d64_nf(sp + 2);
				md2 = vis_ld_d64_nf(mp + 2);
				MLIB_V_IMAGECOPYMASK_U16(md2, dthresh2);
				emask = vis_edge16(dp, dend);
				vis_pst_16(sd2, dp, emask & cmask);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageCopyMask_U16_3_A8D2(
    mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *mask,
    mlib_s32 mlb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh0)
{
/* aligned src address */
	mlib_d64 *sp, *sl;

/* aligned mask address */
	mlib_d64 *mp, *ml;

/* aligned dst address */
	mlib_d64 *dp, *dl;

/* pointer to end of dst */
	mlib_s16 *dend;

/* source data */
	mlib_d64 sd0, sd1, sd2;

/* mask data */
	mlib_d64 md0, md1, md2;

/* variables for threshold */
	mlib_d64 dthresh1, dthresh2;

/* comparison masks */
	mlib_s32 cmask;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);

/* prepare the threshold */
	vis_alignaddr(dst, 2);
	dthresh2 = vis_faligndata(dthresh0, dthresh0);
	dthresh1 = vis_faligndata(dthresh0, dthresh2);
	vis_alignaddr(dst, 4);
	dthresh2 = vis_faligndata(dthresh0, dthresh1);

	sp = sl = (mlib_d64 *)src;
	mp = ml = (mlib_d64 *)mask;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

		dend = (mlib_s16 *)dp + xsize - 1;

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 12); i += 12) {
			sd0 = sp[0];
			md0 = mp[0];
			sd1 = sp[1];
			md1 = mp[1];
			sd2 = sp[2];
			md2 = mp[2];

			MLIB_V_IMAGECOPYMASK_U16(md0, dthresh0);
			vis_pst_16(sd0, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_U16(md1, dthresh1);
			vis_pst_16(sd1, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_U16(md2, dthresh2);
			vis_pst_16(sd2, dp, cmask);
			dp++;
			sp += 3;
			mp += 3;
		}

/* last 4-pixel */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd0 = vis_ld_d64_nf(sp);
			md0 = vis_ld_d64_nf(mp);
			MLIB_V_IMAGECOPYMASK_U16(md0, dthresh0);
			emask = vis_edge16(dp, dend);
			vis_pst_16(sd0, dp, emask & cmask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd1 = vis_ld_d64_nf(sp + 1);
				md1 = vis_ld_d64_nf(mp + 1);
				MLIB_V_IMAGECOPYMASK_U16(md1, dthresh1);
				emask = vis_edge16(dp, dend);
				vis_pst_16(sd1, dp, emask & cmask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					sd2 = vis_ld_d64_nf(sp + 2);
					md2 = vis_ld_d64_nf(mp + 2);
					MLIB_V_IMAGECOPYMASK_U16(md2, dthresh2);
					emask = vis_edge16(dp, dend);
					vis_pst_16(sd2, dp, emask & cmask);
				}
			}
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		mp = ml = (mlib_d64 *)((mlib_u8 *)ml + mlb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageCopyMask_U16_3(
    mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *mask,
    mlib_s32 mlb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh0)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned mask address */
	mlib_d64 *mp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to src line */
	mlib_u16 *sl;

/* pointer to begin of a mask line */
	mlib_u16 *ml;

/* pointer to dst line */
	mlib_u16 *dl;

/* pointer to end of dst */
	mlib_u16 *dend;

/* source data */
	mlib_d64 sd0, sd1, sd2;

/* temporaries */
	mlib_d64 s0, s1;

/* mask data */
	mlib_d64 md0, md1, md2, m0, m1;

/* variables for threshold */
	mlib_d64 dt[6], dthresh1, dthresh2;

/* comparison masks */
	mlib_s32 cmask;

/* edge masks */
	mlib_s32 emask;

/* offset of address in dst */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);

/* prepare the threshold */
	dt[0] = dt[3] = dthresh0;
	vis_alignaddr((void *)0, 2);
	dthresh2 = vis_faligndata(dthresh0, dthresh0);
	dt[1] = dt[4] = dthresh1 = vis_faligndata(dthresh0, dthresh2);
	vis_alignaddr((void *)0, 4);
	dt[2] = dt[5] = vis_faligndata(dthresh0, dthresh1);

	sl = src;
	ml = mask;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

/* prepare the threshold */
		dthresh0 = dt[3 + (off >> 1)];
		dthresh1 = dt[4 + (off >> 1)];
		dthresh2 = dt[5 + (off >> 1)];

/* generate edge mask for the start point */
		emask = vis_edge16(dl, dend);

		mp = (mlib_d64 *)vis_alignaddr(ml, off);
		m1 = vis_ld_d64_nf(mp);

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);
		s1 = vis_ld_d64_nf(sp);

/* first-8-byte */
		s0 = s1;
		s1 = vis_ld_d64_nf(sp + 1);
		sd0 = vis_faligndata(s0, s1);
		sp++;

		vis_alignaddr(ml, off);
		m0 = m1;
		m1 = vis_ld_d64_nf(mp + 1);
		md0 = vis_faligndata(m0, m1);
		mp++;
		MLIB_V_IMAGECOPYMASK_U16(md0, dthresh0);
		vis_pst_16(sd0, dp, emask & cmask);
		dp++;
		vis_alignaddr(sl, off);

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s0 = s1;
			s1 = vis_ld_d64_nf(sp + 1);
			sd0 = vis_faligndata(s0, s1);
			sp++;

			vis_alignaddr(ml, off);
			m0 = m1;
			m1 = vis_ld_d64_nf(mp + 1);
			md0 = vis_faligndata(m0, m1);
			mp++;
			MLIB_V_IMAGECOPYMASK_U16(md0, dthresh1);
			emask = vis_edge16(dp, dend);
			vis_pst_16(sd0, dp, emask & cmask);
			dp++;
			vis_alignaddr(sl, off);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s0 = s1;
				s1 = vis_ld_d64_nf(sp + 1);
				sd0 = vis_faligndata(s0, s1);
				sp++;

				vis_alignaddr(ml, off);
				m0 = m1;
				m1 = vis_ld_d64_nf(mp + 1);
				md0 = vis_faligndata(m0, m1);
				mp++;
				MLIB_V_IMAGECOPYMASK_U16(md0, dthresh2);
				emask = vis_edge16(dp, dend);
				vis_pst_16(sd0, dp, emask & cmask);
				dp++;
				vis_alignaddr(sl, off);
			}
		}

/* 4-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 2) / 24;

/* 4-pixel column loop, no emask needed */
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s0 = s1;
			s1 = sp[1];
			sd0 = vis_faligndata(s0, s1);
			s0 = sp[2];
			sd1 = vis_faligndata(s1, s0);
			s1 = vis_ld_d64_nf(sp + 3);
			sd2 = vis_faligndata(s0, s1);
			sp += 3;

			vis_alignaddr(ml, off);
			m0 = m1;
			m1 = mp[1];
			md0 = vis_faligndata(m0, m1);
			m0 = mp[2];
			md1 = vis_faligndata(m1, m0);
			m1 = vis_ld_d64_nf(mp + 3);
			md2 = vis_faligndata(m0, m1);
			mp += 3;

			MLIB_V_IMAGECOPYMASK_U16(md0, dthresh0);
			vis_pst_16(sd0, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_U16(md1, dthresh1);
			vis_pst_16(sd1, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_U16(md2, dthresh2);
			vis_pst_16(sd2, dp, cmask);
			dp++;
			vis_alignaddr(sl, off);
		}

/* end point handling */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s0 = s1;
			s1 = vis_ld_d64_nf(sp + 1);
			sd0 = vis_faligndata(s0, s1);

			vis_alignaddr(ml, off);
			m0 = m1;
			m1 = vis_ld_d64_nf(mp + 1);
			md0 = vis_faligndata(m0, m1);
			MLIB_V_IMAGECOPYMASK_U16(md0, dthresh0);
			emask = vis_edge16(dp, dend);
			vis_pst_16(sd0, dp, emask & cmask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				m0 = m1;
				m1 = vis_ld_d64_nf(mp + 2);
				md0 = vis_faligndata(m0, m1);

				vis_alignaddr(sl, off);
				s0 = s1;
				s1 = vis_ld_d64_nf(sp + 2);
				sd0 = vis_faligndata(s0, s1);
				MLIB_V_IMAGECOPYMASK_U16(md0, dthresh1);
				emask = vis_edge16(dp, dend);
				vis_pst_16(sd0, dp, emask & cmask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					s0 = s1;
					s1 = vis_ld_d64_nf(sp + 3);
					sd0 = vis_faligndata(s0, s1);

					vis_alignaddr(ml, off);
					m0 = m1;
					m1 = vis_ld_d64_nf(mp + 3);
					md0 = vis_faligndata(m0, m1);
					MLIB_V_IMAGECOPYMASK_U16(md0, dthresh2);
					emask = vis_edge16(dp, dend);
					vis_pst_16(sd0, dp, emask & cmask);
				}
			}
		}

		sl = (mlib_u16 *)((mlib_u8 *)sl + slb);
		ml = (mlib_u16 *)((mlib_u8 *)ml + mlb);
		dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

#define	MLIB_V_IMAGECOPYMASK_S32(md, td)	cmask = vis_fcmple32(md, td)

/* *********************************************************** */

void
mlib_v_ImageCopyMask_S32_124_A8D1X4(
    mlib_s32 *src,
    mlib_s32 *mask,
    mlib_s32 *dst,
    mlib_s32 dsize,
    mlib_d64 dthresh0,
    mlib_d64 dthresh1)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned mask address */
	mlib_d64 *mp;

/* aligned dst address */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd0, sd1;

/* mask data */
	mlib_d64 md0, md1;

/* comparison masks */
	mlib_s32 cmask;

/* loop variables */
	mlib_s32 j;

	sp = (mlib_d64 *)src;
	mp = (mlib_d64 *)mask;
	dp = (mlib_d64 *)dst;

/* 4-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 4); j += 4) {
		sd0 = sp[0];
		md0 = mp[0];
		sd1 = sp[1];
		md1 = mp[1];

		MLIB_V_IMAGECOPYMASK_S32(md0, dthresh0);
		vis_pst_32(sd0, dp, cmask);
		dp++;
		MLIB_V_IMAGECOPYMASK_S32(md1, dthresh1);
		vis_pst_32(sd1, dp, cmask);
		dp++;
		sp += 2;
		mp += 2;
	}
}

/* *********************************************************** */

void
mlib_v_ImageCopyMask_S32_124_A8D2(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *mask,
    mlib_s32 mlb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh0,
    mlib_d64 dthresh1)
{
/* aligned src address */
	mlib_d64 *sp, *sl;

/* aligned mask address */
	mlib_d64 *mp, *ml;

/* aligned dst address */
	mlib_d64 *dp, *dl;

/* source data */
	mlib_d64 sd0, sd1;

/* mask data */
	mlib_d64 md0, md1;

/* comparison masks */
	mlib_s32 cmask;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;

	sp = sl = (mlib_d64 *)src;
	mp = ml = (mlib_d64 *)mask;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 4); i += 4) {
			sd0 = sp[0];
			md0 = mp[0];
			sd1 = sp[1];
			md1 = mp[1];

			MLIB_V_IMAGECOPYMASK_S32(md0, dthresh0);
			vis_pst_32(sd0, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_S32(md1, dthresh1);
			vis_pst_32(sd1, dp, cmask);
			dp++;
			sp += 2;
			mp += 2;
		}

/* last 4-pixel column */

		if (i < xsize) {
			sd0 = vis_ld_d64_nf(sp);
			md0 = vis_ld_d64_nf(mp);
			MLIB_V_IMAGECOPYMASK_S32(md0, dthresh0);

			if (i <= (xsize - 3)) {
				vis_pst_32(sd0, dp, cmask);
				dp++;
				sd1 = vis_ld_d64_nf(sp + 1);
				md1 = vis_ld_d64_nf(mp + 1);
				MLIB_V_IMAGECOPYMASK_S32(md1, dthresh1);
				vis_pst_32(sd1, dp, 2 & cmask);
			} else {
				emask =
				    vis_edge32((void *)0,
				    (void *)(4 * (xsize - i - 1)));
				vis_pst_32(sd0, dp, emask & cmask);
			}
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		mp = ml = (mlib_d64 *)((mlib_u8 *)ml + mlb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageCopyMask_S32_124(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *mask,
    mlib_s32 mlb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh0,
    mlib_d64 dthresh1)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned mask address */
	mlib_d64 *mp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to begin of a src line */
	mlib_s32 *sl;

/* pointer to begin of a mask line */
	mlib_s32 *ml;

/* pointer to begin of a dst line */
	mlib_s32 *dl;

/* pointer to end of a dst line */
	mlib_s32 *dend;

/* source data */
	mlib_d64 sd0, sd1, sd2, sd3, s0, s1;

/* mask data */
	mlib_d64 md0, md1, md2, md3, m0, m1;

/* intermediate variables */
	mlib_d64 dthresh_0, dthresh_1;

/* comparison masks */
	mlib_s32 cmask;

/* edge mask */
	mlib_s32 emask;

/* offset in dst address */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;

	sl = src;
	ml = mask;
	dl = dst;

/* prepare the threshold */
	vis_alignaddr((void *)0, 4);
	dthresh_0 = vis_faligndata(dthresh1, dthresh0);
	dthresh_1 = vis_faligndata(dthresh0, dthresh1);

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

/* prepare the threshold */
		vis_alignaddr(dp, 4 + off);
		dthresh0 = vis_faligndata(dthresh_0, dthresh_1);
		dthresh1 = vis_faligndata(dthresh_1, dthresh_0);

/* generate edge mask for the start point */
		emask = vis_edge32(dl, dend);

/* prepare the mask address */
		mp = (mlib_d64 *)vis_alignaddr(ml, off);
		m0 = vis_ld_d64_nf(mp);

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);
		s0 = vis_ld_d64_nf(sp);

/* first-8-byte (4 pixels) */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s1 = s0;
			s0 = vis_ld_d64_nf(sp + 1);
			sd0 = vis_faligndata(s1, s0);
			sp++;

			vis_alignaddr(ml, off);
			m1 = m0;
			m0 = vis_ld_d64_nf(mp + 1);
			md0 = vis_faligndata(m1, m0);
			mp++;
			MLIB_V_IMAGECOPYMASK_S32(md0, dthresh0);
			vis_pst_32(sd0, dp, emask & cmask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				m1 = vis_ld_d64_nf(mp + 1);
				md1 = vis_faligndata(m0, m1);
				mp++;

				vis_alignaddr(sl, off);
				s1 = vis_ld_d64_nf(sp + 1);
				sd1 = vis_faligndata(s0, s1);
				sp++;

				MLIB_V_IMAGECOPYMASK_S32(md1, dthresh1);
				emask = vis_edge32(dp, dend);
				vis_pst_32(sd1, dp, emask & cmask);
				dp++;
			}
		}

/* 4-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 4) >> 5;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s0 = sp[1];
			sd0 = vis_faligndata(s1, s0);
			s1 = sp[2];
			sd1 = vis_faligndata(s0, s1);
			s0 = sp[3];
			sd2 = vis_faligndata(s1, s0);
			s1 = vis_ld_d64_nf(sp + 4);
			sd3 = vis_faligndata(s0, s1);
			sp += 4;

			vis_alignaddr(ml, off);
			m0 = mp[1];
			md0 = vis_faligndata(m1, m0);
			m1 = mp[2];
			md1 = vis_faligndata(m0, m1);
			m0 = mp[3];
			md2 = vis_faligndata(m1, m0);
			m1 = vis_ld_d64_nf(mp + 4);
			md3 = vis_faligndata(m0, m1);
			mp += 4;

			MLIB_V_IMAGECOPYMASK_S32(md0, dthresh0);
			vis_pst_32(sd0, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_S32(md1, dthresh1);
			vis_pst_32(sd1, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_S32(md2, dthresh0);
			vis_pst_32(sd2, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_S32(md3, dthresh1);
			vis_pst_32(sd3, dp, cmask);
			dp++;
			vis_alignaddr(sl, off);
		}

		if (((mlib_s32 *)dend - (mlib_s32 *)dp) >= 3) {
			s0 = sp[1];
			sd0 = vis_faligndata(s1, s0);
			s1 = vis_ld_d64_nf(sp + 2);
			sd1 = vis_faligndata(s0, s1);
			vis_alignaddr(ml, off);
			m0 = mp[1];
			md0 = vis_faligndata(m1, m0);
			m1 = vis_ld_d64_nf(mp + 2);
			md1 = vis_faligndata(m0, m1);
			MLIB_V_IMAGECOPYMASK_S32(md0, dthresh0);
			vis_pst_32(sd0, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_S32(md1, dthresh1);
			vis_pst_32(sd1, dp, cmask);
			dp++;
			sp += 2;
			mp += 2;
			vis_alignaddr(sl, off);
		}

/* last 4-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s0 = vis_ld_d64_nf(sp + 1);
			sd0 = vis_faligndata(s1, s0);

			vis_alignaddr(ml, off);
			m0 = vis_ld_d64_nf(mp + 1);
			md0 = vis_faligndata(m1, m0);
			MLIB_V_IMAGECOPYMASK_S32(md0, dthresh0);
			emask = vis_edge32(dp, dend);
			vis_pst_32(sd0, dp, emask & cmask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				m1 = vis_ld_d64_nf(mp + 2);
				md1 = vis_faligndata(m0, m1);

				vis_alignaddr(sl, off);
				s1 = vis_ld_d64_nf(sp + 2);
				sd1 = vis_faligndata(s0, s1);

				MLIB_V_IMAGECOPYMASK_S32(md1, dthresh1);
				emask = vis_edge32(dp, dend);
				vis_pst_32(sd1, dp, emask & cmask);
			}
		}

		sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
		ml = (mlib_s32 *)((mlib_u8 *)ml + mlb);
		dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageCopyMask_S32_3_A8D1(
    mlib_s32 *src,
    mlib_s32 *mask,
    mlib_s32 *dst,
    mlib_s32 dsize,
    mlib_d64 dthresh0,
    mlib_d64 dthresh1)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned mask address */
	mlib_d64 *mp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to end of dst */
	mlib_s32 *dend;

/* source data */
	mlib_d64 sd0, sd1, sd2;

/* mask data */
	mlib_d64 md0, md1, md2;

/* variables for threshold */
	mlib_d64 dthresh2;

/* comparison masks */
	mlib_s32 cmask;

/* loop variable */
	mlib_s32 j;

/* prepare the threshold */
	vis_alignaddr(dst, 4);
	dthresh2 = vis_faligndata(dthresh0, dthresh1);

	sp = (mlib_d64 *)src;
	mp = (mlib_d64 *)mask;
	dp = (mlib_d64 *)dst;

	dend = (mlib_s32 *)dp + dsize - 1;

/* 3-pixel column loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 6); j += 6) {
		sd0 = sp[0];
		md0 = mp[0];
		sd1 = sp[1];
		md1 = mp[1];
		sd2 = sp[2];
		md2 = mp[2];

		MLIB_V_IMAGECOPYMASK_S32(md0, dthresh0);
		vis_pst_32(sd0, dp, cmask);
		dp++;
		MLIB_V_IMAGECOPYMASK_S32(md1, dthresh1);
		vis_pst_32(sd1, dp, cmask);
		dp++;
		MLIB_V_IMAGECOPYMASK_S32(md2, dthresh2);
		vis_pst_32(sd2, dp, cmask);
		dp++;
		sp += 3;
		mp += 3;
	}

/* last 3-pixel */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		sd0 = vis_ld_d64_nf(sp);
		md0 = vis_ld_d64_nf(mp);
		sd1 = vis_ld_d64_nf(sp + 1);
		md1 = vis_ld_d64_nf(mp + 1);
		MLIB_V_IMAGECOPYMASK_S32(md0, dthresh0);
		vis_pst_32(sd0, dp, cmask);
		dp++;
		MLIB_V_IMAGECOPYMASK_S32(md1, dthresh1);
		vis_pst_32(sd1, dp, cmask & 2);
	}
}

/* *********************************************************** */

void
mlib_v_ImageCopyMask_S32_3_A8D2(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *mask,
    mlib_s32 mlb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh0,
    mlib_d64 dthresh1)
{
/* aligned src address */
	mlib_d64 *sp, *sl;

/* aligned mask address */
	mlib_d64 *mp, *ml;

/* aligned dst address */
	mlib_d64 *dp, *dl;

/* pointer to end of dst */
	mlib_s32 *dend;

/* source data */
	mlib_d64 sd0, sd1, sd2;

/* mask data */
	mlib_d64 md0, md1, md2;

/* variables for threshold */
	mlib_d64 dthresh2;

/* comparison masks */
	mlib_s32 cmask;

/* loop variables */
	mlib_s32 i, j;

/* prepare the threshold */
	vis_alignaddr(dst, 4);
	dthresh2 = vis_faligndata(dthresh0, dthresh1);

	sp = sl = (mlib_d64 *)src;
	mp = ml = (mlib_d64 *)mask;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

		dend = (mlib_s32 *)dp + xsize - 1;

/* 3-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i <= (xsize - 6); i += 6) {
			sd0 = sp[0];
			md0 = mp[0];
			sd1 = sp[1];
			md1 = mp[1];
			sd2 = sp[2];
			md2 = mp[2];

			MLIB_V_IMAGECOPYMASK_S32(md0, dthresh0);
			vis_pst_32(sd0, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_S32(md1, dthresh1);
			vis_pst_32(sd1, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_S32(md2, dthresh2);
			vis_pst_32(sd2, dp, cmask);
			dp++;
			sp += 3;
			mp += 3;
		}

/* last 3-pixel */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			sd0 = sp[0];
			md0 = mp[0];
			sd1 = vis_ld_d64_nf(sp + 1);
			md1 = vis_ld_d64_nf(mp + 1);
			MLIB_V_IMAGECOPYMASK_S32(md0, dthresh0);
			vis_pst_32(sd0, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_S32(md1, dthresh1);
			vis_pst_32(sd1, dp, cmask & 2);
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		mp = ml = (mlib_d64 *)((mlib_u8 *)ml + mlb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageCopyMask_S32_3(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *mask,
    mlib_s32 mlb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh0_0,
    mlib_d64 dthresh1_0)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned mask address */
	mlib_d64 *mp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to src line */
	mlib_s32 *sl;

/* pointer to begin of a mask line */
	mlib_s32 *ml;

/* pointer to dst line */
	mlib_s32 *dl;

/* pointer to end of dst */
	mlib_s32 *dend;

/* source data */
	mlib_d64 sd0, sd1, sd2;

/* temporaries */
	mlib_d64 s0, s1;

/* mask data */
	mlib_d64 md0, md1, md2, m0, m1;

/* variables for threshold */
	mlib_d64 dthresh2_0;

/* variables for threshold */
	mlib_d64 dthresh0, dthresh1, dthresh2;

/* comparison masks */
	mlib_s32 cmask;

/* edge masks */
	mlib_s32 emask;

/* offset of address in dst */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;

/* prepare the threshold */
	vis_alignaddr((void *)0, 4);
	dthresh2_0 = vis_faligndata(dthresh0_0, dthresh1_0);

	sl = src;
	ml = mask;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

/* prepare the threshold */
		vis_alignaddr(dp, 4 + off);
		dthresh0 = vis_faligndata(dthresh1_0, dthresh2_0);
		dthresh1 = vis_faligndata(dthresh2_0, dthresh0_0);
		dthresh2 = vis_faligndata(dthresh0_0, dthresh1_0);

/* generate edge mask for the start point */
		emask = vis_edge32(dl, dend);

		mp = (mlib_d64 *)vis_alignaddr(ml, off);
		m1 = vis_ld_d64_nf(mp);

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);
		s1 = vis_ld_d64_nf(sp);

/* first-8-byte */
		s0 = s1;
		s1 = vis_ld_d64_nf(sp + 1);
		sd0 = vis_faligndata(s0, s1);
		sp++;

		vis_alignaddr(ml, off);
		m0 = m1;
		m1 = vis_ld_d64_nf(mp + 1);
		md0 = vis_faligndata(m0, m1);
		mp++;
		MLIB_V_IMAGECOPYMASK_S32(md0, dthresh0);
		vis_pst_32(sd0, dp, emask & cmask);
		dp++;

		if ((mlib_addr)dp <= (mlib_addr)dend) {

			m0 = m1;
			m1 = vis_ld_d64_nf(mp + 1);
			md0 = vis_faligndata(m0, m1);
			mp++;

			vis_alignaddr(sl, off);
			s0 = s1;
			s1 = vis_ld_d64_nf(sp + 1);
			sd0 = vis_faligndata(s0, s1);
			sp++;
			MLIB_V_IMAGECOPYMASK_S32(md0, dthresh1);
			emask = vis_edge32(dp, dend);
			vis_pst_32(sd0, dp, emask & cmask);
			dp++;
			vis_alignaddr(sl, off);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s0 = s1;
				s1 = vis_ld_d64_nf(sp + 1);
				sd0 = vis_faligndata(s0, s1);
				sp++;

				vis_alignaddr(ml, off);
				m0 = m1;
				m1 = vis_ld_d64_nf(mp + 1);
				md0 = vis_faligndata(m0, m1);
				mp++;
				MLIB_V_IMAGECOPYMASK_S32(md0, dthresh2);
				emask = vis_edge32(dp, dend);
				vis_pst_32(sd0, dp, emask & cmask);
				dp++;
				vis_alignaddr(sl, off);
			}
		}

/* 4-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 4) / 24;

/* 4-pixel column loop, no emask needed */
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s0 = s1;
			s1 = sp[1];
			sd0 = vis_faligndata(s0, s1);
			s0 = sp[2];
			sd1 = vis_faligndata(s1, s0);
			s1 = vis_ld_d64_nf(sp + 3);
			sd2 = vis_faligndata(s0, s1);
			sp += 3;

			vis_alignaddr(ml, off);
			m0 = m1;
			m1 = mp[1];
			md0 = vis_faligndata(m0, m1);
			m0 = mp[2];
			md1 = vis_faligndata(m1, m0);
			m1 = vis_ld_d64_nf(mp + 3);
			md2 = vis_faligndata(m0, m1);
			mp += 3;

			MLIB_V_IMAGECOPYMASK_S32(md0, dthresh0);
			vis_pst_32(sd0, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_S32(md1, dthresh1);
			vis_pst_32(sd1, dp, cmask);
			dp++;
			MLIB_V_IMAGECOPYMASK_S32(md2, dthresh2);
			vis_pst_32(sd2, dp, cmask);
			dp++;
			vis_alignaddr(sl, off);
		}

/* end point handling */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s0 = s1;
			s1 = vis_ld_d64_nf(sp + 1);
			sd0 = vis_faligndata(s0, s1);

			vis_alignaddr(ml, off);
			m0 = m1;
			m1 = vis_ld_d64_nf(mp + 1);
			md0 = vis_faligndata(m0, m1);
			MLIB_V_IMAGECOPYMASK_S32(md0, dthresh0);
			emask = vis_edge32(dp, dend);
			vis_pst_32(sd0, dp, emask & cmask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				m0 = m1;
				m1 = vis_ld_d64_nf(mp + 2);
				md0 = vis_faligndata(m0, m1);

				vis_alignaddr(sl, off);
				s0 = s1;
				s1 = vis_ld_d64_nf(sp + 2);
				sd0 = vis_faligndata(s0, s1);
				MLIB_V_IMAGECOPYMASK_S32(md0, dthresh1);
				emask = vis_edge32(dp, dend);
				vis_pst_32(sd0, dp, emask & cmask);
				dp++;
			}
		}

		sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
		ml = (mlib_s32 *)((mlib_u8 *)ml + mlb);
		dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */
