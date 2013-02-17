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

#pragma ident	"@(#)mlib_ImageRankFilterMxN_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageRankFilterMxN_Fp   - performs rank filtering on an image.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageRankFilterMxN_Fp(mlib_image       *dst,
 *                                             const mlib_image *src,
 *                                             mlib_s32         m,
 *                                             mlib_s32         n,
 *                                             mlib_s32         rank)
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
 *      They can be in MLIB_FLOAT, MLIB_DOUBLE type.
 */

#include <mlib_image.h>
#include <mlib_ImageMedianFilter.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageRankFilterMxN_Fp = __mlib_ImageRankFilterMxN_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageRankFilterMxN_Fp) mlib_ImageRankFilterMxN_Fp
    __attribute__((weak, alias("__mlib_ImageRankFilterMxN_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageRankFilterMxN_Fp(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 rank)
{
	if (rank < 0 || rank >= m * n)
		return (MLIB_OUTOFRANGE);

	return mlib_ImageMedianFilterMxN_alltypes(dst, src, m, n,
	    MLIB_MEDIAN_MASK_RECT, 0xF, MLIB_EDGE_DST_NO_WRITE, 1, rank + 1);
}

/* *********************************************************** */
