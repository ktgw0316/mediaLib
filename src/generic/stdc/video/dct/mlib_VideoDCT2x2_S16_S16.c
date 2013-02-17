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

#pragma ident	"@(#)mlib_VideoDCT2x2_S16_S16.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoDCT2x2_S16_S16 - Computes the forward DCT in the inter mode.
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoDCT2x2_S16_S16(mlib_s16       *coeffs,
 *                                           const mlib_s16 *block)
 *
 * ARGUMENTS
 *      coeffs          Pointer to the output DCT coefficients
 *      block           Pointer to an 2x2 motion-compensated block which is the
 *                      difference between the reference block and current block
 *
 *      Coeffs and  block both must be 8-byte aligned
 *      Input block values should not be out of S15 range.
 *
 * DESCRIPTION
 *
 *   f = f(x, y)  x = 0, 1; y = 0, 1
 *   F = F(u, v)  u = 0, 1; v = 0, 1
 *
 *                       1   1              pi*(2*x+1)*u        pi*(2*y+1)*v
 *   F(u, v) = C(u)*C(v)*SUM SUM f(x, y)*cos(--------------)*cos(--------------)
 *                      x = 0 y = 0                   4                   4
 *
 *   C(u) = 1/sqrt(2) if u == 0, otherwise C(u) = 1
 *
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoDCT2x2_S16_S16 = __mlib_VideoDCT2x2_S16_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoDCT2x2_S16_S16) mlib_VideoDCT2x2_S16_S16
	__attribute__((weak, alias("__mlib_VideoDCT2x2_S16_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoDCT2x2_S16_S16(
	mlib_s16 *coeff,
	const mlib_s16 *blk)
{
	mlib_s32 x0p1, x0m1, x2p3, x2m3;

	x0p1 = blk[0] + blk[1];
	x0m1 = blk[0] - blk[1];
	x2p3 = blk[2] + blk[3];
	x2m3 = blk[2] - blk[3];

	coeff[0] = (x0p1 + x2p3) >> 1;
	coeff[1] = (x0m1 + x2m3) >> 1;
	coeff[2] = (x0p1 - x2p3) >> 1;
	coeff[3] = (x0m1 - x2m3) >> 1;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
