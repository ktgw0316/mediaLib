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

#pragma ident   "@(#)mlib_s_ImageConv4x4_d64nw.c	9.3	07/11/05 SMI"

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

#define	GET_SRC_DST_PARAMETERS(type)                           \
	mlib_s32 height = mlib_ImageGetHeight(src);              \
	mlib_s32 width = mlib_ImageGetWidth(src);                \
	mlib_s32 sll = mlib_ImageGetStride(src) / sizeof (type); \
	mlib_s32 dll = mlib_ImageGetStride(dst) / sizeof (type); \
	type *sl = (type *) mlib_ImageGetData(src);              \
	type *dl = (type *) mlib_ImageGetData(dst);              \
	type *sp, *dp

/* *********************************************************** */
#define	GET_KERN()                                       \
	k1 = _mm_set1_pd((mlib_d64)kern[0]);                 \
    k2 = _mm_set1_pd((mlib_d64)kern[1]);                 \
    k3 = _mm_set1_pd((mlib_d64)kern[2]);                 \
    k4 = _mm_set1_pd((mlib_d64)kern[3]);                 \
    k5 = _mm_set1_pd((mlib_d64)kern[4]);                 \
    k6 = _mm_set1_pd((mlib_d64)kern[5]);                 \
    k7 = _mm_set1_pd((mlib_d64)kern[6]);                 \
    k8 = _mm_set1_pd((mlib_d64)kern[7]);                 \
    k9 = _mm_set1_pd((mlib_d64)kern[8]);                 \
	k10 = _mm_set1_pd((mlib_d64)kern[9]);                \
    k11 = _mm_set1_pd((mlib_d64)kern[10]);               \
    k12 = _mm_set1_pd((mlib_d64)kern[11]);               \
    k13 = _mm_set1_pd((mlib_d64)kern[12]);               \
    k14 = _mm_set1_pd((mlib_d64)kern[13]);               \
    k15 = _mm_set1_pd((mlib_d64)kern[14]);               \
    k16 = _mm_set1_pd((mlib_d64)kern[15]);               \

/* *********************************************************** */

#define	LOAD_SRC(ptr)	_mm_loadu_pd((void*)(ptr))

/* *********************************************************** */

#define	PREP_4x4(NCHAN)                                  \
	d0 = LOAD_SRC(sp);                                   \
	sum0 = _mm_mul_pd(d0, k1);                           \
	sum1 = _mm_mul_pd(d0, k5);                           \
	sum2 = _mm_mul_pd(d0, k9);                           \
	aa = LOAD_SRC(sp + NCHAN);                           \
	sum0 = _mm_add_pd(sum0, _mm_mul_pd(aa, k2));         \
	sum1 = _mm_add_pd(sum1, _mm_mul_pd(aa, k6));         \
	sum2 = _mm_add_pd(sum2, _mm_mul_pd(aa, k10));        \
	bb = LOAD_SRC(sp + 2 * NCHAN);                       \
	sum0 = _mm_add_pd(sum0, _mm_mul_pd(bb, k3));         \
	sum1 = _mm_add_pd(sum1, _mm_mul_pd(bb, k7));         \
	sum2 = _mm_add_pd(sum2, _mm_mul_pd(bb, k11));        \
	cc = LOAD_SRC(sp + 3 * NCHAN);                       \
	sum0 = _mm_add_pd(sum0, _mm_mul_pd(cc, k4));         \
	sum1 = _mm_add_pd(sum1, _mm_mul_pd(cc, k8));         \
	sum2 = _mm_add_pd(sum2, _mm_mul_pd(cc, k12));        \
	_mm_store_pd((mlib_d64 *)(buff0 + i), sum0);         \
	_mm_store_pd((mlib_d64 *)(buff1 + i), sum1);         \
	_mm_store_pd((mlib_d64 *)(buff2 + i), sum2);         \
	sp += 2

/* *********************************************************** */

#define	CONV_4x4(NCHAN, i)                                   \
	d0 = LOAD_SRC(sp);                                   \
	sum0 = _mm_mul_pd(d0, k1);                           \
	sum1 = _mm_mul_pd(d0, k5);                           \
	sum2 = _mm_mul_pd(d0, k9);                           \
	sum3 = _mm_mul_pd(d0, k13);                          \
	aa = LOAD_SRC(sp + NCHAN);                           \
	sum0 = _mm_add_pd(sum0, _mm_mul_pd(aa, k2));         \
	sum1 = _mm_add_pd(sum1, _mm_mul_pd(aa, k6));         \
	sum2 = _mm_add_pd(sum2, _mm_mul_pd(aa, k10));        \
	sum3 = _mm_add_pd(sum3, _mm_mul_pd(aa, k14));        \
	bb = LOAD_SRC(sp + 2 * NCHAN);                       \
	sum0 = _mm_add_pd(sum0, _mm_mul_pd(bb, k3));         \
	sum1 = _mm_add_pd(sum1, _mm_mul_pd(bb, k7));         \
	sum2 = _mm_add_pd(sum2, _mm_mul_pd(bb, k11));        \
	sum3 = _mm_add_pd(sum3, _mm_mul_pd(bb, k15));        \
	cc = LOAD_SRC(sp + 3 * NCHAN);                       \
	sum0 = _mm_add_pd(sum0, _mm_mul_pd(cc, k4));         \
	sum1 = _mm_add_pd(sum1, _mm_mul_pd(cc, k8));         \
	sum2 = _mm_add_pd(sum2, _mm_mul_pd(cc, k12));        \
	sum3 = _mm_add_pd(sum3, _mm_mul_pd(cc, k16));        \
	prev0 = _mm_load_pd((mlib_d64 *)(buff0 + i));        \
	prev1 = _mm_load_pd((mlib_d64 *)(buff1 + i));        \
	prev2 = _mm_load_pd((mlib_d64 *)(buff2 + i));        \
	sum3 = _mm_add_pd(sum3, prev0);                      \
	sum3 = _mm_add_pd(sum3, prev1);                      \
	sum3 = _mm_add_pd(sum3, prev2);                      \
	_mm_store_pd((mlib_d64 *)(buff0 + i), sum0);         \
	_mm_store_pd((mlib_d64 *)(buff1 + i), sum1);         \
	_mm_store_pd((mlib_d64 *)(buff2 + i), sum2);         \
	res = sum3;                                          \
	sp += 2;                                             \

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

#define	FUNC_CONV4x4_d64nw(NCHAN)                             \
	mlib_status                                               \
	mlib_s_conv4x4_d64nw_##NCHAN(mlib_image *dst,             \
	    const mlib_image *src, const mlib_d64 *kern)          \
	{                                                         \
	    __m128d *pbuff, *buff_arr[12], **pbuff_arr = buff_arr;\
	    __m128d *buff0, *buff1, *buff2;                       \
	    __m128d k1, k2, k3, k4, k5, k6, k7, k8;               \
	    __m128d k9, k10, k11, k12, k13, k14, k15, k16;        \
	    __m128d s0, d0, d1, prev0, prev1, prev2;              \
	    __m128d sum0, sum1, sum2, sum3, aa, bb, cc, res;      \
	    mlib_s32 ind;                                         \
	    mlib_s32 row, wid4, i, j;                             \
	                                                          \
	    GET_SRC_DST_PARAMETERS(mlib_d64);                     \
	    GET_KERN();                                           \
		                                                      \
	    width -= 3;                                           \
	    height -= 3;                                          \
	    width *= NCHAN;                                       \
	    dl += (dll + NCHAN);                                  \
	    wid4 = (width + 1) >> 1;                              \
	    pbuff = __mlib_malloc(                                \
			sizeof (__m128d) * 6 * wid4);                     \
	    for (i = 0; i < 6; i++) {                             \
			buff_arr[i] = pbuff + i * wid4;                   \
	    }                                                     \
	    ind = 0;                                              \
	    for (j = 1; j <= 3; j++) {                            \
			buff0 = buff_arr[ind];                            \
			buff1 = buff_arr[ind + 1];                        \
			buff2 = buff_arr[ind + 2];                        \
			sp = sl;                                          \
			for (i = 0; i < wid4; i++) {                      \
				PREP_4x4(NCHAN);                              \
			}                                                 \
			sl += sll;                                        \
			ind += j;                                         \
	    }                                                     \
	    for (row = 0; row < height; row++) {                  \
			buff0 = pbuff_arr[0];                             \
			buff1 = pbuff_arr[2];                             \
			buff2 = pbuff_arr[5];                             \
			sp = sl;                                          \
			dp = dl;                                          \
			for (i = 0; i < (width >> 1); i++) {              \
				CONV_4x4(NCHAN, i);                           \
				_mm_storeu_pd((void *)dp, res);               \
				dp += 2;                                      \
			}                                                 \
			if (width & 1) {                                  \
				CONV_4x4(NCHAN, i);                           \
				_mm_storel_pd((void *)dp, res);               \
			}                                                 \
			ind = (pbuff_arr == buff_arr) ? 6 : -6;           \
			pbuff_arr[ind + 0] = pbuff_arr[1];                \
			pbuff_arr[ind + 1] = pbuff_arr[3];                \
			pbuff_arr[ind + 2] = pbuff_arr[4];                \
			pbuff_arr[ind + 3] = pbuff_arr[0];                \
			pbuff_arr[ind + 4] = pbuff_arr[2];                \
			pbuff_arr[ind + 5] = pbuff_arr[5];                \
			pbuff_arr += ind;                                 \
			sl += sll;                                        \
			dl += dll;                                        \
	    }                                                     \
	    mlib_free(pbuff);                                     \
	    return (MLIB_SUCCESS);                                \
	}

/* *********************************************************** */

FUNC_CONV4x4_d64nw(1)
FUNC_CONV4x4_d64nw(2)
FUNC_CONV4x4_d64nw(3)
FUNC_CONV4x4_d64nw(4)

/* *********************************************************** */
