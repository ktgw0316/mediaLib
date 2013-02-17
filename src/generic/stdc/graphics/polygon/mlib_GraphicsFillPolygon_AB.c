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

#pragma ident	"@(#)mlib_GraphicsFillPolygon_AB.c	9.2	07/10/09 SMI"

/*
 *  FUNCTION
 *    mlib_GraphicsFillPolygon_AB_[8|32] - fill Polygon (x1, y1),
 *              (x2, y2) ... (xn, yn), (x1, y1) with antialiasing
 * 		and alpha blending
 *
 *  SYNOPSIS
 *    mlib_status mlib_GraphicsFillPolygon_AB_[8|32]
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

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsFillPolygon_AB_32 = \
	__mlib_GraphicsFillPolygon_AB_32
#pragma weak mlib_GraphicsFillPolygon_AB_8 = __mlib_GraphicsFillPolygon_AB_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsFillPolygon_AB_32) mlib_GraphicsFillPolygon_AB_32
    __attribute__((weak, alias("__mlib_GraphicsFillPolygon_AB_32")));
__typeof__(__mlib_GraphicsFillPolygon_AB_8) mlib_GraphicsFillPolygon_AB_8
    __attribute__((weak, alias("__mlib_GraphicsFillPolygon_AB_8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillPolygon_AB_8(
    mlib_image *buffer,
    const mlib_s16 *x,
    const mlib_s16 *y,
    mlib_s32 npoints,
    mlib_s32 c,
    mlib_s32 a)
{
	__mlib_GraphicsFillPolygon_B_8(buffer, x, y, npoints, c, a);
	return (__mlib_GraphicsDrawPolygon_AB_8(buffer, x, y, npoints, c, a));
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillPolygon_AB_32(
    mlib_image *buffer,
    const mlib_s16 *x,
    const mlib_s16 *y,
    mlib_s32 npoints,
    mlib_s32 c,
    mlib_s32 a)
{
	__mlib_GraphicsFillPolygon_B_32(buffer, x, y, npoints, c, a);
	return (__mlib_GraphicsDrawPolygon_AB_32(buffer, x, y, npoints, c, a));
}

/* *********************************************************** */
