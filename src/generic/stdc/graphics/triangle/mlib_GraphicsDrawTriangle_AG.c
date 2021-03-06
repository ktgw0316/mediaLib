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

#pragma ident	"@(#)mlib_GraphicsDrawTriangle_AG.c	9.2	07/10/09 SMI"

/*
 *  FUNCTION
 *    mlib_GraphicsDrawTriangle_AG_[8|32] - draw triangle with vertices at
 *                                    (x1, y1), (x2, y2) and (x3, y3) with
 *                                    antialiasing and Gouraud Shading
 *    mlib_GraphicsDrawTriangle[|Fan|Strip]Set_AG_[8|32]
 *      - draw corresponding set of triangles in corresponding mode :
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
 *  SYNOPSIS
 *    mlib_status mlib_GraphicsDrawTriangle_AG_[8|32]
 *                                      (mlib_image *buffer,
 *                                       mlib_s16   x1,
 *                                       mlib_s16   y1,
 *                                       mlib_s16   x2,
 *                                       mlib_s16   y2,
 *                                       mlib_s16   x3,
 *                                       mlib_s16   y3,
 *                                       mlib_s32   c1,
 *                                       mlib_s32   c2,
 *                                       mlib_s32   c3);
 *  ARGUMENTS
 *    buffer  Pointer to the image which the function is drawing into.
 *    x1      X coordinate of the first vertex.
 *    y1      Y coordinate of the first vertex.
 *    x2      X coordinate of the second vertex.
 *    y2      Y coordinate of the second vertex.
 *    x3      X coordinate of the third vertex.
 *    y3      Y coordinate of the third vertex.
 *    c1       Color of the first vertex.
 *    c2       Color of the second vertex.
 *    c3       Color of the third vertex.
 *
 *  DESCRIPTION
 *    The drawable buffer is either MLIB_BYTE(8-bit) or MLIB_INT(32-bit)
 *    type single channel mediaLib image. Pixels falling outside the
 *    drawable buffer will be clipped and discarded.
 *    All colors are specified in 32-bit format. For 8-bit buffer, only
 *    the bits 0-7 are used and the bits 8-23 should be zero. For 32-bit
 *    frame buffer, the bits 24-31 are reserved for alpha channel and
 *    should be 0xff.
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>
#include <mlib_GraphicsMacro.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsDrawTriangleFanSet_AG_32 = \
	__mlib_GraphicsDrawTriangleFanSet_AG_32
#pragma weak mlib_GraphicsDrawTriangleFanSet_AG_8 = \
	__mlib_GraphicsDrawTriangleFanSet_AG_8
#pragma weak mlib_GraphicsDrawTriangleSet_AG_32 = \
	__mlib_GraphicsDrawTriangleSet_AG_32
#pragma weak mlib_GraphicsDrawTriangleSet_AG_8 = \
	__mlib_GraphicsDrawTriangleSet_AG_8
#pragma weak mlib_GraphicsDrawTriangleStripSet_AG_32 = \
	__mlib_GraphicsDrawTriangleStripSet_AG_32
#pragma weak mlib_GraphicsDrawTriangleStripSet_AG_8 = \
	__mlib_GraphicsDrawTriangleStripSet_AG_8
#pragma weak mlib_GraphicsDrawTriangle_AG_32 = \
	__mlib_GraphicsDrawTriangle_AG_32
#pragma weak mlib_GraphicsDrawTriangle_AG_8 = \
	__mlib_GraphicsDrawTriangle_AG_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsDrawTriangleFanSet_AG_32)
	mlib_GraphicsDrawTriangleFanSet_AG_32
	__attribute__((weak,
	alias("__mlib_GraphicsDrawTriangleFanSet_AG_32")));
__typeof__(__mlib_GraphicsDrawTriangleFanSet_AG_8)
	mlib_GraphicsDrawTriangleFanSet_AG_8
	__attribute__((weak, alias("__mlib_GraphicsDrawTriangleFanSet_AG_8")));
__typeof__(__mlib_GraphicsDrawTriangleSet_AG_32)
	mlib_GraphicsDrawTriangleSet_AG_32
	__attribute__((weak, alias("__mlib_GraphicsDrawTriangleSet_AG_32")));
__typeof__(__mlib_GraphicsDrawTriangleSet_AG_8)
	mlib_GraphicsDrawTriangleSet_AG_8
	__attribute__((weak, alias("__mlib_GraphicsDrawTriangleSet_AG_8")));
__typeof__(__mlib_GraphicsDrawTriangleStripSet_AG_32)
	mlib_GraphicsDrawTriangleStripSet_AG_32
	__attribute__((weak,
	alias("__mlib_GraphicsDrawTriangleStripSet_AG_32")));
__typeof__(__mlib_GraphicsDrawTriangleStripSet_AG_8)
	mlib_GraphicsDrawTriangleStripSet_AG_8
	__attribute__((weak,
	alias("__mlib_GraphicsDrawTriangleStripSet_AG_8")));
__typeof__(__mlib_GraphicsDrawTriangle_AG_32) mlib_GraphicsDrawTriangle_AG_32
	__attribute__((weak, alias("__mlib_GraphicsDrawTriangle_AG_32")));
__typeof__(__mlib_GraphicsDrawTriangle_AG_8) mlib_GraphicsDrawTriangle_AG_8
	__attribute__((weak, alias("__mlib_GraphicsDrawTriangle_AG_8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_GRAPHICS_TRIANGLE_INNER_G(NEXT)                    \
	mlib_s32 coords[3 * NEXT];                              \
	mlib_s16 x[4], y[4];                                    \
	mlib_s32 c[4];                                          \
	                                                        \
	x[0] = x1;                                              \
	x[1] = x2;                                              \
	x[2] = x3;                                              \
	x[3] = x1;                                              \
	y[0] = y1;                                              \
	y[1] = y2;                                              \
	y[2] = y3;                                              \
	y[3] = y1;                                              \
	c[0] = c1;                                              \
	c[1] = c2;                                              \
	c[2] = c3;                                              \
	c[3] = c1

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawTriangle_AG_8(
	mlib_image *buffer,
	mlib_s16 x1,
	mlib_s16 y1,
	mlib_s16 x2,
	mlib_s16 y2,
	mlib_s16 x3,
	mlib_s16 y3,
	mlib_s32 c1,
	mlib_s32 c2,
	mlib_s32 c3)
{
	MLIB_GRAPHICS_DECL_VAR;

	MLIB_GRAPHICS_TRIANGLE_INNER_G(MLIB_GDS_NEXT_GZ);

	MLIB_GRAPHICS_CHECK(MLIB_NOTHING, MLIB_NOTHING);

	mlib_GraphicsClipLine_G_8(coords, width, height, x, y, c);
	mlib_GraphicsClipLine_G_8(coords + MLIB_GDS_NEXT_GZ, width, height,
		x + 1, y + 1, c + 1);
	mlib_GraphicsClipLine_G_8(coords + 2 * MLIB_GDS_NEXT_GZ, width, height,
		x + 2, y + 2, c + 2);
	mlib_GraphicsSpamLineSet_AG_8(rtable, stride, coords,
		coords + 3 * MLIB_GDS_NEXT_GZ, width - 1, height - 1);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawTriangle_AG_32(
	mlib_image *buffer,
	mlib_s16 x1,
	mlib_s16 y1,
	mlib_s16 x2,
	mlib_s16 y2,
	mlib_s16 x3,
	mlib_s16 y3,
	mlib_s32 c1,
	mlib_s32 c2,
	mlib_s32 c3)
{
	MLIB_GRAPHICS_DECL_VAR;

	MLIB_GRAPHICS_TRIANGLE_INNER_G(MLIB_GDS_NEXT_GZ32);

	MLIB_GRAPHICS_CHECK(MLIB_NOTHING, MLIB_NOTHING);

	mlib_GraphicsClipLine_G_32(coords, width, height, x, y, c);
	mlib_GraphicsClipLine_G_32(coords + MLIB_GDS_NEXT_GZ32, width, height,
		x + 1, y + 1, c + 1);
	mlib_GraphicsClipLine_G_32(coords + 2 * MLIB_GDS_NEXT_GZ32, width,
		height, x + 2, y + 2, c + 2);
	mlib_GraphicsSpamLineSet_AG_32(rtable, stride, coords,
		coords + 3 * MLIB_GDS_NEXT_GZ32, width - 1, height - 1);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MLIB_GDT_COMPLECT_AG(N, SUF, DP0, DP1)                          \
	MLIB_GRAPHICS_DECL_VAR;                                         \
	MLIB_GRAPHICS_DECL_##SUF;                                       \
	MLIB_GRAPHICS_CHECK(MLIB_GD_CHECK_XYG, MLIB_GD_CHECK_##SUF);    \
	mlib_GraphicsClipTriangleSet_G_##N(rtable, width, height,       \
		stride, (void *)x, (void *)y, (void *)c, nlines, DP0,   \
		DP1, mlib_GraphicsSpamLineSet_AG_##N);                  \
	return MLIB_SUCCESS

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawTriangleSet_AG_8(
	mlib_image *buffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	const mlib_s32 *c)
{
	MLIB_GDT_COMPLECT_AG(8, TS, 3, 3);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawTriangleSet_AG_32(
	mlib_image *buffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	const mlib_s32 *c)
{
	MLIB_GDT_COMPLECT_AG(32, TS, 3, 3);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawTriangleFanSet_AG_8(
	mlib_image *buffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	const mlib_s32 *c)
{
	MLIB_GDT_COMPLECT_AG(8, TFS, 0, 1);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawTriangleFanSet_AG_32(
	mlib_image *buffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	const mlib_s32 *c)
{
	MLIB_GDT_COMPLECT_AG(32, TFS, 0, 1);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawTriangleStripSet_AG_8(
	mlib_image *buffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	const mlib_s32 *c)
{
	MLIB_GDT_COMPLECT_AG(8, TSS, 1, 1);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawTriangleStripSet_AG_32(
	mlib_image *buffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	const mlib_s32 *c)
{
	MLIB_GDT_COMPLECT_AG(32, TSS, 1, 1);
}

/* *********************************************************** */
