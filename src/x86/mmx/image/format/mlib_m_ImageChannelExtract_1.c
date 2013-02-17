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

#pragma ident	"@(#)mlib_m_ImageChannelExtract_1.c	9.5	07/11/05 SMI"

/*
 * FUNCTIONS
 *  mlib_m_ImageChannelExtract_U8_21_A8D1X8
 *  mlib_m_ImageChannelExtract_U8_21_A8D2X8
 *  mlib_m_ImageChannelExtract_U8_21_D1
 *  mlib_m_ImageChannelExtract_U8_21
 *  mlib_m_ImageChannelExtract_U8_31_A8D1X8
 *  mlib_m_ImageChannelExtract_U8_31_A8D2X8
 *  mlib_m_ImageChannelExtract_U8_31_D1
 *  mlib_m_ImageChannelExtract_U8_31
 *  mlib_m_ImageChannelExtract_U8_41_A8D1X8
 *  mlib_m_ImageChannelExtract_U8_41_A8D2X8
 *  mlib_m_ImageChannelExtract_U8_41_D1
 *  mlib_m_ImageChannelExtract_U8_41
 *  mlib_m_ImageChannelExtract_S16_21_A8D1X4
 *  mlib_m_ImageChannelExtract_S16_21_A8D2X4
 *  mlib_m_ImageChannelExtract_S16_21_D1
 *  mlib_m_ImageChannelExtract_S16_21
 *  mlib_m_ImageChannelExtract_S16_31_A8D1X4
 *  mlib_m_ImageChannelExtract_S16_31_A8D2X4
 *  mlib_m_ImageChannelExtract_S16_31_D1
 *  mlib_m_ImageChannelExtract_S16_31
 *  mlib_m_ImageChannelExtract_S16_41_A8D1X4
 *  mlib_m_ImageChannelExtract_S16_41_A8D2X4
 *  mlib_m_ImageChannelExtract_S16_41_D1
 *  mlib_m_ImageChannelExtract_S16_41
 *
 * ARGUMENT
 *      src    pointer to source image data
 *      dst    pointer to destination image data
 *      slb    source image line stride in bytes
 *      dlb    destination image line stride in bytes
 *      dsize  image data size in pixels
 *      xsize  image width in pixels
 *      ysize  image height in lines
 *      cmask  channel mask
 *
 * DESCRIPTION
 *      Extract the one selected channel of the source image into the
 *      1-channel destination image.
 *
 * NOTE
 *      These functions are separated from mlib_ImageChannelExtract.c
 *      for loop unrolling and structure clarity.
 */

/* *********************************************************** */

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <mmintrin.h>
#endif


#include <mlib_mmx_utils.h>
#include <mlib_image.h>

/* *********************************************************** */

#define	CHANNEL_EXTRACT_U8_21R(sd0, sd1, dd)                            \
	dd =                                                            \
	_mm_packs_pi16(_mm_srai_pi16(sd0, 8), _mm_srai_pi16(sd1, 8));

/* *********************************************************** */

#define	CHANNEL_EXTRACT_U8_21L(sd0, sd1, dd)                       \
	dd =                                                       \
	_mm_packs_pu16(_mm_and_si64(sd0, _mm_set1_pi16(0x00ff)),   \
	    _mm_and_si64(sd1, _mm_set1_pi16(0x00ff)));

/* *********************************************************** */

/*
 * extract one channel from a 2-channel image.
 * both source and destination image data are 8-byte aligned.
 * xsize is multiple of 8.
 *
 */

void
mlib_m_ImageChannelExtract_U8_21_A8D1X8(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	__m64 *sp, *dp;
	mlib_s32 j, n8;

	sp = (__m64 *) src;
	dp = (__m64 *) dst;

	n8 = dsize >> 3;

	if (cmask == 2) {
		for (j = 0; j < n8; j++) {
			CHANNEL_EXTRACT_U8_21L(sp[2 * j + 0], sp[2 * j + 1],
			    dp[j]);
		}
	} else
		for (j = 0; j < n8; j++) {
			CHANNEL_EXTRACT_U8_21R(sp[2 * j + 0], sp[2 * j + 1],
			    dp[j]);
		}

	_mm_empty();
}

/* *********************************************************** */

/*
 * extract one channel from a 2-channel image.
 * both source and destination image data are 8-byte aligned.
 * xsize is multiple of 8.
 *
 */

void
mlib_m_ImageChannelExtract_U8_21_A8D2X8(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	__m64 *sp, *dp;
	__m64 *sl, *dl;
	mlib_s32 i, j, n8;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

	n8 = xsize >> 3;

	if (cmask == 2) {
		for (j = 0; j < ysize; j++) {
			for (i = 0; i < n8; i++) {
				CHANNEL_EXTRACT_U8_21L(sp[0], sp[1], dp[0]);
				sp += 2;
				dp += 1;
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else
		for (j = 0; j < ysize; j++) {
			for (i = 0; i < n8; i++) {
				CHANNEL_EXTRACT_U8_21R(sp[0], sp[1], dp[0]);
				sp += 2;
				dp += 1;
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}

	_mm_empty();
}

/* *********************************************************** */

/*
 * extract one channel from a 2-channel image.
 */

void
mlib_m_ImageChannelExtract_U8_21_D1(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	__m64 *sp, *dp;
	__m64 dd;
	mlib_s32 j, n1, n8, emask;

	sp = (__m64 *) src;
	dp = (__m64 *) dst;

	n8 = dsize >> 3;
	n1 = dsize & 7;

	if (cmask == 2) {
		for (j = 0; j < n8; j++) {
			CHANNEL_EXTRACT_U8_21L(sp[0], sp[1], dp[0]);
			sp += 2;
			dp += 1;
		}

		if (n1 > 0) {
			emask = (0xFF << (8 - n1));
			CHANNEL_EXTRACT_U8_21L(sp[0], sp[1], dd);
			mmx_pst_8(dd, dp, emask);
		}
	} else {
		for (j = 0; j < n8; j++) {
			CHANNEL_EXTRACT_U8_21R(sp[0], sp[1], dp[0]);
			sp += 2;
			dp += 1;
		}

		if (n1 > 0) {
			emask = (0xFF << (8 - n1));
			CHANNEL_EXTRACT_U8_21R(sp[0], sp[1], dd);
			mmx_pst_8(dd, dp, emask);
		}
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_U8_21(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	__m64 *sp, *dp;
	__m64 *sl, *dl;
	__m64 dd;
	mlib_s32 j, n1, nu, emask;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

	if (cmask == 2) {
		for (j = 0; j < ysize; j++) {
			n1 = xsize;
			nu = ((8 - (mlib_addr)sp) & 7);
			if ((nu > 0) && (nu <= n1)) {
				emask = 0xFF << (8 - nu);
				CHANNEL_EXTRACT_U8_21L(sp[0], sp[1], dd);
				mmx_pst_8(dd, dp, emask);
				n1 -= nu;
				dp = (__m64 *) ((mlib_u8 *)dp + nu);
				sp = (__m64 *) ((mlib_u8 *)sp + (nu << 1));
			}

			for (; n1 > 7; n1 -= 8) {
				CHANNEL_EXTRACT_U8_21L(sp[0], sp[1], dp[0]);
				sp += 2;
				dp += 1;
			}

			if (n1 > 0) {
				emask = (0xFF << (8 - n1));
				CHANNEL_EXTRACT_U8_21L(sp[0], sp[1], dd);
				mmx_pst_8(dd, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else
		for (j = 0; j < ysize; j++) {
			n1 = xsize;
			nu = ((8 - (mlib_addr)sp) & 7);
			if ((nu > 0) && (nu <= n1)) {
				emask = 0xFF << (8 - nu);
				CHANNEL_EXTRACT_U8_21R(sp[0], sp[1], dd);
				mmx_pst_8(dd, dp, emask);
				n1 -= nu;
				dp = (__m64 *) ((mlib_u8 *)dl + nu);
				sp = (__m64 *) ((mlib_u8 *)sl + (nu << 1));
			}

			for (; n1 > 7; n1 -= 8) {
				CHANNEL_EXTRACT_U8_21R(sp[0], sp[1], dp[0]);
				sp += 2;
				dp += 1;
			}

			if (n1 > 0) {
				emask = (0xFF << (8 - n1));
				CHANNEL_EXTRACT_U8_21R(sp[0], sp[1], dd);
				mmx_pst_8(dd, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}

	_mm_empty();
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_U8_31R(sd0, sd1, sd2, dd)                      \
	{                                                              \
	    __m64 sda =                                                \
		_mm_unpacklo_pi8(sd0, _mm_unpackhi_pi32(sd1, sd1));    \
	    __m64 sdb =                                                \
		_mm_unpackhi_pi8(sd0, _mm_unpacklo_pi32(sd2, sd2));    \
	    __m64 sdc =                                                \
		_mm_unpacklo_pi8(sd1, _mm_unpackhi_pi32(sd2, sd2));    \
	    __m64 sdd =                                                \
		_mm_unpackhi_pi8(sda, _mm_unpacklo_pi32(sdc, sdc));    \
	    __m64 sde =                                                \
		_mm_unpacklo_pi8(sdb, _mm_unpackhi_pi32(sdc, sdc));    \
	    dd = _mm_unpacklo_pi8(sdd, _mm_unpackhi_pi32(sde, sde));   \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_U8_31L(sd0, sd1, sd2, dd)                      \
	{                                                              \
	    __m64 sda =                                                \
		_mm_unpacklo_pi8(sd0, _mm_unpackhi_pi32(sd1, sd1));    \
	    __m64 sdb =                                                \
		_mm_unpackhi_pi8(sd0, _mm_unpacklo_pi32(sd2, sd2));    \
	    __m64 sdc =                                                \
		_mm_unpacklo_pi8(sd1, _mm_unpackhi_pi32(sd2, sd2));    \
	    __m64 sdd =                                                \
		_mm_unpacklo_pi8(sda, _mm_unpackhi_pi32(sdb, sdb));    \
	    __m64 sde =                                                \
		_mm_unpackhi_pi8(sda, _mm_unpacklo_pi32(sdc, sdc));    \
	    dd = _mm_unpacklo_pi8(sdd, _mm_unpackhi_pi32(sde, sde));   \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_U8_31M(sd0, sd1, sd2, dd)                      \
	{                                                              \
	    __m64 sda =                                                \
		_mm_unpacklo_pi8(sd0, _mm_unpackhi_pi32(sd1, sd1));    \
	    __m64 sdb =                                                \
		_mm_unpackhi_pi8(sd0, _mm_unpacklo_pi32(sd2, sd2));    \
	    __m64 sdc =                                                \
		_mm_unpacklo_pi8(sd1, _mm_unpackhi_pi32(sd2, sd2));    \
	    __m64 sdd =                                                \
		_mm_unpacklo_pi8(sda, _mm_unpackhi_pi32(sdb, sdb));    \
	    __m64 sde =                                                \
		_mm_unpacklo_pi8(sdb, _mm_unpackhi_pi32(sdc, sdc));    \
	    dd = _mm_unpackhi_pi8(sdd, _mm_unpacklo_pi32(sde, sde));   \
	}

/* *********************************************************** */

/*
 * extract one channel from a 3-channel image.
 * both source and destination image data are 8-byte aligned.
 * xsize is multiple of 8.
 *
 */

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_U8_31_A8D1X8(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	__m64 *sp, *dp;
	mlib_s32 n8;

	sp = (__m64 *) src;
	dp = (__m64 *) dst;

	if (cmask == 4) {
		for (n8 = dsize; n8 > 7; n8 -= 8) {
			CHANNEL_EXTRACT_U8_31L(sp[0], sp[1], sp[2], dp[0]);
			sp += 3;
			dp += 1;
		}
	} else if (cmask == 2) {
		for (n8 = dsize; n8 > 7; n8 -= 8) {
			CHANNEL_EXTRACT_U8_31M(sp[0], sp[1], sp[2], dp[0]);
			sp += 3;
			dp += 1;
		}
	} else
		for (n8 = dsize; n8 > 7; n8 -= 8) {
			CHANNEL_EXTRACT_U8_31R(sp[0], sp[1], sp[2], dp[0]);
			sp += 3;
			dp += 1;
		}

	_mm_empty();
}

/* *********************************************************** */

/*
 * extract one channel from a 3-channel image.
 * both source and destination image data are 8-byte aligned.
 * xsize is multiple of 8.
 *
 */

void
mlib_m_ImageChannelExtract_U8_31_A8D2X8(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	__m64 *sp, *dp;
	__m64 *sl, *dl;
	mlib_s32 j, n8;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

	if (cmask == 4) {
		for (j = 0; j < ysize; j++) {
			for (n8 = xsize; n8 > 7; n8 -= 8) {
				CHANNEL_EXTRACT_U8_31L(sp[0], sp[1], sp[2],
				    dp[0]);
				sp += 3;
				dp += 1;
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 2) {
		for (j = 0; j < ysize; j++) {
			for (n8 = xsize; n8 > 7; n8 -= 8) {
				CHANNEL_EXTRACT_U8_31M(sp[0], sp[1], sp[2],
				    dp[0]);
				sp += 3;
				dp += 1;
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else
		for (j = 0; j < ysize; j++) {
			for (n8 = xsize; n8 > 7; n8 -= 8) {
				CHANNEL_EXTRACT_U8_31R(sp[0], sp[1], sp[2],
				    dp[0]);
				sp += 3;
				dp += 1;
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}

	_mm_empty();
}

/* *********************************************************** */

/*
 * extract one channel from a 3-channel image.
 */

void
mlib_m_ImageChannelExtract_U8_31_D1(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	__m64 *sp, *dp;
	__m64 dd;
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;

	sp = (__m64 *) src;
	dp = (__m64 *) dst;

	if (cmask == 4) {
		for (n1 = dsize; n1 > 7; n1 -= 8) {
			CHANNEL_EXTRACT_U8_31L(sp[0], sp[1], sp[2], dp[0]);
			sp += 3;
			dp += 1;
		}

		if (n1 > 0) {
			emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
			CHANNEL_EXTRACT_U8_31L(sp[0], sp[1], sp[2], dd);
			mmx_pst_8(dd, dp, emask);
		}
	} else if (cmask == 2) {
		for (n1 = dsize; n1 > 7; n1 -= 8) {
			CHANNEL_EXTRACT_U8_31M(sp[0], sp[1], sp[2], dp[0]);
			sp += 3;
			dp += 1;
		}

		if (n1 > 0) {
			emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
			CHANNEL_EXTRACT_U8_31M(sp[0], sp[1], sp[2], dd);
			mmx_pst_8(dd, dp, emask);
		}
	} else {
		for (n1 = dsize; n1 > 7; n1 -= 8) {
			CHANNEL_EXTRACT_U8_31R(sp[0], sp[1], sp[2], dp[0]);
			sp += 3;
			dp += 1;
		}

		if (n1 > 0) {
			emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
			CHANNEL_EXTRACT_U8_31R(sp[0], sp[1], sp[2], dd);
			mmx_pst_8(dd, dp, emask);
		}
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_U8_31(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	__m64 *sp, *dp;
	__m64 *sl, *dl;
	__m64 dd;
	mlib_s32 j, n1, nu, emask;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

	if (cmask == 4) {
		for (j = 0; j < ysize; j++) {
			n1 = xsize;
			nu = ((8 - (mlib_addr)sp) & 7);
			if ((nu > 0) && (nu <= n1)) {
				emask = 0xFF << (8 - nu);
				CHANNEL_EXTRACT_U8_31L(sp[0], sp[1], sp[2], dd);
				mmx_pst_8(dd, dp, emask);
				n1 -= nu;
				dp = (__m64 *) ((mlib_u8 *)dp + nu);
				sp = (__m64 *) ((mlib_u8 *)sp + (nu * 3));
			}

			for (; n1 > 7; n1 -= 8) {
				CHANNEL_EXTRACT_U8_31L(sp[0], sp[1], sp[2],
				    dp[0]);
				sp += 3;
				dp += 1;
			}

			if (n1 > 0) {
				emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
				CHANNEL_EXTRACT_U8_31L(sp[0], sp[1], sp[2], dd);
				mmx_pst_8(dd, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 2) {
		for (j = 0; j < ysize; j++) {
			n1 = xsize;
			nu = ((8 - (mlib_addr)sp) & 7);
			if ((nu > 0) && (nu <= n1)) {
				emask = 0xFF << (8 - nu);
				CHANNEL_EXTRACT_U8_31M(sp[0], sp[1], sp[2], dd);
				mmx_pst_8(dd, dp, emask);
				n1 -= nu;
				dp = (__m64 *) ((mlib_u8 *)dp + nu);
				sp = (__m64 *) ((mlib_u8 *)sp + (nu * 3));
			}

			for (; n1 > 7; n1 -= 8) {
				CHANNEL_EXTRACT_U8_31M(sp[0], sp[1], sp[2],
				    dp[0]);
				sp += 3;
				dp += 1;
			}

			if (n1 > 0) {
				emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
				CHANNEL_EXTRACT_U8_31M(sp[0], sp[1], sp[2], dd);
				mmx_pst_8(dd, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else
		for (j = 0; j < ysize; j++) {
			n1 = xsize;
			nu = ((8 - (mlib_addr)sp) & 7);
			if ((nu > 0) && (nu <= n1)) {
				emask = 0xFF << (8 - nu);
				CHANNEL_EXTRACT_U8_31R(sp[0], sp[1], sp[2], dd);
				mmx_pst_8(dd, dp, emask);
				n1 -= nu;
				dp = (__m64 *) ((mlib_u8 *)dp + nu);
				sp = (__m64 *) ((mlib_u8 *)sp + (nu * 3));
			}

			for (; n1 > 7; n1 -= 8) {
				CHANNEL_EXTRACT_U8_31R(sp[0], sp[1], sp[2],
				    dp[0]);
				sp += 3;
				dp += 1;
			}

			if (n1 > 0) {
				emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
				CHANNEL_EXTRACT_U8_31R(sp[0], sp[1], sp[2], dd);
				mmx_pst_8(dd, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}

	_mm_empty();
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_U8_41L(sd0, sd1, sd2, sd3, dd)          \
	{                                                       \
	    __m64 sda = _mm_unpacklo_pi8(sd0, sd2);             \
	    __m64 sdb = _mm_unpackhi_pi8(sd0, sd2);             \
	    __m64 sdc = _mm_unpacklo_pi8(sd1, sd3);             \
	    __m64 sdd = _mm_unpackhi_pi8(sd1, sd3);             \
	    __m64 sde = _mm_unpacklo_pi8(sda, sdc);             \
	    __m64 sdf = _mm_unpacklo_pi8(sdb, sdd);             \
	                                                        \
	    dd = _mm_unpacklo_pi8(sde, sdf);                    \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_U8_41ML(sd0, sd1, sd2, sd3, dd)         \
	{                                                       \
	    __m64 sda = _mm_unpacklo_pi8(sd0, sd2);             \
	    __m64 sdb = _mm_unpackhi_pi8(sd0, sd2);             \
	    __m64 sdc = _mm_unpacklo_pi8(sd1, sd3);             \
	    __m64 sdd = _mm_unpackhi_pi8(sd1, sd3);             \
	    __m64 sde = _mm_unpacklo_pi8(sda, sdc);             \
	    __m64 sdf = _mm_unpacklo_pi8(sdb, sdd);             \
	                                                        \
	    dd = _mm_unpackhi_pi8(sde, sdf);                    \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_U8_41MR(sd0, sd1, sd2, sd3, dd)         \
	{                                                       \
	    __m64 sda = _mm_unpacklo_pi8(sd0, sd2);             \
	    __m64 sdb = _mm_unpackhi_pi8(sd0, sd2);             \
	    __m64 sdc = _mm_unpacklo_pi8(sd1, sd3);             \
	    __m64 sdd = _mm_unpackhi_pi8(sd1, sd3);             \
	    __m64 sde = _mm_unpackhi_pi8(sda, sdc);             \
	    __m64 sdf = _mm_unpackhi_pi8(sdb, sdd);             \
	                                                        \
	    dd = _mm_unpacklo_pi8(sde, sdf);                    \
	}

/* *********************************************************** */
#define	CHANNEL_EXTRACT_U8_41R(sd0, sd1, sd2, sd3, dd)          \
	{                                                       \
	    __m64 sda = _mm_unpacklo_pi8(sd0, sd2);             \
	    __m64 sdb = _mm_unpackhi_pi8(sd0, sd2);             \
	    __m64 sdc = _mm_unpacklo_pi8(sd1, sd3);             \
	    __m64 sdd = _mm_unpackhi_pi8(sd1, sd3);             \
	    __m64 sde = _mm_unpackhi_pi8(sda, sdc);             \
	    __m64 sdf = _mm_unpackhi_pi8(sdb, sdd);             \
	                                                        \
	    dd = _mm_unpackhi_pi8(sde, sdf);                    \
	}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_U8_41_A8D1X8(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	__m64 *sp, *dp;
	mlib_s32 i, n8;

	n8 = dsize >> 3;

	sp = (__m64 *) src;
	dp = (__m64 *) dst;

	if (cmask == 8) {
		for (i = 0; i < n8; i++) {
			CHANNEL_EXTRACT_U8_41L(sp[0], sp[1], sp[2], sp[3],
			    dp[0]);
			sp += 4;
			dp += 1;
		}
	} else if (cmask == 4) {
		for (i = 0; i < n8; i++) {
			CHANNEL_EXTRACT_U8_41ML(sp[0], sp[1], sp[2], sp[3],
			    dp[0]);
			sp += 4;
			dp += 1;
		}
	} else if (cmask == 2) {
		for (i = 0; i < n8; i++) {
			CHANNEL_EXTRACT_U8_41MR(sp[0], sp[1], sp[2], sp[3],
			    dp[0]);
			sp += 4;
			dp += 1;
		}
	} else
		for (i = 0; i < n8; i++) {
			CHANNEL_EXTRACT_U8_41R(sp[0], sp[1], sp[2], sp[3],
			    dp[0]);
			sp += 4;
			dp += 1;
		}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_U8_41_A8D2X8(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	__m64 *sp, *dp;
	__m64 *sl, *dl;
	mlib_s32 i, j, n8;

	n8 = xsize >> 3;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

	if (cmask == 8) {
		for (j = 0; j < ysize; j++) {
			for (i = 0; i < n8; i++) {
				CHANNEL_EXTRACT_U8_41L(sp[0], sp[1], sp[2],
				    sp[3], dp[0]);
				sp += 4;
				dp += 1;
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 4) {
		for (j = 0; j < ysize; j++) {
			for (i = 0; i < n8; i++) {
				CHANNEL_EXTRACT_U8_41ML(sp[0], sp[1], sp[2],
				    sp[3], dp[0]);
				sp += 4;
				dp += 1;
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 2) {
		for (j = 0; j < ysize; j++) {
			for (i = 0; i < n8; i++) {
				CHANNEL_EXTRACT_U8_41MR(sp[0], sp[1], sp[2],
				    sp[3], dp[0]);
				sp += 4;
				dp += 1;
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else
		for (j = 0; j < ysize; j++) {
			for (i = 0; i < n8; i++) {
				CHANNEL_EXTRACT_U8_41R(sp[0], sp[1], sp[2],
				    sp[3], dp[0]);
				sp += 4;
				dp += 1;
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}

	_mm_empty();
}

/* *********************************************************** */
void
mlib_m_ImageChannelExtract_U8_41_D1(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	__m64 *sp, *dp;
	__m64 dd;
	mlib_s32 emask;
	mlib_s32 i, n8, n1;

	n8 = dsize >> 3;
	n1 = dsize & 7;

	sp = (__m64 *) src;
	dp = (__m64 *) dst;

	if (cmask == 8) {
		for (i = 0; i < n8; i++) {
			CHANNEL_EXTRACT_U8_41L(sp[0], sp[1], sp[2], sp[3],
			    dp[0]);
			sp += 4;
			dp += 1;
		}

		if (n1 > 0) {
			emask = (0xFF << (8 - n1));
			CHANNEL_EXTRACT_U8_41L(sp[0], sp[1], sp[2], sp[3], dd);
			mmx_pst_8(dd, dp, emask);
		}
	} else if (cmask == 4) {
		for (i = 0; i < n8; i++) {
			CHANNEL_EXTRACT_U8_41ML(sp[0], sp[1], sp[2], sp[3],
			    dp[0]);
			sp += 4;
			dp += 1;
		}

		if (n1 > 0) {
			emask = (0xFF << (8 - n1));
			CHANNEL_EXTRACT_U8_41ML(sp[0], sp[1], sp[2], sp[3], dd);
			mmx_pst_8(dd, dp, emask);
		}
	} else if (cmask == 2) {
		for (i = 0; i < n8; i++) {
			CHANNEL_EXTRACT_U8_41MR(sp[0], sp[1], sp[2], sp[3],
			    dp[0]);
			sp += 4;
			dp += 1;
		}

		if (n1 > 0) {
			emask = (0xFF << (8 - n1));
			CHANNEL_EXTRACT_U8_41MR(sp[0], sp[1], sp[2], sp[3], dd);
			mmx_pst_8(dd, dp, emask);
		}
	} else {
		for (i = 0; i < n8; i++) {
			CHANNEL_EXTRACT_U8_41R(sp[0], sp[1], sp[2], sp[3],
			    dp[0]);
			sp += 4;
			dp += 1;
		}

		if (n1 > 0) {
			emask = (0xFF << (8 - n1));
			CHANNEL_EXTRACT_U8_41R(sp[0], sp[1], sp[2], sp[3], dd);
			mmx_pst_8(dd, dp, emask);
		}
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_U8_41(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	__m64 *sp, *dp;
	__m64 *sl, *dl;
	__m64 dd;
	mlib_s32 j, n1, nu, emask;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

	if (cmask == 8) {
		for (j = 0; j < ysize; j++) {
			n1 = xsize;
			nu = ((8 - (mlib_addr)sp) & 7);
			if ((nu > 0) && (nu <= n1)) {
				emask = 0xFF << (8 - nu);
				CHANNEL_EXTRACT_U8_41L(sp[0], sp[1], sp[2],
				    sp[3], dd);
				mmx_pst_8(dd, dp, emask);
				n1 -= nu;
				dp = (__m64 *) ((mlib_u8 *)dp + nu);
				sp = (__m64 *) ((mlib_u8 *)sp + (nu << 2));
			}

			for (; n1 > 7; n1 -= 8) {
				CHANNEL_EXTRACT_U8_41L(sp[0], sp[1], sp[2],
				    sp[3], dp[0]);
				sp += 4;
				dp += 1;
			}

			if (n1 > 0) {
				emask = (0xFF << (8 - n1));
				CHANNEL_EXTRACT_U8_41L(sp[0], sp[1], sp[2],
				    sp[3], dd);
				mmx_pst_8(dd, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 4) {
		for (j = 0; j < ysize; j++) {
			n1 = xsize;
			nu = ((8 - (mlib_addr)sp) & 7);
			if ((nu > 0) && (nu <= n1)) {
				emask = 0xFF << (8 - nu);
				CHANNEL_EXTRACT_U8_41ML(sp[0], sp[1], sp[2],
				    sp[3], dd);
				mmx_pst_8(dd, dp, emask);
				n1 -= nu;
				dp = (__m64 *) ((mlib_u8 *)dp + nu);
				sp = (__m64 *) ((mlib_u8 *)sp + (nu << 2));
			}

			for (; n1 > 7; n1 -= 8) {
				CHANNEL_EXTRACT_U8_41ML(sp[0], sp[1], sp[2],
				    sp[3], dp[0]);
				sp += 4;
				dp += 1;
			}

			if (n1 > 0) {
				emask = (0xFF << (8 - n1));
				CHANNEL_EXTRACT_U8_41ML(sp[0], sp[1], sp[2],
				    sp[3], dd);
				mmx_pst_8(dd, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 2) {
		for (j = 0; j < ysize; j++) {
			n1 = xsize;
			nu = ((8 - (mlib_addr)sp) & 7);
			if ((nu > 0) && (nu <= n1)) {
				emask = 0xFF << (8 - nu);
				CHANNEL_EXTRACT_U8_41MR(sp[0], sp[1], sp[2],
				    sp[3], dd);
				mmx_pst_8(dd, dp, emask);
				n1 -= nu;
				dp = (__m64 *) ((mlib_u8 *)dp + nu);
				sp = (__m64 *) ((mlib_u8 *)sp + (nu << 2));
			}

			for (; n1 > 7; n1 -= 8) {
				CHANNEL_EXTRACT_U8_41MR(sp[0], sp[1], sp[2],
				    sp[3], dp[0]);
				sp += 4;
				dp += 1;
			}

			if (n1 > 0) {
				emask = (0xFF << (8 - n1));
				CHANNEL_EXTRACT_U8_41MR(sp[0], sp[1], sp[2],
				    sp[3], dd);
				mmx_pst_8(dd, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else
		for (j = 0; j < ysize; j++) {
			n1 = xsize;
			nu = ((8 - (mlib_addr)sp) & 7);
			if ((nu > 0) && (nu <= n1)) {
				emask = 0xFF << (8 - nu);
				CHANNEL_EXTRACT_U8_41R(sp[0], sp[1], sp[2],
				    sp[3], dd);
				mmx_pst_8(dd, dp, emask);
				n1 -= nu;
				dp = (__m64 *) ((mlib_u8 *)dp + nu);
				sp = (__m64 *) ((mlib_u8 *)sp + (nu << 2));
			}

			for (; n1 > 7; n1 -= 8) {
				CHANNEL_EXTRACT_U8_41R(sp[0], sp[1], sp[2],
				    sp[3], dp[0]);
				sp += 4;
				dp += 1;
			}

			if (n1 > 0) {
				emask = (0xFF << (8 - n1));
				CHANNEL_EXTRACT_U8_41R(sp[0], sp[1], sp[2],
				    sp[3], dd);
				mmx_pst_8(dd, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}

	_mm_empty();
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_S16_21L(sd0, sd1, dd)                   \
	{                                                       \
	    __m64 sda = _mm_unpacklo_pi16(sd0, sd1);            \
	    __m64 sdb = _mm_unpackhi_pi16(sd0, sd1);            \
	                                                        \
	    dd = _mm_unpacklo_pi16(sda, sdb);                   \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_S16_21R(sd0, sd1, dd)                   \
	{                                                       \
	    __m64 sda = _mm_unpacklo_pi16(sd0, sd1);            \
	    __m64 sdb = _mm_unpackhi_pi16(sd0, sd1);            \
	                                                        \
	    dd = _mm_unpackhi_pi16(sda, sdb);                   \
	}

/* *********************************************************** */

/*
 * extract one channel from a 2-channel image.
 * both source and destination image data are 8-byte aligned.
 * xsize is multiple of 8.
 *
 */

void
mlib_m_ImageChannelExtract_S16_21_A8D1X4(
    mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	__m64 *sp, *dp;
	mlib_s32 j, n4;

	sp = (__m64 *) src;
	dp = (__m64 *) dst;

	n4 = dsize >> 2;

	if (cmask == 2) {
		for (j = 0; j < n4; j++) {
			CHANNEL_EXTRACT_S16_21L(sp[0], sp[1], dp[0]);
			sp += 2;
			dp += 1;
		}
	} else
		for (j = 0; j < n4; j++) {
			CHANNEL_EXTRACT_S16_21R(sp[0], sp[1], dp[0]);
			sp += 2;
			dp += 1;
		}

	_mm_empty();
}

/* *********************************************************** */

/*
 * extract one channel from a 2-channel image.
 * both source and destination image data are 8-byte aligned.
 * xsize is multiple of 8.
 *
 */

void
mlib_m_ImageChannelExtract_S16_21_A8D2X4(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	__m64 *sp, *dp;
	__m64 *sl, *dl;
	mlib_s32 i, j, n4;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

	n4 = xsize >> 2;

	if (cmask == 2) {
		for (j = 0; j < ysize; j++) {
			for (i = 0; i < n4; i++) {
				CHANNEL_EXTRACT_S16_21L(sp[0], sp[1], dp[0]);
				sp += 2;
				dp += 1;
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else
		for (j = 0; j < ysize; j++) {
			for (i = 0; i < n4; i++) {
				CHANNEL_EXTRACT_S16_21R(sp[0], sp[1], dp[0]);
				sp += 2;
				dp += 1;
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}

	_mm_empty();
}

/* *********************************************************** */

/*
 * extract one channel from a 2-channel image.
 */

void
mlib_m_ImageChannelExtract_S16_21_D1(
    mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	__m64 *sp, *dp;
	__m64 dd;
	mlib_s32 j, n1, n4, emask;

	sp = (__m64 *) src;
	dp = (__m64 *) dst;

	n4 = dsize >> 2;
	n1 = dsize & 3;

	if (cmask == 2) {
		for (j = 0; j < n4; j++) {
			CHANNEL_EXTRACT_S16_21L(sp[0], sp[1], dp[0]);
			sp += 2;
			dp += 1;
		}

		if (n1 > 0) {
			emask = (0xF << (4 - n1));
			CHANNEL_EXTRACT_S16_21L(sp[0], sp[1], dd);
			mmx_pst_16(dd, dp, emask);
		}
	} else {
		for (j = 0; j < n4; j++) {
			CHANNEL_EXTRACT_S16_21R(sp[0], sp[1], dp[0]);
			sp += 2;
			dp += 1;
		}

		if (n1 > 0) {
			emask = (0xF << (4 - n1));
			CHANNEL_EXTRACT_S16_21R(sp[0], sp[1], dd);
			mmx_pst_16(dd, dp, emask);
		}
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_S16_21(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	__m64 *sp, *dp;
	__m64 *sl, *dl;
	__m64 dd;
	mlib_s32 j, n1, nu, emask;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

	if (cmask == 2) {
		for (j = 0; j < ysize; j++) {
			n1 = xsize;
			nu = ((8 - (mlib_addr)sp) & 7);
			if ((nu > 0) && (nu <= n1)) {
				emask = 0xF << (4 - (nu >> 1));
				CHANNEL_EXTRACT_S16_21L(sp[0], sp[1], dd);
				mmx_pst_16(dd, dp, emask);
				n1 -= (nu >> 1);
				dp = (__m64 *) ((mlib_u8 *)dp + nu);
				sp = (__m64 *) ((mlib_u8 *)sp + (nu << 1));
			}

			for (; n1 > 3; n1 -= 4) {
				CHANNEL_EXTRACT_S16_21L(sp[0], sp[1], dp[0]);
				sp += 2;
				dp += 1;
			}

			if (n1 > 0) {
				emask = (0xF << (4 - n1));
				CHANNEL_EXTRACT_S16_21L(sp[0], sp[1], dd);
				mmx_pst_16(dd, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else
		for (j = 0; j < ysize; j++) {
			n1 = xsize;
			nu = ((8 - (mlib_addr)sp) & 7);
			if ((nu > 0) && (nu <= n1)) {
				emask = 0xF << (4 - (nu >> 1));
				CHANNEL_EXTRACT_S16_21R(sp[0], sp[1], dd);
				mmx_pst_16(dd, dp, emask);
				n1 -= (nu >> 1);
				dp = (__m64 *) ((mlib_u8 *)dl + nu);
				sp = (__m64 *) ((mlib_u8 *)sl + (nu << 1));
			}

			for (; n1 > 3; n1 -= 4) {
				CHANNEL_EXTRACT_S16_21R(sp[0], sp[1], dp[0]);
				sp += 2;
				dp += 1;
			}

			if (n1 > 0) {
				emask = (0xF << (4 - n1));
				CHANNEL_EXTRACT_S16_21R(sp[0], sp[1], dd);
				mmx_pst_16(dd, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}

	_mm_empty();
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_S16_31L(sd0, sd1, sd2, dd)                      \
	{                                                               \
	    __m64 sda =                                                 \
		_mm_unpacklo_pi16(sd0, _mm_unpackhi_pi32(sd1, sd1));    \
	    __m64 sdb =                                                 \
		_mm_unpackhi_pi16(sd0, _mm_unpacklo_pi32(sd2, sd2));    \
	    dd = _mm_unpacklo_pi16(sda, _mm_unpackhi_pi32(sdb, sdb));   \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_S16_31M(sd0, sd1, sd2, dd)                      \
	{                                                               \
	    __m64 sda =                                                 \
		_mm_unpacklo_pi16(sd0, _mm_unpackhi_pi32(sd1, sd1));    \
	    __m64 sdb =                                                 \
		_mm_unpacklo_pi16(sd1, _mm_unpackhi_pi32(sd2, sd2));    \
	    dd = _mm_unpackhi_pi16(sda, _mm_unpacklo_pi32(sdb, sdb));   \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_S16_31R(sd0, sd1, sd2, dd)                        \
	{                                                                 \
	    __m64 sda = _mm_unpackhi_pi16(sd0, _mm_slli_si64(sd2, 32));   \
	    __m64 sdb = _mm_unpacklo_pi16(sd1, _mm_srli_si64(sd2, 32));   \
	                                                                  \
	    dd = _mm_unpacklo_pi16(sda, _mm_unpackhi_pi32(sdb, sdb));     \
	}

/* *********************************************************** */

/*
 * extract one channel from a 3-channel image.
 * both source and destination image data are 8-byte aligned.
 * xsize is multiple of 8.
 *
 */

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_S16_31_A8D1X4(
    mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	__m64 *sp, *dp;
	mlib_s32 n4;

	sp = (__m64 *) src;
	dp = (__m64 *) dst;

	if (cmask == 4) {
		for (n4 = dsize; n4 > 3; n4 -= 4) {
			CHANNEL_EXTRACT_S16_31L(sp[0], sp[1], sp[2], dp[0]);
			sp += 3;
			dp += 1;
		}
	} else if (cmask == 2) {
		for (n4 = dsize; n4 > 3; n4 -= 4) {
			CHANNEL_EXTRACT_S16_31M(sp[0], sp[1], sp[2], dp[0]);
			sp += 3;
			dp += 1;
		}
	} else
		for (n4 = dsize; n4 > 3; n4 -= 4) {
			CHANNEL_EXTRACT_S16_31R(sp[0], sp[1], sp[2], dp[0]);
			sp += 3;
			dp += 1;
		}

	_mm_empty();
}

/* *********************************************************** */

/*
 * extract one channel from a 3-channel image.
 * both source and destination image data are 8-byte aligned.
 * xsize is multiple of 8.
 *
 */

void
mlib_m_ImageChannelExtract_S16_31_A8D2X4(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	__m64 *sp, *dp;
	__m64 *sl, *dl;
	mlib_s32 j, n4;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

	if (cmask == 4) {
		for (j = 0; j < ysize; j++) {
			for (n4 = xsize; n4 > 3; n4 -= 4) {
				CHANNEL_EXTRACT_S16_31L(sp[0], sp[1], sp[2],
				    dp[0]);
				sp += 3;
				dp += 1;
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 2) {
		for (j = 0; j < ysize; j++) {
			for (n4 = xsize; n4 > 3; n4 -= 4) {
				CHANNEL_EXTRACT_S16_31M(sp[0], sp[1], sp[2],
				    dp[0]);
				sp += 3;
				dp += 1;
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else
		for (j = 0; j < ysize; j++) {
			for (n4 = xsize; n4 > 3; n4 -= 4) {
				CHANNEL_EXTRACT_S16_31R(sp[0], sp[1], sp[2],
				    dp[0]);
				sp += 3;
				dp += 1;
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}

	_mm_empty();
}

/* *********************************************************** */

/*
 * extract one channel from a 3-channel image.
 */

void
mlib_m_ImageChannelExtract_S16_31_D1(
    mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	__m64 *sp, *dp;
	__m64 dd;
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;

	sp = (__m64 *) src;
	dp = (__m64 *) dst;

	if (cmask == 4) {
		for (n1 = dsize; n1 > 3; n1 -= 4) {
			CHANNEL_EXTRACT_S16_31L(sp[0], sp[1], sp[2], dp[0]);
			sp += 3;
			dp += 1;
		}

		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			CHANNEL_EXTRACT_S16_31L(sp[0], sp[1], sp[2], dd);
			mmx_pst_16(dd, dp, emask);
		}
	} else if (cmask == 2) {
		for (n1 = dsize; n1 > 3; n1 -= 4) {
			CHANNEL_EXTRACT_S16_31M(sp[0], sp[1], sp[2], dp[0]);
			sp += 3;
			dp += 1;
		}

		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			CHANNEL_EXTRACT_S16_31M(sp[0], sp[1], sp[2], dd);
			mmx_pst_16(dd, dp, emask);
		}
	} else {
		for (n1 = dsize; n1 > 3; n1 -= 4) {
			CHANNEL_EXTRACT_S16_31R(sp[0], sp[1], sp[2], dp[0]);
			sp += 3;
			dp += 1;
		}

		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			CHANNEL_EXTRACT_S16_31R(sp[0], sp[1], sp[2], dd);
			mmx_pst_16(dd, dp, emask);
		}
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_S16_31(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	__m64 *sp, *dp;
	__m64 *sl, *dl;
	__m64 dd;
	mlib_s32 j, n1, nu, emask;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

	if (cmask == 4) {
		for (j = 0; j < ysize; j++) {
			n1 = xsize;
			nu = (((8 - (mlib_addr)sp) & 7));
			if ((nu > 0) && (nu <= n1)) {
				emask = 0xF << (4 - (nu >> 1));
				CHANNEL_EXTRACT_S16_31L(sp[0], sp[1], sp[2],
				    dd);
				mmx_pst_16(dd, dp, emask);
				n1 -= (nu >> 1);
				dp = (__m64 *) ((mlib_u8 *)dp + nu);
				sp = (__m64 *) ((mlib_u8 *)sp + (nu * 3));
			}

			for (; n1 > 3; n1 -= 4) {
				CHANNEL_EXTRACT_S16_31L(sp[0], sp[1], sp[2],
				    dp[0]);
				sp += 3;
				dp += 1;
			}

			if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				CHANNEL_EXTRACT_S16_31L(sp[0], sp[1], sp[2],
				    dd);
				mmx_pst_16(dd, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 2) {
		for (j = 0; j < ysize; j++) {
			n1 = xsize;
			nu = (((8 - (mlib_addr)sp) & 7));
			if ((nu > 0) && (nu <= n1)) {
				emask = 0xF << (4 - (nu >> 1));
				CHANNEL_EXTRACT_S16_31M(sp[0], sp[1], sp[2],
				    dd);
				mmx_pst_16(dd, dp, emask);
				n1 -= (nu >> 1);
				dp = (__m64 *) ((mlib_u8 *)dp + nu);
				sp = (__m64 *) ((mlib_u8 *)sp + (nu * 3));
			}

			for (; n1 > 3; n1 -= 4) {
				CHANNEL_EXTRACT_S16_31M(sp[0], sp[1], sp[2],
				    dp[0]);
				sp += 3;
				dp += 1;
			}

			if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				CHANNEL_EXTRACT_S16_31M(sp[0], sp[1], sp[2],
				    dd);
				mmx_pst_16(dd, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else
		for (j = 0; j < ysize; j++) {
			n1 = xsize;
			nu = (((8 - (mlib_addr)sp) & 7));
			if ((nu > 0) && (nu <= n1)) {
				emask = 0xF << (4 - (nu >> 1));
				CHANNEL_EXTRACT_S16_31R(sp[0], sp[1], sp[2],
				    dd);
				mmx_pst_16(dd, dp, emask);
				n1 -= (nu >> 1);
				dp = (__m64 *) ((mlib_u8 *)dp + nu);
				sp = (__m64 *) ((mlib_u8 *)sp + (nu * 3));
			}

			for (; n1 > 3; n1 -= 4) {
				CHANNEL_EXTRACT_S16_31R(sp[0], sp[1], sp[2],
				    dp[0]);
				sp += 3;
				dp += 1;
			}

			if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				CHANNEL_EXTRACT_S16_31R(sp[0], sp[1], sp[2],
				    dd);
				mmx_pst_16(dd, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}

	_mm_empty();
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_S16_41L(sd0, sd1, sd2, sd3, dd)         \
	{                                                       \
	    __m64 sda = _mm_unpacklo_pi16(sd0, sd2);            \
	    __m64 sdb = _mm_unpacklo_pi16(sd1, sd3);            \
	                                                        \
	    dd = _mm_unpacklo_pi16(sda, sdb);                   \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_S16_41ML(sd0, sd1, sd2, sd3, dd)        \
	{                                                       \
	    __m64 sda = _mm_unpacklo_pi16(sd0, sd2);            \
	    __m64 sdb = _mm_unpacklo_pi16(sd1, sd3);            \
	                                                        \
	    dd = _mm_unpackhi_pi16(sda, sdb);                   \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_S16_41MR(sd0, sd1, sd2, sd3, dd)        \
	{                                                       \
	    __m64 sda = _mm_unpackhi_pi16(sd0, sd2);            \
	    __m64 sdb = _mm_unpackhi_pi16(sd1, sd3);            \
	                                                        \
	    dd = _mm_unpacklo_pi16(sda, sdb);                   \
	}

/* *********************************************************** */
#define	CHANNEL_EXTRACT_S16_41R(sd0, sd1, sd2, sd3, dd)         \
	{                                                       \
	    __m64 sda = _mm_unpackhi_pi16(sd0, sd2);            \
	    __m64 sdb = _mm_unpackhi_pi16(sd1, sd3);            \
	                                                        \
	    dd = _mm_unpackhi_pi16(sda, sdb);                   \
	}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_S16_41_A8D1X4(
    mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	__m64 *sp, *dp;
	mlib_s32 i, n4;

	n4 = dsize >> 2;

	sp = (__m64 *) src;
	dp = (__m64 *) dst;

	if (cmask == 8) {
		for (i = 0; i < n4; i++) {
			CHANNEL_EXTRACT_S16_41L(sp[0], sp[1], sp[2], sp[3],
			    dp[0]);
			sp += 4;
			dp += 1;
		}
	} else if (cmask == 4) {
		for (i = 0; i < n4; i++) {
			CHANNEL_EXTRACT_S16_41ML(sp[0], sp[1], sp[2], sp[3],
			    dp[0]);
			sp += 4;
			dp += 1;
		}
	} else if (cmask == 2) {
		for (i = 0; i < n4; i++) {
			CHANNEL_EXTRACT_S16_41MR(sp[0], sp[1], sp[2], sp[3],
			    dp[0]);
			sp += 4;
			dp += 1;
		}
	} else
		for (i = 0; i < n4; i++) {
			CHANNEL_EXTRACT_S16_41R(sp[0], sp[1], sp[2], sp[3],
			    dp[0]);
			sp += 4;
			dp += 1;
		}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_S16_41_A8D2X4(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	__m64 *sp, *dp;
	__m64 *sl, *dl;
	mlib_s32 i, j, n4;

	n4 = xsize >> 2;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

	if (cmask == 8) {
		for (j = 0; j < ysize; j++) {
			for (i = 0; i < n4; i++) {
				CHANNEL_EXTRACT_S16_41L(sp[0], sp[1], sp[2],
				    sp[3], dp[0]);
				sp += 4;
				dp += 1;
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 4) {
		for (j = 0; j < ysize; j++) {
			for (i = 0; i < n4; i++) {
				CHANNEL_EXTRACT_S16_41ML(sp[0], sp[1], sp[2],
				    sp[3], dp[0]);
				sp += 4;
				dp += 1;
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 2) {
		for (j = 0; j < ysize; j++) {
			for (i = 0; i < n4; i++) {
				CHANNEL_EXTRACT_S16_41MR(sp[0], sp[1], sp[2],
				    sp[3], dp[0]);
				sp += 4;
				dp += 1;
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else
		for (j = 0; j < ysize; j++) {
			for (i = 0; i < n4; i++) {
				CHANNEL_EXTRACT_S16_41R(sp[0], sp[1], sp[2],
				    sp[3], dp[0]);
				sp += 4;
				dp += 1;
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}

	_mm_empty();
}

/* *********************************************************** */
void
mlib_m_ImageChannelExtract_S16_41_D1(
    mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	__m64 *sp, *dp;
	__m64 dd;
	mlib_s32 emask;
	mlib_s32 i, n4, n1;

	n4 = dsize >> 2;
	n1 = dsize & 3;

	sp = (__m64 *) src;
	dp = (__m64 *) dst;

	if (cmask == 8) {
		for (i = 0; i < n4; i++) {
			CHANNEL_EXTRACT_S16_41L(sp[0], sp[1], sp[2], sp[3],
			    dp[0]);
			sp += 4;
			dp += 1;
		}

		if (n1 > 0) {
			emask = (0xF << (4 - n1));
			CHANNEL_EXTRACT_S16_41L(sp[0], sp[1], sp[2], sp[3], dd);
			mmx_pst_16(dd, dp, emask);
		}
	} else if (cmask == 4) {
		for (i = 0; i < n4; i++) {
			CHANNEL_EXTRACT_S16_41ML(sp[0], sp[1], sp[2], sp[3],
			    dp[0]);
			sp += 4;
			dp += 1;
		}

		if (n1 > 0) {
			emask = (0xF << (4 - n1));
			CHANNEL_EXTRACT_S16_41ML(sp[0], sp[1], sp[2], sp[3],
			    dd);
			mmx_pst_16(dd, dp, emask);
		}
	} else if (cmask == 2) {
		for (i = 0; i < n4; i++) {
			CHANNEL_EXTRACT_S16_41MR(sp[0], sp[1], sp[2], sp[3],
			    dp[0]);
			sp += 4;
			dp += 1;
		}

		if (n1 > 0) {
			emask = (0xF << (4 - n1));
			CHANNEL_EXTRACT_S16_41MR(sp[0], sp[1], sp[2], sp[3],
			    dd);
			mmx_pst_16(dd, dp, emask);
		}
	} else {
		for (i = 0; i < n4; i++) {
			CHANNEL_EXTRACT_S16_41R(sp[0], sp[1], sp[2], sp[3],
			    dp[0]);
			sp += 4;
			dp += 1;
		}

		if (n1 > 0) {
			emask = (0xF << (4 - n1));
			CHANNEL_EXTRACT_S16_41R(sp[0], sp[1], sp[2], sp[3], dd);
			mmx_pst_16(dd, dp, emask);
		}
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_S16_41(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	__m64 *sp, *dp;
	__m64 *sl, *dl;
	__m64 dd;
	mlib_s32 j, n1, nu, emask;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

	if (cmask == 8) {
		for (j = 0; j < ysize; j++) {
			n1 = xsize;
			nu = ((8 - (mlib_addr)sp) & 7);
			if ((nu > 0) && (nu <= n1)) {
				emask = 0xF << (4 - (nu >> 1));
				CHANNEL_EXTRACT_S16_41L(sp[0], sp[1], sp[2],
				    sp[3], dd);
				mmx_pst_16(dd, dp, emask);
				n1 -= (nu >> 1);
				dp = (__m64 *) ((mlib_u8 *)dp + nu);
				sp = (__m64 *) ((mlib_u8 *)sp + (nu << 2));
			}

			for (; n1 > 3; n1 -= 4) {
				CHANNEL_EXTRACT_S16_41L(sp[0], sp[1], sp[2],
				    sp[3], dp[0]);
				sp += 4;
				dp += 1;
			}

			if (n1 > 0) {
				emask = (0xF << (4 - n1));
				CHANNEL_EXTRACT_S16_41L(sp[0], sp[1], sp[2],
				    sp[3], dd);
				mmx_pst_16(dd, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 4) {
		for (j = 0; j < ysize; j++) {
			n1 = xsize;
			nu = ((8 - (mlib_addr)sp) & 7);
			if ((nu > 0) && (nu <= n1)) {
				emask = 0xF << (4 - (nu >> 1));
				CHANNEL_EXTRACT_S16_41ML(sp[0], sp[1], sp[2],
				    sp[3], dd);
				mmx_pst_16(dd, dp, emask);
				n1 -= (nu >> 1);
				dp = (__m64 *) ((mlib_u8 *)dp + nu);
				sp = (__m64 *) ((mlib_u8 *)sp + (nu << 2));
			}

			for (; n1 > 3; n1 -= 4) {
				CHANNEL_EXTRACT_S16_41ML(sp[0], sp[1], sp[2],
				    sp[3], dp[0]);
				sp += 4;
				dp += 1;
			}

			if (n1 > 0) {
				emask = (0xF << (4 - n1));
				CHANNEL_EXTRACT_S16_41ML(sp[0], sp[1], sp[2],
				    sp[3], dd);
				mmx_pst_16(dd, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 2) {
		for (j = 0; j < ysize; j++) {
			n1 = xsize;
			nu = ((8 - (mlib_addr)sp) & 7);
			if ((nu > 0) && (nu <= n1)) {
				emask = 0xF << (4 - (nu >> 1));
				CHANNEL_EXTRACT_S16_41MR(sp[0], sp[1], sp[2],
				    sp[3], dd);
				mmx_pst_16(dd, dp, emask);
				n1 -= (nu >> 1);
				dp = (__m64 *) ((mlib_u8 *)dp + nu);
				sp = (__m64 *) ((mlib_u8 *)sp + (nu << 2));
			}

			for (; n1 > 3; n1 -= 4) {
				CHANNEL_EXTRACT_S16_41MR(sp[0], sp[1], sp[2],
				    sp[3], dp[0]);
				sp += 4;
				dp += 1;
			}

			if (n1 > 0) {
				emask = (0xF << (4 - n1));
				CHANNEL_EXTRACT_S16_41MR(sp[0], sp[1], sp[2],
				    sp[3], dd);
				mmx_pst_16(dd, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else
		for (j = 0; j < ysize; j++) {
			n1 = xsize;
			nu = ((8 - (mlib_addr)sp) & 7);
			if ((nu > 0) && (nu <= n1)) {
				emask = 0xF << (4 - (nu >> 1));
				CHANNEL_EXTRACT_S16_41R(sp[0], sp[1], sp[2],
				    sp[3], dd);
				mmx_pst_16(dd, dp, emask);
				n1 -= (nu >> 1);
				dp = (__m64 *) ((mlib_u8 *)dp + nu);
				sp = (__m64 *) ((mlib_u8 *)sp + (nu << 2));
			}

			for (; n1 > 3; n1 -= 4) {
				CHANNEL_EXTRACT_S16_41R(sp[0], sp[1], sp[2],
				    sp[3], dp[0]);
				sp += 4;
				dp += 1;
			}

			if (n1 > 0) {
				emask = (0xF << (4 - n1));
				CHANNEL_EXTRACT_S16_41R(sp[0], sp[1], sp[2],
				    sp[3], dd);
				mmx_pst_16(dd, dp, emask);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}

	_mm_empty();
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_32_21R(sd0, sd1, dd)                    \
	{                                                       \
	    dd = _mm_unpackhi_pi32(sd0, sd1);                   \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_32_21L(sd0, sd1, dd)                    \
	{                                                       \
	    dd = _mm_unpacklo_pi32(sd0, sd1);                   \
	}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_32_21(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	__m64 *sp, *dp;
	__m64 *sl, *dl;
	__m64 dd;
	mlib_s32 j, n1;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

	if (cmask == 2) {
		for (j = 0; j < ysize; j++) {
			for (n1 = xsize; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_21L(sp[0], sp[1], dp[0]);
				sp += 2;
				dp += 1;
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_21L(sp[0], sp[1], dd);
				mmx_pst_32(dd, dp, 2);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else
		for (j = 0; j < ysize; j++) {
			for (n1 = xsize; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_21R(sp[0], sp[1], dp[0]);
				sp += 2;
				dp += 1;
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_21R(sp[0], sp[1], dd);
				mmx_pst_32(dd, dp, 2);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}

	_mm_empty();
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_32_31R(sd1, sd2, dd)                       \
	{                                                          \
	    dd = _mm_unpacklo_pi32(sd1, _mm_srli_si64(sd2, 32));   \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_32_31M(sd0, sd2, dd)                       \
	{                                                          \
	    dd = _mm_unpackhi_pi32(sd0, _mm_slli_si64(sd2, 32));   \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_32_31L(sd0, sd1, dd)                       \
	{                                                          \
	    dd = _mm_unpacklo_pi32(sd0, _mm_srli_si64(sd1, 32));   \
	}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_32_31(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	__m64 *sp, *dp;
	__m64 *sl, *dl;
	__m64 dd;
	mlib_s32 j, n1;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

	if (cmask == 4) {
		for (j = 0; j < ysize; j++) {
			n1 = xsize;

			for (; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_31L(sp[0], sp[1], dp[0]);
				sp += 3;
				dp += 1;
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_31L(sp[0], sp[1], dd);
				mmx_pst_32(dd, dp, 2);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 2) {
		for (j = 0; j < ysize; j++) {
			n1 = xsize;

			for (; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_31M(sp[0], sp[2], dp[0]);
				sp += 3;
				dp += 1;
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_31M(sp[0], sp[2], dd);
				mmx_pst_32(dd, dp, 2);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else
		for (j = 0; j < ysize; j++) {
			n1 = xsize;

			for (; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_31R(sp[1], sp[2], dp[0]);
				sp += 3;
				dp += 1;
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_31R(sp[1], sp[2], dd);
				mmx_pst_32(dd, dp, 2);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}

	_mm_empty();
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_32_41R(sd1, sd3, dd)                    \
	{                                                       \
	    dd = _mm_unpackhi_pi32(sd1, sd3);                   \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_32_41MR(sd1, sd3, dd)                   \
	{                                                       \
	    dd = _mm_unpacklo_pi32(sd1, sd3);                   \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_32_41ML(sd0, sd2, dd)                   \
	{                                                       \
	    dd = _mm_unpackhi_pi32(sd0, sd2);                   \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_32_41L(sd0, sd2, dd)                    \
	{                                                       \
	    dd = _mm_unpacklo_pi32(sd0, sd2);                   \
	}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_32_41(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	__m64 *sp, *dp;
	__m64 *sl, *dl;
	__m64 dd;
	mlib_s32 j, n1;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

	if (cmask == 8) {
		for (j = 0; j < ysize; j++) {
			n1 = xsize;
			for (; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_41L(sp[0], sp[2], dp[0]);
				sp += 4;
				dp += 1;
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_41L(sp[0], sp[2], dd);
				mmx_pst_32(dd, dp, 2);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 4) {
		for (j = 0; j < ysize; j++) {
			n1 = xsize;
			for (; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_41ML(sp[0], sp[2], dp[0]);
				sp += 4;
				dp += 1;
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_41ML(sp[0], sp[2], dd);
				mmx_pst_32(dd, dp, 2);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 2) {
		for (j = 0; j < ysize; j++) {
			n1 = xsize;
			for (; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_41MR(sp[1], sp[3], dp[0]);
				sp += 4;
				dp += 1;
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_41MR(sp[1], sp[3], dd);
				mmx_pst_32(dd, dp, 2);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else
		for (j = 0; j < ysize; j++) {
			n1 = xsize;
			for (; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_41R(sp[1], sp[3], dp[0]);
				sp += 4;
				dp += 1;
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_41R(sp[1], sp[3], dd);
				mmx_pst_32(dd, dp, 2);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}

	_mm_empty();
}

/* *********************************************************** */
