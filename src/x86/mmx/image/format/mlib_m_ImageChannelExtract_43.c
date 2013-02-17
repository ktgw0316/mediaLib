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

#pragma ident	"@(#)mlib_m_ImageChannelExtract_43.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_m_ImageChannelExtract_U8_43
 *      mlib_m_ImageChannelExtract_U8_43R_D1
 *      mlib_m_ImageChannelExtract_U8_43R
 *      mlib_m_ImageChannelExtract_U8_43L_D1
 *      mlib_m_ImageChannelExtract_U8_43L
 *      mlib_m_ImageChannelExtract_S16_43R_D1
 *      mlib_m_ImageChannelExtract_S16_43R
 *      mlib_m_ImageChannelExtract_S16_43L_D1
 *      mlib_m_ImageChannelExtract_S16_43L
 *
 * SYNOPSIS
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
 *      extract the right or left 3 channels of a 4-channel image to
 *      a 3-channel image -- VIS version low level functions.
 *
 *      ABGR => BGR   (43R), or  RGBA => RGB  (43L)
 *
 * NOTE
 *      These functions are separated from mlib_m_ImageChannelExtract.c
 *      for loop unrolling and structure clarity.
 */

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <mmintrin.h>
#endif


#include <mlib_image.h>
#include <mlib_mmx_utils.h>

/* *********************************************************** */
/*
 * Both source and destination image data are 1-d vectors and
 * 8-byte aligned. And dsize is multiple of 8.
 *
 */

void
mlib_m_ImageChannelExtract_U8_43R_D1(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
	mlib_s32 *sp;
	mlib_s32 n1;
	mlib_u8 *dp;

	sp = (mlib_s32 *)src;
	dp = (mlib_u8 *)dst;

	n1 = dsize;

	for (; n1 > 4; n1 -= 4) {
		*((mlib_s32 *)(dp + 0)) = sp[0] >> 8;
		*((mlib_s32 *)(dp + 3)) = sp[1] >> 8;
		*((mlib_s32 *)(dp + 6)) = sp[2] >> 8;
		*((mlib_s32 *)(dp + 9)) = sp[3] >> 8;
		dp += 12;
		sp += 4;
	}

	for (; n1 > 0; n1--) {
		*(dp + 0) = *((mlib_u8 *)sp + 1);
		*(dp + 1) = *((mlib_u8 *)sp + 2);
		*(dp + 2) = *((mlib_u8 *)sp + 3);
		dp += 3;
		sp += 1;
	}
}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_U8_43R(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s32 *sp, *sl;
	mlib_s32 j, n1;
	mlib_u8 *dp, *dl;

	sp = sl = (mlib_s32 *)src;
	dp = dl = (mlib_u8 *)dst;

	for (j = 0; j < ysize; j++) {
		for (n1 = xsize; n1 > 4; n1 -= 4) {
			*((mlib_s32 *)(dp + 0)) = sp[0] >> 8;
			*((mlib_s32 *)(dp + 3)) = sp[1] >> 8;
			*((mlib_s32 *)(dp + 6)) = sp[2] >> 8;
			*((mlib_s32 *)(dp + 9)) = sp[3] >> 8;
			dp += 12;
			sp += 4;
		}

		for (; n1 > 0; n1--) {
			*(dp + 0) = *((mlib_u8 *)sp + 1);
			*(dp + 1) = *((mlib_u8 *)sp + 2);
			*(dp + 2) = *((mlib_u8 *)sp + 3);
			dp += 3;
			sp += 1;
		}

		sp = sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */
/*
 * Both source and destination image data are 1-d vectors and
 * 8-byte aligned. And dsize is multiple of 8.
 *
 */

void
mlib_m_ImageChannelExtract_U8_43L_D1(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
	mlib_s32 *sp;
	mlib_s32 n1;
	mlib_u8 *dp;

	sp = (mlib_s32 *)src;
	dp = (mlib_u8 *)dst;

	n1 = dsize;

	for (; n1 > 4; n1 -= 4) {
		*((mlib_s32 *)(dp + 0)) = sp[0];
		*((mlib_s32 *)(dp + 3)) = sp[1];
		*((mlib_s32 *)(dp + 6)) = sp[2];
		*((mlib_s32 *)(dp + 9)) = sp[3];
		dp += 12;
		sp += 4;
	}

	for (; n1 > 0; n1--) {
		*(dp + 0) = *((mlib_u8 *)sp + 0);
		*(dp + 1) = *((mlib_u8 *)sp + 1);
		*(dp + 2) = *((mlib_u8 *)sp + 2);
		dp += 3;
		sp += 1;
	}
}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_U8_43L(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s32 *sp, *sl;
	mlib_s32 j, n1;
	mlib_u8 *dp, *dl;

	sp = sl = (mlib_s32 *)src;
	dp = dl = (mlib_u8 *)dst;

	for (j = 0; j < ysize; j++) {
		for (n1 = xsize; n1 > 4; n1 -= 4) {
			*((mlib_s32 *)(dp + 0)) = sp[0];
			*((mlib_s32 *)(dp + 3)) = sp[1];
			*((mlib_s32 *)(dp + 6)) = sp[2];
			*((mlib_s32 *)(dp + 9)) = sp[3];
			dp += 12;
			sp += 4;
		}

		for (; n1 > 0; n1--) {
			*(dp + 0) = *((mlib_u8 *)sp + 0);
			*(dp + 1) = *((mlib_u8 *)sp + 1);
			*(dp + 2) = *((mlib_u8 *)sp + 2);
			dp += 3;
			sp += 1;
		}

		sp = sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_U8_43(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_s32 *sp, *sl;
	mlib_s32 j, n1;
	mlib_u8 *dp, *dl;

	sp = sl = (mlib_s32 *)src;
	dp = dl = (mlib_u8 *)dst;

	if (cmask == 11) {
		for (j = 0; j < ysize; j++) {
			n1 = xsize;

			for (; n1 > 4; n1 -= 4) {
				*((mlib_s32 *)(dp + 0)) =
				    (sp[0] & 0x000000ff) | ((sp[0] & 0xffff0000)
				    >> 8);
				*((mlib_s32 *)(dp + 3)) =
				    (sp[1] & 0x000000ff) | ((sp[1] & 0xffff0000)
				    >> 8);
				*((mlib_s32 *)(dp + 6)) =
				    (sp[2] & 0x000000ff) | ((sp[2] & 0xffff0000)
				    >> 8);
				*((mlib_s32 *)(dp + 9)) =
				    (sp[3] & 0x000000ff) | ((sp[3] & 0xffff0000)
				    >> 8);
				dp += 12;
				sp += 4;
			}

			for (; n1 > 0; n1--) {
				*(dp + 0) = *((mlib_u8 *)sp + 0);
				*(dp + 1) = *((mlib_u8 *)sp + 2);
				*(dp + 2) = *((mlib_u8 *)sp + 3);
				dp += 3;
				sp += 1;
			}

			sp = sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
		}
	} else {
		for (j = 0; j < ysize; j++) {
			n1 = xsize;

			for (; n1 > 4; n1 -= 4) {
				*((mlib_s32 *)(dp + 0)) =
				    (sp[0] & 0x0000ffff) | ((sp[0] & 0xff000000)
				    >> 8);
				*((mlib_s32 *)(dp + 3)) =
				    (sp[1] & 0x0000ffff) | ((sp[1] & 0xff000000)
				    >> 8);
				*((mlib_s32 *)(dp + 6)) =
				    (sp[2] & 0x0000ffff) | ((sp[2] & 0xff000000)
				    >> 8);
				*((mlib_s32 *)(dp + 9)) =
				    (sp[3] & 0x0000ffff) | ((sp[3] & 0xff000000)
				    >> 8);
				dp += 12;
				sp += 4;
			}

			for (; n1 > 0; n1--) {
				*(dp + 0) = *((mlib_u8 *)sp + 0);
				*(dp + 1) = *((mlib_u8 *)sp + 1);
				*(dp + 2) = *((mlib_u8 *)sp + 3);
				dp += 3;
				sp += 1;
			}

			sp = sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_S16_43R(sd0, sd1, sd2, sd3, dd0, dd1,       \
	dd2)                                                        \
	{                                                           \
	    __m64 sda =                                             \
		_mm_unpacklo_pi16(_mm_srli_si64(sd0, 48),           \
		_mm_srli_si64(sd1, 16));                            \
	    __m64 sdb =                                             \
		_mm_unpacklo_pi16(_mm_srli_si64(sd2, 48),           \
		_mm_srli_si64(sd3, 16));                            \
	    dd0 = _mm_unpacklo_pi32(_mm_srli_si64(sd0, 16), sda);   \
	    dd1 = _mm_unpackhi_pi32(sd1, _mm_slli_si64(sd2, 16));   \
	    dd2 = _mm_unpacklo_pi32(sdb, _mm_srli_si64(sd3, 32));   \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_S16_43L(sd0, sd1, sd2, sd3, dd0, dd1,             \
	dd2)                                                              \
	{                                                                 \
	    __m64 sda = _mm_unpacklo_pi16(_mm_srli_si64(sd0, 32), sd1);   \
	    __m64 sdb = _mm_unpacklo_pi16(_mm_srli_si64(sd2, 32), sd3);   \
	                                                                  \
	    dd0 = _mm_unpacklo_pi32(sd0, sda);                            \
	    dd1 = _mm_unpacklo_pi32(_mm_srli_si64(sd1, 16), sd2);         \
	    dd2 = _mm_unpacklo_pi32(sdb, _mm_srli_si64(sd3, 16));         \
	}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_S16_43R_A8D1X4(
    mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
	__m64 *sp, *dp;
	mlib_s32 n1;

	sp = (__m64 *) src;
	dp = (__m64 *) dst;

	n1 = dsize;

	for (; n1 > 3; n1 -= 4) {
		CHANNEL_EXTRACT_S16_43R(sp[0], sp[1], sp[2], sp[3], dp[0],
		    dp[1], dp[2])
		    sp += 4;
		dp += 3;
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_S16_43R_A8D2X4(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	__m64 *sp, *dp;
	__m64 *sl, *dl;
	mlib_s32 i, j, n4;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

	n4 = xsize >> 2;

	for (j = 0; j < ysize; j++) {
		for (i = 0; i < n4; i++) {
			CHANNEL_EXTRACT_S16_43R(sp[0], sp[1], sp[2], sp[3],
			    dp[0], dp[1], dp[2])
			    sp += 4;
			dp += 3;
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_S16_43R_D1(
    mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
	__m64 *sp, *dp;
	__m64 dd0, dd1, dd2;
	mlib_s32 n1;

	sp = (__m64 *) src;
	dp = (__m64 *) dst;

	n1 = dsize;

	for (; n1 > 3; n1 -= 4) {
		CHANNEL_EXTRACT_S16_43R(sp[0], sp[1], sp[2], sp[3], dp[0],
		    dp[1], dp[2])
		    sp += 4;
		dp += 3;
	}

	if (n1 > 2) {
		CHANNEL_EXTRACT_S16_43R(sp[0], sp[1], sp[2], sp[3], dp[0],
		    dp[1], dd0)
		    dp += 2;
		mmx_pst_16(dd0, dp, (0xF << 3));
	} else if (n1 > 1) {
		CHANNEL_EXTRACT_S16_43R(sp[0], sp[1], sp[2], sp[3], dp[0], dd0,
		    dd1)
		    dp += 1;
		mmx_pst_16(dd0, dp, (0xF << 2));
	} else if (n1 > 0) {
		CHANNEL_EXTRACT_S16_43R(sp[0], sp[1], sp[2], sp[3], dd0, dd1,
		    dd2)
		    mmx_pst_16(dd0, dp, (0xF << 1));
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_S16_43R(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	__m64 *sp, *dp;
	__m64 *sl, *dl;
	__m64 dd0, dd1, dd2;
	mlib_s32 j, n1;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

	for (j = 0; j < ysize; j++) {
		for (n1 = xsize; n1 > 3; n1 -= 4) {
			CHANNEL_EXTRACT_S16_43R(sp[0], sp[1], sp[2], sp[3],
			    dp[0], dp[1], dp[2])
			    sp += 4;
			dp += 3;
		}

		if (n1 > 2) {
			CHANNEL_EXTRACT_S16_43R(sp[0], sp[1], sp[2], sp[3],
			    dp[0], dp[1], dd0)
			    dp += 2;
			mmx_pst_16(dd0, dp, (0xF << 3));
		} else if (n1 > 1) {
			CHANNEL_EXTRACT_S16_43R(sp[0], sp[1], sp[2], sp[3],
			    dp[0], dd0, dd1)
			    dp += 1;
			mmx_pst_16(dd0, dp, (0xF << 2));
		} else if (n1 > 0) {
			CHANNEL_EXTRACT_S16_43R(sp[0], sp[1], sp[2], sp[3], dd0,
			    dd1, dd2)
			    mmx_pst_16(dd0, dp, (0xF << 1));
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_S16_43L_A8D1X4(
    mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
	__m64 *sp, *dp;
	mlib_s32 n1;

	sp = (__m64 *) src;
	dp = (__m64 *) dst;

	n1 = dsize;

	for (; n1 > 3; n1 -= 4) {
		CHANNEL_EXTRACT_S16_43L(sp[0], sp[1], sp[2], sp[3], dp[0],
		    dp[1], dp[2])
		    sp += 4;
		dp += 3;
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_S16_43L_A8D2X4(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	__m64 *sp, *dp;
	__m64 *sl, *dl;
	mlib_s32 i, j, n4;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

	n4 = xsize >> 2;

	for (j = 0; j < ysize; j++) {
		for (i = 0; i < n4; i++) {
			CHANNEL_EXTRACT_S16_43L(sp[0], sp[1], sp[2], sp[3],
			    dp[0], dp[1], dp[2])
			    sp += 4;
			dp += 3;
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_S16_43L_D1(
    mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
	__m64 *sp, *dp;
	__m64 dd0, dd1, dd2;
	mlib_s32 n1;

	sp = (__m64 *) src;
	dp = (__m64 *) dst;

	n1 = dsize;

	for (; n1 > 3; n1 -= 4) {
		CHANNEL_EXTRACT_S16_43L(sp[0], sp[1], sp[2], sp[3], dp[0],
		    dp[1], dp[2])
		    sp += 4;
		dp += 3;
	}

	if (n1 > 2) {
		CHANNEL_EXTRACT_S16_43L(sp[0], sp[1], sp[2], sp[3], dp[0],
		    dp[1], dd0)
		    dp += 2;
		mmx_pst_16(dd0, dp, (0xF << 3));
	} else if (n1 > 1) {
		CHANNEL_EXTRACT_S16_43L(sp[0], sp[1], sp[2], sp[3], dp[0], dd0,
		    dd1)
		    dp += 1;
		mmx_pst_16(dd0, dp, (0xF << 2));
	} else if (n1 > 0) {
		CHANNEL_EXTRACT_S16_43L(sp[0], sp[1], sp[2], sp[3], dd0, dd1,
		    dd2)
		    mmx_pst_16(dd0, dp, (0xF << 1));
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_S16_43L(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	__m64 *sp, *dp;
	__m64 *sl, *dl;
	__m64 dd0, dd1, dd2;
	mlib_s32 j, n1;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

	for (j = 0; j < ysize; j++) {
		for (n1 = xsize; n1 > 3; n1 -= 4) {
			CHANNEL_EXTRACT_S16_43L(sp[0], sp[1], sp[2], sp[3],
			    dp[0], dp[1], dp[2])
			    sp += 4;
			dp += 3;
		}

		if (n1 > 2) {
			CHANNEL_EXTRACT_S16_43L(sp[0], sp[1], sp[2], sp[3],
			    dp[0], dp[1], dd0)
			    dp += 2;
			mmx_pst_16(dd0, dp, (0xF << 3));
		} else if (n1 > 1) {
			CHANNEL_EXTRACT_S16_43L(sp[0], sp[1], sp[2], sp[3],
			    dp[0], dd0, dd1)
			    dp += 1;
			mmx_pst_16(dd0, dp, (0xF << 2));
		} else if (n1 > 0) {
			CHANNEL_EXTRACT_S16_43L(sp[0], sp[1], sp[2], sp[3], dd0,
			    dd1, dd2)
			    mmx_pst_16(dd0, dp, (0xF << 1));
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_S16_43BL(sd0, sd1, sd2, sd3, dd0,                 \
	dd1, dd2)                                                         \
	{                                                                 \
	    __m64 sda = _mm_unpacklo_pi16(_mm_srli_si64(sd0, 48), sd1);   \
	    __m64 sdb = _mm_unpackhi_pi16(_mm_slli_si64(sd1, 32), sd1);   \
	    __m64 sdc = _mm_unpacklo_pi16(_mm_srli_si64(sd2, 48), sd3);   \
	    __m64 sdd = _mm_unpackhi_pi16(_mm_slli_si64(sd3, 32), sd3);   \
	                                                                  \
	    dd0 = _mm_unpacklo_pi32(sd0, sda);                            \
	    dd1 = _mm_unpackhi_pi32(sdb, _mm_slli_si64(sd2, 32));         \
	    dd2 = _mm_unpackhi_pi32(_mm_slli_si64(sdc, 32), sdd);         \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_S16_43BR(sd0, sd1, sd2, sd3, dd0,                 \
	dd1, dd2)                                                         \
	{                                                                 \
	    __m64 sda = _mm_unpacklo_pi16(sd0, _mm_srli_si64(sd0, 32));   \
	    __m64 sdb = _mm_unpacklo_pi16(_mm_srli_si64(sd0, 48), sd1);   \
	    __m64 sdc =                                                   \
		_mm_unpackhi_pi16(_mm_slli_si64(sd2, 48),                 \
		_mm_slli_si64(sd2, 16));                                  \
	    __m64 sdd = _mm_unpackhi_pi16(sd2, _mm_slli_si64(sd3, 48));   \
	                                                                  \
	    dd0 = _mm_unpacklo_pi32(sda, sdb);                            \
	    dd1 = _mm_unpackhi_pi32(sd1, sdc);                            \
	    dd2 = _mm_unpackhi_pi32(sdd, sd3);                            \
	}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_S16_43(
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
	__m64 dd0, dd1, dd2;
	mlib_s32 j, n1;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

	if (cmask == 11) {
		for (j = 0; j < ysize; j++) {
			for (n1 = xsize; n1 > 3; n1 -= 4) {
				CHANNEL_EXTRACT_S16_43BR(sp[0], sp[1], sp[2],
				    sp[3], dp[0], dp[1], dp[2])
				    sp += 4;
				dp += 3;
			}

			if (n1 > 2) {
				CHANNEL_EXTRACT_S16_43BR(sp[0], sp[1], sp[2],
				    sp[3], dp[0], dp[1], dd0)
				    dp += 2;
				mmx_pst_16(dd0, dp, (0xF << 3));
			} else if (n1 > 1) {
				CHANNEL_EXTRACT_S16_43BR(sp[0], sp[1], sp[2],
				    sp[3], dp[0], dd0, dd1)
				    dp += 1;
				mmx_pst_16(dd0, dp, (0xF << 2));
			} else if (n1 > 0) {
				CHANNEL_EXTRACT_S16_43BR(sp[0], sp[1], sp[2],
				    sp[3], dd0, dd1, dd2)
				    mmx_pst_16(dd0, dp, (0xF << 1));
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else {
		for (j = 0; j < ysize; j++) {
			for (n1 = xsize; n1 > 3; n1 -= 4) {
				CHANNEL_EXTRACT_S16_43BL(sp[0], sp[1], sp[2],
				    sp[3], dp[0], dp[1], dp[2])
				    sp += 4;
				dp += 3;
			}

			if (n1 > 2) {
				CHANNEL_EXTRACT_S16_43BL(sp[0], sp[1], sp[2],
				    sp[3], dp[0], dp[1], dd0)
				    dp += 2;
				mmx_pst_16(dd0, dp, (0xF << 3));
			} else if (n1 > 1) {
				CHANNEL_EXTRACT_S16_43BL(sp[0], sp[1], sp[2],
				    sp[3], dp[0], dd0, dd1)
				    dp += 1;
				mmx_pst_16(dd0, dp, (0xF << 2));
			} else if (n1 > 0) {
				CHANNEL_EXTRACT_S16_43BL(sp[0], sp[1], sp[2],
				    sp[3], dd0, dd1, dd2)
				    mmx_pst_16(dd0, dp, (0xF << 1));
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	}
	_mm_empty();
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_32_43R(sd0, sd1, sd2, sd3, dd0, dd1, dd2)   \
	{                                                           \
	    dd0 = _mm_unpackhi_pi32(sd0, _mm_slli_si64(sd1, 32));   \
	    dd1 = _mm_unpackhi_pi32(sd1, sd2);                      \
	    dd2 = sd3;                                              \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_32_43L(sd0, sd1, sd2, sd3, dd0, dd1, dd2)   \
	{                                                           \
	    dd0 = sd0;                                              \
	    dd1 = _mm_unpacklo_pi32(sd1, sd2);                      \
	    dd2 = _mm_unpackhi_pi32(sd2, _mm_slli_si64(sd3, 32));   \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_32_43BR(sd0, sd1, sd2, sd3, dd0, dd1,       \
	dd2)                                                        \
	{                                                           \
	    dd0 = _mm_unpacklo_pi32(sd0, sd1);                      \
	    dd1 = _mm_unpackhi_pi32(sd1, _mm_slli_si64(sd2, 32));   \
	    dd2 = sd3;                                              \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_32_43BL(sd0, sd1, sd2, sd3, dd0, dd1,       \
	dd2)                                                        \
	{                                                           \
	    dd0 = sd0;                                              \
	    dd1 = _mm_unpackhi_pi32(sd1, _mm_slli_si64(sd2, 32));   \
	    dd2 = _mm_unpackhi_pi32(sd2, sd3);                      \
	}

/* *********************************************************** */

void
mlib_m_ImageChannelExtract_32_43(
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
	__m64 dd0, dd1, dd2;
	mlib_s32 j, n1;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

	if (cmask == 7) {
		for (j = 0; j < ysize; j++) {
			for (n1 = xsize; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_43R(sp[0], sp[1], sp[2],
				    sp[3], dp[0], dp[1], dp[2])
				    sp += 4;
				dp += 3;
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_43R(sp[0], sp[1], sp[2],
				    sp[3], dp[0], dd1, dd2)
				    dp++;
				mmx_pst_32(dd1, dp, 2);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 14) {
		for (j = 0; j < ysize; j++) {
			for (n1 = xsize; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_43L(sp[0], sp[1], sp[2],
				    sp[3], dp[0], dp[1], dp[2])
				    sp += 4;
				dp += 3;
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_43L(sp[0], sp[1], sp[2],
				    sp[3], dp[0], dd1, dd2)
				    dp++;
				mmx_pst_32(dd1, dp, 2);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 11) {
		for (j = 0; j < ysize; j++) {
			for (n1 = xsize; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_43BR(sp[0], sp[1], sp[2],
				    sp[3], dp[0], dp[1], dp[2])
				    sp += 4;
				dp += 3;
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_43BR(sp[0], sp[1], sp[2],
				    sp[3], dp[0], dd1, dd2)
				    dp++;
				mmx_pst_32(dd1, dp, 2);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	} else {
		for (j = 0; j < ysize; j++) {
			for (n1 = xsize; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_43BL(sp[0], sp[1], sp[2],
				    sp[3], dp[0], dp[1], dp[2])
				    sp += 4;
				dp += 3;
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_43BL(sp[0], sp[1], sp[2],
				    sp[3], dp[0], dd1, dd2)
				    dp++;
				mmx_pst_32(dd1, dp, 2);
			}

			sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
			dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
		}
	}

	_mm_empty();
}

/* *********************************************************** */
