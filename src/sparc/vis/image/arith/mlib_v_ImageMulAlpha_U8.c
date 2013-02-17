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

#pragma ident	"@(#)mlib_v_ImageMulAlpha_U8.c	9.2	07/11/05 SMI"

/*
 *  Internal function for mlib_ImageMulAlpha on U8 data type.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCopy.h>
#include <mlib_v_ImageMulAlpha.h>

/* *********************************************************** */

#define	MEM_COPY(src, dst, n)	mlib_ImageCopy_na((void*)src, (void*)dst, n)

/* *********************************************************** */

#define	EXPAND0(x)	vis_fpmerge(fzeros, x)
#define	EXPAND1(x)	vis_fmul8x16al(x, fzeros)
#define	EXPAND2(x)	vis_fexpand(x)

/* *********************************************************** */

#define	MUL_ALPHA_2CH_0()                                       \
	rr = vis_fand(ss, dmask);                               \
	s1 = vis_fpmerge(vis_read_hi(ss), vis_read_lo(ss));     \
	s1 = vis_fpmerge(vis_read_hi(s1), vis_read_lo(s1));     \
	rr = vis_fmul8x16(vis_read_hi(s1), rr);                 \
	rr = vis_fpmerge(vis_read_hi(s1), vis_fpack16(rr))

/* *********************************************************** */

#define	MUL_ALPHA_2CH_1()                                       \
	rr = vis_fand(ss, dmask);                               \
	s1 = vis_fpmerge(vis_read_hi(ss), vis_read_lo(ss));     \
	s1 = vis_fpmerge(vis_read_hi(s1), vis_read_lo(s1));     \
	rr = vis_fmul8x16(vis_read_hi(s1), rr);                 \
	rr = vis_fpmerge(vis_fpack16(rr), vis_read_lo(s1))

/* *********************************************************** */

#define	LOAD_3CH_0_NF()                                         \
	s0 = sp[0];                                             \
	s1 = vis_ld_d64_nf(sp + 1);                             \
	s2 = vis_ld_d64_nf(sp + 2);                             \
	a0 = vis_fand(s0, amask0);                              \
	a1 = vis_fand(s1, amask1);                              \
	a2 = vis_fand(s2, amask2);                              \
	a2 = vis_faligndata(a1, a2);                            \
	a1 = vis_faligndata(a0, a1);                            \
	a0 = vis_faligndata(a0, a0);                            \
	a2 = vis_for(a2, vis_faligndata(a1, a2));               \
	a1 = vis_for(a1, vis_faligndata(a0, a1));               \
	a0 = vis_for(a0, vis_faligndata(done, a0))

/* *********************************************************** */

#define	LOAD_3CH_0()                                            \
	s0 = sp[0];                                             \
	s1 = sp[1];                                             \
	s2 = sp[2];                                             \
	a0 = vis_fand(s0, amask0);                              \
	a1 = vis_fand(s1, amask1);                              \
	a2 = vis_fand(s2, amask2);                              \
	a2 = vis_faligndata(a1, a2);                            \
	a1 = vis_faligndata(a0, a1);                            \
	a0 = vis_faligndata(a0, a0);                            \
	a2 = vis_for(a2, vis_faligndata(a1, a2));               \
	a1 = vis_for(a1, vis_faligndata(a0, a1));               \
	a0 = vis_for(a0, vis_faligndata(done, a0))

/* *********************************************************** */

#define	LOAD_3CH_1_NF()                                         \
	s0 = sp[0];                                             \
	s1 = vis_ld_d64_nf(sp + 1);                             \
	s2 = vis_ld_d64_nf(sp + 2);                             \
	a0 = vis_fand(s0, amask0);                              \
	a1 = vis_fand(s1, amask1);                              \
	a2 = vis_fand(s2, amask2);                              \
	vis_alignaddr((void *)0, 1);                            \
	b0 = vis_faligndata(a0, a0);                            \
	b1 = vis_faligndata(a1, a2);                            \
	b2 = vis_faligndata(a2, a0);                            \
	vis_alignaddr((void *)0, 7);                            \
	a2 = vis_for(b2, vis_faligndata(a1, a2));               \
	a1 = vis_for(b1, vis_faligndata(a0, a1));               \
	a0 = vis_for(b0, vis_faligndata(done, a0))

/* *********************************************************** */

