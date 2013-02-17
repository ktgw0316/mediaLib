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

#pragma ident	"@(#)mlib_s_SignalLimit_Fp.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalLimit_[F32|F32S]_[F32|F32S],
 *      mlib_SignalLimit_[F32|F32S]             - hard limiting
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalLimit_F32(mlib_f32       *data,
 *                                       const mlib_f32 *low,
 *                                       const mlib_f32 *high,
 *                                       mlib_s32       n);
 *      mlib_status mlib_SignalLimit_F32S(mlib_f32       *data,
 *                                        const mlib_f32 *low,
 *                                        const mlib_f32 *high,
 *                                        mlib_s32       n);
 *      mlib_status mlib_SignalLimit_F32S_F32S(mlib_f32       *dst,
 *                                             const mlib_f32 *src,
 *                                             const mlib_f32 *low,
 *                                             const mlib_f32 *high,
 *                                             mlib_s32       n);
 *      mlib_status mlib_SignalLimit_F32_F32(mlib_f32       *dst,
 *                                           const mlib_f32 *src,
 *                                           const mlib_f32 *low,
 *                                           const mlib_f32 *high,
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
 *      dst[i] = { low    , if src[i] < low
 *                 src[i] , if low <= src[i] < high
 *                 high   , if src[i] >= high
 *               }
 *
 */

#ifdef	__SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif
#include <mlib_signal.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalLimit_F32 = __mlib_SignalLimit_F32
#pragma weak mlib_SignalLimit_F32S = __mlib_SignalLimit_F32S
#pragma weak mlib_SignalLimit_F32S_F32S = __mlib_SignalLimit_F32S_F32S
#pragma weak mlib_SignalLimit_F32_F32 = __mlib_SignalLimit_F32_F32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalLimit_F32) mlib_SignalLimit_F32
    __attribute__((weak, alias("__mlib_SignalLimit_F32")));
__typeof__(__mlib_SignalLimit_F32S) mlib_SignalLimit_F32S
    __attribute__((weak, alias("__mlib_SignalLimit_F32S")));
__typeof__(__mlib_SignalLimit_F32S_F32S) mlib_SignalLimit_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalLimit_F32S_F32S")));
__typeof__(__mlib_SignalLimit_F32_F32) mlib_SignalLimit_F32_F32
    __attribute__((weak, alias("__mlib_SignalLimit_F32_F32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_SignalLimit_F32_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    const mlib_f32 *low,
    const mlib_f32 *high,
    mlib_s32 n)
{
	mlib_s32 i, count;
	mlib_f32 tl0;
	mlib_f32 th0;
	mlib_f32 x;
	__m128 tl0_p;
	__m128 th0_p;
	__m128 dx;
	mlib_f32 *psrc, *pdst;
	__m128 *spsrc, *spdst;

	tl0 = low[0];
	th0 = high[0];

	if ((tl0 > th0) || (n <= 0))
		return (MLIB_FAILURE);

	tl0_p = _mm_set1_ps(tl0);
	th0_p = _mm_set1_ps(th0);

	psrc = (mlib_f32 *)src;
	pdst = (mlib_f32 *)dst;

	count = (16 - ((mlib_addr)psrc & 15)) >> 2;
	if (count > n) count = n;

	for (i = 0; i < count; i++) {
		x = psrc[i];
		x = (x < tl0) ? tl0 : x;
		x = (x >= th0) ? th0 : x;
		pdst[i] = x;
	}

	n -= count;
	psrc += count;
	pdst += count;
	spsrc = (__m128 *)psrc;
	spdst = (__m128 *)pdst;

	if ((mlib_addr)spdst & 15) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 2; i++) {
			dx = _mm_load_ps((mlib_f32 *)(spsrc + i));
			dx = _mm_max_ps(dx, tl0_p);
			dx = _mm_min_ps(dx, th0_p);
			_mm_storeu_ps((mlib_f32 *)(spdst + i), dx);
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 2; i++) {
			dx = _mm_load_ps((mlib_f32 *)(spsrc + i));
			dx = _mm_max_ps(dx, tl0_p);
			dx = _mm_min_ps(dx, th0_p);
			_mm_store_ps((mlib_f32 *)(spdst + i), dx);
		}
	}

	i <<= 2;

	for (; i < n; i++) {
		x = psrc[i];
		x = (x < tl0) ? tl0 : x;
		x = (x >= th0) ? th0 : x;
		pdst[i] = x;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLimit_F32(
    mlib_f32 *data,
    const mlib_f32 *low,
    const mlib_f32 *high,
    mlib_s32 n)
{
	return (__mlib_SignalLimit_F32_F32(data, data, low, high, n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalLimit_F32S_F32S(
    mlib_f32 *dst,
    const mlib_f32 *src,
    const mlib_f32 *low,
    const mlib_f32 *high,
    mlib_s32 n)
{
	mlib_s32 i, count;
	mlib_f32 tl0, tl1;
	mlib_f32 th0, th1;
	mlib_f32 x, x1;
	__m128 t_low, t_high;
	__m128 dx;
	mlib_f32 *psrc = (mlib_f32 *)src;
	mlib_f32 *pdst = (mlib_f32 *)dst;
	mlib_s32 samples = 2 * n;

	tl0 = low[0];
	th0 = high[0];
	tl1 = low[1];
	th1 = high[1];

	if ((tl0 > th0) || (tl1 > th1) || (n <= 0))
		return (MLIB_FAILURE);

	count = (16 - ((mlib_addr)psrc & 15)) >> 2;
	if (count > samples) count = samples;

	for (i = 0; i < count - 1; i += 2) {
		x = (*psrc++);
		x = (x < tl0) ? tl0 : x;
		x = (x >= th0) ? th0 : x;
		(*pdst++) = x;
		x1 = (*psrc++);
		x1 = (x1 < tl1) ? tl1 : x1;
		x1 = (x1 >= th1) ? th1 : x1;
		(*pdst++) = x1;
	}

	if (count & 1) {
		x = (*psrc++);
		x = (x < tl0) ? tl0 : x;
		x = (x >= th0) ? th0 : x;
		(*pdst++) = x;
	}

	if (count & 1) {
		t_low = _mm_setr_ps(tl1, tl0, tl1, tl0);
		t_high = _mm_setr_ps(th1, th0, th1, th0);
	} else {
		t_low = _mm_setr_ps(tl0, tl1, tl0, tl1);
		t_high = _mm_setr_ps(th0, th1, th0, th1);
	}

	samples -= count;

	if ((mlib_addr)pdst & 15) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < samples >> 2; i++) {
			dx = _mm_load_ps(psrc + 4 * i);
			dx = _mm_max_ps(dx, t_low);
			dx = _mm_min_ps(dx, t_high);
			_mm_storeu_ps(pdst + 4 * i, dx);
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < samples >> 2; i++) {
			dx = _mm_load_ps(psrc + 4 * i);
			dx = _mm_max_ps(dx, t_low);
			dx = _mm_min_ps(dx, t_high);
			_mm_store_ps(pdst + 4 * i, dx);
		}
	}

	i <<= 2;
	psrc += i;
	pdst += i;

	if (count & 1 && i < samples) {
		x = (*psrc++);
		x = (x < tl1) ? tl1 : x;
		x = (x >= th1) ? th1 : x;
		(*pdst++) = x;

		i ++;
	}

	for (; i < samples - 1; i += 2) {
		x = (*psrc++);
		x = (x < tl0) ? tl0 : x;
		x = (x >= th0) ? th0 : x;
		(*pdst++) = x;
		x1 = (*psrc++);
		x1 = (x1 < tl1) ? tl1 : x1;
		x1 = (x1 >= th1) ? th1 : x1;
		(*pdst++) = x1;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLimit_F32S(
    mlib_f32 *data,
    const mlib_f32 *low,
    const mlib_f32 *high,
    mlib_s32 n)
{
	return (__mlib_SignalLimit_F32S_F32S(data, data, low, high, n));
}

/* *********************************************************** */
