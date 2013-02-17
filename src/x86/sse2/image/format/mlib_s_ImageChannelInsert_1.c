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

#pragma ident	"@(#)mlib_s_ImageChannelInsert_1.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_s_ImageChannelInsert_U8
 *      mlib_s_ImageChannelInsert_U8_12_A8D1X8
 *      mlib_s_ImageChannelInsert_U8_12_A8D2X8
 *      mlib_s_ImageChannelInsert_U8_12_D1
 *      mlib_s_ImageChannelInsert_U8_12
 *      mlib_s_ImageChannelInsert_U8_14_A8D1X8
 *      mlib_s_ImageChannelInsert_U8_14_A8D2X8
 *      mlib_s_ImageChannelInsert_U8_14_D1
 *      mlib_s_ImageChannelInsert_U8_14
 *      mlib_s_ImageChannelInsert_S16
 *      mlib_s_ImageChannelInsert_S16_12_A8D1X4
 *      mlib_s_ImageChannelInsert_S16_12_A8D2X4
 *      mlib_s_ImageChannelInsert_S16_12_D1
 *      mlib_s_ImageChannelInsert_S16_12
 *      mlib_s_ImageChannelInsert_S32
 *      mlib_s_ImageChannelInsert_D64
 *
 * ARGUMENT
 *      src     pointer to source image data
 *      dst     pointer to destination image data
 *      slb     source image line stride in bytes
 *      dlb     destination image line stride in bytes
 *      dsize   image data size in pixels
 *      xsize   image width in pixels
 *      ysize   image height in lines
 *      cmask   channel mask
 *
 * DESCRIPTION
 *      Copy the 1-channel source image into the selected channel
 *      of the destination image -- SSE2 version low level functions.
 *
 * NOTE
 *      These functions are separated from mlib_s_ImageChannelInsert.c
 *      for loop unrolling and structure clarity.
 */

#include <mlib_image.h>
#include <mlib_s_ImageChannelInsert.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

