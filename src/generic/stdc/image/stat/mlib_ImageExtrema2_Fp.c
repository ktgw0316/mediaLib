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

#pragma ident	"@(#)mlib_ImageExtrema2_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageExtrema2_Fp   - finds the image-wise minimum and maximum
 *                                pixel values for each channel
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageExtrema2_Fp(mlib_d64         *min,
 *                                        mlib_d64         *max,
 *                                        const mlib_image *img,
 *                                        mlib_s32         xStart,
 *                                        mlib_s32         yStart,
 *                                        mlib_s32         xPeriod,
 *                                        mlib_s32         yPeriod);
 *
 * ARGUMENT
 *      min        The pointer to the minimum values
 *      max        The pointer to the maximum values
 *      img        The pointer to the input image.
 *      xStart     The initial X sample coordinate.
 *      yStart     The initial Y sample coordinate.
 *      xPeriod    The X sampling rate, xPeriod >= 1.
 *      yPeriod    The Y sampling rate, yPeriod >= 1.
 *
 * RESTRICTION
 *      The image can have 1, 2, 3 or 4 channels.
 *      It can be of MLIB_FLOAT and MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *
 *      The mlib_ImageExtrema2_Fp function scans an image, finds the minimum
 *      and maximum pixel values for each channel.
 *
 *      The set of pixels scanned may furthermore be reduced by specifying
 *      "xPeriod" and "yPeriod" parameters that specify the sampling rate
 *      along each axis.
 *
 *      The set of pixels to be scanned may be obtained by intersecting
 *      the grid (xStart + i*xPeriod, yStart + j*yPeriod); i, j >= 0 with
 *      the bounding rectangle of the image.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageCreate.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageExtrema2_Fp = __mlib_ImageExtrema2_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageExtrema2_Fp) mlib_ImageExtrema2_Fp
    __attribute__((weak, alias("__mlib_ImageExtrema2_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MIN(min, x)	if (x < min) min = x;
#define	MAX(max, x)	if (x > max) max = x;

#define	MINMAX(min, max, x)                                     \
	max = (max > x) ? max : x;                              \
	min = (min < x) ? min : x

/* *********************************************************** */

#define	FUNC_IMAGE_EXTREMA2_2(STYPE)                                   \
	static void                                                    \
	mlib_ImageExtrema2_2_##STYPE(mlib_d64 *dmin, mlib_d64 *dmax,   \
	    mlib_##STYPE * sl, mlib_s32 xsize, mlib_s32 ysize,         \
	    mlib_s32 slb, mlib_s32 off1, mlib_s32 off2)                \
	{                                                              \
	    mlib_##STYPE min0, min1;                                   \
	    mlib_##STYPE max0, max1;                                   \
	    mlib_s32 j;                                                \
	                                                               \
	    min0 = min1 = (mlib_##STYPE) dmin[0];                      \
	    max0 = max1 = (mlib_##STYPE) dmax[0];                      \
	    for (j = 0; j < ysize; j++) {                              \
		mlib_##STYPE *sp = sl;                                 \
		mlib_##STYPE *send = sl + xsize;                       \
	                                                               \
		for (; sp < (send - off1); sp += off2) {               \
		    mlib_##STYPE s0, s1;                               \
	                                                               \
		    s0 = sp[0];                                        \
		    s1 = sp[off1];                                     \
		    MINMAX(min0, max0, s0);                            \
		    MINMAX(min1, max1, s1);                            \
		}                                                      \
		if (sp < send) {                                       \
		    MINMAX(min0, max0, sp[0]);                         \
		}                                                      \
		sl += slb;                                             \
	    }                                                          \
	    dmin[0] = min0;                                            \
	    dmax[0] = max0;                                            \
	    dmin[1] = min1;                                            \
	    dmax[1] = max1;                                            \
	}

/* *********************************************************** */

#define	FUNC_IMAGE_EXTREMA2_3(STYPE)                                   \
	static void                                                    \
	mlib_ImageExtrema2_3_##STYPE(mlib_d64 *dmin, mlib_d64 *dmax,   \
	    mlib_##STYPE * sl, mlib_s32 xsize, mlib_s32 ysize,         \
	    mlib_s32 slb, mlib_s32 off3)                               \
	{                                                              \
	    mlib_##STYPE min0, min1, min2;                             \
	    mlib_##STYPE max0, max1, max2;                             \
	    mlib_s32 j;                                                \
	                                                               \
	    min0 = min1 = min2 = (mlib_##STYPE) dmin[0];               \
	    max0 = max1 = max2 = (mlib_##STYPE) dmax[0];               \
	    for (j = 0; j < ysize; j++) {                              \
		mlib_##STYPE *sp = sl;                                 \
		mlib_##STYPE *send = sl + xsize;                       \
	                                                               \
		for (; sp < (send - 2); sp += off3) {                  \
		    mlib_##STYPE s0, s1, s2;                           \
	                                                               \
		    s0 = sp[0];                                        \
		    s1 = sp[1];                                        \
		    s2 = sp[2];                                        \
		    MINMAX(min0, max0, s0);                            \
		    MINMAX(min1, max1, s1);                            \
		    MINMAX(min2, max2, s2);                            \
		}                                                      \
		sl += slb;                                             \
	    }                                                          \
	    dmin[0] = min0;                                            \
	    dmax[0] = max0;                                            \
	    dmin[1] = min1;                                            \
	    dmax[1] = max1;                                            \
	    dmin[2] = min2;                                            \
	    dmax[2] = max2;                                            \
	}

/* *********************************************************** */

#define	FUNC_IMAGE_EXTREMA2_4(STYPE)                                     \
	static void                                                      \
	mlib_ImageExtrema2_4_##STYPE(mlib_d64 *dmin, mlib_d64 *dmax,     \
	    mlib_##STYPE * sl, mlib_s32 xsize, mlib_s32 ysize,           \
	    mlib_s32 slb, mlib_s32 off1, mlib_s32 off2, mlib_s32 off3,   \
	    mlib_s32 off4)                                               \
	{                                                                \
	    mlib_##STYPE min0, min1, min2, min3;                         \
	    mlib_##STYPE max0, max1, max2, max3;                         \
	    mlib_s32 j;                                                  \
	                                                                 \
	    min0 = min1 = min2 = min3 = (mlib_##STYPE) dmin[0];          \
	    max0 = max1 = max2 = max3 = (mlib_##STYPE) dmax[0];          \
	    for (j = 0; j < ysize; j++) {                                \
		mlib_##STYPE *sp = sl;                                   \
		mlib_##STYPE *send = sl + xsize;                         \
	                                                                 \
		for (; sp < (send - off3); sp += off4) {                 \
		    mlib_##STYPE s0, s1, s2, s3;                         \
	                                                                 \
		    s0 = sp[0];                                          \
		    s1 = sp[off1];                                       \
		    s2 = sp[off2];                                       \
		    s3 = sp[off3];                                       \
		    MINMAX(min0, max0, s0);                              \
		    MINMAX(min1, max1, s1);                              \
		    MINMAX(min2, max2, s2);                              \
		    MINMAX(min3, max3, s3);                              \
		}                                                        \
		if ((sp) < send) {                                       \
		    MINMAX(min0, max0, sp[0]);                           \
		}                                                        \
		if ((sp + off1) < send) {                                \
		    MINMAX(min1, max1, sp[off1]);                        \
		}                                                        \
		if ((sp + off2) < send) {                                \
		    MINMAX(min2, max2, sp[off2]);                        \
		}                                                        \
		sl += slb;                                               \
	    }                                                            \
	    dmin[0] = min0;                                              \
	    dmax[0] = max0;                                              \
	    dmin[1] = min1;                                              \
	    dmax[1] = max1;                                              \
	    dmin[2] = min2;                                              \
	    dmax[2] = max2;                                              \
	    dmin[3] = min3;                                              \
	    dmax[3] = max3;                                              \
	}

/* *********************************************************** */

FUNC_IMAGE_EXTREMA2_3(d64)
    FUNC_IMAGE_EXTREMA2_4(d64)
    FUNC_IMAGE_EXTREMA2_2(f32)
    FUNC_IMAGE_EXTREMA2_3(f32)
    FUNC_IMAGE_EXTREMA2_4(f32)

/* *********************************************************** */
    mlib_status __mlib_ImageExtrema2_Fp(
    mlib_d64 *min,
    mlib_d64 *max,
    const mlib_image *img,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 xPeriod,
    mlib_s32 yPeriod)
{
	mlib_type type;
	void *sl;
	mlib_s32 nchan, xsize, ysize, slb, tsize;
	mlib_d64 dmin[4], dmax[4];

	MLIB_IMAGE_CHECK(img);

	if (min == NULL || max == NULL)
		return (MLIB_NULLPOINTER);
	if (xPeriod < 1 || yPeriod < 1)
		return (MLIB_OUTOFRANGE);

	while (xStart < 0)
		xStart += xPeriod;
	while (yStart < 0)
		yStart += yPeriod;

	MLIB_IMAGE_GET_ALL_PARAMS(img, type, nchan, xsize, ysize, slb, sl);

	if (type == MLIB_DOUBLE)
		tsize = 8;
	else if (type == MLIB_FLOAT)
		tsize = 4;
	else
		return (MLIB_FAILURE);

	sl = (mlib_u8 *)sl + yStart * slb + xStart * nchan * tsize;
	xsize = (xsize - xStart);
	ysize = ((ysize - yStart) + (yPeriod - 1)) / yPeriod;
	slb = slb * yPeriod;

	if (xsize <= 0 || ysize <= 0)
		return (MLIB_SUCCESS);

	if (type == MLIB_FLOAT && xPeriod == 1) {
		mlib_image tmp_img;

		if (mlib_ImageSet(&tmp_img, type, nchan, xsize, ysize, slb,
		    sl) != NULL) {
			if (__mlib_ImageMinimum_Fp(min,
			    &tmp_img) == MLIB_SUCCESS &&
			    __mlib_ImageMaximum_Fp(max,
			    &tmp_img) == MLIB_SUCCESS)
				return (MLIB_SUCCESS);
		}
	}

	xsize *= nchan;
	slb /= tsize;

	if (type == MLIB_DOUBLE) {

		dmin[0] = min[0] = MLIB_D64_MAX;
		dmax[0] = max[0] = MLIB_D64_MIN;

		switch (nchan) {
		case 1:
			mlib_ImageExtrema2_4_d64(dmin, dmax, sl, xsize, ysize,
			    slb, xPeriod, 2 * xPeriod, 3 * xPeriod,
			    4 * xPeriod);
			MIN(dmin[0], dmin[1]);
			MAX(dmax[0], dmax[1]);
			MIN(dmin[0], dmin[2]);
			MAX(dmax[0], dmax[2]);
			MIN(dmin[0], dmin[3]);
			MAX(dmax[0], dmax[3]);
			min[0] = dmin[0];
			max[0] = dmax[0];
			break;

		case 2:
			mlib_ImageExtrema2_4_d64(dmin, dmax, sl, xsize, ysize,
			    slb, 1, 2 * xPeriod, 2 * xPeriod + 1, 4 * xPeriod);
			MIN(dmin[0], dmin[2]);
			MAX(dmax[0], dmax[2]);
			MIN(dmin[1], dmin[3]);
			MAX(dmax[1], dmax[3]);
			min[0] = dmin[0];
			max[0] = dmax[0];
			min[1] = dmin[1];
			max[1] = dmax[1];
			break;

		case 3:
			mlib_ImageExtrema2_3_d64(min, max, sl, xsize, ysize,
			    slb, 3 * xPeriod);
			break;

		case 4:
			mlib_ImageExtrema2_4_d64(min, max, sl, xsize, ysize,
			    slb, 1, 2, 3, 4 * xPeriod);
			break;

		default:
			return (MLIB_FAILURE);
		}

	} else {

		dmin[0] = min[0] = MLIB_F32_MAX;
		dmax[0] = max[0] = MLIB_F32_MIN;

		switch (nchan) {
		case 1:
			mlib_ImageExtrema2_2_f32(dmin, dmax, sl, xsize, ysize,
			    slb, xPeriod, 2 * xPeriod);
			MIN(dmin[0], dmin[1]);
			MAX(dmax[0], dmax[1]);
			min[0] = dmin[0];
			max[0] = dmax[0];
			break;

		case 2:
			mlib_ImageExtrema2_2_f32(min, max, sl, xsize, ysize,
			    slb, 1, 2 * xPeriod);
			break;

		case 3:
			mlib_ImageExtrema2_3_f32(min, max, sl, xsize, ysize,
			    slb, 3 * xPeriod);
			break;

		case 4:
			mlib_ImageExtrema2_4_f32(min, max, sl, xsize, ysize,
			    slb, 1, 2, 3, 4 * xPeriod);
			break;

		default:
			return (MLIB_FAILURE);
		}

	};

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
