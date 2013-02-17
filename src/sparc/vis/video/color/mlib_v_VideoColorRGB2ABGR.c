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

#pragma ident	"@(#)mlib_v_VideoColorRGB2ABGR.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorRGB2ABGR - RGB to ABGR conversion
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorRGB2ABGR(mlib_u8       *abgr,
 *                                          const mlib_u8 *rgb,
 *                                          mlib_s32      n);
 * ARGUMENTS
 *      abgr      Pointer to ABGR multicomponent row, 8-byte aligned
 *      rgb       Pointer to  RGB multicomponent row, 8-byte aligned
 *      n         Number of pixels
 *
 *      abgr[4*i] - undefined
 *      abgr[4*i+1] = rgb[3*i+2]
 *      abgr[4*i+2] = rgb[3*i+1]
 *      abgr[4*i+3] = rgb[3*i]
 *
 *      0 <= i < n
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorRGB2ABGR = __mlib_VideoColorRGB2ABGR

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorRGB2ABGR) mlib_VideoColorRGB2ABGR
	__attribute__((weak, alias("__mlib_VideoColorRGB2ABGR")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	INSERT_U8_34R                                            \
	sda = vis_fpmerge(vis_read_hi(sd0), vis_read_lo(sd1));   \
	sdb = vis_fpmerge(vis_read_lo(sd0), vis_read_hi(sd2));   \
	sdc = vis_fpmerge(vis_read_hi(sd1), vis_read_lo(sd2));   \
	sdd = vis_fpmerge(vis_read_hi(sda), vis_read_lo(sdb));   \
	sde = vis_fpmerge(vis_read_lo(sda), vis_read_hi(sdc));   \
	sdf = vis_fpmerge(vis_read_hi(sdb), vis_read_lo(sdc));   \
	sdg = vis_fpmerge(vis_read_hi(sdd), vis_read_lo(sde));   \
	sdh = vis_fpmerge(vis_read_lo(sdd), vis_read_hi(sdf));   \
	sdi = vis_fpmerge(vis_read_hi(sde), vis_read_lo(sdf));   \
	sdj = vis_fpmerge(vis_read_hi(sdi), vis_read_hi(sdg));   \
	sdk = vis_fpmerge(vis_read_lo(sdi), vis_read_lo(sdg));   \
	sdl = vis_fpmerge(vis_fzeros(), vis_read_hi(sdh));       \
	sdm = vis_fpmerge(vis_fzeros(), vis_read_lo(sdh));       \
	dd0 = vis_fpmerge(vis_read_hi(sdl), vis_read_hi(sdj));   \
	dd1 = vis_fpmerge(vis_read_lo(sdl), vis_read_lo(sdj));   \
	dd2 = vis_fpmerge(vis_read_hi(sdm), vis_read_hi(sdk));   \
	dd3 = vis_fpmerge(vis_read_lo(sdm), vis_read_lo(sdk))

/* *********************************************************** */

#define	MERGE_S32(abgr0, abgr1, r0, g0, b0r1, g1b1)                      \
	abgr0 = r0 | (g0 << 8) | ((b0r1 & ~0xFF) << 8);                  \
	abgr1 = (b0r1 & 0xFF) | (g1b1 & ~0xFF) | ((g1b1 & 0xFF) << 16)

/* *********************************************************** */

#define	MERGE_S32_U8(x, a0, a1, a2)	x = (a0 << 16) | (a1 << 8) | (a2)

/* *********************************************************** */

mlib_status
__mlib_VideoColorRGB2ABGR(
	mlib_u8 *abgr,
	const mlib_u8 *rgb,
	mlib_s32 n)
{
/* 8-byte aligned start points in dst */
	mlib_d64 *dp = (mlib_d64 *)abgr;

/* 8-byte aligned start point in src */
	mlib_d64 *sp = (mlib_d64 *)rgb;

/* 8-byte source data */
	mlib_d64 sd0, sd1, sd2;

/* dst data */
	mlib_d64 dd0, dd1, dd2, dd3;

/* intermediate variables */
	mlib_d64 sda, sdb, sdc, sdd;
	mlib_d64 sde, sdf, sdg, sdh;
	mlib_d64 sdi, sdj, sdk, sdl, sdm;
	mlib_s32 i;

	if (n <= 0)
		return (MLIB_FAILURE);

#pragma pipeloop(0)
	for (i = 0; i <= (n - 16); i += 16) {
/* r0g0b0r1g1b1r2g2 */
		sd0 = sp[0];
/* b2r3g3b3r4g4b4r5 */
		sd1 = sp[1];
/* g5b5r6g6b6r7g7b7 */
		sd2 = sp[2];
		INSERT_U8_34R;
		MERGE_S32(((mlib_u32 *)dp)[8], ((mlib_u32 *)dp)[9],
			((mlib_u8 *)sp)[24], ((mlib_u8 *)sp)[25],
			((mlib_u16 *)sp)[13], ((mlib_u16 *)sp)[14]);
		MERGE_S32(((mlib_u32 *)dp)[10], ((mlib_u32 *)dp)[11],
			((mlib_u8 *)sp)[30], ((mlib_u8 *)sp)[31],
			((mlib_u16 *)sp)[16], ((mlib_u16 *)sp)[17]);
		MERGE_S32(((mlib_u32 *)dp)[12], ((mlib_u32 *)dp)[13],
			((mlib_u8 *)sp)[36], ((mlib_u8 *)sp)[37],
			((mlib_u16 *)sp)[19], ((mlib_u16 *)sp)[20]);
		MERGE_S32(((mlib_u32 *)dp)[14], ((mlib_u32 *)dp)[15],
			((mlib_u8 *)sp)[42], ((mlib_u8 *)sp)[43],
			((mlib_u16 *)sp)[22], ((mlib_u16 *)sp)[23]);
		dp[0] = dd0;
		dp[1] = dd1;
		dp[2] = dd2;
		dp[3] = dd3;
		sp += 6;
		dp += 8;
	}

	for (; i < n; i++) {
		MERGE_S32_U8(((mlib_u32 *)abgr)[i], rgb[3 * i + 2],
			rgb[3 * i + 1], rgb[3 * i]);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
