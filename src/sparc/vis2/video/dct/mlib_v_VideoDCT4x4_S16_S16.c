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

#pragma ident	"@(#)mlib_v_VideoDCT4x4_S16_S16.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoDCT4x4_S16_S16 - Computes the forward DCT in the inter mode.
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoDCT4x4_S16_S16(mlib_s16 *coeffs,
 *                                           mlib_s16 *block)
 *
 * ARGUMENTS
 *      coeffs          Pointer to the output DCT coefficients
 *      block           Pointer to an 4x4 motion-compensated block which is the
 *                      difference between the reference block and current block
 *
 *    coeffs and  block both must be 8-byte aligned
 *    Input block values should be in [-2048, 2047] range.
 *
 * DESCRIPTION
 *
 *   f = f(x, y)  x = 0, 1,2, 3; y = 0, 1,2, 3
 *   F = F(u, v)  u = 0, 1,2, 3; v = 0, 1,2, 3
 *
 *             1             3   3              pi*(2*x+1)*u      pi*(2*y+1)*v
 *   F(u, v) = ---*C(u)*C(v)*SUM SUM f(x,y)*cos(------------)*cos(------------)
 *             2            x = 0 y = 0             8                   8
 *
 *   C(u) = 1/sqrt(2) if u == 0, otherwise C(u) = 1
 *
 */

#include <mlib_video.h>
#include <vis_proto.h>
#include <mlib_v_VideoDCT_VIS2.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoDCT4x4_S16_S16 = __mlib_VideoDCT4x4_S16_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoDCT4x4_S16_S16) mlib_VideoDCT4x4_S16_S16
	__attribute__((weak, alias("__mlib_VideoDCT4x4_S16_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static const mlib_u32 mlib_dct4vtab[3] = {
	0xecececec,
	0x62626262,
	0x40404040,
};

/* *********************************************************** */

mlib_status
__mlib_VideoDCT4x4_S16_S16(
	mlib_s16 *coeff,
	const mlib_s16 *blk)
{
	mlib_d64 a0, a1, a2, a3, b0, b1, b2, b3, c0, c1, c2, c3;

	mlib_f32 a = ((mlib_f32 *)mlib_dct4vtab)[0];
	mlib_f32 b = ((mlib_f32 *)mlib_dct4vtab)[1];

	mlib_f32 c = ((mlib_f32 *)mlib_dct4vtab)[2];

	mlib_d64 *src = (mlib_d64 *)blk;
	mlib_d64 *dst = (mlib_d64 *)coeff;

/* column 1D DCT */
	vis_write_bmask(0x018923ab, 0x0);

	a3 = vis_fpsub16(src[0], src[3]);
	a0 = vis_fpadd16(src[0], src[3]);
	a2 = vis_fpsub16(src[1], src[2]);
	a1 = vis_fpadd16(src[1], src[2]);

	c1 = vis_fpadd16(a3, a2);
	c3 = vis_fpsub16(a3, a2);
	b1 = vis_fpadd16(vis_fmul8x16(a, c1), vis_fmul8x16(b, c3));
	b3 = vis_fpsub16(vis_fmul8x16(a, c3), vis_fmul8x16(b, c1));
	b0 = vis_fpadd16(a0, a1);
	b2 = vis_fpsub16(a0, a1);

	TRANSPOSE_VIS2(b0, b1, b2, b3, c0, c1, c2, c3);

	a3 = vis_fpsub16(c0, c3);
	a0 = vis_fpadd16(c0, c3);
	a2 = vis_fpsub16(c1, c2);
	a1 = vis_fpadd16(c1, c2);

	c1 = vis_fpadd16(a3, a2);
	c3 = vis_fpsub16(a3, a2);
	b1 = vis_fpadd16(vis_fmul8x16(a, c1), vis_fmul8x16(b, c3));
	b3 = vis_fpsub16(vis_fmul8x16(a, c3), vis_fmul8x16(b, c1));
	b0 = vis_fpadd16(a0, a1);
	b2 = vis_fpsub16(a0, a1);

	b0 = vis_fmul8x16(c, b0);
	b2 = vis_fmul8x16(c, b2);
	b1 = vis_fmul8x16(c, b1);
	b3 = vis_fmul8x16(c, b3);

	TRANSPOSE_VIS2(b0, b1, b2, b3, dst[0], dst[1], dst[2], dst[3]);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
