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

#pragma ident	"@(#)mlib_s_VideoDCT4x4_S16_S16.c	9.2	07/11/05 SMI"

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

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

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

mlib_status
__mlib_VideoDCT4x4_S16_S16(
	mlib_s16 *coeff,
	const mlib_s16 *blk)
{
	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm6, txmm7;
	__m128i *pSrc = (__m128i *)blk;

	mlib_s16 C0 = 0x4000;
	mlib_s16 C = 0x22A3;
	mlib_s16 B = 0x539F;

	__m128i *dataptr = (__m128i *)coeff;

	txmm0 = _mm_loadu_si128(pSrc++);
	txmm0 = _mm_slli_epi16(txmm0, 2);
	txmm1 = _mm_loadu_si128(pSrc++);
	txmm1 = _mm_slli_epi16(txmm1, 2);
	txmm2 = _mm_unpackhi_epi16(txmm0, txmm1);
	txmm3 = _mm_unpacklo_epi16(txmm0, txmm1);
	txmm4 = _mm_unpacklo_epi16(txmm3, txmm2);
	txmm5 = _mm_unpackhi_epi16(txmm3, txmm2);
	txmm5 = _mm_shuffle_epi32(txmm5, 0x4E);
	txmm0 = _mm_add_epi16(txmm4, txmm5);
	txmm1 = _mm_sub_epi16(txmm4, txmm5);
	txmm2 = _mm_unpackhi_epi16(txmm0, txmm1);
	txmm1 = _mm_unpacklo_epi16(txmm0, txmm1);
	txmm6 = _mm_set_epi16(B, C0, B, C0, B, C0, B, C0);
	txmm7 = _mm_set_epi16(C, C0, C, C0, C, C0, C, C0);
	txmm0 = _mm_mulhi_epi16(txmm1, txmm6);
	txmm3 = _mm_mulhi_epi16(txmm2, txmm7);
	txmm4 = _mm_add_epi16(txmm0, txmm3);
	txmm0 = _mm_mulhi_epi16(txmm1, txmm7);
	txmm3 = _mm_mulhi_epi16(txmm2, txmm6);
	txmm5 = _mm_sub_epi16(txmm0, txmm3);
	txmm4 = _mm_shufflehi_epi16(txmm4, 0x4E);
	txmm5 = _mm_shufflehi_epi16(txmm5, 0x4E);
	txmm0 = _mm_unpacklo_epi16(txmm4, txmm5);
	txmm1 = _mm_unpackhi_epi16(txmm4, txmm5);
	txmm2 = _mm_add_epi16(txmm0, txmm1);
	txmm3 = _mm_sub_epi16(txmm0, txmm1);
	txmm0 = _mm_unpacklo_epi16(txmm2, txmm3);
	txmm1 = _mm_unpackhi_epi16(txmm2, txmm3);
	txmm4 = _mm_mulhi_epi16(txmm0, txmm6);
	txmm5 = _mm_mulhi_epi16(txmm1, txmm7);
	txmm2 = _mm_add_epi16(txmm4, txmm5);
	txmm0 = _mm_mulhi_epi16(txmm0, txmm7);
	txmm1 = _mm_mulhi_epi16(txmm1, txmm6);
	txmm3 = _mm_sub_epi16(txmm0, txmm1);
	txmm2 = _mm_shufflehi_epi16(txmm2, 0xD8);
	txmm3 = _mm_shufflehi_epi16(txmm3, 0xD8);
	txmm2 = _mm_shufflelo_epi16(txmm2, 0xD8);
	txmm3 = _mm_shufflelo_epi16(txmm3, 0xD8);
	txmm2 = _mm_shuffle_epi32(txmm2, 0xD8);
	txmm3 = _mm_shuffle_epi32(txmm3, 0xD8);
	txmm2 = _mm_shufflehi_epi16(txmm2, 0xD8);
	txmm3 = _mm_shufflehi_epi16(txmm3, 0xD8);
	txmm2 = _mm_shufflelo_epi16(txmm2, 0xD8);
	_mm_storeu_si128(dataptr++, txmm2);
	txmm3 = _mm_shufflelo_epi16(txmm3, 0xD8);
	_mm_storeu_si128(dataptr++, txmm3);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
