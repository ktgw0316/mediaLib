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

#pragma ident	"@(#)mlib_GraphicsFillRectangle_X.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_GraphicsFillRectangle_X_[8|32] - draw filled rectangle in XOR mode
 *
 * SYNOPSIS
 *    mlib_status mlib_GraphicsFillRectangle_X_[8|32]
 *                                          (mlib_image *buffer,
 *                                           mlib_s16   x,
 *                                           mlib_s16   y,
 *                                           mlib_s32   w,
 *                                           mlib_s32   h,
 *                                           mlib_s32   c,
 *                                           mlib_s32   c2);
 *
 * ARGUMENTS
 *    buffer  Pointer to the image which the function is drawing into.
 *    x       X coordinate of the upper left corner of the rectangle.
 *    y       Y coordinate of the upper left corner of the rectangle.
 *    w       Width of the rectangle.
 *    h       Height of the rectangle.
 *    c       Color used in the drawing.
 *    c2      The second color.
 *
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsFillRow.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsFillRectangle_X_32 = \
	__mlib_GraphicsFillRectangle_X_32
#pragma weak mlib_GraphicsFillRectangle_X_8 = \
	__mlib_GraphicsFillRectangle_X_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsFillRectangle_X_32) mlib_GraphicsFillRectangle_X_32
	__attribute__((weak, alias("__mlib_GraphicsFillRectangle_X_32")));
__typeof__(__mlib_GraphicsFillRectangle_X_8) mlib_GraphicsFillRectangle_X_8
	__attribute__((weak, alias("__mlib_GraphicsFillRectangle_X_8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillRectangle_X_8(
	mlib_image *buffer,
	mlib_s16 xx,
	mlib_s16 yy,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 c,
	mlib_s32 c2)
{
/* pointer to the data of dst-image */
	mlib_u8 *pdst = mlib_ImageGetData(buffer);

/* height of destination image */
	mlib_s32 height = mlib_ImageGetHeight(buffer);

/* width of destination image */
	mlib_s32 width = mlib_ImageGetWidth(buffer);

/* elements to next row */
	mlib_s32 stride = mlib_ImageGetStride(buffer);
	mlib_s32 j;
	mlib_s32 x = xx, y = yy;
	mlib_s32 cx = c ^ c2;
	mlib_d64 dc;

	if ((x + w) <= 0 || (y + h) <= 0 || x >= width || y >= height || w <= 0)
		return (MLIB_SUCCESS);

	if (x < 0) {
		w += x;
		x = 0;
	}

	if (y < 0) {
		h += y;
		y = 0;
	}

	if ((x + w) > width)
		w = width - x;

	if ((y + h) > height)
		h = height - y;

	cx &= 0xff;
	cx |= (cx << 8);
	cx |= (cx << 16);
	DOUBLE_FROM_INT(dc, cx);

	for (j = 0; j < h; j++) {
		MLIB_FILL_ROW_X_8(pdst + (y + j) * stride, x, x + w - 1, cx,
			dc);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillRectangle_X_32(
	mlib_image *buffer,
	mlib_s16 xx,
	mlib_s16 yy,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 c,
	mlib_s32 c2)
{
/* pointer to the data of dst-image */
	mlib_s32 *pdst = mlib_ImageGetData(buffer);

/* height of destination image */
	mlib_s32 height = mlib_ImageGetHeight(buffer);

/* width of destination image */
	mlib_s32 width = mlib_ImageGetWidth(buffer);

/* elements to next row */
	mlib_s32 stride = mlib_ImageGetStride(buffer) / 4;
	mlib_s32 j;
	mlib_s32 x = xx, y = yy;
	mlib_s32 cx = c ^ c2;
	mlib_d64 dc;

	if ((x + w) <= 0 || (y + h) <= 0 || x >= width || y >= height || w <= 0)
		return (MLIB_SUCCESS);

	if (x < 0) {
		w += x;
		x = 0;
	}

	if (y < 0) {
		h += y;
		y = 0;
	}

	if ((x + w) > width)
		w = width - x;

	if ((y + h) > height)
		h = height - y;

	DOUBLE_FROM_INT(dc, cx);

	for (j = 0; j < h; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		MLIB_FILL_ROW_X_32(pdst + (y + j) * stride, x, x + w - 1, cx,
			dc);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */