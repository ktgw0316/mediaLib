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
 *    The src and dst can be either MLIB_BYTE, MLIB_SHORT, MLIB_USHORT,
 *    MLIB_INT, MLIB_FLOAT or  MLIB_DOUBLE.
 *
 * DESCRIPTION
 *    Copy the selected channels of the source image into the
 *    destination image
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageChannelExtract.h>

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

/* offset of first selected channel */
	mlib_s32 delta0 = 0;

/* number of channels in first group */
	mlib_s32 count1 = 0;
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
				} else if (((flags & MLIB_IMAGE_ONEDVECTOR) ==
				    0) &&
				    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
					mlib_v_ImageChannelExtract_U8_21_D1(
					    (mlib_u8 *)sp, (mlib_u8 *)dp, dsize,
					    ncmask);
				} else {
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
		} else if ((channels == 4) &&
				(channeld == 3) && (ncmask == 7)) {
			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X8) == 0)) {
				mlib_v_ImageChannelExtract_U8_43R_A8D1X8(
				    (mlib_u8 *)sp, (mlib_u8 *)dp, dsize);
			} else if (((flags & A8D2X8) == 0) &&
			    ((flagd & A8D2X8) == 0)) {
				mlib_v_ImageChannelExtract_U8_43R_A8D2X8(
				    (mlib_u8 *)sp, strides, (mlib_u8 *)dp,
				    strided, width, height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageChannelExtract_U8_43R_D1((mlib_u8 *)
				    sp, (mlib_u8 *)dp, dsize);
			} else {
				mlib_v_ImageChannelExtract_U8_43R((mlib_u8 *)sp,
				    strides, (mlib_u8 *)dp, strided, width,
				    height);
			}

			return (MLIB_SUCCESS);
		} else if ((channels == 4) &&
				(channeld == 3) && (ncmask == 14)) {
			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X8) == 0)) {
				mlib_v_ImageChannelExtract_U8_43L_A8D1X8(
				    (mlib_u8 *)sp, (mlib_u8 *)dp, dsize);
			} else if (((flags & A8D2X8) == 0) &&
			    ((flagd & A8D2X8) == 0)) {
				mlib_v_ImageChannelExtract_U8_43L_A8D2X8(
				    (mlib_u8 *)sp, strides, (mlib_u8 *)dp,
				    strided, width, height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageChannelExtract_U8_43L_D1((mlib_u8 *)
				    sp, (mlib_u8 *)dp, dsize);
			} else {
				mlib_v_ImageChannelExtract_U8_43L((mlib_u8 *)sp,
				    strides, (mlib_u8 *)dp, strided, width,
				    height);
			}

			return (MLIB_SUCCESS);
		}

		break;

	case MLIB_SHORT:
	case MLIB_USHORT:

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
		} else if ((channels == 4) &&
				(channeld == 3) && (ncmask == 7)) {
			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X4) == 0)) {
				mlib_v_ImageChannelExtract_S16_43R_A8D1X4(
				    (mlib_s16 *)sp, (mlib_s16 *)dp, dsize);
			} else if (((flags & A8D2X4) == 0) &&
			    ((flagd & A8D2X4) == 0)) {
				mlib_v_ImageChannelExtract_S16_43R_A8D2X4(
				    (mlib_s16 *)sp, strides, (mlib_s16 *)dp,
				    strided, width, height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageChannelExtract_S16_43R_D1((mlib_s16
				    *)sp, (mlib_s16 *)dp, dsize);
			} else {
				mlib_v_ImageChannelExtract_S16_43R((mlib_s16 *)
				    sp, strides, (mlib_s16 *)dp, strided, width,
				    height);
			}

			return (MLIB_SUCCESS);
		} else if ((channels == 4) &&
				(channeld == 3) && (ncmask == 14)) {
			if (((flags & A8D1) == 0) && ((flagd & A8D1) == 0) &&
			    ((dsize & X4) == 0)) {
				mlib_v_ImageChannelExtract_S16_43L_A8D1X4(
				    (mlib_s16 *)sp, (mlib_s16 *)dp, dsize);
			} else if (((flags & A8D2X4) == 0) &&
			    ((flagd & A8D2X4) == 0)) {
				mlib_v_ImageChannelExtract_S16_43L_A8D2X4(
				    (mlib_s16 *)sp, strides, (mlib_s16 *)dp,
				    strided, width, height);
			} else if (((flags & MLIB_IMAGE_ONEDVECTOR) == 0) &&
			    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {
				mlib_v_ImageChannelExtract_S16_43L_D1((mlib_s16
				    *)sp, (mlib_s16 *)dp, dsize);
			} else {
				mlib_v_ImageChannelExtract_S16_43L((mlib_s16 *)
				    sp, strides, (mlib_s16 *)dp, strided, width,
				    height);
			}

			return (MLIB_SUCCESS);
		}

		break;
	}

