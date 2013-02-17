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

#pragma ident	"@(#)mlib_v_VideoColorRGB2ARGB.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorRGB2ARGB - RGB to ARGB conversion
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorRGB2ARGB(mlib_u8       *argb,
 *                                          const mlib_u8 *rgb,
 *                                          mlib_s32      n);
 * ARGUMENTS
 *      argb      Pointer to ARGB multicomponent row, 8-byte aligned
 *      rgb       Pointer to  RGB multicomponent row, 8-byte aligned
 *      n         Number of pixels
 *
 * DESCRIPTION
 *
 *      argb[4*i] - undefined
 *      argb[4*i+1] = rgb[3*i]
 *      argb[4*i+2] = rgb[3*i+1]
 *      argb[4*i+3] = rgb[3*i+2]
 *
 *      0 <= i < n
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorRGB2ARGB = __mlib_VideoColorRGB2ARGB

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorRGB2ARGB) mlib_VideoColorRGB2ARGB
	__attribute__((weak, alias("__mlib_VideoColorRGB2ARGB")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MERGE_S32_U8(x, a0, a1, a2)	x = (a0 << 16) | (a1 << 8) | (a2)

/* *********************************************************** */

mlib_status
__mlib_VideoColorRGB2ARGB(
	mlib_u8 *argb,
	const mlib_u8 *rgb,
	mlib_s32 n)
{
/* 8-byte aligned start points in dst */
	mlib_d64 *dp = (mlib_d64 *)argb;

/* 8-byte aligned start point in src */
	mlib_d64 *sp = (mlib_d64 *)rgb;

/* 8-byte source data */
	mlib_d64 sd0, sd1, sd2;

/* intermediate variables */
	mlib_d64 sda, sdb;
	mlib_s32 i;
	mlib_d64 zero = vis_fzero();

	if (n <= 0)
		return (MLIB_FAILURE);

	vis_alignaddr(sp, 6);
#pragma pipeloop(0)
	for (i = 0; i <= (n - 8); i += 8) {
/* r0g0b0r1g1b1r2g2 */
		sd0 = sp[0];
/* b2r3g3b3r4g4b4r5 */
		sd1 = sp[1];
/* g5b5r6g6b6r7g7b7 */
		sd2 = sp[2];
		sda = vis_faligndata(sd0, sd1);
		sdb = vis_freg_pair(vis_read_lo(sd1), vis_read_hi(sd2));
		vis_write_bmask(0x80128345, 0);
		dp[0] = vis_bshuffle(sd0, zero);
		dp[1] = vis_bshuffle(sda, zero);
		dp[2] = vis_bshuffle(sdb, zero);
		vis_write_bmask(0x0abc0def, 0);
		dp[3] = vis_bshuffle(zero, sd2);
		sp += 3;
		dp += 4;
	}

	for (; i < n; i++) {
		MERGE_S32_U8(((mlib_u32 *)argb)[i], rgb[3 * i], rgb[3 * i + 1],
			rgb[3 * i + 2]);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
