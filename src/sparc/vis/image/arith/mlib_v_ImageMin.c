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

#pragma ident	"@(#)mlib_v_ImageMin.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageMin - get min pixel from two images
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMin(mlib_image       *dst,
 *                                const mlib_image *src1,
 *                                const mlib_image *src2);
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
 *      Min two images for each channel:  dst = min(src1, src2)
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageMinFunc.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMin = __mlib_ImageMin

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMin) mlib_ImageMin
    __attribute__((weak, alias("__mlib_ImageMin")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageMin(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2)
{
	mlib_s32 slb1, slb2, dlb, xsize, ysize, nchan;
	mlib_type dtype;
	void *sa1, *sa2, *da;
	const mlib_s32 D1 = MLIB_IMAGE_ONEDVECTOR;
	const mlib_s32 A8 = MLIB_IMAGE_ALIGNED8;
	const mlib_s32 A8D2 = MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_STRIDE8X;
	mlib_s32 flag1, flag2, flagd;
	mlib_s32 dsize;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src1);
	MLIB_IMAGE_CHECK(src2);
	MLIB_IMAGE_FULL_EQUAL(dst, src1);
	MLIB_IMAGE_FULL_EQUAL(dst, src2);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, nchan, xsize, ysize, dlb, da);
	slb1 = mlib_ImageGetStride(src1);
	sa1 = mlib_ImageGetData(src1);
	slb2 = mlib_ImageGetStride(src2);
	sa2 = mlib_ImageGetData(src2);
	flag1 = mlib_ImageGetFlags(src1);
	flag2 = mlib_ImageGetFlags(src2);
	flagd = mlib_ImageGetFlags(dst);

	xsize = xsize * nchan;
	dsize = xsize * ysize;

	if (dtype == MLIB_BYTE) {
/* MLIB_IMAGE_ONEDVECTOR && same alignment */

		if (((flag1 & D1) == 0) && ((flag2 & D1) == 0) &&
		    ((flagd & D1) == 0) && ((flag1 & A8) == (flag2 & A8)) &&
		    ((flag1 & A8) == (flagd & A8))) {

			mlib_v_ImageMin_U8_D1((mlib_u8 *)sa1,
			    (mlib_u8 *)sa2, (mlib_u8 *)da, dsize);
			return (MLIB_SUCCESS);

		} else if (((flag1 & D1) == 0) && ((flag2 & D1) == 0) &&
		    ((flagd & D1) == 0)) {

			mlib_v_ImageMin_U8_1D((mlib_u8 *)sa1,
			    (mlib_u8 *)sa2, (mlib_u8 *)da, dsize);
			return (MLIB_SUCCESS);

		} else if (((flag1 & A8D2) == 0) && ((flag2 & A8D2) == 0) &&
		    ((flagd & A8D2) == 0)) {

			mlib_v_ImageMin_U8_A8D2((mlib_u8 *)sa1, slb1,
			    (mlib_u8 *)sa2, slb2,
			    (mlib_u8 *)da, dlb, xsize, ysize);
			return (MLIB_SUCCESS);

		} else {

			mlib_v_ImageMin_U8((mlib_u8 *)sa1, slb1,
			    (mlib_u8 *)sa2, slb2,
			    (mlib_u8 *)da, dlb, xsize, ysize);
			return (MLIB_SUCCESS);
		}

	} else if (dtype == MLIB_SHORT) {

/* MLIB_IMAGE_ONEDVECTOR && same alignment */

		if (((flag1 & D1) == 0) && ((flag2 & D1) == 0) &&
		    ((flagd & D1) == 0) && ((flag1 & A8) == (flag2 & A8)) &&
		    ((flag1 & A8) == (flagd & A8))) {

			mlib_v_ImageMin_S16_D1((mlib_s16 *)sa1,
			    (mlib_s16 *)sa2, (mlib_s16 *)da, dsize);
			return (MLIB_SUCCESS);

		} else if (((flag1 & D1) == 0) && ((flag2 & D1) == 0) &&
		    ((flagd & D1) == 0)) {

			mlib_v_ImageMin_S16_1D((mlib_s16 *)sa1,
			    (mlib_s16 *)sa2, (mlib_s16 *)da, dsize);
			return (MLIB_SUCCESS);

		} else if (((flag1 & A8D2) == 0) && ((flag2 & A8D2) == 0) &&
		    ((flagd & A8D2) == 0)) {

			mlib_v_ImageMin_S16_A8D2((mlib_s16 *)sa1, slb1,
			    (mlib_s16 *)sa2, slb2,
			    (mlib_s16 *)da, dlb, xsize, ysize);
			return (MLIB_SUCCESS);

		} else {

			mlib_v_ImageMin_S16((mlib_s16 *)sa1, slb1,
			    (mlib_s16 *)sa2, slb2,
			    (mlib_s16 *)da, dlb, xsize, ysize);
			return (MLIB_SUCCESS);
		}

	} else if (dtype == MLIB_USHORT) {

/* MLIB_IMAGE_ONEDVECTOR && same alignment */

		if (((flag1 & D1) == 0) && ((flag2 & D1) == 0) &&
		    ((flagd & D1) == 0) && ((flag1 & A8) == (flag2 & A8)) &&
		    ((flag1 & A8) == (flagd & A8))) {

			mlib_v_ImageMin_U16_D1((mlib_u16 *)sa1,
			    (mlib_u16 *)sa2, (mlib_u16 *)da, dsize);
			return (MLIB_SUCCESS);

		} else if (((flag1 & D1) == 0) && ((flag2 & D1) == 0) &&
		    ((flagd & D1) == 0)) {

			mlib_v_ImageMin_U16_1D((mlib_u16 *)sa1,
			    (mlib_u16 *)sa2, (mlib_u16 *)da, dsize);
			return (MLIB_SUCCESS);

		} else if (((flag1 & A8D2) == 0) && ((flag2 & A8D2) == 0) &&
		    ((flagd & A8D2) == 0)) {

			mlib_v_ImageMin_U16_A8D2((mlib_u16 *)sa1, slb1,
			    (mlib_u16 *)sa2, slb2,
			    (mlib_u16 *)da, dlb, xsize, ysize);
			return (MLIB_SUCCESS);

		} else {

			mlib_v_ImageMin_U16((mlib_u16 *)sa1, slb1,
			    (mlib_u16 *)sa2, slb2,
			    (mlib_u16 *)da, dlb, xsize, ysize);
			return (MLIB_SUCCESS);
		}

	} else if (dtype == MLIB_INT) {

/* MLIB_IMAGE_ONEDVECTOR && same alignment */

		if (((flag1 & D1) == 0) && ((flag2 & D1) == 0) &&
		    ((flagd & D1) == 0) && ((flag1 & A8) == (flag2 & A8)) &&
		    ((flag1 & A8) == (flagd & A8))) {

			mlib_v_ImageMin_S32_D1((mlib_s32 *)sa1,
			    (mlib_s32 *)sa2, (mlib_s32 *)da, dsize);
			return (MLIB_SUCCESS);

		} else if (((flag1 & D1) == 0) && ((flag2 & D1) == 0) &&
		    ((flagd & D1) == 0)) {

			mlib_v_ImageMin_S32_1D((mlib_s32 *)sa1,
			    (mlib_s32 *)sa2, (mlib_s32 *)da, dsize);
			return (MLIB_SUCCESS);

		} else if (((flag1 & A8D2) == 0) && ((flag2 & A8D2) == 0) &&
		    ((flagd & A8D2) == 0)) {

			mlib_v_ImageMin_S32_A8D2((mlib_s32 *)sa1, slb1,
			    (mlib_s32 *)sa2, slb2,
			    (mlib_s32 *)da, dlb, xsize, ysize);
			return (MLIB_SUCCESS);

		} else {

			mlib_v_ImageMin_S32((mlib_s32 *)sa1, slb1,
			    (mlib_s32 *)sa2, slb2,
			    (mlib_s32 *)da, dlb, xsize, ysize);
			return (MLIB_SUCCESS);
		}

	} else
		return (MLIB_FAILURE);
}

/* *********************************************************** */
