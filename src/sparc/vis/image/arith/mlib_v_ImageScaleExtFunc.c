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

#pragma ident	"@(#)mlib_v_ImageScaleExtFunc.c	9.3	07/11/05 SMI"

/*
 * Internal functions.
 *
 *     void mlib_v_ImageScale_U8_S16_124_A8D1X8(mlib_u8 *src,
 *                                              mlib_s16 *dst,
 *                                              mlib_s32 dsize,
 *                                              mlib_d64 ad,
 *                                              mlib_d64 bdh,
 *                                              mlib_d64 bdl);
 *
 *     void mlib_v_ImageScale_U8_S16_124_A8D2(mlib_u8 *src,
 *                                            mlib_s32 slb,
 *                                            mlib_s16 *dst,
 *                                            mlib_s32 dlb,
 *                                            mlib_s32 xsize,
 *                                            mlib_s32 ysize,
 *                                            mlib_d64 ad,
 *                                            mlib_d64 bdh,
 *                                            mlib_d64 bdl);
 *
 *     void mlib_v_ImageScale_U8_S16_124(mlib_u8 *src,
 *                                       mlib_s32 slb,
 *                                       mlib_s16 *dst,
 *                                       mlib_s32 dlb,
 *                                       mlib_s32 xsize,
 *                                       mlib_s32 ysize,
 *                                       mlib_d64 ad,
 *                                       mlib_d64 bdh,
 *                                       mlib_d64 bdl);
 *
 *     void mlib_v_ImageScale_U8_S16_3(mlib_u8 *src,
 *                                     mlib_s32 slb,
 *                                     mlib_s16 *dst,
 *                                     mlib_s32 dlb,
 *                                     mlib_s32 xsize,
 *                                     mlib_s32 ysize,
 *                                     mlib_d64 ad,
 *                                     mlib_d64 bdh,
 *                                     mlib_d64 bdl);
 *
 *     void mlib_v_ImageScale_S16_S32_124(mlib_u8 *src,
 *                                        mlib_s32 slb,
 *                                        mlib_u8 *dst,
 *                                        mlib_s32 dlb,
 *                                        mlib_s32 xsize,
 *                                        mlib_s32 ysize,
 *                                        mlib_f32 *afh,
 *                                        mlib_f32 *afl,
 *                                        mlib_d64 *bdh,
 *                                        mlib_d64 *bdl,
 *                                        mlib_s32 shift);
 *
 *     void mlib_v_ImageScale_S16_S32_3(mlib_u8 *src,
 *                                      mlib_s32 slb,
 *                                      mlib_u8 *dst,
 *                                      mlib_s32 dlb,
 *                                      mlib_s32 xsize,
 *                                      mlib_s32 ysize,
 *                                      mlib_f32 *afh,
 *                                      mlib_d64 *bdh,
 *                                      mlib_s32 shift);
 *
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageScale.h>

/* *********************************************************** */

#define	MLIB_V_IMAGESCALE_U8_S16(sf, afh, afl, bdh, bdl, dd)    \
	sd0 = vis_fpmerge(sf, sf);                              \
	rdh = vis_fmuld8ulx16(vis_read_hi(sd0), afh);           \
	rdh = vis_fpadd32(rdh, bdh);                            \
	rdl = vis_fmuld8ulx16(vis_read_lo(sd0), afl);           \
	rdl = vis_fpadd32(rdl, bdl);                            \
	dd = vis_fpackfix_pair(rdh, rdl)

/* *********************************************************** */

#define	DEF_NAME	MLIB_V_IMAGESCALE_U8_S16

void
mlib_v_ImageScale_U8_S16_124_A8D1X8(
    mlib_u8 *src,
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
	mlib_d64 sd0;

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
		DEF_NAME(vis_read_hi(sd), vis_read_hi(ad),
		    vis_read_lo(ad), bdh, bdl, dd);
		(*dp++) = dd;

		DEF_NAME(vis_read_lo(sd), vis_read_hi(ad),
		    vis_read_lo(ad), bdh, bdl, dd);
		(*dp++) = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_U8_S16_124_A8D2(
    mlib_u8 *src,
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
	mlib_d64 sd0;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* pointer to end of a dst line */
	mlib_s16 *dend;

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
			DEF_NAME(vis_read_hi(sd),
			    vis_read_hi(ad), vis_read_lo(ad), bdh, bdl, dd);
			(*dp++) = dd;
			DEF_NAME(vis_read_lo(sd),
			    vis_read_hi(ad), vis_read_lo(ad), bdh, bdl, dd);
			(*dp++) = dd;
		}

/* last 8-pixel column */

		if (i < xsize) {
			sd = *sp;
			DEF_NAME(vis_read_hi(sd),
			    vis_read_hi(ad), vis_read_lo(ad), bdh, bdl, dd);
			dend = (mlib_s16 *)dl + xsize - 1;
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				DEF_NAME(vis_read_lo(sd),
				    vis_read_hi(ad), vis_read_lo(ad), bdh, bdl,
				    dd);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
			}
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_U8_S16_124(
    mlib_u8 *src,
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
	mlib_u8 *sl;

/* pointer to begin of a dst line */
	mlib_s16 *dl;

/* pointer to end of a dst line */
	mlib_s16 *dend;

/* source data */
	mlib_d64 sd, s0, s1;

/* destination data */
	mlib_d64 dd;

/* alpha and beta data */
	mlib_d64 ad0, b[8];

/* temporaries used in macro */
	mlib_d64 sd0;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

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
		off = (mlib_u8 *)dp - (mlib_u8 *)dl;
		dend = dl + xsize - 1;

/* prepare the alpha value */
		vis_alignaddr(dp, off);
		ad0 = vis_faligndata(ad, ad);

/* prepare the beta value */
		off >>= 1;
		bdh = b[-off];
		bdl = b[4 - off];

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* first 8-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s0 = vis_ld_d64_nf(sp);
			s1 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s0, s1);
			sp++;
			DEF_NAME(vis_read_hi(sd),
			    vis_read_hi(ad0), vis_read_lo(ad0), bdh, bdl, dd);
			emask = vis_edge16(dl, dend);
			vis_pst_16(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				DEF_NAME(vis_read_lo(sd),
				    vis_read_hi(ad0), vis_read_lo(ad0), bdh,
				    bdl, dd);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
				dp++;
			}
		}

