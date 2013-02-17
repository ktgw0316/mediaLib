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

#pragma ident	"@(#)mlib_SignalMulS_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulS_[F32|F32S],
 *      mlib_SignalMulS_[F32|F32S]_[F32|F32S] - multiplication of
 *                     mlib_f32 format signal array to a scalar
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalMulS_F32(mlib_f32       *data,
 *                                      const mlib_f32 *c,
 *                                      mlib_s32       n);
 *      mlib_status mlib_SignalMulS_F32S(mlib_f32       *data,
 *                                       const mlib_f32 *c,
 *                                       mlib_s32       n);
 *      mlib_status mlib_SignalMulS_F32S_F32S(mlib_f32       *dst,
 *                                            const mlib_f32 *src,
 *                                            const mlib_f32 *c,
 *                                            mlib_s32       n);
 *      mlib_status mlib_SignalMulS_F32_F32(mlib_f32       *dst,
 *                                          const mlib_f32 *src,
 *                                          const mlib_f32 *c,
 *                                          mlib_s32       n);
 * ARGUMENTS
 *      data  Input and output signal array
 *      dst   Output signal array
 *      src   Input signal array
 *      c     Scaling factor.
 *      n     Number of samples in the input signal arrays.
 *
 * DESCRIPTION
 *      dst = src * c
 */

#include <mlib_signal.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulS_F32 = __mlib_SignalMulS_F32
#pragma weak mlib_SignalMulS_F32S = __mlib_SignalMulS_F32S
#pragma weak mlib_SignalMulS_F32S_F32S = __mlib_SignalMulS_F32S_F32S
#pragma weak mlib_SignalMulS_F32_F32 = __mlib_SignalMulS_F32_F32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulS_F32) mlib_SignalMulS_F32
    __attribute__((weak, alias("__mlib_SignalMulS_F32")));
__typeof__(__mlib_SignalMulS_F32S) mlib_SignalMulS_F32S
    __attribute__((weak, alias("__mlib_SignalMulS_F32S")));
__typeof__(__mlib_SignalMulS_F32S_F32S) mlib_SignalMulS_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalMulS_F32S_F32S")));
__typeof__(__mlib_SignalMulS_F32_F32) mlib_SignalMulS_F32_F32
    __attribute__((weak, alias("__mlib_SignalMulS_F32_F32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

typedef union
{
	mlib_d64 d64;
	struct
	{
		mlib_f32 i0, i1;
	} f32x2;
} mlib_union64;

/* *********************************************************** */

mlib_status
__mlib_SignalMulS_F32_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    const mlib_f32 *c,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_f32 scalar = *c;

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((mlib_addr)src & 7) {
		dst[0] = src[0] * scalar;
		dst++;
		src++;
		n--;
	}

	if ((mlib_addr)dst & 7) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 1; i++) {
			mlib_union64 uni;

			uni.d64 = ((mlib_d64 *)src)[i];
			dst[2 * i] = uni.f32x2.i0 * scalar;
			dst[2 * i + 1] = uni.f32x2.i1 * scalar;
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 1; i++) {
			mlib_union64 uni, dst0;

			uni.d64 = ((mlib_d64 *)src)[i];
			dst0.f32x2.i0 = uni.f32x2.i0 * scalar;
			dst0.f32x2.i1 = uni.f32x2.i1 * scalar;
			((mlib_d64 *)dst)[i] = dst0.d64;
		}
	}

	for (i <<= 1; i < n; i++)
		dst[i] = src[i] * scalar;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulS_F32(
    mlib_f32 *data,
    const mlib_f32 *c,
    mlib_s32 n)
{
	return (__mlib_SignalMulS_F32_F32(data, data, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulS_F32S_F32S(
    mlib_f32 *dst,
    const mlib_f32 *src,
    const mlib_f32 *c,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_f32 sc0 = *c, sc1 = *(c + 1);
	mlib_f32 scalar0, scalar1;
	mlib_s32 flag = 0;

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((mlib_addr)src & 7) {
		dst[0] = src[0] * sc0;
		dst++;
		src++;
		scalar0 = sc1;
		scalar1 = sc0;
		flag = 1;
		n--;
	} else {
		scalar0 = sc0;
		scalar1 = sc1;
	}

	if ((mlib_addr)dst & 7) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			mlib_union64 uni;

			uni.d64 = ((mlib_d64 *)src)[i];
			dst[2 * i] = uni.f32x2.i0 * scalar0;
			dst[2 * i + 1] = uni.f32x2.i1 * scalar1;
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			mlib_union64 uni, dst0;

			uni.d64 = ((mlib_d64 *)src)[i];
			dst0.f32x2.i0 = uni.f32x2.i0 * scalar0;
			dst0.f32x2.i1 = uni.f32x2.i1 * scalar1;
			((mlib_d64 *)dst)[i] = dst0.d64;
		}
	}

	if (flag) {
		dst[2 * i] = src[2 * i] * sc1;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulS_F32S(
    mlib_f32 *data,
    const mlib_f32 *c,
    mlib_s32 n)
{
	return (__mlib_SignalMulS_F32S_F32S(data, data, c, n));
}

/* *********************************************************** */
