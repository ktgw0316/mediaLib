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

#pragma ident   "@(#)mlib_s_ImageConv2x2_f32ext.c	9.3	07/11/05 SMI"

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

#define	GET_SRC_DST_PARAMETERS(type)                               \
	mlib_s32 height = mlib_ImageGetHeight(src);                \
	mlib_s32 width = mlib_ImageGetWidth(src);                  \
	mlib_s32 sll = mlib_ImageGetStride(src) / sizeof (type);   \
	mlib_s32 dll = mlib_ImageGetStride(dst) / sizeof (type);   \
	type *sl = (type *) mlib_ImageGetData(src);                \
	type *dl = (type *) mlib_ImageGetData(dst);                \
	type *sp, *dp, *sbuff

/* *********************************************************** */

#define	GET_KERN()                                            \
	ker1 = _mm_set1_ps((mlib_f32)kern[0]);                 \
	ker2 = _mm_set1_ps((mlib_f32)kern[1]);                 \
	ker3 = _mm_set1_ps((mlib_f32)kern[2]);                 \
	ker4 = _mm_set1_ps((mlib_f32)kern[3]);                 \

/* *********************************************************** */
#define	PREP_2x2(NCHAN, l)                                  \
	d0 = _mm_loadu_ps((void *)sbuff);                      \
	sum0 = _mm_mul_ps(d0, ker1);                           \
	aa = _mm_loadu_ps((void *)(sbuff + NCHAN));            \
	sum0 = _mm_add_ps(sum0, _mm_mul_ps(aa, ker2));         \
	_mm_store_ps((mlib_f32 *)(buff0 + l), sum0);           \
	sbuff += 4;                                            \

/* *********************************************************** */

#define	CONV_2x2(NCHAN, l)                                  \
	d0 = _mm_loadu_ps((void *)sbuff);                      \
	prev0 = _mm_load_ps((mlib_f32 *)(buff0 + l));          \
	sum0 = _mm_mul_ps(d0, ker1);                           \
	sum1 = _mm_mul_ps(d0, ker3);                           \
	aa = _mm_loadu_ps((void *)(sbuff + NCHAN));            \
	sum0 = _mm_add_ps(sum0, _mm_mul_ps(aa, ker2));         \
	sum1 = _mm_add_ps(sum1, _mm_mul_ps(aa, ker4));         \
	sum1 = _mm_add_ps(sum1, prev0);                        \
	_mm_store_ps((mlib_f32 *)(buff0 + l), sum0);           \
	sbuff += 4;                                            \
	res = sum1;                                            \

/* *********************************************************** */

#define	LOAD_SRC_NF(l)                                      \
	_mm_storeu_ps((mlib_f32 *)(buffn + l),                 \
		_mm_loadu_ps(sp));                                 \
	sp += 4;

/* *********************************************************** */

#define	EXT_LEFT_1(buff, dx_l, l)                        \
	*((buff) + 0 + l) = *((buff) + 0 + dx_l)

/* *********************************************************** */

#define	EXT_LEFT_2(buff, dx_l, l)                        \
	*((buff) + 0 + 2 * l) = *((buff) + 0 + 2 * dx_l);    \
	*((buff) + 1 + 2 * l) = *((buff) + 1 + 2 * dx_l)

/* *********************************************************** */

#define	EXT_LEFT_3(buff, dx_l, l)                        \
	*((buff) + 0 + 3 * l) = *((buff) + 0 + 3 * dx_l);    \
	*((buff) + 1 + 3 * l) = *((buff) + 1 + 3 * dx_l);    \
	*((buff) + 2 + 3 * l) = *((buff) + 2 + 3 * dx_l)

/* *********************************************************** */

#define	EXT_LEFT_4(buff, dx_l, l)                        \
	*((buff) + 0 + 4 * l) = *((buff) + 0 + 4 * dx_l);    \
	*((buff) + 1 + 4 * l) = *((buff) + 1 + 4 * dx_l);    \
	*((buff) + 2 + 4 * l) = *((buff) + 2 + 4 * dx_l);    \
	*((buff) + 3 + 4 * l) = *((buff) + 3 + 4 * dx_l)

/* *********************************************************** */

#define	EXT_RIGHT_1(buff, dx_l, l)                       \
	*((buff) + 0 + l + width + dx_l) =                   \
	    *((buff) + 0 + (dx_l - 1) + width)

/* *********************************************************** */

#define	EXT_RIGHT_2(buff, dx_l, l)                       \
	*((buff) + 0 + 2 * l + width + dx_l * 2) =           \
	    *((buff) + 0 + 2 * (dx_l - 1) + width);          \
	*((buff) + 1 + 2 * l + width + dx_l * 2) =           \
	    *((buff) + 1 + 2 * (dx_l - 1) + width)

/* *********************************************************** */

#define	EXT_RIGHT_3(buff, dx_l, l)                       \
	*((buff) + 0 + 3 * l + width + dx_l * 3) =           \
	    *((buff) + 0 + 3 * (dx_l - 1) + width);          \
	*((buff) + 1 + 3 * l + width + dx_l * 3) =           \
	    *((buff) + 1 + 3 * (dx_l - 1) + width);          \
	*((buff) + 2 + 3 * l + width + dx_l * 3) =           \
	    *((buff) + 2 + 3 * (dx_l - 1) + width)

/* *********************************************************** */

#define	EXT_RIGHT_4(buff, dx_l, l)                       \
	*((buff) + 0 + 4 * l + width + dx_l * 4) =           \
	    *((buff) + 0 + 4 * (dx_l - 1) + width);          \
	*((buff) + 1 + 4 * l + width + dx_l * 4) =           \
	    *((buff) + 1 + 4 * (dx_l - 1) + width);          \
	*((buff) + 2 + 4 * l + width + dx_l * 4) =           \
	    *((buff) + 2 + 4 * (dx_l - 1) + width);          \
	*((buff) + 3 + 4 * l + width + dx_l * 4) =           \
	    *((buff) + 3 + 4 * (dx_l - 1) + width);          \

/* *********************************************************** */

#undef KSIZE
#define	KSIZE	2

/* *********************************************************** */

#define	FUNC_CONV2x2_f32ext(NCHAN)                             \
	mlib_status                                            \
	mlib_s_conv2x2_f32ext_##NCHAN(                         \
		mlib_image *dst, const mlib_image *src,            \
		mlib_s32 dx_l, mlib_s32 dx_r, mlib_s32 dy_t,       \
		mlib_s32 dy_b, const mlib_d64 *kern)               \
	{                                                      \
	    __m128 *buff0, *pbuff;                             \
		__m128 *buffs[2 * (KSIZE + 1)], **buffc, *buffn;   \
	    __m128 ker1, ker2, ker3, ker4;                     \
	    __m128 s0, d0, prev0, sum0, sum1, aa, res, mask;   \
	    mlib_s32 dsize, ssize, esize;                      \
	    mlib_s32 row, i, j, buff_ind = 0;                  \
	                                                       \
	    GET_SRC_DST_PARAMETERS(mlib_f32);                  \
	    GET_KERN();                                        \
		                                                   \
	    width *= NCHAN;                                    \
		ssize = (width + NCHAN * (KSIZE - 1) + 3) >> 2;    \
	    dsize = (width + 3) >> 2;                          \
		esize = ssize + 4;                                 \
		mask = _mm_load_ps((void *)                        \
			(mlib_mask128_arr + 2 * (width & 3)));         \
	                                                       \
		pbuff = __mlib_malloc(sizeof (__m128) *            \
			(KSIZE + 2) * esize);                          \
		if (pbuff == NULL)                                 \
			return (MLIB_FAILURE);                         \
		                                                   \
		for (i = 0; i < (KSIZE + 1); i++) {                \
			buffs[i] = pbuff + i * esize;                  \
		}                                                  \
		for (i = 0; i < (KSIZE + 1); i++) {                \
			buffs[(KSIZE + 1) + i] = buffs[i];             \
		}                                                  \
	    buff0 = buffs[KSIZE] + esize;                      \
	                                                       \
		for (i = 0; i < KSIZE; i++) {                      \
			buffn = buffs[i];                              \
			sp = sl;                                       \
			for (j = 0; j < ssize; j++) {                  \
				LOAD_SRC_NF(j);                            \
			}                                              \
			if (dx_r) {                                    \
				EXT_RIGHT_##NCHAN(                         \
					(mlib_f32 *)buffn, 0, 0);              \
			}                                              \
			if ((i >= dy_t) && (i < height - dy_b))        \
				sl += sll;                                 \
		}                                                  \
		                                                   \
		buffc = buffs + buff_ind;                          \
		sbuff = (mlib_f32 *)(buffc[0]);                    \
		buffn = buffc[KSIZE];                              \
		sp = sl;                                           \
			                                               \
		for (j = 0; j < dsize; j++) {                      \
			PREP_2x2(NCHAN, j);                            \
			LOAD_SRC_NF(j);                                \
		}                                                  \
			                                               \
		if (ssize - dsize) {                               \
			LOAD_SRC_NF(j);                                \
		}                                                  \
                                                           \
		if (dx_r) {                                        \
			EXT_RIGHT_##NCHAN(                             \
			(mlib_f32 *)buffn, 0, 0);                      \
		}                                                  \
		                                                   \
		if (0 < height - dy_b - 2)                         \
			sl += sll;                                     \
		buff_ind++;                                        \
		                                                   \
		for (row = 0; row < height; row++) {               \
			buffc = buffs + buff_ind;                      \
                                                           \
			sbuff = (mlib_f32 *)(buffc[0]);                \
			buffn = buffc[KSIZE];                          \
			sp = sl;                                       \
			dp = dl;                                       \
			                                               \
			for (i = 0; i < (width >> 2); i++) {           \
				CONV_2x2(NCHAN, i);                        \
				LOAD_SRC_NF(i);                            \
				_mm_storeu_ps((void *)dp, res);            \
				dp += 4;                                   \
			}                                              \
			if (width & 3) {                               \
				__m128 dd;                                 \
	                                                       \
				CONV_2x2(NCHAN, i);                        \
				LOAD_SRC_NF(i);                            \
				dd = _mm_loadu_ps((void *)dp);             \
				dd = _mm_or_ps(_mm_and_ps(mask, res),      \
					_mm_andnot_ps(mask, dd));              \
				_mm_storeu_ps((void *)dp, dd);             \
				i++;                                       \
			}                                              \
			                                               \
			if (ssize - dsize) {                           \
				LOAD_SRC_NF(i);                            \
			}                                              \
                                                           \
			if (dx_r) {                                    \
				EXT_RIGHT_##NCHAN(                         \
					(mlib_f32 *)buffn, 0, 0);              \
			}                                              \
	                                                       \
			if (row < height - dy_b - 3)                   \
				sl += sll;                                 \
			dl += dll;                                     \
			                                               \
			buff_ind++;                                    \
			if (buff_ind >= (KSIZE + 1))                   \
				buff_ind = 0;                              \
	    }                                                  \
		__mlib_free(pbuff);                                \
	    return (MLIB_SUCCESS);                             \
	}

/* *********************************************************** */

FUNC_CONV2x2_f32ext(1)
FUNC_CONV2x2_f32ext(2)
FUNC_CONV2x2_f32ext(3)
FUNC_CONV2x2_f32ext(4)

/* *********************************************************** */
