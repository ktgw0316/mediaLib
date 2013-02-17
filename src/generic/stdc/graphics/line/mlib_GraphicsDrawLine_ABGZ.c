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

#pragma ident	"@(#)mlib_GraphicsDrawLine_ABGZ.c	9.3	07/10/09 SMI"

/*
 *  FUNCTION
 *    mlib_GraphicsDrawLine_ABGZ_[8|32] - draw line between (x1, y1)
 * 	and (x2, y2) with antialiasing, Gouraud shading and Z buffering
 *
 *  SYNOPSIS
 *    mlib_status mlib_GraphicsDrawLine_ABGZ_[8|32]
 *                                      (mlib_image *buffer,
 *                                       mlib_image *zbuffer,
 *                                       mlib_s16   x1,
 *                                       mlib_s16   y1,
 *                                       mlib_s16   z1,
 *                                       mlib_s16   x2,
 *                                       mlib_s16   y2,
 *                                       mlib_s16   z2,
 *                                       mlib_s32   c1,
 *                                       mlib_s32   c2,
 *                                       mlib_s32   a);
 *  ARGUMENTS
 *    buffer  Pointer to the image which the function is drawing into.
 *    zbuffer Pointer to the image which holds the Z buffer.
 *    x1      X coordinate of the first point.
 *    y1      Y coordinate of the first point.
 *    z1      Z coordinate of the first point.
 *    x2      X coordinate of the second point.
 *    y2      Y coordinate of the second point.
 *    z2      Z coordinate of the second point.
 *    c1      Color of the first point.
 *    c2      Color of the second point.
 *    a       Alpha blending value.
 *
 *  DESCRIPTION
 *    The drawable buffer is either MLIB_BYTE(8-bit) or MLIB_INT(32-bit)
 *    type single channel mediaLib image. Pixels falling outside the
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

#pragma weak mlib_GraphicsDrawLine_ABGZ_8 = __mlib_GraphicsDrawLine_ABGZ_8
#pragma weak mlib_GraphicsDrawLine_ABGZ_32 = __mlib_GraphicsDrawLine_ABGZ_32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsDrawLine_ABGZ_8) mlib_GraphicsDrawLine_ABGZ_8
	__attribute__((weak, alias("__mlib_GraphicsDrawLine_ABGZ_8")));
__typeof__(__mlib_GraphicsDrawLine_ABGZ_32) mlib_GraphicsDrawLine_ABGZ_32
	__attribute__((weak, alias("__mlib_GraphicsDrawLine_ABGZ_32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_GRAPHICS_LINE_INNER_GZ(NEXT)                       \
	mlib_s32 coords[NEXT];                                  \
	mlib_s32 c[2];                                          \
	mlib_s16 x[2], y[2], z[2];                              \
	                                                        \
	x[0] = xx1;                                             \
	x[1] = xx2;                                             \
	y[0] = yy1;                                             \
	y[1] = yy2;                                             \
	z[0] = zz1;                                             \
	z[1] = zz2;                                             \
	c[0] = c1;                                              \
	c[1] = c2

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawLine_ABGZ_8(
	mlib_image *buffer,
	mlib_image *zbuffer,
	mlib_s16 xx1,
	mlib_s16 yy1,
	mlib_s16 zz1,
	mlib_s16 xx2,
	mlib_s16 yy2,
	mlib_s16 zz2,
	mlib_s32 c1,
	mlib_s32 c2,
	mlib_s32 a)
{
	MLIB_GRAPHICS_DECL_VARZ;

	MLIB_GRAPHICS_LINE_INNER_GZ(MLIB_GDS_NEXT_GZ);

	MLIB_GRAPHICS_CHECKZ(MLIB_NOTHING, MLIB_NOTHING);

	mlib_GraphicsClipLine_GZ_8(coords, width, height, x, y, z, c);
	mlib_GraphicsSpamLineSet_ABGZ_8(rtable, zrtable, stride, zstride,
		coords,	coords + MLIB_GDS_NEXT_GZ, a, width - 1, height - 1);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawLine_ABGZ_32(
	mlib_image *buffer,
	mlib_image *zbuffer,
	mlib_s16 xx1,
	mlib_s16 yy1,
	mlib_s16 zz1,
	mlib_s16 xx2,
	mlib_s16 yy2,
	mlib_s16 zz2,
	mlib_s32 c1,
	mlib_s32 c2,
	mlib_s32 a)
{
	MLIB_GRAPHICS_DECL_VARZ;

	MLIB_GRAPHICS_LINE_INNER_GZ(MLIB_GDS_NEXT_GZ32);

	MLIB_GRAPHICS_CHECKZ(MLIB_NOTHING, MLIB_NOTHING);

	mlib_GraphicsClipLine_GZ_32(coords, width, height, x, y, z, c);
	mlib_GraphicsSpamLineSet_ABGZ_32(rtable, zrtable, stride, zstride,
		coords, coords + MLIB_GDS_NEXT_GZ32, a, width - 1, height - 1);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
