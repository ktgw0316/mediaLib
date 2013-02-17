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

#pragma ident	"@(#)mlib_v_VideoColorSplit4_S16.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorSplit4_S16 - split four channels colors
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorSplit4_S16(mlib_s16       *color1,
 *                                            mlib_s16       *color2,
 *                                            mlib_s16       *color3,
 *                                            mlib_s16       *color4,
 *                                            const mlib_s16 *colors,
 *                                            mlib_s32       n);
 * ARGUMENTS
 *      color1    Pointer to first  color component row, 8-byte aligned
 *      color2    Pointer to second color component row, 8-byte aligned
 *      color3    Pointer to third  color component row, 8-byte aligned
 *      color4    Pointer to forth  color component row, 8-byte aligned
 *      colors    Pointer to colors multicomponent row, 8-byte aligned
 *      n         Length of color1, color2, color3, color4 arrays
 *                Note: length of colors array must be 4*n
 *
 * DESCRIPTION
 *
 *      color1[i] = colors[4*i]
 *      color2[i] = colors[4*i+1]
 *      color3[i] = colors[4*i+2]
 *      color4[i] = colors[4*i+3]
 *
 *      0 <= i < n
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorSplit4_S16 = __mlib_VideoColorSplit4_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorSplit4_S16) mlib_VideoColorSplit4_S16
	__attribute__((weak, alias("__mlib_VideoColorSplit4_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoColorSplit4_S16(
	mlib_s16 *color1,
	mlib_s16 *color2,
	mlib_s16 *color3,
	mlib_s16 *color4,
	const mlib_s16 *colors,
	mlib_s32 n)
{
	mlib_d64 *sp = (mlib_d64 *)colors;
	mlib_d64 *dp0 = (mlib_d64 *)color1;
	mlib_d64 *dp1 = (mlib_d64 *)color2;
	mlib_d64 *dp2 = (mlib_d64 *)color3;
	mlib_d64 *dp3 = (mlib_d64 *)color4;
	mlib_d64 s0, s1, s2, s3, dd1, dd2, dd3;
	mlib_s32 i;

	vis_alignaddr((void *)0, 4);
	vis_write_bmask(0xDF13CE02, 0);

/*
 * 4-pixels loop
 */
#pragma pipeloop(0)
	for (i = 0; i <= (n - 4); i += 4) {
		s0 = sp[0];
		s1 = sp[1];
		s2 = sp[2];
		s3 = sp[3];

		dd1 = vis_fpmerge(vis_read_hi(s2), vis_read_hi(s0));
		dd2 = vis_fpmerge(vis_read_hi(s3), vis_read_hi(s1));
		dd3 = vis_faligndata(dd2, dd1);
		(*dp0++) = vis_bshuffle(dd2, dd3);
		(*dp1++) = vis_bshuffle(dd3, dd1);

		dd1 = vis_fpmerge(vis_read_lo(s2), vis_read_lo(s0));
		dd2 = vis_fpmerge(vis_read_lo(s3), vis_read_lo(s1));
		dd3 = vis_faligndata(dd2, dd1);
		(*dp2++) = vis_bshuffle(dd2, dd3);
		(*dp3++) = vis_bshuffle(dd3, dd1);

		sp += 4;
	}

/*
 * last 4 pixels
 */

	if (i < n) {
		mlib_s32 emask = 0xF0 >> (n & 3);

		s0 = sp[0];
		s1 = vis_ld_d64_nf(sp + 1);
		s2 = vis_ld_d64_nf(sp + 2);
		s3 = vis_ld_d64_nf(sp + 3);
		dd1 = vis_fpmerge(vis_read_hi(s2), vis_read_hi(s0));
		dd2 = vis_fpmerge(vis_read_hi(s3), vis_read_hi(s1));
		dd3 = vis_faligndata(dd2, dd1);
		vis_pst_16(vis_bshuffle(dd2, dd3), dp0, emask);
		vis_pst_16(vis_bshuffle(dd3, dd1), dp1, emask);
		dd1 = vis_fpmerge(vis_read_lo(s2), vis_read_lo(s0));
		dd2 = vis_fpmerge(vis_read_lo(s3), vis_read_lo(s1));
		dd3 = vis_faligndata(dd2, dd1);
		vis_pst_16(vis_bshuffle(dd2, dd3), dp2, emask);
		vis_pst_16(vis_bshuffle(dd3, dd1), dp3, emask);
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
