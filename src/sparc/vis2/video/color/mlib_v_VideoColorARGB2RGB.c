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

#pragma ident	"@(#)mlib_v_VideoColorARGB2RGB.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorARGB2RGB - ARGB to RGB conversion
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_v_VideoColorARGB2RGB(mlib_u8       *rgb,
 *                                            const mlib_u8 *argb,
 *                                            mlib_s32      n);
 * ARGUMENTS
 *      rgb       Pointer to  RGB multicomponent row, 8-byte aligned
 *      argb      Pointer to ARGB multicomponent row, 8-byte aligned
 *      n         Number of pixels
 *
 * DESCRIPTION
 *
 *      rgb[3*i  ] = argb[4*i+1]
 *      rgb[3*i+1] = argb[4*i+2]
 *      rgb[3*i+2] = argb[4*i+3]
 *
 *      0 <= i < n
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorARGB2RGB = __mlib_VideoColorARGB2RGB

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorARGB2RGB) mlib_VideoColorARGB2RGB
	__attribute__((weak, alias("__mlib_VideoColorARGB2RGB")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SPLIT_S32_3_U8(x, a1, a2, a3)                           \
	a1 = (x) >> 16;                                         \
	a2 = (x) >> 8;                                          \
	a3 = (x)

/* *********************************************************** */

mlib_status
__mlib_VideoColorARGB2RGB(
	mlib_u8 *rgb,
	const mlib_u8 *argb,
	mlib_s32 n)
{
/* 8-byte aligned start points in dst */
	mlib_d64 *dp = (mlib_d64 *)rgb;

/* 8-byte aligned start point in src */
	mlib_d64 *sp = (mlib_d64 *)argb;

/* 8-byte source data */
	mlib_d64 sd0, sd1, sd2, sd3;

/* 8-byte source data */
	mlib_d64 sd4, sd5, sd6, sd7;
	mlib_s32 i;

	if (n <= 0)
		return (MLIB_FAILURE);

/* --r0g0b0--r1g1b1 */
	sd0 = sp[0];
/* --r2g2b2--r3g3b3 */
	sd1 = vis_ld_d64_nf(sp + 1);
/* --r4g4b4--r5g5b5 */
	sd2 = vis_ld_d64_nf(sp + 2);
/* --r6g6b6--r7g7b7 */
	sd3 = vis_ld_d64_nf(sp + 3);
	sd4 = vis_ld_d64_nf(sp + 4);
	sd5 = vis_ld_d64_nf(sp + 5);
	sd6 = vis_ld_d64_nf(sp + 6);
	sd7 = vis_ld_d64_nf(sp + 7);
#pragma pipeloop(0)
	for (i = 0; i <= (n - 16); i += 16) {
		vis_write_bmask(0x1235679a, 0);
		dp[0] = vis_bshuffle(sd0, sd1);
		dp[3] = vis_bshuffle(sd4, sd5);
		vis_write_bmask(0x35679abd, 0);
		dp[1] = vis_bshuffle(sd1, sd2);
		dp[4] = vis_bshuffle(sd5, sd6);
		vis_write_bmask(0x679abdef, 0);
		dp[2] = vis_bshuffle(sd2, sd3);
		dp[5] = vis_bshuffle(sd6, sd7);
		sd0 = vis_ld_d64_nf(sp + 8);
		sd1 = vis_ld_d64_nf(sp + 9);
		sd2 = vis_ld_d64_nf(sp + 10);
		sd3 = vis_ld_d64_nf(sp + 11);
		sd4 = vis_ld_d64_nf(sp + 12);
		sd5 = vis_ld_d64_nf(sp + 13);
		sd6 = vis_ld_d64_nf(sp + 14);
		sd7 = vis_ld_d64_nf(sp + 15);
		sp += 8;
		dp += 6;
	}

#pragma pipeloop(0)
	for (; i < n; i++) {
		SPLIT_S32_3_U8(((mlib_u32 *)argb)[i], rgb[3 * i],
			rgb[3 * i + 1], rgb[3 * i + 2]);
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
