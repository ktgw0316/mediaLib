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

#pragma ident	"@(#)mlib_GraphicsDrawPolypoint_B.c	9.2	07/10/09 SMI"

/*
 *  FUNCTION
 *    mlib_GraphicsDrawPointSet_B_[8|32] - draw set of points at
 *                                         (x1, y1), (x2, y2)...(xn, yn)
 *                                         with alpha blending
 *    mlib_GraphicsDrawPolypoint_B_[8|32] - draw points at
 *                                          (x1, y1), (x2, y2)...(xn, yn)
 *                                          with alpha blending
 *
 *  SYNOPSIS
 *    mlib_status mlib_GraphicsDrawPointSet_B_[8|32]
 *                                      (mlib_image     *buffer,
 *                                       const mlib_s16 *x,
 *                                       const mlib_s16 *y,
 *                                       mlib_s32       npoints,
 *                                       mlib_s32       c,
 *                                       mlib_s32       a);
 *    mlib_status mlib_GraphicsDrawPolypoint_B_[8|32]
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
 *    a       Alpha value for the blending.
 *
 *  DESCRIPTION
 *    The drawable buffer is either MLIB_BYTE (8-bit) or MLIB_INT (32-bit)
 *    type single channel mediaLib image.
 *
 *    Pixels falling outside the drawable buffer will be clipped and discarded.
 *
 *    All colors are specified in 32-bit format. For 8-bit buffer, only
 *    the bits 0-7 are used and the bits 8-23 should be zero.
 *
 *    In alpha value, only the bits 0-7 are used and the bits 8-23 should
 *    be zero.  For each drawable pixel, the original pixel value is
 *    blended with the drawing color to produce the final pixel value
 *
 *        d = (s * (255 - a) + c * a) / 255
 *
 *    where c is the color used in the drawing, a is the alpha value, s
 *    is the original value of the pixel, and d is the final value of the
 *    pixel.
 *
 *  NOTE
 *    mlib_GraphicsDrawPolypoint* functions are analogous to corresponding
 *    mlib_GraphicsDrawPointSet* functions.
 */

#include <mlib_graphics.h>
#include <mlib_ImageRowTable.h>
#include <mlib_GraphicsBlend.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsDrawPointSet_B_32 = \
	__mlib_GraphicsDrawPointSet_B_32
#pragma weak mlib_GraphicsDrawPointSet_B_8 = __mlib_GraphicsDrawPointSet_B_8
#pragma weak mlib_GraphicsDrawPolypoint_B_32 = \
	__mlib_GraphicsDrawPolypoint_B_32
#pragma weak mlib_GraphicsDrawPolypoint_B_8 = \
	__mlib_GraphicsDrawPolypoint_B_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsDrawPointSet_B_32) mlib_GraphicsDrawPointSet_B_32
	__attribute__((weak, alias("__mlib_GraphicsDrawPointSet_B_32")));
__typeof__(__mlib_GraphicsDrawPointSet_B_8) mlib_GraphicsDrawPointSet_B_8
	__attribute__((weak, alias("__mlib_GraphicsDrawPointSet_B_8")));
__typeof__(__mlib_GraphicsDrawPolypoint_B_32) mlib_GraphicsDrawPolypoint_B_32
	__attribute__((weak, alias("__mlib_GraphicsDrawPolypoint_B_32")));
__typeof__(__mlib_GraphicsDrawPolypoint_B_8) mlib_GraphicsDrawPolypoint_B_8
	__attribute__((weak, alias("__mlib_GraphicsDrawPolypoint_B_8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CHECK                                                   \
	if (npoints <= 0)                                       \
	    return (MLIB_FAILURE);                              \
	buf_width = mlib_ImageGetWidth(buffer);                 \
	buf_height = mlib_ImageGetHeight(buffer);               \
	if ((rtable = mlib_ImageGetRowTable(buffer)) == NULL)   \
	    rtable = mlib_ImageCreateRowTable(buffer)

/* *********************************************************** */

#ifdef _LP64
#define	MLIB_POINTER_SHIFT	3
#else /* _LP64 */
#define	MLIB_POINTER_SHIFT	2
#endif /* _LP64 */

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawPointSet_B_8(
	mlib_image *buffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	mlib_s32 c,
	mlib_s32 a)
{
	mlib_u8 **rtable, *data0;
	mlib_s16 *xend;
	mlib_s32 buf_width, buf_height;
	mlib_s32 tx0, ty0;
	mlib_s32 z0, z1;

	MLIB_GRAPHICS_DECL_VAR_B_8

	CHECK;

	MLIB_GRAPHICS_SET_VAR_B_8(c, a)

	buf_width--;
	buf_height--;

	xend = (void *)(x + npoints);

	for (; x < xend; x++, y++) {
		ty0 = *(mlib_s16 *)((mlib_u8 *)y);
		tx0 = *(mlib_s16 *)((mlib_u8 *)x);
		z1 = (buf_height - ty0) | (buf_width - tx0);
		z0 = (ty0 | tx0 | z1) >> 31;
		ty0 |= z0;
		tx0 |= z0;
		data0 = rtable[ty0];
		MLIB_GRAPHICS_BLEND_8(data0 + tx0)
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawPointSet_B_32(
	mlib_image *buffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	mlib_s32 c,
	mlib_s32 a)
{
	mlib_s32 **rtable, *data0;
	mlib_s16 *xend;
	mlib_s32 buf_width, buf_height;
	mlib_s32 tx0, ty0;
	mlib_s32 z0, z1;

	MLIB_GRAPHICS_DECL_VAR_B_32

	CHECK;

	MLIB_GRAPHICS_SET_VAR_B_32(c, a)

	buf_width--;
	buf_height--;

	xend = (void *)(x + npoints);

	for (; x < xend; x++, y++) {
		ty0 = *(mlib_s16 *)((mlib_u8 *)y);
		tx0 = *(mlib_s16 *)((mlib_u8 *)x);
		z1 = (buf_height - ty0) | (buf_width - tx0);
		z0 = (ty0 | tx0 | z1) >> 31;
		ty0 |= z0;
		tx0 |= z0;
		data0 = rtable[ty0];
		MLIB_GRAPHICS_BLEND_32(data0 + tx0)

	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawPolypoint_B_8(
	mlib_image *buffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	mlib_s32 c,
	mlib_s32 a)
{
	return (__mlib_GraphicsDrawPointSet_B_8(buffer, x, y, npoints, c, a));
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawPolypoint_B_32(
	mlib_image *buffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 npoints,
	mlib_s32 c,
	mlib_s32 a)
{
	return (__mlib_GraphicsDrawPointSet_B_32(buffer, x, y, npoints, c, a));
}

/* *********************************************************** */
