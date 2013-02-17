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

#pragma ident	"@(#)mlib_c_ImageMinimum.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageMinimum - calculate minimum for input image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMinimum(mlib_s32   *min,
 *                                    mlib_image *img)
 *
 * ARGUMENTS
 *      min     pointer to min vector
 *      img     pointer to an image
 *
 * RESTRICTION
 *      The image can have 1, 2, 3 or 4 channels.
 *      It can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *
 *      min = MIN (img[i][j])
 *            i, j
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageExtrema.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMinimum = __mlib_ImageMinimum

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMinimum) mlib_ImageMinimum
    __attribute__((weak, alias("__mlib_ImageMinimum")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MIN(min, arg)	min += ((arg - min) & ((arg - min) >> 31))

#define	MIN_S32(res, x)	res = (x < res) ? x : res

/* *********************************************************** */

static void mlib_c_ImageMinimum_U8_124(
    mlib_s32 *res,
    const mlib_image *img);

static void mlib_c_ImageMinimum_U8_3(
    mlib_s32 *res,
    const mlib_image *img);

static void mlib_c_ImageMinimum_S16_124(
    mlib_s32 *res,
    const mlib_image *img);

static void mlib_c_ImageMinimum_S16_3(
    mlib_s32 *res,
    const mlib_image *img);

static void mlib_c_ImageMinimum_U16_124(
    mlib_s32 *res,
    const mlib_image *img);

static void mlib_c_ImageMinimum_U16_3(
    mlib_s32 *res,
    const mlib_image *img);

/* *********************************************************** */

void
mlib_c_ImageMinimum_U8_124(
    mlib_s32 *res,
    const mlib_image *img)
{
	mlib_u8 *psrc = (mlib_u8 *)mlib_ImageGetData(img);

/* minimums by channels */
	mlib_s32 min1, min2, min3, min4;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img);
	mlib_s32 size_row =
	    mlib_ImageGetWidth(img) * mlib_ImageGetChannels(img);
/* indices */
	mlib_s32 i, j;

	if (src_stride == size_row) {
		size_row *= height;
		height = 1;
	}

	min1 = min2 = min3 = min4 = MLIB_U8_MAX;

	for (i = 0; i < height; i++) {
		for (j = 0; j <= (size_row - 4); j += 4) {
			MIN(min1, psrc[j]);
			MIN(min2, psrc[j + 1]);
			MIN(min3, psrc[j + 2]);
			MIN(min4, psrc[j + 3]);
		}

		if (j < size_row)
			MIN(min1, psrc[j]);
		j++;

		if (j < size_row)
			MIN(min2, psrc[j]);
		j++;

		if (j < size_row)
			MIN(min3, psrc[j]);
		psrc += src_stride;
	}

	res[0] = min1;
	res[1] = min2;
	res[2] = min3;
	res[3] = min4;
}

/* *********************************************************** */

void
mlib_c_ImageMinimum_U8_3(
    mlib_s32 *res,
    const mlib_image *img)
{
	mlib_u8 *psrc = (mlib_u8 *)mlib_ImageGetData(img);

/* minimums by channels */
	mlib_s32 min1, min2, min3;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) * 3;

/* indices */
	mlib_s32 i, j;

	if (src_stride == size_row) {
		size_row *= height;
		height = 1;
	}

	min1 = min2 = min3 = MLIB_U8_MAX;
	for (i = 0; i < height; i++) {
		for (j = 0; j <= (size_row - 3); j += 3) {
			MIN(min1, psrc[j]);
			MIN(min2, psrc[j + 1]);
			MIN(min3, psrc[j + 2]);
		}

		psrc += src_stride;
	}

	res[0] = min1;
	res[1] = min2;
	res[2] = min3;
}

/* *********************************************************** */

void
mlib_c_ImageMinimum_S16_124(
    mlib_s32 *res,
    const mlib_image *img)
{
	mlib_s16 *psrc = (mlib_s16 *)mlib_ImageGetData(img);

/* minimums by channels */
	mlib_s32 min1, min2, min3, min4;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) / 2;
	mlib_s32 size_row =
	    mlib_ImageGetWidth(img) * mlib_ImageGetChannels(img);
/* indices */
	mlib_s32 i, j;

	if (src_stride == size_row) {
		size_row *= height;
		height = 1;
	}

	min1 = min2 = min3 = min4 = MLIB_S16_MAX;

	for (i = 0; i < height; i++) {
		for (j = 0; j <= (size_row - 4); j += 4) {
			MIN(min1, psrc[j]);
			MIN(min2, psrc[j + 1]);
			MIN(min3, psrc[j + 2]);
			MIN(min4, psrc[j + 3]);
		}

		if (j < size_row)
			MIN(min1, psrc[j]);
		j++;

		if (j < size_row)
			MIN(min2, psrc[j]);
		j++;

		if (j < size_row)
			MIN(min3, psrc[j]);
		psrc += src_stride;
	}

	res[0] = min1;
	res[1] = min2;
	res[2] = min3;
	res[3] = min4;
}

/* *********************************************************** */

void
mlib_c_ImageMinimum_S16_3(
    mlib_s32 *res,
    const mlib_image *img)
{
	mlib_s16 *psrc = (mlib_s16 *)mlib_ImageGetData(img);

/* minimums by channels */
	mlib_s32 min1, min2, min3;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) / 2;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) * 3;

/* indices */
	mlib_s32 i, j;

	if (src_stride == size_row) {
		size_row *= height;
		height = 1;
	}

	min1 = min2 = min3 = MLIB_S16_MAX;

	for (i = 0; i < height; i++) {
		for (j = 0; j <= (size_row - 3); j += 3) {
			MIN(min1, psrc[j]);
			MIN(min2, psrc[j + 1]);
			MIN(min3, psrc[j + 2]);
		}

		psrc += src_stride;
	}

	res[0] = min1;
	res[1] = min2;
	res[2] = min3;
}

/* *********************************************************** */

void
mlib_c_ImageMinimum_U16_124(
    mlib_s32 *res,
    const mlib_image *img)
{
	mlib_u16 *psrc = (mlib_u16 *)mlib_ImageGetData(img);

/* minimums by channels */
	mlib_s32 min1, min2, min3, min4;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) / 2;
	mlib_s32 size_row =
	    mlib_ImageGetWidth(img) * mlib_ImageGetChannels(img);
/* indices */
	mlib_s32 i, j;

	if (src_stride == size_row) {
		size_row *= height;
		height = 1;
	}

	min1 = min2 = min3 = min4 = MLIB_U16_MAX;

	for (i = 0; i < height; i++) {
		for (j = 0; j <= (size_row - 4); j += 4) {
			MIN(min1, psrc[j]);
			MIN(min2, psrc[j + 1]);
			MIN(min3, psrc[j + 2]);
			MIN(min4, psrc[j + 3]);
		}

		if (j < size_row)
			MIN(min1, psrc[j]);
		j++;

		if (j < size_row)
			MIN(min2, psrc[j]);
		j++;

		if (j < size_row)
			MIN(min3, psrc[j]);
		psrc += src_stride;
	}

	res[0] = min1;
	res[1] = min2;
	res[2] = min3;
	res[3] = min4;
}

/* *********************************************************** */

void
mlib_c_ImageMinimum_U16_3(
    mlib_s32 *res,
    const mlib_image *img)
{
	mlib_u16 *psrc = (mlib_u16 *)mlib_ImageGetData(img);

/* minimums by channels */
	mlib_s32 min1, min2, min3;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) / 2;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) * 3;

/* indices */
	mlib_s32 i, j;

	if (src_stride == size_row) {
		size_row *= height;
		height = 1;
	}

	min1 = min2 = min3 = MLIB_U16_MAX;

	for (i = 0; i < height; i++) {
		for (j = 0; j <= (size_row - 3); j += 3) {
			MIN(min1, psrc[j]);
			MIN(min2, psrc[j + 1]);
			MIN(min3, psrc[j + 2]);
		}

		psrc += src_stride;
	}

	res[0] = min1;
	res[1] = min2;
	res[2] = min3;
}

/* *********************************************************** */

mlib_status
__mlib_ImageMinimum(
    mlib_s32 *res,
    const mlib_image *img)
{
	mlib_s32 nchan, ires[4];

	MLIB_IMAGE_CHECK(img);

	if (res == NULL)
		return (MLIB_NULLPOINTER);

	nchan = mlib_ImageGetChannels(img);

	switch (mlib_ImageGetType(img)) {
	case MLIB_BYTE:
		switch (nchan) {
		case 1:
		case 2:
			mlib_c_ImageMinimum_U8_124(ires, img);
			break;
		case 3:
			mlib_c_ImageMinimum_U8_3(res, img);
			return (MLIB_SUCCESS);
		case 4:
			mlib_c_ImageMinimum_U8_124(res, img);
			return (MLIB_SUCCESS);
		}

		break;

	case MLIB_SHORT:
		switch (nchan) {
		case 1:
		case 2:
			mlib_c_ImageMinimum_S16_124(ires, img);
			break;
		case 3:
			mlib_c_ImageMinimum_S16_3(res, img);
			return (MLIB_SUCCESS);
		case 4:
			mlib_c_ImageMinimum_S16_124(res, img);
			return (MLIB_SUCCESS);
		}

		break;

	case MLIB_USHORT:
		switch (nchan) {
		case 1:
		case 2:
			mlib_c_ImageMinimum_U16_124(ires, img);
			break;
		case 3:
			mlib_c_ImageMinimum_U16_3(res, img);
			return (MLIB_SUCCESS);
		case 4:
			mlib_c_ImageMinimum_U16_124(res, img);
			return (MLIB_SUCCESS);
		}

		break;

	case MLIB_INT:
		switch (nchan) {
		case 1:
		case 2:
			mlib_ImageMinimum_S32_124(ires, img);
			break;
		case 3:
			mlib_ImageMinimum_S32_3(res, img);
			return (MLIB_SUCCESS);
		case 4:
			mlib_ImageMinimum_S32_124(res, img);
			return (MLIB_SUCCESS);
		}

		break;

	default:
		return (MLIB_FAILURE);
	}

	switch (nchan) {
	case 1:
		MIN_S32(ires[0], ires[2]);
		MIN_S32(ires[1], ires[3]);
		MIN_S32(ires[0], ires[1]);
		res[0] = ires[0];
		break;

	case 2:
		MIN_S32(ires[0], ires[2]);
		MIN_S32(ires[1], ires[3]);
		res[0] = ires[0];
		res[1] = ires[1];
		break;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
