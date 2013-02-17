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

#pragma ident	"@(#)mlib_GraphicsDrawCircle_B.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *    mlib_GraphicsDrawCircle_B_[8|32] - draw circle with alpha blending
 *
 * SYNOPSIS
 *    mlib_status mlib_GraphicsDrawCircle_B_8 (mlib_image  *buffer,
 *                                             mlib_s16    x,
 *                                             mlib_s16    y,
 *                                             mlib_s32    r,
 *                                             mlib_s32    c,
 *                                             mlib_s32    a)
 *
 *    mlib_status mlib_GraphicsDrawCircle_B_32(mlib_image  *buffer,
 *                                             mlib_s16    x,
 *                                             mlib_s16    y,
 *                                             mlib_s32    r,
 *                                             mlib_s32    c,
 *                                             mlib_s32    a)
 *
 * ARGUMENTS
 *    buffer  Pointer to the image which the function is drawing into.
 *    x       X coordinate of the circle center.
 *    y       Y coordinate of the circle center.
 *    r       Circle radius.
 *    c       Color used in the drawing.
 *    a       Alpha value for the blending.
 *
 * DESCRIPTION
 *    Function draws a circle of radius r and center (x, y)
 *    in image pointed by buffer, by alpha blending with color c,
 *    alpha value a.
 *
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
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsBlend.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsDrawCircle_B_8 = __mlib_GraphicsDrawCircle_B_8
#pragma weak mlib_GraphicsDrawCircle_B_32 = __mlib_GraphicsDrawCircle_B_32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsDrawCircle_B_8) mlib_GraphicsDrawCircle_B_8
    __attribute__((weak, alias("__mlib_GraphicsDrawCircle_B_8")));
__typeof__(__mlib_GraphicsDrawCircle_B_32) mlib_GraphicsDrawCircle_B_32
    __attribute__((weak, alias("__mlib_GraphicsDrawCircle_B_32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CHECK_INTERSECTION                                                 \
	if (x - r >= width || x + r < 0 || y - r >= height || y + r < 0)   \
	    return MLIB_SUCCESS

/* *********************************************************** */

#define	PUT_NOCLIP                                              \
	SET_PIX(line0 - cx);                                    \
	SET_PIX(line0 + cx);                                    \
	SET_PIX(line1 - cy);                                    \
	SET_PIX(line1 + cy);                                    \
	SET_PIX(line2 - cy);                                    \
	SET_PIX(line2 + cy);                                    \
	SET_PIX(line3 - cx);                                    \
	SET_PIX(line3 + cx)

/* *********************************************************** */

#define	PUT_NOCLIP_FIRST                                        \
	SET_PIX(line0);                                         \
	if (r == 0)                                             \
	    return (MLIB_SUCCESS);                              \
	SET_PIX(line1 - r);                                     \
	SET_PIX(line1 + r);                                     \
	SET_PIX(line3)

/* *********************************************************** */

#define	PUT_NOCLIP_LAST                                         \
	SET_PIX(line1 - cx);                                    \
	SET_PIX(line1 + cx);                                    \
	SET_PIX(line2 - cx);                                    \
	SET_PIX(line2 + cx)

/* *********************************************************** */

#define	PUT_CLIP_FIRST                                          \
	if (y - r < height && y - r >= 0) {                     \
	    if (x < width && x >= 0)                            \
		SET_PIX(line0);                                 \
	}                                                       \
	if (y < height && y >= 0) {                             \
	    if (x - r < width && x - r >= 0)                    \
		SET_PIX(line1 - r);                             \
	    if (x + r < width && x + r >= 0)                    \
		SET_PIX(line1 + r);                             \
	}                                                       \
	if (y + r < height && y + r >= 0) {                     \
	    if (x < width && x >= 0)                            \
		SET_PIX(line3);                                 \
	}

/* *********************************************************** */

#define	PUT_CLIP_LAST                                           \
	if (y - cx < height && y - cx >= 0) {                   \
	    if (x - cx < width && x - cx >= 0)                  \
		SET_PIX(line1 - cx);                            \
	    if (x + cx < width && x + cx >= 0)                  \
		SET_PIX(line1 + cx);                            \
	}                                                       \
	if (y + cx < height && y + cx >= 0) {                   \
	    if (x - cx < width && x - cx >= 0)                  \
		SET_PIX(line2 - cx);                            \
	    if (x + cx < width && x + cx >= 0)                  \
		SET_PIX(line2 + cx);                            \
	}

/* *********************************************************** */

#define	DTYPE	mlib_u8

/* *********************************************************** */

#define	SET_PIX(DST) MLIB_GRAPHICS_BLEND_8(DST)

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawCircle_B_8(
    mlib_image *buffer,
    mlib_s16 x,
    mlib_s16 y,
    mlib_s32 r,
    mlib_s32 c,
    mlib_s32 a)
{
	mlib_s32 stride = mlib_ImageGetStride(buffer) / sizeof (DTYPE);
	mlib_s32 width = mlib_ImageGetWidth(buffer);
	mlib_s32 height = mlib_ImageGetHeight(buffer);
	DTYPE *data = mlib_ImageGetData(buffer);
	DTYPE *line0, *line1, *line2, *line3;
	mlib_s32 cx, cy, del, mask;
	mlib_s32 doclip = 0;

	MLIB_GRAPHICS_DECL_VAR_B_8

	if (!data)
		return (MLIB_NULLPOINTER);

	if (r < 0)
		return (MLIB_FAILURE);
	CHECK_INTERSECTION;

	MLIB_GRAPHICS_SET_VAR_B_8(c, a)

	if (x + r >= width || x - r < 0 || y + r >= height || y - r < 0)
		doclip = 1;

	line1 = line2 = data + stride * y + x;
	line0 = line2 - stride * r;
	line3 = line2 + stride * r;

	if (doclip) {
		PUT_CLIP_FIRST;
	} else {
		PUT_NOCLIP_FIRST;
	}

	if (r < 2)
		return (MLIB_SUCCESS);

	cy = r;
	del = 1 - r;
	mask = -1;
	cx = 1;
	line1 -= stride;
	line2 += stride;

	if (doclip) {

		mlib_s32 hx0 = (x - 1) | ((width - 1) - (x - 1));
		mlib_s32 hx1 = (x + 1) | ((width - 1) - (x + 1));
		mlib_s32 hy0 = (x - r) | ((width - 1) - (x - r));
		mlib_s32 hy1 = (x + r) | ((width - 1) - (x + r));
		mlib_s32 vy0 = (y - r) | ((height - 1) - (y - r));
		mlib_s32 vx0 = (y - 1) | ((height - 1) - (y - 1));
		mlib_s32 vx1 = (y + 1) | ((height - 1) - (y + 1));
		mlib_s32 vy1 = (y + r) | ((height - 1) - (y + r));

		while (cx < cy) {

			del += (2 * cx + 1) - ((2 * cy) & ~mask);

			if ((vy0 | hx0) >= 0)
				SET_PIX(line0 - cx);
			if ((vy0 | hx1) >= 0)
				SET_PIX(line0 + cx);
			if ((vx0 | hy0) >= 0)
				SET_PIX(line1 - cy);
			if ((vx0 | hy1) >= 0)
				SET_PIX(line1 + cy);
			if ((vx1 | hy0) >= 0)
				SET_PIX(line2 - cy);
			if ((vx1 | hy1) >= 0)
				SET_PIX(line2 + cy);
			if ((vy1 | hx0) >= 0)
				SET_PIX(line3 - cx);
			if ((vy1 | hx1) >= 0)
				SET_PIX(line3 + cx);

			mask = del >> 31;
			cy += ~mask;
			++cx;
			line1 -= stride;
			line2 += stride;
			line0 += stride & ~mask;
			line3 -= stride & ~mask;

			hx0 = (x - cx) | ((width - 1) - (x - cx));
			hx1 = (x + cx) | ((width - 1) - (x + cx));

			vx0 = (y - cx) | ((height - 1) - (y - cx));
			vx1 = (y + cx) | ((height - 1) - (y + cx));

			if (!mask) {
				hy0 = (x - cy) | ((width - 1) - (x - cy));
				hy1 = (x + cy) | ((width - 1) - (x + cy));

				vy0 = (y - cy) | ((height - 1) - (y - cy));
				vy1 = (y + cy) | ((height - 1) - (y + cy));
			}
		}

		if (cx == cy && !mask) {
			PUT_CLIP_LAST;
		}
	} else {

		while (cx < cy) {

			PUT_NOCLIP;

			del += (2 * cx + 1) - ((2 * cy) & ~mask);

			mask = del >> 31;
			cy += ~mask;
			++cx;
			line1 -= stride;
			line2 += stride;
			line0 += stride & ~mask;
			line3 -= stride & ~mask;
		}

		if (cx == cy && !mask) {
			PUT_NOCLIP_LAST;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  DTYPE
#define	DTYPE	mlib_u32

/* *********************************************************** */

#undef  SET_PIX
#define	SET_PIX(DST) MLIB_GRAPHICS_BLEND_32(DST)

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawCircle_B_32(
    mlib_image *buffer,
    mlib_s16 x,
    mlib_s16 y,
    mlib_s32 r,
    mlib_s32 c,
    mlib_s32 a)
{
	mlib_s32 stride = mlib_ImageGetStride(buffer) / sizeof (DTYPE);
	mlib_s32 width = mlib_ImageGetWidth(buffer);
	mlib_s32 height = mlib_ImageGetHeight(buffer);
	DTYPE *data = mlib_ImageGetData(buffer);
	DTYPE *line0, *line1, *line2, *line3;
	mlib_s32 cx, cy, del, mask;
	mlib_s32 doclip = 0;

	MLIB_GRAPHICS_DECL_VAR_B_32

	if (!data)
		return (MLIB_NULLPOINTER);

	if (r < 0)
		return (MLIB_FAILURE);
	CHECK_INTERSECTION;

	MLIB_GRAPHICS_SET_VAR_B_32(c, a)

	if (x + r >= width || x - r < 0 || y + r >= height || y - r < 0)
		doclip = 1;

	line1 = line2 = data + stride * y + x;
	line0 = line2 - stride * r;
	line3 = line2 + stride * r;

	if (doclip) {
		PUT_CLIP_FIRST;
	} else {
		PUT_NOCLIP_FIRST;
	}

	if (r < 2)
		return (MLIB_SUCCESS);

	cy = r;
	del = 1 - r;
	mask = -1;
	cx = 1;
	line1 -= stride;
	line2 += stride;

	if (doclip) {

		mlib_s32 hx0 = (x - 1) | ((width - 1) - (x - 1));
		mlib_s32 hx1 = (x + 1) | ((width - 1) - (x + 1));
		mlib_s32 hy0 = (x - r) | ((width - 1) - (x - r));
		mlib_s32 hy1 = (x + r) | ((width - 1) - (x + r));
		mlib_s32 vy0 = (y - r) | ((height - 1) - (y - r));
		mlib_s32 vx0 = (y - 1) | ((height - 1) - (y - 1));
		mlib_s32 vx1 = (y + 1) | ((height - 1) - (y + 1));
		mlib_s32 vy1 = (y + r) | ((height - 1) - (y + r));

		while (cx < cy) {

			del += (2 * cx + 1) - ((2 * cy) & ~mask);

			if ((vy0 | hx0) >= 0)
				SET_PIX(line0 - cx);
			if ((vy0 | hx1) >= 0)
				SET_PIX(line0 + cx);
			if ((vx0 | hy0) >= 0)
				SET_PIX(line1 - cy);
			if ((vx0 | hy1) >= 0)
				SET_PIX(line1 + cy);
			if ((vx1 | hy0) >= 0)
				SET_PIX(line2 - cy);
			if ((vx1 | hy1) >= 0)
				SET_PIX(line2 + cy);
			if ((vy1 | hx0) >= 0)
				SET_PIX(line3 - cx);
			if ((vy1 | hx1) >= 0)
				SET_PIX(line3 + cx);

			mask = del >> 31;
			cy += ~mask;
			++cx;
			line1 -= stride;
			line2 += stride;
			line0 += stride & ~mask;
			line3 -= stride & ~mask;

			hx0 = (x - cx) | ((width - 1) - (x - cx));
			hx1 = (x + cx) | ((width - 1) - (x + cx));

			vx0 = (y - cx) | ((height - 1) - (y - cx));
			vx1 = (y + cx) | ((height - 1) - (y + cx));

			if (!mask) {
				hy0 = (x - cy) | ((width - 1) - (x - cy));
				hy1 = (x + cy) | ((width - 1) - (x + cy));

				vy0 = (y - cy) | ((height - 1) - (y - cy));
				vy1 = (y + cy) | ((height - 1) - (y + cy));
			}
		}

		if (cx == cy && !mask) {
			PUT_CLIP_LAST;
		}
	} else {

		while (cx < cy) {

			PUT_NOCLIP;

			del += (2 * cx + 1) - ((2 * cy) & ~mask);

			mask = del >> 31;
			cy += ~mask;
			++cx;
			line1 -= stride;
			line2 += stride;
			line0 += stride & ~mask;
			line3 -= stride & ~mask;
		}

		if (cx == cy && !mask) {
			PUT_NOCLIP_LAST;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
