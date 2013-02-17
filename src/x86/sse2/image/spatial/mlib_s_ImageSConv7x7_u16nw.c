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

#pragma ident	"@(#)mlib_s_ImageSConv7x7_u16nw.c	9.5	07/11/05 SMI"

/*
 *      A 2-D convolution.
 */

#include <mlib_image.h>
#include <mlib_sse_utils.h>

/* *********************************************************** */

#define	KER	7

/* *********************************************************** */

#define	BUFF_LINE	256

/* *********************************************************** */
#define	ALGN(x, y, sh)                                                    \
	_mm_or_si128(_mm_srli_si128(x, sh), _mm_slli_si128(y, 16 - sh))

/* *********************************************************** */
#define	GET_SRC_DST_PARAMETERS(type)                               \
	mlib_s32 height = mlib_ImageGetHeight(src);                \
	mlib_s32 width = mlib_ImageGetWidth(src);                  \
	mlib_s32 sll = mlib_ImageGetStride(src) / sizeof (type);   \
	    mlib_s32                                               \
		dll = mlib_ImageGetStride(dst) / sizeof (type);    \
	    type *                                                 \
		sl = (type *) mlib_ImageGetData(src);              \
	    type *                                                 \
		dl = (type *) mlib_ImageGetData(dst);              \
	    type *                                                 \
	    sp, *                                                  \
		dp

/* *********************************************************** */
#define	GET_KERN()                                              \
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
	shift = 2 * scalef_expon - (32 + 32 - 16);              \
	for (i = 0; i < 7; i++) {                               \
	    kerh_sum += (hkernel[i] >> 16);                     \
	    kerv_sum += (vkernel[i] >> 16);                     \
	}                                                       \
	                                                        \
	ker_sum = (mlib_d64)                                    \
	    kerh_sum *                                          \
	    kerv_sum / (1 << (shift + 1));                      \
	                                                        \
	rnd = _mm_set1_epi16(KER/2);                            \
	ker_off = _mm_set1_epi32(ker_sum - 0x8000);             \
	mask8000 = _mm_set1_epi16(0x8000)

/* *********************************************************** */
#define	PREP_7x7(NCHAN)                                           \
	s0 = LOAD_SRC(sp);                                        \
	sum6 = _mm_add_epi16(rnd, _mm_mulhi_epi16(s0, hker0));    \
	s1 = LOAD_SRC(sp + NCHAN);                                \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s1, hker1));   \
	s2 = LOAD_SRC(sp + 2 * NCHAN);                            \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s2, hker2));   \
	s3 = LOAD_SRC(sp + 3 * NCHAN);                            \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s3, hker3));   \
	s4 = LOAD_SRC(sp + 4 * NCHAN);                            \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s4, hker4));   \
	s5 = LOAD_SRC(sp + 5 * NCHAN);                            \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s5, hker5));   \
	s6 = LOAD_SRC(sp + 6 * NCHAN);                            \
	sum6 = _mm_add_epi16(sum6, _mm_mulhi_epi16(s6, hker6));   \
	buff0[i] = sum6;                                          \
	sp += 8

/* *********************************************************** */
#define	CONV_7x7(NCHAN)                                           \
	sum0 = buff0[i];                                          \
	sum1 = buff1[i];                                          \
	sum2 = buff2[i];                                          \
	sum3 = buff3[i];                                          \
	sum4 = buff4[i];                                          \
	sum5 = buff5[i];                                          \
	PREP_7x7(NCHAN);                                          \
	res = _mm_add_epi16(rnd, _mm_mulhi_epi16(sum0, vker0));   \
	res = _mm_add_epi16(res, _mm_mulhi_epi16(sum1, vker1));   \
	res = _mm_add_epi16(res, _mm_mulhi_epi16(sum2, vker2));   \
	res = _mm_add_epi16(res, _mm_mulhi_epi16(sum3, vker3));   \
	res = _mm_add_epi16(res, _mm_mulhi_epi16(sum4, vker4));   \
	res = _mm_add_epi16(res, _mm_mulhi_epi16(sum5, vker5));   \
	res = _mm_add_epi16(res, _mm_mulhi_epi16(sum6, vker6));   \
	rl = _mm_unpacklo_epi16(mzero, res);                      \
	rh = _mm_unpackhi_epi16(mzero, res);                      \
	rl = _mm_srai_epi32(rl, shift);                           \
	rh = _mm_srai_epi32(rh, shift);                           \
	rl = _mm_add_epi32(rl, ker_off);                          \
	rh = _mm_add_epi32(rh, ker_off);                          \
	res = _mm_packs_epi32(rl, rh);                            \
	res = _mm_xor_si128(res, mask8000)

/* *********************************************************** */
#define	LOAD_SRC(sp)                                             \
	_mm_xor_si128(_mm_loadu_si128((void *)(sp)), mask8000)

/* *********************************************************** */

#define	KER  7

/* *********************************************************** */

#define	FUNC_CONV7x7_u16nw(NCHAN)                                           \
	mlib_status                                                         \
	mlib_s_sconv7x7_u16nw_##NCHAN(mlib_image *dst,                      \
	    mlib_image *src, mlib_s32 *hkernel, mlib_s32 *vkernel,          \
	    mlib_s32 scalef_expon)                                          \
	{                                                                   \
	    __m128i buff_loc[(KER - 1) * BUFF_LINE], *pbuff = buff_loc;     \
	    __m128i *buff0, *buff1, *buff2, *buff3, *buff4, *buff5;         \
	    __m128i *buffT;                                                 \
	    GET_SRC_DST_PARAMETERS(mlib_s16);                               \
	    __m128i hker0, hker1, hker2, hker3, hker4, hker5, hker6;        \
	    __m128i vker0, vker1, vker2, vker3, vker4, vker5, vker6;        \
	    __m128i s0, s1, s2, s3, s4, s5, s6, d0, d1, prev0, prev1;       \
	    __m128i sum0, sum1, sum2, sum3, sum4, sum5, sum6;               \
	    __m128i dd, res, rl, rh, mask, rnd;                             \
	    __m128i mzero = _mm_setzero_si128();                            \
	    __m128i ker_off, mask8000;                                      \
	    mlib_s32 shift, emask, ker_sum, kerh_sum = 0, kerv_sum = 0;     \
	    mlib_s32 row, wid8, i, j;                                       \
                                                                            \
	    width -= (KER - 1);                                             \
	    height -= (KER - 1);                                            \
	    width *= NCHAN;                                                 \
	    dl += (KER/2) * (dll + NCHAN);                                  \
	    wid8 = (width + 7) / 8;                                         \
	    if (wid8 > BUFF_LINE) {                                         \
		pbuff = __mlib_malloc(sizeof (__m128i)*((KER - 1)*wid8));   \
	    }                                                               \
	    emask = (0xFF00 >> (width & 7)) & 0xFF;                         \
	    mask =                                                          \
		LOAD_2x64(mask_pst_16, (emask >> 4) & 0xf, emask & 0xf);    \
	    GET_KERN();                                                     \
	    buff0 = pbuff;                                                  \
	    buff1 = buff0 + wid8;                                           \
	    buff2 = buff1 + wid8;                                           \
	    buff3 = buff2 + wid8;                                           \
	    buff4 = buff3 + wid8;                                           \
	    buff5 = buff4 + wid8;                                           \
	    for (j = 0; j < (KER - 1); j++) {                               \
		sp = sl;                                                    \
		for (i = 0; i < wid8; i++) {                                \
		    PREP_7x7(NCHAN);                                        \
		}                                                           \
		sl += sll;                                                  \
		buffT = buff0;                                              \
		buff0 = buff1;                                              \
		buff1 = buff2;                                              \
		buff2 = buff3;                                              \
		buff3 = buff4;                                              \
		buff4 = buff5;                                              \
		buff5 = buffT;                                              \
	    }                                                               \
	    for (row = 0; row < height; row++) {                            \
		sp = sl;                                                    \
		dp = dl;                                                    \
		for (i = 0; i < width / 8; i++) {                           \
		    CONV_7x7(NCHAN);                                        \
		    _mm_storeu_si128((void *)dp, res);                      \
		    dp += 8;                                                \
		}                                                           \
		if (width & 7) {                                            \
		    CONV_7x7(NCHAN);                                        \
		    dd = _mm_loadu_si128((void *)dp);                       \
		    dd = _mm_or_si128(_mm_and_si128(mask, res),             \
			_mm_andnot_si128(mask, dd));                        \
		    _mm_storeu_si128((void *)dp, dd);                       \
		}                                                           \
		buffT = buff0;                                              \
		buff0 = buff1;                                              \
		buff1 = buff2;                                              \
		buff2 = buff3;                                              \
		buff3 = buff4;                                              \
		buff4 = buff5;                                              \
		buff5 = buffT;                                              \
		sl += sll;                                                  \
		dl += dll;                                                  \
	    }                                                               \
	    if (pbuff != buff_loc)                                          \
		__mlib_free(pbuff);                                         \
	    return (MLIB_SUCCESS);                                          \
	}

/* *********************************************************** */

FUNC_CONV7x7_u16nw(1)
FUNC_CONV7x7_u16nw(2)
FUNC_CONV7x7_u16nw(3)
FUNC_CONV7x7_u16nw(4)

/* *********************************************************** */
