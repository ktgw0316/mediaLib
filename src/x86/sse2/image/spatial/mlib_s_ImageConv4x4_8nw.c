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

#pragma ident   "@(#)mlib_s_ImageConv4x4_8nw.c	9.3	07/11/05 SMI"

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

#define	ALGN(x, y, sh)                                         \
	_mm_or_si128(_mm_srli_si128(x, (sh)),                      \
		_mm_slli_si128(y, 16 - (sh)))

/* *********************************************************** */
#define	GET_SRC_DST_PARAMETERS(type)                           \
	mlib_s32 height = mlib_ImageGetHeight(src);                \
	mlib_s32 width = mlib_ImageGetWidth(src);                  \
	mlib_s32 sll = mlib_ImageGetStride(src) / sizeof (type);   \
	mlib_s32 dll = mlib_ImageGetStride(dst) / sizeof (type);   \
	type *sl = (type *) mlib_ImageGetData(src);                \
	type *dl = (type *) mlib_ImageGetData(dst);                \
	type *sp, *dp

/* *********************************************************** */
#define	GET_KERN()                                             \
	k1 = _mm_set1_epi16((kern[0] >> 16) & 0xFFFF);             \
    k2 = _mm_set1_epi16((kern[1] >> 16) & 0xFFFF);             \
    k3 = _mm_set1_epi16((kern[2] >> 16) & 0xFFFF);             \
    k4 = _mm_set1_epi16((kern[3] >> 16) & 0xFFFF);             \
    k5 = _mm_set1_epi16((kern[4] >> 16) & 0xFFFF);             \
    k6 = _mm_set1_epi16((kern[5] >> 16) & 0xFFFF);             \
    k7 = _mm_set1_epi16((kern[6] >> 16) & 0xFFFF);             \
    k8 = _mm_set1_epi16((kern[7] >> 16) & 0xFFFF);             \
    k9 = _mm_set1_epi16((kern[8] >> 16) & 0xFFFF);             \
	k10 = _mm_set1_epi16((kern[9] >> 16) & 0xFFFF);            \
    k11 = _mm_set1_epi16((kern[10] >> 16) & 0xFFFF);           \
    k12 = _mm_set1_epi16((kern[11] >> 16) & 0xFFFF);           \
    k13 = _mm_set1_epi16((kern[12] >> 16) & 0xFFFF);           \
    k14 = _mm_set1_epi16((kern[13] >> 16) & 0xFFFF);           \
    k15 = _mm_set1_epi16((kern[14] >> 16) & 0xFFFF);           \
    k16 = _mm_set1_epi16((kern[15] >> 16) & 0xFFFF);           \
    shift = scalef_expon - 25;                                 \

/* *********************************************************** */

#define	PREP_4x4(NCHAN)                                        \
	d0 = d1;                                                   \
	UNPACK_SRC(d1, sp);                                        \
	sum0 = _mm_mulhi_epi16(d0, k1);                            \
	sum1 = _mm_mulhi_epi16(d0, k5);                            \
	sum2 = _mm_mulhi_epi16(d0, k9);                            \
	aa = ALGN(d0, d1, 2 * NCHAN);                              \
	sum0 = _mm_add_epi16(sum0, _mm_mulhi_epi16(aa, k2));       \
	sum1 = _mm_add_epi16(sum1, _mm_mulhi_epi16(aa, k6));       \
	sum2 = _mm_add_epi16(sum2, _mm_mulhi_epi16(aa, k10));      \
	bb = ALGN(d0, d1, 4 * NCHAN);                              \
	sum0 = _mm_add_epi16(sum0, _mm_mulhi_epi16(bb, k3));       \
	sum1 = _mm_add_epi16(sum1, _mm_mulhi_epi16(bb, k7));       \
	sum2 = _mm_add_epi16(sum2, _mm_mulhi_epi16(bb, k11));      \
	cc = ALGN(d0, d1, 6 * NCHAN);                              \
	sum0 = _mm_add_epi16(sum0, _mm_mulhi_epi16(cc, k4));       \
	sum1 = _mm_add_epi16(sum1, _mm_mulhi_epi16(cc, k8));       \
	sum2 = _mm_add_epi16(sum2, _mm_mulhi_epi16(cc, k12));      \
	buff0[i] = sum0;                                           \
	buff1[i] = sum1;                                           \
	buff2[i] = sum2;                                           \

/* *********************************************************** */
#define	CONV_4x4(NCHAN, i)                                     \
	d0 = d1;                                                   \
	UNPACK_SRC(d1, sp);                                        \
	sum0 = _mm_mulhi_epi16(d0, k1);                            \
	sum1 = _mm_mulhi_epi16(d0, k5);                            \
	sum2 = _mm_mulhi_epi16(d0, k9);                            \
	sum3 = _mm_mulhi_epi16(d0, k13);                           \
	aa = ALGN(d0, d1, 2 * NCHAN);                              \
	sum0 = _mm_add_epi16(sum0, _mm_mulhi_epi16(aa, k2));       \
	sum1 = _mm_add_epi16(sum1, _mm_mulhi_epi16(aa, k6));       \
	sum2 = _mm_add_epi16(sum2, _mm_mulhi_epi16(aa, k10));      \
	sum3 = _mm_add_epi16(sum3, _mm_mulhi_epi16(aa, k14));      \
	bb = ALGN(d0, d1, 4 * NCHAN);                              \
	sum0 = _mm_add_epi16(sum0, _mm_mulhi_epi16(bb, k3));       \
	sum1 = _mm_add_epi16(sum1, _mm_mulhi_epi16(bb, k7));       \
	sum2 = _mm_add_epi16(sum2, _mm_mulhi_epi16(bb, k11));      \
	sum3 = _mm_add_epi16(sum3, _mm_mulhi_epi16(bb, k15));      \
	cc = ALGN(d0, d1, 6 * NCHAN);                              \
	sum0 = _mm_add_epi16(sum0, _mm_mulhi_epi16(cc, k4));       \
	sum1 = _mm_add_epi16(sum1, _mm_mulhi_epi16(cc, k8));       \
	sum2 = _mm_add_epi16(sum2, _mm_mulhi_epi16(cc, k12));      \
	sum3 = _mm_add_epi16(sum3, _mm_mulhi_epi16(cc, k16));      \
	prev0 = buff0[i];                                          \
	prev1 = buff1[i];                                          \
	prev2 = buff2[i];                                          \
	sum3 = _mm_add_epi16(sum3, prev0);                         \
	sum3 = _mm_add_epi16(sum3, prev1);                         \
	sum3 = _mm_add_epi16(sum3, prev2);                         \
	buff0[i] = sum0;                                           \
	buff1[i] = sum1;                                           \
	buff2[i] = sum2;                                           \
	res = _mm_srai_epi16(sum3, shift);                         \
	res = _mm_packus_epi16(res, res)

/* *********************************************************** */

#define	UNPACK_SRC(dd, sp)                                      \
	dd = _mm_loadl_epi64((void *)sp);                       \
	sp += 8;                                                \
	dd = _mm_unpacklo_epi8(dd, mzero);                      \
	dd = _mm_slli_epi16(dd, 7);                             \

/* *********************************************************** */
/*
 * buffers distribution
 *
 * 0
 * 1 2
 * 3 4 5
 * 6 7 8 9    <-- previous line
 *
 * 0 2 5 9 z  <-- current line
 *
 */

