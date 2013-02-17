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

#pragma ident	"@(#)mlib_SignalLPCCovariance.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalLPCCovariance_S16     - linear predictive coding with
 *                                         covariance method
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalLPCCovarianceInit_S16(void     **state,
 *                                                   mlib_s32 length,
 *                                                   mlib_s32 order);
 *      mlib_status mlib_SignalLPCCovariance_S16(mlib_s16       *coeff,
 *                                               mlib_s32       cscale,
 *                                               const mlib_s16 *signal,
 *                                               void           *state);
 *      mlib_status mlib_SignalLPCCovariance_S16_Adp(mlib_s16       *coeff,
 *                                                   mlib_s32       *cscale,
 *                                                   const mlib_s16 *signal,
 *                                                   void           *state);
 *      void mlib_SignalLPCCovarianceFree_S16(void *state);
 *
 * ARGUMENTS
 *      coeff   the linear prediction coefficients.
 *      cscale  the scaling factor of the linear prediction coefficients,
 *              where actual_data = output_data * 2^(-scaling_factor).
 *      signal  the input signal vector, the signal
 *              samples are in Q15 format.
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
 *      In covariance method, the coefficients can be obtained by
 *      solving following set of linear equations.
 *
 *               M
 *              SUM a(i) * c(i, k) = c(0, k), k = 1, ..., M
 *              i = 1
 *
 *      where
 *
 *                       N-k-1
 *              c(i, k) =  SUM s(j) * s(j+k-i)
 *                        j = 0
 *
 *      are the covariance coefficients of s(*), N is the length
 *      of the input speech vector.
 *
 *      Note that the covariance matrix R is a symmetric matrix,
 *      and the equations  can be solved efficiently with Cholesky
 *      decomposition method.
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

#include <mlib_signal.h>
#include <mlib_SysMath.h>
#include <mlib_SignalLPC.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalLPCCovarianceFree_S16 = \
	__mlib_SignalLPCCovarianceFree_S16
#pragma weak mlib_SignalLPCCovarianceInit_S16 = \
	__mlib_SignalLPCCovarianceInit_S16
#pragma weak mlib_SignalLPCCovariance_S16 = __mlib_SignalLPCCovariance_S16
#pragma weak mlib_SignalLPCCovariance_S16_Adp = \
	__mlib_SignalLPCCovariance_S16_Adp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalLPCCovarianceFree_S16) mlib_SignalLPCCovarianceFree_S16
    __attribute__((weak, alias("__mlib_SignalLPCCovarianceFree_S16")));
__typeof__(__mlib_SignalLPCCovarianceInit_S16) mlib_SignalLPCCovarianceInit_S16
    __attribute__((weak, alias("__mlib_SignalLPCCovarianceInit_S16")));
__typeof__(__mlib_SignalLPCCovariance_S16) mlib_SignalLPCCovariance_S16
    __attribute__((weak, alias("__mlib_SignalLPCCovariance_S16")));
__typeof__(__mlib_SignalLPCCovariance_S16_Adp) mlib_SignalLPCCovariance_S16_Adp
    __attribute__((weak, alias("__mlib_SignalLPCCovariance_S16_Adp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SET_FLOAT_ORDER(FLT, ODR)                               \
	* (mlib_s32 *)&(FLT) = (0x7F + (ODR)) << 23

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING
#define	SAT_16(tmp)	((mlib_s32)((tmp) * 65536.f) >> 16)
#else /* MLIB_USE_FTOI_CLAMPING */
#define	SAT_16(tmp)                                             \
	((tmp) < MLIB_S16_MIN ? MLIB_S16_MIN : (tmp) >          \
	    MLIB_S16_MAX ? MLIB_S16_MAX : (tmp))
#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

typedef struct
{
	void *mem;
	mlib_s32 order;
	mlib_s32 length;
	mlib_s32 offset;
	mlib_s32 wrapflag;
	void *signal_fp;
	void *coeff_fp;
} mlib_LPC;

/* *********************************************************** */

mlib_status
__mlib_SignalLPCCovarianceInit_S16(
    void **state,
    mlib_s32 length,
    mlib_s32 order)
{
	mlib_u32 size_str, size_sig, size_coeff, size_mem;
	mlib_LPC *lpc_state;

	if (length <= 0)
		return (MLIB_FAILURE);

	if (order <= 0)
		return (MLIB_FAILURE);

	size_mem = (order + 1) * sizeof (mlib_f32);
	size_mem *= size_mem;

	size_sig = length * sizeof (mlib_f32);
	size_coeff = (order + 2) * sizeof (mlib_f32);

	size_str = (sizeof (mlib_LPC) + 7) & ~7;

	lpc_state = __mlib_malloc(size_mem + size_str + size_sig + size_coeff);

	if (!lpc_state)
		return (MLIB_NULLPOINTER);

	lpc_state->offset = order + 1;
	lpc_state->order = order;
	lpc_state->length = length;
	lpc_state->mem = (mlib_u8 *)lpc_state + size_str;
	lpc_state->signal_fp = (mlib_u8 *)lpc_state->mem + size_mem;
	lpc_state->coeff_fp = (mlib_u8 *)lpc_state->signal_fp + size_sig;

	lpc_state->wrapflag = 0;

	*state = lpc_state;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLPCCovariance_S16(
    mlib_s16 *coeff,
    mlib_s32 cscale,
    const mlib_s16 *signal,
    void *state)
{
	mlib_s32 i;
	mlib_LPC *lpc_state = state;
	mlib_f32 *coeff_fp = lpc_state->coeff_fp;
	mlib_f32 *signal_fp = lpc_state->signal_fp;
	mlib_s32 length = lpc_state->length;
	mlib_s32 order = lpc_state->order;
	mlib_f32 scale, tmp;
	mlib_status res;

	if (lpc_state->wrapflag) {
/*
 * Special case for Java wrappers.
 */

		for (i = 0; i < length; i++) {
			signal_fp[i] = signal[i];
		}
	} else {
		signal_fp += ((mlib_addr)signal & 3) >> 1;
		mlib_SignalLPCCov_S162F32(signal_fp, signal, length);
	}

	res = __mlib_SignalLPCCovariance_F32(coeff_fp, signal_fp, state);

	if (res != MLIB_SUCCESS)
		return (res);

	SET_FLOAT_ORDER(scale, cscale);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 1; i <= order; i++) {
		tmp = coeff_fp[i] * scale;
		tmp = tmp > 0 ? tmp + 0.5 : tmp - 0.5;
		coeff[i] = SAT_16(tmp);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLPCCovariance_S16_Adp(
    mlib_s16 *coeff,
    mlib_s32 *cscale,
    const mlib_s16 *signal,
    void *state)
{
	mlib_s32 i, itmp;
	mlib_LPC *lpc_state = state;
	mlib_f32 *coeff_fp = lpc_state->coeff_fp;
	mlib_f32 *signal_fp = lpc_state->signal_fp;
	mlib_s32 length = lpc_state->length;
	mlib_s32 order = lpc_state->order;
	mlib_f32 scale, fmax, tmp;
	mlib_status res;

	if (lpc_state->wrapflag) {
/*
 * Special case for Java wrappers.
 */

		for (i = 0; i < length; i++) {
			signal_fp[i] = signal[i];
		}
	} else {
		signal_fp += ((mlib_addr)signal & 3) >> 1;
		mlib_SignalLPCCov_S162F32(signal_fp, signal, length);
	}

	res = __mlib_SignalLPCCovariance_F32(coeff_fp, signal_fp, state);

	if (res != MLIB_SUCCESS)
		return (res);

	fmax = mlib_fabsf(coeff_fp[1]);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 2; i <= order; i++) {
		if (mlib_fabsf(coeff_fp[i]) > fmax)
			fmax = mlib_fabsf(coeff_fp[i]);
	}

	itmp = 14 - mlib_ilogb(fmax);
	SET_FLOAT_ORDER(scale, itmp);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 1; i <= order; i++) {
		tmp = coeff_fp[i] * scale;
		tmp = tmp > 0 ? tmp + 0.5 : tmp - 0.5;
		coeff[i] = tmp;
	}

	*cscale = itmp;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
__mlib_SignalLPCCovarianceFree_S16(
    void *state)
{
	__mlib_free(state);
}

/* *********************************************************** */
