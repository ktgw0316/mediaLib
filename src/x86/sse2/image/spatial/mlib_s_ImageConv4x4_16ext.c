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

#pragma ident   "@(#)mlib_s_ImageConv4x4_16ext.c	9.3	07/11/05 SMI"

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

#define	GET_KERN()                                               \
	ker1 = _mm_set1_epi16((kern[0] >> 16) & 0xFFFF);          \
	ker2 = _mm_set1_epi16((kern[1] >> 16) & 0xFFFF);          \
	ker3 = _mm_set1_epi16((kern[2] >> 16) & 0xFFFF);          \
	ker4 = _mm_set1_epi16((kern[3] >> 16) & 0xFFFF);          \
	ker5 = _mm_set1_epi16((kern[4] >> 16) & 0xFFFF);          \
	ker6 = _mm_set1_epi16((kern[5] >> 16) & 0xFFFF);          \
	ker7 = _mm_set1_epi16((kern[6] >> 16) & 0xFFFF);          \
	ker8 = _mm_set1_epi16((kern[7] >> 16) & 0xFFFF);          \
	ker9 = _mm_set1_epi16((kern[8] >> 16) & 0xFFFF);          \
	ker10 = _mm_set1_epi16((kern[9] >> 16) & 0xFFFF);         \
	ker11 = _mm_set1_epi16((kern[10] >> 16) & 0xFFFF);        \
	ker12 = _mm_set1_epi16((kern[11] >> 16) & 0xFFFF);        \
	ker13 = _mm_set1_epi16((kern[12] >> 16) & 0xFFFF);        \
	ker14 = _mm_set1_epi16((kern[13] >> 16) & 0xFFFF);        \
	ker15 = _mm_set1_epi16((kern[14] >> 16) & 0xFFFF);        \
	ker16 = _mm_set1_epi16((kern[15] >> 16) & 0xFFFF);        \
	shift = scale - 16;                                       \

/* *********************************************************** */

#define	PREP_4x4(NCHAN, l)                                  \
	d0 = _mm_loadl_epi64((void *)sbuff);                   \
	sum0 = sse_mul_16x16(d0, ker1);                        \
	sum1 = sse_mul_16x16(d0, ker5);                        \
	sum2 = sse_mul_16x16(d0, ker9);                        \
	aa = _mm_loadl_epi64((void *)(sbuff + NCHAN));         \
	sum0 = _mm_add_epi32(sum0, sse_mul_16x16(aa, ker2));   \
	sum1 = _mm_add_epi32(sum1, sse_mul_16x16(aa, ker6));   \
	sum2 = _mm_add_epi32(sum2, sse_mul_16x16(aa, ker10));  \
	bb = _mm_loadl_epi64((void *)(sbuff + 2 * NCHAN));     \
	sum0 = _mm_add_epi32(sum0, sse_mul_16x16(bb, ker3));   \
	sum1 = _mm_add_epi32(sum1, sse_mul_16x16(bb, ker7));   \
	sum2 = _mm_add_epi32(sum2, sse_mul_16x16(bb, ker11));  \
	cc = _mm_loadl_epi64((void *)(sbuff + 3 * NCHAN));     \
	sum0 = _mm_add_epi32(sum0, sse_mul_16x16(cc, ker4));   \
	sum1 = _mm_add_epi32(sum1, sse_mul_16x16(cc, ker8));   \
	sum2 = _mm_add_epi32(sum2, sse_mul_16x16(cc, ker12));  \
	_mm_store_si128(buff0 + l, sum0);                      \
	_mm_store_si128(buff1 + l, sum1);                      \
	_mm_store_si128(buff2 + l, sum2);                      \
	sbuff += 4;                                            \

/* *********************************************************** */

