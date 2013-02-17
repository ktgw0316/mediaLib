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

#pragma ident	"@(#)mlib_c_ImageChannelExtract.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageChannelExtract  - Copy the selected channels of the source
 *                                  image into the destination image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageChannelExtract(mlib_image       *dst,
 *                                           const mlib_image *src,
 *                                           mlib_s32         cmask);
 *
 * ARGUMENT
 *    dst     Pointer to destination image.
 *    src     Pointer to source image.
 *    cmask   Source channel selection mask.
 *
 *    The least significant bit (LSB) is corresponding to the
 *    last channel in the source image data.
 *    The bits with value 1 stand for the channels selected.
 *    If more than N channels are selected, the leftmost N
 *    channels are extracted, where N is the number of channels
 *    in the destination image.
 *
 * RESTRICTION
 *    The src and dst must have the same width, height and data type.
 *    The src and dst can have 1, 2, 3 or 4 channels.
 *    The src and dst can be either MLIB_BYTE, MLIB_SHORT, MLIB_USHORT,
 *    MLIB_INT, MLIB_FLOAT or MLIB_DOUBLE.
 *
 * DESCRIPTION
 *    Copy the selected channels of the source image into the
 *    destination image
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_c_ImageChannelExtract.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageChannelExtract = __mlib_ImageChannelExtract

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageChannelExtract) mlib_ImageChannelExtract
    __attribute__((weak, alias("__mlib_ImageChannelExtract")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageChannelExtract(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 cmask)
{
/* pointer for pixel in src */
	void *sp;

/* pointer for pixel in dst */
	void *dp;

/* normalized channel mask */
	mlib_s32 ncmask = 0;

/* number of channels for src */
	mlib_s32 channels;

/* number of channels for dst */
	mlib_s32 channeld;

/* for src and dst */
	mlib_s32 width, height;

/* stride in bytes for src */
	mlib_s32 strides;

/* stride in bytes for dst */
	mlib_s32 strided;

/* number of selected channels */
	mlib_s32 bit1count = 0;

/* offset of first selected channel */
	mlib_s32 delta0 = 0;

/* number of channels in first group */
	mlib_s32 count1 = 0;
	mlib_s32 i;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_SIZE_EQUAL(src, dst);

	channels = mlib_ImageGetChannels(src);
	channeld = mlib_ImageGetChannels(dst);
	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	strides = mlib_ImageGetStride(src);
	strided = mlib_ImageGetStride(dst);
	sp = mlib_ImageGetData(src);
	dp = mlib_ImageGetData(dst);

/* normalize the cmask, and count the number of bit with value 1 */
	for (i = (channels - 1); i >= 0; i--) {
		if (((cmask & (1 << i)) != 0) && (bit1count < channeld)) {
			ncmask += (1 << i);
			bit1count++;
		}
	}

/*
 * do not support the cases in which the number of selected channels is
 * less than the number of channels in the destination image
 */

	if (bit1count < channeld) {
		return (MLIB_FAILURE);
	}

	if (channels == channeld) {
		return (__mlib_ImageCopy(dst, src));
	}

	for (i = (channels - 1); i >= 0; i--) {
		if (!(ncmask & (1 << i)))
			delta0++;
		else
			break;
	}

	for (; i >= 0; i--) {
		if (ncmask & (1 << i))
			count1++;
		else
			break;
	}

	switch (mlib_ImageGetType(src)) {
	case MLIB_BYTE:
#ifdef _LITTLE_ENDIAN
		mlib_c_ImageChannelExtract_U8((mlib_u8 *)sp, strides,
		    (mlib_u8 *)dp, strided,
		    channels, channeld, width, height, ncmask);
		return (MLIB_SUCCESS);
#else /* _LITTLE_ENDIAN */
	    {
		    mlib_u8 *sl = (mlib_u8 *)sp + delta0;
		    mlib_u8 *dl = (mlib_u8 *)dp;

		    switch (channels * 10 + channeld) {
		    case 21:
			    mlib_c_ImageChannelExtract_U8_2_1(sl, strides, dl,
				strided, width, height);
			    return (MLIB_SUCCESS);

		    case 31:
			    mlib_c_ImageChannelExtract_U8_3_1(sl, strides, dl,
				strided, width, height);
			    return (MLIB_SUCCESS);

		    case 32:
			    mlib_c_ImageChannelExtract_U8_3_2(sl, strides, dl,
				strided, width, height, count1);
			    return (MLIB_SUCCESS);

		    case 42:

/* mask 1010 or 0101 */
			    if (ncmask == 0xA || ncmask == 0x5) {
				    mlib_c_ImageChannelExtract_U8_2_1(sl,
					strides, dl, strided, 2 * width,
					height);
				    return (MLIB_SUCCESS);
			    }

			    mlib_c_ImageChannelExtract_U8_4_2(sl, strides, dl,
				strided, width, height, count1);
			    return (MLIB_SUCCESS);

		    case 41:
		    case 43:
			    mlib_c_ImageChannelExtract_U8((mlib_u8 *)sp,
				strides, (mlib_u8 *)dp, strided, channels,
				channeld, width, height, ncmask);
			    return (MLIB_SUCCESS);

		    default:
			    return (MLIB_FAILURE);
		    }
	    }

#endif /* _LITTLE_ENDIAN */

	case MLIB_USHORT:
	case MLIB_SHORT:
		mlib_c_ImageChannelExtract_S16((mlib_u16 *)sp, strides,
		    (mlib_u16 *)dp, strided,
		    channels, channeld, width, height, ncmask);
		break;

	case MLIB_INT:
	case MLIB_FLOAT:
	    {
#ifndef _LITTLE_ENDIAN
		    mlib_f32 *sl = (mlib_f32 *)sp + delta0;
		    mlib_f32 *dl = (mlib_f32 *)dp;
#endif /* _LITTLE_ENDIAN */

		    strides /= 4;
		    strided /= 4;

#ifdef _LITTLE_ENDIAN
		    mlib_c_ImageChannelExtract_S32((mlib_s32 *)sp, strides,
			(mlib_s32 *)dp, strided,
			channels, channeld, width, height, ncmask);
		    return (MLIB_SUCCESS);
#else /* _LITTLE_ENDIAN */
		    switch (channels * 10 + channeld) {
		    case 21:
			    mlib_c_ImageChannelExtract_32_2_1(sl, strides, dl,
				strided, width, height);
			    return (MLIB_SUCCESS);

		    case 31:
			    mlib_c_ImageChannelExtract_32_3_1(sl, strides, dl,
				strided, width, height);
			    return (MLIB_SUCCESS);

		    case 32:
			    mlib_c_ImageChannelExtract_32_3_2(sl, strides, dl,
				strided, width, height, count1);
			    return (MLIB_SUCCESS);

		    case 41:
			    mlib_c_ImageChannelExtract_32_4_1(sl, strides, dl,
				strided, width, height);
			    return (MLIB_SUCCESS);

		    case 42:

/* mask 1010 or 0101 */
			    if (ncmask == 0xA || ncmask == 0x5) {
				    mlib_c_ImageChannelExtract_32_2_1(sl,
					strides, dl, strided, 2 * width,
					height);
			    } else {
				    mlib_c_ImageChannelExtract_32_4_2(sl,
					strides, dl, strided, width, height,
					count1);
			    }

			    return (MLIB_SUCCESS);

		    case 43:
			    mlib_c_ImageChannelExtract_32_4_3(sl, strides, dl,
				strided, width, height, count1);
			    return (MLIB_SUCCESS);

		    default:
			    return (MLIB_FAILURE);
		    }

#endif /* _LITTLE_ENDIAN */
	    }

	case MLIB_DOUBLE:
		mlib_c_ImageChannelExtract_D64((mlib_d64 *)sp, strides,
		    (mlib_d64 *)dp, strided,
		    channels, channeld, width, height, ncmask);
		break;

	case MLIB_BIT:
	default:
/* MLIB_BIT is not supported here */
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
