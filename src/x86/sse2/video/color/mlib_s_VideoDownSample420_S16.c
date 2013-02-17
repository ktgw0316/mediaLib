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

#pragma ident	"@(#)mlib_s_VideoDownSample420_S16.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoDownSample420_S16 - 420 downsampling with bilinear filter
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoDownSample420_S16(mlib_s16       *dst,
 *                                              const mlib_s16 *src0,
 *                                              const mlib_s16 *src1,
 *                                              mlib_s32       n)
 *
 * ARGUMENTS
 *      dst       Pointer to destination row, 8-byte aligned
 *      src0      Pointer to source upper row, 8-byte aligned
 *      src2      Pointer to source lower row, 8-byte aligned
 *      n         Length of the src0, src1 arrays
 *                Note: n must be even,
 *                      length of dst array must be n/2
 *
 * DESCRIPTION
 *      mlib_VideoDownSample420_S16:
 *        dst[i] = (src0[2*i] + src0[2*i+1] +
 *                  src1[2*i] + src1[2*i+1] + (1 << (i&1))) / 4, 0 <= i < n/2
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

#pragma weak mlib_VideoDownSample420_S16 = __mlib_VideoDownSample420_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoDownSample420_S16) mlib_VideoDownSample420_S16
	__attribute__((weak, alias("__mlib_VideoDownSample420_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoDownSample420_S16(
	mlib_s16 *dst,
	const mlib_s16 *src0,
	const mlib_s16 *src1,
	mlib_s32 n)
{
	mlib_s32 i, sa0, sa01, sa02, sa1, sa11, sa12;

	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm6, txmm7;
	mlib_s16 *sp0 = (void *)src0;
	mlib_s16 *sp1 = (void *)src1;
	mlib_s16 *dp = dst;

	mlib_s32 dw = n & 0xF;

	txmm4 = _mm_set_epi32(2, 1, 2, 1);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n - dw; i += 16) {
		txmm0 = _mm_loadu_si128((__m128i *)sp0);
		txmm1 = _mm_loadu_si128((__m128i *)sp1);
		txmm2 = txmm0;
		txmm3 = txmm1;
		txmm0 = _mm_srai_epi32(txmm0, 16);
		txmm2 = _mm_slli_epi32(txmm2, 16);
		txmm2 = _mm_srai_epi32(txmm2, 16);

		txmm1 = _mm_srai_epi32(txmm1, 16);
		txmm3 = _mm_slli_epi32(txmm3, 16);
		txmm3 = _mm_srai_epi32(txmm3, 16);

		txmm0 = _mm_add_epi32(txmm0, txmm2);
		txmm1 = _mm_add_epi32(txmm1, txmm3);
		txmm0 = _mm_add_epi32(txmm0, txmm1);
		txmm1 = _mm_add_epi32(txmm0, txmm4);
		txmm1 = _mm_srai_epi32(txmm1, 2);

		txmm5 = _mm_loadu_si128((__m128i *)(sp0 + 8));
		txmm6 = _mm_loadu_si128((__m128i *)(sp1 + 8));
		txmm2 = txmm5;
		txmm3 = txmm6;
		txmm5 = _mm_srai_epi32(txmm5, 16);
		txmm2 = _mm_slli_epi32(txmm2, 16);
		txmm2 = _mm_srai_epi32(txmm2, 16);

		txmm6 = _mm_srai_epi32(txmm6, 16);
		txmm3 = _mm_slli_epi32(txmm3, 16);
		txmm3 = _mm_srai_epi32(txmm3, 16);

		txmm2 = _mm_add_epi32(txmm5, txmm2);
		txmm3 = _mm_add_epi32(txmm6, txmm3);
		txmm2 = _mm_add_epi32(txmm2, txmm3);
		txmm2 = _mm_add_epi32(txmm2, txmm4);

		txmm2 = _mm_srai_epi32(txmm2, 2);

		txmm0 = _mm_packs_epi32(txmm1, txmm2);

		_mm_storeu_si128((__m128i *)dp, txmm0);
		sp0 += 16;
		sp1 += 16;
		dp += 8;
	}

	for (; i <= n - 4; i += 4) {
		sa0 = *((mlib_s32 *)(src0 + i));
		sa01 = src0[i + 2];
		sa02 = src0[i + 3];
		sa1 = *((mlib_s32 *)(src1 + i));
		sa11 = src1[i + 2];
		sa12 = src1[i + 3];
		dp[0] = (((sa0 << 16) >> 16) + (sa0 >> 16) + 1 +
			((sa1 << 16) >> 16) + (sa1 >> 16)) >> 2;
		dp[1] = (sa01 + sa02 + 2 + sa11 + sa12) >> 2;
		dp += 2;
	}

	if (i < n)
		dp[0] = (src0[i] + src0[i + 1] + 1 + src1[i] + src1[i +
			1]) >> 2;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
