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

#pragma ident	"@(#)mlib_s_ImageStdDev.c	9.3	07/11/05 SMI"

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
#include <mlib_s_ImageMoment2.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

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

static void mlib_s_ImageStdDev2ParamU8_1(
    const mlib_image *src,
    mlib_d64 *stdev);
static void mlib_s_ImageStdDev2ParamU8_2(
    const mlib_image *src,
    mlib_d64 *stdev);
static void mlib_s_ImageStdDev2ParamU8_3(
    const mlib_image *src,
    mlib_d64 *stdev);
static void mlib_s_ImageStdDev2ParamU8_4(
    const mlib_image *src,
    mlib_d64 *stdev);


static void mlib_s_ImageStdDev2ParamU16_1(
    const mlib_image *src,
    mlib_d64 *stdev);
static void mlib_s_ImageStdDev2ParamU16_2(
    const mlib_image *src,
    mlib_d64 *stdev);
static void mlib_s_ImageStdDev2ParamU16_3(
    const mlib_image *src,
    mlib_d64 *stdev);
static void mlib_s_ImageStdDev2ParamU16_4(
    const mlib_image *src,
    mlib_d64 *stdev);

static void mlib_s_ImageStdDev2ParamS16_1(
    const mlib_image *src,
    mlib_d64 *stdev);
static void mlib_s_ImageStdDev2ParamS16_2(
    const mlib_image *src,
    mlib_d64 *stdev);
static void mlib_s_ImageStdDev2ParamS16_3(
    const mlib_image *src,
    mlib_d64 *stdev);
static void mlib_s_ImageStdDev2ParamS16_4(
    const mlib_image *src,
    mlib_d64 *stdev);

static void mlib_s_ImageStdDev2ParamS32_1(
    const mlib_image *src,
    mlib_d64 *stdev);
static void mlib_s_ImageStdDev2ParamS32_2(
    const mlib_image *src,
    mlib_d64 *stdev);
static void mlib_s_ImageStdDev2ParamS32_3(
    const mlib_image *src,
    mlib_d64 *stdev);
static void mlib_s_ImageStdDev2ParamS32_4(
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
			switch (mlib_ImageGetChannels(src)) {
			case 1:
				mlib_s_ImageStdDev2ParamU8_1(src, stdev);
				break;
			case 2:
				mlib_s_ImageStdDev2ParamU8_2(src, stdev);
				break;
			case 3:
				mlib_s_ImageStdDev2ParamU8_3(src, stdev);
				break;
			case 4:
				mlib_s_ImageStdDev2ParamU8_4(src, stdev);
				break;
			default:
				return (MLIB_FAILURE);
			}

			return (MLIB_SUCCESS);


		case MLIB_SHORT:
			switch (mlib_ImageGetChannels(src)) {
			case 1:
				mlib_s_ImageStdDev2ParamS16_1(src, stdev);
				break;
			case 2:
				mlib_s_ImageStdDev2ParamS16_2(src, stdev);
				break;
			case 3:
				mlib_s_ImageStdDev2ParamS16_3(src, stdev);
				break;
			case 4:
				mlib_s_ImageStdDev2ParamS16_4(src, stdev);
				break;
			default:
				return (MLIB_FAILURE);
			}

			return (MLIB_SUCCESS);


		case MLIB_USHORT:
			switch (mlib_ImageGetChannels(src)) {
			case 1:
				mlib_s_ImageStdDev2ParamU16_1(src, stdev);
				break;
			case 2:
				mlib_s_ImageStdDev2ParamU16_2(src, stdev);
				break;
			case 3:
				mlib_s_ImageStdDev2ParamU16_3(src, stdev);
				break;
			case 4:
				mlib_s_ImageStdDev2ParamU16_4(src, stdev);
				break;
			default:
				return (MLIB_FAILURE);
			}

			return (MLIB_SUCCESS);


		case MLIB_INT:
			switch (mlib_ImageGetChannels(src)) {
			case 1:
				mlib_s_ImageStdDev2ParamS32_1(src, stdev);
				break;
			case 2:
				mlib_s_ImageStdDev2ParamS32_2(src, stdev);
				break;
			case 3:
				mlib_s_ImageStdDev2ParamS32_3(src, stdev);
				break;
			case 4:
				mlib_s_ImageStdDev2ParamS32_4(src, stdev);
				break;
			default:
				return (MLIB_FAILURE);
			}
			return (MLIB_SUCCESS);

		default:
			return (MLIB_FAILURE);
		}
	}

	switch (mlib_ImageGetType(src)) {
	case MLIB_BYTE:
		switch (mlib_ImageGetChannels(src)) {
		case 1:
			mlib_s_ImageMoment2_U8_1(src, rez);
			break;
		case 2:
			mlib_s_ImageMoment2_U8_2(src, rez);
			break;
		case 3:
			mlib_s_ImageMoment2_U8_3(src, rez);
			break;
		case 4:
			mlib_s_ImageMoment2_U8_4(src, rez);
			break;
		default:
			return (MLIB_FAILURE);
		}

		break;

	case MLIB_USHORT:

		switch (mlib_ImageGetChannels(src)) {
		case 1:
			mlib_s_ImageMoment2_U16_1(src, rez);
			break;
		case 2:
			mlib_s_ImageMoment2_U16_2(src, rez);
			break;
		case 3:
			mlib_s_ImageMoment2_U16_3(src, rez);
			break;
		case 4:
			mlib_s_ImageMoment2_U16_4(src, rez);
			break;
		default:
			return (MLIB_FAILURE);
		}
		break;

	case MLIB_SHORT:

		switch (mlib_ImageGetChannels(src)) {
		case 1:
			mlib_s_ImageMoment2_S16_1(src, rez);
			break;
		case 2:
			mlib_s_ImageMoment2_S16_2(src, rez);
			break;
		case 3:
			mlib_s_ImageMoment2_S16_3(src, rez);
			break;
		case 4:
			mlib_s_ImageMoment2_S16_4(src, rez);
			break;
		default:
			return (MLIB_FAILURE);
		}
		break;

	case MLIB_INT:

		switch (mlib_ImageGetChannels(src)) {
		case 1:
			mlib_s_ImageMoment2_S32_1(src, rez);
			break;
		case 2:
			mlib_s_ImageMoment2_S32_2(src, rez);
			break;
		case 3:
			mlib_s_ImageMoment2_S32_3(src, rez);
			break;
		case 4:
			mlib_s_ImageMoment2_S32_4(src, rez);
			break;
		default:
			return (MLIB_FAILURE);
		}
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
#define	MAX_BLOCK_FOR_U8    8192
/* *********************************************************** */

void
mlib_s_ImageStdDev2ParamU8_1(
    const mlib_image *src,
    mlib_d64 *stdev)
{
/* pointer to the data of image */
	mlib_u8 *psrc = (mlib_u8 *)mlib_ImageGetData(src);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(src);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(src);
	mlib_s32 src_chan = mlib_ImageGetChannels(src);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(src);

/* indices */
	mlib_s32 i, j, l;

/* total number of pixels */
	mlib_s32 srcTotalPixNum = height * size_row;


	if (src_stride == size_row) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}


	mlib_d64 buffer0 = 0.0, abuffer0 = 0.0;
	mlib_d64 divider = 1.0 / srcTotalPixNum;
	mlib_s32 db_num;
	__m128i* sl;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {

		for (j = 0; (j < size_row) &&
		    ((mlib_addr)(psrc +j) & 15); j++) {
			buffer0 += psrc[j] * psrc[j];
			abuffer0 += psrc[j];
		}

		db_num = ((size_row - j) >> 4);
		sl = (__m128i *)(psrc + j);
		__m128i load, sd, sdl, sdh, sdm1, sdm2, sd1, sd2, sd3, sd4;
		__m128i sda1, sda2, sda3, sda4;

		for (; db_num > 0; db_num -= MAX_BLOCK_FOR_U8) {
			__m128i sum0, asum0;
			mlib_s32 size = MAX_BLOCK_FOR_U8;

			sum0 = _mm_setzero_si128();
			asum0 = _mm_setzero_si128();

			if (db_num < MAX_BLOCK_FOR_U8)
				size = db_num;

			j += (size << 4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (l = 0; l < size; l++) {
				load = _mm_load_si128(sl);
				sdl = _mm_unpacklo_epi8(load, x_zero);
				sdh = _mm_unpackhi_epi8(load, x_zero);

				sdm1 = _mm_madd_epi16(sdl, sdl);
				sdm2 = _mm_madd_epi16(sdh, sdh);

				sd = _mm_add_epi16(sdl, sdh);
				sda1 = _mm_unpacklo_epi16(sd, x_zero);
				sda2 = _mm_unpackhi_epi16(sd, x_zero);

				sum0 = _mm_add_epi32(sdm1, sum0);
				sum0 = _mm_add_epi32(sdm2, sum0);

				asum0 = _mm_add_epi32(sda1, asum0);
				asum0 = _mm_add_epi32(sda2, asum0);

				sl++;
			}

			buffer0 += (mlib_d64)((mlib_s32 *)&sum0)[0] +
				((mlib_s32 *)&sum0)[1] +
				((mlib_s32 *)&sum0)[2] +
				((mlib_s32 *)&sum0)[3];
			abuffer0 += (mlib_d64)((mlib_s32 *)&asum0)[0] +
				((mlib_s32 *)&asum0)[1] +
				((mlib_s32 *)&asum0)[2] +
				((mlib_s32 *)&asum0)[3];
		}

		for (; j < size_row; j++) {
			buffer0 += psrc[j] * psrc[j];
			abuffer0 += psrc[j];
		}

		psrc += src_stride;
	}

	mlib_d64 moment = 0.0, mean = 0.0;

	divider = 1.0 / srcTotalPixNum;
	moment = buffer0 * divider;
	mean = abuffer0 * divider;

	stdev[0] = mlib_sqrt(mlib_fabs(moment - mean * mean));
}
/* *********************************************************** */

void
mlib_s_ImageStdDev2ParamU8_2(
    const mlib_image *src,
    mlib_d64 *stdev)
{
/* pointer to the data of image */
	mlib_u8 *psrc = (mlib_u8 *)mlib_ImageGetData(src);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(src);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(src);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(src) << 1;

/* total number of pixels */
	mlib_s32 srcTotalPixNum = height * (size_row >> 1);

/* indices */
	mlib_s32 i, j, l;

	if (src_stride == size_row) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0, buffer1 = 0.0;
	mlib_d64 abuffer0 = 0.0, abuffer1 = 0.0;
	mlib_d64 divider = 1.0 / srcTotalPixNum;
	__m128i* sl;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 alignment = 1;
		mlib_s32 db_num;

		if ((mlib_addr)psrc & 1) {
			alignment = 0;
			buffer0 += psrc[0] * psrc[0];
			abuffer0 += psrc[0];

			for (j = 1; (j < (size_row - 2)) &&
			    ((mlib_addr)(psrc + j) & 15); j += 2) {
				buffer1 += psrc[j + 0] * psrc[j + 0];
				buffer0 += psrc[j + 1] * psrc[j + 1];
				abuffer1 += psrc[j + 0];
				abuffer0 += psrc[j + 1];
			}
		} else {
			for (j = 0; (j < size_row) &&
			    ((mlib_addr)(psrc + j) & 15); j += 2) {
				buffer0 += psrc[j + 0] * psrc[j + 0];
				buffer1 += psrc[j + 1] * psrc[j + 1];
				abuffer0 += psrc[j + 0];
				abuffer1 += psrc[j + 1];
			}
		}

		db_num = ((size_row - j) >> 4);
		sl = (__m128i *)(psrc + j);
		__m128i load, load0, load1, sdl0, sdh0, sdl1, sdh1;
		__m128i sdm0, sdm1, sdm2, sdm3;
		__m128i sd, sda1, sda2, sda3, sda4;

		for (; db_num > 0; db_num -= MAX_BLOCK_FOR_U8) {
			__m128i sum0, sum1, asum0;
			mlib_s32 size = MAX_BLOCK_FOR_U8;

			sum0 = _mm_setzero_si128();
			asum0 = _mm_setzero_si128();

			if (db_num < MAX_BLOCK_FOR_U8)
				size = db_num;

			j += (size << 4);


#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (l = 0; (l < size); l++) {
				load = _mm_load_si128(sl);
				load0 = _mm_unpacklo_epi8(load, x_zero);
				load1 = _mm_unpackhi_epi8(load, x_zero);

				sdl0 = _mm_mullo_epi16(load0, load0);
				sdh0 = _mm_mulhi_epi16(load0, load0);
				sdl1 = _mm_mullo_epi16(load1, load1);
				sdh1 = _mm_mulhi_epi16(load1, load1);

				sdm0 = _mm_unpacklo_epi16(sdl0, sdh0);
				sdm1 = _mm_unpackhi_epi16(sdl0, sdh0);
				sdm2 = _mm_unpacklo_epi16(sdl1, sdh1);
				sdm3 = _mm_unpackhi_epi16(sdl1, sdh1);

				sum0 = _mm_add_epi32(sum0, sdm0);
				sum0 = _mm_add_epi32(sum0, sdm2);
				sum1 = _mm_add_epi32(sdm1, sdm3);
				sum0 = _mm_add_epi32(sum0, sum1);

				sd = _mm_add_epi16(load0, load1);
				sda1 = _mm_unpacklo_epi16(sd, x_zero);
				sda2 = _mm_unpackhi_epi16(sd, x_zero);
				asum0 = _mm_add_epi32(sda1, asum0);
				asum0 = _mm_add_epi32(sda2, asum0);

				sl++;
			}

			if (alignment) {
				buffer0 += (mlib_d64)((mlib_s32 *)&sum0)[0] +
					((mlib_s32 *)&sum0)[2];
				buffer1 += (mlib_d64)((mlib_s32 *)&sum0)[1] +
					((mlib_s32 *)&sum0)[3];
				abuffer0 += (mlib_d64)((mlib_s32 *)&asum0)[0] +
					((mlib_s32 *)&asum0)[2];
				abuffer1 += (mlib_d64)((mlib_s32 *)&asum0)[1] +
					((mlib_s32 *)&asum0)[3];

			} else {
				buffer1 += (mlib_d64)((mlib_s32 *)&sum0)[0] +
					((mlib_s32 *)&sum0)[2];
				buffer0 += (mlib_d64)((mlib_s32 *)&sum0)[1] +
					((mlib_s32 *)&sum0)[3];
				abuffer1 += (mlib_d64)((mlib_s32 *)&asum0)[0] +
					((mlib_s32 *)&asum0)[2];
				abuffer0 += (mlib_d64)((mlib_s32 *)&asum0)[1] +
					((mlib_s32 *)&asum0)[3];
			}
		}

		if (alignment) {
			for (; j < size_row; j += 2) {
				buffer0 += psrc[j + 0] * psrc[j + 0];
				buffer1 += psrc[j + 1] * psrc[j + 1];
				abuffer0 += psrc[j + 0];
				abuffer1 += psrc[j + 1];
			}
		} else {
			for (; j < (size_row - 1); j += 2) {
				buffer1 += psrc[j + 0] * psrc[j + 0];
				buffer0 += psrc[j + 1] * psrc[j + 1];
				abuffer1 += psrc[j + 0];
				abuffer0 += psrc[j + 1];
			}
			buffer1 += psrc[j] * psrc[j];
			abuffer1 += psrc[j];
		}

		psrc += src_stride;
	}

	mlib_d64 moment0 = 0.0, mean0 = 0.0;
	mlib_d64 moment1 = 0.0, mean1 = 0.0;

	moment0 = buffer0 * divider;
	moment1 = buffer1 * divider;
	mean0 = abuffer0 * divider;
	mean1 = abuffer1 * divider;

	stdev[0] = mlib_sqrt(mlib_fabs(moment0 - mean0 * mean0));
	stdev[1] = mlib_sqrt(mlib_fabs(moment1 - mean1 * mean1));

}
/* *********************************************************** */

void
mlib_s_ImageStdDev2ParamU8_4(
    const mlib_image *src,
    mlib_d64 *stdev)
{
/* pointer to the data of image */
	mlib_u8 *psrc = (mlib_u8 *)mlib_ImageGetData(src);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(src);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(src);
	mlib_s32 src_chan = mlib_ImageGetChannels(src);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(src) << 2;

/* total number of pixels */
	mlib_s32 srcTotalPixNum = height * (size_row >> 2);

/* indices */
	mlib_s32 i, j, l;

	if (src_stride == size_row) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_d64 buffer2 = 0.0;
	mlib_d64 buffer3 = 0.0;
	mlib_d64 abuffer0 = 0.0;
	mlib_d64 abuffer1 = 0.0;
	mlib_d64 abuffer2 = 0.0;
	mlib_d64 abuffer3 = 0.0;
	mlib_d64 divider = 1.0 / srcTotalPixNum;
	mlib_s32 db_num;
	__m128i* sl;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 sign = 0;
		mlib_s32 alignment =
		    ((16 - ((mlib_addr)psrc & 15)) & 15);

		for (; (j < size_row - 3) && (alignment >= 4); j += 4) {
			buffer0 += psrc[j + 0] * psrc[j + 0];
			buffer1 += psrc[j + 1] * psrc[j + 1];
			buffer2 += psrc[j + 2] * psrc[j + 2];
			buffer3 += psrc[j + 3] * psrc[j + 3];
			abuffer0 += psrc[j + 0];
			abuffer1 += psrc[j + 1];
			abuffer2 += psrc[j + 2];
			abuffer3 += psrc[j + 3];

			alignment -= 4;
		}

		if (j < (size_row - 3)) {
			if ((alignment == 3)) {
				buffer0 += psrc[j + 0] * psrc[j + 0];
				buffer1 += psrc[j + 1] * psrc[j + 1];
				buffer2 += psrc[j + 2] * psrc[j + 2];
				abuffer0 += psrc[j + 0];
				abuffer1 += psrc[j + 1];
				abuffer2 += psrc[j + 2];
				j += 3;
				sign = 1;
			} else if ((alignment == 2)) {
				buffer0 += psrc[j + 0] * psrc[j + 0];
				buffer1 += psrc[j + 1] * psrc[j + 1];
				abuffer0 += psrc[j + 0];
				abuffer1 += psrc[j + 1];
				j += 2;
				sign = 2;
			} else if ((alignment == 1)) {
				buffer0 += psrc[j + 0] * psrc[j + 0];
				abuffer0 += psrc[j + 0];
				j += 1;
				sign = 3;
			}
		}

		db_num = ((size_row - j) >> 4);
		sl = (__m128i *)(psrc + j);
		__m128i load, load0, load1, sdl0, sdh0, sdl1, sdh1;
		__m128i sdm0, sdm1, sdm2, sdm3;
		__m128i sd, sda1, sda2, sda3, sda4;

		for (; db_num >= 0; db_num -= MAX_BLOCK_FOR_U8) {
			__m128i sum0, sum1, asum0;
			mlib_s32 size = MAX_BLOCK_FOR_U8;

			sum0 = _mm_setzero_si128();
			asum0 = _mm_setzero_si128();

			if (db_num < MAX_BLOCK_FOR_U8)
				size = db_num;

			j += (size << 4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (l = 0; (l < size); l++) {
				load = _mm_load_si128(sl);
				load0 = _mm_unpacklo_epi8(load, x_zero);
				load1 = _mm_unpackhi_epi8(load, x_zero);

				sdl0 = _mm_mullo_epi16(load0, load0);
				sdh0 = _mm_mulhi_epi16(load0, load0);
				sdl1 = _mm_mullo_epi16(load1, load1);
				sdh1 = _mm_mulhi_epi16(load1, load1);

				sdm0 = _mm_unpacklo_epi16(sdl0, sdh0);
				sdm1 = _mm_unpackhi_epi16(sdl0, sdh0);
				sdm2 = _mm_unpacklo_epi16(sdl1, sdh1);
				sdm3 = _mm_unpackhi_epi16(sdl1, sdh1);
				sum0 = _mm_add_epi32(sum0, sdm0);
				sum0 = _mm_add_epi32(sum0, sdm2);
				sum1 = _mm_add_epi32(sdm1, sdm3);
				sum0 = _mm_add_epi32(sum0, sum1);

				sd = _mm_add_epi16(load0, load1);
				sda1 = _mm_unpacklo_epi16(sd, x_zero);
				sda2 = _mm_unpackhi_epi16(sd, x_zero);
				asum0 = _mm_add_epi32(sda1, asum0);
				asum0 = _mm_add_epi32(sda2, asum0);

				sl++;
			}

			if (sign == 3) {
				buffer0 += (mlib_d64)((mlib_s32 *)&sum0)[3];
				buffer1 += psrc[j + 0] * psrc[j + 0] +
					(mlib_d64)((mlib_s32 *)&sum0)[0];
				buffer2 += psrc[j + 1] * psrc[j + 1] +
					(mlib_d64)((mlib_s32 *)&sum0)[1];
				buffer3 += psrc[j + 2] * psrc[j + 2] +
					(mlib_d64)((mlib_s32 *)&sum0)[2];

				abuffer0 += (mlib_d64)((mlib_s32 *)&asum0)[3];
				abuffer1 += psrc[j + 0] +
					(mlib_d64)((mlib_s32 *)&asum0)[0];
				abuffer2 += psrc[j + 1] +
					(mlib_d64)((mlib_s32 *)&asum0)[1];
				abuffer3 += psrc[j + 2] +
					(mlib_d64)((mlib_s32 *)&asum0)[2];

				j += 3;

			} else if (sign == 2) {
				buffer0 += (mlib_d64)((mlib_s32 *)&sum0)[2];
				buffer1 += (mlib_d64)((mlib_s32 *)&sum0)[3];
				buffer2 += psrc[j + 0] * psrc[j + 0] +
					(mlib_d64)((mlib_s32 *)&sum0)[0];
				buffer3 += psrc[j + 1] * psrc[j + 1] +
					(mlib_d64)((mlib_s32 *)&sum0)[1];

				abuffer0 += (mlib_d64)((mlib_s32 *)&asum0)[2];
				abuffer1 += (mlib_d64)((mlib_s32 *)&asum0)[3];
				abuffer2 += psrc[j + 0] +
					(mlib_d64)((mlib_s32 *)&asum0)[0];
				abuffer3 += psrc[j + 1] +
					(mlib_d64)((mlib_s32 *)&asum0)[1];

				j += 2;

			} else if (sign == 1) {
				buffer0 += (mlib_d64)((mlib_s32 *)&sum0)[1];
				buffer1 += (mlib_d64)((mlib_s32 *)&sum0)[2];
				buffer2 += (mlib_d64)((mlib_s32 *)&sum0)[3];
				buffer3 += psrc[j + 0] * psrc[j + 0] +
					(mlib_d64)((mlib_s32 *)&sum0)[0];

				abuffer0 += (mlib_d64)((mlib_s32 *)&asum0)[1];
				abuffer1 += (mlib_d64)((mlib_s32 *)&asum0)[2];
				abuffer2 += (mlib_d64)((mlib_s32 *)&asum0)[3];
				abuffer3 += psrc[j + 0] +
					(mlib_d64)((mlib_s32 *)&asum0)[0];

				j++;
			} else if (sign == 0) {
				buffer0 += (mlib_d64)((mlib_s32 *)&sum0)[0];
				buffer1 += (mlib_d64)((mlib_s32 *)&sum0)[1];
				buffer2 += (mlib_d64)((mlib_s32 *)&sum0)[2];
				buffer3 += (mlib_d64)((mlib_s32 *)&sum0)[3];

				abuffer0 += (mlib_d64)((mlib_s32 *)&asum0)[0];
				abuffer1 += (mlib_d64)((mlib_s32 *)&asum0)[1];
				abuffer2 += (mlib_d64)((mlib_s32 *)&asum0)[2];
				abuffer3 += (mlib_d64)((mlib_s32 *)&asum0)[3];
			}
		}

		for (; j < (size_row - 3); j += 4) {
			buffer0 += psrc[j + 0] * psrc[j + 0];
			buffer1 += psrc[j + 1] * psrc[j + 1];
			buffer2 += psrc[j + 2] * psrc[j + 2];
			buffer3 += psrc[j + 3] * psrc[j + 3];

			abuffer0 += psrc[j + 0];
			abuffer1 += psrc[j + 1];
			abuffer2 += psrc[j + 2];
			abuffer3 += psrc[j + 3];
		}

		psrc += src_stride;
	}

	mlib_d64 moment0 = 0.0, mean0 = 0.0;
	mlib_d64 moment1 = 0.0, mean1 = 0.0;
	mlib_d64 moment2 = 0.0, mean2 = 0.0;
	mlib_d64 moment3 = 0.0, mean3 = 0.0;

	moment0 = buffer0 * divider;
	moment1 = buffer1 * divider;
	moment2 = buffer2 * divider;
	moment3 = buffer3 * divider;
	mean0 = abuffer0 * divider;
	mean1 = abuffer1 * divider;
	mean2 = abuffer2 * divider;
	mean3 = abuffer3 * divider;

	stdev[0] = mlib_sqrt(mlib_fabs(moment0 - mean0 * mean0));
	stdev[1] = mlib_sqrt(mlib_fabs(moment1 - mean1 * mean1));
	stdev[2] = mlib_sqrt(mlib_fabs(moment2 - mean2 * mean2));
	stdev[3] = mlib_sqrt(mlib_fabs(moment3 - mean3 * mean3));

}
/* *********************************************************** */

#define	MUL_U8(x)	(mlib_SQ_U8[(x)])
#define	ADD_U8(x)   (mlib_U82D64[(x)])

/* *********************************************************** */

void
mlib_s_ImageStdDev2ParamU8_3(
    const mlib_image *src,
    mlib_d64 *stdev)
{
/* pointer to the data of image */
	mlib_u8 *psrc = (mlib_u8 *)mlib_ImageGetData(src);

/* accumulators by channels */
	mlib_d64 c1, c2, c3;

/* accumulators by channels */
	mlib_d64 d1, d2, d3;

	mlib_u8 a0, a1, a2, a3, a4, a5;
/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(src);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(src);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(src) * 3;

/* total number of pixels */
	mlib_s32 srcTotalPixNum = height * (size_row / 3);

/* indices */
	mlib_s32 i, j;

	mlib_d64 divider = 1.0 / srcTotalPixNum;

	if (src_stride == size_row) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}

	c1 = c2 = c3 = 0;
	d1 = d2 = d3 = 0;
	for (i = 0; i < height; i++) {
		j = 0;

		if ((mlib_addr)psrc & 1) {
			a0 = psrc[j];
			a1 = psrc[j + 1];
			a2 = psrc[j + 2];

			c1 += MUL_U8(a0);
			c2 += MUL_U8(a1);
			c3 += MUL_U8(a2);

			d1 += ADD_U8(a0);
			d2 += ADD_U8(a1);
			d3 += ADD_U8(a2);

			j += 3;
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (size_row - 6); j += 6) {
			mlib_u32 u1, u2;

			u1 = ((mlib_u16 *)(psrc + j))[0];
			u2 = ((mlib_u16 *)(psrc + j))[1];
			a0 = u1 & 0xff;
			a1 = u1 >> 8;
			a2 = u2 & 0xff;
			a3 = u2 >> 8;
			a4 = psrc[j + 4];
			a5 = psrc[j + 5];


#ifdef _LITTLE_ENDIAN
			c1 += MUL_U8(a0);
			c2 += MUL_U8(a1);
			c3 += MUL_U8(a2);
			c1 += MUL_U8(a3);

			d1 += ADD_U8(a0);
			d2 += ADD_U8(a1);
			d3 += ADD_U8(a2);
			d1 += ADD_U8(a3);

#else /* _LITTLE_ENDIAN */
			c1 += MUL_U8(a1);
			c2 += MUL_U8(a0);
			c3 += MUL_U8(a3);
			c1 += MUL_U8(a2);

			d1 += ADD_U8(a1);
			d2 += ADD_U8(a0);
			d3 += ADD_U8(a3);
			d1 += ADD_U8(a2);

#endif /* _LITTLE_ENDIAN */
			c2 += MUL_U8(a4);
			c3 += MUL_U8(a5);

			d2 += ADD_U8(a4);
			d3 += ADD_U8(a5);

		}

		if (j < size_row) {
			a0 = psrc[j];
			a1 = psrc[j + 1];
			a2 = psrc[j + 2];

			c1 += MUL_U8(a0);
			c2 += MUL_U8(a1);
			c3 += MUL_U8(a2);

			d1 += ADD_U8(a0);
			d2 += ADD_U8(a1);
			d3 += ADD_U8(a2);

		}

		psrc += src_stride;
	}

	mlib_d64 moment0 = 0.0, mean0 = 0.0;
	mlib_d64 moment1 = 0.0, mean1 = 0.0;
	mlib_d64 moment2 = 0.0, mean2 = 0.0;

	moment0 = c1 * divider;
	moment1 = c2 * divider;
	moment2 = c3 * divider;
	mean0 = d1 * divider;
	mean1 = d2 * divider;
	mean2 = d3 * divider;

	stdev[0] = mlib_sqrt(mlib_fabs(moment0 - mean0 * mean0));
	stdev[1] = mlib_sqrt(mlib_fabs(moment1 - mean1 * mean1));
	stdev[2] = mlib_sqrt(mlib_fabs(moment2 - mean2 * mean2));

}
/* *********************************************************** */

#define	MAX_BLOCK_FOR_U16    16384
#define	MLIB_CONVERT_U16_S16(x) ((mlib_d64)x - 32768.0)

/* *********************************************************** */

void
mlib_s_ImageStdDev2ParamU16_1(
    const mlib_image *src,
    mlib_d64 *stdev)
{
/* pointer to the data of image */
	mlib_u16 *psrc = (mlib_u16 *)mlib_ImageGetData(src);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(src);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(src) >> 1;
	mlib_s32 src_chan = mlib_ImageGetChannels(src);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(src);

/* total number of pixels */
	mlib_s32 srcTotalPixNum = height * size_row;

/* indices */
	mlib_s32 i, j, l;

	mlib_d64 divider = 1.0 / srcTotalPixNum;

	if (src_stride == size_row) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 abuffer0 = 0.0;
	mlib_s32 db_num;
	__m128i* sl;
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i mask8000 = _mm_set1_epi32(0x80008000);
	__m128i x_sign;

	for (i = 0; i < height; i++) {
		j = 0;

		for (j = 0; (j < size_row) &&
		    ((mlib_addr)(psrc +j) & 15); j++) {
			buffer0 += MLIB_CONVERT_U16_S16(psrc[j]) *
				MLIB_CONVERT_U16_S16(psrc[j]);
			abuffer0 += MLIB_CONVERT_U16_S16(psrc[j]);
		}

		db_num = ((size_row - j) >> 3);
		sl = (__m128i *)(psrc + j);

		for (; db_num > 0; db_num -= MAX_BLOCK_FOR_U16) {
			__m128i load, sdl, sdh, sd1, sd2, sd3, sd4, sd5, sd6;
			__m128i sum1, sum2, sum3, sum4;
			__m128i asum0, load0, load1;
			__m128i sd, sda1, sda2, sda3, sda4;

			sum1 = _mm_setzero_si128();
			sum2 = _mm_setzero_si128();
			sum3 = _mm_setzero_si128();
			sum4 = _mm_setzero_si128();
			asum0 = _mm_setzero_si128();

			mlib_s32 size = MAX_BLOCK_FOR_U16;

			if (db_num < MAX_BLOCK_FOR_U16)
				size = db_num;

			j += (size << 3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (l = 0; (l < size); l++) {
				load = _mm_xor_si128(
					_mm_load_si128(sl), mask8000);
				sdl = _mm_madd_epi16(load, load);
				sd1 = _mm_unpacklo_epi32(sdl, x_zero);
				sd2 = _mm_unpackhi_epi32(sdl, x_zero);
				sum1 = _mm_add_epi64(sum1, sd1);
				sum2 = _mm_add_epi64(sum2, sd2);

				x_sign = _mm_srai_epi16(load, 15);
				sda1 = _mm_unpacklo_epi16(load, x_sign);
				sda2 = _mm_unpackhi_epi16(load, x_sign);
				asum0 = _mm_add_epi32(sda1, asum0);
				asum0 = _mm_add_epi32(sda2, asum0);

				sl++;
			}


			sum1 = _mm_add_epi64(sum1, sum2);

			buffer0 += (mlib_d64)((mlib_s64 *)&sum1)[0] +
				((mlib_s64 *)&sum1)[1];
			abuffer0 += (mlib_d64)((mlib_s32 *)&asum0)[0] +
				((mlib_s32 *)&asum0)[1] +
				((mlib_s32 *)&asum0)[2] +
				((mlib_s32 *)&asum0)[3];
		}

		for (; j < size_row; j++) {
			buffer0 += MLIB_CONVERT_U16_S16(psrc[j]) *
				MLIB_CONVERT_U16_S16(psrc[j]);
			abuffer0 += MLIB_CONVERT_U16_S16(psrc[j]);
		}

		psrc += src_stride;
	}

	mlib_d64 moment0 = 0.0, mean0 = 0.0;

	buffer0 = buffer0 + 65536.0 * abuffer0 +
		32768.0 * 32768.0 * srcTotalPixNum;
	abuffer0 = abuffer0 + 32768.0 * srcTotalPixNum;
	moment0 = buffer0 * divider;
	mean0 = abuffer0 * divider;

	stdev[0] = mlib_sqrt(mlib_fabs(moment0 - mean0 * mean0));

}

/* *********************************************************** */

void
mlib_s_ImageStdDev2ParamU16_2(
    const mlib_image *src,
    mlib_d64 *stdev)
{
/* pointer to the data of image */
	mlib_u16 *psrc = (mlib_u16 *)mlib_ImageGetData(src);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(src);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(src) >> 1;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(src) << 1;

/* total number of pixels */
	mlib_s32 srcTotalPixNum = height * (size_row >> 1);

/* indices */
	mlib_s32 i, j, l;

	mlib_d64 divider = 1.0 / srcTotalPixNum;

	if (src_stride == size_row) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_d64 abuffer0 = 0.0;
	mlib_d64 abuffer1 = 0.0;

	__m128i* sl;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 alignment = 1;
		mlib_s32 db_num;

		if ((mlib_addr)psrc & 1) {
			alignment = 0;
			buffer0 += (mlib_d64)psrc[0] * psrc[0];
			abuffer0 += (mlib_d64)psrc[0];

			for (j = 1; (j < (size_row - 2)) &&
			    ((mlib_addr)(psrc + j) & 15); j += 2) {
				buffer1 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer0 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
				abuffer1 += (mlib_d64)psrc[j + 0];
				abuffer0 += (mlib_d64)psrc[j + 1];
			}
		} else {
			for (j = 0; (j < size_row) &&
			    ((mlib_addr)(psrc + j) & 15); j += 2) {
				buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
				abuffer0 += (mlib_d64)psrc[j + 0];
				abuffer1 += (mlib_d64)psrc[j + 1];
			}
		}

		db_num = ((size_row - j) >> 3);
		sl = (__m128i *)(psrc + j);

		for (; db_num > 0; db_num -= MAX_BLOCK_FOR_U16) {

			__m128i load, sdl, sdh, sd1, sd2, sd3, sd4, sd5, sd6;
			__m128i sum1, sum2, sum3, sum4;
			__m128i asum0, load0, load1;
			__m128i sd, sda1, sda2, sda3, sda4;

			sum1 = _mm_setzero_si128();
			sum2 = _mm_setzero_si128();
			asum0 = _mm_setzero_si128();

			mlib_s32 size = MAX_BLOCK_FOR_U16;

			if (db_num < MAX_BLOCK_FOR_U16)
				size = db_num;

			j += (size << 3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (l = 0; (l < size); l++) {
				load = _mm_load_si128(sl);
				sdl = _mm_mullo_epi16(load, load);
				sdh = _mm_mulhi_epu16(load, load);
				sd1 = _mm_unpacklo_epi16(sdl, sdh);
				sd2 = _mm_unpackhi_epi16(sdl, sdh);
				sd3 = _mm_unpacklo_epi32(sd1, x_zero);
				sd4 = _mm_unpackhi_epi32(sd1, x_zero);
				sd5 = _mm_unpacklo_epi32(sd2, x_zero);
				sd6 = _mm_unpackhi_epi32(sd2, x_zero);

				sum1 = _mm_add_epi64(sum1, sd3);
				sum2 = _mm_add_epi64(sum2, sd4);
				sum1 = _mm_add_epi64(sum1, sd5);
				sum2 = _mm_add_epi64(sum2, sd6);

				sda1 = _mm_unpacklo_epi16(load, x_zero);
				sda2 = _mm_unpackhi_epi16(load, x_zero);
				asum0 = _mm_add_epi32(sda1, asum0);
				asum0 = _mm_add_epi32(sda2, asum0);

				sl++;
			}

			sum1 = _mm_add_epi64(sum1, sum2);

			if (alignment) {
				buffer0 += ((mlib_s64 *)&sum1)[0];
				buffer1 += ((mlib_s64 *)&sum1)[1];
				abuffer0 += ((mlib_s32 *)&asum0)[0] +
					((mlib_s32 *)&asum0)[2];
				abuffer1 += ((mlib_s32 *)&asum0)[1] +
					((mlib_s32 *)&asum0)[3];

			} else {
				buffer1 += ((mlib_s64 *)&sum1)[0];
				buffer0 += ((mlib_s64 *)&sum1)[1];
				abuffer1 += ((mlib_s32 *)&asum0)[0] +
					((mlib_s32 *)&asum0)[2];
				abuffer0 += ((mlib_s32 *)&asum0)[1] +
					((mlib_s32 *)&asum0)[3];
			}

		}

		if (alignment) {
			for (; j < size_row; j += 2) {
				buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
				abuffer0 += (mlib_d64)psrc[j + 0];
				abuffer1 += (mlib_d64)psrc[j + 1];
			}
		} else {
			for (; j < (size_row - 1); j += 2) {
				buffer1 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer0 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
				abuffer1 += (mlib_d64)psrc[j + 0];
				abuffer0 += (mlib_d64)psrc[j + 1];
			}
			buffer1 += (mlib_d64)psrc[j] * psrc[j];
			abuffer1 += (mlib_d64)psrc[j];
		}

		psrc += src_stride;
	}

	mlib_d64 moment0 = 0.0, mean0 = 0.0;
	mlib_d64 moment1 = 0.0, mean1 = 0.0;

	moment0 = buffer0 * divider;
	moment1 = buffer1 * divider;
	mean0 = abuffer0 * divider;
	mean1 = abuffer1 * divider;

	stdev[0] = mlib_sqrt(mlib_fabs(moment0 - mean0 * mean0));
	stdev[1] = mlib_sqrt(mlib_fabs(moment1 - mean1 * mean1));

}
/* *********************************************************** */

void
mlib_s_ImageStdDev2ParamU16_4(
    const mlib_image *src,
    mlib_d64 *stdev)
{
/* pointer to the data of image */
	mlib_u16 *psrc = (mlib_u16 *)mlib_ImageGetData(src);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(src);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(src) >> 1;
	mlib_s32 src_chan = mlib_ImageGetChannels(src);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(src) << (src_chan >> 1);

/* total number of pixels */
	mlib_s32 srcTotalPixNum = height * (size_row >> 2);

/* indices */
	mlib_s32 i, j, l;

	mlib_d64 divider = 1.0 / srcTotalPixNum;

	if (src_stride == size_row) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_d64 buffer2 = 0.0;
	mlib_d64 buffer3 = 0.0;
	mlib_d64 abuffer0 = 0.0;
	mlib_d64 abuffer1 = 0.0;
	mlib_d64 abuffer2 = 0.0;
	mlib_d64 abuffer3 = 0.0;

	mlib_s32 db_num;
	__m128i* sl;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 sign = 0;
		mlib_s32 alignment =
		    ((16 - ((mlib_addr)psrc & 15)) & 15) >> 1;

		for (; (j < size_row - 3) && (alignment >= 4); j += 4) {
			buffer0 += (mlib_d64)psrc[0] * psrc[0];
			buffer1 += (mlib_d64)psrc[1] * psrc[1];
			buffer2 += (mlib_d64)psrc[2] * psrc[2];
			buffer3 += (mlib_d64)psrc[3] * psrc[3];

			abuffer0 += (mlib_d64)psrc[0];
			abuffer1 += (mlib_d64)psrc[1];
			abuffer2 += (mlib_d64)psrc[2];
			abuffer3 += (mlib_d64)psrc[3];

			alignment -= 4;
		}

		if (j < (size_row - 3)) {
			if ((alignment == 3)) {
				buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
				buffer2 += (mlib_d64)psrc[j + 2] * psrc[j + 2];

				abuffer0 += (mlib_d64)psrc[j + 0];
				abuffer1 += (mlib_d64)psrc[j + 1];
				abuffer2 += (mlib_d64)psrc[j + 2];

				j += 3;
				sign = 1;
			} else if ((alignment == 2)) {
				buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];

				abuffer0 += (mlib_d64)psrc[j + 0];
				abuffer1 += (mlib_d64)psrc[j + 1];

				j += 2;
				sign = 2;
			} else if ((alignment == 1)) {
				buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				abuffer0 += (mlib_d64)psrc[j + 0];
				j += 1;
				sign = 3;
			}
		}

		db_num = ((size_row - j) >> 3);
		sl = (__m128i *)(psrc + j);

		for (; db_num >= 0; db_num -= MAX_BLOCK_FOR_U16) {

			__m128i load, sdl, sdh, sd1, sd2, sd3, sd4, sd5, sd6;
			__m128i sum1, sum2, sum3, sum4;
			__m128i asum0, load0, load1;
			__m128i sd, sda1, sda2, sda3, sda4;

			sum1 = _mm_setzero_si128();
			sum2 = _mm_setzero_si128();
			asum0 = _mm_setzero_si128();

			mlib_s32 size = MAX_BLOCK_FOR_U16;

			if (db_num < MAX_BLOCK_FOR_U16)
				size = db_num;

			j += (size << 3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (l = 0; (l < size); l++) {
				load = _mm_load_si128(sl);
				sdl = _mm_mullo_epi16(load, load);
				sdh = _mm_mulhi_epu16(load, load);
				sd1 = _mm_unpacklo_epi16(sdl, sdh);
				sd2 = _mm_unpackhi_epi16(sdl, sdh);
				sd3 = _mm_unpacklo_epi32(sd1, x_zero);
				sd4 = _mm_unpackhi_epi32(sd1, x_zero);
				sd5 = _mm_unpacklo_epi32(sd2, x_zero);
				sd6 = _mm_unpackhi_epi32(sd2, x_zero);

				sum1 = _mm_add_epi64(sum1, sd3);
				sum2 = _mm_add_epi64(sum2, sd4);
				sum1 = _mm_add_epi64(sum1, sd5);
				sum2 = _mm_add_epi64(sum2, sd6);

				sda1 = _mm_unpacklo_epi16(load, x_zero);
				sda2 = _mm_unpackhi_epi16(load, x_zero);
				asum0 = _mm_add_epi32(sda1, asum0);
				asum0 = _mm_add_epi32(sda2, asum0);

				sl++;
			}

			if (sign == 3) {
				buffer0 += (mlib_d64)((mlib_s64 *)&sum2)[1];
				buffer1 += (mlib_d64)psrc[j + 0] * psrc[j + 0] +
					(mlib_d64)((mlib_s64 *)&sum1)[0];
				buffer2 += (mlib_d64)psrc[j + 1] * psrc[j + 1] +
					(mlib_d64)((mlib_s64 *)&sum1)[1];
				buffer3 += (mlib_d64)psrc[j + 2] * psrc[j + 2] +
					(mlib_d64)((mlib_s64 *)&sum2)[0];

				abuffer0 += (mlib_d64)((mlib_s32 *)&asum0)[3];
				abuffer1 += (mlib_d64)psrc[j + 0] +
					(mlib_d64)((mlib_s32 *)&asum0)[0];
				abuffer2 += (mlib_d64)psrc[j + 1] +
					(mlib_d64)((mlib_s32 *)&asum0)[1];
				abuffer3 += (mlib_d64)psrc[j + 2] +
					(mlib_d64)((mlib_s32 *)&asum0)[2];

				j += 3;

			} else if (sign == 2) {
				buffer0 += (mlib_d64)((mlib_s64 *)&sum2)[0];
				buffer1 += (mlib_d64)((mlib_s64 *)&sum2)[1];
				buffer2 += (mlib_d64)psrc[j + 0] * psrc[j + 0] +
					(mlib_d64)((mlib_s64 *)&sum1)[0];
				buffer3 += (mlib_d64)psrc[j + 1] * psrc[j + 1] +
					(mlib_d64)((mlib_s64 *)&sum1)[1];

				abuffer0 += (mlib_d64)((mlib_s32 *)&asum0)[2];
				abuffer1 += (mlib_d64)((mlib_s32 *)&asum0)[3];
				abuffer2 += (mlib_d64)psrc[j + 0] +
					(mlib_d64)((mlib_s32 *)&asum0)[0];
				abuffer3 += (mlib_d64)psrc[j + 1] +
					(mlib_d64)((mlib_s32 *)&asum0)[1];

				j += 2;

			} else if (sign == 1) {
				buffer0 += (mlib_d64)((mlib_s64 *)&sum1)[1];
				buffer1 += (mlib_d64)((mlib_s64 *)&sum2)[0];
				buffer2 += (mlib_d64)((mlib_s64 *)&sum2)[1];
				buffer3 += (mlib_d64)psrc[j + 0] * psrc[j + 0] +
					(mlib_d64)((mlib_s64 *)&sum1)[0];

				abuffer0 += (mlib_d64)((mlib_s32 *)&asum0)[1];
				abuffer1 += (mlib_d64)((mlib_s32 *)&asum0)[2];
				abuffer2 += (mlib_d64)((mlib_s32 *)&asum0)[3];
				abuffer3 += (mlib_d64)psrc[j + 0] +
					(mlib_d64)((mlib_s32 *)&asum0)[0];

				j++;

			} else if (sign == 0) {
				buffer0 += (mlib_d64)((mlib_s64 *)&sum1)[0];
				buffer1 += (mlib_d64)((mlib_s64 *)&sum1)[1];
				buffer2 += (mlib_d64)((mlib_s64 *)&sum2)[0];
				buffer3 += (mlib_d64)((mlib_s64 *)&sum2)[1];

				abuffer0 += (mlib_d64)((mlib_s32 *)&asum0)[0];
				abuffer1 += (mlib_d64)((mlib_s32 *)&asum0)[1];
				abuffer2 += (mlib_d64)((mlib_s32 *)&asum0)[2];
				abuffer3 += (mlib_d64)((mlib_s32 *)&asum0)[3];

			}

		}

		for (; j < (size_row - 3); j += 4) {
			buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
			buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			buffer2 += (mlib_d64)psrc[j + 2] * psrc[j + 2];
			buffer3 += (mlib_d64)psrc[j + 3] * psrc[j + 3];

			abuffer0 += (mlib_d64)psrc[j + 0];
			abuffer1 += (mlib_d64)psrc[j + 1];
			abuffer2 += (mlib_d64)psrc[j + 2];
			abuffer3 += (mlib_d64)psrc[j + 3];
		}

		psrc += src_stride;
	}

	mlib_d64 moment0 = 0.0, mean0 = 0.0;
	mlib_d64 moment1 = 0.0, mean1 = 0.0;
	mlib_d64 moment2 = 0.0, mean2 = 0.0;
	mlib_d64 moment3 = 0.0, mean3 = 0.0;

	moment0 = buffer0 * divider;
	moment1 = buffer1 * divider;
	moment2 = buffer2 * divider;
	moment3 = buffer3 * divider;
	mean0 = abuffer0 * divider;
	mean1 = abuffer1 * divider;
	mean2 = abuffer2 * divider;
	mean3 = abuffer3 * divider;

	stdev[0] = mlib_sqrt(mlib_fabs(moment0 - mean0 * mean0));
	stdev[1] = mlib_sqrt(mlib_fabs(moment1 - mean1 * mean1));
	stdev[2] = mlib_sqrt(mlib_fabs(moment2 - mean2 * mean2));
	stdev[3] = mlib_sqrt(mlib_fabs(moment3 - mean3 * mean3));

}
/* *********************************************************** */


