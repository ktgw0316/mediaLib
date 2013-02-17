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

#pragma ident	"@(#)mlib_v_VideoColorSplit4.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorSplit4 - split four channels colors
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorSplit4(mlib_u8       *color1,
 *                                        mlib_u8       *color2,
 *                                        mlib_u8       *color3,
 *                                        mlib_u8       *color4,
 *                                        const mlib_u8 *colors,
 *                                        mlib_s32      n);
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
 *        color1[i] = colors[4*i]
 *        color2[i] = colors[4*i+1]
 *        color3[i] = colors[4*i+2]
 *        color4[i] = colors[4*i+3]
 *
 *        0 <= i < n
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorSplit4 = __mlib_VideoColorSplit4

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorSplit4) mlib_VideoColorSplit4
	__attribute__((weak, alias("__mlib_VideoColorSplit4")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_LOAD_PREP_U8(sp)                                       \
	sd01 = vis_ld_d64_nf(sp); sp++;                             \
	sd23 = vis_ld_d64_nf(sp); sp++;                             \
	sd45 = vis_ld_d64_nf(sp); sp++;                             \
	sd67 = vis_ld_d64_nf(sp); sp++;                             \
	sd04 = vis_fpmerge(vis_read_hi(sd01), vis_read_hi(sd45));   \
	sd26 = vis_fpmerge(vis_read_hi(sd23), vis_read_hi(sd67));   \
	sd15 = vis_fpmerge(vis_read_lo(sd01), vis_read_lo(sd45));   \
	sd37 = vis_fpmerge(vis_read_lo(sd23), vis_read_lo(sd67))

/* *********************************************************** */

#define	MLIB_LOAD_SPLIT4_U8(sp, dd0, dd1, dd2, dd3)                 \
	sd01 = vis_ld_d64_nf(sp); sp++;                             \
	sd23 = vis_ld_d64_nf(sp); sp++;                             \
	sd45 = vis_ld_d64_nf(sp); sp++;                             \
	sd67 = vis_ld_d64_nf(sp); sp++;                             \
	dh0 = vis_fpmerge(vis_read_hi(sd04), vis_read_hi(sd26));    \
	dh1 = vis_fpmerge(vis_read_lo(sd04), vis_read_lo(sd26));    \
	dl0 = vis_fpmerge(vis_read_hi(sd15), vis_read_hi(sd37));    \
	dl1 = vis_fpmerge(vis_read_lo(sd15), vis_read_lo(sd37));    \
	dd0 = vis_fpmerge(vis_read_hi(dh0), vis_read_hi(dl0));      \
	dd1 = vis_fpmerge(vis_read_lo(dh0), vis_read_lo(dl0));      \
	dd2 = vis_fpmerge(vis_read_hi(dh1), vis_read_hi(dl1));      \
	dd3 = vis_fpmerge(vis_read_lo(dh1), vis_read_lo(dl1));      \
	sd04 = vis_fpmerge(vis_read_hi(sd01), vis_read_hi(sd45));   \
	sd26 = vis_fpmerge(vis_read_hi(sd23), vis_read_hi(sd67));   \
	sd15 = vis_fpmerge(vis_read_lo(sd01), vis_read_lo(sd45));   \
	sd37 = vis_fpmerge(vis_read_lo(sd23), vis_read_lo(sd67))

/* *********************************************************** */

mlib_status
__mlib_VideoColorSplit4(
	mlib_u8 *color1,
	mlib_u8 *color2,
	mlib_u8 *color3,
	mlib_u8 *color4,
	const mlib_u8 *colors,
	mlib_s32 n)
{
	mlib_d64 *sp = (mlib_d64 *)colors;
	mlib_d64 *dp0 = (mlib_d64 *)color1;
	mlib_d64 *dp1 = (mlib_d64 *)color2;
	mlib_d64 *dp2 = (mlib_d64 *)color3;
	mlib_d64 *dp3 = (mlib_d64 *)color4;
	mlib_d64 sd01, sd23, sd45, sd67, dd0, dd1, dd2, dd3;
	mlib_d64 sd04, sd26, sd15, sd37, dh0, dh1, dl0, dl1;
	mlib_s32 i;

	MLIB_LOAD_PREP_U8(sp);
/*
 * 8-pixels loop
 */
#pragma pipeloop(0)
	for (i = 0; i < (n / 8); i++) {
		MLIB_LOAD_SPLIT4_U8(sp, dd0, dd1, dd2, dd3);
		(*dp0++) = dd0;
		(*dp1++) = dd1;
		(*dp2++) = dd2;
		(*dp3++) = dd3;
	}

/*
 * last 8 pixels
 */

	if (n & 7) {
		mlib_s32 emask = (0xFF00 >> (n & 7)) & 0xFF;

		MLIB_LOAD_SPLIT4_U8(sp, dd0, dd1, dd2, dd3);
		vis_pst_8(dd0, dp0, emask);
		vis_pst_8(dd1, dp1, emask);
		vis_pst_8(dd2, dp2, emask);
		vis_pst_8(dd3, dp3, emask);
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
