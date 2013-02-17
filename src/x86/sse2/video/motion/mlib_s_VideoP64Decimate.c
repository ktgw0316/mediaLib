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

#pragma ident	"@(#)mlib_s_VideoP64Decimate.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoP64Decimate_U8_U8 - Averages the input raster image
 *                                over 2x2 blocks and writes the results
 *                                to output raster image.
 *
 * SYNOPSIS
 *    mlib_status mlib_VideoP64Decimate_U8_U8(mlib_u8       *dst,
 *                                            const mlib_u8 *src,
 *                                            mlib_s32      width,
 *                                            mlib_s32      height,
 *                                            mlib_s32      dst_stride,
 *                                            mlib_s32      src_stride)
 *
 * ARGUMENT
 *      dst         Pointer to output raster image. Note that dst must be
 *                  8-byte aligned.
 *      src         Pointer to input raster image. Note that dst must be
 *                  8-byte aligned.
 *      width       Width of the input image
 *      height      Height of the input image
 *      dst_stride  Stride in bytes between adjacent rows in output
 *                  image. Note that dst_stride must be a multiple of 8.
 *      src_stride  Stride in bytes between adjacent rows in input
 *                  image. Note that src_stride must be a multiple of 8.
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

#pragma weak mlib_VideoP64Decimate_U8_U8 = __mlib_VideoP64Decimate_U8_U8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoP64Decimate_U8_U8) mlib_VideoP64Decimate_U8_U8
	__attribute__((weak, alias("__mlib_VideoP64Decimate_U8_U8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoP64Decimate_U8_U8(
	mlib_u8 *dst,
	const mlib_u8 *src,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 dst_stride,
	mlib_s32 src_stride)
{
	mlib_s32 x, y;
	const mlib_u8 *sd1, *sd2;
	mlib_u8 *dd;
	mlib_u32 src_stride2;

	sd1 = src;
	sd2 = src + src_stride;
	src_stride2 = 2 * src_stride;
	dd = dst;

	mlib_s32 dw = width & 0xF;
	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm6, txmm7;

	txmm7 = _mm_set1_epi16(0xff);
	for (y = 0; y < height; y++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (x = 0; x < width - dw; x += 16) {
			txmm0 = _mm_loadu_si128((__m128i *)&sd1[2*x]);
			txmm1 = _mm_loadu_si128((__m128i *)&sd2[2*x]);
			txmm2 =  _mm_srli_si128(txmm0, 1);
			txmm3 =  _mm_srli_si128(txmm1, 1);
			txmm4 = _mm_avg_epu8(txmm0, txmm2);
			txmm5 = _mm_avg_epu8(txmm1, txmm3);
			txmm6 = _mm_avg_epu8(txmm5, txmm4);
			txmm6 = _mm_and_si128(txmm6, txmm7);
			txmm0 = _mm_loadu_si128((__m128i *)&sd1[2 * x + 16]);
			txmm1 = _mm_loadu_si128((__m128i *)&sd2[2 * x + 16]);
			txmm2 =  _mm_srli_si128(txmm0, 1);
			txmm3 =  _mm_srli_si128(txmm1, 1);
			txmm4 = _mm_avg_epu8(txmm0, txmm2);
			txmm5 = _mm_avg_epu8(txmm1, txmm3);
			txmm5 = _mm_avg_epu8(txmm5, txmm4);
			txmm5 = _mm_and_si128(txmm5, txmm7);
			txmm1 = _mm_packus_epi16(txmm6, txmm5);
			_mm_storeu_si128((__m128i *)&dd[x], txmm1);

		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; x < width; x++) {
			dd[x] = (sd1[x * 2] + sd1[x * 2 + 1] + sd2[x * 2] +
				sd2[x * 2 + 1] + 2) >> 2;
		}

		sd1 += src_stride2;
		sd2 += src_stride2;
		dd += dst_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
