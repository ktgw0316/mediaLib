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

#pragma ident	"@(#)mlib_s_ImageConv5x5_16nw.c	9.4	07/11/05 SMI"

/*
 *      2-D convolution.
 */

#include <mlib_image.h>
#include <mlib_sse_utils.h>

/* *********************************************************** */

#define	sse_mul_16x16(x, y)                                                \
	_mm_unpacklo_epi16(_mm_mullo_epi16(x, y), _mm_mulhi_epi16(x, y))

/* *********************************************************** */

#define	GET_SRC_DST_PARAMETERS(type)                               \
	mlib_s32 height = mlib_ImageGetHeight(src);                \
	mlib_s32 width = mlib_ImageGetWidth(src);                  \
	mlib_s32 sll = mlib_ImageGetStride(src) / sizeof (type);   \
	    mlib_s32                                               \
		dll = mlib_ImageGetStride(dst) / sizeof (type);    \
	    type *                                                 \
		sl = (type *) mlib_ImageGetData(src);              \
	    type *                                                 \
		dl = (type *) mlib_ImageGetData(dst);              \
	    type *                                                 \
	    sp, *                                                  \
		dp

/* *********************************************************** */

#define	GET_KERN()                                                   \
	for (j = 0; j < 5; j++) {                                    \
	    for (i = 0; i < 5; i++) {                                \
		ker[j][i] = _mm_set1_epi16(kern[5 * j + i] >> 16);   \
	    }                                                        \
	}                                                            \
	                                                             \
	shift = scalef_expon - 16

/* *********************************************************** */

#define	LOAD_SRC(ptr)	_mm_loadl_epi64((void*)(ptr))

/* *********************************************************** */

#define	PREP_5x5(NCHAN)                                             \
	d0 = LOAD_SRC(sp);                                          \
	sum0 = sse_mul_16x16(d0, ker[0][0]);                        \
	sum1 = sse_mul_16x16(d0, ker[1][0]);                        \
	sum2 = sse_mul_16x16(d0, ker[2][0]);                        \
	sum3 = sse_mul_16x16(d0, ker[3][0]);                        \
	aa = LOAD_SRC(sp + NCHAN);                                  \
	sum0 = _mm_add_epi32(sum0, sse_mul_16x16(aa, ker[0][1]));   \
	sum1 = _mm_add_epi32(sum1, sse_mul_16x16(aa, ker[1][1]));   \
	sum2 = _mm_add_epi32(sum2, sse_mul_16x16(aa, ker[2][1]));   \
	sum3 = _mm_add_epi32(sum3, sse_mul_16x16(aa, ker[3][1]));   \
	bb = LOAD_SRC(sp + 2 * NCHAN);                              \
	sum0 = _mm_add_epi32(sum0, sse_mul_16x16(bb, ker[0][2]));   \
	sum1 = _mm_add_epi32(sum1, sse_mul_16x16(bb, ker[1][2]));   \
	sum2 = _mm_add_epi32(sum2, sse_mul_16x16(bb, ker[2][2]));   \
	sum3 = _mm_add_epi32(sum3, sse_mul_16x16(bb, ker[3][2]));   \
	cc = LOAD_SRC(sp + 3 * NCHAN);                              \
	sum0 = _mm_add_epi32(sum0, sse_mul_16x16(cc, ker[0][3]));   \
	sum1 = _mm_add_epi32(sum1, sse_mul_16x16(cc, ker[1][3]));   \
	sum2 = _mm_add_epi32(sum2, sse_mul_16x16(cc, ker[2][3]));   \
	sum3 = _mm_add_epi32(sum3, sse_mul_16x16(cc, ker[3][3]));   \
	dd = LOAD_SRC(sp + 4 * NCHAN);                              \
	sum0 = _mm_add_epi32(sum0, sse_mul_16x16(dd, ker[0][4]));   \
	sum1 = _mm_add_epi32(sum1, sse_mul_16x16(dd, ker[1][4]));   \
	sum2 = _mm_add_epi32(sum2, sse_mul_16x16(dd, ker[2][4]));   \
	sum3 = _mm_add_epi32(sum3, sse_mul_16x16(dd, ker[3][4]));   \
	buff0[i] = sum0;                                            \
	buff1[i] = sum1;                                            \
	buff2[i] = sum2;                                            \
	buff3[i] = sum3;                                            \
	sp += 4

/* *********************************************************** */

#define	CONV_5x5(NCHAN, i)                                          \
	d0 = LOAD_SRC(sp);                                          \
	sum0 = sse_mul_16x16(d0, ker[0][0]);                        \
	sum1 = sse_mul_16x16(d0, ker[1][0]);                        \
	sum2 = sse_mul_16x16(d0, ker[2][0]);                        \
	sum3 = sse_mul_16x16(d0, ker[3][0]);                        \
	sum4 = sse_mul_16x16(d0, ker[4][0]);                        \
	aa = LOAD_SRC(sp + NCHAN);                                  \
	sum0 = _mm_add_epi32(sum0, sse_mul_16x16(aa, ker[0][1]));   \
	sum1 = _mm_add_epi32(sum1, sse_mul_16x16(aa, ker[1][1]));   \
	sum2 = _mm_add_epi32(sum2, sse_mul_16x16(aa, ker[2][1]));   \
	sum3 = _mm_add_epi32(sum3, sse_mul_16x16(aa, ker[3][1]));   \
	sum4 = _mm_add_epi32(sum4, sse_mul_16x16(aa, ker[4][1]));   \
	bb = LOAD_SRC(sp + 2 * NCHAN);                              \
	sum0 = _mm_add_epi32(sum0, sse_mul_16x16(bb, ker[0][2]));   \
	sum1 = _mm_add_epi32(sum1, sse_mul_16x16(bb, ker[1][2]));   \
	sum2 = _mm_add_epi32(sum2, sse_mul_16x16(bb, ker[2][2]));   \
	sum3 = _mm_add_epi32(sum3, sse_mul_16x16(bb, ker[3][2]));   \
	sum4 = _mm_add_epi32(sum4, sse_mul_16x16(bb, ker[4][2]));   \
	cc = LOAD_SRC(sp + 3 * NCHAN);                              \
	sum0 = _mm_add_epi32(sum0, sse_mul_16x16(cc, ker[0][3]));   \
	sum1 = _mm_add_epi32(sum1, sse_mul_16x16(cc, ker[1][3]));   \
	sum2 = _mm_add_epi32(sum2, sse_mul_16x16(cc, ker[2][3]));   \
	sum3 = _mm_add_epi32(sum3, sse_mul_16x16(cc, ker[3][3]));   \
	sum4 = _mm_add_epi32(sum4, sse_mul_16x16(cc, ker[4][3]));   \
	dd = LOAD_SRC(sp + 4 * NCHAN);                              \
	sum0 = _mm_add_epi32(sum0, sse_mul_16x16(dd, ker[0][4]));   \
	sum1 = _mm_add_epi32(sum1, sse_mul_16x16(dd, ker[1][4]));   \
	sum2 = _mm_add_epi32(sum2, sse_mul_16x16(dd, ker[2][4]));   \
	sum3 = _mm_add_epi32(sum3, sse_mul_16x16(dd, ker[3][4]));   \
	sum4 = _mm_add_epi32(sum4, sse_mul_16x16(dd, ker[4][4]));   \
	prev0 = buff0[i];                                           \
	prev1 = buff1[i];                                           \
	prev2 = buff2[i];                                           \
	prev3 = buff3[i];                                           \
	sum4 = _mm_add_epi32(sum4, prev0);                          \
	sum4 = _mm_add_epi32(sum4, prev1);                          \
	sum4 = _mm_add_epi32(sum4, prev2);                          \
	sum4 = _mm_add_epi32(sum4, prev3);                          \
	buff0[i] = sum0;                                            \
	buff1[i] = sum1;                                            \
	buff2[i] = sum2;                                            \
	buff3[i] = sum3;                                            \
	sp += 4;                                                    \
	res = _mm_srai_epi32(sum4, shift);                          \
	res = _mm_packs_epi32(res, res)

/* *********************************************************** */

/*
 * buffers distribution
 *
 * 0
 * 1 2
 * 3 4 5
 * 6 7 8 9    <-- previous line
 *
 * 0 2 5 9 z  <-- current line
 *
 */

