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

#pragma ident	"@(#)mlib_SignalQuant_Fp.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalQuant_[U8|U8S]_[F32|F32S]   - floating point 32-bit to 8-bit
 *                                               Quantization
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalQuant_U8S_F32S(mlib_u8        *dst,
 *                                            const mlib_f32 *src,
 *                                            const mlib_f32 *thresh,
 *                                            mlib_s32       n);
 *      mlib_status mlib_SignalQuant_U8_F32(mlib_u8        *dst,
 *                                          const mlib_f32 *src,
 *                                          const mlib_f32 *thresh,
 *                                          mlib_s32       n);
 *
 * ARGUMENTS
 *      dst      Output signal array
 *      src      Input signal array
 *      thresh   Array of 256 thresholds
 *      n        Number of samples in the input signal array
 *
 * DESCRIPTION
 *
 *      X(n)  = x(n), n = 0, 1, ...
 *      Z(n)  = z(n), n = 0, 1, ...
 *
 *            =   0,  x(n) < t(0)
 *            =   k,  t(k) <= x(n) < t(k+1)
 *            = 255,  x(n) >= t(255)
 */

/*
 * FUNCTIONS
 *      mlib_SignalQuant_[S16|S16S]_[F32|F32S] - floating point 32-bit to 16-bit
 *                                               Quantization
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalQuant_S16S_F32S(mlib_s16       *dst,
 *                                             const mlib_f32 *src,
 *                                             const mlib_f32 *thresh,
 *                                             mlib_s32       n);
 *      mlib_status mlib_SignalQuant_S16_F32(mlib_s16       *dst,
 *                                           const mlib_f32 *src,
 *                                           const mlib_f32 *thresh,
 *                                           mlib_s32       n);
 * ARGUMENTS
 *      dst      Output signal array
 *      src      Input signal array
 *      thresh   Array of 65536 thresholds
 *      n        Number of samples in the input signal array
 *
 * DESCRIPTION
 *
 *      X(n)  = x(n), n = 0, 1, ...
 *      Z(n)  = z(n), n = 0, 1, ...
 *
 *            = -32768,  x(n) <  t(-32768)
 *            =      k,  t(k) <= x(n) < t(k+1)
 *            = +32767,  x(n) >= t(+32767)
 */

/*
 * FUNCTIONS
 *      mlib_SignalQuant2_[S16|S16S]_[F32|F32S] - floating point
 *                                                32-bit to 16-bit
 *                                                Quantization
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalQuant2_S16S_F32S(mlib_s16       *dst,
 *                                              const mlib_f32 *src,
 *                                              const mlib_f32 *thresh,
 *                                              mlib_s32       length,
 *                                              mlib_s16       offset,
 *                                              mlib_s32       n);
 *      mlib_status mlib_SignalQuant2_S16_F32(mlib_s16       *dst,
 *                                            const mlib_f32 *src,
 *                                            const mlib_f32 *thresh,
 *                                            mlib_s32       length,
 *                                            mlib_s16       offset,
 *                                            mlib_s32       n);
 *
 * ARGUMENTS
 *      dst      Output signal array
 *      src      Input signal array
 *      thresh   Array of thresholds
 *      length   Length of the array of thresholds
 *      offset   Offset for thresholds
 *      n        Number of samples in the input signal array
 *
 * DESCRIPTION
 *
 *      X(n)  = x(n), n = 0, 1, ...
 *      Z(n)  = z(n), n = 0, 1, ...
 *
 *            = offset,               x(n) <  t(0)
 *            = offset + k,           t(k) <= x(n) < t(k+1)
 *            = offset + length - 1,  x(n) >= t(length - 1)
 */

#include <mlib_signal.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalQuant2_S16S_F32S = __mlib_SignalQuant2_S16S_F32S
#pragma weak mlib_SignalQuant2_S16_F32 = __mlib_SignalQuant2_S16_F32
#pragma weak mlib_SignalQuant_S16S_F32S = __mlib_SignalQuant_S16S_F32S
#pragma weak mlib_SignalQuant_S16_F32 = __mlib_SignalQuant_S16_F32
#pragma weak mlib_SignalQuant_U8S_F32S = __mlib_SignalQuant_U8S_F32S
#pragma weak mlib_SignalQuant_U8_F32 = __mlib_SignalQuant_U8_F32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalQuant2_S16S_F32S) mlib_SignalQuant2_S16S_F32S
    __attribute__((weak, alias("__mlib_SignalQuant2_S16S_F32S")));
__typeof__(__mlib_SignalQuant2_S16_F32) mlib_SignalQuant2_S16_F32
    __attribute__((weak, alias("__mlib_SignalQuant2_S16_F32")));
__typeof__(__mlib_SignalQuant_S16S_F32S) mlib_SignalQuant_S16S_F32S
    __attribute__((weak, alias("__mlib_SignalQuant_S16S_F32S")));
