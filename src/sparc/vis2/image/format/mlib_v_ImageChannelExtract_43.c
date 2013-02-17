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
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_v_ImageChannelExtract_43.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_v_ImageChannelExtract_43_A8D2X8
 *      mlib_v_ImageChannelExtract_43
 *
 * DESCRIPTION
 *      extract the right or left 3 channels of a 4-channel image to
 *      a 3-channel image -- VIS version low level functions.
 *
 *      ABGR => BGR   (43), or  RGBA => RGB  (43L)
 *
 * NOTE
 *      These functions are separated from mlib_v_ImageChannelExtract.c
 *      for loop unrolling and structure clarity.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageChannelExtract_2.h>

/* *********************************************************** */

#define	EXTRACT_43                                              \
	                                                        \
/*                                                              \
 * shift right                                                  \
 */                                                             \
	vis_write_bmask(bm0, 0);                                \
	dd0 = vis_bshuffle(sd0, sd1);                           \
	vis_write_bmask(bm1, 0);                                \
	dd1 = vis_bshuffle(sd1, sd2);                           \
	vis_write_bmask(bm2, 0);                                \
	dd2 = vis_bshuffle(sd2, sd3)

/* *********************************************************** */

#define	LOAD_EXTRACT_43                                         \
	s0 = s4;                                                \
	s1 = sp[1];                                             \
	s2 = sp[2];                                             \
	s3 = sp[3];                                             \
	s4 = sp[4];                                             \
	sd0 = vis_faligndata(s0, s1);                           \
	sd1 = vis_faligndata(s1, s2);                           \
	sd2 = vis_faligndata(s2, s3);                           \
	sd3 = vis_faligndata(s3, s4);                           \
	sp += 4;                                                \
	dd2old = dd2;                                           \
	EXTRACT_43

/* *********************************************************** */

#define	LOAD_EXTRACT_43_NF                                      \
	s0 = s4;                                                \
	s1 = vis_ld_d64_nf(sp + 1);                             \
	s2 = vis_ld_d64_nf(sp + 2);                             \
	s3 = vis_ld_d64_nf(sp + 3);                             \
	s4 = vis_ld_d64_nf(sp + 4);                             \
	sd0 = vis_faligndata(s0, s1);                           \
	sd1 = vis_faligndata(s1, s2);                           \
	sd2 = vis_faligndata(s2, s3);                           \
	sd3 = vis_faligndata(s3, s4);                           \
	sp += 4;                                                \
	dd2old = dd2;                                           \
	EXTRACT_43

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_43_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *bmask)
{
/* 8-byte aligned pointer for pixel */
	mlib_d64 *sp, *dp;

/* 8-byte aligned pointer for line */
	mlib_d64 *sl, *dl;
	mlib_s32 bm0, bm1, bm2;
	mlib_s32 i, j, dsize;

	if (slb == 4 * xsize && dlb == 3 * xsize) {
		xsize *= ysize;
		ysize = 1;
	}

	dsize = xsize / 8;

	bm0 = bmask[0];
	bm1 = bmask[1];
	bm2 = bmask[2];

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

#pragma pipeloop(0)
		for (i = 0; i < dsize / 2; i++) {
			vis_write_bmask(bm0, 0);
			dp[0] = vis_bshuffle(sp[0], sp[1]);
			dp[3] = vis_bshuffle(sp[4], sp[5]);
			vis_write_bmask(bm1, 0);
			dp[1] = vis_bshuffle(sp[1], sp[2]);
			dp[4] = vis_bshuffle(sp[5], sp[6]);
			vis_write_bmask(bm2, 0);
			dp[2] = vis_bshuffle(sp[2], sp[3]);
			dp[5] = vis_bshuffle(sp[6], sp[7]);
			sp += 8;
			dp += 6;
		}

		if (dsize & 1) {
			vis_write_bmask(bm0, 0);
			dp[0] = vis_bshuffle(sp[0], sp[1]);
			vis_write_bmask(bm1, 0);
			dp[1] = vis_bshuffle(sp[1], sp[2]);
			vis_write_bmask(bm2, 0);
			dp[2] = vis_bshuffle(sp[2], sp[3]);
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_43(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *bmask)
{
	mlib_u8 *sa, *da;

/* end points in dst */
	mlib_u8 *dend, *dend2;

/* 8-byte aligned start points in dst */
	mlib_d64 *dp;

/* 8-byte aligned start point in src */
	mlib_d64 *sp;

/* 8-byte source row data */
	mlib_d64 s0, s1, s2, s3, s4;

/* 8-byte source data */
	mlib_d64 sd0, sd1, sd2, sd3;

/* dst data */
	mlib_d64 dd0, dd1, dd2;

/* the last datum of the last step */
	mlib_d64 dd2old;
	mlib_s32 bm0, bm1, bm2;

/* offset of address in src */
	mlib_s32 soff;

/* offset of address in dst */
	mlib_s32 doff;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 i, j, n;

	bm0 = bmask[0];
	bm1 = bmask[1];
	bm2 = bmask[2];

	for (j = 0; j < ysize; j++) {
		sa = (void *)src;
		da = dst;

/* prepare the source address */
		sp = (mlib_d64 *)((mlib_addr)sa & (~7));
		soff = ((mlib_addr)sa & 7);

/* prepare the destination addresses */
		dp = (mlib_d64 *)((mlib_addr)da & (~7));
		dend = da + xsize * 3 - 1;
		dend2 = dend - 23;
		doff = 8 - ((mlib_addr)da & 7);

/* generate edge mask for the start point */
		emask = vis_edge8(da, dend);

/* load 32 byte, convert, store 24 bytes */
/* initial value */
		s4 = vis_ld_d64_nf(sp);
		vis_alignaddr((void *)soff, 0);
		LOAD_EXTRACT_43_NF;

		if (xsize >= 8) {
			if (doff == 8) {
				vis_pst_8(dd0, dp++, emask);
				(*dp++) = dd1;
				(*dp++) = dd2;
			} else {
				vis_alignaddr((void *)doff, 0);
				vis_pst_8(vis_faligndata(dd0, dd0), dp++,
				    emask);
				(*dp++) = vis_faligndata(dd0, dd1);
				(*dp++) = vis_faligndata(dd1, dd2);
			}
		} else {
/* for very small size */
			if (doff == 8) {
				vis_pst_8(dd0, dp++, emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge8(dp, dend);
					vis_pst_8(dd1, dp++, emask);

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge8(dp, dend);
						vis_pst_8(dd2, dp++, emask);
					}
				}
			} else {
				vis_alignaddr((void *)doff, 0);
				vis_pst_8(vis_faligndata(dd0, dd0), dp++,
				    emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge8(dp, dend);
					vis_pst_8(vis_faligndata(dd0, dd1),
					    dp++, emask);

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge8(dp, dend);
						vis_pst_8(vis_faligndata(dd1,
						    dd2), dp++, emask);

						if ((mlib_addr)dp <=
						    (mlib_addr)dend) {
							emask =
							    vis_edge8(dp, dend);
							vis_pst_8(vis_faligndata
							    (dd2, dd2), dp++,
							    emask);
						}
					}
				}
			}
		}

/* no edge handling is needed in the loop */

		if (doff == 8) {
			if ((mlib_addr)dp <= (mlib_addr)dend2) {
				n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 24 + 1;
				vis_alignaddr((void *)soff, 0);

#pragma pipeloop(0)
				for (i = 0; i < n; i++) {
					LOAD_EXTRACT_43;
					(*dp++) = dd0;
					(*dp++) = dd1;
					(*dp++) = dd2;
				}
			}
		} else {
			if ((mlib_addr)dp <= (mlib_addr)dend2) {
				n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 24 + 1;

#pragma pipeloop(0)
				for (i = 0; i < n; i++) {
					vis_alignaddr((void *)soff, 0);
					LOAD_EXTRACT_43;
					vis_alignaddr((void *)doff, 0);
					(*dp++) = vis_faligndata(dd2old, dd0);
					(*dp++) = vis_faligndata(dd0, dd1);
					(*dp++) = vis_faligndata(dd1, dd2);
				}
			}
		}

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			vis_alignaddr((void *)soff, 0);
			LOAD_EXTRACT_43_NF;
			emask = vis_edge8(dp, dend);

			if (doff == 8) {
				vis_pst_8(dd0, dp++, emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge8(dp, dend);
					vis_pst_8(dd1, dp++, emask);

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge8(dp, dend);
						vis_pst_8(dd2, dp++, emask);
					}
				}
			} else {
				vis_alignaddr((void *)doff, 0);
				vis_pst_8(vis_faligndata(dd2old, dd0), dp++,
				    emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge8(dp, dend);
					vis_pst_8(vis_faligndata(dd0, dd1),
					    dp++, emask);

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge8(dp, dend);
						vis_pst_8(vis_faligndata(dd1,
						    dd2), dp, emask);
					}
				}
			}
		}

		src += slb;
		dst += dlb;
	}
}

/* *********************************************************** */
