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

#pragma ident	"@(#)mlib_ImageMedianFilter7x7.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageMedianFilter7x7    - median filtering with 7x7 mask
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMedianFilter7x7(mlib_image       *dst,
 *                                            mlib_image       *src,
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
 *      The src and the dst can be images of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT
 *      or MLIB_INT type.
 *
 *      The src and dst must have same number of channels (1, 2, 3, or 4).
 *      The unselected channels in dst images are not overwritten. If both
 *      src and dst have just one channel, cmask is ignored.
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
#include <mlib_ImageMedianFilter.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMedianFilter7x7 = __mlib_ImageMedianFilter7x7

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMedianFilter7x7) mlib_ImageMedianFilter7x7
    __attribute__((weak, alias("__mlib_ImageMedianFilter7x7")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_median_row_7x7rsep_u8(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

static void mlib_median_row_7x7rsep_s16(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

static void mlib_median_row_7x7rsep_u16(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

static void mlib_median_row_7x7rsep_s32(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

static void mlib_median_row_7x7rsep_f32(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

static void mlib_median_row_7x7rsep_d64(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

/* *********************************************************** */

void
mlib_median_row_7x7rsep_u8(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs)
{
	mlib_s32 nchan = fs->nchan, cmask = fs->cmask, size = fs->size;
	mlib_u8 *dl, *sl;
	mlib_s32 *buff1, *buff2, *buff3, *buff4, *buff5, *buff6, *buff7, mrow;
	mlib_s32 k, i, off1 = nchan, off2 = 2 * nchan, off3 = 3 * nchan;

	buff1 = (fs->buff_row)[fs->buff_ind - 6];
	buff2 = (fs->buff_row)[fs->buff_ind - 5];
	buff3 = (fs->buff_row)[fs->buff_ind - 4];
	buff4 = (fs->buff_row)[fs->buff_ind - 3];
	buff5 = (fs->buff_row)[fs->buff_ind - 2];
	buff6 = (fs->buff_row)[fs->buff_ind - 1];
	buff7 = (fs->buff_row)[fs->buff_ind];

	for (k = 0; k < nchan; k++) {
		if (!(cmask & (1 << (nchan - 1 - k))))
			continue;

		dl = (mlib_u8 *)pdst + k;
		sl = (mlib_u8 *)psl[6] + k;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < size; i++) {
			MEDIAN7(mrow, sl[-off3], sl[-off2], sl[-off1], sl[0],
			    sl[off1], sl[off2], sl[off3], u8);
			buff7[i * nchan] = mrow;

			MEDIAN7(dl[i * nchan], buff1[i * nchan],
			    buff2[i * nchan], buff3[i * nchan], mrow,
			    buff4[i * nchan], buff5[i * nchan],
			    buff6[i * nchan], u8);
			sl += nchan;
		}

		buff1++;
		buff2++;
		buff3++;
		buff4++;
		buff5++;
		buff6++;
		buff7++;
	}

	if (++(fs->buff_ind) >= 13)
		(fs->buff_ind) -= 7;
}

/* *********************************************************** */

void
mlib_median_row_7x7rsep_s16(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs)
{
	mlib_s32 nchan = fs->nchan, cmask = fs->cmask, size = fs->size;
	mlib_s16 *dl, *sl;
	mlib_s32 *buff1, *buff2, *buff3, *buff4, *buff5, *buff6, *buff7, mrow;
	mlib_s32 k, i, off1 = nchan, off2 = 2 * nchan, off3 = 3 * nchan;

	buff1 = (fs->buff_row)[fs->buff_ind - 6];
	buff2 = (fs->buff_row)[fs->buff_ind - 5];
	buff3 = (fs->buff_row)[fs->buff_ind - 4];
	buff4 = (fs->buff_row)[fs->buff_ind - 3];
	buff5 = (fs->buff_row)[fs->buff_ind - 2];
	buff6 = (fs->buff_row)[fs->buff_ind - 1];
	buff7 = (fs->buff_row)[fs->buff_ind];

	for (k = 0; k < nchan; k++) {
		if (!(cmask & (1 << (nchan - 1 - k))))
			continue;

		dl = (mlib_s16 *)pdst + k;
		sl = (mlib_s16 *)psl[6] + k;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < size; i++) {
			MEDIAN7(mrow, sl[-off3], sl[-off2], sl[-off1], sl[0],
			    sl[off1], sl[off2], sl[off3], s16);
			buff7[i * nchan] = mrow;

			MEDIAN7(dl[i * nchan], buff1[i * nchan],
			    buff2[i * nchan], buff3[i * nchan], mrow,
			    buff4[i * nchan], buff5[i * nchan],
			    buff6[i * nchan], s16);
			sl += nchan;
		}

		buff1++;
		buff2++;
		buff3++;
		buff4++;
		buff5++;
		buff6++;
		buff7++;
	}

	if (++(fs->buff_ind) >= 13)
		(fs->buff_ind) -= 7;
}

/* *********************************************************** */

void
mlib_median_row_7x7rsep_u16(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs)
{
	mlib_s32 nchan = fs->nchan, cmask = fs->cmask, size = fs->size;
	mlib_u16 *dl, *sl;
	mlib_s32 *buff1, *buff2, *buff3, *buff4, *buff5, *buff6, *buff7, mrow;
	mlib_s32 k, i, off1 = nchan, off2 = 2 * nchan, off3 = 3 * nchan;

	buff1 = (fs->buff_row)[fs->buff_ind - 6];
	buff2 = (fs->buff_row)[fs->buff_ind - 5];
	buff3 = (fs->buff_row)[fs->buff_ind - 4];
	buff4 = (fs->buff_row)[fs->buff_ind - 3];
	buff5 = (fs->buff_row)[fs->buff_ind - 2];
	buff6 = (fs->buff_row)[fs->buff_ind - 1];
	buff7 = (fs->buff_row)[fs->buff_ind];

	for (k = 0; k < nchan; k++) {
		if (!(cmask & (1 << (nchan - 1 - k))))
			continue;

		dl = (mlib_u16 *)pdst + k;
		sl = (mlib_u16 *)psl[6] + k;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < size; i++) {
			MEDIAN7(mrow, sl[-off3], sl[-off2], sl[-off1], sl[0],
			    sl[off1], sl[off2], sl[off3], u16);
			buff7[i * nchan] = mrow;

			MEDIAN7(dl[i * nchan], buff1[i * nchan],
			    buff2[i * nchan], buff3[i * nchan], mrow,
			    buff4[i * nchan], buff5[i * nchan],
			    buff6[i * nchan], u16);
			sl += nchan;
		}

		buff1++;
		buff2++;
		buff3++;
		buff4++;
		buff5++;
		buff6++;
		buff7++;
	}

	if (++(fs->buff_ind) >= 13)
		(fs->buff_ind) -= 7;
}

/* *********************************************************** */

void
mlib_median_row_7x7rsep_s32(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs)
{
	mlib_s32 nchan = fs->nchan, cmask = fs->cmask, size = fs->size;
	mlib_s32 *dl, *sl;
	mlib_s32 *buff1, *buff2, *buff3, *buff4, *buff5, *buff6, *buff7, mrow;
	mlib_s32 k, i, off1 = nchan, off2 = 2 * nchan, off3 = 3 * nchan;

	buff1 = (fs->buff_row)[fs->buff_ind - 6];
	buff2 = (fs->buff_row)[fs->buff_ind - 5];
	buff3 = (fs->buff_row)[fs->buff_ind - 4];
	buff4 = (fs->buff_row)[fs->buff_ind - 3];
	buff5 = (fs->buff_row)[fs->buff_ind - 2];
	buff6 = (fs->buff_row)[fs->buff_ind - 1];
	buff7 = (fs->buff_row)[fs->buff_ind];

	for (k = 0; k < nchan; k++) {
		if (!(cmask & (1 << (nchan - 1 - k))))
			continue;

		dl = (mlib_s32 *)pdst + k;
		sl = (mlib_s32 *)psl[6] + k;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < size; i++) {
			MEDIAN7(mrow, sl[-off3], sl[-off2], sl[-off1], sl[0],
			    sl[off1], sl[off2], sl[off3], s32);
			buff7[i * nchan] = mrow;

			MEDIAN7(dl[i * nchan], buff1[i * nchan],
			    buff2[i * nchan], buff3[i * nchan], mrow,
			    buff4[i * nchan], buff5[i * nchan],
			    buff6[i * nchan], s32);
			sl += nchan;
		}

		buff1++;
		buff2++;
		buff3++;
		buff4++;
		buff5++;
		buff6++;
		buff7++;
	}

	if (++(fs->buff_ind) >= 13)
		(fs->buff_ind) -= 7;
}

/* *********************************************************** */

void
mlib_median_row_7x7rsep_f32(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs)
{
	mlib_s32 nchan = fs->nchan, cmask = fs->cmask, size = fs->size;
	mlib_f32 *dl, *sl;
	mlib_f32 *buff1, *buff2, *buff3, *buff4, *buff5, *buff6, *buff7, mrow;
	mlib_s32 k, i, off1 = nchan, off2 = 2 * nchan, off3 = 3 * nchan;

	buff1 = (fs->buff_row)[fs->buff_ind - 6];
	buff2 = (fs->buff_row)[fs->buff_ind - 5];
	buff3 = (fs->buff_row)[fs->buff_ind - 4];
	buff4 = (fs->buff_row)[fs->buff_ind - 3];
	buff5 = (fs->buff_row)[fs->buff_ind - 2];
	buff6 = (fs->buff_row)[fs->buff_ind - 1];
	buff7 = (fs->buff_row)[fs->buff_ind];

	for (k = 0; k < nchan; k++) {
		if (!(cmask & (1 << (nchan - 1 - k))))
			continue;

		dl = (mlib_f32 *)pdst + k;
		sl = (mlib_f32 *)psl[6] + k;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < size; i++) {
			MEDIAN7(mrow, sl[-off3], sl[-off2], sl[-off1], sl[0],
			    sl[off1], sl[off2], sl[off3], f32);
			buff7[i * nchan] = mrow;

			MEDIAN7(dl[i * nchan], buff1[i * nchan],
			    buff2[i * nchan], buff3[i * nchan], mrow,
			    buff4[i * nchan], buff5[i * nchan],
			    buff6[i * nchan], f32);
			sl += nchan;
		}

		buff1++;
		buff2++;
		buff3++;
		buff4++;
		buff5++;
		buff6++;
		buff7++;
	}

	if (++(fs->buff_ind) >= 13)
		(fs->buff_ind) -= 7;
}

/* *********************************************************** */

void
mlib_median_row_7x7rsep_d64(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs)
{
	mlib_s32 nchan = fs->nchan, cmask = fs->cmask, size = fs->size;
	mlib_d64 *dl, *sl;
	mlib_d64 *buff1, *buff2, *buff3, *buff4, *buff5, *buff6, *buff7, mrow;
	mlib_s32 k, i, off1 = nchan, off2 = 2 * nchan, off3 = 3 * nchan;

	buff1 = (fs->buff_row)[fs->buff_ind - 6];
	buff2 = (fs->buff_row)[fs->buff_ind - 5];
	buff3 = (fs->buff_row)[fs->buff_ind - 4];
	buff4 = (fs->buff_row)[fs->buff_ind - 3];
	buff5 = (fs->buff_row)[fs->buff_ind - 2];
	buff6 = (fs->buff_row)[fs->buff_ind - 1];
	buff7 = (fs->buff_row)[fs->buff_ind];

	for (k = 0; k < nchan; k++) {
		if (!(cmask & (1 << (nchan - 1 - k))))
			continue;

		dl = (mlib_d64 *)pdst + k;
		sl = (mlib_d64 *)psl[6] + k;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < size; i++) {
			MEDIAN7(mrow, sl[-off3], sl[-off2], sl[-off1], sl[0],
			    sl[off1], sl[off2], sl[off3], d64);
			buff7[i * nchan] = mrow;

			MEDIAN7(dl[i * nchan], buff1[i * nchan],
			    buff2[i * nchan], buff3[i * nchan], mrow,
			    buff4[i * nchan], buff5[i * nchan],
			    buff6[i * nchan], d64);
			sl += nchan;
		}

		buff1++;
		buff2++;
		buff3++;
		buff4++;
		buff5++;
		buff6++;
		buff7++;
	}

	if (++(fs->buff_ind) >= 13)
		(fs->buff_ind) -= 7;
}

/* *********************************************************** */

#define	FUNC_MEDIAN_INITROW_7x7RSEP(img_type)                             \
	static void                                                       \
	mlib_median_initrow_7x7rsep_##img_type(mlib_u8 *pdst,             \
	    void **psl, mlib_filter_state *fs)                            \
	{                                                                 \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask,                \
		size = fs->size;                                          \
	    mlib_##img_type *sl;                                          \
	    sorttype_##img_type *buff;                                    \
	    mlib_s32 i, k, off1 = nchan, off2 = 2 * nchan, off3 =         \
		3 * nchan;                                                \
	    buff = (fs->buff_row)[fs->buff_ind];                          \
	    for (k = 0; k < nchan; k++) {                                 \
		if (!(cmask & (1 << (nchan - 1 - k))))                    \
		    continue;                                             \
		sl = (mlib_##img_type *) psl[0] + k;                      \
		for (i = 0; i < size; i++) {                              \
		    MEDIAN7(buff[i * nchan], sl[-off3], sl[-off2],        \
			sl[-off1], sl[0], sl[off1], sl[off2], sl[off3],   \
			img_type);                                        \
		    sl += nchan;                                          \
		}                                                         \
		buff++;                                                   \
	    }                                                             \
	    if (++(fs->buff_ind) >= 13)                                   \
		(fs->buff_ind) -= 7;                                      \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_COL_7x7RSEP(img_type)                                 \
	static void                                                       \
	mlib_median_col_7x7rsep_##img_type(mlib_u8 *dl, void **psl,       \
	    mlib_filter_state *fs)                                        \
	{                                                                 \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask,                \
		size = fs->size;                                          \
	    mlib_s32 *x_off = fs->x_off, dlb = fs->dlp;                   \
	    mlib_##img_type *dp, *sl;                                     \
	    sorttype_##img_type a1, a2, a3, a4, a5, a6, a7;               \
	    mlib_s32 off0, off1, off2, off3, off4, off5, off6, i, k;      \
	                                                                  \
	    off0 = x_off[0];                                              \
	    off1 = x_off[1];                                              \
	    off2 = x_off[2];                                              \
	    off3 = x_off[3];                                              \
	    off4 = x_off[4];                                              \
	    off5 = x_off[5];                                              \
	    off6 = x_off[6];                                              \
	    for (k = 0; k < nchan;                                        \
		k++, off0++, off1++, off2++, off3++, off4++, off5++,      \
		off6++) {                                                 \
		if (!(cmask & (1 << (nchan - 1 - k))))                    \
		    continue;                                             \
		dp = (mlib_##img_type *) dl + k;                          \
		sl = (mlib_##img_type *) psl[0];                          \
		MEDIAN7(a1, sl[off0], sl[off1], sl[off2], sl[off3],       \
		    sl[off4], sl[off5], sl[off6], img_type);              \
		sl = (mlib_##img_type *) psl[1];                          \
		MEDIAN7(a2, sl[off0], sl[off1], sl[off2], sl[off3],       \
		    sl[off4], sl[off5], sl[off6], img_type);              \
		sl = (mlib_##img_type *) psl[2];                          \
		MEDIAN7(a3, sl[off0], sl[off1], sl[off2], sl[off3],       \
		    sl[off4], sl[off5], sl[off6], img_type);              \
		sl = (mlib_##img_type *) psl[3];                          \
		MEDIAN7(a4, sl[off0], sl[off1], sl[off2], sl[off3],       \
		    sl[off4], sl[off5], sl[off6], img_type);              \
		sl = (mlib_##img_type *) psl[4];                          \
		MEDIAN7(a5, sl[off0], sl[off1], sl[off2], sl[off3],       \
		    sl[off4], sl[off5], sl[off6], img_type);              \
		sl = (mlib_##img_type *) psl[5];                          \
		MEDIAN7(a6, sl[off0], sl[off1], sl[off2], sl[off3],       \
		    sl[off4], sl[off5], sl[off6], img_type);              \
		for (i = 6; i < 6 + size; i++) {                          \
		    sl = (mlib_##img_type *) psl[i];                      \
		    MEDIAN7(a7, sl[off0], sl[off1], sl[off2], sl[off3],   \
			sl[off4], sl[off5], sl[off6], img_type);          \
		    MEDIAN7(dp[0], a1, a2, a3, a4, a5, a6, a7,            \
			img_type);                                        \
		    a1 = a2;                                              \
		    a2 = a3;                                              \
		    a3 = a4;                                              \
		    a4 = a5;                                              \
		    a5 = a6;                                              \
		    a6 = a7;                                              \
		    dp += dlb;                                            \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

FUNC_MEDIAN_COL_7x7RSEP(u8)
FUNC_MEDIAN_INITROW_7x7RSEP(u8)

FUNC_MEDIAN_COL_7x7RSEP(s16)
FUNC_MEDIAN_INITROW_7x7RSEP(s16)

FUNC_MEDIAN_COL_7x7RSEP(u16)
FUNC_MEDIAN_INITROW_7x7RSEP(u16)

FUNC_MEDIAN_COL_7x7RSEP(s32)
FUNC_MEDIAN_INITROW_7x7RSEP(s32)

FUNC_MEDIAN_COL_7x7RSEP(f32)
FUNC_MEDIAN_INITROW_7x7RSEP(f32)

FUNC_MEDIAN_COL_7x7RSEP(d64)
FUNC_MEDIAN_INITROW_7x7RSEP(d64)

/* *********************************************************** */

static const mlib_median_fun_type median_row_7x7_funs[] = {
	NULL, NULL, NULL,
	mlib_median_row_7x7rsep_u8,
	NULL, NULL, NULL,
	mlib_median_row_7x7rsep_s16,
	NULL, NULL, NULL,
	mlib_median_row_7x7rsep_u16,
	NULL, NULL, NULL,
	mlib_median_row_7x7rsep_s32,
	NULL, NULL, NULL,
	mlib_median_row_7x7rsep_f32,
	NULL, NULL, NULL,
	mlib_median_row_7x7rsep_d64
};

static const mlib_median_fun_type median_col_7x7_funs[] = {
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

static const mlib_median_fun_type median_init_7x7_funs[] = {
	mlib_median_initrow_7x7rsep_u8,
	mlib_median_initrow_7x7rsep_s16,
	mlib_median_initrow_7x7rsep_u16,
	mlib_median_initrow_7x7rsep_s32,
	mlib_median_initrow_7x7rsep_f32,
	mlib_median_initrow_7x7rsep_d64
};

/* *********************************************************** */

mlib_status
mlib_ImageMedianFilter7x7_alltypes(
    mlib_image *dst,
    const mlib_image *src,
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
	return mlib_ImageMedianFilter7x7_alltypes(dst, src, mmask, cmask, edge,
	    0);
}

/* *********************************************************** */
