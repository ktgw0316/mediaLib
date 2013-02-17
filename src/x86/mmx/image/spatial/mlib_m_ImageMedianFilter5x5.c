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

#pragma ident	"@(#)mlib_m_ImageMedianFilter5x5.c	9.6	07/11/05 SMI"

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
#include <mlib_m_ImageMedianFilter.h>

/* *************************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMedianFilter5x5 = __mlib_ImageMedianFilter5x5

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageMedianFilter5x5) mlib_ImageMedianFilter5x5
    __attribute__((weak, alias("__mlib_ImageMedianFilter5x5")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

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

#define	MMX_MEDIAN_5x5rect(itype)                               \
	{                                                       \
	    __m64 d10, d11, d12, d13, d14, d15, d16, d17;       \
	    __m64 d18, d19, d20, d21, d22, d23, d24, d25;       \
	                                                        \
	    d1 = *(__m64 *) (pp0 - 2 * bchan);                  \
	    d2 = *(__m64 *) (pp0 - bchan);                      \
	    d3 = *(__m64 *) (pp0);                              \
	    d4 = *(__m64 *) (pp0 + bchan);                      \
	    d5 = *(__m64 *) (pp0 + 2 * bchan);                  \
	    d6 = *(__m64 *) (pp1 - 2 * bchan);                  \
	    d7 = *(__m64 *) (pp1 - bchan);                      \
	    d8 = *(__m64 *) (pp1);                              \
	    d9 = *(__m64 *) (pp1 + bchan);                      \
	    d10 = *(__m64 *) (pp1 + 2 * bchan);                 \
	    d11 = *(__m64 *) (pp2 - 2 * bchan);                 \
	    d12 = *(__m64 *) (pp2 - bchan);                     \
	    d13 = *(__m64 *) (pp2);                             \
	    d14 = *(__m64 *) (pp2 + bchan);                     \
	    d15 = *(__m64 *) (pp2 + 2 * bchan);                 \
	    d16 = *(__m64 *) (pp3 - 2 * bchan);                 \
	    d17 = *(__m64 *) (pp3 - bchan);                     \
	    d18 = *(__m64 *) (pp3);                             \
	    d19 = *(__m64 *) (pp3 + bchan);                     \
	    d20 = *(__m64 *) (pp3 + 2 * bchan);                 \
	    d21 = *(__m64 *) (pp4 - 2 * bchan);                 \
	    d22 = *(__m64 *) (pp4 - bchan);                     \
	    d23 = *(__m64 *) (pp4);                             \
	    d24 = *(__m64 *) (pp4 + bchan);                     \
	    d25 = *(__m64 *) (pp4 + 2 * bchan);                 \
	    MMX_SORT25(itype);                                  \
	    d5 = d13;                                           \
	}

/* *********************************************************** */

#define	MMX_MEDIAN_5x5plus(itype)                               \
	d1 = *(__m64 *) (pp0);                                  \
	d2 = *(__m64 *) (pp1);                                  \
	d3 = *(__m64 *) (pp2 - 2 * bchan);                      \
	d4 = *(__m64 *) (pp2 - bchan);                          \
	d5 = *(__m64 *) (pp2);                                  \
	d6 = *(__m64 *) (pp2 + bchan);                          \
	d7 = *(__m64 *) (pp2 + 2 * bchan);                      \
	d8 = *(__m64 *) (pp3);                                  \
	d9 = *(__m64 *) (pp4);                                  \
	MMX_SORT9(d1, d2, d3, d4, d5, d6, d7, d8, d9, itype)

/* *********************************************************** */

#define	MMX_MEDIAN_5x5xmsk(itype)                               \
	d1 = *(__m64 *) (pp0 - 2 * bchan);                      \
	d2 = *(__m64 *) (pp0 + 2 * bchan);                      \
	d3 = *(__m64 *) (pp1 - bchan);                          \
	d4 = *(__m64 *) (pp1 + bchan);                          \
	d5 = *(__m64 *) (pp2);                                  \
	d6 = *(__m64 *) (pp3 - bchan);                          \
	d7 = *(__m64 *) (pp3 + bchan);                          \
	d8 = *(__m64 *) (pp4 - 2 * bchan);                      \
	d9 = *(__m64 *) (pp4 + 2 * bchan);                      \
	MMX_SORT9(d1, d2, d3, d4, d5, d6, d7, d8, d9, itype)

/* *********************************************************** */

