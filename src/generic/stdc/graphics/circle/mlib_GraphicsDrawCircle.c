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

#pragma ident	"@(#)mlib_GraphicsDrawCircle.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_GraphicsDrawCircle_[8|32] - draw circle
 *
 * SYNOPSIS
 *      mlib_status mlib_GraphicsDrawCircle_8 (mlib_image *buffer,
 *                                             mlib_s16   x,
 *                                             mlib_s16   y,
 *                                             mlib_s32   r,
 *                                             mlib_s32   c)
 *
 *      mlib_status mlib_GraphicsDrawCircle_32(mlib_image *buffer,
 *                                             mlib_s16   x,
 *                                             mlib_s16   y,
 *                                             mlib_s32   r,
 *                                             mlib_s32   c)
 *
 * ARGUMENTS
 *      buffer  pointer to an dst image
 *      x, y     start point coords
 *      r       radius
 *      c       color
 *
 * DESCRIPTION
 *      Function draws circle with radius r in image pointed by buffer
 *      with color c and center in point x, y.
 *
 */

#include <mlib_graphics.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsDrawCircle_32 = __mlib_GraphicsDrawCircle_32
#pragma weak mlib_GraphicsDrawCircle_8 = __mlib_GraphicsDrawCircle_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsDrawCircle_32) mlib_GraphicsDrawCircle_32
    __attribute__((weak, alias("__mlib_GraphicsDrawCircle_32")));
__typeof__(__mlib_GraphicsDrawCircle_8) mlib_GraphicsDrawCircle_8
    __attribute__((weak, alias("__mlib_GraphicsDrawCircle_8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SET_PIX(DST)	DST = c

/* *********************************************************** */

#define	CHECK_INTERSECTION                                                 \
	if (x - r >= width || x + r < 0 || y - r >= height || y + r < 0)   \
	    return MLIB_SUCCESS

/* *********************************************************** */

#define	PUT_NOCLIP                                              \
	SET_PIX(line0[-cx]);                                    \
	SET_PIX(line0[cx]);                                     \
	SET_PIX(line1[-cy]);                                    \
	SET_PIX(line1[cy]);                                     \
	SET_PIX(line2[-cy]);                                    \
	SET_PIX(line2[cy]);                                     \
	SET_PIX(line3[-cx]);                                    \
	SET_PIX(line3[cx])

/* *********************************************************** */

#define	PUT_NOCLIP_FIRST                                        \
	SET_PIX(line0[0]);                                      \
	if (r == 0)                                             \
	    return (MLIB_SUCCESS);                              \
	SET_PIX(line1[-r]);                                     \
	SET_PIX(line1[r]);                                      \
	SET_PIX(line3[0])

/* *********************************************************** */

#define	PUT_NOCLIP_LAST                                         \
	SET_PIX(line1[-cx]);                                    \
	SET_PIX(line1[cx]);                                     \
	SET_PIX(line2[-cx]);                                    \
	SET_PIX(line2[cx])

/* *********************************************************** */

#define	PUT_CLIP_FIRST                                          \
	if (y - r < height && y - r >= 0) {                     \
	    if (x < width && x >= 0)                            \
		SET_PIX(line0[0]);                              \
	}                                                       \
	if (y < height && y >= 0) {                             \
	    if (x - r < width && x - r >= 0)                    \
		SET_PIX(line1[-r]);                             \
	    if (x + r < width && x + r >= 0)                    \
		SET_PIX(line1[r]);                              \
	}                                                       \
	if (y + r < height && y + r >= 0) {                     \
	    if (x < width && x >= 0)                            \
		SET_PIX(line3[0]);                              \
	}

/* *********************************************************** */

#define	PUT_CLIP_LAST                                           \
	if (y - cx < height && y - cx >= 0) {                   \
	    if (x - cx < width && x - cx >= 0)                  \
		SET_PIX(line1[-cx]);                            \
	    if (x + cx < width && x + cx >= 0)                  \
		SET_PIX(line1[cx]);                             \
	}                                                       \
	if (y + cx < height && y + cx >= 0) {                   \
	    if (x - cx < width && x - cx >= 0)                  \
		SET_PIX(line2[-cx]);                            \
	    if (x + cx < width && x + cx >= 0)                  \
		SET_PIX(line2[cx]);                             \
	}

/* *********************************************************** */

#define	DTYPE	mlib_u8

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawCircle_8(
    mlib_image *buffer,
    mlib_s16 x,
    mlib_s16 y,
    mlib_s32 r,
    mlib_s32 c)
{
	mlib_s32 stride = mlib_ImageGetStride(buffer) / sizeof (DTYPE);
	mlib_s32 width = mlib_ImageGetWidth(buffer);
	mlib_s32 height = mlib_ImageGetHeight(buffer);
	DTYPE *data = mlib_ImageGetData(buffer);
	DTYPE *line0, *line1, *line2, *line3;
	mlib_s32 cx, cy, del, mask;
	mlib_s32 doclip = 0;

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
				SET_PIX(line0[-cx]);
			if ((vy0 | hx1) >= 0)
				SET_PIX(line0[cx]);
			if ((vx0 | hy0) >= 0)
				SET_PIX(line1[-cy]);
			if ((vx0 | hy1) >= 0)
				SET_PIX(line1[cy]);
			if ((vx1 | hy0) >= 0)
				SET_PIX(line2[-cy]);
			if ((vx1 | hy1) >= 0)
				SET_PIX(line2[cy]);
			if ((vy1 | hx0) >= 0)
				SET_PIX(line3[-cx]);
			if ((vy1 | hx1) >= 0)
				SET_PIX(line3[cx]);

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

mlib_status
__mlib_GraphicsDrawCircle_32(
    mlib_image *buffer,
    mlib_s16 x,
    mlib_s16 y,
    mlib_s32 r,
    mlib_s32 c)
{
	mlib_s32 stride = mlib_ImageGetStride(buffer) / sizeof (DTYPE);
	mlib_s32 width = mlib_ImageGetWidth(buffer);
	mlib_s32 height = mlib_ImageGetHeight(buffer);
	DTYPE *data = mlib_ImageGetData(buffer);
	DTYPE *line0, *line1, *line2, *line3;
	mlib_s32 cx, cy, del, mask;
	mlib_s32 doclip = 0;

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
				SET_PIX(line0[-cx]);
			if ((vy0 | hx1) >= 0)
				SET_PIX(line0[cx]);
			if ((vx0 | hy0) >= 0)
				SET_PIX(line1[-cy]);
			if ((vx0 | hy1) >= 0)
				SET_PIX(line1[cy]);
			if ((vx1 | hy0) >= 0)
				SET_PIX(line2[-cy]);
			if ((vx1 | hy1) >= 0)
				SET_PIX(line2[cy]);
			if ((vy1 | hx0) >= 0)
				SET_PIX(line3[-cx]);
			if ((vy1 | hx1) >= 0)
				SET_PIX(line3[cx]);

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
