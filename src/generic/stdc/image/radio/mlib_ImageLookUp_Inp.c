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

#pragma ident	"@(#)mlib_ImageLookUp_Inp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageLookUp_Inp - table lookup
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageLookUp_Inp(mlib_image *srcdst,
 *                                       const void **table);
 *
 * ARGUMENT
 *      srcdst   Pointer to input and output image.
 *      table    Lookup table.
 *
 * DESCRIPTION
 *      The mlib_ImageLookUp_Inp function performs table lookup on an
 *      image. The destination image is obtained by passing a source image
 *      through a lookup table.
 *
 *      The srcdst image may be 1-, 2-, 3-, or 4-channeled of data types
 *      MLIB_BYTE, MLIB_USHORT, MLIB_SHORT, or MLIB_INT. The srcdst image
 *      must have the same number of channels and the same data type as
 *      the lookup table.
 *
 *      It is the user's responsibility to make sure that the lookup table
 *      supplied is suitable for the source image. Specifically, the table
 *      entries cover the entire range of source data. Otherwise, the result
 *      of this function is undefined.
 *
 *      The pixel values of the destination image are defined as the following:
 *
 *          srcdst[x][y][c] = table[c][srcdst[x][y][c]]
 */

#include <mlib_image.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageLookUp_Inp = __mlib_ImageLookUp_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageLookUp_Inp) mlib_ImageLookUp_Inp
    __attribute__((weak, alias("__mlib_ImageLookUp_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageLookUp_Inp(
    mlib_image *srcdst,
    const void **table)
{
	return (__mlib_ImageLookUp(srcdst, srcdst, table));
}

/* *********************************************************** */
