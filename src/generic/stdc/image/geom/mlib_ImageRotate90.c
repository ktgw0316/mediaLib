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

#pragma ident	"@(#)mlib_ImageRotate90.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageRotate90 - rotate image 90 degrees clockwise
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageRotate90(mlib_image       *dst,
 *                                     const mlib_image *src)
 *
 * ARGUMENTS
 *      dst  pointer to an dst image
 *      src  pointer to an src image
 *
 * RESTRICTION
 *      src and dst must be the same type and the same number of channels.
 *      They can be in MLIB_BIT, MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or
 *      MLIB_INT data type.
 *      Images of MLIB_BIT date type can have only 1 channel,
 *      images of other date types can have 1, 2, 3 or 4 channels.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageFlipMainDiag.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageRotate90 = __mlib_ImageRotate90

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageRotate90) mlib_ImageRotate90
    __attribute__((weak, alias("__mlib_ImageRotate90")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageRotate90(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_type type;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);

	type = mlib_ImageGetType(dst);

	if (type == MLIB_FLOAT || type == MLIB_DOUBLE) {
		return (MLIB_FAILURE);
	}

	return (mlib_ImageFlipRotate(dst, src, 0, -1));
}

/* *********************************************************** */
