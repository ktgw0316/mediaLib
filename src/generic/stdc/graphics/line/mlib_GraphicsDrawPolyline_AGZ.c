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

#pragma ident	"@(#)mlib_GraphicsDrawPolyline_AGZ.c	9.2	07/10/09 SMI"

/*
 *  FUNCTION
 *    mlib_GraphicsDraw[Line[|Fan|Strip]Set|Polyline]_AGZ_[8|32]
 *      - draw corresponding set of lines in corresponding mode :
 *        LineSet      - set of lines connecting ((x1, y1), (x2, y2)),
 *          ((x3, y3), (x4, y4)) ..., and ((x(n-1), y(n-1)), (xn, yn))
 *        LineFanSet   - set of lines connecting ((x1, y1), (x2, y2)),
 *          ((x1, y1), (x3, y3)) ..., and ((x1, y1), (xn, yn))
 *        LineStripSet -  set of lines connecting ((x1, y1), (x2, y2)),
 *          ((x2, y2), (x3, y3) ..., and ((x(n-1), y(n-1)), (xn, yn))
 *        Polyline     -  same with LineStripSet
 *
 *  SYNOPSIS
 *    mlib_GraphicsDraw[Line[|Fan|Strip]Set|Polyline]_AGZ_[8|32]
 *                                                   (mlib_image     *buffer,
 *                                                    mlib_image     *zbuffer,
 *                                                    const mlib_s16 *x,
 *                                                    const mlib_s16 *y,
 *                                                    const mlib_s16 *z,
 *                                                    mlib_s32       npoints,
 *                                                    const mlib_s32 *c);
 *  ARGUMENTS
 *    buffer  Pointer to the image which the function is drawing into.
 *    zbuffer Pointer to the image which holds the Z-buffer.
 *    x       Pointer to array of X coordinate of the points.
 *    y       Pointer to array of Y coordinate of the points.
 *    z       Pointer to array of Z coordinate of the points.
 *    npoints Number of points in the arrays.
 *    c       Pointer to array of color of points.
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

#pragma weak mlib_GraphicsDrawLineStripSet_AGZ_8 = \
	__mlib_GraphicsDrawLineStripSet_AGZ_8
#pragma weak mlib_GraphicsDrawLineFanSet_AGZ_32 = \
	__mlib_GraphicsDrawLineFanSet_AGZ_32
#pragma weak mlib_GraphicsDrawPolyline_AGZ_32 = \
	__mlib_GraphicsDrawPolyline_AGZ_32
#pragma weak mlib_GraphicsDrawLineFanSet_AGZ_8 = \
	__mlib_GraphicsDrawLineFanSet_AGZ_8
#pragma weak mlib_GraphicsDrawLineSet_AGZ_32 = \
	__mlib_GraphicsDrawLineSet_AGZ_32
#pragma weak mlib_GraphicsDrawLineStripSet_AGZ_32 = \
	__mlib_GraphicsDrawLineStripSet_AGZ_32
#pragma weak mlib_GraphicsDrawPolyline_AGZ_8 = \
	__mlib_GraphicsDrawPolyline_AGZ_8
#pragma weak mlib_GraphicsDrawLineSet_AGZ_8 = \
	__mlib_GraphicsDrawLineSet_AGZ_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsDrawLineStripSet_AGZ_8)
	mlib_GraphicsDrawLineStripSet_AGZ_8
	__attribute__((weak, alias("__mlib_GraphicsDrawLineStripSet_AGZ_8")));
__typeof__(__mlib_GraphicsDrawLineFanSet_AGZ_32)
	mlib_GraphicsDrawLineFanSet_AGZ_32
	__attribute__((weak, alias("__mlib_GraphicsDrawLineFanSet_AGZ_32")));
__typeof__(__mlib_GraphicsDrawPolyline_AGZ_32) mlib_GraphicsDrawPolyline_AGZ_32
	__attribute__((weak, alias("__mlib_GraphicsDrawPolyline_AGZ_32")));
__typeof__(__mlib_GraphicsDrawLineFanSet_AGZ_8)
	mlib_GraphicsDrawLineFanSet_AGZ_8
	__attribute__((weak, alias("__mlib_GraphicsDrawLineFanSet_AGZ_8")));
__typeof__(__mlib_GraphicsDrawLineSet_AGZ_32) mlib_GraphicsDrawLineSet_AGZ_32
	__attribute__((weak, alias("__mlib_GraphicsDrawLineSet_AGZ_32")));
__typeof__(__mlib_GraphicsDrawLineStripSet_AGZ_32)
	mlib_GraphicsDrawLineStripSet_AGZ_32
	__attribute__((weak, alias("__mlib_GraphicsDrawLineStripSet_AGZ_32")));
__typeof__(__mlib_GraphicsDrawPolyline_AGZ_8) mlib_GraphicsDrawPolyline_AGZ_8
	__attribute__((weak, alias("__mlib_GraphicsDrawPolyline_AGZ_8")));
__typeof__(__mlib_GraphicsDrawLineSet_AGZ_8) mlib_GraphicsDrawLineSet_AGZ_8
	__attribute__((weak, alias("__mlib_GraphicsDrawLineSet_AGZ_8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_GDL_COMPLECT_AGZ(N, SUF, DP0, DP1)                   \
	MLIB_GRAPHICS_DECL_VARZ;                                  \
	MLIB_GRAPHICS_DECL_##SUF;                                 \
	MLIB_GRAPHICS_CHECKZ(MLIB_GD_CHECK_XYZG,                  \
		MLIB_GD_CHECK_##SUF);                             \
	mlib_GraphicsClipLineSet_GZ_##N(rtable, zrtable, width,   \
		height, stride, zstride, (void *)x, (void *)y,    \
		(void *)z, (void *)c, nlines, DP0, DP1,           \
		mlib_GraphicsSpamLineSet_AGZ_##N);                \
	return MLIB_SUCCESS

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawLineSet_AGZ_8(
	mlib_image *buffer,
	mlib_image *zbuffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *z,
	mlib_s32 npoints,
	const mlib_s32 *c)
{
	MLIB_GDL_COMPLECT_AGZ(8, LS, 2, 2);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawLineSet_AGZ_32(
	mlib_image *buffer,
	mlib_image *zbuffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *z,
	mlib_s32 npoints,
	const mlib_s32 *c)
{
	MLIB_GDL_COMPLECT_AGZ(32, LS, 2, 2);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawLineFanSet_AGZ_8(
	mlib_image *buffer,
	mlib_image *zbuffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *z,
	mlib_s32 npoints,
	const mlib_s32 *c)
{
	MLIB_GDL_COMPLECT_AGZ(8, LFS, 0, 1);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawLineFanSet_AGZ_32(
	mlib_image *buffer,
	mlib_image *zbuffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *z,
	mlib_s32 npoints,
	const mlib_s32 *c)
{
	MLIB_GDL_COMPLECT_AGZ(32, LFS, 0, 1);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawLineStripSet_AGZ_8(
	mlib_image *buffer,
	mlib_image *zbuffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *z,
	mlib_s32 npoints,
	const mlib_s32 *c)
{
	MLIB_GDL_COMPLECT_AGZ(8, LSS, 1, 1);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawLineStripSet_AGZ_32(
	mlib_image *buffer,
	mlib_image *zbuffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *z,
	mlib_s32 npoints,
	const mlib_s32 *c)
{
	MLIB_GDL_COMPLECT_AGZ(32, LSS, 1, 1);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawPolyline_AGZ_8(
	mlib_image *buffer,
	mlib_image *zbuffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *z,
	mlib_s32 npoints,
	const mlib_s32 *c)
{
	MLIB_GDL_COMPLECT_AGZ(8, LSS, 1, 1);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawPolyline_AGZ_32(
	mlib_image *buffer,
	mlib_image *zbuffer,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *z,
	mlib_s32 npoints,
	const mlib_s32 *c)
{
	MLIB_GDL_COMPLECT_AGZ(32, LSS, 1, 1);
}

/* *********************************************************** */
