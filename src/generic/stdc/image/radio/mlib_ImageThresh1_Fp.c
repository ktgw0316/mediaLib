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

#pragma ident	"@(#)mlib_ImageThresh1_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageThresh1_Fp - thresholding
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageThresh1_Fp(mlib_image       *dst,
 *                                       const mlib_image *src,
 *                                       const mlib_d64   *thresh,
 *                                       const mlib_d64   *ghigh,
 *                                       const mlib_d64   *glow);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      thresh  array of thresholds
 *      ghigh   array of values above thresholds
 *      glow    array of values below thresholds
 *
 * RESTRICTION
 *      The images must have the same size, and the same number
 *      of channels.
 *      The images can have 1, 2, 3, or 4 channels.
 *      The images can be in MLIB_FLOAT or MLIB_DOUBLE data type.
 *      The type of the output image can be MLIB_BIT, or the same as the
 *      type of the input image.
 *
 * DESCRIPTION
 *      If the pixel band value is above the threshold for that channel,
 *      set the destination to the ghigh value for that channel.
 *      Otherwise, set the destination to the glow value for that channel.
 *
 *                      +- glow[c]   src[x][y][c] <= thresh[c]
 *      dst[x][y][c]  = |
 *                      +- ghigh[c]  src[x][y][c] >  thresh[c]
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_c_ImageThresh1.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageThresh1_Fp = __mlib_ImageThresh1_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageThresh1_Fp) mlib_ImageThresh1_Fp
    __attribute__((weak, alias("__mlib_ImageThresh1_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CALL_PARAMS                                             \
	psrc, pdst, sstride, dstride, width,                    \
	height, (void *)thresh, (void *)ghigh, (void *)glow

typedef void (*mlib_func_type1) (PARAMS);
typedef void (*mlib_func_type2) (PARAMS, mlib_s32 dbit_off);

/* *********************************************************** */

static const mlib_func_type1 mlib_func_arr1[] = {
	mlib_c_ImageThresh1_F321,
	mlib_c_ImageThresh1_F322,
	mlib_c_ImageThresh1_F323,
	mlib_c_ImageThresh1_F324,
	mlib_c_ImageThresh1_D641,
	mlib_c_ImageThresh1_D642,
	mlib_c_ImageThresh1_D643,
	mlib_c_ImageThresh1_D644
};

static const mlib_func_type2 mlib_func_arr2[] = {
	mlib_c_ImageThresh1_F321_1B,
	mlib_c_ImageThresh1_F322_1B,
	mlib_c_ImageThresh1_F323_1B,
	mlib_c_ImageThresh1_F324_1B,
	mlib_c_ImageThresh1_D641_1B,
	mlib_c_ImageThresh1_D642_1B,
	mlib_c_ImageThresh1_D643_1B,
	mlib_c_ImageThresh1_D644_1B
};

/* *********************************************************** */

mlib_status
__mlib_ImageThresh1_Fp(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *thresh,
    const mlib_d64 *ghigh,
    const mlib_d64 *glow)
{
	mlib_s32 sstride, dstride, width, height, nchan;
	mlib_type stype, dtype;
	void *psrc, *pdst;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_SIZE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);
	MLIB_IMAGE_TYPE_DSTBIT_OR_EQ(src, dst);

	stype = mlib_ImageGetType(src);
	dtype = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(src);
	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	sstride = mlib_ImageGetStride(src);
	dstride = mlib_ImageGetStride(dst);
	psrc = mlib_ImageGetData(src);
	pdst = mlib_ImageGetData(dst);

	if (stype == MLIB_FLOAT)
		sstride >>= 2;
	else if (stype == MLIB_DOUBLE)
		sstride >>= 3;
	else
		return (MLIB_FAILURE);

	if (dtype == MLIB_FLOAT)
		dstride >>= 2;
	if (dtype == MLIB_DOUBLE)
		dstride >>= 3;

	if (dtype != MLIB_BIT) {
		mlib_func_arr1[(stype - MLIB_FLOAT) * 4 + (nchan -
		    1)] (CALL_PARAMS);
	} else {
		mlib_s32 dbit_off = mlib_ImageGetBitOffset(dst);

		mlib_func_arr2[(stype - MLIB_FLOAT) * 4 + (nchan -
		    1)] (CALL_PARAMS, dbit_off);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
