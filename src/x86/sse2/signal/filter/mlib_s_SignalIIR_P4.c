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

#pragma ident	"@(#)mlib_s_SignalIIR_P4.c	9.7	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalIIRInit_P4_[S16|S16S]_[S16|S16S] - allocate the
 *           memory for internal filter structure and convert the
 *           filter coefficients into internal representation
 *
 *      mlib_SignalIIR_P4_[S16|S16S]_[S16|S16S] - apply the IIR filter
 *           one packet of signal and update the filter states
 *
 *      mlib_SignalIIRFree_P4_[S16|S16S]_[S16|S16S] - release the memory
 *           allocated for the internal filter structure
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalIIRInit_P4_S16S_S16S(void           **filter,
 *                                                  const mlib_f32 *flt);
 *      mlib_status mlib_SignalIIRInit_P4_S16_S16(void           **filter,
 *                                                const mlib_f32 *flt);
 *      mlib_status mlib_SignalIIR_P4_S16S_S16S_Sat(mlib_s16       *dst,
 *                                                  const mlib_s16 *src,
 *                                                  void           *filter,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_SignalIIR_P4_S16_S16_Sat(mlib_s16       *dst,
 *                                                const mlib_s16 *src,
 *                                                void           *filter,
 *                                                mlib_s32       n);
 *      void mlib_SignalIIRFree_P4_S16S_S16S(void *filter);
 *      void mlib_SignalIIRFree_P4_S16_S16(void *filter);
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
 *            k = 0                 k = 1
 *
 *     Z     - output signal array
 *     a(k),
 *     b(k)  - filter coefficients
 *     x(i)  - input signal array
 *
 * ALGORITHM
 *
 *     z1(n) = a00 * x(n) + a10 * x(n - 1) +
 *             b10 * z1(n - 1) + b20 * z1(n - 2)
 *     z2(n) = a01 * x(n) + a11 * x(n - 1) +
 *             b11 * z2(n - 1) + b21 * z2(n - 2)
 *     z(n) = C * x(n) + z1(n) + z2(n)
 *
 * INTERNAL FILTER STRUCTURE (S16_S16)
 *
 *   filter.C   = C
 *   filter.a00 = a[0]
 *   filter.a01 = a[1]
 *   filter.a10 = a[2]
 *   filter.a11 = a[3]
 *   filter.a01 = a[4]
 *   filter.b10 = b[1]
 *   filter.b11 = b[2]
 *   filter.b20 = b[3]
 *   filter.b21 = b[4]
 *   filter.y11 = z1[1]
 *   filter.y12 = z1[2]
 *   filter.y21 = z2[1]
 *   filter.y22 = z2[2]
 *   filter.x1  = x[1]
 *
 * INTERNAL FILTER STRUCTURE (S16S_S16S)
 *
 *   filter.C   = C
 *   filter.a00 = a[0]
 *   filter.a01 = a[1]
 *   filter.a10 = a[2]
 *   filter.a11 = a[3]
 *   filter.a01 = a[4]
 *   filter.b10 = b[1]
 *   filter.b11 = b[2]
 *   filter.b20 = b[3]
 *   filter.b21 = b[4]
 *   filter.y11_0 = z1[1]
 *   filter.y12_0 = z1[2]
 *   filter.y21_0 = z1[3]
 *   filter.y22_0 = z1[4]
 *   filter.y11_1 = z2[1]
 *   filter.y12_1 = z2[2]
 *   filter.y21_1 = z2[3]
 *   filter.y22_1 = z2[4]
 *   filter.x1    = x1[1]
 *   filter.x2    = x2[1]
 */

#include <mlib_signal.h>
#include <mlib_Utils.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif
#include <mlib_s_SignalFIR.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalIIRFree_P4_S16S_S16S = \
	__mlib_SignalIIRFree_P4_S16S_S16S
#pragma weak mlib_SignalIIRFree_P4_S16_S16 = __mlib_SignalIIRFree_P4_S16_S16
#pragma weak mlib_SignalIIRInit_P4_S16S_S16S = \
	__mlib_SignalIIRInit_P4_S16S_S16S
#pragma weak mlib_SignalIIRInit_P4_S16_S16 = __mlib_SignalIIRInit_P4_S16_S16
#pragma weak mlib_SignalIIR_P4_S16S_S16S_Sat = \
	__mlib_SignalIIR_P4_S16S_S16S_Sat
#pragma weak mlib_SignalIIR_P4_S16_S16_Sat = __mlib_SignalIIR_P4_S16_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalIIRFree_P4_S16S_S16S) mlib_SignalIIRFree_P4_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalIIRFree_P4_S16S_S16S")));
__typeof__(__mlib_SignalIIRFree_P4_S16_S16) mlib_SignalIIRFree_P4_S16_S16
    __attribute__((weak, alias("__mlib_SignalIIRFree_P4_S16_S16")));
__typeof__(__mlib_SignalIIRInit_P4_S16S_S16S) mlib_SignalIIRInit_P4_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalIIRInit_P4_S16S_S16S")));
__typeof__(__mlib_SignalIIRInit_P4_S16_S16) mlib_SignalIIRInit_P4_S16_S16
    __attribute__((weak, alias("__mlib_SignalIIRInit_P4_S16_S16")));
__typeof__(__mlib_SignalIIR_P4_S16S_S16S_Sat) mlib_SignalIIR_P4_S16S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalIIR_P4_S16S_S16S_Sat")));
__typeof__(__mlib_SignalIIR_P4_S16_S16_Sat) mlib_SignalIIR_P4_S16_S16_Sat
    __attribute__((weak, alias("__mlib_SignalIIR_P4_S16_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

typedef struct
{
/* coeff filter */
	mlib_d64 C;
	mlib_d64 a00;
	mlib_d64 a01;
	mlib_d64 a10;
	mlib_d64 a11;
	mlib_d64 b10;
	mlib_d64 b11;
	mlib_d64 b20;
	mlib_d64 b21;
/* delay element */
	mlib_d64 y10;
	mlib_d64 y20;
	mlib_d64 y11;
	mlib_d64 y21;
	mlib_d64 x1;
} mlib_IIR_filt_S16_P4;

/* *********************************************************** */

typedef struct
{
/* coeff filter */
	mlib_d64 C;
	mlib_d64 a00;
	mlib_d64 a01;
	mlib_d64 a10;
	mlib_d64 a11;
	mlib_d64 b10;
	mlib_d64 b11;
	mlib_d64 b20;
	mlib_d64 b21;
/* delay element */
	mlib_d64 y10_0;
	mlib_d64 y20_0;
	mlib_d64 y11_0;
	mlib_d64 y21_0;
	mlib_d64 y10_1;
	mlib_d64 y20_1;
	mlib_d64 y11_1;
	mlib_d64 y21_1;
	mlib_d64 x1_0;
	mlib_d64 x1_1;
	mlib_u8 flags;
} mlib_IIR_filt_S16S_P4;

/* *********************************************************** */

/* Mono signal */

/* *********************************************************** */

mlib_status
__mlib_SignalIIRInit_P4_S16_S16(
    void **filter,
    const mlib_f32 *flt)
{
	mlib_IIR_filt_S16_P4 *pflt;

	if (flt == NULL)
		return (MLIB_NULLPOINTER);

	*filter = (void *)__mlib_malloc(sizeof (mlib_IIR_filt_S16_P4));

	if (*filter == NULL)
		return (MLIB_NULLPOINTER);

	pflt = (mlib_IIR_filt_S16_P4 *) (*filter);

	pflt->C = flt[0] * SAT_SCALE_S16;

	pflt->a00 = flt[1] * SAT_SCALE_S16;
	pflt->a10 = flt[2] * SAT_SCALE_S16;
	pflt->b10 = flt[3];
	pflt->b20 = flt[4];

	pflt->a01 = flt[5] * SAT_SCALE_S16;
	pflt->a11 = flt[6] * SAT_SCALE_S16;
	pflt->b11 = flt[7];
	pflt->b21 = flt[8];

	pflt->y10 = 0;
	pflt->y20 = 0;
	pflt->y11 = 0;
	pflt->y21 = 0;

	pflt->x1 = 0;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIIR_P4_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    void *filter,
    mlib_s32 n)
{
	mlib_s32 i;
	__m128d sa00, sa10, sb10, sb20, sa01, sa11, sb11, sb21,
	    sy20, sy21, sy10, sy11, sc, sx1, sx0;

	mlib_IIR_filt_S16_P4 *pflt = (mlib_IIR_filt_S16_P4 *) filter;

	__m128d *pBuffer = __mlib_malloc(((n + 1) >> 1) * sizeof (__m128d));
	mlib_d64 *srcBuffer = (mlib_d64 *)pBuffer;
	mlib_SignalFIR_s16_to_d64((void *)srcBuffer,
	    (void *)src, n);

	if (filter == NULL || src == NULL || dst == NULL)
		return (MLIB_NULLPOINTER);

	if (n <= 0)
		return (MLIB_OUTOFRANGE);

	for (i = 0; (i < n) && ((mlib_addr)(dst + i) & 15); i++) {
		mlib_d64 x0, y00, y01, r0;
		mlib_d64 C, a00, a10, b10, b20, a01, a11, b11, b21;
		a00 = pflt->a00;
		a10 = pflt->a10;
		a01 = pflt->a01;
		a11 = pflt->a11;
		b10 = pflt->b10;
		b20 = pflt->b20;
		b11 = pflt->b11;
		b21 = pflt->b21;
		C = pflt->C;

		x0 = src[i];

		y00 = a00 * x0 + a10 * pflt->x1 +
		    b10 * pflt->y10 + b20 * pflt->y20;

		y01 = a01 * x0 + a11 * pflt->x1 +
		    b11 * pflt->y11 + b21 * pflt->y21;

		r0 = C * x0 + y01 + y00;

		pflt->y20 = pflt->y10;
		pflt->y10 = y00;

		pflt->x1 = x0;

		pflt->y21 = pflt->y11;
		pflt->y11 = y01;

		CLAMP_S16_ARITH(dst[i], r0);
	}

	sa00 = _mm_set1_pd(pflt->a00);
	sa10 = _mm_set1_pd(pflt->a10);
	sa01 = _mm_set1_pd(pflt->a01);
	sa11 = _mm_set1_pd(pflt->a11);
	sb10 = _mm_set1_pd(pflt->b10);
	sb20 = _mm_set1_pd(pflt->b20);
	sb11 = _mm_set1_pd(pflt->b11);
	sb21 = _mm_set1_pd(pflt->b21);
	sc = _mm_set1_pd(pflt->C);

	sy20 = _mm_setr_pd(pflt->y20, pflt->y10);
	sy21 = _mm_setr_pd(pflt->y21, pflt->y11);

	sx1 = _mm_setr_pd(pflt->x1, src[i]);

	__m128d d_zero = _mm_setzero_pd();
	sy10 = _mm_unpackhi_pd(sy20, d_zero);
	sy11 = _mm_unpackhi_pd(sy21, d_zero);

	mlib_d64 dmax = (mlib_d64)(32767);
	mlib_d64 dmin = (mlib_d64)(-32768);
	__m128d x_max = _mm_load1_pd(&dmax);
	__m128d x_min = _mm_load1_pd(&dmin);

	if (((mlib_addr)(srcBuffer + i) & 15) == 0) {

	sx0 = _mm_load_pd((srcBuffer + i));

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < (n - 7); i += 8) {
		__m128d st0, st1, st2, st3, st4, st5, sr0, sr1, sm0, sm1;
		st0 = _mm_add_pd(
		    _mm_add_pd(_mm_mul_pd(sa00, sx0), _mm_mul_pd(sa10, sx1)),
		    _mm_add_pd(_mm_mul_pd(sb10, sy10), _mm_mul_pd(sb20, sy20)));
		st1 = _mm_unpacklo_pd(d_zero, st0);
		st2 = _mm_mul_pd(sb10, st1);
		sy20 = _mm_add_pd(st0, st2);

		st3 = _mm_add_pd(
		    _mm_add_pd(_mm_mul_pd(sa01, sx0), _mm_mul_pd(sa11, sx1)),
		    _mm_add_pd(_mm_mul_pd(sb11, sy11), _mm_mul_pd(sb21, sy21)));
		st4 = _mm_unpacklo_pd(d_zero, st3);
		st5 = _mm_mul_pd(sb11, st4);
		sy21 = _mm_add_pd(st3, st5);

		sy10 = _mm_unpackhi_pd(sy20, d_zero);
		sy11 = _mm_unpackhi_pd(sy21, d_zero);

		sr0 = _mm_add_pd(_mm_add_pd(_mm_mul_pd(sc, sx0), sy21), sy20);
		st0 = _mm_max_pd(x_min, sr0);
		sm0 = _mm_min_pd(x_max, st0);

		sx0 = _mm_load_pd((srcBuffer + i + 2));
		sx1 = _mm_loadu_pd((srcBuffer + i + 1));

		st0 = _mm_add_pd(
		    _mm_add_pd(_mm_mul_pd(sa00, sx0), _mm_mul_pd(sa10, sx1)),
		    _mm_add_pd(_mm_mul_pd(sb10, sy10), _mm_mul_pd(sb20, sy20)));
		st1 = _mm_unpacklo_pd(d_zero, st0);
		st2 = _mm_mul_pd(sb10, st1);
		sy20 = _mm_add_pd(st0, st2);

		st3 = _mm_add_pd(
		    _mm_add_pd(_mm_mul_pd(sa01, sx0), _mm_mul_pd(sa11, sx1)),
		    _mm_add_pd(_mm_mul_pd(sb11, sy11), _mm_mul_pd(sb21, sy21)));
		st4 = _mm_unpacklo_pd(d_zero, st3);
		st5 = _mm_mul_pd(sb11, st4);
		sy21 = _mm_add_pd(st3, st5);

		sy10 = _mm_unpackhi_pd(sy20, d_zero);
		sy11 = _mm_unpackhi_pd(sy21, d_zero);

		sr1 = _mm_add_pd(_mm_add_pd(_mm_mul_pd(sc, sx0), sy21), sy20);
		st0 = _mm_max_pd(x_min, sr1);
		sm1 = _mm_min_pd(x_max, st0);
		__m128i xsd0 = _mm_cvtpd_epi32(sm0);
		__m128i xsd1 = _mm_cvtpd_epi32(sm1);
		xsd0 = _mm_unpacklo_epi64(xsd0, xsd1);

		sx0 = _mm_load_pd((srcBuffer + i + 2 + 2));
		sx1 = _mm_loadu_pd((srcBuffer + i + 2 + 1));

		st0 = _mm_add_pd(
		    _mm_add_pd(_mm_mul_pd(sa00, sx0), _mm_mul_pd(sa10, sx1)),
		    _mm_add_pd(_mm_mul_pd(sb10, sy10), _mm_mul_pd(sb20, sy20)));
		st1 = _mm_unpacklo_pd(d_zero, st0);
		st2 = _mm_mul_pd(sb10, st1);
		sy20 = _mm_add_pd(st0, st2);

		st3 = _mm_add_pd(
		    _mm_add_pd(_mm_mul_pd(sa01, sx0), _mm_mul_pd(sa11, sx1)),
		    _mm_add_pd(_mm_mul_pd(sb11, sy11), _mm_mul_pd(sb21, sy21)));
		st4 = _mm_unpacklo_pd(d_zero, st3);
		st5 = _mm_mul_pd(sb11, st4);
		sy21 = _mm_add_pd(st3, st5);

		sy10 = _mm_unpackhi_pd(sy20, d_zero);
		sy11 = _mm_unpackhi_pd(sy21, d_zero);

		sr0 = _mm_add_pd(_mm_add_pd(_mm_mul_pd(sc, sx0), sy21), sy20);
		st0 = _mm_max_pd(x_min, sr0);
		sm0 = _mm_min_pd(x_max, st0);

		sx0 = _mm_load_pd((srcBuffer + i + 4 + 2));
		sx1 = _mm_loadu_pd((srcBuffer + i + 4 + 1));

		st0 = _mm_add_pd(
		    _mm_add_pd(_mm_mul_pd(sa00, sx0), _mm_mul_pd(sa10, sx1)),
		    _mm_add_pd(_mm_mul_pd(sb10, sy10), _mm_mul_pd(sb20, sy20)));
		st1 = _mm_unpacklo_pd(d_zero, st0);
		st2 = _mm_mul_pd(sb10, st1);
		sy20 = _mm_add_pd(st0, st2);

		st3 = _mm_add_pd(
		    _mm_add_pd(_mm_mul_pd(sa01, sx0), _mm_mul_pd(sa11, sx1)),
		    _mm_add_pd(_mm_mul_pd(sb11, sy11), _mm_mul_pd(sb21, sy21)));
		st4 = _mm_unpacklo_pd(d_zero, st3);
		st5 = _mm_mul_pd(sb11, st4);
		sy21 = _mm_add_pd(st3, st5);

		sy10 = _mm_unpackhi_pd(sy20, d_zero);
		sy11 = _mm_unpackhi_pd(sy21, d_zero);

		sr0 = _mm_add_pd(_mm_add_pd(_mm_mul_pd(sc, sx0), sy21), sy20);
		st0 = _mm_max_pd(x_min, sr0);
		sm1 = _mm_min_pd(x_max, st0);

		__m128i xsd2 = _mm_cvtpd_epi32(sm0);
		__m128i xsd3 = _mm_cvtpd_epi32(sm1);
		xsd2 = _mm_unpacklo_epi64(xsd2, xsd3);
		xsd3 = _mm_packs_epi32(xsd0, xsd2);
		_mm_store_si128((void *)(dst + i), xsd3);

		sx0 = _mm_load_pd((srcBuffer + i + 6 + 2));
		sx1 = _mm_loadu_pd((srcBuffer + i + 6 + 1));
	} /* for */

	} else {

	sx0 = _mm_loadu_pd((srcBuffer + i));

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < (n - 7); i += 8) {
		__m128d st0, st1, st2, st3, st4, st5, sr0, sr1, sm0, sm1;
		st0 = _mm_add_pd(
		    _mm_add_pd(_mm_mul_pd(sa00, sx0), _mm_mul_pd(sa10, sx1)),
		    _mm_add_pd(_mm_mul_pd(sb10, sy10), _mm_mul_pd(sb20, sy20)));
		st1 = _mm_unpacklo_pd(d_zero, st0);
		st2 = _mm_mul_pd(sb10, st1);
		sy20 = _mm_add_pd(st0, st2);

		st3 = _mm_add_pd(
		    _mm_add_pd(_mm_mul_pd(sa01, sx0), _mm_mul_pd(sa11, sx1)),
		    _mm_add_pd(_mm_mul_pd(sb11, sy11), _mm_mul_pd(sb21, sy21)));
		st4 = _mm_unpacklo_pd(d_zero, st3);
		st5 = _mm_mul_pd(sb11, st4);
		sy21 = _mm_add_pd(st3, st5);

		sy10 = _mm_unpackhi_pd(sy20, d_zero);
		sy11 = _mm_unpackhi_pd(sy21, d_zero);

		sr0 = _mm_add_pd(_mm_add_pd(_mm_mul_pd(sc, sx0), sy21), sy20);
		st0 = _mm_max_pd(x_min, sr0);
		sm0 = _mm_min_pd(x_max, st0);

		sx0 = _mm_loadu_pd((srcBuffer + i + 2));
		sx1 = _mm_load_pd((srcBuffer + i + 1));

		st0 = _mm_add_pd(
		    _mm_add_pd(_mm_mul_pd(sa00, sx0), _mm_mul_pd(sa10, sx1)),
		    _mm_add_pd(_mm_mul_pd(sb10, sy10), _mm_mul_pd(sb20, sy20)));
		st1 = _mm_unpacklo_pd(d_zero, st0);
		st2 = _mm_mul_pd(sb10, st1);
		sy20 = _mm_add_pd(st0, st2);

		st3 = _mm_add_pd(
		    _mm_add_pd(_mm_mul_pd(sa01, sx0), _mm_mul_pd(sa11, sx1)),
		    _mm_add_pd(_mm_mul_pd(sb11, sy11), _mm_mul_pd(sb21, sy21)));
		st4 = _mm_unpacklo_pd(d_zero, st3);
		st5 = _mm_mul_pd(sb11, st4);
		sy21 = _mm_add_pd(st3, st5);

		sy10 = _mm_unpackhi_pd(sy20, d_zero);
		sy11 = _mm_unpackhi_pd(sy21, d_zero);

		sr1 = _mm_add_pd(_mm_add_pd(_mm_mul_pd(sc, sx0), sy21), sy20);
		st0 = _mm_max_pd(x_min, sr1);
		sm1 = _mm_min_pd(x_max, st0);
		__m128i xsd0 = _mm_cvtpd_epi32(sm0);
		__m128i xsd1 = _mm_cvtpd_epi32(sm1);
		xsd0 = _mm_unpacklo_epi64(xsd0, xsd1);

		sx0 = _mm_loadu_pd((srcBuffer + i + 2 + 2));
		sx1 = _mm_load_pd((srcBuffer + i + 2 + 1));

		st0 = _mm_add_pd(
		    _mm_add_pd(_mm_mul_pd(sa00, sx0), _mm_mul_pd(sa10, sx1)),
		    _mm_add_pd(_mm_mul_pd(sb10, sy10), _mm_mul_pd(sb20, sy20)));
		st1 = _mm_unpacklo_pd(d_zero, st0);
		st2 = _mm_mul_pd(sb10, st1);
		sy20 = _mm_add_pd(st0, st2);

		st3 = _mm_add_pd(
		    _mm_add_pd(_mm_mul_pd(sa01, sx0), _mm_mul_pd(sa11, sx1)),
		    _mm_add_pd(_mm_mul_pd(sb11, sy11), _mm_mul_pd(sb21, sy21)));
		st4 = _mm_unpacklo_pd(d_zero, st3);
		st5 = _mm_mul_pd(sb11, st4);
		sy21 = _mm_add_pd(st3, st5);

		sy10 = _mm_unpackhi_pd(sy20, d_zero);
		sy11 = _mm_unpackhi_pd(sy21, d_zero);

		sr0 = _mm_add_pd(_mm_add_pd(_mm_mul_pd(sc, sx0), sy21), sy20);
		st0 = _mm_max_pd(x_min, sr0);
		sm0 = _mm_min_pd(x_max, st0);

		sx0 = _mm_loadu_pd((srcBuffer + i + 4 + 2));
		sx1 = _mm_load_pd((srcBuffer + i + 4 + 1));

		st0 = _mm_add_pd(
		    _mm_add_pd(_mm_mul_pd(sa00, sx0), _mm_mul_pd(sa10, sx1)),
		    _mm_add_pd(_mm_mul_pd(sb10, sy10), _mm_mul_pd(sb20, sy20)));
		st1 = _mm_unpacklo_pd(d_zero, st0);
		st2 = _mm_mul_pd(sb10, st1);
		sy20 = _mm_add_pd(st0, st2);

		st3 = _mm_add_pd(
		    _mm_add_pd(_mm_mul_pd(sa01, sx0), _mm_mul_pd(sa11, sx1)),
		    _mm_add_pd(_mm_mul_pd(sb11, sy11), _mm_mul_pd(sb21, sy21)));
		st4 = _mm_unpacklo_pd(d_zero, st3);
		st5 = _mm_mul_pd(sb11, st4);
		sy21 = _mm_add_pd(st3, st5);

		sy10 = _mm_unpackhi_pd(sy20, d_zero);
		sy11 = _mm_unpackhi_pd(sy21, d_zero);

		sr0 = _mm_add_pd(_mm_add_pd(_mm_mul_pd(sc, sx0), sy21), sy20);
		st0 = _mm_max_pd(x_min, sr0);
		sm1 = _mm_min_pd(x_max, st0);

		__m128i xsd2 = _mm_cvtpd_epi32(sm0);
		__m128i xsd3 = _mm_cvtpd_epi32(sm1);
		xsd2 = _mm_unpacklo_epi64(xsd2, xsd3);
		xsd3 = _mm_packs_epi32(xsd0, xsd2);
		_mm_store_si128((void *)(dst + i), xsd3);

		sx0 = _mm_loadu_pd((srcBuffer + i + 6 + 2));
		sx1 = _mm_load_pd((srcBuffer + i + 6 + 1));
	} /* for */
	} /* if (((mlib_addr)(srcBuffer + i) & 15) == 0) */

	_mm_storel_pd(&(pflt->x1), sx1);
	_mm_storeh_pd(&(pflt->y10), sy20);
	_mm_storeh_pd(&(pflt->y11), sy21);
	_mm_storel_pd(&(pflt->y20), sy20);
	_mm_storel_pd(&(pflt->y21), sy21);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n; i++) {
		mlib_d64 x0, y00, y01, r0;
		mlib_d64 C, a00, a10, b10, b20, a01, a11, b11, b21;
		a00 = pflt->a00;
		a10 = pflt->a10;
		a01 = pflt->a01;
		a11 = pflt->a11;
		b10 = pflt->b10;
		b20 = pflt->b20;
		b11 = pflt->b11;
		b21 = pflt->b21;
		C = pflt->C;

		x0 = src[i];

		y00 = a00 * x0 + a10 * pflt->x1 +
		    b10 * pflt->y10 + b20 * pflt->y20;

		y01 = a01 * x0 + a11 * pflt->x1 +
		    b11 * pflt->y11 + b21 * pflt->y21;

		r0 = C * x0 + y01 + y00;

		pflt->y20 = pflt->y10;
		pflt->y10 = y00;

		pflt->x1 = x0;

		pflt->y21 = pflt->y11;
		pflt->y11 = y01;

		CLAMP_S16_ARITH(dst[i], r0);
	}

	__mlib_free(pBuffer);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
__mlib_SignalIIRFree_P4_S16_S16(
    void *filter)
{
	__mlib_free(filter);
}

/* *********************************************************** */

/* Stereo signal */

/* *********************************************************** */

mlib_status
__mlib_SignalIIRInit_P4_S16S_S16S(
    void **filter,
    const mlib_f32 *flt)
{
	mlib_IIR_filt_S16S_P4 *pflt;

	if (flt == NULL)
		return (MLIB_NULLPOINTER);

	*filter = (void *)__mlib_malloc(sizeof (mlib_IIR_filt_S16S_P4));

	if (*filter == NULL)
		return (MLIB_NULLPOINTER);

	pflt = (mlib_IIR_filt_S16S_P4 *) (*filter);

	pflt->C = flt[0] * SAT_SCALE_S16;

	pflt->a00 = flt[1] * SAT_SCALE_S16;
	pflt->a10 = flt[2] * SAT_SCALE_S16;
	pflt->b10 = flt[3];
	pflt->b20 = flt[4];

	pflt->a01 = flt[5] * SAT_SCALE_S16;
	pflt->a11 = flt[6] * SAT_SCALE_S16;
	pflt->b11 = flt[7];
	pflt->b21 = flt[8];

	pflt->y10_0 = 0;
	pflt->y20_0 = 0;
	pflt->y11_0 = 0;
	pflt->y21_0 = 0;

	pflt->y10_1 = 0;
	pflt->y20_1 = 0;
	pflt->y11_1 = 0;
	pflt->y21_1 = 0;

	pflt->x1_0 = 0;
	pflt->x1_1 = 0;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalIIR_P4_S16S_S16S_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    void *filter,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 C, a00, a10, b10, b20, a01, a11, b11, b21;
	mlib_d64 y00_0, y10_0, y20_0, y01_0, y11_0, y21_0, x1_0;
	mlib_d64 r0_0, x0_0, y00_1, y10_1;
	mlib_d64 y20_1, y01_1, y11_1, y21_1, x1_1, r0_1, x0_1;

	__m128d sa0, sa1, sb1, sb2, sc;
	__m128d sx00, sx10, sy10, sy20, sy00;
	__m128d sx01, sx11, sy11, sy21, sy01;

	mlib_IIR_filt_S16S_P4 *pflt = (mlib_IIR_filt_S16S_P4 *) filter;

	if (filter == NULL || src == NULL || dst == NULL)
		return (MLIB_NULLPOINTER);

	if (n <= 0)
		return (MLIB_OUTOFRANGE);

	a00 = pflt->a00;
	a10 = pflt->a10;

	a01 = pflt->a01;
	a11 = pflt->a11;

	b10 = pflt->b10;
	b20 = pflt->b20;

	b11 = pflt->b11;
	b21 = pflt->b21;

	y10_0 = pflt->y10_0;
	y20_0 = pflt->y20_0;
	y11_0 = pflt->y11_0;
	y21_0 = pflt->y21_0;

	y10_1 = pflt->y10_1;
	y20_1 = pflt->y20_1;
	y11_1 = pflt->y11_1;
	y21_1 = pflt->y21_1;

	sa0 = _mm_setr_pd(a00, a01);
	sa1 = _mm_setr_pd(a10, a11);
	sb1 = _mm_setr_pd(b10, b11);
	sb2 = _mm_setr_pd(b20, b21);

	sy10 = _mm_setr_pd(y10_0, y11_0);
	sy20 = _mm_setr_pd(y20_0, y21_0);
	sy11 = _mm_setr_pd(y10_1, y11_1);
	sy21 = _mm_setr_pd(y20_1, y21_1);

	x1_1 = pflt->x1_1;
	x1_0 = pflt->x1_0;

	sx10 = _mm_set1_pd(x1_0);
	sx11 = _mm_set1_pd(x1_1);

	C = pflt->C;
	sc = _mm_set1_pd(C);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (n << 1); i += 2) {
		__m128d st0, st1, st2, st3, st4, st5, sy00, sy01;

		st1 = _mm_mul_pd(sa1, sx10);
		x0_0 = src[i];
		sx10 = _mm_set1_pd(x0_0);
		st0 = _mm_mul_pd(sa0, sx10);
		st2 = _mm_mul_pd(sb1, sy10);
		st3 = _mm_mul_pd(sb2, sy20);
		st4 = _mm_add_pd(st0, st1);
		st5 = _mm_add_pd(st2, st3);
		sy00 = _mm_add_pd(st4, st5);
		sy20 = sy10;
		sy10 = sy00;

		st1 = _mm_mul_pd(sa1, sx11);
		x0_1 = src[i + 1];
		sx11 = _mm_set1_pd(x0_1);
		st0 = _mm_mul_pd(sa0, sx11);
		st2 = _mm_mul_pd(sb1, sy11);
		st3 = _mm_mul_pd(sb2, sy21);
		st4 = _mm_add_pd(st0, st1);
		st5 = _mm_add_pd(st2, st3);
		sy01 = _mm_add_pd(st4, st5);
		sy21 = sy11;
		sy11 = sy01;

		st0 = _mm_unpacklo_pd(sy00, sy01);
		st1 = _mm_unpackhi_pd(sy00, sy01);
		st2 = _mm_add_pd(st0, st1);
		st3 = _mm_unpacklo_pd(sx10, sx11);
		st4 = _mm_mul_pd(sc, st3);
		st5 = _mm_add_pd(st4, st2);
		mlib_d64 sl, sh;
		_mm_storel_pd(&sl, st5);
		_mm_storeh_pd(&sh, st5);
		CLAMP_S16_ARITH(dst[i], sl);
		CLAMP_S16_ARITH(dst[i + 1], sh);
	}

	_mm_storel_pd(&(pflt->y10_0), sy10);
	_mm_storel_pd(&(pflt->y20_0), sy20);
	_mm_storeh_pd(&(pflt->y11_0), sy10);
	_mm_storeh_pd(&(pflt->y21_0), sy20);

	_mm_storel_pd(&(pflt->y10_1), sy11);
	_mm_storel_pd(&(pflt->y20_1), sy21);
	_mm_storeh_pd(&(pflt->y11_1), sy11);
	_mm_storeh_pd(&(pflt->y21_1), sy21);

	_mm_storel_pd(&(pflt->x1_0), sx10);
	_mm_storel_pd(&(pflt->x1_1), sx11);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
__mlib_SignalIIRFree_P4_S16S_S16S(
    void *filter)
{
	__mlib_free(filter);
}

/* *********************************************************** */
