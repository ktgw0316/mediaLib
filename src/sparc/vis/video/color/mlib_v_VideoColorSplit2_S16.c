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

#pragma ident	"@(#)mlib_v_VideoColorSplit2_S16.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorSplit2_S16 - split two channels colors
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorSplit2_S16(mlib_s16       *color1,
 *                                            mlib_s16       *color2,
 *                                            const mlib_s16 *colors,
 *                                            mlib_s32       n);
 * ARGUMENTS
 *      color1    Pointer to first  color component row, 8-byte aligned
 *      color2    Pointer to second color component row, 8-byte aligned
 *      colors    Pointer to colors multicomponent row, 8-byte aligned
 *      n         Length of color1, color2 arrays
 *                Note: length of colors array must be 2*n
 *
 * DESCRIPTION
 *
 *      color1[i] = colors[2*i]
 *      color2[i] = colors[2*i+1]
 *
 *      0 <= i < n
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorSplit2_S16 = __mlib_VideoColorSplit2_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorSplit2_S16) mlib_VideoColorSplit2_S16
	__attribute__((weak, alias("__mlib_VideoColorSplit2_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoColorSplit2_S16(
	mlib_s16 *color1,
	mlib_s16 *color2,
	const mlib_s16 *colors,
	mlib_s32 n)
{
	mlib_d64 *sp = (mlib_d64 *)colors;
	mlib_d64 *dp0 = (mlib_d64 *)color1;
	mlib_d64 *dp1 = (mlib_d64 *)color2;
	mlib_d64 sd0, sd1, sd2, sd3, dd0, dd1, dr, dr1;
	mlib_s32 i;

	vis_write_gsr(2);

/*
 * 8-pixels loop
 */
#pragma pipeloop(0)
	for (i = 0; i <= (n - 8); i += 8) {
		sd0 = sp[0];
		sd1 = sp[1];
		sd2 = sp[2];
		sd3 = sp[3];
		dr = vis_fpmerge(vis_read_hi(sd0), vis_read_hi(sd1));
		dr1 = vis_fpmerge(vis_read_lo(sd0), vis_read_lo(sd1));
		dd0 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));
		dp0[0] = vis_fpmerge(vis_read_hi(dd0), vis_read_lo(dd0));
		dd1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr1));
		dp1[0] = vis_fpmerge(vis_read_hi(dd1), vis_read_lo(dd1));
		dd0 = vis_faligndata(sd2, sd2);
		dd1 = vis_fpack32(sd3, sd3);
		dd1 = vis_fpack32(dd1, dd1);
		dp0[1] = vis_fpackfix_pair(sd2, sd3);
		dp1[1] = vis_fpackfix_pair(dd0, dd1);
		sp += 4;
		dp0 += 2;
		dp1 += 2;
	}

/*
 * next 4 pixels
 */

	if (i <= (n - 4)) {
		sd0 = sp[0];
		sd1 = sp[1];
		dr = vis_fpmerge(vis_read_hi(sd0), vis_read_hi(sd1));
		dr1 = vis_fpmerge(vis_read_lo(sd0), vis_read_lo(sd1));
		dd0 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));
		(*dp0++) = vis_fpmerge(vis_read_hi(dd0), vis_read_lo(dd0));
		dd1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr1));
		(*dp1++) = vis_fpmerge(vis_read_hi(dd1), vis_read_lo(dd1));
		sp += 2;
		i += 4;
	}

/*
 * last 4 pixels
 */

	if (i < n) {
		mlib_s32 emask = 0xF0 >> (n & 3);

		sd0 = sp[0];
		sd1 = vis_ld_d64_nf(sp + 1);
		dr = vis_fpmerge(vis_read_hi(sd0), vis_read_hi(sd1));
		dr1 = vis_fpmerge(vis_read_lo(sd0), vis_read_lo(sd1));
		dd0 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));
		dd0 = vis_fpmerge(vis_read_hi(dd0), vis_read_lo(dd0));
		dd1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr1));
		dd1 = vis_fpmerge(vis_read_hi(dd1), vis_read_lo(dd1));
		vis_pst_16(dd0, dp0, emask);
		vis_pst_16(dd1, dp1, emask);
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
