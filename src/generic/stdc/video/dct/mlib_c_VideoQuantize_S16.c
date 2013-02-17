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

#pragma ident	"@(#)mlib_c_VideoQuantize_S16.c	9.6	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_VideoQuantizeInit_S16   - convert quantization table parameter for
 *                                     mlib_VideoQuantize_S16
 *      mlib_VideoQuantize_S16       - "round" divide of aligned S16 vectors
 *      mlib_VideoDeQuantizeInit_S16 - convert quantization table parameter for
 *                                     mlib_VideoDeQuantize_S16
 *      mlib_VideoDeQuantize_S16     - multiply of aligned S16 vectors
 *
 * SYNOPSIS
 *
 *      mlib_status  mlib_VideoQuantizeInit_S16(mlib_d64       dqtable[64],
 *                                              const mlib_s16 iqtable[64])
 *
 *      mlib_status  mlib_VideoQuantize_S16(mlib_s16       icoeffs[64],
 *                                          const mlib_d64 dqtable[64])
 *
 *      mlib_status  mlib_VideoDeQuantizeInit_S16(mlib_d64       dqtable[64],
 *                                                const mlib_s16 iqtable[64])
 *
 *      mlib_status  mlib_VideoDeQuantize_S16(mlib_s16       icoeffs[64],
 *                                            const mlib_d64 dqtable[64])
 *
 * ARGUMENTS
 *
 *      icoeffs     Pointer to DCT coefficients, 8-byte aligned
 *
 *      iqtable     Pointer to quantization table coefficients
 *
 *      dqtable     Pointer to converted quantization table coefficients,
 *                  different for Quantize and DeQuantize functions,
 *                  also different for C/VIS/SSE2 versions
 *
 * DESCRIPTION
 *
 *      mlib_VideoQuantize_S16:
 *        icoeffs[i] = "round"(icoeffs[i]/iqtable[i]), 0 <= i < 64
 *
 *        current formula of "round" :
 *          ivalue = (int)(dvalue + 32768.5) - (1 << 15)
 *
 *      mlib_VideoDeQuantize_S16:
 *        icoeffs[i] = icoeffs[i]*iqtable[i], 0 <= i < 64
 *
 * RESTRICTION
 *
 *           0 <  iqtable[i] < 256
 *      -16384 <= icoeffs[i] < 16384,           0 <= i < 64
 *
 *      Results of overflow are not controlled.
 *
 * NOTE
 *
 *   mlib_Video[De]QuantizeInit_S16 generate converted quantization tables
 *   for mlib_Vide[De]Quantize_S16, respectively, in different ways.  The
 *   converted tables for C and VIS versions of the same function can be
 *   different as well.  So never mix the converted quantization tables
 *   between Quantize and DeQuantize functions or between C and VIS versions.
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoDeQuantizeInit_S16 = __mlib_VideoDeQuantizeInit_S16
#pragma weak mlib_VideoDeQuantize_S16 = __mlib_VideoDeQuantize_S16
#pragma weak mlib_VideoQuantizeInit_S16 = __mlib_VideoQuantizeInit_S16
#pragma weak mlib_VideoQuantize_S16 = __mlib_VideoQuantize_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoDeQuantizeInit_S16) mlib_VideoDeQuantizeInit_S16
	__attribute__((weak, alias("__mlib_VideoDeQuantizeInit_S16")));
__typeof__(__mlib_VideoDeQuantize_S16) mlib_VideoDeQuantize_S16
	__attribute__((weak, alias("__mlib_VideoDeQuantize_S16")));
__typeof__(__mlib_VideoQuantizeInit_S16) mlib_VideoQuantizeInit_S16
	__attribute__((weak, alias("__mlib_VideoQuantizeInit_S16")));
__typeof__(__mlib_VideoQuantize_S16) mlib_VideoQuantize_S16
	__attribute__((weak, alias("__mlib_VideoQuantize_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoQuantizeInit_S16(
	mlib_d64 dqtable[64],
	const mlib_s16 iqtable[64])
{
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < 64; i++) {
		if (iqtable[i] <= 0)
			return (MLIB_FAILURE);
		dqtable[i] = 1.0 / iqtable[i];
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoQuantize_S16(
	mlib_s16 icoeffs[64],
	const mlib_d64 dqtable[64])
{
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < 64; i++) {
		icoeffs[i] =
			(mlib_s16)((mlib_s32)(icoeffs[i] * dqtable[i]
			+ 32768.5) - (1 << 15));
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoDeQuantizeInit_S16(
	mlib_d64 dqtable[64],
	const mlib_s16 iqtable[64])
{
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < 64; i++) {
		dqtable[i] = iqtable[i];
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoDeQuantize_S16(
	mlib_s16 icoeffs[64],
	const mlib_d64 dqtable[64])
{
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < 64; i++) {
		icoeffs[i] = (mlib_s16)(icoeffs[i] * dqtable[i]);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
