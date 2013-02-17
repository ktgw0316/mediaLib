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

#pragma ident	"@(#)mlib_SignalMulRectangular_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulRectangular_F32 - multiply source data on
 *          coefficients of the corresponding window
 *
 *      mlib_SignalMulRectangular_F32 - multiply stereo source
 *          data on coefficients of the corresponding window
 *
 *      mlib_SignalMulRectangular_F32_F32 - multiply source data on
 *          coefficients of the corresponding window with results
 *          in destination array
 *
 *      mlib_SignalMulRectangular_F32S_F32S - multiply stereo source
 *          data on coefficients of the corresponding window with
 *          result in destination array
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalMulRectangular_F32(mlib_f32 *data,
 *                                                mlib_s32 m,
 *                                                mlib_s32 n);
 *      mlib_status mlib_SignalMulRectangular_F32S(mlib_f32 *data,
 *                                                 mlib_s32 m,
 *                                                 mlib_s32 n);
 *      mlib_status mlib_SignalMulRectangular_F32S_F32S(mlib_f32       *dst,
 *                                                      const mlib_f32 *src,
 *                                                      mlib_s32       m,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_SignalMulRectangular_F32_F32(mlib_f32       *dst,
 *                                                    const mlib_f32 *src,
 *                                                    mlib_s32       m,
 *                                                    mlib_s32       n);
 * ARGUMENTS
 *      data     Input and output signal array
 *      dst      Output signal array
 *      src      Input signal array
 *      m        Rectangular window parameter (m <= n)
 *      n        Length of window array
 *
 * DESCRIPTION
 *      Mono        : dst[i] = src[i] * window[i]    i = 0..(n-1)
 *      Stereo      : dst[i] = src[i] * window[i/2]  i = 0..(2*n-1)
 *      Rectangular : window[j] = (j < m) ? 1 : 0    j = 0..(n-1)
 */

#include <mlib_signal.h>
#include <mlib_SysMath.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulRectangular_F32 = __mlib_SignalMulRectangular_F32
#pragma weak mlib_SignalMulRectangular_F32S = \
	__mlib_SignalMulRectangular_F32S
#pragma weak mlib_SignalMulRectangular_F32S_F32S = \
	__mlib_SignalMulRectangular_F32S_F32S
#pragma weak mlib_SignalMulRectangular_F32_F32 = \
	__mlib_SignalMulRectangular_F32_F32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulRectangular_F32) mlib_SignalMulRectangular_F32
    __attribute__((weak, alias("__mlib_SignalMulRectangular_F32")));
__typeof__(__mlib_SignalMulRectangular_F32S) mlib_SignalMulRectangular_F32S
    __attribute__((weak, alias("__mlib_SignalMulRectangular_F32S")));
__typeof__(__mlib_SignalMulRectangular_F32S_F32S)
    mlib_SignalMulRectangular_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalMulRectangular_F32S_F32S")));
__typeof__(__mlib_SignalMulRectangular_F32_F32)
    mlib_SignalMulRectangular_F32_F32
    __attribute__((weak, alias("__mlib_SignalMulRectangular_F32_F32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef _NO_LONGLONG

typedef union
{
	mlib_d64 db;
	struct
	{
#ifdef _LITTLE_ENDIAN
		mlib_f32 i1, i0;
#else		   /* _LITTLE_ENDIAN */
		mlib_f32 i0, i1;
#endif		   /* _LITTLE_ENDIAN */
	} fl;
} mlib_union64;

#ifdef _LITTLE_ENDIAN
#define	FL0(x)	x.fl.i1
#define	FL1(x)	x.fl.i0
#else /* _LITTLE_ENDIAN */
#define	FL0(x)	x.fl.i0
#define	FL1(x)	x.fl.i1
#endif /* _LITTLE_ENDIAN */
#endif /* _NO_LONGLONG */

/* *********************************************************** */

mlib_status
__mlib_SignalMulRectangular_F32(
    mlib_f32 *data,
    mlib_s32 m,
    mlib_s32 n)
{
	mlib_s32 zero_len = n - m;

	if ((data == NULL) || (m < 0) || (n <= 0) || (m > n))
		return (MLIB_FAILURE);

	if (zero_len <= 0)
		return (MLIB_SUCCESS);
	data += m;

	if ((mlib_addr)data & 7) {
		(*data++) = 0;
		zero_len--;
	}

	for (; zero_len > 1; zero_len -= 2) {
		*(mlib_d64 *)data = 0;
		data += 2;
	}

	if (zero_len > 0)
		*data = 0;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulRectangular_F32_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 m,
    mlib_s32 n)
{
	mlib_s32 zero_len = n - m;

#ifdef _NO_LONGLONG
	mlib_union64 val;
#else /* _NO_LONGLONG */
	mlib_u64 val, x0, x1;
#endif /* _NO_LONGLONG */

	if ((dst == NULL) || (m < 0) || (n <= 0) || (m > n) || (src == NULL))
		return (MLIB_FAILURE);

	if (m == 0)
		goto clean;

	if ((mlib_addr)dst & 7) {
		(*dst++) = (*src++);
		m--;
	}

	if ((((mlib_addr)dst ^ (mlib_addr)src) & 7) == 0) {

		for (; m > 1; m -= 2) {
			*(mlib_d64 *)dst = *(mlib_d64 *)src;
			dst += 2;
			src += 2;
		}

		if (m > 0)
			(*dst++) = (*src++);
	} else {

#ifdef _NO_LONGLONG
		for (; m > 1; m -= 2) {
			FL0(val) = src[0];
			FL1(val) = src[1];
			*(mlib_d64 *)dst = val.db;
			dst += 2;
			src += 2;
		}

#else /* _NO_LONGLONG */
		for (; m > 1; m -= 2) {
			x0 = *(mlib_u32 *)(src + 0);
			x1 = *(mlib_u32 *)(src + 1);
#ifdef _LITTLE_ENDIAN
			val = (x1 << 32) | x0;
#else /* _LITTLE_ENDIAN */
			val = (x0 << 32) | x1;
#endif /* _LITTLE_ENDIAN */
			*(mlib_u64 *)dst = val;
			dst = (void *)((mlib_u8 *)dst + 8);
			src = (void *)((mlib_u8 *)src + 8);
		}

#endif /* _NO_LONGLONG */

		if (m > 0)
			(*dst++) = (*src++);
	}

	clean:

	if (zero_len <= 0)
		return (MLIB_SUCCESS);

	if ((mlib_addr)dst & 7) {
		(*dst++) = 0;
		zero_len--;
	}

	for (; zero_len > 1; zero_len -= 2) {
		*(mlib_d64 *)dst = 0;
		dst += 2;
	}

	if (zero_len > 0)
		*dst = 0;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulRectangular_F32S(
    mlib_f32 *data,
    mlib_s32 m,
    mlib_s32 n)
{
	return (__mlib_SignalMulRectangular_F32(data, m + m, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulRectangular_F32S_F32S(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 m,
    mlib_s32 n)
{
	return (__mlib_SignalMulRectangular_F32_F32(dst, src, m + m, n + n));
}

/* *********************************************************** */