/* 8-pixel column loop */
		n = ((mlib_u8 *)dend - (mlib_u8 *)dp + 2) >> 4;
		s1 = vis_ld_d64_nf(sp);
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s0 = s1;
			s1 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s0, s1);
			sp++;
			DEF_NAME(vis_read_hi(sd),
			    vis_read_hi(ad0), vis_read_lo(ad0), bdh, bdl, dd);
			(*dp++) = dd;

			DEF_NAME(vis_read_lo(sd),
			    vis_read_hi(ad0), vis_read_lo(ad0), bdh, bdl, dd);
			(*dp++) = dd;
		}

/* last 8-pixel column */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			s0 = vis_ld_d64_nf(sp);
			s1 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s0, s1);
			DEF_NAME(vis_read_hi(sd),
			    vis_read_hi(ad0), vis_read_lo(ad0), bdh, bdl, dd);
			emask = vis_edge16(dp, dend);
			vis_pst_16(dd, dp, emask);
			dp++;

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				DEF_NAME(vis_read_lo(sd),
				    vis_read_hi(ad0), vis_read_lo(ad0), bdh,
				    bdl, dd);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
			}
		}

		sl += slb;
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_U8_S16_3(
    mlib_u8 *src,
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
	mlib_u8 *sl;

/* pointer to dst line */
	mlib_s16 *dl;

/* pointer to end of dst */
	mlib_s16 *dend;

/* source data */
	mlib_d64 sd, s0, s1;

/* destination data */
	mlib_d64 dd;

/* alpha and beta data */
	mlib_d64 adm, bdm, a[5], b[6], dtmp;

/* temporaries used in macro */
	mlib_d64 sd0;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* edge mask */
	mlib_s32 emask;

/* offset of address in dst */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;
	mlib_f32 adf0, adf1, adf2, ftmp;

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

	if ((xsize == slb) && (dlb == 2 * slb)) {
		xsize *= ysize;
		ysize = 1;
	}

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = ((mlib_u8 *)dp - (mlib_u8 *)dl) >> 1;
		dend = dl + xsize - 1;

/* prepare the alpha and beta */
		bdh = b[-off];
		bdl = b[1 - off];
		bdm = b[2 - off];
		ad = a[3 + off];
		adm = a[4 + off];

		adf0 = vis_read_hi(ad);
		adf1 = vis_read_lo(ad);
		adf2 = vis_read_hi(adm);

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);
		off = (mlib_u8 *)sp - ((mlib_u8 *)sl + off);
		emask = vis_edge16(dl, dend);

		if (off == 0) {
			if (emask != 0xf) {
				sd = (*sp++);
				DEF_NAME(vis_read_hi(sd), adf0,
				    adf1, bdh, bdl, dd);
				vis_pst_16(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge16(dp, dend);
					DEF_NAME(vis_read_lo
					    (sd), adf2, adf0, bdm, bdh, dd);
					vis_pst_16(dd, dp, emask);
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
			}

/* 8-pixel column loop */
			n = (((mlib_u8 *)dend - (mlib_u8 *)dp + 2) / 48) * 3;

#pragma pipeloop(0)
			for (i = 0; i < n; i += 3) {
				sd = sp[i];
				DEF_NAME(vis_read_hi(sd), adf0,
				    adf1, bdh, bdl, dd);
				dp[2 * i] = dd;

				DEF_NAME(vis_read_lo(sd), adf2,
				    adf0, bdm, bdh, dd);
				dp[2 * i + 1] = dd;

				sd = sp[i + 1];
				DEF_NAME(vis_read_hi(sd), adf1,
				    adf2, bdl, bdm, dd);
				dp[2 * i + 2] = dd;

				DEF_NAME(vis_read_lo(sd), adf0,
				    adf1, bdh, bdl, dd);
				dp[2 * i + 3] = dd;

				sd = sp[i + 2];
				DEF_NAME(vis_read_hi(sd), adf2,
				    adf0, bdm, bdh, dd);
				dp[2 * i + 4] = dd;

				DEF_NAME(vis_read_lo(sd), adf1,
				    adf2, bdl, bdm, dd);
				dp[2 * i + 5] = dd;
			}

			sp += i;
			dp += 2 * i;

/* last 8-pixel column */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				sd = sp[0];
				DEF_NAME(vis_read_hi(sd), adf0,
				    adf1, bdh, bdl, dd);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					DEF_NAME(vis_read_lo
					    (sd), adf2, adf0, bdm, bdh, dd);
					emask = vis_edge16(dp, dend);
					vis_pst_16(dd, dp, emask);
					dp++;

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						sd = sp[1];
						DEF_NAME
						    (vis_read_hi(sd), adf1,
						    adf2, bdl, bdm, dd);
						emask = vis_edge16(dp, dend);
						vis_pst_16(dd, dp, emask);
						dp++;

						if ((mlib_addr)dp <=
						    (mlib_addr)dend) {
						    DEF_NAME
							(vis_read_lo(sd),
							    adf0, adf1, bdh,
							    bdl, dd);
						    emask =
							vis_edge16(dp,
							    dend);
						    vis_pst_16(dd, dp,
							    emask);
						    dp++;

						    if ((mlib_addr)dp <=
							(mlib_addr)dend) {
							sd = sp[2];
							DEF_NAME
							    (vis_read_hi
								(sd), adf2,
								adf0, bdm,
								bdh, dd);
							emask =
							    vis_edge16
								(dp, dend);
							vis_pst_16(dd,
								dp, emask);
							dp++;

							if ((mlib_addr)dp <=
							    (mlib_addr)
							    dend) {
							    DEF_NAME
								(vis_read_lo
									(sd),
									adf1,
									adf2,
									bdl,
									bdm,
									dd);
							    emask =
								vis_edge16
									(dp,
									dend);
							    vis_pst_16
								(dd,
								dp,
								emask);
							}
						    }
						}
					}
				}
			}

		} else {

			s0 = vis_ld_d64_nf(sp); sp++;

			if (emask != 0xf) {
				s1 = vis_ld_d64_nf(sp); sp++;
				sd = vis_faligndata(s0, s1);
				DEF_NAME(vis_read_hi(sd), adf0,
				    adf1, bdh, bdl, dd);
				vis_pst_16(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge16(dp, dend);
					DEF_NAME(vis_read_lo
					    (sd), adf2, adf0, bdm, bdh, dd);
					vis_pst_16(dd, dp, emask);
					dp++;
					ftmp = adf0;
					adf0 = adf1;
					adf1 = adf2;
					adf2 = ftmp;
					dtmp = bdh;
					bdh = bdl;
					bdl = bdm;
					bdm = dtmp;
					s0 = s1;
				}
			}

/* 8-pixel column loop */
			n = (((mlib_u8 *)dend - (mlib_u8 *)dp + 2) / 48) * 3;

#pragma pipeloop(0)
			for (i = 0; i < n; i += 3) {
				s0 = sp[i - 1];
				s1 = sp[i];
				sd = vis_faligndata(s0, s1);
				DEF_NAME(vis_read_hi(sd), adf0,
				    adf1, bdh, bdl, dd);
				dp[2 * i] = dd;

				DEF_NAME(vis_read_lo(sd), adf2,
				    adf0, bdm, bdh, dd);
				dp[2 * i + 1] = dd;

				s0 = sp[i + 1];
				sd = vis_faligndata(s1, s0);
				DEF_NAME(vis_read_hi(sd), adf1,
				    adf2, bdl, bdm, dd);
				dp[2 * i + 2] = dd;

				DEF_NAME(vis_read_lo(sd), adf0,
				    adf1, bdh, bdl, dd);
				dp[2 * i + 3] = dd;

				s1 = sp[i + 2];
				sd = vis_faligndata(s0, s1);
				DEF_NAME(vis_read_hi(sd), adf2,
				    adf0, bdm, bdh, dd);
				dp[2 * i + 4] = dd;

				DEF_NAME(vis_read_lo(sd), adf1,
				    adf2, bdl, bdm, dd);
				dp[2 * i + 5] = dd;
			}

			sp += i;
			dp += 2 * i;

/* last 8-pixel column */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				s0 = sp[-1];
				s1 = vis_ld_d64_nf(sp);
				sd = vis_faligndata(s0, s1);
				DEF_NAME(vis_read_hi(sd), adf0,
				    adf1, bdh, bdl, dd);
				emask = vis_edge16(dp, dend);
				vis_pst_16(dd, dp, emask);
				dp++;

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					DEF_NAME(vis_read_lo
					    (sd), adf2, adf0, bdm, bdh, dd);
					emask = vis_edge16(dp, dend);
					vis_pst_16(dd, dp, emask);
					dp++;

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						s0 = vis_ld_d64_nf(sp + 1);
						sd = vis_faligndata(s1, s0);
						DEF_NAME
						    (vis_read_hi(sd), adf1,
						    adf2, bdl, bdm, dd);
						emask = vis_edge16(dp, dend);
						vis_pst_16(dd, dp, emask);
						dp++;

						if ((mlib_addr)dp <=
						    (mlib_addr)dend) {
						    DEF_NAME
							(vis_read_lo(sd),
							    adf0, adf1, bdh,
							    bdl, dd);
						    emask =
							vis_edge16(dp,
							    dend);
						    vis_pst_16(dd, dp,
							emask);
						    dp++;

						    if ((mlib_addr)dp <=
							(mlib_addr)dend) {
							s1 = vis_ld_d64_nf
								(sp + 2);
							sd = vis_faligndata
							    (s0, s1);
							DEF_NAME
								(vis_read_hi
								    (sd), adf2,
								    adf0, bdm,
								    bdh, dd);
							emask =
								vis_edge16
								    (dp, dend);
							vis_pst_16(dd,
								dp, emask);
							dp++;

							if ((mlib_addr)
							    dp <=
							    (mlib_addr)
							    dend) {
							    DEF_NAME
								(vis_read_lo
									(sd),
									adf1,
									adf2,
									bdl,
									bdm,
									dd);
							    emask =
								vis_edge16
									(dp,
									dend);
							    vis_pst_16
									(dd,
									dp,
									emask);
							}
						    }
						}
					}
				}
			}
		}

		sl += slb;
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

#define	MLIB_V_IMAGESCALE_S16_S32(sf, af, bd, dd)               \
	rdh = vis_fmuld8sux16(sf, af);                          \
	rdl = vis_fmuld8ulx16(sf, af);                          \
	rd = vis_fpadd32(rdh, rdl);                             \
	dd = vis_fpadd32(rd, bd)

/* *********************************************************** */

#define	MLIB_V_IMAGESCALE_S16_S32_i(sd, ah, al, bh, bl)            \
	MLIB_V_IMAGESCALE_S16_S32(vis_read_hi(sd), ah, bh, dd0);   \
	MLIB_V_IMAGESCALE_S16_S32(vis_read_lo(sd), al, bl, dd1);   \
	i0 = *(mlib_s32 *)&dd0;                                    \
	i1 = *((mlib_s32 *)&dd0 + 1);                              \
	i2 = *(mlib_s32 *)&dd1;                                    \
	i3 = *((mlib_s32 *)&dd1 + 1);                              \
	i0 >>= shift;                                              \
	i1 >>= shift;                                              \
	i2 >>= shift;                                              \
	i3 >>= shift

/* *********************************************************** */

#define	MLIB_V_IMAGESCALE_S16_S32_p(sd, ah, al, bh, bl, pd)        \
	MLIB_V_IMAGESCALE_S16_S32(vis_read_hi(sd), ah, bh, dd0);   \
	MLIB_V_IMAGESCALE_S16_S32(vis_read_lo(sd), al, bl, dd1);   \
	li0 = ((*(mlib_s64 *)&dd0) >> shift);                      \
	li1 = (((*(mlib_s64 *)&dd0) << 32) >> (shift + 32));       \
	li2 = ((*(mlib_s64 *)&dd1) >> shift);                      \
	li3 = (((*(mlib_s64 *)&dd1) << 32) >> (shift + 32));       \
	*(pd) = (li0 & mask0) | (li1 & mask1);                     \
	*(pd + 1) = (li2 & mask0) | (li3 & mask1)

/* *********************************************************** */

void
mlib_v_ImageScale_S16_S32_124(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_f32 *afh,
    mlib_f32 *afl,
    mlib_d64 *bdh,
    mlib_d64 *bdl,
    mlib_s32 shift)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_s64 *dp;

/* source data */
	mlib_d64 s0, s1, sd;
	mlib_s32 *dpp, *dl, *dend;

/* loop variables */
	mlib_s32 j;

/* destination data */
	mlib_d64 dd0, dd1;

/* temporaries used in macro */
	mlib_d64 rdh, rdl, rd;

/* temporaries used in macro */
	mlib_s64 li0, li1, li2, li3;

/* temporaries used in macro */
	mlib_s32 i0, i1, i2, i3;
	mlib_s16 *sl;
	mlib_d64 bh, bl;
	mlib_f32 ah, al;
	mlib_s32 off, i, n;
	mlib_u64 mask0 = MLIB_U64_CONST(0xFFFFFFFF00000000);
	mlib_u64 mask1 = MLIB_U64_CONST(0x00000000FFFFFFFF);

	sl = (mlib_s16 *)src;
	dl = (mlib_s32 *)dst;

	if ((slb == (dlb >> 1)) && (slb == 2 * xsize)) {
		xsize *= ysize;
		ysize = 1;
	}

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the source and destination address */
		dend = dl + xsize - 1;
		dp = (mlib_s64 *)((mlib_addr)dl & (~7));
		off = (((mlib_u8 *)dl - (mlib_u8 *)dp)) >> 2;

		if (off != 0) {
			dl[0] =
			    ((mlib_s32)sl[0] * (*(mlib_s16 *)afh) +
			    (*(mlib_s32 *)bdh)) >> shift;
			dp++;
		}

		sp = (mlib_d64 *)vis_alignaddr(sl, 2 * off);
		off = (4 - off) & 3;

/* prepare the alpha and beta values */
		ah = afh[off];
		al = afl[off];
		bh = bdh[off];
		bl = bdl[off];

		off = ((mlib_u8 *)sp - ((mlib_u8 *)sl + 2 * off));
		n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) >> 4;

		if (off == 0) {
/* 4-pixel column loop */
#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				sd = (*sp++);
				MLIB_V_IMAGESCALE_S16_S32_p(sd, ah, al, bh, bl,
				    dp + 2 * i);
			}

			dp += 2 * i;
			dpp = (mlib_s32 *)dp;

/* last 4-pixel column */

			if ((mlib_addr)dpp <= (mlib_addr)dend) {
				sd = (*sp++);
				MLIB_V_IMAGESCALE_S16_S32_i(sd, ah, al, bh, bl);
				(*dpp++) = i0;

				if ((mlib_addr)dpp <= (mlib_addr)dend)
					(*dpp++) = i1;

				if ((mlib_addr)dpp <= (mlib_addr)dend)
					*dpp = i2;
			}

		} else {
/* 4-pixel column loop */
			s0 = (*sp++);
#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				s1 = (*sp++);
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_S16_S32_p(sd, ah, al, bh, bl,
				    dp + 2 * i);
				s0 = s1;
			}

			dp += 2 * i;
			dpp = (mlib_s32 *)dp;

/* last 4-pixel column */

			if ((mlib_addr)dpp <= (mlib_addr)dend) {
				s1 = vis_ld_d64_nf(sp); sp++;
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_S16_S32_i(sd, ah, al, bh, bl);
				(*dpp++) = i0;

				if ((mlib_addr)dpp <= (mlib_addr)dend)
					(*dpp++) = i1;

				if ((mlib_addr)dpp <= (mlib_addr)dend)
					*dpp = i2;
			}
		}

		sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_S16_S32_3(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_f32 *afh,
    mlib_d64 *bdh,
    mlib_s32 shift)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_s64 *dp;

/* source data */
	mlib_d64 s0, s1, sd;
	mlib_s32 *dpp, *dl, *dend;

/* loop variables */
	mlib_s32 j;

/* destination data */
	mlib_d64 dd0, dd1;

/* temporaries used in macro */
	mlib_d64 rdh, rdl, rd;

/* temporaries used in macro */
	mlib_s64 li0, li1, li2, li3;

/* temporaries used in macro */
	mlib_s32 i0, i1, i2, i3;
	mlib_s16 *sl;
	mlib_d64 bh0, bh1, bh2;
	mlib_f32 ah0, ah1, ah2;
	mlib_s32 off, i, n;
	mlib_u64 mask0 = MLIB_U64_CONST(0xFFFFFFFF00000000);
	mlib_u64 mask1 = MLIB_U64_CONST(0x00000000FFFFFFFF);

	sl = (mlib_s16 *)src;
	dl = (mlib_s32 *)dst;

	if ((slb == (dlb >> 1)) && (slb == 2 * xsize)) {
		xsize *= ysize;
		ysize = 1;
	}

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the source and destination address */
		dend = dl + xsize - 1;
		dp = (mlib_s64 *)((mlib_addr)dl & (~7));
		off = (((mlib_u8 *)dl - (mlib_u8 *)dp)) >> 1;

		if (off != 0) {
			dl[0] =
			    ((mlib_s32)sl[0] * (*(mlib_s16 *)afh) +
			    (*(mlib_s32 *)bdh)) >> shift;
			dp++;
		}

/* prepare the alpha and beta values */

		if (off == 0) {
			ah0 = afh[0];
			ah1 = afh[2];
			ah2 = afh[1];
			bh0 = bdh[0];
			bh1 = bdh[2];
			bh2 = bdh[1];
		} else {
			ah0 = afh[1];
			ah1 = afh[0];
			ah2 = afh[2];
			bh0 = bdh[1];
			bh1 = bdh[0];
			bh2 = bdh[2];
		}

		sp = (mlib_d64 *)vis_alignaddr(sl, off);
		off = ((mlib_u8 *)sp - ((mlib_u8 *)sl + off));

		n = (((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) / 48) * 3;

		if (off == 0) {

#pragma pipeloop(0)
			for (i = 0; i < n; i += 3) {
				sd = sp[i];
				MLIB_V_IMAGESCALE_S16_S32_p(sd, ah0, ah1, bh0,
				    bh1, dp + 2 * i);

				sd = sp[i + 1];
				MLIB_V_IMAGESCALE_S16_S32_p(sd, ah2, ah0, bh2,
				    bh0, dp + 2 * i + 2);

				sd = sp[i + 2];
				MLIB_V_IMAGESCALE_S16_S32_p(sd, ah1, ah2, bh1,
				    bh2, dp + 2 * i + 4);
			}

			sp += i;
			dp += 2 * i;
			dpp = (mlib_s32 *)dp;

			if ((mlib_addr)dpp <= (mlib_addr)dend) {
				sd = (*sp++);
				MLIB_V_IMAGESCALE_S16_S32_i(sd, ah0, ah1, bh0,
				    bh1);
				(*dpp++) = i0;

				if ((mlib_addr)dpp <= (mlib_addr)dend)
					(*dpp++) = i1;

				if ((mlib_addr)dpp <= (mlib_addr)dend)
					(*dpp++) = i2;

				if ((mlib_addr)dpp <= (mlib_addr)dend)
					(*dpp++) = i3;

				if ((mlib_addr)dpp <= (mlib_addr)dend) {
					sd = (*sp++);
					MLIB_V_IMAGESCALE_S16_S32_i(sd, ah2,
					    ah0, bh2, bh0);
					(*dpp++) = i0;

					if ((mlib_addr)dpp <= (mlib_addr)dend)
						(*dpp++) = i1;

					if ((mlib_addr)dpp <= (mlib_addr)dend)
						(*dpp++) = i2;

					if ((mlib_addr)dpp <= (mlib_addr)dend)
						(*dpp++) = i3;

					if ((mlib_addr)dpp <= (mlib_addr)dend) {
						sd = (*sp++);
						MLIB_V_IMAGESCALE_S16_S32_i(sd,
						    ah1, ah2, bh1, bh2);
						(*dpp++) = i0;

						if ((mlib_addr)dpp <=
						    (mlib_addr)dend)
							(*dpp++) = i1;

						if ((mlib_addr)dpp <=
						    (mlib_addr)dend)
							*dpp = i2;
					}
				}
			}

		} else {

#pragma pipeloop(0)
			for (i = 0; i < n; i += 3) {
				s0 = sp[i];
				s1 = sp[i + 1];
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_S16_S32_p(sd, ah0, ah1, bh0,
				    bh1, dp + 2 * i);

				s0 = sp[i + 2];
				sd = vis_faligndata(s1, s0);
				MLIB_V_IMAGESCALE_S16_S32_p(sd, ah2, ah0, bh2,
				    bh0, dp + 2 * i + 2);

				s1 = sp[i + 3];
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_S16_S32_p(sd, ah1, ah2, bh1,
				    bh2, dp + 2 * i + 4);
			}

			sp += i;
			dp += 2 * i;
			dpp = (mlib_s32 *)dp;

			if ((mlib_addr)dpp <= (mlib_addr)dend) {
				s0 = (*sp++);
				s1 = vis_ld_d64_nf(sp); sp++;
				sd = vis_faligndata(s0, s1);
				MLIB_V_IMAGESCALE_S16_S32_i(sd, ah0, ah1, bh0,
				    bh1);
				(*dpp++) = i0;

				if ((mlib_addr)dpp <= (mlib_addr)dend)
					(*dpp++) = i1;

				if ((mlib_addr)dpp <= (mlib_addr)dend)
					(*dpp++) = i2;

				if ((mlib_addr)dpp <= (mlib_addr)dend)
					(*dpp++) = i3;

				if ((mlib_addr)dpp <= (mlib_addr)dend) {
					s0 = vis_ld_d64_nf(sp); sp++;
					sd = vis_faligndata(s1, s0);
					MLIB_V_IMAGESCALE_S16_S32_i(sd, ah2,
					    ah0, bh2, bh0);
					(*dpp++) = i0;

					if ((mlib_addr)dpp <= (mlib_addr)dend)
						(*dpp++) = i1;

					if ((mlib_addr)dpp <= (mlib_addr)dend)
						(*dpp++) = i2;

					if ((mlib_addr)dpp <= (mlib_addr)dend)
						(*dpp++) = i3;

					if ((mlib_addr)dpp <= (mlib_addr)dend) {
						s1 = vis_ld_d64_nf(sp); sp++;
						sd = vis_faligndata(s0, s1);
						MLIB_V_IMAGESCALE_S16_S32_i(sd,
						    ah1, ah2, bh1, bh2);
						(*dpp++) = i0;

						if ((mlib_addr)dpp <=
						    (mlib_addr)dend)
							(*dpp++) = i1;

						if ((mlib_addr)dpp <=
						    (mlib_addr)dend)
							*dpp = i2;
					}
				}
			}
		}

		sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

#define	MLIB_V_IMAGESCALE_U8_S32(sf, afh, afl, bdh, bdl, dd0,    \
	dd1)                                                     \
	sd0 = vis_fpmerge(sf, sf);                               \
	rdh = vis_fmuld8ulx16(vis_read_hi(sd0), afh);            \
	dd0 = vis_fpadd32(rdh, bdh);                             \
	rdl = vis_fmuld8ulx16(vis_read_lo(sd0), afl);            \
	dd1 = vis_fpadd32(rdl, bdl)

/* *********************************************************** */

#define	MLIB_V_IMAGESCALE_U8_S32_i(sf, ah, al, bh, bl)            \
	MLIB_V_IMAGESCALE_U8_S32(sf, ah, al, bh, bl, dd0, dd1);   \
	i0 = *(mlib_s32 *)&dd0;                                   \
	i1 = *((mlib_s32 *)&dd0 + 1);                             \
	i2 = *(mlib_s32 *)&dd1;                                   \
	i3 = *((mlib_s32 *)&dd1 + 1);                             \
	i0 >>= shift;                                             \
	i1 >>= shift;                                             \
	i2 >>= shift;                                             \
	i3 >>= shift

/* *********************************************************** */

#define	MLIB_V_IMAGESCALE_U8_S32_p(sd, ah, al, bh, bl, pd)      \
	MLIB_V_IMAGESCALE_U8_S32_i(sd, ah, al, bh, bl);         \
	*(pd) = i0;                                             \
	*(pd + 1) = i1;                                         \
	*(pd + 2) = i2;                                         \
	*(pd + 3) = i3

/*
 * WARNING! No VIS version for U8->S32.                           *
 * See function mlib_ImageScale_CheckForVIS (line 61) for change. *
 * ****************************************************************
 *
 * void mlib_v_ImageScale_U8_S32_124_A8D1(mlib_u8  *src,
 *                                        mlib_u8  *dst,
 *                                        mlib_s32 dsize,
 *                                        mlib_d64 ad,
 *                                        mlib_d64 bdh,
 *                                        mlib_d64 bdl,
 *                                        mlib_s32 shift)
 * {
 *   mlib_f32 *sp;
 *   mlib_f32  sd;
 *   mlib_s32 *dp, *enddp;
 *   mlib_d64  dd, dd0, dd1;
 *   mlib_d64  rdh, rdl, rd;
 *   mlib_s32  i0, i1, i2, i3;
 *   mlib_d64  sd0;
 *   mlib_s32  j;
 *   mlib_s64  l0, l1;
 *
 *   sp = (mlib_f32 *) src;
 *   dp = (mlib_s32 *) dst;
 *   enddp = dp + (dsize-4);
 *
 * #pragma pipeloop(0)
 *   for (; dp <= enddp; dp += 4) {
 *     sd = (*sp++);
 *     MLIB_V_IMAGESCALE_U8_S32_p
 *         (sd, vis_read_hi(ad), vis_read_lo(ad), bdh, bdl, dp);
 *   }
 * }
 *
 * void mlib_v_ImageScale_U8_S32_3_A8D1(mlib_u8  *src,
 *                                      mlib_u8  *dst,
 *                                      mlib_s32 dsize,
 *                                      mlib_f32 *afh,
 *                                      mlib_f32 *afl,
 *                                      mlib_d64 *bdh,
 *                                      mlib_d64 *bdl,
 *                                      mlib_s32 shift)
 * {
 *   mlib_f32 *sp;
 *   mlib_s32 *dp, *dend;
 *   mlib_f32  sd;
 *   mlib_s32  j;
 *   mlib_d64  dd, dd0, dd1;
 *   mlib_d64  rdh, rdl, rd;
 *   mlib_s32  i0, i1, i2, i3;
 *   mlib_d64  sd0;
 *
 *   mlib_f32  ah, al;
 *   mlib_d64  bh, bl;
 *   mlib_s32  emask, n;
 *
 *   sp = (mlib_f32 *) src;
 *   dp = (mlib_s32 *) dst;
 *   dend  = (mlib_s32 *) dp + dsize - 1;
 *
 *   n = dsize/12;
 *
 *   ah = afh[0]; al = afl[0];
 *   bh = bdh[0]; bl = bdl[0];
 * #pragma pipeloop(0)
 *   for (j = 0; j < n; j ++) {
 *     sd = sp[3*j];
 *     MLIB_V_IMAGESCALE_U8_S32_p(sd, ah, al, bh, bl, dp+12*j);
 *   }
 *
 *   ah = afh[1]; al = afl[1];
 *   bh = bdh[1]; bl = bdl[1];
 * #pragma pipeloop(0)
 *   for (j = 0; j < n; j ++) {
 *     sd = sp[3*j+1];
 *     MLIB_V_IMAGESCALE_U8_S32_p(sd, ah, al, bh, bl, dp+12*j+4);
 *   }
 *
 *   ah = afh[2]; al = afl[2];
 *   bh = bdh[2]; bl = bdl[2];
 * #pragma pipeloop(0)
 *   for (j = 0; j < n; j ++) {
 *     sd = sp[3*j+2];
 *     MLIB_V_IMAGESCALE_U8_S32_p(sd, ah, al, bh, bl, dp+12*j+8);
 *   }
 *
 *   sp += 3*n;
 *   dp += 12*n;
 *
 *   if ((mlib_addr) dp <= (mlib_addr) dend) {
 *     sd = (*sp++);
 *     MLIB_V_IMAGESCALE_U8_S32_i(sd, afh[0], afl[0], bdh[0], bdl[0]);
 *
 *     if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i0;
 *     if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i1;
 *     if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i2;
 *     if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i3;
 *     if ((mlib_addr) dp <= (mlib_addr) dend) {
 *       sd = (*sp++);
 *       MLIB_V_IMAGESCALE_U8_S32_i(sd, afh[1], afl[1], bdh[1], bdl[1]);
 *
 *       if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i0;
 *       if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i1;
 *       if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i2;
 *       if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i3;
 *
 *       if ((mlib_addr) dp <= (mlib_addr) dend) {
 *         sd = (*sp++);
 *         MLIB_V_IMAGESCALE_U8_S32_i(sd, afh[2], afl[2], bdh[2], bdl[2]);
 *
 *         if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i0;
 *         if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i1;
 *         if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i2;
 *         if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i3;
 *       }
 *     }
 *   }
 * }
 *
 * void mlib_v_ImageScale_U8_S32_124(mlib_u8  *src,
 *                                   mlib_s32 slb,
 *                                   mlib_u8  *dst,
 *                                   mlib_s32 dlb,
 *                                   mlib_s32 xsize,
 *                                   mlib_s32 ysize,
 *                                   mlib_f32 *afh,
 *                                   mlib_f32 *afl,
 *                                   mlib_d64 *bdh,
 *                                   mlib_d64 *bdl,
 *                                   mlib_s32 shift)
 * {
 *   mlib_f32 *sp;
 *   mlib_f32  sd;
 *   mlib_s32 *dp, *dl, *dend;
 *   mlib_s32  j;
 *   mlib_d64  dd, dd0, dd1;
 *   mlib_d64  rdh, rdl, rd;
 *   mlib_s32  i0, i1, i2, i3;
 *   mlib_d64  sd0;
 *
 *   mlib_u8  *sl;
 *   mlib_d64  bh, bl;
 *   mlib_f32  ah, al;
 *   mlib_u8   si[4];
 *   mlib_s32  di[4];
 *   mlib_s32  off, i, n, emask;
 *
 *   sl = (mlib_u8*)src;
 *   dl = (mlib_s32*)dst;
 *
 *   for (j = 0; j < ysize; j ++) {
 *
 *     dp = dl;
 *     dend = dl + xsize - 1;
 *     off = (mlib_s32)sl & 3;
 *     sp = (mlib_f32 *) (sl - off);
 *
 *     if (off) {
 *       for (i = 0; i < 4-off; i++) si[i] = sl[i];
 *       MLIB_V_IMAGESCALE_U8_S32_p
 *           (*(mlib_f32*)&si, afh[0], afl[0], bdh[0], bdl[0], di);
 *       for (i = 0; (i < 4-off) && (dp <= dend); i++) (*dp++) = di[i];
 *       sp++;
 *     }
 *
 *     ah = afh[off];
 *     al = afl[off];
 *     bh = bdh[off];
 *     bl = bdl[off];
 *
 *     n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) >> 4;
 * #pragma pipeloop(0)
 *     for (i = 0; i < n; i ++, dp += 4) {
 *       sd = (*sp++);
 *       MLIB_V_IMAGESCALE_U8_S32_p(sd, ah, al, bh, bl, dp);
 *     }
 *
 *     if ((mlib_addr) dp <= (mlib_addr) dend) {
 *       sd = (*sp++);
 *       MLIB_V_IMAGESCALE_U8_S32_i(sd, ah, al, bh, bl);
 *
 *       if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i0;
 *       if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i1;
 *       if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i2;
 *       if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i3;
 *     }
 *
 *     sl = (mlib_u8 *) ((mlib_u8 *) sl + slb);
 *     dl = (mlib_s32 *) ((mlib_u8 *) dl + dlb);
 *   }
 * }
 *
 * void mlib_v_ImageScale_U8_S32_3(mlib_u8  *src,
 *                                 mlib_s32 slb,
 *                                 mlib_u8  *dst,
 *                                 mlib_s32 dlb,
 *                                 mlib_s32 xsize,
 *                                 mlib_s32 ysize,
 *                                 mlib_f32 *afh,
 *                                 mlib_f32 *afl,
 *                                 mlib_d64 *bdh,
 *                                 mlib_d64 *bdl,
 *                                 mlib_s32 shift)
 * {
 *   mlib_f32 *sp;
 *   mlib_f32  sd;
 *   mlib_s32 *dp, *dl, *dend;
 *   mlib_s32  j;
 *   mlib_d64  dd, dd0, dd1;
 *   mlib_d64  rdh, rdl, rd;
 *   mlib_s32  i0, i1, i2, i3;
 *   mlib_d64  sd0;
 *
 *   mlib_u8  *sl;
 *   mlib_d64  bh, bl;
 *   mlib_f32  ah, al;
 *   mlib_u8   si[4];
 *   mlib_s32  di[4];
 *   mlib_s32  off, off0, off1, off2, i, n, emask;
 *
 *   sl = (mlib_u8*)src;
 *   dl = (mlib_s32*)dst;
 *
 *   for (j = 0; j < ysize; j ++) {
 *
 *     dp = dl;
 *     dend = dl + xsize - 1;
 *     off = (mlib_s32)sl & 3;
 *     sp = (mlib_f32 *) (sl - off);
 *
 *     if (off) off0 = 3 - off;
 *         else off0 = 0;
 *
 *     if (off) {
 *       for (i = 0; i < 4-off; i++) si[i] = sl[i];
 *       MLIB_V_IMAGESCALE_U8_S32_p
 *           (*(mlib_f32*)&si, afh[0], afl[0], bdh[0], bdl[0], di);
 *       for (i = 0; (i < 4-off) && (dp <= dend); i++) (*dp++) = di[i];
 *       sp++;
 *       off0++;
 *
 *       if (off0 == 3) off0 = 0;
 *     }
 *
 *     n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) / (3*16);
 *
 *     ah = afh[off0]; al = afl[off0];
 *     bh = bdh[off0]; bl = bdl[off0];
 * #pragma pipeloop(0)
 *     for (i = 0; i < n; i ++) {
 *       sd = sp[3*i];
 *       MLIB_V_IMAGESCALE_U8_S32_p(sd, ah, al, bh, bl, dp+12*i);
 *     }
 *
 *     off1 = off0 + 1;
 *
 *     if (off1 == 3) off1 = 0;
 *     ah = afh[off1]; al = afl[off1];
 *     bh = bdh[off1]; bl = bdl[off1];
 * #pragma pipeloop(0)
 *     for (i = 0; i < n; i ++) {
 *       sd = sp[3*i+1];
 *       MLIB_V_IMAGESCALE_U8_S32_p(sd, ah, al, bh, bl, dp+12*i+4);
 *     }
 *
 *     off2 = off1 + 1;
 *
 *     if (off2 == 3) off2 = 0;
 *     ah = afh[off2]; al = afl[off2];
 *     bh = bdh[off2]; bl = bdl[off2];
 * #pragma pipeloop(0)
 *     for (i = 0; i < n; i ++) {
 *       sd = sp[3*i+2];
 *       MLIB_V_IMAGESCALE_U8_S32_p(sd, ah, al, bh, bl, dp+12*i+8);
 *     }
 *
 *     sp += 3*n;
 *     dp += 12*n;
 *
 *     if ((mlib_addr) dp <= (mlib_addr) dend) {
 *       sd = (*sp++);
 *       MLIB_V_IMAGESCALE_U8_S32_i
 *           (sd, afh[off0], afl[off0], bdh[off0], bdl[off0]);
 *
 *       if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i0;
 *       if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i1;
 *       if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i2;
 *       if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i3;
 *
 *       if ((mlib_addr) dp <= (mlib_addr) dend) {
 *         sd = (*sp++);
 *         MLIB_V_IMAGESCALE_U8_S32_i
 *             (sd, afh[off1], afl[off1], bdh[off1], bdl[off1]);
 *
 *         if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i0;
 *         if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i1;
 *         if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i2;
 *         if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i3;
 *
 *         if ((mlib_addr) dp <= (mlib_addr) dend) {
 *           sd = (*sp++);
 *           MLIB_V_IMAGESCALE_U8_S32_i
 *               (sd, afh[off2], afl[off2], bdh[off2], bdl[off2]);
 *
 *           if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i0;
 *           if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i1;
 *           if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i2;
 *           if ((mlib_addr) dp <= (mlib_addr) dend) (*dp++) = i3;
 *         }
 *       }
 *     }
 *
 *     sl = (mlib_u8 *) ((mlib_u8 *) sl + slb);
 *     dl = (mlib_s32 *) ((mlib_u8 *) dl + dlb);
 *   }
 * }
 *
 */

/* *********************************************************** */
