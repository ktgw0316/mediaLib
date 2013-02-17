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

#pragma ident	"@(#)mlib_ImageCrossCorrel_Fp.c	9.3	07/10/09 SMI"

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
#include <mlib_ImageCorrel.h>

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

		if (mlib_ImageGetChannels(img1) == 3)
			mlib_ImageCrossCorrel_F32_3(img1, img2, rez);
		else
			mlib_ImageCrossCorrel_F32_124(img1, img2, rez);
		break;

	case MLIB_DOUBLE:

		if (mlib_ImageGetChannels(img1) == 3)
			mlib_ImageCrossCorrel_D64_3(img1, img2, rez);
		else
			mlib_ImageCrossCorrel_D64_124(img1, img2, rez);
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

#define	MUL_D64(x, y)	((x) * (y))

/* *********************************************************** */

void
mlib_ImageCrossCorrel_D64_124(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez)
{
	mlib_d64 *psrc1 = (mlib_d64 *)mlib_ImageGetData(img1);
	mlib_d64 *psrc2 = (mlib_d64 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 c1, c2, c3, c4;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1) >> 3;

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2) >> 3;
	mlib_s32 nchan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << (nchan >> 1);

/* indices */
	mlib_s32 i, j, k;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	c1 = c2 = c3 = c4 = 0.0;
	rez[0] = rez[1] = rez[2] = 0.0;
	for (i = 0; i < height; i++) {
		for (j = 0; j <= (size_row - 4); j += 4) {
			c1 += MUL_D64(psrc1[j], psrc2[j]);
			c2 += MUL_D64(psrc1[j + 1], psrc2[j + 1]);
			c3 += MUL_D64(psrc1[j + 2], psrc2[j + 2]);
			c4 += MUL_D64(psrc1[j + 3], psrc2[j + 3]);
		}

		for (k = 0; j < size_row; j++, k++)
			rez[k] += MUL_D64(psrc1[j], psrc2[j]);
		psrc1 += src1_stride;
		psrc2 += src2_stride;
	}

	rez[0] += c1;
	rez[1] += c2;
	rez[2] += c3;
	rez[3] = c4;
}

/* *********************************************************** */

void
mlib_ImageCrossCorrel_D64_3(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez)
{
	mlib_d64 *psrc1 = (mlib_d64 *)mlib_ImageGetData(img1);
	mlib_d64 *psrc2 = (mlib_d64 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 c1, c2, c3;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1) >> 3;

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2) >> 3;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) * 3;

/* indices */
	mlib_s32 i, j;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	c1 = c2 = c3 = 0.0;
	for (i = 0; i < height; i++) {
		for (j = 0; j < size_row; j += 3) {
			c1 += MUL_D64(psrc1[j], psrc2[j]);
			c2 += MUL_D64(psrc1[j + 1], psrc2[j + 1]);
			c3 += MUL_D64(psrc1[j + 2], psrc2[j + 2]);
		}

		psrc1 += src1_stride;
		psrc2 += src2_stride;
	}

	rez[0] = c1;
	rez[1] = c2;
	rez[2] = c3;
}

/* *********************************************************** */

#define	MUL_F32(x, y)	((x) * (mlib_d64) (y))

/* *********************************************************** */

void
mlib_ImageCrossCorrel_F32_124(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez)
{
	mlib_f32 *psrc1 = (mlib_f32 *)mlib_ImageGetData(img1);
	mlib_f32 *psrc2 = (mlib_f32 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 c1, c2, c3, c4;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1) >> 2;

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2) >> 2;
	mlib_s32 nchan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << (nchan >> 1);

