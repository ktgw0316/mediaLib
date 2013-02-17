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

#pragma ident	"@(#)mlib_s_SignalDownSample_Fp.c	9.5	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalDownSample_[F32|F32S]_[F32|F32S] - downsampling
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalDownSample_F32S_F32S(mlib_f32       *dst,
 *                                                  const mlib_f32 *src,
 *                                                  mlib_s32       factor,
 *                                                  mlib_s32       phase,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_SignalDownSample_F32_F32(mlib_f32       *dst,
 *                                                const mlib_f32 *src,
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
 *              = x(factor*k + phase), n = 0, 1, ...
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

#pragma weak mlib_SignalDownSample_F32S_F32S = \
	__mlib_SignalDownSample_F32S_F32S
#pragma weak mlib_SignalDownSample_F32_F32 = __mlib_SignalDownSample_F32_F32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalDownSample_F32S_F32S) mlib_SignalDownSample_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalDownSample_F32S_F32S")));
__typeof__(__mlib_SignalDownSample_F32_F32) mlib_SignalDownSample_F32_F32
    __attribute__((weak, alias("__mlib_SignalDownSample_F32_F32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_SignalDownSample_F32_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 factor,
    mlib_s32 phase,
    mlib_s32 n)
{
	__m128i dDst;
	__m128i *spDst;
	mlib_s32 offset = phase;
	mlib_s32 count, samples;
	mlib_s32 k, j;
	mlib_s32 *isrc = (mlib_s32 *)src;

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
		dst[k] = src[offset];
	}

	samples -= count;
	dst += count;
	spDst = (__m128i *)dst;

	if ((mlib_addr)spDst & 15) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (k = 0; k < samples >> 2; k++, offset += 4 * factor) {
			dDst = _mm_setr_epi32(isrc[offset],
					isrc[offset + factor],
					isrc[offset + 2 * factor],
					isrc[offset + 3 * factor]);
			_mm_storeu_si128(spDst + k, dDst);
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (k = 0; k < samples >> 2; k++, offset += 4 * factor) {
			dDst = _mm_setr_epi32(isrc[offset],
					isrc[offset + factor],
					isrc[offset + 2 * factor],
					isrc[offset + 3 * factor]);
			_mm_store_si128(spDst + k, dDst);
		}
	}

	k <<= 2;

	for (; k < samples; k++, offset += factor) {
		dst[k] = src[offset];
	}

#else

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (k = 0; k < samples; k++, offset += factor) {
		dst[k] = src[offset];
	}

#endif

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_SignalDownSample_F32S_F32S(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 factor,
    mlib_s32 phase,
    mlib_s32 n)
{
	__m128i dDst;
	__m128i *spDst;
	mlib_s32 offset = phase;
	mlib_s32 count, samples;
	mlib_s32 k, j;
	__m64 *isrc = (__m64 *)src;

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
 * since the implementation of _mm_setr_epi64() in Sun compiler (Intel compiler
 * as well) is not the best: data is loaded from memory to xmm registers
 * via GPRs while moving from GPRs to xmm registers is slow
 */

#if 0

	count = (16 - (mlib_addr)dst & 15) >> 3;
	if (count > samples) count = samples;

	for (k = 0; k < count; k++, offset += factor) {
		dst[2 * k] = src[2 * offset];
		dst[2 * k + 1] = src[2 * offset + 1];
	}

	samples -= count;
	dst += 2 * count;
	spDst = (__m128i *)dst;

	if ((mlib_addr)spDst & 15) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (k = 0; k < samples >> 1; k++, offset += 2 * factor) {
			dDst = _mm_setr_epi64(isrc[offset],
					isrc[offset + factor]);
			_mm_storeu_si128(spDst + k, dDst);
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (k = 0; k < samples >> 1; k++, offset += 2 * factor) {
			dDst = _mm_setr_epi64(isrc[offset],
					isrc[offset + factor]);
			_mm_store_si128(spDst + k, dDst);
		}
	}

	k <<= 1;

	for (; k < samples; k++, offset += factor) {
		dst[2 * k] = src[2 * offset];
		dst[2 * k + 1] = src[2 * offset + 1];
	}

	_mm_empty();

#else

	mlib_u64 *pSrc = (mlib_u64 *)src;
	mlib_u64 *pDst = (mlib_u64 *)dst;

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
