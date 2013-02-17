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

#pragma ident	"@(#)mlib_SignalMulBartlett_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulBartlett_F32 - multiply source data on
 *          coefficients of the corresponding window
 *
 *      mlib_SignalMulBartlett_F32S - multiply stereo source
 *          data on coefficients of the corresponding window
 *
 *      mlib_SignalMulBartlett_F32_F32 - multiply source data on
 *          coefficients of the corresponding window with results
 *          in destination array
 *
 *      mlib_SignalMulBartlett_F32S_F32S - multiply stereo source
 *          data on coefficients of the corresponding window with
 *          result in destination array
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalMulBartlett_F32(mlib_f32 *data,
 *                                             mlib_s32 n);
 *      mlib_status mlib_SignalMulBartlett_F32S(mlib_f32 *data,
 *                                              mlib_s32 n);
 *      mlib_status mlib_SignalMulBartlett_F32S_F32S(mlib_f32       *dst,
 *                                                   const mlib_f32 *src,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_SignalMulBartlett_F32_F32(mlib_f32       *dst,
 *                                                 const mlib_f32 *src,
 *                                                 mlib_s32       n);
 * ARGUMENTS
 *      data     Input and output signal array
 *      dst      Output signal array
 *      src      Input signal array
 *      n        Length of window array
 *
 * DESCRIPTION
 *
 *      Mono        :   dst[i] = src[i] * window[i]    i = 0..(N-1)
 *
 *      Stereo      :   dst[i] = src[i] * window[i/2]  i = 0..(2*N-1)
 *
 *      Bartlett    :   window[i] = ((i <= (N - 1)/2) ? (2*i/(N -1)) :
 *                        (2 - (2 * i)/(N-1)))
 *
 */

#include <mlib_signal.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulBartlett_F32 = __mlib_SignalMulBartlett_F32
#pragma weak mlib_SignalMulBartlett_F32S = __mlib_SignalMulBartlett_F32S
#pragma weak mlib_SignalMulBartlett_F32S_F32S = \
	__mlib_SignalMulBartlett_F32S_F32S
#pragma weak mlib_SignalMulBartlett_F32_F32 = \
	__mlib_SignalMulBartlett_F32_F32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulBartlett_F32) mlib_SignalMulBartlett_F32
    __attribute__((weak, alias("__mlib_SignalMulBartlett_F32")));
__typeof__(__mlib_SignalMulBartlett_F32S) mlib_SignalMulBartlett_F32S
    __attribute__((weak, alias("__mlib_SignalMulBartlett_F32S")));
__typeof__(__mlib_SignalMulBartlett_F32S_F32S) mlib_SignalMulBartlett_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalMulBartlett_F32S_F32S")));
__typeof__(__mlib_SignalMulBartlett_F32_F32) mlib_SignalMulBartlett_F32_F32
    __attribute__((weak, alias("__mlib_SignalMulBartlett_F32_F32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_SignalMulBartlett_F32S(
    mlib_f32 *data,
    mlib_s32 n)
{
	mlib_d64 step, base;
	mlib_s32 n2, n1, i, j0, j1, e0, e1;
	mlib_d64 src0, src1, src2, src3;

	if ((data == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	e1 = (n - 1) << 1;
	e0 = e1 + 1;
	j0 = 0;
	j1 = 1;
	n1 = (n - 1);
	n2 = (n1) >> 1;
	base = 0;
	step = 2. / n1;

	src0 = 0;
	src1 = 0;
	src2 = 0;
	src3 = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n2; i++) {
		data[j0] = (mlib_f32)(src0);
		j0 += 2;
		src0 = (mlib_d64)data[j0];
		data[j1] = (mlib_f32)(base * src1);
		j1 += 2;
		src1 = (mlib_d64)data[j1];
		data[e1] = (mlib_f32)(base * src2);
		data[e0] = (mlib_f32)(base * src3);
		base += step;
		src0 *= base;
		e1 -= 2;
		src2 = (mlib_d64)data[e1];
		e0 -= 2;
		src3 = (mlib_d64)data[e0];
	}

	data[j0] = (mlib_f32)src0;
	data[j1] = (mlib_f32)base *src1;
	data[e1] = (mlib_f32)base *src2;
	data[e0] = (mlib_f32)base *src3;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulBartlett_F32_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 n)
{
	mlib_d64 step, base;
	mlib_s32 n2, n1, i;

	if ((dst == NULL) || (n <= 1) || (src == NULL))
		return (MLIB_FAILURE);

	n1 = n - 1;
	n2 = (n1 >> 1);
	base = 0;
	step = 2. / n1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= n2; i++) {
		mlib_d64 dat1 = (mlib_d64)src[i];
		mlib_d64 dat2 = (mlib_d64)src[n1 - i];
		dst[i] = (mlib_f32)base *dat1;
		dst[n1 - i] = (mlib_f32)base *dat2;

		base += step;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulBartlett_F32S_F32S(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 n)
{
	mlib_d64 step, base;
	mlib_s32 n2, n1, i, j, e0, e1;

	if ((dst == NULL) || (n <= 1) || (src == NULL))
		return (MLIB_FAILURE);

	e1 = (n - 1) << 1;
	e0 = e1 + 1;
	n1 = (n - 1);
	n2 = (n1) >> 1;
	base = 0;
	step = 2. / n1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0, j = 0; i <= n2; i++, j += 2) {
		mlib_d64 src0 = (mlib_d64)src[j];
		mlib_d64 src1 = (mlib_d64)src[j + 1];
		mlib_d64 src2 = (mlib_d64)src[e1 - j];
		mlib_d64 src3 = (mlib_d64)src[e0 - j];
		dst[j] = (mlib_f32)base *src0;
		dst[j + 1] = (mlib_f32)base *src1;
		dst[e1 - j] = (mlib_f32)base *src2;
		dst[e0 - j] = (mlib_f32)base *src3;

		base += step;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulBartlett_F32(
    mlib_f32 *data,
    mlib_s32 n)
{
	mlib_d64 step, base;
	mlib_s32 n2, n1, i;

	if ((data == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	n1 = n - 1;
	n2 = (n1 >> 1);
	base = 0;
	step = 2. / n1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= n2; i++) {
		mlib_d64 dat1 = (mlib_d64)data[i];
		mlib_d64 dat2 = (mlib_d64)data[n1 - i];

		data[i] = (mlib_f32)(base * dat1);
		data[n1 - i] = (mlib_f32)(base * dat2);
		base += step;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
