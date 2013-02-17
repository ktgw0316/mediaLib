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

#pragma ident	"@(#)mlib_s_ImageExtrema2_Fp.c	9.2	07/11/05 SMI"

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
#include <mlib_sse_utils.h>

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
#define	MIN(min, x)	min = (min < x) ? min : x

#define	MAX(max, x)	max = (max > x) ? max : x

#define	MLIB_MIN(min, x)	     \
	(min < x) ? min : x

#define	MLIB_MAX(max, x)	     \
	(max > x) ? max : x

#define	MINMAX(min, max, x)          \
	max = (max > x) ? max : x;   \
	min = (min < x) ? min : x

/* *********************************************************** */

#define	FUNC_IMAGE_EXTREMA2_3(STYPE)                                   \
	static void                                                    \
	mlib_s_ImageExtrema2_3_##STYPE(mlib_d64 *dmin, mlib_d64 *dmax, \
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
	mlib_s_ImageExtrema2_4_##STYPE(mlib_d64 *dmin, mlib_d64 *dmax,   \
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
#define	MLIB_S_IMAGE_MAXIMUM_F32(dd1, max, sd1)         \
{                                                       \
	__m128 cmask = _mm_cmpgt_ps(sd1, max);          \
	dd1 =                                           \
	    _mm_or_ps(_mm_and_ps(sd1, cmask),           \
	    _mm_andnot_ps(cmask, max));                 \
}

/* *********************************************************** */
#define	MLIB_S_IMAGE_MINIMUM_F32(dd1, min, sd1)         \
{                                                       \
	__m128 cmask = _mm_cmpgt_ps(min, sd1);          \
	dd1 =                                           \
	    _mm_or_ps(_mm_and_ps(sd1, cmask),           \
	    _mm_andnot_ps(cmask, min));                 \
}

/* *********************************************************** */
/* mlib_s_ImageExtrema2_3_d64 */
	FUNC_IMAGE_EXTREMA2_3(d64)

/* mlib_s_ImageExtrema2_4_d64 */
    FUNC_IMAGE_EXTREMA2_4(d64)

/* *********************************************************** */
void
mlib_s_ImageExtrema2_1_f32(
    mlib_f32 *dmin,
    mlib_f32 *dmax,
    mlib_f32 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 slb,
    mlib_s32 off1,
    mlib_s32 off2,
    mlib_s32 off3,
    mlib_s32 off4)
{
	__m128 _mm_load_min;
	__m128 _mm_load_max;
	__m128 _mm_min;
	__m128 _mm_max;
	mlib_s32 j, k;
	mlib_s32 a = off1;
	mlib_s32 b = 2 * off1;
	mlib_s32 c = 3 * off1;
	mlib_s32 d = 4 * off1;

	_mm_min = _mm_set1_ps(MLIB_F32_MAX);
	_mm_max = _mm_set1_ps(MLIB_F32_MIN);

	const __m128 x_zero = _mm_setzero_ps();

	for (j = 0; j < ysize; j++) {
		mlib_f32 *sp = sl;
		mlib_f32 *send = sl + xsize;

		for (k = 0; sp < (send - off3); k++, sp += off4) {
			_mm_load_min = _mm_load_max =
			    _mm_setr_ps(sp[0], sp[off1], sp[off2], sp[off3]);

			MLIB_S_IMAGE_MAXIMUM_F32(_mm_max, _mm_max,
			    _mm_load_max);
			MLIB_S_IMAGE_MINIMUM_F32(_mm_min, _mm_min,
			    _mm_load_min);
		}

		mlib_f32 stmax[3], stmin[3];
		for (k = 0; k < 3; k++) {
			stmin[k] = MLIB_F32_MAX;
			stmax[k] = MLIB_F32_MIN;
		}

		for (k = 0; sp < send; k++, sp += off1) {
			stmin[k] = stmax[k] = *sp;
		}

		_mm_load_min =
		    _mm_setr_ps(stmin[0], stmin[1], stmin[2], MLIB_F32_MAX);

		_mm_load_max =
		    _mm_setr_ps(stmax[0], stmax[1], stmax[2], MLIB_F32_MIN);

		MLIB_S_IMAGE_MAXIMUM_F32(_mm_max, _mm_max, _mm_load_max);
		MLIB_S_IMAGE_MINIMUM_F32(_mm_min, _mm_min, _mm_load_min);
		sl += slb;
	}

	_mm_storeu_ps((void *)dmax, _mm_max);
	_mm_storeu_ps((void *)dmin, _mm_min);
}

/* *********************************************************** */
void
mlib_s_ImageExtrema2_2_f32(
    mlib_f32 *dmin,
    mlib_f32 *dmax,
    mlib_f32 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 slb,
    mlib_s32 off1,
    mlib_s32 off2,
    mlib_s32 off3,
    mlib_s32 off4)
{
	__m128 _mm_load_min;
	__m128 _mm_load_max;
	__m128 _mm_min;
	__m128 _mm_max;
	mlib_s32 j, k;
	mlib_s32 a = off1;
	mlib_s32 b = 2 * off1;
	mlib_s32 c = 3 * off1;
	mlib_s32 d = 4 * off1;

	_mm_min = _mm_set1_ps(MLIB_F32_MAX);
	_mm_max = _mm_set1_ps(MLIB_F32_MIN);

	const __m128 x_zero = _mm_setzero_ps();

	for (j = 0; j < ysize; j++) {
		mlib_f32 *sp = sl;
		mlib_f32 *send = sl + xsize;

		for (k = 0; sp < (send - off3); k++, sp += off4) {
			_mm_load_min = _mm_load_max =
			    _mm_setr_ps(sp[0], sp[off1], sp[off2], sp[off3]);

			MLIB_S_IMAGE_MAXIMUM_F32(_mm_max, _mm_max,
			    _mm_load_max);
			MLIB_S_IMAGE_MINIMUM_F32(_mm_min, _mm_min,
			    _mm_load_min);
		}

		mlib_f32 stmax[2], stmin[2];
		for (k = 0; k < 2; k++) {
			stmin[k] = MLIB_F32_MAX;
			stmax[k] = MLIB_F32_MIN;
		}

		for (k = 0; sp < send; k += 2, sp += off2) {
			stmin[k] = stmax[k] = *sp;
			stmin[k + 1] = stmax[k + 1] = *(sp + 1);
		}

		_mm_load_min =
		    _mm_setr_ps(stmin[0], stmin[1], MLIB_F32_MAX, MLIB_F32_MAX);

		_mm_load_max =
		    _mm_setr_ps(stmax[0], stmax[1], MLIB_F32_MIN, MLIB_F32_MIN);

		MLIB_S_IMAGE_MAXIMUM_F32(_mm_max, _mm_max, _mm_load_max);
		MLIB_S_IMAGE_MINIMUM_F32(_mm_min, _mm_min, _mm_load_min);
		sl += slb;
	}

	_mm_storeu_ps((void *)dmax, _mm_max);
	_mm_storeu_ps((void *)dmin, _mm_min);
}