#define	FUNC_MEDIAN_ROW(MEDMASK, img_type)                          \
	void                                                        \
	mlib_median_row_##MEDMASK##_##img_type(mlib_u8 *dl,         \
	    void **psl, mlib_filter_state * fs)                     \
	{                                                           \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask, size =   \
		fs->size;                                           \
	    mlib_##img_type *dp, *sp0, *sp1, *sp2, *sp3, *sp4;      \
	    sorttype_##img_type mask, d1, d2, d3, d4, d5, d6, d7,   \
		d8, d9;                                             \
	    mlib_s32 off0 = -2 * nchan, off1 = -nchan, off2 = 0;    \
	    mlib_s32 off3 = nchan, off4 = 2 * nchan, i, k;          \
	                                                            \
	    for (k = 0; k < nchan; k++) {                           \
		if (!(cmask & (1 << (nchan - 1 - k))))              \
		    continue;                                       \
		dp = (mlib_##img_type *) dl + k;                    \
		sp0 = (mlib_##img_type *) psl[0] + k;               \
		sp1 = (mlib_##img_type *) psl[1] + k;               \
		sp2 = (mlib_##img_type *) psl[2] + k;               \
		sp3 = (mlib_##img_type *) psl[3] + k;               \
		sp4 = (mlib_##img_type *) psl[4] + k;               \
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

#define	_MMX_MEDIAN_ROW(MEDMASK, img_type)                            \
	void                                                          \
	mlib_m_median_row_##MEDMASK##_##img_type(mlib_u8              \
	    *dl, void **psl, mlib_filter_state * fs)                  \
	{                                                             \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask, size =     \
		fs->size;                                             \
	    mlib_u8 *dp0, *pp0, *pp1, *pp2, *pp3, *pp4;               \
	    mlib_##img_type *dp, *sp0, *sp1, *sp2, *sp3, *sp4;        \
	    __m64 mask80, mask, d1, d2, d3, d4, d5, d6, d7, d8, d9;   \
	    mlib_s32 off0 = -2 * nchan, off1 = -nchan, off2 = 0;      \
	    mlib_s32 off3 = nchan, off4 = 2 * nchan;                  \
	    mlib_s32 bchan = nchan * sizeof (mlib_##img_type);        \
	    mlib_s32 bsize = bchan * size;                            \
	    mlib_s32 amask = (1 << nchan) - 1;                        \
	    mlib_s32 i, k;                                            \
	                                                              \
	    if ((cmask & amask) != amask) {                           \
		mlib_median_row_##MEDMASK##_##img_type(dl,            \
		    psl, fs);                                         \
		return;                                               \
	    }                                                         \
	    dp0 = (mlib_u8 *)dl;                                      \
	    pp0 = (mlib_u8 *)psl[0];                                  \
	    pp1 = (mlib_u8 *)psl[1];                                  \
	    pp2 = (mlib_u8 *)psl[2];                                  \
	    pp3 = (mlib_u8 *)psl[3];                                  \
	    pp4 = (mlib_u8 *)psl[4];                                  \
	    amask =                                                   \
		(sizeof (mlib_##img_type) ==                          \
		1) ? 0x80808080 : 0x80008000;                         \
	    mask80 = mmx_from_int_dup(amask);                         \
	    for (i = 0; i <= bsize - 8; i += 8) {                     \
		MMX_MEDIAN_##MEDMASK(img_type);                       \
		*(__m64 *) dp0 = d5;                                  \
		pp0 += 8;                                             \
		pp1 += 8;                                             \
		pp2 += 8;                                             \
		pp3 += 8;                                             \
		pp4 += 8;                                             \
		dp0 += 8;                                             \
	    }                                                         \
	    dp = (mlib_##img_type *) dp0;                             \
	    sp0 = (mlib_##img_type *) pp0;                            \
	    sp1 = (mlib_##img_type *) pp1;                            \
	    sp2 = (mlib_##img_type *) pp2;                            \
	    sp3 = (mlib_##img_type *) pp3;                            \
	    sp4 = (mlib_##img_type *) pp4;                            \
	    for (; i < bsize; i += sizeof (mlib_##img_type)) {        \
		sorttype_##img_type mask, d1, d2, d3, d4, d5, d6,     \
		    d7, d8, d9;                                       \
		MEDIAN_##MEDMASK(img_type);                           \
		sp0++;                                                \
		sp1++;                                                \
		sp2++;                                                \
		sp3++;                                                \
		sp4++;                                                \
		dp++;                                                 \
	    }                                                         \
	    _mm_empty();                                              \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_COL(MEDMASK, img_type)                          \
	void                                                        \
	mlib_median_col_##MEDMASK##_##img_type(mlib_u8 *dl,         \
	    void **psl, mlib_filter_state * fs)                     \
	{                                                           \
	    mlib_s32 nchan = fs->nchan, cmask = fs->cmask, size =   \
		fs->size;                                           \
	    mlib_s32 *x_off = fs->x_off, dlb = fs->dlp;             \
	    mlib_##img_type *dp, *sp0, *sp1, *sp2, *sp3, *sp4;      \
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
		dp = (mlib_##img_type *) dl + k;                    \
		for (i = 0; i < size; i++) {                        \
		    sp0 = (mlib_##img_type *) psl[i];               \
		    sp1 = (mlib_##img_type *) psl[i + 1];           \
		    sp2 = (mlib_##img_type *) psl[i + 2];           \
		    sp3 = (mlib_##img_type *) psl[i + 3];           \
		    sp4 = (mlib_##img_type *) psl[i + 4];           \
		    MEDIAN_##MEDMASK(img_type);                     \
		    dp += dlb;                                      \
		}                                                   \
	    }                                                       \
	}

/* *********************************************************** */

#define	PROT_MEDIAN_INITROW_5x5RSEP(img_type)                   \
	void                                                    \
	mlib_median_initrow_5x5rsep_##img_type(mlib_u8 *dl,     \
	    void **psl, mlib_filter_state * fs)

/* *********************************************************** */

#define	PROT_MEDIAN_ROW_5x5RSEP(img_type)                             \
	void                                                          \
	mlib_median_row_5x5rsep_##img_type(mlib_u8 *dl, void **psl,   \
	    mlib_filter_state * fs)

/* *********************************************************** */

#define	_MMX_MEDIAN_ROW_5x5RSEP(img_type)                       \
	void                                                    \
	mlib_m_median_row_5x5rsep_##img_type(mlib_u8 *dl,       \
	    void **psl, mlib_filter_state * fs)

/* *********************************************************** */

#define	PROT_MEDIAN_COL_5x5RSEP(img_type)                             \
	void                                                          \
	mlib_median_col_5x5rsep_##img_type(mlib_u8 *dl, void **psl,   \
	    mlib_filter_state * fs)

/* *********************************************************** */

FUNC_MEDIAN_ROW(5x5rect, u8)
_MMX_MEDIAN_ROW(5x5rect, u8)
FUNC_MEDIAN_COL(5x5rect, u8)

FUNC_MEDIAN_ROW(5x5plus, u8)
_MMX_MEDIAN_ROW(5x5plus, u8)
FUNC_MEDIAN_COL(5x5plus, u8)

FUNC_MEDIAN_ROW(5x5xmsk, u8)
_MMX_MEDIAN_ROW(5x5xmsk, u8)
FUNC_MEDIAN_COL(5x5xmsk, u8)

PROT_MEDIAN_ROW_5x5RSEP(u8);
_MMX_MEDIAN_ROW_5x5RSEP(u8);
PROT_MEDIAN_COL_5x5RSEP(u8);
PROT_MEDIAN_INITROW_5x5RSEP(u8);

/* *********************************************************** */

FUNC_MEDIAN_ROW(5x5rect, s16)
_MMX_MEDIAN_ROW(5x5rect, s16)
FUNC_MEDIAN_COL(5x5rect, s16)

FUNC_MEDIAN_ROW(5x5plus, s16)
_MMX_MEDIAN_ROW(5x5plus, s16)
FUNC_MEDIAN_COL(5x5plus, s16)

FUNC_MEDIAN_ROW(5x5xmsk, s16)
_MMX_MEDIAN_ROW(5x5xmsk, s16)
FUNC_MEDIAN_COL(5x5xmsk, s16)

PROT_MEDIAN_ROW_5x5RSEP(s16);
_MMX_MEDIAN_ROW_5x5RSEP(s16);
PROT_MEDIAN_COL_5x5RSEP(s16);
PROT_MEDIAN_INITROW_5x5RSEP(s16);

/* *********************************************************** */

FUNC_MEDIAN_ROW(5x5rect, u16)
_MMX_MEDIAN_ROW(5x5rect, u16)
FUNC_MEDIAN_COL(5x5rect, u16)

FUNC_MEDIAN_ROW(5x5plus, u16)
_MMX_MEDIAN_ROW(5x5plus, u16)
FUNC_MEDIAN_COL(5x5plus, u16)

FUNC_MEDIAN_ROW(5x5xmsk, u16)
_MMX_MEDIAN_ROW(5x5xmsk, u16)
FUNC_MEDIAN_COL(5x5xmsk, u16)

PROT_MEDIAN_ROW_5x5RSEP(u16);
_MMX_MEDIAN_ROW_5x5RSEP(u16);
PROT_MEDIAN_COL_5x5RSEP(u16);
PROT_MEDIAN_INITROW_5x5RSEP(u16);

/* *********************************************************** */

FUNC_MEDIAN_ROW(5x5rect, s32)
_MMX_MEDIAN_ROW(5x5rect, s32)
FUNC_MEDIAN_COL(5x5rect, s32)

FUNC_MEDIAN_ROW(5x5plus, s32)
_MMX_MEDIAN_ROW(5x5plus, s32)
FUNC_MEDIAN_COL(5x5plus, s32)

FUNC_MEDIAN_ROW(5x5xmsk, s32)
_MMX_MEDIAN_ROW(5x5xmsk, s32)
FUNC_MEDIAN_COL(5x5xmsk, s32)

PROT_MEDIAN_ROW_5x5RSEP(s32);
_MMX_MEDIAN_ROW_5x5RSEP(s32);
PROT_MEDIAN_COL_5x5RSEP(s32);
PROT_MEDIAN_INITROW_5x5RSEP(s32);

/* *********************************************************** */

FUNC_MEDIAN_ROW(5x5rect, f32)
FUNC_MEDIAN_COL(5x5rect, f32)

FUNC_MEDIAN_ROW(5x5plus, f32)
FUNC_MEDIAN_COL(5x5plus, f32)

FUNC_MEDIAN_ROW(5x5xmsk, f32)
FUNC_MEDIAN_COL(5x5xmsk, f32)

PROT_MEDIAN_ROW_5x5RSEP(f32);
PROT_MEDIAN_COL_5x5RSEP(f32);
PROT_MEDIAN_INITROW_5x5RSEP(f32);

/* *********************************************************** */
FUNC_MEDIAN_ROW(5x5rect, d64)
FUNC_MEDIAN_COL(5x5rect, d64)

FUNC_MEDIAN_ROW(5x5plus, d64)
FUNC_MEDIAN_COL(5x5plus, d64)

FUNC_MEDIAN_ROW(5x5xmsk, d64)
FUNC_MEDIAN_COL(5x5xmsk, d64)

PROT_MEDIAN_ROW_5x5RSEP(d64);
PROT_MEDIAN_COL_5x5RSEP(d64);
PROT_MEDIAN_INITROW_5x5RSEP(d64);

/* *********************************************************** */

static mlib_median_fun_type median_row_5x5_funs[] = {
	mlib_m_median_row_5x5rect_u8,
	mlib_m_median_row_5x5plus_u8,
	mlib_m_median_row_5x5xmsk_u8,
	mlib_m_median_row_5x5rsep_u8,
	mlib_m_median_row_5x5rect_s16,
	mlib_m_median_row_5x5plus_s16,
	mlib_m_median_row_5x5xmsk_s16,
	mlib_m_median_row_5x5rsep_s16,
	mlib_m_median_row_5x5rect_u16,
	mlib_m_median_row_5x5plus_u16,
	mlib_m_median_row_5x5xmsk_u16,
	mlib_m_median_row_5x5rsep_u16,
	mlib_m_median_row_5x5rect_s32,
	mlib_m_median_row_5x5plus_s32,
	mlib_m_median_row_5x5xmsk_s32,
	mlib_m_median_row_5x5rsep_s32,
	mlib_median_row_5x5rect_f32,
	mlib_median_row_5x5plus_f32,
	mlib_median_row_5x5xmsk_f32,
	mlib_median_row_5x5rsep_f32,
	mlib_median_row_5x5rect_d64,
	mlib_median_row_5x5plus_d64,
	mlib_median_row_5x5xmsk_d64,
	mlib_median_row_5x5rsep_d64
};

static mlib_median_fun_type median_col_5x5_funs[] = {
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

static mlib_median_fun_type median_init_5x5_funs[] = {
	mlib_median_initrow_5x5rsep_u8,
	mlib_median_initrow_5x5rsep_s16,
	mlib_median_initrow_5x5rsep_u16,
	mlib_median_initrow_5x5rsep_s32,
	mlib_median_initrow_5x5rsep_f32,
	mlib_median_initrow_5x5rsep_d64
};

/* *********************************************************** */

mlib_status
mlib_ImageMedianFilter5x5_alltypes(
    mlib_image *dst,
    mlib_image *src,
    mlib_median_mask mmask,
    mlib_s32 cmask,
    mlib_edge edge,
    mlib_s32 fp_version)
{
	mlib_filter_state fs;

	fs.m = 5;
	fs.n = 5;
	fs.mmask = mmask;
	fs.cmask = cmask;
	fs.edge = edge;
	fs.fp_version = fp_version;
	fs.mn_version = 0;
	fs.row_funs = median_row_5x5_funs;
	fs.col_funs = median_col_5x5_funs;
	fs.init_funs = median_init_5x5_funs;

	return (mlib_ImageMedianFilter_fun(dst, src, &fs));
}

/* *********************************************************** */

mlib_status
__mlib_ImageMedianFilter5x5(
    mlib_image *dst,
    const mlib_image *src,
    mlib_median_mask mmask,
    mlib_s32 cmask,
    mlib_edge edge)
{
	return mlib_ImageMedianFilter5x5_alltypes(dst, (void *)src, mmask,
	    cmask, edge, 0);
}

/* *********************************************************** */
