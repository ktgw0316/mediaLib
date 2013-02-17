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

#pragma ident	"@(#)mlib_v_VideoColorARGB2RGB.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorARGB2RGB - ARGB to RGB conversion
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorARGB2RGB(mlib_u8       *rgb,
 *                                          const mlib_u8 *argb,
 *                                          mlib_s32      n);
 *
 * ARGUMENTS
 *      rgb       Pointer to  RGB multicomponent row, 8-byte aligned
 *      argb      Pointer to ARGB multicomponent row, 8-byte aligned
 *      n         Number of pixels
 *
 * DESCRIPTION
 *
 *      rgb[3*i  ] = argb[4*i+1]
 *      rgb[3*i+1] = argb[4*i+2]
 *      rgb[3*i+2] = argb[4*i+3]
 *
 *      0 <= i < n
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorARGB2RGB = __mlib_VideoColorARGB2RGB

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorARGB2RGB) mlib_VideoColorARGB2RGB
	__attribute__((weak, alias("__mlib_VideoColorARGB2RGB")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_UNMERGE(dd, sf0, sf1)                              \
	dd = vis_fpmerge(sf0, sf1);                             \
	dd = vis_fpmerge(vis_read_hi(dd), vis_read_lo(dd))

/* *********************************************************** */

#define	EXTRACT_U8_43R                                           \
	sda = vis_fpmerge(vis_read_hi(sd0), vis_read_hi(sd2));   \
	sdb = vis_fpmerge(vis_read_lo(sd0), vis_read_lo(sd2));   \
	sdc = vis_fpmerge(vis_read_hi(sd1), vis_read_hi(sd3));   \
	sdd = vis_fpmerge(vis_read_lo(sd1), vis_read_lo(sd3));   \
	sde = vis_fpmerge(vis_read_hi(sda), vis_read_hi(sdc));   \
	sdf = vis_fpmerge(vis_read_lo(sda), vis_read_lo(sdc));   \
	sdg = vis_fpmerge(vis_read_hi(sdb), vis_read_hi(sdd));   \
	sdh = vis_fpmerge(vis_read_lo(sdb), vis_read_lo(sdd));   \
	MLIB_UNMERGE(sda, vis_read_lo(sde), vis_read_hi(sdf));   \
	MLIB_UNMERGE(sdb, vis_read_lo(sdf), vis_read_lo(sdg));   \
	MLIB_UNMERGE(sdc, vis_read_hi(sdh), vis_read_lo(sdh));   \
	MLIB_UNMERGE(dd0, vis_read_hi(sda), vis_read_hi(sdb));   \
	MLIB_UNMERGE(dd1, vis_read_hi(sdc), vis_read_lo(sda));   \
	MLIB_UNMERGE(dd2, vis_read_lo(sdb), vis_read_lo(sdc))

/* *********************************************************** */

#define	SPLIT_S32(rgbr, gbrg, brgb, argb0, argb1, argb2, argb3)   \
	rgbr = (argb0 << 8) | ((argb1 >> 16) & 0xFF);             \
	gbrg = (argb1 << 16) | ((argb2 >> 8) & 0xFFFF);           \
	brgb = (argb2 << 24) | (argb3 & 0xFFFFFF)

/* *********************************************************** */

#define	SPLIT_S32_3_U8(x, a1, a2, a3)                           \
	a1 = (x) >> 16;                                         \
	a2 = (x) >> 8;                                          \
	a3 = (x)

/* *********************************************************** */

mlib_status
__mlib_VideoColorARGB2RGB(
	mlib_u8 *rgb,
	const mlib_u8 *argb,
	mlib_s32 n)
{
/* 8-byte aligned start points in dst */
	mlib_f32 *dp = (mlib_f32 *)rgb;

/* 8-byte aligned start point in src */
	mlib_d64 *sp = (mlib_d64 *)argb;

/* 8-byte source data */
	mlib_d64 sd0, sd1, sd2, sd3;

/* intermediate variables */
	mlib_d64 sda, sdb, sdc, sdd;

/* intermediate variables */
	mlib_d64 sde, sdf, sdg, sdh;

/* dst data */
	mlib_d64 dd0, dd1, dd2;
	mlib_s32 i;

	if (n <= 0)
		return (MLIB_FAILURE);

#pragma pipeloop(1)
	for (i = 0; i <= (n - 16); i += 16) {
/* --r0g0b0--r1g1b1 */
		sd0 = sp[0];
/* --r2g2b2--r3g3b3 */
		sd1 = sp[1];
/* --r4g4b4--r5g5b5 */
		sd2 = sp[2];
/* --r6g6b6--r7g7b7 */
		sd3 = sp[3];
		EXTRACT_U8_43R;
		dp[0] = vis_read_hi(dd0);
		dp[3] = vis_read_lo(dd0);
		dp[1] = vis_read_hi(dd1);
		dp[4] = vis_read_lo(dd1);
		dp[2] = vis_read_hi(dd2);
		dp[5] = vis_read_lo(dd2);
		SPLIT_S32(((mlib_u32 *)dp)[6], ((mlib_u32 *)dp)[7],
			((mlib_u32 *)dp)[8], ((mlib_u32 *)sp)[8],
			((mlib_u32 *)sp)[9], ((mlib_u32 *)sp)[10],
			((mlib_u32 *)sp)[11]);
		SPLIT_S32(((mlib_u32 *)dp)[9], ((mlib_u32 *)dp)[10],
			((mlib_u32 *)dp)[11], ((mlib_u32 *)sp)[12],
			((mlib_u32 *)sp)[13], ((mlib_u32 *)sp)[14],
			((mlib_u32 *)sp)[15]);
		sp += 8;
		dp += 12;
	}

#pragma unroll(1)
	for (; i < n; i++) {
		SPLIT_S32_3_U8(((mlib_u32 *)argb)[i], rgb[3 * i],
			rgb[3 * i + 1], rgb[3 * i + 2]);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
