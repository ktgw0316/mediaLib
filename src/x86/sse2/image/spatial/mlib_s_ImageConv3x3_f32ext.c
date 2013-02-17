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

#pragma ident   "@(#)mlib_s_ImageConv3x3_f32ext.c	9.3	07/11/05 SMI"

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

#define	GET_KERN()                                        \
	ker1 = _mm_set1_ps((mlib_f32)kern[0]);              \
	ker2 = _mm_set1_ps((mlib_f32)kern[1]);              \
	ker3 = _mm_set1_ps((mlib_f32)kern[2]);              \
	ker4 = _mm_set1_ps((mlib_f32)kern[3]);              \
	ker5 = _mm_set1_ps((mlib_f32)kern[4]);              \
	ker6 = _mm_set1_ps((mlib_f32)kern[5]);              \
	ker7 = _mm_set1_ps((mlib_f32)kern[6]);              \
	ker8 = _mm_set1_ps((mlib_f32)kern[7]);              \
	ker9 = _mm_set1_ps((mlib_f32)kern[8]);              \

/* *********************************************************** */

#define	PREP_3x3(NCHAN, l)                                  \
	d0 = _mm_loadu_ps((void *)sbuff);                   \
	sum0 = _mm_mul_ps(d0, ker1);                        \
	sum1 = _mm_mul_ps(d0, ker4);                        \
	aa = _mm_loadu_ps((void *)(sbuff + NCHAN));         \
	sum0 = _mm_add_ps(sum0, _mm_mul_ps(aa, ker2));      \
	sum1 = _mm_add_ps(sum1, _mm_mul_ps(aa, ker5));      \
	bb = _mm_loadu_ps((void *)(sbuff + 2 * NCHAN));     \
	sum0 = _mm_add_ps(sum0, _mm_mul_ps(bb, ker3));      \
	sum1 = _mm_add_ps(sum1, _mm_mul_ps(bb, ker6));      \
	_mm_store_ps((mlib_f32 *)(buff1 + l), sum0);        \
	_mm_store_ps((mlib_f32 *)(buff2 + l), sum1);        \
	sbuff += 4;                                         \

/* *********************************************************** */

#define	CONV_3x3(NCHAN, l)                                  \
	d0 = _mm_loadu_ps((void *)sbuff);                   \
	prev0 = _mm_load_ps((mlib_f32 *)(buff0 + l));       \
	prev1 = _mm_load_ps((mlib_f32 *)(buff2 + l));       \
	sum0 = _mm_mul_ps(d0, ker1);                        \
	sum1 = _mm_mul_ps(d0, ker4);                        \
	sum2 = _mm_mul_ps(d0, ker7);                        \
	aa = _mm_loadu_ps((void *)(sbuff + NCHAN));         \
	sum0 = _mm_add_ps(sum0, _mm_mul_ps(aa, ker2));      \
	sum1 = _mm_add_ps(sum1, _mm_mul_ps(aa, ker5));      \
	sum2 = _mm_add_ps(sum2, _mm_mul_ps(aa, ker8));      \
	bb = _mm_loadu_ps((void *)(sbuff + 2 * NCHAN));     \
	sum0 = _mm_add_ps(sum0, _mm_mul_ps(bb, ker3));      \
	sum1 = _mm_add_ps(sum1, _mm_mul_ps(bb, ker6));      \
	sum2 = _mm_add_ps(sum2, _mm_mul_ps(bb, ker9));      \
	sum2 = _mm_add_ps(sum2, prev0);                     \
	sum2 = _mm_add_ps(sum2, prev1);                     \
	_mm_store_ps((mlib_f32 *)(buff0 + l), sum0);        \
	_mm_store_ps((mlib_f32 *)(buff2 + l), sum1);        \
	res = sum2;                                         \
	sbuff += 4;                                         \

/* *********************************************************** */


#define	LOAD_SRC_NF(l)                                      \
	_mm_storeu_ps((mlib_f32 *)(buffn + l),               \
		_mm_loadu_ps((mlib_f32 *)sp));                   \
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
#define	KSIZE	3

/* *********************************************************** */

