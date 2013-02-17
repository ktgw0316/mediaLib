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

#pragma ident	"@(#)mlib_c_ImageColorErrDiff3x3.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageColorErrorDiffusion3x3 - convert a true color image to an
 *                                         indexed color image by error
 *                                         diffusion dithering
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorErrorDiffusion3x3(mlib_image       *dst,
 *                                                   const mlib_image *src,
 *                                                   const mlib_s32   *kernel,
 *                                                   mlib_s32         scale,
 *                                                   const void       *colormap)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      kernel    Pointer to the error-distribution kernel, in row major order.
 *      scale     The scaling factor for kernel or to convert the input
 *                integer coefficients into floating-point coefficients:
 *                floating-point coefficient = integer coefficient * 2^(-scale)
 *      colormap  Internal data structure for inverse color mapping.
 *
 * DESCRIPTION
 *      Convert a true color image to a pseudo color image with the
 *      method of error diffusion dithering.
 *
 *      The src can be an MLIB_BYTE or MLIB_SHORT image with 3 or 4 channels.
 *      The dst must be a 1-channel MLIB_BYTE or MLIB_SHORT image.
 *
 *   Dithering kernel 3x3:  ...   ...    ...
 *                          ...   ...    koef[0]
 *                          beta  gamma  delta
 *
 * RESTRICTIONS
 *   (abs(koef[0]) + abs(beta) + abs(gamma) + abs(delta)) * 2^(-scale) < 8192
 *    scale >= 0
 */

#include <mlib_image.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageColorErrorDiffusion3x3 = \
	__mlib_ImageColorErrorDiffusion3x3

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageColorErrorDiffusion3x3) mlib_ImageColorErrorDiffusion3x3
    __attribute__((weak, alias("__mlib_ImageColorErrorDiffusion3x3")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageColorErrorDiffusion3x3(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scale,
    const void *colormap)
{
	return __mlib_ImageColorErrorDiffusionMxN(dst, src, kernel, 3, 3, 1, 1,
	    scale, colormap);
}

/* *********************************************************** */
