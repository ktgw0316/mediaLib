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

#pragma ident	"@(#)mlib_m_ImageConstAdd.c	9.6	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageConstAdd - add image with constant
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageConstAdd(mlib_image       *dst,
 *                                     const mlib_image *src,
 *                                     const mlib_s32   *c);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      c       array of constants to be added to each pixel
 *
 * RESTRICTION
 *      src, and dst must be the same type, the same size and the same
 *      number of channels. They can have 1, 2, 3 or 4 channels.
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      dst(i, j) = c + src(i, j).
 */

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <mmintrin.h>
#endif

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_mmx_utils.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageConstAdd = __mlib_ImageConstAdd

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageConstAdd) mlib_ImageConstAdd
    __attribute__((weak, alias("__mlib_ImageConstAdd")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_M_IMAGE_CONST_ADD_U8_S(dd, sd, cd)                 \
	dd = _mm_adds_pu8(cd, sd);

/* *********************************************************** */

#define	MLIB_M_IMAGE_CONST_ADD_U8(dd, sd, cd1, cd2)             \
	{                                                       \
	    __m64 s1 = _mm_unpacklo_pi8(sd, _m_zero);                 \
	    __m64 s2 = _mm_unpackhi_pi8(sd, _m_zero);                 \
	                                                        \
	    MLIB_M_IMAGE_CONST_ADD_S16(dd1, s1, cd1);           \
	    MLIB_M_IMAGE_CONST_ADD_S16(dd2, s2, cd2);           \
	    dd = _mm_packs_pu16(dd1, dd2);                      \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_CONST_ADD_S16(dd, sd, cd)                  \
	dd = _mm_add_pi16(cd, sd);

/* *********************************************************** */

#define	MLIB_M_IMAGE_CONST_ADD_U16(dd, sd, cd)                  \
	dd = _mm_adds_pu16(cd, sd);

/* *********************************************************** */

#define	MLIB_M_IMAGE_CONST_ADD_U16_S(dd, sd, cd_add, cd_sub)    \
	dd = _mm_adds_pu16(cd_add, sd);                         \
	dd = _mm_subs_pu16(dd, cd_sub)

/* *********************************************************** */

#define	MLIB_M_IMAGE_CONST_ADD_S32(dd, sd, cd)                  \
	dd = _mm_add_pi32(cd, sd);

/* *********************************************************** */

    static void mlib_m_ImageConstAdd_S32_124_A8D1X4(
    mlib_s32 *src,
    mlib_s32 *dst,
    mlib_s32 dsize,
    __m64 dc1,
    __m64 dc2)
{
/* aligned src address */
	__m64 *sp;

/* aligned dst address */
	__m64 *dp;

/* source data */
	__m64 sd1, sd2;

/* destination data */
	__m64 dd1, dd2;

	sp = (__m64 *) src;
	dp = (__m64 *) dst;

/* 4-pixel loop */
	for (; dsize > 0; dsize -= 4) {
		sd1 = (*sp++);
		sd2 = (*sp++);
		MLIB_M_IMAGE_CONST_ADD_S32(dd1, sd1, dc1);
		MLIB_M_IMAGE_CONST_ADD_S32(dd2, sd2, dc2);
		(*dp++) = dd1;
		(*dp++) = dd2;
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_S32_124_A8D2(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc1,
    __m64 dc2)
{
/* src address */
	__m64 *sp, *sl;

/* dst address */
	__m64 *dp, *dl;

/* source data */
	__m64 sd1, sd2;

/* destination data */
	__m64 dd1, dd2;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;

/* pointer to end of a dst line */
	mlib_s32 *dend;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

/* row loop */
	for (; ysize > 0; ysize--) {
		dend = (mlib_s32 *)dp + xsize;

/* 4-pixel column loop */
		for (n1 = xsize; n1 > 3; n1 -= 4) {
			sd1 = (*sp++);
			sd2 = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_S32(dd1, sd1, dc1);
			MLIB_M_IMAGE_CONST_ADD_S32(dd2, sd2, dc2);
			(*dp++) = dd1;
			(*dp++) = dd2;
		}

/* last 4-pixel column */
		if (n1 > 0) {
			emask = (n1 > 1) ? 3 : 2;
			sd1 = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_S32(dd1, sd1, dc1);
			mmx_pst_32(dd1, dp, emask);
			dp++;

			n1 = (mlib_s32 *)dend - (mlib_s32 *)dp;
			if (n1 > 0) {
				emask = (n1 > 1) ? 3 : 2;
				sd1 = *sp;
				MLIB_M_IMAGE_CONST_ADD_S32(dd1, sd1, dc2);
				mmx_pst_32(dd1, dp, emask);
			}
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_S32_124(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc1,
    __m64 dc2)
{
/* src address */
	__m64 *sp, *sl;

/* dst address */
	__m64 *dp, *dl;

/* source data */
	__m64 sd1, sd2;

/* destination data */
	__m64 dd1, dd2;

/* destination data */
	__m64 dc_1, dc_2, dc_3, dc_4;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;

/* pointer to end of a dst line */
	mlib_s32 *dend;

	dc_3 = _mm_or_si64(_mm_slli_si64(dc2, 32), _mm_srli_si64(dc1, 32));
	dc_4 = _mm_or_si64(_mm_slli_si64(dc1, 32), _mm_srli_si64(dc2, 32));

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

/* row loop */
	for (; ysize > 0; ysize--) {
		dc_1 = dc1;
		dc_2 = dc2;

		n1 = xsize;
		dend = (mlib_s32 *)dp + xsize;

		if ((mlib_addr)sp & 7) {
			sd1 = *sp;
			MLIB_M_IMAGE_CONST_ADD_S32(dd1, sd1, dc_1);
			mmx_pst_32(dd1, dp, 2);
			sp = (__m64 *) ((mlib_s32 *)sl + 1);
			dp = (__m64 *) ((mlib_s32 *)dl + 1);
			dc_1 = dc_3;
			dc_2 = dc_4;
			n1--;
		}

/* 4-pixel column loop */
		for (; n1 > 3; n1 -= 4) {
			sd1 = (*sp++);
			sd2 = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_S32(dd1, sd1, dc_1);
			MLIB_M_IMAGE_CONST_ADD_S32(dd2, sd2, dc_2);
			(*dp++) = dd1;
			(*dp++) = dd2;
		}

/* last 4-pixel column */
		if (n1 > 0) {
			emask = (n1 > 1) ? 3 : 2;
			sd1 = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_S32(dd1, sd1, dc_1);
			mmx_pst_32(dd1, dp, emask);
			dp++;

			n1 = (mlib_s32 *)dend - (mlib_s32 *)dp;
			if (n1 > 0) {
				emask = (n1 > 1) ? 3 : 2;
				sd1 = *sp;
				MLIB_M_IMAGE_CONST_ADD_S32(dd1, sd1, dc_2);
				mmx_pst_32(dd1, dp, emask);
			}
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_S32_3_A8D1(
    mlib_s32 *src,
    mlib_s32 *dst,
    mlib_s32 dsize,
    __m64 dc1,
    __m64 dc2,
    __m64 dc3)
{
/* aligned src address */
	__m64 *sp;

/* aligned dst address */
	__m64 *dp;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd;

/* loop variable */
	mlib_s32 n1;

	sp = (__m64 *) src;
	dp = (__m64 *) dst;

/* 6-pixel column loop */
	for (n1 = dsize; n1 > 5; n1 -= 6) {
		sd = sp[0];
		MLIB_M_IMAGE_CONST_ADD_S32(dp[0], sd, dc1);
		sd = sp[1];
		MLIB_M_IMAGE_CONST_ADD_S32(dp[1], sd, dc2);
		sd = sp[2];
		MLIB_M_IMAGE_CONST_ADD_S32(dp[2], sd, dc3);
		sp += 3;
		dp += 3;
	}

/* last 3-pixel column */
	if (n1 > 0) {
		sd = sp[0];
		MLIB_M_IMAGE_CONST_ADD_S32(dd, sd, dc1);
		mmx_pst_32(dd, dp, 3);
		sd = sp[1];
		dp++;
		MLIB_M_IMAGE_CONST_ADD_S32(dd, sd, dc2);
		mmx_pst_32(dd, dp, 2);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_S32_3_A8D2(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc1,
    __m64 dc2,
    __m64 dc3)
{
/* src address */
	__m64 *sp, *sl;

/* dst address */
	__m64 *dp, *dl;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd;
	mlib_s32 n1;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

/* row loop */
	for (; ysize > 0; ysize--) {
/* 6-pixel column loop */
		for (n1 = xsize; n1 > 5; n1 -= 6) {
			sd = sp[0];
			MLIB_M_IMAGE_CONST_ADD_S32(dp[0], sd, dc1);
			sd = sp[1];
			MLIB_M_IMAGE_CONST_ADD_S32(dp[1], sd, dc2);
			sd = sp[2];
			MLIB_M_IMAGE_CONST_ADD_S32(dp[2], sd, dc3);
			sp += 3;
			dp += 3;
		}

/* last 3-pixel column */
		if (n1 > 0) {
			sd = sp[0];
			MLIB_M_IMAGE_CONST_ADD_S32(dd, sd, dc1);
			mmx_pst_32(dd, dp, 3);
			sd = sp[1];
			dp++;
			MLIB_M_IMAGE_CONST_ADD_S32(dd, sd, dc2);
			mmx_pst_32(dd, dp, 2);
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_S32_3(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc1,
    __m64 dc2,
    __m64 dc3)
{
/* src address */
	__m64 *sp, *sl;

/* dst address */
	__m64 *dp, *dl;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd;
	__m64 dc_0, dc_1, dc_2, dc_3;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;

/* pointer to end of dst */
	mlib_s32 *dend;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

/* row loop */
	for (; ysize > 0; ysize--) {
		dc_1 = dc1;
		dc_2 = dc2;
		dc_3 = dc3;

		n1 = xsize;
		dend = (mlib_s32 *)dp + xsize;

		if ((mlib_addr)sp & 7) {
			sd = *sp;
			MLIB_M_IMAGE_CONST_ADD_S32(dd, sd, dc_1);
			mmx_pst_32(dd, dp, 2);
			sp = (__m64 *) ((mlib_s32 *)sl + 1);
			dp = (__m64 *) ((mlib_s32 *)dl + 1);
			dc_0 = dc_1;
			dc_1 = dc_3;
			dc_3 = dc_2;
			dc_2 = dc_0;
			n1--;
		}

/* 6-pixel column loop */
		for (; n1 > 5; n1 -= 6) {
			sd = sp[0];
			MLIB_M_IMAGE_CONST_ADD_S32(dp[0], sd, dc_1);
			sd = sp[1];
			MLIB_M_IMAGE_CONST_ADD_S32(dp[1], sd, dc_2);
			sd = sp[2];
			MLIB_M_IMAGE_CONST_ADD_S32(dp[2], sd, dc_3);
			sp += 3;
			dp += 3;
		}

/* last 4-pixel column */
		if (n1 > 0) {
			emask = (n1 > 1) ? 3 : 2;
			sd = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_S32(dd, sd, dc_1);
			mmx_pst_32(dd, dp, emask);
			dp++;

			n1 = (mlib_s32 *)dend - (mlib_s32 *)dp;
			if (n1 > 0) {
				emask = (n1 > 1) ? 3 : 2;
				sd = (*sp++);
				MLIB_M_IMAGE_CONST_ADD_S32(dd, sd, dc_2);
				mmx_pst_32(dd, dp, emask);
				dp++;

				n1 = (mlib_s32 *)dend - (mlib_s32 *)dp;
				if (n1 > 0) {
					sd = *sp;
					MLIB_M_IMAGE_CONST_ADD_S32(dd, sd,
					    dc_3);
					mmx_pst_32(dd, dp, 2);
				}

			}

		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_S16_124_A8D1X4(
    mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    __m64 dc)
{
/* aligned src address */
	__m64 *sp;

/* aligned dst address */
	__m64 *dp;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd;

	sp = (__m64 *) src;
	dp = (__m64 *) dst;

/* 4-pixel loop */
	for (; dsize > 0; dsize -= 4) {
		sd = (*sp++);
		MLIB_M_IMAGE_CONST_ADD_S16(dd, sd, dc);
		(*dp++) = dd;
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_S16_124_A8D2(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc)
{
/* src address */
	__m64 *sp, *sl;

/* dst address */
	__m64 *dp, *dl;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

/* row loop */
	for (; ysize > 0; ysize--) {
/* 4-pixel column loop */
		for (n1 = xsize; n1 > 3; n1 -= 4) {
			sd = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_S16(dd, sd, dc);
			(*dp++) = dd;
		}

/* last 4-pixel column */
		if (n1 > 0) {
			emask = 0xF << (4 - n1);
			sd = *sp;
			MLIB_M_IMAGE_CONST_ADD_S16(dd, sd, dc);
			mmx_pst_16(dd, dp, emask);
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_S16_124(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc)
{
/* src address */
	__m64 *sp, *sl;

/* dst address */
	__m64 *dp, *dl;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd;
	__m64 dc_1;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1, nu;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

/* row loop */
	for (; ysize > 0; ysize--) {
		dc_1 = dc;
		n1 = xsize;

		nu = ((8 - (mlib_addr)sp) & 7) >> 1;
		if ((nu > 0) && (nu < n1)) {
			mlib_s32 nr = 4 - nu;

			emask = 0xF << nr;
			sd = *sp;
			MLIB_M_IMAGE_CONST_ADD_S16(dd, sd, dc_1);
			mmx_pst_16(dd, dp, emask);
			sp = (__m64 *) ((mlib_s16 *)sl + nu);
			dp = (__m64 *) ((mlib_s16 *)dl + nu);
			dc_1 =
			    _mm_or_si64(_mm_slli_si64(dc_1, nr << 4),
			    _mm_srli_si64(dc_1, nu << 4));
			n1 -= nu;
		}

/* 4-pixel column loop */
		for (; n1 > 3; n1 -= 4) {
			sd = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_S16(dd, sd, dc_1);
			(*dp++) = dd;
		}

/* last 4-pixel column */
		if (n1 > 0) {
			emask = 0xF << (4 - n1);
			sd = *sp;
			MLIB_M_IMAGE_CONST_ADD_S16(dd, sd, dc_1);
			mmx_pst_16(dd, dp, emask);
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_S16_3_A8D1(
    mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    __m64 dc1,
    __m64 dc2,
    __m64 dc3)
{
/* aligned src address */
	__m64 *sp;

/* aligned dst address */
	__m64 *dp;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd;

/* edge mask */
	mlib_s32 emask;

/* loop variable */
	mlib_s32 n1;

/* pointer to end of dst */
	mlib_s16 *dend;

	sp = (__m64 *) src;
	dp = (__m64 *) dst;

	dend = (mlib_s16 *)dp + dsize;

/* 4-pixel column loop */
	for (n1 = dsize; n1 > 11; n1 -= 12) {
		sd = sp[0];
		MLIB_M_IMAGE_CONST_ADD_S16(dp[0], sd, dc1);
		sd = sp[1];
		MLIB_M_IMAGE_CONST_ADD_S16(dp[1], sd, dc2);
		sd = sp[2];
		MLIB_M_IMAGE_CONST_ADD_S16(dp[2], sd, dc3);
		sp += 3;
		dp += 3;
	}

	if (n1 > 0) {
		emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
		sd = (*sp++);
		MLIB_M_IMAGE_CONST_ADD_S16(dd, sd, dc1);
		mmx_pst_16(dd, dp, emask);
		dp++;

		n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp);
		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			sd = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_S16(dd, sd, dc2);
			mmx_pst_16(dd, dp, emask);
			dp++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp);
			if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				sd = *sp;
				MLIB_M_IMAGE_CONST_ADD_S16(dd, sd, dc3);
				mmx_pst_16(dd, dp, emask);
			}
		}
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_S16_3_A8D2(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc1,
    __m64 dc2,
    __m64 dc3)
{
/* aligned src address */
	__m64 *sp, *sl;

/* aligned dst address */
	__m64 *dp, *dl;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;

/* pointer to end of dst */
	mlib_s16 *dend;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

/* row loop */
	for (; ysize > 0; ysize--) {
		dend = (mlib_s16 *)dp + xsize;

/* 4-pixel column loop */
		for (n1 = xsize; n1 > 11; n1 -= 12) {
			sd = sp[0];
			MLIB_M_IMAGE_CONST_ADD_S16(dp[0], sd, dc1);
			sd = sp[1];
			MLIB_M_IMAGE_CONST_ADD_S16(dp[1], sd, dc2);
			sd = sp[2];
			MLIB_M_IMAGE_CONST_ADD_S16(dp[2], sd, dc3);
			sp += 3;
			dp += 3;
		}

/* last 4-pixel column loop */
		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			sd = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_S16(dd, sd, dc1);
			mmx_pst_16(dd, dp, emask);
			dp++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp);
			if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				sd = (*sp++);
				MLIB_M_IMAGE_CONST_ADD_S16(dd, sd, dc2);
				mmx_pst_16(dd, dp, emask);
				dp++;

				n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp);
				if (n1 > 0) {
					emask =
					    (n1 > 3) ? 0xF : (0xF << (4 - n1));
					sd = *sp;
					MLIB_M_IMAGE_CONST_ADD_S16(dd, sd, dc3);
					mmx_pst_16(dd, dp, emask);
				}
			}
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_S16_3(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc1,
    __m64 dc2,
    __m64 dc3)
{
	__m64 c[6];

/* aligned src address */
	__m64 *sp, *sl;

/* aligned dst address */
	__m64 *dp, *dl;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd;
	__m64 dc_1, dc_2, dc_3;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1, nu;

/* pointer to end of dst */
	mlib_s16 *dend;

	c[0] = c[3] = dc2;
	c[1] = c[4] = dc3;
	c[2] = c[5] = dc1;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

/* row loop */
	for (; ysize > 0; ysize--) {
		dc_1 = dc1;
		dc_2 = dc2;
		dc_3 = dc3;

		n1 = xsize;
		dend = (mlib_s16 *)dp + xsize;

		nu = ((8 - (mlib_addr)sp) & 7) >> 1;
		if ((nu > 0) && (nu < n1)) {
			mlib_s32 nr = 4 - nu;

			emask = 0xF << nr;
			sd = *sp;
			MLIB_M_IMAGE_CONST_ADD_S16(dd, sd, dc_1);
			mmx_pst_16(dd, dp, emask);
			sp = (__m64 *) ((mlib_s16 *)sl + nu);
			dp = (__m64 *) ((mlib_s16 *)dl + nu);
			n1 -= nu;
			dc_1 = c[nu - 1];
			dc_2 = c[nu + 0];
			dc_3 = c[nu + 1];
		}

/* 4-pixel column loop */
		for (; n1 > 11; n1 -= 12) {
			sd = sp[0];
			MLIB_M_IMAGE_CONST_ADD_S16(dp[0], sd, dc_1);
			sd = sp[1];
			MLIB_M_IMAGE_CONST_ADD_S16(dp[1], sd, dc_2);
			sd = sp[2];
			MLIB_M_IMAGE_CONST_ADD_S16(dp[2], sd, dc_3);
			sp += 3;
			dp += 3;
		}

/* last 4-pixel column loop */
		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			sd = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_S16(dd, sd, dc_1);
			mmx_pst_16(dd, dp, emask);
			dp++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp);
			if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				sd = (*sp++);
				MLIB_M_IMAGE_CONST_ADD_S16(dd, sd, dc_2);
				mmx_pst_16(dd, dp, emask);
				dp++;

				n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp);
				if (n1 > 0) {
					emask =
					    (n1 > 3) ? 0xF : (0xF << (4 - n1));
					sd = *sp;
					MLIB_M_IMAGE_CONST_ADD_S16(dd, sd,
					    dc_3);
					mmx_pst_16(dd, dp, emask);
				}
			}
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_U16_124_A8D2(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc)
{
/* src address */
	__m64 *sp, *sl;

/* dst address */
	__m64 *dp, *dl;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

/* row loop */
	for (; ysize > 0; ysize--) {
/* 4-pixel column loop */
		for (n1 = xsize; n1 > 3; n1 -= 4) {
			sd = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_U16(dd, sd, dc);
			(*dp++) = dd;
		}

/* last 4-pixel column */
		if (n1 > 0) {
			emask = 0xF << (4 - n1);
			sd = *sp;
			MLIB_M_IMAGE_CONST_ADD_U16(dd, sd, dc);
			mmx_pst_16(dd, dp, emask);
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_U16_124(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc)
{
/* src address */
	__m64 *sp, *sl;

/* dst address */
	__m64 *dp, *dl;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd;
	__m64 dc_1;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1, nu;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

/* row loop */
	for (; ysize > 0; ysize--) {
		dc_1 = dc;
		n1 = xsize;

		nu = ((8 - (mlib_addr)sp) & 7) >> 1;
		if ((nu > 0) && (nu < n1)) {
			mlib_s32 nr = 4 - nu;

			emask = 0xF << nr;
			sd = *sp;
			MLIB_M_IMAGE_CONST_ADD_U16(dd, sd, dc_1);
			mmx_pst_16(dd, dp, emask);
			sp = (__m64 *) ((mlib_s16 *)sl + nu);
			dp = (__m64 *) ((mlib_s16 *)dl + nu);
			dc_1 =
			    _mm_or_si64(_mm_slli_si64(dc_1, nr << 4),
			    _mm_srli_si64(dc_1, nu << 4));
			n1 -= nu;
		}

/* 4-pixel column loop */
		for (; n1 > 3; n1 -= 4) {
			sd = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_U16(dd, sd, dc_1);
			(*dp++) = dd;
		}

/* last 4-pixel column */
		if (n1 > 0) {
			emask = 0xF << (4 - n1);
			sd = *sp;
			MLIB_M_IMAGE_CONST_ADD_U16(dd, sd, dc_1);
			mmx_pst_16(dd, dp, emask);
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_U16_3_A8D2(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc1,
    __m64 dc2,
    __m64 dc3)
{
/* aligned src address */
	__m64 *sp, *sl;

/* aligned dst address */
	__m64 *dp, *dl;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;

/* pointer to end of dst */
	mlib_s16 *dend;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

/* row loop */
	for (; ysize > 0; ysize--) {
		dend = (mlib_s16 *)dp + xsize;

/* 4-pixel column loop */
		for (n1 = xsize; n1 > 11; n1 -= 12) {
			sd = sp[0];
			MLIB_M_IMAGE_CONST_ADD_U16(dp[0], sd, dc1);
			sd = sp[1];
			MLIB_M_IMAGE_CONST_ADD_U16(dp[1], sd, dc2);
			sd = sp[2];
			MLIB_M_IMAGE_CONST_ADD_U16(dp[2], sd, dc3);
			sp += 3;
			dp += 3;
		}

/* last 4-pixel column loop */
		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			sd = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_U16(dd, sd, dc1);
			mmx_pst_16(dd, dp, emask);
			dp++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp);
			if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				sd = (*sp++);
				MLIB_M_IMAGE_CONST_ADD_U16(dd, sd, dc2);
				mmx_pst_16(dd, dp, emask);
				dp++;

				n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp);
				if (n1 > 0) {
					emask =
					    (n1 > 3) ? 0xF : (0xF << (4 - n1));
					sd = *sp;
					MLIB_M_IMAGE_CONST_ADD_U16(dd, sd, dc3);
					mmx_pst_16(dd, dp, emask);
				}
			}
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_U16_3(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc1,
    __m64 dc2,
    __m64 dc3)
{
	__m64 c[6];

/* aligned src address */
	__m64 *sp, *sl;

/* aligned dst address */
	__m64 *dp, *dl;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd;
	__m64 dc_1, dc_2, dc_3;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1, nu;

/* pointer to end of dst */
	mlib_s16 *dend;

	c[0] = c[3] = dc2;
	c[1] = c[4] = dc3;
	c[2] = c[5] = dc1;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

/* row loop */
	for (; ysize > 0; ysize--) {
		dc_1 = dc1;
		dc_2 = dc2;
		dc_3 = dc3;

		n1 = xsize;
		dend = (mlib_s16 *)dp + xsize;

		nu = ((8 - (mlib_addr)sp) & 7) >> 1;
		if ((nu > 0) && (nu < n1)) {
			mlib_s32 nr = 4 - nu;

			emask = 0xF << nr;
			sd = *sp;
			MLIB_M_IMAGE_CONST_ADD_U16(dd, sd, dc_1);
			mmx_pst_16(dd, dp, emask);
			sp = (__m64 *) ((mlib_s16 *)sl + nu);
			dp = (__m64 *) ((mlib_s16 *)dl + nu);
			n1 -= nu;
			dc_1 = c[nu - 1];
			dc_2 = c[nu + 0];
			dc_3 = c[nu + 1];
		}

/* 4-pixel column loop */
		for (; n1 > 11; n1 -= 12) {
			sd = sp[0];
			MLIB_M_IMAGE_CONST_ADD_U16(dp[0], sd, dc_1);
			sd = sp[1];
			MLIB_M_IMAGE_CONST_ADD_U16(dp[1], sd, dc_2);
			sd = sp[2];
			MLIB_M_IMAGE_CONST_ADD_U16(dp[2], sd, dc_3);
			sp += 3;
			dp += 3;
		}

/* last 4-pixel column loop */
		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			sd = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_U16(dd, sd, dc_1);
			mmx_pst_16(dd, dp, emask);
			dp++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp);
			if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				sd = (*sp++);
				MLIB_M_IMAGE_CONST_ADD_U16(dd, sd, dc_2);
				mmx_pst_16(dd, dp, emask);
				dp++;

				n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp);
				if (n1 > 0) {
					emask =
					    (n1 > 3) ? 0xF : (0xF << (4 - n1));
					sd = *sp;
					MLIB_M_IMAGE_CONST_ADD_U16(dd, sd,
					    dc_3);
					mmx_pst_16(dd, dp, emask);
				}
			}
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_U16_124_A8D2_S(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc_add,
    __m64 dc_sub)
{
/* src address */
	__m64 *sp, *sl;

/* dst address */
	__m64 *dp, *dl;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

/* row loop */
	for (; ysize > 0; ysize--) {
/* 4-pixel column loop */
		for (n1 = xsize; n1 > 3; n1 -= 4) {
			sd = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_U16_S(dd, sd, dc_add, dc_sub);
			(*dp++) = dd;
		}

/* last 4-pixel column */
		if (n1 > 0) {
			emask = 0xF << (4 - n1);
			sd = *sp;
			MLIB_M_IMAGE_CONST_ADD_U16_S(dd, sd, dc_add, dc_sub);
			mmx_pst_16(dd, dp, emask);
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_U16_124_S(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc_add,
    __m64 dc_sub)
{
/* src address */
	__m64 *sp, *sl;

/* dst address */
	__m64 *dp, *dl;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd;
	__m64 dc_add1, dc_sub1;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1, nu;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

/* row loop */
	for (; ysize > 0; ysize--) {
		dc_add1 = dc_add;
		dc_sub1 = dc_sub;
		n1 = xsize;

		nu = ((8 - (mlib_addr)sp) & 7) >> 1;
		if ((nu > 0) && (nu < n1)) {
			mlib_s32 nr = 4 - nu;

			emask = 0xF << nr;
			sd = *sp;
			MLIB_M_IMAGE_CONST_ADD_U16_S(dd, sd, dc_add1, dc_sub1);
			mmx_pst_16(dd, dp, emask);
			sp = (__m64 *) ((mlib_s16 *)sl + nu);
			dp = (__m64 *) ((mlib_s16 *)dl + nu);
			dc_add1 =
			    _mm_or_si64(_mm_slli_si64(dc_add1, nr << 4),
			    _mm_srli_si64(dc_add1, nu << 4));
			dc_sub1 =
			    _mm_or_si64(_mm_slli_si64(dc_sub1, nr << 4),
			    _mm_srli_si64(dc_sub1, nu << 4));
			n1 -= nu;
		}

/* 4-pixel column loop */
		for (; n1 > 3; n1 -= 4) {
			sd = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_U16_S(dd, sd, dc_add1, dc_sub1);
			(*dp++) = dd;
		}

/* last 4-pixel column */
		if (n1 > 0) {
			emask = 0xF << (4 - n1);
			sd = *sp;
			MLIB_M_IMAGE_CONST_ADD_U16_S(dd, sd, dc_add1, dc_sub1);
			mmx_pst_16(dd, dp, emask);
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_U16_3_A8D2_S(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc_add1,
    __m64 dc_add2,
    __m64 dc_add3,
    __m64 dc_sub1,
    __m64 dc_sub2,
    __m64 dc_sub3)
{
/* aligned src address */
	__m64 *sp, *sl;

/* aligned dst address */
	__m64 *dp, *dl;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;

/* pointer to end of dst */
	mlib_s16 *dend;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

/* row loop */
	for (; ysize > 0; ysize--) {
		dend = (mlib_s16 *)dp + xsize;

/* 4-pixel column loop */
		for (n1 = xsize; n1 > 11; n1 -= 12) {
			sd = sp[0];
			MLIB_M_IMAGE_CONST_ADD_U16_S(dp[0], sd, dc_add1,
			    dc_sub1);
			sd = sp[1];
			MLIB_M_IMAGE_CONST_ADD_U16_S(dp[1], sd, dc_add2,
			    dc_sub2);
			sd = sp[2];
			MLIB_M_IMAGE_CONST_ADD_U16_S(dp[2], sd, dc_add3,
			    dc_sub3);
			sp += 3;
			dp += 3;
		}

/* last 4-pixel column loop */
		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			sd = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_U16_S(dd, sd, dc_add1, dc_sub1);
			mmx_pst_16(dd, dp, emask);
			dp++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp);
			if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				sd = (*sp++);
				MLIB_M_IMAGE_CONST_ADD_U16_S(dd, sd, dc_add2,
				    dc_sub2);
				mmx_pst_16(dd, dp, emask);
				dp++;

				n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp);
				if (n1 > 0) {
					emask =
					    (n1 > 3) ? 0xF : (0xF << (4 - n1));
					sd = *sp;
					MLIB_M_IMAGE_CONST_ADD_U16_S(dd, sd,
					    dc_add3, dc_sub3);
					mmx_pst_16(dd, dp, emask);
				}
			}
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_U16_3_S(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc_add1,
    __m64 dc_add2,
    __m64 dc_add3,
    __m64 dc_sub1,
    __m64 dc_sub2,
    __m64 dc_sub3)
{
	__m64 c_add[6], c_sub[6];

/* aligned src address */
	__m64 *sp, *sl;

/* aligned dst address */
	__m64 *dp, *dl;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1, nu;

/* pointer to end of dst */
	mlib_s16 *dend;

	c_add[0] = c_add[3] = dc_add2;
	c_add[1] = c_add[4] = dc_add3;
	c_add[2] = c_add[5] = dc_add1;
	c_sub[0] = c_sub[3] = dc_sub2;
	c_sub[1] = c_sub[4] = dc_sub3;
	c_sub[2] = c_sub[5] = dc_sub1;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

/* row loop */
	for (; ysize > 0; ysize--) {
		dc_add1 = c_add[2];
		dc_add2 = c_add[3];
		dc_add3 = c_add[4];
		dc_sub1 = c_sub[2];
		dc_sub2 = c_sub[3];
		dc_sub3 = c_sub[4];

		n1 = xsize;
		dend = (mlib_s16 *)dp + xsize;

		nu = ((8 - (mlib_addr)sp) & 7) >> 1;
		if ((nu > 0) && (nu < n1)) {
			mlib_s32 nr = 4 - nu;

			emask = 0xF << nr;
			sd = *sp;
			MLIB_M_IMAGE_CONST_ADD_U16_S(dd, sd, dc_add1, dc_sub1);
			mmx_pst_16(dd, dp, emask);
			sp = (__m64 *) ((mlib_s16 *)sl + nu);
			dp = (__m64 *) ((mlib_s16 *)dl + nu);
			n1 -= nu;
			dc_add1 = c_add[nu - 1];
			dc_add2 = c_add[nu + 0];
			dc_add3 = c_add[nu + 1];
			dc_sub1 = c_sub[nu - 1];
			dc_sub2 = c_sub[nu + 0];
			dc_sub3 = c_sub[nu + 1];
		}

/* 4-pixel column loop */
		for (; n1 > 11; n1 -= 12) {
			sd = sp[0];
			MLIB_M_IMAGE_CONST_ADD_U16_S(dp[0], sd, dc_add1,
			    dc_sub1);
			sd = sp[1];
			MLIB_M_IMAGE_CONST_ADD_U16_S(dp[1], sd, dc_add2,
			    dc_sub2);
			sd = sp[2];
			MLIB_M_IMAGE_CONST_ADD_U16_S(dp[2], sd, dc_add3,
			    dc_sub3);
			sp += 3;
			dp += 3;
		}

/* last 4-pixel column loop */
		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			sd = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_U16_S(dd, sd, dc_add1, dc_sub1);
			mmx_pst_16(dd, dp, emask);
			dp++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp);
			if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				sd = (*sp++);
				MLIB_M_IMAGE_CONST_ADD_U16_S(dd, sd, dc_add2,
				    dc_sub2);
				mmx_pst_16(dd, dp, emask);
				dp++;

				n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp);
				if (n1 > 0) {
					emask =
					    (n1 > 3) ? 0xF : (0xF << (4 - n1));
					sd = *sp;
					MLIB_M_IMAGE_CONST_ADD_U16_S(dd, sd,
					    dc_add3, dc_sub3);
					mmx_pst_16(dd, dp, emask);
				}
			}
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_U8_124_A8D1X8(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    __m64 dc)
{
/* aligned src address */
	__m64 *sp;

/* aligned dst address */
	__m64 *dp;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd, dd1, dd2;

	sp = (__m64 *) src;
	dp = (__m64 *) dst;

/* 8-pixel loop */
	for (; dsize > 0; dsize -= 8) {
		sd = (*sp++);
		MLIB_M_IMAGE_CONST_ADD_U8(dd, sd, dc, dc);
		(*dp++) = dd;
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_U8_124_A8D2(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc)
{
/* src address */
	__m64 *sp, *sl;

/* dst address */
	__m64 *dp, *dl;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd, dd1, dd2;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

/* row loop */
	for (; ysize > 0; ysize--) {
/* 8-pixel column loop */
		for (n1 = xsize; n1 > 7; n1 -= 8) {
			sd = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_U8(dd, sd, dc, dc);
			(*dp++) = dd;
		}

/* last 8-pixel column */
		if (n1 > 0) {
			emask = 0xFF << (8 - n1);
			sd = *sp;
			MLIB_M_IMAGE_CONST_ADD_U8(dd, sd, dc, dc)
			    mmx_pst_8(dd, dp, emask);
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_U8_124(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc)
{
/* src address */
	__m64 *sp, *sl;

/* dst address */
	__m64 *dp, *dl;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd, dd1, dd2;

/* destination data */
	__m64 dc_1;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1, nu;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

/* row loop */
	for (; ysize > 0; ysize--) {
		dc_1 = dc;
		n1 = xsize;

		nu = ((8 - (mlib_addr)sp) & 7);
		if ((nu > 0) && (nu < n1)) {
			mlib_s32 nr = 8 - nu;

			emask = 0xFF << nr;
			sd = *sp;
			MLIB_M_IMAGE_CONST_ADD_U8(dd, sd, dc_1, dc_1)
			    mmx_pst_8(dd, dp, emask);
			sp = (__m64 *) ((mlib_u8 *)sl + nu);
			dp = (__m64 *) ((mlib_u8 *)dl + nu);
			dc_1 =
			    _mm_or_si64(_mm_slli_si64(dc_1, (nr & 3) << 4),
			    _mm_srli_si64(dc_1, (nu & 3) << 4));
			n1 -= nu;
		}

/* 8-pixel column loop */
		for (; n1 > 7; n1 -= 8) {
			sd = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_U8(dd, sd, dc_1, dc_1);
			(*dp++) = dd;
		}

/* last 8-pixel column */
		if (n1 > 0) {
			emask = 0xFF << (8 - n1);
			sd = *sp;
			MLIB_M_IMAGE_CONST_ADD_U8(dd, sd, dc_1, dc_1)
			    mmx_pst_8(dd, dp, emask);
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_U8_3_A8D1(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    __m64 dc1,
    __m64 dc2,
    __m64 dc3)
{
/* aligned src address */
	__m64 *sp;

/* aligned dst address */
	__m64 *dp;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd, dd1, dd2;

/* edge mask */
	mlib_s32 emask;

/* loop variable */
	mlib_s32 n1;

/* pointer to end of dst */
	mlib_u8 *dend;

	sp = (__m64 *) src;
	dp = (__m64 *) dst;

	dend = (mlib_u8 *)dp + dsize;

/* 8-pixel column loop */
	for (n1 = dsize; n1 > 23; n1 -= 24) {
		sd = sp[0];
		MLIB_M_IMAGE_CONST_ADD_U8(dp[0], sd, dc1, dc2);
		sd = sp[1];
		MLIB_M_IMAGE_CONST_ADD_U8(dp[1], sd, dc3, dc1);
		sd = sp[2];
		MLIB_M_IMAGE_CONST_ADD_U8(dp[2], sd, dc2, dc3);
		sp += 3;
		dp += 3;
	}

	if (n1 > 0) {
		emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
		sd = (*sp++);
		MLIB_M_IMAGE_CONST_ADD_U8(dd, sd, dc1, dc2);
		mmx_pst_8(dd, dp, emask);
		dp++;

		n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp);
		if (n1 > 0) {
			emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
			sd = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_U8(dd, sd, dc3, dc1);
			mmx_pst_8(dd, dp, emask);
			dp++;

			n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp);
			if (n1 > 0) {
				emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
				sd = *sp;
				MLIB_M_IMAGE_CONST_ADD_U8(dd, sd, dc2, dc3);
				mmx_pst_8(dd, dp, emask);
			}
		}
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_U8_3_A8D2(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc1,
    __m64 dc2,
    __m64 dc3)
{
/* aligned src address */
	__m64 *sp, *sl;

/* aligned dst address */
	__m64 *dp, *dl;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd, dd1, dd2;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;

/* pointer to end of dst */
	mlib_u8 *dend;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

/* row loop */
	for (; ysize > 0; ysize--) {
		dend = (mlib_u8 *)dp + xsize;

/* 8-pixel column loop */
		for (n1 = xsize; n1 > 23; n1 -= 24) {
			sd = sp[0];
			MLIB_M_IMAGE_CONST_ADD_U8(dp[0], sd, dc1, dc2);
			sd = sp[1];
			MLIB_M_IMAGE_CONST_ADD_U8(dp[1], sd, dc3, dc1);
			sd = sp[2];
			MLIB_M_IMAGE_CONST_ADD_U8(dp[2], sd, dc2, dc3);
			sp += 3;
			dp += 3;
		}

		if (n1 > 0) {
			emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
			sd = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_U8(dd, sd, dc1, dc2);
			mmx_pst_8(dd, dp, emask);
			dp++;

			n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp);
			if (n1 > 0) {
				emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
				sd = (*sp++);
				MLIB_M_IMAGE_CONST_ADD_U8(dd, sd, dc3, dc1);
				mmx_pst_8(dd, dp, emask);
				dp++;

				n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp);
				if (n1 > 0) {
					emask =
					    (n1 >
					    7) ? 0xFF : (0xFF << (8 - n1));
					sd = *sp;
					MLIB_M_IMAGE_CONST_ADD_U8(dd, sd, dc2,
					    dc3);
					mmx_pst_8(dd, dp, emask);
				}
			}
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_U8_3(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc1,
    __m64 dc2,
    __m64 dc3)
{
	__m64 c[9];

/* aligned src address */
	__m64 *sp, *sl;

/* aligned dst address */
	__m64 *dp, *dl;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd, dd1, dd2;
	__m64 dc_1, dc_2, dc_3;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1, nu;

/* pointer to end of dst */
	mlib_u8 *dend;

	c[0] = c[3] = c[6] = dc2;
	c[1] = c[4] = c[7] = dc3;
	c[2] = c[5] = c[8] = dc1;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

/* row loop */
	for (; ysize > 0; ysize--) {
		dc_1 = dc1;
		dc_2 = dc2;
		dc_3 = dc3;

		n1 = xsize;
		dend = (mlib_u8 *)dp + xsize;

		nu = ((8 - (mlib_addr)sp) & 7);
		if ((nu > 0) && (nu < n1)) {
			mlib_s32 nr = 8 - nu;

			emask = 0xFF << nr;
			sd = *sp;
			MLIB_M_IMAGE_CONST_ADD_U8(dd, sd, dc_1, dc_2);
			mmx_pst_8(dd, dp, emask);
			sp = (__m64 *) ((mlib_u8 *)sl + nu);
			dp = (__m64 *) ((mlib_u8 *)dl + nu);
			n1 -= nu;
			dc_1 = c[nu - 1];
			dc_2 = c[nu + 0];
			dc_3 = c[nu + 1];
		}

/* 8-pixel column loop */

		for (; n1 > 23; n1 -= 24) {
			sd = sp[0];
			MLIB_M_IMAGE_CONST_ADD_U8(dp[0], sd, dc_1, dc_2);
			sd = sp[1];
			MLIB_M_IMAGE_CONST_ADD_U8(dp[1], sd, dc_3, dc_1);
			sd = sp[2];
			MLIB_M_IMAGE_CONST_ADD_U8(dp[2], sd, dc_2, dc_3);
			sp += 3;
			dp += 3;
		}

		if (n1 > 0) {
			emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
			sd = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_U8(dd, sd, dc_1, dc_2);
			mmx_pst_8(dd, dp, emask);
			dp++;

			n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp);
			if (n1 > 0) {
				emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
				sd = (*sp++);
				MLIB_M_IMAGE_CONST_ADD_U8(dd, sd, dc_3, dc_1);
				mmx_pst_8(dd, dp, emask);
				dp++;

				n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp);
				if (n1 > 0) {
					emask =
					    (n1 >
					    7) ? 0xFF : (0xFF << (8 - n1));
					sd = *sp;
					MLIB_M_IMAGE_CONST_ADD_U8(dd, sd, dc_2,
					    dc_3);
					mmx_pst_8(dd, dp, emask);
				}
			}
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_U8_124_A8D1X8_S(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    __m64 dc)
{
/* aligned src address */
	__m64 *sp;

/* aligned dst address */
	__m64 *dp;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd;

	sp = (__m64 *) src;
	dp = (__m64 *) dst;

/* 8-pixel loop */
	for (; dsize > 0; dsize -= 8) {
		sd = (*sp++);
		MLIB_M_IMAGE_CONST_ADD_U8_S(dd, sd, dc);
		(*dp++) = dd;
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_U8_124_A8D2_S(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc)
{
/* src address */
	__m64 *sp, *sl;

/* dst address */
	__m64 *dp, *dl;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

/* row loop */
	for (; ysize > 0; ysize--) {
/* 8-pixel column loop */
		for (n1 = xsize; n1 > 7; n1 -= 8) {
			sd = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_U8_S(dd, sd, dc);
			(*dp++) = dd;
		}

/* last 8-pixel column */
		if (n1 > 0) {
			emask = (0xFF << (8 - n1));
			sd = *sp;
			MLIB_M_IMAGE_CONST_ADD_U8_S(dd, sd, dc)
			    mmx_pst_8(dd, dp, emask);
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_U8_124_S(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc)
{
/* src address */
	__m64 *sp, *sl;

/* dst address */
	__m64 *dp, *dl;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd;
	__m64 dc_1;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1, nu;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

/* row loop */
	for (; ysize > 0; ysize--) {
		dc_1 = dc;
		n1 = xsize;

		nu = ((8 - (mlib_addr)sp) & 7);
		if ((nu > 0) && (nu < n1)) {
			mlib_s32 nr = 8 - nu;

			emask = 0xFF << nr;
			sd = *sp;
			MLIB_M_IMAGE_CONST_ADD_U8_S(dd, sd, dc_1)
			    mmx_pst_8(dd, dp, emask);
			sp = (__m64 *) ((mlib_u8 *)sl + nu);
			dp = (__m64 *) ((mlib_u8 *)dl + nu);
			dc_1 =
			    _mm_or_si64(_mm_slli_si64(dc_1, (nr & 3) << 3),
			    _mm_srli_si64(dc_1, (nu & 3) << 3));
			n1 -= nu;
		}

/* 8-pixel column loop */
		for (; n1 > 7; n1 -= 8) {
			sd = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_U8_S(dd, sd, dc_1);
			(*dp++) = dd;
		}

/* last 8-pixel column */
		if (n1 > 0) {
			emask = (0xFF << (8 - n1));
			sd = *sp;
			MLIB_M_IMAGE_CONST_ADD_U8_S(dd, sd, dc_1)
			    mmx_pst_8(dd, dp, emask);
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_U8_3_A8D1_S(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    __m64 dc1,
    __m64 dc2,
    __m64 dc3)
{
/* aligned src address */
	__m64 *sp;

/* aligned dst address */
	__m64 *dp;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd;

/* edge mask */
	mlib_s32 emask;

/* loop variable */
	mlib_s32 n1;

/* pointer to end of dst */
	mlib_u8 *dend;

	sp = (__m64 *) src;
	dp = (__m64 *) dst;

	dend = (mlib_u8 *)dp + dsize;

/* 8-pixel column loop */
	for (n1 = dsize; n1 > 23; n1 -= 24) {
		sd = sp[0];
		MLIB_M_IMAGE_CONST_ADD_U8_S(dp[0], sd, dc1);
		sd = sp[1];
		MLIB_M_IMAGE_CONST_ADD_U8_S(dp[1], sd, dc2);
		sd = sp[2];
		MLIB_M_IMAGE_CONST_ADD_U8_S(dp[2], sd, dc3);
		sp += 3;
		dp += 3;
	}

	if (n1 > 0) {
		emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
		sd = (*sp++);
		MLIB_M_IMAGE_CONST_ADD_U8_S(dd, sd, dc1);
		mmx_pst_8(dd, dp, emask);
		dp++;

		n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp);
		if (n1 > 0) {
			emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
			sd = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_U8_S(dd, sd, dc2);
			mmx_pst_8(dd, dp, emask);
			dp++;

			n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp);
			if (n1 > 0) {
				emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
				sd = *sp;
				MLIB_M_IMAGE_CONST_ADD_U8_S(dd, sd, dc3);
				mmx_pst_8(dd, dp, emask);
			}
		}
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_U8_3_A8D2_S(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc1,
    __m64 dc2,
    __m64 dc3)
{
/* aligned src address */
	__m64 *sp, *sl;

/* aligned dst address */
	__m64 *dp, *dl;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;

/* pointer to end of dst */
	mlib_u8 *dend;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

/* row loop */
	for (; ysize > 0; ysize--) {
		dend = (mlib_u8 *)dp + xsize;

/* 8-pixel column loop */
		for (n1 = xsize; n1 > 23; n1 -= 24) {
			sd = sp[0];
			MLIB_M_IMAGE_CONST_ADD_U8_S(dp[0], sd, dc1);
			sd = sp[1];
			MLIB_M_IMAGE_CONST_ADD_U8_S(dp[1], sd, dc2);
			sd = sp[2];
			MLIB_M_IMAGE_CONST_ADD_U8_S(dp[2], sd, dc3);
			sp += 3;
			dp += 3;
		}

		if (n1 > 0) {
			emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
			sd = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_U8_S(dd, sd, dc1);
			mmx_pst_8(dd, dp, emask);
			dp++;

			n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp);
			if (n1 > 0) {
				emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
				sd = (*sp++);
				MLIB_M_IMAGE_CONST_ADD_U8_S(dd, sd, dc2);
				mmx_pst_8(dd, dp, emask);
				dp++;

				n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp);
				if (n1 > 0) {
					emask =
					    (n1 >
					    7) ? 0xFF : (0xFF << (8 - n1));
					sd = *sp;
					MLIB_M_IMAGE_CONST_ADD_U8_S(dd, sd,
					    dc3);
					mmx_pst_8(dd, dp, emask);
				}
			}
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstAdd_U8_3_S(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc1,
    __m64 dc2,
    __m64 dc3)
{
/* aligned src address */
	__m64 *sp, *sl;

/* aligned dst address */
	__m64 *dp, *dl;

/* source data */
	__m64 sd;

/* destination data */
	__m64 dd;
	__m64 dc_0, dc_1, dc_2, dc_3;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1, nu;

/* pointer to end of dst */
	mlib_u8 *dend;

	sp = sl = (__m64 *) src;
	dp = dl = (__m64 *) dst;

/* row loop */
	for (; ysize > 0; ysize--) {
		dc_1 = dc1;
		dc_2 = dc2;
		dc_3 = dc3;

		n1 = xsize;
		dend = (mlib_u8 *)dp + xsize;

		nu = ((8 - (mlib_addr)sp) & 7);
		if ((nu > 0) && (nu < n1)) {
			mlib_s32 nr = 8 - nu;

			emask = 0xFF << nr;
			sd = *sp;
			MLIB_M_IMAGE_CONST_ADD_U8_S(dd, sd, dc_1);
			mmx_pst_8(dd, dp, emask);
			sp = (__m64 *) ((mlib_u8 *)sl + nu);
			dp = (__m64 *) ((mlib_u8 *)dl + nu);
			n1 -= nu;

			if (nu == 1 || nu == 4 || nu == 7) {
				dc_0 = dc_1;
				dc_1 = dc_3;
				dc_3 = dc_2;
				dc_2 = dc_0;
			} else if (nu == 2 || nu == 5) {
				dc_0 = dc_1;
				dc_1 = dc_2;
				dc_2 = dc_3;
				dc_3 = dc_0;
			}
		}

/* 8-pixel column loop */
		for (; n1 > 23; n1 -= 24) {
			sd = sp[0];
			MLIB_M_IMAGE_CONST_ADD_U8_S(dp[0], sd, dc_1);
			sd = sp[1];
			MLIB_M_IMAGE_CONST_ADD_U8_S(dp[1], sd, dc_2);
			sd = sp[2];
			MLIB_M_IMAGE_CONST_ADD_U8_S(dp[2], sd, dc_3);
			sp += 3;
			dp += 3;
		}

		if (n1 > 0) {
			emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
			sd = (*sp++);
			MLIB_M_IMAGE_CONST_ADD_U8_S(dd, sd, dc_1);
			mmx_pst_8(dd, dp, emask);
			dp++;

			n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp);
			if (n1 > 0) {
				emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
				sd = (*sp++);
				MLIB_M_IMAGE_CONST_ADD_U8_S(dd, sd, dc_2);
				mmx_pst_8(dd, dp, emask);
				dp++;

				n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp);
				if (n1 > 0) {
					emask =
					    (n1 >
					    7) ? 0xFF : (0xFF << (8 - n1));
					sd = *sp;
					MLIB_M_IMAGE_CONST_ADD_U8_S(dd, sd,
					    dc_3);
					mmx_pst_8(dd, dp, emask);
				}
			}
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

mlib_status
__mlib_ImageConstAdd(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c)
{
	__m64 dc1, dc2, dc3, dc4, dc5, dc6;

	static const mlib_s32 A8D1 =
	    MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_ONEDVECTOR;
	static const mlib_s32 A8D2 = MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_STRIDE8X;

	mlib_s32 v_c[4], c_add[4], c_sub[4], c_flag;

	mlib_s32 i, slb, dlb, xsize, ysize, dsize, nchan, flags, flagd;

	mlib_s32 special_add = 0;
	mlib_s32 special_max = 0;
	mlib_s32 special_min = 0;

	void *sa, *da;
	mlib_type dtype;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);
	if (c == NULL)
		return (MLIB_FAILURE);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, nchan, xsize, ysize, dlb, da);

	sa = mlib_ImageGetData(src);
	slb = mlib_ImageGetStride(src);
	flags = mlib_ImageGetFlags(src);
	flagd = mlib_ImageGetFlags(dst);

	xsize = xsize * nchan;
	dsize = xsize * ysize;

	if (dtype == MLIB_INT) {
		switch (nchan) {
		case 1:
			dc1 = dc2 = _mm_set1_pi32(c[0]);
			break;

		case 2:
			dc1 = dc2 = _mm_set_pi32(c[1], c[0]);
			break;

		case 3:
			dc1 = _mm_set_pi32(c[1], c[0]);
			dc2 = _mm_set_pi32(c[0], c[2]);
			dc3 = _mm_set_pi32(c[2], c[1]);
			break;

		case 4:
			dc1 = _mm_set_pi32(c[1], c[0]);
			dc2 = _mm_set_pi32(c[3], c[2]);
			break;

		default:
			return (MLIB_FAILURE);
		}
	} else if (dtype == MLIB_SHORT) {
		switch (nchan) {
		case 1:
			dc1 = _mm_set1_pi16((mlib_s16)(c[0]));
			break;

		case 2:
			dc1 =
			    _mm_set_pi16((mlib_s16)(c[1]), (mlib_s16)(c[0]),
			    (mlib_s16)(c[1]), (mlib_s16)(c[0]));
			break;

		case 3:
			dc1 =
			    _mm_set_pi16((mlib_s16)(c[0]), (mlib_s16)(c[2]),
			    (mlib_s16)(c[1]), (mlib_s16)(c[0]));
			dc2 =
			    _mm_set_pi16((mlib_s16)(c[1]), (mlib_s16)(c[0]),
			    (mlib_s16)(c[2]), (mlib_s16)(c[1]));
			dc3 =
			    _mm_set_pi16((mlib_s16)(c[2]), (mlib_s16)(c[1]),
			    (mlib_s16)(c[0]), (mlib_s16)(c[2]));
			break;

		case 4:
			dc1 =
			    _mm_set_pi16((mlib_s16)(c[3]), (mlib_s16)(c[2]),
			    (mlib_s16)(c[1]), (mlib_s16)(c[0]));
			break;

		default:
			return (MLIB_FAILURE);
		}
	} else if (dtype == MLIB_USHORT) {
		c_flag = 0;
		for (i = 0; i < nchan; i++) {
			if (c[i] < 0) {
				c_add[i] = 0;
				c_sub[i] =
				    (c[i] <
				    -MLIB_U16_MAX) ? MLIB_U16_MAX : (-c[i]);
				c_flag = 1;
			} else if (c[i] > MLIB_U16_MAX) {
				c_add[i] = MLIB_U16_MAX;
				c_sub[i] = 0;
				c_flag = 1;
			} else {
				c_add[i] = c[i];
				c_sub[i] = 0;
			}
		}
		for (i = nchan; i < 4; i++) {
			c_add[i] = c_add[i - nchan];
			c_sub[i] = c_sub[i - nchan];
		}
		c = c_add;
		dc1 =
		    _mm_set_pi16((mlib_s16)(c[3]), (mlib_s16)(c[2]),
		    (mlib_s16)(c[1]), (mlib_s16)(c[0]));
		dc2 =
		    _mm_set_pi16((mlib_s16)(c[1]), (mlib_s16)(c[0]),
		    (mlib_s16)(c[2]), (mlib_s16)(c[1]));
		dc3 =
		    _mm_set_pi16((mlib_s16)(c[2]), (mlib_s16)(c[1]),
		    (mlib_s16)(c[0]), (mlib_s16)(c[2]));
		c = c_sub;
		dc4 =
		    _mm_set_pi16((mlib_s16)(c[3]), (mlib_s16)(c[2]),
		    (mlib_s16)(c[1]), (mlib_s16)(c[0]));
		dc5 =
		    _mm_set_pi16((mlib_s16)(c[1]), (mlib_s16)(c[0]),
		    (mlib_s16)(c[2]), (mlib_s16)(c[1]));
		dc6 =
		    _mm_set_pi16((mlib_s16)(c[2]), (mlib_s16)(c[1]),
		    (mlib_s16)(c[0]), (mlib_s16)(c[2]));
	} else {
/* dtype == MLIB_BYTE */
		special_add = special_min = special_max = 0;
		for (i = 0; i < nchan; i++) {
			v_c[i] =
			    (c[i] > MLIB_U8_MAX) ? MLIB_U8_MAX : (c[i] <
			    MLIB_S16_MIN) ? MLIB_S16_MIN : c[i];
			special_add =
			    (c[i] >=
			    MLIB_U8_MIN) ? special_add + 1 : special_add;
			special_max =
			    (c[i] >=
			    MLIB_U8_MAX) ? special_max + 1 : special_max;
			special_min =
			    (c[i] <=
			    -MLIB_U8_MAX) ? special_min + 1 : special_min;
		}
		c = v_c;

		if (special_min == nchan) {
			mlib_s32 v_clr[] =
			    { MLIB_U8_MIN, MLIB_U8_MIN, MLIB_U8_MIN,
				    MLIB_U8_MIN };
			return (__mlib_ImageClear(dst, v_clr));
		}

		if (special_max == nchan) {
			mlib_s32 v_clr[] =
			    { MLIB_U8_MAX, MLIB_U8_MAX, MLIB_U8_MAX,
				    MLIB_U8_MAX };
			return (__mlib_ImageClear(dst, v_clr));
		}

		if (special_add == nchan) {
			switch (nchan) {
			case 1:
				dc1 = _mm_set1_pi8((mlib_s8)(c[0]));
				break;

			case 2:
				dc1 =
				    _mm_set_pi8((mlib_s8)(c[1]),
				    (mlib_s8)(c[0]), (mlib_s8)(c[1]),
				    (mlib_s8)(c[0]), (mlib_s8)(c[1]),
				    (mlib_s8)(c[0]), (mlib_s8)(c[1]),
				    (mlib_s8)(c[0]));
				break;

			case 3:
				dc1 =
				    _mm_set_pi8((mlib_s8)(c[1]),
				    (mlib_s8)(c[0]), (mlib_s8)(c[2]),
				    (mlib_s8)(c[1]), (mlib_s8)(c[0]),
				    (mlib_s8)(c[2]), (mlib_s8)(c[1]),
				    (mlib_s8)(c[0]));

				dc2 =
				    _mm_set_pi8((mlib_s8)(c[0]),
				    (mlib_s8)(c[2]), (mlib_s8)(c[1]),
				    (mlib_s8)(c[0]), (mlib_s8)(c[2]),
				    (mlib_s8)(c[1]), (mlib_s8)(c[0]),
				    (mlib_s8)(c[2]));

				dc3 =
				    _mm_set_pi8((mlib_s8)(c[2]),
				    (mlib_s8)(c[1]), (mlib_s8)(c[0]),
				    (mlib_s8)(c[2]), (mlib_s8)(c[1]),
				    (mlib_s8)(c[0]), (mlib_s8)(c[2]),
				    (mlib_s8)(c[1]));
				break;

			case 4:
				dc1 =
				    _mm_set_pi8((mlib_s8)(c[3]),
				    (mlib_s8)(c[2]), (mlib_s8)(c[1]),
				    (mlib_s8)(c[0]), (mlib_s8)(c[3]),
				    (mlib_s8)(c[2]), (mlib_s8)(c[1]),
				    (mlib_s8)(c[0]));
				break;

			default:
				return (MLIB_FAILURE);
			}
		} else {
			switch (nchan) {
			case 1:
				dc1 = _mm_set1_pi16((mlib_s16)(c[0]));
				break;

			case 2:
				dc1 =
				    _mm_set_pi16((mlib_s16)(c[1]),
				    (mlib_s16)(c[0]), (mlib_s16)(c[1]),
				    (mlib_s16)(c[0]));
				break;

			case 3:
				dc1 =
				    _mm_set_pi16((mlib_s16)(c[0]),
				    (mlib_s16)(c[2]), (mlib_s16)(c[1]),
				    (mlib_s16)(c[0]));
				dc2 =
				    _mm_set_pi16((mlib_s16)(c[1]),
				    (mlib_s16)(c[0]), (mlib_s16)(c[2]),
				    (mlib_s16)(c[1]));
				dc3 =
				    _mm_set_pi16((mlib_s16)(c[2]),
				    (mlib_s16)(c[1]), (mlib_s16)(c[0]),
				    (mlib_s16)(c[2]));
				break;

			case 4:
				dc1 =
				    _mm_set_pi16((mlib_s16)(c[3]),
				    (mlib_s16)(c[2]), (mlib_s16)(c[1]),
				    (mlib_s16)(c[0]));
				break;

			default:
				return (MLIB_FAILURE);
			}
		}
	}

	_mm_empty();

	if (dtype == MLIB_BYTE) {
		if (special_add == nchan) {
			if (nchan == 3) {
				if (((flags & A8D1) == 0) &&
				    ((flagd & A8D1) == 0)) {
					mlib_m_ImageConstAdd_U8_3_A8D1_S(
					    (mlib_u8 *)sa, (mlib_u8 *)da, dsize,
					    dc1, dc2, dc3);
				} else if (((flags & A8D2) == 0) &&
				    ((flagd & A8D2) == 0)) {
					mlib_m_ImageConstAdd_U8_3_A8D2_S(
					    (mlib_u8 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize, dc1, dc2, dc3);
				} else {
					mlib_m_ImageConstAdd_U8_3_S((mlib_u8 *)
					    sa, slb, (mlib_u8 *)da, dlb, xsize,
					    ysize, dc1, dc2, dc3);
				}
			} else
			    if (((flags & A8D1) == 0) &&
				((flagd & A8D1) == 0) &&
				((dsize & 7) == 0)) {
				mlib_m_ImageConstAdd_U8_124_A8D1X8_S((mlib_u8 *)
				    sa, (mlib_u8 *)da, dsize, dc1);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0)) {
				mlib_m_ImageConstAdd_U8_124_A8D2_S((mlib_u8 *)
				    sa, slb, (mlib_u8 *)da, dlb, xsize, ysize,
				    dc1);
			} else {
				mlib_m_ImageConstAdd_U8_124_S((mlib_u8 *)sa,
				    slb, (mlib_u8 *)da, dlb, xsize, ysize, dc1);
			}
		} else {
			if (nchan == 3) {
				if (((flags & A8D1) == 0) &&
				    ((flagd & A8D1) == 0)) {
					mlib_m_ImageConstAdd_U8_3_A8D1((mlib_u8
					    *)sa, (mlib_u8 *)da, dsize, dc1,
					    dc2, dc3);
				} else if (((flags & A8D2) == 0) &&
				    ((flagd & A8D2) == 0)) {
					mlib_m_ImageConstAdd_U8_3_A8D2((mlib_u8
					    *)sa, slb, (mlib_u8 *)da, dlb,
					    xsize, ysize, dc1, dc2, dc3);
				} else {
					mlib_m_ImageConstAdd_U8_3((mlib_u8 *)sa,
					    slb, (mlib_u8 *)da, dlb, xsize,
					    ysize, dc1, dc2, dc3);
				}
			} else
			    if (((flags & A8D1) == 0) &&
				((flagd & A8D1) == 0) &&
				((dsize & 7) == 0)) {
				mlib_m_ImageConstAdd_U8_124_A8D1X8((mlib_u8 *)
				    sa, (mlib_u8 *)da, dsize, dc1);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0)) {
				mlib_m_ImageConstAdd_U8_124_A8D2((mlib_u8 *)sa,
				    slb, (mlib_u8 *)da, dlb, xsize, ysize, dc1);
			} else {
				mlib_m_ImageConstAdd_U8_124((mlib_u8 *)sa, slb,
				    (mlib_u8 *)da, dlb, xsize, ysize, dc1);
			}
		}

	} else if (dtype == MLIB_SHORT) {
		if (nchan == 3) {
			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0)) {
				mlib_m_ImageConstAdd_S16_3_A8D1((mlib_s16 *)sa,
				    (mlib_s16 *)da, dsize, dc1, dc2, dc3);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0)) {
				mlib_m_ImageConstAdd_S16_3_A8D2((mlib_s16 *)sa,
				    slb, (mlib_s16 *)da, dlb, xsize, ysize, dc1,
				    dc2, dc3);
			} else {
				mlib_m_ImageConstAdd_S16_3((mlib_s16 *)sa, slb,
				    (mlib_s16 *)da, dlb, xsize, ysize, dc1, dc2,
				    dc3);
			}
		} else
		    if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
		    ((dsize & 3) == 0)) {
			mlib_m_ImageConstAdd_S16_124_A8D1X4((mlib_s16 *)sa,
			    (mlib_s16 *)da, dsize, dc1);
		} else if (((flags & A8D2) == 0) && ((flagd & A8D2) == 0)) {
			mlib_m_ImageConstAdd_S16_124_A8D2((mlib_s16 *)sa, slb,
			    (mlib_s16 *)da, dlb, xsize, ysize, dc1);
		} else {
			mlib_m_ImageConstAdd_S16_124((mlib_s16 *)sa, slb,
			    (mlib_s16 *)da, dlb, xsize, ysize, dc1);
		}

	} else if (dtype == MLIB_USHORT) {
		if (!c_flag) {
			if (nchan == 3) {
				if (((flags & A8D2) == 0) &&
				    ((flagd & A8D2) == 0)) {
					mlib_m_ImageConstAdd_U16_3_A8D2(
					    (mlib_s16 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize, dc1, dc2, dc3);
				} else {
					mlib_m_ImageConstAdd_U16_3((mlib_s16 *)
					    sa, slb, (mlib_s16 *)da, dlb, xsize,
					    ysize, dc1, dc2, dc3);
				}
			} else {
				if (((flags & A8D2) == 0) &&
				    ((flagd & A8D2) == 0)) {
					mlib_m_ImageConstAdd_U16_124_A8D2(
					    (mlib_s16 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize, dc1);
				} else {
					mlib_m_ImageConstAdd_U16_124((mlib_s16
					    *)sa, slb, (mlib_s16 *)da, dlb,
					    xsize, ysize, dc1);
				}
			}
		} else {
			if (nchan == 3) {
				if (((flags & A8D2) == 0) &&
				    ((flagd & A8D2) == 0)) {
					mlib_m_ImageConstAdd_U16_3_A8D2_S(
					    (mlib_s16 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize, dc1, dc2, dc3,
					    dc4, dc5, dc6);
				} else {
					mlib_m_ImageConstAdd_U16_3_S((mlib_s16
					    *)sa, slb, (mlib_s16 *)da, dlb,
					    xsize, ysize, dc1, dc2, dc3, dc4,
					    dc5, dc6);
				}
			} else {
				if (((flags & A8D2) == 0) &&
				    ((flagd & A8D2) == 0)) {
					mlib_m_ImageConstAdd_U16_124_A8D2_S(
					    (mlib_s16 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize, dc1, dc4);
				} else {
					mlib_m_ImageConstAdd_U16_124_S((mlib_s16
					    *)sa, slb, (mlib_s16 *)da, dlb,
					    xsize, ysize, dc1, dc4);
				}
			}
		}

	} else if (dtype == MLIB_INT) {
		if (nchan == 3) {
			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0)) {
				mlib_m_ImageConstAdd_S32_3_A8D1((mlib_s32 *)sa,
				    (mlib_s32 *)da, dsize, dc1, dc2, dc3);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0)) {
				mlib_m_ImageConstAdd_S32_3_A8D2((mlib_s32 *)sa,
				    slb, (mlib_s32 *)da, dlb, xsize, ysize, dc1,
				    dc2, dc3);
			} else {
				mlib_m_ImageConstAdd_S32_3((mlib_s32 *)sa, slb,
				    (mlib_s32 *)da, dlb, xsize, ysize, dc1, dc2,
				    dc3);
			}
		} else {
			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & 3) == 0)) {
				mlib_m_ImageConstAdd_S32_124_A8D1X4((mlib_s32 *)
				    sa, (mlib_s32 *)da, dsize, dc1, dc2);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0)) {
				mlib_m_ImageConstAdd_S32_124_A8D2((mlib_s32 *)
				    sa, slb, (mlib_s32 *)da, dlb, xsize, ysize,
				    dc1, dc2);
			} else {
				mlib_m_ImageConstAdd_S32_124((mlib_s32 *)sa,
				    slb, (mlib_s32 *)da, dlb, xsize, ysize, dc1,
				    dc2);
			}
		}
	} else {
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
