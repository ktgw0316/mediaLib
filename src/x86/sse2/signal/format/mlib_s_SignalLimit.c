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

#pragma ident	"@(#)mlib_s_SignalLimit.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalLimit_[S16|S16S]_[S16|S16S],
 *      mlib_SignalLimit_[S16|S16S]             - hard limiting
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalLimit_S16(mlib_s16       *data,
 *                                       const mlib_s16 *low,
 *                                       const mlib_s16 *high,
 *                                       mlib_s32       n);
 *      mlib_status mlib_SignalLimit_S16S(mlib_s16       *data,
 *                                        const mlib_s16 *low,
 *                                        const mlib_s16 *high,
 *                                        mlib_s32       n);
 *      mlib_status mlib_SignalLimit_S16S_S16S(mlib_s16       *dst,
 *                                             const mlib_s16 *src,
 *                                             const mlib_s16 *low,
 *                                             const mlib_s16 *high,
 *                                             mlib_s32       n);
 *      mlib_status mlib_SignalLimit_S16_S16(mlib_s16       *dst,
 *                                           const mlib_s16 *src,
 *                                           const mlib_s16 *low,
 *                                           const mlib_s16 *high,
 *                                           mlib_s32       n);
 * ARGUMENTS
 *      data  Input and output signal array
 *      dst   Output signal array
 *      src   Input signal array
 *      low   Lower input limit
 *      high  Upper input limit
 *      n     Number of samples in the input signal arrays.
 *
 * DESCRIPTION
 *      dst[i] = { t      , if src[i] < t
 *                  low                  low
 *                 src[i] , if t <= src[i] < t
 *                              low           high
 *                 t      , if src[i] >= t
 *                  high                  high
 *                }
 *
 */

#include <mlib_signal.h>
#ifdef	__SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalLimit_S16 = __mlib_SignalLimit_S16
#pragma weak mlib_SignalLimit_S16S = __mlib_SignalLimit_S16S
#pragma weak mlib_SignalLimit_S16S_S16S = __mlib_SignalLimit_S16S_S16S
#pragma weak mlib_SignalLimit_S16_S16 = __mlib_SignalLimit_S16_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalLimit_S16) mlib_SignalLimit_S16
    __attribute__((weak, alias("__mlib_SignalLimit_S16")));
__typeof__(__mlib_SignalLimit_S16S) mlib_SignalLimit_S16S
    __attribute__((weak, alias("__mlib_SignalLimit_S16S")));
__typeof__(__mlib_SignalLimit_S16S_S16S) mlib_SignalLimit_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalLimit_S16S_S16S")));
__typeof__(__mlib_SignalLimit_S16_S16) mlib_SignalLimit_S16_S16
    __attribute__((weak, alias("__mlib_SignalLimit_S16_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	LIMIT_S16                                               \
	mlib_s32 i;                                             \
	mlib_s16 t_low = *low;                                  \
	mlib_s16 t_high = *high;                                \
	mlib_s16 x, x1;                                         \
	                                                        \
	if ((t_low > t_high) || (n <= 0))                       \
	    return (MLIB_FAILURE);                              \
	if (i = n & 1) {                                        \
	    x = src[0];                                         \
	    if (x < t_low)                                      \
		x = t_low;                                      \
	    else if (x >= t_high)                               \
		x = t_high;                                     \
	    dst[0] = x;                                         \
	}                                                       \
	for (; i < n; i += 2) {                                 \
	    x = src[i];                                         \
	    if (x < t_low)                                      \
		x = t_low;                                      \
	    else if (x >= t_high)                               \
		x = t_high;                                     \
	    dst[i] = x;                                         \
	    x1 = src[i + 1];                                    \
	    if (x1 < t_low)                                     \
		x1 = t_low;                                     \
	    else if (x1 >= t_high)                              \
		x1 = t_high;                                    \
	    dst[i + 1] = x1;                                    \
	}                                                       \
	return MLIB_SUCCESS

/* *********************************************************** */

#define	LIMIT_S16S                                                      \
	mlib_s32 i;                                                     \
	mlib_s16 t_low_0 = *low;                                        \
	mlib_s16 t_high_0 = *high;                                      \
	mlib_s16 t_low_1 = *(low + 1);                                  \
	mlib_s16 t_high_1 = *(high + 1);                                \
	mlib_s16 x_0, x_1;                                              \
	                                                                \
	if ((t_low_0 > t_high_0) || (t_low_1 > t_high_1) || (n <= 0))   \
	    return (MLIB_FAILURE);                                      \
	for (i = 0; i < n; i++) {                                       \
	    x_0 = src[2 * i];                                           \
	    x_1 = src[2 * i + 1];                                       \
	    if (x_0 < t_low_0)                                          \
		x_0 = t_low_0;                                          \
	    else if (x_0 >= t_high_0)                                   \
		x_0 = t_high_0;                                         \
	    dst[2 * i] = x_0;                                           \
	    if (x_1 < t_low_1)                                          \
		x_1 = t_low_1;                                          \
	    else if (x_1 >= t_high_1)                                   \
		x_1 = t_high_1;                                         \
	    dst[2 * i + 1] = x_1;                                       \
	}                                                               \
	                                                                \
	return MLIB_SUCCESS

/* *********************************************************** */

mlib_status
__mlib_SignalLimit_S16_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *low,
    const mlib_s16 *high,
    mlib_s32 n)
{
	__m128i t_low;
	__m128i t_high;
	__m128i dx;
	__m128i *spsrc, *spdst;
	mlib_s16 *psrc, *pdst;
	mlib_s32 i, count;
	mlib_s16 x;
	mlib_s16 t_low_s = *low;
	mlib_s16 t_high_s = *high;
	psrc = (mlib_s16 *)src;
	pdst = (mlib_s16 *)dst;
	t_low = _mm_set1_epi16(t_low_s);
	t_high = _mm_set1_epi16(t_high_s);

	if ((t_low_s > t_high_s) || (n <= 0))
	    return (MLIB_FAILURE);

	count = (16 - ((mlib_addr)psrc & 15)) >> 1;
	if (count > n) count = n;

	for (i = 0; i < count; i++) {
		x = psrc[i];
		if (x < t_low_s) {
			x = t_low_s;
		} else if (x >= t_high_s) {
			x = t_high_s;
		}
		pdst[i] = x;
	}

	n -= count;
	psrc += count;
	pdst += count;
	spsrc = (__m128i *)psrc;
	spdst = (__m128i *)pdst;

	if ((mlib_addr)spdst & 15) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 3; i++) {
			dx = _mm_load_si128(spsrc + i);
			dx = _mm_max_epi16(dx, t_low);
			dx = _mm_min_epi16(dx, t_high);
			_mm_storeu_si128(spdst + i, dx);
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 3; i++) {
			dx = _mm_load_si128(spsrc + i);
			dx = _mm_max_epi16(dx, t_low);
			dx = _mm_min_epi16(dx, t_high);
			_mm_store_si128(spdst + i, dx);
		}
	}

	i <<= 3;

	for (; i < n; i++) {
		x = psrc[i];
		if (x < t_low_s) {
			x = t_low_s;
		} else if (x >= t_high_s) {
			x = t_high_s;
		}
		pdst[i] = x;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLimit_S16(
    mlib_s16 *data,
    const mlib_s16 *low,
    const mlib_s16 *high,
    mlib_s32 n)
{
	return (__mlib_SignalLimit_S16_S16(data, data, low, high, n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalLimit_S16S_S16S(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *low,
    const mlib_s16 *high,
    mlib_s32 n)
{
	__m128i t_low;
	__m128i t_high;
	__m128i dx;
	__m128i *spsrc, *spdst;
	mlib_s16 *psrc, *pdst;
	mlib_s32 i, count;
	mlib_s16 x0, x1;
	mlib_s16 t_low_0 = *low;
	mlib_s16 t_high_0 = *high;
	mlib_s16 t_low_1 = *(low + 1);
	mlib_s16 t_high_1 = *(high + 1);
	mlib_s32 samples = 2 * n;
	psrc = (mlib_s16 *)src;
	pdst = (mlib_s16 *)dst;

	if ((t_low_0 > t_high_0) || (t_low_1 > t_high_1) || (n <= 0))
	    return (MLIB_FAILURE);

	count = (16 - ((mlib_addr)psrc & 15)) >> 1;
	if (count > samples) count = samples;

	for (i = 0; i < count - 1; i += 2) {
		x0 = psrc[i];
		x1 = psrc[i + 1];
		if (x0 < t_low_0) {
			x0 = t_low_0;
		} else if (x0 >= t_high_0) {
			x0 = t_high_0;
		}
		pdst[i] = x0;
		if (x1 < t_low_1) {
			x1 = t_low_1;
		} else if (x1 >= t_high_1) {
			x1 = t_high_1;
		}
		pdst[i + 1] = x1;
	}

	if (count & 1) {
		x0 = psrc[count - 1];
		if (x0 < t_low_0) {
			x0 = t_low_0;
		} else if (x0 >= t_high_0) {
			x0 = t_high_0;
		}
		pdst[count - 1] = x0;
	}

	samples -= count;
	psrc += count;
	pdst += count;
	spsrc = (__m128i *)psrc;
	spdst = (__m128i *)pdst;

	if (count & 1) {
		t_low = _mm_setr_epi16(t_low_1, t_low_0, t_low_1, t_low_0,
			t_low_1, t_low_0, t_low_1, t_low_0);
		t_high = _mm_setr_epi16(t_high_1, t_high_0, t_high_1, t_high_0,
			t_high_1, t_high_0, t_high_1, t_high_0);
	} else {
		t_low = _mm_setr_epi16(t_low_0, t_low_1, t_low_0, t_low_1,
			t_low_0, t_low_1, t_low_0, t_low_1);
		t_high = _mm_setr_epi16(t_high_0, t_high_1, t_high_0, t_high_1,
			t_high_0, t_high_1, t_high_0, t_high_1);
	}

	if ((mlib_addr)spdst & 15) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < samples >> 3; i++) {
			dx = _mm_load_si128(spsrc + i);
			dx = _mm_max_epi16(dx, t_low);
			dx = _mm_min_epi16(dx, t_high);
			_mm_storeu_si128(spdst + i, dx);
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < samples >> 3; i++) {
			dx = _mm_load_si128(spsrc + i);
			dx = _mm_max_epi16(dx, t_low);
			dx = _mm_min_epi16(dx, t_high);
			_mm_store_si128(spdst + i, dx);
		}
	}

	i <<= 3;

	if (count & 1 && i < samples) {
		x1 = psrc[i];
		if (x1 < t_low_1) {
			x1 = t_low_1;
		} else if (x1 >= t_high_1) {
			x1 = t_high_1;
		}
		pdst[i] = x1;

		i ++;
	}

	for (; i < samples - 1; i += 2) {
		x0 = psrc[i];
		x1 = psrc[i + 1];
		if (x0 < t_low_0) {
			x0 = t_low_0;
		} else if (x0 >= t_high_0) {
			x0 = t_high_0;
		}
		pdst[i] = x0;
		if (x1 < t_low_1) {
			x1 = t_low_1;
		} else if (x1 >= t_high_1) {
			x1 = t_high_1;
		}
		pdst[i + 1] = x1;
	}

	return (MLIB_SUCCESS);
}


/* *********************************************************** */

mlib_status
__mlib_SignalLimit_S16S(
    mlib_s16 *data,
    const mlib_s16 *low,
    const mlib_s16 *high,
    mlib_s32 n)
{
	return (__mlib_SignalLimit_S16S_S16S(data, data, low, high, n));
}

/* *********************************************************** */
