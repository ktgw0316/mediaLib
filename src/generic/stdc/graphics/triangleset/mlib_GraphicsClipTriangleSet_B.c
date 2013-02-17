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

#pragma ident	"@(#)mlib_GraphicsClipTriangleSet_B.c	9.2	07/10/09 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsClipTriangleSet_B    - clipping function for simple modes
 *						with alpha blending
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsClipTriangleSet_B
 *                                        (mlib_u8  **rtable,
 *                                         mlib_s32 width,
 *                                         mlib_s32 height,
 *                                         mlib_s32 stride,
 *                                         mlib_s16 *x,
 *                                         mlib_s16 *y,
 *                                         mlib_s32 nlines,
 *                                         mlib_s32 dp0,
 *                                         mlib_s32 dp1,
 *                                         mlib_s32 c,
 *                                         mlib_s32 a);
 *
 *  ARGUMENTS
 *    rtable      Pointer to array of starts of row, rtable[-1][-1] is buffer
 *                  for unwanted stores
 *    width       Image.Width - 1, is using to discover whether clipping needed
 *    height      Image.Height - 1, is using to discover whether clipping needed
 *    x           Pointer to array of X coordinate of the points
 *    y           Pointer to array of Y coordinate of the points
 *    nlines      Number of lines
 *    dp0         Step from (x0, y0) to the (x0, y0) of the next line
 *    dp1         Step from (x1, y1) to the (x1, y1) of the next line,
 *                for example
 *                  DrawTriangleSet_B       dp0 = 3, dp1 = 3
 *                  DrawTriangleFanSet    dp0 = 0, dp1 = 1
 *                  DrawTriangleStripSet  dp0 = 1, dp1 = 1
 *    c           Color to draw, for MLIB_BYTE type of image color must be
 *                  the same in all bytes :
 *                    c = (color & 0xFF),
 *                    c |= (c << 8),
 *                    c |= (c << 16)
 *    a           Alpha value for the blending.
 *    stride      Stride of the image
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>

/* *********************************************************** */

#define	NB	512

/* *********************************************************** */

#define	NB3	510

/* *********************************************************** */

void
mlib_GraphicsClipTriangleSet_B(
	mlib_u8 **rtable,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 stride,
	mlib_s16 *x,
	mlib_s16 *y,
	mlib_s32 nlines,
	mlib_s32 dp0,
	mlib_s32 dp1,
	mlib_s32 c,
	mlib_s32 a,
	void (*SpamLineSet) ())
{
	mlib_s16 xl[2 * NB], yl[2 * NB];
	mlib_s16 *px0 = x, *px1 = x + 1, *py0 = y, *py1 = y + 1;
	mlib_s32 i, all, k = 0;

	all = nlines;

	if (dp0 != 3) {
		xl[0] = *px0;
		xl[1] = *px1;
		yl[0] = *py0;
		yl[1] = *py1;
		k = 2;
	}

	while (all > 0) {

		if (dp0 != 3) {
			nlines = (all > (NB - 2)) ? (NB - 2) : all;
			all -= nlines;

			for (i = 0; i < nlines; i += 2) {
				mlib_s32 x0 = *px0, x1 = *px1, x2 = px1[1];
				mlib_s32 y0 = *py0, y1 = *py1, y2 = py1[1];

				px0 += dp0;
				px1 += dp1;
				py0 += dp0;
				py1 += dp1;

				xl[2 * i + 0 + k] = x1;
				xl[2 * i + 1 + k] = x2;
				xl[2 * i + 2 + k] = x2;
				xl[2 * i + 3 + k] = x0;

				yl[2 * i + 0 + k] = y1;
				yl[2 * i + 1 + k] = y2;
				yl[2 * i + 2 + k] = y2;
				yl[2 * i + 3 + k] = y0;
			}
		} else {
			nlines = (all > NB3) ? (NB3) : all;
			all -= nlines;

			for (i = 0; i < nlines; i += 3) {
				mlib_s32 x0 = *px0, x1 = *px1, x2 = px1[1];
				mlib_s32 y0 = *py0, y1 = *py1, y2 = py1[1];

				px0 += dp0;
				px1 += dp1;
				py0 += dp0;
				py1 += dp1;

				xl[2 * i] = x0;
				xl[2 * i + 1] = x1;
				xl[2 * i + 2] = x1;
				xl[2 * i + 3] = x2;
				xl[2 * i + 4] = x2;
				xl[2 * i + 5] = x0;

				yl[2 * i] = y0;
				yl[2 * i + 1] = y1;
				yl[2 * i + 2] = y1;
				yl[2 * i + 3] = y2;
				yl[2 * i + 4] = y2;
				yl[2 * i + 5] = y0;
			}
		}

		SpamLineSet(rtable, width, height, stride, xl, yl,
			nlines + (k >> 1), 2, 2, c, a);

		k = 0;
	}
}

/* *********************************************************** */
