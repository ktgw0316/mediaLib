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

#pragma ident	"@(#)mlib_s_ImageMean.c	9.4	07/11/05 SMI"

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

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

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

/*  CONVERT  */

#define	MLIB_S_CONVERT_16U8_8S16(_8s16_1, _8s16_2, _16u8) \
{                                                         \
	_8s16_1 = _mm_unpackhi_epi8(_16u8, x_zero);       \
	_8s16_2 = _mm_unpacklo_epi8(_16u8, x_zero);       \
}

/* *********************************************************** */

#define	MLIB_S_CONVERT_4S16_2S32(_2s32_1, _2s32_2, _4s16)  \
{                                                          \
	__m128i xsign = _mm_srai_epi16(_4s16, 15);         \
	_2s32_1 =                                          \
	    _mm_unpackhi_epi16(_4s16, xsign);              \
	_2s32_2 =                                          \
	    _mm_unpacklo_epi16(_4s16, xsign);              \
}

/* *********************************************************** */

#define	MLIB_S_CONVERT_4U16_2S32(_2s32_1, _2s32_2, _4u16)  \
{                                                          \
	_2s32_1 = _mm_unpackhi_epi16(_4u16, x_zero);       \
	_2s32_2 = _mm_unpacklo_epi16(_4u16, x_zero);       \
}

/* *********************************************************** */

#define	MLIB_S_CONVERT_4S32_2S64(_2s64_1, _2s64_2, _4s32)  \
{                                                          \
	__m128i _4s32sign = _mm_srai_epi32(_4s32, 31);	   \
	_2s64_1 = _mm_unpackhi_epi32(_4s32, _4s32sign);    \
	_2s64_2 = _mm_unpacklo_epi32(_4s32, _4s32sign);    \
}

/* *********************************************************** */

static void mlib_s_ImageMeanU8Case(
    const mlib_image *src,
    mlib_d64 *mean);

static void mlib_s_ImageMeanS16Case(
    const mlib_image *src,
    mlib_d64 *mean);

static void mlib_s_ImageMeanU16Case(
    const mlib_image *src,
    mlib_d64 *mean);

static void mlib_s_ImageMeanS32Case(
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
		mlib_s_ImageMeanU8Case(src, mean);
		return (MLIB_SUCCESS);

	case MLIB_USHORT:
		mlib_s_ImageMeanU16Case(src, mean);
		return (MLIB_SUCCESS);

/* handle MLIB_SHORT data type of image */
	case MLIB_SHORT:
		mlib_s_ImageMeanS16Case(src, mean);
		return (MLIB_SUCCESS);

/* handle MLIB_INT data type of image */
	case MLIB_INT:
		mlib_s_ImageMeanS32Case(src, mean);
		return (MLIB_SUCCESS);

/* discard any other data types */
	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

#define	MAX_BLOCK_FOR_U8	128

/* *********************************************************** */

void
mlib_s_ImageMeanU8Case(
    const mlib_image *src,
    mlib_d64 *mean)
{
/* pointer for pixel of source */
	mlib_u8 *sp = (mlib_u8 *)mlib_ImageGetData(src);

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

	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < 4; i++)
		rowSum[i] = 0.0;

	if (numOfChannels * srcWidth == srcYStride) {
		srcWidth = srcTotalPixNum;
		srcHeight = 1;
	}

	if (numOfChannels * srcWidth < 16) {
		for (i = 0; i < srcHeight; i++) {
			for (j = 0; j < srcWidth; j++) {
				for (k = 0; k < numOfChannels; k++)
					rowSum[k] += (mlib_s32)sp[i *
					    srcYStride + j * numOfChannels + k];
			}
		}

		for (k = 0; k < numOfChannels; k++)
			mean[k] = rowSum[k] / srcTotalPixNum;

		return;
	}

	switch (numOfChannels) {

	case 1: {
		__m128i *da;
		mlib_d64 divider, sum0;
		mlib_s32 buffer0 = 0;

		for (i = 0; i < srcHeight; i++) {

			mlib_s32 j, db_num = 0;

			for (j = 0;
			    (j < srcWidth) && ((mlib_addr)(sp + j) & 15); j++)
				buffer0 += sp[j];

			db_num = (srcWidth - j) >> 4;
			da = (__m128i *) (sp + j);

			for (; db_num > 0; db_num -= MAX_BLOCK_FOR_U8) {

				__m128i summa0, summa1;

				summa1 = _mm_setzero_si128();

				mlib_s32 size = MAX_BLOCK_FOR_U8;

				if (db_num < MAX_BLOCK_FOR_U8)
					size = db_num;

				j += (size << 4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (k = 0; k < size; k++, da++) {
					__m128i load0, sd1, sd2;

					load0 = _mm_load_si128(da);
					MLIB_S_CONVERT_16U8_8S16(
					    sd1, sd2, load0);
					summa0 = _mm_add_epi16(sd1, sd2);
					summa1 = _mm_add_epi16(summa1, summa0);
				}

				__m128i suma032, suma132, sum_32;
				suma032 = _mm_unpacklo_epi16(summa1, x_zero);
				suma132 = _mm_unpackhi_epi16(summa1, x_zero);
				sum_32 = _mm_add_epi32(suma032, suma132);

				__m128i xsum;
				mlib_s32 *psum = (mlib_s32 *)&xsum;
				_mm_store_si128((void *)psum, sum_32);
				buffer0 += psum[0] + psum[1] +
				    psum[2] + psum[3];
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < srcWidth; j++)
				buffer0 += sp[j];

			sp += srcYStride;
		}

		sum0 = (mlib_d64)buffer0;
		divider = 1.0 / srcTotalPixNum;
		mean[0] = sum0 * divider;

		break;
	} /* case 1 */

	case 2: {
		__m128i *da;
		mlib_d64 divider;
		mlib_d64 sum0, sum1;
		mlib_s32 buffer0 = 0, buffer1 = 0;

		srcWidth <<= 1;

		for (i = 0; i < srcHeight; i++) {
			mlib_s32 db_num = 0;
			mlib_s32 alignment = 1;
			mlib_s32 j = 0;

			if ((mlib_addr)sp & 1) {
				alignment = 0;
				buffer0 += sp[0];

				for (j = 1;
				    (j < (srcWidth - 2)) &&
				    ((mlib_addr)(sp + j) & 15); j += 2) {
					buffer1 += sp[j + 0];
					buffer0 += sp[j + 1];
				}
			} else {
				for (j = 0;
				    (j < srcWidth) &&
				    ((mlib_addr)(sp + j) & 15); j += 2) {
					buffer0 += sp[j + 0];
					buffer1 += sp[j + 1];
				}
			}

			db_num = (srcWidth - j) >> 4;
			da = (__m128i *) (sp + j);

			for (; (db_num > 0); db_num -= MAX_BLOCK_FOR_U8) {
				__m128i summa0, summa1;

				summa1 = _mm_setzero_si128();

				mlib_s32 size = MAX_BLOCK_FOR_U8;

				if (db_num < MAX_BLOCK_FOR_U8)
					size = db_num;

				j += (size << 4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (k = 0; k < size; k++, da++) {
					__m128i load0, sd1, sd2;

					load0 = _mm_load_si128(da);
					MLIB_S_CONVERT_16U8_8S16(sd1, sd2,
					    load0);
					summa0 = _mm_add_epi16(sd1, sd2);
					summa1 = _mm_add_epi16(summa1, summa0);
				}

				__m128i suma032, suma132, sum_32;
				suma032 = _mm_unpackhi_epi16(summa1, x_zero);
				suma132 = _mm_unpacklo_epi16(summa1, x_zero);
				sum_32 = _mm_add_epi32(suma032, suma132);

				__m128i suma064, suma164, sum_64;
				suma064 = _mm_unpackhi_epi32(sum_32, x_zero);
				suma164 = _mm_unpacklo_epi32(sum_32, x_zero);
				sum_64 = _mm_add_epi64(suma064, suma164);

				__m128i xsum;
				mlib_s64 *psum = (mlib_s64 *)&xsum;
				_mm_store_si128((void *)psum, sum_64);

				if (alignment) {
					buffer0 += psum[0];
					buffer1 += psum[1];
				} else {
					buffer1 += psum[0];
					buffer0 += psum[1];
				}
			}

			if (alignment) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < srcWidth; j += 2) {
					buffer0 += sp[j + 0];
					buffer1 += sp[j + 1];
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < (srcWidth - 1); j += 2) {
					buffer1 += sp[j + 0];
					buffer0 += sp[j + 1];
				}

				buffer1 += sp[j];
			}

			sp += srcYStride;
		}

		sum0 = (mlib_d64)buffer0;
		sum1 = (mlib_d64)buffer1;

		divider = 1.0 / srcTotalPixNum;

		mean[0] = sum0 * divider;
		mean[1] = sum1 * divider;

		break;
	} /* case 2 */

	case 3: {
		__m128i s0, s1, s2;
		__m128i *da;

		mlib_d64 divider;
		mlib_d64 sum0, sum1, sum2;
		mlib_s32 buffer0 = 0;
		mlib_s32 buffer1 = 0;
		mlib_s32 buffer2 = 0;
		mlib_s32 buffer3 = 0;
		mlib_s32 buffer4 = 0;
		mlib_s32 buffer5 = 0;

		srcWidth *= 3;

		for (i = 0; i < srcHeight; i++) {
			mlib_s32 db_num = 0;
			mlib_s32 sign = 0;
			mlib_s32 j = 0;
			mlib_s32 alignment = (16 - ((mlib_addr)sp & 15)) & 15;

			for (j = 0;
				(j < srcWidth - 3) && (j <= (alignment - 3));
				j += 3) {
				buffer0 += sp[j + 0];
				buffer1 += sp[j + 1];
				buffer2 += sp[j + 2];
			}

			if ((alignment - j) == 2) {
				buffer0 += sp[j + 0];
				buffer1 += sp[j + 1];
				j += 2;
				sign = 1;
			} else if ((alignment - j) == 1) {
				buffer0 += sp[j];
				j += 1;
				sign = 2;
			}

			db_num = (srcWidth - j) / 48;
			da = (__m128i *)(sp + j);

			for (; db_num > 0; db_num -= MAX_BLOCK_FOR_U8) {
				__m128i summa0210_0, summa1021_0, summa2102_0;
				__m128i summa0210_1, summa1021_1, summa2102_1;

				summa0210_0 = summa1021_0 = summa2102_0 =
				    _mm_setzero_si128();
				summa0210_1 = summa1021_1 = summa2102_1 =
				    _mm_setzero_si128();

				mlib_s32 size = MAX_BLOCK_FOR_U8;

				if (db_num < MAX_BLOCK_FOR_U8)
					size = db_num;

				j += (48 * size);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; size > 0; size--) {
					__m128i load0, load1, load2;
					load0 = _mm_load_si128(da + 0);
					load1 = _mm_load_si128(da + 1);
					load2 = _mm_load_si128(da + 2);

					summa0210_0 =
					    _mm_add_epi16(summa0210_0,
					    _mm_unpacklo_epi8(load0, x_zero));
					summa1021_0 =
					    _mm_add_epi16(summa1021_0,
					    _mm_unpackhi_epi8(load0, x_zero));
					summa2102_0 =
					    _mm_add_epi16(summa2102_0,
					    _mm_unpacklo_epi8(load1, x_zero));
					summa0210_1 =
					    _mm_add_epi16(summa0210_1,
					    _mm_unpackhi_epi8(load1, x_zero));
					summa1021_1 =
					    _mm_add_epi16(summa1021_1,
					    _mm_unpacklo_epi8(load2, x_zero));
					summa2102_1 =
					    _mm_add_epi16(summa2102_1,
					    _mm_unpackhi_epi8(load2, x_zero));

					da += 3;
				}
				summa0210_0 =
				    _mm_add_epi16(summa0210_0, summa0210_1);
				summa2102_0 =
				    _mm_add_epi16(summa2102_0, summa2102_1);
				summa1021_1 =
				    _mm_add_epi16(summa1021_1, summa1021_0);

				__m128i sum00, sum01, sum10;
				__m128i sum11, sum20, sum21;
				sum00 = _mm_unpacklo_epi16(summa0210_0, x_zero);
				sum01 = _mm_unpackhi_epi16(summa0210_0, x_zero);
				sum10 = _mm_unpacklo_epi16(summa1021_1, x_zero);
				sum11 = _mm_unpackhi_epi16(summa1021_1, x_zero);
				sum20 = _mm_unpacklo_epi16(summa2102_0, x_zero);
				sum21 = _mm_unpackhi_epi16(summa2102_0, x_zero);

				sum00 = _mm_add_epi32(sum00, sum11);
				sum10 = _mm_add_epi32(sum10, sum21);
				sum20 = _mm_add_epi32(sum20, sum01);

				__m128i xsum0, xsum1, xsum2;
				mlib_s32 *ps0, *ps1, *ps2;
				ps0 = (mlib_s32 *)&xsum0;
				ps1 = (mlib_s32 *)&xsum1;
				ps2 = (mlib_s32 *)&xsum2;
				_mm_store_si128((void *)ps0, sum00);
				_mm_store_si128((void *)ps1, sum10);
				_mm_store_si128((void *)ps2, sum20);

				buffer3 += ps0[0] + ps0[3] + ps1[1] + ps2[2];
				buffer4 += ps0[1] + ps1[2] + ps2[0] + ps2[3];
				buffer5 += ps0[2] + ps1[0] + ps1[3] + ps2[1];
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < (srcWidth - 2); j += 3) {
				buffer3 += sp[j + 0];
				buffer4 += sp[j + 1];
				buffer5 += sp[j + 2];
			}

			if (sign == 2) {
				buffer1 += sp[j + 0];
				buffer2 += sp[j + 1];

				buffer0 += buffer5;
				buffer1 += buffer3;
				buffer2 += buffer4;
			} else if (sign == 1) {
				buffer2 += sp[j];

				buffer0 += buffer4;
				buffer1 += buffer5;
				buffer2 += buffer3;
			} else if (sign == 0) {
				buffer0 += buffer3;
				buffer1 += buffer4;
				buffer2 += buffer5;
			}

			buffer3 = buffer4 = buffer5 = (mlib_s32)0;
			sp += srcYStride;
		}

		sum0 = (mlib_d64)buffer0;
		sum1 = (mlib_d64)buffer1;
		sum2 = (mlib_d64)buffer2;

		divider = 1.0 / srcTotalPixNum;

		mean[0] = sum0 * divider;
		mean[1] = sum1 * divider;
		mean[2] = sum2 * divider;

		break;
	} /* case 3 */

	case 4: {
		__m128i *da;
		mlib_d64 divider, sum0, sum1, sum2, sum3;
		mlib_s32 buffer0 = 0;
		mlib_s32 buffer1 = 0;
		mlib_s32 buffer2 = 0;
		mlib_s32 buffer3 = 0;
		mlib_s32 buffer4 = 0;
		mlib_s32 buffer5 = 0;
		mlib_s32 buffer6 = 0;
		mlib_s32 buffer7 = 0;

		srcWidth <<= 2;

		for (i = 0; i < srcHeight; i++) {
			mlib_s32 db_num = 0;
			mlib_s32 sign = 0;
			mlib_s32 j = 0;
			mlib_s32 alignment = (16 - ((mlib_addr)sp & 15)) & 15;

			for (; (j < (srcWidth - 3)) && (alignment - j >= 4);
			    j += 4) {
				buffer0 += sp[j + 0];
				buffer1 += sp[j + 1];
				buffer2 += sp[j + 2];
				buffer3 += sp[j + 3];
			}

			if ((j < (srcWidth - 3)) && ((alignment - j) == 3)) {
				buffer0 += sp[j + 0];
				buffer1 += sp[j + 1];
				buffer2 += sp[j + 2];
				j += 3;
				sign = 1;
			} else if ((j < (srcWidth - 3)) &&
				((alignment - j) == 2)) {
				buffer0 += sp[j + 0];
				buffer1 += sp[j + 1];
				j += 2;
				sign = 2;
			} else if ((j < (srcWidth - 3)) &&
				((alignment - j) == 1)) {
				buffer0 += sp[j + 0];
				j += 1;
				sign = 3;
			}

			db_num = (srcWidth - j) >> 4;
			da = (__m128i *) (sp + j);

			for (; (db_num > 0); db_num -= MAX_BLOCK_FOR_U8) {
				__m128i summa0, summa1;

				summa1 = _mm_setzero_si128();

				mlib_s32 size = MAX_BLOCK_FOR_U8;

				if (db_num < MAX_BLOCK_FOR_U8)
					size = db_num;

				j += (size << 4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (k = 0; k < size; k++, da++) {
					__m128i load0, sd1, sd2;

					load0 = _mm_load_si128(da);
					MLIB_S_CONVERT_16U8_8S16(sd1, sd2,
					    load0);
					summa0 = _mm_add_epi16(sd1, sd2);
					summa1 = _mm_add_epi16(summa1, summa0);
				}

				__m128i suma032, suma132, sum_32;
				suma132 = _mm_unpacklo_epi16(summa1, x_zero);
				suma032 = _mm_unpackhi_epi16(summa1, x_zero);
				sum_32 = _mm_add_epi32(suma032, suma132);

				__m128i xsum;
				mlib_s32 *psum = (mlib_s32 *)&xsum;
				_mm_store_si128((void *)psum, sum_32);
				buffer4 += psum[0];
				buffer5 += psum[1];
				buffer6 += psum[2];
				buffer7 += psum[3];
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < (srcWidth - 3); j += 4) {
				buffer4 += sp[j + 0];
				buffer5 += sp[j + 1];
				buffer6 += sp[j + 2];
				buffer7 += sp[j + 3];
			}

			if (sign == 3) {
				buffer1 += sp[j + 0];
				buffer2 += sp[j + 1];
				buffer3 += sp[j + 2];

				buffer0 += buffer7;
				buffer1 += buffer4;
				buffer2 += buffer5;
				buffer3 += buffer6;
			} else if (sign == 2) {
				buffer2 += sp[j + 0];
				buffer3 += sp[j + 1];

				buffer0 += buffer6;
				buffer1 += buffer7;
				buffer2 += buffer4;
				buffer3 += buffer5;
			} else if (sign == 1) {
				buffer3 += sp[j];

				buffer0 += buffer5;
				buffer1 += buffer6;
				buffer2 += buffer7;
				buffer3 += buffer4;
			} else if (sign == 0) {
				buffer0 += buffer4;
				buffer1 += buffer5;
				buffer2 += buffer6;
				buffer3 += buffer7;
			}

			sp += srcYStride;
			buffer4 = buffer5 = buffer6 = buffer7 = (mlib_s32)0;
		}

		sum0 = (mlib_d64)buffer0;
		sum1 = (mlib_d64)buffer1;
		sum2 = (mlib_d64)buffer2;
		sum3 = (mlib_d64)buffer3;

		divider = 1.0 / srcTotalPixNum;

		mean[0] = sum0 * divider;
		mean[1] = sum1 * divider;
		mean[2] = sum2 * divider;
		mean[3] = sum3 * divider;

		break;
		} /* case 4 */
	} /* switch (numOfChannels) */
}

/* *********************************************************** */

#define	MAX_BLOCK_FOR_S16	32768

/* *********************************************************** */

void
mlib_s_ImageMeanS16Case(
    const mlib_image *src,
    mlib_d64 *mean)
{
/* pointer for pixel of source */
	mlib_s16 *sa = (mlib_s16 *)mlib_ImageGetData(src);

/* width of source image */
	mlib_s32 width = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 height = mlib_ImageGetHeight(src);

/* total number of pixels */
	mlib_s32 npixels = height * width;

/* linestride = bytes to next row */
	mlib_s32 stride = mlib_ImageGetStride(src);

/* number of channels */
	mlib_s32 channels = mlib_ImageGetChannels(src);

/* indices */
	mlib_s32 i, x;

	const __m128i x_zero = _mm_setzero_si128();

	if (2 * channels * width == stride) {
		width = npixels;
		height = 1;
	}

	switch (channels) {

	case 1: {
		mlib_d64 divider;
		mlib_d64 sum0;
		mlib_s64 buffer0 = 0;

		for (i = 0; i < height; i++) {
			mlib_s32 db_num = 0;
			mlib_s32 j = 0;
			__m128i *sl;

			for (j = 0; (j < width) &&
			    ((mlib_addr)((mlib_s16 *)sa + j) & 15); j++)
				buffer0 += sa[j];

			db_num = (width - j) >> 3;
			sl = (__m128i *) ((mlib_s16 *)sa + j);

			for (; db_num > 0; db_num -= MAX_BLOCK_FOR_S16) {
				__m128i summa0, summa1;

				summa0 = summa1 = _mm_setzero_si128();
				mlib_s32 size = MAX_BLOCK_FOR_S16;

				if (db_num < MAX_BLOCK_FOR_S16)
					size = db_num;

				j += (size << 3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (x = 0; x < size; x++, sl++) {
					__m128i load0, data32_1, data32_2;
					load0 = _mm_load_si128(sl);
					MLIB_S_CONVERT_4S16_2S32(data32_1,
					    data32_2, load0);
					summa0 =
					    _mm_add_epi32(summa0, data32_1);
					summa1 =
					    _mm_add_epi32(summa1, data32_2);
				}

				summa0 = _mm_add_epi32(summa0, summa1);

				__m128i xsum0;
				mlib_s32 *psum0 = (mlib_s32 *)&xsum0;
				_mm_store_si128((void *)psum0, summa0);
				buffer0 += psum0[0] + psum0[2] +
				    psum0[1] + psum0[3];
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < width; j++)
				buffer0 += sa[j];

			sa += (stride >> 1);
		}

		sum0 = (mlib_d64)buffer0;
		divider = 1.0 / npixels;
		mean[0] = sum0 * divider;
		break;
	} /* case 1 */

	case 2: {
		__m128i *sl;
		mlib_d64 divider;
		mlib_d64 sum0, sum1;
		mlib_s64 buffer0 = 0;
		mlib_s64 buffer1 = 0;
		mlib_s64 buffer2 = 0;
		mlib_s64 buffer3 = 0;

/*   ImageWidth * ChannelNumber   */
		width <<= 1;

		for (i = 0; i < height; i++) {
			mlib_s32 db_num = 0;
			mlib_s32 alignment = 1;
			mlib_s32 j = 0;

			if ((mlib_addr)sa & 1) {
				alignment = 0;
				buffer0 += sa[0];

				for (j = 1;
				    (j < (width - 2)) &&
				    ((mlib_addr)(sa + j) & 15); j += 2) {
					buffer1 += sa[j + 0];
					buffer0 += sa[j + 1];
				}
			} else {
				for (j = 0;
				    (j < width) &&
				    ((mlib_addr)(sa + j) & 15); j += 2) {
					buffer0 += sa[j + 0];
					buffer1 += sa[j + 1];
				}
			}

			db_num = (width - j) >> 3;
			sl = (__m128i *) (sa + j);

			for (; (db_num > 0); db_num -= MAX_BLOCK_FOR_S16) {
				__m128i summa0, summa1;

				summa0 = summa1 = _mm_setzero_si128();
				mlib_s32 size = MAX_BLOCK_FOR_S16;

				if (db_num < MAX_BLOCK_FOR_S16)
					size = db_num;

				j += (size << 3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (x = 0; x < size; x++, sl++) {
					__m128i load0, data32_1, data32_2;
					load0 = _mm_load_si128(sl);
					MLIB_S_CONVERT_4S16_2S32(data32_1,
					    data32_2, load0);
					summa0 =
					    _mm_add_epi32(summa0, data32_1);
					summa1 =
					    _mm_add_epi32(summa1, data32_2);
				}

				summa0 = _mm_add_epi32(summa0, summa1);

				__m128i xsum0;
				mlib_s32 *psum0 = (mlib_s32 *)&xsum0;
				_mm_storeu_si128((void *)psum0, summa0);
				buffer2 += psum0[0] + psum0[2];
				buffer3 += psum0[1] + psum0[3];
			}

			if (alignment) {
				buffer0 += buffer2;
				buffer1 += buffer3;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < width; j += 2) {
					buffer0 += sa[j + 0];
					buffer1 += sa[j + 1];
				}
			} else {
				buffer1 += buffer2;
				buffer0 += buffer3;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < (width - 1); j += 2) {
					buffer1 += sa[j + 0];
					buffer0 += sa[j + 1];
				}

				buffer1 += sa[j];
			}

			sa += (stride >> 1);
			buffer2 = buffer3 = 0;
		}

		sum0 = (mlib_d64)buffer0;
		sum1 = (mlib_d64)buffer1;
		divider = 1.0 / npixels;
		mean[0] = sum0 * divider;
		mean[1] = sum1 * divider;
		break;
	} /* case 2 */

	case 3: {
		__m128i *sl;
		mlib_d64 divider, sum0, sum1, sum2;

		mlib_s64 buffer0 = 0;
		mlib_s64 buffer1 = 0;
		mlib_s64 buffer2 = 0;
		mlib_s64 buffer3 = 0;
		mlib_s64 buffer4 = 0;
		mlib_s64 buffer5 = 0;

		width *= 3;

		for (i = 0; i < height; i++) {
			mlib_s32 db_num_12 = 0;
			mlib_s32 sign = 0;
			mlib_s32 j = 0;
			mlib_s32 alignment =
			    ((16 - ((mlib_addr)sa & 15)) & 15) >> 1;

			for (j = 0;
			    (j < (width - 3)) && (j <= (alignment - 3));
			    j += 3) {
				buffer0 += sa[j + 0];
				buffer1 += sa[j + 1];
				buffer2 += sa[j + 2];
			}

			if ((alignment - j) == 2) {
				buffer0 += sa[j + 0];
				buffer1 += sa[j + 1];
				j += 2;
				sign = 1;
			} else if ((alignment - j) == 1) {
				buffer0 += sa[j];
				j += 1;
				sign = 2;
			}

			db_num_12 = ((width - j) >> 3);
			sl = (__m128i *) (sa + j);

			for (; (db_num_12 > 0);
			    db_num_12 -= (MAX_BLOCK_FOR_S16 * 3)) {

				__m128i summa10, summa02, summa21;
				summa10 = summa02 = summa21 =
				    _mm_setzero_si128();

				mlib_s32 size = (MAX_BLOCK_FOR_S16 * 3);

				if (db_num_12 < (MAX_BLOCK_FOR_S16 * 3))
					size = db_num_12;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (x = 0; x < (size - 2);
				    x += 3, j += 24, sl += 3) {
					__m128i load0, load1, load2;
					__m128i s0, s1, s2, s3, s4, s5;

					load0 = _mm_load_si128(sl);
					load1 = _mm_load_si128(sl + 1);
					load2 = _mm_load_si128(sl + 2);
					MLIB_S_CONVERT_4S16_2S32(s1, s0,
					    load0);
					MLIB_S_CONVERT_4S16_2S32(s3, s2,
					    load1);
					MLIB_S_CONVERT_4S16_2S32(s5, s4,
					    load2);
					summa10 =
					    _mm_add_epi32(summa10, s0);
					summa02 =
					    _mm_add_epi32(summa02, s1);
					summa21 =
					    _mm_add_epi32(summa21, s2);
					summa10 =
					    _mm_add_epi32(summa10, s3);
					summa02 =
					    _mm_add_epi32(summa02, s4);
					summa21 =
					    _mm_add_epi32(summa21, s5);
				}

				__m128i xsum0, xsum1, xsum2;
				mlib_s32 *psum0, *psum1, *psum2;
				psum0 = (mlib_s32 *)&xsum0;
				psum1 = (mlib_s32 *)&xsum1;
				psum2 = (mlib_s32 *)&xsum2;
				_mm_store_si128((void *)psum0, summa10);
				_mm_store_si128((void *)psum1, summa02);
				_mm_store_si128((void *)psum2, summa21);

				buffer3 += psum0[0] + psum0[3] +
				    psum1[2] + psum2[1];
				buffer4 += psum0[1] + psum1[0] +
				    psum1[3] + psum2[2];
				buffer5 += psum0[2] + psum1[1] +
				    psum2[0] + psum2[3];
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < (width - 2); j += 3) {
				buffer3 += sa[j + 0];
				buffer4 += sa[j + 1];
				buffer5 += sa[j + 2];
			}

			if (sign == 2) {
				buffer1 += sa[j + 0];
				buffer2 += sa[j + 1];
				buffer0 += buffer5;
				buffer1 += buffer3;
				buffer2 += buffer4;
			} else if (sign == 1) {
				buffer2 += sa[j];
				buffer0 += buffer4;
				buffer1 += buffer5;
				buffer2 += buffer3;
			} else if (sign == 0) {
				buffer0 += buffer3;
				buffer1 += buffer4;
				buffer2 += buffer5;
			}

			buffer3 = buffer4 = buffer5 = (mlib_s64)0;
			sa += (stride >> 1);
		}

		sum0 = (mlib_d64)buffer0;
		sum1 = (mlib_d64)buffer1;
		sum2 = (mlib_d64)buffer2;
		divider = 1.0 / npixels;
		mean[0] = sum0 * divider;
		mean[1] = sum1 * divider;
		mean[2] = sum2 * divider;
		break;
	} /* case 3 */

	case 4: {
		__m128i *sl;
		mlib_d64 divider, sum0, sum1, sum2, sum3;
		mlib_s64 buffer0 = 0;
		mlib_s64 buffer1 = 0;
		mlib_s64 buffer2 = 0;
		mlib_s64 buffer3 = 0;

		width <<= 2;

		for (i = 0; i < height; i++) {
			mlib_s32 db_num = 0;
			mlib_s32 j = 0;

			db_num = width >> 3;
			sl = (__m128i *) (sa);

			for (; (db_num > 0); db_num -= (MAX_BLOCK_FOR_S16)) {
				__m128i summa0, summa1;
				mlib_s32 size = MAX_BLOCK_FOR_S16;

				if (db_num < MAX_BLOCK_FOR_S16)
					size = db_num;

				j += (size << 3);

				summa0 = summa1 = _mm_setzero_si128();

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (x = 0; x < size; x++) {
					__m128i xsd0, data32_1, data32_2;

					xsd0 = _mm_loadu_si128(sl);
					MLIB_S_CONVERT_4S16_2S32(
					    data32_1, data32_2, xsd0);
					summa0 = _mm_add_epi32(
					    summa0, data32_1);
					summa1 = _mm_add_epi32(
					    summa1, data32_2);
					sl++;
				}

				summa0 = _mm_add_epi32(summa0, summa1);

				__m128i xsum;
				mlib_s32 *psum = (mlib_s32 *)&xsum;
				_mm_store_si128((void *)psum, summa0);
				buffer0 += psum[0];
				buffer1 += psum[1];
				buffer2 += psum[2];
				buffer3 += psum[3];
			}

			if (j < (width - 3)) {
				buffer0 += sa[j + 0];
				buffer1 += sa[j + 1];
				buffer2 += sa[j + 2];
				buffer3 += sa[j + 3];
			}

			sa += (stride >> 1);
		}

		sum0 = (mlib_d64)buffer0;
		sum1 = (mlib_d64)buffer1;
		sum2 = (mlib_d64)buffer2;
		sum3 = (mlib_d64)buffer3;

		divider = 1.0 / npixels;

		mean[0] = sum0 * divider;
		mean[1] = sum1 * divider;
		mean[2] = sum2 * divider;
		mean[3] = sum3 * divider;
		break;
	} /* case 4 */
	}
}

/* *********************************************************** */

#define	MAX_BLOCK_FOR_U16	4096

/* *********************************************************** */

void
mlib_s_ImageMeanU16Case(
    const mlib_image *src,
    mlib_d64 *mean)
{
/* pointer for pixel of source */
	mlib_u16 *sa = (mlib_u16 *)mlib_ImageGetData(src);

/* width of source image */
	mlib_s32 width = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 height = mlib_ImageGetHeight(src);

/* total number of pixels */
	mlib_s32 npixels = height * width;

/* linestride = bytes to next row */
	mlib_s32 stride = mlib_ImageGetStride(src);

/* number of channels */
	mlib_s32 channels = mlib_ImageGetChannels(src);

/* indices */
	mlib_s32 i, x;

	const __m128i x_zero = _mm_setzero_si128();

	if (2 * channels * width == stride) {
		width = npixels;
		height = 1;
	}

	switch (channels) {

	case 1: {
		mlib_d64 divider;
		mlib_d64 sum0;
		mlib_s64 buffer0 = 0;

		for (i = 0; i < height; i++) {
			mlib_s32 db_num = 0;
			mlib_s32 j = 0;
			__m128i *sl;

			for (j = 0;
			    (j < width) &&
			    ((mlib_addr)((mlib_u16 *)sa + j) & 15); j++)
				buffer0 += sa[j];

			db_num = (width - j) >> 3;
			sl = (__m128i *) ((mlib_u16 *)sa + j);

			for (; db_num > 0; db_num -= MAX_BLOCK_FOR_U16) {
				__m128i summa0, summa1;
				summa0 = summa1 = _mm_setzero_si128();

				mlib_s32 size = MAX_BLOCK_FOR_U16;

				if (db_num < MAX_BLOCK_FOR_U16)
					size = db_num;

				j += (size << 3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (x = 0; x < size; x++, sl++) {
					__m128i load0, data32_1, data32_2;
					load0 = _mm_load_si128(sl);
					MLIB_S_CONVERT_4U16_2S32(data32_1,
					    data32_2, load0);
					summa0 =
					    _mm_add_epi32(summa0, data32_1);
					summa1 =
					    _mm_add_epi32(summa1, data32_2);
				}

				summa0 = _mm_add_epi32(summa0, summa1);

				__m128i xsum0;
				mlib_s32 *psum0 = (mlib_s32 *)&xsum0;
				_mm_store_si128((void *)psum0, summa0);
				buffer0 += psum0[0] + psum0[2] +
				    psum0[1] + psum0[3];
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < width; j++)
				buffer0 += sa[j];

			sa += (stride >> 1);
		}

		sum0 = (mlib_d64)buffer0;
		divider = 1.0 / npixels;
		mean[0] = sum0 * divider;
		break;
	} /* case 1 */

	case 2: {
		__m128i *sl;

		mlib_d64 divider;
		mlib_d64 sum0, sum1;
		mlib_s64 buffer0 = 0;
		mlib_s64 buffer1 = 0;
		mlib_s64 buffer2 = 0;
		mlib_s64 buffer3 = 0;

/*   ImageWidth * ChannelNumber   */
		width <<= 1;

		for (i = 0; i < height; i++) {
			mlib_s32 db_num = 0;
			mlib_s32 alignment = 1;
			mlib_s32 j = 0;

			if ((mlib_addr)sa & 1) {
				alignment = 0;
				buffer0 += sa[0];

				for (j = 1; (j < (width - 2)) &&
				    ((mlib_addr)(sa + j) & 15); j += 2) {
					buffer1 += sa[j + 0];
					buffer0 += sa[j + 1];
				}

			} else {

				for (j = 0; (j < width) &&
				    ((mlib_addr)(sa + j) & 15); j += 2) {
					buffer0 += sa[j + 0];
					buffer1 += sa[j + 1];
				}
			}

			db_num = (width - j) >> 3;
			sl = (__m128i *)(sa + j);

			for (; (db_num > 0); db_num -= MAX_BLOCK_FOR_U16) {

				__m128i summa0, summa1;

				summa0 = summa1 = _mm_setzero_si128();
				mlib_s32 size = MAX_BLOCK_FOR_U16;

				if (db_num < MAX_BLOCK_FOR_U16)
					size = db_num;

				j += (size << 3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (x = 0; x < size; x++, sl++) {
					__m128i load0, data32_1, data32_2;
					load0 = _mm_load_si128(sl);
					MLIB_S_CONVERT_4U16_2S32(data32_1,
					    data32_2, load0);
					summa0 =
					    _mm_add_epi32(summa0, data32_1);
					summa1 =
					    _mm_add_epi32(summa1, data32_2);
				}

				summa0 = _mm_add_epi32(summa0, summa1);

				__m128i xsum0;
				mlib_s32 *psum0 = (mlib_s32 *)&xsum0;
				_mm_storeu_si128((void *)psum0, summa0);
				buffer2 += psum0[0] + psum0[2];
				buffer3 += psum0[1] + psum0[3];
			}

			if (alignment) {
				buffer0 += buffer2;
				buffer1 += buffer3;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < width; j += 2) {
					buffer0 += sa[j + 0];
					buffer1 += sa[j + 1];
				}
			} else {
				buffer1 += buffer2;
				buffer0 += buffer3;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < (width - 1); j += 2) {
					buffer1 += sa[j + 0];
					buffer0 += sa[j + 1];
				}

				buffer1 += sa[j];
			}

			sa += (stride >> 1);
			buffer2 = buffer3 = 0;
		}

		sum0 = (mlib_d64)buffer0;
		sum1 = (mlib_d64)buffer1;
		divider = 1.0 / npixels;
		mean[0] = sum0 * divider;
		mean[1] = sum1 * divider;
		break;
	} /* case 2 */

	case 3: {
		__m128i *sl;

		mlib_d64 divider, sum0, sum1, sum2;
		mlib_s64 buffer0 = 0;
		mlib_s64 buffer1 = 0;
		mlib_s64 buffer2 = 0;
		mlib_s64 buffer3 = 0;
		mlib_s64 buffer4 = 0;
		mlib_s64 buffer5 = 0;

		width *= 3;

		for (i = 0; i < height; i++) {
			mlib_s32 db_num_12 = 0;
			mlib_s32 sign = 0;
			mlib_s32 j = 0;
			mlib_s32 alignment =
			    ((16 - ((mlib_addr)sa & 15)) & 15) >> 1;

			for (j = 0;
			    (j < (width - 3)) && (j <= (alignment - 3));
			    j += 3) {
				buffer0 += sa[j + 0];
				buffer1 += sa[j + 1];
				buffer2 += sa[j + 2];
			}

			if ((alignment - j) == 2) {
				buffer0 += sa[j + 0];
				buffer1 += sa[j + 1];
				j += 2;
				sign = 1;
			} else if ((alignment - j) == 1) {
				buffer0 += sa[j];
				j += 1;
				sign = 2;
			}

			db_num_12 = ((width - j) >> 3);
			sl = (__m128i *) (sa + j);

			for (; (db_num_12 > 0);
			    db_num_12 -= (MAX_BLOCK_FOR_U16 * 3)) {

				__m128i summa10, summa02, summa21;

				summa10 = summa02 = summa21 =
				    _mm_setzero_si128();
				mlib_s32 size = (MAX_BLOCK_FOR_U16 * 3);

				if (db_num_12 < (MAX_BLOCK_FOR_U16 * 3))
					size = db_num_12;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (x = 0; x < (size - 2);
				    x += 3, j += 24, sl += 3) {
					__m128i load0, load1, load2;
					__m128i s0, s1, s2, s3, s4, s5;
					load0 = _mm_load_si128(sl);
					load1 = _mm_load_si128(sl + 1);
					load2 = _mm_load_si128(sl + 2);
					MLIB_S_CONVERT_4U16_2S32(s1, s0,
					    load0);
					MLIB_S_CONVERT_4U16_2S32(s3, s2,
					    load1);
					MLIB_S_CONVERT_4U16_2S32(s5, s4,
					    load2);
					summa10 =
					    _mm_add_epi32(summa10, s0);
					summa02 =
					    _mm_add_epi32(summa02, s1);
					summa21 =
					    _mm_add_epi32(summa21, s2);
					summa10 =
					    _mm_add_epi32(summa10, s3);
					summa02 =
					    _mm_add_epi32(summa02, s4);
					summa21 =
					    _mm_add_epi32(summa21, s5);
				}

				__m128i xsum0, xsum1, xsum2;
				mlib_s32 *psum0, *psum1, *psum2;
				psum0 = (mlib_s32 *)&xsum0;
				psum1 = (mlib_s32 *)&xsum1;
				psum2 = (mlib_s32 *)&xsum2;
				_mm_store_si128((void *)psum0, summa10);
				_mm_store_si128((void *)psum1, summa02);
				_mm_store_si128((void *)psum2, summa21);

				buffer3 += psum0[0] + psum0[3] +
				    psum1[2] + psum2[1];
				buffer4 += psum0[1] + psum1[0] +
				    psum1[3] + psum2[2];
				buffer5 += psum0[2] + psum1[1] +
				    psum2[0] + psum2[3];
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < (width - 2); j += 3) {
				buffer3 += sa[j + 0];
				buffer4 += sa[j + 1];
				buffer5 += sa[j + 2];
			}

			if (sign == 2) {
				buffer1 += sa[j + 0];
				buffer2 += sa[j + 1];
				buffer0 += buffer5;
				buffer1 += buffer3;
				buffer2 += buffer4;
			} else if (sign == 1) {
				buffer2 += sa[j];
				buffer0 += buffer4;
				buffer1 += buffer5;
				buffer2 += buffer3;
			} else if (sign == 0) {
				buffer0 += buffer3;
				buffer1 += buffer4;
				buffer2 += buffer5;
			}

			buffer3 = buffer4 = buffer5 = (mlib_s64)0;
			sa += (stride >> 1);
		}

		sum0 = (mlib_d64)buffer0;
		sum1 = (mlib_d64)buffer1;
		sum2 = (mlib_d64)buffer2;
		divider = 1.0 / npixels;
		mean[0] = sum0 * divider;
		mean[1] = sum1 * divider;
		mean[2] = sum2 * divider;
		break;
	} /* case 3 */

	case 4: {
		__m128i *sl;
		mlib_d64 divider, sum0, sum1, sum2, sum3;
		mlib_s64 buffer0 = 0;
		mlib_s64 buffer1 = 0;
		mlib_s64 buffer2 = 0;
		mlib_s64 buffer3 = 0;
		mlib_s64 buffer4 = 0;
		mlib_s64 buffer5 = 0;
		mlib_s64 buffer6 = 0;
		mlib_s64 buffer7 = 0;

		width <<= 2;

		for (i = 0; i < height; i++) {
			mlib_s32 db_num = 0;
			mlib_s32 sign = 0;
			mlib_s32 j = 0, l;
			mlib_s32 alignment =
			    ((16 - ((mlib_addr)sa & 15)) & 15) >> 1;

			for (; (j < width - 3) && (alignment >= 4);
			    j += 4) {
				buffer0 += sa[0];
				buffer1 += sa[1];
				buffer2 += sa[2];
				buffer3 += sa[3];
				alignment -= 4;
			}

			if (j < (width - 3)) {
				if ((alignment == 3)) {
					buffer0 += sa[j + 0];
					buffer1 += sa[j + 1];
					buffer2 += sa[j + 2];
					j += 3;
					sign = 1;
				} else if ((alignment == 2)) {
					buffer0 += sa[j + 0];
					buffer1 += sa[j + 1];
					j += 2;
					sign = 2;
				} else if ((alignment == 1)) {
					buffer0 += sa[j + 0];
					j += 1;
					sign = 3;
				}

				db_num = ((width - j) >> 3);
				sl = (__m128i *) (sa + j);

				for (; (db_num > 0);
					db_num -= (MAX_BLOCK_FOR_U16)) {
					__m128i summa0, summa1;

					mlib_s32 size = MAX_BLOCK_FOR_U16;

					if (db_num < MAX_BLOCK_FOR_U16)
						size = db_num;
					j += (size << 3);

					summa0 = summa1 =
					    _mm_setzero_si128();

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (x = 0; x < size; x++) {
						__m128i load0;
						__m128i data32_1, data32_2;

						load0 = _mm_load_si128(sl);
						MLIB_S_CONVERT_4U16_2S32(
						    data32_1, data32_2, load0);
						summa0 = _mm_add_epi32(
						    summa0, data32_1);
						summa1 = _mm_add_epi32(
						    summa1, data32_2);
						sl++;
					}

					summa0 = _mm_add_epi32(
					    summa0, summa1);
					__m128i xsum0;
					mlib_s32 * psum0 = (mlib_s32 *)&xsum0;
					_mm_store_si128((void *)psum0, summa0);
					buffer4 += psum0[0];
					buffer5 += psum0[1];
					buffer6 += psum0[2];
					buffer7 += psum0[3];
				}
			}

			if (j < (width - 3)) {
				buffer4 += sa[j + 0];
				buffer5 += sa[j + 1];
				buffer6 += sa[j + 2];
				buffer7 += sa[j + 3];
				j += 4;
			}

			if (sign == 3) {
				buffer1 += sa[j + 0];
				buffer2 += sa[j + 1];
				buffer3 += sa[j + 2];

				buffer0 += buffer7;
				buffer1 += buffer4;
				buffer2 += buffer5;
				buffer3 += buffer6;
			} else if (sign == 2) {
				buffer2 += sa[j + 0];
				buffer3 += sa[j + 1];

				buffer0 += buffer6;
				buffer1 += buffer7;
				buffer2 += buffer4;
				buffer3 += buffer5;
			} else if (sign == 1) {
				buffer3 += sa[j];

				buffer0 += buffer5;
				buffer1 += buffer6;
				buffer2 += buffer7;
				buffer3 += buffer4;
			} else if (sign == 0) {
				buffer0 += buffer4;
				buffer1 += buffer5;
				buffer2 += buffer6;
				buffer3 += buffer7;
			}

			sa += (stride >> 1);
			buffer4 = buffer5 = buffer6 = buffer7 = 0;
		}

		sum0 = (mlib_d64)buffer0;
		sum1 = (mlib_d64)buffer1;
		sum2 = (mlib_d64)buffer2;
		sum3 = (mlib_d64)buffer3;

		divider = 1.0 / npixels;

		mean[0] = sum0 * divider;
		mean[1] = sum1 * divider;
		mean[2] = sum2 * divider;
		mean[3] = sum3 * divider;
		break;
	} /* case 4 */
	} /* switch (channels) */
}

