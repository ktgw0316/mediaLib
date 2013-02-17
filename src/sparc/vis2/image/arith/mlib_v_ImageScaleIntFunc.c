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
 *       void mlib_v_ImageScale_S32_S16_124_A8D1(mlib_u8  *src,
 *                                               mlib_u8  *dst,
 *                                               mlib_s32 dsize,
 *                                               mlib_d64 ad,
 *                                               mlib_d64 bdh,
 *                                               mlib_d64 bdl);
 *
 *       void mlib_v_ImageScale_S32_S16_124(mlib_u8  *src,
 *                                          mlib_s32 slb,
 *                                          mlib_u8  *dst,
 *                                          mlib_s32 dlb,
 *                                          mlib_s32 xsize,
 *                                          mlib_s32 ysize,
 *                                          mlib_f32 *afh,
 *                                          mlib_f32 *afl,
 *                                          mlib_d64 *bdh,
 *                                          mlib_d64 *bdl);
 *
 *       void mlib_v_ImageScale_S32_S16_3(mlib_u8  *src,
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
 *       void mlib_v_ImageScale_S32_S16_3_A8D1(mlib_u8  *src,
 *                                             mlib_u8  *dst,
 *                                             mlib_s32 dsize,
 *                                             mlib_f32 *afh,
 *                                             mlib_f32 *afl,
 *                                             mlib_d64 *bdh,
 *                                             mlib_d64 *bdl);
 *
 *       void mlib_v_ImageScale_S32_U16_124(mlib_u8  *src,
 *                                          mlib_s32 slb,
 *                                          mlib_u8  *dst,
 *                                          mlib_s32 dlb,
 *                                          mlib_s32 xsize,
 *                                          mlib_s32 ysize,
 *                                          mlib_f32 *afh,
 *                                          mlib_f32 *afl,
 *                                          mlib_d64 *bdh,
 *                                          mlib_d64 *bdl);
 *
 *       void mlib_v_ImageScale_S32_U16_3(mlib_u8  *src,
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
 *       void mlib_v_ImageScale_S32_U8_124_A8D1(mlib_u8  *src,
 *                                              mlib_u8  *dst,
 *                                              mlib_s32 dsize,
 *                                              mlib_d64 ad,
 *                                              mlib_d64 bdh,
 *                                              mlib_d64 bdl);
 *
 *       void mlib_v_ImageScale_S32_U8_124(mlib_u8  *src,
 *                                         mlib_s32 slb,
 *                                         mlib_u8  *dst,
 *                                         mlib_s32 dlb,
 *                                         mlib_s32 xsize,
 *                                         mlib_s32 ysize,
 *                                         mlib_f32 *afh,
 *                                         mlib_f32 *afl,
 *                                         mlib_d64 *bdh,
 *                                         mlib_d64 *bdl);
 *
 *       void mlib_v_ImageScale_S32_U8_3(mlib_u8  *src,
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
 *       void mlib_v_ImageScale_S32_U8_3_A8D1(mlib_u8  *src,
 *                                            mlib_u8  *dst,
 *                                            mlib_s32 dsize,
 *                                            mlib_f32 *afh,
 *                                            mlib_f32 *afl,
 *                                            mlib_d64 *bdh,
 *                                            mlib_d64 *bdl);
 *
 *       void mlib_v_ImageScale_S16_U8_124(mlib_s16 *src,
 *                                         mlib_s32 slb,
 *                                         mlib_u8  *dst,
 *                                         mlib_s32 dlb,
 *                                         mlib_s32 xsize,
 *                                         mlib_s32 ysize,
 *                                         mlib_d64 ad,
 *                                         mlib_d64 bdh,
 *                                         mlib_d64 bdl);
 *
 *       void mlib_v_ImageScale_S16_U8_124_A8D1X4(mlib_s16 *src,
 *                                                mlib_u8  *dst,
 *                                                mlib_s32 dsize,
 *                                                mlib_d64 ad,
 *                                                mlib_d64 bdh,
 *                                                mlib_d64 bdl);
 *
 *       void mlib_v_ImageScale_S16_U8_124_A8D2(mlib_s16 *src,
 *                                              mlib_s32 slb,
 *                                              mlib_u8  *dst,
 *                                              mlib_s32 dlb,
 *                                              mlib_s32 xsize,
 *                                              mlib_s32 ysize,
 *                                              mlib_d64 ad,
 *                                              mlib_d64 bdh,
 *                                              mlib_d64 bdl);
 *
 *       void mlib_v_ImageScale_S16_U8_3(mlib_s16 *src,
 *                                       mlib_s32 slb,
 *                                       mlib_u8  *dst,
 *                                       mlib_s32 dlb,
 *                                       mlib_s32 xsize,
 *                                       mlib_s32 ysize,
 *                                       mlib_d64 ad,
 *                                       mlib_d64 bdh,
 *                                       mlib_d64 bdl);
 *
 *       void mlib_v_ImageScale_S16_U8_3_A8D1(mlib_s16 *src,
 *                                            mlib_u8  *dst,
 *                                            mlib_s32 dsize,
 *                                            mlib_d64 ad,
 *                                            mlib_d64 bdh,
 *                                            mlib_d64 bdl);
 *
 *       void mlib_v_ImageScale_S16_U8_3_A8D2(mlib_s16 *src,
 *                                            mlib_s32 slb,
 *                                            mlib_u8  *dst,
 *                                            mlib_s32 dlb,
 *                                            mlib_s32 xsize,
 *                                            mlib_s32 ysize,
 *                                            mlib_d64 ad,
 *                                            mlib_d64 bdh,
 *                                            mlib_d64 bdl);
 *
 *       void mlib_v_ImageScale_U16_U8_124(mlib_u16 *src,
 *                                         mlib_s32 slb,
 *                                         mlib_u8  *dst,
 *                                         mlib_s32 dlb,
 *                                         mlib_s32 xsize,
 *                                         mlib_s32 ysize,
 *                                         mlib_d64 ad,
 *                                         mlib_d64 bdh,
 *                                         mlib_d64 bdl);
 *
 *       void mlib_v_ImageScale_U16_U8_3(mlib_u16 *src,
 *                                       mlib_s32 slb,
 *                                       mlib_u8  *dst,
 *                                       mlib_s32 dlb,
 *                                       mlib_s32 xsize,
 *                                       mlib_s32 ysize,
 *                                       mlib_d64 ad,
 *                                       mlib_d64 bdh,
 *                                       mlib_d64 bdl);
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

#define	MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, afh, afl, bdh,      \
	bdl, dd)                                                \
	sd = vis_bshuffle(sd0, sd1);                            \
	MLIB_V_IMAGESCALE_S16_S16(sd, afh, afl, bdh, bdl, dd)

/* *********************************************************** */

#define	MLIB_V_IMAGESCALE_S32_S16_NA(sd0, sd1, afh, afl, bdh,    \
	bdl, dd)                                                 \
	sd01 = vis_bshuffle(sd0, sd1);                           \
	sd = vis_faligndata(sd00, sd01);                         \
	sd00 = sd01;                                             \
	MLIB_V_IMAGESCALE_S16_S16(sd, afh, afl, bdh, bdl, dd)

/* *********************************************************** */

#define	MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, afh, afl, bdh,             \
	bdl, dd)                                                       \
	MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, afh, afl, bdh, bdl, dd);   \
	dd = vis_fxor(dd, mask_8000)

/* *********************************************************** */

#define	MLIB_V_IMAGESCALE_S32_U16_NA(sd0, sd1, afh, afl, bdh,             \
	bdl, dd)                                                          \
	MLIB_V_IMAGESCALE_S32_S16_NA(sd0, sd1, afh, afl, bdh, bdl, dd);   \
	dd = vis_fxor(dd, mask_8000)

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

	vis_write_bmask(0x014589CD, 0);

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
	mlib_d64 sd, sd0, sd1, sd00, sd01;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* loop variables */
	mlib_s32 j;
	mlib_f32 *sl;
	mlib_d64 *sp;
	mlib_s16 *dl, *dend;
	mlib_d64 bh, bl;
	mlib_f32 ah, al;
	mlib_d64 sf[2];
	mlib_s32 off, i, n, emask;

	sl = (mlib_f32 *)src;
	dl = (mlib_s16 *)dst;

/* prepare align for macro */
	vis_write_bmask(0x014589CD, 0);

	if ((2 * xsize == dlb) && (slb == 2 * dlb)) {
		xsize *= ysize;
		ysize = 1;
	}

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the source and destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_u8 *)dl - (mlib_u8 *)dp;
		dend = dl + xsize - 1;

/* prepare the alpha and beta values */
		off >>= 1;
		ah = afh[off];
		al = afl[off];
		bh = bdh[off];
		bl = bdl[off];

		sp = (mlib_d64 *)vis_alignaddr(sl, -4 * off);
		emask = vis_edge16(dl, dend);

		if (((mlib_u8 *)sp - ((mlib_u8 *)sl - 4 * off)) == 0) {
			if (emask != 0xf) {
				for (i = off; i < 4; i++)
					*((mlib_f32 *)sf + i) = sl[i - off];

				MLIB_V_IMAGESCALE_S32_S16(sf[0], sf[1], ah, al,
				    bh, bl, dd);
				vis_pst_16(dd, dp, emask);
				dp++;
				sp += 2;
			}

			n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) >> 3;
#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				sd0 = (*sp++);
				sd1 = (*sp++);
				MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, ah, al, bh,
				    bl, dd);
				*dp = dd;
				dp++;
			}

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				sd0 = (*sp++);
				sd1 = *sp;
				MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, ah, al, bh,
				    bl, dd);
				vis_pst_16(dd, dp, emask);
			}

		} else {
			vis_alignaddr(sp, 2);

			for (i = off + 1; i < 4; i++)
				*((mlib_f32 *)sf + i) = sl[i - off - 1];

			sd00 = vis_bshuffle(sf[0], sf[1]);
			sp += 2;

			if (emask != 0xf) {
				sd0 = (*sp++);
				sd1 = (*sp++);
				MLIB_V_IMAGESCALE_S32_S16_NA(sd0, sd1, ah, al,
				    bh, bl, dd);
				vis_pst_16(dd, dp, emask);
				dp++;
			}

			n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) >> 3;
#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				sd0 = (*sp++);
				sd1 = (*sp++);
				MLIB_V_IMAGESCALE_S32_S16_NA(sd0, sd1, ah, al,
				    bh, bl, dd);
				*dp = dd;
				dp++;
			}

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				MLIB_V_IMAGESCALE_S32_S16_NA(sd0, sd1, ah, al,
				    bh, bl, dd);
				vis_pst_16(dd, dp, emask);
			}
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

/* aligned src address */
	mlib_d64 *sp;

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
	mlib_f32 *sl, *spp;
	mlib_s16 *dl, *dend;
	mlib_d64 sf[4];
	mlib_s32 off, off0, i, n, emask;
	mlib_d64 b0, b1, b2, dtmp;
	mlib_f32 af0, af1, af2, ftmp;

	vis_write_bmask(0x014589CD, 0);

/* line addresses */
	sl = (mlib_f32 *)src;
	dl = (mlib_s16 *)dst;

	if ((2 * xsize == dlb) && (slb == 2 * dlb)) {
		xsize *= ysize;
		ysize = 1;
	}

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the source and destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_u8 *)dl - (mlib_u8 *)dp;
		dend = dl + xsize - 1;

		off >>= 1;

		if ((off & 1) == 0) {
			off0 = off >> 1;
			af0 = afh[off0];
			af1 = afl[off0];
			af2 = afh[off0 + 1];

			b0 = bdh[off0];
			b1 = bdl[off0];
			b2 = bdh[off0 + 1];
		} else {
			off0 = off >> 1;
			af0 = afl[off0];
			af1 = afh[off0 + 1];
			af2 = afl[off0 + 1];

			b0 = bdl[off0];
			b1 = bdh[off0 + 1];
			b2 = bdl[off0 + 1];
		}

		sp = (mlib_d64 *)vis_alignaddr(sl, -4 * off);
		emask = vis_edge16(dl, dend);

		if (((mlib_u8 *)sp - ((mlib_u8 *)sl - 4 * off)) == 0) {
			if (emask != 0xf) {
				for (i = off; i < 4; i++)
					*((mlib_f32 *)sf + i) = sl[i - off];

				MLIB_V_IMAGESCALE_S32_S16(sf[0], sf[1], af0,
				    af1, b0, b1, dd);
				vis_pst_16(dd, dp, emask);
				dp++;
				sp += 2;
				ftmp = af0;
				af0 = af2;
				af2 = af1;
				af1 = ftmp;
				dtmp = b0;
				b0 = b2;
				b2 = b1;
				b1 = dtmp;
			}

			n = (((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) / 24) * 3;

#pragma pipeloop(0)
			for (i = 0; i < n; i += 3) {
				sd0 = sp[2 * i];
				sd1 = sp[2 * i + 1];
				MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, af0, af1,
				    b0, b1, dd);
				dp[i] = dd;

				sd0 = sp[2 * i + 2];
				sd1 = sp[2 * i + 3];
				MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, af2, af0,
				    b2, b0, dd);
				dp[i + 1] = dd;

				sd0 = sp[2 * i + 4];
				sd1 = sp[2 * i + 5];
				MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, af1, af2,
				    b1, b2, dd);
				dp[i + 2] = dd;
			}

			dp += i;
			sp += 2 * i;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				sd0 = sp[0];
				sd1 = sp[1];
				MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, af0, af1,
				    b0, b1, dd);
				vis_pst_16(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge16(dp, dend);
					sd0 = sp[2];
					sd1 = sp[3];
					MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, af2,
					    af0, b2, b0, dd);
					vis_pst_16(dd, dp, emask);
					dp++;

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge16(dp, dend);
						sd0 = sp[4];
						sd1 = sp[5];
						MLIB_V_IMAGESCALE_S32_S16(sd0,
						    sd1, af1, af2, b1, b2, dd);
						vis_pst_16(dd, dp, emask);
					}
				}
			}

		} else {

			spp = (mlib_f32 *)sp + 1;

			if (emask != 0xf) {
				for (i = off; i < 4; i++)
					*((mlib_f32 *)sf + i) = sl[i - off];

				MLIB_V_IMAGESCALE_S32_S16(sf[0], sf[1], af0,
				    af1, b0, b1, dd);
				vis_pst_16(dd, dp, emask);
				dp++;
				spp += 4;
				ftmp = af0;
				af0 = af2;
				af2 = af1;
				af1 = ftmp;
				dtmp = b0;
				b0 = b2;
				b2 = b1;
				b1 = dtmp;
			}

			n = (((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) / 24) * 3;

#pragma pipeloop(0)
			for (i = 0; i < n; i += 3) {
				sd0 = vis_freg_pair(spp[0], spp[1]);
				sd1 = vis_freg_pair(spp[2], spp[3]);
				MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, af0, af1,
				    b0, b1, dd);
				dp[i] = dd;

				sd0 = vis_freg_pair(spp[4], spp[5]);
				sd1 = vis_freg_pair(spp[6], spp[7]);
				MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, af2, af0,
				    b2, b0, dd);
				dp[i + 1] = dd;

				sd0 = vis_freg_pair(spp[8], spp[9]);
				sd1 = vis_freg_pair(spp[10], spp[11]);
				MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, af1, af2,
				    b1, b2, dd);
				dp[i + 2] = dd;
				spp += 12;
			}

			dp += i;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				sd0 = vis_freg_pair(spp[0], spp[1]);
				sd1 = vis_freg_pair(spp[2], spp[3]);
				MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, af0, af1,
				    b0, b1, dd);
				vis_pst_16(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge16(dp, dend);
					sd0 = vis_freg_pair(spp[4], spp[5]);
					sd1 = vis_freg_pair(spp[6], spp[7]);
					MLIB_V_IMAGESCALE_S32_S16(sd0, sd1, af2,
					    af0, b2, b0, dd);
					vis_pst_16(dd, dp, emask);
					dp++;

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge16(dp, dend);
						sd0 =
						    vis_freg_pair(spp[8],
						    spp[9]);
						sd1 =
						    vis_freg_pair(spp[10],
						    spp[11]);
						MLIB_V_IMAGESCALE_S32_S16(sd0,
						    sd1, af1, af2, b1, b2, dd);
						vis_pst_16(dd, dp, emask);
					}
				}
			}
		}

		sl = (mlib_f32 *)((mlib_u8 *)sl + slb);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
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
	mlib_v_ImageScale_S32_S16_3(src, 0, dst, 0, dsize, 1, afh, afl, bdh,
	    bdl);
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
	mlib_d64 sd, sd0, sd1, sd00, sd01;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* loop variables */
	mlib_s32 j;
	mlib_f32 *sl;
	mlib_d64 *sp;
	mlib_s16 *dl, *dend;
	mlib_d64 bh, bl;
	mlib_f32 ah, al;
	mlib_d64 sf[2];
	mlib_s32 off, i, n, emask;
	mlib_d64 mask_8000 = vis_to_double_dup(0x80008000);

	sl = (mlib_f32 *)src;
	dl = (mlib_s16 *)dst;

/* prepare align for macro */
	vis_write_bmask(0x014589CD, 0);

	if ((2 * xsize == dlb) && (slb == 2 * dlb)) {
		xsize *= ysize;
		ysize = 1;
	}

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the source and destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_u8 *)dl - (mlib_u8 *)dp;
		dend = dl + xsize - 1;

/* prepare the alpha and beta values */
		off >>= 1;
		ah = afh[off];
		al = afl[off];
		bh = bdh[off];
		bl = bdl[off];

		sp = (mlib_d64 *)vis_alignaddr(sl, -4 * off);
		emask = vis_edge16(dl, dend);

		if (((mlib_u8 *)sp - ((mlib_u8 *)sl - 4 * off)) == 0) {
			if (emask != 0xf) {
				for (i = off; i < 4; i++)
					*((mlib_f32 *)sf + i) = sl[i - off];

				MLIB_V_IMAGESCALE_S32_U16(sf[0], sf[1], ah, al,
				    bh, bl, dd);
				vis_pst_16(dd, dp, emask);
				dp++;
				sp += 2;
			}

			n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) >> 3;
#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				sd0 = (*sp++);
				sd1 = (*sp++);
				MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, ah, al, bh,
				    bl, dd);
				*dp = dd;
				dp++;
			}

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				sd0 = (*sp++);
				sd1 = *sp;
				MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, ah, al, bh,
				    bl, dd);
				vis_pst_16(dd, dp, emask);
			}

		} else {
			vis_alignaddr(sp, 2);

			for (i = off + 1; i < 4; i++)
				*((mlib_f32 *)sf + i) = sl[i - off - 1];

			sd00 = vis_bshuffle(sf[0], sf[1]);
			sp += 2;

			if (emask != 0xf) {
				sd0 = (*sp++);
				sd1 = (*sp++);
				MLIB_V_IMAGESCALE_S32_U16_NA(sd0, sd1, ah, al,
				    bh, bl, dd);
				vis_pst_16(dd, dp, emask);
				dp++;
			}

			n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) >> 3;
#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				sd0 = (*sp++);
				sd1 = (*sp++);
				MLIB_V_IMAGESCALE_S32_U16_NA(sd0, sd1, ah, al,
				    bh, bl, dd);
				*dp = dd;
				dp++;
			}

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				MLIB_V_IMAGESCALE_S32_U16_NA(sd0, sd1, ah, al,
				    bh, bl, dd);
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

/* aligned src address */
	mlib_d64 *sp;

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
	mlib_f32 *sl, *spp;
	mlib_s16 *dl, *dend;
	mlib_d64 sf[4];
	mlib_s32 off, off0, i, n, emask;
	mlib_d64 b0, b1, b2, dtmp;
	mlib_f32 af0, af1, af2, ftmp;
	mlib_d64 mask_8000 = vis_to_double_dup(0x80008000);

	vis_write_bmask(0x014589CD, 0);

/* line addresses */
	sl = (mlib_f32 *)src;
	dl = (mlib_s16 *)dst;

	if ((2 * xsize == dlb) && (slb == 2 * dlb)) {
		xsize *= ysize;
		ysize = 1;
	}

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the source and destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_u8 *)dl - (mlib_u8 *)dp;
		dend = dl + xsize - 1;

		off >>= 1;

		if ((off & 1) == 0) {
			off0 = off >> 1;
			af0 = afh[off0];
			af1 = afl[off0];
			af2 = afh[off0 + 1];

			b0 = bdh[off0];
			b1 = bdl[off0];
			b2 = bdh[off0 + 1];
		} else {
			off0 = off >> 1;
			af0 = afl[off0];
			af1 = afh[off0 + 1];
			af2 = afl[off0 + 1];

			b0 = bdl[off0];
			b1 = bdh[off0 + 1];
			b2 = bdl[off0 + 1];
		}

		sp = (mlib_d64 *)vis_alignaddr(sl, -4 * off);
		emask = vis_edge16(dl, dend);

		if (((mlib_u8 *)sp - ((mlib_u8 *)sl - 4 * off)) == 0) {
			if (emask != 0xf) {
				for (i = off; i < 4; i++)
					*((mlib_f32 *)sf + i) = sl[i - off];

				MLIB_V_IMAGESCALE_S32_U16(sf[0], sf[1], af0,
				    af1, b0, b1, dd);
				vis_pst_16(dd, dp, emask);
				dp++;
				sp += 2;
				ftmp = af0;
				af0 = af2;
				af2 = af1;
				af1 = ftmp;
				dtmp = b0;
				b0 = b2;
				b2 = b1;
				b1 = dtmp;
			}

			n = (((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) / 24) * 3;

#pragma pipeloop(0)
			for (i = 0; i < n; i += 3) {
				sd0 = sp[2 * i];
				sd1 = sp[2 * i + 1];
				MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, af0, af1,
				    b0, b1, dd);
				dp[i] = dd;

				sd0 = sp[2 * i + 2];
				sd1 = sp[2 * i + 3];
				MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, af2, af0,
				    b2, b0, dd);
				dp[i + 1] = dd;

				sd0 = sp[2 * i + 4];
				sd1 = sp[2 * i + 5];
				MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, af1, af2,
				    b1, b2, dd);
				dp[i + 2] = dd;
			}

			dp += i;
			sp += 2 * i;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				sd0 = sp[0];
				sd1 = sp[1];
				MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, af0, af1,
				    b0, b1, dd);
				vis_pst_16(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge16(dp, dend);
					sd0 = sp[2];
					sd1 = sp[3];
					MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, af2,
					    af0, b2, b0, dd);
					vis_pst_16(dd, dp, emask);
					dp++;

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge16(dp, dend);
						sd0 = sp[4];
						sd1 = sp[5];
						MLIB_V_IMAGESCALE_S32_U16(sd0,
						    sd1, af1, af2, b1, b2, dd);
						vis_pst_16(dd, dp, emask);
					}
				}
			}

		} else {

			spp = (mlib_f32 *)sp + 1;

			if (emask != 0xf) {
				for (i = off; i < 4; i++)
					*((mlib_f32 *)sf + i) = sl[i - off];

				MLIB_V_IMAGESCALE_S32_U16(sf[0], sf[1], af0,
				    af1, b0, b1, dd);
				vis_pst_16(dd, dp, emask);
				dp++;
				spp += 4;
				ftmp = af0;
				af0 = af2;
				af2 = af1;
				af1 = ftmp;
				dtmp = b0;
				b0 = b2;
				b2 = b1;
				b1 = dtmp;
			}

			n = (((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) / 24) * 3;

#pragma pipeloop(0)
			for (i = 0; i < n; i += 3) {
				sd0 = vis_freg_pair(spp[0], spp[1]);
				sd1 = vis_freg_pair(spp[2], spp[3]);
				MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, af0, af1,
				    b0, b1, dd);
				dp[i] = dd;

				sd0 = vis_freg_pair(spp[4], spp[5]);
				sd1 = vis_freg_pair(spp[6], spp[7]);
				MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, af2, af0,
				    b2, b0, dd);
				dp[i + 1] = dd;

				sd0 = vis_freg_pair(spp[8], spp[9]);
				sd1 = vis_freg_pair(spp[10], spp[11]);
				MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, af1, af2,
				    b1, b2, dd);
				dp[i + 2] = dd;
				spp += 12;
			}

			dp += i;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				sd0 = vis_freg_pair(spp[0], spp[1]);
				sd1 = vis_freg_pair(spp[2], spp[3]);
				MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, af0, af1,
				    b0, b1, dd);
				vis_pst_16(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge16(dp, dend);
					sd0 = vis_freg_pair(spp[4], spp[5]);
					sd1 = vis_freg_pair(spp[6], spp[7]);
					MLIB_V_IMAGESCALE_S32_U16(sd0, sd1, af2,
					    af0, b2, b0, dd);
					vis_pst_16(dd, dp, emask);
					dp++;

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge16(dp, dend);
						sd0 =
						    vis_freg_pair(spp[8],
						    spp[9]);
						sd1 =
						    vis_freg_pair(spp[10],
						    spp[11]);
						MLIB_V_IMAGESCALE_S32_U16(sd0,
						    sd1, af1, af2, b1, b2, dd);
						vis_pst_16(dd, dp, emask);
					}
				}
			}
		}

		sl = (mlib_f32 *)((mlib_u8 *)sl + slb);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

#define	MLIB_V_IMAGESCALE_S16_U8(afh, afl, bdh, bdl)            \
	rdhh = vis_fmuld8sux16(vis_read_hi(sd), afh);           \
	rdhl = vis_fmuld8ulx16(vis_read_hi(sd), afh);           \
	rdh = vis_fpadd32(rdhh, rdhl);                          \
	rdh = vis_fpadd32(rdh, bdh);                            \
	ddh = vis_fpack32(ddh, rdh);                            \
	rdlh = vis_fmuld8sux16(vis_read_lo(sd), afl);           \
	rdll = vis_fmuld8ulx16(vis_read_lo(sd), afl);           \
	rdl = vis_fpadd32(rdlh, rdll);                          \
	rdl = vis_fpadd32(rdl, bdl);                            \
	ddh = vis_fpack32(ddh, rdl)

/* *********************************************************** */

#define	MLIB_V_IMAGESCALE_S32_U8(sd0, sd1, afh, afl, bdh, bdl)   \
	sd = vis_bshuffle(sd0, sd1);                             \
	MLIB_V_IMAGESCALE_S16_U8(afh, afl, bdh, bdl)

/* *********************************************************** */

#define	MLIB_V_IMAGESCALE_S32_U8_NA(sd0, sd1, afh, afl, bdh, bdl)   \
	sd01 = vis_bshuffle(sd0, sd1);                              \
	sd = vis_faligndata(sd00, sd01);                            \
	sd00 = sd01;                                                \
	MLIB_V_IMAGESCALE_S16_U8(afh, afl, bdh, bdl)

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
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd0, sd1, sd;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* temporaries used in macro */
	mlib_d64 ddh;

/* loop variables */
	mlib_s32 j;
	mlib_d64 dd;

	vis_write_bmask(0x014589CD, 0);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

/* 4-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j <= (dsize - 8); j += 8) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		MLIB_V_IMAGESCALE_S32_U8(sd0, sd1, vis_read_hi(ad),
		    vis_read_lo(ad), bdh, bdl);
		sd0 = (*sp++);
		sd1 = (*sp++);
		MLIB_V_IMAGESCALE_S32_U8(sd0, sd1, vis_read_hi(ad),
		    vis_read_lo(ad), bdh, bdl);

		dd = vis_fpmerge(vis_read_hi(ddh), vis_read_lo(ddh));
		(*dp++) = dd;
	}

	if (j < dsize) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		MLIB_V_IMAGESCALE_S32_U8(sd0, sd1, vis_read_hi(ad),
		    vis_read_lo(ad), bdh, bdl);
		*(mlib_f32 *)dp =
		    vis_read_lo(vis_fpmerge(vis_read_hi(ddh),
		    vis_read_lo(ddh)));
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
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd, sd0, sd1, sd00, sd01;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* loop variables */
	mlib_s32 j;
	mlib_f32 *sl;
	mlib_u8 *dl, *dend;
	mlib_d64 bh, bl;
	mlib_f32 ah, al;
	mlib_d64 sf[4], *sp;
	mlib_s32 off, i, n, emask;
	mlib_d64 ddh;

	vis_write_bmask(0x014589CD, 0);

	sl = (mlib_f32 *)src;
	dl = (mlib_u8 *)dst;

	if ((xsize == dlb) && (slb == 4 * dlb)) {
		xsize *= ysize;
		ysize = 1;
	}

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the source and destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_u8 *)dl - (mlib_u8 *)dp;
		dend = dl + xsize - 1;

