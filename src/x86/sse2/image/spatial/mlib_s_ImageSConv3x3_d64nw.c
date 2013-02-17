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

#pragma ident   "@(#)mlib_s_ImageSConv3x3_d64nw.c	9.3	07/11/05 SMI"

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
	hker0 = _mm_set1_pd((mlib_d64)hkernel[0]);              \
	hker1 = _mm_set1_pd((mlib_d64)hkernel[1]);              \
	hker2 = _mm_set1_pd((mlib_d64)hkernel[2]);              \
	vker0 = _mm_set1_pd((mlib_d64)vkernel[0]);              \
	vker1 = _mm_set1_pd((mlib_d64)vkernel[1]);              \
	vker2 = _mm_set1_pd((mlib_d64)vkernel[2]);              \

/* *********************************************************** */
#define	PREP_3x3(NCHAN)                                     \
	d0 = LOADU_SRC(0);                                      \
	sum2 = _mm_mul_pd(d0, hker0);                           \
	aa = LOADU_SRC(NCHAN);                                  \
	sum2 = _mm_add_pd(sum2, _mm_mul_pd(aa, hker1));         \
	bb = LOADU_SRC(2 * NCHAN);                              \
	sum2 = _mm_add_pd(sum2, _mm_mul_pd(bb, hker2));         \
	_mm_store_pd((mlib_d64 *)(buff0 + i), sum2);            \
	sp += 2

/* *********************************************************** */
#define	CONV_3x3(NCHAN)                                       \
	sum0 = _mm_load_pd((mlib_d64 *)(buff0 + i));            \
	sum1 = _mm_load_pd((mlib_d64 *)(buff1 + i));            \
	PREP_3x3(NCHAN);                                        \
	res = _mm_mul_pd(sum0, vker0);                          \
	res = _mm_add_pd(res, _mm_mul_pd(sum1, vker1));         \
	res = _mm_add_pd(res, _mm_mul_pd(sum2, vker2));         \

/* *********************************************************** */
#define	LOADU_SRC(off) _mm_loadu_pd((void*)(sp + off))

/* *********************************************************** */

#define	FUNC_CONV3x3_d64nw(NCHAN)                               \
	mlib_status                                            \
	mlib_s_sconv3x3_d64nw_##NCHAN(mlib_image *dst,         \
	    mlib_image *src, const mlib_d64 *hkernel,          \
		const mlib_d64 *vkernel)                           \
	{                                                      \
	    __m128d buff_loc[3 * BUFF_LINE + 2],               \
			*pbuff = buff_loc;                             \
	    __m128d *buff0, *buff1, *buffT;                    \
	    __m128d hker0, hker1, hker2, vker0, vker1, vker2;  \
	    __m128d s0, d0, prev0, prev1,                      \
			sum0, sum1, sum2, aa, bb, res;                 \
	    __m128d dd;                                        \
	    mlib_s32 row, wid2, i, j;                          \
	                                                       \
	    GET_SRC_DST_PARAMETERS(mlib_d64);                  \
	    GET_KERN();                                        \
		                                                   \
	    width -= 2;                                        \
	    height -= 2;                                       \
	    width *= NCHAN;                                    \
	    dl += dll + NCHAN;                                 \
	    wid2 = (width + 1) >> 1;                           \
	                                                       \
	    if (wid2 > BUFF_LINE) {                            \
			pbuff = __mlib_malloc(                         \
				sizeof (__m128d) * 2 * wid2);              \
	    }                                                  \
		buff0 = pbuff;                                     \
	    buff1 = buff0 + wid2;                              \
	    for (j = 0; j < 2; j++) {                          \
			sp = sl;                                       \
			for (i = 0; i < wid2; i++) {                   \
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
			for (i = 0; i < (width >> 1); i++) {           \
				CONV_3x3(NCHAN);                           \
				_mm_storeu_pd((void *)dp, res);            \
				dp += 2;                                   \
			}                                              \
			if (width & 1) {                               \
				CONV_3x3(NCHAN);                           \
				_mm_storel_pd((void *)dp, res);            \
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

FUNC_CONV3x3_d64nw(1)
FUNC_CONV3x3_d64nw(2)
FUNC_CONV3x3_d64nw(3)
FUNC_CONV3x3_d64nw(4)

/* *********************************************************** */
