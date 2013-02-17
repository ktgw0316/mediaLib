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

#pragma ident	"@(#)mlib_v_ImageNot_Bit.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageNot   - not an bit type image (VIS version)
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageNot(mlib_image       *dst,
 *                                const mlib_image *src);
 *
 * ARGUMENT
 *      dst     pointer to destination image
 *      src     pointer to source image
 *
 * RESTRICTION
 *      The src and dst must be the same type and the same size.
 *      They can have 1, 2, 3, or 4 channels.
 *      They can be in MLIB_BIT data type.
 *
 * DESCRIPTION
 *      Not an image for each channel:  dst = ~src
 */

#include <mlib_image.h>
#include <mlib_v_ImageLogic_proto.h>

/* *********************************************************** */

mlib_status
mlib_v_ImageNot_Bit(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 j;
	mlib_s32 amount, dend_offset;
	mlib_u8 mask;

/* pointers for pixel and line of source */
	mlib_u8 *sp1, *sl1;

/* pointers for pixel and line of dst */
	mlib_u8 *dp, *dl;
	mlib_s32 width, height, channels;

/* for src */
	mlib_s32 stride1;

/* for dst */
	mlib_s32 strided;

/* for dst */
	mlib_s32 bitoffsetd;

	dp = (mlib_u8 *)mlib_ImageGetData(dst);
	sp1 = (mlib_u8 *)mlib_ImageGetData(src);
	height = mlib_ImageGetHeight(dst);
	width = mlib_ImageGetWidth(dst);
	stride1 = mlib_ImageGetStride(src);
	strided = mlib_ImageGetStride(dst);
	channels = mlib_ImageGetChannels(dst);
	bitoffsetd = mlib_ImageGetBitOffset(dst);

	width *= channels;

	if (width > stride1 * 8 || width > strided * 8)
		return (MLIB_FAILURE);

	if ((width == stride1 * 8) && (width == strided * 8)) {

		amount = height * width;
		dend_offset = (bitoffsetd + amount) & 7;

		if (bitoffsetd != 0) {
			mask = (0xFF >> bitoffsetd);

			if (amount < 8 - bitoffsetd) {
				mask =
				    mask & (0xFF << (8 - bitoffsetd - amount));
				*dp = (((*dp) & (~mask)) | ((~(*sp1)) & mask));
				return (MLIB_SUCCESS);
			}

			*dp = (((*dp) & (~mask)) | ((~(*sp1)) & mask));
			sp1++;
			dp++;
			amount -= (8 - bitoffsetd);
		}

		if (amount >= 8) {
			amount >>= 3;

			mlib_v_ImageNot_na(sp1, dp, amount);

			dp += amount;
			sp1 += amount;
		}

		if (dend_offset != 0) {
			mask = (0xFF << (8 - dend_offset));
			*dp = (((*dp) & (~mask)) | (~(*sp1) & mask));
		}
	} else {

		sl1 = sp1;
		dl = dp;

		dend_offset = (bitoffsetd + width) & 7;

		for (j = 0; j < height; j++) {

			amount = width;

			if (bitoffsetd != 0) {
				mask = (0xFF >> bitoffsetd);

				if (amount < 8 - bitoffsetd) {
					mask =
					    mask & (0xFF << (8 - bitoffsetd -
					    amount));
					*dp =
					    (((*dp) & (~mask)) | ((~(*sp1)) &
					    mask));
					sp1 = sl1 += stride1;
					dp = dl += strided;
					continue;
				}

				*dp = (((*dp) & (~mask)) | ((~(*sp1)) & mask));
				sp1++;
				dp++;
				amount -= (8 - bitoffsetd);
			}

			if (amount >= 8) {
				amount >>= 3;

				mlib_v_ImageNot_na(sp1, dp, amount);

				dp += amount;
				sp1 += amount;
			}

			if (dend_offset != 0) {
				mask = (0xFF << (8 - dend_offset));
				*dp = (((*dp) & (~mask)) | (~(*sp1) & mask));
			}

			sp1 = sl1 += stride1;
			dp = dl += strided;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
