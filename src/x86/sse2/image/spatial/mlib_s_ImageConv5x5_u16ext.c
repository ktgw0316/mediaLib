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

#pragma ident   "@(#)mlib_s_ImageConv5x5_u16ext.c	9.3	07/11/05 SMI"

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

#define	sse_mul_16x16(x, y)                                    \
	_mm_unpacklo_epi16(_mm_mullo_epi16(x, y), _mm_mulhi_epi16(x, y))

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
	shift = scale - 16;                                     \
	ker_sum = 0;                                            \
	for (j = 0; j < 5; j++) {                               \
	    for (i = 0; i < 5; i++) {                           \
			mlib_s32 kk = kern[5 * j + i] >> 16;            \
	                                                        \
			ker[j][i] = _mm_set1_epi16(kk);                 \
			ker_sum += kk;                                  \
	    }                                                   \
	}                                                       \
	                                                        \
	if (shift <= 15) {                                      \
	    ker_sum <<= (15 - shift);                           \
	} else {                                                \
	    ker_sum >>= (shift - 15);                           \
	}                                                       \
	                                                        \
	ker_off = _mm_set1_epi32(ker_sum - 0x8000);             \
	mask8000 = _mm_set1_epi16(0x8000)

/* *********************************************************** */

#define	PREP_5x5(NCHAN, l)                                       \
	d0 = _mm_loadl_epi64((void *)sbuff);                      \
	sum0 = sse_mul_16x16(d0, ker[0][0]);                      \
	sum1 = sse_mul_16x16(d0, ker[1][0]);                      \
	sum2 = sse_mul_16x16(d0, ker[2][0]);                      \
	sum3 = sse_mul_16x16(d0, ker[3][0]);                      \
	aa = _mm_loadl_epi64((void *)(sbuff + NCHAN));            \
	sum0 = _mm_add_epi32(sum0, sse_mul_16x16(aa, ker[0][1])); \
	sum1 = _mm_add_epi32(sum1, sse_mul_16x16(aa, ker[1][1])); \
	sum2 = _mm_add_epi32(sum2, sse_mul_16x16(aa, ker[2][1])); \
	sum3 = _mm_add_epi32(sum3, sse_mul_16x16(aa, ker[3][1])); \
	bb = _mm_loadl_epi64((void *)(sbuff + 2 * NCHAN));        \
	sum0 = _mm_add_epi32(sum0, sse_mul_16x16(bb, ker[0][2])); \
	sum1 = _mm_add_epi32(sum1, sse_mul_16x16(bb, ker[1][2])); \
	sum2 = _mm_add_epi32(sum2, sse_mul_16x16(bb, ker[2][2])); \
	sum3 = _mm_add_epi32(sum3, sse_mul_16x16(bb, ker[3][2])); \
	cc = _mm_loadl_epi64((void *)(sbuff + 3 * NCHAN));        \
	sum0 = _mm_add_epi32(sum0, sse_mul_16x16(cc, ker[0][3])); \
	sum1 = _mm_add_epi32(sum1, sse_mul_16x16(cc, ker[1][3])); \
	sum2 = _mm_add_epi32(sum2, sse_mul_16x16(cc, ker[2][3])); \
	sum3 = _mm_add_epi32(sum3, sse_mul_16x16(cc, ker[3][3])); \
	dd = _mm_loadl_epi64((void *)(sbuff + 4 * NCHAN));        \
	sum0 = _mm_add_epi32(sum0, sse_mul_16x16(dd, ker[0][4])); \
	sum1 = _mm_add_epi32(sum1, sse_mul_16x16(dd, ker[1][4])); \
	sum2 = _mm_add_epi32(sum2, sse_mul_16x16(dd, ker[2][4])); \
	sum3 = _mm_add_epi32(sum3, sse_mul_16x16(dd, ker[3][4])); \
	_mm_store_si128(buff0 + l, sum0);                         \
	_mm_store_si128(buff1 + l, sum1);                         \
	_mm_store_si128(buff2 + l, sum2);                         \
	_mm_store_si128(buff3 + l, sum3);                         \
	sbuff += 4;                                               \

/* *********************************************************** */

