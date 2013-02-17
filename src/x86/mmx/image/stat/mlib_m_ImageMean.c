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

#pragma ident	"@(#)mlib_m_ImageMean.c	9.7	07/11/05 SMI"

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
 *      The image can have 1, 2, 3 or 4 channels.
 *      It can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *
 *           1    w-1 h-1
 *      m = --- * SUM SUM x(i, j)
 *          w*h   i = 0 j = 0
 */

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <mmintrin.h>
#endif

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_mmx_utils.h>

/* *********************************************************** */

#ifdef	_NO_LONGLONG
#define	LL_INT64	long long
#else	/* _NO_LONGLONG */
#define	LL_INT64	mlib_s64
#endif	/* _NO_LONGLONG */

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMean = __mlib_ImageMean

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageMean) mlib_ImageMean
    __attribute__((weak, alias("__mlib_ImageMean")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

    void mlib_m_ImageMean_U8(
    const mlib_image *src,
    mlib_d64 *mean);
    void mlib_m_ImageMean_U16(
    const mlib_image *src,
    mlib_d64 *mean);
    void mlib_m_ImageMean_S16(
    const mlib_image *src,
    mlib_d64 *mean);
    void mlib_m_ImageMean_S32(
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

	case MLIB_BYTE:
		mlib_m_ImageMean_U8(src, mean);
		return (MLIB_SUCCESS);

	case MLIB_USHORT:
		mlib_m_ImageMean_U16(src, mean);
		return (MLIB_SUCCESS);

	case MLIB_SHORT:
		mlib_m_ImageMean_S16(src, mean);
		return (MLIB_SUCCESS);

	case MLIB_INT:
		mlib_m_ImageMean_S32(src, mean);
		return (MLIB_SUCCESS);

	default:
		return (MLIB_FAILURE);
	}
}

/*  CONVERT  */

#define	MLIB_M_CONVERT_8U8_4S16(_4s16_1, _4s16_2, _8u8)         \
	{                                                       \
	    _4s16_1 = _mm_unpackhi_pi8(_8u8, _m_zero);          \
	    _4s16_2 = _mm_unpacklo_pi8(_8u8, _m_zero);          \
	}

/* *********************************************************** */
#define	MLIB_M_CONVERT_4S16_2S32(_2s32_1, _2s32_2, _4s16)               \
	{                                                               \
	    _2s32_1 =                                                   \
		_mm_srai_pi32(_mm_unpackhi_pi16(_m_zero, _4s16), 16);   \
	    _2s32_2 =                                                   \
		_mm_srai_pi32(_mm_unpacklo_pi16(_m_zero, _4s16), 16);   \
	}

/* *********************************************************** */
#define	MLIB_M_CONVERT_4U16_2S32(_2s32_1, _2s32_2, _4u16)       \
	{                                                       \
	    _2s32_1 = _mm_unpackhi_pi16(_4u16, _m_zero);        \
	    _2s32_2 = _mm_unpacklo_pi16(_4u16, _m_zero);        \
	}

/* *********************************************************** */
#define	MAX_BLOCK_FOR_U8	128

/* *********************************************************** */

void
mlib_m_ImageMean_U8(
    const mlib_image *src,
    mlib_d64 *mean)
{
/* pointer for pixel of source */
	mlib_u8 *sa = (mlib_u8 *)mlib_ImageGetData(src);

/* width of source image */
	mlib_s32 width = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 height = mlib_ImageGetHeight(src);

/* linestride = bytes to next row */
	mlib_s32 stride = mlib_ImageGetStride(src);

/* number of channels */
	mlib_s32 channels = mlib_ImageGetChannels(src);

/* total number of pixels */
	mlib_s32 npixels = height * width;

/* indices */
	mlib_s32 i, x;

	if (channels * width == stride) {
		width = npixels;
		height = 1;
	}

	switch (channels) {
	case 1:
	    {
		    __m64 load0, summa0, summa1;
		    __m64 *da;
		    mlib_d64 divider, sum0;
		    mlib_s32 buffer0 = 0;
		    mlib_s16 temp[8];

		    summa0 = summa1 = _mm_setzero_si64();

		    for (i = 0; i < height; i++) {

			    mlib_s32 j, db_num = 0;

			    for (j = 0;
				(j < width) && ((mlib_addr)(sa + j) & 7); j++)
				    buffer0 += sa[j];

			    db_num = (width - j) >> 3;
			    da = (__m64 *) (sa + j);

			    for (; db_num > 0; db_num -= MAX_BLOCK_FOR_U8) {

				    mlib_s32 size = MAX_BLOCK_FOR_U8;

				    if (db_num < MAX_BLOCK_FOR_U8)
					    size = db_num;

				    j += (size << 3);

				    for (x = 0; x < size; x++) {
					    __m64 sd1, sd2;

					    load0 = da[x];
					    MLIB_M_CONVERT_8U8_4S16(sd1, sd2,
						load0);
					    summa0 = _mm_add_pi16(summa0, sd1);
					    summa1 = _mm_add_pi16(summa1, sd2);
				    }

				    da += size;

				    ((__m64 *) temp)[0] = summa0;
				    ((__m64 *) temp)[1] = summa1;

				    buffer0 += temp[0];
				    buffer0 += temp[1];
				    buffer0 += temp[2];
				    buffer0 += temp[3];
				    buffer0 += temp[4];
				    buffer0 += temp[5];
				    buffer0 += temp[6];
				    buffer0 += temp[7];
				    summa0 = summa1 = _mm_setzero_si64();
			    }

			    for (; j < width; j++)
				    buffer0 += sa[j];

			    sa += stride;
		    }

		    _mm_empty();

		    sum0 = (mlib_d64)buffer0;
		    divider = 1.0 / npixels;
		    mean[0] = sum0 * divider;

		    break;
	    }

	case 2:
	    {
		    __m64 load0, summa0, summa1, *da;
		    mlib_d64 divider;
		    mlib_d64 sum0, sum1;
		    mlib_s32 buffer0 = 0, buffer1 = 0;
		    mlib_s16 temp[8];

		    width <<= 1;
		    summa0 = summa1 = _mm_setzero_si64();

		    for (i = 0; i < height; i++) {

			    mlib_s32 db_num = 0;
			    mlib_s32 alignment = 1;
			    mlib_s32 j = 0;

			    if ((mlib_addr)sa & 1) {
				    alignment = 0;
				    buffer0 += sa[0];

				    for (j = 1;
					(j < (width - 2)) &&
					((mlib_addr)(sa + j) & 7); j += 2) {
					    buffer1 += sa[j + 0];
					    buffer0 += sa[j + 1];
				    }
			    } else {
				    for (j = 0;
					(j < width) &&
					((mlib_addr)(sa + j) & 7); j += 2) {
					    buffer0 += sa[j + 0];
					    buffer1 += sa[j + 1];
				    }
			    }

			    db_num = (width - j) >> 3;
			    da = (__m64 *) (sa + j);

			    for (; (db_num > 0); db_num -= MAX_BLOCK_FOR_U8) {

				    mlib_s32 size = MAX_BLOCK_FOR_U8;

				    if (db_num < MAX_BLOCK_FOR_U8)
					    size = db_num;

				    j += (size << 3);

				    for (x = 0; x < size; x++) {
					    __m64 sd1, sd2;

					    load0 = da[x];
					    MLIB_M_CONVERT_8U8_4S16(sd1, sd2,
						load0);
					    summa0 = _mm_add_pi16(summa0, sd1);
					    summa1 = _mm_add_pi16(summa1, sd2);
				    }

				    da += size;

				    ((__m64 *) temp)[0] = summa0;
				    ((__m64 *) temp)[1] = summa1;

				    if (alignment) {
					    buffer0 += temp[0];
					    buffer0 += temp[2];
					    buffer0 += temp[4];
					    buffer0 += temp[6];
					    buffer1 += temp[1];
					    buffer1 += temp[3];
					    buffer1 += temp[5];
					    buffer1 += temp[7];
				    } else {
					    buffer0 += temp[1];
					    buffer0 += temp[3];
					    buffer0 += temp[5];
					    buffer0 += temp[7];
					    buffer1 += temp[0];
					    buffer1 += temp[2];
					    buffer1 += temp[4];
					    buffer1 += temp[6];
				    }

				    summa0 = summa1 = _mm_setzero_si64();
			    }

			    if (alignment) {
				    for (; j < width; j += 2) {
					    buffer0 += sa[j + 0];
					    buffer1 += sa[j + 1];
				    }
			    } else {
				    for (; j < (width - 1); j += 2) {
					    buffer1 += sa[j + 0];
					    buffer0 += sa[j + 1];
				    }

				    buffer1 += sa[j];
			    }

			    sa += stride;
		    }

		    _mm_empty();

		    sum0 = (mlib_d64)buffer0;
		    sum1 = (mlib_d64)buffer1;

		    divider = 1.0 / npixels;

		    mean[0] = sum0 * divider;
		    mean[1] = sum1 * divider;

		    break;
	    }

	case 3:
	    {
		    __m64 data0 = _mm_setzero_si64();
		    __m64 load0, load1, load2;
		    __m64 summa0210_0, summa1021_0, summa2102_0;
		    __m64 summa0210_1, summa1021_1, summa2102_1;
		    __m64 s0, s1, s2;
		    __m64 *da;
		    mlib_d64 divider;
		    mlib_d64 sum0, sum1, sum2;
		    mlib_s32 buffer0 = 0;
		    mlib_s32 buffer1 = 0;
		    mlib_s32 buffer2 = 0;
		    mlib_s32 buffer3 = 0;
		    mlib_s32 buffer4 = 0;
		    mlib_s32 buffer5 = 0;

		    width *= 3;

		    summa0210_0 = summa1021_0 = summa2102_0 =
			_mm_setzero_si64();
		    summa0210_1 = summa1021_1 = summa2102_1 =
			_mm_setzero_si64();

		    for (i = 0; i < height; i++) {

			    mlib_s32 db_num = 0;
			    mlib_s32 sign = 0;
			    mlib_s32 j = 0;
			    mlib_s32 alignment = (8 - ((mlib_addr)sa & 7)) & 7;

			    for (j = 0;
				(j < width - 3) && (j <= (alignment - 3));
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

			    db_num = (width - j) / 24;
			    da = (__m64 *) (sa + j);

			    for (; db_num > 0; db_num -= MAX_BLOCK_FOR_U8) {

				    mlib_s32 size = MAX_BLOCK_FOR_U8;

				    if (db_num < MAX_BLOCK_FOR_U8)
					    size = db_num;

				    j += (24 * size);

				    for (; size > 0; size--) {
					    load0 = da[0];
					    load1 = da[1];
					    load2 = da[2];

					    summa0210_0 =
						_mm_add_pi16(summa0210_0,
						_mm_unpacklo_pi8(load0, data0));
					    summa1021_0 =
						_mm_add_pi16(summa1021_0,
						_mm_unpackhi_pi8(load0, data0));
					    summa2102_0 =
						_mm_add_pi16(summa2102_0,
						_mm_unpacklo_pi8(load1, data0));
					    summa0210_1 =
						_mm_add_pi16(summa0210_1,
						_mm_unpackhi_pi8(load1, data0));
					    summa1021_1 =
						_mm_add_pi16(summa1021_1,
						_mm_unpacklo_pi8(load2, data0));
					    summa2102_1 =
						_mm_add_pi16(summa2102_1,
						_mm_unpackhi_pi8(load2, data0));

					    da += 3;
				    }

				    s0 = _mm_add_pi32(_mm_add_pi32
					(_mm_unpacklo_pi16(summa0210_0, data0),
					_mm_unpacklo_pi16(summa0210_1, data0)),
					_mm_add_pi32(_mm_unpackhi_pi16
					(summa1021_0, data0),
					_mm_unpackhi_pi16(summa1021_1, data0)));
				    s1 = _mm_add_pi32(_mm_add_pi32
					(_mm_unpackhi_pi16(summa0210_0, data0),
					_mm_unpackhi_pi16(summa0210_1, data0)),
					_mm_add_pi32(_mm_unpacklo_pi16
					(summa2102_0, data0),
					_mm_unpacklo_pi16(summa2102_1, data0)));
				    s2 = _mm_add_pi32(_mm_add_pi32
					(_mm_unpacklo_pi16(summa1021_0, data0),
					_mm_unpacklo_pi16(summa1021_1, data0)),
					_mm_add_pi32(_mm_unpackhi_pi16
					(summa2102_0, data0),
					_mm_unpackhi_pi16(summa2102_1, data0)));

				    buffer3 +=
					_mm_cvtsi64_si32(s0) +
					_mm_cvtsi64_si32(_mm_srli_si64(s1, 32));
				    buffer4 +=
					_mm_cvtsi64_si32(s2) +
					_mm_cvtsi64_si32(_mm_srli_si64(s0, 32));
				    buffer5 +=
					_mm_cvtsi64_si32(s1) +
					_mm_cvtsi64_si32(_mm_srli_si64(s2, 32));

				    summa0210_0 = summa1021_0 = summa2102_0 =
					_mm_setzero_si64();
				    summa0210_1 = summa1021_1 = summa2102_1 =
					_mm_setzero_si64();
			    }

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

			    buffer3 = buffer4 = buffer5 = (mlib_s32)0;
			    sa += stride;
		    }

		    _mm_empty();

		    sum0 = (mlib_d64)buffer0;
		    sum1 = (mlib_d64)buffer1;
		    sum2 = (mlib_d64)buffer2;

		    divider = 1.0 / npixels;

		    mean[0] = sum0 * divider;
		    mean[1] = sum1 * divider;
		    mean[2] = sum2 * divider;

		    break;
	    }

	case 4:
	    {
		    __m64 load0, summa0, summa1, *da;
		    mlib_d64 divider, sum0, sum1, sum2, sum3;
		    mlib_s32 buffer0 = 0;
		    mlib_s32 buffer1 = 0;
		    mlib_s32 buffer2 = 0;
		    mlib_s32 buffer3 = 0;
		    mlib_s32 buffer4 = 0;
		    mlib_s32 buffer5 = 0;
		    mlib_s32 buffer6 = 0;
		    mlib_s32 buffer7 = 0;
		    mlib_s16 temp[8];

		    width <<= 2;
		    summa0 = summa1 = _mm_setzero_si64();

		    for (i = 0; i < height; i++) {
			    mlib_s32 db_num = 0;
			    mlib_s32 sign = 0;
			    mlib_s32 j = 0;
			    mlib_s32 alignment = (8 - ((mlib_addr)sa & 7)) & 7;

			    if ((j < (width - 3)) && (alignment >= 4)) {
				    buffer0 += sa[j + 0];
				    buffer1 += sa[j + 1];
				    buffer2 += sa[j + 2];
				    buffer3 += sa[j + 3];
				    j += 4;
			    }

			    if ((j < (width - 3)) && ((alignment - j) == 3)) {
				    buffer0 += sa[j + 0];
				    buffer1 += sa[j + 1];
				    buffer2 += sa[j + 2];
				    j += 3;
				    sign = 1;
			    } else if ((j < (width - 3)) &&
				((alignment - j) == 2)) {
				    buffer0 += sa[j + 0];
				    buffer1 += sa[j + 1];
				    j += 2;
				    sign = 2;
			    } else if ((j < (width - 3)) &&
				((alignment - j) == 1)) {
				    buffer0 += sa[j + 0];
				    j += 1;
				    sign = 3;
			    }

			    db_num = (width - j) >> 3;
			    da = (__m64 *) (sa + j);

			    for (; (db_num > 0); db_num -= MAX_BLOCK_FOR_U8) {

				    mlib_s32 size = MAX_BLOCK_FOR_U8;

				    if (db_num < MAX_BLOCK_FOR_U8)
					    size = db_num;

				    j += (size << 3);

				    for (x = 0; x < size; x++) {
					    __m64 sd1, sd2;

					    load0 = da[x];
					    MLIB_M_CONVERT_8U8_4S16(sd1, sd2,
						load0);
					    summa0 = _mm_add_pi16(summa0, sd1);
					    summa1 = _mm_add_pi16(summa1, sd2);
				    }

				    da += size;

				    ((__m64 *) temp)[0] = summa0;
				    ((__m64 *) temp)[1] = summa1;

				    buffer7 += (temp[3] + temp[7]);
				    buffer6 += (temp[2] + temp[6]);
				    buffer5 += (temp[1] + temp[5]);
				    buffer4 += (temp[0] + temp[4]);

				    summa0 = summa1 = _mm_setzero_si64();
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

			    sa += stride;
			    buffer4 = buffer5 = buffer6 = buffer7 = (mlib_s32)0;
		    }

		    _mm_empty();

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
	    }
	}

	_mm_empty();
}

/* *********************************************************** */

#define	MAX_BLOCK_FOR_S16	128

/* *********************************************************** */

void
mlib_m_ImageMean_S16(
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

	if (2 * channels * width == stride) {
		width = npixels;
		height = 1;
	}

	switch (channels) {

	case 1: {
		    __m64 load0;
		    __m64 data32_1, data32_2;
		    __m64 summa0, summa1;

		    mlib_d64 divider;
		    mlib_d64 sum0;
		    LL_INT64 buffer0 = 0;

		    summa0 = summa1 = _mm_setzero_si64();

		    for (i = 0; i < height; i++) {
			    mlib_s32 db_num = 0;
			    mlib_s32 j = 0;
			    __m64 *sl;

			    for (j = 0;
				(j < width) &&
				((mlib_addr)((mlib_s16 *)sa + j) & 7); j++)
				    buffer0 += sa[j];

			    db_num = (width - j) >> 2;
			    sl = (__m64 *) ((mlib_s16 *)sa + j);

			    for (; db_num > 0; db_num -= MAX_BLOCK_FOR_S16) {

				    mlib_s32 size = MAX_BLOCK_FOR_S16;

				    if (db_num < MAX_BLOCK_FOR_S16)
					    size = db_num;

				    j += (size << 2);

				    for (x = 0; x < size; x++) {
					    load0 = sl[x];
					    MLIB_M_CONVERT_4S16_2S32(data32_1,
						data32_2, load0);
					    summa0 =
						_mm_add_pi32(summa0, data32_1);
					    summa1 =
						_mm_add_pi32(summa1, data32_2);
				    }

				    sl += size;
				    summa0 = _mm_add_pi32(summa0, summa1);

				    buffer0 +=
					((LL_INT64)_mm_cvtsi64_si32(summa0)
					+
					(LL_INT64)
					_mm_cvtsi64_si32(_mm_srli_si64(summa0,
					32)));
				    summa0 = summa1 = _mm_setzero_si64();
			    }

			    for (; j < width; j++)
				    buffer0 += sa[j];

			    sa += (stride >> 1);
		    }

		    _mm_empty();

		    sum0 = (mlib_d64)buffer0;
		    divider = 1.0 / npixels;
		    mean[0] = sum0 * divider;
		    break;
	    }

	case 2: {
		    __m64 load0;
		    __m64 data32_1, data32_2;
		    __m64 summa0, summa1;
		    __m64 *sl;

		    mlib_d64 divider;
		    mlib_d64 sum0, sum1;
		    LL_INT64 buffer0 = 0;
		    LL_INT64 buffer1 = 0;
		    LL_INT64 buffer2 = 0;
		    LL_INT64 buffer3 = 0;

/*   ImageWidth * ChannelNumber   */
		    width <<= 1;
		    summa0 = summa1 = _mm_setzero_si64();

		    for (i = 0; i < height; i++) {
			    mlib_s32 db_num = 0;
			    mlib_s32 alignment = 1;
			    mlib_s32 j = 0;

			    if ((mlib_addr)sa & 1) {
				    alignment = 0;
				    buffer0 += sa[0];

				    for (j = 1;
					(j < (width - 2)) &&
					((mlib_addr)(sa + j) & 7); j += 2) {
					    buffer1 += sa[j + 0];
					    buffer0 += sa[j + 1];
				    }
			    } else {
				    for (j = 0;
					(j < width) &&
					((mlib_addr)(sa + j) & 7); j += 2) {
					    buffer0 += sa[j + 0];
					    buffer1 += sa[j + 1];
				    }
			    }

			    db_num = (width - j) >> 2;
			    sl = (__m64 *) (sa + j);

			    for (; (db_num > 0); db_num -= MAX_BLOCK_FOR_S16) {
				    mlib_s32 size = MAX_BLOCK_FOR_S16;

				    if (db_num < MAX_BLOCK_FOR_S16)
					    size = db_num;
				    j += (size << 2);

				    for (x = 0; x < size; x++) {
					    load0 = sl[x];
					    MLIB_M_CONVERT_4S16_2S32(data32_1,
						data32_2, load0);
					    summa0 =
						_mm_add_pi32(summa0, data32_1);
					    summa1 =
						_mm_add_pi32(summa1, data32_2);
				    }

				    sl += size;

				    summa0 = _mm_add_pi32(summa0, summa1);

				    buffer2 += _mm_cvtsi64_si32(summa0);
				    buffer3 +=
					_mm_cvtsi64_si32(_mm_srli_si64(summa0,
					32));

				    summa0 = summa1 = _mm_setzero_si64();
			    }

			    if (alignment) {
				    buffer0 += buffer2;
				    buffer1 += buffer3;

				    for (; j < width; j += 2) {
					    buffer0 += sa[j + 0];
					    buffer1 += sa[j + 1];
				    }
			    } else {
				    buffer1 += buffer2;
				    buffer0 += buffer3;

				    for (; j < (width - 1); j += 2) {
					    buffer1 += sa[j + 0];
					    buffer0 += sa[j + 1];
				    }

				    buffer1 += sa[j];
			    }

			    sa += (stride >> 1);
			    buffer2 = buffer3 = 0;
		    }

		    _mm_empty();

		    sum0 = (mlib_d64)buffer0;
		    sum1 = (mlib_d64)buffer1;
		    divider = 1.0 / npixels;
		    mean[0] = sum0 * divider;
		    mean[1] = sum1 * divider;
		    break;
	    }

	case 3: {
		    __m64 load0, load1, load2;
		    __m64 s0, s1, s2, s3, s4, s5;
		    __m64 summa10, summa02, summa21;
		    __m64 *sl;

		    mlib_d64 divider, sum0, sum1, sum2;
		    LL_INT64 buffer0 = 0;
		    LL_INT64 buffer1 = 0;
		    LL_INT64 buffer2 = 0;
		    LL_INT64 buffer3 = 0;
		    LL_INT64 buffer4 = 0;
		    LL_INT64 buffer5 = 0;

		    width *= 3;
		    summa10 = summa02 = summa21 = _mm_setzero_si64();

		    for (i = 0; i < height; i++) {
			    mlib_s32 db_num_12 = 0;
			    mlib_s32 sign = 0;
			    mlib_s32 j = 0;
			    mlib_s32 alignment =
				((8 - ((mlib_addr)sa & 7)) & 7) >> 1;

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

			    db_num_12 = ((width - j) >> 2);
			    sl = (__m64 *) (sa + j);

			    for (; (db_num_12 > 0);
				db_num_12 -= (MAX_BLOCK_FOR_S16 * 3)) {
				    mlib_s32 size = (MAX_BLOCK_FOR_S16 * 3);

				    if (db_num_12 < (MAX_BLOCK_FOR_S16 * 3))
					    size = db_num_12;

				    for (x = 0; x < (size - 2);
						x += 3, j += 12) {
					    load0 = sl[x + 0];
					    load1 = sl[x + 1];
					    load2 = sl[x + 2];
					    MLIB_M_CONVERT_4S16_2S32(s1, s0,
						load0);
					    MLIB_M_CONVERT_4S16_2S32(s3, s2,
						load1);
					    MLIB_M_CONVERT_4S16_2S32(s5, s4,
						load2);
					    summa10 = _mm_add_pi32(summa10, s0);
					    summa02 = _mm_add_pi32(summa02, s1);
					    summa21 = _mm_add_pi32(summa21, s2);
					    summa10 = _mm_add_pi32(summa10, s3);
					    summa02 = _mm_add_pi32(summa02, s4);
					    summa21 = _mm_add_pi32(summa21, s5);
				    }

				    sl += size;

				    buffer3 +=
					(LL_INT64)(_mm_cvtsi64_si32
					(_mm_srli_si64(summa02,
					32)) + _mm_cvtsi64_si32(summa10));
				    buffer4 +=
					(LL_INT64)(_mm_cvtsi64_si32
					(_mm_srli_si64(summa10,
					32)) + _mm_cvtsi64_si32(summa21));
				    buffer5 +=
					(LL_INT64)(_mm_cvtsi64_si32
					(_mm_srli_si64(summa21,
					32)) + _mm_cvtsi64_si32(summa02));
				    summa10 = summa02 = summa21 =
					_mm_setzero_si64();
			    }

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

			    buffer3 = buffer4 = buffer5 = (LL_INT64)0;
			    sa += (stride >> 1);
		    }

		    _mm_empty();

		    sum0 = (mlib_d64)buffer0;
		    sum1 = (mlib_d64)buffer1;
		    sum2 = (mlib_d64)buffer2;
		    divider = 1.0 / npixels;
		    mean[0] = sum0 * divider;
		    mean[1] = sum1 * divider;
		    mean[2] = sum2 * divider;
		    break;
	    }

	case 4: {
		    __m64 load0;
		    __m64 data32_1, data32_2;
		    __m64 summa0, summa1;
		    __m64 *sl;
		    mlib_d64 divider, sum0, sum1, sum2, sum3;
		    LL_INT64 buffer0 = 0;
		    LL_INT64 buffer1 = 0;
		    LL_INT64 buffer2 = 0;
		    LL_INT64 buffer3 = 0;
		    LL_INT64 buffer4 = 0;
		    LL_INT64 buffer5 = 0;
		    LL_INT64 buffer6 = 0;
		    LL_INT64 buffer7 = 0;

		    width <<= 2;

		    for (i = 0; i < height; i++) {
			    mlib_s32 db_num = 0;
			    mlib_s32 sign = 0;
			    mlib_s32 j = 0;
			    mlib_s32 alignment = (8 - ((mlib_addr)sa & 7)) & 7;

			    if (alignment >= 4) {
				    buffer0 += sa[0];
				    buffer1 += sa[1];
				    buffer2 += sa[2];
				    buffer3 += sa[3];
				    alignment -= 4;
				    j += 4;
			    }

			    if (j < (width - 3)) {
				    switch (alignment) {
				    case 0: {
						sign = 0;
						break;
					}

				    case 2: {
						buffer0 += sa[j + 0];
						buffer1 += sa[j + 1];
						buffer2 += sa[j + 2];
						sign = 1;
						j += 3;
						break;
					}
				    }

				    db_num = ((width - j) >> 2);
				    sl = (__m64 *) (sa + j);

				    for (; (db_num > 0);
					db_num -= (MAX_BLOCK_FOR_S16)) {
					    mlib_s32 size = MAX_BLOCK_FOR_S16;

					    if (db_num < MAX_BLOCK_FOR_S16)
						    size = db_num;
					    j += (size << 2);

					    summa0 = summa1 =
						_mm_setzero_si64();

					    for (x = 0; x < size; x++) {
						    load0 = sl[x];
						    MLIB_M_CONVERT_4S16_2S32
							(data32_2, data32_1,
							load0);
						    summa0 =
							_mm_add_pi32(summa0,
							data32_1);
						    summa1 =
							_mm_add_pi32(summa1,
							data32_2);
					    }

					    sl += size;

					    buffer4 +=
						(LL_INT64)
						_mm_cvtsi64_si32(summa0);
					    buffer5 +=
						(LL_INT64)
						_mm_cvtsi64_si32(_mm_srli_si64
						(summa0, 32));
					    buffer6 +=
						(LL_INT64)
						_mm_cvtsi64_si32(summa1);
					    buffer7 +=
						(LL_INT64)
						_mm_cvtsi64_si32(_mm_srli_si64
						(summa1, 32));
					    summa0 = summa1 =
						_mm_setzero_si64();
				    }
			    }

			    switch (sign) {
			    case 0: {
					buffer0 += buffer4;
					buffer1 += buffer5;
					buffer2 += buffer6;
					buffer3 += buffer7;
					break;
				}

			    case 1: {
					buffer3 += sa[j + 0];
					buffer0 += buffer5;
					buffer1 += buffer6;
					buffer2 += buffer7;
					buffer3 += buffer4;
					break;
				}
			    }

			    sa += (stride >> 1);
			    buffer4 = buffer5 = buffer6 = buffer7 = 0;
		    }

		    _mm_empty();

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
	    }
	}
}

/* *********************************************************** */

#define	MAX_BLOCK_FOR_U16	128

/* *********************************************************** */

void
mlib_m_ImageMean_U16(
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

	if (2 * channels * width == stride) {
		width = npixels;
		height = 1;
	}

	switch (channels) {

	case 1: {
		    __m64 load0;
		    __m64 data32_1, data32_2;
		    __m64 summa0, summa1;

		    mlib_d64 divider;
		    mlib_d64 sum0;
		    LL_INT64 buffer0 = 0;

		    summa0 = summa1 = _mm_setzero_si64();

		    for (i = 0; i < height; i++) {
			    mlib_s32 db_num = 0;
			    mlib_s32 j = 0;
			    __m64 *sl;

			    for (j = 0;
				(j < width) &&
				((mlib_addr)((mlib_u16 *)sa + j) & 7); j++)
				    buffer0 += sa[j];

			    db_num = (width - j) >> 2;
			    sl = (__m64 *) ((mlib_u16 *)sa + j);

			    for (; db_num > 0; db_num -= MAX_BLOCK_FOR_U16) {

				    mlib_s32 size = MAX_BLOCK_FOR_U16;

				    if (db_num < MAX_BLOCK_FOR_U16)
					    size = db_num;

				    j += (size << 2);

				    for (x = 0; x < size; x++) {
					    load0 = sl[x];
					    MLIB_M_CONVERT_4U16_2S32(data32_1,
						data32_2, load0);
					    summa0 =
						_mm_add_pi32(summa0, data32_1);
					    summa1 =
						_mm_add_pi32(summa1, data32_2);
				    }

				    sl += size;
				    summa0 = _mm_add_pi32(summa0, summa1);

				    buffer0 +=
					((LL_INT64)_mm_cvtsi64_si32(summa0)
					+
					(LL_INT64)
					_mm_cvtsi64_si32(_mm_srli_si64(summa0,
					32)));
				    summa0 = summa1 = _mm_setzero_si64();
			    }

			    for (; j < width; j++)
				    buffer0 += sa[j];

			    sa += (stride >> 1);
		    }

		    _mm_empty();

		    sum0 = (mlib_d64)buffer0;
		    divider = 1.0 / npixels;
		    mean[0] = sum0 * divider;
		    break;
	    }

	case 2: {
		    __m64 load0;
		    __m64 data32_1, data32_2;
		    __m64 summa0, summa1;
		    __m64 *sl;

		    mlib_d64 divider;
		    mlib_d64 sum0, sum1;
		    LL_INT64 buffer0 = 0;
		    LL_INT64 buffer1 = 0;
		    LL_INT64 buffer2 = 0;
		    LL_INT64 buffer3 = 0;

/*   ImageWidth * ChannelNumber   */
		    width <<= 1;
		    summa0 = summa1 = _mm_setzero_si64();

		    for (i = 0; i < height; i++) {
			    mlib_s32 db_num = 0;
			    mlib_s32 alignment = 1;
			    mlib_s32 j = 0;

			    if ((mlib_addr)sa & 1) {
				    alignment = 0;
				    buffer0 += sa[0];

				    for (j = 1;
					(j < (width - 2)) &&
					((mlib_addr)(sa + j) & 7); j += 2) {
					    buffer1 += sa[j + 0];
					    buffer0 += sa[j + 1];
				    }
			    } else {
				    for (j = 0;
					(j < width) &&
					((mlib_addr)(sa + j) & 7); j += 2) {
					    buffer0 += sa[j + 0];
					    buffer1 += sa[j + 1];
				    }
			    }

			    db_num = (width - j) >> 2;
			    sl = (__m64 *) (sa + j);

			    for (; (db_num > 0); db_num -= MAX_BLOCK_FOR_U16) {
				    mlib_s32 size = MAX_BLOCK_FOR_U16;

				    if (db_num < MAX_BLOCK_FOR_U16)
					    size = db_num;
				    j += (size << 2);

				    for (x = 0; x < size; x++) {
					    load0 = sl[x];
					    MLIB_M_CONVERT_4U16_2S32(data32_1,
						data32_2, load0);
					    summa0 =
						_mm_add_pi32(summa0, data32_1);
					    summa1 =
						_mm_add_pi32(summa1, data32_2);
				    }

				    sl += size;

				    summa0 = _mm_add_pi32(summa0, summa1);

				    buffer2 += _mm_cvtsi64_si32(summa0);
				    buffer3 +=
					_mm_cvtsi64_si32(_mm_srli_si64(summa0,
					32));

				    summa0 = summa1 = _mm_setzero_si64();
			    }

			    if (alignment) {
				    buffer0 += buffer2;
				    buffer1 += buffer3;

				    for (; j < width; j += 2) {
					    buffer0 += sa[j + 0];
					    buffer1 += sa[j + 1];
				    }
			    } else {
				    buffer1 += buffer2;
				    buffer0 += buffer3;

				    for (; j < (width - 1); j += 2) {
					    buffer1 += sa[j + 0];
					    buffer0 += sa[j + 1];
				    }

				    buffer1 += sa[j];
			    }

			    sa += (stride >> 1);
			    buffer2 = buffer3 = 0;
		    }

		    _mm_empty();

		    sum0 = (mlib_d64)buffer0;
		    sum1 = (mlib_d64)buffer1;
		    divider = 1.0 / npixels;
		    mean[0] = sum0 * divider;
		    mean[1] = sum1 * divider;
		    break;
	    }

	case 3: {
		    __m64 load0, load1, load2;
		    __m64 s0, s1, s2, s3, s4, s5;
		    __m64 summa10, summa02, summa21;
		    __m64 *sl;

		    mlib_d64 divider, sum0, sum1, sum2;
		    LL_INT64 buffer0 = 0;
		    LL_INT64 buffer1 = 0;
		    LL_INT64 buffer2 = 0;
		    LL_INT64 buffer3 = 0;
		    LL_INT64 buffer4 = 0;
		    LL_INT64 buffer5 = 0;

		    width *= 3;
		    summa10 = summa02 = summa21 = _mm_setzero_si64();

		    for (i = 0; i < height; i++) {
			    mlib_s32 db_num_12 = 0;
			    mlib_s32 sign = 0;
			    mlib_s32 j = 0;
			    mlib_s32 alignment =
				((8 - ((mlib_addr)sa & 7)) & 7) >> 1;

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

			    db_num_12 = ((width - j) >> 2);
			    sl = (__m64 *) (sa + j);

			    for (; (db_num_12 > 0);
				db_num_12 -= (MAX_BLOCK_FOR_U16 * 3)) {
				    mlib_s32 size = (MAX_BLOCK_FOR_U16 * 3);

				    if (db_num_12 < (MAX_BLOCK_FOR_U16 * 3))
					    size = db_num_12;

				    for (x = 0; x < (size - 2);
						x += 3, j += 12) {
					    load0 = sl[x + 0];
					    load1 = sl[x + 1];
					    load2 = sl[x + 2];
					    MLIB_M_CONVERT_4U16_2S32(s1, s0,
						load0);
					    MLIB_M_CONVERT_4U16_2S32(s3, s2,
						load1);
					    MLIB_M_CONVERT_4U16_2S32(s5, s4,
						load2);
					    summa10 = _mm_add_pi32(summa10, s0);
					    summa02 = _mm_add_pi32(summa02, s1);
					    summa21 = _mm_add_pi32(summa21, s2);
					    summa10 = _mm_add_pi32(summa10, s3);
					    summa02 = _mm_add_pi32(summa02, s4);
					    summa21 = _mm_add_pi32(summa21, s5);
				    }

				    sl += size;

				    buffer3 +=
					(LL_INT64)(_mm_cvtsi64_si32
					(_mm_srli_si64(summa02,
					32)) + _mm_cvtsi64_si32(summa10));
				    buffer4 +=
					(LL_INT64)(_mm_cvtsi64_si32
					(_mm_srli_si64(summa10,
					32)) + _mm_cvtsi64_si32(summa21));
				    buffer5 +=
					(LL_INT64)(_mm_cvtsi64_si32
					(_mm_srli_si64(summa21,
					32)) + _mm_cvtsi64_si32(summa02));
				    summa10 = summa02 = summa21 =
					_mm_setzero_si64();
			    }

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

			    buffer3 = buffer4 = buffer5 = (LL_INT64)0;
			    sa += (stride >> 1);
		    }

		    _mm_empty();

		    sum0 = (mlib_d64)buffer0;
		    sum1 = (mlib_d64)buffer1;
		    sum2 = (mlib_d64)buffer2;
		    divider = 1.0 / npixels;
		    mean[0] = sum0 * divider;
		    mean[1] = sum1 * divider;
		    mean[2] = sum2 * divider;
		    break;
	    }

	case 4: {
		    __m64 load0;
		    __m64 data32_1, data32_2;
		    __m64 summa0, summa1;
		    __m64 *sl;
		    mlib_d64 divider, sum0, sum1, sum2, sum3;
		    LL_INT64 buffer0 = 0;
		    LL_INT64 buffer1 = 0;
		    LL_INT64 buffer2 = 0;
		    LL_INT64 buffer3 = 0;
		    LL_INT64 buffer4 = 0;
		    LL_INT64 buffer5 = 0;
		    LL_INT64 buffer6 = 0;
		    LL_INT64 buffer7 = 0;

		    width <<= 2;

		    for (i = 0; i < height; i++) {
			    mlib_s32 db_num = 0;
			    mlib_s32 sign = 0;
			    mlib_s32 j = 0, l;
			    mlib_s32 alignment = (8 - ((mlib_addr)sa & 7)) & 7;

			    if (alignment >= 4) {
				    buffer0 += sa[0];
				    buffer1 += sa[1];
				    buffer2 += sa[2];
				    buffer3 += sa[3];
				    alignment -= 4;
				    j += 4;
			    }

			    if (j < (width - 3)) {
				    switch (alignment) {
				    case 0: {
						sign = 0;
						break;
					}

				    case 2: {
						buffer0 += sa[j + 0];
						buffer1 += sa[j + 1];
						buffer2 += sa[j + 2];
						sign = 1;
						j += 3;
						break;
					}
				    }

				    db_num = ((width - j) >> 2);
				    sl = (__m64 *) (sa + j);

				    for (; (db_num > 0);
					db_num -= (MAX_BLOCK_FOR_U16)) {
					    mlib_s32 size = MAX_BLOCK_FOR_U16;

					    if (db_num < MAX_BLOCK_FOR_U16)
						    size = db_num;
					    j += (size << 2);

					    summa0 = summa1 =
						_mm_setzero_si64();

					    for (x = 0; x < size; x++) {
						    load0 = sl[x];
						    MLIB_M_CONVERT_4U16_2S32
							(data32_2, data32_1,
							load0);
						    summa0 =
							_mm_add_pi32(summa0,
							data32_1);
						    summa1 =
							_mm_add_pi32(summa1,
							data32_2);
					    }

					    sl += size;

					    buffer4 +=
						(LL_INT64)
						_mm_cvtsi64_si32(summa0);
					    buffer5 +=
						(LL_INT64)
						_mm_cvtsi64_si32(_mm_srli_si64
						(summa0, 32));
					    buffer6 +=
						(LL_INT64)
						_mm_cvtsi64_si32(summa1);
					    buffer7 +=
						(LL_INT64)
						_mm_cvtsi64_si32(_mm_srli_si64
						(summa1, 32));
					    summa0 = summa1 =
						_mm_setzero_si64();
				    }
			    }

			    switch (sign) {
			    case 0: {
					buffer0 += buffer4;
					buffer1 += buffer5;
					buffer2 += buffer6;
					buffer3 += buffer7;
					break;
				}

			    case 1: {
					buffer3 += sa[j + 0];
					buffer0 += buffer5;
					buffer1 += buffer6;
					buffer2 += buffer7;
					buffer3 += buffer4;
					break;
				}
			    }

			    sa += (stride >> 1);
			    buffer4 = buffer5 = buffer6 = buffer7 = 0;
		    }

		    _mm_empty();

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
	    }
	}
}

/* *********************************************************** */

void
mlib_m_ImageMean_S32(
    const mlib_image *src,
    mlib_d64 *mean)
{
/* width of source image */
	mlib_s32 width = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 height = mlib_ImageGetHeight(src);

/* linestride = bytes to next row */
	mlib_s32 stride = mlib_ImageGetStride(src);

/* number of channels */
	mlib_s32 channels = mlib_ImageGetChannels(src);

/* pointer for pixel of source */
	mlib_s32 *sa = (mlib_s32 *)mlib_ImageGetData(src);

/* total number of pixels */
	mlib_s32 npixels = height * width;
	mlib_d64 divider = 1.0 / npixels;

/* indices */
	mlib_s32 k, j;

	if (4 * channels * width == stride) {
		width = npixels;
		height = 1;
	}

	switch (channels) {

	case 1: {

		    mlib_d64 firstAX = 0.0;
		    mlib_s32 i, cycleNumber = width >> 16;
		    mlib_s32 val, hi, lo;
		    mlib_s32 *sl;

		    width &= 0xffff;

		    for (j = 0; j < height; j++) {
			    hi = lo = 0;
			    sl = (mlib_s32 *)sa;
			    for (i = 0; i < cycleNumber; i++) {
				    for (k = 0; k < 32768; k++) {
					    firstAX += sl[0];
					    val = sl[1];
					    hi += val >> 16;
					    lo += val & 0xffff;
					    sl += 2;
				    }

				    firstAX +=
					(mlib_d64)lo + (mlib_d64)hi *65536.;
				    hi = lo = 0;
			    }

			    for (k = 0; k < (width >> 1); k++) {
				    firstAX += sl[0];
				    val = sl[1];
				    hi += val >> 16;
				    lo += val & 0xffff;
				    sl += 2;
			    }

			    if (width & 1)
				    firstAX += sl[0];

			    firstAX += (mlib_d64)lo + (mlib_d64)hi *65536.;

			    sa += (stride >> 2);
		    }

		    firstAX *= divider;

		    mean[0] = firstAX;
		    break;
	    }

	case 2: {

		    mlib_d64 firstAX = 0.0;
		    mlib_d64 secondAX = 0.0;
		    mlib_s32 i, cycleNumber = width >> 15;
		    mlib_s32 val, hi, lo;
		    mlib_s32 *sl;

		    width &= 0x7fff;

		    for (j = 0; j < height; j++) {
			    hi = lo = 0;
			    sl = (mlib_s32 *)sa;
			    for (i = 0; i < cycleNumber; i++) {
				    for (k = 0; k < 0x8000; k++) {
					    firstAX += sl[0];
					    val = sl[1];
					    hi += val >> 16;
					    lo += val & 0xffff;
					    sl += 2;
				    }

				    secondAX +=
					(mlib_d64)lo + (mlib_d64)hi *65536.;
				    hi = lo = 0;
			    }

			    for (k = 0; k < width; k++) {
				    firstAX += sl[0];
				    val = sl[1];
				    hi += val >> 16;
				    lo += val & 0xffff;
				    sl += 2;
			    }

			    secondAX += (mlib_d64)lo + (mlib_d64)hi *65536.;

			    sa += (stride >> 2);
		    }

		    firstAX *= divider;
		    secondAX *= divider;

		    mean[0] = firstAX;
		    mean[1] = secondAX;

		    break;
	    }

	case 3: {

		    mlib_d64 firstAX = 0.0;
		    mlib_d64 secondAX = 0.0;
		    mlib_d64 thirdAX = 0.0;
		    mlib_s32 i, cycleNumber = width >> 15;
		    mlib_s32 val, hi, lo;
		    mlib_s32 *sl;

		    width &= 0x7fff;

		    for (j = 0; j < height; j++) {
			    hi = lo = 0;
			    sl = (mlib_s32 *)sa;
			    for (i = 0; i < cycleNumber; i++) {

				    for (k = 0; k < 0x8000; k++) {
					    firstAX += sl[0];
					    secondAX += sl[1];
					    val = sl[2];
					    hi += val >> 16;
					    lo += val & 0xffff;
					    sl += 3;
				    }

				    thirdAX +=
					(mlib_d64)lo + (mlib_d64)hi *65536.;
				    hi = lo = 0;
			    }

			    for (k = 0; k < width; k++) {
				    firstAX += sl[0];
				    secondAX += sl[1];
				    val = sl[2];
				    hi += val >> 16;
				    lo += val & 0xffff;
				    sl += 3;
			    }

			    sa += (stride >> 2);
			    thirdAX += (mlib_d64)lo + (mlib_d64)hi *65536.;
		    }

		    firstAX *= divider;
		    secondAX *= divider;
		    thirdAX *= divider;

		    mean[0] = firstAX;
		    mean[1] = secondAX;
		    mean[2] = thirdAX;

		    break;
	    }

	case 4: {

		    mlib_d64 firstAX = 0.0;
		    mlib_d64 secondAX = 0.0;
		    mlib_d64 thirdAX = 0.0;
		    mlib_d64 forthAX = 0.0;
		    mlib_s32 i, cycleNumber = width >> 15;
		    mlib_s32 val, hi, lo;
		    mlib_s32 val2, hi2, lo2;
		    mlib_s32 *sl;

		    width &= 0x7fff;

		    for (j = 0; j < height; j++) {
			    hi = lo = hi2 = lo2 = 0;
			    sl = (mlib_s32 *)sa;
			    for (i = 0; i < cycleNumber; i++) {

				    for (k = 0; k < 0x8000; k++) {
					    firstAX += sl[0];
					    secondAX += sl[1];
					    val2 = sl[2];
					    hi2 += val2 >> 16;
					    lo2 += val2 & 0xffff;
					    val = sl[3];
					    hi += val >> 16;
					    lo += val & 0xffff;
					    sl += 4;
				    }

				    thirdAX +=
					(mlib_d64)lo2 + (mlib_d64)hi2 *65536.;
				    forthAX +=
					(mlib_d64)lo + (mlib_d64)hi *65536.;
				    hi = lo = hi2 = lo2 = 0;
			    }

			    for (k = 0; k < width; k++) {
				    firstAX += sl[0];
				    secondAX += sl[1];
				    val2 = sl[2];
				    hi2 += val2 >> 16;
				    lo2 += val2 & 0xffff;
				    val = sl[3];
				    hi += val >> 16;
				    lo += val & 0xffff;
				    sl += 4;
			    }

			    sa += (stride >> 2);
			    thirdAX += (mlib_d64)lo2 + (mlib_d64)hi2 *65536.;
			    forthAX += (mlib_d64)lo + (mlib_d64)hi *65536.;
		    }

		    firstAX *= divider;
		    secondAX *= divider;
		    thirdAX *= divider;
		    forthAX *= divider;

		    mean[0] = firstAX;
		    mean[1] = secondAX;
		    mean[2] = thirdAX;
		    mean[3] = forthAX;

		    break;
	    }
	}
}

/* *********************************************************** */
