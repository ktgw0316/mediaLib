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

#pragma ident	"@(#)mlib_v_ImageMulShift.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageMulShift - multiply two images with shifting
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMulShift(mlib_image       *dst,
 *                                     const mlib_image *src1,
 *                                     const mlib_image *src2,
 *                                     mlib_s32         shift);
 *
 * ARGUMENT
 *      src1    pointer to first input image
 *      src2    pointer to second input image
 *      dst     pointer to output image
 *      shift   right shifting factor
 *
 * NOTE
 *      ranges of valid shift are:
 *           0 <= shift <= 11 for MLIB_BYTE  type of image
 *           0 <= shift <= 16 for MLIB_SHORT type of image
 *           0 <= shift <= 16 for MLIB_USHORT type of image
 *           0 <= shift <= 31 for MLIB_INT   type of image
 *
 * RESTRICTION
 *      src1, src2, and dst must be the same type, the same size and the same
 *      number of channels.
 *      They can have 1, 2, 3, or 4 channels.
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      dst = (src1 * src2) >> shift
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageMulShift.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMulShift = __mlib_ImageMulShift

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMulShift) mlib_ImageMulShift
    __attribute__((weak, alias("__mlib_ImageMulShift")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageMulShift(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2,
    mlib_s32 shift)
{
	mlib_type dtype;
	mlib_u8 *sa1, *sa2, *da;
	mlib_s32 slb1, slb2, dlb, nchan, width, height;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src1);
	MLIB_IMAGE_CHECK(src2);
	MLIB_IMAGE_FULL_EQUAL(dst, src1);
	MLIB_IMAGE_FULL_EQUAL(dst, src2);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, nchan, width, height, dlb, da);
	slb1 = mlib_ImageGetStride(src1);
	sa1 = mlib_ImageGetData(src1);
	slb2 = mlib_ImageGetStride(src2);
	sa2 = mlib_ImageGetData(src2);

	width *= nchan;

	if (dtype == MLIB_BYTE) {

		if (shift == 0) {

			mlib_v_ImageMul_U8((mlib_u8 *)sa1, slb1, (mlib_u8 *)sa2,
			    slb2, (mlib_u8 *)da, dlb, width, height);
		} else if (shift >= 1 && shift <= 3) {

			mlib_v_ImageMulShift123_U8((mlib_u8 *)sa1, slb1,
			    (mlib_u8 *)sa2, slb2, (mlib_u8 *)da, dlb, width,
			    height, shift);
		} else if (shift >= 4 && shift <= 11) {

			mlib_v_ImageMulShift_U8((mlib_u8 *)sa1, slb1,
			    (mlib_u8 *)sa2, slb2, (mlib_u8 *)da, dlb, width,
			    height, shift);
		} else {

			return (MLIB_OUTOFRANGE);
		}

		return (MLIB_SUCCESS);
	}

	if (dtype == MLIB_SHORT) {

		if (shift == 0) {

			mlib_v_ImageMul_S16((mlib_s16 *)sa1, (slb1 >> 1),
			    (mlib_s16 *)sa2, (slb2 >> 1), (mlib_s16 *)da,
			    (dlb >> 1), width, height);
		} else if (shift >= 1 && shift <= 16) {

			mlib_v_ImageMulShift_S16((mlib_s16 *)sa1, (slb1 >> 1),
			    (mlib_s16 *)sa2, (slb2 >> 1), (mlib_s16 *)da,
			    (dlb >> 1), width, height, shift);
		} else {

			return (MLIB_OUTOFRANGE);
		}

		return (MLIB_SUCCESS);
	}

	if (dtype == MLIB_USHORT) {

		if (shift < 4) {

			mlib_v_ImageMulSmallShift_U16((mlib_s16 *)sa1,
			    (slb1 >> 1), (mlib_s16 *)sa2, (slb2 >> 1),
			    (mlib_s16 *)da, (dlb >> 1), width, height, shift);
		} else if (shift >= 4 && shift <= 16) {

			mlib_v_ImageMulShift_U16((mlib_s16 *)sa1, (slb1 >> 1),
			    (mlib_s16 *)sa2, (slb2 >> 1), (mlib_s16 *)da,
			    (dlb >> 1), width, height, shift);
		} else {

			return (MLIB_OUTOFRANGE);
		}

		return (MLIB_SUCCESS);
	}

	if (dtype == MLIB_INT) {

		if (shift < 0 || shift > 31)
			return (MLIB_OUTOFRANGE);

		mlib_v_ImageMulShift_S32((mlib_s32 *)sa1, (slb1 >> 2),
		    (mlib_s32 *)sa2, (slb2 >> 2), (mlib_s32 *)da, (dlb >> 2),
		    width, height, shift);
		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */
