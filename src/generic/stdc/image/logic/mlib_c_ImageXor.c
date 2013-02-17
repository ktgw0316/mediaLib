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

#pragma ident	"@(#)mlib_c_ImageXor.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageXor      - xor two images
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageXor(mlib_image       *dst,
 *                                const mlib_image *src1,
 *                                const mlib_image *src2);
 *
 * ARGUMENT
 *      dst     pointer to destination image
 *      src1    pointer to first source image
 *      src2    pointer to second source image
 *
 * RESTRICTION
 *      The src1, src2, and dst must be the same type and the same size.
 *      They can have 1, 2, 3, or 4 channels.
 *      They can be in MLIB_BYTE
 *      MLIB_SHORT, MLIB_USHORT, MLIB_INT or MLIB_BIT data type.
 *
 * DESCRIPTION
 *      Xor two images for each band:     dst = src1 ^ src2
 */

#include <mlib_image.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageXor = __mlib_ImageXor

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageXor) mlib_ImageXor
    __attribute__((weak, alias("__mlib_ImageXor")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	OPERATION(a1, a2)	(a1) ^ (a2)

#include <mlib_c_ImageLogic.h>

/* *********************************************************** */

mlib_status
__mlib_ImageXor(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2)
{
	MLIB_IMAGE_CHECK(src1);
	MLIB_IMAGE_CHECK(src2);
	MLIB_IMAGE_CHECK(dst);

	if ((mlib_ImageGetType(src1) == MLIB_BYTE) ||
	    (mlib_ImageGetType(src1) == MLIB_SHORT) ||
	    (mlib_ImageGetType(src1) == MLIB_USHORT) ||
	    (mlib_ImageGetType(src1) == MLIB_INT)) {

		return (mlib_c_ImageLogic(dst, src1, src2));
	} else if (mlib_ImageGetType(src1) == MLIB_BIT) {
		return (mlib_ImageXor_Bit(dst, src1, src2));
	} else
		return (MLIB_FAILURE);
}

/* *********************************************************** */
