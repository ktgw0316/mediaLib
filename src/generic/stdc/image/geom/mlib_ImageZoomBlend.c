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

#pragma ident	"@(#)mlib_ImageZoomBlend.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomBlend - image scaling with alpha blending
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoomBlend(mlib_image       *dst,
 *                                      const mlib_image *src,
 *                                      mlib_d64         zoomx,
 *                                      mlib_d64         zoomy,
 *                                      mlib_filter      filter,
 *                                      mlib_edge        edge,
 *                                      mlib_blend       blend,
 *                                      mlib_s32         alpha,
 *                                      mlib_s32         cmask);
 *
 * ARGUMENTS
 *      dst       Pointer to the destination image.
 *      src       Pointer to the source image.
 *      zoomx     X zoom factor.
 *      zoomy     Y zoom factor.
 *      filter    Type of resampling filter.
 *      edge      Type of edge condition.
 *      blend     Type of alpha blending.
 *      alpha     The overall alpha for blending.
 *      cmask     The channel mask to indicate the alpha channel.
 *
 * DESCRIPTION
 *      Zoom an image in or out, and blend it with another image.
 *
 *      The center of the source image is mapped to the center of the
 *      destination image.
 *
 *      The center of the upper - left corner pixel of an image is considered
 *      to be located at (0.5, 0.5).
 *
 *      The resampling filter can be one of the following:
 *          MLIB_NEAREST
 *          MLIB_BILINEAR
 *          MLIB_BICUBIC
 *          MLIB_BICUBIC2
 *
 *      The edge condition can be one of the following:
 *          MLIB_EDGE_DST_NO_WRITE  (default)
 *          MLIB_EDGE_DST_FILL_ZERO
 *          MLIB_EDGE_OP_NEAREST
 *          MLIB_EDGE_SRC_EXTEND
 *          MLIB_EDGE_SRC_PADDED
 *
 *      The type of alpha blending can be one of the following:
 *          MLIB_BLEND_GTK_SRC
 *          MLIB_BLEND_GTK_SRC_OVER
 *          MLIB_BLEND_GTK_SRC_OVER2
 *
 *      This function is a special case of mlib_ImageZoomTranslateBlend, so
 *      it can be implemented by simply calling the latter.
 *
 * RESTRICTION
 *      Both src and dst must be of type MLIB_BYTE.
 *      They can have either 3 or 4 channels.
 *      The cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *
 *      The src image can not have width or height larger than 32767.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageZoomBlend = __mlib_ImageZoomBlend

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageZoomBlend) mlib_ImageZoomBlend
	__attribute__((weak, alias("__mlib_ImageZoomBlend")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageZoomBlend(
	mlib_image *dst,
	const mlib_image *src,
	mlib_d64 zoomx,
	mlib_d64 zoomy,
	mlib_filter filter,
	mlib_edge edge,
	mlib_blend blend,
	mlib_s32 alpha,
	mlib_s32 cmask)
{
	mlib_d64 tx, ty;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);

	tx = (mlib_ImageGetWidth(dst) - zoomx * mlib_ImageGetWidth(src)) * 0.5;
	ty = (mlib_ImageGetHeight(dst) -
		zoomy * mlib_ImageGetHeight(src)) * 0.5;

	return __mlib_ImageZoomTranslateBlend(dst, src, zoomx, zoomy, tx, ty,
		filter, edge, blend, alpha, cmask);
}

/* *********************************************************** */
