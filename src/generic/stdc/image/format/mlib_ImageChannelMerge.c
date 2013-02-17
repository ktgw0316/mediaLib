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

#pragma ident	"@(#)mlib_ImageChannelMerge.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageChannelMerge - convert an array of single channel
 *                               images into a multi-channel image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageChannelMerge(mlib_image       *dst,
 *                                         const mlib_image **srcs);
 *
 * ARGUMENTS
 *      dst     Pointer to a destination multi-channel image
 *      srcs    Pointer to an array of input single channel images
 *
 * RESTRICTIONS
 *      All images must have the same type, same width, and same height.
 *      They can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT, MLIB_INT,
 *      MLIB_FLOAT or MLIB_DOUBLE data type.
 *      Destination image must have the number of channels equal to
 *      the number of images in the srcs array.
 *      Input images must be single channel images.
 *
 * DESCRIPTION
 *      Convert an array of single channel images into a multi-channel
 *      image.
 *
 *            A0 A1 A2 ...
 *            B0 B1 B2 ...     ===>     A0 B0 C0 A1 B1 C1 A2 B2 C2 ...
 *            C0 C1 C2 ...
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageChannelMerge.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageChannelMerge = __mlib_ImageChannelMerge

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageChannelMerge) mlib_ImageChannelMerge
    __attribute__((weak, alias("__mlib_ImageChannelMerge")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageChannelMerge(
    mlib_image *dst,
    const mlib_image **srcs)
{
	mlib_type image_type;
	mlib_s32 nchan;
	mlib_u8 *dst_0;
	mlib_u8 *src_0;
	mlib_u8 *src_1;
	mlib_u8 *src_2 = NULL;
	mlib_u8 *src_3 = NULL;
	mlib_s32 dst_str;
	mlib_s32 src0_str;
	mlib_s32 src1_str;
	mlib_s32 src2_str = 0;
	mlib_s32 src3_str = 0;
	mlib_s32 height;
	mlib_s32 width;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(srcs);
	MLIB_IMAGE_CHECK(srcs[0]);
	MLIB_IMAGE_CHECK(srcs[1]);

	image_type = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(dst);
	width = mlib_ImageGetWidth(dst);
	height = mlib_ImageGetHeight(dst);

	MLIB_IMAGE_HAVE_TYPE(srcs[0], image_type);
	MLIB_IMAGE_HAVE_TYPE(srcs[1], image_type);
	MLIB_IMAGE_HAVE_CHAN(srcs[0], 1);
	MLIB_IMAGE_HAVE_CHAN(srcs[1], 1);

	dst_0 = (mlib_u8 *)mlib_ImageGetData(dst);
	src_0 = (mlib_u8 *)mlib_ImageGetData(srcs[0]);
	src_1 = (mlib_u8 *)mlib_ImageGetData(srcs[1]);
	dst_str = mlib_ImageGetStride(dst);
	src0_str = mlib_ImageGetStride(srcs[0]);
	src1_str = mlib_ImageGetStride(srcs[1]);

	if (nchan > 2) {
		MLIB_IMAGE_CHECK(srcs[2]);
		MLIB_IMAGE_HAVE_CHAN(srcs[2], 1);
		MLIB_IMAGE_HAVE_TYPE(srcs[2], image_type);
		src_2 = (mlib_u8 *)mlib_ImageGetData(srcs[2]);
		src2_str = mlib_ImageGetStride(srcs[2]);

		if (nchan > 3) {
			MLIB_IMAGE_CHECK(srcs[3]);
			MLIB_IMAGE_HAVE_CHAN(srcs[3], 1);
			MLIB_IMAGE_HAVE_TYPE(srcs[3], image_type);
			src_3 = (mlib_u8 *)mlib_ImageGetData(srcs[3]);
			src3_str = mlib_ImageGetStride(srcs[3]);
		}
	}

	switch (image_type) {

	case MLIB_BYTE:
		switch (nchan) {
		case 2:
			return mlib_ImageChannelMerge2_U8(dst_0, src_0, src_1,
			    height, width, dst_str, src0_str, src1_str);
		case 3:
			return mlib_ImageChannelMerge3_U8(dst_0, src_0, src_1,
			    src_2, height, width, dst_str, src0_str, src1_str,
			    src2_str);

		case 4:
			return mlib_ImageChannelMerge4_U8(dst_0, src_0, src_1,
			    src_2, src_3, height, width, dst_str, src0_str,
			    src1_str, src2_str, src3_str);

		default:
			return (MLIB_FAILURE);
		}

	case MLIB_USHORT:
	case MLIB_SHORT:
		dst_str >>= 1;
		src0_str >>= 1;
		src1_str >>= 1;
		switch (nchan) {
		case 2:
			return mlib_ImageChannelMerge2_S16((mlib_s16 *)dst_0,
			    (mlib_s16 *)src_0,
			    (mlib_s16 *)src_1, height, width,
			    dst_str, src0_str, src1_str);

		case 3:
			src2_str >>= 1;
			return mlib_ImageChannelMerge3_S16((mlib_s16 *)dst_0,
			    (mlib_s16 *)src_0,
			    (mlib_s16 *)src_1,
			    (mlib_s16 *)src_2, height, width,
			    dst_str, src0_str, src1_str, src2_str);

		case 4:
			src2_str >>= 1;
			src3_str >>= 1;
			return mlib_ImageChannelMerge4_S16((mlib_s16 *)dst_0,
			    (mlib_s16 *)src_0,
			    (mlib_s16 *)src_1,
			    (mlib_s16 *)src_2,
			    (mlib_s16 *)src_3, height, width,
			    dst_str, src0_str, src1_str, src2_str, src3_str);

		default:
			return (MLIB_FAILURE);
		}

	case MLIB_FLOAT:
	case MLIB_INT:
		dst_str >>= 2;
		src0_str >>= 2;
		src1_str >>= 2;
		switch (nchan) {
		case 2:
			return mlib_ImageChannelMerge2_FS32((mlib_s32 *)dst_0,
			    (mlib_s32 *)src_0,
			    (mlib_s32 *)src_1, height, width,
			    dst_str, src0_str, src1_str);

		case 3:
			src2_str >>= 2;
			return mlib_ImageChannelMerge3_FS32((mlib_s32 *)dst_0,
			    (mlib_s32 *)src_0,
			    (mlib_s32 *)src_1,
			    (mlib_s32 *)src_2, height, width,
			    dst_str, src0_str, src1_str, src2_str);

		case 4:
			src2_str >>= 2;
			src3_str >>= 2;
			return mlib_ImageChannelMerge4_FS32((mlib_s32 *)dst_0,
			    (mlib_s32 *)src_0,
			    (mlib_s32 *)src_1,
			    (mlib_s32 *)src_2,
			    (mlib_s32 *)src_3, height, width,
			    dst_str, src0_str, src1_str, src2_str, src3_str);

		default:
			return (MLIB_FAILURE);
		}

	case MLIB_DOUBLE:
		dst_str >>= 3;
		src0_str >>= 3;
		src1_str >>= 3;
		switch (nchan) {
		case 2:
			return mlib_ImageChannelMerge2_D64((mlib_d64 *)dst_0,
			    (mlib_d64 *)src_0,
			    (mlib_d64 *)src_1, height, width,
			    dst_str, src0_str, src1_str);

		case 3:
			src2_str >>= 3;
			return mlib_ImageChannelMerge3_D64((mlib_d64 *)dst_0,
			    (mlib_d64 *)src_0,
			    (mlib_d64 *)src_1,
			    (mlib_d64 *)src_2, height, width,
			    dst_str, src0_str, src1_str, src2_str);

		case 4:
			src2_str >>= 3;
			src3_str >>= 3;
			return mlib_ImageChannelMerge4_D64((mlib_d64 *)dst_0,
			    (mlib_d64 *)src_0,
			    (mlib_d64 *)src_1,
			    (mlib_d64 *)src_2,
			    (mlib_d64 *)src_3, height, width,
			    dst_str, src0_str, src1_str, src2_str, src3_str);

		default:
			return (MLIB_FAILURE);
		}

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */
