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

#pragma ident	"@(#)mlib_v_ImageDivAlphaFunc.c	9.2	07/11/05 SMI"

/*
 *  Internal function for mlib_ImageDivAlpha on U8 data type.
 *
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCopy.h>
#include <mlib_v_ImageDivAlpha.h>

/* *********************************************************** */

#define	DEF_FUNC(FUNC_NAME, DTYPE)                               \
	mlib_status                                              \
	FUNC_NAME(DTYPE * sl, mlib_s32 sstride, DTYPE * dl,      \
	    mlib_s32 dstride, mlib_s32 width, mlib_s32 height,   \
	    mlib_s32 channel, mlib_s32 alpha)

/* *********************************************************** */

#define	MEM_COPY(src, dst, n)	mlib_ImageCopy_na((void*)src, (void*)dst, n)

/* *********************************************************** */

#define	GET_ALPHA(aa, pp, ind)	aa = p_tbl[((mlib_u8*)pp)[ind]]

/* *********************************************************** */

#define	GET_ALPHA_NF(aa, pp, ind)                          \
	aa = p_tbl[vis_ld_u8_nf(((mlib_u8*)pp) + ind)]

/* *********************************************************** */

#define	DIV_ALPHA(rr, ss, aa)                                   \
	rr = vis_fmul8x16(ss, aa);                              \
	rr = vis_fand(rr, mask7FFF)

/* *********************************************************** */

#define	GET_ALPHA_3CH_0_NF()                                    \
	GET_ALPHA_NF(a0, ap, 0);                                \
	GET_ALPHA_NF(a1, ap, 3);                                \
	GET_ALPHA_NF(aa, ap, 6);                                \
	GET_ALPHA_NF(a2, ap, 9);                                \
	a1 = vis_faligndata(a1, aa);                            \
	a2 = vis_faligndata(aa, a2);                            \
	GET_ALPHA_NF(b0, ap, 12);                               \
	GET_ALPHA_NF(b1, ap, 15);                               \
	GET_ALPHA_NF(bb, ap, 18);                               \
	GET_ALPHA_NF(b2, ap, 21);                               \
	b1 = vis_faligndata(b1, bb);                            \
	b2 = vis_faligndata(bb, b2)

/* *********************************************************** */

#define	GET_ALPHA_3CH_0()                                       \
	GET_ALPHA(a0, ap, 0);                                   \
	GET_ALPHA(a1, ap, 3);                                   \
	GET_ALPHA(aa, ap, 6);                                   \
	GET_ALPHA(a2, ap, 9);                                   \
	a1 = vis_faligndata(a1, aa);                            \
	a2 = vis_faligndata(aa, a2);                            \
	GET_ALPHA(b0, ap, 12);                                  \
	GET_ALPHA(b1, ap, 15);                                  \
	GET_ALPHA(bb, ap, 18);                                  \
	GET_ALPHA(b2, ap, 21);                                  \
	b1 = vis_faligndata(b1, bb);                            \
	b2 = vis_faligndata(bb, b2)

/* *********************************************************** */

#define	GET_ALPHA_3CH_1_NF()                                    \
	GET_ALPHA_NF(a0, ap, 0);                                \
	GET_ALPHA_NF(a1, ap, 3);                                \
	GET_ALPHA_NF(aa, ap, 6);                                \
	GET_ALPHA_NF(a2, ap, 9);                                \
	GET_ALPHA_NF(b0, ap, 12);                               \
	GET_ALPHA_NF(b1, ap, 15);                               \
	GET_ALPHA_NF(bb, ap, 18);                               \
	GET_ALPHA_NF(b2, ap, 21);                               \
	vis_alignaddr((void *)0, 2);                            \
	a0 = vis_faligndata(a0, a1);                            \
	b0 = vis_faligndata(b0, b1);                            \
	vis_alignaddr((void *)0, 4);                            \
	a1 = vis_faligndata(a1, aa);                            \
	b1 = vis_faligndata(b1, bb);                            \
	vis_alignaddr((void *)0, 6);                            \
	a2 = vis_faligndata(aa, a2);                            \
	b2 = vis_faligndata(bb, b2)

/* *********************************************************** */

#define	GET_ALPHA_3CH_1()                                       \
	GET_ALPHA(a0, ap, 0);                                   \
	GET_ALPHA(a1, ap, 3);                                   \
	GET_ALPHA(aa, ap, 6);                                   \
	GET_ALPHA(a2, ap, 9);                                   \
	GET_ALPHA(b0, ap, 12);                                  \
	GET_ALPHA(b1, ap, 15);                                  \
	GET_ALPHA(bb, ap, 18);                                  \
	GET_ALPHA(b2, ap, 21);                                  \
	vis_alignaddr((void *)0, 2);                            \
	a0 = vis_faligndata(a0, a1);                            \
	b0 = vis_faligndata(b0, b1);                            \
	vis_alignaddr((void *)0, 4);                            \
	a1 = vis_faligndata(a1, aa);                            \
	b1 = vis_faligndata(b1, bb);                            \
	vis_alignaddr((void *)0, 6);                            \
	a2 = vis_faligndata(aa, a2);                            \
	b2 = vis_faligndata(bb, b2)

/* *********************************************************** */

#define	GET_ALPHA_3CH_2_NF()                                    \
	GET_ALPHA_NF(a0, ap, 0);                                \
	GET_ALPHA_NF(a1, ap, 3);                                \
	GET_ALPHA_NF(aa, ap, 6);                                \
	GET_ALPHA_NF(a2, ap, 9);                                \
	a0 = vis_faligndata(a0, a1);                            \
	a1 = vis_faligndata(a1, aa);                            \
	GET_ALPHA_NF(b0, ap, 12);                               \
	GET_ALPHA_NF(b1, ap, 15);                               \
	GET_ALPHA_NF(bb, ap, 18);                               \
	GET_ALPHA_NF(b2, ap, 21);                               \
	b0 = vis_faligndata(b0, b1);                            \
	b1 = vis_faligndata(b1, bb)

/* *********************************************************** */

#define	GET_ALPHA_3CH_2()                                       \
	GET_ALPHA(a0, ap, 0);                                   \
	GET_ALPHA(a1, ap, 3);                                   \
	GET_ALPHA(aa, ap, 6);                                   \
	GET_ALPHA(a2, ap, 9);                                   \
	a0 = vis_faligndata(a0, a1);                            \
	a1 = vis_faligndata(a1, aa);                            \
	GET_ALPHA(b0, ap, 12);                                  \
	GET_ALPHA(b1, ap, 15);                                  \
	GET_ALPHA(bb, ap, 18);                                  \
	GET_ALPHA(b2, ap, 21);                                  \
	b0 = vis_faligndata(b0, b1);                            \
	b1 = vis_faligndata(b1, bb)

/* *********************************************************** */

#define	DIV_ALPHA_3CH_NF()                                      \
	s0 = sp[0];                                             \
	s1 = vis_ld_d64_nf(sp + 1);                             \
	s2 = vis_ld_d64_nf(sp + 2);                             \
	DIV_ALPHA(d0, vis_read_hi(s0), a0);                     \
	DIV_ALPHA(d1, vis_read_lo(s0), a1);                     \
	dd = vis_fpack16_pair(d0, d1);                          \
	vis_pst_8(dd, dp, ~cmask0);                             \
	vis_pst_8(s0, dp, cmask0);                              \
	DIV_ALPHA(d0, vis_read_hi(s1), a2);                     \
	DIV_ALPHA(d1, vis_read_lo(s1), b0);                     \
	dd = vis_fpack16_pair(d0, d1);                          \
	vis_pst_8(dd, dp + 1, ~cmask1);                         \
	vis_pst_8(s1, dp + 1, cmask1);                          \
	DIV_ALPHA(d0, vis_read_hi(s2), b1);                     \
	DIV_ALPHA(d1, vis_read_lo(s2), b2);                     \
	dd = vis_fpack16_pair(d0, d1);                          \
	vis_pst_8(dd, dp + 2, ~cmask2);                         \
	vis_pst_8(s2, dp + 2, cmask2);                          \
	ap += 24;                                               \
	sp += 3;                                                \
	dp += 3

/* *********************************************************** */

#define	DIV_ALPHA_3CH()                                         \
	s0 = sp[0];                                             \
	s1 = sp[1];                                             \
	s2 = sp[2];                                             \
	DIV_ALPHA(d0, vis_read_hi(s0), a0);                     \
	DIV_ALPHA(d1, vis_read_lo(s0), a1);                     \
	dd = vis_fpack16_pair(d0, d1);                          \
	vis_pst_8(dd, dp, ~cmask0);                             \
	vis_pst_8(s0, dp, cmask0);                              \
	DIV_ALPHA(d0, vis_read_hi(s1), a2);                     \
	DIV_ALPHA(d1, vis_read_lo(s1), b0);                     \
	dd = vis_fpack16_pair(d0, d1);                          \
	vis_pst_8(dd, dp + 1, ~cmask1);                         \
	vis_pst_8(s1, dp + 1, cmask1);                          \
	DIV_ALPHA(d0, vis_read_hi(s2), b1);                     \
	DIV_ALPHA(d1, vis_read_lo(s2), b2);                     \
	dd = vis_fpack16_pair(d0, d1);                          \
	vis_pst_8(dd, dp + 2, ~cmask2);                         \
	vis_pst_8(s2, dp + 2, cmask2);                          \
	ap += 24;                                               \
	sp += 3;                                                \
	dp += 3

