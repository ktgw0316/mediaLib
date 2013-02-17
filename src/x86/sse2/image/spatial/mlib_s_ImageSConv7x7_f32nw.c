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

#pragma ident   "@(#)mlib_s_ImageSConv7x7_f32nw.c	9.3	07/11/05 SMI"

/*
 *      A 2-D convolution.
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

#define	GET_SRC_DST_PARAMETERS(type)                         \
	mlib_s32 height = mlib_ImageGetHeight(src);              \
	mlib_s32 width = mlib_ImageGetWidth(src);                \
	mlib_s32 sll = mlib_ImageGetStride(src) / sizeof (type); \
	mlib_s32 dll = mlib_ImageGetStride(dst) / sizeof (type); \
	type *sl = (type *) mlib_ImageGetData(src);              \
	type *dl = (type *) mlib_ImageGetData(dst);              \
	type *sp, *dp

/* *********************************************************** */
#define	GET_KERN()                                              \
	hker0 = _mm_set1_ps((mlib_f32)hkernel[0]);               \
	hker1 = _mm_set1_ps((mlib_f32)hkernel[1]);               \
	hker2 = _mm_set1_ps((mlib_f32)hkernel[2]);               \
	hker3 = _mm_set1_ps((mlib_f32)hkernel[3]);               \
	hker4 = _mm_set1_ps((mlib_f32)hkernel[4]);               \
	hker5 = _mm_set1_ps((mlib_f32)hkernel[5]);               \
	hker6 = _mm_set1_ps((mlib_f32)hkernel[6]);               \
	vker0 = _mm_set1_ps((mlib_f32)vkernel[0]);               \
	vker1 = _mm_set1_ps((mlib_f32)vkernel[1]);               \
	vker2 = _mm_set1_ps((mlib_f32)vkernel[2]);               \
	vker3 = _mm_set1_ps((mlib_f32)vkernel[3]);               \
	vker4 = _mm_set1_ps((mlib_f32)vkernel[4]);               \
	vker5 = _mm_set1_ps((mlib_f32)vkernel[5]);               \
	vker6 = _mm_set1_ps((mlib_f32)vkernel[6]);               \

/* *********************************************************** */
#define	PREP_7x7(NCHAN)                                        \
	s0 = LOAD_SRC(0);                                        \
	sum6 = _mm_mul_ps(s0, hker0);                            \
	s1 = LOAD_SRC(NCHAN);                                    \
	sum6 = _mm_add_ps(sum6, _mm_mul_ps(s1, hker1));          \
	s2 = LOAD_SRC(2 * NCHAN);                                \
	sum6 = _mm_add_ps(sum6, _mm_mul_ps(s2, hker2));          \
	s3 = LOAD_SRC(3 * NCHAN);                                \
	sum6 = _mm_add_ps(sum6, _mm_mul_ps(s3, hker3));          \
	s4 = LOAD_SRC(4 * NCHAN);                                \
	sum6 = _mm_add_ps(sum6, _mm_mul_ps(s4, hker4));          \
	s5 = LOAD_SRC(5 * NCHAN);                                \
	sum6 = _mm_add_ps(sum6, _mm_mul_ps(s5, hker5));          \
	s6 = LOAD_SRC(6 * NCHAN);                                \
	sum6 = _mm_add_ps(sum6, _mm_mul_ps(s6, hker6));          \
	_mm_store_ps((mlib_f32 *)(buff0 + i), sum6);             \
	sp += 4

/* *********************************************************** */
#define	CONV_7x7(NCHAN)                                           \
	sum0 = _mm_load_ps((mlib_f32 *)(buff0 + i));             \
	sum1 = _mm_load_ps((mlib_f32 *)(buff1 + i));             \
	sum2 = _mm_load_ps((mlib_f32 *)(buff2 + i));             \
	sum3 = _mm_load_ps((mlib_f32 *)(buff3 + i));             \
	sum4 = _mm_load_ps((mlib_f32 *)(buff4 + i));             \
	sum5 = _mm_load_ps((mlib_f32 *)(buff5 + i));             \
	PREP_7x7(NCHAN);                                         \
	res = _mm_mul_ps(sum0, vker0);                           \
	res = _mm_add_ps(res, _mm_mul_ps(sum1, vker1));          \
	res = _mm_add_ps(res, _mm_mul_ps(sum2, vker2));          \
	res = _mm_add_ps(res, _mm_mul_ps(sum3, vker3));          \
	res = _mm_add_ps(res, _mm_mul_ps(sum4, vker4));          \
	res = _mm_add_ps(res, _mm_mul_ps(sum5, vker5));          \
	res = _mm_add_ps(res, _mm_mul_ps(sum6, vker6));          \

/* *********************************************************** */
#define	LOAD_SRC(off)                                              \
	_mm_loadu_ps((void*)(sp + off))

/* *********************************************************** */

#define	KER  7

/* *********************************************************** */

#define	FUNC_CONV7x7_f32nw(NCHAN)                               \
	mlib_status                                            \
	mlib_s_sconv7x7_f32nw_##NCHAN(mlib_image *dst,         \
	    const mlib_image *src, const mlib_d64 *hkernel,    \
		const mlib_d64 *vkernel)                           \
	{                                                      \
	    __m128 buff_loc[(KER - 1) * BUFF_LINE + 2],        \
			*pbuff = buff_loc;                             \
	    __m128 *buff0, *buff1, *buff2, *buff3,             \
			*buff4, *buff5, *buffT;                        \
	    __m128 hker0, hker1, hker2, hker3,                 \
			hker4, hker5, hker6;                           \
	    __m128 vker0, vker1, vker2, vker3,                 \
			vker4, vker5, vker6;                           \
	    __m128 s0, s1, s2, s3, s4, s5, s6,                 \
			d0, d1, prev0, prev1;                          \
	    __m128 sum0, sum1, sum2, sum3,                     \
			sum4, sum5, sum6;                              \
	    __m128 dd, res, mask;                              \
	    mlib_s32 row, wid4, i, j;                          \
                                                           \
	    GET_SRC_DST_PARAMETERS(mlib_f32);                  \
	    GET_KERN();                                        \
                                                           \
	    width -= (KER - 1);                                \
	    height -= (KER - 1);                               \
	    width *= NCHAN;                                    \
	    dl += (KER / 2) * (dll + NCHAN);                   \
	    wid4 = (width + 3) >> 2;                           \
		mask = _mm_loadu_ps((void *)                       \
			(mlib_mask128_arr + 2 * (width & 3)));         \
		                                                   \
	    if (wid4 > BUFF_LINE) {                            \
			pbuff = __mlib_malloc(                         \
				sizeof (__m128) * ((KER - 1) * wid4));     \
	    }                                                  \
	    buff0 = pbuff;                                     \
	    buff1 = buff0 + wid4;                              \
	    buff2 = buff1 + wid4;                              \
	    buff3 = buff2 + wid4;                              \
	    buff4 = buff3 + wid4;                              \
	    buff5 = buff4 + wid4;                              \
	    for (j = 0; j < (KER - 1); j++) {                  \
			sp = sl;                                       \
			for (i = 0; i < wid4; i++) {                   \
				PREP_7x7(NCHAN);                           \
			}                                              \
			sl += sll;                                     \
			buffT = buff0;                                 \
			buff0 = buff1;                                 \
			buff1 = buff2;                                 \
			buff2 = buff3;                                 \
			buff3 = buff4;                                 \
			buff4 = buff5;                                 \
			buff5 = buffT;                                 \
	    }                                                  \
	    for (row = 0; row < height; row++) {               \
			sp = sl;                                       \
			dp = dl;                                       \
			for (i = 0; i < (width >> 2); i++) {           \
				CONV_7x7(NCHAN);                           \
				_mm_storeu_ps((void *)dp, res);            \
				dp += 4;                                   \
			}                                              \
			if (width & 3) {                               \
				CONV_7x7(NCHAN);                           \
				dd = _mm_loadu_ps((void *)dp);             \
				dd = _mm_or_ps(_mm_and_ps(mask, res),      \
					_mm_andnot_ps(mask, dd));              \
				_mm_storeu_ps((void *)dp, dd);             \
			}                                              \
			buffT = buff0;                                 \
			buff0 = buff1;                                 \
			buff1 = buff2;                                 \
			buff2 = buff3;                                 \
			buff3 = buff4;                                 \
			buff4 = buff5;                                 \
			buff5 = buffT;                                 \
			sl += sll;                                     \
			dl += dll;                                     \
	    }                                                  \
	    if (pbuff != buff_loc)                             \
			__mlib_free(pbuff);                            \
	    return (MLIB_SUCCESS);                             \
	}

/* *********************************************************** */

FUNC_CONV7x7_f32nw(1)
FUNC_CONV7x7_f32nw(2)
FUNC_CONV7x7_f32nw(3)
FUNC_CONV7x7_f32nw(4)

/* *********************************************************** */