/* general channel insertion: slower due to the inner loop */
void
mlib_s_ImageChannelInsert_U8(
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

/* general channel insertion: slower due to the inner loop */
void
mlib_s_ImageChannelInsert_D64(
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
	mlib_d64 *sp;

/* pointer for line in src */
	mlib_d64 *sl;

/* pointer for pixel in dst */
	mlib_d64 *dp;

/* pointer for line in dst */
	mlib_d64 *dl;

/* indices for x, y, channel */
	mlib_s32 i, j, k;
	mlib_s32 deltac[5] = { 0, 1, 1, 1, 1 };
	mlib_s32 inc0, inc1, inc2;
	mlib_d64 s0, s1, s2;

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

			sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
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

			sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
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

			sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */

/* general channel insertion: slower due to the inner loop */
void
mlib_s_ImageChannelInsert_S16(
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

/* general channel insertion: slower due to the inner loop */

void
mlib_s_ImageChannelInsert_S32(
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

/*
 * insert one channel to a 2-channel image.
 * both source and destination image data are 16-byte aligned.
 * both stride is equal to width in bytes.
 */

void
mlib_s_ImageChannelInsert_U8_12_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_u8 *sp, *dp;
	__m128i x_mask, x_nmask;
	mlib_s32 bmask = 0;
	mlib_s32 i;

	for (i = 1; i >= 0; i--) {
		if ((cmask & (1 << i)) != 0) {
			bmask += (0xFF << (8 * i));
		}
	}

	x_mask = _mm_set1_epi16(bmask);
	x_nmask = _mm_set1_epi16((~bmask));

	sp = (mlib_u8 *)src;
	dp = (mlib_u8 *)dst;

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 15); i += 16) {
		__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_sd4;
		__m128i x_dd0, x_dd1, x_dd2, x_dd3, x_dd4, x_dd5;

		x_sd0 = _mm_load_si128((__m128i *)sp);
		x_dd0 = _mm_load_si128((__m128i *)dp);
		x_dd1 = _mm_load_si128((__m128i *)dp + 1);

		x_sd1 = _mm_unpacklo_epi8(x_sd0, x_sd0);
		x_sd2 = _mm_unpackhi_epi8(x_sd0, x_sd0);

		x_sd3 = _mm_and_si128(x_nmask, x_sd1);
		x_sd4 = _mm_and_si128(x_nmask, x_sd2);
		x_dd2 = _mm_and_si128(x_mask, x_dd0);
		x_dd3 = _mm_and_si128(x_mask, x_dd1);
		x_dd4 = _mm_or_si128(x_sd3, x_dd2);
		x_dd5 = _mm_or_si128(x_sd4, x_dd3);

		_mm_store_si128((__m128i *)dp, x_dd4);
		dp += 16;
		_mm_store_si128((__m128i *)dp, x_dd5);
		dp += 16;
		sp += 16;
	}

/* 8 pixels */
	if (i < (dsize - 7)) {
		__m128i x_sd0, x_sd1, x_sd2;
		__m128i x_dd0, x_dd1, x_dd2;

		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_sd1 = _mm_unpacklo_epi8(x_sd0, x_sd0);

		x_dd0 = _mm_load_si128((__m128i *)dp);

		x_sd2 = _mm_and_si128(x_nmask, x_sd1);
		x_dd1 = _mm_and_si128(x_mask, x_dd0);
		x_dd2 = _mm_or_si128(x_sd2, x_dd1);

		_mm_store_si128((__m128i *)dp, x_dd2);
		dp += 16;
		sp += 8;
		i += 8;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < dsize; i++) {
		mlib_u8 s0;
		mlib_u16 d0, d1;
		s0 = *sp++;
		d0 = *((mlib_u16 *)dp);
		d1 = (((s0 << 8) | s0) & ~bmask) |
		    (d0 & bmask);
		*((mlib_u16 *)dp) = d1;
		dp += 2;
	}
}

/* *********************************************************** */

/*
 * insert one channel to a 2-channel image.
 * both source and destination image data are 16-byte aligned.
 * not both stride is equal to width in bytes.
 * both stride is multiple of 16.
 */

void
mlib_s_ImageChannelInsert_U8_12_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_u8 *sp, *dp;
	mlib_u8 *sl, *dl;
	mlib_s32 i, j;

	sp = sl = (mlib_u8 *)src;
	dp = dl = (mlib_u8 *)dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageChannelInsert_U8_12_A8D1X8(sp, dp, xsize, cmask);

		sp = sl = sl + slb;
		dp = dl = dl + dlb;
	}
}

/* *********************************************************** */

/*
 * insert one channel to a 2-channel image.
 * not both source and destination image data are 16-byte aligned.
 * both stride is equal to width in bytes.
 */

void
mlib_s_ImageChannelInsert_U8_12_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_u8 *sp, *dp;
	__m128i x_mask, x_nmask;
	mlib_s16 bmask = 0;
	mlib_s32 i;

	for (i = 1; i >= 0; i--) {
		if ((cmask & (1 << i)) != 0) {
			bmask += (0xFF << (8 * i));
		}
	}

	x_mask = _mm_set1_epi16(bmask);
	x_nmask = _mm_set1_epi16((~bmask));

	sp = (mlib_u8 *)src;
	dp = (mlib_u8 *)dst;

	if (((mlib_addr)dp & 1) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; (i < dsize) && ((mlib_addr)dp & 15); i++) {
		mlib_u8 s0;
		mlib_u16 d0, d1;
		s0 = *sp++;
		d0 = *((mlib_u16 *)dp);
		d1 = (((s0 << 8) | s0) & ~bmask) |
		    (d0 & bmask);
		*((mlib_u16 *)dp) = d1;
		dp += 2;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < (dsize - 15); i += 16) {
		__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_sd4;
		__m128i x_dd0, x_dd1, x_dd2, x_dd3;
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		x_dd0 = _mm_load_si128((__m128i *)dp);
		x_dd1 = _mm_load_si128((__m128i *)dp + 1);

		x_sd1 = _mm_unpacklo_epi8(x_sd0, x_sd0);
		x_sd2 = _mm_unpackhi_epi8(x_sd0, x_sd0);

		x_sd3 = _mm_and_si128(x_nmask, x_sd1);
		x_sd4 = _mm_and_si128(x_nmask, x_sd2);
		x_dd2 = _mm_and_si128(x_mask, x_dd0);
		x_dd3 = _mm_and_si128(x_mask, x_dd1);
		x_dd0 = _mm_or_si128(x_sd3, x_dd2);
		x_dd1 = _mm_or_si128(x_sd4, x_dd3);

		_mm_store_si128((__m128i *)dp, x_dd0);
		dp += 16;
		_mm_store_si128((__m128i *)dp, x_dd1);
		dp += 16;
		sp += 16;
	}

/* 8 pixels */
	if (i < (dsize - 7)) {
		__m128i x_sd0, x_sd1, x_sd2;
		__m128i x_dd0, x_dd1, x_dd2;

		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_dd0 = _mm_load_si128((__m128i *)dp);

		x_sd1 = _mm_unpacklo_epi8(x_sd0, x_sd0);
		x_sd2 = _mm_and_si128(x_nmask, x_sd1);
		x_dd1 = _mm_and_si128(x_mask, x_dd0);

		x_dd2 = _mm_or_si128(x_sd2, x_dd1);

		_mm_store_si128((__m128i *)dp, x_dd2);
		dp += 16;
		sp += 8;
		i += 8;
	}
	} else {
/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 15); i += 16) {
		__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_sd4;
		__m128i x_dd0, x_dd1, x_dd2, x_dd3;
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		x_dd0 = _mm_loadu_si128((__m128i *)dp);
		x_dd1 = _mm_loadu_si128((__m128i *)dp + 1);

		x_sd1 = _mm_unpacklo_epi8(x_sd0, x_sd0);
		x_sd2 = _mm_unpackhi_epi8(x_sd0, x_sd0);

		x_sd3 = _mm_and_si128(x_nmask, x_sd1);
		x_sd4 = _mm_and_si128(x_nmask, x_sd2);
		x_dd2 = _mm_and_si128(x_mask, x_dd0);
		x_dd3 = _mm_and_si128(x_mask, x_dd1);
		x_dd0 = _mm_or_si128(x_sd3, x_dd2);
		x_dd1 = _mm_or_si128(x_sd4, x_dd3);

		_mm_storeu_si128((__m128i *)dp, x_dd0);
		dp += 16;
		_mm_storeu_si128((__m128i *)dp, x_dd1);
		dp += 16;
		sp += 16;
	}

/* 8 pixels */
	if (i < (dsize - 7)) {
		__m128i x_sd0, x_sd1, x_sd2;
		__m128i x_dd0, x_dd1, x_dd2;

		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_dd0 = _mm_loadu_si128((__m128i *)dp);

		x_sd1 = _mm_unpacklo_epi8(x_sd0, x_sd0);
		x_sd2 = _mm_and_si128(x_nmask, x_sd1);
		x_dd1 = _mm_and_si128(x_mask, x_dd0);

		x_dd2 = _mm_or_si128(x_sd2, x_dd1);

		_mm_storeu_si128((__m128i *)dp, x_dd2);
		dp += 16;
		sp += 8;
		i += 8;
	}
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < dsize; i++) {
		mlib_u8 s0;
		mlib_u16 d0, d1;
		s0 = *sp++;
		d0 = *((mlib_u16 *)dp);
		d1 = (((s0 << 8) | s0) & ~bmask) |
		    (d0 & bmask);
		*((mlib_u16 *)dp) = d1;
		dp += 2;
	}
}

/* *********************************************************** */

/*
 * insert one channel to a 2-channel image.
 * not both source and destination image data are 16-byte aligned.
 * not both stride is equal to width in bytes.
 */

void
mlib_s_ImageChannelInsert_U8_12(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_u8 *sa, *da;
	mlib_u8 *sl, *dl;
	mlib_s32 j;

	sa = sl = (void *)src;
	da = dl = dst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < ysize; j++) {
		mlib_s_ImageChannelInsert_U8_12_D1(sa, da, xsize, cmask);
		sa = sl += slb;
		da = dl += dlb;
	}
}

/* *********************************************************** */

/*
 * insert one channel to a 4-channel image.
 * both source and destination image data are 16-byte aligned.
 * both stride is equal to width in bytes.
 */

void
mlib_s_ImageChannelInsert_U8_14_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_u8 *sp, *dp;
	__m128i x_mask, x_nmask;
	mlib_s32 bmask = 0;
	mlib_s32 i;

	for (i = 0; i <= 3; i++) {
		if ((cmask & (1 << i)) != 0) {
			bmask += (0xFF << (8 * (3 - i)));
		}
	}

	x_mask = _mm_set1_epi32(bmask);
	x_nmask = _mm_set1_epi32((~bmask));

	sp = (mlib_u8 *)src;
	dp = (mlib_u8 *)dst;

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 15); i += 16) {
		__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_sd4, x_sd5;
		__m128i x_sd6, x_sd7, x_sd8, x_sd9, x_sd10;
		__m128i x_dd0, x_dd1, x_dd2, x_dd3;
		__m128i x_dd4, x_dd5, x_dd6, x_dd7;
		__m128i x_dd8, x_dd9, x_dd10, x_dd11;

		x_sd0 = _mm_load_si128((__m128i *)sp);
		x_sd1 = _mm_unpacklo_epi8(x_sd0, x_sd0);
		x_sd2 = _mm_unpackhi_epi8(x_sd0, x_sd0);

		x_dd0 = _mm_load_si128((__m128i *)dp);
		x_dd1 = _mm_load_si128((__m128i *)dp + 1);
		x_dd2 = _mm_load_si128((__m128i *)dp + 2);
		x_dd3 = _mm_load_si128((__m128i *)dp + 3);

		x_sd3 = _mm_unpacklo_epi16(x_sd1, x_sd1);
		x_sd4 = _mm_unpackhi_epi16(x_sd1, x_sd1);
		x_sd5 = _mm_unpacklo_epi16(x_sd2, x_sd2);
		x_sd6 = _mm_unpackhi_epi16(x_sd2, x_sd2);

		x_sd7 = _mm_and_si128(x_mask, x_sd3);
		x_dd4 = _mm_and_si128(x_nmask, x_dd0);
		x_dd8 = _mm_or_si128(x_sd7, x_dd4);
		_mm_store_si128((__m128i *)dp, x_dd8);
		dp += 16;

		x_sd8 = _mm_and_si128(x_mask, x_sd4);
		x_dd5 = _mm_and_si128(x_nmask, x_dd1);
		x_dd9 = _mm_or_si128(x_sd8, x_dd5);
		_mm_store_si128((__m128i *)dp, x_dd9);
		dp += 16;

		x_sd9 = _mm_and_si128(x_mask, x_sd5);
		x_dd6 = _mm_and_si128(x_nmask, x_dd2);
		x_dd10 = _mm_or_si128(x_sd9, x_dd6);
		_mm_store_si128((__m128i *)dp, x_dd10);
		dp += 16;

		x_sd10 = _mm_and_si128(x_mask, x_sd6);
		x_dd7 = _mm_and_si128(x_nmask, x_dd3);
		x_dd11 = _mm_or_si128(x_sd10, x_dd7);
		_mm_store_si128((__m128i *)dp, x_dd11);
		dp += 16;

		sp += 16;
	}

/* 8 pixels */
	if (i < (dsize - 7)) {
		__m128i x_sd0, x_sd1, x_sd2, x_sd3;
		__m128i x_dd0, x_dd1, x_dd2, x_dd3;

		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_dd0 = _mm_load_si128((__m128i *)dp);
		x_dd1 = _mm_load_si128((__m128i *)dp + 1);

		x_sd1 = _mm_unpacklo_epi8(x_sd0, x_sd0);
		x_sd2 = _mm_unpacklo_epi16(x_sd1, x_sd1);
		x_sd3 = _mm_unpackhi_epi16(x_sd1, x_sd1);

		x_sd2 = _mm_and_si128(x_mask, x_sd2);
		x_sd3 = _mm_and_si128(x_mask, x_sd3);
		x_dd0 = _mm_and_si128(x_nmask, x_dd0);
		x_dd1 = _mm_and_si128(x_nmask, x_dd1);

		x_dd2 = _mm_or_si128(x_sd2, x_dd0);
		x_dd3 = _mm_or_si128(x_sd3, x_dd1);

		_mm_store_si128((__m128i *)dp, x_dd2);
		dp += 16;
		_mm_store_si128((__m128i *)dp, x_dd3);
		dp += 16;
		sp += 8;
		i += 8;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < dsize; i++) {
		mlib_u8 s0;
		mlib_u32 d0, d1;
		s0 = *sp++;
		d0 = *((mlib_u32 *)dp);
		d1 = ((((s0 << 24) | (s0 << 16) | s0 << 8) | s0) & bmask) |
		    (d0 & ~bmask);
		*((mlib_u32 *)dp) = d1;
		dp += 4;
	}
}

