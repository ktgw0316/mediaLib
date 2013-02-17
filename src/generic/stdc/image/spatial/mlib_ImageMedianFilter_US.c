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

#pragma ident	"@(#)mlib_ImageMedianFilter_US.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageMedianFilter3x3_US,
 *      mlib_ImageMedianFilter5x5_US,
 *      mlib_ImageMedianFilter7x7_US,
 *      mlib_ImageMedianFilterMxN_US - functions perform median filtering on a
 *                                     MLIB_SHORT type of image that contains
 *                                     mlib_u32 data.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageMedianFilter3x3_US(mlib_image       *dst,
 *                                               mlib_image       *src,
 *                                               mlib_median_mask mmask,
 *                                               mlib_s32         cmask,
 *                                               mlib_edge        edge,
 *                                               mlib_s32         bits)
 *
 *      mlib_status mlib_ImageMedianFilter5x5_US(mlib_image       *dst,
 *                                               mlib_image       *src,
 *                                               mlib_median_mask mmask,
 *                                               mlib_s32         cmask,
 *                                               mlib_edge        edge,
 *                                               mlib_s32         bits)
 *
 *      mlib_status mlib_ImageMedianFilter7x7_US(mlib_image       *dst,
 *                                               mlib_image       *src,
 *                                               mlib_median_mask mmask,
 *                                               mlib_s32         cmask,
 *                                               mlib_edge        edge,
 *                                               mlib_s32         bits)
 *
 *      mlib_status mlib_ImageMedianFilterMxN_US(mlib_image       *dst,
 *                                               mlib_image       *src,
 *                                               mlib_s32         m,
 *                                               mlib_s32         n,
 *                                               mlib_median_mask mmask,
 *                                               mlib_s32         cmask,
 *                                               mlib_edge        edge,
 *                                               mlib_s32         bits)
 *
 * ARGUMENTS
 *      dst     Pointer to destination image.
 *      src     Pointer to source image.
 *      m       width of filter mask, m must be an odd number larger than 1
 *      n       height of filter mask, n must be an odd number larger than 1
 *      mmask   Shape of the mask to be used for median filtering.
 *      cmask   Channel mask to indicate the channels to be filtered.
 *              Each bit of which represents a channel in the image. The
 *              channels corresponded to 1 bits are those to be processed.
 *      edge    Type of edge condition.
 *      bits    The number of mlib_u32 bits for pixel dynamic range.
 *              9 <= bits <= 15.
 *
 * DESCRIPTION
 *      Median filtering.
 *
 *      There are several shapes possible for the mask: rectangle mask,
 *      plus mask, and X mask.
 *
 *      The separable rectangle median is defined as the median of the
 *      medians of each row.
 *
 *      The unselected channels are not overwritten. If both src and dst have
 *      just one channel, cmask is ignored.
 *
 *      The mapping of source image to destination image is left/top corner to
 *      left/top corner, but with a shift of the destination image if it's
 *      smaller than the source image.
 *      The exact mapping formula is
 *           xs = xd + x_offset
 *           ys = yd + y_offset
 *      where
 *           x_offset = (dst_width >= src_width) ? 0 : (kernel_size - 1)/2
 *           y_offset = (dst_height >= src_height) ? 0 : (kernel_size - 1)/2
 *
 *      The src and the dst must be images of MLIB_SHORT type.
 *      The src and dst must have same number of channels (1, 2, 3, or 4).
 *
 *      The mask shape can be one of the following:
 *              MLIB_MEDIAN_MASK_RECT   (default)
 *              MLIB_MEDIAN_MASK_PLUS
 *              MLIB_MEDIAN_MASK_X
 *              MLIB_MEDIAN_MASK_RECT_SEPARABLE
 *
 *      In the case with an MLIB_MEDIAN_MASK_X mask, there must be (m == n).
 *
 *      The edge condition can be one of the following:
 *              MLIB_EDGE_DST_NO_WRITE  (default)
 *              MLIB_EDGE_DST_FILL_ZERO
 *              MLIB_EDGE_DST_COPY_SRC
 *              MLIB_EDGE_SRC_EXTEND
 */

#include <mlib_image.h>
#include <mlib_ImageCreate.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageMedianFilter.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMedianFilter3x3_US = __mlib_ImageMedianFilter3x3_US
#pragma weak mlib_ImageMedianFilter5x5_US = __mlib_ImageMedianFilter5x5_US
#pragma weak mlib_ImageMedianFilter7x7_US = __mlib_ImageMedianFilter7x7_US
#pragma weak mlib_ImageMedianFilterMxN_US = __mlib_ImageMedianFilterMxN_US

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMedianFilter3x3_US) mlib_ImageMedianFilter3x3_US
    __attribute__((weak, alias("__mlib_ImageMedianFilter3x3_US")));
__typeof__(__mlib_ImageMedianFilter5x5_US) mlib_ImageMedianFilter5x5_US
    __attribute__((weak, alias("__mlib_ImageMedianFilter5x5_US")));
__typeof__(__mlib_ImageMedianFilter7x7_US) mlib_ImageMedianFilter7x7_US
    __attribute__((weak, alias("__mlib_ImageMedianFilter7x7_US")));
__typeof__(__mlib_ImageMedianFilterMxN_US) mlib_ImageMedianFilterMxN_US
    __attribute__((weak, alias("__mlib_ImageMedianFilterMxN_US")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

typedef void (*mlib_row_scan_US) (
    mlib_u8 *pd,
    mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_s32 nchan2,
    mlib_u8 *histogram);

typedef void (*mlib_col_scan_US) (
    mlib_u8 *pd,
    mlib_u8 *ps,
    mlib_s32 height,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_d,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_s32 nchan2,
    mlib_u8 *histogram);

/* *********************************************************** */

static void mlib_fill_MxN_US_s32(
    mlib_u8 *pd,
    mlib_s32 value,
    mlib_s32 size);

static void mlib_rank_row_MxN_bxh1w8(
    mlib_u8 *pd,
    mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_s32 nchan2,
    mlib_u8 *histogram);

static void mlib_rank_col_MxN_bxh1w8(
    mlib_u8 *pd,
    mlib_u8 *ps,
    mlib_s32 height,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_d,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_s32 nchan2,
    mlib_u8 *histogram);

static void mlib_rank_row_MxN_bxh1w16(
    mlib_u8 *pd,
    mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_s32 nchan2,
    mlib_u8 *histogram);

static void mlib_rank_col_MxN_bxh1w16(
    mlib_u8 *pd,
    mlib_u8 *ps,
    mlib_s32 height,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_d,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_s32 nchan2,
    mlib_u8 *histogram);

static void mlib_rank_row_MxN_bxh3w8(
    mlib_u8 *pd,
    mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_s32 nchan2,
    mlib_u8 *histogram);

static void mlib_rank_col_MxN_bxh3w8(
    mlib_u8 *pd,
    mlib_u8 *ps,
    mlib_s32 height,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_d,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_s32 nchan2,
    mlib_u8 *histogram);

static void mlib_rank_row_MxN_bxh3w16(
    mlib_u8 *pd,
    mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_s32 nchan2,
    mlib_u8 *histogram);

static void mlib_rank_col_MxN_bxh3w16(
    mlib_u8 *pd,
    mlib_u8 *ps,
    mlib_s32 height,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_d,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_s32 nchan2,
    mlib_u8 *histogram);

static mlib_status mlib_ImageRankFilterMxN_US_i(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 rank,
    mlib_s32 cmask,
    mlib_s32 bits);

static mlib_status mlib_ImageProcessEdges(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 cmask,
    mlib_edge edge);

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
    mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_s32 nchan2,
    mlib_u8 *histogram)
{
	mlib_u8 *psx;
	mlib_s32 i, j, data, count, index;
	mlib_s32 mask = (1 << bits) - 1;
	mlib_u8 *phist = histogram;

	index = count = 0;

	for (j = 0; j < m - 1; j++) {
		psx = ps;
		ps += nchan2;
		for (i = 0; i < n; i++) {
			data = mask & (*(mlib_u16 *)psx);
			psx += stride_s;
			phist[data]++;
		}
	}

	for (; j < width; j++) {
		psx = ps;
		ps += nchan2;
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
		pd += nchan2;

		psx = ps - nchan2 * m;
		for (i = 0; i < n; i++) {
			data = mask & (*(mlib_u16 *)psx);
			psx += stride_s;
			phist[data]--;
			count -= (((data - index) >> 31) & 1);
		}
	}

	ps -= nchan2 * (m - 1);
	for (j = 0; j < m - 1; j++) {
		psx = ps;
		ps += nchan2;
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
    mlib_u8 *ps,
    mlib_s32 height,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_d,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_s32 nchan2,
    mlib_u8 *histogram)
{
	mlib_u8 *psx;
	mlib_s32 i, j, data, count, index;
	mlib_s32 wdiff = n * stride_s;
	mlib_u8 *phist = histogram;

	index = count = 0;

	for (j = 0; j < n - 1; j++) {
		psx = ps;
		ps += stride_s;
		for (i = 0; i < m; i++) {
			data = *(mlib_u16 *)psx;
			psx += nchan2;
			phist[data]++;
		}
	}

	for (; j < height; j++) {
		psx = ps;
		ps += stride_s;
		for (i = 0; i < m; i++) {
			data = *(mlib_u16 *)psx;
			psx += nchan2;
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
			psx += nchan2;
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
			psx += nchan2;
			phist[data]--;
		}
	}
}

/* *********************************************************** */

void
mlib_rank_row_MxN_bxh1w16(
    mlib_u8 *pd,
    mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_s32 nchan2,
    mlib_u8 *histogram)
{
	mlib_u8 *psx;
	mlib_s32 i, j, data, count, index;
	mlib_s32 mask = (1 << bits) - 1;
	mlib_u16 *phist = (mlib_u16 *)histogram;

	index = count = 0;

	for (j = 0; j < m - 1; j++) {
		psx = ps;
		ps += nchan2;
		for (i = 0; i < n; i++) {
			data = mask & (*(mlib_u16 *)psx);
			psx += stride_s;
			phist[data]++;
		}
	}

	for (; j < width; j++) {
		psx = ps;
		ps += nchan2;
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
		pd += nchan2;

		psx = ps - nchan2 * m;
		for (i = 0; i < n; i++) {
			data = mask & (*(mlib_u16 *)psx);
			psx += stride_s;
			phist[data]--;
			count -= (((data - index) >> 31) & 1);
		}
	}

	ps -= nchan2 * (m - 1);
	for (j = 0; j < m - 1; j++) {
		psx = ps;
		ps += nchan2;
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
    mlib_u8 *ps,
    mlib_s32 height,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_d,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_s32 nchan2,
    mlib_u8 *histogram)
{
	mlib_u8 *psx;
	mlib_s32 i, j, data, count, index;
	mlib_s32 wdiff = n * stride_s;
	mlib_u16 *phist = (mlib_u16 *)histogram;

	index = count = 0;

	for (j = 0; j < n - 1; j++) {
		psx = ps;
		ps += stride_s;
		for (i = 0; i < m; i++) {
			data = *(mlib_u16 *)psx;
			psx += nchan2;
			phist[data]++;
		}
	}

	for (; j < height; j++) {
		psx = ps;
		ps += stride_s;
		for (i = 0; i < m; i++) {
			data = *(mlib_u16 *)psx;
			psx += nchan2;
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
			psx += nchan2;
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
			psx += nchan2;
			phist[data]--;
		}
	}
}

/* *********************************************************** */

void
mlib_rank_row_MxN_bxh3w8(
    mlib_u8 *pd,
    mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_s32 nchan2,
    mlib_u8 *histogram)
{
	mlib_u8 *psx;
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
		ps += nchan2;
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
		ps += nchan2;
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
		pd += nchan2;

		psx = ps - nchan2 * m;
		for (i = 0; i < n; i++) {
			data = mask & (*(mlib_u16 *)psx);
			psx += stride_s;
			phist3[data >> shft2]--;
			phist2[data >> shft1]--;
			phist[data]--;
		}
	}

	ps -= nchan2 * (m - 1);
	for (j = 0; j < m - 1; j++) {
		psx = ps;
		ps += nchan2;
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
    mlib_u8 *ps,
    mlib_s32 height,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_d,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_s32 nchan2,
    mlib_u8 *histogram)
{
	mlib_u8 *psx;
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
			psx += nchan2;
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
			psx += nchan2;
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
			psx += nchan2;
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
			psx += nchan2;
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
    mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_s32 nchan2,
    mlib_u8 *histogram)
{
	mlib_u8 *psx;
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
		ps += nchan2;
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
		ps += nchan2;
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
		pd += nchan2;

		psx = ps - nchan2 * m;
		for (i = 0; i < n; i++) {
			data = mask & (*(mlib_u16 *)psx);
			psx += stride_s;
			phist3[data >> shft2]--;
			phist2[data >> shft1]--;
			phist[data]--;
		}
	}

	ps -= nchan2 * (m - 1);
	for (j = 0; j < m - 1; j++) {
		psx = ps;
		ps += nchan2;
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
    mlib_u8 *ps,
    mlib_s32 height,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_d,
    mlib_s32 stride_s,
    mlib_s32 bits,
    mlib_s32 nchan2,
    mlib_u8 *histogram)
{
	mlib_u8 *psx;
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
			psx += nchan2;
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
			psx += nchan2;
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
			psx += nchan2;
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
			psx += nchan2;
			phist3[data >> shft2]--;
			phist2[data >> shft1]--;
			phist[data]--;
		}
	}
}

/* *********************************************************** */

mlib_status
mlib_ImageRankFilterMxN_US_i(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 rank,
    mlib_s32 cmask,
    mlib_s32 bits)
{
	mlib_s32 width, height, nchan, nchan2, stride_s, stride_d;
	mlib_u8 *ps;
	mlib_u8 *pd;
	mlib_u8 *phist;
	mlib_s32 i, k, histSize;
	mlib_s32 b1, b2, b3;
	mlib_s32 hLevel = 0;
	mlib_s32 wmin, wmax;
	mlib_row_scan_US row_scan_func = NULL;
	mlib_col_scan_US col_scan_func = NULL;

	nchan = mlib_ImageGetChannels(dst);
	width = mlib_ImageGetWidth(dst);
	height = mlib_ImageGetHeight(dst);
	stride_d = mlib_ImageGetStride(dst);
	pd = (void *)mlib_ImageGetData(dst);

	stride_s = mlib_ImageGetStride(src);
	ps = mlib_ImageGetData(src);
	nchan2 = 2 * nchan;

	if ((m & 1) == 0 || m < 3 || (n & 1) == 0 || n < 3)
		return (MLIB_OUTOFRANGE);

	if (width < m || height < n)
		return (MLIB_SUCCESS);

	if (nchan == 1) {
		return (__mlib_ImageRankFilterMxN_US(dst, src, m, n, rank,
			bits));
	}

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
	case 15:
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

	pd += (m / 2) * nchan2 + (n / 2) * stride_d;

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
			for (k = 0; k < nchan; k++) {
				if (!(cmask & (1 << (nchan - 1 - k))))
					continue;
				row_scan_func(pd + 2 * k, ps + 2 * k, width,
				    rank, m, n, stride_s, bits, nchan2, phist);
			}

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
			for (k = 0; k < nchan; k++) {
				if (!(cmask & (1 << (nchan - 1 - k))))
					continue;
				col_scan_func(pd + 2 * k, ps + 2 * k, height,
				    rank, m, n, stride_d, stride_s, bits,
				    nchan2, phist);
			}

			pd += nchan2;
			ps += nchan2;
		}
	}

	__mlib_free(phist);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageProcessEdges(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 cmask,
    mlib_edge edge)
{
	mlib_filter_state fs[1];
	mlib_image edst[1], esrc[1];
	mlib_type type;
	mlib_s32 Left = m / 2, Right = m / 2, Top = n / 2, Bottom = n / 2;
	mlib_s32 slb, dlb, nchan, bchan;
	mlib_s32 src_wid, src_hgt, dst_wid, dst_hgt;
	mlib_s32 dx, dy, dxd, dxs, dyd, dys, dw, dh;
	mlib_s32 dx_l, dx_r, dy_t, dy_b;
	mlib_u8 *sa, *da;

	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, nchan, dst_wid, dst_hgt, dlb, da);
	MLIB_IMAGE_GET_ALL_PARAMS(src, type, nchan, src_wid, src_hgt, slb, sa);

	bchan = 2 * nchan;
	dw = (src_wid < dst_wid) ? src_wid : dst_wid;
	dh = (src_hgt < dst_hgt) ? src_hgt : dst_hgt;
	dx = (src_wid - dst_wid);
	dy = (src_hgt - dst_hgt);
	dxs = ((dx > 0) ? (dx + 1) >> 1 : 0);
	dxd = ((dx > 0) ? 0 : (-dx) >> 1);
	dys = ((dy > 0) ? (dy + 1) >> 1 : 0);
	dyd = ((dy > 0) ? 0 : (-dy) >> 1);

	dx_l = Left - dxs;
	dx_r = dxs - dx + Right;
	dy_t = Top - dys;
	dy_b = dys - dy + Bottom;
	dx_l = ((dx_l < 0) ? 0 : dx_l);
	dx_r = ((dx_r < 0) ? 0 : (dx_r > Right ? Right : dx_r));
	dy_t = ((dy_t < 0) ? 0 : dy_t);
	dy_b = ((dy_b < 0) ? 0 : (dy_b > Bottom ? Bottom : dy_b));

	fs->dx_l = dx_l;
	fs->dx_r = dx_r;
	fs->dy_t = dy_t;
	fs->dy_b = dy_b;
	fs->cmask = cmask;

	if (dx_l + dx_r + dy_t + dy_b == 0)
		edge = MLIB_EDGE_DST_NO_WRITE;

	mlib_ImageSet(edst, type, nchan, dw, dh, dlb,
	    (da + dlb * dyd + bchan * dxd));
	mlib_ImageSet(esrc, type, nchan, dw, dh, slb,
	    (sa + slb * dys + bchan * dxs));

	switch (edge) {
	case MLIB_EDGE_DST_FILL_ZERO:
		return (mlib_ImageFilterClearEdge(edst, fs));
	case MLIB_EDGE_DST_COPY_SRC:
		return (mlib_ImageFilterCopyEdge(edst, esrc, fs));
	default:
		return (MLIB_SUCCESS);
	}
}

/* *********************************************************** */

mlib_status
__mlib_ImageMedianFilterMxN_US(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 m,
    mlib_s32 n,
    mlib_median_mask mmask,
    mlib_s32 cmask,
    mlib_edge edge,
    mlib_s32 bits)
{
	mlib_status res;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(dst, src);
	MLIB_IMAGE_CHAN_EQUAL(dst, src);

	MLIB_IMAGE_HAVE_TYPE(dst, MLIB_SHORT);

	if (bits < 9 || bits > 15)
		return (MLIB_OUTOFRANGE);

	if (mmask != MLIB_MEDIAN_MASK_RECT || edge == MLIB_EDGE_SRC_EXTEND ||
	    mlib_ImageGetWidth(dst) != mlib_ImageGetWidth(src) ||
	    mlib_ImageGetHeight(dst) != mlib_ImageGetHeight(src)) {

		if (m == 5 && n == 5) {
			return __mlib_ImageMedianFilter5x5(dst, src, mmask,
			    cmask, edge);
		}

		return __mlib_ImageMedianFilterMxN(dst, src, m, n, mmask, cmask,
		    edge);
	}

	res =
	    mlib_ImageRankFilterMxN_US_i(dst, src, m, n, (m * n) / 2, cmask,
	    bits);

	if (res != MLIB_SUCCESS)
		return (res);

	switch (edge) {
	case MLIB_EDGE_DST_NO_WRITE:
		return (MLIB_SUCCESS);
	case MLIB_EDGE_DST_FILL_ZERO:
		return (mlib_ImageProcessEdges(dst, src, m, n, cmask, edge));
	case MLIB_EDGE_DST_COPY_SRC:
		return (mlib_ImageProcessEdges(dst, src, m, n, cmask, edge));
	default:
/* MLIB_FAILURE; */
		return (MLIB_SUCCESS);
	}
}

/* *********************************************************** */

mlib_status
__mlib_ImageMedianFilter3x3_US(
    mlib_image *dst,
    const mlib_image *src,
    mlib_median_mask mmask,
    mlib_s32 cmask,
    mlib_edge edge,
    mlib_s32 bits)
{
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_HAVE_TYPE(dst, MLIB_SHORT);

	if (bits < 9 || bits > 15)
		return (MLIB_OUTOFRANGE);

	return (__mlib_ImageMedianFilter3x3(dst, src, mmask, cmask, edge));
}

/* *********************************************************** */

mlib_status
__mlib_ImageMedianFilter5x5_US(
    mlib_image *dst,
    const mlib_image *src,
    mlib_median_mask mmask,
    mlib_s32 cmask,
    mlib_edge edge,
    mlib_s32 bits)
{
	if (mmask == MLIB_MEDIAN_MASK_RECT && bits < 14) {
		return __mlib_ImageMedianFilterMxN_US(dst, src, 5, 5, mmask,
		    cmask, edge, bits);
	} else {
		MLIB_IMAGE_CHECK(dst);
		MLIB_IMAGE_HAVE_TYPE(dst, MLIB_SHORT);

		if (bits < 9 || bits > 15)
			return (MLIB_OUTOFRANGE);

		return __mlib_ImageMedianFilter5x5(dst, src, mmask, cmask,
		    edge);
	}
}

/* *********************************************************** */

mlib_status
__mlib_ImageMedianFilter7x7_US(
    mlib_image *dst,
    const mlib_image *src,
    mlib_median_mask mmask,
    mlib_s32 cmask,
    mlib_edge edge,
    mlib_s32 bits)
{
	if (mmask == MLIB_MEDIAN_MASK_RECT) {
		return __mlib_ImageMedianFilterMxN_US(dst, src, 7, 7, mmask,
		    cmask, edge, bits);
	} else {
		return __mlib_ImageMedianFilter7x7(dst, src, mmask, cmask,
		    edge);
	}
}

/* *********************************************************** */
