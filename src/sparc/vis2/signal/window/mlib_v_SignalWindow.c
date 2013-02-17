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

#pragma ident	"@(#)mlib_v_SignalWindow.c	9.2	07/11/05 SMI"

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
#include <mlib_SignalWindow.h>
#include <mlib_SignalWindowUtil.h>
#include <mlib_SysMath.h>
#include <vis_proto.h>

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

#define	MLIB_SIGNAL_MASK	0x7FFFFFFF
#define	MLIB_HANNING_VAL	16383.5

/* *********************************************************** */

typedef union
{
	mlib_d64 d64;
	struct
	{
		mlib_s32 i0, i1;
	} s32x2;
} mlib_union64;

/* *********************************************************** */

#define	MLIB_BARTLETT                                           \
	for (i = 0; i < offset; ) {                              \
	    dw[i] = base >> 15;                                 \
	    i++;                                                \
	    base += step;                                       \
	}                                                       \
	                                                        \
	dw += offset;                                           \
	left -= offset

/* *********************************************************** */

#define	COUNT(result)                                                      \
	yv4 = base * base;                                                 \
	yv8 = yv4 * yv4;                                                   \
	yv16 = yv8 * yv8;                                                  \
	result =                                                           \
	    KA0 + KA3 * base * (KA1_KA3 + KA2_KA3 * base + yv4) +          \
	    KA7 * yv8 * (KA4_KA7 + KA5_KA7 * base + yv4 * (KA6_KA7 +       \
	    base)) + KA11 * yv16 * (KA8_KA11 + KA9_KA11 * base +           \
	    yv4 * (KA10_KA11 + base)) + KA15 * yv8 * yv16 * (KA12_KA15 +   \
	    KA13_KA15 * base + yv4 * (KA14_KA15 + base));                  \
	step -= sqbeta2;                                                   \
	base += step

/* *********************************************************** */

#define	COUNT_RESULT(result)                                      \
	t3 = (KP0 * temp2 + KP1) / (base + KQ2 + KQ1 * temp2);    \
	t1 = mlib_sqrt(temp2);                                    \
	t2 = K128ONLN2 * temp2;                                   \
	shift1 = (mlib_s32)t2;                                    \
	temp = mlib_constexp[shift1 & 0x7f];                      \
	t2 -= (mlib_d64)shift1;                                   \
	t2 = KE2 * temp * t2 * (t2 + KE1_2) + temp;               \
	shift1 >>= 7;                                             \
	step -= sqbeta2;                                          \
	base += step;                                             \
	temp2 = mlib_sqrt(base);                                  \
	temp = t1 * coef1 * (t2 * t3);                            \
	shift1 = shift - shift1;                                  \
	result = (shift1 > 30) ? 0 : ((mlib_s32)temp) >> shift1

/* *********************************************************** */

mlib_status
__mlib_SignalGenBartlett_S16(
    mlib_s16 *window,
    mlib_s32 n)
{
	mlib_s32 result, n2, n1, i;

/* check for obvious errors */

	if ((window == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	n1 = n - 1;
	n2 = (n1 >> 1);

	if (n <= 4096) {
		mlib_s32 step, base, n4, offset, left = n;
		mlib_s32 step2, step4;
		mlib_d64 v_step, v_base, v_base2;
		mlib_s16 *dw = window;

		n2++;
		base = 0;
		step = (MLIB_SIGNAL_MASK) / n1;
		step2 = step << 1;
		step4 = step2 << 1;
		vis_write_gsr(1 << 3);
		offset = ((mlib_addr)window) & 7;

		if (offset > 0) {
			offset = (8 - offset) >> 1;

			if (offset > (left - n2))
				offset = left - n2;
			MLIB_BARTLETT;
		}

		v_base = vis_to_double(base, base + step);
		v_step = vis_to_double_dup(step2);
		v_base2 = vis_fpadd32(v_base, v_step);
		v_step = vis_fpadd32(v_step, v_step);
		n4 = (left - n2) >> 2;

		for (i = 0; i < n4; i++) {
			*((mlib_d64 *)dw) = vis_fpackfix_pair(v_base, v_base2);
			v_base = vis_fpadd32(v_base, v_step);
			v_base2 = vis_fpadd32(v_base2, v_step);
			dw += 4;
			base += step4;
		}

		left -= n4 << 2;
		offset = left - n2;

		MLIB_BARTLETT;
		step = -step;

		step2 = step << 1;
		step4 = step2 << 1;

		if ((n & 1) == 0)
			base += step;
		offset = ((mlib_addr)dw) & 7;

		if (offset > 0) {
			offset = (8 - offset) >> 1;

			if (offset > left)
				offset = left;
			MLIB_BARTLETT;
		}

		v_base = vis_to_double(base, base + step);
		v_step = vis_to_double_dup(step2);
		v_base2 = vis_fpadd32(v_base, v_step);
		v_step = vis_fpadd32(v_step, v_step);
		n4 = left >> 2;

		for (i = 0; i < n4; i++) {
			*((mlib_d64 *)dw) = vis_fpackfix_pair(v_base, v_base2);
			v_base = vis_fpadd32(v_base, v_step);
			v_base2 = vis_fpadd32(v_base2, v_step);
			dw += 4;
			base += step4;
		}

		left &= 3;
		offset = left;

		MLIB_BARTLETT;
	} else {
		mlib_d64 step, base;

		base = 0;
		step = 2. / n1;
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
__mlib_SignalGenHanning_S16(
    mlib_s16 *window,
    mlib_s32 n)
{
	mlib_union64 tmp0, tmp1;
	mlib_d64 cbase0, cbase1, cbase2, cbase3, cbase4, cbase5, cbase6,
	    cbase7, cbase[3], win1, win2, win2_new, win2_last, win2_tmp;
	mlib_d64 temp0, temp1, temp2, temp3, cstep;
	mlib_s32 result0;
	mlib_s16 *window2 = window + n - 1;
	mlib_d64 tmp2, tmp3;
	mlib_s32 i, l, k = 0;
	mlib_s32 N = n, off, kol;
	mlib_d64 *dwin1, *dwin2;
	mlib_u8 emask;

	if ((window == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	vis_write_gsr(16 << 3);
	vis_write_bmask(0x67452301, 0);

	cstep = mlib_cos(2. * M_PI / (n - 1));
	cbase[0] = MLIB_HANNING_VAL;
	cbase[1] = cstep * MLIB_HANNING_VAL;
	cbase[2] = 2. * cstep * cbase[1] - cbase[0];

	cbase0 = cbase[0];
	cbase1 = cbase[1];

	if ((mlib_addr)window & 7) {
		for (l = 0; (n > 1) &&
		    ((mlib_addr)window & 7); n = n - 2, l++) {
			result0 = MLIB_HANNING_VAL - cbase[l];
			(*window++) = result0;
			*window2-- = result0;
		}

		cbase0 = MLIB_HANNING_VAL * mlib_cos(2 * M_PI / (N - 1) * (l));
		cbase1 = 2. * cstep * cbase0 - cbase[l - 1];
	}

	cbase2 = 2. * cstep * cbase1 - cbase0;
	cbase3 = 2. * cstep * cbase2 - cbase1;
	cbase4 = 2. * cstep * cbase3 - cbase2;
	cbase5 = 2. * cstep * cbase4 - cbase3;
	cbase6 = 2. * cstep * cbase5 - cbase4;
	cbase7 = 2. * cstep * cbase6 - cbase5;
	cstep = 16. * cstep * cstep * (cstep * cstep - 1) + 2.;

	if (!((mlib_addr)(window2 - 3) & 7)) {

		dwin1 = (mlib_d64 *)window;
		dwin2 = (mlib_d64 *)(window2 - 3);

#pragma pipeloop(0)
		for (i = 0; i < n / 8; i++) {
			tmp0.s32x2.i0 = MLIB_HANNING_VAL - cbase0;
			tmp0.s32x2.i1 = MLIB_HANNING_VAL - cbase1;
			tmp2 = tmp0.d64;

			tmp1.s32x2.i0 = MLIB_HANNING_VAL - cbase2;
			tmp1.s32x2.i1 = MLIB_HANNING_VAL - cbase3;
			tmp3 = tmp1.d64;

			win1 = vis_fpackfix_pair(tmp2, tmp3);
			*dwin1 = win1;
			dwin1++;
			win2 = vis_bshuffle(win1, win1);
			*dwin2 = win2;
			dwin2--;

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
	} else {

		if (n > 7) {

			tmp0.s32x2.i0 = MLIB_HANNING_VAL - cbase0;
			tmp0.s32x2.i1 = MLIB_HANNING_VAL - cbase1;
			tmp2 = tmp0.d64;

			tmp1.s32x2.i0 = MLIB_HANNING_VAL - cbase2;
			tmp1.s32x2.i1 = MLIB_HANNING_VAL - cbase3;
			tmp3 = tmp1.d64;

			dwin1 = (mlib_d64 *)window;
			win1 = vis_fpackfix_pair(tmp2, tmp3);
			*dwin1 = win1;
			dwin1++;

			kol =
			    window2 - (mlib_s16 *)((mlib_addr)window2 & (~7)) +
			    1;
			off = 4 - kol;
			dwin2 =
			    vis_alignaddr((void *)(window2 + 1),
			    (off - kol) * 2);
			emask = vis_edge16(dwin2, window2);
			win2_last = vis_bshuffle(win1, win1);
			win2_tmp = vis_faligndata(win2_last, win2_last);
			vis_pst_16(win2_tmp, dwin2, emask);
			dwin2--;

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

			n = n - (kol + 4);
			k = k + 4;
		}
#pragma pipeloop(0)
		for (i = 0; i < n / 8; i++) {
			tmp0.s32x2.i0 = MLIB_HANNING_VAL - cbase0;
			tmp0.s32x2.i1 = MLIB_HANNING_VAL - cbase1;
			tmp2 = tmp0.d64;

			tmp1.s32x2.i0 = MLIB_HANNING_VAL - cbase2;
			tmp1.s32x2.i1 = MLIB_HANNING_VAL - cbase3;
			tmp3 = tmp1.d64;

			win1 = vis_fpackfix_pair(tmp2, tmp3);
			*dwin1 = win1;
			dwin1++;
			win2 = vis_bshuffle(win1, win1);
			win2_new = vis_faligndata(win2, win2_last);
			*dwin2 = win2_new;
			dwin2--;
			win2_last = win2;

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
	}

	if (n & 7) {
		window += 4 * i + k;

		switch (n & 7) {
		case 7:
			window[6] = MLIB_HANNING_VAL - cbase6;
		case 6:
			window[5] = MLIB_HANNING_VAL - cbase5;
		case 5:
			window[4] = MLIB_HANNING_VAL - cbase4;
		case 4:
			window[3] = MLIB_HANNING_VAL - cbase3;
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
mlib_SignalGenHanning_S16_i(
    mlib_s16 *window,
    mlib_s16 *window2,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1)
{
	mlib_s16 *rwindow = window;
	mlib_union64 tmp0, tmp1;
	mlib_d64 cbase0, cbase1, cbase2, cbase3, cbase4, cbase5, cbase6,
	    cbase7, cbase[3], win1, win2, win2_new, win2_last, win2_tmp;
	mlib_d64 temp0, temp1, temp2, temp3, cstep, cstep0;
	mlib_s32 result0;
	mlib_d64 tmp2, tmp3;
	mlib_s32 i, l, k = 0;
	mlib_s32 N = n, off = 0, kol = 0, nm = (i1 - i0) * 4;
	mlib_d64 *dwin1, *dwin2;
	mlib_u8 emask;

	if ((window == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	vis_write_gsr(16 << 3);
	vis_write_bmask(0x67452301, 0);

	cstep0 = cstep = mlib_cos(2. * M_PI / (n - 1));
	cbase[0] = MLIB_HANNING_VAL *
		mlib_cos(2. * M_PI / (n - 1) * (2 * i0));
	cbase[1] = MLIB_HANNING_VAL *
		mlib_cos(2. * M_PI / (n - 1) * (2 * i0 + 1));
	cbase[2] = 2. * cstep * cbase[1] - cbase[0];

	cbase0 = cbase[0];
	cbase1 = cbase[1];

	window += i0 * 2;
	window2 -= i0 * 2;

	if ((mlib_addr)window & 7) {
		for (l = 0; (nm > 1) &&
		    ((mlib_addr)window & 7); nm = nm - 2, l++) {
			result0 = MLIB_HANNING_VAL - cbase[l];
			(*window++) = result0;
			*window2-- = result0;
		}

		cbase0 = MLIB_HANNING_VAL *
			mlib_cos(2 * M_PI / (N - 1) * (2 * i0 + l));
		cbase1 = 2. * cstep * cbase0 - cbase[l - 1];
	}

	cbase2 = 2. * cstep * cbase1 - cbase0;
	cbase3 = 2. * cstep * cbase2 - cbase1;
	cbase4 = 2. * cstep * cbase3 - cbase2;
	cbase5 = 2. * cstep * cbase4 - cbase3;
	cbase6 = 2. * cstep * cbase5 - cbase4;
	cbase7 = 2. * cstep * cbase6 - cbase5;
	cstep = 16. * cstep * cstep * (cstep * cstep - 1) + 2.;

	if (!((mlib_addr)(window2 - 3) & 7)) {
		dwin1 = (mlib_d64 *)window;
		dwin2 = (mlib_d64 *)(window2 - 3);

#pragma pipeloop(0)
		for (i = 0; i < nm / 8; i++) {
			tmp0.s32x2.i0 = MLIB_HANNING_VAL - cbase0;
			tmp0.s32x2.i1 = MLIB_HANNING_VAL - cbase1;
			tmp2 = tmp0.d64;

			tmp1.s32x2.i0 = MLIB_HANNING_VAL - cbase2;
			tmp1.s32x2.i1 = MLIB_HANNING_VAL - cbase3;
			tmp3 = tmp1.d64;

			win1 = vis_fpackfix_pair(tmp2, tmp3);
			*dwin1 = win1;
			dwin1++;
			win2 = vis_bshuffle(win1, win1);
			*dwin2 = win2;
			dwin2--;

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
	} else {
		if (nm > 7) {

			tmp0.s32x2.i0 = MLIB_HANNING_VAL - cbase0;
			tmp0.s32x2.i1 = MLIB_HANNING_VAL - cbase1;
			tmp2 = tmp0.d64;

			tmp1.s32x2.i0 = MLIB_HANNING_VAL - cbase2;
			tmp1.s32x2.i1 = MLIB_HANNING_VAL - cbase3;
			tmp3 = tmp1.d64;

			dwin1 = (mlib_d64 *)window;
			win1 = vis_fpackfix_pair(tmp2, tmp3);
			*dwin1 = win1;
			dwin1++;

			kol =
			    window2 - (mlib_s16 *)((mlib_addr)window2 & (~7)) +
			    1;
			off = 4 - kol;
			dwin2 =
			    vis_alignaddr((void *)(window2 + 1),
			    (off - kol) * 2);
			emask = vis_edge16(dwin2, window2);
			win2_last = vis_bshuffle(win1, win1);
			win2_tmp = vis_faligndata(win2_last, win2_last);
			vis_pst_16(win2_tmp, dwin2, emask);
			dwin2--;

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

			nm = nm - (kol + 4);
			k = k + 4;
		}
#pragma pipeloop(0)
		for (i = 0; i < nm / 8; i++) {
			tmp0.s32x2.i0 = MLIB_HANNING_VAL - cbase0;
			tmp0.s32x2.i1 = MLIB_HANNING_VAL - cbase1;
			tmp2 = tmp0.d64;

			tmp1.s32x2.i0 = MLIB_HANNING_VAL - cbase2;
			tmp1.s32x2.i1 = MLIB_HANNING_VAL - cbase3;
			tmp3 = tmp1.d64;

			win1 = vis_fpackfix_pair(tmp2, tmp3);
			*dwin1 = win1;
			dwin1++;
			win2 = vis_bshuffle(win1, win1);
			win2_new = vis_faligndata(win2, win2_last);
			*dwin2 = win2_new;
			dwin2--;
			win2_last = win2;

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

		if (off) {
			emask = vis_edge16((mlib_s16 *)dwin2 - off,
				(mlib_s16 *)dwin2 - 1);
			win2_tmp = vis_faligndata(win2_last, win2_last);
			vis_pst_16(win2_tmp, dwin2, emask);
		}
	}

	nm >>= 1;
	window += 4 * i + k;
	window2 -= 4 * i + k;

	if (nm & 3) {
		switch (nm & 3) {
		case 3:
			window2[-2] = window[2] = MLIB_HANNING_VAL - cbase2;
		case 2:
			window2[-1] = window[1] = MLIB_HANNING_VAL - cbase1;
		case 1:
			window2[0] = window[0] = MLIB_HANNING_VAL - cbase0;
		}
	}

	if ((n & 3) && i1 == n / 4) {
		window = rwindow + 2 * i1;
		cbase0 = MLIB_HANNING_VAL *
			mlib_cos(2. * M_PI / (n - 1) * (2 * i1));
		cbase1 = MLIB_HANNING_VAL *
			mlib_cos(2. * M_PI / (n - 1) * (2 * i1 + 1));
		cbase2 = 2. * cstep0 * cbase1 - cbase0;

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

/* *********************************************************** */

#define	MLIB_HAMMING_SUB	17694.18
#define	MLIB_HAMMING_MUL	15072.82

/* *********************************************************** */

mlib_status
__mlib_SignalGenHamming_S16(
    mlib_s16 *window,
    mlib_s32 n)
{
	mlib_union64 tmp0, tmp1;
	mlib_d64 cbase0, cbase1, cbase2, cbase3, cbase4, cbase5, cbase6,
	    cbase7, cbase[3], win1, win2, win2_new, win2_last, win2_tmp;
	mlib_d64 temp0, temp1, temp2, temp3, cstep;
	mlib_s32 result0;
	mlib_s16 *window2 = window + n - 1;
	mlib_d64 tmp2[1], tmp3[1];
	mlib_s32 i, l, k = 0;
	mlib_s32 N = n, off, kol;
	mlib_d64 *dwin1, *dwin2;
	mlib_u8 emask;

	if ((window == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	vis_write_gsr(16 << 3);
	vis_write_bmask(0x67452301, 0);

	cstep = mlib_cos(2. * M_PI / (n - 1));
	cbase[0] = MLIB_HAMMING_MUL;
	cbase[1] = cstep * MLIB_HAMMING_MUL;
	cbase[2] = 2. * cstep * cbase[1] - cbase[0];

	cbase0 = cbase[0];
	cbase1 = cbase[1];

	if ((mlib_addr)window & 7) {
		for (l = 0; (n > 1) &&
		    ((mlib_addr)window & 7); n = n - 2, l++) {
			result0 = MLIB_HAMMING_SUB - cbase[l];
			(*window++) = result0;
			*window2-- = result0;
		}

		cbase0 = MLIB_HAMMING_MUL * mlib_cos(2 * M_PI / (N - 1) * (l));
		cbase1 = 2. * cstep * cbase0 - cbase[l - 1];
	}

	cbase2 = 2. * cstep * cbase1 - cbase0;
	cbase3 = 2. * cstep * cbase2 - cbase1;
	cbase4 = 2. * cstep * cbase3 - cbase2;
	cbase5 = 2. * cstep * cbase4 - cbase3;
	cbase6 = 2. * cstep * cbase5 - cbase4;
	cbase7 = 2. * cstep * cbase6 - cbase5;
	cstep = 16. * cstep * cstep * (cstep * cstep - 1) + 2.;

	if (!((mlib_addr)(window2 - 3) & 7)) {

		dwin1 = (mlib_d64 *)window;
		dwin2 = (mlib_d64 *)(window2 - 3);

#pragma pipeloop(0)
		for (i = 0; i < n / 8; i++) {
			tmp0.s32x2.i0 = MLIB_HAMMING_SUB - cbase0;
			tmp0.s32x2.i1 = MLIB_HAMMING_SUB - cbase1;
			tmp2[0] = tmp0.d64;

			tmp1.s32x2.i0 = MLIB_HAMMING_SUB - cbase2;
			tmp1.s32x2.i1 = MLIB_HAMMING_SUB - cbase3;
			tmp3[0] = tmp1.d64;

			win1 = vis_fpackfix_pair(tmp2[0], tmp3[0]);
			*dwin1 = win1;
			dwin1++;
			win2 = vis_bshuffle(win1, win1);
			*dwin2 = win2;
			dwin2--;

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
	} else {

		if (n > 7) {

			tmp0.s32x2.i0 = MLIB_HAMMING_SUB - cbase0;
			tmp0.s32x2.i1 = MLIB_HAMMING_SUB - cbase1;
			tmp2[0] = tmp0.d64;

			tmp1.s32x2.i0 = MLIB_HAMMING_SUB - cbase2;
			tmp1.s32x2.i1 = MLIB_HAMMING_SUB - cbase3;
			tmp3[0] = tmp1.d64;

			dwin1 = (mlib_d64 *)window;
			win1 = vis_fpackfix_pair(tmp2[0], tmp3[0]);
			*dwin1 = win1;
			dwin1++;

			kol =
			    window2 - (mlib_s16 *)((mlib_addr)window2 & (~7)) +
			    1;
			off = 4 - kol;
			dwin2 =
			    vis_alignaddr((void *)(window2 + 1),
			    (off - kol) * 2);
			emask = vis_edge16(dwin2, window2);
			win2_last = vis_bshuffle(win1, win1);
			win2_tmp = vis_faligndata(win2_last, win2_last);
			vis_pst_16(win2_tmp, dwin2, emask);
			dwin2--;

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

			n = n - (kol + 4);
			k = k + 4;
		}
#pragma pipeloop(0)
		for (i = 0; i < n / 8; i++) {
			tmp0.s32x2.i0 = MLIB_HAMMING_SUB - cbase0;
			tmp0.s32x2.i1 = MLIB_HAMMING_SUB - cbase1;
			tmp2[0] = tmp0.d64;

			tmp1.s32x2.i0 = MLIB_HAMMING_SUB - cbase2;
			tmp1.s32x2.i1 = MLIB_HAMMING_SUB - cbase3;
			tmp3[0] = tmp1.d64;

			win1 = vis_fpackfix_pair(tmp2[0], tmp3[0]);
			*dwin1 = win1;
			dwin1++;
			win2 = vis_bshuffle(win1, win1);
			win2_new = vis_faligndata(win2, win2_last);
			*dwin2 = win2_new;
			dwin2--;
			win2_last = win2;

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
	}

	if (n & 7) {
		window += 4 * i + k;

		switch (n & 7) {
		case 7:
			window[6] = MLIB_HAMMING_SUB - cbase6;
		case 6:
			window[5] = MLIB_HAMMING_SUB - cbase5;
		case 5:
			window[4] = MLIB_HAMMING_SUB - cbase4;
		case 4:
			window[3] = MLIB_HAMMING_SUB - cbase3;
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
mlib_SignalGenHamming_S16_i(
    mlib_s16 *window,
    mlib_s16 *window2,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1)
{
	mlib_s16 *rwindow = window;
	mlib_union64 tmp0, tmp1;
	mlib_d64 cbase0, cbase1, cbase2, cbase3, cbase4, cbase5, cbase6,
	    cbase7, cbase[3], win1, win2, win2_new, win2_last, win2_tmp;
	mlib_d64 temp0, temp1, temp2, temp3, cstep, cstep0;
	mlib_s32 result0;
	mlib_d64 tmp2, tmp3;
	mlib_s32 i, l, k = 0;
	mlib_s32 N = n, off = 0, kol = 0, nm = (i1 - i0) * 4;
	mlib_d64 *dwin1, *dwin2;
	mlib_u8 emask;

	if ((window == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	vis_write_gsr(16 << 3);
	vis_write_bmask(0x67452301, 0);

	cstep0 = cstep = mlib_cos(2. * M_PI / (n - 1));
	cbase[0] = MLIB_HAMMING_MUL *
		mlib_cos(2. * M_PI / (n - 1) * (2 * i0));
	cbase[1] = MLIB_HAMMING_MUL *
		mlib_cos(2. * M_PI / (n - 1) * (2 * i0 + 1));
	cbase[2] = 2. * cstep * cbase[1] - cbase[0];

	cbase0 = cbase[0];
	cbase1 = cbase[1];

	window += i0 * 2;
	window2 -= i0 * 2;

	if ((mlib_addr)window & 7) {
		for (l = 0; (nm > 1) &&
		    ((mlib_addr)window & 7); nm = nm - 2, l++) {
			result0 = MLIB_HAMMING_SUB - cbase[l];
			(*window++) = result0;
			*window2-- = result0;
		}

		cbase0 = MLIB_HAMMING_MUL *
			mlib_cos(2 * M_PI / (N - 1) * (2 * i0 + l));
		cbase1 = 2. * cstep * cbase0 - cbase[l - 1];
	}

	cbase2 = 2. * cstep * cbase1 - cbase0;
	cbase3 = 2. * cstep * cbase2 - cbase1;
	cbase4 = 2. * cstep * cbase3 - cbase2;
	cbase5 = 2. * cstep * cbase4 - cbase3;
	cbase6 = 2. * cstep * cbase5 - cbase4;
	cbase7 = 2. * cstep * cbase6 - cbase5;
	cstep = 16. * cstep * cstep * (cstep * cstep - 1) + 2.;

	if (!((mlib_addr)(window2 - 3) & 7)) {
		dwin1 = (mlib_d64 *)window;
		dwin2 = (mlib_d64 *)(window2 - 3);

#pragma pipeloop(0)
		for (i = 0; i < nm / 8; i++) {
			tmp0.s32x2.i0 = MLIB_HAMMING_SUB - cbase0;
			tmp0.s32x2.i1 = MLIB_HAMMING_SUB - cbase1;
			tmp2 = tmp0.d64;

			tmp1.s32x2.i0 = MLIB_HAMMING_SUB - cbase2;
			tmp1.s32x2.i1 = MLIB_HAMMING_SUB - cbase3;
			tmp3 = tmp1.d64;

			win1 = vis_fpackfix_pair(tmp2, tmp3);
			*dwin1 = win1;
			dwin1++;
			win2 = vis_bshuffle(win1, win1);
			*dwin2 = win2;
			dwin2--;

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
	} else {
		if (nm > 7) {

			tmp0.s32x2.i0 = MLIB_HAMMING_SUB - cbase0;
			tmp0.s32x2.i1 = MLIB_HAMMING_SUB - cbase1;
			tmp2 = tmp0.d64;

			tmp1.s32x2.i0 = MLIB_HAMMING_SUB - cbase2;
			tmp1.s32x2.i1 = MLIB_HAMMING_SUB - cbase3;
			tmp3 = tmp1.d64;

			dwin1 = (mlib_d64 *)window;
			win1 = vis_fpackfix_pair(tmp2, tmp3);
			*dwin1 = win1;
			dwin1++;

			kol =
			    window2 - (mlib_s16 *)((mlib_addr)window2 & (~7)) +
			    1;
			off = 4 - kol;
			dwin2 =
			    vis_alignaddr((void *)(window2 + 1),
			    (off - kol) * 2);
			emask = vis_edge16(dwin2, window2);
			win2_last = vis_bshuffle(win1, win1);
			win2_tmp = vis_faligndata(win2_last, win2_last);
			vis_pst_16(win2_tmp, dwin2, emask);
			dwin2--;

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

			nm = nm - (kol + 4);
			k = k + 4;
		}
#pragma pipeloop(0)
		for (i = 0; i < nm / 8; i++) {
			tmp0.s32x2.i0 = MLIB_HAMMING_SUB - cbase0;
			tmp0.s32x2.i1 = MLIB_HAMMING_SUB - cbase1;
			tmp2 = tmp0.d64;

			tmp1.s32x2.i0 = MLIB_HAMMING_SUB - cbase2;
			tmp1.s32x2.i1 = MLIB_HAMMING_SUB - cbase3;
			tmp3 = tmp1.d64;

			win1 = vis_fpackfix_pair(tmp2, tmp3);
			*dwin1 = win1;
			dwin1++;
			win2 = vis_bshuffle(win1, win1);
			win2_new = vis_faligndata(win2, win2_last);
			*dwin2 = win2_new;
			dwin2--;
			win2_last = win2;

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

		if (off) {
			emask = vis_edge16((mlib_s16 *)dwin2 - off,
				(mlib_s16 *)dwin2 - 1);
			win2_tmp = vis_faligndata(win2_last, win2_last);
			vis_pst_16(win2_tmp, dwin2, emask);
		}
	}

	nm >>= 1;
	window += 4 * i + k;
	window2 -= 4 * i + k;

	if (nm & 3) {
		switch (nm & 3) {
		case 3:
			window2[-2] = window[2] = MLIB_HAMMING_SUB - cbase2;
		case 2:
			window2[-1] = window[1] = MLIB_HAMMING_SUB - cbase1;
		case 1:
			window2[0] = window[0] = MLIB_HAMMING_SUB - cbase0;
		}
	}

	if ((n & 3) && i1 == n / 4) {
		window = rwindow + 2 * i1;
		cbase0 = MLIB_HAMMING_MUL *
			mlib_cos(2. * M_PI / (n - 1) * (2 * i1));
		cbase1 = MLIB_HAMMING_MUL *
			mlib_cos(2. * M_PI / (n - 1) * (2 * i1 + 1));
		cbase2 = 2. * cstep0 * cbase1 - cbase0;

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

/* *********************************************************** */

#define	MLIB_BLACKMAN(X)	(coeff1 - (X) - coeff2 * (X) * (X))

/* *********************************************************** */

mlib_status
__mlib_SignalGenBlackman_S16(
    mlib_s16 *window,
    mlib_f32 alpha,
    mlib_s32 n)
{
	mlib_union64 tmp0, tmp1;
	mlib_d64 cbase0, cbase1, cbase2, cbase3, cbase4, cbase5, cbase6,
	    cbase7, cbase[3], win1, win2, win2_new, win2_last, win2_tmp;
	mlib_d64 temp0, temp1, temp2, temp3, cstep;
	mlib_s32 result0;
	mlib_s16 *window2 = window + n - 1;
	mlib_d64 tmp2[1], tmp3[1], coeff1, coeff2;
	mlib_s32 i, l, k = 0;
	mlib_s32 N = n, off, kol;
	mlib_d64 *dwin1, *dwin2;
	mlib_u8 emask;

	if ((window == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	if ((alpha <= -1.0) || (alpha >= 0.0))
		return (MLIB_OUTOFRANGE);

	vis_write_gsr(16 << 3);
	vis_write_bmask(0x67452301, 0);

	coeff1 = (.5 + alpha) * 32767;
	coeff2 = 2. * alpha / 16383.5;
	cstep = mlib_cos(2. * M_PI / (n - 1));
	cbase[0] = 16383.5;
	cbase[1] = 16383.5 * cstep;
	cbase[2] = 2. * cstep * cbase[1] - cbase[0];

	cbase0 = cbase[0];
	cbase1 = cbase[1];

	if ((mlib_addr)window & 7) {
		for (l = 0; (n > 1) &&
		    ((mlib_addr)window & 7); n = n - 2, l++) {
			result0 = MLIB_BLACKMAN(cbase[l]);
			(*window++) = result0;
			*window2-- = result0;
		}

		cbase0 = 16383.5 * mlib_cos(2 * M_PI / (N - 1) * (l));
		cbase1 = 2. * cstep * cbase0 - cbase[l - 1];
	}

	cbase2 = 2. * cstep * cbase1 - cbase0;
	cbase3 = 2. * cstep * cbase2 - cbase1;
	cbase4 = 2. * cstep * cbase3 - cbase2;
	cbase5 = 2. * cstep * cbase4 - cbase3;
	cbase6 = 2. * cstep * cbase5 - cbase4;
	cbase7 = 2. * cstep * cbase6 - cbase5;
	cstep = 16. * cstep * cstep * (cstep * cstep - 1) + 2.;

	if (!((mlib_addr)(window2 - 3) & 7)) {

		dwin1 = (mlib_d64 *)window;
		dwin2 = (mlib_d64 *)(window2 - 3);

#pragma pipeloop(0)
		for (i = 0; i < n / 8; i++) {
			tmp0.s32x2.i0 = MLIB_BLACKMAN(cbase0);
			tmp0.s32x2.i1 = MLIB_BLACKMAN(cbase1);
			tmp2[0] = tmp0.d64;

			tmp1.s32x2.i0 = MLIB_BLACKMAN(cbase2);
			tmp1.s32x2.i1 = MLIB_BLACKMAN(cbase3);
			tmp3[0] = tmp1.d64;

			win1 = vis_fpackfix_pair(tmp2[0], tmp3[0]);
			*dwin1 = win1;
			dwin1++;
			win2 = vis_bshuffle(win1, win1);
			*dwin2 = win2;
			dwin2--;

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
	} else {

		if (n > 7) {

			tmp0.s32x2.i0 = MLIB_BLACKMAN(cbase0);
			tmp0.s32x2.i1 = MLIB_BLACKMAN(cbase1);
			tmp2[0] = tmp0.d64;

			tmp1.s32x2.i0 = MLIB_BLACKMAN(cbase2);
			tmp1.s32x2.i1 = MLIB_BLACKMAN(cbase3);
			tmp3[0] = tmp1.d64;

			dwin1 = (mlib_d64 *)window;
			win1 = vis_fpackfix_pair(tmp2[0], tmp3[0]);
			*dwin1 = win1;
			dwin1++;

			kol =
			    window2 - (mlib_s16 *)((mlib_addr)window2 & (~7)) +
			    1;
			off = 4 - kol;
			dwin2 =
			    vis_alignaddr((void *)(window2 + 1),
			    (off - kol) * 2);
			emask = vis_edge16(dwin2, window2);
			win2_last = vis_bshuffle(win1, win1);
			win2_tmp = vis_faligndata(win2_last, win2_last);
			vis_pst_16(win2_tmp, dwin2, emask);
			dwin2--;

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

			n = n - (kol + 4);
			k = k + 4;
		}
#pragma pipeloop(0)
		for (i = 0; i < n / 8; i++) {
			tmp0.s32x2.i0 = MLIB_BLACKMAN(cbase0);
			tmp0.s32x2.i1 = MLIB_BLACKMAN(cbase1);
			tmp2[0] = tmp0.d64;

			tmp1.s32x2.i0 = MLIB_BLACKMAN(cbase2);
			tmp1.s32x2.i1 = MLIB_BLACKMAN(cbase3);
			tmp3[0] = tmp1.d64;

			win1 = vis_fpackfix_pair(tmp2[0], tmp3[0]);
			*dwin1 = win1;
			dwin1++;
			win2 = vis_bshuffle(win1, win1);
			win2_new = vis_faligndata(win2, win2_last);
			*dwin2 = win2_new;
			dwin2--;
			win2_last = win2;

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
	}

	if (n & 7) {
		window += 4 * i + k;

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
mlib_SignalGenBlackman_S16_i(
    mlib_s16 *window,
    mlib_s16 *window2,
    mlib_f32 alpha,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1)
{
	mlib_s16 *rwindow = window;
	mlib_union64 tmp0, tmp1;
	mlib_d64 cbase0, cbase1, cbase2, cbase3, cbase4, cbase5, cbase6,
	    cbase7, cbase[3], win1, win2, win2_new, win2_last, win2_tmp;
	mlib_d64 temp0, temp1, temp2, temp3, cstep, cstep0;
	mlib_s32 result0;
	mlib_d64 tmp2, tmp3, coeff1, coeff2;
	mlib_s32 i, l, k = 0;
	mlib_s32 N = n, off = 0, kol = 0, nm = (i1 - i0) * 4;
	mlib_d64 *dwin1, *dwin2;
	mlib_u8 emask;

	if ((window == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	if ((alpha <= -1.0) || (alpha >= 0.0))
		return (MLIB_OUTOFRANGE);

	vis_write_gsr(16 << 3);
	vis_write_bmask(0x67452301, 0);

	coeff1 = (.5 + alpha) * 32767;
	coeff2 = 2. * alpha / 16383.5;
	cstep0 = cstep = mlib_cos(2. * M_PI / (n - 1));
	cbase[0] = 16383.5 *
		mlib_cos(2. * M_PI / (n - 1) * (2 * i0));
	cbase[1] = 16383.5 *
		mlib_cos(2. * M_PI / (n - 1) * (2 * i0 + 1));
	cbase[2] = 2. * cstep * cbase[1] - cbase[0];

	cbase0 = cbase[0];
	cbase1 = cbase[1];

	window += i0 * 2;
	window2 -= i0 * 2;

	if ((mlib_addr)window & 7) {
		for (l = 0; (nm > 1) &&
		    ((mlib_addr)window & 7); nm = nm - 2, l++) {
			result0 = MLIB_BLACKMAN(cbase[l]);
			(*window++) = result0;
			*window2-- = result0;
		}

		cbase0 = 16383.5 *
			mlib_cos(2 * M_PI / (N - 1) * (2 * i0 + l));
		cbase1 = 2. * cstep * cbase0 - cbase[l - 1];
	}

	cbase2 = 2. * cstep * cbase1 - cbase0;
	cbase3 = 2. * cstep * cbase2 - cbase1;
	cbase4 = 2. * cstep * cbase3 - cbase2;
	cbase5 = 2. * cstep * cbase4 - cbase3;
	cbase6 = 2. * cstep * cbase5 - cbase4;
	cbase7 = 2. * cstep * cbase6 - cbase5;
	cstep = 16. * cstep * cstep * (cstep * cstep - 1) + 2.;

	if (!((mlib_addr)(window2 - 3) & 7)) {
		dwin1 = (mlib_d64 *)window;
		dwin2 = (mlib_d64 *)(window2 - 3);

#pragma pipeloop(0)
		for (i = 0; i < nm / 8; i++) {
			tmp0.s32x2.i0 = MLIB_BLACKMAN(cbase0);
			tmp0.s32x2.i1 = MLIB_BLACKMAN(cbase1);
			tmp2 = tmp0.d64;

			tmp1.s32x2.i0 = MLIB_BLACKMAN(cbase2);
			tmp1.s32x2.i1 = MLIB_BLACKMAN(cbase3);
			tmp3 = tmp1.d64;

			win1 = vis_fpackfix_pair(tmp2, tmp3);
			*dwin1 = win1;
			dwin1++;
			win2 = vis_bshuffle(win1, win1);
			*dwin2 = win2;
			dwin2--;

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
	} else {
		if (nm > 7) {

			tmp0.s32x2.i0 = MLIB_BLACKMAN(cbase0);
			tmp0.s32x2.i1 = MLIB_BLACKMAN(cbase1);
			tmp2 = tmp0.d64;

			tmp1.s32x2.i0 = MLIB_BLACKMAN(cbase2);
			tmp1.s32x2.i1 = MLIB_BLACKMAN(cbase3);
			tmp3 = tmp1.d64;

			dwin1 = (mlib_d64 *)window;
			win1 = vis_fpackfix_pair(tmp2, tmp3);
			*dwin1 = win1;
			dwin1++;

			kol =
			    window2 - (mlib_s16 *)((mlib_addr)window2 & (~7)) +
			    1;
			off = 4 - kol;
			dwin2 =
			    vis_alignaddr((void *)(window2 + 1),
			    (off - kol) * 2);
			emask = vis_edge16(dwin2, window2);
			win2_last = vis_bshuffle(win1, win1);
			win2_tmp = vis_faligndata(win2_last, win2_last);
			vis_pst_16(win2_tmp, dwin2, emask);
			dwin2--;

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

			nm = nm - (kol + 4);
			k = k + 4;
		}
#pragma pipeloop(0)
		for (i = 0; i < nm / 8; i++) {
			tmp0.s32x2.i0 = MLIB_BLACKMAN(cbase0);
			tmp0.s32x2.i1 = MLIB_BLACKMAN(cbase1);
			tmp2 = tmp0.d64;

			tmp1.s32x2.i0 = MLIB_BLACKMAN(cbase2);
			tmp1.s32x2.i1 = MLIB_BLACKMAN(cbase3);
			tmp3 = tmp1.d64;

			win1 = vis_fpackfix_pair(tmp2, tmp3);
			*dwin1 = win1;
			dwin1++;
			win2 = vis_bshuffle(win1, win1);
			win2_new = vis_faligndata(win2, win2_last);
			*dwin2 = win2_new;
			dwin2--;
			win2_last = win2;

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

		if (off) {
			emask = vis_edge16((mlib_s16 *)dwin2 - off,
				(mlib_s16 *)dwin2 - 1);
			win2_tmp = vis_faligndata(win2_last, win2_last);
			vis_pst_16(win2_tmp, dwin2, emask);
		}
	}

	nm >>= 1;
	window += 4 * i + k;
	window2 -= 4 * i + k;

	if (nm & 3) {
		switch (nm & 3) {
		case 3:
			window2[-2] = window[2] = MLIB_BLACKMAN(cbase2);
		case 2:
			window2[-1] = window[1] = MLIB_BLACKMAN(cbase1);
		case 1:
			window2[0] = window[0] = MLIB_BLACKMAN(cbase0);
		}
	}

	if ((n & 3) && i1 == n / 4) {
		window = rwindow + 2 * i1;
		cbase0 = 16383.5 *
			mlib_cos(2. * M_PI / (n - 1) * (2 * i1));
		cbase1 = 16383.5 *
			mlib_cos(2. * M_PI / (n - 1) * (2 * i1 + 1));
		cbase2 = 2. * cstep0 * cbase1 - cbase0;

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
__mlib_SignalGenKaiser_S16(
    mlib_s16 *window,
    mlib_f32 beta,
    mlib_s32 n)
{
	mlib_union64 tmp0, tmp1;
	mlib_d64 base, step, coef1, coef2, sqbeta, sqbeta2;
	mlib_d64 temp, temp_0, temp_1, temp_2, temp_3, temp2, t1;
	mlib_d64 t2, t3, tmp2, tmp3, win1, win2;
	mlib_d64 *dwin1, *dwin2;
	mlib_d64 yv4, yv8, yv16;
	type_union_mlib_d64 dbl;
	mlib_s32 n1, n2, i, i0, l = 0, result0, result1, result2, result3;
	mlib_s32 shift, shift1;
	mlib_s16 *window2 = window + n - 1;

/* check for obvious errors */

	if ((window == NULL) || (n <= 0) || (beta < 0))
		return (MLIB_FAILURE);

	vis_write_gsr(16 << 3);
	vis_write_bmask(0x67452301, 0);

	n1 = n - 1;
	n2 = (n1 >> 1);
	temp = n1 * 0.5;
	coef1 = 32767.5 / mlib_i0(beta * temp, &shift);
	temp2 = 15 / beta;
	i0 = (temp <
	    temp2) ? n2 : temp - mlib_sqrt(temp * temp - temp2 * temp2);
	sqbeta = beta * beta;
	sqbeta2 = sqbeta + sqbeta;
	base = step = (n1 - 1) * sqbeta;

	if (shift > 1023)
		coef2 = 0;
	else {
		dbl.value = coef1;
		dbl.two_int.int0 -= (shift << 20);
		coef2 = dbl.value;
	}

	window[0] = window2[0] = (mlib_s32)coef2;
	window++;
	window2--;
	n = n - 2;

	if (i0 == n2) {
		if ((mlib_addr)window & 7) {
			for (l = 0; (n > 0) && ((mlib_addr)window & 7);
			    n = n - 2, l++) {
				COUNT(temp);
				window[0] = window2[0] =
				    (mlib_s32)(coef2 * temp);
				window++;
				window2--;
			}
		}

		dwin1 = (mlib_d64 *)window;
		n2 = n2 - l - 1;

#pragma pipeloop(0)
		for (i = 0; i < n2 / 4; i++) {
			COUNT(temp_0);
			COUNT(temp_1);
			COUNT(temp_2);
			COUNT(temp_3);

			tmp0.s32x2.i0 = coef2 * temp_0;
			tmp0.s32x2.i1 = coef2 * temp_1;
			tmp2 = tmp0.d64;

			tmp1.s32x2.i0 = coef2 * temp_2;
			tmp1.s32x2.i1 = coef2 * temp_3;
			tmp3 = tmp1.d64;

			win1 = vis_fpackfix_pair(tmp2, tmp3);
			*dwin1 = win1;
			dwin1++;

			window2[0] = (mlib_s32)(coef2 * temp_0);
			window2[-1] = (mlib_s32)(coef2 * temp_1);
			window2[-2] = (mlib_s32)(coef2 * temp_2);
			window2[-3] = (mlib_s32)(coef2 * temp_3);
			window2 -= 4;
			window += 4;
		}

		for (i = 0; (n > 0) && (i <= (n2 & 3)); i++) {
			COUNT(temp);
			window[0] = window2[0] = (mlib_s32)(coef2 * temp);
			window++;
			window2--;
		}
	} else {

		for (i = 1; i <= i0; i++) {
			COUNT(temp);
			window[0] = window2[0] = (mlib_s32)(coef2 * temp);
			window++;
			window2--;
		}

		temp2 = mlib_sqrt(base);

		if ((mlib_addr)window & 7) {
			for (l = 0; (n > 0) && ((mlib_addr)window & 7);
			    n = n - 2, l++) {
				COUNT_RESULT(result0);
				window[0] = window2[0] = result0;
				window++;
				window2--;
			}
		}

		i += l;
		n2 = n2 - i;
		dwin1 = (mlib_d64 *)window;

		if (!((mlib_addr)(window2 - 3) & 7)) {
			dwin2 = (mlib_d64 *)(window2 - 3);

#pragma pipeloop(0)
			for (i = 0; i < n2 / 4; i++) {

				COUNT_RESULT(result0);
				COUNT_RESULT(result1);
				COUNT_RESULT(result2);
				COUNT_RESULT(result3);

				tmp0.s32x2.i0 = result0;
				tmp0.s32x2.i1 = result1;
				tmp2 = tmp0.d64;

				tmp1.s32x2.i0 = result2;
				tmp1.s32x2.i1 = result3;
				tmp3 = tmp1.d64;

				win1 = vis_fpackfix_pair(tmp2, tmp3);
				*dwin1 = win1;
				dwin1++;
				win2 = vis_bshuffle(win1, win1);
				*dwin2 = win2;
				dwin2--;
				window2 -= 4;
				window += 4;
			}

			for (i = 0; i <= (n2 & 3); i++) {
				COUNT_RESULT(result0);
				window[0] = window2[0] = result0;
				window++;
				window2--;
			}
		} else {

#pragma pipeloop(0)
			for (i = 0; i < n2 / 4; i++) {

				COUNT_RESULT(result0);
				COUNT_RESULT(result1);
				COUNT_RESULT(result2);
				COUNT_RESULT(result3);

				tmp0.s32x2.i0 = result0;
				tmp0.s32x2.i1 = result1;
				tmp2 = tmp0.d64;

				tmp1.s32x2.i0 = result2;
				tmp1.s32x2.i1 = result3;
				tmp3 = tmp1.d64;

				win1 = vis_fpackfix_pair(tmp2, tmp3);
				*dwin1 = win1;
				dwin1++;

				window2[0] = result0;
				window2[-1] = result1;
				window2[-2] = result2;
				window2[-3] = result3;
				window2 -= 4;
				window += 4;
			}

			for (i = 0; i <= (n2 & 3); i++) {
				COUNT_RESULT(result0);
				window[0] = window2[0] = result0;
				window++;
				window2--;
			}
		}
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_SignalGenKaiser_S16_i(
    mlib_s16 *window,
    mlib_f32 beta,
    mlib_s32 n,
    mlib_s32 istart,
    mlib_s32 iend)
{
	mlib_d64 base, step, coef1, coef2, sqbeta, sqbeta2;
	mlib_d64 temp, temp2, t1, t2, t3;
	type_union_mlib_d64 dbl;
	mlib_s32 n1, n2, i, i0, result;
	mlib_s32 shift, shift1;

/* check for obvious errors */

	if ((window == NULL) || (n <= 0) || (beta < 0))
		return (MLIB_FAILURE);

	n1 = n - 1;
	n2 = (n1 >> 1);
	temp = n1 * 0.5;
	coef1 = 32767.5 / mlib_i0(beta * temp, &shift);
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

	window[0] = window[n1] = (mlib_s32)coef2;

	for (i = istart; i <= ((iend < i0) ? iend : i0); i++) {
		mlib_d64 yv4, yv8, yv16;

		yv4 = base * base;
		yv8 = yv4 * yv4;
		yv16 = yv8 * yv8;
		temp =
		    KA0 + KA3 * base * (KA1_KA3 + KA2_KA3 * base + yv4) +
		    KA7 * yv8 * (KA4_KA7 + KA5_KA7 * base + yv4 * (KA6_KA7 +
		    base)) + KA11 * yv16 * (KA8_KA11 + KA9_KA11 * base +
		    yv4 * (KA10_KA11 + base)) + KA15 * yv8 * yv16 * (KA12_KA15 +
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