/* prepare the alpha and beta values */
		ah = afh[off & 0x3];
		al = afl[off & 0x3];
		bh = bdh[off & 0x3];
		bl = bdl[off & 0x3];

		sp = vis_alignaddr(sl, -4 * off);
		emask = vis_edge8(dl, dend);

		if (((mlib_u8 *)sp - ((mlib_u8 *)sl - 4 * off)) == 0) {
			if (emask != 0xff) {
				for (i = off; i < 8; i++)
					*((mlib_f32 *)sf + i) = sl[i - off];

				MLIB_V_IMAGESCALE_S32_U8(sf[0], sf[1], ah, al,
				    bh, bl);
				MLIB_V_IMAGESCALE_S32_U8(sf[2], sf[3], ah, al,
				    bh, bl);

				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));

				vis_pst_8(dd, dp, emask);
				dp++;
				sp += 4;
			}

			n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) >> 3;

#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				sd0 = (*sp++);
				sd1 = (*sp++);
				MLIB_V_IMAGESCALE_S32_U8(sd0, sd1, ah, al, bh,
				    bl);
				sd0 = (*sp++);
				sd1 = (*sp++);
				MLIB_V_IMAGESCALE_S32_U8(sd0, sd1, ah, al, bh,
				    bl);

				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
				(*dp++) = dd;
			}

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);

				sd0 = (*sp++);
				sd1 = (*sp++);
				MLIB_V_IMAGESCALE_S32_U8(sd0, sd1, ah, al, bh,
				    bl);
				sd0 = (*sp++);
				sd1 = (*sp++);
				MLIB_V_IMAGESCALE_S32_U8(sd0, sd1, ah, al, bh,
				    bl);

				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
				vis_pst_8(dd, dp, emask);
			}

		} else {
			vis_alignaddr(sp, 2);

			for (i = off + 1; i < 8; i++)
				*((mlib_f32 *)sf + i) = sl[i - off - 1];

			sd00 = vis_bshuffle(sf[0], sf[1]);
			sp += 2;

			if (emask != 0xff) {
				sp += 2;
				MLIB_V_IMAGESCALE_S32_U8_NA(sf[2], sf[3], ah,
				    al, bh, bl);
				sd0 = (*sp++);
				sd1 = (*sp++);
				MLIB_V_IMAGESCALE_S32_U8_NA(sd0, sd1, ah, al,
				    bh, bl);
				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));

				vis_pst_8(dd, dp, emask);
				dp++;
			}

			n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) >> 3;

#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				sd0 = (*sp++);
				sd1 = (*sp++);
				MLIB_V_IMAGESCALE_S32_U8_NA(sd0, sd1, ah, al,
				    bh, bl);
				sd0 = (*sp++);
				sd1 = (*sp++);
				MLIB_V_IMAGESCALE_S32_U8_NA(sd0, sd1, ah, al,
				    bh, bl);

				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
				(*dp++) = dd;
			}

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);

				sd0 = (*sp++);
				sd1 = (*sp++);
				MLIB_V_IMAGESCALE_S32_U8_NA(sd0, sd1, ah, al,
				    bh, bl);
				MLIB_V_IMAGESCALE_S32_U8_NA(sd0, sd1, ah, al,
				    bh, bl);

				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
				vis_pst_8(dd, dp, emask);
			}
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
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd, sd0, sd1, sd00, sd01;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl, rdh;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll, rdl;

/* loop variables */
	mlib_s32 j;
	mlib_f32 *sl;
	mlib_d64 *sp;
	mlib_u8 *dl, *dend;
	mlib_d64 sf[4];
	mlib_s32 off, off0, i, n, emask;
	mlib_d64 ddh;

	mlib_f32 af0, af1, af2, ftmp;
	mlib_d64 b0, b1, b2, dtmp;

	vis_write_bmask(0x014589CD, 0);

/* line addresses */
	sl = (mlib_f32 *)src;
	dl = (mlib_u8 *)dst;

	if ((xsize == dlb) && (slb == 4 * dlb)) {
		xsize *= ysize;
		ysize = 1;
	}

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the source and destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_u8 *)dl - (mlib_u8 *)dp;
		dend = dl + xsize - 1;

		off0 = off - (off / 3) * 3;

		if (off0 == 1) {
			af0 = afl[0];
			af1 = afh[1];
			af2 = afl[1];

			b0 = bdl[0];
			b1 = bdh[1];
			b2 = bdl[1];

		} else {
			off0 >>= 1;
			af0 = afh[off0];
			af1 = afl[off0];
			af2 = afh[off0 + 1];

			b0 = bdh[off0];
			b1 = bdl[off0];
			b2 = bdh[off0 + 1];
		}

		sp = vis_alignaddr(sl, -4 * off);
		emask = vis_edge8(dl, dend);

		if (((mlib_u8 *)sp - ((mlib_u8 *)sl - 4 * off)) == 0) {
			if (emask != 0xff) {
				for (i = off; i < 8; i++)
					*((mlib_f32 *)sf + i) = sl[i - off];

				MLIB_V_IMAGESCALE_S32_U8(sf[0], sf[1], af0, af1,
				    b0, b1);
				MLIB_V_IMAGESCALE_S32_U8(sf[2], sf[3], af2, af0,
				    b2, b0);

				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));

				vis_pst_8(dd, dp, emask);
				dp++;
				sp += 4;

				ftmp = af0;
				af0 = af1;
				af1 = af2;
				af2 = ftmp;
				dtmp = b0;
				b0 = b1;
				b1 = b2;
				b2 = dtmp;
			}

			n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) >> 3;

#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				sd0 = (*sp++);
				sd1 = (*sp++);
				MLIB_V_IMAGESCALE_S32_U8(sd0, sd1, af0, af1, b0,
				    b1);
				sd0 = (*sp++);
				sd1 = (*sp++);
				MLIB_V_IMAGESCALE_S32_U8(sd0, sd1, af2, af0, b2,
				    b0);

				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
				(*dp++) = dd;

				ftmp = af0;
				af0 = af1;
				af1 = af2;
				af2 = ftmp;
				dtmp = b0;
				b0 = b1;
				b1 = b2;
				b2 = dtmp;
			}

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);

				sd0 = (*sp++);
				sd1 = (*sp++);
				MLIB_V_IMAGESCALE_S32_U8(sd0, sd1, af0, af1, b0,
				    b1);
				sd0 = (*sp++);
				sd1 = (*sp++);
				MLIB_V_IMAGESCALE_S32_U8(sd0, sd1, af2, af0, b2,
				    b0);

				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
				vis_pst_8(dd, dp, emask);
			}

		} else {
			vis_alignaddr(sp, 2);

			for (i = off + 1; i < 8; i++)
				*((mlib_f32 *)sf + i) = sl[i - off - 1];

			sd00 = vis_bshuffle(sf[0], sf[1]);
			sp += 2;

			if (emask != 0xff) {
				sp += 2;
				MLIB_V_IMAGESCALE_S32_U8_NA(sf[2], sf[3], af0,
				    af1, b0, b1);
				sd0 = (*sp++);
				sd1 = (*sp++);
				MLIB_V_IMAGESCALE_S32_U8_NA(sd0, sd1, af2, af0,
				    b2, b0);
				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));

				vis_pst_8(dd, dp, emask);
				dp++;

				ftmp = af0;
				af0 = af1;
				af1 = af2;
				af2 = ftmp;
				dtmp = b0;
				b0 = b1;
				b1 = b2;
				b2 = dtmp;
			}

			n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) >> 3;

#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				sd0 = (*sp++);
				sd1 = (*sp++);
				MLIB_V_IMAGESCALE_S32_U8_NA(sd0, sd1, af0, af1,
				    b0, b1);
				sd0 = (*sp++);
				sd1 = (*sp++);
				MLIB_V_IMAGESCALE_S32_U8_NA(sd0, sd1, af2, af0,
				    b2, b0);

				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
				(*dp++) = dd;

				ftmp = af0;
				af0 = af1;
				af1 = af2;
				af2 = ftmp;
				dtmp = b0;
				b0 = b1;
				b1 = b2;
				b2 = dtmp;
			}

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);

				sd0 = (*sp++);
				sd1 = (*sp++);
				MLIB_V_IMAGESCALE_S32_U8_NA(sd0, sd1, af0, af1,
				    b0, b1);
				MLIB_V_IMAGESCALE_S32_U8_NA(sd0, sd1, af2, af0,
				    b2, b0);

				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
				vis_pst_8(dd, dp, emask);
			}
		}

		sl = (mlib_f32 *)((mlib_u8 *)sl + slb);
		dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
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
	mlib_v_ImageScale_S32_U8_3(src, 0, dst, 0, dsize, 1, afh, afl, bdh,
	    bdl);
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
	mlib_d64 *dp;

/* pointer to begin of a src line */
	mlib_s16 *sl;

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

	if ((xsize == dlb) && (slb == 2 * dlb)) {
		xsize *= ysize;
		ysize = 1;
	}

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
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
		off = (mlib_u8 *)sp - ((mlib_u8 *)sl + off);
		emask = vis_edge8(dl, dend);

		if (off == 0) {
			if (emask != 0xff) {
				sd = (*sp++);
				MLIB_V_IMAGESCALE_S16_U8(vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl);
				sd = (*sp++);
				MLIB_V_IMAGESCALE_S16_U8(vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl);
				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
				vis_pst_8(dd, dp, emask);
				dp++;
			}

/* 8-pixel column loop */
			n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 1) >> 3;

#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				sd = (*sp++);
				MLIB_V_IMAGESCALE_S16_U8(vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl);
				sd = (*sp++);
				MLIB_V_IMAGESCALE_S16_U8(vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl);
				(*dp++) =
				    vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
			}

/* last 8-pixel column */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				sd = (*sp++);
				MLIB_V_IMAGESCALE_S16_U8(vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl);
				sd = (*sp++);
				MLIB_V_IMAGESCALE_S16_U8(vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl);
				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
				vis_pst_8(dd, dp, emask);
			}

		} else {
			s0 = (*sp++);

			if (emask != 0xff) {
				s1 = (*sp++);
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_S16_U8(vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl);
				s0 = (*sp++);
				sd = vis_faligndata(s1, s0);
				MLIB_V_IMAGESCALE_S16_U8(vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl);
				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
				vis_pst_8(dd, dp, emask);
				dp++;
			}

/* 8-pixel column loop */
			n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 1) >> 3;

#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				s1 = (*sp++);
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_S16_U8(vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl);
				s0 = (*sp++);
				sd = vis_faligndata(s1, s0);
				MLIB_V_IMAGESCALE_S16_U8(vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl);
				(*dp++) =
				    vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
			}

/* last 8-pixel column */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				s1 = (*sp++);
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_S16_U8(vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl);
				s0 = (*sp++);
				sd = vis_faligndata(s1, s0);
				MLIB_V_IMAGESCALE_S16_U8(vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl);
				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
				vis_pst_8(dd, dp, emask);
			}
		}

		sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dl += dlb;
	}
}

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
	mlib_v_ImageScale_S16_U8_124(src, 0, dst, 0, dsize, 1, ad, bdh, bdl);
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
	mlib_v_ImageScale_S16_U8_124(src, slb, dst, dlb, xsize, ysize, ad, bdh,
	    bdl);
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
	mlib_d64 *dp;

/* pointer to src line */
	mlib_s16 *sl;

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
	mlib_d64 bdm, dtmp, b[10];

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
	mlib_f32 adf0, adf1, adf2, ftmp;

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

	if ((xsize == dlb) && (slb == 2 * dlb)) {
		xsize *= ysize;
		ysize = 1;
	}

