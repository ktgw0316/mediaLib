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

#pragma ident	"@(#)mlib_s_VideoDownSample420.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoDownSample420 - 420 downsampling with bilinear filter
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoDownSample420(mlib_u8       *dst,
 *                                          const mlib_u8 *src0,
 *                                          const mlib_u8 *src1,
 *                                          mlib_s32      n)
 *
 * ARGUMENTS
 *      dst       Pointer to destination row, 8-byte aligned
 *      src0      Pointer to source upper row, 8-byte aligned
 *      src1      Pointer to source lower row, 8-byte aligned
 *      n         Length of the src0, src1 arrays
 *                Note: n must be even,
 *                      length of dst array must be n/2
 *
 * DESCRIPTION
 *      mlib_VideoDownSample420:
 *        dst[i] = (src0[2*i] + src0[2*i+1] +
 *                  src1[2*i] + src1[2*i+1] + (1 << (i&1))) / 4, 0 <= i < n/2
 */
#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoDownSample420 = __mlib_VideoDownSample420

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoDownSample420) mlib_VideoDownSample420
	__attribute__((weak, alias("__mlib_VideoDownSample420")));

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
__mlib_VideoDownSample420(
	mlib_u8 *dst,
	const mlib_u8 *src0,
	const mlib_u8 *src1,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_u32 *sp0 = (mlib_u32 *)src0;
	mlib_u32 *sp1 = (mlib_u32 *)src1;
	mlib_u32 sa0, sa1, data0, data1, data;
	__m128i txmm0, txmm1, txmm2, txmm3, txmm5, txmm6;
	__m128i txmm4;
	__m128i *p0 = (__m128i *)sp0;
	__m128i *p1 = (__m128i *)sp1;
	__m128i *pDst = (__m128i *)dst;
	txmm4 = _mm_set_epi16(2, 1, 2, 1, 2, 1, 2, 1);
	if (!(((mlib_addr) src0 & 0xF) || ((mlib_addr) src1 & 0xF) || \
				((mlib_addr) dst & 0xF))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= n - 32; i += 32) {
		txmm0 = _mm_load_si128(p0++);
		txmm1 = _mm_load_si128(p1++);
		txmm2 = txmm0;
		txmm3 = txmm1;
		txmm0 = _mm_srli_epi16(txmm0, 8);
		txmm2 = _mm_slli_epi16(txmm2, 8);
		txmm2 = _mm_srli_epi16(txmm2, 8);

		txmm1 = _mm_srli_epi16(txmm1, 8);
		txmm3 = _mm_slli_epi16(txmm3, 8);
		txmm3 = _mm_srli_epi16(txmm3, 8);

		txmm0 = _mm_add_epi16(txmm0, txmm2);
		txmm1 = _mm_add_epi16(txmm1, txmm3);
		txmm0 = _mm_add_epi16(txmm0, txmm1);
		txmm1 = _mm_add_epi16(txmm0, txmm4);
		txmm1 = _mm_srli_epi16(txmm1, 2);

		txmm5 = _mm_load_si128(p0++);
		txmm6 = _mm_load_si128(p1++);
		txmm2 = txmm5;
		txmm3 = txmm6;
		txmm5 = _mm_srli_epi16(txmm5, 8);
		txmm2 = _mm_slli_epi16(txmm2, 8);
		txmm2 = _mm_srli_epi16(txmm2, 8);

		txmm6 = _mm_srli_epi16(txmm6, 8);
		txmm3 = _mm_slli_epi16(txmm3, 8);
		txmm3 = _mm_srli_epi16(txmm3, 8);

		txmm2 = _mm_add_epi16(txmm5, txmm2);
		txmm3 = _mm_add_epi16(txmm6, txmm3);
		txmm2 = _mm_add_epi16(txmm2, txmm3);
		txmm2 = _mm_add_epi16(txmm2, txmm4);

		txmm2 = _mm_srli_epi16(txmm2, 2);
		txmm0 = _mm_packus_epi16(txmm1, txmm2);
		_mm_store_si128(pDst++, txmm0);

	}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= n - 32; i += 32) {
		txmm0 = _mm_loadu_si128(p0++);
		txmm1 = _mm_loadu_si128(p1++);
		txmm2 = txmm0;
		txmm3 = txmm1;
		txmm0 = _mm_srli_epi16(txmm0, 8);
		txmm2 = _mm_slli_epi16(txmm2, 8);
		txmm2 = _mm_srli_epi16(txmm2, 8);

		txmm1 = _mm_srli_epi16(txmm1, 8);
		txmm3 = _mm_slli_epi16(txmm3, 8);
		txmm3 = _mm_srli_epi16(txmm3, 8);

		txmm0 = _mm_add_epi16(txmm0, txmm2);
		txmm1 = _mm_add_epi16(txmm1, txmm3);
		txmm0 = _mm_add_epi16(txmm0, txmm1);
		txmm1 = _mm_add_epi16(txmm0, txmm4);
		txmm1 = _mm_srli_epi16(txmm1, 2);

		txmm5 = _mm_loadu_si128(p0++);
		txmm6 = _mm_loadu_si128(p1++);
		txmm2 = txmm5;
		txmm3 = txmm6;
		txmm5 = _mm_srli_epi16(txmm5, 8);
		txmm2 = _mm_slli_epi16(txmm2, 8);
		txmm2 = _mm_srli_epi16(txmm2, 8);

		txmm6 = _mm_srli_epi16(txmm6, 8);
		txmm3 = _mm_slli_epi16(txmm3, 8);
		txmm3 = _mm_srli_epi16(txmm3, 8);

		txmm2 = _mm_add_epi16(txmm5, txmm2);
		txmm3 = _mm_add_epi16(txmm6, txmm3);
		txmm2 = _mm_add_epi16(txmm2, txmm3);
		txmm2 = _mm_add_epi16(txmm2, txmm4);

		txmm2 = _mm_srli_epi16(txmm2, 2);
		txmm0 = _mm_packus_epi16(txmm1, txmm2);
		_mm_storeu_si128(pDst++, txmm0);

	}
	}

	dst = (mlib_u8 *)pDst;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < n; i += 2) {
		(*dst++) =
			(src0[i] + src0[i + 1] + src1[i] + src1[i + 1] +
			(1 << ((i>>1)&1))) >> 2;
		}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
