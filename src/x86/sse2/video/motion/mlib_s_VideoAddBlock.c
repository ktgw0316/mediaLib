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

#pragma ident	"@(#)mlib_s_VideoAddBlock.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoAddBlock_U8_S16 - Performs additions of prediction and
 *                                  coefficient data
 *
 * SYNOPSIS
 *    mlib_status mlib_VideoAddBlock_U8_S16(mlib_u8        *curr_block,
 *                                          const mlib_s16 *mc_block,
 *                                          mlib_s32       stride)
 *
 * ARGUMENT
 *      curr_block  Pointer to  current block, must be 8-byte aligned
 *      mc_block    Pointer to 8x8 motion-compensated block(prediction data),
 *                  must be 8-byte aligned
 *      stride      Stride in bytes between adjacent rows in current block,
 *                  must be a multiple of 8
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

#pragma weak mlib_VideoAddBlock_U8_S16 = __mlib_VideoAddBlock_U8_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoAddBlock_U8_S16) mlib_VideoAddBlock_U8_S16
	__attribute__((weak, alias("__mlib_VideoAddBlock_U8_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */


/* *********************************************************** */

mlib_status
__mlib_VideoAddBlock_U8_S16(
	mlib_u8 *curr_block,
	const mlib_s16 *mc_block,
	mlib_s32 stride)
{
	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm7;
	mlib_s32 x, y;
	mlib_u8 *pDst = (void *)curr_block;
	__m128i *pSrc = (__m128i *)mc_block;
	txmm7 = _mm_setzero_si128();
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (y = 0; y < 8; y++) {
		txmm0 = _mm_loadu_si128(pSrc++);
		txmm1 = _mm_loadu_si128((__m128i *)pDst);
		txmm2 = _mm_unpacklo_epi8(txmm1, txmm7);
		txmm3 = _mm_add_epi16(txmm0, txmm2);
		txmm4 = _mm_packus_epi16(txmm3, txmm7);
		_mm_storel_epi64((__m128i *)pDst, txmm4);
		pDst += stride;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
