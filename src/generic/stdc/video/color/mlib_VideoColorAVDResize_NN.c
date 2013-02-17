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

#pragma ident	"@(#)mlib_VideoColorAVDResize_NN.c	9.2	07/10/09 SMI"

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

#include <mlib_algebra.h>
#include <mlib_VideoColorAVDResize.h>

/* *********************************************************** */

#define	MLIB_SHIFT	15
#define	MLIB_PREC	(1 << MLIB_SHIFT)
#define	MLIB_PREC2	(1 << (MLIB_SHIFT - 1))

/* *********************************************************** */

void
mlib_VideoZoom_U8_4_Nearest(
	mlib_u32 *dst,
	const mlib_u32 *src,
	mlib_s32 dx,
	mlib_s32 dy,
	mlib_s32 dw,
	mlib_s32 dh,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	mlib_s32 i, j, tx, ty, txx, flag = 1;
	const mlib_u32 *src1 = src;
	mlib_u32 *dst1 = dst;

	ty = (dy + 1) >> 1;
	slb >>= 2;
	dlb >>= 2;

	for (i = 0; i < dh; i++, dst1 += dlb) {
		mlib_s32 tty = (ty >> MLIB_SHIFT);

		src1 = src1 + tty * slb;

		if (tty + flag) {
			if (dx != MLIB_PREC) {
				tx = (dx + 1) >> 1;
				txx = ((tx >> MLIB_SHIFT - 2) & ~3);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < dw; j++) {
					tx += dx;
					dst1[j] =
						*(mlib_u32 *)((mlib_u8 *)src1 +
						txx);
					txx = ((tx >> MLIB_SHIFT - 2) & ~3);
				}
			} else {
				__mlib_VectorCopy_S32((mlib_s32 *)dst1,
					(mlib_s32 *)src1, dw);
			}
		} else {
			__mlib_VectorCopy_S32((mlib_s32 *)dst1,
				(mlib_s32 *)(dst1 - dlb), dw);
		}

		flag = 0;
		ty = ty & (MLIB_PREC - 1);
		ty += dy;
	}
}

/* *********************************************************** */

void
mlib_VideoZoom_U8_4_Copy(
	mlib_u32 *dst,
	const mlib_u32 *src,
	mlib_s32 dw,
	mlib_s32 dh,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	if ((dlb == slb) && (slb == (dw << 2))) {
		__mlib_VectorCopy_S32((mlib_s32 *)dst, (mlib_s32 *)src,
			dh * dw);
	} else {
		mlib_s32 i;

		dlb >>= 2;
		slb >>= 2;

		for (i = 0; i < dh; i++, dst += dlb, src += slb) {
			__mlib_VectorCopy_S32((mlib_s32 *)dst, (mlib_s32 *)src,
				dw);
		}
	}
}

/* *********************************************************** */
