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

#pragma ident	"@(#)mlib_ImageStdDev_Fp.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageStdDev_Fp - calculates image standard deviation
 *                            for the input image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageStdDev_Fp(mlib_d64         *stdev,
 *                                      const mlib_image *src,
 *                                      const mlib_d64   *mean)
 *
 * ARGUMENT
 *      stdev  Pointer to standard deviation array
 *      src    Pointer to input image
 *      mean   Pointer to mean array
 *
 * DESCRIPTION
 *
 *           1    w-1 h-1
 *      m = --- * SUM SUM x(i, j)
 *          w*h   i = 0 j = 0
 *
 *                    1    w-1 h-1         2
 *      s = sqrt (   --- * SUM SUM (x(i, j) - m)  )
 *                   w*h   i = 0 j = 0
 *
 * RESTRICTION
 *      src can have 1, 2, 3 or 4 channels. It can be of MLIB_FLOAT or
 *      MLIB_DOUBLE data type.
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_SysMath.h>
#include <mlib_ImageMoment2.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageStdDev_Fp = __mlib_ImageStdDev_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageStdDev_Fp) mlib_ImageStdDev_Fp
    __attribute__((weak, alias("__mlib_ImageStdDev_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_ImageStdDev2ParamF32(
    const mlib_image *src,
    mlib_d64 *stdev);

static void mlib_ImageStdDev2ParamD64(
    const mlib_image *src,
    mlib_d64 *stdev);

/* *********************************************************** */

mlib_status
__mlib_ImageStdDev_Fp(
    mlib_d64 *stdev,
    const mlib_image *src,
    const mlib_d64 *mean)
{
	mlib_d64 rez[4];
	mlib_d64 divider;
	mlib_d64 moment;
	mlib_s32 num_channels;

/* check for obvious errors */
	MLIB_IMAGE_CHECK(src);

	if (stdev == NULL)
		return (MLIB_NULLPOINTER);

	num_channels = mlib_ImageGetChannels(src);

	if (mean == NULL) {
		switch (mlib_ImageGetType(src)) {
		case MLIB_FLOAT:
			mlib_ImageStdDev2ParamF32(src, stdev);
			return (MLIB_SUCCESS);

		case MLIB_DOUBLE:
			mlib_ImageStdDev2ParamD64(src, stdev);
			return (MLIB_SUCCESS);

		default:
			return (MLIB_FAILURE);
		}
	}

	switch (mlib_ImageGetType(src)) {
	case MLIB_FLOAT:
		mlib_ImageMoment2_F32(src, rez);
		break;

	case MLIB_DOUBLE:
		mlib_ImageMoment2_D64(src, rez);
		break;

	default:
		return (MLIB_FAILURE);
	}

	divider = 1.0 / (mlib_ImageGetWidth(src) * mlib_ImageGetHeight(src));
	switch (num_channels) {
	case 1:
		moment = (rez[0] + rez[1] + rez[2] + rez[3]) * divider;
		stdev[0] = mlib_sqrt(mlib_fabs(moment - mean[0] * mean[0]));
		break;

	case 2:
		moment = (rez[0] + rez[2]) * divider;
		stdev[0] = mlib_sqrt(mlib_fabs(moment - mean[0] * mean[0]));
		moment = (rez[1] + rez[3]) * divider;
		stdev[1] = mlib_sqrt(mlib_fabs(moment - mean[1] * mean[1]));
		break;

	case 4:
		moment = rez[3] * divider;
		stdev[3] = mlib_sqrt(mlib_fabs(moment - mean[3] * mean[3]));

	case 3:
		moment = rez[0] * divider;
		stdev[0] = mlib_sqrt(mlib_fabs(moment - mean[0] * mean[0]));
		moment = rez[1] * divider;
		stdev[1] = mlib_sqrt(mlib_fabs(moment - mean[1] * mean[1]));
		moment = rez[2] * divider;
		stdev[2] = mlib_sqrt(mlib_fabs(moment - mean[2] * mean[2]));
		break;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_ImageStdDev2ParamF32(
    const mlib_image *src,
    mlib_d64 *stdev)
{
/* pointer for pixel of source */
	mlib_f32 *srcPixelPtr = (mlib_f32 *)mlib_ImageGetData(src);

/* pointer for line of source */
	mlib_u8 *srcRowPtr = (mlib_u8 *)srcPixelPtr;

/* width of source image */
	mlib_s32 srcWidth = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 srcHeight = mlib_ImageGetHeight(src);

/* linestride = bytes to next row */
	mlib_s32 srcYStride = mlib_ImageGetStride(src);

/* number of channels */
	mlib_s32 numOfChannels = mlib_ImageGetChannels(src);

/* total number of pixels */
	mlib_s32 srcTotalPixNum = srcHeight * srcWidth;
	mlib_s32 pixelNumber;
	mlib_s32 loadsNumber;

/* indices */
	mlib_s32 j, k;
	mlib_d64 sum0 = 0.0, sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
	mlib_d64 mtwo0 = 0.0, mtwo1 = 0.0, mtwo2 = 0.0, mtwo3 = 0.0;
	mlib_d64 divider = 1.0 / (mlib_d64)srcTotalPixNum;
	mlib_d64 ch0, ch1, ch2, ch3;

	if (numOfChannels != 3) {

		pixelNumber = srcWidth << (numOfChannels >> 1);

		if (pixelNumber * 4 == srcYStride) {
			pixelNumber = srcTotalPixNum << (numOfChannels >> 1);
			srcHeight = 1;
		}

		loadsNumber = pixelNumber >> 2;
		pixelNumber &= 3;

		for (j = 0; j < srcHeight; j++) {
			if (loadsNumber) {
				ch0 = (*srcPixelPtr++);
				ch1 = (*srcPixelPtr++);
				ch2 = (*srcPixelPtr++);
			}

			for (k = 0; k < loadsNumber - 1; k++) {

				ch3 = (*srcPixelPtr++);

				sum0 += ch0;
				sum1 += ch1;
				sum2 += ch2;
				sum3 += ch3;

				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
				mtwo3 += ch3 * ch3;

				ch0 = (*srcPixelPtr++);
				ch1 = (*srcPixelPtr++);
				ch2 = (*srcPixelPtr++);
			}

			if (loadsNumber) {
				ch3 = (*srcPixelPtr++);

				sum0 += ch0;
				sum1 += ch1;
				sum2 += ch2;
				sum3 += ch3;

				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
				mtwo3 += ch3 * ch3;
			}

			switch (pixelNumber) {
			case 3:

				ch0 = (*srcPixelPtr++);
				ch1 = (*srcPixelPtr++);
				ch2 = (*srcPixelPtr++);
				sum0 += ch0;
				sum1 += ch1;
				sum2 += ch2;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
				break;
			case 2:
				ch0 = (*srcPixelPtr++);
				ch1 = (*srcPixelPtr++);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				break;
			case 1:
				ch0 = (*srcPixelPtr++);
				sum0 += ch0;
				mtwo0 += ch0 * ch0;
				break;
			}

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_f32 *)srcRowPtr;
		}

		switch (numOfChannels) {
		case 1:
			mtwo0 = (mtwo0 + mtwo1 + mtwo2 + mtwo3) * divider;
			sum0 = (sum0 + sum1 + sum2 + sum3) * divider;
			stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 - sum0 * sum0));
			break;

		case 2:
			mtwo0 = (mtwo0 + mtwo2) * divider;
			mtwo1 = (mtwo1 + mtwo3) * divider;
			sum0 = (sum0 + sum2) * divider;
			sum1 = (sum1 + sum3) * divider;
			stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 - sum0 * sum0));
			stdev[1] = mlib_sqrt(mlib_fabs(mtwo1 - sum1 * sum1));
			break;

		case 4:
			mtwo0 *= divider;
			mtwo1 *= divider;
			mtwo2 *= divider;
			mtwo3 *= divider;
			sum0 *= divider;
			sum1 *= divider;
			sum2 *= divider;
			sum3 *= divider;
			stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 - sum0 * sum0));
			stdev[1] = mlib_sqrt(mlib_fabs(mtwo1 - sum1 * sum1));
			stdev[2] = mlib_sqrt(mlib_fabs(mtwo2 - sum2 * sum2));
			stdev[3] = mlib_sqrt(mlib_fabs(mtwo3 - sum3 * sum3));
			break;
		}
	} else {
/* case 3: */

		pixelNumber = srcWidth;

		if (srcWidth * 4 * 3 == srcYStride) {
			pixelNumber = srcTotalPixNum;
			srcHeight = 1;
		}

		for (j = 0; j < srcHeight; j++) {
			ch0 = (*srcPixelPtr++);
			ch1 = (*srcPixelPtr++);
			ch2 = (*srcPixelPtr++);

			for (k = 1; k < pixelNumber; k++) {
				sum0 += ch0;
				sum1 += ch1;
				sum2 += ch2;

				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;

				ch0 = (*srcPixelPtr++);
				ch1 = (*srcPixelPtr++);
				ch2 = (*srcPixelPtr++);
			}

			sum0 += ch0;
			sum1 += ch1;
			sum2 += ch2;

			mtwo0 += ch0 * ch0;
			mtwo1 += ch1 * ch1;
			mtwo2 += ch2 * ch2;

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_f32 *)srcRowPtr;
		}

		mtwo0 *= divider;
		mtwo1 *= divider;
		mtwo2 *= divider;
		sum0 *= divider;
		sum1 *= divider;
		sum2 *= divider;
		stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 - sum0 * sum0));
		stdev[1] = mlib_sqrt(mlib_fabs(mtwo1 - sum1 * sum1));
		stdev[2] = mlib_sqrt(mlib_fabs(mtwo2 - sum2 * sum2));
	}
}

/* *********************************************************** */

void
mlib_ImageStdDev2ParamD64(
    const mlib_image *src,
    mlib_d64 *stdev)
{
/* pointer for pixel of source */
	mlib_d64 *srcPixelPtr = (mlib_d64 *)mlib_ImageGetData(src);

/* pointer for line of source */
	mlib_u8 *srcRowPtr = (mlib_u8 *)srcPixelPtr;

/* width of source image */
	mlib_s32 srcWidth = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 srcHeight = mlib_ImageGetHeight(src);

/* linestride = bytes to next row */
	mlib_s32 srcYStride = mlib_ImageGetStride(src);

/* number of channels */
	mlib_s32 numOfChannels = mlib_ImageGetChannels(src);

/* total number of pixels */
	mlib_s32 srcTotalPixNum = srcHeight * srcWidth;

/* indices */
	mlib_s32 j, k;
	mlib_s32 pixelNumber;
	mlib_s32 loadsNumber;
	mlib_d64 sum0 = 0.0, sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
	mlib_d64 mtwo0 = 0.0, mtwo1 = 0.0, mtwo2 = 0.0, mtwo3 = 0.0;
	mlib_d64 divider = 1.0 / (mlib_d64)srcTotalPixNum;
	mlib_d64 ch0, ch1, ch2, ch3;

	if (numOfChannels != 3) {

		pixelNumber = srcWidth << (numOfChannels >> 1);

		if (pixelNumber * 8 == srcYStride) {
			pixelNumber = srcTotalPixNum << (numOfChannels >> 1);
			srcHeight = 1;
		}

		loadsNumber = pixelNumber >> 2;
		pixelNumber &= 3;

		for (j = 0; j < srcHeight; j++) {
			for (k = 0; k < loadsNumber; k++) {
				sum0 += ch0 = (*srcPixelPtr++);
				sum1 += ch1 = (*srcPixelPtr++);
				sum2 += ch2 = (*srcPixelPtr++);
				sum3 += ch3 = (*srcPixelPtr++);

				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
				mtwo3 += ch3 * ch3;
			}

			switch (pixelNumber) {

			case 3:
				sum0 += ch0 = (*srcPixelPtr++);
				sum1 += ch1 = (*srcPixelPtr++);
				sum2 += ch2 = *srcPixelPtr;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
				break;

			case 2:
				sum0 += ch0 = (*srcPixelPtr++);
				sum1 += ch1 = *srcPixelPtr;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				break;

			case 1:
				sum0 += ch0 = *srcPixelPtr;
				mtwo0 += ch0 * ch0;
				break;
			}

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_d64 *)srcRowPtr;
		}

		switch (numOfChannels) {

		case 1:
			mtwo0 = (mtwo0 + mtwo1 + mtwo2 + mtwo3) * divider;
			sum0 = (sum0 + sum1 + sum2 + sum3) * divider;
			stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 - sum0 * sum0));
			break;

		case 2:
			mtwo0 = (mtwo0 + mtwo2) * divider;
			mtwo1 = (mtwo1 + mtwo3) * divider;
			sum0 = (sum0 + sum2) * divider;
			sum1 = (sum1 + sum3) * divider;
			stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 - sum0 * sum0));
			stdev[1] = mlib_sqrt(mlib_fabs(mtwo1 - sum1 * sum1));
			break;

		case 4:
			mtwo0 *= divider;
			mtwo1 *= divider;
			mtwo2 *= divider;
			mtwo3 *= divider;
			sum0 *= divider;
			sum1 *= divider;
			sum2 *= divider;
			sum3 *= divider;
			stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 - sum0 * sum0));
			stdev[1] = mlib_sqrt(mlib_fabs(mtwo1 - sum1 * sum1));
			stdev[2] = mlib_sqrt(mlib_fabs(mtwo2 - sum2 * sum2));
			stdev[3] = mlib_sqrt(mlib_fabs(mtwo3 - sum3 * sum3));
			break;
		}
	} else {
/* case 3: */

		pixelNumber = srcWidth;

		if (srcWidth * 8 * 3 == srcYStride) {
			pixelNumber = srcTotalPixNum;
			srcHeight = 1;
		}

		for (j = 0; j < srcHeight; j++) {
			for (k = 0; k < pixelNumber; k++) {
				sum0 += ch0 = (*srcPixelPtr++);
				sum1 += ch1 = (*srcPixelPtr++);
				sum2 += ch2 = (*srcPixelPtr++);

				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
			}

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_d64 *)srcRowPtr;
		}

		mtwo0 *= divider;
		mtwo1 *= divider;
		mtwo2 *= divider;
		sum0 *= divider;
		sum1 *= divider;
		sum2 *= divider;
		stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 - sum0 * sum0));
		stdev[1] = mlib_sqrt(mlib_fabs(mtwo1 - sum1 * sum1));
		stdev[2] = mlib_sqrt(mlib_fabs(mtwo2 - sum2 * sum2));
	}
}

/* *********************************************************** */
