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

#pragma ident	"@(#)mlib_v_ImageStdDev.c	9.3	07/11/05 SMI"

/*
 * FILENAME: mlib_ImageStdDev.c
 *
 * FUNCTION
 *      mlib_ImageStdDev - calculates image standard deviation
 *      for the input image
 *
 * SYNOPSIS
 *  mlib_status mlib_ImageStdDev(mlib_d64         *stdev,
 *                               const mlib_image *src,
 *                               const mlib_d64   *mean)
 *
 * ARGUMENT
 *  stdev  Pointer to standard deviation array
 *  src    Pointer to input image
 *  mean   Pointer to mean array
 *
 * DESCRIPTION
 *
 *
 *           1    w-1 h-1
 *      m = --- * SUM SUM x(i, j)
 *          w*h   i = 0 j = 0
 *
 *                  1    w-1 h-1         2
 *    s = sqrt (   --- * SUM SUM (x(i, j) - m)  )
 *                 w*h   i = 0 j = 0
 *
 * RESTRICTION
 *      The image can have 1, 2, 3 or 4 channels.
 *      It can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageMoment2.h>
#include <mlib_v_ImageStat.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageStdDev = __mlib_ImageStdDev

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageStdDev) mlib_ImageStdDev
    __attribute__((weak, alias("__mlib_ImageStdDev")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_v_ImageStdDev2ParamU8_124(
    const mlib_image *src,
    mlib_d64 *stdev);

static void mlib_v_ImageStdDev2ParamU8_3(
    const mlib_image *src,
    mlib_d64 *stdev);

static void mlib_v_ImageStdDev2ParamU16(
    const mlib_image *src,
    mlib_d64 *stdev);

static void mlib_v_ImageStdDev_Fast_U16(
    const mlib_image *src,
    mlib_d64 *stdev,
    const mlib_d64 *mean);

static void mlib_v_ImageStdDev2ParamS16(
    const mlib_image *src,
    mlib_d64 *stdev);

static void mlib_v_ImageStdDev2ParamS32(
    const mlib_image *src,
    mlib_d64 *stdev);

/* *********************************************************** */

mlib_status
__mlib_ImageStdDev(
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
		case MLIB_BYTE:

			if (num_channels != 3)
				mlib_v_ImageStdDev2ParamU8_124(src, stdev);
			else
				mlib_v_ImageStdDev2ParamU8_3(src, stdev);
			return (MLIB_SUCCESS);

		case MLIB_USHORT:
			mlib_v_ImageStdDev2ParamU16(src, stdev);
			return (MLIB_SUCCESS);

		case MLIB_SHORT:
			mlib_v_ImageStdDev2ParamS16(src, stdev);
			return (MLIB_SUCCESS);

		case MLIB_INT:
			mlib_v_ImageStdDev2ParamS32(src, stdev);
			return (MLIB_SUCCESS);

		default:
			return (MLIB_FAILURE);
		}
	}

	switch (mlib_ImageGetType(src)) {
	case MLIB_BYTE:

		if (num_channels != 3)
			mlib_v_ImageMoment2_U8_124(src, rez);
		else
			mlib_v_ImageMoment2_U8_3(src, rez);
		break;

	case MLIB_USHORT:
		mlib_v_ImageStdDev_Fast_U16(src, stdev, mean);
		return (MLIB_SUCCESS);

	case MLIB_SHORT:

		if (num_channels != 3)
			mlib_v_ImageMoment2_S16_124(src, rez);
		else
			mlib_v_ImageMoment2_S16_3(src, rez);
		break;

	case MLIB_INT:

		if (num_channels != 3)
			mlib_v_ImageMoment2_S32_124(src, rez);
		else
			mlib_v_ImageMoment2_S32_3(src, rez);
		break;

	default:
		return (MLIB_FAILURE);
	}

	divider =
	    ((mlib_d64)mlib_ImageGetWidth(src) * mlib_ImageGetHeight(src));
	switch (num_channels) {
	case 1:
		moment = (rez[0] + rez[1] + rez[2] + rez[3]) / divider;
		stdev[0] = mlib_sqrt(mlib_fabs(moment - mean[0] * mean[0]));
		break;

	case 2:
		moment = (rez[0] + rez[2]) / divider;
		stdev[0] = mlib_sqrt(mlib_fabs(moment - mean[0] * mean[0]));
		moment = (rez[1] + rez[3]) / divider;
		stdev[1] = mlib_sqrt(mlib_fabs(moment - mean[1] * mean[1]));
		break;

	case 4:
		moment = rez[3] / divider;
		stdev[3] = mlib_sqrt(mlib_fabs(moment - mean[3] * mean[3]));

	case 3:
		moment = rez[0] / divider;
		stdev[0] = mlib_sqrt(mlib_fabs(moment - mean[0] * mean[0]));
		moment = rez[1] / divider;
		stdev[1] = mlib_sqrt(mlib_fabs(moment - mean[1] * mean[1]));
		moment = rez[2] / divider;
		stdev[2] = mlib_sqrt(mlib_fabs(moment - mean[2] * mean[2]));
		break;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_v_ImageStdDev2ParamU8_124(
    const mlib_image *src,
    mlib_d64 *stdev)
{
/* pointer for pixel of source */
	mlib_u8 *srcPixelPtr = (mlib_u8 *)mlib_ImageGetData(src);

/* pointer for row of source */
	mlib_u8 *srcRowPtr = (mlib_u8 *)srcPixelPtr;

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
	mlib_s32 alignBytes;
	mlib_s32 tailBytes;
	mlib_s32 loadsNumber;
	mlib_s32 cycleNumber;

	mlib_s32 lEdgeMask, rEdgeMask;
	mlib_d64 tmpArr[1];
	mlib_d64 *srcLoadPtr;
	mlib_d64 srcLoad, loadHi, loadLo, mul01, mul23, mul45, mul67;
	mlib_d64 acc1 = vis_fzero();
	mlib_d64 acc2 = vis_fzero();
	mlib_d64 tmpAccumulator = vis_fzero();
	mlib_d64 firstAccumulator = vis_fzero();
	mlib_d64 secondAccumulator = vis_fzero();
	mlib_d64 sum0, sum1, sum2, sum3;
	mlib_d64 mtwo0, mtwo1, mtwo2, mtwo3;
	mlib_d64 rowMean[4], rowMTwo[4];
	mlib_d64 divider = 1.0 / (mlib_d64)srcTotalPixNum;
	mlib_f32 scale = vis_to_float(0x100);
	mlib_f32 mult = vis_to_float(0x10001);
	mlib_s32 byteWidth = srcWidth << (numOfChannels >> 1);
	type_union_mlib_d64 utmp0, utmp1;

	mtwo0 = mtwo1 = mtwo2 = mtwo3 = sum0 = sum1 = sum2 = sum3 = vis_fzero();

	for (i = 0; i < 4; i++)
		rowMean[i] = rowMTwo[i] = vis_fzero();

	if (byteWidth == srcYStride) {
		srcWidth = srcTotalPixNum;
		byteWidth = srcTotalPixNum << (numOfChannels >> 1);
		srcHeight = 1;
	}

	for (j = 0; j < srcHeight; j++) {
		tmpArr[0] = vis_fzero();
		alignBytes = (mlib_addr)srcPixelPtr & 7;
		tailBytes = byteWidth + alignBytes;
		loadsNumber = tailBytes >> 3;
		tailBytes &= 7;

		lEdgeMask = vis_edge8(srcPixelPtr, srcPixelPtr + byteWidth - 1);
		rEdgeMask = ~(0xff >> tailBytes);

		srcLoadPtr = (mlib_d64 *)(srcPixelPtr - alignBytes);
		alignBytes &= 3;

		srcLoad = *srcLoadPtr++;
		vis_pst_8(srcLoad, tmpArr, lEdgeMask);
		srcLoad = tmpArr[0];

		cycleNumber = loadsNumber >> 6;
		loadsNumber &= 63;

		acc1 = acc2 = firstAccumulator = vis_fzero();
		while (cycleNumber >= 0x8000 / 64) {
			cycleNumber -= 0x8000 / 64;
			for (i = 0; i < 0x8000 / 64; i++) {
				for (k = 0; k < 64; k++) {
					loadHi =
					    vis_fmul8x16al(vis_read_hi(srcLoad),
					    scale);
					loadLo =
					    vis_fmul8x16al(vis_read_lo(srcLoad),
					    scale);

					srcLoad = vis_ld_d64_nf(srcLoadPtr);
					srcLoadPtr++;
					tmpAccumulator =
					    vis_fpadd16(tmpAccumulator, loadHi);
					tmpAccumulator =
					    vis_fpadd16(tmpAccumulator, loadLo);

					mul01 =
					    vis_fmuld8ulx16(vis_read_hi(loadHi),
					    vis_read_hi(loadHi));
					mul23 =
					    vis_fmuld8ulx16(vis_read_lo(loadHi),
					    vis_read_lo(loadHi));
					mul45 =
					    vis_fmuld8ulx16(vis_read_hi(loadLo),
					    vis_read_hi(loadLo));
					mul67 =
					    vis_fmuld8ulx16(vis_read_lo(loadLo),
					    vis_read_lo(loadLo));
					acc1 = vis_fpadd32(acc1, mul01);
					acc2 = vis_fpadd32(acc2, mul23);
					acc1 = vis_fpadd32(acc1, mul45);
					acc2 = vis_fpadd32(acc2, mul67);
				}

				firstAccumulator = vis_fpadd32(firstAccumulator,
				    vis_fmuld8ulx16(mult,
				    vis_read_hi(tmpAccumulator)));
				secondAccumulator =
				    vis_fpadd32(secondAccumulator,
				    vis_fmuld8ulx16(mult,
				    vis_read_lo(tmpAccumulator)));

				tmpAccumulator = vis_fzero();
			}

			utmp0.db = acc1;
			utmp1.db = acc2;
			mtwo0 += utmp0.two_int.int0;
			mtwo1 += utmp0.two_int.int1;
			mtwo2 += utmp1.two_int.int0;
			mtwo3 += utmp1.two_int.int1;
			utmp0.db = firstAccumulator;
			utmp1.db = secondAccumulator;
			sum0 += utmp0.two_int.int0;
			sum1 += utmp0.two_int.int1;
			sum2 += utmp1.two_int.int0;
			sum3 += utmp1.two_int.int1;
			acc1 = acc2 = firstAccumulator = secondAccumulator =
			    vis_fzero();
		}

		for (i = 0; i < cycleNumber; i++) {
			for (k = 0; k < 64; k++) {
				loadHi =
				    vis_fmul8x16al(vis_read_hi(srcLoad), scale);
				loadLo =
				    vis_fmul8x16al(vis_read_lo(srcLoad), scale);

				srcLoad = vis_ld_d64_nf(srcLoadPtr);
				srcLoadPtr++;
				tmpAccumulator =
				    vis_fpadd16(tmpAccumulator, loadHi);
				tmpAccumulator =
				    vis_fpadd16(tmpAccumulator, loadLo);

				mul01 =
				    vis_fmuld8ulx16(vis_read_hi(loadHi),
				    vis_read_hi(loadHi));
				mul23 =
				    vis_fmuld8ulx16(vis_read_lo(loadHi),
				    vis_read_lo(loadHi));
				mul45 =
				    vis_fmuld8ulx16(vis_read_hi(loadLo),
				    vis_read_hi(loadLo));
				mul67 =
				    vis_fmuld8ulx16(vis_read_lo(loadLo),
				    vis_read_lo(loadLo));
				acc1 = vis_fpadd32(acc1, mul01);
				acc2 = vis_fpadd32(acc2, mul23);
				acc1 = vis_fpadd32(acc1, mul45);
				acc2 = vis_fpadd32(acc2, mul67);
			}

			firstAccumulator = vis_fpadd32(firstAccumulator,
			    vis_fmuld8ulx16(mult, vis_read_hi(tmpAccumulator)));
			secondAccumulator = vis_fpadd32(secondAccumulator,
			    vis_fmuld8ulx16(mult, vis_read_lo(tmpAccumulator)));

			tmpAccumulator = vis_fzero();
		}

		for (k = 0; k < loadsNumber; k++) {
			loadHi = vis_fmul8x16al(vis_read_hi(srcLoad), scale);
			loadLo = vis_fmul8x16al(vis_read_lo(srcLoad), scale);

			srcLoad = vis_ld_d64_nf(srcLoadPtr);
			srcLoadPtr++;
			tmpAccumulator = vis_fpadd16(tmpAccumulator, loadHi);
			tmpAccumulator = vis_fpadd16(tmpAccumulator, loadLo);

			mul01 =
			    vis_fmuld8ulx16(vis_read_hi(loadHi),
			    vis_read_hi(loadHi));
			mul23 =
			    vis_fmuld8ulx16(vis_read_lo(loadHi),
			    vis_read_lo(loadHi));
			mul45 =
			    vis_fmuld8ulx16(vis_read_hi(loadLo),
			    vis_read_hi(loadLo));
			mul67 =
			    vis_fmuld8ulx16(vis_read_lo(loadLo),
			    vis_read_lo(loadLo));
			acc1 = vis_fpadd32(acc1, mul01);
			acc2 = vis_fpadd32(acc2, mul23);
			acc1 = vis_fpadd32(acc1, mul45);
			acc2 = vis_fpadd32(acc2, mul67);
		}

		tmpArr[0] = vis_fzero();

		if (tailBytes) {
			vis_pst_8(srcLoad, tmpArr, rEdgeMask);
			srcLoad = tmpArr[0];
			loadHi = vis_fmul8x16al(vis_read_hi(srcLoad), scale);
			loadLo = vis_fmul8x16al(vis_read_lo(srcLoad), scale);

			tmpAccumulator = vis_fpadd16(tmpAccumulator, loadHi);
			tmpAccumulator = vis_fpadd16(tmpAccumulator, loadLo);

			mul01 =
			    vis_fmuld8ulx16(vis_read_hi(loadHi),
			    vis_read_hi(loadHi));
			mul23 =
			    vis_fmuld8ulx16(vis_read_lo(loadHi),
			    vis_read_lo(loadHi));
			mul45 =
			    vis_fmuld8ulx16(vis_read_hi(loadLo),
			    vis_read_hi(loadLo));
			mul67 =
			    vis_fmuld8ulx16(vis_read_lo(loadLo),
			    vis_read_lo(loadLo));
			acc1 = vis_fpadd32(acc1, mul01);
			acc2 = vis_fpadd32(acc2, mul23);
			acc1 = vis_fpadd32(acc1, mul45);
			acc2 = vis_fpadd32(acc2, mul67);
		}

		firstAccumulator = vis_fpadd32(firstAccumulator,
		    vis_fmuld8ulx16(mult, vis_read_hi(tmpAccumulator)));
		secondAccumulator = vis_fpadd32(secondAccumulator,
		    vis_fmuld8ulx16(mult, vis_read_lo(tmpAccumulator)));

		utmp0.db = acc1;
		utmp1.db = acc2;
		mtwo0 += utmp0.two_int.int0;
		mtwo1 += utmp0.two_int.int1;
		mtwo2 += utmp1.two_int.int0;
		mtwo3 += utmp1.two_int.int1;
		utmp0.db = firstAccumulator;
		utmp1.db = secondAccumulator;
		sum0 += utmp0.two_int.int0;
		sum1 += utmp0.two_int.int1;
		sum2 += utmp1.two_int.int0;
		sum3 += utmp1.two_int.int1;

		rowMTwo[3 & (0 - alignBytes)] += mtwo0;
		rowMTwo[3 & (1 - alignBytes)] += mtwo1;
		rowMTwo[3 & (2 - alignBytes)] += mtwo2;
		rowMTwo[(3 - alignBytes)] += mtwo3;
		rowMean[3 & (0 - alignBytes)] += sum0;
		rowMean[3 & (1 - alignBytes)] += sum1;
		rowMean[3 & (2 - alignBytes)] += sum2;
		rowMean[(3 - alignBytes)] += sum3;

		tmpAccumulator = acc1 = acc2 = firstAccumulator =
		    secondAccumulator = vis_fzero();
		mtwo0 = mtwo1 = mtwo2 = mtwo3 = sum0 = sum1 = sum2 = sum3 =
		    vis_fzero();

		srcRowPtr += srcYStride;
		srcPixelPtr = srcRowPtr;
	}

	switch (numOfChannels) {
	case 1:
		sum0 =
		    (rowMean[0] + rowMean[1] + rowMean[2] +
		    rowMean[3]) * divider;
		mtwo0 =
		    (rowMTwo[0] + rowMTwo[1] + rowMTwo[2] +
		    rowMTwo[3]) * divider;
		stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 - sum0 * sum0));
		break;

	case 2:
		sum0 = (rowMean[0] + rowMean[2]) * divider;
		mtwo0 = (rowMTwo[0] + rowMTwo[2]) * divider;
		stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 - sum0 * sum0));
		sum1 = (rowMean[1] + rowMean[3]) * divider;
		mtwo1 = (rowMTwo[1] + rowMTwo[3]) * divider;
		stdev[1] = mlib_sqrt(mlib_fabs(mtwo1 - sum1 * sum1));
		break;

	default:	/* only case 4: */
		sum0 = rowMean[0] * divider;
		sum1 = rowMean[1] * divider;
		sum2 = rowMean[2] * divider;
		sum3 = rowMean[3] * divider;
		mtwo0 = rowMTwo[0] * divider;
		mtwo1 = rowMTwo[1] * divider;
		mtwo2 = rowMTwo[2] * divider;
		mtwo3 = rowMTwo[3] * divider;
		stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 - sum0 * sum0));
		stdev[1] = mlib_sqrt(mlib_fabs(mtwo1 - sum1 * sum1));
		stdev[2] = mlib_sqrt(mlib_fabs(mtwo2 - sum2 * sum2));
		stdev[3] = mlib_sqrt(mlib_fabs(mtwo3 - sum3 * sum3));
		break;
	}
}

/* *********************************************************** */

void
mlib_v_ImageStdDev2ParamU8_3(
    const mlib_image *src,
    mlib_d64 *stdev)
{
/* pointer for pixel of source */
	mlib_u8 *srcPixelPtr = (mlib_u8 *)mlib_ImageGetData(src);

/* pointer for row of source */
	mlib_u8 *srcRowPtr = (mlib_u8 *)srcPixelPtr;

/* width of source image */
	mlib_s32 srcWidth = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 srcHeight = mlib_ImageGetHeight(src);

/* total number of pixels */
	mlib_s32 srcTotalPixNum = srcHeight * srcWidth;

/* linestride = bytes to next row */
	mlib_s32 srcYStride = mlib_ImageGetStride(src);

/* indices */
	mlib_s32 i, j, k;
	mlib_s32 alignBytes;
	mlib_s32 cycleNumber;

	mlib_s32 lEdgeMask, rEdgeMask;
	mlib_d64 tmpArr[2];
	mlib_f32 *srcLoadPtr;
	mlib_d64 srcLoad, loadHi, loadLo, mul01, mul23, mul45, mul67;
	mlib_d64 acc1 = vis_fzero();
	mlib_d64 acc2 = vis_fzero();
	mlib_d64 acc3 = vis_fzero();
	mlib_d64 tmp1Accumulator = vis_fzero();
	mlib_d64 tmp2Accumulator = vis_fzero();
	mlib_d64 tmp3Accumulator = vis_fzero();
	mlib_d64 firstAccumulator = vis_fzero();
	mlib_d64 secondAccumulator = vis_fzero();
	mlib_d64 thirdAccumulator = vis_fzero();
	mlib_d64 sum0, sum1, sum2;
	mlib_d64 mtwo0, mtwo1, mtwo2;
	mlib_d64 rowMean[3], rowMTwo[3];
	mlib_d64 divider = 1.0 / (mlib_d64)srcTotalPixNum;
	mlib_f32 scale = vis_to_float(0x100);
	mlib_f32 mult = vis_to_float(0x10001);
	mlib_s32 byteWidth = srcWidth * 3;
	type_union_mlib_d64 utmp0, utmp1, utmp2;

/* compute mean3_off(align, sumn) */
	const mlib_u8 al2off[12] = { 0, 2, 1, 0, 2, 1, 0, 2, 1, 0, 2, 1 };

	mtwo0 = mtwo1 = mtwo2 = sum0 = sum1 = sum2 = vis_fzero();

	for (i = 0; i < 3; i++)
		rowMean[i] = rowMTwo[i] = vis_fzero();

	if (byteWidth == srcYStride) {
		srcWidth = srcTotalPixNum;
		byteWidth = srcTotalPixNum * 3;
		srcHeight = 1;
	}

	for (j = 0; j < srcHeight; j++) {
		tmpArr[0] = vis_fzero();
		alignBytes = (mlib_addr)srcPixelPtr & 7;
		cycleNumber = byteWidth + alignBytes;

		lEdgeMask = vis_edge8(srcPixelPtr, srcPixelPtr + byteWidth - 1);

		srcLoadPtr = (mlib_f32 *)(srcPixelPtr - alignBytes);
		srcLoad = *(mlib_d64 *)srcLoadPtr;
		srcLoadPtr += 2;
		vis_pst_8(srcLoad, tmpArr, lEdgeMask);
		srcLoad = tmpArr[0];

		acc1 = acc2 = firstAccumulator = vis_fzero();
		for (; cycleNumber >= 3 * 0x8000; cycleNumber -= 3 * 0x8000) {
			for (i = 0; i < 0x8000 / 4 / 128; i++) {
				for (k = 0; k < 128; k++) {
					loadHi =
					    vis_fmul8x16al(vis_read_hi(srcLoad),
					    scale);
					loadLo =
					    vis_fmul8x16al(vis_read_lo(srcLoad),
					    scale);

					tmp1Accumulator =
					    vis_fpadd16(tmp1Accumulator,
					    loadHi);
					tmp2Accumulator =
					    vis_fpadd16(tmp2Accumulator,
					    loadLo);

					mul01 =
					    vis_fmuld8ulx16(vis_read_hi(loadHi),
					    vis_read_hi(loadHi));
					mul23 =
					    vis_fmuld8ulx16(vis_read_lo(loadHi),
					    vis_read_lo(loadHi));
					mul45 =
					    vis_fmuld8ulx16(vis_read_hi(loadLo),
					    vis_read_hi(loadLo));
					mul67 =
					    vis_fmuld8ulx16(vis_read_lo(loadLo),
					    vis_read_lo(loadLo));
					acc1 = vis_fpadd32(acc1, mul01);
					acc2 = vis_fpadd32(acc2, mul23);
					acc3 = vis_fpadd32(acc3, mul45);
					acc1 = vis_fpadd32(acc1, mul67);

					loadHi =
					    vis_fmul8x16al(*srcLoadPtr++,
					    scale);

					srcLoad =
					    vis_freg_pair(
					    vis_ld_f32_nf(srcLoadPtr),
					    vis_ld_f32_nf(srcLoadPtr + 1));
					srcLoadPtr += 2;
					tmp3Accumulator =
					    vis_fpadd16(tmp3Accumulator,
					    loadHi);

					mul01 =
					    vis_fmuld8ulx16(vis_read_hi(loadHi),
					    vis_read_hi(loadHi));
					mul23 =
					    vis_fmuld8ulx16(vis_read_lo(loadHi),
					    vis_read_lo(loadHi));
					acc2 = vis_fpadd32(acc2, mul01);
					acc3 = vis_fpadd32(acc3, mul23);
				}

				firstAccumulator = vis_fpadd32(firstAccumulator,
				    vis_fmuld8ulx16(mult,
				    vis_read_hi(tmp1Accumulator)));
				secondAccumulator =
				    vis_fpadd32(secondAccumulator,
				    vis_fmuld8ulx16(mult,
				    vis_read_lo(tmp1Accumulator)));
				thirdAccumulator =
				    vis_fpadd32(thirdAccumulator,
				    vis_fmuld8ulx16(mult,
				    vis_read_hi(tmp2Accumulator)));
				firstAccumulator =
				    vis_fpadd32(firstAccumulator,
				    vis_fmuld8ulx16(mult,
				    vis_read_lo(tmp2Accumulator)));
				secondAccumulator =
				    vis_fpadd32(secondAccumulator,
				    vis_fmuld8ulx16(mult,
				    vis_read_hi(tmp3Accumulator)));
				thirdAccumulator =
				    vis_fpadd32(thirdAccumulator,
				    vis_fmuld8ulx16(mult,
				    vis_read_lo(tmp3Accumulator)));

				tmp1Accumulator = tmp2Accumulator =
				    tmp3Accumulator = vis_fzero();
			}

			utmp0.db = acc1;
			utmp1.db = acc2;
			utmp2.db = acc3;
			utmp0.two_float.fl0 =
			    vis_fpadd32s(utmp0.two_float.fl0,
			    utmp1.two_float.fl1);
			utmp0.two_float.fl1 =
			    vis_fpadd32s(utmp0.two_float.fl1,
			    utmp2.two_float.fl0);
			utmp1.two_float.fl0 =
			    vis_fpadd32s(utmp1.two_float.fl0,
			    utmp2.two_float.fl1);
			mtwo0 += utmp0.two_int.int0;
			mtwo1 += utmp0.two_int.int1;
			mtwo2 += utmp1.two_int.int0;
			utmp0.db = firstAccumulator;
			utmp1.db = secondAccumulator;
			utmp2.db = thirdAccumulator;
			utmp0.two_float.fl0 =
			    vis_fpadd32s(utmp0.two_float.fl0,
			    utmp1.two_float.fl1);
			utmp0.two_float.fl1 =
			    vis_fpadd32s(utmp0.two_float.fl1,
			    utmp2.two_float.fl0);
			utmp1.two_float.fl0 =
			    vis_fpadd32s(utmp1.two_float.fl0,
			    utmp2.two_float.fl1);
			sum0 += utmp0.two_int.int0;
			sum1 += utmp0.two_int.int1;
			sum2 += utmp1.two_int.int0;
			firstAccumulator = secondAccumulator =
			    thirdAccumulator = acc1 = acc2 = acc3 = vis_fzero();
		}

		for (; cycleNumber >= 12 * 128; cycleNumber -= 12 * 128) {
			for (k = 0; k < 128; k++) {
				loadHi =
				    vis_fmul8x16al(vis_read_hi(srcLoad), scale);
				loadLo =
				    vis_fmul8x16al(vis_read_lo(srcLoad), scale);

				tmp1Accumulator =
				    vis_fpadd16(tmp1Accumulator, loadHi);
				tmp2Accumulator =
				    vis_fpadd16(tmp2Accumulator, loadLo);

				mul01 =
				    vis_fmuld8ulx16(vis_read_hi(loadHi),
				    vis_read_hi(loadHi));
				mul23 =
				    vis_fmuld8ulx16(vis_read_lo(loadHi),
				    vis_read_lo(loadHi));
				mul45 =
				    vis_fmuld8ulx16(vis_read_hi(loadLo),
				    vis_read_hi(loadLo));
				mul67 =
				    vis_fmuld8ulx16(vis_read_lo(loadLo),
				    vis_read_lo(loadLo));
				acc1 = vis_fpadd32(acc1, mul01);
				acc2 = vis_fpadd32(acc2, mul23);
				acc3 = vis_fpadd32(acc3, mul45);
				acc1 = vis_fpadd32(acc1, mul67);

				loadHi = vis_fmul8x16al(*srcLoadPtr++, scale);

				srcLoad =
				    vis_freg_pair(srcLoadPtr[0], srcLoadPtr[1]);
				srcLoadPtr += 2;
				tmp3Accumulator =
				    vis_fpadd16(tmp3Accumulator, loadHi);

				mul01 =
				    vis_fmuld8ulx16(vis_read_hi(loadHi),
				    vis_read_hi(loadHi));
				mul23 =
				    vis_fmuld8ulx16(vis_read_lo(loadHi),
				    vis_read_lo(loadHi));
				acc2 = vis_fpadd32(acc2, mul01);
				acc3 = vis_fpadd32(acc3, mul23);
			}

			firstAccumulator = vis_fpadd32(firstAccumulator,
			    vis_fmuld8ulx16(mult,
			    vis_read_hi(tmp1Accumulator)));
			secondAccumulator =
			    vis_fpadd32(secondAccumulator, vis_fmuld8ulx16(mult,
			    vis_read_lo(tmp1Accumulator)));
			thirdAccumulator =
			    vis_fpadd32(thirdAccumulator, vis_fmuld8ulx16(mult,
			    vis_read_hi(tmp2Accumulator)));
			firstAccumulator =
			    vis_fpadd32(firstAccumulator, vis_fmuld8ulx16(mult,
			    vis_read_lo(tmp2Accumulator)));
			secondAccumulator =
			    vis_fpadd32(secondAccumulator, vis_fmuld8ulx16(mult,
			    vis_read_hi(tmp3Accumulator)));
			thirdAccumulator =
			    vis_fpadd32(thirdAccumulator, vis_fmuld8ulx16(mult,
			    vis_read_lo(tmp3Accumulator)));

			tmp1Accumulator = tmp2Accumulator = tmp3Accumulator =
			    vis_fzero();
		}

		for (; cycleNumber >= 12; cycleNumber -= 12) {
			loadHi = vis_fmul8x16al(vis_read_hi(srcLoad), scale);
			loadLo = vis_fmul8x16al(vis_read_lo(srcLoad), scale);

			tmp1Accumulator = vis_fpadd16(tmp1Accumulator, loadHi);
			tmp2Accumulator = vis_fpadd16(tmp2Accumulator, loadLo);

			mul01 =
			    vis_fmuld8ulx16(vis_read_hi(loadHi),
			    vis_read_hi(loadHi));
			mul23 =
			    vis_fmuld8ulx16(vis_read_lo(loadHi),
			    vis_read_lo(loadHi));
			mul45 =
			    vis_fmuld8ulx16(vis_read_hi(loadLo),
			    vis_read_hi(loadLo));
			mul67 =
			    vis_fmuld8ulx16(vis_read_lo(loadLo),
			    vis_read_lo(loadLo));
			acc1 = vis_fpadd32(acc1, mul01);
			acc2 = vis_fpadd32(acc2, mul23);
			acc3 = vis_fpadd32(acc3, mul45);
			acc1 = vis_fpadd32(acc1, mul67);

			loadHi = vis_fmul8x16al(*srcLoadPtr++, scale);

			srcLoad = vis_freg_pair(vis_ld_f32_nf(srcLoadPtr),
				vis_ld_f32_nf(srcLoadPtr + 1));
			srcLoadPtr += 2;
			tmp3Accumulator = vis_fpadd16(tmp3Accumulator, loadHi);

			mul01 =
			    vis_fmuld8ulx16(vis_read_hi(loadHi),
			    vis_read_hi(loadHi));
			mul23 =
			    vis_fmuld8ulx16(vis_read_lo(loadHi),
			    vis_read_lo(loadHi));
			acc2 = vis_fpadd32(acc2, mul01);
			acc3 = vis_fpadd32(acc3, mul23);
		}

		tmpArr[0] = tmpArr[1] = vis_fzero();

		if (cycleNumber) {
			rEdgeMask = ~(0xfff >> cycleNumber);
			vis_pst_8(srcLoad, tmpArr, rEdgeMask >> 4);
			srcLoad = vis_write_lo(srcLoad,
				vis_ld_f32_nf(srcLoadPtr));
			vis_pst_8(srcLoad, tmpArr + 1, rEdgeMask);
			srcLoad = tmpArr[0];

			loadHi = vis_fmul8x16al(vis_read_hi(srcLoad), scale);
			loadLo = vis_fmul8x16al(vis_read_lo(srcLoad), scale);

			srcLoad = tmpArr[1];
			tmp1Accumulator = vis_fpadd16(tmp1Accumulator, loadHi);
			tmp2Accumulator = vis_fpadd16(tmp2Accumulator, loadLo);

			mul01 =
			    vis_fmuld8ulx16(vis_read_hi(loadHi),
			    vis_read_hi(loadHi));
			mul23 =
			    vis_fmuld8ulx16(vis_read_lo(loadHi),
			    vis_read_lo(loadHi));
			mul45 =
			    vis_fmuld8ulx16(vis_read_hi(loadLo),
			    vis_read_hi(loadLo));
			mul67 =
			    vis_fmuld8ulx16(vis_read_lo(loadLo),
			    vis_read_lo(loadLo));
			acc1 = vis_fpadd32(acc1, mul01);
			acc2 = vis_fpadd32(acc2, mul23);
			acc3 = vis_fpadd32(acc3, mul45);
			acc1 = vis_fpadd32(acc1, mul67);

			loadHi = vis_fmul8x16al(vis_read_lo(srcLoad), scale);

			tmp3Accumulator = vis_fpadd16(tmp3Accumulator, loadHi);

			mul01 =
			    vis_fmuld8ulx16(vis_read_hi(loadHi),
			    vis_read_hi(loadHi));
			mul23 =
			    vis_fmuld8ulx16(vis_read_lo(loadHi),
			    vis_read_lo(loadHi));
			acc2 = vis_fpadd32(acc2, mul01);
			acc3 = vis_fpadd32(acc3, mul23);
		}

		firstAccumulator = vis_fpadd32(firstAccumulator,
		    vis_fmuld8ulx16(mult, vis_read_hi(tmp1Accumulator)));
		secondAccumulator = vis_fpadd32(secondAccumulator,
		    vis_fmuld8ulx16(mult, vis_read_lo(tmp1Accumulator)));
		thirdAccumulator = vis_fpadd32(thirdAccumulator,
		    vis_fmuld8ulx16(mult, vis_read_hi(tmp2Accumulator)));
		firstAccumulator = vis_fpadd32(firstAccumulator,
		    vis_fmuld8ulx16(mult, vis_read_lo(tmp2Accumulator)));
		secondAccumulator = vis_fpadd32(secondAccumulator,
		    vis_fmuld8ulx16(mult, vis_read_hi(tmp3Accumulator)));
		thirdAccumulator = vis_fpadd32(thirdAccumulator,
		    vis_fmuld8ulx16(mult, vis_read_lo(tmp3Accumulator)));

		tmp1Accumulator = tmp2Accumulator = tmp3Accumulator =
		    vis_fzero();

		utmp0.db = acc1;
		utmp1.db = acc2;
		utmp2.db = acc3;
		utmp0.two_float.fl0 =
		    vis_fpadd32s(utmp0.two_float.fl0, utmp1.two_float.fl1);
		utmp0.two_float.fl1 =
		    vis_fpadd32s(utmp0.two_float.fl1, utmp2.two_float.fl0);
		utmp1.two_float.fl0 =
		    vis_fpadd32s(utmp1.two_float.fl0, utmp2.two_float.fl1);
		mtwo0 += utmp0.two_int.int0;
		mtwo1 += utmp0.two_int.int1;
		mtwo2 += utmp1.two_int.int0;
		utmp0.db = firstAccumulator;
		utmp1.db = secondAccumulator;
		utmp2.db = thirdAccumulator;
		utmp0.two_float.fl0 =
		    vis_fpadd32s(utmp0.two_float.fl0, utmp1.two_float.fl1);
		utmp0.two_float.fl1 =
		    vis_fpadd32s(utmp0.two_float.fl1, utmp2.two_float.fl0);
		utmp1.two_float.fl0 =
		    vis_fpadd32s(utmp1.two_float.fl0, utmp2.two_float.fl1);
		sum0 += utmp0.two_int.int0;
		sum1 += utmp0.two_int.int1;
		sum2 += utmp1.two_int.int0;
		firstAccumulator = secondAccumulator = thirdAccumulator = acc1 =
		    acc2 = acc3 = vis_fzero();

		rowMTwo[al2off[alignBytes + 0]] += mtwo0;
		rowMTwo[al2off[alignBytes + 2]] += mtwo1;
		rowMTwo[al2off[alignBytes + 1]] += mtwo2;
		rowMean[al2off[alignBytes + 0]] += sum0;
		rowMean[al2off[alignBytes + 2]] += sum1;
		rowMean[al2off[alignBytes + 1]] += sum2;

		mtwo0 = mtwo1 = mtwo2 = sum0 = sum1 = sum2 = vis_fzero();

		srcRowPtr += srcYStride;
		srcPixelPtr = srcRowPtr;
	}

	sum0 = rowMean[0] * divider;
	sum1 = rowMean[1] * divider;
	sum2 = rowMean[2] * divider;
	mtwo0 = rowMTwo[0] * divider;
	mtwo1 = rowMTwo[1] * divider;
	mtwo2 = rowMTwo[2] * divider;
	stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 - sum0 * sum0));
	stdev[1] = mlib_sqrt(mlib_fabs(mtwo1 - sum1 * sum1));
	stdev[2] = mlib_sqrt(mlib_fabs(mtwo2 - sum2 * sum2));
}

