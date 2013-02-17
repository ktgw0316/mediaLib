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

#pragma ident   "@(#)mlib_s_ImageConv2x2_u16nw.c	9.3	07/11/05 SMI"

/*
 *      2-D convolution.
 */

#include <mlib_image.h>
#include <mlib_sse_utils.h>
#ifdef  __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif
/* *********************************************************** */

#define	BUFF_LINE	256

/* *********************************************************** */

#define	sse_mul_16x16(x, y)                                    \
	_mm_unpacklo_epi16(_mm_mullo_epi16(x, y),                  \
		_mm_mulhi_epi16(x, y))

/* *********************************************************** */

#define	GET_SRC_DST_PARAMETERS(type)                           \
	mlib_s32 height = mlib_ImageGetHeight(src);                \
	mlib_s32 width = mlib_ImageGetWidth(src);                  \
	mlib_s32 sll = mlib_ImageGetStride(src) / sizeof (type);   \
	    mlib_s32                                               \
		dll = mlib_ImageGetStride(dst) / sizeof (type);        \
	    type *                                                 \
		sl = (type *) mlib_ImageGetData(src);                  \
	    type *                                                 \
		dl = (type *) mlib_ImageGetData(dst);                  \
	    type *                                                 \
	    sp, *                                                  \
		dp

/* *********************************************************** */
#define	GET_KERN()                                              \
	ker1 = _mm_set1_epi16((kern[0] >> 16) & 0xFFFF);        \
	ker2 = _mm_set1_epi16((kern[1] >> 16) & 0xFFFF);        \
	ker3 = _mm_set1_epi16((kern[2] >> 16) & 0xFFFF);        \
	ker4 = _mm_set1_epi16((kern[3] >> 16) & 0xFFFF);        \
	shift = scalef_expon - 16;                              \
	ker_sum = 0;                                            \
	for (i = 0; i < 4; i++) {                               \
	    ker_sum += (kern[i] >> 16);                         \
	}                                                       \
	                                                        \
	if (shift <= 15) {                                      \
	    ker_sum <<= (15 - shift);                           \
	} else {                                                \
	    ker_sum >>= (shift - 15);                           \
	}                                                       \
	                                                        \
	ker_off = _mm_set1_epi32(ker_sum - 0x8000);             \
	mask8000 = _mm_set1_epi16(0x8000)

/* *********************************************************** */

#define	LOAD_SRC(ptr)                                       \
	_mm_xor_si128(_mm_loadl_epi64((void *)(ptr)), mask8000)

/* *********************************************************** */
#define	PREP_2x2(NCHAN)                                     \
	d0 = LOAD_SRC(sp);                                      \
	sum0 = sse_mul_16x16(d0, ker1);                         \
	aa = LOAD_SRC(sp + NCHAN);                              \
	sum0 = _mm_add_epi32(sum0, sse_mul_16x16(aa, ker2));    \
	buff0[i] = sum0;                                        \
	sp += 4

/* *********************************************************** */
#define	CONV_2x2(NCHAN, i)                                  \
	prev0 = buff0[i];                                       \
	d0 = LOAD_SRC(sp);                                      \
	sum0 = sse_mul_16x16(d0, ker1);                         \
	sum1 = sse_mul_16x16(d0, ker3);                         \
	aa = LOAD_SRC(sp + NCHAN);                              \
	sum0 = _mm_add_epi32(sum0, sse_mul_16x16(aa, ker2));    \
	sum1 = _mm_add_epi32(sum1, sse_mul_16x16(aa, ker4));    \
	sum1 = _mm_add_epi32(sum1, prev0);                      \
	buff0[i] = sum0;                                        \
	sp += 4;                                                \
	res = _mm_srai_epi32(sum1, shift);                      \
	res = _mm_add_epi32(res, ker_off);                      \
	res = _mm_packs_epi32(res, res);                        \
	res = _mm_xor_si128(res, mask8000)

/* *********************************************************** */

#define	FUNC_CONV2x2_u16nw(NCHAN)                           \
	mlib_status                                             \
	mlib_s_conv2x2_u16nw_##NCHAN(                           \
		mlib_image *dst, mlib_image *src,                   \
		mlib_s32 *kern, mlib_s32 scalef_expon)              \
	{                                                       \
	    __m128i buff_loc[BUFF_LINE], *pbuff = buff_loc;     \
	    __m128i *buff0, *buff1, *buff2, *buffT;             \
	    GET_SRC_DST_PARAMETERS(mlib_s16);                   \
	    __m128i ker1, ker2, ker3, ker4, ker5,               \
			ker6, ker7, ker8, ker9;                         \
	    __m128i s0, d0, d1, prev0, prev1,                   \
			sum0, sum1, aa, bb, res;                        \
	    __m128i mask8000, ker_off;                          \
	    __m128i mzero = _mm_setzero_si128();                \
	    mlib_s32 shift, ker_sum;                            \
	    mlib_s32 row, wid4, i, j;                           \
	                                                        \
	    width -= 1;                                         \
	    height -= 1;                                        \
	    width *= NCHAN;                                     \
	    wid4 = (width + 3) / 4;                             \
	    if (wid4 > BUFF_LINE) {                             \
			pbuff = __mlib_malloc(sizeof (__m128i) * wid4); \
	    }                                                   \
	    GET_KERN();                                         \
	    buff0 = pbuff;                                      \
		sp = sl;                                            \
		for (i = 0; i < wid4; i++) {                        \
		    PREP_2x2(NCHAN);                                \
		}                                                   \
		sl += sll;                                          \
	    for (row = 0; row < height; row++) {                \
			sp = sl;                                        \
			dp = dl;                                        \
			for (i = 0; i < width / 4; i++) {               \
				CONV_2x2(NCHAN, i);                         \
				_mm_storel_epi64((void *)dp, res);          \
				dp += 4;                                    \
			}                                               \
			if (width & 3) {                                \
				__m128i mask, dd;                           \
	                                                        \
				CONV_2x2(NCHAN, i);                         \
				mask =                                      \
				_mm_loadl_epi64(                            \
				(void *)(mlib_mask64_arr +                  \
				2 * (width & 3)));                          \
				dd = _mm_loadl_epi64((void *)dp);           \
				dd = _mm_or_si128(_mm_and_si128(mask, res), \
				_mm_andnot_si128(mask, dd));                \
				_mm_storel_epi64((void *)dp, dd);           \
			}                                               \
			sl += sll;                                      \
			dl += dll;                                      \
	    }                                                   \
	    if (pbuff != buff_loc)                              \
			__mlib_free(pbuff);                               \
	    return (MLIB_SUCCESS);                              \
	}

/* *********************************************************** */

FUNC_CONV2x2_u16nw(1)
    FUNC_CONV2x2_u16nw(2)
    FUNC_CONV2x2_u16nw(3)
    FUNC_CONV2x2_u16nw(4)

/* *********************************************************** */
