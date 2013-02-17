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

#pragma ident	"@(#)mlib_m_ImageBlend.c	9.6	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageBlend - blend two images
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageBlend(mlib_image *dst,
 *                                  const mlib_image *src1,
 *                                  const mlib_image *src2,
 *                                  const mlib_image *alpha);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src1    pointer to first input image
 *      src2    pointer to second input image
 *      alpha   pointer to alpha image
 *
 * RESTRICTION
 *      src1, src2, dst, and alpha must be the same type,
 *      the same size and the same number of channels.
 *      They can have one to four channels. They can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type. Optionally, alpha can be
 *      single-channel image while src1, src2, and dst are multi-channel images.
 *      MLIB_BYTE alpha coefficients are in Q8 format.
 *      MLIB_SHORT alpha coefficients are in Q15 format and must be positive.
 *      MLIB_USHORT alpha coefficients are in Q16 format.
 *      MLIB_INT alpha coefficients are in Q31 format and must be positive.
 *
 * DESCRIPTION
 *      Blend two image with an alpha coefficient for each channel.
 *      dst = alpha*src1 + (1-alpha)*src2
 */

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <mmintrin.h>
#endif

#include <mlib_ImageCheck.h>
#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_mmx_utils.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageBlend = __mlib_ImageBlend

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageBlend) mlib_ImageBlend
    __attribute__((weak, alias("__mlib_ImageBlend")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

    static void mlib_m_ImageBlend_U8(
    mlib_u8 *src1,
    mlib_s32 slb1,
    mlib_u8 *src2,
    mlib_s32 slb2,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_u8 *aimg,
    mlib_s32 alb,
    mlib_s32 dw,
    mlib_s32 dh);

    static void mlib_m_ImageBlendSA_U8_2(
    mlib_u8 *src1,
    mlib_s32 slb1,
    mlib_u8 *src2,
    mlib_s32 slb2,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_u8 *aimg,
    mlib_s32 alb,
    mlib_s32 dw,
    mlib_s32 dh);

    static void mlib_m_ImageBlendSA_U8_3(
    mlib_u8 *src1,
    mlib_s32 slb1,
    mlib_u8 *src2,
    mlib_s32 slb2,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_u8 *aimg,
    mlib_s32 alb,
    mlib_s32 dw,
    mlib_s32 dh);

    static void mlib_m_ImageBlendSA_U8_4(
    mlib_u8 *src1,
    mlib_s32 slb1,
    mlib_u8 *src2,
    mlib_s32 slb2,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_u8 *aimg,
    mlib_s32 alb,
    mlib_s32 dw,
    mlib_s32 dh);

/* *********************************************************** */

    static void mlib_m_ImageBlend_S16(
    mlib_s16 *src1,
    mlib_s32 slb1,
    mlib_s16 *src2,
    mlib_s32 slb2,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s16 *aimg,
    mlib_s32 alb,
    mlib_s32 dw,
    mlib_s32 dh);

    static void mlib_m_ImageBlendSA_S16_2(
    mlib_s16 *src1,
    mlib_s32 slb1,
    mlib_s16 *src2,
    mlib_s32 slb2,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s16 *aimg,
    mlib_s32 alb,
    mlib_s32 dw,
    mlib_s32 dh);

    static void mlib_m_ImageBlendSA_S16_3(
    mlib_s16 *src1,
    mlib_s32 slb1,
    mlib_s16 *src2,
    mlib_s32 slb2,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s16 *aimg,
    mlib_s32 alb,
    mlib_s32 dw,
    mlib_s32 dh);

    static void mlib_m_ImageBlendSA_S16_4(
    mlib_s16 *src1,
    mlib_s32 slb1,
    mlib_s16 *src2,
    mlib_s32 slb2,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s16 *aimg,
    mlib_s32 alb,
    mlib_s32 dw,
    mlib_s32 dh);

/* *********************************************************** */

    static void mlib_m_ImageBlend_U16(
    mlib_u16 *src1,
    mlib_s32 slb1,
    mlib_u16 *src2,
    mlib_s32 slb2,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_u16 *aimg,
    mlib_s32 alb,
    mlib_s32 dw,
    mlib_s32 dh);

    static void mlib_m_ImageBlendSA_U16_2(
    mlib_u16 *src1,
    mlib_s32 slb1,
    mlib_u16 *src2,
    mlib_s32 slb2,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_u16 *aimg,
    mlib_s32 alb,
    mlib_s32 dw,
    mlib_s32 dh);

    static void mlib_m_ImageBlendSA_U16_3(
    mlib_u16 *src1,
    mlib_s32 slb1,
    mlib_u16 *src2,
    mlib_s32 slb2,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_u16 *aimg,
    mlib_s32 alb,
    mlib_s32 dw,
    mlib_s32 dh);

    static void mlib_m_ImageBlendSA_U16_4(
    mlib_u16 *src1,
    mlib_s32 slb1,
    mlib_u16 *src2,
    mlib_s32 slb2,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_u16 *aimg,
    mlib_s32 alb,
    mlib_s32 dw,
    mlib_s32 dh);

/* *********************************************************** */

    static void mlib_m_ImageBlend_S32(
    mlib_s32 *src1,
    mlib_s32 slb1,
    mlib_s32 *src2,
    mlib_s32 slb2,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 *aimg,
    mlib_s32 alb,
    mlib_s32 dw,
    mlib_s32 dh);

    static void mlib_m_ImageBlendSA_S32(
    mlib_s32 *src1,
    mlib_s32 slb1,
    mlib_s32 *src2,
    mlib_s32 slb2,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 *aimg,
    mlib_s32 alb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 ch);

/* *********************************************************** */

    mlib_status __mlib_ImageBlend(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    const mlib_image *alpha)
{
	void *sa1, *sa2, *da, *aa;

	mlib_s32 slb1, slb2, dlb, alb, dw, dh, nchan, achan;
	mlib_type dtype;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src1);
	MLIB_IMAGE_CHECK(src2);
	MLIB_IMAGE_CHECK(alpha);
	MLIB_IMAGE_FULL_EQUAL(dst, src1);
	MLIB_IMAGE_FULL_EQUAL(dst, src2);
	MLIB_IMAGE_TYPE_EQUAL(dst, alpha);
	MLIB_IMAGE_SIZE_EQUAL(dst, alpha);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, nchan, dw, dh, dlb, da);

	slb1 = mlib_ImageGetStride(src1);
	slb2 = mlib_ImageGetStride(src2);
	alb = mlib_ImageGetStride(alpha);

	sa1 = mlib_ImageGetData(src1);
	sa2 = mlib_ImageGetData(src2);
	aa = mlib_ImageGetData(alpha);

	achan = mlib_ImageGetChannels(alpha);

	dw *= achan;

	if (!mlib_ImageIsNotOneDvector(alpha) &&
	    !mlib_ImageIsNotOneDvector(src1) &&
	    !mlib_ImageIsNotOneDvector(src2) &&
	    !mlib_ImageIsNotOneDvector(dst)) {
		dw *= dh;
		slb1 *= dh;
		slb2 *= dh;
		dlb *= dh;
		alb *= dh;
		dh = 1;
	}

	if (achan == nchan) {
		if (dtype == MLIB_BYTE) {
			mlib_m_ImageBlend_U8((mlib_u8 *)sa1, slb1,
			    (mlib_u8 *)sa2, slb2,
			    (mlib_u8 *)da, dlb, (mlib_u8 *)aa, alb, dw, dh);
			return (MLIB_SUCCESS);
		} else if (dtype == MLIB_SHORT) {
			mlib_m_ImageBlend_S16((mlib_s16 *)sa1, slb1,
			    (mlib_s16 *)sa2, slb2,
			    (mlib_s16 *)da, dlb, (mlib_s16 *)aa, alb, dw, dh);
			return (MLIB_SUCCESS);
		} else if (dtype == MLIB_USHORT) {
			mlib_m_ImageBlend_U16((mlib_u16 *)sa1, slb1,
			    (mlib_u16 *)sa2, slb2,
			    (mlib_u16 *)da, dlb, (mlib_u16 *)aa, alb, dw, dh);
			return (MLIB_SUCCESS);
		} else if (dtype == MLIB_INT) {
			mlib_m_ImageBlend_S32((mlib_s32 *)sa1, slb1,
			    (mlib_s32 *)sa2, slb2,
			    (mlib_s32 *)da, dlb, (mlib_s32 *)aa, alb, dw, dh);
			return (MLIB_SUCCESS);
		}
	} else if (achan == 1) {
		if (dtype == MLIB_BYTE) {
			switch (nchan) {
			case 2:
				mlib_m_ImageBlendSA_U8_2((mlib_u8 *)sa1, slb1,
				    (mlib_u8 *)sa2, slb2,
				    (mlib_u8 *)da, dlb,
				    (mlib_u8 *)aa, alb, dw, dh);
				break;

			case 3:
				mlib_m_ImageBlendSA_U8_3((mlib_u8 *)sa1, slb1,
				    (mlib_u8 *)sa2, slb2,
				    (mlib_u8 *)da, dlb,
				    (mlib_u8 *)aa, alb, dw, dh);
				break;

			case 4:
				mlib_m_ImageBlendSA_U8_4((mlib_u8 *)sa1, slb1,
				    (mlib_u8 *)sa2, slb2,
				    (mlib_u8 *)da, dlb,
				    (mlib_u8 *)aa, alb, dw, dh);
				break;

			default:
				return (MLIB_FAILURE);
			}

			return (MLIB_SUCCESS);
		} else if (dtype == MLIB_SHORT) {
			switch (nchan) {
			case 2:
			    {
				    mlib_m_ImageBlendSA_S16_2((mlib_s16 *)sa1,
					slb1, (mlib_s16 *)sa2, slb2,
					(mlib_s16 *)da, dlb, (mlib_s16 *)aa,
					alb, dw, dh);
			    }
				break;

			case 3:
			    {
				    mlib_m_ImageBlendSA_S16_3((mlib_s16 *)sa1,
					slb1, (mlib_s16 *)sa2, slb2,
					(mlib_s16 *)da, dlb, (mlib_s16 *)aa,
					alb, dw, dh);
			    }
				break;

			case 4:
			    {
				    mlib_m_ImageBlendSA_S16_4((mlib_s16 *)sa1,
					slb1, (mlib_s16 *)sa2, slb2,
					(mlib_s16 *)da, dlb, (mlib_s16 *)aa,
					alb, dw, dh);
			    }
				break;

			default:
				return (MLIB_FAILURE);
			}
			return (MLIB_SUCCESS);
		} else if (dtype == MLIB_USHORT) {
			switch (nchan) {
			case 2:
			    {
				    mlib_m_ImageBlendSA_U16_2((mlib_u16 *)sa1,
					slb1, (mlib_u16 *)sa2, slb2,
					(mlib_u16 *)da, dlb, (mlib_u16 *)aa,
					alb, dw, dh);
			    }
				break;

			case 3:
			    {
				    mlib_m_ImageBlendSA_U16_3((mlib_u16 *)sa1,
					slb1, (mlib_u16 *)sa2, slb2,
					(mlib_u16 *)da, dlb, (mlib_u16 *)aa,
					alb, dw, dh);
			    }
				break;

			case 4:
			    {
				    mlib_m_ImageBlendSA_U16_4((mlib_u16 *)sa1,
					slb1, (mlib_u16 *)sa2, slb2,
					(mlib_u16 *)da, dlb, (mlib_u16 *)aa,
					alb, dw, dh);
			    }
				break;

			default:
				return (MLIB_FAILURE);
			}
			return (MLIB_SUCCESS);
		} else if (dtype == MLIB_INT) {
			mlib_m_ImageBlendSA_S32((mlib_s32 *)sa1, slb1,
			    (mlib_s32 *)sa2, slb2,
			    (mlib_s32 *)da, dlb,
			    (mlib_s32 *)aa, alb, dw, dh, nchan);
			return (MLIB_SUCCESS);
		}
	}
	return (MLIB_FAILURE);
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_BLEND_U8(dd1, sd1, sd2, sda)                          \
	{                                                                  \
	    __m64 a00 = _mm_sub_pi8(ones, sda);                            \
	    __m64 a10 = _mm_unpackhi_pi8(sda, zero);                       \
	    __m64 a11 = _mm_unpacklo_pi8(sda, zero);                       \
	    __m64 a20 = _mm_unpackhi_pi8(a00, zero);                       \
	    __m64 a21 = _mm_unpacklo_pi8(a00, zero);                       \
	    __m64 r10 = _mm_unpackhi_pi8(sd1, zero);                       \
	    __m64 r11 = _mm_unpacklo_pi8(sd1, zero);                       \
	    __m64 r20 = _mm_unpackhi_pi8(sd2, zero);                       \
	    __m64 r21 = _mm_unpacklo_pi8(sd2, zero);                       \
	    __m64 r30 = _mm_mullo_pi16(r10, a10);                          \
	    __m64 r31 = _mm_mullo_pi16(r11, a11);                          \
	    __m64 r40 = _mm_mullo_pi16(r20, a20);                          \
	    __m64 r41 = _mm_mullo_pi16(r21, a21);                          \
	    __m64 r50 = _mm_add_pi16(_mm_add_pi16(r30, r40), rand);        \
	    __m64 r51 = _mm_add_pi16(_mm_add_pi16(r31, r41), rand);        \
	                                                                   \
	    dd1 =                                                          \
		_mm_packs_pu16(_mm_srli_pi16(r51, 8), _mm_srli_pi16(r50,   \
		8));                                                       \
	}

/* *********************************************************** */

static void
mlib_m_ImageBlend_U8(
    mlib_u8 *src1,
    mlib_s32 slb1,
    mlib_u8 *src2,
    mlib_s32 slb2,
    mlib_u8 *dst1,
    mlib_s32 dlb1,
    mlib_u8 *srca,
    mlib_s32 slba,
    mlib_s32 dw,
    mlib_s32 dh)
{
	const __m64 ones = _mm_set1_pi8(0xff);
	const __m64 zero = _mm_set1_pi8(0x00);
	const __m64 rand = _mm_set1_pi16(0x80);

/* src address */
	__m64 *sp1, *sl1;

/* src address */
	__m64 *sp2, *sl2;

/* src address */
	__m64 *spa, *sla;

/* dst address */
	__m64 *dp1, *dl1;

/* destination data */
	__m64 sd1, sd2, sda, dd1;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1, nu;

	sp1 = sl1 = (__m64 *) src1;
	sp2 = sl2 = (__m64 *) src2;
	spa = sla = (__m64 *) srca;
	dp1 = dl1 = (__m64 *) dst1;

	for (; dh > 0; dh--) {
		n1 = dw;

		nu = ((8 - (mlib_addr)dp1) & 7);
		if ((nu > 0) && (nu <= n1)) {
			mlib_s32 nr = 8 - nu;

			emask = 0xFF << nr;
			sd1 = *sp1;
			sd2 = *sp2;
			sda = *spa;
			MLIB_M_IMAGE_BLEND_U8(dd1, sd1, sd2, sda);
			mmx_pst_8(dd1, dp1, emask);
			spa = (__m64 *) ((mlib_u8 *)sla + nu);
			sp1 = (__m64 *) ((mlib_u8 *)sl1 + nu);
			sp2 = (__m64 *) ((mlib_u8 *)sl2 + nu);
			dp1 = (__m64 *) ((mlib_u8 *)dl1 + nu);
			n1 -= nu;
		}

		for (; n1 > 7; n1 -= 8) {
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			sda = (*spa++);
			MLIB_M_IMAGE_BLEND_U8(dd1, sd1, sd2, sda);
			(*dp1++) = dd1;
		}

		if (n1 > 0) {
			emask = (0xFF << (8 - n1));
			sd1 = *sp1;
			sd2 = *sp2;
			sda = *spa;
			MLIB_M_IMAGE_BLEND_U8(dd1, sd1, sd2, sda);
			mmx_pst_8(dd1, dp1, emask);
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb1);
		sp2 = sl2 = (__m64 *) ((mlib_u8 *)sl2 + slb2);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb1);
		spa = sla = (__m64 *) ((mlib_u8 *)sla + slba);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_CHANNEL2_U8(sda1, sda2, sda)               \
	{                                                       \
	    sda1 = _mm_unpacklo_pi8(sda, sda);                  \
	    sda2 = _mm_unpackhi_pi8(sda, sda);                  \
	}

/* *********************************************************** */

static void
mlib_m_ImageBlendSA_U8_2(
    mlib_u8 *src1,
    mlib_s32 slb1,
    mlib_u8 *src2,
    mlib_s32 slb2,
    mlib_u8 *dst1,
    mlib_s32 dlb1,
    mlib_u8 *srca,
    mlib_s32 slba,
    mlib_s32 dw,
    mlib_s32 dh)
{
	const __m64 ones = _mm_set1_pi8(0xff);
	const __m64 zero = _mm_set1_pi8(0x00);
	const __m64 rand = _mm_set1_pi16(0x80);

/* src address */
	__m64 *sp1, *sl1;

/* src address */
	__m64 *sp2, *sl2;

/* src address */
	__m64 *spa, *sla;

/* dst address */
	__m64 *dp1, *dl1;

/* destination data */
	__m64 sd1, sd2, sda, dd1;
	__m64 sda1, sda2;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;
	mlib_u8 *dend;

	sp1 = sl1 = (__m64 *) src1;
	sp2 = sl2 = (__m64 *) src2;
	spa = sla = (__m64 *) srca;
	dp1 = dl1 = (__m64 *) dst1;

	dw *= 2;

	for (; dh > 0; dh--) {
		n1 = dw;
		dend = (mlib_u8 *)dp1 + dw;

		for (; n1 > 15; n1 -= 16) {
			sda = (*spa++);
			MLIB_M_IMAGE_CHANNEL2_U8(sda1, sda2, sda);
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_U8(dd1, sd1, sd2, sda1);
			(*dp1++) = dd1;
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_U8(dd1, sd1, sd2, sda2);
			(*dp1++) = dd1;
		}

		if (n1 > 0) {
			emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
			sda = *spa;
			MLIB_M_IMAGE_CHANNEL2_U8(sda1, sda2, sda);
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_U8(dd1, sd1, sd2, sda1);
			mmx_pst_8(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp1);
			if (n1 > 0) {
				emask = (0xFF << (8 - n1));
				sd1 = *sp1;
				sd2 = *sp2;
				MLIB_M_IMAGE_BLEND_U8(dd1, sd1, sd2, sda2);
				mmx_pst_8(dd1, dp1, emask);
			}
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb1);
		sp2 = sl2 = (__m64 *) ((mlib_u8 *)sl2 + slb2);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb1);
		spa = sla = (__m64 *) ((mlib_u8 *)sla + slba);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_CHANNEL3_U8(sda1, sda2, sda3, s)                      \
	{                                                                  \
	    __m64 s0 = _mm_unpacklo_pi8(s, _mm_srli_si64(s, 8));           \
	    __m64 s2 =                                                     \
		_mm_unpacklo_pi8(s, _mm_unpacklo_pi8(s, _mm_srli_si64(s,   \
		16)));                                                     \
	    __m64 s3 = _mm_srli_si64(s, 24);                               \
	    __m64 s4 = _mm_unpacklo_pi8(s3, _mm_srli_si64(s, 40));         \
	    __m64 s5 = _mm_unpacklo_pi8(s3, s4);                           \
	    __m64 s6 =                                                     \
		_mm_unpacklo_pi8(_mm_srli_si64(s, 16), _mm_srli_si64(s,    \
		32));                                                      \
	    __m64 s7 = _mm_unpackhi_pi8(_mm_slli_si64(s, 8), s);           \
	    __m64 s8 = _mm_unpackhi_pi8(_mm_slli_si64(s, 16), s);          \
	    __m64 s9 = _mm_unpackhi_pi8(s8, s);                            \
	                                                                   \
	    sda1 = _mm_unpacklo_pi8(s2, s0);                               \
	    sda2 = _mm_unpacklo_pi8(_mm_unpacklo_pi8(s6, s3), s5);         \
	    sda3 = _mm_unpackhi_pi8(s7, s9);                               \
	}

/* *********************************************************** */

static void
mlib_m_ImageBlendSA_U8_3(
    mlib_u8 *src1,
    mlib_s32 slb1,
    mlib_u8 *src2,
    mlib_s32 slb2,
    mlib_u8 *dst1,
    mlib_s32 dlb1,
    mlib_u8 *srca,
    mlib_s32 slba,
    mlib_s32 dw,
    mlib_s32 dh)
{
	const __m64 ones = _mm_set1_pi8(0xff);
	const __m64 zero = _mm_set1_pi8(0x00);
	const __m64 rand = _mm_set1_pi16(0x80);

/* src address */
	__m64 *sp1, *sl1;

/* src address */
	__m64 *sp2, *sl2;

/* src address */
	__m64 *spa, *sla;

/* dst address */
	__m64 *dp1, *dl1;

/* destination data */
	__m64 sd1, sd2, sda, dd1;
	__m64 sda1, sda2, sda3;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;
	mlib_u8 *dend;

	sp1 = sl1 = (__m64 *) src1;
	sp2 = sl2 = (__m64 *) src2;
	spa = sla = (__m64 *) srca;
	dp1 = dl1 = (__m64 *) dst1;

	dw *= 3;

	for (; dh > 0; dh--) {
		n1 = dw;
		dend = (mlib_u8 *)dp1 + dw;

		for (; n1 > 23; n1 -= 24) {
			sda = (*spa++);
			MLIB_M_IMAGE_CHANNEL3_U8(sda1, sda2, sda3, sda);
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_U8(dd1, sd1, sd2, sda1);
			(*dp1++) = dd1;
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_U8(dd1, sd1, sd2, sda2);
			(*dp1++) = dd1;
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_U8(dd1, sd1, sd2, sda3);
			(*dp1++) = dd1;
		}

		if (n1 > 0) {
			emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
			sda = *spa;
			MLIB_M_IMAGE_CHANNEL3_U8(sda1, sda2, sda3, sda);
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_U8(dd1, sd1, sd2, sda1);
			mmx_pst_8(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp1);
			if (n1 > 0) {
				emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
				sd1 = (*sp1++);
				sd2 = (*sp2++);
				MLIB_M_IMAGE_BLEND_U8(dd1, sd1, sd2, sda2);
				mmx_pst_8(dd1, dp1, emask);
				dp1++;

				n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp1);
				if (n1 > 0) {
					emask = (0xFF << (8 - n1));
					sd1 = *sp1;
					sd2 = *sp2;
					MLIB_M_IMAGE_BLEND_U8(dd1, sd1, sd2,
					    sda3);
					mmx_pst_8(dd1, dp1, emask);
				}
			}
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb1);
		sp2 = sl2 = (__m64 *) ((mlib_u8 *)sl2 + slb2);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb1);
		spa = sla = (__m64 *) ((mlib_u8 *)sla + slba);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_CHANNEL4_U8(sda1, sda2, sda3, sda4, s)     \
	{                                                       \
	    __m64 s0 = _mm_unpacklo_pi8(s, s);                  \
	    __m64 s1 = _mm_srli_si64(s, 16);                    \
	    __m64 s2 = _mm_unpacklo_pi8(s1, s1);                \
	    __m64 s3 = _mm_slli_si64(s, 16);                    \
	    __m64 s4 = _mm_unpackhi_pi8(s3, s3);                \
	    __m64 s5 = _mm_unpackhi_pi8(s, s);                  \
	                                                        \
	    sda1 = _mm_unpacklo_pi8(s0, s0);                    \
	    sda2 = _mm_unpacklo_pi8(s2, s2);                    \
	    sda3 = _mm_unpackhi_pi8(s4, s4);                    \
	    sda4 = _mm_unpackhi_pi8(s5, s5);                    \
	}

/* *********************************************************** */

static void
mlib_m_ImageBlendSA_U8_4(
    mlib_u8 *src1,
    mlib_s32 slb1,
    mlib_u8 *src2,
    mlib_s32 slb2,
    mlib_u8 *dst1,
    mlib_s32 dlb1,
    mlib_u8 *srca,
    mlib_s32 slba,
    mlib_s32 dw,
    mlib_s32 dh)
{
	const __m64 ones = _mm_set1_pi8(0xff);
	const __m64 zero = _mm_set1_pi8(0x00);
	const __m64 rand = _mm_set1_pi16(0x80);

/* src address */
	__m64 *sp1, *sl1;

/* src address */
	__m64 *sp2, *sl2;

/* src address */
	__m64 *spa, *sla;

/* dst address */
	__m64 *dp1, *dl1;

/* destination data */
	__m64 sd1, sd2, sda, dd1;
	__m64 sda1, sda2, sda3, sda4;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;
	mlib_u8 *dend;

	sp1 = sl1 = (__m64 *) src1;
	sp2 = sl2 = (__m64 *) src2;
	spa = sla = (__m64 *) srca;
	dp1 = dl1 = (__m64 *) dst1;

	dw *= 4;

	for (; dh > 0; dh--) {
		n1 = dw;
		dend = (mlib_u8 *)dp1 + dw;

		for (; n1 > 31; n1 -= 32) {
			sda = (*spa++);
			MLIB_M_IMAGE_CHANNEL4_U8(sda1, sda2, sda3, sda4, sda);
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_U8(dd1, sd1, sd2, sda1);
			(*dp1++) = dd1;
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_U8(dd1, sd1, sd2, sda2);
			(*dp1++) = dd1;
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_U8(dd1, sd1, sd2, sda3);
			(*dp1++) = dd1;
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_U8(dd1, sd1, sd2, sda4);
			(*dp1++) = dd1;
		}

		if (n1 > 0) {
			emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
			sda = *spa;
			MLIB_M_IMAGE_CHANNEL4_U8(sda1, sda2, sda3, sda4, sda);
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_U8(dd1, sd1, sd2, sda1);
			mmx_pst_8(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp1);
			if (n1 > 0) {
				emask = (n1 > 7) ? 0xFF : (0xFF << (8 - n1));
				sd1 = (*sp1++);
				sd2 = (*sp2++);
				MLIB_M_IMAGE_BLEND_U8(dd1, sd1, sd2, sda2);
				mmx_pst_8(dd1, dp1, emask);
				dp1++;

				n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp1);
				if (n1 > 0) {
					emask =
					    (n1 >
					    7) ? 0xFF : (0xFF << (8 - n1));
					sd1 = (*sp1++);
					sd2 = (*sp2++);
					MLIB_M_IMAGE_BLEND_U8(dd1, sd1, sd2,
					    sda3);
					mmx_pst_8(dd1, dp1, emask);
					dp1++;

					n1 = ((mlib_u8 *)dend - (mlib_u8 *)dp1);
					if (n1 > 0) {
						emask = (0xFF << (8 - n1));
						sd1 = *sp1;
						sd2 = *sp2;
						MLIB_M_IMAGE_BLEND_U8(dd1, sd1,
						    sd2, sda4);
						mmx_pst_8(dd1, dp1, emask);
					}
				}
			}
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb1);
		sp2 = sl2 = (__m64 *) ((mlib_u8 *)sl2 + slb2);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb1);
		spa = sla = (__m64 *) ((mlib_u8 *)sla + slba);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_BLEND_S16(dd1, sd1, sd2, sda)                \
	{                                                         \
	    __m64 rd1 = _mm_and_si64(ones, sda);                  \
	    __m64 rd2 = _mm_mulhi_pi16(sd1, rd1);                 \
	                                                          \
	    rd1 = _mm_mulhi_pi16(sd2, _mm_sub_pi16(ones, rd1));   \
	    dd1 = _mm_add_pi16(rd2, rd1);                         \
	    dd1 = _mm_add_pi16(dd1, dd1);                         \
	}

/* *********************************************************** */

static void
mlib_m_ImageBlend_S16(
    mlib_s16 *src1,
    mlib_s32 slb1,
    mlib_s16 *src2,
    mlib_s32 slb2,
    mlib_s16 *dst1,
    mlib_s32 dlb1,
    mlib_s16 *srca,
    mlib_s32 slba,
    mlib_s32 dw,
    mlib_s32 dh)
{
	const __m64 ones = _mm_set1_pi16(0x7fff);

/* src address */
	__m64 *sp1, *sl1;

/* src address */
	__m64 *sp2, *sl2;

/* src address */
	__m64 *spa, *sla;

/* dst address */
	__m64 *dp1, *dl1;

/* destination data */
	__m64 sd1, sd2, sda, dd1;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1, nu;

	sp1 = sl1 = (__m64 *) src1;
	sp2 = sl2 = (__m64 *) src2;
	spa = sla = (__m64 *) srca;
	dp1 = dl1 = (__m64 *) dst1;

	for (; dh > 0; dh--) {
		n1 = dw;

		nu = ((8 - (mlib_addr)dp1) & 7) >> 1;
		if ((nu > 0) && (nu <= n1)) {
			mlib_s32 nr = 4 - nu;

			emask = 0xF << nr;
			sd1 = *sp1;
			sd2 = *sp2;
			sda = *spa;
			MLIB_M_IMAGE_BLEND_S16(dd1, sd1, sd2, sda);
			mmx_pst_16(dd1, dp1, emask);
			spa = (__m64 *) ((mlib_s16 *)sla + nu);
			sp1 = (__m64 *) ((mlib_s16 *)sl1 + nu);
			sp2 = (__m64 *) ((mlib_s16 *)sl2 + nu);
			dp1 = (__m64 *) ((mlib_s16 *)dl1 + nu);
			n1 -= nu;
		}

		for (; n1 > 3; n1 -= 4) {
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			sda = (*spa++);
			MLIB_M_IMAGE_BLEND_S16(dd1, sd1, sd2, sda);
			(*dp1++) = dd1;
		}

		if (n1 > 0) {
			emask = (0xF << (4 - n1));
			sd1 = *sp1;
			sd2 = *sp2;
			sda = *spa;
			MLIB_M_IMAGE_BLEND_S16(dd1, sd1, sd2, sda);
			mmx_pst_16(dd1, dp1, emask);
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb1);
		sp2 = sl2 = (__m64 *) ((mlib_u8 *)sl2 + slb2);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb1);
		spa = sla = (__m64 *) ((mlib_u8 *)sla + slba);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_CHANNEL2_S16(sda1, sda2, sda)              \
	{                                                       \
	    sda1 = _mm_unpacklo_pi16(sda, sda);                 \
	    sda2 = _mm_unpackhi_pi16(sda, sda);                 \
	}

/* *********************************************************** */

static void
mlib_m_ImageBlendSA_S16_2(
    mlib_s16 *src1,
    mlib_s32 slb1,
    mlib_s16 *src2,
    mlib_s32 slb2,
    mlib_s16 *dst1,
    mlib_s32 dlb1,
    mlib_s16 *srca,
    mlib_s32 slba,
    mlib_s32 dw,
    mlib_s32 dh)
{
	const __m64 ones = _mm_set1_pi16(0x7fff);

/* src address */
	__m64 *sp1, *sl1;

/* src address */
	__m64 *sp2, *sl2;

/* src address */
	__m64 *spa, *sla;

/* dst address */
	__m64 *dp1, *dl1;

/* destination data */
	__m64 sd1, sd2, sda, dd1;
	__m64 sda1, sda2;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;
	mlib_s16 *dend;

	sp1 = sl1 = (__m64 *) src1;
	sp2 = sl2 = (__m64 *) src2;
	spa = sla = (__m64 *) srca;
	dp1 = dl1 = (__m64 *) dst1;

	dw *= 2;

	for (; dh > 0; dh--) {
		n1 = dw;
		dend = (mlib_s16 *)dp1 + dw;

		for (; n1 > 7; n1 -= 8) {
			sda = (*spa++);
			MLIB_M_IMAGE_CHANNEL2_S16(sda1, sda2, sda);
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_S16(dd1, sd1, sd2, sda1);
			(*dp1++) = dd1;
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_S16(dd1, sd1, sd2, sda2);
			(*dp1++) = dd1;
		}

		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			sda = *spa;
			MLIB_M_IMAGE_CHANNEL2_S16(sda1, sda2, sda);
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_S16(dd1, sd1, sd2, sda1);
			mmx_pst_16(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
			if (n1 > 0) {
				emask = (0xF << (4 - n1));
				sd1 = *sp1;
				sd2 = *sp2;
				MLIB_M_IMAGE_BLEND_S16(dd1, sd1, sd2, sda2);
				mmx_pst_16(dd1, dp1, emask);
			}
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb1);
		sp2 = sl2 = (__m64 *) ((mlib_u8 *)sl2 + slb2);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb1);
		spa = sla = (__m64 *) ((mlib_u8 *)sla + slba);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_CHANNEL3_S16(sda1, sda2, sda3, s)          \
	{                                                       \
	    __m64 s0 = _mm_unpacklo_pi16(s, s);                 \
	    __m64 s1 = _mm_srli_si64(s, 16);                    \
	    __m64 s2 = _mm_unpackhi_pi16(s, s);                 \
	                                                        \
	    sda1 = _mm_unpacklo_pi32(s0, s);                    \
	    sda2 = _mm_unpacklo_pi16(s1, s1);                   \
	    sda3 = _mm_unpackhi_pi32(s, s2);                    \
	}

/* *********************************************************** */

static void
mlib_m_ImageBlendSA_S16_3(
    mlib_s16 *src1,
    mlib_s32 slb1,
    mlib_s16 *src2,
    mlib_s32 slb2,
    mlib_s16 *dst1,
    mlib_s32 dlb1,
    mlib_s16 *srca,
    mlib_s32 slba,
    mlib_s32 dw,
    mlib_s32 dh)
{
	const __m64 ones = _mm_set1_pi16(0x7fff);

/* src address */
	__m64 *sp1, *sl1;

/* src address */
	__m64 *sp2, *sl2;

/* src address */
	__m64 *spa, *sla;

/* dst address */
	__m64 *dp1, *dl1;

/* destination data */
	__m64 sd1, sd2, sda, dd1;
	__m64 sda1, sda2, sda3;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;
	mlib_s16 *dend;

	sp1 = sl1 = (__m64 *) src1;
	sp2 = sl2 = (__m64 *) src2;
	spa = sla = (__m64 *) srca;
	dp1 = dl1 = (__m64 *) dst1;

	dw *= 3;

	for (; dh > 0; dh--) {
		n1 = dw;
		dend = (mlib_s16 *)dp1 + dw;

		for (; n1 > 11; n1 -= 12) {
			sda = (*spa++);
			MLIB_M_IMAGE_CHANNEL3_S16(sda1, sda2, sda3, sda);
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_S16(dd1, sd1, sd2, sda1);
			(*dp1++) = dd1;
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_S16(dd1, sd1, sd2, sda2);
			(*dp1++) = dd1;
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_S16(dd1, sd1, sd2, sda3);
			(*dp1++) = dd1;
		}

		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			sda = *spa;
			MLIB_M_IMAGE_CHANNEL3_S16(sda1, sda2, sda3, sda);
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_S16(dd1, sd1, sd2, sda1);
			mmx_pst_16(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
			if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				sd1 = (*sp1++);
				sd2 = (*sp2++);
				MLIB_M_IMAGE_BLEND_S16(dd1, sd1, sd2, sda2);
				mmx_pst_16(dd1, dp1, emask);
				dp1++;

				n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
				if (n1 > 0) {
					emask = (0xF << (4 - n1));
					sd1 = *sp1;
					sd2 = *sp2;
					MLIB_M_IMAGE_BLEND_S16(dd1, sd1, sd2,
					    sda3);
					mmx_pst_16(dd1, dp1, emask);
				}
			}
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb1);
		sp2 = sl2 = (__m64 *) ((mlib_u8 *)sl2 + slb2);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb1);
		spa = sla = (__m64 *) ((mlib_u8 *)sla + slba);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_CHANNEL4_S16(sda1, sda2, sda3, sda4, s)    \
	{                                                       \
	    __m64 s0 = _mm_unpacklo_pi16(s, s);                 \
	    __m64 s1 = _mm_srli_si64(s, 16);                    \
	    __m64 s2 = _mm_unpacklo_pi16(s1, s1);               \
	    __m64 s3 = _mm_slli_si64(s, 16);                    \
	    __m64 s4 = _mm_unpackhi_pi16(s3, s3);               \
	    __m64 s5 = _mm_unpackhi_pi16(s, s);                 \
	                                                        \
	    sda1 = _mm_unpacklo_pi32(s0, s0);                   \
	    sda2 = _mm_unpacklo_pi32(s2, s2);                   \
	    sda3 = _mm_unpackhi_pi32(s4, s4);                   \
	    sda4 = _mm_unpackhi_pi32(s5, s5);                   \
	}

/* *********************************************************** */

static void
mlib_m_ImageBlendSA_S16_4(
    mlib_s16 *src1,
    mlib_s32 slb1,
    mlib_s16 *src2,
    mlib_s32 slb2,
    mlib_s16 *dst1,
    mlib_s32 dlb1,
    mlib_s16 *srca,
    mlib_s32 slba,
    mlib_s32 dw,
    mlib_s32 dh)
{
	const __m64 ones = _mm_set1_pi16(0x7fff);

/* src address */
	__m64 *sp1, *sl1;

/* src address */
	__m64 *sp2, *sl2;

/* src address */
	__m64 *spa, *sla;

/* dst address */
	__m64 *dp1, *dl1;

/* destination data */
	__m64 sd1, sd2, sda, dd1;
	__m64 sda1, sda2, sda3, sda4;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;
	mlib_s16 *dend;

	sp1 = sl1 = (__m64 *) src1;
	sp2 = sl2 = (__m64 *) src2;
	spa = sla = (__m64 *) srca;
	dp1 = dl1 = (__m64 *) dst1;

	dw *= 4;

	for (; dh > 0; dh--) {
		n1 = dw;
		dend = (mlib_s16 *)dp1 + dw;

		for (; n1 > 15; n1 -= 16) {
			sda = (*spa++);
			MLIB_M_IMAGE_CHANNEL4_S16(sda1, sda2, sda3, sda4, sda);
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_S16(dd1, sd1, sd2, sda1);
			(*dp1++) = dd1;
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_S16(dd1, sd1, sd2, sda2);
			(*dp1++) = dd1;
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_S16(dd1, sd1, sd2, sda3);
			(*dp1++) = dd1;
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_S16(dd1, sd1, sd2, sda4);
			(*dp1++) = dd1;
		}

		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			sda = *spa;
			MLIB_M_IMAGE_CHANNEL4_S16(sda1, sda2, sda3, sda4, sda);
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_S16(dd1, sd1, sd2, sda1);
			mmx_pst_16(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
			if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				sd1 = (*sp1++);
				sd2 = (*sp2++);
				MLIB_M_IMAGE_BLEND_S16(dd1, sd1, sd2, sda2);
				mmx_pst_16(dd1, dp1, emask);
				dp1++;

				n1 = ((mlib_s16 *)dend - (mlib_s16 *)dp1);
				if (n1 > 0) {
					emask =
					    (n1 > 3) ? 0xF : (0xF << (4 - n1));
					sd1 = (*sp1++);
					sd2 = (*sp2++);
					MLIB_M_IMAGE_BLEND_S16(dd1, sd1, sd2,
					    sda3);
					mmx_pst_16(dd1, dp1, emask);
					dp1++;

					n1 = ((mlib_s16 *)dend -
					    (mlib_s16 *)dp1);
					if (n1 > 0) {
						emask = (0xF << (4 - n1));
						sd1 = *sp1;
						sd2 = *sp2;
						MLIB_M_IMAGE_BLEND_S16(dd1, sd1,
						    sd2, sda4);
						mmx_pst_16(dd1, dp1, emask);
					}
				}
			}
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb1);
		sp2 = sl2 = (__m64 *) ((mlib_u8 *)sl2 + slb2);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb1);
		spa = sla = (__m64 *) ((mlib_u8 *)sla + slba);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_BLEND_U16(dd1, sd1, sd2, sda)              \
	{                                                       \
	    __m64 rd1 = _mm_sub_pi16(sd1, fmax);                \
	    __m64 rd2 = _mm_sub_pi16(sd2, fmax);                \
	    __m64 ad = _mm_srli_pi16(sda, 1);                   \
	    __m64 bd = _mm_sub_pi16(ones, ad);                  \
	                                                        \
	    rd1 = _mm_mulhi_pi16(rd1, ad);                      \
	    rd2 = _mm_mulhi_pi16(rd2, bd);                      \
	    dd1 = _mm_add_pi16(rd1, rd2);                       \
	    dd1 = _mm_add_pi16(_mm_add_pi16(dd1, dd1), fmax);   \
	}

/* *********************************************************** */

static void
mlib_m_ImageBlend_U16(
    mlib_u16 *src1,
    mlib_s32 slb1,
    mlib_u16 *src2,
    mlib_s32 slb2,
    mlib_u16 *dst1,
    mlib_s32 dlb1,
    mlib_u16 *srca,
    mlib_s32 slba,
    mlib_s32 dw,
    mlib_s32 dh)
{
	const __m64 ones = _mm_set1_pi16(0x7fff);
	const __m64 fmax = _mm_set1_pi16(0x8000);

/* src address */
	__m64 *sp1, *sl1;

/* src address */
	__m64 *sp2, *sl2;

/* src address */
	__m64 *spa, *sla;

/* dst address */
	__m64 *dp1, *dl1;

/* destination data */
	__m64 sd1, sd2, sda, dd1;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1, nu;

	sp1 = sl1 = (__m64 *) src1;
	sp2 = sl2 = (__m64 *) src2;
	spa = sla = (__m64 *) srca;
	dp1 = dl1 = (__m64 *) dst1;

	for (; dh > 0; dh--) {
		n1 = dw;

		nu = ((8 - (mlib_addr)dp1) & 7) >> 1;
		if ((nu > 0) && (nu <= n1)) {
			mlib_s32 nr = 4 - nu;

			emask = 0xF << nr;
			sd1 = *sp1;
			sd2 = *sp2;
			sda = *spa;
			MLIB_M_IMAGE_BLEND_U16(dd1, sd1, sd2, sda);
			mmx_pst_16(dd1, dp1, emask);
			spa = (__m64 *) ((mlib_u16 *)sla + nu);
			sp1 = (__m64 *) ((mlib_u16 *)sl1 + nu);
			sp2 = (__m64 *) ((mlib_u16 *)sl2 + nu);
			dp1 = (__m64 *) ((mlib_u16 *)dl1 + nu);
			n1 -= nu;
		}

		for (; n1 > 3; n1 -= 4) {
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			sda = (*spa++);
			MLIB_M_IMAGE_BLEND_U16(dd1, sd1, sd2, sda);
			(*dp1++) = dd1;
		}

		if (n1 > 0) {
			emask = (0xF << (4 - n1));
			sd1 = *sp1;
			sd2 = *sp2;
			sda = *spa;
			MLIB_M_IMAGE_BLEND_U16(dd1, sd1, sd2, sda);
			mmx_pst_16(dd1, dp1, emask);
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb1);
		sp2 = sl2 = (__m64 *) ((mlib_u8 *)sl2 + slb2);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb1);
		spa = sla = (__m64 *) ((mlib_u8 *)sla + slba);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_CHANNEL2_U16(sda1, sda2, sda)              \
	{                                                       \
	    sda1 = _mm_unpacklo_pi16(sda, sda);                 \
	    sda2 = _mm_unpackhi_pi16(sda, sda);                 \
	}

/* *********************************************************** */

static void
mlib_m_ImageBlendSA_U16_2(
    mlib_u16 *src1,
    mlib_s32 slb1,
    mlib_u16 *src2,
    mlib_s32 slb2,
    mlib_u16 *dst1,
    mlib_s32 dlb1,
    mlib_u16 *srca,
    mlib_s32 slba,
    mlib_s32 dw,
    mlib_s32 dh)
{
	const __m64 ones = _mm_set1_pi16(0x7fff);
	const __m64 fmax = _mm_set1_pi16(0x8000);

/* src address */
	__m64 *sp1, *sl1;

/* src address */
	__m64 *sp2, *sl2;

/* src address */
	__m64 *spa, *sla;

/* dst address */
	__m64 *dp1, *dl1;

/* destination data */
	__m64 sd1, sd2, sda, dd1;
	__m64 sda1, sda2;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;
	mlib_u16 *dend;

	sp1 = sl1 = (__m64 *) src1;
	sp2 = sl2 = (__m64 *) src2;
	spa = sla = (__m64 *) srca;
	dp1 = dl1 = (__m64 *) dst1;

	dw *= 2;

	for (; dh > 0; dh--) {
		n1 = dw;
		dend = (mlib_u16 *)dp1 + dw;

		for (; n1 > 7; n1 -= 8) {
			sda = (*spa++);
			MLIB_M_IMAGE_CHANNEL2_U16(sda1, sda2, sda);
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_U16(dd1, sd1, sd2, sda1);
			(*dp1++) = dd1;
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_U16(dd1, sd1, sd2, sda2);
			(*dp1++) = dd1;
		}

		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			sda = *spa;
			MLIB_M_IMAGE_CHANNEL2_U16(sda1, sda2, sda);
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_U16(dd1, sd1, sd2, sda1);
			mmx_pst_16(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_u16 *)dend - (mlib_u16 *)dp1);
			if (n1 > 0) {
				emask = (0xF << (4 - n1));
				sd1 = *sp1;
				sd2 = *sp2;
				MLIB_M_IMAGE_BLEND_U16(dd1, sd1, sd2, sda2);
				mmx_pst_16(dd1, dp1, emask);
			}
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb1);
		sp2 = sl2 = (__m64 *) ((mlib_u8 *)sl2 + slb2);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb1);
		spa = sla = (__m64 *) ((mlib_u8 *)sla + slba);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_CHANNEL3_U16(sda1, sda2, sda3, s)          \
	{                                                       \
	    __m64 s0 = _mm_unpacklo_pi16(s, s);                 \
	    __m64 s1 = _mm_srli_si64(s, 16);                    \
	    __m64 s2 = _mm_unpackhi_pi16(s, s);                 \
	                                                        \
	    sda1 = _mm_unpacklo_pi32(s0, s);                    \
	    sda2 = _mm_unpacklo_pi16(s1, s1);                   \
	    sda3 = _mm_unpackhi_pi32(s, s2);                    \
	}

/* *********************************************************** */

static void
mlib_m_ImageBlendSA_U16_3(
    mlib_u16 *src1,
    mlib_s32 slb1,
    mlib_u16 *src2,
    mlib_s32 slb2,
    mlib_u16 *dst1,
    mlib_s32 dlb1,
    mlib_u16 *srca,
    mlib_s32 slba,
    mlib_s32 dw,
    mlib_s32 dh)
{
	const __m64 ones = _mm_set1_pi16(0x7fff);
	const __m64 fmax = _mm_set1_pi16(0x8000);

/* src address */
	__m64 *sp1, *sl1;

/* src address */
	__m64 *sp2, *sl2;

/* src address */
	__m64 *spa, *sla;

/* dst address */
	__m64 *dp1, *dl1;

/* destination data */
	__m64 sd1, sd2, sda, dd1;
	__m64 sda1, sda2, sda3;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;
	mlib_u16 *dend;

	sp1 = sl1 = (__m64 *) src1;
	sp2 = sl2 = (__m64 *) src2;
	spa = sla = (__m64 *) srca;
	dp1 = dl1 = (__m64 *) dst1;

	dw *= 3;

	for (; dh > 0; dh--) {
		n1 = dw;
		dend = (mlib_u16 *)dp1 + dw;

		for (; n1 > 11; n1 -= 12) {
			sda = (*spa++);
			MLIB_M_IMAGE_CHANNEL3_U16(sda1, sda2, sda3, sda);
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_U16(dd1, sd1, sd2, sda1);
			(*dp1++) = dd1;
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_U16(dd1, sd1, sd2, sda2);
			(*dp1++) = dd1;
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_U16(dd1, sd1, sd2, sda3);
			(*dp1++) = dd1;
		}

		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			sda = *spa;
			MLIB_M_IMAGE_CHANNEL3_U16(sda1, sda2, sda3, sda);
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_U16(dd1, sd1, sd2, sda1);
			mmx_pst_16(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_u16 *)dend - (mlib_u16 *)dp1);
			if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				sd1 = (*sp1++);
				sd2 = (*sp2++);
				MLIB_M_IMAGE_BLEND_U16(dd1, sd1, sd2, sda2);
				mmx_pst_16(dd1, dp1, emask);
				dp1++;

				n1 = ((mlib_u16 *)dend - (mlib_u16 *)dp1);
				if (n1 > 0) {
					emask = (0xF << (4 - n1));
					sd1 = *sp1;
					sd2 = *sp2;
					MLIB_M_IMAGE_BLEND_U16(dd1, sd1, sd2,
					    sda3);
					mmx_pst_16(dd1, dp1, emask);
				}
			}
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb1);
		sp2 = sl2 = (__m64 *) ((mlib_u8 *)sl2 + slb2);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb1);
		spa = sla = (__m64 *) ((mlib_u8 *)sla + slba);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_CHANNEL4_U16(sda1, sda2, sda3, sda4, s)    \
	{                                                       \
	    __m64 s0 = _mm_unpacklo_pi16(s, s);                 \
	    __m64 s1 = _mm_srli_si64(s, 16);                    \
	    __m64 s2 = _mm_unpacklo_pi16(s1, s1);               \
	    __m64 s3 = _mm_slli_si64(s, 16);                    \
	    __m64 s4 = _mm_unpackhi_pi16(s3, s3);               \
	    __m64 s5 = _mm_unpackhi_pi16(s, s);                 \
	                                                        \
	    sda1 = _mm_unpacklo_pi32(s0, s0);                   \
	    sda2 = _mm_unpacklo_pi32(s2, s2);                   \
	    sda3 = _mm_unpackhi_pi32(s4, s4);                   \
	    sda4 = _mm_unpackhi_pi32(s5, s5);                   \
	}

/* *********************************************************** */

static void
mlib_m_ImageBlendSA_U16_4(
    mlib_u16 *src1,
    mlib_s32 slb1,
    mlib_u16 *src2,
    mlib_s32 slb2,
    mlib_u16 *dst1,
    mlib_s32 dlb1,
    mlib_u16 *srca,
    mlib_s32 slba,
    mlib_s32 dw,
    mlib_s32 dh)
{
	const __m64 ones = _mm_set1_pi16(0x7fff);
	const __m64 fmax = _mm_set1_pi16(0x8000);

/* src address */
	__m64 *sp1, *sl1;

/* src address */
	__m64 *sp2, *sl2;

/* src address */
	__m64 *spa, *sla;

/* dst address */
	__m64 *dp1, *dl1;

/* destination data */
	__m64 sd1, sd2, sda, dd1;
	__m64 sda1, sda2, sda3, sda4;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 n1;
	mlib_u16 *dend;

	sp1 = sl1 = (__m64 *) src1;
	sp2 = sl2 = (__m64 *) src2;
	spa = sla = (__m64 *) srca;
	dp1 = dl1 = (__m64 *) dst1;

	dw *= 4;

	for (; dh > 0; dh--) {
		n1 = dw;
		dend = (mlib_u16 *)dp1 + dw;

		for (; n1 > 15; n1 -= 16) {
			sda = (*spa++);
			MLIB_M_IMAGE_CHANNEL4_U16(sda1, sda2, sda3, sda4, sda);
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_U16(dd1, sd1, sd2, sda1);
			(*dp1++) = dd1;
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_U16(dd1, sd1, sd2, sda2);
			(*dp1++) = dd1;
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_U16(dd1, sd1, sd2, sda3);
			(*dp1++) = dd1;
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_U16(dd1, sd1, sd2, sda4);
			(*dp1++) = dd1;
		}

		if (n1 > 0) {
			emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
			sda = *spa;
			MLIB_M_IMAGE_CHANNEL4_U16(sda1, sda2, sda3, sda4, sda);
			sd1 = (*sp1++);
			sd2 = (*sp2++);
			MLIB_M_IMAGE_BLEND_U16(dd1, sd1, sd2, sda1);
			mmx_pst_16(dd1, dp1, emask);
			dp1++;

			n1 = ((mlib_u16 *)dend - (mlib_u16 *)dp1);
			if (n1 > 0) {
				emask = (n1 > 3) ? 0xF : (0xF << (4 - n1));
				sd1 = (*sp1++);
				sd2 = (*sp2++);
				MLIB_M_IMAGE_BLEND_U16(dd1, sd1, sd2, sda2);
				mmx_pst_16(dd1, dp1, emask);
				dp1++;

				n1 = ((mlib_u16 *)dend - (mlib_u16 *)dp1);
				if (n1 > 0) {
					emask =
					    (n1 > 3) ? 0xF : (0xF << (4 - n1));
					sd1 = (*sp1++);
					sd2 = (*sp2++);
					MLIB_M_IMAGE_BLEND_U16(dd1, sd1, sd2,
					    sda3);
					mmx_pst_16(dd1, dp1, emask);
					dp1++;

					n1 = ((mlib_u16 *)dend -
					    (mlib_u16 *)dp1);
					if (n1 > 0) {
						emask = (0xF << (4 - n1));
						sd1 = *sp1;
						sd2 = *sp2;
						MLIB_M_IMAGE_BLEND_U16(dd1, sd1,
						    sd2, sda4);
						mmx_pst_16(dd1, dp1, emask);
					}
				}
			}
		}

		sp1 = sl1 = (__m64 *) ((mlib_u8 *)sl1 + slb1);
		sp2 = sl2 = (__m64 *) ((mlib_u8 *)sl2 + slb2);
		dp1 = dl1 = (__m64 *) ((mlib_u8 *)dl1 + dlb1);
		spa = sla = (__m64 *) ((mlib_u8 *)sla + slba);
	}

	_mm_empty();
}

/* *********************************************************** */

#define	mlib_fabs	fabs

/* *********************************************************** */

static void
mlib_m_ImageBlend_S32(
    mlib_s32 *src1,
    mlib_s32 slb1,
    mlib_s32 *src2,
    mlib_s32 slb2,
    mlib_s32 *dst1,
    mlib_s32 dlb1,
    mlib_s32 *srca,
    mlib_s32 alb1,
    mlib_s32 dw,
    mlib_s32 dh)
{
/* indices for x and y */
	mlib_s32 i, j;

/* variable for alpha coefficients */
	mlib_d64 fa;
	mlib_d64 fshift = -1.0 / MLIB_S32_MIN;

	for (j = 0; j < dh; j++) {
		for (i = 0; i < dw; i++) {
			fa = mlib_fabs((mlib_d64)srca[i]) * fshift;
			dst1[i] =
			    (mlib_s32)(fa * (mlib_d64)src1[i] + (1 -
			    fa) * (mlib_d64)src2[i]);
		}
		src1 = (mlib_s32 *)((mlib_u8 *)src1 + slb1);
		src2 = (mlib_s32 *)((mlib_u8 *)src2 + slb2);
		srca = (mlib_s32 *)((mlib_u8 *)srca + alb1);
		dst1 = (mlib_s32 *)((mlib_u8 *)dst1 + dlb1);
	}
}

/* *********************************************************** */

static void
mlib_m_ImageBlendSA_S32(
    mlib_s32 *src1,
    mlib_s32 slb1,
    mlib_s32 *src2,
    mlib_s32 slb2,
    mlib_s32 *dst1,
    mlib_s32 dlb1,
    mlib_s32 *srca,
    mlib_s32 alb1,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 ch)
{
/* indices for x and y */
	mlib_s32 i, j;

/* variable for alpha coefficients */
	mlib_d64 fa;
	mlib_d64 fshift = -1.0 / MLIB_S32_MIN;

	if (ch == 2) {
		for (j = 0; j < dh; j++) {
			for (i = 0; i < dw; i++) {
				fa = mlib_fabs((mlib_d64)srca[i]) * fshift;
				dst1[2 * i + 0] =
				    (mlib_s32)(fa * (mlib_d64)src1[2 * i + 0] +
				    (1 - fa) * (mlib_d64)src2[2 * i + 0]);
				dst1[2 * i + 1] =
				    (mlib_s32)(fa * (mlib_d64)src1[2 * i + 1] +
				    (1 - fa) * (mlib_d64)src2[2 * i + 1]);
			}
			src1 = (mlib_s32 *)((mlib_u8 *)src1 + slb1);
			src2 = (mlib_s32 *)((mlib_u8 *)src2 + slb2);
			srca = (mlib_s32 *)((mlib_u8 *)srca + alb1);
			dst1 = (mlib_s32 *)((mlib_u8 *)dst1 + dlb1);
		}
	} else if (ch == 3) {
		for (j = 0; j < dh; j++) {
			for (i = 0; i < dw; i++) {
				fa = mlib_fabs((mlib_d64)srca[i]) * fshift;
				dst1[3 * i + 0] =
				    (mlib_s32)(fa * (mlib_d64)src1[3 * i + 0] +
				    (1 - fa) * (mlib_d64)src2[3 * i + 0]);
				dst1[3 * i + 1] =
				    (mlib_s32)(fa * (mlib_d64)src1[3 * i + 1] +
				    (1 - fa) * (mlib_d64)src2[3 * i + 1]);
				dst1[3 * i + 2] =
				    (mlib_s32)(fa * (mlib_d64)src1[3 * i + 2] +
				    (1 - fa) * (mlib_d64)src2[3 * i + 2]);
			}
			src1 = (mlib_s32 *)((mlib_u8 *)src1 + slb1);
			src2 = (mlib_s32 *)((mlib_u8 *)src2 + slb2);
			srca = (mlib_s32 *)((mlib_u8 *)srca + alb1);
			dst1 = (mlib_s32 *)((mlib_u8 *)dst1 + dlb1);
		}
	} else {
		for (j = 0; j < dh; j++) {
			for (i = 0; i < dw; i++) {
				fa = mlib_fabs((mlib_d64)srca[i]) * fshift;
				dst1[4 * i + 0] =
				    (mlib_s32)(fa * (mlib_d64)src1[4 * i + 0] +
				    (1 - fa) * (mlib_d64)src2[4 * i + 0]);
				dst1[4 * i + 1] =
				    (mlib_s32)(fa * (mlib_d64)src1[4 * i + 1] +
				    (1 - fa) * (mlib_d64)src2[4 * i + 1]);
				dst1[4 * i + 2] =
				    (mlib_s32)(fa * (mlib_d64)src1[4 * i + 2] +
				    (1 - fa) * (mlib_d64)src2[4 * i + 2]);
				dst1[4 * i + 3] =
				    (mlib_s32)(fa * (mlib_d64)src1[4 * i + 3] +
				    (1 - fa) * (mlib_d64)src2[4 * i + 3]);
			}
			src1 = (mlib_s32 *)((mlib_u8 *)src1 + slb1);
			src2 = (mlib_s32 *)((mlib_u8 *)src2 + slb2);
			srca = (mlib_s32 *)((mlib_u8 *)srca + alb1);
			dst1 = (mlib_s32 *)((mlib_u8 *)dst1 + dlb1);
		}
	}
}

/* *********************************************************** */