void
mlib_s_ImageStdDev2ParamU16_3(
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

/* indicies */
	mlib_s32 i, j;
	mlib_s32 loadsNum, pixelNum;
	mlib_d64 ch0, ch1, ch2, ch3;
	mlib_d64 sum0 = 0.0, sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
	mlib_d64 mtwo0 = 0.0, mtwo1 = 0.0, mtwo2 = 0.0, mtwo3 = 0.0;
	mlib_d64 divider = 1 / (mlib_d64)srcTotalPixNum;
	mlib_u32 a1, a2, a3, a4;
	mlib_s32 align;
	mlib_d64 dsum[4], moment[4];


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

	mlib_d64 moment0 = 0.0, mean0 = 0.0;
	mlib_d64 moment1 = 0.0, mean1 = 0.0;
	mlib_d64 moment2 = 0.0, mean2 = 0.0;

	moment0 = mtwo0 * divider;
	moment1 = mtwo1 * divider;
	moment2 = mtwo2 * divider;
	mean0 = sum0 * divider;
	mean1 = sum1 * divider;
	mean2 = sum2 * divider;

	stdev[0] = mlib_sqrt(mlib_fabs(moment0 - mean0 * mean0));
	stdev[1] = mlib_sqrt(mlib_fabs(moment1 - mean1 * mean1));
	stdev[2] = mlib_sqrt(mlib_fabs(moment2 - mean2 * mean2));

}
/* *********************************************************** */
void
mlib_s_ImageStdDev2ParamS16_1(
    const mlib_image *src,
    mlib_d64 *stdev)
{
/* pointer to the data of image */
	mlib_s16 *psrc = (mlib_s16 *)mlib_ImageGetData(src);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(src);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(src) >> 1;
	mlib_s32 src_chan = mlib_ImageGetChannels(src);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(src);

/* total number of pixels */
	mlib_s32 srcTotalPixNum = height * size_row;

/* indices */
	mlib_s32 i, j, l;

	mlib_d64 divider = 1.0 / srcTotalPixNum;

	if (src_stride == size_row) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 abuffer0 = 0.0;
	mlib_s32 db_num;
	__m128i* sl;
	const __m128i x_zero = _mm_setzero_si128();
	__m128i x_sign;

	for (i = 0; i < height; i++) {
		j = 0;

		for (j = 0; (j < size_row) &&
		    ((mlib_addr)(psrc +j) & 15); j++) {
			buffer0 += (mlib_d64)psrc[j] * psrc[j];
			abuffer0 += (mlib_d64)psrc[j];
		}

		db_num = ((size_row - j) >> 3);
		sl = (__m128i *)(psrc + j);

		for (; db_num > 0; db_num -= MAX_BLOCK_FOR_U16) {
			__m128i load, sdl, sdh, sd1, sd2, sd3, sd4, sd5, sd6;
			__m128i sum1, sum2, sum3, sum4;
			__m128i asum0, load0, load1;
			__m128i sd, sda1, sda2, sda3, sda4;

			sum1 = _mm_setzero_si128();
			sum2 = _mm_setzero_si128();
			sum3 = _mm_setzero_si128();
			sum4 = _mm_setzero_si128();
			asum0 = _mm_setzero_si128();

			mlib_s32 size = MAX_BLOCK_FOR_U16;

			if (db_num < MAX_BLOCK_FOR_U16)
				size = db_num;

			j += (size << 3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (l = 0; (l < size); l++) {
				load = _mm_load_si128(sl);
				sdl = _mm_madd_epi16(load, load);
				sd1 = _mm_unpacklo_epi32(sdl, x_zero);
				sd2 = _mm_unpackhi_epi32(sdl, x_zero);
				sum1 = _mm_add_epi64(sum1, sd1);
				sum2 = _mm_add_epi64(sum2, sd2);

				x_sign = _mm_srai_epi16(load, 15);
				sda1 = _mm_unpacklo_epi16(load, x_sign);
				sda2 = _mm_unpackhi_epi16(load, x_sign);
				asum0 = _mm_add_epi32(sda1, asum0);
				asum0 = _mm_add_epi32(sda2, asum0);

				sl++;
			}


			sum1 = _mm_add_epi64(sum1, sum2);

			buffer0 += (mlib_d64)((mlib_s64 *)&sum1)[0] +
				((mlib_s64 *)&sum1)[1];
			abuffer0 += (mlib_d64)((mlib_s32 *)&asum0)[0] +
				((mlib_s32 *)&asum0)[1] +
				((mlib_s32 *)&asum0)[2] +
				((mlib_s32 *)&asum0)[3];
		}

		for (; j < size_row; j++) {
			buffer0 += (mlib_d64)psrc[j] * psrc[j];
			abuffer0 += (mlib_d64)psrc[j];
		}

		psrc += src_stride;
	}

	mlib_d64 moment0 = 0.0, mean0 = 0.0;

	moment0 = buffer0 * divider;
	mean0 = abuffer0 * divider;

	stdev[0] = mlib_sqrt(mlib_fabs(moment0 - mean0 * mean0));

}
/* *********************************************************** */

void
mlib_s_ImageStdDev2ParamS16_2(
    const mlib_image *src,
    mlib_d64 *stdev)
{
/* pointer to the data of image */
	mlib_s16 *psrc = (mlib_s16 *)mlib_ImageGetData(src);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(src);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(src) >> 1;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(src) << 1;

/* total number of pixels */
	mlib_s32 srcTotalPixNum = height * (size_row >> 1);

/* indices */
	mlib_s32 i, j, l;

	mlib_d64 divider = 1.0 / srcTotalPixNum;

	if (src_stride == size_row) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_d64 abuffer0 = 0.0;
	mlib_d64 abuffer1 = 0.0;

	__m128i* sl;
	const __m128i x_zero = _mm_setzero_si128();
	__m128i x_sign;

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 alignment = 1;
		mlib_s32 db_num;

		if ((mlib_addr)psrc & 1) {
			alignment = 0;
			buffer0 += (mlib_d64)psrc[0] * psrc[0];
			abuffer0 += (mlib_d64)psrc[0];

			for (j = 1; (j < (size_row - 2)) &&
			    ((mlib_addr)(psrc + j) & 15); j += 2) {
				buffer1 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer0 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
				abuffer1 += (mlib_d64)psrc[j + 0];
				abuffer0 += (mlib_d64)psrc[j + 1];
			}
		} else {
			for (j = 0; (j < size_row) &&
			    ((mlib_addr)(psrc + j) & 15); j += 2) {
				buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
				abuffer0 += (mlib_d64)psrc[j + 0];
				abuffer1 += (mlib_d64)psrc[j + 1];
			}
		}

		db_num = ((size_row - j) >> 3);
		sl = (__m128i *)(psrc + j);

		for (; db_num > 0; db_num -= MAX_BLOCK_FOR_U16) {

			__m128i load, sdl, sdh, sd1, sd2, sd3, sd4, sd5, sd6;
			__m128i sum1, sum2, sum3, sum4;
			__m128i asum0, load0, load1;
			__m128i sd, sda1, sda2, sda3, sda4;

			sum1 = _mm_setzero_si128();
			sum2 = _mm_setzero_si128();
			asum0 = _mm_setzero_si128();

			mlib_s32 size = MAX_BLOCK_FOR_U16;

			if (db_num < MAX_BLOCK_FOR_U16)
				size = db_num;

			j += (size << 3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (l = 0; (l < size); l++) {
				load = _mm_load_si128(sl);
				sdl = _mm_mullo_epi16(load, load);
				sdh = _mm_mulhi_epi16(load, load);
				sd1 = _mm_unpacklo_epi16(sdl, sdh);
				sd2 = _mm_unpackhi_epi16(sdl, sdh);
				sd3 = _mm_unpacklo_epi32(sd1, x_zero);
				sd4 = _mm_unpackhi_epi32(sd1, x_zero);
				sd5 = _mm_unpacklo_epi32(sd2, x_zero);
				sd6 = _mm_unpackhi_epi32(sd2, x_zero);

				sum1 = _mm_add_epi64(sum1, sd3);
				sum2 = _mm_add_epi64(sum2, sd4);
				sum1 = _mm_add_epi64(sum1, sd5);
				sum2 = _mm_add_epi64(sum2, sd6);

				x_sign = _mm_srai_epi16(load, 15);
				sda1 = _mm_unpacklo_epi16(load, x_sign);
				sda2 = _mm_unpackhi_epi16(load, x_sign);
				asum0 = _mm_add_epi32(sda1, asum0);
				asum0 = _mm_add_epi32(sda2, asum0);

				sl++;
			}

			sum1 = _mm_add_epi64(sum1, sum2);

			if (alignment) {
				buffer0 += ((mlib_s64 *)&sum1)[0];
				buffer1 += ((mlib_s64 *)&sum1)[1];
				abuffer0 += ((mlib_s32 *)&asum0)[0] +
					((mlib_s32 *)&asum0)[2];
				abuffer1 += ((mlib_s32 *)&asum0)[1] +
					((mlib_s32 *)&asum0)[3];

			} else {
				buffer1 += ((mlib_s64 *)&sum1)[0];
				buffer0 += ((mlib_s64 *)&sum1)[1];
				abuffer1 += ((mlib_s32 *)&asum0)[0] +
					((mlib_s32 *)&asum0)[2];
				abuffer0 += ((mlib_s32 *)&asum0)[1] +
					((mlib_s32 *)&asum0)[3];
			}

		}

		if (alignment) {
			for (; j < size_row; j += 2) {
				buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
				abuffer0 += (mlib_d64)psrc[j + 0];
				abuffer1 += (mlib_d64)psrc[j + 1];
			}
		} else {
			for (; j < (size_row - 1); j += 2) {
				buffer1 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer0 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
				abuffer1 += (mlib_d64)psrc[j + 0];
				abuffer0 += (mlib_d64)psrc[j + 1];
			}
			buffer1 += (mlib_d64)psrc[j] * psrc[j];
			abuffer1 += (mlib_d64)psrc[j];
		}

		psrc += src_stride;
	}

	mlib_d64 moment0 = 0.0, mean0 = 0.0;
	mlib_d64 moment1 = 0.0, mean1 = 0.0;

	moment0 = buffer0 * divider;
	moment1 = buffer1 * divider;
	mean0 = abuffer0 * divider;
	mean1 = abuffer1 * divider;

	stdev[0] = mlib_sqrt(mlib_fabs(moment0 - mean0 * mean0));
	stdev[1] = mlib_sqrt(mlib_fabs(moment1 - mean1 * mean1));

}
/* *********************************************************** */

void
mlib_s_ImageStdDev2ParamS16_4(
    const mlib_image *src,
    mlib_d64 *stdev)
{
/* pointer to the data of image */
	mlib_s16 *psrc = (mlib_s16 *)mlib_ImageGetData(src);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(src);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(src) >> 1;
	mlib_s32 src_chan = mlib_ImageGetChannels(src);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(src) << (src_chan >> 1);

/* total number of pixels */
	mlib_s32 srcTotalPixNum = height * (size_row >> 2);

/* indices */
	mlib_s32 i, j, l;

	mlib_d64 divider = 1.0 / srcTotalPixNum;

	if (src_stride == size_row) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_d64 buffer2 = 0.0;
	mlib_d64 buffer3 = 0.0;
	mlib_d64 abuffer0 = 0.0;
	mlib_d64 abuffer1 = 0.0;
	mlib_d64 abuffer2 = 0.0;
	mlib_d64 abuffer3 = 0.0;

	mlib_s32 db_num;
	__m128i* sl;
	const __m128i x_zero = _mm_setzero_si128();
	__m128i x_sign;

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 sign = 0;
		mlib_s32 alignment =
		    ((16 - ((mlib_addr)psrc & 15)) & 15) >> 1;

		for (; (j < size_row - 3) && (alignment >= 4); j += 4) {
			buffer0 += (mlib_d64)psrc[0] * psrc[0];
			buffer1 += (mlib_d64)psrc[1] * psrc[1];
			buffer2 += (mlib_d64)psrc[2] * psrc[2];
			buffer3 += (mlib_d64)psrc[3] * psrc[3];

			abuffer0 += (mlib_d64)psrc[0];
			abuffer1 += (mlib_d64)psrc[1];
			abuffer2 += (mlib_d64)psrc[2];
			abuffer3 += (mlib_d64)psrc[3];

			alignment -= 4;
		}

		if (j < (size_row - 3)) {
			if ((alignment == 3)) {
				buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
				buffer2 += (mlib_d64)psrc[j + 2] * psrc[j + 2];

				abuffer0 += (mlib_d64)psrc[j + 0];
				abuffer1 += (mlib_d64)psrc[j + 1];
				abuffer2 += (mlib_d64)psrc[j + 2];

				j += 3;
				sign = 1;
			} else if ((alignment == 2)) {
				buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];

				abuffer0 += (mlib_d64)psrc[j + 0];
				abuffer1 += (mlib_d64)psrc[j + 1];

				j += 2;
				sign = 2;
			} else if ((alignment == 1)) {
				buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				abuffer0 += (mlib_d64)psrc[j + 0];
				j += 1;
				sign = 3;
			}
		}

		db_num = ((size_row - j) >> 3);
		sl = (__m128i *)(psrc + j);

		for (; db_num >= 0; db_num -= MAX_BLOCK_FOR_U16) {

			__m128i load, sdl, sdh, sd1, sd2, sd3, sd4, sd5, sd6;
			__m128i sum1, sum2, sum3, sum4;
			__m128i asum0, load0, load1;
			__m128i sd, sda1, sda2, sda3, sda4;

			sum1 = _mm_setzero_si128();
			sum2 = _mm_setzero_si128();
			asum0 = _mm_setzero_si128();

			mlib_s32 size = MAX_BLOCK_FOR_U16;

			if (db_num < MAX_BLOCK_FOR_U16)
				size = db_num;

			j += (size << 3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (l = 0; (l < size); l++) {
				load = _mm_load_si128(sl);
				sdl = _mm_mullo_epi16(load, load);
				sdh = _mm_mulhi_epi16(load, load);
				sd1 = _mm_unpacklo_epi16(sdl, sdh);
				sd2 = _mm_unpackhi_epi16(sdl, sdh);
				sd3 = _mm_unpacklo_epi32(sd1, x_zero);
				sd4 = _mm_unpackhi_epi32(sd1, x_zero);
				sd5 = _mm_unpacklo_epi32(sd2, x_zero);
				sd6 = _mm_unpackhi_epi32(sd2, x_zero);

				sum1 = _mm_add_epi64(sum1, sd3);
				sum2 = _mm_add_epi64(sum2, sd4);
				sum1 = _mm_add_epi64(sum1, sd5);
				sum2 = _mm_add_epi64(sum2, sd6);

				x_sign = _mm_srai_epi16(load, 15);
				sda1 = _mm_unpacklo_epi16(load, x_sign);
				sda2 = _mm_unpackhi_epi16(load, x_sign);
				asum0 = _mm_add_epi32(sda1, asum0);
				asum0 = _mm_add_epi32(sda2, asum0);

				sl++;
			}

			if (sign == 3) {
				buffer0 += (mlib_d64)((mlib_s64 *)&sum2)[1];
				buffer1 += (mlib_d64)psrc[j + 0] * psrc[j + 0] +
					(mlib_d64)((mlib_s64 *)&sum1)[0];
				buffer2 += (mlib_d64)psrc[j + 1] * psrc[j + 1] +
					(mlib_d64)((mlib_s64 *)&sum1)[1];
				buffer3 += (mlib_d64)psrc[j + 2] * psrc[j + 2] +
					(mlib_d64)((mlib_s64 *)&sum2)[0];

				abuffer0 += (mlib_d64)((mlib_s32 *)&asum0)[3];
				abuffer1 += (mlib_d64)psrc[j + 0] +
					(mlib_d64)((mlib_s32 *)&asum0)[0];
				abuffer2 += (mlib_d64)psrc[j + 1] +
					(mlib_d64)((mlib_s32 *)&asum0)[1];
				abuffer3 += (mlib_d64)psrc[j + 2] +
					(mlib_d64)((mlib_s32 *)&asum0)[2];

				j += 3;

			} else if (sign == 2) {
				buffer0 += (mlib_d64)((mlib_s64 *)&sum2)[0];
				buffer1 += (mlib_d64)((mlib_s64 *)&sum2)[1];
				buffer2 += (mlib_d64)psrc[j + 0] * psrc[j + 0] +
					(mlib_d64)((mlib_s64 *)&sum1)[0];
				buffer3 += (mlib_d64)psrc[j + 1] * psrc[j + 1] +
					(mlib_d64)((mlib_s64 *)&sum1)[1];

				abuffer0 += (mlib_d64)((mlib_s32 *)&asum0)[2];
				abuffer1 += (mlib_d64)((mlib_s32 *)&asum0)[3];
				abuffer2 += (mlib_d64)psrc[j + 0] +
					(mlib_d64)((mlib_s32 *)&asum0)[0];
				abuffer3 += (mlib_d64)psrc[j + 1] +
					(mlib_d64)((mlib_s32 *)&asum0)[1];

				j += 2;

			} else if (sign == 1) {
				buffer0 += (mlib_d64)((mlib_s64 *)&sum1)[1];
				buffer1 += (mlib_d64)((mlib_s64 *)&sum2)[0];
				buffer2 += (mlib_d64)((mlib_s64 *)&sum2)[1];
				buffer3 += (mlib_d64)psrc[j + 0] * psrc[j + 0] +
					(mlib_d64)((mlib_s64 *)&sum1)[0];

				abuffer0 += (mlib_d64)((mlib_s32 *)&asum0)[1];
				abuffer1 += (mlib_d64)((mlib_s32 *)&asum0)[2];
				abuffer2 += (mlib_d64)((mlib_s32 *)&asum0)[3];
				abuffer3 += (mlib_d64)psrc[j + 0] +
					(mlib_d64)((mlib_s32 *)&asum0)[0];

				j++;

			} else if (sign == 0) {
				buffer0 += (mlib_d64)((mlib_s64 *)&sum1)[0];
				buffer1 += (mlib_d64)((mlib_s64 *)&sum1)[1];
				buffer2 += (mlib_d64)((mlib_s64 *)&sum2)[0];
				buffer3 += (mlib_d64)((mlib_s64 *)&sum2)[1];

				abuffer0 += (mlib_d64)((mlib_s32 *)&asum0)[0];
				abuffer1 += (mlib_d64)((mlib_s32 *)&asum0)[1];
				abuffer2 += (mlib_d64)((mlib_s32 *)&asum0)[2];
				abuffer3 += (mlib_d64)((mlib_s32 *)&asum0)[3];

			}

		}

		for (; j < (size_row - 3); j += 4) {
			buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
			buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			buffer2 += (mlib_d64)psrc[j + 2] * psrc[j + 2];
			buffer3 += (mlib_d64)psrc[j + 3] * psrc[j + 3];

			abuffer0 += (mlib_d64)psrc[j + 0];
			abuffer1 += (mlib_d64)psrc[j + 1];
			abuffer2 += (mlib_d64)psrc[j + 2];
			abuffer3 += (mlib_d64)psrc[j + 3];
		}

		psrc += src_stride;
	}

	mlib_d64 moment0 = 0.0, mean0 = 0.0;
	mlib_d64 moment1 = 0.0, mean1 = 0.0;
	mlib_d64 moment2 = 0.0, mean2 = 0.0;
	mlib_d64 moment3 = 0.0, mean3 = 0.0;

	moment0 = buffer0 * divider;
	moment1 = buffer1 * divider;
	moment2 = buffer2 * divider;
	moment3 = buffer3 * divider;
	mean0 = abuffer0 * divider;
	mean1 = abuffer1 * divider;
	mean2 = abuffer2 * divider;
	mean3 = abuffer3 * divider;

	stdev[0] = mlib_sqrt(mlib_fabs(moment0 - mean0 * mean0));
	stdev[1] = mlib_sqrt(mlib_fabs(moment1 - mean1 * mean1));
	stdev[2] = mlib_sqrt(mlib_fabs(moment2 - mean2 * mean2));
	stdev[3] = mlib_sqrt(mlib_fabs(moment3 - mean3 * mean3));

}
/* *********************************************************** */

