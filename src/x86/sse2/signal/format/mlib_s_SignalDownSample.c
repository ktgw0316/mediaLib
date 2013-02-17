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

#pragma ident	"@(#)mlib_s_SignalDownSample.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalDownSample_[S16|S16S]_[S16|S16S] - downsampling
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalDownSample_S16S_S16S(mlib_s16       *dst,
 *                                                  const mlib_s16 *src,
 *                                                  mlib_s32       factor,
 *                                                  mlib_s32       phase,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_SignalDownSample_S16_S16(mlib_s16       *dst,
 *                                                const mlib_s16 *src,
 *                                                mlib_s32       factor,
 *                                                mlib_s32       phase,
 *                                                mlib_s32       n);
 *
 * ARGUMENTS
 *      dst     Output signal array
 *      src     Input signal array
 *      factor  Factor by which to downsample
 *      phase   Parameter that determines relative position of an input
 *              value, within the output signal. 0 <= phase < factor.
 *      n       Number of samples in the input signal array.
 *
 * DESCRIPTION
 *      X       = x(n), n = 0, 1, ...
 *      Zdown   = z(n), n = 0, 1, ...
 *
 *              = x(factor*n + phase), n = 0, 1, ...
 *
 * COMMENTS
 *      A user should supply input and output vectors
 *      big enough to hold all samples.
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

#pragma weak mlib_SignalDownSample_S16S_S16S = \
	__mlib_SignalDownSample_S16S_S16S
#pragma weak mlib_SignalDownSample_S16_S16 = __mlib_SignalDownSample_S16_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalDownSample_S16S_S16S) mlib_SignalDownSample_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalDownSample_S16S_S16S")));
__typeof__(__mlib_SignalDownSample_S16_S16) mlib_SignalDownSample_S16_S16
    __attribute__((weak, alias("__mlib_SignalDownSample_S16_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_SHIFT	16

/* *********************************************************** */

mlib_status
__mlib_SignalDownSample_S16_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 factor,
    mlib_s32 phase,
    mlib_s32 n)
{
	__m128i dDst;
	__m128i *spDst;
	mlib_s16 *pSrc = (mlib_s16 *)src, *pDst = (mlib_s16 *)dst;
	mlib_s32 Factor1 = 1 * factor;
	mlib_s32 Factor2 = 2 * factor;
	mlib_s32 Factor3 = 3 * factor;
	mlib_s32 Factor4 = 4 * factor;
	mlib_s32 Factor5 = 5 * factor;
	mlib_s32 Factor6 = 6 * factor;
	mlib_s32 Factor7 = 7 * factor;
	mlib_s32 Factor8 = 8 * factor;
	mlib_s32 offset = phase;
	mlib_s32 align, times, samples;
	mlib_s32 i, k;

/* Check for obvious errors */

	if (src == NULL || dst == NULL)
		return (MLIB_NULLPOINTER);

	if ((phase < 0) || (factor <= 0) || (phase >= factor))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

	samples =
	    (mlib_s32)((mlib_d64)(n - phase + factor - 1) / (mlib_d64)factor);

/*
 * SSE2 version is substituted by C version
 * since the implementation of _mm_setr_epi16() in Sun compiler (Intel compiler
 * as well) is not the best: data is loaded from memory to xmm registers
 * via GPRs while moving from GPRs to xmm registers is slow
 */

#if 0

	align = (mlib_addr)pDst & 15;

	for (k = 0; k < samples && align != 0; k++, offset += factor) {
		dst[k] = src[offset];
		pDst++;
		align = (mlib_addr)pDst & 15;
	}

	times = (samples - k) > 0 ? (samples - k) >> 3 : 0;
	spDst = (__m128i *)((mlib_addr)pDst & ~15);

	if ((mlib_addr)spDst & 15) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < times; i++, offset += Factor8) {
			dDst = _mm_setr_epi16(pSrc[offset],
					pSrc[offset + Factor1],
					pSrc[offset + Factor2],
					pSrc[offset + Factor3],
					pSrc[offset + Factor4],
					pSrc[offset + Factor5],
					pSrc[offset + Factor6],
					pSrc[offset + Factor7]);

			_mm_storeu_si128(spDst + i, dDst);
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < times; i++, offset += Factor8) {
			dDst = _mm_setr_epi16(pSrc[offset],
					pSrc[offset + Factor1],
					pSrc[offset + Factor2],
					pSrc[offset + Factor3],
					pSrc[offset + Factor4],
					pSrc[offset + Factor5],
					pSrc[offset + Factor6],
					pSrc[offset + Factor7]);

			_mm_store_si128(spDst + i, dDst);
		}
	}

	k = k + 8 * times;

	for (; k < samples; k++, offset += factor)
		dst[k] = src[offset];

#else

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (k = 0; k < samples; k++, offset += factor)
		dst[k] = src[offset];

#endif

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalDownSample_S16S_S16S(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 factor,
    mlib_s32 phase,
    mlib_s32 n)
{
	__m128i dDst;
	mlib_s32 count;
	mlib_s32 samples;
	mlib_s32 *pSrc = (mlib_s32 *)src;
	mlib_s32 *pDst = (mlib_s32 *)dst;
	__m128i *spDst;
	mlib_s32 offset = phase;
	mlib_s32 k;

/* Check for obvious errors */

	if (src == NULL || dst == NULL)
		return (MLIB_NULLPOINTER);

	if ((phase < 0) || (factor <= 0) || (phase >= factor))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

	samples =
	    (mlib_s32)((mlib_d64)(n - phase + factor - 1) / (mlib_d64)factor);

/*
 * SSE2 version is substituted by C version
 * since the implementation of _mm_setr_epi32() in Sun compiler (Intel compiler
 * as well) is not the best: data is loaded from memory to xmm registers
 * via GPRs while moving from GPRs to xmm registers is slow
 */

#if 0

	count = (16 - (mlib_addr)dst & 15) >> 2;
	if (count > samples) count = samples;


	for (k = 0; k < count; k++, offset += factor) {
		pDst[k] = pSrc[offset];
	}

	samples -= count;
	pDst += count;
	spDst = (__m128i *)pDst;

	if ((mlib_addr)spDst & 15) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (k = 0; k < samples >> 2; k++, offset += 4 * factor) {
			dDst = _mm_setr_epi32(pSrc[offset],
					pSrc[offset + factor],
					pSrc[offset + 2 * factor],
					pSrc[offset + 3 * factor]);
			_mm_storeu_si128(spDst + k, dDst);
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (k = 0; k < samples >> 2; k++, offset += 4 * factor) {
			dDst = _mm_setr_epi32(pSrc[offset],
					pSrc[offset + factor],
					pSrc[offset + 2 * factor],
					pSrc[offset + 3 * factor]);
			_mm_store_si128(spDst + k, dDst);
		}
	}

	k <<= 2;

	for (; k < samples; k++, offset += factor) {
		pDst[k] = pSrc[offset];
	}

#else

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (k = 0; k < samples; k++, offset += factor) {
		pDst[k] = pSrc[offset];
	}

#endif

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
