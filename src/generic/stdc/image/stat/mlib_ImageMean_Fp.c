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

#pragma ident	"@(#)mlib_ImageMean_Fp.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageMean_Fp - calculates image mean for the input image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMean_Fp(mlib_d64         *mean,
 *                                    const mlib_image *src)
 *
 * ARGUMENTS
 *  mean  Pointer to mean array
 *  src   Pointer to input image
 *
 * DESCRIPTION
 *
 *           1    w-1 h-1
 *      m = --- * SUM SUM x(i, j)
 *          w*h   i = 0 j = 0
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

#pragma weak mlib_ImageMean_Fp = __mlib_ImageMean_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMean_Fp) mlib_ImageMean_Fp
    __attribute__((weak, alias("__mlib_ImageMean_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_ImageMeanD64Case(
    const mlib_image *src,
    mlib_d64 *mean);

static void mlib_ImageMeanF32Case(
    const mlib_image *src,
    mlib_d64 *mean);

/* *********************************************************** */

mlib_status
__mlib_ImageMean_Fp(
    mlib_d64 *mean,
    const mlib_image *src)
{
/* check for obvious errors */
	MLIB_IMAGE_CHECK(src);

	if (mean == NULL)
		return (MLIB_NULLPOINTER);

	switch (mlib_ImageGetType(src)) {

/* handle MLIB_FLOAT data type of image */
	case MLIB_FLOAT:
		mlib_ImageMeanF32Case(src, mean);
		return (MLIB_SUCCESS);

/* handle MLIB_DOUBLE data type of image */
	case MLIB_DOUBLE:
		mlib_ImageMeanD64Case(src, mean);
		return (MLIB_SUCCESS);

/* discard any other data types */
	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

static void
mlib_ImageMeanD64Case(
    const mlib_image *src,
    mlib_d64 *mean)
{
/* pointer for pixel of source */
	mlib_d64 *srcPixelPtr = (mlib_d64 *)mlib_ImageGetData(src);

/* pointer for line of source */
	mlib_u8 *srcLinePtr = (mlib_u8 *)srcPixelPtr;

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
	mlib_d64 divider = 1.0 / (mlib_d64)srcTotalPixNum;

/* indices */
	mlib_s32 j, k;
	mlib_d64 firstAccumulator = 0.0;
	mlib_d64 secondAccumulator = 0.0;
	mlib_d64 thirdAccumulator = 0.0;
	mlib_d64 forthAccumulator = 0.0;

	if (8 * numOfChannels * srcWidth == srcYStride) {
		srcWidth = srcTotalPixNum;
		srcHeight = 1;
	}

	switch (numOfChannels) {
	case 1:
	    {
		    for (j = 0; j < srcHeight; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (k = 0; k < (srcWidth >> 2); k++) {
				    firstAccumulator += (*srcPixelPtr++);
				    secondAccumulator += (*srcPixelPtr++);
				    thirdAccumulator += (*srcPixelPtr++);
				    forthAccumulator += (*srcPixelPtr++);
			    }

			    if (srcWidth & 2) {
				    firstAccumulator += (*srcPixelPtr++);
				    secondAccumulator += (*srcPixelPtr++);
			    }

			    if (srcWidth & 1)
				    firstAccumulator += *srcPixelPtr;

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_d64 *)srcLinePtr;
		    }

		    firstAccumulator =
			(forthAccumulator + thirdAccumulator +
			secondAccumulator + firstAccumulator) * divider;

		    mean[0] = firstAccumulator;
	    }

		break;

	case 2:
	    {
		    for (j = 0; j < srcHeight; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (k = 0; k < (srcWidth >> 1); k++) {
				    firstAccumulator += (*srcPixelPtr++);
				    secondAccumulator += (*srcPixelPtr++);
				    thirdAccumulator += (*srcPixelPtr++);
				    forthAccumulator += (*srcPixelPtr++);
			    }

			    if (srcWidth & 1) {
				    firstAccumulator += (*srcPixelPtr++);
				    secondAccumulator += (*srcPixelPtr++);
			    }

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_d64 *)srcLinePtr;
		    }

		    firstAccumulator =
			(firstAccumulator + thirdAccumulator) * divider;
		    secondAccumulator =
			(secondAccumulator + forthAccumulator) * divider;

		    mean[0] = firstAccumulator;
		    mean[1] = secondAccumulator;
	    }

		break;

	case 3:
	    {
		    for (j = 0; j < srcHeight; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (k = 0; k < srcWidth; k++) {
				    firstAccumulator += (*srcPixelPtr++);
				    secondAccumulator += (*srcPixelPtr++);
				    thirdAccumulator += (*srcPixelPtr++);
			    }

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_d64 *)srcLinePtr;
		    }

		    firstAccumulator *= divider;
		    secondAccumulator *= divider;
		    thirdAccumulator *= divider;

		    mean[0] = firstAccumulator;
		    mean[1] = secondAccumulator;
		    mean[2] = thirdAccumulator;
	    }

		break;

	case 4:
	    {
		    for (j = 0; j < srcHeight; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (k = 0; k < srcWidth; k++) {
				    firstAccumulator += (*srcPixelPtr++);
				    secondAccumulator += (*srcPixelPtr++);
				    thirdAccumulator += (*srcPixelPtr++);
				    forthAccumulator += (*srcPixelPtr++);
			    }

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_d64 *)srcLinePtr;
		    }

		    firstAccumulator *= divider;
		    secondAccumulator *= divider;
		    thirdAccumulator *= divider;
		    forthAccumulator *= divider;

		    mean[0] = firstAccumulator;
		    mean[1] = secondAccumulator;
		    mean[2] = thirdAccumulator;
		    mean[3] = forthAccumulator;
	    }

		break;
	}
}

/* *********************************************************** */

static void
mlib_ImageMeanF32Case(
    const mlib_image *src,
    mlib_d64 *mean)
{
/* pointer for pixel of source */
	mlib_f32 *srcPixelPtr = (mlib_f32 *)mlib_ImageGetData(src);

/* pointer for line of source */
	mlib_u8 *srcLinePtr = (mlib_u8 *)srcPixelPtr;

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
	mlib_d64 divider = 1.0 / srcTotalPixNum;

/* indices */
	mlib_s32 j, k;
	mlib_d64 firstAccumulator = 0.0;
	mlib_d64 secondAccumulator = 0.0;
	mlib_d64 thirdAccumulator = 0.0;
	mlib_d64 forthAccumulator = 0.0;

	if (4 * numOfChannels * srcWidth == srcYStride) {
		srcWidth = srcTotalPixNum;
		srcHeight = 1;
	}

	switch (numOfChannels) {
	case 1:
	    {
		    for (j = 0; j < srcHeight; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (k = 0; k < (srcWidth >> 2); k++) {
				    firstAccumulator += (*srcPixelPtr++);
				    secondAccumulator += (*srcPixelPtr++);
				    thirdAccumulator += (*srcPixelPtr++);
				    forthAccumulator += (*srcPixelPtr++);
			    }

			    if (srcWidth & 2) {
				    firstAccumulator += (*srcPixelPtr++);
				    secondAccumulator += (*srcPixelPtr++);
			    }

			    if (srcWidth & 1)
				    firstAccumulator += *srcPixelPtr;

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_f32 *)srcLinePtr;
		    }

		    firstAccumulator =
			(firstAccumulator + secondAccumulator +
			thirdAccumulator + forthAccumulator) * divider;

		    mean[0] = firstAccumulator;
	    }

		break;

	case 2:
	    {
		    for (j = 0; j < srcHeight; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (k = 0; k < (srcWidth >> 1); k++) {
				    firstAccumulator += (*srcPixelPtr++);
				    secondAccumulator += (*srcPixelPtr++);
				    thirdAccumulator += (*srcPixelPtr++);
				    forthAccumulator += (*srcPixelPtr++);
			    }

			    if (srcWidth & 1) {
				    firstAccumulator += (*srcPixelPtr++);
				    secondAccumulator += (*srcPixelPtr++);
			    }

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_f32 *)srcLinePtr;
		    }

		    firstAccumulator =
			(firstAccumulator + thirdAccumulator) * divider;
		    secondAccumulator =
			(secondAccumulator + forthAccumulator) * divider;

		    mean[0] = firstAccumulator;
		    mean[1] = secondAccumulator;
	    }

		break;

	case 3:
	    {
		    for (j = 0; j < srcHeight; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (k = 0; k < srcWidth; k++) {
				    firstAccumulator += (*srcPixelPtr++);
				    secondAccumulator += (*srcPixelPtr++);
				    thirdAccumulator += (*srcPixelPtr++);
			    }

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_f32 *)srcLinePtr;
		    }

		    firstAccumulator *= divider;
		    secondAccumulator *= divider;
		    thirdAccumulator *= divider;

		    mean[0] = firstAccumulator;
		    mean[1] = secondAccumulator;
		    mean[2] = thirdAccumulator;
	    }

		break;

	case 4:
	    {
		    for (j = 0; j < srcHeight; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (k = 0; k < srcWidth; k++) {
				    firstAccumulator += (*srcPixelPtr++);
				    secondAccumulator += (*srcPixelPtr++);
				    thirdAccumulator += (*srcPixelPtr++);
				    forthAccumulator += (*srcPixelPtr++);
			    }

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_f32 *)srcLinePtr;
		    }

		    firstAccumulator *= divider;
		    secondAccumulator *= divider;
		    thirdAccumulator *= divider;
		    forthAccumulator *= divider;

		    mean[0] = firstAccumulator;
		    mean[1] = secondAccumulator;
		    mean[2] = thirdAccumulator;
		    mean[3] = forthAccumulator;
	    }

		break;
	}
}

/* *********************************************************** */