void
mlib_s_ImageStdDev2ParamS16_3(
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

/* indicies */
	mlib_s32 i, j;
	mlib_s32 loadsNum, pixelNum;
	mlib_d64 ch0, ch1, ch2, ch3;
	mlib_d64 sum0 = 0.0, sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
	mlib_d64 mtwo0 = 0.0, mtwo1 = 0.0, mtwo2 = 0.0, mtwo3 = 0.0;
	mlib_d64 divider = 1 / (mlib_d64)srcTotalPixNum;
	mlib_s32 a1, a2, a3, a4;
	mlib_s32 align;
	mlib_d64 dsum[4], moment[4];

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

	mlib_d64 moment0 = 0.0, mean0 = 0.0;
	mlib_d64 moment1 = 0.0, mean1 = 0.0;
	mlib_d64 moment2 = 0.0, mean2 = 0.0;

	moment0 = mtwo0 * divider;
	moment1 = mtwo1 * divider;
	moment2 = mtwo2 * divider;
	mean0 = sum0 * divider;
	mean1 = sum1 * divider;
	mean2 = sum2 * divider;

	stdev[0] = mlib_sqrt(mlib_fabs(moment0 - mean0 * mean0));
	stdev[1] = mlib_sqrt(mlib_fabs(moment1 - mean1 * mean1));
	stdev[2] = mlib_sqrt(mlib_fabs(moment2 - mean2 * mean2));

}
/* *********************************************************** */

