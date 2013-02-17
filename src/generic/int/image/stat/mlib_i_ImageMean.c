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

#pragma ident	"@(#)mlib_i_ImageMean.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageMean - calculates image mean for the input image
 *
 * SYNOPSIS
 *  mlib_status mlib_ImageMean(mlib_d64         *mean,
 *                             const mlib_image *src)
 *
 * ARGUMENT
 *      mean  Pointer to mean array
 *      src   Pointer to input image
 *
 * RESTRICTION
 *
 *      The image can have 1, 2, 3 or 4 channels.
 *      It can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *
 *      For each channel calculates mean value:
 *
 *           1    w-1 h-1
 *      m = --- * SUM SUM x(i, j)
 *          w*h   i = 0 j = 0
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

#ifndef _NO_LONGLONG /* for 64-bit platform */
/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMean = __mlib_ImageMean

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMean) mlib_ImageMean
    __attribute__((weak, alias("__mlib_ImageMean")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MASK_00FF00FF	0x00FF00FF
#define	MASK_0000FFFF	0x0000FFFF

/* *********************************************************** */

static void mlib_c_ImageMeanU8Case(
    const mlib_image *src,
    mlib_d64 *mean);
static void mlib_c_ImageMeanS16Case(
    const mlib_image *src,
    mlib_d64 *mean);
static void mlib_c_ImageMeanU16Case(
    const mlib_image *src,
    mlib_d64 *mean);
static void mlib_c_ImageMeanS32Case(
    const mlib_image *src,
    mlib_d64 *mean);

/* *********************************************************** */

mlib_status
__mlib_ImageMean(
    mlib_d64 *mean,
    const mlib_image *src)
{
/* check for obvious errors */
	MLIB_IMAGE_CHECK(src);

	if (mean == NULL)
		return (MLIB_NULLPOINTER);

	switch (mlib_ImageGetType(src)) {

/* handle MLIB_BYTE data type of image */
	case MLIB_BYTE:
		mlib_c_ImageMeanU8Case(src, mean);
		return (MLIB_SUCCESS);

/* handle MLIB_SHORT data type of image */
	case MLIB_SHORT:
		mlib_c_ImageMeanS16Case(src, mean);
		return (MLIB_SUCCESS);

	case MLIB_USHORT:
		mlib_c_ImageMeanU16Case(src, mean);
		return (MLIB_SUCCESS);

/* handle MLIB_INT data type of image */
	case MLIB_INT:
		mlib_c_ImageMeanS32Case(src, mean);
		return (MLIB_SUCCESS);

/* discard any other data types */
	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

void
mlib_c_ImageMeanU8Case(
    const mlib_image *src,
    mlib_d64 *mean)
{
	mlib_s64 lmean[4];
/* pointer for pixel of source */
	mlib_u8 *srcPixelPtr = (mlib_u8 *)mlib_ImageGetData(src);
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
	mlib_s64 sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0;

/* compute mean3_off(align, sumn) */
	const mlib_u8 al2off[6] = { 0, 2, 1, 0, 2, 1 };
/* indices */
	mlib_s32 i, k, j;
	mlib_s32 alignSteps;
	mlib_s32 byteNumber;
	mlib_s32 loadsNumber;
	mlib_s32 cycleNumber;
	mlib_u32 firstAccumulator;
	mlib_u32 secondAccumulator;
	mlib_u32 srcLoad;
	mlib_u32 *srcLoadPtr;
	mlib_u32 mask;

	if (numOfChannels * srcWidth == srcYStride) {
		srcWidth = srcTotalPixNum;
		srcHeight = 1;
	}

	if (srcWidth < 4) {
		for (k = 0; k < numOfChannels; k++) {
			sum0 = 0;
			for (i = 0; i < srcHeight; i++) {
#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
				for (j = 0; j < srcWidth; j++) {
					sum0 += (mlib_s64)srcPixelPtr[
					    i*srcYStride + j*numOfChannels + k];
				}
			}
			mean[k] = sum0 * divider;

		}
		return;
	}

	switch (numOfChannels) {

	case 1:
		for (j = 0; j < srcHeight; j++) {
			alignSteps = (mlib_addr)srcPixelPtr & 3;
			byteNumber = srcWidth + alignSteps - 4;
			loadsNumber = byteNumber >> 2;
			cycleNumber = loadsNumber >> 8;
			byteNumber &= 3;
			loadsNumber &= 0xff;

			firstAccumulator = secondAccumulator = 0;
			srcLoadPtr = (mlib_u32 *)(srcPixelPtr - alignSteps);
#ifdef _LITTLE_ENDIAN
			mask = (mlib_u32)0xffffffff << (alignSteps << 3);
#else /* _LITTLE_ENDIAN */
			mask = (mlib_u32)0xffffffff >> (alignSteps << 3);
#endif /* _LITTLE_ENDIAN */
			srcLoad = mask & (*srcLoadPtr++);
			firstAccumulator += (srcLoad & MASK_00FF00FF);
			secondAccumulator += ((srcLoad >> 8) & MASK_00FF00FF);

			for (i = 0; i < cycleNumber; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (k = 0; k < 64; k++) {
					srcLoad = (*srcLoadPtr++);
					firstAccumulator +=
					    (srcLoad & MASK_00FF00FF);
					secondAccumulator +=
					    ((srcLoad >> 8) & MASK_00FF00FF);

					srcLoad = (*srcLoadPtr++);
					firstAccumulator +=
					    (srcLoad & MASK_00FF00FF);
					secondAccumulator +=
					    ((srcLoad >> 8) & MASK_00FF00FF);

					srcLoad = (*srcLoadPtr++);
					firstAccumulator +=
					    (srcLoad & MASK_00FF00FF);
					secondAccumulator +=
					    ((srcLoad >> 8) & MASK_00FF00FF);

					srcLoad = (*srcLoadPtr++);
					firstAccumulator +=
					    (srcLoad & MASK_00FF00FF);
					secondAccumulator +=
					    ((srcLoad >> 8) & MASK_00FF00FF);
				}

				sum0 +=
				    (secondAccumulator & MASK_0000FFFF) +
				    ((secondAccumulator) >> 16) +
				    (firstAccumulator & MASK_0000FFFF) +
				    ((firstAccumulator) >> 16);

				firstAccumulator = secondAccumulator = 0;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < loadsNumber; i++) {
				srcLoad = (*srcLoadPtr++);
				firstAccumulator += (srcLoad & MASK_00FF00FF);
				secondAccumulator +=
				    ((srcLoad >> 8) & MASK_00FF00FF);
			}

			if (byteNumber) {
				srcLoad = (*srcLoadPtr++);
#ifdef _LITTLE_ENDIAN
				srcLoad = srcLoad & ~((mlib_u32)0xffffffff <<
					(byteNumber << 3));
#else /* _LITTLE_ENDIAN */
				srcLoad = srcLoad & ~((mlib_u32)0xffffffff >>
					(byteNumber << 3));
#endif /* _LITTLE_ENDIAN */
				firstAccumulator += (srcLoad & MASK_00FF00FF);
				secondAccumulator += ((srcLoad >> 8) &
					MASK_00FF00FF);
			}

			sum0 +=
			    (secondAccumulator & MASK_0000FFFF) +
			    ((secondAccumulator) >> 16) +
			    (firstAccumulator & MASK_0000FFFF) +
			    ((firstAccumulator) >> 16);

			firstAccumulator = secondAccumulator = 0;

			srcLinePtr += srcYStride;
			srcPixelPtr = srcLinePtr;
		}

		mean[0] = sum0 * divider;
		break;

	case 2:
		lmean[0] = lmean[1] = 0;
		for (j = 0; j < srcHeight; j++) {
			alignSteps = (mlib_addr)srcPixelPtr & 3;
			byteNumber = srcWidth * 2 + alignSteps - 4;
			loadsNumber = byteNumber >> 2;
			cycleNumber = loadsNumber >> 8;
			byteNumber &= 3;
			loadsNumber &= 0xff;

			firstAccumulator = secondAccumulator = 0;
			srcLoadPtr = (mlib_u32 *)(srcPixelPtr - alignSteps);
#ifdef _LITTLE_ENDIAN
			mask = (mlib_u32)0xffffffff << (alignSteps << 3);
#else /* _LITTLE_ENDIAN */
			mask = (mlib_u32)0xffffffff >> (alignSteps << 3);
#endif /* _LITTLE_ENDIAN */
			srcLoad = mask & (*srcLoadPtr++);
			firstAccumulator += (srcLoad & MASK_00FF00FF);
			secondAccumulator += ((srcLoad >> 8) & MASK_00FF00FF);

			for (i = 0; i < cycleNumber; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (k = 0; k < 64; k++) {
					srcLoad = (*srcLoadPtr++);
					firstAccumulator +=
					    (srcLoad & MASK_00FF00FF);
					secondAccumulator +=
					    ((srcLoad >> 8) & MASK_00FF00FF);

					srcLoad = (*srcLoadPtr++);
					firstAccumulator +=
					    (srcLoad & MASK_00FF00FF);
					secondAccumulator +=
					    ((srcLoad >> 8) & MASK_00FF00FF);

					srcLoad = (*srcLoadPtr++);
					firstAccumulator +=
					    (srcLoad & MASK_00FF00FF);
					secondAccumulator +=
					    ((srcLoad >> 8) & MASK_00FF00FF);

					srcLoad = (*srcLoadPtr++);
					firstAccumulator +=
					    (srcLoad & MASK_00FF00FF);
					secondAccumulator +=
					    ((srcLoad >> 8) & MASK_00FF00FF);
				}

				sum0 +=
				    (secondAccumulator & MASK_0000FFFF) +
				    ((secondAccumulator) >> 16);

				sum1 +=
				    (firstAccumulator & MASK_0000FFFF) +
				    ((firstAccumulator) >> 16);

				firstAccumulator = secondAccumulator = 0;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < loadsNumber; i++) {
				srcLoad = (*srcLoadPtr++);
				firstAccumulator += (srcLoad & MASK_00FF00FF);
				secondAccumulator +=
				    ((srcLoad >> 8) & MASK_00FF00FF);
			}

			if (byteNumber) {

				srcLoad = (*srcLoadPtr++);
#ifdef _LITTLE_ENDIAN
				srcLoad = srcLoad & ~((mlib_u32)0xffffffff <<
					(byteNumber << 3));
#else /* _LITTLE_ENDIAN */
				srcLoad = srcLoad & ~((mlib_u32)0xffffffff >>
					(byteNumber << 3));
#endif /* _LITTLE_ENDIAN */
				firstAccumulator += (srcLoad & MASK_00FF00FF);
				secondAccumulator += ((srcLoad >> 8) &
					MASK_00FF00FF);
			}

			sum0 +=
			    (secondAccumulator & MASK_0000FFFF) +
			    ((secondAccumulator) >> 16);

			sum1 +=
			    (firstAccumulator & MASK_0000FFFF) +
			    ((firstAccumulator) >> 16);

#ifdef _LITTLE_ENDIAN
			lmean[alignSteps & 1] += sum1;
			lmean[(alignSteps & 1) ^ 1] += sum0;
#else /* _LITTLE_ENDIAN */
			lmean[alignSteps & 1] += sum0;
			lmean[(alignSteps & 1) ^ 1] += sum1;
#endif /* _LITTLE_ENDIAN */

			firstAccumulator = secondAccumulator = 0;
			sum0 = sum1 = 0;

			srcLinePtr += srcYStride;
			srcPixelPtr = srcLinePtr;
		}
		mean[alignSteps & 1] = lmean[alignSteps & 1] * divider;
		mean[(alignSteps & 1) ^ 1] =
				lmean[(alignSteps & 1) ^ 1] * divider;

		break;

	case 3: {
/* It's the worst case, we'll need some extra variables here */
		    mlib_u32 secondLoad, thirdLoad;
		    mlib_u32 thirdAccumulator;

			lmean[0] = lmean[1] = lmean[2] = 0;
		    for (j = 0; j < srcHeight; j++) {
			    alignSteps = (mlib_addr)srcPixelPtr & 3;
			    byteNumber = srcWidth * 3 + alignSteps;

			    firstAccumulator = secondAccumulator =
				thirdAccumulator = 0;
			    sum0 = sum1 = sum2 = 0;

			    srcLoadPtr = (mlib_u32 *)(srcPixelPtr - alignSteps);
#ifdef _LITTLE_ENDIAN
			    mask = (mlib_u32)0xffffffff << (alignSteps << 3);
#else /* _LITTLE_ENDIAN */
			    mask = (mlib_u32)0xffffffff >> (alignSteps << 3);
#endif /* _LITTLE_ENDIAN */
			    srcLoad = mask & (*srcLoadPtr++);

			    for (i = 0; i <= byteNumber - 12 * 128 - 4;
				i += 12 * 128) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(1)
#endif /* __SUNPRO_C */
				    for (k = 0; k < 64; k++) {
					    secondLoad = (*srcLoadPtr++);
					    thirdLoad = (*srcLoadPtr++);

					    firstAccumulator +=
						((srcLoad >> 8) &
						MASK_00FF00FF);
					    secondAccumulator +=
						(srcLoad & MASK_00FF00FF);
#ifdef _LITTLE_ENDIAN
					    thirdAccumulator +=
						((secondLoad >> 8) &
						MASK_00FF00FF);
					    firstAccumulator +=
						(secondLoad & MASK_00FF00FF);
					    secondAccumulator +=
						((thirdLoad >> 8) &
						MASK_00FF00FF);
					    thirdAccumulator +=
						(thirdLoad & MASK_00FF00FF);
#else /* _LITTLE_ENDIAN */
					    secondAccumulator +=
						((secondLoad >> 8) &
						MASK_00FF00FF);
					    thirdAccumulator +=
						(secondLoad & MASK_00FF00FF);
					    thirdAccumulator +=
						((thirdLoad >> 8) &
						MASK_00FF00FF);
					    firstAccumulator +=
						(thirdLoad & MASK_00FF00FF);
#endif /* _LITTLE_ENDIAN */

					    srcLoad = (*srcLoadPtr++);
					    secondLoad = (*srcLoadPtr++);
					    thirdLoad = (*srcLoadPtr++);

					    firstAccumulator +=
						((srcLoad >> 8) &
						MASK_00FF00FF);
					    secondAccumulator +=
						(srcLoad & MASK_00FF00FF);
#ifdef _LITTLE_ENDIAN
					    thirdAccumulator +=
						((secondLoad >> 8) &
						MASK_00FF00FF);
					    firstAccumulator +=
						(secondLoad & MASK_00FF00FF);
					    secondAccumulator +=
						((thirdLoad >> 8) &
						MASK_00FF00FF);
					    thirdAccumulator +=
						(thirdLoad & MASK_00FF00FF);
#else /* _LITTLE_ENDIAN */
					    secondAccumulator +=
						((secondLoad >> 8) &
						MASK_00FF00FF);
					    thirdAccumulator +=
						(secondLoad & MASK_00FF00FF);
					    thirdAccumulator +=
						((thirdLoad >> 8) &
						MASK_00FF00FF);
					    firstAccumulator +=
						(thirdLoad & MASK_00FF00FF);
#endif /* _LITTLE_ENDIAN */
					    srcLoad = (*srcLoadPtr++);
				    }

				    sum0 +=
					(secondAccumulator & MASK_0000FFFF) +
					((firstAccumulator) >> 16);

				    sum1 +=
					(thirdAccumulator & MASK_0000FFFF) +
					((secondAccumulator) >> 16);

				    sum2 +=
					(firstAccumulator & MASK_0000FFFF) +
					((thirdAccumulator) >> 16);
				    thirdAccumulator = firstAccumulator =
					secondAccumulator = 0;
			    }
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (; i <= byteNumber - 12 - 4; i += 12) {
				    secondLoad = (*srcLoadPtr++);
				    thirdLoad = (*srcLoadPtr++);

				    firstAccumulator +=
					((srcLoad >> 8) & MASK_00FF00FF);
				    secondAccumulator +=
					(srcLoad & MASK_00FF00FF);
#ifdef _LITTLE_ENDIAN
				    thirdAccumulator +=
					((secondLoad >> 8) & MASK_00FF00FF);
				    firstAccumulator +=
					(secondLoad & MASK_00FF00FF);
				    secondAccumulator +=
					((thirdLoad >> 8) & MASK_00FF00FF);
				    thirdAccumulator +=
					(thirdLoad & MASK_00FF00FF);
#else /* _LITTLE_ENDIAN */
				    secondAccumulator +=
					((secondLoad >> 8) & MASK_00FF00FF);
				    thirdAccumulator +=
					(secondLoad & MASK_00FF00FF);
				    thirdAccumulator +=
					((thirdLoad >> 8) & MASK_00FF00FF);
				    firstAccumulator +=
					(thirdLoad & MASK_00FF00FF);
#endif /* _LITTLE_ENDIAN */
				    srcLoad = (*srcLoadPtr++);
			    }

			    byteNumber -= i;

			    if (byteNumber >= 12) {
				    secondLoad = (*srcLoadPtr++);
				    thirdLoad = (*srcLoadPtr++);
				    byteNumber -= 12;

				    firstAccumulator +=
					((srcLoad >> 8) & MASK_00FF00FF);
				    secondAccumulator +=
					(srcLoad & MASK_00FF00FF);
#ifdef _LITTLE_ENDIAN
				    thirdAccumulator +=
					((secondLoad >> 8) & MASK_00FF00FF);
				    firstAccumulator +=
					(secondLoad & MASK_00FF00FF);
				    secondAccumulator +=
					((thirdLoad >> 8) & MASK_00FF00FF);
				    thirdAccumulator +=
					(thirdLoad & MASK_00FF00FF);
#else /* _LITTLE_ENDIAN */
				    secondAccumulator +=
					((secondLoad >> 8) & MASK_00FF00FF);
				    thirdAccumulator +=
					(secondLoad & MASK_00FF00FF);
				    thirdAccumulator +=
					((thirdLoad >> 8) & MASK_00FF00FF);
				    firstAccumulator +=
					(thirdLoad & MASK_00FF00FF);
#endif /* _LITTLE_ENDIAN */
				    if (byteNumber > 0) {
					srcLoad = (*srcLoadPtr++);
#ifdef _LITTLE_ENDIAN
					srcLoad =
					    srcLoad & ~((mlib_u32)0xffffffff <<
					    (byteNumber << 3));
#else /* _LITTLE_ENDIAN */
					srcLoad =
					    srcLoad & ~((mlib_u32)0xffffffff >>
					    (byteNumber << 3));
#endif /* _LITTLE_ENDIAN */
					firstAccumulator +=
					    ((srcLoad >> 8) & MASK_00FF00FF);
					secondAccumulator +=
					    (srcLoad & MASK_00FF00FF);
				    }
			    } else if (byteNumber >= 8) {
				    secondLoad = (*srcLoadPtr++);
				    byteNumber -= 8;

				    firstAccumulator +=
					((srcLoad >> 8) & MASK_00FF00FF);
				    secondAccumulator +=
					(srcLoad & MASK_00FF00FF);
#ifdef _LITTLE_ENDIAN
				    thirdAccumulator +=
					((secondLoad >> 8) & MASK_00FF00FF);
				    firstAccumulator +=
					(secondLoad & MASK_00FF00FF);
#else /* _LITTLE_ENDIAN */
				    secondAccumulator +=
					((secondLoad >> 8) & MASK_00FF00FF);
				    thirdAccumulator +=
					(secondLoad & MASK_00FF00FF);
#endif /* _LITTLE_ENDIAN */

				    if (byteNumber > 0) {
					thirdLoad = (*srcLoadPtr++);
#ifdef _LITTLE_ENDIAN
					thirdLoad =
					    thirdLoad & ~((mlib_u32)0xffffffff
					    << (byteNumber << 3));
					secondAccumulator +=
					    ((thirdLoad >> 8) & MASK_00FF00FF);
					thirdAccumulator +=
					    (thirdLoad & MASK_00FF00FF);
#else /* _LITTLE_ENDIAN */
					thirdLoad =
					    thirdLoad & ~((mlib_u32)0xffffffff
					    >> (byteNumber << 3));
					thirdAccumulator +=
					    ((thirdLoad >> 8) & MASK_00FF00FF);
					firstAccumulator +=
					    (thirdLoad & MASK_00FF00FF);
#endif /* _LITTLE_ENDIAN */
				    }
			    } else if (byteNumber >= 4) {
				    byteNumber -= 4;

				    firstAccumulator +=
					((srcLoad >> 8) & MASK_00FF00FF);
				    secondAccumulator +=
					(srcLoad & MASK_00FF00FF);

				    if (byteNumber > 0) {
					secondLoad = (*srcLoadPtr++);
#ifdef _LITTLE_ENDIAN
					secondLoad =
					    secondLoad & ~((mlib_u32)0xffffffff
					    << (byteNumber << 3));
					thirdAccumulator +=
					    ((secondLoad >> 8) & MASK_00FF00FF);
					firstAccumulator +=
					    (secondLoad & MASK_00FF00FF);
#else /* _LITTLE_ENDIAN */
					secondLoad =
					    secondLoad & ~((mlib_u32)0xffffffff
					    >> (byteNumber << 3));
					secondAccumulator +=
					    ((secondLoad >> 8) & MASK_00FF00FF);
					thirdAccumulator +=
					    (secondLoad & MASK_00FF00FF);
#endif /* _LITTLE_ENDIAN */
				    }
			    } else if (byteNumber > 0) {
#ifdef _LITTLE_ENDIAN
				    srcLoad =
					srcLoad & ~((mlib_u32)0xffffffff <<
					(byteNumber << 3));
#else /* _LITTLE_ENDIAN */
				    srcLoad =
					srcLoad & ~((mlib_u32)0xffffffff >>
					(byteNumber << 3));
#endif /* _LITTLE_ENDIAN */
				    firstAccumulator +=
					((srcLoad >> 8) & MASK_00FF00FF);
				    secondAccumulator +=
					(srcLoad & MASK_00FF00FF);
			    }

			    sum0 += (secondAccumulator & MASK_0000FFFF) +
				((firstAccumulator) >> 16);

			    sum1 += (thirdAccumulator & MASK_0000FFFF) +
				((secondAccumulator) >> 16);

			    sum2 += (firstAccumulator & MASK_0000FFFF) +
				((thirdAccumulator) >> 16);

			    lmean[al2off[alignSteps]] += sum0;
#ifdef _LITTLE_ENDIAN
			    lmean[al2off[alignSteps + 1]] += sum1;
			    lmean[al2off[alignSteps + 2]] += sum2;
#else /* _LITTLE_ENDIAN */
			    lmean[al2off[alignSteps + 2]] += sum1;
			    lmean[al2off[alignSteps + 1]] += sum2;
#endif /* _LITTLE_ENDIAN */

			    srcLinePtr += srcYStride;
			    srcPixelPtr = srcLinePtr;
		    }

			mean[al2off[alignSteps]] =
					lmean[al2off[alignSteps]] * divider;
			mean[al2off[alignSteps + 1]] =
					lmean[al2off[alignSteps + 1]] * divider;
			mean[al2off[alignSteps + 2]] =
					lmean[al2off[alignSteps + 2]] * divider;
		}

		break;

	case 4:
		lmean[0] = lmean[1] = lmean[2] = lmean[3] = 0;
		for (j = 0; j < srcHeight; j++) {
			alignSteps = (mlib_addr)srcPixelPtr & 3;
			cycleNumber = (srcWidth - 1) >> 8;
			loadsNumber = (srcWidth - 1) & 0xff;

			firstAccumulator = secondAccumulator = 0;
			sum0 = sum1 = sum2 = sum3 = 0;

			srcLoadPtr = (mlib_u32 *)(srcPixelPtr - alignSteps);
#ifdef _LITTLE_ENDIAN
			mask = (mlib_u32)0xffffffff << (alignSteps << 3);
#else /* _LITTLE_ENDIAN */
			mask = (mlib_u32)0xffffffff >> (alignSteps << 3);
#endif /* _LITTLE_ENDIAN */
			srcLoad = mask & (*srcLoadPtr++);
			firstAccumulator += (srcLoad & MASK_00FF00FF);
			secondAccumulator += ((srcLoad >> 8) & MASK_00FF00FF);

			for (i = 0; i < cycleNumber; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (k = 0; k < 64; k++) {
					srcLoad = (*srcLoadPtr++);
					firstAccumulator +=
					    (srcLoad & MASK_00FF00FF);
					secondAccumulator +=
					    ((srcLoad >> 8) & MASK_00FF00FF);

					srcLoad = (*srcLoadPtr++);
					firstAccumulator +=
					    (srcLoad & MASK_00FF00FF);
					secondAccumulator +=
					    ((srcLoad >> 8) & MASK_00FF00FF);

					srcLoad = (*srcLoadPtr++);
					firstAccumulator +=
					    (srcLoad & MASK_00FF00FF);
					secondAccumulator +=
					    ((srcLoad >> 8) & MASK_00FF00FF);

					srcLoad = (*srcLoadPtr++);
					firstAccumulator +=
					    (srcLoad & MASK_00FF00FF);
					secondAccumulator +=
					    ((srcLoad >> 8) & MASK_00FF00FF);
				}

				sum0 += ((secondAccumulator) >> 16);
				sum1 += ((firstAccumulator) >> 16);
				sum2 += (secondAccumulator & MASK_0000FFFF);
				sum3 += (firstAccumulator & MASK_0000FFFF);

				firstAccumulator = secondAccumulator = 0;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < loadsNumber; i++) {
				srcLoad = (*srcLoadPtr++);
				firstAccumulator += (srcLoad & MASK_00FF00FF);
				secondAccumulator +=
				    ((srcLoad >> 8) & MASK_00FF00FF);

			}

			if (alignSteps) {
				srcLoad = (*srcLoadPtr++);
				srcLoad = srcLoad & ~mask;
				firstAccumulator += (srcLoad & MASK_00FF00FF);
				secondAccumulator += ((srcLoad >> 8) &
				    MASK_00FF00FF);
			}

			sum0 += ((secondAccumulator) >> 16);
			sum1 += ((firstAccumulator) >> 16);
			sum2 += (secondAccumulator & MASK_0000FFFF);
			sum3 += (firstAccumulator & MASK_0000FFFF);

#ifdef _LITTLE_ENDIAN
			lmean[3 & (0 - alignSteps)] += sum3;
			lmean[3 & (1 - alignSteps)] += sum2;
			lmean[3 & (2 - alignSteps)] += sum1;
			lmean[3 & (3 - alignSteps)] += sum0;
#else /* _LITTLE_ENDIAN */
			lmean[3 & (0 - alignSteps)] += sum0;
			lmean[3 & (1 - alignSteps)] += sum1;
			lmean[3 & (2 - alignSteps)] += sum2;
			lmean[3 & (3 - alignSteps)] += sum3;
#endif /* _LITTLE_ENDIAN */

			srcLinePtr += srcYStride;
			srcPixelPtr = srcLinePtr;
		}

		mean[3 & (0 - alignSteps)] =
			lmean[3 & (0 - alignSteps)] * divider;
		mean[3 & (1 - alignSteps)] =
			lmean[3 & (1 - alignSteps)] * divider;
		mean[3 & (2 - alignSteps)] =
			lmean[3 & (2 - alignSteps)] * divider;
		mean[3 & (3 - alignSteps)] =
			lmean[3 & (3 - alignSteps)] * divider;

		break;
	}
}

/* *********************************************************** */

void
mlib_c_ImageMeanU16Case(
    const mlib_image *src,
    mlib_d64 *mean)
{
/* pointer for pixel of source */
	mlib_u16 *srcPixelPtr = (mlib_u16 *)mlib_ImageGetData(src);

/* pointer for line of source */
	mlib_u8 *srcLinePtr = (mlib_u8 *)srcPixelPtr;

/* width of source image */
	mlib_s32 srcWidth = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 srcHeight = mlib_ImageGetHeight(src);

/* total number of pixels */
	mlib_s32 srcTotalPixNum = srcHeight * srcWidth;

/* linestride = bytes to next row */
	mlib_s32 srcYStride = mlib_ImageGetStride(src);

/* number of channels */
	mlib_s32 numOfChannels = mlib_ImageGetChannels(src);
	mlib_s64 sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0;
	mlib_d64 divider = 1.0 / srcTotalPixNum;

/* indices */
	mlib_s32 i, k, j;
	mlib_u32 pixelNumber;
	mlib_u32 cycleNumber;

	if (2 * numOfChannels * srcWidth == srcYStride) {
		srcWidth = srcTotalPixNum;
		srcHeight = 1;
	}

/* pixel number is equal to number of pixels in a row */
	pixelNumber = (mlib_u32)srcWidth;
/* number of 2^15 cycles */
	cycleNumber = pixelNumber >> 15;
	pixelNumber &= 0x7fff;

	switch (numOfChannels) {

	case 1: {

		    mlib_s32 firstAccumulator = 0;

		    for (j = 0; j < srcHeight; j++) {
			    for (k = 0; k < (mlib_s32)cycleNumber; k++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				    for (i = 0; i < 16384; i++) {
					    firstAccumulator +=
						(*srcPixelPtr++);
					    firstAccumulator +=
						(*srcPixelPtr++);
				    }

				    sum0 += firstAccumulator;
				    firstAccumulator = 0;
			    }

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (i = 0; i < (mlib_s32)(pixelNumber >> 1); i++) {
				    firstAccumulator += (*srcPixelPtr++);
				    firstAccumulator += (*srcPixelPtr++);
			    }

			    if (pixelNumber & 1)
				    firstAccumulator += (*srcPixelPtr++);

			    sum0 += firstAccumulator;
			    firstAccumulator = 0;

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_u16 *)srcLinePtr;
		    }

		    mean[0] = sum0 * divider;
	    }

		break;

	case 2: {

		    mlib_s32 firstAccumulator = 0;
		    mlib_s32 secondAccumulator = 0;

		    for (j = 0; j < srcHeight; j++) {
			    for (k = 0; k < (mlib_s32)cycleNumber; k++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				    for (i = 0; i < 16384; i++) {
					    firstAccumulator +=
						(*srcPixelPtr++);
					    secondAccumulator +=
						(*srcPixelPtr++);
					    firstAccumulator +=
						(*srcPixelPtr++);
					    secondAccumulator +=
						(*srcPixelPtr++);
				    }

				    sum0 += firstAccumulator;
				    sum1 += secondAccumulator;
				    firstAccumulator = secondAccumulator = 0;
			    }

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (i = 0; i < (mlib_s32)pixelNumber; i++) {
				    firstAccumulator += (*srcPixelPtr++);
				    secondAccumulator += (*srcPixelPtr++);
			    }

			    sum0 += firstAccumulator;
			    sum1 += secondAccumulator;
			    firstAccumulator = secondAccumulator = 0;

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_u16 *)srcLinePtr;
		    }

		    mean[0] = sum0 * divider;
		    mean[1] = sum1 * divider;
	    }

		break;

	case 3: {

		    mlib_s32 firstAccumulator = 0;
		    mlib_s32 secondAccumulator = 0;
		    mlib_s32 thirdAccumulator = 0;

		    for (j = 0; j < srcHeight; j++) {
			    for (k = 0; k < (mlib_s32)cycleNumber; k++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				    for (i = 0; i < 16384; i++) {
					    firstAccumulator +=
						(*srcPixelPtr++);
					    secondAccumulator +=
						(*srcPixelPtr++);
					    thirdAccumulator +=
						(*srcPixelPtr++);
					    firstAccumulator +=
						(*srcPixelPtr++);
					    secondAccumulator +=
						(*srcPixelPtr++);
					    thirdAccumulator +=
						(*srcPixelPtr++);
				    }

				    sum0 += firstAccumulator;
				    sum1 += secondAccumulator;
				    sum2 += thirdAccumulator;
				    firstAccumulator = secondAccumulator =
					thirdAccumulator = 0;
			    }

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (i = 0; i < (mlib_s32)pixelNumber - 1; i += 2) {
				    firstAccumulator += (*srcPixelPtr++);
				    secondAccumulator += (*srcPixelPtr++);
				    thirdAccumulator += (*srcPixelPtr++);
				    firstAccumulator += (*srcPixelPtr++);
				    secondAccumulator += (*srcPixelPtr++);
				    thirdAccumulator += (*srcPixelPtr++);
			    }

			    if (i < (mlib_s32)pixelNumber) {
				    firstAccumulator += (*srcPixelPtr++);
				    secondAccumulator += (*srcPixelPtr++);
				    thirdAccumulator += (*srcPixelPtr++);
			    }

			    sum0 += firstAccumulator;
			    sum1 += secondAccumulator;
			    sum2 += thirdAccumulator;
			    firstAccumulator = secondAccumulator =
				thirdAccumulator = 0;

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_u16 *)srcLinePtr;
		    }

		    mean[0] = sum0 * divider;
		    mean[1] = sum1 * divider;
		    mean[2] = sum2 * divider;
	    }

		break;

	case 4: {

		    mlib_s32 firstAccumulator = 0;
		    mlib_s32 secondAccumulator = 0;
		    mlib_s32 thirdAccumulator = 0;
		    mlib_s32 forthAccumulator = 0;

		    for (j = 0; j < srcHeight; j++) {
			    for (k = 0; k < (mlib_s32)cycleNumber; k++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				    for (i = 0; i < 32768; i++) {
					    firstAccumulator +=
						(*srcPixelPtr++);
					    secondAccumulator +=
						(*srcPixelPtr++);
					    thirdAccumulator +=
						(*srcPixelPtr++);
					    forthAccumulator +=
						(*srcPixelPtr++);
				    }

				    sum0 += firstAccumulator;
				    sum1 += secondAccumulator;
				    sum2 += thirdAccumulator;
				    sum3 += forthAccumulator;
				    firstAccumulator = secondAccumulator =
					thirdAccumulator = forthAccumulator = 0;
			    }

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (i = 0; i < (mlib_s32)pixelNumber; i++) {
				    firstAccumulator += (*srcPixelPtr++);
				    secondAccumulator += (*srcPixelPtr++);
				    thirdAccumulator += (*srcPixelPtr++);
				    forthAccumulator += (*srcPixelPtr++);
			    }

			    sum0 += firstAccumulator;
			    sum1 += secondAccumulator;
			    sum2 += thirdAccumulator;
			    sum3 += forthAccumulator;
			    firstAccumulator = secondAccumulator =
				thirdAccumulator = forthAccumulator = 0;

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_u16 *)srcLinePtr;
		    }

		    mean[0] = sum0 * divider;
		    mean[1] = sum1 * divider;
		    mean[2] = sum2 * divider;
		    mean[3] = sum3 * divider;
	    }

		break;
	}
}

/* *********************************************************** */

void
mlib_c_ImageMeanS16Case(
    const mlib_image *src,
    mlib_d64 *mean)
{
/* pointer for pixel of source */
	mlib_s16 *srcPixelPtr = (mlib_s16 *)mlib_ImageGetData(src);

/* pointer for line of source */
	mlib_u8 *srcLinePtr = (mlib_u8 *)srcPixelPtr;

/* width of source image */
	mlib_s32 srcWidth = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 srcHeight = mlib_ImageGetHeight(src);

/* total number of pixels */
	mlib_s32 srcTotalPixNum = srcHeight * srcWidth;

/* linestride = bytes to next row */
	mlib_s32 srcYStride = mlib_ImageGetStride(src);

/* number of channels */
	mlib_s32 numOfChannels = mlib_ImageGetChannels(src);
	mlib_s64 sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0;
	mlib_d64 divider = 1.0 / srcTotalPixNum;

/* indices */
	mlib_s32 i, k, j;
	mlib_u32 pixelNumber;
	mlib_u32 cycleNumber;

	if (2 * numOfChannels * srcWidth == srcYStride) {
		srcWidth = srcTotalPixNum;
		srcHeight = 1;
	}

/* pixel number is equal to number of pixels in a row */
	pixelNumber = (mlib_u32)srcWidth;
/* number of 2^15 cycles */
	cycleNumber = pixelNumber >> 15;
	pixelNumber &= 0x7fff;

	switch (numOfChannels) {

	case 1: {

		    mlib_s32 firstAccumulator = 0;

		    for (j = 0; j < srcHeight; j++) {
			    for (k = 0; k < (mlib_s32)cycleNumber; k++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				    for (i = 0; i < 32768; i++) {
					    firstAccumulator +=
						0x8000 ^ *(mlib_u16
						*)(srcPixelPtr++);
				    }

				    sum0 += firstAccumulator - 0x40000000;
				    firstAccumulator = 0;
			    }

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (i = 0; i < (mlib_s32)(pixelNumber >> 1); i++) {
				    firstAccumulator +=
					0x8000 ^ *(mlib_u16 *)(srcPixelPtr++);
				    firstAccumulator +=
					0x8000 ^ *(mlib_u16 *)(srcPixelPtr++);
			    }

			    if (pixelNumber & 1)
				    firstAccumulator +=
					0x8000 ^ *(mlib_u16 *)(srcPixelPtr++);

			    sum0 +=
				(firstAccumulator -
				((mlib_s32)pixelNumber << 15));
			    firstAccumulator = 0;

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_s16 *)srcLinePtr;
		    }

		    mean[0] = sum0 * divider;
	    }

		break;

	case 2: {

		    mlib_s32 firstAccumulator = 0;
		    mlib_s32 secondAccumulator = 0;

		    for (j = 0; j < srcHeight; j++) {
			    for (k = 0; k < (mlib_s32)cycleNumber; k++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				    for (i = 0; i < 32768; i++) {
					    firstAccumulator +=
						0x8000 ^ *(mlib_u16
						*)(srcPixelPtr++);
					    secondAccumulator +=
						0x8000 ^ *(mlib_u16
						*)(srcPixelPtr++);
				    }

				    sum0 += firstAccumulator - 0x40000000;
				    sum1 += secondAccumulator - 0x40000000;
				    firstAccumulator = secondAccumulator = 0;
			    }

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (i = 0; i < (mlib_s32)pixelNumber; i++) {
				    firstAccumulator +=
					0x8000 ^ *(mlib_u16 *)(srcPixelPtr++);
				    secondAccumulator +=
					0x8000 ^ *(mlib_u16 *)(srcPixelPtr++);
			    }

			    sum0 +=
				firstAccumulator -
				((mlib_s32)pixelNumber << 15);
			    sum1 +=
				secondAccumulator -
				((mlib_s32)pixelNumber << 15);
			    firstAccumulator = secondAccumulator = 0;

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_s16 *)srcLinePtr;
		    }

		    mean[0] = sum0 * divider;
		    mean[1] = sum1 * divider;
	    }

		break;

	case 3: {

		    mlib_s32 firstAccumulator = 0;
		    mlib_s32 secondAccumulator = 0;
		    mlib_s32 thirdAccumulator = 0;

		    for (j = 0; j < srcHeight; j++) {
			    for (k = 0; k < (mlib_s32)cycleNumber; k++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				    for (i = 0; i < 32768; i++) {
					    firstAccumulator +=
						0x8000 ^ *(mlib_u16
						*)(srcPixelPtr++);
					    secondAccumulator +=
						*(srcPixelPtr++);
					    thirdAccumulator +=
						*(srcPixelPtr++);
				    }

				    sum0 += firstAccumulator - 0x40000000;
				    sum1 += secondAccumulator;
				    sum2 += thirdAccumulator;
				    firstAccumulator = secondAccumulator =
					thirdAccumulator = 0;
			    }

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (i = 0; i < (mlib_s32)pixelNumber; i++) {
				    firstAccumulator +=
					0x8000 ^ *(mlib_u16 *)(srcPixelPtr++);
				    secondAccumulator += *(srcPixelPtr++);
				    thirdAccumulator += *(srcPixelPtr++);
			    }

			    sum0 +=
				firstAccumulator -
				((mlib_s32)pixelNumber << 15);
			    sum1 += secondAccumulator;
			    sum2 += thirdAccumulator;
			    firstAccumulator = secondAccumulator =
				thirdAccumulator = 0;

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_s16 *)srcLinePtr;
		    }

		    mean[0] = sum0 * divider;
		    mean[1] = sum1 * divider;
		    mean[2] = sum2 * divider;
	    }

		break;

	case 4: {

		    mlib_s32 firstAccumulator = 0;
		    mlib_s32 secondAccumulator = 0;
		    mlib_s32 thirdAccumulator = 0;
		    mlib_s32 forthAccumulator = 0;

		    for (j = 0; j < srcHeight; j++) {
			    for (k = 0; k < (mlib_s32)cycleNumber; k++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				    for (i = 0; i < 32768; i++) {
					    firstAccumulator +=
						0x8000 ^ *(mlib_u16
						*)(srcPixelPtr++);
					    secondAccumulator +=
						0x8000 ^ *(mlib_u16
						*)(srcPixelPtr++);
					    thirdAccumulator +=
						*(srcPixelPtr++);
					    forthAccumulator +=
						*(srcPixelPtr++);
				    }

				    sum0 += firstAccumulator - 0x40000000;
				    sum1 += secondAccumulator - 0x40000000;
				    sum2 += thirdAccumulator;
				    sum3 += forthAccumulator;
				    firstAccumulator = secondAccumulator =
					thirdAccumulator = forthAccumulator = 0;
			    }

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (i = 0; i < (mlib_s32)pixelNumber; i++) {
				    firstAccumulator +=
					0x8000 ^ *(mlib_u16 *)(srcPixelPtr++);
				    secondAccumulator +=
					0x8000 ^ *(mlib_u16 *)(srcPixelPtr++);
				    thirdAccumulator += *(srcPixelPtr++);
				    forthAccumulator += *(srcPixelPtr++);
			    }

			    sum0 +=
				firstAccumulator -
				((mlib_s32)pixelNumber << 15);
			    sum1 +=
				secondAccumulator -
				((mlib_s32)pixelNumber << 15);
			    sum2 += thirdAccumulator;
			    sum3 += forthAccumulator;
			    firstAccumulator = secondAccumulator =
				thirdAccumulator = forthAccumulator = 0;

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_s16 *)srcLinePtr;
		    }

		    mean[0] = sum0 * divider;
		    mean[1] = sum1 * divider;
		    mean[2] = sum2 * divider;
		    mean[3] = sum3 * divider;
	    }

		break;
	}
}

/* *********************************************************** */

void
mlib_c_ImageMeanS32Case(
    const mlib_image *src,
    mlib_d64 *mean)
{
/* width of source image */
	mlib_s32 srcWidth = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 srcHeight = mlib_ImageGetHeight(src);

/* linestride = bytes to next row */
	mlib_s32 srcYStride = mlib_ImageGetStride(src);

/* number of channels */
	mlib_s32 numOfChannels = mlib_ImageGetChannels(src);

/* pointer for line of source */
	mlib_u8 *srcLinePtr = (mlib_u8 *)mlib_ImageGetData(src);

/* pointer for pixel of source */
	mlib_s32 *srcPixelPtr = (mlib_s32 *)srcLinePtr;

/* total number of pixels */
	mlib_s32 srcTotalPixNum = srcHeight * srcWidth;
	mlib_d64 divider = 1.0 / srcTotalPixNum;

/* indices */
	mlib_s32 k, j;

	if (4 * numOfChannels * srcWidth == srcYStride) {
		srcWidth = srcTotalPixNum;
		srcHeight = 1;
	}
	switch (numOfChannels) {

	case 1: {

			mlib_s64 firstAccumulator = 0;
			mlib_s32 i, cycleNumber = srcWidth >> 16;
		    mlib_s32 val;
			mlib_s64 hi, lo;

		    srcWidth &= 0xffff;

		    for (j = 0; j < srcHeight; j++) {
			    hi = lo = 0;
			    for (i = 0; i < cycleNumber; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				    for (k = 0; k < 0x8000; k++) {
					    firstAccumulator +=
						(*srcPixelPtr++);
					    val = (*srcPixelPtr++);
					    hi += val >> 16;
					    lo += val & 0xffff;
					}

					firstAccumulator += lo + (hi << 16);
				    hi = lo = 0;
				}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (k = 0; k < (srcWidth >> 1); k++) {
				    firstAccumulator += (*srcPixelPtr++);
				    val = (*srcPixelPtr++);
				    hi += val >> 16;
				    lo += val & 0xffff;
			    }

			    if (srcWidth & 1)
				    firstAccumulator += *srcPixelPtr;
				firstAccumulator += lo + (hi << 16);
			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_s32 *)srcLinePtr;
		    }

		    mean[0] = firstAccumulator * divider;
	    }

		break;

	case 2: {
			mlib_s64 firstAccumulator = 0;
			mlib_s64 secondAccumulator = 0;

		    mlib_s32 i, cycleNumber = srcWidth >> 15;
		    mlib_s32 val;
			mlib_s64 hi, lo;

		    srcWidth &= 0x7fff;

		    for (j = 0; j < srcHeight; j++) {
			    hi = lo = 0;
			    for (i = 0; i < cycleNumber; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				    for (k = 0; k < 0x8000; k++) {
					    firstAccumulator +=
						(*srcPixelPtr++);
					    val = (*srcPixelPtr++);
					    hi += val >> 16;
					    lo += val & 0xffff;
				    }

					secondAccumulator += lo + (hi << 16);
				    hi = lo = 0;
			    }

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (k = 0; k < srcWidth; k++) {
				    firstAccumulator += (*srcPixelPtr++);
				    val = (*srcPixelPtr++);
				    hi += val >> 16;
				    lo += val & 0xffff;
			    }

				secondAccumulator += lo + (hi << 16);
			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_s32 *)srcLinePtr;
		    }

		    mean[0] = firstAccumulator * divider;
		    mean[1] = secondAccumulator * divider;
	    }

		break;

	case 3: {

			mlib_s64 firstAccumulator = 0;
			mlib_s64 secondAccumulator = 0;
			mlib_s64 thirdAccumulator = 0;

		    mlib_s32 i, cycleNumber = srcWidth >> 15;
		    mlib_s32 val;
			mlib_s64 hi, lo;

		    srcWidth &= 0x7fff;

		    for (j = 0; j < srcHeight; j++) {
			    hi = lo = 0;
			    for (i = 0; i < cycleNumber; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				    for (k = 0; k < 0x8000; k++) {
					    firstAccumulator +=
						(*srcPixelPtr++);
					    secondAccumulator +=
						(*srcPixelPtr++);
					    val = (*srcPixelPtr++);
					    hi += val >> 16;
					    lo += val & 0xffff;
				    }

					thirdAccumulator += lo + (hi << 16);

				    hi = lo = 0;
			    }

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (k = 0; k < srcWidth; k++) {
				    firstAccumulator += (*srcPixelPtr++);
				    secondAccumulator += (*srcPixelPtr++);
				    val = (*srcPixelPtr++);
				    hi += val >> 16;
				    lo += val & 0xffff;
			    }

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_s32 *)srcLinePtr;
				thirdAccumulator += lo + (hi << 16);
			}

		    mean[0] = firstAccumulator * divider;
		    mean[1] = secondAccumulator * divider;
		    mean[2] = thirdAccumulator * divider;
	    }

		break;

	case 4: {

			mlib_s64 firstAccumulator = 0;
			mlib_s64 secondAccumulator = 0;
			mlib_s64 thirdAccumulator = 0;
			mlib_s64 forthAccumulator = 0;

			mlib_s32 i, cycleNumber = srcWidth >> 15;
		    mlib_s32 val;
			mlib_s64 hi, lo;
		    mlib_s32 val2;
			mlib_s64 hi2, lo2;

		    srcWidth &= 0x7fff;

		    for (j = 0; j < srcHeight; j++) {
			    hi = lo = hi2 = lo2 = 0;
			    for (i = 0; i < cycleNumber; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				    for (k = 0; k < 0x8000; k++) {
					    firstAccumulator +=
						(*srcPixelPtr++);
					    secondAccumulator +=
						(*srcPixelPtr++);
					    val2 = (*srcPixelPtr++);
					    hi2 += val2 >> 16;
					    lo2 += val2 & 0xffff;
					    val = (*srcPixelPtr++);
					    hi += val >> 16;
					    lo += val & 0xffff;
				    }

					thirdAccumulator += lo2 + (hi2 << 16);
					forthAccumulator += lo + (hi << 16);

				    hi = lo = hi2 = lo2 = 0;
			    }

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (k = 0; k < srcWidth; k++) {
				    firstAccumulator += (*srcPixelPtr++);
				    secondAccumulator += (*srcPixelPtr++);
				    val2 = (*srcPixelPtr++);
				    hi2 += val2 >> 16;
				    lo2 += val2 & 0xffff;
				    val = (*srcPixelPtr++);
				    hi += val >> 16;
				    lo += val & 0xffff;
			    }

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_s32 *)srcLinePtr;
				thirdAccumulator += lo2 + (hi2 << 16);
				forthAccumulator += lo + (hi << 16);
			}

		    mean[0] = firstAccumulator * divider;
		    mean[1] = secondAccumulator * divider;
		    mean[2] = thirdAccumulator * divider;
		    mean[3] = forthAccumulator * divider;
	    }

		break;
	}
}

/* *********************************************************** */
#else /* _NO_LONGLONG */
/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMean = __mlib_ImageMean

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMean) mlib_ImageMean
    __attribute__((weak, alias("__mlib_ImageMean")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MASK_00FF00FF	0x00FF00FF
#define	MASK_0000FFFF	0x0000FFFF

/* *********************************************************** */

static void mlib_c_ImageMeanU8Case(
    const mlib_image *src,
    mlib_d64 *mean);
static void mlib_c_ImageMeanS16Case(
    const mlib_image *src,
    mlib_d64 *mean);
static void mlib_c_ImageMeanU16Case(
    const mlib_image *src,
    mlib_d64 *mean);
static void mlib_c_ImageMeanS32Case(
    const mlib_image *src,
    mlib_d64 *mean);

/* *********************************************************** */

mlib_status
__mlib_ImageMean(
    mlib_d64 *mean,
    const mlib_image *src)
{
/* check for obvious errors */
	MLIB_IMAGE_CHECK(src);

	if (mean == NULL)
		return (MLIB_NULLPOINTER);

	switch (mlib_ImageGetType(src)) {

/* handle MLIB_BYTE data type of image */
	case MLIB_BYTE:
		mlib_c_ImageMeanU8Case(src, mean);
		return (MLIB_SUCCESS);

/* handle MLIB_SHORT data type of image */
	case MLIB_SHORT:
		mlib_c_ImageMeanS16Case(src, mean);
		return (MLIB_SUCCESS);

	case MLIB_USHORT:
		mlib_c_ImageMeanU16Case(src, mean);
		return (MLIB_SUCCESS);

/* handle MLIB_INT data type of image */
	case MLIB_INT:
		mlib_c_ImageMeanS32Case(src, mean);
		return (MLIB_SUCCESS);

/* discard any other data types */
	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

void
mlib_c_ImageMeanU8Case(
    const mlib_image *src,
    mlib_d64 *mean)
{
/* pointer for pixel of source */
	mlib_u8 *srcPixelPtr = (mlib_u8 *)mlib_ImageGetData(src);
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
	mlib_d64 sum0 = 0.0, sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;

/* compute mean3_off(align, sumn) */
	const mlib_u8 al2off[6] = { 0, 2, 1, 0, 2, 1 };
/* indices */
	mlib_s32 i, k, j;
	mlib_s32 alignSteps;
	mlib_s32 byteNumber;
	mlib_s32 loadsNumber;
	mlib_s32 cycleNumber;
	mlib_u32 firstAccumulator;
	mlib_u32 secondAccumulator;
	mlib_u32 srcLoad;
	mlib_u32 *srcLoadPtr;
	mlib_u32 mask;

	if (numOfChannels * srcWidth == srcYStride) {
		srcWidth = srcTotalPixNum;
		srcHeight = 1;
	}

	if (srcWidth < 4) {
		for (k = 0; k < numOfChannels; k++) {
			sum0 = 0.;
			for (i = 0; i < srcHeight; i++) {
#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
				for (j = 0; j < srcWidth; j++) {
					sum0 += (mlib_d64) srcPixelPtr[
					    i*srcYStride + j*numOfChannels + k];
				}
			}
			mean[k] = sum0 * divider;

		}
		return;
	}

	switch (numOfChannels) {

	case 1:
		mean[0] = 0.0;
		for (j = 0; j < srcHeight; j++) {
			alignSteps = (mlib_addr)srcPixelPtr & 3;
			byteNumber = srcWidth + alignSteps - 4;
			loadsNumber = byteNumber >> 2;
			cycleNumber = loadsNumber >> 8;
			byteNumber &= 3;
			loadsNumber &= 0xff;

			firstAccumulator = secondAccumulator = 0;
			srcLoadPtr = (mlib_u32 *)(srcPixelPtr - alignSteps);
#ifdef _LITTLE_ENDIAN
			mask = (mlib_u32)0xffffffff << (alignSteps << 3);
#else /* _LITTLE_ENDIAN */
			mask = (mlib_u32)0xffffffff >> (alignSteps << 3);
#endif /* _LITTLE_ENDIAN */
			srcLoad = mask & (*srcLoadPtr++);
			firstAccumulator += (srcLoad & MASK_00FF00FF);
			secondAccumulator += ((srcLoad >> 8) & MASK_00FF00FF);

			for (i = 0; i < cycleNumber; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (k = 0; k < 64; k++) {
					srcLoad = (*srcLoadPtr++);
					firstAccumulator +=
					    (srcLoad & MASK_00FF00FF);
					secondAccumulator +=
					    ((srcLoad >> 8) & MASK_00FF00FF);

					srcLoad = (*srcLoadPtr++);
					firstAccumulator +=
					    (srcLoad & MASK_00FF00FF);
					secondAccumulator +=
					    ((srcLoad >> 8) & MASK_00FF00FF);

					srcLoad = (*srcLoadPtr++);
					firstAccumulator +=
					    (srcLoad & MASK_00FF00FF);
					secondAccumulator +=
					    ((srcLoad >> 8) & MASK_00FF00FF);

					srcLoad = (*srcLoadPtr++);
					firstAccumulator +=
					    (srcLoad & MASK_00FF00FF);
					secondAccumulator +=
					    ((srcLoad >> 8) & MASK_00FF00FF);
				}

				sum0 +=
				    (secondAccumulator & MASK_0000FFFF) +
				    ((secondAccumulator) >> 16) +
				    (firstAccumulator & MASK_0000FFFF) +
				    ((firstAccumulator) >> 16);

				firstAccumulator = secondAccumulator = 0;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < loadsNumber; i++) {
				srcLoad = (*srcLoadPtr++);
				firstAccumulator += (srcLoad & MASK_00FF00FF);
				secondAccumulator +=
				    ((srcLoad >> 8) & MASK_00FF00FF);
			}

			if (byteNumber) {
				srcLoad = (*srcLoadPtr++);
#ifdef _LITTLE_ENDIAN
				srcLoad = srcLoad & ~((mlib_u32)0xffffffff <<
					(byteNumber << 3));
#else /* _LITTLE_ENDIAN */
				srcLoad = srcLoad & ~((mlib_u32)0xffffffff >>
					(byteNumber << 3));
#endif /* _LITTLE_ENDIAN */
				firstAccumulator += (srcLoad & MASK_00FF00FF);
				secondAccumulator += ((srcLoad >> 8) &
					MASK_00FF00FF);
			}

			sum0 +=
			    (secondAccumulator & MASK_0000FFFF) +
			    ((secondAccumulator) >> 16) +
			    (firstAccumulator & MASK_0000FFFF) +
			    ((firstAccumulator) >> 16);

			firstAccumulator = secondAccumulator = 0;

			srcLinePtr += srcYStride;
			srcPixelPtr = srcLinePtr;
		}

		mean[0] = sum0 * divider;
		break;

	case 2:
		mean[0] = mean[1] = 0.0;
		for (j = 0; j < srcHeight; j++) {
			alignSteps = (mlib_addr)srcPixelPtr & 3;
			byteNumber = srcWidth * 2 + alignSteps - 4;
			loadsNumber = byteNumber >> 2;
			cycleNumber = loadsNumber >> 8;
			byteNumber &= 3;
			loadsNumber &= 0xff;

			firstAccumulator = secondAccumulator = 0;
			srcLoadPtr = (mlib_u32 *)(srcPixelPtr - alignSteps);
#ifdef _LITTLE_ENDIAN
			mask = (mlib_u32)0xffffffff << (alignSteps << 3);
#else /* _LITTLE_ENDIAN */
			mask = (mlib_u32)0xffffffff >> (alignSteps << 3);
#endif /* _LITTLE_ENDIAN */
			srcLoad = mask & (*srcLoadPtr++);
			firstAccumulator += (srcLoad & MASK_00FF00FF);
			secondAccumulator += ((srcLoad >> 8) & MASK_00FF00FF);

			for (i = 0; i < cycleNumber; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (k = 0; k < 64; k++) {
					srcLoad = (*srcLoadPtr++);
					firstAccumulator +=
					    (srcLoad & MASK_00FF00FF);
					secondAccumulator +=
					    ((srcLoad >> 8) & MASK_00FF00FF);

					srcLoad = (*srcLoadPtr++);
					firstAccumulator +=
					    (srcLoad & MASK_00FF00FF);
					secondAccumulator +=
					    ((srcLoad >> 8) & MASK_00FF00FF);

					srcLoad = (*srcLoadPtr++);
					firstAccumulator +=
					    (srcLoad & MASK_00FF00FF);
					secondAccumulator +=
					    ((srcLoad >> 8) & MASK_00FF00FF);

					srcLoad = (*srcLoadPtr++);
					firstAccumulator +=
					    (srcLoad & MASK_00FF00FF);
					secondAccumulator +=
					    ((srcLoad >> 8) & MASK_00FF00FF);
				}

				sum0 +=
				    (secondAccumulator & MASK_0000FFFF) +
				    ((secondAccumulator) >> 16);

				sum1 +=
				    (firstAccumulator & MASK_0000FFFF) +
				    ((firstAccumulator) >> 16);

				firstAccumulator = secondAccumulator = 0;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < loadsNumber; i++) {
				srcLoad = (*srcLoadPtr++);
				firstAccumulator += (srcLoad & MASK_00FF00FF);
				secondAccumulator +=
				    ((srcLoad >> 8) & MASK_00FF00FF);
			}

			if (byteNumber) {

				srcLoad = (*srcLoadPtr++);
#ifdef _LITTLE_ENDIAN
				srcLoad = srcLoad & ~((mlib_u32)0xffffffff <<
					(byteNumber << 3));
#else /* _LITTLE_ENDIAN */
				srcLoad = srcLoad & ~((mlib_u32)0xffffffff >>
					(byteNumber << 3));
#endif /* _LITTLE_ENDIAN */
				firstAccumulator += (srcLoad & MASK_00FF00FF);
				secondAccumulator += ((srcLoad >> 8) &
					MASK_00FF00FF);
			}

			sum0 +=
			    (secondAccumulator & MASK_0000FFFF) +
			    ((secondAccumulator) >> 16);

			sum1 +=
			    (firstAccumulator & MASK_0000FFFF) +
			    ((firstAccumulator) >> 16);

#ifdef _LITTLE_ENDIAN
			mean[alignSteps & 1] += sum1 * divider;
			mean[(alignSteps & 1) ^ 1] += sum0 * divider;
#else /* _LITTLE_ENDIAN */
			mean[alignSteps & 1] += sum0 * divider;
			mean[(alignSteps & 1) ^ 1] += sum1 * divider;
#endif /* _LITTLE_ENDIAN */

			firstAccumulator = secondAccumulator = 0;
			sum0 = sum1 = 0.0;

			srcLinePtr += srcYStride;
			srcPixelPtr = srcLinePtr;
		}

		break;

	case 3: {
/* It's the worst case, we'll need some extra variables here */
		    mlib_u32 secondLoad, thirdLoad;
		    mlib_u32 thirdAccumulator;

		    mean[0] = mean[1] = mean[2] = 0.;

		    for (j = 0; j < srcHeight; j++) {
			    alignSteps = (mlib_addr)srcPixelPtr & 3;
			    byteNumber = srcWidth * 3 + alignSteps;

			    firstAccumulator = secondAccumulator =
				thirdAccumulator = 0;
			    sum0 = sum1 = sum2 = 0.0;

			    srcLoadPtr = (mlib_u32 *)(srcPixelPtr - alignSteps);
#ifdef _LITTLE_ENDIAN
			    mask = (mlib_u32)0xffffffff << (alignSteps << 3);
#else /* _LITTLE_ENDIAN */
			    mask = (mlib_u32)0xffffffff >> (alignSteps << 3);
#endif /* _LITTLE_ENDIAN */
			    srcLoad = mask & (*srcLoadPtr++);

			    for (i = 0; i <= byteNumber - 12 * 128 - 4;
				i += 12 * 128) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(1)
#endif /* __SUNPRO_C */
				    for (k = 0; k < 64; k++) {
					    secondLoad = (*srcLoadPtr++);
					    thirdLoad = (*srcLoadPtr++);

					    firstAccumulator +=
						((srcLoad >> 8) &
						MASK_00FF00FF);
					    secondAccumulator +=
						(srcLoad & MASK_00FF00FF);
#ifdef _LITTLE_ENDIAN
					    thirdAccumulator +=
						((secondLoad >> 8) &
						MASK_00FF00FF);
					    firstAccumulator +=
						(secondLoad & MASK_00FF00FF);
					    secondAccumulator +=
						((thirdLoad >> 8) &
						MASK_00FF00FF);
					    thirdAccumulator +=
						(thirdLoad & MASK_00FF00FF);
#else /* _LITTLE_ENDIAN */
					    secondAccumulator +=
						((secondLoad >> 8) &
						MASK_00FF00FF);
					    thirdAccumulator +=
						(secondLoad & MASK_00FF00FF);
					    thirdAccumulator +=
						((thirdLoad >> 8) &
						MASK_00FF00FF);
					    firstAccumulator +=
						(thirdLoad & MASK_00FF00FF);
#endif /* _LITTLE_ENDIAN */

					    srcLoad = (*srcLoadPtr++);
					    secondLoad = (*srcLoadPtr++);
					    thirdLoad = (*srcLoadPtr++);

					    firstAccumulator +=
						((srcLoad >> 8) &
						MASK_00FF00FF);
					    secondAccumulator +=
						(srcLoad & MASK_00FF00FF);
#ifdef _LITTLE_ENDIAN
					    thirdAccumulator +=
						((secondLoad >> 8) &
						MASK_00FF00FF);
					    firstAccumulator +=
						(secondLoad & MASK_00FF00FF);
					    secondAccumulator +=
						((thirdLoad >> 8) &
						MASK_00FF00FF);
					    thirdAccumulator +=
						(thirdLoad & MASK_00FF00FF);
#else /* _LITTLE_ENDIAN */
					    secondAccumulator +=
						((secondLoad >> 8) &
						MASK_00FF00FF);
					    thirdAccumulator +=
						(secondLoad & MASK_00FF00FF);
					    thirdAccumulator +=
						((thirdLoad >> 8) &
						MASK_00FF00FF);
					    firstAccumulator +=
						(thirdLoad & MASK_00FF00FF);
#endif /* _LITTLE_ENDIAN */
					    srcLoad = (*srcLoadPtr++);
				    }

				    sum0 +=
					(secondAccumulator & MASK_0000FFFF) +
					((firstAccumulator) >> 16);

				    sum1 +=
					(thirdAccumulator & MASK_0000FFFF) +
					((secondAccumulator) >> 16);

				    sum2 +=
					(firstAccumulator & MASK_0000FFFF) +
					((thirdAccumulator) >> 16);
				    thirdAccumulator = firstAccumulator =
					secondAccumulator = 0;
			    }
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (; i <= byteNumber - 12 - 4; i += 12) {
				    secondLoad = (*srcLoadPtr++);
				    thirdLoad = (*srcLoadPtr++);

				    firstAccumulator +=
					((srcLoad >> 8) & MASK_00FF00FF);
				    secondAccumulator +=
					(srcLoad & MASK_00FF00FF);
#ifdef _LITTLE_ENDIAN
				    thirdAccumulator +=
					((secondLoad >> 8) & MASK_00FF00FF);
				    firstAccumulator +=
					(secondLoad & MASK_00FF00FF);
				    secondAccumulator +=
					((thirdLoad >> 8) & MASK_00FF00FF);
				    thirdAccumulator +=
					(thirdLoad & MASK_00FF00FF);
#else /* _LITTLE_ENDIAN */
				    secondAccumulator +=
					((secondLoad >> 8) & MASK_00FF00FF);
				    thirdAccumulator +=
					(secondLoad & MASK_00FF00FF);
				    thirdAccumulator +=
					((thirdLoad >> 8) & MASK_00FF00FF);
				    firstAccumulator +=
					(thirdLoad & MASK_00FF00FF);
#endif /* _LITTLE_ENDIAN */
				    srcLoad = (*srcLoadPtr++);
			    }

			    byteNumber -= i;

			    if (byteNumber >= 12) {
				    secondLoad = (*srcLoadPtr++);
				    thirdLoad = (*srcLoadPtr++);
				    byteNumber -= 12;

				    firstAccumulator +=
					((srcLoad >> 8) & MASK_00FF00FF);
				    secondAccumulator +=
					(srcLoad & MASK_00FF00FF);
#ifdef _LITTLE_ENDIAN
				    thirdAccumulator +=
					((secondLoad >> 8) & MASK_00FF00FF);
				    firstAccumulator +=
					(secondLoad & MASK_00FF00FF);
				    secondAccumulator +=
					((thirdLoad >> 8) & MASK_00FF00FF);
				    thirdAccumulator +=
					(thirdLoad & MASK_00FF00FF);
#else /* _LITTLE_ENDIAN */
				    secondAccumulator +=
					((secondLoad >> 8) & MASK_00FF00FF);
				    thirdAccumulator +=
					(secondLoad & MASK_00FF00FF);
				    thirdAccumulator +=
					((thirdLoad >> 8) & MASK_00FF00FF);
				    firstAccumulator +=
					(thirdLoad & MASK_00FF00FF);
#endif /* _LITTLE_ENDIAN */
				    if (byteNumber > 0) {
					srcLoad = (*srcLoadPtr++);
#ifdef _LITTLE_ENDIAN
					srcLoad =
					    srcLoad & ~((mlib_u32)0xffffffff <<
					    (byteNumber << 3));
#else /* _LITTLE_ENDIAN */
					srcLoad =
					    srcLoad & ~((mlib_u32)0xffffffff >>
					    (byteNumber << 3));
#endif /* _LITTLE_ENDIAN */
					firstAccumulator +=
					    ((srcLoad >> 8) & MASK_00FF00FF);
					secondAccumulator +=
					    (srcLoad & MASK_00FF00FF);
				    }
			    } else if (byteNumber >= 8) {
				    secondLoad = (*srcLoadPtr++);
				    byteNumber -= 8;

				    firstAccumulator +=
					((srcLoad >> 8) & MASK_00FF00FF);
				    secondAccumulator +=
					(srcLoad & MASK_00FF00FF);
#ifdef _LITTLE_ENDIAN
				    thirdAccumulator +=
					((secondLoad >> 8) & MASK_00FF00FF);
				    firstAccumulator +=
					(secondLoad & MASK_00FF00FF);
#else /* _LITTLE_ENDIAN */
				    secondAccumulator +=
					((secondLoad >> 8) & MASK_00FF00FF);
				    thirdAccumulator +=
					(secondLoad & MASK_00FF00FF);
#endif /* _LITTLE_ENDIAN */

				    if (byteNumber > 0) {
					thirdLoad = (*srcLoadPtr++);
#ifdef _LITTLE_ENDIAN
					thirdLoad =
					    thirdLoad & ~((mlib_u32)0xffffffff
					    << (byteNumber << 3));
					secondAccumulator +=
					    ((thirdLoad >> 8) & MASK_00FF00FF);
					thirdAccumulator +=
					    (thirdLoad & MASK_00FF00FF);
#else /* _LITTLE_ENDIAN */
					thirdLoad =
					    thirdLoad & ~((mlib_u32)0xffffffff
					    >> (byteNumber << 3));
					thirdAccumulator +=
					    ((thirdLoad >> 8) & MASK_00FF00FF);
					firstAccumulator +=
					    (thirdLoad & MASK_00FF00FF);
#endif /* _LITTLE_ENDIAN */
				    }
			    } else if (byteNumber >= 4) {
				    byteNumber -= 4;

				    firstAccumulator +=
					((srcLoad >> 8) & MASK_00FF00FF);
				    secondAccumulator +=
					(srcLoad & MASK_00FF00FF);

				    if (byteNumber > 0) {
					secondLoad = (*srcLoadPtr++);
#ifdef _LITTLE_ENDIAN
					secondLoad =
					    secondLoad & ~((mlib_u32)0xffffffff
					    << (byteNumber << 3));
					thirdAccumulator +=
					    ((secondLoad >> 8) & MASK_00FF00FF);
					firstAccumulator +=
					    (secondLoad & MASK_00FF00FF);
#else /* _LITTLE_ENDIAN */
					secondLoad =
					    secondLoad & ~((mlib_u32)0xffffffff
					    >> (byteNumber << 3));
					secondAccumulator +=
					    ((secondLoad >> 8) & MASK_00FF00FF);
					thirdAccumulator +=
					    (secondLoad & MASK_00FF00FF);
#endif /* _LITTLE_ENDIAN */
				    }
			    } else if (byteNumber > 0) {
#ifdef _LITTLE_ENDIAN
				    srcLoad =
					srcLoad & ~((mlib_u32)0xffffffff <<
					(byteNumber << 3));
#else /* _LITTLE_ENDIAN */
				    srcLoad =
					srcLoad & ~((mlib_u32)0xffffffff >>
					(byteNumber << 3));
#endif /* _LITTLE_ENDIAN */
				    firstAccumulator +=
					((srcLoad >> 8) & MASK_00FF00FF);
				    secondAccumulator +=
					(srcLoad & MASK_00FF00FF);
			    }

			    sum0 += (secondAccumulator & MASK_0000FFFF) +
				((firstAccumulator) >> 16);

			    sum1 += (thirdAccumulator & MASK_0000FFFF) +
				((secondAccumulator) >> 16);

			    sum2 += (firstAccumulator & MASK_0000FFFF) +
				((thirdAccumulator) >> 16);

			    mean[al2off[alignSteps]] += sum0 * divider;
#ifdef _LITTLE_ENDIAN
			    mean[al2off[alignSteps + 1]] += sum1 * divider;
			    mean[al2off[alignSteps + 2]] += sum2 * divider;
#else /* _LITTLE_ENDIAN */
			    mean[al2off[alignSteps + 2]] += sum1 * divider;
			    mean[al2off[alignSteps + 1]] += sum2 * divider;
#endif /* _LITTLE_ENDIAN */

			    srcLinePtr += srcYStride;
			    srcPixelPtr = srcLinePtr;
		    }
		}

		break;

	case 4:
		mean[0] = mean[1] = mean[2] = mean[3] = 0.0;
		for (j = 0; j < srcHeight; j++) {
			alignSteps = (mlib_addr)srcPixelPtr & 3;
			cycleNumber = (srcWidth - 1) >> 8;
			loadsNumber = (srcWidth - 1) & 0xff;

			firstAccumulator = secondAccumulator = 0;
			sum0 = sum1 = sum2 = sum3 = 0.0;

			srcLoadPtr = (mlib_u32 *)(srcPixelPtr - alignSteps);
#ifdef _LITTLE_ENDIAN
			mask = (mlib_u32)0xffffffff << (alignSteps << 3);
#else /* _LITTLE_ENDIAN */
			mask = (mlib_u32)0xffffffff >> (alignSteps << 3);
#endif /* _LITTLE_ENDIAN */
			srcLoad = mask & (*srcLoadPtr++);
			firstAccumulator += (srcLoad & MASK_00FF00FF);
			secondAccumulator += ((srcLoad >> 8) & MASK_00FF00FF);

			for (i = 0; i < cycleNumber; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (k = 0; k < 64; k++) {
					srcLoad = (*srcLoadPtr++);
					firstAccumulator +=
					    (srcLoad & MASK_00FF00FF);
					secondAccumulator +=
					    ((srcLoad >> 8) & MASK_00FF00FF);

					srcLoad = (*srcLoadPtr++);
					firstAccumulator +=
					    (srcLoad & MASK_00FF00FF);
					secondAccumulator +=
					    ((srcLoad >> 8) & MASK_00FF00FF);

					srcLoad = (*srcLoadPtr++);
					firstAccumulator +=
					    (srcLoad & MASK_00FF00FF);
					secondAccumulator +=
					    ((srcLoad >> 8) & MASK_00FF00FF);

					srcLoad = (*srcLoadPtr++);
					firstAccumulator +=
					    (srcLoad & MASK_00FF00FF);
					secondAccumulator +=
					    ((srcLoad >> 8) & MASK_00FF00FF);
				}

				sum0 += ((secondAccumulator) >> 16);
				sum1 += ((firstAccumulator) >> 16);
				sum2 += (secondAccumulator & MASK_0000FFFF);
				sum3 += (firstAccumulator & MASK_0000FFFF);

				firstAccumulator = secondAccumulator = 0;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < loadsNumber; i++) {
				srcLoad = (*srcLoadPtr++);
				firstAccumulator += (srcLoad & MASK_00FF00FF);
				secondAccumulator +=
				    ((srcLoad >> 8) & MASK_00FF00FF);

			}

			if (alignSteps) {
				srcLoad = (*srcLoadPtr++);
				srcLoad = srcLoad & ~mask;
				firstAccumulator += (srcLoad & MASK_00FF00FF);
				secondAccumulator += ((srcLoad >> 8) &
				    MASK_00FF00FF);
			}

			sum0 += ((secondAccumulator) >> 16);
			sum1 += ((firstAccumulator) >> 16);
			sum2 += (secondAccumulator & MASK_0000FFFF);
			sum3 += (firstAccumulator & MASK_0000FFFF);

#ifdef _LITTLE_ENDIAN
			mean[3 & (0 - alignSteps)] += sum3 * divider;
			mean[3 & (1 - alignSteps)] += sum2 * divider;
			mean[3 & (2 - alignSteps)] += sum1 * divider;
			mean[3 & (3 - alignSteps)] += sum0 * divider;
#else /* _LITTLE_ENDIAN */
			mean[3 & (0 - alignSteps)] += sum0 * divider;
			mean[3 & (1 - alignSteps)] += sum1 * divider;
			mean[3 & (2 - alignSteps)] += sum2 * divider;
			mean[3 & (3 - alignSteps)] += sum3 * divider;
#endif /* _LITTLE_ENDIAN */

			srcLinePtr += srcYStride;
			srcPixelPtr = srcLinePtr;
		}

		break;
	}
}

/* *********************************************************** */

void
mlib_c_ImageMeanU16Case(
    const mlib_image *src,
    mlib_d64 *mean)
{
/* pointer for pixel of source */
	mlib_u16 *srcPixelPtr = (mlib_u16 *)mlib_ImageGetData(src);

/* pointer for line of source */
	mlib_u8 *srcLinePtr = (mlib_u8 *)srcPixelPtr;

/* width of source image */
	mlib_s32 srcWidth = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 srcHeight = mlib_ImageGetHeight(src);

/* total number of pixels */
	mlib_s32 srcTotalPixNum = srcHeight * srcWidth;

/* linestride = bytes to next row */
	mlib_s32 srcYStride = mlib_ImageGetStride(src);

/* number of channels */
	mlib_s32 numOfChannels = mlib_ImageGetChannels(src);
	mlib_d64 sum0 = 0.0, sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
	mlib_d64 divider = 1.0 / srcTotalPixNum;

/* indices */
	mlib_s32 i, k, j;
	mlib_u32 pixelNumber;
	mlib_u32 cycleNumber;

	if (2 * numOfChannels * srcWidth == srcYStride) {
		srcWidth = srcTotalPixNum;
		srcHeight = 1;
	}

/* pixel number is equal to number of pixels in a row */
	pixelNumber = (mlib_u32)srcWidth;
/* number of 2^15 cycles */
	cycleNumber = pixelNumber >> 15;
	pixelNumber &= 0x7fff;

	switch (numOfChannels) {

	case 1: {

		    mlib_s32 firstAccumulator = 0;

		    for (j = 0; j < srcHeight; j++) {
			    for (k = 0; k < (mlib_s32)cycleNumber; k++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				    for (i = 0; i < 16384; i++) {
					    firstAccumulator +=
						(*srcPixelPtr++);
					    firstAccumulator +=
						(*srcPixelPtr++);
				    }

				    sum0 += firstAccumulator;
				    firstAccumulator = 0;
			    }

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (i = 0; i < (mlib_s32)(pixelNumber >> 1); i++) {
				    firstAccumulator += (*srcPixelPtr++);
				    firstAccumulator += (*srcPixelPtr++);
			    }

			    if (pixelNumber & 1)
				    firstAccumulator += (*srcPixelPtr++);

			    sum0 += firstAccumulator;
			    firstAccumulator = 0;

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_u16 *)srcLinePtr;
		    }

		    sum0 *= divider;
		    mean[0] = sum0;
	    }

		break;

	case 2: {

		    mlib_s32 firstAccumulator = 0;
		    mlib_s32 secondAccumulator = 0;

		    for (j = 0; j < srcHeight; j++) {
			    for (k = 0; k < (mlib_s32)cycleNumber; k++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				    for (i = 0; i < 16384; i++) {
					    firstAccumulator +=
						(*srcPixelPtr++);
					    secondAccumulator +=
						(*srcPixelPtr++);
					    firstAccumulator +=
						(*srcPixelPtr++);
					    secondAccumulator +=
						(*srcPixelPtr++);
				    }

				    sum0 += firstAccumulator;
				    sum1 += secondAccumulator;
				    firstAccumulator = secondAccumulator = 0;
			    }

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (i = 0; i < (mlib_s32)pixelNumber; i++) {
				    firstAccumulator += (*srcPixelPtr++);
				    secondAccumulator += (*srcPixelPtr++);
			    }

			    sum0 += firstAccumulator;
			    sum1 += secondAccumulator;
			    firstAccumulator = secondAccumulator = 0;

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_u16 *)srcLinePtr;
		    }

		    sum0 *= divider;
		    sum1 *= divider;

		    mean[0] = sum0;
		    mean[1] = sum1;
	    }

		break;

	case 3: {

		    mlib_s32 firstAccumulator = 0;
		    mlib_s32 secondAccumulator = 0;
		    mlib_s32 thirdAccumulator = 0;

		    for (j = 0; j < srcHeight; j++) {
			    for (k = 0; k < (mlib_s32)cycleNumber; k++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				    for (i = 0; i < 16384; i++) {
					    firstAccumulator +=
						(*srcPixelPtr++);
					    secondAccumulator +=
						(*srcPixelPtr++);
					    thirdAccumulator +=
						(*srcPixelPtr++);
					    firstAccumulator +=
						(*srcPixelPtr++);
					    secondAccumulator +=
						(*srcPixelPtr++);
					    thirdAccumulator +=
						(*srcPixelPtr++);
				    }

				    sum0 += firstAccumulator;
				    sum1 += secondAccumulator;
				    sum2 += thirdAccumulator;
				    firstAccumulator = secondAccumulator =
					thirdAccumulator = 0;
			    }

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (i = 0; i < (mlib_s32)pixelNumber - 1; i += 2) {
				    firstAccumulator += (*srcPixelPtr++);
				    secondAccumulator += (*srcPixelPtr++);
				    thirdAccumulator += (*srcPixelPtr++);
				    firstAccumulator += (*srcPixelPtr++);
				    secondAccumulator += (*srcPixelPtr++);
				    thirdAccumulator += (*srcPixelPtr++);
			    }

			    if (i < (mlib_s32)pixelNumber) {
				    firstAccumulator += (*srcPixelPtr++);
				    secondAccumulator += (*srcPixelPtr++);
				    thirdAccumulator += (*srcPixelPtr++);
			    }

			    sum0 += firstAccumulator;
			    sum1 += secondAccumulator;
			    sum2 += thirdAccumulator;
			    firstAccumulator = secondAccumulator =
				thirdAccumulator = 0;

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_u16 *)srcLinePtr;
		    }

		    sum0 *= divider;
		    sum1 *= divider;
		    sum2 *= divider;

		    mean[0] = sum0;
		    mean[1] = sum1;
		    mean[2] = sum2;
	    }

		break;

	case 4: {

		    mlib_s32 firstAccumulator = 0;
		    mlib_s32 secondAccumulator = 0;
		    mlib_s32 thirdAccumulator = 0;
		    mlib_s32 forthAccumulator = 0;

		    for (j = 0; j < srcHeight; j++) {
			    for (k = 0; k < (mlib_s32)cycleNumber; k++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				    for (i = 0; i < 32768; i++) {
					    firstAccumulator +=
						(*srcPixelPtr++);
					    secondAccumulator +=
						(*srcPixelPtr++);
					    thirdAccumulator +=
						(*srcPixelPtr++);
					    forthAccumulator +=
						(*srcPixelPtr++);
				    }

				    sum0 += firstAccumulator;
				    sum1 += secondAccumulator;
				    sum2 += thirdAccumulator;
				    sum3 += forthAccumulator;
				    firstAccumulator = secondAccumulator =
					thirdAccumulator = forthAccumulator = 0;
			    }

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (i = 0; i < (mlib_s32)pixelNumber; i++) {
				    firstAccumulator += (*srcPixelPtr++);
				    secondAccumulator += (*srcPixelPtr++);
				    thirdAccumulator += (*srcPixelPtr++);
				    forthAccumulator += (*srcPixelPtr++);
			    }

			    sum0 += firstAccumulator;
			    sum1 += secondAccumulator;
			    sum2 += thirdAccumulator;
			    sum3 += forthAccumulator;
			    firstAccumulator = secondAccumulator =
				thirdAccumulator = forthAccumulator = 0;

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_u16 *)srcLinePtr;
		    }

		    sum0 *= divider;
		    sum1 *= divider;
		    sum2 *= divider;
		    sum3 *= divider;

		    mean[0] = sum0;
		    mean[1] = sum1;
		    mean[2] = sum2;
		    mean[3] = sum3;
	    }

		break;
	}
}

/* *********************************************************** */

void
mlib_c_ImageMeanS16Case(
    const mlib_image *src,
    mlib_d64 *mean)
{
/* pointer for pixel of source */
	mlib_s16 *srcPixelPtr = (mlib_s16 *)mlib_ImageGetData(src);

/* pointer for line of source */
	mlib_u8 *srcLinePtr = (mlib_u8 *)srcPixelPtr;

/* width of source image */
	mlib_s32 srcWidth = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 srcHeight = mlib_ImageGetHeight(src);

/* total number of pixels */
	mlib_s32 srcTotalPixNum = srcHeight * srcWidth;

/* linestride = bytes to next row */
	mlib_s32 srcYStride = mlib_ImageGetStride(src);

/* number of channels */
	mlib_s32 numOfChannels = mlib_ImageGetChannels(src);
	mlib_d64 sum0 = 0.0, sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
	mlib_d64 divider = 1.0 / srcTotalPixNum;

/* indices */
	mlib_s32 i, k, j;
	mlib_u32 pixelNumber;
	mlib_u32 cycleNumber;

	if (2 * numOfChannels * srcWidth == srcYStride) {
		srcWidth = srcTotalPixNum;
		srcHeight = 1;
	}

/* pixel number is equal to number of pixels in a row */
	pixelNumber = (mlib_u32)srcWidth;
/* number of 2^15 cycles */
	cycleNumber = pixelNumber >> 15;
	pixelNumber &= 0x7fff;

	switch (numOfChannels) {

	case 1: {

		    mlib_s32 firstAccumulator = 0;

		    for (j = 0; j < srcHeight; j++) {
			    for (k = 0; k < (mlib_s32)cycleNumber; k++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				    for (i = 0; i < 32768; i++) {
					    firstAccumulator +=
						0x8000 ^ *(mlib_u16
						*)(srcPixelPtr++);
				    }

				    sum0 += firstAccumulator - 0x40000000;
				    firstAccumulator = 0;
			    }

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (i = 0; i < (mlib_s32)(pixelNumber >> 1); i++) {
				    firstAccumulator +=
					0x8000 ^ *(mlib_u16 *)(srcPixelPtr++);
				    firstAccumulator +=
					0x8000 ^ *(mlib_u16 *)(srcPixelPtr++);
			    }

			    if (pixelNumber & 1)
				    firstAccumulator +=
					0x8000 ^ *(mlib_u16 *)(srcPixelPtr++);

			    sum0 +=
				(firstAccumulator -
				((mlib_s32)pixelNumber << 15));
			    firstAccumulator = 0;

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_s16 *)srcLinePtr;
		    }

		    sum0 *= divider;

		    mean[0] = sum0;
	    }

		break;

	case 2: {

		    mlib_s32 firstAccumulator = 0;
		    mlib_s32 secondAccumulator = 0;

		    for (j = 0; j < srcHeight; j++) {
			    for (k = 0; k < (mlib_s32)cycleNumber; k++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				    for (i = 0; i < 32768; i++) {
					    firstAccumulator +=
						0x8000 ^ *(mlib_u16
						*)(srcPixelPtr++);
					    secondAccumulator +=
						0x8000 ^ *(mlib_u16
						*)(srcPixelPtr++);
				    }

				    sum0 += firstAccumulator - 0x40000000;
				    sum1 += secondAccumulator - 0x40000000;
				    firstAccumulator = secondAccumulator = 0;
			    }

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (i = 0; i < (mlib_s32)pixelNumber; i++) {
				    firstAccumulator +=
					0x8000 ^ *(mlib_u16 *)(srcPixelPtr++);
				    secondAccumulator +=
					0x8000 ^ *(mlib_u16 *)(srcPixelPtr++);
			    }

			    sum0 +=
				firstAccumulator -
				((mlib_s32)pixelNumber << 15);
			    sum1 +=
				secondAccumulator -
				((mlib_s32)pixelNumber << 15);
			    firstAccumulator = secondAccumulator = 0;

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_s16 *)srcLinePtr;
		    }

		    sum0 *= divider;
		    sum1 *= divider;

		    mean[0] = sum0;
		    mean[1] = sum1;
	    }

		break;

	case 3: {

		    mlib_s32 firstAccumulator = 0;
		    mlib_s32 secondAccumulator = 0;
		    mlib_s32 thirdAccumulator = 0;

		    for (j = 0; j < srcHeight; j++) {
			    for (k = 0; k < (mlib_s32)cycleNumber; k++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				    for (i = 0; i < 32768; i++) {
					    firstAccumulator +=
						0x8000 ^ *(mlib_u16
						*)(srcPixelPtr++);
					    secondAccumulator +=
						*(srcPixelPtr++);
					    thirdAccumulator +=
						*(srcPixelPtr++);
				    }

				    sum0 += firstAccumulator - 0x40000000;
				    sum1 += secondAccumulator;
				    sum2 += thirdAccumulator;
				    firstAccumulator = secondAccumulator =
					thirdAccumulator = 0;
			    }

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (i = 0; i < (mlib_s32)pixelNumber; i++) {
				    firstAccumulator +=
					0x8000 ^ *(mlib_u16 *)(srcPixelPtr++);
				    secondAccumulator += *(srcPixelPtr++);
				    thirdAccumulator += *(srcPixelPtr++);
			    }

			    sum0 +=
				firstAccumulator -
				((mlib_s32)pixelNumber << 15);
			    sum1 += secondAccumulator;
			    sum2 += thirdAccumulator;
			    firstAccumulator = secondAccumulator =
				thirdAccumulator = 0;

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_s16 *)srcLinePtr;
		    }

		    sum0 *= divider;
		    sum1 *= divider;
		    sum2 *= divider;

		    mean[0] = sum0;
		    mean[1] = sum1;
		    mean[2] = sum2;
	    }

		break;

	case 4: {

		    mlib_s32 firstAccumulator = 0;
		    mlib_s32 secondAccumulator = 0;
		    mlib_s32 thirdAccumulator = 0;
		    mlib_s32 forthAccumulator = 0;

		    for (j = 0; j < srcHeight; j++) {
			    for (k = 0; k < (mlib_s32)cycleNumber; k++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				    for (i = 0; i < 32768; i++) {
					    firstAccumulator +=
						0x8000 ^ *(mlib_u16
						*)(srcPixelPtr++);
					    secondAccumulator +=
						0x8000 ^ *(mlib_u16
						*)(srcPixelPtr++);
					    thirdAccumulator +=
						*(srcPixelPtr++);
					    forthAccumulator +=
						*(srcPixelPtr++);
				    }

				    sum0 += firstAccumulator - 0x40000000;
				    sum1 += secondAccumulator - 0x40000000;
				    sum2 += thirdAccumulator;
				    sum3 += forthAccumulator;
				    firstAccumulator = secondAccumulator =
					thirdAccumulator = forthAccumulator = 0;
			    }

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (i = 0; i < (mlib_s32)pixelNumber; i++) {
				    firstAccumulator +=
					0x8000 ^ *(mlib_u16 *)(srcPixelPtr++);
				    secondAccumulator +=
					0x8000 ^ *(mlib_u16 *)(srcPixelPtr++);
				    thirdAccumulator += *(srcPixelPtr++);
				    forthAccumulator += *(srcPixelPtr++);
			    }

			    sum0 +=
				firstAccumulator -
				((mlib_s32)pixelNumber << 15);
			    sum1 +=
				secondAccumulator -
				((mlib_s32)pixelNumber << 15);
			    sum2 += thirdAccumulator;
			    sum3 += forthAccumulator;
			    firstAccumulator = secondAccumulator =
				thirdAccumulator = forthAccumulator = 0;

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_s16 *)srcLinePtr;
		    }

		    sum0 *= divider;
		    sum1 *= divider;
		    sum2 *= divider;
		    sum3 *= divider;

		    mean[0] = sum0;
		    mean[1] = sum1;
		    mean[2] = sum2;
		    mean[3] = sum3;
	    }

		break;
	}
}

/* *********************************************************** */

void
mlib_c_ImageMeanS32Case(
    const mlib_image *src,
    mlib_d64 *mean)
{
/* width of source image */
	mlib_s32 srcWidth = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 srcHeight = mlib_ImageGetHeight(src);

/* linestride = bytes to next row */
	mlib_s32 srcYStride = mlib_ImageGetStride(src);

/* number of channels */
	mlib_s32 numOfChannels = mlib_ImageGetChannels(src);

/* pointer for line of source */
	mlib_u8 *srcLinePtr = (mlib_u8 *)mlib_ImageGetData(src);

/* pointer for pixel of source */
	mlib_s32 *srcPixelPtr = (mlib_s32 *)srcLinePtr;

/* total number of pixels */
	mlib_s32 srcTotalPixNum = srcHeight * srcWidth;
	mlib_d64 divider = 1.0 / srcTotalPixNum;

/* indices */
	mlib_s32 k, j;

	if (4 * numOfChannels * srcWidth == srcYStride) {
		srcWidth = srcTotalPixNum;
		srcHeight = 1;
	}

	switch (numOfChannels) {

	case 1: {

		    mlib_d64 firstAccumulator = 0.0;
		    mlib_s32 i, cycleNumber = srcWidth >> 16;
		    mlib_s32 val, hi, lo;

		    srcWidth &= 0xffff;

		    for (j = 0; j < srcHeight; j++) {
			    hi = lo = 0;
			    for (i = 0; i < cycleNumber; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				    for (k = 0; k < 0x8000; k++) {
					    firstAccumulator +=
						(*srcPixelPtr++);
					    val = (*srcPixelPtr++);
					    hi += val >> 16;
					    lo += val & 0xffff;
				    }

				    firstAccumulator +=
					(mlib_d64)lo + (mlib_d64)hi *65536.;

				    hi = lo = 0;
			    }

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (k = 0; k < (srcWidth >> 1); k++) {
				    firstAccumulator += (*srcPixelPtr++);
				    val = (*srcPixelPtr++);
				    hi += val >> 16;
				    lo += val & 0xffff;
			    }

			    if (srcWidth & 1)
				    firstAccumulator += *srcPixelPtr;
			    firstAccumulator +=
				(mlib_d64)lo + (mlib_d64)hi *65536.;

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_s32 *)srcLinePtr;
		    }

		    firstAccumulator *= divider;

		    mean[0] = firstAccumulator;
	    }

		break;

	case 2: {

		    mlib_d64 firstAccumulator = 0.0;
		    mlib_d64 secondAccumulator = 0.0;
		    mlib_s32 i, cycleNumber = srcWidth >> 15;
		    mlib_s32 val, hi, lo;

		    srcWidth &= 0x7fff;

		    for (j = 0; j < srcHeight; j++) {
			    hi = lo = 0;
			    for (i = 0; i < cycleNumber; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				    for (k = 0; k < 0x8000; k++) {
					    firstAccumulator +=
						(*srcPixelPtr++);
					    val = (*srcPixelPtr++);
					    hi += val >> 16;
					    lo += val & 0xffff;
				    }

				    secondAccumulator +=
					(mlib_d64)lo + (mlib_d64)hi *65536.;

				    hi = lo = 0;
			    }

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (k = 0; k < srcWidth; k++) {
				    firstAccumulator += (*srcPixelPtr++);
				    val = (*srcPixelPtr++);
				    hi += val >> 16;
				    lo += val & 0xffff;
			    }

			    secondAccumulator +=
				(mlib_d64)lo + (mlib_d64)hi *65536.;

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_s32 *)srcLinePtr;
		    }

		    firstAccumulator *= divider;
		    secondAccumulator *= divider;

		    mean[0] = firstAccumulator;
		    mean[1] = secondAccumulator;
	    }

		break;

	case 3: {

		    mlib_d64 firstAccumulator = 0.0;
		    mlib_d64 secondAccumulator = 0.0;
		    mlib_d64 thirdAccumulator = 0.0;
		    mlib_s32 i, cycleNumber = srcWidth >> 15;
		    mlib_s32 val, hi, lo;

		    srcWidth &= 0x7fff;

		    for (j = 0; j < srcHeight; j++) {
			    hi = lo = 0;
			    for (i = 0; i < cycleNumber; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				    for (k = 0; k < 0x8000; k++) {
					    firstAccumulator +=
						(*srcPixelPtr++);
					    secondAccumulator +=
						(*srcPixelPtr++);
					    val = (*srcPixelPtr++);
					    hi += val >> 16;
					    lo += val & 0xffff;
				    }

				    thirdAccumulator +=
					(mlib_d64)lo + (mlib_d64)hi *65536.;

				    hi = lo = 0;
			    }

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (k = 0; k < srcWidth; k++) {
				    firstAccumulator += (*srcPixelPtr++);
				    secondAccumulator += (*srcPixelPtr++);
				    val = (*srcPixelPtr++);
				    hi += val >> 16;
				    lo += val & 0xffff;
			    }

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_s32 *)srcLinePtr;
			    thirdAccumulator +=
				(mlib_d64)lo + (mlib_d64)hi *65536.;
		    }

		    firstAccumulator *= divider;
		    secondAccumulator *= divider;
		    thirdAccumulator *= divider;

		    mean[0] = firstAccumulator;
		    mean[1] = secondAccumulator;
		    mean[2] = thirdAccumulator;
	    }

		break;

	case 4: {

		    mlib_d64 firstAccumulator = 0.0;
		    mlib_d64 secondAccumulator = 0.0;
		    mlib_d64 thirdAccumulator = 0.0;
		    mlib_d64 forthAccumulator = 0.0;
		    mlib_s32 i, cycleNumber = srcWidth >> 15;
		    mlib_s32 val, hi, lo;
		    mlib_s32 val2, hi2, lo2;

		    srcWidth &= 0x7fff;

		    for (j = 0; j < srcHeight; j++) {
			    hi = lo = hi2 = lo2 = 0;
			    for (i = 0; i < cycleNumber; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				    for (k = 0; k < 0x8000; k++) {
					    firstAccumulator +=
						(*srcPixelPtr++);
					    secondAccumulator +=
						(*srcPixelPtr++);
					    val2 = (*srcPixelPtr++);
					    hi2 += val2 >> 16;
					    lo2 += val2 & 0xffff;
					    val = (*srcPixelPtr++);
					    hi += val >> 16;
					    lo += val & 0xffff;
				    }

				    thirdAccumulator +=
					(mlib_d64)lo2 + (mlib_d64)hi2 *65536.;
				    forthAccumulator +=
					(mlib_d64)lo + (mlib_d64)hi *65536.;

				    hi = lo = hi2 = lo2 = 0;
			    }

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			    for (k = 0; k < srcWidth; k++) {
				    firstAccumulator += (*srcPixelPtr++);
				    secondAccumulator += (*srcPixelPtr++);
				    val2 = (*srcPixelPtr++);
				    hi2 += val2 >> 16;
				    lo2 += val2 & 0xffff;
				    val = (*srcPixelPtr++);
				    hi += val >> 16;
				    lo += val & 0xffff;
			    }

			    srcLinePtr += srcYStride;
			    srcPixelPtr = (mlib_s32 *)srcLinePtr;
			    thirdAccumulator +=
				(mlib_d64)lo2 + (mlib_d64)hi2 *65536.;
			    forthAccumulator +=
				(mlib_d64)lo + (mlib_d64)hi *65536.;
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
#endif /* end of ifndef _NO_LONGLONG */
/* *********************************************************** */
