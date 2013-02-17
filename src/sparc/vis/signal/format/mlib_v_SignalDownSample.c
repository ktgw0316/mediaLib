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

#pragma ident	"@(#)mlib_v_SignalDownSample.c	9.3	07/11/05 SMI"

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

#include <vis_proto.h>
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

mlib_status
__mlib_SignalDownSample_S16_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 factor,
    mlib_s32 phase,
    mlib_s32 n)
{
	mlib_u16 *pSrc = (mlib_u16 *)src;
	mlib_u16 *pDst = (mlib_u16 *)dst;
	mlib_s32 oneFactor = 2 * factor;
	mlib_s32 twoFactor = 2 * 2 * factor;
	mlib_s32 threeFactor = 2 * 3 * factor;
	mlib_s32 fourFactor = 2 * 4 * factor;
	mlib_s32 fiveFactor = 2 * 5 * factor;
	mlib_s32 sixFactor = 2 * 6 * factor;
	mlib_s32 sevenFactor = 2 * 7 * factor;
	mlib_s32 offset = phase;
	mlib_d64 *dpDst, dDst;
	mlib_d64 data0, data1, data2, data3;
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

	align = (mlib_addr)pDst & 7;

	for (k = 0; k < samples && align != 0; k++, offset += factor) {
		dst[k] = src[offset];
		pDst++;
		align = (mlib_addr)pDst & 7;
	}

	times = (samples - k) > 0 ? (samples - k) >> 2 : 0;
	pSrc = pSrc + offset;
	dpDst = (mlib_d64 *)((mlib_addr)pDst & ~7);

	vis_alignaddr(dpDst, 6);

	data0 = vis_ld_u16_i(pSrc, threeFactor);
	data1 = vis_ld_u16_i(pSrc, twoFactor);
	data2 = vis_ld_u16_i(pSrc, oneFactor);
	data3 = vis_ld_u16_i(pSrc, 0);

	dDst = vis_faligndata(data0, data0);

	for (i = 0; i < times; i++, pSrc += twoFactor) {
		data0 = vis_ld_u16_i(pSrc, sevenFactor);
		dDst = vis_faligndata(data1, dDst);
		data1 = vis_ld_u16_i(pSrc, sixFactor);
		dDst = vis_faligndata(data2, dDst);
		data2 = vis_ld_u16_i(pSrc, fiveFactor);
		dDst = vis_faligndata(data3, dDst);
		data3 = vis_ld_u16_i(pSrc, fourFactor);

		dpDst[i] = dDst;

		dDst = vis_faligndata(data0, dDst);
	}

	k = k + 4 * times;
	offset = k * factor + phase;

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

/* Check for obvious errors */

	if (src == NULL || dst == NULL)
		return (MLIB_NULLPOINTER);

	if ((phase < 0) || (factor <= 0) || (phase >= factor))
		return (MLIB_OUTOFRANGE);

	srcAlign = (mlib_addr)src & 3;
	dstAlign = (mlib_addr)dst & 3;

	if ((srcAlign | dstAlign) == 0) {
		return __mlib_SignalDownSample_F32_F32((mlib_f32 *)dst,
		    (mlib_f32 *)src, factor, phase, n);
	}

	if (n <= 0) {
		return (MLIB_FAILURE);
	} else {
		mlib_s32 samples =
		    (mlib_d64)(n - phase + factor - 1) / (mlib_d64)factor;
		mlib_u16 *pDst = (mlib_u16 *)dst, *pSrc;
		mlib_s32 twoN = 2 * samples;
		mlib_s32 twoPhase = 2 * phase;
		mlib_s32 twoFactor = 2 * factor;
		mlib_s32 two2Factor = 2 * twoFactor;
		mlib_s32 four2Factor = 4 * twoFactor;
		mlib_s32 six2Factor = 6 * twoFactor;
		mlib_s32 eight2factor = 8 * twoFactor;
		mlib_s32 offset = twoPhase;
		mlib_s32 i, k;
		mlib_s32 align, times;
		mlib_d64 *dpDst, dDst;
		mlib_d64 data0, data1, data2, data3;

		if (dstAlign == 0) {
			align = ((mlib_addr)pDst & 7);

			for (k = 0; k < twoN && align != 0;
			    k += 2, offset += twoFactor) {
				dst[k] = src[offset];
				dst[k + 1] = src[offset + 1];
				pDst += 2;
				align = ((mlib_addr)pDst & 7);
			}

			dpDst = (mlib_d64 *)((mlib_addr)pDst & ~7);
			pSrc = (mlib_u16 *)src + offset;
			times =
			    ((twoN - k) >> 2) - 1 >
			    0 ? ((twoN - k) >> 2) - 1 : 0;

			vis_alignaddr(dpDst, 6);

			data0 = vis_ld_u16_i(pSrc, two2Factor + 2);
			data1 = vis_ld_u16_i(pSrc, two2Factor);
			data2 = vis_ld_u16_i(pSrc, 2);
			data3 = vis_ld_u16_i(pSrc, 0);

			dDst = vis_faligndata(data0, data0);

			for (i = 0; i < times; i++, pSrc += 2 * twoFactor) {
				data0 = vis_ld_u16_i(pSrc, six2Factor + 2);
				dDst = vis_faligndata(data1, dDst);
				data1 = vis_ld_u16_i(pSrc, six2Factor);
				dDst = vis_faligndata(data2, dDst);
				data2 = vis_ld_u16_i(pSrc, four2Factor + 2);
				dDst = vis_faligndata(data3, dDst);
				data3 = vis_ld_u16_i(pSrc, four2Factor);

				dpDst[i] = dDst;

				dDst = vis_faligndata(data0, dDst);
			}

			k = k + 4 * times;
			offset = twoPhase + k * factor;

			for (; k < twoN; k += 2, offset += twoFactor) {
				dst[k] = src[offset];
				dst[k + 1] = src[offset + 1];
			}
		} else {
			align = 0;

			for (k = 0; k < twoN && align != 2;
			    k += 2, offset += twoFactor) {
				dst[k] = src[offset];
				dst[k + 1] = src[offset + 1];
				pDst += 2;
				align = ((mlib_addr)pDst & 7);
			}

			dpDst = (mlib_d64 *)(((mlib_addr)pDst - 2) & ~7);
			pSrc = (mlib_u16 *)src + (offset - twoFactor);
			times =
			    ((twoN - k) >> 2) - 1 >
			    0 ? ((twoN - k) >> 2) - 1 : 0;

			vis_alignaddr(dpDst, 6);

			data0 = vis_ld_u16_i(pSrc, four2Factor);
			data1 = vis_ld_u16_i(pSrc, two2Factor + 2);
			data2 = vis_ld_u16_i(pSrc, two2Factor);
			data3 = vis_ld_u16_i(pSrc, 2);

			dDst = vis_faligndata(data0, data0);

			for (i = 0; i < times; i++, pSrc += 2 * twoFactor) {
				data0 = vis_ld_u16_i(pSrc, eight2factor);
				dDst = vis_faligndata(data1, dDst);
				data1 = vis_ld_u16_i(pSrc, six2Factor + 2);
				dDst = vis_faligndata(data2, dDst);
				data2 = vis_ld_u16_i(pSrc, six2Factor);
				dDst = vis_faligndata(data3, dDst);
				data3 = vis_ld_u16_i(pSrc, four2Factor + 2);

				dpDst[i] = dDst;

				dDst = vis_faligndata(data0, dDst);
			}

			k = (k + 4 * times - 2) > 0 ? k + 4 * times - 2 : 0;
			offset = twoPhase + k * factor;

			for (; k < twoN; k += 2, offset += twoFactor) {
				dst[k] = src[offset];
				dst[k + 1] = src[offset + 1];
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
