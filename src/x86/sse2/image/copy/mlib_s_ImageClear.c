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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_ImageClear.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageClear         - Clear an image to a specific color.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageClear(mlib_image     *img,
 *                                  const mlib_s32 *color);
 *
 * ARGUMENT
 *      img     Pointer to an image.
 *      color   Pointer to the color that the image is set to.
 *
 * RESTRICTION
 *      img can have 1, 2, 3 or 4 channels of MLIB_BIT, MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      Clear an image to a specific color.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_s_ImageClear_f.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageClear = __mlib_ImageClear

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageClear) mlib_ImageClear
    __attribute__((weak, alias("__mlib_ImageClear")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageClear(
    mlib_image *img,
    const mlib_s32 *color)
{
	MLIB_IMAGE_CHECK(img);

	switch (mlib_ImageGetType(img)) {

	case MLIB_BIT:
		return (mlib_s_ImageClear_BIT(img, color));

	case MLIB_BYTE:
		switch (mlib_ImageGetChannels(img)) {

		case 1:
			mlib_s_ImageClear_U8_1(img, color);
			break;

		case 2:
			mlib_s_ImageClear_U8_2(img, color);
			break;

		case 3:
			mlib_s_ImageClear_U8_3(img, color);
			break;

		case 4:
			mlib_s_ImageClear_U8_4(img, color);
			break;

		default:
			return (MLIB_FAILURE);
		}

		break;

	case MLIB_SHORT:
	case MLIB_USHORT:
		switch (mlib_ImageGetChannels(img)) {

		case 1:
			mlib_s_ImageClear_S16_1(img, color);
			break;

		case 2:
			mlib_s_ImageClear_S16_2(img, color);
			break;

		case 3:
			mlib_s_ImageClear_S16_3(img, color);
			break;

		case 4:
			mlib_s_ImageClear_S16_4(img, color);
			break;

		default:
			return (MLIB_FAILURE);
		}

		break;

	case MLIB_INT:
		switch (mlib_ImageGetChannels(img)) {

		case 1:
			mlib_s_ImageClear_S32_1(img, color);
			break;

		case 2:
			mlib_s_ImageClear_S32_2(img, color);
			break;

		case 3:
			mlib_s_ImageClear_S32_3(img, color);
			break;

		case 4:
			mlib_s_ImageClear_S32_4(img, color);
			break;

		default:
			return (MLIB_FAILURE);
		}

		break;

	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
