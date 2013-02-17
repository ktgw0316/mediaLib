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

#pragma ident	"@(#)mlib_ImageRankFilter5x5.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageRankFilter5x5   - performs rank filtering on an image.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageRankFilter5x5(mlib_image       *dst,
 *                                          const mlib_image *src,
 *                                          mlib_s32         rank)
 *
 * ARGUMENT
 *      dst     pointer to structure of destination image,
 *      src     pointer to structure of source image,
 *      rank    the rank of the output pixel. rank value must be from 0 to 24.
 *              The pixel with minimum value is designated rank 0 and
 *              the pixel with maximum value is designated rank 24.
 *
 * DESCRIPTION
 *      X  - x(i, j), (i = 0 ... w - 1), (j = 0 ... h - 1)
 *      Z  - z(i, j), (i = 2 ... w - 3), (j = 2 ... h - 3)
 *      z(i, j) = rank value in a sorted array of 25 source image pixels -
 *                { x(p, q), (i - 2 <= p <= i + 2), (j - 2 <= q <= j + 2) }.
 *      so if rank equals 0 - min value, rank equals 24 - max value.
 *      if rank equals 12 - median value.
 *      The input and output images must be single channel images and have
 *      the same width, height and image type.
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT type.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageRankFilter5x5 = __mlib_ImageRankFilter5x5

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageRankFilter5x5) mlib_ImageRankFilter5x5
    __attribute__((weak, alias("__mlib_ImageRankFilter5x5")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	sort2(a, b)	mask = ((b - a) & ((b - a) >> 31)), a += mask, b -= mask

/* *********************************************************** */

static void mlib_rank_row_5x5_b8(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 stride_s);

static void mlib_fill_5x5_s32(
    mlib_u8 *pd,
    mlib_s32 value,
    mlib_s32 size);

/* *********************************************************** */

mlib_status
__mlib_ImageRankFilter5x5(
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
	mlib_s32 i;

/* checking images */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_FULL_EQUAL(dst, src);
	MLIB_IMAGE_HAVE_CHAN(dst, 1);

	type = mlib_ImageGetType(dst);

	if (type != MLIB_BYTE && type != MLIB_SHORT && type != MLIB_USHORT &&
	    type != MLIB_INT)
		return (MLIB_FAILURE);

	if (rank < 0 || rank > 24)
		return (MLIB_OUTOFRANGE);

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	stride_s = mlib_ImageGetStride(src);
	stride_d = mlib_ImageGetStride(dst);
	psrc = mlib_ImageGetData(src);
	pdst = mlib_ImageGetData(dst);

	if (width < 5 || height < 5)
		return (MLIB_SUCCESS);

	if (rank == 0)
		return (__mlib_ImageMinFilter5x5(dst, src));

	if (rank == 24)
		return (__mlib_ImageMaxFilter5x5(dst, src));

	if (type != MLIB_BYTE)
		return (__mlib_ImageRankFilterMxN(dst, src, 5, 5, rank));

	rank++;

	pdst += 2 + 2 * stride_d;

	for (i = 0; i < height - 4; i++) {
		mlib_rank_row_5x5_b8(pdst, psrc, width, rank, stride_s);
		pdst += stride_d;
		psrc += stride_s;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_rank_row_5x5_b8(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 stride_s)
{
	mlib_d64 hist_buff[256 / sizeof (mlib_d64)];
	mlib_u8 *phist = (mlib_u8 *)hist_buff;
	const mlib_u8 *ps1 = ps;
	const mlib_u8 *ps2 = ps + stride_s;
	const mlib_u8 *ps3 = ps + stride_s * 2;
	const mlib_u8 *ps4 = ps + stride_s * 3;
	const mlib_u8 *ps5 = ps + stride_s * 4;
	mlib_s32 j, data, count, index;

	mlib_fill_5x5_s32(phist, 0, 256);

	index = count = 0;

	phist[ps1[0]]++;
	phist[ps1[1]]++;
	phist[ps1[2]]++;
	phist[ps1[3]]++;

	phist[ps2[0]]++;
	phist[ps2[1]]++;
	phist[ps2[2]]++;
	phist[ps2[3]]++;

	phist[ps3[0]]++;
	phist[ps3[1]]++;
	phist[ps3[2]]++;
	phist[ps3[3]]++;

	phist[ps4[0]]++;
	phist[ps4[1]]++;
	phist[ps4[2]]++;
	phist[ps4[3]]++;

	phist[ps5[0]]++;
	phist[ps5[1]]++;
	phist[ps5[2]]++;
	phist[ps5[3]]++;

	for (j = 0; j < width - 4; j++) {
		data = ps1[4];
		phist[data]++;
		count += (((data - index) >> 31) & 1);

		data = ps2[4];
		phist[data]++;
		count += (((data - index) >> 31) & 1);

		data = ps3[4];
		phist[data]++;
		count += (((data - index) >> 31) & 1);

		data = ps4[4];
		phist[data]++;
		count += (((data - index) >> 31) & 1);

		data = ps5[4];
		phist[data]++;
		count += (((data - index) >> 31) & 1);

		if (count < rank) {
			for (; count < rank; index++)
				count += phist[index];
		} else {
			for (; count >= rank; index--)
				count -= phist[index - 1];

			count += phist[index++];
		}

		*pd = index - 1;
		pd++;

		data = (*ps1++);
		phist[data]--;
		count -= (((data - index) >> 31) & 1);

		data = (*ps2++);
		phist[data]--;
		count -= (((data - index) >> 31) & 1);

		data = (*ps3++);
		phist[data]--;
		count -= (((data - index) >> 31) & 1);

		data = (*ps4++);
		phist[data]--;
		count -= (((data - index) >> 31) & 1);

		data = (*ps5++);
		phist[data]--;
		count -= (((data - index) >> 31) & 1);
	}
}

/* *********************************************************** */

void
mlib_fill_5x5_s32(
    mlib_u8 *pd,
    mlib_s32 value,
    mlib_s32 size)
{
	mlib_s32 i;
	mlib_s32 *pl = (mlib_s32 *)pd;

	value &= 0xFF;
	value = (value | (value << 8));
	value = (value | (value << 16));

	for (i = 0; i < size - 15; i += 16) {
		pl[0] = value;
		pl[1] = value;
		pl[2] = value;
		pl[3] = value;
		pl += 4;
	}

	for (; i < size - 3; i += 4)
		(*pl++) = value;

	pd = (mlib_u8 *)pl;

	for (; i < size; i++)
		(*pd++) = value;
}

/* *********************************************************** */
