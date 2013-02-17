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
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident   "@(#)mlib_ImageRankFilter3x3_Fp.c	9.2    07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageRankFilter3x3_Fp   - performs rank filtering on an image.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageRankFilter3x3_Fp(mlib_image       *dst,
 *                                             const mlib_image *src,
 *                                             mlib_s32         rank)
 *
 * ARGUMENT
 *      dst     pointer to structure of destination image,
 *      src     pointer to structure of source image,
 *      rank    the rank of the output pixel. rank value must be from 0 to 8.
 *              The pixel with minimum value is designated rank 0 and
 *              the pixel with maximum value is designated rank 8.
 *
 * DESCRIPTION
 *      X  - x(i, j), (i = 0 ... w - 1), (j = 0 ... h - 1)
 *      Z  - z(i, j), (i = 1 ... w - 2), (j = 1 ... h - 2)
 *      z(i, j) = rank value in a sorted array of 9 source image pixels -
 *                { x(p, q), (i - 1 <= p <= i + 1), (j - 1 <= q <= j + 1) }.
 *      so if rank equals 0 - min value, rank equals 8 - max value.
 *      if rank equals 4 - median value.
 *      The input and output images must be single channel images and have
 *      the same width, height and image type.
 *      They can be in MLIB_FLOAT, MLIB_DOUBLE type.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageRankFilter.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageRankFilter3x3_Fp = __mlib_ImageRankFilter3x3_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageRankFilter3x3_Fp) mlib_ImageRankFilter3x3_Fp
    __attribute__((weak, alias("__mlib_ImageRankFilter3x3_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static const mlib_RankFilterRow mlib_rank_row_funcs_Fp[] = {
	mlib_rank_row_3x3_r1_f32,
	mlib_rank_row_3x3_r2_f32,
	mlib_rank_row_3x3_r3_f32,
	mlib_rank_row_3x3_r4_f32,
	mlib_rank_row_3x3_r5_f32,
	mlib_rank_row_3x3_r6_f32,
	mlib_rank_row_3x3_r7_f32,
	mlib_rank_row_3x3_r1_d64,
	mlib_rank_row_3x3_r2_d64,
	mlib_rank_row_3x3_r3_d64,
	mlib_rank_row_3x3_r4_d64,
	mlib_rank_row_3x3_r5_d64,
	mlib_rank_row_3x3_r6_d64,
	mlib_rank_row_3x3_r7_d64
};

/* *********************************************************** */

mlib_status
__mlib_ImageRankFilter3x3_Fp(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 rank)
{
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 type;
	mlib_s32 stride_s, stride_d;
	mlib_u8 *psrc;
	mlib_u8 *pdst;
	mlib_RankFilterRow rowFunc;
	mlib_s32 rankIndex;
	mlib_s32 i;

/* checking images */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_FULL_EQUAL(dst, src);
	MLIB_IMAGE_HAVE_CHAN(dst, 1);

	type = mlib_ImageGetType(dst);
	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	stride_s = mlib_ImageGetStride(src);
	stride_d = mlib_ImageGetStride(dst);
	psrc = mlib_ImageGetData(src);
	pdst = mlib_ImageGetData(dst);

	if (type != MLIB_FLOAT && type != MLIB_DOUBLE)
		return (MLIB_FAILURE);

	if (rank < 0 || rank > 8)
		return (MLIB_OUTOFRANGE);

	if (width < 3 || height < 3)
		return (MLIB_SUCCESS);

	if (rank == 0)
		return (__mlib_ImageMinFilter3x3_Fp(dst, src));

	if (rank == 8)
		return (__mlib_ImageMaxFilter3x3_Fp(dst, src));

	rankIndex = rank - 1;

	if (type == MLIB_FLOAT) {
		pdst += 4 + stride_d;
	} else if (type == MLIB_DOUBLE) {
		pdst += 8 + stride_d;
		rankIndex += 7;
	}

	rowFunc = mlib_rank_row_funcs_Fp[rankIndex];

	for (i = 0; i < height - 3 + 1; i++) {
		rowFunc(pdst, psrc, width, stride_s);
		pdst += stride_d;
		psrc += stride_s;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
