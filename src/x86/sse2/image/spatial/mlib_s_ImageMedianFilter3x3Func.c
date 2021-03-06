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

#pragma ident   "@(#)mlib_s_ImageMedianFilter3x3Func.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *   Internal functions for mlib_ImageMedianFilter3x3
 *   on case of MLIB_MEDIAN_MASK_RECT_SEPARABLE.
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_s_ImageMedianFilter.h>

/* *********************************************************** */
#define	FUNC_MEDIAN_ROW(MEDMASK, img_type)                \
	void                                                  \
	mlib_median_row_##MEDMASK##_##img_type(mlib_u8        \
	    *pdst, void **psl, mlib_filter_state * fs)        \
	{                                                     \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask,    \
			size = fs->size;                              \
	    mlib_##img_type *dl, *sl;                         \
	    mlib_##img_type *buff1, *buff2, *buff3;           \
	    mlib_##img_type mask, d1, d2, d3, d4, d5;         \
	    mlib_s32 i, k;                                    \
	                                                      \
	    buff1 = (fs->buff_row)[fs->buff_ind - 2];         \
	    buff2 = (fs->buff_row)[fs->buff_ind - 1];         \
	    buff3 = (fs->buff_row)[fs->buff_ind];             \
	    for (k = 0; k < nchan; k++) {                     \
			if (!(cmask & (1 << (nchan - 1 - k))))        \
				continue;                                 \
			dl = (mlib_##img_type *) pdst + k;            \
			sl = (mlib_##img_type *) psl[2] + k;          \
			for (i = 0; i < size; i++) {                  \
				d1 = sl[i * nchan - nchan];               \
				d2 = sl[i * nchan];                       \
				d3 = sl[i * nchan + nchan];               \
				d4 = buff1[i * nchan];                    \
				d5 = buff2[i * nchan];                    \
				SORT3(d1, d2, d3, SORT2_##img_type);      \
				buff3[i * nchan] = d2;                    \
				SORT3(d4, d2, d5, SORT2_##img_type);      \
				dl[i * nchan] = d2;                       \
			}                                             \
			buff1++;                                      \
			buff2++;                                      \
			buff3++;                                      \
	    }                                                 \
	    if (++(fs->buff_ind) >= 5)                        \
			(fs->buff_ind) -= 3;                          \
	}                                                     \
	                                                      \

/* *********************************************************** */

#define	_SSE2_MEDIAN_ROW(MEDMASK, img_type)                    \
	void                                                    \
	mlib_s_median_row_##MEDMASK##_##img_type(mlib_u8        \
	    *pdst, void **psl, mlib_filter_state * fs)          \
	{                                                       \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask,      \
			size = fs->size;                                \
	    mlib_u8 *dp, *sp;                                   \
	    sse2_##img_type mask80, mask, d1, d2, d3, d4, d5;   \
	    mlib_u8 *buff1, *buff2, *buff3;                     \
	    mlib_s32 bchan = nchan * sizeof (mlib_##img_type);  \
	    mlib_s32 bsize = bchan * size;                      \
	    mlib_s32 amask = (1 << nchan) - 1;                  \
	    mlib_s32 i;                                         \
	                                                        \
	    if ((cmask & amask) != amask) {                     \
			mlib_median_row_##MEDMASK##_##img_type(pdst,    \
				psl, fs);                                   \
			return;                                         \
	    }                                                   \
	    buff1 = (fs->buff_row)[fs->buff_ind - 2];           \
	    buff2 = (fs->buff_row)[fs->buff_ind - 1];           \
	    buff3 = (fs->buff_row)[fs->buff_ind];               \
	    dp = pdst;                                          \
	    sp = psl[2];                                        \
	    amask =                                             \
			(sizeof (mlib_##img_type) ==                    \
			1) ? 0x80808080 : 0x80008000;                   \
	    mask80 = SSE2_SET_##img_type(amask);                \
	    for (i = 0; i <= bsize - 16; i += 16) {             \
			d1 = SSE2_LOAD_##img_type((sp - bchan));        \
			d2 = SSE2_LOAD_##img_type((sp));                \
			d3 = SSE2_LOAD_##img_type((sp + bchan));        \
			d4 = SSE2_LOAD_##img_type(buff1);               \
			d5 = SSE2_LOAD_##img_type(buff2);               \
			SSE2_SORT3(d1, d2, d3, img_type);               \
			SSE2_STORE_##img_type(buff3, d2);               \
			SSE2_SORT3(d4, d2, d5, img_type);               \
			SSE2_STORE_##img_type(dp, d2);                  \
			sp += 16;                                       \
			dp += 16;                                       \
			buff1 += 16;                                    \
			buff2 += 16;                                    \
			buff3 += 16;                                    \
	    }                                                   \
	    for (; i < bsize; i += sizeof (mlib_##img_type)) {  \
			sorttype_##img_type mask, d1, d2, d3, d4, d5;   \
	                                                        \
			d1 = *(mlib_##img_type *) (sp - bchan);         \
			d2 = *(mlib_##img_type *) (sp);                 \
			d3 = *(mlib_##img_type *) (sp + bchan);         \
			d4 = *(mlib_##img_type *) buff1;                \
			d5 = *(mlib_##img_type *) buff2;                \
			SORT3(d1, d2, d3, SORT2_##img_type);            \
			*(mlib_##img_type *) buff3 = d2;                \
			SORT3(d4, d2, d5, SORT2_##img_type);            \
			*(mlib_##img_type *) dp = d2;                   \
			sp += sizeof (mlib_##img_type);                 \
			dp += sizeof (mlib_##img_type);                 \
			buff1 += sizeof (mlib_##img_type);              \
			buff2 += sizeof (mlib_##img_type);              \
			buff3 += sizeof (mlib_##img_type);              \
	    }                                                   \
	    if (++(fs->buff_ind) >= 5)                          \
			(fs->buff_ind) -= 3;                            \
	}

/* *********************************************************** */

FUNC_MEDIAN_ROW(3x3rsep, u8)
FUNC_MEDIAN_ROW(3x3rsep, s16)
FUNC_MEDIAN_ROW(3x3rsep, u16)
FUNC_MEDIAN_ROW(3x3rsep, s32)
FUNC_MEDIAN_ROW(3x3rsep, f32)
FUNC_MEDIAN_ROW(3x3rsep, d64)

_SSE2_MEDIAN_ROW(3x3rsep, u8)
_SSE2_MEDIAN_ROW(3x3rsep, s16)
_SSE2_MEDIAN_ROW(3x3rsep, u16)
_SSE2_MEDIAN_ROW(3x3rsep, s32)
_SSE2_MEDIAN_ROW(3x3rsep, f32)
_SSE2_MEDIAN_ROW(3x3rsep, d64)

/* *********************************************************** */
#define	FUNC_MEDIAN_INITROW_3x3RSEP(img_type)                       \
	void                                                    \
	mlib_median_initrow_3x3rsep_##img_type(mlib_u8 *pdst,   \
	    void **psl, mlib_filter_state * fs)                 \
	{                                                       \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask,      \
			size = fs->size;                                \
	    mlib_##img_type *sl;                                \
	    sorttype_##img_type mask, d1, d2, d3;               \
	    mlib_##img_type *buff;                              \
	    mlib_s32 i, k;                                      \
	                                                        \
	    buff = (fs->buff_row)[fs->buff_ind];                \
	    for (k = 0; k < nchan; k++) {                       \
			if (!(cmask & (1 << (nchan - 1 - k))))          \
				continue;                                   \
			sl = (mlib_##img_type *) psl[0] + k;            \
			for (i = 0; i < size; i++) {                    \
				d1 = sl[i * nchan - nchan];                 \
				d2 = sl[i * nchan];                         \
				d3 = sl[i * nchan + nchan];                 \
				SORT3(d1, d2, d3, SORT2_##img_type);        \
				buff[i * nchan] = d2;                       \
			}                                               \
			buff++;                                         \
	    }                                                   \
	    if (++(fs->buff_ind) >= 5)                          \
			(fs->buff_ind) -= 3;                            \
	}

/* *********************************************************** */

#define	_SSE2_MEDIAN_INITROW_3x3RSEP(img_type)                 \
	void                                                      \
	mlib_s_median_initrow_3x3rsep_##img_type(mlib_u8          \
	    *pdst, void **psl, mlib_filter_state * fs)            \
	{                                                         \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask,        \
			size = fs->size;                                  \
	    mlib_u8 *dp, *sp;                                     \
	    sse2_##img_type mask80, mask, d1, d2, d3;             \
	    mlib_s32 bchan = nchan * sizeof (mlib_##img_type);    \
	    mlib_s32 bsize = bchan * size;                        \
	    mlib_s32 amask = (1 << nchan) - 1;                    \
	    mlib_s32 i;                                           \
	                                                          \
	    if ((cmask & amask) != amask) {                       \
			mlib_median_initrow_3x3rsep_##img_type(           \
				pdst, psl, fs);                               \
			return;                                           \
	    }                                                     \
	    dp = (fs->buff_row)[fs->buff_ind];                    \
	    sp = psl[0];                                          \
	    amask =                                               \
			(sizeof (mlib_##img_type) ==                      \
			1) ? 0x80808080 : 0x80008000;                     \
	    mask80 = SSE2_SET_##img_type(amask);                  \
	    for (i = 0; i <= bsize - 16; i += 16) {               \
			d1 = SSE2_LOAD_##img_type((sp - bchan));          \
			d2 = SSE2_LOAD_##img_type((sp));                  \
			d3 = SSE2_LOAD_##img_type((sp + bchan));          \
			SSE2_SORT3(d1, d2, d3, img_type);                 \
			SSE2_STORE_##img_type(dp, d2);                    \
			sp += 16;                                         \
			dp += 16;                                         \
	    }                                                     \
	    for (; i < bsize; i += sizeof (mlib_##img_type)) {    \
			sorttype_##img_type mask, d1, d2, d3, d4, d5;     \
	                                                          \
			d1 = *(mlib_##img_type *) (sp - bchan);           \
			d2 = *(mlib_##img_type *) (sp);                   \
			d3 = *(mlib_##img_type *) (sp + bchan);           \
			SORT3(d1, d2, d3, SORT2_##img_type);              \
			*(mlib_##img_type *) dp = d2;                     \
			sp += sizeof (mlib_##img_type);                   \
			dp += sizeof (mlib_##img_type);                   \
	    }                                                     \
	    if (++(fs->buff_ind) >= 5)                            \
			(fs->buff_ind) -= 3;                              \
	}

/* *********************************************************** */


#define	FUNC_MEDIAN_COL_3x3RSEP(img_type)                       \
	void                                                    \
	mlib_median_col_3x3rsep_##img_type(mlib_u8 *pdst,       \
	    void **psl, mlib_filter_state * fs)                 \
	{                                                       \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask,      \
			size = fs->size;                                \
	    mlib_s32 *x_off = fs->x_off, dlb = fs->dlp;         \
	    mlib_##img_type *dp, *sl;                           \
	    sorttype_##img_type a1, a2, a3;                     \
	    mlib_s32 x_off0, x_off1, x_off2;                    \
	    mlib_s32 i, k;                                      \
	                                                        \
	    x_off0 = x_off[0];                                  \
	    x_off1 = x_off[1];                                  \
	    x_off2 = x_off[2];                                  \
	    for (k = 0; k < nchan;                              \
			k++, x_off0++, x_off1++, x_off2++) {            \
			if (!(cmask & (1 << (nchan - 1 - k))))          \
				continue;                                   \
			dp = (mlib_##img_type *) pdst + k;              \
			sl = (mlib_##img_type *) psl[0];                \
			MEDIAN3(a1, sl[x_off0], sl[x_off1], sl[x_off2], \
				img_type);                                  \
			sl = (mlib_##img_type *) psl[1];                \
			MEDIAN3(a2, sl[x_off0], sl[x_off1], sl[x_off2], \
				img_type);                                  \
			for (i = 2; i < 2 + size; i++) {                \
				sl = (mlib_##img_type *) psl[i];            \
				MEDIAN3(a3, sl[x_off0], sl[x_off1],         \
				sl[x_off2], img_type);                      \
				MEDIAN3(dp[0], a1, a2, a3, img_type);       \
				a1 = a2;                                    \
				a2 = a3;                                    \
				dp += dlb;                                  \
			}                                               \
	    }                                                   \
	}

/* *********************************************************** */

    FUNC_MEDIAN_COL_3x3RSEP(u8)
    FUNC_MEDIAN_INITROW_3x3RSEP(u8)
    _SSE2_MEDIAN_INITROW_3x3RSEP(u8)

    FUNC_MEDIAN_COL_3x3RSEP(s16)
    FUNC_MEDIAN_INITROW_3x3RSEP(s16)
    _SSE2_MEDIAN_INITROW_3x3RSEP(s16)

    FUNC_MEDIAN_COL_3x3RSEP(u16)
    FUNC_MEDIAN_INITROW_3x3RSEP(u16)
    _SSE2_MEDIAN_INITROW_3x3RSEP(u16)

    FUNC_MEDIAN_COL_3x3RSEP(s32)
    FUNC_MEDIAN_INITROW_3x3RSEP(s32)
    _SSE2_MEDIAN_INITROW_3x3RSEP(s32)

    FUNC_MEDIAN_COL_3x3RSEP(f32)
    FUNC_MEDIAN_INITROW_3x3RSEP(f32)
    _SSE2_MEDIAN_INITROW_3x3RSEP(f32)

    FUNC_MEDIAN_COL_3x3RSEP(d64)
    FUNC_MEDIAN_INITROW_3x3RSEP(d64)
    _SSE2_MEDIAN_INITROW_3x3RSEP(d64)

/* *********************************************************** */
