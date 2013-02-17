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

#pragma ident	"@(#)mlib_SignalConv.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalConv_[S16|S16S]_[S16|S16S]_Sat - convolution
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalConv_F32S_F32S(mlib_f32       *dst,
 *                                            const mlib_f32 *src1,
 *                                            const mlib_f32 *src2,
 *                                            mlib_s32       m,
 *                                            mlib_s32       n);
 *      mlib_status mlib_SignalConv_F32_F32(mlib_f32       *dst,
 *                                          const mlib_f32 *src1,
 *                                          const mlib_f32 *src2,
 *                                          mlib_s32       m,
 *                                          mlib_s32       n);
 *      mlib_status mlib_SignalConv_S16S_S16S_Sat(mlib_s16       *dst,
 *                                                const mlib_s16 *src1,
 *                                                const mlib_s16 *src2,
 *                                                mlib_s32       m,
 *                                                mlib_s32       n);
 *      mlib_status mlib_SignalConv_S16_S16_Sat(mlib_s16       *dst,
 *                                              const mlib_s16 *src1,
 *                                              const mlib_s16 *src2,
 *                                              mlib_s32       m,
 *                                              mlib_s32       n);
 *
 * ARGUMENTS
 *      dst   Output signal array
 *      src1  Input signal array
 *      src2  Input signal array
 *      m     Number of samples in the first input signal array
 *      n     Number of samples in the second input signal array
 *
 * DESCRIPTION
 *           min(n, m)-1
 *      z[i] = SUM (y[k]*x[i-k]) ,  i = 0...(n+m-2)
 *             k = 0
 *
 * IMPLEMENTATION
 *
 * mlib_conv_fft uses an external function from signal/fourier section of mlib,
 * mlib_fftDispatcher_D64C_D64C with dst_aux = src_aux = NULL,
 *                              shiftscale = order
 */

#include <mlib_signal.h>
#include <mlib_Utils.h>
#include <mlib_SignalConv.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalConv_F32S_F32S = __mlib_SignalConv_F32S_F32S
#pragma weak mlib_SignalConv_F32_F32 = __mlib_SignalConv_F32_F32
#pragma weak mlib_SignalConv_S16S_S16S_Sat = __mlib_SignalConv_S16S_S16S_Sat
#pragma weak mlib_SignalConv_S16_S16_Sat = __mlib_SignalConv_S16_S16_Sat
#pragma weak mlib_SignalConvKernel_F32 = __mlib_SignalConvKernel_F32
#pragma weak mlib_SignalConvKernel_F32S = __mlib_SignalConvKernel_F32S
#pragma weak mlib_SignalConvKernel_S16 = __mlib_SignalConvKernel_S16
#pragma weak mlib_SignalConvKernel_S16S = __mlib_SignalConvKernel_S16S

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalConv_F32S_F32S) mlib_SignalConv_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalConv_F32S_F32S")));
__typeof__(__mlib_SignalConv_F32_F32) mlib_SignalConv_F32_F32
    __attribute__((weak, alias("__mlib_SignalConv_F32_F32")));
__typeof__(__mlib_SignalConv_S16S_S16S_Sat) mlib_SignalConv_S16S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalConv_S16S_S16S_Sat")));
__typeof__(__mlib_SignalConv_S16_S16_Sat) mlib_SignalConv_S16_S16_Sat
    __attribute__((weak, alias("__mlib_SignalConv_S16_S16_Sat")));
__typeof__(__mlib_SignalConvKernel_F32) mlib_SignalConvKernel_F32
    __attribute__((weak, alias("__mlib_SignalConvKernel_F32")));
__typeof__(__mlib_SignalConvKernel_F32S) mlib_SignalConvKernel_F32S
    __attribute__((weak, alias("__mlib_SignalConvKernel_F32S")));
__typeof__(__mlib_SignalConvKernel_S16) mlib_SignalConvKernel_S16
    __attribute__((weak, alias("__mlib_SignalConvKernel_S16")));
__typeof__(__mlib_SignalConvKernel_S16S) mlib_SignalConvKernel_S16S
    __attribute__((weak, alias("__mlib_SignalConvKernel_S16S")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

/*
 * The ceiling Log2 of an Integer
 * n <= 2 ^ (k)
 */

#define	LOG2(n, k)	        \
	int x, y, c, e;         \
	x = n;                  \
	y = -(x >> 16);         \
	c = (y >> 16) & 16;     \
	e = 16 - c;             \
	x = x >> c;             \
                                \
	y = x - 0x100;          \
	c = (y >> 16) & 8;      \
	e = e + c;              \
	x = x << c;             \
                                \
	y = x - 0x1000;         \
	c = (y >> 16) & 4;      \
	e = e + c;              \
	x = x << c;             \
                                \
	y = x - 0x4000;         \
	c = (y >> 16) & 2;      \
	e = e + c;              \
	x = x << c;             \
                                \
	y = x >> 14;            \
	c = y & ~(y >> 1);      \
	k = (31 - (e + 2 - c));

/* *********************************************************** */

#define	SATURATE_S16(X)                                            \
	(((X) > 0x7fff) ? MLIB_S16_MAX : ((X) <                    \
	(mlib_s32)(-0x8000) ? MLIB_S16_MIN : (mlib_s16)(X)))

#define	SAT_S(Y, X)	Y = (X >> 16)

/* *********************************************************** */

/* fk = 1 << (k - 1) (k=1, 2...); fk = 0 (k = 0) */
#define	MUL_QK(X, Y)	((X * Y + fk) >> k)

/* *********************************************************** */

#define	MLIB_FRAME	1024

/* approved experimentally */
#define	MAGIC_VALUE	5.

/* *********************************************************** */

static mlib_status mlib_fftConv_F32_F32(
    mlib_f32 *dst,
    mlib_f32 *src1,
    mlib_f32 *src2,
    mlib_s32 m,
    mlib_s32 n);

static mlib_status mlib_fftConv_F32S_F32S(
    mlib_f32 *dst,
    mlib_f32 *src1,
    mlib_f32 *src2,
    mlib_s32 m,
    mlib_s32 n);

/* *********************************************************** */

#define	SET_RANGE(beg0, end0, add) \
	beg = beg0; \
	end = end0; \
	if (beg + (add) < i0) beg = i0 - (add); \
	if (end + (add) > i1) end = i1 - (add); \
	if (beg > end) beg = end

#define	SRC2(a) ((a) >= 0 ? src2[a] : 0.)
#define	SRC2S(a) (((a) / 2) >= 0 ? src2[a] : 0.)

/* *********************************************************** */

void __mlib_SignalConvKernel_S16_a(
	const mlib_s16 *src2,
	mlib_d64 *ddst,
	mlib_d64 *dsrc1,
	mlib_d64 *dsrc2,
	mlib_s32 n,
	mlib_s32 i0,
	mlib_s32 i1)
{
	mlib_s32 j;
	mlib_d64 sum, sum0, sum1, sum2, sum3;
	mlib_d64 dsrc1_0, dsrc1_1, dsrc1_2, dsrc1_3;
	mlib_d64 tmp, dsrc2_1, dsrc2_2, dsrc2_3;
	mlib_s32 beg, end;
	mlib_d64 tt[4];

	dsrc1_0 = dsrc1[0];
	dsrc1_1 = dsrc1[1];
	dsrc1_2 = dsrc1[2];
	dsrc1_3 = dsrc1[3];

	SET_RANGE(0, n, 0);

	dsrc2_1 = dsrc2[beg + 0] = SRC2(beg + 0 - 4);
	dsrc2_2 = dsrc2[beg + 1] = SRC2(beg + 1 - 4);
	dsrc2_3 = dsrc2[beg + 2] = SRC2(beg + 2 - 4);
	tmp = dsrc2[beg + 3] = SRC2(beg + 3 - 4);

	sum0 = dsrc1_3 * dsrc2_1;
	sum1 = dsrc1_2 * dsrc2_2;
	sum2 = dsrc1_1 * dsrc2_3;
	sum3 = dsrc1_0 * tmp;
	dsrc2_1 = dsrc2_2;
	dsrc2_2 = dsrc2_3;
	dsrc2_3 = tmp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = beg; j < end; j++) {
		sum = sum0 + sum1 + sum2 + sum3;
		tmp = dsrc2[j + 4] = src2[j];
		sum0 = dsrc1_3 * dsrc2_1;
		sum1 = dsrc1_2 * dsrc2_2;
		sum2 = dsrc1_1 * dsrc2_3;
		sum3 = dsrc1_0 * tmp;
		dsrc2_1 = dsrc2_2;
		dsrc2_2 = dsrc2_3;
		dsrc2_3 = tmp;
		ddst[j] = sum;
	}

	SET_RANGE(0, 4, n);

	if (end > beg) {
		tt[0] = sum0 + sum1 + sum2 + sum3;
		tt[1] =
		    dsrc1_3 * dsrc2_1 + dsrc1_2 * dsrc2_2 + dsrc1_1 * dsrc2_3;
		tt[2] = dsrc1_3 * dsrc2_2 + dsrc1_2 * dsrc2_3;
		tt[3] = dsrc1_3 * dsrc2_3;

		for (j = beg; j < end; j++)
			ddst[n + j] = tt[j];
	}
}

void __mlib_SignalConvKernel_S16_b(
	mlib_s16 *dst,
	mlib_d64 *ddst,
	mlib_d64 *dsrc1,
	mlib_d64 *dsrc2,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 i0,
	mlib_s32 i1)
{
	mlib_s32 i, j;
	mlib_d64 sum, sum0, sum1, sum2, sum3;
	mlib_d64 dsrc1_0, dsrc1_1, dsrc1_2, dsrc1_3;
	mlib_d64 tmp, dsrc2_1, dsrc2_2, dsrc2_3;
	mlib_s32 s = n + m + 4;
	mlib_s32 beg, end;

	SET_RANGE(n + 4, s, 0);

	for (i = beg; i < end; i++)
		ddst[i] = 0.;

	for (i = 4; i + 4 < m; i += 4) {

		SET_RANGE(0, n + 4, i);

		if (end > beg) {
			sum = ddst[i + beg];
			dsrc1_0 = dsrc1[i];
			dsrc1_1 = dsrc1[i + 1];
			dsrc1_2 = dsrc1[i + 2];
			dsrc1_3 = dsrc1[i + 3];
			dsrc2_1 = dsrc2[beg + 1];
			dsrc2_2 = dsrc2[beg + 2];
			dsrc2_3 = dsrc2[beg + 3];
			sum0 = dsrc1_3 * dsrc2[beg];
			sum1 = dsrc1_2 * dsrc2_1;
			sum2 = dsrc1_1 * dsrc2_2;
			sum3 = dsrc1_0 * dsrc2_3;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = beg; j < end - 1; j++) {
				sum += sum0 + sum1 + sum2 + sum3;
				tmp = dsrc2[j + 4];
				sum0 = dsrc1_3 * dsrc2_1;
				sum1 = dsrc1_2 * dsrc2_2;
				sum2 = dsrc1_1 * dsrc2_3;
				sum3 = dsrc1_0 * tmp;
				dsrc2_1 = dsrc2_2;
				dsrc2_2 = dsrc2_3;
				dsrc2_3 = tmp;
				ddst[i + j] = sum;
				sum = ddst[i + j + 1];
			}

			sum += sum0 + sum1 + sum2 + sum3;
			ddst[i + j] = sum;
		}
	}

	if (i < m) {
		SET_RANGE(0, i, 1);

		for (j = beg; j < end; j++) {
			mlib_d64 sum = ddst[j + 1] * 2.;

			SAT_S16(dst[j], sum);
		}

		SET_RANGE(0, s - 4 - i, i);

		if (beg < end) {
			sum = ddst[i + beg];
			dsrc1_0 = dsrc1[i];
			dsrc1_1 = dsrc1[i + 1];
			dsrc1_2 = dsrc1[i + 2];
			dsrc1_3 = dsrc1[i + 3];
			dsrc2_1 = dsrc2[beg + 1];
			dsrc2_2 = dsrc2[beg + 2];
			dsrc2_3 = dsrc2[beg + 3];
			sum0 = dsrc1_3 * dsrc2[beg];
			sum1 = dsrc1_2 * dsrc2_1;
			sum2 = dsrc1_1 * dsrc2_2;
			sum3 = dsrc1_0 * dsrc2_3;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = beg; j < end - 1; j++) {
				sum = 2. * (sum + sum0 + sum1 + sum2 + sum3);
				tmp = dsrc2[j + 4];
				sum0 = dsrc1_3 * dsrc2_1;
				sum1 = dsrc1_2 * dsrc2_2;
				sum2 = dsrc1_1 * dsrc2_3;
				sum3 = dsrc1_0 * tmp;
				dsrc2_1 = dsrc2_2;
				dsrc2_2 = dsrc2_3;
				dsrc2_3 = tmp;
				SAT_S16(dst[i + j - 1], sum);
				sum = ddst[i + j + 1];
			}

			sum = 2. * (sum + sum0 + sum1 + sum2 + sum3);
			SAT_S16(dst[i + j - 1], sum);
		}
	} else {

		SET_RANGE(0, s - 1 - 4, 1);

		for (i = beg; i < end; i++) {
			mlib_d64 sum = ddst[i + 1] * 2.;

			SAT_S16(dst[i], sum);
		}
	}
}

void __mlib_SignalConvKernel_F32_a(
	const mlib_f32 *src2,
	mlib_d64 *ddst,
	mlib_d64 *dsrc1,
	mlib_d64 *dsrc2,
	mlib_s32 n,
	mlib_s32 i0,
	mlib_s32 i1)
{
	mlib_s32 j;
	mlib_d64 sum, sum0, sum1, sum2, sum3;
	mlib_d64 dsrc1_0, dsrc1_1, dsrc1_2, dsrc1_3;
	mlib_d64 tmp, dsrc2_1, dsrc2_2, dsrc2_3;
	mlib_s32 beg, end;
	mlib_d64 tt[4];

	dsrc1_0 = dsrc1[0];
	dsrc1_1 = dsrc1[1];
	dsrc1_2 = dsrc1[2];
	dsrc1_3 = dsrc1[3];

	SET_RANGE(0, n, 0);

	dsrc2_1 = dsrc2[beg + 0] = SRC2(beg + 0 - 4);
	dsrc2_2 = dsrc2[beg + 1] = SRC2(beg + 1 - 4);
	dsrc2_3 = dsrc2[beg + 2] = SRC2(beg + 2 - 4);
	tmp = dsrc2[beg + 3] = SRC2(beg + 3 - 4);

	sum0 = dsrc1_3 * dsrc2_1;
	sum1 = dsrc1_2 * dsrc2_2;
	sum2 = dsrc1_1 * dsrc2_3;
	sum3 = dsrc1_0 * tmp;
	dsrc2_1 = dsrc2_2;
	dsrc2_2 = dsrc2_3;
	dsrc2_3 = tmp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = beg; j < end; j++) {
		sum = sum0 + sum1 + sum2 + sum3;
		tmp = dsrc2[j + 4] = src2[j];
		sum0 = dsrc1_3 * dsrc2_1;
		sum1 = dsrc1_2 * dsrc2_2;
		sum2 = dsrc1_1 * dsrc2_3;
		sum3 = dsrc1_0 * tmp;
		dsrc2_1 = dsrc2_2;
		dsrc2_2 = dsrc2_3;
		dsrc2_3 = tmp;
		ddst[j] = sum;
	}

	SET_RANGE(0, 4, n);

	if (end > beg) {
		tt[0] = sum0 + sum1 + sum2 + sum3;
		tt[1] =
		    dsrc1_3 * dsrc2_1 + dsrc1_2 * dsrc2_2 + dsrc1_1 * dsrc2_3;
		tt[2] = dsrc1_3 * dsrc2_2 + dsrc1_2 * dsrc2_3;
		tt[3] = dsrc1_3 * dsrc2_3;

		for (j = beg; j < end; j++)
			ddst[n + j] = tt[j];
	}
}

void __mlib_SignalConvKernel_F32_b(
	mlib_f32 *dst,
	mlib_d64 *ddst,
	mlib_d64 *dsrc1,
	mlib_d64 *dsrc2,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 i0,
	mlib_s32 i1)
{
	mlib_s32 i, j;
	mlib_d64 sum, sum0, sum1, sum2, sum3;
	mlib_d64 dsrc1_0, dsrc1_1, dsrc1_2, dsrc1_3;
	mlib_d64 tmp, dsrc2_1, dsrc2_2, dsrc2_3;
	mlib_s32 s = n + m + 4;
	mlib_s32 beg, end;

	SET_RANGE(n + 4, s, 0);

	for (i = beg; i < end; i++)
		ddst[i] = 0.;

	for (i = 4; i + 4 < m; i += 4) {
		SET_RANGE(0, n + 4, i);

		if (end > beg) {
			sum = ddst[i + beg];
			dsrc1_0 = dsrc1[i];
			dsrc1_1 = dsrc1[i + 1];
			dsrc1_2 = dsrc1[i + 2];
			dsrc1_3 = dsrc1[i + 3];
			dsrc2_1 = dsrc2[beg + 1];
			dsrc2_2 = dsrc2[beg + 2];
			dsrc2_3 = dsrc2[beg + 3];
			sum0 = dsrc1_3 * dsrc2[beg];
			sum1 = dsrc1_2 * dsrc2_1;
			sum2 = dsrc1_1 * dsrc2_2;
			sum3 = dsrc1_0 * dsrc2_3;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = beg; j < end - 1; j++) {
				sum += sum0 + sum1 + sum2 + sum3;
				tmp = dsrc2[j + 4];
				sum0 = dsrc1_3 * dsrc2_1;
				sum1 = dsrc1_2 * dsrc2_2;
				sum2 = dsrc1_1 * dsrc2_3;
				sum3 = dsrc1_0 * tmp;
				dsrc2_1 = dsrc2_2;
				dsrc2_2 = dsrc2_3;
				dsrc2_3 = tmp;
				ddst[i + j] = sum;
				sum = ddst[i + j + 1];
			}

			sum += sum0 + sum1 + sum2 + sum3;
			ddst[i + j] = sum;
		}
	}

	if (i < m) {
		SET_RANGE(0, i, 1);

		for (j = beg; j < end; j++)
			dst[j] = ddst[j + 1];

		SET_RANGE(0, s - 4 - i, i);

		if (beg < end) {
			sum = ddst[i + beg];
			dsrc1_0 = dsrc1[i];
			dsrc1_1 = dsrc1[i + 1];
			dsrc1_2 = dsrc1[i + 2];
			dsrc1_3 = dsrc1[i + 3];
			dsrc2_1 = dsrc2[beg + 1];
			dsrc2_2 = dsrc2[beg + 2];
			dsrc2_3 = dsrc2[beg + 3];
			sum0 = dsrc1_3 * dsrc2[beg];
			sum1 = dsrc1_2 * dsrc2_1;
			sum2 = dsrc1_1 * dsrc2_2;
			sum3 = dsrc1_0 * dsrc2_3;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = beg; j < end - 1; j++) {
				sum = (sum + sum0 + sum1 + sum2 + sum3);
				tmp = dsrc2[j + 4];
				sum0 = dsrc1_3 * dsrc2_1;
				sum1 = dsrc1_2 * dsrc2_2;
				sum2 = dsrc1_1 * dsrc2_3;
				sum3 = dsrc1_0 * tmp;
				dsrc2_1 = dsrc2_2;
				dsrc2_2 = dsrc2_3;
				dsrc2_3 = tmp;
				dst[i + j - 1] = sum;
				sum = ddst[i + j + 1];
			}

			sum = (sum + sum0 + sum1 + sum2 + sum3);
			dst[i + j - 1] = sum;
		}
	} else {

		SET_RANGE(0, s - 1 - 4, 1);

		for (i = beg; i < end; i++)
			dst[i] = ddst[i + 1];
	}
}

/* *********************************************************** */

