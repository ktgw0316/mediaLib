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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_GraphicsFillTriangle.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *    mlib_GraphicsFillTriangle_[8|32] - draw filled triangle
 *    mlib_GraphicsFillTriangle[|Fan|Strip]Set_[8|32]
 *      - draw corresponding set of filled triangles in corresponding mode :
 *        TriangleSet      - set of triangles at ((x1, y1), (x2, y2), (x3, y3)),
 *          ((x4, y4), (x5, y5), (x6, y6)), ..., and
 *          ((x(n-2), y(n-2)), (x(n-1), y(n-1)), (xn, yn))
 *        TriangleFanSet   - set of triangles at ((x1, y1), (x2, y2), (x3, y3)),
 *          ((x1, y1), (x3, y3), (x4, y4)), ..., and
 *          ((x1, y1), (x(n-1), y(n-1)), (xn, yn))
 *        TriangleStripSet - set of triangles at ((x1, y1), (x2, y2), (x3, y3)),
 *          ((x2, y2), (x3, y3), (x4, y4)), ..., and
 *          ((x(n-2), y(n-2)), (x(n-1), y(n-1)), (xn, yn))
 *
 * SYNOPSIS
 *    mlib_status mlib_GraphicsFillTriangle_[8|32]
 *                                          (mlib_image *buffer,
 *                                           mlib_s16   x1,
 *                                           mlib_s16   y1,
 *                                           mlib_s16   x2,
 *                                           mlib_s16   y2,
 *                                           mlib_s16   x3,
 *                                           mlib_s16   y3,
 *                                           mlib_s32   c);
 *
 * ARGUMENTS
 *    buffer  Pointer to the image which the function is drawing into.
 *    x1      X coordinate of the first vertex.
 *    y1      Y coordinate of the first vertex.
 *    x2      X coordinate of the second vertex.
 *    y2      Y coordinate of the second vertex.
 *    x3      X coordinate of the third vertex.
 *    y3      Y coordinate of the third vertex.
 *    c       Color used in the drawing.
 *
 *
 * DESCRIPTION
 *
 *  x1, y1      x3, y3
 *  *----------*
 *   \        /
 *    \      /
 *     \    /
 *      \  /
 *       \/
 *       x2, y2
 *
 *  visible
 *   -------------------------------
 *  | x | x | x | x | x | x | x | 1 |
 *   -------------------------------
 *                                |
 *                                *---- 0 - 2 left border. other 1
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>
#include <mlib_GraphicsMacro.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsFillTriangleFanSet_32 = \
	__mlib_GraphicsFillTriangleFanSet_32
#pragma weak mlib_GraphicsFillTriangleFanSet_8 = \
	__mlib_GraphicsFillTriangleFanSet_8
#pragma weak mlib_GraphicsFillTriangleSet_32 = \
	__mlib_GraphicsFillTriangleSet_32
#pragma weak mlib_GraphicsFillTriangleSet_8 = \
	__mlib_GraphicsFillTriangleSet_8
#pragma weak mlib_GraphicsFillTriangleStripSet_32 = \
	__mlib_GraphicsFillTriangleStripSet_32
#pragma weak mlib_GraphicsFillTriangleStripSet_8 = \
	__mlib_GraphicsFillTriangleStripSet_8
#pragma weak mlib_GraphicsFillTriangle_32 = __mlib_GraphicsFillTriangle_32
#pragma weak mlib_GraphicsFillTriangle_8 = __mlib_GraphicsFillTriangle_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsFillTriangleFanSet_32)
	mlib_GraphicsFillTriangleFanSet_32
	__attribute__((weak, alias("__mlib_GraphicsFillTriangleFanSet_32")));
__typeof__(__mlib_GraphicsFillTriangleFanSet_8)
	mlib_GraphicsFillTriangleFanSet_8
	__attribute__((weak, alias("__mlib_GraphicsFillTriangleFanSet_8")));
__typeof__(__mlib_GraphicsFillTriangleSet_32) mlib_GraphicsFillTriangleSet_32
	__attribute__((weak, alias("__mlib_GraphicsFillTriangleSet_32")));
__typeof__(__mlib_GraphicsFillTriangleSet_8) mlib_GraphicsFillTriangleSet_8
	__attribute__((weak, alias("__mlib_GraphicsFillTriangleSet_8")));
