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

#pragma ident	"@(#)mlib_c_GraphicsFillPolygon_GZ.c	9.2	07/10/09 SMI"

/*
 *  FUNCTION
 *    mlib_GraphicsFillPolygon_GZ_[8|32] - draw filled polygon
 *    enclose (x1, y1), (x2, y2) ... (xn, yn), (x1, y1)
 *    mlib_GraphicsFillPolygon_BGZ_[8|32] - draw filled polygon
 *    enclose (x1, y1), (x2, y2) ... (xn, yn), (x1, y1)
 *
 *  SYNOPSIS
 *    mlib_status mlib_GraphicsFillPolygon_GZ_[8|32]
 *                                      (mlib_image     *buffer,
 *                                       mlib_image     *zbuffer,
 *                                       const mlib_s16 *x,
 *                                       const mlib_s16 *y,
 *                                       const mlib_s16 *z,
 *                                       mlib_s32       npoints,
 *                                       const mlib_s32 *c);
 *    mlib_status mlib_GraphicsFillPolygon_BGZ_[8|32]
 *                                      (mlib_image     *buffer,
 *                                       mlib_image     *zbuffer,
 *                                       const mlib_s16 *x,
 *                                       const mlib_s16 *y,
 *                                       const mlib_s16 *z,
 *                                       mlib_s32       npoints,
 *                                       const mlib_s32 *c,
 *					 mlib_s32 alpha);
 *  ARGUMENTS
 *    buffer  Pointer to the image which the function is drawing into.
 *    zbuffer Pointer to the image which holds the Z buffer.
 *    x       Pointer to array of X coordinate of the points.
 *    y       Pointer to array of Y coordinate of the points.
 *    z       Pointer to array of Z coordinate of the points.
 *    npoints Number of points in the arrays.
 *    c       Pointer to array of color of the vertices.
 *    alpha   Alpha value for the blending.
 *
 *  DESCRIPTION
 *    The drawable buffer is either MLIB_BYTE(8-bit) or MLIB_INT(32-bit)
 *    type single channel mediaLib image. Z buffer is a MLIB_SHORT type
 *    single channel mediaLib image and must have the same width and height
 *    as the drawable buffer. Pixels fall outside the drawable buffer
 *    will be clipped and discarded.
 *    All colors are specified in 32-bit format. For 8-bit buffer, only
 *    the bits 0-7 are used and the bits 8-23 should be zero. For 32-bit
 *    frame buffer, the bits 24-31 are reserved for alpha channel and
 *    should be 0xff.
 *    The Z value of any point inside a polygon is determined by linear
 *    interpolation of vertex Z value along each edge and then between
 *    edges along each scanline.
 *    The shade of any point inside a polygon is determined by linear
 *    interpolation of vertex intensities along each edge and then between
 *    edges along each scanline.
 */

#include <mlib_graphics.h>
#include <mlib_ImageDivTables.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsFillPolygon_GZ_32 = \
	__mlib_GraphicsFillPolygon_GZ_32
#pragma weak mlib_GraphicsFillPolygon_GZ_8 = __mlib_GraphicsFillPolygon_GZ_8
#pragma weak mlib_GraphicsFillPolygon_BGZ_32 = \
	__mlib_GraphicsFillPolygon_BGZ_32
#pragma weak mlib_GraphicsFillPolygon_BGZ_8 = __mlib_GraphicsFillPolygon_BGZ_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsFillPolygon_GZ_32) mlib_GraphicsFillPolygon_GZ_32
	__attribute__((weak, alias("__mlib_GraphicsFillPolygon_GZ_32")));
__typeof__(__mlib_GraphicsFillPolygon_GZ_8) mlib_GraphicsFillPolygon_GZ_8
	__attribute__((weak, alias("__mlib_GraphicsFillPolygon_GZ_8")));
__typeof__(__mlib_GraphicsFillPolygon_BGZ_32) mlib_GraphicsFillPolygon_BGZ_32
	__attribute__((weak, alias("__mlib_GraphicsFillPolygon_BGZ_32")));
