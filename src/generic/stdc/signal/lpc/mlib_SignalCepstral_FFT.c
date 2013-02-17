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

#pragma ident	"@(#)mlib_SignalCepstral_FFT.c	9.3	07/10/09 SMI"

#include <mlib_signal.h>
#include <mlib_SignalCepstral.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_fft_r2c_ll = __mlib_fft_r2c_ll
#pragma weak mlib_fft_r2r_1l = __mlib_fft_r2r_1l

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_fft_r2c_ll) mlib_fft_r2c_ll
    __attribute__((weak, alias("__mlib_fft_r2c_ll")));
__typeof__(__mlib_fft_r2r_1l) mlib_fft_r2r_1l
    __attribute__((weak, alias("__mlib_fft_r2r_1l")));
#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status mlib_SignalFFT_1_D64C(
    mlib_d64 *datac,
    mlib_s32 order);
mlib_status mlib_SignalIFFT_2_D64C(
    mlib_d64 *dstc,
    mlib_s32 order);

static void mlib_intr_ifft_r2c(
    deal_t data[],
    mlib_s32 order);
static void mlib_intr_fft_std(
    deal_t *data,
    mlib_s32 order,
    mlib_s32 isign);

#ifdef __SUNPRO_C
#pragma no_inline(mlib_intr_ifft_r2c, mlib_intr_fft_std)
#endif /* __SUNPRO_C */

/* *********************************************************** */

#if USE_MLIB_FFT_STD && USE_MLIB_FFT_F32
#error USE_MLIB_FFT_STD && USE_MLIB_FFT_F32
#endif /* USE_MLIB_FFT_STD && USE_MLIB_FFT_F32 */

#if USE_MLIB_FFT_F32 && USE_MLIB_FFT_D64
#error USE_MLIB_FFT_F32 && USE_MLIB_FFT_D64
#endif /* USE_MLIB_FFT_F32 && USE_MLIB_FFT_D64 */

#if USE_MLIB_FFT_STD && USE_MLIB_FFT_D64
#error USE_MLIB_FFT_STD && USE_MLIB_FFT_D64
#endif /* USE_MLIB_FFT_STD && USE_MLIB_FFT_D64 */

#if !USE_MLIB_FFT_STD && !USE_MLIB_FFT_D64 && !USE_MLIB_FFT_F32
#error Please define USE_MLIB_FFT_STD or USE_MLIB_FFT_F32 or USE_MLIB_FFT_D64
#endif /* !USE_MLIB_FFT_STD && !USE_MLIB_FFT_D64 && !USE_MLIB_FFT_F32 */

/* *********************************************************** */

void
__mlib_fft_r2c_ll_i(
	deal_t data[],
	mlib_s32 order,
	mlib_d64 sign,
	mlib_s32 i0,
	mlib_s32 i1,
	mlib_d64 rot_r,
	mlib_d64 rot_i)
{
	mlib_s32 iii;
	mlib_s32 length = 1 << order;
	deal_t incr_r, incr_i, wtmp;
	wtmp = SINUS(order);
	incr_r = 1. - 2. * wtmp * wtmp;
	incr_i = sign * SINUS(order - 1);

	for (iii = i0; iii < i1; iii++) {
		mlib_s32 i1 = 2 * iii, i2 = i1 + 1;
		mlib_s32 i3 = length - i1, i4 = i3 + 1;
		deal_t d1_r = data[i1], d1_i = data[i2];
		deal_t d2_r = data[i3], d2_i = -data[i4];
		deal_t h1r, h1i, h2r, h2i, h2rot_r, h2rot_i;
		h1r = (d1_r + d2_r);
		h1i = (d1_i + d2_i);
		h2r = (d1_i - d2_i);
		h2i = -(d1_r - d2_r);
		h2rot_r = rot_r * h2r - rot_i * h2i;
		h2rot_i = rot_r * h2i + rot_i * h2r;
		data[i1] = .5 * (h1r + h2rot_r);
		data[i2] = -.5 * (h1i + h2rot_i);
		data[i3] = .5 * (h1r - h2rot_r);
		data[i4] = -.5 * (-h1i + h2rot_i);
		ROTATE_CMPLX_TMP(rot, incr, wtmp);
	}
}


void
__mlib_fft_r2c_ll(
    deal_t data[],
    mlib_s32 order,
    mlib_d64 sign)
{
	mlib_s32 length = 1 << order;
	mlib_s32 quarter = length >> 2;
	mlib_s32 iii;
	deal_t wr, wi, wpr, wpi, wtemp;

	wtemp = SINUS(order);
	wr = wpr = 1. - 2. * wtemp * wtemp;
	wi = wpi = sign * SINUS(order - 1);

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (iii = 1; iii < quarter; iii++) {
		mlib_s32 i1 = 2 * iii;
		mlib_s32 i2 = i1 + 1;
		mlib_s32 i3 = length - i1;
		mlib_s32 i4 = i3 + 1;

		deal_t d1_r = data[i1], d1_i = data[i2];
		deal_t d2_r = data[i3], d2_i = -data[i4];
		deal_t h1r, h1i, h2r, h2i, h2rot_r, h2rot_i;

		h1r = (d1_r + d2_r);
		h1i = (d1_i + d2_i);
		h2r = (d1_i - d2_i);
		h2i = -(d1_r - d2_r);
		h2rot_r = wr * h2r - wi * h2i;
		h2rot_i = wr * h2i + wi * h2r;
		data[i1] = .5 * (h1r + h2rot_r);
		data[i2] = -.5 * (h1i + h2rot_i);
		data[i3] = .5 * (h1r - h2rot_r);
		data[i4] = -.5 * (-h1i + h2rot_i);
		wr = (wtemp = wr) * wpr - wi * wpi;
		wi = wi * wpr + wtemp * wpi;
	}

	data[0] = (wtemp = data[0]) + data[1];
	wtemp -= data[1];
	data[1] = 0;
	if (sign > 0.0)
		data[2 * quarter + 1] = -data[2 * quarter + 1];
	data[length] = wtemp;
	data[length + 1] = 0;

#if REAL_FFT_NEEDWASTE
	for (iii = length + 2; iii < 2 * length; iii += 2) {
		data[iii] = data[2 * length - iii];
		data[iii + 1] = -data[2 * length - iii + 1];
	}

#endif /* REAL_FFT_NEEDWASTE */
}

mlib_d64
__mlib_fft_r2r_1l_i(
	deal_t data[],
	mlib_s32 order,
	mlib_s32 i0,
	mlib_s32 i1,
	mlib_d64 wr_,
	mlib_d64 wi_)
{
	mlib_s32 j;
	mlib_s32 length = 1 << order;
	deal_t sum = 0.0, y1, y2;
	deal_t wi = wi_, wr = wr_, wpi, wpr, wtemp;

	wtemp = SINUS(order + 1);
	wpr = 1.0 - 2.0 * wtemp * wtemp;
	wpi = SINUS(order);

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = i0; j < i1; j++) {
		y1 = .5 * (data[j] + data[length - j]);
		y2 = (data[j] - data[length - j]);
		data[j] = y1 - wi * y2;
		data[length - j] = y1 + wi * y2;
		sum += wr * y2;
		wr = (wtemp = wr) * wpr - wi * wpi;
		wi = wi * wpr + wtemp * wpi;
	}
	return (sum);
}

mlib_d64
__mlib_fft_r2r_1l(
    deal_t data[],
    mlib_s32 order)
{
	mlib_s32 j;
	mlib_s32 length = 1 << order;
	mlib_s32 halflength = length >> 1;
	deal_t sum, y1, y2;
	deal_t wi, wr, wpi, wpr, wtemp;

	wtemp = SINUS(order + 1);
	wr = wpr = 1.0 - 2.0 * wtemp * wtemp;
	wi = wpi = SINUS(order);

	sum = 0.5 * (data[0] - data[length]);
	data[0] = 0.5 * (data[0] + data[length]);

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 1; j < halflength; j++) {
		y1 = .5 * (data[j] + data[length - j]);
		y2 = (data[j] - data[length - j]);
		data[j] = y1 - wi * y2;
		data[length - j] = y1 + wi * y2;
		sum += wr * y2;
		wr = (wtemp = wr) * wpr - wi * wpi;
		wi = wi * wpr + wtemp * wpi;
	}
	return (sum);
}

void
mlib_fft_r2c(
    deal_t data[],
    mlib_s32 order)
{
	mlib_intr_fft_std(data, order - 1, INVERS_FFT);

	mlib_fft_r2c_ll(data, order, 1.0);
}

/* *********************************************************** */

static void
mlib_intr_ifft_r2c(
    deal_t data[],
    mlib_s32 order)
{
	mlib_s32 length = 1 << order;
	mlib_s32 iii;

	mlib_intr_fft_std(data, order - 1, DIRECT_FFT);

	mlib_fft_r2c_ll(data, order, -1.0);

#if REAL_IFFT_NEEDWASTE
	for (iii = length + 2; iii < 2 * length; iii += 2) {
		data[iii] = data[2 * length - iii];
		data[iii + 1] = -data[2 * length - iii + 1];
	}

#endif /* REAL_IFFT_NEEDWASTE */
}

/* *********************************************************** */

void
mlib_fft_r2r(
    deal_t *data,
    mlib_s32 order)
{
	mlib_s32 j;
	mlib_s32 length = 1 << order;
	deal_t sum = mlib_fft_r2r_1l(data, order);

	mlib_intr_ifft_r2c(data, order);

	data[1] = sum;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 3; j < length; j += 2) {
		sum += data[j];
		data[j] = sum;
	}

#if ICOS_FFT_NEEDWASTE
	for (j = length + 1; j < 2 * length; j++) {
		data[j] = data[2 * length - j];
	}

#endif /* ICOS_FFT_NEEDWASTE */
}

/* *********************************************************** */

void
mlib_fft_r2r2(
    deal_t *data,
    mlib_s32 order)
{
/*
 * costrans2 of 1 << order real-valued points
 *        N-1
 * F[k] = SUM f[i]*cos(PI*k*(j+0.5)/N)
 *        j = 0
 */
	mlib_s32 i;
	mlib_s32 length = 1 << order;
	mlib_s32 halflength = length >> 1;
	deal_t sum, sum1, y1, y2;
	deal_t wpr;
	deal_t wi0_0, wi1_0, wi2_0, wi0_1, wi1_1, wi2_1;
	deal_t wr0_0, wr1_0, wr2_0, wr0_1, wr1_1, wr2_1;

	if (order == 0)
		return;
	if (order == 1) {
		wi0_0 = data[0];
		wi1_0 = data[1];
		data[0] = wi0_0 + wi1_0;
		data[1] = 0.707106781186548 * (wi0_0 - wi1_0);
		return;
	}

	length = 1 << order;
	halflength = length >> 1;

	wi2_0 = SINUS(order + 1);
	wi1_0 = -wi2_0;
	wpr = (deal_t)2.0 - (deal_t)(4.0) * wi2_0 * wi2_0;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < halflength; i++) {
		y1 = (deal_t)0.5 *
			(data[i] + data[length - i - 1]);

		y2 = wi2_0 * (data[i] - data[length - i - 1]);
		data[i] = y1 + y2;
		data[length - i - 1] = y1 - y2;

		wi0_0 = wi1_0;
		wi1_0 = wi2_0;
		wi2_0 = wpr * wi1_0 - wi0_0;
	}

	mlib_intr_ifft_r2c(data, order);

	wi2_0 = SINUS(order);
	wi1_0 = -wi2_0;
	wi2_1 = SINUS(order - 1);
	wi1_1 = 0;

	wr2_0 = wr1_0 = (deal_t)0.5 *wpr;

	wr2_1 = (deal_t)1.0 - (deal_t)(2.0) * wi2_0 * wi2_0;
	wr1_1 = (deal_t)1.0;

	wpr = (deal_t)2.0 *wr2_1;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 2; i < length - 2; i += 4) {
		y1 = data[i] * wr2_0 - data[i + 1] * wi2_0;
		y2 = data[i + 1] * wr2_0 + data[i] * wi2_0;
		data[i] = y1;
		data[i + 1] = y2;

		wi0_0 = wi1_0;
		wi1_0 = wi2_0;
		wi2_0 = wpr * wi1_0 - wi0_0;

		wr0_0 = wr1_0;
		wr1_0 = wr2_0;
		wr2_0 = wpr * wr1_0 - wr0_0;

		y1 = data[i + 2] * wr2_1 - data[i + 3] * wi2_1;
		y2 = data[i + 3] * wr2_1 + data[i + 2] * wi2_1;
		data[i + 2] = y1;
		data[i + 3] = y2;

		wi0_1 = wi1_1;
		wi1_1 = wi2_1;
		wi2_1 = wpr * wi1_1 - wi0_1;

		wr0_1 = wr1_1;
		wr1_1 = wr2_1;
		wr2_1 = wpr * wr1_1 - wr0_1;
	}

	y1 = data[i] * wr2_0 - data[i + 1] * wi2_0;
	y2 = data[i + 1] * wr2_0 + data[i] * wi2_0;
	data[i] = y1;
	data[i + 1] = y2;

	sum = (deal_t)0.5 *data[length];

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = length - 1; i >= 1; i -= 2) {
		sum1 = sum;
		sum += data[i];
		data[i] = sum1;
	}
}

