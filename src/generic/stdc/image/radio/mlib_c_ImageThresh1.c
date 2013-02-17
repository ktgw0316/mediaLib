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

#pragma ident	"@(#)mlib_c_ImageThresh1.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageThresh1 - thresholding
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageThresh1(mlib_image       *dst,
 *                                    const mlib_image *src,
 *                                    const mlib_s32   *thresh,
 *                                    const mlib_s32   *ghigh,
 *                                    const mlib_s32   *glow);
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
 *      The images can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT
 *      or MLIB_INT data type.
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

#pragma weak mlib_ImageThresh1 = __mlib_ImageThresh1

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageThresh1) mlib_ImageThresh1
    __attribute__((weak, alias("__mlib_ImageThresh1")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CALL_PARAMS                                     \
	psrc, pdst, sstride, dstride, width,            \
	height, (void *)th, (void *)ghigh, (void *)gl

typedef void (*mlib_func_type1) (PARAMS);
typedef void (*mlib_func_type2) (PARAMS, mlib_s32 dbit_off);

/* *********************************************************** */

static const mlib_func_type1 mlib_func_arr1[] = {
	mlib_c_ImageThresh1_U81,
	mlib_c_ImageThresh1_U82,
	mlib_c_ImageThresh1_U83,
	mlib_c_ImageThresh1_U84,
	mlib_c_ImageThresh1_S161,
	mlib_c_ImageThresh1_S162,
	mlib_c_ImageThresh1_S163,
	mlib_c_ImageThresh1_S164,
	mlib_c_ImageThresh1_U161,
	mlib_c_ImageThresh1_U162,
	mlib_c_ImageThresh1_U163,
	mlib_c_ImageThresh1_U164,
	mlib_c_ImageThresh1_S321,
	mlib_c_ImageThresh1_S322,
	mlib_c_ImageThresh1_S323,
	mlib_c_ImageThresh1_S324
};

static const mlib_func_type2 mlib_func_arr2[] = {
	mlib_c_ImageThresh1_U81_1B,
	mlib_c_ImageThresh1_U82_1B,
	mlib_c_ImageThresh1_U83_1B,
	mlib_c_ImageThresh1_U84_1B,
	mlib_c_ImageThresh1_S161_1B,
	mlib_c_ImageThresh1_S162_1B,
	mlib_c_ImageThresh1_S163_1B,
	mlib_c_ImageThresh1_S164_1B,
	mlib_c_ImageThresh1_U161_1B,
	mlib_c_ImageThresh1_U162_1B,
	mlib_c_ImageThresh1_U163_1B,
	mlib_c_ImageThresh1_U164_1B,
	mlib_c_ImageThresh1_S321_1B,
	mlib_c_ImageThresh1_S322_1B,
	mlib_c_ImageThresh1_S323_1B,
	mlib_c_ImageThresh1_S324_1B
};

/* *********************************************************** */

mlib_status
__mlib_ImageThresh1(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow)
{
	mlib_s32 sstride, dstride, width, height, nchan;
	mlib_type stype, dtype;
	void *psrc, *pdst;
	mlib_s32 buff[8], *th = buff, *gl = buff + 4;
	mlib_s32 i;

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

	if (stype == MLIB_BYTE) {
		for (i = 0; i < nchan; i++) {
			gl[i] = glow[i];

			if (thresh[i] > MLIB_U8_MAX) {
				th[i] = MLIB_U8_MAX;
			} else if (thresh[i] < 0) {
				th[i] = 0;
				gl[i] = ghigh[i];
			} else {
				th[i] = thresh[i];
			}
		}
	} else if (stype == MLIB_SHORT) {
		for (i = 0; i < nchan; i++) {
			gl[i] = glow[i];

			if (thresh[i] > MLIB_S16_MAX)
				th[i] = MLIB_S16_MAX;
			else if (thresh[i] < MLIB_S16_MIN) {
				th[i] = MLIB_S16_MIN;
				gl[i] = ghigh[i];
			} else
				th[i] = thresh[i];
		}
	} else if (stype == MLIB_USHORT) {
		for (i = 0; i < nchan; i++) {
			gl[i] = glow[i];

			if (thresh[i] > MLIB_U16_MAX)
				th[i] = MLIB_U16_MAX;
			else if (thresh[i] < MLIB_U16_MIN) {
				th[i] = MLIB_U16_MIN;
				gl[i] = ghigh[i];
			} else
				th[i] = thresh[i];
		}
	} else if (stype == MLIB_INT) {
		th = (void *)thresh;
		gl = (void *)glow;
	} else {
		return (MLIB_FAILURE);
	}

/* x * 4.0 < x * 2.2 + 256 */
	if (dtype != MLIB_BIT && stype == MLIB_BYTE &&
	    (height * width) >= 160) {
		mlib_s32 i, c, t;
		mlib_d64 lookup[128];
		mlib_u8 *arr[4], *p;
		mlib_u8 gu;

		arr[0] = (mlib_u8 *)(&(lookup[0]));
		arr[1] = (mlib_u8 *)(&(lookup[32]));
		arr[2] = (mlib_u8 *)(&(lookup[64]));
		arr[3] = (mlib_u8 *)(&(lookup[96]));
		for (c = 0; c < nchan; c++) {
			p = arr[c];
			t = th[c];
			gu = gl[c];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= t; i++) {
				p[i] = gu;
			}

			gu = ghigh[c];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < 256; i++) {
				p[i] = gu;
			}
		}

		return (__mlib_ImageLookUp(dst, src, (const void **)arr));
	}

	if (dtype != MLIB_BIT) {

		if (dtype == MLIB_SHORT || dtype == MLIB_USHORT) {
			dstride >>= 1;
			sstride >>= 1;
		} else if (dtype == MLIB_INT) {
			dstride >>= 2;
			sstride >>= 2;
		}

		switch (stype) {
		case MLIB_BYTE:
			mlib_func_arr1[nchan - 1] (CALL_PARAMS);
			break;

		case MLIB_SHORT:
			mlib_func_arr1[nchan - 1 + 4] (CALL_PARAMS);
			break;

		case MLIB_USHORT:
			mlib_func_arr1[nchan - 1 + 8] (CALL_PARAMS);
			break;

		case MLIB_INT:
			mlib_func_arr1[nchan - 1 + 12] (CALL_PARAMS);
			break;

		default:
			return (MLIB_FAILURE);
		}
	} else {
		mlib_s32 dbit_off = mlib_ImageGetBitOffset(dst);

		if (stype == MLIB_SHORT || stype == MLIB_USHORT)
			sstride >>= 1;
		else if (stype == MLIB_INT)
			sstride >>= 2;

		switch (stype) {
		case MLIB_BYTE:
			mlib_func_arr2[nchan - 1] (CALL_PARAMS, dbit_off);
			break;

		case MLIB_SHORT:
			mlib_func_arr2[nchan - 1 + 4] (CALL_PARAMS, dbit_off);
			break;

		case MLIB_USHORT:
			mlib_func_arr2[nchan - 1 + 8] (CALL_PARAMS, dbit_off);
			break;

		case MLIB_INT:
			mlib_func_arr2[nchan - 1 + 12] (CALL_PARAMS, dbit_off);
			break;

		default:
			return (MLIB_FAILURE);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
