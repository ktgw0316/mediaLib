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

#pragma ident	"@(#)mlib_ImageAbs.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageAbs - absolute value of each pixel
 *
 * SYNOPSIS
 *        mlib_status mlib_ImageAbs (mlib_image       *dst,
 *                                   const mlib_image *src)
 *
 * ARGUMENT
 *      dst     pointer to destination image
 *      src     pointer to source image
 *
 * RESTRICTION
 *      src and dst must be the same type, the same size and the same
 *      number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      dst(i, j) = |src(i, j)|
 *
 *      |MLIB_S16_MIN| = MLIB_S16_MIN for MLIB_SHORT image type,
 *      |MLIB_S32_MIN| = MLIB_S32_MIN for MLIB_INT   image type.
 *
 *      Call mlib_ImageCopy() for 8-bit, mlib_ImageAbs_[S|U]16() for 16-bit,
 *      mlib_ImageAbs_S32() for 32-bit.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageAbs.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageAbs = __mlib_ImageAbs

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageAbs) mlib_ImageAbs
    __attribute__((weak, alias("__mlib_ImageAbs")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageAbs(
    mlib_image *dst,
    const mlib_image *src)
{
/* dst image type */
	mlib_type dtype;

/* dst height */
	mlib_s32 dh;

/* dst width */
	mlib_s32 dw;

/* dst num of channels */
	mlib_s32 dchan;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	dtype = mlib_ImageGetType(dst);

	if (dtype == MLIB_BYTE || dtype == MLIB_USHORT) {
		return (__mlib_ImageCopy(dst, src));
	} else {
/* ptrs for pixel of src, dst */
		void *sa, *da;

/* src, dst y stride */
		mlib_s32 slb, dlb;

/* wid including all chan */
		mlib_s32 xsize;

		MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, dchan, dw, dh, dlb, da);
		slb = mlib_ImageGetStride(src);
		sa = mlib_ImageGetData(src);

		xsize = dw * dchan;

		if (!mlib_ImageIsNotOneDvector(src) &&	/* if both 1-D */
		    !mlib_ImageIsNotOneDvector(dst)) {
/* pretend it is one mlib_s32 row (5% speedup) */
			xsize *= dh;
			slb *= dh;
			dlb *= dh;
			dh = 1;
		}

		if (dtype == MLIB_SHORT)
			return mlib_ImageAbs_S16((mlib_s16 *)da, (mlib_s16 *)sa,
			    dlb, slb, xsize, dh);
		else if (dtype == MLIB_INT)
			return mlib_ImageAbs_S32((mlib_s32 *)da, (mlib_s32 *)sa,
			    dlb, slb, xsize, dh);
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */
