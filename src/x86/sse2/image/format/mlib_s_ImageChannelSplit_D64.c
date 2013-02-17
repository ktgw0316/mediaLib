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

#pragma ident	"@(#)mlib_s_ImageChannelSplit_D64.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageChannelSplit2_D64
 *      mlib_ImageChannelSplit3_D64
 *      mlib_ImageChannelSplit4_D64
 *
 * DESCRIPTION
 *      Internal functions.
 *
 */

#include <mlib_image.h>
#include <mlib_ImageChannelSplit.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

mlib_status
mlib_ImageChannelSplit2_D64(
    mlib_d64 *dst_d64_0,
    mlib_d64 *dst_d64_1,
    const mlib_d64 *src_d64_0,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst0_stride,
    mlib_s32 dst1_stride,
    mlib_s32 src0_stride)
{
	__m128i *x_sp, *x_dp0, *x_dp1;
	__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_sd4, x_sd5;
	__m128i x_dd0, x_dd1;
	mlib_s32 j, i, n;
	const __m128i x_zero = _mm_setzero_si128();

	if ((src0_stride == width * 16) &&
	    (dst0_stride == width * 8) &&
	    (dst1_stride == width * 8)) {
		width *= height;
		height = 1;
	}

	n = width;

	for (j = 0; j < height; j++) {

		i = 0;
		x_sp = (__m128i *)src_d64_0;
		x_dp0 = (__m128i *)dst_d64_0;
		x_dp1 = (__m128i *)dst_d64_1;

		if (0 == (0xf & ((mlib_addr)src_d64_0 |
		    (mlib_addr)dst_d64_0 |
		    (mlib_addr)dst_d64_1))) {
/*
 * Ideal case: both dst vectors and src vector are
 * aligned to 16 bytes
 */

/* 2 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < (n - 1); i += 2) {
				x_sd0 = _mm_load_si128(x_sp);
				x_sp++;
				x_sd1 = _mm_load_si128(x_sp);
				x_sp++;
				x_sd2 = _mm_unpacklo_epi64(x_sd0, x_sd1);
				x_sd3 = _mm_unpackhi_epi64(x_sd0, x_sd1);

				_mm_store_si128(x_dp0, x_sd2);
				_mm_store_si128(x_dp1, x_sd3);
				x_dp0++;
				x_dp1++;
			}

		} else {

/*
 * not both dst vectors and src vector are
 * aligned to 16 bytes
 */

/* 2 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < (n - 1); i += 2) {
				x_sd0 = _mm_loadu_si128(x_sp);
				x_sp++;
				x_sd1 = _mm_loadu_si128(x_sp);
				x_sp++;
				x_sd2 = _mm_unpacklo_epi64(x_sd0, x_sd1);
				x_sd3 = _mm_unpackhi_epi64(x_sd0, x_sd1);

				_mm_storeu_si128(x_dp0, x_sd2);
				_mm_storeu_si128(x_dp1, x_sd3);
				x_dp0++;
				x_dp1++;
			}
		}

/* 1 pixels */
		if (i < n) {
			x_sd0 = _mm_loadu_si128(x_sp);
			x_sd1 = _mm_unpacklo_epi64(x_sd0, x_zero);
			x_sd2 = _mm_unpackhi_epi64(x_sd0, x_zero);

			_mm_storel_epi64(x_dp0, x_sd1);
			_mm_storel_epi64(x_dp1, x_sd2);
			x_dp0 = (__m128i *)((mlib_s32 *)x_dp0 + 2);
			x_dp1 = (__m128i *)((mlib_s32 *)x_dp1 + 2);
		}

		dst_d64_0 = (mlib_d64 *)((mlib_u8 *)dst_d64_0 + dst0_stride);
		dst_d64_1 = (mlib_d64 *)((mlib_u8 *)dst_d64_1 + dst1_stride);
		src_d64_0 = (mlib_d64 *)((mlib_u8 *)src_d64_0 + src0_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

/* Pure C implementation */

mlib_status
mlib_ImageChannelSplit3_D64(
    mlib_d64 *dst_d64_0,
    mlib_d64 *dst_d64_1,
    mlib_d64 *dst_d64_2,
    const mlib_d64 *src_d64_0,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst0_stride,
    mlib_s32 dst1_stride,
    mlib_s32 dst2_stride,
    mlib_s32 src0_stride)
{
	mlib_s32 j, i;

	dst0_stride >>= 3;
	dst1_stride >>= 3;
	dst2_stride >>= 3;
	src0_stride >>= 3;

	for (j = 0; j < height; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			dst_d64_0[i] = src_d64_0[3 * i + 0];
			dst_d64_1[i] = src_d64_0[3 * i + 1];
			dst_d64_2[i] = src_d64_0[3 * i + 2];
		}

		dst_d64_0 += dst0_stride;
		dst_d64_1 += dst1_stride;
		dst_d64_2 += dst2_stride;
		src_d64_0 += src0_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

/* Pure C implementation */

mlib_status
mlib_ImageChannelSplit4_D64(
    mlib_d64 *dst_d64_0,
    mlib_d64 *dst_d64_1,
    mlib_d64 *dst_d64_2,
    mlib_d64 *dst_d64_3,
    const mlib_d64 *src_d64_0,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst0_stride,
    mlib_s32 dst1_stride,
    mlib_s32 dst2_stride,
    mlib_s32 dst3_stride,
    mlib_s32 src0_stride)
{
	mlib_s32 j, i;

	dst0_stride >>= 3;
	dst1_stride >>= 3;
	dst2_stride >>= 3;
	dst3_stride >>= 3;
	src0_stride >>= 3;

	for (j = 0; j < height; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			dst_d64_0[i] = src_d64_0[4 * i + 0];
			dst_d64_1[i] = src_d64_0[4 * i + 1];
			dst_d64_2[i] = src_d64_0[4 * i + 2];
			dst_d64_3[i] = src_d64_0[4 * i + 3];
		}

		dst_d64_0 += dst0_stride;
		dst_d64_1 += dst1_stride;
		dst_d64_2 += dst2_stride;
		dst_d64_3 += dst3_stride;
		src_d64_0 += src0_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
