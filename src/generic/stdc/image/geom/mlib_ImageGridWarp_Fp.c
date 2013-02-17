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

#pragma ident	"@(#)mlib_ImageGridWarp_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *
 *      mlib_ImageGridWarp_Fp - grid-based image warp
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageGridWarp_Fp(mlib_image       *dst,
 *                                        const mlib_image *src,
 *                                        const mlib_f32   *xWarpPos,
 *                                        const mlib_f32   *yWarpPos,
 *                                        mlib_d64         postShiftX,
 *                                        mlib_d64         postShiftY,
 *                                        mlib_s32         xStart,
 *                                        mlib_s32         xStep,
 *                                        mlib_s32         xNumCells,
 *                                        mlib_s32         yStart,
 *                                        mlib_s32         yStep,
 *                                        mlib_s32         yNumCells,
 *                                        mlib_filter      filter,
 *                                        mlib_edge        edge)
 *
 * ARGUMENTS
 *
 *      dst       The pointer to destination image.
 *      src       The pointer to source image.
 *      xWarpPos  An float array of length (xNumCells + 1)*(yNumCells + 1)
 *                containing horizontal warp positions at the grid
 *                points, in row-major order.
 *      yWarpPos  An float array of length (xNumCells + 1)*(yNumCells + 1)
 *                containing vertical warp positions at the grid
 *                points, in row-major order.
 *      postShiftX        The source image displacement in X direction.
 *      postShiftY        The source image displacement in Y direction.
 *      xStart    The minimum X coordinate of the grid.
 *      xStep     The horizontal spacing between grid cells.
 *      xNumCells The number of grid cell columns.
 *      yStart    The minimum Y coordinate of the grid.
 *      yStep     The vertical spacing between grid cells.
 *      yNumCells The number of grid cell rows.
 *      filter    The type of resampling filter.
 *      edge      The type of edge condition.
 *
 * RESTRICTION
 *
 *  The images must have the same type, and the same number of channels.
 *  The images can have 1, 2, 3, or 4 channels.
 *  The images can be in MLIB_FLOAT or MLIB_DOUBLE data type.
 *  The two images might have different sizes.
 *  src image can not have width or height larger than 32767.
 *
 *  The image pixels are assumed to be centered at .5 coordinate points,
 *  e.g., the upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *  The resampling filter can be one of the following:
 *    MLIB_NEAREST
 *    MLIB_BILINEAR
 *    MLIB_BICUBIC
 *    MLIB_BICUBIC2
 *
 *  The edge condition can be one of the following:
 *    MLIB_EDGE_DST_NO_WRITE  (default)
 *    MLIB_EDGE_SRC_PADDED
 *
 * DESCRIPTION
 *
 *      A regular grid-based image warp.
 *
 *      For each pixel in the destination image, its center point D is
 *      backward mapped to a point S in the source image. Then the source
 *      pixels with their centers surrounding point S are selected to do
 *      one of the interpolations specified by the `filter' parameter to
 *      generate the pixel value for point D.
 *
 *      The mapping from destination pixels to source positions is described
 *      by bilinear interpolation between a rectilinear grid of points with
 *      known mappings.
 *
 *      Given a destination pixel coordinate (x, y) that lies within a cell
 *      having corners at (x0, y0), (x1, y0), (x0, y1) and (x1, y1), with
 *      source coordinates defined at each respective corner equal to
 *      (sx0, sy0), (sx1, sy1), (sx2, sy2) and (sx3, sy3), the source
 *      position (sx, sy) that maps onto (x, y) is given by the formulas:
 *
 *        xfrac = (x - x0)/(x1 - x0)
 *        yfrac = (y - y0)/(y1 - y0)
 *
 *        s = sx0 + (sx1 - sx0)*xfrac
 *        t = sy0 + (sy1 - sy0)*xfrac
 *
 *        u = sx2 + (sx3 - sx2)*xfrac
 *        v = sy2 + (sy3 - sy2)*xfrac
 *
 *        sx = s + (u - s)*yfrac - postShiftX
 *        sy = t + (v - t)*yfrac - postShiftY
 *
 *      In other words, the source x and y values are interpolated
 *      horizontally along the top and bottom edges of the grid cell, and
 *      the results are interpolated vertically:
 *
 *        (x0, y0) ->            (x1, y0) ->
 *          (sx0, sy0)             (sx1, sy1)
 *           +------------+---------+
 *           |            |\        |
 *           |            | (s, t)  |
 *           |            |         |
 *           |            |         |
 *           |            |         |
 *           |            |         |
 *           | (x, y) ->  |         |
 *           |  (sx, sy)--+         |
 *           |            |         |
 *           |            |         |
 *           |            | (u, v)  |
 *           |            |/        |
 *           +------------+---------+
 *        (x0, y1) ->          (x1, y1) ->
 *          (sx2, sy2)           (sx3, sy3)
 *
 *      The grid is defined by a set of equal-sized cells. The grid starts
 *      at (xStart, yStart). Each cell has width equal to xStep and height
 *      equal to yStep, and there are xNumCells cells horizontally and
 *      yNumCells cells vertically.
 *
 *      The degree of warping within each cell is defined by the values in
 *      xWarpPos and yWarpPos parameters. Each of these parameters must
 *      contain (xNumCells + 1)*(yNumCells + 1) values, which respectively
 *      contain the source X and source Y coordinates that map to the upper-left
 *      corner of each cell in the destination image. The cells are enumerated
 *      in row-major order, that is, all the grid points along a row are
 *      enumerated first, then the grid points for the next row are
 *      enumerated, and so on.
 *
 *      As an example, suppose xNumCells is equal to 2 and yNumCells is
 *      equal 1. Then the order of the data in the xWarpPos would be:
 *
 *        x00, x10, x20, x01, x11, x21
 *
 *      and in the yWarpPos:
 *
 *        y00, y10, y20, y01, y11, y21
 *
 *      for a total of (2 + 1)*(1 + 1) = 6 elements in each table.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageGridWarp.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageGridWarp_Fp = __mlib_ImageGridWarp_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageGridWarp_Fp) mlib_ImageGridWarp_Fp
    __attribute__((weak, alias("__mlib_ImageGridWarp_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageGridWarp_Fp(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_f32 *xWarpPos,
    const mlib_f32 *yWarpPos,
    mlib_d64 postShiftX,
    mlib_d64 postShiftY,
    mlib_s32 xStart,
    mlib_s32 xStep,
    mlib_s32 xNumCells,
    mlib_s32 yStart,
    mlib_s32 yStep,
    mlib_s32 yNumCells,
    mlib_filter filter,
    mlib_edge edge)
{
	mlib_type type;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);

	type = mlib_ImageGetType(dst);

	if (type != MLIB_FLOAT && type != MLIB_DOUBLE) {
		return (MLIB_FAILURE);
	}

	return mlib_ImageGridWarp_alltypes(dst, src, xWarpPos, yWarpPos,
	    postShiftX, postShiftY,
	    xStart, xStep, xNumCells, yStart, yStep, yNumCells, filter, edge);
}

/* *********************************************************** */
