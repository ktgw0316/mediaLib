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

#pragma ident	"@(#)mlib_c_VideoColorARGB2JFIFYCC420.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorARGB2JFIFYCC420 - color conversion with
 *                                       420 downsampling with bilinear filter
 *                                       Only Cr and Cb components are
 *                                       under upsampling.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorARGB2JFIFYCC420(mlib_u8       *y0,
 *                                                 mlib_u8       *y1,
 *                                                 mlib_u8       *cb,
 *                                                 mlib_u8       *cr,
 *                                                 const mlib_u8 *argb0,
 *                                                 const mlib_u8 *argb1,
 *                                                 mlib_s32      n)
 *
 * ARGUMENTS
 *      y0        Pointer to upper Y component row, 8-byte aligned
 *      y1        Pointer to lower Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      argb0     Pointer to upper ABGR multicomponent row, 8-byte aligned
 *      argb1     Pointer to lower ABGR multicomponent row, 8-byte aligned
 *      n         Length of y0 and y1 arrays
 *                Note: n must be even,
 *                      length of cb, cr arrays must be n/2
 *                      length of argb0, argb1 arrays must be 4*n
 *
 * DESCRIPTION
 *
 *      mlib_VideoColorARGB2JFIFYCC420:
 *        composition of mlib_VideoColorARGB2JFIFYCC444 with
 *                       mlib_VideoDownSample420
 */

#include <mlib_video.h>
#include <mlib_c_VideoColorJFIFYCC.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorARGB2JFIFYCC420 = \
	__mlib_VideoColorARGB2JFIFYCC420

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorARGB2JFIFYCC420) mlib_VideoColorARGB2JFIFYCC420
	__attribute__((weak, alias("__mlib_VideoColorARGB2JFIFYCC420")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoColorARGB2JFIFYCC420(
	mlib_u8 *y0,
	mlib_u8 *y1,
	mlib_u8 *cb,
	mlib_u8 *cr,
	const mlib_u8 *argb0,
	const mlib_u8 *argb1,
	mlib_s32 n)
{
	mlib_u8 *y0end = y0 + n;
	mlib_s32 off = (2 << 10) + (2 << 21);

/* 9 bits mask */
	mlib_s32 mask = (0x1ff << 10) + (0x1ff << 21);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; y0 <= (y0end - 4); y0 += 4) {
		mlib_u32 s0, s1, s2, s3;
		mlib_u32 t0, t1, t2, t3;
		mlib_u32 ss0, ss1;

		ARGB2JFIFYCC_4s(argb0, s0, s1, s2, s3);
		ARGB2JFIFYCC_4s(argb1, t0, t1, t2, t3);
		ss0 = ((s0 + s1) & mask) + ((t0 + t1) & mask);
		ss1 = ((s2 + s3) & mask) + (((t2 + t3) & mask) + off);
		y0[0] = s0;
		y0[1] = s1;
		y0[2] = s2;
		y0[3] = s3;
		y1[0] = t0;
		y1[1] = t1;
		y1[2] = t2;
		y1[3] = t3;
		cb[0] = ss0 >> 12;
		cb[1] = ss1 >> 12;
		cr[0] = ss0 >> 23;
		cr[1] = ss1 >> 23;

		y1 += 4;
		cb += 2;
		cr += 2;
		argb0 += 16;
		argb1 += 16;
	}

	if (y0 <= (y0end - 2)) {
		mlib_u32 s0, s1, t0, t1, ss0;

		RGB2JFIFYCC_1s(argb0 + 1, s0);
		RGB2JFIFYCC_1s(argb0 + 5, s1);
		RGB2JFIFYCC_1s(argb1 + 1, t0);
		RGB2JFIFYCC_1s(argb1 + 5, t1);
		ss0 = ((s0 + s1) & mask) + ((t0 + t1) & mask);
		y0[0] = s0;
		y0[1] = s1;
		y1[0] = t0;
		y1[1] = t1;
		cb[0] = ss0 >> 12;
		cr[0] = ss0 >> 23;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
