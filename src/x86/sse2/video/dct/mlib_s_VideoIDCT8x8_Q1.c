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

#pragma ident	"@(#)mlib_s_VideoIDCT8x8_Q1.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *   mlib_VideoIDCT8x8_U8_S16_Q1 - computes the inverse DCT.
 *                                 The stride applies to the block which is
 *                                 the part of the current frame
 *                                 being reconstructed,
 *                                 in case of an intra block.
 *   mlib_VideoIDCT8x8_S16_S16_Q1 -computes the inverse DCT in the inter mode.
 *                                 The output of the IDCT routine which
 *                                 is really the difference between
 *                                 the current block and the
 *                                 reference block can occupy 9 bits, and is
 *                                 represented as a 16-bit data.
 *                                 This output needs to be added to
 *                                 the motion-compensated reference
 *                                 block to reconstruct the current block.
 *
 * SYNOPSIS
 *      mlib_status  mlib_VideoIDCT8x8_U8_S16_Q1(mlib_u8        *block,
 *                                               const mlib_s16 *coeffs,
 *                                               mlib_s32       stride)
 *
 *      mlib_status  mlib_VideoIDCT8x8_S16_S16_Q1(mlib_s16       *block,
 *                                                const mlib_s16 *coeffs)
 *
 * ARGUMENTS
 *      block       Pointer to 8x8 block in current frame.
 *      coeffs      Pointer to input DCT coefficients.
 *      stride      Stride in bytes between adjacent rows in a block.
 *
 *      It is assumed that block and coeffs are 8-byte aligned and stride is
 *      a multiple of 8.
 *      Input DCT coeffs should be in S12 range or it should be obtained
 *      from the correspondent direct DCT.
 *
 * DESCRIPTION
 *   function should be used when coeffs[u][v] (u, v = 0...7) are non zero
 *   only for u and v less then 4
 *
 *   F = F(u, v)  u = 0, 1, ... ,7; v = 0, 1, ... ,7
 *   f = f(x, y)  x = 0, 1, ... ,7; y = 0, 1, ... ,7
 *
 *             1   3   3                        pi*(2*x+1)*u      pi*(2*y+1)*v
 *   f(x, y) = ---*SUM SUM C(u)*C(v)*F(u,v)*cos(------------)*cos(------------)
 *             4  u = 0 v = 0                        16                  16
 *
 *   C(u) = 1/sqrt(2) if u == 0, otherwise C(u) = 1
 */

#include <mlib_video.h>
#include "mlib_s_video.h"

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoIDCT8x8_S16_S16_Q1 = __mlib_VideoIDCT8x8_S16_S16_Q1
#pragma weak mlib_VideoIDCT8x8_U8_S16_Q1 = __mlib_VideoIDCT8x8_U8_S16_Q1

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoIDCT8x8_S16_S16_Q1) mlib_VideoIDCT8x8_S16_S16_Q1
	__attribute__((weak, alias("__mlib_VideoIDCT8x8_S16_S16_Q1")));
__typeof__(__mlib_VideoIDCT8x8_U8_S16_Q1) mlib_VideoIDCT8x8_U8_S16_Q1
	__attribute__((weak, alias("__mlib_VideoIDCT8x8_U8_S16_Q1")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoIDCT8x8_U8_S16_Q1(
	mlib_u8 *block,
	const mlib_s16 *coeffs,
	mlib_s32 stride)
{
	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm6, txmm7, txmm8;

	__m128i tmp00, tmp10, tmp20, tmp30, tmp40, tmp50, tmp60, tmp70;
	__m128i tmp01, tmp11, tmp21, tmp31, tmp41, tmp51, tmp61, tmp71;
	__m128i xmmc0, xmmc1, xmmc2, xmmc3, xmmc4;

	__m128i t0, t1, t2, t3, t4, t5, t6, t7;

	__m128i xt0, xt1, xt2;

	__m128i *pSrc = (__m128i *)coeffs;
	mlib_s16 *p = (void *)coeffs;
	__m128i Czero = _mm_setzero_si128();
	mlib_u8 *dataptr = (void *)block;
	mlib_s16 tmp = coeffs[0];

	p[0] += (-128 * 8) + 4;

	LOADDATA_Q1;
	SSE2_TRANSPOSE4x4;

	LOAD_IDCT_ROW_C_Q1;
	IDCT_8x8_S16_S16_Q1;
/* This is for more performance improvement but low precision */

	SSE2_TRANSPOSE;
	ROW_IDCT_U8_Q1;
	p[0] = tmp;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoIDCT8x8_S16_S16_Q1(
	mlib_s16 *block,
	const mlib_s16 *coeffs)
{
	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm6, txmm7, txmm8;
	__m128i tmp00, tmp10, tmp20, tmp30, tmp40, tmp50, tmp60, tmp70;
	__m128i tmp01, tmp11, tmp21, tmp31, tmp41, tmp51, tmp61, tmp71;
	__m128i xmmc0, xmmc1, xmmc2, xmmc3, xmmc4;
	__m128i xt0, xt1, xt2;
	__m128i t0, t1, t2, t3, t4, t5, t6, t7;
	__m128i *pSrc = (__m128i *)coeffs;

	mlib_s16 *p = (void *)coeffs;
	__m128i Czero = _mm_setzero_si128();
	mlib_s16 *dataptr = (void *)block;
	mlib_s16 tmp = coeffs[0];

	p[0] += 16384 + 4;

	LOADDATA_Q1;

	SSE2_TRANSPOSE4x4;
	LOAD_IDCT_ROW_C_Q1;

	IDCT_8x8_S16_S16_Q1;
	SSE2_TRANSPOSE;

	ROW_IDCT_S16_Q1(00, 0);
	ROW_IDCT_S16_Q1(10, 1);
	ROW_IDCT_S16_Q1(20, 2);
	ROW_IDCT_S16_Q1(30, 3);
	ROW_IDCT_S16_Q1(40, 4);
	ROW_IDCT_S16_Q1(50, 5);
	ROW_IDCT_S16_Q1(60, 6);
	ROW_IDCT_S16_Q1(70, 7);

	p[0] = tmp;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