/* *********************************************************** */

DEF_FUNC(mlib_ImageDivAlpha_U8, mlib_u8)
{
	mlib_d64 mask7FFF = vis_to_double_dup(0x7FFF7FFF);
	mlib_d64 *p_tbl;
	mlib_d64 *buffs, *buffd;
	mlib_d64 *sp, *dp;
	mlib_d64 ss, d0, d1, dd, a0, a1;
	mlib_s32 cmask = (1 << (channel - alpha - 1));
	mlib_s32 ww, dflag, i, j;

	vis_write_gsr(7 << 3);
	cmask |= (cmask << channel);
	cmask |= (cmask << 2 * channel);

	if (channel == 3) {
		p_tbl = (mlib_d64 *)mlib_DivAlpha_tbl;
	} else {
		p_tbl = (mlib_d64 *)mlib_DivAlpha_tbl4 + alpha * 256;
	}

	width *= channel;
	ww = (width + 7) / 8;

	if (channel == 3) {
		ww = 3 * ((ww + 2) / 3);
	}

	buffs = __mlib_malloc(2 * sizeof (mlib_d64) * ww);

	if (buffs == NULL) {
		return (MLIB_FAILURE);
	}

	buffd = buffs + ww;

	for (j = 0; j < height; j++) {
		mlib_u8 *ap = sl + alpha;

		if (((int)sl & 7)) {
			MEM_COPY(sl, buffs, width * sizeof (mlib_u8));
			sp = buffs;
		} else {
			sp = (mlib_d64 *)sl;
		}

		dflag = 0;

		if (((int)dl | width) & 7) {
			dp = buffd;
			dflag = 1;
		} else {
			dp = (mlib_d64 *)dl;
		}

		if (channel == 4) {
#pragma pipeloop(0)
			for (i = 0; i < ww; i++) {
				ss = *sp;
				GET_ALPHA(a0, sp, alpha);
				GET_ALPHA(a1, sp, alpha + 4);
				DIV_ALPHA(d0, vis_read_hi(ss), a0);
				DIV_ALPHA(d1, vis_read_lo(ss), a1);
				*dp = vis_fpack16_pair(d0, d1);
				sp++;
				dp++;
			}

		} else if (channel == 3) {
			mlib_d64 a0, a1, a2, aa;
			mlib_d64 b0, b1, b2, bb;
			mlib_d64 s0, s1, s2;
			mlib_d64 d0, d1;
			mlib_s32 cmask0, cmask1, cmask2;

			cmask0 = 0x492 >> alpha;
			cmask1 = 0x492 >> (alpha + 1);
			cmask2 = 0x492 >> (alpha + 2);

			vis_alignaddr((void *)0, 4);

			if (alpha == 0) {
#pragma pipeloop(0)
				for (i = 0; i < ww - 3; i += 3) {
					GET_ALPHA_3CH_0();
					DIV_ALPHA_3CH();
				}

				if (i < ww) {
					GET_ALPHA_3CH_0_NF();
					DIV_ALPHA_3CH_NF();
				}

			} else if (alpha == 1) {
#pragma pipeloop(0)
				for (i = 0; i < ww - 3; i += 3) {
					GET_ALPHA_3CH_1();
					DIV_ALPHA_3CH();
				}


				if (i < ww) {
					GET_ALPHA_3CH_1_NF();
					DIV_ALPHA_3CH_NF();
				}

			} else {	/* if (alpha == 2) */

#pragma pipeloop(0)
				for (i = 0; i < ww - 3; i += 3) {
					GET_ALPHA_3CH_2();
					DIV_ALPHA_3CH();
				}

				if (i < ww) {
					GET_ALPHA_3CH_2_NF();
					DIV_ALPHA_3CH_NF();
				}
			}

		} else {	/* if (channel == 2) */

#pragma pipeloop(0)
			for (i = 0; i < ww; i++) {
				ss = *sp;
				a0 = vis_freg_pair(*(mlib_f32 *)(p_tbl + ap[0]),
				    *(mlib_f32 *)(p_tbl +
				    vis_ld_u8_nf(ap + 2)));
				a1 = vis_freg_pair(*(mlib_f32 *)(p_tbl +
					vis_ld_u8_nf(ap + 4)),
				    *(mlib_f32 *)(p_tbl +
				    vis_ld_u8_nf(ap + 6)));
				DIV_ALPHA(d0, vis_read_hi(ss), a0);
				DIV_ALPHA(d1, vis_read_lo(ss), a1);
				*dp = vis_fpack16_pair(d0, d1);
				ap += 8;
				sp++;
				dp++;
			}
		}

		if (dflag) {
			MEM_COPY(buffd, dl, width * sizeof (mlib_u8));
		}

		sl += sstride;
		dl += dstride;
	}

	__mlib_free(buffs);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
