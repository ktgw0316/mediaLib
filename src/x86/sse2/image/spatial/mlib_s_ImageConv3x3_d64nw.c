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

#pragma ident   "@(#)mlib_s_ImageConv3x3_d64nw.c	9.3	07/11/05 SMI"

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

#define	GET_SRC_DST_PARAMETERS(type)                            \
	mlib_s32 height = mlib_ImageGetHeight(src);              \
	mlib_s32 width = mlib_ImageGetWidth(src);                \
	mlib_s32 sll = mlib_ImageGetStride(src) / sizeof (type); \
	mlib_s32 dll = mlib_ImageGetStride(dst) / sizeof (type); \
	type *sl = (type *) mlib_ImageGetData(src);              \
	type *dl = (type *) mlib_ImageGetData(dst);              \
	type *sp, *dp

/* *********************************************************** */
#define	GET_KERN()                                              \
	ker1 = _mm_set1_pd((mlib_d64)kern[0]);        \
	ker2 = _mm_set1_pd((mlib_d64)kern[1]);        \
	ker3 = _mm_set1_pd((mlib_d64)kern[2]);        \
	ker4 = _mm_set1_pd((mlib_d64)kern[3]);        \
	ker5 = _mm_set1_pd((mlib_d64)kern[4]);        \
	ker6 = _mm_set1_pd((mlib_d64)kern[5]);        \
	ker7 = _mm_set1_pd((mlib_d64)kern[6]);        \
	ker8 = _mm_set1_pd((mlib_d64)kern[7]);        \
	ker9 = _mm_set1_pd((mlib_d64)kern[8]);        \

/* *********************************************************** */
#define	PREP_3x3(NCHAN)                                   \
	d0 = _mm_loadu_pd((void *)sp);                    \
	sum0 = _mm_mul_pd(d0, ker1);                      \
	sum1 = _mm_mul_pd(d0, ker4);                      \
	aa = _mm_loadu_pd((void *)(sp + NCHAN));          \
	sum0 = _mm_add_pd(sum0, _mm_mul_pd(aa, ker2));    \
	sum1 = _mm_add_pd(sum1, _mm_mul_pd(aa, ker5));    \
	bb = _mm_loadu_pd((void *)(sp + 2 * NCHAN));      \
	sum0 = _mm_add_pd(sum0, _mm_mul_pd(bb, ker3));    \
	sum1 = _mm_add_pd(sum1, _mm_mul_pd(bb, ker6));    \
	_mm_store_pd((mlib_d64 *)(buff1 + i), sum0);      \
	_mm_store_pd((mlib_d64 *)(buff2 + i), sum1);      \
	sp += 2;

/* *********************************************************** */
#define	CONV_3x3(NCHAN, l)                             \
	prev0 = _mm_load_pd((mlib_d64 *)(buff0 + l));     \
	prev1 = _mm_load_pd((mlib_d64 *)(buff2 + l));     \
	d0 = _mm_loadu_pd((void *)sp);                    \
	sum0 = _mm_mul_pd(d0, ker1);                      \
	sum1 = _mm_mul_pd(d0, ker4);                      \
	sum2 = _mm_mul_pd(d0, ker7);                      \
	aa = _mm_loadu_pd((void *)(sp + NCHAN));          \
	sum0 = _mm_add_pd(sum0, _mm_mul_pd(aa, ker2));    \
	sum1 = _mm_add_pd(sum1, _mm_mul_pd(aa, ker5));    \
	sum2 = _mm_add_pd(sum2, _mm_mul_pd(aa, ker8));    \
	bb = _mm_loadu_pd((void *)(sp + 2 * NCHAN));      \
	sum0 = _mm_add_pd(sum0, _mm_mul_pd(bb, ker3));    \
	sum1 = _mm_add_pd(sum1, _mm_mul_pd(bb, ker6));    \
	sum2 = _mm_add_pd(sum2, _mm_mul_pd(bb, ker9));    \
	sum2 = _mm_add_pd(sum2, prev0);                   \
	sum2 = _mm_add_pd(sum2, prev1);                   \
	_mm_store_pd((mlib_d64 *)(buff0 + l), sum0);      \
	_mm_store_pd((mlib_d64 *)(buff2 + l), sum1);      \
	res = sum2;                                       \
	sp += 2

/* *********************************************************** */

#define	FUNC_CONV3x3_d64nw(NCHAN)                                      \
	mlib_status                                               \
	mlib_s_conv3x3_d64nw_##NCHAN(mlib_image *dst,             \
	    const mlib_image *src, const mlib_d64 *kern)          \
{                                                             \
	__m128d buff_loc[3 * BUFF_LINE + 2], *pbuff = buff_loc;   \
	__m128d *buff0, *buff1, *buff2, *buffT;                   \
	__m128d ker1, ker2, ker3, ker4, ker5,                     \
		ker6, ker7, ker8, ker9;                               \
	__m128d s0, d0, d1, prev0, prev1,                         \
		sum0, sum1, sum2, aa, bb, res;                        \
	mlib_s32 row, wid2, i, j, tail;                           \
	                                                          \
	GET_SRC_DST_PARAMETERS(mlib_d64);                         \
	GET_KERN();                                               \
		                                                      \
	width -= 2;                                               \
	height -= 2;                                              \
	width *= NCHAN;                                           \
	dl += dll + NCHAN;                                        \
	wid2 = (width + 1) >> 1;                                  \
	tail = width & 1;                                         \
	if (wid2 > BUFF_LINE) {                                   \
		pbuff = __mlib_malloc(sizeof (__m128d) *              \
			3 * wid2);                                        \
	}                                                         \
	buff0 = pbuff;                                            \
	buff1 = buff0 + wid2;                                     \
	buff2 = buff1 + wid2;                                     \
	for (j = 0; j < 2; j++) {                                 \
		sp = sl;                                              \
		for (i = 0; i < wid2; i++) {                          \
			PREP_3x3(NCHAN);                                  \
		}                                                     \
		sl += sll;                                            \
		if (j == 0) {                                         \
			buffT = buff1;                                    \
		    buff1 = buff0;                                    \
		    buff0 = buffT;                                    \
		}                                                     \
	}                                                         \
	for (row = 0; row < height; row++) {                      \
		sp = sl;                                              \
		dp = dl;                                              \
		for (i = 0; i < (width >> 1); i++) {                  \
			CONV_3x3(NCHAN, i);                               \
			_mm_storeu_pd((void *)dp, res);                   \
			dp += 2;                                          \
		}                                                     \
		if (tail) {                                           \
			CONV_3x3(NCHAN, i);                               \
			_mm_storel_pd((void *)dp, res);                   \
		}                                                     \
		buffT = buff1;                                        \
		buff1 = buff0;                                        \
		buff0 = buffT;                                        \
		sl += sll;                                            \
		dl += dll;                                            \
	}                                                         \
	if (pbuff != buff_loc)                                    \
		__mlib_free(pbuff);                                   \
	return (MLIB_SUCCESS);                                    \
}

/* *********************************************************** */

FUNC_CONV3x3_d64nw(1)
FUNC_CONV3x3_d64nw(2)
FUNC_CONV3x3_d64nw(3)
FUNC_CONV3x3_d64nw(4)

/* *********************************************************** */
