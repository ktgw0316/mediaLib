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

#pragma ident	"@(#)mlib_v_ImageRankFilter3x3.c	9.2	07/11/05 SMI"

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
#include <mlib_ImageCopy.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageRankFilter.h>
#include <mlib_v_ImageDataTypeConvert.h>
#include <mlib_v_ImageLineMaxMin.h>
#include <mlib_v_ImageDilateErode.h>

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

typedef void (*type_line_func) (
	mlib_u8 *dp,
	const mlib_u8 *sp,
	mlib_s32 n);

/* *********************************************************** */

#define	LINE_SORT(buff0, buff1)	func_LineSort(buff0, buff1, width)

/* *********************************************************** */

#define	LINE_MAX_(buff0, buff1)	func_LineMax(buff1, buff0, width)

/* *********************************************************** */

#define	LINE_MIN_(buff0, buff1)	func_LineMin(buff0, buff1, width)

/* *********************************************************** */

static const mlib_RankFilterRow mlib_rank_row_funcs[] = {
	NULL,
	mlib_rank_row_3x3_r1_s32,
	mlib_rank_row_3x3_r2_s32,
	mlib_rank_row_3x3_r3_s32,
	NULL,
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
	mlib_d64 *buff;
	void *buff_arr[21], *buff1, *buff2, *buff3, *buff4;
	void *buff5, *buff6, *buff7, *buff8, *buff9;
	mlib_s32 width, height, stride_s, stride_d, tsize;
	mlib_type type;
	mlib_u8 *psrc, *pdst;
	mlib_s32 i, dsize, bline, b_ind = 0, z_ind = 0;
	type_line_func func_LineSort, func_LineMax, func_LineMin;
	type_line_func func_SrcCopy, func_DstCopy;

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

	if (type == MLIB_BYTE) {
		tsize = 1;
		func_SrcCopy =
		    (type_line_func) & mlib_v_ImageDataTypeConvert_U8_S16_D1;
		func_DstCopy =
		    (type_line_func) & mlib_v_ImageDataTypeConvert_S16_U8_D1;
	} else if (type == MLIB_SHORT) {
		tsize = 2;
		func_SrcCopy = (type_line_func) & mlib_ImageCopy_na;
		func_DstCopy = (type_line_func) & mlib_ImageCopy_na;
	} else if (type == MLIB_USHORT) {
		tsize = 2;
		func_SrcCopy = (type_line_func) & mlib_ImageLineXor8000;
		func_DstCopy = (type_line_func) & mlib_ImageLineXor8000;
	} else if (type == MLIB_INT) {
		mlib_RankFilterRow rowFunc = mlib_rank_row_funcs[rank];

		pdst += stride_d + 4;
		for (i = 0; i < height - 2; i++) {
			rowFunc(pdst, psrc, width, stride_s);
			pdst += stride_d;
			psrc += stride_s;
		}

		return (MLIB_SUCCESS);
	} else {
		return (MLIB_FAILURE);
	}

	width -= 2;
	pdst += stride_d + tsize;
	dsize = width * tsize;

	bline = (width + 3) / 4;
	buff = __mlib_malloc((9 + 3) * sizeof (mlib_d64) * bline);
	for (i = 0; i < 9; i++) {
		buff_arr[i + 9] = buff_arr[i] = buff + i * bline;
	}

	for (i = 0; i < 3; i++) {
		buff_arr[i + 18] = buff + (i + 9) * bline;
	}

	if (rank < 4) {
		func_LineSort = mlib_ImageLineSortMin;
		func_LineMin = mlib_ImageLineMin;
		func_LineMax = mlib_ImageLineMax;
	} else {
		rank = 8 - rank;
		func_LineSort = mlib_ImageLineSortMax;
		func_LineMin = mlib_ImageLineMax;
		func_LineMax = mlib_ImageLineMin;
	}

	for (i = 0; i < height; i++) {
		buff7 = buff_arr[b_ind + 6];
		buff8 = buff_arr[b_ind + 7];
		buff9 = buff_arr[b_ind + 8];
		func_SrcCopy(psrc, buff7, dsize);
		func_SrcCopy(psrc + tsize, buff8, dsize);
		func_SrcCopy(psrc + 2 * tsize, buff9, dsize);
		psrc += stride_s;

		LINE_SORT(buff7, buff8);
		LINE_SORT(buff7, buff9);

		if (rank == 1) {
			LINE_MIN_(buff8, buff9);
		} else {
			LINE_SORT(buff8, buff9);
		}

/* first two lines of source without write to destination */
		if (i < 2) {
			b_ind += 3;
			continue;
		}

		if (!z_ind) {
			buff1 = buff_arr[b_ind + 3];
			buff2 = buff_arr[b_ind + 4];
			buff3 = buff_arr[b_ind + 5];
			buff4 = buff_arr[b_ind + 6];
			buff5 = buff_arr[b_ind + 7];
			buff6 = buff_arr[b_ind + 8];
			buff7 = buff_arr[b_ind];
			buff8 = buff_arr[b_ind + 1];
			buff9 = buff_arr[b_ind + 2];

			if (rank == 1) {
				mlib_ImageCopy_na(buff4, buff3, 8 * bline);
				LINE_SORT(buff1, buff3);
				LINE_MIN_(buff2, buff5);

				LINE_MAX_(buff1, buff7);
				LINE_MIN_(buff7, buff3);
				LINE_MIN_(buff8, buff2);
				LINE_MIN_(buff7, buff8);
			} else if (rank == 2) {
				LINE_MIN_(buff1, buff6);
				LINE_MIN_(buff2, buff5);
				LINE_MIN_(buff3, buff4);
				LINE_SORT(buff1, buff2);
				LINE_SORT(buff1, buff3);
				LINE_SORT(buff2, buff3);

				LINE_MIN_(buff9, buff1);
				LINE_MIN_(buff8, buff2);
				LINE_MIN_(buff7, buff3);
				LINE_MAX_(buff9, buff7);
				LINE_MAX_(buff8, buff7);
			} else {
/* (rank == 3) */

				mlib_ImageCopy_na(buff4, buff_arr[18],
				    3 * 8 * bline);
				buff4 = buff_arr[18];
				buff5 = buff_arr[19];
				buff6 = buff_arr[20];
				LINE_SORT(buff1, buff6);
				LINE_SORT(buff2, buff5);
				LINE_SORT(buff3, buff4);
				LINE_SORT(buff1, buff2);
				LINE_SORT(buff1, buff3);
				LINE_SORT(buff2, buff3);
				LINE_MIN_(buff4, buff5);
				LINE_MIN_(buff4, buff6);

				LINE_MAX_(buff1, buff9);
				LINE_MAX_(buff2, buff8);
				LINE_MAX_(buff3, buff7);
				LINE_MIN_(buff7, buff8);
				LINE_MIN_(buff7, buff9);
				LINE_MIN_(buff7, buff4);
			}

			func_DstCopy(buff7, pdst, dsize);
		} else {
			buff1 = buff_arr[b_ind];
			buff2 = buff_arr[b_ind + 1];
			buff3 = buff_arr[b_ind + 2];

			if (rank == 1) {
				LINE_MAX_(buff7, buff1);
				LINE_MIN_(buff3, buff1);
				LINE_MIN_(buff2, buff8);
				LINE_MIN_(buff3, buff2);
			} else if (rank == 2) {
				LINE_MIN_(buff1, buff9);
				LINE_MIN_(buff2, buff8);
				LINE_MIN_(buff3, buff7);
				LINE_MAX_(buff1, buff3);
				LINE_MAX_(buff2, buff3);
			} else {
/* (rank == 3) */

				buff4 = buff_arr[18];
				LINE_MAX_(buff9, buff1);
				LINE_MAX_(buff8, buff2);
				LINE_MAX_(buff7, buff3);
				LINE_MIN_(buff3, buff2);
				LINE_MIN_(buff3, buff1);
				LINE_MIN_(buff3, buff4);
			}

			func_DstCopy(buff3, pdst, dsize);
		}

		pdst += stride_d;

		b_ind += 3;

		if (b_ind >= 9)
			b_ind -= 9;
		z_ind ^= 1;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
