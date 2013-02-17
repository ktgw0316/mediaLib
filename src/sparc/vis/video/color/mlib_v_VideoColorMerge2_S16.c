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

#pragma ident	"@(#)mlib_v_VideoColorMerge2_S16.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorMerge2_S16 - merges two channels colors
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorMerge2_S16(mlib_s16       *colors,
 *                                            const mlib_s16 *color1,
 *                                            const mlib_s16 *color2,
 *                                            mlib_s32       n);
 * ARGUMENTS
 *      colors    Pointer to colors multicomponent row, 8-byte aligned
 *      color1    Pointer to first color component row, 8-byte aligned
 *      color2    Pointer to second color component row, 8-byte aligned
 *      n         Length of the color1 array
 *
 * DESCRIPTION
 *
 *      colors[2*i]   = color1[i]
 *      colors[2*i+1] = color2[i]
 *
 *      0 <= i < n
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorMerge2_S16 = __mlib_VideoColorMerge2_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorMerge2_S16) mlib_VideoColorMerge2_S16
	__attribute__((weak, alias("__mlib_VideoColorMerge2_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoColorMerge2_S16(
	mlib_s16 *colors,
	const mlib_s16 *color1,
	const mlib_s16 *color2,
	mlib_s32 n)
{
	mlib_d64 *dp = (mlib_d64 *)colors;
	mlib_d64 *sp0 = (mlib_d64 *)color1;
	mlib_d64 *sp1 = (mlib_d64 *)color2;
	mlib_d64 sd0, sd1, sd2, sd3, dd0, dd1, dr, dr1;
	mlib_d64 dmask = vis_to_double_dup(0xFFFF);
	mlib_f32 f11 = vis_to_float(0x10001), f1010 = vis_to_float(0x1000100);
	mlib_s32 i;

	vis_write_gsr(0);

/*
 * 8-pixels loop
 */
#pragma pipeloop(0)
	for (i = 0; i <= (n - 8); i += 8) {
		sd0 = sp0[0];
		sd1 = sp1[0];
		sd2 = sp0[1];
		sd3 = sp1[1];
		dd0 = vis_fpmerge(vis_read_hi(sd0), vis_read_lo(sd0));
		dd0 = vis_fpmerge(vis_read_hi(dd0), vis_read_lo(dd0));
		dd1 = vis_fpmerge(vis_read_hi(sd1), vis_read_lo(sd1));
		dd1 = vis_fpmerge(vis_read_hi(dd1), vis_read_lo(dd1));
		dr = vis_fpmerge(vis_read_hi(dd0), vis_read_hi(dd1));
		dr1 = vis_fpmerge(vis_read_lo(dd0), vis_read_lo(dd1));
		dp[0] = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));
		dp[1] = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr1));

		dd0 = vis_fmuld8sux16(f1010, vis_read_hi(sd2));
		dd1 = vis_fmuld8sux16(f1010, vis_read_lo(sd2));
		dr = vis_fmuld8ulx16(f11, vis_read_hi(sd3));
		dr1 = vis_fmuld8ulx16(f11, vis_read_lo(sd3));
		dd0 = vis_fpack32(dd0, dmask);
		dd1 = vis_fpack32(dd1, dmask);
		dp[2] = vis_for(vis_fand(dmask, dr), dd0);
		dp[3] = dr1;
		vis_pst_16(dd1, dp + 3, 0xa);
		sp0 += 2;
		sp1 += 2;
		dp += 4;
	}

/*
 * next 4 pixels
 */

	if (i <= (n - 4)) {
		sd0 = (*sp0++);
		sd1 = (*sp1++);
		dd0 = vis_fpmerge(vis_read_hi(sd0), vis_read_lo(sd0));
		dd0 = vis_fpmerge(vis_read_hi(dd0), vis_read_lo(dd0));
		dd1 = vis_fpmerge(vis_read_hi(sd1), vis_read_lo(sd1));
		dd1 = vis_fpmerge(vis_read_hi(dd1), vis_read_lo(dd1));
		dr = vis_fpmerge(vis_read_hi(dd0), vis_read_hi(dd1));
		dr1 = vis_fpmerge(vis_read_lo(dd0), vis_read_lo(dd1));
		dp[0] = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));
		dp[1] = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr1));
		dp += 2;
		i += 4;
	}

/*
 * last 4 pixels
 */

	if (i < n) {
		mlib_s32 emask = 0xF0 >> (n & 3);

		sd0 = sp0[0];
		sd1 = sp1[0];
		dd0 = vis_fpmerge(vis_read_hi(sd0), vis_read_lo(sd0));
		dd0 = vis_fpmerge(vis_read_hi(dd0), vis_read_lo(dd0));
		dd1 = vis_fpmerge(vis_read_hi(sd1), vis_read_lo(sd1));
		dd1 = vis_fpmerge(vis_read_hi(dd1), vis_read_lo(dd1));
		dr = vis_fpmerge(vis_read_hi(dd0), vis_read_hi(dd1));
		dr1 = vis_fpmerge(vis_read_lo(dd0), vis_read_lo(dd1));
		dd0 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));
		vis_pst_32(dd0, dp, emask >> 2);

		if (emask & 0x3) {
			dd1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr1));
			vis_pst_32(dd1, dp + 1, emask);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
