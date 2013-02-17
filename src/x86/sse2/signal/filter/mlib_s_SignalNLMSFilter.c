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

#pragma ident	"@(#)mlib_s_SignalNLMSFilter.c	9.3	07/11/05 SMI"

/*
 *  FUNCTION
 *    mlib_SignalNLMSFilterInit_[S16|S16S]_[S16|S16S] - allocate memory
 *                                                      for internal
 *                                                      filter structure
 *                                                      and convert
 *                                                      the parameters
 *                                                      into internal
 *                                                      representation.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalNLMSFilterInit_S16S_S16S(void           **filter,
 *                                                      const mlib_f32 *flt,
 *                                                      mlib_s32       tap,
 *                                                      mlib_f32       beta);
 *      mlib_status mlib_SignalNLMSFilterInit_S16_S16(void           **filter,
 *                                                    const mlib_f32 *flt,
 *                                                    mlib_s32       tap,
 *                                                    mlib_f32       beta);
 * ARGUMENTS
 *    filter - Internal filter structure.
 *    flt    - Filter coefficients array.
 *    tap    - Taps of the filter.
 *    beta   - Error weighting factor.  0 < beta < 1.
 *
 *  FUNCTION
 *    mlib_SignalNLMSFilter_[S16|S16S]_[S16|S16S]_Sat - apply the LMS
 *                                                      adaptive filter
 *                                                      on one packet
 *                                                      of signal and
 *                                                      update the
 *                                                      filter states.
 *
 *    mlib_SignalNLMSFilterNonAdapt_[S16|S16S]_[S16|S16S]_Sat - apply the LMS
 *                                                              adaptive filter
 *                                                              on one packet
 *                                                              of signal
 *                                                              without
 *                                                              coefficients
 *                                                              adaptation and
 *                                                              update the
 *                                                              filter states.
 *
 * SYNOPSIS
 *
 *      mlib_status
 *      mlib_SignalNLMSFilter_[S16|S16S]_[S16|S16S]_Sat(mlib_s16       *dst,
 *                                                      const mlib_s16 *src,
 *                                                      const mlib_s16 *ref,
 *                                                      void           *filter,
 *                                                      mlib_s32       n);
 *      mlib_status
 *      mlib_SignalNLMSFilterNonAdapt_[S16|S16S]_[S16|S16S]_Sat
 *                                                      (mlib_s16       *dst,
 *                                                       const mlib_s16 *src,
 *                                                       const mlib_s16 *ref,
 *                                                       void           *filter,
 *                                                       mlib_s32       n);
 * ARGUMENTS
 *    dst     - Output signal array.
 *    src     - Input signal array.
 *    ref     - Reference or 'desired' signal array.
 *    filter  - Internal filter structure.
 *    n       - Number of samples in the input signal array.
 *
 *  FUNCTION
 *
 * SYNOPSIS
 *
 *      void mlib_SignalNLMSFilterFree_S16S_S16S(void *filter);
 *      void mlib_SignalNLMSFilterFree_S16_S16(void *filter);
 *
 * ARGUMENTS
 *    filter  - Internal filter structure.
 *
 * DESCRIPTION
 *     The basic NLMS adaptive algorithm is summarized below.
 *     ( 1) Initialize weight Wk(i), i = 0, 1, ..., tap - 1.
 *     ( 2) Initialize previous source elements Xo(i),
 *          i = 0, 1, ..., tap - 1.
 *     ( 3) Read Xk(t) from src and Yk(t) from ref, t = 0, 1, ..., n - 1.
 *     ( 4) Compute filter output: nk = sum(Wk(i) * Xk(t - i)),
 *          i = 0, 1, ..., tap - 1.
 *          If i > t, use previous source elements stored in Xo vector.
 *     ( 5) Compute source elements power: Pwk = sum(Xk(t - i) * Xk(t - i)),
 *          i = 0, 1, ..., tap - 1.
 *          If i > t, use previous source elements stored in Xo vector.
 *     ( 6) Store filter output : dst[t] = nk.
 *     ( 7) Compute the error estimate: Ek = Yk - nk.
 *     ( 8) Compute factor BE0 = 2 * beta * Ek / Pwk.
 *     ( 9) Update filter weights: Wk(i) += BE0 * Xk(t - i),
 *          i = 0, 1, ..., tap - 1.
 *          If i > t, use previous source elements stored in Xo vector.
 *     (10) Next t, go to step (3).
 *     (11) Store N ending source elements in previous source elements
 *          vector Xo: if N > n, N = n; else N = tap.
 */

#include <mlib_signal.h>
#include <mlib_s_SignalFIR.h>
#include <mlib_Utils.h>
#include <mlib_SignalLMS.h>
#include <mlib_sse_utils.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalNLMSFilterFree_S16S_S16S = \
	__mlib_SignalNLMSFilterFree_S16S_S16S
#pragma weak mlib_SignalNLMSFilterFree_S16_S16 = \
	__mlib_SignalNLMSFilterFree_S16_S16
#pragma weak mlib_SignalNLMSFilterInit_S16S_S16S = \
	__mlib_SignalNLMSFilterInit_S16S_S16S
#pragma weak mlib_SignalNLMSFilterInit_S16_S16 = \
	__mlib_SignalNLMSFilterInit_S16_S16
#pragma weak mlib_SignalNLMSFilter_S16S_S16S_Sat = \
	__mlib_SignalNLMSFilter_S16S_S16S_Sat
#pragma weak mlib_SignalNLMSFilter_S16_S16_Sat = \
	__mlib_SignalNLMSFilter_S16_S16_Sat
#pragma weak mlib_SignalNLMSFilterNonAdapt_S16S_S16S_Sat = \
	__mlib_SignalNLMSFilterNonAdapt_S16S_S16S_Sat
#pragma weak mlib_SignalNLMSFilterNonAdapt_S16_S16_Sat = \
	__mlib_SignalNLMSFilterNonAdapt_S16_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalNLMSFilterFree_S16S_S16S)
    mlib_SignalNLMSFilterFree_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalNLMSFilterFree_S16S_S16S")));
__typeof__(__mlib_SignalNLMSFilterFree_S16_S16)
    mlib_SignalNLMSFilterFree_S16_S16
    __attribute__((weak, alias("__mlib_SignalNLMSFilterFree_S16_S16")));
__typeof__(__mlib_SignalNLMSFilterInit_S16S_S16S)
    mlib_SignalNLMSFilterInit_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalNLMSFilterInit_S16S_S16S")));
__typeof__(__mlib_SignalNLMSFilterInit_S16_S16)
    mlib_SignalNLMSFilterInit_S16_S16
    __attribute__((weak, alias("__mlib_SignalNLMSFilterInit_S16_S16")));
__typeof__(__mlib_SignalNLMSFilter_S16S_S16S_Sat)
    mlib_SignalNLMSFilter_S16S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalNLMSFilter_S16S_S16S_Sat")));
__typeof__(__mlib_SignalNLMSFilter_S16_S16_Sat)
    mlib_SignalNLMSFilter_S16_S16_Sat
    __attribute__((weak, alias("__mlib_SignalNLMSFilter_S16_S16_Sat")));
__typeof__(__mlib_SignalNLMSFilterNonAdapt_S16S_S16S_Sat)
    mlib_SignalNLMSFilterNonAdapt_S16S_S16S_Sat
    __attribute__((weak,
    alias("__mlib_SignalNLMSFilterNonAdapt_S16S_S16S_Sat")));
__typeof__(__mlib_SignalNLMSFilterNonAdapt_S16_S16_Sat)
    mlib_SignalNLMSFilterNonAdapt_S16_S16_Sat
    __attribute__((weak,
    alias("__mlib_SignalNLMSFilterNonAdapt_S16_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_CLAMP_S16(DST, SRC)	CLAMP_S16(DST, SRC)

/* *********************************************************** */

mlib_status
__mlib_SignalNLMSFilter_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *ref,
    void *filter,
    mlib_s32 n)
{
	LMS_Filter *pLMS_Filter = (LMS_Filter *) filter;
	mlib_d64 U = pLMS_Filter->U;
	mlib_d64 BE0, pow;
	mlib_d64 *W;
	mlib_d64 *srcBuffer;
	mlib_d64 D, Y;
	mlib_s32 i, t, off, count;
	mlib_s32 tap, bufferSize;
	mlib_s16 *psrc, *pdst, *pref;

	__m128d srcs1, srcs2;
	__m128d SBE0, SD;
	__m128d *SW;

	if (src == NULL || dst == NULL || ref == NULL)
		return (MLIB_NULLPOINTER);

	if (n <= 0)
		return (MLIB_FAILURE);

	tap = pLMS_Filter->tap;
	SW = __mlib_malloc(tap * sizeof (__m128d));
	bufferSize = pLMS_Filter->bufferSize;
	W = pLMS_Filter->data;
	srcBuffer = (W + 2 * tap);
	BE0 = pLMS_Filter->BE[0];
	pow = pLMS_Filter->pow[0];

	psrc = (mlib_s16 *)src;
	pdst = (mlib_s16 *)dst;
	pref = (mlib_s16 *)ref;

	for (off = 0; off < n; off += count) {

		count = n - off;
		if (count > bufferSize) {
			count = bufferSize;
		}

		mlib_SignalFIR_s16_to_d64((void *)srcBuffer,
		    (void *)psrc, count);

		for (i = 0; i < (tap / 2); i++) {
			SW[i] = _mm_set_pd(W[i * 2], W[i * 2 + 1]);
		}

		for (t = 0; t < count; t++) {
			D = 0;
			Y = pref[t] * (mlib_d64)SAT_SCALE_S16;

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

			pow = pow - srcBuffer[t - tap] * srcBuffer[t - tap] +
			    srcBuffer[t] * srcBuffer[t];

			if (pow == 0)
				BE0 = 0;
			else
				BE0 = (Y - D) * U / pow;

			MLIB_CLAMP_S16(pdst[t], D);
		}

		mlib_d64 TW[2];
		for (i = 0; i < (tap / 2); i++) {
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
	pLMS_Filter->pow[0] = pow;

	__mlib_free(SW);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalNLMSFilter_S16S_S16S_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *ref,
    void *filter,
    mlib_s32 n)
{
	LMS_Filter *pLMS_Filter = (LMS_Filter *) filter;
	mlib_d64 *W[2], *srcBuffer[2];
	mlib_d64 U = pLMS_Filter->U;
	mlib_d64 D, Y;
	mlib_s32 i, t, j, off, count;
	mlib_s32 tap, bufferSize;
	mlib_s16 *psrc, *pdst, *pref;

	__m128d srcs1, srcs2, srcs3, srcs4;
	__m128d SBE0, SD;
	__m128d *SW;
	__m128d temp;

	if (src == NULL || dst == NULL || ref == NULL)
		return (MLIB_NULLPOINTER);

	if (n <= 0)
		return (MLIB_FAILURE);

	tap = pLMS_Filter->tap;
	SW = __mlib_malloc(tap * sizeof (__m128d));
	bufferSize = pLMS_Filter->bufferSize;

	W[0] = pLMS_Filter->data;
	srcBuffer[0] = (W[0] + 2 * tap);
	W[1] = srcBuffer[0] + 2 * bufferSize;
	srcBuffer[1] = (W[1] + 2 * tap);

	psrc = (mlib_s16 *)src;
	pdst = (mlib_s16 *)dst;
	pref = (mlib_s16 *)ref;

	for (off = 0; off < n; off += count) {

		count = n - off;
		if (count > bufferSize) {
			count = bufferSize;
		}

		mlib_SignalFIR_s16_to_d64S((void *)srcBuffer[0],
		    (void *)srcBuffer[1], (void *)psrc, count * 2);

		for (j = 0; j < 2; j++) {
			mlib_d64 *psrcBuffer = srcBuffer[j];
			mlib_d64 *pW = W[j];
			mlib_d64 BE0 = pLMS_Filter->BE[j];
			mlib_d64 pow = pLMS_Filter->pow[j];

			for (i = 0; i < (tap / 2); i++) {
				SW[i] = _mm_set_pd(pW[i * 2], pW[i * 2 + 1]);
			}


			for (t = 0; t < count; t++) {
				D = 0;
				Y = pref[2 * t + j] * (mlib_d64)SAT_SCALE_S16;
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

				pow =
				    pow - psrcBuffer[t - tap] * psrcBuffer[t -
				    tap] + psrcBuffer[t] * psrcBuffer[t];

				if (pow == 0)
					BE0 = 0;
				else
					BE0 = (Y - D) * U / pow;

				MLIB_CLAMP_S16(pdst[2 * t + j], D);
			}

			mlib_d64 TW[2];
			for (i = 0; i < (tap / 2); i++) {
				_mm_storeu_pd(TW, SW[i]);
				pW[i * 2 + 1] = TW[0];
				pW[i * 2] = TW[1];
			}

			for (i = 0; i < tap; i++) {
				psrcBuffer[i - tap] =
				    psrcBuffer[count + (i - tap)];
			}

			pLMS_Filter->BE[j] = BE0;
			pLMS_Filter->pow[j] = pow;
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
__mlib_SignalNLMSFilterNonAdapt_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *ref,
    void *filter,
    mlib_s32 n)
{
	return (__mlib_SignalLMSFilterNonAdapt_S16_S16_Sat(dst, src, ref,
	    filter, n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalNLMSFilterNonAdapt_S16S_S16S_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *ref,
    void *filter,
    mlib_s32 n)
{
	return (__mlib_SignalLMSFilterNonAdapt_S16S_S16S_Sat(dst, src, ref,
	    filter, n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalNLMSFilterInit_S16_S16(
    void **filter,
    const mlib_f32 *flt,
    mlib_s32 tap,
    mlib_f32 beta)
{
	return (__mlib_SignalLMSInit_common(filter, flt, tap, beta,
	    1.0, SAT_SCALE_S16, 0));
}

/* *********************************************************** */

mlib_status
__mlib_SignalNLMSFilterInit_S16S_S16S(
    void **filter,
    const mlib_f32 *flt,
    mlib_s32 tap,
    mlib_f32 beta)
{
	return (__mlib_SignalLMSInit_common(filter, flt, tap, beta,
	    1.0, SAT_SCALE_S16, 1));
}

/* *********************************************************** */

void
__mlib_SignalNLMSFilterFree_S16_S16(
    void *filter)
{
	__mlib_free(filter);
}

/* *********************************************************** */

void
__mlib_SignalNLMSFilterFree_S16S_S16S(
    void *filter)
{
	__mlib_free(filter);
}

/* *********************************************************** */