#define	CONV_5x5(NCHAN, l)                                    \
	d0 = _mm_loadl_epi64((void *)sbuff);                      \
	sum0 = sse_mul_16x16(d0, ker[0][0]);                      \
	sum1 = sse_mul_16x16(d0, ker[1][0]);                      \
	sum2 = sse_mul_16x16(d0, ker[2][0]);                      \
	sum3 = sse_mul_16x16(d0, ker[3][0]);                      \
	sum4 = sse_mul_16x16(d0, ker[4][0]);                      \
	aa = _mm_loadl_epi64((void *)(sbuff + NCHAN));            \
	sum0 = _mm_add_epi32(sum0, sse_mul_16x16(aa, ker[0][1])); \
	sum1 = _mm_add_epi32(sum1, sse_mul_16x16(aa, ker[1][1])); \
	sum2 = _mm_add_epi32(sum2, sse_mul_16x16(aa, ker[2][1])); \
	sum3 = _mm_add_epi32(sum3, sse_mul_16x16(aa, ker[3][1])); \
	sum4 = _mm_add_epi32(sum4, sse_mul_16x16(aa, ker[4][1])); \
	bb = _mm_loadl_epi64((void *)(sbuff + 2 * NCHAN));        \
	sum0 = _mm_add_epi32(sum0, sse_mul_16x16(bb, ker[0][2])); \
	sum1 = _mm_add_epi32(sum1, sse_mul_16x16(bb, ker[1][2])); \
	sum2 = _mm_add_epi32(sum2, sse_mul_16x16(bb, ker[2][2])); \
	sum3 = _mm_add_epi32(sum3, sse_mul_16x16(bb, ker[3][2])); \
	sum4 = _mm_add_epi32(sum4, sse_mul_16x16(bb, ker[4][2])); \
	cc = _mm_loadl_epi64((void *)(sbuff + 3 * NCHAN));        \
	sum0 = _mm_add_epi32(sum0, sse_mul_16x16(cc, ker[0][3])); \
	sum1 = _mm_add_epi32(sum1, sse_mul_16x16(cc, ker[1][3])); \
	sum2 = _mm_add_epi32(sum2, sse_mul_16x16(cc, ker[2][3])); \
	sum3 = _mm_add_epi32(sum3, sse_mul_16x16(cc, ker[3][3])); \
	sum4 = _mm_add_epi32(sum4, sse_mul_16x16(cc, ker[4][3])); \
	dd = _mm_loadl_epi64((void *)(sbuff + 4 * NCHAN));        \
	sum0 = _mm_add_epi32(sum0, sse_mul_16x16(dd, ker[0][4])); \
	sum1 = _mm_add_epi32(sum1, sse_mul_16x16(dd, ker[1][4])); \
	sum2 = _mm_add_epi32(sum2, sse_mul_16x16(dd, ker[2][4])); \
	sum3 = _mm_add_epi32(sum3, sse_mul_16x16(dd, ker[3][4])); \
	sum4 = _mm_add_epi32(sum4, sse_mul_16x16(dd, ker[4][4])); \
	prev0 = _mm_load_si128(buff0 + l);                        \
	prev1 = _mm_load_si128(buff1 + l);                        \
	prev2 = _mm_load_si128(buff2 + l);                        \
	prev3 = _mm_load_si128(buff3 + l);                        \
	sum4 = _mm_add_epi32(sum4, prev0);                        \
	sum4 = _mm_add_epi32(sum4, prev1);                        \
	sum4 = _mm_add_epi32(sum4, prev2);                        \
	sum4 = _mm_add_epi32(sum4, prev3);                        \
	_mm_store_si128(buff0 + l, sum0);                         \
	_mm_store_si128(buff1 + l, sum1);                         \
	_mm_store_si128(buff2 + l, sum2);                         \
	_mm_store_si128(buff3 + l, sum3);                         \
	sbuff += 4;                                               \
	res = _mm_srai_epi32(sum4, shift);                        \
	res = _mm_add_epi32(res, ker_off);                        \
	res = _mm_packs_epi32(res, res);                          \
	res = _mm_xor_si128(res, mask8000)

/* *********************************************************** */

#define	LOAD_SRC_NF(l)                                      \
	_mm_storel_epi64((__m128i *)((mlib_d64 *)buffn + l),  \
		_mm_xor_si128(_mm_loadl_epi64(                    \
		(__m128i *)sp), mask8000));                       \
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
#define	KSIZE	5

/* *********************************************************** */

