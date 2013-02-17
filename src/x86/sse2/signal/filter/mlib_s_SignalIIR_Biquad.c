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

#pragma ident	"@(#)mlib_s_SignalIIR_Biquad.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalIIRInit_Biquad_[S16|S16S]_[S16|S16S] - allocate the
 *           memory for internal filter structure and convert the
 *           filter coefficients into internal representation
 *
 *      mlib_SignalIIR_Biquad_[S16|S16S]_[S16|S16S] - apply the IIR
 *           filter one packet of signal and update the filter states
 *
 *      mlib_SignalIIRFree_Biquad_[S16|S16S]_[S16|S16S] - release the
 *           memory allocated for the internal filter structure
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalIIRInit_Biquad_S16S_S16S(void           **filter,
 *                                                      const mlib_f32 *flt);
 *      mlib_status mlib_SignalIIRInit_Biquad_S16_S16(void           **filter,
 *                                                    const mlib_f32 *flt);
 *      mlib_status mlib_SignalIIR_Biquad_S16S_S16S_Sat(mlib_s16       *dst,
 *                                                      const mlib_s16 *src,
 *                                                      void           *filter,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_SignalIIR_Biquad_S16_S16_Sat(mlib_s16       *dst,
 *                                                    const mlib_s16 *src,
 *                                                    void           *filter,
 *                                                    mlib_s32       n);
 *      void mlib_SignalIIRFree_Biquad_S16S_S16S(void *filter);
 *      void mlib_SignalIIRFree_Biquad_S16_S16(void *filter);
 *
 * ARGUMENTS
 *      filter   Internal filter structure
 *      flt      Filter coefficient array
 *      dst      Output signal array
 *      src      Input signal array
 *      n        Number of samples in the input signal array
 *
 * DESCRIPTION
 *             N                   M
 *     Z(n) = SUM a(k) * x(n-k) + SUM b(k) * z(n - k)   n = 0, 1, 2, ...
 *            k = 0               k = 1
 *
 *     Z     - output signal array
 *     a(k),
 *     b(k)  - filter coefficients
 *     x(i)  - input signal array
 *
 * ALGORITHM
 *
 *     z(n) = a0 * x(n) + a1 * x(n - 1) + a2 * x(n - 2) +
 *            b1 * y(n - 1) + b2 * y(n - 2)
 *
 * INTERNAL FILTER STRUCTURE (S16_S16)
 *
 *   filter.a0 = a[0]
 *   filter.a1 = a[1]
 *   filter.a2 = a[2]
 *   filter.b1 = b[1]
 *   filter.b2 = b[2]
 *   filter.x1 = x[1]
 *   filter.x2 = x[2]
 *   filter.y1 = y[1]
 *   filter.y2 = y[2]
 *
 * INTERNAL FILTER STRUCTURE (S16S_S16S)
 *
 *   filter.a0  = a[0]
 *   filter.a1  = a[1]
 *   filter.a2  = a[2]
 *   filter.b1  = b[1]
 *   filter.b2  = b[2]
 *   filter.x10 = x_0[1]
 *   filter.x20 = x_0[2]
 *   filter.x11 = x_1[1]
 *   filter.x21 = x_1[2]
 *   filter.y10 = y_0[1]
 *   filter.y20 = y_0[2]
 *   filter.y11 = y_1[1]
 *   filter.y21 = y_1[2]
 */

#include <mlib_signal.h>
#include <mlib_s_SignalFIR.h>
#include <mlib_sse_utils.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalIIRFree_Biquad_S16S_S16S = \
	__mlib_SignalIIRFree_Biquad_S16S_S16S
#pragma weak mlib_SignalIIRFree_Biquad_S16_S16 = \
	__mlib_SignalIIRFree_Biquad_S16_S16
#pragma weak mlib_SignalIIRInit_Biquad_S16S_S16S = \
	__mlib_SignalIIRInit_Biquad_S16S_S16S
#pragma weak mlib_SignalIIRInit_Biquad_S16_S16 = \
	__mlib_SignalIIRInit_Biquad_S16_S16
#pragma weak mlib_SignalIIR_Biquad_S16S_S16S_Sat = \
	__mlib_SignalIIR_Biquad_S16S_S16S_Sat
#pragma weak mlib_SignalIIR_Biquad_S16_S16_Sat = \
	__mlib_SignalIIR_Biquad_S16_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalIIRFree_Biquad_S16S_S16S)
    mlib_SignalIIRFree_Biquad_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalIIRFree_Biquad_S16S_S16S")));
__typeof__(__mlib_SignalIIRFree_Biquad_S16_S16)
    mlib_SignalIIRFree_Biquad_S16_S16
    __attribute__((weak, alias("__mlib_SignalIIRFree_Biquad_S16_S16")));
__typeof__(__mlib_SignalIIRInit_Biquad_S16S_S16S)
    mlib_SignalIIRInit_Biquad_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalIIRInit_Biquad_S16S_S16S")));
__typeof__(__mlib_SignalIIRInit_Biquad_S16_S16)
    mlib_SignalIIRInit_Biquad_S16_S16
    __attribute__((weak, alias("__mlib_SignalIIRInit_Biquad_S16_S16")));
__typeof__(__mlib_SignalIIR_Biquad_S16S_S16S_Sat)
    mlib_SignalIIR_Biquad_S16S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalIIR_Biquad_S16S_S16S_Sat")));
__typeof__(__mlib_SignalIIR_Biquad_S16_S16_Sat)
    mlib_SignalIIR_Biquad_S16_S16_Sat
    __attribute__((weak, alias("__mlib_SignalIIR_Biquad_S16_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

typedef struct
{
/* coeff filter */
	mlib_d64 a0;
	mlib_d64 a1;
	mlib_d64 a2;
	mlib_d64 b1;
	mlib_d64 b2;
/* delay element */
	mlib_d64 x1;
	mlib_d64 x2;
	mlib_d64 y1;
	mlib_d64 y2;
} mlib_IIR_filt_S16;

/* *********************************************************** */

typedef struct
{
/* coeff filter */
	mlib_d64 a0;
	mlib_d64 a1;
	mlib_d64 a2;
	mlib_d64 b1;
	mlib_d64 b2;
/* delay element */
	mlib_d64 x10;
	mlib_d64 x20;
	mlib_d64 x11;
	mlib_d64 x21;
	mlib_d64 y10;
	mlib_d64 y20;
	mlib_d64 y11;
	mlib_d64 y21;
} mlib_IIR_filt_S16S;

/* *********************************************************** */

/* Mono signal */

/* *********************************************************** */

mlib_status
__mlib_SignalIIRInit_Biquad_S16_S16(
    void **filter,
    const mlib_f32 *flt)
{
	mlib_s32 i;
	mlib_IIR_filt_S16 *pflt;

	if (flt == NULL)
		return (MLIB_NULLPOINTER);

	*filter = (void *)__mlib_malloc(sizeof (mlib_IIR_filt_S16));

	if (*filter == NULL)
		return (MLIB_NULLPOINTER);

	pflt = (mlib_IIR_filt_S16 *) (*filter);

	for (i = 0; i < 5; i++) {

/* check value Inf and NaN */

		if (flt[i] - flt[i]) {
			__mlib_free(filter);
			return (MLIB_OUTOFRANGE);
		}
	}

	pflt->a0 = flt[0];
	pflt->a1 = flt[1];
	pflt->a2 = flt[2];
	pflt->b1 = flt[3];
	pflt->b2 = flt[4];
	pflt->x1 = 0;
	pflt->x2 = 0;
	pflt->y1 = 0;
	pflt->y2 = 0;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIIR_Biquad_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    void *filter,
    mlib_s32 n)
{
#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_s32 d;
#endif /* MLIB_USE_FTOI_CLAMPING */
	mlib_d64 a0, a1, a2, b1, b2, x1, x2, y1, y2, r0, x0;

	mlib_s32 i, j, tmp;
	__m128d sa0, sa1, sa2, sb1, sb2, sx1, sx2, sy1, sy2, sr0, sx0;
	__m128d stmp1, stmp2, stmp3, stmp4, stmp5;
	mlib_d64 tr0[2], tx0[2], tx1[2], tx2[2], ty1[2], ty2[2];

	mlib_IIR_filt_S16 *pflt = (mlib_IIR_filt_S16 *) filter;

	if (filter == NULL || src == NULL || dst == NULL)
		return (MLIB_NULLPOINTER);

	if (n <= 0)
		return (MLIB_OUTOFRANGE);

	a0 = pflt->a0;
	a1 = pflt->a1;
	a2 = pflt->a2;

	b1 = pflt->b1;
	b2 = pflt->b2;

	x1 = pflt->x1;
	x2 = pflt->x2;
	y1 = pflt->y1;
	y2 = pflt->y2;

	for (j = 0; (j < n) &&(j < 2); j++) {
		x0 = src[j];

		r0 = a0 * x0 + a1 * x1 + a2 * x2 + b2 * y2 + b1 * y1;

		x2 = x1;
		x1 = x0;

		y2 = y1;
		y1 = r0;

#ifndef MLIB_USE_FTOI_CLAMPING

		if (r0 > MLIB_S16_MAX)
			d = MLIB_S16_MAX;
		else if (r0 < MLIB_S16_MIN)
			d = MLIB_S16_MIN;
		else
			d = (mlib_s16)r0;

		dst[j] = d;
#else /* MLIB_USE_FTOI_CLAMPING */
		dst[j] = ((mlib_s16)r0);
#endif /* MLIB_USE_FTOI_CLAMPING */
	}

	sa0 = _mm_set1_pd(a0);
	sa1 = _mm_set1_pd(a1);
	sa2 = _mm_set1_pd(a2);
	sb1 = _mm_set1_pd(b1);
	sb2 = _mm_set1_pd(b2);

	sx0 = _mm_setr_pd(src[j], src[j + 1]);
	sx1 = _mm_setr_pd(x1, src[j]);
	sx2 = _mm_setr_pd(x2, x1);
	sy1 = _mm_setr_pd(y1, 0);
	sy2 = _mm_setr_pd(y2, y1);

	for (i = j; i < n - 1; i += 2) {
		stmp1 = _mm_mul_pd(sa0, sx0);
		stmp2 = _mm_mul_pd(sa1, sx1);
		stmp3 = _mm_mul_pd(sa2, sx2);
		stmp4 = _mm_mul_pd(sb1, sy1);
		stmp5 = _mm_mul_pd(sb2, sy2);

		stmp1 = _mm_add_pd(stmp1, stmp2);
		stmp3 = _mm_add_pd(stmp3, stmp4);
		stmp1 = _mm_add_pd(stmp1, stmp3);
		sr0 = _mm_add_pd(stmp1, stmp5);

		_mm_storeu_pd(tr0, sr0);
		tr0[1] += (b1 * tr0[0]);

		_mm_storeu_pd(tx0, sx0);

		sx2 = sx0;
		sx1 = _mm_setr_pd(tx0[1], src[i + 2]);
		sx0 = _mm_setr_pd(src[i + 2], src[i + 3]);
		sy1 = _mm_setr_pd(tr0[1], 0);
		sy2 = _mm_setr_pd(tr0[0], tr0[1]);

#ifndef MLIB_USE_FTOI_CLAMPING
		if (tr0[0] > MLIB_S16_MAX)
			d = MLIB_S16_MAX;
		else if (tr0[0] < MLIB_S16_MIN)
			d = MLIB_S16_MIN;
		else
			d = (mlib_s16)tr0[0];

		dst[i] = d;
#else /* MLIB_USE_FTOI_CLAMPING */
		dst[i] = (mlib_s16)tr0[0];
#endif /* MLIB_USE_FTOI_CLAMPING */

#ifndef MLIB_USE_FTOI_CLAMPING
		if (tr0[1] > MLIB_S16_MAX)
			d = MLIB_S16_MAX;
		else if (tr0[1] < MLIB_S16_MIN)
			d = MLIB_S16_MIN;
		else
			d = (mlib_s16)tr0[1];

		dst[i + 1] = d;
#else /* MLIB_USE_FTOI_CLAMPING */
		dst[i + 1] = (mlib_s16)tr0[1];
#endif /* MLIB_USE_FTOI_CLAMPING */
	}

		_mm_storeu_pd(tx1, sx1);
		x1 = tx1[0];
		_mm_storeu_pd(tx2, sx2);
		x2 = tx2[0];

		_mm_storeu_pd(ty1, sy1);
		y1 = ty1[0];
		_mm_storeu_pd(ty2, sy2);
		y2 = ty2[0];

	for (; (i < n); i++) {
		x0 = src[i];

		r0 = a0 * x0 + a1 * x1 + a2 * x2 + b2 * y2 + b1 * y1;

		x2 = x1;
		x1 = x0;

		y2 = y1;
		y1 = r0;

#ifndef MLIB_USE_FTOI_CLAMPING

		if (r0 > MLIB_S16_MAX)
			d = MLIB_S16_MAX;
		else if (r0 < MLIB_S16_MIN)
			d = MLIB_S16_MIN;
		else
			d = (mlib_s16)r0;

		dst[i] = d;
#else /* MLIB_USE_FTOI_CLAMPING */
		dst[i] = ((mlib_s16)r0);
#endif /* MLIB_USE_FTOI_CLAMPING */
	}

	pflt->x1 = x1;
	pflt->x2 = x2;
	pflt->y1 = y1;
	pflt->y2 = y2;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
__mlib_SignalIIRFree_Biquad_S16_S16(
    void *filter)
{
	__mlib_free(filter);
}

/* *********************************************************** */

/* Stereo signal */

/* *********************************************************** */

mlib_status
__mlib_SignalIIRInit_Biquad_S16S_S16S(
    void **filter,
    const mlib_f32 *flt)
{
	mlib_s32 i;
	mlib_IIR_filt_S16S *pflt;

	if (flt == NULL)
		return (MLIB_NULLPOINTER);

	*filter = (void *)__mlib_malloc(sizeof (mlib_IIR_filt_S16S));

	if (*filter == NULL)
		return (MLIB_NULLPOINTER);

	pflt = (mlib_IIR_filt_S16S *) (*filter);

	for (i = 0; i < 5; i++) {

/* check value Inf and NaN */

		if (flt[i] - flt[i]) {
			__mlib_free(filter);
			return (MLIB_OUTOFRANGE);
		}
	}

	pflt->a0 = flt[0];
	pflt->a1 = flt[1];
	pflt->a2 = flt[2];
	pflt->b1 = flt[3];
	pflt->b2 = flt[4];

	pflt->x10 = 0;
	pflt->x20 = 0;
	pflt->x11 = 0;
	pflt->x21 = 0;

	pflt->y10 = 0;
	pflt->y20 = 0;
	pflt->y11 = 0;
	pflt->y21 = 0;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIIR_Biquad_S16S_S16S_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    void *filter,
    mlib_s32 n)
{
	mlib_s32 i, tmp;

#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_s32 d0, d1;
#endif /* MLIB_USE_FTOI_CLAMPING */
	mlib_d64 a0, a1, a2, b1, b2, x00, x01, x10, x11, x20, x21, y10, y11,
	    y20, y21, r00, r01;

	__m128d sa0, sa1, sa2, sb1, sb2;
	__m128d sx1, sx2, sy1, sy2, sr0, sx0;
	__m128d stmp1, stmp2, stmp3, stmp4, stmp5;
	mlib_d64 tx1[2], tx2[2], ty1[2], ty2[2], tr0[2];

	mlib_IIR_filt_S16S *pflt = (mlib_IIR_filt_S16S *) filter;

	if (filter == NULL || src == NULL || dst == NULL)
		return (MLIB_NULLPOINTER);

	if (n <= 0)
		return (MLIB_OUTOFRANGE);

	n *= 2;

	a0 = pflt->a0;
	a1 = pflt->a1;
	a2 = pflt->a2;

	b1 = pflt->b1;
	b2 = pflt->b2;

	x10 = pflt->x10;
	x20 = pflt->x20;
	y10 = pflt->y10;
	y20 = pflt->y20;
	x11 = pflt->x11;
	x21 = pflt->x21;
	y11 = pflt->y11;
	y21 = pflt->y21;

	sa0 = _mm_set1_pd(a0);
	sa1 = _mm_set1_pd(a1);
	sa2 = _mm_set1_pd(a2);
	sb1 = _mm_set1_pd(b1);
	sb2 = _mm_set1_pd(b2);

	sx1 = _mm_setr_pd(x10, x11);
	sx2 = _mm_setr_pd(x20, x21);
	sx0 = _mm_setr_pd(src[0], src[1]);
	sy1 = _mm_setr_pd(y10, y11);
	sy2 = _mm_setr_pd(y20, y21);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i += 2) {
		stmp1 = _mm_mul_pd(sa0, sx0);
		stmp2 = _mm_mul_pd(sa1, sx1);
		stmp3 = _mm_mul_pd(sa2, sx2);
		stmp4 = _mm_mul_pd(sb2, sy2);
		stmp5 = _mm_mul_pd(sb1, sy1);
		stmp1 = _mm_add_pd(stmp1, stmp2);
		stmp3 = _mm_add_pd(stmp3, stmp4);
		stmp1 = _mm_add_pd(stmp1, stmp3);
		sr0 = _mm_add_pd(stmp1, stmp5);

		sx2 = sx1;
		sx1 = sx0;
		sx0 = _mm_setr_pd(src[2], src[3]);
		sy2 = sy1;
		sy1 = sr0;

		_mm_storeu_pd(tr0, sr0);
#ifndef MLIB_USE_FTOI_CLAMPING
		if (tr0[0] > MLIB_S16_MAX)
			d0 = MLIB_S16_MAX;
		else if (tr0[0] < MLIB_S16_MIN)
			d0 = MLIB_S16_MIN;
		else
			d0 = (mlib_s16)tr0[0];

		dst[i] = d0;
#else /* MLIB_USE_FTOI_CLAMPING */
		dst[i] = (mlib_s16)tr0[0];
#endif /* MLIB_USE_FTOI_CLAMPING */

#ifndef MLIB_USE_FTOI_CLAMPING
		if (tr0[1] > MLIB_S16_MAX)
			d1 = MLIB_S16_MAX;
		else if (tr0[1] < MLIB_S16_MIN)
			d1 = MLIB_S16_MIN;
		else
			d1 = (mlib_s16)tr0[1];

		dst[i + 1] = d1;
#else /* MLIB_USE_FTOI_CLAMPING */
		dst[i + 1] = (mlib_s16)tr0[1];
#endif /* MLIB_USE_FTOI_CLAMPING */

		src += 2;
	}

	_mm_storeu_pd(tx1, sx1);
	_mm_storeu_pd(tx2, sx2);
	_mm_storeu_pd(ty1, sy1);
	_mm_storeu_pd(ty2, sy2);
	pflt->x10 = tx1[0];
	pflt->x11 = tx1[1];
	pflt->x20 = tx2[0];
	pflt->x21 = tx2[1];

	pflt->y10 = ty1[0];
	pflt->y11 = ty1[1];
	pflt->y20 = ty2[0];
	pflt->y21 = ty2[1];

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
__mlib_SignalIIRFree_Biquad_S16S_S16S(
    void *filter)
{
	__mlib_free(filter);
}

/* *********************************************************** */
