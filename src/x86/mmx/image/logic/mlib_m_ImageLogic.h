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

#ifndef _MLIB_M_IMAGELOGIC_H
#define	_MLIB_M_IMAGELOGIC_H

#pragma ident	"@(#)mlib_m_ImageLogic.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_mmx_utils.h>

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

static void mlib_m_ImageLogic_U8(
    mlib_u8 *dst1,
    mlib_u8 *src1,
    mlib_u8 *src2,
    mlib_s32 dlb1,
    mlib_s32 slb1,
    mlib_s32 slb2,
    mlib_s32 dw,
    mlib_s32 dh);

static void mlib_m_ImageLogic_S16(
    mlib_s16 *dst1,
    mlib_s16 *src1,
    mlib_s16 *src2,
    mlib_s32 dlb1,
    mlib_s32 slb1,
    mlib_s32 slb2,
    mlib_s32 dw,
    mlib_s32 dh);

static void mlib_m_ImageLogic_S32(
    mlib_s32 *dst1,
    mlib_s32 *src1,
    mlib_s32 *src2,
    mlib_s32 dlb1,
    mlib_s32 slb1,
    mlib_s32 slb2,
    mlib_s32 dw,
    mlib_s32 dh);

/* *********************************************************** */
static mlib_status
mlib_m_ImageLogic(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2)
{
	mlib_s32 slb1, slb2, dlb, dw, dh, dchan;
	void *da;

	mlib_type dtype;

	MLIB_IMAGE_CHECK(dst);

	MLIB_IMAGE_CHECK(src1);

	MLIB_IMAGE_CHECK(src2);

	MLIB_IMAGE_FULL_EQUAL(dst, src1);

	MLIB_IMAGE_FULL_EQUAL(dst, src2);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, dchan, dw, dh, dlb, da);

	slb1 = mlib_ImageGetStride(src1);

	slb2 = mlib_ImageGetStride(src2);

	dw *= dchan;

	if (!mlib_ImageIsNotOneDvector(src1) &&
	    !mlib_ImageIsNotOneDvector(src2) &&
	    !mlib_ImageIsNotOneDvector(dst)) {
		dw *= dh;
		slb1 *= dh;
		slb2 *= dh;
		dlb *= dh;
		dh = 1;
	}

	if (dtype == MLIB_BYTE) {
		mlib_u8 *sa1 = (mlib_u8 *)mlib_ImageGetData(src1);

		mlib_u8 *sa2 = (mlib_u8 *)mlib_ImageGetData(src2);

		mlib_m_ImageLogic_U8((mlib_u8 *)da, sa1, sa2, dlb, slb1, slb2,
		    dw, dh);

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_SHORT || dtype == MLIB_USHORT) {
		mlib_s16 *sa1 = (mlib_s16 *)mlib_ImageGetData(src1);

		mlib_s16 *sa2 = (mlib_s16 *)mlib_ImageGetData(src2);

		mlib_m_ImageLogic_S16((mlib_s16 *)da, sa1, sa2, dlb, slb1, slb2,
		    dw, dh);

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_INT) {
		mlib_s32 *sa1 = (mlib_s32 *)mlib_ImageGetData(src1);

		mlib_s32 *sa2 = (mlib_s32 *)mlib_ImageGetData(src2);

		mlib_m_ImageLogic_S32((mlib_s32 *)da, sa1, sa2, dlb, slb1, slb2,
		    dw, dh);

		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */
static void
mlib_m_ImageLogic_U8(
    mlib_u8 *dst1,
    mlib_u8 *src1,
    mlib_u8 *src2,
    mlib_s32 dlb1,
    mlib_s32 slb1,
    mlib_s32 slb2,
    mlib_s32 dw,
    mlib_s32 dh)
{
/* dst address */
	__m64 *da1, *dp1, *dl1, dd1;

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
			MLIB_M_IMAGE_LOGIC(dd1, sd1, sd2);

			mmx_pst_8(dd1, da1, emask);

			n1 += 8;
			da1++;
		}

/* 8-pixel column loop */

		for (; n1 <= dw - 8; n1 += 8) {
			sd1 = (*sa1++);
			sd2 = (*sa2++);
			MLIB_M_IMAGE_LOGIC(dd1, sd1, sd2);

			(*da1++) = dd1;
		}

/* last 8-pixel column */
		if (n1 < dw) {
			emask = mmx_edge8(da1, dend);

			sd1 = *sa1;
			sd2 = *sa2;
			MLIB_M_IMAGE_LOGIC(dd1, sd1, sd2);

			mmx_pst_8(dd1, da1, emask);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb1);

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb1);

		sp2 = sl2 = (__m64 *) ((mlib_u8 *)sl2 + slb2);
	}

	_mm_empty();
}

/* *********************************************************** */
static void
mlib_m_ImageLogic_S16(
    mlib_s16 *dst1,
    mlib_s16 *src1,
    mlib_s16 *src2,
    mlib_s32 dlb1,
    mlib_s32 slb1,
    mlib_s32 slb2,
    mlib_s32 dw,
    mlib_s32 dh)
{
/* dst address */
	__m64 *da1, *dp1, *dl1, dd1;

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
			MLIB_M_IMAGE_LOGIC(dd1, sd1, sd2);

			mmx_pst_16(dd1, da1, emask);

			n1 += 4;
			da1++;
		}

/* 4-pixel column loop */
		for (; n1 <= dw - 4; n1 += 4) {
			sd1 = (*sa1++);
			sd2 = (*sa2++);
			MLIB_M_IMAGE_LOGIC(dd1, sd1, sd2);

			(*da1++) = dd1;
		}

/* last 4-pixel column */
		if (n1 < dw) {
			emask = mmx_edge16(da1, dend);

			sd1 = *sa1;
			sd2 = *sa2;
			MLIB_M_IMAGE_LOGIC(dd1, sd1, sd2);

			mmx_pst_16(dd1, da1, emask);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb1);

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb1);

		sp2 = sl2 = (__m64 *) ((mlib_u8 *)sl2 + slb2);
	}

	_mm_empty();
}

/* *********************************************************** */
static void
mlib_m_ImageLogic_S32(
    mlib_s32 *dst1,
    mlib_s32 *src1,
    mlib_s32 *src2,
    mlib_s32 dlb1,
    mlib_s32 slb1,
    mlib_s32 slb2,
    mlib_s32 dw,
    mlib_s32 dh)
{
/* dst address */
	__m64 *da1, *dp1, *dl1, dd1;

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
			MLIB_M_IMAGE_LOGIC(dd1, sd1, sd2);

			mmx_pst_32(dd1, da1, emask);

			n1 += 2;
			da1++;
		}

/* 2-pixel column loop */
		for (; n1 <= dw - 2; n1 += 2) {
			sd1 = (*sa1++);
			sd2 = (*sa2++);
			MLIB_M_IMAGE_LOGIC(dd1, sd1, sd2);

			(*da1++) = dd1;
		}

/* last 2-pixel column */
		if (n1 < dw) {
			emask = mmx_edge32(da1, dend);

			sd1 = *sa1;
			sd2 = *sa2;
			MLIB_M_IMAGE_LOGIC(dd1, sd1, sd2);

			mmx_pst_32(dd1, da1, emask);
		}

		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb1);

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb1);

		sp2 = sl2 = (__m64 *) ((mlib_u8 *)sl2 + slb2);
	}

	_mm_empty();
}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_M_IMAGELOGIC_H */
