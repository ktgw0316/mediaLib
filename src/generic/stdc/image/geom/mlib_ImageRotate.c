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

#pragma ident	"@(#)mlib_ImageRotate.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageRotate - image rotation with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageRotate(mlib_image         *dst,
 *                                   const mlib_image   *src,
 *                                   mlib_d64           angle,
 *                                   mlib_d64           xcenter,
 *                                   mlib_d64           ycenter,
 *                                   mlib_filter filter
 *                                   mlib_edge          edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      angle     Angle of rotation measured in radians.
 *      xcenter   X coordinate of the rotation center.
 *      ycenter   Y coordinate of the rotation center.
 *      filter    Type of resampling filter.
 *      edge      Type of edge condition.
 *
 * DESCRIPTION
 *  The point (xcenter, ycenter) of the source image is mapped to the
 *  center of the destination image.
 *  The direction of image rotation is defined as that rotating with
 *  a positive angle rotates points on the positive X axis toward
 *  the positive Y axis, i.e., a clockwise rotation.
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
 *      They can have 1, 2, 3 or 4 channels. They can be in MLIB_BIT, MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 *      src image can not have width or height larger than 32767.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_SysMath.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageRotate = __mlib_ImageRotate

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageRotate) mlib_ImageRotate
    __attribute__((weak, alias("__mlib_ImageRotate")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	OFFSET	0

/* *********************************************************** */

mlib_status
__mlib_ImageRotate(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 angle,
    mlib_d64 xcenter,
    mlib_d64 ycenter,
    mlib_filter filter,
    mlib_edge edge)
{
	mlib_d64 mtx[6];
	mlib_d64 cos1, sin1;

	cos1 = mlib_cos(-angle);
	sin1 = mlib_sin(-angle);
	mtx[0] = cos1;
	mtx[1] = sin1;
	mtx[2] =
	    0.5 * (mlib_ImageGetWidth(dst) - OFFSET) - xcenter * cos1 -
	    ycenter * sin1;
	mtx[3] = -sin1;
	mtx[4] = cos1;
	mtx[5] =
	    0.5 * (mlib_ImageGetHeight(dst) - OFFSET) + xcenter * sin1 -
	    ycenter * cos1;
	return (__mlib_ImageAffine(dst, src, mtx, filter, edge));
}

/* *********************************************************** */
