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

#pragma ident	"@(#)mlib_ImageConstMul.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageConstMul              - multiply an image with constants
 *      mlib_ImageConstMul_Inp
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageConstMul(mlib_image       *dst,
 *                                     const mlib_image *src,
 *                                     const mlib_d64   *consts)
 *      mlib_status mlib_ImageConstMul_Inp(mlib_image     *srcdst,
 *                                         const mlib_d64 *consts)
 *
 * ARGUMENT
 *      src     pointer to the input image
 *      dst     pointer to the output image
 *      srcdst  pointer to the input and the output image
 *      consts  array of constants to be multiplied to each channel of the
 *              pixels
 *
 * RESTRICTION
 *      The images must have the same type, the same size, and the same number
 *      of channels.
 *      The images can have 1, 2, 3, or 4 channels.
 *      The images can be in MLIB_BYTE, MLIB_SHORT,
 *      MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      For the general version:
 *              dst[x][y][c] = consts[c] * src[x][y][c]
 *
 *      For in-place processing version (*_Inp):
 *              srcdst[x][y][c] = consts[c] * srcdst[x][y][c]
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

#pragma weak mlib_ImageConstMul = __mlib_ImageConstMul
#pragma weak mlib_ImageConstMul_Inp = __mlib_ImageConstMul_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageConstMul) mlib_ImageConstMul
    __attribute__((weak, alias("__mlib_ImageConstMul")));
__typeof__(__mlib_ImageConstMul_Inp) mlib_ImageConstMul_Inp
    __attribute__((weak, alias("__mlib_ImageConstMul_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageConstMul(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *consts)
{
	mlib_d64 dbeta[4] = { 0, 0, 0, 0 };

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	return (__mlib_ImageScale2(dst, src, consts, dbeta));
}

/* *********************************************************** */

mlib_status
__mlib_ImageConstMul_Inp(
    mlib_image *srcdst,
    const mlib_d64 *consts)
{
	return (__mlib_ImageConstMul(srcdst, srcdst, consts));
}

/* *********************************************************** */
