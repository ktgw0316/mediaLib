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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_i_VideoDCT8x8_S16_S16_B12.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *    mlib_VideoDCT8x8_S16_S16_B12 - Computes the forward DCT in the inter mode.
 *
 * SYNOPSIS
 *    mlib_s32 mlib_VideoDCT8x8_S16_S16_B12(mlib_s16       coeffs[64],
 *                                          const mlib_s16 *block)
 *
 * ARGUMENTS
 *    coeffs          Pointer to the output DCT coefficients
 *    block           Pointer to an 8x8 motion-compensated block which is the
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
	return (__mlib_VideoDCT8x8_S16_S16_B10(coeffs, block));
}

/* *********************************************************** */
