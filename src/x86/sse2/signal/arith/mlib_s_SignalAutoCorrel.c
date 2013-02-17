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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_SignalAutoCorrel.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_SignalAutoCorrel - calculates auto-correlation
 *                              for the input signal
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalAutoCorrel_S16(mlib_d64       *correl,
 *                                            const mlib_s16 *src,
 *                                            mlib_s32       disp,
 *                                            mlib_s32       n);
 *      mlib_status mlib_SignalAutoCorrel_S16S(mlib_d64       *correl,
 *                                             const mlib_s16 *src,
 *                                             mlib_s32       disp,
 *                                             mlib_s32       n);
 * ARGUMENTS
 *      correl   pointer to auto-correlation array
 *      src      a signal array
 *      disp     displacement
 *      n        number of samples in the signal array
 *
 * DESCRIPTION
 *
 *             1     N-d-1
 *      c = ------- * SUM (x[n] * x[n+d])
 *           N - d    n = 0
 */

#include <mlib_signal.h>
#include <mlib_SignalCorrel.h>
#include <mlib_s_SignalArith.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalAutoCorrel_S16 = __mlib_SignalAutoCorrel_S16
#pragma weak mlib_SignalAutoCorrel_S16S = __mlib_SignalAutoCorrel_S16S

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalAutoCorrel_S16) mlib_SignalAutoCorrel_S16
    __attribute__((weak, alias("__mlib_SignalAutoCorrel_S16")));
__typeof__(__mlib_SignalAutoCorrel_S16S) mlib_SignalAutoCorrel_S16S
    __attribute__((weak, alias("__mlib_SignalAutoCorrel_S16S")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_SignalAutoCorrel_S16(
    mlib_d64 *correl,
    const mlib_s16 *src,
    mlib_s32 disp,
    mlib_s32 n)
{
	const mlib_s16 *src1, *src2;
	mlib_s32 num = n - disp;
	mlib_d64 rez[8];

	if (correl == NULL)
		return (MLIB_FAILURE);

	if (num <= 0 || disp < 0)
		return (MLIB_FAILURE);

	src1 = src;
	src2 = src + disp;

	mlib_SignalCrossCorrel_Intr_S16(src1, src2, rez, num);

	correl[0] =
	    (rez[0] + rez[1] + rez[2] + rez[3]) / (num * (mlib_d64)(1 << 30));

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalAutoCorrel_S16S(
    mlib_d64 *correl,
    const mlib_s16 *src,
    mlib_s32 disp,
    mlib_s32 n)
{
	const mlib_s16 *src1, *src2;
	mlib_s32 num = n - disp;
	mlib_d64 rez[8];

	if (correl == NULL)
		return (MLIB_FAILURE);

	if (num <= 0 || disp < 0)
		return (MLIB_FAILURE);

	src1 = src;
	src2 = src + 2 * disp;

	mlib_SignalCrossCorrel_Intr_S16(src1, src2, rez, num + num);

	correl[0] = (rez[0] + rez[2]) / (num * (mlib_d64)(1 << 30));
	correl[1] = (rez[1] + rez[3]) / (num * (mlib_d64)(1 << 30));

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