/* *********************************************************** */

#define	MUL3CH                                                             \
	dr1 =                                                              \
	vis_fmuld8sux16(vis_read_hi(srcLoad), vis_read_hi(srcLoad));       \
	dr2 =                                                              \
	vis_fmuld8ulx16(vis_read_hi(srcLoad), vis_read_hi(srcLoad));       \
	dr3 =                                                              \
	vis_fmuld8sux16(vis_read_lo(secondLoad),                           \
	    vis_read_lo(secondLoad));                                      \
	dr4 =                                                              \
	vis_fmuld8ulx16(vis_read_lo(secondLoad),                           \
	    vis_read_lo(secondLoad));                                      \
	dr1 = vis_fpadd32(dr1, mone);                                      \
/*                                                                         \
 * sub one                                                                 \
 */                                                                        \
	dr2 = vis_fpadd32(dr1, dr2);                                       \
	dr3 = vis_fpadd32(dr2, dr3);                                       \
	dc1.db = vis_fpadd32(dr3, dr4);                                    \
	mtwo0 += (mlib_d64)dc1.two_int.int0;                               \
	mtwo1 += (mlib_d64)dc1.two_int.int1;                               \
	dr1 =                                                              \
	vis_fmuld8sux16(vis_read_hi(secondLoad),                           \
	    vis_read_hi(secondLoad));                                      \
	dr2 =                                                              \
	vis_fmuld8ulx16(vis_read_hi(secondLoad),                           \
	    vis_read_hi(secondLoad));                                      \
	dr3 =                                                              \
	vis_fmuld8sux16(vis_read_lo(thirdLoad), vis_read_lo(thirdLoad));   \
	dr4 =                                                              \
	vis_fmuld8ulx16(vis_read_lo(thirdLoad), vis_read_lo(thirdLoad));   \
	dr1 = vis_fpadd32(dr1, mone);                                      \
