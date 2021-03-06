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

#pragma ident   "@(#)mlib_s_ImageConv2x2_f32nw.c	9.3	07/11/05 SMI"

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

#define	GET_SRC_DST_PARAMETERS(type)                        \
	mlib_s32 height = mlib_ImageGetHeight(src);             \
	mlib_s32 width = mlib_ImageGetWidth(src);               \
	mlib_s32 sll = mlib_ImageGetStride(src) / sizeof (type);\
	mlib_s32 dll = mlib_ImageGetStride(dst) / sizeof (type);\
	type *sl = (type *) mlib_ImageGetData(src);             \
	type *dl = (type *) mlib_ImageGetData(dst);             \
	type *sp, *dp

/* *********************************************************** */
#define	GET_KERN()                                          \
	ker1 = _mm_set1_ps((mlib_f32)kern[0]);                  \
	ker2 = _mm_set1_ps((mlib_f32)kern[1]);                  \
	ker3 = _mm_set1_ps((mlib_f32)kern[2]);                  \
	ker4 = _mm_set1_ps((mlib_f32)kern[3]);                  \

/* *********************************************************** */
#define	PREP_2x2(NCHAN)                                     \
	d0 = _mm_loadu_ps((void *)sp);                          \
	sum0 = _mm_mul_ps(d0, ker1);                            \
	aa = _mm_loadu_ps((void *)(sp + NCHAN));                \
	sum0 = _mm_add_ps(sum0, _mm_mul_ps(aa, ker2));          \
	_mm_store_ps((mlib_f32 *)(buff0 + i), sum0);            \
	sp += 4;

/* *********************************************************** */
#define	CONV_2x2(NCHAN, i)                                      \
	prev0 = _mm_load_ps((mlib_f32 *)(buff0 + i));           \
	d0 = _mm_loadu_ps((void *)sp);                          \
	sum0 = _mm_mul_ps(d0, ker1);                            \
	sum1 = _mm_mul_ps(d0, ker3);                            \
	aa = _mm_loadu_ps((void *)(sp + NCHAN));                \
	sum0 = _mm_add_ps(sum0, _mm_mul_ps(aa, ker2));          \
	sum1 = _mm_add_ps(sum1, _mm_mul_ps(aa, ker4));          \
	sum1 = _mm_add_ps(sum1, prev0);                         \
	_mm_store_ps((mlib_f32 *)(buff0 + i), sum0);            \
	sp += 4;                                                \
	res = sum1                                              \

/* *********************************************************** */

#define	FUNC_CONV2x2_f32nw(NCHAN)                          \
	mlib_status                                           \
	mlib_s_conv2x2_f32nw_##NCHAN(                         \
		mlib_image *dst, const mlib_image *src,           \
		const mlib_d64 *kern)                             \
	{                                                     \
	    __m128 buff_loc[BUFF_LINE], *pbuff = buff_loc;    \
	    __m128 *buff0, *buff1, *buff2, *buffT;            \
	    __m128 ker1, ker2, ker3, ker4;                    \
	    __m128 s0, d0, d1, prev0, prev1,                  \
			sum0, sum1, aa, bb, res, mask;                \
	    __m128 mzero = _mm_setzero_ps();                  \
	    mlib_s32 shift;                                   \
	    mlib_s32 row, wid4, i, j;                         \
	                                                      \
	    GET_SRC_DST_PARAMETERS(mlib_f32);                 \
	    GET_KERN();                                       \
		                                                  \
	    width -= 1;                                       \
	    height -= 1;                                      \
	    width *= NCHAN;                                   \
	    wid4 = (width + 3) / 4;                           \
		mask = _mm_load_ps((void *)                       \
			(mlib_mask128_arr + 2 * (width & 3)));        \
                                                          \
	    if (wid4 > BUFF_LINE) {                           \
			pbuff = __mlib_malloc(sizeof (__m128) * wid4);\
	    }                                                 \
	    buff0 = pbuff;                                    \
		sp = sl;                                          \
		for (i = 0; i < wid4; i++) {                      \
		    PREP_2x2(NCHAN);                              \
		}                                                 \
		sl += sll;                                        \
	    for (row = 0; row < height; row++) {              \
			sp = sl;                                      \
			dp = dl;                                      \
			for (i = 0; i < (width >> 2); i++) {          \
				CONV_2x2(NCHAN, i);                       \
				_mm_storeu_ps((void *)dp, res);           \
				dp += 4;                                  \
			}                                             \
			if (width & 3) {                              \
				__m128 dd;                                \
	                                                      \
				CONV_2x2(NCHAN, i);                       \
				dd = _mm_loadu_ps((void *)dp);            \
				dd = _mm_or_ps(_mm_and_ps(mask, res),     \
					_mm_andnot_ps(mask, dd));             \
				_mm_storeu_ps((void *)dp, dd);            \
			}                                             \
			sl += sll;                                    \
			dl += dll;                                    \
	    }                                                 \
	    if (pbuff != buff_loc)                            \
			__mlib_free(pbuff);                           \
	    return (MLIB_SUCCESS);                            \
	}

/* *********************************************************** */

FUNC_CONV2x2_f32nw(1)
FUNC_CONV2x2_f32nw(2)
FUNC_CONV2x2_f32nw(3)
FUNC_CONV2x2_f32nw(4)

/* *********************************************************** */
