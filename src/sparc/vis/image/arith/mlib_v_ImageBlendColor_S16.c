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

#pragma ident	"@(#)mlib_v_ImageBlendColor_S16.c	9.2	07/11/05 SMI"

/*
 *  Internal function for mlib_ImageBlendColor on S16 data type.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageBlendColor.h>
#include <mlib_ImageCopy.h>

/* *********************************************************** */

#define	MEM_COPY(src, dst, n)	mlib_ImageCopy_na((void*)src, (void*)dst, n)

/* *********************************************************** */

#define	FMUL_16x16(x, y)                                          \
	vis_fpadd16(vis_fmul8sux16(x, y), vis_fmul8ulx16(x, y))

/* *********************************************************** */

#define	DOUBLE_4S16(x0, x1, x2, x3)                              \
	vis_to_double((((x0 & 0xFFFF) << 16) | (x1 & 0xFFFF)),   \
	    (((x2 & 0xFFFF) << 16) | (x3 & 0xFFFF)))

/* *********************************************************** */

#define	GET_ALPHA(aa, ss, h1, h2)                               \
	aa = vis_fand(ss, mask7FFE);                            \
	aa = vis_fmul8x16##h2(f_half, vis_read_##h1(aa));       \
	aa = vis_fpadd16(aa, aa)

/* *********************************************************** */

#define	BLEND_COLOR(rr, ss, aa, cc)                             \
	rr = vis_fpsub16(mask7FFF, aa);                         \
	rr = FMUL_16x16(rr, cc);                                \
	rr = vis_fpadd16(rr, FMUL_16x16(aa, ss));               \
	rr = vis_fpadd16(rr, rr)

/* *********************************************************** */

#define	LOAD_3CH_0()                                            \
	s0 = sp[0];                                             \
	s1 = sp[1];                                             \
	s2 = sp[2];                                             \
	GET_ALPHA(a0, s0, hi, au);                              \
	GET_ALPHA(a1, s0, lo, al);                              \
	GET_ALPHA(aa, s1, lo, au);                              \
	GET_ALPHA(a2, s2, hi, al);                              \
	a1 = vis_faligndata(a1, aa);                            \
	a2 = vis_faligndata(aa, a2)

/* *********************************************************** */

#define	LOAD_3CH_1()                                            \
	s0 = sp[0];                                             \
	s1 = sp[1];                                             \
	s2 = sp[2];                                             \
	GET_ALPHA(a0, s0, hi, al);                              \
	GET_ALPHA(a1, s1, hi, au);                              \
	GET_ALPHA(aa, s1, lo, al);                              \
	GET_ALPHA(a2, s2, lo, au);                              \
	vis_alignaddr((void *)0, 2);                            \
	a0 = vis_faligndata(a0, a1);                            \
	vis_alignaddr((void *)0, 4);                            \
	a1 = vis_faligndata(a1, aa);                            \
	vis_alignaddr((void *)0, 6);                            \
	a2 = vis_faligndata(aa, a2)

/* *********************************************************** */

#define	LOAD_3CH_2()                                            \
	s0 = sp[0];                                             \
	s1 = sp[1];                                             \
	s2 = sp[2];                                             \
	GET_ALPHA(a0, s0, lo, au);                              \
	GET_ALPHA(a1, s1, hi, al);                              \
	GET_ALPHA(aa, s2, hi, au);                              \
	GET_ALPHA(a2, s2, lo, al);                              \
	a0 = vis_faligndata(a0, a1);                            \
	a1 = vis_faligndata(a1, aa)

/* *********************************************************** */

#define	BLEND_COLOR_3CH()                                       \
	BLEND_COLOR(d0, s0, a0, c0);                            \
	vis_pst_16(d0, dp, ~cmask0);                            \
	vis_pst_16(mask7FFF, dp, cmask0);                       \
	BLEND_COLOR(d1, s1, a1, c1);                            \
	vis_pst_16(d1, dp + 1, ~cmask1);                        \
	vis_pst_16(mask7FFF, dp + 1, cmask1);                   \
	BLEND_COLOR(d2, s2, a2, c2);                            \
	vis_pst_16(d2, dp + 2, ~cmask2);                        \
	vis_pst_16(mask7FFF, dp + 2, cmask2);                   \
	sp += 3;                                                \
	dp += 3

/* *********************************************************** */

