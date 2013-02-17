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

#pragma ident	"@(#)mlib_m_ImageNot.c	9.6	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageNot - not an images
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageNot(mlib_image       *dst,
 *                                const mlib_image *src);
 *
 * ARGUMENT
 *      dst     pointer to destination image
 *      src     pointer to source image
 *
 * RESTRICTION
 *      The src and dst must be the same type and the same size.
 *      They can have 1, 2, 3, or 4 channels.
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT, MLIB_INT or
 *      MLIB_BIT data type.
 *
 * DESCRIPTION
 *      Not an image for each channel:
 *        dst = ~src
 */

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <mmintrin.h>
#endif

#include <mlib_mmx_utils.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageLogic_proto.h>

/* *************************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageNot = __mlib_ImageNot

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageNot) mlib_ImageNot
    __attribute__((weak, alias("__mlib_ImageNot")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOGIC(d, s)	d = _mm_not_si64(s);

/* *********************************************************** */

    static void mlib_m_ImageLogic_U8(
    mlib_u8 *da1,
    mlib_u8 *sa1,
    mlib_s32 dlb1,
    mlib_s32 slb1,
    mlib_s32 dw,
    mlib_s32 dh);

    static void mlib_m_ImageLogic_S16(
    mlib_s16 *da1,
    mlib_s16 *sa1,
    mlib_s32 dlb1,
    mlib_s32 slb1,
    mlib_s32 dw,
    mlib_s32 dh);

    static void mlib_m_ImageLogic_S32(
    mlib_s32 *da1,
    mlib_s32 *sa1,
    mlib_s32 dlb1,
    mlib_s32 slb1,
    mlib_s32 dw,
    mlib_s32 dh);

/* *********************************************************** */

static void
mlib_m_ImageLogic_U8(
    mlib_u8 *da1,
    mlib_u8 *sa1,
    mlib_s32 dlb1,
    mlib_s32 slb1,
    mlib_s32 dw,
    mlib_s32 dh)
{
/* dst address */
	__m64 *dp1, *dl1, dd1;

/* src1 address */
	__m64 *sp1, *sl1;

/* loop variables */
	mlib_s32 i, n1, nu;

/* edge mask */
	mlib_s32 emask;

	dp1 = dl1 = (__m64 *) da1;
	sp1 = sl1 = (__m64 *) sa1;

/* row loop */
	for (; dh > 0; dh--) {
		n1 = dw & 7;

		nu = ((8 - (mlib_addr)dp1) & 7);
		if ((nu > 0) && (nu < n1)) {
			emask = 0xFF << (8 - nu);
			MLIB_M_IMAGE_LOGIC(dd1, sp1[0]);
			mmx_pst_8(dd1, dp1, emask);
			n1 -= nu;

			dp1 = (__m64 *) ((mlib_u8 *)dl1 + nu);
			sp1 = (__m64 *) ((mlib_u8 *)sl1 + nu);
		}

/* 8-pixel column loop */
		for (i = 0; i < (dw >> 3); i++) {
			MLIB_M_IMAGE_LOGIC(dp1[i], sp1[i]);
		}

/* last 8-pixel column */
		if (n1 > 0) {
			dp1 = (__m64 *) (dp1 + i);
			emask = (0xFF << (8 - n1));
			MLIB_M_IMAGE_LOGIC(dd1, sp1[i]);
			mmx_pst_8(dd1, dp1, emask);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb1);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb1);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageLogic_S16(
    mlib_s16 *da1,
    mlib_s16 *sa1,
    mlib_s32 dlb1,
    mlib_s32 slb1,
    mlib_s32 dw,
    mlib_s32 dh)
{
/* dst address */
	__m64 *dp1, *dl1, dd1;

/* src1 address */
	__m64 *sp1, *sl1;

/* loop variables */
	mlib_s32 i, n1, nu;

/* edge mask */
	mlib_s32 emask;

	dp1 = dl1 = (__m64 *) da1;
	sp1 = sl1 = (__m64 *) sa1;

/* row loop */
	for (; dh > 0; dh--) {
		n1 = dw & 3;

		nu = ((8 - (mlib_addr)dp1) & 7) >> 1;
		if ((nu > 0) && (nu < n1)) {
			emask = 0xFF << (4 - nu);
			MLIB_M_IMAGE_LOGIC(dd1, sp1[0]);
			mmx_pst_16(dd1, dp1, emask);
			n1 -= nu;

			dp1 = (__m64 *) ((mlib_s16 *)dl1 + nu);
			sp1 = (__m64 *) ((mlib_s16 *)sl1 + nu);
		}

/* 8-pixel column loop */
		for (i = 0; i < (dw >> 2); i++) {
			MLIB_M_IMAGE_LOGIC(dp1[i], sp1[i]);
		}

/* last 8-pixel column */
		if (n1 > 0) {
			dp1 = (__m64 *) (dp1 + i);
			emask = (0xFF << (4 - n1));
			MLIB_M_IMAGE_LOGIC(dd1, sp1[i]);
			mmx_pst_16(dd1, dp1, emask);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb1);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb1);
	}

	_mm_empty();
}

/* *********************************************************** */

static void
mlib_m_ImageLogic_S32(
    mlib_s32 *da1,
    mlib_s32 *sa1,
    mlib_s32 dlb1,
    mlib_s32 slb1,
    mlib_s32 dw,
    mlib_s32 dh)
{
/* dst address */
	__m64 *dp1, *dl1, dd1;

/* src1 address */
	__m64 *sp1, *sl1;

/* loop variables */
	mlib_s32 i, n1;

	dp1 = dl1 = (__m64 *) da1;
	sp1 = sl1 = (__m64 *) sa1;

/* row loop */
	for (; dh > 0; dh--) {
		n1 = dw;

		if ((mlib_addr)dp1 & 7) {
			MLIB_M_IMAGE_LOGIC(dd1, sp1[0]);
			mmx_pst_32(dd1, dp1, 2);
			n1--;

			dp1 = (__m64 *) ((mlib_s32 *)dl1 + 1);
			sp1 = (__m64 *) ((mlib_s32 *)sl1 + 1);
		}

/* 8-pixel column loop */
		for (i = 0; i < (n1 >> 1); i++) {
			MLIB_M_IMAGE_LOGIC(dp1[i], sp1[i]);
		}

/* last 8-pixel column */
		if (n1 - (i << 1)) {
			dp1 = (__m64 *) (dp1 + i);
			MLIB_M_IMAGE_LOGIC(dd1, sp1[i]);
			mmx_pst_32(dd1, dp1, 2);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb1);
		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb1);
	}

	_mm_empty();
}

/* *********************************************************** */

mlib_status
__mlib_ImageNot(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 slb, dlb, dw, dh, dchan;
	void *da;
	mlib_type dtype;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, dchan, dw, dh, dlb, da);

	if (dtype == MLIB_BIT) {
		return (mlib_ImageNot_Bit(dst, src));
	}

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	slb = mlib_ImageGetStride(src);

	if ((dw * dchan) > slb || (dw * dchan) > dlb)
		return (MLIB_FAILURE);

	if (!mlib_ImageIsNotOneDvector(src) &&
		!mlib_ImageIsNotOneDvector(dst)) {
		dw *= dh;
		dh = 1;
	}

	dw *= dchan;

	if (dtype == MLIB_BYTE) {
		mlib_u8 *sa = (mlib_u8 *)mlib_ImageGetData(src);

		mlib_m_ImageLogic_U8((mlib_u8 *)da, sa, dlb, slb, dw, dh);
		return (MLIB_SUCCESS);
	}

	if (dtype == MLIB_SHORT || dtype == MLIB_USHORT) {
		mlib_s16 *sa = (mlib_s16 *)mlib_ImageGetData(src);

		mlib_m_ImageLogic_S16((mlib_s16 *)da, sa, dlb, slb, dw, dh);
		return (MLIB_SUCCESS);
	}

	if (dtype == MLIB_INT) {
		mlib_s32 *sa = (mlib_s32 *)mlib_ImageGetData(src);

		mlib_m_ImageLogic_S32((mlib_s32 *)da, sa, dlb, slb, dw, dh);
		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */
