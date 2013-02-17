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

#pragma ident	"@(#)mlib_s_SignalWindow.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalGenBartlett_S16 - generate normalized
 *          coefficients of the corresponding window
 *
 *      mlib_SignalGenHanning_S16 - generate normalized
 *          coefficients of the corresponding window
 *
 *      mlib_SignalGenHamming_S16 - generate normalized
 *          coefficients of the corresponding window
 *
 *      mlib_SignalGenBlackman_S16 - generate normalized
 *          coefficients of the corresponding window
 *
 *      mlib_SignalGenKaiser_S16 - generate normalized
 *          coefficients of the corresponding window
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalGenBartlett_S16(mlib_s16 *window,
 *                                             mlib_s32 n);
 *      mlib_status mlib_SignalGenBlackman_S16(mlib_s16 *window,
 *                                             mlib_f32 alpha,
 *                                             mlib_s32 n);
 *      mlib_status mlib_SignalGenHamming_S16(mlib_s16 *window,
 *                                            mlib_s32 n);
 *      mlib_status mlib_SignalGenHanning_S16(mlib_s16 *window,
 *                                            mlib_s32 n);
 *      mlib_status mlib_SignalGenKaiser_S16(mlib_s16 *window,
 *                                           mlib_f32 beta,
 *                                           mlib_s32 n);
 * ARGUMENTS
 *      window   Generated window coefficient array. The window
 *               coefficients are in Q15 format
 *      alpha    Blackman window parameter
 *      beta     Kaiser window parameter
 *      n        Length of window array
 *
 * DESCRIPTION
 *     Bartlett:          window[i] = ((i <= (N - 1)/2) ? (2*i/(N -1)):
 *                          (2 - (2 * i)/(N-1)))
 *
 *     Hanning:           window[i] = 0.5*(1 - cos(2*pi*i/(N -1)))
 *
 *     Hamming:           window[i] = 0.54 - 0.46*cos(2*pi*i/(N - 1))
 *
 *     Blackman:          window[i] = (alpha + 1)/2 - 0.5*cos(2*pi*i/(N - 1)) -
 *                          (alpha/2)*cos(4*pi*i/(N - 1))
 *
 *     Kaiser:            window[i] = I0(beta * sqrt(sqr((N - 1)/2) -
 *                          sqr(i - (N - 1)/2)))/I0(beta*((N - 1)/2))
 */

#include <mlib_signal.h>
#include <mlib_SysMath.h>
#include <mlib_SignalWindow.h>
#include <mlib_SignalWindowUtil.h>
#include <mlib_s_SignalWinTab.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalGenBartlett_S16 = __mlib_SignalGenBartlett_S16
#pragma weak mlib_SignalGenBlackman_S16 = __mlib_SignalGenBlackman_S16
#pragma weak mlib_SignalGenHamming_S16 = __mlib_SignalGenHamming_S16
#pragma weak mlib_SignalGenHanning_S16 = __mlib_SignalGenHanning_S16
#pragma weak mlib_SignalGenKaiser_S16 = __mlib_SignalGenKaiser_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalGenBartlett_S16) mlib_SignalGenBartlett_S16
    __attribute__((weak, alias("__mlib_SignalGenBartlett_S16")));
__typeof__(__mlib_SignalGenBlackman_S16) mlib_SignalGenBlackman_S16
    __attribute__((weak, alias("__mlib_SignalGenBlackman_S16")));
__typeof__(__mlib_SignalGenHamming_S16) mlib_SignalGenHamming_S16
    __attribute__((weak, alias("__mlib_SignalGenHamming_S16")));
__typeof__(__mlib_SignalGenHanning_S16) mlib_SignalGenHanning_S16
    __attribute__((weak, alias("__mlib_SignalGenHanning_S16")));
__typeof__(__mlib_SignalGenKaiser_S16) mlib_SignalGenKaiser_S16
    __attribute__((weak, alias("__mlib_SignalGenKaiser_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_HANNING_VAL    16383.5

	/* do not copy by mlib_d64 data type for x86 */
#ifdef	i386

typedef struct
{
	mlib_s32 int0, int1;
} two_int;
#define	TYPE_64BIT  two_int

#else /* i386 */
#define	TYPE_64BIT  mlib_d64
#endif /* i386 */

typedef union
{
	TYPE_64BIT d64;
	struct
	{
		mlib_s32 i0, i1;
	} s32x2;
} mlib_union64;

/* *********************************************************** */

#define	MLIB_SIGNAL_MASK    0x7FFFFFFF

/* *********************************************************** */

mlib_status
__mlib_SignalGenBartlett_S16(
    mlib_s16 *window,
    mlib_s32 n)
{
	mlib_s32 result, n2, n1, i;

	n1 = n - 1;
	n2 = (n1 >> 1);
/* check for obvious errors */

	if ((window == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	if (n <= 4096 && n >= 16) {
		__m128i *dpx = (__m128i *) window;
		__m128i *dpz;
		__m128i dx0, dx1, dx2, dx3;
		__m128i dz0, dz1, dz2, dz3;
		__m128i dresult1, dresult2;
		__m128i dstep;

		mlib_s16 *reverse;
		reverse = window + n - 8;
		dpz = (__m128i *) reverse;

		mlib_s32 nlen = n >> 4;

		mlib_s32 step, base1, base2, base3, base0;
		mlib_s32 result;

		step = (MLIB_SIGNAL_MASK) / n1;

		base0 = step << 3;
		dstep = _mm_set1_epi32(base0);

		base0 = 0;
		base1 = step;
		base2 = step << 1;
		base3 = base2 + step;
		dx0 = _mm_set_epi32(base3, base2, base1, base0);
		dx1 = _mm_set_epi32(base0, base1, base2, base3);

		base0 = step << 2;
		base1 = base0 + step;
		base2 = base1 + step;
		base3 = base2 + step;
		dx2 = _mm_set_epi32(base3, base2, base1, base0);
		dx3 = _mm_set_epi32(base0, base1, base2, base3);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < nlen; i++) {
			dz0 = _mm_srai_epi32(dx0, 15);
			dz1 = _mm_srai_epi32(dx1, 15);
			dz2 = _mm_srai_epi32(dx2, 15);
			dz3 = _mm_srai_epi32(dx3, 15);

			dresult1 = _mm_packs_epi32(dz0, dz2);
			dresult2 = _mm_packs_epi32(dz3, dz1);

			dx0 = _mm_add_epi32(dx0, dstep);
			dx1 = _mm_add_epi32(dx1, dstep);
			dx2 = _mm_add_epi32(dx2, dstep);
			dx3 = _mm_add_epi32(dx3, dstep);

			if (0 == ((mlib_addr) window & 0xf)) {
				_mm_store_si128(dpx, dresult1);
			} else {
				_mm_storeu_si128(dpx, dresult1);
			}
			if (0 == ((mlib_addr) reverse & 0xf)) {
				_mm_store_si128(dpz, dresult2);
			} else {
				_mm_storeu_si128(dpz, dresult2);
			}

			dpx++;
			dpz--;
		}

		if (n != (nlen << 4)) {
			mlib_s32 index, end;
			index = nlen << 3;
			base0 = step * (nlen << 3);
			end = ((n - 1) - (nlen << 4)) >> 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= end; i++) {
				result = base0 >> 15;
				base0 += step;
				window[index] = result;
				window[n1 - index] = result;
				index++;
			}
		}
	} else if (n < 16) {
		mlib_s32 step, base;

		step = (MLIB_SIGNAL_MASK) / n1;
		base = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i <= n2; i++) {
			result = base >> 15;
			base += step;
			window[i] = result;
			window[n1 - i] = result;
		}
	} else {
		mlib_d64 step, base;

		base = 0;
		step = 2. / n1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i <= n2; i++) {
			result = base * 32767;
			base += step;
			window[i] = result;
			window[n1 - i] = result;
		}
	}
	return (MLIB_SUCCESS);
}

mlib_status
mlib_SignalGenBartlett_S16_i(
    mlib_s16 *window,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1)
{
	mlib_s32 result, n1, i;

	n1 = n - 1;

/* check for obvious errors */

	if ((window == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	if (n <= 4096) {
		mlib_s32 step, base;

		step = (MLIB_SIGNAL_MASK) / n1;
		base = step * i0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = i0; i < i1; i++) {
			result = base >> 15;
			base += step;
			window[i] = result;
			window[n1 - i] = result;
		}
	} else {
		mlib_d64 step, base;
		step = 2. / n1;
		base = step * i0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = i0; i < i1; i++) {
			result = base * 32767;
			base += step;
			window[i] = result;
			window[n1 - i] = result;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_SignalGenHanning_S16_i(
    mlib_s16 *window,
    mlib_s16 *window2,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1)
{
	if ((window == NULL) || (n <= 1))
		return (MLIB_FAILURE);
	if ((n <= 1024) && ((n & (n - 1)) == 0)) {

		const mlib_s16 *psrc =
				mlib_s_Hanning_S16 + (n >> 1) - 1;
		mlib_s32  j;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = i0; j < i1; j++) {
			window[2 * j] = window2[-2 * j] = psrc[j * 2];
			window[2 * j + 1] = window2[-2 * j - 1] =
			psrc[j * 2 + 1];
		}

		if ((n == 2) && ~i1)
			window[0] = window2[0] = psrc[0];

		return (MLIB_SUCCESS);
	}

	mlib_union64 tmp0;
	mlib_d64 cbase0, cbase1, cbase2, cbase3;
	mlib_d64 temp0, temp1, cstep;
	mlib_s32 result0, result1;
	mlib_s32 i;
	TYPE_64BIT tmp2[1];

	cstep = mlib_cos(2. * M_PI / (n - 1));
	if (i0) {
		cbase0 = MLIB_HANNING_VAL *
		mlib_cos(2. * M_PI / (n - 1) * (2 * i0));
		cbase1 = MLIB_HANNING_VAL *
		mlib_cos(2. * M_PI / (n - 1) * (2 * i0 + 1));
	} else {
		cbase0 = MLIB_HANNING_VAL;
		cbase1 = MLIB_HANNING_VAL * cstep;
	}
	cbase2 = 2. * cstep * cbase1 - cbase0;
	cbase3 = 2. * cstep * cbase2 - cbase1;
	cstep = 4. * cstep * cstep - 2.;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = i0; i < i1; i++) {
		tmp0.s32x2.i0 = MLIB_HANNING_VAL - cbase0;
		tmp0.s32x2.i1 = MLIB_HANNING_VAL - cbase1;
		tmp2[0] = tmp0.d64;
		result0 = ((mlib_s32 *)tmp2)[0];
		result1 = ((mlib_s32 *)tmp2)[1];
		window[2 * i] = result0;
		window[2 * i + 1] = result1;
		window2[-(2 * i)] = result0;
		window2[-(2 * i + 1)] = result1;
		temp0 = cstep * cbase2 - cbase0;
		temp1 = cstep * cbase3 - cbase1;
		cbase0 = cbase2;
		cbase1 = cbase3;
		cbase2 = temp0;
		cbase3 = temp1;
	}

	if ((n & 3) && i1 == n / 4) {
		window += 2 * i;
		switch (n & 3) {
		case 3:
			window[2] = MLIB_HANNING_VAL - cbase2;
		case 2:
			window[1] = MLIB_HANNING_VAL - cbase1;
		case 1:
			window[0] = MLIB_HANNING_VAL - cbase0;
		}
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_SignalGenHanning_S16(
    mlib_s16 *window,
    mlib_s32 n)
{
	return mlib_SignalGenHanning_S16_i(window, window + n - 1, n,
		0, n / 4);
}

/* *********************************************************** */

#define	MLIB_HAMMING_SUB    17694.18
#define	MLIB_HAMMING_MUL    15072.82

/* *********************************************************** */

mlib_status
mlib_SignalGenHamming_S16_i(
    mlib_s16 *window,
    mlib_s16 *window2,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1)
{
	if ((window == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	if ((n <= 1024) && ((n & (n - 1)) == 0)) {

		const mlib_s16 *psrc =
				mlib_s_Hamming_S16 + (n >> 1) - 1;
		mlib_s32  j;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = i0; j < i1; j++) {
			window[2 * j] = window2[-2 * j] = psrc[j * 2];
			window[2 * j + 1] = window2[-2 * j - 1] =
					psrc[j * 2 + 1];
		}

		if ((n == 2) && ~i1)
			window[0] = window2[0] = psrc[0];

		return (MLIB_SUCCESS);
	}
	mlib_union64 tmp0;
	mlib_d64 cbase0, cbase1, cbase2, cbase3;
	mlib_d64 temp0, temp1, cstep;
	mlib_s32 result0, result1;
	mlib_s32 i;
	TYPE_64BIT tmp2[1];

	cstep = mlib_cos(2. * M_PI / (n - 1));
	if (i0) {
		cbase0 = MLIB_HAMMING_MUL *
				mlib_cos(2. * M_PI / (n - 1) * (2 * i0));
		cbase1 = MLIB_HAMMING_MUL *
				mlib_cos(2. * M_PI / (n - 1) * (2 * i0 + 1));
	} else {
		cbase0 = MLIB_HAMMING_MUL;
		cbase1 = MLIB_HAMMING_MUL * cstep;
	}
	cbase2 = 2. * cstep * cbase1 - cbase0;
	cbase3 = 2. * cstep * cbase2 - cbase1;
	cstep = 4. * cstep * cstep - 2.;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = i0; i < i1; i++) {
		tmp0.s32x2.i0 = MLIB_HAMMING_SUB - cbase0;
		tmp0.s32x2.i1 = MLIB_HAMMING_SUB - cbase1;
		tmp2[0] = tmp0.d64;
		result0 = ((mlib_s32 *)tmp2)[0];
		result1 = ((mlib_s32 *)tmp2)[1];
		window[2 * i] = result0;
		window[2 * i + 1] = result1;
		window2[-(2 * i)] = result0;
		window2[-(2 * i + 1)] = result1;
		temp0 = cstep * cbase2 - cbase0;
		temp1 = cstep * cbase3 - cbase1;
		cbase0 = cbase2;
		cbase1 = cbase3;
		cbase2 = temp0;
		cbase3 = temp1;
	}

	if ((n & 3) && i1 == n / 4) {
		window += 2 * i;
		switch (n & 3) {
		case 3:
			window[2] = MLIB_HAMMING_SUB - cbase2;
		case 2:
			window[1] = MLIB_HAMMING_SUB - cbase1;
		case 1:
			window[0] = MLIB_HAMMING_SUB - cbase0;
		}
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_SignalGenHamming_S16(
    mlib_s16 *window,
    mlib_s32 n)
{
	return (mlib_SignalGenHamming_S16_i(window, window + n - 1, n,
		0, n / 4));
}

/* *********************************************************** */

#define	MLIB_BLACKMAN(X)    (coeff1 - (X) - coeff2 * (X) * (X))

#define	ALPHA_G  0xBE23D70A  /* -0.16 */

/* *********************************************************** */

mlib_status
mlib_SignalGenBlackman_S16_i(
    mlib_s16 *window,
    mlib_s16 *window2,
    mlib_f32 alpha,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1)
{
	if ((window == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	if ((alpha <= -1.0) || (alpha >= 0.0))
		return (MLIB_OUTOFRANGE);

	if ((n <= 1024) && ((n & (n - 1)) == 0) &&
					*(mlib_s32 *)(& alpha) == ALPHA_G) {

		const mlib_s16 *psrc =
				mlib_s_Blackman_S16 + (n >> 1) - 1;
		mlib_s32  j;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = i0; j < i1; j++) {
			window[2 * j] = window2[-2 * j] = psrc[j * 2];
			window[2 * j + 1] = window2[-2 * j - 1] =
						psrc[j * 2 + 1];
		}

		if ((n == 2) && ~i1)
			window[0] = window2[0] = psrc[0];

		return (MLIB_SUCCESS);

	}
	mlib_union64 tmp0;
	TYPE_64BIT tmp2[1];
	mlib_d64 cbase0, cbase1, cbase2, cbase3;
	mlib_d64 temp0, temp1, cstep;
	mlib_d64 coeff1, coeff2;
	mlib_s32 result0, result1;
	mlib_s32 i;

	coeff1 = (.5 + alpha) * 32767;
	coeff2 = 2. * alpha / 16383.5;
	cstep = mlib_cos(2. * M_PI / (n - 1));
	if (i0) {
		cbase0 = 16383.5 * mlib_cos(2. * M_PI / (n - 1) * (2 * i0));
		cbase1 = 16383.5 *
				mlib_cos(2. * M_PI / (n - 1) * (2 * i0 + 1));
	} else {
		cbase0 = 16383.5;
		cbase1 = 16383.5 * cstep;
	}
	cbase2 = 2. * cstep * cbase1 - cbase0;
	cbase3 = 2. * cstep * cbase2 - cbase1;
	cstep = 4. * cstep * cstep - 2.;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = i0; i < i1; i++) {
		tmp0.s32x2.i0 = MLIB_BLACKMAN(cbase0);
		tmp0.s32x2.i1 = MLIB_BLACKMAN(cbase1);
		tmp2[0] = tmp0.d64;
		result0 = ((mlib_s32 *)tmp2)[0];
		result1 = ((mlib_s32 *)tmp2)[1];
		window[2 * i] = result0;
		window[2 * i + 1] = result1;
		window2[-(2 * i)] = result0;
		window2[-(2 * i + 1)] = result1;
		temp0 = cstep * cbase2 - cbase0;
		temp1 = cstep * cbase3 - cbase1;
		cbase0 = cbase2;
		cbase1 = cbase3;
		cbase2 = temp0;
		cbase3 = temp1;
	}

	if ((n & 3) && i1 == n / 4) {
		window += 2 * i;

		switch (n & 3) {
		case 3:
			window[2] = MLIB_BLACKMAN(cbase2);
		case 2:
			window[1] = MLIB_BLACKMAN(cbase1);
		case 1:
			window[0] = MLIB_BLACKMAN(cbase0);
		}
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_SignalGenBlackman_S16(
    mlib_s16 *window,
    mlib_f32 alpha,
    mlib_s32 n)
{
	return mlib_SignalGenBlackman_S16_i(window, window + n - 1, alpha, n,
		0, n / 4);
}

/* *********************************************************** */

#define	BETA_G  0x3F000000  /* 0.5 */

mlib_status
mlib_SignalGenKaiser_S16_i(
    mlib_s16 *window,
    mlib_f32 beta,
    mlib_s32 n,
    mlib_s32 istart,
    mlib_s32 iend)
{
	if ((window == NULL) || (n <= 0) ||
				(*((mlib_s32 *)(&beta)) < 0))
		return (MLIB_FAILURE);

	if (n == 1) {
		window[0] = MLIB_S16_MAX;
		return (MLIB_SUCCESS);
	}

	if ((n <= 1024) && ((n & (n - 1)) == 0) &&
				*(mlib_s32 *)(& beta) == BETA_G) {

		const mlib_s16 *psrc =
				mlib_s_Kaiser_S16 + (n >> 1) - 1;
		mlib_s16 * pdst = window, * pdst_ = window + n - 1;
		mlib_s32  j;

		if (istart == 1)
			pdst[0] = pdst_[0] = psrc[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = istart; j <= iend; j++)
			pdst[j] = pdst_[-j] = psrc[j];

		return (MLIB_SUCCESS);

	}

	mlib_d64 base, step, coef1, coef2, sqbeta, sqbeta2;
	mlib_d64 temp, temp2, t1, t2, t3;
	type_union_mlib_d64 dbl;
	mlib_s32 n1, n2, i, i0, result;
	mlib_s32 shift, shift1;

	n1 = n - 1;
	n2 = (n1 >> 1);
	temp = n1 * 0.5;
	coef1 = 32767.5 / mlib_i0(beta * temp, &shift);
	temp2 = 15 / beta;
	i0 = (temp < temp2) ? n2 : temp - mlib_sqrt(temp
			* temp - temp2 * temp2);
	sqbeta = beta * beta;
	sqbeta2 = sqbeta + sqbeta;
	step = (n1 - 1) * sqbeta;
	step -= (istart - 1) * sqbeta2;
	base = ((n1 - 1) * sqbeta + step) * istart / 2;

/* coef2 = scalbn(coef1, -shift); */

	if (shift > 1023)
		coef2 = 0;
	else {
		dbl.value = coef1;
		dbl.two_int.int0 -= (shift << 20);
		coef2 = dbl.value;
	}

	window[0] = window[n1] = (mlib_s32)coef2;

	for (i = istart; i <= ((iend < i0) ? iend : i0); i++) {
		mlib_d64 yv4, yv8, yv16;

		yv4 = base * base;
		yv8 = yv4 * yv4;
		yv16 = yv8 * yv8;
		temp = KA0 + KA3 * base * (KA1_KA3 + KA2_KA3 * base + yv4) +
			KA7 * yv8 * (KA4_KA7 + KA5_KA7 * base + yv4 * (KA6_KA7 +
			base)) + KA11 * yv16 * (KA8_KA11 + KA9_KA11 * base +
			yv4 * (KA10_KA11 + base)) +
			KA15 * yv8 * yv16 * (KA12_KA15 +
			KA13_KA15 * base + yv4 * (KA14_KA15 + base));
		window[i] = window[n1 - i] = (mlib_s32)(coef2 * temp);
		step -= sqbeta2;
		base += step;
	}

	temp2 = mlib_sqrt(base);
	for (; i <= iend; i++) {
		t3 = (KP0 * temp2 + KP1) / (base + KQ2 + KQ1 * temp2);
		t1 = mlib_sqrt(temp2);

		/* t2 = mlib_exp(temp2, &shift1); */
		t2 = K128ONLN2 * temp2;
		shift1 = (mlib_s32)t2;
		temp = mlib_constexp[shift1 & 0x7f];
		t2 -= (mlib_d64)shift1;
		t2 = KE2 * temp * t2 * (t2 + KE1_2) + temp;
		/* end of mlib_exp */
		shift1 >>= 7;
		/* for next loop iteration */
		step -= sqbeta2;
		base += step;
		temp2 = mlib_sqrt(base);

		temp = t1 * coef1 * (t2 * t3);
		shift1 = shift - shift1;
		result = (shift1 > 30) ? 0 : ((mlib_s32)temp) >> shift1;
		window[i] = window[n1 - i] = result;
	}

	return (MLIB_SUCCESS);
}
/* *********************************************************** */

mlib_status
__mlib_SignalGenKaiser_S16(
    mlib_s16 *window,
    mlib_f32 beta,
    mlib_s32 n)
{
	return mlib_SignalGenKaiser_S16_i(window, beta, n,
		1, (n - 1) >> 1);
}

/* *********************************************************** */
