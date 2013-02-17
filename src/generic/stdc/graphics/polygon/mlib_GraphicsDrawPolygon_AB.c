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

#pragma ident	"@(#)mlib_GraphicsDrawPolygon_AB.c	9.2	07/10/09 SMI"

/*
 *  FUNCTION
 *    mlib_GraphicsDrawPolygon_AB_[8|32] - draw Polygon connecting
 *    points (x1, y1), (x2, y2) ... (xn, yn), (x1, y1) with antialiasing
 *    and alpha blending
 *
 *  SYNOPSIS
 *    mlib_status mlib_GraphicsDrawPolygon_AB_[8|32]
 *                                      (mlib_image     *buffer,
 *                                       const mlib_s16 *x,
 *                                       const mlib_s16 *y,
 *                                       mlib_s32       npoints,
 *                                       mlib_s32       c,
 *                                       mlib_s32       a);
 *  ARGUMENTS
 *    buffer  Pointer to the image which the function is drawing into.
 *    x       Pointer to array of X coordinate of the points.
 *    y       Pointer to array of Y coordinate of the points.
 *    npoints Number of points in the arrays.
 *    c       Color used in the drawing.
 *    a       Alpha blending value.
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

#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>
#include <mlib_GraphicsMacro.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsDrawPolygon_AB_32 = \
	__mlib_GraphicsDrawPolygon_AB_32
#pragma weak mlib_GraphicsDrawPolygon_AB_8 = __mlib_GraphicsDrawPolygon_AB_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsDrawPolygon_AB_32) mlib_GraphicsDrawPolygon_AB_32
    __attribute__((weak, alias("__mlib_GraphicsDrawPolygon_AB_32")));
__typeof__(__mlib_GraphicsDrawPolygon_AB_8) mlib_GraphicsDrawPolygon_AB_8
    __attribute__((weak, alias("__mlib_GraphicsDrawPolygon_AB_8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_GRAPHICS_POLYGON_INNER(NEXT)                       \
	mlib_s32 coords[NEXT];                                  \
	mlib_s16 xx[2], yy[2];                                  \
	mlib_d64 d_c;                                           \
	                                                        \
	xx[0] = x[0];                                           \
	xx[1] = x[npoints - 1];                                 \
	yy[0] = y[0];                                           \
	yy[1] = y[npoints - 1]

/* *********************************************************** */

#define	MLIB_GDP_COMPLECT_AB(N, NEXT)                                \
	MLIB_GRAPHICS_DECL_VAR;                                      \
	MLIB_GRAPHICS_DECL_LSS;                                      \
	MLIB_GRAPHICS_POLYGON_INNER(NEXT);                           \
	MLIB_GRAPHICS_COLOR_##N(c);                                  \
	MLIB_GRAPHICS_TO_DOUBLE(d_c, c);                             \
	MLIB_GRAPHICS_CHECK(MLIB_GD_CHECK_XY, MLIB_GD_CHECK_LSS);    \
	mlib_GraphicsClipLineSet_AB(rtable, width, height, stride,   \
	    (void *)x, (void *)y, nlines, 1, 1, c, a,                \
	    mlib_GraphicsSpamLineSet_AB_##N);                        \
	mlib_GraphicsClipLine(coords, width, height, xx, yy);        \
	mlib_GraphicsSpamLineSet_AB_##N(rtable, stride, coords,      \
	    coords + NEXT, c, a, d_c, width - 1, height - 1);        \
	return MLIB_SUCCESS

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawPolygon_AB_8(
    mlib_image *buffer,
    const mlib_s16 *x,
    const mlib_s16 *y,
    mlib_s32 npoints,
    mlib_s32 c,
    mlib_s32 a)
{
	MLIB_GDP_COMPLECT_AB(8, MLIB_GDS_NEXT);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawPolygon_AB_32(
    mlib_image *buffer,
    const mlib_s16 *x,
    const mlib_s16 *y,
    mlib_s32 npoints,
    mlib_s32 c,
    mlib_s32 a)
{
	MLIB_GDP_COMPLECT_AB(32, MLIB_GDS_NEXT);
}

/* *********************************************************** */
