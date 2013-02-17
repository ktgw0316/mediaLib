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

#pragma ident	"@(#)mlib_ImageGridWarp.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *
 *      mlib_ImageGridWarp    - grid-based image warp
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageGridWarp(mlib_image       *dst,
 *                                     const mlib_image *src,
 *                                     const mlib_f32   *xWarpPos,
 *                                     const mlib_f32   *yWarpPos,
 *                                     mlib_d64         postShiftX,
 *                                     mlib_d64         postShiftY,
 *                                     mlib_s32         xStart,
 *                                     mlib_s32         xStep,
 *                                     mlib_s32         xNumCells,
 *                                     mlib_s32         yStart,
 *                                     mlib_s32         yStep,
 *                                     mlib_s32         yNumCells,
 *                                     mlib_filter      filter,
 *                                     mlib_edge        edge)
 *
 * ARGUMENTS
 *
 *      dst       The pointer to destination image.
 *      src       The pointer to source image.
 *      xWarpPos  An mlib_f32 array of length (xNumCells + 1)*(yNumCells + 1)
 *                containing horizontal warp positions at the grid
 *                points, in row-major order.
 *      yWarpPos  An mlib_f32 array of length (xNumCells + 1)*(yNumCells + 1)
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
 *  The images can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT
 *  data type.
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

