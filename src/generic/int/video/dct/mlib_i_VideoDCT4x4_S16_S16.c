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

#pragma ident	"@(#)mlib_i_VideoDCT4x4_S16_S16.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *    mlib_VideoDCT4x4_S16_S16 - Computes the forward DCT in the inter mode.
 *
 * SYNOPSIS
 *    mlib_status mlib_VideoDCT4x4_S16_S16(mlib_s16       *coeffs,
 *                                         const mlib_s16 *block)
 *
 * ARGUMENTS
 *    coeffs          Pointer to the output DCT coefficients
 *    block           Pointer to an 4x4 motion-compensated block which is the
 *                    difference between the reference block and current block
 *
 *    coeffs and  block both must be 8-byte aligned
 *
 *    Input block values should be in [-2048, 2047] range.
 *
 * DESCRIPTION
 *
 *   f = f(x, y)  x = 0, 1,2, 3; y = 0, 1,2, 3
 *   F = F(u, v)  u = 0, 1,2, 3; v = 0, 1,2, 3
 *
 *             1             3   3              pi*(2*x+1)*u      pi*(2*y+1)*v
 *   F(u, v) = ---*C(u)*C(v)*SUM SUM f(x,y)*cos(------------)*cos(------------)
 *             2            x = 0 y = 0              8                   8
 *
 *   C(u) = 1/sqrt(2) if u == 0, otherwise C(u) = 1
 *
 */

#include <mlib_video.h>

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

#ifdef _NO_LONGLONG
/* *********************************************************** */

static const mlib_f32 mlib_dct4ctab[3] = {
	1.000000000f,
	1.306562901f,
	0.541196108f,
};

/* *********************************************************** */

mlib_status
__mlib_VideoDCT4x4_S16_S16(
	mlib_s16 *coeff,
	const mlib_s16 *blk)
{
	mlib_f32 a00, a10, a20, a30;
	mlib_f32 a01, a11, a21, a31;
	mlib_f32 a02, a12, a22, a32;
	mlib_f32 a03, a13, a23, a33;

	mlib_f32 b00, b10, b20, b30;
	mlib_f32 b01, b11, b21, b31;
	mlib_f32 b02, b12, b22, b32;
	mlib_f32 b03, b13, b23, b33;

	mlib_f32 b = ((mlib_f32 *)mlib_dct4ctab)[1];
	mlib_f32 c = ((mlib_f32 *)mlib_dct4ctab)[2];

/* column 1D DCT */

	a00 = (mlib_s32)blk[0] + (mlib_s32)blk[3];
	a03 = (mlib_s32)blk[0] - (mlib_s32)blk[3];
	a01 = (mlib_s32)blk[1] + (mlib_s32)blk[2];
	a02 = (mlib_s32)blk[1] - (mlib_s32)blk[2];

	a10 = (mlib_s32)blk[4] + (mlib_s32)blk[7];
	a13 = (mlib_s32)blk[4] - (mlib_s32)blk[7];
	a11 = (mlib_s32)blk[5] + (mlib_s32)blk[6];
	a12 = (mlib_s32)blk[5] - (mlib_s32)blk[6];

	a20 = (mlib_s32)blk[8] + (mlib_s32)blk[11];
	a23 = (mlib_s32)blk[8] - (mlib_s32)blk[11];
	a21 = (mlib_s32)blk[9] + (mlib_s32)blk[10];
	a22 = (mlib_s32)blk[9] - (mlib_s32)blk[10];

	a30 = (mlib_s32)blk[12] + (mlib_s32)blk[15];
	a33 = (mlib_s32)blk[12] - (mlib_s32)blk[15];
	a31 = (mlib_s32)blk[13] + (mlib_s32)blk[14];
	a32 = (mlib_s32)blk[13] - (mlib_s32)blk[14];

	b00 = a00 + a01;
	b02 = a00 - a01;
	b01 = a03 * b + a02 * c;
	b03 = a03 * c - a02 * b;

	b10 = a10 + a11;
	b12 = a10 - a11;
	b11 = a13 * b + a12 * c;
	b13 = a13 * c - a12 * b;

	b20 = a20 + a21;
	b22 = a20 - a21;
	b21 = a23 * b + a22 * c;
	b23 = a23 * c - a22 * b;

	b30 = a30 + a31;
	b32 = a30 - a31;
	b31 = a33 * b + a32 * c;
	b33 = a33 * c - a32 * b;

	a00 = b00 + b30;
	a03 = b00 - b30;
	a01 = b10 + b20;
	a02 = b10 - b20;

	a20 = b02 + b32;
	a23 = b02 - b32;
	a21 = b12 + b22;
	a22 = b12 - b22;

	a10 = b01 + b31;
	a13 = b01 - b31;
	a11 = b11 + b21;
	a12 = b11 - b21;

	a30 = b03 + b33;
	a33 = b03 - b33;
	a31 = b13 + b23;
	a32 = b13 - b23;

	coeff[0] = (mlib_s32)(a00 + a01) >> 2;
	coeff[8] = (mlib_s32)(a00 - a01) >> 2;
	coeff[4] = (mlib_s32)(a03 * b + a02 * c) >> 2;
	coeff[12] = (mlib_s32)(a03 * c - a02 * b) >> 2;

	coeff[2] = (mlib_s32)(a20 + a21) >> 2;
	coeff[10] = (mlib_s32)(a20 - a21) >> 2;
	coeff[6] = (mlib_s32)(a23 * b + a22 * c) >> 2;
	coeff[14] = (mlib_s32)(a23 * c - a22 * b) >> 2;

	coeff[1] = (mlib_s32)(a10 + a11) >> 2;
	coeff[9] = (mlib_s32)(a10 - a11) >> 2;
	coeff[5] = (mlib_s32)(a13 * b + a12 * c) >> 2;
	coeff[13] = (mlib_s32)(a13 * c - a12 * b) >> 2;

	coeff[3] = (mlib_s32)(a30 + a31) >> 2;
	coeff[11] = (mlib_s32)(a30 - a31) >> 2;
	coeff[7] = (mlib_s32)(a33 * b + a32 * c) >> 2;
	coeff[15] = (mlib_s32)(a33 * c - a32 * b) >> 2;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
#else
/* *********************************************************** */

#define	ROUND(a)	((a + 2048) >> 12)

static const mlib_s32 mlib_dct4ctab[3] = {
	4096,
	5351,
	2217,
};

/* *********************************************************** */

mlib_status
__mlib_VideoDCT4x4_S16_S16(
	mlib_s16 *coeff,
	const mlib_s16 *blk)
{
	mlib_s32 a00, a10, a20, a30;
	mlib_s32 a01, a11, a21, a31;
	mlib_s32 a02, a12, a22, a32;
	mlib_s32 a03, a13, a23, a33;

	mlib_s32 b00, b10, b20, b30;
	mlib_s32 b01, b11, b21, b31;
	mlib_s32 b02, b12, b22, b32;
	mlib_s32 b03, b13, b23, b33;

	mlib_s32 b = ((mlib_s32 *)mlib_dct4ctab)[1];
	mlib_s32 c = ((mlib_s32 *)mlib_dct4ctab)[2];

/* column 1D DCT */

	a00 = (mlib_s32)blk[0] + (mlib_s32)blk[3];
	a03 = (mlib_s32)blk[0] - (mlib_s32)blk[3];
	a01 = (mlib_s32)blk[1] + (mlib_s32)blk[2];
	a02 = (mlib_s32)blk[1] - (mlib_s32)blk[2];

	a10 = (mlib_s32)blk[4] + (mlib_s32)blk[7];
	a13 = (mlib_s32)blk[4] - (mlib_s32)blk[7];
	a11 = (mlib_s32)blk[5] + (mlib_s32)blk[6];
	a12 = (mlib_s32)blk[5] - (mlib_s32)blk[6];

	a20 = (mlib_s32)blk[8] + (mlib_s32)blk[11];
	a23 = (mlib_s32)blk[8] - (mlib_s32)blk[11];
	a21 = (mlib_s32)blk[9] + (mlib_s32)blk[10];
	a22 = (mlib_s32)blk[9] - (mlib_s32)blk[10];

	a30 = (mlib_s32)blk[12] + (mlib_s32)blk[15];
	a33 = (mlib_s32)blk[12] - (mlib_s32)blk[15];
	a31 = (mlib_s32)blk[13] + (mlib_s32)blk[14];
	a32 = (mlib_s32)blk[13] - (mlib_s32)blk[14];

	b00 = a00 + a01;
	b02 = a00 - a01;
	b01 = ROUND(a03 * b + a02 * c);
	b03 = ROUND(a03 * c - a02 * b);

	b10 = a10 + a11;
	b12 = a10 - a11;
	b11 = ROUND(a13 * b + a12 * c);
	b13 = ROUND(a13 * c - a12 * b);

	b20 = a20 + a21;
	b22 = a20 - a21;
	b21 = ROUND(a23 * b + a22 * c);
	b23 = ROUND(a23 * c - a22 * b);

	b30 = a30 + a31;
	b32 = a30 - a31;
	b31 = ROUND(a33 * b + a32 * c);
	b33 = ROUND(a33 * c - a32 * b);

	a00 = b00 + b30;
	a03 = b00 - b30;
	a01 = b10 + b20;
	a02 = b10 - b20;

	a20 = b02 + b32;
	a23 = b02 - b32;
	a21 = b12 + b22;
	a22 = b12 - b22;

	a10 = b01 + b31;
	a13 = b01 - b31;
	a11 = b11 + b21;
	a12 = b11 - b21;

	a30 = b03 + b33;
	a33 = b03 - b33;
	a31 = b13 + b23;
	a32 = b13 - b23;

	coeff[0] = (mlib_s32)(a00 + a01) >> 2;
	coeff[8] = (mlib_s32)(a00 - a01) >> 2;
	coeff[4] = (mlib_s32)(ROUND(a03 * b + a02 * c)) >> 2;
	coeff[12] = (mlib_s32)(ROUND(a03 * c - a02 * b)) >> 2;

	coeff[2] = (mlib_s32)(a20 + a21) >> 2;
	coeff[10] = (mlib_s32)(a20 - a21) >> 2;
	coeff[6] = (mlib_s32)(ROUND(a23 * b + a22 * c)) >> 2;
	coeff[14] = (mlib_s32)(ROUND(a23 * c - a22 * b)) >> 2;

	coeff[1] = (mlib_s32)(a10 + a11) >> 2;
	coeff[9] = (mlib_s32)(a10 - a11) >> 2;
	coeff[5] = (mlib_s32)(ROUND(a13 * b + a12 * c)) >> 2;
	coeff[13] = (mlib_s32)(ROUND(a13 * c - a12 * b)) >> 2;

	coeff[3] = (mlib_s32)(a30 + a31) >> 2;
	coeff[11] = (mlib_s32)(a30 - a31) >> 2;
	coeff[7] = (mlib_s32)(ROUND(a33 * b + a32 * c)) >> 2;
	coeff[15] = (mlib_s32)(ROUND(a33 * c - a32 * b)) >> 2;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
#endif
