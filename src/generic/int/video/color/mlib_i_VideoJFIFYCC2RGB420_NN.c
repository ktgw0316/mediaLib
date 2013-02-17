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

#pragma ident	"@(#)mlib_i_VideoJFIFYCC2RGB420_NN.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorJFIFYCC2RGB420_Nearest - color conversion with
 *                                              420 upsampling
 *                                              with nearest filter.
 *                                              Only Cr and Cb components are
 *                                              under upsampling.
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorJFIFYCC2RGB420_Nearest(mlib_u8       *rgb0,
 *                                                        mlib_u8       *rgb1,
 *                                                        const mlib_u8 *y0,
 *                                                        const mlib_u8 *y1,
 *                                                        const mlib_u8 *cb,
 *                                                        const mlib_u8 *cr,
 *                                                        mlib_s32      n)
 *
 * ARGUMENTS
 *      rgb0      Pointer to upper RGB multicomponent row, 8-byte aligned
 *      rgb1      Pointer to lower RGB multicomponent row, 8-byte aligned
 *      y0        Pointer to upper Y component row, 8-byte aligned
 *      y1        Pointer to lower Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      n         Length of y0 and y1 arrays
 *                Note: n must be even,
 *                      length of cb, cr arrays must be n/2
 *                      length of rgb0, rgb1 arrays must be 3*n
 *
 * DESCRIPTION
 *
 *      mlib_VideoColorJFIFYCC2RGB420_Nearest:
 *        composition of mlib_VideoUpSample420_Nearest with
 *                       mlib_VideoColorJFIFYCC2RGB444
 */

#include <mlib_video.h>
#include <mlib_c_VideoColorJFIFYCC.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorJFIFYCC2RGB420_Nearest = \
	__mlib_VideoColorJFIFYCC2RGB420_Nearest

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorJFIFYCC2RGB420_Nearest)
	mlib_VideoColorJFIFYCC2RGB420_Nearest
	__attribute__((weak,
	alias("__mlib_VideoColorJFIFYCC2RGB420_Nearest")));

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
__mlib_VideoColorJFIFYCC2RGB420_Nearest(
	mlib_u8 *rgb0,
	mlib_u8 *rgb1,
	const mlib_u8 *y0,
	const mlib_u8 *y1,
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
		mlib_s32 y00, y01, y10, y11, cb0, cr0, sr, sg, sb;
		mlib_u8 a0, a1, a2, a3, a4, a5;
		mlib_u8 b0, b1, b2, b3, b4, b5;

		cb0 = cb[0];
		cr0 = cr[0];

		y00 = y0[0] * 2;
		y01 = y0[1] * 2;
		y10 = y1[0] * 2;
		y11 = y1[1] * 2;

		sr = mlib_tRCr[cr0] - RRi;
		sg = mlib_tGCb[cb0] - mlib_tGCr[cr0];
		sb = mlib_tBCb[cb0] - BBi;

		SCLAMP_U8(y00 + sr, a0);
		TCLAMP_U8(y00 + sg, a1);
		TCLAMP_U8(y00 + sb, a2);

		SCLAMP_U8(y01 + sr, a3);
		TCLAMP_U8(y01 + sg, a4);
		TCLAMP_U8(y01 + sb, a5);

		SCLAMP_U8(y10 + sr, b0);
		TCLAMP_U8(y10 + sg, b1);
		TCLAMP_U8(y10 + sb, b2);

		SCLAMP_U8(y11 + sr, b3);
		TCLAMP_U8(y11 + sg, b4);
		TCLAMP_U8(y11 + sb, b5);

#ifdef _LITTLE_ENDIAN
		((mlib_u16 *)rgb0)[0] = ((mlib_u16)a1 << 8) | ((mlib_u16) a0);
		((mlib_u16 *)rgb0)[1] = ((mlib_u16)a3 << 8) | ((mlib_u16) a2);
		((mlib_u16 *)rgb0)[2] = ((mlib_u16)a5 << 8) | ((mlib_u16) a4);
		((mlib_u16 *)rgb1)[0] = ((mlib_u16)b1 << 8) | ((mlib_u16) b0);
		((mlib_u16 *)rgb1)[1] = ((mlib_u16)b3 << 8) | ((mlib_u16) b2);
		((mlib_u16 *)rgb1)[2] = ((mlib_u16)b5 << 8) | ((mlib_u16) b4);
#else
		((mlib_u16 *)rgb0)[0] = ((mlib_u16)a0 << 8) | ((mlib_u16) a1);
		((mlib_u16 *)rgb0)[1] = ((mlib_u16)a2 << 8) | ((mlib_u16) a3);
		((mlib_u16 *)rgb0)[2] = ((mlib_u16)a4 << 8) | ((mlib_u16) a5);
		((mlib_u16 *)rgb1)[0] = ((mlib_u16)b0 << 8) | ((mlib_u16) b1);
		((mlib_u16 *)rgb1)[1] = ((mlib_u16)b2 << 8) | ((mlib_u16) b3);
		((mlib_u16 *)rgb1)[2] = ((mlib_u16)b4 << 8) | ((mlib_u16) b5);
#endif
		y0 += 2;
		y1 += 2;
		cb++;
		cr++;
		rgb0 += 6;
		rgb1 += 6;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
