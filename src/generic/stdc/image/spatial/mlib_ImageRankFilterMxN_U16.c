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

#pragma ident	"@(#)mlib_ImageRankFilterMxN_U16.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageRankFilterMxN   - performs rank filtering on an image.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageRankFilterMxN(mlib_image       *dst,
 *                                          const mlib_image *src,
 *                                          mlib_s32         m,
 *                                          mlib_s32         n,
 *                                          mlib_s32         rank)
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
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT type.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageRankFilterFunc.h>
#include <mlib_ImageRankFilter.h>

/* *********************************************************** */

#define	GET_DATA(psx)	(*(mlib_u16*)(psx))
#define	IND_CONST	1

/* *********************************************************** */

static void mlib_rank_row_MxN_u16w8h3(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_s,
    mlib_u8 *phist);

static void mlib_rank_row_MxN_u16w8h4(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_s,
    mlib_u8 *phist);

static void mlib_rank_col_MxN_u16w8h3(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 height,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_d,
    mlib_s32 stride_s,
    mlib_u8 *phist);

static void mlib_rank_col_MxN_u16w8h4(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 height,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_d,
    mlib_s32 stride_s,
    mlib_u8 *phist);

static void mlib_rank_row_MxN_u16w16h3(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_s,
    mlib_u16 *phist);

static void mlib_rank_row_MxN_u16w16h4(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_s,
    mlib_u16 *phist);

static void mlib_rank_col_MxN_u16w16h3(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 height,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_d,
    mlib_s32 stride_s,
    mlib_u16 *phist);

static void mlib_rank_col_MxN_u16w16h4(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 height,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_d,
    mlib_s32 stride_s,
    mlib_u16 *phist);

/* *********************************************************** */

mlib_status
mlib_ImageRankFilterMxN_U16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 rank,
    mlib_s32 stride_d,
    mlib_s32 stride_s)
{
	mlib_u8 *phist;
	mlib_s32 histSize = 0x10000 + 4096 + 256 + 16;
	mlib_u8 *pd;
	mlib_u8 *ps;
	mlib_s32 i;

	ps = (mlib_u8 *)src;
	pd = (mlib_u8 *)dst + (m / 2) * 2 + (n / 2) * stride_d;

	if (m * n > 256) {
		phist = (mlib_u8 *)__mlib_malloc(histSize * 2);

		if (phist == NULL)
			return (MLIB_FAILURE);

		mlib_fill_MxN_s32(phist, 0, histSize * 2);

		if (m < n) {
			if (m <= 9) {
				for (i = 0; i < width - m + 1; i++) {
					mlib_rank_col_MxN_u16w16h4(pd, ps,
					    height, rank, m, n, stride_d,
					    stride_s, (mlib_u16 *)phist);
					pd += 2;
					ps += 2;
				}
			} else {
				for (i = 0; i < width - m + 1; i++) {
					mlib_rank_col_MxN_u16w16h3(pd, ps,
					    height, rank, m, n, stride_d,
					    stride_s, (mlib_u16 *)phist);
					pd += 2;
					ps += 2;
				}
			}
		} else {

			if (n <= 9) {
				for (i = 0; i < height - n + 1; i++) {
					mlib_rank_row_MxN_u16w16h4(pd, ps,
					    width, rank, m, n, stride_s,
					    (mlib_u16 *)phist);
					pd += stride_d;
					ps += stride_s;
				}
			} else {
				for (i = 0; i < height - n + 1; i++) {
					mlib_rank_row_MxN_u16w16h3(pd, ps,
					    width, rank, m, n, stride_s,
					    (mlib_u16 *)phist);
					pd += stride_d;
					ps += stride_s;
				}
			}
		}
	} else {
		phist = (mlib_u8 *)__mlib_malloc(histSize);

		if (phist == NULL)
			return (MLIB_FAILURE);

		mlib_fill_MxN_s32(phist, 0, histSize);

		if (m < n) {
			if (m <= 9) {
				for (i = 0; i < width - m + 1; i++) {
					mlib_rank_col_MxN_u16w8h4(pd, ps,
					    height, rank, m, n, stride_d,
					    stride_s, phist);
					pd += 2;
					ps += 2;
				}
			} else {
				for (i = 0; i < width - m + 1; i++) {
					mlib_rank_col_MxN_u16w8h3(pd, ps,
					    height, rank, m, n, stride_d,
					    stride_s, phist);
					pd += 2;
					ps += 2;
				}
			}
		} else {

			if (n <= 9) {
				for (i = 0; i < height - n + 1; i++) {
					mlib_rank_row_MxN_u16w8h4(pd, ps, width,
					    rank, m, n, stride_s, phist);
					pd += stride_d;
					ps += stride_s;
				}
			} else {
				for (i = 0; i < height - n + 1; i++) {
					mlib_rank_row_MxN_u16w8h3(pd, ps, width,
					    rank, m, n, stride_s, phist);
					pd += stride_d;
					ps += stride_s;
				}
			}
		}
	}

	__mlib_free(phist);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_rank_row_MxN_u16w8h3(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_s,
    mlib_u8 *phist)
{
	mlib_u8 *phist1 = phist + 0x10000;
	mlib_u8 *phist2 = phist1 + 1024;
	const mlib_u8 *psx;
	mlib_s32 i, j, data, count, index;

	for (j = 0; j < m - 1; j++) {
		psx = ps;
		ps += 2;
		for (i = 0; i < n; i++) {
			data = GET_DATA(psx);
			psx += stride_s;
			phist2[data >> 11]++;
			phist1[data >> 6]++;
			phist[data]++;
		}
	}

	for (; j < width; j++) {
		psx = ps;
		ps += 2;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			data = GET_DATA(psx);
			psx += stride_s;
			phist2[data >> 11]++;
			phist1[data >> 6]++;
			phist[data]++;
		}

		for (count = index = 0; count < rank; index++)
			count += phist2[index];

		count -= phist2[--index];
		index = (index << 5);

		for (; count < rank; index++)
			count += phist1[index];

		count -= phist1[--index];
		index = (index << 6);

		for (; count < rank; index++)
			count += phist[index];

		*(mlib_u16 *)pd = index - IND_CONST;
		pd += 2;

		psx = ps - m * 2;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			data = GET_DATA(psx);
			psx += stride_s;
			phist2[data >> 11]--;
			phist1[data >> 6]--;
			phist[data]--;
		}
	}

	ps -= m * 2;
	for (j = 0; j < m - 1; j++) {
		ps += 2;
		psx = ps;
		for (i = 0; i < n; i++) {
			data = GET_DATA(psx);
			psx += stride_s;
			phist2[data >> 11]--;
			phist1[data >> 6]--;
			phist[data]--;
		}
	}
}

/* *********************************************************** */

void
mlib_rank_row_MxN_u16w8h4(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_s,
    mlib_u8 *phist)
{
	mlib_u8 *phist1 = phist + 0x10000;
	mlib_u8 *phist2 = phist1 + 4096;
	mlib_u8 *phist3 = phist2 + 256;
	const mlib_u8 *psx;
	mlib_s32 i, j, data, count, index;

	for (j = 0; j < m - 1; j++) {
		psx = ps;
		ps += 2;
		for (i = 0; i < n; i++) {
			data = GET_DATA(psx);
			psx += stride_s;
			phist3[data >> 12]++;
			phist2[data >> 8]++;
			phist1[data >> 4]++;
			phist[data]++;
		}
	}

	for (; j < width; j++) {
		psx = ps;
		ps += 2;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			data = GET_DATA(psx);
			psx += stride_s;
			phist3[data >> 12]++;
			phist2[data >> 8]++;
			phist1[data >> 4]++;
			phist[data]++;
		}

		for (count = index = 0; count < rank; index++)
			count += phist3[index];

		count -= phist3[--index];
		index = (index << 4);

		for (; count < rank; index++)
			count += phist2[index];

		count -= phist2[--index];
		index = (index << 4);

		for (; count < rank; index++)
			count += phist1[index];

		count -= phist1[--index];
		index = (index << 4);

		for (; count < rank; index++)
			count += phist[index];

		*(mlib_u16 *)pd = index - IND_CONST;
		pd += 2;

		psx = ps - m * 2;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			data = GET_DATA(psx);
			psx += stride_s;
			phist3[data >> 12]--;
			phist2[data >> 8]--;
			phist1[data >> 4]--;
			phist[data]--;
		}
	}

	ps -= m * 2;
	for (j = 0; j < m - 1; j++) {
		ps += 2;
		psx = ps;
		for (i = 0; i < n; i++) {
			data = GET_DATA(psx);
			psx += stride_s;
			phist3[data >> 12]--;
			phist2[data >> 8]--;
			phist1[data >> 4]--;
			phist[data]--;
		}
	}
}

/* *********************************************************** */

void
mlib_rank_col_MxN_u16w8h3(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 height,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_d,
    mlib_s32 stride_s,
    mlib_u8 *phist)
{
	mlib_u8 *phist1 = phist + 0x10000;
	mlib_u8 *phist2 = phist1 + 1024;
	const mlib_u8 *psx;
	mlib_s32 i, j, data, count, index;
	mlib_s32 wdiff = n * stride_s;

	for (j = 0; j < n - 1; j++) {
		psx = ps;
		ps += stride_s;
		for (i = 0; i < m; i++) {
			data = GET_DATA(psx);
			psx += 2;
			phist2[data >> 11]++;
			phist1[data >> 6]++;
			phist[data]++;
		}
	}

	for (; j < height; j++) {
		psx = ps;
		ps += stride_s;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < m; i++) {
			data = GET_DATA(psx);
			psx += 2;
			phist2[data >> 11]++;
			phist1[data >> 6]++;
			phist[data]++;
		}

		for (count = index = 0; count < rank; index++)
			count += phist2[index];

		count -= phist2[--index];
		index = (index << 5);

		for (; count < rank; index++)
			count += phist1[index];

		count -= phist1[--index];
		index = (index << 6);

		for (; count < rank; index++)
			count += phist[index];

		*(mlib_u16 *)pd = index - IND_CONST;
		pd += stride_d;

		psx = ps - wdiff;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < m; i++) {
			data = GET_DATA(psx);
			psx += 2;
			phist2[data >> 11]--;
			phist1[data >> 6]--;
			phist[data]--;
		}
	}

	ps -= wdiff;
	for (j = 0; j < n - 1; j++) {
		ps += stride_s;
		psx = ps;
		for (i = 0; i < m; i++) {
			data = GET_DATA(psx);
			psx += 2;
			phist2[data >> 11]--;
			phist1[data >> 6]--;
			phist[data]--;
		}
	}
}

/* *********************************************************** */

void
mlib_rank_col_MxN_u16w8h4(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 height,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_d,
    mlib_s32 stride_s,
    mlib_u8 *phist)
{
	mlib_u8 *phist1 = phist + 0x10000;
	mlib_u8 *phist2 = phist1 + 4096;
	mlib_u8 *phist3 = phist2 + 256;
	const mlib_u8 *psx;
	mlib_s32 i, j, data, count, index;
	mlib_s32 wdiff = n * stride_s;

	for (j = 0; j < n - 1; j++) {
		psx = ps;
		ps += stride_s;
		for (i = 0; i < m; i++) {
			data = GET_DATA(psx);
			psx += 2;
			phist3[data >> 12]++;
			phist2[data >> 8]++;
			phist1[data >> 4]++;
			phist[data]++;
		}
	}

	for (; j < height; j++) {
		psx = ps;
		ps += stride_s;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < m; i++) {
			data = GET_DATA(psx);
			psx += 2;
			phist3[data >> 12]++;
			phist2[data >> 8]++;
			phist1[data >> 4]++;
			phist[data]++;
		}

		for (count = index = 0; count < rank; index++)
			count += phist3[index];

		count -= phist3[--index];
		index = (index << 4);

		for (; count < rank; index++)
			count += phist2[index];

		count -= phist2[--index];
		index = (index << 4);

		for (; count < rank; index++)
			count += phist1[index];

		count -= phist1[--index];
		index = (index << 4);

		for (; count < rank; index++)
			count += phist[index];

		*(mlib_u16 *)pd = index - IND_CONST;
		pd += stride_d;

		psx = ps - wdiff;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < m; i++) {
			data = GET_DATA(psx);
			psx += 2;
			phist3[data >> 12]--;
			phist2[data >> 8]--;
			phist1[data >> 4]--;
			phist[data]--;
		}
	}

	ps -= wdiff;
	for (j = 0; j < n - 1; j++) {
		ps += stride_s;
		psx = ps;
		for (i = 0; i < m; i++) {
			data = GET_DATA(psx);
			psx += 2;
			phist3[data >> 12]--;
			phist2[data >> 8]--;
			phist1[data >> 4]--;
			phist[data]--;
		}
	}
}

