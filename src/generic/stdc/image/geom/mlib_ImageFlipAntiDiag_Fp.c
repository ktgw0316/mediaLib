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

#pragma ident	"@(#)mlib_ImageFlipAntiDiag_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageFlipAntiDiag_Fp - flip image on anti diag
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageFlipAntiDiag_Fp(mlib_image       *dst,
 *                                            const mlib_image *src)
 *
 * ARGUMENTS
 *      dst  pointer to an dst image
 *      src  pointer to an src image
 *
 * RESTRICTION
 *      src and dst must be the same type and the same number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be of MLIB_FLOAT or
 *      MLIB_DOUBLE data type.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageFlipMainDiag.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageFlipAntiDiag_Fp = __mlib_ImageFlipAntiDiag_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageFlipAntiDiag_Fp) mlib_ImageFlipAntiDiag_Fp
    __attribute__((weak, alias("__mlib_ImageFlipAntiDiag_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageFlipAntiDiag_Fp(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_type type;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);

	type = mlib_ImageGetType(dst);

	if (type != MLIB_FLOAT && type != MLIB_DOUBLE) {
		return (MLIB_FAILURE);
	}

	return (mlib_ImageFlipRotate(dst, src, -1, -1));
}

/* *********************************************************** */
