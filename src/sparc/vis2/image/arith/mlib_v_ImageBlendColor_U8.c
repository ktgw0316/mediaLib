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

#pragma ident	"@(#)mlib_v_ImageBlendColor_U8.c	9.2	07/11/05 SMI"

/*
 *  Internal function for mlib_ImageBlendColor on U8 data type.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCopy.h>
#include <mlib_ImageBlendColor.h>
#include <mlib_v_ImageMulAlpha.h>

/* *********************************************************** */

static const mlib_u64 mlib_dmask_arr[] = {
	0x0000000000000000, 0x000000000000FFFF, 0x00000000FFFF0000,
	    0x00000000FFFFFFFF,
	0x0000FFFF00000000, 0x0000FFFF0000FFFF, 0x0000FFFFFFFF0000,
	    0x0000FFFFFFFFFFFF,
	0xFFFF000000000000, 0xFFFF00000000FFFF, 0xFFFF0000FFFF0000,
	    0xFFFF0000FFFFFFFF,
	0xFFFFFFFF00000000, 0xFFFFFFFF0000FFFF, 0xFFFFFFFFFFFF0000,
	    0xFFFFFFFFFFFFFFFF
};

/* *********************************************************** */

#define	MEM_COPY(src, dst, n)	mlib_ImageCopy_na((void*)src, (void*)dst, n)

/* *********************************************************** */

#define	DOUBLE_4U16(x0, x1, x2, x3)                             \
	vis_to_double((((x0 & 0xFF) << 16) | (x1 & 0xFF)),      \
	    (((x2 & 0xFF) << 16) | (x3 & 0xFF)))

/* *********************************************************** */

#define	EXPAND0(x)	vis_fpmerge(fzeros, x)
#define	EXPAND1(x)	vis_fmul8x16al(x, fzeros)
#define	EXPAND2(x)	vis_fexpand(x)

/* *********************************************************** */

#define	BLEND_COLOR(rr, src, alp, cc, EXPAND)                   \
	rr = EXPAND(src);                                       \
	rr = vis_fpsub16(rr, cc);                               \
	rr = vis_fmul8x16(alp, rr);                             \
	rr = vis_fpadd16(rr, cc)

/* *********************************************************** */

#define	BLEND_COLOR_2CH_0()                                     \
	rr = vis_fand(ss, dmask);                               \
	rr = vis_fpsub16(rr, cc);                               \
	ss = vis_bshuffle(ss, ss);                              \
	rr = vis_fmul8x16(vis_read_hi(ss), rr);                 \
	rr = vis_fpadd16(rr, cc);                               \
	rr = vis_fpmerge(fmax, vis_fpack16(rr))

/* *********************************************************** */

#define	BLEND_COLOR_2CH_1()                                     \
	rr = vis_fand(ss, dmask);                               \
	tt = vis_fpsub16(done, rr);                             \
	ss = vis_bshuffle(ss, ss);                              \
	rr = vis_fmul8x16(vis_read_hi(ss), rr);                 \
	tt = vis_fmul8x16(vis_read_hi(cc), tt);                 \
	rr = vis_fpadd16(rr, tt);                               \
	rr = vis_fpmerge(vis_fpack16(rr), fmax)

/* *********************************************************** */

#define	LOAD_3CH_0()                                            \
	s0 = sp[0];                                             \
	s1 = sp[1];                                             \
	s2 = sp[2];                                             \
	t1 = vis_fand(s1, amask1);                              \
	vis_write_bmask(bm0, 0);                                \
	a0 = vis_bshuffle(s0, t1);                              \
	vis_write_bmask(bm1, 0);                                \
	a1 = vis_bshuffle(s0, t1);                              \
	vis_write_bmask(bm2, 0);                                \
	a2 = vis_bshuffle(t1, s2)

/* *********************************************************** */

#define	LOAD_3CH_1()                                            \
	s0 = sp[0];                                             \
	s1 = sp[1];                                             \
	s2 = sp[2];                                             \
	t1 = vis_fand(s1, amask1);                              \
	vis_write_bmask(bm0, 0);                                \
	a0 = vis_bshuffle(s0, t1);                              \
	vis_write_bmask(bm1, 0);                                \
	a1 = vis_bshuffle(s0, t1);                              \
	vis_write_bmask(bm2, 0);                                \
	a2 = vis_bshuffle(t1, s2);                              \
	vis_write_bmask(bm3, 0);                                \
	a1 = vis_bshuffle(a1, s2)

/* *********************************************************** */

#define	LOAD_3CH_2()                                            \
	s0 = sp[0];                                             \
	s1 = sp[1];                                             \
	s2 = sp[2];                                             \
	t1 = vis_fand(s1, amask1);                              \
	vis_write_bmask(bm0, 0);                                \
	a0 = vis_bshuffle(s0, t1);                              \
	vis_write_bmask(bm1, 0);                                \
	a1 = vis_bshuffle(t1, s2);                              \
	vis_write_bmask(bm2, 0);                                \
	a2 = vis_bshuffle(t1, s2)

/* *********************************************************** */

#define	BLEND_COLOR_3CH()                                                 \
	BLEND_COLOR(d0, vis_read_hi(s0), vis_read_hi(a0), c0, EXPAND0);   \
	BLEND_COLOR(d1, vis_read_lo(s0), vis_read_lo(a0), c1, EXPAND0);   \
	dp[0] = vis_fpack16_pair(d0, d1);                                 \
	BLEND_COLOR(d0, vis_read_hi(s1), vis_read_hi(a1), c2, EXPAND0);   \
	BLEND_COLOR(d1, vis_read_lo(s1), vis_read_lo(a1), c0, EXPAND0);   \
	dp[1] = vis_fpack16_pair(d0, d1);                                 \
	BLEND_COLOR(d0, vis_read_hi(s2), vis_read_hi(a2), c1, EXPAND0);   \
	BLEND_COLOR(d1, vis_read_lo(s2), vis_read_lo(a2), c2, EXPAND0);   \
	dp[2] = vis_fpack16_pair(d0, d1);                                 \
	sp += 3;                                                          \
	dp += 3

