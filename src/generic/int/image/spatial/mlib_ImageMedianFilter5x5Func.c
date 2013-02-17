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

#pragma ident   "@(#)mlib_ImageMedianFilter5x5Func.c	9.2    07/11/05 SMI"

/*
 * FUNCTION
 *   Internal functions for mlib_ImageMedianFilter5x5 on case of
 *   MLIB_MEDIAN_MASK_RECT_SEPARABLE.
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageMedianFilter.h>

/* *********************************************************** */
#if defined(_NO_LONGLONG)

#define	FTYPE mlib_f32
#define	DTYPE mlib_d64
#else

#define	FTYPE mlib_s32
#define	DTYPE mlib_s64
#endif
/* *********************************************************** */

void
mlib_median_row_5x5rsep_u8(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs)
{
	mlib_s32 nchan = fs->nchan, cmask = fs->cmask, size = fs->size;
	mlib_u8 *dl, *sl;
	mlib_s32 mask, d1, d2, d3, d4, d5, d6, d7, d8, d9;
	mlib_s32 *buff1, *buff2, *buff3, *buff4, *buff5;
	mlib_s32 off0 = -2 * nchan, off1 = -nchan, off2 = 0;
	mlib_s32 off3 = nchan, off4 = 2 * nchan, i, k;

	buff1 = (fs->buff_row)[fs->buff_ind - 4];
	buff2 = (fs->buff_row)[fs->buff_ind - 3];
	buff3 = (fs->buff_row)[fs->buff_ind - 2];
	buff4 = (fs->buff_row)[fs->buff_ind - 1];
	buff5 = (fs->buff_row)[fs->buff_ind];

	for (k = 0; k < nchan; k++) {
		if (!(cmask & (1 << (nchan - 1 - k))))
			continue;

		dl = (mlib_u8 *)pdst + k;
		sl = (mlib_u8 *)psl[4] + k;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < size; i++) {
			d1 = sl[off0];
			d2 = sl[off1];
			d3 = sl[off2];
			d4 = sl[off3];
			d5 = sl[off4];
			d6 = buff1[i * nchan];
			d7 = buff2[i * nchan];
			d8 = buff3[i * nchan];
			d9 = buff4[i * nchan];

			SORT5(d1, d2, d3, d4, d5, SORT2_u8);
			buff5[i * nchan] = d3;
			SORT5(d6, d7, d8, d9, d3, SORT2_u8);
			dl[i * nchan] = d8;
			sl += nchan;
		}

		buff1++;
		buff2++;
		buff3++;
		buff4++;
		buff5++;
	}

	if (++(fs->buff_ind) >= 9)
		(fs->buff_ind) -= 5;
}

/* *********************************************************** */

void
mlib_median_row_5x5rsep_s16(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs)
{
	mlib_s32 nchan = fs->nchan, cmask = fs->cmask, size = fs->size;
	mlib_s16 *dl, *sl;
	mlib_s32 mask, d1, d2, d3, d4, d5, d6, d7, d8, d9;
	mlib_s32 *buff1, *buff2, *buff3, *buff4, *buff5;
	mlib_s32 off0 = -2 * nchan, off1 = -nchan, off2 = 0;
	mlib_s32 off3 = nchan, off4 = 2 * nchan, i, k;

	buff1 = (fs->buff_row)[fs->buff_ind - 4];
	buff2 = (fs->buff_row)[fs->buff_ind - 3];
	buff3 = (fs->buff_row)[fs->buff_ind - 2];
	buff4 = (fs->buff_row)[fs->buff_ind - 1];
	buff5 = (fs->buff_row)[fs->buff_ind];

	for (k = 0; k < nchan; k++) {
		if (!(cmask & (1 << (nchan - 1 - k))))
			continue;

		dl = (mlib_s16 *)pdst + k;
		sl = (mlib_s16 *)psl[4] + k;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < size; i++) {
			d1 = sl[off0];
			d2 = sl[off1];
			d3 = sl[off2];
			d4 = sl[off3];
			d5 = sl[off4];
			d6 = buff1[i * nchan];
			d7 = buff2[i * nchan];
			d8 = buff3[i * nchan];
			d9 = buff4[i * nchan];

			SORT5(d1, d2, d3, d4, d5, SORT2_s16);
			buff5[i * nchan] = d3;
			SORT5(d6, d7, d8, d9, d3, SORT2_s16);
			dl[i * nchan] = d8;
			sl += nchan;
		}

		buff1++;
		buff2++;
		buff3++;
		buff4++;
		buff5++;
	}

	if (++(fs->buff_ind) >= 9)
		(fs->buff_ind) -= 5;
}

/* *********************************************************** */

void
mlib_median_row_5x5rsep_u16(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs)
{
	mlib_s32 nchan = fs->nchan, cmask = fs->cmask, size = fs->size;
	mlib_u16 *dl, *sl;
	mlib_s32 mask, d1, d2, d3, d4, d5, d6, d7, d8, d9;
	mlib_s32 *buff1, *buff2, *buff3, *buff4, *buff5;
	mlib_s32 off0 = -2 * nchan, off1 = -nchan, off2 = 0;
	mlib_s32 off3 = nchan, off4 = 2 * nchan, i, k;

	buff1 = (fs->buff_row)[fs->buff_ind - 4];
	buff2 = (fs->buff_row)[fs->buff_ind - 3];
	buff3 = (fs->buff_row)[fs->buff_ind - 2];
	buff4 = (fs->buff_row)[fs->buff_ind - 1];
	buff5 = (fs->buff_row)[fs->buff_ind];

	for (k = 0; k < nchan; k++) {
		if (!(cmask & (1 << (nchan - 1 - k))))
			continue;

		dl = (mlib_u16 *)pdst + k;
		sl = (mlib_u16 *)psl[4] + k;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < size; i++) {
			d1 = sl[off0];
			d2 = sl[off1];
			d3 = sl[off2];
			d4 = sl[off3];
			d5 = sl[off4];
			d6 = buff1[i * nchan];
			d7 = buff2[i * nchan];
			d8 = buff3[i * nchan];
			d9 = buff4[i * nchan];

			SORT5(d1, d2, d3, d4, d5, SORT2_u16);
			buff5[i * nchan] = d3;
			SORT5(d6, d7, d8, d9, d3, SORT2_u16);
			dl[i * nchan] = d8;
			sl += nchan;
		}

		buff1++;
		buff2++;
		buff3++;
		buff4++;
		buff5++;
	}

	if (++(fs->buff_ind) >= 9)
		(fs->buff_ind) -= 5;
}

/* *********************************************************** */

void
mlib_median_row_5x5rsep_s32(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs)
{
	mlib_s32 nchan = fs->nchan, cmask = fs->cmask, size = fs->size;
	mlib_s32 *dl, *sl;
	mlib_s32 mask, d1, d2, d3, d4, d5, d6, d7, d8, d9;
	mlib_s32 *buff1, *buff2, *buff3, *buff4, *buff5;
	mlib_s32 off0 = -2 * nchan, off1 = -nchan, off2 = 0;
	mlib_s32 off3 = nchan, off4 = 2 * nchan, i, k;

	buff1 = (fs->buff_row)[fs->buff_ind - 4];
	buff2 = (fs->buff_row)[fs->buff_ind - 3];
	buff3 = (fs->buff_row)[fs->buff_ind - 2];
	buff4 = (fs->buff_row)[fs->buff_ind - 1];
	buff5 = (fs->buff_row)[fs->buff_ind];

	for (k = 0; k < nchan; k++) {
		if (!(cmask & (1 << (nchan - 1 - k))))
			continue;

		dl = (mlib_s32 *)pdst + k;
		sl = (mlib_s32 *)psl[4] + k;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < size; i++) {
			d1 = sl[off0];
			d2 = sl[off1];
			d3 = sl[off2];
			d4 = sl[off3];
			d5 = sl[off4];
			d6 = buff1[i * nchan];
			d7 = buff2[i * nchan];
			d8 = buff3[i * nchan];
			d9 = buff4[i * nchan];

			SORT5(d1, d2, d3, d4, d5, SORT2_s32);
			buff5[i * nchan] = d3;
			SORT5(d6, d7, d8, d9, d3, SORT2_s32);
			dl[i * nchan] = d8;
			sl += nchan;
		}

		buff1++;
		buff2++;
		buff3++;
		buff4++;
		buff5++;
	}

	if (++(fs->buff_ind) >= 9)
		(fs->buff_ind) -= 5;
}

/* *********************************************************** */

