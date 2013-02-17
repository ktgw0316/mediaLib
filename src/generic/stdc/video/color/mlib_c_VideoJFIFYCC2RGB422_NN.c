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

#pragma ident	"@(#)mlib_c_VideoJFIFYCC2RGB422_NN.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorJFIFYCC2RGB422_Nearest - color conversion with
 *                                              422 upsampling
 *                                              with nearest filter.
 *                                              Only Cr and Cb components are
 *                                              under upsampling
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorJFIFYCC2RGB422_Nearest(mlib_u8       *rgb,
 *                                                        const mlib_u8 *y,
 *                                                        const mlib_u8 *cb,
 *                                                        const mlib_u8 *cr,
 *                                                        mlib_s32      n)
 *
 * ARGUMENTS
 *      rgb       Pointer to RGB multicomponent row, 8-byte aligned
 *      y         Pointer to Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      n         Length of y array
 *                Note: n must be even,
 *                      length of cb, cr arrays must be n/2
 *                      length of rgb array must be 3*n
 *
 * DESCRIPTION
 *      mlib_VideoColorJFIFYCC2RGB422_Nearest:
 *        composition of mlib_VideoUpSample422_Nearest with
 *                       mlib_VideoColorJFIFYCC2RGB444
 *
 */

#include <mlib_video.h>
#include <mlib_c_VideoColorJFIFYCC.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorJFIFYCC2RGB422_Nearest = \
	__mlib_VideoColorJFIFYCC2RGB422_Nearest

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorJFIFYCC2RGB422_Nearest)
	mlib_VideoColorJFIFYCC2RGB422_Nearest
	__attribute__((weak,
	alias("__mlib_VideoColorJFIFYCC2RGB422_Nearest")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	TCLAMP_U8(val, dst)	dst = pClip[val]

/* *********************************************************** */

#define	SCLAMP_U8(s, dst)                                       \
	{                                                       \
	    mlib_s32 v = s, mask = (v - 0x1fe) >> 31;           \
	                                                        \
	    dst = (((mlib_u32)v >> 1) | ~mask) & ~(v >> 31);    \
	}

/* *********************************************************** */

mlib_status
__mlib_VideoColorJFIFYCC2RGB422_Nearest(
	mlib_u8 *rgb,
	const mlib_u8 *y,
	const mlib_u8 *cb,
	const mlib_u8 *cr,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_s32 BBi = 454;
	mlib_s32 RRi = 359;
	mlib_u8 *const pClip = (mlib_u8 *const)mlib_tClip + 288 * 2;

	n = n >> 1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_s32 y0, y1, cb0, cr0, sr, sg, sb;

		cb0 = cb[0];
		cr0 = cr[0];

		y0 = y[0] * 2;
		y1 = y[1] * 2;

		sr = mlib_tRCr[cr0] - RRi;
		sg = mlib_tGCb[cb0] - mlib_tGCr[cr0];
		sb = mlib_tBCb[cb0] - BBi;

		SCLAMP_U8(y0 + sr, rgb[0]);
		TCLAMP_U8(y0 + sg, rgb[1]);
		TCLAMP_U8(y0 + sb, rgb[2]);

		SCLAMP_U8(y1 + sr, rgb[3]);
		TCLAMP_U8(y1 + sg, rgb[4]);
		TCLAMP_U8(y1 + sb, rgb[5]);

		y += 2;
		cb++;
		cr++;
		rgb += 6;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
