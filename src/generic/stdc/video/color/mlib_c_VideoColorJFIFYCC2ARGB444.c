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

#pragma ident	"@(#)mlib_c_VideoColorJFIFYCC2ARGB444.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorJFIFYCC2ARGB444 - color conversion from JFIF YCrCb
 *                                       to ARGB space
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorJFIFYCC2ARGB444(mlib_u8       *argb,
 *                                                 const mlib_u8 *y,
 *                                                 const mlib_u8 *cb,
 *                                                 const mlib_u8 *cr,
 *                                                 mlib_s32      n)
 *
 * ARGUMENTS
 *      argb      Pointer to ARGB multicomponent row, 8-byte aligned
 *      y         Pointer to Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      n         Length of y, cb, cr arrays must be n
 *                Note: length of argb array must be 4*n
 *
 * DESCRIPTION
 *      mlib_VideoColorJFIFYCC2ARGB444
 *        R  = Y                        + 1.40200 * (Cr - 128)
 *        G  = Y - 0.34414 * (Cb - 128) - 0.71414 * (Cr - 128)
 *        B  = Y + 1.77200 * (Cb - 128)
 *
 *        Y = y[i], Cr = cr[i], Cb = cb[i]
 *        R = argb[4*i+1], G = argb[4*i+2], B = argb[4*i+3]
 *        A = argb[4*i] - should be 0xFF
 *
 *        0 <= i < n
 */

#include <mlib_video.h>
#include <mlib_c_VideoColorJFIFYCC.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorJFIFYCC2ARGB444 = \
	__mlib_VideoColorJFIFYCC2ARGB444

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorJFIFYCC2ARGB444) mlib_VideoColorJFIFYCC2ARGB444
	__attribute__((weak, alias("__mlib_VideoColorJFIFYCC2ARGB444")));

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
__mlib_VideoColorJFIFYCC2ARGB444(
	mlib_u8 *argb,
	const mlib_u8 *y,
	const mlib_u8 *cb,
	const mlib_u8 *cr,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_s32 BBi = 454;
	mlib_s32 RRi = 359;
	mlib_u8 *const pClip = (mlib_u8 *const)mlib_tClip + 288 * 2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_s32 y0, cb0, cr0, sr, sg, sb;

		cb0 = cb[0];
		cr0 = cr[0];

		y0 = y[0] * 2;

		sr = mlib_tRCr[cr0] - RRi;
		sg = mlib_tGCb[cb0] - mlib_tGCr[cr0];
		sb = mlib_tBCb[cb0] - BBi;

		argb[0] = 0xff;
		SCLAMP_U8(y0 + sr, argb[1]);
		TCLAMP_U8(y0 + sg, argb[2]);
		TCLAMP_U8(y0 + sb, argb[3]);

		y++;
		cb++;
		cr++;
		argb += 4;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
