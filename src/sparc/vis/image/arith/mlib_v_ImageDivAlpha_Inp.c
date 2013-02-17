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

#pragma ident	"@(#)mlib_v_ImageDivAlpha_Inp.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageDivAlpha_Inp - divide color channels by the alpha channel
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageDivAlpha_Inp(mlib_image *img,
 *                                         mlib_s32   cmask)
 *
 * ARGUMENTS
 *    img     Pointer to the image.
 *    cmask   Channel mask to indicate the alpha channel.
 *            Each bit of which represents a channel in the image.
 *            The channel corresponded to the 1 bit is the alpha channel.
 *
 * RESTRICTION
 *      The image can have 2, 3 or 4 channels. The image can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      Divide color channels by the alpha channel in the way of
 *      pixel by pixel.
 *
 *  The image must have at least two channels.
 *
 *  For an MLIB_BYTE image
 *    img->data[i][j] /= img->data[i][a] * pow(2, -8)
 *  For an MLIB_SHORT image
 *    img->data[i][j] /= img->data[i][a] * pow(2, -15)
 *  For an MLIB_USHORT image
 *    img->data[i][j] /= img->data[i][a] * pow(2, -16)
 *  For an MLIB_INT image
 *    img->data[i][j] /= img->data[i][a] * pow(2, -31)
 *  where, j != a
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageDivAlpha_f.h>
#include <mlib_v_ImageDivAlpha.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageDivAlpha_Inp = __mlib_ImageDivAlpha_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageDivAlpha_Inp) mlib_ImageDivAlpha_Inp
    __attribute__((weak, alias("__mlib_ImageDivAlpha_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageDivAlpha_Inp(
    mlib_image *src,
    mlib_s32 cmask)
{
	mlib_s32 chan, width, height, stride, flag;
	void *data;
	mlib_type type;
	mlib_s32 alpha;
	mlib_s32 dsize, lsize;
	const mlib_s32 X8 = 0x7;
	const mlib_s32 A8D1 = MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_ONEDVECTOR;
	const mlib_s32 A8D2 = MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_STRIDE8X;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_GET_ALL_PARAMS(src, type, chan, width, height, stride, data);
	flag = mlib_ImageGetFlags(src);

	if ((chan < 2) || (cmask == 0))
		return (MLIB_OUTOFRANGE);

	for (alpha = 0; (cmask & 0x1) == 0; alpha++)
		cmask >>= 1;

	if ((cmask >> 1) != 0)
		return (MLIB_OUTOFRANGE);

/*  0 <= alpha < channel */
	alpha = chan - alpha - 1;

	if (0 > alpha && alpha >= chan)
		return (MLIB_OUTOFRANGE);

	dsize = width * height * chan;
	lsize = width * chan;

	if (type == MLIB_BYTE) {
		if (((flag & A8D1) == 0) && ((dsize & X8) == 0)) {

			if (chan == 4) {
				switch (alpha) {
				case 0:
					mlib_v_ImageDivAlpha_Inp_U8_4_NOOFF_1(
					    (mlib_d64 *)data, dsize);
					break;
				case 1:
					mlib_v_ImageDivAlpha_Inp_U8_4_NOOFF_2(
					    (mlib_d64 *)data, dsize);
					break;
				case 2:
					mlib_v_ImageDivAlpha_Inp_U8_4_NOOFF_3(
					    (mlib_d64 *)data, dsize);
					break;
				case 3:
					mlib_v_ImageDivAlpha_Inp_U8_4_NOOFF_4(
					    (mlib_d64 *)data, dsize);
					break;
				}
			} else if (chan == 3) {
				mlib_v_ImageDivAlpha_Inp_U8_3((mlib_u8 *)data,
				    stride, width, height, alpha);
			} else if (chan == 2) {
				switch (alpha) {
				case 0:
					mlib_v_ImageDivAlpha_Inp_U8_2_NOOFF_1(
					    (mlib_d64 *)data, dsize);
					break;
				case 1:
					mlib_v_ImageDivAlpha_Inp_U8_2_NOOFF_2(
					    (mlib_d64 *)data, dsize);
					break;
				}
			}
		} else if (((flag & A8D2) == 0) && ((lsize & X8) == 0)) {

			if (chan == 4) {
				switch (alpha) {
				case 0:
					mlib_v_ImageDivAlpha_Inp_U8_4_HALFOFF_1(
					    (mlib_d64 *)data, stride, width,
					    height);
					break;
				case 1:
					mlib_v_ImageDivAlpha_Inp_U8_4_HALFOFF_2(
					    (mlib_d64 *)data, stride, width,
					    height);
					break;
				case 2:
					mlib_v_ImageDivAlpha_Inp_U8_4_HALFOFF_3(
					    (mlib_d64 *)data, stride, width,
					    height);
					break;
				case 3:
					mlib_v_ImageDivAlpha_Inp_U8_4_HALFOFF_4(
					    (mlib_d64 *)data, stride, width,
					    height);
					break;
				}
			} else if (chan == 3) {
				mlib_v_ImageDivAlpha_Inp_U8_3((mlib_u8 *)data,
				    stride, width, height, alpha);
			} else if (chan == 2) {
				switch (alpha) {
				case 0:
					mlib_v_ImageDivAlpha_Inp_U8_2_HALFOFF_1(
					    (mlib_d64 *)data, stride, width,
					    height);
					break;
				case 1:
					mlib_v_ImageDivAlpha_Inp_U8_2_HALFOFF_2(
					    (mlib_d64 *)data, stride, width,
					    height);
					break;
				}
			}
		} else {

			if (chan == 4) {
				return mlib_ImageDivAlpha_U8(data, stride, data,
				    stride, width, height, chan, alpha);
			} else if (chan == 3) {
				mlib_v_ImageDivAlpha_Inp_U8_3((mlib_u8 *)data,
				    stride, width, height, alpha);
			} else if (chan == 2) {
				switch (alpha) {
				case 0:
					mlib_v_ImageDivAlpha_Inp_U8_2_OFF_1(
					    (mlib_u8 *)data, stride, width,
					    height);
					break;
				case 1:
					mlib_v_ImageDivAlpha_Inp_U8_2_OFF_2(
					    (mlib_u8 *)data, stride, width,
					    height);
					break;
				}
			}
		}

		return (MLIB_SUCCESS);
	} else if (type == MLIB_SHORT) {
		stride >>= 1;

		if (chan == 4) {
			mlib_v_ImageDivAlpha_Inp_S16_4((mlib_s16 *)data, stride,
			    width, height, alpha);
		} else if (chan == 3) {
			mlib_v_ImageDivAlpha_Inp_S16_3((mlib_s16 *)data, stride,
			    width, height, alpha);
		} else if (chan == 2) {
			mlib_v_ImageDivAlpha_Inp_S16_2((mlib_s16 *)data, stride,
			    width, height, alpha);
		}

		return (MLIB_SUCCESS);
	} else if (type == MLIB_USHORT) {
		stride >>= 1;

		if (chan == 4) {
			mlib_v_ImageDivAlpha_Inp_U16_4((mlib_u16 *)data, stride,
			    width, height, alpha);
		} else if (chan == 3) {
			mlib_v_ImageDivAlpha_Inp_U16_3((mlib_u16 *)data, stride,
			    width, height, alpha);
		} else if (chan == 2) {
			mlib_v_ImageDivAlpha_Inp_U16_2((mlib_u16 *)data, stride,
			    width, height, alpha);
		}

		return (MLIB_SUCCESS);
	} else if (type == MLIB_INT) {
		stride >>= 2;

		if (chan == 4) {
			mlib_v_ImageDivAlpha_Inp_S32_4((mlib_s32 *)data, stride,
			    width, height, alpha);
		} else if (chan == 3) {
			mlib_v_ImageDivAlpha_Inp_S32_3((mlib_s32 *)data, stride,
			    width, height, alpha);
		} else if (chan == 2) {
			mlib_v_ImageDivAlpha_Inp_S32_2((mlib_s32 *)data, stride,
			    width, height, alpha);
		}

		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */
