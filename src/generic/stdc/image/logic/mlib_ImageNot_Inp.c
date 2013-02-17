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

#pragma ident	"@(#)mlib_ImageNot_Inp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageNot_Inp - not an image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageNot_Inp(mlib_image *srcdst);
 *
 * ARGUMENT
 *      srcdst  pointer to input and output image
 *
 * RESTRICTION
 *      The srcdst can have 1, 2, 3, or 4 channels.
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT,
 *      MLIB_INT or MLIB_BIT data type.
 *
 * DESCRIPTION
 *      Not an image for each channel:    srcdst = ~srcdst
 */

#include <mlib_image.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageNot_Inp = __mlib_ImageNot_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageNot_Inp) mlib_ImageNot_Inp
    __attribute__((weak, alias("__mlib_ImageNot_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageNot_Inp(
    mlib_image *srcdst)
{
	return (__mlib_ImageNot(srcdst, srcdst));
}

/* *********************************************************** */
