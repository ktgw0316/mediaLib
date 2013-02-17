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

#pragma ident	"@(#)mlib_SignalLPCAutoCorrel_Fp.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalLPCAutoCorrel_F32     - linear predictive coding with
 *                                         autocorrelation method
 *
 * SYNOPSIS
 *      void mlib_SignalLPCAutoCorrelFree_F32(void *state);
 *      mlib_status mlib_SignalLPCAutoCorrelGetEnergy_F32(mlib_f32 *energy,
 *                                                        void     *state);
 *      mlib_status mlib_SignalLPCAutoCorrelGetPARCOR_F32(mlib_f32 *parcor,
 *                                                        void     *state);
 *      mlib_status mlib_SignalLPCAutoCorrelInit_F32(void     **state,
 *                                                   mlib_s32 length,
 *                                                   mlib_s32 order);
 *      mlib_status mlib_SignalLPCAutoCorrel_F32(mlib_f32       *coeff,
 *                                               const mlib_f32 *signal,
 *                                               void           *state);
 * ARGUMENTS
 *      coeff   the linear prediction coefficients.
 *      signal  the input signal vector.
 *      energy  the energy of the input signal.
 *      parcor  the partial correlation (PARCOR) coefficients.
 *      order   the order of the linear prediction filter.
 *      length  the length of the input signal vector.
 *      state   the internal state structure.
 *
 * RETURN VALUE
 *      MLIB_SUCCESS is returned if the LPC coefficients are evaluated
 *      successfully. MLIB_FAILURE is returned for ill-defined filter
 *      or when error is encountered.
 *
 * RESTRICTIONS
 *      The init functions should only perform internal structure
 *      allocation and global initialization. Per LPC function call
 *      initialization should be done in LPC function, so the
 *      same internal structure can be reused for multiple LPC
 *      function calls.
 *
 * DESCRIPTION
 *      In linear predictive coding (LPC) model, each speech sample
 *      is prepresented as a linear combination of the past M samples.
 *
 *                      M
 *              s(n) = SUM a(i) * s(n-i) + G * u(n)
 *                     i = 1
 *
 *      where s(*) is the speech signal, u(*) is the excitation signal,
 *      and G is the gain constants, M is the order of the linear
 *      prediction filter. Given s(*), the goal is to find a set of
 *      coefficient a(*) that minimizes the prediction error e(*).
 *
 *                             M
 *              e(n) = s(n) - SUM a(i) * s(n-i)
 *                            i = 1
 *
 *      In autocorrelation method, the coefficients can be obtained by
 *      solving following set of linear equations.
 *
 *               M
 *              SUM a(i) * r(|i-k|) = r(k), k = 1, ..., M
 *              i = 1
 *
 *      where
 *
 *                    N-k-1
 *              r(k) = SUM s(j) * s(j+k)
 *                     j = 0
 *
 *      are the autocorrelation coefficients of s(*), N is the length
 *      of the input speech vector. r(0) is the energy of the speech
 *      signal.
 *
 *      Note that the autocorrelation matrix R is a Toeplitz matrix
 *      (symmetric with all diagonal elements equal), and the equations
 *      can be solved efficiently with Levinson-Durbin algorithm.
 *
 *      See "Fundamentals of Speech Recognition" by Lawrence Rabiner
 *      and Biing-Hwang Juang, Prentice Hall, 1993.
 *
 *      Note that the buffer length for linear prediction coefficients
 *      must be order+1, while coeff[0] element is not initialized and
 *      not used.
 */

#include <mlib_signal.h>
#include <mlib_SignalLPC.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalLPCAutoCorrelFree_F32 = \
	__mlib_SignalLPCAutoCorrelFree_F32
#pragma weak mlib_SignalLPCAutoCorrelGetEnergy_F32 = \
	__mlib_SignalLPCAutoCorrelGetEnergy_F32
#pragma weak mlib_SignalLPCAutoCorrelGetPARCOR_F32 = \
	__mlib_SignalLPCAutoCorrelGetPARCOR_F32
#pragma weak mlib_SignalLPCAutoCorrelInit_F32 = \
	__mlib_SignalLPCAutoCorrelInit_F32
#pragma weak mlib_SignalLPCAutoCorrel_F32 = __mlib_SignalLPCAutoCorrel_F32
#pragma weak mlib_SignalLPCAutoCorrel_F32_cor = \
	__mlib_SignalLPCAutoCorrel_F32_cor

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalLPCAutoCorrelFree_F32) mlib_SignalLPCAutoCorrelFree_F32
    __attribute__((weak, alias("__mlib_SignalLPCAutoCorrelFree_F32")));
__typeof__(__mlib_SignalLPCAutoCorrelGetEnergy_F32)
    mlib_SignalLPCAutoCorrelGetEnergy_F32
    __attribute__((weak, alias("__mlib_SignalLPCAutoCorrelGetEnergy_F32")));
__typeof__(__mlib_SignalLPCAutoCorrelGetPARCOR_F32)
    mlib_SignalLPCAutoCorrelGetPARCOR_F32
    __attribute__((weak, alias("__mlib_SignalLPCAutoCorrelGetPARCOR_F32")));
__typeof__(__mlib_SignalLPCAutoCorrelInit_F32) mlib_SignalLPCAutoCorrelInit_F32
    __attribute__((weak, alias("__mlib_SignalLPCAutoCorrelInit_F32")));
__typeof__(__mlib_SignalLPCAutoCorrel_F32) mlib_SignalLPCAutoCorrel_F32
    __attribute__((weak, alias("__mlib_SignalLPCAutoCorrel_F32")));
__typeof__(__mlib_SignalLPCAutoCorrel_F32_cor) mlib_SignalLPCAutoCorrel_F32_cor
    __attribute__((weak, alias("__mlib_SignalLPCAutoCorrel_F32_cor")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

typedef struct
{
	mlib_s32 order;
	mlib_s32 length;
/* m + 1 partial correlation (PARCOR) coefficients */
	mlib_f32 *parcor;
/* energy of the signal Err[1] */
	mlib_f32 *Err;
/* buffer for correlations */
	mlib_f32 *cor_buf;
/* buffer for coeff's */
	mlib_f32 *lpc_coef_buf;
} mlib_lpc_state;

/* *********************************************************** */

void
__mlib_SignalLPCAutoCorrel_F32_cor(
	mlib_f32 *cor,
	const mlib_f32 *signal,
	mlib_s32 order,
	mlib_s32 length)
{
	__mlib_SignalLPCAutoCorrel_F32_cor_i(cor, signal, length,
		0, order + 1);
}

void __mlib_SignalLPCAutoCorrel_F32_cor_i(
	mlib_f32 *cor,
	const mlib_f32 *signal,
	mlib_s32 length,
	mlib_s32 i0,
	mlib_s32 i1)
{
	mlib_s32 i, j1, j2;
	mlib_f32 x0, x1, x2, x3, x4, x5, x6, x7, x8;
	mlib_f32 y0, y1, y2, y3, y4, y5, y6, y7, y8;
	mlib_f32 z0, z1, z2, z3, z4, z5, z6, z7;

/* Autocorrelation */

	for (i = i0; i < i1 - 7; i += 8) {
		x0 = signal[0];
		x1 = signal[1];
		x2 = signal[2];
		x3 = signal[3];
		x4 = signal[4];
		x5 = signal[5];
		x6 = signal[6];
		x7 = signal[7];

		y0 = signal[i + 0];
		y1 = signal[i + 1];
		y2 = signal[i + 2];
		y3 = signal[i + 3];
		y4 = signal[i + 4];
		y5 = signal[i + 5];
		y6 = signal[i + 6];
		y7 = signal[i + 7];

		z0 = y0 * x0 + y1 * x1 + y2 * x2 + y3 * x3 + y4 * x4 + y5 * x5 +
		    y6 * x6 + y7 * x7;
		z1 = y1 * x0 + y2 * x1 + y3 * x2 + y4 * x3 + y5 * x4 + y6 * x5 +
		    y7 * x6;
		z2 = y2 * x0 + y3 * x1 + y4 * x2 + y5 * x3 + y6 * x4 + y7 * x5;
		z3 = y3 * x0 + y4 * x1 + y5 * x2 + y6 * x3 + y7 * x4;
		z4 = y4 * x0 + y5 * x1 + y6 * x2 + y7 * x3;
		z5 = y5 * x0 + y6 * x1 + y7 * x2;
		z6 = y6 * x0 + y7 * x1;
		z7 = y7 * x0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j1 = 8, j2 = i + 8; j2 < length; j1++, j2++) {
			x8 = signal[j1];
			y8 = signal[j2];
			z0 += y8 * x8;
			z1 += y8 * x7;
			z2 += y8 * x6;
			z3 += y8 * x5;
			z4 += y8 * x4;
			z5 += y8 * x3;
			z6 += y8 * x2;
			z7 += y8 * x1;

			x1 = x2;
			x2 = x3;
			x3 = x4;
			x4 = x5;
			x5 = x6;
			x6 = x7;
			x7 = x8;

			y1 = y2;
			y2 = y3;
			y3 = y4;
			y4 = y5;
			y5 = y6;
			y6 = y7;
			y7 = y8;
		}

		cor[i + 0] = z0;
		cor[i + 1] = z1;
		cor[i + 2] = z2;
		cor[i + 3] = z3;
		cor[i + 4] = z4;
		cor[i + 5] = z5;
		cor[i + 6] = z6;
		cor[i + 7] = z7;
	}

	if (i < i1 - 3) {
		x0 = signal[0];
		x1 = signal[1];
		x2 = signal[2];
		x3 = signal[3];

		y0 = signal[i + 0];
		y1 = signal[i + 1];
		y2 = signal[i + 2];
		y3 = signal[i + 3];

		z0 = y0 * x0 + y1 * x1 + y2 * x2 + y3 * x3;
		z1 = y1 * x0 + y2 * x1 + y3 * x2;
		z2 = y2 * x0 + y3 * x1;
		z3 = y3 * x0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j1 = 4, j2 = i + 4; j2 < length; j1++, j2++) {
			x4 = signal[j1];
			y4 = signal[j2];
			z0 += y4 * x4;
			z1 += y4 * x3;
			z2 += y4 * x2;
			z3 += y4 * x1;

			x1 = x2;
			x2 = x3;
			x3 = x4;
			y1 = y2;
			y2 = y3;
			y3 = y4;
		}

		cor[i + 0] = z0;
		cor[i + 1] = z1;
		cor[i + 2] = z2;
		cor[i + 3] = z3;
		i += 4;
	}

	if (i < i1 - 1) {
		x0 = signal[0];
		x1 = signal[1];
		y0 = signal[i + 0];
		y1 = signal[i + 1];

		z0 = y0 * x0 + y1 * x1;
		z1 = y1 * x0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j1 = 2, j2 = i + 2; j2 < length; j1++, j2++) {
			x2 = signal[j1];
			y2 = signal[j2];
			z0 += y2 * x2;
			z1 += y2 * x1;

			x1 = x2;
			y1 = y2;
		}

		cor[i + 0] = z0;
		cor[i + 1] = z1;
		i += 2;
	}

	if (i < i1) {
		x0 = signal[0];
		y0 = signal[i];
		z0 = y0 * x0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j1 = 1, j2 = i + 1; j2 < length; j1++, j2++) {
			x1 = signal[j1];
			y1 = signal[j2];
			z0 += y1 * x1;
		}

		cor[i + 0] = z0;
	}

}

mlib_status
__mlib_SignalLPCAutoCorrel_F32(
    mlib_f32 *coeff,
    const mlib_f32 *signal,
    void *state)
{
	mlib_lpc_state *lpc = state;
	mlib_s32 order = lpc->order;
	mlib_s32 length = lpc->length;
	mlib_f32 *reflect = lpc->parcor;
	mlib_s32 i, j;
	mlib_f32 *cor = lpc->cor_buf, *Err = lpc->Err;
	mlib_f32 error, r, *lpc_coef = lpc->lpc_coef_buf;

	if ((state == NULL) || (signal == NULL) || (coeff == NULL))
		return (MLIB_FAILURE);

	if (order + 1 > length)
		return (MLIB_FAILURE);

	mlib_SignalLPCAutoCorrel_F32_cor(cor, signal, order, length);

/* Checking for obvious error */

	if (cor[0] == 0)
		return (MLIB_FAILURE);

/* Levinson-Durbin algorithm */
	error = cor[0];
	for (i = 0; i < order; i++) {
/* Sum up this iteration's reflection coefficient */

		r = cor[i + 1];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < i; j++)
			r = r - lpc_coef[j] * cor[i - j];
		r = r / error;
		reflect[i + 1] = r;

/* Update LPC coefficients and total error */
		lpc_coef[i] = r;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < i / 2; j++) {
			mlib_f32 tmp0 = lpc_coef[j];
			mlib_f32 tmp1 = lpc_coef[i - 1 - j];

			lpc_coef[j] = tmp0 - r * tmp1;
			lpc_coef[i - 1 - j] = tmp1 - r * tmp0;
		}

		if (i & 1) {
			mlib_f32 tmp0 = lpc_coef[j];

			lpc_coef[j] = tmp0 - tmp0 * r;
		}

		error *= 1 - r * r;

		if (error <= 0)
			return (MLIB_FAILURE);
	}

/* storing test result coeffs */
/* coeff[0] = 1; */
	Err[0] = error;

	coeff++;

	if ((((mlib_addr)lpc_coef ^ (mlib_addr)coeff) & 7) == 0) {
		mlib_d64 *lpc_ptr;
		mlib_d64 *coeff_ptr;

		i = 0;

		if (((mlib_addr)coeff & 7) != 0) {
			(*coeff++) = (*lpc_coef++);
			i++;
		}

		lpc_ptr = (mlib_d64 *)lpc_coef;
		coeff_ptr = (mlib_d64 *)coeff;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < order - 1; i += 2)
			(*coeff_ptr++) = (*lpc_ptr++);

		if (i < order)
			*((mlib_f32 *)coeff_ptr) = *((mlib_f32 *)lpc_ptr);
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < order - 1; i += 2) {
			coeff[i] = lpc_coef[i];
			coeff[i + 1] = lpc_coef[i + 1];
		}

		if (i < order)
			coeff[i] = lpc_coef[i];
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLPCAutoCorrelInit_F32(
    void **state,
    mlib_s32 length,
    mlib_s32 order)
{
	mlib_lpc_state *state_ptr;
	mlib_s32 i, struct_size;
	mlib_f32 *parcor_ptr, *Err_ptr;

/* check for obvious errors */

	if (state == NULL)
		return (MLIB_NULLPOINTER);

	if (order < 1)
		return (MLIB_FAILURE);

	if (length <= 0)
		return (MLIB_FAILURE);

/* to be multiple of double */
	struct_size = (sizeof (mlib_lpc_state) + 7) & ~7;

	state_ptr =
	    (mlib_lpc_state *) __mlib_malloc(struct_size + sizeof (mlib_f32) +
	    (order + 1) * 3 * sizeof (mlib_f32));

	if (state_ptr == NULL)
		return (MLIB_NULLPOINTER);

	state_ptr->order = order;
	state_ptr->length = length;
	state_ptr->parcor = (mlib_f32 *)((mlib_u8 *)state_ptr + struct_size);
	state_ptr->Err = (state_ptr->parcor) + (order + 1);
	state_ptr->cor_buf = (state_ptr->Err) + 1;
	state_ptr->lpc_coef_buf = (state_ptr->cor_buf) + (order + 1);

	parcor_ptr = state_ptr->parcor;
	Err_ptr = state_ptr->Err;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= order; i++) {
		parcor_ptr[i] = 0;
	}

	Err_ptr[0] = 0;

	*state = state_ptr;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
__mlib_SignalLPCAutoCorrelFree_F32(
    void *state)
{
/* check for obvious errors */

	if (state == NULL)
		return; /* MLIB_FAILURE */

	__mlib_free(state);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLPCAutoCorrelGetEnergy_F32(
    mlib_f32 *energy,
    void *state)
{
	mlib_f32 *en;
	mlib_lpc_state *lpc = state;

/* check for obvious errors */

	if ((state == NULL) || (energy == NULL))
		return (MLIB_FAILURE);

	en = lpc->Err;

	if (en == 0)
		return (MLIB_FAILURE);

	energy[0] = en[0];

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLPCAutoCorrelGetPARCOR_F32(
    mlib_f32 *parcor,
    void *state)
{
	mlib_lpc_state *lpc = state;
	mlib_f32 *par = lpc->parcor;
	mlib_s32 i, order = lpc->order;

/* check for obvious errors */

	if ((state == NULL) || (parcor == NULL))
		return (MLIB_FAILURE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 1; i <= order; i++) {
		parcor[i] = par[i];
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