/* *********************************************************** */

#define	MAX_BLOCK_FOR_S32	32768

/* *********************************************************** */

void
mlib_s_ImageMeanS32Case(
    const mlib_image *src,
    mlib_d64 *mean)
{
/* pointer for pixel of source */
	mlib_s32 *sa = (mlib_s32 *)mlib_ImageGetData(src);

/* width of source image */
	mlib_s32 width = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 height = mlib_ImageGetHeight(src);

/* total number of pixels */
	mlib_s32 npixels = height * width;

/* linestride = bytes to next row */
	mlib_s32 stride = mlib_ImageGetStride(src);

/* number of channels */
	mlib_s32 channels = mlib_ImageGetChannels(src);

/* indices */
	mlib_s32 i, x;

	const __m128i x_zero = _mm_setzero_si128();

	if (4 * channels * width == stride) {
		width = npixels;
		height = 1;
	}

	switch (channels) {

	case 1: {
		mlib_d64 buffer0 = 0.0;

		for (i = 0; i < height; i++) {
			mlib_s32 j = 0;
			__m128i *sl;

			__m128d summa0, summa1;
			summa0 = summa1 = _mm_setzero_pd();

			for (j = 0; (j < width) &&
			    ((mlib_addr)((mlib_s32 *)sa + j) & 15); j++) {
				buffer0 += sa[j];
			}

			sl = (__m128i *) ((mlib_s32 *)sa + j);

			mlib_s32 size = (width - j) >> 2;
			j += (size << 2);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (x = 0; x < size; x++, sl++) {
				__m128i load0;
				__m128d data32_1, data32_2;
				load0 = _mm_load_si128(sl);
				data32_1 = _mm_cvtepi32_pd(load0);
				data32_2 = _mm_cvtepi32_pd(
				    _mm_srli_si128(load0, 8));
				summa0 =
				    _mm_add_pd(summa0, data32_1);
				summa1 =
				    _mm_add_pd(summa1, data32_2);
			}

			summa0 = _mm_add_pd(summa0, summa1);
			__m128d xsum;
			mlib_d64 *psum = (mlib_d64 *)&xsum;
			_mm_store_pd(psum, summa0);
			buffer0 += psum[0] + psum[1];

			for (; j < width; j++) {
				buffer0 += sa[j];
			}

			sa = (mlib_s32 *)((mlib_u8 *)sa + stride);
		}


		mlib_d64 divider = 1.0 / npixels;
		mean[0] = buffer0 * divider;
		break;
	} /* case 1 */

	case 2: {
		__m128i *sl;
		mlib_d64 buffer0 = 0.0;
		mlib_d64 buffer1 = 0.0;

/*   ImageWidth * ChannelNumber   */
		width <<= 1;

		for (i = 0; i < height; i++) {
			mlib_s32 alignment = 1;
			mlib_s32 j = 0;

			if ((mlib_addr)sa & 1) {
				alignment = 0;
				buffer0 += sa[0];

				for (j = 1;	(j < (width - 2)) &&
				    ((mlib_addr)(sa + j) & 15); j += 2) {
					buffer1 += sa[j + 0];
					buffer0 += sa[j + 1];
				}
			} else {
				for (j = 0;	(j < width) &&
				    ((mlib_addr)(sa + j) & 15); j += 2) {
					buffer0 += sa[j + 0];
					buffer1 += sa[j + 1];
				}
			}

			mlib_s32 size = (width - j) >> 2;
			sl = (__m128i *) (sa + j);
			j += (size << 2);

			__m128d summa0, summa1;
			summa0 = summa1 = _mm_setzero_pd();

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (x = 0; x < size; x++, sl++) {
				__m128i load0;
				__m128d data32_1, data32_2;
				load0 = _mm_load_si128(sl);
				data32_1 = _mm_cvtepi32_pd(load0);
				data32_2 = _mm_cvtepi32_pd(
				    _mm_srli_si128(load0, 8));
				summa0 =
				    _mm_add_pd(summa0, data32_1);
				summa1 =
				    _mm_add_pd(summa1, data32_2);
			}

			summa0 = _mm_add_pd(summa0, summa1);
			mlib_d64 buffer2, buffer3;
			_mm_storel_pd(&buffer2, summa0);
			_mm_storeh_pd(&buffer3, summa0);

			if (alignment) {
				buffer0 += buffer2;
				buffer1 += buffer3;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < width; j += 2) {
					buffer0 += sa[j + 0];
					buffer1 += sa[j + 1];
				}
			} else {
				buffer1 += buffer2;
				buffer0 += buffer3;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < (width - 1); j += 2) {
					buffer1 += sa[j + 0];
					buffer0 += sa[j + 1];
				}

				buffer1 += sa[j];
			}

			sa += (stride >> 2);
		}

		mlib_d64 divider = 1.0 / npixels;
		mean[0] = buffer0 * divider;
		mean[1] = buffer1 * divider;
		break;
	} /* case 2 */

	case 3: {
		__m128i *sl;
		mlib_d64 sum0, sum1, sum2;
		mlib_s64 buffer0 = 0;
		mlib_s64 buffer1 = 0;
		mlib_s64 buffer2 = 0;
		mlib_s64 buffer3 = 0;
		mlib_s64 buffer4 = 0;
		mlib_s64 buffer5 = 0;

		width *= 3;

		for (i = 0; i < height; i++) {
			mlib_s32 db_num_12 = 0;
			mlib_s32 sign = 0;
			mlib_s32 j = 0;
			mlib_s32 alignment =
			    ((16 - ((mlib_addr)sa & 15)) & 15) >> 2;

			for (j = 0; (j < (width - 3)) &&
			    (j <= (alignment - 3));	j += 3) {
				buffer0 += sa[j + 0];
				buffer1 += sa[j + 1];
				buffer2 += sa[j + 2];
			}

			if ((alignment - j) == 2) {
				buffer0 += sa[j + 0];
				buffer1 += sa[j + 1];
				j += 2;
				sign = 1;
			} else if ((alignment - j) == 1) {
				buffer0 += sa[j];
				j += 1;
				sign = 2;
			}

			db_num_12 = ((width - j) >> 2);
			sl = (__m128i *) (sa + j);

			for (; (db_num_12 > 0);
				db_num_12 -= (MAX_BLOCK_FOR_S32 * 3)) {
				__m128i summa10, summa02, summa21;

				summa10 = summa02 = summa21 =
				    _mm_setzero_si128();

				mlib_s32 size = (MAX_BLOCK_FOR_S32 * 3);

				if (db_num_12 < (MAX_BLOCK_FOR_S32 * 3))
					size = db_num_12;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (x = 0; x < (size - 2);
				    x += 3, j += 12, sl += 3) {
					__m128i load0, load1, load2;
					__m128i s0, s1, s2, s3, s4, s5;

					load0 = _mm_load_si128(sl);
					load1 = _mm_load_si128(sl + 1);
					load2 = _mm_load_si128(sl + 2);
					MLIB_S_CONVERT_4S32_2S64(s1, s0,
					    load0);
					MLIB_S_CONVERT_4S32_2S64(s3, s2,
					    load1);
					MLIB_S_CONVERT_4S32_2S64(s5, s4,
					    load2);
					summa10 = _mm_add_epi64(summa10, s0);
					summa02 = _mm_add_epi64(summa02, s1);
					summa21 = _mm_add_epi64(summa21, s2);
					summa10 = _mm_add_epi64(summa10, s3);
					summa02 = _mm_add_epi64(summa02, s4);
					summa21 = _mm_add_epi64(summa21, s5);
				}

				__m128i xsum0, xsum1, xsum2;
				mlib_s64 *psum0 = (mlib_s64 *)&xsum0;
				mlib_s64 *psum1 = (mlib_s64 *)&xsum1;
				mlib_s64 *psum2 = (mlib_s64 *)&xsum2;

				_mm_store_si128((void *)psum0, summa10);
				_mm_store_si128((void *)psum1, summa21);
				_mm_store_si128((void *)psum2, summa02);

				buffer3 += psum0[0] + psum2[1];
				buffer4 += psum0[1] + psum1[0];
				buffer5 += psum1[1] + psum2[0];
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < (width - 2); j += 3) {
				buffer3 += sa[j + 0];
				buffer4 += sa[j + 1];
				buffer5 += sa[j + 2];
			}

			if (sign == 2) {
				buffer1 += sa[j + 0];
				buffer2 += sa[j + 1];
				buffer0 += buffer5;
				buffer1 += buffer3;
				buffer2 += buffer4;
			} else if (sign == 1) {
				buffer2 += sa[j];
				buffer0 += buffer4;
				buffer1 += buffer5;
				buffer2 += buffer3;
			} else if (sign == 0) {
				buffer0 += buffer3;
				buffer1 += buffer4;
				buffer2 += buffer5;
			}

			buffer3 = buffer4 = buffer5 = (mlib_s64)0;
			sa += (stride >> 2);
		}

		sum0 = (mlib_d64)buffer0;
		sum1 = (mlib_d64)buffer1;
		sum2 = (mlib_d64)buffer2;
		mlib_d64 divider = 1.0 / npixels;
		mean[0] = sum0 * divider;
		mean[1] = sum1 * divider;
		mean[2] = sum2 * divider;
		break;
	} /* case 3 */

	case 4: {
		__m128i *sl;
		mlib_d64 buffer0 = 0.0;
		mlib_d64 buffer1 = 0.0;
		mlib_d64 buffer2 = 0.0;
		mlib_d64 buffer3 = 0.0;

		width <<= 2;

		for (i = 0; i < height; i++) {
			mlib_s32 db_num = 0;
			mlib_s32 sign = 0;
			mlib_s32 j = 0, l;
			mlib_s32 alignment =
			    ((16 - ((mlib_addr)sa & 15)) & 15) >> 2;

			for (; (j < width - 3) &&
			    (alignment >= 4); j += 4) {
				buffer0 += sa[0];
				buffer1 += sa[1];
				buffer2 += sa[2];
				buffer3 += sa[3];
				alignment -= 4;
			}

			if ((alignment == 3)) {
				buffer0 += sa[j + 0];
				buffer1 += sa[j + 1];
				buffer2 += sa[j + 2];
				j += 3;
				sign = 1;
			} else if ((alignment == 2)) {
				buffer0 += sa[j + 0];
				buffer1 += sa[j + 1];
				j += 2;
				sign = 2;
			} else if ((alignment == 1)) {
				buffer0 += sa[j + 0];
				j += 1;
				sign = 3;
			}

			mlib_s32 size = ((width - j) >> 2);
			sl = (__m128i *) (sa + j);

			__m128d summa0, summa1;
			summa0 = summa1 =
			    _mm_setzero_pd();

			j += (size << 2);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(3)
#endif /* __SUNPRO_C */
			for (x = 0; x < size; x++, sl++) {
				__m128i load0;
				__m128d data64_1, data64_2;
				load0 = _mm_load_si128(sl);
				data64_1 = _mm_cvtepi32_pd(load0);
				data64_2 = _mm_cvtepi32_pd(
				    _mm_srli_si128(load0, 8));

				summa0 = _mm_add_pd(
				    summa0, data64_1);
				summa1 = _mm_add_pd(
				    summa1, data64_2);
			}

			mlib_d64 buffer4, buffer5, buffer6, buffer7;
			_mm_storel_pd(&buffer4, summa0);
			_mm_storeh_pd(&buffer5, summa0);
			_mm_storel_pd(&buffer6, summa1);
			_mm_storeh_pd(&buffer7, summa1);

			if (sign == 3) {
				buffer1 += sa[j + 0];
				buffer2 += sa[j + 1];
				buffer3 += sa[j + 2];

				buffer0 += buffer7;
				buffer1 += buffer4;
				buffer2 += buffer5;
				buffer3 += buffer6;
			} else if (sign == 2) {
				buffer2 += sa[j + 0];
				buffer3 += sa[j + 1];

				buffer0 += buffer6;
				buffer1 += buffer7;
				buffer2 += buffer4;
				buffer3 += buffer5;
			} else if (sign == 1) {
				buffer3 += sa[j];

				buffer0 += buffer5;
				buffer1 += buffer6;
				buffer2 += buffer7;
				buffer3 += buffer4;
			} else if (sign == 0) {
				buffer0 += buffer4;
				buffer1 += buffer5;
				buffer2 += buffer6;
				buffer3 += buffer7;
			}

			sa += (stride >> 2);
		}

		mlib_d64 divider = 1.0 / npixels;

		mean[0] = buffer0 * divider;
		mean[1] = buffer1 * divider;
		mean[2] = buffer2 * divider;
		mean[3] = buffer3 * divider;
		break;
	} /* case 4 */
	} /* switch (channels) */
}

/* *********************************************************** */
