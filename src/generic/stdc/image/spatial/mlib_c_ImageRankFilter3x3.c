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

#pragma ident	"@(#)mlib_c_ImageRankFilter3x3.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageRankFilter3x3   - performs rank filtering on an image.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageRankFilter3x3(mlib_image       *dst,
 *                                          const mlib_image *src,
 *                                          mlib_s32         rank)
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
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT type.
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageRankFilter.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageRankFilter3x3 = __mlib_ImageRankFilter3x3

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageRankFilter3x3) mlib_ImageRankFilter3x3
    __attribute__((weak, alias("__mlib_ImageRankFilter3x3")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

RANK_ROW(mlib_u8, mlib_s32, SORT2, RANK_1_FUNC, r1_u8)
RANK_ROW(mlib_u8, mlib_s32, SORT2, RANK_2_FUNC, r2_u8)
RANK_ROW(mlib_u8, mlib_s32, SORT2, RANK_3_FUNC, r3_u8)
RANK_ROW(mlib_u8, mlib_s32, SORT2, RANK_5_FUNC, r5_u8)
RANK_ROW(mlib_u8, mlib_s32, SORT2, RANK_6_FUNC, r6_u8)
RANK_ROW(mlib_u8, mlib_s32, SORT2, RANK_7_FUNC, r7_u8)

RANK_ROW(mlib_s16, mlib_s32, SORT2, RANK_1_FUNC, r1_s16)
RANK_ROW(mlib_s16, mlib_s32, SORT2, RANK_2_FUNC, r2_s16)
RANK_ROW(mlib_s16, mlib_s32, SORT2, RANK_3_FUNC, r3_s16)
RANK_ROW(mlib_s16, mlib_s32, SORT2, RANK_5_FUNC, r5_s16)
RANK_ROW(mlib_s16, mlib_s32, SORT2, RANK_6_FUNC, r6_s16)
RANK_ROW(mlib_s16, mlib_s32, SORT2, RANK_7_FUNC, r7_s16)

RANK_ROW(mlib_u16, mlib_s32, SORT2, RANK_1_FUNC, r1_u16)
RANK_ROW(mlib_u16, mlib_s32, SORT2, RANK_2_FUNC, r2_u16)
RANK_ROW(mlib_u16, mlib_s32, SORT2, RANK_3_FUNC, r3_u16)
RANK_ROW(mlib_u16, mlib_s32, SORT2, RANK_5_FUNC, r5_u16)
RANK_ROW(mlib_u16, mlib_s32, SORT2, RANK_6_FUNC, r6_u16)
RANK_ROW(mlib_u16, mlib_s32, SORT2, RANK_7_FUNC, r7_u16)

/*
 * DESCRIBE_ROW_FUNC(r1_s32);
 * DESCRIBE_ROW_FUNC(r2_s32);
 * DESCRIBE_ROW_FUNC(r3_s32);
 * DESCRIBE_ROW_FUNC(r5_s32);
 * DESCRIBE_ROW_FUNC(r6_s32);
 * DESCRIBE_ROW_FUNC(r7_s32);
 */

/* *********************************************************** */

static const mlib_RankFilterRow mlib_rank_row_funcs[] = {
	mlib_rank_row_3x3_r1_u8,
	mlib_rank_row_3x3_r2_u8,
	mlib_rank_row_3x3_r3_u8,
	mlib_rank_row_3x3_r5_u8,
	mlib_rank_row_3x3_r6_u8,
	mlib_rank_row_3x3_r7_u8,
	mlib_rank_row_3x3_r1_s16,
	mlib_rank_row_3x3_r2_s16,
	mlib_rank_row_3x3_r3_s16,
	mlib_rank_row_3x3_r5_s16,
	mlib_rank_row_3x3_r6_s16,
	mlib_rank_row_3x3_r7_s16,
	mlib_rank_row_3x3_r1_u16,
	mlib_rank_row_3x3_r2_u16,
	mlib_rank_row_3x3_r3_u16,
	mlib_rank_row_3x3_r5_u16,
	mlib_rank_row_3x3_r6_u16,
	mlib_rank_row_3x3_r7_u16,
	mlib_rank_row_3x3_r1_s32,
	mlib_rank_row_3x3_r2_s32,
	mlib_rank_row_3x3_r3_s32,
	mlib_rank_row_3x3_r5_s32,
	mlib_rank_row_3x3_r6_s32,
	mlib_rank_row_3x3_r7_s32
};

/* *********************************************************** */

mlib_status
__mlib_ImageRankFilter3x3(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 rank)
{
	mlib_s32 width, height, stride_s, stride_d, tsize;
	mlib_type type;
	mlib_u8 *psrc, *pdst;
	mlib_s32 i;
	mlib_RankFilterRow rowFunc;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_FULL_EQUAL(dst, src);
	MLIB_IMAGE_HAVE_CHAN(dst, 1);

	type = mlib_ImageGetType(dst);
	width = mlib_ImageGetWidth(dst);
	height = mlib_ImageGetHeight(dst);
	stride_d = mlib_ImageGetStride(dst);
	pdst = (void *)mlib_ImageGetData(dst);

	stride_s = mlib_ImageGetStride(src);
	psrc = mlib_ImageGetData(src);

	if (rank <= 0) {
		if (rank < 0)
			return (MLIB_OUTOFRANGE);
		return (__mlib_ImageMinFilter3x3(dst, src));
	}

	if (rank == 4) {
		return __mlib_ImageMedianFilter3x3(dst, src,
		    MLIB_MEDIAN_MASK_RECT, 0xF, MLIB_EDGE_DST_NO_WRITE);
	}

	if (rank >= 8) {
		if (rank > 8)
			return (MLIB_OUTOFRANGE);
		return (__mlib_ImageMaxFilter3x3(dst, src));
	}

	if (width <= 2 || height <= 2) {
		return (MLIB_SUCCESS);
	}

	rank--;

	if (rank > 3)
		rank--;

	if (type == MLIB_BYTE) {
		tsize = 1;
	} else if (type == MLIB_SHORT) {
		tsize = 2;
		rank += 6;
	} else if (type == MLIB_USHORT) {
		tsize = 2;
		rank += 12;
	} else if (type == MLIB_INT) {
		tsize = 4;
		rank += 18;
	} else {
		return (MLIB_FAILURE);
	}

	pdst += stride_d + tsize;
	rowFunc = mlib_rank_row_funcs[rank];

	for (i = 0; i < height - 3 + 1; i++) {
		rowFunc(pdst, psrc, width, stride_s);
		pdst += stride_d;
		psrc += stride_s;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
