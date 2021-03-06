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

#pragma ident	"@(#)mlib_GraphicsFillPolygon.c	9.2	07/10/09 SMI"

/*
 *  FUNCTION
 *    mlib_GraphicsFillPolygon_[8|32] - draw filled polygon
 *    enclose (x1, y1), (x2, y2) ... (xn, yn), (x1, y1)
 *
 *  SYNOPSIS
 *    mlib_status mlib_GraphicsFillPolygon_[8|32]
 *                                      (mlib_image     *buffer,
 *                                       const mlib_s16 *x,
 *                                       const mlib_s16 *y,
 *                                       mlib_s32       npoints,
 *                                       mlib_s32       c);
 *  ARGUMENTS
 *    buffer  Pointer to the image which the function is drawing into.
 *    x       Pointer to array of X coordinate of the points.
 *    y       Pointer to array of Y coordinate of the points.
 *    npoints Number of points in the arrays.
 *    c       Color used in the drawing.
 *
 *  DESCRIPTION
 *    The drawable buffer is either MLIB_BYTE(8-bit) or MLIB_INT(32-bit)
 *    type single channel mediaLib image. Pixels fall outside the
 *    drawable buffer will be clipped and discarded.
 *    All colors are specified in 32-bit format. For 8-bit buffer, only
 *    the bits 0-7 are used and the bits 8-23 should be zero. For 32-bit
 *    frame buffer, the bits 24-31 are reserved for alpha channel and
 *    should be 0xff.
 */

#include <stdlib.h>
#include <mlib_graphics.h>
#include <mlib_GraphicsFillRow.h>
#include <mlib_GraphicsMacro.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsFillPolygon_32 = __mlib_GraphicsFillPolygon_32
#pragma weak mlib_GraphicsFillPolygon_8 = __mlib_GraphicsFillPolygon_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsFillPolygon_32) mlib_GraphicsFillPolygon_32
	__attribute__((weak, alias("__mlib_GraphicsFillPolygon_32")));
__typeof__(__mlib_GraphicsFillPolygon_8) mlib_GraphicsFillPolygon_8
	__attribute__((weak, alias("__mlib_GraphicsFillPolygon_8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static mlib_s32 mlib_CountExtremaAndMinMax(
	const mlib_s16 *y,
	mlib_s32 npoints,
	mlib_s32 *pyMin,
	mlib_s32 *pyMax);

/* *********************************************************** */

static void mlib_ComputeSpansData(
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	mlib_s32 yMin,
	mlib_s32 yMax,
	mlib_s32 *spansData,
	mlib_s32 maxSpans,
	mlib_s32 *rowSpans);

/* *********************************************************** */

#define	MLIB_NOTVISIBLE	MLIB_SUCCESS

/* *********************************************************** */

#define	ARRAY_DIM(array)	(sizeof (array) / sizeof (array[0]))
#define	MLIB_SIGNSHIFT	31
#define	MLIB_INTMIN(a, b)	(a + ((b - a) & ((b - a) >> MLIB_SIGNSHIFT)))
#define	MLIB_INTMAX(a, b)	(a - ((a - b) & ((a - b) >> MLIB_SIGNSHIFT)))

/* *********************************************************** */

#define	CHECK_WITHOUT_TEST                                      \
	buf_data = mlib_ImageGetData(buffer);                   \
	buf_width = mlib_ImageGetWidth(buffer);                 \
	buf_height = mlib_ImageGetHeight(buffer);               \
	buf_stride = mlib_ImageGetStride(buffer)

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillPolygon_8(
	mlib_image *buffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	mlib_s32 c)
{
	mlib_s32 buf_height, buf_width, buf_stride;
	mlib_u8 *buf_data;
	mlib_s32 i, j;
	mlib_s32 yMin, yMax, intrsMax;
	mlib_s32 *pxTrans;
	mlib_s32 *rowPtr;
	mlib_s32 *rowSpans;
	mlib_s32 xs1, xs2;
	mlib_d64 dc;

	CHECK_WITHOUT_TEST;

	if (npoints <= 2)
		return (MLIB_SUCCESS);

	intrsMax = mlib_CountExtremaAndMinMax(y, npoints, &yMin, &yMax);

/* Vertical clipped or horizontal line case. */

	if (yMax <= 0 || yMin >= buf_height || intrsMax == 0)
		return (MLIB_SUCCESS);

	yMin = MLIB_INTMAX(yMin, 0);
	yMax = MLIB_INTMIN(yMax, buf_height);

/* Allocating memory for x-transition array. */
	pxTrans =
		(mlib_s32 *)__mlib_malloc(((intrsMax + 1) * (yMax - yMin)) *
		sizeof (mlib_s32));

	if (pxTrans == NULL)
		return (MLIB_FAILURE);

	rowSpans = pxTrans + (yMax - yMin) * intrsMax;
	mlib_ComputeSpansData(x, y, npoints, yMin, yMax, pxTrans, intrsMax,
		rowSpans);

	MLIB_GRAPHICS_COLOR_8(c);
	MLIB_GRAPHICS_TO_DOUBLE(dc, c);

/* Fill scan lines. */

	buf_data += buf_stride * yMin;
	rowPtr = pxTrans;

	for (j = yMin; j < yMax;
		j++, buf_data += buf_stride, rowPtr += intrsMax) {
		for (i = 0; i < intrsMax; i += 2) {
			xs1 = rowPtr[i];
			xs2 = rowPtr[i + 1] - 1;

			if (xs1 >= buf_width)
				break;

			if (xs2 < xs1 || xs2 < 0)
				continue;

			xs1 = MLIB_INTMAX(xs1, 0);
			xs2 = MLIB_INTMIN(xs2, buf_width - 1);

			MLIB_FILL_ROW_8(buf_data, xs1, xs2, c, dc);
		}
	}

	__mlib_free(pxTrans);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillPolygon_32(
	mlib_image *buffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	mlib_s32 c)
{
	mlib_s32 buf_height, buf_width, buf_stride;
	mlib_u8 *buf_data;
	mlib_s32 i, j;
	mlib_s32 yMin, yMax, intrsMax;
	mlib_s32 *pxTrans;
	mlib_s32 *rowPtr;
	mlib_s32 *rowSpans;
	mlib_s32 xs1, xs2;
	mlib_d64 dc;

	CHECK_WITHOUT_TEST;

	if (npoints <= 2)
		return (MLIB_SUCCESS);

	intrsMax = mlib_CountExtremaAndMinMax(y, npoints, &yMin, &yMax);

/* Vertical clipped or horizontal line case. */

	if (yMax <= 0 || yMin >= buf_height || intrsMax == 0)
		return (MLIB_SUCCESS);

	yMin = MLIB_INTMAX(yMin, 0);
	yMax = MLIB_INTMIN(yMax, buf_height);

/* Allocating memory for x-transition array. */
	pxTrans =
		(mlib_s32 *)__mlib_malloc(((intrsMax + 1) * (yMax - yMin)) *
		sizeof (mlib_s32));

	if (pxTrans == NULL)
		return (MLIB_FAILURE);

	rowSpans = pxTrans + (yMax - yMin) * intrsMax;
	mlib_ComputeSpansData(x, y, npoints, yMin, yMax, pxTrans, intrsMax,
		rowSpans);

	MLIB_GRAPHICS_COLOR_32(c);
	MLIB_GRAPHICS_TO_DOUBLE(dc, c);

/* Fill scan lines. */

	buf_data += buf_stride * yMin;
	rowPtr = pxTrans;

	for (j = yMin; j < yMax;
		j++, buf_data += buf_stride, rowPtr += intrsMax) {
		for (i = 0; i < intrsMax; i += 2) {
			xs1 = rowPtr[i];
			xs2 = rowPtr[i + 1] - 1;

			if (xs1 >= buf_width)
				break;

			if (xs2 < xs1 || xs2 < 0)
				continue;

			xs1 = MLIB_INTMAX(xs1, 0);
			xs2 = MLIB_INTMIN(xs2, buf_width - 1);

			MLIB_FILL_ROW_32(((mlib_s32 *)buf_data), xs1, xs2, c,
				dc);
		}
	}

	__mlib_free(pxTrans);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

/*
 * Count vertical limits of polygon and max number of intersections
 * with scan line (number of polygon extrema).
 */

static mlib_s32
mlib_CountExtremaAndMinMax(
	const mlib_s16 *y,
	mlib_s32 npoints,
	mlib_s32 *pyMin,
	mlib_s32 *pyMax)
{
	mlib_s32 yMin, yMax, extrNum;
	mlib_s32 i, yCur, yPrev, dir, firstDir;

	yMin = MLIB_S32_MAX;
	yMax = MLIB_S32_MIN;

	yPrev = y[npoints - 1];
	dir = 0;
	firstDir = 0;
	extrNum = 0;

	for (i = 0; i < npoints; i++) {
		yCur = y[i];

		if (yCur > yPrev) {
			if (dir < 0)
				extrNum++;
			dir = 1;

			if (firstDir == 0)
				firstDir = 1;
		} else if (yCur < yPrev) {
			if (dir > 0)
				extrNum++;
			dir = -1;

			if (firstDir == 0)
				firstDir = -1;
		}

		if (yMin > yCur)
			yMin = yCur;

		if (yMax < yCur)
			yMax = yCur;

		yPrev = yCur;
	}

	if (firstDir != dir)
		extrNum++;

	*pyMin = yMin;
	*pyMax = yMax;

	return (extrNum);
}

/* *********************************************************** */

static void
mlib_ComputeSpansData(
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	mlib_s32 yMin,
	mlib_s32 yMax,
	mlib_s32 *spansData,
	mlib_s32 maxSpans,
	mlib_s32 *rowSpans)
{
	mlib_s32 *rowPtr;
	mlib_s32 i, j, count;
	mlib_s32 xPrev, yPrev;
	mlib_s32 x1, x2, y1, y2, dx, dy;
	mlib_s32 xStepConst, eps, delx, dely, mask;

	rowPtr = spansData;
	count = (yMax - yMin) * maxSpans;
	for (i = 0; i < count; i++)
		(*rowPtr++) = MLIB_S32_MAX;

	rowPtr = rowSpans;
	count = (yMax - yMin);
	for (i = 0; i < count; i++)
		(*rowPtr++) = 0;

	xPrev = x[npoints - 1];
	yPrev = y[npoints - 1];

	for (i = 0; i < npoints; i++) {
		x1 = xPrev;
		y1 = yPrev;
		x2 = xPrev = x[i];
		y2 = yPrev = y[i];

		if (y1 > y2) {
			x1 ^= x2 ^= x1 ^= x2;
			y1 ^= y2 ^= y1 ^= y2;
		}

/* vertical clipping and horizontal edges. */

		if (y2 <= yMin || y1 >= yMax || y1 == y2)
			continue;

		dx = x2 - x1;
		dy = y2 - y1;

		xStepConst = (dx - ((dy - 1) & (dx >> MLIB_SIGNSHIFT))) / dy;
		dx -= xStepConst * dy;

		dely = (yMin - y1) & ((y1 - yMin) >> MLIB_SIGNSHIFT);
		delx = (dx * dely + dy - 1) / dy;
		eps = delx * dy - dely * dx - dx;
		x1 += delx + dely * xStepConst;
		y1 += dely;

		y2 = MLIB_INTMIN(y2, yMax);

		rowPtr = spansData + (y1 - yMin) * maxSpans;

		for (; y1 < y2; y1++, rowPtr += maxSpans) {
			j = rowSpans[y1 - yMin]++;
			while (--j >= 0 && x1 < rowPtr[j])
				rowPtr[j + 1] = rowPtr[j];
			rowPtr[j + 1] = x1;

			mask = (eps >> MLIB_SIGNSHIFT);
			x1 += (xStepConst + (mask & 1));
			eps -= (dx - (mask & dy));
		}
	}
}

/* *********************************************************** */
