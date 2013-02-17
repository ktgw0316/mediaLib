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

#pragma ident "@(#)mlib_s_ImageScalarBlend.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *	mlib_ImageScalarBlend	- blend two images
 *	mlib_ImageScalarBlend_Inp	- blend two images
 *
 * SYNOPSIS
 *	mlib_status mlib_ImageScalarBlend(mlib_image       *dst,
 *	                               const mlib_image *src1,
 *                                     const mlib_image *src2,
 *                                     const mlib_s32 *alpha);
 *	mlib_status mlib_ImageScalarBlend_Inp(mlib_image *src1dst,
 *	                                    const mlib_image *src2,
 *	                                    const mlib_s32 *alpha);
 *
 * ARGUMENT
 *	dst	pointer to output image
 *	src1	pointer to first input image
 *	src2	pointer to second input image
 *	src1dst	pointer to the first input and the output image
 *      alpha	pointer to array of alpha coefficients
 *
 * RESTRICTION
 *	src1, src2, src1dst, and dst must be the same type,
 *	the same size and the same number of channels.
 *	They can have 1, 2, 3, or 4 channels.
 *	They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT
 *	or MLIB_INT data type.
 *      The alpha coefficients are in Q31 format and must
 *	be positive.
 *
 * DESCRIPTION
 *	Blend two image with an alpha coefficient for each channel.
 *
 *		dst = alpha*src1 + (0x7fffffff-alpha)*src2,
 *
 *	where (0 <= alpha <= 0x7fffffff)
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include "mlib_s_ImageScalarBlend.h"

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageScalarBlend = __mlib_ImageScalarBlend
#pragma weak mlib_ImageScalarBlend_Inp = __mlib_ImageScalarBlend_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageScalarBlend) mlib_ImageScalarBlend
	__attribute__((weak, alias("__mlib_ImageScalarBlend")));
__typeof__(__mlib_ImageScalarBlend_Inp) mlib_ImageScalarBlend_Inp
	__attribute__((weak, alias("__mlib_ImageScalarBlend1_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageScalarBlend(
	mlib_image *dst,
	const mlib_image *src1,
	const mlib_image *src2,
	const mlib_s32 *alpha)
{
	mlib_type dtype;
	mlib_s32 slb1, slb2, dlb, xsize, ysize, nchan;
	void *sa1, *sa2, *da;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src1);
	MLIB_IMAGE_CHECK(src2);
	MLIB_IMAGE_FULL_EQUAL(dst, src1);
	MLIB_IMAGE_FULL_EQUAL(dst, src2);

	dtype = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	slb1 = mlib_ImageGetStride(src1);
	slb2 = mlib_ImageGetStride(src2);
	dlb = mlib_ImageGetStride(dst);
	sa1 = mlib_ImageGetData(src1);
	sa2 = mlib_ImageGetData(src2);
	da = mlib_ImageGetData(dst);

	if (dtype == MLIB_BYTE) {
		mlib_s_ImageScalarBlend_u8(da, dlb,
					sa1, slb1,
					sa2, slb2,
					alpha,
					xsize, ysize, nchan);
		return (MLIB_SUCCESS);
	} else
	if (dtype == MLIB_SHORT) {
		mlib_s_ImageScalarBlend_s16(da, dlb,
					sa1, slb1,
					sa2, slb2,
					alpha,
					xsize, ysize, nchan);
		return (MLIB_SUCCESS);
	} else
	if (dtype == MLIB_USHORT) {
		mlib_s_ImageScalarBlend_u16(da, dlb,
					sa1, slb1,
					sa2, slb2,
					alpha,
					xsize, ysize, nchan);
		return (MLIB_SUCCESS);
	} else
	if (dtype == MLIB_INT) {
		mlib_s_ImageScalarBlend_s32(da, dlb,
					sa1, slb1,
					sa2, slb2,
					alpha,
					xsize, ysize, nchan);
		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_ImageScalarBlend_Inp(
	mlib_image *src1dst,
	const mlib_image *src2,
	const mlib_s32 *alpha)
{
	return (__mlib_ImageScalarBlend(src1dst, src1dst, src2, alpha));
}

/* *********************************************************** */
