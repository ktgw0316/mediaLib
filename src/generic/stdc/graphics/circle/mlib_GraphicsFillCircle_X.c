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

#pragma ident	"@(#)mlib_GraphicsFillCircle_X.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_GraphicsFillCircle_X_[8 | 32] - draw filled circle with XOR
 *
 * SYNOPSIS
 *      mlib_status mlib_GraphicsFillCircle_X_8 (mlib_image *buffer,
 *                                               mlib_s16   x,
 *                                               mlib_s16   y,
 *                                               mlib_s32   r,
 *                                               mlib_s32   c,
 *                                               mlib_s32   c2)
 *
 *      mlib_status mlib_GraphicsFillCircle_X_32(mlib_image *buffer,
 *                                               mlib_s16   x,
 *                                               mlib_s16   y,
 *                                               mlib_s32   r,
 *                                               mlib_s32   c,
 *                                               mlib_s32   c2)
 *
 * ARGUMENTS
 *      buffer  pointer to an dst image
 *      x, y     start point coords
 *      r       radius
 *      c, c2    colors
 *
 * DESCRIPTION
 *      Function draw filled circle with radius r in image pointed by buffer
 *      by XORing with colors c and c2 and center in point x, y.
 *
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsFillRow.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsFillCircle_X_32 = __mlib_GraphicsFillCircle_X_32
#pragma weak mlib_GraphicsFillCircle_X_8 = __mlib_GraphicsFillCircle_X_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsFillCircle_X_32) mlib_GraphicsFillCircle_X_32
    __attribute__((weak, alias("__mlib_GraphicsFillCircle_X_32")));
__typeof__(__mlib_GraphicsFillCircle_X_8) mlib_GraphicsFillCircle_X_8
    __attribute__((weak, alias("__mlib_GraphicsFillCircle_X_8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CHECK_INTERSECTION                                                 \
	if (x - r >= width || x + r < 0 || y - r >= height || y + r < 0)   \
	    return MLIB_SUCCESS

/* *********************************************************** */

#define	PUT_CLIP_FIRST(form)                                           \
	{                                                              \
	    if (r < 2)                                                 \
		if (y - r < height && y - r >= 0)                      \
		    if (x < width && x >= 0)                           \
			line0[0] ^= cxor;                              \
	    if (r == 0)                                                \
		return (MLIB_SUCCESS);                                 \
	    if (y < height && y >= 0) {                                \
		xb = x - r;                                            \
		xe = x + r;                                            \
		if (xb < 0)                                            \
		    xb = 0;                                            \
		if (xe >= (width - 1))                                 \
		    xe = width - 1;                                    \
		MLIB_FILL_ROW_X_##form(line1 - x, xb, xe, cxor, dc);   \
	    }                                                          \
	    if (r < 2)                                                 \
		if (y + r < height && y + r >= 0)                      \
		    if (x < width && x >= 0)                           \
			line3[0] ^= cxor;                              \
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

mlib_status
__mlib_GraphicsFillCircle_X_8(
    mlib_image *buffer,
    mlib_s16 xx,
    mlib_s16 yy,
    mlib_s32 r,
    mlib_s32 c0,
    mlib_s32 c2)
{
	mlib_s32 stride = mlib_ImageGetStride(buffer);
	mlib_s32 width = mlib_ImageGetWidth(buffer);
	mlib_s32 height = mlib_ImageGetHeight(buffer);
	mlib_u8 *data = mlib_ImageGetData(buffer);
	mlib_u8 *line0, *line1, *line2, *line3;
	mlib_s32 cx, cy, del, mask;
	mlib_s32 doclip = 0;
	mlib_s32 x = xx, y = yy;
	mlib_s32 cxor = c0 ^ c2;
	mlib_s32 xb, xe;
	mlib_d64 dc;

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

	cxor &= 0xff;
	cxor |= (cxor << 8);
	cxor |= (cxor << 16);
	DOUBLE_FROM_INT(dc, cxor);

	if (doclip) {
		PUT_CLIP_FIRST(8);
	} else {
		if (r < 2)
			line0[0] ^= cxor;

		if (r == 0)
			return (MLIB_SUCCESS);
		MLIB_FILL_ROW_X_8(line1 - r, 0, 2 * r, cxor, dc);

		if (r < 2)
			line3[0] ^= cxor;
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

			if (vx0 >= 0)
				MLIB_FILL_ROW_X_8(line1 - x, xb, xe, cxor, dc);

			if (vx1 >= 0)
				MLIB_FILL_ROW_X_8(line2 - x, xb, xe, cxor, dc);

			mask = del >> 31;

			if (!mask) {
				LEFT(x - cx, xb);
				RIGHT(x + cx, xe);

				if (vy0 >= 0)
					MLIB_FILL_ROW_X_8(line0 - x, xb, xe,
					    cxor, dc);

				if (vy1 >= 0)
					MLIB_FILL_ROW_X_8(line3 - x, xb, xe,
					    cxor, dc);
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
				MLIB_FILL_ROW_X_8(line0 - x, xb, xe, cxor, dc);

			if (vy1 >= 0)
				MLIB_FILL_ROW_X_8(line3 - x, xb, xe, cxor, dc);
		} else if (cx == cy) {
			xb = x - cx;
			xe = x + cx;

			if (xb < 0)
				xb = 0;

			if (xe >= (width - 1))
				xe = width - 1;

			if (y - cx < height && y - cx >= 0)
				MLIB_FILL_ROW_X_8(line1 - x, xb, xe, cxor, dc);

			if (y + cx < height && y + cx >= 0)
				MLIB_FILL_ROW_X_8(line2 - x, xb, xe, cxor, dc);
		}
	} else {

		while (cx < cy) {

			MLIB_FILL_ROW_X_8(line1 - cy, 0, 2 * cy, cxor, dc);
			MLIB_FILL_ROW_X_8(line2 - cy, 0, 2 * cy, cxor, dc);

			del += (2 * cx + 1) - ((2 * cy) & ~mask);

			mask = del >> 31;

			if (!mask) {
				MLIB_FILL_ROW_X_8(line0 - cx, 0, 2 * cx, cxor,
				    dc);
				MLIB_FILL_ROW_X_8(line3 - cx, 0, 2 * cx, cxor,
				    dc);
			}

			cy += ~mask;
			++cx;
			line1 -= stride;
			line2 += stride;
			line0 += stride & ~mask;
			line3 -= stride & ~mask;
		}

		if (mask) {
			MLIB_FILL_ROW_X_8(line0 - cx + 1, 0, 2 * cx - 2, cxor,
			    dc);
			MLIB_FILL_ROW_X_8(line3 - cx + 1, 0, 2 * cx - 2, cxor,
			    dc);
		} else if (cx == cy) {
			MLIB_FILL_ROW_X_8(line1 - cx, 0, 2 * cx, cxor, dc);
			MLIB_FILL_ROW_X_8(line2 - cx, 0, 2 * cx, cxor, dc);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsFillCircle_X_32(
    mlib_image *buffer,
    mlib_s16 xx,
    mlib_s16 yy,
    mlib_s32 r,
    mlib_s32 c0,
    mlib_s32 c2)
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
	mlib_s32 cxor = c0 ^ c2;
	mlib_s32 xb, xe;
	mlib_d64 dc;

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

	DOUBLE_FROM_INT(dc, cxor);

	if (doclip) {
		PUT_CLIP_FIRST(32);
	} else {
		if (r < 2)
			line0[0] ^= cxor;

		if (r == 0)
			return (MLIB_SUCCESS);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		MLIB_FILL_ROW_X_32(line1 - r, 0, 2 * r, cxor, dc);

		if (r < 2)
			line3[0] ^= cxor;
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
				MLIB_FILL_ROW_X_32(line1 - x, xb, xe, cxor, dc);

			if (vx1 >= 0)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				MLIB_FILL_ROW_X_32(line2 - x, xb, xe, cxor, dc);

			mask = del >> 31;

			if (!mask) {
				LEFT(x - cx, xb);
				RIGHT(x + cx, xe);

				if (vy0 >= 0)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					MLIB_FILL_ROW_X_32(line0 - x, xb, xe,
					    cxor, dc);

				if (vy1 >= 0)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					MLIB_FILL_ROW_X_32(line3 - x, xb, xe,
					    cxor, dc);
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
				MLIB_FILL_ROW_X_32(line0 - x, xb, xe, cxor, dc);

			if (vy1 >= 0)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				MLIB_FILL_ROW_X_32(line3 - x, xb, xe, cxor, dc);
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
				MLIB_FILL_ROW_X_32(line1 - x, xb, xe, cxor, dc);

			if (y + cx < height && y + cx >= 0)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				MLIB_FILL_ROW_X_32(line2 - x, xb, xe, cxor, dc);
		}
	} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		while (cx < cy) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			MLIB_FILL_ROW_X_32(line1 - cy, 0, 2 * cy, cxor, dc);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			MLIB_FILL_ROW_X_32(line2 - cy, 0, 2 * cy, cxor, dc);

			del += (4 * cx + 2) - ((4 * cy) & ~mask);

			mask = del >> 31;

			if (!mask) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				MLIB_FILL_ROW_X_32(line0 - cx, 0, 2 * cx, cxor,
				    dc);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				MLIB_FILL_ROW_X_32(line3 - cx, 0, 2 * cx, cxor,
				    dc);
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
			MLIB_FILL_ROW_X_32(line0 - cx + 1, 0, 2 * cx - 2, cxor,
			    dc);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			MLIB_FILL_ROW_X_32(line3 - cx + 1, 0, 2 * cx - 2, cxor,
			    dc);
		} else if (cx == cy) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			MLIB_FILL_ROW_X_32(line1 - cx, 0, 2 * cx, cxor, dc);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			MLIB_FILL_ROW_X_32(line2 - cx, 0, 2 * cx, cxor, dc);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
