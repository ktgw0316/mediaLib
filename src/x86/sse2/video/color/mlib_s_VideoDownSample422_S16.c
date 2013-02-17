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

#pragma ident	"@(#)mlib_s_VideoDownSample422_S16.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoDownSample422_S16 - 422 downsampling with bilinear filter
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoDownSample422_S16(mlib_s16       *dst,
 *                                              const mlib_s16 *src,
 *                                              mlib_s32       n)
 *
 * ARGUMENTS
 *      dst       Pointer to destination row, 8-byte aligned
 *      src       Pointer to source row, 8-byte aligned
 *      n         Length of the src array
 *                Note: n must be even,
 *                      length of dst array must be n/2
 *
 * DESCRIPTION
 *      mlib_VideoDownSample422_S16:
 *        dst[i] = (src[2*i] + src[2*i+1] + (i&1)) / 2, 0 <= i < n/2
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

#pragma weak mlib_VideoDownSample422_S16 = __mlib_VideoDownSample422_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoDownSample422_S16) mlib_VideoDownSample422_S16
	__attribute__((weak, alias("__mlib_VideoDownSample422_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoDownSample422_S16(
	mlib_s16 *dst,
	const mlib_s16 *src,
	mlib_s32 n)
{
	mlib_s32 i, sa, sa1, sa2;

	mlib_s16 *sp = (void *)src;
	mlib_s16 *dp = dst;

	mlib_s32 dw = n & 0xF;
	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm6, txmm7;
	txmm4 = _mm_set_epi32(1, 0, 1, 0);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n - dw; i += 16) {
		txmm0 = _mm_loadu_si128((__m128i *)sp);
		txmm7 = _mm_loadu_si128((__m128i *)(sp + 8));
		txmm1 = txmm0;
		txmm6 = txmm7;
		txmm0 = _mm_srai_epi32(txmm0, 16);
		txmm1 = _mm_slli_epi32(txmm1, 16);
		txmm1 = _mm_srai_epi32(txmm1, 16);
		txmm7 = _mm_srai_epi32(txmm7, 16);
		txmm6 = _mm_slli_epi32(txmm6, 16);
		txmm6 = _mm_srai_epi32(txmm6, 16);
		txmm0 = _mm_add_epi32(txmm0, txmm1);
		txmm7 = _mm_add_epi32(txmm6, txmm7);
		txmm2 = _mm_add_epi32(txmm0, txmm4);
		txmm5 = _mm_add_epi32(txmm7, txmm4);
		txmm3 = _mm_srai_epi32(txmm2, 1);
		txmm5 = _mm_srai_epi32(txmm5, 1);
		txmm6 = _mm_packs_epi32(txmm3, txmm5);
		_mm_storeu_si128((__m128i *)dp, txmm6);
		sp += 16;
		dp += 8;
	}

	for (; i <= n - 4; i += 4) {
		sa1 = src[i];
		sa2 = src[i + 1];
		sa = *((mlib_s32 *)(src + i + 2));
		dp[0] = (sa1 + sa2) >> 1;
		dp[1] = (((sa << 16) >> 16) + (sa >> 16) + 1) >> 1;
		dp += 2;
	}

	if (i < n)
		dp[0] = (src[i] + src[i + 1]) >> 1;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