void
__mlib_SignalConvKernel_S16(
	mlib_s16 *dst,
	const mlib_s16 *src1,
	const mlib_s16 *src2,
	mlib_d64 *pdst,
	mlib_d64 *psrc1,
	mlib_d64 *psrc2,
	mlib_s32 m,
	mlib_s32 n)
{
	mlib_s32 i, j;
	mlib_s32 *ddst = (mlib_s32 *)pdst;
	mlib_s32 *dsrc1 = (mlib_s32 *)psrc1;
	mlib_s32 *dsrc2 = (mlib_s32 *)psrc2;

	mlib_s32 sum, tmpsum;
	mlib_s32 sum0, sum1, sum2, sum3;
	mlib_s32 dsrc1_0, dsrc1_1, dsrc1_2, dsrc1_3;
	mlib_s32 tmp, dsrc2_1, dsrc2_2, dsrc2_3;
	mlib_s32 s = n + m + 4;
/* fk = 1 << (k - 1) (k=1, 2...); fk = 0 (k = 0) */
	mlib_s32 fk;
/* n <= 2 ^ k */
	mlib_s16 k;

	LOG2(n, k)
	fk = ((-k) >> 16) & (1 << (k - 1));

	for (i = 0; i < m; i++)
		dsrc1[i] = src1[i];

	for (i = m; i < m + 4; i++)
		dsrc1[i] = 0;

	sum0 = sum1 = sum2 = sum3 = dsrc2_1 = dsrc2_2 = dsrc2_3 =
	    dsrc2[1] = dsrc2[2] = dsrc2[3] = 0;
	dsrc1_0 = dsrc1[0];
	dsrc1_1 = dsrc1[1];
	dsrc1_2 = dsrc1[2];
	dsrc1_3 = dsrc1[3];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
		sum = sum0 + sum1 + sum2 + sum3;
		tmp = dsrc2[j + 4] = src2[j];
		sum0 = MUL_QK(dsrc1_3, dsrc2_1);
		sum1 = MUL_QK(dsrc1_2, dsrc2_2);
		sum2 = MUL_QK(dsrc1_1, dsrc2_3);
		sum3 = MUL_QK(dsrc1_0, tmp);

		dsrc2_1 = dsrc2_2;
		dsrc2_2 = dsrc2_3;
		dsrc2_3 = tmp;
		ddst[j] = sum;
	}

	dsrc2[n + 4] = dsrc2[n + 1 + 4] = dsrc2[n + 2 + 4] = 0;
	ddst[n] =  sum0 + sum1 + sum2 + sum3;
	ddst[n + 1] = MUL_QK(dsrc1_3, dsrc2_1) +
	    MUL_QK(dsrc1_2, dsrc2_2) + MUL_QK(dsrc1_1, dsrc2_3);
	ddst[n + 2] =  MUL_QK(dsrc1_3, dsrc2_2) + MUL_QK(dsrc1_2, dsrc2_3);
	ddst[n + 3] =  MUL_QK(dsrc1_3, dsrc2_3);

	for (i = n + 4; i < s; i++)
		ddst[i] = 0;

	for (i = 4; i + 4 < m; i += 4) {

		sum = ddst[i];
		sum0 = sum1 = sum2 = sum3 = 0;
		dsrc1_0 = dsrc1[i];
		dsrc1_1 = dsrc1[i + 1];
		dsrc1_2 = dsrc1[i + 2];
		dsrc1_3 = dsrc1[i + 3];
		dsrc2_1 = dsrc2[1];
		dsrc2_2 = dsrc2[2];
		dsrc2_3 = dsrc2[3];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < n + 4 - 1; j++) {
			sum +=  sum0 + sum1 + sum2 + sum3;
			tmp = dsrc2[j + 4];
			sum0 = MUL_QK(dsrc1_3, dsrc2_1);
			sum1 = MUL_QK(dsrc1_2, dsrc2_2);
			sum2 = MUL_QK(dsrc1_1, dsrc2_3);
			sum3 = MUL_QK(dsrc1_0, tmp);
			dsrc2_1 = dsrc2_2;
			dsrc2_2 = dsrc2_3;
			dsrc2_3 = tmp;
			ddst[i + j] = sum;
			sum = ddst[i + j + 1];
		}

		sum +=  sum0 + sum1 + sum2 + sum3;
		ddst[i + j] = sum;
	}

	if (i < m) {
		for (j = 0; j < i; j++) {
			tmpsum = ((ddst[j + 1] + (1 << (14 - k))) >> (15 - k));
			dst[j] = SATURATE_S16(tmpsum);
		}

		sum = ddst[i];
		sum0 = sum1 = sum2 = sum3 = 0;
		dsrc1_0 = dsrc1[i];
		dsrc1_1 = dsrc1[i + 1];
		dsrc1_2 = dsrc1[i + 2];
		dsrc1_3 = dsrc1[i + 3];
		dsrc2_1 = dsrc2[1];
		dsrc2_2 = dsrc2[2];
		dsrc2_3 = dsrc2[3];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < s - 1 - 4 - i; j++) {
			sum = sum + sum0 + sum1 + sum2 + sum3;
			tmp = dsrc2[j + 4];
			sum0 = MUL_QK(dsrc1_3, dsrc2_1);
			sum1 = MUL_QK(dsrc1_2, dsrc2_2);
			sum2 = MUL_QK(dsrc1_1, dsrc2_3);
			sum3 = MUL_QK(dsrc1_0, tmp);
			dsrc2_1 = dsrc2_2;
			dsrc2_2 = dsrc2_3;
			dsrc2_3 = tmp;
			tmpsum = (sum + (1 << (14 - k))) >> (15 - k);
			dst[i + j - 1] = SATURATE_S16(tmpsum);
			sum = ddst[i + j + 1];
		}

		sum = sum + sum0 + sum1 + sum2 + sum3;
		tmpsum = (sum + (1 << (14 - k))) >> (15 - k);
		dst[i + j - 1] = SATURATE_S16(tmpsum);
	} else {
		for (i = 0; i < s - 1 - 4; i++) {
			tmpsum = ((ddst[i + 1] + (1 << (14 - k))) >> (15 - k));
			dst[i] = SATURATE_S16(tmpsum);
		}
	}
}

/* *********************************************************** */

void
__mlib_SignalConvKernel_F32(
	mlib_f32 *dst,
	const mlib_f32 *src1,
	const mlib_f32 *src2,
	mlib_d64 *ddst,
	mlib_d64 *dsrc1,
	mlib_d64 *dsrc2,
	mlib_s32 m,
	mlib_s32 n)
{
	mlib_s32 i, j;
	mlib_d64 sum, sum0, sum1, sum2, sum3;
	mlib_d64 dsrc1_0, dsrc1_1, dsrc1_2, dsrc1_3;
	mlib_d64 tmp, dsrc2_1, dsrc2_2, dsrc2_3;
	mlib_s32 s = n + m + 4;

	for (i = 0; i < m; i++)
		dsrc1[i] = src1[i];

	for (i = m; i < m + 4; i++)
		dsrc1[i] = 0.;

	sum0 = sum1 = sum2 = sum3 = dsrc2_1 = dsrc2_2 = dsrc2_3 =
	    dsrc2[1] = dsrc2[2] = dsrc2[3] = 0.;
	dsrc1_0 = dsrc1[0];
	dsrc1_1 = dsrc1[1];
	dsrc1_2 = dsrc1[2];
	dsrc1_3 = dsrc1[3];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
		sum = sum0 + sum1 + sum2 + sum3;
		tmp = dsrc2[j + 4] = src2[j];
		sum0 = dsrc1_3 * dsrc2_1;
		sum1 = dsrc1_2 * dsrc2_2;
		sum2 = dsrc1_1 * dsrc2_3;
		sum3 = dsrc1_0 * tmp;
		dsrc2_1 = dsrc2_2;
		dsrc2_2 = dsrc2_3;
		dsrc2_3 = tmp;
		ddst[j] = sum;
	}

	dsrc2[n + 4] = dsrc2[n + 1 + 4] = dsrc2[n + 2 + 4] = 0.;
	ddst[n] = sum0 + sum1 + sum2 + sum3;
	ddst[n + 1] =
	    dsrc1_3 * dsrc2_1 + dsrc1_2 * dsrc2_2 + dsrc1_1 * dsrc2_3;
	ddst[n + 2] = dsrc1_3 * dsrc2_2 + dsrc1_2 * dsrc2_3;
	ddst[n + 3] = dsrc1_3 * dsrc2_3;

	for (i = n + 4; i < s; i++)
		ddst[i] = 0.;

	for (i = 4; i + 4 < m; i += 4) {

		sum = ddst[i];
		sum0 = sum1 = sum2 = sum3 = 0.;
		dsrc1_0 = dsrc1[i];
		dsrc1_1 = dsrc1[i + 1];
		dsrc1_2 = dsrc1[i + 2];
		dsrc1_3 = dsrc1[i + 3];
		dsrc2_1 = dsrc2[1];
		dsrc2_2 = dsrc2[2];
		dsrc2_3 = dsrc2[3];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < n + 4 - 1; j++) {
			sum += sum0 + sum1 + sum2 + sum3;
			tmp = dsrc2[j + 4];
			sum0 = dsrc1_3 * dsrc2_1;
			sum1 = dsrc1_2 * dsrc2_2;
			sum2 = dsrc1_1 * dsrc2_3;
			sum3 = dsrc1_0 * tmp;
			dsrc2_1 = dsrc2_2;
			dsrc2_2 = dsrc2_3;
			dsrc2_3 = tmp;
			ddst[i + j] = sum;
			sum = ddst[i + j + 1];
		}

		sum += sum0 + sum1 + sum2 + sum3;
		ddst[i + j] = sum;
	}

	if (i < m) {
		for (j = 0; j < i; j++)
			dst[j] = ddst[j + 1];

		sum = ddst[i];
		sum0 = sum1 = sum2 = sum3 = 0.;
		dsrc1_0 = dsrc1[i];
		dsrc1_1 = dsrc1[i + 1];
		dsrc1_2 = dsrc1[i + 2];
		dsrc1_3 = dsrc1[i + 3];
		dsrc2_1 = dsrc2[1];
		dsrc2_2 = dsrc2[2];
		dsrc2_3 = dsrc2[3];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < s - 1 - 4 - i; j++) {
			sum = (sum + sum0 + sum1 + sum2 + sum3);
			tmp = dsrc2[j + 4];
			sum0 = dsrc1_3 * dsrc2_1;
			sum1 = dsrc1_2 * dsrc2_2;
			sum2 = dsrc1_1 * dsrc2_3;
			sum3 = dsrc1_0 * tmp;
			dsrc2_1 = dsrc2_2;
			dsrc2_2 = dsrc2_3;
			dsrc2_3 = tmp;
			dst[i + j - 1] = sum;
			sum = ddst[i + j + 1];
		}

		sum = (sum + sum0 + sum1 + sum2 + sum3);
		dst[i + j - 1] = sum;
	} else {
		for (i = 0; i < s - 1 - 4; i++)
			dst[i] = ddst[i + 1];
	}
}

/* *********************************************************** */

