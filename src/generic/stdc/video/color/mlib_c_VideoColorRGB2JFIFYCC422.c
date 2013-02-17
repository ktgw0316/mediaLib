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

#pragma ident	"@(#)mlib_c_VideoColorRGB2JFIFYCC422.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorRGB2JFIFYCC422 - color conversion with
 *                                      422 downsampling with bilinear filter
 *                                      Only Cr and Cb components are
 *                                      under upsampling.
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorRGB2JFIFYCC422(mlib_u8       *y,
 *                                                mlib_u8       *cb,
 *                                                mlib_u8       *cr,
 *                                                const mlib_u8 *rgb,
 *                                                mlib_s32      n)
 *
 * ARGUMENTS
 *      y         Pointer to Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      rgb       Pointer to RGB multicomponent row, 8-byte aligned
 *      n         Length of y array
 *                Note: n must be even,
 *                      length of cb, cr arrays must be n/2
 *                      length of rgb array must be 3*n
 *
 * DESCRIPTION
 *      mlib_VideoColorRGB2JFIFYCC422:
 *        composition of mlib_VideoColorRGB2JFIFYCC444 with
 *                       mlib_VideoDownSample422
 *
 */

#include <mlib_video.h>
#include <mlib_c_VideoColorJFIFYCC.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorRGB2JFIFYCC422 = __mlib_VideoColorRGB2JFIFYCC422

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorRGB2JFIFYCC422) mlib_VideoColorRGB2JFIFYCC422
	__attribute__((weak, alias("__mlib_VideoColorRGB2JFIFYCC422")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoColorRGB2JFIFYCC422(
	mlib_u8 *y,
	mlib_u8 *cb,
	mlib_u8 *cr,
	const mlib_u8 *rgb,
	mlib_s32 n)
{
	mlib_u8 *yend = y + n;
	mlib_s32 off = (1 << 10) + (1 << 21);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; y <= (yend - 4); y += 4) {
		mlib_u32 s0, s1, s2, s3, ss0, ss1;

		RGB2JFIFYCC_4s(rgb, s0, s1, s2, s3);
		ss0 = s0 + s1;
		ss1 = s2 + s3 + off;
		y[0] = s0;
		y[1] = s1;
		y[2] = s2;
		y[3] = s3;
		cb[0] = ss0 >> 11;
		cb[1] = ss1 >> 11;
		cr[0] = ss0 >> 22;
		cr[1] = ss1 >> 22;

		cb += 2;
		cr += 2;
		rgb += 12;
	}

	if (y <= (yend - 2)) {
		mlib_u32 s0, s1, ss0;

		RGB2JFIFYCC_1s(rgb, s0);
		RGB2JFIFYCC_1s(rgb + 3, s1);
		ss0 = s0 + s1;
		y[0] = s0;
		y[1] = s1;
		cb[0] = ss0 >> 11;
		cr[0] = ss0 >> 22;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
