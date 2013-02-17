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

#pragma ident   "@(#)mlib_s_ImageSConv3x3_f32nw.c	9.3	07/11/05 SMI"

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
#define	GET_KERN()                                            \
	hker0 = _mm_set1_ps((mlib_f32)hkernel[0]);              \
	hker1 = _mm_set1_ps((mlib_f32)hkernel[1]);              \
	hker2 = _mm_set1_ps((mlib_f32)hkernel[2]);              \
	vker0 = _mm_set1_ps((mlib_f32)vkernel[0]);              \
	vker1 = _mm_set1_ps((mlib_f32)vkernel[1]);              \
	vker2 = _mm_set1_ps((mlib_f32)vkernel[2]);              \

/* *********************************************************** */
#define	PREP_3x3(NCHAN)                                     \
	d0 = LOADU_SRC(0);                                      \
	sum2 = _mm_mul_ps(d0, hker0);                           \
	aa = LOADU_SRC(NCHAN);                                  \
	sum2 = _mm_add_ps(sum2, _mm_mul_ps(aa, hker1));         \
	bb = LOADU_SRC(2 * NCHAN);                              \
	sum2 = _mm_add_ps(sum2, _mm_mul_ps(bb, hker2));         \
	_mm_store_ps((mlib_f32 *)(buff0 + i), sum2);            \
	sp += 4

/* *********************************************************** */
#define	CONV_3x3(NCHAN)                                       \
	sum0 = _mm_load_ps((mlib_f32 *)(buff0 + i));            \
	sum1 = _mm_load_ps((mlib_f32 *)(buff1 + i));            \
	PREP_3x3(NCHAN);                                        \
	res = _mm_mul_ps(sum0, vker0);                          \
	res = _mm_add_ps(res, _mm_mul_ps(sum1, vker1));         \
	res = _mm_add_ps(res, _mm_mul_ps(sum2, vker2));         \

/* *********************************************************** */
#define	LOADU_SRC(off) _mm_loadu_ps((void*)(sp + off))

/* *********************************************************** */

#define	FUNC_CONV3x3_f32nw(NCHAN)                               \
	mlib_status                                            \
	mlib_s_sconv3x3_f32nw_##NCHAN(mlib_image *dst,         \
	    const mlib_image *src, const mlib_d64 *hkernel,    \
		const mlib_d64 *vkernel)                           \
	{                                                      \
	    __m128 buff_loc[3 * BUFF_LINE + 2],                \
			*pbuff = buff_loc;                             \
	    __m128 *buff0, *buff1, *buffT;                     \
	    __m128 hker0, hker1, hker2, vker0, vker1, vker2;   \
	    __m128 s0, d0, prev0, prev1,                       \
			sum0, sum1, sum2, aa, bb, res;                 \
	    __m128 mask, dd;                                   \
	    mlib_s32 row, wid4, i, j;                          \
	                                                       \
	    GET_SRC_DST_PARAMETERS(mlib_f32);                  \
	    GET_KERN();                                        \
		                                                   \
	    width -= 2;                                        \
	    height -= 2;                                       \
	    width *= NCHAN;                                    \
	    dl += dll + NCHAN;                                 \
	    wid4 = (width + 3) >> 2;                           \
		mask = _mm_loadu_ps((void *)                       \
			(mlib_mask128_arr + 2 * (width & 3)));         \
	                                                       \
	    if (wid4 > BUFF_LINE) {                            \
			pbuff = __mlib_malloc(                         \
				sizeof (__m128) * 2 * wid4);               \
	    }                                                  \
		buff0 = pbuff;                                     \
	    buff1 = buff0 + wid4;                              \
	    for (j = 0; j < 2; j++) {                          \
			sp = sl;                                       \
			for (i = 0; i < wid4; i++) {                   \
				PREP_3x3(NCHAN);                           \
			}                                              \
			sl += sll;                                     \
			buffT = buff1;                                 \
			buff1 = buff0;                                 \
			buff0 = buffT;                                 \
	    }                                                  \
	    for (row = 0; row < height; row++) {               \
			sp = sl;                                       \
			dp = dl;                                       \
			for (i = 0; i < (width >> 2); i++) {           \
				CONV_3x3(NCHAN);                           \
				_mm_storeu_ps((void *)dp, res);            \
				dp += 4;                                   \
			}                                              \
			if (width & 3) {                               \
				CONV_3x3(NCHAN);                           \
				dd = _mm_loadu_ps((void *)dp);             \
				dd = _mm_or_ps(_mm_and_ps(mask, res),      \
					_mm_andnot_ps(mask, dd));              \
				_mm_storeu_ps((void *)dp, dd);             \
			}                                              \
			buffT = buff1;                                 \
			buff1 = buff0;                                 \
			buff0 = buffT;                                 \
			sl += sll;                                     \
			dl += dll;                                     \
	    }                                                  \
	    if (pbuff != buff_loc)                             \
			__mlib_free(pbuff);                            \
	    return (MLIB_SUCCESS);                             \
	}

/* *********************************************************** */

FUNC_CONV3x3_f32nw(1)
FUNC_CONV3x3_f32nw(2)
FUNC_CONV3x3_f32nw(3)
FUNC_CONV3x3_f32nw(4)

/* *********************************************************** */