void
__mlib_SignalConvKernel_F32S_a(
	const mlib_f32 *src2,
	mlib_d64 *ddst,
	mlib_d64 *dsrc1,
	mlib_d64 *dsrc2,
	mlib_s32 n,
	mlib_s32 i0,
	mlib_s32 i1)
{
	mlib_s32 j;
	mlib_d64 dsrc1_0, dsrc1_1, dsrc1_2, dsrc1_3, dsrc1_4;
	mlib_d64 dsrc1_5, dsrc1_6, dsrc1_7;
	mlib_d64 dsrc2_0, dsrc2_1, dsrc2_2, dsrc2_3;
	mlib_d64 dsrc2_4, dsrc2_5, dsrc2_6, dsrc2_7;
	mlib_s32 beg, end;
	mlib_d64 tt[6];

	dsrc1_0 = dsrc1[0];
	dsrc1_1 = dsrc1[1];
	dsrc1_2 = dsrc1[2];
	dsrc1_3 = dsrc1[3];
	dsrc1_4 = dsrc1[4];
	dsrc1_5 = dsrc1[5];
	dsrc1_6 = dsrc1[6];
	dsrc1_7 = dsrc1[7];

	SET_RANGE(0, n, 1);

	dsrc2_2 = dsrc2[2 + 2 * beg] = SRC2(2 * beg - 6);
	dsrc2_3 = dsrc2[3 + 2 * beg] = SRC2(2 * beg - 5);
	dsrc2_4 = dsrc2[4 + 2 * beg] = SRC2(2 * beg - 4);
	dsrc2_5 = dsrc2[5 + 2 * beg] = SRC2(2 * beg - 3);
	dsrc2_6 = dsrc2[6 + 2 * beg] = SRC2(2 * beg - 2);
	dsrc2_7 = dsrc2[7 + 2 * beg] = SRC2(2 * beg - 1);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = beg; j < end; j++) {
		ddst[2 * j + 2] =
		    dsrc1_6 * (dsrc2_2) + dsrc1_4 * (dsrc2_4) +
		    dsrc1_2 * (dsrc2_6) + dsrc1_0 * (dsrc2_0 =
		    dsrc2[2 * j + 8] = src2[2 * j]);
		dsrc2_2 = dsrc2_4;
		dsrc2_4 = dsrc2_6;
		dsrc2_6 = dsrc2_0;
		ddst[2 * j + 3] =
		    dsrc1_7 * (dsrc2_3) + dsrc1_5 * (dsrc2_5) +
		    dsrc1_3 * (dsrc2_7) + dsrc1_1 * (dsrc2_1 =
		    dsrc2[2 * j + 9] = src2[2 * j + 1]);
		dsrc2_3 = dsrc2_5;
		dsrc2_5 = dsrc2_7;
		dsrc2_7 = dsrc2_1;
	}

	SET_RANGE(0, 3, n + 1);

	if (end > beg) {
		tt[0] =
			dsrc1_6 * dsrc2_2 + dsrc1_4 * dsrc2_4 +
			dsrc1_2 * dsrc2_6;
		tt[1] =
		    dsrc1_7 * dsrc2_3 + dsrc1_5 * dsrc2_5 + dsrc1_3 * dsrc2_7;
		tt[2] = dsrc1_6 * dsrc2_4 + dsrc1_4 * dsrc2_6;
		tt[3] = dsrc1_7 * dsrc2_5 + dsrc1_5 * dsrc2_7;
		tt[4] = dsrc1_6 * dsrc2_6;
		tt[5] = dsrc1_7 * dsrc2_7;
		for (j = beg; j < end; j++) {
			ddst[2 * (n + j + 1)] = tt[2 * j];
			ddst[2 * (n + j + 1) + 1] = tt[2 * j + 1];
		}
	}
}

void __mlib_SignalConvKernel_F32S_b(
	mlib_f32 *dst,
	mlib_d64 *ddst,
	mlib_d64 *dsrc1,
	mlib_d64 *dsrc2,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 i0,
	mlib_s32 i1)
{
	mlib_s32 i, j, s = n + m + 4;
	mlib_d64 dsrc1_0, dsrc1_1, dsrc1_2, dsrc1_3, dsrc1_4;
	mlib_d64 dsrc1_5, dsrc1_6, dsrc1_7;
	mlib_s32 beg, end;

	SET_RANGE(n + 4, s, 0);
	for (i = beg; i < end; i++) {
		ddst[2 * i] = 0.;
		ddst[2 * i + 1] = 0.;
	}

	for (i = 4; i + 4 < m; i += 4) {
		SET_RANGE(0, n + 3, i + 1);

		dsrc1_0 = dsrc1[2 * i];
		dsrc1_1 = dsrc1[2 * i + 1];
		dsrc1_2 = dsrc1[2 * i + 2];
		dsrc1_3 = dsrc1[2 * i + 3];
		dsrc1_4 = dsrc1[2 * i + 4];
		dsrc1_5 = dsrc1[2 * i + 5];
		dsrc1_6 = dsrc1[2 * i + 6];
		dsrc1_7 = dsrc1[2 * i + 7];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = beg; j < end; j++) {
			ddst[2 * (i + j) + 2] +=
			    dsrc1_6 * dsrc2[2 * j + 2] +
			    dsrc1_4 * dsrc2[2 * j + 4] +
			    dsrc1_2 * dsrc2[2 * j + 6] +
			    dsrc1_0 * dsrc2[2 * j + 8];
			ddst[2 * (i + j) + 3] +=
			    dsrc1_7 * dsrc2[2 * j + 3] +
			    dsrc1_5 * dsrc2[2 * j + 5] +
			    dsrc1_3 * dsrc2[2 * j + 7] +
			    dsrc1_1 * dsrc2[2 * j + 9];
		}
	}

	if (i < m) {
		SET_RANGE(0, i + 1, 1);

		for (j = beg; j < end; j++) {
			dst[2 * j] = ddst[2 * j + 2];
			dst[2 * j + 1] = ddst[2 * j + 3];
		}

		dsrc1_0 = dsrc1[2 * i];
		dsrc1_1 = dsrc1[2 * i + 1];
		dsrc1_2 = dsrc1[2 * i + 2];
		dsrc1_3 = dsrc1[2 * i + 3];
		dsrc1_4 = dsrc1[2 * i + 4];
		dsrc1_5 = dsrc1[2 * i + 5];
		dsrc1_6 = dsrc1[2 * i + 6];
		dsrc1_7 = dsrc1[2 * i + 7];

		SET_RANGE(0, s - 4 - i - 1, i + 1);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = beg; j < end; j++) {

			dst[2 * (i + j)] =
			    (ddst[2 * (i + j) + 2] +
			    dsrc1_6 * dsrc2[2 * j + 2] +
			    dsrc1_4 * dsrc2[2 * j + 4] +
			    dsrc1_2 * dsrc2[2 * j + 6] +
			    dsrc1_0 * dsrc2[2 * j + 8]);

			dst[2 * (i + j) + 1] =
			    (ddst[2 * (i + j) + 3] +
			    dsrc1_7 * dsrc2[2 * j + 3] +
			    dsrc1_5 * dsrc2[2 * j + 5] +
			    dsrc1_3 * dsrc2[2 * j + 7] +
			    dsrc1_1 * dsrc2[2 * j + 9]);
		}
	} else {
		SET_RANGE(0, s - 5, 1);

		for (i = beg; i < end; i++) {
			dst[2 * i] = ddst[2 * i + 2];
			dst[2 * i + 1] = ddst[2 * i + 3];
		}
	}
}

void
__mlib_SignalConvKernel_F32S(
	mlib_f32 *dst,
	const mlib_f32 *src1,
	const mlib_f32 *src2,
	mlib_d64 *ddst,
	mlib_d64 *dsrc1,
	mlib_d64 *dsrc2,
	mlib_s32 m,
	mlib_s32 n)
{
	mlib_s32 i, j, s = n + m + 4;
	mlib_d64 dsrc1_0, dsrc1_1, dsrc1_2, dsrc1_3, dsrc1_4;
	mlib_d64 dsrc1_5, dsrc1_6, dsrc1_7;
	mlib_d64 dsrc2_0, dsrc2_1, dsrc2_2, dsrc2_3;
	mlib_d64 dsrc2_4, dsrc2_5, dsrc2_6, dsrc2_7;

	for (i = 0; i < 2 * m; i++)
		dsrc1[i] = src1[i];

	for (i = 2 * m; i < 2 * (m + 4); i++)
		dsrc1[i] = 0.;

	for (i = 2; i < 2 * 4; i++)
		dsrc2[i] = 0.;

	dsrc1_0 = dsrc1[0];
	dsrc1_1 = dsrc1[1];
	dsrc1_2 = dsrc1[2];
	dsrc1_3 = dsrc1[3];
	dsrc1_4 = dsrc1[4];
	dsrc1_5 = dsrc1[5];
	dsrc1_6 = dsrc1[6];
	dsrc1_7 = dsrc1[7];

	dsrc2_0 = dsrc2[0 + 2 * 4] = src2[0];
	dsrc2_1 = dsrc2[1 + 2 * 4] = src2[1];
	dsrc2_2 = dsrc2[2 + 2 * 4] = src2[2];
	dsrc2_3 = dsrc2[3 + 2 * 4] = src2[3];
	dsrc2_4 = dsrc2[4 + 2 * 4] = src2[4];
	dsrc2_5 = dsrc2[5 + 2 * 4] = src2[5];
	dsrc2_6 = dsrc2[6 + 2 * 4] = src2[6];
	dsrc2_7 = dsrc2[7 + 2 * 4] = src2[7];

	for (i = 2 * (n + 4); i < 2 * (n + 8); i++)
		dsrc2[i] = 0.;

	ddst[2] = dsrc1_0 * dsrc2_0;
	ddst[3] = dsrc1_1 * dsrc2_1;
	ddst[4] = dsrc1_2 * dsrc2_0 + dsrc1_0 * dsrc2_2;
	ddst[5] = dsrc1_3 * dsrc2_1 + dsrc1_1 * dsrc2_3;
	ddst[6] =
	    dsrc1_4 * dsrc2_0 + dsrc1_2 * dsrc2_2 + dsrc1_0 * dsrc2_4;
	ddst[7] =
	    dsrc1_5 * dsrc2_1 + dsrc1_3 * dsrc2_3 + dsrc1_1 * dsrc2_5;
	ddst[8] =
	    dsrc1_6 * dsrc2_0 + dsrc1_4 * dsrc2_2 + dsrc1_2 * dsrc2_4 +
	    dsrc1_0 * dsrc2_6;
	ddst[9] =
	    dsrc1_7 * dsrc2_1 + dsrc1_5 * dsrc2_3 + dsrc1_3 * dsrc2_5 +
	    dsrc1_1 * dsrc2_7;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 4; j < n; j++) {
		ddst[2 * j + 2] =
		    dsrc1_6 * (dsrc2_2) + dsrc1_4 * (dsrc2_4) +
		    dsrc1_2 * (dsrc2_6) + dsrc1_0 * (dsrc2_0 =
		    dsrc2[2 * j + 8] = src2[2 * j]);
		dsrc2_2 = dsrc2_4;
		dsrc2_4 = dsrc2_6;
		dsrc2_6 = dsrc2_0;
		ddst[2 * j + 3] =
		    dsrc1_7 * (dsrc2_3) + dsrc1_5 * (dsrc2_5) +
		    dsrc1_3 * (dsrc2_7) + dsrc1_1 * (dsrc2_1 =
		    dsrc2[2 * j + 9] = src2[2 * j + 1]);
		dsrc2_3 = dsrc2_5;
		dsrc2_5 = dsrc2_7;
		dsrc2_7 = dsrc2_1;
	}

	ddst[2 * n + 2] =
	    dsrc1_6 * dsrc2_2 + dsrc1_4 * dsrc2_4 + dsrc1_2 * dsrc2_6;
	ddst[2 * n + 3] =
	    dsrc1_7 * dsrc2_3 + dsrc1_5 * dsrc2_5 + dsrc1_3 * dsrc2_7;
	ddst[2 * n + 4] = dsrc1_6 * dsrc2_4 + dsrc1_4 * dsrc2_6;
	ddst[2 * n + 5] = dsrc1_7 * dsrc2_5 + dsrc1_5 * dsrc2_7;
	ddst[2 * n + 6] = dsrc1_6 * dsrc2_6;
	ddst[2 * n + 7] = dsrc1_7 * dsrc2_7;

	for (i = 2 * (n + 3) + 2; i < 2 * s; i++)
		ddst[i] = 0.;

	for (i = 4; i + 4 < m; i += 4) {
		dsrc1_0 = dsrc1[2 * i];
		dsrc1_1 = dsrc1[2 * i + 1];
		dsrc1_2 = dsrc1[2 * i + 2];
		dsrc1_3 = dsrc1[2 * i + 3];
		dsrc1_4 = dsrc1[2 * i + 4];
		dsrc1_5 = dsrc1[2 * i + 5];
		dsrc1_6 = dsrc1[2 * i + 6];
		dsrc1_7 = dsrc1[2 * i + 7];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < n + 3; j++) {
			ddst[2 * (i + j) + 2] +=
			    dsrc1_6 * dsrc2[2 * j + 2] +
			    dsrc1_4 * dsrc2[2 * j + 4] +
			    dsrc1_2 * dsrc2[2 * j + 6] +
			    dsrc1_0 * dsrc2[2 * j + 8];
			ddst[2 * (i + j) + 3] +=
			    dsrc1_7 * dsrc2[2 * j + 3] +
			    dsrc1_5 * dsrc2[2 * j + 5] +
			    dsrc1_3 * dsrc2[2 * j + 7] +
			    dsrc1_1 * dsrc2[2 * j + 9];
		}
	}

	if (i < m) {

		for (j = 0; j < 2 * i + 2; j++) {
			dst[j] = ddst[j + 2];
		}

		dsrc1_0 = dsrc1[2 * i];
		dsrc1_1 = dsrc1[2 * i + 1];
		dsrc1_2 = dsrc1[2 * i + 2];
		dsrc1_3 = dsrc1[2 * i + 3];
		dsrc1_4 = dsrc1[2 * i + 4];
		dsrc1_5 = dsrc1[2 * i + 5];
		dsrc1_6 = dsrc1[2 * i + 6];
		dsrc1_7 = dsrc1[2 * i + 7];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < s - i - 4 - 1; j++) {

			dst[2 * (i + j)] =
			    (ddst[2 * (i + j) + 2] +
			    dsrc1_6 * dsrc2[2 * j + 2] +
			    dsrc1_4 * dsrc2[2 * j + 4] +
			    dsrc1_2 * dsrc2[2 * j + 6] +
			    dsrc1_0 * dsrc2[2 * j + 8]);

			dst[2 * (i + j) + 1] =
			    (ddst[2 * (i + j) + 3] +
			    dsrc1_7 * dsrc2[2 * j + 3] +
			    dsrc1_5 * dsrc2[2 * j + 5] +
			    dsrc1_3 * dsrc2[2 * j + 7] +
			    dsrc1_1 * dsrc2[2 * j + 9]);
		}
	} else {
		for (i = 0; i < 2 * (s - 5); i++) {
			dst[i] = ddst[i + 2];
		}
	}
}

void
__mlib_SignalConvKernel_S16S_a(
	const mlib_s16 *src2,
	mlib_d64 *ddst,
	mlib_d64 *dsrc1,
	mlib_d64 *dsrc2,
	mlib_s32 n,
	mlib_s32 i0,
	mlib_s32 i1)
{
	mlib_s32 j;
	mlib_d64 dsrc1_0, dsrc1_1, dsrc1_2, dsrc1_3, dsrc1_4;
	mlib_d64 dsrc1_5, dsrc1_6, dsrc1_7;
	mlib_d64 dsrc2_0, dsrc2_1, dsrc2_2, dsrc2_3;
	mlib_d64 dsrc2_4, dsrc2_5, dsrc2_6, dsrc2_7;
	mlib_s32 beg, end;
	mlib_d64 tt[6];

	dsrc1_0 = dsrc1[0];
	dsrc1_1 = dsrc1[1];
	dsrc1_2 = dsrc1[2];
	dsrc1_3 = dsrc1[3];
	dsrc1_4 = dsrc1[4];
	dsrc1_5 = dsrc1[5];
	dsrc1_6 = dsrc1[6];
	dsrc1_7 = dsrc1[7];

	SET_RANGE(0, n, 1);

	dsrc2_2 = dsrc2[2 + 2 * beg] = SRC2(2 * beg - 6);
	dsrc2_3 = dsrc2[3 + 2 * beg] = SRC2(2 * beg - 5);
	dsrc2_4 = dsrc2[4 + 2 * beg] = SRC2(2 * beg - 4);
	dsrc2_5 = dsrc2[5 + 2 * beg] = SRC2(2 * beg - 3);
	dsrc2_6 = dsrc2[6 + 2 * beg] = SRC2(2 * beg - 2);
	dsrc2_7 = dsrc2[7 + 2 * beg] = SRC2(2 * beg - 1);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = beg; j < end; j++) {
		ddst[2 * j + 2] =
		    dsrc1_6 * (dsrc2_2) + dsrc1_4 * (dsrc2_4) +
		    dsrc1_2 * (dsrc2_6) + dsrc1_0 * (dsrc2_0 =
		    dsrc2[2 * j + 8] = src2[2 * j]);
		dsrc2_2 = dsrc2_4;
		dsrc2_4 = dsrc2_6;
		dsrc2_6 = dsrc2_0;
		ddst[2 * j + 3] =
		    dsrc1_7 * (dsrc2_3) + dsrc1_5 * (dsrc2_5) +
		    dsrc1_3 * (dsrc2_7) + dsrc1_1 * (dsrc2_1 =
		    dsrc2[2 * j + 9] = src2[2 * j + 1]);
		dsrc2_3 = dsrc2_5;
		dsrc2_5 = dsrc2_7;
		dsrc2_7 = dsrc2_1;
	}

	SET_RANGE(0, 3, n + 1);

	if (end > beg) {
		tt[0] =
			dsrc1_6 * dsrc2_2 + dsrc1_4 * dsrc2_4 +
			dsrc1_2 * dsrc2_6;
		tt[1] =
		    dsrc1_7 * dsrc2_3 + dsrc1_5 * dsrc2_5 + dsrc1_3 * dsrc2_7;
		tt[2] = dsrc1_6 * dsrc2_4 + dsrc1_4 * dsrc2_6;
		tt[3] = dsrc1_7 * dsrc2_5 + dsrc1_5 * dsrc2_7;
		tt[4] = dsrc1_6 * dsrc2_6;
		tt[5] = dsrc1_7 * dsrc2_7;
		for (j = beg; j < end; j++) {
			ddst[2 * (n + j + 1)] = tt[2 * j];
			ddst[2 * (n + j + 1) + 1] = tt[2 * j + 1];
		}
	}
}

void __mlib_SignalConvKernel_S16S_b(
	mlib_s16 *dst,
	mlib_d64 *ddst,
	mlib_d64 *dsrc1,
	mlib_d64 *dsrc2,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 i0,
	mlib_s32 i1)
{
	mlib_s32 i, j, s = n + m + 4;
	mlib_d64 dsrc1_0, dsrc1_1, dsrc1_2, dsrc1_3, dsrc1_4;
	mlib_d64 dsrc1_5, dsrc1_6, dsrc1_7;
	mlib_s32 beg, end;

	SET_RANGE(n + 4, s, 0);
	for (i = beg; i < end; i++) {
		ddst[2 * i] = 0.;
		ddst[2 * i + 1] = 0.;
	}

	for (i = 4; i + 4 < m; i += 4) {
		SET_RANGE(0, n + 3, i + 1);

		dsrc1_0 = dsrc1[2 * i];
		dsrc1_1 = dsrc1[2 * i + 1];
		dsrc1_2 = dsrc1[2 * i + 2];
		dsrc1_3 = dsrc1[2 * i + 3];
		dsrc1_4 = dsrc1[2 * i + 4];
		dsrc1_5 = dsrc1[2 * i + 5];
		dsrc1_6 = dsrc1[2 * i + 6];
		dsrc1_7 = dsrc1[2 * i + 7];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = beg; j < end; j++) {
			ddst[2 * (i + j) + 2] +=
			    dsrc1_6 * dsrc2[2 * j + 2] +
			    dsrc1_4 * dsrc2[2 * j + 4] +
			    dsrc1_2 * dsrc2[2 * j + 6] +
			    dsrc1_0 * dsrc2[2 * j + 8];
			ddst[2 * (i + j) + 3] +=
			    dsrc1_7 * dsrc2[2 * j + 3] +
			    dsrc1_5 * dsrc2[2 * j + 5] +
			    dsrc1_3 * dsrc2[2 * j + 7] +
			    dsrc1_1 * dsrc2[2 * j + 9];
		}
	}

	if (i < m) {
		SET_RANGE(0, i + 1, 1);

		for (j = beg; j < end; j++) {
			mlib_d64 sum0 = ddst[2 * j + 2] * 2.,
				sum1 = ddst[2 * j + 3] * 2.;
			SAT_S16(dst[2 * j], sum0);
			SAT_S16(dst[2 * j + 1], sum1);
		}

		dsrc1_0 = dsrc1[2 * i];
		dsrc1_1 = dsrc1[2 * i + 1];
		dsrc1_2 = dsrc1[2 * i + 2];
		dsrc1_3 = dsrc1[2 * i + 3];
		dsrc1_4 = dsrc1[2 * i + 4];
		dsrc1_5 = dsrc1[2 * i + 5];
		dsrc1_6 = dsrc1[2 * i + 6];
		dsrc1_7 = dsrc1[2 * i + 7];

		SET_RANGE(0, s - 4 - i - 1, i + 1);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = beg; j < end; j++) {
			mlib_d64 sum0, sum1;

			sum0 = 2. *
			    (ddst[2 * (i + j) + 2] +
			    dsrc1_6 * dsrc2[2 * j + 2] +
			    dsrc1_4 * dsrc2[2 * j + 4] +
			    dsrc1_2 * dsrc2[2 * j + 6] +
			    dsrc1_0 * dsrc2[2 * j + 8]);
			SAT_S16(dst[2 * (i + j)], sum0);

			sum1 = 2. *
			    (ddst[2 * (i + j) + 3] +
			    dsrc1_7 * dsrc2[2 * j + 3] +
			    dsrc1_5 * dsrc2[2 * j + 5] +
			    dsrc1_3 * dsrc2[2 * j + 7] +
			    dsrc1_1 * dsrc2[2 * j + 9]);
			SAT_S16(dst[2 * (i + j) + 1], sum1);
		}
	} else {
		SET_RANGE(0, s - 5, 1);

		for (i = beg; i < end; i++) {
			mlib_d64 sum0 = ddst[2 * i + 2] * 2.;
			mlib_d64 sum1 = ddst[2 * i + 3] * 2.;

			SAT_S16(dst[2 * i], sum0);
			SAT_S16(dst[2 * i + 1], sum1);
		}
	}
}

