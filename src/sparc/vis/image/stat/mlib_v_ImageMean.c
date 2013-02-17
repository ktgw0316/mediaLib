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

#pragma ident	"@(#)mlib_v_ImageMean.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageMean - calculates image mean for the input image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMean(mlib_d64         *mean,
 *                                 const mlib_image *src)
 *
 * ARGUMENT
 *      mean  Pointer to mean array
 *      src    Pointer to input image
 *
 * DESCRIPTION
 *
 *           1    w - 1 h - 1
 *      m = --- * SUM SUM x(i, j)
 *          w * h   i = 0 j = 0
 *
 * RESTRICTION
 *      The image can have 1, 2, 3 or 4 channels.
 *      It can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageStat.h>

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

static void mlib_v_ImageMeanU8Case(
    const mlib_image *src,
    mlib_d64 *mean);

static void mlib_v_ImageMeanS16Case(
    const mlib_image *src,
    mlib_d64 *mean);

static void mlib_v_ImageMeanU16Case(
    const mlib_image *src,
    mlib_d64 *mean);

static void mlib_v_ImageMeanS32Case(
    const mlib_image *src,
    mlib_d64 *mean);

/* *********************************************************** */

#define	MASK_0000FFFF	0x0000FFFF
#define	MASK_00007FFF	0x00007FFF
#define	MASK_000000FF	0x000000FF

/* *********************************************************** */

#define	PROLOG                                                  \
	if (alignSteps > 6)                                     \
	    sum1 += (*srcPixelPtr++);                           \
	if (alignSteps > 5)                                     \
	    sum2 += (*srcPixelPtr++);                           \
	if (alignSteps > 4)                                     \
	    sum3 += (*srcPixelPtr++);                           \
	if (alignSteps > 3)                                     \
	    sum0 += (*srcPixelPtr++);                           \
	if (alignSteps > 2)                                     \
	    sum1 += (*srcPixelPtr++);                           \
	if (alignSteps > 1)                                     \
	    sum2 += (*srcPixelPtr++);                           \
	if (alignSteps > 0)                                     \
	    sum3 += (*srcPixelPtr++);                           \
	srcLoadPtr = (mlib_d64 *)srcPixelPtr

/* *********************************************************** */

#define	EPILOG                                                  \
	switch (alignSteps) {                                   \
	case 0:                                                 \
	case 4:                                                 \
	    if (byteNumber > 6)                                 \
		sum1 += (*srcPixelPtr++);                       \
	    if (byteNumber > 5)                                 \
		sum2 += (*srcPixelPtr++);                       \
	    if (byteNumber > 4)                                 \
		sum3 += (*srcPixelPtr++);                       \
	    if (byteNumber > 3)                                 \
		sum0 += (*srcPixelPtr++);                       \
	    if (byteNumber > 2)                                 \
		sum1 += (*srcPixelPtr++);                       \
	    if (byteNumber > 1)                                 \
		sum2 += (*srcPixelPtr++);                       \
	    if (byteNumber > 0)                                 \
		sum3 += *srcPixelPtr;                           \
	    break;                                              \
	case 1:                                                 \
	case 5:                                                 \
	    if (byteNumber > 6)                                 \
		sum0 += (*srcPixelPtr++);                       \
	    if (byteNumber > 5)                                 \
		sum1 += (*srcPixelPtr++);                       \
	    if (byteNumber > 4)                                 \
		sum2 += (*srcPixelPtr++);                       \
	    if (byteNumber > 3)                                 \
		sum3 += (*srcPixelPtr++);                       \
	    if (byteNumber > 2)                                 \
		sum0 += (*srcPixelPtr++);                       \
	    if (byteNumber > 1)                                 \
		sum1 += (*srcPixelPtr++);                       \
	    if (byteNumber > 0)                                 \
		sum2 += *srcPixelPtr;                           \
	    break;                                              \
	case 2:                                                 \
	case 6:                                                 \
	    if (byteNumber > 6)                                 \
		sum3 += (*srcPixelPtr++);                       \
	    if (byteNumber > 5)                                 \
		sum0 += (*srcPixelPtr++);                       \
	    if (byteNumber > 4)                                 \
		sum1 += (*srcPixelPtr++);                       \
	    if (byteNumber > 3)                                 \
		sum2 += (*srcPixelPtr++);                       \
	    if (byteNumber > 2)                                 \
		sum3 += (*srcPixelPtr++);                       \
	    if (byteNumber > 1)                                 \
		sum0 += (*srcPixelPtr++);                       \
	    if (byteNumber > 0)                                 \
		sum1 += *srcPixelPtr;                           \
	    break;                                              \
	case 3:                                                 \
	case 7:                                                 \
	    if (byteNumber > 6)                                 \
		sum2 += (*srcPixelPtr++);                       \
	    if (byteNumber > 5)                                 \
		sum3 += (*srcPixelPtr++);                       \
	    if (byteNumber > 4)                                 \
		sum0 += (*srcPixelPtr++);                       \
	    if (byteNumber > 3)                                 \
		sum1 += (*srcPixelPtr++);                       \
	    if (byteNumber > 2)                                 \
		sum2 += (*srcPixelPtr++);                       \
	    if (byteNumber > 1)                                 \
		sum3 += (*srcPixelPtr++);                       \
	    if (byteNumber > 0)                                 \
		sum0 += *srcPixelPtr;                           \
	    break;                                              \
	}

/* *********************************************************** */

#define	MAIN_CYCLE                                                      \
	for (i = 0; i < cycleNumber; i++) {                             \
	    firstAccumulator = secondAccumulator = vis_fzero();         \
	    for (k = 0; k < 256; k++) {                                 \
		srcLoad = (*srcLoadPtr++);                              \
		firstAccumulator =                                      \
		    vis_fpadd16(firstAccumulator,                       \
		    vis_fmul8x16al(vis_read_hi(srcLoad), scale));       \
		secondAccumulator =                                     \
		    vis_fpadd16(secondAccumulator,                      \
		    vis_fmul8x16al(vis_read_lo(srcLoad), scale));       \
	    }                                                           \
	    resUneven.db = vis_fand(firstAccumulator, mask);            \
	    firstAccumulator =                                          \
		vis_faligndata(firstAccumulator, firstAccumulator);     \
	    resEven.db = vis_fand(firstAccumulator, mask);              \
	    sum0 += resEven.two_int.int0;                               \
	    sum1 += resUneven.two_int.int0;                             \
	    sum2 += resEven.two_int.int1;                               \
	    sum3 += resUneven.two_int.int1;                             \
	    resUneven.db = vis_fand(secondAccumulator, mask);           \
	    secondAccumulator =                                         \
		vis_faligndata(secondAccumulator, secondAccumulator);   \
	    resEven.db = vis_fand(secondAccumulator, mask);             \
	    sum0 += resEven.two_int.int0;                               \
	    sum1 += resUneven.two_int.int0;                             \
	    sum2 += resEven.two_int.int1;                               \
	    sum3 += resUneven.two_int.int1;                             \
	}

/* *********************************************************** */

#define	TAIL_CYCLE                                              \
	firstAccumulator = secondAccumulator = vis_fzero();     \
	for (i = 0; i < loadsNumber; i++) {                     \
	    srcLoad = (*srcLoadPtr++);                          \
	    firstAccumulator =                                  \
		vis_fpadd16(firstAccumulator,                   \
		vis_fmul8x16al(vis_read_hi(srcLoad), scale));   \
	    secondAccumulator =                                 \
		vis_fpadd16(secondAccumulator,                  \
		vis_fmul8x16al(vis_read_lo(srcLoad), scale));   \
	}                                                       \
	                                                        \
	resUneven.db = vis_fand(firstAccumulator, mask);        \
	firstAccumulator =                                      \
	vis_faligndata(firstAccumulator, firstAccumulator);     \
	resEven.db = vis_fand(firstAccumulator, mask);          \
	sum0 += resEven.two_int.int0;                           \
	sum1 += resUneven.two_int.int0;                         \
	sum2 += resEven.two_int.int1;                           \
	sum3 += resUneven.two_int.int1;                         \
	resUneven.db = vis_fand(secondAccumulator, mask);       \
	secondAccumulator =                                     \
	vis_faligndata(secondAccumulator, secondAccumulator);   \
	resEven.db = vis_fand(secondAccumulator, mask);         \
	sum0 += resEven.two_int.int0;                           \
	sum1 += resUneven.two_int.int0;                         \
	sum2 += resEven.two_int.int1;                           \
	sum3 += resUneven.two_int.int1;                         \
	srcPixelPtr = (mlib_u8 *)srcLoadPtr

/* *********************************************************** */

#define	ROWRESULT                                               \
	switch (numOfChannels) {                                \
	case 1:                                                 \
	    rowSum[0] += (sum0 + sum1 + sum2 + sum3);           \
	    break;                                              \
	case 2:                                                 \
	    if (alignSteps & 1) {                               \
		rowSum[1] += (sum0 + sum2);                     \
		rowSum[0] += (sum1 + sum3);                     \
	    } else {                                            \
		rowSum[0] += (sum0 + sum2);                     \
		rowSum[1] += (sum1 + sum3);                     \
	    }                                                   \
	    break;                                              \
	case 4:                                                 \
	    switch (alignSteps) {                               \
	    case 0:                                             \
	    case 4:                                             \
		rowSum[0] += sum0;                              \
		rowSum[1] += sum1;                              \
		rowSum[2] += sum2;                              \
		rowSum[3] += sum3;                              \
		break;                                          \
	    case 1:                                             \
	    case 5:                                             \
		rowSum[1] += sum0;                              \
		rowSum[2] += sum1;                              \
		rowSum[3] += sum2;                              \
		rowSum[0] += sum3;                              \
		break;                                          \
	    case 2:                                             \
	    case 6:                                             \
		rowSum[2] += sum0;                              \
		rowSum[3] += sum1;                              \
		rowSum[0] += sum2;                              \
		rowSum[1] += sum3;                              \
		break;                                          \
	    case 3:                                             \
	    case 7:                                             \
		rowSum[3] += sum0;                              \
		rowSum[0] += sum1;                              \
		rowSum[1] += sum2;                              \
		rowSum[2] += sum3;                              \
		break;                                          \
	    }                                                   \
	    break;                                              \
	}

/* *********************************************************** */

#define	ROWRESULT3CH                                            \
	switch (alignSteps) {                                   \
	case 0:                                                 \
	case 3:                                                 \
	case 6:                                                 \
		rowSum[0] += sum0;                              \
		rowSum[1] += sum1;                              \
		rowSum[2] += sum2;                              \
		break;                                          \
	case 1:                                                 \
	case 4:                                                 \
	case 7:                                                 \
		rowSum[0] += sum2;                              \
		rowSum[1] += sum0;                              \
		rowSum[2] += sum1;                              \
		break;                                          \
	case 2:                                                 \
	case 5:                                                 \
		rowSum[0] += sum1;                              \
		rowSum[1] += sum2;                              \
		rowSum[2] += sum0;                              \
		break;                                          \
	}

/* *********************************************************** */

#define	ROWRESULT16                                             \
	switch (numOfChannels) {                                \
	case 1:                                                 \
	    rowSum[0] += (sum0 + sum1 + sum2 + sum3);           \
	    break;                                              \
	case 2:                                                 \
	    switch (alignBytes) {                               \
	    case 0:                                             \
	    case 4:                                             \
		rowSum[0] += (sum0 + sum2);                     \
		rowSum[1] += (sum1 + sum3);                     \
		break;                                          \
	    case 2:                                             \
	    case 6:                                             \
		rowSum[1] += (sum0 + sum2);                     \
		rowSum[0] += (sum1 + sum3);                     \
		break;                                          \
	    }                                                   \
	    break;                                              \
	case 4:                                                 \
	    switch (alignBytes) {                               \
	    case 0:                                             \
		rowSum[0] += sum0;                              \
		rowSum[1] += sum1;                              \
		rowSum[2] += sum2;                              \
		rowSum[3] += sum3;                              \
		break;                                          \
	    case 2:                                             \
		rowSum[0] += sum3;                              \
		rowSum[1] += sum0;                              \
		rowSum[2] += sum1;                              \
		rowSum[3] += sum2;                              \
		break;                                          \
	    case 4:                                             \
		rowSum[0] += sum2;                              \
		rowSum[1] += sum3;                              \
		rowSum[2] += sum0;                              \
		rowSum[3] += sum1;                              \
		break;                                          \
	    case 6:                                             \
		rowSum[0] += sum1;                              \
		rowSum[1] += sum2;                              \
		rowSum[2] += sum3;                              \
		rowSum[3] += sum0;                              \
		break;                                          \
	    }                                                   \
	    break;                                              \
	}

/* *********************************************************** */

#define	ROWRESULT163CH                                  	\
	switch (alignBytes) {                               	\
	case 0:                                             	\
	case 6:                                             	\
		rowSum[0] += sum0;                              \
		rowSum[1] += sum1;                              \
		rowSum[2] += sum2;                              \
		break;                                          \
	case 2:                                             	\
		rowSum[0] += sum2;                              \
		rowSum[1] += sum0;                              \
		rowSum[2] += sum1;                              \
		break;                                          \
	case 4:                                             	\
		rowSum[0] += sum1;                              \
		rowSum[1] += sum2;                              \
		rowSum[2] += sum0;                              \
		break;                                          \
	}

/* *********************************************************** */

#define	FINRESULT                                               \
	switch (numOfChannels) {                                \
	case 4:                                                 \
	    mean[3] = rowSum[3] / srcTotalPixNum;               \
	    mean[2] = rowSum[2] / srcTotalPixNum;               \
	case 2:                                                 \
	    mean[1] = rowSum[1] / srcTotalPixNum;               \
	case 1:                                                 \
	    mean[0] = rowSum[0] / srcTotalPixNum;               \
	    break;                                              \
	}

/* *********************************************************** */

#define	FINRESULT3CH                                    	\
	mean[0] = rowSum[0] / srcTotalPixNum;               	\
	mean[1] = rowSum[1] / srcTotalPixNum;               	\
	mean[2] = rowSum[2] / srcTotalPixNum

/* *********************************************************** */

#define	ADD2SUM                                                 \
	resUneven.db = vis_fand(firstAccumulator, mask);        \
	firstAccumulator =                                      \
	vis_faligndata(firstAccumulator, firstAccumulator);     \
	resEven.db = vis_fand(firstAccumulator, mask);          \
	sum0 += resEven.two_int.int0;                           \
	sum1 += resUneven.two_int.int0;                         \
	sum2 += resEven.two_int.int1;                           \
	sum0 += resUneven.two_int.int1;                         \
	resUneven.db = vis_fand(secondAccumulator, mask);       \
	secondAccumulator =                                     \
	vis_faligndata(secondAccumulator, secondAccumulator);   \
	resEven.db = vis_fand(secondAccumulator, mask);         \
	sum1 += resEven.two_int.int0;                           \
	sum2 += resUneven.two_int.int0;                         \
	sum0 += resEven.two_int.int1;                           \
	sum1 += resUneven.two_int.int1;                         \
	resUneven.db = vis_fand(thirdAccumulator, mask);        \
	thirdAccumulator =                                      \
	vis_faligndata(thirdAccumulator, thirdAccumulator);     \
	resEven.db = vis_fand(thirdAccumulator, mask);          \
	sum2 += resEven.two_int.int0;                           \
	sum0 += resUneven.two_int.int0;                         \
	sum1 += resEven.two_int.int1;                           \
	sum2 += resUneven.two_int.int1;                         \
	resUneven.db = vis_fand(forthAccumulator, mask);        \
	forthAccumulator =                                      \
	vis_faligndata(forthAccumulator, forthAccumulator);     \
	resEven.db = vis_fand(forthAccumulator, mask);          \
	sum0 += resEven.two_int.int0;                           \
	sum1 += resUneven.two_int.int0;                         \
	sum2 += resEven.two_int.int1;                           \
	sum0 += resUneven.two_int.int1;                         \
	resUneven.db = vis_fand(fifthAccumulator, mask);        \
	fifthAccumulator =                                      \
	vis_faligndata(fifthAccumulator, fifthAccumulator);     \
	resEven.db = vis_fand(fifthAccumulator, mask);          \
	sum1 += resEven.two_int.int0;                           \
	sum2 += resUneven.two_int.int0;                         \
	sum0 += resEven.two_int.int1;                           \
	sum1 += resUneven.two_int.int1;                         \
	resUneven.db = vis_fand(sixthAccumulator, mask);        \
	sixthAccumulator =                                      \
	vis_faligndata(sixthAccumulator, sixthAccumulator);     \
	resEven.db = vis_fand(sixthAccumulator, mask);          \
	sum2 += resEven.two_int.int0;                           \
	sum0 += resUneven.two_int.int0;                         \
	sum1 += resEven.two_int.int1;                           \
	sum2 += resUneven.two_int.int1

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
		mlib_v_ImageMeanU8Case(src, mean);
		return (MLIB_SUCCESS);

	case MLIB_USHORT:
		mlib_v_ImageMeanU16Case(src, mean);
		return (MLIB_SUCCESS);

/* handle MLIB_SHORT data type of image */
	case MLIB_SHORT:
		mlib_v_ImageMeanS16Case(src, mean);
		return (MLIB_SUCCESS);

/* handle MLIB_INT data type of image */
	case MLIB_INT:
		mlib_v_ImageMeanS32Case(src, mean);
		return (MLIB_SUCCESS);

/* discard any other data types */
	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

void
mlib_v_ImageMeanU8Case(
    const mlib_image *src,
    mlib_d64 *mean)
{
/* pointer for pixel of source */
	mlib_u8 *srcPixelPtr = (mlib_u8 *)mlib_ImageGetData(src);

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

/* indices */
	mlib_s32 i, j, k;
	mlib_d64 rowSum[4];

	mlib_u8 *srcLinePtr = srcPixelPtr;
	mlib_s32 alignSteps, byteNumber, loadsNumber, cycleNumber;
	mlib_d64 *srcLoadPtr;
	mlib_d64 srcLoad;
	mlib_d64 firstAccumulator, secondAccumulator;
	type_union_mlib_d64 resEven;
	type_union_mlib_d64 resUneven;
	mlib_d64 mask = vis_to_double_dup(MASK_0000FFFF);
	mlib_f32 scale = vis_to_float(256);
	mlib_d64 sum0, sum1, sum2, sum3;

	for (i = 0; i < 4; i++)
		rowSum[i] = 0.0;

	vis_alignaddr(0, 6);

	if (numOfChannels * srcWidth == srcYStride) {
		srcWidth = srcTotalPixNum;
		srcHeight = 1;
	}

	if (numOfChannels * srcWidth < 16) {
		for (i = 0; i < srcHeight; i++) {
			for (j = 0; j < srcWidth; j++) {
				for (k = 0; k < numOfChannels; k++)
					rowSum[k] += (mlib_s32)srcPixelPtr[i *
					    srcYStride + j * numOfChannels + k];
			}
		}

		for (k = 0; k < numOfChannels; k++)
			mean[k] = rowSum[k] / srcTotalPixNum;

		return;
	}

	if (numOfChannels != 3) {	/* 1, 2 or 4 channels */

		for (j = 0; j < srcHeight; j++) {
			sum0 = sum1 = sum2 = sum3 = 0.0;
			alignSteps = (-(mlib_addr)srcPixelPtr) & 7;
			byteNumber =
			    (srcWidth << (numOfChannels >> 1)) -
			    alignSteps;
			loadsNumber = byteNumber >> 3;
			cycleNumber = loadsNumber >> 8;
			loadsNumber &= MASK_000000FF;
			byteNumber &= 7;

			PROLOG;

			MAIN_CYCLE;

			TAIL_CYCLE;

			EPILOG;

			ROWRESULT;

			srcLinePtr += srcYStride;
			srcPixelPtr = srcLinePtr;
		}

		FINRESULT;

	} else {	/* 3 channels */
		mlib_d64 secondLoad, thirdLoad;
		mlib_d64 thirdAccumulator, forthAccumulator,
		    fifthAccumulator, sixthAccumulator;
		mlib_s32 tailCycleNumber;

		for (j = 0; j < srcHeight; j++) {
			sum0 = sum1 = sum2 = 0.0;
			alignSteps = (-(mlib_addr)srcPixelPtr) & 7;
			byteNumber = srcWidth * 3 - alignSteps;
			loadsNumber = byteNumber >> 3;
			cycleNumber = (loadsNumber >> 8) / 3;
			loadsNumber =
			    loadsNumber - 768 * cycleNumber;
			tailCycleNumber = loadsNumber / 3;
			loadsNumber =
			    loadsNumber - 3 * tailCycleNumber;
			byteNumber &= 7;

			if (alignSteps > 6)
				sum2 += (*srcPixelPtr++);
			if (alignSteps > 5)
				sum0 += (*srcPixelPtr++);
			if (alignSteps > 4)
				sum1 += (*srcPixelPtr++);
			if (alignSteps > 3)
				sum2 += (*srcPixelPtr++);
			if (alignSteps > 2)
				sum0 += (*srcPixelPtr++);
			if (alignSteps > 1)
				sum1 += (*srcPixelPtr++);
			if (alignSteps > 0)
				sum2 += (*srcPixelPtr++);

			srcLoadPtr = (mlib_d64 *)srcPixelPtr;

			for (i = 0; i < cycleNumber; i++) {

				firstAccumulator = secondAccumulator =
				thirdAccumulator = forthAccumulator =
				fifthAccumulator = sixthAccumulator =
					vis_fzero();

				for (k = 0; k < 256; k++) {
					srcLoad = (*srcLoadPtr++);
					secondLoad = (*srcLoadPtr++);
					thirdLoad = (*srcLoadPtr++);

					firstAccumulator = vis_fpadd16
					    (firstAccumulator, vis_fmul8x16al
					    (vis_read_hi(srcLoad), scale));
					secondAccumulator = vis_fpadd16
					    (secondAccumulator, vis_fmul8x16al
					    (vis_read_lo(srcLoad), scale));

					thirdAccumulator = vis_fpadd16
					    (thirdAccumulator, vis_fmul8x16al
					    (vis_read_hi(secondLoad), scale));
					forthAccumulator = vis_fpadd16
					    (forthAccumulator, vis_fmul8x16al
					    (vis_read_lo(secondLoad), scale));

					fifthAccumulator = vis_fpadd16
					    (fifthAccumulator, vis_fmul8x16al
					    (vis_read_hi(thirdLoad), scale));
					sixthAccumulator = vis_fpadd16
					    (sixthAccumulator, vis_fmul8x16al
					    (vis_read_lo(thirdLoad), scale));
				}

				ADD2SUM;
			}

			firstAccumulator = secondAccumulator =
			thirdAccumulator = forthAccumulator =
			fifthAccumulator = sixthAccumulator = vis_fzero();

			for (i = 0; i < tailCycleNumber; i++) {
				srcLoad = (*srcLoadPtr++);
				secondLoad = (*srcLoadPtr++);
				thirdLoad = (*srcLoadPtr++);

				firstAccumulator = vis_fpadd16(
				    firstAccumulator, vis_fmul8x16al(
				    vis_read_hi(srcLoad), scale));
				secondAccumulator = vis_fpadd16(
				    secondAccumulator, vis_fmul8x16al(
				    vis_read_lo(srcLoad), scale));

				thirdAccumulator = vis_fpadd16(thirdAccumulator,
				    vis_fmul8x16al(vis_read_hi(secondLoad),
				    scale));
				forthAccumulator = vis_fpadd16(forthAccumulator,
				    vis_fmul8x16al(vis_read_lo(secondLoad),
				    scale));

				fifthAccumulator = vis_fpadd16(fifthAccumulator,
				    vis_fmul8x16al(vis_read_hi(thirdLoad),
				    scale));
				sixthAccumulator = vis_fpadd16(sixthAccumulator,
				    vis_fmul8x16al(vis_read_lo(thirdLoad),
				    scale));
			}

			ADD2SUM;

			firstAccumulator = secondAccumulator =
			thirdAccumulator = forthAccumulator =
			fifthAccumulator = sixthAccumulator = vis_fzero();

			switch (loadsNumber) {
			case 2:
				srcLoad = (*srcLoadPtr++);
				secondLoad = (*srcLoadPtr++);

				firstAccumulator = vis_fpadd16(
				    firstAccumulator, vis_fmul8x16al(
				    vis_read_hi(srcLoad), scale));
				secondAccumulator = vis_fpadd16(
				    secondAccumulator, vis_fmul8x16al(
				    vis_read_lo(srcLoad), scale));

				thirdAccumulator = vis_fpadd16(thirdAccumulator,
				    vis_fmul8x16al(vis_read_hi(secondLoad),
				    scale));
				forthAccumulator = vis_fpadd16(forthAccumulator,
				    vis_fmul8x16al(vis_read_lo(secondLoad),
				    scale));
				break;

			case 1:
				srcLoad = (*srcLoadPtr++);

				firstAccumulator = vis_fpadd16(
				    firstAccumulator, vis_fmul8x16al(
				    vis_read_hi(srcLoad), scale));
				secondAccumulator = vis_fpadd16(
				    secondAccumulator, vis_fmul8x16al(
				    vis_read_lo(srcLoad), scale));
				break;
			}

			ADD2SUM;

			srcPixelPtr = (mlib_u8 *)srcLoadPtr;

			switch (alignSteps) {
			case 0:
			case 3:
			case 6:
				if (byteNumber > 6)
					sum2 += (*srcPixelPtr++);
				if (byteNumber > 5)
					sum0 += (*srcPixelPtr++);
				if (byteNumber > 4)
					sum1 += (*srcPixelPtr++);
				if (byteNumber > 3)
					sum2 += (*srcPixelPtr++);
				if (byteNumber > 2)
					sum0 += (*srcPixelPtr++);
				if (byteNumber > 1)
					sum1 += (*srcPixelPtr++);
				if (byteNumber > 0)
					sum2 += *srcPixelPtr;
				break;

			case 1:
			case 4:
			case 7:
				if (byteNumber > 6)
					sum1 += (*srcPixelPtr++);
				if (byteNumber > 5)
					sum2 += (*srcPixelPtr++);
				if (byteNumber > 4)
					sum0 += (*srcPixelPtr++);
				if (byteNumber > 3)
					sum1 += (*srcPixelPtr++);
				if (byteNumber > 2)
					sum2 += (*srcPixelPtr++);
				if (byteNumber > 1)
					sum0 += (*srcPixelPtr++);
				if (byteNumber > 0)
					sum1 += *srcPixelPtr;
				break;

			case 2:
			case 5:
				if (byteNumber > 6)
					sum0 += (*srcPixelPtr++);
				if (byteNumber > 5)
					sum1 += (*srcPixelPtr++);
				if (byteNumber > 4)
					sum2 += (*srcPixelPtr++);
				if (byteNumber > 3)
					sum0 += (*srcPixelPtr++);
				if (byteNumber > 2)
					sum1 += (*srcPixelPtr++);
				if (byteNumber > 1)
					sum2 += (*srcPixelPtr++);
				if (byteNumber > 0)
					sum0 += *srcPixelPtr;
				break;
			}

			ROWRESULT3CH;

			srcLinePtr += srcYStride;
			srcPixelPtr = srcLinePtr;
		}

		FINRESULT3CH;
	    }
}

/* *********************************************************** */

#define	ACC3CH                                                  \
	a1 = vis_fmuld8ulx16(scale, vis_read_hi(srcLoad));      \
	a2 = vis_fmuld8ulx16(scale, vis_read_lo(srcLoad));      \
	acc1 = vis_fpadd32(acc1, a1);                           \
	acc2 = vis_fpadd32(acc2, a2);                           \
	a1 = vis_fmuld8ulx16(scale, vis_read_hi(secondLoad));   \
	a2 = vis_fmuld8ulx16(scale, vis_read_lo(secondLoad));   \
	acc3 = vis_fpadd32(acc3, a1);                           \
	acc4 = vis_fpadd32(acc4, a2);                           \
	a1 = vis_fmuld8ulx16(scale, vis_read_hi(thirdLoad));    \
	a2 = vis_fmuld8ulx16(scale, vis_read_lo(thirdLoad));    \
	acc5 = vis_fpadd32(acc5, a1);                           \
	acc6 = vis_fpadd32(acc6, a2)

/* *********************************************************** */

#define	ACC3CH_U16                                              \
	srcLoad = vis_fxor(srcLoad, xorMask);                   \
	a1 = vis_fmuld8ulx16(scale, vis_read_hi(srcLoad));      \
	a2 = vis_fmuld8ulx16(scale, vis_read_lo(srcLoad));      \
	a1 = vis_fpadd32(a1, addMask);                          \
	a2 = vis_fpadd32(a2, addMask);                          \
	acc1 = vis_fpadd32(acc1, a1);                           \
	acc2 = vis_fpadd32(acc2, a2);                           \
	secondLoad = vis_fxor(secondLoad, xorMask);             \
	a1 = vis_fmuld8ulx16(scale, vis_read_hi(secondLoad));   \
	a2 = vis_fmuld8ulx16(scale, vis_read_lo(secondLoad));   \
	a1 = vis_fpadd32(a1, addMask);                          \
	a2 = vis_fpadd32(a2, addMask);                          \
	acc3 = vis_fpadd32(acc3, a1);                           \
	acc4 = vis_fpadd32(acc4, a2);                           \
	thirdLoad = vis_fxor(thirdLoad, xorMask);               \
	a1 = vis_fmuld8ulx16(scale, vis_read_hi(thirdLoad));    \
	a2 = vis_fmuld8ulx16(scale, vis_read_lo(thirdLoad));    \
	a1 = vis_fpadd32(a1, addMask);                          \
	a2 = vis_fpadd32(a2, addMask);                          \
	acc5 = vis_fpadd32(acc5, a1);                           \
	acc6 = vis_fpadd32(acc6, a2)

/* *********************************************************** */

#define	ACC3CH_U16L                                             \
	srcLoad = vis_fxor(srcLoad, xorMask);                   \
	a1 = vis_fmuld8ulx16(scale, vis_read_hi(srcLoad));      \
	a2 = vis_fmuld8ulx16(scale, vis_read_lo(srcLoad));      \
	acc1 = vis_fpadd32(acc1, a1);                           \
	acc2 = vis_fpadd32(acc2, a2);                           \
	secondLoad = vis_fxor(secondLoad, xorMask);             \
	a1 = vis_fmuld8ulx16(scale, vis_read_hi(secondLoad));   \
	a2 = vis_fmuld8ulx16(scale, vis_read_lo(secondLoad));   \
	acc3 = vis_fpadd32(acc3, a1);                           \
	acc4 = vis_fpadd32(acc4, a2);                           \
	thirdLoad = vis_fxor(thirdLoad, xorMask);               \
	a1 = vis_fmuld8ulx16(scale, vis_read_hi(thirdLoad));    \
	a2 = vis_fmuld8ulx16(scale, vis_read_lo(thirdLoad));    \
	acc5 = vis_fpadd32(acc5, a1);                           \
	acc6 = vis_fpadd32(acc6, a2)

/* *********************************************************** */

#define	SUM3CH                                                  \
	firstAccumulator.db = acc1;                             \
	secondAccumulator.db = acc2;                            \
	sum0 += firstAccumulator.two_int.int0;                  \
	sum1 += firstAccumulator.two_int.int1;                  \
	sum2 += secondAccumulator.two_int.int0;                 \
	sum0 += secondAccumulator.two_int.int1;                 \
	firstAccumulator.db = acc3;                             \
	secondAccumulator.db = acc4;                            \
	sum1 += firstAccumulator.two_int.int0;                  \
	sum2 += firstAccumulator.two_int.int1;                  \
	sum0 += secondAccumulator.two_int.int0;                 \
	sum1 += secondAccumulator.two_int.int1;                 \
	firstAccumulator.db = acc5;                             \
	secondAccumulator.db = acc6;                            \
	sum2 += firstAccumulator.two_int.int0;                  \
	sum0 += firstAccumulator.two_int.int1;                  \
	sum1 += secondAccumulator.two_int.int0;                 \
	sum2 += secondAccumulator.two_int.int1

/* *********************************************************** */

void
mlib_v_ImageMeanS16Case(
    const mlib_image *src,
    mlib_d64 *mean)
{
/* pointer for pixel of source */
	mlib_s16 *srcPixelPtr = (mlib_s16 *)mlib_ImageGetData(src);

/* width of source image */
	mlib_s32 srcWidth = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 srcHeight = mlib_ImageGetHeight(src);

/* total number of pixels */
	mlib_s32 srcTotalPixNum = srcHeight * srcWidth;

/* linestride = bytes to next row */
	mlib_s32 srcYStride = mlib_ImageGetStride(src) >> 1;

/* number of channels */
	mlib_s32 numOfChannels = mlib_ImageGetChannels(src);

/* indices */
	mlib_s32 i, j, k;

	mlib_d64 *srcLoadPtr;
	mlib_d64 srcLoad;
	mlib_d64 tmpArr[1];
	mlib_s32 lEdgeMask, rEdgeMask, alignBytes;

	mlib_f32 scale = vis_to_float(0x10001);

	mlib_d64 sum0, sum1, sum2, sum3;
	mlib_s32 pixelNumber, loadsNumber, cycleNumber;
	mlib_d64 rowSum[4];
	mlib_d64 a1, a2, acc1, acc2;

	type_union_mlib_d64 firstAccumulator;
	type_union_mlib_d64 secondAccumulator;

	for (i = 0; i < 4; i++)
		rowSum[i] = 0.0;

	if (numOfChannels * srcWidth == srcYStride) {
		srcWidth = srcTotalPixNum;
		srcHeight = 1;
	}

	if (numOfChannels * srcWidth < 8) {
		for (i = 0; i < srcHeight; i++) {
			for (j = 0; j < srcWidth; j++) {
				for (k = 0; k < numOfChannels; k++)
					rowSum[k] += (mlib_s32)srcPixelPtr[i *
					    srcYStride + j * numOfChannels + k];
			}
		}

		for (k = 0; k < numOfChannels; k++)
			mean[k] = rowSum[k] / srcTotalPixNum;

		return;
	}

	if (numOfChannels != 3) {	/* 1, 2 or 4 channels */

		for (j = 0; j < srcHeight; j++) {
			sum0 = sum1 = sum2 = sum3 = 0.0;
			alignBytes = (-(mlib_addr)srcPixelPtr) & 7;
			pixelNumber =
			    (2 * numOfChannels * srcWidth - alignBytes) >> 1;
			loadsNumber = pixelNumber >> 2;
			cycleNumber = loadsNumber >> 16;
			loadsNumber &= MASK_0000FFFF;
			pixelNumber &= 3;

			srcLoadPtr =
			    (mlib_d64 *)((mlib_addr)srcPixelPtr & (~7));

			if (alignBytes) {
				tmpArr[0] = vis_fzero();
				lEdgeMask = vis_edge16((void *)srcPixelPtr,
				    (void *)(srcPixelPtr + 8));
				srcLoad = (*srcLoadPtr++);
				vis_pst_16(srcLoad, tmpArr, lEdgeMask);
				srcLoad = tmpArr[0];

				firstAccumulator.db = vis_fmuld8ulx16(scale,
				    vis_read_hi(srcLoad));
				secondAccumulator.db = vis_fmuld8ulx16(scale,
				    vis_read_lo(srcLoad));

				sum0 = firstAccumulator.two_int.int0;
				sum1 = firstAccumulator.two_int.int1;
				sum2 = secondAccumulator.two_int.int0;
				sum3 = secondAccumulator.two_int.int1;
			}

			for (i = 0; i < cycleNumber; i++) {

				acc1 = acc2 = vis_fzero();

				for (k = 0; k < 65536; k++) {
					srcLoad = (*srcLoadPtr++);

					a1 = vis_fmuld8ulx16(scale,
					    vis_read_hi(srcLoad));
					a2 = vis_fmuld8ulx16(scale,
					    vis_read_lo(srcLoad));

					acc1 = vis_fpadd32(acc1, a1);
					acc2 = vis_fpadd32(acc2, a2);
				}

				firstAccumulator.db = acc1;
				secondAccumulator.db = acc2;

				sum0 += firstAccumulator.two_int.int0;
				sum1 += firstAccumulator.two_int.int1;
				sum2 += secondAccumulator.two_int.int0;
				sum3 += secondAccumulator.two_int.int1;
			}

			acc1 = acc2 = vis_fzero();

			for (i = 0; i < loadsNumber; i++) {
				srcLoad = (*srcLoadPtr++);

				a1 = vis_fmuld8ulx16(scale,
				    vis_read_hi(srcLoad));
				a2 = vis_fmuld8ulx16(scale,
				    vis_read_lo(srcLoad));

				acc1 = vis_fpadd32(acc1, a1);
				acc2 = vis_fpadd32(acc2, a2);
			}

			firstAccumulator.db = acc1;
			secondAccumulator.db = acc2;

			sum0 += firstAccumulator.two_int.int0;
			sum1 += firstAccumulator.two_int.int1;
			sum2 += secondAccumulator.two_int.int0;
			sum3 += secondAccumulator.two_int.int1;

			if (pixelNumber) {
				tmpArr[0] = vis_fzero();
				rEdgeMask = vis_edge16((void *) 0,
				    (void *)(pixelNumber * 2 - 2));
				srcLoad = *srcLoadPtr;
				vis_pst_16(srcLoad, tmpArr, rEdgeMask);
				srcLoad = tmpArr[0];

				firstAccumulator.db =
				    vis_fmuld8ulx16(scale,
				    vis_read_hi(srcLoad));
				secondAccumulator.db =
				    vis_fmuld8ulx16(scale,
				    vis_read_lo(srcLoad));

				sum0 += firstAccumulator.two_int.int0;
				sum1 += firstAccumulator.two_int.int1;
				sum2 += secondAccumulator.two_int.int0;
				sum3 += secondAccumulator.two_int.int1;
			}

			ROWRESULT16;

			srcPixelPtr += srcYStride;
		}

		FINRESULT;

	} else {	/* 3 channels */

		mlib_d64 secondLoad, thirdLoad;
		mlib_d64 acc3, acc4, acc5, acc6;
		mlib_s32 tailCycleNumber;

		for (j = 0; j < srcHeight; j++) {
			sum0 = sum1 = sum2 = 0.0;
			alignBytes = (-(mlib_addr)srcPixelPtr) & 7;
			pixelNumber = (srcWidth * 3 * 2 - alignBytes) >> 1;
			loadsNumber = pixelNumber >> 2;
			cycleNumber = (loadsNumber >> 16) / 3;
			loadsNumber = loadsNumber - 196608 * cycleNumber;
			tailCycleNumber = loadsNumber / 3;
			loadsNumber = loadsNumber - 3 * tailCycleNumber;
			pixelNumber &= 3;

			srcLoadPtr =
				(mlib_d64 *)((mlib_addr)srcPixelPtr & (~7));

			if (alignBytes) {
				tmpArr[0] = vis_fzero();
				lEdgeMask = vis_edge16((void *)srcPixelPtr,
					(void *)(srcPixelPtr + 8));
				srcLoad = (*srcLoadPtr++);
				vis_pst_16(srcLoad, tmpArr, lEdgeMask);
				srcLoad = tmpArr[0];

				firstAccumulator.db =
					vis_fmuld8ulx16(scale,
					vis_read_hi(srcLoad));
				secondAccumulator.db =
					vis_fmuld8ulx16(scale,
					vis_read_lo(srcLoad));

				sum0 += firstAccumulator.two_int.int1;
				sum1 += secondAccumulator.two_int.int0;
				sum2 += secondAccumulator.two_int.int1;
			}

			for (i = 0; i < cycleNumber; i++) {

				acc1 = acc2 = acc3 = acc4 = acc5 = acc6 =
					vis_fzero();

				for (k = 0; k < 65536; k++) {
					srcLoad = (*srcLoadPtr++);
					secondLoad = (*srcLoadPtr++);
					thirdLoad = (*srcLoadPtr++);

					ACC3CH;
				}

				SUM3CH;
			}

			acc1 = acc2 = acc3 = acc4 = acc5 = acc6 = vis_fzero();

			for (i = 0; i < tailCycleNumber; i++) {
				srcLoad = (*srcLoadPtr++);
				secondLoad = (*srcLoadPtr++);
				thirdLoad = (*srcLoadPtr++);

				ACC3CH;
			}

			SUM3CH;

			acc1 = acc2 = acc3 = acc4 = acc5 = acc6 = vis_fzero();

			srcLoad = secondLoad = thirdLoad = vis_fzero();
			tmpArr[0] = vis_fzero();

			switch (loadsNumber) {
			case 2:
				srcLoad = (*srcLoadPtr++);
				secondLoad = (*srcLoadPtr++);

				if (pixelNumber) {
					rEdgeMask = vis_edge16((void *) 0,
						(void *)(pixelNumber * 2 - 2));
					thirdLoad = *srcLoadPtr;
					vis_pst_16(thirdLoad, tmpArr,
						rEdgeMask);
					thirdLoad = tmpArr[0];
				}
				break;

			case 1:
				srcLoad = (*srcLoadPtr++);

				if (pixelNumber) {
					rEdgeMask = vis_edge16((void *) 0,
						(void *)(pixelNumber * 2 - 2));
					secondLoad = *srcLoadPtr;
					vis_pst_16(secondLoad, tmpArr,
						rEdgeMask);
					secondLoad = tmpArr[0];
				}
				break;

			case 0:
				if (pixelNumber) {
					rEdgeMask = vis_edge16((void *) 0,
						(void *)(pixelNumber * 2 - 2));
					srcLoad = *srcLoadPtr;
					vis_pst_16(srcLoad, tmpArr,
						rEdgeMask);
					srcLoad = tmpArr[0];
				}
				break;
			}

			ACC3CH;
			SUM3CH;

			ROWRESULT163CH;

			srcPixelPtr += srcYStride;
		}

		FINRESULT3CH;
	}
}

/* *********************************************************** */

void
mlib_v_ImageMeanU16Case(
    const mlib_image *src,
    mlib_d64 *mean)
{
/* pointer for pixel of source */
	mlib_u16 *srcPixelPtr = (mlib_u16 *)mlib_ImageGetData(src);

/* width of source image */
	mlib_s32 srcWidth = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 srcHeight = mlib_ImageGetHeight(src);

/* total number of pixels */
	mlib_s32 srcTotalPixNum = srcHeight * srcWidth;

/* linestride = bytes to next row */
	mlib_s32 srcYStride = mlib_ImageGetStride(src) >> 1;

/* number of channels */
	mlib_s32 numOfChannels = mlib_ImageGetChannels(src);

/* indices */
	mlib_s32 i, j, k;

	mlib_d64 *srcLoadPtr;
	mlib_d64 srcLoad;
	mlib_d64 xorMask = vis_to_double_dup(0x80008000);
	mlib_d64 addMask = vis_to_double_dup(0x00008000);
	mlib_d64 addMask2 = vis_to_double_dup(0x40000000);
	mlib_d64 addVal;
	mlib_d64 tmpArr[1];
	mlib_s32 lEdgeMask, rEdgeMask, alignBytes;

	mlib_f32 scale = vis_to_float(0x10001);

	mlib_d64 sum0, sum1, sum2, sum3;
	mlib_s32 pixelNumber, loadsNumber, cycleNumber;
	mlib_d64 rowSum[4];
	mlib_d64 a1, a2, acc1, acc2;

	type_union_mlib_d64 firstAccumulator;
	type_union_mlib_d64 secondAccumulator;

	for (i = 0; i < 4; i++)
		rowSum[i] = 0.0;

	if (numOfChannels * srcWidth == srcYStride) {
		srcWidth = srcTotalPixNum;
		srcHeight = 1;
	}

	if (numOfChannels * srcWidth < 8) {
		for (i = 0; i < srcHeight; i++) {
			for (j = 0; j < srcWidth; j++) {
				for (k = 0; k < numOfChannels; k++)
					rowSum[k] += (mlib_s32)srcPixelPtr[i *
					    srcYStride + j * numOfChannels + k];
			}
		}

		for (k = 0; k < numOfChannels; k++)
			mean[k] = rowSum[k] / srcTotalPixNum;

		return;
	}

	if (numOfChannels != 3) {	/* 1, 2 or 4 channels */

		for (j = 0; j < srcHeight; j++) {
			sum0 = sum1 = sum2 = sum3 = 0.0;
			alignBytes = (-(mlib_addr)srcPixelPtr) & 7;
			pixelNumber =
			    (2 * numOfChannels * srcWidth - alignBytes) >> 1;
			loadsNumber = pixelNumber >> 2;
			cycleNumber = loadsNumber >> 15;
			loadsNumber &= MASK_00007FFF;
			pixelNumber &= 3;

			srcLoadPtr =
			    (mlib_d64 *)((mlib_addr)srcPixelPtr & (~7));

			if (alignBytes) {
				tmpArr[0] = vis_fzero();
				lEdgeMask = vis_edge16((void *)srcPixelPtr,
				    (void *)(srcPixelPtr + 8));
				srcLoad = (*srcLoadPtr++);
				vis_pst_16(srcLoad, tmpArr, lEdgeMask);
				srcLoad = tmpArr[0];

				srcLoad = vis_fxor(srcLoad, xorMask);
				firstAccumulator.db =
				    vis_fmuld8ulx16(scale,
				    vis_read_hi(srcLoad));
				secondAccumulator.db =
				    vis_fmuld8ulx16(scale,
				    vis_read_lo(srcLoad));
				firstAccumulator.db =
				    vis_fpadd32(firstAccumulator.db, addMask);
				secondAccumulator.db =
				    vis_fpadd32(secondAccumulator.db, addMask);

				sum0 = firstAccumulator.two_int.int0;
				sum1 = firstAccumulator.two_int.int1;
				sum2 = secondAccumulator.two_int.int0;
				sum3 = secondAccumulator.two_int.int1;
			}

			for (i = 0; i < cycleNumber; i++) {

				acc1 = acc2 = vis_fzero();

				for (k = 0; k < 32768; k++) {
					srcLoad = (*srcLoadPtr++);

					srcLoad = vis_fxor(srcLoad, xorMask);
					a1 = vis_fmuld8ulx16(scale,
					    vis_read_hi(srcLoad));
					a2 = vis_fmuld8ulx16(scale,
					    vis_read_lo(srcLoad));

					acc1 = vis_fpadd32(acc1, a1);
					acc2 = vis_fpadd32(acc2, a2);
				}

				firstAccumulator.db =
					vis_fpadd32(acc1, addMask2);
				secondAccumulator.db =
					vis_fpadd32(acc2, addMask2);

				sum0 += firstAccumulator.two_int.int0;
				sum1 += firstAccumulator.two_int.int1;
				sum2 += secondAccumulator.two_int.int0;
				sum3 += secondAccumulator.two_int.int1;
			}

			acc1 = acc2 = vis_fzero();

			for (i = 0; i < loadsNumber; i++) {
				srcLoad = (*srcLoadPtr++);

				srcLoad = vis_fxor(srcLoad, xorMask);
				a1 = vis_fmuld8ulx16(scale,
				    vis_read_hi(srcLoad));
				a2 = vis_fmuld8ulx16(scale,
				    vis_read_lo(srcLoad));

				acc1 = vis_fpadd32(acc1, a1);
				acc2 = vis_fpadd32(acc2, a2);
			}

			firstAccumulator.db = acc1;
			secondAccumulator.db = acc2;

			addVal = (mlib_d64)(loadsNumber << 15);
			sum0 += firstAccumulator.two_int.int0;
			sum1 += firstAccumulator.two_int.int1;
			sum2 += secondAccumulator.two_int.int0;
			sum3 += secondAccumulator.two_int.int1;
			sum0 += addVal;
			sum1 += addVal;
			sum2 += addVal;
			sum3 += addVal;

			if (pixelNumber) {
				tmpArr[0] = vis_fzero();
				rEdgeMask = vis_edge16((void *) 0,
				    (void *)(pixelNumber * 2 - 2));
				srcLoad = *srcLoadPtr;
				vis_pst_16(srcLoad, tmpArr, rEdgeMask);
				srcLoad = tmpArr[0];

				srcLoad = vis_fxor(srcLoad, xorMask);
				firstAccumulator.db =
				    vis_fmuld8ulx16(scale,
				    vis_read_hi(srcLoad));
				secondAccumulator.db =
				    vis_fmuld8ulx16(scale,
				    vis_read_lo(srcLoad));
				firstAccumulator.db =
				    vis_fpadd32(firstAccumulator.db, addMask);
				secondAccumulator.db =
				    vis_fpadd32(secondAccumulator.db, addMask);

				sum0 += firstAccumulator.two_int.int0;
				sum1 += firstAccumulator.two_int.int1;
				sum2 += secondAccumulator.two_int.int0;
				sum3 += secondAccumulator.two_int.int1;
			}

			ROWRESULT16;

			srcPixelPtr += srcYStride;
		}

		FINRESULT;

	} else {	/* 3 channels */

		mlib_d64 secondLoad, thirdLoad;
		mlib_d64 acc3, acc4, acc5, acc6;
		mlib_s32 tailCycleNumber;

		for (j = 0; j < srcHeight; j++) {
			sum0 = sum1 = sum2 = 0.0;
			alignBytes = (-(mlib_addr)srcPixelPtr) & 7;
			pixelNumber = (srcWidth * 3 * 2 - alignBytes) >> 1;
			loadsNumber = pixelNumber >> 2;
			cycleNumber = (loadsNumber >> 15) / 3;
			loadsNumber = loadsNumber - 98304 * cycleNumber;
			tailCycleNumber = loadsNumber / 3;
			loadsNumber = loadsNumber - 3 * tailCycleNumber;
			pixelNumber &= 3;

			srcLoadPtr =
				(mlib_d64 *)((mlib_addr)srcPixelPtr & (~7));

			if (alignBytes) {
				tmpArr[0] = vis_fzero();
				lEdgeMask = vis_edge16((void *)srcPixelPtr,
					(void *)(srcPixelPtr + 8));
				srcLoad = (*srcLoadPtr++);
				vis_pst_16(srcLoad, tmpArr, lEdgeMask);
				srcLoad = tmpArr[0];

				srcLoad = vis_fxor(srcLoad, xorMask);
				firstAccumulator.db =
					vis_fmuld8ulx16(scale,
					vis_read_hi(srcLoad));
				secondAccumulator.db =
					vis_fmuld8ulx16(scale,
					vis_read_lo(srcLoad));
				firstAccumulator.db =
					vis_fpadd32(firstAccumulator.db,
					addMask);
				secondAccumulator.db =
					vis_fpadd32(secondAccumulator.db,
					addMask);

				sum0 += firstAccumulator.two_int.int1;
				sum1 += secondAccumulator.two_int.int0;
				sum2 += secondAccumulator.two_int.int1;
			}

			for (i = 0; i < cycleNumber; i++) {

				acc1 = acc2 = acc3 = acc4 = acc5 = acc6 =
					vis_fzero();

				for (k = 0; k < 32768; k++) {
					srcLoad = (*srcLoadPtr++);
					secondLoad = (*srcLoadPtr++);
					thirdLoad = (*srcLoadPtr++);

					ACC3CH_U16L;
				}
/* (mlib_d64)0x1, 0000, 0000UL */
				sum0 += 4294967296.;
				sum1 += 4294967296.;
				sum2 += 4294967296.;
				SUM3CH;

			}

			acc1 = acc2 = acc3 = acc4 = acc5 = acc6 = vis_fzero();

			for (i = 0; i < tailCycleNumber; i++) {
				srcLoad = (*srcLoadPtr++);
				secondLoad = (*srcLoadPtr++);
				thirdLoad = (*srcLoadPtr++);

				ACC3CH_U16L;
			}

			addVal = (mlib_d64)tailCycleNumber *131072;

			sum0 += addVal;
			sum1 += addVal;
			sum2 += addVal;
			SUM3CH;

			acc1 = acc2 = acc3 = acc4 = acc5 = acc6 = vis_fzero();

			srcLoad = secondLoad = thirdLoad = vis_fzero();
			tmpArr[0] = vis_fzero();

			switch (loadsNumber) {
			case 2:
				srcLoad = (*srcLoadPtr++);
				secondLoad = (*srcLoadPtr++);

				if (pixelNumber) {
					rEdgeMask = vis_edge16((void *) 0,
						(void *)(pixelNumber * 2 - 2));
					thirdLoad = *srcLoadPtr;
					vis_pst_16(thirdLoad, tmpArr,
						rEdgeMask);
					thirdLoad = tmpArr[0];
				}
				break;

			case 1:
				srcLoad = (*srcLoadPtr++);

				if (pixelNumber) {
					rEdgeMask = vis_edge16((void *) 0,
						(void *)(pixelNumber * 2 - 2));
					secondLoad = *srcLoadPtr;
					vis_pst_16(secondLoad, tmpArr,
						rEdgeMask);
					secondLoad = tmpArr[0];
				}
				break;

			case 0:
				if (pixelNumber) {
					rEdgeMask = vis_edge16((void *) 0,
						(void *)(pixelNumber * 2 - 2));
					srcLoad = *srcLoadPtr;
					vis_pst_16(srcLoad, tmpArr, rEdgeMask);
					srcLoad = tmpArr[0];
				}
				break;
			}

			ACC3CH_U16;
			SUM3CH;

			ROWRESULT163CH;

			srcPixelPtr += srcYStride;
		}

		FINRESULT3CH;
	}
}

/* *********************************************************** */

void
mlib_v_ImageMeanS32Case(
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
			    for (i = 0; i < cycleNumber; i++) {

				    hi = lo = 0;
#pragma pipeloop(0)
				    for (k = 0; k < 0x8000; k++) {
					    firstAccumulator +=
						(*srcPixelPtr++);
					    val = (*srcPixelPtr++);
					    hi += val >> 16;
					    lo += val & 0xffff;
				    }

				    firstAccumulator +=
					(mlib_d64)lo + (mlib_d64)hi *65536.;
			    }

			    hi = lo = 0;
#pragma pipeloop(0)
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

			    for (i = 0; i < cycleNumber; i++) {

				    hi = lo = 0;
#pragma pipeloop(0)
				    for (k = 0; k < 0x8000; k++) {
					    firstAccumulator +=
						(*srcPixelPtr++);
					    val = (*srcPixelPtr++);
					    hi += val >> 16;
					    lo += val & 0xffff;
				    }

				    secondAccumulator +=
					(mlib_d64)lo + (mlib_d64)hi *65536.;

			    }

			    hi = lo = 0;
#pragma pipeloop(0)
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

			    for (i = 0; i < cycleNumber; i++) {

				    hi = lo = 0;
#pragma pipeloop(0)
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
			    }

			    hi = lo = 0;
#pragma pipeloop(0)
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

			    for (i = 0; i < cycleNumber; i++) {

				    hi = lo = hi2 = lo2 = 0;
#pragma pipeloop(0)
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
			    }

			    hi = lo = hi2 = lo2 = 0;
#pragma pipeloop(0)
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
