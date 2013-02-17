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

#pragma ident   "@(#)mlib_ImageMedianFilter5x5.c	9.2    07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageMedianFilter5x5    - median filtering with 5x5 mask
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMedianFilter5x5(mlib_image       *dst,
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

#pragma weak mlib_ImageMedianFilter5x5 = __mlib_ImageMedianFilter5x5

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMedianFilter5x5) mlib_ImageMedianFilter5x5
    __attribute__((weak, alias("__mlib_ImageMedianFilter5x5")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_median_col_5x5rect_u8(
    mlib_u8 *dl,
    void **psl,
    mlib_filter_state *fs);

/* *********************************************************** */

#define	MEDIAN_5x5rect(itype)                                          \
	{                                                              \
	    sorttype_##itype d10, d11, d12, d13, d14, d15, d16, d17;   \
	    sorttype_##itype d18, d19, d20, d21, d22, d23, d24, d25;   \
	                                                               \
	    d1 = sp0[off0];                                            \
	    d2 = sp0[off1];                                            \
	    d3 = sp0[off2];                                            \
	    d4 = sp0[off3];                                            \
	    d5 = sp0[off4];                                            \
	    d6 = sp1[off0];                                            \
	    d7 = sp1[off1];                                            \
	    d8 = sp1[off2];                                            \
	    d9 = sp1[off3];                                            \
	    d10 = sp1[off4];                                           \
	    d11 = sp2[off0];                                           \
	    d12 = sp2[off1];                                           \
	    d13 = sp2[off2];                                           \
	    d14 = sp2[off3];                                           \
	    d15 = sp2[off4];                                           \
	    d16 = sp3[off0];                                           \
	    d17 = sp3[off1];                                           \
	    d18 = sp3[off2];                                           \
	    d19 = sp3[off3];                                           \
	    d20 = sp3[off4];                                           \
	    d21 = sp4[off0];                                           \
	    d22 = sp4[off1];                                           \
	    d23 = sp4[off2];                                           \
	    d24 = sp4[off3];                                           \
	    d25 = sp4[off4];                                           \
	    SORT25(SORT2_##itype);                                     \
	    dp[0] = d13;                                               \
	}

/* *********************************************************** */

#define	MEDIAN_5x5plus(itype)                                       \
	d1 = sp0[off2];                                             \
	d2 = sp1[off2];                                             \
	d3 = sp2[off0];                                             \
	d4 = sp2[off1];                                             \
	d5 = sp2[off2];                                             \
	d6 = sp2[off3];                                             \
	d7 = sp2[off4];                                             \
	d8 = sp3[off2];                                             \
	d9 = sp4[off2];                                             \
	SORT9(d1, d2, d3, d4, d5, d6, d7, d8, d9, SORT2_##itype);   \
	dp[0] = d5

/* *********************************************************** */

#define	MEDIAN_5x5xmsk(itype)                                       \
	d1 = sp0[off0];                                             \
	d2 = sp0[off4];                                             \
	d3 = sp1[off1];                                             \
	d4 = sp1[off3];                                             \
	d5 = sp2[off2];                                             \
	d6 = sp3[off1];                                             \
	d7 = sp3[off3];                                             \
	d8 = sp4[off0];                                             \
	d9 = sp4[off4];                                             \
	SORT9(d1, d2, d3, d4, d5, d6, d7, d8, d9, SORT2_##itype);   \
	dp[0] = d5

/* *********************************************************** */

#define	FUNC_MEDIAN_ROW(MEDMASK, img_type)                          \
	static void                                                 \
	mlib_median_row_##MEDMASK##_##img_type(mlib_u8 *dl,         \
	    void **psl, mlib_filter_state *fs)                      \
	{                                                           \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask;          \
	    mlib_s32 size = fs->size;                               \
	    ptype_##img_type *dp, *sp0, *sp1, *sp2, *sp3, *sp4;      \
	    sorttype_##img_type mask, d1, d2, d3, d4, d5, d6, d7,   \
		d8, d9;                                             \
	    mlib_s32 off0 = -2 * nchan, off1 = -nchan, off2 = 0;    \
	    mlib_s32 off3 = nchan, off4 = 2 * nchan, i, k;          \
	                                                            \
	    for (k = 0; k < nchan; k++) {                           \
		if (!(cmask & (1 << (nchan - 1 - k))))              \
		    continue;                                       \
		dp = (ptype_##img_type *) dl + k;                    \
		sp0 = (ptype_##img_type *) psl[0] + k;               \
		sp1 = (ptype_##img_type *) psl[1] + k;               \
		sp2 = (ptype_##img_type *) psl[2] + k;               \
		sp3 = (ptype_##img_type *) psl[3] + k;               \
		sp4 = (ptype_##img_type *) psl[4] + k;               \
		for (i = 0; i < size; i++) {                        \
		    MEDIAN_##MEDMASK(img_type);                     \
		    sp0 += nchan;                                   \
		    sp1 += nchan;                                   \
		    sp2 += nchan;                                   \
		    sp3 += nchan;                                   \
		    sp4 += nchan;                                   \
		    dp += nchan;                                    \
		}                                                   \
	    }                                                       \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_COL(MEDMASK, img_type)                          \
	static void                                                 \
	mlib_median_col_##MEDMASK##_##img_type(mlib_u8 *dl,         \
	    void **psl, mlib_filter_state *fs)                      \
	{                                                           \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask, size =   \
		fs->size;                                           \
	    mlib_s32 *x_off = fs->x_off, dlb = fs->dlp;             \
	    ptype_##img_type *dp, *sp0, *sp1, *sp2, *sp3, *sp4;      \
	    sorttype_##img_type mask, d1, d2, d3, d4, d5, d6, d7,   \
		d8, d9;                                             \
	    mlib_s32 off0, off1, off2, off3, off4, i, k;            \
	                                                            \
	    off0 = x_off[0];                                        \
	    off1 = x_off[1];                                        \
	    off2 = x_off[2];                                        \
	    off3 = x_off[3];                                        \
	    off4 = x_off[4];                                        \
	    for (k = 0; k < nchan;                                  \
		k++, off0++, off1++, off2++, off3++, off4++) {      \
		if (!(cmask & (1 << (nchan - 1 - k))))              \
		    continue;                                       \
		dp = (ptype_##img_type *) dl + k;                    \
		for (i = 0; i < size; i++) {                        \
		    sp0 = (ptype_##img_type *) psl[i];               \
		    sp1 = (ptype_##img_type *) psl[i + 1];           \
		    sp2 = (ptype_##img_type *) psl[i + 2];           \
		    sp3 = (ptype_##img_type *) psl[i + 3];           \
		    sp4 = (ptype_##img_type *) psl[i + 4];           \
		    MEDIAN_##MEDMASK(img_type);                     \
		    dp += dlb;                                      \
		}                                                   \
	    }                                                       \
	}

/* *********************************************************** */

#ifdef _MSC_VER
#pragma optimize("", off)
#endif /* _MSC_VER */

void
mlib_median_row_5x5rect_u8(
    mlib_u8 *dl,
    void **psl,
    mlib_filter_state *fs)
{
	mlib_s32 nchan = fs->nchan, cmask = fs->cmask, size = fs->size;
	mlib_u8 *pd, *ps1, *ps2, *ps3, *ps4, *ps5;
	mlib_s32 off0 = -2 * nchan, off1 = -nchan, off2 = 0;
	mlib_s32 off3 = nchan, off4 = 2 * nchan;
	mlib_d64 hist_buff[256 / sizeof (mlib_d64)];
	mlib_u8 *phist = (mlib_u8 *)hist_buff;
	mlib_s32 i, j, k, data, count, index, rank = 13;

	for (k = 0; k < nchan; k++) {
		if (!(cmask & (1 << (nchan - 1 - k))))
			continue;

		for (i = 0; i < (mlib_s32)(256 / sizeof (mlib_d64)); i++)
			hist_buff[i] = 0;

		index = count = 0;

		pd = dl + k;
		ps1 = (mlib_u8 *)psl[0] + k;
		ps2 = (mlib_u8 *)psl[1] + k;
		ps3 = (mlib_u8 *)psl[2] + k;
		ps4 = (mlib_u8 *)psl[3] + k;
		ps5 = (mlib_u8 *)psl[4] + k;

		phist[ps1[off0]]++;
		phist[ps1[off1]]++;
		phist[ps1[off2]]++;
		phist[ps1[off3]]++;

		phist[ps2[off0]]++;
		phist[ps2[off1]]++;
		phist[ps2[off2]]++;
		phist[ps2[off3]]++;

		phist[ps3[off0]]++;
		phist[ps3[off1]]++;
		phist[ps3[off2]]++;
		phist[ps3[off3]]++;

		phist[ps4[off0]]++;
		phist[ps4[off1]]++;
		phist[ps4[off2]]++;
		phist[ps4[off3]]++;

		phist[ps5[off0]]++;
		phist[ps5[off1]]++;
		phist[ps5[off2]]++;
		phist[ps5[off3]]++;

		for (j = 0; j < size; j++) {
			data = ps1[off4];
			phist[data]++;
			count -= (data - index) >> 31;

			data = ps2[off4];
			phist[data]++;
			count -= (data - index) >> 31;

			data = ps3[off4];
			phist[data]++;
			count -= (data - index) >> 31;

			data = ps4[off4];
			phist[data]++;
			count -= (data - index) >> 31;

			data = ps5[off4];
			phist[data]++;
			count -= (data - index) >> 31;

			if (count < rank) {
				for (; count < rank; index++)
					count += phist[index];
			} else {
				for (; count >= rank; index--)
					count -= phist[index - 1];

				count += phist[index++];
			}

			*pd = index - 1;

			data = ps1[off0];
			phist[data]--;
			count += (data - index) >> 31;

			data = ps2[off0];
			phist[data]--;
			count += (data - index) >> 31;

			data = ps3[off0];
			phist[data]--;
			count += (data - index) >> 31;

			data = ps4[off0];
			phist[data]--;
			count += (data - index) >> 31;

			data = ps5[off0];
			phist[data]--;
			count += (data - index) >> 31;

			ps1 += nchan;
			ps2 += nchan;
			ps3 += nchan;
			ps4 += nchan;
			ps5 += nchan;
			pd += nchan;
		}
	}
}

#ifdef _MSC_VER
#pragma optimize("", on)
#endif /* _MSC_VER */

/* *********************************************************** */

void
mlib_median_col_5x5rect_u8(
    mlib_u8 *dl,
    void **psl,
    mlib_filter_state *fs)
{
	mlib_s32 nchan = fs->nchan, cmask = fs->cmask, size = fs->size;
	mlib_s32 *x_off = fs->x_off, dlb = fs->dlp;
	mlib_u8 *pd, *ps1, *ps2, *ps3, *ps4, *ps5;
	mlib_s32 off0, off1, off2, off3, off4;
	mlib_d64 hist_buff[256 / sizeof (mlib_d64)];
	mlib_u8 *phist = (mlib_u8 *)hist_buff;
	mlib_s32 i, j, k, data, count, index, rank = 13;

	off0 = x_off[0];
	off1 = x_off[1];
	off2 = x_off[2];
	off3 = x_off[3];
	off4 = x_off[4];

	for (k = 0; k < nchan; k++, off0++, off1++, off2++, off3++, off4++) {
		if (!(cmask & (1 << (nchan - 1 - k))))
			continue;

		pd = dl + k;
		ps1 = (mlib_u8 *)psl[0];
		ps2 = (mlib_u8 *)psl[1];
		ps3 = (mlib_u8 *)psl[2];
		ps4 = (mlib_u8 *)psl[3];
		ps5 = (mlib_u8 *)psl[4];

		for (i = 0; i < (mlib_s32)(256 / sizeof (mlib_d64)); i++)
			hist_buff[i] = 0;

		index = count = 0;

		phist[ps1[off0]]++;
		phist[ps1[off1]]++;
		phist[ps1[off2]]++;
		phist[ps1[off3]]++;
		phist[ps1[off4]]++;

		phist[ps2[off0]]++;
		phist[ps2[off1]]++;
		phist[ps2[off2]]++;
		phist[ps2[off3]]++;
		phist[ps2[off4]]++;

		phist[ps3[off0]]++;
		phist[ps3[off1]]++;
		phist[ps3[off2]]++;
		phist[ps3[off3]]++;
		phist[ps3[off4]]++;

		phist[ps4[off0]]++;
		phist[ps4[off1]]++;
		phist[ps4[off2]]++;
		phist[ps4[off3]]++;
		phist[ps4[off4]]++;

		for (j = 0; j < size; j++) {
			ps5 = (mlib_u8 *)psl[j + 4];

			data = ps5[off0];
			phist[data]++;
			count -= (data - index) >> 31;

			data = ps5[off1];
			phist[data]++;
			count -= (data - index) >> 31;

			data = ps5[off2];
			phist[data]++;
			count -= (data - index) >> 31;

			data = ps5[off3];
			phist[data]++;
			count -= (data - index) >> 31;

			data = ps5[off4];
			phist[data]++;
			count -= (data - index) >> 31;

			if (count < rank) {
				for (; count < rank; index++) {
					count += phist[index];
				}

			} else {
				for (; count >= rank; index--)
					count -= phist[index - 1];

				count += phist[index++];
			}

			*pd = index - 1;

			ps1 = (mlib_u8 *)psl[j];

			data = ps1[off0];
			phist[data]--;
			count += (data - index) >> 31;

			data = ps1[off1];
			phist[data]--;
			count += (data - index) >> 31;

			data = ps1[off2];
			phist[data]--;
			count += (data - index) >> 31;

			data = ps1[off3];
			phist[data]--;
			count += (data - index) >> 31;

			data = ps1[off4];
			phist[data]--;
			count += (data - index) >> 31;

			pd += dlb;
		}
	}
}

/* *********************************************************** */

FUNC_MEDIAN_ROW(5x5plus, u8)
FUNC_MEDIAN_COL(5x5plus, u8)

FUNC_MEDIAN_ROW(5x5xmsk, u8)
FUNC_MEDIAN_COL(5x5xmsk, u8)

/* *********************************************************** */

FUNC_MEDIAN_ROW(5x5rect, s16)
FUNC_MEDIAN_COL(5x5rect, s16)

FUNC_MEDIAN_ROW(5x5plus, s16)
FUNC_MEDIAN_COL(5x5plus, s16)

FUNC_MEDIAN_ROW(5x5xmsk, s16)
FUNC_MEDIAN_COL(5x5xmsk, s16)

/* *********************************************************** */

FUNC_MEDIAN_ROW(5x5rect, u16)
FUNC_MEDIAN_COL(5x5rect, u16)

FUNC_MEDIAN_ROW(5x5plus, u16)
FUNC_MEDIAN_COL(5x5plus, u16)

FUNC_MEDIAN_ROW(5x5xmsk, u16)
FUNC_MEDIAN_COL(5x5xmsk, u16)

/* *********************************************************** */

FUNC_MEDIAN_ROW(5x5rect, s32)
FUNC_MEDIAN_COL(5x5rect, s32)

FUNC_MEDIAN_ROW(5x5plus, s32)
FUNC_MEDIAN_COL(5x5plus, s32)

FUNC_MEDIAN_ROW(5x5xmsk, s32)
FUNC_MEDIAN_COL(5x5xmsk, s32)

/* *********************************************************** */

FUNC_MEDIAN_ROW(5x5rect, f32)
FUNC_MEDIAN_COL(5x5rect, f32)

FUNC_MEDIAN_ROW(5x5plus, f32)
FUNC_MEDIAN_COL(5x5plus, f32)

FUNC_MEDIAN_ROW(5x5xmsk, f32)
FUNC_MEDIAN_COL(5x5xmsk, f32)

/* *********************************************************** */

FUNC_MEDIAN_ROW(5x5rect, d64)
FUNC_MEDIAN_COL(5x5rect, d64)

FUNC_MEDIAN_ROW(5x5plus, d64)
FUNC_MEDIAN_COL(5x5plus, d64)

FUNC_MEDIAN_ROW(5x5xmsk, d64)
FUNC_MEDIAN_COL(5x5xmsk, d64)

/* *********************************************************** */

const mlib_median_fun_type mlib_median_row_5x5_funs[] = {
	mlib_median_row_5x5rect_u8,
	mlib_median_row_5x5plus_u8,
	mlib_median_row_5x5xmsk_u8,
	mlib_median_row_5x5rsep_u8,
	mlib_median_row_5x5rect_s16,
	mlib_median_row_5x5plus_s16,
	mlib_median_row_5x5xmsk_s16,
	mlib_median_row_5x5rsep_s16,
	mlib_median_row_5x5rect_u16,
	mlib_median_row_5x5plus_u16,
	mlib_median_row_5x5xmsk_u16,
	mlib_median_row_5x5rsep_u16,
	mlib_median_row_5x5rect_s32,
	mlib_median_row_5x5plus_s32,
	mlib_median_row_5x5xmsk_s32,
	mlib_median_row_5x5rsep_s32,
	mlib_median_row_5x5rect_f32,
	mlib_median_row_5x5plus_f32,
	mlib_median_row_5x5xmsk_f32,
	mlib_median_row_5x5rsep_f32,
	mlib_median_row_5x5rect_d64,
	mlib_median_row_5x5plus_d64,
	mlib_median_row_5x5xmsk_d64,
	mlib_median_row_5x5rsep_d64
};

const mlib_median_fun_type mlib_median_col_5x5_funs[] = {
	mlib_median_col_5x5rect_u8,
	mlib_median_col_5x5plus_u8,
	mlib_median_col_5x5xmsk_u8,
	mlib_median_col_5x5rsep_u8,
	mlib_median_col_5x5rect_s16,
	mlib_median_col_5x5plus_s16,
	mlib_median_col_5x5xmsk_s16,
	mlib_median_col_5x5rsep_s16,
	mlib_median_col_5x5rect_u16,
	mlib_median_col_5x5plus_u16,
	mlib_median_col_5x5xmsk_u16,
	mlib_median_col_5x5rsep_u16,
	mlib_median_col_5x5rect_s32,
	mlib_median_col_5x5plus_s32,
	mlib_median_col_5x5xmsk_s32,
	mlib_median_col_5x5rsep_s32,
	mlib_median_col_5x5rect_f32,
	mlib_median_col_5x5plus_f32,
	mlib_median_col_5x5xmsk_f32,
	mlib_median_col_5x5rsep_f32,
	mlib_median_col_5x5rect_d64,
	mlib_median_col_5x5plus_d64,
	mlib_median_col_5x5xmsk_d64,
	mlib_median_col_5x5rsep_d64
};

const mlib_median_fun_type mlib_median_init_5x5_funs[] = {
	mlib_median_initrow_5x5rsep_u8,
	mlib_median_initrow_5x5rsep_s16,
	mlib_median_initrow_5x5rsep_u16,
	mlib_median_initrow_5x5rsep_s32,
	mlib_median_initrow_5x5rsep_f32,
	mlib_median_initrow_5x5rsep_d64
};

/* *********************************************************** */

mlib_status
__mlib_ImageMedianFilter5x5(
    mlib_image *dst,
    const mlib_image *src,
    mlib_median_mask mmask,
    mlib_s32 cmask,
    mlib_edge edge)
{
	return mlib_ImageMedianFilter5x5_alltypes(dst, src, mmask, cmask, edge,
	    0);
}

/* *********************************************************** */
