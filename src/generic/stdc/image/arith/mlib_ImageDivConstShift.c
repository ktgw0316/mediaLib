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

#pragma ident	"@(#)mlib_ImageDivConstShift.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageDivConstShift         - divide an image by constants
 *      mlib_ImageDivConstShift_Inp     - divide an image by constants
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageDivConstShift(mlib_image       *dst,
 *                                          const mlib_image *src,
 *                                          const mlib_s32   *consts,
 *                                          mlib_s32         shift)
 *      mlib_status mlib_ImageDivConstShift_Inp(mlib_image     *srcdst,
 *                                              const mlib_s32 *consts,
 *                                              mlib_s32       shift)
 *
 * ARGUMENT
 *      src     pointer to the input image
 *      dst     pointer to the output image
 *      srcdst  pointer to the input and the output image
 *      consts  array of constants to divide into each channel of the pixels
 *      shift   left shifting factor
 *
 * RESTRICTION
 *      The images must have the same type, the same size, and the same number
 *      of channels.
 *      The images can have 1, 2, 3, or 4 channels.
 *      The images can be in MLIB_BYTE, MLIB_SHORT,
 *      MLIB_USHORT or MLIB_INT data type.
 *
 *      0 <= shift <= 31
 *
 * DESCRIPTION
 *      For the general version:
 *              dst[x][y][c] = src[x][y][c] / consts[c] * 2^shift
 *
 *      For in-place processing version (*_Inp):
 *              srcdst[x][y][c] = srcdst[x][y][c] / consts[c] * 2^shift
 *
 *      If the result of the operation underflows/overflows the
 *      minimum/maximum value supported by the destination image, then it
 *      will be clamped to the minimum/maximum value respectively.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageDivConstShift = __mlib_ImageDivConstShift
#pragma weak mlib_ImageDivConstShift_Inp = __mlib_ImageDivConstShift_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageDivConstShift) mlib_ImageDivConstShift
    __attribute__((weak, alias("__mlib_ImageDivConstShift")));
__typeof__(__mlib_ImageDivConstShift_Inp) mlib_ImageDivConstShift_Inp
    __attribute__((weak, alias("__mlib_ImageDivConstShift_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageDivConstShift(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *consts,
    mlib_s32 shift)
{
	mlib_d64 dalpha[4], dbeta[4], dshift;
	mlib_s32 nchan, k;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);
	nchan = mlib_ImageGetChannels(dst);

	if (shift < 0 || shift > 31)
		return (MLIB_OUTOFRANGE);

	dshift = (1u << shift);
	for (k = 0; k < nchan; k++) {
		dalpha[k] =
		    (consts[k] == 0) ? 2.0 * MLIB_S32_MAX : dshift / consts[k];
		dbeta[k] = 0;
	}

	return (__mlib_ImageScale2(dst, src, dalpha, dbeta));
}

/* *********************************************************** */

mlib_status
__mlib_ImageDivConstShift_Inp(
    mlib_image *srcdst,
    const mlib_s32 *consts,
    mlib_s32 shift)
{
	return (__mlib_ImageDivConstShift(srcdst, srcdst, consts, shift));
}

/* *********************************************************** */
