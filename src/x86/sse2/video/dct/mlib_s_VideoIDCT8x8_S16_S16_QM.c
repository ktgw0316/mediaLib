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

#pragma ident	"@(#)mlib_s_VideoIDCT8x8_S16_S16_QM.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *
 *    mlib_VideoIDCT8x8_S16_S16_Q1_Mismatch - Computes the inverse IDCT in
 *                                            the inter mode
 *
 * SYNOPSIS
 *
 *    mlib_status mlib_VideoIDCT8x8_S16_S16_Q1_Mismatch(mlib_s16       *block,
 *                                                      const mlib_s16 *coeffs)
 *
 * ARGUMENT
 *      block           Pointer to an 8x8 motion-compensated block which is the
 *                      difference between the reference block and current block
 *      coeffs          Pointer to the input DCT coefficients
 *
 *      It is assumed that block and coeffs are 8-byte aligned
 *      Input DCT coeffs should be in S12 range or it should be obtained
 *      from the correspondent direct DCT.
 *
 * DESCRIPTION
 *   function is similar to mlib_v_VideoIDCT8x8_S16_S16_Q1: it should only
 *   be used when coeffs[u][v] (u, v = 0...7) are non zero
 *   only for u and v less then 4. However, this function
 *   also allows element coeff[7][7] to be non-zero.
 *   The primary benefit of this modification is that it can handle
 *   situations where coeffs[7][7] has been made non-zero
 *   by MPEG mismatch-control, allowing a simplified version of the IDCT
 *   to be undertaken for a much larger number of situations.
 */

#include <mlib_video.h>
#include <mlib_s_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoIDCT8x8_S16_S16_Q1_Mismatch = \
	__mlib_VideoIDCT8x8_S16_S16_Q1_Mismatch

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoIDCT8x8_S16_S16_Q1_Mismatch)
	mlib_VideoIDCT8x8_S16_S16_Q1_Mismatch
	__attribute__((weak,
	alias("__mlib_VideoIDCT8x8_S16_S16_Q1_Mismatch")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */


/* *********************************************************** */

mlib_status
__mlib_VideoIDCT8x8_S16_S16_Q1_Mismatch(
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
	mlib_s16 *p = (mlib_s16 *)coeffs + 63;
	mlib_s16 y = (*p) << 1;

	p = (mlib_s16 *)coeffs;
	__m128i Y, Cxmm;
	__m128i Czero;

	mlib_s16 tmp = coeffs[0];
	p[0] += 16384 + 4;
	Czero = _mm_setzero_si128();
	mlib_s16 *dataptr = (void *)block;

	LOADDATA_Q1;
	SSE2_TRANSPOSE4x4;

	LOAD_IDCT_ROW_C_Q1;

	IDCT_8x8_S16_S16_QM;
	LOAD_IDCT_ROW_C;
	SSE2_TRANSPOSE;

	ROW_IDCT_S16(00, 0);
	ROW_IDCT_S16(10, 1);
	ROW_IDCT_S16(20, 2);
	ROW_IDCT_S16(30, 3);
	ROW_IDCT_S16(40, 4);
	ROW_IDCT_S16(50, 5);
	ROW_IDCT_S16(60, 6);
	ROW_IDCT_S16(70, 7);

	p[0] = tmp;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
