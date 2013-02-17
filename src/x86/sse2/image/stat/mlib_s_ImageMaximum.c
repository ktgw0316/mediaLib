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

#pragma ident	"@(#)mlib_s_ImageMaximum.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageMaximum - calculate maximum for input image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMaximum(mlib_s32         *max,
 *                                    const mlib_image *img)
 *
 * ARGUMENTS
 *      max     pointer to max vector
 *      img     pointer to an image
 *
 * RESTRICTION
 *      The image can have 1, 2, 3 or 4 channels.
 *      It can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *
 *      max = MAX (img[i][j]);
 *            i, j
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageExtrema.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMaximum = __mlib_ImageMaximum

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMaximum) mlib_ImageMaximum
    __attribute__((weak, alias("__mlib_ImageMaximum")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_S_IMAGE_MAXIMUM_S32(dst, src1, src2)              \
	{                                                      \
	    __m128i cmask = _mm_cmpgt_epi32(src1, src2);       \
	    dst =                                              \
		_mm_or_si128(_mm_and_si128(src1, cmask),       \
				_mm_andnot_si128(cmask, src2));\
	}
/* *********************************************************** */

static void mlib_s_ImageMaximum_U8_124(
    mlib_s32 *res,
    const mlib_image *img);

static void mlib_s_ImageMaximum_U8_3(
    mlib_s32 *res,
    const mlib_image *img);

static void mlib_s_ImageMaximum_S16_124(
    mlib_s32 *res,
    const mlib_image *img);

static void mlib_s_ImageMaximum_S16_3(
    mlib_s32 *res,
    const mlib_image *img);

static void mlib_s_ImageMaximum_U16_124(
    mlib_s32 *res,
    const mlib_image *img);

static void mlib_s_ImageMaximum_U16_3(
    mlib_s32 *res,
    const mlib_image *img);

static void mlib_s_ImageMaximum_S32_124(
    mlib_s32 *res,
    const mlib_image *img);

static void mlib_s_ImageMaximum_S32_3(
    mlib_s32 *res,
    const mlib_image *img);

/* *********************************************************** */

void
mlib_s_ImageMaximum_U8_124(
    mlib_s32 *res,
    const mlib_image *img)
{
/* src address */
	__m128i *sp, *sl;
/* src data */
	__m128i sd;
/* max values */
	__m128i max;

/* intermediate variables */
	__m128i _8s16_1, _8s16_2;
	__m128i _4s32_1, _4s32_2;
	__m128i _m_zero = _mm_setzero_si128();

/* loop variables */
	mlib_s32 n1;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img);

/* number of image channels */
	mlib_s32 channels = mlib_ImageGetChannels(img);
	mlib_s32 size_row = mlib_ImageGetWidth(img) * channels;

	if (src_stride == size_row) {
		size_row *= height;
		height = 1;
	}
	sp = sl = (__m128i *)mlib_ImageGetData(img);
/* max values */
	max = _mm_set1_epi8(MLIB_U8_MIN);

	for (; height > 0; height--) {
		n1 = size_row;
		for (; n1 > 15; n1 -= 16) {
		    sd = _mm_loadu_si128(sp);
		    sp++;
		    max = _mm_max_epu8(max, sd);
		}
		if (n1 > 0) {
		    mlib_u8 *sd1 = (mlib_u8 *)sp;
		    mlib_u8 sd2[16] = {
			MLIB_U8_MIN, MLIB_U8_MIN,
			MLIB_U8_MIN, MLIB_U8_MIN,
			MLIB_U8_MIN, MLIB_U8_MIN,
			MLIB_U8_MIN, MLIB_U8_MIN,
			MLIB_U8_MIN, MLIB_U8_MIN,
			MLIB_U8_MIN, MLIB_U8_MIN,
			MLIB_U8_MIN, MLIB_U8_MIN,
			MLIB_U8_MIN, MLIB_U8_MIN};
		    for (int i = 0; i < n1; i++) {
			sd2[i] = *sd1++;
		    }
		    sd = _mm_set_epi8(
				sd2[15], sd2[14], sd2[13], sd2[12],
				sd2[11], sd2[10], sd2[9], sd2[8],
				sd2[7], sd2[6], sd2[5], sd2[4],
				sd2[3], sd2[2], sd2[1], sd2[0]);
		    max = _mm_max_epu8(max, sd);
		}
		sp = sl = (__m128i *) ((mlib_u8 *)sl + src_stride);
	}

	switch (channels) {
	case 1:
	    {
		_8s16_1 = _mm_unpackhi_epi8(max, _m_zero);
		_8s16_2 = _mm_unpacklo_epi8(max, _m_zero);
		_8s16_1 = _mm_max_epi16(_8s16_1, _8s16_2);
		_4s32_1 = _mm_unpackhi_epi16(_8s16_1, _m_zero);
		_4s32_2 = _mm_unpacklo_epi16(_8s16_1, _m_zero);
		MLIB_S_IMAGE_MAXIMUM_S32(_4s32_1, _4s32_1, _4s32_2);
		_8s16_1 = _mm_packs_epi32(_4s32_1, _m_zero);
		_8s16_2 = _mm_srli_si128(_8s16_1, 4);
		_8s16_1 = _mm_max_epi16(_8s16_1, _8s16_2);
		_8s16_2 = _mm_srli_si128(_8s16_1, 2);
		_8s16_1 = _mm_max_epi16(_8s16_1, _8s16_2);
		_4s32_2 = _mm_unpacklo_epi16(_8s16_1, _m_zero);

		res[0] = _mm_cvtsi128_si32(_4s32_2);
		break;
	    }
	case 2:
	    {
		_8s16_1 = _mm_unpackhi_epi8(max, _m_zero);
		_8s16_2 = _mm_unpacklo_epi8(max, _m_zero);
		_8s16_1 = _mm_max_epi16(_8s16_1, _8s16_2);
		_4s32_1 = _mm_unpackhi_epi16(_8s16_1, _m_zero);
		_4s32_2 = _mm_unpacklo_epi16(_8s16_1, _m_zero);
		MLIB_S_IMAGE_MAXIMUM_S32(_4s32_1, _4s32_1, _4s32_2);
		_4s32_2 = _mm_srli_si128(_4s32_1, 8);
		MLIB_S_IMAGE_MAXIMUM_S32(_4s32_1, _4s32_1, _4s32_2);
		res[0] = _mm_cvtsi128_si32(_4s32_1);
		_4s32_1 = _mm_srli_si128(_4s32_1, 4);
		res[1] = _mm_cvtsi128_si32(_4s32_1);
		break;
	    }
	case 4:
	    {
		_8s16_1 = _mm_unpackhi_epi8(max, _m_zero);
		_8s16_2 = _mm_unpacklo_epi8(max, _m_zero);
		_8s16_1 = _mm_max_epi16(_8s16_1, _8s16_2);
		_4s32_1 = _mm_unpackhi_epi16(_8s16_1, _m_zero);
		_4s32_2 = _mm_unpacklo_epi16(_8s16_1, _m_zero);
		MLIB_S_IMAGE_MAXIMUM_S32(_4s32_1, _4s32_1, _4s32_2);
		_mm_storeu_si128(((__m128i *) res), _4s32_1);
		break;
	    }
	}
}

/* *********************************************************** */

void
mlib_s_ImageMaximum_U8_3(
    mlib_s32 *res,
    const mlib_image *img)
{
/* src address */
	__m128i *sp, *sl;
/* src data */
	__m128i sd;
/* max values */
	__m128i max, max1, max2, max3;
/* loop variables */
	mlib_s32 n1;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img);
	mlib_s32 size_row = mlib_ImageGetWidth(img) * 3;

	if (src_stride == size_row) {
		size_row *= height;
		height = 1;
	}

/* pointer to image data */
	sp = sl = (__m128i *)mlib_ImageGetData(img);

/* max values */
	max1 = _mm_set1_epi8(MLIB_U8_MIN);
	max2 = _mm_set1_epi8(MLIB_U8_MIN);
	max3 = _mm_set1_epi8(MLIB_U8_MIN);

	for (; height > 0; height--) {
	    n1 = size_row;
	    for (; n1 > 47; n1 -= 48) {
		sd = _mm_loadu_si128(sp++);
		max1 = _mm_max_epu8(max1, sd);
		sd = _mm_loadu_si128(sp++);
		max2 = _mm_max_epu8(max2, sd);
		sd = _mm_loadu_si128(sp++);
		max3 = _mm_max_epu8(max3, sd);
	    }
	    if (n1 > 0 && n1 < 16) {
		mlib_u8 *sd1 = (mlib_u8 *)sp;
		mlib_u8 sd2[16] = {
		    MLIB_U8_MIN, MLIB_U8_MIN,
		    MLIB_U8_MIN, MLIB_U8_MIN,
		    MLIB_U8_MIN, MLIB_U8_MIN,
		    MLIB_U8_MIN, MLIB_U8_MIN,
		    MLIB_U8_MIN, MLIB_U8_MIN,
		    MLIB_U8_MIN, MLIB_U8_MIN,
		    MLIB_U8_MIN, MLIB_U8_MIN,
		    MLIB_U8_MIN, MLIB_U8_MIN};
		for (int i = 0; i < n1; i++) {
		    sd2[i] = *sd1++;
		}
		sd = _mm_set_epi8(
			sd2[15], sd2[14], sd2[13], sd2[12],
			sd2[11], sd2[10], sd2[9], sd2[8],
			sd2[7], sd2[6], sd2[5], sd2[4],
			sd2[3], sd2[2], sd2[1], sd2[0]);
		max1 = _mm_max_epu8(max1, sd);
	    } else if (n1 > 0) {
		    sd = _mm_loadu_si128(sp++);
		    max1 = _mm_max_epu8(max1, sd);
		    n1 -= 16;

		    if (n1 > 0 && n1 < 16) {
			mlib_u8 *sd1 = (mlib_u8 *)sp;
			mlib_u8 sd2[16] = {
			    MLIB_U8_MIN, MLIB_U8_MIN,
			    MLIB_U8_MIN, MLIB_U8_MIN,
			    MLIB_U8_MIN, MLIB_U8_MIN,
			    MLIB_U8_MIN, MLIB_U8_MIN,
			    MLIB_U8_MIN, MLIB_U8_MIN,
			    MLIB_U8_MIN, MLIB_U8_MIN,
			    MLIB_U8_MIN, MLIB_U8_MIN,
			    MLIB_U8_MIN, MLIB_U8_MIN};
			for (int i = 0; i < n1; i++) {
			    sd2[i] = *sd1++;
			}
			sd = _mm_set_epi8(
				sd2[15], sd2[14], sd2[13], sd2[12],
				sd2[11], sd2[10], sd2[9], sd2[8],
				sd2[7], sd2[6], sd2[5], sd2[4],
				sd2[3], sd2[2], sd2[1], sd2[0]);
			max2 = _mm_max_epu8(max2, sd);
		    } else if (n1 > 0) {
			sd = _mm_loadu_si128(sp++);
			max2 = _mm_max_epu8(max2, sd);
			n1 -= 16;
			if (n1 > 0) {
			    mlib_u8 *sd1 = (mlib_u8 *)sp;
			    mlib_u8 sd2[16] = {
				MLIB_U8_MIN, MLIB_U8_MIN,
				MLIB_U8_MIN, MLIB_U8_MIN,
				MLIB_U8_MIN, MLIB_U8_MIN,
				MLIB_U8_MIN, MLIB_U8_MIN,
				MLIB_U8_MIN, MLIB_U8_MIN,
				MLIB_U8_MIN, MLIB_U8_MIN,
				MLIB_U8_MIN, MLIB_U8_MIN,
				MLIB_U8_MIN, MLIB_U8_MIN};
			    for (int i = 0; i < n1; i++) {
				sd2[i] = *sd1++;
			    }
			    sd = _mm_set_epi8(
					sd2[15], sd2[14], sd2[13], sd2[12],
					sd2[11], sd2[10], sd2[9], sd2[8],
					sd2[7], sd2[6], sd2[5], sd2[4],
					sd2[3], sd2[2], sd2[1], sd2[0]);
			    max3 = _mm_max_epu8(max3, sd);
			}
		    }
	    }
		sp = sl = (__m128i *) ((mlib_u8 *)sl + src_stride);
	}

	max = _mm_max_epu8(max1, _mm_srli_si128(max2, 2));
	max = _mm_max_epu8(max, _mm_slli_si128(max3, 2));
	max = _mm_max_epu8(max, _mm_slli_si128(max2, 1));
	max = _mm_max_epu8(max, _mm_srli_si128(max3, 1));
	max = _mm_max_epu8(max, _mm_srli_si128(max, 6));
	max = _mm_srli_si128(_mm_slli_si128(max, 6), 6);
	max = _mm_max_epu8(max, _mm_srli_si128(max, 3));
	max = _mm_srli_si128(_mm_slli_si128(max, 3), 3);
	max = _mm_max_epu8(max, _mm_srli_si128(max, 3));
	max = _mm_srli_si128(_mm_slli_si128(max, 3), 3);

	int tmpRes;
	tmpRes = _mm_cvtsi128_si32(max);
	res[0] = (tmpRes & 0xff);
	res[1] = ((tmpRes >> 8) & 0xff);
	res[2] = ((tmpRes >> 16) & 0xff);
	tmpRes = ((tmpRes >> 24) & 0xff);
	if (res[0] < tmpRes) res[0] = tmpRes;

}

/* *********************************************************** */

void
mlib_s_ImageMaximum_S16_124(
    mlib_s32 *res,
    const mlib_image *img)
{
/* src address */
	__m128i *sp, *sl;
/* src data */
	__m128i sd;
/* max values */
	__m128i max;

/* intermediate variables */
	__m128i _4s32_1, _4s32_2;

/* loop variables */
	mlib_s32 n1;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img);

/* number of image channels */
	mlib_s32 channels = mlib_ImageGetChannels(img);
	mlib_s32 size_row = mlib_ImageGetWidth(img) * channels;
/* indices */
	mlib_s32 i, j;

	if (src_stride == size_row) {
		size_row *= height;
		height = 1;
	}
	sp = sl = (__m128i *)mlib_ImageGetData(img);
/* max values */
	max = _mm_set1_epi16(MLIB_S16_MIN);

	for (; height > 0; height--) {
		n1 = size_row;
		for (; n1 > 7; n1 -= 8) {
		    sd = _mm_loadu_si128(sp);
		    sp++;
		    max = _mm_max_epi16(max, sd);
		}

		if (n1 > 0) {
		    mlib_s16 *sd1 = (mlib_s16 *)sp;
		    mlib_s16 sd2[8] = {
			MLIB_S16_MIN, MLIB_S16_MIN,
			MLIB_S16_MIN, MLIB_S16_MIN,
			MLIB_S16_MIN, MLIB_S16_MIN,
			MLIB_S16_MIN, MLIB_S16_MIN};
		    for (int i = 0; i < n1; i++) {
			sd2[i] = *sd1++;
		    }
		    sd = _mm_set_epi16(
			sd2[7], sd2[6], sd2[5], sd2[4],
			sd2[3], sd2[2], sd2[1], sd2[0]);
		    max = _mm_max_epi16(max, sd);
		}
		sp = sl = (__m128i *) ((mlib_u8 *)sl + src_stride);
	}

	switch (channels) {
	case 1:
	    {
		__m128i emask = _mm_srai_epi16(max, 15);
		_4s32_1 = _mm_unpackhi_epi16(max, emask);
		_4s32_2 = _mm_unpacklo_epi16(max, emask);
		MLIB_S_IMAGE_MAXIMUM_S32(_4s32_1, _4s32_1, _4s32_2);
		_4s32_2 = _mm_srli_si128(_4s32_1, 8);
		MLIB_S_IMAGE_MAXIMUM_S32(_4s32_1, _4s32_1, _4s32_2);

		res[0] = _mm_cvtsi128_si32(_4s32_1);
		_4s32_1 = _mm_srli_si128(_4s32_1, 4);
		int tmpRes = _mm_cvtsi128_si32(_4s32_1);
		if (res[0] < tmpRes) res[0] = tmpRes;
		break;
	    }
	case 2:
	    {
		__m128i emask = _mm_srai_epi16(max, 15);
		_4s32_1 = _mm_unpackhi_epi16(max, emask);
		_4s32_2 = _mm_unpacklo_epi16(max, emask);
		MLIB_S_IMAGE_MAXIMUM_S32(_4s32_1, _4s32_1, _4s32_2);
		_4s32_2 = _mm_srli_si128(_4s32_1, 8);
		MLIB_S_IMAGE_MAXIMUM_S32(_4s32_1, _4s32_1, _4s32_2);
		res[0] = _mm_cvtsi128_si32(_4s32_1);
		_4s32_1 = _mm_srli_si128(_4s32_1, 4);
		res[1] = _mm_cvtsi128_si32(_4s32_1);
		break;
	    }
	case 4:
	    {
		__m128i emask = _mm_srai_epi16(max, 15);
		_4s32_1 = _mm_unpackhi_epi16(max, emask);
		_4s32_2 = _mm_unpacklo_epi16(max, emask);
		MLIB_S_IMAGE_MAXIMUM_S32(_4s32_1, _4s32_1, _4s32_2);
		_mm_storeu_si128(((__m128i *) res), _4s32_1);
		break;
	    }
	}
}

/* *********************************************************** */

void
mlib_s_ImageMaximum_S16_3(
    mlib_s32 *res,
    const mlib_image *img)
{
/* src address */
	__m128i *sp, *sl;
/* src data */
	__m128i sd;
/* max values */
	__m128i max, max1, max2, max3;

/* intermediate variables */
	__m128i _4s32_1, _4s32_2;

/* loop variables */
	mlib_s32 n1;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img);
	mlib_s32 size_row = mlib_ImageGetWidth(img) * 3;

	if (src_stride == size_row) {
		size_row *= height;
		height = 1;
	}

/* pointer to image data */
	sp = sl = (__m128i *)mlib_ImageGetData(img);

/* max values */
	max1 = _mm_set1_epi16(MLIB_S16_MIN);
	max2 = _mm_set1_epi16(MLIB_S16_MIN);
	max3 = _mm_set1_epi16(MLIB_S16_MIN);

	for (; height > 0; height--) {
	    n1 = size_row;
	    for (; n1 > 23; n1 -= 24) {
		sd = _mm_loadu_si128(sp++);
		max1 = _mm_max_epi16(max1, sd);
		sd = _mm_loadu_si128(sp++);
		max2 = _mm_max_epi16(max2, sd);
		sd = _mm_loadu_si128(sp++);
		max3 = _mm_max_epi16(max3, sd);
	    }
	    if (n1 > 0 && n1 < 8) {
		mlib_s16 *sd1 = (mlib_s16 *)sp;
		mlib_s16 sd2[8] = {
		    MLIB_S16_MIN, MLIB_S16_MIN,
		    MLIB_S16_MIN, MLIB_S16_MIN,
		    MLIB_S16_MIN, MLIB_S16_MIN,
		    MLIB_S16_MIN, MLIB_S16_MIN};
		for (int i = 0; i < n1; i++) {
		    sd2[i] = *sd1++;
		}
		sd = _mm_set_epi16(
			sd2[7], sd2[6], sd2[5], sd2[4],
			sd2[3], sd2[2], sd2[1], sd2[0]);
		max1 = _mm_max_epi16(max1, sd);
	    } else if (n1 > 0) {
		    sd = _mm_loadu_si128(sp++);
		    max1 = _mm_max_epi16(max1, sd);
		    n1 -= 8;

		    if (n1 > 0 && n1 < 8) {
			mlib_s16 *sd1 = (mlib_s16 *)sp;
			mlib_s16 sd2[8] = {
			    MLIB_S16_MIN, MLIB_S16_MIN,
			    MLIB_S16_MIN, MLIB_S16_MIN,
			    MLIB_S16_MIN, MLIB_S16_MIN,
			    MLIB_S16_MIN, MLIB_S16_MIN};
			for (int i = 0; i < n1; i++) {
			    sd2[i] = *sd1++;
			}
			sd = _mm_set_epi16(
				sd2[7], sd2[6], sd2[5], sd2[4],
				sd2[3], sd2[2], sd2[1], sd2[0]);
			max2 = _mm_max_epi16(max2, sd);

		    } else if (n1 > 0) {
			sd = _mm_loadu_si128(sp++);
			max2 = _mm_max_epi16(max2, sd);
			n1 -= 8;

			if (n1 > 0) {
			    mlib_s16 *sd1 = (mlib_s16 *)sp;
			    mlib_s16 sd2[8] = {
				MLIB_S16_MIN, MLIB_S16_MIN,
				MLIB_S16_MIN, MLIB_S16_MIN,
				MLIB_S16_MIN, MLIB_S16_MIN,
				MLIB_S16_MIN, MLIB_S16_MIN};
			    for (int i = 0; i < n1; i++) {
				sd2[i] = *sd1++;
			    }
			    sd = _mm_set_epi16(
					sd2[7], sd2[6], sd2[5], sd2[4],
					sd2[3], sd2[2], sd2[1], sd2[0]);
			    max3 = _mm_max_epi16(max3, sd);
			}
		    }
		}
		sp = sl = (__m128i *) ((mlib_u8 *)sl + src_stride);
	}

	__m128i cmask;
	cmask = _mm_set_epi16(
			0x0, 0x0, 0x0, 0x0, 0x0,
			0x0, MLIB_S16_MIN, MLIB_S16_MIN);
	max = _mm_max_epi16(max1, _mm_or_si128(_mm_slli_si128(max2, 4), cmask));
	cmask = _mm_set_epi16(
			MLIB_S16_MIN, MLIB_S16_MIN,
			0x0, 0x0, 0x0, 0x0, 0x0, 0x0);
	max = _mm_max_epi16(max, _mm_or_si128(_mm_srli_si128(max3, 4), cmask));
	cmask = _mm_set_epi16(
			MLIB_S16_MIN, 0x0,
			0x0, 0x0, 0x0, 0x0, 0x0, 0x0);
	max = _mm_max_epi16(max, _mm_or_si128(_mm_srli_si128(max2, 2), cmask));
	cmask = _mm_set_epi16(
			0x0, 0x0, 0x0, 0x0, 0x0,
			0x0, 0x0, MLIB_S16_MIN);
	max = _mm_max_epi16(max, _mm_or_si128(_mm_slli_si128(max3, 2), cmask));

	max = _mm_max_epi16(max, _mm_srli_si128(max, 6));
	max = _mm_srli_si128(_mm_slli_si128(max, 6), 6);
	__m128i emask = _mm_srai_epi16(max, 15);
	_4s32_1 = _mm_unpackhi_epi16(max, emask);
	_4s32_2 = _mm_unpacklo_epi16(max, emask);

	res[0] = _mm_cvtsi128_si32(_4s32_2);
	res[1] = _mm_cvtsi128_si32(_mm_srli_si128(_4s32_2, 4));
	res[2] = _mm_cvtsi128_si32(_mm_srli_si128(_4s32_2, 8));
	int tmpRes = _mm_cvtsi128_si32(_mm_srli_si128(_4s32_2, 12));
	if (res[0] < tmpRes) res[0] = tmpRes;
	tmpRes = _mm_cvtsi128_si32(_4s32_1);
	if (res[1] < tmpRes) res[1] = tmpRes;
}

/* *********************************************************** */

void
mlib_s_ImageMaximum_U16_124(
    mlib_s32 *res,
    const mlib_image *img)
{
/* src address */
	__m128i *sp, *sl;
/* src data */
	__m128i sd;
/* max values */
	__m128i max;
/* diff values */
	__m128i diff;

/* intermediate variables */
	__m128i _4s32_1, _4s32_2;

/* loop variables */
	mlib_s32 n1;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img);

/* number of image channels */
	mlib_s32 channels = mlib_ImageGetChannels(img);
	mlib_s32 size_row = mlib_ImageGetWidth(img) * channels;

	if (src_stride == size_row) {
		size_row *= height;
		height = 1;
	}
	sp = sl = (__m128i *)mlib_ImageGetData(img);
/* max values */
	max = _mm_set1_epi16(MLIB_S16_MIN);
/* diff value, used to change u16 to s16 by subtraction */
	diff = _mm_set1_epi16(MLIB_S16_MIN);

	for (; height > 0; height--) {
		n1 = size_row;
		for (; n1 > 7; n1 -= 8) {
		    sd = _mm_loadu_si128(sp);
		    sp++;
		    max = _mm_max_epi16(max, _mm_add_epi16(sd, diff));
		}

		if (n1 > 0) {
		    mlib_u16 *sd1 = (mlib_u16 *)sp;
		    mlib_s16 sd2[8] = {
			MLIB_S16_MIN, MLIB_S16_MIN,
			MLIB_S16_MIN, MLIB_S16_MIN,
			MLIB_S16_MIN, MLIB_S16_MIN,
			MLIB_S16_MIN, MLIB_S16_MIN};
		    for (int i = 0; i < n1; i++) {
			/* change u16 to s16 by subtraction */
			sd2[i] = (*sd1++) + MLIB_S16_MIN;
		    }
		    sd = _mm_set_epi16(
				sd2[7], sd2[6], sd2[5], sd2[4],
				sd2[3], sd2[2], sd2[1], sd2[0]);
		    max = _mm_max_epi16(max, sd);
		}
		sp = sl = (__m128i *) ((mlib_u8 *)sl + src_stride);
	}

	switch (channels) {
	case 1:
	    {
		__m128i emask = _mm_srai_epi16(max, 15);
		_4s32_1 = _mm_unpackhi_epi16(max, emask);
		_4s32_2 = _mm_unpacklo_epi16(max, emask);
		MLIB_S_IMAGE_MAXIMUM_S32(_4s32_1, _4s32_1, _4s32_2);
		_4s32_2 = _mm_srli_si128(_4s32_1, 8);
		MLIB_S_IMAGE_MAXIMUM_S32(_4s32_1, _4s32_1, _4s32_2);
		res[0] = _mm_cvtsi128_si32(_4s32_1);
		int tmpRes = _mm_cvtsi128_si32(_mm_srli_si128(_4s32_1, 4));
		if (res[0] < tmpRes) res[0] = tmpRes;
		res[0] = res[0] - MLIB_S16_MIN;
		break;
	    }
	case 2:
	    {
		__m128i emask = _mm_srai_epi16(max, 15);
		_4s32_1 = _mm_unpackhi_epi16(max, emask);
		_4s32_2 = _mm_unpacklo_epi16(max, emask);
		MLIB_S_IMAGE_MAXIMUM_S32(_4s32_1, _4s32_1, _4s32_2);
		_4s32_2 = _mm_srli_si128(_4s32_1, 8);
		MLIB_S_IMAGE_MAXIMUM_S32(_4s32_1, _4s32_1, _4s32_2);
		_4s32_1 = _mm_sub_epi32(_4s32_1, _mm_set1_epi32(MLIB_S16_MIN));
		res[0] = _mm_cvtsi128_si32(_4s32_1);
		_4s32_1 = _mm_srli_si128(_4s32_1, 4);
		res[1] = _mm_cvtsi128_si32(_4s32_1);
		break;
	    }
	case 4:
	    {
		__m128i emask = _mm_srai_epi16(max, 15);
		_4s32_1 = _mm_unpackhi_epi16(max, emask);
		_4s32_2 = _mm_unpacklo_epi16(max, emask);
		MLIB_S_IMAGE_MAXIMUM_S32(_4s32_1, _4s32_1, _4s32_2);
		_4s32_1 = _mm_sub_epi32(_4s32_1, _mm_set1_epi32(MLIB_S16_MIN));
		_mm_storeu_si128(((__m128i *) res), _4s32_1);
		break;
	    }
	}
}

/* *********************************************************** */

void
mlib_s_ImageMaximum_U16_3(
    mlib_s32 *res,
    const mlib_image *img)
{
/* src address */
	__m128i *sp, *sl;
/* src data */
	__m128i sd;
/* max values */
	__m128i max, max1, max2, max3;
/* diff values */
	__m128i diff;

/* intermediate variables */
	__m128i _4s32_1, _4s32_2;

/* loop variables */
	mlib_s32 n1;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img);
	mlib_s32 size_row = mlib_ImageGetWidth(img) * 3;

	if (src_stride == size_row) {
		size_row *= height;
		height = 1;
	}

/* pointer to image data */
	sp = sl = (__m128i *)mlib_ImageGetData(img);

/* max values */
	max1 = _mm_set1_epi16(MLIB_S16_MIN);
	max2 = _mm_set1_epi16(MLIB_S16_MIN);
	max3 = _mm_set1_epi16(MLIB_S16_MIN);
/* diff value, used to change u16 to s16 by subtraction */
	diff = _mm_set1_epi16(MLIB_S16_MIN);

	for (; height > 0; height--) {
	    n1 = size_row;
	    for (; n1 > 23; n1 -= 24) {
		sd = _mm_loadu_si128(sp++);
		max1 = _mm_max_epi16(max1, _mm_add_epi16(sd, diff));
		sd = _mm_loadu_si128(sp++);
		max2 = _mm_max_epi16(max2, _mm_add_epi16(sd, diff));
		sd = _mm_loadu_si128(sp++);
		max3 = _mm_max_epi16(max3, _mm_add_epi16(sd, diff));
	    }
	    if (n1 > 0 && n1 < 8) {
		mlib_u16 *sd1 = (mlib_u16 *)sp;
		mlib_s16 sd2[8] = {
		    MLIB_S16_MIN, MLIB_S16_MIN,
		    MLIB_S16_MIN, MLIB_S16_MIN,
		    MLIB_S16_MIN, MLIB_S16_MIN,
		    MLIB_S16_MIN, MLIB_S16_MIN};
		for (int i = 0; i < n1; i++) {
		    /* change u16 to s16 by subtraction */
		    sd2[i] = (*sd1++) + MLIB_S16_MIN;
		}
		sd = _mm_set_epi16(
			sd2[7], sd2[6], sd2[5], sd2[4],
			sd2[3], sd2[2], sd2[1], sd2[0]);
		max1 = _mm_max_epi16(max1, sd);
	    } else if (n1 > 0) {
		    sd = _mm_loadu_si128(sp++);
		    max1 = _mm_max_epi16(max1, _mm_add_epi16(sd, diff));
		    n1 -= 8;

		    if (n1 > 0 && n1 < 8) {
			mlib_u16 *sd1 = (mlib_u16 *)sp;
			mlib_s16 sd2[8] = {
			    MLIB_S16_MIN, MLIB_S16_MIN,
			    MLIB_S16_MIN, MLIB_S16_MIN,
			    MLIB_S16_MIN, MLIB_S16_MIN,
			    MLIB_S16_MIN, MLIB_S16_MIN};
			for (int i = 0; i < n1; i++) {
			    sd2[i] = (*sd1++) + MLIB_S16_MIN;
			}
			sd = _mm_set_epi16(
				sd2[7], sd2[6], sd2[5], sd2[4],
				sd2[3], sd2[2], sd2[1], sd2[0]);
			max2 = _mm_max_epi16(max2, sd);
		    } else if (n1 > 0) {
			sd = _mm_loadu_si128(sp++);
			max2 = _mm_max_epi16(max2, _mm_add_epi16(sd, diff));
			n1 -= 8;

			if (n1 > 0) {
			    mlib_u16 *sd1 = (mlib_u16 *)sp;
			    mlib_s16 sd2[8] = {
				MLIB_S16_MIN, MLIB_S16_MIN,
				MLIB_S16_MIN, MLIB_S16_MIN,
				MLIB_S16_MIN, MLIB_S16_MIN,
				MLIB_S16_MIN, MLIB_S16_MIN};
			    for (int i = 0; i < n1; i++) {
				sd2[i] = (*sd1++) + MLIB_S16_MIN;
			    }
			    sd = _mm_set_epi16(
					sd2[7], sd2[6], sd2[5], sd2[4],
					sd2[3], sd2[2], sd2[1], sd2[0]);
			    max3 = _mm_max_epi16(max3, sd);
			}
		    }
		}
		sp = sl = (__m128i *) ((mlib_u8 *)sl + src_stride);
	}

	__m128i cmask;
	cmask = _mm_set_epi16(
			0x0, 0x0, 0x0, 0x0, 0x0,
			0x0, MLIB_S16_MIN, MLIB_S16_MIN);
	max = _mm_max_epi16(max1, _mm_or_si128(_mm_slli_si128(max2, 4), cmask));
	cmask = _mm_set_epi16(
			MLIB_S16_MIN, MLIB_S16_MIN,
			0x0, 0x0, 0x0, 0x0, 0x0, 0x0);
	max = _mm_max_epi16(max, _mm_or_si128(_mm_srli_si128(max3, 4), cmask));
	cmask = _mm_set_epi16(MLIB_S16_MIN, 0x0,
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0);
	max = _mm_max_epi16(max, _mm_or_si128(_mm_srli_si128(max2, 2), cmask));
	cmask = _mm_set_epi16(
			0x0, 0x0, 0x0, 0x0, 0x0,
			0x0, 0x0, MLIB_S16_MIN);
	max = _mm_max_epi16(max, _mm_or_si128(_mm_slli_si128(max3, 2), cmask));

	max = _mm_max_epi16(max, _mm_srli_si128(max, 6));
	max = _mm_srli_si128(_mm_slli_si128(max, 6), 6);
	__m128i emask = _mm_srai_epi16(max, 15);
	_4s32_1 = _mm_unpackhi_epi16(max, emask);
	_4s32_1 = _mm_sub_epi32(_4s32_1, _mm_set1_epi32(MLIB_S16_MIN));
	_4s32_2 = _mm_unpacklo_epi16(max, emask);
	_4s32_2 = _mm_sub_epi32(_4s32_2, _mm_set1_epi32(MLIB_S16_MIN));

	res[0] = _mm_cvtsi128_si32(_4s32_2);
	res[1] = _mm_cvtsi128_si32(_mm_srli_si128(_4s32_2, 4));
	res[2] = _mm_cvtsi128_si32(_mm_srli_si128(_4s32_2, 8));
	int tmpRes = _mm_cvtsi128_si32(_mm_srli_si128(_4s32_2, 12));
	if (res[0] < tmpRes) res[0] = tmpRes;
	tmpRes = _mm_cvtsi128_si32(_4s32_1);
	if (res[1] < tmpRes) res[1] = tmpRes;
}

/* *********************************************************** */

void
mlib_s_ImageMaximum_S32_124(
    mlib_s32 *res,
    const mlib_image *img)
{
/* src address */
	__m128i *sp, *sl;
/* src data */
	__m128i sd;
/* max values */
	__m128i max;

/* loop variables */
	mlib_s32 n1;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img);

/* number of image channels */
	mlib_s32 channels = mlib_ImageGetChannels(img);
	mlib_s32 size_row = mlib_ImageGetWidth(img) * channels;

	if (src_stride == size_row) {
		size_row *= height;
		height = 1;
	}
	sp = sl = (__m128i *)mlib_ImageGetData(img);
/* max values */
	max = _mm_set1_epi32(MLIB_S32_MIN);

	for (; height > 0; height--) {
		n1 = size_row;
		for (; n1 > 3; n1 -= 4) {
		    sd = _mm_loadu_si128(sp);
		    sp++;
		    MLIB_S_IMAGE_MAXIMUM_S32(max, max, sd);
		}
		if (n1 > 0) {
		    mlib_s32 *sd1 = (mlib_s32 *)sp;
		    mlib_s32 sd2[4] = {
			MLIB_S32_MIN, MLIB_S32_MIN,
			MLIB_S32_MIN, MLIB_S32_MIN};
		    for (int i = 0; i < n1; i++) {
			sd2[i] = *sd1++;
		    }
		    sd = _mm_set_epi32(sd2[3], sd2[2], sd2[1], sd2[0]);
		    MLIB_S_IMAGE_MAXIMUM_S32(max, max, sd);
		}
		sp = sl = (__m128i *) ((mlib_u8 *)sl + src_stride);
	}

	switch (channels) {
	case 1:
	    {
		int tmpRes[4];
		_mm_storeu_si128(((__m128i *) tmpRes), max);
		res[0] = (tmpRes[0] > tmpRes[1]) ? tmpRes[0] : tmpRes[1];
		if (res[0] < tmpRes[2]) res[0] = tmpRes[2];
		if (res[0] < tmpRes[3]) res[0] = tmpRes[3];
		break;
	    }
	case 2:
	    {
		int tmpRes[4];
		_mm_storeu_si128(((__m128i *) tmpRes), max);
		res[0] = (tmpRes[0] > tmpRes[2]) ? tmpRes[0] : tmpRes[2];
		res[1] = (tmpRes[1] > tmpRes[3]) ? tmpRes[1] : tmpRes[3];
		break;
	    }
	case 4:
	    {
		_mm_storeu_si128(((__m128i *) res), max);
		break;
	    }
	}
}

/* *********************************************************** */

void
mlib_s_ImageMaximum_S32_3(
    mlib_s32 *res,
    const mlib_image *img)
{
/* src address */
	__m128i *sp, *sl;
/* src data */
	__m128i sd;
/* max values */
	__m128i max, max1, max2, max3;
/* loop variables */
	mlib_s32 n1;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img);
	mlib_s32 size_row = mlib_ImageGetWidth(img) * 3;

	if (src_stride == size_row) {
		size_row *= height;
		height = 1;
	}

/* pointer to image data */
	sp = sl = (__m128i *)mlib_ImageGetData(img);

/* max values */
	max1 = _mm_set1_epi32(MLIB_S32_MIN);
	max2 = _mm_set1_epi32(MLIB_S32_MIN);
	max3 = _mm_set1_epi32(MLIB_S32_MIN);

	for (; height > 0; height--) {
	    n1 = size_row;
	    for (; n1 > 11; n1 -= 12) {
		sd = _mm_loadu_si128(sp++);
		MLIB_S_IMAGE_MAXIMUM_S32(max1, max1, sd);
		sd = _mm_loadu_si128(sp++);
		MLIB_S_IMAGE_MAXIMUM_S32(max2, max2, sd);
		sd = _mm_loadu_si128(sp++);
		MLIB_S_IMAGE_MAXIMUM_S32(max3, max3, sd);
	    }
	    if (n1 > 0 && n1 < 4) {
		mlib_s32 *sd1 = (mlib_s32 *)sp;
		mlib_s32 sd2[4] = {
		    MLIB_S32_MIN, MLIB_S32_MIN,
		    MLIB_S32_MIN, MLIB_S32_MIN};
		for (int i = 0; i < n1; i++) {
		    sd2[i] = *sd1++;
		}
		sd = _mm_set_epi32(sd2[3], sd2[2], sd2[1], sd2[0]);
		MLIB_S_IMAGE_MAXIMUM_S32(max1, max1, sd);
	    } else if (n1 > 0) {
		    sd = _mm_loadu_si128(sp++);
		    MLIB_S_IMAGE_MAXIMUM_S32(max1, max1, sd);
		    n1 -= 4;

		    if (n1 > 0 && n1 < 4) {
			mlib_s32 *sd1 = (mlib_s32 *)sp;
			mlib_s32 sd2[4] = {
			    MLIB_S32_MIN, MLIB_S32_MIN,
			    MLIB_S32_MIN, MLIB_S32_MIN};
			for (int i = 0; i < n1; i++) {
			    sd2[i] = *sd1++;
			}
			sd = _mm_set_epi32(sd2[3], sd2[2], sd2[1], sd2[0]);
			MLIB_S_IMAGE_MAXIMUM_S32(max2, max2, sd);
		    } else if (n1 > 0) {
			sd = _mm_loadu_si128(sp++);
			MLIB_S_IMAGE_MAXIMUM_S32(max2, max2, sd);
			n1 -= 4;

			if (n1 > 0) {
			    mlib_s32 *sd1 = (mlib_s32 *)sp;
			    mlib_s32 sd2[4] = {
				MLIB_S32_MIN, MLIB_S32_MIN,
				MLIB_S32_MIN, MLIB_S32_MIN};
			    for (int i = 0; i < n1; i++) {
				sd2[i] = *sd1++;
			    }
			    sd = _mm_set_epi32(sd2[3], sd2[2], sd2[1], sd2[0]);
			    MLIB_S_IMAGE_MAXIMUM_S32(max3, max3, sd);
			}
		    }
		}
		sp = sl = (__m128i *) ((mlib_u8 *)sl + src_stride);
	}

	__m128i cmask, emask;
	cmask = _mm_set_epi32(MLIB_S32_MIN, MLIB_S32_MIN, 0x0, 0x0);
	emask = _mm_or_si128(cmask, _mm_srli_si128(max2, 8));
	MLIB_S_IMAGE_MAXIMUM_S32(max, max1, emask);

	cmask = _mm_set_epi32(0x0, 0x0, MLIB_S32_MIN, MLIB_S32_MIN);
	emask = _mm_or_si128(cmask, _mm_slli_si128(max3, 8));
	MLIB_S_IMAGE_MAXIMUM_S32(max, max, emask);

	cmask = _mm_set_epi32(0x0, 0x0, 0x0, MLIB_S32_MIN);
	emask = _mm_or_si128(cmask, _mm_slli_si128(max2, 4));
	MLIB_S_IMAGE_MAXIMUM_S32(max, max, emask);

	cmask = _mm_set_epi32(MLIB_S32_MIN, 0x0, 0x0, 0x0);
	emask = _mm_or_si128(cmask, _mm_srli_si128(max3, 4));
	MLIB_S_IMAGE_MAXIMUM_S32(max, max, emask);

	int tmpRes = res[3];
	_mm_storeu_si128(((__m128i *) res), max);
	if (res[0] < res[3]) res[0] = res[3];
	/* restore original value of res[3] to pass test suite */
	res[3] = tmpRes;
}

