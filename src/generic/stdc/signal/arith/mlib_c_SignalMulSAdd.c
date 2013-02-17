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

#pragma ident	"@(#)mlib_c_SignalMulSAdd.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulSAdd_[S16|S16S]_Sat,
 *      mlib_SignalMulSAdd_[S16|S16S]_[S16|S16S]_Sat - linear function on
 *                     mlib_s32 16-bit format signal array
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalMulSAdd_S16S_S16S_Sat(mlib_s16       *dst,
 *                                                   const mlib_s16 *src1,
 *                                                   const mlib_s16 *src2,
 *                                                   const mlib_s16 *c,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_SignalMulSAdd_S16S_Sat(mlib_s16       *src1dst,
 *                                              const mlib_s16 *src2,
 *                                              const mlib_s16 *c,
 *                                              mlib_s32       n);
 *      mlib_status mlib_SignalMulSAdd_S16_S16_Sat(mlib_s16       *dst,
 *                                                 const mlib_s16 *src1,
 *                                                 const mlib_s16 *src2,
 *                                                 const mlib_s16 *c,
 *                                                 mlib_s32       n);
 *      mlib_status mlib_SignalMulSAdd_S16_Sat(mlib_s16       *src1dst,
 *                                             const mlib_s16 *src2,
 *                                             const mlib_s16 *c,
 *                                             mlib_s32       n);
 * ARGUMENTS
 *      src1dst  The first input and output signal array
 *      dst      Output signal array
 *      src1     The first input signal array
 *      src2     The second input signal array
 *      c        Scaling factor. The scaling factor is in Q15 format
 *      n        Number of samples in the input signal arrays.
 *
 * DESCRIPTION
 *      dst = src1 + c * src2
 */

#include <mlib_signal.h>
#include <mlib_Utils.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulSAdd_S16S_S16S_Sat = \
	__mlib_SignalMulSAdd_S16S_S16S_Sat
#pragma weak mlib_SignalMulSAdd_S16_S16_Sat = \
	__mlib_SignalMulSAdd_S16_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulSAdd_S16S_S16S_Sat) mlib_SignalMulSAdd_S16S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalMulSAdd_S16S_S16S_Sat")));
__typeof__(__mlib_SignalMulSAdd_S16_S16_Sat) mlib_SignalMulSAdd_S16_S16_Sat
    __attribute__((weak, alias("__mlib_SignalMulSAdd_S16_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_SignalMulSAdd_S16S_S16S_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    const mlib_s16 *c,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 val_c_0 = c[0] << 1;
	mlib_d64 val_c_1 = c[1] << 1;
	mlib_s32 a1, b1, a2, b2;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		a1 = ((mlib_u16 *)src1)[2 * i] << 16;
		b1 = src2[2 * i];
		a2 = ((mlib_u16 *)src1)[2 * i + 1] << 16;
		b2 = src2[2 * i + 1];
		SAT_S16(dst[2 * i], a1 + b1 * val_c_0);
		SAT_S16(dst[2 * i + 1], a2 + b2 * val_c_1);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulSAdd_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    const mlib_s16 *c,
    mlib_s32 n)
{
	mlib_s16 c2[2];

	if (n <= 0)
		return (MLIB_FAILURE);

	if (n & 1) {
		mlib_d64 val_c = c[0] << 1;
		mlib_s32 a, b;

		a = (*src1++) << 16;
		b = (*src2++);
		SAT_S16(dst[0], a + b * val_c);
		dst++;
	}

	c2[0] = c[0];
	c2[1] = c[0];
	__mlib_SignalMulSAdd_S16S_S16S_Sat(dst, src1, src2, c2, n >> 1);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
