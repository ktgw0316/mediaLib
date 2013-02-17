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

#pragma ident	"@(#)mlib_i_SignalDownSample.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalDownSample_[S16|S16S]_[S16|S16S] - downsampling
 *
 * SYNOPSIS
 *
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
 *
 */

#include <mlib_signal.h>

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
	mlib_u16 *pSrc = (mlib_u16 *)src, *pDst = (mlib_u16 *)dst;
	mlib_s32 twoFactor = 2 * factor;
	mlib_s32 threeFactor = 3 * factor;
	mlib_s32 offset = phase;
	mlib_s32 *spDst;
	mlib_u32 dDst0, dDst1;
	mlib_u16 uSrc0, uSrc1;
	mlib_s32 align, times, samples;
	mlib_s32 i, k;

/* Check for obvious errors */

	if (src == NULL || dst == NULL)
		return (MLIB_NULLPOINTER);

	if ((phase < 0) || (factor <= 0) || (phase >= factor))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

	samples = ((n - phase + factor - 1) / factor);

	align = (mlib_addr)pDst & 3;

	for (k = 0; k < samples && align != 0; k++, offset += factor) {
		dst[k] = src[offset];
		pDst++;
		align = (mlib_addr)pDst & 3;
	}

	times = (samples - k) > 0 ? (samples - k) >> 1 : 0;
	spDst = (mlib_s32 *)((mlib_addr)pDst & ~3);

	uSrc0 = pSrc[offset];
	uSrc1 = pSrc[offset + factor];

#ifdef _LITTLE_ENDIAN

	dDst0 = uSrc0;

	for (i = 0; i < times - 1; i++, offset += twoFactor) {
		dDst0 |= (uSrc1 << MLIB_SHIFT);
		uSrc0 = pSrc[offset + twoFactor];
		uSrc1 = pSrc[offset + threeFactor];
		dDst1 = uSrc0;

		spDst[i] = dDst0;

		dDst0 = dDst1;
	}

	if (times > 0) {
		dDst0 |= (uSrc1 << MLIB_SHIFT);
		spDst[i] = dDst0;
		offset += twoFactor;
	}
#else /* _LITTLE_ENDIAN */

	dDst0 = uSrc0 << MLIB_SHIFT;

	for (i = 0; i < times - 1; i++, offset += twoFactor) {
		dDst0 |= uSrc1;
		uSrc0 = pSrc[offset + twoFactor];
		uSrc1 = pSrc[offset + threeFactor];
		dDst1 = uSrc0 << MLIB_SHIFT;

		spDst[i] = dDst0;

		dDst0 = dDst1;
	}

	if (times > 0) {
		dDst0 |= uSrc1;
		spDst[i] = dDst0;
		offset += twoFactor;
	}
#endif /* _LITTLE_ENDIAN */

	k = k + 2 * times;

	for (; k < samples; k++, offset += factor)
		dst[k] = src[offset];

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
	mlib_s32 srcAlign, dstAlign;
	mlib_s32 samples;

/* Check for obvious errors */

	if (src == NULL || dst == NULL)
		return (MLIB_NULLPOINTER);

	if ((phase < 0) || (factor <= 0) || (phase >= factor))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

	srcAlign = (mlib_addr)src & 3;
	dstAlign = (mlib_addr)dst & 3;

	samples = ((n - phase + factor - 1) / factor);

	if (srcAlign == 0 && dstAlign == 0) {
		mlib_u32 *pSrc = (mlib_u32 *)src;
		mlib_u32 *pDst = (mlib_u32 *)dst;
		mlib_s32 offset = phase;
		mlib_s32 k;

		for (k = 0; k < samples; k++, offset += factor)
			pDst[k] = pSrc[offset];
	} else {
		mlib_u16 *pSrc = (mlib_u16 *)src, *pDst = (mlib_u16 *)dst;
		mlib_s32 twoFactor = 2 * factor;
		mlib_s32 offset = 2 * phase;
		mlib_s32 dDst0, dDst1;
		mlib_u16 uSrc0, uSrc1;
		mlib_s32 k;

		if (dstAlign == 0) {
			mlib_s32 *spDst = (mlib_s32 *)pDst;

			uSrc0 = pSrc[offset];
			uSrc1 = pSrc[offset + 1];

#ifdef _LITTLE_ENDIAN

			dDst0 = uSrc0;

			for (k = 0; k < samples; k++, offset += twoFactor) {
				dDst0 |= (uSrc1 << MLIB_SHIFT);
				uSrc0 = pSrc[offset + twoFactor];
				uSrc1 = pSrc[offset + twoFactor + 1];
				dDst1 = uSrc0;

				spDst[k] = dDst0;
				dDst0 = dDst1;
			}

#else /* _LITTLE_ENDIAN */
			dDst0 = uSrc0 << MLIB_SHIFT;

			for (k = 0; k < samples; k++, offset += twoFactor) {
				dDst0 |= uSrc1;
				uSrc0 = pSrc[offset + twoFactor];
				uSrc1 = pSrc[offset + twoFactor + 1];
				dDst1 = uSrc0 << MLIB_SHIFT;

				spDst[k] = dDst0;
				dDst0 = dDst1;
			}

#endif /* _LITTLE_ENDIAN */
		} else if (samples != 0) {
			mlib_s32 *spDst = (mlib_s32 *)(pDst + 1);
			mlib_s32 fourFactor = 4 * factor;
			mlib_s32 times = samples - 1;

			dst[0] = src[offset];

			uSrc0 = pSrc[offset + 1];
			uSrc1 = pSrc[offset + twoFactor];

#ifdef _LITTLE_ENDIAN

			dDst0 = uSrc0;

			for (k = 0; k < times; k++, offset += twoFactor) {
				dDst0 |= (uSrc1 << MLIB_SHIFT);
				uSrc0 = pSrc[offset + twoFactor + 1];
				uSrc1 = pSrc[offset + fourFactor];
				dDst1 = uSrc0;

				spDst[k] = dDst0;
				dDst0 = dDst1;
			}

#else /* _LITTLE_ENDIAN */
			dDst0 = uSrc0 << MLIB_SHIFT;

			for (k = 0; k < times; k++, offset += twoFactor) {
				dDst0 |= uSrc1;
				uSrc0 = pSrc[offset + twoFactor + 1];
				uSrc1 = pSrc[offset + fourFactor];
				dDst1 = uSrc0 << MLIB_SHIFT;

				spDst[k] = dDst0;
				dDst0 = dDst1;
			}

#endif /* _LITTLE_ENDIAN */

			dst[2 * samples - 1] = src[offset + 1];
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
