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

#pragma ident	"@(#)mlib_ImageMaximum_Fp.c	9.3	07/10/09 SMI"

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
			mlib_ImageMaximum_F32_124(img, drez);
		break;

	case MLIB_DOUBLE:

		if (mlib_ImageGetChannels(img) == 3)
			mlib_ImageMaximum_D64_3(img, max);
		else
			mlib_ImageMaximum_D64_124(img, drez);
		break;

	default:
		return (MLIB_FAILURE);
	}

	switch (mlib_ImageGetChannels(img)) {
	case 1:
		drez[0] = drez[0] > drez[2] ? drez[0] : drez[2];
		drez[1] = drez[1] > drez[3] ? drez[1] : drez[3];
		max[0] = drez[0] > drez[1] ? drez[0] : drez[1];
		break;

	case 2:
		max[0] = drez[0] > drez[2] ? drez[0] : drez[2];
		max[1] = drez[1] > drez[3] ? drez[1] : drez[3];
		break;

	case 4:
		max[0] = drez[0];
		max[1] = drez[1];
		max[2] = drez[2];
		max[3] = drez[3];
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MAX(max, arg)	max = (max > arg) ? max : arg

/* *********************************************************** */

void
mlib_ImageMaximum_F32_124(
    const mlib_image *img,
    mlib_d64 *rez)
{
	mlib_f32 *psrc = (mlib_f32 *)mlib_ImageGetData(img);

/* maximums by channels */
	mlib_f32 max1, max2, max3, max4;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) / 4;
	mlib_s32 size_row =
	    mlib_ImageGetWidth(img) * mlib_ImageGetChannels(img);
/* indices */
	mlib_s32 i, j, k;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	rez[0] = rez[1] = rez[2] = MLIB_F32_MIN;
	max1 = max2 = max3 = max4 = MLIB_F32_MIN;
	for (i = 0; i < height; i++) {
		for (j = 0; j <= (size_row - 4); j += 4) {
			MAX(max1, psrc[j]);
			MAX(max2, psrc[j + 1]);
			MAX(max3, psrc[j + 2]);
			MAX(max4, psrc[j + 3]);
		}

		for (k = 0; j < size_row; j++, k++)
			MAX(rez[k], psrc[j]);
		psrc += src_stride;
	}

	MAX(rez[0], max1);
	MAX(rez[1], max2);
	MAX(rez[2], max3);
	rez[3] = max4;
}

/* *********************************************************** */

void
mlib_ImageMaximum_F32_3(
    const mlib_image *img,
    mlib_d64 *max)
{
	mlib_f32 *psrc = (mlib_f32 *)mlib_ImageGetData(img);

/* maximums by channels */
	mlib_f32 max1, max2, max3;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) / 4;
	mlib_s32 size_row = mlib_ImageGetWidth(img) * 3;

/* indices */
	mlib_s32 i, j;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	max1 = max2 = max3 = MLIB_F32_MIN;
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

void
mlib_ImageMaximum_D64_124(
    const mlib_image *img,
    mlib_d64 *rez)
{
	mlib_d64 *psrc = (mlib_d64 *)mlib_ImageGetData(img);

/* maximums by channels */
	mlib_d64 max1, max2, max3, max4;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) / 8;
	mlib_s32 size_row =
	    mlib_ImageGetWidth(img) * mlib_ImageGetChannels(img);
/* indices */
	mlib_s32 i, j, k;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	rez[0] = rez[1] = rez[2] = MLIB_D64_MIN;
	max1 = max2 = max3 = max4 = MLIB_D64_MIN;
	for (i = 0; i < height; i++) {
		for (j = 0; j <= (size_row - 4); j += 4) {
			MAX(max1, psrc[j]);
			MAX(max2, psrc[j + 1]);
			MAX(max3, psrc[j + 2]);
			MAX(max4, psrc[j + 3]);
		}

		for (k = 0; j < size_row; j++, k++)
			MAX(rez[k], psrc[j]);
		psrc += src_stride;
	}

	MAX(rez[0], max1);
	MAX(rez[1], max2);
	MAX(rez[2], max3);
	rez[3] = max4;
}

/* *********************************************************** */

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