#define	FUNC_CONV3x3_f32ext(NCHAN)                             \
	mlib_status                                            \
	mlib_s_conv3x3_f32ext_##NCHAN(                         \
		mlib_image *dst, const mlib_image *src,            \
		mlib_s32 dx_l, mlib_s32 dx_r, mlib_s32 dy_t,       \
		mlib_s32 dy_b, const mlib_d64 *kern)               \
	{                                                      \
	    __m128 *buff0, *buff1, *buff2,                     \
			*buffT, *pbuff;                                \
		__m128 *tbuf, *buffs[2 * (KSIZE + 1)], *buffn;     \
	    __m128 ker1, ker2, ker3, ker4, ker5,               \
			ker6, ker7, ker8, ker9;                        \
	    __m128 s0, d0, prev0, prev1,                       \
			sum0, sum1, sum2, aa, bb, res, mask;           \
	    mlib_s32 dsize, ssize, esize;                      \
	    mlib_s32 row, i, j, buff_ind = 0, gap;             \
	                                                       \
	    GET_SRC_DST_PARAMETERS(mlib_f32);                  \
	    GET_KERN();                                        \
		                                                   \
	    width *= NCHAN;                                    \
		ssize = (width + NCHAN * (KSIZE - 1) + 3) >> 2;    \
	    dsize = (width + 3) >> 2;                          \
		esize = ssize + 4;                                 \
		mask = _mm_loadu_ps((void *)                       \
			(mlib_mask128_arr + 2 * (width & 3)));         \
	                                                       \
		pbuff = __mlib_malloc(sizeof (__m128) *            \
			(KSIZE + 4) * esize);                          \
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
	    buff1 = buff0 + esize;                             \
	    buff2 = buff1 + esize;                             \
	                                                       \
		gap = NCHAN * dx_l;                                \
		for (i = 0; i < KSIZE; i++) {                      \
			buffn = buffs[i];                              \
			sp = sl;                                       \
			tbuf = buffn;                                  \
			buffn = (__m128 *)(((mlib_f32 *)buffn) + gap); \
			for (j = 0; j < ssize; j++) {                  \
				LOAD_SRC_NF(j);                            \
			}                                              \
			buffn = tbuf;                                  \
			if (dx_l) {                                    \
				EXT_LEFT_##NCHAN(                          \
					(mlib_f32 *)buffn, dx_l, 0);           \
			}                                              \
			if (dx_r) {                                    \
				EXT_RIGHT_##NCHAN(                         \
					(mlib_f32 *)buffn, (2 - dx_r), 0);     \
			}                                              \
			if ((i >= dy_t) && (i < height + 1 - dy_b))    \
				sl += sll;                                 \
		}                                                  \
	    for (i = 0; i < 2; i++) {                          \
			__m128 **buffc = buffs + buff_ind;             \
                                                           \
			sbuff = (mlib_f32 *)(buffc[0]);                \
			buffn = buffc[KSIZE];                          \
			tbuf = buffn;                                  \
			buffn = (__m128 *)(((mlib_f32 *)buffn) + gap); \
			sp = sl;                                       \
			                                               \
			for (j = 0; j < dsize; j++) {                  \
				PREP_3x3(NCHAN, j);                        \
				LOAD_SRC_NF(j);                            \
			}                                              \
			                                               \
			for (; j < ssize; j++) {                       \
				LOAD_SRC_NF(j);                            \
			}                                              \
                                                           \
			if (i == 0) {                                  \
				buffT = buff1;                             \
				buff1 = buff0;                             \
				buff0 = buffT;                             \
			}                                              \
			buffn = tbuf;                                  \
			if (dx_l) {                                    \
				EXT_LEFT_##NCHAN(                          \
					(mlib_f32 *)buffn, dx_l, 0);           \
			}                                              \
			if (dx_r) {                                    \
				EXT_RIGHT_##NCHAN(                         \
				(mlib_f32 *)buffn, (2 - dx_r), 0);         \
			}                                              \
		                                                   \
			if (i < height - dy_b - 2)                     \
				sl += sll;                                 \
			buff_ind++;                                    \
		}                                                  \
		for (row = 0; row < height; row++) {               \
			__m128 **buffc = buffs + buff_ind;             \
                                                           \
			sbuff = (mlib_f32 *)(buffc[0]);                \
			buffn = buffc[KSIZE];                          \
			tbuf = buffn;                                  \
			buffn = (__m128 *)(((mlib_f32 *)buffn) + gap); \
			sp = sl;                                       \
			dp = dl;                                       \
			                                               \
			for (i = 0; i < (width >> 2); i++) {           \
				CONV_3x3(NCHAN, i);                        \
				LOAD_SRC_NF(i);                            \
				_mm_storeu_ps((void *)dp, res);            \
				dp += 4;                                   \
			}                                              \
			if (width & 3) {                               \
				__m128 dd;                                 \
	                                                       \
				CONV_3x3(NCHAN, i);                        \
				LOAD_SRC_NF(i);                            \
				dd = _mm_loadu_ps((void *)dp);             \
				dd = _mm_or_ps(_mm_and_ps(mask, res),      \
					_mm_andnot_ps(mask, dd));              \
				_mm_storeu_ps((void *)dp, dd);             \
				i++;                                       \
			}                                              \
			                                               \
			for (; i < ssize; i++) {                       \
				LOAD_SRC_NF(i);                            \
			}                                              \
                                                           \
			buffT = buff1;                                 \
			buff1 = buff0;                                 \
			buff0 = buffT;                                 \
		                                                   \
			buffn = tbuf;                                  \
			if (dx_l) {                                    \
				EXT_LEFT_##NCHAN(                          \
					(mlib_f32 *)buffn, dx_l, 0);           \
			}                                              \
			if (dx_r) {                                    \
				EXT_RIGHT_##NCHAN(                         \
					(mlib_f32 *)buffn, (2 - dx_r), 0);     \
			}                                              \
	                                                       \
			if (row < height - dy_b - 4)                   \
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

FUNC_CONV3x3_f32ext(1)
FUNC_CONV3x3_f32ext(2)
FUNC_CONV3x3_f32ext(3)
FUNC_CONV3x3_f32ext(4)

/* *********************************************************** */
