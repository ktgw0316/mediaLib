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

#pragma ident	"@(#)mlib_c_SignalCrossCorrel.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_SignalCrossCorrel - calculates image cross correlation
 *                               for the input signals
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalCrossCorrel_S16(mlib_d64       *correl,
 *                                             const mlib_s16 *src1,
 *                                             const mlib_s16 *src2,
 *                                             mlib_s32       n);
 *      mlib_status mlib_SignalCrossCorrel_S16S(mlib_d64       *correl,
 *                                              const mlib_s16 *src1,
 *                                              const mlib_s16 *src2,
 *                                              mlib_s32       n);
 * ARGUMENTS
 *      correl   pointer to cross correlation array
 *      src1     the first input signal array
 *      src2     the second input signal array
 *      n        number of samples in the signal arrays
 *
 * DESCRIPTION
 *
 *           1    N-1
 *      c = --- * SUM (x[n] * y[n])
 *           N    n=0
 */

#include <mlib_signal.h>
#include <mlib_SignalCorrel.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalCrossCorrel_S16 = __mlib_SignalCrossCorrel_S16
#pragma weak mlib_SignalCrossCorrel_S16S = __mlib_SignalCrossCorrel_S16S

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalCrossCorrel_S16) mlib_SignalCrossCorrel_S16
    __attribute__((weak, alias("__mlib_SignalCrossCorrel_S16")));
__typeof__(__mlib_SignalCrossCorrel_S16S) mlib_SignalCrossCorrel_S16S
    __attribute__((weak, alias("__mlib_SignalCrossCorrel_S16S")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_SignalCrossCorrel_S16(
    mlib_d64 *correl,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    mlib_s32 n)
{
	mlib_d64 rez[4];

	if (correl == NULL)
		return (MLIB_FAILURE);

	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_SignalCrossCorrel_Intr_S16(src1, src2, rez, n);
	correl[0] =
	    (rez[0] + rez[1] + rez[2] + rez[3]) / (n * (mlib_d64)(1 << 30));

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalCrossCorrel_S16S(
    mlib_d64 *correl,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    mlib_s32 n)
{
	mlib_d64 rez[4];

	if (correl == NULL)
		return (MLIB_FAILURE);

	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_SignalCrossCorrel_Intr_S16(src1, src2, rez, n + n);
	correl[0] = (rez[0] + rez[2]) / (n * (mlib_d64)(1 << 30));
	correl[1] = (rez[1] + rez[3]) / (n * (mlib_d64)(1 << 30));

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S16(x, y)	((x) * (mlib_d64) (y))

/* *********************************************************** */

void
mlib_SignalCrossCorrel_Intr_S16(
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    mlib_d64 *rez,
    mlib_s32 n)
{
/* pointer to the data of first signal array */
	const mlib_s16 *psrc1 = src1;

/* pointer to the data of second signal array */
	const mlib_s16 *psrc2 = src2;

/* accumulators */
	mlib_d64 c1, c2, c3, c4;

/* number of samples */
	mlib_s32 size = n;

/* indices */
	mlib_s32 j, k;

	c1 = c2 = c3 = c4 = 0;
	rez[0] = rez[1] = rez[2] = 0;
	for (j = 0; j <= (size - 4); j += 4) {
		c1 += MUL_S16(psrc1[j], psrc2[j]);
		c2 += MUL_S16(psrc1[j + 1], psrc2[j + 1]);
		c3 += MUL_S16(psrc1[j + 2], psrc2[j + 2]);
		c4 += MUL_S16(psrc1[j + 3], psrc2[j + 3]);
	}

	for (k = 0; j < size; j++, k++)
		rez[k] += MUL_S16(psrc1[j], psrc2[j]);
	rez[0] += c1;
	rez[1] += c2;
	rez[2] += c3;
	rez[3] = c4;
}

/* *********************************************************** */