#define	FUNC_CONV5x5_16nw(NCHAN)                                         \
	mlib_status                                                      \
	mlib_s_conv5x5_16nw_##NCHAN(mlib_image *dst,                     \
	    mlib_image *src, mlib_s32 *kern, mlib_s32 scalef_expon)      \
	{                                                                \
	    __m128i *pbuff, *buff_arr[20], **pbuff_arr = buff_arr;       \
	    __m128i *buff0, *buff1, *buff2, *buff3;                      \
	    GET_SRC_DST_PARAMETERS(mlib_s16);                            \
	    __m128i ker[5][5];                                           \
	    __m128i s0, d0, d1, prev0, prev1, prev2, prev3;              \
	    __m128i sum0, sum1, sum2, sum3, sum4, aa, bb, cc, dd, res;   \
	    __m128i mzero = _mm_setzero_si128();                         \
	    mlib_s32 shift, ind;                                         \
	    mlib_s32 row, wid4, i, j;                                    \
	                                                                 \
	    width -= 4;                                                  \
	    height -= 4;                                                 \
	    width *= NCHAN;                                              \
	    dl += 2 * (dll + NCHAN);                                     \
	    wid4 = (width + 3) / 4;                                      \
	    pbuff = __mlib_malloc(sizeof (__m128i) * 10 * wid4);          \
	    GET_KERN();                                                  \
	    for (i = 0; i < 10; i++) {                                   \
		buff_arr[i] = pbuff + i * wid4;                          \
	    }                                                            \
	    ind = 0;                                                     \
	    for (j = 1; j <= 4; j++) {                                   \
		buff0 = buff_arr[ind];                                   \
		buff1 = buff_arr[ind + 1];                               \
		buff2 = buff_arr[ind + 2];                               \
		buff3 = buff_arr[ind + 3];                               \
		sp = sl;                                                 \
		for (i = 0; i < wid4; i++) {                             \
		    PREP_5x5(NCHAN);                                     \
		}                                                        \
		sl += sll;                                               \
		ind += j;                                                \
	    }                                                            \
	    for (row = 0; row < height; row++) {                         \
		buff0 = pbuff_arr[0];                                    \
		buff1 = pbuff_arr[2];                                    \
		buff2 = pbuff_arr[5];                                    \
		buff3 = pbuff_arr[9];                                    \
		sp = sl;                                                 \
		dp = dl;                                                 \
		for (i = 0; i < width / 4; i++) {                        \
		    CONV_5x5(NCHAN, i);                                  \
		    _mm_storel_epi64((void *)dp, res);                   \
		    dp += 4;                                             \
		}                                                        \
		if (width & 3) {                                         \
		    __m128i mask, dd;                                    \
	                                                                 \
		    CONV_5x5(NCHAN, i);                                  \
		    mask =                                               \
			_mm_loadl_epi64((void *)(mlib_mask64_arr +       \
			2 * (width & 3)));                               \
		    dd = _mm_loadl_epi64((void *)dp);                    \
		    dd = _mm_or_si128(_mm_and_si128(mask, res),          \
			_mm_andnot_si128(mask, dd));                     \
		    _mm_storel_epi64((void *)dp, dd);                    \
		}                                                        \
		ind = (pbuff_arr == buff_arr) ? 10 : -10;                \
		pbuff_arr[ind + 0] = pbuff_arr[1];                       \
		pbuff_arr[ind + 1] = pbuff_arr[3];                       \
		pbuff_arr[ind + 2] = pbuff_arr[4];                       \
		pbuff_arr[ind + 3] = pbuff_arr[6];                       \
		pbuff_arr[ind + 4] = pbuff_arr[7];                       \
		pbuff_arr[ind + 5] = pbuff_arr[8];                       \
		pbuff_arr[ind + 6] = pbuff_arr[0];                       \
		pbuff_arr[ind + 7] = pbuff_arr[2];                       \
		pbuff_arr[ind + 8] = pbuff_arr[5];                       \
		pbuff_arr[ind + 9] = pbuff_arr[9];                       \
		pbuff_arr += ind;                                        \
		sl += sll;                                               \
		dl += dll;                                               \
	    }                                                            \
	    __mlib_free(pbuff);                                            \
	    return (MLIB_SUCCESS);                                       \
	}

/* *********************************************************** */

FUNC_CONV5x5_16nw(1)
    FUNC_CONV5x5_16nw(2)
    FUNC_CONV5x5_16nw(3)
    FUNC_CONV5x5_16nw(4)

/* *********************************************************** */
