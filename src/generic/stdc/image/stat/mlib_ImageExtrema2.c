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

#pragma ident	"@(#)mlib_ImageExtrema2.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageExtrema2      - finds the image-wise minimum and maximum
 *                                pixel values for each channel
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageExtrema2(mlib_s32         *min,
 *                                     mlib_s32         *max,
 *                                     const mlib_image *img,
 *                                     mlib_s32         xStart,
 *                                     mlib_s32         yStart,
 *                                     mlib_s32         xPeriod,
 *                                     mlib_s32         yPeriod);
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
 * DESCRIPTION
 *
 *      The mlib_ImageExtrema2 function scans an image, finds the minimum
 *      and maximum pixel values for each channel.
 *
 *      The set of pixels scanned may furthermore be reduced by specifying
 *      "xPeriod" and "yPeriod" parameters that specify the sampling rate
 *      along each axis.
 *
 *      The set of pixels to be scanned may be obtained by intersecting
 *      the grid (xStart + i*xPeriod, yStart + j*yPeriod); i, j >= 0 with
 *      the bounding rectangle of the image.
 *
 * RESTRICTION
 *      The image can have 1, 2, 3 or 4 channels.
 *      It can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageCreate.h>
#include <mlib_ImageExtrema.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageExtrema2 = __mlib_ImageExtrema2

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageExtrema2) mlib_ImageExtrema2
    __attribute__((weak, alias("__mlib_ImageExtrema2")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MIN(min, x)	min = (min < x) ? min : x
#define	MAX(max, x)	max = (max > x) ? max : x

/* *********************************************************** */

#define	MIN_u8(min, x)	min += ((x - min) & ((x - min) >> 31))
#define	MIN_s16(min, x)	min += ((x - min) & ((x - min) >> 31))

#define	MIN_u16(min, x)	min += ((x - min) & ((x - min) >> 31))

#define	MIN_s32(min, x)	MIN(min, x)

/* *********************************************************** */

#define	MAX_u8(max, x)	max -= (max - x) & ((max - x) >> 31)
#define	MAX_s16(max, x)	max -= (max - x) & ((max - x) >> 31)

#define	MAX_u16(max, x)	max -= (max - x) & ((max - x) >> 31)

#define	MAX_s32(max, x)	MAX(max, x)

/* *********************************************************** */

#define	MINMAX(min, max, x, y)                                  \
	{                                                       \
	    mlib_s32 del, mask;                                 \
	                                                        \
	    del = x - y;                                        \
	    mask = del >> 31;                                   \
	    max = x - (del & mask);                             \
	    min = x - (del & ~mask);                            \
	}

/* *********************************************************** */

#define	MINMAX_u8(min, max, x, y)	MINMAX(min, max, x, y)
#define	MINMAX_s16(min, max, x, y)	MINMAX(min, max, x, y)

#define	MINMAX_u16(min, max, x, y)	MINMAX(min, max, x, y)

/* *********************************************************** */

#define	MINMAX_s32(min, max, x, y)                              \
	{                                                       \
	    mlib_s32 del, mask;                                 \
	                                                        \
	    del = x - y;                                        \
	    mask = (del & ~(y & ~x) | (x & ~y)) >> 31;          \
	    max = x - (del & mask);                             \
	    min = x - (del & ~mask);                            \
	}

/* *********************************************************** */

#define	FUNC_IMAGE_EXTREMA2_2(STYPE)                                     \
	static void                                                      \
	mlib_ImageExtrema2_2_##STYPE(mlib_s32 *dmin, mlib_s32 *dmax,     \
	    mlib_##STYPE * sl, mlib_s32 xsize, mlib_s32 ysize,           \
	    mlib_s32 slb, mlib_s32 off1, mlib_s32 off2, mlib_s32 off3,   \
	    mlib_s32 off4)                                               \
	{                                                                \
	    mlib_s32 min0, min1;                                         \
	    mlib_s32 max0, max1;                                         \
	    mlib_s32 j;                                                  \
	                                                                 \
	    min0 = min1 = dmin[0];                                       \
	    max0 = max1 = dmax[0];                                       \
	    for (j = 0; j < ysize; j++) {                                \
		mlib_##STYPE *sp = sl;                                   \
		mlib_##STYPE *send = sl + xsize;                         \
	                                                                 \
		for (; sp < (send - off3); sp += off4) {                 \
		    mlib_s32 s0, s1, s2, s3;                             \
		    mlib_s32 max02, min02, max13, min13;                 \
	                                                                 \
		    s0 = sp[0];                                          \
		    s1 = sp[off1];                                       \
		    s2 = sp[off2];                                       \
		    s3 = sp[off3];                                       \
		    MINMAX_##STYPE(min02, max02, s0, s2);                \
		    MINMAX_##STYPE(min13, max13, s1, s3);                \
		    MIN_##STYPE(min0, min02);                            \
		    MIN_##STYPE(min1, min13);                            \
		    MAX_##STYPE(max0, max02);                            \
		    MAX_##STYPE(max1, max13);                            \
		}                                                        \
		if ((sp) < send) {                                       \
		    MIN_##STYPE(min0, sp[0]);                            \
		    MAX_##STYPE(max0, sp[0]);                            \
		}                                                        \
		if ((sp + off1) < send) {                                \
		    MIN_##STYPE(min1, sp[off1]);                         \
		    MAX_##STYPE(max1, sp[off1]);                         \
		}                                                        \
		if ((sp + off2) < send) {                                \
		    MIN_##STYPE(min0, sp[off2]);                         \
		    MAX_##STYPE(max0, sp[off2]);                         \
		}                                                        \
		sl += slb;                                               \
	    }                                                            \
	    dmin[0] = min0;                                              \
	    dmax[0] = max0;                                              \
	    dmin[1] = min1;                                              \
	    dmax[1] = max1;                                              \
	}

/* *********************************************************** */

#define	FUNC_IMAGE_EXTREMA2_3(STYPE)                                   \
	static void                                                    \
	mlib_ImageExtrema2_3_##STYPE(mlib_s32 *dmin, mlib_s32 *dmax,   \
	    mlib_##STYPE * sl, mlib_s32 xsize, mlib_s32 ysize,         \
	    mlib_s32 slb, mlib_s32 off3)                               \
	{                                                              \
	    mlib_s32 min0, min1, min2;                                 \
	    mlib_s32 max0, max1, max2;                                 \
	    mlib_s32 j;                                                \
	                                                               \
	    min0 = min1 = min2 = dmin[0];                              \
	    max0 = max1 = max2 = dmax[0];                              \
	    for (j = 0; j < ysize; j++) {                              \
		mlib_##STYPE *sp = sl;                                 \
		mlib_##STYPE *send = sl + xsize;                       \
	                                                               \
		for (; sp < (send - off3 - 2); sp += off3) {           \
		    mlib_s32 s0, s1, s2, s3, s4, s5;                   \
		    mlib_s32 min03, min14, min25;                      \
		    mlib_s32 max03, max14, max25;                      \
	                                                               \
		    s0 = sp[0];                                        \
		    s1 = sp[1];                                        \
		    s2 = sp[2];                                        \
		    sp += off3;                                        \
		    s3 = sp[0];                                        \
		    s4 = sp[1];                                        \
		    s5 = sp[2];                                        \
		    MINMAX_##STYPE(min03, max03, s0, s3);              \
		    MINMAX_##STYPE(min14, max14, s1, s4);              \
		    MINMAX_##STYPE(min25, max25, s2, s5);              \
		    MIN_##STYPE(min0, min03);                          \
		    MIN_##STYPE(min1, min14);                          \
		    MIN_##STYPE(min2, min25);                          \
		    MAX_##STYPE(max0, max03);                          \
		    MAX_##STYPE(max1, max14);                          \
		    MAX_##STYPE(max2, max25);                          \
		}                                                      \
		if ((sp) < send) {                                     \
		    MIN_##STYPE(min0, sp[0]);                          \
		    MAX_##STYPE(max0, sp[0]);                          \
		}                                                      \
		if ((sp + 1) < send) {                                 \
		    MIN_##STYPE(min1, sp[1]);                          \
		    MAX_##STYPE(max1, sp[1]);                          \
		}                                                      \
		if ((sp + 2) < send) {                                 \
		    MIN_##STYPE(min2, sp[2]);                          \
		    MAX_##STYPE(max2, sp[2]);                          \
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

#define	FUNC_IMAGE_EXTREMA2_124(STYPE)                                    \
	static void                                                       \
	mlib_ImageExtrema2_124_##STYPE(mlib_s32 *dmin,                    \
	    mlib_s32 *dmax, mlib_##STYPE * sl, mlib_s32 xsize,            \
	    mlib_s32 ysize, mlib_s32 slb, mlib_s32 off1, mlib_s32 off2,   \
	    mlib_s32 off3, mlib_s32 off4)                                 \
	{                                                                 \
	    mlib_s32 min0, min1, min2, min3;                              \
	    mlib_s32 max0, max1, max2, max3;                              \
	    mlib_s32 j;                                                   \
	                                                                  \
	    min0 = min1 = min2 = min3 = dmin[0];                          \
	    max0 = max1 = max2 = max3 = dmax[0];                          \
	    for (j = 0; j < ysize; j++) {                                 \
		mlib_##STYPE *sp = sl;                                    \
		mlib_##STYPE *send = sl + xsize;                          \
	                                                                  \
		for (; sp < (send - off4 - off3); sp += off4) {           \
		    mlib_s32 s0, s1, s2, s3, s4, s5, s6, s7;              \
		    mlib_s32 min04, min15, min26, min37;                  \
		    mlib_s32 max04, max15, max26, max37;                  \
	                                                                  \
		    s0 = sp[0];                                           \
		    s1 = sp[off1];                                        \
		    s2 = sp[off2];                                        \
		    s3 = sp[off3];                                        \
		    sp += off4;                                           \
		    s4 = sp[0];                                           \
		    s5 = sp[off1];                                        \
		    s6 = sp[off2];                                        \
		    s7 = sp[off3];                                        \
		    MINMAX_##STYPE(min04, max04, s0, s4);                 \
		    MINMAX_##STYPE(min15, max15, s1, s5);                 \
		    MINMAX_##STYPE(min26, max26, s2, s6);                 \
		    MINMAX_##STYPE(min37, max37, s3, s7);                 \
		    MIN_##STYPE(min0, min04);                             \
		    MIN_##STYPE(min1, min15);                             \
		    MIN_##STYPE(min2, min26);                             \
		    MIN_##STYPE(min3, min37);                             \
		    MAX_##STYPE(max0, max04);                             \
		    MAX_##STYPE(max1, max15);                             \
		    MAX_##STYPE(max2, max26);                             \
		    MAX_##STYPE(max3, max37);                             \
		}                                                         \
		if ((sp) < send) {                                        \
		    MIN_##STYPE(min0, sp[0]);                             \
		    MAX_##STYPE(max0, sp[0]);                             \
		}                                                         \
		if ((sp + off1) < send) {                                 \
		    MIN_##STYPE(min1, sp[off1]);                          \
		    MAX_##STYPE(max1, sp[off1]);                          \
		}                                                         \
		if ((sp + off2) < send) {                                 \
		    MIN_##STYPE(min2, sp[off2]);                          \
		    MAX_##STYPE(max2, sp[off2]);                          \
		}                                                         \
		if ((sp + off3) < send) {                                 \
		    MIN_##STYPE(min3, sp[off3]);                          \
		    MAX_##STYPE(max3, sp[off3]);                          \
		}                                                         \
		sp += off4;                                               \
		if ((sp) < send) {                                        \
		    MIN_##STYPE(min0, sp[0]);                             \
		    MAX_##STYPE(max0, sp[0]);                             \
		}                                                         \
		if ((sp + off1) < send) {                                 \
		    MIN_##STYPE(min1, sp[off1]);                          \
		    MAX_##STYPE(max1, sp[off1]);                          \
		}                                                         \
		if ((sp + off2) < send) {                                 \
		    MIN_##STYPE(min2, sp[off2]);                          \
		    MAX_##STYPE(max2, sp[off2]);                          \
		}                                                         \
		sl += slb;                                                \
	    }                                                             \
	    dmin[0] = min0;                                               \
	    dmax[0] = max0;                                               \
	    dmin[1] = min1;                                               \
	    dmax[1] = max1;                                               \
	    dmin[2] = min2;                                               \
	    dmax[2] = max2;                                               \
	    dmin[3] = min3;                                               \
	    dmax[3] = max3;                                               \
	}

/* *********************************************************** */

#define	FUNC_IMAGE_EXTREMA2_4s(STYPE)                                  \
	static void                                                    \
	mlib_ImageExtrema2_4_##STYPE(mlib_s32 *dmin, mlib_s32 *dmax,   \
	    mlib_##STYPE * sl, mlib_s32 xsize, mlib_s32 ysize,         \
	    mlib_s32 slb, mlib_s32 off4)                               \
	{                                                              \
	    mlib_s32 min0, min1, min2, min3;                           \
	    mlib_s32 max0, max1, max2, max3;                           \
	    mlib_s32 j;                                                \
	                                                               \
	    min0 = min1 = min2 = min3 = dmin[0];                       \
	    max0 = max1 = max2 = max3 = dmax[0];                       \
	    for (j = 0; j < ysize; j++) {                              \
		mlib_##STYPE *sp = sl;                                 \
		mlib_##STYPE *send = sl + xsize;                       \
	                                                               \
		for (; sp < (send - 3); sp += off4) {                  \
		    mlib_##STYPE s0, s1, s2, s3;                       \
	                                                               \
		    s0 = sp[0];                                        \
		    s1 = sp[1];                                        \
		    s2 = sp[2];                                        \
		    s3 = sp[3];                                        \
		    MIN_##STYPE(min0, s0);                             \
		    MIN_##STYPE(min1, s1);                             \
		    MIN_##STYPE(min2, s2);                             \
		    MIN_##STYPE(min3, s3);                             \
		    MAX_##STYPE(max0, s0);                             \
		    MAX_##STYPE(max1, s1);                             \
		    MAX_##STYPE(max2, s2);                             \
		    MAX_##STYPE(max3, s3);                             \
		}                                                      \
		sl += slb;                                             \
	    }                                                          \
	    dmin[0] = min0;                                            \
	    dmax[0] = max0;                                            \
	    dmin[1] = min1;                                            \
	    dmax[1] = max1;                                            \
	    dmin[2] = min2;                                            \
	    dmax[2] = max2;                                            \
	    dmin[3] = min3;                                            \
	    dmax[3] = max3;                                            \
	}

/* *********************************************************** */

FUNC_IMAGE_EXTREMA2_2(u8)
    FUNC_IMAGE_EXTREMA2_3(u8)
    FUNC_IMAGE_EXTREMA2_4s(u8)
    FUNC_IMAGE_EXTREMA2_2(s16)
    FUNC_IMAGE_EXTREMA2_3(s16)
    FUNC_IMAGE_EXTREMA2_4s(s16)
    FUNC_IMAGE_EXTREMA2_2(u16)
    FUNC_IMAGE_EXTREMA2_3(u16)
    FUNC_IMAGE_EXTREMA2_4s(u16)

/*
 *  FUNC_IMAGE_EXTREMA2_2(s32)
 *  covered by FUNC_IMAGE_EXTREMA2_124(s32)
 */
    FUNC_IMAGE_EXTREMA2_3(s32)
    FUNC_IMAGE_EXTREMA2_124(s32)

/* *********************************************************** */
    mlib_status __mlib_ImageExtrema2(
    mlib_s32 *min,
    mlib_s32 *max,
    const mlib_image *img,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 xPeriod,
    mlib_s32 yPeriod)
{
	mlib_type type;
	void *sl;
	mlib_s32 nchan, xsize, ysize, slb, tsize;
	mlib_s32 dmin[4], dmax[4];

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

	if (type == MLIB_BYTE)
		tsize = 1;
	else if (type == MLIB_SHORT)
		tsize = 2;
	else if (type == MLIB_USHORT)
		tsize = 2;
	else if (type == MLIB_INT)
		tsize = 4;
	else
		return (MLIB_FAILURE);

	sl = (mlib_u8 *)sl + yStart * slb + xStart * nchan * tsize;
	xsize = (xsize - xStart);
	ysize = ((ysize - yStart) + (yPeriod - 1)) / yPeriod;
	slb = slb * yPeriod;

	if (xsize <= 0 || ysize <= 0)
		return (MLIB_SUCCESS);

/* on VIS and (xPeriod == 1) branch to mlib_ImageMinimum/mlib_ImageMaximum */

	if (xPeriod == 1) {
		mlib_image tmp_img;

		if (mlib_ImageSet(&tmp_img, type, nchan, xsize, ysize, slb,
		    sl) != NULL) {
			if (mlib_ImageExtrema2_fast(min, max,
			    &tmp_img) == MLIB_SUCCESS)
				return (MLIB_SUCCESS);
		}
	}

	xsize *= nchan;
	slb /= tsize;

	if (type == MLIB_BYTE) {

		dmin[0] = min[0] = MLIB_U8_MAX;
		dmax[0] = max[0] = MLIB_U8_MIN;

		switch (nchan) {
		case 1:
			mlib_ImageExtrema2_2_u8(dmin, dmax, sl, xsize, ysize,
			    slb, xPeriod, 2 * xPeriod, 3 * xPeriod,
			    4 * xPeriod);
			break;

		case 2:
			mlib_ImageExtrema2_2_u8(min, max, sl, xsize, ysize, slb,
			    1, 2 * xPeriod, 2 * xPeriod + 1, 4 * xPeriod);
			break;

		case 3:
			mlib_ImageExtrema2_3_u8(min, max, sl, xsize, ysize, slb,
			    3 * xPeriod);
			break;

		case 4:
			mlib_ImageExtrema2_4_u8(min, max, sl, xsize, ysize, slb,
			    4 * xPeriod);
			break;

		default:
			return (MLIB_FAILURE);
		}

	} else if (type == MLIB_SHORT) {

		dmin[0] = min[0] = MLIB_S16_MAX;
		dmax[0] = max[0] = MLIB_S16_MIN;

		switch (nchan) {
		case 1:
			mlib_ImageExtrema2_2_s16(dmin, dmax, sl, xsize, ysize,
			    slb, xPeriod, 2 * xPeriod, 3 * xPeriod,
			    4 * xPeriod);
			break;

		case 2:
			mlib_ImageExtrema2_2_s16(min, max, sl, xsize, ysize,
			    slb, 1, 2 * xPeriod, 2 * xPeriod + 1, 4 * xPeriod);
			break;

		case 3:
			mlib_ImageExtrema2_3_s16(min, max, sl, xsize, ysize,
			    slb, 3 * xPeriod);
			break;

		case 4:
			mlib_ImageExtrema2_4_s16(min, max, sl, xsize, ysize,
			    slb, 4 * xPeriod);
			break;

		default:
			return (MLIB_FAILURE);
		}

	} else if (type == MLIB_USHORT) {
		dmin[0] = min[0] = MLIB_U16_MAX;
		dmax[0] = max[0] = MLIB_U16_MIN;

		switch (nchan) {
		case 1:
			mlib_ImageExtrema2_2_u16(dmin, dmax, sl, xsize, ysize,
			    slb, xPeriod, 2 * xPeriod, 3 * xPeriod,
			    4 * xPeriod);
			break;

		case 2:
			mlib_ImageExtrema2_2_u16(min, max, sl, xsize, ysize,
			    slb, 1, 2 * xPeriod, 2 * xPeriod + 1, 4 * xPeriod);
			break;

		case 3:
			mlib_ImageExtrema2_3_u16(min, max, sl, xsize, ysize,
			    slb, 3 * xPeriod);
			break;

		case 4:
			mlib_ImageExtrema2_4_u16(min, max, sl, xsize, ysize,
			    slb, 4 * xPeriod);
			break;

		default:
			return (MLIB_FAILURE);
		}

	} else if (type == MLIB_INT) {

		dmin[0] = min[0] = MLIB_S32_MAX;
		dmax[0] = max[0] = MLIB_S32_MIN;

		switch (nchan) {
		case 1:
			mlib_ImageExtrema2_124_s32(dmin, dmax, sl, xsize, ysize,
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
			return (MLIB_SUCCESS);

		case 2:
			mlib_ImageExtrema2_124_s32(dmin, dmax, sl, xsize, ysize,
			    slb, 1, 2 * xPeriod, 2 * xPeriod + 1, 4 * xPeriod);
			MIN(dmin[0], dmin[2]);
			MAX(dmax[0], dmax[2]);
			MIN(dmin[1], dmin[3]);
			MAX(dmax[1], dmax[3]);
			min[0] = dmin[0];
			max[0] = dmax[0];
			min[1] = dmin[1];
			max[1] = dmax[1];
			return (MLIB_SUCCESS);

		case 3:
			mlib_ImageExtrema2_3_s32(min, max, sl, xsize, ysize,
			    slb, 3 * xPeriod);
			return (MLIB_SUCCESS);

		case 4:
			mlib_ImageExtrema2_124_s32(min, max, sl, xsize, ysize,
			    slb, 1, 2, 3, 4 * xPeriod);
			return (MLIB_SUCCESS);

		default:
			return (MLIB_FAILURE);
		}
	}

	if (nchan == 1) {
		MIN(dmin[0], dmin[1]);
		MAX(dmax[0], dmax[1]);
		min[0] = dmin[0];
		max[0] = dmax[0];
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