/* indices */
	mlib_s32 i, j, k;
	mlib_s32 off;
	union
	{
		mlib_d64 d;
		mlib_f32 f[2];
	} dc1,
	    dc2,
	    dc3,
	    dc4;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	rez[0] = rez[1] = rez[2] = rez[3] = 0.0;
	for (i = 0; i < height; i++) {
		c1 = c2 = c3 = c4 = 0.0;
		off = (mlib_addr)psrc1 & 4;
		j = 0;

		if (off) {
			c4 += MUL_F32(psrc1[0], psrc2[0]);
			j = 1;
		}

		if ((((mlib_addr)psrc1 ^ (mlib_addr)psrc2) & 7) == 0) {
			for (; j <= (size_row - 4); j += 4) {
				dc1.d = *(mlib_d64 *)&psrc1[j];
				dc2.d = *(mlib_d64 *)&psrc2[j];
				dc3.d = *(mlib_d64 *)&psrc1[j + 2];
				dc4.d = *(mlib_d64 *)&psrc2[j + 2];
				c1 += MUL_F32(dc1.f[0], dc2.f[0]);
				c2 += MUL_F32(dc1.f[1], dc2.f[1]);
				c3 += MUL_F32(dc3.f[0], dc4.f[0]);
				c4 += MUL_F32(dc3.f[1], dc4.f[1]);
			}
		} else {
			for (; j <= (size_row - 4); j += 4) {
				dc1.d = *(mlib_d64 *)&psrc1[j];
				dc2.d = *(mlib_d64 *)&psrc2[j + 1];
				dc3.d = *(mlib_d64 *)&psrc1[j + 2];
				c1 += MUL_F32(dc1.f[0], psrc2[j]);
				c2 += MUL_F32(dc1.f[1], dc2.f[0]);
				c3 += MUL_F32(dc3.f[0], dc2.f[1]);
				c4 += MUL_F32(dc3.f[1], psrc2[j + 3]);
			}
		}

		off >>= 2;
		for (k = off; j < size_row; j++, k++)
			rez[k & 3] += MUL_F32(psrc1[j], psrc2[j]);
		rez[0 + off] += c1;
		rez[1 + off] += c2;
		rez[2 + off] += c3;
		rez[3 & (3 + off)] += c4;
		psrc1 += src1_stride;
		psrc2 += src2_stride;
	}
}

/* *********************************************************** */

void
mlib_ImageCrossCorrel_F32_3(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez)
{
	mlib_f32 *psrc1 = (mlib_f32 *)mlib_ImageGetData(img1);
	mlib_f32 *psrc2 = (mlib_f32 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 c1, c2, c3;

/* accumulators by channels */
	mlib_d64 c4, c5, c6;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1) >> 2;

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2) >> 2;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) * 3;

/* indices */
	mlib_s32 i, j;
	union
	{
		mlib_d64 d;
		mlib_f32 f[2];
	} dc1,
	    dc2,
	    dc3,
	    dc4,
	    dc5,
	    dc6;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	c1 = c2 = c3 = c4 = c5 = c6 = 0.0;
	for (i = 0; i < height; i++) {
		j = 0;

		if ((mlib_addr)psrc1 & 4) {
			c1 += MUL_F32(psrc1[j], psrc2[j]);
			c2 += MUL_F32(psrc1[j + 1], psrc2[j + 1]);
			c3 += MUL_F32(psrc1[j + 2], psrc2[j + 2]);
			j = 3;
		}

		if ((((mlib_addr)psrc1 ^ (mlib_addr)psrc2) & 7) == 0) {
			for (; j <= (size_row - 6); j += 6) {
				dc1.d = *(mlib_d64 *)&psrc1[j];
				dc2.d = *(mlib_d64 *)&psrc2[j];
				dc3.d = *(mlib_d64 *)&psrc1[j + 2];
				dc4.d = *(mlib_d64 *)&psrc2[j + 2];
				dc5.d = *(mlib_d64 *)&psrc1[j + 4];
				dc6.d = *(mlib_d64 *)&psrc2[j + 4];
				c1 += MUL_F32(dc1.f[0], dc2.f[0]);
				c2 += MUL_F32(dc1.f[1], dc2.f[1]);
				c3 += MUL_F32(dc3.f[0], dc4.f[0]);
				c4 += MUL_F32(dc3.f[1], dc4.f[1]);
				c5 += MUL_F32(dc5.f[0], dc6.f[0]);
				c6 += MUL_F32(dc5.f[1], dc6.f[1]);
			}
		} else {
			for (; j <= (size_row - 6); j += 6) {
				dc1.d = *(mlib_d64 *)&psrc1[j];
				dc2.d = *(mlib_d64 *)&psrc2[j + 1];
				dc3.d = *(mlib_d64 *)&psrc1[j + 2];
				dc4.d = *(mlib_d64 *)&psrc2[j + 3];
				dc5.d = *(mlib_d64 *)&psrc1[j + 4];
				c1 += MUL_F32(dc1.f[0], psrc2[j]);
				c2 += MUL_F32(dc1.f[1], dc2.f[0]);
				c3 += MUL_F32(dc3.f[0], dc2.f[1]);
				c1 += MUL_F32(dc3.f[1], dc4.f[0]);
				c2 += MUL_F32(dc5.f[0], dc4.f[1]);
				c3 += MUL_F32(dc5.f[1], psrc2[j + 5]);
			}
		}

		if (j < size_row) {
			c1 += MUL_F32(psrc1[j], psrc2[j]);
			c2 += MUL_F32(psrc1[j + 1], psrc2[j + 1]);
			c3 += MUL_F32(psrc1[j + 2], psrc2[j + 2]);
		}

		psrc1 += src1_stride;
		psrc2 += src2_stride;
	}

	rez[0] = c1 + c4;
	rez[1] = c2 + c5;
	rez[2] = c3 + c6;
}

/* *********************************************************** */
