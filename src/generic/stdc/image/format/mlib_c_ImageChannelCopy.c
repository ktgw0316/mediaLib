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

#pragma ident	"@(#)mlib_c_ImageChannelCopy.c	9.2	07/10/09 SMI"

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
#include <mlib_c_ImageChannelCopy.h>

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
	mlib_type dtype;
	mlib_s32 src_stride, dst_stride, src_width, src_height, nchannels;
	mlib_s32 mask;
	void *sa, *da;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);
	MLIB_IMAGE_SIZE_EQUAL(src, dst);

	dtype = mlib_ImageGetType(src);
	nchannels = mlib_ImageGetChannels(src);
	src_stride = mlib_ImageGetStride(src);
	dst_stride = mlib_ImageGetStride(dst);
	src_height = mlib_ImageGetHeight(src);
	src_width = mlib_ImageGetWidth(src);
	sa = mlib_ImageGetData(src);
	da = mlib_ImageGetData(dst);

	mask = cmask & ((1 << nchannels) - 1);

	if (!mask)
		return (MLIB_SUCCESS);

	if (((nchannels == 1) && (cmask == 1)) ||
	    ((nchannels == 2) && (cmask == 3)) ||
	    ((nchannels == 3) && (cmask == 7)) ||
	    ((nchannels == 4) && (cmask == 15))) {
		return (__mlib_ImageCopy(dst, src));
	} else {
		mlib_s32 src_width_nchannels = src_width * nchannels;

		if (src_width_nchannels == src_stride &&
		    src_width_nchannels == dst_stride) {
			src_width *= src_height;
			src_height = 1;
		}

		if (dtype == MLIB_BYTE) {
			if (nchannels == 3) {
				mlib_c_ImageChannelCopy_u8_3((mlib_u8 *)sa,
				    src_stride, (mlib_u8 *)da, dst_stride,
				    src_width, src_height, mask);
				return (MLIB_SUCCESS);
			} else {
				mlib_c_ImageChannelCopy_u8_24((mlib_u8 *)sa,
				    src_stride, (mlib_u8 *)da, dst_stride,
				    nchannels, src_width, src_height, mask);
				return (MLIB_SUCCESS);
			}
		} else if ((dtype == MLIB_SHORT) || (dtype == MLIB_USHORT)) {

			if (nchannels == 4) {
				mlib_c_ImageChannelCopy_s16_4((mlib_s16 *)sa,
				    src_stride, (mlib_s16 *)da, dst_stride,
				    src_width, src_height, mask);
				return (MLIB_SUCCESS);
			} else if (nchannels == 3) {
				mlib_c_ImageChannelCopy_s16_3((mlib_s16 *)sa,
				    src_stride, (mlib_s16 *)da, dst_stride,
				    src_width, src_height, mask);
				return (MLIB_SUCCESS);
			} else if (nchannels == 2) {
				mlib_c_ImageChannelCopy_s16_2((mlib_s16 *)sa,
				    src_stride, (mlib_s16 *)da, dst_stride,
				    src_width, src_height, mask);
				return (MLIB_SUCCESS);
			}

			return (MLIB_SUCCESS);
		} else if ((dtype == MLIB_INT) || (dtype == MLIB_FLOAT)) {
			mlib_c_ImageChannelCopy_s32((mlib_s32 *)sa, src_stride,
			    (mlib_s32 *)da, dst_stride, nchannels, src_width,
			    src_height, mask);
			return (MLIB_SUCCESS);
		} else if (dtype == MLIB_DOUBLE) {
			mlib_c_ImageChannelCopy_d64((mlib_d64 *)sa, src_stride,
			    (mlib_d64 *)da, dst_stride, nchannels, src_width,
			    src_height, mask);
			return (MLIB_SUCCESS);
		} else {
			return (MLIB_FAILURE);
		}
	}
}

/* *********************************************************** */
