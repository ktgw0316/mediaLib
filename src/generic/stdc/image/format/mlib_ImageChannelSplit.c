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

#pragma ident	"@(#)mlib_ImageChannelSplit.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageChannelSplit - convert a multi-channel image into
 *                               an array of single channel image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageChannelSplit(mlib_image       **dsts,
 *                                         const mlib_image *src);
 *
 * ARGUMENTS
 *      dsts    Pointer to an array of destination single channel images
 *      src     Pointer to an input multi-channel image
 *
 * RESTRICTIONS
 *      All images must have the same type, width, and height.
 *      They can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT, MLIB_INT,
 *      MLIB_FLOAT or MLIB_DOUBLE data type.
 *      Input image must have the number of channels equal to
 *      the number of images in the dsts array.
 *      Destination images must be single channel images.
 *
 * DESCRIPTION
 *      Convert a multi-channel image into an array of single channel
 *      images.
 *
 *                                                       A0 A1 A2 ...
 *             A0 B0 C0 A1 B1 C1 A2 B2 C2 ...    ===>    B0 B1 B2 ...
 *                                                       C0 C1 C2 ...
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageChannelSplit.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageChannelSplit = __mlib_ImageChannelSplit

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageChannelSplit) mlib_ImageChannelSplit
    __attribute__((weak, alias("__mlib_ImageChannelSplit")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageChannelSplit(
    mlib_image **dsts,
    const mlib_image *src)
{
	mlib_type image_type;
	mlib_u8 *dst0, *dst1, *dst2 = NULL, *dst3 = NULL, *src0;
	mlib_s32 dst0_str, dst1_str, dst2_str = 0, dst3_str = 0, src0_str;
	mlib_s32 height, width, nchan;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dsts[0]);
	MLIB_IMAGE_CHECK(dsts[1]);

	image_type = mlib_ImageGetType(src);
	nchan = mlib_ImageGetChannels(src);
	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);

	MLIB_IMAGE_HAVE_TYPE(dsts[0], image_type);
	MLIB_IMAGE_HAVE_TYPE(dsts[1], image_type);
	MLIB_IMAGE_HAVE_CHAN(dsts[0], 1);
	MLIB_IMAGE_HAVE_CHAN(dsts[1], 1);

	dst0 = (mlib_u8 *)mlib_ImageGetData(dsts[0]);
	dst1 = (mlib_u8 *)mlib_ImageGetData(dsts[1]);
	src0 = (mlib_u8 *)mlib_ImageGetData(src);
	src0_str = mlib_ImageGetStride(src);
	dst0_str = mlib_ImageGetStride(dsts[0]);
	dst1_str = mlib_ImageGetStride(dsts[1]);

	if (nchan > 2) {
		MLIB_IMAGE_CHECK(dsts[2]);
		MLIB_IMAGE_HAVE_CHAN(dsts[2], 1);
		MLIB_IMAGE_HAVE_TYPE(dsts[2], image_type);
		dst2 = mlib_ImageGetData(dsts[2]);
		dst2_str = mlib_ImageGetStride(dsts[2]);

		if (nchan > 3) {
			MLIB_IMAGE_CHECK(dsts[3]);
			MLIB_IMAGE_HAVE_CHAN(dsts[3], 1);
			MLIB_IMAGE_HAVE_TYPE(dsts[3], image_type);
			dst3 = mlib_ImageGetData(dsts[3]);
			dst3_str = mlib_ImageGetStride(dsts[3]);
		}
	}

	switch (image_type) {
	case MLIB_BYTE:
		switch (nchan) {
		case 2:
			return mlib_ImageChannelSplit2_U8(dst0, dst1, src0,
			    height, width, dst0_str, dst1_str, src0_str);
		case 3:
			return mlib_ImageChannelSplit3_U8(dst0, dst1, dst2,
			    src0, height, width, dst0_str, dst1_str, dst2_str,
			    src0_str);
		case 4:
			return mlib_ImageChannelSplit4_U8(dst0, dst1, dst2,
			    dst3, src0, height, width, dst0_str, dst1_str,
			    dst2_str, dst3_str, src0_str);
		default:
			return (MLIB_FAILURE);
		}

	case MLIB_USHORT:
	case MLIB_SHORT:
		switch (nchan) {
		case 2:
			return mlib_ImageChannelSplit2_S16((mlib_s16 *)dst0,
			    (mlib_s16 *)dst1,
			    (mlib_s16 *)src0, height, width,
			    dst0_str, dst1_str, src0_str);
		case 3:
			return mlib_ImageChannelSplit3_S16((mlib_s16 *)dst0,
			    (mlib_s16 *)dst1,
			    (mlib_s16 *)dst2,
			    (mlib_s16 *)src0, height, width,
			    dst0_str, dst1_str, dst2_str, src0_str);
		case 4:
			return mlib_ImageChannelSplit4_S16((mlib_s16 *)dst0,
			    (mlib_s16 *)dst1,
			    (mlib_s16 *)dst2,
			    (mlib_s16 *)dst3,
			    (mlib_s16 *)src0, height, width,
			    dst0_str, dst1_str, dst2_str, dst3_str, src0_str);
		default:
			return (MLIB_FAILURE);
		}

	case MLIB_INT:
	case MLIB_FLOAT:
		switch (nchan) {
		case 2:
			return mlib_ImageChannelSplit2_FS32((mlib_s32 *)dst0,
			    (mlib_s32 *)dst1,
			    (mlib_s32 *)src0, height, width,
			    dst0_str, dst1_str, src0_str);
		case 3:
			return mlib_ImageChannelSplit3_FS32((mlib_s32 *)dst0,
			    (mlib_s32 *)dst1,
			    (mlib_s32 *)dst2,
			    (mlib_s32 *)src0, height, width,
			    dst0_str, dst1_str, dst2_str, src0_str);
		case 4:
			return mlib_ImageChannelSplit4_FS32((mlib_s32 *)dst0,
			    (mlib_s32 *)dst1,
			    (mlib_s32 *)dst2,
			    (mlib_s32 *)dst3,
			    (mlib_s32 *)src0, height, width,
			    dst0_str, dst1_str, dst2_str, dst3_str, src0_str);
		default:
			return (MLIB_FAILURE);
		}

	case MLIB_DOUBLE:
		switch (nchan) {
		case 2:
			return mlib_ImageChannelSplit2_D64((mlib_d64 *)dst0,
			    (mlib_d64 *)dst1,
			    (mlib_d64 *)src0, height, width,
			    dst0_str, dst1_str, src0_str);
		case 3:
			return mlib_ImageChannelSplit3_D64((mlib_d64 *)dst0,
			    (mlib_d64 *)dst1,
			    (mlib_d64 *)dst2,
			    (mlib_d64 *)src0, height, width,
			    dst0_str, dst1_str, dst2_str, src0_str);
		case 4:
			return mlib_ImageChannelSplit4_D64((mlib_d64 *)dst0,
			    (mlib_d64 *)dst1,
			    (mlib_d64 *)dst2,
			    (mlib_d64 *)dst3,
			    (mlib_d64 *)src0, height, width,
			    dst0_str, dst1_str, dst2_str, dst3_str, src0_str);
		default:
			return (MLIB_FAILURE);
		}

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */
