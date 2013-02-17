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

#pragma ident	"@(#)mlib_v_SignalWindow_Fp.c	9.3	07/11/05 SMI"

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
 */

#include <mlib_signal.h>
#include <mlib_SysMath.h>
#include <mlib_SignalWindowUtil.h>
#include <vis_proto.h>

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

typedef union
{
	mlib_d64 d64;
	struct
	{
		mlib_f32 i0, i1;
	} f32x2;
} mlib_union64;

/* *********************************************************** */

#define	COUNT(result)                                                    \
	yv4 = base * base;                                               \
	yv8 = yv4 * yv4;                                                 \
	yv16 = yv8 * yv8;                                                \
	result =                                                         \
	    (mlib_f32)((KA0 + KA3 * base * (KA1_KA3 + KA2_KA3 * base +   \
	    yv4) + KA7 * yv8 * (KA4_KA7 + KA5_KA7 * base +               \
	    yv4 * (KA6_KA7 + base)) + KA11 * yv16 * (KA8_KA11 +          \
	    KA9_KA11 * base + yv4 * (KA10_KA11 + base)) +                \
	    KA15 * yv8 * yv16 * (KA12_KA15 + KA13_KA15 * base +          \
	    yv4 * (KA14_KA15 + base))) * coef2);                         \
	step -= sqbeta2;                                                 \
	base += step

/* *********************************************************** */

#define	COUNT_RESULT(result)                                     \
	t3 = (KP0 * temp2 + KP1) / (base + KQ2 + KQ1 * temp2);   \
	t1 = mlib_sqrt(temp2);                                   \
	t2 = K128ONLN2 * temp2;                                  \
	shift1 = (mlib_s32)t2;                                   \
	step -= sqbeta2;                                         \
	base += step;                                            \
	temp2 = mlib_sqrt(base);                                 \
	temp1 = mlib_constexp[shift1 & 0x7f];                    \
	t2 -= (mlib_d64)shift1;                                  \
	t2 = KE2 * temp1 * t2 * (t2 + KE1_2) + temp1;            \
	shift1 >>= 7;                                            \
	t1 *= (coef1 * (t2 * t3));                               \
	shift1 = shift - shift1;                                 \
	dbl.value = t1;                                          \
	dbl.two_int.int0 -= (shift1 << 20);                      \
	t1 = dbl.value;                                          \
	t1 = (t1 < 1e-36) ? 0.0 : t1;                            \
	result = (mlib_f32)t1

/* *********************************************************** */

mlib_status
__mlib_SignalGenBartlett_F32(
    mlib_f32 *window,
    mlib_s32 n)
{
	mlib_union64 tmp_1;
	mlib_s32 n1, i, k = 0;
	mlib_d64 tmp1, cbase0, cbase1, step, *dwin, *dwin2;
	mlib_d64 tmp2_last, tmp2_new, tmp2;
	mlib_f32 *window2 = window + n - 1;
	mlib_u8 emask;

	if ((window == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	n1 = n - 1;
	step = 2. / (double)n1;
	cbase0 = 0.;
	cbase1 = cbase0 + step;

	if ((mlib_addr)window & 7) {
		(*window++) = 0.;
		*window2-- = 0.;
		n = n - 2;
		cbase0 = cbase1;
		cbase1 = cbase0 + step;
	}

	if (n == 1) {
		*window = cbase0;
		return (MLIB_SUCCESS);
	}

	dwin = (mlib_d64 *)(window);
	vis_write_bmask(0x45670123, 0);

	if (!((mlib_addr)(window2 - 1) & 7)) {

		dwin2 = (mlib_d64 *)(window2 - 1);
#pragma pipeloop(0)
		for (i = 0; i < n / 4; i++) {
			tmp_1.f32x2.i0 = cbase0;
			tmp_1.f32x2.i1 = cbase1;
			tmp1 = tmp_1.d64;

			*dwin = tmp1;
			dwin++;
			tmp1 = vis_bshuffle(tmp1, tmp1);
			*dwin2 = tmp1;
			dwin2--;

			cbase0 = cbase1 + step;
			cbase1 = cbase0 + step;
		}

		if (n & 3) {
			window += 2 * i + k;
			window[1] = cbase0;
			window[0] = cbase0;
		}
	} else {

		if (n > 3) {

			tmp_1.f32x2.i0 = cbase0;
			tmp_1.f32x2.i1 = cbase1;
			tmp1 = tmp_1.d64;

			*dwin = tmp1;
			dwin++;
			dwin2 = vis_alignaddr((void *)(window2 + 1), 0);
			emask = 12;
			tmp2_last = vis_bshuffle(tmp1, tmp1);
			vis_pst_16(tmp1, dwin2, emask);
			dwin2--;

			cbase0 = cbase1 + step;
			cbase1 = cbase0 + step;

			n = n - 3;
			k = k + 2;

#pragma pipeloop(0)
			for (i = 0; i < n / 4; i++) {
				tmp_1.f32x2.i0 = cbase0;
				tmp_1.f32x2.i1 = cbase1;
				tmp1 = tmp_1.d64;

				*dwin = tmp1;
				dwin++;
				tmp2 = vis_bshuffle(tmp1, tmp1);
				tmp2_new = vis_faligndata(tmp2, tmp2_last);
				*dwin2 = tmp2_new;
				dwin2--;
				tmp2_last = tmp2;

				cbase0 = cbase1 + step;
				cbase1 = cbase0 + step;
			}

			if (n & 3) {
				window += 2 * i + k;
				window[1] = cbase0 - step;
				window[0] = cbase1 - step;
			}
		} else {
			window[2] = cbase0;
			window[1] = cbase1;
			window[0] = cbase0;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalGenHanning_F32(
    mlib_f32 *window,
    mlib_s32 n)
{
	mlib_union64 tmp_1;
	mlib_d64 cbase0, cbase1, cbase2, cbase3;
	mlib_d64 tmp2_last, tmp2_new, tmp1, tmp2;
	mlib_d64 temp0, temp1, cstep, *dwin, *dwin2;
	mlib_s32 i, k = 0;
	mlib_f32 *window2 = window + n - 1;
	mlib_u8 emask;

	if ((window == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	cstep = mlib_cos(2. * M_PI / (n - 1));
	cbase0 = .5;
	cbase1 = .5 * cstep;

	if ((mlib_addr)window & 7) {
		(*window++) = 0.;
		*window2-- = 0.;
		n = n - 2;
		cbase0 = cbase1;
		cbase1 = 2. * cstep * cbase0 - .5;
	}

	dwin = (mlib_d64 *)(window);
	vis_write_bmask(0x45670123, 0);

	cbase2 = 2. * cstep * cbase1 - cbase0;
	cbase3 = 2. * cstep * cbase2 - cbase1;
	cstep = 4. * cstep * cstep - 2.;

	if (!((mlib_addr)(window2 - 1) & 7)) {

		dwin2 = (mlib_d64 *)(window2 - 1);

#pragma pipeloop(0)
		for (i = 0; i < n / 4; i++) {
			tmp_1.f32x2.i0 = .5 - cbase0;
			tmp_1.f32x2.i1 = .5 - cbase1;
			tmp1 = tmp_1.d64;

			*dwin = tmp1;
			dwin++;
			tmp2 = vis_bshuffle(tmp1, tmp1);
			*dwin2 = tmp2;
			dwin2--;

			temp0 = cstep * cbase2 - cbase0;
			temp1 = cstep * cbase3 - cbase1;

			cbase0 = cbase2;
			cbase1 = cbase3;

			cbase2 = temp0;
			cbase3 = temp1;
		}
	} else {

		if (n > 3) {

			tmp_1.f32x2.i0 = .5 - cbase0;
			tmp_1.f32x2.i1 = .5 - cbase1;
			tmp1 = tmp_1.d64;
			*dwin = tmp1;
			dwin++;

			dwin2 = vis_alignaddr((void *)(window2 + 1), 0);
			emask = 12;
			tmp2_last = vis_bshuffle(tmp1, tmp1);
			vis_pst_16(tmp1, dwin2, emask);
			dwin2--;

			temp0 = cstep * cbase2 - cbase0;
			temp1 = cstep * cbase3 - cbase1;

			cbase0 = cbase2;
			cbase1 = cbase3;

			cbase2 = temp0;
			cbase3 = temp1;

			n = n - 3;
			k = k + 2;
		}
#pragma pipeloop(0)
		for (i = 0; i < n / 4; i++) {
			tmp_1.f32x2.i0 = .5 - cbase0;
			tmp_1.f32x2.i1 = .5 - cbase1;
			tmp1 = tmp_1.d64;

			*dwin = tmp1;
			dwin++;
			tmp2 = vis_bshuffle(tmp1, tmp1);
			tmp2_new = vis_faligndata(tmp2, tmp2_last);
			*dwin2 = tmp2_new;
			dwin2--;
			tmp2_last = tmp2;

			temp0 = cstep * cbase2 - cbase0;
			temp1 = cstep * cbase3 - cbase1;

			cbase0 = cbase2;
			cbase1 = cbase3;

			cbase2 = temp0;
			cbase3 = temp1;
		}
	}

	if (n & 3) {
		window += 2 * i + k;
		switch (n & 3) {
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

/* *********************************************************** */

mlib_status
__mlib_SignalGenHamming_F32(
    mlib_f32 *window,
    mlib_s32 n)
{
	mlib_union64 tmp_1;
	mlib_d64 cbase0, cbase1, cbase2, cbase3;
	mlib_d64 tmp2_last, tmp2_new, tmp1, tmp2;
	mlib_d64 temp0, temp1, cstep, *dwin, *dwin2;
	mlib_s32 i, k = 0;
	mlib_f32 *window2 = window + n - 1;
	mlib_u8 emask;

	if ((window == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	cstep = mlib_cos(2. * M_PI / (n - 1));
	cbase0 = .46;
	cbase1 = .46 * cstep;

	if ((mlib_addr)window & 7) {
		(*window++) = .08;
		*window2-- = .08;
		n = n - 2;
		cbase0 = cbase1;
		cbase1 = 2. * cstep * cbase0 - .46;
	}

	dwin = (mlib_d64 *)(window);
	vis_write_bmask(0x45670123, 0);

	cbase2 = 2. * cstep * cbase1 - cbase0;
	cbase3 = 2. * cstep * cbase2 - cbase1;
	cstep = 4. * cstep * cstep - 2.;

	if (!((mlib_addr)(window2 - 1) & 7)) {

		dwin2 = (mlib_d64 *)(window2 - 1);

#pragma pipeloop(0)
		for (i = 0; i < n / 4; i++) {
			tmp_1.f32x2.i0 = .54 - cbase0;
			tmp_1.f32x2.i1 = .54 - cbase1;
			tmp1 = tmp_1.d64;

			*dwin = tmp1;
			dwin++;
			tmp2 = vis_bshuffle(tmp1, tmp1);
			*dwin2 = tmp2;
			dwin2--;

			temp0 = cstep * cbase2 - cbase0;
			temp1 = cstep * cbase3 - cbase1;

			cbase0 = cbase2;
			cbase1 = cbase3;

			cbase2 = temp0;
			cbase3 = temp1;
		}
	} else {

		if (n > 3) {

			tmp_1.f32x2.i0 = .54 - cbase0;
			tmp_1.f32x2.i1 = .54 - cbase1;
			tmp1 = tmp_1.d64;
			*dwin = tmp1;
			dwin++;

			dwin2 = vis_alignaddr((void *)(window2 + 1), 0);
			emask = 12;
			tmp2_last = vis_bshuffle(tmp1, tmp1);
			vis_pst_16(tmp1, dwin2, emask);
			dwin2--;

			temp0 = cstep * cbase2 - cbase0;
			temp1 = cstep * cbase3 - cbase1;

			cbase0 = cbase2;
			cbase1 = cbase3;

			cbase2 = temp0;
			cbase3 = temp1;

			n = n - 3;
			k = k + 2;
		}
#pragma pipeloop(0)
		for (i = 0; i < n / 4; i++) {
			tmp_1.f32x2.i0 = .54 - cbase0;
			tmp_1.f32x2.i1 = .54 - cbase1;
			tmp1 = tmp_1.d64;

			*dwin = tmp1;
			dwin++;
			tmp2 = vis_bshuffle(tmp1, tmp1);
			tmp2_new = vis_faligndata(tmp2, tmp2_last);
			*dwin2 = tmp2_new;
			dwin2--;
			tmp2_last = tmp2;

			temp0 = cstep * cbase2 - cbase0;
			temp1 = cstep * cbase3 - cbase1;

			cbase0 = cbase2;
			cbase1 = cbase3;

			cbase2 = temp0;
			cbase3 = temp1;
		}
	}

	if (n & 3) {
		window += 2 * i + k;

		switch (n & 3) {
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

/* *********************************************************** */

#define	MLIB_BLACKMAN(X)	(coeff1 - (X) - coeff2 * (X) * (X))

/* *********************************************************** */

mlib_status
__mlib_SignalGenBlackman_F32(
    mlib_f32 *window,
    mlib_f32 alpha,
    mlib_s32 n)
{
	mlib_union64 tmp_1;
	mlib_d64 coeff1, coeff2;
	mlib_d64 cbase0, cbase1, cbase2, cbase3;
	mlib_d64 tmp2_last, tmp2_new, tmp1, tmp2;
	mlib_d64 temp0, temp1, cstep, *dwin, *dwin2;
	mlib_s32 i, k = 0;
	mlib_f32 *window2 = window + n - 1;
	mlib_u8 emask;

	if ((window == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	if ((alpha <= -1.0) || (alpha >= 0.0))
		return (MLIB_OUTOFRANGE);

	coeff1 = .5 + alpha;
	coeff2 = 4. * alpha;
	cstep = mlib_cos(2. * M_PI / (n - 1));
	cbase0 = .5;
	cbase1 = .5 * cstep;

	if ((mlib_addr)window & 7) {
		(*window++) = *window2-- = MLIB_BLACKMAN(cbase0);
		n = n - 2;
		cbase0 = cbase1;
		cbase1 = 2. * cstep * cbase0 - .5;
	}

	dwin = (mlib_d64 *)(window);
	vis_write_bmask(0x45670123, 0);

	cbase2 = 2. * cstep * cbase1 - cbase0;
	cbase3 = 2. * cstep * cbase2 - cbase1;
	cstep = 4. * cstep * cstep - 2.;

	if (!((mlib_addr)(window2 - 1) & 7)) {

		dwin2 = (mlib_d64 *)(window2 - 1);

#pragma pipeloop(0)
		for (i = 0; i < n / 4; i++) {
			tmp_1.f32x2.i0 = MLIB_BLACKMAN(cbase0);
			tmp_1.f32x2.i1 = MLIB_BLACKMAN(cbase1);
			tmp1 = tmp_1.d64;

			*dwin = tmp1;
			dwin++;
			tmp2 = vis_bshuffle(tmp1, tmp1);
			*dwin2 = tmp2;
			dwin2--;

			temp0 = cstep * cbase2 - cbase0;
			temp1 = cstep * cbase3 - cbase1;

			cbase0 = cbase2;
			cbase1 = cbase3;

			cbase2 = temp0;
			cbase3 = temp1;
		}
	} else {

		if (n > 3) {

			tmp_1.f32x2.i0 = MLIB_BLACKMAN(cbase0);
			tmp_1.f32x2.i1 = MLIB_BLACKMAN(cbase1);
			tmp1 = tmp_1.d64;
			*dwin = tmp1;
			dwin++;

			dwin2 = vis_alignaddr((void *)(window2 + 1), 0);
			emask = 12;
			tmp2_last = vis_bshuffle(tmp1, tmp1);
			vis_pst_16(tmp1, dwin2, emask);
			dwin2--;

			temp0 = cstep * cbase2 - cbase0;
			temp1 = cstep * cbase3 - cbase1;

			cbase0 = cbase2;
			cbase1 = cbase3;

			cbase2 = temp0;
			cbase3 = temp1;

			n = n - 3;
			k = k + 2;
		}
#pragma pipeloop(0)
		for (i = 0; i < n / 4; i++) {
			tmp_1.f32x2.i0 = MLIB_BLACKMAN(cbase0);
			tmp_1.f32x2.i1 = MLIB_BLACKMAN(cbase1);
			tmp1 = tmp_1.d64;

			*dwin = tmp1;
			dwin++;
			tmp2 = vis_bshuffle(tmp1, tmp1);
			tmp2_new = vis_faligndata(tmp2, tmp2_last);
			*dwin2 = tmp2_new;
			dwin2--;
			tmp2_last = tmp2;

			temp0 = cstep * cbase2 - cbase0;
			temp1 = cstep * cbase3 - cbase1;

			cbase0 = cbase2;
			cbase1 = cbase3;

			cbase2 = temp0;
			cbase3 = temp1;
		}
	}

	if (n & 3) {
		window += 2 * i + k;

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

/* *********************************************************** */

mlib_status
__mlib_SignalGenKaiser_F32(
    mlib_f32 *window,
    mlib_f32 beta,
    mlib_s32 n)
{
	mlib_union64 tmp0;
	mlib_d64 base, step;
	mlib_d64 coef1, coef2;
	mlib_d64 sqbeta1, sqbeta2;
	mlib_d64 temp1, temp2;
	type_union_mlib_d64 dbl;
	mlib_s32 n1, n2, i, i0, l = 0;
	mlib_s32 shift, shift1;
	mlib_f32 result, result0, result1, temp_0, temp_1;
	mlib_d64 yv4, yv8, yv16, t1, t2, t3, win2, tmp2;
	mlib_f32 *window2 = window + n - 1;
	mlib_d64 *dwin1, *dwin2;

	if ((window == NULL) || (n <= 0) || (beta < 0))
		return (MLIB_FAILURE);

	vis_write_bmask(0x45670123, 0);

	n1 = n - 1;
	n2 = (n1 >> 1);
	temp1 = n1 * .5;
	coef1 = 1 / mlib_i0(beta * temp1, &shift);
	temp2 = 15 / beta;
	i0 = (mlib_s32)((temp1 <
	    temp2) ? n2 : temp1 - mlib_sqrt(temp1 * temp1 - temp2 * temp2));
	sqbeta1 = beta * beta;
	sqbeta2 = sqbeta1 + sqbeta1;
	base = step = (n1 - 1) * sqbeta1;

	if (shift > 1023)
		coef2 = 0.0;
	else {
		dbl.value = coef1;
		dbl.two_int.int0 -= (shift << 20);
		coef2 = dbl.value;
	}

	window[0] = window2[0] = (mlib_f32)coef2;

	if (n <= 2)
		return (MLIB_SUCCESS);

	window++;
	window2--;
	n = n - 2;

	if (i0 == n2) {
		if ((mlib_addr)window & 7) {
			COUNT(result);
			window[0] = window2[0] = result;
			window++;
			window2--;
			n = n - 2;
			l++;
		}

		dwin1 = (mlib_d64 *)window;
		n2 = n2 - l - 1;

#pragma pipeloop(0)
		for (i = 0; i < n2 / 2; i++) {
			COUNT(temp_0);
			COUNT(temp_1);
			tmp0.f32x2.i0 = temp_0;
			tmp0.f32x2.i1 = temp_1;
			tmp2 = tmp0.d64;
			*dwin1 = tmp2;
			dwin1++;
			window2[0] = temp_0;
			window2[-1] = temp_1;
			window2 -= 2;
			window += 2;
		}

		for (i = 0; (n > 0) && (i <= (n2 & 1)); i++) {
			COUNT(result);
			window[0] = window2[0] = result;
			window++;
			window2--;
		}
	} else {

		for (i = 1; i <= i0; i++) {
			COUNT(result);
			window[0] = window2[0] = result;
			window++;
			window2--;
		}

		temp2 = mlib_sqrt(base);

		if ((mlib_addr)window & 7) {
			COUNT_RESULT(result0);
			window[0] = window2[0] = result0;
			window++;
			window2--;
			n = n - 2;
			l++;
		}

		i += l;
		n2 = n2 - i;
		dwin1 = (mlib_d64 *)window;

		if (!((mlib_addr)(window2 - 1) & 7)) {
			dwin2 = (mlib_d64 *)(window2 - 1);

/* #pragma pipeloop(0) */
#pragma unroll(1)	/* compiler bug */
			for (i = 0; i < n2 / 2; i++) {
				COUNT_RESULT(result0);
				COUNT_RESULT(result1);
				tmp0.f32x2.i0 = result0;
				tmp0.f32x2.i1 = result1;
				tmp2 = tmp0.d64;
				*dwin1 = tmp2;
				dwin1++;
				win2 = vis_bshuffle(tmp2, tmp2);
				*dwin2 = win2;
				dwin2--;
				window2 -= 2;
				window += 2;
			}

			for (i = 0; i <= (n2 & 1); i++) {
				COUNT_RESULT(result0);
				window[0] = window2[0] = result0;
				window++;
				window2--;
			}
		} else {

/* #pragma pipeloop(0) */
#pragma unroll(1)	/* compiler bug */
			for (i = 0; i < n2 / 2; i++) {
				COUNT_RESULT(result0);
				COUNT_RESULT(result1);
				tmp0.f32x2.i0 = result0;
				tmp0.f32x2.i1 = result1;
				tmp2 = tmp0.d64;
				*dwin1 = tmp2;
				dwin1++;
				window2[0] = result0;
				window2[-1] = result1;
				window2 -= 2;
				window += 2;
			}

			for (i = 0; i <= (n2 & 1); i++) {
				COUNT_RESULT(result0);
				window[0] = window2[0] = result0;
				window++;
				window2--;
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

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

/* *********************************************************** */
