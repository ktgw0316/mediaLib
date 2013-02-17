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

#pragma ident	"@(#)mlib_SignalLMSFilter.c	9.2	07/10/09 SMI"

/*
 *  FUNCTION
 *    mlib_SignalLMSFilterInit_[S16|S16S]_[S16|S16S] - allocate memory
 *                                                     for internal
 *                                                     filter structure
 *                                                     and convert
 *                                                     the parameters
 *                                                     into internal
 *                                                     representation.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalLMSFilterInit_S16S_S16S(void           **filter,
 *                                                     const mlib_f32 *flt,
 *                                                     mlib_s32       tap,
 *                                                     mlib_f32       beta);
 *      mlib_status mlib_SignalLMSFilterInit_S16_S16(void           **filter,
 *                                                   const mlib_f32 *flt,
 *                                                   mlib_s32       tap,
 *                                                   mlib_f32       beta);
 * ARGUMENTS
 *    filter - Internal filter structure.
 *    flt    - Filter coefficients array.
 *    tap    - Taps of the filter.
 *    beta   - Error weighting factor.  0 < beta < 1.
 *
 *  FUNCTION
 *    mlib_SignalLMSFilter_[S16|S16S]_[S16|S16S]_Sat - apply the LMS
 *                                                     adaptive filter
 *                                                     on one packet
 *                                                     of signal and
 *                                                     update the
 *                                                     filter states.
 *
 *    mlib_SignalLMSFilterNonAdapt_[S16|S16S]_[S16|S16S]_Sat - apply the LMS
 *                                                             adaptive filter
 *                                                             on one packet
 *                                                             of signal without
 *                                                             coefficients
 *                                                             adaptation and
 *                                                             update the
 *                                                             filter states.
 *
 * SYNOPSIS
 *
 *      mlib_status
 *      mlib_SignalLMSFilter_[S16|S16S]_[S16|S16S]_Sat(mlib_s16       *dst,
 *                                                     const mlib_s16 *src,
 *                                                     const mlib_s16 *ref,
 *                                                     void           *filter,
 *                                                     mlib_s32       n);
 *      mlib_status
 *      mlib_SignalLMSFilterNonAdapt_[S16|S16S]_[S16|S16S]_Sat
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
 *      void mlib_SignalLMSFilterFree_S16S_S16S(void *filter);
 *      void mlib_SignalLMSFilterFree_S16_S16(void *filter);
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
#include <mlib_SignalFIR.h>
#include <mlib_Utils.h>
#include <mlib_SignalLMS.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalLMSFilterFree_S16S_S16S = \
	__mlib_SignalLMSFilterFree_S16S_S16S
#pragma weak mlib_SignalLMSFilterFree_S16_S16 = \
	__mlib_SignalLMSFilterFree_S16_S16
#pragma weak mlib_SignalLMSFilterInit_S16S_S16S = \
	__mlib_SignalLMSFilterInit_S16S_S16S
#pragma weak mlib_SignalLMSFilterInit_S16_S16 = \
	__mlib_SignalLMSFilterInit_S16_S16
#pragma weak mlib_SignalLMSFilter_S16S_S16S_Sat = \
	__mlib_SignalLMSFilter_S16S_S16S_Sat
#pragma weak mlib_SignalLMSFilter_S16_S16_Sat = \
	__mlib_SignalLMSFilter_S16_S16_Sat
#pragma weak mlib_SignalLMSFilterNonAdapt_S16S_S16S_Sat = \
	__mlib_SignalLMSFilterNonAdapt_S16S_S16S_Sat
#pragma weak mlib_SignalLMSFilterNonAdapt_S16_S16_Sat = \
	__mlib_SignalLMSFilterNonAdapt_S16_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalLMSFilterFree_S16S_S16S)
    mlib_SignalLMSFilterFree_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalLMSFilterFree_S16S_S16S")));
__typeof__(__mlib_SignalLMSFilterFree_S16_S16) mlib_SignalLMSFilterFree_S16_S16
    __attribute__((weak, alias("__mlib_SignalLMSFilterFree_S16_S16")));
__typeof__(__mlib_SignalLMSFilterInit_S16S_S16S)
    mlib_SignalLMSFilterInit_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalLMSFilterInit_S16S_S16S")));
__typeof__(__mlib_SignalLMSFilterInit_S16_S16) mlib_SignalLMSFilterInit_S16_S16
    __attribute__((weak, alias("__mlib_SignalLMSFilterInit_S16_S16")));
__typeof__(__mlib_SignalLMSFilter_S16S_S16S_Sat)
    mlib_SignalLMSFilter_S16S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalLMSFilter_S16S_S16S_Sat")));
__typeof__(__mlib_SignalLMSFilter_S16_S16_Sat) mlib_SignalLMSFilter_S16_S16_Sat
    __attribute__((weak, alias("__mlib_SignalLMSFilter_S16_S16_Sat")));
__typeof__(__mlib_SignalLMSFilterNonAdapt_S16S_S16S_Sat)
    mlib_SignalLMSFilterNonAdapt_S16S_S16S_Sat
    __attribute__((weak,
    alias("__mlib_SignalLMSFilterNonAdapt_S16S_S16S_Sat")));
__typeof__(__mlib_SignalLMSFilterNonAdapt_S16_S16_Sat)
    mlib_SignalLMSFilterNonAdapt_S16_S16_Sat
    __attribute__((weak, alias("__mlib_SignalLMSFilterNonAdapt_S16_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_CLAMP_S16(DST, SRC)	CLAMP_S16(DST, SRC)

/* *********************************************************** */

void
mlib_SignalFIR_s16_to_d64S(
    mlib_d64 *pdst0,
    mlib_d64 *pdst1,
    mlib_s16 *psrc,
    mlib_s32 n)
{
	mlib_s32 i;

#ifdef _NO_LONGLONG
	mlib_u32 data;

	if (((mlib_addr)psrc & 3) != 0) {
		mlib_d64 *tmp_ptr;

		*pdst0 = (mlib_d64)((*psrc++));
		tmp_ptr = pdst0 + 1;
		pdst0 = pdst1;
		pdst1 = tmp_ptr;
		n--;
	}

	for (i = 0; i < n - 1; i += 2) {
		data = *(mlib_u32 *)psrc;

#ifdef _LITTLE_ENDIAN
		pdst1[0] = (mlib_d64)(mlib_s16)(data >> 16);
		pdst0[0] = (mlib_d64)(mlib_s16)(data);
#else /* _LITTLE_ENDIAN */
		pdst0[0] = (mlib_d64)(mlib_s16)(data >> 16);
		pdst1[0] = (mlib_d64)(mlib_s16)(data);
#endif /* _LITTLE_ENDIAN */

		psrc += 2;
		pdst0 += 1;
		pdst1 += 1;
	}

	if (i < n)
		*pdst0 = (mlib_d64)*psrc;

#else /* _NO_LONGLONG */
	mlib_u64 data;

	for (i = 0; i < n && (7 & (mlib_addr)psrc) != 0; i++) {
		mlib_d64 *tmp_ptr;

		*pdst0 = (mlib_d64)((*psrc++));
		tmp_ptr = pdst0 + 1;
		pdst0 = pdst1;
		pdst1 = tmp_ptr;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n - 3; i += 4) {
		data = *(mlib_u64 *)psrc;

#ifdef _LITTLE_ENDIAN
		pdst1[1] = (mlib_d64)(mlib_s16)(data >> 48);
		pdst0[1] = (mlib_d64)(mlib_s16)(data >> 32);
		pdst1[0] = (mlib_d64)(mlib_s16)(data >> 16);
		pdst0[0] = (mlib_d64)(mlib_s16)(data);
#else /* _LITTLE_ENDIAN */
		pdst0[0] = (mlib_d64)(mlib_s16)(data >> 48);
		pdst1[0] = (mlib_d64)(mlib_s16)(data >> 32);
		pdst0[1] = (mlib_d64)(mlib_s16)(data >> 16);
		pdst1[1] = (mlib_d64)(mlib_s16)(data);
#endif /* _LITTLE_ENDIAN */

		psrc += 4;
		pdst0 += 2;
		pdst1 += 2;
	}

	for (; i < n; i++) {
		mlib_d64 *tmp_ptr;

		*pdst0 = (mlib_d64)((*psrc++));
		tmp_ptr = pdst0 + 1;
		pdst0 = pdst1;
		pdst1 = tmp_ptr;
	}
#endif /* _NO_LONGLONG */
}

/* *********************************************************** */

void
mlib_SignalFIR_d64_to_s16S(
    mlib_s16 *pdst,
    mlib_d64 *psrc0,
    mlib_d64 *psrc1,
    mlib_s32 n)
{
	mlib_s32 i;

#ifndef MLIB_USE_FTOI_CLAMPING
	for (i = 0; i < n - 1; i += 2) {
		CLAMP_S16(pdst[i], psrc0[i >> 1]);
		CLAMP_S16(pdst[i + 1], psrc1[i >> 1]);
	}

	if (i < n)
		CLAMP_S16(pdst[i], psrc0[i >> 1]);

#else /* MLIB_USE_FTOI_CLAMPING */

#ifdef _NO_LONGLONG
	mlib_u32 data;

	if (((mlib_addr)pdst & 3) != 0) {
		mlib_d64 *tmp_ptr;

		((*pdst++)) = ((mlib_s32)(*psrc0)) >> 16;
		tmp_ptr = psrc0 + 1;
		psrc0 = psrc1;
		psrc1 = tmp_ptr;
		n--;
	}

	for (i = 0; i < n - 1; i += 2) {
		data = ((mlib_u32)(mlib_s32)psrc0[0]) & ~0xFFFF;
		data += (((mlib_u32)(mlib_s32)psrc1[0]) >> 16);

		*(mlib_u32 *)pdst = data;
		psrc0 += 1;
		psrc1 += 1;
		pdst += 2;
	}

	if (i < n)
		*pdst = ((mlib_s32)*psrc0) >> 16;

#else /* _NO_LONGLONG */

	mlib_u64 data;

	for (i = 0; i < n && ((mlib_addr)pdst & 7) != 0; i++) {
		mlib_d64 *tmp_ptr;

		((*pdst++)) = ((mlib_s32)(*psrc0)) >> 16;
		tmp_ptr = psrc0 + 1;
		psrc0 = psrc1;
		psrc1 = tmp_ptr;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n - 3; i += 4) {
		data = ((mlib_u32)(mlib_s32)psrc0[0]) & ~0xFFFF;
		data += (((mlib_u32)(mlib_s32)psrc1[0]) >> 16);
		data <<= 16;
		data += (((mlib_u32)(mlib_s32)psrc0[1]) >> 16);
		data <<= 16;
		data += (((mlib_u32)(mlib_s32)psrc1[1]) >> 16);

		*(mlib_u64 *)pdst = data;
		psrc0 += 2;
		psrc1 += 2;
		pdst += 4;
	}

	for (; i < n; i++) {
		mlib_d64 *tmp_ptr;

		((*pdst++)) = ((mlib_s32)(*psrc0)) >> 16;
		tmp_ptr = psrc0 + 1;
		psrc0 = psrc1;
		psrc1 = tmp_ptr;
	}

#endif /* _NO_LONGLONG */
#endif /* MLIB_USE_FTOI_CLAMPING */
}

/* *********************************************************** */

mlib_status
__mlib_SignalLMSFilter_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *ref,
    void *filter,
    mlib_s32 n)
{
	LMS_Filter *pLMS_Filter = (LMS_Filter *) filter;
	mlib_d64 U = pLMS_Filter->U;
	mlib_d64 BE0;
	mlib_d64 *W;
	mlib_d64 *srcBuffer;
	mlib_d64 D, Y;
	mlib_s32 i, t, off, count;
	mlib_s32 tap, bufferSize;
	mlib_s16 *psrc, *pdst, *pref;

	if (src == NULL || dst == NULL || ref == NULL)
		return (MLIB_NULLPOINTER);

	if (n <= 0)
		return (MLIB_FAILURE);

	tap = pLMS_Filter->tap;
	bufferSize = pLMS_Filter->bufferSize;
	W = pLMS_Filter->data;
	srcBuffer = (W + 2 * tap);
	BE0 = pLMS_Filter->BE[0];

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

		for (t = 0; t < count; t++) {
			D = 0;
			Y = pref[t] * (mlib_d64)SAT_SCALE_S16;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < tap; i++) {
				W[i] += BE0 * srcBuffer[t - i - 1];
				D += srcBuffer[t - i] * W[i];
			}

			BE0 = (Y - D) * U;

			MLIB_CLAMP_S16(pdst[t], D);
		}

		psrc += count;
		pdst += count;
		pref += count;

		for (i = 0; i < tap; i++) {
			srcBuffer[i - tap] = srcBuffer[count + (i - tap)];
		}
	}

	pLMS_Filter->BE[0] = BE0;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLMSFilter_S16S_S16S_Sat(
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

			for (t = 0; t < count; t++) {
				D = 0;
				Y = pref[2 * t + j] * (mlib_d64)SAT_SCALE_S16;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < tap; i++) {
					pW[i] += BE0 * psrcBuffer[t - i - 1];
					D += psrcBuffer[t - i] * pW[i];
				}

				BE0 = (Y - D) * U;

				MLIB_CLAMP_S16(pdst[2 * t + j], D);
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

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLMSFilterNonAdapt_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *ref,
    void *filter,
    mlib_s32 n)
{
	LMS_Filter *pLMS_Filter = (LMS_Filter *) filter;
	mlib_d64 *W, *srcBuffer, *dstBuffer;
	mlib_s32 i, k, off, count;
	mlib_s32 tap, bufferSize;
	mlib_s16 *psrc, *pdst;

	if (src == NULL || dst == NULL)
		return (MLIB_NULLPOINTER);

	if (n <= 0)
		return (MLIB_FAILURE);

	tap = pLMS_Filter->tap;
	bufferSize = pLMS_Filter->bufferSize;

	W = pLMS_Filter->data;
	srcBuffer = (W + 2 * tap);
	dstBuffer = srcBuffer + bufferSize;

	psrc = (mlib_s16 *)src;
	pdst = (mlib_s16 *)dst;

	for (off = 0; off < n; off += count) {

		count = n - off;
		if (count > bufferSize) {
			count = bufferSize;
		}

		mlib_SignalFIR_s16_to_d64((void *)srcBuffer, (void *)psrc,
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

		mlib_SignalFIR_d64_to_s16(pdst, dstBuffer, count);

		psrc += count;
		pdst += count;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLMSFilterNonAdapt_S16S_S16S_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *ref,
    void *filter,
    mlib_s32 n)
{
	LMS_Filter *pLMS_Filter = (LMS_Filter *) filter;
	mlib_d64 *W[2], *srcBuffer[2], *dstBuffer[2];
	mlib_s32 i, k, off, count;
	mlib_s32 tap, bufferSize;
	mlib_s16 *psrc, *pdst;

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

	psrc = (mlib_s16 *)src;
	pdst = (mlib_s16 *)dst;

	for (off = 0; off < n; off += count) {

		count = n - off;
		if (count > bufferSize) {
			count = bufferSize;
		}

		mlib_SignalFIR_s16_to_d64S((void *)srcBuffer[0],
		    (void *)srcBuffer[1], (void *)psrc, count * 2);

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

		mlib_SignalFIR_d64_to_s16S(pdst, (mlib_d64 *)dstBuffer[0],
		    (mlib_d64 *)dstBuffer[1], count * 2);

		psrc += 2 * count;
		pdst += 2 * count;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLMSInit_common(
    void **filter,
    const mlib_f32 *flt,
    mlib_s32 tap,
    mlib_f32 beta,
    mlib_d64 beta_scale,
    mlib_d64 scale,
    mlib_s32 stereo)
{
	mlib_s32 bufferSize = LMS_BUFFER_SIZE;
	mlib_s32 i, j, memSize;
	mlib_d64 *pdbl;
	LMS_Filter *lms_data_ptr;
	mlib_s32 struct_size;

	*filter = NULL;

	if (tap <= 0) {
		return (MLIB_OUTOFRANGE);
	}

	struct_size = (sizeof (LMS_Filter) + 7) & ~7;

	memSize = struct_size;
	memSize += ((tap + bufferSize) << (stereo + 1)) * sizeof (mlib_d64);

	lms_data_ptr = (LMS_Filter *) __mlib_malloc(memSize);

	if (lms_data_ptr == NULL) {
		return (MLIB_NULLPOINTER);
	}

	pdbl = (mlib_d64 *)((mlib_u8 *)lms_data_ptr + struct_size);

	lms_data_ptr->tap = tap;
	lms_data_ptr->bufferSize = bufferSize;
	lms_data_ptr->data = pdbl;

	*filter = lms_data_ptr;

	for (j = 0; j <= stereo; j++) {
		for (i = 0; i < tap; i++) {
			pdbl[i] = flt[i] * scale;
		}

/* clear start of src buffer */
		for (i = 0; i < tap; i++) {
			pdbl[i + tap] = 0;
		}

		pdbl += (tap + bufferSize) * 2;
	}

	lms_data_ptr->U = 2 * beta * beta_scale;
	lms_data_ptr->BE[0] = 0;
	lms_data_ptr->BE[1] = 0;
	lms_data_ptr->pow[0] = 0;
	lms_data_ptr->pow[1] = 0;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLMSFilterInit_S16_S16(
    void **filter,
    const mlib_f32 *flt,
    mlib_s32 tap,
    mlib_f32 beta)
{
	return (__mlib_SignalLMSInit_common(filter, flt, tap, beta,
	    POWER, SAT_SCALE_S16, 0));
}

/* *********************************************************** */

mlib_status
__mlib_SignalLMSFilterInit_S16S_S16S(
    void **filter,
    const mlib_f32 *flt,
    mlib_s32 tap,
    mlib_f32 beta)
{
	return (__mlib_SignalLMSInit_common(filter, flt, tap, beta,
	    POWER, SAT_SCALE_S16, 1));
}

/* *********************************************************** */

void
__mlib_SignalLMSFilterFree_S16_S16(
    void *filter)
{
	__mlib_free(filter);
}

/* *********************************************************** */

void
__mlib_SignalLMSFilterFree_S16S_S16S(
    void *filter)
{
	__mlib_free(filter);
}

/* *********************************************************** */
