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

#pragma ident	"@(#)mlib_s_ImageChannelInsert.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageChannelInsert    - Copy the source image into the selected
 *                                   channels of the destination image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageChannelInsert(mlib_image       *dst,
 *                                          const mlib_image *src,
 *                                          mlib_s32         cmask);
 *
 * ARGUMENT
 *      dst     Pointer to destination image.
 *      src     Pointer to source image.
 *      cmask   Destination channel selection mask.
 *              The least significant bit (LSB) is corresponding to the
 *              last channel in the destination image data.
 *              The bits with value 1 stand for the channels selected.
 *              If more than N channels are selected, the leftmost N
 *              channels are inserted, where N is the number of channels
 *              in the source image.
 *
 * RESTRICTION
 *      The src and dst must have the same width, height and data type.
 *      The src and dst can have 1, 2, 3 or 4 channels.
 *      The src and dst can be either MLIB_BYTE, MLIB_SHORT, MLIB_USHORT,
 *      MLIB_INT, MLIB_FLOAT or MLIB_DOUBLE.
 *
 * DESCRIPTION
 *      Copy the source image into the selected channels of the
 *      destination image.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_s_ImageChannelInsert.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageChannelInsert = __mlib_ImageChannelInsert

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageChannelInsert) mlib_ImageChannelInsert
    __attribute__((weak, alias("__mlib_ImageChannelInsert")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageChannelInsert(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 cmask)
{
	const mlib_s32 X16 = 0xF;
	const mlib_s32 X8 = 0x7;
	const mlib_s32 X4 = 0x3;
	const mlib_s32 D1 = MLIB_IMAGE_ONEDVECTOR;
	const mlib_s32 A8D1 = MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_ONEDVECTOR;
	const mlib_s32 A8D2X8 =
	    MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_STRIDE8X | MLIB_IMAGE_WIDTH8X;
	const mlib_s32 A8D2X4 =
	    MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_STRIDE8X | MLIB_IMAGE_WIDTH4X;
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

/* strides in bytes for src */
	mlib_s32 strides;

/* strides in bytes for dst */
	mlib_s32 strided;
	mlib_s32 flags;
	mlib_s32 flagd;
	mlib_s32 dsize;
	mlib_s32 i, bit1count = 0;

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
	flags = mlib_ImageGetFlags(src);
	flagd = mlib_ImageGetFlags(dst);
	dsize = width * height;

/* normalize the cmask, and count the number of bit with value 1 */
	for (i = (channeld - 1); i >= 0; i--) {
		if (((cmask & (1 << i)) != 0) && (bit1count < channels)) {
			ncmask += (1 << i);
			bit1count++;
		}
	}

/*
 * do not support the cases in which the number of selected channels is
 * less than the number of channels in the source image
 */

	if (bit1count < channels) {
		return (MLIB_FAILURE);
	}

	if (((channels == 1) && (channeld == 1)) || ((channels == 2) &&
	    (channeld == 2)) || ((channels == 3) && (channeld == 3)) ||
	    ((channels == 4) && (channeld == 4))) {
		return (__mlib_ImageCopy(dst, src));
	}

	switch (mlib_ImageGetType(src)) {
	case MLIB_BYTE:

		if (channels == 1) {
			switch (channeld) {
			case 2:

				if (((flags & D1) == 0) &&
				    ((flagd & D1) == 0) &&
				    ((((mlib_addr)sp) & X16) == 0) &&
				    ((((mlib_addr)dp) & X16) == 0)) {
					mlib_s_ImageChannelInsert_U8_12_A8D1X8(
					    (mlib_u8 *)sp, (mlib_u8 *)dp, dsize,
					    ncmask);
				} else if (((((mlib_addr)sp) & X16) == 0) &&
				    ((((mlib_addr)dp) & X16) == 0) &&
				    ((strides & X16) == 0) &&
				    ((strided & X16) == 0)) {
					mlib_s_ImageChannelInsert_U8_12_A8D2X8(
					    (mlib_u8 *)sp, strides,
					    (mlib_u8 *)dp, strided, width,
					    height, ncmask);
				} else if (((flags & D1) == 0) &&
				    ((flagd & D1) == 0)) {
					mlib_s_ImageChannelInsert_U8_12_D1(
					    (mlib_u8 *)sp, (mlib_u8 *)dp, dsize,
					    ncmask);
				} else {
					mlib_s_ImageChannelInsert_U8_12((mlib_u8
					    *)sp, strides, (mlib_u8 *)dp,
					    strided, width, height, ncmask);
				}
				break;

			case 3:
				mlib_s_ImageChannelInsert_U8((mlib_u8 *)sp,
				    strides, (mlib_u8 *)dp, strided, channels,
				    channeld, width, height, ncmask);
				break;

			case 4:

				if (((flags & D1) == 0) &&
				    ((flagd & D1) == 0) &&
				    ((((mlib_addr)sp) & X16) == 0) &&
				    ((((mlib_addr)dp) & X16) == 0)) {
					mlib_s_ImageChannelInsert_U8_14_A8D1X8(
					    (mlib_u8 *)sp, (mlib_u8 *)dp, dsize,
					    ncmask);
				} else if (((((mlib_addr)sp) & X16) == 0) &&
				    ((((mlib_addr)dp) & X16) == 0) &&
				    ((strides & X16) == 0) &&
				    ((strided & X16) == 0)) {
					mlib_s_ImageChannelInsert_U8_14_A8D2X8(
					    (mlib_u8 *)sp, strides,
					    (mlib_u8 *)dp, strided, width,
					    height, ncmask);
				} else if (((flags & D1) == 0) &&
				    ((flagd & D1) == 0)) {
					mlib_s_ImageChannelInsert_U8_14_D1(
					    (mlib_u8 *)sp, (mlib_u8 *)dp, dsize,
					    ncmask);
				} else {
					mlib_s_ImageChannelInsert_U8_14((mlib_u8
					    *)sp, strides, (mlib_u8 *)dp,
					    strided, width, height, ncmask);
				}

				break;

			default:
				return (MLIB_FAILURE);
			}
		} else {

				mlib_s_ImageChannelInsert_U8((mlib_u8 *)sp,
				    strides, (mlib_u8 *)dp, strided, channels,
				    channeld, width, height, ncmask);
		}

		break;

	case MLIB_SHORT:
	case MLIB_USHORT:

		if (channels == 1) {
			switch (channeld) {
			case 2:

				if (((flags & D1) == 0) &&
				    ((flagd & D1) == 0) &&
				    ((((mlib_addr)sp) & X16) == 0) &&
				    ((((mlib_addr)dp) & X16) == 0)) {
					mlib_s_ImageChannelInsert_S16_12_A8D1X4(
					    (mlib_s16 *)sp, (mlib_s16 *)dp,
					    dsize, ncmask);
				} else if (((((mlib_addr)sp) & X16) == 0) &&
				    ((((mlib_addr)dp) & X16) == 0) &&
				    ((strides & X16) == 0) &&
				    ((strided & X16) == 0)) {
					mlib_s_ImageChannelInsert_S16_12_A8D2X4(
					    (mlib_s16 *)sp, strides,
					    (mlib_s16 *)dp, strided, width,
					    height, ncmask);
				} else if (((flags & D1) == 0) &&
				    ((flagd & D1) == 0)) {
					mlib_s_ImageChannelInsert_S16_12_D1(
					    (mlib_s16 *)sp, (mlib_s16 *)dp,
					    dsize, ncmask);
				} else {
					mlib_s_ImageChannelInsert_S16_12(
					    (mlib_s16 *)sp, strides,
					    (mlib_s16 *)dp, strided, width,
					    height, ncmask);
				}

				break;

			case 3:
				mlib_s_ImageChannelInsert_S16((mlib_s16 *)sp,
				    strides, (mlib_s16 *)dp, strided, channels,
				    channeld, width, height, ncmask);

				break;

			case 4:
				mlib_s_ImageChannelInsert_S16((mlib_s16 *)sp,
				    strides, (mlib_s16 *)dp, strided, channels,
				    channeld, width, height, ncmask);

				break;
			default:
				return (MLIB_FAILURE);
			}
		} else {
			mlib_s_ImageChannelInsert_S16((mlib_s16 *)sp, strides,
			    (mlib_s16 *)dp, strided, channels, channeld, width,
			    height, ncmask);
		}

		break;

	case MLIB_INT:
		mlib_s_ImageChannelInsert_S32((mlib_s32 *)sp, strides,
		    (mlib_s32 *)dp, strided, channels, channeld, width, height,
		    ncmask);
		break;

	case MLIB_FLOAT:
		mlib_s_ImageChannelInsert_S32((mlib_s32 *)sp, strides,
		    (mlib_s32 *)dp, strided, channels, channeld, width, height,
		    ncmask);
		break;

	case MLIB_DOUBLE:
		mlib_s_ImageChannelInsert_D64((mlib_d64 *)sp, strides,
		    (mlib_d64 *)dp, strided, channels, channeld, width, height,
		    ncmask);
		break;

	case MLIB_BIT:
	default:
/* MLIB_BIT is not supported here */
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
