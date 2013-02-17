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

#pragma ident	"@(#)mlib_s_ImageCrossCorrel_Fp.c	9.6	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageCrossCorrel_Fp - calculates image cross correlation
 *                                 for the input images
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageCrossCorrel_Fp(mlib_d64         *correl,
 *                                           const mlib_image *img1,
 *                                           const mlib_image *img2);
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
 *      They can have 1, 2, 3 or 4 channels. They can be of MLIB_FLOAT or
 *      MLIB_DOUBLE data type.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_s_ImageCorrel.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageCrossCorrel_Fp = __mlib_ImageCrossCorrel_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageCrossCorrel_Fp) mlib_ImageCrossCorrel_Fp
    __attribute__((weak, alias("__mlib_ImageCrossCorrel_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageCrossCorrel_Fp(
    mlib_d64 *correl,
    const mlib_image *img1,
    const mlib_image *img2)
{
	mlib_s32 num_pixel;
	mlib_d64 rez[4];
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
	case MLIB_FLOAT:

		switch (mlib_ImageGetChannels(img1)) {
		case 1:
			mlib_ImageCrossCorrel_F32_1(img1, img2, rez);
			break;
		case 2:
			mlib_ImageCrossCorrel_F32_2(img1, img2, rez);
			break;
		case 3:
			mlib_ImageCrossCorrel_F32_3(img1, img2, rez);
			break;
		case 4:
			mlib_ImageCrossCorrel_F32_4(img1, img2, rez);
			break;
		default:
			return (MLIB_FAILURE);
		}
		break;

	case MLIB_DOUBLE:

		switch (mlib_ImageGetChannels(img1)) {
		case 1:
			mlib_ImageCrossCorrel_D64_1(img1, img2, rez);
			break;
		case 2:
			mlib_ImageCrossCorrel_D64_2(img1, img2, rez);
			break;
		case 3:
			mlib_ImageCrossCorrel_D64_3(img1, img2, rez);
			break;
		case 4:
			mlib_ImageCrossCorrel_D64_4(img1, img2, rez);
			break;
		default:
			return (MLIB_FAILURE);
		}
		break;

	default:
		return (MLIB_FAILURE);
	}

	switch (mlib_ImageGetChannels(img1)) {
	case 1:
		correl[0] = (rez[0] + rez[1] + rez[2] + rez[3]) * divider;
		break;

	case 2:
		correl[0] = (rez[0] + rez[2]) * divider;
		correl[1] = (rez[1] + rez[3]) * divider;
		break;

	case 4:
		correl[3] = rez[3] * divider;
	case 3:
		correl[0] = rez[0] * divider;
		correl[1] = rez[1] * divider;
		correl[2] = rez[2] * divider;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_ImageCrossCorrel_D64_1(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_d64 *psrc1 = (mlib_d64 *)mlib_ImageGetData(img1);
	mlib_d64 *psrc2 = (mlib_d64 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src_stride1 = mlib_ImageGetStride(img1) >> 3;
	mlib_s32 src_stride2 = mlib_ImageGetStride(img2) >> 3;

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
	mlib_d64 *sl1, *sl2;

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 aligntwo =
		    ((mlib_addr)psrc1 & 15) -
		    ((mlib_addr)psrc2 & 15);

		for (j = 0; (j < size_row) &&
		    ((mlib_addr)(psrc1 + j) & 15); j++) {
			buffer0 += psrc1[j] * psrc2[j];
		}

		db_num = ((size_row - j) >> 2);
		sl1 = psrc1 + j;
		sl2 = psrc2 + j;
		__m128d load1, load2, fs1, fsum1, fs2, fsum2;
		__m128d load3, load4;
		fsum1 = _mm_setzero_pd();
		fsum2 = _mm_setzero_pd();
		j += (db_num << 2);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			load1 = _mm_load_pd(sl1);
			sl1 += 2;
			load3 = _mm_load_pd(sl1);
			sl1 += 2;
			if (0 == aligntwo) {
				load2 = _mm_load_pd(sl2);
				sl2 += 2;
				load4 = _mm_load_pd(sl2);
				sl2 += 2;
			} else {
				load2 = _mm_loadu_pd(sl2);
				sl2 += 2;
				load4 = _mm_loadu_pd(sl2);
				sl2 += 2;
			}

			fs1 = _mm_mul_pd(load1, load2);
			fs2 = _mm_mul_pd(load3, load4);
			fsum1 = _mm_add_pd(fsum1, fs1);
			fsum2 = _mm_add_pd(fsum2, fs2);
		}

		fsum1 = _mm_add_pd(fsum1, fsum2);
		_mm_storeu_pd(sumbuff, fsum1);

		for (; j < size_row; j++) {
			buffer0 += psrc1[j] * psrc2[j];
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
mlib_ImageCrossCorrel_D64_2(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_d64 *psrc1 = (mlib_d64 *)mlib_ImageGetData(img1);
	mlib_d64 *psrc2 = (mlib_d64 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src_stride1 = mlib_ImageGetStride(img1) >> 3;
	mlib_s32 src_stride2 = mlib_ImageGetStride(img2) >> 3;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << 1;

/* indices */
	mlib_s32 i, j, k;

	if (src_stride1 == size_row && src_stride2 == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	__m128d fsum1, fsum2;
	fsum1 = _mm_setzero_pd();
	fsum2 = _mm_setzero_pd();

	res[0] = res[1] = 0.0;

	for (i = 0; i < height; i++) {

		if ((((mlib_addr)psrc1 & 15) == 0) &&
		    (((mlib_addr)psrc2 & 15) == 0)) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= (size_row - 4); j += 4) {
				__m128d load1, load2, load3, load4, fs1, fs2;
				load1 = _mm_load_pd(psrc1 + j);
				load2 = _mm_load_pd(psrc2 + j);
				load3 = _mm_load_pd(psrc1 + j + 2);
				load4 = _mm_load_pd(psrc2 + j + 2);

				fs1 = _mm_mul_pd(load1, load2);
				fs2 = _mm_mul_pd(load3, load4);
				fsum1 = _mm_add_pd(fsum1, fs1);
				fsum2 = _mm_add_pd(fsum2, fs2);
			}

		} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= (size_row - 4); j += 4) {
				__m128d load1, load2, load3, load4, fs1, fs2;
				load1 = _mm_loadu_pd(psrc1 + j);
				load3 = _mm_loadu_pd(psrc1+ j + 2);
				load2 = _mm_loadu_pd(psrc2 + j);
				load4 = _mm_loadu_pd(psrc2 + j + 2);

				fs1 = _mm_mul_pd(load1, load2);
				fs2 = _mm_mul_pd(load3, load4);
				fsum1 = _mm_add_pd(fsum1, fs1);
				fsum2 = _mm_add_pd(fsum2, fs2);
			}
		}

		if (j < size_row) {
			res[0] += psrc1[j] * psrc2[j];
			res[1] += psrc1[j + 1] * psrc2[j + 1];
		}

		psrc1 += src_stride1;
		psrc2 += src_stride2;
	}

	fsum1 = _mm_add_pd(fsum1, fsum2);
	mlib_d64 sumbuff[2];
	_mm_storeu_pd((void *)sumbuff, fsum1);

	res[0] += sumbuff[0];
	res[1] += sumbuff[1];
	res[2] = 0.0;
	res[3] = 0.0;
}

/* *********************************************************** */

void
mlib_ImageCrossCorrel_D64_4(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_d64 *psrc1 = (mlib_d64 *)mlib_ImageGetData(img1);
	mlib_d64 *psrc2 = (mlib_d64 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src_stride1 = mlib_ImageGetStride(img1) >> 3;
	mlib_s32 src_stride2 = mlib_ImageGetStride(img2) >> 3;
	mlib_f32 src_chan = mlib_ImageGetChannels(img1);

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
	mlib_d64 *sl1, *sl2;

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 sign = 0;
		mlib_s32 alignment =
		    ((16 - ((mlib_addr)psrc1 & 15)) & 15) >> 3;
		mlib_s32 aligntwo =
		    ((mlib_addr)psrc1 & 15) -
		    ((mlib_addr)psrc2 & 15);

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

		db_num = ((size_row - j) >> 2);
		sl1 = psrc1 + j;
		sl2 = psrc2 + j;
		__m128d load1, load2, load3, load4;
		__m128d fs1, fs2, fsum1, fsum2;
		fsum1 = _mm_setzero_pd();
		fsum2 = _mm_setzero_pd();
		j += (db_num << 2);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			load1 = _mm_load_pd(sl1);
			sl1 += 2;
			load2 = _mm_load_pd(sl1);
			sl1 += 2;
			if (0 == aligntwo) {
				load3 = _mm_load_pd(sl2);
				sl2 += 2;
				load4 = _mm_load_pd(sl2);
				sl2 += 2;
			} else {
				load3 = _mm_loadu_pd(sl2);
				sl2 += 2;
				load4 = _mm_loadu_pd(sl2);
				sl2 += 2;
			}

			fs1 = _mm_mul_pd(load1, load3);
			fs2 = _mm_mul_pd(load2, load4);
			fsum1 = _mm_add_pd(fsum1, fs1);
			fsum2 = _mm_add_pd(fsum2, fs2);
		}

		_mm_storeu_pd((void *)sumbuff, fsum1);
		_mm_storeu_pd((void *)(sumbuff + 2), fsum2);

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
mlib_ImageCrossCorrel_D64_3(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_d64 *psrc1 = (mlib_d64 *)mlib_ImageGetData(img1);
	mlib_d64 *psrc2 = (mlib_d64 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src_stride1 = mlib_ImageGetStride(img1) >> 3;
	mlib_s32 src_stride2 = mlib_ImageGetStride(img2) >> 3;

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
	mlib_d64 *sl1, *sl2;

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 sign = 0;
		mlib_s32 alignment =
			((16 - ((mlib_addr)psrc1 & 15)) & 15) >> 3;
		mlib_s32 aligntwo =
		    ((mlib_addr)psrc1 & 15) -
		    ((mlib_addr)psrc2 & 15);

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

		db_num = (size_row - j) / 6;
		sl1 = psrc1 + j;
		sl2 = psrc2 + j;
		__m128d load10, load11, load12;
		__m128d load20, load21, load22;
		__m128d fs0, fs1, fs2, fsum0, fsum1, fsum2;
		mlib_d64 sumbuff0[2], sumbuff1[2], sumbuff2[2];
		fsum0 = _mm_setzero_pd();
		fsum1 = _mm_setzero_pd();
		fsum2 = _mm_setzero_pd();

		j += (db_num * 6);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			load10 = _mm_load_pd(sl1);
			sl1 += 2;
			load11 = _mm_load_pd(sl1);
			sl1 += 2;
			load12 = _mm_load_pd(sl1);
			sl1 += 2;

			if (0 == aligntwo) {
				load20 = _mm_load_pd(sl2);
				sl2 += 2;
				load21 = _mm_load_pd(sl2);
				sl2 += 2;
				load22 = _mm_load_pd(sl2);
				sl2 += 2;
			} else {
				load20 = _mm_loadu_pd(sl2);
				sl2 += 2;
				load21 = _mm_loadu_pd(sl2);
				sl2 += 2;
				load22 = _mm_loadu_pd(sl2);
				sl2 += 2;
			}

			fs0 = _mm_mul_pd(load10, load20);
			fs1 = _mm_mul_pd(load11, load21);
			fs2 = _mm_mul_pd(load12, load22);

			fsum0 = _mm_add_pd(fsum0, fs0);
			fsum1 = _mm_add_pd(fsum1, fs1);
			fsum2 = _mm_add_pd(fsum2, fs2);
		}

		_mm_storeu_pd(sumbuff0, fsum0);
		_mm_storeu_pd(sumbuff1, fsum1);
		_mm_storeu_pd(sumbuff2, fsum2);

		buffer3 = sumbuff0[0] + sumbuff1[1];
		buffer4 = sumbuff0[1] + sumbuff2[0];
		buffer5 = sumbuff1[0] + sumbuff2[1];

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
mlib_ImageCrossCorrel_F32_1(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_f32 *psrc1 = (mlib_f32 *)mlib_ImageGetData(img1);
	mlib_f32 *psrc2 = (mlib_f32 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src_stride1 = mlib_ImageGetStride(img1) >> 2;
	mlib_s32 src_stride2 = mlib_ImageGetStride(img2) >> 2;

	mlib_s32 src_chan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1);

/* indices */
	mlib_s32 i, j;

	if (src_stride1 == size_row && src_stride2 == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	__m128d fsum1, fsum2;
	fsum1 = fsum2 = _mm_setzero_pd();

	for (i = 0; i < height; i++) {
		mlib_f32 *sl1 = psrc1;
		mlib_f32 *sl2 = psrc2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; (j <= size_row - 4); j += 4) {
			__m128d fd1, fd2, fd3, fd4, fs1, fs2;
			fd1 = _mm_cvtps_pd(*((__m128 *)sl1));
			fd2 = _mm_cvtps_pd(*((__m128 *)(sl1 + 2)));
			fd3 = _mm_cvtps_pd(*((__m128 *)sl2));
			fd4 = _mm_cvtps_pd(*((__m128 *)(sl2 + 2)));

			fs1 = _mm_mul_pd(fd1, fd3);
			fs2 = _mm_mul_pd(fd2, fd4);
			fsum1 = _mm_add_pd(fsum1, fs1);
			fsum2 = _mm_add_pd(fsum2, fs2);
			sl1 += 4;
			sl2 += 4;
		}

		for (; j < size_row; j++) {
			buffer0 += (mlib_d64)psrc1[j] * psrc2[j];
		}

		psrc1 += src_stride1;
		psrc2 += src_stride2;
	}

	fsum1 = _mm_add_pd(fsum1, fsum2);
	_mm_storel_pd((res + 1), fsum1);
	_mm_storeh_pd((res + 2), fsum1);

	res[0] = buffer0;
	res[3] = 0.0;
}

/* *********************************************************** */

void
mlib_ImageCrossCorrel_F32_2(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_f32 *psrc1 = (mlib_f32 *)mlib_ImageGetData(img1);
	mlib_f32 *psrc2 = (mlib_f32 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src_stride1 = mlib_ImageGetStride(img1) >> 2;
	mlib_s32 src_stride2 = mlib_ImageGetStride(img2) >> 2;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << 1;

/* indices */
	mlib_s32 i, j;

	if (src_stride1 == size_row && src_stride2 == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;

	__m128d fsum1, fsum2;
	fsum1 = fsum2 = _mm_setzero_pd();

	for (i = 0; i < height; i++) {
		mlib_f32 *sl1, *sl2;
		sl1 = psrc1;
		sl2 = psrc2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; (j <= size_row - 4); j += 4, sl1 += 4, sl2 += 4) {
			__m128d fd1, fd2, fd3, fd4, fs1, fs2;
			fd1 = _mm_cvtps_pd(*((__m128 *)sl1));
			fd2 = _mm_cvtps_pd(*((__m128 *)(sl1 + 2)));
			fd3 = _mm_cvtps_pd(*((__m128 *)sl2));
			fd4 = _mm_cvtps_pd(*((__m128 *)(sl2 + 2)));

			fs1 = _mm_mul_pd(fd1, fd3);
			fs2 = _mm_mul_pd(fd2, fd4);
			fsum1 = _mm_add_pd(fsum1, fs1);
			fsum2 = _mm_add_pd(fsum2, fs2);
		}

		if (j < size_row) {
			buffer0 +=
			    (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
			buffer1 +=
			    (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
		}

		psrc1 += src_stride1;
		psrc2 += src_stride2;
	}

	fsum1 = _mm_add_pd(fsum1, fsum2);
	__m128d xsum;
	mlib_d64 *psum = (mlib_d64 *)&xsum;
	_mm_store_pd((void *)psum, fsum1);

	res[0] = buffer0 + psum[0];
	res[1] = buffer1 + psum[1];
	res[2] = 0.0;
	res[3] = 0.0;
}

/* *********************************************************** */

void
mlib_ImageCrossCorrel_F32_4(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_f32 *psrc1 = (mlib_f32 *)mlib_ImageGetData(img1);
	mlib_f32 *psrc2 = (mlib_f32 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src_stride1 = mlib_ImageGetStride(img1) >> 2;
	mlib_s32 src_stride2 = mlib_ImageGetStride(img2) >> 2;
	mlib_f32 src_chan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << 2;

/* indices */
	mlib_s32 i, l;

	if (src_stride1 == size_row && src_stride2 == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	__m128d fsum1, fsum2;
	fsum1 = fsum2 = _mm_setzero_pd();

	for (i = 0; i < height; i++) {
		mlib_f32 *sl1, *sl2;
		sl1 = psrc1;
		sl2 = psrc2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l <= size_row - 4); l += 4, sl1 += 4, sl2 += 4) {
			__m128d fd1, fd2, fd3, fd4, fs1, fs2;
			fd1 = _mm_cvtps_pd(*((__m128 *)sl1));
			fd2 = _mm_cvtps_pd(*((__m128 *)(sl1 + 2)));
			fd3 = _mm_cvtps_pd(*((__m128 *)sl2));
			fd4 = _mm_cvtps_pd(*((__m128 *)(sl2 + 2)));

			fs1 = _mm_mul_pd(fd1, fd3);
			fs2 = _mm_mul_pd(fd2, fd4);
			fsum1 = _mm_add_pd(fsum1, fs1);
			fsum2 = _mm_add_pd(fsum2, fs2);
		}

		psrc1 += src_stride1;
		psrc2 += src_stride2;
	}

	__m128d xsum0, xsum1;
	mlib_d64 *psum0, *psum1;
	psum0 = (mlib_d64 *)&xsum0;
	psum1 = (mlib_d64 *)&xsum1;

	_mm_store_pd(psum0, fsum1);
	_mm_store_pd(psum1, fsum2);

	res[0] = psum0[0];
	res[1] = psum0[1];
	res[2] = psum1[0];
	res[3] = psum1[1];
}

/* *********************************************************** */

void
mlib_ImageCrossCorrel_F32_3(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_f32 *psrc1 = (mlib_f32 *)mlib_ImageGetData(img1);
	mlib_f32 *psrc2 = (mlib_f32 *)mlib_ImageGetData(img2);

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

	mlib_s32 db_num;
	mlib_f32 *sl1, *sl2;
	__m128d fsum0, fsum1, fsum2, fsum3, fsum4, fsum5;
	fsum0 = fsum1 = fsum2 = fsum3 = fsum4 = fsum5 = _mm_setzero_pd();

	for (i = 0; i < height; i++) {
		j = 0;
		db_num = (size_row - j) / 12;
		sl1 = psrc1 + j;
		sl2 = psrc2 + j;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			__m128d fd10, fd11, fd12, fd13, fd14, fd15;
			__m128d fd20, fd21, fd22, fd23, fd24, fd25;
			__m128d fs0, fs1, fs2, fs3, fs4, fs5;

			fd10 = _mm_cvtps_pd(*((__m128 *)(sl1 + 0)));
			fd11 = _mm_cvtps_pd(*((__m128 *)(sl1 + 2)));
			fd12 = _mm_cvtps_pd(*((__m128 *)(sl1 + 4)));
			fd13 = _mm_cvtps_pd(*((__m128 *)(sl1 + 6)));
			fd14 = _mm_cvtps_pd(*((__m128 *)(sl1 + 8)));
			fd15 = _mm_cvtps_pd(*((__m128 *)(sl1 + 10)));

			fd20 = _mm_cvtps_pd(*((__m128 *)(sl2 + 0)));
			fd21 = _mm_cvtps_pd(*((__m128 *)(sl2 + 2)));
			fd22 = _mm_cvtps_pd(*((__m128 *)(sl2 + 4)));
			fd23 = _mm_cvtps_pd(*((__m128 *)(sl2 + 6)));
			fd24 = _mm_cvtps_pd(*((__m128 *)(sl2 + 8)));
			fd25 = _mm_cvtps_pd(*((__m128 *)(sl2 + 10)));

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
			sl1 += 12;
			sl2 += 12;
		}

		j += (db_num * 12);
		for (; j < (size_row - 2); j += 3) {
			buffer0 += (mlib_d64)psrc1[j + 0] * psrc2[j + 0];
			buffer1 += (mlib_d64)psrc1[j + 1] * psrc2[j + 1];
			buffer2 += (mlib_d64)psrc1[j + 2] * psrc2[j + 2];
		}

		psrc1 += src_stride1;
		psrc2 += src_stride2;
	}

	__m128d xsum0, xsum1, xsum2, xsum3, xsum4, xsum5;
	mlib_d64 *psum0, *psum1, *psum2;
	mlib_d64 *psum3, *psum4, *psum5;
	psum0 = (mlib_d64 *)&xsum0;
	psum1 = (mlib_d64 *)&xsum1;
	psum2 = (mlib_d64 *)&xsum2;
	psum3 = (mlib_d64 *)&xsum3;
	psum4 = (mlib_d64 *)&xsum4;
	psum5 = (mlib_d64 *)&xsum5;

	_mm_store_pd(psum0, fsum0);
	_mm_store_pd(psum1, fsum1);
	_mm_store_pd(psum2, fsum2);
	_mm_store_pd(psum3, fsum3);
	_mm_store_pd(psum4, fsum4);
	_mm_store_pd(psum5, fsum5);

	buffer0 += psum0[0] + psum1[1] + psum3[0] + psum4[1];
	buffer1 += psum0[1] + psum2[0] + psum3[1] + psum5[0];
	buffer2 += psum1[0] + psum2[1] + psum4[0] + psum5[1];

	res[0] = buffer0;
	res[1] = buffer1;
	res[2] = buffer2;
}

/* *********************************************************** */
