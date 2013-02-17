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

#pragma ident	"@(#)mlib_SignalLPCCovariance_Fp.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalLPCCovariance_F32     - linear predictive coding with
 *                                         covariance method
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalLPCCovarianceInit_F32(void     **state,
 *                                                   mlib_s32 length,
 *                                                   mlib_s32 order);
 *      mlib_status mlib_SignalLPCCovariance_F32(mlib_f32       *coeff,
 *                                               const mlib_f32 *signal,
 *                                               void           *state);
 *      void mlib_SignalLPCCovarianceFree_F32(void *state);
 *
 * ARGUMENTS
 *      coeff   the linear prediction coefficients.
 *      signal  the input signal vector.
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
 *      are the covariance coefficients of c(*), N is the length
 *      of the input speech vector.
 *
 *      Note that the covariance matrix C is a symmetric matrix,
 *      and the equations  can be solved efficiently with Cholesky
 *      decomposition method.
 *
 *      See "Fundamentals of Speech Recognition" by Lawrence Rabiner
 *      and Biing-Hwang Juang, Prentice Hall, 1993.
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

#pragma weak mlib_SignalLPCCovarianceFree_F32 = \
	__mlib_SignalLPCCovarianceFree_F32
#pragma weak mlib_SignalLPCCovarianceInit_F32 = \
	__mlib_SignalLPCCovarianceInit_F32
#pragma weak mlib_SignalLPCCovariance_F32 = __mlib_SignalLPCCovariance_F32
#pragma weak mlib_LPC_CreateMatrix = __mlib_LPC_CreateMatrix

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalLPCCovarianceFree_F32) mlib_SignalLPCCovarianceFree_F32
    __attribute__((weak, alias("__mlib_SignalLPCCovarianceFree_F32")));
__typeof__(__mlib_SignalLPCCovarianceInit_F32) mlib_SignalLPCCovarianceInit_F32
    __attribute__((weak, alias("__mlib_SignalLPCCovarianceInit_F32")));
__typeof__(__mlib_SignalLPCCovariance_F32) mlib_SignalLPCCovariance_F32
    __attribute__((weak, alias("__mlib_SignalLPCCovariance_F32")));
