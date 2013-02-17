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

#pragma ident	"@(#)mlib_c_SignalMulS.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulS_[S16|S16S]_Sat,
 *      mlib_SignalMulS_[S16|S16S]_[S16|S16S]_Sat - multiplication of
 *                     mlib_s32 16-bit format signal array to a scalar
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalMulS_S16S_S16S_Sat(mlib_s16       *dst,
 *                                                const mlib_s16 *src,
 *                                                const mlib_s16 *c,
 *                                                mlib_s32       n);
 *      mlib_status mlib_SignalMulS_S16S_Sat(mlib_s16       *data,
 *                                           const mlib_s16 *c,
 *                                           mlib_s32       n);
 *      mlib_status mlib_SignalMulS_S16_S16_Sat(mlib_s16       *dst,
 *                                              const mlib_s16 *src,
 *                                              const mlib_s16 *c,
 *                                              mlib_s32       n);
 *      mlib_status mlib_SignalMulS_S16_Sat(mlib_s16       *data,
 *                                          const mlib_s16 *c,
 *                                          mlib_s32       n);
 * ARGUMENTS
 *      data  Input and output signal array
 *      dst   Output signal array
 *      src   Input signal array
 *      c     Scaling factor. The scaling factor is in Q15 format
 *      n     Number of samples in the input signal arrays.
 *
 * DESCRIPTION
 *      dst = src * c
 */

#include <mlib_signal.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulS_S16S_S16S_Sat = __mlib_SignalMulS_S16S_S16S_Sat
#pragma weak mlib_SignalMulS_S16_S16_Sat = __mlib_SignalMulS_S16_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulS_S16S_S16S_Sat) mlib_SignalMulS_S16S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalMulS_S16S_S16S_Sat")));
__typeof__(__mlib_SignalMulS_S16_S16_Sat) mlib_SignalMulS_S16_S16_Sat
    __attribute__((weak, alias("__mlib_SignalMulS_S16_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(X)	((mlib_s32)(X))

#else /* MLIB_USE_FTOI_CLAMPING */

#define	FLOAT2INT_CLAMP(X)                                        \
	(((X) > MLIB_S32_MAX) ? MLIB_S32_MAX : ((mlib_s32)(X)))

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	MULS_S16                                                \
	mlib_s32 i;                                             \
	mlib_d64 val_c = *c * 2.;                               \
	                                                        \
	for (i = 0; i < n; i++) {                               \
	    dst[i] = FLOAT2INT_CLAMP(src[i] * val_c) >> 16;     \
	}                                                       \
	                                                        \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/* *********************************************************** */

#define	MULS_S16S                                                       \
	mlib_s32 i;                                                     \
	mlib_d64 val_c_0 = *c * 2.;                                     \
	mlib_d64 val_c_1 = *(c + 1) * 2.;                               \
	                                                                \
	for (i = 0; i < n; i++) {                                       \
	    dst[2 * i] = FLOAT2INT_CLAMP(src[2 * i] * val_c_0) >> 16;   \
	    dst[2 * i + 1] =                                            \
		FLOAT2INT_CLAMP(src[2 * i + 1] * val_c_1) >> 16;        \
	}                                                               \
	                                                                \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/* *********************************************************** */

mlib_status
__mlib_SignalMulS_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *c,
    mlib_s32 n)
{
	MULS_S16;
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulS_S16S_S16S_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *c,
    mlib_s32 n)
{
	MULS_S16S;
}

/* *********************************************************** */
