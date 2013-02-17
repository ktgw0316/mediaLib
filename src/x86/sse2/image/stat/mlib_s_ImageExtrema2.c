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

#pragma ident	"@(#)mlib_s_ImageExtrema2.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageExtrema2_fast  - no fast VIS version
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

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

#define	MLIB_MIN(min, x)	(min < x) ? min : x
#define	MLIB_MAX(max, x)	(max > x) ? max : x

#define	MLIB_MIN_u16(min, x)	min += ((x - min) & ((x - min) >> 31))
#define	MLIB_MAX_u16(max, x)	max -= (max - x) & ((max - x) >> 31)

#define	MIN_u8(min, x)	min += ((x - min) & ((x - min) >> 31))
#define	MIN_u16(min, x)	min += ((x - min) & ((x - min) >> 31))
#define	MIN_s16(min, x)	min += ((x - min) & ((x - min) >> 31))
#define	MIN_s32(min, x)	MIN(min, x)

#define	MAX_u8(max, x)	max -= (max - x) & ((max - x) >> 31)
#define	MAX_u16(max, x)	max -= (max - x) & ((max - x) >> 31)
#define	MAX_s16(max, x)	max -= (max - x) & ((max - x) >> 31)
#define	MAX_s32(max, x)	MAX(max, x)

/* *********************************************************** */

#define	MLIB_S_IMAGE_MAXIMUM_S16(dd1, max, sd1)                 \
	{                                                       \
	    dd1 = _mm_max_epi16(max, sd1); 			\
	}

/* *********************************************************** */

#define	MLIB_S_IMAGE_MINIMUM_S16(dd1, min, sd1)                 \
	{                                                       \
	    dd1 = _mm_min_epi16(min, sd1); 			\
	}

/* *********************************************************** */

void
mlib_s_ImageExtrema2_1_U8(
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
	__m128i _mm_load_min;
	__m128i _mm_load_max;
	__m128i _mm_min;
	__m128i _mm_max;
	mlib_s32 j, k;
	mlib_s32 min0, min1;
	mlib_s32 max0, max1, max_l, max_h;
	mlib_s32 a = off1;
	mlib_s32 b = 2 * off1;
	mlib_s32 c = 3 * off1;
	mlib_s32 d = 4 * off1;
	mlib_s32 e = 5 * off1;
	mlib_s32 f = 6 * off1;
	mlib_s32 g = 7 * off1;
	mlib_s32 h = 8 * off1;

	_mm_min = _mm_set1_epi16(MLIB_U8_MAX);
	_mm_max = _mm_set1_epi16(MLIB_U8_MIN);

	min0 = min1 = dmin[0];
	max0 = max1 = dmax[0];

	for (j = 0; j < ysize; j++) {
		mlib_u8 *sp = sl;
		mlib_u8 *send = sl + xsize;

		for (k = 0; sp < (send - g); k++, sp += h) {
			_mm_load_min = _mm_load_max =
			    _mm_setr_epi16(sp[0], sp[a], sp[b], sp[c],
			    sp[d], sp[e], sp[f], sp[g]);

			MLIB_S_IMAGE_MAXIMUM_S16(_mm_max, _mm_max,
			    _mm_load_max);
			MLIB_S_IMAGE_MINIMUM_S16(_mm_min, _mm_min,
			    _mm_load_min);
		}

		mlib_s16 stmax[7], stmin[7];
		for (k = 0; k < 7; k++) {
			stmin[k] = MLIB_U8_MAX;
			stmax[k] = MLIB_U8_MIN;
		}

		for (k = 0; sp < send; k++, sp += off1) {
			stmin[k] = stmax[k] = *sp;
		}

		_mm_load_min =
		    _mm_setr_epi16(stmin[0], stmin[1], stmin[2], stmin[3],
		    stmin[4], stmin[5], stmin[6], MLIB_U8_MAX);

		_mm_load_max =
		    _mm_setr_epi16(stmax[0], stmax[1], stmax[2], stmax[3],
		    stmax[4], stmax[5], stmax[6], MLIB_U8_MIN);

		MLIB_S_IMAGE_MAXIMUM_S16(_mm_max, _mm_max, _mm_load_max);
		MLIB_S_IMAGE_MINIMUM_S16(_mm_min, _mm_min, _mm_load_min);
		sl += slb;
	}

	__m128i s128max, s128min;
	mlib_s16 *s16max = (mlib_s16 *)&s128max;
	mlib_s16 *s16min = (mlib_s16 *)&s128min;
	_mm_store_si128((void *)s16max, _mm_max);
	_mm_store_si128((void *)s16min, _mm_min);

	for (k = 0; k < 8; k += 2) {
		max0 = MLIB_MAX(max0, s16max[k]);
		max1 = MLIB_MAX(max1, s16max[k + 1]);
		min0 = MLIB_MIN(min0, s16min[k]);
		min1 = MLIB_MIN(min1, s16min[k + 1]);
	}

	dmin[0] = min0;
	dmax[0] = max0;
	dmin[1] = min1;
	dmax[1] = max1;
}

/* *********************************************************** */

void
mlib_s_ImageExtrema2_2_U8(
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
	__m128i _mm_load_min;
	__m128i _mm_load_max;
	__m128i _mm_min;
	__m128i _mm_max;
	mlib_s32 j, k;
	mlib_s32 min0, min1;
	mlib_s32 max0, max1;

	_mm_min = _mm_set1_epi16(MLIB_U8_MAX);
	_mm_max = _mm_set1_epi16(MLIB_U8_MIN);

	min0 = min1 = dmin[0];
	max0 = max1 = dmax[0];

	for (j = 0; j < ysize; j++) {
		mlib_u8 *sp = sl;
		mlib_u8 *send = sl + xsize;

		for (; sp < (send - 3 * off2); sp += 4 * off2) {
			_mm_load_min = _mm_load_max =
			    _mm_setr_epi16(sp[0], sp[1], sp[off2], sp[off2 + 1],
			    sp[2 * off2], sp[2 * off2 + 1], sp[3 * off2],
			    sp[3 * off2 + 1]);

			MLIB_S_IMAGE_MAXIMUM_S16(_mm_max, _mm_max,
			    _mm_load_max);
			MLIB_S_IMAGE_MINIMUM_S16(_mm_min, _mm_min,
			    _mm_load_max);
		}

		mlib_s16 stmax[6], stmin[6];
		for (k = 0; k < 6; k++) {
			stmin[k] = MLIB_U8_MAX;
			stmax[k] = MLIB_U8_MIN;
		}

		for (k = 0; sp < send; k += 2, sp += off2) {
			stmin[k] = stmax[k] = *sp;
			stmin[k + 1] = stmax[k + 1] = *(sp + 1);
		}

		_mm_load_min =
		    _mm_setr_epi16(stmin[0], stmin[1], stmin[2], stmin[3],
		    stmin[4], stmin[5], MLIB_U8_MAX, MLIB_U8_MAX);

		_mm_load_max =
		    _mm_setr_epi16(stmax[0], stmax[1], stmax[2], stmax[3],
		    stmax[4], stmax[5], MLIB_U8_MIN, MLIB_U8_MIN);

		MLIB_S_IMAGE_MAXIMUM_S16(_mm_max, _mm_max, _mm_load_max);
		MLIB_S_IMAGE_MINIMUM_S16(_mm_min, _mm_min, _mm_load_min);
		sl += slb;
	}

	__m128i s128max, s128min;
	mlib_s16 *s16max = (mlib_s16 *)&s128max;
	mlib_s16 *s16min = (mlib_s16 *)&s128min;
	_mm_store_si128((void *)s16max, _mm_max);
	_mm_store_si128((void *)s16min, _mm_min);

	for (k = 0; k < 8; k += 2) {
		max0 = MLIB_MAX(max0, s16max[k]);
		max1 = MLIB_MAX(max1, s16max[k + 1]);
		min0 = MLIB_MIN(min0, s16min[k]);
		min1 = MLIB_MIN(min1, s16min[k + 1]);
	}

	dmin[0] = min0;
	dmax[0] = max0;
	dmin[1] = min1;
	dmax[1] = max1;
}

/* *********************************************************** */

void
mlib_s_ImageExtrema2_4_U8(
    mlib_s32 *dmin,
    mlib_s32 *dmax,
    mlib_u8 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 slb,
    mlib_s32 off4)
{
	__m128i _mm_load_min;
	__m128i _mm_load_max;
	__m128i _mm_min;
	__m128i _mm_max;
	mlib_s32 j, k;
	mlib_s32 min0, min1, min2, min3;
	mlib_s32 max0, max1, max2, max3;

	_mm_min = _mm_set1_epi16(MLIB_U8_MAX);
	_mm_max = _mm_set1_epi16(MLIB_U8_MIN);

	min0 = min1 = min2 = min3 = dmin[0];
	max0 = max1 = max2 = max3 = dmax[0];

	for (j = 0; j < ysize; j++) {
		mlib_u8 *sp = sl;
		mlib_u8 *send = sl + xsize;

		for (; sp < (send - off4); sp += 2 * off4) {
			_mm_load_min = _mm_load_max =
			    _mm_setr_epi16(sp[0], sp[1], sp[2], sp[3],
			    sp[off4], sp[off4 + 1], sp[off4 + 2], sp[off4 + 3]);

			MLIB_S_IMAGE_MAXIMUM_S16(_mm_max, _mm_max,
			    _mm_load_max);
			MLIB_S_IMAGE_MINIMUM_S16(_mm_min, _mm_min,
			    _mm_load_max);
		}

		mlib_s16 stmax[4], stmin[4];
		for (k = 0; k < 4; k++) {
			stmin[k] = MLIB_U8_MAX;
			stmax[k] = MLIB_U8_MIN;
		}

		for (k = 0; sp < send; k += 4, sp += off4) {
			stmin[k] = stmax[k] = *sp;
			stmin[k + 1] = stmax[k + 1] = *(sp + 1);
			stmin[k + 2] = stmax[k + 2] = *(sp + 2);
			stmin[k + 3] = stmax[k + 3] = *(sp + 3);
		}

		_mm_load_min =
		    _mm_setr_epi16(stmin[0], stmin[1], stmin[2], stmin[3],
		    MLIB_U8_MAX, MLIB_U8_MAX, MLIB_U8_MAX, MLIB_U8_MAX);

		_mm_load_max =
		    _mm_setr_epi16(stmax[0], stmax[1], stmax[2], stmax[3],
		    MLIB_U8_MIN, MLIB_U8_MIN, MLIB_U8_MIN, MLIB_U8_MIN);

		MLIB_S_IMAGE_MAXIMUM_S16(_mm_max, _mm_max, _mm_load_max);
		MLIB_S_IMAGE_MINIMUM_S16(_mm_min, _mm_min, _mm_load_min);
		sl += slb;
	}

	__m128i s128max, s128min;
	mlib_s16 *s16max = (mlib_s16 *)&s128max;
	mlib_s16 *s16min = (mlib_s16 *)&s128min;
	_mm_store_si128((void *)s16max, _mm_max);
	_mm_store_si128((void *)s16min, _mm_min);

	for (k = 0; k < 8; k += 4) {
		max0 = MLIB_MAX(max0, s16max[k]);
		max1 = MLIB_MAX(max1, s16max[k + 1]);
		max2 = MLIB_MAX(max2, s16max[k + 2]);
		max3 = MLIB_MAX(max3, s16max[k + 3]);

		min0 = MLIB_MIN(min0, s16min[k]);
		min1 = MLIB_MIN(min1, s16min[k + 1]);
		min2 = MLIB_MIN(min2, s16min[k + 2]);
		min3 = MLIB_MIN(min3, s16min[k + 3]);
	}

	dmin[0] = min0;
	dmin[1] = min1;
	dmin[2] = min2;
	dmin[3] = min3;
	dmax[0] = max0;
	dmax[1] = max1;
	dmax[2] = max2;
	dmax[3] = max3;
}

/* *********************************************************** */

void
mlib_s_ImageExtrema2_3_U8(
    mlib_s32 *dmin,
    mlib_s32 *dmax,
    mlib_u8 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 slb,
    mlib_s32 period)
{
	__m128i _mm_load_min0, _mm_load_min1, _mm_load_min2;
	__m128i _mm_load_max0, _mm_load_max1, _mm_load_max2;
	__m128i _mm_min0, _mm_min1, _mm_min2;
	__m128i _mm_max0, _mm_max1, _mm_max2;
	__m128i _mm_mask;
	mlib_s32 min0, min1, min2, min3;
	mlib_s32 max0, max1, max2, max3;
	mlib_s32 j, k;
	mlib_s32 a = period;
	mlib_s32 b = period * 2;
	mlib_s32 c = period * 3;
	mlib_s32 d = period * 4;
	mlib_s32 e = period * 5;
	mlib_s32 f = period * 6;
	mlib_s32 g = period * 7;
	mlib_s32 h = period * 8;

	mlib_s16 temp_store[24];

	_mm_min0 = _mm_min1 = _mm_min2 = _mm_set1_epi16(MLIB_U8_MAX);
	_mm_max0 = _mm_max1 = _mm_max2 = _mm_set1_epi16(MLIB_U8_MIN);

	min0 = min1 = min2 = min3 = dmin[0];
	max0 = max1 = max2 = max3 = dmax[0];

	for (j = 0; j < ysize; j++) {
		mlib_u8 *sp = sl;
		mlib_u8 *loopend = sl + xsize;

		for (; sp < (loopend - g); sp += h) {
			_mm_load_min0 = _mm_load_max0 = _mm_setr_epi16(
			    sp[0], sp[1], sp[2], sp[a + 0], sp[a + 1],
			    sp[a + 2], sp[b + 0], sp[b + 1]);
			_mm_load_min1 = _mm_load_max1 = _mm_setr_epi16(
				sp[b + 2], sp[c + 0], sp[c + 1], sp[c + 2],
			    sp[d + 0], sp[d + 1], sp[d + 2], sp[e + 0]);
			_mm_load_min2 = _mm_load_max2 = _mm_setr_epi16(
			    sp[e + 1], sp[e + 2], sp[f + 0], sp[f + 1],
			    sp[f + 2], sp[g + 0], sp[g + 1], sp[g + 2]);

			MLIB_S_IMAGE_MAXIMUM_S16(_mm_max0, _mm_max0,
			    _mm_load_max0);
			MLIB_S_IMAGE_MINIMUM_S16(_mm_min0, _mm_min0,
			    _mm_load_max0);
			MLIB_S_IMAGE_MAXIMUM_S16(_mm_max1, _mm_max1,
			    _mm_load_max1);
			MLIB_S_IMAGE_MINIMUM_S16(_mm_min1, _mm_min1,
			    _mm_load_max1);
			MLIB_S_IMAGE_MAXIMUM_S16(_mm_max2, _mm_max2,
			    _mm_load_max2);
			MLIB_S_IMAGE_MINIMUM_S16(_mm_min2, _mm_min2,
			    _mm_load_max2);
		}

		mlib_s16 stmax[21], stmin[21];
		for (k = 0; k < 21; k++) {
			stmin[k] = MLIB_U8_MAX;
			stmax[k] = MLIB_U8_MIN;
		}

		for (k = 0; sp < loopend; k += 3, sp += period) {
			stmin[k] = stmax[k] = *sp;
			stmin[k + 1] = stmax[k + 1] = *(sp + 1);
			stmin[k + 2] = stmax[k + 2] = *(sp + 2);
		}

		_mm_load_min0 =
		    _mm_setr_epi16(stmin[0], stmin[1], stmin[2], stmin[3],
		    stmin[4], stmin[5], stmin[6], stmin[7]);
		_mm_load_min1 =
		    _mm_setr_epi16(stmin[8], stmin[9], stmin[10], stmin[11],
		    stmin[12], stmin[13], stmin[14], stmin[15]);
		_mm_load_min2 =
		    _mm_setr_epi16(stmin[16], stmin[17], stmin[18], stmin[19],
		    stmin[20], MLIB_U8_MAX, MLIB_U8_MAX, MLIB_U8_MAX);

		_mm_load_max0 =
		    _mm_setr_epi16(stmax[0], stmax[1], stmax[2], stmax[3],
		    stmax[4], stmax[5], stmax[6], stmax[7]);
		_mm_load_max1 =
		    _mm_setr_epi16(stmax[8], stmax[9], stmax[10], stmax[11],
		    stmax[12], stmax[13], stmax[14], stmax[15]);
		_mm_load_max2 =
		    _mm_setr_epi16(stmax[16], stmax[17], stmax[18], stmax[19],
		    stmax[20], MLIB_U8_MIN, MLIB_U8_MIN, MLIB_U8_MIN);

		MLIB_S_IMAGE_MAXIMUM_S16(_mm_max0, _mm_max0, _mm_load_max0);
		MLIB_S_IMAGE_MINIMUM_S16(_mm_min0, _mm_min0, _mm_load_min0);
		MLIB_S_IMAGE_MAXIMUM_S16(_mm_max1, _mm_max1, _mm_load_max1);
		MLIB_S_IMAGE_MINIMUM_S16(_mm_min1, _mm_min1, _mm_load_min1);
		MLIB_S_IMAGE_MAXIMUM_S16(_mm_max2, _mm_max2, _mm_load_max2);
		MLIB_S_IMAGE_MINIMUM_S16(_mm_min2, _mm_min2, _mm_load_min2);
		sl += slb;
	}

	mlib_s16 s16max0[8], s16max1[8], s16max2[8];
	mlib_s16 s16min0[8], s16min1[8], s16min2[8];
	_mm_storeu_si128((void *)s16max0, _mm_max0);
	_mm_storeu_si128((void *)s16max1, _mm_max1);
	_mm_storeu_si128((void *)s16max2, _mm_max2);
	_mm_storeu_si128((void *)s16min0, _mm_min0);
	_mm_storeu_si128((void *)s16min1, _mm_min1);
	_mm_storeu_si128((void *)s16min2, _mm_min2);

/* zero channel */
	min0 = MLIB_MIN(s16min0[0], s16min0[3]);
	min1 = MLIB_MIN(s16min0[6], s16min1[1]);
	min2 = MLIB_MIN(s16min1[4], s16min1[7]);
	min3 = MLIB_MIN(s16min2[2], s16min2[5]);
	min0 = MLIB_MIN(min0, min1);
	min2 = MLIB_MIN(min2, min3);
	min0 = MLIB_MIN(min0, min2);

	max0 = MLIB_MAX(s16max0[0], s16max0[3]);
	max1 = MLIB_MAX(s16max0[6], s16max1[1]);
	max2 = MLIB_MAX(s16max1[4], s16max1[7]);
	max3 = MLIB_MAX(s16max2[2], s16max2[5]);
	max0 = MLIB_MAX(max0, max1);
	max2 = MLIB_MAX(max2, max3);
	max0 = MLIB_MAX(max0, max2);
	dmin[0] = min0;
	dmax[0] = max0;

/* first channel */
	min0 = MLIB_MIN(s16min0[1], s16min0[4]);
	min1 = MLIB_MIN(s16min0[7], s16min1[2]);
	min2 = MLIB_MIN(s16min1[5], s16min2[0]);
	min3 = MLIB_MIN(s16min2[3], s16min2[6]);
	min0 = MLIB_MIN(min0, min1);
	min2 = MLIB_MIN(min2, min3);
	min0 = MLIB_MIN(min0, min2);

	max0 = MLIB_MAX(s16max0[1], s16max0[4]);
	max1 = MLIB_MAX(s16max0[7], s16max1[2]);
	max2 = MLIB_MAX(s16max1[5], s16max2[0]);
	max3 = MLIB_MAX(s16max2[3], s16max2[6]);
	max0 = MLIB_MAX(max0, max1);
	max2 = MLIB_MAX(max2, max3);
	max0 = MLIB_MAX(max0, max2);
	dmin[1] = min0;
	dmax[1] = max0;

/* second channel */
	min0 = MLIB_MIN(s16min0[2], s16min0[5]);
	min1 = MLIB_MIN(s16min1[0], s16min1[3]);
	min2 = MLIB_MIN(s16min1[6], s16min2[1]);
	min3 = MLIB_MIN(s16min2[4], s16min2[7]);
	min0 = MLIB_MIN(min0, min1);
	min2 = MLIB_MIN(min2, min3);
	min0 = MLIB_MIN(min0, min2);

	max0 = MLIB_MAX(s16max0[2], s16max0[5]);
	max1 = MLIB_MAX(s16max1[0], s16max1[3]);
	max2 = MLIB_MAX(s16max1[6], s16max2[1]);
	max3 = MLIB_MAX(s16max2[4], s16max2[7]);
	max0 = MLIB_MAX(max0, max1);
	max2 = MLIB_MAX(max2, max3);
	max0 = MLIB_MAX(max0, max2);
	dmin[2] = min0;
	dmax[2] = max0;
}

/* *********************************************************** */

void
mlib_s_ImageExtrema2_1_S16(
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
	__m128i _mm_load_min;
	__m128i _mm_load_max;
	__m128i _mm_min;
	__m128i _mm_max;
	mlib_s32 j, k;
	mlib_s32 min0, min1;
	mlib_s32 max0, max1;
	mlib_s32 a = off1;
	mlib_s32 b = 2 * off1;
	mlib_s32 c = 3 * off1;
	mlib_s32 d = 4 * off1;
	mlib_s32 e = 5 * off1;
	mlib_s32 f = 6 * off1;
	mlib_s32 g = 7 * off1;
	mlib_s32 h = 8 * off1;

	_mm_min = _mm_set1_epi16(MLIB_S16_MAX);
	_mm_max = _mm_set1_epi16(MLIB_S16_MIN);

	min0 = min1 = dmin[0];
	max0 = max1 = dmax[0];

	const __m128i x_zero = _mm_setzero_si128();

	for (j = 0; j < ysize; j++) {
		mlib_s16 *sp = sl;
		mlib_s16 *send = sl + xsize;

		for (k = 0; sp < (send - g); k++, sp += h) {
			_mm_load_min = _mm_load_max =
			    _mm_setr_epi16(sp[0], sp[a], sp[b], sp[c],
				    sp[d], sp[e], sp[f], sp[g]);

			MLIB_S_IMAGE_MAXIMUM_S16(_mm_max, _mm_max,
			    _mm_load_max);
			MLIB_S_IMAGE_MINIMUM_S16(_mm_min, _mm_min,
			    _mm_load_min);
		}

		mlib_s16 stmax[7], stmin[7];
		for (k = 0; k < 7; k++) {
			stmin[k] = MLIB_S16_MAX;
			stmax[k] = MLIB_S16_MIN;
		}

		for (k = 0; sp < send; k++, sp += off1) {
			stmin[k] = stmax[k] = *sp;
		}

		_mm_load_min =
		    _mm_setr_epi16(stmin[0], stmin[1], stmin[2], stmin[3],
		    stmin[4], stmin[5], stmin[6], MLIB_S16_MAX);

		_mm_load_max =
		    _mm_setr_epi16(stmax[0], stmax[1], stmax[2], stmax[3],
		    stmax[4], stmax[5], stmax[6], MLIB_S16_MIN);

		MLIB_S_IMAGE_MAXIMUM_S16(_mm_max, _mm_max, _mm_load_max);
		MLIB_S_IMAGE_MINIMUM_S16(_mm_min, _mm_min, _mm_load_min);
		sl += slb;
	}

	__m128i s128max, s128min;
	mlib_s16 *s16max = (mlib_s16 *)&s128max;
	mlib_s16 *s16min = (mlib_s16 *)&s128min;
	_mm_store_si128((void *)s16max, _mm_max);
	_mm_store_si128((void *)s16min, _mm_min);

		for (k = 0; k < 8; k += 2) {
			max0 = MLIB_MAX(max0, s16max[k]);
			max1 = MLIB_MAX(max1, s16max[k + 1]);
			min0 = MLIB_MIN(min0, s16min[k]);
			min1 = MLIB_MIN(min1, s16min[k + 1]);
		}

	dmin[0] = min0;
	dmax[0] = max0;
	dmin[1] = min1;
	dmax[1] = max1;
}

/* *********************************************************** */

void
mlib_s_ImageExtrema2_2_S16(
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
	__m128i _mm_load_min;
	__m128i _mm_load_max;
	__m128i _mm_min;
	__m128i _mm_max;
	mlib_s32 j, k;
	mlib_s32 min0, min1;
	mlib_s32 max0, max1;

	_mm_min = _mm_set1_epi16(MLIB_S16_MAX);
	_mm_max = _mm_set1_epi16(MLIB_S16_MIN);

	min0 = min1 = dmin[0];
	max0 = max1 = dmax[0];

	const __m128i x_zero = _mm_setzero_si128();

	for (j = 0; j < ysize; j++) {
		mlib_s16 *sp = sl;
		mlib_s16 *send = sl + xsize;

		for (; sp < (send - 3 * off2); sp += 4 * off2) {
			_mm_load_min = _mm_load_max =
			    _mm_setr_epi16(sp[0], sp[1], sp[off2], sp[off2 + 1],
			    sp[2 * off2], sp[2 * off2 + 1], sp[3 * off2],
			    sp[3 * off2 + 1]);

			MLIB_S_IMAGE_MAXIMUM_S16(_mm_max, _mm_max,
			    _mm_load_max);
			MLIB_S_IMAGE_MINIMUM_S16(_mm_min, _mm_min,
			    _mm_load_min);
		}

		mlib_s16 stmax[6], stmin[6];
		for (k = 0; k < 6; k++) {
			stmin[k] = MLIB_S16_MAX;
			stmax[k] = MLIB_S16_MIN;
		}

		for (k = 0; sp < send; k += 2, sp += off2) {
			stmin[k] = stmax[k] = *sp;
			stmin[k + 1] = stmax[k + 1] = *(sp + 1);
		}

		_mm_load_min =
		    _mm_setr_epi16(stmin[0], stmin[1], stmin[2], stmin[3],
		    stmin[4], stmin[5], MLIB_S16_MAX, MLIB_S16_MAX);

		_mm_load_max =
		    _mm_setr_epi16(stmax[0], stmax[1], stmax[2], stmax[3],
		    stmax[4], stmax[5], MLIB_S16_MIN, MLIB_S16_MIN);

		MLIB_S_IMAGE_MAXIMUM_S16(_mm_max, _mm_max, _mm_load_max);
		MLIB_S_IMAGE_MINIMUM_S16(_mm_min, _mm_min, _mm_load_min);
		sl += slb;
	}

	__m128i s128max, s128min;
	mlib_s16 *s16max = (mlib_s16 *)&s128max;
	mlib_s16 *s16min = (mlib_s16 *)&s128min;
	_mm_store_si128((void *)s16max, _mm_max);
	_mm_store_si128((void *)s16min, _mm_min);

	for (k = 0; k < 8; k += 2) {
		max0 = MLIB_MAX(max0, s16max[k]);
		max1 = MLIB_MAX(max1, s16max[k + 1]);
		min0 = MLIB_MIN(min0, s16min[k]);
		min1 = MLIB_MIN(min1, s16min[k + 1]);
	}

	dmin[0] = min0;
	dmax[0] = max0;
	dmin[1] = min1;
	dmax[1] = max1;
}

/* *********************************************************** */

