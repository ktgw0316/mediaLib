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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_ImageMoment2.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageMoment2 - calculates moment of power two
 *                          for the input image.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMoment2(mlib_d64         *moment,
 *                                    const mlib_image *img)
 *
 * ARGUMENTS
 *      moment   pointer to moment array
 *      img      pointer to an image
 *
 * DESCRIPTION
 *
 *           1    w-1 h-1         2
 *      c = --- * SUM SUM(x[i][j])
 *          w*h   i = 0 j = 0
 *
 * RESTRICTION
 *      The image can have 1, 2, 3 or 4 channels.
 *      It can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageDivTables.h>
#include <mlib_s_ImageMoment2.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif
#include <mlib_sse_utils.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMoment2 = __mlib_ImageMoment2

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMoment2) mlib_ImageMoment2
    __attribute__((weak, alias("__mlib_ImageMoment2")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageMoment2(
    mlib_d64 *moment,
    const mlib_image *img)
{
/* total number of pixels */
	mlib_s32 num_pixel;
	mlib_d64 rez[4];
	mlib_d64 divider;

/* check for obvious errors */
	MLIB_IMAGE_CHECK(img);

	if (moment == NULL)
		return (MLIB_NULLPOINTER);

/* total number of pixels */
	num_pixel = mlib_ImageGetWidth(img) * mlib_ImageGetHeight(img);
	divider = 1.0 / (mlib_d64)num_pixel;

	switch (mlib_ImageGetType(img)) {
/* handle MLIB_BYTE data type of image */
	case MLIB_BYTE:

		switch (mlib_ImageGetChannels(img)) {
		case 1:
			mlib_s_ImageMoment2_U8_1(img, rez);
			break;
		case 2:
			mlib_s_ImageMoment2_U8_2(img, rez);
			break;
		case 3:
			mlib_s_ImageMoment2_U8_3(img, rez);
			break;
		case 4:
			mlib_s_ImageMoment2_U8_4(img, rez);
			break;
		default:
			return (MLIB_FAILURE);
		}

		break;

/* handle MLIB_USHORT data type of image */
	case MLIB_USHORT:

		switch (mlib_ImageGetChannels(img)) {
		case 1:
			mlib_s_ImageMoment2_U16_1(img, rez);
			break;
		case 2:
			mlib_s_ImageMoment2_U16_2(img, rez);
			break;
		case 3:
			mlib_s_ImageMoment2_U16_3(img, rez);
			break;
		case 4:
			mlib_s_ImageMoment2_U16_4(img, rez);
			break;
		default:
			return (MLIB_FAILURE);
		}
		break;

/* handle MLIB_SHORT data type of image */
	case MLIB_SHORT:
		switch (mlib_ImageGetChannels(img)) {
		case 1:
			mlib_s_ImageMoment2_S16_1(img, rez);
			break;
		case 2:
			mlib_s_ImageMoment2_S16_2(img, rez);
			break;
		case 3:
			mlib_s_ImageMoment2_S16_3(img, rez);
			break;
		case 4:
			mlib_s_ImageMoment2_S16_4(img, rez);
			break;
		default:
			return (MLIB_FAILURE);
		}
		break;

/* handle MLIB_INT data type of image */
	case MLIB_INT:
		switch (mlib_ImageGetChannels(img)) {
		case 1:
			mlib_s_ImageMoment2_S32_1(img, rez);
			break;
		case 2:
			mlib_s_ImageMoment2_S32_2(img, rez);
			break;
		case 3:
			mlib_s_ImageMoment2_S32_3(img, rez);
			break;
		case 4:
			mlib_s_ImageMoment2_S32_4(img, rez);
			break;
		default:
			return (MLIB_FAILURE);
		}
		break;

/* discard any other data types */
	default:
		return (MLIB_FAILURE);
	}

	switch (mlib_ImageGetChannels(img)) {
	case 1:
		moment[0] = (rez[0] + rez[1] + rez[2] + rez[3]) * divider;
		break;

	case 2:
		moment[0] = (rez[0] + rez[2]) * divider;
		moment[1] = (rez[1] + rez[3]) * divider;
		break;

	case 4:
		moment[3] = rez[3] * divider;
	case 3:
		moment[0] = rez[0] * divider;
		moment[1] = rez[1] * divider;
		moment[2] = rez[2] * divider;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_s_ImageMoment2_U8_1(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_u8 *psrc = (mlib_u8 *)mlib_ImageGetData(img);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img);
	mlib_s32 src_chan = mlib_ImageGetChannels(img);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img);

/* indices */
	mlib_s32 i, j, l;

	if (src_stride == size_row) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_s64 sumbuff[2];
	mlib_s32 db_num;
	__m128i* sl;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {
		j = 0;

		for (j = 0; (j < size_row) &&
		    ((mlib_addr)(psrc +j) & 15); j++) {
			buffer0 += psrc[j] * psrc[j];
		}

		db_num = ((size_row - j) >> 4);
		sl = (__m128i *)(psrc + j);
		__m128i load, sdl, sdh, sdm1, sdm2, sd1, sd2, sd3, sd4;
		__m128i sum1, sum2;
		sum1 = _mm_setzero_si128();
		sum2 = _mm_setzero_si128();
		j += (db_num << 4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			load = _mm_load_si128(sl);
			sdl = _mm_unpacklo_epi8(load, x_zero);
			sdh = _mm_unpackhi_epi8(load, x_zero);
			sdm1 = _mm_madd_epi16(sdl, sdl);
			sdm2 = _mm_madd_epi16(sdh, sdh);
			sd1 = _mm_unpacklo_epi32(sdm1, x_zero);
			sd2 = _mm_unpackhi_epi32(sdm1, x_zero);
			sd3 = _mm_unpacklo_epi32(sdm2, x_zero);
			sd4 = _mm_unpackhi_epi32(sdm2, x_zero);
			sum1 = _mm_add_epi64(sum1, sd1);
			sum2 = _mm_add_epi64(sum2, sd2);
			sum1 = _mm_add_epi64(sum1, sd3);
			sum2 = _mm_add_epi64(sum2, sd4);
			sl++;
		}

		sum1 = _mm_add_epi64(sum1, sum2);
		_mm_storeu_si128((void *)sumbuff, sum1);

		for (; j < size_row; j++) {
			buffer0 += psrc[j] * psrc[j];
		}

		buffer0 += sumbuff[0];
		buffer1 += sumbuff[1];
		psrc += src_stride;
	}

	rez[0] = buffer0;
	rez[1] = buffer1;
	rez[2] = 0.0;
	rez[3] = 0.0;
}

/* *********************************************************** */

void
mlib_s_ImageMoment2_U8_2(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_u8 *psrc = (mlib_u8 *)mlib_ImageGetData(img);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) << 1;

/* indices */
	mlib_s32 i, j, l;

	if (src_stride == size_row) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_s64 sumbuff[2];
	__m128i* sl;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 alignment = 1;
		mlib_s32 db_num;

		if ((mlib_addr)psrc & 1) {
			alignment = 0;
			buffer0 += psrc[0] * psrc[0];

			for (j = 1; (j < (size_row - 2)) &&
			    ((mlib_addr)(psrc + j) & 15); j += 2) {
				buffer1 += psrc[j + 0] * psrc[j + 0];
				buffer0 += psrc[j + 1] * psrc[j + 1];
			}
		} else {
			for (j = 0; (j < size_row) &&
			    ((mlib_addr)(psrc + j) & 15); j += 2) {
				buffer0 += psrc[j + 0] * psrc[j + 0];
				buffer1 += psrc[j + 1] * psrc[j + 1];
			}
		}

		db_num = ((size_row - j) >> 4);
		sl = (__m128i *)(psrc + j);
		__m128i load, load0, load1, sdl0, sdh0, sdl1, sdh1;
		__m128i sdm0, sdm1, sdm2, sdm3;
		__m128i sd0, sd1, sd2, sd3, sd4, sd5, sd6, sd7;
		__m128i sum1, sum2;
		sum1 = _mm_setzero_si128();
		sum2 = _mm_setzero_si128();

		j += (db_num << 4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
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

			sd0 = _mm_unpacklo_epi32(sdm0, x_zero);
			sd1 = _mm_unpackhi_epi32(sdm0, x_zero);
			sd2 = _mm_unpacklo_epi32(sdm1, x_zero);
			sd3 = _mm_unpackhi_epi32(sdm1, x_zero);
			sd4 = _mm_unpacklo_epi32(sdm2, x_zero);
			sd5 = _mm_unpackhi_epi32(sdm2, x_zero);
			sd6 = _mm_unpacklo_epi32(sdm3, x_zero);
			sd7 = _mm_unpackhi_epi32(sdm3, x_zero);

			sum1 = _mm_add_epi64(sum1, sd0);
			sum2 = _mm_add_epi64(sum2, sd1);
			sum1 = _mm_add_epi64(sum1, sd2);
			sum2 = _mm_add_epi64(sum2, sd3);
			sum1 = _mm_add_epi64(sum1, sd4);
			sum2 = _mm_add_epi64(sum2, sd5);
			sum1 = _mm_add_epi64(sum1, sd6);
			sum2 = _mm_add_epi64(sum2, sd7);

			sl++;
		}

		sum1 = _mm_add_epi64(sum1, sum2);
		_mm_storeu_si128((void *)sumbuff, sum1);

		if (alignment) {
			buffer0 += sumbuff[0];
			buffer1 += sumbuff[1];

			for (; j < size_row; j += 2) {
				buffer0 += psrc[j + 0] * psrc[j + 0];
				buffer1 += psrc[j + 1] * psrc[j + 1];
			}
		} else {
			buffer1 += sumbuff[0];
			buffer0 += sumbuff[1];

			for (; j < (size_row - 1); j += 2) {
				buffer1 += psrc[j + 0] * psrc[j + 0];
				buffer0 += psrc[j + 1] * psrc[j + 1];
			}

			buffer1 += psrc[j] * psrc[j];
		}

		psrc += src_stride;
	}

	rez[0] = buffer0;
	rez[1] = buffer1;
	rez[2] = 0.0;
	rez[3] = 0.0;
}

/* *********************************************************** */

void
mlib_s_ImageMoment2_U8_4(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_u8 *psrc = (mlib_u8 *)mlib_ImageGetData(img);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img);
	mlib_s32 src_chan = mlib_ImageGetChannels(img);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) << 2;

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
	mlib_s64 sumbuff[4];
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
			alignment -= 4;
		}

		if (j < (size_row - 3)) {
			if ((alignment == 3)) {
				buffer0 += psrc[j + 0] * psrc[j + 0];
				buffer1 += psrc[j + 1] * psrc[j + 1];
				buffer2 += psrc[j + 2] * psrc[j + 2];
				j += 3;
				sign = 1;
			} else if ((alignment == 2)) {
				buffer0 += psrc[j + 0] * psrc[j + 0];
				buffer1 += psrc[j + 1] * psrc[j + 1];
				j += 2;
				sign = 2;
			} else if ((alignment == 1)) {
				buffer0 += psrc[j + 0] * psrc[j + 0];
				j += 1;
				sign = 3;
			}
		}

		db_num = ((size_row - j) >> 4);
		sl = (__m128i *)(psrc + j);
		__m128i load, load0, load1, sdl0, sdh0, sdl1, sdh1;
		__m128i sdm0, sdm1, sdm2, sdm3;
		__m128i sd0, sd1, sd2, sd3, sd4, sd5, sd6, sd7;
		__m128i sum1, sum2;
		sum1 = _mm_setzero_si128();
		sum2 = _mm_setzero_si128();
		j += (db_num << 4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
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

			sd0 = _mm_unpacklo_epi32(sdm0, x_zero);
			sd1 = _mm_unpackhi_epi32(sdm0, x_zero);
			sd2 = _mm_unpacklo_epi32(sdm1, x_zero);
			sd3 = _mm_unpackhi_epi32(sdm1, x_zero);
			sd4 = _mm_unpacklo_epi32(sdm2, x_zero);
			sd5 = _mm_unpackhi_epi32(sdm2, x_zero);
			sd6 = _mm_unpacklo_epi32(sdm3, x_zero);
			sd7 = _mm_unpackhi_epi32(sdm3, x_zero);

			sum1 = _mm_add_epi64(sum1, sd0);
			sum2 = _mm_add_epi64(sum2, sd1);
			sum1 = _mm_add_epi64(sum1, sd2);
			sum2 = _mm_add_epi64(sum2, sd3);
			sum1 = _mm_add_epi64(sum1, sd4);
			sum2 = _mm_add_epi64(sum2, sd5);
			sum1 = _mm_add_epi64(sum1, sd6);
			sum2 = _mm_add_epi64(sum2, sd7);

			sl++;
		}

		_mm_storeu_si128((void *)sumbuff, sum1);
		_mm_storeu_si128((void *)(sumbuff + 2), sum2);

		for (; j < (size_row - 3); j += 4) {
			sumbuff[0] += psrc[j + 0] * psrc[j + 0];
			sumbuff[1] += psrc[j + 1] * psrc[j + 1];
			sumbuff[2] += psrc[j + 2] * psrc[j + 2];
			sumbuff[3] += psrc[j + 3] * psrc[j + 3];
		}

		if (sign == 3) {
			buffer1 += psrc[j + 0] * psrc[j + 0];
			buffer2 += psrc[j + 1] * psrc[j + 1];
			buffer3 += psrc[j + 2] * psrc[j + 2];
			j += 3;

			buffer0 += sumbuff[3];
			buffer1 += sumbuff[0];
			buffer2 += sumbuff[1];
			buffer3 += sumbuff[2];
		} else if (sign == 2) {
			buffer2 += psrc[j + 0] * psrc[j + 0];
			buffer3 += psrc[j + 1] * psrc[j + 1];
			j += 2;

			buffer0 += sumbuff[2];
			buffer1 += sumbuff[3];
			buffer2 += sumbuff[0];
			buffer3 += sumbuff[1];
		} else if (sign == 1) {
			buffer3 += psrc[j + 0] * psrc[j + 0];
			j++;

			buffer0 += sumbuff[1];
			buffer1 += sumbuff[2];
			buffer2 += sumbuff[3];
			buffer3 += sumbuff[0];
		} else if (sign == 0) {

			buffer0 += sumbuff[0];
			buffer1 += sumbuff[1];
			buffer2 += sumbuff[2];
			buffer3 += sumbuff[3];
		}

		psrc += src_stride;
	}

	rez[0] = buffer0;
	rez[1] = buffer1;
	rez[2] = buffer2;
	rez[3] = buffer3;
}

/* *********************************************************** */

#define	MUL_U8(x)	(mlib_U82D64[(x)] * mlib_U82D64[(x)])

/* *********************************************************** */

/* Pure C implementation of this function */

void
mlib_s_ImageMoment2_U8_3(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_u8 *psrc = (mlib_u8 *)mlib_ImageGetData(img);

/* accumulators by channels */
	mlib_d64 c1, c2, c3;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) * 3;

/* indices */
	mlib_s32 i, j;

	if (src_stride == size_row) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}

	c1 = c2 = c3 = 0;
	for (i = 0; i < height; i++) {
		j = 0;

		if ((mlib_addr)psrc & 1) {
			c1 += MUL_U8(psrc[j]);
			c2 += MUL_U8(psrc[j + 1]);
			c3 += MUL_U8(psrc[j + 2]);
			j += 3;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (size_row - 6); j += 6) {
			mlib_u32 u1, u2;

			u1 = ((mlib_u16 *)(psrc + j))[0];
			u2 = ((mlib_u16 *)(psrc + j))[1];
#ifdef _LITTLE_ENDIAN
			c1 += MUL_U8(u1 & 0xff);
			c2 += MUL_U8(u1 >> 8);
			c3 += MUL_U8(u2 & 0xff);
			c1 += MUL_U8(u2 >> 8);
#else /* _LITTLE_ENDIAN */
			c1 += MUL_U8(u1 >> 8);
			c2 += MUL_U8(u1 & 0xff);
			c3 += MUL_U8(u2 >> 8);
			c1 += MUL_U8(u2 & 0xff);
#endif /* _LITTLE_ENDIAN */
			c2 += MUL_U8(psrc[j + 4]);
			c3 += MUL_U8(psrc[j + 5]);
		}

		if (j < size_row) {
			c1 += MUL_U8(psrc[j]);
			c2 += MUL_U8(psrc[j + 1]);
			c3 += MUL_U8(psrc[j + 2]);
		}

		psrc += src_stride;
	}

	rez[0] = c1;
	rez[1] = c2;
	rez[2] = c3;
}

/* *********************************************************** */

void
mlib_s_ImageMoment2_U16_1(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_u16 *psrc = (mlib_u16 *)mlib_ImageGetData(img);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 1;
	mlib_s32 src_chan = mlib_ImageGetChannels(img);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img);

/* indices */
	mlib_s32 i, j, l;

	if (src_stride == size_row) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_s32 db_num;
	__m128i* sl;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {
		j = 0;

		for (j = 0; (j < size_row) &&
		    ((mlib_addr)(psrc +j) & 15); j++) {
			buffer0 += (mlib_d64)psrc[j] * psrc[j];
		}

		db_num = ((size_row - j) >> 3);
		sl = (__m128i *)(psrc + j);
		__m128i load, sdl, sdh, sd1, sd2, sd3, sd4, sd5, sd6;
		__m128i sum1, sum2, sum3, sum4;
		mlib_s64 sumbuff[2];
		sum1 = _mm_setzero_si128();
		sum2 = _mm_setzero_si128();
		sum3 = _mm_setzero_si128();
		sum4 = _mm_setzero_si128();
		j += (db_num << 3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
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
			sum3 = _mm_add_epi64(sum3, sd4);
			sum2 = _mm_add_epi64(sum2, sd5);
			sum4 = _mm_add_epi64(sum4, sd6);
			sl++;
		}

		sum1 = _mm_add_epi64(sum1, sum2);
		sum3 = _mm_add_epi64(sum3, sum4);
		sum1 = _mm_add_epi64(sum1, sum3);
		_mm_storeu_si128((void *)sumbuff, sum1);

		for (; j < size_row; j++) {
			buffer0 += (mlib_d64)psrc[j] * psrc[j];
		}

		buffer0 += sumbuff[0];
		buffer1 += sumbuff[1];
		psrc += src_stride;
	}

	rez[0] = buffer0;
	rez[1] = buffer1;
	rez[2] = 0.0;
	rez[3] = 0.0;
}

/* *********************************************************** */

void
mlib_s_ImageMoment2_U16_2(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_u16 *psrc = (mlib_u16 *)mlib_ImageGetData(img);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 1;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) << 1;

/* indices */
	mlib_s32 i, j, l;

	if (src_stride == size_row) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_s64 sumbuff[2];
	__m128i* sl;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 alignment = 1;
		mlib_s32 db_num;
		sumbuff[0] = sumbuff[1] = 0;

		if ((mlib_addr)psrc & 1) {
			alignment = 0;
			buffer0 += (mlib_d64)psrc[0] * psrc[0];

			for (j = 1; (j < (size_row - 2)) &&
			    ((mlib_addr)(psrc + j) & 15); j += 2) {
				buffer1 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer0 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			}
		} else {
			for (j = 0; (j < size_row) &&
			    ((mlib_addr)(psrc + j) & 15); j += 2) {
				buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			}
		}

		db_num = ((size_row - j) >> 3);
		sl = (__m128i *)(psrc + j);
		__m128i load, sdl, sdh, sd1, sd2, sd3, sd4, sd5, sd6;
		__m128i sum1, sum2;
		sum1 = _mm_setzero_si128();
		sum2 = _mm_setzero_si128();
		j += (db_num << 3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
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
			sl++;
		}

		sum1 = _mm_add_epi64(sum1, sum2);
		_mm_storeu_si128((void *)sumbuff, sum1);

		if (alignment) {
			buffer0 += sumbuff[0];
			buffer1 += sumbuff[1];

			for (; j < size_row; j += 2) {
				buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			}
		} else {
			buffer1 += sumbuff[0];
			buffer0 += sumbuff[1];

			for (; j < (size_row - 1); j += 2) {
				buffer1 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer0 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			}

			buffer1 += (mlib_d64)psrc[j] * psrc[j];
		}

		psrc += src_stride;
	}

	rez[0] = buffer0;
	rez[1] = buffer1;
	rez[2] = 0.0;
	rez[3] = 0.0;
}

/* *********************************************************** */

void
mlib_s_ImageMoment2_U16_3(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_u16 *psrc = (mlib_u16 *)mlib_ImageGetData(img);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 1;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) * 3;

/* indices */
	mlib_s32 i, j, l;
	mlib_s32 a1, a2, a3;
	mlib_s32 ch0, ch1, ch2;

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

	mlib_s32 db_num;
	__m128i* sl;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 sign = 0;
		mlib_s32 alignment =
			((16 - ((mlib_addr)psrc & 15)) & 15) >> 1;

		for (j = 0;
		    (j < (size_row - 3)) && (j <= (alignment - 3));
		    j += 3) {
			buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
			buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			buffer2 += (mlib_d64)psrc[j + 2] * psrc[j + 2];
		}

		if ((alignment - j) == 2) {
			buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
			buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			j += 2;
			sign = 1;
		} else if ((alignment - j) == 1) {
			buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
			j += 1;
			sign = 2;
		}

		db_num = ((size_row - j) / 24);
		sl = (__m128i *)(psrc + j);
		__m128i load0, load1, load2;
		__m128i sdl0, sdh0, sdl1, sdh1, sdl2, sdh2;
		__m128i sd1, sd2, sd3, sd4, sd5, sd6;
		__m128i sd10, sd20, sd30, sd40, sd50, sd60;
		__m128i sd11, sd21, sd31, sd41, sd51, sd61;
		__m128i sum1, sum2, sum3;
		mlib_s64 sumbuff1[2], sumbuff2[2], sumbuff3[2];
		sum1 = _mm_setzero_si128();
		sum2 = _mm_setzero_si128();
		sum3 = _mm_setzero_si128();

		j += (db_num * 24);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			load0 = _mm_load_si128(sl);
			load1 = _mm_load_si128(sl + 1);
			load2 = _mm_load_si128(sl + 2);

			sdl0 = _mm_mullo_epi16(load0, load0);
			sdh0 = _mm_mulhi_epu16(load0, load0);
			sd1 = _mm_unpacklo_epi16(sdl0, sdh0);
			sd2 = _mm_unpackhi_epi16(sdl0, sdh0);

			sdl1 = _mm_mullo_epi16(load1, load1);
			sdh1 = _mm_mulhi_epu16(load1, load1);
			sd3 = _mm_unpacklo_epi16(sdl1, sdh1);
			sd4 = _mm_unpackhi_epi16(sdl1, sdh1);

			sdl2 = _mm_mullo_epi16(load2, load2);
			sdh2 = _mm_mulhi_epu16(load2, load2);
			sd5 = _mm_unpacklo_epi16(sdl2, sdh2);
			sd6 = _mm_unpackhi_epi16(sdl2, sdh2);

			sd10 = _mm_unpacklo_epi32(sd1, x_zero);
			sd11 = _mm_unpackhi_epi32(sd1, x_zero);
			sd20 = _mm_unpacklo_epi32(sd2, x_zero);
			sd21 = _mm_unpackhi_epi32(sd2, x_zero);

			sd30 = _mm_unpacklo_epi32(sd3, x_zero);
			sd31 = _mm_unpackhi_epi32(sd3, x_zero);
			sd40 = _mm_unpacklo_epi32(sd4, x_zero);
			sd41 = _mm_unpackhi_epi32(sd4, x_zero);

			sd50 = _mm_unpacklo_epi32(sd5, x_zero);
			sd51 = _mm_unpackhi_epi32(sd5, x_zero);
			sd60 = _mm_unpacklo_epi32(sd6, x_zero);
			sd61 = _mm_unpackhi_epi32(sd6, x_zero);

			sum1 = _mm_add_epi64(sum1, sd10);
			sum2 = _mm_add_epi64(sum2, sd11);
			sum3 = _mm_add_epi64(sum3, sd20);
			sum1 = _mm_add_epi64(sum1, sd21);
			sum2 = _mm_add_epi64(sum2, sd30);
			sum3 = _mm_add_epi64(sum3, sd31);
			sum1 = _mm_add_epi64(sum1, sd40);
			sum2 = _mm_add_epi64(sum2, sd41);
			sum3 = _mm_add_epi64(sum3, sd50);
			sum1 = _mm_add_epi64(sum1, sd51);
			sum2 = _mm_add_epi64(sum2, sd60);
			sum3 = _mm_add_epi64(sum3, sd61);

			sl += 3;
		}

		_mm_storeu_si128((void *)sumbuff1, sum1);
		_mm_storeu_si128((void *)sumbuff2, sum2);
		_mm_storeu_si128((void *)sumbuff3, sum3);

		buffer3 = sumbuff1[0] + sumbuff2[1];
		buffer4 = sumbuff1[1] + sumbuff3[0];
		buffer5 = sumbuff2[0] + sumbuff3[1];

		for (; j < (size_row - 2); j += 3) {
			buffer3 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
			buffer4 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			buffer5 += (mlib_d64)psrc[j + 2] * psrc[j + 2];
		}

		if (sign == 2) {
			buffer1 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
			buffer2 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			buffer0 += buffer5;
			buffer1 += buffer3;
			buffer2 += buffer4;
		} else if (sign == 1) {
			buffer2 += (mlib_d64)psrc[j] * psrc[j];
			buffer0 += buffer4;
			buffer1 += buffer5;
			buffer2 += buffer3;
		} else if (sign == 0) {
			buffer0 += buffer3;
			buffer1 += buffer4;
			buffer2 += buffer5;
		}

		psrc += src_stride;
	}

	rez[0] = buffer0;
	rez[1] = buffer1;
	rez[2] = buffer2;
}

/* *********************************************************** */

void
mlib_s_ImageMoment2_U16_4(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_u16 *psrc = (mlib_u16 *)mlib_ImageGetData(img);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 1;
	mlib_s32 src_chan = mlib_ImageGetChannels(img);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) << (src_chan >> 1);

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
	mlib_s64 sumbuff[4];
	mlib_s32 db_num;
	__m128i* sl;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 sign = 0;
		mlib_s32 alignment =
		    ((16 - ((mlib_addr)psrc & 15)) & 15) >> 1;

		sumbuff[0] = sumbuff[1] = sumbuff[2] = sumbuff[3] = 0;

		for (; (j < size_row - 3) && (alignment >= 4); j += 4) {
			buffer0 += (mlib_d64)psrc[0] * psrc[0];
			buffer1 += (mlib_d64)psrc[1] * psrc[1];
			buffer2 += (mlib_d64)psrc[2] * psrc[2];
			buffer3 += (mlib_d64)psrc[3] * psrc[3];
			alignment -= 4;
		}

		if (j < (size_row - 3)) {
			if ((alignment == 3)) {
				buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
				buffer2 += (mlib_d64)psrc[j + 2] * psrc[j + 2];
				j += 3;
				sign = 1;
			} else if ((alignment == 2)) {
				buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
				j += 2;
				sign = 2;
			} else if ((alignment == 1)) {
				buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				j += 1;
				sign = 3;
			}
		}

		db_num = ((size_row - j) >> 3);
		sl = (__m128i *)(psrc + j);
		__m128i load, sdl, sdh, sd1, sd2, sd3, sd4, sd5, sd6;
		__m128i sum1, sum2;
		sum1 = _mm_setzero_si128();
		sum2 = _mm_setzero_si128();
		j += (db_num << 3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
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
			sl++;
		}

		_mm_storeu_si128((void *)sumbuff, sum1);
		_mm_storeu_si128((void *)(sumbuff + 2), sum2);

		for (; j < (size_row - 3); j += 4) {
			sumbuff[0] += (mlib_d64)psrc[j + 0] * psrc[j + 0];
			sumbuff[1] += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			sumbuff[2] += (mlib_d64)psrc[j + 2] * psrc[j + 2];
			sumbuff[3] += (mlib_d64)psrc[j + 3] * psrc[j + 3];
		}

		if (sign == 3) {
			buffer1 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
			buffer2 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			buffer3 += (mlib_d64)psrc[j + 2] * psrc[j + 2];
			j += 3;

			buffer0 += sumbuff[3];
			buffer1 += sumbuff[0];
			buffer2 += sumbuff[1];
			buffer3 += sumbuff[2];
		} else if (sign == 2) {
			buffer2 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
			buffer3 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			j += 2;

			buffer0 += sumbuff[2];
			buffer1 += sumbuff[3];
			buffer2 += sumbuff[0];
			buffer3 += sumbuff[1];
		} else if (sign == 1) {
			buffer3 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
			j++;

			buffer0 += sumbuff[1];
			buffer1 += sumbuff[2];
			buffer2 += sumbuff[3];
			buffer3 += sumbuff[0];
		} else if (sign == 0) {
			buffer0 += sumbuff[0];
			buffer1 += sumbuff[1];
			buffer2 += sumbuff[2];
			buffer3 += sumbuff[3];
		}

		psrc += src_stride;
	}

	rez[0] = buffer0;
	rez[1] = buffer1;
	rez[2] = buffer2;
	rez[3] = buffer3;
}

/* *********************************************************** */

void
mlib_s_ImageMoment2_S16_1(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_s16 *psrc = (mlib_s16 *)mlib_ImageGetData(img);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 1;
	mlib_s32 src_chan = mlib_ImageGetChannels(img);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img);

/* indices */
	mlib_s32 i, j, l;

	if (src_stride == size_row) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_s64 sumbuff[2];
	mlib_s32 db_num;
	__m128i* sl;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {
		j = 0;
		sumbuff[0] = sumbuff[1] = 0;

		for (j = 0; (j < size_row) &&
		    ((mlib_addr)(psrc +j) & 15); j++) {
			buffer0 += psrc[j] * psrc[j];
		}

		db_num = ((size_row - j) >> 3);
		sl = (__m128i *)(psrc + j);
		__m128i load, sdl, sdh, sd1, sd2;
		__m128i sum1, sum2;
		sum1 = _mm_setzero_si128();
		sum2 = _mm_setzero_si128();
		j += (db_num << 3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			load = _mm_load_si128(sl);
			sdl = _mm_madd_epi16(load, load);
			sd1 = _mm_unpacklo_epi32(sdl, x_zero);
			sd2 = _mm_unpackhi_epi32(sdl, x_zero);
			sum1 = _mm_add_epi64(sum1, sd1);
			sum2 = _mm_add_epi64(sum2, sd2);
			sl++;
		}

		sum1 = _mm_add_epi64(sum1, sum2);
		_mm_storeu_si128((void *)sumbuff, sum1);

		for (; j < size_row; j++) {
			buffer0 += psrc[j] * psrc[j];
		}

		buffer0 += sumbuff[0];
		buffer1 += sumbuff[1];
		psrc += src_stride;
	}

	rez[0] = buffer0;
	rez[1] = buffer1;
	rez[2] = 0.0;
	rez[3] = 0.0;
}

/* *********************************************************** */

void
mlib_s_ImageMoment2_S16_2(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_s16 *psrc = (mlib_s16 *)mlib_ImageGetData(img);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 1;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) << 1;

/* indices */
	mlib_s32 i, j, l;

	if (src_stride == size_row) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_s64 sumbuff[2];
	__m128i* sl;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 alignment = 1;
		mlib_s32 db_num;

		if ((mlib_addr)psrc & 1) {
			alignment = 0;
			buffer0 += psrc[0] * psrc[0];

			for (j = 1; (j < (size_row - 2)) &&
			    ((mlib_addr)(psrc + j) & 15); j += 2) {
				buffer1 += psrc[j + 0] * psrc[j + 0];
				buffer0 += psrc[j + 1] * psrc[j + 1];
			}
		} else {
			for (j = 0; (j < size_row) &&
			    ((mlib_addr)(psrc + j) & 15); j += 2) {
				buffer0 += psrc[j + 0] * psrc[j + 0];
				buffer1 += psrc[j + 1] * psrc[j + 1];
			}
		}

		db_num = ((size_row - j) >> 3);
		sl = (__m128i *)(psrc + j);
		__m128i load, sdl, sdh, sd1, sd2, sd3, sd4, sd5, sd6;
		__m128i sum1, sum2;
		sum1 = _mm_setzero_si128();
		sum2 = _mm_setzero_si128();

		j += (db_num << 3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
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

			sl++;
		}

		sum1 = _mm_add_epi64(sum1, sum2);
		_mm_storeu_si128((void *)sumbuff, sum1);

		if (alignment) {
			buffer0 += sumbuff[0];
			buffer1 += sumbuff[1];

			for (; j < size_row; j += 2) {
				buffer0 += psrc[j + 0] * psrc[j + 0];
				buffer1 += psrc[j + 1] * psrc[j + 1];
			}
		} else {
			buffer1 += sumbuff[0];
			buffer0 += sumbuff[1];

			for (; j < (size_row - 1); j += 2) {
				buffer1 += psrc[j + 0] * psrc[j + 0];
				buffer0 += psrc[j + 1] * psrc[j + 1];
			}

			buffer1 += psrc[j] * psrc[j];
		}

		psrc += src_stride;
	}

	rez[0] = buffer0;
	rez[1] = buffer1;
	rez[2] = 0.0;
	rez[3] = 0.0;
}

/* *********************************************************** */

