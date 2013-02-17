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

#pragma ident	"@(#)mlib_c_SignalLPCAutoCorrel.c	9.5	07/10/09 SMI"

/*
 * FUNCTIONS
 *     mlib_SignalLPCAutoCorrel_S16     - linear predictive coding with
 *                                        autocorrelation method
 *
 * SYNOPSIS
 *      void mlib_SignalLPCAutoCorrelFree_S16(void *state);
 *      mlib_status mlib_SignalLPCAutoCorrelGetEnergy_S16(mlib_s16 *energy,
 *                                                        mlib_s32 escale,
 *                                                        void *state);
 *      mlib_status mlib_SignalLPCAutoCorrelGetEnergy_S16_Adp(mlib_s16 *energy,
 *                                                            mlib_s32 *escale,
 *                                                            void *state);
 *      mlib_status mlib_SignalLPCAutoCorrelGetPARCOR_S16(mlib_s16 *parcor,
 *                                                        mlib_s32 pscale,
 *                                                        void *state);
 *      mlib_status mlib_SignalLPCAutoCorrelGetPARCOR_S16_Adp(mlib_s16 *parcor,
 *                                                            mlib_s32 *pscale,
 *                                                            void *state);
 *      mlib_status mlib_SignalLPCAutoCorrelInit_S16(void **state,
 *                                                   mlib_s32 length,
 *                                                   mlib_s32 order);
 *      mlib_status mlib_SignalLPCAutoCorrel_S16(mlib_s16 *coeff,
 *                                               mlib_s32 cscale,
 *                                               const mlib_s16 *signal,
 *                                               void *state);
 *      mlib_status mlib_SignalLPCAutoCorrel_S16_Adp(mlib_s16 *coeff,
 *                                                   mlib_s32 *cscale,
 *                                                   const mlib_s16 *signal,
 *                                                   void *state);
 * ARGUMENTS
 *      coeff   the linear prediction coefficients.
 *      cscale  the scaling factor of the linear prediction coefficients,
 *              where actual_data = output_data * 2^(-scaling_factor).
 *      signal  the input signal vector, the signal
 *              samples are in Q15 format.
 *      energy  the energy of the input signal.
 *      escale  the scaling factor of the energy of the input signal,
 *              where actual_data = output_data * 2^(-scaling_factor).
 *      parcor  the partial correlation (PARCOR) coefficients.
 *      pscale  the scaling factor of the PARCOR coefficients,
 *              where actual_data = output_data * 2^(-scaling_factor).
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
 *      Note for functions with adaptive scaling (with _Adp postfix),
 *      the scaling factor of the output data will be calculated based
 *      on the actual data; for functions with non-adaptive scaling
 *      (without _Adp postfix), the user supplied scaling factor will
 *      be used and the output will be saturated if necessary.
 *
 *      Note that the buffer length for linear prediction coefficients
 *      must be order+1, while coeff[0] element is not initialized and
 *      not used.
 */

#include <stdlib.h>
#include <mlib_signal.h>
#include <mlib_SysMath.h>
#include <mlib_SignalLPC.h>
/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalLPCAutoCorrelFree_S16 = \
	__mlib_SignalLPCAutoCorrelFree_S16
#pragma weak mlib_SignalLPCAutoCorrelGetEnergy_S16 = \
	__mlib_SignalLPCAutoCorrelGetEnergy_S16
#pragma weak mlib_SignalLPCAutoCorrelGetEnergy_S16_Adp = \
	__mlib_SignalLPCAutoCorrelGetEnergy_S16_Adp
#pragma weak mlib_SignalLPCAutoCorrelGetPARCOR_S16 = \
	__mlib_SignalLPCAutoCorrelGetPARCOR_S16
#pragma weak mlib_SignalLPCAutoCorrelGetPARCOR_S16_Adp = \
	__mlib_SignalLPCAutoCorrelGetPARCOR_S16_Adp
#pragma weak mlib_SignalLPCAutoCorrelInit_S16 = \
	__mlib_SignalLPCAutoCorrelInit_S16
#pragma weak mlib_SignalLPCAutoCorrel_S16 = __mlib_SignalLPCAutoCorrel_S16
#pragma weak mlib_SignalLPCAutoCorrel_S16_Adp = \
	__mlib_SignalLPCAutoCorrel_S16_Adp
#pragma weak mlib_SignalLPCAutoCorrel_S16_cor = \
	__mlib_SignalLPCAutoCorrel_S16_cor

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalLPCAutoCorrelFree_S16) mlib_SignalLPCAutoCorrelFree_S16
    __attribute__((weak, alias("__mlib_SignalLPCAutoCorrelFree_S16")));
__typeof__(__mlib_SignalLPCAutoCorrelGetEnergy_S16)
    mlib_SignalLPCAutoCorrelGetEnergy_S16
    __attribute__((weak, alias("__mlib_SignalLPCAutoCorrelGetEnergy_S16")));
__typeof__(__mlib_SignalLPCAutoCorrelGetEnergy_S16_Adp)
    mlib_SignalLPCAutoCorrelGetEnergy_S16_Adp
    __attribute__((weak,
    alias("__mlib_SignalLPCAutoCorrelGetEnergy_S16_Adp")));
__typeof__(__mlib_SignalLPCAutoCorrelGetPARCOR_S16)
    mlib_SignalLPCAutoCorrelGetPARCOR_S16
    __attribute__((weak, alias("__mlib_SignalLPCAutoCorrelGetPARCOR_S16")));
__typeof__(__mlib_SignalLPCAutoCorrelGetPARCOR_S16_Adp)
    mlib_SignalLPCAutoCorrelGetPARCOR_S16_Adp
    __attribute__((weak,
    alias("__mlib_SignalLPCAutoCorrelGetPARCOR_S16_Adp")));
__typeof__(__mlib_SignalLPCAutoCorrelInit_S16) mlib_SignalLPCAutoCorrelInit_S16
    __attribute__((weak, alias("__mlib_SignalLPCAutoCorrelInit_S16")));
__typeof__(__mlib_SignalLPCAutoCorrel_S16) mlib_SignalLPCAutoCorrel_S16
    __attribute__((weak, alias("__mlib_SignalLPCAutoCorrel_S16")));
__typeof__(__mlib_SignalLPCAutoCorrel_S16_Adp) mlib_SignalLPCAutoCorrel_S16_Adp
    __attribute__((weak, alias("__mlib_SignalLPCAutoCorrel_S16_Adp")));
