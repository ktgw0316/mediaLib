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

#pragma ident	"@(#)mlib_v_VideoColorMerge3_S16.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorMerge3_S16 - merges three channels colors
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorMerge3_S16(mlib_s16       *colors,
 *                                            const mlib_s16 *color1,
 *                                            const mlib_s16 *color2,
 *                                            const mlib_s16 *color3,
 *                                            mlib_s32       n);
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

#pragma weak mlib_VideoColorMerge3_S16 = __mlib_VideoColorMerge3_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorMerge3_S16) mlib_VideoColorMerge3_S16
	__attribute__((weak, alias("__mlib_VideoColorMerge3_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoColorMerge3_S16(
	mlib_s16 *colors,
	const mlib_s16 *color1,
	const mlib_s16 *color2,
	const mlib_s16 *color3,
	mlib_s32 n)
{
	mlib_f32 *dp = (mlib_f32 *)colors;
	mlib_d64 *sp0 = (mlib_d64 *)color1;
	mlib_d64 *sp1 = (mlib_d64 *)color2;
	mlib_d64 *sp2 = (mlib_d64 *)color3;
	mlib_d64 sd0, sd1, sd2, dd0, dd1, dd2;
	mlib_f32 f11 = vis_to_float(0x1000100);
	mlib_s32 i;

	vis_write_gsr(8 << 3);

/*
 * 4-pixels loop
 */
#pragma pipeloop(0)
	for (i = 0; i <= (n - 4); i += 4) {
		sd0 = (*sp0++);
		sd1 = (*sp1++);
		sd2 = (*sp2++);
		sd0 = vis_fpmerge(vis_read_hi(sd0), vis_read_lo(sd0));
		sd1 = vis_fpmerge(vis_read_hi(sd1), vis_read_lo(sd1));
		sd2 = vis_fpmerge(vis_read_hi(sd2), vis_read_lo(sd2));
		dd0 = vis_fpmerge(vis_read_hi(sd0), vis_read_hi(sd1));
		dd1 = vis_fpmerge(vis_read_hi(sd2), vis_read_lo(sd0));
		dd2 = vis_fpmerge(vis_read_lo(sd1), vis_read_lo(sd2));
		dd0 = vis_fpmerge(vis_read_hi(dd0), vis_read_lo(dd0));
		dd1 = vis_fpmerge(vis_read_hi(dd1), vis_read_lo(dd1));
		dd2 = vis_fpmerge(vis_read_hi(dd2), vis_read_lo(dd2));

/*
 * i.e. vis_freg_pair(vis_read_hi(dd0), vis_read_hi(dd1))
 */
		((mlib_d64 *)dp)[0] =
			vis_fpackfix_pair(vis_fmuld8sux16(f11,
			vis_read_hi(dd0)), vis_fmuld8sux16(f11,
			vis_read_hi(dd1)));
/*
 * i.e. vis_freg_pair(vis_read_hi(dd2), vis_read_lo(dd0))
 */
		((mlib_d64 *)dp)[1] =
			vis_fpackfix_pair(vis_fmuld8sux16(f11,
			vis_read_hi(dd2)), vis_fmuld8sux16(f11,
			vis_read_lo(dd0)));
		dp[4] = vis_read_lo(dd1);
		dp[5] = vis_read_lo(dd2);
		dp += 6;
	}

	for (; i < n; i++) {
		colors[3 * i] = ((mlib_u16 *)color1)[i];
		colors[3 * i + 1] = ((mlib_u16 *)color2)[i];
		colors[3 * i + 2] = ((mlib_u16 *)color3)[i];
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
