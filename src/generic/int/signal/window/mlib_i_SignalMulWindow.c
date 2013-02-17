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

#pragma ident	"@(#)mlib_i_SignalMulWindow.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulWindow_S16 - multiply source data on
 *          coefficients of the corresponding window
 *
 *      mlib_SignalMulWindow_S16S - multiply stereo source
 *          data on coefficients of the corresponding window
 *
 *      mlib_SignalMulWindow_S16_S16 - multiply source data on
 *          coefficients of the corresponding window with results
 *          in destination array
 *
 *      mlib_SignalMulWindow_S16S_S16S - multiply stereo source
 *          data on coefficients of the corresponding window with
 *          result in destination array
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalMulWindow_S16(mlib_s16       *data,
 *                                           const mlib_s16 *window,
 *                                           mlib_s32       n)
 *
 *      mlib_status mlib_SignalMulWindow_S16S(mlib_s16       *data,
 *                                            const mlib_s16 *window,
 *                                            mlib_s32       n)
 *
 *      mlib_status mlib_SignalMulWindow_S16_S16(mlib_s16               *dst,
 *                                               const mlib_s16         *src,
 *                                               const mlib_s16 *window
 *                                               mlib_s32               n)
 *
 *      mlib_status mlib_SignalMulWindow_S16S_S16S(mlib_s16       *dst,
 *                                                 const mlib_s16 *src,
 *                                                 const mlib_s16 *window,
 *                                                 mlib_s32       n)
 *
 * ARGUMENTS
 *      data     Input and output signal array
 *      dst      Output signal array
 *      src      Input signal array
 *      window   Window coefficient array
 *      n        Length of window array
 *
 * DESCRIPTION
 *      Mono        :   dst[i] = src[i] * window[i]    i = 0..(N-1)
 *      Stereo      :   dst[i] = src[i] * window[i/2]  i = 0..(2*N-1)
 */

#include <mlib_signal.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulWindow_S16 = __mlib_SignalMulWindow_S16
#pragma weak mlib_SignalMulWindow_S16S = __mlib_SignalMulWindow_S16S
#pragma weak mlib_SignalMulWindow_S16S_S16S = \
	__mlib_SignalMulWindow_S16S_S16S
#pragma weak mlib_SignalMulWindow_S16_S16 = __mlib_SignalMulWindow_S16_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulWindow_S16) mlib_SignalMulWindow_S16
    __attribute__((weak, alias("__mlib_SignalMulWindow_S16")));
__typeof__(__mlib_SignalMulWindow_S16S) mlib_SignalMulWindow_S16S
    __attribute__((weak, alias("__mlib_SignalMulWindow_S16S")));
__typeof__(__mlib_SignalMulWindow_S16S_S16S) mlib_SignalMulWindow_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalMulWindow_S16S_S16S")));
__typeof__(__mlib_SignalMulWindow_S16_S16) mlib_SignalMulWindow_S16_S16
    __attribute__((weak, alias("__mlib_SignalMulWindow_S16_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SAT_S16(x)						\
	((x == MLIB_S16_MIN) ? MLIB_S16_MAX : x)

/* *********************************************************** */

mlib_status
__mlib_SignalMulWindow_S16_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *window,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_s16 dtmp;

/* check for obvious errors */
	if ((dst == NULL) || (n <= 0) || (src == NULL) || (window == NULL))
		return (MLIB_FAILURE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		dtmp = (mlib_s32)(src[i] * window[i]) >> 15;
		dst[i] = SAT_S16(dtmp);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulWindow_S16S_S16S(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *window,
    mlib_s32 n)
{
	mlib_s32 i, j;
	mlib_s16 dtmp1, dtmp2;

/* check for obvious errors */

	if ((dst == NULL) || (n <= 0) || (src == NULL) || (window == NULL))
		return (MLIB_FAILURE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0, j = 0; j < n; i += 2, j++) {
		dtmp1 = (mlib_s32)(src[i] * window[j]) >> 15;
		dtmp2 = (mlib_s32)(src[i + 1] * window[j]) >> 15;
		dst[i] = SAT_S16(dtmp1);
		dst[i + 1] = SAT_S16(dtmp2);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulWindow_S16(
    mlib_s16 *data,
    const mlib_s16 *window,
    mlib_s32 n)
{
	return (__mlib_SignalMulWindow_S16_S16(data, data, window, n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulWindow_S16S(
    mlib_s16 *data,
    const mlib_s16 *window,
    mlib_s32 n)
{
	return (__mlib_SignalMulWindow_S16S_S16S(data, data, window, n));
}

/* *********************************************************** */
