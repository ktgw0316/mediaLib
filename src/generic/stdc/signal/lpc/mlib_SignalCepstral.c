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

#pragma ident	"@(#)mlib_SignalCepstral.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *     mlib_SignalCepstral_S16  - cepstral analysis
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalCepstralInit_S16(void **state,
 *                                              mlib_s32 order);
 *      mlib_status mlib_SignalCepstral_S16(mlib_s16 *cepst,
 *                                          mlib_s32 cscale,
 *                                          const mlib_s16 *signal,
 *                                          void *state);
 *      mlib_status mlib_SignalCepstral_S16_Adp(mlib_s16 *cepst,
 *                                              mlib_s32 *cscale,
 *                                              const mlib_s16 *signal,
 *                                              void *state);
 *      void mlib_SignalCepstralFree_S16(void *state);
 *
 * ARGUMENTS
 *      cepst   the cepstral coefficients.
 *      cscale  the scaling factor of the cepstral coefficients,
 *              where actual_data = output_data * 2^(-scaling_factor).
 *      signal  the input signal vector, the signal samples
 *              are in Q15 format.
 *      order   the order of the input signal vector and the cepstral
 *              coefficients, where length = 2^order.
 *      state   the internal state structure.
 *
 * RETURN VALUE
 *      MLIB_SUCCESS is returned if the cepstral analysis is successful,
 *      MLIB_FAILURE is returned when error is encountered.
 *
 * RESTRICTIONS
 *      The init functions should only perform internal structure
 *      allocation and global initialization. Per function call
 *      initialization should be done in function, so the
 *      same internal structure can be reused for multiple
 *      function calls.
 *
 * DESCRIPTION
 *      The basic operations to compute the cepstrum is shown below.
 *
 *                    +-----------+      +--------+       +-----------+
 *                    |  Fourier  |      |        |       |  Inverse  |
 *              ----->|           |----->| log|*| |------>|  Fourier  |----->
 *               x(n) | Transform | X(k) |        | X'(k) | Transform | c(n)
 *                    +-----------+      +--------+       +-----------+
 *
 *      where x(n) is the input signal and c(n) is its cepstrum.
 *      In mathematics, they are
 *
 *                     N-1                2*PI*k*n
 *              X(k) = SUM x(n) * exp(-j*----------)
 *                     n = 0                   N
 *
 *              X'(k) = log|X(k)|
 *
 *                      1  N-1                2*PI*k*n
 *              c(k) = --- SUM X'(n) * exp(j*----------)
 *                      N  n = 0                   N
 *
 *      Since X'(k) is real and even (symmetric), i.e.
 *
 *              X'(k) = X'(N - k)
 *
 *      the c(n) is real and the the equation becomes Cosine transform.
 *
 *                      1  N-1              2*PI*k*n
 *              c(k) = --- SUM X'(n) * cos(----------)
 *                      N  n = 0                 N
 *
 *      The cepstral coefficients in LPC is a special case of the above.
 *
 *      See "Digital Signal Processing" by Alan V. Oppenheim and
 *      Ronald W. Schafer, Prentice Hall, 1974.
 *
 *      See "Fundamentals of Speech Recognition" by Lawrence Rabiner
 *      and Biing-Hwang Juang, Prentice Hall, 1993.
 */

#include <mlib_SignalCepstral.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalCepstralFree_S16 = __mlib_SignalCepstralFree_S16
#pragma weak mlib_SignalCepstralInit_S16 = __mlib_SignalCepstralInit_S16
#pragma weak mlib_SignalCepstral_S16 = __mlib_SignalCepstral_S16
#pragma weak mlib_SignalCepstral_S16_Adp = __mlib_SignalCepstral_S16_Adp
#pragma weak mlib_SignalCepstral_S16_Middle = __mlib_SignalCepstral_S16_Middle

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalCepstralFree_S16) mlib_SignalCepstralFree_S16
    __attribute__((weak, alias("__mlib_SignalCepstralFree_S16")));
__typeof__(__mlib_SignalCepstralInit_S16) mlib_SignalCepstralInit_S16
    __attribute__((weak, alias("__mlib_SignalCepstralInit_S16")));
__typeof__(__mlib_SignalCepstral_S16) mlib_SignalCepstral_S16
    __attribute__((weak, alias("__mlib_SignalCepstral_S16")));
__typeof__(__mlib_SignalCepstral_S16_Adp) mlib_SignalCepstral_S16_Adp
    __attribute__((weak, alias("__mlib_SignalCepstral_S16_Adp")));
__typeof__(__mlib_SignalCepstral_S16_Middle) mlib_SignalCepstral_S16_Middle
    __attribute__((weak, alias("__mlib_SignalCepstral_S16_Middle")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_SignalCepstralInit_S16(
    void **state,
    mlib_s32 order)
{
	return (__mlib_SignalCepstralInit_F32(state, order));
}

/* *********************************************************** */

void
__mlib_SignalCepstral_S16_Middle(
	deal_t *buffer,
	deal_t *coeff,
	deal_t *buffer1,
	deal_t *coeff_s,
	mlib_s32 i0,
	mlib_s32 i1)
{
	deal_t tmp;
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = i0; i < i1; i++) {
		tmp = buffer[2 * i + 0] * buffer[2 * i + 0];
		tmp += buffer[2 * i + 1] * buffer[2 * i + 1];
		buffer1[i] = tmp;
	}

	mlib_vlog(coeff + i0, buffer1 + i0, i1 - i0);
}

mlib_status
__mlib_SignalCepstral_S16(
    mlib_s16 *cepst,
    mlib_s32 cscale,
    const mlib_s16 *signal,
    void *state)
{
	mlib_s32 i, len;
	mlib_f32 fscale;
	mlib_cepstral *str_cepst = state;
	mlib_s32 order = str_cepst->order;
	deal_t *buffer = str_cepst->buffer;
	deal_t *coeff = str_cepst->coeff;
	deal_t *coeff_s = str_cepst->coeff_s;
	mlib_s32 len_full = 1 << order;
	deal_t tmp, scale = (deal_t)1 / len_full;

#if REAL_IFFT_NEEDWASTE
	len = len_full;
#else /* REAL_IFFT_NEEDWASTE */
	len = len_full / 2 + 1;
#endif /* REAL_IFFT_NEEDWASTE */

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < len_full; i++)
		buffer[i] = signal[i] / (deal_t)32768;

	mlib_fft_r2c(buffer, order);

	mlib_SignalCepstral_S16_Middle(buffer, coeff, buffer, coeff_s, 0, len);

	mlib_fft_r2r(coeff, order - 1);

	SET_FLOAT_ORDER(fscale, cscale);
	scale *= fscale;

#if ICOS_FFT_NEEDWASTE
#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < len_full; i++)
		cepst[i] = SAT_16(coeff[i] * scale);
#else /* ICOS_FFT_NEEDWASTE */
	cepst[0] = SAT_16(coeff[0] * scale);
	cepst[len] = SAT_16(coeff[len] * scale);

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 1; i < len; i++) {
		tmp = coeff[i] * scale;
		cepst[i] = cepst[len_full - i] = SAT_16(tmp);
	}

#endif /* ICOS_FFT_NEEDWASTE */

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalCepstral_S16_Adp(
    mlib_s16 *cepst,
    mlib_s32 *cscale,
    const mlib_s16 *signal,
    void *state)
{
	mlib_s32 i, len, itmp;
	mlib_f32 fscale;
	mlib_cepstral *str_cepst = state;
	mlib_s32 order = str_cepst->order;
	deal_t *buffer = str_cepst->buffer;
	deal_t *coeff = str_cepst->coeff;
	deal_t *coeff_s = str_cepst->coeff_s;
	mlib_s32 len_full = 1 << order;
	deal_t tmp, fmax, scale;

#if REAL_IFFT_NEEDWASTE
	len = len_full;
#else /* REAL_IFFT_NEEDWASTE */
	len = len_full / 2 + 1;
#endif /* REAL_IFFT_NEEDWASTE */

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < len_full; i++)
		buffer[i] = signal[i] / (deal_t)32768;

	mlib_fft_r2c(buffer, order);

	mlib_SignalCepstral_S16_Middle(buffer, coeff, buffer, coeff_s, 0, len);

	mlib_fft_r2r(coeff, order - 1);

	fmax = 0;
#ifdef __SUNPRO_C
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < len; i++) {
		tmp = mlib_fabsf(coeff[i]);

		if (tmp > fmax)
			fmax = tmp;
	}

	itmp = 14 - mlib_ilogb(fmax);
	SET_FLOAT_ORDER(fscale, itmp);
	scale = fscale;

#if ICOS_FFT_NEEDWASTE
#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < len_full; i++)
		cepst[i] = coeff[i] * scale;
#else /* ICOS_FFT_NEEDWASTE */
	cepst[0] = coeff[0] * scale;
	cepst[len] = coeff[len] * scale;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 1; i < len; i++) {
		cepst[i] = cepst[len_full - i] = coeff[i] * scale;
	}

#endif /* ICOS_FFT_NEEDWASTE */

	*cscale = itmp + order;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
__mlib_SignalCepstralFree_S16(
    void *state)
{
	__mlib_SignalCepstralFree_F32(state);
}

/* *********************************************************** */
