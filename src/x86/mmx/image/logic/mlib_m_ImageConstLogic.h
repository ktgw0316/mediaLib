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

#ifndef _MLIB_M_IMAGECONSTLOGIC_H
#define	_MLIB_M_IMAGECONSTLOGIC_H

#pragma ident	"@(#)mlib_m_ImageConstLogic.h	9.3	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Macro definitions for MMX version image logical functions.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_mmx_utils.h>

static void mlib_m_ImageConstLogic_S32_124_A8D1X4(
    mlib_s32 *src,
    mlib_s32 *dst,
    mlib_s32 dsize,
    __m64 dc1,
    __m64 dc2);

static void mlib_m_ImageConstLogic_S32_124_A8D2(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc1,
    __m64 dc2);

static void mlib_m_ImageConstLogic_S32_124(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc1,
    __m64 dc2);

static void mlib_m_ImageConstLogic_S32_3_A8D1(
    mlib_s32 *src,
    mlib_s32 *dst,
    mlib_s32 dsize,
    __m64 dc1,
    __m64 dc2,
    __m64 dc3);

static void mlib_m_ImageConstLogic_S32_3_A8D2(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc1,
    __m64 dc2,
    __m64 dc3);

static void mlib_m_ImageConstLogic_S32_3(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc1,
    __m64 dc2,
    __m64 dc3);

/* *********************************************************** */
static void mlib_m_ImageConstLogic_S16_124_A8D1X4(
    mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    __m64 dc1);

static void mlib_m_ImageConstLogic_S16_124_A8D2(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc1);

static void mlib_m_ImageConstLogic_S16_124(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc1);

static void mlib_m_ImageConstLogic_S16_3_A8D1(
    mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    __m64 dc1,
    __m64 dc2,
    __m64 dc3);

static void mlib_m_ImageConstLogic_S16_3_A8D2(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc1,
    __m64 dc2,
    __m64 dc3);

static void mlib_m_ImageConstLogic_S16_3(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc1,
    __m64 dc2,
    __m64 dc3);

/* *********************************************************** */
static void mlib_m_ImageConstLogic_U8_124_A8D1X8(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    __m64 dc1);

static void mlib_m_ImageConstLogic_U8_3_A8D1(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    __m64 dc1,
    __m64 dc2,
    __m64 dc3);

static void mlib_m_ImageConstLogic_U8_124_A8D2(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc1);

static void mlib_m_ImageConstLogic_U8_124(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc1);

static void mlib_m_ImageConstLogic_U8_3_A8D2(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc1,
    __m64 dc2,
    __m64 dc3);

static void mlib_m_ImageConstLogic_U8_3(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    __m64 dc1,
    __m64 dc2,
    __m64 dc3);

/* *********************************************************** */

static mlib_status
mlib_m_ImageConstLogic(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c)
{
	__m64 dc1, dc2, dc3;

	static const mlib_s32 A8D1 =
	    MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_ONEDVECTOR;
	static const mlib_s32 A8D2 = MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_STRIDE8X;

	mlib_s32 slb, dlb, xsize, ysize, dsize, nchan, flags, flagd;

	void *sa, *da;
	mlib_type dtype;

	MLIB_IMAGE_SIZE_EQUAL(dst, src);
	MLIB_IMAGE_TYPE_EQUAL(dst, src);
	MLIB_IMAGE_CHAN_EQUAL(dst, src);

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
	} else if (dtype == MLIB_SHORT || dtype == MLIB_USHORT) {
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
	} else {
		if (dtype == MLIB_BYTE) {
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
		}
	}

	_mm_empty();

	if ((xsize > slb) || (xsize > slb))
		return (MLIB_FAILURE);

	if (dtype == MLIB_BYTE) {
		if (nchan == 3) {
			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0)) {
				mlib_m_ImageConstLogic_U8_3_A8D1((mlib_u8 *)sa,
				    (mlib_u8 *)da, dsize, dc1, dc2, dc3);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0)) {
				mlib_m_ImageConstLogic_U8_3_A8D2((mlib_u8 *)sa,
				    slb, (mlib_u8 *)da, dlb, xsize, ysize, dc1,
				    dc2, dc3);
			} else {
				mlib_m_ImageConstLogic_U8_3((mlib_u8 *)sa, slb,
				    (mlib_u8 *)da, dlb, xsize, ysize, dc1, dc2,
				    dc3);
			}
		} else {
			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & 7) == 0)) {
				mlib_m_ImageConstLogic_U8_124_A8D1X8((mlib_u8 *)
				    sa, (mlib_u8 *)da, dsize, dc1);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0)) {
				mlib_m_ImageConstLogic_U8_124_A8D2((mlib_u8 *)
				    sa, slb, (mlib_u8 *)da, dlb, xsize, ysize,
				    dc1);
			} else {
				mlib_m_ImageConstLogic_U8_124((mlib_u8 *)sa,
				    slb, (mlib_u8 *)da, dlb, xsize, ysize, dc1);
			}
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_SHORT || dtype == MLIB_USHORT) {
		if (nchan == 3) {
			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0)) {
				mlib_m_ImageConstLogic_S16_3_A8D1((mlib_s16 *)
				    sa, (mlib_s16 *)da, dsize, dc1, dc2, dc3);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0)) {
				mlib_m_ImageConstLogic_S16_3_A8D2((mlib_s16 *)
				    sa, slb, (mlib_s16 *)da, dlb, xsize, ysize,
				    dc1, dc2, dc3);
			} else {
				mlib_m_ImageConstLogic_S16_3((mlib_s16 *)sa,
				    slb, (mlib_s16 *)da, dlb, xsize, ysize, dc1,
				    dc2, dc3);
			}
		} else if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
		    ((dsize & 3) == 0)) {
			mlib_m_ImageConstLogic_S16_124_A8D1X4((mlib_s16 *)sa,
			    (mlib_s16 *)da, dsize, dc1);
		} else if (((flags & A8D2) == 0) && ((flagd & A8D2) == 0)) {
			mlib_m_ImageConstLogic_S16_124_A8D2((mlib_s16 *)sa, slb,
			    (mlib_s16 *)da, dlb, xsize, ysize, dc1);
		} else {
			mlib_m_ImageConstLogic_S16_124((mlib_s16 *)sa, slb,
			    (mlib_s16 *)da, dlb, xsize, ysize, dc1);
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_INT) {
		if (nchan == 3) {
			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0)) {
				mlib_m_ImageConstLogic_S32_3_A8D1((mlib_s32 *)
				    sa, (mlib_s32 *)da, dsize, dc1, dc2, dc3);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0)) {
				mlib_m_ImageConstLogic_S32_3_A8D2((mlib_s32 *)
				    sa, slb, (mlib_s32 *)da, dlb, xsize, ysize,
				    dc1, dc2, dc3);
			} else {
				mlib_m_ImageConstLogic_S32_3((mlib_s32 *)sa,
				    slb, (mlib_s32 *)da, dlb, xsize, ysize, dc1,
				    dc2, dc3);
			}
		} else {
			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & 3) == 0)) {
				mlib_m_ImageConstLogic_S32_124_A8D1X4((mlib_s32
				    *)sa, (mlib_s32 *)da, dsize, dc1, dc2);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0)) {
				mlib_m_ImageConstLogic_S32_124_A8D2((mlib_s32 *)
				    sa, slb, (mlib_s32 *)da, dlb, xsize, ysize,
				    dc1, dc2);
			} else {
				mlib_m_ImageConstLogic_S32_124((mlib_s32 *)sa,
				    slb, (mlib_s32 *)da, dlb, xsize, ysize, dc1,
				    dc2);
			}
		}

		_mm_empty();
		return (MLIB_SUCCESS);
	}

	_mm_empty();
	return (MLIB_FAILURE);
}

/* *********************************************************** */

static void
mlib_m_ImageConstLogic_S32_124_A8D1X4(
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
		MMX_CONST_LOGIC(dd1, sd1, dc1);
		MMX_CONST_LOGIC(dd2, sd2, dc2);
		(*dp++) = dd1;
		(*dp++) = dd2;
	}

	_mm_empty();
}

/* *********************************************************** */
static void
mlib_m_ImageConstLogic_S32_124_A8D2(
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
			MMX_CONST_LOGIC(dd1, sd1, dc1);
			MMX_CONST_LOGIC(dd2, sd2, dc2);
			(*dp++) = dd1;
			(*dp++) = dd2;
		}

