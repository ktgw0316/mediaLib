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

#pragma ident   "@(#)mlib_s_ImageSConv7x7_f32ext.c	9.3	07/11/05 SMI"

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
#define	PREP_7x7(NCHAN, l)                                       \
	d0 = LOADU_SRC(0);                                       \
	sum6 = _mm_mul_ps(d0, hker0);                            \
	s1 = LOADU_SRC(NCHAN);                                   \
	sum6 = _mm_add_ps(sum6, _mm_mul_ps(s1, hker1));          \
	s2 = LOADU_SRC(2 * NCHAN);                               \
	sum6 = _mm_add_ps(sum6, _mm_mul_ps(s2, hker2));          \
	s3 = LOADU_SRC(3 * NCHAN);                               \
	sum6 = _mm_add_ps(sum6, _mm_mul_ps(s3, hker3));          \
	s4 = LOADU_SRC(4 * NCHAN);                               \
	sum6 = _mm_add_ps(sum6, _mm_mul_ps(s4, hker4));          \
	s5 = LOADU_SRC(5 * NCHAN);                               \
	sum6 = _mm_add_ps(sum6, _mm_mul_ps(s5, hker5));          \
	s6 = LOADU_SRC(6 * NCHAN);                               \
	sum6 = _mm_add_ps(sum6, _mm_mul_ps(s6, hker6));          \
	_mm_store_ps((mlib_f32 *)(buff0 + l), sum6);             \
	sbuff++

/* *********************************************************** */
#define	CONV_7x7(NCHAN, l)                                      \
	sum0 = _mm_load_ps((mlib_f32 *)(buff0 + l));             \
	sum1 = _mm_load_ps((mlib_f32 *)(buff1 + l));             \
	sum2 = _mm_load_ps((mlib_f32 *)(buff2 + l));             \
	sum3 = _mm_load_ps((mlib_f32 *)(buff3 + l));             \
	sum4 = _mm_load_ps((mlib_f32 *)(buff4 + l));             \
	sum5 = _mm_load_ps((mlib_f32 *)(buff5 + l));             \
	PREP_7x7(NCHAN, l);                                      \
	res = _mm_mul_ps(sum0, vker0);                           \
	res = _mm_add_ps(res, _mm_mul_ps(sum1, vker1));          \
	res = _mm_add_ps(res, _mm_mul_ps(sum2, vker2));          \
	res = _mm_add_ps(res, _mm_mul_ps(sum3, vker3));          \
	res = _mm_add_ps(res, _mm_mul_ps(sum4, vker4));          \
	res = _mm_add_ps(res, _mm_mul_ps(sum5, vker5));          \
	res = _mm_add_ps(res, _mm_mul_ps(sum6, vker6));          \

/* *********************************************************** */
#define	LOADU_SRC(off)                                       \
	_mm_loadu_ps((mlib_f32 *)sbuff + off);                   \

/* *********************************************************** */

#define	LOAD_SRC_NF(l)                                       \
	dt = _mm_loadu_ps(sp);                                   \
	_mm_storeu_ps((mlib_f32 *)(buffn + l), dt);              \
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
#define	KSIZE	7

/* *********************************************************** */

#define	FUNC_CONV7x7_f32ext(NCHAN)                             \
	mlib_status                                            \
	mlib_s_sconv7x7_f32ext_##NCHAN(                        \
		mlib_image *dst, const mlib_image *src,            \
		mlib_s32 dx_l, mlib_s32 dx_r, mlib_s32 dy_t,       \
		mlib_s32 dy_b, const mlib_d64 *hkernel,            \
		const mlib_d64 *vkernel)                           \
	{                                                      \
	    __m128 *buff0, *buff1, *buff2, *buff3, *buff4,     \
			*buff5, *buffT, *pbuff, *sbuff;                \
		__m128 *tbuf, *buffs[2 * (KSIZE + 1)], *buffn;     \
		__m128 hker0, hker1, hker2, hker3,                 \
			hker4, hker5, hker6;                           \
		__m128 vker0, vker1, vker2, vker3,                 \
			vker4, vker5, vker6;                           \
	    __m128 d0, dt, sum0, sum1, sum2,                   \
			sum3, sum4, sum5, sum6,                        \
			s1, s2, s3, s4, s5, s6, res, mask;             \
	    mlib_s32 dsize, ssize, esize;                      \
	    mlib_s32 row, i, j, buff_ind = 0, gap;             \
	                                                       \
	    GET_SRC_DST_PARAMETERS(mlib_f32);                  \
	    GET_KERN();                                        \
		                                                   \
	    width *= NCHAN;                                    \
		ssize = (width + NCHAN * (KSIZE - 1) + 3) >> 2;    \
	    dsize = (width + 3) >> 2;                          \
		esize = ssize + 12;                                \
		mask = _mm_loadu_ps((void *)                       \
			(mlib_mask128_arr + 2 * (width & 3)));         \
		                                                   \
		pbuff = __mlib_malloc(sizeof (__m128) *            \
			(KSIZE + 8) * esize);                          \
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
		buff4 = buff3 + esize;                             \
		buff5 = buff4 + esize;                             \
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
			for (j = 0; j < dx_l; j++) {                   \
				EXT_LEFT_##NCHAN(                          \
					(mlib_f32 *)buffn, dx_l, j);           \
			}                                              \
			for (j = 0; j < dx_r; j++) {                   \
				EXT_RIGHT_##NCHAN(                         \
					(mlib_f32 *)buffn, (6 - dx_r), j);     \
			}                                              \
			if ((i >= dy_t) && (i < height + 5 - dy_b))    \
				sl += sll;                                 \
		}                                                  \
	    for (i = 0; i < (KSIZE - 1); i++) {                \
			__m128 **buffc = buffs + buff_ind;             \
                                                           \
			sbuff = buffc[0];                              \
			buffn = buffc[KSIZE];                          \
			tbuf = buffn;                                  \
			buffn = (__m128 *)(((mlib_f32 *)buffn) + gap); \
			                                               \
			sp = sl;                                       \
			                                               \
			for (j = 0; j < dsize; j++) {                  \
				PREP_7x7(NCHAN, j);                        \
				LOAD_SRC_NF(j);                            \
			}                                              \
                                                           \
			buffT = buff0;                                 \
			buff0 = buff1;                                 \
			buff1 = buff2;                                 \
			buff2 = buff3;                                 \
			buff3 = buff4;                                 \
			buff4 = buff5;                                 \
			buff5 = buffT;                                 \
		                                                   \
			for (; j < ssize; j++) {                       \
				LOAD_SRC_NF(j);                            \
			}                                              \
			                                               \
			buffn = tbuf;                                  \
			for (j = 0; j < dx_l; j++) {                   \
				EXT_LEFT_##NCHAN(                          \
					(mlib_f32 *)buffn, dx_l, j);           \
			}                                              \
			for (j = 0; j < dx_r; j++) {                   \
				EXT_RIGHT_##NCHAN(                         \
				(mlib_f32 *)buffn, (6 - dx_r), j);         \
			}                                              \
			                                               \
			if (i < height - dy_b - 2)                     \
				sl += sll;                                 \
			buff_ind++;                                    \
		}                                                  \
		for (row = 0; row < height; row++) {               \
			__m128 **buffc = buffs + buff_ind;             \
                                                           \
			sbuff = buffc[0];                              \
			buffn = buffc[KSIZE];                          \
			tbuf = buffn;                                  \
			buffn = (__m128 *)(((mlib_f32 *)buffn) + gap); \
                                                           \
			sp = sl;                                       \
			dp = dl;                                       \
			                                               \
			for (i = 0; i < (width >> 2); i++) {           \
				LOAD_SRC_NF(i);                            \
				CONV_7x7(NCHAN, i);                        \
				_mm_storeu_ps((void *)dp, res);            \
				dp += 4;                                   \
			}                                              \
			if (width & 3) {                               \
				__m128 dd;                                 \
	                                                       \
				LOAD_SRC_NF(i);                            \
				CONV_7x7(NCHAN, i);                        \
				dd = _mm_loadu_ps((void *)dp);             \
				dd = _mm_or_ps(_mm_and_ps(mask, res),      \
					_mm_andnot_ps(mask, dd));              \
				_mm_storeu_ps((void *)dp, dd);             \
				i++;                                       \
			}                                              \
			                                               \
			buffT = buff0;                                 \
			buff0 = buff1;                                 \
			buff1 = buff2;                                 \
			buff2 = buff3;                                 \
			buff3 = buff4;                                 \
			buff4 = buff5;                                 \
			buff5 = buffT;                                 \
		                                                   \
			for (; i < ssize; i++) {                       \
				LOAD_SRC_NF(i);                            \
			}                                              \
			                                               \
			buffn = tbuf;                                  \
			for (j = 0; j < dx_l; j++) {                   \
				EXT_LEFT_##NCHAN(                          \
					(mlib_f32 *)buffn, dx_l, j);           \
			}                                              \
			for (j = 0; j < dx_r; j++) {                   \
				EXT_RIGHT_##NCHAN(                         \
					(mlib_f32 *)buffn, (6 - dx_r), j);     \
			}                                              \
	                                                       \
			if (row < height - dy_b - 8)                   \
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

FUNC_CONV7x7_f32ext(1)
FUNC_CONV7x7_f32ext(2)
FUNC_CONV7x7_f32ext(3)
FUNC_CONV7x7_f32ext(4)

/* *********************************************************** */
