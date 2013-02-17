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

#pragma ident	"@(#)mlib_s_VideoUpSample420.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoUpSample420_Nearest - 420 upsampling with nearest filter
 *      mlib_VideoUpSample420         - 420 upsampling with bilinear filter
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoUpSample420_Nearest(mlib_u8       *dst0,
 *                                                mlib_u8       *dst1,
 *                                                const mlib_u8 *src,
 *                                                mlib_s32      n)
 *
 *      mlib_status mlib_VideoUpSample420(mlib_u8       *dst0,
 *                                        mlib_u8       *dst1,
 *                                        const mlib_u8 *src0,
 *                                        const mlib_u8 *src1,
 *                                        const mlib_u8 *src2,
 *                                        mlib_s32      n)
 *
 * ARGUMENTS
 *      dst0      Pointer to destination upper row, 8-byte aligned
 *      dst1      Pointer to destination lower row, 8-byte aligned
 *      src       Pointer to source row, 8-byte aligned
 *      src0      Pointer to source upper row, 8-byte aligned
 *      src1      Pointer to source middle row, 8-byte aligned
 *      src2      Pointer to source lower row, 8-byte aligned
 *      n         Length of the source arrays
 *                Note: length of dst0 and dst1 arrays must be 2*n
 *                      For mlib_VideoUpSample420: n >= 2
 *
 * DESCRIPTION
 *      mlib_VideoUpSample420_Nearest:
 *        dst0[i]  = src[i/2],   0 <= i < 2*n
 *        dst1[i]  = src[i/2],   0 <= i < 2*n
 *
 *      mlib_VideoUpSample420:
 *        dst0[0]     = (4*(3*src1[0]   + src0[0]) + 8) /16
 *        dst0[1]     = (3*(3*src1[0]   + src0[0]) +
 *                         (3*src1[1]   + src0[1]) + 7) / 16
 *        dst0[2*i]   = (3*(3*src1[i]   + src0[i]) +
 *                         (3*src1[i-1] + src0[i-1]) + 8) / 16,  0 < i < n-1
 *        dst0[2*i+1] = (3*(3*src1[i]   + src0[i]) +
 *                         (3*src1[i+1] + src0[i+1]) + 7) / 16,  0 < i < n-1
 *        dst0[2*n-2] = (3*(3*src1[n-1] + src0[n-1]) +
 *                         (3*src1[n-2] + src0[n-2]) + 8) / 16
 *        dst0[2*n-1] = (4*(3*src1[n-1] + src0[n-1]) + 7) / 16
 *        dst1[0]     = (4*(3*src1[0]   + src2[0]) + 8) /16
 *        dst1[1]     = (3*(3*src1[0]   + src2[0]) +
 *                         (3*src1[1]   + src2[1]) + 7) / 16
 *        dst1[2*i]   = (3*(3*src1[i]   + src2[i]) +
 *                         (3*src1[i-1] + src2[i-1]) + 8) / 16,  0 < i < n-1
 *        dst1[2*i+1] = (3*(3*src1[i]   + src2[i]) +
 *                         (3*src1[i+1] + src2[i+1]) + 7) / 16,  0 < i < n-1
 *        dst1[2*n-2] = (3*(3*src1[n-1] + src2[n-1]) +
 *                         (3*src1[n-2] + src2[n-2]) + 8) / 16
 *        dst1[2*n-1] = (4*(3*src1[n-1] + src2[n-1]) + 7) / 16
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoUpSample420 = __mlib_VideoUpSample420
#pragma weak mlib_VideoUpSample420_Nearest = __mlib_VideoUpSample420_Nearest

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoUpSample420) mlib_VideoUpSample420
	__attribute__((weak, alias("__mlib_VideoUpSample420")));
__typeof__(__mlib_VideoUpSample420_Nearest) mlib_VideoUpSample420_Nearest
	__attribute__((weak, alias("__mlib_VideoUpSample420_Nearest")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

mlib_status
__mlib_VideoUpSample420_Nearest(
	mlib_u8 *dst0,
	mlib_u8 *dst1,
	const mlib_u8 *src,
	mlib_s32 n)
{
	mlib_s16 *sp = (mlib_s16 *)src;
	mlib_s32 *dp0 = (mlib_s32 *)dst0;
	mlib_s32 *dp1 = (mlib_s32 *)dst1;
	mlib_s32 sa, da, i;
	__m128i txmm0, txmm1, txmm2;
	mlib_s32 dw = n & 0xF;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n - dw; i += 16) {
		txmm0 = _mm_loadu_si128((__m128i *)sp);
		txmm1 = _mm_unpacklo_epi8(txmm0, txmm0);
		_mm_storeu_si128((__m128i *)dp0, txmm1);
		_mm_storeu_si128((__m128i *)dp1, txmm1);
		txmm2 = _mm_unpackhi_epi8(txmm0, txmm0);
		_mm_storeu_si128((__m128i *)(dp0+4), txmm2);
		_mm_storeu_si128((__m128i *)(dp1+4), txmm2);
		dp0 += 8;
		dp1 += 8;
		sp += 8;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n - 1; i += 2) {
		sa = *sp;
		da = ((sa & 0xFF00) << 8) + (sa & 0xFF);
		*dp0 = *dp1 = (da << 8) + da;
		sp++;
		dp0++;
		dp1++;
	}

	if ((n & 1) != 0) {
		sa = *((mlib_u8 *)sp);
		da = ((sa & 0xFF) << 8) + (sa & 0xFF);
		*((mlib_s16 *)dp0) = *((mlib_s16 *)dp1) = (mlib_s16)da;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoUpSample420(
	mlib_u8 *dst0,
	mlib_u8 *dst1,
	const mlib_u8 *src0,
	const mlib_u8 *src1,
	const mlib_u8 *src2,
	mlib_s32 n)
{
	mlib_s32 thiscolsum0, nextcolsum0, lastcolsum0, colctr;
	mlib_s32 thiscolsum1, nextcolsum1, lastcolsum1;

	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm6;
	__m128i rxmm0, rxmm1, rxmm2, rxmm3, rxmm4, rxmm5, rxmm6;
	__m128i ixmm0, ixmm1, ixmm2, ixmm3, ixmm4, ixmm5, ixmm6;
	mlib_s32 dw = n & 0xF;
	mlib_s32 i;
	mlib_u8 *sp0 = (void *)src0;
	mlib_u8 *sp1 = (void *)src1;
	mlib_u8 *sp2 = (void *)src2;
	mlib_u8 *dp0 = dst0;
	mlib_u8 *dp1 = dst1;

	for (i = 0; i < n - dw; i += 16) {
		txmm0 = _mm_loadu_si128((__m128i *)(sp0 - 1));
		txmm1 = _mm_loadu_si128((__m128i *)(sp0));
		txmm2 = _mm_loadu_si128((__m128i *)(sp0 + 1));
		rxmm0 = _mm_loadu_si128((__m128i *)(sp1 - 1));
		rxmm1 = _mm_loadu_si128((__m128i *)(sp1));
		rxmm2 = _mm_loadu_si128((__m128i *)(sp1 + 1));
		ixmm0 = _mm_loadu_si128((__m128i *)(sp2 - 1));
		ixmm1 = _mm_loadu_si128((__m128i *)(sp2));
		ixmm2 = _mm_loadu_si128((__m128i *)(sp2 + 1));

		rxmm3 = _mm_avg_epu8(rxmm0, rxmm1);
		txmm3 = _mm_avg_epu8(txmm0, txmm1);
		txmm4 = _mm_avg_epu8(rxmm0, txmm1);
		rxmm4 = _mm_avg_epu8(txmm3, rxmm3);
		txmm4 = _mm_avg_epu8(txmm4, rxmm4);
		txmm4 = _mm_avg_epu8(txmm4, rxmm1);

		rxmm5 = _mm_avg_epu8(rxmm2, rxmm1);
		txmm5 = _mm_avg_epu8(txmm2, txmm1);
		rxmm4 = _mm_avg_epu8(rxmm2, txmm1);
		rxmm6 = _mm_avg_epu8(txmm5, rxmm5);
		txmm6 = _mm_avg_epu8(rxmm6, rxmm4);
		txmm6 = _mm_avg_epu8(txmm6, rxmm1);
		txmm3 = _mm_unpacklo_epi8(txmm4, txmm6);
		_mm_storeu_si128((__m128i *)dp0, txmm3);
		txmm5 = _mm_unpackhi_epi8(txmm4, txmm6);
		_mm_storeu_si128((__m128i *)(dp0 + 16), txmm5);

		rxmm3 = _mm_avg_epu8(rxmm0, rxmm1);
		ixmm3 = _mm_avg_epu8(ixmm0, ixmm1);
		ixmm4 = _mm_avg_epu8(rxmm0, ixmm1);
		rxmm4 = _mm_avg_epu8(ixmm3, rxmm3);
		ixmm4 = _mm_avg_epu8(ixmm4, rxmm4);
		ixmm4 = _mm_avg_epu8(ixmm4, rxmm1);

		rxmm5 = _mm_avg_epu8(rxmm2, rxmm1);
		ixmm5 = _mm_avg_epu8(ixmm2, ixmm1);
		rxmm4 = _mm_avg_epu8(rxmm2, ixmm1);
		rxmm6 = _mm_avg_epu8(ixmm5, rxmm5);
		ixmm6 = _mm_avg_epu8(rxmm6, rxmm4);
		ixmm6 = _mm_avg_epu8(ixmm6, rxmm1);
		ixmm3 = _mm_unpacklo_epi8(ixmm4, ixmm6);
		_mm_storeu_si128((__m128i *)dp1, ixmm3);
		ixmm5 = _mm_unpackhi_epi8(ixmm4, ixmm6);
		_mm_storeu_si128((__m128i *)(dp1 + 16), ixmm5);

		sp0 += 16;
		sp1 += 16;
		sp2 += 16;
		dp0 += 32;
		dp1 += 32;
	}

	thiscolsum0 = sp1[0] * 3 + sp0[0];
	thiscolsum1 = sp1[0] * 3 + sp2[0];
	lastcolsum0 = sp1[-1] * 3 + sp0[-1];
	lastcolsum1 = sp1[-1] * 3 + sp2[-1];

	for (; i < n - 1; i++) {
/* General case: 3/4 * nearer pixel + 1/4 * further pixel in each */
/* dimension, thus 9/16, 3/16, 3/16, 1/16 overall */
		nextcolsum0 = sp1[1] * 3 + sp0[1];
		nextcolsum1 = sp1[1] * 3 + sp2[1];
		dp0[0] = ((thiscolsum0 * 3 + lastcolsum0 + 8) >> 4);
		dp0[1] = ((thiscolsum0 * 3 + nextcolsum0 + 7) >> 4);
		dp1[0] = ((thiscolsum1 * 3 + lastcolsum1 + 8) >> 4);
		dp1[1] = ((thiscolsum1 * 3 + nextcolsum1 + 7) >> 4);
		lastcolsum0 = thiscolsum0;
		thiscolsum0 = nextcolsum0;
		lastcolsum1 = thiscolsum1;
		thiscolsum1 = nextcolsum1;
		sp0++;
		sp1++;
		sp2++;
		dp0 += 2;
		dp1 += 2;
	}

	if (i == n) {
	thiscolsum0 = src1[n - 1] * 3 + src0[n - 1];
	lastcolsum0 = src1[n - 2] * 3 + src0[n - 2];
	thiscolsum1 = src1[n - 1] * 3 + src2[n - 1];
	lastcolsum1 = src1[n - 2] * 3 + src2[n - 2];
/* Special case for last column */
	dp0[-2] = ((thiscolsum0 * 3 + lastcolsum0 + 8) >> 4);
	dp0[-1] = ((thiscolsum0 * 4 + 7) >> 4);
	dp1[-2] = ((thiscolsum1 * 3 + lastcolsum1 + 8) >> 4);
	dp1[-1] = ((thiscolsum1 * 4 + 7) >> 4);
	} else {
	thiscolsum0 = src1[n - 1] * 3 + src0[n - 1];
	lastcolsum0 = src1[n - 2] * 3 + src0[n - 2];
	thiscolsum1 = src1[n - 1] * 3 + src2[n - 1];
	lastcolsum1 = src1[n - 2] * 3 + src2[n - 2];
/* Special case for last column */
	dp0[0] = ((thiscolsum0 * 3 + lastcolsum0 + 8) >> 4);
	dp0[1] = ((thiscolsum0 * 4 + 7) >> 4);
	dp1[0] = ((thiscolsum1 * 3 + lastcolsum1 + 8) >> 4);
	dp1[1] = ((thiscolsum1 * 4 + 7) >> 4);
	}

/* Special case for first column */
	lastcolsum0 = src1[0] * 3 + src0[0];
	lastcolsum1 = src1[0] * 3 + src2[0];
	thiscolsum0 = src1[1] * 3 + src0[1];
	thiscolsum1 = src1[1] * 3 + src2[1];
	dst0[0] = ((lastcolsum0 * 4 + 8) >> 4);
	dst0[1] = ((lastcolsum0 * 3 + thiscolsum0 + 7) >> 4);
	dst1[0] = ((lastcolsum1 * 4 + 8) >> 4);
	dst1[1] = ((lastcolsum1 * 3 + thiscolsum1 + 7) >> 4);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
