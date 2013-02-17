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

#pragma ident   "@(#)mlib_s_ImageSConv5x5_d64ext.c	9.3	07/11/05 SMI"

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
	type *sp, *dp

/* *********************************************************** */
#define	GET_KERN()                                                 \
	hker0 = _mm_set1_pd((mlib_d64)hkernel[0]);               \
	hker1 = _mm_set1_pd((mlib_d64)hkernel[1]);               \
	hker2 = _mm_set1_pd((mlib_d64)hkernel[2]);               \
	hker3 = _mm_set1_pd((mlib_d64)hkernel[3]);               \
	hker4 = _mm_set1_pd((mlib_d64)hkernel[4]);               \
	vker0 = _mm_set1_pd((mlib_d64)vkernel[0]);               \
	vker1 = _mm_set1_pd((mlib_d64)vkernel[1]);               \
	vker2 = _mm_set1_pd((mlib_d64)vkernel[2]);               \
	vker3 = _mm_set1_pd((mlib_d64)vkernel[3]);               \
	vker4 = _mm_set1_pd((mlib_d64)vkernel[4]);               \
/* *********************************************************** */
#define	PREP_5x5(NCHAN, l)                                      \
	d0 = LOADU_SRC(0);                                       \
	sum4 = _mm_mul_pd(d0, hker0);                            \
	aa = LOADU_SRC(NCHAN);                                   \
	sum4 = _mm_add_pd(sum4, _mm_mul_pd(aa, hker1));          \
	bb = LOADU_SRC(2 * NCHAN);                               \
	sum4 = _mm_add_pd(sum4, _mm_mul_pd(bb, hker2));          \
	cc = LOADU_SRC(3 * NCHAN);                               \
	sum4 = _mm_add_pd(sum4, _mm_mul_pd(cc, hker3));          \
	dd = LOADU_SRC(4 * NCHAN);                               \
	sum4 = _mm_add_pd(sum4, _mm_mul_pd(dd, hker4));          \
	_mm_store_pd((mlib_d64 *)(buff0 + l), sum4);             \
	sbuff++

/* *********************************************************** */
#define	CONV_5x5(NCHAN, l)                                      \
	sum0 = _mm_load_pd((mlib_d64 *)(buff0 + l));             \
	sum1 = _mm_load_pd((mlib_d64 *)(buff1 + l));             \
	sum2 = _mm_load_pd((mlib_d64 *)(buff2 + l));             \
	sum3 = _mm_load_pd((mlib_d64 *)(buff3 + l));             \
	PREP_5x5(NCHAN, l);                                      \
	res = _mm_mul_pd(sum0, vker0);                           \
	res = _mm_add_pd(res, _mm_mul_pd(sum1, vker1));          \
	res = _mm_add_pd(res, _mm_mul_pd(sum2, vker2));          \
	res = _mm_add_pd(res, _mm_mul_pd(sum3, vker3));          \
	res = _mm_add_pd(res, _mm_mul_pd(sum4, vker4));          \


/* *********************************************************** */
#define	LOADU_SRC(off)                                   \
	_mm_loadu_pd((mlib_d64 *)sbuff + off)

/* *********************************************************** */

#define	LOAD_SRC_NF(l)                                      \
	dt = _mm_loadu_pd(sp);                               \
	_mm_storeu_pd((void *)(buffn + l), dt);              \
	sp += 2

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
#define	KSIZE	5

/* *********************************************************** */

#define	FUNC_CONV5x5_d64ext(NCHAN)                             \
	mlib_status                                            \
	mlib_s_sconv5x5_d64ext_##NCHAN(                        \
		mlib_image *dst, const mlib_image *src,            \
		mlib_s32 dx_l, mlib_s32 dx_r, mlib_s32 dy_t,       \
		mlib_s32 dy_b, const mlib_d64 *hkernel,            \
		const mlib_d64 *vkernel)                           \
	{                                                      \
	    __m128d *buff0, *buff1, *buff2,                    \
			*buff3, *buffT, *pbuff, *sbuff;                \
		__m128d *tbuf, *buffs[2 * (KSIZE + 1)], *buffn;    \
		__m128d hker0, hker1, hker2, hker3, hker4,         \
			vker0, vker1, vker2, vker3, vker4;             \
	    __m128d s0, d0, dt,                                \
			sum0, sum1, sum2, sum3, sum4,                  \
			aa, bb, cc, dd, res;                           \
	    mlib_s32 dsize, ssize, esize;                      \
	    mlib_s32 row, i, j, buff_ind = 0, gap;             \
	                                                       \
	    GET_SRC_DST_PARAMETERS(mlib_d64);                  \
	    GET_KERN();                                        \
		                                                   \
	    width *= NCHAN;                                    \
		ssize = (width + NCHAN * (KSIZE - 1) + 1) >> 1;    \
	    dsize = (width + 1) >> 1;                          \
		esize = ssize + 8;                                 \
		                                                   \
		pbuff = __mlib_malloc(sizeof (__m128d) *           \
			(KSIZE + 6) * esize);                          \
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
		buff3 = buff2 + esize;                             \
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
			for (j = 0; j < dx_l; j++) {                   \
				EXT_LEFT_##NCHAN(                          \
					(mlib_d64 *)buffn, dx_l, j);           \
			}                                              \
			for (j = 0; j < dx_r; j++) {                   \
				EXT_RIGHT_##NCHAN(                         \
					(mlib_d64 *)buffn, (4 - dx_r), j);     \
			}                                              \
			if ((i >= dy_t) && (i < height + 3 - dy_b))    \
				sl += sll;                                 \
		}                                                  \
	    for (i = 0; i < (KSIZE - 1); i++) {                \
			__m128d **buffc = buffs + buff_ind;            \
                                                           \
			sbuff = buffc[0];                              \
			buffn = buffc[KSIZE];                          \
			tbuf = buffn;                                  \
			buffn = (__m128d *)(((mlib_d64 *)buffn) + gap);\
			                                               \
			sp = sl;                                       \
			                                               \
			for (j = 0; j < dsize; j++) {                  \
				PREP_5x5(NCHAN, j);                        \
				LOAD_SRC_NF(j);                            \
			}                                              \
			                                               \
			for (; j < ssize; j++) {                       \
				LOAD_SRC_NF(j);                            \
			}                                              \
			                                               \
			buffn = tbuf;                                  \
			for (j = 0; j < dx_l; j++) {                   \
				EXT_LEFT_##NCHAN(                          \
					(mlib_d64 *)buffn, dx_l, j);           \
			}                                              \
			for (j = 0; j < dx_r; j++) {                   \
				EXT_RIGHT_##NCHAN(                         \
				(mlib_d64 *)buffn, (4 - dx_r), j);         \
			}                                              \
		                                                   \
			buffT = buff0;                                 \
			buff0 = buff1;                                 \
			buff1 = buff2;                                 \
			buff2 = buff3;                                 \
			buff3 = buffT;                                 \
			                                               \
			if (i < height - dy_b - 2)                     \
				sl += sll;                                 \
			buff_ind++;                                    \
		}                                                  \
		for (row = 0; row < height; row++) {               \
			__m128d **buffc = buffs + buff_ind;            \
                                                           \
			sbuff = buffc[0];                              \
			buffn = buffc[KSIZE];                          \
			tbuf = buffn;                                  \
			buffn = (__m128d *)(((mlib_d64 *)buffn) + gap);\
                                                           \
			sp = sl;                                       \
			dp = dl;                                       \
			                                               \
			for (i = 0; i < (width >> 1); i++) {           \
				LOAD_SRC_NF(i);                            \
				CONV_5x5(NCHAN, i);                        \
				_mm_storeu_pd((void *)dp, res);            \
				dp += 2;                                   \
			}                                              \
			if (width & 1) {                               \
				__m128d dd;                                \
	                                                       \
				LOAD_SRC_NF(i);                            \
				CONV_5x5(NCHAN, i);                        \
				_mm_storel_pd((void *)dp, res);            \
				i++;                                       \
			}                                              \
			                                               \
			buffT = buff0;                                 \
			buff0 = buff1;                                 \
			buff1 = buff2;                                 \
			buff2 = buff3;                                 \
			buff3 = buffT;                                 \
		                                                   \
			for (; i < ssize; i++) {                       \
				LOAD_SRC_NF(i);                            \
			}                                              \
			                                               \
			buffn = tbuf;                                  \
			for (j = 0; j < dx_l; j++) {                   \
				EXT_LEFT_##NCHAN(                          \
					(mlib_d64 *)buffn, dx_l, j);           \
			}                                              \
			for (j = 0; j < dx_r; j++) {                   \
				EXT_RIGHT_##NCHAN(                         \
					(mlib_d64 *)buffn, (4 - dx_r), j);     \
			}                                              \
	                                                       \
			if (row < height - dy_b - 6)                   \
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

FUNC_CONV5x5_d64ext(1)
FUNC_CONV5x5_d64ext(2)
FUNC_CONV5x5_d64ext(3)
FUNC_CONV5x5_d64ext(4)

/* *********************************************************** */
