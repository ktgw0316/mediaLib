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

#pragma ident	"@(#)mlib_s_ImageMoment2_Fp.c	9.5	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageMoment2_Fp - calculates moment of power two
 *                             for the input image.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMoment2_Fp(mlib_d64         *moment,
 *                                       const mlib_image *img)
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
 *      img can have 1, 2, 3 or 4 channels. It can be of MLIB_FLOAT or
 *      MLIB_DOUBLE data type.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_s_ImageMoment2.h>
#include <mlib_sse_utils.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMoment2_Fp = __mlib_ImageMoment2_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMoment2_Fp) mlib_ImageMoment2_Fp
    __attribute__((weak, alias("__mlib_ImageMoment2_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageMoment2_Fp(
    mlib_d64 *moment,
    const mlib_image *img)
{
	mlib_d64 res[4];
	mlib_d64 divider;

/* check for obvious errors */
	MLIB_IMAGE_CHECK(img);

	if (moment == NULL)
		return (MLIB_NULLPOINTER);

	divider = 1.0 / (mlib_ImageGetWidth(img) * mlib_ImageGetHeight(img));

	switch (mlib_ImageGetType(img)) {
	case MLIB_FLOAT:
		switch (mlib_ImageGetChannels(img)) {
		case 1:
			mlib_s_ImageMoment2_F32_1(img, res);
			break;
		case 2:
			mlib_s_ImageMoment2_F32_2(img, res);
			break;
		case 3:
			mlib_s_ImageMoment2_F32_3(img, res);
			break;
		case 4:
			mlib_s_ImageMoment2_F32_4(img, res);
			break;
		default:
			return (MLIB_FAILURE);
		}
		break;

	case MLIB_DOUBLE:
		switch (mlib_ImageGetChannels(img)) {
		case 1:
			mlib_s_ImageMoment2_D64_1(img, res);
			break;
		case 2:
			mlib_s_ImageMoment2_D64_2(img, res);
			break;
		case 3:
			mlib_s_ImageMoment2_D64_3(img, res);
			break;
		case 4:
			mlib_s_ImageMoment2_D64_4(img, res);
			break;
		default:
			return (MLIB_FAILURE);
		}
		break;

	default:
		return (MLIB_FAILURE);
	}

	switch (mlib_ImageGetChannels(img)) {
	case 1:
		moment[0] = (res[0] + res[1] + res[2] + res[3]) * divider;
		break;

	case 2:
		moment[0] = (res[0] + res[2]) * divider;
		moment[1] = (res[1] + res[3]) * divider;
		break;

	case 4:
		moment[3] = res[3] * divider;
	case 3:
		moment[0] = res[0] * divider;
		moment[1] = res[1] * divider;
		moment[2] = res[2] * divider;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_s_ImageMoment2_F32_1(
    const mlib_image *img,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_f32 *psrc = (mlib_f32 *)mlib_ImageGetData(img);

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
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;

	__m128d fsum1, fsum2;
	fsum1 = fsum2 = _mm_setzero_pd();

	for (i = 0; i < height; i++) {
		mlib_f32 *sl = psrc;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(4)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (size_row - 4); j += 4, sl += 4) {
			__m128d fd1, fd2, fs1, fs2;
			fd1 = _mm_cvtps_pd(*((__m128 *)sl));
			fd2 = _mm_cvtps_pd(*((__m128 *)(sl + 2)));
			fs1 = _mm_mul_pd(fd1, fd1);
			fs2 = _mm_mul_pd(fd2, fd2);
			fsum1 = _mm_add_pd(fsum1, fs1);
			fsum2 = _mm_add_pd(fsum2, fs2);
		}

		for (; j < size_row; j++) {
			buffer0 += (mlib_d64)psrc[j] * psrc[j];
		}

		psrc += src_stride;
	}

	fsum1 = _mm_add_pd(fsum1, fsum2);
	_mm_storel_pd((res + 1), fsum1);
	_mm_storeh_pd((res + 2), fsum1);

	res[0] = buffer0;
	res[3] = 0.0;
}

/* *********************************************************** */

void
mlib_s_ImageMoment2_F32_2(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_f32 *psrc = (mlib_f32 *)mlib_ImageGetData(img);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 2;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) << 1;

/* indices */
	mlib_s32 i, j, l;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	__m128d fsum1, fsum2;
	fsum1 = fsum2 = _mm_setzero_pd();

	for (i = 0; i < height; i++) {
		mlib_f32 *sl = psrc;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(4)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (size_row - 4); j += 4, sl += 4) {
			__m128d fd1, fd2, fs1, fs2;
			fd1 = _mm_cvtps_pd(*((__m128 *)sl));
			fd2 = _mm_cvtps_pd(*((__m128 *)(sl + 2)));
			fs1 = _mm_mul_pd(fd1, fd1);
			fs2 = _mm_mul_pd(fd2, fd2);
			fsum1 = _mm_add_pd(fsum1, fs1);
			fsum2 = _mm_add_pd(fsum2, fs2);
		}

		if (j < size_row) {
			buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
			buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
		}

		psrc += src_stride;
	}

	fsum1 = _mm_add_pd(fsum1, fsum2);
	__m128d xsum;
	mlib_d64 *psum = (mlib_d64 *)&xsum;
	_mm_store_pd(psum, fsum1);

	rez[0] = buffer0 + psum[0];
	rez[1] = buffer1 + psum[1];
	rez[2] = 0.0;
	rez[3] = 0.0;
}

/* *********************************************************** */

void
mlib_s_ImageMoment2_F32_4(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_f32 *psrc = (mlib_f32 *)mlib_ImageGetData(img);

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
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	__m128d fsum1, fsum2;
	fsum1 = fsum2 = _mm_setzero_pd();

	for (i = 0; i < height; i++) {
		mlib_f32 *sl = psrc;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(4)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (size_row - 4); j += 4, sl += 4) {
			__m128d fd1, fd2, fs1, fs2;
			fd1 = _mm_cvtps_pd(*((__m128 *)sl));
			fd2 = _mm_cvtps_pd(*((__m128 *)(sl + 2)));
			fs1 = _mm_mul_pd(fd1, fd1);
			fs2 = _mm_mul_pd(fd2, fd2);
			fsum1 = _mm_add_pd(fsum1, fs1);
			fsum2 = _mm_add_pd(fsum2, fs2);
		}

		psrc += src_stride;
	}

	_mm_storel_pd((rez), fsum1);
	_mm_storeh_pd((rez + 1), fsum1);
	_mm_storel_pd((rez + 2), fsum2);
	_mm_storeh_pd((rez + 3), fsum2);
}

/* *********************************************************** */

void
mlib_s_ImageMoment2_F32_3(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_f32 *psrc = (mlib_f32 *)mlib_ImageGetData(img);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 2;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) * 3;

/* indices */
	mlib_s32 i, j, l;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_d64 buffer2 = 0.0;

	mlib_s32 db_num = (size_row / 12);

	__m128d fsum0, fsum1, fsum2, fsum3, fsum4, fsum5;
	fsum0 = fsum1 = fsum2 = fsum3 =	fsum4 = fsum5 = _mm_setzero_pd();

	for (i = 0; i < height; i++) {
		mlib_f32 *sl = psrc;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (l = 0; (l < db_num); l++) {
			__m128d fd0, fd1, fd2, fd3, fd4, fd5;
			__m128d fs0, fs1, fs2, fs3, fs4, fs5;
			fd0 = _mm_cvtps_pd(*((__m128 *)sl));
			fd1 = _mm_cvtps_pd(*((__m128 *)(sl + 2)));
			fd2 = _mm_cvtps_pd(*((__m128 *)(sl + 4)));
			fd3 = _mm_cvtps_pd(*((__m128 *)(sl + 6)));
			fd4 = _mm_cvtps_pd(*((__m128 *)(sl + 8)));
			fd5 = _mm_cvtps_pd(*((__m128 *)(sl + 10)));

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
			sl += 12;
		}

		j = (db_num * 12);
		for (; j < (size_row - 2); j += 3) {
			buffer0 += (mlib_d64)psrc[j + 0] * psrc[j + 0];
			buffer1 += (mlib_d64)psrc[j + 1] * psrc[j + 1];
			buffer2 += (mlib_d64)psrc[j + 2] * psrc[j + 2];
		}

		psrc += src_stride;
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

	rez[0] = buffer0 + psum0[0] + psum1[1] + psum3[0] + psum4[1];
	rez[1] = buffer1 + psum0[1] + psum2[0] + psum3[1] + psum5[0];
	rez[2] = buffer2 + psum1[0] + psum2[1] + psum4[0] + psum5[1];
}

/* *********************************************************** */

void
mlib_s_ImageMoment2_D64_1(
    const mlib_image *img,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_d64 *psrc = (mlib_d64 *)mlib_ImageGetData(img);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 3;
	mlib_s32 src_chan = mlib_ImageGetChannels(img);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img);

/* indices */
	mlib_s32 i, j, l;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	__m128d fsum0, fsum1;
	fsum0 = fsum1 = _mm_setzero_pd();

	for (i = 0; i < height; i++) {
		j = 0;

		if ((0 != ((mlib_addr)psrc & 15))) {
			buffer0 += (mlib_d64)psrc[0] * psrc[0];
			j++;
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(4)
#endif /* __SUNPRO_C */
		for (; j <= (size_row - 4); j += 4) {
			__m128d load0, load1, fs0, fs1;
			load0 = _mm_load_pd(psrc + j);
			load1 = _mm_load_pd(psrc + j + 2);
			fs0 = _mm_mul_pd(load0, load0);
			fs1 = _mm_mul_pd(load1, load1);
			fsum0 = _mm_add_pd(fsum0, fs0);
			fsum1 = _mm_add_pd(fsum1, fs1);
		}

		if (j <= size_row - 2) {
			__m128d load0, fs0;
			load0 = _mm_load_pd(psrc + j);
			fs0 = _mm_mul_pd(load0, load0);
			fsum0 = _mm_add_pd(fsum0, fs0);
			j += 2;
		}

		if (j < size_row) {
			buffer0 += psrc[j] * psrc[j];
		}

		psrc += src_stride;
	}

	fsum0 = _mm_add_pd(fsum0, fsum1);
	_mm_storel_pd((res), fsum0);
	_mm_storeh_pd((res + 1), fsum0);
	res[2] = buffer0;
	res[3] = 0.0;
}

/* *********************************************************** */

void
mlib_s_ImageMoment2_D64_2(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_d64 *psrc = (mlib_d64 *)mlib_ImageGetData(img);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 3;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) << 1;

/* indices */
	mlib_s32 i, j, l;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 alignment = 0;

		if ((0 != ((mlib_addr)psrc & 15))) {
			alignment = 1;
			buffer0 += (mlib_d64)psrc[0] * psrc[0];
			j = 1;
		}

		__m128d fsum0, fsum1;
		fsum0 = fsum1 = _mm_setzero_pd();

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(4)
#endif /* __SUNPRO_C */
		for (; j <= (size_row - 4); j += 4) {
			__m128d load0, load1, fs0, fs1;
			load0 = _mm_load_pd(psrc + j);
			load1 = _mm_load_pd(psrc + j + 2);
			fs0 = _mm_mul_pd(load0, load0);
			fs1 = _mm_mul_pd(load1, load1);
			fsum0 = _mm_add_pd(fsum0, fs0);
			fsum1 = _mm_add_pd(fsum1, fs1);
		}

		if (j <= size_row - 2) {
			__m128d load0, fs0;
			load0 = _mm_load_pd(psrc + j);
			fs0 = _mm_mul_pd(load0, load0);
			fsum0 = _mm_add_pd(fsum0, fs0);
			j += 2;
		}

		fsum0 = _mm_add_pd(fsum0, fsum1);
		mlib_d64 s0, s1;
		_mm_storel_pd(&s0, fsum0);
		_mm_storeh_pd(&s1, fsum0);

		if (alignment) {
			buffer1 += psrc[j] * psrc[j];
			buffer1 += s0;
			buffer0 += s1;
		} else {
			buffer0 += s0;
			buffer1 += s1;
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
mlib_s_ImageMoment2_D64_4(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_d64 *psrc = (mlib_d64 *)mlib_ImageGetData(img);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 3;
	mlib_s32 src_chan = mlib_ImageGetChannels(img);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) << 2;

/* indices */
	mlib_s32 i, j, l;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_d64 buffer2 = 0.0;
	mlib_d64 buffer3 = 0.0;

	for (i = 0; i < height; i++) {
		mlib_s32 alignment = 0;
		j = 0;

		if ((0 != ((mlib_addr)psrc & 15))) {
			j = alignment = 1;
			buffer0 += (mlib_d64)psrc[0] * psrc[0];
		}

		__m128d fsum1, fsum2;
		fsum1 = fsum2 = _mm_setzero_pd();

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(4)
#endif /* __SUNPRO_C */
		for (; j <= (size_row - 4); j += 4) {
			__m128d load0, load1, fs1, fs2;
			load0 = _mm_load_pd(psrc + j);
			load1 = _mm_load_pd(psrc + j + 2);
			fs1 = _mm_mul_pd(load0, load0);
			fs2 = _mm_mul_pd(load1, load1);
			fsum1 = _mm_add_pd(fsum1, fs1);
			fsum2 = _mm_add_pd(fsum2, fs2);
		}

		mlib_d64 s0, s1, s2, s3;
		_mm_storel_pd(&s0, fsum1);
		_mm_storeh_pd(&s1, fsum1);
		_mm_storel_pd(&s2, fsum2);
		_mm_storeh_pd(&s3, fsum2);

		if (alignment) {
			buffer1 += psrc[j] * psrc[j];
			buffer2 += psrc[j + 1] * psrc[j + 1];
			buffer3 += psrc[j + 2] * psrc[j + 2];

			buffer0 += s3;
			buffer1 += s0;
			buffer2 += s1;
			buffer3 += s2;
		} else {
			buffer0 += s0;
			buffer1 += s1;
			buffer2 += s2;
			buffer3 += s3;
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
mlib_s_ImageMoment2_D64_3(
    const mlib_image *img,
    mlib_d64 *rez)
{
/* pointer to the data of image */
	mlib_d64 *psrc = (mlib_d64 *)mlib_ImageGetData(img);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 3;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) * 3;

/* indices */
	mlib_s32 i, j, l;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	mlib_d64 buffer0 = 0.0;
	mlib_d64 buffer1 = 0.0;
	mlib_d64 buffer2 = 0.0;
	mlib_d64 buffer3, buffer4, buffer5;

	for (i = 0; i < height; i++) {
		j = 0;
		mlib_s32 alignment = 0;

		if ((0 != ((mlib_addr)psrc & 15))) {
			alignment = 1;
			buffer0 += (mlib_d64)psrc[0] * psrc[0];
			j = 1;
		}

		__m128d fsum0, fsum1, fsum2;
		fsum0 = fsum1 = fsum2 = _mm_setzero_pd();

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (size_row - 6); j += 6) {
			__m128d load0, load1, load2;
			__m128d fs0, fs1, fs2;
			load0 = _mm_load_pd(psrc + j);
			load1 = _mm_load_pd(psrc + j + 2);
			load2 = _mm_load_pd(psrc + j + 4);

			fs0 = _mm_mul_pd(load0, load0);
			fs1 = _mm_mul_pd(load1, load1);
			fs2 = _mm_mul_pd(load2, load2);

			fsum0 = _mm_add_pd(fsum0, fs0);
			fsum1 = _mm_add_pd(fsum1, fs1);
			fsum2 = _mm_add_pd(fsum2, fs2);
		}

		mlib_d64 s0, s1, s2, s3, s4, s5;
		_mm_storel_pd(&s0, fsum0);
		_mm_storeh_pd(&s1, fsum0);
		_mm_storel_pd(&s2, fsum1);
		_mm_storeh_pd(&s3, fsum1);
		_mm_storel_pd(&s4, fsum2);
		_mm_storeh_pd(&s5, fsum2);

		buffer3 = s0 + s3;
		buffer4 = s1 + s4;
		buffer5 = s2 + s5;

		if (j <= (size_row - 3)) {
			buffer3 += psrc[j + 0] * psrc[j + 0];
			buffer4 += psrc[j + 1] * psrc[j + 1];
			buffer5 += psrc[j + 2] * psrc[j + 2];
			j += 3;
		}

		if (alignment) {
			buffer1 += psrc[j] * psrc[j];
			buffer2 += psrc[j + 1] * psrc[j + 1];
			buffer0 += buffer5;
			buffer1 += buffer3;
			buffer2 += buffer4;
		} else {
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
