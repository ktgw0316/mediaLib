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

#pragma ident	"@(#)mlib_c_SignalQuant.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalQuant_[U8|U8S]_[S16|S16S] - 16-Bit to 8-Bit
 *                                             quantization
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalQuant_U8S_S16S(mlib_u8        *dst,
 *                                            const mlib_s16 *src,
 *                                            const mlib_s16 *thresh,
 *                                            mlib_s32       n);
 *      mlib_status mlib_SignalQuant_U8_S16(mlib_u8        *dst,
 *                                          const mlib_s16 *src,
 *                                          const mlib_s16 *thresh,
 *                                          mlib_s32       n);
 *
 * ARGUMENTS
 *      dst      Output signal array
 *      src      Input signal array
 *      thresh   Array of 256 thresholds
 *      n        Number of samples in the input signal array
 *
 * DESCRIPTION
 *      X(n)  = x(n), n = 0, 1, ...
 *      Z(n)  = z(n), n = 0, 1, ...
 *
 *            =   0,  x(n) < t(0)
 *            =   k,  t(k) <= x(n) < t(k+1)
 *            = 255,  x(n) >= t(255)
 */

#include <mlib_signal.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalQuant_U8S_S16S = __mlib_SignalQuant_U8S_S16S
#pragma weak mlib_SignalQuant_U8_S16 = __mlib_SignalQuant_U8_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalQuant_U8S_S16S) mlib_SignalQuant_U8S_S16S
    __attribute__((weak, alias("__mlib_SignalQuant_U8S_S16S")));
__typeof__(__mlib_SignalQuant_U8_S16) mlib_SignalQuant_U8_S16
    __attribute__((weak, alias("__mlib_SignalQuant_U8_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	LOAD_S16(ind)	*((mlib_s16 *) ((mlib_u8 *) thresh + ind))

/* *********************************************************** */

mlib_status
__mlib_SignalQuant_U8_S16(
    mlib_u8 *dst,
    const mlib_s16 *src,
    const mlib_s16 *thresh,
    mlib_s32 n)
{
	mlib_s32 i, k;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (dst == NULL || src == NULL || thresh == NULL)
		return (MLIB_NULLPOINTER);

	for (i = 0; i < n; i++) {
		k = 384 + (((src[i] - LOAD_S16(256)) >> 31) & ~0xff);
		k += 64 + (((src[i] - LOAD_S16(k)) >> 31) & ~0x7f);
		k += 32 + (((src[i] - LOAD_S16(k)) >> 31) & ~0x3f);
		k += 16 + (((src[i] - LOAD_S16(k)) >> 31) & ~0x1f);
		k += 8 + (((src[i] - LOAD_S16(k)) >> 31) & ~0xf);
		k += 4 + (((src[i] - LOAD_S16(k)) >> 31) & ~7);
		k += 2 + (((src[i] - LOAD_S16(k)) >> 31) & ~3);
		dst[i] = (k >> 1) + ((src[i] - LOAD_S16(k)) >> 31);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalQuant_U8S_S16S(
    mlib_u8 *dst,
    const mlib_s16 *src,
    const mlib_s16 *thresh,
    mlib_s32 n)
{
	return (__mlib_SignalQuant_U8_S16(dst, src, thresh, 2 * n));
}

/* *********************************************************** */
