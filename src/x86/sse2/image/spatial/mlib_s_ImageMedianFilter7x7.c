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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident   "@(#)mlib_s_ImageMedianFilter7x7.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageMedianFilter7x7    - median filtering with 7x7 mask
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMedianFilter7x7(mlib_image       *dst,
 *                                            const mlib_image *src,
 *                                            mlib_median_mask mmask,
 *                                            mlib_s32         cmask,
 *                                            mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst     Pointer to destination image.
 *      src     Pointer to source image.
 *      mmask   Shape of the mask to be used for median filtering.
 *      cmask   Channel mask to indicate the channels to be filtered.
 *              Each bit of which represents a channel in the image. The
 *              channels corresponded to 1 bits are those to be processed.
 *      edge    Type of edge condition.
 *
 * DESCRIPTION
 *      Median filtering.
 *
 *      There are several shapes possible for the mask: rectangle mask,
 *      plus mask, and X mask.
 *
 *      The separable rectangle median is defined as the median of the
 *      medians of each row.
 *
 *      The unselected channels are not overwritten. If both src and dst have
 *      just one channel, cmask is ignored.
 *
 *      The mapping of source image to destination image is left/top corner to
 *      left/top corner, but with a shift of the destination image if it's
 *      smaller than the source image.
 *      The exact mapping formula is
 *           xs = xd + x_offset
 *           ys = yd + y_offset
 *      where
 *           x_offset = (dst_width >= src_width) ? 0 : (kernel_size - 1)/2
 *           y_offset = (dst_height >= src_height) ? 0 : (kernel_size - 1)/2
 *
 *      The src and the dst must be images of the same type.
 *      The src and the dst can be images of MLIB_BYTE, MLIB_SHORT,
 *      MLIB_USHORT or MLIB_INT type.
 *      The src and dst must have same number of channels (1, 2, 3, or 4).
 *
 *      The mask shape can be one of the following:
 *              MLIB_MEDIAN_MASK_RECT   (default)
 *              MLIB_MEDIAN_MASK_PLUS
 *              MLIB_MEDIAN_MASK_X
 *              MLIB_MEDIAN_MASK_RECT_SEPARABLE
 *
 *      The edge condition can be one of the following:
 *              MLIB_EDGE_DST_NO_WRITE  (default)
 *              MLIB_EDGE_DST_FILL_ZERO
 *              MLIB_EDGE_DST_COPY_SRC
 *              MLIB_EDGE_SRC_EXTEND
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_s_ImageMedianFilter.h>

/* *************************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMedianFilter7x7 = __mlib_ImageMedianFilter7x7

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageMedianFilter7x7) mlib_ImageMedianFilter7x7
    __attribute__((weak, alias("__mlib_ImageMedianFilter7x7")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	FUNC_MEDIAN_ROW(MEDMASK, img_type)                     \
	void                                                   \
	mlib_median_row_##MEDMASK##_##img_type(mlib_u8         \
	    *pdst, void **psl, mlib_filter_state * fs)         \
	{                                                      \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask,     \
			size = fs->size;                               \
	    mlib_##img_type *dl, *sl;                          \
	    mlib_##img_type *buff1, *buff2, *buff3,            \
			*buff4, *buff5, *buff6, *buff7;                \
	    mlib_##img_type d1, d2, d3, d4, d5, d6, d7, mask;  \
	    mlib_##img_type v1, v2, v3, v4, v5, v6;            \
	    mlib_s32 i, k;                                     \
	                                                       \
	    buff1 = (fs->buff_row)[fs->buff_ind - 6];          \
	    buff2 = (fs->buff_row)[fs->buff_ind - 5];          \
	    buff3 = (fs->buff_row)[fs->buff_ind - 4];          \
	    buff4 = (fs->buff_row)[fs->buff_ind - 3];          \
	    buff5 = (fs->buff_row)[fs->buff_ind - 2];          \
	    buff6 = (fs->buff_row)[fs->buff_ind - 1];          \
	    buff7 = (fs->buff_row)[fs->buff_ind];              \
	    for (k = 0; k < nchan; k++) {                      \
			if (!(cmask & (1 << (nchan - 1 - k))))         \
				continue;                                  \
			dl = (mlib_##img_type *) pdst + k;             \
			sl = (mlib_##img_type *) psl[6] + k;           \
			for (i = 0; i < size; i++) {                   \
				d1 = sl[-3 * nchan];                       \
				d2 = sl[-2 * nchan];                       \
				d3 = sl[-nchan];                           \
				d4 = sl[0];                                \
				d5 = sl[nchan];                            \
				d6 = sl[2 * nchan];                        \
				d7 = sl[3 * nchan];                        \
				v1 = buff1[i * nchan];                     \
				v2 = buff2[i * nchan];                     \
				v3 = buff3[i * nchan];                     \
				v4 = buff4[i * nchan];                     \
				v5 = buff5[i * nchan];                     \
				v6 = buff6[i * nchan];                     \
				SORT7(d1, d2, d3, d4, d5, d6, d7,          \
					SORT2_##img_type);                     \
				buff7[i * nchan] = d4;                     \
				SORT7(v1, v2, v3, v4, v5, v6, d4,          \
					SORT2_##img_type);                     \
				dl[i * nchan] = v4;                        \
				sl += nchan;                               \
			}                                              \
			buff1++;                                       \
			buff2++;                                       \
			buff3++;                                       \
			buff4++;                                       \
			buff5++;                                       \
			buff6++;                                       \
			buff7++;                                       \
	    }                                                  \
	    if (++(fs->buff_ind) >= 13)                        \
			(fs->buff_ind) -= 7;                           \
	}                                                      \
	                                                       \
/*                                                                     \
 * ************************************************************        \
 */                                                                \
	void                                                       \
	mlib_s_median_row_##MEDMASK##_##img_type(mlib_u8           \
	    *pdst, void **psl, mlib_filter_state * fs)             \
	{                                                          \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask,         \
			size = fs->size;                                   \
	    mlib_u8 *dp, *sp;                                      \
	    sse2_##img_type d1, d2, d3, d4,                        \
			d5, d6, d7, mask80, mask;                          \
	    sse2_##img_type v1, v2, v3, v4, v5, v6;                \
	    mlib_u8 *buff1, *buff2, *buff3, *buff4,                \
			*buff5, *buff6, *buff7;                            \
	    mlib_s32 bchan = nchan * sizeof (mlib_##img_type);     \
	    mlib_s32 bsize = bchan * size;                         \
	    mlib_s32 amask = (1 << nchan) - 1;                     \
	    mlib_s32 i;                                            \
	                                                           \
	    if ((cmask & amask) != amask) {                        \
			mlib_median_row_##MEDMASK##_##img_type(            \
			pdst, psl, fs);                                    \
			return;                                            \
	    }                                                      \
	    buff1 = (fs->buff_row)[fs->buff_ind - 6];              \
	    buff2 = (fs->buff_row)[fs->buff_ind - 5];              \
	    buff3 = (fs->buff_row)[fs->buff_ind - 4];              \
	    buff4 = (fs->buff_row)[fs->buff_ind - 3];              \
	    buff5 = (fs->buff_row)[fs->buff_ind - 2];              \
	    buff6 = (fs->buff_row)[fs->buff_ind - 1];              \
	    buff7 = (fs->buff_row)[fs->buff_ind];                  \
	    dp = pdst;                                             \
	    sp = psl[6];                                           \
	    amask =                                                \
			(sizeof (mlib_##img_type) ==                       \
			1) ? 0x80808080 : 0x80008000;                      \
	    mask80 = SSE2_SET_##img_type(amask);                   \
	    for (i = 0; i <= bsize - 16; i += 16) {                \
			d1 = SSE2_LOAD_##img_type((sp - 3 * bchan));       \
			d2 = SSE2_LOAD_##img_type((sp - 2 * bchan));       \
			d3 = SSE2_LOAD_##img_type((sp - bchan));           \
			d4 = SSE2_LOAD_##img_type((sp));                   \
			d5 = SSE2_LOAD_##img_type((sp + bchan));           \
			d6 = SSE2_LOAD_##img_type((sp + 2 * bchan));       \
			d7 = SSE2_LOAD_##img_type((sp + 3 * bchan));       \
			v1 = SSE2_LOAD_##img_type(buff1);                  \
			v2 = SSE2_LOAD_##img_type(buff2);                  \
			v3 = SSE2_LOAD_##img_type(buff3);                  \
			v4 = SSE2_LOAD_##img_type(buff4);                  \
			v5 = SSE2_LOAD_##img_type(buff5);                  \
			v6 = SSE2_LOAD_##img_type(buff6);                  \
			SSE2_SORT7(d1, d2, d3, d4, d5, d6, d7, img_type);  \
			SSE2_STORE_##img_type(buff7, d4);                  \
			SSE2_SORT7(v1, v2, v3, v4, v5, v6, d4, img_type);  \
			SSE2_STORE_##img_type(dp, v4);                     \
			sp += 16;                                          \
			dp += 16;                                          \
			buff1 += 16;                                       \
			buff2 += 16;                                       \
			buff3 += 16;                                       \
			buff4 += 16;                                       \
			buff5 += 16;                                       \
			buff6 += 16;                                       \
			buff7 += 16;                                       \
	    }                                                      \
	    for (; i < bsize; i += sizeof (mlib_##img_type)) {     \
			sorttype_##img_type d1, d2, d3,                    \
			d4, d5, d6, d7, mask;                              \
			sorttype_##img_type v1, v2, v3, v4, v5, v6;        \
	                                                           \
			d1 = *(mlib_##img_type *) (sp - 3 * bchan);        \
			d2 = *(mlib_##img_type *) (sp - 2 * bchan);        \
			d3 = *(mlib_##img_type *) (sp - bchan);            \
			d4 = *(mlib_##img_type *) (sp);                    \
			d5 = *(mlib_##img_type *) (sp + bchan);            \
			d6 = *(mlib_##img_type *) (sp + 2 * bchan);        \
			d7 = *(mlib_##img_type *) (sp + 3 * bchan);        \
			v1 = *(mlib_##img_type *) buff1;                   \
			v2 = *(mlib_##img_type *) buff2;                   \
			v3 = *(mlib_##img_type *) buff3;                   \
			v4 = *(mlib_##img_type *) buff4;                   \
			v5 = *(mlib_##img_type *) buff5;                   \
			v6 = *(mlib_##img_type *) buff6;                   \
			SORT7(d1, d2, d3, d4, d5, d6, d7,                  \
				SORT2_##img_type);                             \
			*(mlib_##img_type *) buff7 = d4;                   \
			SORT7(v1, v2, v3, v4, v5, v6, d4,                  \
				SORT2_##img_type);                             \
			*(mlib_##img_type *) dp = v4;                      \
			sp += sizeof (mlib_##img_type);                    \
			dp += sizeof (mlib_##img_type);                    \
			buff1 += sizeof (mlib_##img_type);                 \
			buff2 += sizeof (mlib_##img_type);                 \
			buff3 += sizeof (mlib_##img_type);                 \
			buff4 += sizeof (mlib_##img_type);                 \
			buff5 += sizeof (mlib_##img_type);                 \
			buff6 += sizeof (mlib_##img_type);                 \
			buff7 += sizeof (mlib_##img_type);                 \
	    }                                                      \
	    if (++(fs->buff_ind) >= 13)                            \
			(fs->buff_ind) -= 7;                               \
	}

/* *********************************************************** */

FUNC_MEDIAN_ROW(7x7rsep, u8)
FUNC_MEDIAN_ROW(7x7rsep, s16)
FUNC_MEDIAN_ROW(7x7rsep, u16)
FUNC_MEDIAN_ROW(7x7rsep, s32)
FUNC_MEDIAN_ROW(7x7rsep, f32)
FUNC_MEDIAN_ROW(7x7rsep, d64)

/* *********************************************************** */

#define	FUNC_MEDIAN_INITROW_7x7RSEP(img_type)                   \
	void                                                    \
	mlib_median_initrow_7x7rsep_##img_type(mlib_u8 *pdst,   \
	    void **psl, mlib_filter_state * fs)                 \
	{                                                       \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask,      \
			size = fs->size;                                \
	    mlib_##img_type *sl;                                \
	    mlib_##img_type *buff;                              \
	    mlib_s32 i, k, off1 = nchan, off2 = 2 * nchan,      \
			off3 = 3 * nchan;                               \
	    buff = (fs->buff_row)[fs->buff_ind];                \
	    for (k = 0; k < nchan; k++) {                       \
			if (!(cmask & (1 << (nchan - 1 - k))))          \
				continue;                                   \
			sl = (mlib_##img_type *) psl[0] + k;            \
			for (i = 0; i < size; i++) {                    \
				MEDIAN7(buff[i * nchan], sl[-off3],         \
				sl[-off2], sl[-off1], sl[0], sl[off1],      \
				sl[off2], sl[off3], img_type);              \
				sl += nchan;                                \
			}                                               \
			buff++;                                         \
	    }                                                   \
	    if (++(fs->buff_ind) >= 13)                         \
			(fs->buff_ind) -= 7;                            \
	}

/* *********************************************************** */

#define	_SSE2_MEDIAN_INITROW_7x7RSEP(img_type)                   \
	void                                                       \
	mlib_s_median_initrow_7x7rsep_##img_type(mlib_u8           \
	    *pdst, void **psl, mlib_filter_state * fs)             \
	{                                                          \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask,         \
			size = fs->size;                                   \
	    mlib_u8 *dp, *sp;                                      \
	    sse2_##img_type d1, d2, d3, d4,                        \
			d5, d6, d7, mask80, mask;                          \
	    mlib_u8 *buff;                                         \
	    mlib_s32 bchan = nchan * sizeof (mlib_##img_type);     \
	    mlib_s32 bsize = bchan * size;                         \
	    mlib_s32 amask = (1 << nchan) - 1;                     \
	    mlib_s32 i;                                            \
	                                                           \
	    if ((cmask & amask) != amask) {                        \
			mlib_median_initrow_7x7rsep_##img_type(            \
			pdst, psl, fs);                                    \
			return;                                            \
	    }                                                      \
		buff = (fs->buff_row)[fs->buff_ind];                   \
	    dp = buff;                                             \
	    sp = psl[0];                                           \
	    amask =                                                \
			(sizeof (mlib_##img_type) ==                       \
			1) ? 0x80808080 : 0x80008000;                      \
	    mask80 = SSE2_SET_##img_type(amask);                   \
	    for (i = 0; i <= bsize - 16; i += 16) {                \
			d1 = SSE2_LOAD_##img_type((sp - 3 * bchan));       \
			d2 = SSE2_LOAD_##img_type((sp - 2 * bchan));       \
			d3 = SSE2_LOAD_##img_type((sp - bchan));           \
			d4 = SSE2_LOAD_##img_type((sp));                   \
			d5 = SSE2_LOAD_##img_type((sp + bchan));           \
			d6 = SSE2_LOAD_##img_type((sp + 2 * bchan));       \
			d7 = SSE2_LOAD_##img_type((sp + 3 * bchan));       \
			SSE2_SORT7(d1, d2, d3, d4, d5, d6, d7, img_type);  \
			SSE2_STORE_##img_type(dp, d4);                     \
			sp += 16;                                          \
			dp += 16;                                          \
	    }                                                      \
	    for (; i < bsize; i += sizeof (mlib_##img_type)) {     \
			sorttype_##img_type d1, d2, d3,                    \
			d4, d5, d6, d7, mask;                              \
	                                                           \
			d1 = *(mlib_##img_type *) (sp - 3 * bchan);        \
			d2 = *(mlib_##img_type *) (sp - 2 * bchan);        \
			d3 = *(mlib_##img_type *) (sp - bchan);            \
			d4 = *(mlib_##img_type *) (sp);                    \
			d5 = *(mlib_##img_type *) (sp + bchan);            \
			d6 = *(mlib_##img_type *) (sp + 2 * bchan);        \
			d7 = *(mlib_##img_type *) (sp + 3 * bchan);        \
			SORT7(d1, d2, d3, d4, d5, d6, d7,                  \
				SORT2_##img_type);                             \
			*(mlib_##img_type *) dp = d4;                      \
			sp += sizeof (mlib_##img_type);                    \
			dp += sizeof (mlib_##img_type);                    \
	    }                                                      \
	    if (++(fs->buff_ind) >= 13)                            \
			(fs->buff_ind) -= 7;                               \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_COL_7x7RSEP(img_type)                       \
	void                                                    \
	mlib_median_col_7x7rsep_##img_type(                     \
		mlib_u8 *dl, void **psl,                            \
	    mlib_filter_state * fs)                             \
	{                                                       \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask,      \
			size = fs->size;                                \
	    mlib_s32 *x_off = fs->x_off, dlb = fs->dlp;         \
	    mlib_##img_type *dp, *sl;                           \
	    sorttype_##img_type a1, a2, a3, a4, a5, a6, a7;     \
	    mlib_s32 off0, off1, off2, off3,                    \
			off4, off5, off6, i, k;                         \
	                                                        \
	    off0 = x_off[0];                                    \
	    off1 = x_off[1];                                    \
	    off2 = x_off[2];                                    \
	    off3 = x_off[3];                                    \
	    off4 = x_off[4];                                    \
	    off5 = x_off[5];                                    \
	    off6 = x_off[6];                                    \
	    for (k = 0; k < nchan;                              \
			k++, off0++, off1++, off2++,                    \
			off3++, off4++, off5++, off6++) {               \
			if (!(cmask & (1 << (nchan - 1 - k))))          \
		    continue;                                       \
			dp = (mlib_##img_type *) dl + k;                \
			sl = (mlib_##img_type *) psl[0];                \
			MEDIAN7(a1, sl[off0], sl[off1],                 \
				sl[off2], sl[off3],                         \
				sl[off4], sl[off5],                         \
				sl[off6], img_type);                        \
			sl = (mlib_##img_type *) psl[1];                \
			MEDIAN7(a2, sl[off0], sl[off1],                 \
				sl[off2], sl[off3],                         \
				sl[off4], sl[off5],                         \
				sl[off6], img_type);                        \
			sl = (mlib_##img_type *) psl[2];                \
			MEDIAN7(a3, sl[off0], sl[off1],                 \
				sl[off2], sl[off3],                         \
				sl[off4], sl[off5],                         \
				sl[off6], img_type);                        \
			sl = (mlib_##img_type *) psl[3];                \
			MEDIAN7(a4, sl[off0], sl[off1],                 \
				sl[off2], sl[off3],                         \
				sl[off4], sl[off5],                         \
				sl[off6], img_type);                        \
			sl = (mlib_##img_type *) psl[4];                \
			MEDIAN7(a5, sl[off0], sl[off1],                 \
				sl[off2], sl[off3],                         \
				sl[off4], sl[off5],                         \
				sl[off6], img_type);                        \
			sl = (mlib_##img_type *) psl[5];                \
			MEDIAN7(a6, sl[off0], sl[off1],                 \
				sl[off2], sl[off3],                         \
				sl[off4], sl[off5],                         \
				sl[off6], img_type);                        \
			for (i = 6; i < 6 + size; i++) {                \
				sl = (mlib_##img_type *) psl[i];            \
				MEDIAN7(a7, sl[off0], sl[off1],             \
				sl[off2], sl[off3],                         \
				sl[off4], sl[off5],                         \
				sl[off6], img_type);                        \
				MEDIAN7(dp[0], a1, a2, a3, a4,              \
				a5, a6, a7, img_type);                      \
				a1 = a2;                                    \
				a2 = a3;                                    \
				a3 = a4;                                    \
				a4 = a5;                                    \
				a5 = a6;                                    \
				a6 = a7;                                    \
				dp += dlb;                                  \
			}                                               \
	    }                                                   \
	}

/* *********************************************************** */

FUNC_MEDIAN_COL_7x7RSEP(u8)
FUNC_MEDIAN_INITROW_7x7RSEP(u8)
_SSE2_MEDIAN_INITROW_7x7RSEP(u8)

FUNC_MEDIAN_COL_7x7RSEP(s16)
FUNC_MEDIAN_INITROW_7x7RSEP(s16)
_SSE2_MEDIAN_INITROW_7x7RSEP(s16)

FUNC_MEDIAN_COL_7x7RSEP(u16)
FUNC_MEDIAN_INITROW_7x7RSEP(u16)
_SSE2_MEDIAN_INITROW_7x7RSEP(u16)

FUNC_MEDIAN_COL_7x7RSEP(s32)
FUNC_MEDIAN_INITROW_7x7RSEP(s32)
_SSE2_MEDIAN_INITROW_7x7RSEP(s32)

FUNC_MEDIAN_COL_7x7RSEP(f32)
FUNC_MEDIAN_INITROW_7x7RSEP(f32)
_SSE2_MEDIAN_INITROW_7x7RSEP(f32)

FUNC_MEDIAN_COL_7x7RSEP(d64)
FUNC_MEDIAN_INITROW_7x7RSEP(d64)
_SSE2_MEDIAN_INITROW_7x7RSEP(d64)

/* *********************************************************** */

static mlib_median_fun_type median_row_7x7_funs[] = {
	NULL, NULL, NULL,
	mlib_s_median_row_7x7rsep_u8,
	NULL, NULL, NULL,
	mlib_s_median_row_7x7rsep_s16,
	NULL, NULL, NULL,
	mlib_s_median_row_7x7rsep_u16,
	NULL, NULL, NULL,
	mlib_s_median_row_7x7rsep_s32,
	NULL, NULL, NULL,
	mlib_s_median_row_7x7rsep_f32,
	NULL, NULL, NULL,
	mlib_s_median_row_7x7rsep_d64
};

static mlib_median_fun_type median_col_7x7_funs[] = {
	NULL, NULL, NULL,
	mlib_median_col_7x7rsep_u8,
	NULL, NULL, NULL,
	mlib_median_col_7x7rsep_s16,
	NULL, NULL, NULL,
	mlib_median_col_7x7rsep_u16,
	NULL, NULL, NULL,
	mlib_median_col_7x7rsep_s32,
	NULL, NULL, NULL,
	mlib_median_col_7x7rsep_f32,
	NULL, NULL, NULL,
	mlib_median_col_7x7rsep_d64
};

static mlib_median_fun_type median_init_7x7_funs[] = {
	mlib_s_median_initrow_7x7rsep_u8,
	mlib_s_median_initrow_7x7rsep_s16,
	mlib_s_median_initrow_7x7rsep_u16,
	mlib_s_median_initrow_7x7rsep_s32,
	mlib_s_median_initrow_7x7rsep_f32,
	mlib_s_median_initrow_7x7rsep_d64
};

/* *********************************************************** */

mlib_status
mlib_ImageMedianFilter7x7_alltypes(
    mlib_image *dst,
    mlib_image *src,
    mlib_median_mask mmask,
    mlib_s32 cmask,
    mlib_edge edge,
    mlib_s32 fp_version)
{
	mlib_filter_state fs;

	if (mmask == MLIB_MEDIAN_MASK_RECT_SEPARABLE) {

		fs.m = 7;
		fs.n = 7;
		fs.mmask = mmask;
		fs.cmask = cmask;
		fs.edge = edge;
		fs.fp_version = fp_version;
		fs.mn_version = 0;
		fs.row_funs = median_row_7x7_funs;
		fs.col_funs = median_col_7x7_funs;
		fs.init_funs = median_init_7x7_funs;

		return (mlib_ImageMedianFilter_fun(dst, src, &fs));

	} else {

		if (fp_version) {
			return __mlib_ImageMedianFilterMxN_Fp(dst, src, 7, 7,
			    mmask, cmask, edge);
		} else {
			return __mlib_ImageMedianFilterMxN(dst, src, 7, 7,
			    mmask, cmask, edge);
		}
	}
}

/* *********************************************************** */

mlib_status
__mlib_ImageMedianFilter7x7(
    mlib_image *dst,
    const mlib_image *src,
    mlib_median_mask mmask,
    mlib_s32 cmask,
    mlib_edge edge)
{
	return mlib_ImageMedianFilter7x7_alltypes(dst, (void *)src, mmask,
	    cmask, edge, 0);
}

/* *********************************************************** */
