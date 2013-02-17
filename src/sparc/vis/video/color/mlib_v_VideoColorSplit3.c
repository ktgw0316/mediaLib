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

#pragma ident	"@(#)mlib_v_VideoColorSplit3.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorSplit3 - split three channels colors
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorSplit3(mlib_u8       *color1,
 *                                        mlib_u8       *color2,
 *                                        mlib_u8       *color3,
 *                                        const mlib_u8 *colors,
 *                                        mlib_s32      n);
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
 *        color1[i] = colors[3*i]
 *        color2[i] = colors[3*i+1]
 *        color3[i] = colors[3*i+2]
 *
 *        0 <= i < n
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorSplit3 = __mlib_VideoColorSplit3

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorSplit3) mlib_VideoColorSplit3
	__attribute__((weak, alias("__mlib_VideoColorSplit3")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_SPLIT3_U8(sd0, sd1, sd2, dd0, dd1, dd2)             \
	sda = vis_fpmerge(vis_read_hi(sd0), vis_read_lo(sd1));   \
	sdb = vis_fpmerge(vis_read_lo(sd0), vis_read_hi(sd2));   \
	sdc = vis_fpmerge(vis_read_hi(sd1), vis_read_lo(sd2));   \
	sdd = vis_fpmerge(vis_read_hi(sda), vis_read_lo(sdb));   \
	sde = vis_fpmerge(vis_read_lo(sda), vis_read_hi(sdc));   \
	dd0 = vis_fpmerge(vis_read_hi(sdd), vis_read_lo(sde));   \
	sde = vis_fpmerge(vis_read_hi(sdb), vis_read_lo(sdc));   \
	dd1 = vis_fpmerge(vis_read_lo(sdd), vis_read_hi(sde));   \
	sdd = vis_fpmerge(vis_read_lo(sda), vis_read_hi(sdc));   \
	dd2 = vis_fpmerge(vis_read_hi(sdd), vis_read_lo(sde))

/* *********************************************************** */

mlib_status
__mlib_VideoColorSplit3(
	mlib_u8 *color1,
	mlib_u8 *color2,
	mlib_u8 *color3,
	const mlib_u8 *colors,
	mlib_s32 n)
{
	mlib_d64 *sp = (mlib_d64 *)colors;
	mlib_d64 *dp0 = (mlib_d64 *)color1;
	mlib_d64 *dp1 = (mlib_d64 *)color2;
	mlib_d64 *dp2 = (mlib_d64 *)color3;
	mlib_d64 sd0, sd1, sd2, dd0, dd1, dd2;
	mlib_d64 sda, sdb, sdc, sdd, sde;
	mlib_s32 i;

/*
 * 8-pixels loop
 */
#pragma pipeloop(0)
	for (i = 0; i < (n / 8); i++) {
		sd0 = (*sp++);
		sd1 = (*sp++);
		sd2 = (*sp++);
		MLIB_SPLIT3_U8(sd0, sd1, sd2, dd0, dd1, dd2);
		(*dp0++) = dd0;
		(*dp1++) = dd1;
		(*dp2++) = dd2;
	}

/*
 * last 8 pixels
 */

	if (n & 7) {
		mlib_s32 emask = (0xFF00 >> (n & 7)) & 0xFF;

		sd0 = (*sp++);
		sd1 = vis_ld_d64_nf(sp); sp++;
		sd2 = vis_ld_d64_nf(sp);
		MLIB_SPLIT3_U8(sd0, sd1, sd2, dd0, dd1, dd2);
		vis_pst_8(dd0, dp0, emask);
		vis_pst_8(dd1, dp1, emask);
		vis_pst_8(dd2, dp2, emask);
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
