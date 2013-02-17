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

#pragma ident	"@(#)mlib_ImageFlipY_Bit.c	9.2	07/10/09 SMI"

#include <mlib_image.h>
#include <mlib_ImageFlipY.h>
#include <mlib_SignalFFTUtil.h>
#include <stdio.h>

/* *********************************************************** */

#define	TABLE	mlib_fftK_Rev8

/* *********************************************************** */

void
mlib_ImageFlipY_BIT_1(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 i, j;
	mlib_u32 pix_sp, pix_dp;
	mlib_s32 dp_offset_start = 0, dp_offset_end, sp_offset_orig;
	mlib_s32 dp_mask_end;
	mlib_s32 dp_mask_start;
	mlib_s32 src_stride = mlib_ImageGetStride(src);
	mlib_s32 dst_stride = mlib_ImageGetStride(dst);
	mlib_s32 ws = mlib_ImageGetWidth(src);
	mlib_s32 hs = mlib_ImageGetHeight(src);
	mlib_s32 wd = mlib_ImageGetWidth(dst);
	mlib_s32 hd = mlib_ImageGetHeight(dst);
	mlib_s32 sp_offset = mlib_ImageGetBitOffset(src);
	mlib_s32 dp_offset = mlib_ImageGetBitOffset(dst);
	mlib_s32 sw = (ws < wd) ? ws : wd;
	mlib_s32 sh = (hs < hd) ? hs : hd;
	mlib_s32 dx = ((mlib_s32)ws - (mlib_s32)wd);
	mlib_s32 dy = ((mlib_s32)hs - (mlib_s32)hd);

/* for Rotate180 */
	mlib_s32 Q = (src_stride >= 0) ? 1 : 0;
	mlib_s32 dxs = ((dx > 0) ? (dx + 1) >> 1 : 0);
	mlib_s32 dxd = ((dx > 0) ? 0 : (1 - dx) >> 1);
	mlib_s32 dys = ((dy > 0) ? (dy + Q) >> 1 : 0);
	mlib_s32 dyd = ((dy > 0) ? 0 : (1 - Q - dy) >> 1);
	mlib_u8 *sa =
	    (mlib_u8 *)mlib_ImageGetData(src) + (dxs + sp_offset) / 8 +
	    src_stride * dys;
	mlib_u8 *da =
	    (mlib_u8 *)mlib_ImageGetData(dst) + (dxd + dp_offset) / 8 +
	    dst_stride * dyd;
	mlib_u8 *sp;
	mlib_u8 *dp;

	sp_offset = (dxs + sp_offset) & 7;
	dp_offset = (dxd + dp_offset) & 7;
	dp_offset_end = (dp_offset + sw) & 7;
	dp_mask_end = 0xFF << (8 - dp_offset_end);
	dp_mask_start = 0xFF << (8 - dp_offset);
	da += (dp_offset + sw) / 8;
	sp_offset_orig = sp_offset;

	if (dp_offset + sw < 8) {
		dp_mask_end ^= dp_mask_start;
		dp_offset_start = dp_offset;
	}

	for (j = 0; j < sh; j++) {
		sp = sa;
		dp = da - 1;

		if (sp_offset == dp_offset && dp_offset == 0 && !(sw & 7)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < sw / 8; i++)
				*dp-- = TABLE[(*sp++)];
		} else {
			if (dp_offset_end) {
				if ((dp_offset_end - dp_offset_start) <= (8 -
				    sp_offset))
					pix_sp = (sp[0] << sp_offset) & 0xFF;
				else
					pix_sp = ((sp[0] << sp_offset) | (sp[1]
					    >> (8 - sp_offset))) & 0xFF;
				pix_dp = TABLE[pix_sp] << (8 - dp_offset_end);
				dp[1] = (dp[1] & ~dp_mask_end) | (pix_dp &
				    dp_mask_end);
			}

			if (dp_offset + sw >= 8) {
				sp += (sp_offset + dp_offset_end) / 8;
				sp_offset = (dp_offset_end + sp_offset) & 7;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 1; i < (sw - dp_offset_end) / 8; i++) {
					pix_sp = ((sp[0] << sp_offset) | (sp[1]
					    >> (8 - sp_offset))) & 0xFF;
					*dp-- = TABLE[pix_sp];
					sp++;
				}

				if (i <= (sw - dp_offset_end) / 8) {
					if (sp_offset)
						pix_sp = ((sp[0] << sp_offset) |
						    (sp[1] >> (8 - sp_offset)))
						    & 0xFF;
					else
						pix_sp = sp[0];
					*dp-- = TABLE[pix_sp];
					sp++;
				}

				if (dp_offset) {
					if (dp_offset >= sp_offset)
						pix_sp = (sp[0] << sp_offset)
						    & 0xFF;
					else
						pix_sp = ((sp[0] << sp_offset) |
						    (sp[1] >> (8 - sp_offset)))
						    & 0xFF;
					pix_dp = TABLE[pix_sp] & ~dp_mask_start;
					pix_dp |= dp[0] & dp_mask_start;
					*dp = pix_dp;
				}
			}
		}

		sa += src_stride;
		da += dst_stride;
		sp_offset = sp_offset_orig;
	}
}

/* *********************************************************** */
