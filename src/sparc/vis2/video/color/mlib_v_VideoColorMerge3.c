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

#pragma ident	"@(#)mlib_v_VideoColorMerge3.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorMerge3 - merges three channels colors
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_v_VideoColorMerge3(mlib_u8       *colors,
 *                                          const mlib_u8 *color1,
 *                                          const mlib_u8 *color2,
 *                                          const mlib_u8 *color3,
 *                                          mlib_s32      n);
 * ARGUMENTS
 *      colors    Pointer to colors multicomponent row, 8-byte aligned
 *      color1    Pointer to first color component row, 8-byte aligned
 *      color2    Pointer to second color component row, 8-byte aligned
 *      color3    Pointer to third color component row, 8-byte aligned
 *      n         Length of the color1, color2, color3 arrays
 *
 * DESCRIPTION
 *
 *      colors[3*i]   = color1[i]
 *      colors[3*i+1] = color2[i]
 *      colors[3*i+2] = color3[i]
 *
 *      0 <= i < n
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorMerge3 = __mlib_VideoColorMerge3

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorMerge3) mlib_VideoColorMerge3
	__attribute__((weak, alias("__mlib_VideoColorMerge3")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoColorMerge3(
	mlib_u8 *colors,
	const mlib_u8 *color1,
	const mlib_u8 *color2,
	const mlib_u8 *color3,
	mlib_s32 n)
{
	mlib_d64 *dp = (mlib_d64 *)colors;
	mlib_d64 *sp0 = (mlib_d64 *)color1;
	mlib_d64 *sp1 = (mlib_d64 *)color2;
	mlib_d64 *sp2 = (mlib_d64 *)color3;
	mlib_d64 sd0, sd1, sd2, sd3, sd4, sd5;
	mlib_d64 dd0, dd1, dd2, dd3, dd4, dd5;
	mlib_s32 i;

/*
 * 16-pixels loop
 */
#pragma pipeloop(0)
	for (i = 0; i < (n / 16); i++) {
		sd0 = (*sp0++);
		sd1 = (*sp1++);
		sd2 = (*sp2++);
		sd3 = (*sp0++);
		sd4 = (*sp1++);
		sd5 = (*sp2++);
		vis_write_bmask(0x0801902A, 0);
		dd0 = vis_bshuffle(sd0, sd1);
		dd3 = vis_bshuffle(sd3, sd4);
		vis_write_bmask(0x03B04C05, 0);
		dd1 = vis_bshuffle(sd0, sd1);
		dd4 = vis_bshuffle(sd3, sd4);
		vis_write_bmask(0xD06E07F0, 0);
		dd2 = vis_bshuffle(sd0, sd1);
		dd5 = vis_bshuffle(sd3, sd4);
		vis_write_bmask(0x01834967, 0);
		dd0 = vis_bshuffle(dd0, sd2);
		dd3 = vis_bshuffle(dd3, sd5);
		vis_write_bmask(0xA12B45C7, 0);
		dd1 = vis_bshuffle(dd1, sd2);
		dd4 = vis_bshuffle(dd4, sd5);
		vis_write_bmask(0x0D23E56F, 0);
		dd2 = vis_bshuffle(dd2, sd2);
		dd5 = vis_bshuffle(dd5, sd5);
		dp[0] = dd0;
		dp[1] = dd1;
		dp[2] = dd2;
		dp[3] = dd3;
		dp[4] = dd4;
		dp[5] = dd5;
		dp += 6;
	}

/*
 * last pixels
 */

	if (n & 15) {
		mlib_u8 *_sp0 = (mlib_u8 *)sp0;
		mlib_u8 *_sp1 = (mlib_u8 *)sp1;
		mlib_u8 *_sp2 = (mlib_u8 *)sp2;
		mlib_u8 *_dp = (mlib_u8 *)dp;

		for (i = 0; i < (n & 15); i++) {
			(*_dp++) = (*_sp0++);
			(*_dp++) = (*_sp1++);
			(*_dp++) = (*_sp2++);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
