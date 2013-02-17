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

#pragma ident	"@(#)mlib_i_ImageChannelInsert_f.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_c_ImageChannelInsert_U8
 *      mlib_c_ImageChannelInsert_S16
 *      mlib_c_ImageChannelInsert_S32
 *      mlib_c_ImageChannelInsert_D64
 *
 * DESCRIPTION
 *      Internal functions.
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_c_ImageChannelInsert.h>

/* *********************************************************** */

void
mlib_c_ImageChannelInsert_U8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 channels,
    mlib_s32 channeld,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask)
{
/* pointer for pixel in src */
	mlib_u8 *sp;

/* pointer for line in src */
	mlib_u8 *sl;

/* pointer for pixel in dst */
	mlib_u8 *dp;

/* pointer for line in dst */
	mlib_u8 *dl;

/* indices for x, y, channel */
	mlib_s32 i, j, k;
	mlib_s32 deltac[5] = { 0, 1, 1, 1, 1 };
	mlib_s32 inc0, inc1, inc2;
	mlib_u8 s0, s1, s2;

	deltac[channels] = 1;
	for (i = (channeld - 1), k = 0; i >= 0; i--) {
		if ((cmask & (1 << i)) == 0)
			deltac[k]++;
		else
			k++;
	}

	deltac[channels] = channeld;
	for (i = 1; i < channels; i++) {
		deltac[channels] -= deltac[i];
	}

	sp = sl = (void *)src;
	dp = dl = dst + deltac[0];

	if (channels == 1) {
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				s0 = sp[i];
				dp[0] = s0;
				dp += channeld;
			}

			sp = sl += slb;
			dp = dl += dlb;
		}
	} else if (channels == 2) {
		inc0 = deltac[1];
		inc1 = deltac[2] + inc0;
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				s1 = sp[1];
				dp[0] = s0;
				dp[inc0] = s1;
				dp += inc1;
				sp += 2;
			}

			sp = sl += slb;
			dp = dl += dlb;
		}
	} else if (channels == 3) {
		inc0 = deltac[1];
		inc1 = deltac[2] + inc0;
		inc2 = deltac[3] + inc1;
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				s1 = sp[1];
				s2 = sp[2];
				dp[0] = s0;
				dp[inc0] = s1;
				dp[inc1] = s2;
				dp += inc2;
				sp += 3;
			}

			sp = sl += slb;
			dp = dl += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageChannelInsert_S16(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 channels,
    mlib_s32 channeld,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask)
{
/* pointer for pixel in src */
	mlib_s16 *sp;

/* pointer for line in src */
	mlib_s16 *sl;

/* pointer for pixel in dst */
	mlib_s16 *dp;

/* pointer for line in dst */
	mlib_s16 *dl;

/* indices for x, y, channel */
	mlib_s32 i, j, k;
	mlib_s32 deltac[5] = { 0, 1, 1, 1, 1 };
	mlib_s32 inc0, inc1, inc2;
	mlib_s16 s0, s1, s2;

	deltac[channels] = 1;
	for (i = (channeld - 1), k = 0; i >= 0; i--) {
		if ((cmask & (1 << i)) == 0)
			deltac[k]++;
		else
			k++;
	}

	deltac[channels] = channeld;
	for (i = 1; i < channels; i++) {
		deltac[channels] -= deltac[i];
	}

	sp = sl = (void *)src;
	dp = dl = dst + deltac[0];

	if (channels == 1) {
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				dp[0] = s0;
				dp += channeld;
				sp++;
			}

			sp = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
		}
	} else if (channels == 2) {
		inc0 = deltac[1];
		inc1 = deltac[2] + inc0;
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				s1 = sp[1];
				dp[0] = s0;
				dp[inc0] = s1;
				dp += inc1;
				sp += 2;
			}

			sp = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
		}
	} else if (channels == 3) {
		inc0 = deltac[1];
		inc1 = deltac[2] + inc0;
		inc2 = deltac[3] + inc1;
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				s1 = sp[1];
				s2 = sp[2];
				dp[0] = s0;
				dp[inc0] = s1;
				dp[inc1] = s2;
				dp += inc2;
				sp += 3;
			}

			sp = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageChannelInsert_S32(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 channels,
    mlib_s32 channeld,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask)
{
/* pointer for pixel in src */
	mlib_s32 *sp;

/* pointer for line in src */
	mlib_s32 *sl;

/* pointer for pixel in dst */
	mlib_s32 *dp;

/* pointer for line in dst */
	mlib_s32 *dl;

/* indices for x, y, channel */
	mlib_s32 i, j, k;
	mlib_s32 deltac[5] = { 0, 1, 1, 1, 1 };
	mlib_s32 inc0, inc1, inc2;
	mlib_s32 s0, s1, s2;

	deltac[channels] = 1;
	for (i = (channeld - 1), k = 0; i >= 0; i--) {
		if ((cmask & (1 << i)) == 0)
			deltac[k]++;
		else
			k++;
	}

	deltac[channels] = channeld;
	for (i = 1; i < channels; i++) {
		deltac[channels] -= deltac[i];
	}

	sp = sl = (void *)src;
	dp = dl = dst + deltac[0];

	if (channels == 1) {
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				dp[0] = s0;
				dp += channeld;
				sp++;
			}

			sp = sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
		}
	} else if (channels == 2) {
		inc0 = deltac[1];
		inc1 = deltac[2] + inc0;
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				s1 = sp[1];
				dp[0] = s0;
				dp[inc0] = s1;
				dp += inc1;
				sp += 2;
			}

			sp = sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
		}
	} else if (channels == 3) {
		inc0 = deltac[1];
		inc1 = deltac[2] + inc0;
		inc2 = deltac[3] + inc1;
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				s1 = sp[1];
				s2 = sp[2];
				dp[0] = s0;
				dp[inc0] = s1;
				dp[inc1] = s2;
				dp += inc2;
				sp += 3;
			}

			sp = sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageChannelInsert_D64(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 channels,
    mlib_s32 channeld,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask)
{
/* pointer for pixel in src */
	mlib_s64 *sp;

/* pointer for line in src */
	mlib_s64 *sl;

/* pointer for pixel in dst */
	mlib_s64 *dp;

/* pointer for line in dst */
	mlib_s64 *dl;

/* indices for x, y, channel */
	mlib_s32 i, j, k;
	mlib_s32 deltac[5] = { 0, 1, 1, 1, 1 };
	mlib_s32 inc0, inc1, inc2;
	mlib_s64 s0, s1, s2;

	deltac[channels] = 1;
	for (i = (channeld - 1), k = 0; i >= 0; i--) {
		if ((cmask & (1 << i)) == 0)
			deltac[k]++;
		else
			k++;
	}

	deltac[channels] = channeld;
	for (i = 1; i < channels; i++) {
		deltac[channels] -= deltac[i];
	}

	sp = sl = (void *)src;
	dp = dl = (void *)(dst + deltac[0]);

	if (channels == 1) {
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				dp[0] = s0;
				dp += channeld;
				sp++;
			}

			sp = sl = (mlib_s64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s64 *)((mlib_u8 *)dl + dlb);
		}
	} else if (channels == 2) {
		inc0 = deltac[1];
		inc1 = deltac[2] + inc0;
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				s1 = sp[1];
				dp[0] = s0;
				dp[inc0] = s1;
				dp += inc1;
				sp += 2;
			}

			sp = sl = (mlib_s64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s64 *)((mlib_u8 *)dl + dlb);
		}
	} else if (channels == 3) {
		inc0 = deltac[1];
		inc1 = deltac[2] + inc0;
		inc2 = deltac[3] + inc1;
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				s1 = sp[1];
				s2 = sp[2];
				dp[0] = s0;
				dp[inc0] = s1;
				dp[inc1] = s2;
				dp += inc2;
				sp += 3;
			}

			sp = sl = (mlib_s64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s64 *)((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */
