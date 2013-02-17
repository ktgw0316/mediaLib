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

#pragma ident	"@(#)mlib_ImageMoment2_Fp.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageMoment2_Fp - calculates moment of power two
 *                             for the input image.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMoment2_Fp(mlib_d64         *moment,
 *                                       const mlib_image *img)
 *
 * ARGUMENTS
 *      moment   pointer to moment array
 *      img      pointer to an image
 *
 * DESCRIPTION
 *
 *           1    w-1 h-1         2
 *      c = --- * SUM SUM(x[i][j])
 *          w*h   i = 0 j = 0
 *
 * RESTRICTION
 *      img can have 1, 2, 3 or 4 channels. It can be of MLIB_FLOAT or
 *      MLIB_DOUBLE data type.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageMoment2.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMoment2_Fp = __mlib_ImageMoment2_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMoment2_Fp) mlib_ImageMoment2_Fp
    __attribute__((weak, alias("__mlib_ImageMoment2_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageMoment2_Fp(
    mlib_d64 *moment,
    const mlib_image *img)
{
	mlib_d64 res[4];
	mlib_d64 divider;

/* check for obvious errors */
	MLIB_IMAGE_CHECK(img);

	if (moment == NULL)
		return (MLIB_NULLPOINTER);

	divider = 1.0 / (mlib_ImageGetWidth(img) * mlib_ImageGetHeight(img));

	switch (mlib_ImageGetType(img)) {
	case MLIB_FLOAT:
		mlib_ImageMoment2_F32(img, res);
		break;

	case MLIB_DOUBLE:
		mlib_ImageMoment2_D64(img, res);
		break;

	default:
		return (MLIB_FAILURE);
	}

	switch (mlib_ImageGetChannels(img)) {
	case 1:
		moment[0] = (res[0] + res[1] + res[2] + res[3]) * divider;
		break;

	case 2:
		moment[0] = (res[0] + res[2]) * divider;
		moment[1] = (res[1] + res[3]) * divider;
		break;

	case 4:
		moment[3] = res[3] * divider;
	case 3:
		moment[0] = res[0] * divider;
		moment[1] = res[1] * divider;
		moment[2] = res[2] * divider;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_F32(x)	((x) * (mlib_d64) (x))
#define	MUL_D64(x)	((x) * (x))

/* *********************************************************** */

void
mlib_ImageMoment2_F32(
    const mlib_image *img,
    mlib_d64 *res)
{
	mlib_f32 *psrc = (mlib_f32 *)mlib_ImageGetData(img);

/* accumulators by channels */
	mlib_d64 c1, c2, c3, c4;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 2;
	mlib_s32 src_chan = mlib_ImageGetChannels(img);
	mlib_s32 size_row = mlib_ImageGetWidth(img) * src_chan;

/* indices */
	mlib_s32 i, j, k;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	if (src_chan != 3) {
		res[0] = res[1] = res[2] = 0;
		c1 = c2 = c3 = c4 = 0;
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= (size_row - 4); j += 4) {
				c1 += MUL_F32(psrc[j]);
				c2 += MUL_F32(psrc[j + 1]);
				c3 += MUL_F32(psrc[j + 2]);
				c4 += MUL_F32(psrc[j + 3]);
			}

			for (k = 0; j < size_row; j++, k++)
				res[k] += MUL_F32(psrc[j]);
			psrc += src_stride;
		}

		res[0] += c1;
		res[1] += c2;
		res[2] += c3;
		res[3] = c4;
	} else {
		c1 = c2 = c3 = 0;
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= (size_row - 3); j += 3) {
				c1 += MUL_F32(psrc[j]);
				c2 += MUL_F32(psrc[j + 1]);
				c3 += MUL_F32(psrc[j + 2]);
			}

			psrc += src_stride;
		}

		res[0] = c1;
		res[1] = c2;
		res[2] = c3;
	}
}

/* *********************************************************** */

void
mlib_ImageMoment2_D64(
    const mlib_image *img,
    mlib_d64 *res)
{
	mlib_d64 *psrc = (mlib_d64 *)mlib_ImageGetData(img);

/* accumulators by channels */
	mlib_d64 c1, c2, c3, c4;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 3;
	mlib_s32 src_chan = mlib_ImageGetChannels(img);
	mlib_s32 size_row = mlib_ImageGetWidth(img) * src_chan;

/* indices */
	mlib_s32 i, j, k;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	if (src_chan != 3) {
		res[0] = res[1] = res[2] = 0;
		c1 = c2 = c3 = c4 = 0;
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= (size_row - 4); j += 4) {
				c1 += MUL_D64(psrc[j]);
				c2 += MUL_D64(psrc[j + 1]);
				c3 += MUL_D64(psrc[j + 2]);
				c4 += MUL_D64(psrc[j + 3]);
			}

			for (k = 0; j < size_row; j++, k++)
				res[k] += MUL_D64(psrc[j]);
			psrc += src_stride;
		}

		res[0] += c1;
		res[1] += c2;
		res[2] += c3;
		res[3] = c4;
	} else {
		c1 = c2 = c3 = 0;
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= (size_row - 3); j += 3) {
				c1 += MUL_D64(psrc[j]);
				c2 += MUL_D64(psrc[j + 1]);
				c3 += MUL_D64(psrc[j + 2]);
			}

			psrc += src_stride;
		}

		res[0] = c1;
		res[1] = c2;
		res[2] = c3;
	}
}

/* *********************************************************** */
