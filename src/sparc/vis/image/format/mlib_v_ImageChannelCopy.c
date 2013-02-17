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

#pragma ident	"@(#)mlib_v_ImageChannelCopy.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageChannelCopy  - Copy the selected channels of the source
 *                               image into the destination image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageChannelCopy(mlib_image       *dst,
 *                                        const mlib_image *src,
 *                                        mlib_s32         cmask);
 *
 * ARGUMENT
 *      dst     Pointer to output or destination image.
 *      src     Pointer to input or source image.
 *      cmask   Channel selection mask. Each bit of the mask represents a
 *              channel in the image data. The least significant bit (LSB)
 *              is corresponding to the last channel in the image data.
 *              The bits with value 1 stand for the channels selected.
 *
 * RESTRICTION
 *      src and dst images must have the same size, type and
 *      number of channels.
 *      They can have 1, 2, 3 or 4 channels of
 *      MLIB_BYTE, MLIB_SHORT, MLIB_USHORT, MLIB_INT,
 *      MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      Copy the selected channels of the source image into the
 *      destination image.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageChannelCopy.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageChannelCopy = __mlib_ImageChannelCopy

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageChannelCopy) mlib_ImageChannelCopy
    __attribute__((weak, alias("__mlib_ImageChannelCopy")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageChannelCopy(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 cmask)
{
	const mlib_s32 X8 = 0x7;
	const mlib_s32 X4 = 0x3;
	const mlib_s32 X2 = 0x1;
	const mlib_s32 A8D1 = MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_ONEDVECTOR;
	const mlib_s32 A8D2 = MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_STRIDE8X;
	const mlib_s32 A8D2X4 =
	    MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_STRIDE8X | MLIB_IMAGE_WIDTH4X;
	const mlib_s32 A8D2X2 =
	    MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_STRIDE8X | MLIB_IMAGE_WIDTH2X;

	mlib_type dtype;
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	mlib_s32 mask;
	void *sa, *da;
	mlib_s32 flags, flagd;
	mlib_s32 dsize;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);
	MLIB_IMAGE_SIZE_EQUAL(src, dst);

	dtype = mlib_ImageGetType(src);
	nchan = mlib_ImageGetChannels(src);
	xsize = mlib_ImageGetWidth(src);
	ysize = mlib_ImageGetHeight(src);
	slb = mlib_ImageGetStride(src);
	dlb = mlib_ImageGetStride(dst);
	sa = mlib_ImageGetData(src);
	da = mlib_ImageGetData(dst);
	flags = mlib_ImageGetFlags(src);
	flagd = mlib_ImageGetFlags(dst);
	dsize = xsize * ysize;

	mask = cmask & ((1 << nchan) - 1);

	if (!mask)
		return (MLIB_SUCCESS);

	if (((nchan == 1) && (cmask == 1)) || ((nchan == 2) && (cmask == 3)) ||
	    ((nchan == 3) && (cmask == 7)) || ((nchan == 4) && (cmask == 15))) {
		return (__mlib_ImageCopy(dst, src));
	} else if (dtype == MLIB_BYTE) {

		switch (nchan) {
		case 2:

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X4) == 0)) {

				mlib_v_ImageChannelCopy_U8_2_A8D1X4((mlib_u8 *)
				    sa, (mlib_u8 *)da, dsize, cmask);
			} else if (((flags & A8D2X4) == 0) &&
			    ((flagd & A8D2X4) == 0)) {

				mlib_v_ImageChannelCopy_U8_2_A8D2X4((mlib_u8 *)
				    sa, slb, (mlib_u8 *)da, dlb, xsize, ysize,
				    cmask);
			} else {

				mlib_v_ImageChannelCopy_U8_2((mlib_u8 *)sa, slb,
				    (mlib_u8 *)da, dlb, xsize, ysize, cmask);
			}

			break;

		case 3:

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X8) == 0)) {
				mlib_v_ImageChannelCopy_U8_3_A8D1((mlib_u8 *)sa,
				    (mlib_u8 *)da, dsize, cmask);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0) && ((xsize & X8) == 0)) {
				mlib_v_ImageChannelCopy_U8_3_A8D2((mlib_u8 *)sa,
				    slb, (mlib_u8 *)da, dlb, xsize, ysize,
				    cmask);
			} else {
				mlib_v_ImageChannelCopy_U8_3((mlib_u8 *)sa, slb,
				    (mlib_u8 *)da, dlb, xsize, ysize, cmask);
			}

			break;

		case 4:

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X2) == 0)) {

				mlib_v_ImageChannelCopy_U8_4_A8D1X2((mlib_u8 *)
				    sa, (mlib_u8 *)da, dsize, cmask);
			} else if (((flags & A8D2X2) == 0) &&
			    ((flagd & A8D2X2) == 0)) {

				mlib_v_ImageChannelCopy_U8_4_A8D2X2((mlib_u8 *)
				    sa, slb, (mlib_u8 *)da, dlb, xsize, ysize,
				    cmask);
			} else {

				mlib_v_ImageChannelCopy_U8_4((mlib_u8 *)sa, slb,
				    (mlib_u8 *)da, dlb, xsize, ysize, cmask);
			}

			break;
		}

		return (MLIB_SUCCESS);
	} else if ((dtype == MLIB_SHORT) || (dtype == MLIB_USHORT)) {
		switch (nchan) {
		case 2:

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X2) == 0)) {

				mlib_v_ImageChannelCopy_S16_2_A8D1X2((mlib_s16
				    *)sa, (mlib_s16 *)da, dsize, cmask);
			} else if (((flags & A8D2X2) == 0) &&
			    ((flagd & A8D2X2) == 0)) {

				mlib_v_ImageChannelCopy_S16_2_A8D2X2((mlib_s16
				    *)sa, slb, (mlib_s16 *)da, dlb, xsize,
				    ysize, cmask);
			} else {
				mlib_v_ImageChannelCopy_S16_2((mlib_s16 *)sa,
				    slb, (mlib_s16 *)da, dlb, xsize, ysize,
				    cmask);
			}

			break;

		case 3:

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X4) == 0)) {

				mlib_v_ImageChannelCopy_S16_3_A8D1((mlib_s16 *)
				    sa, (mlib_s16 *)da, dsize, cmask);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0) && ((xsize & X4) == 0)) {

				mlib_v_ImageChannelCopy_S16_3_A8D2((mlib_s16 *)
				    sa, slb, (mlib_s16 *)da, dlb, xsize, ysize,
				    cmask);
			} else {

				mlib_v_ImageChannelCopy_S16_3((mlib_s16 *)sa,
				    slb, (mlib_s16 *)da, dlb, xsize, ysize,
				    cmask);
			}

			break;

		case 4:

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0)) {

				mlib_v_ImageChannelCopy_S16_4_A8D1((mlib_s16 *)
				    sa, (mlib_s16 *)da, dsize, cmask);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0)) {

				mlib_v_ImageChannelCopy_S16_4_A8D2((mlib_s16 *)
				    sa, slb, (mlib_s16 *)da, dlb, xsize, ysize,
				    cmask);
			} else {

				mlib_v_ImageChannelCopy_S16_4((mlib_s16 *)sa,
				    slb, (mlib_s16 *)da, dlb, xsize, ysize,
				    cmask);
			}

			break;
		}

		return (MLIB_SUCCESS);
	} else if ((dtype == MLIB_INT) || (dtype == MLIB_FLOAT)) {

		switch (nchan) {
		case 2:

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0)) {

				mlib_v_ImageChannelCopy_S32_2_A8D1((mlib_s32 *)
				    sa, (mlib_s32 *)da, dsize, cmask);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0)) {

				mlib_v_ImageChannelCopy_S32_2_A8D2((mlib_s32 *)
				    sa, slb, (mlib_s32 *)da, dlb, xsize, ysize,
				    cmask);
			} else {

				mlib_v_ImageChannelCopy_S32_2((mlib_s32 *)sa,
				    slb, (mlib_s32 *)da, dlb, xsize, ysize,
				    cmask);
			}

			break;

		case 3:

			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X2) == 0)) {

				mlib_v_ImageChannelCopy_S32_3_A8D1((mlib_s32 *)
				    sa, (mlib_s32 *)da, dsize, cmask);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0) && ((xsize & X2) == 0)) {

				mlib_v_ImageChannelCopy_S32_3_A8D2((mlib_s32 *)
				    sa, slb, (mlib_s32 *)da, dlb, xsize, ysize,
				    cmask);
			} else if (cmask == 1 || cmask == 2 || cmask == 4) {

				mlib_v_ImageChannelCopy_s32((mlib_s32 *)sa, slb,
				    (mlib_s32 *)da, dlb, nchan, xsize, ysize,
				    cmask);
			} else {

				mlib_v_ImageChannelCopy_S32_3((mlib_s32 *)sa,
				    slb, (mlib_s32 *)da, dlb, xsize, ysize,
				    cmask);
			}

			break;

		case 4:

			if (cmask == 1 || cmask == 2 || cmask == 4 ||
			    cmask == 8) {

				mlib_v_ImageChannelCopy_s32((mlib_s32 *)sa, slb,
				    (mlib_s32 *)da, dlb, nchan, xsize, ysize,
				    cmask);
			} else if (((flags & A8D1) == 0) &&
			    ((flagd & A8D1) == 0)) {
				mlib_v_ImageChannelCopy_S32_4_A8D1((mlib_s32 *)
				    sa, (mlib_s32 *)da, dsize, cmask);
			} else if (((flags & A8D2) == 0) &&
			    ((flagd & A8D2) == 0)) {
				mlib_v_ImageChannelCopy_S32_4_A8D2((mlib_s32 *)
				    sa, slb, (mlib_s32 *)da, dlb, xsize, ysize,
				    cmask);
			} else {
				mlib_v_ImageChannelCopy_S32_4((mlib_s32 *)sa,
				    slb, (mlib_s32 *)da, dlb, xsize, ysize,
				    cmask);
			}

			break;
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_DOUBLE) {

		mlib_v_ImageChannelCopy_D64((mlib_d64 *)sa, slb,
		    (mlib_d64 *)da, dlb, nchan, xsize, ysize, cmask);
		return (MLIB_SUCCESS);
	} else {
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */
