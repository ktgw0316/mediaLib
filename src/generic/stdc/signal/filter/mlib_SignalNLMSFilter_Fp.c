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
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_SignalNLMSFilter_Fp.c	9.2	07/10/09 SMI"

/*
 *  FUNCTION
 *    mlib_SignalNLMSFilterInit_[F32|F32S]_[F32|F32S] - allocate memory
 *                                                      for internal
 *                                                      filter structure
 *                                                      and convert
 *                                                      the parameters
 *                                                      into internal
 *                                                      representation.
 *
 * SYNOPSIS
 *    mlib_status
 *    mlib_SignalNLMSFilterInit_[F32|F32S]_[F32|F32S](void     **filter,
 *                                                    mlib_f32 *flt,
 *                                                    mlib_s32 tap,
 *                                                    mlib_f32 beta)
 * ARGUMENTS
 *    filter - Internal filter structure.
 *    flt    - Filter coefficients array.
 *    tap    - Taps of the filter.
 *    beta   - Error weighting factor.  0 < beta < 1.
 *
 *  FUNCTION
 *    mlib_SignalNLMSFilter_[F32|F32S]_[F32|F32S] - apply the LMS
 *                                                  adaptive filter
 *                                                  on one packet
 *                                                  of signal and
 *                                                  update the
 *                                                  filter states.
 *
 *    mlib_SignalNLMSFilterNonAdapt_[F32|F32S]_[F32|F32S] - apply the LMS
 *                                                          adaptive filter
 *                                                          on one packet
 *                                                          of signal without
 *                                                          coefficients
 *                                                          adaptation and
 *                                                          update the
 *                                                          filter states.
 *
 * SYNOPSIS
 *    mlib_status
 *    mlib_SignalNLMSFilter_[F32|F32S]_[F32|F32S](mlib_f32       *dst,
 *                                                const mlib_f32 *src,
 *                                                const mlib_f32 *ref,
 *                                                void           *filter,
 *                                                mlib_s32       n)
 *      mlib_status
 *      mlib_SignalNLMSFilterNonAdapt_[F32|F32S]_[F32|F32S]
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
 *    mlib_SignalNLMSFilterFree_[F32|F32S]_[F32|F32S] - release the memory
 *                                                      allocated for
 *                                                      the internal
 *                                                      filter structure.
 *
 * SYNOPSIS
 *    void mlib_SignalNLMSFilterFree_[F32|F32S]_[F32|F32S](void *filter)
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
#include <mlib_SignalFIR.h>
#include <mlib_SignalLMS.h>
#include <stdio.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalNLMSFilterFree_F32S_F32S = \
	__mlib_SignalNLMSFilterFree_F32S_F32S
#pragma weak mlib_SignalNLMSFilterFree_F32_F32 = \
	__mlib_SignalNLMSFilterFree_F32_F32
#pragma weak mlib_SignalNLMSFilterInit_F32S_F32S = \
	__mlib_SignalNLMSFilterInit_F32S_F32S
#pragma weak mlib_SignalNLMSFilterInit_F32_F32 = \
	__mlib_SignalNLMSFilterInit_F32_F32
#pragma weak mlib_SignalNLMSFilter_F32S_F32S = \
	__mlib_SignalNLMSFilter_F32S_F32S
#pragma weak mlib_SignalNLMSFilter_F32_F32 = \
    __mlib_SignalNLMSFilter_F32_F32
#pragma weak mlib_SignalNLMSFilterNonAdapt_F32S_F32S = \
	__mlib_SignalNLMSFilterNonAdapt_F32S_F32S
#pragma weak mlib_SignalNLMSFilterNonAdapt_F32_F32 = \
	__mlib_SignalNLMSFilterNonAdapt_F32_F32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalNLMSFilterFree_F32S_F32S)
    mlib_SignalNLMSFilterFree_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalNLMSFilterFree_F32S_F32S")));
__typeof__(__mlib_SignalNLMSFilterFree_F32_F32)
    mlib_SignalNLMSFilterFree_F32_F32
    __attribute__((weak, alias("__mlib_SignalNLMSFilterFree_F32_F32")));
__typeof__(__mlib_SignalNLMSFilterInit_F32S_F32S)
    mlib_SignalNLMSFilterInit_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalNLMSFilterInit_F32S_F32S")));
__typeof__(__mlib_SignalNLMSFilterInit_F32_F32)
    mlib_SignalNLMSFilterInit_F32_F32
    __attribute__((weak, alias("__mlib_SignalNLMSFilterInit_F32_F32")));
__typeof__(__mlib_SignalNLMSFilter_F32S_F32S) mlib_SignalNLMSFilter_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalNLMSFilter_F32S_F32S")));
__typeof__(__mlib_SignalNLMSFilter_F32_F32) mlib_SignalNLMSFilter_F32_F32
    __attribute__((weak, alias("__mlib_SignalNLMSFilter_F32_F32")));
__typeof__(__mlib_SignalNLMSFilterNonAdapt_F32S_F32S)
    mlib_SignalNLMSFilterNonAdapt_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalNLMSFilterNonAdapt_F32S_F32S")));
__typeof__(__mlib_SignalNLMSFilterNonAdapt_F32_F32)
    mlib_SignalNLMSFilterNonAdapt_F32_F32
    __attribute__((weak, alias("__mlib_SignalNLMSFilterNonAdapt_F32_F32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_SignalNLMSFilter_F32_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    const mlib_f32 *ref,
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
	mlib_f32 *psrc, *pdst, *pref;

	if (src == NULL || dst == NULL || ref == NULL)
		return (MLIB_NULLPOINTER);

	if (n <= 0)
		return (MLIB_FAILURE);

	tap = pLMS_Filter->tap;
	bufferSize = pLMS_Filter->bufferSize;
	W = pLMS_Filter->data;
	srcBuffer = (W + 2 * tap);
	BE0 = pLMS_Filter->BE[0];
	pow = pLMS_Filter->pow[0];

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

		for (t = 0; t < count; t++) {
			D = 0;
			Y = pref[t];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < tap; i++) {
				W[i] += BE0 * srcBuffer[t - i - 1];
				D += srcBuffer[t - i] * W[i];
			}

			pow = pow - srcBuffer[t - tap] * srcBuffer[t - tap] +
			    srcBuffer[t] * srcBuffer[t];
			if (pow == 0)
				BE0 = 0;
			else
				BE0 = (Y - D) * U / pow;
			pdst[t] = (mlib_f32)D;
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

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalNLMSFilter_F32S_F32S(
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

	if (src == NULL || dst == NULL || ref == NULL)
		return (MLIB_NULLPOINTER);

	if (n <= 0)
		return (MLIB_FAILURE);

	tap = pLMS_Filter->tap;
	bufferSize = pLMS_Filter->bufferSize;

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
			mlib_d64 pow = pLMS_Filter->pow[j];

			for (t = 0; t < count; t++) {
				D = 0;
				Y = pref[2 * t + j];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < tap; i++) {
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
				pdst[2 * t + j] = (mlib_f32)D;
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

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalNLMSFilterNonAdapt_F32_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    const mlib_f32 *ref,
    void *filter,
    mlib_s32 n)
{
	return (__mlib_SignalLMSFilterNonAdapt_F32_F32(dst, src, ref, filter,
	    n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalNLMSFilterNonAdapt_F32S_F32S(
    mlib_f32 *dst,
    const mlib_f32 *src,
    const mlib_f32 *ref,
    void *filter,
    mlib_s32 n)
{
	return (__mlib_SignalLMSFilterNonAdapt_F32S_F32S(dst, src, ref, filter,
	    n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalNLMSFilterInit_F32_F32(
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
__mlib_SignalNLMSFilterInit_F32S_F32S(
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
__mlib_SignalNLMSFilterFree_F32_F32(
    void *filter)
{
	__mlib_free(filter);
}

/* *********************************************************** */

void
__mlib_SignalNLMSFilterFree_F32S_F32S(
    void *filter)
{
	__mlib_free(filter);
}

/* *********************************************************** */