__typeof__(__mlib_SignalLPCAutoCorrel_S16_cor) mlib_SignalLPCAutoCorrel_S16_cor
    __attribute__((weak, alias("__mlib_SignalLPCAutoCorrel_S16_cor")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	SAT_S16(val)	((mlib_s16)((mlib_s32)(val * 65536) >> 16))

#else /* MLIB_USE_FTOI_CLAMPING */

#define	SAT_S16(val)					\
	(((val) > MLIB_S16_MAX) ?			\
		MLIB_S16_MAX :				\
		(((val) < (MLIB_S16_MIN + 1)) ?		\
			MLIB_S16_MIN :			\
			(((val) < 0.0) ?		\
				(mlib_s16)(val - 1) :	\
				(mlib_s16)(val))))

#endif /* MLIB_USE_FTOI_CLAMPING */

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
/* buffer for s16 to f32 conversion */
	mlib_f32 *convert_buf;
} mlib_lpc_state;

/* *********************************************************** */

/* power[i] = (mlib_f32) 2 ^ i */

static const mlib_f32 power[] = {
	1, 2, 4, 8, 16, 32, 64, 128,
	256, 512, 1024, 2048, 4096,
	8192, 16384, 32768, 65536
};

/* *********************************************************** */

void
__mlib_SignalLPCAutoCorrel_S16_cor(
	void *cor,
	const void *signal,
	mlib_s32 order,
	mlib_s32 length)
{
	__mlib_SignalLPCAutoCorrel_F32_cor_i(cor, signal, length,
		0, order + 1);
}

void __mlib_SignalLPCAutoCorrel_S16_cor_i(
	void *cor,
	const void *signal,
	mlib_s32 length,
	mlib_s32 i0,
	mlib_s32 i1)
{
	__mlib_SignalLPCAutoCorrel_F32_cor_i(cor, signal, length,
		i0, i1);
}

mlib_status
__mlib_SignalLPCAutoCorrel_S16(
    mlib_s16 *coeff,
    mlib_s32 cscale,
    const mlib_s16 *signal,
    void *state)
{
	mlib_lpc_state *lpc = state;
	mlib_s32 order = lpc->order;
	mlib_s32 length = lpc->length;
	mlib_f32 *reflect = lpc->parcor;
	mlib_s32 i, j;
	mlib_f32 *cor = lpc->cor_buf, *Err = lpc->Err;
	mlib_f32 error, r, *lpc_coef = lpc->lpc_coef_buf;
	mlib_f32 scale = 1, mul = power[16];
	mlib_f32 *short2float = lpc->convert_buf;
	mlib_s32 *psig, ld;
	mlib_f32 val0, val1;

	if ((state == NULL) || (signal == NULL) || (coeff == NULL))
		return (MLIB_FAILURE);

	i = 0;

	if ((mlib_addr)signal & 3) {
		short2float[i] = signal[i] << 16;
		i++;
	}

	psig = (mlib_s32 *)(signal + i);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < length - 1; i += 2) {
		ld = (*psig++);
#ifdef _LITTLE_ENDIAN
		short2float[i] = ld << 16;
		short2float[i + 1] = ld & ~0xFFFF;
#else /* _LITTLE_ENDIAN */
		short2float[i] = ld & ~0xFFFF;
		short2float[i + 1] = ld << 16;
#endif /* _LITTLE_ENDIAN */
	}

	if (i < length)
		short2float[i] = *((mlib_s16 *)psig) << 16;

/* Autocorrelation */
	mlib_SignalLPCAutoCorrel_S16_cor(cor, short2float, order, length);

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

	for (i = abs(cscale); i > 15; i -= 16)
		scale *= mul;

	scale *= power[i];

	if (cscale < 0)
		scale = 1 / scale;

/* storing test result coeffs */
	Err[0] = error;
/* coeff[0] = SAT_S16(1 * scale + .5); */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 1; i < order; i += 2) {
		val0 = lpc_coef[i - 1] * scale;
		val1 = lpc_coef[i] * scale;
		val0 = (val0 > 0) ? (val0 + .5) : (val0 - .5);
		val1 = (val1 > 0) ? (val1 + .5) : (val1 - .5);
		coeff[i] = SAT_S16(val0);
		coeff[i + 1] = SAT_S16(val1);
	}

	if (i <= order) {
		val0 = lpc_coef[i - 1] * scale;
		val0 = (val0 > 0) ? (val0 + .5) : (val0 - .5);
		coeff[i] = SAT_S16(val0);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLPCAutoCorrel_S16_Adp(
    mlib_s16 *coeff,
    mlib_s32 *cscale,
    const mlib_s16 *signal,
    void *state)
{
	mlib_lpc_state *lpc = state;
	mlib_s32 order = lpc->order;
	mlib_s32 length = lpc->length;
	mlib_f32 *reflect = lpc->parcor;
	mlib_s32 i, j, sfactor;
	mlib_f32 *cor = lpc->cor_buf, *Err = lpc->Err;
	mlib_f32 error, r, *lpc_coef = lpc->lpc_coef_buf;
	mlib_f32 scale = 1, mul = power[16];
	mlib_f32 *short2float = lpc->convert_buf, max_coef;
	mlib_s32 *psig, ld, exp2;

	if ((state == NULL) || (signal == NULL) || (coeff == NULL))
		return (MLIB_FAILURE);

	i = 0;

	if ((mlib_addr)signal & 3) {
		short2float[i] = signal[i] << 16;
		i++;
	}

	psig = (mlib_s32 *)(signal + i);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < length - 1; i += 2) {
		ld = (*psig++);
#ifdef _LITTLE_ENDIAN
		short2float[i] = ld << 16;
		short2float[i + 1] = ld & ~0xFFFF;
#else /* _LITTLE_ENDIAN */
		short2float[i] = ld & ~0xFFFF;
		short2float[i + 1] = ld << 16;
#endif /* _LITTLE_ENDIAN */
	}

	if (i < length)
		short2float[i] = *((mlib_s16 *)psig) << 16;

/* Autocorrelation */
	mlib_SignalLPCAutoCorrel_S16_cor(cor, short2float, order, length);

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

	Err[0] = error;

/* max_coef = 1; */
	max_coef = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 1; i <= order; i++) {
		mlib_f32 mod = mlib_fabsf(lpc_coef[i - 1]);

		max_coef = (mod > max_coef) ? mod : max_coef;
	}

	exp2 = mlib_ilogb(max_coef);

	sfactor = 14 - exp2;
	cscale[0] = sfactor;

	for (i = abs(sfactor); i > 15; i -= 16)
		scale *= mul;

	scale *= power[i];

	if (sfactor < 0)
		scale = 1 / scale;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 1; i <= order; i++) {
		mlib_f32 val = lpc_coef[i - 1] * scale;

		val = (val > 0) ? (val + .5) : (val - .5);
		coeff[i] = (mlib_s16)val;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLPCAutoCorrelInit_S16(
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

/* to be multiple of mlib_d64 */
	struct_size = (sizeof (mlib_lpc_state) + 7) & ~7;

	state_ptr =
	    (mlib_lpc_state *) __mlib_malloc(struct_size + sizeof (mlib_f32) +
	    (order + 1) * 3 * sizeof (mlib_f32) +
	    (length + 1) * sizeof (mlib_f32));

	if (state_ptr == NULL)
		return (MLIB_NULLPOINTER);

	state_ptr->order = order;
	state_ptr->length = length;
	state_ptr->parcor = (mlib_f32 *)((mlib_u8 *)state_ptr + struct_size);
	state_ptr->Err = (state_ptr->parcor) + (order + 1);
	state_ptr->cor_buf = (state_ptr->Err) + 1;
	state_ptr->lpc_coef_buf = (state_ptr->cor_buf) + (order + 1);
	state_ptr->convert_buf = (state_ptr->lpc_coef_buf) + (order + 1);

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
__mlib_SignalLPCAutoCorrelFree_S16(
    void *state)
{
/* check for obvious errors */

	if (state == NULL)
		return; /* MLIB_FAILURE */

	__mlib_free(state);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLPCAutoCorrelGetEnergy_S16(
    mlib_s16 *energy,
    mlib_s32 escale,
    void *state)
{
/* 2^16 * 2^16 * 2^15 * 2^15 */
	mlib_f32 *en, error, div = 4611686018427387904;
	mlib_lpc_state *lpc = state;
	mlib_f32 scale = 1, mul = power[16];
	mlib_s32 i;

/* check for obvious errors */

	if ((state == NULL) || (energy == NULL))
		return (MLIB_FAILURE);

	en = lpc->Err;

	if (en == 0)
		return (MLIB_FAILURE);

	error = en[0];

	for (i = abs(escale); i > 15; i -= 16)
		scale *= mul;

	scale *= power[i];

	if (escale < 0)
		scale = 1 / scale;

	error = error / div * scale;
	error = (error > 0) ? (error + .5) : (error - .5);

	energy[0] = SAT_S16(error);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLPCAutoCorrelGetEnergy_S16_Adp(
    mlib_s16 *energy,
    mlib_s32 *escale,
    void *state)
{
/* 2^16 * 2^16 * 2^15 * 2^15 */
	mlib_f32 *en, error, div = 4611686018427387904;
	mlib_lpc_state *lpc = state;
	mlib_f32 scale = 1, mul = power[16];
	mlib_s32 i, exp2, sfactor;

/* check for obvious errors */

	if ((state == NULL) || (energy == NULL))
		return (MLIB_FAILURE);

	en = lpc->Err;

	if (en == 0)
		return (MLIB_FAILURE);

	error = en[0];
	error /= div;

	exp2 = mlib_ilogb(error);
	sfactor = 14 - exp2;

	escale[0] = sfactor;

	for (i = abs(sfactor); i > 15; i -= 16)
		scale *= mul;

	scale *= power[i];

	if (sfactor < 0)
		scale = 1 / scale;

	error = error * scale;
	error = (error > 0) ? (error + .5) : (error - .5);
	energy[0] = (mlib_s16)(error);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLPCAutoCorrelGetPARCOR_S16(
    mlib_s16 *parcor,
    mlib_s32 pscale,
    void *state)
{
	mlib_lpc_state *lpc = state;
	mlib_f32 *par = lpc->parcor, val;
	mlib_s32 i, order = lpc->order;
	mlib_f32 scale = 1, mul = power[16];

/* check for obvious errors */

	if ((state == NULL) || (parcor == NULL))
		return (MLIB_FAILURE);

	for (i = abs(pscale); i > 15; i -= 16)
		scale *= mul;

	scale *= power[i];

	if (pscale < 0)
		scale = 1 / scale;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 1; i <= order; i++) {
		val = par[i] * scale;
		val = (val > 0) ? (val + .5) : (val - .5);
		parcor[i] = SAT_S16(val);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLPCAutoCorrelGetPARCOR_S16_Adp(
    mlib_s16 *parcor,
    mlib_s32 *pscale,
    void *state)
{
	mlib_lpc_state *lpc = state;
	mlib_f32 *par = lpc->parcor, max_par, val;
	mlib_s32 i, order = lpc->order, sfactor, exp2;
	mlib_f32 scale = 1, mul = power[16];

/* check for obvious errors */

	if ((state == NULL) || (parcor == NULL))
		return (MLIB_FAILURE);

	max_par = mlib_fabsf(par[1]);
	for (i = 2; i <= order; i++) {
		mlib_f32 mod = mlib_fabsf(par[i]);

		max_par = (mod > max_par) ? mod : max_par;
	}

	exp2 = mlib_ilogb(max_par);

	sfactor = 14 - exp2;
	pscale[0] = sfactor;

	for (i = abs(sfactor); i > 15; i -= 16)
		scale *= mul;

	scale *= power[i];

	if (sfactor < 0)
		scale = 1 / scale;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 1; i <= order; i++) {
		val = par[i] * scale;
		parcor[i] =
		    (val > 0) ? (mlib_s16)(val + .5) : (mlib_s16)(val - .5);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