__typeof__(__mlib_GraphicsFillPolygon_BGZ_8) mlib_GraphicsFillPolygon_BGZ_8
	__attribute__((weak, alias("__mlib_GraphicsFillPolygon_BGZ_8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

typedef struct
{
	mlib_s32 x;
	mlib_s32 z;
	mlib_s32 c;

} SpanDataG8, *SpanDataG8Ptr;

/* *********************************************************** */

typedef struct
{
	mlib_s32 x;
	mlib_s32 z;
	mlib_s32 r;
	mlib_s32 g;
	mlib_s32 b;

} SpanDataG32, *SpanDataG32Ptr;

/* *********************************************************** */

static mlib_s32 mlib_CountExtremaAndMinMax_GZ(
	const mlib_s16 *y,
	mlib_s32 npoints,
	mlib_s32 *pyMin,
	mlib_s32 *pyMax);

static void mlib_ComputeSpansData_GZ8(
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *z,
	mlib_s32 npoints,
	const mlib_s32 *c,
	mlib_s32 yMin,
	mlib_s32 yMax,
	SpanDataG8Ptr spansData,
	mlib_s32 maxSpans,
	mlib_s32 *rowSpans);

static void mlib_ComputeSpansData_GZ32(
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *z,
	mlib_s32 npoints,
	const mlib_s32 *c,
	mlib_s32 yMin,
	mlib_s32 yMax,
	SpanDataG32Ptr spansData,
	mlib_s32 maxSpans,
	mlib_s32 *rowSpans);

/* *********************************************************** */

#define	MLIB_BLENDING_8(s, c)                       	\
    s = (mlib_U82D64[s] * A0) + (mlib_U82D64[c] * A1)

/* *********************************************************** */

#define	MLIB_FILL_ROW_GZ_8(buf_data, zbuf_data, xstart,          \
	xstop, cstart, cstep, zstart, zstep)                     \
	{                                                        \
	    mlib_u8 *dPtr = ((mlib_u8 *)buf_data) + xstart;      \
	    mlib_s16 *zPtr = ((mlib_s16 *)zbuf_data) + xstart;   \
	    mlib_s32 zv, i;                                      \
	    mlib_s32 count = xstop - xstart + 1;                 \
	                                                         \
	    for (i = 0; i < count; i++) {                        \
		zv = (zstart >> MLIB_SHIFT);                     \
		if (zv <= *zPtr) {                               \
		    *zPtr = zv;                                  \
		    *dPtr = (mlib_u8)(cstart >> MLIB_SHIFT);     \
		}                                                \
		zstart += zstep;                                 \
		cstart += cstep;                                 \
		zPtr++;                                          \
		dPtr++;                                          \
	    }                                                    \
	}

/* *********************************************************** */

#define	MLIB_FILL_ROW_BGZ_8(buf_data, zbuf_data, xstart,           \
	xstop, cstart, cstep, zstart, zstep)                       \
	{                                                          \
	mlib_u8 *dPtr = ((mlib_u8 *)buf_data) + xstart;            \
	mlib_s16 *zPtr = ((mlib_s16 *)zbuf_data) + xstart;         \
	mlib_s32 zv, i;                                            \
	mlib_s32 count = xstop - xstart + 1;                       \
	                                                           \
	for (i = 0; i < count; i++) {                              \
	zv = (zstart >> MLIB_SHIFT);                               \
	if (zv <= *zPtr) {                                         \
		*zPtr = zv;                                        \
		MLIB_BLENDING_8(*dPtr, (cstart >> MLIB_SHIFT));    \
	}                                                          \
	zstart += zstep;                                           \
	cstart += cstep;                                           \
	zPtr++;                                                    \
	dPtr++;                                                    \
	}                                                          \
	}

/* *********************************************************** */

#define	MLIB_FILL_ROW_GZ_32(buf_data, zbuf_data, xstart,                  \
	xstop, rstart, rstep, gstart, gstep, bstart, bstep, zstart,       \
	zstep)                                                            \
	{                                                                 \
	    mlib_s32 *dPtr = ((mlib_s32 *)buf_data) + xstart;             \
	    mlib_s16 *zPtr = ((mlib_s16 *)zbuf_data) + xstart;            \
	    mlib_s32 zv, color, i;                                        \
	    mlib_s32 count = xstop - xstart + 1;                          \
	                                                                  \
	    for (i = 0; i < count; i++) {                                 \
		zv = (zstart >> MLIB_SHIFT);                              \
		if (zv <= *zPtr) {                                        \
		    *zPtr = zv;                                           \
		    color = (rstart >> 16) | ((gstart >> 8) & 0xFF00) |   \
			    (bstart & 0xFF0000) | 0xFF000000U;            \
		    *dPtr = color;                                        \
		}                                                         \
		rstart += rstep;                                          \
		gstart += gstep;                                          \
		bstart += bstep;                                          \
		zstart += zstep;                                          \
		zPtr++;                                                   \
		dPtr++;                                                   \
	    }                                                             \
	}

/* *********************************************************** */

#define	MLIB_FILL_ROW_BGZ_32(buf_data, zbuf_data, xstart,     	     \
	xstop, rstart, rstep, gstart, gstep, bstart, bstep, zstart,  \
	zstep)                                                       \
	{                                                            \
	mlib_s32 *dPtr = ((mlib_s32 *)buf_data) + xstart;            \
	mlib_s16 *zPtr = ((mlib_s16 *)zbuf_data) + xstart;           \
	mlib_s32 zv, color, i, src, r, g, b;                         \
	mlib_s32 count = xstop - xstart + 1;                         \
	                                                             \
	for (i = 0; i < count; i++) {                                \
	zv = (zstart >> MLIB_SHIFT);                                 \
	if (zv <= *zPtr) {                                           \
	    *zPtr = zv;                                              \
	src = *dPtr;                                                 \
	b = (src >> 16) & 0xFF;                                      \
	g = (src >> 8) & 0xFF;                                       \
	r = src & 0xFF;                                              \
	MLIB_BLENDING_8(b, (bstart >> 16));                          \
	MLIB_BLENDING_8(g, (gstart >> 16));                          \
	MLIB_BLENDING_8(r, (rstart >> 16));                          \
	*dPtr = 0xFF000000U | (b << 16 | (g << 8) | r);              \
	}                                                            \
	rstart += rstep;                                             \
	gstart += gstep;                                             \
	bstart += bstep;                                             \
	zstart += zstep;                                             \
	zPtr++;                                                      \
	dPtr++;                                                      \
	}                                                            \
	}

/* *********************************************************** */

#define	MLIB_SHIFT	16
#define	MLIB_05	0x8000
#define	MLIB_SIGNSHIFT	31
#define	COLOR_MASK	0xFF

/* *********************************************************** */

#define	MLIB_INTMIN(a, b)	(a + ((b - a) & ((b - a) >> MLIB_SIGNSHIFT)))
#define	MLIB_INTMAX(a, b)	(a - ((a - b) & ((a - b) >> MLIB_SIGNSHIFT)))

/* *********************************************************** */

#define	CHECK_WITHOUT_TEST                                      \
	buf_data = mlib_ImageGetData(buffer);                   \
	buf_width = mlib_ImageGetWidth(buffer);                 \
	buf_height = mlib_ImageGetHeight(buffer);               \
	buf_stride = mlib_ImageGetStride(buffer);               \
	zbuf_data = mlib_ImageGetData(zbuffer);                 \
	zbuf_stride = mlib_ImageGetStride(zbuffer)

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillPolygon_GZ_8(
	mlib_image *buffer,
	mlib_image *zbuffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *z,
	mlib_s32 npoints,
	const mlib_s32 *c)
{
	mlib_s32 buf_height, buf_width, buf_stride;
	mlib_u8 *buf_data;
	mlib_s32 zbuf_stride;
	mlib_u8 *zbuf_data;

	mlib_s32 i, j;

	mlib_s32 yMin, yMax, intrsMax;
	SpanDataG8Ptr pxTrans, rowPtr;
	mlib_s32 *rowSpans;
	mlib_s32 cFixed, cFixedStep;
	mlib_s32 zFixed, zFixedStep;

	mlib_s32 xs1, xs2;

	CHECK_WITHOUT_TEST;

	if (npoints <= 2)
		return (MLIB_SUCCESS);

	intrsMax = mlib_CountExtremaAndMinMax_GZ(y, npoints, &yMin, &yMax);

/* Vertical clipped or horizontal line case. */

	if (yMax <= 0 || yMin >= buf_height || intrsMax == 0)
		return (MLIB_SUCCESS);

	yMin = MLIB_INTMAX(yMin, 0);
	yMax = MLIB_INTMIN(yMax, buf_height);

/* Allocating memory for x-transition array and z-values. */
	pxTrans =
		(SpanDataG8Ptr) __mlib_malloc((intrsMax * sizeof (SpanDataG8) +
		sizeof (mlib_s32)) * (yMax - yMin));

	if (pxTrans == NULL)
		return (MLIB_FAILURE);

	rowSpans = (mlib_s32 *)(pxTrans + (yMax - yMin) * intrsMax);
	mlib_ComputeSpansData_GZ8(x, y, z, npoints, c, yMin, yMax, pxTrans,
		intrsMax, rowSpans);

/* Fill scan lines. */

	buf_data += buf_stride * yMin;
	zbuf_data += zbuf_stride * yMin;
	rowPtr = pxTrans;

	for (j = yMin; j < yMax;
		j++, buf_data += buf_stride, zbuf_data += zbuf_stride, rowPtr +=
		intrsMax) {
		for (i = 0; i < intrsMax; i += 2) {
			xs1 = rowPtr[i].x;
			xs2 = rowPtr[i + 1].x - 1;

			if (xs1 >= buf_width)
				break;

			if (xs2 < xs1 || xs2 < 0)
				continue;

			cFixed = rowPtr[i].c;
			cFixedStep =
				(rowPtr[i + 1].c - rowPtr[i].c) / (xs2 - xs1 +
				1);

			zFixed = rowPtr[i].z;
			zFixedStep =
				(rowPtr[i + 1].z - rowPtr[i].z) / (xs2 - xs1 +
				1);

			if (xs1 < 0) {
				cFixed += (cFixedStep * (-xs1));
				zFixed += (zFixedStep * (-xs1));
				xs1 = 0;
			}

			xs2 = MLIB_INTMIN(xs2, buf_width - 1);

			MLIB_FILL_ROW_GZ_8(buf_data, zbuf_data, xs1, xs2,
				cFixed, cFixedStep, zFixed, zFixedStep);
		}
	}

	__mlib_free(pxTrans);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillPolygon_BGZ_8(
	mlib_image *buffer,
	mlib_image *zbuffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *z,
	mlib_s32 npoints,
	const mlib_s32 *c,
	mlib_s32 alpha)
{
	mlib_s32 buf_height, buf_width, buf_stride;
	mlib_u8 *buf_data;
	mlib_s32 zbuf_stride;
	mlib_u8 *zbuf_data;

	mlib_s32 i, j;

	mlib_s32 yMin, yMax, intrsMax;
	SpanDataG8Ptr pxTrans, rowPtr;
	mlib_s32 *rowSpans;
	mlib_s32 cFixed, cFixedStep;
	mlib_s32 zFixed, zFixedStep;

	mlib_s32 xs1, xs2;
	mlib_d64 A0, A1;

	CHECK_WITHOUT_TEST;

	A1 = (alpha & 0xFF) / 255.;
	A0 = 1. - A1;

	if (npoints <= 2)
		return (MLIB_SUCCESS);

	intrsMax = mlib_CountExtremaAndMinMax_GZ(y, npoints, &yMin, &yMax);

/* Vertical clipped or horizontal line case. */

	if (yMax <= 0 || yMin >= buf_height || intrsMax == 0)
		return (MLIB_SUCCESS);

	yMin = MLIB_INTMAX(yMin, 0);
	yMax = MLIB_INTMIN(yMax, buf_height);

/* Allocating memory for x-transition array and z-values. */
	pxTrans =
		(SpanDataG8Ptr) __mlib_malloc((intrsMax * sizeof (SpanDataG8) +
		sizeof (mlib_s32)) * (yMax - yMin));

	if (pxTrans == NULL)
		return (MLIB_FAILURE);

	rowSpans = (mlib_s32 *)(pxTrans + (yMax - yMin) * intrsMax);
	mlib_ComputeSpansData_GZ8(x, y, z, npoints, c, yMin, yMax, pxTrans,
		intrsMax, rowSpans);

/* Fill scan lines. */

	buf_data += buf_stride * yMin;
	zbuf_data += zbuf_stride * yMin;
	rowPtr = pxTrans;

	for (j = yMin; j < yMax;
		j++, buf_data += buf_stride, zbuf_data += zbuf_stride, rowPtr +=
		intrsMax) {
		for (i = 0; i < intrsMax; i += 2) {
			xs1 = rowPtr[i].x;
			xs2 = rowPtr[i + 1].x - 1;

			if (xs1 >= buf_width)
				break;

			if (xs2 < xs1 || xs2 < 0)
				continue;

			cFixed = rowPtr[i].c;
			cFixedStep =
				(rowPtr[i + 1].c - rowPtr[i].c) / (xs2 - xs1 +
				1);

			zFixed = rowPtr[i].z;
			zFixedStep =
				(rowPtr[i + 1].z - rowPtr[i].z) / (xs2 - xs1 +
				1);

			if (xs1 < 0) {
				cFixed += (cFixedStep * (-xs1));
				zFixed += (zFixedStep * (-xs1));
				xs1 = 0;
			}

			xs2 = MLIB_INTMIN(xs2, buf_width - 1);

			MLIB_FILL_ROW_BGZ_8(buf_data, zbuf_data, xs1, xs2,
				cFixed, cFixedStep, zFixed, zFixedStep);
		}
	}

	__mlib_free(pxTrans);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillPolygon_GZ_32(
	mlib_image *buffer,
	mlib_image *zbuffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *z,
	mlib_s32 npoints,
	const mlib_s32 *c)
{
	mlib_s32 buf_height, buf_width, buf_stride;
	mlib_u8 *buf_data;
	mlib_s32 zbuf_stride;
	mlib_u8 *zbuf_data;
	mlib_s32 rFixed, rFixedStep;
	mlib_s32 gFixed, gFixedStep;
	mlib_s32 bFixed, bFixedStep;
	mlib_s32 zFixed, zFixedStep;

	mlib_s32 i, j;

	mlib_s32 yMin, yMax, intrsMax;
	SpanDataG32Ptr pxTrans, rowPtr;
	mlib_s32 *rowSpans;

	mlib_s32 xs1, xs2;

	CHECK_WITHOUT_TEST;

	if (npoints <= 2)
		return (MLIB_SUCCESS);

	intrsMax = mlib_CountExtremaAndMinMax_GZ(y, npoints, &yMin, &yMax);

/* Vertical clipped or horizontal line case. */

	if (yMax <= 0 || yMin >= buf_height || intrsMax == 0)
		return (MLIB_SUCCESS);

	yMin = MLIB_INTMAX(yMin, 0);
	yMax = MLIB_INTMIN(yMax, buf_height);

/* Allocating memory for x-transition array and z-values. */
	pxTrans =
		(SpanDataG32Ptr) __mlib_malloc((intrsMax *
		sizeof (SpanDataG32) + sizeof (mlib_s32)) * (yMax - yMin));

	if (pxTrans == NULL)
		return (MLIB_FAILURE);

	rowSpans = (mlib_s32 *)(pxTrans + (yMax - yMin) * intrsMax);
	mlib_ComputeSpansData_GZ32(x, y, z, npoints, c, yMin, yMax, pxTrans,
		intrsMax, rowSpans);

/* Fill scan lines. */

	buf_data += buf_stride * yMin;
	zbuf_data += zbuf_stride * yMin;
	rowPtr = pxTrans;

	for (j = yMin; j < yMax;
		j++, buf_data += buf_stride, zbuf_data += zbuf_stride, rowPtr +=
		intrsMax) {
		for (i = 0; i < intrsMax; i += 2) {
			xs1 = rowPtr[i].x;
			xs2 = rowPtr[i + 1].x - 1;

			if (xs1 >= buf_width)
				break;

			if (xs2 < xs1 || xs2 < 0)
				continue;

			rFixed = rowPtr[i].r;
			rFixedStep =
				(rowPtr[i + 1].r - rowPtr[i].r) / (xs2 - xs1 +
				1);
			gFixed = rowPtr[i].g;
			gFixedStep =
				(rowPtr[i + 1].g - rowPtr[i].g) / (xs2 - xs1 +
				1);
			bFixed = rowPtr[i].b;
			bFixedStep =
				(rowPtr[i + 1].b - rowPtr[i].b) / (xs2 - xs1 +
				1);
			zFixed = rowPtr[i].z;
			zFixedStep =
				(rowPtr[i + 1].z - rowPtr[i].z) / (xs2 - xs1 +
				1);

			if (xs1 < 0) {
				rFixed += (rFixedStep * (-xs1));
				gFixed += (gFixedStep * (-xs1));
				bFixed += (bFixedStep * (-xs1));
				zFixed += (zFixedStep * (-xs1));
				xs1 = 0;
			}

			xs2 = MLIB_INTMIN(xs2, buf_width - 1);

			MLIB_FILL_ROW_GZ_32(buf_data, zbuf_data, xs1, xs2,
				rFixed, rFixedStep, gFixed, gFixedStep, bFixed,
				bFixedStep, zFixed, zFixedStep);
		}
	}

	__mlib_free(pxTrans);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillPolygon_BGZ_32(
	mlib_image *buffer,
	mlib_image *zbuffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *z,
	mlib_s32 npoints,
	const mlib_s32 *c,
	mlib_s32 alpha)
{
	mlib_s32 buf_height, buf_width, buf_stride;
	mlib_u8 *buf_data;
	mlib_s32 zbuf_stride;
	mlib_u8 *zbuf_data;
	mlib_s32 rFixed, rFixedStep;
	mlib_s32 gFixed, gFixedStep;
	mlib_s32 bFixed, bFixedStep;
	mlib_s32 zFixed, zFixedStep;

	mlib_s32 i, j;

	mlib_s32 yMin, yMax, intrsMax;
	SpanDataG32Ptr pxTrans, rowPtr;
	mlib_s32 *rowSpans;

	mlib_s32 xs1, xs2;
	mlib_d64 A0, A1;

	CHECK_WITHOUT_TEST;

	A1 = alpha / 255.;
	A0 = 1. - A1;

	if (npoints <= 2)
		return (MLIB_SUCCESS);

	intrsMax = mlib_CountExtremaAndMinMax_GZ(y, npoints, &yMin, &yMax);

/* Vertical clipped or horizontal line case. */

	if (yMax <= 0 || yMin >= buf_height || intrsMax == 0)
		return (MLIB_SUCCESS);

	yMin = MLIB_INTMAX(yMin, 0);
	yMax = MLIB_INTMIN(yMax, buf_height);

/* Allocating memory for x-transition array and z-values. */
	pxTrans =
		(SpanDataG32Ptr) __mlib_malloc((intrsMax *
		sizeof (SpanDataG32) + sizeof (mlib_s32)) * (yMax - yMin));

	if (pxTrans == NULL)
		return (MLIB_FAILURE);

	rowSpans = (mlib_s32 *)(pxTrans + (yMax - yMin) * intrsMax);
	mlib_ComputeSpansData_GZ32(x, y, z, npoints, c, yMin, yMax, pxTrans,
		intrsMax, rowSpans);

/* Fill scan lines. */

	buf_data += buf_stride * yMin;
	zbuf_data += zbuf_stride * yMin;
	rowPtr = pxTrans;

	for (j = yMin; j < yMax;
		j++, buf_data += buf_stride, zbuf_data += zbuf_stride, rowPtr +=
		intrsMax) {
		for (i = 0; i < intrsMax; i += 2) {
			xs1 = rowPtr[i].x;
			xs2 = rowPtr[i + 1].x - 1;

			if (xs1 >= buf_width)
				break;

			if (xs2 < xs1 || xs2 < 0)
				continue;

			rFixed = rowPtr[i].r;
			rFixedStep =
				(rowPtr[i + 1].r - rowPtr[i].r) / (xs2 - xs1 +
				1);
			gFixed = rowPtr[i].g;
			gFixedStep =
				(rowPtr[i + 1].g - rowPtr[i].g) / (xs2 - xs1 +
				1);
			bFixed = rowPtr[i].b;
			bFixedStep =
				(rowPtr[i + 1].b - rowPtr[i].b) / (xs2 - xs1 +
				1);
			zFixed = rowPtr[i].z;
			zFixedStep =
				(rowPtr[i + 1].z - rowPtr[i].z) / (xs2 - xs1 +
				1);

			if (xs1 < 0) {
				rFixed += (rFixedStep * (-xs1));
				gFixed += (gFixedStep * (-xs1));
				bFixed += (bFixedStep * (-xs1));
				zFixed += (zFixedStep * (-xs1));
				xs1 = 0;
			}

			xs2 = MLIB_INTMIN(xs2, buf_width - 1);

			MLIB_FILL_ROW_BGZ_32(buf_data, zbuf_data, xs1, xs2,
				rFixed, rFixedStep, gFixed, gFixedStep, bFixed,
				bFixedStep, zFixed, zFixedStep);
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
mlib_CountExtremaAndMinMax_GZ(
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
mlib_ComputeSpansData_GZ8(
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *z,
	mlib_s32 npoints,
	const mlib_s32 *c,
	mlib_s32 yMin,
	mlib_s32 yMax,
	SpanDataG8Ptr spansData,
	mlib_s32 maxSpans,
	mlib_s32 *rowSpans)
{
	SpanDataG8Ptr rowPtr;
	mlib_s32 i, j, count;
	mlib_s32 xPrev, yPrev;
	mlib_s32 x1, x2, y1, y2, dx, dy;
	mlib_s32 xStepConst, eps, delx, dely, mask;

	mlib_s32 z1, z2, zPrev;
	mlib_s32 zFixed, zFixedStep;

	mlib_s32 c1, c2, cPrev;
	mlib_s32 cFixed, cFixedStep;

	rowPtr = spansData;
	count = (yMax - yMin) * maxSpans;
	for (i = 0; i < count; i++) {
		rowPtr->x = MLIB_S32_MAX;
		rowPtr++;
	}

	count = (yMax - yMin);
	for (i = 0; i < count; i++)
		rowSpans[i] = 0;

	xPrev = x[npoints - 1];
	yPrev = y[npoints - 1];
	zPrev = z[npoints - 1];
	cPrev = (COLOR_MASK & c[npoints - 1]);

	for (i = 0; i < npoints; i++) {
		x1 = xPrev;
		y1 = yPrev;
		z1 = zPrev;
		c1 = cPrev;
		x2 = xPrev = x[i];
		y2 = yPrev = y[i];
		z2 = zPrev = z[i];
		c2 = cPrev = (COLOR_MASK & c[i]);

		if (y1 > y2) {
			x1 ^= x2 ^= x1 ^= x2;
			y1 ^= y2 ^= y1 ^= y2;
			z1 ^= z2 ^= z1 ^= z2;
			c1 ^= c2 ^= c1 ^= c2;
		}

/* vertical clipping and horizontal edges. */

		if (y2 <= yMin || y1 >= yMax || y1 == y2)
			continue;

		dx = x2 - x1;
		dy = y2 - y1;

		zFixed = (z1 << MLIB_SHIFT) + MLIB_05;
		zFixedStep = ((z2 - z1) << MLIB_SHIFT) / dy;

		cFixed = (c1 << MLIB_SHIFT) + MLIB_05;
		cFixedStep = ((c2 - c1) << MLIB_SHIFT) / dy;

		xStepConst = (dx - ((dy - 1) & (dx >> MLIB_SIGNSHIFT))) / dy;
		dx -= xStepConst * dy;

		dely = (yMin - y1) & ((y1 - yMin) >> MLIB_SIGNSHIFT);
		y1 += dely;

		delx = (dx * dely + dy - 1) / dy;
		eps = delx * dy - dely * dx - dx;
		x1 += delx + dely * xStepConst;

		zFixed += zFixedStep * dely;
		cFixed += cFixedStep * dely;

		y2 = MLIB_INTMIN(y2, yMax);

		rowPtr = spansData + (y1 - yMin) * maxSpans;

		for (; y1 < y2; y1++, rowPtr += maxSpans) {
			j = rowSpans[y1 - yMin]++;
			while (--j >= 0 && x1 < rowPtr[j].x)
				rowPtr[j + 1] = rowPtr[j];
			rowPtr[j + 1].x = x1;
			rowPtr[j + 1].z = zFixed;
			rowPtr[j + 1].c = cFixed;

			mask = (eps >> MLIB_SIGNSHIFT);
			x1 += (xStepConst + (mask & 1));
			eps -= (dx - (mask & dy));
			zFixed += zFixedStep;
			cFixed += cFixedStep;
		}
	}
}

/* *********************************************************** */

static void
mlib_ComputeSpansData_GZ32(
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *z,
	mlib_s32 npoints,
	const mlib_s32 *c,
	mlib_s32 yMin,
	mlib_s32 yMax,
	SpanDataG32Ptr spansData,
	mlib_s32 maxSpans,
	mlib_s32 *rowSpans)
{
	SpanDataG32Ptr rowPtr;
	mlib_s32 i, j, count;
	mlib_s32 xPrev, yPrev;
	mlib_s32 x1, x2, y1, y2, dx, dy;
	mlib_s32 xStepConst, eps, delx, dely, mask;

	mlib_s32 z1, z2, zPrev;
	mlib_s32 zFixed, zFixedStep;

	mlib_s32 c1, c2, cPrev;
	mlib_s32 rFixed, rFixedStep;
	mlib_s32 gFixed, gFixedStep;
	mlib_s32 bFixed, bFixedStep;

	rowPtr = spansData;
	count = (yMax - yMin) * maxSpans;
	for (i = 0; i < count; i++) {
		rowPtr->x = MLIB_S32_MAX;
		rowPtr++;
	}

	count = (yMax - yMin);
	for (i = 0; i < count; i++)
		rowSpans[i] = 0;

	xPrev = x[npoints - 1];
	yPrev = y[npoints - 1];
	zPrev = z[npoints - 1];
	cPrev = c[npoints - 1];

	for (i = 0; i < npoints; i++) {
		x1 = xPrev;
		y1 = yPrev;
		z1 = zPrev;
		c1 = cPrev;
		x2 = xPrev = x[i];
		y2 = yPrev = y[i];
		z2 = zPrev = z[i];
		c2 = cPrev = c[i];

		if (y1 > y2) {
			x1 ^= x2 ^= x1 ^= x2;
			y1 ^= y2 ^= y1 ^= y2;
			z1 ^= z2 ^= z1 ^= z2;
			c1 ^= c2 ^= c1 ^= c2;
		}

/* vertical clipping and horizontal edges. */

		if (y2 <= yMin || y1 >= yMax || y1 == y2)
			continue;

		dx = x2 - x1;
		dy = y2 - y1;

		zFixed = (z1 << MLIB_SHIFT) + MLIB_05;
		zFixedStep = ((z2 - z1) << MLIB_SHIFT) / dy;

		rFixed = ((c1 & 0xFF) << MLIB_SHIFT) + MLIB_05;
		rFixedStep = (((c2 & 0xFF) - (c1 & 0xFF)) << MLIB_SHIFT) / dy;

		gFixed = ((c1 & 0xFF00) << 8) + MLIB_05;
		gFixedStep = (((c2 & 0xFF00) - (c1 & 0xFF00)) << 8) / dy;

		bFixed = (c1 & 0xFF0000) + MLIB_05;
		bFixedStep = ((c2 & 0xFF0000) - (c1 & 0xFF0000)) / dy;

		xStepConst = (dx - ((dy - 1) & (dx >> MLIB_SIGNSHIFT))) / dy;
		dx -= xStepConst * dy;

		dely = (yMin - y1) & ((y1 - yMin) >> MLIB_SIGNSHIFT);
		y1 += dely;

		delx = (dx * dely + dy - 1) / dy;
		eps = delx * dy - dely * dx - dx;
		x1 += delx + dely * xStepConst;

		zFixed += zFixedStep * dely;
		rFixed += rFixedStep * dely;
		gFixed += gFixedStep * dely;
		bFixed += bFixedStep * dely;

		y2 = MLIB_INTMIN(y2, yMax);

		rowPtr = spansData + (y1 - yMin) * maxSpans;

		for (; y1 < y2; y1++, rowPtr += maxSpans) {
			j = rowSpans[y1 - yMin]++;
			while (--j >= 0 && x1 < rowPtr[j].x)
				rowPtr[j + 1] = rowPtr[j];
			rowPtr[j + 1].x = x1;
			rowPtr[j + 1].z = zFixed;
			rowPtr[j + 1].r = rFixed;
			rowPtr[j + 1].g = gFixed;
			rowPtr[j + 1].b = bFixed;

			mask = (eps >> MLIB_SIGNSHIFT);
			x1 += (xStepConst + (mask & 1));
			eps -= (dx - (mask & dy));

			zFixed += zFixedStep;
			rFixed += rFixedStep;
			gFixed += gFixedStep;
			bFixed += bFixedStep;
		}
	}
}

/* *********************************************************** */