#define	FUNC_CONV5x5_u16ext(NCHAN)                             \
	mlib_status                                            \
	mlib_s_conv5x5_u16ext_##NCHAN(                          \
		mlib_image *dst, const mlib_image *src,            \
		mlib_s32 dx_l, mlib_s32 dx_r, mlib_s32 dy_t,       \
		mlib_s32 dy_b, const mlib_s32 *kern,               \
		mlib_s32 scale, mlib_s32 cmask)                    \
	{                                                      \
	    __m128i *buff0, *buff1, *buff2, *buff3, *pbuff;    \
		__m128i *tbuf, *buffs[2 * (KSIZE + 1)], *buffn,    \
			*buff_arr[20], **pbuff_arr = buff_arr;         \
	    __m128i ker[5][5];                                 \
	    __m128i s0, d0, prev0, prev1, prev2, prev3,        \
			sum0, sum1, sum2, sum3, sum4,                  \
			aa, bb, cc, dd, res;                           \
		__m128i ker_off, mask8000;                         \
	    mlib_s32 shift, dsize, ssize, esize, ker_sum;      \
	    mlib_s32 row, i, j, buff_ind = 0, ind = 0, gap;    \
	                                                       \
	    GET_SRC_DST_PARAMETERS(mlib_s16);                  \
	    GET_KERN();                                        \
		                                                   \
	    width *= NCHAN;                                    \
		ssize = (width + NCHAN * (KSIZE - 1) + 3) >> 2;    \
	    dsize = (width + 3) >> 2;                          \
		esize = ssize + 4;                                 \
		                                                   \
		pbuff = __mlib_malloc(sizeof (__m128i) *           \
			(KSIZE + 12) * esize);                         \
		if (pbuff == NULL)                                 \
			return (MLIB_FAILURE);                         \
		                                                   \
		for (i = 0; i < (KSIZE + 1); i++) {                \
			buffs[i] = pbuff + i * esize;                  \
		}                                                  \
		for (i = 0; i < (KSIZE + 1); i++) {                \
			buffs[(KSIZE + 1) + i] = buffs[i];             \
		}                                                  \
		for (i = 0; i < 10; i++) {                         \
			buff_arr[i] = buffs[KSIZE] + (i + 1) * esize;  \
		}                                                  \
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
					(mlib_s16 *)buffn, (4 - dx_r), j);     \
			}                                              \
			if ((i >= dy_t) && (i < height + 3 - dy_b))    \
				sl += sll;                                 \
		}                                                  \
	    for (i = 1; i <= 4; i++) {                         \
			__m128i **buffc = buffs + buff_ind;            \
                                                           \
			sbuff = (mlib_s16 *)(buffc[0]);                \
			buffn = buffc[KSIZE];                          \
			tbuf = buffn;                                  \
			buffn = (__m128i *)(((mlib_s16 *)buffn) + gap);\
			sp = sl;                                       \
			                                               \
			buff0 = buff_arr[ind];                         \
			buff1 = buff_arr[ind + 1];                     \
			buff2 = buff_arr[ind + 2];                     \
			buff3 = buff_arr[ind + 3];                     \
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
					(mlib_s16 *)buffn, dx_l, j);           \
			}                                              \
			for (j = 0; j < dx_r; j++) {                   \
				EXT_RIGHT_##NCHAN(                         \
				(mlib_s16 *)buffn, (4 - dx_r), j);         \
			}                                              \
		                                                   \
			if (i < height - dy_b - 1)                     \
				sl += sll;                                 \
			buff_ind++;                                    \
			ind += i;                                      \
		}                                                  \
		for (row = 0; row < height; row++) {               \
			__m128i **buffc = buffs + buff_ind;            \
                                                           \
			sbuff = (mlib_s16 *)(buffc[0]);                \
			buffn = buffc[KSIZE];                          \
			tbuf = buffn;                                  \
			buffn = (__m128i *)(((mlib_s16 *)buffn) + gap);\
			                                               \
			buff0 = pbuff_arr[0];                          \
			buff1 = pbuff_arr[2];                          \
			buff2 = pbuff_arr[5];                          \
			buff3 = pbuff_arr[9];                          \
			                                               \
			sp = sl;                                       \
			dp = dl;                                       \
			                                               \
			for (i = 0; i < (width >> 2); i++) {           \
				CONV_5x5(NCHAN, i);                        \
				LOAD_SRC_NF(i);                            \
				_mm_storel_epi64((void *)dp, res);         \
				dp += 4;                                   \
			}                                              \
			if (width & 3) {                               \
				__m128i mask, dd;                          \
	                                                       \
				CONV_5x5(NCHAN, i);                        \
				LOAD_SRC_NF(i);                            \
				mask = _mm_loadl_epi64((void *)            \
					(mlib_mask64_arr + 2 * (width & 3)));  \
				dd = _mm_loadl_epi64((void *)dp);          \
				dd = _mm_or_si128(_mm_and_si128(mask, res),\
					_mm_andnot_si128(mask, dd));           \
				_mm_storel_epi64((void *)dp, dd);          \
				i++;                                       \
			}                                              \
			                                               \
			ind = (pbuff_arr == buff_arr) ? 10 : -10;      \
			pbuff_arr[ind + 0] = pbuff_arr[1];             \
			pbuff_arr[ind + 1] = pbuff_arr[3];             \
			pbuff_arr[ind + 2] = pbuff_arr[4];             \
			pbuff_arr[ind + 3] = pbuff_arr[6];             \
			pbuff_arr[ind + 4] = pbuff_arr[7];             \
			pbuff_arr[ind + 5] = pbuff_arr[8];             \
			pbuff_arr[ind + 6] = pbuff_arr[0];             \
			pbuff_arr[ind + 7] = pbuff_arr[2];             \
			pbuff_arr[ind + 8] = pbuff_arr[5];             \
			pbuff_arr[ind + 9] = pbuff_arr[9];             \
			pbuff_arr += ind;                              \
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
					(mlib_s16 *)buffn, (4 - dx_r), j);     \
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

FUNC_CONV5x5_u16ext(1)
FUNC_CONV5x5_u16ext(2)
FUNC_CONV5x5_u16ext(3)
FUNC_CONV5x5_u16ext(4)

/* *********************************************************** */