/* *********************************************************** */

/*
 * insert one channel to a 4-channel image.
 * both source and destination image data are 16-byte aligned.
 * not both stride is equal to width in bytes.
 * both stride is multiple of 16.
 */

void
mlib_s_ImageChannelInsert_U8_14_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_u8 *sp, *dp;
	mlib_u8 *sl, *dl;
	mlib_s32 i, j;

	sp = sl = (mlib_u8 *)src;
	dp = dl = (mlib_u8 *)dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageChannelInsert_U8_14_A8D1X8(sp, dp, xsize, cmask);

		sp = sl = sl + slb;
		dp = dl = dl + dlb;
	}
}

/* *********************************************************** */

/*
 * insert one channel to a 4-channel image.
 * not both source and destination image data are 16-byte aligned.
 * both stride is equal to width in bytes.
 */

void
mlib_s_ImageChannelInsert_U8_14_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_u8 *sp, *dp;
	__m128i x_mask, x_nmask;
	mlib_s32 bmask = 0;
	mlib_s32 i;

	for (i = 0; i <= 3; i++) {
		if ((cmask & (1 << i)) != 0) {
			bmask += (0xFF << (8 * (3 - i)));
		}
	}

	x_mask = _mm_set1_epi32(bmask);
	x_nmask = _mm_set1_epi32((~bmask));

	sp = (mlib_u8 *)src;
	dp = (mlib_u8 *)dst;

	if (((mlib_addr)dp & 3) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; (i < dsize) && ((mlib_addr)dp & 15); i++) {
		mlib_u8 s0;
		mlib_u32 d0, d1;
		s0 = *sp++;
		d0 = *((mlib_u32 *)dp);
		d1 = ((((s0 << 24) | (s0 << 16) | s0 << 8) | s0) & bmask) |
		    (d0 & ~bmask);
		*((mlib_u32 *)dp) = d1;
		dp += 4;
	}

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < (dsize - 15); i += 16) {
		__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_sd4, x_sd5, x_sd6;
		__m128i x_sd7, x_sd8, x_sd9, x_sd10;
		__m128i x_dd0, x_dd1, x_dd2, x_dd3;
		__m128i x_dd4, x_dd5, x_dd6, x_dd7;
		__m128i x_dd8, x_dd9, x_dd10, x_dd11;

		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		x_sd1 = _mm_unpacklo_epi8(x_sd0, x_sd0);
		x_sd2 = _mm_unpackhi_epi8(x_sd0, x_sd0);

		x_dd0 = _mm_load_si128((__m128i *)dp);
		x_dd1 = _mm_load_si128((__m128i *)dp + 1);
		x_dd2 = _mm_load_si128((__m128i *)dp + 2);
		x_dd3 = _mm_load_si128((__m128i *)dp + 3);

		x_sd3 = _mm_unpacklo_epi16(x_sd1, x_sd1);
		x_sd4 = _mm_unpackhi_epi16(x_sd1, x_sd1);
		x_sd5 = _mm_unpacklo_epi16(x_sd2, x_sd2);
		x_sd6 = _mm_unpackhi_epi16(x_sd2, x_sd2);

		x_sd7 = _mm_and_si128(x_mask, x_sd3);
		x_dd4 = _mm_and_si128(x_nmask, x_dd0);
		x_dd8 = _mm_or_si128(x_sd7, x_dd4);
		_mm_store_si128((__m128i *)dp, x_dd8);
		dp += 16;

		x_sd8 = _mm_and_si128(x_mask, x_sd4);
		x_dd5 = _mm_and_si128(x_nmask, x_dd1);
		x_dd9 = _mm_or_si128(x_sd8, x_dd5);
		_mm_store_si128((__m128i *)dp, x_dd9);
		dp += 16;

		x_sd9 = _mm_and_si128(x_mask, x_sd5);
		x_dd6 = _mm_and_si128(x_nmask, x_dd2);
		x_dd10 = _mm_or_si128(x_sd9, x_dd6);
		_mm_store_si128((__m128i *)dp, x_dd10);
		dp += 16;

		x_sd10 = _mm_and_si128(x_mask, x_sd6);
		x_dd7 = _mm_and_si128(x_nmask, x_dd3);
		x_dd11 = _mm_or_si128(x_sd10, x_dd7);
		_mm_store_si128((__m128i *)dp, x_dd11);
		dp += 16;
		sp += 16;
	}

