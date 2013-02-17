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

#pragma ident   "@(#)mlib_s_ImageMedianFilter3x3.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageMedianFilter3x3    - median filtering with 3x3 mask
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMedianFilter3x3(mlib_image       *dst,
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
 *      The src and the dst can be images of MLIB_BYTE, MLIB_SHORT or
 *      MLIB_INT type.
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

#pragma weak mlib_ImageMedianFilter3x3 = __mlib_ImageMedianFilter3x3

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageMedianFilter3x3) mlib_ImageMedianFilter3x3
    __attribute__((weak, alias("__mlib_ImageMedianFilter3x3")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MEDIAN_3x3rect(sortF)                                   \
	SORT9(d1, d2, d3, d4, d5, d6, d7, d8, d9, sortF)

/* *********************************************************** */

#define	MEDIAN_3x3plus(sortF)	SORT5(d2, d4, d5, d6, d8, sortF)

/* *********************************************************** */

#define	MEDIAN_3x3xmsk(sortF)	SORT5(d1, d3, d5, d7, d9, sortF)

/* *********************************************************** */

#define	SSE2_MEDIAN_3x3rect(itype)                               \
	SSE2_SORT9(d1, d2, d3, d4, d5, d6, d7, d8, d9, itype)

/* *********************************************************** */

#define	SSE2_MEDIAN_3x3plus(itype)                               \
	SSE2_SORT5(d2, d4, d5, d6, d8, itype)

/* *********************************************************** */

#define	SSE2_MEDIAN_3x3xmsk(itype)                               \
	SSE2_SORT5(d1, d3, d5, d7, d9, itype)

/* *********************************************************** */

#define	FUNC_MEDIAN_ROW(MEDMASK, img_type)                      \
	void                                                    \
	mlib_median_row_##MEDMASK##_##img_type(mlib_u8 *dl,     \
	    void **psl, mlib_filter_state * fs)                 \
	{                                                       \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask,      \
			size = fs->size;                                \
	    mlib_##img_type *dp, *sp0, *sp1, *sp2;              \
	    sorttype_##img_type mask, d1, d2, d3, d4, d5,       \
			d6, d7, d8, d9;                                 \
	    mlib_s32 i, k;                                      \
	                                                        \
	    for (k = 0; k < nchan; k++) {                       \
			if (!(cmask & (1 << (nchan - 1 - k))))          \
				continue;                                   \
			dp = (mlib_##img_type *) dl + k;                \
			sp0 = (mlib_##img_type *) psl[0] + k;           \
			sp1 = (mlib_##img_type *) psl[1] + k;           \
			sp2 = (mlib_##img_type *) psl[2] + k;           \
			for (i = 0; i < size; i++) {                    \
				d1 = sp0[-nchan];                           \
				d2 = sp0[0];                                \
				d3 = sp0[nchan];                            \
				d4 = sp1[-nchan];                           \
				d5 = sp1[0];                                \
				d6 = sp1[nchan];                            \
				d7 = sp2[-nchan];                           \
				d8 = sp2[0];                                \
				d9 = sp2[nchan];                            \
				MEDIAN_##MEDMASK(SORT2_##img_type);         \
				dp[0] = d5;                                 \
				sp0 += nchan;                               \
				sp1 += nchan;                               \
				sp2 += nchan;                               \
				dp += nchan;                                \
			}                                               \
	    }                                                   \
	}

/* *********************************************************** */

#define	_SSE2_MEDIAN_ROW(MEDMASK, img_type)                            \
	void                                                      \
	mlib_s_median_row_##MEDMASK##_##img_type(mlib_u8          \
	    *dl, void **psl, mlib_filter_state * fs)              \
	{                                                         \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask, size = \
			fs->size;                                         \
	    mlib_u8 *dp, *sp0, *sp1, *sp2;                        \
	    sse2_##img_type mask80, mask, d1, d2, d3, d4, d5,     \
			d6, d7, d8, d9;                                   \
	    mlib_s32 bchan = nchan * sizeof (mlib_##img_type);    \
	    mlib_s32 bsize = bchan * size;                        \
	    mlib_s32 amask = (1 << nchan) - 1;                    \
	    mlib_s32 i, k;                                        \
	                                                          \
	    if ((cmask & amask) != amask) {                       \
			mlib_median_row_##MEDMASK##_##img_type(dl,        \
				psl, fs);                                     \
			return;                                           \
	    }                                                     \
	    dp = (mlib_u8 *)dl;                                   \
	    sp0 = (mlib_u8 *)psl[0];                              \
	    sp1 = (mlib_u8 *)psl[1];                              \
	    sp2 = (mlib_u8 *)psl[2];                              \
	    amask =                                               \
			(sizeof (mlib_##img_type) ==                      \
			1) ? 0x80808080 : 0x80008000;                     \
	    mask80 = SSE2_SET_##img_type(amask);                  \
	    for (i = 0; i <= bsize - 16; i += 16) {               \
			d1 = SSE2_LOAD_##img_type((sp0 - bchan));         \
			d2 = SSE2_LOAD_##img_type((sp0));                 \
			d3 = SSE2_LOAD_##img_type((sp0 + bchan));         \
			d4 = SSE2_LOAD_##img_type((sp1 - bchan));         \
			d5 = SSE2_LOAD_##img_type((sp1));                 \
			d6 = SSE2_LOAD_##img_type((sp1 + bchan));         \
			d7 = SSE2_LOAD_##img_type((sp2 - bchan));         \
			d8 = SSE2_LOAD_##img_type((sp2));                 \
			d9 = SSE2_LOAD_##img_type((sp2 + bchan));         \
			SSE2_MEDIAN_##MEDMASK(img_type);                  \
			SSE2_STORE_##img_type(dp, d5);                    \
			sp0 += 16;                                        \
			sp1 += 16;                                        \
			sp2 += 16;                                        \
			dp += 16;                                         \
	    }                                                     \
	    for (; i < bsize; i += sizeof (mlib_##img_type)) {    \
			sorttype_##img_type mask, d1, d2, d3, d4, d5, d6, \
				d7, d8, d9;                                   \
			d1 = *(mlib_##img_type *) (sp0 - bchan);          \
			d2 = *(mlib_##img_type *) (sp0);                  \
			d3 = *(mlib_##img_type *) (sp0 + bchan);          \
			d4 = *(mlib_##img_type *) (sp1 - bchan);          \
			d5 = *(mlib_##img_type *) (sp1);                  \
			d6 = *(mlib_##img_type *) (sp1 + bchan);          \
			d7 = *(mlib_##img_type *) (sp2 - bchan);          \
			d8 = *(mlib_##img_type *) (sp2);                  \
			d9 = *(mlib_##img_type *) (sp2 + bchan);          \
			MEDIAN_##MEDMASK(SORT2_##img_type);               \
			*(mlib_##img_type *) dp = d5;                     \
			sp0 += sizeof (mlib_##img_type);                  \
			sp1 += sizeof (mlib_##img_type);                  \
			sp2 += sizeof (mlib_##img_type);                  \
			dp += sizeof (mlib_##img_type);                   \
	    }                                                     \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_COL(MEDMASK, img_type)                        \
	void                                                      \
	mlib_median_col_##MEDMASK##_##img_type(mlib_u8 *dl,       \
	    void **psl, mlib_filter_state * fs)                   \
	{                                                         \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask, size = \
			fs->size;                                         \
	    mlib_s32 *x_off = fs->x_off, dlb = fs->dlp;           \
	    mlib_##img_type *dp, *sp0, *sp1, *sp2;                \
	    sorttype_##img_type mask, d1, d2, d3, d4, d5, d6, d7, \
			d8, d9;                                           \
	    mlib_s32 x_off0, x_off1, x_off2, i, k;                \
	                                                          \
	    x_off0 = x_off[0];                                    \
	    x_off1 = x_off[1];                                    \
	    x_off2 = x_off[2];                                    \
	    for (k = 0; k < nchan;                                \
			k++, x_off0++, x_off1++, x_off2++) {              \
			if (!(cmask & (1 << (nchan - 1 - k))))            \
				continue;                                     \
			dp = (mlib_##img_type *) dl + k;                  \
			for (i = 0; i < size; i++) {                      \
				sp0 = (mlib_##img_type *) psl[i];             \
				sp1 = (mlib_##img_type *) psl[i + 1];         \
				sp2 = (mlib_##img_type *) psl[i + 2];         \
				d1 = sp0[x_off0];                             \
				d2 = sp0[x_off1];                             \
				d3 = sp0[x_off2];                             \
				d4 = sp1[x_off0];                             \
				d5 = sp1[x_off1];                             \
				d6 = sp1[x_off2];                             \
				d7 = sp2[x_off0];                             \
				d8 = sp2[x_off1];                             \
				d9 = sp2[x_off2];                             \
				MEDIAN_##MEDMASK(SORT2_##img_type);           \
				dp[0] = d5;                                   \
				dp += dlb;                                    \
			}                                                 \
	    }                                                     \
	}

/* *********************************************************** */

#define	PROT_MEDIAN_INITROW_3x3RSEP(img_type)                   \
	void                                                    \
	mlib_median_initrow_3x3rsep_##img_type(mlib_u8 *dl,     \
	    void **psl, mlib_filter_state * fs);

/* *********************************************************** */

#define	_SSE2_MEDIAN_INITROW_3x3RSEP(img_type)                   \
	void                                                    \
	mlib_s_median_initrow_3x3rsep_##img_type(mlib_u8 *dl,   \
	    void **psl, mlib_filter_state * fs);                \

/* *********************************************************** */

#define	PROT_MEDIAN_ROW_3x3RSEP(img_type)                             \
	void                                                          \
	mlib_median_row_3x3rsep_##img_type(mlib_u8 *dl, void **psl,   \
	    mlib_filter_state * fs);

/* *********************************************************** */

#define	_SSE2_MEDIAN_ROW_3x3RSEP(img_type)                       \
	void                                                    \
	mlib_s_median_row_3x3rsep_##img_type(mlib_u8 *dl,       \
	    void **psl, mlib_filter_state * fs);

/* *********************************************************** */

#define	PROT_MEDIAN_COL_3x3RSEP(img_type)                             \
	void                                                          \
	mlib_median_col_3x3rsep_##img_type(mlib_u8 *dl, void **psl,   \
	    mlib_filter_state * fs);

/* *********************************************************** */

FUNC_MEDIAN_ROW(3x3rect, u8)
_SSE2_MEDIAN_ROW(3x3rect, u8)
FUNC_MEDIAN_COL(3x3rect, u8)

FUNC_MEDIAN_ROW(3x3plus, u8)
_SSE2_MEDIAN_ROW(3x3plus, u8)
FUNC_MEDIAN_COL(3x3plus, u8)

FUNC_MEDIAN_ROW(3x3xmsk, u8)
_SSE2_MEDIAN_ROW(3x3xmsk, u8)
FUNC_MEDIAN_COL(3x3xmsk, u8)

PROT_MEDIAN_ROW_3x3RSEP(u8)
_SSE2_MEDIAN_ROW_3x3RSEP(u8)
PROT_MEDIAN_COL_3x3RSEP(u8)
PROT_MEDIAN_INITROW_3x3RSEP(u8)
_SSE2_MEDIAN_INITROW_3x3RSEP(u8)

/* *********************************************************** */

FUNC_MEDIAN_ROW(3x3rect, s16)
_SSE2_MEDIAN_ROW(3x3rect, s16)
FUNC_MEDIAN_COL(3x3rect, s16)

FUNC_MEDIAN_ROW(3x3plus, s16)
_SSE2_MEDIAN_ROW(3x3plus, s16)
FUNC_MEDIAN_COL(3x3plus, s16)

FUNC_MEDIAN_ROW(3x3xmsk, s16)
_SSE2_MEDIAN_ROW(3x3xmsk, s16)
FUNC_MEDIAN_COL(3x3xmsk, s16)

PROT_MEDIAN_ROW_3x3RSEP(s16)
_SSE2_MEDIAN_ROW_3x3RSEP(s16)
PROT_MEDIAN_COL_3x3RSEP(s16)
PROT_MEDIAN_INITROW_3x3RSEP(s16)
_SSE2_MEDIAN_INITROW_3x3RSEP(s16)

/* *********************************************************** */

FUNC_MEDIAN_ROW(3x3rect, u16)
_SSE2_MEDIAN_ROW(3x3rect, u16)
FUNC_MEDIAN_COL(3x3rect, u16)

FUNC_MEDIAN_ROW(3x3plus, u16)
_SSE2_MEDIAN_ROW(3x3plus, u16)
FUNC_MEDIAN_COL(3x3plus, u16)

FUNC_MEDIAN_ROW(3x3xmsk, u16)
_SSE2_MEDIAN_ROW(3x3xmsk, u16)
FUNC_MEDIAN_COL(3x3xmsk, u16)

PROT_MEDIAN_ROW_3x3RSEP(u16)
_SSE2_MEDIAN_ROW_3x3RSEP(u16)
PROT_MEDIAN_COL_3x3RSEP(u16)
PROT_MEDIAN_INITROW_3x3RSEP(u16)
_SSE2_MEDIAN_INITROW_3x3RSEP(u16)

/* *********************************************************** */

FUNC_MEDIAN_ROW(3x3rect, s32)
_SSE2_MEDIAN_ROW(3x3rect, s32)
FUNC_MEDIAN_COL(3x3rect, s32)

FUNC_MEDIAN_ROW(3x3plus, s32)
_SSE2_MEDIAN_ROW(3x3plus, s32)
FUNC_MEDIAN_COL(3x3plus, s32)

FUNC_MEDIAN_ROW(3x3xmsk, s32)
_SSE2_MEDIAN_ROW(3x3xmsk, s32)
FUNC_MEDIAN_COL(3x3xmsk, s32)

PROT_MEDIAN_ROW_3x3RSEP(s32)
_SSE2_MEDIAN_ROW_3x3RSEP(s32)
PROT_MEDIAN_COL_3x3RSEP(s32)
PROT_MEDIAN_INITROW_3x3RSEP(s32)
_SSE2_MEDIAN_INITROW_3x3RSEP(s32)

/* *********************************************************** */

FUNC_MEDIAN_ROW(3x3rect, f32)
_SSE2_MEDIAN_ROW(3x3rect, f32)
FUNC_MEDIAN_COL(3x3rect, f32)

FUNC_MEDIAN_ROW(3x3plus, f32)
_SSE2_MEDIAN_ROW(3x3plus, f32)
FUNC_MEDIAN_COL(3x3plus, f32)

FUNC_MEDIAN_ROW(3x3xmsk, f32)
_SSE2_MEDIAN_ROW(3x3xmsk, f32)
FUNC_MEDIAN_COL(3x3xmsk, f32)

PROT_MEDIAN_ROW_3x3RSEP(f32)
_SSE2_MEDIAN_ROW_3x3RSEP(f32)
PROT_MEDIAN_COL_3x3RSEP(f32)
PROT_MEDIAN_INITROW_3x3RSEP(f32)
_SSE2_MEDIAN_INITROW_3x3RSEP(f32)

/* *********************************************************** */

FUNC_MEDIAN_ROW(3x3rect, d64)
_SSE2_MEDIAN_ROW(3x3rect, d64)
FUNC_MEDIAN_COL(3x3rect, d64)

FUNC_MEDIAN_ROW(3x3plus, d64)
_SSE2_MEDIAN_ROW(3x3plus, d64)
FUNC_MEDIAN_COL(3x3plus, d64)

FUNC_MEDIAN_ROW(3x3xmsk, d64)
_SSE2_MEDIAN_ROW(3x3xmsk, d64)
FUNC_MEDIAN_COL(3x3xmsk, d64)

PROT_MEDIAN_ROW_3x3RSEP(d64)
_SSE2_MEDIAN_ROW_3x3RSEP(d64)
PROT_MEDIAN_COL_3x3RSEP(d64)
PROT_MEDIAN_INITROW_3x3RSEP(d64)
_SSE2_MEDIAN_INITROW_3x3RSEP(d64)

/* *********************************************************** */

static mlib_median_fun_type median_row_3x3_funs[] = {
	mlib_s_median_row_3x3rect_u8,
	mlib_s_median_row_3x3plus_u8,
	mlib_s_median_row_3x3xmsk_u8,
	mlib_s_median_row_3x3rsep_u8,
	mlib_s_median_row_3x3rect_s16,
	mlib_s_median_row_3x3plus_s16,
	mlib_s_median_row_3x3xmsk_s16,
	mlib_s_median_row_3x3rsep_s16,
	mlib_s_median_row_3x3rect_u16,
	mlib_s_median_row_3x3plus_u16,
	mlib_s_median_row_3x3xmsk_u16,
	mlib_s_median_row_3x3rsep_u16,
	mlib_s_median_row_3x3rect_s32,
	mlib_s_median_row_3x3plus_s32,
	mlib_s_median_row_3x3xmsk_s32,
	mlib_s_median_row_3x3rsep_s32,
	mlib_s_median_row_3x3rect_f32,
	mlib_s_median_row_3x3plus_f32,
	mlib_s_median_row_3x3xmsk_f32,
	mlib_s_median_row_3x3rsep_f32,
	mlib_s_median_row_3x3rect_d64,
	mlib_s_median_row_3x3plus_d64,
	mlib_s_median_row_3x3xmsk_d64,
	mlib_s_median_row_3x3rsep_d64
};

static mlib_median_fun_type median_col_3x3_funs[] = {
	mlib_median_col_3x3rect_u8,
	mlib_median_col_3x3plus_u8,
	mlib_median_col_3x3xmsk_u8,
	mlib_median_col_3x3rsep_u8,
	mlib_median_col_3x3rect_s16,
	mlib_median_col_3x3plus_s16,
	mlib_median_col_3x3xmsk_s16,
	mlib_median_col_3x3rsep_s16,
	mlib_median_col_3x3rect_u16,
	mlib_median_col_3x3plus_u16,
	mlib_median_col_3x3xmsk_u16,
	mlib_median_col_3x3rsep_u16,
	mlib_median_col_3x3rect_s32,
	mlib_median_col_3x3plus_s32,
	mlib_median_col_3x3xmsk_s32,
	mlib_median_col_3x3rsep_s32,
	mlib_median_col_3x3rect_f32,
	mlib_median_col_3x3plus_f32,
	mlib_median_col_3x3xmsk_f32,
	mlib_median_col_3x3rsep_f32,
	mlib_median_col_3x3rect_d64,
	mlib_median_col_3x3plus_d64,
	mlib_median_col_3x3xmsk_d64,
	mlib_median_col_3x3rsep_d64
};

static mlib_median_fun_type median_init_3x3_funs[] = {
	mlib_s_median_initrow_3x3rsep_u8,
	mlib_s_median_initrow_3x3rsep_s16,
	mlib_s_median_initrow_3x3rsep_u16,
	mlib_s_median_initrow_3x3rsep_s32,
	mlib_s_median_initrow_3x3rsep_f32,
	mlib_s_median_initrow_3x3rsep_d64
};

/* *********************************************************** */

mlib_status
mlib_ImageMedianFilter3x3_alltypes(
    mlib_image *dst,
    mlib_image *src,
    mlib_median_mask mmask,
    mlib_s32 cmask,
    mlib_edge edge,
    mlib_s32 fp_version)
{
	mlib_filter_state fs;

	fs.m = 3;
	fs.n = 3;
	fs.mmask = mmask;
	fs.cmask = cmask;
	fs.edge = edge;
	fs.fp_version = fp_version;
	fs.mn_version = 0;
	fs.row_funs = median_row_3x3_funs;
	fs.col_funs = median_col_3x3_funs;
	fs.init_funs = median_init_3x3_funs;

	return (mlib_ImageMedianFilter_fun(dst, src, &fs));
}

/* *********************************************************** */

mlib_status
__mlib_ImageMedianFilter3x3(
    mlib_image *dst,
    const mlib_image *src,
    mlib_median_mask mmask,
    mlib_s32 cmask,
    mlib_edge edge)
{
	return mlib_ImageMedianFilter3x3_alltypes(dst, (void *)src, mmask,
	    cmask, edge, 0);
}

/* *********************************************************** */