void
mlib_s_ImageExtrema2_3_S16(
    mlib_s32 *dmin,
    mlib_s32 *dmax,
    mlib_s16 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 slb,
    mlib_s32 period)
{
	__m128i _mm_load_min0, _mm_load_min1, _mm_load_min2;
	__m128i _mm_load_max0, _mm_load_max1, _mm_load_max2;
	__m128i _mm_min0, _mm_min1, _mm_min2;
	__m128i _mm_max0, _mm_max1, _mm_max2;
	__m128i _mm_mask;
	mlib_s32 min0, min1, min2, min3;
	mlib_s32 max0, max1, max2, max3;
	mlib_s32 j, k;
	mlib_s32 a = period;
	mlib_s32 b = period * 2;
	mlib_s32 c = period * 3;
	mlib_s32 d = period * 4;
	mlib_s32 e = period * 5;
	mlib_s32 f = period * 6;
	mlib_s32 g = period * 7;
	mlib_s32 h = period * 8;

	const __m128i x_zero = _mm_setzero_si128();

	_mm_min0 = _mm_min1 = _mm_min2 = _mm_set1_epi16(MLIB_S16_MAX);
	_mm_max0 = _mm_max1 = _mm_max2 = _mm_set1_epi16(MLIB_S16_MIN);

	min0 = min1 = min2 = min3 = dmin[0];
	max0 = max1 = max2 = max3 = dmax[0];

	for (j = 0; j < ysize; j++) {
		mlib_s16 *sp = sl;
		mlib_s16 *loopend = sl + xsize;

		for (; sp < (loopend - g); sp += h) {
			_mm_load_min0 = _mm_load_max0 = _mm_setr_epi16(
			    sp[0], sp[1], sp[2], sp[a + 0], sp[a + 1],
			    sp[a + 2], sp[b + 0], sp[b + 1]);
			_mm_load_min1 = _mm_load_max1 = _mm_setr_epi16(
			    sp[b + 2], sp[c + 0], sp[c + 1], sp[c + 2],
			    sp[d + 0], sp[d + 1], sp[d + 2], sp[e + 0]);
			_mm_load_min2 = _mm_load_max2 = _mm_setr_epi16(
			    sp[e + 1], sp[e + 2], sp[f + 0], sp[f + 1],
			    sp[f + 2], sp[g + 0], sp[g + 1], sp[g + 2]);

			MLIB_S_IMAGE_MAXIMUM_S16(_mm_max0, _mm_max0,
			    _mm_load_max0);
			MLIB_S_IMAGE_MINIMUM_S16(_mm_min0, _mm_min0,
			    _mm_load_max0);
			MLIB_S_IMAGE_MAXIMUM_S16(_mm_max1, _mm_max1,
			    _mm_load_max1);
			MLIB_S_IMAGE_MINIMUM_S16(_mm_min1, _mm_min1,
			    _mm_load_max1);
			MLIB_S_IMAGE_MAXIMUM_S16(_mm_max2, _mm_max2,
			    _mm_load_max2);
			MLIB_S_IMAGE_MINIMUM_S16(_mm_min2, _mm_min2,
			    _mm_load_max2);
		}

		mlib_s16 stmax[21], stmin[21];
		for (k = 0; k < 21; k++) {
			stmin[k] = MLIB_S16_MAX;
			stmax[k] = MLIB_S16_MIN;
		}

		for (k = 0; sp < loopend; k += 3, sp += period) {
			stmin[k] = stmax[k] = *sp;
			stmin[k + 1] = stmax[k + 1] = *(sp + 1);
			stmin[k + 2] = stmax[k + 2] = *(sp + 2);
		}

		_mm_load_min0 =
		    _mm_setr_epi16(stmin[0], stmin[1], stmin[2], stmin[3],
		    stmin[4], stmin[5], stmin[6], stmin[7]);
		_mm_load_min1 =
		    _mm_setr_epi16(stmin[8], stmin[9], stmin[10], stmin[11],
		    stmin[12], stmin[13], stmin[14], stmin[15]);
		_mm_load_min2 =
		    _mm_setr_epi16(stmin[16], stmin[17], stmin[18], stmin[19],
		    stmin[20], MLIB_S16_MAX, MLIB_S16_MAX, MLIB_S16_MAX);

		_mm_load_max0 =
		    _mm_setr_epi16(stmax[0], stmax[1], stmax[2], stmax[3],
		    stmax[4], stmax[5], stmax[6], stmax[7]);
		_mm_load_max1 =
		    _mm_setr_epi16(stmax[8], stmax[9], stmax[10], stmax[11],
		    stmax[12], stmax[13], stmax[14], stmax[15]);
		_mm_load_max2 =
		    _mm_setr_epi16(stmax[16], stmax[17], stmax[18], stmax[19],
		    stmax[20], MLIB_S16_MIN, MLIB_S16_MIN, MLIB_S16_MIN);

		MLIB_S_IMAGE_MAXIMUM_S16(_mm_max0, _mm_max0, _mm_load_max0);
		MLIB_S_IMAGE_MINIMUM_S16(_mm_min0, _mm_min0, _mm_load_min0);
		MLIB_S_IMAGE_MAXIMUM_S16(_mm_max1, _mm_max1, _mm_load_max1);
		MLIB_S_IMAGE_MINIMUM_S16(_mm_min1, _mm_min1, _mm_load_min1);
		MLIB_S_IMAGE_MAXIMUM_S16(_mm_max2, _mm_max2, _mm_load_max2);
		MLIB_S_IMAGE_MINIMUM_S16(_mm_min2, _mm_min2, _mm_load_min2);
		sl += slb;
	}

	mlib_s16 s16max0[8], s16max1[8], s16max2[8];
	mlib_s16 s16min0[8], s16min1[8], s16min2[8];
	_mm_storeu_si128((void *)s16max0, _mm_max0);
	_mm_storeu_si128((void *)s16max1, _mm_max1);
	_mm_storeu_si128((void *)s16max2, _mm_max2);
	_mm_storeu_si128((void *)s16min0, _mm_min0);
	_mm_storeu_si128((void *)s16min1, _mm_min1);
	_mm_storeu_si128((void *)s16min2, _mm_min2);

/* zero channel */
	min0 = MLIB_MIN(s16min0[0], s16min0[3]);
	min1 = MLIB_MIN(s16min0[6], s16min1[1]);
	min2 = MLIB_MIN(s16min1[4], s16min1[7]);
	min3 = MLIB_MIN(s16min2[2], s16min2[5]);
	min0 = MLIB_MIN(min0, min1);
	min2 = MLIB_MIN(min2, min3);
	min0 = MLIB_MIN(min0, min2);

	max0 = MLIB_MAX(s16max0[0], s16max0[3]);
	max1 = MLIB_MAX(s16max0[6], s16max1[1]);
	max2 = MLIB_MAX(s16max1[4], s16max1[7]);
	max3 = MLIB_MAX(s16max2[2], s16max2[5]);
	max0 = MLIB_MAX(max0, max1);
	max2 = MLIB_MAX(max2, max3);
	max0 = MLIB_MAX(max0, max2);
	dmin[0] = min0;
	dmax[0] = max0;

/* first channel */
	min0 = MLIB_MIN(s16min0[1], s16min0[4]);
	min1 = MLIB_MIN(s16min0[7], s16min1[2]);
	min2 = MLIB_MIN(s16min1[5], s16min2[0]);
	min3 = MLIB_MIN(s16min2[3], s16min2[6]);
	min0 = MLIB_MIN(min0, min1);
	min2 = MLIB_MIN(min2, min3);
	min0 = MLIB_MIN(min0, min2);

	max0 = MLIB_MAX(s16max0[1], s16max0[4]);
	max1 = MLIB_MAX(s16max0[7], s16max1[2]);
	max2 = MLIB_MAX(s16max1[5], s16max2[0]);
	max3 = MLIB_MAX(s16max2[3], s16max2[6]);
	max0 = MLIB_MAX(max0, max1);
	max2 = MLIB_MAX(max2, max3);
	max0 = MLIB_MAX(max0, max2);
	dmin[1] = min0;
	dmax[1] = max0;

/* second channel */
	min0 = MLIB_MIN(s16min0[2], s16min0[5]);
	min1 = MLIB_MIN(s16min1[0], s16min1[3]);
	min2 = MLIB_MIN(s16min1[6], s16min2[1]);
	min3 = MLIB_MIN(s16min2[4], s16min2[7]);
	min0 = MLIB_MIN(min0, min1);
	min2 = MLIB_MIN(min2, min3);
	min0 = MLIB_MIN(min0, min2);

	max0 = MLIB_MAX(s16max0[2], s16max0[5]);
	max1 = MLIB_MAX(s16max1[0], s16max1[3]);
	max2 = MLIB_MAX(s16max1[6], s16max2[1]);
	max3 = MLIB_MAX(s16max2[4], s16max2[7]);
	max0 = MLIB_MAX(max0, max1);
	max2 = MLIB_MAX(max2, max3);
	max0 = MLIB_MAX(max0, max2);
	dmin[2] = min0;
	dmax[2] = max0;
}

/* *********************************************************** */

void
mlib_s_ImageExtrema2_4_S16(
    mlib_s32 *dmin,
    mlib_s32 *dmax,
    mlib_s16 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 slb,
    mlib_s32 period)
{
	__m128i _mm_load_min;
	__m128i _mm_load_max;
	__m128i _mm_min;
	__m128i _mm_max;
	mlib_s32 j, k;
	mlib_s32 min0, min1, min2, min3;
	mlib_s32 max0, max1, max2, max3;

	const __m128i x_zero = _mm_setzero_si128();

	_mm_min = _mm_set1_epi16(MLIB_S16_MAX);
	_mm_max = _mm_set1_epi16(MLIB_S16_MIN);

	min0 = min1 = min2 = min3 = dmin[0];
	max0 = max1 = max2 = max3 = dmax[0];

	for (j = 0; j < ysize; j++) {
		mlib_s16 *sp = sl;
		mlib_s16 *send = sl + xsize;

		for (; sp < (send - period); sp += 2 * period) {
			_mm_load_min = _mm_load_max =
			    _mm_setr_epi16(sp[0], sp[1], sp[2], sp[3],
			    sp[period], sp[period + 1], sp[period + 2],
			    sp[period + 3]);

			MLIB_S_IMAGE_MAXIMUM_S16(_mm_max, _mm_max,
			    _mm_load_max);
			MLIB_S_IMAGE_MINIMUM_S16(_mm_min, _mm_min,
			    _mm_load_max);
		}

		mlib_s16 stmax[4], stmin[4];
		for (k = 0; k < 4; k++) {
			stmin[k] = MLIB_S16_MAX;
			stmax[k] = MLIB_S16_MIN;
		}

		for (k = 0; sp < send; k += 4, sp += period) {
			stmin[k] = stmax[k] = *sp;
			stmin[k + 1] = stmax[k + 1] = *(sp + 1);
			stmin[k + 2] = stmax[k + 2] = *(sp + 2);
			stmin[k + 3] = stmax[k + 3] = *(sp + 3);
		}

		_mm_load_min =
		    _mm_setr_epi16(stmin[0], stmin[1], stmin[2], stmin[3],
		    MLIB_S16_MAX, MLIB_S16_MAX, MLIB_S16_MAX, MLIB_S16_MAX);

		_mm_load_max =
		    _mm_setr_epi16(stmax[0], stmax[1], stmax[2], stmax[3],
		    MLIB_S16_MIN, MLIB_S16_MIN, MLIB_S16_MIN, MLIB_S16_MIN);

		MLIB_S_IMAGE_MAXIMUM_S16(_mm_max, _mm_max, _mm_load_max);
		MLIB_S_IMAGE_MINIMUM_S16(_mm_min, _mm_min, _mm_load_min);
		sl += slb;
	}

	__m128i s128max, s128min;
	mlib_s16 *s16max = (mlib_s16 *)&s128max;
	mlib_s16 *s16min = (mlib_s16 *)&s128min;
	_mm_store_si128((void *)s16max, _mm_max);
	_mm_store_si128((void *)s16min, _mm_min);

	for (k = 0; k < 8; k += 4) {
		max0 = MLIB_MAX(max0, s16max[k]);
		max1 = MLIB_MAX(max1, s16max[k + 1]);
		max2 = MLIB_MAX(max2, s16max[k + 2]);
		max3 = MLIB_MAX(max3, s16max[k + 3]);

		min0 = MLIB_MIN(min0, s16min[k]);
		min1 = MLIB_MIN(min1, s16min[k + 1]);
		min2 = MLIB_MIN(min2, s16min[k + 2]);
		min3 = MLIB_MIN(min3, s16min[k + 3]);
	}

	dmin[0] = min0;
	dmin[1] = min1;
	dmin[2] = min2;
	dmin[3] = min3;
	dmax[0] = max0;
	dmax[1] = max1;
	dmax[2] = max2;
	dmax[3] = max3;
}

/* *********************************************************** */

#define	MLIB_S_IMAGE_MAXIMUM_U16(dd1, max, sd1)                 \
{                                                               \
	__m128i cmask =                                         \
	    _mm_cmpeq_epi16(_mm_subs_epu16(max, sd1), x_zero);  \
	dd1 =                                                   \
	    _mm_or_si128(_mm_and_si128(sd1, cmask),             \
	    _mm_andnot_si128(cmask, max));                      \
}

/* *********************************************************** */

#define	MLIB_S_IMAGE_MINIMUM_U16(dd1, min, sd1)             \
{                                                           \
	__m128i rmp = _mm_subs_epu16(sd1, min);             \
	__m128i cmask =                                     \
	    _mm_cmpeq_epi16(rmp, x_zero);                   \
	dd1 =                                               \
	    _mm_or_si128(_mm_and_si128(sd1, cmask),         \
	    _mm_andnot_si128(cmask, min));                  \
}

/* *********************************************************** */

void
mlib_s_ImageExtrema2_1_U16(
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
	__m128i _mm_load_min;
	__m128i _mm_load_max;
	__m128i _mm_min;
	__m128i _mm_max;
	mlib_s32 j, k;
	mlib_s32 min0, min1;
	mlib_s32 max0, max1;
	mlib_s32 a = off1;
	mlib_s32 b = 2 * off1;
	mlib_s32 c = 3 * off1;
	mlib_s32 d = 4 * off1;
	mlib_s32 e = 5 * off1;
	mlib_s32 f = 6 * off1;
	mlib_s32 g = 7 * off1;
	mlib_s32 h = 8 * off1;

	_mm_min = _mm_set1_epi16(MLIB_U16_MAX);
	_mm_max = _mm_set1_epi16(MLIB_U16_MIN);

	min0 = min1 = dmin[0];
	max0 = max1 = dmax[0];

	const __m128i x_zero = _mm_setzero_si128();

	for (j = 0; j < ysize; j++) {
		mlib_u16 *sp = sl;
		mlib_u16 *send = sl + xsize;

		for (k = 0; sp < (send - g); k++, sp += h) {
			_mm_load_min = _mm_load_max =
			    _mm_setr_epi16(sp[0], sp[a], sp[b], sp[c],
				    sp[d], sp[e], sp[f], sp[g]);

			MLIB_S_IMAGE_MAXIMUM_U16(_mm_max, _mm_max,
			    _mm_load_max);
			MLIB_S_IMAGE_MINIMUM_U16(_mm_min, _mm_min,
			    _mm_load_min);
		}

		mlib_u16 stmax[7], stmin[7];
		for (k = 0; k < 7; k++) {
			stmin[k] = MLIB_U16_MAX;
			stmax[k] = MLIB_U16_MIN;
		}

		for (k = 0; sp < send; k++, sp += off1) {
			stmin[k] = stmax[k] = *sp;
		}

		_mm_load_min =
		    _mm_setr_epi16(stmin[0], stmin[1], stmin[2], stmin[3],
		    stmin[4], stmin[5], stmin[6], MLIB_U16_MAX);

		_mm_load_max =
		    _mm_setr_epi16(stmax[0], stmax[1], stmax[2], stmax[3],
		    stmax[4], stmax[5], stmax[6], MLIB_U16_MIN);

		MLIB_S_IMAGE_MAXIMUM_U16(_mm_max, _mm_max, _mm_load_max);
		MLIB_S_IMAGE_MINIMUM_U16(_mm_min, _mm_min, _mm_load_min);
		sl += slb;
	}

	__m128i s128max, s128min;
	mlib_u16 *u16max = (mlib_u16 *)&s128max;
	mlib_u16 *u16min = (mlib_u16 *)&s128min;

	_mm_store_si128((void *)u16max, _mm_max);
	_mm_store_si128((void *)u16min, _mm_min);

	for (k = 0; k < 8; k += 2) {
		max0 = MLIB_MAX(max0, u16max[k]);
		max1 = MLIB_MAX(max1, u16max[k + 1]);
		min0 = MLIB_MIN(min0, u16min[k]);
		min1 = MLIB_MIN(min1, u16min[k + 1]);
	}

	dmin[0] = min0;
	dmax[0] = max0;
	dmin[1] = min1;
	dmax[1] = max1;
}

/* *********************************************************** */

void
mlib_s_ImageExtrema2_2_U16(
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
	__m128i _mm_load_min;
	__m128i _mm_load_max;
	__m128i _mm_min;
	__m128i _mm_max;
	mlib_s32 j, k;
	mlib_s32 min0, min1;
	mlib_s32 max0, max1;

	_mm_min = _mm_set1_epi16(MLIB_U16_MAX);
	_mm_max = _mm_set1_epi16(MLIB_U16_MIN);

	min0 = min1 = dmin[0];
	max0 = max1 = dmax[0];

	const __m128i x_zero = _mm_setzero_si128();

	for (j = 0; j < ysize; j++) {
		mlib_u16 *sp = sl;
		mlib_u16 *send = sl + xsize;

		for (; sp < (send - 3 * off2); sp += 4 * off2) {
			_mm_load_min = _mm_load_max =
			    _mm_setr_epi16(sp[0], sp[1], sp[off2], sp[off2 + 1],
			    sp[2 * off2], sp[2 * off2 + 1], sp[3 * off2],
			    sp[3 * off2 + 1]);

			MLIB_S_IMAGE_MAXIMUM_U16(_mm_max, _mm_max,
			    _mm_load_max);
			MLIB_S_IMAGE_MINIMUM_U16(_mm_min, _mm_min,
			    _mm_load_min);
		}

		mlib_u16 stmax[6], stmin[6];
		for (k = 0; k < 6; k++) {
			stmin[k] = MLIB_U16_MAX;
			stmax[k] = MLIB_U16_MIN;
		}

		for (k = 0; sp < send; k += 2, sp += off2) {
			stmin[k] = stmax[k] = *sp;
			stmin[k + 1] = stmax[k + 1] = *(sp + 1);
		}

		_mm_load_min =
		    _mm_setr_epi16(stmin[0], stmin[1], stmin[2], stmin[3],
		    stmin[4], stmin[5], MLIB_U16_MAX, MLIB_U16_MAX);

		_mm_load_max =
		    _mm_setr_epi16(stmax[0], stmax[1], stmax[2], stmax[3],
		    stmax[4], stmax[5], MLIB_U16_MIN, MLIB_U16_MIN);

		MLIB_S_IMAGE_MAXIMUM_U16(_mm_max, _mm_max, _mm_load_max);
		MLIB_S_IMAGE_MINIMUM_U16(_mm_min, _mm_min, _mm_load_min);
		sl += slb;
	}

	__m128i s128max, s128min;
	mlib_u16 *u16max = (mlib_u16 *)&s128max;
	mlib_u16 *u16min = (mlib_u16 *)&s128min;
	_mm_store_si128((void *)u16max, _mm_max);
	_mm_store_si128((void *)u16min, _mm_min);

		for (k = 0; k < 8; k += 2) {
			max0 = MLIB_MAX(max0, u16max[k]);
			max1 = MLIB_MAX(max1, u16max[k + 1]);
			min0 = MLIB_MIN(min0, u16min[k]);
			min1 = MLIB_MIN(min1, u16min[k + 1]);
		}

	dmin[0] = min0;
	dmax[0] = max0;
	dmin[1] = min1;
	dmax[1] = max1;
}

/* *********************************************************** */

void
mlib_s_ImageExtrema2_3_U16(
    mlib_s32 *dmin,
    mlib_s32 *dmax,
    mlib_u16 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 slb,
    mlib_s32 period)
{
	__m128i _mm_load_min0, _mm_load_min1, _mm_load_min2;
	__m128i _mm_load_max0, _mm_load_max1, _mm_load_max2;
	__m128i _mm_min0, _mm_min1, _mm_min2;
	__m128i _mm_max0, _mm_max1, _mm_max2;
	__m128i _mm_mask;
	mlib_s32 min0, min1, min2, min3;
	mlib_s32 max0, max1, max2, max3;
	mlib_s32 j, k;
	mlib_s32 a = period;
	mlib_s32 b = period * 2;
	mlib_s32 c = period * 3;
	mlib_s32 d = period * 4;
	mlib_s32 e = period * 5;
	mlib_s32 f = period * 6;
	mlib_s32 g = period * 7;
	mlib_s32 h = period * 8;

	const __m128i x_zero = _mm_setzero_si128();

	_mm_min0 = _mm_min1 = _mm_min2 = _mm_set1_epi16(MLIB_U16_MAX);
	_mm_max0 = _mm_max1 = _mm_max2 = _mm_set1_epi16(MLIB_U16_MIN);

	min0 = min1 = min2 = min3 = dmin[0];
	max0 = max1 = max2 = max3 = dmax[0];

	for (j = 0; j < ysize; j++) {
		mlib_u16 *sp = sl;
		mlib_u16 *loopend = sl + xsize;

		for (; sp < (loopend - g); sp += h) {
			_mm_load_min0 = _mm_load_max0 = _mm_setr_epi16(
			    sp[0], sp[1], sp[2], sp[a + 0], sp[a + 1],
			    sp[a + 2], sp[b + 0], sp[b + 1]);
			_mm_load_min1 = _mm_load_max1 = _mm_setr_epi16(
			    sp[b + 2], sp[c + 0], sp[c + 1], sp[c + 2],
			    sp[d + 0], sp[d + 1], sp[d + 2], sp[e + 0]);
			_mm_load_min2 = _mm_load_max2 = _mm_setr_epi16(
			    sp[e + 1], sp[e + 2], sp[f + 0], sp[f + 1],
			    sp[f + 2], sp[g + 0], sp[g + 1], sp[g + 2]);

			MLIB_S_IMAGE_MAXIMUM_U16(_mm_max0, _mm_max0,
			    _mm_load_max0);
			MLIB_S_IMAGE_MINIMUM_U16(_mm_min0, _mm_min0,
			    _mm_load_max0);
			MLIB_S_IMAGE_MAXIMUM_U16(_mm_max1, _mm_max1,
			    _mm_load_max1);
			MLIB_S_IMAGE_MINIMUM_U16(_mm_min1, _mm_min1,
			    _mm_load_max1);
			MLIB_S_IMAGE_MAXIMUM_U16(_mm_max2, _mm_max2,
			    _mm_load_max2);
			MLIB_S_IMAGE_MINIMUM_U16(_mm_min2, _mm_min2,
			    _mm_load_max2);
		}

		mlib_u16 stmax[21], stmin[21];
		for (k = 0; k < 21; k++) {
			stmin[k] = MLIB_U16_MAX;
			stmax[k] = MLIB_U16_MIN;
		}

		for (k = 0; sp < loopend; k += 3, sp += period) {
			stmin[k] = stmax[k] = *sp;
			stmin[k + 1] = stmax[k + 1] = *(sp + 1);
			stmin[k + 2] = stmax[k + 2] = *(sp + 2);
		}

		_mm_load_min0 =
		    _mm_setr_epi16(stmin[0], stmin[1], stmin[2], stmin[3],
		    stmin[4], stmin[5], stmin[6], stmin[7]);
		_mm_load_min1 =
		    _mm_setr_epi16(stmin[8], stmin[9], stmin[10], stmin[11],
		    stmin[12], stmin[13], stmin[14], stmin[15]);
		_mm_load_min2 =
		    _mm_setr_epi16(stmin[16], stmin[17], stmin[18], stmin[19],
		    stmin[20], MLIB_U16_MAX, MLIB_U16_MAX, MLIB_U16_MAX);

		_mm_load_max0 =
		    _mm_setr_epi16(stmax[0], stmax[1], stmax[2], stmax[3],
		    stmax[4], stmax[5], stmax[6], stmax[7]);
		_mm_load_max1 =
		    _mm_setr_epi16(stmax[8], stmax[9], stmax[10], stmax[11],
		    stmax[12], stmax[13], stmax[14], stmax[15]);
		_mm_load_max2 =
		    _mm_setr_epi16(stmax[16], stmax[17], stmax[18], stmax[19],
		    stmax[20], MLIB_U16_MIN, MLIB_U16_MIN, MLIB_U16_MIN);

		MLIB_S_IMAGE_MAXIMUM_U16(_mm_max0, _mm_max0, _mm_load_max0);
		MLIB_S_IMAGE_MINIMUM_U16(_mm_min0, _mm_min0, _mm_load_min0);
		MLIB_S_IMAGE_MAXIMUM_U16(_mm_max1, _mm_max1, _mm_load_max1);
		MLIB_S_IMAGE_MINIMUM_U16(_mm_min1, _mm_min1, _mm_load_min1);
		MLIB_S_IMAGE_MAXIMUM_U16(_mm_max2, _mm_max2, _mm_load_max2);
		MLIB_S_IMAGE_MINIMUM_U16(_mm_min2, _mm_min2, _mm_load_min2);
		sl += slb;
	}

	mlib_u16 u16max0[8], u16max1[8], u16max2[8];
	mlib_u16 u16min0[8], u16min1[8], u16min2[8];
	_mm_storeu_si128((void *)u16max0, _mm_max0);
	_mm_storeu_si128((void *)u16max1, _mm_max1);
	_mm_storeu_si128((void *)u16max2, _mm_max2);
	_mm_storeu_si128((void *)u16min0, _mm_min0);
	_mm_storeu_si128((void *)u16min1, _mm_min1);
	_mm_storeu_si128((void *)u16min2, _mm_min2);

/* zero channel */
	min0 = MLIB_MIN(u16min0[0], u16min0[3]);
	min1 = MLIB_MIN(u16min0[6], u16min1[1]);
	min2 = MLIB_MIN(u16min1[4], u16min1[7]);
	min3 = MLIB_MIN(u16min2[2], u16min2[5]);
	min0 = MLIB_MIN(min0, min1);
	min2 = MLIB_MIN(min2, min3);
	min0 = MLIB_MIN(min0, min2);

	max0 = MLIB_MAX(u16max0[0], u16max0[3]);
	max1 = MLIB_MAX(u16max0[6], u16max1[1]);
	max2 = MLIB_MAX(u16max1[4], u16max1[7]);
	max3 = MLIB_MAX(u16max2[2], u16max2[5]);
	max0 = MLIB_MAX(max0, max1);
	max2 = MLIB_MAX(max2, max3);
	max0 = MLIB_MAX(max0, max2);
	dmin[0] = min0;
	dmax[0] = max0;

/* first channel */
	min0 = MLIB_MIN(u16min0[1], u16min0[4]);
	min1 = MLIB_MIN(u16min0[7], u16min1[2]);
	min2 = MLIB_MIN(u16min1[5], u16min2[0]);
	min3 = MLIB_MIN(u16min2[3], u16min2[6]);
	min0 = MLIB_MIN(min0, min1);
	min2 = MLIB_MIN(min2, min3);
	min0 = MLIB_MIN(min0, min2);

	max0 = MLIB_MAX(u16max0[1], u16max0[4]);
	max1 = MLIB_MAX(u16max0[7], u16max1[2]);
	max2 = MLIB_MAX(u16max1[5], u16max2[0]);
	max3 = MLIB_MAX(u16max2[3], u16max2[6]);
	max0 = MLIB_MAX(max0, max1);
	max2 = MLIB_MAX(max2, max3);
	max0 = MLIB_MAX(max0, max2);
	dmin[1] = min0;
	dmax[1] = max0;

/* second channel */
	min0 = MLIB_MIN(u16min0[2], u16min0[5]);
	min1 = MLIB_MIN(u16min1[0], u16min1[3]);
	min2 = MLIB_MIN(u16min1[6], u16min2[1]);
	min3 = MLIB_MIN(u16min2[4], u16min2[7]);
	min0 = MLIB_MIN(min0, min1);
	min2 = MLIB_MIN(min2, min3);
	min0 = MLIB_MIN(min0, min2);

	max0 = MLIB_MAX(u16max0[2], u16max0[5]);
	max1 = MLIB_MAX(u16max1[0], u16max1[3]);
	max2 = MLIB_MAX(u16max1[6], u16max2[1]);
	max3 = MLIB_MAX(u16max2[4], u16max2[7]);
	max0 = MLIB_MAX(max0, max1);
	max2 = MLIB_MAX(max2, max3);
	max0 = MLIB_MAX(max0, max2);
	dmin[2] = min0;
	dmax[2] = max0;
}

/* *********************************************************** */

