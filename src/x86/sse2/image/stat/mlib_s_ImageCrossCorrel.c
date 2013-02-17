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

#pragma ident	"@(#)mlib_s_ImageCrossCorrel.c	9.5	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageCrossCorrel - calculates image cross correlation
 *                              for the input images
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageCrossCorrel(mlib_d64         *correl,
 *                                        const mlib_image *img1,
 *                                        const mlib_image *img2);
 *
 * ARGUMENTS
 *      correl   pointer to cross correlation array
 *      img1     pointer to the first input image
 *      img2     pointer to the second input image
 *
 * DESCRIPTION
 *
 *           1    w-1 h-1
 *      c = --- * SUM SUM(x[i][j] * y[i][j])
 *          w*h   i = 0 j = 0
 *
 * RESTRICTION
 *      The images must have the same type, the same size, and the same number
 *      of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be of MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_s_ImageCorrel.h>
#include <mlib_ImageDivTables.h>
#include <stdio.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif
#include <mlib_sse_utils.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageCrossCorrel = __mlib_ImageCrossCorrel

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageCrossCorrel) mlib_ImageCrossCorrel
    __attribute__((weak, alias("__mlib_ImageCrossCorrel")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageCrossCorrel(
    mlib_d64 *correl,
    const mlib_image *img1,
    const mlib_image *img2)
{
	mlib_s32 num_pixel;
	mlib_d64 res[4];
	mlib_d64 divider;

/* check for obvious errors */
	MLIB_IMAGE_CHECK(img1);
	MLIB_IMAGE_CHECK(img2);
	MLIB_IMAGE_SIZE_EQUAL(img1, img2);
	MLIB_IMAGE_TYPE_EQUAL(img1, img2);
	MLIB_IMAGE_CHAN_EQUAL(img1, img2);

	if (correl == NULL)
		return (MLIB_NULLPOINTER);

/* total number of pixels */
	num_pixel = mlib_ImageGetWidth(img1) * mlib_ImageGetHeight(img1);
	divider = 1.0 / (mlib_d64)num_pixel;

	switch (mlib_ImageGetType(img1)) {
/* handle MLIB_BYTE data type of image */
	case MLIB_BYTE:

		switch (mlib_ImageGetChannels(img1)) {
		case 1:
			mlib_s_ImageCrossCorrel_U8_1(img1, img2, res);
			break;
		case 2:
			mlib_s_ImageCrossCorrel_U8_2(img1, img2, res);
			break;
		case 3:
			mlib_s_ImageCrossCorrel_U8_3(img1, img2, res);
			break;
		case 4:
			mlib_s_ImageCrossCorrel_U8_4(img1, img2, res);
			break;
		default:
			return (MLIB_FAILURE);
		}

		break;

/* handle MLIB_USHORT data type of image */
	case MLIB_USHORT:

		switch (mlib_ImageGetChannels(img1)) {
		case 1:
			mlib_s_ImageCrossCorrel_U16_1(img1, img2, res);
			break;
		case 2:
			mlib_s_ImageCrossCorrel_U16_2(img1, img2, res);
			break;
		case 3:
			mlib_s_ImageCrossCorrel_U16_3(img1, img2, res);
			break;
		case 4:
			mlib_s_ImageCrossCorrel_U16_4(img1, img2, res);
			break;
		default:
			return (MLIB_FAILURE);
		}
		break;

/* handle MLIB_SHORT data type of image */
	case MLIB_SHORT:
		switch (mlib_ImageGetChannels(img1)) {
		case 1:
			mlib_s_ImageCrossCorrel_S16_1(img1, img2, res);
			break;
		case 2:
			mlib_s_ImageCrossCorrel_S16_2(img1, img2, res);
			break;
		case 3:
			mlib_s_ImageCrossCorrel_S16_3(img1, img2, res);
			break;
		case 4:
			mlib_s_ImageCrossCorrel_S16_4(img1, img2, res);
			break;
		default:
			return (MLIB_FAILURE);
		}
		break;

/* handle MLIB_INT data type of image */
	case MLIB_INT:
		switch (mlib_ImageGetChannels(img1)) {
		case 1:
			mlib_s_ImageCrossCorrel_S32_1(img1, img2, res);
			break;
		case 2:
			mlib_s_ImageCrossCorrel_S32_2(img1, img2, res);
			break;
		case 3:
			mlib_s_ImageCrossCorrel_S32_3(img1, img2, res);
			break;
		case 4:
			mlib_s_ImageCrossCorrel_S32_4(img1, img2, res);
			break;
		default:
			return (MLIB_FAILURE);
		}
		break;

/* discard any other data types */
	default:
		return (MLIB_FAILURE);
	}

	switch (mlib_ImageGetChannels(img1)) {
	case 1:
		correl[0] = (res[0] + res[1] + res[2] + res[3]) * divider;
		break;

	case 2:
		correl[0] = (res[0] + res[2]) * divider;
		correl[1] = (res[1] + res[3]) * divider;
		break;

	case 4:
		correl[3] = res[3] * divider;
	case 3:
		correl[0] = res[0] * divider;
		correl[1] = res[1] * divider;
		correl[2] = res[2] * divider;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_U8(x, y)	(mlib_U82D64[(x)] * mlib_U82D64[(y)])

/* *********************************************************** */

void
mlib_s_ImageCrossCorrel_U8_3(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_u8 *psrc1 = (mlib_u8 *)mlib_ImageGetData(img1);
	mlib_u8 *psrc2 = (mlib_u8 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src_stride1 = mlib_ImageGetStride(img1);
	mlib_s32 src_stride2 = mlib_ImageGetStride(img2);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) * 3;

/* indices */
	mlib_s32 i, j, l;

	if (src_stride1 == size_row && src_stride2 == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
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
	__m128i *sl1, *sl2;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 sign = 0;
		buffer3 = buffer4 = buffer5 = 0.0;

		mlib_s32 alignment =
			((16 - ((mlib_addr)psrc1 & 15)) & 15);
		mlib_s32 aligntwo = alignment -
		    ((16 - ((mlib_addr)psrc2 & 15)) & 15);

		for (j = 0;
		    (j < (size_row - 3)) && (j <= (alignment - 3));
		    j += 3) {
			buffer0 += psrc1[j + 0] * psrc2[j + 0];
			buffer1 += psrc1[j + 1] * psrc2[j + 1];
			buffer2 += psrc1[j + 2] * psrc2[j + 2];
		}

		if ((alignment - j) == 2) {
			buffer0 += psrc1[j + 0] * psrc2[j + 0];
			buffer1 += psrc1[j + 1] * psrc2[j + 1];
			j += 2;
			sign = 1;
		} else if ((alignment - j) == 1) {
			buffer0 += psrc1[j + 0] * psrc2[j + 0];
			j += 1;
			sign = 2;
		}

		db_num = ((size_row - j) / 48);
		sl1 = (__m128i *)(psrc1 + j);
		sl2 = (__m128i *)(psrc2 + j);
		__m128i load10, load11, load12;
		__m128i load10_l, load11_l, load12_l;
		__m128i load10_h, load11_h, load12_h;
		__m128i load20, load21, load22;
		__m128i load20_l, load21_l, load22_l;
		__m128i load20_h, load21_h, load22_h;
		__m128i sdl0_l, sdh0_l;
		__m128i sdl0_h, sdh0_h;
		__m128i sd0, sd1, sd2, sd3, sd4, sd5, sd6;
		__m128i sd7, sd8, sd9, sd10, sd11;
		__m128d st[24], sum[24];
		mlib_d64 sf[24][2];

		for (int k = 0; k < 24; k++) {
			sum[k] = _mm_setzero_pd();
		}

		__m128d fsd10, fsd20, fsd30, fsd40, fsd50, fsd60;
		__m128d fsd11, fsd21, fsd31, fsd41, fsd51, fsd61;
		__m128d fsum1, fsum2, fsum3;
		mlib_d64 sumbuff1[2], sumbuff2[2], sumbuff3[2];
		fsum1 = _mm_setzero_pd();
		fsum2 = _mm_setzero_pd();
		fsum3 = _mm_setzero_pd();

		j += (db_num * 48);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			load10 = _mm_load_si128(sl1);
			load11 = _mm_load_si128(sl1 + 1);
			load12 = _mm_load_si128(sl1 + 2);
			if (0 == aligntwo) {
				load20 = _mm_load_si128(sl2);
				load21 = _mm_load_si128(sl2 + 1);
				load22 = _mm_load_si128(sl2 + 2);
			} else {
				load20 = _mm_loadu_si128(sl2);
				load21 = _mm_loadu_si128(sl2 + 1);
				load22 = _mm_loadu_si128(sl2 + 2);
			}
			load10_l = _mm_unpacklo_epi8(load10, x_zero);
			load10_h = _mm_unpackhi_epi8(load10, x_zero);
			load11_l = _mm_unpacklo_epi8(load11, x_zero);
			load11_h = _mm_unpackhi_epi8(load11, x_zero);
			load12_l = _mm_unpacklo_epi8(load12, x_zero);
			load12_h = _mm_unpackhi_epi8(load12, x_zero);

			load20_l = _mm_unpacklo_epi8(load20, x_zero);
			load20_h = _mm_unpackhi_epi8(load20, x_zero);
			load21_l = _mm_unpacklo_epi8(load21, x_zero);
			load21_h = _mm_unpackhi_epi8(load21, x_zero);
			load22_l = _mm_unpacklo_epi8(load22, x_zero);
			load22_h = _mm_unpackhi_epi8(load22, x_zero);

			sdl0_l = _mm_mullo_epi16(load10_l, load20_l);
			sdl0_h = _mm_mulhi_epi16(load10_l, load20_l);
			sd0 = _mm_unpacklo_epi16(sdl0_l, sdl0_h);
			sd1 = _mm_unpackhi_epi16(sdl0_l, sdl0_h);

			sdh0_l = _mm_mullo_epi16(load10_h, load20_h);
			sdh0_h = _mm_mulhi_epi16(load10_h, load20_h);
			sd2 = _mm_unpacklo_epi16(sdh0_l, sdh0_h);
			sd3 = _mm_unpackhi_epi16(sdh0_l, sdh0_h);

			sdl0_l = _mm_mullo_epi16(load11_l, load21_l);
			sdl0_h = _mm_mulhi_epi16(load11_l, load21_l);
			sd4 = _mm_unpacklo_epi16(sdl0_l, sdl0_h);
			sd5 = _mm_unpackhi_epi16(sdl0_l, sdl0_h);

			sdh0_l = _mm_mullo_epi16(load11_h, load21_h);
			sdh0_h = _mm_mulhi_epi16(load11_h, load21_h);
			sd6 = _mm_unpacklo_epi16(sdh0_l, sdh0_h);
			sd7 = _mm_unpackhi_epi16(sdh0_l, sdh0_h);

			sdl0_l = _mm_mullo_epi16(load12_l, load22_l);
			sdl0_h = _mm_mulhi_epi16(load12_l, load22_l);
			sd8 = _mm_unpacklo_epi16(sdl0_l, sdl0_h);
			sd9 = _mm_unpackhi_epi16(sdl0_l, sdl0_h);

			sdh0_l = _mm_mullo_epi16(load12_h, load22_h);
			sdh0_h = _mm_mulhi_epi16(load12_h, load22_h);
			sd10 = _mm_unpacklo_epi16(sdh0_l, sdh0_h);
			sd11 = _mm_unpackhi_epi16(sdh0_l, sdh0_h);

			st[0] = _mm_cvtepi32_pd(sd0);
			st[1] = _mm_cvtepi32_pd(_mm_srli_si128(sd0, 8));
			st[2] = _mm_cvtepi32_pd(sd1);
			st[3] = _mm_cvtepi32_pd(_mm_srli_si128(sd1, 8));
			st[4] = _mm_cvtepi32_pd(sd2);
			st[5] = _mm_cvtepi32_pd(_mm_srli_si128(sd2, 8));
			st[6] = _mm_cvtepi32_pd(sd3);
			st[7] = _mm_cvtepi32_pd(_mm_srli_si128(sd3, 8));
			st[8] = _mm_cvtepi32_pd(sd4);
			st[9] = _mm_cvtepi32_pd(_mm_srli_si128(sd4, 8));
			st[10] = _mm_cvtepi32_pd(sd5);
			st[11] = _mm_cvtepi32_pd(_mm_srli_si128(sd5, 8));
			st[12] = _mm_cvtepi32_pd(sd6);
			st[13] = _mm_cvtepi32_pd(_mm_srli_si128(sd6, 8));
			st[14] = _mm_cvtepi32_pd(sd7);
			st[15] = _mm_cvtepi32_pd(_mm_srli_si128(sd7, 8));
			st[16] = _mm_cvtepi32_pd(sd8);
			st[17] = _mm_cvtepi32_pd(_mm_srli_si128(sd8, 8));
			st[18] = _mm_cvtepi32_pd(sd9);
			st[19] = _mm_cvtepi32_pd(_mm_srli_si128(sd9, 8));
			st[20] = _mm_cvtepi32_pd(sd10);
			st[21] = _mm_cvtepi32_pd(_mm_srli_si128(sd10, 8));
			st[22] = _mm_cvtepi32_pd(sd11);
			st[23] = _mm_cvtepi32_pd(_mm_srli_si128(sd11, 8));

			sum[0] = _mm_add_pd(sum[0], st[0]);
			sum[1] = _mm_add_pd(sum[1], st[1]);
			sum[2] = _mm_add_pd(sum[2], st[2]);
			sum[3] = _mm_add_pd(sum[3], st[3]);
			sum[4] = _mm_add_pd(sum[4], st[4]);
			sum[5] = _mm_add_pd(sum[5], st[5]);
			sum[6] = _mm_add_pd(sum[6], st[6]);
			sum[7] = _mm_add_pd(sum[7], st[7]);
			sum[8] = _mm_add_pd(sum[8], st[8]);
			sum[9] = _mm_add_pd(sum[9], st[9]);
			sum[10] = _mm_add_pd(sum[10], st[10]);
			sum[11] = _mm_add_pd(sum[11], st[11]);
			sum[12] = _mm_add_pd(sum[12], st[12]);
			sum[13] = _mm_add_pd(sum[13], st[13]);
			sum[14] = _mm_add_pd(sum[14], st[14]);
			sum[15] = _mm_add_pd(sum[15], st[15]);
			sum[16] = _mm_add_pd(sum[16], st[16]);
			sum[17] = _mm_add_pd(sum[17], st[17]);
			sum[18] = _mm_add_pd(sum[18], st[18]);
			sum[19] = _mm_add_pd(sum[19], st[19]);
			sum[20] = _mm_add_pd(sum[20], st[20]);
			sum[21] = _mm_add_pd(sum[21], st[21]);
			sum[22] = _mm_add_pd(sum[22], st[22]);
			sum[23] = _mm_add_pd(sum[23], st[23]);

			sl1 += 3;
			sl2 += 3;
		}

		_mm_storeu_pd((void *)sf[0], sum[0]);
		_mm_storeu_pd((void *)sf[1], sum[1]);
		_mm_storeu_pd((void *)sf[2], sum[2]);
		_mm_storeu_pd((void *)sf[3], sum[3]);
		_mm_storeu_pd((void *)sf[4], sum[4]);
		_mm_storeu_pd((void *)sf[5], sum[5]);
		_mm_storeu_pd((void *)sf[6], sum[6]);
		_mm_storeu_pd((void *)sf[7], sum[7]);
		_mm_storeu_pd((void *)sf[8], sum[8]);
		_mm_storeu_pd((void *)sf[9], sum[9]);
		_mm_storeu_pd((void *)sf[10], sum[10]);
		_mm_storeu_pd((void *)sf[11], sum[11]);
		_mm_storeu_pd((void *)sf[12], sum[12]);
		_mm_storeu_pd((void *)sf[13], sum[13]);
		_mm_storeu_pd((void *)sf[14], sum[14]);
		_mm_storeu_pd((void *)sf[15], sum[15]);
		_mm_storeu_pd((void *)sf[16], sum[16]);
		_mm_storeu_pd((void *)sf[17], sum[17]);
		_mm_storeu_pd((void *)sf[18], sum[18]);
		_mm_storeu_pd((void *)sf[19], sum[19]);
		_mm_storeu_pd((void *)sf[20], sum[20]);
		_mm_storeu_pd((void *)sf[21], sum[21]);
		_mm_storeu_pd((void *)sf[22], sum[22]);
		_mm_storeu_pd((void *)sf[23], sum[23]);

		buffer3 = sf[0][0] + sf[1][1] + sf[3][0] + sf[4][1] +
		    sf[6][0] + sf[7][1] + sf[9][0] + sf[10][1] +
		    sf[12][0] + sf[13][1] + sf[15][0] + sf[16][1] +
		    sf[18][0] + sf[19][1] + sf[21][0] + sf[22][1];
		buffer4 = sf[0][1] + sf[2][0] + sf[3][1] + sf[5][0] +
		    sf[6][1] + sf[8][0] + sf[9][1] + sf[11][0] +
		    sf[12][1] + sf[14][0] + sf[15][1] + sf[17][0] +
		    sf[18][1] + sf[20][0] + sf[21][1] + sf[23][0];
		buffer5 = sf[1][0] + sf[2][1] + sf[4][0] + sf[5][1] +
		    sf[7][0] + sf[8][1] + sf[10][0] + sf[11][1] +
		    sf[13][0] + sf[14][1] + sf[16][0] + sf[17][1] +
		    sf[19][0] + sf[20][1] + sf[22][0] + sf[23][1];

		for (; j < (size_row - 2); j += 3) {
			buffer3 += psrc1[j + 0] * psrc2[j + 0];
			buffer4 += psrc1[j + 1] * psrc2[j + 1];
			buffer5 += psrc1[j + 2] * psrc2[j + 2];
		}

		if (sign == 2) {
			buffer1 += psrc1[j + 0] * psrc2[j + 0];
			buffer2 += psrc1[j + 1] * psrc2[j + 1];
			buffer0 += buffer5;
			buffer1 += buffer3;
			buffer2 += buffer4;
		} else if (sign == 1) {
			buffer2 += psrc1[j] * psrc2[j];
			buffer0 += buffer4;
			buffer1 += buffer5;
			buffer2 += buffer3;
		} else if (sign == 0) {
			buffer0 += buffer3;
			buffer1 += buffer4;
			buffer2 += buffer5;
		}

		psrc1 += src_stride1;
		psrc2 += src_stride2;
	}

	res[0] = buffer0;
	res[1] = buffer1;
	res[2] = buffer2;
}

/* *********************************************************** */

void
mlib_s_ImageCrossCorrel_U8_3back(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of first image */
	mlib_u8 *point_src1 = (mlib_u8 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_u8 *point_src2 = (mlib_u8 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 c1, c2, c3;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src_stride1 = mlib_ImageGetStride(img1);

/* elements to next row */
	mlib_s32 src_stride2 = mlib_ImageGetStride(img2);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) * 3;

/* indices */
	mlib_s32 i, j;
	mlib_u8 *psrc1, *psrc2;

	if (src_stride1 == size_row && src_stride2 == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	c1 = c2 = c3 = 0;
	for (i = 0; i < height; i++) {
		psrc1 = point_src1;
		psrc2 = point_src2;
		j = 0;

		if ((mlib_addr)psrc1 & 1) {
			c1 += MUL_U8(psrc1[j], psrc2[j]);
			c2 += MUL_U8(psrc1[j + 1], psrc2[j + 1]);
			c3 += MUL_U8(psrc1[j + 2], psrc2[j + 2]);
			j += 3;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (size_row - 6); j += 6) {
			mlib_u32 u1, u2, u3;

			u1 = ((mlib_u16 *)(psrc1 + j))[0];
			u2 = ((mlib_u16 *)(psrc1 + j))[1];
			u3 = ((mlib_u16 *)(psrc1 + j))[2];
#ifdef _LITTLE_ENDIAN
			c1 += MUL_U8((u1 & 0xff), psrc2[j]);
			c2 += MUL_U8((u1 >> 8), psrc2[j + 1]);
			c3 += MUL_U8((u2 & 0xff), psrc2[j + 2]);
			c1 += MUL_U8((u2 >> 8), psrc2[j + 3]);
			c2 += MUL_U8((u3 & 0xff), psrc2[j + 4]);
			c3 += MUL_U8((u3 >> 8), psrc2[j + 5]);
#else /* _LITTLE_ENDIAN */
			c1 += MUL_U8((u1 >> 8), psrc2[j]);
			c2 += MUL_U8((u1 & 0xff), psrc2[j + 1]);
			c3 += MUL_U8((u2 >> 8), psrc2[j + 2]);
			c1 += MUL_U8((u2 & 0xff), psrc2[j + 3]);
			c2 += MUL_U8((u3 >> 8), psrc2[j + 4]);
			c3 += MUL_U8((u3 & 0xff), psrc2[j + 5]);
#endif /* _LITTLE_ENDIAN */
		}

		if (j < size_row) {
			c1 += MUL_U8(psrc1[j], psrc2[j]);
			c2 += MUL_U8(psrc1[j + 1], psrc2[j + 1]);
			c3 += MUL_U8(psrc1[j + 2], psrc2[j + 2]);
		}

		point_src1 += src_stride1;
		point_src2 += src_stride2;
	}

	res[0] = c1;
	res[1] = c2;
	res[2] = c3;
}

/* *********************************************************** */

void
mlib_s_ImageCrossCorrel_U8_1(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_u8 *psrc1 = (mlib_u8 *)mlib_ImageGetData(img1);
	mlib_u8 *psrc2 = (mlib_u8 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src_stride1 = mlib_ImageGetStride(img1);
	mlib_s32 src_stride2 = mlib_ImageGetStride(img2);
	mlib_s32 src_chan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1);

/* indices */
	mlib_s32 i, j, l;

	if (src_stride1 == size_row && src_stride2 == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_s32 db_num;
	__m128i *sl1, *sl2;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {
		j = 0;

		mlib_s32 aligntwo =
		    ((16 - ((mlib_addr)psrc1 & 15)) & 15) -
		    ((16 - ((mlib_addr)psrc2 & 15)) & 15);

		for (j = 0; (j < size_row) &&
		    ((mlib_addr)(psrc1 +j) & 15); j++) {
			buffer0 += psrc1[j] * psrc2[j];
		}

		db_num = ((size_row - j) >> 4);
		sl1 = (__m128i *)(psrc1 + j);
		sl2 = (__m128i *)(psrc2 + j);
		__m128i load1, load2, sdl1, sdh1;
		__m128i sdl2, sdh2, sd1, sd2, sd3, sd4;
		__m128i sdm1, sdm2;
		__m128d sum1, sum2;
		sum1 = _mm_setzero_pd();
		sum2 = _mm_setzero_pd();
		j += (db_num << 4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			load1 = _mm_load_si128(sl1);
			if (0 == aligntwo) {
				load2 = _mm_load_si128(sl2);
			} else {
				load2 = _mm_loadu_si128(sl2);
			}
			sdl1 = _mm_unpacklo_epi8(load1, x_zero);
			sdh1 = _mm_unpackhi_epi8(load1, x_zero);
			sdl2 = _mm_unpacklo_epi8(load2, x_zero);
			sdh2 = _mm_unpackhi_epi8(load2, x_zero);
			sdm1 = _mm_madd_epi16(sdl1, sdl2);
			sdm2 = _mm_madd_epi16(sdh1, sdh2);

			sd1 = sdm1;
			sd2 = _mm_unpackhi_epi64(sdm1, x_zero);
			sd3 = sdm2;
			sd4 = _mm_unpackhi_epi64(sdm2, x_zero);

			sum1 = _mm_add_pd(sum1, _mm_cvtepi32_pd(sd1));
			sum2 = _mm_add_pd(sum2, _mm_cvtepi32_pd(sd2));
			sum1 = _mm_add_pd(sum1, _mm_cvtepi32_pd(sd3));
			sum2 = _mm_add_pd(sum2, _mm_cvtepi32_pd(sd4));
			sl1++;
			sl2++;
		}

		sum1 = _mm_add_pd(sum1, sum2);

		for (; j < size_row; j++) {
			buffer0 += psrc1[j] * psrc2[j];
		}

		buffer0 += ((mlib_d64 *)&sum1)[0];
		buffer1 += ((mlib_d64 *)&sum1)[1];
		psrc1 += src_stride1;
		psrc2 += src_stride2;
	}

	res[0] = buffer0;
	res[1] = buffer1;
	res[2] = 0.0;
	res[3] = 0.0;
}

/* *********************************************************** */

void
mlib_s_ImageCrossCorrel_U8_2(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_u8 *psrc1 = (mlib_u8 *)mlib_ImageGetData(img1);
	mlib_u8 *psrc2 = (mlib_u8 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src_stride1 = mlib_ImageGetStride(img1);
	mlib_s32 src_stride2 = mlib_ImageGetStride(img2);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << 1;

/* indices */
	mlib_s32 i, j, l;

	if (src_stride1 == size_row && src_stride2 == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	__m128i *sl1, *sl2;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 alignment = 1;
		mlib_s32 db_num;
		mlib_s32 aligntwo =
		    ((16 - ((mlib_addr)psrc1 & 15)) & 15) -
		    ((16 - ((mlib_addr)psrc2 & 15)) & 15);

		if ((mlib_addr)psrc1 & 1) {
			alignment = 0;
			buffer0 += psrc1[0] * psrc2[0];

			for (j = 1; (j < (size_row - 2)) &&
			    ((mlib_addr)(psrc1 +j) & 15); j += 2) {
				buffer1 += psrc1[j + 0] * psrc2[j + 0];
				buffer0 += psrc1[j + 1] * psrc2[j + 1];
			}
		} else {
			for (j = 0; (j < size_row) &&
			    ((mlib_addr)(psrc1 +j) & 15); j += 2) {
				buffer0 += psrc1[j + 0] * psrc2[j + 0];
				buffer1 += psrc1[j + 1] * psrc2[j + 1];
			}
		}

		db_num = ((size_row - j) >> 4);
		sl1 = (__m128i *)(psrc1 + j);
		sl2 = (__m128i *)(psrc2 + j);
		__m128i load1, load2, load10, load11, load20, load21;
		__m128i sdl0, sdh0, sdl1, sdh1, sdm0, sdm1, sdm2, sdm3;
		__m128i sd0, sd1, sd2, sd3, sd4, sd5, sd6, sd7;
		__m128d sum1, sum2;
		sum1 = _mm_setzero_pd();
		sum2 = _mm_setzero_pd();

		j += (db_num << 4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			load1 = _mm_load_si128(sl1);
			if (0 == aligntwo) {
				load2 = _mm_load_si128(sl2);
			} else {
				load2 = _mm_loadu_si128(sl2);
			}
			load10 = _mm_unpacklo_epi8(load1, x_zero);
			load11 = _mm_unpackhi_epi8(load1, x_zero);
			load20 = _mm_unpacklo_epi8(load2, x_zero);
			load21 = _mm_unpackhi_epi8(load2, x_zero);

			sdl0 = _mm_mullo_epi16(load10, load20);
			sdh0 = _mm_mulhi_epi16(load10, load20);
			sdl1 = _mm_mullo_epi16(load11, load21);
			sdh1 = _mm_mulhi_epi16(load11, load21);

			sdm0 = _mm_unpacklo_epi16(sdl0, sdh0);
			sdm1 = _mm_unpackhi_epi16(sdl0, sdh0);
			sdm2 = _mm_unpacklo_epi16(sdl1, sdh1);
			sdm3 = _mm_unpackhi_epi16(sdl1, sdh1);

			sd0 = sdm0;
			sd1 = _mm_unpackhi_epi64(sdm0, x_zero);
			sd2 = sdm1;
			sd3 = _mm_unpackhi_epi64(sdm1, x_zero);
			sd4 = sdm2;
			sd5 = _mm_unpackhi_epi64(sdm2, x_zero);
			sd6 = sdm3;
			sd7 = _mm_unpackhi_epi64(sdm3, x_zero);

			sum1 = _mm_add_pd(sum1, _mm_cvtepi32_pd(sd0));
			sum2 = _mm_add_pd(sum2, _mm_cvtepi32_pd(sd1));
			sum1 = _mm_add_pd(sum1, _mm_cvtepi32_pd(sd2));
			sum2 = _mm_add_pd(sum2, _mm_cvtepi32_pd(sd3));
			sum1 = _mm_add_pd(sum1, _mm_cvtepi32_pd(sd4));
			sum2 = _mm_add_pd(sum2, _mm_cvtepi32_pd(sd5));
			sum1 = _mm_add_pd(sum1, _mm_cvtepi32_pd(sd6));
			sum2 = _mm_add_pd(sum2, _mm_cvtepi32_pd(sd7));

			sl1++;
			sl2++;
		}

		sum1 = _mm_add_pd(sum1, sum2);

		if (alignment) {
			buffer0 += ((mlib_d64 *)&sum1)[0];
			buffer1 += ((mlib_d64 *)&sum1)[1];

			for (; j < size_row; j += 2) {
				buffer0 += psrc1[j + 0] * psrc2[j + 0];
				buffer1 += psrc1[j + 1] * psrc2[j + 1];
			}
		} else {
			buffer1 += ((mlib_d64 *)&sum1)[0];
			buffer0 += ((mlib_d64 *)&sum1)[1];

			for (; j < (size_row - 1); j += 2) {
				buffer1 += psrc1[j + 0] * psrc2[j + 0];
				buffer0 += psrc1[j + 1] * psrc2[j + 1];
			}

			buffer1 += psrc1[j] * psrc2[j];
		}

		psrc1 += src_stride1;
		psrc2 += src_stride2;
	}

	res[0] = buffer0;
	res[1] = buffer1;
	res[2] = 0.0;
	res[3] = 0.0;
}

/* *********************************************************** */

void
mlib_s_ImageCrossCorrel_U8_4(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_u8 *psrc1 = (mlib_u8 *)mlib_ImageGetData(img1);
	mlib_u8 *psrc2 = (mlib_u8 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src_stride1 = mlib_ImageGetStride(img1);
	mlib_s32 src_stride2 = mlib_ImageGetStride(img2);

	mlib_s32 src_chan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << 2;

/* indices */
	mlib_s32 i, j, l;

	if (src_stride1 == size_row && src_stride2 == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_d64 buffer2 = 0.0;
	mlib_d64 buffer3 = 0.0;
	mlib_s32 db_num;
	__m128i *sl1, *sl2;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 sign = 0;
		mlib_s32 alignment =
		    ((16 - ((mlib_addr)psrc1 & 15)) & 15);
		mlib_s32 aligntwo = alignment -
		    ((16 - ((mlib_addr)psrc2 & 15)) & 15);

		for (; (j < size_row - 3) && (alignment >= 4); j += 4) {
			buffer0 += psrc1[j + 0] * psrc2[j + 0];
			buffer1 += psrc1[j + 1] * psrc2[j + 1];
			buffer2 += psrc1[j + 2] * psrc2[j + 2];
			buffer3 += psrc1[j + 3] * psrc2[j + 3];
			alignment -= 4;
		}

		if (j < (size_row - 3)) {
			if ((alignment == 3)) {
				buffer0 += psrc1[j + 0] * psrc2[j + 0];
				buffer1 += psrc1[j + 1] * psrc2[j + 1];
				buffer2 += psrc1[j + 2] * psrc2[j + 2];
				j += 3;
				sign = 1;
			} else if ((alignment == 2)) {
				buffer0 += psrc1[j + 0] * psrc2[j + 0];
				buffer1 += psrc1[j + 1] * psrc2[j + 1];
				j += 2;
				sign = 2;
			} else if ((alignment == 1)) {
				buffer0 += psrc1[j + 0] * psrc2[j + 0];
				j += 1;
				sign = 3;
			}
		}

		db_num = ((size_row - j) >> 4);
		sl1 = (__m128i *)(psrc1 + j);
		sl2 = (__m128i *)(psrc2 + j);
		__m128i load1, load2, load10, load11, load20, load21;
		__m128i sdl0, sdh0, sdl1, sdh1, sdm0, sdm1, sdm2, sdm3;
		__m128i sd0, sd1, sd2, sd3, sd4, sd5, sd6, sd7;
		__m128d sum1, sum2;
		sum1 = _mm_setzero_pd();
		sum2 = _mm_setzero_pd();
		j += (db_num << 4);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			load1 = _mm_load_si128(sl1);
			if (0 == aligntwo) {
				load2 = _mm_load_si128(sl2);
			} else {
				load2 = _mm_loadu_si128(sl2);
			}
			load10 = _mm_unpacklo_epi8(load1, x_zero);
			load11 = _mm_unpackhi_epi8(load1, x_zero);
			load20 = _mm_unpacklo_epi8(load2, x_zero);
			load21 = _mm_unpackhi_epi8(load2, x_zero);

			sdl0 = _mm_mullo_epi16(load10, load20);
			sdh0 = _mm_mulhi_epi16(load10, load20);
			sdl1 = _mm_mullo_epi16(load11, load21);
			sdh1 = _mm_mulhi_epi16(load11, load21);

			sdm0 = _mm_unpacklo_epi16(sdl0, sdh0);
			sdm1 = _mm_unpackhi_epi16(sdl0, sdh0);
			sdm2 = _mm_unpacklo_epi16(sdl1, sdh1);
			sdm3 = _mm_unpackhi_epi16(sdl1, sdh1);

			sd0 = sdm0;
			sd1 = _mm_unpackhi_epi64(sdm0, x_zero);
			sd2 = sdm1;
			sd3 = _mm_unpackhi_epi64(sdm1, x_zero);
			sd4 = sdm2;
			sd5 = _mm_unpackhi_epi64(sdm2, x_zero);
			sd6 = sdm3;
			sd7 = _mm_unpackhi_epi64(sdm3, x_zero);

			sum1 = _mm_add_pd(sum1, _mm_cvtepi32_pd(sd0));
			sum2 = _mm_add_pd(sum2, _mm_cvtepi32_pd(sd1));
			sum1 = _mm_add_pd(sum1, _mm_cvtepi32_pd(sd2));
			sum2 = _mm_add_pd(sum2, _mm_cvtepi32_pd(sd3));
			sum1 = _mm_add_pd(sum1, _mm_cvtepi32_pd(sd4));
			sum2 = _mm_add_pd(sum2, _mm_cvtepi32_pd(sd5));
			sum1 = _mm_add_pd(sum1, _mm_cvtepi32_pd(sd6));
			sum2 = _mm_add_pd(sum2, _mm_cvtepi32_pd(sd7));


			sl1++;
			sl2++;
		}


		for (; j < (size_row - 3); j += 4) {
			((mlib_d64 *)&sum1)[0] += psrc1[j + 0] * psrc2[j + 0];
			((mlib_d64 *)&sum1)[1] += psrc1[j + 1] * psrc2[j + 1];
			((mlib_d64 *)&sum2)[0] += psrc1[j + 2] * psrc2[j + 2];
			((mlib_d64 *)&sum2)[1] += psrc1[j + 3] * psrc2[j + 3];
		}

		if (sign == 3) {
			buffer1 += psrc1[j + 0] * psrc2[j + 0];
			buffer2 += psrc1[j + 1] * psrc2[j + 1];
			buffer3 += psrc1[j + 2] * psrc2[j + 2];
			j += 3;

			buffer0 += ((mlib_d64 *)&sum2)[1];
			buffer1 += ((mlib_d64 *)&sum1)[0];
			buffer2 += ((mlib_d64 *)&sum1)[1];
			buffer3 += ((mlib_d64 *)&sum2)[0];
		} else if (sign == 2) {
			buffer2 += psrc1[j + 0] * psrc2[j + 0];
			buffer3 += psrc1[j + 1] * psrc2[j + 1];
			j += 2;

			buffer0 += ((mlib_d64 *)&sum2)[0];
			buffer1 += ((mlib_d64 *)&sum2)[1];
			buffer2 += ((mlib_d64 *)&sum1)[0];
			buffer3 += ((mlib_d64 *)&sum1)[1];
		} else if (sign == 1) {
			buffer3 += psrc1[j + 0] * psrc2[j + 0];
			j++;

			buffer0 += ((mlib_d64 *)&sum1)[1];
			buffer1 += ((mlib_d64 *)&sum2)[0];
			buffer2 += ((mlib_d64 *)&sum2)[1];
			buffer3 += ((mlib_d64 *)&sum1)[0];
		} else if (sign == 0) {

			buffer0 += ((mlib_d64 *)&sum1)[0];
			buffer1 += ((mlib_d64 *)&sum1)[1];
			buffer2 += ((mlib_d64 *)&sum2)[0];
			buffer3 += ((mlib_d64 *)&sum2)[1];
		}

		psrc1 += src_stride1;
		psrc2 += src_stride2;
	}

	res[0] = buffer0;
	res[1] = buffer1;
	res[2] = buffer2;
	res[3] = buffer3;
}

/* *********************************************************** */

void
mlib_s_ImageCrossCorrel_U16_1(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_u16 *psrc1 = (mlib_u16 *)mlib_ImageGetData(img1);
	mlib_u16 *psrc2 = (mlib_u16 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src_stride1 = mlib_ImageGetStride(img1) >> 1;
	mlib_s32 src_stride2 = mlib_ImageGetStride(img2) >> 1;
	mlib_s32 src_chan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1);

/* indices */
	mlib_s32 i, j, l;

	if (src_stride1 == size_row && src_stride2 == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_s32 db_num;
	__m128i *sl1, *sl2;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {
		j = 0;

		mlib_s32 aligntwo =
		    ((16 - ((mlib_addr)psrc1 & 15)) & 15) -
		    ((16 - ((mlib_addr)psrc2 & 15)) & 15);

		for (j = 0; (j < size_row) &&
		    ((mlib_addr)(psrc1 +j) & 15); j++) {
			buffer0 += (mlib_d64)psrc1[j] * psrc2[j];
		}

		db_num = ((size_row - j) >> 3);
		sl1 = (__m128i *)(psrc1 + j);
		sl2 = (__m128i *)(psrc2 + j);
		__m128i load1, load2;
		__m128i sdl1, sdh1, sdl2, sdh2;
		__m128d fdl10, fdl11, fdh10, fdh11;
		__m128d fdl20, fdl21, fdh20, fdh21;
		__m128d fml0, fmh0, fml1, fmh1;
		__m128d fsum1, fsum2, fsum3, fsum4;
		fsum1 = _mm_setzero_pd();
		fsum2 = _mm_setzero_pd();
		fsum3 = _mm_setzero_pd();
		fsum4 = _mm_setzero_pd();
		mlib_d64 sbuff[2];

		j += (db_num << 3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			load1 = _mm_load_si128(sl1);
			if (0 == aligntwo) {
				load2 = _mm_load_si128(sl2);
			} else {
				load2 = _mm_loadu_si128(sl2);
			}
			sdl1 = _mm_unpacklo_epi16(load1, x_zero);
			sdh1 = _mm_unpackhi_epi16(load1, x_zero);
			sdl2 = _mm_unpacklo_epi16(load2, x_zero);
			sdh2 = _mm_unpackhi_epi16(load2, x_zero);
			fdl10 = _mm_cvtepi32_pd(sdl1);
			fdl11 = _mm_cvtepi32_pd(_mm_srli_si128(sdl1, 8));
			fdh10 = _mm_cvtepi32_pd(sdh1);
			fdh11 = _mm_cvtepi32_pd(_mm_srli_si128(sdh1, 8));
			fdl20 = _mm_cvtepi32_pd(sdl2);
			fdl21 = _mm_cvtepi32_pd(_mm_srli_si128(sdl2, 8));
			fdh20 = _mm_cvtepi32_pd(sdh2);
			fdh21 = _mm_cvtepi32_pd(_mm_srli_si128(sdh2, 8));

			fml0 = _mm_mul_pd(fdl10, fdl20);
			fml1 = _mm_mul_pd(fdl11, fdl21);
			fmh0 = _mm_mul_pd(fdh10, fdh20);
			fmh1 = _mm_mul_pd(fdh11, fdh21);

			fsum1 = _mm_add_pd(fsum1, fml0);
			fsum2 = _mm_add_pd(fsum2, fml1);
			fsum3 = _mm_add_pd(fsum3, fmh0);
			fsum4 = _mm_add_pd(fsum4, fmh1);

			sl1++;
			sl2++;
		}

		fsum1 = _mm_add_pd(fsum1, fsum2);
		fsum3 = _mm_add_pd(fsum3, fsum4);
		fsum1 = _mm_add_pd(fsum1, fsum3);

		_mm_storeu_pd(sbuff, fsum1);
		buffer0 += (mlib_d64)sbuff[0] + sbuff[1];

		for (; j < size_row; j++) {
			buffer0 += (mlib_d64)psrc1[j] * psrc2[j];
		}

		psrc1 += src_stride1;
		psrc2 += src_stride2;
	}

	res[0] = buffer0;
	res[1] = buffer1;
	res[2] = 0.0;
	res[3] = 0.0;
}

/* *********************************************************** */

void
mlib_s_ImageCrossCorrel_U16_2(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_u16 *psrc1 = (mlib_u16 *)mlib_ImageGetData(img1);
	mlib_u16 *psrc2 = (mlib_u16 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src_stride1 = mlib_ImageGetStride(img1) >> 1;
	mlib_s32 src_stride2 = mlib_ImageGetStride(img2) >> 1;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << 1;

/* indices */
	mlib_s32 i, j, l;

	if (src_stride1 == size_row && src_stride2 == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_d64 buffer2, buffer3;
	__m128i *sl1, *sl2;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 alignment = 1;
		mlib_s32 db_num;

		mlib_s32 aligntwo =
		    ((16 - ((mlib_addr)psrc1 & 15)) & 15) -
		    ((16 - ((mlib_addr)psrc2 & 15)) & 15);

		if ((mlib_addr)psrc1 & 1) {
			alignment = 0;
			buffer0 += (mlib_d64)psrc1[0] * psrc2[0];

			for (j = 1; (j < (size_row - 2)) &&
			    ((mlib_addr)(psrc1 + j) & 15); j += 2) {
				buffer1 +=
				    (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
				buffer0 +=
				    (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
			}
		} else {
			for (j = 0; (j < size_row) &&
			    ((mlib_addr)(psrc1 + j) & 15); j += 2) {
				buffer0 +=
				    (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
				buffer1 +=
				    (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
			}
		}

		db_num = ((size_row - j) >> 3);
		sl1 = (__m128i *)(psrc1 + j);
		sl2 = (__m128i *)(psrc2 + j);
		__m128i load1, load2;
		__m128i sdl1, sdh1, sdl2, sdh2;
		__m128d fdl10, fdl11, fdh10, fdh11;
		__m128d fdl20, fdl21, fdh20, fdh21;
		__m128d fml0, fmh0, fml1, fmh1;
		__m128d fsum1, fsum2, fsum3, fsum4;
		fsum1 = _mm_setzero_pd();
		fsum2 = _mm_setzero_pd();
		fsum3 = _mm_setzero_pd();
		fsum4 = _mm_setzero_pd();
		mlib_d64 sbuff[2];
		j += (db_num << 3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			load1 = _mm_load_si128(sl1);
			if (0 == aligntwo) {
				load2 = _mm_load_si128(sl2);
			} else {
				load2 = _mm_loadu_si128(sl2);
			}
			sdl1 = _mm_unpacklo_epi16(load1, x_zero);
			sdh1 = _mm_unpackhi_epi16(load1, x_zero);
			sdl2 = _mm_unpacklo_epi16(load2, x_zero);
			sdh2 = _mm_unpackhi_epi16(load2, x_zero);
			fdl10 = _mm_cvtepi32_pd(sdl1);
			fdl11 = _mm_cvtepi32_pd(_mm_srli_si128(sdl1, 8));
			fdh10 = _mm_cvtepi32_pd(sdh1);
			fdh11 = _mm_cvtepi32_pd(_mm_srli_si128(sdh1, 8));
			fdl20 = _mm_cvtepi32_pd(sdl2);
			fdl21 = _mm_cvtepi32_pd(_mm_srli_si128(sdl2, 8));
			fdh20 = _mm_cvtepi32_pd(sdh2);
			fdh21 = _mm_cvtepi32_pd(_mm_srli_si128(sdh2, 8));

			fml0 = _mm_mul_pd(fdl10, fdl20);
			fml1 = _mm_mul_pd(fdl11, fdl21);
			fmh0 = _mm_mul_pd(fdh10, fdh20);
			fmh1 = _mm_mul_pd(fdh11, fdh21);

			fsum1 = _mm_add_pd(fsum1, fml0);
			fsum2 = _mm_add_pd(fsum2, fml1);
			fsum3 = _mm_add_pd(fsum3, fmh0);
			fsum4 = _mm_add_pd(fsum4, fmh1);

			sl1++;
			sl2++;
		}

		fsum1 = _mm_add_pd(fsum1, fsum2);
		fsum3 = _mm_add_pd(fsum3, fsum4);
		fsum1 = _mm_add_pd(fsum1, fsum3);
		_mm_storeu_pd(sbuff, fsum1);

		buffer2 = (mlib_d64)sbuff[0];
		buffer3 = (mlib_d64)sbuff[1];

		if (alignment) {
			buffer0 += buffer2;
			buffer1 += buffer3;

			for (; j < size_row; j += 2) {
				buffer0 +=
				    (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
				buffer1 +=
				    (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
			}
		} else {
			buffer1 += buffer2;
			buffer0 += buffer3;

			for (; j < (size_row - 1); j += 2) {
				buffer1 +=
				    (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
				buffer0 +=
				    (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
			}

			buffer1 += (mlib_d64)psrc1[j] * psrc2[j];
		}

		psrc1 += src_stride1;
		psrc2 += src_stride2;
	}

	res[0] = buffer0;
	res[1] = buffer1;
	res[2] = 0.0;
	res[3] = 0.0;
}

/* *********************************************************** */

void
mlib_s_ImageCrossCorrel_U16_3back(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_u16 *psrc1 = (mlib_u16 *)mlib_ImageGetData(img1);
	mlib_u16 *psrc2 = (mlib_u16 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src_stride1 = mlib_ImageGetStride(img1) >> 1;
	mlib_s32 src_stride2 = mlib_ImageGetStride(img2) >> 1;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) * 3;

/* indices */
	mlib_s32 i, j, l;

	if (src_stride1 == size_row && src_stride2 == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
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
	__m128i *sl1, *sl2;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 sign = 0;
		buffer3 = buffer4 = buffer5 = 0.0;
		mlib_s32 alignment =
			((16 - ((mlib_addr)psrc1 & 15)) & 15) >> 1;
		mlib_s32 aligntwo =
		    ((16 - ((mlib_addr)psrc1 & 15)) & 15) -
		    ((16 - ((mlib_addr)psrc2 & 15)) & 15);

		for (j = 0;
		    (j < (size_row - 3)) && (j <= (alignment - 3));
		    j += 3) {
			buffer0 += (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
			buffer1 += (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
			buffer2 += (mlib_d64)psrc1[j + 2] * psrc2[j + 2];
		}

		if ((alignment - j) == 2) {
			buffer0 += (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
			buffer1 += (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
			j += 2;
			sign = 1;
		} else if ((alignment - j) == 1) {
			buffer0 += (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
			j += 1;
			sign = 2;
		}

		db_num = ((size_row - j) / 24);
		sl1 = (__m128i *)(psrc1 + j);
		sl2 = (__m128i *)(psrc2 + j);
		__m128i load10, load11, load12;
		__m128i load20, load21, load22;
		__m128i sdl0, sdh0, sdl1, sdh1, sdl2, sdh2;
		__m128i sd1, sd2, sd3, sd4, sd5, sd6;
		__m128i da1, da2, da3, da4, da5, da6;
		mlib_u32 *ub1 = (mlib_u32 *)&da1;
		mlib_u32 *ub2 = (mlib_u32 *)&da2;
		mlib_u32 *ub3 = (mlib_u32 *)&da3;
		mlib_u32 *ub4 = (mlib_u32 *)&da4;
		mlib_u32 *ub5 = (mlib_u32 *)&da5;
		mlib_u32 *ub6 = (mlib_u32 *)&da6;

		__m128i load1, load2;
		__m128i sdl10, sdh10, sdl20, sdh20;
		__m128i sdl11, sdh11, sdl21, sdh21;
		__m128i sdl12, sdh12, sdl22, sdh22;

		__m128 fsl10, fsh10, fsl20, fsh20;
		__m128 fsl11, fsh11, fsl21, fsh21;
		__m128 fsl12, fsh12, fsl22, fsh22;

		__m128 fml0, fmh0, fml1, fmh1, fml2, fmh2;
		__m128 fsum1, fsum2, fsum3, fsum4, fsum5, fsum6;
		fsum1 = _mm_setzero_ps();
		fsum2 = _mm_setzero_ps();
		fsum3 = _mm_setzero_ps();
		fsum4 = _mm_setzero_ps();
		fsum5 = _mm_setzero_ps();
		fsum6 = _mm_setzero_ps();

		j += (db_num * 24);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			load10 = _mm_load_si128(sl1);
			load11 = _mm_load_si128(sl1 + 1);
			load12 = _mm_load_si128(sl1 + 2);
			if (0 == aligntwo) {
				load20 = _mm_load_si128(sl2);
				load21 = _mm_load_si128(sl2 + 1);
				load22 = _mm_load_si128(sl2 + 2);
			} else {
				load20 = _mm_loadu_si128(sl2);
				load21 = _mm_loadu_si128(sl2 + 1);
				load22 = _mm_loadu_si128(sl2 + 2);
			}

			int en = 3;
			sdl10 = _mm_slli_epi32(
			    _mm_unpacklo_epi16(load10, x_zero), en);
			sdh10 = _mm_slli_epi32(
			    _mm_unpackhi_epi16(load10, x_zero), en);
			sdl11 = _mm_slli_epi32(
			    _mm_unpacklo_epi16(load11, x_zero), en);
			sdh11 = _mm_slli_epi32(
			    _mm_unpackhi_epi16(load11, x_zero), en);
			sdl12 = _mm_slli_epi32(
			    _mm_unpacklo_epi16(load12, x_zero), en);
			sdh12 = _mm_slli_epi32(
			    _mm_unpackhi_epi16(load12, x_zero), en);
			sdl20 = _mm_slli_epi32(
			    _mm_unpacklo_epi16(load20, x_zero), en);
			sdh20 = _mm_slli_epi32(
			    _mm_unpackhi_epi16(load20, x_zero), en);
			sdl21 = _mm_slli_epi32(
			    _mm_unpacklo_epi16(load21, x_zero), en);
			sdh21 = _mm_slli_epi32(
			    _mm_unpackhi_epi16(load21, x_zero), en);
			sdl22 = _mm_slli_epi32(
			    _mm_unpacklo_epi16(load22, x_zero), en);
			sdh22 = _mm_slli_epi32(
			    _mm_unpackhi_epi16(load22, x_zero), en);

			fsl10 = _mm_cvtepi32_ps(sdl10);
			fsh10 = _mm_cvtepi32_ps(sdh10);
			fsl11 = _mm_cvtepi32_ps(sdl11);
			fsh11 = _mm_cvtepi32_ps(sdh11);
			fsl12 = _mm_cvtepi32_ps(sdl12);
			fsh12 = _mm_cvtepi32_ps(sdh12);
			fsl20 = _mm_cvtepi32_ps(sdl20);
			fsh20 = _mm_cvtepi32_ps(sdh20);
			fsl21 = _mm_cvtepi32_ps(sdl21);
			fsh21 = _mm_cvtepi32_ps(sdh21);
			fsl22 = _mm_cvtepi32_ps(sdl22);
			fsh22 = _mm_cvtepi32_ps(sdh22);

			fml0 = _mm_mul_ps(fsl10, fsl20);
			fmh0 = _mm_mul_ps(fsh10, fsh20);
			fml1 = _mm_mul_ps(fsl11, fsl21);
			fmh1 = _mm_mul_ps(fsh11, fsh21);
			fml2 = _mm_mul_ps(fsl12, fsl22);
			fmh2 = _mm_mul_ps(fsh12, fsh22);

			fsum1 = _mm_add_ps(fsum1, fml0);
			fsum2 = _mm_add_ps(fsum2, fmh0);
			fsum3 = _mm_add_ps(fsum3, fml1);
			fsum4 = _mm_add_ps(fsum4, fmh1);
			fsum5 = _mm_add_ps(fsum5, fml2);
			fsum6 = _mm_add_ps(fsum6, fmh2);

			sl1 += 3;
			sl2 += 3;
		}

		if (l > 0) {

			buffer3 += (mlib_d64)((mlib_f32 *)&fsum1)[0] +
					((mlib_f32 *)&fsum1)[3] +
					((mlib_f32 *)&fsum2)[2] +
					((mlib_f32 *)&fsum3)[1] +
					((mlib_f32 *)&fsum4)[0] +
					((mlib_f32 *)&fsum4)[3] +
					((mlib_f32 *)&fsum5)[2] +
					((mlib_f32 *)&fsum6)[1];
			buffer4 += (mlib_d64)((mlib_f32 *)&fsum1)[1] +
					((mlib_f32 *)&fsum2)[0] +
					((mlib_f32 *)&fsum2)[3] +
					((mlib_f32 *)&fsum3)[2] +
					((mlib_f32 *)&fsum4)[1] +
					((mlib_f32 *)&fsum5)[0] +
					((mlib_f32 *)&fsum5)[3] +
					((mlib_f32 *)&fsum6)[2];
			buffer5 += (mlib_d64)((mlib_f32 *)&fsum1)[2] +
					((mlib_f32 *)&fsum2)[1] +
					((mlib_f32 *)&fsum3)[0] +
					((mlib_f32 *)&fsum3)[3] +
					((mlib_f32 *)&fsum4)[2] +
					((mlib_f32 *)&fsum5)[1] +
					((mlib_f32 *)&fsum6)[0] +
					((mlib_f32 *)&fsum6)[3];
		}

		for (; j < (size_row - 2); j += 3) {
			buffer3 += (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
			buffer4 += (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
			buffer5 += (mlib_d64)psrc1[j + 2] * psrc2[j + 2];
		}

		if (sign == 2) {
			buffer1 += (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
			buffer2 += (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
			buffer0 += buffer5;
			buffer1 += buffer3;
			buffer2 += buffer4;
		} else if (sign == 1) {
			buffer2 += (mlib_d64)psrc1[j] * psrc2[j];
			buffer0 += buffer4;
			buffer1 += buffer5;
			buffer2 += buffer3;
		} else if (sign == 0) {
			buffer0 += buffer3;
			buffer1 += buffer4;
			buffer2 += buffer5;
		}

		psrc1 += src_stride1;
		psrc2 += src_stride2;
	}

	res[0] = buffer0;
	res[1] = buffer1;
	res[2] = buffer2;
}

/* *********************************************************** */

void
mlib_s_ImageCrossCorrel_U16_3(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_u16 *psrc1 = (mlib_u16 *)mlib_ImageGetData(img1);
	mlib_u16 *psrc2 = (mlib_u16 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src_stride1 = mlib_ImageGetStride(img1) >> 1;
	mlib_s32 src_stride2 = mlib_ImageGetStride(img2) >> 1;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) * 3;

/* indices */
	mlib_s32 i, j, l;

	if (src_stride1 == size_row && src_stride2 == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
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
	__m128i *sl1, *sl2;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 sign = 0;
		buffer3 = buffer4 = buffer5 = 0.0;
		mlib_s32 alignment =
			((16 - ((mlib_addr)psrc1 & 15)) & 15) >> 1;
		mlib_s32 aligntwo =
		    ((16 - ((mlib_addr)psrc1 & 15)) & 15) -
		    ((16 - ((mlib_addr)psrc2 & 15)) & 15);

		for (j = 0;
		    (j < (size_row - 3)) && (j <= (alignment - 3));
		    j += 3) {
			buffer0 += (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
			buffer1 += (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
			buffer2 += (mlib_d64)psrc1[j + 2] * psrc2[j + 2];
		}

		if ((alignment - j) == 2) {
			buffer0 += (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
			buffer1 += (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
			j += 2;
			sign = 1;
		} else if ((alignment - j) == 1) {
			buffer0 += (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
			j += 1;
			sign = 2;
		}

		db_num = ((size_row - j) / 24);
		sl1 = (__m128i *)(psrc1 + j);
		sl2 = (__m128i *)(psrc2 + j);
		__m128i load10, load11, load12;
		__m128i load20, load21, load22;
		__m128i sdl0, sdh0, sdl1, sdh1, sdl2, sdh2;
		__m128i sd1, sd2, sd3, sd4, sd5, sd6;
		__m128i da1, da2, da3, da4, da5, da6;
		mlib_u32 *ub1 = (mlib_u32 *)&da1;
		mlib_u32 *ub2 = (mlib_u32 *)&da2;
		mlib_u32 *ub3 = (mlib_u32 *)&da3;
		mlib_u32 *ub4 = (mlib_u32 *)&da4;
		mlib_u32 *ub5 = (mlib_u32 *)&da5;
		mlib_u32 *ub6 = (mlib_u32 *)&da6;

		__m128i load1, load2;
		__m128i sdl10, sdh10, sdl20, sdh20;
		__m128i sdl11, sdh11, sdl21, sdh21;
		__m128i sdl12, sdh12, sdl22, sdh22;

		__m128d fdl10l, fdl10h, fdh10l, fdh10h;
		__m128d fdl11l, fdl11h, fdh11l, fdh11h;
		__m128d fdl12l, fdl12h, fdh12l, fdh12h;
		__m128d fdl20l, fdl20h, fdh20l, fdh20h;
		__m128d fdl21l, fdl21h, fdh21l, fdh21h;
		__m128d fdl22l, fdl22h, fdh22l, fdh22h;

		__m128d fml0l, fml0h, fmh0l, fmh0h;
		__m128d fml1l, fml1h, fmh1l, fmh1h;
		__m128d fml2l, fml2h, fmh2l, fmh2h;

		__m128d fsum1, fsum2, fsum3, fsum4, fsum5, fsum6;
		__m128d fsum7, fsum8, fsum9, fsum10, fsum11, fsum12;
		fsum1 = _mm_setzero_pd();
		fsum2 = _mm_setzero_pd();
		fsum3 = _mm_setzero_pd();
		fsum4 = _mm_setzero_pd();
		fsum5 = _mm_setzero_pd();
		fsum6 = _mm_setzero_pd();
		fsum7 = _mm_setzero_pd();
		fsum8 = _mm_setzero_pd();
		fsum9 = _mm_setzero_pd();
		fsum10 = _mm_setzero_pd();
		fsum11 = _mm_setzero_pd();
		fsum12 = _mm_setzero_pd();
		j += (db_num * 24);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			load10 = _mm_load_si128(sl1);
			load11 = _mm_load_si128(sl1 + 1);
			load12 = _mm_load_si128(sl1 + 2);
			if (0 == aligntwo) {
				load20 = _mm_load_si128(sl2);
				load21 = _mm_load_si128(sl2 + 1);
				load22 = _mm_load_si128(sl2 + 2);
			} else {
				load20 = _mm_loadu_si128(sl2);
				load21 = _mm_loadu_si128(sl2 + 1);
				load22 = _mm_loadu_si128(sl2 + 2);
			}

			sdl10 = _mm_unpacklo_epi16(load10, x_zero);
			sdh10 = _mm_unpackhi_epi16(load10, x_zero);
			sdl11 = _mm_unpacklo_epi16(load11, x_zero);
			sdh11 = _mm_unpackhi_epi16(load11, x_zero);
			sdl12 = _mm_unpacklo_epi16(load12, x_zero);
			sdh12 = _mm_unpackhi_epi16(load12, x_zero);
			sdl20 = _mm_unpacklo_epi16(load20, x_zero);
			sdh20 = _mm_unpackhi_epi16(load20, x_zero);
			sdl21 = _mm_unpacklo_epi16(load21, x_zero);
			sdh21 = _mm_unpackhi_epi16(load21, x_zero);
			sdl22 = _mm_unpacklo_epi16(load22, x_zero);
			sdh22 = _mm_unpackhi_epi16(load22, x_zero);

			fdl10l = _mm_cvtepi32_pd(sdl10);
			fdl10h = _mm_cvtepi32_pd(_mm_srli_si128(sdl10, 8));
			fdh10l = _mm_cvtepi32_pd(sdh10);
			fdh10h = _mm_cvtepi32_pd(_mm_srli_si128(sdh10, 8));
			fdl11l = _mm_cvtepi32_pd(sdl11);
			fdl11h = _mm_cvtepi32_pd(_mm_srli_si128(sdl11, 8));
			fdh11l = _mm_cvtepi32_pd(sdh11);
			fdh11h = _mm_cvtepi32_pd(_mm_srli_si128(sdh11, 8));
			fdl12l = _mm_cvtepi32_pd(sdl12);
			fdl12h = _mm_cvtepi32_pd(_mm_srli_si128(sdl12, 8));
			fdh12l = _mm_cvtepi32_pd(sdh12);
			fdh12h = _mm_cvtepi32_pd(_mm_srli_si128(sdh12, 8));

			fdl20l = _mm_cvtepi32_pd(sdl20);
			fdl20h = _mm_cvtepi32_pd(_mm_srli_si128(sdl20, 8));
			fdh20l = _mm_cvtepi32_pd(sdh20);
			fdh20h = _mm_cvtepi32_pd(_mm_srli_si128(sdh20, 8));
			fdl21l = _mm_cvtepi32_pd(sdl21);
			fdl21h = _mm_cvtepi32_pd(_mm_srli_si128(sdl21, 8));
			fdh21l = _mm_cvtepi32_pd(sdh21);
			fdh21h = _mm_cvtepi32_pd(_mm_srli_si128(sdh21, 8));
			fdl22l = _mm_cvtepi32_pd(sdl22);
			fdl22h = _mm_cvtepi32_pd(_mm_srli_si128(sdl22, 8));
			fdh22l = _mm_cvtepi32_pd(sdh22);
			fdh22h = _mm_cvtepi32_pd(_mm_srli_si128(sdh22, 8));

			fml0l = _mm_mul_pd(fdl10l, fdl20l);
			fml0h = _mm_mul_pd(fdl10h, fdl20h);
			fmh0l = _mm_mul_pd(fdh10l, fdh20l);
			fmh0h = _mm_mul_pd(fdh10h, fdh20h);
			fml1l = _mm_mul_pd(fdl11l, fdl21l);
			fml1h = _mm_mul_pd(fdl11h, fdl21h);
			fmh1l = _mm_mul_pd(fdh11l, fdh21l);
			fmh1h = _mm_mul_pd(fdh11h, fdh21h);
			fml2l = _mm_mul_pd(fdl12l, fdl22l);
			fml2h = _mm_mul_pd(fdl12h, fdl22h);
			fmh2l = _mm_mul_pd(fdh12l, fdh22l);
			fmh2h = _mm_mul_pd(fdh12h, fdh22h);

			fsum1 = _mm_add_pd(fsum1, fml0l);
			fsum2 = _mm_add_pd(fsum2, fml0h);
			fsum3 = _mm_add_pd(fsum3, fmh0l);
			fsum4 = _mm_add_pd(fsum4, fmh0h);
			fsum5 = _mm_add_pd(fsum5, fml1l);
			fsum6 = _mm_add_pd(fsum6, fml1h);
			fsum7 = _mm_add_pd(fsum7, fmh1l);
			fsum8 = _mm_add_pd(fsum8, fmh1h);
			fsum9 = _mm_add_pd(fsum9, fml2l);
			fsum10 = _mm_add_pd(fsum10, fml2h);
			fsum11 = _mm_add_pd(fsum11, fmh2l);
			fsum12 = _mm_add_pd(fsum12, fmh2h);

			sl1 += 3;
			sl2 += 3;
		}

		if (l > 0) {

			buffer3 += (mlib_d64)((mlib_d64 *)&fsum1)[0] +
					((mlib_d64 *)&fsum2)[1] +
					((mlib_d64 *)&fsum4)[0] +
					((mlib_d64 *)&fsum5)[1] +
					((mlib_d64 *)&fsum7)[0] +
					((mlib_d64 *)&fsum8)[1] +
					((mlib_d64 *)&fsum10)[0] +
					((mlib_d64 *)&fsum11)[1];
			buffer4 += (mlib_d64)((mlib_d64 *)&fsum1)[1] +
					((mlib_d64 *)&fsum3)[0] +
					((mlib_d64 *)&fsum4)[1] +
					((mlib_d64 *)&fsum6)[0] +
					((mlib_d64 *)&fsum7)[1] +
					((mlib_d64 *)&fsum9)[0] +
					((mlib_d64 *)&fsum10)[1] +
					((mlib_d64 *)&fsum12)[0];
			buffer5 += (mlib_d64)((mlib_d64 *)&fsum2)[0] +
					((mlib_d64 *)&fsum3)[1] +
					((mlib_d64 *)&fsum5)[0] +
					((mlib_d64 *)&fsum6)[1] +
					((mlib_d64 *)&fsum8)[0] +
					((mlib_d64 *)&fsum9)[1] +
					((mlib_d64 *)&fsum11)[0] +
					((mlib_d64 *)&fsum12)[1];
		}

		for (; j < (size_row - 2); j += 3) {
			buffer3 += (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
			buffer4 += (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
			buffer5 += (mlib_d64)psrc1[j + 2] * psrc2[j + 2];
		}

		if (sign == 2) {
			buffer1 += (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
			buffer2 += (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
			buffer0 += buffer5;
			buffer1 += buffer3;
			buffer2 += buffer4;
		} else if (sign == 1) {
			buffer2 += (mlib_d64)psrc1[j] * psrc2[j];
			buffer0 += buffer4;
			buffer1 += buffer5;
			buffer2 += buffer3;
		} else if (sign == 0) {
			buffer0 += buffer3;
			buffer1 += buffer4;
			buffer2 += buffer5;
		}

		psrc1 += src_stride1;
		psrc2 += src_stride2;
	}

	res[0] = buffer0;
	res[1] = buffer1;
	res[2] = buffer2;
}

/* *********************************************************** */

void
mlib_s_ImageCrossCorrel_U16_4(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_u16 *psrc1 = (mlib_u16 *)mlib_ImageGetData(img1);
	mlib_u16 *psrc2 = (mlib_u16 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src_stride1 = mlib_ImageGetStride(img1) >> 1;
	mlib_s32 src_stride2 = mlib_ImageGetStride(img2) >> 1;
	mlib_s32 src_chan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << (src_chan >> 1);

/* indices */
	mlib_s32 i, j, l;

	if (src_stride1 == size_row && src_stride2 == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_d64 buffer2 = 0.0;
	mlib_d64 buffer3 = 0.0;
	mlib_d64 buffer4, buffer5, buffer6, buffer7;
	mlib_s32 db_num;
	__m128i *sl1, *sl2;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 sign = 0;
		mlib_s32 alignment =
		    ((16 - ((mlib_addr)psrc1 & 15)) & 15) >> 1;
		mlib_s32 aligntwo =
		    ((16 - ((mlib_addr)psrc1 & 15)) & 15) -
		    ((16 - ((mlib_addr)psrc2 & 15)) & 15);

		buffer4 = buffer5 = buffer6 = buffer7 = 0.0;

		for (; (j < size_row - 3) && (alignment >= 4); j += 4) {
			buffer0 += (mlib_d64)psrc1[0] * psrc2[0];
			buffer1 += (mlib_d64)psrc1[1] * psrc2[1];
			buffer2 += (mlib_d64)psrc1[2] * psrc2[2];
			buffer3 += (mlib_d64)psrc1[3] * psrc2[3];
			alignment -= 4;
		}

		if (j < (size_row - 3)) {
			if ((alignment == 3)) {
				buffer0 +=
				    (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
				buffer1 +=
				    (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
				buffer2 +=
				    (mlib_d64)psrc1[j + 2] * psrc2[j + 2];
				j += 3;
				sign = 1;
			} else if ((alignment == 2)) {
				buffer0 +=
				    (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
				buffer1 +=
				    (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
				j += 2;
				sign = 2;
			} else if ((alignment == 1)) {
				buffer0 +=
				    (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
				j += 1;
				sign = 3;
			}
		}

		db_num = ((size_row - j) >> 3);
		sl1 = (__m128i *)(psrc1 + j);
		sl2 = (__m128i *)(psrc2 + j);
		__m128i load1, load2;
		__m128i sdl1, sdh1, sdl2, sdh2;
		__m128d fdl10, fdl11, fdh10, fdh11;
		__m128d fdl20, fdl21, fdh20, fdh21;
		__m128d fml0, fmh0, fml1, fmh1;
		__m128d fsum1, fsum2, fsum3, fsum4;
		fsum1 = _mm_setzero_pd();
		fsum2 = _mm_setzero_pd();
		fsum3 = _mm_setzero_pd();
		fsum4 = _mm_setzero_pd();
		mlib_d64 sbuff[4];
		j += (db_num << 3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			load1 = _mm_load_si128(sl1);
			if (0 == aligntwo) {
				load2 = _mm_load_si128(sl2);
			} else {
				load2 = _mm_loadu_si128(sl2);
			}

			sdl1 = _mm_unpacklo_epi16(load1, x_zero);
			sdh1 = _mm_unpackhi_epi16(load1, x_zero);
			sdl2 = _mm_unpacklo_epi16(load2, x_zero);
			sdh2 = _mm_unpackhi_epi16(load2, x_zero);
			fdl10 = _mm_cvtepi32_pd(sdl1);
			fdl11 = _mm_cvtepi32_pd(_mm_srli_si128(sdl1, 8));
			fdh10 = _mm_cvtepi32_pd(sdh1);
			fdh11 = _mm_cvtepi32_pd(_mm_srli_si128(sdh1, 8));
			fdl20 = _mm_cvtepi32_pd(sdl2);
			fdl21 = _mm_cvtepi32_pd(_mm_srli_si128(sdl2, 8));
			fdh20 = _mm_cvtepi32_pd(sdh2);
			fdh21 = _mm_cvtepi32_pd(_mm_srli_si128(sdh2, 8));

			fml0 = _mm_mul_pd(fdl10, fdl20);
			fml1 = _mm_mul_pd(fdl11, fdl21);
			fmh0 = _mm_mul_pd(fdh10, fdh20);
			fmh1 = _mm_mul_pd(fdh11, fdh21);

			fsum1 = _mm_add_pd(fsum1, fml0);
			fsum2 = _mm_add_pd(fsum2, fml1);
			fsum3 = _mm_add_pd(fsum3, fmh0);
			fsum4 = _mm_add_pd(fsum4, fmh1);

			sl1++;
			sl2++;
		}

		fsum1 = _mm_add_pd(fsum1, fsum3);
		fsum2 = _mm_add_pd(fsum2, fsum4);
		_mm_storeu_pd(sbuff, fsum1);
		_mm_storeu_pd((sbuff + 2), fsum2);

		buffer4 = (mlib_d64)sbuff[0];
		buffer5 = (mlib_d64)sbuff[1];
		buffer6 = (mlib_d64)sbuff[2];
		buffer7 = (mlib_d64)sbuff[3];

		for (; j < (size_row - 3); j += 4) {
			buffer4 += (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
			buffer5 += (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
			buffer6 += (mlib_d64)psrc1[j + 2] * psrc2[j + 2];
			buffer7 += (mlib_d64)psrc1[j + 3] * psrc2[j + 3];
		}

		if (sign == 3) {
			buffer1 += (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
			buffer2 += (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
			buffer3 += (mlib_d64)psrc1[j + 2] * psrc2[j + 2];
			j += 3;

			buffer0 += buffer7;
			buffer1 += buffer4;
			buffer2 += buffer5;
			buffer3 += buffer6;
		} else if (sign == 2) {
			buffer2 += (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
			buffer3 += (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
			j += 2;

			buffer0 += buffer6;
			buffer1 += buffer7;
			buffer2 += buffer4;
			buffer3 += buffer5;
		} else if (sign == 1) {
			buffer3 += (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
			j++;

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

		psrc1 += src_stride1;
		psrc2 += src_stride2;
	}

	res[0] = buffer0;
	res[1] = buffer1;
	res[2] = buffer2;
	res[3] = buffer3;
}

/* *********************************************************** */

void
mlib_s_ImageCrossCorrel_S16_1(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_s16 *psrc1 = (mlib_s16 *)mlib_ImageGetData(img1);
	mlib_s16 *psrc2 = (mlib_s16 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src_stride1 = mlib_ImageGetStride(img1) >> 1;
	mlib_s32 src_stride2 = mlib_ImageGetStride(img2) >> 1;
	mlib_s32 src_chan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1);

/* indices */
	mlib_s32 i, j, l;

	if (src_stride1 == size_row && src_stride2 == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_d64 fsumbuff[2];
	mlib_s32 db_num;
	__m128i *sl1, *sl2;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {
		j = 0;

		mlib_s32 aligntwo =
		    (((16 - ((mlib_addr)psrc1 & 15)) & 15) >> 1) -
		    (((16 - ((mlib_addr)psrc2 & 15)) & 15) >> 1);

		for (j = 0; (j < size_row) &&
		    ((mlib_addr)(psrc1 + j) & 15); j++) {
			buffer0 += psrc1[j] * psrc2[j];
		}

		db_num = ((size_row - j) >> 3);
		sl1 = (__m128i *)(psrc1 + j);
		sl2 = (__m128i *)(psrc2 + j);
		__m128i load1, load2, sd1;
		__m128d fd1, fd2;
		__m128d fsum1, fsum2;
		fsum1 = _mm_setzero_pd();
		fsum2 = _mm_setzero_pd();
		j += (db_num << 3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			load1 = _mm_load_si128(sl1);
			if (0 == aligntwo) {
				load2 = _mm_load_si128(sl2);
			} else {
				load2 = _mm_loadu_si128(sl2);
			}
			sd1 = _mm_madd_epi16(load1, load2);
			fd1 = _mm_cvtepi32_pd(sd1);
			fd2 = _mm_cvtepi32_pd(_mm_srli_si128(sd1, 8));
			fsum1 = _mm_add_pd(fsum1, fd1);
			fsum2 = _mm_add_pd(fsum2, fd2);
			sl1++;
			sl2++;
		}

		fsum1 = _mm_add_pd(fsum1, fsum2);
		_mm_storeu_pd(fsumbuff, fsum1);

		for (; j < size_row; j++) {
			buffer0 += psrc1[j] * psrc2[j];
		}

		buffer0 += fsumbuff[0];
		buffer1 += fsumbuff[1];
		psrc1 += src_stride1;
		psrc2 += src_stride2;
	}

	res[0] = buffer0;
	res[1] = buffer1;
	res[2] = 0.0;
	res[3] = 0.0;
}

/* *********************************************************** */

void
mlib_s_ImageCrossCorrel_S16_2(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_s16 *psrc1 = (mlib_s16 *)mlib_ImageGetData(img1);
	mlib_s16 *psrc2 = (mlib_s16 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src_stride1 = mlib_ImageGetStride(img1) >> 1;
	mlib_s32 src_stride2 = mlib_ImageGetStride(img2) >> 1;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << 1;

/* indices */
	mlib_s32 i, j, l;

	if (src_stride1 == size_row && src_stride2 == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_d64 sumbuff[2];
	__m128i *sl1, *sl2;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 alignment = 1;
		mlib_s32 db_num;
		mlib_s32 aligntwo =
		    (((16 - ((mlib_addr)psrc1 & 15)) & 15) >> 1) -
		    (((16 - ((mlib_addr)psrc2 & 15)) & 15) >> 1);

		if ((mlib_addr)psrc1 & 1) {
			alignment = 0;
			buffer0 += psrc1[0] * psrc2[0];

			for (j = 1; (j < (size_row - 2)) &&
			    ((mlib_addr)(psrc1 + j) & 15); j += 2) {
				buffer1 += psrc1[j + 0] * psrc2[j + 0];
				buffer0 += psrc1[j + 1] * psrc2[j + 1];
			}
		} else {
			for (j = 0; (j < size_row) &&
			    ((mlib_addr)(psrc1 + j) & 15); j += 2) {
				buffer0 += psrc1[j + 0] * psrc2[j + 0];
				buffer1 += psrc1[j + 1] * psrc2[j + 1];
			}
		}

		db_num = ((size_row - j) >> 3);
		sl1 = (__m128i *)(psrc1 + j);
		sl2 = (__m128i *)(psrc2 + j);
		__m128i load1, load2, sdl, sdh, sd1, sd2;
		__m128d fsum1, fsum2;
		__m128d fd1, fd2, fd3, fd4;
		fsum1 = _mm_setzero_pd();
		fsum2 = _mm_setzero_pd();

		j += (db_num << 3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			load1 = _mm_load_si128(sl1);
			if (0 == aligntwo) {
				load2 = _mm_load_si128(sl2);
			} else {
				load2 = _mm_loadu_si128(sl2);
			}
			sdl = _mm_mullo_epi16(load1, load2);
			sdh = _mm_mulhi_epi16(load1, load2);
			sd1 = _mm_unpacklo_epi16(sdl, sdh);
			sd2 = _mm_unpackhi_epi16(sdl, sdh);
			fd1 = _mm_cvtepi32_pd(sd1);
			fd2 = _mm_cvtepi32_pd(_mm_srli_si128(sd1, 8));
			fd3 = _mm_cvtepi32_pd(sd2);
			fd4 = _mm_cvtepi32_pd(_mm_srli_si128(sd2, 8));
			fsum1 = _mm_add_pd(fsum1, fd1);
			fsum2 = _mm_add_pd(fsum2, fd2);
			fsum1 = _mm_add_pd(fsum1, fd3);
			fsum2 = _mm_add_pd(fsum2, fd4);

			sl1++;
			sl2++;
		}

		fsum1 = _mm_add_pd(fsum1, fsum2);
		_mm_storeu_pd(sumbuff, fsum1);

		if (alignment) {
			buffer0 += sumbuff[0];
			buffer1 += sumbuff[1];

			for (; j < size_row; j += 2) {
				buffer0 += psrc1[j + 0] * psrc2[j + 0];
				buffer1 += psrc1[j + 1] * psrc2[j + 1];
			}
		} else {
			buffer1 += sumbuff[0];
			buffer0 += sumbuff[1];

			for (; j < (size_row - 1); j += 2) {
				buffer1 += psrc1[j + 0] * psrc2[j + 0];
				buffer0 += psrc1[j + 1] * psrc2[j + 1];
			}

			buffer1 += psrc1[j] * psrc2[j];
		}

		psrc1 += src_stride1;
		psrc2 += src_stride2;
	}

	res[0] = buffer0;
	res[1] = buffer1;
	res[2] = 0.0;
	res[3] = 0.0;
}

/* *********************************************************** */

void
mlib_s_ImageCrossCorrel_S16_4(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_s16 *psrc1 = (mlib_s16 *)mlib_ImageGetData(img1);
	mlib_s16 *psrc2 = (mlib_s16 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src_stride1 = mlib_ImageGetStride(img1) >> 1;
	mlib_s32 src_stride2 = mlib_ImageGetStride(img2) >> 1;
	mlib_s32 src_chan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << (src_chan >> 1);

/* indices */
	mlib_s32 i, j, l;

	if (src_stride1 == size_row && src_stride2 == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_d64 buffer2 = 0.0;
	mlib_d64 buffer3 = 0.0;
	mlib_d64 sumbuff[4];
	mlib_s32 db_num;
	__m128i *sl1, *sl2;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 sign = 0;
		mlib_s32 alignment =
		    ((16 - ((mlib_addr)psrc1 & 15)) & 15) >> 1;
		mlib_s32 aligntwo = alignment -
		    (((16 - ((mlib_addr)psrc2 & 15)) & 15) >> 1);

		for (; (j < size_row - 3) && (alignment >= 4); j += 4) {
			buffer0 += psrc1[0] * psrc2[0];
			buffer1 += psrc1[1] * psrc2[1];
			buffer2 += psrc1[2] * psrc2[2];
			buffer3 += psrc1[3] * psrc2[3];
			alignment -= 4;
		}

		if (j < (size_row - 3)) {
			if ((alignment == 3)) {
				buffer0 += psrc1[j + 0] * psrc2[j + 0];
				buffer1 += psrc1[j + 1] * psrc2[j + 1];
				buffer2 += psrc1[j + 2] * psrc2[j + 2];
				j += 3;
				sign = 1;
			} else if ((alignment == 2)) {
				buffer0 += psrc1[j + 0] * psrc2[j + 0];
				buffer1 += psrc1[j + 1] * psrc2[j + 1];
				j += 2;
				sign = 2;
			} else if ((alignment == 1)) {
				buffer0 += psrc1[j + 0] * psrc2[j + 0];
				j += 1;
				sign = 3;
			}
		}

		db_num = ((size_row - j) >> 3);
		sl1 = (__m128i *)(psrc1 + j);
		sl2 = (__m128i *)(psrc2 + j);
		__m128i load1, load2, sdl, sdh, sd1, sd2;
		__m128d fsum1, fsum2;
		__m128d fd1, fd2, fd3, fd4;
		fsum1 = _mm_setzero_pd();
		fsum2 = _mm_setzero_pd();
		j += (db_num << 3);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			load1 = _mm_load_si128(sl1);
			if (0 == aligntwo) {
				load2 = _mm_load_si128(sl2);
			} else {
				load2 = _mm_loadu_si128(sl2);
			}
			sdl = _mm_mullo_epi16(load1, load2);
			sdh = _mm_mulhi_epi16(load1, load2);
			sd1 = _mm_unpacklo_epi16(sdl, sdh);
			sd2 = _mm_unpackhi_epi16(sdl, sdh);
			fd1 = _mm_cvtepi32_pd(sd1);
			fd2 = _mm_cvtepi32_pd(_mm_srli_si128(sd1, 8));
			fd3 = _mm_cvtepi32_pd(sd2);
			fd4 = _mm_cvtepi32_pd(_mm_srli_si128(sd2, 8));
			fsum1 = _mm_add_pd(fsum1, fd1);
			fsum2 = _mm_add_pd(fsum2, fd2);
			fsum1 = _mm_add_pd(fsum1, fd3);
			fsum2 = _mm_add_pd(fsum2, fd4);

			sl1++;
			sl2++;
		}

		_mm_storeu_pd(sumbuff, fsum1);
		_mm_storeu_pd((sumbuff + 2), fsum2);

		for (; j < (size_row - 3); j += 4) {
			sumbuff[0] += psrc1[j + 0] * psrc2[j + 0];
			sumbuff[1] += psrc1[j + 1] * psrc2[j + 1];
			sumbuff[2] += psrc1[j + 2] * psrc2[j + 2];
			sumbuff[3] += psrc1[j + 3] * psrc2[j + 3];
		}

		if (sign == 3) {
			buffer1 += psrc1[j + 0] * psrc2[j + 0];
			buffer2 += psrc1[j + 1] * psrc2[j + 1];
			buffer3 += psrc1[j + 2] * psrc2[j + 2];
			j += 3;

			buffer0 += sumbuff[3];
			buffer1 += sumbuff[0];
			buffer2 += sumbuff[1];
			buffer3 += sumbuff[2];
		} else if (sign == 2) {
			buffer2 += psrc1[j + 0] * psrc2[j + 0];
			buffer3 += psrc1[j + 1] * psrc2[j + 1];
			j += 2;

			buffer0 += sumbuff[2];
			buffer1 += sumbuff[3];
			buffer2 += sumbuff[0];
			buffer3 += sumbuff[1];
		} else if (sign == 1) {
			buffer3 += psrc1[j + 0] * psrc2[j + 0];
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

		psrc1 += src_stride1;
		psrc2 += src_stride2;
	}

	res[0] = buffer0;
	res[1] = buffer1;
	res[2] = buffer2;
	res[3] = buffer3;
}

/* *********************************************************** */

void
mlib_s_ImageCrossCorrel_S16_3(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_s16 *psrc1 = (mlib_s16 *)mlib_ImageGetData(img1);
	mlib_s16 *psrc2 = (mlib_s16 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src_stride1 = mlib_ImageGetStride(img1) >> 1;
	mlib_s32 src_stride2 = mlib_ImageGetStride(img2) >> 1;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) * 3;

/* indices */
	mlib_s32 i, j, l;

	if (src_stride1 == size_row && src_stride2 == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
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
	__m128i *sl1, *sl2;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 sign = 0;
		mlib_s32 alignment =
			((16 - ((mlib_addr)psrc1 & 15)) & 15) >> 1;
		mlib_s32 aligntwo = alignment -
		    (((16 - ((mlib_addr)psrc2 & 15)) & 15) >> 1);

		for (j = 0;
		    (j < (size_row - 3)) && (j <= (alignment - 3));
		    j += 3) {
			buffer0 += psrc1[j + 0] * psrc2[j + 0];
			buffer1 += psrc1[j + 1] * psrc2[j + 1];
			buffer2 += psrc1[j + 2] * psrc2[j + 2];
		}

		if ((alignment - j) == 2) {
			buffer0 += psrc1[j + 0] * psrc2[j + 0];
			buffer1 += psrc1[j + 1] * psrc2[j + 1];
			j += 2;
			sign = 1;
		} else if ((alignment - j) == 1) {
			buffer0 += psrc1[j + 0] * psrc2[j + 0];
			j += 1;
			sign = 2;
		}

		db_num = ((size_row - j) / 24);
		sl1 = (__m128i *)(psrc1 + j);
		sl2 = (__m128i *)(psrc2 + j);
		__m128i load10, load11, load12;
		__m128i load20, load21, load22;
		__m128i sdl0, sdh0, sdl1, sdh1, sdl2, sdh2;
		__m128i sd1, sd2, sd3, sd4, sd5, sd6;
		__m128d fsd10, fsd20, fsd30, fsd40, fsd50, fsd60;
		__m128d fsd11, fsd21, fsd31, fsd41, fsd51, fsd61;
		__m128d fsum1, fsum2, fsum3;
		mlib_d64 sumbuff1[2], sumbuff2[2], sumbuff3[2];
		fsum1 = _mm_setzero_pd();
		fsum2 = _mm_setzero_pd();
		fsum3 = _mm_setzero_pd();

		j += (db_num * 24);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			load10 = _mm_load_si128(sl1);
			load11 = _mm_load_si128(sl1 + 1);
			load12 = _mm_load_si128(sl1 + 2);
			if (0 == aligntwo) {
				load20 = _mm_load_si128(sl2);
				load21 = _mm_load_si128(sl2 + 1);
				load22 = _mm_load_si128(sl2 + 2);
			} else {
				load20 = _mm_loadu_si128(sl2);
				load21 = _mm_loadu_si128(sl2 + 1);
				load22 = _mm_loadu_si128(sl2 + 2);
			}

			sdl0 = _mm_mullo_epi16(load10, load20);
			sdh0 = _mm_mulhi_epi16(load10, load20);
			sd1 = _mm_unpacklo_epi16(sdl0, sdh0);
			sd2 = _mm_unpackhi_epi16(sdl0, sdh0);

			sdl1 = _mm_mullo_epi16(load11, load21);
			sdh1 = _mm_mulhi_epi16(load11, load21);
			sd3 = _mm_unpacklo_epi16(sdl1, sdh1);
			sd4 = _mm_unpackhi_epi16(sdl1, sdh1);

			sdl2 = _mm_mullo_epi16(load12, load22);
			sdh2 = _mm_mulhi_epi16(load12, load22);
			sd5 = _mm_unpacklo_epi16(sdl2, sdh2);
			sd6 = _mm_unpackhi_epi16(sdl2, sdh2);

			fsd10 = _mm_cvtepi32_pd(sd1);
			fsd11 = _mm_cvtepi32_pd(_mm_srli_si128(sd1, 8));
			fsd20 = _mm_cvtepi32_pd(sd2);
			fsd21 = _mm_cvtepi32_pd(_mm_srli_si128(sd2, 8));
			fsd30 = _mm_cvtepi32_pd(sd3);
			fsd31 = _mm_cvtepi32_pd(_mm_srli_si128(sd3, 8));
			fsd40 = _mm_cvtepi32_pd(sd4);
			fsd41 = _mm_cvtepi32_pd(_mm_srli_si128(sd4, 8));
			fsd50 = _mm_cvtepi32_pd(sd5);
			fsd51 = _mm_cvtepi32_pd(_mm_srli_si128(sd5, 8));
			fsd60 = _mm_cvtepi32_pd(sd6);
			fsd61 = _mm_cvtepi32_pd(_mm_srli_si128(sd6, 8));

			fsum1 = _mm_add_pd(fsum1, fsd10);
			fsum2 = _mm_add_pd(fsum2, fsd11);
			fsum3 = _mm_add_pd(fsum3, fsd20);
			fsum1 = _mm_add_pd(fsum1, fsd21);
			fsum2 = _mm_add_pd(fsum2, fsd30);
			fsum3 = _mm_add_pd(fsum3, fsd31);
			fsum1 = _mm_add_pd(fsum1, fsd40);
			fsum2 = _mm_add_pd(fsum2, fsd41);
			fsum3 = _mm_add_pd(fsum3, fsd50);
			fsum1 = _mm_add_pd(fsum1, fsd51);
			fsum2 = _mm_add_pd(fsum2, fsd60);
			fsum3 = _mm_add_pd(fsum3, fsd61);

			sl1 += 3;
			sl2 += 3;
		}

		_mm_storeu_pd(sumbuff1, fsum1);
		_mm_storeu_pd(sumbuff2, fsum2);
		_mm_storeu_pd(sumbuff3, fsum3);

		buffer3 = sumbuff1[0] + sumbuff2[1];
		buffer4 = sumbuff1[1] + sumbuff3[0];
		buffer5 = sumbuff2[0] + sumbuff3[1];

		for (; j < (size_row - 2); j += 3) {
			buffer3 += psrc1[j + 0] * psrc2[j + 0];
			buffer4 += psrc1[j + 1] * psrc2[j + 1];
			buffer5 += psrc1[j + 2] * psrc2[j + 2];
		}

		if (sign == 2) {
			buffer1 += psrc1[j + 0] * psrc2[j + 0];
			buffer2 += psrc1[j + 1] * psrc2[j + 1];
			buffer0 += buffer5;
			buffer1 += buffer3;
			buffer2 += buffer4;
		} else if (sign == 1) {
			buffer2 += psrc1[j] * psrc2[j];
			buffer0 += buffer4;
			buffer1 += buffer5;
			buffer2 += buffer3;
		} else if (sign == 0) {
			buffer0 += buffer3;
			buffer1 += buffer4;
			buffer2 += buffer5;
		}

		psrc1 += src_stride1;
		psrc2 += src_stride2;
	}

	res[0] = buffer0;
	res[1] = buffer1;
	res[2] = buffer2;
}

/* *********************************************************** */

void
mlib_s_ImageCrossCorrel_S32_1(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_s32 *psrc1 = (mlib_s32 *)mlib_ImageGetData(img1);
	mlib_s32 *psrc2 = (mlib_s32 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src_stride1 = mlib_ImageGetStride(img1) >> 2;
	mlib_s32 src_stride2 = mlib_ImageGetStride(img2) >> 2;

	mlib_s32 src_chan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1);

/* indices */
	mlib_s32 i, j, l;

	if (src_stride1 == size_row && src_stride2 == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 sumbuff[2];
	mlib_s32 db_num;
	__m128i *sl1, *sl2;

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 aligntwo =
		    ((mlib_addr)psrc1 & 15) -
		    ((mlib_addr)psrc2 & 15);

		for (j = 0; (j < size_row) &&
		    ((mlib_addr)(psrc1 + j) & 15); j++) {
			buffer0 += (mlib_d64)psrc1[j] * psrc2[j];
		}

		db_num = ((size_row - j) >> 2);
		sl1 = (__m128i *)(psrc1 + j);
		sl2 = (__m128i *)(psrc2 + j);
		__m128i load1, load2;
		__m128d fd1, fd2, fd3, fd4, fs1, fs2, fsum1, fsum2;
		fsum1 = _mm_setzero_pd();
		fsum2 = _mm_setzero_pd();
		j += (db_num << 2);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			load1 = _mm_load_si128(sl1);
			if (0 == aligntwo) {
				load2 = _mm_load_si128(sl2);
			} else {
				load2 = _mm_loadu_si128(sl2);
			}
			fd1 = _mm_cvtepi32_pd(load1);
			fd2 = _mm_cvtepi32_pd(_mm_srli_si128(load1, 8));
			fd3 = _mm_cvtepi32_pd(load2);
			fd4 = _mm_cvtepi32_pd(_mm_srli_si128(load2, 8));

			fs1 = _mm_mul_pd(fd1, fd3);
			fs2 = _mm_mul_pd(fd2, fd4);
			fsum1 = _mm_add_pd(fsum1, fs1);
			fsum2 = _mm_add_pd(fsum2, fs2);
			sl1++;
			sl2++;
		}

		fsum1 = _mm_add_pd(fsum1, fsum2);
		_mm_storeu_pd(sumbuff, fsum1);

		for (; j < size_row; j++) {
			buffer0 += (mlib_d64)psrc1[j] * psrc2[j];
		}

		buffer0 += sumbuff[0] + sumbuff[1];
		psrc1 += src_stride1;
		psrc2 += src_stride2;
	}

	res[0] = buffer0;
	res[1] = 0.0;
	res[2] = 0.0;
	res[3] = 0.0;
}

/* *********************************************************** */

void
mlib_s_ImageCrossCorrel_S32_2(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_s32 *psrc1 = (mlib_s32 *)mlib_ImageGetData(img1);
	mlib_s32 *psrc2 = (mlib_s32 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src_stride1 = mlib_ImageGetStride(img1) >> 2;
	mlib_s32 src_stride2 = mlib_ImageGetStride(img2) >> 2;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << 1;

/* indices */
	mlib_s32 i, j, l;

	if (src_stride1 == size_row && src_stride2 == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_d64 sumbuff[2];
	__m128i *sl1, *sl2;

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 alignment = 1;
		mlib_s32 db_num;
		sumbuff[0] = sumbuff[1] = 0.0;
		mlib_s32 aligntwo =
		    ((16 - ((mlib_addr)psrc1 & 15)) & 15) -
		    ((16 - ((mlib_addr)psrc2 & 15)) & 15);

		if ((mlib_addr)psrc1 & 1) {
			alignment = 0;
			buffer0 +=
			    (mlib_d64)psrc1[0] * psrc2[0];

			for (j = 1; (j < (size_row - 2)) &&
			    ((mlib_addr)(psrc1 + j) & 15); j += 2) {
				buffer1 +=
				    (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
				buffer0 +=
				    (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
			}
		} else {
			for (j = 0; (j < size_row) &&
			    ((mlib_addr)(psrc1 + j) & 15); j += 2) {
				buffer0 +=
				    (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
				buffer1 +=
				    (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
			}
		}

		db_num = ((size_row - j) >> 2);
		sl1 = (__m128i *)(psrc1 + j);
		sl2 = (__m128i *)(psrc2 + j);
		__m128i load1, load2;
		__m128d fd1, fd2, fd3, fd4, fs1, fs2, fsum1, fsum2;
		fsum1 = _mm_setzero_pd();
		fsum2 = _mm_setzero_pd();

		j += (db_num << 2);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			load1 = _mm_load_si128(sl1);
			if (0 == aligntwo) {
				load2 = _mm_load_si128(sl2);
			} else {
				load2 = _mm_loadu_si128(sl2);
			}
			fd1 = _mm_cvtepi32_pd(load1);
			fd2 = _mm_cvtepi32_pd(_mm_srli_si128(load1, 8));
			fd3 = _mm_cvtepi32_pd(load2);
			fd4 = _mm_cvtepi32_pd(_mm_srli_si128(load2, 8));
			fs1 = _mm_mul_pd(fd1, fd3);
			fs2 = _mm_mul_pd(fd2, fd4);
			fsum1 = _mm_add_pd(fsum1, fs1);
			fsum2 = _mm_add_pd(fsum2, fs2);
			sl1++;
			sl2++;
		}

		fsum1 = _mm_add_pd(fsum1, fsum2);
		_mm_storeu_pd((void *)sumbuff, fsum1);

		if (alignment) {
			buffer0 += sumbuff[0];
			buffer1 += sumbuff[1];

			for (; j < size_row; j += 2) {
				buffer0 +=
				    (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
				buffer1 +=
				    (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
			}
		} else {
			buffer1 += sumbuff[0];
			buffer0 += sumbuff[1];

			for (; j < (size_row - 1); j += 2) {
				buffer1 +=
				    (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
				buffer0 +=
				    (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
			}

			buffer1 += (mlib_d64)psrc1[j] * psrc2[j];
		}

		psrc1 += src_stride1;
		psrc2 += src_stride2;
	}

	res[0] = buffer0;
	res[1] = buffer1;
	res[2] = 0.0;
	res[3] = 0.0;
}

/* *********************************************************** */

void
mlib_s_ImageCrossCorrel_S32_4(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_s32 *psrc1 = (mlib_s32 *)mlib_ImageGetData(img1);
	mlib_s32 *psrc2 = (mlib_s32 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src_stride1 = mlib_ImageGetStride(img1) >> 2;
	mlib_s32 src_stride2 = mlib_ImageGetStride(img2) >> 2;
	mlib_s32 src_chan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << 2;

/* indices */
	mlib_s32 i, j, l;

	if (src_stride1 == size_row && src_stride2 == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_d64 buffer2 = 0.0;
	mlib_d64 buffer3 = 0.0;
	mlib_d64 sumbuff[4];
	mlib_s32 db_num;
	__m128i *sl1, *sl2;

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 sign = 0;
		mlib_s32 alignment =
		    ((16 - ((mlib_addr)psrc1 & 15)) & 15) >> 2;
		mlib_s32 aligntwo =
		    ((mlib_addr)psrc1 & 15) -
		    ((mlib_addr)psrc2 & 15);

		for (; (j < size_row - 3) && (alignment >= 4); j += 4) {
			buffer0 += (mlib_d64)psrc1[0] * psrc2[0];
			buffer1 += (mlib_d64)psrc1[1] * psrc2[1];
			buffer2 += (mlib_d64)psrc1[2] * psrc2[2];
			buffer3 += (mlib_d64)psrc1[3] * psrc2[3];
			alignment -= 4;
		}

		if (j < (size_row - 3)) {
			if ((alignment == 3)) {
				buffer0 +=
				    (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
				buffer1 +=
				    (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
				buffer2 +=
				    (mlib_d64)psrc1[j + 2] * psrc2[j + 2];
				j += 3;
				sign = 1;
			} else if ((alignment == 2)) {
				buffer0 +=
				    (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
				buffer1 +=
				    (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
				j += 2;
				sign = 2;
			} else if ((alignment == 1)) {
				buffer0 +=
				    (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
				j += 1;
				sign = 3;
			}
		}

		db_num = ((size_row - j) >> 2);
		sl1 = (__m128i *)(psrc1 + j);
		sl2 = (__m128i *)(psrc2 + j);
		__m128i load1, load2;
		__m128d fd1, fd2, fd3, fd4, fs1, fs2, fsum1, fsum2;
		fsum1 = _mm_setzero_pd();
		fsum2 = _mm_setzero_pd();
		j += (db_num << 2);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			load1 = _mm_load_si128(sl1);
			if (0 == aligntwo) {
				load2 = _mm_load_si128(sl2);
			} else {
				load2 = _mm_loadu_si128(sl2);
			}
			fd1 = _mm_cvtepi32_pd(load1);
			fd2 = _mm_cvtepi32_pd(_mm_srli_si128(load1, 8));
			fd3 = _mm_cvtepi32_pd(load2);
			fd4 = _mm_cvtepi32_pd(_mm_srli_si128(load2, 8));

			fs1 = _mm_mul_pd(fd1, fd3);
			fs2 = _mm_mul_pd(fd2, fd4);
			fsum1 = _mm_add_pd(fsum1, fs1);
			fsum2 = _mm_add_pd(fsum2, fs2);
			sl1++;
			sl2++;
		}

		_mm_storeu_pd((void *)sumbuff, fsum1);
		_mm_storeu_pd((void *)(sumbuff + 2), fsum2);

		for (; j < (size_row - 3); j += 4) {
			sumbuff[0] += (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
			sumbuff[1] += (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
			sumbuff[2] += (mlib_d64)psrc1[j + 2] * psrc2[j + 2];
			sumbuff[3] += (mlib_d64)psrc1[j + 3] * psrc2[j + 3];
		}

		if (sign == 3) {
			buffer1 += (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
			buffer2 += (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
			buffer3 += (mlib_d64)psrc1[j + 2] * psrc2[j + 2];
			j += 3;

			buffer0 += sumbuff[3];
			buffer1 += sumbuff[0];
			buffer2 += sumbuff[1];
			buffer3 += sumbuff[2];
		} else if (sign == 2) {
			buffer2 += (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
			buffer3 += (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
			j += 2;

			buffer0 += sumbuff[2];
			buffer1 += sumbuff[3];
			buffer2 += sumbuff[0];
			buffer3 += sumbuff[1];
		} else if (sign == 1) {
			buffer3 += (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
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

		psrc1 += src_stride1;
		psrc2 += src_stride2;
	}

	res[0] = buffer0;
	res[1] = buffer1;
	res[2] = buffer2;
	res[3] = buffer3;
}

/* *********************************************************** */

void
mlib_s_ImageCrossCorrel_S32_3(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_s32 *psrc1 = (mlib_s32 *)mlib_ImageGetData(img1);
	mlib_s32 *psrc2 = (mlib_s32 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src_stride1 = mlib_ImageGetStride(img1) >> 2;
	mlib_s32 src_stride2 = mlib_ImageGetStride(img2) >> 2;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) * 3;

/* indices */
	mlib_s32 i, j, l;

	if (src_stride1 == size_row && src_stride2 == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
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
	__m128i *sl1, *sl2;

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 sign = 0;
		mlib_s32 alignment =
			((16 - ((mlib_addr)psrc1 & 15)) & 15) >> 2;
		mlib_s32 aligntwo =
		    ((mlib_addr)psrc1 & 15) -
		    ((mlib_addr)psrc2 & 15);

		for (j = 0;
		    (j < (size_row - 3)) && (j <= (alignment - 3));
		    j += 3) {
			buffer0 += (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
			buffer1 += (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
			buffer2 += (mlib_d64)psrc1[j + 2] * psrc2[j + 2];
		}

		if ((alignment - j) == 2) {
			buffer0 += (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
			buffer1 += (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
			j += 2;
			sign = 1;
		} else if ((alignment - j) == 1) {
			buffer0 += (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
			j += 1;
			sign = 2;
		}

		db_num = (size_row - j) / 12;
		sl1 = (__m128i *)(psrc1 + j);
		sl2 = (__m128i *)(psrc2 + j);
		__m128i load10, load11, load12;
		__m128i load20, load21, load22;
		__m128d fd10, fd11, fd12, fd13, fd14, fd15;
		__m128d fd20, fd21, fd22, fd23, fd24, fd25;
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
			load10 = _mm_load_si128(sl1);
			load11 = _mm_load_si128(sl1 + 1);
			load12 = _mm_load_si128(sl1 + 2);
			if (0 == aligntwo) {
				load20 = _mm_load_si128(sl2);
				load21 = _mm_load_si128(sl2 + 1);
				load22 = _mm_load_si128(sl2 + 2);
			} else {
				load20 = _mm_loadu_si128(sl2);
				load21 = _mm_loadu_si128(sl2 + 1);
				load22 = _mm_loadu_si128(sl2 + 2);
			}

			fd10 = _mm_cvtepi32_pd(load10);
			fd11 = _mm_cvtepi32_pd(_mm_srli_si128(load10, 8));
			fd12 = _mm_cvtepi32_pd(load11);
			fd13 = _mm_cvtepi32_pd(_mm_srli_si128(load11, 8));
			fd14 = _mm_cvtepi32_pd(load12);
			fd15 = _mm_cvtepi32_pd(_mm_srli_si128(load12, 8));

			fd20 = _mm_cvtepi32_pd(load20);
			fd21 = _mm_cvtepi32_pd(_mm_srli_si128(load20, 8));
			fd22 = _mm_cvtepi32_pd(load21);
			fd23 = _mm_cvtepi32_pd(_mm_srli_si128(load21, 8));
			fd24 = _mm_cvtepi32_pd(load22);
			fd25 = _mm_cvtepi32_pd(_mm_srli_si128(load22, 8));

			fs0 = _mm_mul_pd(fd10, fd20);
			fs1 = _mm_mul_pd(fd11, fd21);
			fs2 = _mm_mul_pd(fd12, fd22);
			fs3 = _mm_mul_pd(fd13, fd23);
			fs4 = _mm_mul_pd(fd14, fd24);
			fs5 = _mm_mul_pd(fd15, fd25);

			fsum0 = _mm_add_pd(fsum0, fs0);
			fsum1 = _mm_add_pd(fsum1, fs1);
			fsum2 = _mm_add_pd(fsum2, fs2);
			fsum3 = _mm_add_pd(fsum3, fs3);
			fsum4 = _mm_add_pd(fsum4, fs4);
			fsum5 = _mm_add_pd(fsum5, fs5);
			sl1 += 3;
			sl2 += 3;
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
			buffer3 += (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
			buffer4 += (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
			buffer5 += (mlib_d64)psrc1[j + 2] * psrc2[j + 2];
		}

		if (sign == 2) {
			buffer1 += (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
			buffer2 += (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
			buffer0 += buffer5;
			buffer1 += buffer3;
			buffer2 += buffer4;
		} else if (sign == 1) {
			buffer2 += (mlib_d64)psrc1[j] * psrc2[j];
			buffer0 += buffer4;
			buffer1 += buffer5;
			buffer2 += buffer3;
		} else if (sign == 0) {
			buffer0 += buffer3;
			buffer1 += buffer4;
			buffer2 += buffer5;
		}

		psrc1 += src_stride1;
		psrc2 += src_stride2;
	}

	res[0] = buffer0;
	res[1] = buffer1;
	res[2] = buffer2;
}

/* *********************************************************** */