/* row loop */
	for (j = 0; j < ysize; j++) {

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

		adf0 = vis_read_hi(ad);
		adf1 = vis_read_lo(ad);
		adf2 = vis_read_hi(ad1);

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, 2 * off);
		off = (mlib_u8 *)sp - ((mlib_u8 *)sl + 2 * off);
		emask = vis_edge8(dl, dend);

		if (off == 0) {
			if (emask != 0xff) {
				sd = (*sp++);
				MLIB_V_IMAGESCALE_S16_U8(adf0, adf1, bdh, bdl);
				sd = (*sp++);
				MLIB_V_IMAGESCALE_S16_U8(adf2, adf0, bdm, bdh);
				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
				vis_pst_8(dd, dp, emask);
				dp++;
				ftmp = adf0;
				adf0 = adf1;
				adf1 = adf2;
				adf2 = ftmp;
				dtmp = bdh;
				bdh = bdl;
				bdl = bdm;
				bdm = dtmp;
			}

/* 8-pixel column loop */
			n = (((mlib_u8 *)dend - (mlib_u8 *)dp + 1) / 24) * 3;

#pragma pipeloop(0)
			for (i = 0; i < n; i += 3) {
				sd = sp[2 * i];
				MLIB_V_IMAGESCALE_S16_U8(adf0, adf1, bdh, bdl);
				sd = sp[2 * i + 1];
				MLIB_V_IMAGESCALE_S16_U8(adf2, adf0, bdm, bdh);
				dp[i] =
				    vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
			}

#pragma pipeloop(0)
			for (i = 0; i < n; i += 3) {
				sd = sp[2 * i + 2];
				MLIB_V_IMAGESCALE_S16_U8(adf1, adf2, bdl, bdm);
				sd = sp[2 * i + 3];
				MLIB_V_IMAGESCALE_S16_U8(adf0, adf1, bdh, bdl);
				dp[i + 1] =
				    vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
			}

#pragma pipeloop(0)
			for (i = 0; i < n; i += 3) {
				sd = sp[2 * i + 4];
				MLIB_V_IMAGESCALE_S16_U8(adf2, adf0, bdm, bdh);
				sd = sp[2 * i + 5];
				MLIB_V_IMAGESCALE_S16_U8(adf1, adf2, bdl, bdm);
				dp[i + 2] =
				    vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
			}

			dp += i;

/* last 8-pixel column */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				sd = sp[2 * i];
				MLIB_V_IMAGESCALE_S16_U8(adf0, adf1, bdh, bdl);
				sd = sp[2 * i + 1];
				MLIB_V_IMAGESCALE_S16_U8(adf2, adf0, bdm, bdh);
				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
				vis_pst_8(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge8(dp, dend);
					sd = sp[2 * i + 2];
					MLIB_V_IMAGESCALE_S16_U8(adf1, adf2,
					    bdl, bdm);
					sd = sp[2 * i + 3];
					MLIB_V_IMAGESCALE_S16_U8(adf0, adf1,
					    bdh, bdl);
					dd = vis_fpmerge(vis_read_hi(ddh),
					    vis_read_lo(ddh));
					vis_pst_8(dd, dp, emask);
					dp++;

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge8(dp, dend);
						sd = sp[2 * i + 4];
						MLIB_V_IMAGESCALE_S16_U8(adf2,
						    adf0, bdm, bdh);
						sd = sp[2 * i + 5];
						MLIB_V_IMAGESCALE_S16_U8(adf1,
						    adf2, bdl, bdm);
						dd = vis_fpmerge(vis_read_hi
						    (ddh), vis_read_lo(ddh));
						vis_pst_8(dd, dp, emask);
					}
				}
			}

		} else {

			if (emask != 0xff) {
				s0 = (*sp++);
				s1 = (*sp++);
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_S16_U8(adf0, adf1, bdh, bdl);
				s0 = *sp;
				sd = vis_faligndata(s1, s0);
				MLIB_V_IMAGESCALE_S16_U8(adf2, adf0, bdm, bdh);
				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
				vis_pst_8(dd, dp, emask);
				dp++;
				ftmp = adf0;
				adf0 = adf1;
				adf1 = adf2;
				adf2 = ftmp;
				dtmp = bdh;
				bdh = bdl;
				bdl = bdm;
				bdm = dtmp;
			}

/* 8-pixel column loop */
			n = (((mlib_u8 *)dend - (mlib_u8 *)dp + 1) / 24) * 3;

#pragma pipeloop(0)
			for (i = 0; i < n; i += 3) {
				s0 = sp[2 * i];
				s1 = sp[2 * i + 1];
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_S16_U8(adf0, adf1, bdh, bdl);
				s0 = sp[2 * i + 2];
				sd = vis_faligndata(s1, s0);
				MLIB_V_IMAGESCALE_S16_U8(adf2, adf0, bdm, bdh);
				dp[i] =
				    vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
			}

#pragma pipeloop(0)
			for (i = 0; i < n; i += 3) {
				s0 = sp[2 * i + 2];
				s1 = sp[2 * i + 3];
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_S16_U8(adf1, adf2, bdl, bdm);
				s0 = sp[2 * i + 4];
				sd = vis_faligndata(s1, s0);
				MLIB_V_IMAGESCALE_S16_U8(adf0, adf1, bdh, bdl);
				dp[i + 1] =
				    vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
			}

#pragma pipeloop(0)
			for (i = 0; i < n; i += 3) {
				s0 = sp[2 * i + 4];
				s1 = sp[2 * i + 5];
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_S16_U8(adf2, adf0, bdm, bdh);
				s0 = sp[2 * i + 6];
				sd = vis_faligndata(s1, s0);
				MLIB_V_IMAGESCALE_S16_U8(adf1, adf2, bdl, bdm);
				dp[i + 2] =
				    vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
			}

			dp += i;

/* last 8-pixel column */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				s0 = sp[2 * i];
				s1 = sp[2 * i + 1];
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_S16_U8(adf0, adf1, bdh, bdl);
				s0 = sp[2 * i + 2];
				sd = vis_faligndata(s1, s0);
				MLIB_V_IMAGESCALE_S16_U8(adf2, adf0, bdm, bdh);
				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
				vis_pst_8(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge8(dp, dend);
					s1 = sp[2 * i + 3];
					sd = vis_faligndata(s0, s1);
					MLIB_V_IMAGESCALE_S16_U8(adf1, adf2,
					    bdl, bdm);
					s0 = sp[2 * i + 4];
					sd = vis_faligndata(s1, s0);
					MLIB_V_IMAGESCALE_S16_U8(adf0, adf1,
					    bdh, bdl);
					dd = vis_fpmerge(vis_read_hi(ddh),
					    vis_read_lo(ddh));
					vis_pst_8(dd, dp, emask);
					dp++;

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge8(dp, dend);
						s1 = sp[2 * i + 5];
						sd = vis_faligndata(s0, s1);
						MLIB_V_IMAGESCALE_S16_U8(adf2,
						    adf0, bdm, bdh);
						s0 = sp[2 * i + 6];
						sd = vis_faligndata(s1, s0);
						MLIB_V_IMAGESCALE_S16_U8(adf1,
						    adf2, bdl, bdm);
						dd = vis_fpmerge(vis_read_hi
						    (ddh), vis_read_lo(ddh));
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

void
mlib_v_ImageScale_S16_U8_3_A8D1(
    mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl)
{
	mlib_v_ImageScale_S16_U8_3(src, 0, dst, 0, dsize, 1, ad, bdh, bdl);
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
	mlib_v_ImageScale_S16_U8_3(src, slb, dst, dlb, xsize, ysize, ad, bdh,
	    bdl);
}

/* *********************************************************** */

#define	MLIB_V_IMAGESCALE_U16_U8(afh, afl, bdh, bdl)            \
	sd = vis_fpsub16(sd, mask_8000);                        \
	MLIB_V_IMAGESCALE_S16_U8(afh, afl, bdh, bdl)

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
	mlib_d64 *dp;

/* pointer to begin of a src line */
	mlib_s16 *sl;

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

	if ((xsize == dlb) && (slb == 2 * dlb)) {
		xsize *= ysize;
		ysize = 1;
	}

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
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
		off = (mlib_u8 *)sp - ((mlib_u8 *)sl + off);
		emask = vis_edge8(dl, dend);

		if (off == 0) {
			if (emask != 0xff) {
				sd = (*sp++);
				MLIB_V_IMAGESCALE_U16_U8(vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl);
				sd = (*sp++);
				MLIB_V_IMAGESCALE_U16_U8(vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl);
				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
				vis_pst_8(dd, dp, emask);
				dp++;
			}

/* 8-pixel column loop */
			n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 1) >> 3;

#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				sd = (*sp++);
				MLIB_V_IMAGESCALE_U16_U8(vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl);
				sd = (*sp++);
				MLIB_V_IMAGESCALE_U16_U8(vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl);
				(*dp++) =
				    vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
			}

/* last 8-pixel column */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				sd = (*sp++);
				MLIB_V_IMAGESCALE_U16_U8(vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl);
				sd = (*sp++);
				MLIB_V_IMAGESCALE_U16_U8(vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl);
				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
				vis_pst_8(dd, dp, emask);
			}

		} else {
			s0 = (*sp++);

			if (emask != 0xff) {
				s1 = (*sp++);
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_U16_U8(vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl);
				s0 = (*sp++);
				sd = vis_faligndata(s1, s0);
				MLIB_V_IMAGESCALE_U16_U8(vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl);
				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
				vis_pst_8(dd, dp, emask);
				dp++;
			}

/* 8-pixel column loop */
			n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 1) >> 3;

#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				s1 = (*sp++);
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_U16_U8(vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl);
				s0 = (*sp++);
				sd = vis_faligndata(s1, s0);
				MLIB_V_IMAGESCALE_U16_U8(vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl);
				(*dp++) =
				    vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
			}

/* last 8-pixel column */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				s1 = (*sp++);
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_U16_U8(vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl);
				s0 = (*sp++);
				sd = vis_faligndata(s1, s0);
				MLIB_V_IMAGESCALE_U16_U8(vis_read_hi(ad0),
				    vis_read_lo(ad0), bdh, bdl);
				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
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
	mlib_d64 *dp;

/* pointer to src line */
	mlib_s16 *sl;

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
	mlib_d64 bdm, dtmp, b[10];

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
	mlib_f32 adf0, adf1, adf2, ftmp;
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

	if ((xsize == dlb) && (slb == 2 * dlb)) {
		xsize *= ysize;
		ysize = 1;
	}

