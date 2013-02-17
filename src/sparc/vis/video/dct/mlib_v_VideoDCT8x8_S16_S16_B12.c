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

#pragma ident	"@(#)mlib_v_VideoDCT8x8_S16_S16_B12.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoDCT8x8_S16_S16_B12 - Computes the forward DCT
 *                                     in the inter mode.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoDCT8x8_S16_S16_B12(mlib_s16       coeffs[64],
 *                                               const mlib_s16 *block);
 *
 * ARGUMENTS
 *      coeffs          Pointer to the output DCT coefficients
 *      block           Pointer to an 8x8 motion-compensated block which is the
 *                    difference between the reference block and current block
 *
 *    coeffs and  block both must be 8-byte aligned
 *
 *    Input block values should not be out of [-2048, 2047] range.
 *
 * DESCRIPTION
 *
 *   f = f(x, y)  x = 0, 1, ... ,7; y = 0, 1, ... ,7
 *   F = F(u, v)  u = 0, 1, ... ,7; v = 0, 1, ... ,7
 *
 *             1             7   7              pi*(2*x+1)*u      pi*(2*y+1)*v
 *   F(u, v) = ---*C(u)*C(v)*SUM SUM f(x,y)*cos(------------)*cos(------------)
 *             4            x = 0 y = 0             16                  16
 *
 *   C(u) = 1/sqrt(2) if u == 0, otherwise C(u) = 1
 *
 */

#include <mlib_video.h>
#include <vis_proto.h>
#include <mlib_v_VideoDCT.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoDCT8x8_S16_S16_B12 = __mlib_VideoDCT8x8_S16_S16_B12

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoDCT8x8_S16_S16_B12) mlib_VideoDCT8x8_S16_S16_B12
	__attribute__((weak, alias("__mlib_VideoDCT8x8_S16_S16_B12")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoDCT8x8_S16_S16_B12(
	mlib_s16 coeffs[64],
	const mlib_s16 *block)
{
	mlib_d64 *sp = (mlib_d64 *)block;
	mlib_d64 *dp = (mlib_d64 *)coeffs;

	mlib_d64 d00, d10, d20, d30, d40, d50, d60, d70;
	mlib_d64 d01, d11, d21, d31, d41, d51, d61, d71;
	mlib_d64 t00, t10, t20, t30, t40, t50, t60, t70, t80, t90;
	mlib_d64 t01, t11, t21, t31, t41, t51, t61, t71, t81, t91;
	mlib_d64 r00, r10, r20, r30, r40, r50, r60, r70;
	mlib_d64 r01, r11, r21, r31, r41, r51, r61, r71;
	mlib_f32 FCOS, c17, c26, c35, c_4;

/*
 * first stage
 */

	LOAD_DATA_AA_INTER1;
	TRANSPOSE(d00, d20, d40, d60, r00, r10, r20, r30);
	TRANSPOSE(d10, d30, d50, d70, r40, r50, r60, r70);
	LOADCONSTS4_12;

	PREPARE_DATA_INTER(0);

	LOAD_DATA_AA_INTER2;
	TRANSPOSE(d01, d21, d41, d61, r01, r11, r21, r31);

	COMPUTING_DATA(0);

	TRANSPOSE(d11, d31, d51, d71, r41, r51, r61, r71);
	PREPARE_DATA_INTER(1);
	COMPUTING_DATA(1);

/*
 * second stage
 */

	TRANSPOSE(d01, d11, d21, d31, r40, r50, r60, r70);
	TRANSPOSE(d00, d10, d20, d30, r00, r10, r20, r30);
	PREPARE_DATA_INTER(0);
	TRANSPOSE(d40, d50, d60, d70, r01, r11, r21, r31);
	COMPUTING_DATA_12(0);

	TRANSPOSE(d41, d51, d61, d71, r41, r51, r61, r71);
	ENDSCALE_12(0);

	dp[0] = d00;
	dp[2] = d10;
	dp[4] = d20;
	dp[6] = d30;
	dp[8] = d40;
	dp[10] = d50;
	dp[12] = d60;
	dp[14] = d70;

	PREPARE_DATA_INTER(1);
	COMPUTING_DATA_12(1);

	ENDSCALE_12(1);

	dp[1] = d01;
	dp[3] = d11;
	dp[5] = d21;
	dp[7] = d31;
	dp[9] = d41;
	dp[11] = d51;
	dp[13] = d61;
	dp[15] = d71;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
