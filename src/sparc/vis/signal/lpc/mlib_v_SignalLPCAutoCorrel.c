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

#pragma ident	"@(#)mlib_v_SignalLPCAutoCorrel.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *     mlib_SignalLPCAutoCorrel_S16     - linear predictive coding with
 *                                        autocorrelation method
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalLPCAutoCorrelGetEnergy_S16(mlib_s16 *energy,
 *                                                        mlib_s32 escale,
 *                                                        void     *state);
 *      mlib_status
 *      mlib_SignalLPCAutoCorrelGetEnergy_S16_Adp(mlib_s16       *energy,
 *                                                const mlib_s32 *escale,
 *                                                void           *state);
 *      mlib_status
 *      mlib_SignalLPCAutoCorrelGetPARCOR_S16(mlib_s16   *parcor,
 *                                            mlib_s32   pscale,
 *                                            const void *state);
 *      mlib_status
 *      mlib_SignalLPCAutoCorrelGetPARCOR_S16_Adp(mlib_s16 *parcor,
 *                                                mlib_s32 *pscale,
 *                                                void     *state);
 *      mlib_status mlib_SignalLPCAutoCorrelInit_S16(void     **state,
 *                                                   mlib_s32 length,
 *                                                   mlib_s32 order);
 *      mlib_status mlib_SignalLPCAutoCorrel_S16(mlib_s16       *coeff,
 *                                               mlib_s32       cscale,
 *                                               const mlib_s16 *signal,
 *                                               void           *state);
 *      mlib_status mlib_SignalLPCAutoCorrel_S16_Adp(mlib_s16       *coeff,
 *                                                   mlib_s32       *cscale,
 *                                                   const mlib_s16 *signal,
 *                                                   void           *state);
 *      void mlib_SignalLPCAutoCorrelFree_S16(void *state);
 *
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
#include <vis_proto.h>

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
    m7lib_SignalLPCAutoCorrelGetEnergy_S16
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

#define	FLOAT2INT_CLAMP(DST, SRC)	DST = ((mlib_s32)(SRC) >> 16)

#else /* MLIB_USE_FTOI_CLAMPING */

#define	FLOAT2INT_CLAMP(DST, SRC)                               \
	{                                                       \
	    mlib_d64 dsrc = (mlib_d64)(SRC);                    \
	                                                        \
	    if (dsrc > (mlib_d64)MLIB_S32_MAX)                  \
		dsrc = (mlib_d64)MLIB_S32_MAX;                  \
	    if (dsrc < (mlib_d64)MLIB_S32_MIN)                  \
		dsrc = (mlib_d64)MLIB_S32_MIN;                  \
	    DST = (mlib_s32)dsrc >> 16;                         \
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

typedef struct
{
	mlib_s32 order;
	mlib_s32 length;
/* m + 1 partial correlation (PARCOR) coefficients */
	mlib_d64 *parcor;
/* energy of the signal Err[1] */
	mlib_d64 *Err;
/* buffer for correlations */
	mlib_d64 *cor_buf;
/* buffer for coeff's */
	mlib_d64 *lpc_coef_buf;
/* buffer for s16 to f32 conversion */
	mlib_d64 *convert_buf;
} mlib_lpc_state;

/* *********************************************************** */

/* power[i] = (mlib_f32) 2 ^ i */

static const mlib_d64 power[] = {
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
	__mlib_SignalLPCAutoCorrel_S16_cor_i(cor, signal, length,
		0, order + 1);
}

void __mlib_SignalLPCAutoCorrel_S16_cor_i(
	void *rcor,
	const void *rdb_buf,
	mlib_s32 length,
	mlib_s32 i0,
	mlib_s32 i1)
{
	mlib_d64 *cor = rcor;
	const mlib_d64 *db_buf = rdb_buf;
	mlib_d64 x0, x1, x2, x3, x4, x5, x6, x7, x8;
	mlib_d64 y0, y1, y2, y3, y4, y5, y6, y7, y8;
	mlib_d64 z0, z1, z2, z3, z4, z5, z6, z7;
	mlib_s32 i, j1, j2;

	for (i = i0; i < i1 - 7; i += 8) {
		x0 = db_buf[0];
		x1 = db_buf[1];
		x2 = db_buf[2];
		x3 = db_buf[3];
		x4 = db_buf[4];
		x5 = db_buf[5];
		x6 = db_buf[6];
		x7 = db_buf[7];

		y0 = db_buf[i + 0];
		y1 = db_buf[i + 1];
		y2 = db_buf[i + 2];
		y3 = db_buf[i + 3];
		y4 = db_buf[i + 4];
		y5 = db_buf[i + 5];
		y6 = db_buf[i + 6];
		y7 = db_buf[i + 7];

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

#pragma pipeloop(0)
		for (j1 = 8, j2 = i + 8; j2 < length; j1++, j2++) {
			x8 = db_buf[j1];
			y8 = db_buf[j2];
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
		x0 = db_buf[0];
		x1 = db_buf[1];
		x2 = db_buf[2];
		x3 = db_buf[3];

		y0 = db_buf[i + 0];
		y1 = db_buf[i + 1];
		y2 = db_buf[i + 2];
		y3 = db_buf[i + 3];

		z0 = y0 * x0 + y1 * x1 + y2 * x2 + y3 * x3;
		z1 = y1 * x0 + y2 * x1 + y3 * x2;
		z2 = y2 * x0 + y3 * x1;
		z3 = y3 * x0;

#pragma pipeloop(0)
		for (j1 = 4, j2 = i + 4; j2 < length; j1++, j2++) {
			x4 = db_buf[j1];
			y4 = db_buf[j2];
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
		x0 = db_buf[0];
		x1 = db_buf[1];
		y0 = db_buf[i + 0];
		y1 = db_buf[i + 1];

		z0 = y0 * x0 + y1 * x1;
		z1 = y1 * x0;

#pragma pipeloop(0)
		for (j1 = 2, j2 = i + 2; j2 < length; j1++, j2++) {
			x2 = db_buf[j1];
			y2 = db_buf[j2];

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
		x0 = db_buf[0];
		y0 = db_buf[i];
		z0 = y0 * x0;

#pragma pipeloop(0)
		for (j1 = 1, j2 = i + 1; j2 < length; j1++, j2++) {
			x1 = db_buf[j1];
			y1 = db_buf[j2];
			z0 += y1 * x1;
		}

		cor[i + 0] = z0;
	}
}

static void
LPCSignalConvert(
    mlib_d64 *buffer,
    const mlib_s16 *signal,
    mlib_s32 length)
{
	mlib_d64 *sp;
	mlib_d64 src, src0, src1, str1, str2;
	mlib_s32 i;
	mlib_f32 fone = vis_to_float(0x00010001);

	if (((mlib_addr)signal & 7) == 0) {
		sp = (mlib_d64 *)signal;

#pragma pipeloop(0)
		for (i = 0; i <= length - 4; i += 4) {
			src = (*sp++);
			str1 = vis_fmuld8ulx16(fone, vis_read_hi(src));
			buffer[i] = *((mlib_s32 *)&str1);
			buffer[i + 1] = *((mlib_s32 *)&str1 + 1);
			str2 = vis_fmuld8ulx16(fone, vis_read_lo(src));
			buffer[i + 2] = *((mlib_s32 *)&str2);
			buffer[i + 3] = *((mlib_s32 *)&str2 + 1);
		}

		for (; i < length; i++) {
			buffer[i] = signal[i];
		}
	} else {
		sp = (mlib_d64 *)vis_alignaddr((void *)signal, 0);
		src0 = (*sp++);

#pragma pipeloop(0)
		for (i = 0; i <= length - 4; i += 4) {
			src1 = (*sp++);
			src = vis_faligndata(src0, src1);
			src0 = src1;
			str1 = vis_fmuld8ulx16(fone, vis_read_hi(src));
			buffer[i] = *((mlib_s32 *)&str1);
			buffer[i + 1] = *((mlib_s32 *)&str1 + 1);
			str2 = vis_fmuld8ulx16(fone, vis_read_lo(src));
			buffer[i + 2] = *((mlib_s32 *)&str2);
			buffer[i + 3] = *((mlib_s32 *)&str2 + 1);
		}

		for (; i < length; i++) {
			buffer[i] = signal[i];
		}
	}
}
#if 0
static void
LPCSignalConvert(
    mlib_d64 *buffer,
    const mlib_s16 *signal,
    mlib_s32 length)
{
	mlib_d64 *sp;
	mlib_d64 src, src0, src1, str1, str2;
	mlib_s32 i;
	mlib_f32 fone = vis_to_float(0x00010001);

	if (((mlib_addr)signal & 7) == 0) {
		sp = (mlib_d64 *)signal;

#pragma pipeloop(0)
		for (i = 0; i <= length - 4; i += 4) {
			src = (*sp++);
			str1 = vis_fmuld8ulx16(fone, vis_read_hi(src));
			buffer[i] = *((mlib_s32 *)&str1);
			buffer[i + 1] = *((mlib_s32 *)&str1 + 1);
			str2 = vis_fmuld8ulx16(fone, vis_read_lo(src));
			buffer[i + 2] = *((mlib_s32 *)&str2);
			buffer[i + 3] = *((mlib_s32 *)&str2 + 1);
		}

		for (; i < length; i++) {
			buffer[i] = signal[i];
		}
	} else {
		sp = (mlib_d64 *)vis_alignaddr((void *)signal, 0);
		src0 = (*sp++);

#pragma pipeloop(0)
		for (i = 0; i <= length - 4; i += 4) {
			src1 = (*sp++);
			src = vis_faligndata(src0, src1);
			src0 = src1;
			str1 = vis_fmuld8ulx16(fone, vis_read_hi(src));
			buffer[i] = *((mlib_s32 *)&str1);
			buffer[i + 1] = *((mlib_s32 *)&str1 + 1);
			str2 = vis_fmuld8ulx16(fone, vis_read_lo(src));
			buffer[i + 2] = *((mlib_s32 *)&str2);
			buffer[i + 3] = *((mlib_s32 *)&str2 + 1);
		}

		for (; i < length; i++) {
			buffer[i] = signal[i];
		}
	}
}
#endif
/* *********************************************************** */

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
	mlib_d64 *reflect = lpc->parcor;
	mlib_s32 i, j;
	mlib_d64 *cor = lpc->cor_buf;
	mlib_d64 *Err = lpc->Err;
	mlib_d64 error, r, *lpc_coef = lpc->lpc_coef_buf;
	mlib_f32 scale = 1, mul = power[16];
	mlib_d64 *db_buf = lpc->convert_buf;
	mlib_f32 val0, val1;

	if ((state == NULL) || (signal == NULL) || (coeff == NULL))
		return (MLIB_FAILURE);

	LPCSignalConvert(db_buf, signal, length);

/* Autocorrelation */
	mlib_SignalLPCAutoCorrel_S16_cor(cor, db_buf, order, length);

/* Checking for obvious error */

	if (cor[0] == 0)
		return (MLIB_FAILURE);

/* Levinson-Durbin algorithm */
	error = cor[0];

	for (i = 0; i < order; i++) {
/* Sum up this iteration's reflection coefficient */
		r = cor[i + 1];

#pragma pipeloop(0)
		for (j = 0; j < i; j++)
			r = r - lpc_coef[j] * cor[i - j];
		r = r / error;
		reflect[i + 1] = r;

/* Update LPC coefficients and total error */
		lpc_coef[i] = r;

#pragma pipeloop(0)
		for (j = 0; j < i / 2; j++) {
			mlib_d64 tmp0 = lpc_coef[j];
			mlib_d64 tmp1 = lpc_coef[i - 1 - j];

			lpc_coef[j] = tmp0 - r * tmp1;
			lpc_coef[i - 1 - j] = tmp1 - r * tmp0;
		}

		if (i & 1) {
			mlib_d64 tmp0 = lpc_coef[j];

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

#pragma pipeloop(0)
	for (i = 1; i < order; i += 2) {
		val0 = lpc_coef[i - 1] * scale;
		val1 = lpc_coef[i] * scale;
		val0 = (val0 > 0) ? (val0 + .5) : (val0 - .5);
		val1 = (val1 > 0) ? (val1 + .5) : (val1 - .5);
		FLOAT2INT_CLAMP(coeff[i], val0 * 65536);
		FLOAT2INT_CLAMP(coeff[i + 1], val1 * 65536);
	}

	if (i <= order) {
		val0 = lpc_coef[i - 1] * scale;
		val0 = (val0 > 0) ? (val0 + .5) : (val0 - .5);
		FLOAT2INT_CLAMP(coeff[i], val0 * 65536);
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
	mlib_d64 *reflect = lpc->parcor;
	mlib_s32 i, j, sfactor;
	mlib_d64 *cor = lpc->cor_buf, *Err = lpc->Err;
	mlib_d64 error, r, *lpc_coef = lpc->lpc_coef_buf;
	mlib_f32 scale = 1, mul = power[16];
	mlib_d64 *db_buf = lpc->convert_buf, max_coef;
	mlib_s32 exp2;

	if ((state == NULL) || (signal == NULL) || (coeff == NULL))
		return (MLIB_FAILURE);

	LPCSignalConvert(db_buf, signal, length);

/* Autocorrelation */
	mlib_SignalLPCAutoCorrel_S16_cor(cor, db_buf, order, length);

/* Checking for obvious error */

	if (cor[0] == 0)
		return (MLIB_FAILURE);

/* Levinson-Durbin algorithm */
	error = cor[0];

	for (i = 0; i < order; i++) {
/* Sum up this iteration's reflection coefficient */

		r = cor[i + 1];

#pragma pipeloop(0)
		for (j = 0; j < i; j++)
			r = r - lpc_coef[j] * cor[i - j];
		r = r / error;
		reflect[i + 1] = r;

/* Update LPC coefficients and total error */
		lpc_coef[i] = r;

#pragma pipeloop(0)
		for (j = 0; j < i / 2; j++) {
			mlib_d64 tmp0 = lpc_coef[j];
			mlib_d64 tmp1 = lpc_coef[i - 1 - j];

			lpc_coef[j] = tmp0 - r * tmp1;
			lpc_coef[i - 1 - j] = tmp1 - r * tmp0;
		}

		if (i & 1) {
			mlib_d64 tmp0 = lpc_coef[j];

			lpc_coef[j] = tmp0 - tmp0 * r;
		}

		error *= 1 - r * r;

		if (error <= 0)
			return (MLIB_FAILURE);
	}

	Err[0] = error;

/* max_coef = 1; */
	max_coef = 0;

#pragma pipeloop(0)
	for (i = 1; i <= order; i++) {
		mlib_d64 mod = mlib_fabs(lpc_coef[i - 1]);

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

#pragma pipeloop(0)
	for (i = 1; i <= order; i++) {
		mlib_d64 val = lpc_coef[i - 1] * scale;

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
	mlib_d64 *parcor_ptr, *Err_ptr;

/* check for obvious errors */

	if (state == NULL)
		return (MLIB_FAILURE);

	if (order < 1)
		return (MLIB_FAILURE);

	if (length <= 0)
		return (MLIB_FAILURE);

/* to be multiple of double */
	struct_size = (sizeof (mlib_lpc_state) + 7) & ~7;

	state_ptr =
	    (mlib_lpc_state *) __mlib_malloc(struct_size +
	    sizeof (mlib_d64) * 2 + (order + 1) * 3 * sizeof (mlib_d64) +
	    (length + 1) * sizeof (mlib_d64));

	if (state_ptr == NULL)
		return (MLIB_FAILURE);

	state_ptr->order = order;
	state_ptr->length = length;
	state_ptr->parcor = (mlib_d64 *)((mlib_u8 *)state_ptr + struct_size);
	state_ptr->Err = (state_ptr->parcor) + (order + 1);
	state_ptr->cor_buf = (state_ptr->Err) + 1;
	state_ptr->lpc_coef_buf = (state_ptr->cor_buf) + (order + 1);
	state_ptr->convert_buf = (state_ptr->lpc_coef_buf) + (order + 1);

	parcor_ptr = state_ptr->parcor;
	Err_ptr = state_ptr->Err;

#pragma pipeloop(0)
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
/* 2^15 * 2^15 */
	mlib_d64 *en, error, div = 1073741824;
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

	FLOAT2INT_CLAMP(energy[0], error * 65536);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLPCAutoCorrelGetEnergy_S16_Adp(
    mlib_s16 *energy,
    mlib_s32 *escale,
    void *state)
{
/* 2^15 * 2^15 */
	mlib_d64 *en, error, div = 1073741824;
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
	mlib_d64 *par = lpc->parcor, val;
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

#pragma pipeloop(0)
	for (i = 1; i <= order; i++) {
		val = par[i] * scale;
		val = (val > 0) ? (val + .5) : (val - .5);
		FLOAT2INT_CLAMP(parcor[i], val * 65536);
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
	mlib_d64 *par = lpc->parcor, max_par, val;
	mlib_s32 i, order = lpc->order, sfactor, exp2;
	mlib_f32 scale = 1, mul = power[16];

/* check for obvious errors */

	if ((state == NULL) || (parcor == NULL))
		return (MLIB_FAILURE);

	max_par = mlib_fabs(par[1]);
	for (i = 2; i <= order; i++) {
		mlib_d64 mod = mlib_fabs(par[i]);

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

#pragma pipeloop(0)
	for (i = 1; i <= order; i++) {
		val = par[i] * scale;
		parcor[i] =
		    (val > 0) ? (mlib_s16)(val + .5) : (mlib_s16)(val - .5);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