void
mlib_s_ImageStdDev2ParamS32_1(
    const mlib_image *src,
    mlib_d64 *stdev)
{
/* pointer to the data of image */
	mlib_s32 *psrc = (mlib_s32 *)mlib_ImageGetData(src);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(src);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(src) >> 2;
	mlib_s32 src_chan = mlib_ImageGetChannels(src);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(src);

/* total number of pixels */
	mlib_s32 srcTotalPixNum = height * size_row;

/* indices */
	mlib_s32 i, j, l;

	mlib_d64 divider = 1 / (mlib_d64)srcTotalPixNum;

	if (src_stride == size_row) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 abuffer0 = 0.0;
	mlib_s32 db_num;
	mlib_s32 *sl;

	for (i = 0; i < height; i++) {
		j = 0;

		for (j = 0; (j < size_row) &&
		    ((mlib_addr)(psrc +j) & 15); j++) {
			buffer0 += (mlib_d64)psrc[j] * psrc[j];
			abuffer0 += (mlib_d64)psrc[j];
		}

		db_num = ((size_row - j) >> 2);
		sl = psrc + j;
		__m128d fd1, fd2, fs1, fs2, fsum1, fasum1;
		fsum1 = _mm_setzero_pd();
		fasum1 = _mm_setzero_pd();

		j += (db_num << 2);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			fd1 = _mm_cvtepi32_pd(*((__m128i *)sl));
			fd2 = _mm_cvtepi32_pd(*((__m128i *)(sl + 2)));

			fs1 = _mm_mul_pd(fd1, fd1);
			fs2 = _mm_mul_pd(fd2, fd2);
			fsum1 = _mm_add_pd(fsum1, fs1);
			fsum1 = _mm_add_pd(fsum1, fs2);

			fasum1 = _mm_add_pd(fasum1, fd1);
			fasum1 = _mm_add_pd(fasum1, fd2);

			sl += 4;
		}

		for (; j < size_row; j++) {
			buffer0 += (mlib_d64)psrc[j] * psrc[j];
			abuffer0 += (mlib_d64)psrc[j];
		}

		buffer0 += ((mlib_d64 *)&fsum1)[0] +
			((mlib_d64 *)&fsum1)[1];
		abuffer0 += ((mlib_d64 *)&fasum1)[0] +
			((mlib_d64 *)&fasum1)[1];

		psrc += src_stride;
	}

	mlib_d64 moment0 = 0.0, mean0 = 0.0;

	moment0 = buffer0 * divider;
	mean0 = abuffer0 * divider;

	stdev[0] = mlib_sqrt(mlib_fabs(moment0 - mean0 * mean0));
}

/* *********************************************************** */

void
mlib_s_ImageStdDev2ParamS32_2(
    const mlib_image *src,
    mlib_d64 *stdev)
{
/* pointer to the data of image */
	mlib_s32 *psrc = (mlib_s32 *)mlib_ImageGetData(src);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(src);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(src) >> 2;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(src) << 1;

/* indices */
	mlib_s32 i, j, l;

/* total number of pixels */
	mlib_s32 srcTotalPixNum = height * (size_row >> 1);

	mlib_d64 divider = 1 / (mlib_d64)srcTotalPixNum;

	if (src_stride == size_row) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_d64 abuffer0 = 0.0;
	mlib_d64 abuffer1 = 0.0;
	mlib_s32 *sl;

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 alignment = 1;
		mlib_s32 db_num;

		if ((mlib_addr)psrc & 1) {
			alignment = 0;
			buffer0 += (mlib_d64)psrc[0] * psrc[0];
			abuffer0 += (mlib_d64)psrc[0];

			for (j = 1; (j < (size_row - 2)) &&
			    ((mlib_addr)(psrc + j) & 15); j += 2) {
				buffer1 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer0 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
				abuffer1 += (mlib_d64)psrc[j + 0];
				abuffer0 += (mlib_d64)psrc[j + 1];
			}
		} else {
			for (j = 0; (j < size_row) &&
			    ((mlib_addr)(psrc + j) & 15); j += 2) {
				buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
				abuffer0 += (mlib_d64)psrc[j + 0];
				abuffer1 += (mlib_d64)psrc[j + 1];
			}
		}

		db_num = ((size_row - j) >> 2);
		sl = psrc + j;
		__m128d fd1, fd2, fs1, fs2, fsum1, fasum1;
		fsum1 = _mm_setzero_pd();
		fasum1 = _mm_setzero_pd();

		j += (db_num << 2);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			fd1 = _mm_cvtepi32_pd(*((__m128i *)sl));
			fd2 = _mm_cvtepi32_pd(*((__m128i *)(sl + 2)));

			fs1 = _mm_mul_pd(fd1, fd1);
			fs2 = _mm_mul_pd(fd2, fd2);
			fsum1 = _mm_add_pd(fsum1, fs1);
			fsum1 = _mm_add_pd(fsum1, fs2);

			fasum1 = _mm_add_pd(fasum1, fd1);
			fasum1 = _mm_add_pd(fasum1, fd2);

			sl += 4;
		}

		if (alignment) {
			buffer0 += ((mlib_d64 *)&fsum1)[0];
			buffer1 += ((mlib_d64 *)&fsum1)[1];
			abuffer0 += ((mlib_d64 *)&fasum1)[0];
			abuffer1 += ((mlib_d64 *)&fasum1)[1];

			for (; j < size_row; j += 2) {
				buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
				abuffer0 += (mlib_d64)psrc[j + 0];
				abuffer1 += (mlib_d64)psrc[j + 1];
			}
		} else {
			buffer1 += ((mlib_d64 *)&fsum1)[0];
			buffer0 += ((mlib_d64 *)&fsum1)[1];
			abuffer1 += ((mlib_d64 *)&fasum1)[0];
			abuffer0 += ((mlib_d64 *)&fasum1)[1];

			for (; j < (size_row - 1); j += 2) {
				buffer1 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer0 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
				abuffer1 += (mlib_d64)psrc[j + 0];
				abuffer0 += (mlib_d64)psrc[j + 1];
			}

			buffer1 += (mlib_d64)psrc[j] * psrc[j];
			abuffer1 += (mlib_d64)psrc[j];
		}

		psrc += src_stride;
	}

	mlib_d64 moment0 = 0.0, mean0 = 0.0;
	mlib_d64 moment1 = 0.0, mean1 = 0.0;

	moment0 = buffer0 * divider;
	moment1 = buffer1 * divider;
	mean0 = abuffer0 * divider;
	mean1 = abuffer1 * divider;

	stdev[0] = mlib_sqrt(mlib_fabs(moment0 - mean0 * mean0));
	stdev[1] = mlib_sqrt(mlib_fabs(moment1 - mean1 * mean1));
}
/* *********************************************************** */

void
mlib_s_ImageStdDev2ParamS32_3(
    const mlib_image *src,
    mlib_d64 *stdev)
{
/* pointer to the data of image */
	mlib_s32 *psrc = (mlib_s32 *)mlib_ImageGetData(src);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(src);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(src) >> 2;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(src) * 3;

/* indices */
	mlib_s32 i, j, l;

/* total number of pixels */
	mlib_s32 srcTotalPixNum = height * (size_row / 3);

	mlib_d64 divider = 1 / (mlib_d64)srcTotalPixNum;

	if (src_stride == size_row) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_d64 buffer2 = 0.0;
	mlib_d64 buffer3 = 0.0;
	mlib_d64 buffer4 = 0.0;
	mlib_d64 buffer5 = 0.0;

	mlib_d64 abuffer0 = 0.0;
	mlib_d64 abuffer1 = 0.0;
	mlib_d64 abuffer2 = 0.0;
	mlib_d64 abuffer3 = 0.0;
	mlib_d64 abuffer4 = 0.0;
	mlib_d64 abuffer5 = 0.0;

	mlib_s32 db_num;
	mlib_s32 *sl;

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 sign = 0;
		mlib_s32 alignment =
			((16 - ((mlib_addr)psrc & 15)) & 15) >> 2;

		for (j = 0;
		    (j < (size_row - 3)) && (j <= (alignment - 3));
		    j += 3) {
			buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
			buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			buffer2 += (mlib_d64)psrc[j + 2] * psrc[j + 2];

			abuffer0 += (mlib_d64)psrc[j + 0];
			abuffer1 += (mlib_d64)psrc[j + 1];
			abuffer2 += (mlib_d64)psrc[j + 2];
		}

		if ((alignment - j) == 2) {
			buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
			buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			abuffer0 += (mlib_d64)psrc[j + 0];
			abuffer1 += (mlib_d64)psrc[j + 1];

			j += 2;
			sign = 1;
		} else if ((alignment - j) == 1) {
			buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
			abuffer0 += (mlib_d64)psrc[j + 0];
			j += 1;
			sign = 2;
		}

		db_num = ((size_row - j) / 12);
		sl = psrc + j;
		__m128d fd0, fd1, fd2, fd3, fd4, fd5;
		__m128d fs0, fs1, fs2, fs3, fs4, fs5;
		__m128d fsum0, fsum1, fsum2, fsum3, fsum4, fsum5;
		__m128d fasum0, fasum1, fasum2, fasum3, fasum4, fasum5;

		fsum0 = _mm_setzero_pd();
		fsum1 = _mm_setzero_pd();
		fsum2 = _mm_setzero_pd();
		fsum3 = _mm_setzero_pd();
		fsum4 = _mm_setzero_pd();
		fsum5 = _mm_setzero_pd();

		fasum0 = _mm_setzero_pd();
		fasum1 = _mm_setzero_pd();
		fasum2 = _mm_setzero_pd();
		fasum3 = _mm_setzero_pd();
		fasum4 = _mm_setzero_pd();
		fasum5 = _mm_setzero_pd();

		j += (db_num * 12);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			fd0 = _mm_cvtepi32_pd(*((__m128i *)sl));
			fd1 = _mm_cvtepi32_pd(*((__m128i *)(sl + 2)));
			fd2 = _mm_cvtepi32_pd(*((__m128i *)(sl + 4)));
			fd3 = _mm_cvtepi32_pd(*((__m128i *)(sl + 6)));
			fd4 = _mm_cvtepi32_pd(*((__m128i *)(sl + 8)));
			fd5 = _mm_cvtepi32_pd(*((__m128i *)(sl + 10)));

			fs0 = _mm_mul_pd(fd0, fd0);
			fs1 = _mm_mul_pd(fd1, fd1);
			fs2 = _mm_mul_pd(fd2, fd2);
			fs3 = _mm_mul_pd(fd3, fd3);
			fs4 = _mm_mul_pd(fd4, fd4);
			fs5 = _mm_mul_pd(fd5, fd5);

			fsum0 = _mm_add_pd(fsum0, fs0);
			fsum1 = _mm_add_pd(fsum1, fs1);
			fsum2 = _mm_add_pd(fsum2, fs2);
			fsum3 = _mm_add_pd(fsum3, fs3);
			fsum4 = _mm_add_pd(fsum4, fs4);
			fsum5 = _mm_add_pd(fsum5, fs5);

			fasum0 = _mm_add_pd(fasum0, fd0);
			fasum1 = _mm_add_pd(fasum1, fd1);
			fasum2 = _mm_add_pd(fasum2, fd2);
			fasum3 = _mm_add_pd(fasum3, fd3);
			fasum4 = _mm_add_pd(fasum4, fd4);
			fasum5 = _mm_add_pd(fasum5, fd5);

			sl += 12;
		}

		buffer3 = ((mlib_d64 *)&fsum0)[0] +
			((mlib_d64 *)&fsum1)[1] +
			((mlib_d64 *)&fsum3)[0] +
			((mlib_d64 *)&fsum4)[1];
		buffer4 = ((mlib_d64 *)&fsum0)[1] +
			((mlib_d64 *)&fsum2)[0] +
			((mlib_d64 *)&fsum3)[1] +
			((mlib_d64 *)&fsum5)[0];
		buffer5 = ((mlib_d64 *)&fsum1)[0] +
			((mlib_d64 *)&fsum2)[1] +
			((mlib_d64 *)&fsum4)[0] +
			((mlib_d64 *)&fsum5)[1];

		abuffer3 = ((mlib_d64 *)&fasum0)[0] +
			((mlib_d64 *)&fasum1)[1] +
			((mlib_d64 *)&fasum3)[0] +
			((mlib_d64 *)&fasum4)[1];
		abuffer4 = ((mlib_d64 *)&fasum0)[1] +
			((mlib_d64 *)&fasum2)[0] +
			((mlib_d64 *)&fasum3)[1] +
			((mlib_d64 *)&fasum5)[0];
		abuffer5 = ((mlib_d64 *)&fasum1)[0] +
			((mlib_d64 *)&fasum2)[1] +
			((mlib_d64 *)&fasum4)[0] +
			((mlib_d64 *)&fasum5)[1];

		for (; j < (size_row - 2); j += 3) {
			buffer3 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
			buffer4 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			buffer5 += (mlib_d64)psrc[j + 2] * psrc[j + 2];

			abuffer3 += (mlib_d64)psrc[j + 0];
			abuffer4 += (mlib_d64)psrc[j + 1];
			abuffer5 += (mlib_d64)psrc[j + 2];
		}

		if (sign == 2) {
			buffer1 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
			buffer2 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			buffer0 += buffer5;
			buffer1 += buffer3;
			buffer2 += buffer4;

			abuffer1 += (mlib_d64)psrc[j + 0];
			abuffer2 += (mlib_d64)psrc[j + 1];
			abuffer0 += abuffer5;
			abuffer1 += abuffer3;
			abuffer2 += abuffer4;

		} else if (sign == 1) {
			buffer2 += (mlib_d64)psrc[j] * psrc[j];
			buffer0 += buffer4;
			buffer1 += buffer5;
			buffer2 += buffer3;

			abuffer2 += (mlib_d64)psrc[j];
			abuffer0 += abuffer4;
			abuffer1 += abuffer5;
			abuffer2 += abuffer3;

		} else if (sign == 0) {
			buffer0 += buffer3;
			buffer1 += buffer4;
			buffer2 += buffer5;

			abuffer0 += abuffer3;
			abuffer1 += abuffer4;
			abuffer2 += abuffer5;

		}

		psrc += src_stride;
	}

	mlib_d64 moment0 = 0.0, mean0 = 0.0;
	mlib_d64 moment1 = 0.0, mean1 = 0.0;
	mlib_d64 moment2 = 0.0, mean2 = 0.0;

	moment0 = buffer0 * divider;
	moment1 = buffer1 * divider;
	moment2 = buffer2 * divider;
	mean0 = abuffer0 * divider;
	mean1 = abuffer1 * divider;
	mean2 = abuffer2 * divider;

	stdev[0] = mlib_sqrt(mlib_fabs(moment0 - mean0 * mean0));
	stdev[1] = mlib_sqrt(mlib_fabs(moment1 - mean1 * mean1));
	stdev[2] = mlib_sqrt(mlib_fabs(moment2 - mean2 * mean2));

}
/* *********************************************************** */

void
mlib_s_ImageStdDev2ParamS32_4(
    const mlib_image *src,
    mlib_d64 *stdev)

{
/* pointer to the data of image */
	mlib_s32 *psrc = (mlib_s32 *)mlib_ImageGetData(src);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(src);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(src) >> 2;
	mlib_s32 src_chan = mlib_ImageGetChannels(src);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(src) << 2;

/* indices */
	mlib_s32 i, j, l;

/* total number of pixels */
	mlib_s32 srcTotalPixNum = height * (size_row >> 2);

	mlib_d64 divider = 1 / (mlib_d64)srcTotalPixNum;

	if (src_stride == size_row) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_d64 buffer2 = 0.0;
	mlib_d64 buffer3 = 0.0;
	mlib_d64 abuffer0 = 0.0;
	mlib_d64 abuffer1 = 0.0;
	mlib_d64 abuffer2 = 0.0;
	mlib_d64 abuffer3 = 0.0;

	mlib_s32 db_num;
	mlib_s32 *sl;

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 sign = 0;
		mlib_s32 alignment =
		    ((16 - ((mlib_addr)psrc & 15)) & 15) >> 2;

		for (; (j < size_row - 3) && (alignment >= 4); j += 4) {
			buffer0 += (mlib_d64)psrc[0] * psrc[0];
			buffer1 += (mlib_d64)psrc[1] * psrc[1];
			buffer2 += (mlib_d64)psrc[2] * psrc[2];
			buffer3 += (mlib_d64)psrc[3] * psrc[3];
			abuffer0 += (mlib_d64)psrc[0];
			abuffer1 += (mlib_d64)psrc[1];
			abuffer2 += (mlib_d64)psrc[2];
			abuffer3 += (mlib_d64)psrc[3];

			alignment -= 4;
		}

		if (j < (size_row - 3)) {
			if ((alignment == 3)) {
				buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
				buffer2 += (mlib_d64)psrc[j + 2] * psrc[j + 2];
				abuffer0 += (mlib_d64)psrc[j + 0];
				abuffer1 += (mlib_d64)psrc[j + 1];
				abuffer2 += (mlib_d64)psrc[j + 2];
				j += 3;
				sign = 1;
			} else if ((alignment == 2)) {
				buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
				abuffer0 += (mlib_d64)psrc[j + 0];
				abuffer1 += (mlib_d64)psrc[j + 1];
				j += 2;
				sign = 2;
			} else if ((alignment == 1)) {
				buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				abuffer0 += (mlib_d64)psrc[j + 0];
				j += 1;
				sign = 3;
			}
		}

		db_num = ((size_row - j) >> 2);
		sl = (psrc + j);
		__m128d fd1, fd2, fs1, fs2, fsum1, fsum2;
		__m128d fasum1, fasum2;
		fsum1 = _mm_setzero_pd();
		fsum2 = _mm_setzero_pd();
		fasum1 = _mm_setzero_pd();
		fasum2 = _mm_setzero_pd();

		j += (db_num << 2);


#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			fd1 = _mm_cvtepi32_pd(*((__m128i *)sl));
			fd2 = _mm_cvtepi32_pd(*((__m128i *)(sl + 2)));

			fs1 = _mm_mul_pd(fd1, fd1);
			fs2 = _mm_mul_pd(fd2, fd2);
			fsum1 = _mm_add_pd(fsum1, fs1);
			fsum2 = _mm_add_pd(fsum2, fs2);

			fasum1 = _mm_add_pd(fasum1, fd1);
			fasum2 = _mm_add_pd(fasum2, fd2);

			sl += 4;
		}


		if (sign == 3) {
			buffer0 += ((mlib_d64 *)&fsum2)[1];
			buffer1 += (mlib_d64)psrc[j + 0] * psrc[j + 0] +
				((mlib_d64 *)&fsum1)[0];
			buffer2 += (mlib_d64)psrc[j + 1] * psrc[j + 1] +
				((mlib_d64 *)&fsum1)[1];
			buffer3 += (mlib_d64)psrc[j + 2] * psrc[j + 2] +
				((mlib_d64 *)&fsum2)[0];

			abuffer0 += ((mlib_d64 *)&fasum2)[1];
			abuffer1 += (mlib_d64)psrc[j + 0] +
				((mlib_d64 *)&fasum1)[0];
			abuffer2 += (mlib_d64)psrc[j + 1] +
				((mlib_d64 *)&fasum1)[1];
			abuffer3 += (mlib_d64)psrc[j + 2] +
				((mlib_d64 *)&fasum2)[0];

			j += 3;

		} else if (sign == 2) {
			buffer0 += ((mlib_d64 *)&fsum2)[0];
			buffer1 += ((mlib_d64 *)&fsum2)[1];
			buffer2 += (mlib_d64)psrc[j + 0] * psrc[j + 0] +
				((mlib_d64 *)&fsum1)[0];
			buffer3 += (mlib_d64)psrc[j + 1] * psrc[j + 1] +
				((mlib_d64 *)&fsum1)[1];

			abuffer0 += ((mlib_d64 *)&fasum2)[0];
			abuffer1 += ((mlib_d64 *)&fasum2)[1];
			abuffer2 += (mlib_d64)psrc[j + 0] +
				((mlib_d64 *)&fasum1)[0];
			abuffer3 += (mlib_d64)psrc[j + 1] +
				((mlib_d64 *)&fasum1)[1];

			j += 2;

		} else if (sign == 1) {
			buffer0 += ((mlib_d64 *)&fsum1)[1];
			buffer1 += ((mlib_d64 *)&fsum2)[0];
			buffer2 += ((mlib_d64 *)&fsum2)[1];
			buffer3 += (mlib_d64)psrc[j + 0] * psrc[j + 0] +
				((mlib_d64 *)&fsum1)[0];

			abuffer0 += ((mlib_d64 *)&fasum1)[1];
			abuffer1 += ((mlib_d64 *)&fasum2)[0];
			abuffer2 += ((mlib_d64 *)&fasum2)[1];
			abuffer3 += (mlib_d64)psrc[j + 0] +
				((mlib_d64 *)&fasum1)[0];

			j++;

		} else if (sign == 0) {
			buffer0 += ((mlib_d64 *)&fsum1)[0];
			buffer1 += ((mlib_d64 *)&fsum1)[1];
			buffer2 += ((mlib_d64 *)&fsum2)[0];
			buffer3 += ((mlib_d64 *)&fsum2)[1];

			abuffer0 += ((mlib_d64 *)&fasum1)[0];
			abuffer1 += ((mlib_d64 *)&fasum1)[1];
			abuffer2 += ((mlib_d64 *)&fasum2)[0];
			abuffer3 += ((mlib_d64 *)&fasum2)[1];
		}

		for (; j < (size_row - 3); j += 4) {
			buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
			buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			buffer2 += (mlib_d64)psrc[j + 2] * psrc[j + 2];
			buffer3 += (mlib_d64)psrc[j + 3] * psrc[j + 3];

			abuffer0 += (mlib_d64)psrc[j + 0];
			abuffer1 += (mlib_d64)psrc[j + 1];
			abuffer2 += (mlib_d64)psrc[j + 2];
			abuffer3 += (mlib_d64)psrc[j + 3];
		}

		psrc += src_stride;
	}

	mlib_d64 moment0 = 0.0, mean0 = 0.0;
	mlib_d64 moment1 = 0.0, mean1 = 0.0;
	mlib_d64 moment2 = 0.0, mean2 = 0.0;
	mlib_d64 moment3 = 0.0, mean3 = 0.0;

	moment0 = buffer0 * divider;
	moment1 = buffer1 * divider;
	moment2 = buffer2 * divider;
	moment3 = buffer3 * divider;
	mean0 = abuffer0 * divider;
	mean1 = abuffer1 * divider;
	mean2 = abuffer2 * divider;
	mean3 = abuffer3 * divider;

	stdev[0] = mlib_sqrt(mlib_fabs(moment0 - mean0 * mean0));
	stdev[1] = mlib_sqrt(mlib_fabs(moment1 - mean1 * mean1));
	stdev[2] = mlib_sqrt(mlib_fabs(moment2 - mean2 * mean2));
	stdev[3] = mlib_sqrt(mlib_fabs(moment3 - mean3 * mean3));

}
/* *********************************************************** */
