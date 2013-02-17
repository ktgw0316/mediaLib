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

#pragma ident	"@(#)mlib_m_ImageConstAnd.c	9.6	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageConstAnd - image logical operation with constant
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageConstAnd(mlib_image       *dst,
 *                                     const mlib_image *src,
 *                                     const mlib_s32   *c);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      c       array of constants for each channel
 *
 * RESTRICTION
 *      The src and dst must be the same type and the same size.
 *      They can have 1, 2, 3, or 4 channels.
 *      They can be in MLIB_BIT, MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT
 *      data type.
 *
 * DESCRIPTION
 *      MMX version file for one of the following operations:
 *
 *      And     dst(i, j) = c & src(i, j)
 *      Or      dst(i, j) = c | src(i, j)
 *      Xor     dst(i, j) = c ^ src(i, j)
 *      NotAnd  dst(i, j) = ~(c & src(i, j))
 *      NotOr   dst(i, j) = ~(c | src(i, j))
 *      NotXor  dst(i, j) = ~(c ^ src(i, j))
 *      AndNot  dst(i, j) = c & (~src(i, j))
 *      OrNot   dst(i, j) = c | (~src(i, j))
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

#pragma weak mlib_ImageConstAnd = __mlib_ImageConstAnd

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageConstAnd) mlib_ImageConstAnd
    __attribute__((weak, alias("__mlib_ImageConstAnd")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

    mlib_status mlib_ImageConstAnd_Bit(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c);

/* *********************************************************** */

#define	MMX_CONST_LOGIC(d, s, c)	d = _mm_and_si64(s, c);

#include <mlib_m_ImageConstLogic.h>

/* *********************************************************** */

mlib_status
__mlib_ImageConstAnd(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c)
{
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);

	if (mlib_ImageGetType(src) == MLIB_BIT) {
		return (mlib_ImageConstAnd_Bit(dst, src, c));
	}

	return (mlib_m_ImageConstLogic(dst, src, c));
}

/* *********************************************************** */
