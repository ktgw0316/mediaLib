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

#pragma ident	"@(#)mlib_GraphicsFillRectangle_B.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_GraphicsFillRectangle_B_[8|32] - draw filled rectangle
 *        with alpha blending
 *
 * SYNOPSIS
 *    mlib_status mlib_GraphicsFillRectangle_B_[8|32]
 *                                          (mlib_image *buffer,
 *                                           mlib_s16   x,
 *                                           mlib_s16   y,
 *                                           mlib_s32   w,
 *                                           mlib_s32   h,
 *                                           mlib_s32   c,
 *                                           mlib_s32   a);
 *
 * ARGUMENTS
 *    buffer  Pointer to the image which the function is drawing into.
 *    x       X coordinate of the upper left corner of the rectangle.
 *    y       Y coordinate of the upper left corner of the rectangle.
 *    w       Width of the rectangle.
 *    h       Height of the rectangle.
 *    c       Color used in the drawing.
 *    a       Alpha value for the blending.
 *
 *  DESCRIPTION
 *    The drawable buffer is either MLIB_BYTE(8-bit) or MLIB_INT(32-bit)
 *    type single channel mediaLib image. Pixels falling outside the
 *    drawable buffer will be clipped and discarded.
 *    All colors are specified in 32-bit format. For 8-bit buffer, only
 *    the bits 0-7 are used and the bits 8-23 should be zero. For 32-bit
 *    frame buffer, the bits 24-31 are reserved for alpha channel and
 *    should be 0xff.
 *    In alpha value, only the bits 0-7 are used and the bits 8-23 should
 *    be zero.  For each drawable pixel, the original pixel value is
 *    blended with the drawing color to produce the final pixel value
 *
 *        d = (s * (255 - a) + c * a) / 255
 *
 *    where c is the color used in the drawing, a is the alpha value, s
 *    is the original value of the pixel, and d is the final value of the
 *    pixel.
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsBlend.h>
#include <mlib_GraphicsFillRow.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsFillRectangle_B_32 = \
	__mlib_GraphicsFillRectangle_B_32
#pragma weak mlib_GraphicsFillRectangle_B_8 = \
	__mlib_GraphicsFillRectangle_B_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsFillRectangle_B_32) mlib_GraphicsFillRectangle_B_32
	__attribute__((weak, alias("__mlib_GraphicsFillRectangle_B_32")));
__typeof__(__mlib_GraphicsFillRectangle_B_8) mlib_GraphicsFillRectangle_B_8
	__attribute__((weak, alias("__mlib_GraphicsFillRectangle_B_8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillRectangle_B_8(
	mlib_image *buffer,
	mlib_s16 xx,
	mlib_s16 yy,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 c,
	mlib_s32 a)
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

	MLIB_GRAPHICS_DECL_VAR_B_8

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

	MLIB_GRAPHICS_SET_VAR_B_8(c, a)

	for (j = 0; j < h; j++) {
		MLIB_FILL_ROW_B_8_START(pdst + (y + j) * stride, x, x + w - 1,
				ca, a_1, vca, va_1)
#ifdef __SUNPRO_C
#if MLIB_VIS >= 0x200
#pragma unroll(3)
#elif MLIB_VIS
#pragma pipeloop(0)
#else
#pragma unroll(4)
#endif
#endif /* __SUNPRO_C */
		MLIB_FILL_ROW_B_8_END
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillRectangle_B_32(
	mlib_image *buffer,
	mlib_s16 xx,
	mlib_s16 yy,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 c,
	mlib_s32 a)
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
	mlib_s32 *_pdst_row;

	MLIB_GRAPHICS_DECL_VAR_B_32

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

	MLIB_GRAPHICS_SET_VAR_B_32(c, a)

	_pdst_row = pdst + y * stride + x;

	for (j = 0; j < h; j++) {
		mlib_s32 _i, _w = w - 1;
#ifdef __SUNPRO_C
#if MLIB_VIS >= 0x200
#pragma pipeloop(0)
#elif MLIB_VIS
#pragma unroll(8)
#else
#pragma unroll(6)
#endif
#endif /* __SUNPRO_C */
		for (_i = 0; _i <= _w; _i++)
			MLIB_GRAPHICS_BLEND_32(_pdst_row + _i)

		_pdst_row += stride;

	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
