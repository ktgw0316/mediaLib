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

#pragma ident	"@(#)mlib_m_ImageMax.c	9.7	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageMax - get max pixel from two images
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMax(mlib_image *dst,
 *                                const mlib_image *src1,
 *                                const mlib_image *src2);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src1    pointer to first input image
 *      src2    pointer to second input image
 *
 * RESTRICTION
 *      src1, src2, and dst must be the same type, the same size and the same
 *      number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      Max two images for each channel:
 *         dst = max(src1, src2)
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

#pragma weak mlib_ImageMax = __mlib_ImageMax

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageMax) mlib_ImageMax
    __attribute__((weak, alias("__mlib_ImageMax")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_M_IMAGE_MAX_U8_MASK(cmask64, s1, s2)                 \
	{                                                         \
	    cmask64 = _mm_cmpeq_pi8(_mm_subs_pu8(sd1, sd2),	  \
			    _m_zero);   			  \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_MAX_S16_MASK(cmask64, s1, s2)              \
	{                                                       \
	    cmask64 = _mm_cmpgt_pi16(sd2, sd1);                 \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_MAX_U16_MASK(cmask64, s1, s2)                  \
	{                                                           \
	    cmask64 = _mm_cmpeq_pi16(_mm_subs_pu16(sd1, sd2),	    \
			    _m_zero);   			    \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_MAX_S32_MASK(cmask64, s1, s2)              \
	{                                                       \
	    cmask64 = _mm_cmpgt_pi32(sd2, sd1);                 \
	}

/* *********************************************************** */

    static void mlib_m_ImageMax_U8(
    mlib_u8 *da1,
    mlib_u8 *sa1,
    mlib_u8 *sa2,
    mlib_s32 dlb1,
    mlib_s32 slb1,
    mlib_s32 slb2,
    mlib_s32 dw,
    mlib_s32 dh);

/* *********************************************************** */

    static void mlib_m_ImageMax_S16(
    mlib_s16 *da1,
    mlib_s16 *sa1,
    mlib_s16 *sa2,
    mlib_s32 dlb1,
    mlib_s32 slb1,
    mlib_s32 slb2,
    mlib_s32 dw,
    mlib_s32 dh);

/* *********************************************************** */

    static void mlib_m_ImageMax_U16(
    mlib_u16 *da1,
    mlib_u16 *sa1,
    mlib_u16 *sa2,
    mlib_s32 dlb1,
    mlib_s32 slb1,
    mlib_s32 slb2,
    mlib_s32 dw,
    mlib_s32 dh);

/* *********************************************************** */

    static void mlib_m_ImageMax_S32(
    mlib_s32 *da1,
    mlib_s32 *sa1,
    mlib_s32 *sa2,
    mlib_s32 dlb1,
    mlib_s32 slb1,
    mlib_s32 slb2,
    mlib_s32 dw,
    mlib_s32 dh);

/* *********************************************************** */

    mlib_status __mlib_ImageMax(
    mlib_image *dst1,
    const mlib_image *src1,
    const mlib_image *src2)
{
	mlib_s32 slb1, slb2, dlb1, dw, dh, dchan;
	void *da1;

	mlib_type dtype;

	MLIB_IMAGE_CHECK(dst1);
	MLIB_IMAGE_CHECK(src1);
	MLIB_IMAGE_CHECK(src2);
	MLIB_IMAGE_FULL_EQUAL(dst1, src1);
	MLIB_IMAGE_FULL_EQUAL(dst1, src2);

	MLIB_IMAGE_GET_ALL_PARAMS(dst1, dtype, dchan, dw, dh, dlb1, da1);

	slb1 = mlib_ImageGetStride(src1);
	slb2 = mlib_ImageGetStride(src2);

	dw *= dchan;

	if (!mlib_ImageIsNotOneDvector(src1) &&
	    !mlib_ImageIsNotOneDvector(src2) &&
	    !mlib_ImageIsNotOneDvector(dst1)) {
		dw *= dh;
		slb1 *= dh;
		slb2 *= dh;
		dlb1 *= dh;
		dh = 1;
	}

	if (dtype == MLIB_BYTE) {
		mlib_u8 *sa1 = (mlib_u8 *)mlib_ImageGetData(src1);
		mlib_u8 *sa2 = (mlib_u8 *)mlib_ImageGetData(src2);

		mlib_m_ImageMax_U8((mlib_u8 *)da1, sa1, sa2, dlb1, slb1, slb2,
		    dw, dh);
		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_SHORT) {
		mlib_s16 *sa1 = (mlib_s16 *)mlib_ImageGetData(src1);
		mlib_s16 *sa2 = (mlib_s16 *)mlib_ImageGetData(src2);

		mlib_m_ImageMax_S16((mlib_s16 *)da1, sa1, sa2, dlb1, slb1, slb2,
		    dw, dh);
		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_USHORT) {
		mlib_u16 *sa1 = (mlib_u16 *)mlib_ImageGetData(src1);
		mlib_u16 *sa2 = (mlib_u16 *)mlib_ImageGetData(src2);

		mlib_m_ImageMax_U16((mlib_u16 *)da1, sa1, sa2, dlb1, slb1, slb2,
		    dw, dh);
		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_INT) {
		mlib_s32 *sa1 = (mlib_s32 *)mlib_ImageGetData(src1);
		mlib_s32 *sa2 = (mlib_s32 *)mlib_ImageGetData(src2);

		mlib_m_ImageMax_S32((mlib_s32 *)da1, sa1, sa2, dlb1, slb1, slb2,
		    dw, dh);
		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

static void
mlib_m_ImageMax_U8(
    mlib_u8 *dst1,
    mlib_u8 *src1,
    mlib_u8 *src2,
    mlib_s32 dlb1,
    mlib_s32 slb1,
    mlib_s32 slb2,
    mlib_s32 dw,
    mlib_s32 dh)
{
	__m64 emask64, cmask64;

/* dst address */
	__m64 *da1, *dp1, *dl1;

/* src1 address */
	__m64 *sa1, *sp1, *sl1, sd1;

/* src1 address */
	__m64 *sa2, *sp2, *sl2, sd2;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_u8 *dend;

	dp1 = dl1 = (__m64 *) dst1;
	sp1 = sl1 = (__m64 *) src1;
	sp2 = sl2 = (__m64 *) src2;

/* row loop */
	for (; dh > 0; dh--) {
		da1 = (__m64 *) (((mlib_addr)dp1) & ~7);
		sa1 = (__m64 *) (((mlib_addr)sp1));
		sa2 = (__m64 *) (((mlib_addr)sp2));

		n1 = (mlib_u8 *)da1 - (mlib_u8 *)dp1;
		dend = (mlib_u8 *)dp1 + dw - 1;
		emask = mmx_edge8(dp1, dend);

		if (emask != 0xff) {
			sa1 = (__m64 *) ((mlib_u8 *)sp1 + n1);
			sa2 = (__m64 *) ((mlib_u8 *)sp2 + n1);
			sd1 = (*sa1++);
			sd2 = (*sa2++);
			emask64 = mask_pst_8[emask & 0xFF];
			MLIB_M_IMAGE_MAX_U8_MASK(cmask64, sd1, sd2)
			    cmask64 = _mm_and_si64(cmask64, emask64);
			mmx_pst_8m64(sd1, da1, emask64);
			mmx_pst_8m64(sd2, da1, cmask64);
			n1 += 8;
			da1++;
		}

/* 8-pixel column loop */

		for (; n1 <= dw - 8; n1 += 8) {
			sd1 = (*sa1++);
			sd2 = (*sa2++);
			MLIB_M_IMAGE_MAX_U8_MASK(cmask64, sd1, sd2)
			    * da1 = sd1;
			mmx_pst_8m64(sd2, da1, cmask64);
			da1++;
		}

/* last 8-pixel column */
		if (n1 < dw) {
			emask = mmx_edge8(da1, dend);
			sd1 = *sa1;
			sd2 = *sa2;
			emask64 = mask_pst_8[emask & 0xFF];
			MLIB_M_IMAGE_MAX_U8_MASK(cmask64, sd1, sd2)
			    cmask64 = _mm_and_si64(cmask64, emask64);
			mmx_pst_8m64(sd1, da1, emask64);
			mmx_pst_8m64(sd2, da1, cmask64);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb1);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb1);
		sp2 = sl2 = (__m64 *) ((mlib_u8 *)sl2 + slb2);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageMax_S16(
    mlib_s16 *dst1,
    mlib_s16 *src1,
    mlib_s16 *src2,
    mlib_s32 dlb1,
    mlib_s32 slb1,
    mlib_s32 slb2,
    mlib_s32 dw,
    mlib_s32 dh)
{
	__m64 emask64, cmask64;

/* dst address */
	__m64 *da1, *dp1, *dl1;

/* src1 address */
	__m64 *sa1, *sp1, *sl1, sd1;

/* src1 address */
	__m64 *sa2, *sp2, *sl2, sd2;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_s16 *dend;

	dp1 = dl1 = (__m64 *) dst1;
	sp1 = sl1 = (__m64 *) src1;
	sp2 = sl2 = (__m64 *) src2;

/* row loop */
	for (; dh > 0; dh--) {
		da1 = (__m64 *) (((mlib_addr)dp1) & ~7);
		sa1 = (__m64 *) (((mlib_addr)sp1));
		sa2 = (__m64 *) (((mlib_addr)sp2));

		n1 = (mlib_s16 *)da1 - (mlib_s16 *)dp1;
		dend = (mlib_s16 *)dp1 + dw - 1;
		emask = mmx_edge16(dp1, dend);

		if (emask != 0xf) {
			sa1 = (__m64 *) ((mlib_s16 *)sp1 + n1);
			sa2 = (__m64 *) ((mlib_s16 *)sp2 + n1);
			sd1 = (*sa1++);
			sd2 = (*sa2++);
			emask64 = mask_pst_16[emask & 0xF];
			MLIB_M_IMAGE_MAX_S16_MASK(cmask64, sd1, sd2)
			    cmask64 = _mm_and_si64(cmask64, emask64);
			mmx_pst_16m64(sd1, da1, emask64);
			mmx_pst_16m64(sd2, da1, cmask64);
			n1 += 4;
			da1++;
		}

/* 4-pixel column loop */
		for (; n1 <= dw - 4; n1 += 4) {
			sd1 = (*sa1++);
			sd2 = (*sa2++);
			MLIB_M_IMAGE_MAX_S16_MASK(cmask64, sd1, sd2)
			    * da1 = sd1;
			mmx_pst_16m64(sd2, da1, cmask64);
			da1++;
		}

/* last 4-pixel column */
		if (n1 < dw) {
			emask = mmx_edge16(da1, dend);
			sd1 = *sa1;
			sd2 = *sa2;
			emask64 = mask_pst_16[emask & 0xF];
			MLIB_M_IMAGE_MAX_S16_MASK(cmask64, sd1, sd2)
			    cmask64 = _mm_and_si64(cmask64, emask64);
			mmx_pst_16m64(sd1, da1, emask64);
			mmx_pst_16m64(sd2, da1, cmask64);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb1);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb1);
		sp2 = sl2 = (__m64 *) ((mlib_u8 *)sl2 + slb2);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageMax_U16(
    mlib_u16 *dst1,
    mlib_u16 *src1,
    mlib_u16 *src2,
    mlib_s32 dlb1,
    mlib_s32 slb1,
    mlib_s32 slb2,
    mlib_s32 dw,
    mlib_s32 dh)
{
	__m64 emask64, cmask64;

/* dst address */
	__m64 *da1, *dp1, *dl1;

/* src1 address */
	__m64 *sa1, *sp1, *sl1, sd1;

/* src1 address */
	__m64 *sa2, *sp2, *sl2, sd2;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_u16 *dend;

	dp1 = dl1 = (__m64 *) dst1;
	sp1 = sl1 = (__m64 *) src1;
	sp2 = sl2 = (__m64 *) src2;

/* row loop */
	for (; dh > 0; dh--) {
		da1 = (__m64 *) (((mlib_addr)dp1) & ~7);
		sa1 = (__m64 *) (((mlib_addr)sp1));
		sa2 = (__m64 *) (((mlib_addr)sp2));

		n1 = (mlib_u16 *)da1 - (mlib_u16 *)dp1;
		dend = (mlib_u16 *)dp1 + dw - 1;
		emask = mmx_edge16(dp1, dend);

		if (emask != 0xf) {
			sa1 = (__m64 *) ((mlib_u16 *)sp1 + n1);
			sa2 = (__m64 *) ((mlib_u16 *)sp2 + n1);
			sd1 = (*sa1++);
			sd2 = (*sa2++);
			emask64 = mask_pst_16[emask & 0xF];
			MLIB_M_IMAGE_MAX_U16_MASK(cmask64, sd1, sd2)
			    cmask64 = _mm_and_si64(cmask64, emask64);
			mmx_pst_16m64(sd1, da1, emask64);
			mmx_pst_16m64(sd2, da1, cmask64);
			n1 += 4;
			da1++;
		}

/* 4-pixel column loop */
		for (; n1 <= dw - 4; n1 += 4) {
			sd1 = (*sa1++);
			sd2 = (*sa2++);
			MLIB_M_IMAGE_MAX_U16_MASK(cmask64, sd1, sd2)
			    * da1 = sd1;
			mmx_pst_16m64(sd2, da1, cmask64);
			da1++;
		}

/* last 4-pixel column */
		if (n1 < dw) {
			emask = mmx_edge16(da1, dend);
			sd1 = *sa1;
			sd2 = *sa2;
			emask64 = mask_pst_16[emask & 0xF];
			MLIB_M_IMAGE_MAX_U16_MASK(cmask64, sd1, sd2)
			    cmask64 = _mm_and_si64(cmask64, emask64);
			mmx_pst_16m64(sd1, da1, emask64);
			mmx_pst_16m64(sd2, da1, cmask64);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb1);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb1);
		sp2 = sl2 = (__m64 *) ((mlib_u8 *)sl2 + slb2);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageMax_S32(
    mlib_s32 *dst1,
    mlib_s32 *src1,
    mlib_s32 *src2,
    mlib_s32 dlb1,
    mlib_s32 slb1,
    mlib_s32 slb2,
    mlib_s32 dw,
    mlib_s32 dh)
{
	__m64 emask64, cmask64;

/* dst address */
	__m64 *da1, *dp1, *dl1;

/* src1 address */
	__m64 *sa1, *sp1, *sl1, sd1;

/* src1 address */
	__m64 *sa2, *sp2, *sl2, sd2;

/* loop variables */
	mlib_s32 n1;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 *dend;

	dp1 = dl1 = (__m64 *) dst1;
	sp1 = sl1 = (__m64 *) src1;
	sp2 = sl2 = (__m64 *) src2;

/* row loop */
	for (; dh > 0; dh--) {
		da1 = (__m64 *) (((mlib_addr)dp1) & ~7);
		sa1 = (__m64 *) (((mlib_addr)sp1));
		sa2 = (__m64 *) (((mlib_addr)sp2));

		n1 = (mlib_s32 *)da1 - (mlib_s32 *)dp1;
		dend = (mlib_s32 *)dp1 + dw - 1;
		emask = mmx_edge32(dp1, dend);

		if (emask != 0x3) {
			sa1 = (__m64 *) ((mlib_s32 *)sp1 + n1);
			sa2 = (__m64 *) ((mlib_s32 *)sp2 + n1);
			sd1 = (*sa1++);
			sd2 = (*sa2++);
			emask64 = mask_pst_32[emask & 0x3];
			MLIB_M_IMAGE_MAX_S32_MASK(cmask64, sd1, sd2)
			    cmask64 = _mm_and_si64(cmask64, emask64);
			mmx_pst_32m64(sd1, da1, emask64);
			mmx_pst_32m64(sd2, da1, cmask64);
			n1 += 2;
			da1++;
		}

/* 2-pixel column loop */
		for (; n1 <= dw - 2; n1 += 2) {
			sd1 = (*sa1++);
			sd2 = (*sa2++);
			MLIB_M_IMAGE_MAX_S32_MASK(cmask64, sd1, sd2)
			    * da1 = sd1;
			mmx_pst_32m64(sd2, da1, cmask64);
			da1++;
		}

/* last 2-pixel column */
		if (n1 < dw) {
			emask = mmx_edge32(da1, dend);
			sd1 = *sa1;
			sd2 = *sa2;
			emask64 = mask_pst_32[emask & 0x3];
			MLIB_M_IMAGE_MAX_S32_MASK(cmask64, sd1, sd2)
			    cmask64 = _mm_and_si64(cmask64, emask64);
			mmx_pst_32m64(sd1, da1, emask64);
			mmx_pst_32m64(sd2, da1, cmask64);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb1);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb1);
		sp2 = sl2 = (__m64 *) ((mlib_u8 *)sl2 + slb2);
	}

	_mm_empty();
}

/* *********************************************************** */
