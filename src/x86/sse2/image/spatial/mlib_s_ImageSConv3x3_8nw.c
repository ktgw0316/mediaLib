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

#pragma ident	"@(#)mlib_s_ImageSConv3x3_8nw.c	9.4	07/11/05 SMI"

/*
 *      2-D convolution.
 */

#include <mlib_image.h>
#include <mlib_sse_utils.h>

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
#define	GET_KERN()                                                    \
	hker0 = _mm_set1_epi16((hkernel[0] >> 16) & 0xFFFF);          \
	hker1 = _mm_set1_epi16((hkernel[1] >> 16) & 0xFFFF);          \
	hker2 = _mm_set1_epi16((hkernel[2] >> 16) & 0xFFFF);          \
	vker0 = _mm_set1_epi16((vkernel[0] >> 16) & 0xFFFF);          \
	vker1 = _mm_set1_epi16((vkernel[1] >> 16) & 0xFFFF);          \
	vker2 = _mm_set1_epi16((vkernel[2] >> 16) & 0xFFFF);          \
	shift = 2 * scalef_expon - (32 + 32 - 7);                     \
	_rnd = _mm_set1_epi16((shift > 0) ? (1 << (shift - 1)) : 0)

/* *********************************************************** */
#define	PREP_3x3(NCHAN)                                           \
	d0 = d1;                                                  \
	UNPACK_SRC(d1, sp);                                       \
	sum2 = _mm_mulhi_epi16(d0, hker0);                        \
	aa = ALGN(d0, d1, 2 * NCHAN);                             \
	sum2 = _mm_add_epi16(sum2, _mm_mulhi_epi16(aa, hker1));   \
	bb = ALGN(d0, d1, 4 * NCHAN);                             \
	sum2 = _mm_add_epi16(sum2, _mm_mulhi_epi16(bb, hker2));   \
	buff0[i] = sum2

/* *********************************************************** */
#define	CONV_3x3(NCHAN)                                           \
	sum0 = buff0[i];                                          \
	sum1 = buff1[i];                                          \
	PREP_3x3(NCHAN);                                          \
	res = _mm_mulhi_epi16(sum0, vker0);                       \
	res = _mm_add_epi16(res, _mm_mulhi_epi16(sum1, vker1));   \
	res = _mm_add_epi16(res, _mm_mulhi_epi16(sum2, vker2));   \
	res = _mm_srai_epi16(res, shift);                         \
	res = _mm_packus_epi16(res, res)

/* *********************************************************** */
#define	UNPACK_SRC(dd, sp)                                      \
	dd = _mm_loadl_epi64((void *)sp);                       \
	sp += 8;                                                \
	dd = _mm_unpacklo_epi8(dd, mzero);                      \
	dd = _mm_slli_epi16(dd, 7)

/* *********************************************************** */

#define	FUNC_CONV3x3_8nw(NCHAN)                                           \
	mlib_status                                                       \
	mlib_s_sconv3x3_8nw_##NCHAN(mlib_image *dst,                      \
	    mlib_image *src, mlib_s32 *hkernel, mlib_s32 *vkernel,        \
	    mlib_s32 scalef_expon)                                        \
	{                                                                 \
	    __m128i buff_loc[3 * BUFF_LINE], *pbuff = buff_loc;           \
	    __m128i *buff0, *buff1, *buffT;                               \
	    GET_SRC_DST_PARAMETERS(mlib_u8);                              \
	    __m128i hker0, hker1, hker2, vker0, vker1, vker2, _rnd;       \
	    __m128i s0, d0, d1, prev0, prev1, sum0, sum1, sum2, aa, bb,   \
		res;                                                      \
	    __m128i mzero = _mm_setzero_si128();                          \
	    mlib_s32 shift;                                               \
	    mlib_s32 row, wid8, i, j;                                     \
	                                                                  \
	    width -= 2;                                                   \
	    height -= 2;                                                  \
	    width *= NCHAN;                                               \
	    dl += dll + NCHAN;                                            \
	    wid8 = (width + 7) / 8;                                       \
	    if (wid8 > BUFF_LINE) {                                       \
		pbuff = __mlib_malloc(sizeof (__m128i) * 2 * wid8);        \
	    }                                                             \
	    GET_KERN();                                                   \
	    buff0 = pbuff;                                                \
	    buff1 = buff0 + wid8;                                         \
	    for (j = 0; j < 2; j++) {                                     \
		sp = sl;                                                  \
		UNPACK_SRC(d1, sp);                                       \
		for (i = 0; i < wid8; i++) {                              \
		    PREP_3x3(NCHAN);                                      \
		}                                                         \
		sl += sll;                                                \
		buffT = buff1;                                            \
		buff1 = buff0;                                            \
		buff0 = buffT;                                            \
	    }                                                             \
	    for (row = 0; row < height; row++) {                          \
		sp = sl;                                                  \
		dp = dl;                                                  \
		UNPACK_SRC(d1, sp);                                       \
		for (i = 0; i < width / 8; i++) {                         \
		    CONV_3x3(NCHAN);                                      \
		    _mm_storel_epi64((void *)dp, res);                    \
		    dp += 8;                                              \
		}                                                         \
		if (width & 7) {                                          \
		    __m128i mask, dd;                                     \
	                                                                  \
		    CONV_3x3(NCHAN);                                      \
		    mask =                                                \
			_mm_loadl_epi64((void *)(mlib_mask64_arr +        \
			(width & 7)));                                    \
		    dd = _mm_loadl_epi64((void *)dp);                     \
		    dd = _mm_or_si128(_mm_and_si128(mask, res),           \
			_mm_andnot_si128(mask, dd));                      \
		    _mm_storel_epi64((void *)dp, dd);                     \
		}                                                         \
		buffT = buff1;                                            \
		buff1 = buff0;                                            \
		buff0 = buffT;                                            \
		sl += sll;                                                \
		dl += dll;                                                \
	    }                                                             \
	    if (pbuff != buff_loc)                                        \
		__mlib_free(pbuff);                                         \
	    return (MLIB_SUCCESS);                                        \
	}

/* *********************************************************** */

FUNC_CONV3x3_8nw(1)
    FUNC_CONV3x3_8nw(2)
    FUNC_CONV3x3_8nw(3)
    FUNC_CONV3x3_8nw(4)

/* *********************************************************** */