#define	FUNC_CONV4x4_8nw(NCHAN)                                \
	mlib_status                                                \
	mlib_s_conv4x4_8nw_##NCHAN(                                \
		mlib_image *dst, mlib_image *src,                      \
	    mlib_s32 *kern, mlib_s32 scalef_expon)                 \
	{                                                          \
	    __m128i *pbuff, *buff_arr[12], **pbuff_arr = buff_arr; \
	    __m128i *buff0, *buff1, *buff2;                        \
	    GET_SRC_DST_PARAMETERS(mlib_u8);                       \
	    __m128i k1, k2, k3, k4, k5, k6, k7, k8;                \
	    __m128i k9, k10, k11, k12, k13, k14, k15, k16;         \
	    __m128i s0, d0, d1, prev0, prev1, prev2;               \
	    __m128i sum0, sum1, sum2, sum3, aa, bb, cc, res;       \
	    __m128i mzero = _mm_setzero_si128();                   \
	    mlib_s32 shift, ind;                                   \
	    mlib_s32 row, wid8, i, j;                              \
	                                                           \
	    width -= 3;                                            \
	    height -= 3;                                           \
	    width *= NCHAN;                                        \
	    dl += dll + NCHAN;                                     \
	    wid8 = (width + 7) / 8;                                \
	    pbuff = __mlib_malloc(sizeof (__m128i) * 6 * wid8);    \
	    GET_KERN();                                            \
	    for (i = 0; i < 6; i++) {                              \
			buff_arr[i] = pbuff + i * wid8;                    \
	    }                                                      \
	    ind = 0;                                               \
	    for (j = 1; j <= 3; j++) {                             \
			buff0 = buff_arr[ind];                             \
			buff1 = buff_arr[ind + 1];                         \
			buff2 = buff_arr[ind + 2];                         \
			sp = sl;                                           \
			UNPACK_SRC(d1, sp);                                \
			for (i = 0; i < wid8; i++) {                       \
				PREP_4x4(NCHAN);                               \
			}                                                  \
			sl += sll;                                         \
			ind += j;                                          \
	    }                                                      \
	    for (row = 0; row < height; row++) {                   \
			buff0 = pbuff_arr[0];                              \
			buff1 = pbuff_arr[2];                              \
			buff2 = pbuff_arr[5];                              \
			sp = sl;                                           \
			dp = dl;                                           \
			UNPACK_SRC(d1, sp);                                \
			for (i = 0; i < width / 8; i++) {                  \
				CONV_4x4(NCHAN, i);                            \
				_mm_storel_epi64((void *)dp, res);             \
				dp += 8;                                       \
			}                                                  \
			if (width & 7) {                                   \
				__m128i mask, dd;                              \
	                                                           \
				CONV_4x4(NCHAN, i);                            \
				mask =                                         \
				_mm_loadl_epi64((void *)(mlib_mask64_arr +     \
				(width & 7)));                                 \
				dd = _mm_loadl_epi64((void *)dp);              \
				dd = _mm_or_si128(_mm_and_si128(mask, res),    \
				_mm_andnot_si128(mask, dd));                   \
				_mm_storel_epi64((void *)dp, dd);              \
			}                                                  \
			ind = (pbuff_arr == buff_arr) ? 6 : -6;            \
			pbuff_arr[ind + 0] = pbuff_arr[1];                 \
			pbuff_arr[ind + 1] = pbuff_arr[3];                 \
			pbuff_arr[ind + 2] = pbuff_arr[4];                 \
			pbuff_arr[ind + 3] = pbuff_arr[0];                 \
			pbuff_arr[ind + 4] = pbuff_arr[2];                 \
			pbuff_arr[ind + 5] = pbuff_arr[5];                 \
			pbuff_arr += ind;                                  \
			sl += sll;                                         \
			dl += dll;                                         \
	    }                                                      \
	    __mlib_free(pbuff);                                      \
	    return (MLIB_SUCCESS);                                 \
	}

/* *********************************************************** */

FUNC_CONV4x4_8nw(1)
    FUNC_CONV4x4_8nw(2)

/* *********************************************************** */
#undef PREP_4x4
#define	PREP_4x4(NCHAN)                                        \
	d0 = d1;                                                   \
	d1 = d2;                                                   \
	UNPACK_SRC(d2, sp);                                        \
	sum0 = _mm_mulhi_epi16(d0, k1);                            \
	sum1 = _mm_mulhi_epi16(d0, k5);                            \
	sum2 = _mm_mulhi_epi16(d0, k9);                            \
	aa = ALGN(d0, d1, 2 * NCHAN);                              \
	sum0 = _mm_add_epi16(sum0, _mm_mulhi_epi16(aa, k2));       \
	sum1 = _mm_add_epi16(sum1, _mm_mulhi_epi16(aa, k6));       \
	sum2 = _mm_add_epi16(sum2, _mm_mulhi_epi16(aa, k10));      \
	bb = ALGN(d0, d1, 4 * NCHAN);                              \
	sum0 = _mm_add_epi16(sum0, _mm_mulhi_epi16(bb, k3));       \
	sum1 = _mm_add_epi16(sum1, _mm_mulhi_epi16(bb, k7));       \
	sum2 = _mm_add_epi16(sum2, _mm_mulhi_epi16(bb, k11));      \
	cc = ALGN(d1, d2, 6 * NCHAN - 16);                         \
	sum0 = _mm_add_epi16(sum0, _mm_mulhi_epi16(cc, k4));       \
	sum1 = _mm_add_epi16(sum1, _mm_mulhi_epi16(cc, k8));       \
	sum2 = _mm_add_epi16(sum2, _mm_mulhi_epi16(cc, k12));      \
	buff0[i] = sum0;                                           \
	buff1[i] = sum1;                                           \
	buff2[i] = sum2;                                           \

/* *********************************************************** */
#undef CONV_4x4
#define	CONV_4x4(NCHAN, i)                                     \
	d0 = d1;                                                   \
	d1 = d2;                                                   \
	UNPACK_SRC(d2, sp);                                        \
	sum0 = _mm_mulhi_epi16(d0, k1);                            \
	sum1 = _mm_mulhi_epi16(d0, k5);                            \
	sum2 = _mm_mulhi_epi16(d0, k9);                            \
	sum3 = _mm_mulhi_epi16(d0, k13);                           \
	aa = ALGN(d0, d1, 2 * NCHAN);                              \
	sum0 = _mm_add_epi16(sum0, _mm_mulhi_epi16(aa, k2));       \
	sum1 = _mm_add_epi16(sum1, _mm_mulhi_epi16(aa, k6));       \
	sum2 = _mm_add_epi16(sum2, _mm_mulhi_epi16(aa, k10));      \
	sum3 = _mm_add_epi16(sum3, _mm_mulhi_epi16(aa, k14));      \
	bb = ALGN(d0, d1, 4 * NCHAN);                              \
	sum0 = _mm_add_epi16(sum0, _mm_mulhi_epi16(bb, k3));       \
	sum1 = _mm_add_epi16(sum1, _mm_mulhi_epi16(bb, k7));       \
	sum2 = _mm_add_epi16(sum2, _mm_mulhi_epi16(bb, k11));      \
	sum3 = _mm_add_epi16(sum3, _mm_mulhi_epi16(bb, k15));      \
	cc = ALGN(d1, d2, 6 * NCHAN - 16);                         \
	sum0 = _mm_add_epi16(sum0, _mm_mulhi_epi16(cc, k4));       \
	sum1 = _mm_add_epi16(sum1, _mm_mulhi_epi16(cc, k8));       \
	sum2 = _mm_add_epi16(sum2, _mm_mulhi_epi16(cc, k12));      \
	sum3 = _mm_add_epi16(sum3, _mm_mulhi_epi16(cc, k16));      \
	prev0 = buff0[i];                                          \
	prev1 = buff1[i];                                          \
	prev2 = buff2[i];                                          \
	sum3 = _mm_add_epi16(sum3, prev0);                         \
	sum3 = _mm_add_epi16(sum3, prev1);                         \
	sum3 = _mm_add_epi16(sum3, prev2);                         \
	buff0[i] = sum0;                                           \
	buff1[i] = sum1;                                           \
	buff2[i] = sum2;                                           \
	res = _mm_srai_epi16(sum3, shift);                         \
	res = _mm_packus_epi16(res, res);                          \

/* *********************************************************** */
/*
 * buffers distribution
 *
 * 0
 * 1 2
 * 3 4 5
 * 6 7 8 9    <-- previous line
 *
 * 0 2 5 9 z  <-- current line
 *
 */
#undef  FUNC_CONV4x4_8nw
#define	FUNC_CONV4x4_8nw(NCHAN)                               \
	mlib_status                                               \
	mlib_s_conv4x4_8nw_##NCHAN(                               \
		mlib_image *dst, mlib_image *src,                     \
	    mlib_s32 *kern, mlib_s32 scalef_expon)                \
	{                                                         \
	    __m128i *pbuff, *buff_arr[12], **pbuff_arr = buff_arr;\
	    __m128i *buff0, *buff1, *buff2;                       \
	    GET_SRC_DST_PARAMETERS(mlib_u8);                      \
	    __m128i k1, k2, k3, k4, k5, k6, k7, k8;               \
	    __m128i k9, k10, k11, k12, k13, k14, k15, k16;        \
	    __m128i s0, d0, d1, d2, prev0, prev1, prev2;          \
	    __m128i sum0, sum1, sum2, sum3, aa, bb, cc, res;      \
	    __m128i mzero = _mm_setzero_si128();                  \
	    mlib_s32 shift, ind;                                  \
	    mlib_s32 row, wid8, i, j;                             \
	                                                          \
	    width -= 3;                                           \
	    height -= 3;                                          \
	    width *= NCHAN;                                       \
	    dl += dll + NCHAN;                                    \
	    wid8 = (width + 7) / 8;                               \
	    pbuff = __mlib_malloc(sizeof (__m128i) * 6 * wid8);   \
	    GET_KERN();                                           \
	    for (i = 0; i < 6; i++) {                             \
			buff_arr[i] = pbuff + i * wid8;                   \
	    }                                                     \
	    ind = 0;                                              \
	    for (j = 1; j <= 3; j++) {                            \
			buff0 = buff_arr[ind];                            \
			buff1 = buff_arr[ind + 1];                        \
			buff2 = buff_arr[ind + 2];                        \
			sp = sl;                                          \
			UNPACK_SRC(d1, sp);                               \
			UNPACK_SRC(d2, sp);                               \
			for (i = 0; i < wid8; i++) {                      \
				PREP_4x4(NCHAN);                              \
			}                                                 \
			sl += sll;                                        \
			ind += j;                                         \
	    }                                                     \
	    for (row = 0; row < height; row++) {                  \
			buff0 = pbuff_arr[0];                             \
			buff1 = pbuff_arr[2];                             \
			buff2 = pbuff_arr[5];                             \
			sp = sl;                                          \
			dp = dl;                                          \
			UNPACK_SRC(d1, sp);                               \
			UNPACK_SRC(d2, sp);                               \
			for (i = 0; i < width / 8; i++) {                 \
				CONV_4x4(NCHAN, i);                           \
				_mm_storel_epi64((void *)dp, res);            \
				dp += 8;                                      \
			}                                                 \
			if (width & 7) {                                  \
				__m128i mask, dd;                             \
	                                                          \
				CONV_4x4(NCHAN, i);                           \
				mask =                                        \
				_mm_loadl_epi64((void *)(mlib_mask64_arr +    \
				(width & 7)));                                \
				dd = _mm_loadl_epi64((void *)dp);             \
				dd = _mm_or_si128(_mm_and_si128(mask, res),   \
				_mm_andnot_si128(mask, dd));                  \
				_mm_storel_epi64((void *)dp, dd);             \
			}                                                 \
			ind = (pbuff_arr == buff_arr) ? 6 : -6;           \
			pbuff_arr[ind + 0] = pbuff_arr[1];                \
			pbuff_arr[ind + 1] = pbuff_arr[3];                \
			pbuff_arr[ind + 2] = pbuff_arr[4];                \
			pbuff_arr[ind + 3] = pbuff_arr[0];                \
			pbuff_arr[ind + 4] = pbuff_arr[2];                \
			pbuff_arr[ind + 5] = pbuff_arr[5];                \
			pbuff_arr += ind;                                 \
			sl += sll;                                        \
			dl += dll;                                        \
	    }                                                     \
	    __mlib_free(pbuff);                                     \
	    return (MLIB_SUCCESS);                                \
	}

/* *********************************************************** */

FUNC_CONV4x4_8nw(3)
    FUNC_CONV4x4_8nw(4)

/* *********************************************************** */