/* *********************************************************** */

void
__mlib_SignalConvKernel_S16S(
	mlib_s16 *dst,
	const mlib_s16 *src1,
	const mlib_s16 *src2,
	mlib_d64 *pdst,
	mlib_d64 *psrc1,
	mlib_d64 *psrc2,
	mlib_s32 m,
	mlib_s32 n)
{
	mlib_s32 tmpsum;
	mlib_s32 i, j, s = n + m + 4;
	mlib_s32 *ddst = (mlib_s32 *)pdst;
	mlib_s32 *dsrc1 = (mlib_s32 *)psrc1;
	mlib_s32 *dsrc2 = (mlib_s32 *)psrc2;
	mlib_s32 dsrc1_0, dsrc1_1, dsrc1_2, dsrc1_3, dsrc1_4;
	mlib_s32 dsrc1_5, dsrc1_6, dsrc1_7;
	mlib_s32 dsrc2_0, dsrc2_1, dsrc2_2, dsrc2_3;
	mlib_s32 dsrc2_4, dsrc2_5, dsrc2_6, dsrc2_7;

/* fk = 1 << (k - 1) (k=1, 2...); fk = 0 (k = 0) */
	mlib_s32 fk;
/* n <= 2 ^ k */
	mlib_s16 k;

	LOG2(n, k)
	fk = ((-k) >> 16) & (1 << (k - 1));

	for (i = 0; i < 2 * m; i++)
		dsrc1[i] = src1[i];

	for (i = 2 * m; i < 2 * (m + 4); i++)
		dsrc1[i] = 0;

	for (i = 2; i < 2 * 4; i++)
		dsrc2[i] = 0;

	dsrc1_0 = dsrc1[0];
	dsrc1_1 = dsrc1[1];
	dsrc1_2 = dsrc1[2];
	dsrc1_3 = dsrc1[3];
	dsrc1_4 = dsrc1[4];
	dsrc1_5 = dsrc1[5];
	dsrc1_6 = dsrc1[6];
	dsrc1_7 = dsrc1[7];

	dsrc2_0 = dsrc2[0 + 2 * 4] = src2[0];
	dsrc2_1 = dsrc2[1 + 2 * 4] = src2[1];
	dsrc2_2 = dsrc2[2 + 2 * 4] = src2[2];
	dsrc2_3 = dsrc2[3 + 2 * 4] = src2[3];
	dsrc2_4 = dsrc2[4 + 2 * 4] = src2[4];
	dsrc2_5 = dsrc2[5 + 2 * 4] = src2[5];
	dsrc2_6 = dsrc2[6 + 2 * 4] = src2[6];
	dsrc2_7 = dsrc2[7 + 2 * 4] = src2[7];

	for (i = 2 * (n + 4); i < 2 * (n + 8); i++)
		dsrc2[i] = 0;

	ddst[2] = MUL_QK(dsrc1_0, dsrc2_0);
	ddst[3] = MUL_QK(dsrc1_1, dsrc2_1);
	ddst[4] = MUL_QK(dsrc1_2, dsrc2_0) + MUL_QK(dsrc1_0, dsrc2_2);
	ddst[5] = MUL_QK(dsrc1_3, dsrc2_1) + MUL_QK(dsrc1_1, dsrc2_3);
	ddst[6] = MUL_QK(dsrc1_4, dsrc2_0) + MUL_QK(dsrc1_2, dsrc2_2) +
	    MUL_QK(dsrc1_0, dsrc2_4);
	ddst[7] = MUL_QK(dsrc1_5, dsrc2_1) + MUL_QK(dsrc1_3, dsrc2_3) +
	    MUL_QK(dsrc1_1, dsrc2_5);
	ddst[8] = MUL_QK(dsrc1_6, dsrc2_0) + MUL_QK(dsrc1_4, dsrc2_2) +
	    MUL_QK(dsrc1_2, dsrc2_4) + MUL_QK(dsrc1_0, dsrc2_6);
	ddst[9] = MUL_QK(dsrc1_7, dsrc2_1) + MUL_QK(dsrc1_5, dsrc2_3) +
	    MUL_QK(dsrc1_3, dsrc2_5) + MUL_QK(dsrc1_1, dsrc2_7);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 4; j < n; j++) {
		ddst[2 * j + 2] =
		    MUL_QK(dsrc1_6, (dsrc2_2)) + MUL_QK(dsrc1_4, (dsrc2_4)) +
		    MUL_QK(dsrc1_2, (dsrc2_6)) + MUL_QK(dsrc1_0, (dsrc2_0 =
		    dsrc2[2 * j + 8] = src2[2 * j]));
		dsrc2_2 = dsrc2_4;
		dsrc2_4 = dsrc2_6;
		dsrc2_6 = dsrc2_0;
		ddst[2 * j + 3] =
		    MUL_QK(dsrc1_7, (dsrc2_3)) + MUL_QK(dsrc1_5, (dsrc2_5)) +
		    MUL_QK(dsrc1_3, (dsrc2_7)) + MUL_QK(dsrc1_1, (dsrc2_1 =
		    dsrc2[2 * j + 9] = src2[2 * j + 1]));
		dsrc2_3 = dsrc2_5;
		dsrc2_5 = dsrc2_7;
		dsrc2_7 = dsrc2_1;
	}

	ddst[2 * n + 2] = MUL_QK(dsrc1_6, dsrc2_2) + MUL_QK(dsrc1_4, dsrc2_4) +
	    MUL_QK(dsrc1_2, dsrc2_6);
	ddst[2 * n + 3] = MUL_QK(dsrc1_7, dsrc2_3) + MUL_QK(dsrc1_5, dsrc2_5) +
	    MUL_QK(dsrc1_3, dsrc2_7);
	ddst[2 * n + 4] = MUL_QK(dsrc1_6, dsrc2_4) + MUL_QK(dsrc1_4, dsrc2_6);
	ddst[2 * n + 5] = MUL_QK(dsrc1_7, dsrc2_5) + MUL_QK(dsrc1_5, dsrc2_7);
	ddst[2 * n + 6] = MUL_QK(dsrc1_6, dsrc2_6);
	ddst[2 * n + 7] = MUL_QK(dsrc1_7, dsrc2_7);

	for (i = 2 * (n + 3) + 2; i < 2 * s; i++)
		ddst[i] = 0;

	for (i = 4; i + 4 < m; i += 4) {
		dsrc1_0 = dsrc1[2 * i];
		dsrc1_1 = dsrc1[2 * i + 1];
		dsrc1_2 = dsrc1[2 * i + 2];
		dsrc1_3 = dsrc1[2 * i + 3];
		dsrc1_4 = dsrc1[2 * i + 4];
		dsrc1_5 = dsrc1[2 * i + 5];
		dsrc1_6 = dsrc1[2 * i + 6];
		dsrc1_7 = dsrc1[2 * i + 7];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < n + 3; j++) {
			ddst[2 * (i + j) + 2] +=
			    MUL_QK(dsrc1_6, dsrc2[2 * j + 2]) +
			    MUL_QK(dsrc1_4, dsrc2[2 * j + 4]) +
			    MUL_QK(dsrc1_2, dsrc2[2 * j + 6]) +
			    MUL_QK(dsrc1_0, dsrc2[2 * j + 8]);
			ddst[2 * (i + j) + 3] +=
			    MUL_QK(dsrc1_7, dsrc2[2 * j + 3]) +
			    MUL_QK(dsrc1_5, dsrc2[2 * j + 5]) +
			    MUL_QK(dsrc1_3, dsrc2[2 * j + 7]) +
			    MUL_QK(dsrc1_1, dsrc2[2 * j + 9]);
		}
	}

	if (i < m) {
		for (j = 0; j < 2 * i + 2; j++) {
			tmpsum = (ddst[j + 2] + (1 << (14 - k))) >> (15 - k);
			dst[j] = SATURATE_S16(tmpsum);
		}

		dsrc1_0 = dsrc1[2 * i];
		dsrc1_1 = dsrc1[2 * i + 1];
		dsrc1_2 = dsrc1[2 * i + 2];
		dsrc1_3 = dsrc1[2 * i + 3];
		dsrc1_4 = dsrc1[2 * i + 4];
		dsrc1_5 = dsrc1[2 * i + 5];
		dsrc1_6 = dsrc1[2 * i + 6];
		dsrc1_7 = dsrc1[2 * i + 7];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < s - i - 4 - 1; j++) {
			mlib_s32 sum0, sum1;

			sum0 =
			    ((ddst[2 * (i + j) + 2] +
			    MUL_QK(dsrc1_6, dsrc2[2 * j + 2]) +
			    MUL_QK(dsrc1_4, dsrc2[2 * j + 4]) +
			    MUL_QK(dsrc1_2, dsrc2[2 * j + 6]) +
			    MUL_QK(dsrc1_0, dsrc2[2 * j + 8])) +
			    (1 << (14 - k))) >> (15 - k);
			dst[2 * (i + j)] = SATURATE_S16(sum0);

			sum1 =
			    ((ddst[2 * (i + j) + 3] +
			    MUL_QK(dsrc1_7, dsrc2[2 * j + 3]) +
			    MUL_QK(dsrc1_5, dsrc2[2 * j + 5]) +
			    MUL_QK(dsrc1_3, dsrc2[2 * j + 7]) +
			    MUL_QK(dsrc1_1, dsrc2[2 * j + 9])) +
			    (1 << (14 - k))) >> (15 - k);
			dst[2 * (i + j) + 1] = SATURATE_S16(sum1);
		}
	} else {
		for (i = 0; i < 2 * (s - 5); i++) {
			tmpsum = (ddst[i + 2] + (1 << (14 - k))) >> (15 - k);
			dst[i] = SATURATE_S16(tmpsum);
		}
	}
}

/* *********************************************************** */

mlib_status
__mlib_SignalConv_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    mlib_s32 m,
    mlib_s32 n)
{
	mlib_s32 a_src1[MLIB_FRAME + 2 * 4];
	mlib_s32 a_src2[MLIB_FRAME + 2 * 4];
	mlib_s32 a_dst[MLIB_FRAME * 2 + 4];
	mlib_s32 *dsrc1 = a_src1, *dsrc2 = a_src2;
	mlib_s32 *ddst = a_dst;
	mlib_s32 i, s, k, order;

	if ((m <= 0) || (n <= 0) || (!dst) || (!src1) || (!src2))
		return (MLIB_FAILURE);

	s = n + m - 1;
	k = 1;
	order = 0;
	while (k < s) {
		order++;
		k <<= 1;
	}

/* determine and serve shorter as response */

	if (n < m) {
		const mlib_s16 *swp = src1;

		src1 = src2;
		src2 = swp;
		i = n;
		n = m;
		m = i;
	}

	if (n * m > MAGIC_VALUE * k * order) {
		return mlib_fftConv_S16_S16_Sat(dst, (void *)src1, (void *)src2,
		    m, n);
	}

	s = n + m + 4;

	if ((m <= MLIB_FRAME) && (n <= MLIB_FRAME) &&
	    (s <= 2 * MLIB_FRAME + 4)) {
		mlib_SignalConvKernel_S16(dst, src1, src2, (mlib_d64 *)ddst,
		    (mlib_d64 *)dsrc1, (mlib_d64 *)dsrc2, m, n);
	} else {
		if (m > MLIB_FRAME) {
			dsrc1 = __mlib_malloc((m + 8) * sizeof (mlib_s32));

			if (dsrc1 == NULL)
				return (MLIB_FAILURE);
		}

		if (n > MLIB_FRAME) {
			dsrc2 = __mlib_malloc((n + 8) * sizeof (mlib_s32));

			if (dsrc2 == NULL) {
				if (m > MLIB_FRAME)
					__mlib_free(dsrc1);
				return (MLIB_FAILURE);
			}
		}

		if (s > 2 * MLIB_FRAME + 4) {
			ddst = __mlib_malloc((s) * sizeof (mlib_s32));

			if (ddst == NULL) {
				if (m > MLIB_FRAME)
					__mlib_free(dsrc1);

				if (n > MLIB_FRAME)
					__mlib_free(dsrc2);
				return (MLIB_FAILURE);
			}
		}

		mlib_SignalConvKernel_S16(dst, src1, src2, (mlib_d64 *)ddst,
		    (mlib_d64 *)dsrc1, (mlib_d64 *)dsrc2, m, n);

		if (m > MLIB_FRAME)
			__mlib_free(dsrc1);

		if (n > MLIB_FRAME)
			__mlib_free(dsrc2);

		if (s > 2 * MLIB_FRAME + 4)
			__mlib_free(ddst);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConv_S16S_S16S_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    mlib_s32 m,
    mlib_s32 n)
{
	mlib_s32 a_src1[2 * MLIB_FRAME + 8];
	mlib_s32 a_src2[2 * MLIB_FRAME + 16];
	mlib_s32 a_dst[4 * MLIB_FRAME + 8];
	mlib_s32 *dsrc1 = a_src1, *dsrc2 = a_src2;
	mlib_s32 *ddst = a_dst;
	mlib_s32 i, j, s, k, order;

	if ((m <= 0) || (n <= 0) || (!dst) || (!src1) || (!src2))
		return (MLIB_FAILURE);

	s = n + m - 1;
	k = 1;
	order = 0;
	while (k < s) {
		order++;
		k <<= 1;
	}

/* determine and serve shorter as response */

	if (n < m) {
		const mlib_s16 *swp = src1;

		src1 = src2;
		src2 = swp;
		i = n;
		n = m;
		m = i;
	}

	if (n < 4) {
/* mlib_s16 arrays case, m<=n<4, done simple & slow */
		s = n + m - 1;

		for (i = 0; i < s; i++) {
			mlib_s32 o, l;
			mlib_s32 sum1 = 0;
			mlib_s32 sum2 = 0;

			if (i < n)
				l = i;
			else
				l = n - 1;

			if (i < m)
				o = 0;
			else
				o = i - m + 1;

			for (j = o; j <= l; j++) {
				sum1 += (mlib_s32)src2[2 * j] *
				    src1[2 * (i - j)];
			}
			sum1 = (sum1 + 0x4000) >> 15;
			dst[2 * i] = SATURATE_S16(sum1);

			for (j = o; j <= l; j++) {
				sum2 += (mlib_s32)src2[2 * j + 1] *
				    src1[(2 * (i - j)) + 1];
			}
			sum2 = (sum2 + 0x4000) >> 15;
			dst[2 * i + 1] = SATURATE_S16(sum2);
		}

		return (MLIB_SUCCESS);
	}

	if (n * m > MAGIC_VALUE * k * order) {
		return mlib_fftConv_S16S_S16S_Sat(dst, (void *)src1,
		    (void *)src2, m, n);
	}

	s = n + m + 4;

	if ((m <= MLIB_FRAME) && (n <= MLIB_FRAME) &&
	    (s <= 2 * MLIB_FRAME + 8)) {
		mlib_SignalConvKernel_S16S(dst, src1, src2,
		    (mlib_d64 *)ddst, (mlib_d64 *)dsrc1, (mlib_d64 *)dsrc2,
		    m, n);
	} else {
		if (m > MLIB_FRAME) {
			dsrc1 = __mlib_malloc(2 * (m + 8) * sizeof (mlib_s32));

			if (dsrc1 == NULL)
				return (MLIB_FAILURE);
		}

		if (n > MLIB_FRAME) {
			dsrc2 = __mlib_malloc(2 * (n + 8) * sizeof (mlib_s32));

			if (dsrc2 == NULL) {
				if (m > MLIB_FRAME)
					__mlib_free(dsrc1);
				return (MLIB_FAILURE);
			}
		}

		if (s > 2 * MLIB_FRAME + 8) {
			ddst = __mlib_malloc(2 * (s) * sizeof (mlib_s32));

			if (ddst == NULL) {
				if (m > MLIB_FRAME)
					__mlib_free(dsrc1);

				if (n > MLIB_FRAME)
					__mlib_free(dsrc2);
				return (MLIB_FAILURE);
			}
		}

		mlib_SignalConvKernel_S16S(dst, src1, src2,
		    (mlib_d64 *)ddst, (mlib_d64 *)dsrc1, (mlib_d64 *)dsrc2,
		    m, n);
		if (m > MLIB_FRAME)
			__mlib_free(dsrc1);

		if (n > MLIB_FRAME)
			__mlib_free(dsrc2);

		if (s > 2 * MLIB_FRAME + 8)
			__mlib_free(ddst);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConv_F32_F32(
    mlib_f32 *dst,
    const mlib_f32 *src1,
    const mlib_f32 *src2,
    mlib_s32 m,
    mlib_s32 n)
{
	mlib_d64 a_src1[MLIB_FRAME + 4], a_src2[MLIB_FRAME + 8],
	    a_dst[MLIB_FRAME * 2 + 4];
	mlib_d64 *dsrc1 = a_src1, *dsrc2 = a_src2, *ddst = a_dst;
	mlib_s32 i, s, k, order;

	if ((m <= 0) || (n <= 0) || (!dst) || (!src1) || (!src2))
		return (MLIB_FAILURE);

	s = n + m - 1;
	k = 1;
	order = 0;
	while (k < s) {
		order++;
		k <<= 1;
	}

/* determine and serve shorter as response */

	if (n < m) {
		const mlib_f32 *swp = src1;

		src1 = src2;
		src2 = swp;
		i = n;
		n = m;
		m = i;
	}

	if (n * m > MAGIC_VALUE * k * order)
		return mlib_fftConv_F32_F32(dst, (void *)src1, (void *)src2, m,
		    n);

	s = n + m + 4;

	if ((m <= MLIB_FRAME) && (n <= MLIB_FRAME) &&
	    (s <= 2 * MLIB_FRAME + 4)) {
		mlib_SignalConvKernel_F32(dst, src1, src2,
			ddst, dsrc1, dsrc2, m, n);
	} else {
		if (m > MLIB_FRAME) {
			dsrc1 = __mlib_malloc((m + 8) * sizeof (mlib_d64));

			if (dsrc1 == NULL)
				return (MLIB_FAILURE);
		}

		if (n > MLIB_FRAME) {
			dsrc2 = __mlib_malloc((n + 8) * sizeof (mlib_d64));

			if (dsrc2 == NULL) {
				if (m > MLIB_FRAME)
					__mlib_free(dsrc1);
				return (MLIB_FAILURE);
			}
		}

		if (s > 2 * MLIB_FRAME + 4) {
			ddst = __mlib_malloc((s) * sizeof (mlib_d64));

			if (ddst == NULL) {
				if (m > MLIB_FRAME)
					__mlib_free(dsrc1);

				if (n > MLIB_FRAME)
					__mlib_free(dsrc2);
				return (MLIB_FAILURE);
			}
		}

		mlib_SignalConvKernel_F32(dst, src1, src2,
			ddst, dsrc1, dsrc2, m, n);

		if (m > MLIB_FRAME)
			__mlib_free(dsrc1);

		if (n > MLIB_FRAME)
			__mlib_free(dsrc2);

		if (s > 2 * MLIB_FRAME + 4)
			__mlib_free(ddst);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConv_F32S_F32S(
    mlib_f32 *dst,
    const mlib_f32 *src1,
    const mlib_f32 *src2,
    mlib_s32 m,
    mlib_s32 n)
{
	mlib_d64 a_src1[2 * MLIB_FRAME + 8], a_src2[2 * MLIB_FRAME + 16],
	    a_dst[4 * MLIB_FRAME + 8];
	mlib_d64 *dsrc1 = a_src1, *dsrc2 = a_src2, *ddst = a_dst;
	mlib_s32 i, j, s, k, order;

	if ((m <= 0) || (n <= 0) || (!dst) || (!src1) || (!src2))
		return (MLIB_FAILURE);

	s = n + m - 1;
	k = 1;
	order = 0;
	while (k < s) {
		order++;
		k <<= 1;
	}

/* determine and serve shorter as response */

	if (n < m) {
		const mlib_f32 *swp = src1;

		src1 = src2;
		src2 = swp;
		i = n;
		n = m;
		m = i;
	}

	if (n < 4) {
/* mlib_s16 arrays case, m<=n<4, done simple & slow */
		s = n + m - 1;
		for (i = 0; i < s; i++) {
			mlib_s32 o, l;
			mlib_d64 sum = 0.;

			if (i < n)
				l = i;
			else
				l = n - 1;

			if (i < m)
				o = 0;
			else
				o = i - m + 1;

			for (j = o; j <= l; j++)
				sum +=
				    (mlib_d64)src2[2 * j] * src1[2 * (i - j)];

			dst[2 * i] = sum;

			sum = 0.;
			for (j = o; j <= l; j++)
				sum +=
				    (mlib_d64)src2[2 * j + 1] * src1[(2 * (i -
				    j)) + 1];

			dst[2 * i + 1] = sum;
		}

		return (MLIB_SUCCESS);
	}

	if (n * m > MAGIC_VALUE * k * order)
		return mlib_fftConv_F32S_F32S(dst, (void *)src1, (void *)src2,
		    m, n);

	s = n + m + 4;

	if ((m <= MLIB_FRAME) && (n <= MLIB_FRAME) &&
	    (s <= 2 * MLIB_FRAME + 8)) {
		mlib_SignalConvKernel_F32S(dst, src1, src2,
			ddst, dsrc1, dsrc2, m, n);
	} else {
		if (m > MLIB_FRAME) {
			dsrc1 = __mlib_malloc(2 * (m + 8) * sizeof (mlib_d64));

			if (dsrc1 == NULL)
				return (MLIB_FAILURE);
		}

		if (n > MLIB_FRAME) {
			dsrc2 = __mlib_malloc(2 * (n + 8) * sizeof (mlib_d64));

			if (dsrc2 == NULL) {
				if (m > MLIB_FRAME)
					__mlib_free(dsrc1);
				return (MLIB_FAILURE);
			}
		}

		if (s > 2 * MLIB_FRAME + 8) {
			ddst = __mlib_malloc(2 * (s) * sizeof (mlib_d64));

			if (ddst == NULL) {
				if (m > MLIB_FRAME)
					__mlib_free(dsrc1);

				if (n > MLIB_FRAME)
					__mlib_free(dsrc2);
				return (MLIB_FAILURE);
			}
		}
		mlib_SignalConvKernel_F32S(dst, src1, src2,
			ddst, dsrc1, dsrc2, m, n);

		if (m > MLIB_FRAME)
			__mlib_free(dsrc1);

		if (n > MLIB_FRAME)
			__mlib_free(dsrc2);

		if (s > 2 * MLIB_FRAME + 8)
			__mlib_free(ddst);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

static mlib_status
mlib_fftConv_F32_F32(
    mlib_f32 *dst,
    mlib_f32 *src1,
    mlib_f32 *src2,
    mlib_s32 m,
    mlib_s32 n)
{
	mlib_d64 z;
	mlib_s32 i, s;
	mlib_s32 k, order;
	mlib_d64 a_src1[MLIB_FRAME * 4];
	mlib_d64 *psrc1 = a_src1, *pdst1;

/* fft way */

	switch (m) {
	case 0:
		return (MLIB_SUCCESS);
	case 1:
		z = (mlib_d64)src1[0];
		for (i = 0; i < n; i++) {
			dst[i] = z * src2[i];
		}

		return (MLIB_SUCCESS);
	case 2: {
		    mlib_d64 mult0 = src1[0], mult1 = src1[1];

		    dst[0] = mult0 * src2[0];
		    for (i = 1; i < n; i++) {
			    dst[i] = mult0 * src2[i] + mult1 * src2[i - 1];
		    }

		    dst[n] = mult1 * src2[n - 1];
	    }

		return (MLIB_SUCCESS);
	}

	s = n + m - 1;
	for (order = 0, k = 1; k < s; ) {
		order++;
		k <<= 1;
	}

	if (k > MLIB_FRAME / 2) {
		if (!(psrc1 = __mlib_malloc(2 * 4 * k * sizeof (mlib_d64))))
			return (MLIB_FAILURE);
	}

	pdst1 = psrc1 + 4 * k;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < m; i++) {
		psrc1[2 * i] = src1[i];
		psrc1[2 * i + 1] = src2[i];
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n; i++) {
		psrc1[2 * i] = 0;
		psrc1[2 * i + 1] = src2[i];
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < k; i++) {
		psrc1[2 * i] = psrc1[2 * i + 1] = 0;
	}

	mlib_conv_fft(pdst1, psrc1, order, 1.0);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < s; i++) {
		dst[i] = pdst1[i];
	}

	if (k > MLIB_FRAME / 2)
		__mlib_free(psrc1);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

static mlib_status
mlib_fftConv_F32S_F32S(
    mlib_f32 *dst,
    mlib_f32 *src1,
    mlib_f32 *src2,
    mlib_s32 m,
    mlib_s32 n)
{
	mlib_d64 z, z1;
	mlib_s32 i, s;
	mlib_s32 k, order;
	mlib_d64 a_src1[MLIB_FRAME * 4];
	mlib_d64 *psrc1 = a_src1, *pdst1;

/* fft way */

	switch (m) {
	case 0:
		return (MLIB_SUCCESS);

	case 1:
		z = (mlib_d64)src1[0];
		z1 = (mlib_d64)src1[1];
		for (i = 0; i < n; i++) {
			dst[2 * i] = z * src2[2 * i];
			dst[2 * i + 1] = z1 * src2[2 * i + 1];
		}

		return (MLIB_SUCCESS);

	case 2: {
		    mlib_d64 mult0 = src1[0], mult1 = src1[2];
		    mlib_d64 mult01 = src1[1], mult11 = src1[3];

		    dst[0] = mult0 * src2[0];
		    dst[1] = mult01 * src2[1];
		    for (i = 1; i < n; i++) {
			    dst[2 * i] =
				mult0 * src2[2 * i] + mult1 * src2[2 * (i - 1)];
			    dst[2 * i + 1] =
				mult01 * src2[2 * i + 1] +
				mult11 * src2[2 * (i - 1) + 1];
		    }

		    dst[2 * n] = mult1 * src2[2 * (n - 1)];
		    dst[2 * n + 1] = mult11 * src2[2 * (n - 1) + 1];
	    }

		return (MLIB_SUCCESS);
	}

	s = n + m - 1;
	for (order = 0, k = 1; k < s; ) {
		order++;
		k <<= 1;
	}

	if (k > MLIB_FRAME / 2) {
		if (!(psrc1 = __mlib_malloc(2 * 4 * k * sizeof (mlib_d64))))
			return (MLIB_FAILURE);
	}

	pdst1 = psrc1 + 4 * k;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < m; i++) {
		psrc1[2 * i] = src1[2 * i];
		psrc1[2 * i + 1] = src2[2 * i];
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n; i++) {
		psrc1[2 * i] = 0;
		psrc1[2 * i + 1] = src2[2 * i];
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < k; i++) {
		psrc1[2 * i] = psrc1[2 * i + 1] = 0;
	}

	mlib_conv_fft(pdst1, psrc1, order, 1.0);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < s; i++) {
		dst[2 * i] = pdst1[i];
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < m; i++) {
		psrc1[2 * i] = src1[2 * i + 1];
		psrc1[2 * i + 1] = src2[2 * i + 1];
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n; i++) {
		psrc1[2 * i] = 0;
		psrc1[2 * i + 1] = src2[2 * i + 1];
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < k; i++) {
		psrc1[2 * i] = psrc1[2 * i + 1] = 0;
	}

	mlib_conv_fft(pdst1, psrc1, order, 1.0);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < s; i++) {
		dst[2 * i + 1] = pdst1[i];
	}

	if (k > MLIB_FRAME / 2)
		__mlib_free(psrc1);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_conv_fft(
    deal_t *dst_main,
    deal_t *src_main,
    mlib_s32 order,
    mlib_d64 scale)
{
/* we come here with order >=3 */
	deal_t *pal64 = dst_main;
	mlib_s32 i, j, ii, jj, n = 1 << order, nn2 = n + n;
	deal_t *fft1 = pal64, *fft2 = fft1 + nn2;
	deal_t wr, wi, wpr, wpi, wtemp;

	scale /= (1 << (2 + order));

	wtemp = SINUS(order);
	wpr = 1. - 2. * wtemp * wtemp;
	wpi = SINUS(order - 1);
	wr = wpr;
	wi = wpi;

	mlib_fftDispatcher_D64C_D64C(pal64, NULL, src_main, NULL, order, order,
	    DIRECT_FFT);

	j = 2;
	jj = j + n - 4;
	i = nn2 - 2;
	ii = i - n + 4;

	{
		mlib_s32 j0 = j, jj0 = jj, j1 = j0 + 1, jj1 = jj0 + 1;
		deal_t ff0, ff1, ff2, ff3, a0, b0, c0, d0;
		deal_t a1, b1, c1, d1, d1_r, d1_i, d2_r, d2_i;
		deal_t h1r, h1i, h2r, h2i, wwr, wwi;

		a0 = fft1[j];
		b0 = fft1[i];
		c0 = fft1[j + 1];
		d0 = fft1[i + 1];
		d1_r = a0 * c0 + b0 * d0;
		d1_i = c0 * c0 - d0 * d0 + b0 * b0 - a0 * a0;
		a1 = fft1[jj];
		b1 = fft1[ii];
		c1 = fft1[jj + 1];
		d1 = fft1[ii + 1];
		d2_r = a1 * c1 + b1 * d1;
		d2_i = c1 * c1 - d1 * d1 + b1 * b1 - a1 * a1;
		h1r = 2. * (d1_r + d2_r);
		h1i = d1_i - d2_i;
		h2r = -d1_i - d2_i;
		h2i = 2. * (d1_r - d2_r);
		wwr = wr * h2r - wi * h2i;
		wwi = wr * h2i + wi * h2r;
		ff0 = scale * (h1r + wwr);
		ff1 = scale * (h1i + wwi);
		ff2 = scale * (h1r - wwr);
		ff3 = scale * (wwi - h1i);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j < n / 2; i -= 2, jj -= 2, ii += 2) {
			a0 = fft1[j];
			b0 = fft1[i];
			c0 = fft1[j + 1];
			d0 = fft1[i + 1];
			d1_r = a0 * c0 + b0 * d0;
			d1_i = c0 * c0 - d0 * d0 + b0 * b0 - a0 * a0;
			a1 = fft1[jj];
			b1 = fft1[ii];
			c1 = fft1[jj + 1];
			d1 = fft1[ii + 1];
			d2_r = a1 * c1 + b1 * d1;
			d2_i = c1 * c1 - d1 * d1 + b1 * b1 - a1 * a1;
			fft2[j0] = ff0;
			h1r = 2. * (d1_r + d2_r);
			h1i = d1_i - d2_i;
			fft2[j1] = ff1;
			h2r = -d1_i - d2_i;
			h2i = 2. * (d1_r - d2_r);
			fft2[jj0] = ff2;
			wwr = wr * h2r - wi * h2i;
			wwi = wr * h2i + wi * h2r;
			fft2[jj1] = ff3;
			ff0 = scale * (h1r + wwr);
			ff1 = scale * (h1i + wwi);
			ff2 = scale * (h1r - wwr);
			ff3 = scale * (wwi - h1i);
			wr = (wtemp = wr) * wpr - wi * wpi;
			wi = wi * wpr + wtemp * wpi;
			j1 = (j0 = j) + 1;
			jj1 = (jj0 = jj) + 1;
			j += 2;
		}

		fft2[j0] = ff0;
		fft2[j1] = ff1;
		fft2[jj0] = ff2;
		fft2[jj1] = ff3;
	}

	i = nn2 - (j = n / 2);

	{
		deal_t a = fft1[j], b = fft1[i], c = fft1[j + 1], d =
		    fft1[i + 1];

		fft2[n / 2] = 4. * scale * (a * c + b * d);
		fft2[n / 2 + 1] = -2. * scale * (c * c - d * d + b * b - a * a);
	}

	fft2[0] = 4. * scale * ((wr = fft1[0] * fft1[1])
	    + (wi = fft1[n] * fft1[n + 1]));
	fft2[1] = 4. * scale * (wr - wi);

	mlib_fftDispatcher_D64C_D64C(fft1, NULL, fft2, NULL, order - 1,
	    order - 1, INVERS_FFT);
}

/* *********************************************************** */
