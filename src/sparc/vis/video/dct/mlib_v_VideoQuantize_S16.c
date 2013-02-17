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

#pragma ident	"@(#)mlib_v_VideoQuantize_S16.c	9.8	07/11/05 SMI"

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
 *      mlib_status mlib_VideoDeQuantizeInit_S16(mlib_d64       dqtable[64],
 *                                               const mlib_s16 iqtable[64]);
 *
 *      mlib_status mlib_VideoDeQuantize_S16(mlib_s16       icoeffs[64],
 *                                           const mlib_d64 dqtable[64]);
 *
 *      mlib_status mlib_VideoQuantizeInit_S16(mlib_d64       dqtable[64],
 *                                             const mlib_s16 iqtable[64]);
 *
 *      mlib_status mlib_VideoQuantize_S16(mlib_s16       icoeffs[64],
 *                                         const mlib_d64 dqtable[64]);
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
 *          ivalue = (mlib_s32)(dvalue + 32768.5) - (1 << 15)
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
#include <vis_proto.h>

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
	mlib_s16 *qtable = (mlib_s16 *)dqtable;
	mlib_s32 i;

#pragma pipeloop(0)
	for (i = 0; i < 64; i++) {
		if (iqtable[i] <= 0)
			return (MLIB_FAILURE);

		mlib_s32 tmp = (mlib_s32)(32768.0 / iqtable[i] + 0.5);
		qtable[i] = tmp - (tmp >> 15);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S16_S32(dr, dr1, dx, dy)                                   \
	{                                                              \
	    mlib_d64 dr2, dr3;                                         \
	                                                               \
	    dr = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dy));    \
	    dr1 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dy));   \
	    dr = vis_fpadd32(dr, dr1);                                 \
	    dr2 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dy));   \
	    dr3 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dy));   \
	    dr1 = vis_fpadd32(dr2, dr3);                               \
	}

/* *********************************************************** */

mlib_status
__mlib_VideoQuantize_S16(
	mlib_s16 icoeffs[64],
	const mlib_d64 dqtable[64])
{
	mlib_d64 *dcoeffs = (mlib_d64 *)icoeffs;
	mlib_d64 w_const = vis_to_double_dup(0x4000);
	mlib_d64 dx, dy, dr, dr1;
	mlib_s32 i;

/* initialize GSR scale factor */
	vis_write_gsr(1 << 3);

#pragma pipeloop(0)
	for (i = 0; i < 16; i++) {
		dx = dcoeffs[i];
		dy = dqtable[i];
		MUL_S16_S32(dr, dr1, dx, dy);
		dr = vis_fpadd32(dr, w_const);
		dr1 = vis_fpadd32(dr1, w_const);
		dr = vis_fpackfix_pair(dr, dr1);
		dcoeffs[i] = dr;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoDeQuantizeInit_S16(
	mlib_d64 dqtable[64],
	const mlib_s16 iqtable[64])
{
	mlib_d64 *diqtable = (mlib_d64 *)vis_alignaddr((void *)iqtable, 0);
	mlib_d64 ds0, ds1, dr;
	mlib_s32 i;

	ds1 = vis_ld_d64_nf(diqtable);

#pragma pipeloop(0)
	for (i = 0; i < 16; i++) {
		ds0 = ds1;
		ds1 = vis_ld_d64_nf(diqtable + 1);
		dr = vis_faligndata(ds0, ds1);
		(*dqtable++) = vis_fpadd16(dr, dr);
		diqtable++;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoDeQuantize_S16(
	mlib_s16 icoeffs[64],
	const mlib_d64 dqtable[64])
{
	mlib_d64 *dcoeffs = (mlib_d64 *)icoeffs;
	mlib_d64 dx, dy, dr, dr1;
	mlib_s32 i;

/* initialize GSR scale factor */
	vis_write_gsr(15 << 3);

#pragma pipeloop(0)
	for (i = 0; i < 16; i++) {
		dx = dcoeffs[i];
		dy = dqtable[i];
		MUL_S16_S32(dr, dr1, dx, dy);
		dr = vis_fpackfix_pair(dr, dr1);
		dcoeffs[i] = dr;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