/* 8 pixels */
	if (i < (dsize - 7)) {
		__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_sd4, x_sd5;
		__m128i x_dd0, x_dd1, x_dd2, x_dd3, x_dd4, x_dd5;

		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_dd0 = _mm_load_si128((__m128i *)dp);
		x_dd1 = _mm_load_si128((__m128i *)dp + 1);

		x_sd1 = _mm_unpacklo_epi8(x_sd0, x_sd0);
		x_sd2 = _mm_unpacklo_epi16(x_sd1, x_sd1);
		x_sd3 = _mm_unpackhi_epi16(x_sd1, x_sd1);

		x_sd4 = _mm_and_si128(x_mask, x_sd2);
		x_sd5 = _mm_and_si128(x_mask, x_sd3);

		x_dd2 = _mm_and_si128(x_nmask, x_dd0);
		x_dd3 = _mm_and_si128(x_nmask, x_dd1);

		x_dd4 = _mm_or_si128(x_sd4, x_dd2);
		x_dd5 = _mm_or_si128(x_sd5, x_dd3);

		_mm_store_si128((__m128i *)dp, x_dd4);
		dp += 16;
		_mm_store_si128((__m128i *)dp, x_dd5);
		dp += 16;
		sp += 8;
		i += 8;
	}

	} else {
/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 15); i += 16) {
		__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_sd4, x_sd5, x_sd6;
		__m128i x_sd7, x_sd8, x_sd9, x_sd10;
		__m128i x_dd0, x_dd1, x_dd2, x_dd3;
		__m128i x_dd4, x_dd5, x_dd6, x_dd7;
		__m128i x_dd8, x_dd9, x_dd10, x_dd11;

		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		x_sd1 = _mm_unpacklo_epi8(x_sd0, x_sd0);
		x_sd2 = _mm_unpackhi_epi8(x_sd0, x_sd0);

		x_dd0 = _mm_loadu_si128((__m128i *)dp);
		x_dd1 = _mm_loadu_si128((__m128i *)dp + 1);
		x_dd2 = _mm_loadu_si128((__m128i *)dp + 2);
		x_dd3 = _mm_loadu_si128((__m128i *)dp + 3);

		x_sd3 = _mm_unpacklo_epi16(x_sd1, x_sd1);
		x_sd4 = _mm_unpackhi_epi16(x_sd1, x_sd1);
		x_sd5 = _mm_unpacklo_epi16(x_sd2, x_sd2);
		x_sd6 = _mm_unpackhi_epi16(x_sd2, x_sd2);

		x_sd7 = _mm_and_si128(x_mask, x_sd3);
		x_dd4 = _mm_and_si128(x_nmask, x_dd0);
		x_dd8 = _mm_or_si128(x_sd7, x_dd4);
		_mm_storeu_si128((__m128i *)dp, x_dd8);
		dp += 16;

		x_sd8 = _mm_and_si128(x_mask, x_sd4);
		x_dd5 = _mm_and_si128(x_nmask, x_dd1);
		x_dd9 = _mm_or_si128(x_sd8, x_dd5);
		_mm_storeu_si128((__m128i *)dp, x_dd9);
		dp += 16;

		x_sd9 = _mm_and_si128(x_mask, x_sd5);
		x_dd6 = _mm_and_si128(x_nmask, x_dd2);
		x_dd10 = _mm_or_si128(x_sd9, x_dd6);
		_mm_storeu_si128((__m128i *)dp, x_dd10);
		dp += 16;

		x_sd10 = _mm_and_si128(x_mask, x_sd6);
		x_dd7 = _mm_and_si128(x_nmask, x_dd3);
		x_dd11 = _mm_or_si128(x_sd10, x_dd7);
		_mm_storeu_si128((__m128i *)dp, x_dd11);
		dp += 16;
		sp += 16;
	}

/* 8 pixels */
	if (i < (dsize - 7)) {
		__m128i x_sd0, x_sd1, x_sd2, x_sd3;
		__m128i x_dd0, x_dd1, x_dd2, x_dd3;
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_dd0 = _mm_loadu_si128((__m128i *)dp);
		x_dd1 = _mm_loadu_si128((__m128i *)dp + 1);

		x_sd1 = _mm_unpacklo_epi8(x_sd0, x_sd0);
		x_sd2 = _mm_unpacklo_epi16(x_sd1, x_sd1);
		x_sd3 = _mm_unpackhi_epi16(x_sd1, x_sd1);

		x_sd2 = _mm_and_si128(x_mask, x_sd2);
		x_sd3 = _mm_and_si128(x_mask, x_sd3);
		x_dd0 = _mm_and_si128(x_nmask, x_dd0);
		x_dd1 = _mm_and_si128(x_nmask, x_dd1);

		x_dd2 = _mm_or_si128(x_sd2, x_dd0);
		x_dd3 = _mm_or_si128(x_sd3, x_dd1);

		_mm_storeu_si128((__m128i *)dp, x_dd2);
		dp += 16;
		_mm_storeu_si128((__m128i *)dp, x_dd3);
		dp += 16;
		sp += 8;
		i += 8;
	}
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < dsize; i++) {
		mlib_u8 s0;
		mlib_u32 d0, d1;
		s0 = *sp++;
		d0 = *((mlib_u32 *)dp);
		d1 = ((((s0 << 24) | (s0 << 16) | s0 << 8) | s0) & bmask) |
		    (d0 & ~bmask);
		*((mlib_u32 *)dp) = d1;
		dp += 4;
	}
}

/* *********************************************************** */

/*
 * insert one channel to a 4-channel image.
 * not both source and destination image data are 16-byte aligned.
 * not both stride is not equal to width in bytes.
 */

void
mlib_s_ImageChannelInsert_U8_14(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_u8 *sa, *da;
	mlib_u8 *sl, *dl;
	mlib_s32 j;

	sa = sl = (void *)src;
	da = dl = dst;

	for (j = 0; j < ysize; j++) {
		mlib_s_ImageChannelInsert_U8_14_D1(sa, da, xsize, cmask);
		sa = sl += slb;
		da = dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_s_ImageChannelInsert_S16_12_A8D1X4(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_s16 *sp, *dp;
	__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_sd4;
	__m128i x_dd0, x_dd1, x_dd2, x_dd3;
	__m128i x_mask;
	mlib_s32 bmask = 0;
	mlib_s32 i;
	mlib_s16 s0;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 1; i >= 0; i--) {
		if ((cmask & (1 << i)) != 0) {
			bmask += (0xFFFF << (16 * i));
		}
	}

	x_mask = _mm_set1_epi32(bmask);

	sp = (mlib_s16 *)src;
	dp = dst;

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 7); i += 8) {
		x_sd0 = _mm_load_si128((__m128i *)sp);
		x_dd0 = _mm_load_si128((__m128i *)dp);
		x_dd1 = _mm_load_si128((__m128i *)dp + 1);

		if (cmask == 1) {
			x_sd1 = _mm_unpacklo_epi16(x_zero, x_sd0);
			x_sd2 = _mm_unpackhi_epi16(x_zero, x_sd0);
		} else {
			x_sd1 = _mm_unpacklo_epi16(x_sd0, x_zero);
			x_sd2 = _mm_unpackhi_epi16(x_sd0, x_zero);
		}

		x_dd2 = _mm_and_si128(x_mask, x_dd0);
		x_dd3 = _mm_and_si128(x_mask, x_dd1);
		x_dd0 = _mm_or_si128(x_sd1, x_dd2);
		x_dd1 = _mm_or_si128(x_sd2, x_dd3);

		_mm_store_si128((__m128i *)dp, x_dd0);
		dp += 8;
		_mm_store_si128((__m128i *)dp, x_dd1);
		dp += 8;
		sp += 8;
	}