/* *********************************************************** */

static void
mlib_intr_fft_std(
    deal_t *data,
    mlib_s32 order,
    mlib_s32 isign)
{
#if USE_MLIB_FFT_D64

	if (isign == DIRECT_FFT)
		mlib_SignalFFT_1_D64C(data, order);
	else
		mlib_SignalIFFT_2_D64C(data, order);

#endif /* USE_MLIB_FFT_D64 */

#if USE_MLIB_FFT_F32

	if (isign == DIRECT_FFT)
		mlib_SignalFFT_1_F32C(data, order);
	else
		mlib_SignalIFFT_2_F32C(data, order);

#endif /* USE_MLIB_FFT_F32 */

#if USE_MLIB_FFT_STD

#define	SWAP(a, b, temp)	temp = (a);   \
				(a) = (b);    \
				(b) = temp
/*
 * From 'Numerical Recipes in C' ( adjusted for zero-started data array )
 */
	deal_t wtemp, wr, wpr, wpi, wi, theta;
	deal_t tempr, tempi;
	mlib_s32 iii, jjj, mmm, mmax, istep;
	mlib_s32 length = 1 << order;
	mlib_s32 cmlxlength = length << 1
/*
 * bit reversion
 */
	    for (iii = jjj = 0; iii < (cmlxlength - 1); iii += 2) {
		if (jjj > iii) {
			SWAP(data[jjj], data[iii], wtemp);
			SWAP(data[jjj + 1], data[iii + 1], wtemp);
		}

		mmm = cmlxlength >> 1;
		while (mmm >= 2 && (jjj + 1) > mmm) {
			jjj -= mmm;
			mmm >>= 1;
		}

		jjj += mmm;
	}

/*
 * main stage
 */
	mmax = 2;
	while (cmlxlength > mmax) {
		istep = mmax << 1;
		theta = isign * (2 * M_PI / mmax);
		wtemp = mlib_sin(0.5 * theta);
		wpr = -2.0 * wtemp * wtemp;
		wpi = mlib_sin(theta);
		wr = 1.0;
		wi = 0.0;

		for (mmm = 0; mmm < (mmax - 1); mmm += 2) {
			for (iii = mmm; iii <= (cmlxlength - 1); iii += istep) {
				jjj = iii + mmax;
				tempr = wr * data[jjj] - wi * data[jjj + 1];
				tempi = wr * data[jjj + 1] + wi * data[jjj];
				data[jjj] = data[iii] - tempr;
				data[jjj + 1] = data[iii + 1] - tempi;
				data[iii] += tempr;
				data[iii + 1] += tempi;
			}

			wr = (wtemp = wr) * wpr - wi * wpi + wr;
			wi = wi * wpr + wtemp * wpi + wi;
		}

		mmax = istep;
	}

#endif /* USE_MLIB_FFT_STD */
}

/* *********************************************************** */