__typeof__(__mlib_LPC_CreateMatrix) mlib_LPC_CreateMatrix
    __attribute__((weak, alias("__mlib_LPC_CreateMatrix")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

typedef struct
{
	void *mem;
	mlib_s32 order;
	mlib_s32 length;
	mlib_s32 offset;
} mlib_LPC;

/* *********************************************************** */

#define	TYPE_CALC	mlib_f32

/* *********************************************************** */

void
__mlib_LPC_CreateMatrix_i(
    TYPE_CALC * mat,
    const mlib_f32 *s,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 length,
    mlib_s32 ai0,
    mlib_s32 ai1)
{
	mlib_s32 i, j, k, l, j1;
	mlib_f32 sum;
	TYPE_CALC *lmat;
	TYPE_CALC x0, x1, x2, x3, x4, x5, x6, x7, x8;
	TYPE_CALC q0, q1, q2, q3, q4, q5, q6, q7, q8;
	TYPE_CALC z0, z1, z2, z3, z4, z5, z6, z7;

	for (i = ai0; i <= (ai1 - 7); i += 8) {
		x0 = s[0];
		x1 = s[1];
		x2 = s[2];
		x3 = s[3];
		x4 = s[4];
		x5 = s[5];
		x6 = s[6];
		x7 = s[7];

		q0 = s[i + 0];
		q1 = s[i + 1];
		q2 = s[i + 2];
		q3 = s[i + 3];
		q4 = s[i + 4];
		q5 = s[i + 5];
		q6 = s[i + 6];
		q7 = s[i + 7];

		z0 = q0 * x0 + q1 * x1 + q2 * x2 + q3 * x3 + q4 * x4 + q5 * x5 +
		    q6 * x6 + q7 * x7;
		z1 = q1 * x0 + q2 * x1 + q3 * x2 + q4 * x3 + q5 * x4 + q6 * x5 +
		    q7 * x6;
		z2 = q2 * x0 + q3 * x1 + q4 * x2 + q5 * x3 + q6 * x4 + q7 * x5;
		z3 = q3 * x0 + q4 * x1 + q5 * x2 + q6 * x3 + q7 * x4;
		z4 = q4 * x0 + q5 * x1 + q6 * x2 + q7 * x3;
		z5 = q5 * x0 + q6 * x1 + q7 * x2;
		z6 = q6 * x0 + q7 * x1;
		z7 = q7 * x0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (j = 8, j1 = i + 8; j1 < n; j++, j1++) {
			x8 = s[j];
			q8 = s[j1];
			z0 += q8 * x8;
			z1 += q8 * x7;
			z2 += q8 * x6;
			z3 += q8 * x5;
			z4 += q8 * x4;
			z5 += q8 * x3;
			z6 += q8 * x2;
			z7 += q8 * x1;

			x1 = x2;
			x2 = x3;
			x3 = x4;
			x4 = x5;
			x5 = x6;
			x6 = x7;
			x7 = x8;

			q1 = q2;
			q2 = q3;
			q3 = q4;
			q4 = q5;
			q5 = q6;
			q6 = q7;
			q7 = q8;
		}

		mat[i + 0] = z0;
		mat[i + 1] = z1;
		mat[i + 2] = z2;
		mat[i + 3] = z3;
		mat[i + 4] = z4;
		mat[i + 5] = z5;
		mat[i + 6] = z6;
		mat[i + 7] = z7;
	}

	if (i <= ai1 - 3) {

		x0 = s[0];
		x1 = s[1];
		x2 = s[2];
		x3 = s[3];

		q0 = s[i + 0];
		q1 = s[i + 1];
		q2 = s[i + 2];
		q3 = s[i + 3];

		z0 = q0 * x0 + q1 * x1 + q2 * x2 + q3 * x3;
		z1 = q1 * x0 + q2 * x1 + q3 * x2;
		z2 = q2 * x0 + q3 * x1;
		z3 = q3 * x0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (j = 4, j1 = i + 4; j1 < n; j++, j1++) {
			x4 = s[j];
			q4 = s[j1];
			z0 += q4 * x4;
			z1 += q4 * x3;
			z2 += q4 * x2;
			z3 += q4 * x1;

			x1 = x2;
			x2 = x3;
			x3 = x4;
			q1 = q2;
			q2 = q3;
			q3 = q4;
		}

		mat[i + 0] = z0;
		mat[i + 1] = z1;
		mat[i + 2] = z2;
		mat[i + 3] = z3;

		i += 4;
	}

	if (i <= ai1 - 1) {

		x0 = s[0];
		x1 = s[1];

		q0 = s[i + 0];
		q1 = s[i + 1];

		z0 = q0 * x0 + q1 * x1;
		z1 = q1 * x0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (j = 2, j1 = i + 2; j1 < n; j++, j1++) {
			x2 = s[j];
			q2 = s[j1];
			z0 += q2 * x2;
			z1 += q2 * x1;

			x1 = x2;
			q1 = q2;
		}

		mat[i + 0] = z0;
		mat[i + 1] = z1;
		i += 2;
	}

	if (i <= ai1) {

		x0 = s[0];
		q0 = s[i + 0];

		z0 = q0 * x0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (j = 1, j1 = i + 1; j1 < n; j++, j1++) {
			x1 = s[j];
			q1 = s[j1];
			z0 += q1 * x1;
		}

		mat[i + 0] = z0;
	}

	if (ai1 == m) {
/* Main diag */
		lmat = mat + (mlib_s32)((mlib_f32)m * length);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (sum = 0.f, j = 0; j <= n - m - 1; j++) {
			sum += s[j] * s[j];
		}

		lmat[m] = sum;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (i = m - 1, j = n - m; i >= 1; i--, j++) {
			lmat -= length;
			sum += s[j] * s[j];
			lmat[i] = sum;
		}
		ai1 --;
	}
	if (ai0 == 0) ai0 ++;

/* Other elements */
	lmat = mat + (mlib_s32)((mlib_f32)ai1 * length);
	for (i = ai1; i >= ai0; i--) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (sum = 0.f, j = 0; j <= n - m - 1; j++) {
			sum += s[j] * s[j + m - i];
		}

		lmat[m] = sum;
		lmat -= length;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (k = 1, l = i - 1; l >= 1; l--, k++, j++) {
			sum += s[j] * s[j + m - i];
			mat[(i - k) * length + m - k] = sum;
		}
	}
}

void
__mlib_LPC_CreateMatrix(
    TYPE_CALC * mat,
    const mlib_f32 *s,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 length)
{
	mlib_s32 i, j, k, l, j1;
	mlib_f32 sum;
	TYPE_CALC *lmat;
	TYPE_CALC x0, x1, x2, x3, x4, x5, x6, x7, x8;
	TYPE_CALC q0, q1, q2, q3, q4, q5, q6, q7, q8;
	TYPE_CALC z0, z1, z2, z3, z4, z5, z6, z7;

	for (i = 0; i <= (m - 7); i += 8) {
		x0 = s[0];
		x1 = s[1];
		x2 = s[2];
		x3 = s[3];
		x4 = s[4];
		x5 = s[5];
		x6 = s[6];
		x7 = s[7];

		q0 = s[i + 0];
		q1 = s[i + 1];
		q2 = s[i + 2];
		q3 = s[i + 3];
		q4 = s[i + 4];
		q5 = s[i + 5];
		q6 = s[i + 6];
		q7 = s[i + 7];

		z0 = q0 * x0 + q1 * x1 + q2 * x2 + q3 * x3 + q4 * x4 + q5 * x5 +
		    q6 * x6 + q7 * x7;
		z1 = q1 * x0 + q2 * x1 + q3 * x2 + q4 * x3 + q5 * x4 + q6 * x5 +
		    q7 * x6;
		z2 = q2 * x0 + q3 * x1 + q4 * x2 + q5 * x3 + q6 * x4 + q7 * x5;
		z3 = q3 * x0 + q4 * x1 + q5 * x2 + q6 * x3 + q7 * x4;
		z4 = q4 * x0 + q5 * x1 + q6 * x2 + q7 * x3;
		z5 = q5 * x0 + q6 * x1 + q7 * x2;
		z6 = q6 * x0 + q7 * x1;
		z7 = q7 * x0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (j = 8, j1 = i + 8; j1 < n; j++, j1++) {
			x8 = s[j];
			q8 = s[j1];
			z0 += q8 * x8;
			z1 += q8 * x7;
			z2 += q8 * x6;
			z3 += q8 * x5;
			z4 += q8 * x4;
			z5 += q8 * x3;
			z6 += q8 * x2;
			z7 += q8 * x1;

			x1 = x2;
			x2 = x3;
			x3 = x4;
			x4 = x5;
			x5 = x6;
			x6 = x7;
			x7 = x8;

			q1 = q2;
			q2 = q3;
			q3 = q4;
			q4 = q5;
			q5 = q6;
			q6 = q7;
			q7 = q8;
		}

		mat[i + 0] = z0;
		mat[i + 1] = z1;
		mat[i + 2] = z2;
		mat[i + 3] = z3;
		mat[i + 4] = z4;
		mat[i + 5] = z5;
		mat[i + 6] = z6;
		mat[i + 7] = z7;
	}

	if (i <= m - 3) {

		x0 = s[0];
		x1 = s[1];
		x2 = s[2];
		x3 = s[3];

		q0 = s[i + 0];
		q1 = s[i + 1];
		q2 = s[i + 2];
		q3 = s[i + 3];

		z0 = q0 * x0 + q1 * x1 + q2 * x2 + q3 * x3;
		z1 = q1 * x0 + q2 * x1 + q3 * x2;
		z2 = q2 * x0 + q3 * x1;
		z3 = q3 * x0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (j = 4, j1 = i + 4; j1 < n; j++, j1++) {
			x4 = s[j];
			q4 = s[j1];
			z0 += q4 * x4;
			z1 += q4 * x3;
			z2 += q4 * x2;
			z3 += q4 * x1;

			x1 = x2;
			x2 = x3;
			x3 = x4;
			q1 = q2;
			q2 = q3;
			q3 = q4;
		}

		mat[i + 0] = z0;
		mat[i + 1] = z1;
		mat[i + 2] = z2;
		mat[i + 3] = z3;

		i += 4;
	}

	if (i <= m - 1) {

		x0 = s[0];
		x1 = s[1];

		q0 = s[i + 0];
		q1 = s[i + 1];

		z0 = q0 * x0 + q1 * x1;
		z1 = q1 * x0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (j = 2, j1 = i + 2; j1 < n; j++, j1++) {
			x2 = s[j];
			q2 = s[j1];
			z0 += q2 * x2;
			z1 += q2 * x1;

			x1 = x2;
			q1 = q2;
		}

		mat[i + 0] = z0;
		mat[i + 1] = z1;
		i += 2;
	}

	if (i <= m) {

		x0 = s[0];
		q0 = s[i + 0];

		z0 = q0 * x0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (j = 1, j1 = i + 1; j1 < n; j++, j1++) {
			x1 = s[j];
			q1 = s[j1];
			z0 += q1 * x1;
		}

		mat[i + 0] = z0;
	}

/* Main diag */
	lmat = mat + (mlib_s32)((mlib_f32)m * length);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (sum = 0.f, j = 0; j <= n - m - 1; j++) {
		sum += s[j] * s[j];
	}

	lmat[m] = sum;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = m - 1, j = n - m; i >= 1; i--, j++) {
		lmat -= length;
		sum += s[j] * s[j];
		lmat[i] = sum;
	}

/* Other elements */
	lmat = mat + (mlib_s32)((mlib_f32)(m - 1) * length);
	for (i = m - 1; i >= 1; i--) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (sum = 0.f, j = 0; j <= n - m - 1; j++) {
			sum += s[j] * s[j + m - i];
		}

		lmat[m] = sum;
		lmat -= length;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (k = 1, l = i - 1; l >= 1; l--, k++, j++) {
			sum += s[j] * s[j + m - i];
			mat[(i - k) * length + m - k] = sum;
		}
	}
}

