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

#pragma ident   "@(#)mlib_s_ImageSConv7x7_8ext.c	9.3	07/11/05 SMI"

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

#define	ALGN(x, y, sh)                                                    \
	_mm_or_si128(_mm_srli_si128(x, (sh)), _mm_slli_si128(y, 16 - (sh)))

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
	hker0 = _mm_set1_epi16((hkernel[0] >> 16) & 0xFFFF);    \
	hker1 = _mm_set1_epi16((hkernel[1] >> 16) & 0xFFFF);    \
	hker2 = _mm_set1_epi16((hkernel[2] >> 16) & 0xFFFF);    \
	hker3 = _mm_set1_epi16((hkernel[3] >> 16) & 0xFFFF);    \
	hker4 = _mm_set1_epi16((hkernel[4] >> 16) & 0xFFFF);    \
	hker5 = _mm_set1_epi16((hkernel[5] >> 16) & 0xFFFF);    \
	hker6 = _mm_set1_epi16((hkernel[6] >> 16) & 0xFFFF);    \
	vker0 = _mm_set1_epi16((vkernel[0] >> 16) & 0xFFFF);    \
	vker1 = _mm_set1_epi16((vkernel[1] >> 16) & 0xFFFF);    \
	vker2 = _mm_set1_epi16((vkernel[2] >> 16) & 0xFFFF);    \
	vker3 = _mm_set1_epi16((vkernel[3] >> 16) & 0xFFFF);    \
	vker4 = _mm_set1_epi16((vkernel[4] >> 16) & 0xFFFF);    \
	vker5 = _mm_set1_epi16((vkernel[5] >> 16) & 0xFFFF);    \
	vker6 = _mm_set1_epi16((vkernel[6] >> 16) & 0xFFFF);    \
	shift = 2 * scalef_expon - (32 + 32 - 7)
/* *********************************************************** */
#define	PREP_7x7_1(l)                                        \
	d0 = d1;                                                 \
	LOADU_SRC(d1, sbuff);                                    \
	sum6 = _mm_mulhi_epi16(d0, hker0);                       \
	s1 = ALGN(d0, d1, 2);                                    \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s1, hker1));  \
	s2 = ALGN(d0, d1, 4);                                    \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s2, hker2));  \
	s3 = ALGN(d0, d1, 6);                                    \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s3, hker3));  \
	s4 = ALGN(d0, d1, 8);                                    \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s4, hker4));  \
	s5 = ALGN(d0, d1, 10);                                   \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s5, hker5));  \
	s6 = ALGN(d0, d1, 12);                                   \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s6, hker6));  \
	_mm_store_si128(buff0 + l, sum6)

/* *********************************************************** */
#define	PREP_7x7_2(l)                                        \
	d0 = d1;                                                 \
	d1 = d2;                                                 \
	LOADU_SRC(d2, sbuff);                                    \
	sum6 = _mm_mulhi_epi16(d0, hker0);                       \
	s1 = ALGN(d0, d1, 4);                                    \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s1, hker1));  \
	s2 = ALGN(d0, d1, 8);                                    \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s2, hker2));  \
	s3 = ALGN(d0, d1, 12);                                   \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s3, hker3));  \
	s4 = ALGN(d0, d1, 16);                                   \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s4, hker4));  \
	s5 = ALGN(d1, d2, 4);                                    \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s5, hker5));  \
	s6 = ALGN(d1, d2, 8);                                    \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s6, hker6));  \
	_mm_store_si128(buff0 + l, sum6)
/* *********************************************************** */
#define	PREP_7x7_3(l)                                        \
	d0 = d1;                                                 \
	d1 = d2;                                                 \
	d2 = d3;                                                 \
	LOADU_SRC(d3, sbuff);                                    \
	sum6 = _mm_mulhi_epi16(d0, hker0);                       \
	s1 = ALGN(d0, d1, 6);                                    \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s1, hker1));  \
	s2 = ALGN(d0, d1, 12);                                   \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s2, hker2));  \
	s3 = ALGN(d1, d2, 2);                                    \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s3, hker3));  \
	s4 = ALGN(d1, d2, 8);                                    \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s4, hker4));  \
	s5 = ALGN(d1, d2, 14);                                   \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s5, hker5));  \
	s6 = ALGN(d2, d3, 4);                                    \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s6, hker6));  \
	_mm_store_si128(buff0 + l, sum6)

