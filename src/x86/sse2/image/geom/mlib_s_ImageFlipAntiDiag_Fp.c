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

#pragma ident   "@(#)mlib_s_ImageFlipAntiDiag_Fp.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageFlipAntiDiag_Fp - flip image on main diag
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageFlipAntiDiag_Fp(mlib_image *dst,
 *                                            const mlib_image *src)
 *
 * ARGUMENTS
 *      dst  pointer to an dst image
 *      src  pointer to an src image
 *
 * RESTRICTION
 *      src and dst must be the same type and the same number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be of MLIB_FLOAT or
 *      MLIB_DOUBLE data type.
 */

#include <stdlib.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *************************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageFlipAntiDiag_Fp = __mlib_ImageFlipAntiDiag_Fp

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageFlipAntiDiag_Fp) mlib_ImageFlipAntiDiag_Fp
    __attribute__((weak, alias("__mlib_ImageFlipAntiDiag_Fp")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

    void mlib_ImageFlipAntiDiag_S32_1(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_s_ImageFlipAntiDiag_S32_3(
    mlib_image *dst,
    const mlib_image *src);

    void mlib_s_ImageFlipAntiDiag_D64_1(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_s_ImageFlipAntiDiag_D64_2(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_s_ImageFlipAntiDiag_D64_3(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_s_ImageFlipAntiDiag_D64_4(
    mlib_image *dst,
    const mlib_image *src);

/* *********************************************************** */

mlib_status
__mlib_ImageFlipAntiDiag_Fp(
    mlib_image *dst,
    const mlib_image *src)
{

/*  check for obvious errors  */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);

	switch (mlib_ImageGetType(src)) {

/*  handle MLIB_FLOAT data type of image  */
	case MLIB_FLOAT:
		switch (mlib_ImageGetChannels(src)) {
		case 1:
			mlib_ImageFlipAntiDiag_S32_1(dst, src);
			break;
		case 2:
			mlib_s_ImageFlipAntiDiag_D64_1(dst, src);
			break;
		case 3:
			mlib_s_ImageFlipAntiDiag_S32_3(dst, src);
			break;
		case 4:
			mlib_s_ImageFlipAntiDiag_D64_2(dst, src);
			break;
		default:
			return (MLIB_FAILURE);
		}
		break;

/*  handle MLIB_DOUBLE data type of image  */
	case MLIB_DOUBLE:
		switch (mlib_ImageGetChannels(src)) {
		case 1:
			mlib_s_ImageFlipAntiDiag_D64_1(dst, src);
			break;
		case 2:
			mlib_s_ImageFlipAntiDiag_D64_2(dst, src);
			break;
		case 3:
			mlib_s_ImageFlipAntiDiag_D64_3(dst, src);
			break;
		case 4:
			mlib_s_ImageFlipAntiDiag_D64_4(dst, src);
			break;
		default:
			return (MLIB_FAILURE);
		}
		break;

/*  discard any other data types  */
	default:
		return (MLIB_FAILURE);

	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
