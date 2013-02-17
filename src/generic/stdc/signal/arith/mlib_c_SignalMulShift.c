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

#pragma ident	"@(#)mlib_c_SignalMulShift.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulShift_[S16|S16S]_Sat,
 *      mlib_SignalMulShift_[S16|S16S]_[S16|S16S]_Sat - multiplication of
 *                     mlib_s32 16-bit format signal arrays
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalMulShift_S16S_S16S_Sat(mlib_s16       *dst,
 *                                                    const mlib_s16 *src1,
 *                                                    const mlib_s16 *src2,
 *                                                    mlib_s32       shift,
 *                                                    mlib_s32       n);
 *      mlib_status mlib_SignalMulShift_S16S_Sat(mlib_s16       *data,
 *                                               const mlib_s16 *src2,
 *                                               mlib_s32       shift,
 *                                               mlib_s32       n);
 *      mlib_status mlib_SignalMulShift_S16_S16_Sat(mlib_s16       *dst,
 *                                                  const mlib_s16 *src1,
 *                                                  const mlib_s16 *src2,
 *                                                  mlib_s32       shift,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_SignalMulShift_S16_Sat(mlib_s16       *data,
 *                                              const mlib_s16 *src2,
 *                                              mlib_s32       shift,
 *                                              mlib_s32       n);
 *
 * ARGUMENTS
 *      data  Input and output signal array
 *      dst   Output signal array
 *      src   Input signal array
 *      shift Left shifting factor, may be negative
 *      n     Number of samples in the input signal arrays.
 *
 * DESCRIPTION
 *      dst = src1 * src2 * (2 ** shift)
 */

#include <mlib_signal.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulShift_S16_S16_Sat = \
	__mlib_SignalMulShift_S16_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulShift_S16_S16_Sat) mlib_SignalMulShift_S16_S16_Sat
    __attribute__((weak, alias("__mlib_SignalMulShift_S16_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(X)	((mlib_s32)(X))

#else /* MLIB_USE_FTOI_CLAMPING */

#define	FLOAT2INT_CLAMP(X)                                      \
	(((X) > MLIB_S32_MAX) ? MLIB_S32_MAX : (((X) <          \
	    MLIB_S32_MIN) ? MLIB_S32_MIN : ((mlib_s32)(X))))

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

mlib_status
__mlib_SignalMulShift_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i, sh1;

	if (shift <= -1) {
		if (shift < -16)
			shift = -16;
		sh1 = -shift + 15;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++)
			dst[i] = (mlib_s32)(src1[i] * (mlib_d64)src2[i]) >> sh1;
	} else if (shift <= 15) {
		sh1 = shift + 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++)
			dst[i] =
			    FLOAT2INT_CLAMP((src1[i] << sh1) *
			    (mlib_d64)src2[i]) >> 16;
	} else {
		mlib_s32 a;

		if (shift > 31)
			shift = 31;
		sh1 = shift - 15;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			a = ((mlib_u16 *)src2)[i] << 16;
			dst[i] =
			    FLOAT2INT_CLAMP((src1[i] << sh1) *
			    (mlib_d64)a) >> 16;
		}
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */
