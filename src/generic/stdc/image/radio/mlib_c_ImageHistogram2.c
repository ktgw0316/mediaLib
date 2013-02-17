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

#pragma ident	"@(#)mlib_c_ImageHistogram2.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageHistogram2 - image histogram
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageHistogram2(mlib_s32         **histo,
 *                                       const mlib_image *img,
 *                                       const mlib_s32   *numBins,
 *                                       const mlib_s32   *lowValue,
 *                                       const mlib_s32   *highValue,
 *                                       mlib_s32         xStart,
 *                                       mlib_s32         yStart,
 *                                       mlib_s32         xPeriod,
 *                                       mlib_s32         yPeriod);
 *
 * ARGUMENT
 *      histo      The histogram table. histo[c] is for channel c.
 *      img        The pointer to the input image.
 *      numBins    The number of bins for each channel of the image
 *      lowValue   The lowest pixel value checked for each channel.
 *      highValue  The highest pixel value checked for each channel.
 *                 Note when counting the pixel values, this highValue
 *                 is not included.
 *      xStart     The initial X sample coordinate.
 *      yStart     The initial Y sample coordinate.
 *      xPeriod    The X sampling rate, xPeriod >= 1.
 *      yPeriod    The Y sampling rate, yPeriod >= 1.
 *
 * RESTRICTION
 *      The image can have 1, 2, 3 or 4 channels.
 *      It can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *      The histogram must have the same number of channels as the image has.
 *      Range from lowValue[k] to (highValue[k] - 1) must be valid subrange of
 *      image type range.
 *
 * DESCRIPTION
 *      The mlib_ImageHistogram2 function scans an image, counts number of
 *      pixels within a given range for each channel of the image, and
 *      generates a histogram.
 *
 *      One entry of the histogram, or a bin, is used to accumulate the
 *      number of pixels within a certain sub-range. The legal pixel range
 *      and the number of bins may be controlled separately. If binWidth is
 *      defined as (highValue - lowValue)/numBins, bin i will count pixel
 *      values in the range
 *
 *              lowValue + i*binWidth <= x < lowValue + (i + 1)*binWidth.
 *
 *      The set of pixels scanned may furthermore be reduced by specifying
 *      "xPeriod" and "yPeriod" parameters that specify the sampling rate
 *      along each axis.
 *
 *      The set of pixels to be accumulated may be obtained by intersecting
 *      the grid (xStart + i*xPeriod, yStart + j*yPeriod); i, j >= 0 with
 *      the bounding rectangle of the image.
 *
 *      It is the user's responsibility to clear the histogram table before
 *      this function is called.
 *
 *      It is the user's responsibility to make sure that the histogram
 *      table supplied is suitable for the source image and the parameters.
 *      Otherwise, the result of this function is undefined.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageHistogram.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageHistogram2 = __mlib_ImageHistogram2

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageHistogram2) mlib_ImageHistogram2
    __attribute__((weak, alias("__mlib_ImageHistogram2")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageHistogram2(
    mlib_s32 **histo,
    const mlib_image *img,
    const mlib_s32 *numBins,
    const mlib_s32 *lowValue,
    const mlib_s32 *highValue,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 xPeriod,
    mlib_s32 yPeriod)
{
	mlib_s32 stride, width, height, nchan;
	mlib_type type;
	void *src;
	mlib_d64 one_per_binWidth[4];
	mlib_s32 i;

	MLIB_IMAGE_CHECK(img);

	if (histo == NULL || numBins == NULL ||
	    lowValue == NULL || highValue == NULL) {
		return (MLIB_NULLPOINTER);
	}

	if (xPeriod < 1 || yPeriod < 1)
		return (MLIB_OUTOFRANGE);

	while (xStart < 0)
		xStart += xPeriod;
	while (yStart < 0)
		yStart += yPeriod;

	type = mlib_ImageGetType(img);
	nchan = mlib_ImageGetChannels(img);
	width = mlib_ImageGetWidth(img);
	height = mlib_ImageGetHeight(img);
	stride = mlib_ImageGetStride(img);
	src = mlib_ImageGetData(img);

	for (i = 0; i < nchan; i++) {
		if (highValue[i] <= lowValue[i] || numBins[i] <= 0)
			return (MLIB_OUTOFRANGE);
		one_per_binWidth[i] =
		    numBins[i] / (highValue[i] - (mlib_d64)lowValue[i]);
	}

	switch (type) {
	case MLIB_BYTE:
		return mlib_ImageHistogram2_U8(histo, src, width, height, nchan,
		    stride, lowValue, highValue, one_per_binWidth, xStart,
		    yStart, xPeriod, yPeriod);
	case MLIB_SHORT:
		return mlib_ImageHistogram2_S16(histo, src, width, height,
		    nchan, stride, lowValue, highValue, one_per_binWidth,
		    xStart, yStart, xPeriod, yPeriod);
	case MLIB_USHORT:
		return mlib_ImageHistogram2_U16(histo, src, width, height,
		    nchan, stride, lowValue, highValue, one_per_binWidth,
		    xStart, yStart, xPeriod, yPeriod);
	case MLIB_INT:
		return mlib_ImageHistogram2_S32(histo, src, width, height,
		    nchan, stride, lowValue, highValue, one_per_binWidth,
		    xStart, yStart, xPeriod, yPeriod);
	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */
