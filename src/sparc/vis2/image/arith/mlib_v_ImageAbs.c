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

#pragma ident	"@(#)mlib_v_ImageAbs.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageAbs_S16 - absolute value of each 16-bit pixel
 *      mlib_ImageAbs_S32 - absolute value of each 32-bit pixel
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageAbs_S16(mlib_s16 *dst,
 *                                    mlib_s16 *src,
 *                                    mlib_s32 dlb,
 *                                    mlib_s32 slb,
 *                                    mlib_s32 wid,
 *                                    mlib_s32 hgt);
 *
 *      mlib_status mlib_ImageAbs_S32(mlib_s32 *dst,
 *                                    mlib_s32 *src,
 *                                    mlib_s32 dlb,
 *                                    mlib_s32 slb,
 *                                    mlib_s32 wid,
 *                                    mlib_s32 hgt);
 *
 * ARGUMENT
 *      dst     pointer to destination image
 *      src     pointer to source image
 *      dlb     dst image line bytes (bytes/line)
 *      slb     src image line bytes (bytes/line)
 *      wid     src image width in pixels
 *      hgt     src image height in pixels
 *
 * DESCRIPTION
 *      dst(i, j) = |src(i, j)|
 *      -- VIS2 versions.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageAbs.h>

/* *********************************************************** */

#define	READ_PXLS_UNALIGN                                       \
	curr0 = *(++sp);                                        \
	adat0 = vis_faligndata(prev, curr0);                    \
	prev = curr0

/* *********************************************************** */

#define	READ_PXLS_UNALIGN_S32                                   \
	prev = (*sp++);                                         \
	curr0 = *sp;                                            \
	adat0 = vis_faligndata(prev, curr0)

/* *********************************************************** */

#define	READ_PXLS_ALIGN	adat0 = (*sp++)

/* *********************************************************** */

#define	CALC_ABS_S16                                            \
	dabs = vis_fmul8ulx16(dtwo, vis_fand(mask, adat0));     \
	dabs = vis_fpsub16(vis_fxor(dabs, adat0), dabs)

/* *********************************************************** */

#define	CALC_ABS_S16_UNROLL(dabs, adat)                         \
	dabs = vis_fmul8ulx16(dtwo, vis_fand(mask, adat));      \
	dabs = vis_fpsub16(vis_fxor(dabs, adat), dabs)

/* *********************************************************** */

#define	CALC_ABS_S32                                                \
	dabs = vis_fnand(adat0, mask);                              \
	dabs =                                                      \
	vis_fpack16_pair(vis_fmul8x16au(ftwo, vis_read_hi(dabs)),   \
	    vis_fmul8x16au(ftwo, vis_read_lo(dabs)));               \
	dabs = vis_fpsub32(vis_fxor(dabs, adat0), dabs)

/* *********************************************************** */

#define	STORE_ABS_VALUES	(*dp++) = dabs

#define	ABS_VALUE(x)	(((x) ^ ((x) >> 31)) - ((x) >> 31))

/* *********************************************************** */

mlib_status
mlib_ImageAbs_S16(
    mlib_s16 *dst,
    mlib_s16 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt)
{
/* 8-byte aligned src, dst ptrs */
	mlib_d64 *sp, *dp;

/* unaligned data */
	mlib_d64 prev;
	mlib_d64 curr0;
	mlib_d64 curr1, curr2;

/* aligned data */
	mlib_d64 adat0;

/* absolute values of result */
	mlib_d64 dabs;

/* aligned data */
	mlib_d64 adat1, adat2, adat3;

/* absolute values of result */
	mlib_d64 dabs0, dabs1, dabs2, dabs3;
	mlib_d64 dtwo = vis_to_double_dup(0x20002);
	mlib_d64 mask = vis_to_double_dup(0x80008000);

/* last pixel of line */
	mlib_s16 *dlast;

/* bit mask results of comp */
	mlib_s32 mask0;

/* pxl count of source line */
	mlib_s32 slpxl = slb >> 1;

/* pxl count of destination line */
	mlib_s32 dlpxl = dlb >> 1;

/* dst offset for address alignment */
	mlib_s32 doffs;
	mlib_s32 row, block;

/* full blocks, each of N d64s */
	mlib_s32 numblocks;

	for (row = 0; row < hgt; row++) {

/* ROW SETUP */

/* last dst pixel in row */
		dlast = dst + wid - 1;
		doffs = (mlib_addr)dst & 7;
/* aligned dest ptr */
		dp = (mlib_d64 *)((mlib_addr)dst & ~7);
/* aligned src ptr */
		sp = (mlib_d64 *)vis_alignaddr(src, -(mlib_s32)doffs);
		prev = *sp;

/* FIRST d64 NEEDS EDGE MASK FOR DESTINATION START POINT */

/* edge mask for start point */
		mask0 = vis_edge16(dst, dlast);
		READ_PXLS_UNALIGN;
		CALC_ABS_S16;
		vis_pst_16(dabs, dp++, mask0);
		numblocks = ((mlib_u8 *)dlast + 1 - (mlib_u8 *)dp) >> 3;

/* DO MOST OF ROW IN BLOCKS OF N d64s */

		if ((((mlib_addr)src ^ (mlib_addr)dst) & 7) == 0) {
#pragma pipeloop(0)
			for (block = 0; block < numblocks - 3; block += 4) {
				adat0 = sp[0];
				adat1 = sp[1];
				adat2 = sp[2];

				CALC_ABS_S16_UNROLL(dabs0, adat0);
				adat3 = sp[3];
				dp[0] = dabs0;
				CALC_ABS_S16_UNROLL(dabs1, adat1);
				dp[1] = dabs1;
				CALC_ABS_S16_UNROLL(dabs2, adat2);
				dp[2] = dabs2;
				CALC_ABS_S16_UNROLL(dabs3, adat3);
				dp[3] = dabs3;

				sp += 4;
				dp += 4;
			}

#pragma pipeloop(0)
			for (; block < numblocks; block++) {
				READ_PXLS_ALIGN;
				CALC_ABS_S16;
				STORE_ABS_VALUES;
			}

			prev = *sp;
		} else {
#pragma pipeloop(0)
			for (block = 0; block < numblocks - 3; block += 4) {
				curr0 = *(sp + 1);
				curr1 = *(sp + 2);
				curr2 = *(sp + 3);

				adat0 = vis_faligndata(prev, curr0);
				prev = *(sp + 4);
				CALC_ABS_S16_UNROLL(dabs0, adat0);
				dp[0] = dabs0;
				adat1 = vis_faligndata(curr0, curr1);
				CALC_ABS_S16_UNROLL(dabs1, adat1);
				dp[1] = dabs1;
				adat2 = vis_faligndata(curr1, curr2);
				CALC_ABS_S16_UNROLL(dabs2, adat2);
				dp[2] = dabs2;
				adat3 = vis_faligndata(curr2, prev);
				CALC_ABS_S16_UNROLL(dabs3, adat3);
				dp[3] = dabs3;

				sp += 4;
				dp += 4;
			}

#pragma pipeloop(0)
			for (; block < numblocks; block++) {
				READ_PXLS_UNALIGN;
				CALC_ABS_S16;
				STORE_ABS_VALUES;
			}
		}

/* LAST d64 NEEDS EDGE MASK FOR DESTINATION END POINT */

		if ((mlib_addr)dp <= (mlib_addr)dlast) {
			curr0 = *(++sp);
/* edge mask for end point */
			mask0 = vis_edge16(dp, dlast);
			adat0 = vis_faligndata(prev, curr0);
			CALC_ABS_S16;
			vis_pst_16(dabs, dp, mask0);
		}

/* ptrs to next src row */
		src += slpxl;
/* ptrs to next dst row */
		dst += dlpxl;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageAbs_S32(
    mlib_s32 *dst,
    mlib_s32 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt)
{
/* 8-byte aligned src, dst ptrs */
	mlib_d64 *sp, *dp;

/* unaligned data */
	mlib_d64 prev;
	mlib_d64 curr0;

/* aligned data */
	mlib_d64 adat0;

/* absolute values of result */
	mlib_d64 dabs;
	mlib_f32 ftwo = vis_to_float(0x2020202);
	mlib_d64 mask = vis_to_double_dup(0x80008000);

/* pxl count of source line */
	mlib_s32 slpxl = slb >> 2;

/* pxl count of destination line */
	mlib_s32 dlpxl = dlb >> 2;
	mlib_s32 row, block;
	mlib_s32 pxl0, pxl1;
	mlib_s64 ll;

	vis_write_gsr(7 << 3);

	for (row = 0; row < hgt; row++) {

/* ROW SETUP */
		block = 0;

		if ((mlib_addr)dst & 7) {
			dst[0] = ABS_VALUE(src[0]);
			block = 1;
		}

/* aligned src ptr */
		sp = (mlib_d64 *)vis_alignaddr(src, 4 * block);
		dp = (mlib_d64 *)(dst + block);

/* DO MOST OF ROW IN BLOCKS OF N d64s */

		if ((((mlib_addr)src ^ (mlib_addr)dst) & 7) == 0) {
#pragma pipeloop(0)
			for (; block <= (wid - 4); block += 4) {
				READ_PXLS_ALIGN;
				ll = ((mlib_s64 *)sp)[0];
				pxl0 = (mlib_s32)(ll >> 32);
				pxl1 = (mlib_s32)ll;
				sp++;
				CALC_ABS_S32;
				STORE_ABS_VALUES;
				((mlib_s32 *)dp)[0] = (mlib_s32)ABS_VALUE(pxl0);
				((mlib_s32 *)dp)[1] = (mlib_s32)ABS_VALUE(pxl1);
				dp++;
			}
		} else {
#pragma pipeloop(0)
			for (; block <= (wid - 4); block += 4) {
				READ_PXLS_UNALIGN_S32;
				pxl0 = ((mlib_s32 *)sp)[1];
				pxl1 = ((mlib_s32 *)sp)[2];
				sp++;
				CALC_ABS_S32;
				STORE_ABS_VALUES;
				((mlib_s32 *)dp)[0] = (mlib_s32)ABS_VALUE(pxl0);
				((mlib_s32 *)dp)[1] = (mlib_s32)ABS_VALUE(pxl1);
				dp++;
			}
		}

		for (; block < wid; block++) {
			pxl0 = ((mlib_s32 *)src)[block];
			((mlib_s32 *)dst)[block] = (mlib_s32)ABS_VALUE(pxl0);
		}

/* ptrs to next src row */
		src += slpxl;
/* ptrs to next dst row */
		dst += dlpxl;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
