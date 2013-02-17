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

#pragma ident	"@(#)mlib_s_ImageChannelSplit_U8_24.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageChannelSplit2_U8
 *      mlib_ImageChannelSplit4_U8
 *
 * DESCRIPTION
 *      Internal functions.
 *
 */

#include <stdio.h>
#include <mlib_image.h>
#include <mlib_ImageChannelSplit.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

mlib_status
mlib_ImageChannelSplit2_U8(
    mlib_u8 *dst_u8_0,
    mlib_u8 *dst_u8_1,
    const mlib_u8 *src_u8_0,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst0_str,
    mlib_s32 dst1_str,
    mlib_s32 src0_str)
{
	__m128i *x_sp, *x_dp0, *x_dp1;
	__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_sd4, x_sd5, x_sd6, x_sd7;
	__m128i x_dd0, x_dd1;
	mlib_s32 j, i, i0, i1, n;
	mlib_f32 *df0, *df1;
	mlib_u16 *dp0, *dp1, *sp0;
	mlib_u16 s0, s1, s2;
	const __m128i splitmask0 = _mm_set1_epi16(0x00ff);

	if ((src0_str == width * 2) &&
	    (dst0_str == width) &&
	    (dst1_str == width)) {
		width *= height;
		height = 1;
	}

	n = width * 2;

	for (j = 0; j < height; j++) {

		i = 0;
		x_sp = (__m128i *)src_u8_0;
		x_dp0 = (__m128i *)dst_u8_0;
		x_dp1 = (__m128i *)dst_u8_1;

		if ((((mlib_addr)src_u8_0 & 15) == 0) &&
		    (((mlib_addr)dst_u8_0 & 15) == 0) &&
		    (((mlib_addr)dst_u8_1 & 15) == 0)) {

/*
 * Ideal case: both dst vectors and src vector are
 * aligned to 16 bytes
 */

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < (n - 31); i += 32) {
				x_sd0 = _mm_load_si128(x_sp);
				x_sp++;
				x_sd1 = _mm_load_si128(x_sp);
				x_sp++;
				x_sd2 = _mm_unpacklo_epi8(x_sd0, x_sd1);
				x_sd3 = _mm_unpackhi_epi8(x_sd0, x_sd1);

				x_sd4 = _mm_unpacklo_epi8(x_sd2, x_sd3);
				x_sd5 = _mm_unpackhi_epi8(x_sd2, x_sd3);

				x_sd6 = _mm_unpacklo_epi8(x_sd4, x_sd5);
				x_sd7 = _mm_unpackhi_epi8(x_sd4, x_sd5);

				x_dd0 = _mm_unpacklo_epi16(x_sd6, x_sd7);
				x_dd1 = _mm_unpackhi_epi16(x_sd6, x_sd7);

				_mm_store_si128(x_dp0, x_dd0);
				_mm_store_si128(x_dp1, x_dd1);
				x_dp0++;
				x_dp1++;
			}

		} else {

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < (n - 31); i += 32) {
				x_sd0 = _mm_loadu_si128(x_sp);
				x_sp++;
				x_sd1 = _mm_loadu_si128(x_sp);
				x_sp++;
				x_sd2 = _mm_unpacklo_epi8(x_sd0, x_sd1);
				x_sd3 = _mm_unpackhi_epi8(x_sd0, x_sd1);

				x_sd4 = _mm_unpacklo_epi8(x_sd2, x_sd3);
				x_sd5 = _mm_unpackhi_epi8(x_sd2, x_sd3);

				x_sd6 = _mm_unpacklo_epi8(x_sd4, x_sd5);
				x_sd7 = _mm_unpackhi_epi8(x_sd4, x_sd5);

				x_dd0 = _mm_unpacklo_epi8(x_sd6, x_sd7);
				x_dd1 = _mm_unpackhi_epi8(x_sd6, x_sd7);

				_mm_storeu_si128(x_dp0, x_dd0);
				_mm_storeu_si128(x_dp1, x_dd1);
				x_dp0++;
				x_dp1++;
			}
		}

/* 8 pixels */
			const __m128i x_zero = _mm_setzero_si128();
			if (i < (n - 15)) {
				x_sd0 = _mm_loadu_si128(x_sp);
				x_sp++;
				x_sd1 = _mm_unpackhi_epi64(x_sd0, x_zero);
				x_sd2 = _mm_unpacklo_epi8(x_sd0, x_sd1);
				x_sd3 = _mm_unpackhi_epi64(x_sd2, x_zero);
				x_sd4 = _mm_unpacklo_epi8(x_sd2, x_sd3);
				x_sd5 = _mm_unpackhi_epi64(x_sd4, x_zero);
				x_sd6 = _mm_unpacklo_epi8(x_sd4, x_sd5);
				x_sd7 = _mm_unpackhi_epi64(x_sd6, x_zero);

				_mm_storel_epi64(x_dp0, x_sd6);
				_mm_storel_epi64(x_dp1, x_sd7);
				x_dp0 = (__m128i *)((mlib_u8 *)x_dp0 + 8);
				x_dp1 = (__m128i *)((mlib_u8 *)x_dp1 + 8);
				i += 16;
			}

/* 4 pixels */
		mlib_s32 *dp0 = (mlib_s32 *)x_dp0;
		mlib_s32 *dp1 = (mlib_s32 *)x_dp1;
		if (i < (n - 7)) {
			x_sd0 = _mm_loadl_epi64(x_sp);
			x_sp = (__m128i *)((mlib_u8 *)x_sp + 8);
			x_sd1 = _mm_unpacklo_epi8(x_sd0, x_zero);

			x_sd2 = _mm_unpackhi_epi64(x_sd1, x_zero);
			x_sd3 = _mm_unpacklo_epi8(x_sd1, x_sd2);
			x_sd4 = _mm_unpackhi_epi64(x_sd3, x_zero);
			x_sd5 = _mm_unpacklo_epi8(x_sd3, x_sd4);

			*dp0 = *((mlib_s32 *)&x_sd5);
			*dp1 = *((mlib_s32 *)&x_sd5 + 2);
			dp0++;
			dp1++;
			i += 8;
		}

/* 2 pixels */
		mlib_s16 *ssp = (mlib_s16 *)x_sp;
		mlib_s16 *dp0_s16 = (mlib_s16 *)dp0;
		mlib_s16 *dp1_s16 = (mlib_s16 *)dp1;
		mlib_s16 sd1, sd2, dd1, dd2;

		if (i < (n - 3)) {
			sd1 = (*ssp++);
			sd2 = (*ssp++);
			dd1 = (sd2 << 8) + (sd1 & 0x00ff);
			dd2 = (sd2 & 0xff00) + ((sd1 >> 8) & 0x00ff);

			*(dp0_s16) = dd1;
			*(dp1_s16) = dd2;
			dp0_s16++;
			dp1_s16++;
			i += 4;
		}

/* 1 pixels */
		mlib_u8 *dp0_u8 = (mlib_u8 *)dp0_s16;
		mlib_u8 *dp1_u8 = (mlib_u8 *)dp1_s16;

		if (i < (n - 1)) {
			sd1 = (*ssp++);
			*(dp0_u8) = (sd1 & 0x00ff);
			*(dp1_u8) = ((sd1 >> 8) & 0x00ff);
			dp0_u8++;
			dp1_u8++;
			i += 2;
		}

		dst_u8_0 = (mlib_u8 *)((mlib_u8 *)dst_u8_0 + dst0_str);
		dst_u8_1 = (mlib_u8 *)((mlib_u8 *)dst_u8_1 + dst1_str);
		src_u8_0 = (mlib_u8 *)((mlib_u8 *)src_u8_0 + src0_str);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelSplit4_U8(
    mlib_u8 *dst_u8_0,
    mlib_u8 *dst_u8_1,
    mlib_u8 *dst_u8_2,
    mlib_u8 *dst_u8_3,
    const mlib_u8 *src_u8_0,
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
	__m128i x_sd16, x_sd17, x_sd18, x_sd19;
	__m128i x_dd0, x_dd1;
	const __m128i x_zero = _mm_setzero_si128();
	mlib_s32 j, i, n;

	if ((src0_str == width * 8) &&
	    (dst0_str == width * 2) &&
	    (dst1_str == width * 2) &&
	    (dst2_str == width * 2) &&
	    (dst3_str == width * 2)) {
		width *= height;
		height = 1;
	}

	n = width * 4;

	for (j = 0; j < height; j++) {

		i = 0;
		x_sp = (__m128i *)src_u8_0;
		x_dp0 = (__m128i *)dst_u8_0;
		x_dp1 = (__m128i *)dst_u8_1;
		x_dp2 = (__m128i *)dst_u8_2;
		x_dp3 = (__m128i *)dst_u8_3;

		if ((((mlib_addr)src_u8_0 & 15) == 0) &&
		    (((mlib_addr)dst_u8_0 & 15) == 0) &&
		    (((mlib_addr)dst_u8_1 & 15) == 0) &&
			(((mlib_addr)dst_u8_2 & 15) == 0) &&
			(((mlib_addr)dst_u8_3 & 15) == 0)) {

/*
 * Ideal case: both dst vectors and src vector are
 * aligned to 16 bytes
 */

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < (n - 63); i += 64) {
				x_sd0 = _mm_load_si128(x_sp);
				x_sp++;
				x_sd1 = _mm_load_si128(x_sp);
				x_sp++;
				x_sd2 = _mm_load_si128(x_sp);
				x_sp++;
				x_sd3 = _mm_load_si128(x_sp);
				x_sp++;
				x_sd4 = _mm_unpacklo_epi8(x_sd0, x_sd1);
				x_sd5 = _mm_unpackhi_epi8(x_sd0, x_sd1);
				x_sd6 = _mm_unpacklo_epi8(x_sd2, x_sd3);
				x_sd7 = _mm_unpackhi_epi8(x_sd2, x_sd3);

				x_sd8 = _mm_unpacklo_epi8(x_sd4, x_sd5);
				x_sd9 = _mm_unpackhi_epi8(x_sd4, x_sd5);
				x_sd10 = _mm_unpacklo_epi8(x_sd6, x_sd7);
				x_sd11 = _mm_unpackhi_epi8(x_sd6, x_sd7);

				x_sd12 = _mm_unpacklo_epi8(x_sd8, x_sd9);
				x_sd13 = _mm_unpackhi_epi8(x_sd8, x_sd9);
				x_sd14 = _mm_unpacklo_epi8(x_sd10, x_sd11);
				x_sd15 = _mm_unpackhi_epi8(x_sd10, x_sd11);

				x_sd16 = _mm_unpacklo_epi64(x_sd12, x_sd14);
				x_sd17 = _mm_unpackhi_epi64(x_sd12, x_sd14);
				x_sd18 = _mm_unpacklo_epi64(x_sd13, x_sd15);
				x_sd19 = _mm_unpackhi_epi64(x_sd13, x_sd15);

				_mm_store_si128(x_dp0, x_sd16);
				_mm_store_si128(x_dp1, x_sd17);
				_mm_store_si128(x_dp2, x_sd18);
				_mm_store_si128(x_dp3, x_sd19);
				x_dp0++;
				x_dp1++;
				x_dp2++;
				x_dp3++;
			}

		} else {

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < (n - 63); i += 64) {
				x_sd0 = _mm_loadu_si128(x_sp);
				x_sp++;
				x_sd1 = _mm_loadu_si128(x_sp);
				x_sp++;
				x_sd2 = _mm_loadu_si128(x_sp);
				x_sp++;
				x_sd3 = _mm_loadu_si128(x_sp);
				x_sp++;
				x_sd4 = _mm_unpacklo_epi8(x_sd0, x_sd1);
				x_sd5 = _mm_unpackhi_epi8(x_sd0, x_sd1);
				x_sd6 = _mm_unpacklo_epi8(x_sd2, x_sd3);
				x_sd7 = _mm_unpackhi_epi8(x_sd2, x_sd3);

				x_sd8 = _mm_unpacklo_epi8(x_sd4, x_sd5);
				x_sd9 = _mm_unpackhi_epi8(x_sd4, x_sd5);
				x_sd10 = _mm_unpacklo_epi8(x_sd6, x_sd7);
				x_sd11 = _mm_unpackhi_epi8(x_sd6, x_sd7);

				x_sd12 = _mm_unpacklo_epi8(x_sd8, x_sd9);
				x_sd13 = _mm_unpackhi_epi8(x_sd8, x_sd9);
				x_sd14 = _mm_unpacklo_epi8(x_sd10, x_sd11);
				x_sd15 = _mm_unpackhi_epi8(x_sd10, x_sd11);

				x_sd16 = _mm_unpacklo_epi64(x_sd12, x_sd14);
				x_sd17 = _mm_unpackhi_epi64(x_sd12, x_sd14);
				x_sd18 = _mm_unpacklo_epi64(x_sd13, x_sd15);
				x_sd19 = _mm_unpackhi_epi64(x_sd13, x_sd15);

				_mm_storeu_si128(x_dp0, x_sd16);
				_mm_storeu_si128(x_dp1, x_sd17);
				_mm_storeu_si128(x_dp2, x_sd18);
				_mm_storeu_si128(x_dp3, x_sd19);
				x_dp0++;
				x_dp1++;
				x_dp2++;
				x_dp3++;
			}
		}

/* 8 pixels */
		if (i < (n - 31)) {
			x_sd0 = _mm_loadu_si128(x_sp);
			x_sp++;
			x_sd1 = _mm_loadu_si128(x_sp);
			x_sp++;
			x_sd4 = _mm_unpacklo_epi8(x_sd0, x_sd1);
			x_sd5 = _mm_unpackhi_epi8(x_sd0, x_sd1);

			x_sd6 = _mm_unpacklo_epi8(x_sd4, x_sd5);
			x_sd7 = _mm_unpackhi_epi8(x_sd4, x_sd5);
			x_sd8 = _mm_unpacklo_epi8(x_sd6, x_sd7);
			x_sd9 = _mm_unpackhi_epi8(x_sd6, x_sd7);

			x_sd10 = _mm_unpackhi_epi64(x_sd8, x_zero);
			x_sd11 = _mm_unpackhi_epi64(x_sd9, x_zero);

			_mm_storel_epi64(x_dp0, x_sd8);
			_mm_storel_epi64(x_dp1, x_sd10);
			_mm_storel_epi64(x_dp2, x_sd9);
			_mm_storel_epi64(x_dp3, x_sd11);
			x_dp0 = (__m128i *)((mlib_u8 *)x_dp0 + 8);
			x_dp1 = (__m128i *)((mlib_u8 *)x_dp1 + 8);
			x_dp2 = (__m128i *)((mlib_u8 *)x_dp2 + 8);
			x_dp3 = (__m128i *)((mlib_u8 *)x_dp3 + 8);
			i += 32;
		}

/* 4 pixels */
		mlib_s32 *sp_s32;
		mlib_s32 *dp0_s32 = (mlib_s32 *)x_dp0;
		mlib_s32 *dp1_s32 = (mlib_s32 *)x_dp1;
		mlib_s32 *dp2_s32 = (mlib_s32 *)x_dp2;
		mlib_s32 *dp3_s32 = (mlib_s32 *)x_dp3;

		if (i < (n - 15)) {
			x_sd0 = _mm_loadu_si128(x_sp);
			x_sp++;
			x_sd1 = _mm_unpackhi_epi64(x_sd0, x_zero);
			x_sd2 = _mm_unpacklo_epi8(x_sd0, x_sd1);
			x_sd3 = _mm_unpackhi_epi64(x_sd2, x_zero);
			x_sd4 = _mm_unpacklo_epi8(x_sd2, x_sd3);

			sp_s32 = (mlib_s32 *)&x_sd4;
			*(dp0_s32++) = *(sp_s32++);
			*(dp1_s32++) = *(sp_s32++);
			*(dp2_s32++) = *(sp_s32++);
			*(dp3_s32++) = *(sp_s32++);
			i += 16;
		}

/* 2 pixels */
		mlib_s16 *sp_s16;
		mlib_s16 *dp0_s16 = (mlib_s16 *)dp0_s32;
		mlib_s16 *dp1_s16 = (mlib_s16 *)dp1_s32;
		mlib_s16 *dp2_s16 = (mlib_s16 *)dp2_s32;
		mlib_s16 *dp3_s16 = (mlib_s16 *)dp3_s32;

		if (i < (n - 7)) {
			x_sd0 = _mm_loadl_epi64(x_sp);
			x_sd1 = _mm_unpacklo_epi8(x_sd0, x_zero);
			x_sd2 = _mm_unpackhi_epi64(x_sd1, x_zero);
			x_sd3 = _mm_unpacklo_epi8(x_sd1, x_sd2);
			x_sd4 = _mm_unpackhi_epi8(x_sd1, x_sd2);
			x_sp = (__m128i *)((mlib_u8 *)x_sp + 8);
			sp_s16 = (mlib_s16 *)&x_sd3;
			*(dp0_s16++) = *(sp_s16);
			sp_s16 += 2;
			*(dp1_s16++) = *(sp_s16);
			sp_s16 += 2;
			*(dp2_s16++) = *(sp_s16);
			sp_s16 += 2;
			*(dp3_s16++) = *(sp_s16);
			i += 8;
		}

/* 1 pixels */
		mlib_u8 *sp_u8 = (mlib_u8 *)x_sp;
		mlib_u8 *dp0_u8 = (mlib_u8 *)dp0_s16;
		mlib_u8 *dp1_u8 = (mlib_u8 *)dp1_s16;
		mlib_u8 *dp2_u8 = (mlib_u8 *)dp2_s16;
		mlib_u8 *dp3_u8 = (mlib_u8 *)dp3_s16;

		if (i < (n - 3)) {
			*(dp0_u8++) = *(sp_u8++);
			*(dp1_u8++) = *(sp_u8++);
			*(dp2_u8++) = *(sp_u8++);
			*(dp3_u8++) = *(sp_u8++);
			i += 4;
		}

		dst_u8_0 = (mlib_u8 *)((mlib_u8 *)dst_u8_0 + dst0_str);
		dst_u8_1 = (mlib_u8 *)((mlib_u8 *)dst_u8_1 + dst1_str);
		dst_u8_2 = (mlib_u8 *)((mlib_u8 *)dst_u8_2 + dst2_str);
		dst_u8_3 = (mlib_u8 *)((mlib_u8 *)dst_u8_3 + dst3_str);
		src_u8_0 = (mlib_u8 *)((mlib_u8 *)src_u8_0 + src0_str);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
