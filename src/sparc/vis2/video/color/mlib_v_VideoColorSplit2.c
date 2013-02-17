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

#pragma ident	"@(#)mlib_v_VideoColorSplit2.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorSplit2 - split two channels colors
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_v_VideoColorSplit2(mlib_u8       *color1,
 *                                          mlib_u8       *color2,
 *                                          const mlib_u8 *colors,
 *                                          mlib_s32      n);
 * ARGUMENTS
 *      color1    Pointer to first  color component row, 8-byte aligned
 *      color2    Pointer to second color component row, 8-byte aligned
 *      colors    Pointer to colors multicomponent row, 8-byte aligned
 *      n         Length of color1, color2 arrays
 *                Note: length of colors array must be 2*n
 *
 * DESCRIPTION
 *
 *        color1[i] = colors[2*i]
 *        color2[i] = colors[2*i+1]
 *
 *        0 <= i < n
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorSplit2 = __mlib_VideoColorSplit2

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorSplit2) mlib_VideoColorSplit2
	__attribute__((weak, alias("__mlib_VideoColorSplit2")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoColorSplit2(
	mlib_u8 *color1,
	mlib_u8 *color2,
	const mlib_u8 *colors,
	mlib_s32 n)
{
	mlib_d64 *sp = (mlib_d64 *)colors;
	mlib_d64 *dp0 = (mlib_d64 *)color1;
	mlib_d64 *dp1 = (mlib_d64 *)color2;
	mlib_d64 sd0, sd1, dd0, dd1, dd2, dd3;
	mlib_s32 i;

	vis_write_gsr64(((mlib_u64)0x02468ace << 32) | 1);
/*
 * 8-pixels loop
 */
	sd0 = (*sp++);
	sd1 = vis_ld_d64_nf(sp); sp++;
#pragma pipeloop(0)
	for (i = 0; i < (n / 8); i++) {
		dd0 = vis_faligndata(sd0, sd1);
		dd1 = vis_faligndata(sd1, sd0);
		(*dp0++) = vis_bshuffle(sd0, sd1);
		(*dp1++) = vis_bshuffle(dd0, dd1);
		sd0 = vis_ld_d64_nf(sp); sp++;
		sd1 = vis_ld_d64_nf(sp); sp++;
	}

/*
 * last 8 pixels
 */

	if (n & 7) {
		mlib_s32 emask = (0xFF00 >> (n & 7)) & 0xFF;

		dd0 = vis_faligndata(sd0, sd1);
		dd1 = vis_faligndata(sd1, sd0);
		dd2 = vis_bshuffle(sd0, sd1);
		dd3 = vis_bshuffle(dd0, dd1);
		vis_pst_8(dd2, (mlib_f32 *)dp0, emask);
		vis_pst_8(dd3, (mlib_f32 *)dp1, emask);
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