/* last 4-pixel column */
		if (n1 > 0) {
			emask = (n1 > 1) ? 3 : 2;
			sd1 = (*sp++);
			MMX_CONST_LOGIC(dd1, sd1, dc1);
			mmx_pst_32(dd1, dp, emask);
			dp++;

			n1 = (mlib_s32 *)dend - (mlib_s32 *)dp;
			if (n1 > 0) {
				emask = (n1 > 1) ? 3 : 2;
				sd1 = *sp;
				MMX_CONST_LOGIC(dd1, sd1, dc2);
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
mlib_m_ImageConstLogic_S32_124(
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
			MMX_CONST_LOGIC(dd1, sd1, dc_1);
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
			MMX_CONST_LOGIC(dd1, sd1, dc_1);
			MMX_CONST_LOGIC(dd2, sd2, dc_2);
			(*dp++) = dd1;
			(*dp++) = dd2;
		}

/* last 4-pixel column */
		if (n1 > 0) {
			emask = (n1 > 1) ? 3 : 2;
			sd1 = (*sp++);
			MMX_CONST_LOGIC(dd1, sd1, dc_1);
			mmx_pst_32(dd1, dp, emask);
			dp++;

			n1 = (mlib_s32 *)dend - (mlib_s32 *)dp;
			if (n1 > 0) {
				emask = (n1 > 1) ? 3 : 2;
				sd1 = *sp;
				MMX_CONST_LOGIC(dd1, sd1, dc_2);
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
mlib_m_ImageConstLogic_S32_3_A8D1(
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

/* edge mask */
	mlib_s32 emask;

/* loop variable */
	mlib_s32 n1;

	sp = (__m64 *) src;
	dp = (__m64 *) dst;

/* 6-pixel column loop */
	for (n1 = dsize; n1 > 5; n1 -= 6) {
		sd = sp[0];
		MMX_CONST_LOGIC(dp[0], sd, dc1);
		sd = sp[1];
		MMX_CONST_LOGIC(dp[1], sd, dc2);
		sd = sp[2];
		MMX_CONST_LOGIC(dp[2], sd, dc3);
		sp += 3;
		dp += 3;
	}

/* last 3-pixel column */
	if (n1 > 0) {
		sd = sp[0];
		MMX_CONST_LOGIC(dd, sd, dc1);
		mmx_pst_32(dd, dp, 3);
		sd = sp[1];
		dp++;
		MMX_CONST_LOGIC(dd, sd, dc2);
		mmx_pst_32(dd, dp, 2);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstLogic_S32_3_A8D2(
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
			MMX_CONST_LOGIC(dp[0], sd, dc1);
			sd = sp[1];
			MMX_CONST_LOGIC(dp[1], sd, dc2);
			sd = sp[2];
			MMX_CONST_LOGIC(dp[2], sd, dc3);
			sp += 3;
			dp += 3;
		}

/* last 3-pixel column */
		if (n1 > 0) {
			sd = sp[0];
			MMX_CONST_LOGIC(dd, sd, dc1);
			mmx_pst_32(dd, dp, 3);
			sd = sp[1];
			dp++;
			MMX_CONST_LOGIC(dd, sd, dc2);
			mmx_pst_32(dd, dp, 2);
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstLogic_S32_3(
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
			MMX_CONST_LOGIC(dd, sd, dc_1);
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
			MMX_CONST_LOGIC(dp[0], sd, dc_1);
			sd = sp[1];
			MMX_CONST_LOGIC(dp[1], sd, dc_2);
			sd = sp[2];
			MMX_CONST_LOGIC(dp[2], sd, dc_3);
			sp += 3;
			dp += 3;
		}

/* last 4-pixel column */
		if (n1 > 0) {
			emask = (n1 > 1) ? 3 : 2;
			sd = (*sp++);
			MMX_CONST_LOGIC(dd, sd, dc_1);
			mmx_pst_32(dd, dp, emask);
			dp++;

			n1 = (mlib_s32 *)dend - (mlib_s32 *)dp;
			if (n1 > 0) {
				emask = (n1 > 1) ? 3 : 2;
				sd = (*sp++);
				MMX_CONST_LOGIC(dd, sd, dc_2);
				mmx_pst_32(dd, dp, emask);
				dp++;

				n1 = (mlib_s32 *)dend - (mlib_s32 *)dp;
				if (n1 > 0) {
					sd = *sp;
					MMX_CONST_LOGIC(dd, sd, dc_3);
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
mlib_m_ImageConstLogic_S16_124_A8D1X4(
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
		MMX_CONST_LOGIC(dd, sd, dc);
		(*dp++) = dd;
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstLogic_S16_124_A8D2(
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
			MMX_CONST_LOGIC(dd, sd, dc);
			(*dp++) = dd;
		}

/* last 4-pixel column */
		if (n1 > 0) {
			emask = 0xF << (4 - n1);
			sd = *sp;
			MMX_CONST_LOGIC(dd, sd, dc);
			mmx_pst_16(dd, dp, emask);
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstLogic_S16_124(
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
			MMX_CONST_LOGIC(dd, sd, dc_1);
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
			MMX_CONST_LOGIC(dd, sd, dc_1);
			(*dp++) = dd;
		}

/* last 4-pixel column */
		if (n1 > 0) {
			emask = 0xF << (4 - n1);
			sd = *sp;
			MMX_CONST_LOGIC(dd, sd, dc_1);
			mmx_pst_16(dd, dp, emask);
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstLogic_S16_3_A8D1(
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
	__m64 dc0;

/* loop variable */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;

/* pointer to end of dst */
	mlib_s16 *dend;

	sp = (__m64 *) src;
	dp = (__m64 *) dst;

	dend = (mlib_s16 *)dp + dsize;

/* 4-pixel column loop */
	for (n1 = dsize; n1 > 11; n1 -= 12) {
		sd = sp[0];
		MMX_CONST_LOGIC(dp[0], sd, dc1);
		sd = sp[1];
		MMX_CONST_LOGIC(dp[1], sd, dc2);
		sd = sp[2];
		MMX_CONST_LOGIC(dp[2], sd, dc3);
		sp += 3;
		dp += 3;
	}

	if (n1 > 0) {
		emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
		sd = (*sp++);
		MMX_CONST_LOGIC(dd, sd, dc1);
		mmx_pst_16(dd, dp, emask);
		dp++;

		n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp);
		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			sd = (*sp++);
			MMX_CONST_LOGIC(dd, sd, dc2);
			mmx_pst_16(dd, dp, emask);
			dp++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp);
			if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				sd = *sp;
				MMX_CONST_LOGIC(dd, sd, dc3);
				mmx_pst_16(dd, dp, emask);
			}
		}
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstLogic_S16_3_A8D2(
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
			MMX_CONST_LOGIC(dp[0], sd, dc1);
			sd = sp[1];
			MMX_CONST_LOGIC(dp[1], sd, dc2);
			sd = sp[2];
			MMX_CONST_LOGIC(dp[2], sd, dc3);
			sp += 3;
			dp += 3;
		}

/* last 4-pixel column loop */
		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			sd = (*sp++);
			MMX_CONST_LOGIC(dd, sd, dc1);
			mmx_pst_16(dd, dp, emask);
			dp++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp);
			if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				sd = (*sp++);
				MMX_CONST_LOGIC(dd, sd, dc2);
				mmx_pst_16(dd, dp, emask);
				dp++;

				n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp);
				if (n1 > 0) {
					emask =
					    (n1 > 3) ? 0xF : (0xF << (4 - n1));
					sd = *sp;
					MMX_CONST_LOGIC(dd, sd, dc3);
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
mlib_m_ImageConstLogic_S16_3(
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
			MMX_CONST_LOGIC(dd, sd, dc_1);
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
			MMX_CONST_LOGIC(dp[0], sd, dc_1);
			sd = sp[1];
			MMX_CONST_LOGIC(dp[1], sd, dc_2);
			sd = sp[2];
			MMX_CONST_LOGIC(dp[2], sd, dc_3);
			sp += 3;
			dp += 3;
		}

/* last 4-pixel column loop */
		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			sd = (*sp++);
			MMX_CONST_LOGIC(dd, sd, dc_1);
			mmx_pst_16(dd, dp, emask);
			dp++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp);
			if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				sd = (*sp++);
				MMX_CONST_LOGIC(dd, sd, dc_2);
				mmx_pst_16(dd, dp, emask);
				dp++;

				n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp);
				if (n1 > 0) {
					emask =
					    (n1 > 3) ? 0xF : (0xF << (4 - n1));
					sd = *sp;
					MMX_CONST_LOGIC(dd, sd, dc_3);
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
mlib_m_ImageConstLogic_U8_124_A8D1X8(
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
		MMX_CONST_LOGIC(dd, sd, dc);
		(*dp++) = dd;
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstLogic_U8_124_A8D2(
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
			MMX_CONST_LOGIC(dd, sd, dc);
			(*dp++) = dd;
		}

/* last 8-pixel column */
		if (n1 > 0) {
			emask = (0xFF << (8 - n1));
			sd = *sp;
			MMX_CONST_LOGIC(dd, sd, dc)
			    mmx_pst_8(dd, dp, emask);
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstLogic_U8_124(
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
			MMX_CONST_LOGIC(dd, sd, dc_1)
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
			MMX_CONST_LOGIC(dd, sd, dc_1);
			(*dp++) = dd;
		}

/* last 8-pixel column */
		if (n1 > 0) {
			emask = (0xFF << (8 - n1));
			sd = *sp;
			MMX_CONST_LOGIC(dd, sd, dc_1)
			    mmx_pst_8(dd, dp, emask);
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstLogic_U8_3_A8D1(
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
		MMX_CONST_LOGIC(dp[0], sd, dc1);
		sd = sp[1];
		MMX_CONST_LOGIC(dp[1], sd, dc2);
		sd = sp[2];
		MMX_CONST_LOGIC(dp[2], sd, dc3);
		sp += 3;
		dp += 3;
	}

	if (n1 > 0) {
		emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
		sd = (*sp++);
		MMX_CONST_LOGIC(dd, sd, dc1);
		mmx_pst_8(dd, dp, emask);
		dp++;

		n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp);
		if (n1 > 0) {
			emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
			sd = (*sp++);
			MMX_CONST_LOGIC(dd, sd, dc2);
			mmx_pst_8(dd, dp, emask);
			dp++;

			n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp);
			if (n1 > 0) {
				emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
				sd = *sp;
				MMX_CONST_LOGIC(dd, sd, dc3);
				mmx_pst_8(dd, dp, emask);
			}
		}
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageConstLogic_U8_3_A8D2(
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
			MMX_CONST_LOGIC(dp[0], sd, dc1);
			sd = sp[1];
			MMX_CONST_LOGIC(dp[1], sd, dc2);
			sd = sp[2];
			MMX_CONST_LOGIC(dp[2], sd, dc3);
			sp += 3;
			dp += 3;
		}

		if (n1 > 0) {
			emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
			sd = (*sp++);
			MMX_CONST_LOGIC(dd, sd, dc1);
			mmx_pst_8(dd, dp, emask);
			dp++;

			n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp);
			if (n1 > 0) {
				emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
				sd = (*sp++);
				MMX_CONST_LOGIC(dd, sd, dc2);
				mmx_pst_8(dd, dp, emask);
				dp++;

				n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp);
				if (n1 > 0) {
					emask =
					    (n1 >
					    7) ? 0xFF : (0xFF << (8 - n1));
					sd = *sp;
					MMX_CONST_LOGIC(dd, sd, dc3);
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
mlib_m_ImageConstLogic_U8_3(
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
			MMX_CONST_LOGIC(dd, sd, dc_1);
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
			MMX_CONST_LOGIC(dp[0], sd, dc_1);
			sd = sp[1];
			MMX_CONST_LOGIC(dp[1], sd, dc_2);
			sd = sp[2];
			MMX_CONST_LOGIC(dp[2], sd, dc_3);
			sp += 3;
			dp += 3;
		}

		if (n1 > 0) {
			emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
			sd = (*sp++);
			MMX_CONST_LOGIC(dd, sd, dc_1);
			mmx_pst_8(dd, dp, emask);
			dp++;

			n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp);
			if (n1 > 0) {
				emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
				sd = (*sp++);
				MMX_CONST_LOGIC(dd, sd, dc_2);
				mmx_pst_8(dd, dp, emask);
				dp++;

				n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp);
				if (n1 > 0) {
					emask =
					    (n1 >
					    7) ? 0xFF : (0xFF << (8 - n1));
					sd = *sp;
					MMX_CONST_LOGIC(dd, sd, dc_3);
					mmx_pst_8(dd, dp, emask);
				}
			}
		}

		sp = sl = (__m64 *) ((mlib_u8 *)sl + slb);
		dp = dl = (__m64 *) ((mlib_u8 *)dl + dlb);
	}

	_mm_empty();
}

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_M_IMAGECONSTLOGIC_H */
