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

#pragma ident	"@(#)mlib_s_ImageMaximum_Fp.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageMaximum_Fp - calculate maximum for input image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMaximum_Fp(mlib_d64         *max,
 *                                       const mlib_image *img)
 *
 * ARGUMENTS
 *      max     pointer to max vector
 *      img     pointer to an image
 *
 * DESCRIPTION
 *
 *      max = MAX (img[i][j])
 *            i, j
 *
 * RESTRICTION
 *      img can have 1, 2, 3 or 4 channels. It can be of MLIB_FLOAT or
 *      MLIB_DOUBLE data type.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_sse_utils.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMaximum_Fp = __mlib_ImageMaximum_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMaximum_Fp) mlib_ImageMaximum_Fp
    __attribute__((weak, alias("__mlib_ImageMaximum_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_ImageMaximum_F32_124(
    const mlib_image *img,
    mlib_d64 *rez);

static void mlib_ImageMaximum_F32_3(
    const mlib_image *img,
    mlib_d64 *max);

static void mlib_ImageMaximum_D64_124(
    const mlib_image *img,
    mlib_d64 *rez);

static void mlib_ImageMaximum_D64_3(
    const mlib_image *img,
    mlib_d64 *max);

/* *********************************************************** */

mlib_status
__mlib_ImageMaximum_Fp(
    mlib_d64 *max,
    const mlib_image *img)
{
	mlib_d64 drez[4];

/* check for obvious errors */
	MLIB_IMAGE_CHECK(img);

	if (max == NULL)
		return (MLIB_NULLPOINTER);

	switch (mlib_ImageGetType(img)) {
	case MLIB_FLOAT:

		if (mlib_ImageGetChannels(img) == 3)
			mlib_ImageMaximum_F32_3(img, max);
		else
			mlib_ImageMaximum_F32_124(img, max);
		break;

	case MLIB_DOUBLE:

		if (mlib_ImageGetChannels(img) == 3)
			mlib_ImageMaximum_D64_3(img, max);
		else
			mlib_ImageMaximum_D64_124(img, max);
		break;

	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MAX(max, arg)	max = (max > arg) ? max : arg

#define	MLIB_S_IMAGE_MAXIMUM_F32(dst, src1, src2)              \
	dst = _mm_max_ps(src1, src2)

#define	MLIB_S_IMAGE_MAXIMUM_D64(dst, src1, src2)              \
	dst = _mm_max_pd(src1, src2)

/* *********************************************************** */

void
mlib_ImageMaximum_F32_124(
    const mlib_image *img,
    mlib_d64 *max)
{
/* src address */
	mlib_f32 *sp, *sl;
/* src data */
	__m128 sd;
/* max values */
	__m128 max0;

/* loop variables */
	mlib_s32 n1, i;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img);

/* number of image channels */
	mlib_s32 channels = mlib_ImageGetChannels(img);
	mlib_s32 size_row = mlib_ImageGetWidth(img) * channels;

	sp = sl = mlib_ImageGetData(img);

	if (src_stride == size_row) {
		size_row *= height;
		height = 1;
	}

/* max values */
	max0 = _mm_set_ps1(MLIB_F32_MIN);
	mlib_f32 rez[4] = {
		MLIB_F32_MIN, MLIB_F32_MIN,
		MLIB_F32_MIN, MLIB_F32_MIN};

	for (; height > 0; height--) {
		n1 = size_row;
		for (; n1 > 3; n1 -= 4) {
		    sd = _mm_loadu_ps(sp);
		    sp += 4;
		    MLIB_S_IMAGE_MAXIMUM_F32(max0, max0, sd);
		}

		if (n1 > 0) {
		    mlib_f32 *sd1 = sp;
		    mlib_f32 sd2[4] = {
			MLIB_F32_MIN, MLIB_F32_MIN,
			MLIB_F32_MIN, MLIB_F32_MIN};
		    for (int i = 0; i < n1; i++) {
				sd2[i] = *sd1++;
		    }
		    sd = _mm_set_ps(sd2[3], sd2[2], sd2[1], sd2[0]);

		    MLIB_S_IMAGE_MAXIMUM_F32(max0, max0, sd);
		}
		sp = sl = (mlib_f32 *) ((mlib_u8 *)sl + src_stride);
	}

	switch (channels) {
	case 1: {
		mlib_f32 tmpRes[4];
		_mm_storeu_ps(tmpRes, max0);
		max[0] = (tmpRes[0] > tmpRes[1]) ? tmpRes[0] : tmpRes[1];
		if (max[0] < tmpRes[2]) max[0] = tmpRes[2];
		if (max[0] < tmpRes[3]) max[0] = tmpRes[3];
		break;
	}
	case 2: {
		mlib_f32 tmpRes[4];
		_mm_storeu_ps(tmpRes, max0);
		max[0] = (tmpRes[0] > tmpRes[2]) ? tmpRes[0] : tmpRes[2];
		max[1] = (tmpRes[1] > tmpRes[3]) ? tmpRes[1] : tmpRes[3];
		break;
	}
	case 4: {
		mlib_f32 tmpRes[4];
		_mm_storeu_ps(tmpRes, max0);
		max[0] = tmpRes[0];
		max[1] = tmpRes[1];
		max[2] = tmpRes[2];
		max[3] = tmpRes[3];
		break;
	}
	}
}

/* *********************************************************** */

void
mlib_ImageMaximum_F32_3(
    const mlib_image *img,
    mlib_d64 *max)
{
/* src address */
	mlib_f32 *sp, *sl;
/* src data */
	__m128 sd;
/* max values */
	__m128 max0, max1, max2, max3;
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
	sp = sl = mlib_ImageGetData(img);

/* max values */
	max1 = _mm_set_ps1(MLIB_F32_MIN);
	max2 = _mm_set_ps1(MLIB_F32_MIN);
	max3 = _mm_set_ps1(MLIB_F32_MIN);

	for (; height > 0; height--) {
	    n1 = size_row;
	    for (; n1 > 11; n1 -= 12) {
			sd = _mm_loadu_ps(sp);
			sp += 4;
			MLIB_S_IMAGE_MAXIMUM_F32(max1, max1, sd);
			sd = _mm_loadu_ps(sp);
			sp += 4;
			MLIB_S_IMAGE_MAXIMUM_F32(max2, max2, sd);
			sd = _mm_loadu_ps(sp);
			sp += 4;
			MLIB_S_IMAGE_MAXIMUM_F32(max3, max3, sd);
	    }

	    if (n1 > 0 && n1 < 4) {
		mlib_f32 *sd1 = sp;
		mlib_f32 sd2[4] = {
		    MLIB_F32_MIN, MLIB_F32_MIN,
		    MLIB_F32_MIN, MLIB_F32_MIN};
		for (int i = 0; i < n1; i++) {
		    sd2[i] = *sd1++;
		}
		sd = _mm_set_ps(sd2[3], sd2[2], sd2[1], sd2[0]);
		MLIB_S_IMAGE_MAXIMUM_F32(max1, max1, sd);
	    } else if (n1 > 0) {
		    sd = _mm_loadu_ps(sp);
			sp += 4;
		    MLIB_S_IMAGE_MAXIMUM_F32(max1, max1, sd);
		    n1 -= 4;

		    if (n1 > 0 && n1 < 4) {
			mlib_f32 *sd1 = sp;
			mlib_f32 sd2[4] = {
			    MLIB_F32_MIN, MLIB_F32_MIN,
			    MLIB_F32_MIN, MLIB_F32_MIN};
			for (int i = 0; i < n1; i++) {
			    sd2[i] = *sd1++;
			}
			sd = _mm_set_ps(sd2[3], sd2[2], sd2[1], sd2[0]);
			MLIB_S_IMAGE_MAXIMUM_F32(max2, max2, sd);

			} else if (n1 > 0) {
			sd = _mm_loadu_ps(sp);
			sp += 4;
			MLIB_S_IMAGE_MAXIMUM_F32(max2, max2, sd);
			n1 -= 4;

			if (n1 > 0) {
			    mlib_f32 *sd1 = (mlib_f32 *)sp;
			    mlib_f32 sd2[4] = {
				MLIB_F32_MIN, MLIB_F32_MIN,
				MLIB_F32_MIN, MLIB_F32_MIN};
			    for (int i = 0; i < n1; i++) {
					sd2[i] = *sd1++;
			    }
			    sd = _mm_set_ps(sd2[3], sd2[2], sd2[1], sd2[0]);
			    MLIB_S_IMAGE_MAXIMUM_F32(max3, max3, sd);
			}
		    }
		}
		sp = sl = (mlib_f32 *) ((mlib_u8 *)sl + src_stride);
	}

	const __m128 cmask = _mm_set_ps1(MLIB_F32_MIN);
	__m128 max4 = _mm_shuffle_ps(max2, max3, 0x4E);
	MLIB_S_IMAGE_MAXIMUM_F32(max0, max1, max4);
	__m128 max5 = _mm_unpacklo_ps(max2, cmask);
	__m128 max6 = _mm_unpackhi_ps(max3, cmask);
	MLIB_S_IMAGE_MAXIMUM_F32(max4, max5, max6);
	max4 = _mm_shuffle_ps(max4, max4, 0xE1);
	MLIB_S_IMAGE_MAXIMUM_F32(max1, max0, max4);

	mlib_f32 tmpRes[4];
	_mm_storeu_ps(tmpRes, max1);
	max[0] = tmpRes[0] > tmpRes[3] ? tmpRes[0] : tmpRes[3];
	max[1] = tmpRes[1];
	max[2] = tmpRes[2];
}

/* *********************************************************** */

void
mlib_ImageMaximum_D64_124(
    const mlib_image *img,
    mlib_d64 *max)
{
/* src address */
	mlib_d64 *sp, *sl;
/* src data */
	__m128d sd0, sd1;
/* max values */
	__m128d max0, max1;

/* loop variables */
	mlib_s32 n1, i;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img);

/* number of image channels */
	mlib_s32 channels = mlib_ImageGetChannels(img);
	mlib_s32 size_row = mlib_ImageGetWidth(img) * channels;

	sp = sl = mlib_ImageGetData(img);

	if (src_stride == size_row) {
		size_row *= height;
		height = 1;
	}

/* max values */
	max0 = _mm_set1_pd(MLIB_D64_MIN);
	max1 = _mm_set1_pd(MLIB_D64_MIN);

	mlib_d64 rez[4] = {
		MLIB_D64_MIN, MLIB_D64_MIN,
		MLIB_D64_MIN, MLIB_D64_MIN};

	for (; height > 0; height--) {
		n1 = size_row;
		for (; n1 > 3; n1 -= 4) {
		    sd0 = _mm_loadu_pd(sp);
		    sp += 2;
		    sd1 = _mm_loadu_pd(sp);
		    sp += 2;
		    MLIB_S_IMAGE_MAXIMUM_D64(max0, max0, sd0);
		    MLIB_S_IMAGE_MAXIMUM_D64(max1, max1, sd1);
		}

		if (n1 > 0) {
		    mlib_d64 sd2[4] = {
			MLIB_D64_MIN, MLIB_D64_MIN,
			MLIB_D64_MIN, MLIB_D64_MIN};
		    for (int i = 0; i < n1; i++) {
				sd2[i] = *sp++;
		    }
		    sd0 = _mm_set_pd(sd2[3], sd2[2]);
		    sd1 = _mm_set_pd(sd2[1], sd2[0]);

		    MLIB_S_IMAGE_MAXIMUM_D64(max0, max0, sd0);
		    MLIB_S_IMAGE_MAXIMUM_D64(max1, max1, sd1);
		}
		sp = sl = (mlib_d64 *) ((mlib_u8 *)sl + src_stride);
	}

	switch (channels) {
	case 1: {
		mlib_d64 tmpRes0[2];
		mlib_d64 tmpRes1[2];
		_mm_storeu_pd(tmpRes0, max0);
		_mm_storeu_pd(tmpRes1, max1);
		tmpRes0[0] =
		    (tmpRes0[0] > tmpRes0[1]) ? tmpRes0[0] : tmpRes0[1];
		tmpRes1[0] =
		    (tmpRes1[0] > tmpRes1[1]) ? tmpRes1[0] : tmpRes1[1];
		max[0] = (tmpRes0[0] > tmpRes1[0]) ? tmpRes0[0] : tmpRes1[0];
		break;
	}
	case 2: {
		mlib_d64 tmpRes0[2];
		mlib_d64 tmpRes1[2];
		_mm_storeu_pd(tmpRes0, max0);
		_mm_storeu_pd(tmpRes1, max1);
		max[0] = (tmpRes0[0] > tmpRes1[0]) ? tmpRes0[0] : tmpRes1[0];
		max[1] = (tmpRes0[1] > tmpRes1[1]) ? tmpRes0[1] : tmpRes1[1];
		break;
	}
	case 4: {
		_mm_storeu_pd(max, max0);
		_mm_storeu_pd((max + 2), max1);
		break;
	}
	}
}

/* *********************************************************** */

/* Pure C implementation */

void
mlib_ImageMaximum_D64_3(
    const mlib_image *img,
    mlib_d64 *max)
{
	mlib_d64 *psrc = (mlib_d64 *)mlib_ImageGetData(img);

/* maximums by channels */
	mlib_d64 max1, max2, max3;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) / 8;
	mlib_s32 size_row = mlib_ImageGetWidth(img) * 3;

/* indices */
	mlib_s32 i, j;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	max1 = max2 = max3 = MLIB_D64_MIN;
	for (i = 0; i < height; i++) {
		for (j = 0; j <= (size_row - 3); j += 3) {
			MAX(max1, psrc[j]);
			MAX(max2, psrc[j + 1]);
			MAX(max3, psrc[j + 2]);
		}

		psrc += src_stride;
	}

	max[0] = max1;
	max[1] = max2;
	max[2] = max3;
}

/* *********************************************************** */
