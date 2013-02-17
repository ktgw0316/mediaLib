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

#pragma ident	"@(#)mlib_s_ImageChannelSplit_FS32.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageChannelSplit2_FS32
 *      mlib_ImageChannelSplit3_FS32
 *      mlib_ImageChannelSplit4_FS32
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
mlib_ImageChannelSplit2_FS32(
    mlib_s32 *dst_s32_0,
    mlib_s32 *dst_s32_1,
    const mlib_s32 *src_s32_0,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst0_str,
    mlib_s32 dst1_str,
    mlib_s32 src0_str)
{
	__m128i *x_sp, *x_dp0, *x_dp1;
	__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_sd4, x_sd5;
	__m128i x_dd0, x_dd1;
	mlib_s32 j, i, n;
	const __m128i x_zero = _mm_setzero_si128();

	if ((src0_str == width * 8) &&
	    (dst0_str == width * 4) &&
	    (dst1_str == width * 4)) {
		width *= height;
		height = 1;
	}

	n = width;

	for (j = 0; j < height; j++) {

		i = 0;
		x_sp = (__m128i *)src_s32_0;
		x_dp0 = (__m128i *)dst_s32_0;
		x_dp1 = (__m128i *)dst_s32_1;

		if (0 == (0xf & ((mlib_addr)src_s32_0 |
		    (mlib_addr)dst_s32_0 |
		    (mlib_addr)dst_s32_1))) {
/*
 * Ideal case: both dst vectors and src vector are
 * aligned to 16 bytes
 */

/* 4 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < (n - 3); i += 4) {
				x_sd0 = _mm_load_si128(x_sp);
				x_sp++;
				x_sd1 = _mm_load_si128(x_sp);
				x_sp++;
				x_sd2 = _mm_unpacklo_epi32(x_sd0, x_sd1);
				x_sd3 = _mm_unpackhi_epi32(x_sd0, x_sd1);

				x_sd4 = _mm_unpacklo_epi32(x_sd2, x_sd3);
				x_sd5 = _mm_unpackhi_epi32(x_sd2, x_sd3);

				_mm_store_si128(x_dp0, x_sd4);
				_mm_store_si128(x_dp1, x_sd5);
				x_dp0++;
				x_dp1++;
			}

		} else {

/*
 * not both dst vectors and src vector are
 * aligned to 16 bytes
 */

/* 4 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < (n - 3); i += 4) {
				x_sd0 = _mm_loadu_si128(x_sp);
				x_sp++;
				x_sd1 = _mm_loadu_si128(x_sp);
				x_sp++;
				x_sd2 = _mm_unpacklo_epi32(x_sd0, x_sd1);
				x_sd3 = _mm_unpackhi_epi32(x_sd0, x_sd1);

				x_sd4 = _mm_unpacklo_epi32(x_sd2, x_sd3);
				x_sd5 = _mm_unpackhi_epi32(x_sd2, x_sd3);

				_mm_storeu_si128(x_dp0, x_sd4);
				_mm_storeu_si128(x_dp1, x_sd5);
				x_dp0++;
				x_dp1++;
			}
		}

/* 2 pixels */
		if (i < (n - 1)) {
			x_sd0 = _mm_loadu_si128(x_sp);
			x_sp++;
			x_sd1 = _mm_unpackhi_epi64(x_sd0, x_zero);
			x_sd2 = _mm_unpacklo_epi32(x_sd0, x_sd1);
			x_sd3 = _mm_unpackhi_epi64(x_sd2, x_zero);

			_mm_storel_epi64(x_dp0, x_sd2);
			_mm_storel_epi64(x_dp1, x_sd3);
			x_dp0 = (__m128i *)((mlib_s32 *)x_dp0 + 2);
			x_dp1 = (__m128i *)((mlib_s32 *)x_dp1 + 2);
			i += 2;
		}

/* pure C implementation */
		mlib_s32 *sp = (mlib_s32 *)x_sp;
		mlib_s32 *dp0 = (mlib_s32 *)x_dp0;
		mlib_s32 *dp1 = (mlib_s32 *)x_dp1;

/* 1 pixels */
		if (i < n) {
			(*dp0) = *(sp++);
			(*dp1) = *(sp++);
			dp0++;
			dp1++;
		}

		dst_s32_0 = (mlib_s32 *)((mlib_u8 *)dst_s32_0 + dst0_str);
		dst_s32_1 = (mlib_s32 *)((mlib_u8 *)dst_s32_1 + dst1_str);
		src_s32_0 = (mlib_s32 *)((mlib_u8 *)src_s32_0 + src0_str);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MLIB_S_Separate48_S32						\
{									\
	x_t0 = _mm_unpacklo_epi32(x_sd0, _mm_srli_si128(x_sd1, 8));	\
	x_t1 = _mm_unpacklo_epi32(_mm_srli_si128(x_sd0, 8), x_sd2);	\
	x_t2 = _mm_unpacklo_epi32(x_sd1, _mm_srli_si128(x_sd2, 8));	\
									\
	x_dd0 = _mm_unpacklo_epi32(x_t0, _mm_srli_si128(x_t1, 8));	\
	x_dd1 = _mm_unpacklo_epi32(_mm_srli_si128(x_t0, 8), x_t2);	\
	x_dd2 = _mm_unpacklo_epi32(x_t1, _mm_srli_si128(x_t2, 8));	\
}

#define	MLIB_S_Separate24_S32						\
{									\
	x_t0 = _mm_unpacklo_epi32(x_sd0, x_zero);			\
	x_t1 = _mm_unpacklo_epi32(_mm_srli_si128(x_sd0, 8), x_zero);	\
	x_t2 = _mm_unpacklo_epi32(x_sd1, x_zero);			\
									\
	x_dd0 = _mm_unpacklo_epi32(x_t0, _mm_srli_si128(x_t1, 8));	\
	x_dd1 = _mm_unpacklo_epi32(_mm_srli_si128(x_t0, 8), x_t2);	\
	x_dd2 = _mm_unpacklo_epi32(x_t1, _mm_srli_si128(x_t2, 8));	\
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelSplit3_FS32(
    mlib_s32 *dst_s32_0,
    mlib_s32 *dst_s32_1,
    mlib_s32 *dst_s32_2,
    const mlib_s32 *src_s32_0,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst0_str,
    mlib_s32 dst1_str,
    mlib_s32 dst2_str,
    mlib_s32 src0_str)
{
	__m128i x_sd0, x_sd1, x_sd2;
	__m128i x_t0, x_t1, x_t2, x_t3, x_t4, x_t5;
	__m128i x_dd0, x_dd1, x_dd2;
	mlib_s32 *ps, *pd0, *pd1, *pd2;
	mlib_s32 j, i, n;
	const __m128i x_zero = _mm_setzero_si128();

	if ((src0_str == width * 6) &&
	    (dst0_str == width * 2) &&
	    (dst1_str == width * 2) &&
	    (dst2_str == width * 2)) {
		width *= height;
		height = 1;
	}

	n = width;

	for (j = 0; j < height; j++) {

		i = 0;
		ps = (mlib_s32 *)src_s32_0;
		pd0 = dst_s32_0;
		pd1 = dst_s32_1;
		pd2 = dst_s32_2;

		if (0 == (0xf & ((mlib_addr)src_s32_0 |
		    (mlib_addr)dst_s32_0 |
		    (mlib_addr)dst_s32_1 |
		    (mlib_addr)dst_s32_2))) {
/*
 * Ideal case: both dst vectors and src vector are
 * aligned to 16 bytes
 */

/* 4 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i <= (n - 4); i += 4) {
				x_sd0 = _mm_load_si128((__m128i *)ps);
				ps += 4;
				x_sd1 = _mm_load_si128((__m128i *)ps);
				ps += 4;
				x_sd2 = _mm_load_si128((__m128i *)ps);
				ps += 4;

				MLIB_S_Separate48_S32;

				_mm_store_si128((__m128i *)pd0, x_dd0);
				pd0 += 4;
				_mm_store_si128((__m128i *)pd1, x_dd1);
				pd1 += 4;
				_mm_store_si128((__m128i *)pd2, x_dd2);
				pd2 += 4;
			}

		} else {

/*
 * not both dst vectors and src vector are
 * aligned to 16 bytes
 */

/* 4 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i <= (n - 4); i += 4) {
				x_sd0 = _mm_loadu_si128((__m128i *)ps);
				ps += 4;
				x_sd1 = _mm_loadu_si128((__m128i *)ps);
				ps += 4;
				x_sd2 = _mm_loadu_si128((__m128i *)ps);
				ps += 4;

				MLIB_S_Separate48_S32;

				_mm_storeu_si128((__m128i *)pd0, x_dd0);
				pd0 += 4;
				_mm_storeu_si128((__m128i *)pd1, x_dd1);
				pd1 += 4;
				_mm_storeu_si128((__m128i *)pd2, x_dd2);
				pd2 += 4;
			}
		}

/* 2 pixels */
		if (i < (n - 1)) {
			x_sd0 = _mm_loadu_si128((__m128i *)ps);
			ps += 4;
			x_sd1 = _mm_loadl_epi64((__m128i *)ps);
			ps += 2;

			MLIB_S_Separate24_S32;

			_mm_storel_epi64((__m128i *)pd0, x_dd0);
			pd0 += 2;
			_mm_storel_epi64((__m128i *)pd1, x_dd1);
			pd1 += 2;
			_mm_storel_epi64((__m128i *)pd2, x_dd2);
			pd2 += 2;

			i += 2;
		}

/* pure C implementation */
		mlib_s32 *sp = (mlib_s32 *)ps;
		mlib_s32 *dp0 = (mlib_s32 *)pd0;
		mlib_s32 *dp1 = (mlib_s32 *)pd1;
		mlib_s32 *dp2 = (mlib_s32 *)pd2;

/* 1 pixels */
		if (i < n) {
			*(dp0++) = *(sp++);
			*(dp1++) = *(sp++);
			*(dp2++) = *(sp++);
		}

		src_s32_0 = (mlib_s32 *)((mlib_u8 *)src_s32_0 + src0_str);
		dst_s32_0 = (mlib_s32 *)((mlib_u8 *)dst_s32_0 + dst0_str);
		dst_s32_1 = (mlib_s32 *)((mlib_u8 *)dst_s32_1 + dst1_str);
		dst_s32_2 = (mlib_s32 *)((mlib_u8 *)dst_s32_2 + dst2_str);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelSplit4_FS32(
    mlib_s32 *dst_s32_0,
    mlib_s32 *dst_s32_1,
    mlib_s32 *dst_s32_2,
    mlib_s32 *dst_s32_3,
    const mlib_s32 *src_s32_0,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst0_str,
    mlib_s32 dst1_str,
    mlib_s32 dst2_str,
    mlib_s32 dst3_str,
    mlib_s32 src0_str)
{
	__m128i *x_sp, *x_dp0, *x_dp1, *x_dp2, *x_dp3;
	__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_sd4, x_sd5, x_sd6, x_sd7;
	__m128i x_sd8, x_sd9, x_sd10, x_sd11, x_sd12, x_sd13, x_sd14, x_sd15;
	__m128i x_dd0, x_dd1;
	const __m128i x_zero = _mm_setzero_si128();
	mlib_s32 j, i, n;

	if ((src0_str == width * 16) &&
	    (dst0_str == width * 4) &&
	    (dst1_str == width * 4) &&
	    (dst2_str == width * 4) &&
	    (dst3_str == width * 4)) {
		width *= height;
		height = 1;
	}

	n = width;

	for (j = 0; j < height; j++) {

		i = 0;
		x_sp = (__m128i *)src_s32_0;
		x_dp0 = (__m128i *)dst_s32_0;
		x_dp1 = (__m128i *)dst_s32_1;
		x_dp2 = (__m128i *)dst_s32_2;
		x_dp3 = (__m128i *)dst_s32_3;

		if (0 == (0xf & ((mlib_addr)src_s32_0 |
		    (mlib_addr)dst_s32_0 |
		    (mlib_addr)dst_s32_1 |
		    (mlib_addr)dst_s32_2 |
		    (mlib_addr)dst_s32_3))) {

/*
 * Ideal case: both dst vectors and src vector are
 * aligned to 16 bytes
 */

/* 4 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */

			for (; i < (n - 3); i += 4) {
				x_sd0 = _mm_load_si128(x_sp);
				x_sp++;
				x_sd1 = _mm_load_si128(x_sp);
				x_sp++;
				x_sd2 = _mm_load_si128(x_sp);
				x_sp++;
				x_sd3 = _mm_load_si128(x_sp);
				x_sp++;
				x_sd4 = _mm_unpacklo_epi32(x_sd0, x_sd1);
				x_sd5 = _mm_unpackhi_epi32(x_sd0, x_sd1);
				x_sd6 = _mm_unpacklo_epi32(x_sd2, x_sd3);
				x_sd7 = _mm_unpackhi_epi32(x_sd2, x_sd3);

				x_sd8 = _mm_unpacklo_epi64(x_sd4, x_sd6);
				x_sd9 = _mm_unpackhi_epi64(x_sd4, x_sd6);
				x_sd10 = _mm_unpacklo_epi64(x_sd5, x_sd7);
				x_sd11 = _mm_unpackhi_epi64(x_sd5, x_sd7);

				_mm_store_si128(x_dp0, x_sd8);
				_mm_store_si128(x_dp1, x_sd9);
				_mm_store_si128(x_dp2, x_sd10);
				_mm_store_si128(x_dp3, x_sd11);
				x_dp0++;
				x_dp1++;
				x_dp2++;
				x_dp3++;
			}

		} else {

/*
 * not both dst vectors and src vector are
 * aligned to 16 bytes
 */

/* 4 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < (n - 3); i += 4) {
				x_sd0 = _mm_loadu_si128(x_sp);
				x_sp++;
				x_sd1 = _mm_loadu_si128(x_sp);
				x_sp++;
				x_sd2 = _mm_loadu_si128(x_sp);
				x_sp++;
				x_sd3 = _mm_loadu_si128(x_sp);
				x_sp++;
				x_sd4 = _mm_unpacklo_epi32(x_sd0, x_sd1);
				x_sd5 = _mm_unpackhi_epi32(x_sd0, x_sd1);
				x_sd6 = _mm_unpacklo_epi32(x_sd2, x_sd3);
				x_sd7 = _mm_unpackhi_epi32(x_sd2, x_sd3);

				x_sd8 = _mm_unpacklo_epi64(x_sd4, x_sd6);
				x_sd9 = _mm_unpackhi_epi64(x_sd4, x_sd6);
				x_sd10 = _mm_unpacklo_epi64(x_sd5, x_sd7);
				x_sd11 = _mm_unpackhi_epi64(x_sd5, x_sd7);

				_mm_storeu_si128(x_dp0, x_sd8);
				_mm_storeu_si128(x_dp1, x_sd9);
				_mm_storeu_si128(x_dp2, x_sd10);
				_mm_storeu_si128(x_dp3, x_sd11);
				x_dp0++;
				x_dp1++;
				x_dp2++;
				x_dp3++;
			}
		}

/* 2 pixels */
		if (i < (n - 1)) {
			x_sd0 = _mm_loadu_si128(x_sp);
			x_sp++;
			x_sd1 = _mm_loadu_si128(x_sp);
			x_sp++;
			x_sd2 = _mm_unpacklo_epi32(x_sd0, x_sd1);
			x_sd3 = _mm_unpackhi_epi32(x_sd0, x_sd1);

			x_sd4 = _mm_unpackhi_epi64(x_sd2, x_zero);
			x_sd5 = _mm_unpackhi_epi64(x_sd3, x_zero);

			_mm_storel_epi64(x_dp0, x_sd2);
			_mm_storel_epi64(x_dp1, x_sd4);
			_mm_storel_epi64(x_dp2, x_sd3);
			_mm_storel_epi64(x_dp3, x_sd5);
			x_dp0 = (__m128i *)((mlib_s32 *)x_dp0 + 2);
			x_dp1 = (__m128i *)((mlib_s32 *)x_dp1 + 2);
			x_dp2 = (__m128i *)((mlib_s32 *)x_dp2 + 2);
			x_dp3 = (__m128i *)((mlib_s32 *)x_dp3 + 2);
			i += 2;
		}

/* pure C implementation */
		mlib_s32 *sp_s32 = (mlib_s32 *)x_sp;
		mlib_s32 *dp0_s32 = (mlib_s32 *)x_dp0;
		mlib_s32 *dp1_s32 = (mlib_s32 *)x_dp1;
		mlib_s32 *dp2_s32 = (mlib_s32 *)x_dp2;
		mlib_s32 *dp3_s32 = (mlib_s32 *)x_dp3;

/* 1 pixels */
		if (i < n) {
			*(dp0_s32++) = *(sp_s32++);
			*(dp1_s32++) = *(sp_s32++);
			*(dp2_s32++) = *(sp_s32++);
			*(dp3_s32++) = *(sp_s32++);
		}

		dst_s32_0 = (mlib_s32 *)((mlib_u8 *)dst_s32_0 + dst0_str);
		dst_s32_1 = (mlib_s32 *)((mlib_u8 *)dst_s32_1 + dst1_str);
		dst_s32_2 = (mlib_s32 *)((mlib_u8 *)dst_s32_2 + dst2_str);
		dst_s32_3 = (mlib_s32 *)((mlib_u8 *)dst_s32_3 + dst3_str);
		src_s32_0 = (mlib_s32 *)((mlib_u8 *)src_s32_0 + src0_str);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