void
mlib_median_row_5x5rsep_f32(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs)
{
	mlib_s32 nchan = fs->nchan, cmask = fs->cmask, size = fs->size;
	FTYPE *dl, *sl;
	FTYPE mask, d1, d2, d3, d4, d5, d6, d7, d8, d9;
	FTYPE *buff1, *buff2, *buff3, *buff4, *buff5;
	mlib_s32 off0 = -2 * nchan, off1 = -nchan, off2 = 0;
	mlib_s32 off3 = nchan, off4 = 2 * nchan, i, k;

	buff1 = (fs->buff_row)[fs->buff_ind - 4];
	buff2 = (fs->buff_row)[fs->buff_ind - 3];
	buff3 = (fs->buff_row)[fs->buff_ind - 2];
	buff4 = (fs->buff_row)[fs->buff_ind - 1];
	buff5 = (fs->buff_row)[fs->buff_ind];

	for (k = 0; k < nchan; k++) {
		if (!(cmask & (1 << (nchan - 1 - k))))
			continue;

		dl = (FTYPE *)pdst + k;
		sl = (FTYPE *)psl[4] + k;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < size; i++) {
			d1 = sl[off0];
			d2 = sl[off1];
			d3 = sl[off2];
			d4 = sl[off3];
			d5 = sl[off4];
			d6 = buff1[i * nchan];
			d7 = buff2[i * nchan];
			d8 = buff3[i * nchan];
			d9 = buff4[i * nchan];

			SORT5(d1, d2, d3, d4, d5, SORT2_f32);
			buff5[i * nchan] = d3;
			SORT5(d6, d7, d8, d9, d3, SORT2_f32);
			dl[i * nchan] = d8;
			sl += nchan;
		}

		buff1++;
		buff2++;
		buff3++;
		buff4++;
		buff5++;
	}

	if (++(fs->buff_ind) >= 9)
		(fs->buff_ind) -= 5;
}

/* *********************************************************** */

void
mlib_median_row_5x5rsep_d64(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs)
{
	mlib_s32 nchan = fs->nchan, cmask = fs->cmask, size = fs->size;
	DTYPE *dl, *sl;
	DTYPE mask, d1, d2, d3, d4, d5, d6, d7, d8, d9;
	DTYPE *buff1, *buff2, *buff3, *buff4, *buff5;
	mlib_s32 off0 = -2 * nchan, off1 = -nchan, off2 = 0;
	mlib_s32 off3 = nchan, off4 = 2 * nchan, i, k;

	buff1 = (fs->buff_row)[fs->buff_ind - 4];
	buff2 = (fs->buff_row)[fs->buff_ind - 3];
	buff3 = (fs->buff_row)[fs->buff_ind - 2];
	buff4 = (fs->buff_row)[fs->buff_ind - 1];
	buff5 = (fs->buff_row)[fs->buff_ind];

	for (k = 0; k < nchan; k++) {
		if (!(cmask & (1 << (nchan - 1 - k))))
			continue;

		dl = (DTYPE *)pdst + k;
		sl = (DTYPE *)psl[4] + k;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < size; i++) {
			d1 = sl[off0];
			d2 = sl[off1];
			d3 = sl[off2];
			d4 = sl[off3];
			d5 = sl[off4];
			d6 = buff1[i * nchan];
			d7 = buff2[i * nchan];
			d8 = buff3[i * nchan];
			d9 = buff4[i * nchan];

			SORT5(d1, d2, d3, d4, d5, SORT2_d64);
			buff5[i * nchan] = d3;
			SORT5(d6, d7, d8, d9, d3, SORT2_d64);
			dl[i * nchan] = d8;
			sl += nchan;
		}

		buff1++;
		buff2++;
		buff3++;
		buff4++;
		buff5++;
	}

	if (++(fs->buff_ind) >= 9)
		(fs->buff_ind) -= 5;
}

/* *********************************************************** */