/* *********************************************************** */

/* From C version */

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
	    {
		    mlib_u8 *sl = (mlib_u8 *)sp + delta0;
		    mlib_u8 *dl = (mlib_u8 *)dp;

		    switch (channels * 10 + channeld) {
		    case 32:
			    mlib_v_ImageChannelExtract_U8_3_2(sl, strides, dl,
				strided, width, height, count1);
			    return (MLIB_SUCCESS);

		    case 42:

/* mask 1010 or 0101 */
			    if (ncmask == 0xA || ncmask == 0x5) {
				    mlib_v_ImageChannelExtract_U8_2_1(sl,
					strides, dl, strided, 2 * width,
					height);
				    return (MLIB_SUCCESS);
			    }

			    mlib_v_ImageChannelExtract_U8_4_2(sl, strides, dl,
				strided, width, height, count1);
			    return (MLIB_SUCCESS);

		    case 43:
			    mlib_v_ImageChannelExtract_U8((mlib_u8 *)sp,
				strides, (mlib_u8 *)dp, strided, channels,
				channeld, width, height, ncmask);
			    return (MLIB_SUCCESS);

		    default:
			    return (MLIB_FAILURE);
		    }
	    }

	case MLIB_USHORT:
	case MLIB_SHORT:
		mlib_v_ImageChannelExtract_S16((mlib_u16 *)sp, strides,
		    (mlib_u16 *)dp, strided,
		    channels, channeld, width, height, ncmask);
		break;

	case MLIB_INT:
	case MLIB_FLOAT:
	    {
		    mlib_f32 *sl = (mlib_f32 *)sp + delta0;
		    mlib_f32 *dl = (mlib_f32 *)dp;

		    strides /= 4;
		    strided /= 4;

		    switch (channels * 10 + channeld) {
		    case 21:
			    mlib_v_ImageChannelExtract_32_2_1(sl, strides, dl,
				strided, width, height);
			    return (MLIB_SUCCESS);

		    case 31:
			    mlib_v_ImageChannelExtract_32_3_1(sl, strides, dl,
				strided, width, height);
			    return (MLIB_SUCCESS);

		    case 32:
			    mlib_v_ImageChannelExtract_32_3_2(sl, strides, dl,
				strided, width, height, count1);
			    return (MLIB_SUCCESS);

		    case 41:
			    mlib_v_ImageChannelExtract_32_4_1(sl, strides, dl,
				strided, width, height);
			    return (MLIB_SUCCESS);

		    case 42:

/* mask 1010 or 0101 */
			    if (ncmask == 0xA || ncmask == 0x5) {
				    mlib_v_ImageChannelExtract_32_2_1(sl,
					strides, dl, strided, 2 * width,
					height);
			    } else {
				    mlib_v_ImageChannelExtract_32_4_2(sl,
					strides, dl, strided, width, height,
					count1);
			    }

			    return (MLIB_SUCCESS);

		    case 43:
			    mlib_v_ImageChannelExtract_32_4_3(sl, strides, dl,
				strided, width, height, count1);
			    return (MLIB_SUCCESS);

		    default:
			    return (MLIB_FAILURE);
		    }
	    }

	case MLIB_DOUBLE:
		mlib_v_ImageChannelExtract_D64((mlib_d64 *)sp, strides,
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
