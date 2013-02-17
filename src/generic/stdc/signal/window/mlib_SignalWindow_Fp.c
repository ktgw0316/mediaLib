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

#pragma ident	"@(#)mlib_SignalWindow_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalGenBartlett_F32 - generate normalized
 *          coefficients of the corresponding window
 *
 *      mlib_SignalGenHanning_F32 - generate normalized
 *          coefficients of the corresponding window
 *
 *      mlib_SignalGenHamming_F32 - generate normalized
 *          coefficients of the corresponding window
 *
 *      mlib_SignalGenBlackman_F32 - generate normalized
 *          coefficients of the corresponding window
 *
 *      mlib_SignalGenKaiser_F32 - generate normalized
 *          coefficients of the corresponding window
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalGenBartlett_F32(mlib_f32 *window,
 *                                             mlib_s32 n);
 *      mlib_status mlib_SignalGenBlackman_F32(mlib_f32 *window,
 *                                             mlib_f32 alpha,
 *                                             mlib_s32 n);
 *      mlib_status mlib_SignalGenHamming_F32(mlib_f32 *window,
 *                                            mlib_s32 n);
 *      mlib_status mlib_SignalGenHanning_F32(mlib_f32 *window,
 *                                            mlib_s32 n);
 *      mlib_status mlib_SignalGenKaiser_F32(mlib_f32 *window,
 *                                           mlib_f32 beta,
 *                                           mlib_s32 n);
 *
 * ARGUMENTS
 *      window   Generated window coefficient array. The window
 *               coefficients are in f32 format
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
 *
 */

#include <mlib_signal.h>
#include <mlib_SysMath.h>
#include <mlib_SignalWindowUtil.h>
#include <mlib_SignalWindow.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalGenBartlett_F32 = __mlib_SignalGenBartlett_F32
#pragma weak mlib_SignalGenBlackman_F32 = __mlib_SignalGenBlackman_F32
#pragma weak mlib_SignalGenHamming_F32 = __mlib_SignalGenHamming_F32
#pragma weak mlib_SignalGenHanning_F32 = __mlib_SignalGenHanning_F32
#pragma weak mlib_SignalGenKaiser_F32 = __mlib_SignalGenKaiser_F32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalGenBartlett_F32) mlib_SignalGenBartlett_F32
    __attribute__((weak, alias("__mlib_SignalGenBartlett_F32")));
__typeof__(__mlib_SignalGenBlackman_F32) mlib_SignalGenBlackman_F32
    __attribute__((weak, alias("__mlib_SignalGenBlackman_F32")));
__typeof__(__mlib_SignalGenHamming_F32) mlib_SignalGenHamming_F32
    __attribute__((weak, alias("__mlib_SignalGenHamming_F32")));
__typeof__(__mlib_SignalGenHanning_F32) mlib_SignalGenHanning_F32
    __attribute__((weak, alias("__mlib_SignalGenHanning_F32")));
