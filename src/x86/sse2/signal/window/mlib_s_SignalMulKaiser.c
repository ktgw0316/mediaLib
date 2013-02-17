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

#pragma ident	"@(#)mlib_s_SignalMulKaiser.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulKaiser_S16 - multiply source data on
 *          coefficients of the corresponding window
 *
 *      mlib_SignalMulKaiser_S16S - multiply stereo source
 *          data on coefficients of the corresponding window
 *
 *      mlib_SignalMulKaiser_S16_S16 - multiply source data on
 *          coefficients of the corresponding window with results
 *          in destination array
 *
 *      mlib_SignalMulKaiser_S16S_S16S - multiply stereo source
 *          data on coefficients of the corresponding window with
 *          result in destination array
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalMulKaiser_S16(mlib_s16 *data,
 *                                           mlib_f32 beta,
 *                                           mlib_s32 n);
 *      mlib_status mlib_SignalMulKaiser_S16S(mlib_s16 *data,
 *                                            mlib_f32 beta,
 *                                            mlib_s32 n);
 *      mlib_status mlib_SignalMulKaiser_S16S_S16S(mlib_s16       *dst,
 *                                                 const mlib_s16 *src,
 *                                                 mlib_f32       beta,
 *                                                 mlib_s32       n);
 *      mlib_status mlib_SignalMulKaiser_S16_S16(mlib_s16       *dst,
 *                                               const mlib_s16 *src,
 *                                               mlib_f32       beta,
 *                                               mlib_s32       n);
 *
 * ARGUMENTS
 *      data     Input and output signal array
 *      dst      Output signal array
 *      src      Input signal array
 *      beta     Kaiser window parameter
 *      n        Length of window array
 *
 * DESCRIPTION
 *      Mono        :   dst[i] = src[i] * window[i]    i = 0..(N-1)
 *      Stereo      :   dst[i] = src[i] * window[i/2]  i = 0..(2*N-1)
 *      Kaiser      :   window[i] = I0(beta * sqrt(sqr((N - 1)/2) -
 *                        sqr(i - (N - 1)/2)))/I0(beta*((N - 1)/2))
 */

#include <mlib_signal.h>
#include <mlib_SysMath.h>
#include <mlib_SignalWindow.h>
#include <mlib_SignalWindowUtil.h>
#include <mlib_s_SignalWinTab.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulKaiser_S16 = __mlib_SignalMulKaiser_S16
#pragma weak mlib_SignalMulKaiser_S16S = __mlib_SignalMulKaiser_S16S
#pragma weak mlib_SignalMulKaiser_S16S_S16S = \
	__mlib_SignalMulKaiser_S16S_S16S
#pragma weak mlib_SignalMulKaiser_S16_S16 = __mlib_SignalMulKaiser_S16_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulKaiser_S16) mlib_SignalMulKaiser_S16
    __attribute__((weak, alias("__mlib_SignalMulKaiser_S16")));
__typeof__(__mlib_SignalMulKaiser_S16S) mlib_SignalMulKaiser_S16S
    __attribute__((weak, alias("__mlib_SignalMulKaiser_S16S")));
__typeof__(__mlib_SignalMulKaiser_S16S_S16S) mlib_SignalMulKaiser_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalMulKaiser_S16S_S16S")));
__typeof__(__mlib_SignalMulKaiser_S16_S16) mlib_SignalMulKaiser_S16_S16
    __attribute__((weak, alias("__mlib_SignalMulKaiser_S16_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	BETA_G	0x3F000000	/* 0.5 */

/* *********************************************************** */

mlib_status
mlib_SignalMulKaiser_S16_S16_i(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_f32 beta,
    mlib_s32 n,
    mlib_s32 istart,
    mlib_s32 iend)
{
	if ((src == NULL) || (dst == NULL) || (n <= 0) ||
			(*((mlib_s32 *)(&beta)) < 0))
		return (MLIB_FAILURE);

	if (n == 1) {
		dst[0] = src[0];
		return (MLIB_SUCCESS);
	}

	mlib_s16 *dst_ = dst + n - 1;
	const mlib_s16 *src_ = src + n - 1;
	const mlib_s16 * KAISER =
		mlib_s_Kaiser_S16 + (n >> 1) - 1;

	if ((n <= 1024) && ((n & (n - 1)) == 0) &&
		*(mlib_s32 *)(& beta) == BETA_G) {
		MLIB_MULKAISER2P_S16(KAISER);
		return (MLIB_SUCCESS);
	}

	mlib_d64 base, step, coef1, coef2, sqbeta, sqbeta2;
	mlib_d64 temp, temp2, t1, t2, t3;
	type_union_mlib_d64 dbl;
	mlib_s32 n1, n2, i, i0;
	mlib_s32 shift, shift1;

	n1 = n - 1;
	n2 = (n1 >> 1);
	temp = n1 * 0.5;
	coef1 = 1 / mlib_i0(beta * temp, &shift);
	temp2 = 15 / beta;
	i0 = (temp <
	    temp2) ? n2 : temp - mlib_sqrt(temp * temp - temp2 * temp2);
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

	if (istart == 1) {
		dst[0] = (mlib_s32)(coef2 * src[0]);
		dst[n1] = (mlib_s32)(coef2 * src[n1]);
	}

	for (i = istart; i <= ((iend < i0) ? iend : i0); i++) {
		mlib_s32 s0, s1;
		mlib_d64 yv4 = base * base, yv8 = yv4 * yv4, yv16 = yv8 * yv8;

		temp = KA0 + KA3 * base * (KA1_KA3 + KA2_KA3 * base + yv4) +
		    KA7 * yv8 * (KA4_KA7 + KA5_KA7 * base + yv4 * (KA6_KA7 +
		    base)) + KA11 * yv16 * (KA8_KA11 + KA9_KA11 * base +
		    yv4 * (KA10_KA11 + base)) + KA15 * yv8 * yv16 * (KA12_KA15 +
		    KA13_KA15 * base + yv4 * (KA14_KA15 + base));
		temp *= coef2;
		s0 = src[i];
		s1 = src[n1 - i];
		dst[i] = (mlib_s32)(temp * s0);
		dst[n1 - i] = (mlib_s32)(temp * s1);
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

		if (shift1 > 30)
			dst[i] = dst[n1 - i] = 0;
		else {
			mlib_s32 s0, s1;

			s0 = src[i];
			s1 = src[n1 - i];
			dst[i] = ((mlib_s32)(temp * s0)) >> shift1;
			dst[n1 - i] = ((mlib_s32)(temp * s1)) >> shift1;
		}
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_SignalMulKaiser_S16_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_f32 beta,
    mlib_s32 n)
{
	return mlib_SignalMulKaiser_S16_S16_i(dst, src, beta, n,
		1, ((n - 1) >> 1));
}

mlib_status
__mlib_SignalMulKaiser_S16(
    mlib_s16 *data,
    mlib_f32 beta,
    mlib_s32 n)
{
	return mlib_SignalMulKaiser_S16_S16_i(data, data, beta, n,
		1, ((n - 1) >> 1));
}

/* *********************************************************** */

mlib_status
mlib_SignalMulKaiser_S16S_S16S_i(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_f32 beta,
    mlib_s32 n,
    mlib_s32 istart,
    mlib_s32 iend)
{
	if ((src == NULL) || (dst == NULL) || (n <= 0) ||
			(*((mlib_s32 *)(&beta)) < 0))
		return (MLIB_FAILURE);

	if (n == 1) {
		dst[0] = src[0];
		dst[1] = src[1];
		return (MLIB_SUCCESS);
	}

	mlib_s16 *dst_ = dst + 2 * n - 1;
	const mlib_s16 *src_ = src + 2 * n - 1;
	const mlib_s16 * KAISER =
		mlib_s_Kaiser_S16 + (n >> 1) - 1;

	if ((n <= 1024) && ((n & (n - 1)) == 0) &&
		*(mlib_s32 *)(& beta) == BETA_G) {
		MLIB_MULKAISER2P_S16S(KAISER);
		return (MLIB_SUCCESS);
	}

	mlib_d64 base, step, coef1, coef2, sqbeta, sqbeta2;
	mlib_d64 temp, temp2, t1, t2, t3;
	type_union_mlib_d64 dbl;
	mlib_s32 n1, n2, i, i0;
	mlib_s32 shift, shift1;
	n1 = n - 1;
	n2 = (n1 >> 1);
	temp = n1 * 0.5;
	coef1 = 1 / mlib_i0(beta * temp, &shift);
	temp2 = 15 / beta;
	i0 = (temp <
	    temp2) ? n2 : temp - mlib_sqrt(temp * temp - temp2 * temp2);
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

	if (istart == 1) {
		dst[0] = (mlib_s32)(coef2 * src[0]);
		dst[1] = (mlib_s32)(coef2 * src[1]);
		dst[2 * n1] = (mlib_s32)(coef2 * src[2 * n1]);
		dst[2 * n1 + 1] = (mlib_s32)(coef2 * src[2 * n1 + 1]);
	}

#ifdef __SUNPRO_C
#pragma unroll(1)	/* because of compiler's bug */
#endif /* __SUNPRO_C */
	for (i = istart; i <= ((iend < i0) ? iend : i0); i++) {
		mlib_s32 s0, s1, s2, s3;
		mlib_d64 yv4 = base * base, yv8 = yv4 * yv4, yv16 = yv8 * yv8;

		temp = KA0 + KA3 * base * (KA1_KA3 + KA2_KA3 * base + yv4) +
		    KA7 * yv8 * (KA4_KA7 + KA5_KA7 * base + yv4 * (KA6_KA7 +
		    base)) + KA11 * yv16 * (KA8_KA11 + KA9_KA11 * base +
		    yv4 * (KA10_KA11 + base)) + KA15 * yv8 * yv16 * (KA12_KA15 +
		    KA13_KA15 * base + yv4 * (KA14_KA15 + base));
		temp *= coef2;
		s0 = src[2 * i];
		s1 = src[2 * i + 1];
		s2 = src[2 * (n1 - i)];
		s3 = src[2 * (n1 - i) + 1];
		dst[2 * i] = (mlib_s32)(temp * s0);
		dst[2 * i + 1] = (mlib_s32)(temp * s1);
		dst[2 * (n1 - i)] = (mlib_s32)(temp * s2);
		dst[2 * (n1 - i) + 1] = (mlib_s32)(temp * s3);
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

		if (shift1 > 30)
			dst[2 * i] = dst[2 * i + 1] = dst[2 * (n1 - i)] =
			    dst[2 * (n1 - i) + 1] = 0;
		else {
			mlib_s32 s0, s1, s2, s3;

			s0 = src[2 * i];
			s1 = src[2 * i + 1];
			s2 = src[2 * (n1 - i)];
			s3 = src[2 * (n1 - i) + 1];
			dst[2 * i] = ((mlib_s32)(temp * s0)) >> shift1;
			dst[2 * i + 1] = ((mlib_s32)(temp * s1)) >> shift1;
			dst[2 * (n1 - i)] = ((mlib_s32)(temp * s2)) >> shift1;
			dst[2 * (n1 - i) + 1] =
			    ((mlib_s32)(temp * s3)) >> shift1;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulKaiser_S16S_S16S(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_f32 beta,
    mlib_s32 n)
{
	return mlib_SignalMulKaiser_S16S_S16S_i(dst, src, beta, n,
		1, ((n - 1) >> 1));
}

mlib_status
__mlib_SignalMulKaiser_S16S(
    mlib_s16 *data,
    mlib_f32 beta,
    mlib_s32 n)
{
	return mlib_SignalMulKaiser_S16S_S16S_i(data, data, beta, n,
		1, ((n - 1) >> 1));
}

/* *********************************************************** */