#define	FUNC_MEDIAN_INITROW_5x5RSEP(img_type)                       \
	void                                                        \
	mlib_median_initrow_5x5rsep_##img_type(mlib_u8 *pdst,       \
	    void **psl, mlib_filter_state *fs)                      \
	{                                                           \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask,          \
		size = fs->size;                                    \
	    ptype_##img_type *sl;                                    \
	    sorttype_##img_type mask, d1, d2, d3, d4, d5;           \
	    sorttype_##img_type *buff;                              \
	    mlib_s32 off0 = -2 * nchan, off1 = -nchan, off2 = 0;    \
	    mlib_s32 off3 = nchan, off4 = 2 * nchan, i, k;          \
	                                                            \
	    buff = (fs->buff_row)[fs->buff_ind];                    \
	    for (k = 0; k < nchan; k++) {                           \
		if (!(cmask & (1 << (nchan - 1 - k))))              \
		    continue;                                       \
		sl = (ptype_##img_type *) psl[0] + k;                \
		for (i = 0; i < size; i++) {                        \
		    d1 = sl[off0];                                  \
		    d2 = sl[off1];                                  \
		    d3 = sl[off2];                                  \
		    d4 = sl[off3];                                  \
		    d5 = sl[off4];                                  \
		    SORT5(d1, d2, d3, d4, d5, SORT2_##img_type);    \
		    buff[i * nchan] = d3;                           \
		    sl += nchan;                                    \
		}                                                   \
		buff++;                                             \
	    }                                                       \
	    if (++(fs->buff_ind) >= 9)                              \
		(fs->buff_ind) -= 5;                                \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_COL_5x5RSEP(img_type)                                 \
	void                                                              \
	mlib_median_col_5x5rsep_##img_type(mlib_u8 *pdst,                 \
	    void **psl, mlib_filter_state *fs)                            \
	{                                                                 \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask,                \
		size = fs->size;                                          \
	    mlib_s32 *x_off = fs->x_off, dlb = fs->dlp;                   \
	    ptype_##img_type *dp, *sl;                                     \
	    sorttype_##img_type a1, a2, a3, a4, a5;                       \
	    mlib_s32 off0, off1, off2, off3, off4, i, k;                  \
	                                                                  \
	    off0 = x_off[0];                                              \
	    off1 = x_off[1];                                              \
	    off2 = x_off[2];                                              \
	    off3 = x_off[3];                                              \
	    off4 = x_off[4];                                              \
	    for (k = 0; k < nchan;                                        \
		k++, off0++, off1++, off2++, off3++, off4++) {            \
		if (!(cmask & (1 << (nchan - 1 - k))))                    \
		    continue;                                             \
		dp = (ptype_##img_type *) pdst + k;                        \
		sl = (ptype_##img_type *) psl[0];                          \
		MEDIAN5(a1, sl[off0], sl[off1], sl[off2], sl[off3],       \
		    sl[off4], img_type);                                  \
		sl = (ptype_##img_type *) psl[1];                          \
		MEDIAN5(a2, sl[off0], sl[off1], sl[off2], sl[off3],       \
		    sl[off4], img_type);                                  \
		sl = (ptype_##img_type *) psl[2];                          \
		MEDIAN5(a3, sl[off0], sl[off1], sl[off2], sl[off3],       \
		    sl[off4], img_type);                                  \
		sl = (ptype_##img_type *) psl[3];                          \
		MEDIAN5(a4, sl[off0], sl[off1], sl[off2], sl[off3],       \
		    sl[off4], img_type);                                  \
		for (i = 4; i < 4 + size; i++) {                          \
		    sl = (ptype_##img_type *) psl[i];                      \
		    MEDIAN5(a5, sl[off0], sl[off1], sl[off2], sl[off3],   \
			sl[off4], img_type);                              \
		    MEDIAN5(dp[0], a1, a2, a3, a4, a5, img_type);         \
		    a1 = a2;                                              \
		    a2 = a3;                                              \
		    a3 = a4;                                              \
		    a4 = a5;                                              \
		    dp += dlb;                                            \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

FUNC_MEDIAN_COL_5x5RSEP(u8)
FUNC_MEDIAN_INITROW_5x5RSEP(u8)

FUNC_MEDIAN_COL_5x5RSEP(s16)
FUNC_MEDIAN_INITROW_5x5RSEP(s16)

FUNC_MEDIAN_COL_5x5RSEP(u16)
FUNC_MEDIAN_INITROW_5x5RSEP(u16)

FUNC_MEDIAN_COL_5x5RSEP(s32)
FUNC_MEDIAN_INITROW_5x5RSEP(s32)

FUNC_MEDIAN_COL_5x5RSEP(f32)
FUNC_MEDIAN_INITROW_5x5RSEP(f32)

FUNC_MEDIAN_COL_5x5RSEP(d64)
FUNC_MEDIAN_INITROW_5x5RSEP(d64)

/* *********************************************************** */
