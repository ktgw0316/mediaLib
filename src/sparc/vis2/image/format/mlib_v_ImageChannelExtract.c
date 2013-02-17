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

#pragma ident	"@(#)mlib_v_ImageChannelExtract.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageChannelExtract  - Copy the selected channels of the source
 *                                  image into the destination image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageChannelExtract(mlib_image       *dst,
 *                                           const mlib_image *src,
 *                                           mlib_s32         cmask);
 * ARGUMENT
 *    dst     Pointer to destination image.
 *    src     Pointer to source image.
 *    cmask   Source channel selection mask.
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
 *    The src and dst can be either
 *    MLIB_BYTE, MLIB_SHORT, MLIB_USHORT,  MLIB_INT,
 *    MLIB_FLOAT or  MLIB_DOUBLE.
 *
 * DESCRIPTION
 *    Copy the selected channels of the source image into the
 *    destination image
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageChannelExtract.h>
#include <mlib_v_ImageChannelExtract_2.h>

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

static const mlib_s32 mlib_bmask_U8_43R[] =
{ 0x1235679A, 0x35679ABD, 0x679ABDEF };
static const mlib_s32 mlib_bmask_U8_43L[] =
{ 0x01245689, 0x245689AC, 0x5689ACDE };
static const mlib_s32 mlib_bmask_U8_11[] =
{ 0x0234678A, 0x34678ABC, 0x678ABCEF };
static const mlib_s32 mlib_bmask_U8_13[] =
{ 0x01345789, 0x345789BC, 0x5789BCDF };
static const mlib_s32 mlib_bmask_S16_43R[] =
{ 0x234567AB, 0x4567ABCD, 0x67ABCDEF };
static const mlib_s32 mlib_bmask_S16_43L[] =
{ 0x01234589, 0x234589AB, 0x4589ABCD };
static const mlib_s32 mlib_bmask_S16_11[] =
{ 0x01456789, 0x456789CD, 0x6789CDEF };
static const mlib_s32 mlib_bmask_S16_13[] =
{ 0x01236789, 0x236789AB, 0x6789ABEF };

/* *********************************************************** */

mlib_status
__mlib_ImageChannelExtract(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 cmask)
{
	const mlib_s32 X8 = 0x7;
	const mlib_s32 X4 = 0x3;
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

	switch (mlib_ImageGetType(src)) {
	case MLIB_BYTE:

		if (channeld == 1) {
			switch (channels) {
			case 2:

				if (((flags & A8D1) == 0) &&
				    ((flagd & A8D1) == 0) &&
				    ((dsize & X8) == 0)) {
					mlib_v_ImageChannelExtract_U8_21_A8D1X8(
					    (mlib_u8 *)sp, (mlib_u8 *)dp, dsize,
					    ncmask);
				} else if (((flags & A8D2X8) == 0) &&
				    ((flagd & A8D2X8) == 0)) {
					mlib_v_ImageChannelExtract_U8_21_A8D2X8(
					    (mlib_u8 *)sp, strides,
					    (mlib_u8 *)dp, strided, width,
					    height, ncmask);
				}

/*
 *            else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
 *                     ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
 *              mlib_v_ImageChannelExtract_U8_21_D1((mlib_u8 *)sp,
 *                                                  (mlib_u8 *)dp,
 *                                                  dsize,
 *                                                  ncmask);
 *            }
 *
 */
				else {
					mlib_v_ImageChannelExtract_U8_21(
					    (mlib_u8 *)sp, strides,
					    (mlib_u8 *)dp, strided, width,
					    height, ncmask);
				}

				return (MLIB_SUCCESS);

			case 3:

				if (((flags & A8D1) == 0) &&
				    ((flagd & A8D1) == 0) &&
				    ((dsize & X8) == 0)) {
					mlib_v_ImageChannelExtract_U8_31_A8D1X8(
					    (mlib_u8 *)sp, (mlib_u8 *)dp, dsize,
					    ncmask);
				} else if (((flags & A8D2X8) == 0) &&
				    ((flagd & A8D2X8) == 0)) {
					mlib_v_ImageChannelExtract_U8_31_A8D2X8(
					    (mlib_u8 *)sp, strides,
					    (mlib_u8 *)dp, strided, width,
					    height, ncmask);
				} else if (((flags & MLIB_IMAGE_ONEDVECTOR) ==
				    0) &&
				    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
					mlib_v_ImageChannelExtract_U8_31_D1(
					    (mlib_u8 *)sp, (mlib_u8 *)dp, dsize,
					    ncmask);
				} else {
					mlib_v_ImageChannelExtract_U8_31(
					    (mlib_u8 *)sp, strides,
					    (mlib_u8 *)dp, strided, width,
					    height, ncmask);
				}

				return (MLIB_SUCCESS);

			case 4:

				if (((flags & A8D1) == 0) &&
				    ((flagd & A8D1) == 0) &&
				    ((dsize & X8) == 0)) {
					mlib_v_ImageChannelExtract_U8_41_A8D1X8(
					    (mlib_u8 *)sp, (mlib_u8 *)dp, dsize,
					    ncmask);
				} else if (((flags & A8D2X8) == 0) &&
				    ((flagd & A8D2X8) == 0)) {
					mlib_v_ImageChannelExtract_U8_41_A8D2X8(
					    (mlib_u8 *)sp, strides,
					    (mlib_u8 *)dp, strided, width,
					    height, ncmask);
				} else if (((flags & MLIB_IMAGE_ONEDVECTOR) ==
				    0) &&
				    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
					mlib_v_ImageChannelExtract_U8_41_D1(
					    (mlib_u8 *)sp, (mlib_u8 *)dp, dsize,
					    ncmask);
				} else {
					mlib_v_ImageChannelExtract_U8_41(
					    (mlib_u8 *)sp, strides,
					    (mlib_u8 *)dp, strided, width,
					    height, ncmask);
				}

				return (MLIB_SUCCESS);

			default:
				return (MLIB_FAILURE);
			}
		} else if (channeld == 2) {
			switch (channels) {

			case 3:

				if (((flags & A8D1) == 0) &&
				    ((flagd & A8D1) == 0) &&
				    ((dsize & X8) == 0)) {
					mlib_v_ImageChannelExtract_U8_32_A8D1X8(
					    (mlib_u8 *)sp, (mlib_u8 *)dp, dsize,
					    ncmask);
				} else if (((flags & A8D2X8) == 0) &&
				    ((flagd & A8D2X8) == 0)) {
					mlib_v_ImageChannelExtract_U8_32_A8D2X8(
					    (mlib_u8 *)sp, strides,
					    (mlib_u8 *)dp, strided, width,
					    height, ncmask);
				} else if (((flags & MLIB_IMAGE_ONEDVECTOR) ==
				    0) &&
				    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
					mlib_v_ImageChannelExtract_U8_32_D1(
					    (mlib_u8 *)sp, (mlib_u8 *)dp, dsize,
					    ncmask);
				} else {
					mlib_v_ImageChannelExtract_U8_32(
					    (mlib_u8 *)sp, strides,
					    (mlib_u8 *)dp, strided, width,
					    height, ncmask);
				}

				return (MLIB_SUCCESS);

			case 4:

				if (((flags & A8D1) == 0) &&
				    ((flagd & A8D1) == 0) &&
				    ((dsize & X8) == 0)) {
					mlib_v_ImageChannelExtract_U8_42_A8D1X8(
					    (mlib_u8 *)sp, (mlib_u8 *)dp, dsize,
					    ncmask);
				} else if (((flags & A8D2X8) == 0) &&
				    ((flagd & A8D2X8) == 0)) {
					mlib_v_ImageChannelExtract_U8_42_A8D2X8(
					    (mlib_u8 *)sp, strides,
					    (mlib_u8 *)dp, strided, width,
					    height, ncmask);
				} else if (((flags & MLIB_IMAGE_ONEDVECTOR) ==
				    0) &&
				    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
					mlib_v_ImageChannelExtract_U8_42_D1(
					    (mlib_u8 *)sp, (mlib_u8 *)dp, dsize,
					    ncmask);
				} else {
					mlib_v_ImageChannelExtract_U8_42(
					    (mlib_u8 *)sp, strides,
					    (mlib_u8 *)dp, strided, width,
					    height, ncmask);
				}

				return (MLIB_SUCCESS);

			default:
				return (MLIB_FAILURE);
			}
		} else if ((channels == 4) &&
				(channeld == 3) && (ncmask == 7)) {
			if (((flags | flagd) & A8D2X8) == 0) {
				mlib_v_ImageChannelExtract_43_A8D2X8(sp,
				    strides, dp, strided, width, height,
				    mlib_bmask_U8_43R);
			} else {
				mlib_v_ImageChannelExtract_43(sp, strides, dp,
				    strided, width, height, mlib_bmask_U8_43R);
			}

			return (MLIB_SUCCESS);
		} else if ((channels == 4) &&
				(channeld == 3) && (ncmask == 11)) {
			if (((flags | flagd) & A8D2X8) == 0) {
				mlib_v_ImageChannelExtract_43_A8D2X8(sp,
				    strides, dp, strided, width, height,
				    mlib_bmask_U8_11);
			} else {
				mlib_v_ImageChannelExtract_43(sp, strides, dp,
				    strided, width, height, mlib_bmask_U8_11);
			}

			return (MLIB_SUCCESS);
		} else if ((channels == 4) &&
				(channeld == 3) && (ncmask == 13)) {
			if (((flags | flagd) & A8D2X8) == 0) {
				mlib_v_ImageChannelExtract_43_A8D2X8(sp,
				    strides, dp, strided, width, height,
				    mlib_bmask_U8_13);
			} else {
				mlib_v_ImageChannelExtract_43(sp, strides, dp,
				    strided, width, height, mlib_bmask_U8_13);
			}

			return (MLIB_SUCCESS);
		} else if ((channels == 4) &&
				(channeld == 3) && (ncmask == 14)) {
			if (((flags | flagd) & A8D2X8) == 0) {
				mlib_v_ImageChannelExtract_43_A8D2X8(sp,
				    strides, dp, strided, width, height,
				    mlib_bmask_U8_43L);
			} else {
				mlib_v_ImageChannelExtract_43(sp, strides, dp,
				    strided, width, height, mlib_bmask_U8_43L);
			}

			return (MLIB_SUCCESS);
		}

		break;

	case MLIB_USHORT:
	case MLIB_SHORT:

		if (channeld == 1) {
			switch (channels) {
			case 2:

				if (((flags & A8D1) == 0) &&
				    ((flagd & A8D1) == 0) &&
				    ((dsize & X4) == 0)) {
					mlib_v_ImageChannelExtract_S16_21_A8D1X4
					    ((mlib_s16 *)sp, (mlib_s16 *)dp,
					    dsize, ncmask);
				} else if (((flags & A8D2X4) == 0) &&
				    ((flagd & A8D2X4) == 0)) {
					mlib_v_ImageChannelExtract_S16_21_A8D2X4
					    ((mlib_s16 *)sp, strides,
					    (mlib_s16 *)dp, strided, width,
					    height, ncmask);
				} else if (((flags & MLIB_IMAGE_ONEDVECTOR) ==
				    0) &&
				    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
					mlib_v_ImageChannelExtract_S16_21_D1(
					    (mlib_s16 *)sp, (mlib_s16 *)dp,
					    dsize, ncmask);
				} else {
					mlib_v_ImageChannelExtract_S16_21(
					    (mlib_s16 *)sp, strides,
					    (mlib_s16 *)dp, strided, width,
					    height, ncmask);
				}

				return (MLIB_SUCCESS);

			case 3:

				if (((flags & A8D1) == 0) &&
				    ((flagd & A8D1) == 0) &&
				    ((dsize & X4) == 0)) {
					mlib_v_ImageChannelExtract_S16_31_A8D1X4
					    ((mlib_s16 *)sp, (mlib_s16 *)dp,
					    dsize, ncmask);
				} else if (((flags & A8D2X4) == 0) &&
				    ((flagd & A8D2X4) == 0)) {
					mlib_v_ImageChannelExtract_S16_31_A8D2X4
					    ((mlib_s16 *)sp, strides,
					    (mlib_s16 *)dp, strided, width,
					    height, ncmask);
				} else if (((flags & MLIB_IMAGE_ONEDVECTOR) ==
				    0) &&
				    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
					mlib_v_ImageChannelExtract_S16_31_D1(
					    (mlib_s16 *)sp, (mlib_s16 *)dp,
					    dsize, ncmask);
				} else {
					mlib_v_ImageChannelExtract_S16_31(
					    (mlib_s16 *)sp, strides,
					    (mlib_s16 *)dp, strided, width,
					    height, ncmask);
				}

				return (MLIB_SUCCESS);

			case 4:

				if (((flags & A8D1) == 0) &&
				    ((flagd & A8D1) == 0) &&
				    ((dsize & X4) == 0)) {
					mlib_v_ImageChannelExtract_S16_41_A8D1X4
					    ((mlib_s16 *)sp, (mlib_s16 *)dp,
					    dsize, ncmask);
				} else if (((flags & A8D2X4) == 0) &&
				    ((flagd & A8D2X4) == 0)) {
					mlib_v_ImageChannelExtract_S16_41_A8D2X4
					    ((mlib_s16 *)sp, strides,
					    (mlib_s16 *)dp, strided, width,
					    height, ncmask);
				} else if (((flags & MLIB_IMAGE_ONEDVECTOR) ==
				    0) &&
				    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
					mlib_v_ImageChannelExtract_S16_41_D1(
					    (mlib_s16 *)sp, (mlib_s16 *)dp,
					    dsize, ncmask);
				} else {
					mlib_v_ImageChannelExtract_S16_41(
					    (mlib_s16 *)sp, strides,
					    (mlib_s16 *)dp, strided, width,
					    height, ncmask);
				}

				return (MLIB_SUCCESS);
			default:
				return (MLIB_FAILURE);
			}
		} else if (channeld == 2) {
			switch (channels) {
			case 3:

				if (((flags & A8D1) == 0) &&
				    ((flagd & A8D1) == 0) &&
				    ((dsize & X4) == 0)) {
					mlib_v_ImageChannelExtract_S16_32_A8D1X4
					    ((mlib_s16 *)sp, (mlib_s16 *)dp,
					    dsize, ncmask);
				} else if (((flags & A8D2X4) == 0) &&
				    ((flagd & A8D2X4) == 0)) {
					mlib_v_ImageChannelExtract_S16_32_A8D2X4
					    ((mlib_s16 *)sp, strides,
					    (mlib_s16 *)dp, strided, width,
					    height, ncmask);
				} else if (((flags & MLIB_IMAGE_ONEDVECTOR) ==
				    0) &&
				    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
					mlib_v_ImageChannelExtract_S16_32_D1(
					    (mlib_s16 *)sp, (mlib_s16 *)dp,
					    dsize, ncmask);
				} else {
					mlib_v_ImageChannelExtract_S16_32(
					    (mlib_s16 *)sp, strides,
					    (mlib_s16 *)dp, strided, width,
					    height, ncmask);
				}

				return (MLIB_SUCCESS);

			case 4:

				if (((flags & A8D1) == 0) &&
				    ((flagd & A8D1) == 0) &&
				    ((dsize & X4) == 0)) {
					mlib_v_ImageChannelExtract_S16_42_A8D1X4
					    ((mlib_s16 *)sp, (mlib_s16 *)dp,
					    dsize, ncmask);
				} else if (((flags & A8D2X4) == 0) &&
				    ((flagd & A8D2X4) == 0)) {
					mlib_v_ImageChannelExtract_S16_42_A8D2X4
					    ((mlib_s16 *)sp, strides,
					    (mlib_s16 *)dp, strided, width,
					    height, ncmask);
				} else if (((flags & MLIB_IMAGE_ONEDVECTOR) ==
				    0) &&
				    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
					mlib_v_ImageChannelExtract_S16_42_D1(
					    (mlib_s16 *)sp, (mlib_s16 *)dp,
					    dsize, ncmask);
				} else {
					mlib_v_ImageChannelExtract_S16_42(
					    (mlib_s16 *)sp, strides,
					    (mlib_s16 *)dp, strided, width,
					    height, ncmask);
				}

				return (MLIB_SUCCESS);
			default:
				return (MLIB_FAILURE);
			}
		} else if ((channels == 4) &
				(channeld == 3) && (ncmask == 7)) {
			if (((flags | flagd) & A8D2X4) == 0) {
				mlib_v_ImageChannelExtract_43_A8D2X8(sp,
				    strides, dp, strided, 2 * width, height,
				    mlib_bmask_S16_43R);
			} else {
				mlib_v_ImageChannelExtract_43(sp, strides, dp,
				    strided, 2 * width, height,
				    mlib_bmask_S16_43R);
			}

			return (MLIB_SUCCESS);
		} else if ((channels == 4) &&
				(channeld == 3) && (ncmask == 11)) {
			if (((flags | flagd) & A8D2X4) == 0) {
				mlib_v_ImageChannelExtract_43_A8D2X8(sp,
				    strides, dp, strided, 2 * width, height,
				    mlib_bmask_S16_11);
			} else {
				mlib_v_ImageChannelExtract_43(sp, strides, dp,
				    strided, 2 * width, height,
				    mlib_bmask_S16_11);
			}

			return (MLIB_SUCCESS);
		} else if ((channels == 4) &&
				(channeld == 3) && (ncmask == 13)) {
			if (((flags | flagd) & A8D2X4) == 0) {
				mlib_v_ImageChannelExtract_43_A8D2X8(sp,
				    strides, dp, strided, 2 * width, height,
				    mlib_bmask_S16_13);
			} else {
				mlib_v_ImageChannelExtract_43(sp, strides, dp,
				    strided, 2 * width, height,
				    mlib_bmask_S16_13);
			}

			return (MLIB_SUCCESS);
		} else if ((channels == 4) &&
				(channeld == 3) && (ncmask == 14)) {
			if (((flags | flagd) & A8D2X4) == 0) {
				mlib_v_ImageChannelExtract_43_A8D2X8(sp,
				    strides, dp, strided, 2 * width, height,
				    mlib_bmask_S16_43L);
			} else {
				mlib_v_ImageChannelExtract_43(sp, strides, dp,
				    strided, 2 * width, height,
				    mlib_bmask_S16_43L);
			}

			return (MLIB_SUCCESS);
		}

		break;

	case MLIB_INT:
	case MLIB_FLOAT:

		if (channeld == 1) {
			switch (channels) {
			case 2:

				mlib_v_ImageChannelExtract_32_21((mlib_f32 *)sp,
				    strides, (mlib_f32 *)dp, strided, width,
				    height, ncmask);
				return (MLIB_SUCCESS);

			case 3:
				mlib_v_ImageChannelExtract_32_31((mlib_f32 *)sp,
				    strides, (mlib_f32 *)dp, strided, width,
				    height, ncmask);
				return (MLIB_SUCCESS);

			case 4:
				mlib_v_ImageChannelExtract_32_41((mlib_f32 *)sp,
				    strides, (mlib_f32 *)dp, strided, width,
				    height, ncmask);
				return (MLIB_SUCCESS);
			default:
				return (MLIB_FAILURE);
			}
		} else if (channeld == 2) {
			switch (channels) {
			case 3:
				mlib_v_ImageChannelExtract_32_32((mlib_f32 *)sp,
				    strides, (mlib_f32 *)dp, strided, width,
				    height, ncmask);
				return (MLIB_SUCCESS);

			case 4:
				mlib_v_ImageChannelExtract_32_42((mlib_f32 *)sp,
				    strides, (mlib_f32 *)dp, strided, width,
				    height, ncmask);
				return (MLIB_SUCCESS);
			default:
				return (MLIB_FAILURE);
			}
		} else if (channeld == 3) {
			switch (channels) {

			case 4:
				mlib_v_ImageChannelExtract_32_43((mlib_f32 *)sp,
				    strides, (mlib_f32 *)dp, strided, width,
				    height, ncmask);
				return (MLIB_SUCCESS);
			default:
				return (MLIB_FAILURE);
			}
		}

		break;

	case MLIB_DOUBLE:

		if (channeld == 1) {
			switch (channels) {
			case 2:

				mlib_v_ImageChannelExtract_D64_21((mlib_d64 *)
				    sp, strides, (mlib_d64 *)dp, strided, width,
				    height, ncmask);
				return (MLIB_SUCCESS);

			case 3:
				mlib_v_ImageChannelExtract_D64_31((mlib_d64 *)
				    sp, strides, (mlib_d64 *)dp, strided, width,
				    height, ncmask);
				return (MLIB_SUCCESS);

			case 4:
				mlib_v_ImageChannelExtract_D64_41((mlib_d64 *)
				    sp, strides, (mlib_d64 *)dp, strided, width,
				    height, ncmask);
				return (MLIB_SUCCESS);
			default:
				return (MLIB_FAILURE);
			}
		} else if (channeld == 2) {
			switch (channels) {
			case 3:
				mlib_v_ImageChannelExtract_D64_32((mlib_d64 *)
				    sp, strides, (mlib_d64 *)dp, strided, width,
				    height, ncmask);
				return (MLIB_SUCCESS);

			case 4:
				mlib_v_ImageChannelExtract_D64_42((mlib_d64 *)
				    sp, strides, (mlib_d64 *)dp, strided, width,
				    height, ncmask);
				return (MLIB_SUCCESS);
			default:
				return (MLIB_FAILURE);
			}
		} else if (channeld == 3) {
			switch (channels) {

			case 4:
				mlib_v_ImageChannelExtract_D64_43((mlib_d64 *)
				    sp, strides, (mlib_d64 *)dp, strided, width,
				    height, ncmask);
				return (MLIB_SUCCESS);
			default:
				return (MLIB_FAILURE);
			}
		}

		break;

	case MLIB_BIT:
	default:
/* MLIB_BIT is not supported here */
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_U8_43R_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
	mlib_v_ImageChannelExtract_43(src, 0, dst, 0, dsize, 1,
	    mlib_bmask_U8_43R);
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_S16_43R_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
	mlib_v_ImageChannelExtract_43((void *)src, 0, (void *)dst, 0, 2 * dsize,
	    1, mlib_bmask_S16_43R);
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_U8_43L_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
	mlib_v_ImageChannelExtract_43(src, 0, dst, 0, dsize, 1,
	    mlib_bmask_U8_43L);
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_S16_43L_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
	mlib_v_ImageChannelExtract_43((void *)src, 0, (void *)dst, 0, 2 * dsize,
	    1, mlib_bmask_S16_43L);
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_S16_43L_A8D1X4(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
	mlib_v_ImageChannelExtract_43_A8D2X8((void *)src, 0, (void *)dst, 0,
	    2 * dsize, 1, mlib_bmask_S16_43L);
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_U8_43_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 ncmask)
{
	if (ncmask == 7) {
		mlib_v_ImageChannelExtract_43(src, 0, dst, 0, dsize, 1,
		    mlib_bmask_U8_43R);
	} else if (ncmask == 11) {
		mlib_v_ImageChannelExtract_43(src, 0, dst, 0, dsize, 1,
		    mlib_bmask_U8_11);
	} else if (ncmask == 13) {
		mlib_v_ImageChannelExtract_43(src, 0, dst, 0, dsize, 1,
		    mlib_bmask_U8_13);
	} else if (ncmask == 14) {
		mlib_v_ImageChannelExtract_43(src, 0, dst, 0, dsize, 1,
		    mlib_bmask_U8_43L);
	}
}

/* *********************************************************** */
