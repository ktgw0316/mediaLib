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

#pragma ident	"@(#)mlib_VideoColorAVDResize.c	9.2	07/10/09 SMI"

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

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorResizeABGR = __mlib_VideoColorResizeABGR

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorResizeABGR) mlib_VideoColorResizeABGR
	__attribute__((weak, alias("__mlib_VideoColorResizeABGR")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_SHIFT	15
#define	MLIB_PREC	(1 << MLIB_SHIFT)
#define	MLIB_MASK	(MLIB_PREC - 1)

/* *********************************************************** */

void
__mlib_VideoColorResizeABGR(
	mlib_u32 *dst,
	const mlib_u32 *src,
	mlib_s32 dw,
	mlib_s32 dh,
	mlib_s32 dlb,
	mlib_s32 sw,
	mlib_s32 sh,
	mlib_s32 slb,
	mlib_filter filter)
{
	if (dw == sw && dh == sh) {
		mlib_VideoZoom_U8_4_Copy(dst, src, dw, dh, dlb, slb);
	} else {
		mlib_s32 dx, dy;

		dx = ((sw << MLIB_SHIFT) + ((dw + 1) >> 1)) / dw;
		dy = ((sh << MLIB_SHIFT) + ((dh + 1) >> 1)) / dh;

		if ((((dx | dy) & MLIB_MASK) <= 1) &&
			(((dx & dy) & MLIB_PREC) != 0)) {
			filter = MLIB_NEAREST;
		}

		switch (filter) {
		    case MLIB_NEAREST:
			    mlib_VideoZoom_U8_4_Nearest(dst, src, dx, dy, dw,
				    dh, dlb, slb);
			    break;
		    case MLIB_BILINEAR:
			    mlib_VideoZoom_U8_4_Bilinear(dst, src, dx, dy, dw,
				    dh, dlb, sw, sh, slb);
			    break;
		    case MLIB_BICUBIC:
			    mlib_VideoZoom_U8_4_Bicubic(dst, src, dx, dy, dw,
				    dh, dlb, sw, sh, slb);
		}
	}
}

/* *********************************************************** */
