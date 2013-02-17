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

#pragma ident	"@(#)mlib_ImageRankFilter_US.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageRankFilter3x3_US,
 *      mlib_ImageRankFilter5x5_US,
 *      mlib_ImageRankFilter7x7_US,
 *      mlib_ImageRankFilterMxN_US   - functions perform rank filtering on a
 *                                     MLIB_SHORT type of image that contains
 *                                     unsigned data.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageRankFilter3x3_US (mlib_image       *dst,
 *                                              const mlib_image *src,
 *                                              mlib_s32         rank,
 *                                              mlib_s32         bits)
 *
 *      mlib_status mlib_ImageRankFilter5x5_US (mlib_image       *dst,
 *                                              const mlib_image *src,
 *                                              mlib_s32         rank,
 *                                              mlib_s32         bits)
 *
 *      mlib_status mlib_ImageRankFilter7x7_US (mlib_image       *dst,
 *                                              const mlib_image *src,
 *                                              mlib_s32         rank,
 *                                              mlib_s32         bits)
 *
 *      mlib_status mlib_ImageRankFilterMxN_US (mlib_image       *dst,
 *                                              const mlib_image *src,
 *                                              mlib_s32         m,
 *                                              mlib_s32         n,
 *                                              mlib_s32         rank,
 *                                              mlib_s32         bits)
 *
 * ARGUMENT
 *      dst     pointer to structure of destination image,
 *      src     pointer to structure of source image,
 *      m       width of the filter window, m must be odd number and larger
 *              than 1,
 *      n       height of the filter window, n must be odd number and larger
 *              than 1,
 *      rank    the rank of the output pixel. The pixel with minimum value is
 *              designated rank 0.
 *      bits    the number of unsigned bits for pixel dynamic range.
 *              9 <= bits <= 15.
 *
 * DESCRIPTION
 *      X  - x(i, j), (i = 0 ... w - 1), (j = 0 ... h - 1)
 *      Z  - z(i, j), (i = m/2 ... w - m/2 - 1), (j = n/2 ... h - n/2 - 1)
 *      z(i, j) = rank value in a sorted array of m*n source image pixels -
 *          { x(p, q), (i - m/2 <= p <= i + m/2), (j - n/2 <= q <= j + n/2) }.
 *      so if rank equals 0 - min value, rank equals (m*n - 1) - max value.
 *      if rank equals (m*n)/2 - median value.
 *      The input and output images must be single channel images and have
 *      the same width, height and image type.
 *      They must be MLIB_SHORT type. Pixel data higher bits must be cleared.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageRankFilterFunc.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageRankFilter3x3_US = __mlib_ImageRankFilter3x3_US
#pragma weak mlib_ImageRankFilter5x5_US = __mlib_ImageRankFilter5x5_US
#pragma weak mlib_ImageRankFilter7x7_US = __mlib_ImageRankFilter7x7_US
#pragma weak mlib_ImageRankFilterMxN_US = __mlib_ImageRankFilterMxN_US

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageRankFilter3x3_US) mlib_ImageRankFilter3x3_US
    __attribute__((weak, alias("__mlib_ImageRankFilter3x3_US")));
__typeof__(__mlib_ImageRankFilter5x5_US) mlib_ImageRankFilter5x5_US
    __attribute__((weak, alias("__mlib_ImageRankFilter5x5_US")));
__typeof__(__mlib_ImageRankFilter7x7_US) mlib_ImageRankFilter7x7_US
    __attribute__((weak, alias("__mlib_ImageRankFilter7x7_US")));
__typeof__(__mlib_ImageRankFilterMxN_US) mlib_ImageRankFilterMxN_US
    __attribute__((weak, alias("__mlib_ImageRankFilterMxN_US")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

typedef void (*mlib_row_scan_US) (
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_u8 *histogram);

typedef void (*mlib_col_scan_US) (
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 height,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_d,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_u8 *histogram);

/* *********************************************************** */

static void mlib_rank_row_MxN_bxh1w8(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_u8 *histogram);

static void mlib_rank_col_MxN_bxh1w8(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 height,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_d,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_u8 *histogram);

static void mlib_rank_row_MxN_bxh1w16(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_u8 *histogram);

static void mlib_rank_col_MxN_bxh1w16(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 height,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_d,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_u8 *histogram);

static void mlib_rank_row_MxN_bxh3w8(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_u8 *histogram);

static void mlib_rank_col_MxN_bxh3w8(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 height,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_d,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_u8 *histogram);

static void mlib_rank_row_MxN_bxh3w16(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_u8 *histogram);

static void mlib_rank_col_MxN_bxh3w16(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 height,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_d,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_u8 *histogram);

static void mlib_fill_MxN_US_s32(
    mlib_u8 *pd,
    mlib_s32 value,
    mlib_s32 size);

/* *********************************************************** */

mlib_status
__mlib_ImageRankFilter3x3_US(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 rank,
    mlib_s32 bits)
{
	return (__mlib_ImageRankFilter3x3(dst, src, rank));
}

/* *********************************************************** */

mlib_status
__mlib_ImageRankFilter5x5_US(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 rank,
    mlib_s32 bits)
{
	return (__mlib_ImageRankFilterMxN_US(dst, src, 5, 5, rank, bits));
}

/* *********************************************************** */

mlib_status
__mlib_ImageRankFilter7x7_US(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 rank,
    mlib_s32 bits)
{
	return (__mlib_ImageRankFilterMxN_US(dst, src, 7, 7, rank, bits));
}

/* *********************************************************** */

mlib_status
__mlib_ImageRankFilterMxN_US(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 rank,
    mlib_s32 bits)
{
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 stride_s, stride_d;
	mlib_u8 *ps;
	mlib_u8 *pd;
	mlib_u8 *phist;
	mlib_s32 i, histSize;
	mlib_s32 b1, b2, b3;
	mlib_s32 hLevel = 0;
	mlib_s32 wmin, wmax;
	mlib_row_scan_US row_scan_func = NULL;
	mlib_col_scan_US col_scan_func = NULL;

/* checking images */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_FULL_EQUAL(dst, src);
	MLIB_IMAGE_HAVE_CHAN(dst, 1);
	MLIB_IMAGE_HAVE_TYPE(dst, MLIB_SHORT);

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	stride_s = mlib_ImageGetStride(src);
	stride_d = mlib_ImageGetStride(dst);
	ps = mlib_ImageGetData(src);
	pd = mlib_ImageGetData(dst);

	if (bits < 9 || bits > 15)
		return (MLIB_OUTOFRANGE);

	if (rank < 0 || rank >= m * n)
		return (MLIB_OUTOFRANGE);

	if (m == 3 && n == 3)
		return (__mlib_ImageRankFilter3x3(dst, src, rank));

	if (m == 5 && n == 5 && bits >= 14)
		return (__mlib_ImageRankFilter5x5(dst, src, rank));

	if (bits == 15) {
		return (__mlib_ImageRankFilterMxN(dst, src, m, n, rank));
	}

	if ((m & 1) == 0 || m < 3 || (n & 1) == 0 || n < 3)
		return (MLIB_OUTOFRANGE);
	if (width < m || height < n)
		return (MLIB_SUCCESS);

	if (m < n) {
		wmin = m;
		wmax = n;
	} else {
		wmin = n;
		wmax = m;
	}

	switch (bits) {
	case 9:
		hLevel = 1;
		break;
	case 10:
		switch (wmin) {
		case 3:
			hLevel = 3;
			break;
		case 5:
			hLevel = (wmax > 9) ? 1 : 3;
			break;
		default:
			hLevel = 1;
			break;
		}

		break;
	case 11:
		switch (wmin) {
		case 3:
		case 5:
			hLevel = 3;
			break;
		case 7:
			hLevel = (wmax > 7) ? 1 : 3;
			break;
		default:
			hLevel = 1;
			break;
		}

		break;
	case 12:
		switch (wmin) {
		case 3:
		case 5:
			hLevel = 3;
			break;
		case 7:
			hLevel = (wmax > 15) ? 1 : 3;
			break;
		case 9:
			hLevel = (wmax > 11) ? 1 : 3;
			break;
		default:
			hLevel = 1;
			break;
		}

		break;
	case 13:
		switch (wmin) {
		case 3:
		case 5:
		case 7:
		case 9:
		case 11:
			hLevel = 3;
			break;
		case 13:
			hLevel = (wmax > 15) ? 1 : 3;
			break;
		default:
			hLevel = 1;
			break;
		}

		break;
	case 14:
		hLevel = 3;
		break;
	}

	if (hLevel == 1)
		histSize = (1 << bits);
	else if (hLevel == 3) {
		b1 = bits / 3;
		b2 = (bits - b1) / 2;
		b3 = bits - b1 - b2;
		histSize = (1 << bits) + (1 << (b1 + b2)) + (1 << b1);
	} else {
		b1 = bits / 4;
		b2 = (bits - b1) / 3;
		b3 = (bits - b1 - b2) / 2;
		histSize =
		    (1 << bits) + (1 << (b1 + b2 + b3)) + (1 << (b1 + b2)) +
		    (1 << b1);
	}

	if (m * n >= 256)
		histSize *= 2;

	phist = (mlib_u8 *)__mlib_malloc(histSize);

	if (phist == NULL)
		return (MLIB_FAILURE);

	mlib_fill_MxN_US_s32(phist, 0, histSize);

	rank++;

	pd += (m / 2) * 2 + (n / 2) * stride_d;

	if (n <= m) {

		if (m * n < 256) {
			switch (hLevel) {
			case 1:
				row_scan_func = mlib_rank_row_MxN_bxh1w8;
				break;
			case 3:
				row_scan_func = mlib_rank_row_MxN_bxh3w8;
				break;
			}
		} else {
			switch (hLevel) {
			case 1:
				row_scan_func = mlib_rank_row_MxN_bxh1w16;
				break;
			case 3:
				row_scan_func = mlib_rank_row_MxN_bxh3w16;
				break;
			}
		}

		for (i = 0; i < height - n + 1; i++) {
			row_scan_func(pd, ps, width, rank, m, n, stride_s, bits,
			    phist);
			pd += stride_d;
			ps += stride_s;
		}
	} else {

		if (m * n < 256) {
			switch (hLevel) {
			case 1:
				col_scan_func = mlib_rank_col_MxN_bxh1w8;
				break;
			case 3:
				col_scan_func = mlib_rank_col_MxN_bxh3w8;
				break;
			}
		} else {
			switch (hLevel) {
			case 1:
				col_scan_func = mlib_rank_col_MxN_bxh1w16;
				break;
			case 3:
				col_scan_func = mlib_rank_col_MxN_bxh3w16;
				break;
			}
		}

		for (i = 0; i < width - m + 1; i++) {
			col_scan_func(pd, ps, height, rank, m, n, stride_d,
			    stride_s, bits, phist);
			pd += 2;
			ps += 2;
		}
	}

	__mlib_free(phist);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_fill_MxN_US_s32(
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

void
mlib_rank_row_MxN_bxh1w8(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_u8 *histogram)
{
	const mlib_u8 *psx;
	mlib_s32 i, j, data, count, index;
	mlib_s32 mask = (1 << bits) - 1;
	mlib_u8 *phist = histogram;

	index = count = 0;

	for (j = 0; j < m - 1; j++) {
		psx = ps;
		ps += 2;
		for (i = 0; i < n; i++) {
			data = mask & (*(mlib_u16 *)psx);
			psx += stride_s;
			phist[data]++;
		}
	}

	for (; j < width; j++) {
		psx = ps;
		ps += 2;
		for (i = 0; i < n; i++) {
			data = mask & (*(mlib_u16 *)psx);
			psx += stride_s;
			phist[data]++;
			count += (((data - index) >> 31) & 1);
		}

		if (count < rank) {
			for (; count < rank; index++)
				count += phist[index];
		} else {
			for (; count >= rank; index--)
				count -= phist[index - 1];

			count += phist[index++];
		}

		*(mlib_u16 *)pd = index - 1;
		pd += 2;

		psx = ps - 2 * m;
		for (i = 0; i < n; i++) {
			data = mask & (*(mlib_u16 *)psx);
			psx += stride_s;
			phist[data]--;
			count -= (((data - index) >> 31) & 1);
		}
	}

	ps -= 2 * (m - 1);
	for (j = 0; j < m - 1; j++) {
		psx = ps;
		ps += 2;
		for (i = 0; i < n; i++) {
			data = mask & (*(mlib_u16 *)psx);
			psx += stride_s;
			phist[data]--;
		}
	}
}

/* *********************************************************** */

void
mlib_rank_col_MxN_bxh1w8(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 height,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_d,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_u8 *histogram)
{
	const mlib_u8 *psx;
	mlib_s32 i, j, data, count, index;
	mlib_s32 wdiff = n * stride_s;
	mlib_u8 *phist = histogram;

	index = count = 0;

	for (j = 0; j < n - 1; j++) {
		psx = ps;
		ps += stride_s;
		for (i = 0; i < m; i++) {
			data = *(mlib_u16 *)psx;
			psx += 2;
			phist[data]++;
		}
	}

	for (; j < height; j++) {
		psx = ps;
		ps += stride_s;
		for (i = 0; i < m; i++) {
			data = *(mlib_u16 *)psx;
			psx += 2;
			phist[data]++;
			count += (((data - index) >> 31) & 1);
		}

		if (count < rank) {
			for (; count < rank; index++)
				count += phist[index];
		} else {
			for (; count >= rank; index--)
				count -= phist[index - 1];

			count += phist[index++];
		}

		*(mlib_u16 *)pd = index - 1;
		pd += stride_d;

		psx = ps - wdiff;
		for (i = 0; i < m; i++) {
			data = *(mlib_u16 *)psx;
			psx += 2;
			phist[data]--;
			count -= (((data - index) >> 31) & 1);
		}
	}

	ps -= wdiff;
	for (j = 0; j < n - 1; j++) {
		ps += stride_s;
		psx = ps;
		for (i = 0; i < m; i++) {
			data = *(mlib_u16 *)psx;
			psx += 2;
			phist[data]--;
		}
	}
}

/* *********************************************************** */

void
mlib_rank_row_MxN_bxh1w16(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_u8 *histogram)
{
	const mlib_u8 *psx;
	mlib_s32 i, j, data, count, index;
	mlib_s32 mask = (1 << bits) - 1;
	mlib_u16 *phist = (mlib_u16 *)histogram;

	index = count = 0;

	for (j = 0; j < m - 1; j++) {
		psx = ps;
		ps += 2;
		for (i = 0; i < n; i++) {
			data = mask & (*(mlib_u16 *)psx);
			psx += stride_s;
			phist[data]++;
		}
	}

	for (; j < width; j++) {
		psx = ps;
		ps += 2;
		for (i = 0; i < n; i++) {
			data = mask & (*(mlib_u16 *)psx);
			psx += stride_s;
			phist[data]++;
			count += (((data - index) >> 31) & 1);
		}

		if (count < rank) {
			for (; count < rank; index++)
				count += phist[index];
		} else {
			for (; count >= rank; index--)
				count -= phist[index - 1];

			count += phist[index++];
		}

		*(mlib_u16 *)pd = index - 1;
		pd += 2;

		psx = ps - 2 * m;
		for (i = 0; i < n; i++) {
			data = mask & (*(mlib_u16 *)psx);
			psx += stride_s;
			phist[data]--;
			count -= (((data - index) >> 31) & 1);
		}
	}

	ps -= 2 * (m - 1);
	for (j = 0; j < m - 1; j++) {
		psx = ps;
		ps += 2;
		for (i = 0; i < n; i++) {
			data = mask & (*(mlib_u16 *)psx);
			psx += stride_s;
			phist[data]--;
		}
	}
}

/* *********************************************************** */

void
mlib_rank_col_MxN_bxh1w16(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 height,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_d,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_u8 *histogram)
{
	const mlib_u8 *psx;
	mlib_s32 i, j, data, count, index;
	mlib_s32 wdiff = n * stride_s;
	mlib_u16 *phist = (mlib_u16 *)histogram;

	index = count = 0;

	for (j = 0; j < n - 1; j++) {
		psx = ps;
		ps += stride_s;
		for (i = 0; i < m; i++) {
			data = *(mlib_u16 *)psx;
			psx += 2;
			phist[data]++;
		}
	}

	for (; j < height; j++) {
		psx = ps;
		ps += stride_s;
		for (i = 0; i < m; i++) {
			data = *(mlib_u16 *)psx;
			psx += 2;
			phist[data]++;
			count += (((data - index) >> 31) & 1);
		}

		if (count < rank) {
			for (; count < rank; index++)
				count += phist[index];
		} else {
			for (; count >= rank; index--)
				count -= phist[index - 1];

			count += phist[index++];
		}

		*(mlib_u16 *)pd = index - 1;
		pd += stride_d;

		psx = ps - wdiff;
		for (i = 0; i < m; i++) {
			data = *(mlib_u16 *)psx;
			psx += 2;
			phist[data]--;
			count -= (((data - index) >> 31) & 1);
		}
	}

	ps -= wdiff;
	for (j = 0; j < n - 1; j++) {
		ps += stride_s;
		psx = ps;
		for (i = 0; i < m; i++) {
			data = *(mlib_u16 *)psx;
			psx += 2;
			phist[data]--;
		}
	}
}

/* *********************************************************** */

void
mlib_rank_row_MxN_bxh3w8(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_u8 *histogram)
{
	const mlib_u8 *psx;
	mlib_s32 i, j, data, count, index;
	mlib_s32 mask = (1 << bits) - 1;
	mlib_s32 b1 = bits / 3;
	mlib_s32 b2 = (bits - b1) / 2;
	mlib_s32 b3 = bits - b1 - b2;
	mlib_s32 shft1 = b3;
	mlib_s32 shft2 = b2 + b3;
	mlib_u8 *phist = histogram;
	mlib_u8 *phist2 = phist + (1 << bits);
	mlib_u8 *phist3 = phist2 + (1 << (b1 + b2));

	for (j = 0; j < m - 1; j++) {
		psx = ps;
		ps += 2;
		for (i = 0; i < n; i++) {
			data = mask & (*(mlib_u16 *)psx);
			psx += stride_s;
			phist3[data >> shft2]++;
			phist2[data >> shft1]++;
			phist[data]++;
		}
	}

	for (; j < width; j++) {
		psx = ps;
		ps += 2;
		for (i = 0; i < n; i++) {
			data = mask & (*(mlib_u16 *)psx);
			psx += stride_s;
			phist3[data >> shft2]++;
			phist2[data >> shft1]++;
			phist[data]++;
		}

		for (count = index = 0; count < rank; index++)
			count += phist3[index];

		count -= phist3[--index];
		index = (index << b2);

		for (; count < rank; index++)
			count += phist2[index];

		count -= phist2[--index];
		index = (index << b3);

		for (; count < rank; index++)
			count += phist[index];

		*(mlib_u16 *)pd = index - 1;
		pd += 2;

		psx = ps - 2 * m;
		for (i = 0; i < n; i++) {
			data = mask & (*(mlib_u16 *)psx);
			psx += stride_s;
			phist3[data >> shft2]--;
			phist2[data >> shft1]--;
			phist[data]--;
		}
	}

	ps -= 2 * (m - 1);
	for (j = 0; j < m - 1; j++) {
		psx = ps;
		ps += 2;
		for (i = 0; i < n; i++) {
			data = mask & (*(mlib_u16 *)psx);
			psx += stride_s;
			phist3[data >> shft2]--;
			phist2[data >> shft1]--;
			phist[data]--;
		}
	}
}

/* *********************************************************** */

void
mlib_rank_col_MxN_bxh3w8(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 height,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_d,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_u8 *histogram)
{
	const mlib_u8 *psx;
	mlib_s32 i, j, data, count, index;
	mlib_s32 wdiff = n * stride_s;
	mlib_s32 b1 = bits / 3;
	mlib_s32 b2 = (bits - b1) / 2;
	mlib_s32 b3 = bits - b1 - b2;
	mlib_s32 shft1 = b3;
	mlib_s32 shft2 = b2 + b3;
	mlib_u8 *phist = histogram;
	mlib_u8 *phist2 = phist + (1 << bits);
	mlib_u8 *phist3 = phist2 + (1 << (b1 + b2));

	index = count = 0;

	for (j = 0; j < n - 1; j++) {
		psx = ps;
		ps += stride_s;
		for (i = 0; i < m; i++) {
			data = *(mlib_u16 *)psx;
			psx += 2;
			phist3[data >> shft2]++;
			phist2[data >> shft1]++;
			phist[data]++;
		}
	}

	for (; j < height; j++) {
		psx = ps;
		ps += stride_s;
		for (i = 0; i < m; i++) {
			data = *(mlib_u16 *)psx;
			psx += 2;
			phist3[data >> shft2]++;
			phist2[data >> shft1]++;
			phist[data]++;
		}

		for (count = index = 0; count < rank; index++)
			count += phist3[index];

		count -= phist3[--index];
		index = (index << b2);

		for (; count < rank; index++)
			count += phist2[index];

		count -= phist2[--index];
		index = (index << b3);

		for (; count < rank; index++)
			count += phist[index];

		*(mlib_u16 *)pd = index - 1;
		pd += stride_d;

		psx = ps - wdiff;
		for (i = 0; i < m; i++) {
			data = *(mlib_u16 *)psx;
			psx += 2;
			phist3[data >> shft2]--;
			phist2[data >> shft1]--;
			phist[data]--;
		}
	}

	ps -= wdiff;
	for (j = 0; j < n - 1; j++) {
		ps += stride_s;
		psx = ps;
		for (i = 0; i < m; i++) {
			data = *(mlib_u16 *)psx;
			psx += 2;
			phist3[data >> shft2]--;
			phist2[data >> shft1]--;
			phist[data]--;
		}
	}
}

/* *********************************************************** */

void
mlib_rank_row_MxN_bxh3w16(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_u8 *histogram)
{
	const mlib_u8 *psx;
	mlib_s32 i, j, data, count, index;
	mlib_s32 mask = (1 << bits) - 1;
	mlib_s32 b1 = bits / 3;
	mlib_s32 b2 = (bits - b1) / 2;
	mlib_s32 b3 = bits - b1 - b2;
	mlib_s32 shft1 = b3;
	mlib_s32 shft2 = b2 + b3;
	mlib_u16 *phist = (mlib_u16 *)histogram;
	mlib_u16 *phist2 = phist + (1 << bits);
	mlib_u16 *phist3 = phist2 + (1 << (b1 + b2));

	for (j = 0; j < m - 1; j++) {
		psx = ps;
		ps += 2;
		for (i = 0; i < n; i++) {
			data = mask & (*(mlib_u16 *)psx);
			psx += stride_s;
			phist3[data >> shft2]++;
			phist2[data >> shft1]++;
			phist[data]++;
		}
	}

	for (; j < width; j++) {
		psx = ps;
		ps += 2;
		for (i = 0; i < n; i++) {
			data = mask & (*(mlib_u16 *)psx);
			psx += stride_s;
			phist3[data >> shft2]++;
			phist2[data >> shft1]++;
			phist[data]++;
		}

		for (count = index = 0; count < rank; index++)
			count += phist3[index];

		count -= phist3[--index];
		index = (index << b2);

		for (; count < rank; index++)
			count += phist2[index];

		count -= phist2[--index];
		index = (index << b3);

		for (; count < rank; index++)
			count += phist[index];

		*(mlib_u16 *)pd = index - 1;
		pd += 2;

		psx = ps - 2 * m;
		for (i = 0; i < n; i++) {
			data = mask & (*(mlib_u16 *)psx);
			psx += stride_s;
			phist3[data >> shft2]--;
			phist2[data >> shft1]--;
			phist[data]--;
		}
	}

	ps -= 2 * (m - 1);
	for (j = 0; j < m - 1; j++) {
		psx = ps;
		ps += 2;
		for (i = 0; i < n; i++) {
			data = mask & (*(mlib_u16 *)psx);
			psx += stride_s;
			phist3[data >> shft2]--;
			phist2[data >> shft1]--;
			phist[data]--;
		}
	}
}

/* *********************************************************** */

void
mlib_rank_col_MxN_bxh3w16(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 height,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_d,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_u8 *histogram)
{
	const mlib_u8 *psx;
	mlib_s32 i, j, data, count, index;
	mlib_s32 wdiff = n * stride_s;
	mlib_s32 b1 = bits / 3;
	mlib_s32 b2 = (bits - b1) / 2;
	mlib_s32 b3 = bits - b1 - b2;
	mlib_s32 shft1 = b3;
	mlib_s32 shft2 = b2 + b3;
	mlib_u16 *phist = (mlib_u16 *)histogram;
	mlib_u16 *phist2 = phist + (1 << bits);
	mlib_u16 *phist3 = phist2 + (1 << (b1 + b2));

	index = count = 0;

	for (j = 0; j < n - 1; j++) {
		psx = ps;
		ps += stride_s;
		for (i = 0; i < m; i++) {
			data = *(mlib_u16 *)psx;
			psx += 2;
			phist3[data >> shft2]++;
			phist2[data >> shft1]++;
			phist[data]++;
		}
	}

	for (; j < height; j++) {
		psx = ps;
		ps += stride_s;
		for (i = 0; i < m; i++) {
			data = *(mlib_u16 *)psx;
			psx += 2;
			phist3[data >> shft2]++;
			phist2[data >> shft1]++;
			phist[data]++;
		}

		for (count = index = 0; count < rank; index++)
			count += phist3[index];

		count -= phist3[--index];
		index = (index << b2);

		for (; count < rank; index++)
			count += phist2[index];

		count -= phist2[--index];
		index = (index << b3);

		for (; count < rank; index++)
			count += phist[index];

		*(mlib_u16 *)pd = index - 1;
		pd += stride_d;

		psx = ps - wdiff;
		for (i = 0; i < m; i++) {
			data = *(mlib_u16 *)psx;
			psx += 2;
			phist3[data >> shft2]--;
			phist2[data >> shft1]--;
			phist[data]--;
		}
	}

	ps -= wdiff;
	for (j = 0; j < n - 1; j++) {
		ps += stride_s;
		psx = ps;
		for (i = 0; i < m; i++) {
			data = *(mlib_u16 *)psx;
			psx += 2;
			phist3[data >> shft2]--;
			phist2[data >> shft1]--;
			phist[data]--;
		}
	}
}

/* *********************************************************** */