/* *********************************************************** */

mlib_status
__mlib_ImageMaximum(
    mlib_s32 *res,
    const mlib_image *img)
{
	mlib_s32 nchan;

	MLIB_IMAGE_CHECK(img);

	if (res == NULL)
		return (MLIB_NULLPOINTER);

	nchan = mlib_ImageGetChannels(img);

	switch (mlib_ImageGetType(img)) {
	case MLIB_BYTE:

		switch (nchan) {
		case 1:
		    mlib_s_ImageMaximum_U8_124(res, img);
		    return (MLIB_SUCCESS);
		case 2:
		    mlib_s_ImageMaximum_U8_124(res, img);
		    return (MLIB_SUCCESS);
		case 3:
		    mlib_s_ImageMaximum_U8_3(res, img);
		    return (MLIB_SUCCESS);
		case 4:
		    mlib_s_ImageMaximum_U8_124(res, img);
		    return (MLIB_SUCCESS);
		}

		break;

	case MLIB_SHORT:
		switch (nchan) {
		case 1:
		case 2:
		    mlib_s_ImageMaximum_S16_124(res, img);
		    return (MLIB_SUCCESS);
		case 3:
		    mlib_s_ImageMaximum_S16_3(res, img);
		    return (MLIB_SUCCESS);
		case 4:
		    mlib_s_ImageMaximum_S16_124(res, img);
		    return (MLIB_SUCCESS);
		}

		break;

	case MLIB_USHORT:
		switch (nchan) {
		case 1:
		case 2:
		    mlib_s_ImageMaximum_U16_124(res, img);
		    return (MLIB_SUCCESS);
		case 3:
		    mlib_s_ImageMaximum_U16_3(res, img);
		    return (MLIB_SUCCESS);
		case 4:
		    mlib_s_ImageMaximum_U16_124(res, img);
		    return (MLIB_SUCCESS);
		}

		break;

	case MLIB_INT:
		switch (nchan) {
		case 1:
		case 2:
	/* slower than C version mlib_ImageMaximum_S32_124(ires, img) */
		    mlib_s_ImageMaximum_S32_124(res, img);
		    return (MLIB_SUCCESS);
		case 3:
	/* slower than C version mlib_ImageMaximum_S32_3(res, img) */
		    mlib_s_ImageMaximum_S32_3(res, img);
		    return (MLIB_SUCCESS);
		case 4:
	/* slower than C version mlib_ImageMaximum_S32_124(ires, img) */
		    mlib_s_ImageMaximum_S32_124(res, img);
		    return (MLIB_SUCCESS);
		}

		break;

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */
