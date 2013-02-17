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

#pragma ident	"@(#)mlib_v_VideoColorMerge3.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorMerge3 - merges three channels colors
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorMerge3(mlib_u8       *colors,
 *                                        const mlib_u8 *color1,
 *                                        const mlib_u8 *color2,
 *                                        const mlib_u8 *color3,
 *                                        mlib_s32      n);
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

#pragma weak mlib_VideoColorMerge3 = __mlib_VideoColorMerge3

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorMerge3) mlib_VideoColorMerge3
	__attribute__((weak, alias("__mlib_VideoColorMerge3")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_UNMERGE(dd, sf0, sf1)                                \
	{                                                         \
	    mlib_d64 dt;                                          \
	                                                          \
	    dt = vis_fpmerge(sf0, sf1);                           \
	    dd = vis_fpmerge(vis_read_hi(dt), vis_read_lo(dt));   \
	}

/* *********************************************************** */

#define	MLIB_MERGE3_U8(sd0, sd1, sd2, dd0, dd1, dd2)             \
	MLIB_UNMERGE(dt0, vis_read_hi(sd0), vis_read_lo(sd0));   \
	MLIB_UNMERGE(dt1, vis_read_hi(sd1), vis_read_lo(sd1));   \
	MLIB_UNMERGE(dt2, vis_read_hi(sd2), vis_read_lo(sd2));   \
	MLIB_UNMERGE(ds0, vis_read_hi(dt0), vis_read_hi(dt1));   \
	MLIB_UNMERGE(ds1, vis_read_hi(dt2), vis_read_lo(dt0));   \
	MLIB_UNMERGE(ds2, vis_read_lo(dt1), vis_read_lo(dt2));   \
	MLIB_UNMERGE(dd0, vis_read_hi(ds0), vis_read_hi(ds1));   \
	MLIB_UNMERGE(dd1, vis_read_hi(ds2), vis_read_lo(ds0));   \
	MLIB_UNMERGE(dd2, vis_read_lo(ds1), vis_read_lo(ds2))

/* *********************************************************** */

mlib_status
__mlib_VideoColorMerge3(
	mlib_u8 *colors,
	const mlib_u8 *color1,
	const mlib_u8 *color2,
	const mlib_u8 *color3,
	mlib_s32 n)
{
	mlib_f32 *dp = (mlib_f32 *)colors;
	mlib_d64 *sp0 = (mlib_d64 *)color1;
	mlib_d64 *sp1 = (mlib_d64 *)color2;
	mlib_d64 *sp2 = (mlib_d64 *)color3;
	mlib_d64 sd0, sd1, sd2;
	mlib_d64 dd0, dd1, dd2;
	mlib_d64 dt0, dt1, dt2;
	mlib_d64 ds0, ds1, ds2;
	int i;

/*
 * 8-pixels loop
 */
#pragma pipeloop(0)
	for (i = 0; i < (n / 8); i++) {
		sd0 = (*sp0++);
		sd1 = (*sp1++);
		sd2 = (*sp2++);
		MLIB_MERGE3_U8(sd0, sd1, sd2, dd0, dd1, dd2);
		(*dp++) = vis_read_hi(dd0);
		(*dp++) = vis_read_hi(dd1);
		(*dp++) = vis_read_hi(dd2);
		(*dp++) = vis_read_lo(dd0);
		(*dp++) = vis_read_lo(dd1);
		(*dp++) = vis_read_lo(dd2);
	}

/*
 * last 8 pixels
 */

	if (n & 7) {
		mlib_u8 *_sp0 = (mlib_u8 *)sp0;
		mlib_u8 *_sp1 = (mlib_u8 *)sp1;
		mlib_u8 *_sp2 = (mlib_u8 *)sp2;
		mlib_u8 *_dp = (mlib_u8 *)dp;

		for (i = 0; i < (n & 7); i++) {
			(*_dp++) = (*_sp0++);
			(*_dp++) = (*_sp1++);
			(*_dp++) = (*_sp2++);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
