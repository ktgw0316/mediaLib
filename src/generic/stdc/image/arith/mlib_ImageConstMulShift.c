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

#pragma ident	"@(#)mlib_ImageConstMulShift.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageConstMulShift         - multiply an image with constants
 *      mlib_ImageConstMulShift_Inp     - multiply an image with constants
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageConstMulShift(mlib_image *dst,
 *                                          mlib_image *src,
 *                                          mlib_s32   *consts,
 *                                          mlib_s32   shift)
 *      mlib_status mlib_ImageConstMulShift_Inp(mlib_image *srcdst,
 *                                              mlib_s32   *consts,
 *                                              mlib_s32   shift)
 *
 * ARGUMENT
 *      src     pointer to the input image
 *      dst     pointer to the output image
 *      srcdst  pointer to the input and the output image
 *      consts  array of constants to be multiplied to each channel of the
 *              pixels
 *      shift   right shifting factor
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
 *              dst[x][y][c] = consts[c] * src[x][y][c] * 2^(-shift)
 *
 *      For in-place processing version (*_Inp):
 *              srcdst[x][y][c] = consts[c] * srcdst[x][y][c] * 2^(-shift)
 *
 *      If the result of the operation underflows/overflows the
 *      minimum/maximum value supported by the destination image, then it
 *      will be clamped to the minimum/maximum value respectively.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageScale2.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageConstMulShift = __mlib_ImageConstMulShift
#pragma weak mlib_ImageConstMulShift_Inp = __mlib_ImageConstMulShift_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageConstMulShift) mlib_ImageConstMulShift
    __attribute__((weak, alias("__mlib_ImageConstMulShift")));
__typeof__(__mlib_ImageConstMulShift_Inp) mlib_ImageConstMulShift_Inp
    __attribute__((weak, alias("__mlib_ImageConstMulShift_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageConstMulShift(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *consts,
    mlib_s32 shift)
{
	mlib_type type;
	void *psrc, *pdst;
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	mlib_s32 beta[4] = { 0, 0, 0, 0 };
	mlib_d64 dalpha[4], dbeta[4], dshift;
	mlib_s32 k;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, nchan, xsize, ysize, dlb, pdst);
	slb = mlib_ImageGetStride(src);
	psrc = mlib_ImageGetData(src);

	if (shift < 0 || shift > 31)
		return (MLIB_OUTOFRANGE);

/* branch to ImageScale if possible */

	if (type == MLIB_BYTE || type == MLIB_SHORT || type == MLIB_USHORT) {
		if (__mlib_ImageScale(dst, src, consts, beta,
		    shift) == MLIB_SUCCESS)
			return (MLIB_SUCCESS);
	}

	dshift = 1.0 / (1u << shift);
	for (k = 0; k < nchan; k++) {
		dalpha[k] = consts[k] * dshift;
		dbeta[k] = 0;
	}

	if (type == MLIB_BYTE) {

		return (mlib_ImageScale2_U8(dst, src, dalpha, dbeta));
	} else if (type == MLIB_SHORT) {

		return (mlib_ImageScale2_S16(dst, src, dalpha, dbeta));
	} else if (type == MLIB_USHORT) {

		return (mlib_ImageScale2_U16(dst, src, dalpha, dbeta));
	} else if (type == MLIB_INT) {

		return mlib_ImageDConstMul_S32(pdst, psrc, xsize, ysize, nchan,
		    slb / 4, dlb / 4, dalpha);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_ImageConstMulShift_Inp(
    mlib_image *srcdst,
    const mlib_s32 *consts,
    mlib_s32 shift)
{
	return (__mlib_ImageConstMulShift(srcdst, srcdst, consts, shift));
}

/* *********************************************************** */
