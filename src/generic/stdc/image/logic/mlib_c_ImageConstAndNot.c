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

#pragma ident	"@(#)mlib_c_ImageConstAndNot.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageConstAndNot - image logical operation with constant
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageConstAndNot(mlib_image       *dst,
 *                                        const mlib_image *src,
 *                                        const mlib_s32   *c);
 *
 * ARGUMENT
 *      dst     Pointer to destination image
 *      src     Pointer to source image
 *      c       Array of constants for each channel
 *
 * RESTRICTION
 *      The src and dst must be the same type and the same size.
 *      They can have 1, 2, 3, or 4 channels.
 *      They can be in MLIB_BIT, MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT
 *      data type.
 *
 * DESCRIPTION
 *      File for one of the following operations:
 *
 *      And  dst(i, j) = c & src(i, j)
 *      Or  dst(i, j) = c | src(i, j)
 *      Xor  dst(i, j) = c ^ src(i, j)
 *      NotAnd  dst(i, j) = ~(c & src(i, j))
 *      NotOr  dst(i, j) = ~(c | src(i, j))
 *      NotXor  dst(i, j) = ~(c ^ src(i, j))
 *      AndNot  dst(i, j) = c & (~src(i, j))
 *      OrNot  dst(i, j) = c & (~src(i, j))
 *
 * NOTE
 *      Because of the similarity among the 8 functions, one header
 *      file mlib_c_ImageConstLogic.h is used in the 8 files
 *      (mlib_c_ImageConst*.c) for the 8 similar functions.
 */

#include <mlib_image.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageConstAndNot = __mlib_ImageConstAndNot

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageConstAndNot) mlib_ImageConstAndNot
    __attribute__((weak, alias("__mlib_ImageConstAndNot")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CONSTLOGIC(c, a)	(c) & (~(a))

#include <mlib_c_ImageConstLogic.h>

/* *********************************************************** */

mlib_status
__mlib_ImageConstAndNot(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c)
{
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);

#ifdef _NO_LONGLONG

	if ((mlib_ImageGetType(src) == MLIB_BYTE) ||
	    (mlib_ImageGetType(src) == MLIB_SHORT) ||
	    (mlib_ImageGetType(src) == MLIB_USHORT)) {

		return (mlib_c_ImageConstLogic(dst, src, c));

	} else if (mlib_ImageGetType(src) == MLIB_BIT) {

		return (mlib_ImageConstAndNot_Bit(dst, src, c));

	} else if (mlib_ImageGetType(src) == MLIB_INT) {

		return (mlib_c_ImageConstLogic_S32(dst, src, c));

	} else
		return (MLIB_FAILURE);

#else /* _NO_LONGLONG */

	if ((mlib_ImageGetType(src) == MLIB_BYTE) ||
	    (mlib_ImageGetType(src) == MLIB_SHORT) ||
	    (mlib_ImageGetType(src) == MLIB_USHORT) ||
	    mlib_ImageGetType(src) == MLIB_INT) {

		return (mlib_c_ImageConstLogic(dst, src, c));

	} else if (mlib_ImageGetType(src) == MLIB_BIT) {

		return (mlib_ImageConstAndNot_Bit(dst, src, c));

	} else
		return (MLIB_FAILURE);
#endif /* _NO_LONGLONG */
}

/* *********************************************************** */