/* *********************************************************** */
#define	PREP_7x7_4(l)                                        \
	d0 = d1;                                                 \
	d1 = d2;                                                 \
	d2 = d3;                                                 \
	LOADU_SRC(d3, sbuff);                                    \
	sum6 = _mm_mulhi_epi16(d0, hker0);                       \
	s1 = ALGN(d0, d1, 8);                                    \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s1, hker1));  \
	s2 = ALGN(d0, d1, 16);                                   \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s2, hker2));  \
	s3 = ALGN(d1, d2, 8);                                    \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s3, hker3));  \
	s4 = ALGN(d1, d2, 16);                                   \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s4, hker4));  \
	s5 = ALGN(d2, d3, 8);                                    \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s5, hker5));  \
	s6 = ALGN(d2, d3, 16);                                   \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s6, hker6));  \
	_mm_store_si128(buff0 + l, sum6)


/* *********************************************************** */
#define	CONV_7x7(NCHAN, l)                                      \
	sum0 = _mm_load_si128(buff0 + l);                        \
	sum1 = _mm_load_si128(buff1 + l);                        \
	sum2 = _mm_load_si128(buff2 + l);                        \
	sum3 = _mm_load_si128(buff3 + l);                        \
	sum4 = _mm_load_si128(buff4 + l);                        \
	sum5 = _mm_load_si128(buff5 + l);                        \
	PREP_7x7_##NCHAN(l);                                     \
	res = _mm_mulhi_epi16(sum0, vker0);                      \
	res = _mm_add_epi16(res, _mm_mulhi_epi16(sum1, vker1));  \
	res = _mm_add_epi16(res, _mm_mulhi_epi16(sum2, vker2));  \
	res = _mm_add_epi16(res, _mm_mulhi_epi16(sum3, vker3));  \
	res = _mm_add_epi16(res, _mm_mulhi_epi16(sum4, vker4));  \
	res = _mm_add_epi16(res, _mm_mulhi_epi16(sum5, vker5));  \
	res = _mm_add_epi16(res, _mm_mulhi_epi16(sum6, vker6));  \
	res = _mm_srai_epi16(res, shift);                        \
	res = _mm_packus_epi16(res, res)


/* *********************************************************** */
#define	LOADU_SRC(ds, tp)                                   \
	ds = _mm_load_si128(tp);                             \
	tp++;                                                \

/* *********************************************************** */
#define	LOADU_SRC_1                                          \
	LOADU_SRC(d1, sbuff)

#define	LOADU_SRC_2                                          \
	LOADU_SRC(d1, sbuff);                                    \
	LOADU_SRC(d2, sbuff)

#define	LOADU_SRC_3                                          \
	LOADU_SRC(d1, sbuff);                                    \
	LOADU_SRC(d2, sbuff);                                    \
	LOADU_SRC(d3, sbuff);                                    \

#define	LOADU_SRC_4                                          \
	LOADU_SRC_3

/* *********************************************************** */

#define	LOAD_SRC_NF(l)                                      \
	dt = _mm_loadl_epi64((__m128i *)sp);                 \
	dt = _mm_unpacklo_epi8(dt, mzero);                   \
	dt = _mm_slli_epi16(dt, 7);                          \
	_mm_storeu_si128(buffn + l, dt);                     \
	sp += 8;

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

#define	FUNC_SCONV7x7_8ext(NCHAN)                             \
	mlib_status mlib_s_sconv7x7_8ext_##NCHAN(              \
		mlib_image *dst, const mlib_image *src,            \
		mlib_s32 dx_l, mlib_s32 dx_r, mlib_s32 dy_t,       \
		mlib_s32 dy_b, const mlib_s32 *hkernel,            \
		const mlib_s32 *vkernel, mlib_s32 scalef_expon,    \
		mlib_s32 cmask)                                    \
	{                                                      \
	    __m128i *buff0, *buff1, *buff2, *buff3, *buff4,    \
			*buff5, *buffT, *pbuff, *sbuff;                \
		__m128i *tbuf, *buffs[2 * (KSIZE + 1)], *buffn;    \
		__m128i hker0, hker1, hker2, hker3,                \
			hker4, hker5, hker6;                           \
		__m128i vker0, vker1, vker2, vker3,                \
			vker4, vker5, vker6;                           \
	    __m128i d0, d1, d2, d3, dt,                        \
			sum0, sum1, sum2, sum3, sum4, sum5, sum6,      \
			s1, s2, s3, s4, s5, s6, res;                   \
	    __m128i mzero = _mm_setzero_si128();               \
	    mlib_s32 shift, dsize, ssize, esize;               \
	    mlib_s32 row, i, j, buff_ind = 0, gap;             \
	                                                       \
	    GET_SRC_DST_PARAMETERS(mlib_u8);                   \
	    GET_KERN();                                        \
		                                                   \
	    width *= NCHAN;                                    \
		ssize = (width + NCHAN * (KSIZE - 1) + 7) >> 3;    \
	    dsize = (width + 7) >> 3;                          \
		esize = ssize + 4;                                 \
		                                                   \
		pbuff = __mlib_malloc(sizeof (__m128i) *           \
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
			buffn = (__m128i *)(((mlib_s16 *)buffn) + gap);\
			for (j = 0; j < ssize; j++) {                  \
				LOAD_SRC_NF(j);                            \
			}                                              \
			buffn = tbuf;                                  \
			for (j = 0; j < dx_l; j++) {                   \
				EXT_LEFT_##NCHAN(                          \
					(mlib_s16 *)buffn, dx_l, j);           \
			}                                              \
			for (j = 0; j < dx_r; j++) {                   \
				EXT_RIGHT_##NCHAN(                         \
					(mlib_s16 *)buffn, (6 - dx_r), j);     \
			}                                              \
			if ((i >= dy_t) && (i < height + 5 - dy_b))    \
				sl += sll;                                 \
		}                                                  \
	    for (i = 0; i < (KSIZE - 1); i++) {                \
			__m128i **buffc = buffs + buff_ind;            \
                                                           \
			sbuff = buffc[0];                              \
			buffn = buffc[KSIZE];                          \
			tbuf = buffn;                                  \
			buffn = (__m128i *)(((mlib_s16 *)buffn) + gap);\
			                                               \
			sp = sl;                                       \
			                                               \
			LOADU_SRC_##NCHAN;                             \
			for (j = 0; j < dsize; j++) {                  \
				PREP_7x7_##NCHAN(j);                       \
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
					(mlib_s16 *)buffn, dx_l, j);           \
			}                                              \
			for (j = 0; j < dx_r; j++) {                   \
				EXT_RIGHT_##NCHAN(                         \
				(mlib_s16 *)buffn, (6 - dx_r), j);         \
			}                                              \
			                                               \
			if (i < height - dy_b - 2)                     \
				sl += sll;                                 \
			buff_ind++;                                    \
		}                                                  \
		for (row = 0; row < height; row++) {               \
			__m128i **buffc = buffs + buff_ind;            \
                                                           \
			sbuff = buffc[0];                              \
			buffn = buffc[KSIZE];                          \
			tbuf = buffn;                                  \
			buffn = (__m128i *)(((mlib_s16 *)buffn) + gap);\
                                                           \
			sp = sl;                                       \
			dp = dl;                                       \
			                                               \
			LOADU_SRC_##NCHAN;                             \
			for (i = 0; i < (width >> 3); i++) {           \
				LOAD_SRC_NF(i);                            \
				CONV_7x7(NCHAN, i);                        \
				_mm_storel_epi64((void *)dp, res);         \
				dp += 8;                                   \
			}                                              \
			if (width & 7) {                               \
				__m128i mask, dd;                          \
	                                                       \
				LOAD_SRC_NF(i);                            \
				CONV_7x7(NCHAN, i);                        \
				mask = _mm_loadl_epi64((void *)            \
					(mlib_mask64_arr + (width & 7)));      \
				dd = _mm_loadl_epi64((void *)dp);          \
				dd = _mm_or_si128(_mm_and_si128(mask, res),\
					_mm_andnot_si128(mask, dd));           \
					_mm_storel_epi64((void *)dp, dd);      \
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
					(mlib_s16 *)buffn, dx_l, j);           \
			}                                              \
			for (j = 0; j < dx_r; j++) {                   \
				EXT_RIGHT_##NCHAN(                         \
					(mlib_s16 *)buffn, (6 - dx_r), j);     \
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

FUNC_SCONV7x7_8ext(1)
FUNC_SCONV7x7_8ext(2)
FUNC_SCONV7x7_8ext(3)
FUNC_SCONV7x7_8ext(4)

/* *********************************************************** */