/* *********************************************************** */
void
mlib_s_ImageExtrema2_3_f32(
    mlib_d64 *dmin,
    mlib_d64 *dmax,
    mlib_f32 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 slb,
    mlib_s32 period)
{
	__m128 _mm_load_min0, _mm_load_min1, _mm_load_min2;
	__m128 _mm_load_max0, _mm_load_max1, _mm_load_max2;
	__m128 _mm_min0, _mm_min1, _mm_min2;
	__m128 _mm_max0, _mm_max1, _mm_max2;
	__m128 _mm_mask;
	mlib_f32 min0, min1, min2, min3;
	mlib_f32 max0, max1, max2, max3;
	mlib_s32 j, k;
	mlib_s32 a = period;
	mlib_s32 b = period * 2;
	mlib_s32 c = period * 3;
	mlib_s32 d = period * 4;

	const __m128 x_zero = _mm_setzero_ps();

	_mm_min0 = _mm_min1 = _mm_min2 = _mm_set1_ps(MLIB_F32_MAX);
	_mm_max0 = _mm_max1 = _mm_max2 = _mm_set1_ps(MLIB_F32_MIN);

	min0 = min1 = min2 = min3 = dmin[0];
	max0 = max1 = max2 = max3 = dmax[0];

	for (j = 0; j < ysize; j++) {
		mlib_f32 *sp = sl;
		mlib_f32 *loopend = sl + xsize;

		for (; sp < (loopend - c); sp += d) {
			_mm_load_min0 = _mm_load_max0 =
			    _mm_setr_ps(sp[0], sp[1], sp[2], sp[a + 0]);
			_mm_load_min1 = _mm_load_max1 = _mm_setr_ps(
			    sp[a + 1], sp[a + 2], sp[b + 0], sp[b + 1]);
			_mm_load_min2 = _mm_load_max2 = _mm_setr_ps(
			    sp[b + 2], sp[c + 0], sp[c + 1], sp[c + 2]);

			MLIB_S_IMAGE_MAXIMUM_F32(_mm_max0, _mm_max0,
			    _mm_load_max0);
			MLIB_S_IMAGE_MINIMUM_F32(_mm_min0, _mm_min0,
			    _mm_load_max0);
			MLIB_S_IMAGE_MAXIMUM_F32(_mm_max1, _mm_max1,
			    _mm_load_max1);
			MLIB_S_IMAGE_MINIMUM_F32(_mm_min1, _mm_min1,
			    _mm_load_max1);
			MLIB_S_IMAGE_MAXIMUM_F32(_mm_max2, _mm_max2,
			    _mm_load_max2);
			MLIB_S_IMAGE_MINIMUM_F32(_mm_min2, _mm_min2,
			    _mm_load_max2);
		}

		mlib_f32 stmax[9], stmin[9];
		for (k = 0; k < 9; k++) {
			stmin[k] = MLIB_F32_MAX;
			stmax[k] = MLIB_F32_MIN;
		}

		for (k = 0; sp < loopend; k += 3, sp += period) {
			stmin[k] = stmax[k] = *sp;
			stmin[k + 1] = stmax[k + 1] = *(sp + 1);
			stmin[k + 2] = stmax[k + 2] = *(sp + 2);
		}

		_mm_load_min0 =
		    _mm_setr_ps(stmin[0], stmin[1], stmin[2], stmin[3]);
		_mm_load_min1 =
		    _mm_setr_ps(stmin[4], stmin[5], stmin[6], stmin[7]);
		_mm_load_min2 = _mm_setr_ps(stmin[8], MLIB_F32_MAX,
		    MLIB_F32_MAX, MLIB_F32_MAX);

		_mm_load_max0 =
		    _mm_setr_ps(stmax[0], stmax[1], stmax[2], stmax[3]);
		_mm_load_max1 =
		    _mm_setr_ps(stmax[4], stmax[5], stmax[6], stmax[7]);
		_mm_load_max2 = _mm_setr_ps(stmax[8], MLIB_F32_MIN,
		    MLIB_F32_MIN, MLIB_F32_MIN);

		MLIB_S_IMAGE_MAXIMUM_F32(_mm_max0, _mm_max0, _mm_load_max0);
		MLIB_S_IMAGE_MINIMUM_F32(_mm_min0, _mm_min0, _mm_load_min0);
		MLIB_S_IMAGE_MAXIMUM_F32(_mm_max1, _mm_max1, _mm_load_max1);
		MLIB_S_IMAGE_MINIMUM_F32(_mm_min1, _mm_min1, _mm_load_min1);
		MLIB_S_IMAGE_MAXIMUM_F32(_mm_max2, _mm_max2, _mm_load_max2);
		MLIB_S_IMAGE_MINIMUM_F32(_mm_min2, _mm_min2, _mm_load_min2);
		sl += slb;
	}

	__m128 s128min0, s128min1, s128min2;
	__m128 s128max0, s128max1, s128max2;

	mlib_f32 *s32min0 = (mlib_f32 *)&s128min0;
	mlib_f32 *s32min1 = (mlib_f32 *)&s128min1;
	mlib_f32 *s32min2 = (mlib_f32 *)&s128min2;

	mlib_f32 *s32max0 = (mlib_f32 *)&s128max0;
	mlib_f32 *s32max1 = (mlib_f32 *)&s128max1;
	mlib_f32 *s32max2 = (mlib_f32 *)&s128max2;

	_mm_store_ps((void *)s32max0, _mm_max0);
	_mm_store_ps((void *)s32max1, _mm_max1);
	_mm_store_ps((void *)s32max2, _mm_max2);
	_mm_store_ps((void *)s32min0, _mm_min0);
	_mm_store_ps((void *)s32min1, _mm_min1);
	_mm_store_ps((void *)s32min2, _mm_min2);

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
mlib_s_ImageExtrema2_4_f32(
    mlib_f32 *dmin,
    mlib_f32 *dmax,
    mlib_f32 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 slb,
    mlib_s32 off1,
    mlib_s32 off2,
    mlib_s32 off3,
    mlib_s32 off4)
{
	__m128 _mm_load_min;
	__m128 _mm_load_max;
	__m128 _mm_min;
	__m128 _mm_max;
	mlib_s32 j, k;

	_mm_min = _mm_set1_ps(MLIB_F32_MAX);
	_mm_max = _mm_set1_ps(MLIB_F32_MIN);

	const __m128 x_zero = _mm_setzero_ps();

	for (j = 0; j < ysize; j++) {
		mlib_f32 *sp = sl;
		mlib_f32 *send = sl + xsize;

		for (k = 0; sp < (send - off3); k++, sp += off4) {
			_mm_load_min = _mm_load_max =
			    _mm_setr_ps(sp[0], sp[off1], sp[off2], sp[off3]);

			MLIB_S_IMAGE_MAXIMUM_F32(_mm_max, _mm_max,
			    _mm_load_max);
			MLIB_S_IMAGE_MINIMUM_F32(_mm_min, _mm_min,
			    _mm_load_min);
		}

		sl += slb;
	}

	_mm_storeu_ps((void *)dmax, _mm_max);
	_mm_storeu_ps((void *)dmin, _mm_min);
}

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
	mlib_f32 fmin[4], fmax[4];

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
			mlib_s_ImageExtrema2_4_d64(dmin, dmax, sl, xsize, ysize,
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
			mlib_s_ImageExtrema2_4_d64(dmin, dmax, sl, xsize, ysize,
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
			mlib_s_ImageExtrema2_3_d64(min, max, sl, xsize, ysize,
			    slb, 3 * xPeriod);
			break;

		case 4:
			mlib_s_ImageExtrema2_4_d64(min, max, sl, xsize, ysize,
			    slb, 1, 2, 3, 4 * xPeriod);
			break;

		default:
			return (MLIB_FAILURE);
		}

	} else {

		fmin[0] = MLIB_F32_MAX;
		fmax[0] = MLIB_F32_MIN;

		switch (nchan) {
		case 1:
			mlib_s_ImageExtrema2_1_f32(fmin, fmax, sl, xsize, ysize,
			    slb, xPeriod, 2 * xPeriod, 3 * xPeriod,
			    4 * xPeriod);
			MIN(fmin[0], fmin[1]);
			MAX(fmax[0], fmax[1]);
			MIN(fmin[0], fmin[2]);
			MAX(fmax[0], fmax[2]);
			MIN(fmin[0], fmin[3]);
			MAX(fmax[0], fmax[3]);
			min[0] = fmin[0];
			max[0] = fmax[0];
			break;

		case 2:
			mlib_s_ImageExtrema2_2_f32(fmin, fmax, sl, xsize, ysize,
			    slb, 1, 2 * xPeriod, 2 * xPeriod + 1, 4 * xPeriod);
			MIN(fmin[0], fmin[2]);
			MAX(fmax[0], fmax[2]);
			MIN(fmin[1], fmin[3]);
			MAX(fmax[1], fmax[3]);
			min[0] = fmin[0];
			max[0] = fmax[0];
			min[1] = fmin[1];
			max[1] = fmax[1];
			break;

		case 3:
			mlib_s_ImageExtrema2_3_f32(min, max, sl, xsize, ysize,
			    slb, 3 * xPeriod);
			break;

		case 4:
			mlib_s_ImageExtrema2_4_f32(fmin, fmax, sl, xsize, ysize,
			    slb, 1, 2, 3, 4 * xPeriod);
			min[0] = fmin[0];
			min[1] = fmin[1];
			min[2] = fmin[2];
			min[3] = fmin[3];
			max[0] = fmax[0];
			max[1] = fmax[1];
			max[2] = fmax[2];
			max[3] = fmax[3];
			break;

		default:
			return (MLIB_FAILURE);
		}

	};

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
