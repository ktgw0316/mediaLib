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

#pragma ident	"@(#)mlib_c_ImageSqrShift.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageSqrShift - square of an image with shifting
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageSqrShift(mlib_image       *dst,
 *                                     const mlib_image *src,
 *                                     mlib_s32         shift);
 *
 * ARGUMENT
 *      src     pointer to input image
 *      dst     pointer to output image
 *      shift   right shifting factor
 *
 * NOTE
 *      ranges of valid shift are:
 *           4 <= shift <= 11   for MLIB_BYTE  type of image
 *           1 <= shift <= 16   for MLIB_SHORT type of image
 *           1 <= shift <= 16   for MLIB_USHORT type of image
 *       -1023 <= shift <= 1022 for MLIB_INT   type of image
 *
 * RESTRICTION
 *      src and dst must be the same type, the same size and the same
 *      number of channels.
 *      They can have 1, 2, 3 or 4 channels.
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      dst = (src * src) >> shift
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_c_ImageSqrShift.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageSqrShift = __mlib_ImageSqrShift

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageSqrShift) mlib_ImageSqrShift
    __attribute__((weak, alias("__mlib_ImageSqrShift")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageSqrShift(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 shift)
{
	mlib_type dtype;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	dtype = mlib_ImageGetType(dst);

	if (dtype == MLIB_BYTE) {
		if ((shift < 4) || (shift > 11)) {
			return (MLIB_OUTOFRANGE);
		}
	}

	if (dtype == MLIB_SHORT) {
		if ((shift < 1) || (shift > 16)) {
			return (MLIB_OUTOFRANGE);
		}
	}

	if (dtype == MLIB_INT) {
		if ((shift < -1023) || (shift > 1022)) {
			return (MLIB_OUTOFRANGE);
		}
	}

	if (dtype == MLIB_BYTE) {

		return (mlib_c_ImageSqrShift_U8(dst, src, shift));
	} else {

		void *sa = mlib_ImageGetData(src);
		void *da = mlib_ImageGetData(dst);
		mlib_s32 slb = mlib_ImageGetStride(src);
		mlib_s32 dlb = mlib_ImageGetStride(dst);
		mlib_s32 nchan = mlib_ImageGetChannels(src);
		mlib_s32 xsize = mlib_ImageGetWidth(src) * nchan;
		mlib_s32 ysize = mlib_ImageGetHeight(src);

		if (dtype == MLIB_SHORT) {

			mlib_c_ImageSqrShift_S16((mlib_s16 *)sa, (slb >> 1),
			    (mlib_s16 *)da, (dlb >> 1), xsize, ysize, shift);
			return (MLIB_SUCCESS);
		} else if (dtype == MLIB_USHORT) {

			mlib_c_ImageSqrShift_U16((mlib_u16 *)sa, (slb >> 1),
			    (mlib_u16 *)da, (dlb >> 1), xsize, ysize, shift);
			return (MLIB_SUCCESS);
		} else if (dtype == MLIB_INT) {

			mlib_c_ImageSqrShift_S32((mlib_s32 *)sa, (slb >> 2),
			    (mlib_s32 *)da, (dlb >> 2), xsize, ysize, shift);
			return (MLIB_SUCCESS);
		}
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */
