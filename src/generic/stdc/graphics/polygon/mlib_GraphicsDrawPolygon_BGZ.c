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

#pragma ident	"@(#)mlib_GraphicsDrawPolygon_BGZ.c	9.2	07/10/09 SMI"

/*
 *  FUNCTION
 *    mlib_GraphicsDrawPolygon_BGZ_[8|32] - draw Polygon enclose (x1, y1),
 *    (x2, y2) ... (xn, yn) with Gouraud shading, Z buffering and alpha blending
 *
 *  SYNOPSIS
 *    mlib_status mlib_GraphicsDrawPolygon_BGZ_[8|32]
 *                                      (mlib_image     *buffer,
 *                                       mlib_image     *zbuffer,
 *                                       const mlib_s16 *x,
 *                                       const mlib_s16 *y,
 *                                       const mlib_s16 *z,
 *                                       mlib_s32       npoints,
 *                                       const mlib_s32 *c *c,
 *					 mlib_s32 a);
 *  ARGUMENTS
 *    buffer  Pointer to the image which the function is drawing into.
 *    zbuffer Pointer to the image which holds Z buffer.
 *    x       Pointer to array of X coordinate of the points.
 *    y       Pointer to array of Y coordinate of the points.
 *    z       Pointer to array of Z coordinate of the points.
 *    npoints Number of points in the arrays.
 *    c       Pointer to array of color of the points.
 *    a       Alpha value for the blending.
 *
 *  DESCRIPTION
 *    The drawable buffer is either MLIB_BYTE(8-bit) or MLIB_INT(32-bit)
 *    type single channel mediaLib image. Pixels fall outside the
 *    drawable buffer will be clipped and discarded.
 *    All colors are specified in 32-bit format. For 8-bit buffer, only
 *    the bits 0-7 are used and the bits 8-23 should be zero. For 32-bit
 *    frame buffer, the bits 24-31 are reserved for alpha channel and
 *    should be 0xff.
 *    In Gouraud shading, the shade of any point inside a polygon is
 *    determined by linear interpolation of vertex intensities along
 *    each edge and then between edges along each scanline.
 *    In Z buffering, the Z value of any point inside a polygon is
 *    determined by linear interpolation of vertex Z value along each
 *    edge and then between edges along each scanline.
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>
#include <mlib_GraphicsMacro.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsDrawPolygon_BGZ_32 = \
	__mlib_GraphicsDrawPolygon_BGZ_32
#pragma weak mlib_GraphicsDrawPolygon_BGZ_8 = __mlib_GraphicsDrawPolygon_BGZ_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsDrawPolygon_BGZ_32) mlib_GraphicsDrawPolygon_BGZ_32
	__attribute__((weak, alias("__mlib_GraphicsDrawPolygon_BGZ_32")));
__typeof__(__mlib_GraphicsDrawPolygon_BGZ_8) mlib_GraphicsDrawPolygon_BGZ_8
	__attribute__((weak, alias("__mlib_GraphicsDrawPolygon_BGZ_8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_GRAPHICS_POLYGON_INNER_GZ(NEXT)                    \
	mlib_s32 coords[NEXT];                                  \
	mlib_s16 xx[2], yy[2], zz[2];                           \
	mlib_s32 cc[2];                                         \
	                                                        \
	xx[0] = x[0];                                           \
	xx[1] = x[npoints - 1];                                 \
	yy[0] = y[0];                                           \
	yy[1] = y[npoints - 1];                                 \
	zz[0] = z[0];                                           \
	zz[1] = z[npoints - 1];                                 \
	cc[0] = c[0];                                           \
	cc[1] = c[npoints - 1]

/* *********************************************************** */

#define	MLIB_GDP_COMPLECT_BGZ(N, NEXT)                            \
    MLIB_GRAPHICS_DECL_VARZ;                                      \
    MLIB_GRAPHICS_DECL_LSS;                                       \
    MLIB_GRAPHICS_POLYGON_INNER_GZ(NEXT);                         \
    MLIB_GRAPHICS_CHECKZ(MLIB_GD_CHECK_XYZG, MLIB_GD_CHECK_LSS);  \
    mlib_GraphicsClipLineSet_BGZ_##N(rtable, zrtable, width,      \
	height, stride, zstride, (void *)x, (void *)y,            \
	(void *)z, (void *)c, a, nlines, 1, 1,                    \
	mlib_GraphicsSpamLineSet_BGZ_##N);                        \
    mlib_GraphicsClipLine_GZ_##N(coords, width, height, xx, yy,   \
	zz, cc);                                                  \
    mlib_GraphicsSpamLineSet_BGZ_##N(rtable, zrtable, stride,     \
	zstride, coords, coords + NEXT, a);                       \
    return MLIB_SUCCESS

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawPolygon_BGZ_8(
	mlib_image *buffer,
	mlib_image *zbuffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *z,
	mlib_s32 npoints,
	const mlib_s32 *c,
	mlib_s32 a)
{
	MLIB_GDP_COMPLECT_BGZ(8, MLIB_GDS_NEXT_GZ);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawPolygon_BGZ_32(
	mlib_image *buffer,
	mlib_image *zbuffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *z,
	mlib_s32 npoints,
	const mlib_s32 *c,
	mlib_s32 a)
{
	MLIB_GDP_COMPLECT_BGZ(32, MLIB_GDS_NEXT_GZ32);
}

/* *********************************************************** */
