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

#pragma ident "@(#)mlib_s_ImageCopyMask.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageCopyMask  - Copy one image to another via a mask
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageCopyMask(mlib_image       *dst,
 *                                     const mlib_image *src,
 *                                     const mlib_image *mask,
 *                                     const mlib_s32   *thresh);
 *
 * ARGUMENT
 *      dst     pointer to output or destination image
 *      src     pointer to input or source image
 *      mask    pointer to mask image
 *      thresh  array of thresholds
 *
 * RESTRICTION
 *      src, dst and mask images must have the same size, type and
 *      number of channels.
 *      They can have 1, 2, 3 or 4 channels of MLIB_BYTE, MLIB_SHORT,
 *      MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      Copy one image to another via a mask.
 *      dst(i, j) = src(i, j) when mask(i, j) <= *thresh, or
 *                  dst(i, j) when mask(i, j) >  *thresh
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include "mlib_s_ImageCopyMask.h"

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageCopyMask = __mlib_ImageCopyMask

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageCopyMask) mlib_ImageCopyMask
    __attribute__((weak, alias("__mlib_ImageCopyMask")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageCopyMask(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_image *mask,
    const mlib_s32 *thresh)
{
	mlib_type dtype;
	mlib_s32 slb, mlb, dlb, xsize, ysize, nchan;
	void *sa, *ma, *da;
	mlib_s32 i, ch, chmask;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(mask);

	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_TYPE_EQUAL(mask, dst);

	MLIB_IMAGE_CHAN_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(mask, dst);

	MLIB_IMAGE_SIZE_EQUAL(src, dst);
	MLIB_IMAGE_SIZE_EQUAL(mask, dst);

	dtype = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	slb = mlib_ImageGetStride(src);
	mlb = mlib_ImageGetStride(mask);
	dlb = mlib_ImageGetStride(dst);
	sa = mlib_ImageGetData(src);
	ma = mlib_ImageGetData(mask);
	da = mlib_ImageGetData(dst);

	if (dtype == MLIB_BYTE) {
		/*
		 *  dst(i, j) = src(i, j) if mask(i, j) <  *thresh+1
		 *		dst(i, j) if mask(i, j) >= *thresh+1
		 */
		mlib_s_ImageCopyMask_u8(sa, slb, ma, mlb, da, dlb,
			xsize, ysize, nchan, thresh);
		/*
		 *  dst(i, j) = src(i, j) if *thresh >= MLIB_U8_MAX
		 */
		chmask = 0;
		for (i = 0; i < nchan; i++) {
			ch = (thresh[i] >= MLIB_U8_MAX) ? 1 : 0;
			chmask = (chmask << 1) | ch;
		}
		if (chmask != 0) {
			__mlib_ImageChannelCopy(dst, src, chmask);
		}
		return (MLIB_SUCCESS);
	} else
	if (dtype == MLIB_SHORT) {
		/*
		 *  dst(i, j) = src(i, j) if mask(i, j) <  *thresh+1
		 *		dst(i, j) if mask(i, j) >= *thresh+1
		 */
		mlib_s_ImageCopyMask_s16(sa, slb, ma, mlb, da, dlb,
			xsize, ysize, nchan, thresh);
		/*
		 *  dst(i, j) = src(i, j) if *thresh >= MLIB_S16_MAX
		 */
		chmask = 0;
		for (i = 0; i < nchan; i++) {
			ch = (thresh[i] >= MLIB_S16_MAX) ? 1 : 0;
			chmask = (chmask << 1) | ch;
		}
		if (chmask != 0) {
			__mlib_ImageChannelCopy(dst, src, chmask);
		}
		return (MLIB_SUCCESS);
	} else
	if (dtype == MLIB_USHORT) {
		/*
		 *  dst(i, j) = src(i, j) if mask(i, j) <  *thresh+1
		 *		dst(i, j) if mask(i, j) >= *thresh+1
		 */
		mlib_s_ImageCopyMask_u16(sa, slb, ma, mlb, da, dlb, xsize,
		    ysize, nchan, thresh);
		/*
		 *  dst(i, j) = src(i, j) if *thresh >= MLIB_U16_MAX
		 */
		chmask = 0;
		for (i = 0; i < nchan; i++) {
			ch = (thresh[i] >= MLIB_U16_MAX) ? 1 : 0;
			chmask = (chmask << 1) | ch;
		}
		if (chmask != 0) {
			__mlib_ImageChannelCopy(dst, src, chmask);
		}
		return (MLIB_SUCCESS);
	} else
	if (dtype == MLIB_INT) {
		/*
		 *  dst(i, j) = src(i, j) if mask(i, j) <= *thresh
		 *		dst(i, j) if mask(i, j) >  *thresh
		 */
		mlib_s_ImageCopyMask_s32(sa, slb, ma, mlb, da, dlb,
			xsize, ysize, nchan, thresh);
		return (MLIB_SUCCESS);
	} else {
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */
