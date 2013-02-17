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

#pragma ident	"@(#)mlib_s_SignalLMSFilter_Fp.c	9.3	07/11/05 SMI"

/*
 *  FUNCTION
 *    mlib_SignalLMSFilterInit_[F32|F32S]_[F32|F32S] - allocate memory
 *                                                     for internal
 *                                                     filter structure
 *                                                     and convert
 *                                                     the parameters
 *                                                     into internal
 *                                                     representation.
 *
 * SYNOPSIS
 *    mlib_status
 *    mlib_SignalLMSFilterInit_[F32|F32S]_[F32|F32S](void     **filter,
 *                                                   mlib_f32 *flt,
 *                                                   mlib_s32 tap,
 *                                                   mlib_f32 beta)
 * ARGUMENTS
 *    filter - Internal filter structure.
 *    flt    - Filter coefficients array.
 *    tap    - Taps of the filter.
 *    beta   - Error weighting factor.  0 < beta < 1.
 *
 *  FUNCTION
 *    mlib_SignalLMSFilter_[F32|F32S]_[F32|F32S] - apply the LMS
 *                                                 adaptive filter
 *                                                 on one packet
 *                                                 of signal and
 *                                                 update the
 *                                                 filter states.
 *
 *    mlib_SignalLMSFilterNonAdapt_[F32|F32S]_[F32|F32S] - apply the LMS
 *                                                         adaptive filter
 *                                                         on one packet
 *                                                         of signal without
 *                                                         coefficients
 *                                                         adaptation and
 *                                                         update the
 *                                                         filter states.
 *
 * SYNOPSIS
 *    mlib_status
 *    mlib_SignalLMSFilter_[F32|F32S]_[F32|F32S](mlib_f32       *dst,
 *                                               const mlib_f32 *src,
 *                                               const mlib_f32 *ref,
 *                                               void           *filter,
 *                                               mlib_s32       n)
 *      mlib_status
 *      mlib_SignalLMSFilterNonAdapt_[F32|F32S]_[F32|F32S]
 *                                                      (mlib_f32       *dst,
 *                                                       const mlib_f32 *src,
 *                                                       const mlib_f32 *ref,
 *                                                       void           *filter,
 *                                                       mlib_s32       n);
 * ARGUMENTS
 *    dst     - Output signal array.
 *    src     - Input signal array.
 *    ref     - Reference or 'desired' signal array.
 *    filter  - Internal filter structure.
 *    n       - NumBE0r of samples in the input signal array.
 *
 *  FUNCTION
 *    mlib_SignalLMSFilterFree_[F32|F32S]_[F32|F32S] - release the memory
 *                                                     allocated for
 *                                                     the internal
 *                                                     filter structure.
 *
 * SYNOPSIS
 *    void mlib_SignalLMSFilterFree_[F32|F32S]_[F32|F32S](void *filter)
 *
 * ARGUMENTS
 *    filter  - Internal filter structure.
 *
 * DESCRIPTION
 *     The basic LMS adaptive algorithm is summarized below.
 *     ( 1) Initialize weight Wk(i), i = 0, 1, ..., tap - 1.
 *     ( 2) Initialize previous source elements Xo(i),
 *          i = 0, 1, ..., tap - 1.
 *     ( 3) Read Xk(t) from src and Yk(t) from ref, t = 0, 1, ..., n - 1.
 *     ( 4) Compute filter output: nk = sum(Wk(i) * Xk(t - i)),
 *          i = 0, 1, ..., tap - 1.
 *          If i > t, use previous source elements stored in Xo vector.
 *     ( 5) Store filter output : dst[t] = nk.
 *     ( 6) Compute the error estimate: Ek = Yk - nk.
 *     ( 7) Compute factor BE0 = 2 * beta * Ek.
 *     ( 8) Update filter weights: Wk(i) += BE0 * Xk(t - i),
 *          i = 0, 1, ..., tap - 1.
 *          If i > t, use previous source elements stored in Xo vector.
 *     ( 9) Next t, go to step (3).
 *     (10) Store N ending source elements in previous source elements
 *          vector Xo: if N > n, N = n; else N = tap.
 *
 * NOTE
 *     The function assumes that the input signal has a power maximum
 *     equal to 1.  If it is not, beta should be divided by power maximum.
 *     Power maximum is calculated according to the following formula:
 *
 *                        flt_len
 *     Power_max  = MAX {   SUM  signal(n + k)^2 }
 *                   n     k = 0
 *
 *     It is necessary to consider the maximum of power maxima of both
 *     components as the stereo signal's power maximum.
 */

#include <mlib_signal.h>
#include <mlib_s_SignalFIR.h>
#include <mlib_SignalLMS.h>
#include <mlib_sse_utils.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalLMSFilterFree_F32S_F32S = \
	__mlib_SignalLMSFilterFree_F32S_F32S
#pragma weak mlib_SignalLMSFilterFree_F32_F32 = \
	__mlib_SignalLMSFilterFree_F32_F32
#pragma weak mlib_SignalLMSFilterInit_F32S_F32S = \
	__mlib_SignalLMSFilterInit_F32S_F32S
#pragma weak mlib_SignalLMSFilterInit_F32_F32 = \
	__mlib_SignalLMSFilterInit_F32_F32
#pragma weak mlib_SignalLMSFilter_F32S_F32S = \
	__mlib_SignalLMSFilter_F32S_F32S
#pragma weak mlib_SignalLMSFilter_F32_F32 = \
    __mlib_SignalLMSFilter_F32_F32
#pragma weak mlib_SignalLMSFilterNonAdapt_F32S_F32S = \
	__mlib_SignalLMSFilterNonAdapt_F32S_F32S
#pragma weak mlib_SignalLMSFilterNonAdapt_F32_F32 = \
	__mlib_SignalLMSFilterNonAdapt_F32_F32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalLMSFilterFree_F32S_F32S)
    mlib_SignalLMSFilterFree_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalLMSFilterFree_F32S_F32S")));
__typeof__(__mlib_SignalLMSFilterFree_F32_F32) mlib_SignalLMSFilterFree_F32_F32
    __attribute__((weak, alias("__mlib_SignalLMSFilterFree_F32_F32")));
__typeof__(__mlib_SignalLMSFilterInit_F32S_F32S)
    mlib_SignalLMSFilterInit_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalLMSFilterInit_F32S_F32S")));
__typeof__(__mlib_SignalLMSFilterInit_F32_F32) mlib_SignalLMSFilterInit_F32_F32
    __attribute__((weak, alias("__mlib_SignalLMSFilterInit_F32_F32")));
__typeof__(__mlib_SignalLMSFilter_F32S_F32S) mlib_SignalLMSFilter_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalLMSFilter_F32S_F32S")));
__typeof__(__mlib_SignalLMSFilter_F32_F32) mlib_SignalLMSFilter_F32_F32
    __attribute__((weak, alias("__mlib_SignalLMSFilter_F32_F32")));
__typeof__(__mlib_SignalLMSFilterNonAdapt_F32S_F32S)
    mlib_SignalLMSFilterNonAdapt_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalLMSFilterNonAdapt_F32S_F32S")));
__typeof__(__mlib_SignalLMSFilterNonAdapt_F32_F32)
    mlib_SignalLMSFilterNonAdapt_F32_F32
    __attribute__((weak, alias("__mlib_SignalLMSFilterNonAdapt_F32_F32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_SignalLMSFilter_F32_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    const mlib_f32 *ref,
    void *filter,
    mlib_s32 n)
{
	LMS_Filter *pLMS_Filter = (LMS_Filter *) filter;
	mlib_d64 U = pLMS_Filter->U;
	mlib_d64 BE0;
	mlib_d64 *W;
	mlib_d64 *srcBuffer;
	mlib_d64 D, Y;
	mlib_s32 i, j, t, off, count;
	mlib_s32 tap, bufferSize;
	mlib_f32 *psrc, *pdst, *pref;
	__m128d srcs1, srcs2, SBE0, SD;
	__m128d *SW;

	if (src == NULL || dst == NULL || ref == NULL)
		return (MLIB_NULLPOINTER);

	if (n <= 0)
		return (MLIB_FAILURE);

	tap = pLMS_Filter->tap;
	bufferSize = pLMS_Filter->bufferSize;
	W = pLMS_Filter->data;
	srcBuffer = (W + 2 * tap);
	BE0 = pLMS_Filter->BE[0];

	SW = __mlib_malloc(tap * sizeof (__m128d));

	psrc = (mlib_f32 *)src;
	pdst = (mlib_f32 *)dst;
	pref = (mlib_f32 *)ref;

	for (off = 0; off < n; off += count) {

		count = n - off;
		if (count > bufferSize) {
			count = bufferSize;
		}

		mlib_SignalFIR_f32_to_d64((void *)srcBuffer,
		    (void *)psrc, count);

		for (i = 0; i < (tap / 2); i++) {
			SW[i] = _mm_set_pd(W[i * 2], W[i * 2 + 1]);
		}

		for (t = 0; t < count; t++) {
			D = 0;
			Y = pref[t];

			SBE0 = _mm_set1_pd(BE0);
			SD = _mm_setzero_pd();

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < (tap / 2); i++) {
				srcs1 = _mm_loadu_pd(
				    srcBuffer + t - i * 2 - 1 - 1);
				srcs2 = _mm_loadu_pd(
				    srcBuffer + t - i * 2 - 1);
				__m128d temp = _mm_mul_pd(SBE0, srcs1);
				SW[i] = _mm_add_pd(SW[i], temp);
				SD = _mm_add_pd(SD, _mm_mul_pd(srcs2, SW[i]));
			}

			mlib_d64 TD[2];
			_mm_storeu_pd(TD, SD);
			D += TD[0] + TD[1];

			for (i = i * 2; i < tap; i++) {
				W[i] += BE0 * srcBuffer[t - i - 1];
				D += srcBuffer[t - i] * W[i];
			}

			BE0 = (Y - D) * U;
			pdst[t] = (mlib_f32)D;
		}

		for (i = 0; i < (tap / 2); i++) {
			mlib_d64 TW[2];
			_mm_storeu_pd(TW, SW[i]);
			W[i * 2 + 1] = TW[0];
			W[i * 2] = TW[1];
		}

		psrc += count;
		pdst += count;
		pref += count;

		for (i = 0; i < tap; i++) {
			srcBuffer[i - tap] = srcBuffer[count + (i - tap)];
		}
	}

	pLMS_Filter->BE[0] = BE0;

	__mlib_free(SW);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLMSFilter_F32S_F32S(
    mlib_f32 *dst,
    const mlib_f32 *src,
    const mlib_f32 *ref,
    void *filter,
    mlib_s32 n)
{
	LMS_Filter *pLMS_Filter = (LMS_Filter *) filter;
	mlib_d64 *W[2], *srcBuffer[2];
	mlib_d64 U = pLMS_Filter->U;
	mlib_d64 D, Y;
	mlib_s32 i, t, j, off, count;
	mlib_s32 tap, bufferSize;
	mlib_f32 *psrc, *pdst, *pref;

	__m128d srcs1, srcs2, srcs3, srcs4;
	__m128d SBE0, SD, temp;
	__m128d *SW;

	if (src == NULL || dst == NULL || ref == NULL)
		return (MLIB_NULLPOINTER);

	if (n <= 0)
		return (MLIB_FAILURE);

	tap = pLMS_Filter->tap;
	bufferSize = pLMS_Filter->bufferSize;

	SW = __mlib_malloc(tap * sizeof (__m128d));

	W[0] = pLMS_Filter->data;
	srcBuffer[0] = (W[0] + 2 * tap);
	W[1] = srcBuffer[0] + 2 * bufferSize;
	srcBuffer[1] = (W[1] + 2 * tap);

	psrc = (mlib_f32 *)src;
	pdst = (mlib_f32 *)dst;
	pref = (mlib_f32 *)ref;

	for (off = 0; off < n; off += count) {

		count = n - off;
		if (count > bufferSize) {
			count = bufferSize;
		}

		for (i = 0; i < count; i++) {
			srcBuffer[0][i] = (mlib_d64)psrc[2 * i];
			srcBuffer[1][i] = (mlib_d64)psrc[2 * i + 1];
		}

		for (j = 0; j < 2; j++) {
			mlib_d64 *psrcBuffer = srcBuffer[j];
			mlib_d64 *pW = W[j];
			mlib_d64 BE0 = pLMS_Filter->BE[j];

			for (i = 0; i < (tap / 2); i++) {
				SW[i] = _mm_set_pd(pW[i * 2], pW[i * 2 + 1]);
			}

			for (t = 0; t < count; t++) {
				D = 0;
				Y = pref[2 * t + j];
				SBE0 = _mm_set1_pd(BE0);
				SD = _mm_setzero_pd();
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < (tap / 2); i++) {
					srcs1 = _mm_loadu_pd(
					    psrcBuffer + t - i * 2 - 2);
					srcs2 = _mm_loadu_pd(
					    psrcBuffer + t - i * 2 - 1);

					temp = _mm_mul_pd(SBE0, srcs1);
					SW[i] = _mm_add_pd(SW[i], temp);
					SD = _mm_add_pd(SD,
					    _mm_mul_pd(srcs2, SW[i]));
				}

				mlib_d64 TD[2];
				_mm_storeu_pd(TD, SD);
				D += TD[0] + TD[1];

				for (i = i * 2; i < tap; i++) {
					pW[i] += BE0 * psrcBuffer[t - i - 1];
					D += psrcBuffer[t - i] * pW[i];
				}

				BE0 = (Y - D) * U;

				pdst[2 * t + j] = (mlib_f32)D;
			}

			mlib_d64 TW[2];
			for (i = 0; i < (tap / 2); i++) {
				_mm_storeu_pd(TW, SW[i]);
				pW[i * 2] = TW[1];
				pW[i * 2 + 1] = TW[0];
			}

			for (i = 0; i < tap; i++) {
				psrcBuffer[i - tap] =
				    psrcBuffer[count + (i - tap)];
			}

			pLMS_Filter->BE[j] = BE0;
		}

		psrc += 2 * count;
		pdst += 2 * count;
		pref += 2 * count;
	}

	__mlib_free(SW);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLMSFilterNonAdapt_F32_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    const mlib_f32 *ref,
    void *filter,
    mlib_s32 n)
{
	LMS_Filter *pLMS_Filter = (LMS_Filter *) filter;
	mlib_d64 *W, *srcBuffer, *dstBuffer;
	mlib_s32 i, k, off, count;
	mlib_s32 tap, bufferSize;
	mlib_f32 *psrc, *pdst;

	if (src == NULL || dst == NULL)
		return (MLIB_NULLPOINTER);

	if (n <= 0)
		return (MLIB_FAILURE);

	tap = pLMS_Filter->tap;
	bufferSize = pLMS_Filter->bufferSize;

	W = pLMS_Filter->data;
	srcBuffer = (W + 2 * tap);
	dstBuffer = srcBuffer + bufferSize;

	psrc = (mlib_f32 *)src;
	pdst = (mlib_f32 *)dst;

	for (off = 0; off < n; off += count) {

		count = n - off;
		if (count > bufferSize) {
			count = bufferSize;
		}

		mlib_SignalFIR_f32_to_d64((void *)srcBuffer, (void *)psrc,
		    count);

		k = 0;
		if (tap == 1) {
			mlib_FIR_tap1f_d64(dstBuffer, (void *)(srcBuffer - k),
			    W + k, count);
			k += 1;
		} else if (tap == 2) {
			mlib_FIR_tap2f_d64(dstBuffer,
			    (void *)(srcBuffer - k - 1), W + k, count);
			k += 2;
		} else if (tap & 1) {
			mlib_FIR_tap3f_d64(dstBuffer,
			    (void *)(srcBuffer - k - 2), W + k, count);
			k += 3;
		} else {
			mlib_FIR_tap4f_d64(dstBuffer,
			    (void *)(srcBuffer - k - 3), W + k, count);
			k += 4;
		}
		while (k <= (tap - 4)) {
			mlib_FIR_tap4_d64(dstBuffer,
			    (void *)(srcBuffer - k - 3), W + k, count);
			k += 4;
		}
		if (k < tap) {
			mlib_FIR_tap2_d64(dstBuffer,
			    (void *)(srcBuffer - k - 1), W + k, count);
		}

		for (i = 0; i < tap; i++) {
			srcBuffer[i - tap] = srcBuffer[count + (i - tap)];
		}

		mlib_SignalFIR_d64_to_f32(pdst, dstBuffer, count);

		psrc += count;
		pdst += count;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLMSFilterNonAdapt_F32S_F32S(
    mlib_f32 *dst,
    const mlib_f32 *src,
    const mlib_f32 *ref,
    void *filter,
    mlib_s32 n)
{
	LMS_Filter *pLMS_Filter = (LMS_Filter *) filter;
	mlib_d64 *W[2], *srcBuffer[2], *dstBuffer[2];
	mlib_s32 i, k, off, count;
	mlib_s32 tap, bufferSize;
	mlib_f32 *psrc, *pdst;

	if (src == NULL || dst == NULL)
		return (MLIB_NULLPOINTER);

	if (n <= 0)
		return (MLIB_FAILURE);

	tap = pLMS_Filter->tap;
	bufferSize = pLMS_Filter->bufferSize;

	W[0] = pLMS_Filter->data;
	srcBuffer[0] = (W[0] + 2 * tap);
	dstBuffer[0] = srcBuffer[0] + bufferSize;

	W[1] = dstBuffer[0] + bufferSize;
	srcBuffer[1] = (W[1] + 2 * tap);
	dstBuffer[1] = srcBuffer[1] + bufferSize;

	psrc = (mlib_f32 *)src;
	pdst = (mlib_f32 *)dst;

	for (off = 0; off < n; off += count) {

		count = n - off;
		if (count > bufferSize) {
			count = bufferSize;
		}

		for (i = 0; i < count; i++) {
			srcBuffer[0][i] = (mlib_d64)psrc[2 * i];
			srcBuffer[1][i] = (mlib_d64)psrc[2 * i + 1];
		}

		k = 0;
		if (tap == 1) {
			mlib_FIR_tap1f_d64(dstBuffer[0],
			    (void *)(srcBuffer[0] - k), W[0] + k, count);
			mlib_FIR_tap1f_d64(dstBuffer[1],
			    (void *)(srcBuffer[1] - k), W[1] + k, count);
			k += 1;
		} else if (tap == 2) {
			mlib_FIR_tap2f_d64(dstBuffer[0],
			    (void *)(srcBuffer[0] - k - 1), W[0] + k, count);
			mlib_FIR_tap2f_d64(dstBuffer[1],
			    (void *)(srcBuffer[1] - k - 1), W[1] + k, count);
			k += 2;
		} else if (tap & 1) {
			mlib_FIR_tap3f_d64(dstBuffer[0],
			    (void *)(srcBuffer[0] - k - 2), W[0] + k, count);
			mlib_FIR_tap3f_d64(dstBuffer[1],
			    (void *)(srcBuffer[1] - k - 2), W[1] + k, count);
			k += 3;
		} else {
			mlib_FIR_tap4f_d64(dstBuffer[0],
			    (void *)(srcBuffer[0] - k - 3), W[0] + k, count);
			mlib_FIR_tap4f_d64(dstBuffer[1],
			    (void *)(srcBuffer[1] - k - 3), W[1] + k, count);
			k += 4;
		}
		while (k <= (tap - 4)) {
			mlib_FIR_tap4_d64(dstBuffer[0],
			    (void *)(srcBuffer[0] - k - 3), W[0] + k, count);
			mlib_FIR_tap4_d64(dstBuffer[1],
			    (void *)(srcBuffer[1] - k - 3), W[1] + k, count);
			k += 4;
		}
		if (k < tap) {
			mlib_FIR_tap2_d64(dstBuffer[0],
			    (void *)(srcBuffer[0] - k - 1), W[0] + k, count);
			mlib_FIR_tap2_d64(dstBuffer[1],
			    (void *)(srcBuffer[1] - k - 1), W[1] + k, count);
		}

		for (i = 0; i < tap; i++) {
			srcBuffer[0][i - tap] = srcBuffer[0][count + (i - tap)];
		}

		for (i = 0; i < tap; i++) {
			srcBuffer[1][i - tap] = srcBuffer[1][count + (i - tap)];
		}

		for (i = 0; i < count; i++) {
			pdst[2 * i] = (mlib_f32)dstBuffer[0][i];
			pdst[2 * i + 1] = (mlib_f32)dstBuffer[1][i];
		}

		psrc += 2 * count;
		pdst += 2 * count;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLMSFilterInit_F32_F32(
    void **filter,
    const mlib_f32 *flt,
    mlib_s32 tap,
    mlib_f32 beta)
{
	return (__mlib_SignalLMSInit_common(filter, flt, tap, beta,
	    1.0, 1.0, 0));
}

/* *********************************************************** */

mlib_status
__mlib_SignalLMSFilterInit_F32S_F32S(
    void **filter,
    const mlib_f32 *flt,
    mlib_s32 tap,
    mlib_f32 beta)
{
	return (__mlib_SignalLMSInit_common(filter, flt, tap, beta,
	    1.0, 1.0, 1));
}

/* *********************************************************** */

void
__mlib_SignalLMSFilterFree_F32_F32(
    void *filter)
{
	__mlib_free(filter);
}

/* *********************************************************** */

void
__mlib_SignalLMSFilterFree_F32S_F32S(
    void *filter)
{
	__mlib_free(filter);
}

/* *********************************************************** */