/* 4 pixels */
	if (i < (dsize - 3)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_dd0 = _mm_load_si128((__m128i *)dp);

		x_sd1 = _mm_unpacklo_epi16(x_sd0, x_sd0);
		x_sd2 = _mm_andnot_si128(x_mask, x_sd1);
		x_dd1 = _mm_and_si128(x_mask, x_dd0);

		x_dd2 = _mm_or_si128(x_sd2, x_dd1);

		_mm_store_si128((__m128i *)dp, x_dd2);
		dp += 8;
		sp += 4;
		i += 4;
	}

/* offset for dst */
	mlib_s32 inc;
	switch (cmask) {
		case 1:
			inc = 1;
			break;
		case 2:
			inc = 0;
			break;
	}

	dp = dp + inc;
	for (; i < dsize; i++) {
		s0 = *sp++;
		dp[0] = s0;
		dp += 2;
	}
}

/* *********************************************************** */

void
mlib_s_ImageChannelInsert_S16_12_A8D2X4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_s16 *da, *dl;
	mlib_s16 *sp, *sl;
	mlib_s16 sd;
	mlib_s32 i, j;

	sp = sl = (void *)src;
	da = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageChannelInsert_S16_12_A8D1X4(sp, da, xsize, cmask);

		sp = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		da = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_s_ImageChannelInsert_S16_12_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_s16 *sp, *dp;
	__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_sd4;
	__m128i x_dd0, x_dd1, x_dd2, x_dd3;
	__m128i x_mask;
	mlib_s32 bmask = 0;
	mlib_s32 i;
	mlib_s16 s0;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 1; i >= 0; i--) {
		if ((cmask & (1 << i)) != 0) {
			bmask += (0xFFFF << (16 * i));
		}
	}

	x_mask = _mm_set1_epi32(bmask);

	sp = (mlib_s16 *)src;
	dp = dst;

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 7); i += 8) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		x_dd0 = _mm_loadu_si128((__m128i *)dp);
		x_dd1 = _mm_loadu_si128((__m128i *)dp + 1);

		if (cmask == 1) {
			x_sd1 = _mm_unpacklo_epi16(x_zero, x_sd0);
			x_sd2 = _mm_unpackhi_epi16(x_zero, x_sd0);
		} else {
			x_sd1 = _mm_unpacklo_epi16(x_sd0, x_zero);
			x_sd2 = _mm_unpackhi_epi16(x_sd0, x_zero);
		}
		x_dd2 = _mm_and_si128(x_mask, x_dd0);
		x_dd3 = _mm_and_si128(x_mask, x_dd1);
		x_dd0 = _mm_or_si128(x_sd1, x_dd2);
		x_dd1 = _mm_or_si128(x_sd2, x_dd3);

		_mm_storeu_si128((__m128i *)dp, x_dd0);
		dp += 8;
		_mm_storeu_si128((__m128i *)dp, x_dd1);
		dp += 8;
		sp += 8;
	}

/* 4 pixels */
	if (i < (dsize - 3)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_dd0 = _mm_loadu_si128((__m128i *)dp);

		x_sd1 = _mm_unpacklo_epi16(x_sd0, x_sd0);
		x_sd2 = _mm_andnot_si128(x_mask, x_sd1);
		x_dd1 = _mm_and_si128(x_mask, x_dd0);

		x_dd2 = _mm_or_si128(x_sd2, x_dd1);

		_mm_storeu_si128((__m128i *)dp, x_dd2);
		dp += 8;
		sp += 4;
		i += 4;
	}

/* offset for dst */
	mlib_s32 inc;
	switch (cmask) {
		case 1:
			inc = 1;
			break;
		case 2:
			inc = 0;
			break;
	}

	dp = dp + inc;
	for (; i < dsize; i++) {
		s0 = *sp++;
		dp[0] = s0;
		dp += 2;
	}
}

/* *********************************************************** */

void
mlib_s_ImageChannelInsert_S16_12(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_s16 *sa, *da;
	mlib_s16 *sl, *dl;
	mlib_s32 j;

	sa = sl = (void *)src;
	da = dl = dst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < ysize; j++) {
		mlib_s_ImageChannelInsert_S16_12_D1(sa, da, xsize, cmask);
		sa = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		da = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */
