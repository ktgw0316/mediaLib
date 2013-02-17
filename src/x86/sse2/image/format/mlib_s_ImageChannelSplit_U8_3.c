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

#pragma ident	"@(#)mlib_s_ImageChannelSplit_U8_3.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageChannelSplit3_U8.
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

#define	MLIB_S_Separate48						\
{									\
	x_t1 = _mm_unpacklo_epi8(x_sd0, _mm_srli_si128(x_sd1, 8));	\
	x_t2 = _mm_unpacklo_epi8(_mm_srli_si128(x_sd0, 8), x_sd2);	\
	x_t3 = _mm_unpacklo_epi8(x_sd1, _mm_srli_si128(x_sd2, 8));	\
	x_t4 = _mm_unpacklo_epi8(x_t1, _mm_srli_si128(x_t2, 8));	\
	x_t5 = _mm_unpacklo_epi8(_mm_srli_si128(x_t1, 8), x_t3);	\
									\
	x_t6 = _mm_unpacklo_epi8(x_t4, _mm_srli_si128(x_t5, 8));	\
	x_t7 = _mm_unpacklo_epi8(x_t2, _mm_srli_si128(x_t3, 8));	\
	x_t8 = _mm_unpacklo_epi8(_mm_srli_si128(x_t4, 8), x_t7);	\
	x_t9 = _mm_unpacklo_epi8(x_t5, _mm_srli_si128(x_t7, 8));	\
									\
	x_dd0 = _mm_unpacklo_epi8(x_t6, _mm_srli_si128(x_t8, 8));	\
	x_dd1 = _mm_unpacklo_epi8(_mm_srli_si128(x_t6, 8), x_t9);	\
	x_dd2 = _mm_unpacklo_epi8(x_t8, _mm_srli_si128(x_t9, 8));	\
}

#define	MLIB_S_Separate24						\
{									\
	x_t1 = _mm_unpacklo_epi8(x_sd0, _mm_srli_si128(x_sd1, 8));	\
	x_t2 = _mm_unpacklo_epi8(_mm_srli_si128(x_sd0, 8), x_zero);	\
	x_t3 = _mm_unpacklo_epi8(x_sd1, x_zero);			\
	x_t4 = _mm_unpacklo_epi8(x_t1, _mm_srli_si128(x_t2, 8));	\
	x_t5 = _mm_unpacklo_epi8(_mm_srli_si128(x_t1, 8), x_t3);	\
									\
	x_t6 = _mm_unpacklo_epi8(x_t4, _mm_srli_si128(x_t5, 8));	\
	x_t7 = _mm_unpacklo_epi8(x_t2, _mm_srli_si128(x_t3, 8));	\
	x_t8 = _mm_unpacklo_epi8(_mm_srli_si128(x_t4, 8), x_t7);	\
	x_t9 = _mm_unpacklo_epi8(x_t5, _mm_srli_si128(x_t7, 8));	\
									\
	x_dd0 = _mm_unpacklo_epi8(x_t6, _mm_srli_si128(x_t8, 8));	\
	x_dd1 = _mm_unpacklo_epi8(_mm_srli_si128(x_t6, 8), x_t9);	\
	x_dd2 = _mm_unpacklo_epi8(x_t8, _mm_srli_si128(x_t9, 8));	\
}

/* *********************************************************** */