#include <mlib_ImageCheck.h>
#include <mlib_ImageAffine.h>
#include <mlib_ImageGridWarp.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageGridWarp = __mlib_ImageGridWarp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageGridWarp) mlib_ImageGridWarp
    __attribute__((weak, alias("__mlib_ImageGridWarp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	BUFF_SIZE	600

/* *********************************************************** */

#define	IS_POINT_OUT_OF_SRC_NN(px, py, is_clip)                             \
	if ((px >= minX) && (px < maxX) && (py >= minY) && (py < maxY)) {   \
	    is_clip = 0;                                                    \
	} else                                                              \
	    is_clip = -1

/* *********************************************************** */

#define	GET_GRID_X(x, y)	((xWarpPos[((xNumCells+1)*(y) + (x))]) - \
				postShiftX)
#define	GET_GRID_Y(x, y)	((yWarpPos[((xNumCells+1)*(y) + (x))]) - \
				postShiftY)

#define	DEFINE_COEF(a, b, w)	(((b) - (a)) * (w))

/* *********************************************************** */

#define	INIT_GRID_ROW(x, y)                                     \
	{                                                       \
	    px1 = GET_GRID_X((x), (y));                         \
	    py1 = GET_GRID_Y((x), (y));                         \
	    px3 = GET_GRID_X((x), (y) + 1);                     \
	    py3 = GET_GRID_Y((x), (y) + 1);                     \
	    dx1 = DEFINE_COEF(px1, px3, yStep1);                \
	    dy1 = DEFINE_COEF(py1, py3, yStep1);                \
	}

/* *********************************************************** */

#define	INCR_GRID_ROW(x, y, xskip, yskip)                          \
	{                                                          \
	    px0 = px1;                                             \
	    py0 = py1;                                             \
	    px1 = GET_GRID_X((x) + 1, (y));                        \
	    py1 = GET_GRID_Y((x) + 1, (y));                        \
	    px3 = GET_GRID_X((x) + 1, (y) + 1);                    \
	    py3 = GET_GRID_Y((x) + 1, (y) + 1);                    \
	    dy0 = dy1;                                             \
	    dx0 = dx1;                                             \
	    dx1 = DEFINE_COEF(px1, px3, yStep1);                   \
	    dy1 = DEFINE_COEF(py1, py3, yStep1);                   \
	    dx = DEFINE_COEF(px0, px1, xStep1);                    \
	    dy = DEFINE_COEF(py0, py1, xStep1);                    \
	    delta_x = DEFINE_COEF(dx0, dx1, xStep1);               \
	    delta_y = DEFINE_COEF(dy0, dy1, xStep1);               \
	    cx0 = px0 + (yskip) * dx0;                             \
	    cy0 = py0 + (yskip) * dy0;                             \
	    dx = dx + delta_x * (yskip);                           \
	    dy = dy + delta_y * (yskip);                           \
	    cx0 = cx0 + dx * (xskip);                              \
	    cy0 = cy0 + dy * (xskip);                              \
	    dx0 = dx0 + delta_x * (xskip);                         \
	    dy0 = dy0 + delta_y * (xskip);                         \
	    cx1 = cx0 + dx * xNum;                                 \
	    cy1 = cy0 + dy * xNum;                                 \
	    cx2 = cx0 + dx0 * yNum;                                \
	    cy2 = cy0 + dy0 * yNum;                                \
	    cx3 = cx2 + (dx + delta_x * yNum) * xNum;              \
	    cy3 = cy2 + (dy + delta_y * yNum) * xNum;              \
	    IS_POINT_OUT_OF_SRC_NN(cx0, cy0, is_clip1);            \
	    IS_POINT_OUT_OF_SRC_NN(cx1, cy1, is_clip2);            \
	    IS_POINT_OUT_OF_SRC_NN(cx2, cy2, is_clip3);            \
	    IS_POINT_OUT_OF_SRC_NN(cx3, cy3, is_clip4);            \
	    is_clip = is_clip1 | is_clip2 | is_clip3 | is_clip4;   \
	}

/* *********************************************************** */

#define	IS_CELL_CLIP() (is_clip < 0)

/* *********************************************************** */

#define	MLIB_CLIP(DENOM, NUM)                                   \
	denom = (DENOM);                                        \
	num = (NUM);                                            \
	t = num * denom;                                        \
	if (denom > 0) {                                        \
	    if (t > tE)                                         \
		tE = t;                                         \
	} else if (denom < 0) {                                 \
	    if (t < tL)                                         \
		tL = t;                                         \
	}

/* *********************************************************** */

mlib_status
mlib_ImageGridWarp_alltypes(
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
	mlib_affine_param param[1];
	mlib_status res = MLIB_SUCCESS;
	mlib_type type;
	mlib_s32 nchan;
	mlib_s32 srcWidth, dstWidth, srcHeight, dstHeight;
	mlib_s32 srcYStride, dstYStride;
	mlib_s32 *warp_tbl, sArr[5 + 2 * BUFF_SIZE], t_ind = 0;
	mlib_s32 *leftEdges, *rightEdges, *xStarts, *yStarts;
	mlib_s32 leArr[BUFF_SIZE], reArr[BUFF_SIZE], xsArr[BUFF_SIZE],
	    ysArr[BUFF_SIZE];
	mlib_u8 *laArr[BUFF_SIZE + 4], **lineAddr, *memBuffer = NULL;
	mlib_u8 *srcLinePtr;
	mlib_u8 *srcData, *dstData_beg, *dstData;
	mlib_u8 *paddings;
	mlib_s32 align;
	mlib_s32 xFirst, xLast, xSkip, xRest;
	mlib_s32 yFirst, yLast, ySkip, yRest;
	mlib_d64 minX, minY, maxX, maxY;
	mlib_s32 xBeg, xEnd;
	mlib_s32 yBeg, yEnd;
	mlib_s32 yskip, xskip, yrest, xrest;
	mlib_s32 x, y, cx, cy;
	mlib_d64 px0, py0, px1, py1, px3, py3;
	mlib_d64 dx0, dx1, dy0, dy1, cx0, cx1, cy0, cy1, dx, dy, cx2, cy2, cx3,
	    cy3;
	mlib_d64 delta_x, delta_y;
	mlib_d64 xs, ys;
	mlib_d64 xNum, yNum;
	mlib_d64 xStep1 = 1.0 / xStep, yStep1 = 1.0 / yStep;
	mlib_s32 is_clip, is_clip1, is_clip2, is_clip3, is_clip4;
	mlib_d64 tE, tL, tmp_x0, tmp_y0, tmp_dx, tmp_dy, num, denom, t;
	mlib_d64 d_rdx, d_rdy, x0, y0, x1, y1, offset;
	mlib_s32 shift_left, shift_right, max_xsize;
	mlib_s32 i;

/* check for obvious errors */
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);

	if (xNumCells == 0 || yNumCells == 0)
		return (MLIB_SUCCESS);

	if (xWarpPos == NULL || yWarpPos == NULL)
		return (MLIB_FAILURE);

	if (edge != MLIB_EDGE_DST_NO_WRITE && edge != MLIB_EDGE_SRC_PADDED)
		return (MLIB_FAILURE);

	if (xNumCells < 0 || yNumCells <= 0 || xStep <= 0 || yStep <= 0)
		return (MLIB_FAILURE);

	srcData = mlib_ImageGetData(src);
	dstData_beg = mlib_ImageGetData(dst);
	type = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(dst);
	srcWidth = mlib_ImageGetWidth(src);
	srcHeight = mlib_ImageGetHeight(src);
	dstWidth = mlib_ImageGetWidth(dst);
	dstHeight = mlib_ImageGetHeight(dst);
	srcYStride = mlib_ImageGetStride(src);
	dstYStride = mlib_ImageGetStride(dst);
	paddings = mlib_ImageGetPaddings(src);

	if (srcWidth >= (1 << 15) || srcHeight >= (1 << 15)) {
		return (MLIB_FAILURE);
	}

	if (xStart >= dstWidth ||
	    ((xStart + xStep * xNumCells) <= 0) ||
	    yStart >= dstHeight || ((yStart + yStep * yNumCells) <= 0))
		return (MLIB_SUCCESS);

	if (srcHeight < BUFF_SIZE && dstHeight < BUFF_SIZE) {
		lineAddr = laArr;
		leftEdges = leArr;
		rightEdges = reArr;
		xStarts = xsArr;
		yStarts = ysArr;
		warp_tbl = sArr;
	} else {
		memBuffer =
		    __mlib_malloc((6 * dstHeight + 8) * sizeof (mlib_s32) +
		    (srcHeight + 4) * sizeof (mlib_u8 *));

		if (memBuffer == NULL)
			return (MLIB_FAILURE);
		leftEdges = (mlib_s32 *)(memBuffer);
		rightEdges =
		    (mlib_s32 *)(memBuffer + dstHeight * sizeof (mlib_s32));
		xStarts =
		    (mlib_s32 *)(memBuffer + 2 * dstHeight * sizeof (mlib_s32));
		yStarts =
		    (mlib_s32 *)(memBuffer + 3 * dstHeight * sizeof (mlib_s32));
		warp_tbl =
		    (mlib_s32 *)(memBuffer + 4 * dstHeight * sizeof (mlib_s32));
		lineAddr =
		    (mlib_u8 **)(memBuffer + (6 * dstHeight +
		    8) * sizeof (mlib_s32));
	}

	for (xFirst = 0; ; xFirst++) {
		if ((xStart + xFirst * xStep + xStep - 1) >= 0)
			break;
	}

	for (yFirst = 0; ; yFirst++) {
		if ((yStart + yFirst * yStep + yStep - 1) >= 0)
			break;
	}

	for (xLast = xNumCells - 1; xLast >= xFirst; xLast--) {
		if ((xStart + xLast * xStep) <= (dstWidth - 1))
			break;
	}

	for (yLast = yNumCells - 1; yLast >= yFirst; yLast--) {
		if ((yStart + yLast * yStep) <= (dstHeight - 1))
			break;
	}

	if ((xStart + xFirst * xStep) < 0)
		xSkip = -(xStart + xFirst * xStep);
	else
		xSkip = 0;

	if ((yStart + yFirst * yStep) < 0)
		ySkip = -(yStart + yFirst * yStep);
	else
		ySkip = 0;

	if ((xStart + xLast * xStep + xStep) > (dstWidth))
		xRest = (xStart + xLast * xStep + xStep) - (dstWidth);
	else
		xRest = 0;

	if ((yStart + yLast * yStep + yStep) > (dstHeight))
		yRest = (yStart + yLast * yStep + yStep) - (dstHeight);
	else
		yRest = 0;

	srcLinePtr = (mlib_u8 *)srcData;
	lineAddr += 2;
	for (i = -2; i < srcHeight + 2; i++) {
		lineAddr[i] = srcLinePtr + i * srcYStride;
	}

	if (type == MLIB_BYTE)
		t_ind = 0;
	else if (type == MLIB_SHORT)
		t_ind = 1;
	else if (type == MLIB_INT)
		t_ind = 2;
	else if (type == MLIB_USHORT)
		t_ind = 3;
	else if (type == MLIB_FLOAT)
		t_ind = 4;
	else if (type == MLIB_DOUBLE)
		t_ind = 5;

	if (filter == MLIB_NEAREST) {
		if (t_ind >= 3)
/* correct types USHORT, FLOAT, DOUBLE; new values: 1, 2, 3 */
			t_ind -= 2;

/* two channels as one channel of next type */
		align =
		    (mlib_s32)dstData_beg | (mlib_s32)srcData | dstYStride |
		    srcYStride;
#ifndef _NO_LONGLONG	/* do not perform the copying by 64bit data type */
		while (((nchan | (align >> t_ind)) & 1) == 0 && t_ind < 3)
#else /* do not perform the copying by 64bit data type */
		while (((nchan | (align >> t_ind)) & 1) == 0 && t_ind < 2)
#endif /* do not perform the copying by 64bit data type */
		{
			nchan >>= 1;
			t_ind++;
		}
	}

	switch (filter) {
	case MLIB_NEAREST:
		minX = 0;
		minY = 0;
		maxX = srcWidth;
		maxY = srcHeight;
		offset = 0;
		break;

	case MLIB_BILINEAR:
		minX = 0.5;
		minY = 0.5;
		maxX = srcWidth - 0.5;
		maxY = srcHeight - 0.5;
		offset = 0.5;
		break;

	case MLIB_BICUBIC:
	case MLIB_BICUBIC2:
		minX = 1.5;
		minY = 1.5;
		maxX = srcWidth - 1.5;
		maxY = srcHeight - 1.5;
		offset = 0.5;
		break;

	default:

		if (memBuffer != NULL) {
			__mlib_free(memBuffer);
		}

		return (MLIB_FAILURE);
	}

	if (edge == MLIB_EDGE_SRC_PADDED) {
		if (minX < paddings[0])
			minX = paddings[0];

		if (minY < paddings[1])
			minY = paddings[1];

		if (maxX > (srcWidth - paddings[2]))
			maxX = srcWidth - paddings[2];

		if (maxY > (srcHeight - paddings[3]))
			maxY = srcHeight - paddings[3];
	}

	if ((minX >= maxX) || (minY >= maxY)) {
		if (memBuffer != NULL) {
			__mlib_free(memBuffer);
		}

		return (MLIB_SUCCESS);
	}

/*
 *   STORE_PARAM(param, src);
 */
	param->src = (void *)src;

	STORE_PARAM(param, dst);
	STORE_PARAM(param, lineAddr);
	STORE_PARAM(param, leftEdges);
	STORE_PARAM(param, rightEdges);
	STORE_PARAM(param, xStarts);
	STORE_PARAM(param, yStarts);
	STORE_PARAM(param, srcYStride);
	STORE_PARAM(param, dstYStride);
	STORE_PARAM(param, warp_tbl);
	STORE_PARAM(param, filter);

	for (y = yFirst, cy = yStart + yFirst * yStep; y <= yLast;
	    y++, cy += yStep) {
		yskip = (y == yFirst) ? ySkip : 0;
		yrest = (y == yLast) ? yRest : 0;
		yBeg = cy + yskip;
		yEnd = cy + (yStep - 1) - yrest;
		yNum = yEnd - yBeg;
		ys = yskip + 0.5;
		INIT_GRID_ROW(xFirst, y);
		for (x = xFirst, cx = xStart + xFirst * xStep; x <= xLast;
		    x++, cx += xStep) {
			xskip = (x == xFirst) ? xSkip : 0;
			xrest = (x == xLast) ? xRest : 0;
			xBeg = cx + xskip;
			xEnd = cx + (xStep - 1) - xrest;
			xNum = xEnd - xBeg;
			xs = xskip + 0.5;
			INCR_GRID_ROW(x, y, xs, ys);

			max_xsize = 0;

			if (IS_CELL_CLIP()) {

				for (i = yBeg; i <= yEnd; i++) {
					tE = 0;
					tL = xNum;
					tmp_x0 = cx0 + (i - yBeg) * dx0;
					tmp_y0 = cy0 + (i - yBeg) * dy0;
					tmp_dx = dx + (i - yBeg) * delta_x;
					tmp_dy = dy + (i - yBeg) * delta_y;

					if (tmp_dx) {
						d_rdx = 1.0 / tmp_dx;
						MLIB_CLIP(d_rdx,
						    -tmp_x0 + minX);
						MLIB_CLIP(-d_rdx,
						    tmp_x0 - maxX);
					} else if ((tmp_x0 < minX) ||
					    (tmp_x0 >= maxX)) {
						leftEdges[i] = 1;
						rightEdges[i] = 0;
						continue;
					}

					if (tmp_dy) {
						d_rdy = 1.0 / tmp_dy;
						MLIB_CLIP(d_rdy,
						    -tmp_y0 + minY);
						MLIB_CLIP(-d_rdy,
						    tmp_y0 - maxY);
					} else if ((tmp_y0 < minY) ||
					    (tmp_y0 >= maxY)) {
						leftEdges[i] = 1;
						rightEdges[i] = 0;
						continue;
					}

					if (tE > tL) {
						leftEdges[i] = 1;
						rightEdges[i] = 0;
						continue;
					}

					shift_left = (mlib_s32)tE;

					if ((mlib_d64)shift_left != tE)
						shift_left++;
					shift_right = (mlib_s32)tL;

					if ((mlib_d64)shift_right != tL)
						shift_right++;

					x0 = tmp_x0 + shift_left * tmp_dx;
					x1 = tmp_x0 + shift_right * tmp_dx;

					if (tmp_dx >= 0) {
						if (x0 < minX)
							shift_left++;

						if (x1 >= maxX)
							shift_right--;
					} else {

						if (x0 >= maxX)
							shift_left++;

						if (x1 < minX)
							shift_right--;
					}

					y0 = tmp_y0 + shift_left * tmp_dy;
					y1 = tmp_y0 + shift_right * tmp_dy;

					if (tmp_dy >= 0) {
						if (y0 < minY)
							shift_left++;

						if (y1 >= maxY)
							shift_right--;
					} else {

						if (y0 >= maxY)
							shift_left++;

						if (y1 < minY)
							shift_right--;
					}

					x0 = tmp_x0 + shift_left * tmp_dx;
					y0 = tmp_y0 + shift_left * tmp_dy;

					leftEdges[i] = xBeg + shift_left;
					rightEdges[i] = xBeg + shift_right;

					if ((shift_right - shift_left + 1) >
					    max_xsize)
						max_xsize =
						    (shift_right - shift_left +
						    1);

					xStarts[i] =
					    (mlib_s32)((x0 -
					    offset) * MLIB_PREC);
					yStarts[i] =
					    (mlib_s32)((y0 -
					    offset) * MLIB_PREC);
					warp_tbl[2 * i] =
					    (mlib_s32)(tmp_dx * MLIB_PREC);
					warp_tbl[2 * i + 1] =
					    (mlib_s32)(tmp_dy * MLIB_PREC);
				}
			} else {

				if ((xEnd - xBeg + 1) > max_xsize)
					max_xsize = (xEnd - xBeg + 1);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = yBeg; i <= yEnd; i++) {
					tmp_x0 = cx0 + (i - yBeg) * dx0;
					tmp_y0 = cy0 + (i - yBeg) * dy0;
					tmp_dx = dx + (i - yBeg) * delta_x;
					tmp_dy = dy + (i - yBeg) * delta_y;

					leftEdges[i] = xBeg;
					rightEdges[i] = xEnd;
					xStarts[i] =
					    (mlib_s32)((tmp_x0 -
					    offset) * MLIB_PREC);
					yStarts[i] =
					    (mlib_s32)((tmp_y0 -
					    offset) * MLIB_PREC);
					warp_tbl[2 * i] =
					    (mlib_s32)(tmp_dx * MLIB_PREC);
					warp_tbl[2 * i + 1] =
					    (mlib_s32)(tmp_dy * MLIB_PREC);
				}
			}

			if (max_xsize > 0) {
				mlib_s32 yStart = yBeg;
				mlib_s32 yFinish = yEnd;

				dstData = dstData_beg + (yBeg - 1) * dstYStride;

				STORE_PARAM(param, dstData);
				STORE_PARAM(param, yStart);
				STORE_PARAM(param, yFinish);
				STORE_PARAM(param, max_xsize);

				switch (filter) {
				case MLIB_NEAREST:
					res =
					    mlib_AffineFunArr_nn[4 * t_ind +
					    (nchan - 1)] (param);
					break;
				case MLIB_BILINEAR:
					res =
					    mlib_AffineFunArr_bl[4 * t_ind +
					    (nchan - 1)] (param);
					break;
				case MLIB_BICUBIC:
				case MLIB_BICUBIC2:
					res =
					    mlib_AffineFunArr_bc[4 * t_ind +
					    (nchan - 1)] (param);
					break;
				}

				if (res != MLIB_SUCCESS) {
					if (memBuffer != NULL)
						__mlib_free(memBuffer);
					return (res);
				}
			}
		}
	}

	if (memBuffer != NULL) {
		__mlib_free(memBuffer);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_ImageGridWarp(
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

	if (type != MLIB_BYTE && type != MLIB_SHORT && type != MLIB_USHORT &&
	    type != MLIB_INT) {
		return (MLIB_FAILURE);
	}

	return mlib_ImageGridWarp_alltypes(dst, src, xWarpPos, yWarpPos,
	    postShiftX, postShiftY,
	    xStart, xStep, xNumCells, yStart, yStep, yNumCells, filter, edge);
}

/* *********************************************************** */
