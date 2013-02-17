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

#pragma ident	"@(#)mlib_s_ImageChannelSplit_S16.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageChannelSplit2_S16
 *      mlib_ImageChannelSplit3_S16
 *      mlib_ImageChannelSplit4_S16
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
mlib_ImageChannelSplit2_S16(
    mlib_s16 *dst_s16_0,
    mlib_s16 *dst_s16_1,
    const mlib_s16 *src_s16_0,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst0_str,
    mlib_s32 dst1_str,
    mlib_s32 src0_str)
{
	__m128i *x_sp, *x_dp0, *x_dp1;
	__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_sd4, x_sd5, x_sd6, x_sd7;
	__m128i x_dd0, x_dd1;
	mlib_s32 j, i, n;
	const __m128i x_zero = _mm_setzero_si128();

	if ((src0_str == width * 4) &&
	    (dst0_str == width * 2) &&
	    (dst1_str == width * 2)) {
		width *= height;
		height = 1;
	}

	n = width * 2;

	for (j = 0; j < height; j++) {

		i = 0;
		x_sp = (__m128i *)src_s16_0;
		x_dp0 = (__m128i *)dst_s16_0;
		x_dp1 = (__m128i *)dst_s16_1;

		if ((((mlib_addr)src_s16_0 & 15) == 0) &&
		    (((mlib_addr)dst_s16_0 & 15) == 0) &&
		    (((mlib_addr)dst_s16_1 & 15) == 0)) {

/*
 * Ideal case: both dst vectors and src vector are
 * aligned to 16 bytes
 */

/* 8 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < (n - 15); i += 16) {
				x_sd0 = _mm_load_si128(x_sp);
				x_sp++;
				x_sd1 = _mm_load_si128(x_sp);
				x_sp++;
				x_sd2 = _mm_unpacklo_epi16(x_sd0, x_sd1);
				x_sd3 = _mm_unpackhi_epi16(x_sd0, x_sd1);

				x_sd4 = _mm_unpacklo_epi16(x_sd2, x_sd3);
				x_sd5 = _mm_unpackhi_epi16(x_sd2, x_sd3);

				x_sd6 = _mm_unpacklo_epi16(x_sd4, x_sd5);
				x_sd7 = _mm_unpackhi_epi16(x_sd4, x_sd5);

				_mm_store_si128(x_dp0, x_sd6);
				_mm_store_si128(x_dp1, x_sd7);
				x_dp0++;
				x_dp1++;
			}

		} else {

/*
 * not both dst vectors and src vector are
 * aligned to 16 bytes
 */

/* 8 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < (n - 15); i += 16) {
				x_sd0 = _mm_loadu_si128(x_sp);
				x_sp++;
				x_sd1 = _mm_loadu_si128(x_sp);
				x_sp++;
				x_sd2 = _mm_unpacklo_epi16(x_sd0, x_sd1);
				x_sd3 = _mm_unpackhi_epi16(x_sd0, x_sd1);

				x_sd4 = _mm_unpacklo_epi16(x_sd2, x_sd3);
				x_sd5 = _mm_unpackhi_epi16(x_sd2, x_sd3);

				x_sd6 = _mm_unpacklo_epi16(x_sd4, x_sd5);
				x_sd7 = _mm_unpackhi_epi16(x_sd4, x_sd5);

				_mm_storeu_si128(x_dp0, x_sd6);
				_mm_storeu_si128(x_dp1, x_sd7);
				x_dp0++;
				x_dp1++;
			}
		}

/* 4 pixels */
		if (i < (n - 7)) {
			x_sd0 = _mm_loadu_si128(x_sp);
			x_sp++;
			x_sd1 = _mm_unpackhi_epi64(x_sd0, x_zero);
			x_sd2 = _mm_unpacklo_epi16(x_sd0, x_sd1);
			x_sd3 = _mm_unpackhi_epi64(x_sd2, x_zero);
			x_sd4 = _mm_unpacklo_epi16(x_sd2, x_sd3);
			x_sd5 = _mm_unpackhi_epi64(x_sd4, x_zero);

			_mm_storel_epi64(x_dp0, x_sd4);
			_mm_storel_epi64(x_dp1, x_sd5);
			x_dp0 = (__m128i *)((mlib_s16 *)x_dp0 + 4);
			x_dp1 = (__m128i *)((mlib_s16 *)x_dp1 + 4);
			i += 8;
		}

		mlib_s32 *sp = (mlib_s32 *)x_sp;
		mlib_s32 *dp0 = (mlib_s32 *)x_dp0;
		mlib_s32 *dp1 = (mlib_s32 *)x_dp1;
		mlib_s32 sd0, sd1, sd2, dd0, dd1;

/* 2 pixels */
		if (i < (n - 3)) {
			sd1 = (*sp++);
			sd2 = (*sp++);
			dd0 = (sd2 << 16) + (sd1 & 0x0000ffff);
			dd1 = (sd2 & 0xffff0000) + ((sd1 >> 16) & 0x0000ffff);
			(*dp0) = dd0;
			(*dp1) = dd1;
			dp0++;
			dp1++;
			i += 4;
		}

/* 1 pixels */
		mlib_s16 *ddp0 = (mlib_s16 *)dp0;
		mlib_s16 *ddp1 = (mlib_s16 *)dp1;

		if (i < (n - 1)) {
			sd0 = (*sp++);
			*(ddp0) = (sd0 & 0x0000ffff);
			*(ddp1) = ((sd0 >> 16) & 0x0000ffff);
			ddp0++;
			ddp1++;
			i += 2;
		}

		dst_s16_0 = (mlib_s16 *)((mlib_u8 *)dst_s16_0 + dst0_str);
		dst_s16_1 = (mlib_s16 *)((mlib_u8 *)dst_s16_1 + dst1_str);
		src_s16_0 = (mlib_s16 *)((mlib_u8 *)src_s16_0 + src0_str);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MLIB_S_Separate48_S16						\
{									\
	x_t0 = _mm_unpacklo_epi16(x_sd0, _mm_srli_si128(x_sd1, 8));	\
	x_t1 = _mm_unpacklo_epi16(_mm_srli_si128(x_sd0, 8), x_sd2);	\
	x_t2 = _mm_unpacklo_epi16(x_sd1, _mm_srli_si128(x_sd2, 8));	\
									\
	x_t3 = _mm_unpacklo_epi16(x_t0, _mm_srli_si128(x_t1, 8));	\
	x_t4 = _mm_unpacklo_epi16(_mm_srli_si128(x_t0, 8), x_t2);	\
	x_t5 = _mm_unpacklo_epi16(x_t1, _mm_srli_si128(x_t2, 8));	\
									\
	x_dd0 = _mm_unpacklo_epi16(x_t3, _mm_srli_si128(x_t4, 8));	\
	x_dd1 = _mm_unpacklo_epi16(_mm_srli_si128(x_t3, 8), x_t5);	\
	x_dd2 = _mm_unpacklo_epi16(x_t4, _mm_srli_si128(x_t5, 8));	\
}

#define	MLIB_S_Separate24_S16						\
{									\
	x_t0 = _mm_unpacklo_epi16(x_sd0, x_zero);			\
	x_t1 = _mm_unpacklo_epi16(_mm_srli_si128(x_sd0, 8), x_zero);	\
	x_t2 = _mm_unpacklo_epi16(x_sd1, x_zero);			\
									\
	x_t3 = _mm_unpacklo_epi16(x_t0, _mm_srli_si128(x_t1, 8));	\
	x_t4 = _mm_unpacklo_epi16(_mm_srli_si128(x_t0, 8), x_t2);	\
	x_t5 = _mm_unpacklo_epi16(x_t1, _mm_srli_si128(x_t2, 8));	\
									\
	x_dd0 = _mm_unpacklo_epi16(x_t3, _mm_srli_si128(x_t4, 8));	\
	x_dd1 = _mm_unpacklo_epi16(_mm_srli_si128(x_t3, 8), x_t5);	\
	x_dd2 = _mm_unpacklo_epi16(x_t4, _mm_srli_si128(x_t5, 8));	\
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelSplit3_S16(
    mlib_s16 *dst_s16_0,
    mlib_s16 *dst_s16_1,
    mlib_s16 *dst_s16_2,
    const mlib_s16 *src_s16_0,
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
	mlib_s16 *ps, *pd0, *pd1, *pd2;
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
		ps = (mlib_s16 *)src_s16_0;
		pd0 = dst_s16_0;
		pd1 = dst_s16_1;
		pd2 = dst_s16_2;

		if (0 == (0xf & ((mlib_addr)src_s16_0 |
		    (mlib_addr)dst_s16_0 |
		    (mlib_addr)dst_s16_1 |
		    (mlib_addr)dst_s16_2))) {
/*
 * Ideal case: both dst vectors and src vector are
 * aligned to 16 bytes
 */

/* 8 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (n - 8); i += 8) {
				x_sd0 = _mm_load_si128((__m128i *)ps);
				ps += 8;
				x_sd1 = _mm_load_si128((__m128i *)ps);
				ps += 8;
				x_sd2 = _mm_load_si128((__m128i *)ps);
				ps += 8;

				MLIB_S_Separate48_S16;

				_mm_store_si128((__m128i *)pd0, x_dd0);
				pd0 += 8;
				_mm_store_si128((__m128i *)pd1, x_dd1);
				pd1 += 8;
				_mm_store_si128((__m128i *)pd2, x_dd2);
				pd2 += 8;
			}

		} else {

/*
 * not both dst vectors and src vector are
 * aligned to 16 bytes
 */

/* 8 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (n - 8); i += 8) {
				x_sd0 = _mm_loadu_si128((__m128i *)ps);
				ps += 8;
				x_sd1 = _mm_loadu_si128((__m128i *)ps);
				ps += 8;
				x_sd2 = _mm_loadu_si128((__m128i *)ps);
				ps += 8;

				MLIB_S_Separate48_S16;

				_mm_storeu_si128((__m128i *)pd0, x_dd0);
				pd0 += 8;
				_mm_storeu_si128((__m128i *)pd1, x_dd1);
				pd1 += 8;
				_mm_storeu_si128((__m128i *)pd2, x_dd2);
				pd2 += 8;
			}
		}

/* 4 pixels */
		if (i <= (n - 4)) {
			x_sd0 = _mm_loadu_si128((__m128i *)ps);
			ps += 8;
			x_sd1 = _mm_loadl_epi64((__m128i *)ps);
			ps += 4;

			MLIB_S_Separate24_S16;

			_mm_storel_epi64((__m128i *)pd0, x_dd0);
			pd0 += 4;
			_mm_storel_epi64((__m128i *)pd1, x_dd1);
			pd1 += 4;
			_mm_storel_epi64((__m128i *)pd2, x_dd2);
			pd2 += 4;

			i += 4;
		}

/* pure C implementation */
		mlib_u32 *sp = (mlib_u32 *)ps;
		mlib_u32 *dp0 = (mlib_u32 *)pd0;
		mlib_u32 *dp1 = (mlib_u32 *)pd1;
		mlib_u32 *dp2 = (mlib_u32 *)pd2;
		mlib_u32 s0, s1, s2;

/* 2 pixels */
		for (; i <= (n - 2); i += 2) {
			s0 = sp[0];
			s1 = sp[1];
			s2 = sp[2];
			(*dp0++) = (s1 & ~0xFFFF) + (s0 & 0xFFFF);
			(*dp1++) = (s2 << 16) + (s0 >> 16);
			(*dp2++) = (s2 & ~0xFFFF) + (s1 & 0xFFFF);
			sp += 3;
		}

/* 1 pixels */
		if (i < n) {
			s0 = sp[0];
			*((mlib_u16 *)dp0) = s0;
			*((mlib_u16 *)dp1) = s0 >> 16;
			*((mlib_u16 *)dp2) = ((mlib_u16 *)sp)[2];
		}

		src_s16_0 = (mlib_s16 *)((mlib_u8 *)src_s16_0 + src0_str);
		dst_s16_0 = (mlib_s16 *)((mlib_u8 *)dst_s16_0 + dst0_str);
		dst_s16_1 = (mlib_s16 *)((mlib_u8 *)dst_s16_1 + dst1_str);
		dst_s16_2 = (mlib_s16 *)((mlib_u8 *)dst_s16_2 + dst2_str);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelSplit4_S16(
    mlib_s16 *dst_s16_0,
    mlib_s16 *dst_s16_1,
    mlib_s16 *dst_s16_2,
    mlib_s16 *dst_s16_3,
    const mlib_s16 *src_s16_0,
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
		x_sp = (__m128i *)src_s16_0;
		x_dp0 = (__m128i *)dst_s16_0;
		x_dp1 = (__m128i *)dst_s16_1;
		x_dp2 = (__m128i *)dst_s16_2;
		x_dp3 = (__m128i *)dst_s16_3;

		if ((((mlib_addr)src_s16_0 & 15) == 0) &&
		    (((mlib_addr)dst_s16_0 & 15) == 0) &&
		    (((mlib_addr)dst_s16_1 & 15) == 0) &&
		    (((mlib_addr)dst_s16_2 & 15) == 0) &&
		    (((mlib_addr)dst_s16_3 & 15) == 0)) {

/*
 * Ideal case: both dst vectors and src vector are
 * aligned to 16 bytes
 */

/* 8 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */

			for (; i < (n - 31); i += 32) {
				x_sd0 = _mm_load_si128(x_sp);
				x_sp++;
				x_sd1 = _mm_load_si128(x_sp);
				x_sp++;
				x_sd2 = _mm_load_si128(x_sp);
				x_sp++;
				x_sd3 = _mm_load_si128(x_sp);
				x_sp++;
				x_sd4 = _mm_unpacklo_epi16(x_sd0, x_sd1);
				x_sd5 = _mm_unpackhi_epi16(x_sd0, x_sd1);
				x_sd6 = _mm_unpacklo_epi16(x_sd2, x_sd3);
				x_sd7 = _mm_unpackhi_epi16(x_sd2, x_sd3);

				x_sd8 = _mm_unpacklo_epi16(x_sd4, x_sd5);
				x_sd9 = _mm_unpackhi_epi16(x_sd4, x_sd5);
				x_sd10 = _mm_unpacklo_epi16(x_sd6, x_sd7);
				x_sd11 = _mm_unpackhi_epi16(x_sd6, x_sd7);

				x_sd12 = _mm_unpacklo_epi64(x_sd8, x_sd10);
				x_sd13 = _mm_unpackhi_epi64(x_sd8, x_sd10);
				x_sd14 = _mm_unpacklo_epi64(x_sd9, x_sd11);
				x_sd15 = _mm_unpackhi_epi64(x_sd9, x_sd11);

				_mm_store_si128(x_dp0, x_sd12);
				_mm_store_si128(x_dp1, x_sd13);
				_mm_store_si128(x_dp2, x_sd14);
				_mm_store_si128(x_dp3, x_sd15);
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

/* 8 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < (n - 31); i += 32) {
				x_sd0 = _mm_loadu_si128(x_sp);
				x_sp++;
				x_sd1 = _mm_loadu_si128(x_sp);
				x_sp++;
				x_sd2 = _mm_loadu_si128(x_sp);
				x_sp++;
				x_sd3 = _mm_loadu_si128(x_sp);
				x_sp++;
				x_sd4 = _mm_unpacklo_epi16(x_sd0, x_sd1);
				x_sd5 = _mm_unpackhi_epi16(x_sd0, x_sd1);
				x_sd6 = _mm_unpacklo_epi16(x_sd2, x_sd3);
				x_sd7 = _mm_unpackhi_epi16(x_sd2, x_sd3);

				x_sd8 = _mm_unpacklo_epi16(x_sd4, x_sd5);
				x_sd9 = _mm_unpackhi_epi16(x_sd4, x_sd5);
				x_sd10 = _mm_unpacklo_epi16(x_sd6, x_sd7);
				x_sd11 = _mm_unpackhi_epi16(x_sd6, x_sd7);

				x_sd12 = _mm_unpacklo_epi64(x_sd8, x_sd10);
				x_sd13 = _mm_unpackhi_epi64(x_sd8, x_sd10);
				x_sd14 = _mm_unpacklo_epi64(x_sd9, x_sd11);
				x_sd15 = _mm_unpackhi_epi64(x_sd9, x_sd11);

				_mm_storeu_si128(x_dp0, x_sd12);
				_mm_storeu_si128(x_dp1, x_sd13);
				_mm_storeu_si128(x_dp2, x_sd14);
				_mm_storeu_si128(x_dp3, x_sd15);
				x_dp0++;
				x_dp1++;
				x_dp2++;
				x_dp3++;
			}
		}

/* 4 pixels */
		if (i < (n - 15)) {
			x_sd0 = _mm_loadu_si128(x_sp);
			x_sp++;
			x_sd1 = _mm_loadu_si128(x_sp);
			x_sp++;
			x_sd4 = _mm_unpacklo_epi16(x_sd0, x_sd1);
			x_sd5 = _mm_unpackhi_epi16(x_sd0, x_sd1);

			x_sd6 = _mm_unpacklo_epi16(x_sd4, x_sd5);
			x_sd7 = _mm_unpackhi_epi16(x_sd4, x_sd5);
			x_sd8 = _mm_unpackhi_epi64(x_sd6, x_zero);
			x_sd9 = _mm_unpackhi_epi64(x_sd7, x_zero);

			_mm_storel_epi64(x_dp0, x_sd6);
			_mm_storel_epi64(x_dp1, x_sd8);
			_mm_storel_epi64(x_dp2, x_sd7);
			_mm_storel_epi64(x_dp3, x_sd9);
			x_dp0 = (__m128i *)((mlib_s16 *)x_dp0 + 4);
			x_dp1 = (__m128i *)((mlib_s16 *)x_dp1 + 4);
			x_dp2 = (__m128i *)((mlib_s16 *)x_dp2 + 4);
			x_dp3 = (__m128i *)((mlib_s16 *)x_dp3 + 4);
			i += 16;
		}

		mlib_s32 *sp_s32;
		mlib_s32 *dp0_s32 = (mlib_s32 *)x_dp0;
		mlib_s32 *dp1_s32 = (mlib_s32 *)x_dp1;
		mlib_s32 *dp2_s32 = (mlib_s32 *)x_dp2;
		mlib_s32 *dp3_s32 = (mlib_s32 *)x_dp3;

/* 2 pixels */
		if (i < (n - 7)) {
			x_sd0 = _mm_loadu_si128(x_sp);
			x_sp++;
			x_sd1 = _mm_unpackhi_epi64(x_sd0, x_zero);
			x_sd2 = _mm_unpacklo_epi16(x_sd0, x_sd1);
			sp_s32 = (mlib_s32 *)&x_sd2;
			*(dp0_s32++) = *(sp_s32++);
			*(dp1_s32++) = *(sp_s32++);
			*(dp2_s32++) = *(sp_s32++);
			*(dp3_s32++) = *(sp_s32++);
			i += 8;
		}

		mlib_s16 *sp_s16;
		mlib_s16 *dp0_s16 = (mlib_s16 *)dp0_s32;
		mlib_s16 *dp1_s16 = (mlib_s16 *)dp1_s32;
		mlib_s16 *dp2_s16 = (mlib_s16 *)dp2_s32;
		mlib_s16 *dp3_s16 = (mlib_s16 *)dp3_s32;

/* 1 pixels */
		if (i < (n - 3)) {
			x_sd0 = _mm_loadl_epi64(x_sp);
			x_sp = (__m128i *)((mlib_s16 *)x_sp + 4);
			sp_s16 = (mlib_s16 *)&x_sd0;
			*(dp0_s16++) = *(sp_s16++);
			*(dp1_s16++) = *(sp_s16++);
			*(dp2_s16++) = *(sp_s16++);
			*(dp3_s16++) = *(sp_s16++);
			i += 4;
		}

		dst_s16_0 = (mlib_s16 *)((mlib_u8 *)dst_s16_0 + dst0_str);
		dst_s16_1 = (mlib_s16 *)((mlib_u8 *)dst_s16_1 + dst1_str);
		dst_s16_2 = (mlib_s16 *)((mlib_u8 *)dst_s16_2 + dst2_str);
		dst_s16_3 = (mlib_s16 *)((mlib_u8 *)dst_s16_3 + dst3_str);
		src_s16_0 = (mlib_s16 *)((mlib_u8 *)src_s16_0 + src0_str);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
