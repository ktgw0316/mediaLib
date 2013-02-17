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

#pragma ident	"@(#)mlib_c_ImageFlipY.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageFlipY - flip image on Y axis
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageFlipY(mlib_image       *dst,
 *                                  const mlib_image *src)
 *
 * ARGUMENTS
 *      dst     pointer to an dst image
 *      src     pointer to an src image
 *
 * RESTRICTION
 *      src and dst must be the same type, the same size and the same
 *      number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be in MLIB_BIT, MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageFlipY.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageFlipY = __mlib_ImageFlipY

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageFlipY) mlib_ImageFlipY
    __attribute__((weak, alias("__mlib_ImageFlipY")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageFlipY(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_type type;
	mlib_s32 nchan, tshift;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);

	type = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(dst);

	if (type == MLIB_BIT) {
		if (nchan != 1)
			return (MLIB_FAILURE);
		mlib_ImageFlipY_BIT_1(dst, src);
		return (MLIB_SUCCESS);
	}

	switch (type) {
	case MLIB_BYTE:
		tshift = 0;
		break;
	case MLIB_SHORT:
		tshift = 1;
		break;
	case MLIB_USHORT:
		tshift = 1;
		break;
	case MLIB_INT:
		tshift = 2;
		break;
	default:
		return (MLIB_FAILURE);
	}

#if 0

	if ((nchan & 1) == 0) {
		mlib_u8 *p_src, *p_dst;
		mlib_s32 src_height, dst_height, src_width, dst_width;
		mlib_s32 src_stride, dst_stride, align;

		MLIB_IMAGE_GET_ALL_PARAMS(src, type, nchan, src_width,
		    src_height, src_stride, p_src);
		MLIB_IMAGE_GET_ALL_PARAMS(dst, type, nchan, dst_width,
		    dst_height, dst_stride, p_dst);

		align =
		    (mlib_s32)p_src | (mlib_s32)p_dst | src_stride | dst_stride;

		while (((nchan | (align >> tshift)) & 1) == 0 && tshift < 3) {
/* two channels as one channel of next type */
			nchan >>= 1;
			tshift++;
		}
	}
#endif /* 0 */

	switch (4 * tshift + (nchan - 1)) {
	case 0:
		mlib_ImageFlipY_U8_1(dst, src);
		break;
	case 1:
		mlib_ImageFlipY_U8_2(dst, src);
		break;
	case 2:
		mlib_ImageFlipY_U8_3(dst, src);
		break;
	case 3:
		mlib_ImageFlipY_U8_4(dst, src);
		break;
	case 4:
		mlib_ImageFlipY_S16_1(dst, src);
		break;
	case 5:
		mlib_ImageFlipY_S16_2(dst, src);
		break;
	case 6:
		mlib_ImageFlipY_S16_3(dst, src);
		break;
	case 7:
		mlib_ImageFlipY_S16_4(dst, src);
		break;
	case 8:
		mlib_ImageFlipY_S32_1(dst, src);
		break;
	case 9:
		mlib_ImageFlipY_S32_2(dst, src);
		break;
	case 10:
		mlib_ImageFlipY_S32_3(dst, src);
		break;
	case 11:
		mlib_ImageFlipY_S32_4(dst, src);
		break;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
