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

#pragma ident   "@(#)mlib_s_ImageSConv5x5_u16ext.c	9.3	07/11/05 SMI"

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

#undef KSIZE
#define	KSIZE	5

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
	vker0 = _mm_set1_epi16((vkernel[0] >> 16) & 0xFFFF);    \
	vker1 = _mm_set1_epi16((vkernel[1] >> 16) & 0xFFFF);    \
	vker2 = _mm_set1_epi16((vkernel[2] >> 16) & 0xFFFF);    \
	vker3 = _mm_set1_epi16((vkernel[3] >> 16) & 0xFFFF);    \
	vker4 = _mm_set1_epi16((vkernel[4] >> 16) & 0xFFFF);    \
	shift = 2 * scalef_expon - (32 + 32 - 16);              \
	for (i = 0; i < KSIZE; i++) {                           \
		kerh_sum += (hkernel[i] >> 16);                     \
		kerv_sum += (vkernel[i] >> 16);                     \
	}                                                       \
	                                                        \
	ker_sum = (mlib_d64)kerh_sum *                          \
		kerv_sum / (1 << (shift + 1));                      \
	                                                        \
	ker_off = _mm_set1_epi32(ker_sum - 0x8000);             \
	rnd = _mm_set1_epi16(KSIZE / 2);                        \
	mask8000 = _mm_set1_epi16(0x8000)


/* *********************************************************** */
#define	PREP_5x5(NCHAN, l)                                       \
	d0 = d1;                                                 \
	LOADU_SRC(d1, sbuff);                                    \
	sum4 = _mm_add_epi16(rnd, _mm_mulhi_epi16(d0, hker0));   \
	aa = ALGN(d0, d1, 2 * NCHAN);                            \
	sum4 = _mm_add_epi16(sum4, _mm_mulhi_epi16(aa, hker1));  \
	bb = ALGN(d0, d1, 4 * NCHAN);                            \
	sum4 = _mm_add_epi16(sum4, _mm_mulhi_epi16(bb, hker2));  \
	cc = ALGN(d0, d1, 6 * NCHAN);                            \
	sum4 = _mm_add_epi16(sum4, _mm_mulhi_epi16(cc, hker3));  \
	dd = ALGN(d0, d1, 8 * NCHAN);                            \
	sum4 = _mm_add_epi16(sum4, _mm_mulhi_epi16(dd, hker4));  \
	_mm_store_si128(buff0 + l, sum4)

/* *********************************************************** */
#define	CONV_5x5(NCHAN, l)                                      \
	sum0 = _mm_load_si128(buff0 + l);                        \
	sum1 = _mm_load_si128(buff1 + l);                        \
	sum2 = _mm_load_si128(buff2 + l);                        \
	sum3 = _mm_load_si128(buff3 + l);                        \
	PREP_5x5(NCHAN, l);                                      \
	res = _mm_add_epi16(rnd, _mm_mulhi_epi16(sum0, vker0));  \
	res = _mm_add_epi16(res, _mm_mulhi_epi16(sum1, vker1));  \
	res = _mm_add_epi16(res, _mm_mulhi_epi16(sum2, vker2));  \
	res = _mm_add_epi16(res, _mm_mulhi_epi16(sum3, vker3));  \
	res = _mm_add_epi16(res, _mm_mulhi_epi16(sum4, vker4));  \
	rl = _mm_unpacklo_epi16(mzero, res);                     \
	rh = _mm_unpackhi_epi16(mzero, res);                     \
	rl = _mm_srai_epi32(rl, shift);                          \
	rh = _mm_srai_epi32(rh, shift);                          \
	rl = _mm_add_epi32(rl, ker_off);                         \
	rh = _mm_add_epi32(rh, ker_off);                         \
	res = _mm_packs_epi32(rl, rh);                           \
	res = _mm_xor_si128(res, mask8000)


/* *********************************************************** */
#define	LOADU_SRC(ds, tp)                                   \
	ds = _mm_load_si128(tp);                             \
	tp++;                                                \

/* *********************************************************** */

#define	LOAD_SRC_NF(l)                                      \
	dt = _mm_loadu_si128((__m128i *)sp);                 \
	dt = _mm_xor_si128(dt, mask8000);                    \
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

#define	FUNC_SCONV5x5_u16ext(NCHAN)                             \
	mlib_status mlib_s_sconv5x5_u16ext_##NCHAN(            \
		mlib_image *dst, const mlib_image *src,            \
		mlib_s32 dx_l, mlib_s32 dx_r, mlib_s32 dy_t,       \
		mlib_s32 dy_b, const mlib_s32 *hkernel,            \
		const mlib_s32 *vkernel, mlib_s32 scalef_expon,    \
		mlib_s32 cmask)                                    \
	{                                                      \
	    __m128i *buff0, *buff1, *buff2,                    \
			*buff3, *buffT, *pbuff, *sbuff;                \
		__m128i *tbuf, *buffs[2 * (KSIZE + 1)], *buffn;    \
		__m128i hker0, hker1, hker2, hker3, hker4,         \
			vker0, vker1, vker2, vker3, vker4;             \
	    __m128i s0, d0, d1, dt,                            \
			sum0, sum1, sum2, sum3, sum4,                  \
			aa, bb, cc, dd, rh, rl, res, mask;             \
	    __m128i mzero = _mm_setzero_si128();               \
		__m128i ker_off, mask8000, rnd;                    \
	    mlib_s32 shift, dsize, ssize, esize, emask,        \
			ker_sum, kerh_sum = 0, kerv_sum = 0;           \
	    mlib_s32 row, i, j, buff_ind = 0, gap;             \
	                                                       \
	    GET_SRC_DST_PARAMETERS(mlib_s16);                  \
	    GET_KERN();                                        \
		                                                   \
	    width *= NCHAN;                                    \
		ssize = (width + NCHAN * (KSIZE - 1) + 7) >> 3;    \
	    dsize = (width + 7) >> 3;                          \
		esize = ssize + 4;                                 \
		emask = (0xFF00 >> (width & 7)) & 0xFF;            \
		mask = LOAD_2x64(mask_pst_16,                      \
			(emask >> 4) & 0xf, emask & 0xf);              \
		                                                   \
		pbuff = __mlib_malloc(sizeof (__m128i) *           \
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
			LOADU_SRC(d1, sbuff);                          \
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
			LOADU_SRC(d1, sbuff);                          \
			for (i = 0; i < (width >> 3); i++) {           \
				LOAD_SRC_NF(i);                            \
				CONV_5x5(NCHAN, i);                        \
				_mm_storeu_si128((void *)dp, res);         \
				dp += 8;                                   \
			}                                              \
			if (width & 7) {                               \
				__m128i dd;                                \
	                                                       \
				LOAD_SRC_NF(i);                            \
				CONV_5x5(NCHAN, i);                        \
				dd = _mm_loadu_si128((void *)dp);          \
				dd = _mm_or_si128(_mm_and_si128(mask, res),\
					_mm_andnot_si128(mask, dd));           \
				_mm_storeu_si128((void *)dp, dd);          \
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

FUNC_SCONV5x5_u16ext(1)
FUNC_SCONV5x5_u16ext(2)

/* *********************************************************** */

#undef PREP_5x5
#define	PREP_5x5(NCHAN, l)                                       \
	d0 = d1;                                                 \
	d1 = d2;                                                 \
	LOADU_SRC(d2, sbuff);                                    \
	sum4 = _mm_mulhi_epi16(d0, hker0);                       \
	aa = ALGN(d0, d1, 2 * NCHAN);                            \
	sum4 = _mm_add_epi16(sum4, _mm_mulhi_epi16(aa, hker1));  \
	bb = ALGN(d0, d1, 4 * NCHAN);                            \
	sum4 = _mm_add_epi16(sum4, _mm_mulhi_epi16(bb, hker2));  \
	cc = ALGN(d1, d2, 6 * NCHAN - 16);                       \
	sum4 = _mm_add_epi16(sum4, _mm_mulhi_epi16(cc, hker3));  \
	dd = ALGN(d1, d2, 8 * NCHAN - 16);                       \
	sum4 = _mm_add_epi16(sum4, _mm_mulhi_epi16(dd, hker4));  \
	_mm_store_si128(buff0 + l, sum4)

/* *********************************************************** */

#undef FUNC_SCONV5x5_u16ext
#define	FUNC_SCONV5x5_u16ext(NCHAN)                             \
	mlib_status                                            \
	mlib_s_sconv5x5_u16ext_##NCHAN(                          \
		mlib_image *dst, const mlib_image *src,            \
		mlib_s32 dx_l, mlib_s32 dx_r, mlib_s32 dy_t,       \
		mlib_s32 dy_b, const mlib_s32 *hkernel,            \
		const mlib_s32 *vkernel, mlib_s32 scalef_expon,    \
		mlib_s32 cmask)                                    \
	{                                                      \
	    __m128i *buff0, *buff1, *buff2,                    \
			*buff3, *buffT, *pbuff, *sbuff;                \
		__m128i *tbuf, *buffs[2 * (KSIZE + 1)], *buffn;    \
		__m128i hker0, hker1, hker2, hker3, hker4,         \
			vker0, vker1, vker2, vker3, vker4;             \
	    __m128i s0, d0, d1, d2, dt,                        \
			sum0, sum1, sum2, sum3, sum4,                  \
			aa, bb, cc, dd, rh, rl, res, mask;             \
		__m128i ker_off, mask8000, rnd;                    \
	    __m128i mzero = _mm_setzero_si128();               \
	    mlib_s32 shift, dsize, ssize, esize, emask,        \
			ker_sum, kerh_sum = 0, kerv_sum = 0;           \
	    mlib_s32 row, i, j, buff_ind = 0, gap;             \
	                                                       \
	    GET_SRC_DST_PARAMETERS(mlib_s16);                  \
	    GET_KERN();                                        \
		                                                   \
	    width *= NCHAN;                                    \
		ssize = (width + NCHAN * (KSIZE - 1) + 7) >> 3;    \
	    dsize = (width + 7) >> 3;                          \
		esize = ssize + 4;                                 \
		emask = (0xFF00 >> (width & 7)) & 0xFF;            \
		mask = LOAD_2x64(mask_pst_16,                      \
			(emask >> 4) & 0xf, emask & 0xf);              \
		                                                   \
		pbuff = __mlib_malloc(sizeof (__m128i) *           \
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
			LOADU_SRC(d1, sbuff);                          \
			LOADU_SRC(d2, sbuff);                          \
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
			LOADU_SRC(d1, sbuff);                          \
			LOADU_SRC(d2, sbuff);                          \
			for (i = 0; i < (width >> 3); i++) {           \
				LOAD_SRC_NF(i);                            \
				CONV_5x5(NCHAN, i);                        \
				_mm_storeu_si128((void *)dp, res);         \
				dp += 8;                                   \
			}                                              \
			if (width & 7) {                               \
				__m128i dd;                                \
	                                                       \
				LOAD_SRC_NF(i);                            \
				CONV_5x5(NCHAN, i);                        \
				dd = _mm_loadu_si128((void *)dp);          \
				dd = _mm_or_si128(_mm_and_si128(mask, res),\
					_mm_andnot_si128(mask, dd));           \
				_mm_storeu_si128((void *)dp, dd);          \
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

FUNC_SCONV5x5_u16ext(3)
FUNC_SCONV5x5_u16ext(4)

/* *********************************************************** */