void
mlib_s_ImageExtrema2_4_U16(
    mlib_s32 *dmin,
    mlib_s32 *dmax,
    mlib_u16 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 slb,
    mlib_s32 period)
{
	__m128i _mm_load_min;
	__m128i _mm_load_max;
	__m128i _mm_min;
	__m128i _mm_max;
	mlib_s32 j, k;
	mlib_s32 min0, min1, min2, min3;
	mlib_s32 max0, max1, max2, max3;

	const __m128i x_zero = _mm_setzero_si128();

	_mm_min = _mm_set1_epi16(MLIB_U16_MAX);
	_mm_max = _mm_set1_epi16(MLIB_U16_MIN);

	min0 = min1 = min2 = min3 = dmin[0];
	max0 = max1 = max2 = max3 = dmax[0];

	for (j = 0; j < ysize; j++) {
		mlib_u16 *sp = sl;
		mlib_u16 *send = sl + xsize;

		for (; sp < (send - period); sp += 2 * period) {
			_mm_load_min = _mm_load_max = _mm_setr_epi16(
			    sp[0], sp[1], sp[2], sp[3], sp[period],
			    sp[period + 1], sp[period + 2], sp[period + 3]);

			MLIB_S_IMAGE_MAXIMUM_U16(_mm_max, _mm_max,
			    _mm_load_max);
			MLIB_S_IMAGE_MINIMUM_U16(_mm_min, _mm_min,
			    _mm_load_max);
		}

		mlib_u16 stmax[4], stmin[4];
		for (k = 0; k < 4; k++) {
			stmin[k] = MLIB_U16_MAX;
			stmax[k] = MLIB_U16_MIN;
		}

		for (k = 0; sp < send; k += 4, sp += period) {
			stmin[k] = stmax[k] = *sp;
			stmin[k + 1] = stmax[k + 1] = *(sp + 1);
			stmin[k + 2] = stmax[k + 2] = *(sp + 2);
			stmin[k + 3] = stmax[k + 3] = *(sp + 3);
		}

		_mm_load_min =
		    _mm_setr_epi16(stmin[0], stmin[1], stmin[2], stmin[3],
		    MLIB_U16_MAX, MLIB_U16_MAX, MLIB_U16_MAX, MLIB_U16_MAX);

		_mm_load_max =
		    _mm_setr_epi16(stmax[0], stmax[1], stmax[2], stmax[3],
		    MLIB_U16_MIN, MLIB_U16_MIN, MLIB_U16_MIN, MLIB_U16_MIN);

		MLIB_S_IMAGE_MAXIMUM_U16(_mm_max, _mm_max, _mm_load_max);
		MLIB_S_IMAGE_MINIMUM_U16(_mm_min, _mm_min, _mm_load_min);
		sl += slb;
	}

	__m128i s128max, s128min;
	mlib_u16 *u16max = (mlib_u16 *)&s128max;
	mlib_u16 *u16min = (mlib_u16 *)&s128min;
	_mm_store_si128((void *)u16max, _mm_max);
	_mm_store_si128((void *)u16min, _mm_min);

	for (k = 0; k < 8; k += 4) {
		max0 = MLIB_MAX(max0, u16max[k]);
		max1 = MLIB_MAX(max1, u16max[k + 1]);
		max2 = MLIB_MAX(max2, u16max[k + 2]);
		max3 = MLIB_MAX(max3, u16max[k + 3]);

		min0 = MLIB_MIN(min0, u16min[k]);
		min1 = MLIB_MIN(min1, u16min[k + 1]);
		min2 = MLIB_MIN(min2, u16min[k + 2]);
		min3 = MLIB_MIN(min3, u16min[k + 3]);
	}

	dmin[0] = min0;
	dmin[1] = min1;
	dmin[2] = min2;
	dmin[3] = min3;
	dmax[0] = max0;
	dmax[1] = max1;
	dmax[2] = max2;
	dmax[3] = max3;
}

/* *********************************************************** */

#define	MLIB_S_IMAGE_MAXIMUM_S32(dd1, max, sd1)         \
{                                                       \
	__m128i cmask = _mm_cmpgt_epi32(sd1, max);      \
	dd1 =                                           \
	    _mm_or_si128(_mm_and_si128(sd1, cmask),     \
	    _mm_andnot_si128(cmask, max));              \
}

/* *********************************************************** */

#define	MLIB_S_IMAGE_MINIMUM_S32(dd1, min, sd1)         \
{                                                       \
	__m128i cmask = _mm_cmpgt_epi32(min, sd1);      \
	dd1 =                                           \
	    _mm_or_si128(_mm_and_si128(sd1, cmask),     \
	    _mm_andnot_si128(cmask, min));              \
}

/* *********************************************************** */

void
mlib_s_ImageExtrema2_1_S32(
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
	__m128i _mm_load_min;
	__m128i _mm_load_max;
	__m128i _mm_min;
	__m128i _mm_max;
	mlib_s32 j, k;

	_mm_min = _mm_set1_epi32(MLIB_S32_MAX);
	_mm_max = _mm_set1_epi32(MLIB_S32_MIN);

	const __m128i x_zero = _mm_setzero_si128();

	for (j = 0; j < ysize; j++) {
		mlib_s32 *sp = sl;
		mlib_s32 *send = sl + xsize;

		for (k = 0; sp < (send - off3); k++, sp += off4) {
			_mm_load_min = _mm_load_max =
			    _mm_setr_epi32(sp[0], sp[off1], sp[off2], sp[off3]);

			MLIB_S_IMAGE_MAXIMUM_S32(_mm_max, _mm_max,
			    _mm_load_max);
			MLIB_S_IMAGE_MINIMUM_S32(_mm_min, _mm_min,
			    _mm_load_min);
		}

		mlib_s32 stmax[3], stmin[3];
		for (k = 0; k < 3; k++) {
			stmin[k] = MLIB_S32_MAX;
			stmax[k] = MLIB_S32_MIN;
		}

		for (k = 0; sp < send; k++, sp += off1) {
			stmin[k] = stmax[k] = *sp;
		}

		_mm_load_min =
		    _mm_setr_epi32(stmin[0], stmin[1], stmin[2], MLIB_S32_MAX);

		_mm_load_max =
		    _mm_setr_epi32(stmax[0], stmax[1], stmax[2], MLIB_S32_MIN);

		MLIB_S_IMAGE_MAXIMUM_S32(_mm_max, _mm_max, _mm_load_max);
		MLIB_S_IMAGE_MINIMUM_S32(_mm_min, _mm_min, _mm_load_min);
		sl += slb;
	}

	_mm_storeu_si128((void *)dmax, _mm_max);
	_mm_storeu_si128((void *)dmin, _mm_min);
}

/* *********************************************************** */

void
mlib_s_ImageExtrema2_2_S32(
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
	__m128i _mm_load_min;
	__m128i _mm_load_max;
	__m128i _mm_min;
	__m128i _mm_max;
	mlib_s32 j, k;

	_mm_min = _mm_set1_epi32(MLIB_S32_MAX);
	_mm_max = _mm_set1_epi32(MLIB_S32_MIN);

	const __m128i x_zero = _mm_setzero_si128();

	for (j = 0; j < ysize; j++) {
		mlib_s32 *sp = sl;
		mlib_s32 *send = sl + xsize;

		for (k = 0; sp < (send - off3); k++, sp += off4) {
			_mm_load_min = _mm_load_max =
			    _mm_setr_epi32(sp[0], sp[off1], sp[off2], sp[off3]);

			MLIB_S_IMAGE_MAXIMUM_S32(_mm_max, _mm_max,
			    _mm_load_max);
			MLIB_S_IMAGE_MINIMUM_S32(_mm_min, _mm_min,
			    _mm_load_min);
		}

		mlib_s32 stmax[2], stmin[2];
		for (k = 0; k < 2; k++) {
			stmin[k] = MLIB_S32_MAX;
			stmax[k] = MLIB_S32_MIN;
		}

		for (k = 0; sp < send; k += 2, sp += off2) {
			stmin[k] = stmax[k] = *sp;
			stmin[k + 1] = stmax[k + 1] = *(sp + 1);
		}

		_mm_load_min = _mm_setr_epi32(stmin[0], stmin[1],
		    MLIB_S32_MAX, MLIB_S32_MAX);

		_mm_load_max = _mm_setr_epi32(stmax[0], stmax[1],
		    MLIB_S32_MIN, MLIB_S32_MIN);

		MLIB_S_IMAGE_MAXIMUM_S32(_mm_max, _mm_max, _mm_load_max);
		MLIB_S_IMAGE_MINIMUM_S32(_mm_min, _mm_min, _mm_load_min);
		sl += slb;
	}

	_mm_storeu_si128((void *)dmax, _mm_max);
	_mm_storeu_si128((void *)dmin, _mm_min);
}

/* *********************************************************** */

void
mlib_s_ImageExtrema2_3_S32(
    mlib_s32 *dmin,
    mlib_s32 *dmax,
    mlib_s32 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 slb,
    mlib_s32 period)
{
	__m128i _mm_load_min0, _mm_load_min1, _mm_load_min2;
	__m128i _mm_load_max0, _mm_load_max1, _mm_load_max2;
	__m128i _mm_min0, _mm_min1, _mm_min2;
	__m128i _mm_max0, _mm_max1, _mm_max2;
	__m128i _mm_mask;
	mlib_s32 min0, min1, min2, min3;
	mlib_s32 max0, max1, max2, max3;
	mlib_s32 j, k;
	mlib_s32 a = period;
	mlib_s32 b = period * 2;
	mlib_s32 c = period * 3;
	mlib_s32 d = period * 4;

	const __m128i x_zero = _mm_setzero_si128();

	_mm_min0 = _mm_min1 = _mm_min2 = _mm_set1_epi32(MLIB_S32_MAX);
	_mm_max0 = _mm_max1 = _mm_max2 = _mm_set1_epi32(MLIB_S32_MIN);

	min0 = min1 = min2 = min3 = dmin[0];
	max0 = max1 = max2 = max3 = dmax[0];

	for (j = 0; j < ysize; j++) {
		mlib_s32 *sp = sl;
		mlib_s32 *loopend = sl + xsize;

		for (; sp < (loopend - c); sp += d) {
			_mm_load_min0 = _mm_load_max0 =
			    _mm_setr_epi32(sp[0], sp[1], sp[2], sp[a + 0]);
			_mm_load_min1 = _mm_load_max1 = _mm_setr_epi32(
			    sp[a + 1], sp[a + 2], sp[b + 0], sp[b + 1]);
			_mm_load_min2 = _mm_load_max2 = _mm_setr_epi32(
			    sp[b + 2], sp[c + 0], sp[c + 1], sp[c + 2]);

			MLIB_S_IMAGE_MAXIMUM_S32(_mm_max0, _mm_max0,
			    _mm_load_max0);
			MLIB_S_IMAGE_MINIMUM_S32(_mm_min0, _mm_min0,
			    _mm_load_max0);
			MLIB_S_IMAGE_MAXIMUM_S32(_mm_max1, _mm_max1,
			    _mm_load_max1);
			MLIB_S_IMAGE_MINIMUM_S32(_mm_min1, _mm_min1,
			    _mm_load_max1);
			MLIB_S_IMAGE_MAXIMUM_S32(_mm_max2, _mm_max2,
			    _mm_load_max2);
			MLIB_S_IMAGE_MINIMUM_S32(_mm_min2, _mm_min2,
			    _mm_load_max2);
		}

		mlib_s32 stmax[9], stmin[9];
		for (k = 0; k < 9; k++) {
			stmin[k] = MLIB_S32_MAX;
			stmax[k] = MLIB_S32_MIN;
		}

		for (k = 0; sp < loopend; k += 3, sp += period) {
			stmin[k] = stmax[k] = *sp;
			stmin[k + 1] = stmax[k + 1] = *(sp + 1);
			stmin[k + 2] = stmax[k + 2] = *(sp + 2);
		}

		_mm_load_min0 =
		    _mm_setr_epi32(stmin[0], stmin[1], stmin[2], stmin[3]);
		_mm_load_min1 =
		    _mm_setr_epi32(stmin[4], stmin[5], stmin[6], stmin[7]);
		_mm_load_min2 = _mm_setr_epi32(stmin[8],
		    MLIB_S32_MAX, MLIB_S32_MAX, MLIB_S32_MAX);

		_mm_load_max0 =
		    _mm_setr_epi32(stmax[0], stmax[1], stmax[2], stmax[3]);
		_mm_load_max1 =
		    _mm_setr_epi32(stmax[4], stmax[5], stmax[6], stmax[7]);
		_mm_load_max2 = _mm_setr_epi32(stmax[8],
		    MLIB_S32_MIN, MLIB_S32_MIN, MLIB_S32_MIN);

		MLIB_S_IMAGE_MAXIMUM_S32(_mm_max0, _mm_max0, _mm_load_max0);
		MLIB_S_IMAGE_MINIMUM_S32(_mm_min0, _mm_min0, _mm_load_min0);
		MLIB_S_IMAGE_MAXIMUM_S32(_mm_max1, _mm_max1, _mm_load_max1);
		MLIB_S_IMAGE_MINIMUM_S32(_mm_min1, _mm_min1, _mm_load_min1);
		MLIB_S_IMAGE_MAXIMUM_S32(_mm_max2, _mm_max2, _mm_load_max2);
		MLIB_S_IMAGE_MINIMUM_S32(_mm_min2, _mm_min2, _mm_load_min2);
		sl += slb;
	}

	__m128i s128min0, s128min1, s128min2;
	__m128i s128max0, s128max1, s128max2;

	mlib_s32 *s32min0 = (mlib_s32 *)&s128min0;
	mlib_s32 *s32min1 = (mlib_s32 *)&s128min1;
	mlib_s32 *s32min2 = (mlib_s32 *)&s128min2;

	mlib_s32 *s32max0 = (mlib_s32 *)&s128max0;
	mlib_s32 *s32max1 = (mlib_s32 *)&s128max1;
	mlib_s32 *s32max2 = (mlib_s32 *)&s128max2;

	_mm_store_si128((void *)s32max0, _mm_max0);
	_mm_store_si128((void *)s32max1, _mm_max1);
	_mm_store_si128((void *)s32max2, _mm_max2);
	_mm_store_si128((void *)s32min0, _mm_min0);
	_mm_store_si128((void *)s32min1, _mm_min1);
	_mm_store_si128((void *)s32min2, _mm_min2);

/* zero channel */
	min0 = MLIB_MIN(s32min0[0], s32min0[3]);
	min1 = MLIB_MIN(s32min1[2], s32min2[1]);
	min0 = MLIB_MIN(min0, min1);

	max0 = MLIB_MAX(s32max0[0], s32max0[3]);
	max1 = MLIB_MAX(s32max1[2], s32max2[1]);
	max0 = MLIB_MAX(max0, max1);
	dmin[0] = min0;
	dmax[0] = max0;

/* first channel */
	min0 = MLIB_MIN(s32min0[1], s32min1[0]);
	min1 = MLIB_MIN(s32min1[3], s32min2[2]);
	min0 = MLIB_MIN(min0, min1);

	max0 = MLIB_MAX(s32max0[1], s32max1[0]);
	max1 = MLIB_MAX(s32max1[3], s32max2[2]);
	max0 = MLIB_MAX(max0, max1);
	dmin[1] = min0;
	dmax[1] = max0;

/* second channel */
	min0 = MLIB_MIN(s32min0[2], s32min1[1]);
	min1 = MLIB_MIN(s32min2[0], s32min2[3]);
	min0 = MLIB_MIN(min0, min1);

	max0 = MLIB_MAX(s32max0[2], s32max1[1]);
	max1 = MLIB_MAX(s32max2[0], s32max2[3]);
	max0 = MLIB_MAX(max0, max1);
	dmin[2] = min0;
	dmax[2] = max0;
}

/* *********************************************************** */

void
mlib_s_ImageExtrema2_4_S32(
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
	__m128i _mm_load_min;
	__m128i _mm_load_max;
	__m128i _mm_min;
	__m128i _mm_max;
	mlib_s32 j, k;

	_mm_min = _mm_set1_epi32(MLIB_S32_MAX);
	_mm_max = _mm_set1_epi32(MLIB_S32_MIN);

	const __m128i x_zero = _mm_setzero_si128();

	for (j = 0; j < ysize; j++) {
		mlib_s32 *sp = sl;
		mlib_s32 *send = sl + xsize;

		for (k = 0; sp < (send - off3); k++, sp += off4) {
			_mm_load_min = _mm_load_max =
			    _mm_setr_epi32(sp[0], sp[off1], sp[off2], sp[off3]);

			MLIB_S_IMAGE_MAXIMUM_S32(_mm_max, _mm_max,
			    _mm_load_max);
			MLIB_S_IMAGE_MINIMUM_S32(_mm_min, _mm_min,
			    _mm_load_min);
		}

		sl += slb;
	}

	_mm_storeu_si128((void *)dmax, _mm_max);
	_mm_storeu_si128((void *)dmin, _mm_min);
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
			mlib_s_ImageExtrema2_1_U8(dmin, dmax, (mlib_u8 *)sl,
			    xsize, ysize, slb, xPeriod, 2 * xPeriod,
			    3 * xPeriod, 4 * xPeriod);
			break;

		case 2:
			mlib_s_ImageExtrema2_2_U8(min, max, (mlib_u8 *)sl,
			    xsize, ysize, slb, 1, 2 * xPeriod, 2 * xPeriod + 1,
			    4 * xPeriod);
			break;

		case 3:
			mlib_s_ImageExtrema2_3_U8(min, max, (mlib_u8 *)sl,
			    xsize, ysize, slb, 3 * xPeriod);
			break;

		case 4:
			mlib_s_ImageExtrema2_4_U8(min, max, (mlib_u8 *)sl,
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
			mlib_s_ImageExtrema2_1_S16(dmin, dmax, (mlib_s16 *)sl,
			    xsize, ysize, slb, xPeriod, 2 * xPeriod,
			    3 * xPeriod, 4 * xPeriod);
			break;

		case 2:
			mlib_s_ImageExtrema2_2_S16(min, max, (mlib_s16 *)sl,
			    xsize, ysize, slb, 1, 2 * xPeriod, 2 * xPeriod + 1,
			    4 * xPeriod);
			break;

		case 3:
			mlib_s_ImageExtrema2_3_S16(min, max, (mlib_s16 *)sl,
			    xsize, ysize, slb, 3 * xPeriod);
			break;

		case 4:
			mlib_s_ImageExtrema2_4_S16(min, max, (mlib_s16 *)sl,
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
			mlib_s_ImageExtrema2_1_U16(dmin, dmax, (mlib_u16 *)sl,
			    xsize, ysize, slb, xPeriod, 2 * xPeriod,
			    3 * xPeriod, 4 * xPeriod);
			break;

		case 2:
			mlib_s_ImageExtrema2_2_U16(min, max, (mlib_u16 *)sl,
			    xsize, ysize, slb, 1, 2 * xPeriod, 2 * xPeriod + 1,
			    4 * xPeriod);
			break;

		case 3:
			mlib_s_ImageExtrema2_3_U16(min, max, (mlib_u16 *)sl,
			    xsize, ysize, slb, 3 * xPeriod);
			break;

		case 4:
			mlib_s_ImageExtrema2_4_U16(min, max, (mlib_u16 *)sl,
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
			mlib_s_ImageExtrema2_1_S32(dmin, dmax, (mlib_s32 *)sl,
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
			mlib_s_ImageExtrema2_2_S32(dmin, dmax, (mlib_s32 *)sl,
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
			mlib_s_ImageExtrema2_3_S32(min, max, (mlib_s32 *)sl,
			    xsize, ysize, slb, 3 * xPeriod);
			return (MLIB_SUCCESS);

		case 4:
			mlib_s_ImageExtrema2_4_S32(min, max, (mlib_s32 *)sl,
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