#define	LOAD_3CH_1()                                            \
	s0 = sp[0];                                             \
	s1 = sp[1];                                             \
	s2 = sp[2];                                             \
	a0 = vis_fand(s0, amask0);                              \
	a1 = vis_fand(s1, amask1);                              \
	a2 = vis_fand(s2, amask2);                              \
	vis_alignaddr((void *)0, 1);                            \
	b0 = vis_faligndata(a0, a0);                            \
	b1 = vis_faligndata(a1, a2);                            \
	b2 = vis_faligndata(a2, a0);                            \
	vis_alignaddr((void *)0, 7);                            \
	a2 = vis_for(b2, vis_faligndata(a1, a2));               \
	a1 = vis_for(b1, vis_faligndata(a0, a1));               \
	a0 = vis_for(b0, vis_faligndata(done, a0))

/* *********************************************************** */

#define	LOAD_3CH_2_NF()                                         \
	s0 = sp[0];                                             \
	s1 = vis_ld_d64_nf(sp + 1);                             \
	s2 = vis_ld_d64_nf(sp + 2);                             \
	a0 = vis_fand(s0, amask0);                              \
	a1 = vis_fand(s1, amask1);                              \
	a2 = vis_fand(s2, amask2);                              \
	a0 = vis_faligndata(a0, a1);                            \
	a1 = vis_faligndata(a1, a2);                            \
	a2 = vis_faligndata(a2, a2);                            \
	a0 = vis_for(a0, vis_faligndata(a0, a1));               \
	a1 = vis_for(a1, vis_faligndata(a1, a2));               \
	a2 = vis_for(a2, vis_faligndata(a2, amask0))

/* *********************************************************** */

#define	LOAD_3CH_2()                                            \
	s0 = sp[0];                                             \
	s1 = sp[1];                                             \
	s2 = sp[2];                                             \
	a0 = vis_fand(s0, amask0);                              \
	a1 = vis_fand(s1, amask1);                              \
	a2 = vis_fand(s2, amask2);                              \
	a0 = vis_faligndata(a0, a1);                            \
	a1 = vis_faligndata(a1, a2);                            \
	a2 = vis_faligndata(a2, a2);                            \
	a0 = vis_for(a0, vis_faligndata(a0, a1));               \
	a1 = vis_for(a1, vis_faligndata(a1, a2));               \
	a2 = vis_for(a2, vis_faligndata(a2, amask0))

/* *********************************************************** */

#define	MUL_ALPHA(rr, ss, aa, EXPAND)                           \
	rr = vis_fmul8x16(aa, EXPAND(ss))

/* *********************************************************** */

#define	MUL_ALPHA_3CH()                                             \
	MUL_ALPHA(d0, vis_read_hi(s0), vis_read_hi(a0), EXPAND0);   \
	MUL_ALPHA(d1, vis_read_lo(s0), vis_read_lo(a0), EXPAND0);   \
	d0 = vis_fpack16_pair(d0, d1);                              \
	vis_pst_8(d0, dp, ~cmask0);                                 \
	vis_pst_8(s0, dp, cmask0);                                  \
	MUL_ALPHA(d0, vis_read_hi(s1), vis_read_hi(a1), EXPAND0);   \
	MUL_ALPHA(d1, vis_read_lo(s1), vis_read_lo(a1), EXPAND0);   \
	d0 = vis_fpack16_pair(d0, d1);                              \
	vis_pst_8(d0, dp + 1, ~cmask1);                             \
	vis_pst_8(s1, dp + 1, cmask1);                              \
	MUL_ALPHA(d0, vis_read_hi(s2), vis_read_hi(a2), EXPAND0);   \
	MUL_ALPHA(d1, vis_read_lo(s2), vis_read_lo(a2), EXPAND0);   \
	d0 = vis_fpack16_pair(d0, d1);                              \
	vis_pst_8(d0, dp + 2, ~cmask2);                             \
	vis_pst_8(s2, dp + 2, cmask2);                              \
	sp += 3;                                                    \
	dp += 3

/* *********************************************************** */

#define	MUL_ALPHA_4CH(h1, h2)                                   \
	ss = (*sp++);                                           \
	a0 = EXPAND0(vis_read_hi(ss));                          \
	a1 = EXPAND0(vis_read_lo(ss));                          \
	d0 =                                                    \
	vis_fmul8x16##h2(vis_read_hi(ss), vis_read_##h1(a0));   \
	d1 =                                                    \
	vis_fmul8x16##h2(vis_read_lo(ss), vis_read_##h1(a1));   \
	d0 = vis_fpack16_pair(d0, d1);                          \
	vis_pst_8(d0, dp, ~cmask);                              \
	vis_pst_8(ss, dp, cmask);                               \
	dp++

/* *********************************************************** */

const mlib_u64 mlib_amask3_arr[] = {
	0xFF0000FF0000FF00,
	0x00FF0000FF0000FF,
	0x0000FF0000FF0000,
	0xFF0000FF0000FF00,
	0x00FF0000FF0000FF
};

/* *********************************************************** */

mlib_status
mlib_ImageMulAlpha_U8(
    mlib_u8 *sl,
    mlib_u8 *dl,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha)
{
	mlib_f32 fzeros = vis_fzeros();
	mlib_d64 dmask = vis_to_double_dup(0x00FF00FF);
	mlib_d64 done = vis_to_double_dup(0x01000100);
	mlib_d64 *buffs, *buffd;
	mlib_d64 *sp, *dp;
	mlib_d64 ss, s1, rr, d0, d1;
	mlib_d64 amask0, amask1, amask2;
	mlib_s32 ww, dflag, cmask, i, j;

	vis_write_gsr(7 << 3);

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

	if (channel == 4) {
		cmask = 1 << (3 - alpha);
		cmask |= (cmask << 4);
	} else if (channel == 3) {
		amask0 = ((mlib_d64 *)mlib_amask3_arr)[alpha];
		amask1 = ((mlib_d64 *)mlib_amask3_arr)[alpha + 1];
		amask2 = ((mlib_d64 *)mlib_amask3_arr)[alpha + 2];
	}

	for (j = 0; j < height; j++) {
		if (((int)sl & 7)) {
			MEM_COPY(sl, buffs, width);
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
			mlib_d64 a0, a1;

			if (alpha == 0) {
#pragma pipeloop(0)
				for (i = 0; i < ww; i++) {
					MUL_ALPHA_4CH(hi, au);
				}

			} else if (alpha == 1) {
#pragma pipeloop(0)
				for (i = 0; i < ww; i++) {
					MUL_ALPHA_4CH(hi, al);
				}

			} else if (alpha == 2) {
#pragma pipeloop(0)
				for (i = 0; i < ww; i++) {
					MUL_ALPHA_4CH(lo, au);
				}

			} else {	/* if (alpha == 3) */

#pragma pipeloop(0)
				for (i = 0; i < ww; i++) {
					MUL_ALPHA_4CH(lo, al);
				}
			}

		} else if (channel == 3) {
			mlib_d64 s0, s1, s2;
			mlib_d64 a0, a1, a2;
			mlib_s32 cmask0, cmask1, cmask2;

			cmask0 = 0x492 >> alpha;
			cmask1 = 0x492 >> (alpha + 1);
			cmask2 = 0x492 >> (alpha + 2);

			if (alpha == 0) {
				vis_alignaddr((void *)0, 7);
#pragma pipeloop(0)
				for (i = 0; i < ww - 3; i += 3) {
					LOAD_3CH_0();
					MUL_ALPHA_3CH();
				}

				if (i < ww) {
					LOAD_3CH_0_NF();
					MUL_ALPHA_3CH();
				}

			} else if (alpha == 1) {
				mlib_d64 b0, b1, b2;

#pragma pipeloop(0)
				for (i = 0; i < ww - 3; i += 3) {
					LOAD_3CH_1();
					MUL_ALPHA_3CH();
				}

				if (i < ww) {
					LOAD_3CH_1_NF();
					MUL_ALPHA_3CH();
				}

			} else {	/* if (alpha == 2) */

				vis_alignaddr((void *)0, 1);
#pragma pipeloop(0)
				for (i = 0; i < ww - 3; i += 3) {
					LOAD_3CH_2();
					MUL_ALPHA_3CH();
				}

				if (i < ww) {
					LOAD_3CH_2_NF();
					MUL_ALPHA_3CH();
				}

			}

		} else {	/* if (channel == 2) */

			if (alpha == 0) {
#pragma pipeloop(0)
				for (i = 0; i < ww; i++) {
					ss = (*sp++);
					MUL_ALPHA_2CH_0();
					(*dp++) = rr;
				}

			} else {	/* if (alpha == 1) */

#pragma pipeloop(0)
				for (i = 0; i < ww; i++) {
					ss = (*sp++);
					MUL_ALPHA_2CH_1();
					(*dp++) = rr;
				}
			}
		}

		if (dflag) {
			MEM_COPY(buffd, dl, width);
		}

		sl += sstride;
		dl += dstride;
	}

	__mlib_free(buffs);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