__typeof__(__mlib_GraphicsFillTriangleStripSet_32)
	mlib_GraphicsFillTriangleStripSet_32
	__attribute__((weak, alias("__mlib_GraphicsFillTriangleStripSet_32")));
__typeof__(__mlib_GraphicsFillTriangleStripSet_8)
	mlib_GraphicsFillTriangleStripSet_8
	__attribute__((weak, alias("__mlib_GraphicsFillTriangleStripSet_8")));
__typeof__(__mlib_GraphicsFillTriangle_32) mlib_GraphicsFillTriangle_32
	__attribute__((weak, alias("__mlib_GraphicsFillTriangle_32")));
__typeof__(__mlib_GraphicsFillTriangle_8) mlib_GraphicsFillTriangle_8
	__attribute__((weak, alias("__mlib_GraphicsFillTriangle_8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_GFT_INNER                                          \
	mlib_s16 x[4], y[4];                                    \
	                                                        \
	x[0] = x1;                                              \
	x[1] = x2;                                              \
	x[2] = x3;                                              \
	x[3] = x1;                                              \
	y[0] = y1;                                              \
	y[1] = y2;                                              \
	y[2] = y3;                                              \
	y[3] = y1

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillTriangle_8(
	mlib_image *buffer,
	mlib_s16 x1,
	mlib_s16 y1,
	mlib_s16 x2,
	mlib_s16 y2,
	mlib_s16 x3,
	mlib_s16 y3,
	mlib_s32 c)
{
	MLIB_GRAPHICS_DECL_VAR_1;

	MLIB_GFT_INNER;

	MLIB_GRAPHICS_COLOR_8(c);
	MLIB_GRAPHICS_CHECK_1(MLIB_NOTHING, MLIB_NOTHING);

	mlib_GraphicsClipTriangleHor(rtable, width, height, x, y, 3, 3, 3, c,
		mlib_GraphicsSpamLineHor_8);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillTriangle_32(
	mlib_image *buffer,
	mlib_s16 x1,
	mlib_s16 y1,
	mlib_s16 x2,
	mlib_s16 y2,
	mlib_s16 x3,
	mlib_s16 y3,
	mlib_s32 c)
{
	MLIB_GRAPHICS_DECL_VAR_1;

	MLIB_GFT_INNER;

	MLIB_GRAPHICS_COLOR_32(c);
	MLIB_GRAPHICS_CHECK_1(MLIB_NOTHING, MLIB_NOTHING);

	mlib_GraphicsClipTriangleHor(rtable, width, height, x, y, 3, 3, 3, c,
		mlib_GraphicsSpamLineHor_32);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MLIB_GFT_COMPLECT(N, SUF, DP0, DP1)                              \
	MLIB_GRAPHICS_DECL_VAR_1;                                        \
	MLIB_GRAPHICS_DECL_##SUF;                                        \
	MLIB_GRAPHICS_COLOR_##N(c);                                      \
	MLIB_GRAPHICS_CHECK_1(MLIB_GD_CHECK_XY,                          \
		MLIB_GD_CHECK_##SUF);                                    \
	mlib_GraphicsClipTriangleHor(rtable, width, height, (void *)x,   \
		(void *)y, nlines, DP0, DP1, c,                          \
		mlib_GraphicsSpamLineHor_##N);                           \
	return MLIB_SUCCESS

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillTriangleSet_8(
	mlib_image *buffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	mlib_s32 c)
{
	MLIB_GFT_COMPLECT(8, TS, 3, 3);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillTriangleSet_32(
	mlib_image *buffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	mlib_s32 c)
{
	MLIB_GFT_COMPLECT(32, TS, 3, 3);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillTriangleFanSet_8(
	mlib_image *buffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	mlib_s32 c)
{
	MLIB_GFT_COMPLECT(8, TFS, 0, 1);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillTriangleFanSet_32(
	mlib_image *buffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	mlib_s32 c)
{
	MLIB_GFT_COMPLECT(32, TFS, 0, 1);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillTriangleStripSet_8(
	mlib_image *buffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	mlib_s32 c)
{
	MLIB_GFT_COMPLECT(8, TSS, 1, 1);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillTriangleStripSet_32(
	mlib_image *buffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	mlib_s32 c)
{
	MLIB_GFT_COMPLECT(32, TSS, 1, 1);
}

/* *********************************************************** */
