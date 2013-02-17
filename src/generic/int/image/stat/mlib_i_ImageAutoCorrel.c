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

#pragma ident	"@(#)mlib_i_ImageAutoCorrel.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageAutoCorrel - calculates auto-correlation
 *                              for the input image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageAutoCorrel(mlib_d64         *correl,
 *                                       const mlib_image *img,
 *                                       mlib_s32         dx,
 *                                       mlib_s32         dy);
 *
 * ARGUMENTS
 *      correl   pointer to auto-correlation array
 *      img      pointer to an image
 *      dx       displacement in X direction
 *      dy       displacement in Y direction
 *
 * DESCRIPTION
 *
 *                1        w-dx-1 h-dy-1
 *      c = ------------- * SUM    SUM  (x[i][j] * x[i+dx][j+dy])
 *          (w-dx)*(h-dy)   i = 0    j = 0
 *
 * RESTRICTION
 *      The image can have 1, 2, 3 or 4 channels.
 *      It can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageCreate.h>
#include <mlib_i_ImageCorrel.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageAutoCorrel = __mlib_ImageAutoCorrel

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageAutoCorrel) mlib_ImageAutoCorrel
    __attribute__((weak, alias("__mlib_ImageAutoCorrel")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageAutoCorrel(
    mlib_d64 *correl,
    const mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy)
{
	mlib_image images[2], *img1, *img2;

/* height of image */
	mlib_s32 height;

/* width of image */
	mlib_s32 width;

/* type of image */
	mlib_type type;

/* channels of image */
	mlib_s32 channels;

/* stride of image */
	mlib_s32 stride;

/* data pointer of image */
	mlib_u8 *data, *data2;
	mlib_d64 rez[4];
	mlib_d64 divider;

/* check for obvious errors */
	MLIB_IMAGE_CHECK(img);

	if (correl == NULL)
		return (MLIB_NULLPOINTER);

	if (dx < 0 || dy < 0)
		return (MLIB_OUTOFRANGE);

	width = mlib_ImageGetWidth(img) - dx;
	height = mlib_ImageGetHeight(img) - dy;
	type = mlib_ImageGetType(img);
	channels = mlib_ImageGetChannels(img);
	stride = mlib_ImageGetStride(img);
	data = (mlib_u8 *)mlib_ImageGetData(img);

	divider = 1.0 / ((mlib_d64)width * height);

	switch (type) {
	case MLIB_BYTE:
		data2 = data + dy * stride + channels * dx;
		break;
	case MLIB_USHORT:
	case MLIB_SHORT:
		data2 = data + dy * stride + channels * dx * 2;
		break;
	case MLIB_INT:
		data2 = data + dy * stride + channels * dx * 4;
		break;
	default:
		return (MLIB_FAILURE);
	}

	img1 =
	    mlib_ImageSet(&images[0], type, channels, width, height, stride,
	    (void *)data);

	if (!img1)
		return (MLIB_FAILURE);

	img2 =
	    mlib_ImageSet(&images[1], type, channels, width, height, stride,
	    (void *)data2);

	if (!img2)
		return (MLIB_FAILURE);

	switch (type) {
/* handle MLIB_BYTE data type of image */
	case MLIB_BYTE:

		if (channels == 3)
			mlib_c_ImageCrossCorrel_U8_3(img1, img2, rez);
		else
			mlib_c_ImageCrossCorrel_U8_124(img1, img2, rez);
		break;

/* handle MLIB_USHORT data type of image */
	case MLIB_USHORT:

		if (channels == 3)
			mlib_c_ImageCrossCorrel_U16_3(img1, img2, rez);
		else
			mlib_c_ImageCrossCorrel_U16_124(img1, img2, rez);
		break;

/* handle MLIB_SHORT data type of image */
	case MLIB_SHORT:

		if (channels == 3)
			mlib_c_ImageCrossCorrel_S16_3(img1, img2, rez);
		else
			mlib_c_ImageCrossCorrel_S16_124(img1, img2, rez);
		break;

/* handle MLIB_INT data type of image */
	case MLIB_INT:

		if (channels == 3)
			mlib_c_ImageCrossCorrel_S32_3(img1, img2, rez);
		else
			mlib_c_ImageCrossCorrel_S32_124(img1, img2, rez);
		break;

/* discard any other data types */
	default:
		return (MLIB_FAILURE);
	}

	switch (channels) {
	case 1:
		correl[0] = (rez[0] + rez[1] + rez[2] + rez[3]) * divider;
		break;

	case 2:
		correl[0] = (rez[0] + rez[2]) * divider;
		correl[1] = (rez[1] + rez[3]) * divider;
		break;

	case 4:
		correl[3] = rez[3] * divider;
	case 3:
		correl[0] = rez[0] * divider;
		correl[1] = rez[1] * divider;
		correl[2] = rez[2] * divider;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