DEF_FUNC(mlib_ImageBlendColor_S16, mlib_s16,
    mlib_s32)
{
	mlib_f32 f_half = vis_to_float(0x80808080);
	mlib_d64 mask7FFF = vis_to_double_dup(0x7FFF7FFF);
	mlib_d64 mask7FFE = vis_to_double_dup(0x7FFE7FFE);
	mlib_d64 *buffs, *buffd;
	mlib_d64 *sp, *dp;
	mlib_d64 ss, dd, aa, cc;
	mlib_d64 c0, c1, c2;
	mlib_s32 cmask = (1 << (channel - alpha - 1));
	mlib_s32 ww, dflag, i, j;

	vis_write_gsr(7 << 3);

	width *= channel;
	ww = (width + 3) / 4;

	if (channel == 3) {
		ww = 3 * ((ww + 2) / 3);
	}

	buffs = __mlib_malloc(2 * sizeof (mlib_d64) * ww);

	if (buffs == NULL) {
		return (MLIB_FAILURE);
	}

	buffd = buffs + ww;

	if (channel == 4) {
		cc = DOUBLE_4S16(color[0], color[1], color[2], color[3]);
	} else if (channel == 3) {
		mlib_d64 c_arr[3];
		mlib_s16 *p_arr = (mlib_s16 *)c_arr;

		for (j = 0; j < 4; j++) {
			for (i = 0; i < 3; i++) {
				p_arr[3 * j + i] = color[i];
			}
		}

		c0 = c_arr[0];
		c1 = c_arr[1];
		c2 = c_arr[2];
	} else {	/* if (channel == 2) */

		cc = DOUBLE_4S16(color[0], color[1], color[0], color[1]);
		cmask |= (cmask << channel);
	}

	for (j = 0; j < height; j++) {
		if (((mlib_s32)sl & 7)) {
			MEM_COPY(sl, buffs, width * sizeof (mlib_s16));
			sp = buffs;
		} else {
			sp = (mlib_d64 *)sl;
		}

		dflag = 0;

		if (((mlib_s32)dl | width) & 7) {
			dp = buffd;
			dflag = 1;
		} else {
			dp = (mlib_d64 *)dl;
		}

		if (channel == 4) {
			if (alpha == 0) {
#pragma pipeloop(0)
				for (i = 0; i < ww; i++) {
					ss = (*sp++);
					GET_ALPHA(aa, ss, hi, au);
					BLEND_COLOR(dd, ss, aa, cc);
					vis_pst_16(dd, dp, ~cmask);
					vis_pst_16(mask7FFF, dp, cmask);
					dp++;
				}

			} else if (alpha == 1) {
#pragma pipeloop(0)
				for (i = 0; i < ww; i++) {
					ss = (*sp++);
					GET_ALPHA(aa, ss, hi, al);
					BLEND_COLOR(dd, ss, aa, cc);
					vis_pst_16(dd, dp, ~cmask);
					vis_pst_16(mask7FFF, dp, cmask);
					dp++;
				}

			} else if (alpha == 2) {
#pragma pipeloop(0)
				for (i = 0; i < ww; i++) {
					ss = (*sp++);
					GET_ALPHA(aa, ss, lo, au);
					BLEND_COLOR(dd, ss, aa, cc);
					vis_pst_16(dd, dp, ~cmask);
					vis_pst_16(mask7FFF, dp, cmask);
					dp++;
				}

			} else {	/* if (alpha == 3) */

#pragma pipeloop(0)
				for (i = 0; i < ww; i++) {
					ss = (*sp++);
					GET_ALPHA(aa, ss, lo, al);
					BLEND_COLOR(dd, ss, aa, cc);
					vis_pst_16(dd, dp, ~cmask);
					vis_pst_16(mask7FFF, dp, cmask);
					dp++;
				}
			}

		} else if (channel == 3) {
			mlib_d64 a0, a1, a2;
			mlib_d64 s0, s1, s2;
			mlib_d64 d0, d1, d2;
			mlib_s32 cmask0, cmask1, cmask2;

			cmask0 = 0x49 >> alpha;
			cmask1 = 0x49 >> (alpha + 2);
			cmask2 = 0x49 >> (alpha + 1);

			vis_alignaddr((void *)0, 4);

			if (alpha == 0) {
#pragma pipeloop(0)
				for (i = 0; i < ww; i += 3) {
					LOAD_3CH_0();
					BLEND_COLOR_3CH();
				}

			} else if (alpha == 1) {
#pragma pipeloop(0)
				for (i = 0; i < ww; i += 3) {
					LOAD_3CH_1();
					BLEND_COLOR_3CH();
				}

			} else {	/* if (alpha == 2) */

#pragma pipeloop(0)
				for (i = 0; i < ww; i += 3) {
					LOAD_3CH_2();
					BLEND_COLOR_3CH();
				}
			}

		} else {	/* if (channel == 2) */

			vis_alignaddr((void *)0, 6 - 4 * alpha);

#pragma pipeloop(0)
			for (i = 0; i < ww; i++) {
				ss = (*sp++);
				aa = vis_faligndata(ss, ss);
				aa = vis_fand(aa, mask7FFF);
				BLEND_COLOR(dd, ss, aa, cc);
				vis_pst_16(dd, dp, ~cmask);
				vis_pst_16(mask7FFF, dp, cmask);
				dp++;
			}
		}

		if (dflag) {
			MEM_COPY(buffd, dl, width * sizeof (mlib_s16));
		}

		sl += sstride;
		dl += dstride;
	}

	__mlib_free(buffs);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