mlib_status
mlib_ImageChannelSplit3_U8(
    mlib_u8 *dst_u8_0,
    mlib_u8 *dst_u8_1,
    mlib_u8 *dst_u8_2,
    const mlib_u8 *src_u8_0,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst0_str,
    mlib_s32 dst1_str,
    mlib_s32 dst2_str,
    mlib_s32 src0_str)
{
	__m128i x_sd0, x_sd1, x_sd2;
	__m128i x_t0, x_t1, x_t2, x_t3, x_t4;
	__m128i x_t5, x_t6, x_t7, x_t8, x_t9;
	__m128i x_dd0, x_dd1, x_dd2;
	mlib_u8 *ps, *pd0, *pd1, *pd2;
	mlib_s32 j, i, n;
	const __m128i x_zero = _mm_setzero_si128();

	if ((src0_str == width * 3) &&
	    (dst0_str == width) &&
	    (dst1_str == width) &&
	    (dst2_str == width)) {
		width *= height;
		height = 1;
	}

	n = width;

	for (j = 0; j < height; j++) {

		i = 0;
		ps = (mlib_u8 *)src_u8_0;
		pd0 = dst_u8_0;
		pd1 = dst_u8_1;
		pd2 = dst_u8_2;

		if (0 == (0xf & ((mlib_addr)src_u8_0 |
		    (mlib_addr)dst_u8_0 |
		    (mlib_addr)dst_u8_1 |
		    (mlib_addr)dst_u8_2))) {
/*
 * Ideal case: both dst vectors and src vector are
 * aligned to 16 bytes
 */

/* 16 pixels */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i <= (n - 16); i += 16) {
				x_sd0 = _mm_load_si128((__m128i *)ps);
				ps += 16;
				x_sd1 = _mm_load_si128((__m128i *)ps);
				ps += 16;
				x_sd2 = _mm_load_si128((__m128i *)ps);
				ps += 16;

				MLIB_S_Separate48;

				_mm_store_si128((__m128i *)pd0, x_dd0);
				pd0 += 16;
				_mm_store_si128((__m128i *)pd1, x_dd1);
				pd1 += 16;
				_mm_store_si128((__m128i *)pd2, x_dd2);
				pd2 += 16;
			}

		} else {

/*
 * not both dst vectors and src vector are
 * aligned to 16 bytes
 */

/* 16 pixels */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i <= (n - 16); i += 16) {
				x_sd0 = _mm_loadu_si128((__m128i *)ps);
				ps += 16;
				x_sd1 = _mm_loadu_si128((__m128i *)ps);
				ps += 16;
				x_sd2 = _mm_loadu_si128((__m128i *)ps);
				ps += 16;

				MLIB_S_Separate48;

				_mm_storeu_si128((__m128i *)pd0, x_dd0);
				pd0 += 16;
				_mm_storeu_si128((__m128i *)pd1, x_dd1);
				pd1 += 16;
				_mm_storeu_si128((__m128i *)pd2, x_dd2);
				pd2 += 16;
			}
		}

/* 8 pixels */
		if (i <= (n - 8)) {
			x_sd0 = _mm_loadu_si128((__m128i *)ps);
			ps += 16;
			x_sd1 = _mm_loadl_epi64((__m128i *)ps);
			ps += 8;

			MLIB_S_Separate24;

			_mm_storel_epi64((__m128i *)pd0, x_dd0);
			pd0 += 8;
			_mm_storel_epi64((__m128i *)pd1, x_dd1);
			pd1 += 8;
			_mm_storel_epi64((__m128i *)pd2, x_dd2);
			pd2 += 8;

			i += 8;
		}

/* Pure C implementation */
		mlib_u16 *sp = (mlib_u16 *)ps;
		mlib_u16 *dp0 = (mlib_u16 *)pd0;
		mlib_u16 *dp1 = (mlib_u16 *)pd1;
		mlib_u16 *dp2 = (mlib_u16 *)pd2;
		mlib_s32 s0, s1, s2;

/* 2 pixels */
		for (; i <= (n - 2); i += 2) {
			s0 = sp[0];
			s1 = sp[1];
			s2 = sp[2];
			(*dp0++) = (s1 & ~0xFF) + (s0 & 0xFF);
			(*dp1++) = (s2 << 8) + (s0 >> 8);
			(*dp2++) = (s2 & ~0xFF) + (s1 & 0xFF);
			sp += 3;
		}

/* 1 pixels */
		if (i < n) {
			s0 = sp[0];
			*((mlib_u8 *)dp0) = s0;
			*((mlib_u8 *)dp1) = s0 >> 8;
			*((mlib_u8 *)dp2) = ((mlib_u8 *)sp)[2];
		}

		src_u8_0 = (mlib_u8 *)((mlib_u8 *)src_u8_0 + src0_str);
		dst_u8_0 = (mlib_u8 *)((mlib_u8 *)dst_u8_0 + dst0_str);
		dst_u8_1 = (mlib_u8 *)((mlib_u8 *)dst_u8_1 + dst1_str);
		dst_u8_2 = (mlib_u8 *)((mlib_u8 *)dst_u8_2 + dst2_str);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
