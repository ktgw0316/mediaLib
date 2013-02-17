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

#pragma ident	"@(#)mlib_i_ImageStdDev.c	9.3	07/11/05 SMI"

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
 *           1    w-1 h-1
 *      m = --- * SUM SUM x(i, j)
 *          w*h   i = 0 j = 0
 *
 *                  1    w-1 h-1             2
 *    s = sqrt (   --- * SUM SUM (x(i, j) - m)  )
 *                 w*h   i = 0 j = 0
 *
 * RESTRICTION
 *      The image can have 1, 2, 3 or 4 channels.
 *      It can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or
 *      MLIB_INT data type.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_SysMath.h>
#include <mlib_ImageDivTables.h>
#include <mlib_i_ImageMoment2.h>

#ifndef _NO_LONGLONG /* for 64-bit platform */
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

static void mlib_c_ImageStdDev2ParamU8(
    const mlib_image *src,
    mlib_d64 *stdev);

static void mlib_c_ImageStdDev2ParamU16(
    const mlib_image *src,
    mlib_d64 *stdev);

static void mlib_c_ImageStdDev2ParamS16(
    const mlib_image *src,
    mlib_d64 *stdev);

static void mlib_c_ImageStdDev2ParamS32(
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
			mlib_c_ImageStdDev2ParamU8(src, stdev);
			return (MLIB_SUCCESS);

		case MLIB_SHORT:
			mlib_c_ImageStdDev2ParamS16(src, stdev);
			return (MLIB_SUCCESS);

		case MLIB_USHORT:
			mlib_c_ImageStdDev2ParamU16(src, stdev);
			return (MLIB_SUCCESS);

		case MLIB_INT:
			mlib_c_ImageStdDev2ParamS32(src, stdev);
			return (MLIB_SUCCESS);

		default:
			return (MLIB_FAILURE);
		}
	}

	switch (mlib_ImageGetType(src)) {
	case MLIB_BYTE:

		if (num_channels != 3)
			mlib_c_ImageMoment2_U8_124(src, rez);
		else
			mlib_c_ImageMoment2_U8_3(src, rez);
		break;

	case MLIB_USHORT:

		if (num_channels != 3)
			mlib_c_ImageMoment2_U16_124(src, rez);
		else
			mlib_c_ImageMoment2_U16_3(src, rez);
		break;

	case MLIB_SHORT:

		if (num_channels != 3)
			mlib_c_ImageMoment2_S16_124(src, rez);
		else
			mlib_c_ImageMoment2_S16_3(src, rez);
		break;

	case MLIB_INT:

		if (num_channels != 3)
			mlib_c_ImageMoment2_S32_124(src, rez);
		else
			mlib_c_ImageMoment2_S32_3(src, rez);
		break;

	default:
		return (MLIB_FAILURE);
	}

	divider =
	    1.0 / ((mlib_d64)mlib_ImageGetWidth(src) *
	    mlib_ImageGetHeight(src));
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
mlib_c_ImageStdDev2ParamU8(
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

/* linestride = bytes to next row */
	mlib_s32 srcYStride = mlib_ImageGetStride(src);

/* number of channels */
	mlib_s32 numOfChannels = mlib_ImageGetChannels(src);

/* total number of pixels */
	mlib_s32 srcTotalPixNum = srcHeight * srcWidth;

/* indices */
	mlib_s32 i, j;
	mlib_s32 loadsNum, pixelNum;
	mlib_s64 ch0, ch1, ch2, ch3;
	mlib_s64 sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0;
	mlib_s64 mtwo0 = 0, mtwo1 = 0, mtwo2 = 0, mtwo3 = 0;

	mlib_d64 divider = 1.0 / (mlib_d64)srcTotalPixNum;

	if (numOfChannels != 3) {

		pixelNum = srcWidth << (numOfChannels >> 1);

		if (pixelNum == srcYStride) {
			pixelNum = srcTotalPixNum << (numOfChannels >> 1);
			srcHeight = 1;
		}

		loadsNum = pixelNum >> 2;
		pixelNum &= 3;

		for (i = 0; i < srcHeight; i++) {
			for (j = 0; j < loadsNum; j++) {
				ch0 = (*srcPixelPtr++);
				ch1 = (*srcPixelPtr++);
				ch2 = (*srcPixelPtr++);
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

			switch (pixelNum) {

			case 3:
				ch0 = (*srcPixelPtr++);
				ch1 = (*srcPixelPtr++);
				ch2 = *srcPixelPtr;

				sum0 += ch0;
				sum1 += ch1;
				sum2 += ch2;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
				break;

			case 2:
				ch0 = (*srcPixelPtr++);
				ch1 = *srcPixelPtr;
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				break;

			case 1:
				ch0 = *srcPixelPtr;
				sum0 += ch0;
				mtwo0 += ch0 * ch0;
				break;
			}

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_u8 *)srcRowPtr;
		}

	} else {
/* case 3: */

		if (srcWidth * 3 == srcYStride) {
			srcWidth = srcTotalPixNum;
			srcHeight = 1;
		}

		pixelNum = srcWidth;

		for (i = 0; i < srcHeight; i++) {
			for (j = 0; j < pixelNum; j++) {
				ch0 = (*srcPixelPtr++);
				ch1 = (*srcPixelPtr++);
				ch2 = (*srcPixelPtr++);
				sum0 += ch0;
				sum1 += ch1;
				sum2 += ch2;

				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
			}

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_u8 *)srcRowPtr;
		}
	}

	switch (numOfChannels) {

	case 1:
		mtwo0 = mtwo0 + mtwo1 + mtwo2 + mtwo3;
		sum0 = sum0 + sum1 + sum2 + sum3;
		stdev[0] = mlib_sqrt((mtwo0 * srcTotalPixNum -
			sum0 * sum0) & 0x7FFFFFFFFFFFFFFF) * divider;
		break;

	case 2:
		mtwo0 = mtwo0 + mtwo2;
		mtwo1 = mtwo1 + mtwo3;
		sum0 = sum0 + sum2;
		sum1 = sum1 + sum3;
		stdev[0] = mlib_sqrt((mtwo0 * srcTotalPixNum -
			sum0 * sum0) & 0x7FFFFFFFFFFFFFFF)  * divider;
		stdev[1] = mlib_sqrt((mtwo1 * srcTotalPixNum -
			sum1 * sum1) & 0x7FFFFFFFFFFFFFFF) * divider;

		break;

	case 3:
		stdev[0] = mlib_sqrt((mtwo0 * srcTotalPixNum -
			sum0 * sum0) & 0x7FFFFFFFFFFFFFFF) * divider;
		stdev[1] = mlib_sqrt((mtwo1 * srcTotalPixNum -
			sum1 * sum1) & 0x7FFFFFFFFFFFFFFF) * divider;
		stdev[2] = mlib_sqrt((mtwo2 * srcTotalPixNum -
			sum2 * sum2) & 0x7FFFFFFFFFFFFFFF) * divider;
		break;

	case 4:
		stdev[0] = mlib_sqrt((mtwo0 * srcTotalPixNum -
			sum0 * sum0) & 0x7FFFFFFFFFFFFFFF) * divider;
		stdev[1] = mlib_sqrt((mtwo1 * srcTotalPixNum -
			sum1 * sum1) & 0x7FFFFFFFFFFFFFFF) * divider;
		stdev[2] = mlib_sqrt((mtwo2 * srcTotalPixNum -
			sum2 * sum2) & 0x7FFFFFFFFFFFFFFF) * divider;
		stdev[3] = mlib_sqrt((mtwo3 * srcTotalPixNum -
			sum3 * sum3) & 0x7FFFFFFFFFFFFFFF) * divider;
		break;
	}
}

/* *********************************************************** */

void
mlib_c_ImageStdDev2ParamU16(
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

/* linestride = bytes to next row */
	mlib_s32 srcYStride = mlib_ImageGetStride(src);

/* number of channels */
	mlib_s32 numOfChannels = mlib_ImageGetChannels(src);

/* total number of pixels */
	mlib_s32 srcTotalPixNum = srcHeight * srcWidth;

/* indices */
	mlib_s32 i, j;
	mlib_s32 loadsNum, pixelNum;
	mlib_u64 ch0, ch1, ch2, ch3;
	mlib_u64 sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0;
	mlib_u64 mtwo0 = 0, mtwo1 = 0, mtwo2 = 0, mtwo3 = 0;

	mlib_d64 divider = 1 / (mlib_d64)srcTotalPixNum;
	mlib_u32 a1, a2, a3, a4;
	mlib_s32 align;
	mlib_u64 dsum[4], moment[4];

	if (numOfChannels == 1) {

		if (2 * srcWidth == srcYStride) {
			srcWidth = srcTotalPixNum;
			srcHeight = 1;
		}

		pixelNum = srcWidth;

		for (i = 0; i < srcHeight; i++) {
			j = 0;

			if (2 & (mlib_addr)srcPixelPtr) {
				ch0 = (*srcPixelPtr++);
				sum0 += ch0;
				mtwo0 += ch0 * ch0;
				j = 1;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= pixelNum - 8; j += 8) {
				a1 = ((mlib_u32 *)srcPixelPtr)[0];
				a2 = ((mlib_u32 *)srcPixelPtr)[1];
				a3 = ((mlib_u32 *)srcPixelPtr)[2];
				a4 = ((mlib_u32 *)srcPixelPtr)[3];
				srcPixelPtr += 8;

				ch0 = (mlib_s32)(a1 >> 16);
				ch1 = (mlib_s32)(a1 & 0xffff);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch0 = (mlib_s32)(a2 >> 16);
				ch1 = (mlib_s32)(a2 & 0xffff);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch0 = (mlib_s32)(a3 >> 16);
				ch1 = (mlib_s32)(a3 & 0xffff);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch0 = (mlib_s32)(a4 >> 16);
				ch1 = (mlib_s32)(a4 & 0xffff);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(1)
#endif /* __SUNPRO_C */
			for (; j < pixelNum; j++) {
				ch0 = (*srcPixelPtr++);
				sum0 += ch0;
				mtwo0 += ch0 * ch0;
			}

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_u16 *)srcRowPtr;
		}

		sum0 += sum1;
		mtwo0 += mtwo1;
		stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 * divider -
					sum0 * sum0 * divider * divider));

	} else if (numOfChannels == 2) {

		dsum[0] = dsum[1] = moment[0] = moment[1] = 0;

		if (2 * 2 * srcWidth == srcYStride) {
			srcWidth = srcTotalPixNum;
			srcHeight = 1;
		}

		pixelNum = srcWidth;

		for (i = 0; i < srcHeight; i++) {
			j = align = 0;

			if (2 & (mlib_addr)srcPixelPtr) {
#ifdef _LITTLE_ENDIAN
				ch1 = *(srcPixelPtr + 2 * srcWidth - 1);
				ch0 = (*srcPixelPtr++);
#else /* _LITTLE_ENDIAN */
				ch0 = *(srcPixelPtr + 2 * srcWidth - 1);
				ch1 = (*srcPixelPtr++);
#endif /* _LITTLE_ENDIAN */
				sum1 += ch1;
				sum0 += ch0;
				mtwo1 += ch1 * ch1;
				mtwo0 += ch0 * ch0;
				j = 1;
				align = 1;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= pixelNum - 4; j += 4) {
				a1 = ((mlib_s32 *)srcPixelPtr)[0];
				a2 = ((mlib_s32 *)srcPixelPtr)[1];
				a3 = ((mlib_s32 *)srcPixelPtr)[2];
				a4 = ((mlib_s32 *)srcPixelPtr)[3];
				srcPixelPtr += 8;

				ch0 = (mlib_s32)(a1 >> 16);
				ch1 = (mlib_s32)(a1 & 0xffff);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch0 = (mlib_s32)(a2 >> 16);
				ch1 = (mlib_s32)(a2 & 0xffff);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch0 = (mlib_s32)(a3 >> 16);
				ch1 = (mlib_s32)(a3 & 0xffff);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch0 = (mlib_s32)(a4 >> 16);
				ch1 = (mlib_s32)(a4 & 0xffff);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(1)
#endif /* __SUNPRO_C */
			for (; j < pixelNum; j++) {
#ifdef _LITTLE_ENDIAN
				ch1 = (*srcPixelPtr++);
				ch0 = (*srcPixelPtr++);
#else /* _LITTLE_ENDIAN */
				ch0 = (*srcPixelPtr++);
				ch1 = (*srcPixelPtr++);
#endif /* _LITTLE_ENDIAN */
				sum0 += ch0;
				mtwo0 += ch0 * ch0;
				sum1 += ch1;
				mtwo1 += ch1 * ch1;
			}

#ifdef _LITTLE_ENDIAN
			dsum[1 ^ align] += sum0;
			dsum[align] += sum1;
			moment[1 ^ align] += mtwo0;
			moment[align] += mtwo1;
#else /* _LITTLE_ENDIAN */
			dsum[align] += sum0;
			dsum[1 ^ align] += sum1;
			moment[align] += mtwo0;
			moment[1 ^ align] += mtwo1;
#endif /* _LITTLE_ENDIAN */
			sum0 = sum1 = mtwo0 = mtwo1 = 0;

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_u16 *)srcRowPtr;
		}

		stdev[0] =
		    mlib_sqrt(mlib_fabs(moment[0] * divider -
				dsum[0] * dsum[0] * divider * divider));
		stdev[1] =
		    mlib_sqrt(mlib_fabs(moment[1] * divider -
				dsum[1] * dsum[1] * divider * divider));

	} else if (numOfChannels == 3) {

		if (srcWidth * 3 * 2 == srcYStride) {
			srcWidth = srcTotalPixNum;
			srcHeight = 1;
		}

		pixelNum = srcWidth;

		for (i = 0; i < srcHeight; i++) {
			j = 0;

			if (2 & (mlib_addr)srcPixelPtr) {
				ch0 = (*srcPixelPtr++);
				ch1 = (*srcPixelPtr++);
				ch2 = (*srcPixelPtr++);
				sum0 += ch0;
				sum1 += ch1;
				sum2 += ch2;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
				j = 1;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < pixelNum - 1; j += 2) {
				a1 = ((mlib_s32 *)srcPixelPtr)[0];
				a2 = ((mlib_s32 *)srcPixelPtr)[1];
				a3 = ((mlib_s32 *)srcPixelPtr)[2];
				srcPixelPtr += 6;

#ifdef _LITTLE_ENDIAN
				ch0 = (mlib_s32)(a1 & 0xffff);
				ch1 = (mlib_s32)(a1 >> 16);
				ch2 = (mlib_s32)(a2 & 0xffff);
#else /* _LITTLE_ENDIAN */
				ch0 = (mlib_s32)(a1 >> 16);
				ch1 = (mlib_s32)(a1 & 0xffff);
				ch2 = (mlib_s32)(a2 >> 16);
#endif /* _LITTLE_ENDIAN */
				sum0 += ch0;
				sum1 += ch1;
				sum2 += ch2;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;

#ifdef _LITTLE_ENDIAN
				ch0 = (mlib_s32)(a2 >> 16);
				ch1 = (mlib_s32)(a3 & 0xffff);
				ch2 = (mlib_s32)(a3 >> 16);
#else /* _LITTLE_ENDIAN */
				ch0 = (mlib_s32)(a2 & 0xffff);
				ch1 = (mlib_s32)(a3 >> 16);
				ch2 = (mlib_s32)(a3 & 0xffff);
#endif /* _LITTLE_ENDIAN */
				sum0 += ch0;
				sum1 += ch1;
				sum2 += ch2;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
			}

			if (j < pixelNum) {
				ch0 = (*srcPixelPtr++);
				ch1 = (*srcPixelPtr++);
				ch2 = (*srcPixelPtr++);
				sum0 += ch0;
				sum1 += ch1;
				sum2 += ch2;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
			}

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_u16 *)srcRowPtr;
		}

		stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 * divider -
					sum0 * sum0 * divider * divider));
		stdev[1] = mlib_sqrt(mlib_fabs(mtwo1 * divider -
					sum1 * sum1 * divider * divider));
		stdev[2] = mlib_sqrt(mlib_fabs(mtwo2 * divider -
					sum2 * sum2 * divider * divider));

	} else {
/* case 4: */

		dsum[0] = dsum[1] = moment[0] = moment[1] = 0;
		dsum[2] = dsum[3] = moment[2] = moment[3] = 0;

		if (2 * 4 * srcWidth == srcYStride) {
			srcWidth = srcTotalPixNum;
			srcHeight = 1;
		}

		loadsNum = srcWidth;

		for (i = 0; i < srcHeight; i++) {
			j = align = 0;

			if (2 & (mlib_addr)srcPixelPtr) {
#ifdef _LITTLE_ENDIAN
				ch1 = *(srcPixelPtr + 4 * srcWidth - 1);
				ch0 = (*srcPixelPtr++);
				ch3 = (*srcPixelPtr++);
				ch2 = (*srcPixelPtr++);
#else /* _LITTLE_ENDIAN */
				ch0 = *(srcPixelPtr + 4 * srcWidth - 1);
				ch1 = (*srcPixelPtr++);
				ch2 = (*srcPixelPtr++);
				ch3 = (*srcPixelPtr++);
#endif /* _LITTLE_ENDIAN */
				sum1 += ch1;
				sum2 += ch2;
				sum3 += ch3;
				sum0 += ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
				mtwo3 += ch3 * ch3;
				mtwo0 += ch0 * ch0;
				j = 1;
				align = 1;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= loadsNum - 2; j += 2) {
				a1 = ((mlib_s32 *)srcPixelPtr)[0];
				a2 = ((mlib_s32 *)srcPixelPtr)[1];
				a3 = ((mlib_s32 *)srcPixelPtr)[2];
				a4 = ((mlib_s32 *)srcPixelPtr)[3];
				srcPixelPtr += 8;

				ch0 = (mlib_s32)(a1 >> 16);
				ch1 = (mlib_s32)(a1 & 0xffff);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch2 = (mlib_s32)(a2 >> 16);
				ch3 = (mlib_s32)(a2 & 0xffff);
				sum2 += ch2;
				sum3 += ch3;
				mtwo2 += ch2 * ch2;
				mtwo3 += ch3 * ch3;

				ch0 = (mlib_s32)(a3 >> 16);
				ch1 = (mlib_s32)(a3 & 0xffff);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch2 = (mlib_s32)(a4 >> 16);
				ch3 = (mlib_s32)(a4 & 0xffff);
				sum2 += ch2;
				sum3 += ch3;
				mtwo2 += ch2 * ch2;
				mtwo3 += ch3 * ch3;
			}

			if (j < loadsNum) {
				a1 = ((mlib_s32 *)srcPixelPtr)[0];
				a2 = ((mlib_s32 *)srcPixelPtr)[1];
				srcPixelPtr += 4;

				ch0 = (mlib_s32)(a1 >> 16);
				ch1 = (mlib_s32)(a1 & 0xffff);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch2 = (mlib_s32)(a2 >> 16);
				ch3 = (mlib_s32)(a2 & 0xffff);
				sum2 += ch2;
				sum3 += ch3;
				mtwo2 += ch2 * ch2;
				mtwo3 += ch3 * ch3;
			}
#ifdef _LITTLE_ENDIAN
			dsum[3 & (0 - align)] += sum1;
			dsum[1 - align] += sum0;
			dsum[2 - align] += sum3;
			dsum[3 - align] += sum2;
			moment[3 & (0 - align)] += mtwo1;
			moment[1 - align] += mtwo0;
			moment[2 - align] += mtwo3;
			moment[3 - align] += mtwo2;
#else /* _LITTLE_ENDIAN */
			dsum[3 & (0 - align)] += sum0;
			dsum[1 - align] += sum1;
			dsum[2 - align] += sum2;
			dsum[3 - align] += sum3;
			moment[3 & (0 - align)] += mtwo0;
			moment[1 - align] += mtwo1;
			moment[2 - align] += mtwo2;
			moment[3 - align] += mtwo3;
#endif /* _LITTLE_ENDIAN */

			sum0 = sum1 = mtwo0 = mtwo1 =
			    sum2 = sum3 = mtwo2 = mtwo3 = 0.0;
			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_u16 *)srcRowPtr;
		}

		stdev[0] =
		    mlib_sqrt(mlib_fabs(moment[0] * divider -
				dsum[0] * dsum[0] * divider * divider));
		stdev[1] =
		    mlib_sqrt(mlib_fabs(moment[1] * divider -
				dsum[1] * dsum[1] * divider * divider));
		stdev[2] =
		    mlib_sqrt(mlib_fabs(moment[2] * divider -
				dsum[2] * dsum[2]* divider * divider));
		stdev[3] =
		    mlib_sqrt(mlib_fabs(moment[3] * divider -
				dsum[3] * dsum[3] * divider * divider));
	}
}

/* *********************************************************** */

void
mlib_c_ImageStdDev2ParamS16(
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

/* linestride = bytes to next row */
	mlib_s32 srcYStride = mlib_ImageGetStride(src);

/* number of channels */
	mlib_s32 numOfChannels = mlib_ImageGetChannels(src);

/* total number of pixels */
	mlib_s32 srcTotalPixNum = srcHeight * srcWidth;

/* indices */
	mlib_s32 i, j;
	mlib_s32 loadsNum, pixelNum;
	mlib_s64 ch0, ch1, ch2, ch3;
	mlib_s64 sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0;
	mlib_s64 mtwo0 = 0, mtwo1 = 0, mtwo2 = 0, mtwo3 = 0;
	mlib_d64 divider = 1 / (mlib_d64)srcTotalPixNum;

	mlib_s32 a1, a2, a3, a4, align;
	mlib_s64 dsum[4], moment[4];

	if (numOfChannels == 1) {

		if (2 * srcWidth == srcYStride) {
			srcWidth = srcTotalPixNum;
			srcHeight = 1;
		}

		pixelNum = srcWidth;

		for (i = 0; i < srcHeight; i++) {
			j = 0;

			if (2 & (mlib_addr)srcPixelPtr) {
				ch0 = (*srcPixelPtr++);
				sum0 += ch0;
				mtwo0 += ch0 * ch0;
				j = 1;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= pixelNum - 8; j += 8) {
				a1 = ((mlib_s32 *)srcPixelPtr)[0];
				a2 = ((mlib_s32 *)srcPixelPtr)[1];
				a3 = ((mlib_s32 *)srcPixelPtr)[2];
				a4 = ((mlib_s32 *)srcPixelPtr)[3];
				srcPixelPtr += 8;

				ch0 = a1 >> 16;
				ch1 = (a1 & 0x7fff) - (a1 & 0x8000);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch0 = a2 >> 16;
				ch1 = (a2 & 0x7fff) - (a2 & 0x8000);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch0 = a3 >> 16;
				ch1 = (a3 & 0x7fff) - (a3 & 0x8000);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch0 = a4 >> 16;
				ch1 = (a4 & 0x7fff) - (a4 & 0x8000);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(1)
#endif /* __SUNPRO_C */
			for (; j < pixelNum; j++) {
				ch0 = (*srcPixelPtr++);
				sum0 += ch0;
				mtwo0 += ch0 * ch0;
			}

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_s16 *)srcRowPtr;
		}

		sum0 += sum1;
		mtwo0 += mtwo1;
		stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 * divider -
			sum0 * sum0 * divider * divider));

	} else if (numOfChannels == 2) {

		dsum[0] = dsum[1] = moment[0] = moment[1] = 0;

		if (2 * 2 * srcWidth == srcYStride) {
			srcWidth = srcTotalPixNum;
			srcHeight = 1;
		}

		pixelNum = srcWidth;

		for (i = 0; i < srcHeight; i++) {
			j = align = 0;

			if (2 & (mlib_addr)srcPixelPtr) {
#ifdef _LITTLE_ENDIAN
				ch1 = *(srcPixelPtr + 2 * srcWidth - 1);
				ch0 = (*srcPixelPtr++);
#else /* _LITTLE_ENDIAN */
				ch0 = *(srcPixelPtr + 2 * srcWidth - 1);
				ch1 = (*srcPixelPtr++);
#endif /* _LITTLE_ENDIAN */
				sum1 += ch1;
				sum0 += ch0;
				mtwo1 += ch1 * ch1;
				mtwo0 += ch0 * ch0;
				j = 1;
				align = 1;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= pixelNum - 4; j += 4) {
				a1 = ((mlib_s32 *)srcPixelPtr)[0];
				a2 = ((mlib_s32 *)srcPixelPtr)[1];
				a3 = ((mlib_s32 *)srcPixelPtr)[2];
				a4 = ((mlib_s32 *)srcPixelPtr)[3];
				srcPixelPtr += 8;

				ch0 = a1 >> 16;
				ch1 = (a1 & 0x7fff) - (a1 & 0x8000);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch0 = a2 >> 16;
				ch1 = (a2 & 0x7fff) - (a2 & 0x8000);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch0 = a3 >> 16;
				ch1 = (a3 & 0x7fff) - (a3 & 0x8000);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch0 = a4 >> 16;
				ch1 = (a4 & 0x7fff) - (a4 & 0x8000);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(1)
#endif /* __SUNPRO_C */
			for (; j < pixelNum; j++) {
#ifdef _LITTLE_ENDIAN
				ch1 = (*srcPixelPtr++);
				ch0 = (*srcPixelPtr++);
#else /* _LITTLE_ENDIAN */
				ch0 = (*srcPixelPtr++);
				ch1 = (*srcPixelPtr++);
#endif /* _LITTLE_ENDIAN */
				sum0 += ch0;
				mtwo0 += ch0 * ch0;
				sum1 += ch1;
				mtwo1 += ch1 * ch1;
			}

#ifdef _LITTLE_ENDIAN
			dsum[1 ^ align] += sum0;
			dsum[align] += sum1;
			moment[1 ^ align] += mtwo0;
			moment[align] += mtwo1;
#else /* _LITTLE_ENDIAN */
			dsum[align] += sum0;
			dsum[1 ^ align] += sum1;
			moment[align] += mtwo0;
			moment[1 ^ align] += mtwo1;
#endif /* _LITTLE_ENDIAN */
			sum0 = sum1 = mtwo0 = mtwo1 = 0;

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_s16 *)srcRowPtr;
		}

		stdev[0] =
		    mlib_sqrt(mlib_fabs(moment[0] * divider -
				dsum[0] * dsum[0] * divider * divider));
		stdev[1] =
		    mlib_sqrt(mlib_fabs(moment[1] * divider -
				dsum[1] * dsum[1] * divider* divider));

	} else if (numOfChannels == 3) {

		if (srcWidth * 3 * 2 == srcYStride) {
			srcWidth = srcTotalPixNum;
			srcHeight = 1;
		}

		pixelNum = srcWidth;

		for (i = 0; i < srcHeight; i++) {
			j = 0;

			if (2 & (mlib_addr)srcPixelPtr) {
				ch0 = (*srcPixelPtr++);
				ch1 = (*srcPixelPtr++);
				ch2 = (*srcPixelPtr++);
				sum0 += ch0;
				sum1 += ch1;
				sum2 += ch2;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
				j = 1;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < pixelNum - 1; j += 2) {
				a1 = ((mlib_s32 *)srcPixelPtr)[0];
				a2 = ((mlib_s32 *)srcPixelPtr)[1];
				a3 = ((mlib_s32 *)srcPixelPtr)[2];
				srcPixelPtr += 6;

#ifdef _LITTLE_ENDIAN
				ch0 = (a1 & 0x7fff) - (a1 & 0x8000);
				ch1 = a1 >> 16;
				ch2 = (a2 & 0x7fff) - (a2 & 0x8000);
#else /* _LITTLE_ENDIAN */
				ch0 = a1 >> 16;
				ch1 = (a1 & 0x7fff) - (a1 & 0x8000);
				ch2 = a2 >> 16;
#endif /* _LITTLE_ENDIAN */
				sum0 += ch0;
				sum1 += ch1;
				sum2 += ch2;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;

#ifdef _LITTLE_ENDIAN
				ch0 = a2 >> 16;
				ch1 = (a3 & 0x7fff) - (a3 & 0x8000);
				ch2 = a3 >> 16;
#else /* _LITTLE_ENDIAN */
				ch0 = (a2 & 0x7fff) - (a2 & 0x8000);
				ch1 = a3 >> 16;
				ch2 = (a3 & 0x7fff) - (a3 & 0x8000);
#endif /* _LITTLE_ENDIAN */
				sum0 += ch0;
				sum1 += ch1;
				sum2 += ch2;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
			}

			if (j < pixelNum) {
				ch0 = (*srcPixelPtr++);
				ch1 = (*srcPixelPtr++);
				ch2 = (*srcPixelPtr++);
				sum0 += ch0;
				sum1 += ch1;
				sum2 += ch2;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
			}

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_s16 *)srcRowPtr;
		}

		stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 * divider -
			sum0 * sum0 * divider * divider));
		stdev[1] = mlib_sqrt(mlib_fabs(mtwo1 * divider -
			sum1 * sum1 * divider * divider));
		stdev[2] = mlib_sqrt(mlib_fabs(mtwo2 * divider -
			sum2 * sum2 * divider * divider));

	} else {
/* case 4: */

		dsum[0] = dsum[1] = moment[0] = moment[1] = 0;
		dsum[2] = dsum[3] = moment[2] = moment[3] = 0;

		if (2 * 4 * srcWidth == srcYStride) {
			srcWidth = srcTotalPixNum;
			srcHeight = 1;
		}

		loadsNum = srcWidth;

		for (i = 0; i < srcHeight; i++) {
			j = align = 0;

			if (2 & (mlib_addr)srcPixelPtr) {
#ifdef _LITTLE_ENDIAN
				ch1 = *(srcPixelPtr + 4 * srcWidth - 1);
				ch0 = (*srcPixelPtr++);
				ch3 = (*srcPixelPtr++);
				ch2 = (*srcPixelPtr++);
#else /* _LITTLE_ENDIAN */
				ch0 = *(srcPixelPtr + 4 * srcWidth - 1);
				ch1 = (*srcPixelPtr++);
				ch2 = (*srcPixelPtr++);
				ch3 = (*srcPixelPtr++);
#endif /* _LITTLE_ENDIAN */
				sum1 += ch1;
				sum2 += ch2;
				sum3 += ch3;
				sum0 += ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
				mtwo3 += ch3 * ch3;
				mtwo0 += ch0 * ch0;
				j = 1;
				align = 1;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= loadsNum - 2; j += 2) {
				a1 = ((mlib_s32 *)srcPixelPtr)[0];
				a2 = ((mlib_s32 *)srcPixelPtr)[1];
				a3 = ((mlib_s32 *)srcPixelPtr)[2];
				a4 = ((mlib_s32 *)srcPixelPtr)[3];
				srcPixelPtr += 8;

				ch0 = a1 >> 16;
				ch1 = (a1 & 0x7fff) - (a1 & 0x8000);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch2 = a2 >> 16;
				ch3 = (a2 & 0x7fff) - (a2 & 0x8000);
				sum2 += ch2;
				sum3 += ch3;
				mtwo2 += ch2 * ch2;
				mtwo3 += ch3 * ch3;

				ch0 = a3 >> 16;
				ch1 = (a3 & 0x7fff) - (a3 & 0x8000);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch2 = a4 >> 16;
				ch3 = (a4 & 0x7fff) - (a4 & 0x8000);
				sum2 += ch2;
				sum3 += ch3;
				mtwo2 += ch2 * ch2;
				mtwo3 += ch3 * ch3;
			}

			if (j < loadsNum) {
				a1 = ((mlib_s32 *)srcPixelPtr)[0];
				a2 = ((mlib_s32 *)srcPixelPtr)[1];
				srcPixelPtr += 4;

				ch0 = a1 >> 16;
				ch1 = (a1 & 0x7fff) - (a1 & 0x8000);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch2 = a2 >> 16;
				ch3 = (a2 & 0x7fff) - (a2 & 0x8000);
				sum2 += ch2;
				sum3 += ch3;
				mtwo2 += ch2 * ch2;
				mtwo3 += ch3 * ch3;
			}
#ifdef _LITTLE_ENDIAN
			dsum[3 & (0 - align)] += sum1;
			dsum[1 - align] += sum0;
			dsum[2 - align] += sum3;
			dsum[3 - align] += sum2;
			moment[3 & (0 - align)] += mtwo1;
			moment[1 - align] += mtwo0;
			moment[2 - align] += mtwo3;
			moment[3 - align] += mtwo2;
#else /* _LITTLE_ENDIAN */
			dsum[3 & (0 - align)] += sum0;
			dsum[1 - align] += sum1;
			dsum[2 - align] += sum2;
			dsum[3 - align] += sum3;
			moment[3 & (0 - align)] += mtwo0;
			moment[1 - align] += mtwo1;
			moment[2 - align] += mtwo2;
			moment[3 - align] += mtwo3;
#endif /* _LITTLE_ENDIAN */

			sum0 = sum1 = mtwo0 = mtwo1 =
			    sum2 = sum3 = mtwo2 = mtwo3 = 0.0;
			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_s16 *)srcRowPtr;
		}

		stdev[0] =
		    mlib_sqrt(mlib_fabs(moment[0] * divider -
				dsum[0] * dsum[0] * divider * divider));
		stdev[1] =
		    mlib_sqrt(mlib_fabs(moment[1] * divider -
				dsum[1] * dsum[1] * divider * divider));
		stdev[2] =
		    mlib_sqrt(mlib_fabs(moment[2] * divider -
				dsum[2] * dsum[2] * divider * divider));
		stdev[3] =
		    mlib_sqrt(mlib_fabs(moment[3] * divider -
				dsum[3] * dsum[3] * divider * divider));
	}
}

/* *********************************************************** */

void
mlib_c_ImageStdDev2ParamS32(
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
	mlib_s64 sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0;
	mlib_s64 ch0, ch1, ch2, ch3;
	mlib_s64 mtwo0 = 0, mtwo1 = 0, mtwo2 = 0, mtwo3 = 0;

	mlib_d64 divider = 1 / (mlib_d64)srcTotalPixNum;

	mlib_s64 acc0, acc1, acc2, acc3;
	mlib_s32 val;
	mlib_s64 hi, lo;
	mlib_s64 mtwo0_hi, mtwo0_lo, mtwo0_err;
	mlib_s64 mtwo1_hi, mtwo1_lo, mtwo1_err;
	mlib_s64 mtwo2_hi, mtwo2_lo, mtwo2_err;
	mlib_s64 mtwo3_hi, mtwo3_lo, mtwo3_err;

	mtwo0_hi = mtwo0_lo = mtwo0_err = 0;
	mtwo1_hi = mtwo1_lo = mtwo1_err = 0;
	mtwo2_hi = mtwo2_lo = mtwo2_err = 0;
	mtwo3_hi = mtwo3_lo = mtwo3_err = 0;

	mlib_d64 mrez[4];
	mrez[0] = mrez[1] = mrez[2] = mrez[3] = 0.0;

	if (numOfChannels * srcWidth * 4 == srcYStride) {
		srcWidth = srcHeight * srcWidth;
		srcHeight = 1;
	}

	if (numOfChannels != 3) {

		pixelNumber = numOfChannels * srcWidth;
		loadsNumber = pixelNumber >> 2;
		pixelNumber &= 3;
		acc0 = acc1 = acc2 = acc3 = 0;

		for (j = 0; j < srcHeight; j++) {
#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
			for (k = 0; k < loadsNumber; k++) {
				val = srcPixelPtr[0];
				acc0 += val;
				hi = val >> 16;
				lo = val & 0xffff;
				mtwo0_hi += hi * hi;
				mtwo0_lo += lo * lo;
				mtwo0_err += hi * lo;

				val = srcPixelPtr[1];
				acc1 += val;
				hi = val >> 16;
				lo = val & 0xffff;
				mtwo1_hi += hi * hi;
				mtwo1_lo += lo * lo;
				mtwo1_err += hi * lo;

				val = srcPixelPtr[2];
				acc2 += val;
				hi = val >> 16;
				lo = val & 0xffff;
				mtwo2_hi += hi * hi;
				mtwo2_lo += lo * lo;
				mtwo2_err += hi * lo;

				val = srcPixelPtr[3];
				acc3 += val;
				hi = val >> 16;
				lo = val & 0xffff;
				mtwo3_hi += hi * hi;
				mtwo3_lo += lo * lo;
				mtwo3_err += hi * lo;

				srcPixelPtr += 4;
			}

			switch (pixelNumber) {

			case 3:
				ch0 = (*srcPixelPtr++);
				ch1 = (*srcPixelPtr++);
				ch2 = *srcPixelPtr;
				hi = ch0 >> 16;
				lo = ch0 & 0xffff;
				mtwo0_hi += hi * hi;
				mtwo0_lo += lo * lo;
				mtwo0_err += hi * lo;

				hi = ch1 >> 16;
				lo = ch1 & 0xffff;
				mtwo1_hi += hi * hi;
				mtwo1_lo += lo * lo;
				mtwo1_err += hi * lo;

				hi = ch2 >> 16;
				lo = ch2 & 0xffff;
				mtwo2_hi += hi * hi;
				mtwo2_lo += lo * lo;
				mtwo2_err += hi * lo;

				sum0 += ch0;
				sum1 += ch1;
				sum2 += ch2;
				break;

			case 2:
				ch0 = (*srcPixelPtr++);
				ch1 = (*srcPixelPtr++);
				hi = ch0 >> 16;
				lo = ch0 & 0xffff;
				mtwo0_hi += hi * hi;
				mtwo0_lo += lo * lo;
				mtwo0_err += hi * lo;

				hi = ch1 >> 16;
				lo = ch1 & 0xffff;
				mtwo1_hi += hi * hi;
				mtwo1_lo += lo * lo;
				mtwo1_err += hi * lo;

				sum0 += ch0;
				sum1 += ch1;
				break;

			case 1:
				ch0 = (*srcPixelPtr++);
				hi = ch0 >> 16;
				lo = ch0 & 0xffff;
				mtwo0_hi += hi * hi;
				mtwo0_lo += lo * lo;
				mtwo0_err += hi * lo;

				sum0 += ch0;
				break;
			}

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_s32 *)srcRowPtr;
		}

		sum0 += acc0;
		sum1 += acc1;
		sum2 += acc2;
		sum3 += acc3;
	} else {
/* case 3: */

		pixelNumber = srcWidth;
		acc0 = acc1 = acc2 = acc3 = 0;
		for (j = 0; j < srcHeight; j++) {
			for (k = 0; k < pixelNumber; k++) {

				acc0 += *srcPixelPtr;
				val = (*srcPixelPtr++);
				hi = val >> 16;
				lo = val & 0xffff;
				mtwo0_hi += hi * hi;
				mtwo0_lo += lo * lo;
				mtwo0_err += hi * lo;

				acc1 += *srcPixelPtr;
				val = (*srcPixelPtr++);
				hi = val >> 16;
				lo = val & 0xffff;
				mtwo1_hi += hi * hi;
				mtwo1_lo += lo * lo;
				mtwo1_err += hi * lo;

				acc2 += *srcPixelPtr;
				val = (*srcPixelPtr++);
				hi = val >> 16;
				lo = val & 0xffff;
				mtwo2_hi += hi * hi;
				mtwo2_lo += lo * lo;
				mtwo2_err += hi * lo;
			}

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_s32 *)srcRowPtr;
		}

		sum0 += acc0;
		sum1 += acc1;
		sum2 += acc2;
	}

	switch (numOfChannels) {

	case 1:
		mrez[0] = mtwo0_lo + (mlib_d64)mtwo0_hi * ((mlib_s64)1 << 32) +
			(mlib_d64)mtwo0_err * (1 << 17);
		mrez[1] = mtwo1_lo + (mlib_d64)mtwo1_hi * ((mlib_s64)1 << 32) +
			(mlib_d64)mtwo1_err * (1 << 17);
		mrez[2] = mtwo2_lo + (mlib_d64)mtwo2_hi * ((mlib_s64)1 << 32) +
			(mlib_d64)mtwo2_err * (1 << 17);
		mrez[3] = mtwo3_lo + (mlib_d64)mtwo3_hi * ((mlib_s64)1 << 32) +
			(mlib_d64)mtwo3_err * (1 << 17);
		mrez[0] = (mrez[0] + mrez[1] + mrez[2] + mrez[3]) * divider;
		mlib_d64 tmp;
		tmp = (mlib_d64)sum0 + (mlib_d64)sum1 +
				(mlib_d64)sum2 + (mlib_d64)sum3;

		stdev[0] = mlib_sqrt(mlib_fabs(mrez[0] -
				tmp * tmp * divider * divider));
		break;

	case 2:
		mrez[0] = mtwo0_lo + (mlib_d64)mtwo0_hi * ((mlib_s64)1 << 32) +
			(mlib_d64)mtwo0_err * (1 << 17);
		mrez[1] = mtwo1_lo + (mlib_d64)mtwo1_hi * ((mlib_s64)1 << 32) +
			(mlib_d64)mtwo1_err * (1 << 17);
		mrez[2] = mtwo2_lo + (mlib_d64)mtwo2_hi * ((mlib_s64)1 << 32) +
			(mlib_d64)mtwo2_err * (1 << 17);
		mrez[3] = mtwo3_lo + (mlib_d64)mtwo3_hi * ((mlib_s64)1 << 32) +
			(mlib_d64)mtwo3_err * (1 << 17);
		mrez[0] = (mrez[0] + mrez[2]) * divider;
		mrez[1] = (mrez[1] + mrez[3]) * divider;
		mlib_d64 tmp0, tmp1;
		tmp0 = (mlib_d64)sum0 + (mlib_d64)sum2;
		tmp1 = (mlib_d64)sum1 + (mlib_d64)sum3;

		stdev[0] = mlib_sqrt(mlib_fabs(mrez[0] -
			tmp0 * tmp0 * divider * divider));
		stdev[1] = mlib_sqrt(mlib_fabs(mrez[1] -
			tmp1 * tmp1 * divider * divider));
		break;

	case 3:
		mrez[0] = (mtwo0_lo + (mlib_d64)mtwo0_hi * ((mlib_s64)1 << 32) +
			(mlib_d64)mtwo0_err * (1 << 17)) * divider;
		mrez[1] = (mtwo1_lo + (mlib_d64)mtwo1_hi * ((mlib_s64)1 << 32) +
			(mlib_d64)mtwo1_err * (1 << 17)) * divider;
		mrez[2] = (mtwo2_lo + (mlib_d64)mtwo2_hi * ((mlib_s64)1 << 32) +
			(mlib_d64)mtwo2_err * (1 << 17)) * divider;
		stdev[0] = mlib_sqrt(mlib_fabs(mrez[0] -
			(mlib_d64)sum0 * (mlib_d64)sum0 * divider * divider));
		stdev[1] = mlib_sqrt(mlib_fabs(mrez[1] -
			(mlib_d64)sum1 * (mlib_d64)sum1 * divider * divider));
		stdev[2] = mlib_sqrt(mlib_fabs(mrez[2] -
			(mlib_d64)sum2 * (mlib_d64)sum2 * divider * divider));

		break;

	case 4:
		mrez[0] = (mtwo0_lo + (mlib_d64)mtwo0_hi * ((mlib_s64)1 << 32) +
			(mlib_d64)mtwo0_err * (1 << 17)) * divider;
		mrez[1] = (mtwo1_lo + (mlib_d64)mtwo1_hi * ((mlib_s64)1 << 32) +
			(mlib_d64)mtwo1_err * (1 << 17)) * divider;
		mrez[2] = (mtwo2_lo + (mlib_d64)mtwo2_hi * ((mlib_s64)1 << 32) +
			(mlib_d64)mtwo2_err * (1 << 17)) * divider;
		mrez[3] = (mtwo3_lo + (mlib_d64)mtwo3_hi * ((mlib_s64)1 << 32) +
			(mlib_d64)mtwo3_err * (1 << 17)) * divider;
		stdev[0] = mlib_sqrt(mlib_fabs(mrez[0] -
			(mlib_d64)sum0 * (mlib_d64)sum0 * divider * divider));
		stdev[1] = mlib_sqrt(mlib_fabs(mrez[1] -
			(mlib_d64)sum1 * (mlib_d64)sum1 * divider * divider));
		stdev[2] = mlib_sqrt(mlib_fabs(mrez[2] -
			(mlib_d64)sum2 * (mlib_d64)sum2 * divider * divider));
		stdev[3] = mlib_sqrt(mlib_fabs(mrez[3] -
			(mlib_d64)sum3 * (mlib_d64)sum3 * divider * divider));

		break;
	}
}

/* *********************************************************** */
#else /* else of ifndef _NO_LONGLONG */
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

static void mlib_c_ImageStdDev2ParamU8(
    const mlib_image *src,
    mlib_d64 *stdev);

static void mlib_c_ImageStdDev2ParamU16(
    const mlib_image *src,
    mlib_d64 *stdev);

static void mlib_c_ImageStdDev2ParamS16(
    const mlib_image *src,
    mlib_d64 *stdev);

static void mlib_c_ImageStdDev2ParamS32(
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
			mlib_c_ImageStdDev2ParamU8(src, stdev);
			return (MLIB_SUCCESS);

		case MLIB_SHORT:
			mlib_c_ImageStdDev2ParamS16(src, stdev);
			return (MLIB_SUCCESS);

		case MLIB_USHORT:
			mlib_c_ImageStdDev2ParamU16(src, stdev);
			return (MLIB_SUCCESS);

		case MLIB_INT:
			mlib_c_ImageStdDev2ParamS32(src, stdev);
			return (MLIB_SUCCESS);

		default:
			return (MLIB_FAILURE);
		}
	}

	switch (mlib_ImageGetType(src)) {
	case MLIB_BYTE:

		if (num_channels != 3)
			mlib_c_ImageMoment2_U8_124(src, rez);
		else
			mlib_c_ImageMoment2_U8_3(src, rez);
		break;

	case MLIB_USHORT:

		if (num_channels != 3)
			mlib_c_ImageMoment2_U16_124(src, rez);
		else
			mlib_c_ImageMoment2_U16_3(src, rez);
		break;

	case MLIB_SHORT:

		if (num_channels != 3)
			mlib_c_ImageMoment2_S16_124(src, rez);
		else
			mlib_c_ImageMoment2_S16_3(src, rez);
		break;

	case MLIB_INT:

		if (num_channels != 3)
			mlib_c_ImageMoment2_S32_124(src, rez);
		else
			mlib_c_ImageMoment2_S32_3(src, rez);
		break;

	default:
		return (MLIB_FAILURE);
	}

	divider =
	    1.0 / ((mlib_d64)mlib_ImageGetWidth(src) *
	    mlib_ImageGetHeight(src));
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
mlib_c_ImageStdDev2ParamU8(
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

/* linestride = bytes to next row */
	mlib_s32 srcYStride = mlib_ImageGetStride(src);

/* number of channels */
	mlib_s32 numOfChannels = mlib_ImageGetChannels(src);

/* total number of pixels */
	mlib_s32 srcTotalPixNum = srcHeight * srcWidth;

/* indices */
	mlib_s32 i, j;
	mlib_s32 loadsNum, pixelNum;
	mlib_d64 ch0, ch1, ch2, ch3;
	mlib_d64 sum0 = 0.0, sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
	mlib_d64 mtwo0 = 0.0, mtwo1 = 0.0, mtwo2 = 0.0, mtwo3 = 0.0;
	mlib_d64 divider = 1.0 / (mlib_d64)srcTotalPixNum;

	if (numOfChannels != 3) {

		pixelNum = srcWidth << (numOfChannels >> 1);

		if (pixelNum == srcYStride) {
			pixelNum = srcTotalPixNum << (numOfChannels >> 1);
			srcHeight = 1;
		}

		loadsNum = pixelNum >> 2;
		pixelNum &= 3;

		for (i = 0; i < srcHeight; i++) {
			for (j = 0; j < loadsNum; j++) {
				ch0 = mlib_U82D64[(*srcPixelPtr++)];
				ch1 = mlib_U82D64[(*srcPixelPtr++)];
				ch2 = mlib_U82D64[(*srcPixelPtr++)];
				ch3 = mlib_U82D64[(*srcPixelPtr++)];
				sum0 += ch0;
				sum1 += ch1;
				sum2 += ch2;
				sum3 += ch3;

				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
				mtwo3 += ch3 * ch3;
			}

			switch (pixelNum) {

			case 3:
				ch0 = mlib_U82D64[(*srcPixelPtr++)];
				ch1 = mlib_U82D64[(*srcPixelPtr++)];
				ch2 = mlib_U82D64[*srcPixelPtr];
				sum0 += ch0;
				sum1 += ch1;
				sum2 += ch2;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
				break;

			case 2:
				ch0 = mlib_U82D64[(*srcPixelPtr++)];
				ch1 = mlib_U82D64[*srcPixelPtr];
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				break;

			case 1:
				ch0 = mlib_U82D64[*srcPixelPtr];
				sum0 += ch0;
				mtwo0 += ch0 * ch0;
				break;
			}

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_u8 *)srcRowPtr;
		}

	} else {
/* case 3: */

		if (srcWidth * 3 == srcYStride) {
			srcWidth = srcTotalPixNum;
			srcHeight = 1;
		}

		pixelNum = srcWidth;

		for (i = 0; i < srcHeight; i++) {
			for (j = 0; j < pixelNum; j++) {
				ch0 = mlib_U82D64[(*srcPixelPtr++)];
				ch1 = mlib_U82D64[(*srcPixelPtr++)];
				ch2 = mlib_U82D64[(*srcPixelPtr++)];
				sum0 += ch0;
				sum1 += ch1;
				sum2 += ch2;

				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
			}

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_u8 *)srcRowPtr;
		}
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

void
mlib_c_ImageStdDev2ParamU16(
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

/* linestride = bytes to next row */
	mlib_s32 srcYStride = mlib_ImageGetStride(src);

/* number of channels */
	mlib_s32 numOfChannels = mlib_ImageGetChannels(src);

/* total number of pixels */
	mlib_s32 srcTotalPixNum = srcHeight * srcWidth;

/* indices */
	mlib_s32 i, j;
	mlib_s32 loadsNum, pixelNum;
	mlib_d64 ch0, ch1, ch2, ch3;
	mlib_d64 sum0 = 0.0, sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
	mlib_d64 mtwo0 = 0.0, mtwo1 = 0.0, mtwo2 = 0.0, mtwo3 = 0.0;
	mlib_d64 divider = 1 / (mlib_d64)srcTotalPixNum;
	mlib_u32 a1, a2, a3, a4;
	mlib_s32 align;
	mlib_d64 dsum[4], moment[4];

	if (numOfChannels == 1) {

		if (2 * srcWidth == srcYStride) {
			srcWidth = srcTotalPixNum;
			srcHeight = 1;
		}

		pixelNum = srcWidth;

		for (i = 0; i < srcHeight; i++) {
			j = 0;

			if (2 & (mlib_addr)srcPixelPtr) {
				ch0 = (*srcPixelPtr++);
				sum0 += ch0;
				mtwo0 += ch0 * ch0;
				j = 1;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= pixelNum - 8; j += 8) {
				a1 = ((mlib_u32 *)srcPixelPtr)[0];
				a2 = ((mlib_u32 *)srcPixelPtr)[1];
				a3 = ((mlib_u32 *)srcPixelPtr)[2];
				a4 = ((mlib_u32 *)srcPixelPtr)[3];
				srcPixelPtr += 8;

				ch0 = (mlib_s32)(a1 >> 16);
				ch1 = (mlib_s32)(a1 & 0xffff);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch0 = (mlib_s32)(a2 >> 16);
				ch1 = (mlib_s32)(a2 & 0xffff);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch0 = (mlib_s32)(a3 >> 16);
				ch1 = (mlib_s32)(a3 & 0xffff);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch0 = (mlib_s32)(a4 >> 16);
				ch1 = (mlib_s32)(a4 & 0xffff);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(1)
#endif /* __SUNPRO_C */
			for (; j < pixelNum; j++) {
				ch0 = (*srcPixelPtr++);
				sum0 += ch0;
				mtwo0 += ch0 * ch0;
			}

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_u16 *)srcRowPtr;
		}

		sum0 += sum1;
		mtwo0 += mtwo1;
		sum0 *= divider;
		stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 * divider - sum0 * sum0));

	} else if (numOfChannels == 2) {

		dsum[0] = dsum[1] = moment[0] = moment[1] = 0;

		if (2 * 2 * srcWidth == srcYStride) {
			srcWidth = srcTotalPixNum;
			srcHeight = 1;
		}

		pixelNum = srcWidth;

		for (i = 0; i < srcHeight; i++) {
			j = align = 0;

			if (2 & (mlib_addr)srcPixelPtr) {
#ifdef _LITTLE_ENDIAN
				ch1 = *(srcPixelPtr + 2 * srcWidth - 1);
				ch0 = (*srcPixelPtr++);
#else /* _LITTLE_ENDIAN */
				ch0 = *(srcPixelPtr + 2 * srcWidth - 1);
				ch1 = (*srcPixelPtr++);
#endif /* _LITTLE_ENDIAN */
				sum1 += ch1;
				sum0 += ch0;
				mtwo1 += ch1 * ch1;
				mtwo0 += ch0 * ch0;
				j = 1;
				align = 1;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= pixelNum - 4; j += 4) {
				a1 = ((mlib_s32 *)srcPixelPtr)[0];
				a2 = ((mlib_s32 *)srcPixelPtr)[1];
				a3 = ((mlib_s32 *)srcPixelPtr)[2];
				a4 = ((mlib_s32 *)srcPixelPtr)[3];
				srcPixelPtr += 8;

				ch0 = (mlib_s32)(a1 >> 16);
				ch1 = (mlib_s32)(a1 & 0xffff);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch0 = (mlib_s32)(a2 >> 16);
				ch1 = (mlib_s32)(a2 & 0xffff);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch0 = (mlib_s32)(a3 >> 16);
				ch1 = (mlib_s32)(a3 & 0xffff);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch0 = (mlib_s32)(a4 >> 16);
				ch1 = (mlib_s32)(a4 & 0xffff);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(1)
#endif /* __SUNPRO_C */
			for (; j < pixelNum; j++) {
#ifdef _LITTLE_ENDIAN
				ch1 = (*srcPixelPtr++);
				ch0 = (*srcPixelPtr++);
#else /* _LITTLE_ENDIAN */
				ch0 = (*srcPixelPtr++);
				ch1 = (*srcPixelPtr++);
#endif /* _LITTLE_ENDIAN */
				sum0 += ch0;
				mtwo0 += ch0 * ch0;
				sum1 += ch1;
				mtwo1 += ch1 * ch1;
			}

#ifdef _LITTLE_ENDIAN
			dsum[1 ^ align] += sum0;
			dsum[align] += sum1;
			moment[1 ^ align] += mtwo0;
			moment[align] += mtwo1;
#else /* _LITTLE_ENDIAN */
			dsum[align] += sum0;
			dsum[1 ^ align] += sum1;
			moment[align] += mtwo0;
			moment[1 ^ align] += mtwo1;
#endif /* _LITTLE_ENDIAN */
			sum0 = sum1 = mtwo0 = mtwo1 = 0.0;

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_u16 *)srcRowPtr;
		}

		sum0 = dsum[0] * divider;
		stdev[0] =
		    mlib_sqrt(mlib_fabs(moment[0] * divider - sum0 * sum0));
		sum1 = dsum[1] * divider;
		stdev[1] =
		    mlib_sqrt(mlib_fabs(moment[1] * divider - sum1 * sum1));

	} else if (numOfChannels == 3) {

		if (srcWidth * 3 * 2 == srcYStride) {
			srcWidth = srcTotalPixNum;
			srcHeight = 1;
		}

		pixelNum = srcWidth;

		for (i = 0; i < srcHeight; i++) {
			j = 0;

			if (2 & (mlib_addr)srcPixelPtr) {
				ch0 = (*srcPixelPtr++);
				ch1 = (*srcPixelPtr++);
				ch2 = (*srcPixelPtr++);
				sum0 += ch0;
				sum1 += ch1;
				sum2 += ch2;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
				j = 1;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < pixelNum - 1; j += 2) {
				a1 = ((mlib_s32 *)srcPixelPtr)[0];
				a2 = ((mlib_s32 *)srcPixelPtr)[1];
				a3 = ((mlib_s32 *)srcPixelPtr)[2];
				srcPixelPtr += 6;

#ifdef _LITTLE_ENDIAN
				ch0 = (mlib_s32)(a1 & 0xffff);
				ch1 = (mlib_s32)(a1 >> 16);
				ch2 = (mlib_s32)(a2 & 0xffff);
#else /* _LITTLE_ENDIAN */
				ch0 = (mlib_s32)(a1 >> 16);
				ch1 = (mlib_s32)(a1 & 0xffff);
				ch2 = (mlib_s32)(a2 >> 16);
#endif /* _LITTLE_ENDIAN */
				sum0 += ch0;
				sum1 += ch1;
				sum2 += ch2;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;

#ifdef _LITTLE_ENDIAN
				ch0 = (mlib_s32)(a2 >> 16);
				ch1 = (mlib_s32)(a3 & 0xffff);
				ch2 = (mlib_s32)(a3 >> 16);
#else /* _LITTLE_ENDIAN */
				ch0 = (mlib_s32)(a2 & 0xffff);
				ch1 = (mlib_s32)(a3 >> 16);
				ch2 = (mlib_s32)(a3 & 0xffff);
#endif /* _LITTLE_ENDIAN */
				sum0 += ch0;
				sum1 += ch1;
				sum2 += ch2;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
			}

			if (j < pixelNum) {
				ch0 = (*srcPixelPtr++);
				ch1 = (*srcPixelPtr++);
				ch2 = (*srcPixelPtr++);
				sum0 += ch0;
				sum1 += ch1;
				sum2 += ch2;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
			}

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_u16 *)srcRowPtr;
		}

		sum0 *= divider;
		stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 * divider - sum0 * sum0));
		sum1 *= divider;
		stdev[1] = mlib_sqrt(mlib_fabs(mtwo1 * divider - sum1 * sum1));
		sum2 *= divider;
		stdev[2] = mlib_sqrt(mlib_fabs(mtwo2 * divider - sum2 * sum2));

	} else {
/* case 4: */

		dsum[0] = dsum[1] = moment[0] = moment[1] = 0;
		dsum[2] = dsum[3] = moment[2] = moment[3] = 0;

		if (2 * 4 * srcWidth == srcYStride) {
			srcWidth = srcTotalPixNum;
			srcHeight = 1;
		}

		loadsNum = srcWidth;

		for (i = 0; i < srcHeight; i++) {
			j = align = 0;

			if (2 & (mlib_addr)srcPixelPtr) {
#ifdef _LITTLE_ENDIAN
				ch1 = *(srcPixelPtr + 4 * srcWidth - 1);
				ch0 = (*srcPixelPtr++);
				ch3 = (*srcPixelPtr++);
				ch2 = (*srcPixelPtr++);
#else /* _LITTLE_ENDIAN */
				ch0 = *(srcPixelPtr + 4 * srcWidth - 1);
				ch1 = (*srcPixelPtr++);
				ch2 = (*srcPixelPtr++);
				ch3 = (*srcPixelPtr++);
#endif /* _LITTLE_ENDIAN */
				sum1 += ch1;
				sum2 += ch2;
				sum3 += ch3;
				sum0 += ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
				mtwo3 += ch3 * ch3;
				mtwo0 += ch0 * ch0;
				j = 1;
				align = 1;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= loadsNum - 2; j += 2) {
				a1 = ((mlib_s32 *)srcPixelPtr)[0];
				a2 = ((mlib_s32 *)srcPixelPtr)[1];
				a3 = ((mlib_s32 *)srcPixelPtr)[2];
				a4 = ((mlib_s32 *)srcPixelPtr)[3];
				srcPixelPtr += 8;

				ch0 = (mlib_s32)(a1 >> 16);
				ch1 = (mlib_s32)(a1 & 0xffff);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch2 = (mlib_s32)(a2 >> 16);
				ch3 = (mlib_s32)(a2 & 0xffff);
				sum2 += ch2;
				sum3 += ch3;
				mtwo2 += ch2 * ch2;
				mtwo3 += ch3 * ch3;

				ch0 = (mlib_s32)(a3 >> 16);
				ch1 = (mlib_s32)(a3 & 0xffff);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch2 = (mlib_s32)(a4 >> 16);
				ch3 = (mlib_s32)(a4 & 0xffff);
				sum2 += ch2;
				sum3 += ch3;
				mtwo2 += ch2 * ch2;
				mtwo3 += ch3 * ch3;
			}

			if (j < loadsNum) {
				a1 = ((mlib_s32 *)srcPixelPtr)[0];
				a2 = ((mlib_s32 *)srcPixelPtr)[1];
				srcPixelPtr += 4;

				ch0 = (mlib_s32)(a1 >> 16);
				ch1 = (mlib_s32)(a1 & 0xffff);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch2 = (mlib_s32)(a2 >> 16);
				ch3 = (mlib_s32)(a2 & 0xffff);
				sum2 += ch2;
				sum3 += ch3;
				mtwo2 += ch2 * ch2;
				mtwo3 += ch3 * ch3;
			}
#ifdef _LITTLE_ENDIAN
			dsum[3 & (0 - align)] += sum1;
			dsum[1 - align] += sum0;
			dsum[2 - align] += sum3;
			dsum[3 - align] += sum2;
			moment[3 & (0 - align)] += mtwo1;
			moment[1 - align] += mtwo0;
			moment[2 - align] += mtwo3;
			moment[3 - align] += mtwo2;
#else /* _LITTLE_ENDIAN */
			dsum[3 & (0 - align)] += sum0;
			dsum[1 - align] += sum1;
			dsum[2 - align] += sum2;
			dsum[3 - align] += sum3;
			moment[3 & (0 - align)] += mtwo0;
			moment[1 - align] += mtwo1;
			moment[2 - align] += mtwo2;
			moment[3 - align] += mtwo3;
#endif /* _LITTLE_ENDIAN */

			sum0 = sum1 = mtwo0 = mtwo1 =
			    sum2 = sum3 = mtwo2 = mtwo3 = 0.0;
			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_u16 *)srcRowPtr;
		}

		sum0 = dsum[0] * divider;
		stdev[0] =
		    mlib_sqrt(mlib_fabs(moment[0] * divider - sum0 * sum0));
		sum1 = dsum[1] * divider;
		stdev[1] =
		    mlib_sqrt(mlib_fabs(moment[1] * divider - sum1 * sum1));
		sum2 = dsum[2] * divider;
		stdev[2] =
		    mlib_sqrt(mlib_fabs(moment[2] * divider - sum2 * sum2));
		sum3 = dsum[3] * divider;
		stdev[3] =
		    mlib_sqrt(mlib_fabs(moment[3] * divider - sum3 * sum3));
	}
}

/* *********************************************************** */

void
mlib_c_ImageStdDev2ParamS16(
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

/* linestride = bytes to next row */
	mlib_s32 srcYStride = mlib_ImageGetStride(src);

/* number of channels */
	mlib_s32 numOfChannels = mlib_ImageGetChannels(src);

/* total number of pixels */
	mlib_s32 srcTotalPixNum = srcHeight * srcWidth;

/* indices */
	mlib_s32 i, j;
	mlib_s32 loadsNum, pixelNum;
	mlib_d64 ch0, ch1, ch2, ch3;
	mlib_d64 sum0 = 0.0, sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
	mlib_d64 mtwo0 = 0.0, mtwo1 = 0.0, mtwo2 = 0.0, mtwo3 = 0.0;
	mlib_d64 divider = 1 / (mlib_d64)srcTotalPixNum;
	mlib_s32 a1, a2, a3, a4, align;
	mlib_d64 dsum[4], moment[4];

	if (numOfChannels == 1) {

		if (2 * srcWidth == srcYStride) {
			srcWidth = srcTotalPixNum;
			srcHeight = 1;
		}

		pixelNum = srcWidth;

		for (i = 0; i < srcHeight; i++) {
			j = 0;

			if (2 & (mlib_addr)srcPixelPtr) {
				ch0 = (*srcPixelPtr++);
				sum0 += ch0;
				mtwo0 += ch0 * ch0;
				j = 1;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= pixelNum - 8; j += 8) {
				a1 = ((mlib_s32 *)srcPixelPtr)[0];
				a2 = ((mlib_s32 *)srcPixelPtr)[1];
				a3 = ((mlib_s32 *)srcPixelPtr)[2];
				a4 = ((mlib_s32 *)srcPixelPtr)[3];
				srcPixelPtr += 8;

				ch0 = a1 >> 16;
				ch1 = (a1 & 0x7fff) - (a1 & 0x8000);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch0 = a2 >> 16;
				ch1 = (a2 & 0x7fff) - (a2 & 0x8000);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch0 = a3 >> 16;
				ch1 = (a3 & 0x7fff) - (a3 & 0x8000);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch0 = a4 >> 16;
				ch1 = (a4 & 0x7fff) - (a4 & 0x8000);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(1)
#endif /* __SUNPRO_C */
			for (; j < pixelNum; j++) {
				ch0 = (*srcPixelPtr++);
				sum0 += ch0;
				mtwo0 += ch0 * ch0;
			}

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_s16 *)srcRowPtr;
		}

		sum0 += sum1;
		mtwo0 += mtwo1;
		sum0 *= divider;
		stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 * divider - sum0 * sum0));

	} else if (numOfChannels == 2) {

		dsum[0] = dsum[1] = moment[0] = moment[1] = 0;

		if (2 * 2 * srcWidth == srcYStride) {
			srcWidth = srcTotalPixNum;
			srcHeight = 1;
		}

		pixelNum = srcWidth;

		for (i = 0; i < srcHeight; i++) {
			j = align = 0;

			if (2 & (mlib_addr)srcPixelPtr) {
#ifdef _LITTLE_ENDIAN
				ch1 = *(srcPixelPtr + 2 * srcWidth - 1);
				ch0 = (*srcPixelPtr++);
#else /* _LITTLE_ENDIAN */
				ch0 = *(srcPixelPtr + 2 * srcWidth - 1);
				ch1 = (*srcPixelPtr++);
#endif /* _LITTLE_ENDIAN */
				sum1 += ch1;
				sum0 += ch0;
				mtwo1 += ch1 * ch1;
				mtwo0 += ch0 * ch0;
				j = 1;
				align = 1;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= pixelNum - 4; j += 4) {
				a1 = ((mlib_s32 *)srcPixelPtr)[0];
				a2 = ((mlib_s32 *)srcPixelPtr)[1];
				a3 = ((mlib_s32 *)srcPixelPtr)[2];
				a4 = ((mlib_s32 *)srcPixelPtr)[3];
				srcPixelPtr += 8;

				ch0 = a1 >> 16;
				ch1 = (a1 & 0x7fff) - (a1 & 0x8000);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch0 = a2 >> 16;
				ch1 = (a2 & 0x7fff) - (a2 & 0x8000);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch0 = a3 >> 16;
				ch1 = (a3 & 0x7fff) - (a3 & 0x8000);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch0 = a4 >> 16;
				ch1 = (a4 & 0x7fff) - (a4 & 0x8000);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(1)
#endif /* __SUNPRO_C */
			for (; j < pixelNum; j++) {
#ifdef _LITTLE_ENDIAN
				ch1 = (*srcPixelPtr++);
				ch0 = (*srcPixelPtr++);
#else /* _LITTLE_ENDIAN */
				ch0 = (*srcPixelPtr++);
				ch1 = (*srcPixelPtr++);
#endif /* _LITTLE_ENDIAN */
				sum0 += ch0;
				mtwo0 += ch0 * ch0;
				sum1 += ch1;
				mtwo1 += ch1 * ch1;
			}

#ifdef _LITTLE_ENDIAN
			dsum[1 ^ align] += sum0;
			dsum[align] += sum1;
			moment[1 ^ align] += mtwo0;
			moment[align] += mtwo1;
#else /* _LITTLE_ENDIAN */
			dsum[align] += sum0;
			dsum[1 ^ align] += sum1;
			moment[align] += mtwo0;
			moment[1 ^ align] += mtwo1;
#endif /* _LITTLE_ENDIAN */
			sum0 = sum1 = mtwo0 = mtwo1 = 0.0;

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_s16 *)srcRowPtr;
		}

		sum0 = dsum[0] * divider;
		stdev[0] =
		    mlib_sqrt(mlib_fabs(moment[0] * divider - sum0 * sum0));
		sum1 = dsum[1] * divider;
		stdev[1] =
		    mlib_sqrt(mlib_fabs(moment[1] * divider - sum1 * sum1));

	} else if (numOfChannels == 3) {

		if (srcWidth * 3 * 2 == srcYStride) {
			srcWidth = srcTotalPixNum;
			srcHeight = 1;
		}

		pixelNum = srcWidth;

		for (i = 0; i < srcHeight; i++) {
			j = 0;

			if (2 & (mlib_addr)srcPixelPtr) {
				ch0 = (*srcPixelPtr++);
				ch1 = (*srcPixelPtr++);
				ch2 = (*srcPixelPtr++);
				sum0 += ch0;
				sum1 += ch1;
				sum2 += ch2;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
				j = 1;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < pixelNum - 1; j += 2) {
				a1 = ((mlib_s32 *)srcPixelPtr)[0];
				a2 = ((mlib_s32 *)srcPixelPtr)[1];
				a3 = ((mlib_s32 *)srcPixelPtr)[2];
				srcPixelPtr += 6;

#ifdef _LITTLE_ENDIAN
				ch0 = (a1 & 0x7fff) - (a1 & 0x8000);
				ch1 = a1 >> 16;
				ch2 = (a2 & 0x7fff) - (a2 & 0x8000);
#else /* _LITTLE_ENDIAN */
				ch0 = a1 >> 16;
				ch1 = (a1 & 0x7fff) - (a1 & 0x8000);
				ch2 = a2 >> 16;
#endif /* _LITTLE_ENDIAN */
				sum0 += ch0;
				sum1 += ch1;
				sum2 += ch2;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;

#ifdef _LITTLE_ENDIAN
				ch0 = a2 >> 16;
				ch1 = (a3 & 0x7fff) - (a3 & 0x8000);
				ch2 = a3 >> 16;
#else /* _LITTLE_ENDIAN */
				ch0 = (a2 & 0x7fff) - (a2 & 0x8000);
				ch1 = a3 >> 16;
				ch2 = (a3 & 0x7fff) - (a3 & 0x8000);
#endif /* _LITTLE_ENDIAN */
				sum0 += ch0;
				sum1 += ch1;
				sum2 += ch2;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
			}

			if (j < pixelNum) {
				ch0 = (*srcPixelPtr++);
				ch1 = (*srcPixelPtr++);
				ch2 = (*srcPixelPtr++);
				sum0 += ch0;
				sum1 += ch1;
				sum2 += ch2;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
			}

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_s16 *)srcRowPtr;
		}

		sum0 *= divider;
		stdev[0] = mlib_sqrt(mlib_fabs(mtwo0 * divider - sum0 * sum0));
		sum1 *= divider;
		stdev[1] = mlib_sqrt(mlib_fabs(mtwo1 * divider - sum1 * sum1));
		sum2 *= divider;
		stdev[2] = mlib_sqrt(mlib_fabs(mtwo2 * divider - sum2 * sum2));

	} else {
/* case 4: */

		dsum[0] = dsum[1] = moment[0] = moment[1] = 0;
		dsum[2] = dsum[3] = moment[2] = moment[3] = 0;

		if (2 * 4 * srcWidth == srcYStride) {
			srcWidth = srcTotalPixNum;
			srcHeight = 1;
		}

		loadsNum = srcWidth;

		for (i = 0; i < srcHeight; i++) {
			j = align = 0;

			if (2 & (mlib_addr)srcPixelPtr) {
#ifdef _LITTLE_ENDIAN
				ch1 = *(srcPixelPtr + 4 * srcWidth - 1);
				ch0 = (*srcPixelPtr++);
				ch3 = (*srcPixelPtr++);
				ch2 = (*srcPixelPtr++);
#else /* _LITTLE_ENDIAN */
				ch0 = *(srcPixelPtr + 4 * srcWidth - 1);
				ch1 = (*srcPixelPtr++);
				ch2 = (*srcPixelPtr++);
				ch3 = (*srcPixelPtr++);
#endif /* _LITTLE_ENDIAN */
				sum1 += ch1;
				sum2 += ch2;
				sum3 += ch3;
				sum0 += ch0;
				mtwo1 += ch1 * ch1;
				mtwo2 += ch2 * ch2;
				mtwo3 += ch3 * ch3;
				mtwo0 += ch0 * ch0;
				j = 1;
				align = 1;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= loadsNum - 2; j += 2) {
				a1 = ((mlib_s32 *)srcPixelPtr)[0];
				a2 = ((mlib_s32 *)srcPixelPtr)[1];
				a3 = ((mlib_s32 *)srcPixelPtr)[2];
				a4 = ((mlib_s32 *)srcPixelPtr)[3];
				srcPixelPtr += 8;

				ch0 = a1 >> 16;
				ch1 = (a1 & 0x7fff) - (a1 & 0x8000);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch2 = a2 >> 16;
				ch3 = (a2 & 0x7fff) - (a2 & 0x8000);
				sum2 += ch2;
				sum3 += ch3;
				mtwo2 += ch2 * ch2;
				mtwo3 += ch3 * ch3;

				ch0 = a3 >> 16;
				ch1 = (a3 & 0x7fff) - (a3 & 0x8000);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch2 = a4 >> 16;
				ch3 = (a4 & 0x7fff) - (a4 & 0x8000);
				sum2 += ch2;
				sum3 += ch3;
				mtwo2 += ch2 * ch2;
				mtwo3 += ch3 * ch3;
			}

			if (j < loadsNum) {
				a1 = ((mlib_s32 *)srcPixelPtr)[0];
				a2 = ((mlib_s32 *)srcPixelPtr)[1];
				srcPixelPtr += 4;

				ch0 = a1 >> 16;
				ch1 = (a1 & 0x7fff) - (a1 & 0x8000);
				sum0 += ch0;
				sum1 += ch1;
				mtwo0 += ch0 * ch0;
				mtwo1 += ch1 * ch1;

				ch2 = a2 >> 16;
				ch3 = (a2 & 0x7fff) - (a2 & 0x8000);
				sum2 += ch2;
				sum3 += ch3;
				mtwo2 += ch2 * ch2;
				mtwo3 += ch3 * ch3;
			}
#ifdef _LITTLE_ENDIAN
			dsum[3 & (0 - align)] += sum1;
			dsum[1 - align] += sum0;
			dsum[2 - align] += sum3;
			dsum[3 - align] += sum2;
			moment[3 & (0 - align)] += mtwo1;
			moment[1 - align] += mtwo0;
			moment[2 - align] += mtwo3;
			moment[3 - align] += mtwo2;
#else /* _LITTLE_ENDIAN */
			dsum[3 & (0 - align)] += sum0;
			dsum[1 - align] += sum1;
			dsum[2 - align] += sum2;
			dsum[3 - align] += sum3;
			moment[3 & (0 - align)] += mtwo0;
			moment[1 - align] += mtwo1;
			moment[2 - align] += mtwo2;
			moment[3 - align] += mtwo3;
#endif /* _LITTLE_ENDIAN */

			sum0 = sum1 = mtwo0 = mtwo1 =
			    sum2 = sum3 = mtwo2 = mtwo3 = 0.0;
			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_s16 *)srcRowPtr;
		}

		sum0 = dsum[0] * divider;
		stdev[0] =
		    mlib_sqrt(mlib_fabs(moment[0] * divider - sum0 * sum0));
		sum1 = dsum[1] * divider;
		stdev[1] =
		    mlib_sqrt(mlib_fabs(moment[1] * divider - sum1 * sum1));
		sum2 = dsum[2] * divider;
		stdev[2] =
		    mlib_sqrt(mlib_fabs(moment[2] * divider - sum2 * sum2));
		sum3 = dsum[3] * divider;
		stdev[3] =
		    mlib_sqrt(mlib_fabs(moment[3] * divider - sum3 * sum3));
	}
}

/* *********************************************************** */

void
mlib_c_ImageStdDev2ParamS32(
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

	mlib_s32 cycleNumber = 0;
	mlib_s32 val0, hi0, lo0;
	mlib_s32 val1, hi1, lo1;
	mlib_s32 val2, hi2, lo2;
	mlib_s32 val3, lo3;

	if (numOfChannels * srcWidth * 4 == srcYStride) {
		srcWidth = srcHeight * srcWidth;
		srcHeight = 1;
	}

	if (numOfChannels != 3) {

		pixelNumber = numOfChannels * srcWidth;
		loadsNumber = pixelNumber >> 2;
		pixelNumber &= 3;
		hi0 = lo0 = hi1 = lo1 = hi2 = lo2 = lo3 = 0;
		cycleNumber = loadsNumber >> 15;
		loadsNumber &= 0x7fff;

		for (j = 0; j < srcHeight; j++) {
			for (i = 0; i < cycleNumber; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
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

/* this raise performance !? */
					lo3 &= val3 & 0x1;
					sum3 += ch3;
				}

				sum0 += (mlib_d64)lo0 + (mlib_d64)hi0 *65536.;
				sum1 += (mlib_d64)lo1 + (mlib_d64)hi1 *65536.;
				sum2 += (mlib_d64)lo2 + (mlib_d64)hi2 *65536.;

				sum3 += (mlib_d64)lo3;
				hi0 = lo0 = hi1 = lo1 = hi2 = lo2 = lo3 = 0;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
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

				sum3 += ch3;
			}

			sum0 += (mlib_d64)lo0 + (mlib_d64)hi0 *65536.;
			sum1 += (mlib_d64)lo1 + (mlib_d64)hi1 *65536.;
			sum2 += (mlib_d64)lo2 + (mlib_d64)hi2 *65536.;

			hi0 = lo0 = hi1 = lo1 = hi2 = lo2 = lo3 = 0;

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
	} else {
/* case 3: */

		pixelNumber = srcWidth;
		hi0 = lo0 = hi1 = lo1 = hi2 = lo2 = 0;
		cycleNumber = pixelNumber >> 15;
		pixelNumber &= 0x7fff;
		for (j = 0; j < srcHeight; j++) {
			for (i = 0; i < cycleNumber; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
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
					sum2 += ch2;
				}

				sum0 += (mlib_d64)lo0 + (mlib_d64)hi0 *65536.;
				sum1 += (mlib_d64)lo1 + (mlib_d64)hi1 *65536.;

				hi0 = lo0 = hi1 = lo1 = /* hi2 = lo2 = */ 0;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
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
				sum2 += ch2;
			}

			sum0 += (mlib_d64)lo0 + (mlib_d64)hi0 *65536.;
			sum1 += (mlib_d64)lo1 + (mlib_d64)hi1 *65536.;

			hi0 = lo0 = hi1 = lo1 = /* hi2 = lo2 = */ 0;

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_s32 *)srcRowPtr;
		}

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
#endif /* end of ifndef _NO_LONGLONG */
/* *********************************************************** */