#define	CONV_4x4(NCHAN, l)                                  \
	d0 = _mm_loadl_epi64((void *)sbuff);                   \
	sum0 = sse_mul_16x16(d0, ker1);                        \
	sum1 = sse_mul_16x16(d0, ker5);                        \
	sum2 = sse_mul_16x16(d0, ker9);                        \
	sum3 = sse_mul_16x16(d0, ker13);                       \
	aa = _mm_loadl_epi64((void *)(sbuff + NCHAN));         \
	sum0 = _mm_add_epi32(sum0, sse_mul_16x16(aa, ker2));   \
	sum1 = _mm_add_epi32(sum1, sse_mul_16x16(aa, ker6));   \
	sum2 = _mm_add_epi32(sum2, sse_mul_16x16(aa, ker10));  \
	sum3 = _mm_add_epi32(sum3, sse_mul_16x16(aa, ker14));  \
	bb = _mm_loadl_epi64((void *)(sbuff + 2 * NCHAN));     \
	sum0 = _mm_add_epi32(sum0, sse_mul_16x16(bb, ker3));   \
	sum1 = _mm_add_epi32(sum1, sse_mul_16x16(bb, ker7));   \
	sum2 = _mm_add_epi32(sum2, sse_mul_16x16(bb, ker11));  \
	sum3 = _mm_add_epi32(sum3, sse_mul_16x16(bb, ker15));  \
	cc = _mm_loadl_epi64((void *)(sbuff + 3 * NCHAN));     \
	sum0 = _mm_add_epi32(sum0, sse_mul_16x16(cc, ker4));   \
	sum1 = _mm_add_epi32(sum1, sse_mul_16x16(cc, ker8));   \
	sum2 = _mm_add_epi32(sum2, sse_mul_16x16(cc, ker12));  \
	sum3 = _mm_add_epi32(sum3, sse_mul_16x16(cc, ker16));  \
	prev0 = _mm_load_si128(buff0 + l);                     \
	prev1 = _mm_load_si128(buff1 + l);                     \
	prev2 = _mm_load_si128(buff2 + l);                     \
	sum3 = _mm_add_epi32(sum3, prev0);                     \
	sum3 = _mm_add_epi32(sum3, prev1);                     \
	sum3 = _mm_add_epi32(sum3, prev2);                     \
	_mm_store_si128(buff0 + l, sum0);                      \
	_mm_store_si128(buff1 + l, sum1);                      \
	_mm_store_si128(buff2 + l, sum2);                      \
	sbuff += 4;                                            \
	res = _mm_srai_epi32(sum3, shift);                     \
	res = _mm_packs_epi32(res, res);                       \

/* *********************************************************** */

#define	LOAD_SRC_NF(l)                                      \
	((mlib_d64 *)buffn)[l] = *((mlib_d64 *)sp);           \
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
#define	KSIZE	4

/* *********************************************************** */

#define	FUNC_CONV4x4_16ext(NCHAN)                             \
	mlib_status                                            \
	mlib_s_conv4x4_16ext_##NCHAN(                          \
		mlib_image *dst, const mlib_image *src,            \
		mlib_s32 dx_l, mlib_s32 dx_r, mlib_s32 dy_t,       \
		mlib_s32 dy_b, const mlib_s32 *kern,               \
		mlib_s32 scale, mlib_s32 cmask)                    \
	{                                                      \
	    __m128i *buff0, *buff1, *buff2, *pbuff;            \
		__m128i *tbuf, *buffs[2 * (KSIZE + 1)], *buffn,    \
			*buff_arr[12], **pbuff_arr = buff_arr;         \
	    __m128i ker1, ker2, ker3, ker4, ker5,              \
			ker6, ker7, ker8, ker9, ker10, ker11,          \
			ker12, ker13, ker14, ker15, ker16;             \
	    __m128i s0, d0, prev0, prev1, prev2,               \
			sum0, sum1, sum2, sum3, aa, bb, cc, res;       \
	    mlib_s32 shift, dsize, ssize, esize;               \
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
		for (i = 0; i < 6; i++) {                          \
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
					(mlib_s16 *)buffn, (3 - dx_r), j);     \
			}                                              \
			if ((i >= dy_t) && (i < height + 2 - dy_b))    \
				sl += sll;                                 \
		}                                                  \
	    for (i = 1; i <= 3; i++) {                         \
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
			                                               \
			for (j = 0; j < dsize; j++) {                  \
				PREP_4x4(NCHAN, j);                        \
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
				(mlib_s16 *)buffn, (3 - dx_r), j);         \
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
			                                               \
			sp = sl;                                       \
			dp = dl;                                       \
			                                               \
			for (i = 0; i < (width >> 2); i++) {           \
				CONV_4x4(NCHAN, i);                        \
				LOAD_SRC_NF(i);                            \
				_mm_storel_epi64((void *)dp, res);         \
				dp += 4;                                   \
			}                                              \
			if (width & 3) {                               \
				__m128i mask, dd;                          \
	                                                       \
				CONV_4x4(NCHAN, i);                        \
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
			ind = (pbuff_arr == buff_arr) ? 6 : -6;        \
			pbuff_arr[ind + 0] = pbuff_arr[1];             \
			pbuff_arr[ind + 1] = pbuff_arr[3];             \
			pbuff_arr[ind + 2] = pbuff_arr[4];             \
			pbuff_arr[ind + 3] = pbuff_arr[0];             \
			pbuff_arr[ind + 4] = pbuff_arr[2];             \
			pbuff_arr[ind + 5] = pbuff_arr[5];             \
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
					(mlib_s16 *)buffn, (3 - dx_r), j);     \
			}                                              \
	                                                       \
			if (row < height - dy_b - 5)                   \
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

FUNC_CONV4x4_16ext(1)
FUNC_CONV4x4_16ext(2)
FUNC_CONV4x4_16ext(3)
FUNC_CONV4x4_16ext(4)

/* *********************************************************** */
