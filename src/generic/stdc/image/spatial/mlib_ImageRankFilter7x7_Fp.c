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

#pragma ident	"@(#)mlib_ImageRankFilter7x7_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageRankFilter7x7_Fp   - performs rank filtering on an image.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageRankFilter7x7_Fp(mlib_image       *dst,
 *                                             const mlib_image *src,
 *                                             mlib_s32         rank)
 *
 * ARGUMENT
 *      dst     pointer to structure of destination image,
 *      src     pointer to structure of source image,
 *      rank    the rank of the output pixel. rank value must be from 0 to 48.
 *              The pixel with minimum value is designated rank 0 and
 *              the pixel with maximum value is designated rank 48.
 *
 * DESCRIPTION
 *      X  - x(i, j), (i = 0 ... w - 1), (j = 0 ... h - 1)
 *      Z  - z(i, j), (i = 3 ... w - 4), (j = 3 ... h - 4)
 *      z(i, j) = rank value in a sorted array of 49 source image pixels -
 *                { x(p, q), (i - 3 <= p <= i + 3), (j - 3 <= q <= j + 3) }.
 *      so if rank equals 0 - min value, rank equals 49 - max value.
 *      if rank equals 24 - median value.
 *      The input and output images must be single channel images and have
 *      the same width, height and image type.
 *      They can be in MLIB_FLOAT, MLIB_DOUBLE type.
 */

#include <mlib_image.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageRankFilter7x7_Fp = __mlib_ImageRankFilter7x7_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageRankFilter7x7_Fp) mlib_ImageRankFilter7x7_Fp
    __attribute__((weak, alias("__mlib_ImageRankFilter7x7_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageRankFilter7x7_Fp(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 rank)
{
	if (rank == 0)
		return (__mlib_ImageMinFilter7x7_Fp(dst, src));

	if (rank == 48)
		return (__mlib_ImageMaxFilter7x7_Fp(dst, src));

	return (__mlib_ImageRankFilterMxN_Fp(dst, src, 7, 7, rank));
}

/* *********************************************************** */