/*                                                                         \
 * sub one                                                                 \
 */                                                                        \
	dr2 = vis_fpadd32(dr1, dr2);                                       \
	dr3 = vis_fpadd32(dr2, dr3);                                       \
	dc1.db = vis_fpadd32(dr3, dr4);                                    \
	mtwo1 += (mlib_d64)dc1.two_int.int0;                               \
	mtwo2 += (mlib_d64)dc1.two_int.int1;                               \
	dr1 =                                                              \
	vis_fmuld8sux16(vis_read_hi(thirdLoad), vis_read_hi(thirdLoad));   \
	dr2 =                                                              \
	vis_fmuld8ulx16(vis_read_hi(thirdLoad), vis_read_hi(thirdLoad));   \
	dr3 =                                                              \
	vis_fmuld8sux16(vis_read_lo(srcLoad), vis_read_lo(srcLoad));       \
	dr4 =                                                              \
	vis_fmuld8ulx16(vis_read_lo(srcLoad), vis_read_lo(srcLoad));       \
	dr1 = vis_fpadd32(dr1, mone);                                      \
/*                                                                         \
 * sub one                                                                 \
 */                                                                        \
	dr2 = vis_fpadd32(dr1, dr2);                                       \
	dr3 = vis_fpadd32(dr2, dr3);                                       \
	dc1.db = vis_fpadd32(dr3, dr4);                                    \
	mtwo2 += (mlib_d64)dc1.two_int.int0;                               \
	mtwo0 += (mlib_d64)dc1.two_int.int1

/* *********************************************************** */

#define	ACC3CH                                                             \
	acc1 =                                                             \
	vis_fpadd32(acc1, vis_fmuld8ulx16(scale, vis_read_hi(srcLoad)));   \
	acc1 =                                                             \
	vis_fpadd32(acc1, vis_fmuld8ulx16(scale,                           \
	    vis_read_lo(secondLoad)));                                     \
	acc2 =                                                             \
	vis_fpadd32(acc2, vis_fmuld8ulx16(scale, vis_read_lo(srcLoad)));   \
	acc2 =                                                             \
	vis_fpadd32(acc2, vis_fmuld8ulx16(scale,                           \
	    vis_read_hi(thirdLoad)));                                      \
	acc3 =                                                             \
	vis_fpadd32(acc3, vis_fmuld8ulx16(scale,                           \
	    vis_read_hi(secondLoad)));                                     \
	acc3 =                                                             \
	vis_fpadd32(acc3, vis_fmuld8ulx16(scale,                           \
	    vis_read_lo(thirdLoad)))

/* *********************************************************** */

#define	SUM3CH                                                  \
	firstAccumulator.db = acc1;                             \
	sum0 += firstAccumulator.two_int.int0;                  \
	sum1 += firstAccumulator.two_int.int1;                  \
	firstAccumulator.db = acc2;                             \
	sum2 += firstAccumulator.two_int.int0;                  \
	sum0 += firstAccumulator.two_int.int1;                  \
	firstAccumulator.db = acc3;                             \
	sum1 += firstAccumulator.two_int.int0;                  \
	sum2 += firstAccumulator.two_int.int1

/* *********************************************************** */

void
mlib_v_ImageStdDev2ParamU16(
    const mlib_image *src,
    mlib_d64 *stdev)
{
/* pointer for pixel of source */
	mlib_u16 *srcPixelPtr = (mlib_u16 *)mlib_ImageGetData(src);

/* pointer for row of source */
	mlib_u8 *srcRowPtr = (mlib_u8 *)srcPixelPtr;

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

	mlib_d64 *srcLoadPtr;
	mlib_d64 srcLoad;
	mlib_d64 tmpArr[3];
	mlib_s32 lEdgeMask, rEdgeMask;
	mlib_s32 alignBytes;

	mlib_f32 scale = vis_to_float(0x10001);

	mlib_d64 sum0 = 0.0, sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
	mlib_d64 mtwo0 = 0.0, mtwo1 = 0.0, mtwo2 = 0.0, mtwo3 = 0.0;
	mlib_s32 loadsNumber, cycleNumber;

	mlib_d64 dr1, dr2, dr3, dr4, a1, a2;
	mlib_d64 acc1 = vis_fzero(), acc2 = vis_fzero();
	mlib_d64 rowMean[4], rowMTwo[4];
	mlib_s32 byteWidth, tailBytes;
	mlib_d64 divider = (mlib_d64)srcTotalPixNum;

/* compute mean3_off(align, sumn) */
	const mlib_u8 al2off[6] = { 0, 2, 1, 0, 2, 1 };
	type_union_mlib_d64 dc1, dc2;
	type_union_mlib_d64 firstAccumulator;
	type_union_mlib_d64 secondAccumulator;
	mlib_d64 mone = vis_fone();
	mlib_s32 compens;
	mlib_d64 offset4 = vis_to_double_dup(0x80008000);

	firstAccumulator.db = secondAccumulator.db = vis_fzero();
	tmpArr[0] = vis_fzero();

	for (i = 0; i < 4; i++)
		rowMean[i] = rowMTwo[i] = vis_fzero();

	if (numOfChannels != 3) {

		byteWidth = srcWidth << (numOfChannels >> 1);

		if (2 * byteWidth == srcYStride) {
			srcWidth = srcTotalPixNum;
			byteWidth = srcTotalPixNum << (numOfChannels >> 1);
			srcHeight = 1;
		}

		for (j = 0; j < srcHeight; j++) {

			tmpArr[0] = vis_fzero();
			alignBytes = (mlib_addr)srcPixelPtr & 7;
			tailBytes = byteWidth + (alignBytes >> 1);
			loadsNumber = tailBytes >> 2;
			tailBytes &= 3;

			lEdgeMask =
			    vis_edge16(srcPixelPtr,
			    srcPixelPtr + byteWidth - 1);
			rEdgeMask = ~(0xf >> tailBytes);

			srcLoadPtr =
			    (mlib_d64 *)((mlib_u8 *)srcPixelPtr - alignBytes);
			alignBytes >>= 1;

			srcLoad = *srcLoadPtr++;
			srcLoad = vis_fxor(srcLoad, offset4);
			vis_pst_16(srcLoad, tmpArr, lEdgeMask);
			srcLoad = tmpArr[0];

			cycleNumber = 0;
			compens = (loadsNumber + 1) >> 1;

			while (loadsNumber > 0) {
				cycleNumber =
				    loadsNumber > 65536 ? 65536 : loadsNumber;
				loadsNumber -= cycleNumber;

				for (k = 0; k < (cycleNumber >> 1); k++) {

					dr1 =
					    vis_fmuld8sux16(vis_read_hi
					    (srcLoad), vis_read_hi(srcLoad));
					dr2 =
					    vis_fmuld8ulx16(vis_read_hi
					    (srcLoad), vis_read_hi(srcLoad));
					dr3 =
					    vis_fmuld8sux16(vis_read_lo
					    (srcLoad), vis_read_lo(srcLoad));
					dr4 =
					    vis_fmuld8ulx16(vis_read_lo
					    (srcLoad), vis_read_lo(srcLoad));
/* sub one */
					dr1 = vis_fpadd32(dr1, mone);
/* sub one */
					dr3 = vis_fpadd32(dr3, mone);
					dc1.db = vis_fpadd32(dr1, dr2);
					dc2.db = vis_fpadd32(dr3, dr4);

					a1 = vis_fmuld8ulx16(scale,
					    vis_read_hi(srcLoad));
					a2 = vis_fmuld8ulx16(scale,
					    vis_read_lo(srcLoad));
					srcLoad = *srcLoadPtr++;
					srcLoad = vis_fxor(srcLoad, offset4);

					acc1 = vis_fpadd32(acc1, a1);
					acc2 = vis_fpadd32(acc2, a2);

					dr1 =
					    vis_fmuld8sux16(vis_read_hi
					    (srcLoad), vis_read_hi(srcLoad));
					dr2 =
					    vis_fmuld8ulx16(vis_read_hi
					    (srcLoad), vis_read_hi(srcLoad));
					dr3 =
					    vis_fmuld8sux16(vis_read_lo
					    (srcLoad), vis_read_lo(srcLoad));
					dr4 =
					    vis_fmuld8ulx16(vis_read_lo
					    (srcLoad), vis_read_lo(srcLoad));
					dc1.db = vis_fpadd32(dc1.db, dr1);
					dc2.db = vis_fpadd32(dc2.db, dr3);
					dc1.db = vis_fpadd32(dc1.db, dr2);
					dc2.db = vis_fpadd32(dc2.db, dr4);

					mtwo0 += (mlib_d64)dc1.two_int.int0;
					mtwo1 += (mlib_d64)dc1.two_int.int1;
					mtwo2 += (mlib_d64)dc2.two_int.int0;
					mtwo3 += (mlib_d64)dc2.two_int.int1;

					a1 = vis_fmuld8ulx16(scale,
					    vis_read_hi(srcLoad));
					a2 = vis_fmuld8ulx16(scale,
					    vis_read_lo(srcLoad));
					srcLoad = vis_ld_d64_nf(srcLoadPtr);
					srcLoadPtr++;
					srcLoad = vis_fxor(srcLoad, offset4);

					acc1 = vis_fpadd32(acc1, a1);
					acc2 = vis_fpadd32(acc2, a2);
				}

				firstAccumulator.db = acc1;
				secondAccumulator.db = acc2;

				sum0 += firstAccumulator.two_int.int0;
				sum1 += firstAccumulator.two_int.int1;
				sum2 += secondAccumulator.two_int.int0;
				sum3 += secondAccumulator.two_int.int1;

				firstAccumulator.db = secondAccumulator.db =
				    acc1 = acc2 = vis_fzero();
			}

			tmpArr[0] = dc1.db = dc2.db = vis_fzero();

			if (cycleNumber & 1) {

				dr1 =
				    vis_fmuld8sux16(vis_read_hi(srcLoad),
				    vis_read_hi(srcLoad));
				dr2 =
				    vis_fmuld8ulx16(vis_read_hi(srcLoad),
				    vis_read_hi(srcLoad));
				dr3 =
				    vis_fmuld8sux16(vis_read_lo(srcLoad),
				    vis_read_lo(srcLoad));
				dr4 =
				    vis_fmuld8ulx16(vis_read_lo(srcLoad),
				    vis_read_lo(srcLoad));
/* sub one */
				dr1 = vis_fpadd32(dr1, mone);
/* sub one */
				dr3 = vis_fpadd32(dr3, mone);
				dc1.db = vis_fpadd32(dr1, dr2);
				dc2.db = vis_fpadd32(dr3, dr4);

				a1 = vis_fmuld8ulx16(scale,
				    vis_read_hi(srcLoad));
				a2 = vis_fmuld8ulx16(scale,
				    vis_read_lo(srcLoad));
				srcLoad = vis_ld_d64_nf(srcLoadPtr);
				srcLoadPtr++;
				srcLoad = vis_fxor(srcLoad, offset4);

				acc1 = vis_fpadd32(acc1, a1);
				acc2 = vis_fpadd32(acc2, a2);
			}

			if (tailBytes) {
				vis_pst_16(srcLoad, tmpArr, rEdgeMask);
				srcLoad = tmpArr[0];

				dr1 =
				    vis_fmuld8sux16(vis_read_hi(srcLoad),
				    vis_read_hi(srcLoad));
				dr2 =
				    vis_fmuld8ulx16(vis_read_hi(srcLoad),
				    vis_read_hi(srcLoad));
				dr3 =
				    vis_fmuld8sux16(vis_read_lo(srcLoad),
				    vis_read_lo(srcLoad));
				dr4 =
				    vis_fmuld8ulx16(vis_read_lo(srcLoad),
				    vis_read_lo(srcLoad));
				dc1.db = vis_fpadd32(dc1.db, dr1);
				dc2.db = vis_fpadd32(dc2.db, dr3);
				dc1.db = vis_fpadd32(dc1.db, dr2);
				dc2.db = vis_fpadd32(dc2.db, dr4);

				a1 = vis_fmuld8ulx16(scale,
				    vis_read_hi(srcLoad));
				a2 = vis_fmuld8ulx16(scale,
				    vis_read_lo(srcLoad));
				acc1 = vis_fpadd32(acc1, a1);
				acc2 = vis_fpadd32(acc2, a2);
			}

			mtwo0 += (mlib_d64)dc1.two_int.int0;
			mtwo1 += (mlib_d64)dc1.two_int.int1;
			mtwo2 += (mlib_d64)dc2.two_int.int0;
			mtwo3 += (mlib_d64)dc2.two_int.int1;

			firstAccumulator.db = acc1;
			secondAccumulator.db = acc2;

			sum0 += firstAccumulator.two_int.int0;
			sum1 += firstAccumulator.two_int.int1;
			sum2 += secondAccumulator.two_int.int0;
			sum3 += secondAccumulator.two_int.int1;

			firstAccumulator.db = secondAccumulator.db = acc1 =
			    acc2 = vis_fzero();
			tmpArr[0] = vis_fzero();

			rowMTwo[3 & (0 - alignBytes)] +=
			    mtwo0 + (mlib_d64)compens;
			rowMTwo[3 & (1 - alignBytes)] +=
			    mtwo1 + (mlib_d64)compens;
			rowMTwo[3 & (2 - alignBytes)] +=
			    mtwo2 + (mlib_d64)compens;
			rowMTwo[(3 - alignBytes)] += mtwo3 + (mlib_d64)compens;
			rowMean[3 & (0 - alignBytes)] += sum0;
			rowMean[3 & (1 - alignBytes)] += sum1;
			rowMean[3 & (2 - alignBytes)] += sum2;
			rowMean[(3 - alignBytes)] += sum3;

			mtwo0 = mtwo1 = mtwo2 = mtwo3 = sum0 = sum1 = sum2 =
			    sum3 = vis_fzero();

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_u16 *)srcRowPtr;
		}
	} else {
/* case 3: */

		mlib_d64 secondLoad, thirdLoad;
		mlib_d64 acc3;

		byteWidth = srcWidth * 3;

		if (2 * byteWidth == srcYStride) {
			srcWidth = srcTotalPixNum;
			byteWidth = srcTotalPixNum * 3;
			srcHeight = 1;
		}

		acc3 = vis_fzero();

		for (j = 0; j < srcHeight; j++) {
			alignBytes = (mlib_addr)srcPixelPtr & 7;
			tailBytes = byteWidth + (alignBytes >> 1);

			lEdgeMask =
			    vis_edge16(srcPixelPtr,
			    srcPixelPtr + byteWidth - 1);

			srcLoadPtr =
			    (mlib_d64 *)((mlib_u8 *)srcPixelPtr - alignBytes);
			srcLoad = *srcLoadPtr++;
			secondLoad = vis_ld_d64_nf(srcLoadPtr);
			srcLoadPtr++;
			srcLoad = vis_fxor(srcLoad, offset4);
			secondLoad = vis_fxor(secondLoad, offset4);
			vis_pst_16(srcLoad, tmpArr, lEdgeMask);
			srcLoad = tmpArr[0];
			alignBytes >>= 1;
			loadsNumber = tailBytes / 12;
			tailBytes -= loadsNumber * 12;
			compens = loadsNumber << 1;

			for (; loadsNumber > 0; ) {
				cycleNumber =
				    loadsNumber > 0x8000 ? 0x8000 : loadsNumber;
				loadsNumber -= cycleNumber;

#pragma pipeloop(0)
				for (k = 0; k < cycleNumber - 1; k++) {
					thirdLoad = *srcLoadPtr++;
					thirdLoad =
					    vis_fxor(thirdLoad, offset4);

					MUL3CH;
					ACC3CH;
					srcLoad = *srcLoadPtr++;

					secondLoad = *srcLoadPtr++;
					srcLoad = vis_fxor(srcLoad, offset4);
					secondLoad =
					    vis_fxor(secondLoad, offset4);
				}

				if (k < cycleNumber) {
					thirdLoad = vis_ld_d64_nf(srcLoadPtr);
					srcLoadPtr++;
					thirdLoad =
					    vis_fxor(thirdLoad, offset4);

					MUL3CH;
					ACC3CH;
					srcLoad = vis_ld_d64_nf(srcLoadPtr);
					srcLoadPtr++;

					secondLoad = vis_ld_d64_nf(srcLoadPtr);
					srcLoadPtr++;
					srcLoad = vis_fxor(srcLoad, offset4);
					secondLoad =
					    vis_fxor(secondLoad, offset4);
				}

				SUM3CH;
				acc1 = acc2 = acc3 = vis_fzero();
			}

			tmpArr[0] = tmpArr[1] = tmpArr[2] = vis_fzero();

			if (tailBytes) {
				rEdgeMask = ~(0xfff >> tailBytes);
				thirdLoad = vis_ld_d64_nf(srcLoadPtr);
				srcLoadPtr++;
				thirdLoad = vis_fxor(thirdLoad, offset4);
				vis_pst_16(srcLoad, tmpArr, rEdgeMask >> 8);
				vis_pst_16(secondLoad, tmpArr + 1,
				    rEdgeMask >> 4);
				vis_pst_16(thirdLoad, tmpArr + 2, rEdgeMask);
				srcLoad = tmpArr[0];
				secondLoad = tmpArr[1];
				thirdLoad = tmpArr[2];

				MUL3CH;
				ACC3CH;
				SUM3CH;
				acc1 = acc2 = acc3 = vis_fzero();

				tmpArr[0] = vis_fzero();
				compens += 2;
			}

			rowMTwo[al2off[alignBytes + 0]] +=
			    mtwo0 + (mlib_d64)compens;
			rowMTwo[al2off[alignBytes + 2]] +=
			    mtwo1 + (mlib_d64)compens;
			rowMTwo[al2off[alignBytes + 1]] +=
			    mtwo2 + (mlib_d64)compens;
			rowMean[al2off[alignBytes + 0]] += sum0;
			rowMean[al2off[alignBytes + 2]] += sum1;
			rowMean[al2off[alignBytes + 1]] += sum2;
			mtwo0 = mtwo1 = mtwo2 = sum0 = sum1 = sum2 =
			    vis_fzero();

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_u16 *)srcRowPtr;
		}
	}

	switch (numOfChannels) {
	case 1:
		sum0 =
		    (rowMean[0] + rowMean[1] + rowMean[2] +
		    rowMean[3]) / divider;
		mtwo0 =
		    (rowMTwo[0] + rowMTwo[1] + rowMTwo[2] +
		    rowMTwo[3]) / divider;
		stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 - sum0 * sum0));
		break;

	case 2:
		sum0 = (rowMean[0] + rowMean[2]) / divider;
		mtwo0 = (rowMTwo[0] + rowMTwo[2]) / divider;
		stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 - sum0 * sum0));
		sum1 = (rowMean[1] + rowMean[3]) / divider;
		mtwo1 = (rowMTwo[1] + rowMTwo[3]) / divider;
		stdev[1] = mlib_sqrt(mlib_fabs(mtwo1 - sum1 * sum1));
		break;

	case 3:
		sum0 = rowMean[0] / divider;
		sum1 = rowMean[1] / divider;
		sum2 = rowMean[2] / divider;
		mtwo0 = rowMTwo[0] / divider;
		mtwo1 = rowMTwo[1] / divider;
		mtwo2 = rowMTwo[2] / divider;
		stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 - sum0 * sum0));
		stdev[1] = mlib_sqrt(mlib_fabs(mtwo1 - sum1 * sum1));
		stdev[2] = mlib_sqrt(mlib_fabs(mtwo2 - sum2 * sum2));
		break;

	default:	/* only case 4: */
		sum0 = rowMean[0] / divider;
		sum1 = rowMean[1] / divider;
		sum2 = rowMean[2] / divider;
		sum3 = rowMean[3] / divider;
		mtwo0 = rowMTwo[0] / divider;
		mtwo1 = rowMTwo[1] / divider;
		mtwo2 = rowMTwo[2] / divider;
		mtwo3 = rowMTwo[3] / divider;
		stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 - sum0 * sum0));
		stdev[1] = mlib_sqrt(mlib_fabs(mtwo1 - sum1 * sum1));
		stdev[2] = mlib_sqrt(mlib_fabs(mtwo2 - sum2 * sum2));
		stdev[3] = mlib_sqrt(mlib_fabs(mtwo3 - sum3 * sum3));
		break;
	}
}

/* *********************************************************** */

void
mlib_v_ImageStdDev_Fast_U16(
    const mlib_image *src,
    mlib_d64 *stdev,
    const mlib_d64 *mean)
{
/* pointer for pixel of source */
	mlib_u16 *srcPixelPtr = (mlib_u16 *)mlib_ImageGetData(src);

/* pointer for row of source */
	mlib_u8 *srcRowPtr = (mlib_u8 *)srcPixelPtr;

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

	mlib_d64 *srcLoadPtr;
	mlib_d64 srcLoad;
	mlib_d64 tmpArr[3];
	mlib_s32 lEdgeMask, rEdgeMask;
	mlib_s32 alignBytes;

	mlib_d64 sum0, sum1, sum2, sum3;
	mlib_d64 mtwo0 = 0.0, mtwo1 = 0.0, mtwo2 = 0.0, mtwo3 = 0.0;
	mlib_s32 loadsNumber, cycleNumber;

	mlib_d64 dr1, dr2, dr3, dr4;
	mlib_d64 rowMTwo[4];
	mlib_s32 byteWidth, tailBytes;
	mlib_d64 divider = (mlib_d64)srcTotalPixNum;

/* compute mean3_off(align, sumn) */
	const mlib_u8 al2off[6] = { 0, 2, 1, 0, 2, 1 };
	type_union_mlib_d64 dc1, dc2;
	mlib_d64 mone = vis_fone();
	mlib_s32 compens;
	mlib_d64 offset4 = vis_to_double_dup(0x80008000);

	tmpArr[0] = vis_fzero();

	for (i = 0; i < 4; i++)
		rowMTwo[i] = vis_fzero();

	if (numOfChannels != 3) {

		byteWidth = srcWidth << (numOfChannels >> 1);

		if (2 * byteWidth == srcYStride) {
			srcWidth = srcTotalPixNum;
			byteWidth = srcTotalPixNum << (numOfChannels >> 1);
			srcHeight = 1;
		}

		for (j = 0; j < srcHeight; j++) {

			tmpArr[0] = vis_fzero();
			alignBytes = (mlib_addr)srcPixelPtr & 7;
			tailBytes = byteWidth + (alignBytes >> 1);
			loadsNumber = tailBytes >> 2;
			tailBytes &= 3;

			lEdgeMask =
			    vis_edge16(srcPixelPtr,
			    srcPixelPtr + byteWidth - 1);
			rEdgeMask = ~(0xf >> tailBytes);

			srcLoadPtr =
			    (mlib_d64 *)((mlib_u8 *)srcPixelPtr - alignBytes);
			alignBytes >>= 1;

			srcLoad = *srcLoadPtr++;
			srcLoad = vis_fxor(srcLoad, offset4);
			vis_pst_16(srcLoad, tmpArr, lEdgeMask);
			srcLoad = tmpArr[0];

			cycleNumber = 0;
			compens = (loadsNumber + 1) >> 1;

			while (loadsNumber > 0) {
				cycleNumber =
				    loadsNumber > 65536 ? 65536 : loadsNumber;
				loadsNumber -= cycleNumber;

				for (k = 0; k < (cycleNumber >> 1); k++) {

					dr1 =
					    vis_fmuld8sux16(vis_read_hi
					    (srcLoad), vis_read_hi(srcLoad));
					dr2 =
					    vis_fmuld8ulx16(vis_read_hi
					    (srcLoad), vis_read_hi(srcLoad));
					dr3 =
					    vis_fmuld8sux16(vis_read_lo
					    (srcLoad), vis_read_lo(srcLoad));
					dr4 =
					    vis_fmuld8ulx16(vis_read_lo
					    (srcLoad), vis_read_lo(srcLoad));
/* sub one */
					dr1 = vis_fpadd32(dr1, mone);
/* sub one */
					dr3 = vis_fpadd32(dr3, mone);
					dc1.db = vis_fpadd32(dr1, dr2);
					dc2.db = vis_fpadd32(dr3, dr4);

					srcLoad = *srcLoadPtr++;
					srcLoad = vis_fxor(srcLoad, offset4);

					dr1 =
					    vis_fmuld8sux16(vis_read_hi
					    (srcLoad), vis_read_hi(srcLoad));
					dr2 =
					    vis_fmuld8ulx16(vis_read_hi
					    (srcLoad), vis_read_hi(srcLoad));
					dr3 =
					    vis_fmuld8sux16(vis_read_lo
					    (srcLoad), vis_read_lo(srcLoad));
					dr4 =
					    vis_fmuld8ulx16(vis_read_lo
					    (srcLoad), vis_read_lo(srcLoad));
					dc1.db = vis_fpadd32(dc1.db, dr1);
					dc2.db = vis_fpadd32(dc2.db, dr3);
					dc1.db = vis_fpadd32(dc1.db, dr2);
					dc2.db = vis_fpadd32(dc2.db, dr4);

					mtwo0 += (mlib_d64)dc1.two_int.int0;
					mtwo1 += (mlib_d64)dc1.two_int.int1;
					mtwo2 += (mlib_d64)dc2.two_int.int0;
					mtwo3 += (mlib_d64)dc2.two_int.int1;

					srcLoad = vis_ld_d64_nf(srcLoadPtr);
					srcLoadPtr++;
					srcLoad = vis_fxor(srcLoad, offset4);
				}
			}

			tmpArr[0] = dc1.db = dc2.db = vis_fzero();

			if (cycleNumber & 1) {

				dr1 =
				    vis_fmuld8sux16(vis_read_hi(srcLoad),
				    vis_read_hi(srcLoad));
				dr2 =
				    vis_fmuld8ulx16(vis_read_hi(srcLoad),
				    vis_read_hi(srcLoad));
				dr3 =
				    vis_fmuld8sux16(vis_read_lo(srcLoad),
				    vis_read_lo(srcLoad));
				dr4 =
				    vis_fmuld8ulx16(vis_read_lo(srcLoad),
				    vis_read_lo(srcLoad));
/* sub one */
				dr1 = vis_fpadd32(dr1, mone);
/* sub one */
				dr3 = vis_fpadd32(dr3, mone);
				dc1.db = vis_fpadd32(dr1, dr2);
				dc2.db = vis_fpadd32(dr3, dr4);

				srcLoad = vis_ld_d64_nf(srcLoadPtr);
				srcLoadPtr++;
				srcLoad = vis_fxor(srcLoad, offset4);
			}

			if (tailBytes) {
				vis_pst_16(srcLoad, tmpArr, rEdgeMask);
				srcLoad = tmpArr[0];

				dr1 =
				    vis_fmuld8sux16(vis_read_hi(srcLoad),
				    vis_read_hi(srcLoad));
				dr2 =
				    vis_fmuld8ulx16(vis_read_hi(srcLoad),
				    vis_read_hi(srcLoad));
				dr3 =
				    vis_fmuld8sux16(vis_read_lo(srcLoad),
				    vis_read_lo(srcLoad));
				dr4 =
				    vis_fmuld8ulx16(vis_read_lo(srcLoad),
				    vis_read_lo(srcLoad));
				dc1.db = vis_fpadd32(dc1.db, dr1);
				dc2.db = vis_fpadd32(dc2.db, dr3);
				dc1.db = vis_fpadd32(dc1.db, dr2);
				dc2.db = vis_fpadd32(dc2.db, dr4);
			}

			mtwo0 += (mlib_d64)dc1.two_int.int0;
			mtwo1 += (mlib_d64)dc1.two_int.int1;
			mtwo2 += (mlib_d64)dc2.two_int.int0;
			mtwo3 += (mlib_d64)dc2.two_int.int1;

			tmpArr[0] = vis_fzero();

			rowMTwo[3 & (0 - alignBytes)] +=
			    mtwo0 + (mlib_d64)compens;
			rowMTwo[3 & (1 - alignBytes)] +=
			    mtwo1 + (mlib_d64)compens;
			rowMTwo[3 & (2 - alignBytes)] +=
			    mtwo2 + (mlib_d64)compens;
			rowMTwo[(3 - alignBytes)] += mtwo3 + (mlib_d64)compens;

			mtwo0 = mtwo1 = mtwo2 = mtwo3 = vis_fzero();

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_u16 *)srcRowPtr;
		}
	} else {
/* case 3: */

		mlib_d64 secondLoad, thirdLoad;

		byteWidth = srcWidth * 3;

		if (2 * byteWidth == srcYStride) {
			srcWidth = srcTotalPixNum;
			byteWidth = srcTotalPixNum * 3;
			srcHeight = 1;
		}

		for (j = 0; j < srcHeight; j++) {
			alignBytes = (mlib_addr)srcPixelPtr & 7;
			tailBytes = byteWidth + (alignBytes >> 1);

			lEdgeMask =
			    vis_edge16(srcPixelPtr,
			    srcPixelPtr + byteWidth - 1);

			srcLoadPtr =
			    (mlib_d64 *)((mlib_u8 *)srcPixelPtr - alignBytes);
			srcLoad = *srcLoadPtr++;
			secondLoad = vis_ld_d64_nf(srcLoadPtr);
			srcLoadPtr++;
			srcLoad = vis_fxor(srcLoad, offset4);
			secondLoad = vis_fxor(secondLoad, offset4);
			vis_pst_16(srcLoad, tmpArr, lEdgeMask);
			srcLoad = tmpArr[0];
			alignBytes >>= 1;
			loadsNumber = tailBytes / 12;
			tailBytes -= loadsNumber * 12;
			compens = loadsNumber << 1;

			for (; loadsNumber > 0; ) {
				cycleNumber =
				    loadsNumber > 0x8000 ? 0x8000 : loadsNumber;
				loadsNumber -= cycleNumber;

#pragma pipeloop(0)
				for (k = 0; k < cycleNumber - 1; k++) {
					thirdLoad = *srcLoadPtr++;
					thirdLoad =
					    vis_fxor(thirdLoad, offset4);

					MUL3CH;
					srcLoad = *srcLoadPtr++;

					secondLoad = *srcLoadPtr++;
					srcLoad = vis_fxor(srcLoad, offset4);
					secondLoad =
					    vis_fxor(secondLoad, offset4);
				}

				if (k < cycleNumber) {
					thirdLoad = vis_ld_d64_nf(srcLoadPtr);
					srcLoadPtr++;
					thirdLoad =
					    vis_fxor(thirdLoad, offset4);

					MUL3CH;
					srcLoad = vis_ld_d64_nf(srcLoadPtr);
					srcLoadPtr++;

					secondLoad = vis_ld_d64_nf(srcLoadPtr);
					srcLoadPtr++;
					srcLoad = vis_fxor(srcLoad, offset4);
					secondLoad =
					    vis_fxor(secondLoad, offset4);
				}
			}

			tmpArr[0] = tmpArr[1] = tmpArr[2] = vis_fzero();

			if (tailBytes) {
				rEdgeMask = ~(0xfff >> tailBytes);
				thirdLoad = vis_ld_d64_nf(srcLoadPtr);
				srcLoadPtr++;
				thirdLoad = vis_fxor(thirdLoad, offset4);
				vis_pst_16(srcLoad, tmpArr, rEdgeMask >> 8);
				vis_pst_16(secondLoad, tmpArr + 1,
				    rEdgeMask >> 4);
				vis_pst_16(thirdLoad, tmpArr + 2, rEdgeMask);
				srcLoad = tmpArr[0];
				secondLoad = tmpArr[1];
				thirdLoad = tmpArr[2];

				MUL3CH;
				tmpArr[0] = vis_fzero();

				compens += 2;
			}

			rowMTwo[al2off[alignBytes + 0]] +=
			    mtwo0 + (mlib_d64)compens;
			rowMTwo[al2off[alignBytes + 2]] +=
			    mtwo1 + (mlib_d64)compens;
			rowMTwo[al2off[alignBytes + 1]] +=
			    mtwo2 + (mlib_d64)compens;
			mtwo0 = mtwo1 = mtwo2 = vis_fzero();

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_u16 *)srcRowPtr;
		}
	}

	switch (numOfChannels) {
	case 1:
		sum0 = mean[0] - 32768;
		mtwo0 =
		    (rowMTwo[0] + rowMTwo[1] + rowMTwo[2] +
		    rowMTwo[3]) / divider;
		stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 - sum0 * sum0));
		break;

	case 2:
		sum0 = mean[0] - 32768;
		mtwo0 = (rowMTwo[0] + rowMTwo[2]) / divider;
		stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 - sum0 * sum0));
		sum1 = mean[1] - 32768;
		mtwo1 = (rowMTwo[1] + rowMTwo[3]) / divider;
		stdev[1] = mlib_sqrt(mlib_fabs(mtwo1 - sum1 * sum1));
		break;

	case 3:
		sum0 = mean[0] - 32768;
		sum1 = mean[1] - 32768;
		sum2 = mean[2] - 32768;
		mtwo0 = rowMTwo[0] / divider;
		mtwo1 = rowMTwo[1] / divider;
		mtwo2 = rowMTwo[2] / divider;
		stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 - sum0 * sum0));
		stdev[1] = mlib_sqrt(mlib_fabs(mtwo1 - sum1 * sum1));
		stdev[2] = mlib_sqrt(mlib_fabs(mtwo2 - sum2 * sum2));
		break;

	default:	/* only case 4: */
		sum0 = mean[0] - 32768;
		sum1 = mean[1] - 32768;
		sum2 = mean[2] - 32768;
		sum3 = mean[3] - 32768;
		mtwo0 = rowMTwo[0] / divider;
		mtwo1 = rowMTwo[1] / divider;
		mtwo2 = rowMTwo[2] / divider;
		mtwo3 = rowMTwo[3] / divider;
		stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 - sum0 * sum0));
		stdev[1] = mlib_sqrt(mlib_fabs(mtwo1 - sum1 * sum1));
		stdev[2] = mlib_sqrt(mlib_fabs(mtwo2 - sum2 * sum2));
		stdev[3] = mlib_sqrt(mlib_fabs(mtwo3 - sum3 * sum3));
		break;
	}
}

/* *********************************************************** */

void
mlib_v_ImageStdDev2ParamS16(
    const mlib_image *src,
    mlib_d64 *stdev)
{
/* pointer for pixel of source */
	mlib_s16 *srcPixelPtr = (mlib_s16 *)mlib_ImageGetData(src);

/* pointer for row of source */
	mlib_u8 *srcRowPtr = (mlib_u8 *)srcPixelPtr;

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

	mlib_d64 *srcLoadPtr;
	mlib_d64 srcLoad;
	mlib_d64 tmpArr[3];
	mlib_s32 lEdgeMask, rEdgeMask;
	mlib_s32 alignBytes;

	mlib_f32 scale = vis_to_float(0x10001);

	mlib_d64 sum0 = 0.0, sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
	mlib_d64 mtwo0 = 0.0, mtwo1 = 0.0, mtwo2 = 0.0, mtwo3 = 0.0;
	mlib_s32 loadsNumber, cycleNumber;

	mlib_d64 dr1, dr2, dr3, dr4, a1, a2;
	mlib_d64 acc1 = vis_fzero(), acc2 = vis_fzero();
	mlib_d64 rowMean[4], rowMTwo[4];
	mlib_s32 byteWidth, tailBytes;
	mlib_d64 divider = (mlib_d64)srcTotalPixNum;

/* compute mean3_off(align, sumn) */
	const mlib_u8 al2off[6] = { 0, 2, 1, 0, 2, 1 };
	type_union_mlib_d64 dc1, dc2;
	type_union_mlib_d64 firstAccumulator;
	type_union_mlib_d64 secondAccumulator;
	mlib_d64 mone = vis_fone();
	mlib_s32 compens;

	firstAccumulator.db = secondAccumulator.db = vis_fzero();
	tmpArr[0] = vis_fzero();

	for (i = 0; i < 4; i++)
		rowMean[i] = rowMTwo[i] = vis_fzero();

	if (numOfChannels != 3) {

		byteWidth = srcWidth << (numOfChannels >> 1);

		if (2 * byteWidth == srcYStride) {
			srcWidth = srcTotalPixNum;
			byteWidth = srcTotalPixNum << (numOfChannels >> 1);
			srcHeight = 1;
		}

		for (j = 0; j < srcHeight; j++) {

			tmpArr[0] = vis_fzero();
			alignBytes = (mlib_addr)srcPixelPtr & 7;
			tailBytes = byteWidth + (alignBytes >> 1);
			loadsNumber = tailBytes >> 2;
			tailBytes &= 3;

			lEdgeMask =
			    vis_edge16(srcPixelPtr,
			    srcPixelPtr + byteWidth - 1);
			rEdgeMask = ~(0xf >> tailBytes);

			srcLoadPtr =
			    (mlib_d64 *)((mlib_u8 *)srcPixelPtr - alignBytes);
			alignBytes >>= 1;

			srcLoad = *srcLoadPtr++;
			vis_pst_16(srcLoad, tmpArr, lEdgeMask);
			srcLoad = tmpArr[0];
			cycleNumber = 0;
			compens = (loadsNumber + 1) >> 1;

			while (loadsNumber > 0) {
				cycleNumber =
				    loadsNumber > 65536 ? 65536 : loadsNumber;
				loadsNumber -= cycleNumber;

				for (k = 0; k < (cycleNumber >> 1); k++) {

					dr1 =
					    vis_fmuld8sux16(vis_read_hi
					    (srcLoad), vis_read_hi(srcLoad));
					dr2 =
					    vis_fmuld8ulx16(vis_read_hi
					    (srcLoad), vis_read_hi(srcLoad));
					dr3 =
					    vis_fmuld8sux16(vis_read_lo
					    (srcLoad), vis_read_lo(srcLoad));
					dr4 =
					    vis_fmuld8ulx16(vis_read_lo
					    (srcLoad), vis_read_lo(srcLoad));
/* sub one */
					dr1 = vis_fpadd32(dr1, mone);
/* sub one */
					dr3 = vis_fpadd32(dr3, mone);
					dc1.db = vis_fpadd32(dr1, dr2);
					dc2.db = vis_fpadd32(dr3, dr4);

					a1 = vis_fmuld8ulx16(scale,
					    vis_read_hi(srcLoad));
					a2 = vis_fmuld8ulx16(scale,
					    vis_read_lo(srcLoad));
					srcLoad = *srcLoadPtr++;

					acc1 = vis_fpadd32(acc1, a1);
					acc2 = vis_fpadd32(acc2, a2);

					dr1 =
					    vis_fmuld8sux16(vis_read_hi
					    (srcLoad), vis_read_hi(srcLoad));
					dr2 =
					    vis_fmuld8ulx16(vis_read_hi
					    (srcLoad), vis_read_hi(srcLoad));
					dr3 =
					    vis_fmuld8sux16(vis_read_lo
					    (srcLoad), vis_read_lo(srcLoad));
					dr4 =
					    vis_fmuld8ulx16(vis_read_lo
					    (srcLoad), vis_read_lo(srcLoad));
					dc1.db = vis_fpadd32(dc1.db, dr1);
					dc2.db = vis_fpadd32(dc2.db, dr3);
					dc1.db = vis_fpadd32(dc1.db, dr2);
					dc2.db = vis_fpadd32(dc2.db, dr4);

					mtwo0 += (mlib_d64)dc1.two_int.int0;
					mtwo1 += (mlib_d64)dc1.two_int.int1;
					mtwo2 += (mlib_d64)dc2.two_int.int0;
					mtwo3 += (mlib_d64)dc2.two_int.int1;

					a1 = vis_fmuld8ulx16(scale,
					    vis_read_hi(srcLoad));
					a2 = vis_fmuld8ulx16(scale,
					    vis_read_lo(srcLoad));
					srcLoad = vis_ld_d64_nf(srcLoadPtr);
					srcLoadPtr++;

					acc1 = vis_fpadd32(acc1, a1);
					acc2 = vis_fpadd32(acc2, a2);
				}

				firstAccumulator.db = acc1;
				secondAccumulator.db = acc2;

				sum0 += firstAccumulator.two_int.int0;
				sum1 += firstAccumulator.two_int.int1;
				sum2 += secondAccumulator.two_int.int0;
				sum3 += secondAccumulator.two_int.int1;

				firstAccumulator.db = secondAccumulator.db =
				    acc1 = acc2 = vis_fzero();
			}

			tmpArr[0] = dc1.db = dc2.db = vis_fzero();

			if (cycleNumber & 1) {

				dr1 =
				    vis_fmuld8sux16(vis_read_hi(srcLoad),
				    vis_read_hi(srcLoad));
				dr2 =
				    vis_fmuld8ulx16(vis_read_hi(srcLoad),
				    vis_read_hi(srcLoad));
				dr3 =
				    vis_fmuld8sux16(vis_read_lo(srcLoad),
				    vis_read_lo(srcLoad));
				dr4 =
				    vis_fmuld8ulx16(vis_read_lo(srcLoad),
				    vis_read_lo(srcLoad));
/* sub one */
				dr1 = vis_fpadd32(dr1, mone);
/* sub one */
				dr3 = vis_fpadd32(dr3, mone);
				dc1.db = vis_fpadd32(dr1, dr2);
				dc2.db = vis_fpadd32(dr3, dr4);

				a1 = vis_fmuld8ulx16(scale,
				    vis_read_hi(srcLoad));
				a2 = vis_fmuld8ulx16(scale,
				    vis_read_lo(srcLoad));
				srcLoad = vis_ld_d64_nf(srcLoadPtr);
				srcLoadPtr++;

				acc1 = vis_fpadd32(acc1, a1);
				acc2 = vis_fpadd32(acc2, a2);
			}

			if (tailBytes) {
				vis_pst_16(srcLoad, tmpArr, rEdgeMask);
				srcLoad = tmpArr[0];

				dr1 =
				    vis_fmuld8sux16(vis_read_hi(srcLoad),
				    vis_read_hi(srcLoad));
				dr2 =
				    vis_fmuld8ulx16(vis_read_hi(srcLoad),
				    vis_read_hi(srcLoad));
				dr3 =
				    vis_fmuld8sux16(vis_read_lo(srcLoad),
				    vis_read_lo(srcLoad));
				dr4 =
				    vis_fmuld8ulx16(vis_read_lo(srcLoad),
				    vis_read_lo(srcLoad));
				dc1.db = vis_fpadd32(dc1.db, dr1);
				dc2.db = vis_fpadd32(dc2.db, dr3);
				dc1.db = vis_fpadd32(dc1.db, dr2);
				dc2.db = vis_fpadd32(dc2.db, dr4);

				a1 = vis_fmuld8ulx16(scale,
				    vis_read_hi(srcLoad));
				a2 = vis_fmuld8ulx16(scale,
				    vis_read_lo(srcLoad));
				acc1 = vis_fpadd32(acc1, a1);
				acc2 = vis_fpadd32(acc2, a2);
			}

			mtwo0 += (mlib_d64)dc1.two_int.int0;
			mtwo1 += (mlib_d64)dc1.two_int.int1;
			mtwo2 += (mlib_d64)dc2.two_int.int0;
			mtwo3 += (mlib_d64)dc2.two_int.int1;

			firstAccumulator.db = acc1;
			secondAccumulator.db = acc2;

			sum0 += firstAccumulator.two_int.int0;
			sum1 += firstAccumulator.two_int.int1;
			sum2 += secondAccumulator.two_int.int0;
			sum3 += secondAccumulator.two_int.int1;

			firstAccumulator.db = secondAccumulator.db = acc1 =
			    acc2 = vis_fzero();
			tmpArr[0] = vis_fzero();

			rowMTwo[3 & (0 - alignBytes)] +=
			    mtwo0 + (mlib_d64)compens;
			rowMTwo[3 & (1 - alignBytes)] +=
			    mtwo1 + (mlib_d64)compens;
			rowMTwo[3 & (2 - alignBytes)] +=
			    mtwo2 + (mlib_d64)compens;
			rowMTwo[(3 - alignBytes)] += mtwo3 + (mlib_d64)compens;
			rowMean[3 & (0 - alignBytes)] += sum0;
			rowMean[3 & (1 - alignBytes)] += sum1;
			rowMean[3 & (2 - alignBytes)] += sum2;
			rowMean[(3 - alignBytes)] += sum3;

			mtwo0 = mtwo1 = mtwo2 = mtwo3 = sum0 = sum1 = sum2 =
			    sum3 = vis_fzero();

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_s16 *)srcRowPtr;
		}
	} else {
/* case 3: */

		mlib_d64 secondLoad, thirdLoad;
		mlib_d64 acc3;

		byteWidth = srcWidth * 3;

		if (2 * byteWidth == srcYStride) {
			srcWidth = srcTotalPixNum;
			byteWidth = srcTotalPixNum * 3;
			srcHeight = 1;
		}

		acc3 = vis_fzero();

		for (j = 0; j < srcHeight; j++) {
			alignBytes = (mlib_addr)srcPixelPtr & 7;
			tailBytes = byteWidth + (alignBytes >> 1);

			lEdgeMask =
			    vis_edge16(srcPixelPtr,
			    srcPixelPtr + byteWidth - 1);

			srcLoadPtr =
			    (mlib_d64 *)((mlib_u8 *)srcPixelPtr - alignBytes);
			srcLoad = *srcLoadPtr++;
			secondLoad = vis_ld_d64_nf(srcLoadPtr);
			srcLoadPtr++;
			vis_pst_16(srcLoad, tmpArr, lEdgeMask);
			srcLoad = tmpArr[0];
			alignBytes >>= 1;
			loadsNumber = tailBytes / 12;
			tailBytes -= loadsNumber * 12;
			compens = loadsNumber << 1;

			for (; loadsNumber > 0; ) {
				cycleNumber =
				    loadsNumber > 0x8000 ? 0x8000 : loadsNumber;
				loadsNumber -= cycleNumber;

#pragma pipeloop(0)
				for (k = 0; k < cycleNumber - 1; k++) {
					thirdLoad = *srcLoadPtr++;

					MUL3CH;
					ACC3CH;
					srcLoad = *srcLoadPtr++;

					secondLoad = *srcLoadPtr++;
				}

				if (k < cycleNumber) {
					thirdLoad = vis_ld_d64_nf(srcLoadPtr);
					srcLoadPtr++;

					MUL3CH;
					ACC3CH;
					srcLoad = vis_ld_d64_nf(srcLoadPtr);
					srcLoadPtr++;

					secondLoad = vis_ld_d64_nf(srcLoadPtr);
					srcLoadPtr++;
				}

				SUM3CH;
				acc1 = acc2 = acc3 = vis_fzero();
			}

			tmpArr[0] = tmpArr[1] = tmpArr[2] = vis_fzero();

			if (tailBytes) {
				rEdgeMask = ~(0xfff >> tailBytes);
				thirdLoad = vis_ld_d64_nf(srcLoadPtr);
				srcLoadPtr++;
				vis_pst_16(srcLoad, tmpArr, rEdgeMask >> 8);
				vis_pst_16(secondLoad, tmpArr + 1,
				    rEdgeMask >> 4);
				vis_pst_16(thirdLoad, tmpArr + 2, rEdgeMask);
				srcLoad = tmpArr[0];
				secondLoad = tmpArr[1];
				thirdLoad = tmpArr[2];

				MUL3CH;
				ACC3CH;
				SUM3CH;
				acc1 = acc2 = acc3 = vis_fzero();

				tmpArr[0] = vis_fzero();
				compens += 2;
			}

			rowMTwo[al2off[alignBytes + 0]] +=
			    mtwo0 + (mlib_d64)compens;
			rowMTwo[al2off[alignBytes + 2]] +=
			    mtwo1 + (mlib_d64)compens;
			rowMTwo[al2off[alignBytes + 1]] +=
			    mtwo2 + (mlib_d64)compens;
			rowMean[al2off[alignBytes + 0]] += sum0;
			rowMean[al2off[alignBytes + 2]] += sum1;
			rowMean[al2off[alignBytes + 1]] += sum2;
			mtwo0 = mtwo1 = mtwo2 = sum0 = sum1 = sum2 =
			    vis_fzero();

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_s16 *)srcRowPtr;
		}
	}

	switch (numOfChannels) {
	case 1:
		sum0 =
		    (rowMean[0] + rowMean[1] + rowMean[2] +
		    rowMean[3]) / divider;
		mtwo0 =
		    (rowMTwo[0] + rowMTwo[1] + rowMTwo[2] +
		    rowMTwo[3]) / divider;
		stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 - sum0 * sum0));
		break;

	case 2:
		sum0 = (rowMean[0] + rowMean[2]) / divider;
		mtwo0 = (rowMTwo[0] + rowMTwo[2]) / divider;
		stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 - sum0 * sum0));
		sum1 = (rowMean[1] + rowMean[3]) / divider;
		mtwo1 = (rowMTwo[1] + rowMTwo[3]) / divider;
		stdev[1] = mlib_sqrt(mlib_fabs(mtwo1 - sum1 * sum1));
		break;

	case 3:
		sum0 = rowMean[0] / divider;
		sum1 = rowMean[1] / divider;
		sum2 = rowMean[2] / divider;
		mtwo0 = rowMTwo[0] / divider;
		mtwo1 = rowMTwo[1] / divider;
		mtwo2 = rowMTwo[2] / divider;
		stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 - sum0 * sum0));
		stdev[1] = mlib_sqrt(mlib_fabs(mtwo1 - sum1 * sum1));
		stdev[2] = mlib_sqrt(mlib_fabs(mtwo2 - sum2 * sum2));
		break;

	default:	/* only case 4: */
		sum0 = rowMean[0] / divider;
		sum1 = rowMean[1] / divider;
		sum2 = rowMean[2] / divider;
		sum3 = rowMean[3] / divider;
		mtwo0 = rowMTwo[0] / divider;
		mtwo1 = rowMTwo[1] / divider;
		mtwo2 = rowMTwo[2] / divider;
		mtwo3 = rowMTwo[3] / divider;
		stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 - sum0 * sum0));
		stdev[1] = mlib_sqrt(mlib_fabs(mtwo1 - sum1 * sum1));
		stdev[2] = mlib_sqrt(mlib_fabs(mtwo2 - sum2 * sum2));
		stdev[3] = mlib_sqrt(mlib_fabs(mtwo3 - sum3 * sum3));
		break;
	}
}

/* *********************************************************** */

#ifdef _NO_LONGLONG
#define	HERE_NO_LONGLONG

#else /* _NO_LONGLONG */

/* code with longlong works worse that should */
#define	HERE_NO_LONGLONG

#endif /* _NO_LONGLONG */

/* *********************************************************** */

void
mlib_v_ImageStdDev2ParamS32(
    const mlib_image *src,
    mlib_d64 *stdev)
{
/* pointer for pixel of source */
	mlib_s32 *srcPixelPtr = (mlib_s32 *)mlib_ImageGetData(src);

/* pointer for line of source */
	mlib_u8 *srcRowPtr = (mlib_u8 *)srcPixelPtr;

/* width of source image */
	mlib_s32 srcWidth = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 srcHeight = mlib_ImageGetHeight(src);

/* total number of pixels */
	mlib_s32 srcTotalPixNum = srcHeight * srcWidth;
	mlib_s32 pixelNumber = 0;
	mlib_s32 loadsNumber = 0;

/* linestride = bytes to next row */
	mlib_s32 srcYStride = mlib_ImageGetStride(src);

/* number of channels */
	mlib_s32 numOfChannels = mlib_ImageGetChannels(src);

/* indices */
	mlib_s32 i, j, k;
	mlib_d64 sum0 = 0.0, sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
	mlib_d64 ch0, ch1, ch2, ch3;
	mlib_d64 mtwo0 = 0.0, mtwo1 = 0.0, mtwo2 = 0.0, mtwo3 = 0.0;
	mlib_d64 divider = 1 / (mlib_d64)srcTotalPixNum;

#ifdef HERE_NO_LONGLONG
	mlib_s32 cycleNumber = 0;
	mlib_s32 val0, hi0, lo0;
	mlib_s32 val1, hi1, lo1;
	mlib_s32 val2, hi2, lo2;
	mlib_s32 val3, hi3, lo3;
#else /* HERE_NO_LONGLONG */
	mlib_s64 acc0, acc1, acc2, acc3;
	mlib_s64 val0, val1, val2, val3;
#endif /* HERE_NO_LONGLONG */

	if (numOfChannels * srcWidth * 4 == srcYStride) {
		srcWidth = srcHeight * srcWidth;
		srcHeight = 1;
	}

	if (numOfChannels != 3) {

		pixelNumber = numOfChannels * srcWidth;
		loadsNumber = pixelNumber >> 2;
		pixelNumber &= 3;
#ifdef HERE_NO_LONGLONG
		hi0 = lo0 = hi1 = lo1 = hi2 = lo2 = hi3 = lo3 = 0;
		cycleNumber = loadsNumber >> 15;
		loadsNumber &= 0x7fff;
#else /* HERE_NO_LONGLONG */
		acc0 = acc1 = acc2 = acc3 = 0;
#endif /* HERE_NO_LONGLONG */

		for (j = 0; j < srcHeight; j++) {
#ifdef HERE_NO_LONGLONG
			for (i = 0; i < cycleNumber; i++) {
#pragma pipeloop(0)
				for (k = 0; k < 0x8000; k++) {
					val0 = *srcPixelPtr;
					ch0 = (*srcPixelPtr++);
					mtwo0 += ch0 * ch0;
					hi0 += val0 >> 16;
					lo0 += val0 & 0xffff;

					val1 = *srcPixelPtr;
					ch1 = (*srcPixelPtr++);
					mtwo1 += ch1 * ch1;
					hi1 += val1 >> 16;
					lo1 += val1 & 0xffff;

					val2 = *srcPixelPtr;
					ch2 = (*srcPixelPtr++);
					mtwo2 += ch2 * ch2;
					hi2 += val2 >> 16;
					lo2 += val2 & 0xffff;

					val3 = *srcPixelPtr;
					ch3 = (*srcPixelPtr++);
					mtwo3 += ch3 * ch3;

/*
 * hi3 += val3 >> 16;
 * lo3 += val3 & 0xffff;
 */
/* this raise performance !? */
					lo3 &= val3 & 0x1;
					sum3 += ch3;
				}

/*
 * + (mlib_d64)hi3*65536.
 */
				sum0 += (mlib_d64)lo0 + (mlib_d64)hi0 *65536.;
				sum1 += (mlib_d64)lo1 + (mlib_d64)hi1 *65536.;
				sum2 += (mlib_d64)lo2 + (mlib_d64)hi2 *65536.;

				sum3 += (mlib_d64)lo3;
				hi0 = lo0 = hi1 = lo1 = hi2 = lo2 = hi3 = lo3 =
				    0;
			}

#pragma pipeloop(0)
			for (k = 0; k < loadsNumber; k++) {
				val0 = *srcPixelPtr;
				ch0 = (*srcPixelPtr++);
				mtwo0 += ch0 * ch0;
				hi0 += val0 >> 16;
				lo0 += val0 & 0xffff;

				val1 = *srcPixelPtr;
				ch1 = (*srcPixelPtr++);
				mtwo1 += ch1 * ch1;
				hi1 += val1 >> 16;
				lo1 += val1 & 0xffff;

				val2 = *srcPixelPtr;
				ch2 = (*srcPixelPtr++);
				mtwo2 += ch2 * ch2;
				hi2 += val2 >> 16;
				lo2 += val2 & 0xffff;

				val3 = *srcPixelPtr;
				ch3 = (*srcPixelPtr++);
				mtwo3 += ch3 * ch3;

/*
 * hi3 += val3 >> 16;
 * lo3 += val3 & 0xffff;
 */
				sum3 += ch3;
			}

			sum0 += (mlib_d64)lo0 + (mlib_d64)hi0 *65536.;
			sum1 += (mlib_d64)lo1 + (mlib_d64)hi1 *65536.;
			sum2 += (mlib_d64)lo2 + (mlib_d64)hi2 *65536.;

/* sum3 += (mlib_d64)lo3 + (mlib_d64)hi3*65536.; */
			hi0 = lo0 = hi1 = lo1 = hi2 = lo2 = hi3 = lo3 = 0;
#else /* HERE_NO_LONGLONG */
#pragma unroll(1)
#pragma pipeloop(0)
			for (k = 0; k < loadsNumber; k++) {
				acc0 += srcPixelPtr[0];
				ch0 = srcPixelPtr[0];
				mtwo0 += ch0 * ch0;

				acc1 += srcPixelPtr[1];
				ch1 = srcPixelPtr[1];
				mtwo1 += ch1 * ch1;

				acc2 += srcPixelPtr[2];
				ch2 = srcPixelPtr[2];
				mtwo2 += ch2 * ch2;

				acc3 += srcPixelPtr[3];
				ch3 = srcPixelPtr[3];
				mtwo3 += ch3 * ch3;

				srcPixelPtr += 4;
			}

#endif /* HERE_NO_LONGLONG */

			switch (pixelNumber) {

			case 3:
				ch0 = (*srcPixelPtr++);
				ch1 = (*srcPixelPtr++);
				ch2 = *srcPixelPtr;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
				sum0 += ch0;
				sum1 += ch1;
				sum2 += ch2;
				break;

			case 2:
				ch0 = (*srcPixelPtr++);
				ch1 = (*srcPixelPtr++);
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				sum0 += ch0;
				sum1 += ch1;
				break;

			case 1:
				ch0 = (*srcPixelPtr++);
				mtwo0 += ch0 * ch0;
				sum0 += ch0;
				break;
			}

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_s32 *)srcRowPtr;
		}

#ifndef HERE_NO_LONGLONG
		sum0 += (mlib_d64)acc0;
		sum1 += (mlib_d64)acc1;
		sum2 += (mlib_d64)acc2;
		sum3 += (mlib_d64)acc3;
#endif /* HERE_NO_LONGLONG */
	} else {
/* case 3: */

		pixelNumber = srcWidth;
#ifdef HERE_NO_LONGLONG
		hi0 = lo0 = hi1 = lo1 = hi2 = lo2 = 0;
		cycleNumber = pixelNumber >> 15;
		pixelNumber &= 0x7fff;
#else /* HERE_NO_LONGLONG */
		acc0 = acc1 = acc2 = acc3 = 0;
#endif /* HERE_NO_LONGLONG */
		for (j = 0; j < srcHeight; j++) {
#ifdef HERE_NO_LONGLONG
			for (i = 0; i < cycleNumber; i++) {
#pragma pipeloop(0)
				for (k = 0; k < 0x8000; k++) {
					val0 = *srcPixelPtr;
					ch0 = (*srcPixelPtr++);
					mtwo0 += ch0 * ch0;
					hi0 += val0 >> 16;
					lo0 += val0 & 0xffff;

					val1 = *srcPixelPtr;
					ch1 = (*srcPixelPtr++);
					mtwo1 += ch1 * ch1;
					hi1 += val1 >> 16;
					lo1 += val1 & 0xffff;

					ch2 = (*srcPixelPtr++);
					mtwo2 += ch2 * ch2;
/*
 * val2 = *srcPixelPtr;
 *             hi2 += val2 >> 16;
 * lo2 += val2 & 0xffff;
 */
					sum2 += ch2;
				}

				sum0 += (mlib_d64)lo0 + (mlib_d64)hi0 *65536.;
				sum1 += (mlib_d64)lo1 + (mlib_d64)hi1 *65536.;

/* sum2 += (mlib_d64)lo2 + (mlib_d64)hi2*65536.; */
				hi0 = lo0 = hi1 = lo1 = /* hi2 = lo2 = */ 0;
			}

#pragma pipeloop(0)
			for (k = 0; k < pixelNumber; k++) {
				val0 = *srcPixelPtr;
				ch0 = (*srcPixelPtr++);
				mtwo0 += ch0 * ch0;
				hi0 += val0 >> 16;
				lo0 += val0 & 0xffff;

				val1 = *srcPixelPtr;
				ch1 = (*srcPixelPtr++);
				mtwo1 += ch1 * ch1;
				hi1 += val1 >> 16;
				lo1 += val1 & 0xffff;

				ch2 = (*srcPixelPtr++);
				mtwo2 += ch2 * ch2;
/*
 * val2 = *srcPixelPtr;
 *           hi2 += val2 >> 16;
 * lo2 += val2 & 0xffff;
 */
				sum2 += ch2;
			}

			sum0 += (mlib_d64)lo0 + (mlib_d64)hi0 *65536.;
			sum1 += (mlib_d64)lo1 + (mlib_d64)hi1 *65536.;

/* sum2 += (mlib_d64)lo2 + (mlib_d64)hi2*65536.; */
			hi0 = lo0 = hi1 = lo1 = /* hi2 = lo2 = */ 0;
#else /* HERE_NO_LONGLONG */
			for (k = 0; k < pixelNumber; k++) {
				acc0 += *srcPixelPtr;
				ch0 = (*srcPixelPtr++);
				mtwo0 += ch0 * ch0;

				acc1 += *srcPixelPtr;
				ch1 = (*srcPixelPtr++);
				mtwo1 += ch1 * ch1;

				acc2 += *srcPixelPtr;
				ch2 = (*srcPixelPtr++);
				mtwo2 += ch2 * ch2;
			}

#endif /* HERE_NO_LONGLONG */

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_s32 *)srcRowPtr;
		}

#ifndef HERE_NO_LONGLONG
		sum0 += (mlib_d64)acc0;
		sum1 += (mlib_d64)acc1;
		sum2 += (mlib_d64)acc2;
#endif /* HERE_NO_LONGLONG */
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

	case 3:
		mtwo0 *= divider;
		mtwo1 *= divider;
		mtwo2 *= divider;
		sum0 *= divider;
		sum1 *= divider;
		sum2 *= divider;
		stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 - sum0 * sum0));
		stdev[1] = mlib_sqrt(mlib_fabs(mtwo1 - sum1 * sum1));
		stdev[2] = mlib_sqrt(mlib_fabs(mtwo2 - sum2 * sum2));
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
}

/* *********************************************************** */
