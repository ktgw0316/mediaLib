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

#pragma ident   "@(#)mlib_ImageMedianFilter3x3.c	9.2    07/11/05 SMI"

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

#pragma weak mlib_ImageMedianFilter3x3 = __mlib_ImageMedianFilter3x3

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMedianFilter3x3) mlib_ImageMedianFilter3x3
    __attribute__((weak, alias("__mlib_ImageMedianFilter3x3")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	FUNC_MEDIAN_RECT(img_type)                                    \
	static void                                                   \
	mlib_median_row_3x3rect_##img_type(mlib_u8 *dl, void **psl,   \
	    mlib_filter_state *fs)                                    \
	{                                                             \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask;            \
	    mlib_s32 size = fs->size, i, k;                           \
	    ptype_##img_type *dp, *sp0, *sp1, *sp2;                    \
	    sorttype_##img_type d0, d1, d2, d3, d4, d5, d6, d7, d8;   \
	    sorttype_##img_type e1, e2, e3, e4, e6, e7, e8, mask;     \
	                                                              \
	    for (k = 0; k < nchan; k++) {                             \
		if (!(cmask & (1 << (nchan - 1 - k))))                \
		    continue;                                         \
		dp = (ptype_##img_type *) dl + k;                      \
		sp0 = (ptype_##img_type *) psl[0] + k;                 \
		sp1 = (ptype_##img_type *) psl[1] + k;                 \
		sp2 = (ptype_##img_type *) psl[2] + k;                 \
		for (i = 0; i <= (size - 2); i += 2) {                \
		    d0 = sp0[0];                                      \
		    d1 = sp1[0];                                      \
		    d2 = sp2[0];                                      \
		    d3 = sp0[nchan];                                  \
		    d4 = sp1[nchan];                                  \
		    d5 = sp2[nchan];                                  \
		    d6 = sp0[2 * nchan];                              \
		    d7 = sp1[2 * nchan];                              \
		    d8 = sp2[2 * nchan];                              \
		    e6 = sp0[-nchan];                                 \
		    e7 = sp1[-nchan];                                 \
		    e8 = sp2[-nchan];                                 \
/*                                                                    \
 * common for both pixels                                             \
 */                                                                   \
		    SORT2_##img_type(d0, d3);                         \
		    SORT2_##img_type(d1, d4);                         \
		    SORT2_##img_type(d2, d5);                         \
		    SORT2_##img_type(d0, d1);                         \
		    S_MAX_##img_type(d0, d2);                         \
		    SORT2_##img_type(d3, d5);                         \
		    S_MIN_##img_type(d4, d5);                         \
		    SORT2_##img_type(d1, d2);                         \
		    SORT2_##img_type(d3, d4);                         \
/*                                                                    \
 * first fixel                                                        \
 */                                                                   \
		    e1 = d1;                                          \
		    e2 = d2;                                          \
		    e3 = d3;                                          \
		    e4 = d4;                                          \
		    SORT2_##img_type(e1, e6);                         \
		    S_MAX_##img_type(e1, e3);                         \
		    SORT2_##img_type(e2, e6);                         \
		    S_MIN_##img_type(e4, e6);                         \
		    SORT2_##img_type(e2, e3);                         \
		    SORT2_##img_type(e4, e7);                         \
		    S_MAX_##img_type(e2, e4);                         \
		    S_MIN_##img_type(e3, e7);                         \
		    SORT2_##img_type(e3, e4);                         \
		    S_MAX_##img_type(e3, e8);                         \
		    S_MIN_##img_type(e4, e8);                         \
/*                                                                    \
 * second fixel                                                       \
 */                                                                   \
		    SORT2_##img_type(d1, d6);                         \
		    S_MAX_##img_type(d1, d3);                         \
		    SORT2_##img_type(d2, d6);                         \
		    S_MIN_##img_type(d4, d6);                         \
		    SORT2_##img_type(d2, d3);                         \
		    SORT2_##img_type(d4, d7);                         \
		    S_MAX_##img_type(d2, d4);                         \
		    S_MIN_##img_type(d3, d7);                         \
		    SORT2_##img_type(d3, d4);                         \
		    S_MAX_##img_type(d3, d8);                         \
		    S_MIN_##img_type(d4, d8);                         \
		    dp[0] = e4;                                       \
		    dp[nchan] = d4;                                   \
		    sp0 += 2 * nchan;                                 \
		    sp1 += 2 * nchan;                                 \
		    sp2 += 2 * nchan;                                 \
		    dp += 2 * nchan;                                  \
		}                                                     \
		if (i < size) {                                       \
		    d0 = sp0[0];                                      \
		    d1 = sp1[0];                                      \
		    d2 = sp2[0];                                      \
		    d3 = sp0[nchan];                                  \
		    d4 = sp1[nchan];                                  \
		    d5 = sp2[nchan];                                  \
		    d6 = sp0[-nchan];                                 \
		    d7 = sp1[-nchan];                                 \
		    d8 = sp2[-nchan];                                 \
		    SORT2_##img_type(d0, d3);                         \
		    SORT2_##img_type(d1, d4);                         \
		    SORT2_##img_type(d2, d5);                         \
		    SORT2_##img_type(d0, d1);                         \
		    S_MAX_##img_type(d0, d2);                         \
		    SORT2_##img_type(d3, d5);                         \
		    S_MIN_##img_type(d4, d5);                         \
		    SORT2_##img_type(d1, d2);                         \
		    SORT2_##img_type(d3, d4);                         \
		    SORT2_##img_type(d1, d6);                         \
		    S_MAX_##img_type(d1, d3);                         \
		    SORT2_##img_type(d2, d6);                         \
		    S_MIN_##img_type(d4, d6);                         \
		    SORT2_##img_type(d2, d3);                         \
		    SORT2_##img_type(d4, d7);                         \
		    S_MAX_##img_type(d2, d4);                         \
		    S_MIN_##img_type(d3, d7);                         \
		    SORT2_##img_type(d3, d4);                         \
		    S_MAX_##img_type(d3, d8);                         \
		    S_MIN_##img_type(d4, d8);                         \
		    dp[0] = d4;                                       \
		}                                                     \
	    }                                                         \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_PLUS(img_type)                                    \
	static void                                                   \
	mlib_median_row_3x3plus_##img_type(mlib_u8 *dl, void **psl,   \
	    mlib_filter_state *fs)                                    \
	{                                                             \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask;            \
	    mlib_s32 size = fs->size, i, k;                           \
	    ptype_##img_type *dp, *sp0, *sp1, *sp2;                    \
	    sorttype_##img_type d0, d1, d2, d3, d4;                   \
	    sorttype_##img_type e0, e1, e2, e3, e4, mask;             \
	                                                              \
	    for (k = 0; k < nchan; k++) {                             \
		if (!(cmask & (1 << (nchan - 1 - k))))                \
		    continue;                                         \
		dp = (ptype_##img_type *) dl + k;                      \
		sp0 = (ptype_##img_type *) psl[0] + k;                 \
		sp1 = (ptype_##img_type *) psl[1] + k;                 \
		sp2 = (ptype_##img_type *) psl[2] + k;                 \
		for (i = 0; i <= (size - 2); i += 2) {                \
		    d0 = sp1[0];                                      \
		    d1 = sp1[nchan];                                  \
		    d2 = sp0[0];                                      \
		    d3 = sp2[0];                                      \
		    d4 = sp1[-nchan];                                 \
		    e2 = sp0[nchan];                                  \
		    e3 = sp2[nchan];                                  \
		    e4 = sp1[2 * nchan];                              \
/*                                                                    \
 * common for both pixels                                             \
 */                                                                   \
		    SORT2_##img_type(d0, d1);                         \
/*                                                                    \
 * second fixel                                                       \
 */                                                                   \
		    e0 = d0;                                          \
		    e1 = d1;                                          \
		    SORT2_##img_type(e2, e3);                         \
		    SORT2_##img_type(e2, e4);                         \
		    SORT2_##img_type(e3, e4);                         \
		    S_MAX_##img_type(e0, e2);                         \
		    S_MIN_##img_type(e1, e4);                         \
		    SORT2_##img_type(e1, e2);                         \
		    S_MAX_##img_type(e1, e3);                         \
		    S_MIN_##img_type(e2, e3);                         \
/*                                                                    \
 * first fixel                                                        \
 */                                                                   \
		    SORT2_##img_type(d2, d3);                         \
		    SORT2_##img_type(d2, d4);                         \
		    SORT2_##img_type(d3, d4);                         \
		    S_MAX_##img_type(d0, d2);                         \
		    S_MIN_##img_type(d1, d4);                         \
		    SORT2_##img_type(d1, d2);                         \
		    S_MAX_##img_type(d1, d3);                         \
		    S_MIN_##img_type(d2, d3);                         \
		    dp[0] = d2;                                       \
		    dp[nchan] = e2;                                   \
		    sp0 += 2 * nchan;                                 \
		    sp1 += 2 * nchan;                                 \
		    sp2 += 2 * nchan;                                 \
		    dp += 2 * nchan;                                  \
		}                                                     \
		if (i < size) {                                       \
		    d0 = sp1[0];                                      \
		    d1 = sp1[nchan];                                  \
		    d2 = sp0[0];                                      \
		    d3 = sp2[0];                                      \
		    d4 = sp1[-nchan];                                 \
		    SORT2_##img_type(d0, d1);                         \
		    SORT2_##img_type(d2, d3);                         \
		    SORT2_##img_type(d2, d4);                         \
		    SORT2_##img_type(d3, d4);                         \
		    S_MAX_##img_type(d0, d2);                         \
		    S_MIN_##img_type(d1, d4);                         \
		    SORT2_##img_type(d1, d2);                         \
		    S_MAX_##img_type(d1, d3);                         \
		    S_MIN_##img_type(d2, d3);                         \
		    dp[0] = d2;                                       \
		}                                                     \
	    }                                                         \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_XMSK(img_type)                                    \
	static void                                                   \
	mlib_median_row_3x3xmsk_##img_type(mlib_u8 *dl, void **psl,   \
	    mlib_filter_state *fs)                                    \
	{                                                             \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask;            \
	    mlib_s32 size = fs->size, i, k;                           \
	    ptype_##img_type *dp, *sp0, *sp1, *sp2;                    \
	    sorttype_##img_type d0, d1, d2, d3, d4;                   \
	    sorttype_##img_type e0, e1, e2, e3, e4, mask;             \
	    mlib_s32 size1, kk;                                       \
	                                                              \
	    for (k = 0; k < 2 * nchan; k++) {                         \
		if (k < nchan) {                                      \
		    size1 = (size + 1) / 2;                           \
		    kk = k;                                           \
		} else {                                              \
		    size1 = size / 2;                                 \
		    kk = k - nchan;                                   \
		}                                                     \
		if (!(cmask & (1 << (nchan - 1 - kk))))               \
		    continue;                                         \
		dp = (ptype_##img_type *) dl + k;                      \
		sp0 = (ptype_##img_type *) psl[0] + k;                 \
		sp1 = (ptype_##img_type *) psl[1] + k;                 \
		sp2 = (ptype_##img_type *) psl[2] + k;                 \
		for (i = 0; i <= (size1 - 2); i += 2) {               \
		    d0 = sp0[nchan];                                  \
		    d1 = sp2[nchan];                                  \
		    d2 = sp0[-nchan];                                 \
		    d3 = sp2[-nchan];                                 \
		    d4 = sp1[0];                                      \
		    e2 = sp0[3 * nchan];                              \
		    e3 = sp2[3 * nchan];                              \
		    e4 = sp1[2 * nchan];                              \
/*                                                                    \
 * common for both pixels                                             \
 */                                                                   \
		    SORT2_##img_type(d0, d1);                         \
/*                                                                    \
 * second fixel                                                       \
 */                                                                   \
		    e0 = d0;                                          \
		    e1 = d1;                                          \
		    SORT2_##img_type(e2, e3);                         \
		    SORT2_##img_type(e2, e4);                         \
		    SORT2_##img_type(e3, e4);                         \
		    S_MAX_##img_type(e0, e2);                         \
		    S_MIN_##img_type(e1, e4);                         \
		    SORT2_##img_type(e1, e2);                         \
		    S_MAX_##img_type(e1, e3);                         \
		    S_MIN_##img_type(e2, e3);                         \
/*                                                                    \
 * first fixel                                                        \
 */                                                                   \
		    SORT2_##img_type(d2, d3);                         \
		    SORT2_##img_type(d2, d4);                         \
		    SORT2_##img_type(d3, d4);                         \
		    S_MAX_##img_type(d0, d2);                         \
		    S_MIN_##img_type(d1, d4);                         \
		    SORT2_##img_type(d1, d2);                         \
		    S_MAX_##img_type(d1, d3);                         \
		    S_MIN_##img_type(d2, d3);                         \
		    dp[0] = d2;                                       \
		    dp[2 * nchan] = e2;                               \
		    sp0 += 4 * nchan;                                 \
		    sp1 += 4 * nchan;                                 \
		    sp2 += 4 * nchan;                                 \
		    dp += 4 * nchan;                                  \
		}                                                     \
		if (i < size1) {                                      \
		    d0 = sp0[nchan];                                  \
		    d1 = sp2[nchan];                                  \
		    d2 = sp0[-nchan];                                 \
		    d3 = sp2[-nchan];                                 \
		    d4 = sp1[0];                                      \
		    SORT2_##img_type(d0, d1);                         \
		    SORT2_##img_type(d2, d3);                         \
		    SORT2_##img_type(d2, d4);                         \
		    SORT2_##img_type(d3, d4);                         \
		    S_MAX_##img_type(d0, d2);                         \
		    S_MIN_##img_type(d1, d4);                         \
		    SORT2_##img_type(d1, d2);                         \
		    S_MAX_##img_type(d1, d3);                         \
		    S_MIN_##img_type(d2, d3);                         \
		    dp[0] = d2;                                       \
		}                                                     \
	    }                                                         \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_COL_3x3RECT(img_type)                                 \
	static void                                                       \
	mlib_median_col_3x3rect_##img_type(mlib_u8 *dl, void **psl,       \
	    mlib_filter_state *fs)                                        \
	{                                                                 \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask,                \
		size = fs->size;                                          \
	    mlib_s32 *x_off = fs->x_off, dlb = fs->dlp;                   \
	    ptype_##img_type *dp, *sp0, *sp1, *sp2;                        \
	    sorttype_##img_type mask, d1, d2, d3, d4, d5, d6, d7,         \
		d8, d9;                                                   \
	    mlib_s32 x_off0, x_off1, x_off2, i, k;                        \
	                                                                  \
	    x_off0 = x_off[0];                                            \
	    x_off1 = x_off[1];                                            \
	    x_off2 = x_off[2];                                            \
	    for (k = 0; k < nchan; k++, x_off0++, x_off1++, x_off2++) {   \
		if (!(cmask & (1 << (nchan - 1 - k))))                    \
		    continue;                                             \
		dp = (ptype_##img_type *) dl + k;                          \
		for (i = 0; i < size; i++) {                              \
		    sp0 = (ptype_##img_type *) psl[i];                     \
		    sp1 = (ptype_##img_type *) psl[i + 1];                 \
		    sp2 = (ptype_##img_type *) psl[i + 2];                 \
		    d1 = sp0[x_off0];                                     \
		    d2 = sp0[x_off1];                                     \
		    d3 = sp0[x_off2];                                     \
		    d4 = sp1[x_off0];                                     \
		    d5 = sp1[x_off1];                                     \
		    d6 = sp1[x_off2];                                     \
		    d7 = sp2[x_off0];                                     \
		    d8 = sp2[x_off1];                                     \
		    d9 = sp2[x_off2];                                     \
		    SORT9(d1, d2, d3, d4, d5, d6, d7, d8, d9,             \
			SORT2_##img_type);                                \
		    dp[0] = d5;                                           \
		    dp += dlb;                                            \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_COL_3x3PLUS(img_type)                                 \
	static void                                                       \
	mlib_median_col_3x3plus_##img_type(mlib_u8 *dl, void **psl,       \
	    mlib_filter_state *fs)                                        \
	{                                                                 \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask,                \
		size = fs->size;                                          \
	    mlib_s32 *x_off = fs->x_off, dlb = fs->dlp;                   \
	    ptype_##img_type *dp, *sp0, *sp1, *sp2;                        \
	    sorttype_##img_type mask, d2, d4, d5, d6, d8;                 \
	    mlib_s32 x_off0, x_off1, x_off2, i, k;                        \
	                                                                  \
	    x_off0 = x_off[0];                                            \
	    x_off1 = x_off[1];                                            \
	    x_off2 = x_off[2];                                            \
	    for (k = 0; k < nchan; k++, x_off0++, x_off1++, x_off2++) {   \
		if (!(cmask & (1 << (nchan - 1 - k))))                    \
		    continue;                                             \
		dp = (ptype_##img_type *) dl + k;                          \
		for (i = 0; i < size; i++) {                              \
		    sp0 = (ptype_##img_type *) psl[i];                     \
		    sp1 = (ptype_##img_type *) psl[i + 1];                 \
		    sp2 = (ptype_##img_type *) psl[i + 2];                 \
		    d2 = sp0[x_off1];                                     \
		    d4 = sp1[x_off0];                                     \
		    d5 = sp1[x_off1];                                     \
		    d6 = sp1[x_off2];                                     \
		    d8 = sp2[x_off1];                                     \
		    SORT5(d2, d4, d5, d6, d8, SORT2_##img_type);          \
		    dp[0] = d5;                                           \
		    dp += dlb;                                            \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_COL_3x3XMSK(img_type)                                 \
	static void                                                       \
	mlib_median_col_3x3xmsk_##img_type(mlib_u8 *dl, void **psl,       \
	    mlib_filter_state *fs)                                        \
	{                                                                 \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask,                \
		size = fs->size;                                          \
	    mlib_s32 *x_off = fs->x_off, dlb = fs->dlp;                   \
	    ptype_##img_type *dp, *sp0, *sp1, *sp2;                        \
	    sorttype_##img_type mask, d1, d3, d5, d7, d9;                 \
	    mlib_s32 x_off0, x_off1, x_off2, i, k;                        \
	                                                                  \
	    x_off0 = x_off[0];                                            \
	    x_off1 = x_off[1];                                            \
	    x_off2 = x_off[2];                                            \
	    for (k = 0; k < nchan; k++, x_off0++, x_off1++, x_off2++) {   \
		if (!(cmask & (1 << (nchan - 1 - k))))                    \
		    continue;                                             \
		dp = (ptype_##img_type *) dl + k;                          \
		for (i = 0; i < size; i++) {                              \
		    sp0 = (ptype_##img_type *) psl[i];                     \
		    sp1 = (ptype_##img_type *) psl[i + 1];                 \
		    sp2 = (ptype_##img_type *) psl[i + 2];                 \
		    d1 = sp0[x_off0];                                     \
		    d3 = sp0[x_off2];                                     \
		    d5 = sp1[x_off1];                                     \
		    d7 = sp2[x_off0];                                     \
		    d9 = sp2[x_off2];                                     \
		    SORT5(d1, d3, d5, d7, d9, SORT2_##img_type);          \
		    dp[0] = d5;                                           \
		    dp += dlb;                                            \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

#define	static

FUNC_MEDIAN_RECT(u8)
#undef static

FUNC_MEDIAN_COL_3x3RECT(u8)

FUNC_MEDIAN_PLUS(u8)
FUNC_MEDIAN_COL_3x3PLUS(u8)

FUNC_MEDIAN_XMSK(u8)
FUNC_MEDIAN_COL_3x3XMSK(u8)

/* *********************************************************** */

FUNC_MEDIAN_RECT(s16)
FUNC_MEDIAN_COL_3x3RECT(s16)

FUNC_MEDIAN_PLUS(s16)
FUNC_MEDIAN_COL_3x3PLUS(s16)

FUNC_MEDIAN_XMSK(s16)
FUNC_MEDIAN_COL_3x3XMSK(s16)

/* *********************************************************** */

FUNC_MEDIAN_RECT(u16)
FUNC_MEDIAN_COL_3x3RECT(u16)

FUNC_MEDIAN_PLUS(u16)
FUNC_MEDIAN_COL_3x3PLUS(u16)

FUNC_MEDIAN_XMSK(u16)
FUNC_MEDIAN_COL_3x3XMSK(u16)

/* *********************************************************** */

FUNC_MEDIAN_RECT(s32)
FUNC_MEDIAN_COL_3x3RECT(s32)

FUNC_MEDIAN_PLUS(s32)
FUNC_MEDIAN_COL_3x3PLUS(s32)

FUNC_MEDIAN_XMSK(s32)
FUNC_MEDIAN_COL_3x3XMSK(s32)

/* *********************************************************** */

FUNC_MEDIAN_RECT(f32)
FUNC_MEDIAN_COL_3x3RECT(f32)

FUNC_MEDIAN_PLUS(f32)
FUNC_MEDIAN_COL_3x3PLUS(f32)

FUNC_MEDIAN_XMSK(f32)
FUNC_MEDIAN_COL_3x3XMSK(f32)

/* *********************************************************** */

FUNC_MEDIAN_RECT(d64)
FUNC_MEDIAN_COL_3x3RECT(d64)

FUNC_MEDIAN_PLUS(d64)
FUNC_MEDIAN_COL_3x3PLUS(d64)

FUNC_MEDIAN_XMSK(d64)
FUNC_MEDIAN_COL_3x3XMSK(d64)

/* *********************************************************** */

const mlib_median_fun_type mlib_median_row_3x3_funs[] = {
	mlib_median_row_3x3rect_u8,
	mlib_median_row_3x3plus_u8,
	mlib_median_row_3x3xmsk_u8,
	mlib_median_row_3x3rsep_u8,
	mlib_median_row_3x3rect_s16,
	mlib_median_row_3x3plus_s16,
	mlib_median_row_3x3xmsk_s16,
	mlib_median_row_3x3rsep_s16,
	mlib_median_row_3x3rect_u16,
	mlib_median_row_3x3plus_u16,
	mlib_median_row_3x3xmsk_u16,
	mlib_median_row_3x3rsep_u16,
	mlib_median_row_3x3rect_s32,
	mlib_median_row_3x3plus_s32,
	mlib_median_row_3x3xmsk_s32,
	mlib_median_row_3x3rsep_s32,
	mlib_median_row_3x3rect_f32,
	mlib_median_row_3x3plus_f32,
	mlib_median_row_3x3xmsk_f32,
	mlib_median_row_3x3rsep_f32,
	mlib_median_row_3x3rect_d64,
	mlib_median_row_3x3plus_d64,
	mlib_median_row_3x3xmsk_d64,
	mlib_median_row_3x3rsep_d64
};

const mlib_median_fun_type mlib_median_col_3x3_funs[] = {
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

const mlib_median_fun_type mlib_median_init_3x3_funs[] = {
	mlib_median_initrow_3x3rsep_u8,
	mlib_median_initrow_3x3rsep_s16,
	mlib_median_initrow_3x3rsep_u16,
	mlib_median_initrow_3x3rsep_s32,
	mlib_median_initrow_3x3rsep_f32,
	mlib_median_initrow_3x3rsep_d64
};

/* *********************************************************** */

mlib_status
__mlib_ImageMedianFilter3x3(
    mlib_image *dst,
    const mlib_image *src,
    mlib_median_mask mmask,
    mlib_s32 cmask,
    mlib_edge edge)
{
	return mlib_ImageMedianFilter3x3_alltypes(dst, src, mmask, cmask, edge,
	    0);
}

/* *********************************************************** */
