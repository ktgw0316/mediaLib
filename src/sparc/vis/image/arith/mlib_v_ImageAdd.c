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

#pragma ident	"@(#)mlib_v_ImageAdd.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageAdd - add two images
 *
 * SYNOPSIS
 *      mlib_status  mlib_ImageAdd(mlib_image       *dst,
 *                                 const mlib_image *src1,
 *                                 const mlib_image *src2);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src1    pointer to first input image
 *      src2    pointer to second input image
 *
 * RESTRICTION
 *      src1, src2, and dst must be the same type, the same size and the same
 *      number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      dst = src1 + src2
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageAddFunc.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageAdd = __mlib_ImageAdd

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageAdd) mlib_ImageAdd
    __attribute__((weak, alias("__mlib_ImageAdd")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageAdd(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2)
{
	mlib_type dtype;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src1);
	MLIB_IMAGE_CHECK(src2);
	MLIB_IMAGE_FULL_EQUAL(dst, src1);
	MLIB_IMAGE_FULL_EQUAL(dst, src2);

	dtype = mlib_ImageGetType(dst);

	if (dtype == MLIB_BYTE)
		return (mlib_v_ImageAdd_U8(dst, src1, src2));
	else if (dtype == MLIB_SHORT)
		return (mlib_v_ImageAdd_S16(dst, src1, src2));
	else if (dtype == MLIB_USHORT)
		return (mlib_v_ImageAdd_U16(dst, src1, src2));
	else if (dtype == MLIB_INT)
		return (mlib_v_ImageAdd_S32(dst, src1, src2));
	else
		return (MLIB_FAILURE);
}

/* *********************************************************** */