__typeof__(__mlib_SignalGenKaiser_F32) mlib_SignalGenKaiser_F32
    __attribute__((weak, alias("__mlib_SignalGenKaiser_F32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_SignalGenBartlett_F32(
    mlib_f32 *window,
    mlib_s32 n)
{
	mlib_d64 result;
	mlib_s32 n2, n1, i;
	mlib_d64 step, base;

	n1 = n - 1;
	n2 = (n1 >> 1);

	if ((window == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	n1 = n - 1;
	n2 = (n1 >> 1);
	base = 0.;
	step = 2. / (double)n1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= n2; i++) {
		result = base;
		base += step;
		window[i] = result;
		window[n1 - i] = result;
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_SignalGenBartlett_F32_i(
    mlib_f32 *window,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1)
{
	mlib_d64 result;
	mlib_s32 n1, i;
	mlib_d64 step, base;

	n1 = n - 1;

	if ((window == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	n1 = n - 1;
	step = 2. / (double)n1;
	base = step * i0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = i0; i <= i1; i++) {
		result = base;
		base += step;
		window[i] = result;
		window[n1 - i] = result;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_SignalGenHanning_F32_i(
    mlib_f32 *window,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1)
{
	mlib_d64 cbase0, cbase1, cbase2, cbase3;
	mlib_d64 cbase4, cbase5, cbase6, cbase7;
	mlib_d64 temp0, temp1, temp2, temp3, cstep;
	mlib_f32 result0, result1, result2, result3;
	mlib_s32 i;
	mlib_f32 *window2 = window + n - 1;

	if ((window == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	cstep = mlib_cos(2. * M_PI / (n - 1));
	cbase0 = .5 * mlib_cos(2. * M_PI / (n - 1) * (4 * i0));
	cbase1 = .5 * mlib_cos(2. * M_PI / (n - 1) * (4 * i0 + 1));
	cbase2 = 2. * cstep * cbase1 - cbase0;
	cbase3 = 2. * cstep * cbase2 - cbase1;
	cstep = 2. * cstep * cstep - 1.;
	cbase4 = 2. * cstep * cbase2 - cbase0;
	cbase5 = 2. * cstep * cbase3 - cbase1;
	cbase6 = 2. * cstep * cbase4 - cbase2;
	cbase7 = 2. * cstep * cbase5 - cbase3;
	cstep = 4. * cstep * cstep - 2.;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = i0; i < i1; i++) {
		result0 = .5 - cbase0;
		result1 = .5 - cbase1;
		result2 = .5 - cbase2;
		result3 = .5 - cbase3;
		window[4 * i] = result0;
		window[4 * i + 1] = result1;
		window[4 * i + 2] = result2;
		window[4 * i + 3] = result3;
		window2[-(4 * i)] = result0;
		window2[-(4 * i + 1)] = result1;
		window2[-(4 * i + 2)] = result2;
		window2[-(4 * i + 3)] = result3;
		temp0 = cstep * cbase4 - cbase0;
		temp1 = cstep * cbase5 - cbase1;
		temp2 = cstep * cbase6 - cbase2;
		temp3 = cstep * cbase7 - cbase3;
		cbase0 = cbase4;
		cbase1 = cbase5;
		cbase2 = cbase6;
		cbase3 = cbase7;
		cbase4 = temp0;
		cbase5 = temp1;
		cbase6 = temp2;
		cbase7 = temp3;
	}

	if ((n & 7) && i1 == n / 8) {
		window += 4 * i;

		switch (n & 7) {
		case 7:
			window[6] = .5 - cbase6;
		case 6:
			window[5] = .5 - cbase5;
		case 5:
			window[4] = .5 - cbase4;
		case 4:
			window[3] = .5 - cbase3;
		case 3:
			window[2] = .5 - cbase2;
		case 2:
			window[1] = .5 - cbase1;
		case 1:
			window[0] = .5 - cbase0;
		}
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_SignalGenHanning_F32(
    mlib_f32 *window,
    mlib_s32 n)
{
	return (mlib_SignalGenHanning_F32_i(window, n, 0, n / 8));
}

/* *********************************************************** */

mlib_status
mlib_SignalGenHamming_F32_i(
    mlib_f32 *window,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1)
{
	mlib_d64 cbase0, cbase1, cbase2, cbase3;
	mlib_d64 cbase4, cbase5, cbase6, cbase7;
	mlib_d64 temp0, temp1, temp2, temp3, cstep;
	mlib_f32 result0, result1, result2, result3;
	mlib_s32 i;
	mlib_f32 *window2 = window + n - 1;

	if ((window == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	cstep = mlib_cos(2. * M_PI / (n - 1));
	cbase0 = .46 * mlib_cos(2. * M_PI / (n - 1) * (4 * i0));
	cbase1 = .46 * mlib_cos(2. * M_PI / (n - 1) * (4 * i0 + 1));
	cbase2 = 2. * cstep * cbase1 - cbase0;
	cbase3 = 2. * cstep * cbase2 - cbase1;
	cstep = 2. * cstep * cstep - 1.;
	cbase4 = 2. * cstep * cbase2 - cbase0;
	cbase5 = 2. * cstep * cbase3 - cbase1;
	cbase6 = 2. * cstep * cbase4 - cbase2;
	cbase7 = 2. * cstep * cbase5 - cbase3;
	cstep = 4. * cstep * cstep - 2.;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = i0; i < i1; i++) {
		result0 = .54 - cbase0;
		result1 = .54 - cbase1;
		result2 = .54 - cbase2;
		result3 = .54 - cbase3;
		window[4 * i] = result0;
		window[4 * i + 1] = result1;
		window[4 * i + 2] = result2;
		window[4 * i + 3] = result3;
		window2[-(4 * i)] = result0;
		window2[-(4 * i + 1)] = result1;
		window2[-(4 * i + 2)] = result2;
		window2[-(4 * i + 3)] = result3;
		temp0 = cstep * cbase4 - cbase0;
		temp1 = cstep * cbase5 - cbase1;
		temp2 = cstep * cbase6 - cbase2;
		temp3 = cstep * cbase7 - cbase3;
		cbase0 = cbase4;
		cbase1 = cbase5;
		cbase2 = cbase6;
		cbase3 = cbase7;
		cbase4 = temp0;
		cbase5 = temp1;
		cbase6 = temp2;
		cbase7 = temp3;
	}

	if ((n & 7) && i1 == n / 8) {
		window += 4 * i;

		switch (n & 7) {
		case 7:
			window[6] = .54 - cbase6;
		case 6:
			window[5] = .54 - cbase5;
		case 5:
			window[4] = .54 - cbase4;
		case 4:
			window[3] = .54 - cbase3;
		case 3:
			window[2] = .54 - cbase2;
		case 2:
			window[1] = .54 - cbase1;
		case 1:
			window[0] = .54 - cbase0;
		}
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_SignalGenHamming_F32(
    mlib_f32 *window,
    mlib_s32 n)
{
	return (mlib_SignalGenHamming_F32_i(window, n, 0, n / 8));
}

/* *********************************************************** */

#define	MLIB_BLACKMAN(X)	(coeff1 - (X) - coeff2 * (X) * (X))

/* *********************************************************** */

mlib_status
mlib_SignalGenBlackman_F32_i(
    mlib_f32 *window,
    mlib_f32 alpha,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1)
{
	mlib_d64 cbase0, cbase1, cbase2, cbase3;
	mlib_d64 cbase4, cbase5, cbase6, cbase7;
	mlib_d64 temp0, temp1, temp2, temp3, cstep;
	mlib_d64 coeff1, coeff2;
	mlib_f32 result0, result1, result2, result3;
	mlib_s32 i;
	mlib_f32 *window2 = window + n - 1;

	if ((window == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	if ((alpha <= -1.0) || (alpha >= 0.0))
		return (MLIB_OUTOFRANGE);

	coeff1 = .5 + alpha;
	coeff2 = 4. * alpha;
	cstep = mlib_cos(2. * M_PI / (n - 1));
	cbase0 = 0.5 * mlib_cos(2. * M_PI / (n - 1) * (4 * i0));
	cbase1 = 0.5 * mlib_cos(2. * M_PI / (n - 1) * (4 * i0 + 1));
	cbase2 = 2. * cstep * cbase1 - cbase0;
	cbase3 = 2. * cstep * cbase2 - cbase1;
	cstep = 2. * cstep * cstep - 1.;
	cbase4 = 2. * cstep * cbase2 - cbase0;
	cbase5 = 2. * cstep * cbase3 - cbase1;
	cbase6 = 2. * cstep * cbase4 - cbase2;
	cbase7 = 2. * cstep * cbase5 - cbase3;
	cstep = 4. * cstep * cstep - 2.;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = i0; i < i1; i++) {
		result0 = MLIB_BLACKMAN(cbase0);
		result1 = MLIB_BLACKMAN(cbase1);
		result2 = MLIB_BLACKMAN(cbase2);
		result3 = MLIB_BLACKMAN(cbase3);
		window[4 * i] = result0;
		window[4 * i + 1] = result1;
		window[4 * i + 2] = result2;
		window[4 * i + 3] = result3;
		window2[-(4 * i)] = result0;
		window2[-(4 * i + 1)] = result1;
		window2[-(4 * i + 2)] = result2;
		window2[-(4 * i + 3)] = result3;
		temp0 = cstep * cbase4 - cbase0;
		temp1 = cstep * cbase5 - cbase1;
		temp2 = cstep * cbase6 - cbase2;
		temp3 = cstep * cbase7 - cbase3;
		cbase0 = cbase4;
		cbase1 = cbase5;
		cbase2 = cbase6;
		cbase3 = cbase7;
		cbase4 = temp0;
		cbase5 = temp1;
		cbase6 = temp2;
		cbase7 = temp3;
	}

	if ((n & 7) && i1 == n / 8) {
		window += 4 * i;

		switch (n & 7) {
		case 7:
			window[6] = MLIB_BLACKMAN(cbase6);
		case 6:
			window[5] = MLIB_BLACKMAN(cbase5);
		case 5:
			window[4] = MLIB_BLACKMAN(cbase4);
		case 4:
			window[3] = MLIB_BLACKMAN(cbase3);
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
__mlib_SignalGenBlackman_F32(
    mlib_f32 *window,
    mlib_f32 alpha,
    mlib_s32 n)
{
	return mlib_SignalGenBlackman_F32_i(window, alpha, n,
		0, n / 8);
}

/* *********************************************************** */

mlib_status
mlib_SignalGenKaiser_F32_i(
    mlib_f32 *window,
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

	if ((window == NULL) || (n <= 0) || (beta < 0))
		return (MLIB_FAILURE);

	n1 = n - 1;
	n2 = (n1 >> 1);
	temp1 = n1 * .5;
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

	window[0] = window[n1] = (mlib_f32)coef2;

	{
		mlib_d64 yv4 = base * base;
		mlib_d64 yv8 = yv4 * yv4;
		mlib_d64 yv16 = yv8 * yv8;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = istart; i <= ((iend < i0) ? iend : i0); i++) {

			window[i] = window[n1 - i] =
			    (mlib_f32)((KA0 + KA3 * base * (KA1_KA3 +
			    KA2_KA3 * base + yv4) + KA7 * yv8 * (KA4_KA7 +
			    KA5_KA7 * base + yv4 * (KA6_KA7 + base)) +
			    KA11 * yv16 * (KA8_KA11 + KA9_KA11 * base +
			    yv4 * (KA10_KA11 + base)) +
			    KA15 * yv8 * yv16 * (KA12_KA15 + KA13_KA15 * base +
			    yv4 * (KA14_KA15 + base))) * coef2);
			step -= sqbeta2;
			base += step;
			yv4 = base * base;
			yv8 = yv4 * yv4;
			yv16 = yv8 * yv8;
		}
	}

	{
		mlib_d64 t1, t2, t3;

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

			temp1 = mlib_constexp[shift1 & 0x7f];
			t2 -= (mlib_d64)shift1;
			t2 = KE2 * temp1 * t2 * (t2 + KE1_2) + temp1;
			shift1 >>= 7;

			t1 *= (coef1 * (t2 * t3));
			shift1 = shift - shift1;

			dbl.value = t1;
			dbl.two_int.int0 -= (shift1 << 20);
			t1 = dbl.value;

			t1 = (t1 < 1e-36) ? 0.0 : t1;
			window[i] = window[n1 - i] = (mlib_f32)t1;
		}
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_SignalGenKaiser_F32(
    mlib_f32 *window,
    mlib_f32 beta,
    mlib_s32 n)
{
	return mlib_SignalGenKaiser_F32_i(window, beta, n,
		1, (n - 1) >> 1);
}

/* *********************************************************** */
