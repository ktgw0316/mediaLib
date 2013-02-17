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

#pragma ident	"@(#)mlib_ImageNotAnd_Inp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageNotAnd_Inp - notAnd two images
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageNotAnd_Inp(mlib_image       *src1dst,
 *                                       const mlib_image *src2);
 *
 * ARGUMENT
 *      src1dst  pointer to first input and output image
 *      src2     pointer to second input image
 *
 * RESTRICTION
 *      The src1dst and src2 must be the same type and the same size.
 *      They can have 1, 2, 3, or 4 channels.
 *      They can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT, MLIB_INT or MLIB_BIT data type.
 *
 * DESCRIPTION
 *      NotAnd two images for each channel:  src1dst = ~(src1dst & src2)
 */

#include <mlib_image.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageNotAnd_Inp = __mlib_ImageNotAnd_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageNotAnd_Inp) mlib_ImageNotAnd_Inp
    __attribute__((weak, alias("__mlib_ImageNotAnd_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageNotAnd_Inp(
    mlib_image *src1dst,
    const mlib_image *src2)
{
	return (__mlib_ImageNotAnd(src1dst, src1dst, src2));
}

/* *********************************************************** */
