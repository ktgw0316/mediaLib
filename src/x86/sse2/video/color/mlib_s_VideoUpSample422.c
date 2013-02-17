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

#pragma ident	"@(#)mlib_s_VideoUpSample422.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoUpSample422_Nearest - 422 upsampling with nearest filter
 *      mlib_VideoUpSample422         - 422 upsampling with bilinear filter
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoUpSample422_Nearest(mlib_u8       *dst,
 *                                                const mlib_u8 *src,
 *                                                mlib_s32      n)
 *
 *      mlib_status mlib_VideoUpSample422(mlib_u8       *dst,
 *                                        const mlib_u8 *src,
 *                                        mlib_s32      n)
 *
 * ARGUMENTS
 *      dst       Pointer to destination row, 8-byte aligned
 *      src       Pointer to source row, 8-byte aligned
 *      n         Length of the src array
 *                Note: length of dst array must be 2*n
 *                      For mlib_VideoUpSample422: n >= 2
 *
 * DESCRIPTION
 *      mlib_VideoUpSample422_Nearest:
 *        dst[i]   = src[i/2],   0 <= i < 2*n
 *
 *      mlib_VideoUpSample422:
 *        dst[0]     = src[0]
 *        dst[1]     = (3*src[0] + src[1] + 2) / 4
 *        dst[2*i]   = (3*src[i] + src[i-1] + 1) / 4,  0 < i < n-1
 *        dst[2*i+1] = (3*src[i] + src[i+1] + 2) / 4,  0 < i < n-1
 *        dst[2*n-2] = (3*src[n-1] + src[n-2] + 1) / 4
 *        dst[2*n-1] = src[n-1]
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

#pragma weak mlib_VideoUpSample422 = __mlib_VideoUpSample422
#pragma weak mlib_VideoUpSample422_Nearest = __mlib_VideoUpSample422_Nearest

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoUpSample422) mlib_VideoUpSample422
	__attribute__((weak, alias("__mlib_VideoUpSample422")));
__typeof__(__mlib_VideoUpSample422_Nearest) mlib_VideoUpSample422_Nearest
	__attribute__((weak, alias("__mlib_VideoUpSample422_Nearest")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoUpSample422_Nearest(
	mlib_u8 *dst,
	const mlib_u8 *src,
	mlib_s32 n)
{
	mlib_s16 *sp = (mlib_s16 *)src;
	mlib_s32 *dp = (mlib_s32 *)dst;
	mlib_s32 sa, da, i;

	__m128i txmm0, txmm1, txmm2;
	mlib_s32 dw = n & 0xF;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n - dw; i += 16) {
		txmm0 = _mm_loadu_si128((__m128i *)sp);
		txmm1 = _mm_unpacklo_epi8(txmm0, txmm0);
		_mm_storeu_si128((__m128i *)dp, txmm1);
		txmm2 = _mm_unpackhi_epi8(txmm0, txmm0);
		_mm_storeu_si128((__m128i *)(dp + 4), txmm2);
		dp += 8;
		sp += 8;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n - 1; i += 2) {
		sa = *sp;
		da = ((sa & 0xFF00) << 8) + (sa & 0xFF);
		*dp = (da << 8) + da;
		sp++;
		dp++;
	}

	if ((n & 1) != 0) {
		sa = *((mlib_u8 *)sp);
		da = ((sa & 0xFF) << 8) + (sa & 0xFF);
		*((mlib_s16 *)dp) = (mlib_s16)da;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoUpSample422(
	mlib_u8 *dst,
	const mlib_u8 *src,
	mlib_s32 n)
{
	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm6;
	mlib_s32 dw = n & 0xF;
	mlib_s32 invalue;
	mlib_s32 i;
	mlib_u8 *sp = (void *)src;
	mlib_u8 *dp = dst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n - dw; i += 16) {
		txmm0 = _mm_loadu_si128((__m128i *)(sp - 1));
		txmm1 = _mm_loadu_si128((__m128i *)(sp));
		txmm2 = _mm_loadu_si128((__m128i *)(sp + 1));
		txmm3 = _mm_avg_epu8(txmm0, txmm1);
		txmm4 = _mm_avg_epu8(txmm2, txmm1);
		txmm3 = _mm_avg_epu8(txmm3, txmm1);
		txmm4 = _mm_avg_epu8(txmm4, txmm1);
		txmm5 = _mm_unpacklo_epi8(txmm3, txmm4);
		_mm_storeu_si128((__m128i *)dp, txmm5);
		txmm6 = _mm_unpackhi_epi8(txmm3, txmm4);
		_mm_storeu_si128((__m128i *)(dp + 16), txmm6);
		sp += 16;
		dp += 32;
	}

	for (; i < n - 1; i++) {
		invalue = sp[0] * 3;
		dp[0] = (invalue + sp[-1] + 1) >> 2;
		dp[1] = (invalue + sp[1] + 2) >> 2;
		sp++;
		dp += 2;
	}

	if (i == n) {
/* Special case for last column */
	invalue = sp[-1];
	dp[-2] = (invalue * 3 + sp[-2] + 1) >> 2;
	dp[-1] = invalue;
	} else {
/* Special case for last column */
	invalue = sp[0];
	dp[0] = (invalue * 3 + sp[-1] + 1) >> 2;
	dp[1] = invalue;
	}
/* Special case for first column */
	invalue = ((*src++));
	dst[0] = invalue;
	dst[1] = (invalue * 3 + src[0] + 2) >> 2;


	return (MLIB_SUCCESS);
}

/* *********************************************************** */
