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

#pragma ident	"@(#)mlib_ImageErode4_Fp.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageErode4_Fp
 *              Does a 3x3 dilate4 filter. Edge policy = no write.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageErode4_Fp(mlib_image       *dst,
 *                                      const mlib_image *src)
 *
 * ARGUMENTS
 *      dst     Pointer to destination image.
 *      src     Pointer to source image.
 *
 * RESTRICTION
 *      The images must be single channel images.
 *      The images can be in MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      Does a 3x3 erode 4 or dilate 4 filter.
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
 *      Uses the dilate4 neighborhood (not the dilate8):
 *              Erode4 neighborhood:           Erode8 neighborhood:
 *                        o                             o o o
 *                      o o o                           o o o
 *                        o                             o o o
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageClipping.h>
#include <mlib_ImageErode4.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageErode4_Fp = __mlib_ImageErode4_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageErode4_Fp) mlib_ImageErode4_Fp
    __attribute__((weak, alias("__mlib_ImageErode4_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageErode4_Fp(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_image dst_i[1], src_i[1];
	mlib_type type;
	mlib_s32 wid, hgt, slb, dlb;
	mlib_status ret;
	void *da, *sa;

	ret = mlib_ImageClipping(dst_i, src_i, NULL, NULL, NULL, dst, src, 3);

	if (ret != MLIB_SUCCESS)
		return (ret);

	MLIB_IMAGE_HAVE_CHAN(dst, 1);

	type = mlib_ImageGetType(dst_i);
	wid = mlib_ImageGetWidth(dst_i);
	hgt = mlib_ImageGetHeight(dst_i);
	dlb = mlib_ImageGetStride(dst_i);
	da = (void *)mlib_ImageGetData(dst_i);

	sa = mlib_ImageGetData(src_i);
	slb = mlib_ImageGetStride(src_i);

	if (wid < 3 || hgt < 3)
		return (MLIB_SUCCESS);

	switch (type) {
	case MLIB_FLOAT:
		return (mlib_ImageErode4_F32(da, sa, dlb, slb, wid, hgt));

	case MLIB_DOUBLE:
		return (mlib_ImageErode4_D64(da, sa, dlb, slb, wid, hgt));

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */
