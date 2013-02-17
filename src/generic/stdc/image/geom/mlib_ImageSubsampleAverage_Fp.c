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

#pragma ident	"@(#)mlib_ImageSubsampleAverage_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageSubsampleAverage_Fp - scaling image down with box filter
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageSubsampleAverage_Fp(mlib_image       *dst,
 *                                                const mlib_image *src,
 *                                                mlib_d64         xscale,
 *                                                mlib_d64         yscale)
 *
 * ARGUMENTS
 *      dst       The pointer to the destination image.
 *      src       The pointer to the source image.
 *      xscale    The x scale factor.
 *      yscale    The y scale factor.
 *
 * DESCRIPTION
 *      This function scales image down with an adaptive
 *      box filter.
 *
 *      The subsampling algorithm performs the scaling
 *      operation by averaging all the pixel values from
 *      a block in the source image that correspond to
 *      the destination pixel.
 *
 *      The width and height of the source block for a
 *      destination pixel are computed as:
 *
 *              blockX = (mlib_s32)ceil(1.0/xscale);
 *              blockY = (mlib_s32)ceil(1.0/yscale);
 *
 *      If we denote a pixel's location in an image by its
 *      column number and row number (both counted from 0),
 *      the destination pixel at (i, j) is backward mapped
 *      to the source block whose upper-left corner pixel
 *      is at (xValues[i], yValues[j]), where
 *
 *              xValues[i] = (mlib_s32)(i/xscale + 0.5);
 *              yValues[j] = (mlib_s32)(j/yscale + 0.5);
 *
 *      The width and height of filled area in destination
 *      are restricted by
 *
 *              dstW = (mlib_s32)(srcWidth * xscale)
 *              dstH = (mlib_s32)(srcHeight * yscale)
 *
 *      where srcWidth and srcHeight are width and height of
 *      the source image.
 *
 *      Since the block size in source is defined from scale
 *      factors with roundup, some blocks (the most right and
 *      the most bottom blocks) may overrun border of source
 *      image by 1 pixel. In this case, such blocks are moved
 *      by 1 pixel to left/up direction in order to be inside
 *      of the source image.
 *
 * RESTRICTION
 *      src and dst must be the same type and the same number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be in MLIB_FLOAT or
 *      MLIB_DOUBLE data type.
 *
 *      0.0 < xscale <= 1.0
 *      0.0 < yscale <= 1.0
 *
 * SEE ALSO
 *      mlib_ImageSubsampleBinaryToGray
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageSubsampleAverage_Fp.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageSubsampleAverage_Fp = __mlib_ImageSubsampleAverage_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageSubsampleAverage_Fp) mlib_ImageSubsampleAverage_Fp
    __attribute__((weak, alias("__mlib_ImageSubsampleAverage_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageSubsampleAverage_Fp(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 xscale,
    mlib_d64 yscale)
{
	mlib_type type;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);

	if (!(xscale > 0 && xscale <= 1 && yscale > 0 && yscale <= 1)) {
		return (MLIB_FAILURE);
	}

	type = mlib_ImageGetType(dst);

	switch (type) {
	case MLIB_FLOAT:
		return mlib_ImageSubsampleAverage_F32_main(dst, src, xscale,
		    yscale, 0);
	case MLIB_DOUBLE:
		return mlib_ImageSubsampleAverage_D64_main(dst, src, xscale,
		    yscale, 0);
	default:
		break;
	}

	return (MLIB_FAILURE);
}

mlib_status
__mlib_ImageSubsampleAverage_ty_Fp(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 xscale,
    mlib_d64 yscale,
    mlib_s32 ybegin)
{
	mlib_type type;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);

	if (!(xscale > 0 && xscale <= 1 && yscale > 0 && yscale <= 1)) {
		return (MLIB_FAILURE);
	}

	type = mlib_ImageGetType(dst);

	switch (type) {
	case MLIB_FLOAT:
		return mlib_ImageSubsampleAverage_F32_main(dst, src, xscale,
		    yscale, ybegin);
	case MLIB_DOUBLE:
		return mlib_ImageSubsampleAverage_D64_main(dst, src, xscale,
		    yscale, ybegin);
	default:
		break;
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */
