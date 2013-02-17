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

#pragma ident	"@(#)mlib_c_SignalLimit.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalLimit_[S16|S16S]_[S16|S16S],
 *      mlib_SignalLimit_[S16|S16S]             - hard limiting
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalLimit_S16(mlib_s16       *data,
 *                                       const mlib_s16 *low,
 *                                       const mlib_s16 *high,
 *                                       mlib_s32       n);
 *      mlib_status mlib_SignalLimit_S16S(mlib_s16       *data,
 *                                        const mlib_s16 *low,
 *                                        const mlib_s16 *high,
 *                                        mlib_s32       n);
 *      mlib_status mlib_SignalLimit_S16S_S16S(mlib_s16       *dst,
 *                                             const mlib_s16 *src,
 *                                             const mlib_s16 *low,
 *                                             const mlib_s16 *high,
 *                                             mlib_s32       n);
 *      mlib_status mlib_SignalLimit_S16_S16(mlib_s16       *dst,
 *                                           const mlib_s16 *src,
 *                                           const mlib_s16 *low,
 *                                           const mlib_s16 *high,
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
 *      dst[i] = { t      , if src[i] < t
 *                  low                  low
 *                 src[i] , if t <= src[i] < t
 *                              low           high
 *                 t      , if src[i] >= t
 *                  high                  high
 *                }
 *
 */

#include <mlib_signal.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalLimit_S16 = __mlib_SignalLimit_S16
#pragma weak mlib_SignalLimit_S16S = __mlib_SignalLimit_S16S
#pragma weak mlib_SignalLimit_S16S_S16S = __mlib_SignalLimit_S16S_S16S
#pragma weak mlib_SignalLimit_S16_S16 = __mlib_SignalLimit_S16_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalLimit_S16) mlib_SignalLimit_S16
    __attribute__((weak, alias("__mlib_SignalLimit_S16")));
__typeof__(__mlib_SignalLimit_S16S) mlib_SignalLimit_S16S
    __attribute__((weak, alias("__mlib_SignalLimit_S16S")));
__typeof__(__mlib_SignalLimit_S16S_S16S) mlib_SignalLimit_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalLimit_S16S_S16S")));
__typeof__(__mlib_SignalLimit_S16_S16) mlib_SignalLimit_S16_S16
    __attribute__((weak, alias("__mlib_SignalLimit_S16_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	LIMIT_S16                                               \
	mlib_s32 i;                                             \
	mlib_s16 t_low = *low;                                  \
	mlib_s16 t_high = *high;                                \
	mlib_s16 x, x1;                                         \
	                                                        \
	if ((t_low > t_high) || (n <= 0))                       \
	    return (MLIB_FAILURE);                              \
	if (i = n & 1) {                                        \
	    x = src[0];                                         \
	    if (x < t_low)                                      \
		x = t_low;                                      \
	    else if (x >= t_high)                               \
		x = t_high;                                     \
	    dst[0] = x;                                         \
	}                                                       \
	for (; i < n; i += 2) {                                 \
	    x = src[i];                                         \
	    if (x < t_low)                                      \
		x = t_low;                                      \
	    else if (x >= t_high)                               \
		x = t_high;                                     \
	    dst[i] = x;                                         \
	    x1 = src[i + 1];                                    \
	    if (x1 < t_low)                                     \
		x1 = t_low;                                     \
	    else if (x1 >= t_high)                              \
		x1 = t_high;                                    \
	    dst[i + 1] = x1;                                    \
	}                                                       \
	                                                        \
	return MLIB_SUCCESS

/* *********************************************************** */

#define	LIMIT_S16S                                                      \
	mlib_s32 i;                                                     \
	mlib_s16 t_low_0 = *low;                                        \
	mlib_s16 t_high_0 = *high;                                      \
	mlib_s16 t_low_1 = *(low + 1);                                  \
	mlib_s16 t_high_1 = *(high + 1);                                \
	mlib_s16 x_0, x_1;                                              \
	                                                                \
	if ((t_low_0 > t_high_0) || (t_low_1 > t_high_1) || (n <= 0))   \
	    return (MLIB_FAILURE);                                      \
	for (i = 0; i < n; i++) {                                       \
	    x_0 = src[2 * i];                                           \
	    x_1 = src[2 * i + 1];                                       \
	    if (x_0 < t_low_0)                                          \
		x_0 = t_low_0;                                          \
	    else if (x_0 >= t_high_0)                                   \
		x_0 = t_high_0;                                         \
	    dst[2 * i] = x_0;                                           \
	    if (x_1 < t_low_1)                                          \
		x_1 = t_low_1;                                          \
	    else if (x_1 >= t_high_1)                                   \
		x_1 = t_high_1;                                         \
	    dst[2 * i + 1] = x_1;                                       \
	}                                                               \
	                                                                \
	return MLIB_SUCCESS

/* *********************************************************** */

mlib_status
__mlib_SignalLimit_S16_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *low,
    const mlib_s16 *high,
    mlib_s32 n)
{
	LIMIT_S16;
}

/* *********************************************************** */

mlib_status
__mlib_SignalLimit_S16(
    mlib_s16 *data,
    const mlib_s16 *low,
    const mlib_s16 *high,
    mlib_s32 n)
{
	return (__mlib_SignalLimit_S16_S16(data, data, low, high, n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalLimit_S16S_S16S(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *low,
    const mlib_s16 *high,
    mlib_s32 n)
{
	LIMIT_S16S;
}

/* *********************************************************** */

mlib_status
__mlib_SignalLimit_S16S(
    mlib_s16 *data,
    const mlib_s16 *low,
    const mlib_s16 *high,
    mlib_s32 n)
{
	return (__mlib_SignalLimit_S16S_S16S(data, data, low, high, n));
}

/* *********************************************************** */
