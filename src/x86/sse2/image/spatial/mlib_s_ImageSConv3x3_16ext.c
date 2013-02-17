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

#pragma ident   "@(#)mlib_s_ImageSConv3x3_16ext.c	9.3	07/11/05 SMI"

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
	_mm_or_si128(_mm_srli_si128(x, sh), _mm_slli_si128(y, 16 - sh))

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
#define	GET_KERN()                                              \
	hker0 = _mm_set1_epi16((hkernel[0] >> 16) & 0xFFFF);    \
	hker1 = _mm_set1_epi16((hkernel[1] >> 16) & 0xFFFF);    \
	hker2 = _mm_set1_epi16((hkernel[2] >> 16) & 0xFFFF);    \
	vker0 = _mm_set1_epi16((vkernel[0] >> 16) & 0xFFFF);    \
	vker1 = _mm_set1_epi16((vkernel[1] >> 16) & 0xFFFF);    \
	vker2 = _mm_set1_epi16((vkernel[2] >> 16) & 0xFFFF);    \
	shift = 2 * scale - (32 + 32 - 16)

/* *********************************************************** */
#define	PREP_3x3(NCHAN, l)                                      \
	d0 = d1;                                                \
	LOADU_SRC(d1, sbuff);                                   \
	sum2 = _mm_mulhi_epi16(d0, hker0);                      \
	aa = ALGN(d0, d1, 2 * NCHAN);                           \
	sum2 = _mm_add_epi16(sum2, _mm_mulhi_epi16(aa, hker1)); \
	bb = ALGN(d0, d1, 4 * NCHAN);                           \
	sum2 = _mm_add_epi16(sum2, _mm_mulhi_epi16(bb, hker2)); \
	_mm_store_si128(buff0 + l, sum2);                       \

/* *********************************************************** */
#define	CONV_3x3(NCHAN, l)                                        \
	sum0 = _mm_load_si128(buff0 + l);                         \
	sum1 = _mm_load_si128(buff1 + l);                         \
	PREP_3x3(NCHAN, l);                                       \
	res = _mm_mulhi_epi16(sum0, vker0);                       \
	res = _mm_add_epi16(res, _mm_mulhi_epi16(sum1, vker1));   \
	res = _mm_add_epi16(res, _mm_mulhi_epi16(sum2, vker2));   \
	rl = _mm_unpacklo_epi16(mzero, res);                      \
	rh = _mm_unpackhi_epi16(mzero, res);                      \
	rl = _mm_srai_epi32(rl, shift);                           \
	rh = _mm_srai_epi32(rh, shift);                           \
	res = _mm_packs_epi32(rl, rh)

/* *********************************************************** */

#define	LOADU_SRC(ds, tp)                                   \
	ds = _mm_load_si128(tp);                             \
	tp++;                                                \

/* *********************************************************** */

#define	LOAD_SRC_NF(l)                                        \
	_mm_storeu_si128(buffn + l,                               \
		_mm_loadu_si128((__m128i *)sp));                      \
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
#define	KSIZE	3

/* *********************************************************** */

#define	FUNC_SCONV3x3_16ext(NCHAN)                             \
	mlib_status mlib_s_sconv3x3_16ext_##NCHAN(             \
		mlib_image *dst, const mlib_image *src,            \
		mlib_s32 dx_l, mlib_s32 dx_r, mlib_s32 dy_t,       \
		mlib_s32 dy_b, const mlib_s32 *hkernel,            \
		const mlib_s32 *vkernel, mlib_s32 scale,           \
		mlib_s32 cmask)                                    \
	{                                                      \
	    __m128i *buff0, *buff1, *buffT, *pbuff, *sbuff;    \
		__m128i *tbuf, *buffs[2 * (KSIZE + 1)], *buffn;    \
		__m128i hker0, hker1, hker2, vker0, vker1, vker2;  \
	    __m128i s0, d0, d1, dt, sum0, sum1, sum2,          \
			aa, bb, res, rl, rh, mask;                     \
		__m128i mzero = _mm_setzero_si128();               \
	    mlib_s32 shift, dsize, ssize, esize, emask;        \
	    mlib_s32 row, i, j, buff_ind = 0, gap;             \
	                                                       \
	    GET_SRC_DST_PARAMETERS(mlib_s16);                  \
	    GET_KERN();                                        \
		                                                   \
	    width *= NCHAN;                                    \
		ssize = (width + NCHAN * (KSIZE - 1) + 7) >> 3;    \
	    dsize = (width + 7) >> 3;                          \
		esize = ssize + 4;                                 \
		                                                   \
		emask = (0xFF00 >> (width & 7)) & 0xFF;            \
		mask = LOAD_2x64(mask_pst_16,                      \
			(emask >> 4) & 0xf, emask & 0xf);              \
		                                                   \
		pbuff = __mlib_malloc(sizeof (__m128i) *           \
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
			if (dx_l) {                                    \
				EXT_LEFT_##NCHAN(                          \
					(mlib_s16 *)buffn, dx_l, 0);           \
			}                                              \
			if (dx_r) {                                    \
				EXT_RIGHT_##NCHAN(                         \
					(mlib_s16 *)buffn, (2 - dx_r), 0);     \
			}                                              \
			if ((i >= dy_t) && (i < height + 1 - dy_b))    \
				sl += sll;                                 \
		}                                                  \
	    for (i = 0; i < 2; i++) {                          \
			__m128i **buffc = buffs + buff_ind;            \
                                                           \
			sbuff = buffc[0];                              \
			buffn = buffc[KSIZE];                          \
			tbuf = buffn;                                  \
			buffn = (__m128i *)(((mlib_s16 *)buffn) + gap);\
			sp = sl;                                       \
			                                               \
			LOADU_SRC(d1, sbuff);                          \
			for (j = 0; j < dsize; j++) {                  \
				PREP_3x3(NCHAN, j);                        \
				LOAD_SRC_NF(j);                            \
			}                                              \
			                                               \
			for (; j < ssize; j++) {                       \
				LOAD_SRC_NF(j);                            \
			}                                              \
                                                           \
			buffT = buff1;                                 \
			buff1 = buff0;                                 \
			buff0 = buffT;                                 \
			                                               \
			buffn = tbuf;                                  \
			if (dx_l) {                                    \
				EXT_LEFT_##NCHAN(                          \
					(mlib_s16 *)buffn, dx_l, 0);           \
			}                                              \
			if (dx_r) {                                    \
				EXT_RIGHT_##NCHAN(                         \
				(mlib_s16 *)buffn, (2 - dx_r), 0);         \
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
			sp = sl;                                       \
			dp = dl;                                       \
			                                               \
			LOADU_SRC(d1, sbuff);                          \
			for (i = 0; i < (width >> 3); i++) {           \
				CONV_3x3(NCHAN, i);                        \
				LOAD_SRC_NF(i);                            \
				_mm_storeu_si128((void *)dp, res);         \
				dp += 8;                                   \
			}                                              \
			if (width & 7) {                               \
				__m128i dd;                                \
	                                                       \
				CONV_3x3(NCHAN, i);                        \
				LOAD_SRC_NF(i);                            \
				dd = _mm_loadu_si128((void *)dp);          \
				dd = _mm_or_si128(_mm_and_si128(mask, res),\
					_mm_andnot_si128(mask, dd));           \
				_mm_storeu_si128((void *)dp, dd);          \
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
					(mlib_s16 *)buffn, dx_l, 0);           \
			}                                              \
			if (dx_r) {                                    \
				EXT_RIGHT_##NCHAN(                         \
					(mlib_s16 *)buffn, (2 - dx_r), 0);     \
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

FUNC_SCONV3x3_16ext(1)
FUNC_SCONV3x3_16ext(2)
FUNC_SCONV3x3_16ext(3)
FUNC_SCONV3x3_16ext(4)

/* *********************************************************** */
