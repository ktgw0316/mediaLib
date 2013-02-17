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

#pragma ident	"@(#)mlib_c_VideoColorAVDResize_BL.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorResizeABGR - image resize
 *
 * SYNOPSIS
 *     void mlib_VideoColorResizeABGR(mlib_u32       *dst,
 *                                    const mlib_u32 *src,
 *                                    mlib_s32       dst_width,
 *                                    mlib_s32       dst_height,
 *                                    mlib_s32       dst_stride,
 *                                    mlib_s32       src_width,
 *                                    mlib_s32       src_height,
 *                                    mlib_s32       src_stride,
 *                                    mlib_filter    filter)
 *
 * ARGUMENTS
 *      dst         pointer to output image
 *      src         pointer to input image
 *      dst_stride  output image width in pixels
 *      dst_height  output image height in rows
 *      dst_stride  linebytes for output image
 *      src_width   input image width in pixels
 *      src_height  input image height in lines
 *      src_stride  linebytes for input image
 *      filter      type of interpolation filter
 *
 * DESCRIPTION
 *      Function performs the resize source image with dimensions src_width,
 *      src_height into the destination image with dimensions dst_width,
 *      dst_height using nearest-neighbor, bilinear interpolation, or bicubic
 *      interpolation. Edge conditions are handled according to "source extend"
 *      handling scheme. The interpolation filter can be one of the following:
 *        MLIB_NEAREST
 *        MLIB_BILINEAR
 *        MLIB_BICUBIC
 */

#include <mlib_video.h>
#include <mlib_VideoColorAVDResize.h>
#include <mlib_ImageFilters.h>
#include <mlib_ImageDivTables.h>
#include <mlib_c_ImageBlendTable.h>

/* *********************************************************** */

#define	MLIB_SHIFT	15
#define	MLIB_PREC	(1 << MLIB_SHIFT)
#define	MLIB_PREC2	(1 << (MLIB_SHIFT - 1))
#define	MLIB_MASK	(MLIB_PREC - 1)

/* *********************************************************** */

#define	_MLIB_C_VER_

/* *********************************************************** */

#define	TABLE(A)	mlib_U82F32[(A)]
#define	FTABLE(A)	mlib_c_blend_Q8[(A)]

/* *********************************************************** */

#define	LOAD(N, index)                                          \
	a0##N = TABLE(tsp[cx + N]);                             \
	a3##N = TABLE(tsp2[cx + N]);                            \
	a1##N = TABLE(tsp[cx + index]);                         \
	a2##N = TABLE(tsp2[cx + index])

/* *********************************************************** */

#define	CALC(N)                                                 \
	r0##N = a0##N + fdy * (a3##N - a0##N);                  \
	r1##N = a1##N + fdy * (a2##N - a1##N)

/* *********************************************************** */

#define	RES(N)	res##N = (INT_FORMAT) (r0##N + fdx * (r1##N - r0##N))

/* *********************************************************** */

#define	STOR(N, index)	dp[index] = (mlib_u8)(res##N)

/* *********************************************************** */

#define	VARIABLE_BL(N)                                          \
	FLOAT_FORMAT a0##N, a1##N, a2##N, a3##N,                \
		r0##N, r1##N;                                   \
	INT_FORMAT res##N

/* *********************************************************** */

#define	FLOAT_FORMAT	mlib_f32
#define	INT_FORMAT	mlib_s32

/* *********************************************************** */

void
mlib_VideoZoom_U8_4_Bilinear(
	mlib_u32 *dst,
	const mlib_u32 *src,
	mlib_s32 dx,
	mlib_s32 dy,
	mlib_s32 dw,
	mlib_s32 dh,
	mlib_s32 dst_stride,
	mlib_s32 sw,
	mlib_s32 sh,
	mlib_s32 src_stride)
{
	mlib_s32 srcX = ((dx + 1) >> 1) - MLIB_PREC2;
	mlib_s32 srcY = ((dy + 1) >> 1) - MLIB_PREC2;
	mlib_s32 sw2 = 4 * (sw - 1);
	mlib_u8 *sp = (mlib_u8 *)src, *dp = (mlib_u8 *)dst;
	mlib_f32 fdx, fdy;
	mlib_s32 i, j, cx, x = srcX, y = srcY;

	VARIABLE_BL(0);
	VARIABLE_BL(1);
	VARIABLE_BL(2);
	VARIABLE_BL(3);
	mlib_u8 *tsp = sp;
	mlib_u8 *tsp2;

	sw = (sw - 1) << MLIB_SHIFT;

	for (j = 0; j < dh; j++) {
		mlib_s32 row = (y >> MLIB_SHIFT);

		x = srcX;
		fdy = FTABLE((y >> (MLIB_SHIFT - 8)) & 0xFF);

		if (row < 0) {
			tsp = sp;
			tsp2 = tsp;
		} else if (row >= sh - 1) {
			tsp = sp + (sh - 1) * src_stride;
			tsp2 = tsp;
		} else {
			tsp = sp + row * src_stride;
			tsp2 = tsp + src_stride;
		}

		cx = 0;
		i = 0;

		while (x < 0 && i < dw) {
			fdx = FTABLE((x >> (MLIB_SHIFT - 8)) & 0xFF);

			LOAD(0, 0);
			LOAD(1, 1);
			LOAD(2, 2);
			LOAD(3, 3);

			CALC(0);
			CALC(1);
			CALC(2);
			CALC(3);

			RES(0);
			RES(1);
			RES(2);
			RES(3);

			STOR(0, 4 * i);
			STOR(1, 4 * i + 1);
			STOR(2, 4 * i + 2);
			STOR(3, 4 * i + 3);
			i++;
			x += dx;
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; x < sw; i++, x += dx) {

			cx = (x >> MLIB_SHIFT) * 4;
			fdx = FTABLE((x >> (MLIB_SHIFT - 8)) & 0xFF);

			LOAD(0, 4);
			LOAD(1, 5);
			LOAD(2, 6);
			LOAD(3, 7);

			CALC(0);
			CALC(1);
			CALC(2);
			CALC(3);

			RES(0);
			RES(1);
			RES(2);
			RES(3);

			STOR(0, 4 * i);
			STOR(1, 4 * i + 1);
			STOR(2, 4 * i + 2);
			STOR(3, 4 * i + 3);
		}

		cx = sw2;

		while (i < dw) {
			fdx = FTABLE((x >> (MLIB_SHIFT - 8)) & 0xFF);

			LOAD(0, 0);
			LOAD(1, 1);
			LOAD(2, 2);
			LOAD(3, 3);

			CALC(0);
			CALC(1);
			CALC(2);
			CALC(3);

			RES(0);
			RES(1);
			RES(2);
			RES(3);

			STOR(0, 4 * i);
			STOR(1, 4 * i + 1);
			STOR(2, 4 * i + 2);
			STOR(3, 4 * i + 3);
			i++;
			x += dx;
		}

		y += dy;
		dp += dst_stride;
	}
}

/* *********************************************************** */
