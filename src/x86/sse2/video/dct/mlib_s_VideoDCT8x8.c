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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */
#pragma ident	"@(#)mlib_s_VideoDCT8x8.c	9.8	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VideoDCT8x8_S16_U8[_NA]  - computes the forward DCT in
 *                                      the intra mode.
 *      mlib_VideoDCT8x8_S16_S16_B10[_NA] - computes the forward DCT in
 *                                      the inter mode.
 *
 * SYNOPSIS
 *
 *      mlib_status  mlib_VideoDCT8x8_S16_U8[_NA] (mlib_s16      coeffs[64],
 *                                                 const mlib_u8 *block,
 *                                                 mlib_s32      stride)
 *
 *      mlib_status  mlib_VideoDCT8x8_S16_S16_B10[_NA](mlib_s16   coeffs[64],
 *                                                 const mlib_s16 *block)
 *
 *
 * ARGUMENTS
 *
 *      coeffs     Pointer to output DCT coefficients.
 *      block      Pointer to 8x8 block in current frame or
 *                 motion-compensated reference block.
 *      stride     Stride in bytes between adjacent rows in a block.
 *
 *      It is assumed that block and coeffs are 8-byte aligned and stride is
 *      a multiple of 8 for not *_NA functions.
 *
 *      Input block values should be in [-512, 511] range.
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
#include "mlib_s_video.h"

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoDCT8x8_S16_S16_B10 = __mlib_VideoDCT8x8_S16_S16_B10
#pragma weak mlib_VideoDCT8x8_S16_S16 = __mlib_VideoDCT8x8_S16_S16_B10
#pragma weak __mlib_VideoDCT8x8_S16_S16 = __mlib_VideoDCT8x8_S16_S16_B10
#pragma weak mlib_VideoDCT8x8_S16_S16_B10_NA = __mlib_VideoDCT8x8_S16_S16_B10_NA
#pragma weak mlib_VideoDCT8x8_S16_S16_NA = __mlib_VideoDCT8x8_S16_S16_B10_NA
#pragma weak __mlib_VideoDCT8x8_S16_S16_NA = __mlib_VideoDCT8x8_S16_S16_B10_NA
#pragma weak mlib_VideoDCT8x8_S16_U8 = __mlib_VideoDCT8x8_S16_U8
#pragma weak mlib_VideoDCT8x8_S16_U8_NA = __mlib_VideoDCT8x8_S16_U8_NA

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoDCT8x8_S16_S16_B10) mlib_VideoDCT8x8_S16_S16_B10
	__attribute__((weak, alias("__mlib_VideoDCT8x8_S16_S16_B10")));
__typeof__(__mlib_VideoDCT8x8_S16_S16_B10) mlib_VideoDCT8x8_S16_S16
	__attribute__((weak, alias("__mlib_VideoDCT8x8_S16_S16_B10")));
__typeof__(__mlib_VideoDCT8x8_S16_S16_B10) __mlib_VideoDCT8x8_S16_S16
	__attribute__((weak, alias("__mlib_VideoDCT8x8_S16_S16_B10")));
__typeof__(__mlib_VideoDCT8x8_S16_S16_B10_NA) mlib_VideoDCT8x8_S16_S16_B10_NA
	__attribute__((weak, alias("__mlib_VideoDCT8x8_S16_S16_B10_NA")));
__typeof__(__mlib_VideoDCT8x8_S16_S16_B10_NA) mlib_VideoDCT8x8_S16_S16_NA
	__attribute__((weak, alias("__mlib_VideoDCT8x8_S16_S16_B10_NA")));
__typeof__(__mlib_VideoDCT8x8_S16_S16_B10_NA) __mlib_VideoDCT8x8_S16_S16_NA
	__attribute__((weak, alias("__mlib_VideoDCT8x8_S16_S16_B10_NA")));
__typeof__(__mlib_VideoDCT8x8_S16_U8) mlib_VideoDCT8x8_S16_U8
	__attribute__((weak, alias("__mlib_VideoDCT8x8_S16_U8")));
__typeof__(__mlib_VideoDCT8x8_S16_U8_NA) mlib_VideoDCT8x8_S16_U8_NA
	__attribute__((weak, alias("__mlib_VideoDCT8x8_S16_U8_NA")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */



#define	MLIB_DCT_SIZE	8
#define	DCTSIZE	MLIB_DCT_SIZE

/* *********************************************************** */

mlib_status
__mlib_VideoDCT8x8_S16_U8(
	mlib_s16 coeffs[64],
	const mlib_u8 *block,
	mlib_s32 stride)
{
	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm6, txmm7, txmm8;
	__m128i tmp00, tmp10, tmp20, tmp30, tmp40, tmp50, tmp60, tmp70;
	__m128i tmp01, tmp11, tmp21, tmp31, tmp41, tmp51, tmp61, tmp71;
	__m128i xmmc0, xmmc1, xmmc2, xmmc3, xmmc4;
	__m128i t0, t1, t2, t3, t4, t5, t6, t7;

	mlib_u8 *pSrc = (void *)block;

	__m128i Czero = _mm_setzero_si128();

	mlib_s16 *dataptr = coeffs;

	LOAD_ROW_TALBE;

	ROW_DCT_U8(00);
	ROW_DCT_U8(10);
	ROW_DCT_U8(20);
	ROW_DCT_U8(30);
	ROW_DCT_U8(40);
	ROW_DCT_U8(50);
	ROW_DCT_U8(60);
	ROW_DCT_U8(70);

	COL_DCT_S16();

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoDCT8x8_S16_U8_NA(
	mlib_s16 coeffs[64],
	const mlib_u8 *block,
	mlib_s32 stride)
{

	if (block == NULL || coeffs == NULL || stride <= 0)
		return (MLIB_FAILURE);

	return (__mlib_VideoDCT8x8_S16_U8(coeffs, block, stride));
}

/* *********************************************************** */

mlib_status
__mlib_VideoDCT8x8_S16_S16_B10(
	mlib_s16 coeffs[64],
	const mlib_s16 *block)
{
	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm6, txmm7, txmm8;
	__m128i tmp00, tmp10, tmp20, tmp30, tmp40, tmp50, tmp60, tmp70;
	__m128i tmp01, tmp11, tmp21, tmp31, tmp41, tmp51, tmp61, tmp71;
	__m128i xmmc0, xmmc1, xmmc2, xmmc3, xmmc4;
	__m128i t0, t1, t2, t3, t4, t5, t6, t7;
	__m128i *pSrc = (__m128i *)block;

	__m128i Czero = _mm_setzero_si128();

	mlib_s16 *dataptr = coeffs;

	LOAD_ROW_TALBE;

	ROW_DCT_S16(00);
	ROW_DCT_S16(10);
	ROW_DCT_S16(20);
	ROW_DCT_S16(30);
	ROW_DCT_S16(40);
	ROW_DCT_S16(50);
	ROW_DCT_S16(60);
	ROW_DCT_S16(70);

	COL_DCT_S16();

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoDCT8x8_S16_S16_B10_NA(
	mlib_s16 coeffs[64],
	const mlib_s16 *block)
{
	if (block == NULL || coeffs == NULL)
		return (MLIB_FAILURE);

	return (__mlib_VideoDCT8x8_S16_S16_B10(coeffs, block));
}
/* *********************************************************** */
