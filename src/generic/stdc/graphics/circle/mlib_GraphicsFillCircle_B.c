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

#pragma ident	"@(#)mlib_GraphicsFillCircle_B.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *    mlib_GraphicsFillCircle_B_[8 | 32] - draw filled circle
 *                                         with alpha blending
 *
 * SYNOPSIS
 *    mlib_status mlib_GraphicsFillCircle_B_8 (mlib_image *buffer,
 *                                             mlib_s16   x,
 *                                             mlib_s16   y,
 *                                             mlib_s32   r,
 *                                             mlib_s32   c,
 *                                             mlib_s32   a)
 *
 *    mlib_status mlib_GraphicsFillCircle_B_32(mlib_image *buffer,
 *                                             mlib_s16   x,
 *                                             mlib_s16   y,
 *                                             mlib_s32   r,
 *                                             mlib_s32   c,
 *                                             mlib_s32   a)
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
 *    Function fills a circle of radius r and center (x, y)
 *    in image pointed by buffer by blending with color c,
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
#include <mlib_GraphicsFillRow.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsFillCircle_B_32 = __mlib_GraphicsFillCircle_B_32
#pragma weak mlib_GraphicsFillCircle_B_8 = __mlib_GraphicsFillCircle_B_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsFillCircle_B_32) mlib_GraphicsFillCircle_B_32
    __attribute__((weak, alias("__mlib_GraphicsFillCircle_B_32")));
__typeof__(__mlib_GraphicsFillCircle_B_8) mlib_GraphicsFillCircle_B_8
    __attribute__((weak, alias("__mlib_GraphicsFillCircle_B_8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CHECK_INTERSECTION                                                 \
	if (x - r >= width || x + r < 0 || y - r >= height || y + r < 0)   \
	    return MLIB_SUCCESS

/* *********************************************************** */

#define	PUT_CLIP_FIRST_32                                                  \
	{                                                                  \
	    if (r < 2)                                                     \
		if (y - r < height && y - r >= 0)                          \
		    if (x < width && x >= 0)                               \
			MLIB_GRAPHICS_BLEND_32(line0)                      \
	    if (r == 0)                                                    \
		return (MLIB_SUCCESS);                                     \
	    if (y < height && y >= 0) {                                    \
		xb = x - r;                                                \
		xe = x + r;                                                \
		if (xb < 0)                                                \
		    xb = 0;                                                \
		if (xe >= (width - 1))                                     \
		    xe = width - 1;                                        \
		MLIB_FILL_ROW_B_32(line1 - x, xb, xe, ca1, ca2, ca3, a_1,  \
				vca, va_1)				   \
	    }                                                              \
	    if (r < 2)                                                     \
		if (y + r < height && y + r >= 0)                          \
		    if (x < width && x >= 0)                               \
			MLIB_GRAPHICS_BLEND_32(line3)                      \
	}

/* *********************************************************** */

#define	LEFT(src, dst)	dst = (src) & ~((src) >> 31)

/* *********************************************************** */

#define	RIGHT(src, dst)                                         \
	{                                                       \
	    mlib_s32 dl = (src) - (width - 1);                  \
	                                                        \
	    dst = (dl & (dl >> 31)) + (width - 1);              \
	}

/* *********************************************************** */

#ifdef MLIB_VIS

#define	UNROLL_FAC 3

#else

#define	UNROLL_FAC 2

#endif

mlib_status
__mlib_GraphicsFillCircle_B_8(
    mlib_image *buffer,
    mlib_s16 xx,
    mlib_s16 yy,
    mlib_s32 r,
    mlib_s32 c,
    mlib_s32 a)
{
	mlib_s32 stride = mlib_ImageGetStride(buffer);
	mlib_s32 width = mlib_ImageGetWidth(buffer);
	mlib_s32 height = mlib_ImageGetHeight(buffer);
	mlib_u8 *data = mlib_ImageGetData(buffer);
	mlib_u8 *line0, *line1, *line2, *line3;
	mlib_s32 cx, cy, del, mask;
	mlib_s32 doclip = 0;
	mlib_s32 x = xx, y = yy;
	mlib_s32 xb, xe;

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

	if (doclip) { /* PUT_CLIP_FIRST_8 */

		if (r < 2)
			if (y - r < height && y - r >= 0)
				if (x < width && x >= 0)
					MLIB_GRAPHICS_BLEND_8(line0)
		if (r == 0)
			return (MLIB_SUCCESS);
		if (y < height && y >= 0) {
			xb = x - r;
			xe = x + r;
			if (xb < 0)
				xb = 0;
			if (xe >= (width - 1))
				xe = width - 1;

			MLIB_FILL_ROW_B_8_START(line1 - x, xb, xe, ca, a_1,
				vca, va_1)
#ifdef __SUNPRO_C
#pragma unroll(UNROLL_FAC)
#endif /* __SUNPRO_C */
			MLIB_FILL_ROW_B_8_END

		}
		if (r < 2)
			if (y + r < height && y + r >= 0)
				if (x < width && x >= 0)
					MLIB_GRAPHICS_BLEND_8(line3)

	} else {
		if (r < 2)
			MLIB_GRAPHICS_BLEND_8(line0);

		if (r == 0)
			return (MLIB_SUCCESS);

		MLIB_FILL_ROW_B_8_START(line1 - r, 0, 2 * r, ca, a_1,
				vca, va_1)
#ifdef __SUNPRO_C
#pragma unroll(UNROLL_FAC)
#endif /* __SUNPRO_C */
		MLIB_FILL_ROW_B_8_END

		if (r < 2)
			MLIB_GRAPHICS_BLEND_8(line3)
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

		mlib_s32 vy0 = (y - r) | ((height - 1) - (y - r));
		mlib_s32 vx0 = (y - 1) | ((height - 1) - (y - 1));
		mlib_s32 vx1 = (y + 1) | ((height - 1) - (y + 1));
		mlib_s32 vy1 = (y + r) | ((height - 1) - (y + r));

		while (cx < cy) {

			del += (2 * cx + 1) - ((2 * cy) & ~mask);

			LEFT(x - cy, xb);
			RIGHT(x + cy, xe);

			if (vx0 >= 0) {
				MLIB_FILL_ROW_B_8_START(line1 - x, xb, xe,
					ca, a_1, vca, va_1)
#ifdef __SUNPRO_C
#pragma unroll(UNROLL_FAC)
#endif /* __SUNPRO_C */
				MLIB_FILL_ROW_B_8_END
			}

			if (vx1 >= 0) {
				MLIB_FILL_ROW_B_8_START(line2 - x, xb, xe,
					ca, a_1, vca, va_1)
#ifdef __SUNPRO_C
#pragma unroll(UNROLL_FAC)
#endif /* __SUNPRO_C */
				MLIB_FILL_ROW_B_8_END
			}

			mask = del >> 31;

			if (!mask) {
				LEFT(x - cx, xb);
				RIGHT(x + cx, xe);

				if (vy0 >= 0) {
					MLIB_FILL_ROW_B_8_START(line0 - x,
						xb, xe, ca, a_1, vca, va_1)
#ifdef __SUNPRO_C
#pragma unroll(UNROLL_FAC)
#endif /* __SUNPRO_C */
					MLIB_FILL_ROW_B_8_END
				}

				if (vy1 >= 0) {
					MLIB_FILL_ROW_B_8_START(line3 - x,
						xb, xe, ca, a_1, vca, va_1)
#ifdef __SUNPRO_C
#pragma unroll(UNROLL_FAC)
#endif /* __SUNPRO_C */
					MLIB_FILL_ROW_B_8_END
				}
			}

			cy += ~mask;
			++cx;
			line1 -= stride;
			line2 += stride;
			line0 += stride & ~mask;
			line3 -= stride & ~mask;

			vx0 = (y - cx) | ((height - 1) - (y - cx));
			vx1 = (y + cx) | ((height - 1) - (y + cx));

			if (!mask) {
				vy0 = (y - cy) | ((height - 1) - (y - cy));
				vy1 = (y + cy) | ((height - 1) - (y + cy));
			}
		}

		if (mask) {
			LEFT(x - (cx - 1), xb);
			RIGHT(x + (cx - 1), xe);

			if (vy0 >= 0) {
				MLIB_FILL_ROW_B_8_START(line0 - x,
					xb, xe, ca, a_1, vca, va_1)
#ifdef __SUNPRO_C
#pragma unroll(UNROLL_FAC)
#endif /* __SUNPRO_C */
				MLIB_FILL_ROW_B_8_END
			}

			if (vy1 >= 0) {
				MLIB_FILL_ROW_B_8_START(line3 - x,
					xb, xe, ca, a_1, vca, va_1)
#ifdef __SUNPRO_C
#pragma unroll(UNROLL_FAC)
#endif /* __SUNPRO_C */
				MLIB_FILL_ROW_B_8_END
			}
		} else if (cx == cy) {
			xb = x - cx;
			xe = x + cx;

			if (xb < 0)
				xb = 0;

			if (xe >= (width - 1))
				xe = width - 1;

			if (y - cx < height && y - cx >= 0) {
				MLIB_FILL_ROW_B_8_START(line1 - x,
					xb, xe, ca, a_1, vca, va_1)
#ifdef __SUNPRO_C
#pragma unroll(UNROLL_FAC)
#endif /* __SUNPRO_C */
				MLIB_FILL_ROW_B_8_END
			}

			if (y + cx < height && y + cx >= 0) {
				MLIB_FILL_ROW_B_8_START(line2 - x,
					xb, xe, ca, a_1, vca, va_1)
#ifdef __SUNPRO_C
#pragma unroll(UNROLL_FAC)
#endif /* __SUNPRO_C */
				MLIB_FILL_ROW_B_8_END
			}
		}
	} else {

		while (cx < cy) {

			MLIB_FILL_ROW_B_8_START(line1 - cy, 0, 2 * cy, ca, a_1,
					vca, va_1)
#ifdef __SUNPRO_C
#pragma unroll(UNROLL_FAC)
#endif /* __SUNPRO_C */
			MLIB_FILL_ROW_B_8_END

			MLIB_FILL_ROW_B_8_START(line2 - cy, 0, 2 * cy, ca, a_1,
					vca, va_1)
#ifdef __SUNPRO_C
#pragma unroll(UNROLL_FAC)
#endif /* __SUNPRO_C */
			MLIB_FILL_ROW_B_8_END

			del += (2 * cx + 1) - ((2 * cy) & ~mask);

			mask = del >> 31;

			if (!mask) {
				MLIB_FILL_ROW_B_8_START(line0 - cx, 0, 2 * cx,
					ca, a_1, vca, va_1)
#ifdef __SUNPRO_C
#pragma unroll(UNROLL_FAC)
#endif /* __SUNPRO_C */
				MLIB_FILL_ROW_B_8_END

				MLIB_FILL_ROW_B_8_START(line3 - cx, 0, 2 * cx,
					ca, a_1, vca, va_1)
#ifdef __SUNPRO_C
#pragma unroll(UNROLL_FAC)
#endif /* __SUNPRO_C */
				MLIB_FILL_ROW_B_8_END
			}

			cy += ~mask;
			++cx;
			line1 -= stride;
			line2 += stride;
			line0 += stride & ~mask;
			line3 -= stride & ~mask;
		}

		if (mask) {
			MLIB_FILL_ROW_B_8_START(line0 - cx + 1, 0, 2 * cx - 2,
				ca, a_1, vca, va_1)
#ifdef __SUNPRO_C
#pragma unroll(UNROLL_FAC)
#endif /* __SUNPRO_C */
			MLIB_FILL_ROW_B_8_END

			MLIB_FILL_ROW_B_8_START(line3 - cx + 1, 0, 2 * cx - 2,
				ca, a_1, vca, va_1)
#ifdef __SUNPRO_C
#pragma unroll(UNROLL_FAC)
#endif /* __SUNPRO_C */
			MLIB_FILL_ROW_B_8_END
		} else if (cx == cy) {
			MLIB_FILL_ROW_B_8_START(line1 - cx, 0, 2 * cx, ca, a_1,
					vca, va_1)
#ifdef __SUNPRO_C
#pragma unroll(UNROLL_FAC)
#endif /* __SUNPRO_C */
			MLIB_FILL_ROW_B_8_END

			MLIB_FILL_ROW_B_8_START(line2 - cx, 0, 2 * cx, ca, a_1,
					vca, va_1)
#ifdef __SUNPRO_C
#pragma unroll(UNROLL_FAC)
#endif /* __SUNPRO_C */
			MLIB_FILL_ROW_B_8_END
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillCircle_B_32(
    mlib_image *buffer,
    mlib_s16 xx,
    mlib_s16 yy,
    mlib_s32 r,
    mlib_s32 c,
    mlib_s32 a)
{
	mlib_s32 stride4 = mlib_ImageGetStride(buffer);
	mlib_s32 stride = stride4 / sizeof (mlib_s32);
	mlib_s32 width = mlib_ImageGetWidth(buffer);
	mlib_s32 height = mlib_ImageGetHeight(buffer);
	mlib_s32 *data = mlib_ImageGetData(buffer);
	mlib_s32 *line0, *line1, *line2, *line3;
	mlib_s32 cx, cy, del, mask;
	mlib_s32 doclip = 0;
	mlib_s32 x = xx, y = yy;
	mlib_s32 xb, xe;

	MLIB_GRAPHICS_DECL_VAR_B_32

	if (!data)
		return (MLIB_NULLPOINTER);

	if (r < 0)
		return (MLIB_FAILURE);
	CHECK_INTERSECTION;

	if (x + r >= width || x - r < 0 || y + r >= height || y - r < 0)
		doclip = 1;

	line1 = line2 = data + stride * y + x;
	line0 = line2 - stride * r;
	line3 = line2 + stride * r;

	MLIB_GRAPHICS_SET_VAR_B_32(c, a)

	if (doclip) {
		PUT_CLIP_FIRST_32
	} else {
		if (r < 2)
			MLIB_GRAPHICS_BLEND_32(line0)

		if (r == 0)
			return (MLIB_SUCCESS);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		MLIB_FILL_ROW_B_32(line1 - r, 0, 2 * r, ca1, ca2, ca3, a_1,
				vca, va_1);

		if (r < 2)
			MLIB_GRAPHICS_BLEND_32(line3)
	}

	if (r < 2)
		return (MLIB_SUCCESS);

	cy = r;
	del = 2 - 2 * r;
	mask = -1;
	cx = 1;
	line1 -= stride;
	line2 += stride;

	if (doclip) {

		mlib_s32 vy0 = (y - r) | ((height - 1) - (y - r));
		mlib_s32 vx0 = (y - 1) | ((height - 1) - (y - 1));
		mlib_s32 vx1 = (y + 1) | ((height - 1) - (y + 1));
		mlib_s32 vy1 = (y + r) | ((height - 1) - (y + r));

		while (cx < cy) {

			del += (4 * cx + 2) - ((4 * cy) & ~mask);

			LEFT(x - cy, xb);
			RIGHT(x + cy, xe);

			if (vx0 >= 0)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				MLIB_FILL_ROW_B_32(line1 - x, xb, xe, ca1,
						ca2, ca3, a_1, vca, va_1);

			if (vx1 >= 0)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				MLIB_FILL_ROW_B_32(line2 - x, xb, xe, ca1,
						ca2, ca3, a_1, vca, va_1);

			mask = del >> 31;

			if (!mask) {
				LEFT(x - cx, xb);
				RIGHT(x + cx, xe);

				if (vy0 >= 0)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					MLIB_FILL_ROW_B_32(line0 - x, xb, xe,
					    ca1, ca2, ca3, a_1, vca, va_1);

				if (vy1 >= 0)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					MLIB_FILL_ROW_B_32(line3 - x, xb, xe,
					    ca1, ca2, ca3, a_1, vca, va_1);
			}

			cy += ~mask;
			++cx;
			line1 -= stride;
			line2 += stride;
			line0 += stride & ~mask;
			line3 -= stride & ~mask;

			vx0 = (y - cx) | ((height - 1) - (y - cx));
			vx1 = (y + cx) | ((height - 1) - (y + cx));

			if (!mask) {
				vy0 = (y - cy) | ((height - 1) - (y - cy));
				vy1 = (y + cy) | ((height - 1) - (y + cy));
			}
		}

		if (mask) {
			LEFT(x - (cx - 1), xb);
			RIGHT(x + (cx - 1), xe);

			if (vy0 >= 0)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				MLIB_FILL_ROW_B_32(line0 - x, xb, xe, ca1,
						ca2, ca3, a_1, vca, va_1);

			if (vy1 >= 0)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				MLIB_FILL_ROW_B_32(line3 - x, xb, xe, ca1,
						ca2, ca3, a_1, vca, va_1);
		} else if (cx == cy) {
			xb = x - cx;
			xe = x + cx;

			if (xb < 0)
				xb = 0;

			if (xe >= (width - 1))
				xe = width - 1;

			if (y - cx < height && y - cx >= 0)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				MLIB_FILL_ROW_B_32(line1 - x, xb, xe, ca1,
						ca2, ca3, a_1, vca, va_1);

			if (y + cx < height && y + cx >= 0)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				MLIB_FILL_ROW_B_32(line2 - x, xb, xe, ca1,
						ca2, ca3, a_1, vca, va_1);
		}
	} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		while (cx < cy) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			MLIB_FILL_ROW_B_32(line1 - cy, 0, 2 * cy, ca1, ca2,
					ca3, a_1, vca, va_1);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			MLIB_FILL_ROW_B_32(line2 - cy, 0, 2 * cy, ca1, ca2,
					ca3, a_1, vca, va_1);

			del += (4 * cx + 2) - ((4 * cy) & ~mask);

			mask = del >> 31;

			if (!mask) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				MLIB_FILL_ROW_B_32(line0 - cx, 0, 2 * cx, ca1,
						ca2, ca3, a_1, vca, va_1);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				MLIB_FILL_ROW_B_32(line3 - cx, 0, 2 * cx, ca1,
						ca2, ca3, a_1, vca, va_1);
			}

			cy += ~mask;
			++cx;
			line1 = (mlib_s32 *)((mlib_u8 *)line1 - stride4);
			line2 = (mlib_s32 *)((mlib_u8 *)line2 + stride4);
			line0 =
			    (mlib_s32 *)((mlib_u8 *)line0 + (stride4 & ~mask));
			line3 =
			    (mlib_s32 *)((mlib_u8 *)line3 - (stride4 & ~mask));
		}

		if (mask) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			MLIB_FILL_ROW_B_32(line0 - cx + 1, 0, 2 * cx - 2, ca1,
					ca2, ca3, a_1, vca, va_1);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			MLIB_FILL_ROW_B_32(line3 - cx + 1, 0, 2 * cx - 2, ca1,
					ca2, ca3, a_1, vca, va_1);
		} else if (cx == cy) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			MLIB_FILL_ROW_B_32(line1 - cx, 0, 2 * cx, ca1,
					ca2, ca3, a_1, vca, va_1);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			MLIB_FILL_ROW_B_32(line2 - cx, 0, 2 * cx, ca1,
					ca2, ca3, a_1, vca, va_1);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
