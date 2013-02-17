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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_ImageColorXYZ2RGB.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageColorXYZ2RGB - color conversion from XYZ to RGB space
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorXYZ2RGB(mlib_image       *dst,
 *                                         const mlib_image *src);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *
 * RESTRICTION
 *      src and dst must be the same type, the same size and the same
 *      number of channels.
 *      They must be 3-channel images.
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *
 *        |R|   |fmat[0] fmat[1] fmat[2]|   |X|
 *        |G| = |fmat[3] fmat[4] fmat[5]| * |Y|
 *        |B|   |fmat[6] fmat[7] fmat[8]|   |Z|
 *
 *   where
 *
 *      fmat[] = { 3.240479, -1.537150, -0.498535,
 *                -0.969256,  1.875992,  0.041566,
 *                 0.055648, -0.204043,  1.057311};
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_c_ImageColorTables.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageColorXYZ2RGB = __mlib_ImageColorXYZ2RGB

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageColorXYZ2RGB) mlib_ImageColorXYZ2RGB
    __attribute__((weak, alias("__mlib_ImageColorXYZ2RGB")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
mlib_status
__mlib_ImageColorXYZ2RGB(
    mlib_image *dst,
    const mlib_image *src)
{
/* CIE XYZ to Rec709 RGB with D64 White Point */
	mlib_d64 fmat[9] = { 3.240479, -1.537150, -0.498535,
		-0.969256, 1.875992, 0.041566,
		0.055648, -0.204043, 1.057311
	};

	return (__mlib_ImageColorConvert1(dst, src, fmat));
}

/* *********************************************************** */
