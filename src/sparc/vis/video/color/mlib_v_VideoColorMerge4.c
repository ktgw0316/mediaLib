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

#pragma ident	"@(#)mlib_v_VideoColorMerge4.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorMerge4 - merges four channels colors
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorMerge4(mlib_u8       *colors,
 *                                        const mlib_u8 *color1,
 *                                        const mlib_u8 *color2,
 *                                        const mlib_u8 *color3,
 *                                        const mlib_u8 *color4,
 *                                        mlib_s32      n);
 * ARGUMENTS
 *      colors    Pointer to colors multicomponent row, 8-byte aligned
 *      color1    Pointer to first color component row, 8-byte aligned
 *      color2    Pointer to second color component row, 8-byte aligned
 *      color3    Pointer to third color component row, 8-byte aligned
 *      color4    Pointer to forth color component row, 8-byte aligned
 *      n         Length of the color1 array
 *
 * DESCRIPTION
 *
 *      colors[4*i]   = color1[i]
 *      colors[4*i+1] = color2[i]
 *      colors[4*i+2] = color3[i]
 *      colors[4*i+3] = color4[i]
 *
 *      0 <= i < n
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorMerge4 = __mlib_VideoColorMerge4

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorMerge4) mlib_VideoColorMerge4
	__attribute__((weak, alias("__mlib_VideoColorMerge4")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_MERGE4_U8(sd0, sd1, sd2, sd3, dd0, dd1, dd2, dd3)   \
	h02 = vis_fpmerge(vis_read_hi(sd0), vis_read_hi(sd2));   \
	h13 = vis_fpmerge(vis_read_hi(sd1), vis_read_hi(sd3));   \
	dd0 = vis_fpmerge(vis_read_hi(h02), vis_read_hi(h13));   \
	dd1 = vis_fpmerge(vis_read_lo(h02), vis_read_lo(h13));   \
	h02 = vis_fpmerge(vis_read_lo(sd0), vis_read_lo(sd2));   \
	h13 = vis_fpmerge(vis_read_lo(sd1), vis_read_lo(sd3));   \
	dd2 = vis_fpmerge(vis_read_hi(h02), vis_read_hi(h13));   \
	dd3 = vis_fpmerge(vis_read_lo(h02), vis_read_lo(h13))

/* *********************************************************** */

mlib_status
__mlib_VideoColorMerge4(
	mlib_u8 *colors,
	const mlib_u8 *color1,
	const mlib_u8 *color2,
	const mlib_u8 *color3,
	const mlib_u8 *color4,
	mlib_s32 n)
{
	mlib_d64 *dp = (mlib_d64 *)colors;
	mlib_d64 *sp0 = (mlib_d64 *)color1;
	mlib_d64 *sp1 = (mlib_d64 *)color2;
	mlib_d64 *sp2 = (mlib_d64 *)color3;
	mlib_d64 *sp3 = (mlib_d64 *)color4;
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_d64 dd0, dd1, dd2, dd3;
	mlib_d64 h02, h13;
	mlib_s32 i;

/*
 * 8-pixels loop
 */
#pragma pipeloop(0)
	for (i = 0; i < (n / 8); i++) {
		sd0 = (*sp0++);
		sd1 = (*sp1++);
		sd2 = (*sp2++);
		sd3 = (*sp3++);
		MLIB_MERGE4_U8(sd0, sd1, sd2, sd3, dd0, dd1, dd2, dd3);
		(*dp++) = dd0;
		(*dp++) = dd1;
		(*dp++) = dd2;
		(*dp++) = dd3;
	}

/*
 * last 8 pixels
 */

	if (n & 7) {
		mlib_d64 dbuf[4];
		mlib_f32 *fbuf = (mlib_f32 *)dbuf;
		mlib_f32 *dpf = (mlib_f32 *)dp;

		sd0 = (*sp0++);
		sd1 = (*sp1++);
		sd2 = (*sp2++);
		sd3 = (*sp3++);
		MLIB_MERGE4_U8(sd0, sd1, sd2, sd3, dbuf[0], dbuf[1], dbuf[2],
			dbuf[3]);
		for (i = 0; i < (n & 7); i++)
			dpf[i] = fbuf[i];
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
