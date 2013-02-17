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

#pragma ident	"@(#)mlib_ImageAffineIndex.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageAffineIndex - indexed color image affine transformation.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageAffineIndex(mlib_image       *dst,
 *                                        const mlib_image *src,
 *                                        const mlib_d64   *mtx,
 *                                        mlib_filter      filter,
 *                                        mlib_edge        edge,
 *                                        const void       *colormap)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      mtx       Transformation matrix, where
 *                mtx[0] holds a;  mtx[1] holds b;
 *                mtx[2] holds tx; mtx[3] holds c;
 *                mtx[4] holds d;  mtx[5] holds ty.
 *      filter    Type of resampling filter.
 *      edge      Type of edge conditions.
 *      colormap  Pointer to the colormap structure.
 *
 * DESCRIPTION
 *                      xd = a*xs + b*ys + tx
 *                      yd = c*xs + d*ys + ty
 *
 *  The upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *  The edge condition can be one of the following:
 *    MLIB_EDGE_DST_NO_WRITE  (default)
 *    MLIB_EDGE_DST_FILL_ZERO
 *    MLIB_EDGE_OP_NEAREST
 *    MLIB_EDGE_SRC_EXTEND
 *    MLIB_EDGE_SRC_PADDED
 *
 * RESTRICTION
 *      src and dst must be the same type and have just one channel.
 *      They can be in MLIB_BYTE or MLIB_SHORT data type.
 *
 *      src image can not have width or height larger than 32767.
 */

#include <mlib_image.h>
#include <mlib_ImageAffine.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageAffineIndex = __mlib_ImageAffineIndex

#elif defined(__GNUC__)	/* defined( __SUNPRO_C ) */
__typeof__(__mlib_ImageAffineIndex) mlib_ImageAffineIndex
	__attribute__((weak, alias("__mlib_ImageAffineIndex")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageAffineIndex(
	mlib_image *dst,
	const mlib_image *src,
	const mlib_d64 *mtx,
	mlib_filter filter,
	mlib_edge edge,
	const void *colormap)
{
/*
 * check colormap
 */

	if (colormap == NULL)
		return (MLIB_NULLPOINTER);

	return (mlib_ImageAffine_alltypes(dst, src, mtx, filter,
					edge, colormap));
}

/* *********************************************************** */
