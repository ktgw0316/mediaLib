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

#pragma ident	"@(#)mlib_s_ImageYProj_Fp.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageYProj_Fp - calculates Y Projections for input image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageYProj_Fp(mlib_d64         *yproj,
 *                                     const mlib_image *src)
 *
 * ARGUMENTS
 *      yproj       Pointer to Y-projection vector
 *      src         Pointer to an image
 *
 * RESTRICTION
 *      The image must be a single channel image.
 *      It can be of MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *                 w-1
 *      yproj[j] = SUM (src[i][j]);
 *                 i = 0
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_sse_utils.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageYProj_Fp = __mlib_ImageYProj_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageYProj_Fp) mlib_ImageYProj_Fp
    __attribute__((weak, alias("__mlib_ImageYProj_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_ImageYProj_F32(
    const mlib_image *src,
    mlib_d64 *yproj);
static void mlib_ImageYProj_D64(
    const mlib_image *src,
    mlib_d64 *yproj);

/* *********************************************************** */

mlib_status
__mlib_ImageYProj_Fp(
    mlib_d64 *yproj,
    const mlib_image *src)
{
/* check for obvious errors */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_HAVE_CHAN(src, 1);

	if (yproj == NULL)
		return (MLIB_NULLPOINTER);

	switch (mlib_ImageGetType(src)) {
	case MLIB_FLOAT:
		mlib_ImageYProj_F32(src, yproj);
		break;

	case MLIB_DOUBLE:
		mlib_ImageYProj_D64(src, yproj);
		break;

	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_ImageYProj_F32(
    const mlib_image *src,
    mlib_d64 *yproj)
{

/* width of source image */
	mlib_s32 Width = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 Height = mlib_ImageGetHeight(src);

/* linestride = bytes to next row */
	mlib_s32 YStride = mlib_ImageGetStride(src);

/* pointers to the data of image */
	mlib_f32 *px = (mlib_f32 *)mlib_ImageGetData(src);
	mlib_f32 *spx;

/* number of pixel to process */
	mlib_s32 size = Width;
	mlib_s32 i, j;
	mlib_d64 accum;

	spx = px;

	for (j = 0; j < Height; j++) {
		accum = 0.0;
		__m128d x_sum0_d64, x_sum1_d64;
		x_sum0_d64 = x_sum1_d64 = _mm_setzero_pd();

		for (i = 0; i < (size - 8); i += 8) {
			__m128d x_s0_d64, x_s1_d64;
			__m128d x_s2_d64, x_s3_d64;

			x_s0_d64 = _mm_cvtps_pd(*((__m128 *)px));
			x_s1_d64 = _mm_cvtps_pd(*((__m128 *)(px + 2)));
			x_sum0_d64 = _mm_add_pd(x_sum0_d64, x_s0_d64);
			x_sum1_d64 = _mm_add_pd(x_sum1_d64, x_s1_d64);

			x_s2_d64 = _mm_cvtps_pd(*((__m128 *)(px + 4)));
			x_s3_d64 = _mm_cvtps_pd(*((__m128 *)(px + 6)));
			x_sum0_d64 = _mm_add_pd(x_sum0_d64, x_s2_d64);
			x_sum1_d64 = _mm_add_pd(x_sum1_d64, x_s3_d64);

			px += 8;
		}

		if (i < (size - 4)) {
			__m128d x_s0_d64, x_s1_d64;
			x_s0_d64 = _mm_cvtps_pd(*((__m128 *)px));
			x_s1_d64 = _mm_cvtps_pd(*((__m128 *)(px + 2)));
			x_sum0_d64 = _mm_add_pd(x_sum0_d64, x_s0_d64);
			x_sum1_d64 = _mm_add_pd(x_sum1_d64, x_s1_d64);
			i += 4;
			px += 4;
		}

		for (; i < size; i++, px++) {
			accum += (mlib_d64)(*px);
		}

		__m128d xsum0, xsum1;
		mlib_d64 *psum0, *psum1;
		psum0 = (mlib_d64 *)&xsum0;
		psum1 = (mlib_d64 *)&xsum1;
		_mm_store_pd(psum0, x_sum0_d64);
		_mm_store_pd(psum1, x_sum1_d64);
		accum +=
		    psum0[0] + psum0[1] + psum1[0] + psum1[1];

		yproj[j] = accum;

		px = spx = (mlib_f32 *)((mlib_u8 *)spx + YStride);
	}
}

/* *********************************************************** */

/* Pure C Implementation */

void
mlib_ImageYProj_D64(
    const mlib_image *src,
    mlib_d64 *yproj)
{

/* width of source image */
	int Width = mlib_ImageGetWidth(src);

/* height of source image */
	int Height = mlib_ImageGetHeight(src);

/* bytes to next row */
	int YStride = (mlib_ImageGetStride(src)) >> 3;

/* pointers to the data of image */
	mlib_d64 *LinePntr = (mlib_d64 *)mlib_ImageGetData(src), *PixelPntr;
	mlib_d64 Accum1;
	int i, j;

	for (j = 0; j < Height; j++) {
		Accum1 = 0;
		PixelPntr = LinePntr;

		for (i = 0; i < Width; i++)
			Accum1 += (*PixelPntr++);
		yproj[j] = Accum1;

		LinePntr += YStride;
	}
}

/* *********************************************************** */
