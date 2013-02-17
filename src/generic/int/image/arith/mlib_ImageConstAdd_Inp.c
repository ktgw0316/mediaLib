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

#pragma ident   "@(#)mlib_ImageConstAdd_Inp.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageConstAdd_Inp - add image with constant
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageConstAdd_Inp(mlib_image     *srcdst,
 *                                         const mlib_s32 *c);
 *
 * ARGUMENT
 *      srcdst  pointer to input and output image
 *      c       array of constants to be added to each pixel
 *
 * RESTRICTION
 *      The image can have 1, 2, 3 or 4 channels. The image can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      srcdst(i, j) = c + srcdst(i, j)
 */

#include <mlib_image.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageConstAdd_Inp = __mlib_ImageConstAdd_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageConstAdd_Inp) mlib_ImageConstAdd_Inp
    __attribute__((weak, alias("__mlib_ImageConstAdd_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageConstAdd_Inp(
    mlib_image *srcdst,
    const mlib_s32 *c)
{
	return (__mlib_ImageConstAdd(srcdst, srcdst, c));
}

/* *********************************************************** */
