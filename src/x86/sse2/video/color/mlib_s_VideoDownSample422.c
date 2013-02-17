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

#pragma ident	"@(#)mlib_s_VideoDownSample422.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoDownSample422 - 422 downsampling with bilinear filter
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoDownSample422(mlib_u8       *dst,
 *                                          const mlib_u8 *src,
 *                                          mlib_s32      n)
 *
 * ARGUMENTS
 *      dst       Pointer to destination row, 8-byte aligned
 *      src       Pointer to source row, 8-byte aligned
 *      n         Length of the src array
 *                Note: n must be even,
 *                      length of dst array must be n/2
 *
 * DESCRIPTION
 *      mlib_VideoDownSample422:
 *        dst[i] = (src[2*i] + src[2*i+1] + (i&1)) / 2, 0 <= i < n/2
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoDownSample422 = __mlib_VideoDownSample422

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoDownSample422) mlib_VideoDownSample422
	__attribute__((weak, alias("__mlib_VideoDownSample422")));

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
__mlib_VideoDownSample422(
	mlib_u8 *dst,
	const mlib_u8 *src,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_u32 *sp = (mlib_u32 *)src;
	mlib_u32 sa, data;
	__m128i txmm0, txmm1, txmm2, txmm3, txmm5, txmm6;
	__m128i txmm4;
	__m128i *p0 = (__m128i *)sp;
	__m128i *pDst = (__m128i *)dst;
	txmm4 = _mm_set_epi16(1, 0, 1, 0, 1, 0, 1, 0);
	if (!(((mlib_addr)src & 0xF) || ((mlib_addr)dst & 0xF))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= n - 32; i += 32) {
		txmm0 = _mm_load_si128(p0++);
		txmm2 = txmm0;
		txmm0 = _mm_srli_epi16(txmm0, 8);
		txmm2 = _mm_slli_epi16(txmm2, 8);
		txmm2 = _mm_srli_epi16(txmm2, 8);

		txmm0 = _mm_add_epi16(txmm0, txmm2);
		txmm1 = _mm_add_epi16(txmm0, txmm4);
		txmm1 = _mm_srli_epi16(txmm1, 1);

		txmm5 = _mm_load_si128(p0++);
		txmm2 = txmm5;
		txmm5 = _mm_srli_epi16(txmm5, 8);
		txmm2 = _mm_slli_epi16(txmm2, 8);
		txmm2 = _mm_srli_epi16(txmm2, 8);

		txmm2 = _mm_add_epi16(txmm5, txmm2);
		txmm2 = _mm_add_epi16(txmm2, txmm4);

		txmm2 = _mm_srli_epi16(txmm2, 1);
		txmm0 = _mm_packus_epi16(txmm1, txmm2);
		_mm_store_si128(pDst++, txmm0);

	}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= n - 32; i += 32) {
		txmm0 = _mm_loadu_si128(p0++);
		txmm2 = txmm0;
		txmm0 = _mm_srli_epi16(txmm0, 8);
		txmm2 = _mm_slli_epi16(txmm2, 8);
		txmm2 = _mm_srli_epi16(txmm2, 8);

		txmm0 = _mm_add_epi16(txmm0, txmm2);
		txmm1 = _mm_add_epi16(txmm0, txmm4);
		txmm1 = _mm_srli_epi16(txmm1, 1);

		txmm5 = _mm_loadu_si128(p0++);
		txmm2 = txmm5;
		txmm5 = _mm_srli_epi16(txmm5, 8);
		txmm2 = _mm_slli_epi16(txmm2, 8);
		txmm2 = _mm_srli_epi16(txmm2, 8);

		txmm2 = _mm_add_epi16(txmm5, txmm2);
		txmm2 = _mm_add_epi16(txmm2, txmm4);

		txmm2 = _mm_srli_epi16(txmm2, 1);
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
			(src[i] + src[i + 1]  +	((i>>1)&1)) >> 1;
		}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
