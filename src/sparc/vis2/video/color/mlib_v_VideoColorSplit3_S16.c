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

#pragma ident	"@(#)mlib_v_VideoColorSplit3_S16.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorSplit3_S16 - split three channels colors
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_v_VideoColorSplit3_S16(mlib_s16       *color1,
 *                                              mlib_s16       *color2,
 *                                              mlib_s16       *color3,
 *                                              const mlib_s16 *colors,
 *                                              mlib_s32       n);
 * ARGUMENTS
 *      color1    Pointer to first  color component row, 8-byte aligned
 *      color2    Pointer to second color component row, 8-byte aligned
 *      color3    Pointer to third  color component row, 8-byte aligned
 *      colors    Pointer to colors multicomponent row, 8-byte aligned
 *      n         Length of color1, color2, color3 arrays
 *                Note: length of colors array must be 3*n
 *
 * DESCRIPTION
 *
 *      color1[i] = colors[3*i]
 *      color2[i] = colors[3*i+1]
 *      color3[i] = colors[3*i+2]
 *
 *      0 <= i < n
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorSplit3_S16 = __mlib_VideoColorSplit3_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorSplit3_S16) mlib_VideoColorSplit3_S16
	__attribute__((weak, alias("__mlib_VideoColorSplit3_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoColorSplit3_S16(
	mlib_s16 *color1,
	mlib_s16 *color2,
	mlib_s16 *color3,
	const mlib_s16 *colors,
	mlib_s32 n)
{
	mlib_d64 *sp = (mlib_d64 *)colors;
	mlib_d64 *dp0 = (mlib_d64 *)color1;
	mlib_d64 *dp1 = (mlib_d64 *)color2;
	mlib_d64 *dp2 = (mlib_d64 *)color3;
	mlib_d64 sd0, sd1, sd2, dd0, dd1, dd2, dd3;
	mlib_s32 i;

	vis_write_gsr(4);
	vis_write_bmask(0x02CE13DF, 0);
#pragma pipeloop(0)
#pragma unroll(4)
	for (i = 0; i <= (n - 4); i += 4) {
		sd0 = sp[0];
		sd1 = sp[1];
		sd2 = sp[2];
		dd1 = vis_fpmerge(vis_read_lo(sd0), vis_read_hi(sd2));
		dd0 = vis_fpmerge(vis_read_hi(sd0), vis_read_lo(sd1));
		(*dp0++) = vis_bshuffle(dd0, dd1);
		dd2 = vis_fpmerge(vis_read_hi(sd1), vis_read_lo(sd2));
		dd3 = vis_faligndata(dd0, dd2);
		(*dp1++) = vis_bshuffle(dd3, dd3);
		(*dp2++) = vis_bshuffle(dd1, dd2);
		sp += 3;
	}

/*
 * last 4 pixels
 */

	if (i < n) {
		mlib_s32 emask = 0xF0 >> (n & 3);
		mlib_d64 st0, st1, st2;

		sd0 = sp[0];
		sd1 = vis_ld_d64_nf(sp + 1);
		sd2 = vis_ld_d64_nf(sp + 2);
		dd1 = vis_fpmerge(vis_read_lo(sd0), vis_read_hi(sd2));
		dd0 = vis_fpmerge(vis_read_hi(sd0), vis_read_lo(sd1));
		st0 = vis_bshuffle(dd0, dd1);
		dd2 = vis_fpmerge(vis_read_hi(sd1), vis_read_lo(sd2));
		dd3 = vis_faligndata(dd0, dd2);
		st1 = vis_bshuffle(dd3, dd3);
		st2 = vis_bshuffle(dd1, dd2);
		vis_pst_16(st0, dp0, emask);
		vis_pst_16(st1, dp1, emask);
		vis_pst_16(st2, dp2, emask);
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
