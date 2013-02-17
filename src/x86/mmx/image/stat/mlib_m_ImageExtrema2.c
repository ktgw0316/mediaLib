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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_m_ImageExtrema2.c	9.7	07/11/05 SMI"

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
 * RESTRICTION
 *      The image can have 1, 2, 3 or 4 channels.
 *      It can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
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
 */

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <mmintrin.h>
#endif

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_mmx_utils.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageExtrema2 = __mlib_ImageExtrema2

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageExtrema2) mlib_ImageExtrema2
    __attribute__((weak, alias("__mlib_ImageExtrema2")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

    mlib_image *mlib_ImageSet(
    mlib_image *image,
    mlib_type type,
    mlib_s32 channels,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    void *data);

/* *********************************************************** */

mlib_status
mlib_ImageExtrema2_fast(
    mlib_s32 *min,
    mlib_s32 *max,
    mlib_image *img)
{
	mlib_status res;

	res = __mlib_ImageMinimum(min, img);
	if (res != MLIB_SUCCESS)
		return (res);

	res = __mlib_ImageMaximum(max, img);
	if (res != MLIB_SUCCESS)
		return (res);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MIN(min, x)	min = (min < x) ? min : x
#define	MAX(max, x)	max = (max > x) ? max : x

#define	MIN_u8(min, x)	min += ((x - min) & ((x - min) >> 31))
#define	MIN_u16(min, x)	min += ((x - min) & ((x - min) >> 31))
#define	MIN_s16(min, x)	min += ((x - min) & ((x - min) >> 31))
#define	MIN_s32(min, x)	MIN(min, x)

#define	MAX_u8(max, x)	max -= (max - x) & ((max - x) >> 31)
#define	MAX_u16(max, x)	max -= (max - x) & ((max - x) >> 31)
#define	MAX_s16(max, x)	max -= (max - x) & ((max - x) >> 31)
#define	MAX_s32(max, x)	MAX(max, x)

/* *********************************************************** */

#define	MLIB_M_IMAGE_MAXIMUM_S16(dd1, max, sd1)                 \
	{                                                       \
	    __m64 cmask = _mm_cmpgt_pi16(sd1, max);             \
	                                                        \
	    dd1 =                                               \
		_mm_or_si64(_mm_and_si64(sd1, cmask),           \
		_mm_andnot_si64(cmask, max));                   \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_MINIMUM_S16(dd1, max, sd1)                 \
	{                                                       \
	    __m64 cmask = _mm_cmpgt_pi16(max, sd1);             \
	                                                        \
	    dd1 =                                               \
		_mm_or_si64(_mm_and_si64(sd1, cmask),           \
		_mm_andnot_si64(cmask, max));                   \
	}

/* *********************************************************** */

void
mlib_m_ImageExtrema2_1_U8(
    mlib_s32 *dmin,
    mlib_s32 *dmax,
    mlib_u8 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 slb,
    mlib_s32 off1,
    mlib_s32 off2,
    mlib_s32 off3,
    mlib_s32 off4)
{
	__m64 _mm_load_min;
	__m64 _mm_load_max;
	__m64 _mm_min;
	__m64 _mm_max;
	__m64 _mm_mask_fin;
	mlib_s32 j;
	mlib_s32 min0, min1, min_l, min_h;
	mlib_s32 max0, max1, max_l, max_h;

	_mm_mask_fin = _mm_mlib_setr_pi16(0xffff, 0x0000, 0xffff, 0x0000);
	_mm_min = _mm_set1_pi16(MLIB_U8_MAX);
	_mm_max = _mm_set1_pi16(MLIB_U8_MIN);

	min0 = min1 = dmin[0];
	max0 = max1 = dmax[0];

	for (j = 0; j < ysize; j++) {
		mlib_u8 *sp = sl;
		mlib_u8 *send = sl + xsize;

		for (; sp < (send - off3); sp += off4) {
			_mm_load_min = _mm_load_max =
			    _mm_mlib_setr_pi16(sp[0], sp[off1], sp[off2],
			    sp[off3]);
			MLIB_M_IMAGE_MAXIMUM_S16(_mm_max, _mm_max,
			    _mm_load_max);
			MLIB_M_IMAGE_MINIMUM_S16(_mm_min, _mm_min,
			    _mm_load_max);
		}

		if ((sp + off2) < send) {
			_mm_load_min =
			    _mm_mlib_setr_pi16(sp[0], sp[off1], sp[off2],
			    MLIB_U8_MAX);
			_mm_load_max =
			    _mm_mlib_setr_pi16(sp[0], sp[off1], sp[off2],
			    MLIB_U8_MIN);
		} else if ((sp + off1) < send) {
			_mm_load_min =
			    _mm_mlib_setr_pi16(sp[0], sp[off1], MLIB_U8_MAX,
			    MLIB_U8_MAX);
			_mm_load_max =
			    _mm_mlib_setr_pi16(sp[0], sp[off1], MLIB_U8_MIN,
			    MLIB_U8_MIN);
		} else if ((sp) < send) {
			_mm_load_min =
			    _mm_mlib_setr_pi16(sp[0], MLIB_U8_MAX, MLIB_U8_MAX,
			    MLIB_U8_MAX);
			_mm_load_max =
			    _mm_mlib_setr_pi16(sp[0], MLIB_U8_MIN, MLIB_U8_MIN,
			    MLIB_U8_MIN);
		}

		MLIB_M_IMAGE_MAXIMUM_S16(_mm_max, _mm_max, _mm_load_max);
		MLIB_M_IMAGE_MINIMUM_S16(_mm_min, _mm_min, _mm_load_min);
		sl += slb;
	}

	min_h =
	    _mm_cvtsi64_si32(_mm_srli_si64(_mm_and_si64(_mm_min, _mm_mask_fin),
	    32));
	max_h =
	    _mm_cvtsi64_si32(_mm_srli_si64(_mm_and_si64(_mm_max, _mm_mask_fin),
	    32));
	min_l = _mm_cvtsi64_si32(_mm_and_si64(_mm_min, _mm_mask_fin));
	max_l = _mm_cvtsi64_si32(_mm_and_si64(_mm_max, _mm_mask_fin));

	min0 = min_l + ((min_h - min_l) & ((min_h - min_l) >> 31));
	max0 = max_l - ((max_l - max_h) & ((max_l - max_h) >> 31));
	_mm_min = _mm_srli_pi32(_mm_min, 16);
	_mm_max = _mm_srli_pi32(_mm_max, 16);

	min_h = _mm_cvtsi64_si32(_mm_srli_si64(_mm_min, 32));
	max_h = _mm_cvtsi64_si32(_mm_srli_si64(_mm_max, 32));
	min_l = _mm_cvtsi64_si32(_mm_min);
	max_l = _mm_cvtsi64_si32(_mm_max);
	min1 = min_l + ((min_h - min_l) & ((min_h - min_l) >> 31));
	max1 = max_l - ((max_l - max_h) & ((max_l - max_h) >> 31));

	_mm_empty();

	MIN_u8(min0, min1);
	MAX_u8(max0, max1);

	dmin[0] = min0;
	dmax[0] = max0;
	dmin[1] = min1;
	dmax[1] = max1;
}

/* *********************************************************** */

void
mlib_m_ImageExtrema2_2_U8(
    mlib_s32 *dmin,
    mlib_s32 *dmax,
    mlib_u8 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 slb,
    mlib_s32 off1,
    mlib_s32 off2,
    mlib_s32 off3,
    mlib_s32 off4)
{
	__m64 _mm_load_min;
	__m64 _mm_load_max;
	__m64 _mm_min;
	__m64 _mm_max;
	__m64 _mm_mask_fin;
	mlib_s32 min0, min1, min_l, min_h;
	mlib_s32 max0, max1, max_l, max_h;
	mlib_s32 j;

	_mm_min = _mm_set1_pi16((mlib_s16)MLIB_U8_MAX);
	_mm_max = _mm_set1_pi16((mlib_s16)MLIB_U8_MIN);
	_mm_mask_fin = _mm_mlib_setr_pi16(0xffff, 0x0000, 0xffff, 0x0000);

	min0 = min1 = dmin[0];
	max0 = max1 = dmax[0];

	for (j = 0; j < ysize; j++) {
		mlib_u8 *sp = sl;
		mlib_u8 *send = sl + xsize;

		for (; sp < (send - off3); sp += off4) {
			_mm_load_min = _mm_load_max =
			    _mm_mlib_setr_pi16(sp[0], sp[off1], sp[off2],
			    sp[off3]);
			MLIB_M_IMAGE_MAXIMUM_S16(_mm_max, _mm_max,
			    _mm_load_max);
			MLIB_M_IMAGE_MINIMUM_S16(_mm_min, _mm_min,
			    _mm_load_max);
		}

		if ((sp + off1) < send) {
			_mm_load_min =
			    _mm_mlib_setr_pi16(sp[0], sp[off1], MLIB_U8_MAX,
			    MLIB_U8_MAX);
			_mm_load_max =
			    _mm_mlib_setr_pi16(sp[0], sp[off1], MLIB_U8_MIN,
			    MLIB_U8_MIN);
		}

		MLIB_M_IMAGE_MAXIMUM_S16(_mm_max, _mm_max, _mm_load_max);
		MLIB_M_IMAGE_MINIMUM_S16(_mm_min, _mm_min, _mm_load_min);
		sl += slb;
	}

	min_h =
	    _mm_cvtsi64_si32(_mm_srli_si64(_mm_and_si64(_mm_min, _mm_mask_fin),
	    32));
	max_h =
	    _mm_cvtsi64_si32(_mm_srli_si64(_mm_and_si64(_mm_max, _mm_mask_fin),
	    32));
	min_l = _mm_cvtsi64_si32(_mm_and_si64(_mm_min, _mm_mask_fin));
	max_l = _mm_cvtsi64_si32(_mm_and_si64(_mm_max, _mm_mask_fin));
	min0 = min_l + ((min_h - min_l) & ((min_h - min_l) >> 31));
	max0 = max_l - ((max_l - max_h) & ((max_l - max_h) >> 31));
	_mm_min = _mm_srli_pi32(_mm_min, 16);
	_mm_max = _mm_srli_pi32(_mm_max, 16);

	min_h = _mm_cvtsi64_si32(_mm_srli_si64(_mm_min, 32));
	max_h = _mm_cvtsi64_si32(_mm_srli_si64(_mm_max, 32));
	min_l = _mm_cvtsi64_si32(_mm_min);
	max_l = _mm_cvtsi64_si32(_mm_max);
	min1 = min_l + ((min_h - min_l) & ((min_h - min_l) >> 31));
	max1 = max_l - ((max_l - max_h) & ((max_l - max_h) >> 31));

	_mm_empty();

	dmin[0] = min0;
	dmax[0] = max0;
	dmin[1] = min1;
	dmax[1] = max1;
}

/* *********************************************************** */

void
mlib_m_ImageExtrema2_4_U8(
    mlib_s32 *dmin,
    mlib_s32 *dmax,
    mlib_u8 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 slb,
    mlib_s32 off4)
{
	__m64 _mm_load;
	__m64 _mm_min;
	__m64 _mm_max;
	mlib_s32 min0, min1, min2, min3;
	mlib_s32 max0, max1, max2, max3;
	mlib_s32 j;
	mlib_s16 temp_store[8];

	_mm_min = _mm_set1_pi16(MLIB_U8_MAX);
	_mm_max = _mm_set1_pi16(MLIB_U8_MIN);

	for (j = 0; j < ysize; j++) {
		mlib_u8 *sp = sl;
		mlib_u8 *send = sl + xsize;

		for (; sp < (send - 3); sp += off4) {
			_mm_load =
			    _mm_mlib_setr_pi16(sp[0], sp[1], sp[2], sp[3]);
			MLIB_M_IMAGE_MAXIMUM_S16(_mm_max, _mm_max, _mm_load);
			MLIB_M_IMAGE_MINIMUM_S16(_mm_min, _mm_min, _mm_load);
		}

		sl += slb;
	}

	((__m64 *) temp_store)[0] = _mm_min;
	((__m64 *) temp_store)[1] = _mm_max;

	_mm_empty();

	dmin[0] = temp_store[0];
	dmin[1] = temp_store[1];
	dmin[2] = temp_store[2];
	dmin[3] = temp_store[3];
	dmax[0] = temp_store[4];
	dmax[1] = temp_store[5];
	dmax[2] = temp_store[6];
	dmax[3] = temp_store[7];
}

/* *********************************************************** */

void
mlib_m_ImageExtrema2_3_U8(
    mlib_s32 *dmin,
    mlib_s32 *dmax,
    mlib_u8 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 slb,
    mlib_s32 period)
{
	__m64 _mm_load_min0, _mm_load_min1, _mm_load_min2;
	__m64 _mm_load_max0, _mm_load_max1, _mm_load_max2;
	__m64 _mm_min0, _mm_min1, _mm_min2;
	__m64 _mm_max0, _mm_max1, _mm_max2;
	__m64 _mm_mask;
	mlib_s32 min0, min1, min2, min3;
	mlib_s32 max0, max1, max2, max3;
	mlib_s32 j;
	mlib_s32 a = period;
	mlib_s32 b = period * 2;
	mlib_s32 c = period * 3;
	mlib_s32 d = period * 4;
	mlib_s16 temp_store[24];

	_mm_mask = _mm_mlib_setr_pi16(0xffff, 0x0000, 0x0000, 0x0000)
	    _mm_min0 = _mm_min1 = _mm_min2 = _mm_set1_pi16(MLIB_U8_MAX);
	_mm_max0 = _mm_max1 = _mm_max2 = _mm_set1_pi16(MLIB_U8_MIN);

	min0 = min1 = min2 = min3 = dmin[0];
	max0 = max1 = max2 = max3 = dmax[0];

	for (j = 0; j < ysize; j++) {
		mlib_u8 *sp = sl;
		mlib_u8 *loopend = sl + xsize;

		for (; sp < (loopend - c); sp += d) {
			_mm_load_min0 = _mm_load_max0 =
			    _mm_mlib_setr_pi16(sp[0], sp[1], sp[2], sp[a + 0]);
			_mm_load_min1 = _mm_load_max1 =
			    _mm_mlib_setr_pi16(sp[a + 1], sp[a + 2], sp[b + 0],
			    sp[b + 1]);
			_mm_load_min2 = _mm_load_max2 =
			    _mm_mlib_setr_pi16(sp[b + 2], sp[c + 0], sp[c + 1],
			    sp[c + 2]);
			MLIB_M_IMAGE_MAXIMUM_S16(_mm_max0, _mm_max0,
			    _mm_load_max0);
			MLIB_M_IMAGE_MINIMUM_S16(_mm_min0, _mm_min0,
			    _mm_load_max0);
			MLIB_M_IMAGE_MAXIMUM_S16(_mm_max1, _mm_max1,
			    _mm_load_max1);
			MLIB_M_IMAGE_MINIMUM_S16(_mm_min1, _mm_min1,
			    _mm_load_max1);
			MLIB_M_IMAGE_MAXIMUM_S16(_mm_max2, _mm_max2,
			    _mm_load_max2);
			MLIB_M_IMAGE_MINIMUM_S16(_mm_min2, _mm_min2,
			    _mm_load_max2);
		}

		if ((sp + b) < loopend) {
			_mm_load_min0 = _mm_load_max0 =
			    _mm_mlib_setr_pi16(sp[0], sp[1], sp[2], sp[a + 0]);
			_mm_load_min1 = _mm_load_max1 =
			    _mm_mlib_setr_pi16(sp[a + 1], sp[a + 2], sp[b + 0],
			    sp[b + 1]);
			_mm_load_min2 =
			    _mm_mlib_setr_pi16(sp[b + 2], MLIB_U8_MAX,
			    MLIB_U8_MAX, MLIB_U8_MAX);
			_mm_load_max2 =
			    _mm_mlib_setr_pi16(sp[b + 2], MLIB_U8_MIN,
			    MLIB_U8_MIN, MLIB_U8_MIN);
		} else if ((sp + a) < loopend) {
			_mm_load_min0 = _mm_load_max0 =
			    _mm_mlib_setr_pi16(sp[0], sp[1], sp[2], sp[a + 0]);
			_mm_load_min1 =
			    _mm_mlib_setr_pi16(sp[a + 1], sp[a + 2],
			    MLIB_U8_MAX, MLIB_U8_MAX);
			_mm_load_max1 =
			    _mm_mlib_setr_pi16(sp[a + 1], sp[a + 2],
			    MLIB_U8_MIN, MLIB_U8_MIN);
			_mm_load_min2 =
			    _mm_mlib_setr_pi16(MLIB_U8_MAX, MLIB_U8_MAX,
			    MLIB_U8_MAX, MLIB_U8_MAX);
			_mm_load_max2 =
			    _mm_mlib_setr_pi16(MLIB_U8_MIN, MLIB_U8_MIN,
			    MLIB_U8_MIN, MLIB_U8_MIN);
		} else if ((sp) < loopend) {
			_mm_load_min0 =
			    _mm_mlib_setr_pi16(sp[0], sp[1], sp[2],
			    MLIB_U8_MAX);
			_mm_load_max0 =
			    _mm_mlib_setr_pi16(sp[0], sp[1], sp[2],
			    MLIB_U8_MIN);
			_mm_load_min1 =
			    _mm_mlib_setr_pi16(MLIB_U8_MAX, MLIB_U8_MAX,
			    MLIB_U8_MAX, MLIB_U8_MAX);
			_mm_load_max1 =
			    _mm_mlib_setr_pi16(MLIB_U8_MIN, MLIB_U8_MIN,
			    MLIB_U8_MIN, MLIB_U8_MIN);
			_mm_load_min2 =
			    _mm_mlib_setr_pi16(MLIB_U8_MAX, MLIB_U8_MAX,
			    MLIB_U8_MAX, MLIB_U8_MAX);
			_mm_load_max2 =
			    _mm_mlib_setr_pi16(MLIB_U8_MIN, MLIB_U8_MIN,
			    MLIB_U8_MIN, MLIB_U8_MIN);
		}

		MLIB_M_IMAGE_MAXIMUM_S16(_mm_max0, _mm_max0, _mm_load_max0);
		MLIB_M_IMAGE_MINIMUM_S16(_mm_min0, _mm_min0, _mm_load_min0);
		MLIB_M_IMAGE_MAXIMUM_S16(_mm_max1, _mm_max1, _mm_load_max1);
		MLIB_M_IMAGE_MINIMUM_S16(_mm_min1, _mm_min1, _mm_load_min1);
		MLIB_M_IMAGE_MAXIMUM_S16(_mm_max2, _mm_max2, _mm_load_max2);
		MLIB_M_IMAGE_MINIMUM_S16(_mm_min2, _mm_min2, _mm_load_min2);
		sl += slb;
	}

	((__m64 *) temp_store)[0] = _mm_min0;
	((__m64 *) temp_store)[1] = _mm_min1;
	((__m64 *) temp_store)[2] = _mm_min2;
	((__m64 *) temp_store)[3] = _mm_max0;
	((__m64 *) temp_store)[4] = _mm_max1;
	((__m64 *) temp_store)[5] = _mm_max2;

/*    zero channel   */
	min0 = temp_store[0];
	min1 = temp_store[3];
	min2 = temp_store[6];
	min3 = temp_store[9];
	max0 = temp_store[12];
	max1 = temp_store[15];
	max2 = temp_store[18];
	max3 = temp_store[21];
	MIN(min0, min1);
	MIN(min2, min3);
	MIN(min0, min2);
	MAX(max0, max1);
	MAX(max2, max3);
	MAX(max0, max2);
	dmin[0] = min0;
	dmax[0] = max0;

/*    first channel   */
	min0 = temp_store[1];
	min1 = temp_store[4];
	min2 = temp_store[7];
	min3 = temp_store[10];
	max0 = temp_store[13];
	max1 = temp_store[16];
	max2 = temp_store[19];
	max3 = temp_store[22];
	MIN(min0, min1);
	MIN(min2, min3);
	MIN(min0, min2);
	MAX(max0, max1);
	MAX(max2, max3);
	MAX(max0, max2);
	dmin[1] = min0;
	dmax[1] = max0;

/*    second channel   */
	min0 = temp_store[2];
	min1 = temp_store[5];
	min2 = temp_store[8];
	min3 = temp_store[11];
	max0 = temp_store[14];
	max1 = temp_store[17];
	max2 = temp_store[20];
	max3 = temp_store[23];
	MIN(min0, min1);
	MIN(min2, min3);
	MIN(min0, min2);
	MAX(max0, max1);
	MAX(max2, max3);
	MAX(max0, max2);
	dmin[2] = min0;
	dmax[2] = max0;

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageExtrema2_1_S16(
    mlib_s32 *dmin,
    mlib_s32 *dmax,
    mlib_s16 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 slb,
    mlib_s32 off1,
    mlib_s32 off2,
    mlib_s32 off3,
    mlib_s32 off4)
{
	mlib_s32 min0, min1, min2, min3;
	mlib_s32 max0, max1, max2, max3;
	mlib_s16 temp_store[4];
	mlib_s32 j;
	__m64 _mm_load_min;
	__m64 _mm_load_max;
	__m64 _mm_min;
	__m64 _mm_max;

	_mm_min = _mm_set1_pi16(MLIB_S16_MAX);
	_mm_max = _mm_set1_pi16(MLIB_S16_MIN);

	for (j = 0; j < ysize; j++) {
		mlib_s16 *sp = sl;
		mlib_s16 *send = sl + xsize;

		for (; sp < (send - off3); sp += off4) {
			_mm_load_min = _mm_load_max =
			    _mm_mlib_setr_pi16(sp[0], sp[off1], sp[off2],
			    sp[off3]);
			MLIB_M_IMAGE_MAXIMUM_S16(_mm_max, _mm_max,
			    _mm_load_max);
			MLIB_M_IMAGE_MINIMUM_S16(_mm_min, _mm_min,
			    _mm_load_max);
		}

		if ((sp + off2) < send) {
			_mm_load_min =
			    _mm_mlib_setr_pi16(sp[0], sp[off1], sp[off2],
			    MLIB_S16_MAX);
			_mm_load_max =
			    _mm_mlib_setr_pi16(sp[0], sp[off1], sp[off2],
			    MLIB_S16_MIN);
		} else if ((sp + off1) < send) {
			_mm_load_min =
			    _mm_mlib_setr_pi16(sp[0], sp[off1], MLIB_S16_MAX,
			    MLIB_S16_MAX);
			_mm_load_max =
			    _mm_mlib_setr_pi16(sp[0], sp[off1], MLIB_S16_MIN,
			    MLIB_S16_MIN);
		} else if ((sp) < send) {
			_mm_load_min =
			    _mm_mlib_setr_pi16(sp[0], MLIB_S16_MAX,
			    MLIB_S16_MAX, MLIB_S16_MAX);
			_mm_load_max =
			    _mm_mlib_setr_pi16(sp[0], MLIB_S16_MIN,
			    MLIB_S16_MIN, MLIB_S16_MIN);
		}

		MLIB_M_IMAGE_MAXIMUM_S16(_mm_max, _mm_max, _mm_load_max);
		MLIB_M_IMAGE_MINIMUM_S16(_mm_min, _mm_min, _mm_load_min);
		sl += slb;
	}

	((__m64 *) temp_store)[0] = _mm_min;
	min0 = temp_store[0];
	min1 = temp_store[1];
	min2 = temp_store[2];
	min3 = temp_store[3];
	MIN_s16(min0, min1);
	MIN_s16(min2, min3);
	MIN_s16(min0, min2);

	((__m64 *) temp_store)[0] = _mm_max;
	max0 = temp_store[0];
	max1 = temp_store[1];
	max2 = temp_store[2];
	max3 = temp_store[3];
	MAX_s16(max0, max1);
	MAX_s16(max2, max3);
	MAX_s16(max0, max2);

	_mm_empty();

	dmin[0] = min0;
	dmax[0] = max0;
	dmin[1] = min1;
	dmax[1] = max1;
}

/* *********************************************************** */

void
mlib_m_ImageExtrema2_2_S16(
    mlib_s32 *dmin,
    mlib_s32 *dmax,
    mlib_s16 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 slb,
    mlib_s32 off1,
    mlib_s32 off2,
    mlib_s32 off3,
    mlib_s32 off4)
{
	mlib_s32 min0, min1, min2, min3;
	mlib_s32 max0, max1, max2, max3;
	mlib_s16 temp_store[4];
	mlib_s32 j;
	__m64 _mm_load_min;
	__m64 _mm_load_max;
	__m64 _mm_min;
	__m64 _mm_max;

	_mm_min = _mm_set1_pi16(MLIB_S16_MAX);
	_mm_max = _mm_set1_pi16(MLIB_S16_MIN);

	for (j = 0; j < ysize; j++) {
		mlib_s16 *sp = sl;
		mlib_s16 *send = sl + xsize;

		for (; sp < (send - off3); sp += off4) {
			_mm_load_min = _mm_load_max =
			    _mm_mlib_setr_pi16(sp[0], sp[off1], sp[off2],
			    sp[off3]);
			MLIB_M_IMAGE_MAXIMUM_S16(_mm_max, _mm_max,
			    _mm_load_max);
			MLIB_M_IMAGE_MINIMUM_S16(_mm_min, _mm_min,
			    _mm_load_max);
		}

		if ((sp + off1) < send) {
			_mm_load_min =
			    _mm_mlib_setr_pi16(sp[0], sp[off1], MLIB_S16_MAX,
			    MLIB_S16_MAX);
			_mm_load_max =
			    _mm_mlib_setr_pi16(sp[0], sp[off1], MLIB_S16_MIN,
			    MLIB_S16_MIN);
		}

		MLIB_M_IMAGE_MAXIMUM_S16(_mm_max, _mm_max, _mm_load_max);
		MLIB_M_IMAGE_MINIMUM_S16(_mm_min, _mm_min, _mm_load_min);
		sl += slb;
	}

	((__m64 *) temp_store)[0] = _mm_min;
	min0 = temp_store[0];
	min1 = temp_store[1];
	min2 = temp_store[2];
	min3 = temp_store[3];
	MIN_s16(min0, min2);
	MIN_s16(min1, min3);

	((__m64 *) temp_store)[0] = _mm_max;
	max0 = temp_store[0];
	max1 = temp_store[1];
	max2 = temp_store[2];
	max3 = temp_store[3];
	MAX_s16(max0, max2);
	MAX_s16(max1, max3);

	_mm_empty();

	dmin[0] = min0;
	dmax[0] = max0;
	dmin[1] = min1;
	dmax[1] = max1;
}

/* *********************************************************** */

void
mlib_m_ImageExtrema2_3_S16(
    mlib_s32 *dmin,
    mlib_s32 *dmax,
    mlib_s16 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 slb,
    mlib_s32 period)
{
	mlib_s32 j;
	mlib_s32 a = period;
	mlib_s32 b = period * 2;
	mlib_s32 c = period * 3;
	mlib_s32 d = period * 4;
	mlib_s16 temp_store[12];
	__m64 _mm_load_min0, _mm_load_min1, _mm_load_min2;
	__m64 _mm_load_max0, _mm_load_max1, _mm_load_max2;
	__m64 _mm_min0, _mm_min1, _mm_min2;
	__m64 _mm_max0, _mm_max1, _mm_max2;

	_mm_min0 = _mm_min1 = _mm_min2 = _mm_set1_pi16(MLIB_S16_MAX);
	_mm_max0 = _mm_max1 = _mm_max2 = _mm_set1_pi16(MLIB_S16_MIN);

	for (j = 0; j < ysize; j++) {
		mlib_s16 *sp = sl;
		mlib_s16 *loopend = sl + xsize;

		for (; sp < (loopend - c); sp += d) {
			_mm_load_min0 = _mm_load_max0 =
			    _mm_mlib_setr_pi16(sp[0], sp[1], sp[2], sp[a + 0]);
			_mm_load_min1 = _mm_load_max1 =
			    _mm_mlib_setr_pi16(sp[a + 1], sp[a + 2], sp[b + 0],
			    sp[b + 1]);
			_mm_load_min2 = _mm_load_max2 =
			    _mm_mlib_setr_pi16(sp[b + 2], sp[c + 0], sp[c + 1],
			    sp[c + 2]);
			MLIB_M_IMAGE_MAXIMUM_S16(_mm_max0, _mm_max0,
			    _mm_load_max0);
			MLIB_M_IMAGE_MINIMUM_S16(_mm_min0, _mm_min0,
			    _mm_load_max0);
			MLIB_M_IMAGE_MAXIMUM_S16(_mm_max1, _mm_max1,
			    _mm_load_max1);
			MLIB_M_IMAGE_MINIMUM_S16(_mm_min1, _mm_min1,
			    _mm_load_max1);
			MLIB_M_IMAGE_MAXIMUM_S16(_mm_max2, _mm_max2,
			    _mm_load_max2);
			MLIB_M_IMAGE_MINIMUM_S16(_mm_min2, _mm_min2,
			    _mm_load_max2);
		}

		if ((sp + b) < loopend) {
			_mm_load_min0 =
			    _mm_mlib_setr_pi16(sp[0], sp[1], sp[2], sp[a + 0]);
			_mm_load_max0 =
			    _mm_mlib_setr_pi16(sp[0], sp[1], sp[2], sp[a + 0]);
			_mm_load_min1 =
			    _mm_mlib_setr_pi16(sp[a + 1], sp[a + 2], sp[b + 0],
			    sp[b + 1]);
			_mm_load_max1 =
			    _mm_mlib_setr_pi16(sp[a + 1], sp[a + 2], sp[b + 0],
			    sp[b + 1]);
			_mm_load_min2 =
			    _mm_mlib_setr_pi16(sp[b + 2], MLIB_S16_MAX,
			    MLIB_S16_MAX, MLIB_S16_MAX);
			_mm_load_max2 =
			    _mm_mlib_setr_pi16(sp[b + 2], MLIB_S16_MIN,
			    MLIB_S16_MIN, MLIB_S16_MIN);
		} else if ((sp + a) < loopend) {
			_mm_load_min0 =
			    _mm_mlib_setr_pi16(sp[0], sp[1], sp[2], sp[a + 0]);
			_mm_load_max0 =
			    _mm_mlib_setr_pi16(sp[0], sp[1], sp[2], sp[a + 0]);
			_mm_load_min1 =
			    _mm_mlib_setr_pi16(sp[a + 1], sp[a + 2],
			    MLIB_S16_MAX, MLIB_S16_MAX);
			_mm_load_max1 =
			    _mm_mlib_setr_pi16(sp[a + 1], sp[a + 2],
			    MLIB_S16_MIN, MLIB_S16_MIN);
			_mm_load_min2 =
			    _mm_mlib_setr_pi16(MLIB_S16_MAX, MLIB_S16_MAX,
			    MLIB_S16_MAX, MLIB_S16_MAX);
			_mm_load_max2 =
			    _mm_mlib_setr_pi16(MLIB_S16_MIN, MLIB_S16_MIN,
			    MLIB_S16_MIN, MLIB_S16_MIN);
		} else if ((sp) < loopend) {
			_mm_load_min0 =
			    _mm_mlib_setr_pi16(sp[0], sp[1], sp[2],
			    MLIB_S16_MAX);
			_mm_load_max0 =
			    _mm_mlib_setr_pi16(sp[0], sp[1], sp[2],
			    MLIB_S16_MIN);
			_mm_load_min1 =
			    _mm_mlib_setr_pi16(MLIB_S16_MAX, MLIB_S16_MAX,
			    MLIB_S16_MAX, MLIB_S16_MAX);
			_mm_load_max1 =
			    _mm_mlib_setr_pi16(MLIB_S16_MIN, MLIB_S16_MIN,
			    MLIB_S16_MIN, MLIB_S16_MIN);
			_mm_load_min2 =
			    _mm_mlib_setr_pi16(MLIB_S16_MAX, MLIB_S16_MAX,
			    MLIB_S16_MAX, MLIB_S16_MAX);
			_mm_load_max2 =
			    _mm_mlib_setr_pi16(MLIB_S16_MIN, MLIB_S16_MIN,
			    MLIB_S16_MIN, MLIB_S16_MIN);
		}

		MLIB_M_IMAGE_MAXIMUM_S16(_mm_max0, _mm_max0, _mm_load_max0);
		MLIB_M_IMAGE_MINIMUM_S16(_mm_min0, _mm_min0, _mm_load_min0);
		MLIB_M_IMAGE_MAXIMUM_S16(_mm_max1, _mm_max1, _mm_load_max1);
		MLIB_M_IMAGE_MINIMUM_S16(_mm_min1, _mm_min1, _mm_load_min1);
		MLIB_M_IMAGE_MAXIMUM_S16(_mm_max2, _mm_max2, _mm_load_max2);
		MLIB_M_IMAGE_MINIMUM_S16(_mm_min2, _mm_min2, _mm_load_min2);
		sl += slb;
	}

	((__m64 *) temp_store)[0] = _mm_min0;
	((__m64 *) temp_store)[1] = _mm_min1;
	((__m64 *) temp_store)[2] = _mm_min2;

	{
		mlib_s32 min0 = temp_store[0];
		mlib_s32 min1 = temp_store[1];
		mlib_s32 min2 = temp_store[2];

		MIN_s16(min0, temp_store[3]);
		MIN_s16(min0, temp_store[6]);
		MIN_s16(min0, temp_store[9]);
		MIN_s16(min1, temp_store[4]);
		MIN_s16(min1, temp_store[7]);
		MIN_s16(min1, temp_store[10]);
		MIN_s16(min2, temp_store[5]);
		MIN_s16(min2, temp_store[8]);
		MIN_s16(min2, temp_store[11]);
		dmin[0] = min0;
		dmin[1] = min1;
		dmin[2] = min2;
	}

	((__m64 *) temp_store)[0] = _mm_max0;
	((__m64 *) temp_store)[1] = _mm_max1;
	((__m64 *) temp_store)[2] = _mm_max2;

	{
		mlib_s32 max0 = temp_store[0];
		mlib_s32 max1 = temp_store[1];
		mlib_s32 max2 = temp_store[2];

		MAX_s16(max0, temp_store[3]);
		MAX_s16(max0, temp_store[6]);
		MAX_s16(max0, temp_store[9]);
		MAX_s16(max1, temp_store[4]);
		MAX_s16(max1, temp_store[7]);
		MAX_s16(max1, temp_store[10]);
		MAX_s16(max2, temp_store[5]);
		MAX_s16(max2, temp_store[8]);
		MAX_s16(max2, temp_store[11]);
		dmax[0] = max0;
		dmax[1] = max1;
		dmax[2] = max2;
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageExtrema2_4_S16(
    mlib_s32 *dmin,
    mlib_s32 *dmax,
    mlib_s16 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 slb,
    mlib_s32 period)
{
	mlib_s32 min0, min1, min2, min3;
	mlib_s32 max0, max1, max2, max3;
	mlib_s16 temp_store[4];
	mlib_s32 j;
	__m64 _mm_load_min;
	__m64 _mm_load_max;
	__m64 _mm_min;
	__m64 _mm_max;
	__m64 _mm_mask_fin = _mm_mlib_setr_pi16(0xffff, 0x0000, 0xffff, 0x0000);

	_mm_min = _mm_set1_pi16(MLIB_S16_MAX);
	_mm_max = _mm_set1_pi16(MLIB_S16_MIN);

	for (j = 0; j < ysize; j++) {
		mlib_s16 *sp = sl;
		mlib_s16 *send = sl + xsize;

		for (; sp < (send - 3); sp += period) {
			_mm_load_min = _mm_load_max =
			    _mm_mlib_setr_pi16(sp[0], sp[1], sp[2], sp[3]);
			MLIB_M_IMAGE_MAXIMUM_S16(_mm_max, _mm_max,
			    _mm_load_max);
			MLIB_M_IMAGE_MINIMUM_S16(_mm_min, _mm_min,
			    _mm_load_max);
		}

		MLIB_M_IMAGE_MAXIMUM_S16(_mm_max, _mm_max, _mm_load_max);
		MLIB_M_IMAGE_MINIMUM_S16(_mm_min, _mm_min, _mm_load_min);
		sl += slb;
	}

	((__m64 *) temp_store)[0] = _mm_min;
	dmin[0] = temp_store[0];
	dmin[1] = temp_store[1];
	dmin[2] = temp_store[2];
	dmin[3] = temp_store[3];

	((__m64 *) temp_store)[0] = _mm_max;
	dmax[0] = temp_store[0];
	dmax[1] = temp_store[1];
	dmax[2] = temp_store[2];
	dmax[3] = temp_store[3];

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_MAXIMUM_U16(dd1, max, sd1)                     \
	{                                                           \
	    __m64 cmask =                                           \
		_mm_cmpeq_pi16(_mm_subs_pu16(max, sd1), _m_zero);   \
	    dd1 =                                                   \
		_mm_or_si64(_mm_and_si64(sd1, cmask),               \
		_mm_andnot_si64(cmask, max));                       \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_MINIMUM_U16(dd1, max, sd1)                     \
	{                                                           \
	    __m64 cmask =                                           \
		_mm_cmpeq_pi16(_mm_subs_pu16(sd1, max), _m_zero);   \
	    dd1 =                                                   \
		_mm_or_si64(_mm_and_si64(sd1, cmask),               \
		_mm_andnot_si64(cmask, max));                       \
	}

/* *********************************************************** */

void
mlib_m_ImageExtrema2_1_U16(
    mlib_s32 *dmin,
    mlib_s32 *dmax,
    mlib_u16 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 slb,
    mlib_s32 off1,
    mlib_s32 off2,
    mlib_s32 off3,
    mlib_s32 off4)
{
	mlib_s32 min0, min1, min2, min3;
	mlib_s32 max0, max1, max2, max3;
	mlib_u16 temp_store[4];
	mlib_s32 j;
	__m64 _mm_load_min;
	__m64 _mm_load_max;
	__m64 _mm_min;
	__m64 _mm_max;

	_mm_min = _mm_set1_pi16(MLIB_U16_MAX);
	_mm_max = _mm_set1_pi16(MLIB_U16_MIN);

	for (j = 0; j < ysize; j++) {
		mlib_u16 *sp = sl;
		mlib_u16 *send = sl + xsize;

		for (; sp < (send - off3); sp += off4) {
			_mm_load_min = _mm_load_max =
			    _mm_mlib_setr_pi16(sp[0], sp[off1], sp[off2],
			    sp[off3]);
			MLIB_M_IMAGE_MAXIMUM_U16(_mm_max, _mm_max,
			    _mm_load_max);
			MLIB_M_IMAGE_MINIMUM_U16(_mm_min, _mm_min,
			    _mm_load_max);
		}

		if ((sp + off2) < send) {
			_mm_load_min =
			    _mm_mlib_setr_pi16(sp[0], sp[off1], sp[off2],
			    MLIB_U16_MAX);
			_mm_load_max =
			    _mm_mlib_setr_pi16(sp[0], sp[off1], sp[off2],
			    MLIB_U16_MIN);
		} else if ((sp + off1) < send) {
			_mm_load_min =
			    _mm_mlib_setr_pi16(sp[0], sp[off1], MLIB_U16_MAX,
			    MLIB_U16_MAX);
			_mm_load_max =
			    _mm_mlib_setr_pi16(sp[0], sp[off1], MLIB_U16_MIN,
			    MLIB_U16_MIN);
		} else if ((sp) < send) {
			_mm_load_min =
			    _mm_mlib_setr_pi16(sp[0], MLIB_U16_MAX,
			    MLIB_U16_MAX, MLIB_U16_MAX);
			_mm_load_max =
			    _mm_mlib_setr_pi16(sp[0], MLIB_U16_MIN,
			    MLIB_U16_MIN, MLIB_U16_MIN);
		}

		MLIB_M_IMAGE_MAXIMUM_U16(_mm_max, _mm_max, _mm_load_max);
		MLIB_M_IMAGE_MINIMUM_U16(_mm_min, _mm_min, _mm_load_min);
		sl += slb;
	}

	((__m64 *) temp_store)[0] = _mm_min;
	min0 = temp_store[0];
	min1 = temp_store[1];
	min2 = temp_store[2];
	min3 = temp_store[3];
	MIN_u16(min0, min1);
	MIN_u16(min2, min3);
	MIN_u16(min0, min2);

	((__m64 *) temp_store)[0] = _mm_max;
	max0 = temp_store[0];
	max1 = temp_store[1];
	max2 = temp_store[2];
	max3 = temp_store[3];
	MAX_u16(max0, max1);
	MAX_u16(max2, max3);
	MAX_u16(max0, max2);

	_mm_empty();

	dmin[0] = min0;
	dmax[0] = max0;
	dmin[1] = min1;
	dmax[1] = max1;
}

/* *********************************************************** */

void
mlib_m_ImageExtrema2_2_U16(
    mlib_s32 *dmin,
    mlib_s32 *dmax,
    mlib_u16 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 slb,
    mlib_s32 off1,
    mlib_s32 off2,
    mlib_s32 off3,
    mlib_s32 off4)
{
	mlib_s32 min0, min1, min2, min3;
	mlib_s32 max0, max1, max2, max3;
	mlib_u16 temp_store[4];
	mlib_s32 j;
	__m64 _mm_load_min;
	__m64 _mm_load_max;
	__m64 _mm_min;
	__m64 _mm_max;

	_mm_min = _mm_set1_pi16(MLIB_U16_MAX);
	_mm_max = _mm_set1_pi16(MLIB_U16_MIN);

	for (j = 0; j < ysize; j++) {
		mlib_u16 *sp = sl;
		mlib_u16 *send = sl + xsize;

		for (; sp < (send - off3); sp += off4) {
			_mm_load_min = _mm_load_max =
			    _mm_mlib_setr_pi16(sp[0], sp[off1], sp[off2],
			    sp[off3]);
			MLIB_M_IMAGE_MAXIMUM_U16(_mm_max, _mm_max,
			    _mm_load_max);
			MLIB_M_IMAGE_MINIMUM_U16(_mm_min, _mm_min,
			    _mm_load_max);
		}

		if ((sp + off1) < send) {
			_mm_load_min =
			    _mm_mlib_setr_pi16(sp[0], sp[off1], MLIB_U16_MAX,
			    MLIB_U16_MAX);
			_mm_load_max =
			    _mm_mlib_setr_pi16(sp[0], sp[off1], MLIB_U16_MIN,
			    MLIB_U16_MIN);
		}

		MLIB_M_IMAGE_MAXIMUM_U16(_mm_max, _mm_max, _mm_load_max);
		MLIB_M_IMAGE_MINIMUM_U16(_mm_min, _mm_min, _mm_load_min);
		sl += slb;
	}

	((__m64 *) temp_store)[0] = _mm_min;
	min0 = temp_store[0];
	min1 = temp_store[1];
	min2 = temp_store[2];
	min3 = temp_store[3];
	MIN_u16(min0, min2);
	MIN_u16(min1, min3);

	((__m64 *) temp_store)[0] = _mm_max;
	max0 = temp_store[0];
	max1 = temp_store[1];
	max2 = temp_store[2];
	max3 = temp_store[3];
	MAX_u16(max0, max2);
	MAX_u16(max1, max3);

	_mm_empty();

	dmin[0] = min0;
	dmax[0] = max0;
	dmin[1] = min1;
	dmax[1] = max1;
}

/* *********************************************************** */

void
mlib_m_ImageExtrema2_3_U16(
    mlib_s32 *dmin,
    mlib_s32 *dmax,
    mlib_u16 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 slb,
    mlib_s32 period)
{
	mlib_s32 j;
	mlib_s32 a = period;
	mlib_s32 b = period * 2;
	mlib_s32 c = period * 3;
	mlib_s32 d = period * 4;
	mlib_u16 temp_store[12];
	__m64 _mm_load_min0, _mm_load_min1, _mm_load_min2;
	__m64 _mm_load_max0, _mm_load_max1, _mm_load_max2;
	__m64 _mm_min0, _mm_min1, _mm_min2;
	__m64 _mm_max0, _mm_max1, _mm_max2;

	_mm_min0 = _mm_min1 = _mm_min2 = _mm_set1_pi16(MLIB_U16_MAX);
	_mm_max0 = _mm_max1 = _mm_max2 = _mm_set1_pi16(MLIB_U16_MIN);

	for (j = 0; j < ysize; j++) {
		mlib_u16 *sp = sl;
		mlib_u16 *loopend = sl + xsize;

		for (; sp < (loopend - c); sp += d) {
			_mm_load_min0 = _mm_load_max0 =
			    _mm_mlib_setr_pi16(sp[0], sp[1], sp[2], sp[a + 0]);
			_mm_load_min1 = _mm_load_max1 =
			    _mm_mlib_setr_pi16(sp[a + 1], sp[a + 2], sp[b + 0],
			    sp[b + 1]);
			_mm_load_min2 = _mm_load_max2 =
			    _mm_mlib_setr_pi16(sp[b + 2], sp[c + 0], sp[c + 1],
			    sp[c + 2]);
			MLIB_M_IMAGE_MAXIMUM_U16(_mm_max0, _mm_max0,
			    _mm_load_max0);
			MLIB_M_IMAGE_MINIMUM_U16(_mm_min0, _mm_min0,
			    _mm_load_max0);
			MLIB_M_IMAGE_MAXIMUM_U16(_mm_max1, _mm_max1,
			    _mm_load_max1);
			MLIB_M_IMAGE_MINIMUM_U16(_mm_min1, _mm_min1,
			    _mm_load_max1);
			MLIB_M_IMAGE_MAXIMUM_U16(_mm_max2, _mm_max2,
			    _mm_load_max2);
			MLIB_M_IMAGE_MINIMUM_U16(_mm_min2, _mm_min2,
			    _mm_load_max2);
		}

		if ((sp + b) < loopend) {
			_mm_load_min0 =
			    _mm_mlib_setr_pi16(sp[0], sp[1], sp[2], sp[a + 0]);
			_mm_load_max0 =
			    _mm_mlib_setr_pi16(sp[0], sp[1], sp[2], sp[a + 0]);
			_mm_load_min1 =
			    _mm_mlib_setr_pi16(sp[a + 1], sp[a + 2], sp[b + 0],
			    sp[b + 1]);
			_mm_load_max1 =
			    _mm_mlib_setr_pi16(sp[a + 1], sp[a + 2], sp[b + 0],
			    sp[b + 1]);
			_mm_load_min2 =
			    _mm_mlib_setr_pi16(sp[b + 2], MLIB_U16_MAX,
			    MLIB_U16_MAX, MLIB_U16_MAX);
			_mm_load_max2 =
			    _mm_mlib_setr_pi16(sp[b + 2], MLIB_U16_MIN,
			    MLIB_U16_MIN, MLIB_U16_MIN);
		} else if ((sp + a) < loopend) {
			_mm_load_min0 =
			    _mm_mlib_setr_pi16(sp[0], sp[1], sp[2], sp[a + 0]);
			_mm_load_max0 =
			    _mm_mlib_setr_pi16(sp[0], sp[1], sp[2], sp[a + 0]);
			_mm_load_min1 =
			    _mm_mlib_setr_pi16(sp[a + 1], sp[a + 2],
			    MLIB_U16_MAX, MLIB_U16_MAX);
			_mm_load_max1 =
			    _mm_mlib_setr_pi16(sp[a + 1], sp[a + 2],
			    MLIB_U16_MIN, MLIB_U16_MIN);
			_mm_load_min2 =
			    _mm_mlib_setr_pi16(MLIB_U16_MAX, MLIB_U16_MAX,
			    MLIB_U16_MAX, MLIB_U16_MAX);
			_mm_load_max2 =
			    _mm_mlib_setr_pi16(MLIB_U16_MIN, MLIB_U16_MIN,
			    MLIB_U16_MIN, MLIB_U16_MIN);
		} else if ((sp) < loopend) {
			_mm_load_min0 =
			    _mm_mlib_setr_pi16(sp[0], sp[1], sp[2],
			    MLIB_U16_MAX);
			_mm_load_max0 =
			    _mm_mlib_setr_pi16(sp[0], sp[1], sp[2],
			    MLIB_U16_MIN);
			_mm_load_min1 =
			    _mm_mlib_setr_pi16(MLIB_U16_MAX, MLIB_U16_MAX,
			    MLIB_U16_MAX, MLIB_U16_MAX);
			_mm_load_max1 =
			    _mm_mlib_setr_pi16(MLIB_U16_MIN, MLIB_U16_MIN,
			    MLIB_U16_MIN, MLIB_U16_MIN);
			_mm_load_min2 =
			    _mm_mlib_setr_pi16(MLIB_U16_MAX, MLIB_U16_MAX,
			    MLIB_U16_MAX, MLIB_U16_MAX);
			_mm_load_max2 =
			    _mm_mlib_setr_pi16(MLIB_U16_MIN, MLIB_U16_MIN,
			    MLIB_U16_MIN, MLIB_U16_MIN);
		}

		MLIB_M_IMAGE_MAXIMUM_U16(_mm_max0, _mm_max0, _mm_load_max0);
		MLIB_M_IMAGE_MINIMUM_U16(_mm_min0, _mm_min0, _mm_load_min0);
		MLIB_M_IMAGE_MAXIMUM_U16(_mm_max1, _mm_max1, _mm_load_max1);
		MLIB_M_IMAGE_MINIMUM_U16(_mm_min1, _mm_min1, _mm_load_min1);
		MLIB_M_IMAGE_MAXIMUM_U16(_mm_max2, _mm_max2, _mm_load_max2);
		MLIB_M_IMAGE_MINIMUM_U16(_mm_min2, _mm_min2, _mm_load_min2);
		sl += slb;
	}

	((__m64 *) temp_store)[0] = _mm_min0;
	((__m64 *) temp_store)[1] = _mm_min1;
	((__m64 *) temp_store)[2] = _mm_min2;

	{
		mlib_s32 min0 = temp_store[0];
		mlib_s32 min1 = temp_store[1];
		mlib_s32 min2 = temp_store[2];

		MIN_u16(min0, temp_store[3]);
		MIN_u16(min0, temp_store[6]);
		MIN_u16(min0, temp_store[9]);
		MIN_u16(min1, temp_store[4]);
		MIN_u16(min1, temp_store[7]);
		MIN_u16(min1, temp_store[10]);
		MIN_u16(min2, temp_store[5]);
		MIN_u16(min2, temp_store[8]);
		MIN_u16(min2, temp_store[11]);
		dmin[0] = min0;
		dmin[1] = min1;
		dmin[2] = min2;
	}

	((__m64 *) temp_store)[0] = _mm_max0;
	((__m64 *) temp_store)[1] = _mm_max1;
	((__m64 *) temp_store)[2] = _mm_max2;

	{
		mlib_s32 max0 = temp_store[0];
		mlib_s32 max1 = temp_store[1];
		mlib_s32 max2 = temp_store[2];

		MAX_u16(max0, temp_store[3]);
		MAX_u16(max0, temp_store[6]);
		MAX_u16(max0, temp_store[9]);
		MAX_u16(max1, temp_store[4]);
		MAX_u16(max1, temp_store[7]);
		MAX_u16(max1, temp_store[10]);
		MAX_u16(max2, temp_store[5]);
		MAX_u16(max2, temp_store[8]);
		MAX_u16(max2, temp_store[11]);
		dmax[0] = max0;
		dmax[1] = max1;
		dmax[2] = max2;
	}

	_mm_empty();
}

/* *********************************************************** */

void
mlib_m_ImageExtrema2_4_U16(
    mlib_s32 *dmin,
    mlib_s32 *dmax,
    mlib_u16 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 slb,
    mlib_s32 period)
{
	mlib_s32 min0, min1, min2, min3;
	mlib_s32 max0, max1, max2, max3;
	mlib_u16 temp_store[4];
	mlib_s32 j;
	__m64 _mm_load_min;
	__m64 _mm_load_max;
	__m64 _mm_min;
	__m64 _mm_max;
	__m64 _mm_mask_min;
	__m64 _mm_mask_max;
	__m64 _mm_mask_fin = _mm_mlib_setr_pi16(0xffff, 0x0000, 0xffff, 0x0000);

	_mm_min = _mm_set1_pi16(MLIB_U16_MAX);
	_mm_max = _mm_set1_pi16(MLIB_U16_MIN);

	for (j = 0; j < ysize; j++) {
		mlib_u16 *sp = sl;
		mlib_u16 *send = sl + xsize;

		for (; sp < (send - 3); sp += period) {
			_mm_load_min = _mm_load_max =
			    _mm_mlib_setr_pi16(sp[0], sp[1], sp[2], sp[3]);
			MLIB_M_IMAGE_MAXIMUM_U16(_mm_max, _mm_max,
			    _mm_load_max);
			MLIB_M_IMAGE_MINIMUM_U16(_mm_min, _mm_min,
			    _mm_load_max);
		}

		MLIB_M_IMAGE_MAXIMUM_U16(_mm_max, _mm_max, _mm_load_max);
		MLIB_M_IMAGE_MINIMUM_U16(_mm_min, _mm_min, _mm_load_min);
		sl += slb;
	}

	((__m64 *) temp_store)[0] = _mm_min;
	dmin[0] = temp_store[0];
	dmin[1] = temp_store[1];
	dmin[2] = temp_store[2];
	dmin[3] = temp_store[3];

	((__m64 *) temp_store)[0] = _mm_max;
	dmax[0] = temp_store[0];
	dmax[1] = temp_store[1];
	dmax[2] = temp_store[2];
	dmax[3] = temp_store[3];

	_mm_empty();
}

/* *********************************************************** */

#define	MLIB_M_IMAGE_MAXIMUM_S32(dd1, max, sd1)                 \
	{                                                       \
	    __m64 cmask = _mm_cmpgt_pi32(sd1, max);             \
	                                                        \
	    dd1 =                                               \
		_mm_or_si64(_mm_and_si64(sd1, cmask),           \
		_mm_andnot_si64(cmask, max));                   \
	}

/* *********************************************************** */

#define	MLIB_M_IMAGE_MINIMUM_S32(dd1, max, sd1)                 \
	{                                                       \
	    __m64 cmask = _mm_cmpgt_pi32(max, sd1);             \
	                                                        \
	    dd1 =                                               \
		_mm_or_si64(_mm_and_si64(sd1, cmask),           \
		_mm_andnot_si64(cmask, max));                   \
	}

/* *********************************************************** */

void
mlib_m_ImageExtrema2_1_S32(
    mlib_s32 *dmin,
    mlib_s32 *dmax,
    mlib_s32 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 slb,
    mlib_s32 off1,
    mlib_s32 off2,
    mlib_s32 off3,
    mlib_s32 off4)
{
	mlib_s32 min0, min1, min2, min3;
	mlib_s32 max0, max1, max2, max3;
	mlib_s32 j;
	mlib_s32 s0, s1, s2, s3;
	__m64 _mm_load_min_hi;
	__m64 _mm_load_min_lo;
	__m64 _mm_load_max_hi;
	__m64 _mm_load_max_lo;
	__m64 _mm_min_hi;
	__m64 _mm_min_lo;
	__m64 _mm_max_hi;
	__m64 _mm_max_lo;
	__m64 _mm_mask_min_hi;
	__m64 _mm_mask_min_lo;
	__m64 _mm_mask_max_hi;
	__m64 _mm_mask_max_lo;

	_mm_min_lo = _mm_min_hi = _mm_set1_pi32(MLIB_S32_MAX);
	_mm_max_lo = _mm_max_hi = _mm_set1_pi32(MLIB_S32_MIN);

	for (j = 0; j < ysize; j++) {
		mlib_s32 *sp = sl;
		mlib_s32 *send = sl + xsize;

		s0 = sp[0];
		s1 = sp[off1];
		s2 = sp[off2];
		s3 = sp[off3];

		for (; sp < (send - off3); sp += off4) {
			_mm_load_min_lo = _mm_load_max_lo =
			    _mm_mlib_setr_pi32(s0, s1);
			_mm_load_min_hi = _mm_load_max_hi =
			    _mm_mlib_setr_pi32(s2, s3);
			MLIB_M_IMAGE_MAXIMUM_S32(_mm_max_lo, _mm_max_lo,
			    _mm_load_max_lo);
			MLIB_M_IMAGE_MINIMUM_S32(_mm_min_lo, _mm_min_lo,
			    _mm_load_max_lo);
			MLIB_M_IMAGE_MAXIMUM_S32(_mm_max_hi, _mm_max_hi,
			    _mm_load_max_hi);
			MLIB_M_IMAGE_MINIMUM_S32(_mm_min_hi, _mm_min_hi,
			    _mm_load_max_hi);

			s0 = sp[0 + off4];
			s1 = sp[off1 + off4];
			s2 = sp[off2 + off4];
			s3 = sp[off3 + off4];
		}

		if ((sp + off2) < send) {
			_mm_load_min_lo = _mm_mlib_setr_pi32(s0, s1);
			_mm_load_min_hi = _mm_mlib_setr_pi32(s2, MLIB_S32_MAX);
			_mm_load_max_lo = _mm_mlib_setr_pi32(s0, s1);
			_mm_load_max_hi = _mm_mlib_setr_pi32(s2, MLIB_S32_MIN);
		} else if ((sp + off1) < send) {
			_mm_load_min_lo = _mm_mlib_setr_pi32(s0, s1);
			_mm_load_min_hi = _mm_set1_pi32(MLIB_S32_MAX);
			_mm_load_max_lo = _mm_mlib_setr_pi32(s0, s1);
			_mm_load_max_hi = _mm_set1_pi32(MLIB_S32_MIN);
		} else if ((sp) < send) {
			_mm_load_min_lo = _mm_mlib_setr_pi32(s0, MLIB_S32_MAX);
			_mm_load_min_hi = _mm_set1_pi32(MLIB_S32_MAX);
			_mm_load_max_lo = _mm_mlib_setr_pi32(s0, MLIB_S32_MIN);
			_mm_load_max_hi = _mm_set1_pi32(MLIB_S32_MIN);
		}

		MLIB_M_IMAGE_MAXIMUM_S32(_mm_max_lo, _mm_max_lo,
		    _mm_load_max_lo);
		MLIB_M_IMAGE_MINIMUM_S32(_mm_min_lo, _mm_min_lo,
		    _mm_load_min_lo);
		MLIB_M_IMAGE_MAXIMUM_S32(_mm_max_hi, _mm_max_hi,
		    _mm_load_max_hi);
		MLIB_M_IMAGE_MINIMUM_S32(_mm_min_hi, _mm_min_hi,
		    _mm_load_min_hi);
		sl += slb;
	}

	min0 = _mm_cvtsi64_si32(_mm_min_lo);
	min1 = _mm_cvtsi64_si32(_mm_srli_si64(_mm_min_lo, 32));
	min2 = _mm_cvtsi64_si32(_mm_min_hi);
	min3 = _mm_cvtsi64_si32(_mm_srli_si64(_mm_min_hi, 32));

	max0 = _mm_cvtsi64_si32(_mm_max_lo);
	max1 = _mm_cvtsi64_si32(_mm_srli_si64(_mm_max_lo, 32));
	max2 = _mm_cvtsi64_si32(_mm_max_hi);
	max3 = _mm_cvtsi64_si32(_mm_srli_si64(_mm_max_hi, 32));

	_mm_empty();

	dmin[0] = min0;
	dmax[0] = max0;
	dmin[1] = min1;
	dmax[1] = max1;
	dmin[2] = min2;
	dmax[2] = max2;
	dmin[3] = min3;
	dmax[3] = max3;
}

/* *********************************************************** */

void
mlib_m_ImageExtrema2_3_S32(
    mlib_s32 *dmin,
    mlib_s32 *dmax,
    mlib_s32 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 slb,
    mlib_s32 period)
{
	mlib_s32 j;
	mlib_s32 a = period;
	mlib_s32 b = period * 2;
	mlib_s32 c = period * 3;
	mlib_s32 d = period * 4;
	mlib_s32 temp_store[6];
	__m64 _mm_load_min0, _mm_load_min1, _mm_load_min2;
	__m64 _mm_load_max0, _mm_load_max1, _mm_load_max2;
	__m64 _mm_min0, _mm_min1, _mm_min2;
	__m64 _mm_max0, _mm_max1, _mm_max2;

	_mm_min0 = _mm_min1 = _mm_min2 = _mm_set1_pi32(MLIB_S32_MAX);
	_mm_max0 = _mm_max1 = _mm_max2 = _mm_set1_pi32(MLIB_S32_MIN);

	for (j = 0; j < ysize; j++) {
		mlib_s32 *sp = sl;
		mlib_s32 *loopend = sl + xsize;

		for (; sp < (loopend - a); sp += b) {
			_mm_load_min0 = _mm_load_max0 =
			    _mm_mlib_setr_pi32(sp[0], sp[1]);
			_mm_load_min1 = _mm_load_max1 =
			    _mm_mlib_setr_pi32(sp[2], sp[a + 0]);
			_mm_load_min2 = _mm_load_max2 =
			    _mm_mlib_setr_pi32(sp[a + 1], sp[a + 2]);
			MLIB_M_IMAGE_MAXIMUM_S32(_mm_max0, _mm_max0,
			    _mm_load_max0);
			MLIB_M_IMAGE_MINIMUM_S32(_mm_min0, _mm_min0,
			    _mm_load_max0);
			MLIB_M_IMAGE_MAXIMUM_S32(_mm_max1, _mm_max1,
			    _mm_load_max1);
			MLIB_M_IMAGE_MINIMUM_S32(_mm_min1, _mm_min1,
			    _mm_load_max1);
			MLIB_M_IMAGE_MAXIMUM_S32(_mm_max2, _mm_max2,
			    _mm_load_max2);
			MLIB_M_IMAGE_MINIMUM_S32(_mm_min2, _mm_min2,
			    _mm_load_max2);
		}

		if ((sp) < loopend) {
			_mm_load_min0 = _mm_mlib_setr_pi32(sp[0], sp[1]);
			_mm_load_max0 = _mm_mlib_setr_pi32(sp[0], sp[1]);
			_mm_load_min1 = _mm_mlib_setr_pi32(sp[2], MLIB_S32_MAX);
			_mm_load_max1 = _mm_mlib_setr_pi32(sp[2], MLIB_S32_MIN);
			_mm_load_min2 = _mm_set1_pi32(MLIB_S32_MAX);
			_mm_load_max2 = _mm_set1_pi32(MLIB_S32_MIN);
		}

		MLIB_M_IMAGE_MAXIMUM_S32(_mm_max0, _mm_max0, _mm_load_max0);
		MLIB_M_IMAGE_MINIMUM_S32(_mm_min0, _mm_min0, _mm_load_min0);
		MLIB_M_IMAGE_MAXIMUM_S32(_mm_max1, _mm_max1, _mm_load_max1);
		MLIB_M_IMAGE_MINIMUM_S32(_mm_min1, _mm_min1, _mm_load_min1);
		MLIB_M_IMAGE_MAXIMUM_S32(_mm_max2, _mm_max2, _mm_load_max2);
		MLIB_M_IMAGE_MINIMUM_S32(_mm_min2, _mm_min2, _mm_load_min2);
		sl += slb;
	}

	((__m64 *) temp_store)[0] = _mm_min0;
	((__m64 *) temp_store)[1] = _mm_min1;
	((__m64 *) temp_store)[2] = _mm_min2;

	{
		mlib_s32 min0 = temp_store[0];
		mlib_s32 min1 = temp_store[1];
		mlib_s32 min2 = temp_store[2];

		MIN_s32(min0, temp_store[3]);
		MIN_s32(min1, temp_store[4]);
		MIN_s32(min2, temp_store[5]);
		dmin[0] = min0;
		dmin[1] = min1;
		dmin[2] = min2;
	}

	((__m64 *) temp_store)[0] = _mm_max0;
	((__m64 *) temp_store)[1] = _mm_max1;
	((__m64 *) temp_store)[2] = _mm_max2;

	{
		mlib_s32 max0 = temp_store[0];
		mlib_s32 max1 = temp_store[1];
		mlib_s32 max2 = temp_store[2];

		MAX_s32(max0, temp_store[3]);
		MAX_s32(max1, temp_store[4]);
		MAX_s32(max2, temp_store[5]);
		dmax[0] = max0;
		dmax[1] = max1;
		dmax[2] = max2;
	}

	_mm_empty();
}

/* *********************************************************** */

mlib_status
__mlib_ImageExtrema2(
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
	else if ((type == MLIB_SHORT) || (type == MLIB_USHORT))
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
			mlib_m_ImageExtrema2_1_U8(dmin, dmax, (mlib_u8 *)sl,
			    xsize, ysize, slb, xPeriod, 2 * xPeriod,
			    3 * xPeriod, 4 * xPeriod);
			break;

		case 2:
			mlib_m_ImageExtrema2_2_U8(min, max, (mlib_u8 *)sl,
			    xsize, ysize, slb, 1, 2 * xPeriod, 2 * xPeriod + 1,
			    4 * xPeriod);
			break;

		case 3:
			mlib_m_ImageExtrema2_3_U8(min, max, (mlib_u8 *)sl,
			    xsize, ysize, slb, 3 * xPeriod);
			break;

		case 4:
			mlib_m_ImageExtrema2_4_U8(min, max, (mlib_u8 *)sl,
			    xsize, ysize, slb, 4 * xPeriod);
			break;

		default:
			return (MLIB_FAILURE);
		}

	} else if (type == MLIB_SHORT) {

		dmin[0] = min[0] = MLIB_S16_MAX;
		dmax[0] = max[0] = MLIB_S16_MIN;

		switch (nchan) {
		case 1:
			mlib_m_ImageExtrema2_1_S16(dmin, dmax, (mlib_s16 *)sl,
			    xsize, ysize, slb, xPeriod, 2 * xPeriod,
			    3 * xPeriod, 4 * xPeriod);
			break;

		case 2:
			mlib_m_ImageExtrema2_2_S16(min, max, (mlib_s16 *)sl,
			    xsize, ysize, slb, 1, 2 * xPeriod, 2 * xPeriod + 1,
			    4 * xPeriod);
			break;

		case 3:
			mlib_m_ImageExtrema2_3_S16(min, max, (mlib_s16 *)sl,
			    xsize, ysize, slb, 3 * xPeriod);
			break;

		case 4:
			mlib_m_ImageExtrema2_4_S16(min, max, (mlib_s16 *)sl,
			    xsize, ysize, slb, 4 * xPeriod);
			break;

		default:
			return (MLIB_FAILURE);
		}

	} else if (type == MLIB_USHORT) {

		dmin[0] = min[0] = MLIB_U16_MAX;
		dmax[0] = max[0] = MLIB_U16_MIN;

		switch (nchan) {
		case 1:
			mlib_m_ImageExtrema2_1_U16(dmin, dmax, (mlib_u16 *)sl,
			    xsize, ysize, slb, xPeriod, 2 * xPeriod,
			    3 * xPeriod, 4 * xPeriod);
			break;

		case 2:
			mlib_m_ImageExtrema2_2_U16(min, max, (mlib_u16 *)sl,
			    xsize, ysize, slb, 1, 2 * xPeriod, 2 * xPeriod + 1,
			    4 * xPeriod);
			break;

		case 3:
			mlib_m_ImageExtrema2_3_U16(min, max, (mlib_u16 *)sl,
			    xsize, ysize, slb, 3 * xPeriod);
			break;

		case 4:
			mlib_m_ImageExtrema2_4_U16(min, max, (mlib_u16 *)sl,
			    xsize, ysize, slb, 4 * xPeriod);
			break;

		default:
			return (MLIB_FAILURE);
		}

	} else if (type == MLIB_INT) {

		dmin[0] = min[0] = MLIB_S32_MAX;
		dmax[0] = max[0] = MLIB_S32_MIN;

		switch (nchan) {
		case 1:
			mlib_m_ImageExtrema2_1_S32(dmin, dmax, (mlib_s32 *)sl,
			    xsize, ysize, slb, xPeriod, 2 * xPeriod,
			    3 * xPeriod, 4 * xPeriod);
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
			mlib_m_ImageExtrema2_1_S32(dmin, dmax, (mlib_s32 *)sl,
			    xsize, ysize, slb, 1, 2 * xPeriod, 2 * xPeriod + 1,
			    4 * xPeriod);
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
			mlib_m_ImageExtrema2_3_S32(min, max, (mlib_s32 *)sl,
			    xsize, ysize, slb, 3 * xPeriod);
			return (MLIB_SUCCESS);

		case 4:
			mlib_m_ImageExtrema2_1_S32(min, max, (mlib_s32 *)sl,
			    xsize, ysize, slb, 1, 2, 3, 4 * xPeriod);
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