__typeof__(__mlib_SignalQuant_S16_F32) mlib_SignalQuant_S16_F32
    __attribute__((weak, alias("__mlib_SignalQuant_S16_F32")));
__typeof__(__mlib_SignalQuant_U8S_F32S) mlib_SignalQuant_U8S_F32S
    __attribute__((weak, alias("__mlib_SignalQuant_U8S_F32S")));
__typeof__(__mlib_SignalQuant_U8_F32) mlib_SignalQuant_U8_F32
    __attribute__((weak, alias("__mlib_SignalQuant_U8_F32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CONV(x)	x = x ^ ((x >> 31) & 0x7FFFFFFF)

/* *********************************************************** */

#ifdef _NO_LONGLONG

#define	Q_STEP(N)                                               \
	ti = *(mlib_s32 *)th;                                   \
	CONV(ti);                                               \
	th += (si < ti) ? -(1 << N) : (1 << N)

#else /* _NO_LONGLONG */

#define	Q_STEP(N)                                               \
	ti = *(mlib_s32 *)th;                                   \
	CONV(ti);                                               \
	mask = ((mlib_s64)si - (mlib_s64)ti) >> 63;             \
	mask = ((1 << N) ^ mask) - mask;                        \
	th += mask

#endif /* _NO_LONGLONG */

/* *********************************************************** */

mlib_status
__mlib_SignalQuant_U8_F32(
    mlib_u8 *dst,
    const mlib_f32 *src,
    const mlib_f32 *thresh,
    mlib_s32 n)
{
	mlib_s32 i, si, ti, mask;
	mlib_u8 *th;

	if (n <= 0) {
		return (MLIB_FAILURE);
	}

	if (dst == NULL || src == NULL || thresh == NULL) {
		return (MLIB_NULLPOINTER);
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		th = (mlib_u8 *)thresh + 512;
		si = ((mlib_s32 *)src)[i];
		CONV(si);
		Q_STEP(8);
		Q_STEP(7);
		Q_STEP(6);
		Q_STEP(5);
		Q_STEP(4);
		Q_STEP(3);
		Q_STEP(2);
		Q_STEP(1);
		dst[i] = (mlib_s32)(th - ((mlib_u8 *)thresh + 2)) >> 2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalQuant_U8S_F32S(
    mlib_u8 *dst,
    const mlib_f32 *src,
    const mlib_f32 *thresh,
    mlib_s32 n)
{
	return (__mlib_SignalQuant_U8_F32(dst, src, thresh, 2 * n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalQuant_S16_F32(
    mlib_s16 *dst,
    const mlib_f32 *src,
    const mlib_f32 *thresh,
    mlib_s32 n)
{
	mlib_s32 i, si, ti, mask;
	mlib_u8 *th;

	if (n <= 0) {
		return (MLIB_FAILURE);
	}

	if (dst == NULL || src == NULL || thresh == NULL) {
		return (MLIB_NULLPOINTER);
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		th = (mlib_u8 *)thresh;
		si = ((mlib_s32 *)src)[i];
		CONV(si);
		Q_STEP(16);
		Q_STEP(15);
		Q_STEP(14);
		Q_STEP(13);
		Q_STEP(12);
		Q_STEP(11);
		Q_STEP(10);
		Q_STEP(9);
		Q_STEP(8);
		Q_STEP(7);
		Q_STEP(6);
		Q_STEP(5);
		Q_STEP(4);
		Q_STEP(3);
		Q_STEP(2);
		Q_STEP(1);
		dst[i] = (mlib_s32)(th - ((mlib_u8 *)thresh + 2)) >> 2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalQuant_S16S_F32S(
    mlib_s16 *dst,
    const mlib_f32 *src,
    const mlib_f32 *thresh,
    mlib_s32 n)
{
	return (__mlib_SignalQuant_S16_F32(dst, src, thresh, 2 * n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalQuant2_S16_F32(
    mlib_s16 *dst,
    const mlib_f32 *src,
    const mlib_f32 *thresh,
    mlib_s32 length,
    mlib_s16 offset,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_s32 a, b, c, c_old;
	mlib_f32 s0;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (dst == NULL || src == NULL || thresh == NULL)
		return (MLIB_NULLPOINTER);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		s0 = src[i];
		a = 0;
		b = length - 1;
		c = (a + b) / 2;
		do {
			c_old = c;

			if (s0 < thresh[c]) {
				b = c;
				c = (a + b) / 2;
			} else {
				a = c;
				c = (a + b) / 2;
			}

		} while (c != c_old);

		if (thresh[a] > s0) {
			c = a;
		}

		if (thresh[b] < s0) {
			c = b;
		}

		dst[i] = c + offset;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalQuant2_S16S_F32S(
    mlib_s16 *dst,
    const mlib_f32 *src,
    const mlib_f32 *thresh,
    mlib_s32 length,
    mlib_s16 offset,
    mlib_s32 n)
{
	return __mlib_SignalQuant2_S16_F32(dst, src, thresh, length, offset,
	    2 * n);
}

/* *********************************************************** */
