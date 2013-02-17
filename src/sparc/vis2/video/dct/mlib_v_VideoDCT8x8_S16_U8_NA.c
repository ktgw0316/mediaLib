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

#pragma ident	"@(#)mlib_v_VideoDCT8x8_S16_U8_NA.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VideoDCT8x8_S16_U8_NA - computes the forward DCT in the intra mode.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoDCT8x8_S16_U8_NA(mlib_s16      coeffs[64],
 *                                             const mlib_u8 * block,
 *                                             mlib_s32      stride);
 *
 *
 * ARGUMENTS
 *      coeffs     Pointer to output DCT coefficients.
 *      block      Pointer to block in current frame.
 *      stride     Stride in bytes between adjacent rows in a block.
 *
 *
 * DESCRIPTION
 *
 *   f = f(x, y)  x = 0, 1, ... ,7; y = 0, 1, ... ,7
 *   F = F(u, v)  u = 0, 1, ... ,7; v = 0, 1, ... ,7
 *
 *             1             7   7              pi*(2*x+1)*u      pi*(2*y+1)*v
 *   F(u, v) = ---*C(u)*C(v)*SUM SUM f(x,y)*cos(------------)*cos(------------)
 *             4            x = 0 y = 0            16                  16
 *
 *   C(u) = 1/sqrt(2) if u == 0, otherwise C(u) = 1
 *
 */

#include <mlib_video.h>
#include <vis_proto.h>
#include <mlib_v_VideoDCT.h>
#include <mlib_v_VideoDCT_VIS2.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoDCT8x8_S16_U8_NA = __mlib_VideoDCT8x8_S16_U8_NA

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoDCT8x8_S16_U8_NA) mlib_VideoDCT8x8_S16_U8_NA
	__attribute__((weak, alias("__mlib_VideoDCT8x8_S16_U8_NA")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoDCT8x8_S16_U8_NA(
	mlib_s16 coeffs[64],
	const mlib_u8 *block,
	mlib_s32 stride)
{
	mlib_u8 *sp = (void *)block;
	mlib_d64 d00, d10, d20, d30, d40, d50, d60, d70;
	mlib_d64 d01, d11, d21, d31, d41, d51, d61, d71;
	mlib_d64 t00, t10, t20, t30, t40, t50, t60, t70, t80, t90;
	mlib_d64 t01, t11, t21, t31, t41, t51, t61, t71, t81, t91;
	mlib_d64 r00, r10, r20, r30, r40, r50, r60, r70;
	mlib_d64 r01, r11, r21, r31, r41, r51, r61, r71;
	mlib_d64 *dp;

	mlib_s32 mask;
	mlib_f32 FCOS, FONE, c17, c26, c35;

	if (block == NULL || coeffs == NULL || stride <= 0)
		return (MLIB_FAILURE);

	if (!(((mlib_addr)block | (mlib_addr)coeffs | stride) & 7)) {
		return (__mlib_VideoDCT8x8_S16_U8(coeffs, block, stride));
	}

/*
 * first stage
 */

	vis_write_bmask(0x018923ab, 0x0);

	LOADCONSTS5;
	LOAD_DATA_GE_INTRA;

	TRANSPOSE_8X8(d00, d10, d20, d30, d40, d50, d60, d70,
		d01, d11, d21, d31, d41, d51, d61, d71);

	PREPARE_DATA_INTRA(hi, 0);
	COMPUTING_DATA(0);

	PREPARE_DATA_INTRA(lo, 1);
	TRANSPOSE_VIS2(d40, d50, d60, d70, r01, r11, r21, r31);
	TRANSPOSE_VIS2(d00, d10, d20, d30, r00, r10, r20, r30);
	COMPUTING_DATA(1);

	TRANSPOSE_VIS2(d01, d11, d21, d31, r40, r50, r60, r70);

/*
 * second stage
 */

	PREPARE_DATA_INTER(0);
	COMPUTING_DATA(0);

	ENDSCALE(0);
	TRANSPOSE_VIS2(d41, d51, d61, d71, r41, r51, r61, r71);

	dp = (mlib_d64 *)vis_alignaddr(coeffs, -1);
	mask = 0xFF >> ((mlib_addr)coeffs - (mlib_addr)dp);
	vis_alignaddrl((void *)coeffs, 0);

	PREPARE_DATA_INTER(1);
	COMPUTING_DATA(1);

	ENDSCALE(1);

	dp[1] = vis_faligndata(d00, d01);
	dp[2] = vis_faligndata(d01, d10);
	dp[3] = vis_faligndata(d10, d11);
	dp[4] = vis_faligndata(d11, d20);
	dp[5] = vis_faligndata(d20, d21);
	dp[6] = vis_faligndata(d21, d30);
	dp[7] = vis_faligndata(d30, d31);
	dp[8] = vis_faligndata(d31, d40);
	dp[9] = vis_faligndata(d40, d41);
	dp[10] = vis_faligndata(d41, d50);
	dp[11] = vis_faligndata(d50, d51);
	dp[12] = vis_faligndata(d51, d60);
	dp[13] = vis_faligndata(d60, d61);
	dp[14] = vis_faligndata(d61, d70);
	dp[15] = vis_faligndata(d70, d71);
	vis_pst_8(vis_faligndata(d71, d71), dp + 16, ~mask);
	if ((mlib_addr)coeffs & 7)
		vis_pst_8(vis_faligndata(d00, d00), dp, mask);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
