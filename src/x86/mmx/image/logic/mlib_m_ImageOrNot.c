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

#pragma ident	"@(#)mlib_m_ImageOrNot.c	9.6	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageOrNot - orNot two images
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageOrNot(mlib_image       *dst,
 *                                  const mlib_image *src1,
 *                                  const mlib_image *src2);
 *
 * ARGUMENT
 *      dst     pointer to destination image
 *      src1    pointer to first source image
 *      src2    pointer to second source image
 *
 * RESTRICTION
 *      The src1, src2, and dst must be the same type and the same size.
 *      They can have 1, 2, 3, or 4 channels.
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT, MLIB_INT or
 *      MLIB_BIT data type.
 *
 * DESCRIPTION
 *      OrNot two images for each band:
 *        dst = src1 | (~src2)
 */

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <mmintrin.h>
#endif

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *************************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageOrNot = __mlib_ImageOrNot

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageOrNot) mlib_ImageOrNot
    __attribute__((weak, alias("__mlib_ImageOrNot")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

    mlib_status mlib_ImageOrNot_Bit(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2);

/* *********************************************************** */

#define	MLIB_M_IMAGE_LOGIC(d, s1, s2)	d = _mm_or_si64(s1, _mm_not_si64(s2));

#include <mlib_m_ImageLogic.h>

/* *********************************************************** */

mlib_status
__mlib_ImageOrNot(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2)
{
	MLIB_IMAGE_CHECK(dst);

	if (mlib_ImageGetType(dst) == MLIB_BIT) {
		return (mlib_ImageOrNot_Bit(dst, src1, src2));
	}

	return (mlib_m_ImageLogic(dst, src1, src2));
}

/* *********************************************************** */
