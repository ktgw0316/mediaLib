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

#pragma ident   "@(#)mlib_s_ImageSConv5x5_f32nw.c	9.3	07/11/05 SMI"

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
#define	GET_SRC_DST_PARAMETERS(type)                           \
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
	vker0 = _mm_set1_ps((mlib_f32)vkernel[0]);               \
	vker1 = _mm_set1_ps((mlib_f32)vkernel[1]);               \
	vker2 = _mm_set1_ps((mlib_f32)vkernel[2]);               \
	vker3 = _mm_set1_ps((mlib_f32)vkernel[3]);               \
	vker4 = _mm_set1_ps((mlib_f32)vkernel[4]);               \

/* *********************************************************** */
#define	PREP_5x5(NCHAN)                                         \
	d0 = LOAD_SRC(sp);                                       \
	sum4 = _mm_mul_ps(d0, hker0);                            \
	aa = LOAD_SRC(sp + NCHAN);                               \
	sum4 = _mm_add_ps(sum4, _mm_mul_ps(aa, hker1));          \
	bb = LOAD_SRC(sp + 2 * NCHAN);                           \
	sum4 = _mm_add_ps(sum4, _mm_mul_ps(bb, hker2));          \
	cc = LOAD_SRC(sp + 3 * NCHAN);                           \
	sum4 = _mm_add_ps(sum4, _mm_mul_ps(cc, hker3));          \
	dd = LOAD_SRC(sp + 4 * NCHAN);                           \
	sum4 = _mm_add_ps(sum4, _mm_mul_ps(dd, hker4));          \
	_mm_store_ps((mlib_f32 *)(buff0 + i), sum4);             \
	sp += 4

/* *********************************************************** */
#define	CONV_5x5(NCHAN)                                         \
	sum0 = _mm_load_ps((mlib_f32 *)(buff0 + i));             \
	sum1 = _mm_load_ps((mlib_f32 *)(buff1 + i));             \
	sum2 = _mm_load_ps((mlib_f32 *)(buff2 + i));             \
	sum3 = _mm_load_ps((mlib_f32 *)(buff3 + i));             \
	PREP_5x5(NCHAN);                                         \
	res = _mm_mul_ps(sum0, vker0);                           \
	res = _mm_add_ps(res, _mm_mul_ps(sum1, vker1));          \
	res = _mm_add_ps(res, _mm_mul_ps(sum2, vker2));          \
	res = _mm_add_ps(res, _mm_mul_ps(sum3, vker3));          \
	res = _mm_add_ps(res, _mm_mul_ps(sum4, vker4));          \

/* *********************************************************** */
#define	LOAD_SRC(sp)	_mm_loadu_ps((void*)(sp))

/* *********************************************************** */

#define	FUNC_CONV5x5_f32nw(NCHAN)                                        \
	mlib_status                                            \
	mlib_s_sconv5x5_f32nw_##NCHAN(mlib_image *dst,         \
	    const mlib_image *src, const mlib_d64 *hkernel,    \
		const mlib_d64 *vkernel)                           \
	{                                                      \
	    __m128 buff_loc[4 * BUFF_LINE + 2],                \
			*pbuff = buff_loc;                             \
	    __m128 *buff0, *buff1, *buff2, *buff3, *buffT;     \
	    __m128 hker0, hker1, hker2, hker3, hker4,          \
			vker0, vker1, vker2, vker3, vker4;             \
	    __m128 s0, d0, d1, prev0, prev1,                   \
			sum0, sum1, sum2, sum3, sum4;                  \
	    __m128 aa, bb, cc, dd, res, mask;                  \
	    mlib_s32 row, wid4, i, j;                          \
	                                                       \
	    GET_SRC_DST_PARAMETERS(mlib_f32);                  \
	    GET_KERN();                                        \
		                                                   \
	    width -= 4;                                        \
	    height -= 4;                                       \
	    width *= NCHAN;                                    \
	    dl += 2 * (dll + NCHAN);                           \
	    wid4 = (width + 3) >> 2;                           \
		mask = _mm_loadu_ps((void *)                       \
			(mlib_mask128_arr + 2 * (width & 3)));         \
	    if (wid4 > BUFF_LINE) {                            \
			pbuff = __mlib_malloc(                         \
				sizeof (__m128) * 4 * wid4);               \
	    }                                                  \
	    buff0 = pbuff;                                     \
	    buff1 = buff0 + wid4;                              \
	    buff2 = buff1 + wid4;                              \
	    buff3 = buff2 + wid4;                              \
	    for (j = 0; j < 4; j++) {                          \
			sp = sl;                                       \
			for (i = 0; i < wid4; i++) {                   \
				PREP_5x5(NCHAN);                           \
			}                                              \
			sl += sll;                                     \
			buffT = buff0;                                 \
			buff0 = buff1;                                 \
			buff1 = buff2;                                 \
			buff2 = buff3;                                 \
			buff3 = buffT;                                 \
	    }                                                  \
	    for (row = 0; row < height; row++) {               \
			sp = sl;                                       \
			dp = dl;                                       \
			for (i = 0; i < (width >> 2); i++) {           \
				CONV_5x5(NCHAN);                           \
				_mm_storeu_ps((void *)dp, res);            \
				dp += 4;                                   \
			}                                              \
			if (width & 3) {                               \
				CONV_5x5(NCHAN);                           \
				dd = _mm_loadu_ps((void *)dp);             \
				dd = _mm_or_ps(_mm_and_ps(mask, res),      \
					_mm_andnot_ps(mask, dd));              \
				_mm_storeu_ps((void *)dp, dd);             \
			}                                              \
			buffT = buff0;                                 \
			buff0 = buff1;                                 \
			buff1 = buff2;                                 \
			buff2 = buff3;                                 \
			buff3 = buffT;                                 \
			sl += sll;                                     \
			dl += dll;                                     \
	    }                                                  \
	    if (pbuff != buff_loc)                             \
			__mlib_free(pbuff);                            \
	    return (MLIB_SUCCESS);                             \
	}

/* *********************************************************** */

FUNC_CONV5x5_f32nw(1)
FUNC_CONV5x5_f32nw(2)
FUNC_CONV5x5_f32nw(3)
FUNC_CONV5x5_f32nw(4)

/* *********************************************************** */