/* *********************************************************** */

static mlib_u32 mlib_bmask_arr[] = {
/* nchan = 3, alpha = 0 */
	0xE00E33E6,
	0x6E99ECCE,
	0x770AA0DD,
	0,
/* nchan = 3, alpha = 1 */
	0x1F14F47F,
	0x7AFADFDF,
	0x08B0BE0E,
	0x01234568,
/* nchan = 3, alpha = 2 */
	0x22F55F88,
	0x13316619,
	0x91CC1FF1,
	0,
/* nchan = 4 */
	0x80008444,
	0x18115855,
	0x22826686,
	0x33387778
};

/* *********************************************************** */

DEF_FUNC(mlib_ImageBlendColor_U8, mlib_u8,
    mlib_s32)
{
	mlib_f32 fzeros = vis_fzeros();
	mlib_f32 fmax = vis_to_float(0xFFFFFFFF);
	mlib_d64 dmask = vis_to_double_dup(0x00FF00FF);
	mlib_d64 done = vis_to_double_dup(0x01000100);
	mlib_d64 *buffs, *buffd;
	mlib_d64 *sp, *dp;
	mlib_d64 ss, rr, tt, d0, d1;
	mlib_d64 cc, c0, c1, c2;
	mlib_d64 amask1;
	mlib_s32 ww, dflag, i, j;

	width *= channel;
	ww = (width + 7) / 8;

	if (channel == 3) {
		ww = 3 * ((ww + 2) / 3);
	}

	buffs = __mlib_malloc(2 * sizeof (mlib_d64) * ww);

	if (buffs == NULL)
		return (MLIB_FAILURE);
	buffd = buffs + ww;

	vis_write_gsr(7 << 3);

	if (channel == 4) {
		cc = DOUBLE_4U16(color[0], color[1], color[2], color[3]);
		cc = vis_fand(vis_for(cc,
		    ((mlib_d64 *)mlib_dmask_arr)[8 >> alpha]), dmask);
		vis_write_bmask(mlib_bmask_arr[12 + alpha], 0);
	} else if (channel == 3) {
		mlib_d64 c_arr[3];
		mlib_s16 *p_arr = (mlib_s16 *)c_arr;

		amask1 = ((mlib_d64 *)mlib_amask3_arr)[alpha + 1];
		p_arr[0] = color[0] & 0xFF;
		p_arr[1] = color[1] & 0xFF;
		p_arr[2] = color[2] & 0xFF;
		p_arr[alpha] = 0xFF;
		for (i = 0; i < 3; i++) {
			p_arr[i + 3] = p_arr[i + 6] = p_arr[i + 9] = p_arr[i];
		}

		c0 = c_arr[0];
		c1 = c_arr[1];
		c2 = c_arr[2];
	} else {	/* if (channel == 2) */

		vis_write_bmask(0x02461357, 0);

		if (alpha == 0) {
			mlib_s32 col = color[1] & 0xFF;

			cc = vis_to_double_dup((col << 16) | col);
		} else {
			mlib_s32 col = color[0] & 0xFF;

			cc = vis_to_double_dup((col << 24) | (col << 16) | (col
			    << 8) | col);
		}
	}

	for (j = 0; j < height; j++) {
		if (((mlib_addr)sl & 7)) {
			MEM_COPY(sl, buffs, width);
			sp = buffs;
		} else {
			sp = (mlib_d64 *)sl;
		}

		dflag = 0;

		if (((mlib_addr)dl | width) & 7) {
			dp = buffd;
			dflag = 1;
		} else {
			dp = (mlib_d64 *)dl;
		}

		if (channel == 4) {
			mlib_d64 aa;

#pragma pipeloop(0)
			for (i = 0; i < ww; i++) {
				ss = (*sp++);
				aa = vis_bshuffle(ss, dmask);
				BLEND_COLOR(d0, vis_read_hi(ss),
				    vis_read_hi(aa), cc, EXPAND0);
				BLEND_COLOR(d1, vis_read_lo(ss),
				    vis_read_lo(aa), cc, EXPAND0);
				(*dp++) = vis_fpack16_pair(d0, d1);
			}

		} else if (channel == 3) {
			mlib_d64 s0, s1, s2, t1;
			mlib_d64 a0, a1, a2;
			mlib_u32 bm0 = mlib_bmask_arr[4 * alpha];
			mlib_u32 bm1 = mlib_bmask_arr[4 * alpha + 1];
			mlib_u32 bm2 = mlib_bmask_arr[4 * alpha + 2];
			mlib_u32 bm3 = mlib_bmask_arr[4 * alpha + 3];

			if (alpha == 0) {
				vis_alignaddr((void *)0, 7);
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

				vis_alignaddr((void *)0, 1);
#pragma pipeloop(0)
				for (i = 0; i < ww; i += 3) {
					LOAD_3CH_2();
					BLEND_COLOR_3CH();
				}
			}

		} else {	/* if (channel == 2) */

			if (alpha == 0) {
#pragma pipeloop(0)
				for (i = 0; i < ww; i++) {
					ss = (*sp++);
					BLEND_COLOR_2CH_0();
					(*dp++) = rr;
				}

			} else {	/* if (alpha == 1) */

#pragma pipeloop(0)
				for (i = 0; i < ww; i++) {
					ss = (*sp++);
					BLEND_COLOR_2CH_1();
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
