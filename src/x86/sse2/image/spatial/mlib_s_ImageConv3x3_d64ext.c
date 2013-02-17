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

#pragma ident   "@(#)mlib_s_ImageConv3x3_d64ext.c	9.3	07/11/05 SMI"

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
	ker1 = _mm_set1_pd((mlib_d64)kern[0]);              \
	ker2 = _mm_set1_pd((mlib_d64)kern[1]);              \
	ker3 = _mm_set1_pd((mlib_d64)kern[2]);              \
	ker4 = _mm_set1_pd((mlib_d64)kern[3]);              \
	ker5 = _mm_set1_pd((mlib_d64)kern[4]);              \
	ker6 = _mm_set1_pd((mlib_d64)kern[5]);              \
	ker7 = _mm_set1_pd((mlib_d64)kern[6]);              \
	ker8 = _mm_set1_pd((mlib_d64)kern[7]);              \
	ker9 = _mm_set1_pd((mlib_d64)kern[8]);              \

/* *********************************************************** */

#define	PREP_3x3(NCHAN, l)                                  \
	d0 = _mm_loadu_pd((void *)sbuff);                   \
	sum0 = _mm_mul_pd(d0, ker1);                        \
	sum1 = _mm_mul_pd(d0, ker4);                        \
	aa = _mm_loadu_pd((void *)(sbuff + NCHAN));         \
	sum0 = _mm_add_pd(sum0, _mm_mul_pd(aa, ker2));      \
	sum1 = _mm_add_pd(sum1, _mm_mul_pd(aa, ker5));      \
	bb = _mm_loadu_pd((void *)(sbuff + 2 * NCHAN));     \
	sum0 = _mm_add_pd(sum0, _mm_mul_pd(bb, ker3));      \
	sum1 = _mm_add_pd(sum1, _mm_mul_pd(bb, ker6));      \
	_mm_store_pd((mlib_d64 *)(buff1 + l), sum0);        \
	_mm_store_pd((mlib_d64 *)(buff2 + l), sum1);        \
	sbuff += 2;                                         \

/* *********************************************************** */

#define	CONV_3x3(NCHAN, l)                                  \
	d0 = _mm_loadu_pd((void *)sbuff);                   \
	prev0 = _mm_load_pd((mlib_d64 *)(buff0 + l));       \
	prev1 = _mm_load_pd((mlib_d64 *)(buff2 + l));       \
	sum0 = _mm_mul_pd(d0, ker1);                        \
	sum1 = _mm_mul_pd(d0, ker4);                        \
	sum2 = _mm_mul_pd(d0, ker7);                        \
	aa = _mm_loadu_pd((void *)(sbuff + NCHAN));         \
	sum0 = _mm_add_pd(sum0, _mm_mul_pd(aa, ker2));      \
	sum1 = _mm_add_pd(sum1, _mm_mul_pd(aa, ker5));      \
	sum2 = _mm_add_pd(sum2, _mm_mul_pd(aa, ker8));      \
	bb = _mm_loadu_pd((void *)(sbuff + 2 * NCHAN));     \
	sum0 = _mm_add_pd(sum0, _mm_mul_pd(bb, ker3));      \
	sum1 = _mm_add_pd(sum1, _mm_mul_pd(bb, ker6));      \
	sum2 = _mm_add_pd(sum2, _mm_mul_pd(bb, ker9));      \
	sum2 = _mm_add_pd(sum2, prev0);                     \
	sum2 = _mm_add_pd(sum2, prev1);                     \
	_mm_store_pd((mlib_d64 *)(buff0 + l), sum0);        \
	_mm_store_pd((mlib_d64 *)(buff2 + l), sum1);        \
	res = sum2;                                         \
	sbuff += 2;                                         \

/* *********************************************************** */


#define	LOAD_SRC_NF(l)                                      \
	_mm_storeu_pd((mlib_d64 *)(buffn + l),               \
		_mm_loadu_pd((mlib_d64 *)sp));                   \
	sp += 2;

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

#define	FUNC_CONV3x3_d64ext(NCHAN)                             \
	mlib_status                                            \
	mlib_s_conv3x3_d64ext_##NCHAN(                         \
		mlib_image *dst, const mlib_image *src,            \
		mlib_s32 dx_l, mlib_s32 dx_r, mlib_s32 dy_t,       \
		mlib_s32 dy_b, const mlib_d64 *kern)               \
	{                                                      \
	    __m128d *buff0, *buff1, *buff2,                    \
			*buffT, *pbuff;                                \
		__m128d *tbuf, *buffs[2 * (KSIZE + 1)], *buffn;    \
	    __m128d ker1, ker2, ker3, ker4, ker5,              \
			ker6, ker7, ker8, ker9;                        \
	    __m128d s0, d0, prev0, prev1,                      \
			sum0, sum1, sum2, aa, bb, res;                 \
	    mlib_s32 dsize, ssize, esize;                      \
	    mlib_s32 row, i, j, buff_ind = 0, gap, tail;       \
	                                                       \
	    GET_SRC_DST_PARAMETERS(mlib_d64);                  \
	    GET_KERN();                                        \
		                                                   \
	    width *= NCHAN;                                    \
		ssize = (width + NCHAN * (KSIZE - 1) + 1) >> 1;    \
	    dsize = (width + 1) >> 1;                          \
		esize = ssize + 4;                                 \
		tail = width & 1;                                  \
		                                                   \
		pbuff = __mlib_malloc(sizeof (__m128d) *           \
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
			buffn = (__m128d *)(((mlib_d64 *)buffn) + gap);\
			for (j = 0; j < ssize; j++) {                  \
				LOAD_SRC_NF(j);                            \
			}                                              \
			buffn = tbuf;                                  \
			if (dx_l) {                                    \
				EXT_LEFT_##NCHAN(                          \
					(mlib_d64 *)buffn, dx_l, 0);           \
			}                                              \
			if (dx_r) {                                    \
				EXT_RIGHT_##NCHAN(                         \
					(mlib_d64 *)buffn, (2 - dx_r), 0);     \
			}                                              \
			if ((i >= dy_t) && (i < height + 1 - dy_b))    \
				sl += sll;                                 \
		}                                                  \
	    for (i = 0; i < 2; i++) {                          \
			__m128d **buffc = buffs + buff_ind;            \
                                                           \
			sbuff = (mlib_d64 *)(buffc[0]);                \
			buffn = buffc[KSIZE];                          \
			tbuf = buffn;                                  \
			buffn = (__m128d *)(((mlib_d64 *)buffn) + gap);\
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
					(mlib_d64 *)buffn, dx_l, 0);           \
			}                                              \
			if (dx_r) {                                    \
				EXT_RIGHT_##NCHAN(                         \
				(mlib_d64 *)buffn, (2 - dx_r), 0);         \
			}                                              \
		                                                   \
			if (i < height - dy_b - 2)                     \
				sl += sll;                                 \
			buff_ind++;                                    \
		}                                                  \
		for (row = 0; row < height; row++) {               \
			__m128d **buffc = buffs + buff_ind;            \
                                                           \
			sbuff = (mlib_d64 *)(buffc[0]);                \
			buffn = buffc[KSIZE];                          \
			tbuf = buffn;                                  \
			buffn = (__m128d *)(((mlib_d64 *)buffn) + gap);\
			sp = sl;                                       \
			dp = dl;                                       \
			                                               \
			for (i = 0; i < (width >> 1); i++) {           \
				CONV_3x3(NCHAN, i);                        \
				LOAD_SRC_NF(i);                            \
				_mm_storeu_pd((void *)dp, res);            \
				dp += 2;                                   \
			}                                              \
			if (tail) {                                    \
				CONV_3x3(NCHAN, i);                        \
				LOAD_SRC_NF(i);                            \
				_mm_storel_pd((void *)dp, res);            \
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
					(mlib_d64 *)buffn, dx_l, 0);           \
			}                                              \
			if (dx_r) {                                    \
				EXT_RIGHT_##NCHAN(                         \
					(mlib_d64 *)buffn, (2 - dx_r), 0);     \
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

FUNC_CONV3x3_d64ext(1)
FUNC_CONV3x3_d64ext(2)
FUNC_CONV3x3_d64ext(3)
FUNC_CONV3x3_d64ext(4)

/* *********************************************************** */