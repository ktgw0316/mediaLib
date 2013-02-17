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

#pragma ident	"@(#)mlib_SignalLimit_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalLimit_[F32|F32S]_[F32|F32S],
 *      mlib_SignalLimit_[F32|F32S]             - hard limiting
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalLimit_F32(mlib_f32       *data,
 *                                       const mlib_f32 *low,
 *                                       const mlib_f32 *high,
 *                                       mlib_s32       n);
 *      mlib_status mlib_SignalLimit_F32S(mlib_f32       *data,
 *                                        const mlib_f32 *low,
 *                                        const mlib_f32 *high,
 *                                        mlib_s32       n);
 *      mlib_status mlib_SignalLimit_F32S_F32S(mlib_f32       *dst,
 *                                             const mlib_f32 *src,
 *                                             const mlib_f32 *low,
 *                                             const mlib_f32 *high,
 *                                             mlib_s32       n);
 *      mlib_status mlib_SignalLimit_F32_F32(mlib_f32       *dst,
 *                                           const mlib_f32 *src,
 *                                           const mlib_f32 *low,
 *                                           const mlib_f32 *high,
 *                                           mlib_s32       n);
 * ARGUMENTS
 *      data  Input and output signal array
 *      dst   Output signal array
 *      src   Input signal array
 *      low   Lower input limit
 *      high  Upper input limit
 *      n     Number of samples in the input signal arrays.
 *
 * DESCRIPTION
 *      dst[i] = { low    , if src[i] < low
 *                 src[i] , if low <= src[i] < high
 *                 high   , if src[i] >= high
 *               }
 *
 */

#include <mlib_signal.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalLimit_F32 = __mlib_SignalLimit_F32
#pragma weak mlib_SignalLimit_F32S = __mlib_SignalLimit_F32S
#pragma weak mlib_SignalLimit_F32S_F32S = __mlib_SignalLimit_F32S_F32S
#pragma weak mlib_SignalLimit_F32_F32 = __mlib_SignalLimit_F32_F32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalLimit_F32) mlib_SignalLimit_F32
    __attribute__((weak, alias("__mlib_SignalLimit_F32")));
__typeof__(__mlib_SignalLimit_F32S) mlib_SignalLimit_F32S
    __attribute__((weak, alias("__mlib_SignalLimit_F32S")));
__typeof__(__mlib_SignalLimit_F32S_F32S) mlib_SignalLimit_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalLimit_F32S_F32S")));
__typeof__(__mlib_SignalLimit_F32_F32) mlib_SignalLimit_F32_F32
    __attribute__((weak, alias("__mlib_SignalLimit_F32_F32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_SignalLimit_F32_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    const mlib_f32 *low,
    const mlib_f32 *high,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_f32 tl0;
	mlib_f32 th0;
	mlib_f32 x;

	tl0 = low[0];
	th0 = high[0];

	if ((tl0 > th0) || (n <= 0))
		return (MLIB_FAILURE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		x = src[i];
		x = (x < tl0) ? tl0 : x;
		x = (x >= th0) ? th0 : x;
		dst[i] = x;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLimit_F32(
    mlib_f32 *data,
    const mlib_f32 *low,
    const mlib_f32 *high,
    mlib_s32 n)
{
	return (__mlib_SignalLimit_F32_F32(data, data, low, high, n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalLimit_F32S_F32S(
    mlib_f32 *dst,
    const mlib_f32 *src,
    const mlib_f32 *low,
    const mlib_f32 *high,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_f32 tl0, tl1;
	mlib_f32 th0, th1;
	mlib_f32 x, x1;

	tl0 = low[0];
	th0 = high[0];
	tl1 = low[1];
	th1 = high[1];

	if ((tl0 > th0) || (tl1 > th1) || (n <= 0))
		return (MLIB_FAILURE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		x = (*src++);
		x = (x < tl0) ? tl0 : x;
		x = (x >= th0) ? th0 : x;
		(*dst++) = x;
		x1 = (*src++);
		x1 = (x1 < tl1) ? tl1 : x1;
		x1 = (x1 >= th1) ? th1 : x1;
		(*dst++) = x1;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLimit_F32S(
    mlib_f32 *data,
    const mlib_f32 *low,
    const mlib_f32 *high,
    mlib_s32 n)
{
	return (__mlib_SignalLimit_F32S_F32S(data, data, low, high, n));
}

/* *********************************************************** */
