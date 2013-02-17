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

#pragma ident	"@(#)mlib_SignalMulKaiser_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulKaiser_F32 - multiply source data on
 *          coefficients of the corresponding window
 *
 *      mlib_SignalMulKaiser_F32S - multiply stereo source
 *          data on coefficients of the corresponding window
 *
 *      mlib_SignalMulKaiser_F32_F32 - multiply source data on
 *          coefficients of the corresponding window with results
 *          in destination array
 *
 *      mlib_SignalMulKaiser_F32S_F32S - multiply stereo source
 *          data on coefficients of the corresponding window with
 *          result in destination array
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalMulKaiser_F32( mlib_f32 *data,
 *                                           mlib_f32  beta,
 *                                           mlib_s32  n);
 *      mlib_status mlib_SignalMulKaiser_F32S( mlib_f32 *data,
 *                                            mlib_f32  beta,
 *                                            mlib_s32  n);
 *      mlib_status mlib_SignalMulKaiser_F32S_F32S( mlib_f32      *dst,
 *                                                 const mlib_f32 *src,
 *                                                 mlib_f32       beta,
 *                                                 mlib_s32       n);
 *      mlib_status mlib_SignalMulKaiser_F32_F32(mlib_f32       *dst,
 *                                               const mlib_f32 *src,
 *                                               mlib_f32       beta,
 *                                               mlib_s32       n);
 * ARGUMENTS
 *      data     Input and output signal array
 *      dst      Output signal array
 *      src      Input signal array
 *      beta     Kaiser window parameter
 *      n        Length of window array
 *
 * DESCRIPTION
 *      Mono        :   dst[i] = src[i] * window[i]    i = 0..(N-1)
 *
 *      Stereo      :   dst[i] = src[i] * window[i/2]  i = 0..(2*N-1)
 *
 *      Kaiser      :   window[i] = I0(beta * sqrt(sqr((N - 1)/2) -
 *                          sqr(i - (N - 1)/2)))/I0(beta*((N - 1)/2))
 */

#include <mlib_signal.h>
#include <mlib_SysMath.h>
#include <mlib_SignalWindowUtil.h>
#include <mlib_SignalWindow.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulKaiser_F32 = __mlib_SignalMulKaiser_F32
#pragma weak mlib_SignalMulKaiser_F32S = __mlib_SignalMulKaiser_F32S
#pragma weak mlib_SignalMulKaiser_F32S_F32S = \
	__mlib_SignalMulKaiser_F32S_F32S
#pragma weak mlib_SignalMulKaiser_F32_F32 = __mlib_SignalMulKaiser_F32_F32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulKaiser_F32) mlib_SignalMulKaiser_F32
    __attribute__((weak, alias("__mlib_SignalMulKaiser_F32")));
__typeof__(__mlib_SignalMulKaiser_F32S) mlib_SignalMulKaiser_F32S
    __attribute__((weak, alias("__mlib_SignalMulKaiser_F32S")));
__typeof__(__mlib_SignalMulKaiser_F32S_F32S) mlib_SignalMulKaiser_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalMulKaiser_F32S_F32S")));
__typeof__(__mlib_SignalMulKaiser_F32_F32) mlib_SignalMulKaiser_F32_F32
    __attribute__((weak, alias("__mlib_SignalMulKaiser_F32_F32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
mlib_SignalMulKaiser_F32_F32_i(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_f32 beta,
    mlib_s32 n,
    mlib_s32 istart,
    mlib_s32 iend)
{
	mlib_d64 coef1, coef2, step, base;
	mlib_d64 sqbeta1, sqbeta2;
	mlib_d64 temp1, temp2;
	type_union_mlib_d64 dbl;
	mlib_s32 n1, n2, i, i0;
	mlib_s32 shift, shift1;

	if ((dst == NULL) || (src == NULL) || (n <= 0) || (beta < 0))
		return (MLIB_FAILURE);

	n1 = n - 1;
	n2 = (n1 >> 1);
	temp1 = n1 * .5;
	coef1 = 1. / mlib_i0(beta * temp1, &shift);
	temp2 = 15. / beta;
	i0 = (mlib_s32)((temp1 <
	    temp2) ? n2 : temp1 - mlib_sqrt(temp1 * temp1 - temp2 * temp2));
	sqbeta1 = beta * beta;
	sqbeta2 = sqbeta1 + sqbeta1;
	step = (n1 - 1) * sqbeta1;
	step -= (istart - 1) * sqbeta2;
	base = ((n1 - 1) * sqbeta1 + step) * istart / 2;

	if (shift > 1023)
		coef2 = 0.0;
	else {
		dbl.value = coef1;
		dbl.two_int.int0 -= (shift << 20);
		coef2 = dbl.value;
	}

	if (istart == 1) {
		dst[0] = (mlib_f32)(coef2 * src[0]);
		dst[n1] = (mlib_f32)(coef2 * src[n1]);
	}
	{
		mlib_d64 yv4 = base * base;
		mlib_d64 yv8 = yv4 * yv4;
		mlib_d64 yv16 = yv8 * yv8;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = istart; i <= ((iend < i0) ? iend : i0); i++) {
			mlib_d64 tempo;

			tempo =
			    ((KA0 + KA3 * base * (KA1_KA3 + KA2_KA3 * base +
			    yv4) + KA7 * yv8 * (KA4_KA7 + KA5_KA7 * base +
			    yv4 * (KA6_KA7 + base)) + KA11 * yv16 * (KA8_KA11 +
			    KA9_KA11 * base + yv4 * (KA10_KA11 + base)) +
			    KA15 * yv8 * yv16 * (KA12_KA15 + KA13_KA15 * base +
			    yv4 * (KA14_KA15 + base))) * coef2);

			dst[i] = (mlib_f32)(tempo * src[i]);
			dst[n1 - i] = (mlib_f32)(tempo * src[n1 - i]);

			step -= sqbeta2;
			base += step;

			yv4 = base * base;
			yv8 = yv4 * yv4;
			yv16 = yv8 * yv8;
		}
	}

	{
		mlib_d64 t1, t2, t3;
		mlib_f32 s0, s1;

		temp2 = mlib_sqrt(base);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i <= iend; i++) {
			t3 = (KP0 * temp2 + KP1) / (base + KQ2 + KQ1 * temp2);
			t1 = mlib_sqrt(temp2);

			t2 = K128ONLN2 * temp2;
			shift1 = (mlib_s32)t2;

			step -= sqbeta2;
			base += step;
			temp2 = mlib_sqrt(base);

			s0 = src[i];
			s1 = src[n1 - i];

			temp1 = mlib_constexp[shift1 & 0x7f];
			t2 -= (mlib_d64)shift1;
			t2 = KE2 * temp1 * t2 * (t2 + KE1_2) + temp1;
			shift1 >>= 7;

			t1 *= (coef1 * (t2 * t3));
			shift1 = shift - shift1;

			dbl.value = t1;
			dbl.two_int.int0 -= (shift1 << 20);
			t1 = dbl.value;

			t1 = (t1 < FLT_MIN) ? 0.0 : t1;

			dst[i] = (mlib_f32)(s0 * t1);
			dst[n1 - i] = (mlib_f32)(s1 * t1);
		}
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_SignalMulKaiser_F32_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_f32 beta,
    mlib_s32 n)
{
	return mlib_SignalMulKaiser_F32_F32_i(dst, src, beta, n,
		1, (n - 1) >> 1);
}

mlib_status
__mlib_SignalMulKaiser_F32(
    mlib_f32 *data,
    mlib_f32 beta,
    mlib_s32 n)
{
	return mlib_SignalMulKaiser_F32_F32_i(data, data, beta, n,
		1, (n - 1) >> 1);
}

/* *********************************************************** */

mlib_status
mlib_SignalMulKaiser_F32S_F32S_i(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_f32 beta,
    mlib_s32 n,
    mlib_s32 istart,
    mlib_s32 iend)
{
	mlib_d64 base, step;
	mlib_d64 coef1, coef2;
	mlib_d64 sqbeta1, sqbeta2;
	mlib_d64 temp1, temp2;
	type_union_mlib_d64 dbl;
	mlib_s32 n1, n2, i, i0;
	mlib_s32 shift, shift1;

	if ((dst == NULL) || (src == NULL) || (n <= 0) || (beta < 0))
		return (MLIB_FAILURE);

	n1 = n - 1;
	n2 = (n1 >> 1);
	temp1 = n1 * 0.5;
	coef1 = 1 / mlib_i0(beta * temp1, &shift);
	temp2 = 15 / beta;
	i0 = (mlib_s32)((temp1 <
	    temp2) ? n2 : temp1 - mlib_sqrt(temp1 * temp1 - temp2 * temp2));
	sqbeta1 = beta * beta;
	sqbeta2 = sqbeta1 + sqbeta1;
	step = (n1 - 1) * sqbeta1;
	step -= (istart - 1) * sqbeta2;
	base = ((n1 - 1) * sqbeta1 + step) * istart / 2;


	if (shift > 1023)
		coef2 = 0.0;
	else {
		dbl.value = coef1;
		dbl.two_int.int0 -= (shift << 20);
		coef2 = dbl.value;
	}

	if (istart == 1) {
		dst[0] = (mlib_f32)(coef2 * src[0]);
		dst[1] = (mlib_f32)(coef2 * src[1]);
		dst[2 * n1] = (mlib_f32)(coef2 * src[2 * n1]);
		dst[2 * n1 + 1] = (mlib_f32)(coef2 * src[2 * n1 + 1]);
	}

	{
		mlib_d64 yv4 = base * base;
		mlib_d64 yv8 = yv4 * yv4;
		mlib_d64 yv16 = yv8 * yv8;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = istart; i <= ((iend < i0) ? iend : i0); i++) {
			mlib_s32 beg_ind = (i << 1);
			mlib_s32 end_ind = ((n1 - i) << 1);
			mlib_d64 tempo;

			tempo =
			    ((KA0 + KA3 * base * (KA1_KA3 + KA2_KA3 * base +
			    yv4) + KA7 * yv8 * (KA4_KA7 + KA5_KA7 * base +
			    yv4 * (KA6_KA7 + base)) + KA11 * yv16 * (KA8_KA11 +
			    KA9_KA11 * base + yv4 * (KA10_KA11 + base)) +
			    KA15 * yv8 * yv16 * (KA12_KA15 + KA13_KA15 * base +
			    yv4 * (KA14_KA15 + base))) * coef2);

			dst[beg_ind] = (mlib_f32)(tempo * src[beg_ind]);
			dst[beg_ind + 1] = (mlib_f32)(tempo * src[beg_ind + 1]);
			dst[end_ind] = (mlib_f32)(tempo * src[end_ind]);
			dst[end_ind + 1] = (mlib_f32)(tempo * src[end_ind + 1]);

			step -= sqbeta2;
			base += step;

			yv4 = base * base;
			yv8 = yv4 * yv4;
			yv16 = yv8 * yv8;
		}
	}

	{
		mlib_d64 t1, t2, t3;
		mlib_f32 s0, s1, s2, s3;

		temp2 = mlib_sqrt(base);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i <= iend; i++) {
			mlib_s32 beg_ind = (i << 1);
			mlib_s32 end_ind = ((n1 - i) << 1);

			t3 = (KP0 * temp2 + KP1) / (base + KQ2 + KQ1 * temp2);
			t1 = mlib_sqrt(temp2);

			t2 = K128ONLN2 * temp2;
			shift1 = (mlib_s32)t2;

			step -= sqbeta2;
			base += step;
			temp2 = mlib_sqrt(base);

			s0 = src[beg_ind];
			s1 = src[beg_ind + 1];
			s2 = src[end_ind];
			s3 = src[end_ind + 1];

			temp1 = mlib_constexp[shift1 & 0x7f];
			t2 -= (mlib_d64)shift1;
			t2 = KE2 * temp1 * t2 * (t2 + KE1_2) + temp1;
			shift1 >>= 7;

			t1 *= (coef1 * (t2 * t3));
			shift1 = shift - shift1;

			dbl.value = t1;
			dbl.two_int.int0 -= (shift1 << 20);
			t1 = dbl.value;

			t1 = (t1 < FLT_MIN) ? 0.0 : t1;
			dst[beg_ind] = (mlib_f32)(t1 * s0);
			dst[beg_ind + 1] = (mlib_f32)(t1 * s1);
			dst[end_ind] = (mlib_f32)(t1 * s2);
			dst[end_ind + 1] = (mlib_f32)(t1 * s3);
		}
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_SignalMulKaiser_F32S_F32S(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_f32 beta,
    mlib_s32 n)
{
	return mlib_SignalMulKaiser_F32S_F32S_i(dst, src, beta, n,
		1, (n - 1) >> 1);
}

mlib_status
__mlib_SignalMulKaiser_F32S(
    mlib_f32 *data,
    mlib_f32 beta,
    mlib_s32 n)
{
	return mlib_SignalMulKaiser_F32S_F32S_i(data, data, beta, n,
		1, (n - 1) >> 1);
}

/* *********************************************************** */