/* *********************************************************** */

void
mlib_rank_row_MxN_u16w16h3(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_s,
    mlib_u16 *phist)
{
	mlib_u16 *phist1 = phist + 0x10000;
	mlib_u16 *phist2 = phist1 + 1024;
	const mlib_u8 *psx;
	mlib_s32 i, j, data, count, index;

	for (j = 0; j < m - 1; j++) {
		psx = ps;
		ps += 2;
		for (i = 0; i < n; i++) {
			data = GET_DATA(psx);
			psx += stride_s;
			phist2[data >> 11]++;
			phist1[data >> 6]++;
			phist[data]++;
		}
	}

	for (; j < width; j++) {
		psx = ps;
		ps += 2;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			data = GET_DATA(psx);
			psx += stride_s;
			phist2[data >> 11]++;
			phist1[data >> 6]++;
			phist[data]++;
		}

		for (count = index = 0; count < rank; index++)
			count += phist2[index];

		count -= phist2[--index];
		index = (index << 5);

		for (; count < rank; index++)
			count += phist1[index];

		count -= phist1[--index];
		index = (index << 6);

		for (; count < rank; index++)
			count += phist[index];

		*(mlib_u16 *)pd = index - IND_CONST;
		pd += 2;

		psx = ps - m * 2;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			data = GET_DATA(psx);
			psx += stride_s;
			phist2[data >> 11]--;
			phist1[data >> 6]--;
			phist[data]--;
		}
	}

	ps -= m * 2;
	for (j = 0; j < m - 1; j++) {
		ps += 2;
		psx = ps;
		for (i = 0; i < n; i++) {
			data = GET_DATA(psx);
			psx += stride_s;
			phist2[data >> 11]--;
			phist1[data >> 6]--;
			phist[data]--;
		}
	}
}

/* *********************************************************** */

void
mlib_rank_row_MxN_u16w16h4(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_s,
    mlib_u16 *phist)
{
	mlib_u16 *phist1 = phist + 0x10000;
	mlib_u16 *phist2 = phist1 + 4096;
	mlib_u16 *phist3 = phist2 + 256;
	const mlib_u8 *psx;
	mlib_s32 i, j, data, count, index;

	for (j = 0; j < m - 1; j++) {
		psx = ps;
		ps += 2;
		for (i = 0; i < n; i++) {
			data = GET_DATA(psx);
			psx += stride_s;
			phist3[data >> 12]++;
			phist2[data >> 8]++;
			phist1[data >> 4]++;
			phist[data]++;
		}
	}

	for (; j < width; j++) {
		psx = ps;
		ps += 2;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			data = GET_DATA(psx);
			psx += stride_s;
			phist3[data >> 12]++;
			phist2[data >> 8]++;
			phist1[data >> 4]++;
			phist[data]++;
		}

		for (count = index = 0; count < rank; index++)
			count += phist3[index];

		count -= phist3[--index];
		index = (index << 4);

		for (; count < rank; index++)
			count += phist2[index];

		count -= phist2[--index];
		index = (index << 4);

		for (; count < rank; index++)
			count += phist1[index];

		count -= phist1[--index];
		index = (index << 4);

		for (; count < rank; index++)
			count += phist[index];

		*(mlib_u16 *)pd = index - IND_CONST;
		pd += 2;

		psx = ps - m * 2;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			data = GET_DATA(psx);
			psx += stride_s;
			phist3[data >> 12]--;
			phist2[data >> 8]--;
			phist1[data >> 4]--;
			phist[data]--;
		}
	}

	ps -= m * 2;
	for (j = 0; j < m - 1; j++) {
		ps += 2;
		psx = ps;
		for (i = 0; i < n; i++) {
			data = GET_DATA(psx);
			psx += stride_s;
			phist3[data >> 12]--;
			phist2[data >> 8]--;
			phist1[data >> 4]--;
			phist[data]--;
		}
	}
}

/* *********************************************************** */

void
mlib_rank_col_MxN_u16w16h3(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 height,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_d,
    mlib_s32 stride_s,
    mlib_u16 *phist)
{
	mlib_u16 *phist1 = phist + 0x10000;
	mlib_u16 *phist2 = phist1 + 1024;
	const mlib_u8 *psx;
	mlib_s32 i, j, data, count, index;
	mlib_s32 wdiff = n * stride_s;

	for (j = 0; j < n - 1; j++) {
		psx = ps;
		ps += stride_s;
		for (i = 0; i < m; i++) {
			data = GET_DATA(psx);
			psx += 2;
			phist2[data >> 11]++;
			phist1[data >> 6]++;
			phist[data]++;
		}
	}

	for (; j < height; j++) {
		psx = ps;
		ps += stride_s;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < m; i++) {
			data = GET_DATA(psx);
			psx += 2;
			phist2[data >> 11]++;
			phist1[data >> 6]++;
			phist[data]++;
		}

		for (count = index = 0; count < rank; index++)
			count += phist2[index];

		count -= phist2[--index];
		index = (index << 5);

		for (; count < rank; index++)
			count += phist1[index];

		count -= phist1[--index];
		index = (index << 6);

		for (; count < rank; index++)
			count += phist[index];

		*(mlib_u16 *)pd = index - IND_CONST;
		pd += stride_d;

		psx = ps - wdiff;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < m; i++) {
			data = GET_DATA(psx);
			psx += 2;
			phist2[data >> 11]--;
			phist1[data >> 6]--;
			phist[data]--;
		}
	}

	ps -= wdiff;
	for (j = 0; j < n - 1; j++) {
		ps += stride_s;
		psx = ps;
		for (i = 0; i < m; i++) {
			data = GET_DATA(psx);
			psx += 2;
			phist2[data >> 11]--;
			phist1[data >> 6]--;
			phist[data]--;
		}
	}
}

/* *********************************************************** */

void
mlib_rank_col_MxN_u16w16h4(
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 height,
    mlib_s32 rank,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 stride_d,
    mlib_s32 stride_s,
    mlib_u16 *phist)
{
	mlib_u16 *phist1 = phist + 0x10000;
	mlib_u16 *phist2 = phist1 + 4096;
	mlib_u16 *phist3 = phist2 + 256;
	const mlib_u8 *psx;
	mlib_s32 i, j, data, count, index;
	mlib_s32 wdiff = n * stride_s;

	for (j = 0; j < n - 1; j++) {
		psx = ps;
		ps += stride_s;
		for (i = 0; i < m; i++) {
			data = GET_DATA(psx);
			psx += 2;
			phist3[data >> 12]++;
			phist2[data >> 8]++;
			phist1[data >> 4]++;
			phist[data]++;
		}
	}

	for (; j < height; j++) {
		psx = ps;
		ps += stride_s;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < m; i++) {
			data = GET_DATA(psx);
			psx += 2;
			phist3[data >> 12]++;
			phist2[data >> 8]++;
			phist1[data >> 4]++;
			phist[data]++;
		}

		for (count = index = 0; count < rank; index++)
			count += phist3[index];

		count -= phist3[--index];
		index = (index << 4);

		for (; count < rank; index++)
			count += phist2[index];

		count -= phist2[--index];
		index = (index << 4);

		for (; count < rank; index++)
			count += phist1[index];

		count -= phist1[--index];
		index = (index << 4);

		for (; count < rank; index++)
			count += phist[index];

		*(mlib_u16 *)pd = index - IND_CONST;
		pd += stride_d;

		psx = ps - wdiff;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < m; i++) {
			data = GET_DATA(psx);
			psx += 2;
			phist3[data >> 12]--;
			phist2[data >> 8]--;
			phist1[data >> 4]--;
			phist[data]--;
		}
	}

	ps -= wdiff;
	for (j = 0; j < n - 1; j++) {
		ps += stride_s;
		psx = ps;
		for (i = 0; i < m; i++) {
			data = GET_DATA(psx);
			psx += 2;
			phist3[data >> 12]--;
			phist2[data >> 8]--;
			phist1[data >> 4]--;
			phist[data]--;
		}
	}
}

/* *********************************************************** */
