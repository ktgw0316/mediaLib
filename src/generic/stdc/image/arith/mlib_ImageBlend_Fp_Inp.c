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

#pragma ident	"@(#)mlib_ImageBlend_Fp_Inp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageBlend1_Fp_Inp - blend two images
 *      mlib_ImageBlend2_Fp_Inp - blend two images
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageBlend1_Fp_Inp(mlib_image *src1dst,
 *                                          const mlib_image *src2,
 *                                          const mlib_image *alpha);
 *      mlib_status mlib_ImageBlend2_Fp_Inp(mlib_image *src2dst,
 *                                          const mlib_image *src1,
 *                                          const mlib_image *alpha);
 *
 * ARGUMENT
 *      src1dst  pointer to first input and output image
 *      src2dst  pointer to second input and output image
 *      src1     pointer to first input image
 *      src2     pointer to second input image
 *      alpha    pointer to alpha image
 *
 * RESTRICTION
 *      src1, src2, src1dst, src2dst, and alpha must be the same type and
 *      the same size.
 *      src1, src2, src1dst, and src2dst must be the same number of channels.
 *      They can have one to four channels. They can be in
 *      MLIB_FLOAT or MLIB_DOUBLE data type. Optionally, alpha can be
 *      single-channel image while src1, src2, src1dst and src2dst are
 *      multi-channel images.
 *
 * DESCRIPTION
 *      Blend two image with an alpha coefficient for each channel.
 *      src1dst = alpha*src1dst + (1-alpha)*src2
 *      src2dst = alpha*src1 + (1-alpha)*src2dst
 */

#include <mlib_image.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageBlend1_Fp_Inp = __mlib_ImageBlend1_Fp_Inp
#pragma weak mlib_ImageBlend2_Fp_Inp = __mlib_ImageBlend2_Fp_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageBlend1_Fp_Inp) mlib_ImageBlend1_Fp_Inp
    __attribute__((weak, alias("__mlib_ImageBlend1_Fp_Inp")));
__typeof__(__mlib_ImageBlend2_Fp_Inp) mlib_ImageBlend2_Fp_Inp
    __attribute__((weak, alias("__mlib_ImageBlend2_Fp_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageBlend1_Fp_Inp(
    mlib_image *src1dst,
    const mlib_image *src2,
    const mlib_image *alpha)
{
	return (__mlib_ImageBlend_Fp(src1dst, src1dst, src2, alpha));
}

/* *********************************************************** */

mlib_status
__mlib_ImageBlend2_Fp_Inp(
    mlib_image *src2dst,
    const mlib_image *src1,
    const mlib_image *alpha)
{
	return (__mlib_ImageBlend_Fp(src2dst, src1, src2dst, alpha));
}

/* *********************************************************** */