/* *********************************************************** */

static mlib_status
mlib_LPC_PrepareMatrix(
    TYPE_CALC * mat,
    mlib_s32 m,
    mlib_s32 length)
{
	mlib_s32 i, j, k;
	TYPE_CALC sum;

	for (i = 1; i <= m; i++) {
		for (j = i; j <= m; j++) {

			for (sum = mat[i * length + j],
			    k = i - 1; k >= 1; k--) {
				sum -=
				    mat[i * length + k] * mat[j * length + k];
			}

			if (i == j) {
				if (sum <= 0)
					return (MLIB_FAILURE);
				mat[i * length] = mlib_sqrtf(sum);
			} else {
				mat[j * length + i] = sum / mat[i * length];
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

static void
mlib_LPC_GetCoeff(
    TYPE_CALC * mat,
    mlib_f32 *x,
    mlib_s32 m,
    mlib_s32 length)
{
	mlib_s32 i, k;
	TYPE_CALC sum;

/* Solve L * y = b, storing y in x. */
	for (i = 1; i <= m; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (sum = mat[i], k = i - 1; k >= 1; k--)
			sum -= mat[i * length + k] * x[k];
		x[i] = sum / mat[i * length];
	}

/* Solve trans(L) * x = y. */
	for (i = m; i >= 1; i--) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (sum = x[i], k = i + 1; k <= m; k++)
			sum -= mat[k * length + i] * x[k];
		x[i] = sum / mat[i * length];
	}
}

/* *********************************************************** */

mlib_status
__mlib_SignalLPCCovarianceInit_F32(
    void **state,
    mlib_s32 length,
    mlib_s32 order)
{
	mlib_u32 size_str, size_mem;
	mlib_LPC *lpc_state;

	if (length <= 0)
		return (MLIB_FAILURE);

	if (order <= 0)
		return (MLIB_FAILURE);

	size_mem = (order + 1) * sizeof (TYPE_CALC);
	size_mem *= size_mem;

	size_str = (sizeof (mlib_LPC) + 7) & ~7;

	lpc_state = __mlib_malloc(size_mem + size_str);

	if (!lpc_state)
		return (MLIB_NULLPOINTER);

	lpc_state->offset = order + 1;
	lpc_state->order = order;
	lpc_state->length = length;
	lpc_state->mem = (mlib_u8 *)lpc_state + size_str;

	*state = lpc_state;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#ifdef __SUNPRO_C
#pragma no_inline(mlib_LPC_GetCoeff, mlib_LPC_PrepareMatrix)
#endif /* __SUNPRO_C */

/* *********************************************************** */

mlib_status
__mlib_SignalLPCCovariance_F32(
    mlib_f32 *coeff,
    const mlib_f32 *signal,
    void *state)
{
	mlib_LPC *lpc_state = state;
	TYPE_CALC *matrix = lpc_state->mem;
	mlib_s32 order = lpc_state->order;
	mlib_s32 length = lpc_state->length;
	mlib_s32 offset = lpc_state->offset;
	mlib_status status;

	mlib_LPC_CreateMatrix(matrix, signal, order, length, offset);
	status = mlib_LPC_PrepareMatrix(matrix, order, offset);

	if (status != MLIB_SUCCESS)
		return (status);
	mlib_LPC_GetCoeff(matrix, coeff, order, offset);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
__mlib_SignalLPCCovarianceFree_F32(
    void *state)
{
	__mlib_free(state);
}

/* *********************************************************** */
