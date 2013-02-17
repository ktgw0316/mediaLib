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

#pragma ident	"@(#)mlib_i_ImageCopyMask.c	9.2	07/11/05 SMI"

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
 *                 dst(i, j) when mask(i, j) >  *thresh
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

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

#define	MLIB_C_COPYIMAGEMASK1(D_TYPE)                           \
	for (j = 0; j < ysize; j++) {                           \
	    for (i = 0; i < xsize; i++) {                       \
		if (mask[i] <= thresh0)                         \
		    dst[i] = src[i];                            \
	    }                                                   \
	    src = (D_TYPE *) ((mlib_u8 *)src + slb);            \
	    mask = (D_TYPE *) ((mlib_u8 *)mask + mlb);          \
	    dst = (D_TYPE *) ((mlib_u8 *)dst + dlb);            \
	}

/* *********************************************************** */

#define	MLIB_C_COPYIMAGEMASK2(D_TYPE)                           \
	thresh1 = thresh[1];                                    \
	for (j = 0; j < ysize; j++) {                           \
	    for (i = 0; i < xsize; i++) {                       \
		if (mask[2 * i] <= thresh0)                     \
		    dst[2 * i] = src[2 * i];                    \
		if (mask[2 * i + 1] <= thresh1)                 \
		    dst[2 * i + 1] = src[2 * i + 1];            \
	    }                                                   \
	    src = (D_TYPE *) ((mlib_u8 *)src + slb);            \
	    mask = (D_TYPE *) ((mlib_u8 *)mask + mlb);          \
	    dst = (D_TYPE *) ((mlib_u8 *)dst + dlb);            \
	}

/* *********************************************************** */

#define	MLIB_C_COPYIMAGEMASK3(D_TYPE)                           \
	thresh1 = thresh[1];                                    \
	thresh2 = thresh[2];                                    \
	for (j = 0; j < ysize; j++) {                           \
	    for (i = 0; i < xsize; i++) {                       \
		if (mask[3 * i] <= thresh0)                     \
		    dst[3 * i] = src[3 * i];                    \
		if (mask[3 * i + 1] <= thresh1)                 \
		    dst[3 * i + 1] = src[3 * i + 1];            \
		if (mask[3 * i + 2] <= thresh2)                 \
		    dst[3 * i + 2] = src[3 * i + 2];            \
	    }                                                   \
	    src = (D_TYPE *) ((mlib_u8 *)src + slb);            \
	    mask = (D_TYPE *) ((mlib_u8 *)mask + mlb);          \
	    dst = (D_TYPE *) ((mlib_u8 *)dst + dlb);            \
	}

/* *********************************************************** */

#define	MLIB_C_COPYIMAGEMASK4(D_TYPE)                           \
	thresh1 = thresh[1];                                    \
	thresh2 = thresh[2];                                    \
	thresh3 = thresh[3];                                    \
	for (j = 0; j < ysize; j++) {                           \
	    for (i = 0; i < xsize; i++) {                       \
		if (mask[4 * i] <= thresh0)                     \
		    dst[4 * i] = src[4 * i];                    \
		if (mask[4 * i + 1] <= thresh1)                 \
		    dst[4 * i + 1] = src[4 * i + 1];            \
		if (mask[4 * i + 2] <= thresh2)                 \
		    dst[4 * i + 2] = src[4 * i + 2];            \
		if (mask[4 * i + 3] <= thresh3)                 \
		    dst[4 * i + 3] = src[4 * i + 3];            \
	    }                                                   \
	    src = (D_TYPE *) ((mlib_u8 *)src + slb);            \
	    mask = (D_TYPE *) ((mlib_u8 *)mask + mlb);          \
	    dst = (D_TYPE *) ((mlib_u8 *)dst + dlb);            \
	}

/* *********************************************************** */

static void
mlib_c_ImageCopyMask_u8(
    mlib_u8 *src,
    mlib_s32 slb,
    const mlib_u8 *mask,
    mlib_s32 mlb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    const mlib_s32 *thresh)
{
	mlib_s32 i, j;
	mlib_s32 thresh0 = thresh[0], thresh1, thresh2, thresh3;

	switch (nchan) {
	case 1:
		MLIB_C_COPYIMAGEMASK1(mlib_u8);

		break;
	case 2:
		MLIB_C_COPYIMAGEMASK2(mlib_u8);

		break;
	case 3:
		MLIB_C_COPYIMAGEMASK3(mlib_u8);

		break;
	case 4:
		MLIB_C_COPYIMAGEMASK4(mlib_u8);

		break;
	}
}

/* *********************************************************** */

static void
mlib_c_ImageCopyMask_s16(
    const mlib_s16 *src,
    mlib_s32 slb,
    const mlib_s16 *mask,
    mlib_s32 mlb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    const mlib_s32 *thresh)
{
	mlib_s32 i, j;
	mlib_s32 thresh0 = thresh[0], thresh1, thresh2, thresh3;

	switch (nchan) {
	case 1:
		MLIB_C_COPYIMAGEMASK1(mlib_s16);

		break;
	case 2:
		MLIB_C_COPYIMAGEMASK2(mlib_s16);

		break;
	case 3:
		MLIB_C_COPYIMAGEMASK3(mlib_s16);

		break;
	case 4:
		MLIB_C_COPYIMAGEMASK4(mlib_s16);

		break;
	}
}

/* *********************************************************** */

static void
mlib_c_ImageCopyMask_u16(
    const mlib_u16 *src,
    mlib_s32 slb,
    const mlib_u16 *mask,
    mlib_s32 mlb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    const mlib_s32 *thresh)
{
	mlib_s32 i, j;
	mlib_s32 thresh0 = thresh[0], thresh1, thresh2, thresh3;

	switch (nchan) {
	case 1:
		MLIB_C_COPYIMAGEMASK1(mlib_u16);

		break;
	case 2:
		MLIB_C_COPYIMAGEMASK2(mlib_u16);

		break;
	case 3:
		MLIB_C_COPYIMAGEMASK3(mlib_u16);

		break;
	case 4:
		MLIB_C_COPYIMAGEMASK4(mlib_u16);

		break;
	}
}

/* *********************************************************** */

static void
mlib_c_ImageCopyMask_s32(
    const mlib_s32 *src,
    mlib_s32 slb,
    const mlib_s32 *mask,
    mlib_s32 mlb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    const mlib_s32 *thresh)
{
	mlib_s32 i, j;
	mlib_s32 thresh0 = thresh[0], thresh1, thresh2, thresh3;

	switch (nchan) {
	case 1:
		MLIB_C_COPYIMAGEMASK1(mlib_s32);

		break;
	case 2:
		MLIB_C_COPYIMAGEMASK2(mlib_s32);

		break;
	case 3:
		MLIB_C_COPYIMAGEMASK3(mlib_s32);

		break;
	case 4:
		MLIB_C_COPYIMAGEMASK4(mlib_s32);

		break;
	}
}

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
		mlib_c_ImageCopyMask_u8(sa, slb, ma, mlb, da, dlb, xsize, ysize,
		    nchan, thresh);
		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_SHORT) {
		mlib_c_ImageCopyMask_s16(sa, slb, ma, mlb, da, dlb, xsize,
		    ysize, nchan, thresh);
		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_USHORT) {
		mlib_c_ImageCopyMask_u16(sa, slb, ma, mlb, da, dlb, xsize,
		    ysize, nchan, thresh);
		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_INT) {
		mlib_c_ImageCopyMask_s32(sa, slb, ma, mlb, da, dlb, xsize,
		    ysize, nchan, thresh);
		return (MLIB_SUCCESS);
	} else {
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */
