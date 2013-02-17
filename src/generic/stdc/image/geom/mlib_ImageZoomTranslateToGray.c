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

#pragma ident	"@(#)mlib_ImageZoomTranslateToGray.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomTranslateToGray - image scaling and conversion from binary
 *                                      to grayscale
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoomTranslateToGray(mlib_image       *dst,
 *                                                const mlib_image *src,
 *                                                mlib_d64         zoomx,
 *                                                mlib_d64         zoomy,
 *                                                mlib_d64         tx,
 *                                                mlib_d64         ty,
 *                                                mlib_filter      filter,
 *                                                mlib_edge        edge,
 *                                                const mlib_s32   *ghigh,
 *                                                const mlib_s32   *glow);
 *
 * ARGUMENTS
 *      dst       Pointer to destination image
 *      src       Pointer to source image
 *      zoomx     X zoom factor
 *      zoomy     Y zoom factor
 *      tx        Offset by X coordinate
 *      ty        Offset by Y coordinate
 *      filter    Type of resampling filter
 *      edge      Type of edge condition
 *      ghigh     Pointer to value for '1' pixels in source image
 *      glow      Pointer to value for '0' pixels in source image
 *
 * RESTRICTION
 *  The upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *  The source image must be a one-channeled MLIB_BIT image.
 *  The source image can not have width or height larger than 32767.
 *  The destination image must be a one-channeled MLIB_BYTE image.
 *
 *              zoomx > 0.0
 *              zoomy > 0.0
 *
 *  The resampling filter must be MLIB_NEAREST.
 *
 *  The edge condition can be one of the following:
 *    MLIB_EDGE_DST_NO_WRITE  (default)
 *    MLIB_EDGE_DST_FILL_ZERO
 *    MLIB_EDGE_OP_NEAREST
 *    MLIB_EDGE_SRC_EXTEND
 *    MLIB_EDGE_SRC_PADDED
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageZoom.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageZoomTranslateToGray = __mlib_ImageZoomTranslateToGray

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageZoomTranslateToGray) mlib_ImageZoomTranslateToGray
    __attribute__((weak, alias("__mlib_ImageZoomTranslateToGray")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageZoomTranslateToGray(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 zoomx,
    mlib_d64 zoomy,
    mlib_d64 tx,
    mlib_d64 ty,
    mlib_filter filter,
    mlib_edge edge,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow)
{
	mlib_type stype, dtype;
	mlib_s32 nchan;
	mlib_clipping nearest, current;
	mlib_work_image border;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);

	if (zoomx <= 0 || zoomy <= 0)
		return (MLIB_OUTOFRANGE);

	if (mlib_ImageGetWidth(src) >= (1 << 15) ||
	    mlib_ImageGetHeight(src) >= (1 << 15)) {
		return (MLIB_FAILURE);
	}

	stype = mlib_ImageGetType(src);
	dtype = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(src);

	if (stype != MLIB_BIT || dtype != MLIB_BYTE || nchan != 1 ||
	    filter != MLIB_NEAREST) {
		return (MLIB_FAILURE);
	}

	border.nearest = &nearest;
	border.current = &current;
	mlib_ImageZoomClipping(dst, src, zoomx, zoomy, tx, ty, filter, edge,
	    &border);

	if (current.width > 0) {
		return mlib_ImageZoom_BitToGray_1_Nearest(&border,
		    mlib_ImageGetBitOffset(src), ghigh, glow);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
