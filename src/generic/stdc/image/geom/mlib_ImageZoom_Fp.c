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

#pragma ident	"@(#)mlib_ImageZoom_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoom_Fp - image scaling with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoom_Fp(mlib_image       *dst,
 *                                    const mlib_image *src,
 *                                    mlib_d64         zoomx,
 *                                    mlib_d64         zoomy,
 *                                    mlib_filter      filter,
 *                                    mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image
 *      src       Pointer to source image
 *      zoomx     X zoom factor.
 *      zoomy     Y zoom factor.
 *      filter    Type of resampling filter.
 *      edge      Type of edge condition.
 *
 * DESCRIPTION
 *  The center of the source image is mapped to the center of the
 *  destination image.
 *
 *  The upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *  The resampling filter can be one of the following:
 *    MLIB_NEAREST
 *    MLIB_BILINEAR
 *    MLIB_BICUBIC
 *    MLIB_BICUBIC2
 *
 *  The edge condition can be one of the following:
 *    MLIB_EDGE_DST_NO_WRITE  (default)
 *    MLIB_EDGE_DST_FILL_ZERO
 *    MLIB_EDGE_OP_NEAREST
 *    MLIB_EDGE_SRC_EXTEND
 *    MLIB_EDGE_SRC_PADDED
 *
 * RESTRICTION
 *      src and dst must be the same type and the same number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be in MLIB_FLOAT
 *      or MLIB_DOUBLE data type.
 *
 *      src image can not have width or height larger than 32767.
 */

#include <mlib_image.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageZoom_Fp = __mlib_ImageZoom_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageZoom_Fp) mlib_ImageZoom_Fp
    __attribute__((weak, alias("__mlib_ImageZoom_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	_MLIB_FLOAT_FORMAT_FUNCTION_

#include <mlib_ImageZoom.h>

/* *********************************************************** */

mlib_status
__mlib_ImageZoom_Fp(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 zoomx,
    mlib_d64 zoomy,
    mlib_filter filter,
    mlib_edge edge)
{
	mlib_d64 tx, ty;

	tx = (mlib_ImageGetWidth(dst) - zoomx * mlib_ImageGetWidth(src)) * 0.5;
	ty = (mlib_ImageGetHeight(dst) -
	    zoomy * mlib_ImageGetHeight(src)) * 0.5;

	return __mlib_ImageZoomTranslate_Fp(dst, src, zoomx, zoomy, tx, ty,
	    filter, edge);
}

/* *********************************************************** */
