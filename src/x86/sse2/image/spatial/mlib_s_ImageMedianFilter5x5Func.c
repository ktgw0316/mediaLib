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

#pragma ident   "@(#)mlib_s_ImageMedianFilter5x5Func.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *   Internal functions for mlib_ImageMedianFilter5x5
 *   on case of MLIB_MEDIAN_MASK_RECT_SEPARABLE.
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_s_ImageMedianFilter.h>

/* *********************************************************** */
#define	FUNC_MEDIAN_ROW(MEDMASK, img_type)                      \
	void                                                    \
	mlib_median_row_##MEDMASK##_##img_type(mlib_u8          \
	    *pdst, void **psl, mlib_filter_state * fs)          \
	{                                                       \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask,      \
			size = fs->size;                                \
	    mlib_##img_type *dl, *sl;                           \
	    mlib_##img_type *buff1, *buff2,                     \
			*buff3, *buff4, *buff5;                         \
	    mlib_##img_type mask, d1, d2, d3, d4,               \
			d5, d6, d7, d8, d9;                             \
	    mlib_s32 i, k;                                      \
	                                                        \
	    buff1 = (fs->buff_row)[fs->buff_ind - 4];           \
	    buff2 = (fs->buff_row)[fs->buff_ind - 3];           \
	    buff3 = (fs->buff_row)[fs->buff_ind - 2];           \
	    buff4 = (fs->buff_row)[fs->buff_ind - 1];           \
	    buff5 = (fs->buff_row)[fs->buff_ind];               \
	    for (k = 0; k < nchan; k++) {                       \
			if (!(cmask & (1 << (nchan - 1 - k))))          \
				continue;                                   \
			dl = (mlib_##img_type *) pdst + k;              \
			sl = (mlib_##img_type *) psl[4] + k;            \
			for (i = 0; i < size; i++) {                    \
				d1 = sl[-2 * nchan];                        \
				d2 = sl[-nchan];                            \
				d3 = sl[0];                                 \
				d4 = sl[nchan];                             \
				d5 = sl[2 * nchan];                         \
				d6 = buff1[i * nchan];                      \
				d7 = buff2[i * nchan];                      \
				d8 = buff3[i * nchan];                      \
				d9 = buff4[i * nchan];                      \
				SORT5(d1, d2, d3, d4, d5, SORT2_##img_type);\
				buff5[i * nchan] = d3;                      \
				SORT5(d6, d7, d8, d9, d3, SORT2_##img_type);\
				dl[i * nchan] = d8;                         \
				sl += nchan;                                \
			}                                               \
			buff1++;                                        \
			buff2++;                                        \
			buff3++;                                        \
			buff4++;                                        \
			buff5++;                                        \
	    }                                                   \
	    if (++(fs->buff_ind) >= 9)                          \
			(fs->buff_ind) -= 5;                            \
	}                                                       \

/* *********************************************************** */

#define	_SSE2_MEDIAN_ROW(MEDMASK, img_type)                      \
	void                                                        \
	mlib_s_median_row_##MEDMASK##_##img_type(mlib_u8            \
	    *pdst, void **psl, mlib_filter_state * fs)              \
	{                                                           \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask,          \
			size = fs->size;                                    \
	    mlib_u8 *dp, *sp;                                       \
	    sse2_##img_type mask80, mask, d1, d2, d3, d4, d5,       \
			d6, d7, d8, d9, dA;                                 \
	    mlib_u8 *buff1, *buff2, *buff3, *buff4, *buff5;         \
	    mlib_s32 bchan = nchan * sizeof (mlib_##img_type);      \
	    mlib_s32 bsize = bchan * size;                          \
	    mlib_s32 amask = (1 << nchan) - 1;                      \
	    mlib_s32 i;                                             \
	                                                            \
	    if ((cmask & amask) != amask) {                         \
			mlib_median_row_##MEDMASK##_##img_type(pdst,        \
			psl, fs);                                           \
			return;                                             \
	    }                                                       \
	    buff1 = (fs->buff_row)[fs->buff_ind - 4];               \
	    buff2 = (fs->buff_row)[fs->buff_ind - 3];               \
	    buff3 = (fs->buff_row)[fs->buff_ind - 2];               \
	    buff4 = (fs->buff_row)[fs->buff_ind - 1];               \
	    buff5 = (fs->buff_row)[fs->buff_ind];                   \
	    dp = pdst;                                              \
	    sp = psl[4];                                            \
	    amask =                                                 \
			(sizeof (mlib_##img_type) ==                        \
			1) ? 0x80808080 : 0x80008000;                       \
	    mask80 = SSE2_SET_##img_type(amask);                    \
	    for (i = 0; i <= bsize - 16; i += 16) {                 \
			d1 = SSE2_LOAD_##img_type((sp - 2 * bchan));        \
			d2 = SSE2_LOAD_##img_type((sp - bchan));            \
			d3 = SSE2_LOAD_##img_type((sp));                    \
			d4 = SSE2_LOAD_##img_type((sp + bchan));            \
			d5 = SSE2_LOAD_##img_type((sp + 2 * bchan));        \
			d6 = SSE2_LOAD_##img_type(buff1);                   \
			d7 = SSE2_LOAD_##img_type(buff2);                   \
			d8 = SSE2_LOAD_##img_type(buff3);                   \
			d9 = SSE2_LOAD_##img_type(buff4);                   \
			SSE2_SORT5(d1, d2, d3, d4, d5, img_type);           \
			dA = d3;                                            \
			SSE2_SORT5(d6, d7, d8, d9, dA, img_type);           \
			SSE2_STORE_##img_type(buff5, d3);                   \
			SSE2_STORE_##img_type(dp, d8);                      \
			sp += 16;                                           \
			dp += 16;                                           \
			buff1 += 16;                                        \
			buff2 += 16;                                        \
			buff3 += 16;                                        \
			buff4 += 16;                                        \
			buff5 += 16;                                        \
	    }                                                       \
	    for (; i < bsize; i += sizeof (mlib_##img_type)) {      \
			sorttype_##img_type mask, d1, d2, d3, d4, d5, d6,   \
			d7, d8, d9;                                         \
			d1 = *(mlib_##img_type *) (sp - 2 * bchan);         \
			d2 = *(mlib_##img_type *) (sp - bchan);             \
			d3 = *(mlib_##img_type *) (sp);                     \
			d4 = *(mlib_##img_type *) (sp + bchan);             \
			d5 = *(mlib_##img_type *) (sp + 2 * bchan);         \
			d6 = *(mlib_##img_type *) buff1;                    \
			d7 = *(mlib_##img_type *) buff2;                    \
			d8 = *(mlib_##img_type *) buff3;                    \
			d9 = *(mlib_##img_type *) buff4;                    \
			SORT5(d1, d2, d3, d4, d5, SORT2_##img_type);        \
			*(mlib_##img_type *) buff5 = d3;                    \
			SORT5(d6, d7, d8, d9, d3, SORT2_##img_type);        \
			*(mlib_##img_type *) dp = d8;                       \
			sp += sizeof (mlib_##img_type);                     \
			dp += sizeof (mlib_##img_type);                     \
			buff1 += sizeof (mlib_##img_type);                  \
			buff2 += sizeof (mlib_##img_type);                  \
			buff3 += sizeof (mlib_##img_type);                  \
			buff4 += sizeof (mlib_##img_type);                  \
			buff5 += sizeof (mlib_##img_type);                  \
	    }                                                       \
	    if (++(fs->buff_ind) >= 9)                              \
			(fs->buff_ind) -= 5;                                \
	}

/* *********************************************************** */

FUNC_MEDIAN_ROW(5x5rsep, u8)
FUNC_MEDIAN_ROW(5x5rsep, s16)
FUNC_MEDIAN_ROW(5x5rsep, u16)
FUNC_MEDIAN_ROW(5x5rsep, s32)
FUNC_MEDIAN_ROW(5x5rsep, f32)
FUNC_MEDIAN_ROW(5x5rsep, d64)

_SSE2_MEDIAN_ROW(5x5rsep, u8)
_SSE2_MEDIAN_ROW(5x5rsep, s16)
_SSE2_MEDIAN_ROW(5x5rsep, u16)
_SSE2_MEDIAN_ROW(5x5rsep, s32)
_SSE2_MEDIAN_ROW(5x5rsep, f32)
_SSE2_MEDIAN_ROW(5x5rsep, d64)

/* *********************************************************** */

#define	FUNC_MEDIAN_INITROW_5x5RSEP(img_type)                   \
	void                                                    \
	mlib_median_initrow_5x5rsep_##img_type(mlib_u8 *pdst,   \
	    void **psl, mlib_filter_state * fs)                 \
	{                                                       \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask,      \
			size = fs->size;                                \
	    mlib_##img_type *sl;                                \
	    sorttype_##img_type mask, d1, d2, d3, d4, d5;       \
	    mlib_##img_type *buff;                              \
	    mlib_s32 off0 = -2 * nchan, off1 = -nchan, off2 = 0;\
	    mlib_s32 off3 = nchan, off4 = 2 * nchan, i, k;      \
	                                                        \
	    buff = (fs->buff_row)[fs->buff_ind];                \
	    for (k = 0; k < nchan; k++) {                       \
			if (!(cmask & (1 << (nchan - 1 - k))))          \
				continue;                                   \
			sl = (mlib_##img_type *) psl[0] + k;            \
			for (i = 0; i < size; i++) {                    \
				d1 = sl[off0];                              \
				d2 = sl[off1];                              \
				d3 = sl[off2];                              \
				d4 = sl[off3];                              \
				d5 = sl[off4];                              \
				SORT5(d1, d2, d3, d4, d5, SORT2_##img_type);\
				buff[i * nchan] = d3;                       \
				sl += nchan;                                \
			}                                               \
			buff++;                                         \
	    }                                                   \
	    if (++(fs->buff_ind) >= 9)                          \
			(fs->buff_ind) -= 5;                            \
	}

/* *********************************************************** */

#define	_SSE2_MEDIAN_INITROW_5x5RSEP(img_type)                   \
	void                                                    \
	mlib_s_median_initrow_5x5rsep_##img_type(mlib_u8 *pdst,   \
	    void **psl, mlib_filter_state * fs)                 \
	{                                                           \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask,          \
			size = fs->size;                                    \
	    mlib_u8 *dp, *sp;                                       \
	    sse2_##img_type mask80, mask, d1, d2, d3, d4, d5;       \
	    mlib_u8 *buff;                                          \
	    mlib_s32 bchan = nchan * sizeof (mlib_##img_type);      \
	    mlib_s32 bsize = bchan * size;                          \
	    mlib_s32 amask = (1 << nchan) - 1;                      \
	    mlib_s32 i;                                             \
	                                                            \
	    if ((cmask & amask) != amask) {                         \
			mlib_median_initrow_5x5rsep_##img_type(pdst,        \
			psl, fs);                                           \
			return;                                             \
	    }                                                       \
		buff = (fs->buff_row)[fs->buff_ind];                    \
	    dp = buff;                                              \
	    sp = psl[0];                                            \
	    amask =                                                 \
			(sizeof (mlib_##img_type) ==                        \
			1) ? 0x80808080 : 0x80008000;                       \
	    mask80 = SSE2_SET_##img_type(amask);                    \
	    for (i = 0; i <= bsize - 16; i += 16) {                 \
			d1 = SSE2_LOAD_##img_type((sp - 2 * bchan));        \
			d2 = SSE2_LOAD_##img_type((sp - bchan));            \
			d3 = SSE2_LOAD_##img_type((sp));                    \
			d4 = SSE2_LOAD_##img_type((sp + bchan));            \
			d5 = SSE2_LOAD_##img_type((sp + 2 * bchan));        \
			SSE2_SORT5(d1, d2, d3, d4, d5, img_type);           \
			SSE2_STORE_##img_type(dp, d3);                      \
			sp += 16;                                           \
			dp += 16;                                           \
	    }                                                       \
	    for (; i < bsize; i += sizeof (mlib_##img_type)) {      \
			sorttype_##img_type mask, d1, d2, d3, d4, d5;       \
			d1 = *(mlib_##img_type *) (sp - 2 * bchan);         \
			d2 = *(mlib_##img_type *) (sp - bchan);             \
			d3 = *(mlib_##img_type *) (sp);                     \
			d4 = *(mlib_##img_type *) (sp + bchan);             \
			d5 = *(mlib_##img_type *) (sp + 2 * bchan);         \
			SORT5(d1, d2, d3, d4, d5, SORT2_##img_type);        \
			*(mlib_##img_type *) dp = d3;                       \
			sp += sizeof (mlib_##img_type);                     \
			dp += sizeof (mlib_##img_type);                     \
	    }                                                       \
	    if (++(fs->buff_ind) >= 9)                              \
			(fs->buff_ind) -= 5;                                \
	}


/* *********************************************************** */

#define	FUNC_MEDIAN_COL_5x5RSEP(img_type)                         \
	void                                                      \
	mlib_median_col_5x5rsep_##img_type(mlib_u8 *pdst,         \
	    void **psl, mlib_filter_state * fs)                   \
	{                                                         \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask,        \
			size = fs->size;                                  \
	    mlib_s32 *x_off = fs->x_off, dlb = fs->dlp;           \
	    mlib_##img_type *dp, *sl;                             \
	    sorttype_##img_type a1, a2, a3, a4, a5;               \
	    mlib_s32 off0, off1, off2, off3, off4, i, k;          \
	                                                          \
	    off0 = x_off[0];                                      \
	    off1 = x_off[1];                                      \
	    off2 = x_off[2];                                      \
	    off3 = x_off[3];                                      \
	    off4 = x_off[4];                                      \
	    for (k = 0; k < nchan;                                \
			k++, off0++, off1++, off2++, off3++, off4++) {    \
			if (!(cmask & (1 << (nchan - 1 - k))))            \
				continue;                                     \
			dp = (mlib_##img_type *) pdst + k;                \
			sl = (mlib_##img_type *) psl[0];                  \
			MEDIAN5(a1, sl[off0], sl[off1],                   \
				sl[off2], sl[off3],                           \
				sl[off4], img_type);                          \
			sl = (mlib_##img_type *) psl[1];                  \
			MEDIAN5(a2, sl[off0], sl[off1],                   \
				sl[off2], sl[off3],                           \
				sl[off4], img_type);                          \
			sl = (mlib_##img_type *) psl[2];                  \
			MEDIAN5(a3, sl[off0], sl[off1],                   \
				sl[off2], sl[off3],                           \
				sl[off4], img_type);                          \
			sl = (mlib_##img_type *) psl[3];                  \
			MEDIAN5(a4, sl[off0], sl[off1],                   \
				sl[off2], sl[off3],                           \
				sl[off4], img_type);                          \
			for (i = 4; i < 4 + size; i++) {                  \
				sl = (mlib_##img_type *) psl[i];              \
				MEDIAN5(a5, sl[off0], sl[off1],               \
				sl[off2], sl[off3],                           \
				sl[off4], img_type);                          \
				MEDIAN5(dp[0], a1, a2, a3,                    \
				a4, a5, img_type);                            \
				a1 = a2;                                      \
				a2 = a3;                                      \
				a3 = a4;                                      \
				a4 = a5;                                      \
				dp += dlb;                                    \
			}                                                 \
	    }                                                     \
	}

/* *********************************************************** */
    FUNC_MEDIAN_COL_5x5RSEP(u8)
    FUNC_MEDIAN_INITROW_5x5RSEP(u8)
    _SSE2_MEDIAN_INITROW_5x5RSEP(u8)

    FUNC_MEDIAN_COL_5x5RSEP(s16)
    FUNC_MEDIAN_INITROW_5x5RSEP(s16)
    _SSE2_MEDIAN_INITROW_5x5RSEP(s16)

    FUNC_MEDIAN_COL_5x5RSEP(u16)
    FUNC_MEDIAN_INITROW_5x5RSEP(u16)
    _SSE2_MEDIAN_INITROW_5x5RSEP(u16)

    FUNC_MEDIAN_COL_5x5RSEP(s32)
    FUNC_MEDIAN_INITROW_5x5RSEP(s32)
    _SSE2_MEDIAN_INITROW_5x5RSEP(s32)

    FUNC_MEDIAN_COL_5x5RSEP(f32)
    FUNC_MEDIAN_INITROW_5x5RSEP(f32)
    _SSE2_MEDIAN_INITROW_5x5RSEP(f32)

    FUNC_MEDIAN_COL_5x5RSEP(d64)
    FUNC_MEDIAN_INITROW_5x5RSEP(d64)
    _SSE2_MEDIAN_INITROW_5x5RSEP(d64)

/* *********************************************************** */
