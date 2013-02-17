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

#pragma ident	"@(#)mlib_ImageErode8.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageErode8
 *              Does a 3x3 erode filter. Edge policy = no write.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageErode8(mlib_image       *dst,
 *                                   const mlib_image *src)
 *
 * ARGUMENTS
 *      dst     Pointer to destination image.
 *      src     Pointer to source image.
 *
 * RESTRICTION
 *      The images must be single channel images.
 *      The images can be in MLIB_BIT, MLIB_BYTE, MLIB_SHORT, MLIB_USHORT
 *      or MLIB_INT data type.
 *
 * DESCRIPTION
 *      Does a 3x3 erode filter.
 *      Filtered values are returned in the destination image.
 *      Edge = no write.
 *
 *      The mapping of source image to destination image is left/top corner to
 *      left/top corner, but with a shift of the destination image if it's
 *      smaller than the source image.
 *      The exact mapping formula is
 *           xs = xd + x_offset
 *           ys = yd + y_offset
 *      where
 *           x_offset = (dst_width >= src_width) ? 0 : (kernel_size - 1)/2
 *           y_offset = (dst_height >= src_height) ? 0 : (kernel_size - 1)/2
 *
 *      Uses the erode8 neighborhood (not the erode4):
 *              Erode4 neighborhood:           Erode8 neighborhood:
 *                        o                             o o o
 *                      o o o                           o o o
 *                        o                             o o o
 *
 */

#include <mlib_image.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageErode8 = __mlib_ImageErode8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageErode8) mlib_ImageErode8
    __attribute__((weak, alias("__mlib_ImageErode8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageErode8(
    mlib_image *dst,
    const mlib_image *src)
{
	return (__mlib_ImageMinFilter3x3(dst, src));
}

/* *********************************************************** */