void
mlib_s_ImageMoment2_S16_4(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_s16 *psrc = (mlib_s16 *)mlib_ImageGetData(img);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 1;
	mlib_s32 src_chan = mlib_ImageGetChannels(img);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) << (src_chan >> 1);

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
	mlib_s64 sumbuff[4];
	mlib_s32 db_num;
	__m128i* sl;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 sign = 0;
		mlib_s32 alignment =
		    ((16 - ((mlib_addr)psrc & 15)) & 15) >> 1;

		for (; (j < size_row - 3) && (alignment >= 4); j += 4) {
			buffer0 += psrc[0] * psrc[0];
			buffer1 += psrc[1] * psrc[1];
			buffer2 += psrc[2] * psrc[2];
			buffer3 += psrc[3] * psrc[3];
			alignment -= 4;
		}

		if (j < (size_row - 3)) {
			if ((alignment == 3)) {
				buffer0 += psrc[j + 0] * psrc[j + 0];
				buffer1 += psrc[j + 1] * psrc[j + 1];
				buffer2 += psrc[j + 2] * psrc[j + 2];
				j += 3;
				sign = 1;
			} else if ((alignment == 2)) {
				buffer0 += psrc[j + 0] * psrc[j + 0];
				buffer1 += psrc[j + 1] * psrc[j + 1];
				j += 2;
				sign = 2;
			} else if ((alignment == 1)) {
				buffer0 += psrc[j + 0] * psrc[j + 0];
				j += 1;
				sign = 3;
			}
		}

		db_num = ((size_row - j) >> 3);
		sl = (__m128i *)(psrc + j);
		__m128i load, sdl, sdh, sd1, sd2, sd3, sd4, sd5, sd6;
		__m128i sum1, sum2;
		sum1 = _mm_setzero_si128();
		sum2 = _mm_setzero_si128();
		j += (db_num << 3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
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

			sl++;
		}

		_mm_storeu_si128((void *)sumbuff, sum1);
		_mm_storeu_si128((void *)(sumbuff + 2), sum2);

		for (; j < (size_row - 3); j += 4) {
			sumbuff[0] += psrc[j + 0] * psrc[j + 0];
			sumbuff[1] += psrc[j + 1] * psrc[j + 1];
			sumbuff[2] += psrc[j + 2] * psrc[j + 2];
			sumbuff[3] += psrc[j + 3] * psrc[j + 3];
		}

		if (sign == 3) {
			buffer1 += psrc[j + 0] * psrc[j + 0];
			buffer2 += psrc[j + 1] * psrc[j + 1];
			buffer3 += psrc[j + 2] * psrc[j + 2];
			j += 3;

			buffer0 += sumbuff[3];
			buffer1 += sumbuff[0];
			buffer2 += sumbuff[1];
			buffer3 += sumbuff[2];
		} else if (sign == 2) {
			buffer2 += psrc[j + 0] * psrc[j + 0];
			buffer3 += psrc[j + 1] * psrc[j + 1];
			j += 2;

			buffer0 += sumbuff[2];
			buffer1 += sumbuff[3];
			buffer2 += sumbuff[0];
			buffer3 += sumbuff[1];
		} else if (sign == 1) {
			buffer3 += psrc[j + 0] * psrc[j + 0];
			j++;

			buffer0 += sumbuff[1];
			buffer1 += sumbuff[2];
			buffer2 += sumbuff[3];
			buffer3 += sumbuff[0];
		} else if (sign == 0) {

			buffer0 += sumbuff[0];
			buffer1 += sumbuff[1];
			buffer2 += sumbuff[2];
			buffer3 += sumbuff[3];
		}

		psrc += src_stride;
	}

	rez[0] = buffer0;
	rez[1] = buffer1;
	rez[2] = buffer2;
	rez[3] = buffer3;
}

/* *********************************************************** */

void
mlib_s_ImageMoment2_S16_3(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_s16 *psrc = (mlib_s16 *)mlib_ImageGetData(img);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 1;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) * 3;

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
	mlib_d64 buffer4 = 0.0;
	mlib_d64 buffer5 = 0.0;

	mlib_s32 db_num;
	__m128i* sl;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 sign = 0;
		mlib_s32 alignment =
			((16 - ((mlib_addr)psrc & 15)) & 15) >> 1;

		for (j = 0;
		    (j < (size_row - 3)) && (j <= (alignment - 3));
		    j += 3) {
			buffer0 += psrc[j + 0] * psrc[j + 0];
			buffer1 += psrc[j + 1] * psrc[j + 1];
			buffer2 += psrc[j + 2] * psrc[j + 2];
		}

		if ((alignment - j) == 2) {
			buffer0 += psrc[j + 0] * psrc[j + 0];
			buffer1 += psrc[j + 1] * psrc[j + 1];
			j += 2;
			sign = 1;
		} else if ((alignment - j) == 1) {
			buffer0 += psrc[j + 0] * psrc[j + 0];
			j += 1;
			sign = 2;
		}

		db_num = ((size_row - j) / 24);
		sl = (__m128i *)(psrc + j);
		__m128i load0, load1, load2;
		__m128i sdl0, sdh0, sdl1, sdh1, sdl2, sdh2;
		__m128i sd1, sd2, sd3, sd4, sd5, sd6;
		__m128i sd10, sd20, sd30, sd40, sd50, sd60;
		__m128i sd11, sd21, sd31, sd41, sd51, sd61;
		__m128i sum1, sum2, sum3;
		mlib_s64 sumbuff1[2], sumbuff2[2], sumbuff3[2];
		sum1 = _mm_setzero_si128();
		sum2 = _mm_setzero_si128();
		sum3 = _mm_setzero_si128();

		j += (db_num * 24);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			load0 = _mm_load_si128(sl);
			load1 = _mm_load_si128(sl + 1);
			load2 = _mm_load_si128(sl + 2);

			sdl0 = _mm_mullo_epi16(load0, load0);
			sdh0 = _mm_mulhi_epi16(load0, load0);
			sd1 = _mm_unpacklo_epi16(sdl0, sdh0);
			sd2 = _mm_unpackhi_epi16(sdl0, sdh0);

			sdl1 = _mm_mullo_epi16(load1, load1);
			sdh1 = _mm_mulhi_epi16(load1, load1);
			sd3 = _mm_unpacklo_epi16(sdl1, sdh1);
			sd4 = _mm_unpackhi_epi16(sdl1, sdh1);

			sdl2 = _mm_mullo_epi16(load2, load2);
			sdh2 = _mm_mulhi_epi16(load2, load2);
			sd5 = _mm_unpacklo_epi16(sdl2, sdh2);
			sd6 = _mm_unpackhi_epi16(sdl2, sdh2);

			sd10 = _mm_unpacklo_epi32(sd1, x_zero);
			sd11 = _mm_unpackhi_epi32(sd1, x_zero);
			sd20 = _mm_unpacklo_epi32(sd2, x_zero);
			sd21 = _mm_unpackhi_epi32(sd2, x_zero);

			sd30 = _mm_unpacklo_epi32(sd3, x_zero);
			sd31 = _mm_unpackhi_epi32(sd3, x_zero);
			sd40 = _mm_unpacklo_epi32(sd4, x_zero);
			sd41 = _mm_unpackhi_epi32(sd4, x_zero);

			sd50 = _mm_unpacklo_epi32(sd5, x_zero);
			sd51 = _mm_unpackhi_epi32(sd5, x_zero);
			sd60 = _mm_unpacklo_epi32(sd6, x_zero);
			sd61 = _mm_unpackhi_epi32(sd6, x_zero);

			sum1 = _mm_add_epi64(sum1, sd10);
			sum2 = _mm_add_epi64(sum2, sd11);
			sum3 = _mm_add_epi64(sum3, sd20);
			sum1 = _mm_add_epi64(sum1, sd21);
			sum2 = _mm_add_epi64(sum2, sd30);
			sum3 = _mm_add_epi64(sum3, sd31);
			sum1 = _mm_add_epi64(sum1, sd40);
			sum2 = _mm_add_epi64(sum2, sd41);
			sum3 = _mm_add_epi64(sum3, sd50);
			sum1 = _mm_add_epi64(sum1, sd51);
			sum2 = _mm_add_epi64(sum2, sd60);
			sum3 = _mm_add_epi64(sum3, sd61);

			sl += 3;
		}

		_mm_storeu_si128((void *)sumbuff1, sum1);
		_mm_storeu_si128((void *)sumbuff2, sum2);
		_mm_storeu_si128((void *)sumbuff3, sum3);

		buffer3 = sumbuff1[0] + sumbuff2[1];
		buffer4 = sumbuff1[1] + sumbuff3[0];
		buffer5 = sumbuff2[0] + sumbuff3[1];

		for (; j < (size_row - 2); j += 3) {
			buffer3 += psrc[j + 0] * psrc[j + 0];
			buffer4 += psrc[j + 1] * psrc[j + 1];
			buffer5 += psrc[j + 2] * psrc[j + 2];
		}

		if (sign == 2) {
			buffer1 += psrc[j + 0] * psrc[j + 0];
			buffer2 += psrc[j + 1] * psrc[j + 1];
			buffer0 += buffer5;
			buffer1 += buffer3;
			buffer2 += buffer4;
		} else if (sign == 1) {
			buffer2 += psrc[j] * psrc[j];
			buffer0 += buffer4;
			buffer1 += buffer5;
			buffer2 += buffer3;
		} else if (sign == 0) {
			buffer0 += buffer3;
			buffer1 += buffer4;
			buffer2 += buffer5;
		}

		psrc += src_stride;
	}

	rez[0] = buffer0;
	rez[1] = buffer1;
	rez[2] = buffer2;
}

/* *********************************************************** */

void
mlib_s_ImageMoment2_S32_1(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_s32 *psrc = (mlib_s32 *)mlib_ImageGetData(img);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 2;
	mlib_s32 src_chan = mlib_ImageGetChannels(img);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img);

/* indices */
	mlib_s32 i, j, l;

	if (src_stride == size_row) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 sumbuff[2];
	mlib_s32 db_num;
	mlib_s32 *sl;

	for (i = 0; i < height; i++) {
		j = 0;

		for (j = 0; (j < size_row) &&
		    ((mlib_addr)(psrc +j) & 15); j++) {
			buffer0 += (mlib_d64)psrc[j] * psrc[j];
		}

		db_num = ((size_row - j) >> 2);
		sl = psrc + j;
		__m128i load0, load1;
		__m128d fd1, fd2, fs1, fs2, fsum1, fsum2;
		fsum1 = _mm_setzero_pd();
		fsum2 = _mm_setzero_pd();
		j += (db_num << 2);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			load0 = _mm_loadl_epi64((void *)sl);
			sl += 2;
			fd1 = _mm_cvtepi32_pd(load0);
			load1 = _mm_loadl_epi64((void *)sl);
			sl += 2;
			fd2 = _mm_cvtepi32_pd(load1);

			fs1 = _mm_mul_pd(fd1, fd1);
			fs2 = _mm_mul_pd(fd2, fd2);
			fsum1 = _mm_add_pd(fsum1, fs1);
			fsum2 = _mm_add_pd(fsum2, fs2);
		}

		fsum1 = _mm_add_pd(fsum1, fsum2);
		_mm_storeu_pd(sumbuff, fsum1);

		for (; j < size_row; j++) {
			buffer0 += (mlib_d64)psrc[j] * psrc[j];
		}

		buffer0 += sumbuff[0] + sumbuff[1];
		psrc += src_stride;
	}

	rez[0] = buffer0;
	rez[1] = 0.0;
	rez[2] = 0.0;
	rez[3] = 0.0;
}

/* *********************************************************** */

void
mlib_s_ImageMoment2_S32_2(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_s32 *psrc = (mlib_s32 *)mlib_ImageGetData(img);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 2;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) << 1;

/* indices */
	mlib_s32 i, j, l;

	if (src_stride == size_row) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_d64 sumbuff[2];
	mlib_s32 *sl;

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 alignment = 1;
		mlib_s32 db_num;
		sumbuff[0] = sumbuff[1] = 0.0;

		if ((mlib_addr)psrc & 1) {
			alignment = 0;
			buffer0 += (mlib_d64)psrc[0] * psrc[0];

			for (j = 1; (j < (size_row - 2)) &&
			    ((mlib_addr)(psrc + j) & 15); j += 2) {
				buffer1 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer0 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			}
		} else {
			for (j = 0; (j < size_row) &&
			    ((mlib_addr)(psrc + j) & 15); j += 2) {
				buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			}
		}

		db_num = ((size_row - j) >> 2);
		sl = psrc + j;
		__m128i load0, load1;
		__m128d fd1, fd2, fs1, fs2, fsum1, fsum2;
		fsum1 = _mm_setzero_pd();
		fsum2 = _mm_setzero_pd();

		j += (db_num << 2);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			load0 = _mm_loadl_epi64((void *)sl);
			sl += 2;
			fd1 = _mm_cvtepi32_pd(load0);
			load1 = _mm_loadl_epi64((void *)sl);
			sl += 2;
			fd2 = _mm_cvtepi32_pd(load1);

			fs1 = _mm_mul_pd(fd1, fd1);
			fs2 = _mm_mul_pd(fd2, fd2);
			fsum1 = _mm_add_pd(fsum1, fs1);
			fsum2 = _mm_add_pd(fsum2, fs2);
		}

		fsum1 = _mm_add_pd(fsum1, fsum2);
		_mm_storeu_pd((void *)sumbuff, fsum1);

		if (alignment) {
			buffer0 += sumbuff[0];
			buffer1 += sumbuff[1];

			for (; j < size_row; j += 2) {
				buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			}
		} else {
			buffer1 += sumbuff[0];
			buffer0 += sumbuff[1];

			for (; j < (size_row - 1); j += 2) {
				buffer1 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer0 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			}

			buffer1 += (mlib_d64)psrc[j] * psrc[j];
		}

		psrc += src_stride;
	}

	rez[0] = buffer0;
	rez[1] = buffer1;
	rez[2] = 0.0;
	rez[3] = 0.0;
}

/* *********************************************************** */

void
mlib_s_ImageMoment2_S32_4(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_s32 *psrc = (mlib_s32 *)mlib_ImageGetData(img);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 2;
	mlib_s32 src_chan = mlib_ImageGetChannels(img);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) << 2;

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
	mlib_d64 sumbuff[4];
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
			alignment -= 4;
		}

		if (j < (size_row - 3)) {
			if ((alignment == 3)) {
				buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
				buffer2 += (mlib_d64)psrc[j + 2] * psrc[j + 2];
				j += 3;
				sign = 1;
			} else if ((alignment == 2)) {
				buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
				j += 2;
				sign = 2;
			} else if ((alignment == 1)) {
				buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
				j += 1;
				sign = 3;
			}
		}

		db_num = ((size_row - j) >> 2);
		sl = (psrc + j);
		__m128i load0, load1, sdl, sdh;
		__m128d fd1, fd2, fs1, fs2, fsum1, fsum2;
		fsum1 = _mm_setzero_pd();
		fsum2 = _mm_setzero_pd();
		j += (db_num << 2);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			load0 = _mm_loadl_epi64((void *)sl);
			sl += 2;
			fd1 = _mm_cvtepi32_pd(load0);
			load1 = _mm_loadl_epi64((void *)sl);
			sl += 2;
			fd2 = _mm_cvtepi32_pd(load1);

			fs1 = _mm_mul_pd(fd1, fd1);
			fs2 = _mm_mul_pd(fd2, fd2);
			fsum1 = _mm_add_pd(fsum1, fs1);
			fsum2 = _mm_add_pd(fsum2, fs2);
		}

		_mm_storeu_pd((void *)sumbuff, fsum1);
		_mm_storeu_pd((void *)(sumbuff + 2), fsum2);

		for (; j < (size_row - 3); j += 4) {
			sumbuff[0] += (mlib_d64)psrc[j + 0] * psrc[j + 0];
			sumbuff[1] += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			sumbuff[2] += (mlib_d64)psrc[j + 2] * psrc[j + 2];
			sumbuff[3] += (mlib_d64)psrc[j + 3] * psrc[j + 3];
		}

		if (sign == 3) {
			buffer1 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
			buffer2 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			buffer3 += (mlib_d64)psrc[j + 2] * psrc[j + 2];
			j += 3;

			buffer0 += sumbuff[3];
			buffer1 += sumbuff[0];
			buffer2 += sumbuff[1];
			buffer3 += sumbuff[2];
		} else if (sign == 2) {
			buffer2 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
			buffer3 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			j += 2;

			buffer0 += sumbuff[2];
			buffer1 += sumbuff[3];
			buffer2 += sumbuff[0];
			buffer3 += sumbuff[1];
		} else if (sign == 1) {
			buffer3 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
			j++;

			buffer0 += sumbuff[1];
			buffer1 += sumbuff[2];
			buffer2 += sumbuff[3];
			buffer3 += sumbuff[0];
		} else if (sign == 0) {

			buffer0 += sumbuff[0];
			buffer1 += sumbuff[1];
			buffer2 += sumbuff[2];
			buffer3 += sumbuff[3];
		}

		psrc += src_stride;
	}

	rez[0] = buffer0;
	rez[1] = buffer1;
	rez[2] = buffer2;
	rez[3] = buffer3;
}

/* *********************************************************** */

void
mlib_s_ImageMoment2_S32_3(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_s32 *psrc = (mlib_s32 *)mlib_ImageGetData(img);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 2;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) * 3;

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
	mlib_d64 buffer4 = 0.0;
	mlib_d64 buffer5 = 0.0;

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
		}

		if ((alignment - j) == 2) {
			buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
			buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			j += 2;
			sign = 1;
		} else if ((alignment - j) == 1) {
			buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
			j += 1;
			sign = 2;
		}

		db_num = ((size_row - j) / 12);
		sl = psrc + j;
		__m128i load0, load1, load2, load3, load4, load5;
		__m128d fd0, fd1, fd2, fd3, fd4, fd5;
		__m128d fs0, fs1, fs2, fs3, fs4, fs5;
		__m128d fsum0, fsum1, fsum2, fsum3, fsum4, fsum5;
		mlib_d64 sumbuff0[2], sumbuff1[2], sumbuff2[2];
		mlib_d64 sumbuff3[2], sumbuff4[2], sumbuff5[2];
		fsum0 = _mm_setzero_pd();
		fsum1 = _mm_setzero_pd();
		fsum2 = _mm_setzero_pd();
		fsum3 = _mm_setzero_pd();
		fsum4 = _mm_setzero_pd();
		fsum5 = _mm_setzero_pd();

		j += (db_num * 12);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			load0 = _mm_loadl_epi64((void *)sl);
			sl += 2;
			load1 = _mm_loadl_epi64((void *)sl);
			sl += 2;
			load2 = _mm_loadl_epi64((void *)sl);
			sl += 2;
			load3 = _mm_loadl_epi64((void *)sl);
			sl += 2;
			load4 = _mm_loadl_epi64((void *)sl);
			sl += 2;
			load5 = _mm_loadl_epi64((void *)sl);
			sl += 2;

			fd0 = _mm_cvtepi32_pd(load0);
			fd1 = _mm_cvtepi32_pd(load1);
			fd2 = _mm_cvtepi32_pd(load2);
			fd3 = _mm_cvtepi32_pd(load3);
			fd4 = _mm_cvtepi32_pd(load4);
			fd5 = _mm_cvtepi32_pd(load5);

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
		}

		_mm_storeu_pd(sumbuff0, fsum0);
		_mm_storeu_pd(sumbuff1, fsum1);
		_mm_storeu_pd(sumbuff2, fsum2);
		_mm_storeu_pd(sumbuff3, fsum3);
		_mm_storeu_pd(sumbuff4, fsum4);
		_mm_storeu_pd(sumbuff5, fsum5);

		buffer3 = sumbuff0[0] + sumbuff1[1] + sumbuff3[0] + sumbuff4[1];
		buffer4 = sumbuff0[1] + sumbuff2[0] + sumbuff3[1] + sumbuff5[0];
		buffer5 = sumbuff1[0] + sumbuff2[1] + sumbuff4[0] + sumbuff5[1];

		for (; j < (size_row - 2); j += 3) {
			buffer3 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
			buffer4 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			buffer5 += (mlib_d64)psrc[j + 2] * psrc[j + 2];
		}

		if (sign == 2) {
			buffer1 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
			buffer2 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			buffer0 += buffer5;
			buffer1 += buffer3;
			buffer2 += buffer4;
		} else if (sign == 1) {
			buffer2 += (mlib_d64)psrc[j] * psrc[j];
			buffer0 += buffer4;
			buffer1 += buffer5;
			buffer2 += buffer3;
		} else if (sign == 0) {
			buffer0 += buffer3;
			buffer1 += buffer4;
			buffer2 += buffer5;
		}

		psrc += src_stride;
	}

	rez[0] = buffer0;
	rez[1] = buffer1;
	rez[2] = buffer2;
}

/* *********************************************************** */
