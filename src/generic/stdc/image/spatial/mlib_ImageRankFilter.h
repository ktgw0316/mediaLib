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
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#ifndef _MLIB_IMAGERANKFILTER_H
#define	_MLIB_IMAGERANKFILTER_H

#pragma ident	"@(#)mlib_ImageRankFilter.h	9.2	07/10/09 SMI"

/*
 * DESCRIPTION
 *    Macros for descriptions and definitions of functions
 *    for Rank filtering.
 *    Macros are used for 3x3 rank filtering
 *    MLIB_BYTE, MLIB_SHORT, MLIB_INT, MLIB_FLOAT, MLIB_DOUBLE.
 *    Other are used for MxN rank filtering
 *    MLIB_INT, MLIB_FLOAT, MLIB_DOUBLE.
 *
 *    Source files that use the functions :
 *      mlib_ImageRankFilter3x3.c,
 *      mlib_ImageRankFilter3x3_Fp.c,
 *      mlib_ImageRankFilter7x7.c,
 *      mlib_ImageRankFilter7x7_Fp.c
 */

#ifdef __cplusplus
extern "C" {
#endif

/* *********************************************************** */

#define	RANK_1_FUNC(img_type, sortF)                            \
	sortF(d1, d4);                                          \
	sortF(d1, d7);                                          \
	sortF(d4, d7);                                          \
	sortF(d2, d5);                                          \
	sortF(d2, d8);                                          \
	sortF(d5, d8);                                          \
	sortF(d3, d6);                                          \
	sortF(d3, d9);                                          \
	sortF(d6, d9);                                          \
	sortF(d1, d2);                                          \
	sortF(d1, d3);                                          \
	sortF(d2, d3);                                          \
	sortF(d4, d5);                                          \
	sortF(d4, d6);                                          \
	sortF(d2, d4);                                          \
	*(img_type *) pd = (img_type) d2;                       \
	pd += sizeof (img_type)

/* *********************************************************** */

#define	RANK_2_FUNC(img_type, sortF)                            \
	sortF(d1, d2);                                          \
	sortF(d1, d3);                                          \
	sortF(d2, d3);                                          \
	sortF(d4, d5);                                          \
	sortF(d4, d6);                                          \
	sortF(d5, d6);                                          \
	sortF(d7, d8);                                          \
	sortF(d7, d9);                                          \
	sortF(d8, d9);                                          \
	sortF(d1, d6);                                          \
	sortF(d2, d5);                                          \
	sortF(d3, d4);                                          \
	sortF(d1, d2);                                          \
	sortF(d1, d3);                                          \
	sortF(d2, d3);                                          \
	sortF(d1, d9);                                          \
	sortF(d2, d8);                                          \
	sortF(d3, d7);                                          \
	sortF(d1, d3);                                          \
	sortF(d2, d3);                                          \
	*(img_type *) pd = (img_type) d3;                       \
	pd += sizeof (img_type)

/* *********************************************************** */

#define	RANK_3_FUNC(img_type, sortF)                            \
	sortF(d1, d2);                                          \
	sortF(d1, d3);                                          \
	sortF(d2, d3);                                          \
	sortF(d4, d5);                                          \
	sortF(d4, d6);                                          \
	sortF(d5, d6);                                          \
	sortF(d7, d8);                                          \
	sortF(d7, d9);                                          \
	sortF(d8, d9);                                          \
	sortF(d1, d6);                                          \
	sortF(d2, d5);                                          \
	sortF(d3, d4);                                          \
	sortF(d1, d2);                                          \
	sortF(d1, d3);                                          \
	sortF(d2, d3);                                          \
	sortF(d1, d9);                                          \
	sortF(d2, d8);                                          \
	sortF(d3, d7);                                          \
	sortF(d4, d5);                                          \
	sortF(d4, d6);                                          \
	sortF(d7, d8);                                          \
	sortF(d7, d9);                                          \
	sortF(d4, d7);                                          \
	*(img_type *) pd = (img_type) d4;                       \
	pd += sizeof (img_type)

/* *********************************************************** */

#define	RANK_4_FUNC(img_type, sortF)                            \
	sortF(d1, d4);                                          \
	sortF(d2, d5);                                          \
	sortF(d3, d6);                                          \
	sortF(d1, d2);                                          \
	sortF(d1, d3);                                          \
	sortF(d4, d6);                                          \
	sortF(d5, d6);                                          \
	sortF(d2, d3);                                          \
	sortF(d4, d5);                                          \
	sortF(d2, d7);                                          \
	sortF(d2, d4);                                          \
	sortF(d3, d7);                                          \
	sortF(d5, d7);                                          \
	sortF(d3, d4);                                          \
	sortF(d5, d8);                                          \
	sortF(d3, d5);                                          \
	sortF(d4, d8);                                          \
	sortF(d4, d5);                                          \
	sortF(d4, d9);                                          \
	sortF(d5, d9);                                          \
	*(img_type *) pd = (img_type) d5;                       \
	pd += sizeof (img_type)

/* *********************************************************** */

#define	RANK_5_FUNC(img_type, sortF)                            \
	sortF(d1, d2);                                          \
	sortF(d1, d3);                                          \
	sortF(d2, d3);                                          \
	sortF(d4, d5);                                          \
	sortF(d4, d6);                                          \
	sortF(d5, d6);                                          \
	sortF(d7, d8);                                          \
	sortF(d7, d9);                                          \
	sortF(d8, d9);                                          \
	sortF(d1, d9);                                          \
	sortF(d2, d8);                                          \
	sortF(d3, d7);                                          \
	sortF(d7, d8);                                          \
	sortF(d7, d9);                                          \
	sortF(d8, d9);                                          \
	sortF(d4, d9);                                          \
	sortF(d5, d8);                                          \
	sortF(d6, d7);                                          \
	sortF(d1, d3);                                          \
	sortF(d2, d3);                                          \
	sortF(d4, d6);                                          \
	sortF(d5, d6);                                          \
	sortF(d3, d6);                                          \
	*(img_type *) pd = (img_type) d6;                       \
	pd += sizeof (img_type)

/* *********************************************************** */

#define	RANK_6_FUNC(img_type, sortF)                            \
	sortF(d1, d2);                                          \
	sortF(d1, d3);                                          \
	sortF(d2, d3);                                          \
	sortF(d4, d5);                                          \
	sortF(d4, d6);                                          \
	sortF(d5, d6);                                          \
	sortF(d7, d8);                                          \
	sortF(d7, d9);                                          \
	sortF(d8, d9);                                          \
	sortF(d1, d9);                                          \
	sortF(d2, d8);                                          \
	sortF(d3, d7);                                          \
	sortF(d7, d8);                                          \
	sortF(d7, d9);                                          \
	sortF(d8, d9);                                          \
	sortF(d4, d9);                                          \
	sortF(d5, d8);                                          \
	sortF(d6, d7);                                          \
	sortF(d7, d8);                                          \
	sortF(d7, d9);                                          \
	*(img_type *) pd = (img_type) d7;                       \
	pd += sizeof (img_type)

/* *********************************************************** */

#define	RANK_7_FUNC(img_type, sortF)                            \
	sortF(d1, d4);                                          \
	sortF(d1, d7);                                          \
	sortF(d4, d7);                                          \
	sortF(d2, d5);                                          \
	sortF(d2, d8);                                          \
	sortF(d5, d8);                                          \
	sortF(d3, d6);                                          \
	sortF(d3, d9);                                          \
	sortF(d6, d9);                                          \
	sortF(d7, d8);                                          \
	sortF(d7, d9);                                          \
	sortF(d8, d9);                                          \
	sortF(d4, d6);                                          \
	sortF(d5, d6);                                          \
	sortF(d6, d8);                                          \
	*(img_type *) pd = (img_type) d8;                       \
	pd += sizeof (img_type)

/* *********************************************************** */

#define	RANK_ROW(img_type, sortType, sortF, rankF, suffix)           \
	static void                                                  \
	mlib_rank_row_3x3_##suffix(mlib_u8 *pd, const mlib_u8 *ps,   \
	    mlib_s32 width, mlib_s32 stride_s)                       \
	{                                                            \
	    const mlib_u8 *ps2 = ps + stride_s;                      \
	    const mlib_u8 *ps3 = ps + stride_s * 2;                  \
	    mlib_s32 i;                                              \
	    sortType mask, d1, d2, d3, d4, d5, d6, d7, d8, d9;       \
	                                                             \
	    for (i = 0; i < width - 3 + 1; i++) {                    \
		d1 = ((img_type *) ps)[0];                           \
		d2 = ((img_type *) ps)[1];                           \
		d3 = ((img_type *) ps)[2];                           \
		ps += sizeof (img_type);                             \
		d4 = ((img_type *) ps2)[0];                          \
		d5 = ((img_type *) ps2)[1];                          \
		d6 = ((img_type *) ps2)[2];                          \
		ps2 += sizeof (img_type);                            \
		d7 = ((img_type *) ps3)[0];                          \
		d8 = ((img_type *) ps3)[1];                          \
		d9 = ((img_type *) ps3)[2];                          \
		ps3 += sizeof (img_type);                            \
		rankF(img_type, sortF);                              \
	    }                                                        \
	}
#define	SORT2(a, b)	mask = ((b - a) & ((b - a) >> 31)), a += mask, b -= mask
#define	SORT2L(a, b)	mask = ((b - a) & ((b - a) >> 63)), a += mask, b -= mask
#define	SORT2C(a, b)	{ if (a > b) { mask = a; a = b; b = mask; } }

/* *********************************************************** */

#define	DEFINE_QSORT(img_type, suffix)                              \
	static void                                                 \
	mlib_qsort_##suffix(img_type * data, mlib_s32 n)            \
	{                                                           \
	    img_type mask, val, a1, a2, a3, a4, a5, a6, a7;         \
	    mlib_s32 i, j, m;                                       \
	                                                            \
	    if (n <= 1)                                             \
		return;                                             \
	    switch (n) {                                            \
	    case 2:                                                 \
		a1 = data[0];                                       \
		a2 = data[1];                                       \
		SORT2C(a1, a2);                                     \
		data[0] = a1;                                       \
		data[1] = a2;                                       \
		break;                                              \
	    case 3:                                                 \
		a1 = data[0];                                       \
		a2 = data[1];                                       \
		a3 = data[2];                                       \
		SORT2C(a1, a2);                                     \
		SORT2C(a1, a3);                                     \
		SORT2C(a2, a3);                                     \
		data[0] = a1;                                       \
		data[1] = a2;                                       \
		data[2] = a3;                                       \
		break;                                              \
	    case 4:                                                 \
		a1 = data[0];                                       \
		a2 = data[1];                                       \
		a3 = data[2];                                       \
		a4 = data[3];                                       \
		SORT2C(a1, a2);                                     \
		SORT2C(a3, a4);                                     \
		SORT2C(a2, a4);                                     \
		SORT2C(a1, a3);                                     \
		SORT2C(a2, a3);                                     \
		data[0] = a1;                                       \
		data[1] = a2;                                       \
		data[2] = a3;                                       \
		data[3] = a4;                                       \
		break;                                              \
	    case 5:                                                 \
		a1 = data[0];                                       \
		a2 = data[1];                                       \
		a3 = data[2];                                       \
		a4 = data[3];                                       \
		a5 = data[4];                                       \
		SORT2C(a1, a2);                                     \
		SORT2C(a3, a4);                                     \
		SORT2C(a3, a5);                                     \
		SORT2C(a4, a5);                                     \
		SORT2C(a1, a3);                                     \
		SORT2C(a2, a5);                                     \
		SORT2C(a2, a3);                                     \
		SORT2C(a2, a4);                                     \
		SORT2C(a3, a4);                                     \
		data[0] = a1;                                       \
		data[1] = a2;                                       \
		data[2] = a3;                                       \
		data[3] = a4;                                       \
		data[4] = a5;                                       \
		break;                                              \
	    case 6:                                                 \
		a1 = data[0];                                       \
		a2 = data[1];                                       \
		a3 = data[2];                                       \
		a4 = data[3];                                       \
		a5 = data[4];                                       \
		a6 = data[5];                                       \
		SORT2C(a1, a2);                                     \
		SORT2C(a3, a4);                                     \
		SORT2C(a5, a6);                                     \
		SORT2C(a1, a3);                                     \
		SORT2C(a1, a5);                                     \
		SORT2C(a2, a6);                                     \
		SORT2C(a4, a6);                                     \
		SORT2C(a2, a3);                                     \
		SORT2C(a4, a5);                                     \
		SORT2C(a2, a4);                                     \
		SORT2C(a3, a5);                                     \
		SORT2C(a3, a4);                                     \
		data[0] = a1;                                       \
		data[1] = a2;                                       \
		data[2] = a3;                                       \
		data[3] = a4;                                       \
		data[4] = a5;                                       \
		data[5] = a6;                                       \
		break;                                              \
	    default:                                                \
		i = 0;                                              \
		j = n - 1;                                          \
		m = (i + j) / 2;                                    \
		val = data[m];                                      \
		data[m] = data[0];                                  \
		data[0] = val;                                      \
		for (; ; ) {                                        \
		    while (val < data[j])                           \
			j--;                                        \
		    if (j <= i)                                     \
			break;                                      \
		    data[i++] = data[j];                            \
		    data[j] = val;                                  \
		    while (val > data[i])                           \
			i++;                                        \
		    if (i >= j)                                     \
			break;                                      \
		    data[j--] = data[i];                            \
		    data[i] = val;                                  \
		}                                                   \
		if (i < m) {                                        \
		    if (i > 1)                                      \
			mlib_qsort_##suffix(data, i);               \
		    mlib_qsort_##suffix(data + i + 1, n - i - 1);   \
		} else {                                            \
		    if (i < n - 2)                                  \
			mlib_qsort_##suffix(data + i + 1,           \
			    n - i - 1);                             \
		    mlib_qsort_##suffix(data, i);                   \
		}                                                   \
	    }                                                       \
	}

/* *********************************************************** */

#define	DEFINE_RM_LINE(img_type, suffix)                               \
	static void                                                    \
	mlib_rm_line_##suffix(img_type *dst, const img_type *src1,     \
	    mlib_s32 size1, img_type *src2, mlib_s32 size2)            \
	{                                                              \
	    for (; ; ) {                                               \
		if (*src1 == *src2) {                                  \
		    src1++;                                            \
		    src2++;                                            \
		    size1--;                                           \
		    if (--size2 == 0)                                  \
			break;                                         \
		} else {                                               \
		    (*dst++) = (*src1++);                              \
		    size1--;                                           \
		}                                                      \
	    }                                                          \
	    while (size1--)                                            \
		(*dst++) = (*src1++);                                  \
	}

/* *********************************************************** */

#define	DEFINE_ADD_LINE(img_type, suffix)                             \
	static void                                                   \
	mlib_add_line_##suffix(img_type *dst,                         \
	    const img_type *src1, mlib_s32 size1, img_type *src2,     \
	    mlib_s32 size2)                                           \
	{                                                             \
	    for (; ; ) {                                              \
		if (*src1 <= *src2) {                                 \
		    (*dst++) = (*src1++);                             \
		    if (--size1 == 0)                                 \
			goto src1_over;                               \
		} else {                                              \
		    (*dst++) = (*src2++);                             \
		    if (--size2 == 0)                                 \
			goto src2_over;                               \
		}                                                     \
	    }                                                         \
	    src1_over:while (size2--)                                 \
		(*dst++) = (*src2++);                                 \
	    return;                                                   \
	    src2_over:while (size1--)                                 \
		(*dst++) = (*src1++);                                 \
	    return;                                                   \
	}

/* *********************************************************** */

#define	DEFINE_RANK_COL_MxN(img_type, suffix)                          \
	static void                                                    \
	mlib_rank_col_MxN_##suffix(img_type *pd,                       \
	    const img_type *ps, mlib_s32 height, mlib_s32 rank,        \
	    mlib_s32 m, mlib_s32 n, mlib_s32 stride_d,                 \
	    mlib_s32 stride_s, img_type *wind)                         \
	{                                                              \
	    img_type *pwnd1;                                           \
	    img_type *pwnd2;                                           \
	    img_type *pline1;                                          \
	    mlib_s32 mn = m * n;                                       \
	    img_type *psx;                                             \
	    mlib_s32 i, j, k;                                          \
	    mlib_s32 wdiff = n * stride_s;                             \
	                                                               \
	    pwnd1 = wind;                                              \
	    pwnd2 = pwnd1 + mn;                                        \
	    pline1 = pwnd2 + mn;                                       \
	    k = 0;                                                     \
	    for (j = 0; j < n - 1; j++) {                              \
		psx = ps;                                              \
		ps += stride_s;                                        \
		for (i = 0; i < m; i++)                                \
		    pwnd1[k++] = (*psx++);                             \
	    }                                                          \
	    mlib_qsort_##suffix(pwnd1, mn - m);                        \
	    for (; j < height; j++) {                                  \
		psx = ps;                                              \
		ps += stride_s;                                        \
		for (i = 0; i < m; i++)                                \
		    pline1[i] = (*psx++);                              \
		mlib_qsort_##suffix(pline1, m);                        \
		mlib_add_line_##suffix(pwnd2, pwnd1, mn - m, pline1,   \
		    m);                                                \
		*pd = pwnd2[rank - 1];                                 \
		pd += stride_d;                                        \
		psx = ps - wdiff;                                      \
		for (i = 0; i < m; i++)                                \
		    pline1[i] = (*psx++);                              \
		mlib_qsort_##suffix(pline1, m);                        \
		mlib_rm_line_##suffix(pwnd1, pwnd2, m * n, pline1,     \
		    m);                                                \
	    }                                                          \
	}

/* *********************************************************** */

#define	DEFINE_RANK_ROW_MxN(img_type, suffix)                             \
	void                                                              \
	mlib_rank_row_MxN_##suffix(img_type *pd,                          \
	    const img_type *ps, mlib_s32 width, mlib_s32 rank,            \
	    mlib_s32 m, mlib_s32 n, mlib_s32 stride_s, img_type *wind)    \
	{                                                                 \
	    img_type *pwnd1;                                              \
	    img_type *pwnd2;                                              \
	    img_type *pline1;                                             \
	    mlib_s32 mn = m * n;                                          \
	    const img_type *psx;                                          \
	    mlib_s32 i, j, k;                                             \
	    mlib_s32 wdiff = n * stride_s;                                \
	                                                                  \
	    pwnd1 = wind;                                                 \
	    pwnd2 = pwnd1 + mn;                                           \
	    pline1 = pwnd2 + mn;                                          \
	    k = 0;                                                        \
	    for (j = 0; j < m - 1; j++) {                                 \
		psx = ps;                                                 \
		ps++;                                                     \
		for (i = 0; i < n; i++) {                                 \
		    pwnd1[k++] = *psx;                                    \
		    psx += stride_s;                                      \
		}                                                         \
	    }                                                             \
	    mlib_qsort_##suffix(pwnd1, mn - n);                           \
	    for (; j < width; j++) {                                      \
		psx = ps;                                                 \
		ps++;                                                     \
		for (i = 0; i < n; i++) {                                 \
		    pline1[i] = *psx;                                     \
		    psx += stride_s;                                      \
		}                                                         \
		mlib_qsort_##suffix(pline1, n);                           \
		mlib_add_line_##suffix(pwnd2, pwnd1, mn - n, pline1,      \
		    n);                                                   \
		*pd = pwnd2[rank - 1];                                    \
		pd++;                                                     \
		psx = ps - m;                                             \
		for (i = 0; i < n; i++) {                                 \
		    pline1[i] = *psx;                                     \
		    psx += stride_s;                                      \
		}                                                         \
		mlib_qsort_##suffix(pline1, n);                           \
		mlib_rm_line_##suffix(pwnd1, pwnd2, m * n, pline1,        \
		    n);                                                   \
	    }                                                             \
	}
typedef void (*mlib_RankFilterRow) (
    mlib_u8 *pd,
    const mlib_u8 *ps,
    mlib_s32 width,
    mlib_s32 stride_s);

#ifdef _NO_LONGLONG

RANK_ROW(mlib_s32,
    mlib_s32,
    SORT2C,
    RANK_1_FUNC,
    r1_s32)
RANK_ROW(
    mlib_s32,
    mlib_s32,
    SORT2C,
    RANK_2_FUNC,
    r2_s32)
RANK_ROW(
    mlib_s32,
    mlib_s32,
    SORT2C,
    RANK_3_FUNC,
    r3_s32)
RANK_ROW(
    mlib_s32,
    mlib_s32,
    SORT2C,
    RANK_4_FUNC,
    r4_s32)
RANK_ROW(
    mlib_s32,
    mlib_s32,
    SORT2C,
    RANK_5_FUNC,
    r5_s32)
RANK_ROW(
    mlib_s32,
    mlib_s32,
    SORT2C,
    RANK_6_FUNC,
    r6_s32)
RANK_ROW(
    mlib_s32,
    mlib_s32,
    SORT2C,
    RANK_7_FUNC,
    r7_s32)
#else /* _NO_LONGLONG */

RANK_ROW(mlib_s32,
    mlib_s64,
    SORT2L,
    RANK_1_FUNC,
    r1_s32)
RANK_ROW(
    mlib_s32,
    mlib_s64,
    SORT2L,
    RANK_2_FUNC,
    r2_s32)
RANK_ROW(
    mlib_s32,
    mlib_s64,
    SORT2L,
    RANK_3_FUNC,
    r3_s32)
RANK_ROW(
    mlib_s32,
    mlib_s64,
    SORT2L,
    RANK_4_FUNC,
    r4_s32)
RANK_ROW(
    mlib_s32,
    mlib_s64,
    SORT2L,
    RANK_5_FUNC,
    r5_s32)
RANK_ROW(
    mlib_s32,
    mlib_s64,
    SORT2L,
    RANK_6_FUNC,
    r6_s32)
RANK_ROW(
    mlib_s32,
    mlib_s64,
    SORT2L,
    RANK_7_FUNC,
    r7_s32)
#endif		   /* _NO_LONGLONG */
RANK_ROW(
    mlib_f32,
    mlib_f32,
    SORT2C,
    RANK_1_FUNC,
    r1_f32)
RANK_ROW(
    mlib_f32,
    mlib_f32,
    SORT2C,
    RANK_2_FUNC,
    r2_f32)
RANK_ROW(
    mlib_f32,
    mlib_f32,
    SORT2C,
    RANK_3_FUNC,
    r3_f32)
RANK_ROW(
    mlib_f32,
    mlib_f32,
    SORT2C,
    RANK_4_FUNC,
    r4_f32)
RANK_ROW(
    mlib_f32,
    mlib_f32,
    SORT2C,
    RANK_5_FUNC,
    r5_f32)
RANK_ROW(
    mlib_f32,
    mlib_f32,
    SORT2C,
    RANK_6_FUNC,
    r6_f32)
RANK_ROW(
    mlib_f32,
    mlib_f32,
    SORT2C,
    RANK_7_FUNC,
    r7_f32)

RANK_ROW(
    mlib_d64,
    mlib_d64,
    SORT2C,
    RANK_1_FUNC,
    r1_d64)
RANK_ROW(
    mlib_d64,
    mlib_d64,
    SORT2C,
    RANK_2_FUNC,
    r2_d64)
RANK_ROW(
    mlib_d64,
    mlib_d64,
    SORT2C,
    RANK_3_FUNC,
    r3_d64)
RANK_ROW(
    mlib_d64,
    mlib_d64,
    SORT2C,
    RANK_4_FUNC,
    r4_d64)
RANK_ROW(
    mlib_d64,
    mlib_d64,
    SORT2C,
    RANK_5_FUNC,
    r5_d64)
RANK_ROW(
    mlib_d64,
    mlib_d64,
    SORT2C,
    RANK_6_FUNC,
    r6_d64)
RANK_ROW(
    mlib_d64,
    mlib_d64,
    SORT2C,
    RANK_7_FUNC,
    r7_d64)

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGERANKFILTER_H */