/* row loop */
	for (j = 0; j < ysize; j++) {

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

		adf0 = vis_read_hi(ad);
		adf1 = vis_read_lo(ad);
		adf2 = vis_read_hi(ad1);

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, 2 * off);
		off = (mlib_u8 *)sp - ((mlib_u8 *)sl + 2 * off);
		emask = vis_edge8(dl, dend);

		if (off == 0) {
			if (emask != 0xff) {
				sd = (*sp++);
				MLIB_V_IMAGESCALE_U16_U8(adf0, adf1, bdh, bdl);
				sd = (*sp++);
				MLIB_V_IMAGESCALE_U16_U8(adf2, adf0, bdm, bdh);
				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
				vis_pst_8(dd, dp, emask);
				dp++;
				ftmp = adf0;
				adf0 = adf1;
				adf1 = adf2;
				adf2 = ftmp;
				dtmp = bdh;
				bdh = bdl;
				bdl = bdm;
				bdm = dtmp;
			}

/* 8-pixel column loop */
			n = (((mlib_u8 *)dend - (mlib_u8 *)dp + 1) / 24) * 3;

#pragma pipeloop(0)
			for (i = 0; i < n; i += 3) {
				sd = sp[2 * i];
				MLIB_V_IMAGESCALE_U16_U8(adf0, adf1, bdh, bdl);
				sd = sp[2 * i + 1];
				MLIB_V_IMAGESCALE_U16_U8(adf2, adf0, bdm, bdh);
				dp[i] =
				    vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
			}

#pragma pipeloop(0)
			for (i = 0; i < n; i += 3) {
				sd = sp[2 * i + 2];
				MLIB_V_IMAGESCALE_U16_U8(adf1, adf2, bdl, bdm);
				sd = sp[2 * i + 3];
				MLIB_V_IMAGESCALE_U16_U8(adf0, adf1, bdh, bdl);
				dp[i + 1] =
				    vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
			}

#pragma pipeloop(0)
			for (i = 0; i < n; i += 3) {
				sd = sp[2 * i + 4];
				MLIB_V_IMAGESCALE_U16_U8(adf2, adf0, bdm, bdh);
				sd = sp[2 * i + 5];
				MLIB_V_IMAGESCALE_U16_U8(adf1, adf2, bdl, bdm);
				dp[i + 2] =
				    vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
			}

			dp += i;

/* last 8-pixel column */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				sd = sp[2 * i];
				MLIB_V_IMAGESCALE_U16_U8(adf0, adf1, bdh, bdl);
				sd = sp[2 * i + 1];
				MLIB_V_IMAGESCALE_U16_U8(adf2, adf0, bdm, bdh);
				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
				vis_pst_8(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge8(dp, dend);
					sd = sp[2 * i + 2];
					MLIB_V_IMAGESCALE_U16_U8(adf1, adf2,
					    bdl, bdm);
					sd = sp[2 * i + 3];
					MLIB_V_IMAGESCALE_U16_U8(adf0, adf1,
					    bdh, bdl);
					dd = vis_fpmerge(vis_read_hi(ddh),
					    vis_read_lo(ddh));
					vis_pst_8(dd, dp, emask);
					dp++;

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge8(dp, dend);
						sd = sp[2 * i + 4];
						MLIB_V_IMAGESCALE_U16_U8(adf2,
						    adf0, bdm, bdh);
						sd = sp[2 * i + 5];
						MLIB_V_IMAGESCALE_U16_U8(adf1,
						    adf2, bdl, bdm);
						dd = vis_fpmerge(vis_read_hi
						    (ddh), vis_read_lo(ddh));
						vis_pst_8(dd, dp, emask);
					}
				}
			}

		} else {

			if (emask != 0xff) {
				s0 = (*sp++);
				s1 = (*sp++);
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_U16_U8(adf0, adf1, bdh, bdl);
				s0 = *sp;
				sd = vis_faligndata(s1, s0);
				MLIB_V_IMAGESCALE_U16_U8(adf2, adf0, bdm, bdh);
				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
				vis_pst_8(dd, dp, emask);
				dp++;
				ftmp = adf0;
				adf0 = adf1;
				adf1 = adf2;
				adf2 = ftmp;
				dtmp = bdh;
				bdh = bdl;
				bdl = bdm;
				bdm = dtmp;
			}

/* 8-pixel column loop */
			n = (((mlib_u8 *)dend - (mlib_u8 *)dp + 1) / 24) * 3;

#pragma pipeloop(0)
			for (i = 0; i < n; i += 3) {
				s0 = sp[2 * i];
				s1 = sp[2 * i + 1];
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_U16_U8(adf0, adf1, bdh, bdl);
				s0 = sp[2 * i + 2];
				sd = vis_faligndata(s1, s0);
				MLIB_V_IMAGESCALE_U16_U8(adf2, adf0, bdm, bdh);
				dp[i] =
				    vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
			}

#pragma pipeloop(0)
			for (i = 0; i < n; i += 3) {
				s0 = sp[2 * i + 2];
				s1 = sp[2 * i + 3];
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_U16_U8(adf1, adf2, bdl, bdm);
				s0 = sp[2 * i + 4];
				sd = vis_faligndata(s1, s0);
				MLIB_V_IMAGESCALE_U16_U8(adf0, adf1, bdh, bdl);
				dp[i + 1] =
				    vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
			}

#pragma pipeloop(0)
			for (i = 0; i < n; i += 3) {
				s0 = sp[2 * i + 4];
				s1 = sp[2 * i + 5];
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_U16_U8(adf2, adf0, bdm, bdh);
				s0 = sp[2 * i + 6];
				sd = vis_faligndata(s1, s0);
				MLIB_V_IMAGESCALE_U16_U8(adf1, adf2, bdl, bdm);
				dp[i + 2] =
				    vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
			}

			dp += i;

/* last 8-pixel column */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				s0 = sp[2 * i];
				s1 = sp[2 * i + 1];
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_U16_U8(adf0, adf1, bdh, bdl);
				s0 = sp[2 * i + 2];
				sd = vis_faligndata(s1, s0);
				MLIB_V_IMAGESCALE_U16_U8(adf2, adf0, bdm, bdh);
				dd = vis_fpmerge(vis_read_hi(ddh),
				    vis_read_lo(ddh));
				vis_pst_8(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge8(dp, dend);
					s1 = sp[2 * i + 3];
					sd = vis_faligndata(s0, s1);
					MLIB_V_IMAGESCALE_U16_U8(adf1, adf2,
					    bdl, bdm);
					s0 = sp[2 * i + 4];
					sd = vis_faligndata(s1, s0);
					MLIB_V_IMAGESCALE_U16_U8(adf0, adf1,
					    bdh, bdl);
					dd = vis_fpmerge(vis_read_hi(ddh),
					    vis_read_lo(ddh));
					vis_pst_8(dd, dp, emask);
					dp++;

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge8(dp, dend);
						s1 = sp[2 * i + 5];
						sd = vis_faligndata(s0, s1);
						MLIB_V_IMAGESCALE_U16_U8(adf2,
						    adf0, bdm, bdh);
						s0 = sp[2 * i + 6];
						sd = vis_faligndata(s1, s0);
						MLIB_V_IMAGESCALE_U16_U8(adf1,
						    adf2, bdl, bdm);
						dd = vis_fpmerge(vis_read_hi
						    (ddh), vis_read_lo(ddh));
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
