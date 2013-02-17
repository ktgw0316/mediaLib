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

#pragma ident   "@(#)mlib_ImageMedianFilterMxN.c	9.2    07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageMedianFilterMxN    - median filtering with MxN mask
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMedianFilterMxN(mlib_image       *dst,
 *                                            const mlib_image *src,
 *                                            mlib_s32         m,
 *                                            mlib_s32         n,
 *                                            mlib_median_mask mmask,
 *                                            mlib_s32         cmask,
 *                                            mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst     Pointer to destination image.
 *      src     Pointer to source image.
 *      m       width of filter mask, m must be an odd number larger than 1
 *      n       height of filter mask, n must be an odd number larger than 1
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
 *      In the case with an MLIB_MEDIAN_MASK_X mask, there must be (m == n).
 *
 *      The edge condition can be one of the following:
 *              MLIB_EDGE_DST_NO_WRITE  (default)
 *              MLIB_EDGE_DST_FILL_ZERO
 *              MLIB_EDGE_DST_COPY_SRC
 *              MLIB_EDGE_SRC_EXTEND
 */

#include <mlib_image.h>
#include <mlib_ImageMedianFilter.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMedianFilterMxN = __mlib_ImageMedianFilterMxN

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMedianFilterMxN) mlib_ImageMedianFilterMxN
    __attribute__((weak, alias("__mlib_ImageMedianFilterMxN")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	BUFF_MxN_SIZE	2048

/* *********************************************************** */

#define	MIN(x, y)	((x < y) ? x : y)

/* *********************************************************** */

#define	GET_DATA_u8(res, x)	res = x
#define	GET_DATA_s16(res, x)	res = x

#define	GET_DATA_u16(res, x)	res = x

#define	GET_DATA_s32(res, x)	res = x

#define	GET_DATA_f32(res, x)	res = x

#define	GET_DATA_d64(res, x)	res = x

/* *********************************************************** */

#define	DEF_VARS(itype)                                         \
	mlib_s32 nchan = fs->nchan, cmask = fs->cmask;          \
	mlib_s32 size = fs->size;                               \
	mlib_s32 k

/* *********************************************************** */

#define	ADD_DATA(x, itype)                                      \
	{                                                       \
	    GET_DATA_##itype(sort_arr[0], x);                   \
	    sort_arr++;                                         \
	}

/* *********************************************************** */

#define	FIND_MEDIAN(res, itype)                                 \
	sort_arr = fs->pbuff_MxN;                               \
	mlib_qsort_##itype(sort_arr, 2 * rank - 1);             \
	res = sort_arr[rank - 1]

/* *********************************************************** */

#define	FUNC_QSORT(itype)                                              \
	static void                                                    \
	mlib_qsort_##itype(ptype_##itype *data, mlib_s32 n)        \
	{                                                          \
	    ptype_##itype mask, val, a1, a2, a3, a4, a5, a6;       \
	    mlib_s32 i, j, m;                                      \
	                                                           \
	    if (n <= 1)                                            \
		return;                                                \
	    switch (n) {                                           \
	    case 2:                                                \
		a1 = data[0];                                          \
		a2 = data[1];                                          \
		SSORT2_##itype(a1, a2);                                \
		data[0] = a1;                                          \
		data[1] = a2;                                          \
		break;                                                 \
	    case 3:                                                \
		a1 = data[0];                                          \
		a2 = data[1];                                          \
		a3 = data[2];                                          \
		SSORT2_##itype(a1, a2);                                \
		SSORT2_##itype(a1, a3);                                \
		SSORT2_##itype(a2, a3);                                \
		data[0] = a1;                                          \
		data[1] = a2;                                          \
		data[2] = a3;                                          \
		break;                                                 \
	    case 4:                                                \
		a1 = data[0];                                          \
		a2 = data[1];                                          \
		a3 = data[2];                                          \
		a4 = data[3];                                          \
		SSORT2_##itype(a1, a2);                                \
		SSORT2_##itype(a3, a4);                                \
		SSORT2_##itype(a2, a4);                                \
		SSORT2_##itype(a1, a3);                                \
		SSORT2_##itype(a2, a3);                                \
		data[0] = a1;                                          \
		data[1] = a2;                                          \
		data[2] = a3;                                          \
		data[3] = a4;                                          \
		break;                                                 \
	    case 5:                                                \
		a1 = data[0];                                          \
		a2 = data[1];                                          \
		a3 = data[2];                                          \
		a4 = data[3];                                          \
		a5 = data[4];                                          \
		SSORT2_##itype(a1, a2);                                \
		SSORT2_##itype(a3, a4);                                \
		SSORT2_##itype(a3, a5);                                \
		SSORT2_##itype(a4, a5);                                \
		SSORT2_##itype(a1, a3);                                \
		SSORT2_##itype(a2, a5);                                \
		SSORT2_##itype(a2, a3);                                \
		SSORT2_##itype(a2, a4);                                \
		SSORT2_##itype(a3, a4);                                \
		data[0] = a1;                                          \
		data[1] = a2;                                          \
		data[2] = a3;                                          \
		data[3] = a4;                                          \
		data[4] = a5;                                          \
		break;                                                 \
	    case 6:                                                \
		a1 = data[0];                                          \
		a2 = data[1];                                          \
		a3 = data[2];                                          \
		a4 = data[3];                                          \
		a5 = data[4];                                          \
		a6 = data[5];                                          \
		SSORT2_##itype(a1, a2);                                \
		SSORT2_##itype(a3, a4);                                \
		SSORT2_##itype(a5, a6);                                \
		SSORT2_##itype(a1, a3);                                \
		SSORT2_##itype(a1, a5);                                \
		SSORT2_##itype(a2, a6);                                \
		SSORT2_##itype(a4, a6);                                \
		SSORT2_##itype(a2, a3);                                \
		SSORT2_##itype(a4, a5);                                \
		SSORT2_##itype(a2, a4);                                \
		SSORT2_##itype(a3, a5);                                \
		SSORT2_##itype(a3, a4);                                \
		data[0] = a1;                                          \
		data[1] = a2;                                          \
		data[2] = a3;                                          \
		data[3] = a4;                                          \
		data[4] = a5;                                          \
		data[5] = a6;                                          \
		break;                                                 \
	    default:                                               \
		i = 0;                                                 \
		j = n - 1;                                             \
		m = (i + j) / 2;                                       \
		val = data[m];                                         \
		data[m] = data[0];                                     \
		data[0] = val;                                         \
		for (; ; ) {                                           \
		    while (R_MIN_##itype(val, data[j]))                \
			j--;                                               \
		    if (j <= i)                                        \
			break;                                             \
		    data[i++] = data[j];                               \
		    data[j] = val;                                     \
		    while (R_MAX_##itype(val, data[i]))                \
			i++;                                               \
		    if (i >= j)                                        \
			break;                                             \
		    data[j--] = data[i];                               \
		    data[i] = val;                                     \
		}                                                      \
		if (i < m) {                                           \
		    if (i > 1)                                         \
			mlib_qsort_##itype(data, i);                       \
		    mlib_qsort_##itype(data + i + 1, n - i - 1);       \
		} else {                                               \
		    if (i < n - 2)                                     \
			mlib_qsort_##itype(data + i + 1, n - i - 1);       \
		    mlib_qsort_##itype(data, i);                       \
		}                                                      \
	    }                                                      \
	}

/* *********************************************************** */

#define	REPLACE_ELEM(itype, dst, size, old_data, new_data)    \
	{                                                         \
	    ptype_##itype *ptr, *dst_end = dst + size;            \
	                                                          \
	    if (R_MIN_##itype(old_data, new_data)) {              \
		for (ptr = dst; ptr[0] != old_data; ptr++);           \
		ptr++;                                                \
		if (R_MAXEQ_##itype(new_data, dst_end[-1])) {         \
		    for (; ptr < dst_end; ptr++)                      \
			ptr[-1] = ptr[0];                                 \
		    dst_end[-1] = new_data;                           \
		} else {                                              \
		    for (; R_MIN_##itype(ptr[0], new_data); ptr++)    \
			ptr[-1] = ptr[0];                                 \
		    ptr[-1] = new_data;                               \
		}                                                     \
	    } else {                                              \
		for (ptr = dst_end - 1; ptr[0] != old_data; ptr--);   \
		ptr--;                                                \
		if (R_MINEQ_##itype(new_data, dst[0])) {              \
		    for (; ptr >= dst; ptr--)                         \
			ptr[1] = ptr[0];                                  \
		    dst[0] = new_data;                                \
		} else {                                              \
		    for (; R_MAX_##itype(ptr[0], new_data); ptr--)    \
			ptr[1] = ptr[0];                                  \
		    ptr[1] = new_data;                                \
		}                                                     \
	    }                                                     \
	}

/* *********************************************************** */

#define	REPLACE_SORTLINE(itype, _dst, _src, size, _src1, _src2, size12)  \
	{                                                                \
/*                                                                       \
 * converts _src to _dst by deleting elements in _src1 and adding        \
 * elements in _src2                                                     \
 */                                                                      \
	    ptype_##itype *dst = _dst, del_data, add_data;        \
	    ptype_##itype *src = _src, *end = src + size;         \
	    ptype_##itype *src1 = _src1, *end1 = src1 + size12;   \
	    ptype_##itype *src2 = _src2, *end2 = src2 + size12;   \
	                                                         \
	    for (; ; ) {                                         \
		if (R_MINEQ_##itype(src1[0], src2[0])) {             \
	                                                         \
/*                                                                       \
 * first delete element from src1                                        \
 */                                                          \
		    del_data = (*src1++);                            \
		    for (; R_MIN_##itype(src[0], del_data);          \
					(*dst++) = (*src++));                \
		    src++;                                           \
		    if (src1 < end1)                                 \
			continue;                                        \
/*                                                                       \
 * no more src1, only add src2                                           \
 */                                                          \
		    if (src < end)                                   \
			do {                                             \
			    add_data = src2[0];                          \
			    if (R_MIN_##itype(end[-1], add_data))        \
				break;                                       \
			    for (; R_MIN_##itype(src[0], add_data);      \
				(*dst++) = (*src++));                        \
			    (*dst++) = add_data;                         \
			}                                                \
			while (++src2 < end2);                           \
		    for (; src < end; (*dst++) = (*src++));          \
		    for (; src2 < end2; (*dst++) = (*src2++));       \
		    break;                                           \
		} else {                                             \
	                                                                 \
/*                                                                       \
 * first add element from src2                                           \
 */                                                          \
		    add_data = (*src2++);                            \
		    for (; R_MIN_##itype(src[0], add_data);          \
				(*dst++) = (*src++));                        \
		    (*dst++) = add_data;                             \
		    if (src2 < end2)                                 \
			continue;                                        \
/*                                                                       \
 * no more src2, only del src1                                           \
 */                                                          \
		    for (; src1 < end1; ) {                          \
			del_data = (*src1++);                            \
			for (; R_MIN_##itype(src[0], del_data);          \
				(*dst++) = (*src++));                        \
			src++;                                           \
		    }                                                \
		                                                     \
		    for (; src < end; (*dst++) = (*src++));          \
		    break;                                           \
		}                                                    \
	    }                                                    \
	}

/* *********************************************************** */

#define	FIND_MEDIAN_2P(itype, res, _src1, _src2, m, del_data)   \
	{                                                       \
	    ptype_##itype *src1 = _src1, *src2 = _src2, data;    \
	    mlib_s32 i;                                         \
	                                                        \
	    for (i = 0; i < m - 1; i++) {                       \
		if (R_MINEQ_##itype(*src1, *src2)) {            \
		    src1++;                                     \
		} else {                                        \
		    src2++;                                     \
		}                                               \
	    }                                               \
	    if (R_MINEQ_##itype(*src1, *src2)) {            \
		data = *src1;                                   \
		if (R_MAXEQ_##itype(data, del_data)) {          \
		    src1++;                                     \
		    if (src1 - _src1 >= m)                      \
			data = *src2;                               \
		    else                                        \
			data = T_MIN_##itype(*src1, *src2);         \
		}                                               \
		res = data;                                     \
	    } else {                                        \
		data = *src2;                                   \
		if (R_MAXEQ_##itype(data, del_data)) {          \
		    src2++;                                     \
		    if (src2 - _src2 >= m)                      \
			data = *src1;                               \
		    else                                        \
			data = T_MIN_##itype(*src1, *src2);         \
		}                                               \
		res = data;                                     \
	    }                                               \
	}

/* *********************************************************** */

#define	FIND_MEDIAN_2X(itype, res, _src1, _src2, m, del_data)   \
	{                                                       \
	    ptype_##itype *src1 = _src1, *src2 = _src2, data;    \
	    mlib_s32 i;                                         \
	                                                        \
	    for (i = 0; i < m - 1; i++) {                       \
		src1++;                                         \
		if (R_MINEQ_##itype(src1[-1], src2[0]))         \
		    continue;                                   \
		src1--;                                         \
		src2++;                                         \
	    }                                                   \
	    if (R_MINEQ_##itype(*src1, *src2)) {             \
		data = *src1;                                   \
		if (R_MAXEQ_##itype(data, del_data)) {          \
		    src1++;                                     \
		    if (src1 - _src1 >= m)                      \
			data = *src2;                               \
		    else                                        \
			data = T_MIN_##itype(*src1, *src2);         \
		}                                               \
		res = data;                                     \
	    } else {                                        \
		data = *src2;                                   \
		if (R_MAXEQ_##itype(data, del_data)) {          \
		    src2++;                                     \
		    if (src2 - _src2 >= m)                      \
			data = *src1;                               \
		    else                                        \
			data = T_MIN_##itype(*src1, *src2);         \
		}                                               \
		res = data;                                     \
	    }                                                   \
	}

/* *********************************************************** */

#define	FUNC_FIND_MEDIAN2P(itype)                                      \
	static ptype_##itype                                            \
	mlib_find_median2p_##itype(ptype_##itype *src1,                 \
	    mlib_s32 size1, ptype_##itype *src2, mlib_s32 size2,        \
	    mlib_s32 rank, ptype_##itype del_data)                      \
	{                                                              \
	    ptype_##itype *end1 = src1 + size1, *end2 = src2 + size2;   \
	    ptype_##itype data;                                         \
	    mlib_s32 i;                                                \
	                                                               \
	    for (i = 0; i < rank - 1; i++) {                           \
		if (R_MINEQ_##itype(*src1, *src2)) {                  \
		    src1++;                                            \
		    if (src1 >= end1) {                                \
			for (i++; i < rank - 1; i++)                   \
			    src2++;                                    \
			if (R_MAXEQ_##itype(*src2, del_data))         \
			    src2++;                                    \
			return (*src2);                                \
		    }                                                  \
		} else {                                               \
		    src2++;                                            \
		    if (src2 >= end2) {                                \
			for (i++; i < rank - 1; i++)                   \
			    src1++;                                    \
			if (R_MAXEQ_##itype(*src1, del_data))         \
			    src1++;                                    \
			return (*src1);                                \
		    }                                                  \
		}                                                      \
	    }                                                          \
	    if (R_MINEQ_##itype(*src1, *src2)) {                      \
		data = *src1;                                          \
		if (R_MAXEQ_##itype(data, del_data)) {                \
		    src1++;                                            \
		    if (src1 >= end1)                                  \
			data = *src2;                                  \
		    else                                               \
			data = T_MIN_##itype(*src1, *src2);              \
		}                                                      \
		return (data);                                         \
	    } else {                                                   \
		data = *src2;                                          \
		if (R_MAXEQ_##itype(data, del_data)) {                \
		    src2++;                                            \
		    if (src2 >= end2)                                  \
			data = *src1;                                  \
		    else                                               \
			data = T_MIN_##itype(*src1, *src2);              \
		}                                                      \
		return (data);                                         \
	    }                                                  \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_ROW_MxNRECT_I(itype)                                   \
	static void                                                        \
	mlib_median_initrow_MxNrect_##itype(mlib_u8 *pdst,                 \
	    void **_psl, mlib_filter_state *fs)                            \
	{                                                                  \
	    DEF_VARS(itype);                                               \
	    ptype_##itype *dl = (ptype_##itype *) pdst;                      \
	    ptype_##itype **psl = (ptype_##itype **) _psl;                   \
	    ptype_##itype *pwnd, *pwnd1, *pwnd2, *pcol, *pd, data;          \
	    mlib_s32 m = fs->m;                                            \
	    mlib_s32 x_off, n = fs->n, mn = m * n, m1_nchan =              \
		fs->m1_nchan, rank = fs->rank, i, j;                       \
	    pcol = fs->buff_n_rows;                                        \
	    pwnd1 = fs->pbuff_MxN;                                         \
	    pwnd2 = pwnd1 + mn;                                            \
	    for (k = 0; k < nchan; k++) {                                  \
		if (!(cmask & (1 << (nchan - 1 - k))))                     \
		    continue;                                              \
		pd = dl + k;                                               \
		pwnd = pwnd1;                                              \
		x_off = -m1_nchan + k;                                     \
		for (j = 0; j < m; j++) {                                  \
		    for (i = 0; i < n; i++) {                              \
			GET_DATA_##itype(data, psl[i][x_off]);             \
			(*pwnd++) = data;                                  \
			pcol[i] = data;                                    \
		    }                                                      \
		    mlib_qsort_##itype(pcol, n);                           \
		    x_off += nchan;                                        \
		    pcol += n;                                             \
		}                                                          \
		mlib_qsort_##itype(pwnd1, mn);                             \
		for (j = 0; j < size - 1; j++) {                           \
		    pd[0] = pwnd1[rank - 1];                               \
		    for (i = 0; i < n; i++) {                              \
			GET_DATA_##itype(pcol[i], psl[i][x_off]);          \
		    }                                                      \
		    mlib_qsort_##itype(pcol, n);                           \
		    REPLACE_SORTLINE(itype, pwnd2, pwnd1, mn, pcol - mn,   \
			pcol, n);                                          \
		    pwnd = pwnd1;                                          \
		    pwnd1 = pwnd2;                                         \
		    pwnd2 = pwnd;                                          \
		    pd += nchan;                                           \
		    x_off += nchan;                                        \
		    pcol += n;                                             \
		}                                                          \
		pd[0] = pwnd1[rank - 1];                                   \
	    }                                                              \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_ROW_MxNRECT(itype)                                     \
	FUNC_MEDIAN_ROW_MxNRECT_I(itype)                                   \
	    static void mlib_median_row_MxNrect_##itype(mlib_u8            \
	    *pdst, void **_psl, mlib_filter_state *fs)                     \
	{                                                                  \
	    DEF_VARS(itype);                                               \
	    ptype_##itype *dl = (ptype_##itype *) pdst;                      \
	    ptype_##itype **psl = (ptype_##itype **) _psl;                   \
	    ptype_##itype *pwnd, *pwnd1, *pwnd2, *pcol, *pd,                \
		data_old, data_new;                                        \
	    mlib_s32 m = fs->m;                                            \
	    mlib_s32 x_off, n = fs->n, mn = m * n, m1_nchan =              \
		fs->m1_nchan, rank = fs->rank, i, j;                       \
	    if (!fs->init_flag) {                                          \
		mlib_median_initrow_MxNrect_##itype(pdst, _psl, fs);       \
		fs->init_flag = 1;                                         \
		return;                                                    \
	    }                                                              \
	    pcol = (ptype_##itype *)fs->buff_n_rows;                   \
	    pwnd1 = (ptype_##itype *)fs->pbuff_MxN;                    \
	    pwnd2 = pwnd1 + mn;                                            \
	    for (k = 0; k < nchan; k++) {                                  \
		if (!(cmask & (1 << (nchan - 1 - k))))                     \
		    continue;                                              \
		pd = dl + k;                                               \
		pwnd = pwnd1;                                              \
		x_off = -m1_nchan + k;                                     \
		for (j = 0; j < m; j++) {                                  \
		    GET_DATA_##itype(data_old, psl[-1][x_off]);            \
		    GET_DATA_##itype(data_new, psl[n - 1][x_off]);         \
		    REPLACE_ELEM(itype, pcol, n, data_old, data_new);      \
		    for (i = 0; i < n; i++) {                              \
			(*pwnd++) = pcol[i];                               \
		    }                                                      \
		    x_off += nchan;                                        \
		    pcol += n;                                             \
		}                                                          \
		mlib_qsort_##itype(pwnd1, mn);                             \
		for (j = 0; j < size - 1; j++) {                           \
		    pd[0] = pwnd1[rank - 1];                               \
		    GET_DATA_##itype(data_old, psl[-1][x_off]);            \
		    GET_DATA_##itype(data_new, psl[n - 1][x_off]);         \
		    REPLACE_ELEM(itype, pcol, n, data_old, data_new);      \
		    REPLACE_SORTLINE(itype, pwnd2, pwnd1, mn, pcol - mn,   \
			pcol, n);                                          \
		    pwnd = pwnd1;                                          \
		    pwnd1 = pwnd2;                                         \
		    pwnd2 = pwnd;                                          \
		    pd += nchan;                                           \
		    x_off += nchan;                                        \
		    pcol += n;                                             \
		}                                                          \
		pd[0] = pwnd1[rank - 1];                                   \
	    }                                                              \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_ROW_MxNPLUS_I(itype)                                \
	static void                                                     \
	mlib_median_initrow_MxNplus_##itype(mlib_u8 *pdst,              \
	    void **_psl, mlib_filter_state *fs)                         \
	{                                                               \
	    DEF_VARS(itype);                                            \
	    ptype_##itype *dl = (ptype_##itype *) pdst;                   \
	    ptype_##itype **psl = (ptype_##itype **) _psl;                \
	    ptype_##itype *prow, *pcol, *ps0, *pd, data_old,             \
		data_new;                                               \
	    mlib_s32 m = fs->m;                                         \
	    mlib_s32 x_off, n = fs->n, rank = (m + n) / 2, kh1 =        \
		fs->kh1, m1_nchan = fs->m1_nchan, i, j;                 \
	    pcol = fs->buff_n_rows;                                     \
	    prow = fs->pbuff_MxN;                                       \
	    for (k = 0; k < nchan; k++) {                               \
		if (!(cmask & (1 << (nchan - 1 - k))))                  \
		    continue;                                           \
		pd = dl + k;                                            \
		ps0 = psl[kh1] - m1_nchan + k;                          \
		for (j = 0; j < m; j++) {                               \
		    GET_DATA_##itype(prow[j], ps0[j * nchan]);          \
		}                                                       \
		mlib_qsort_##itype(prow, m);                            \
		x_off = k;                                              \
		ps0 = psl[kh1] + k;                                     \
		for (j = 0; j < size; j++) {                            \
		    for (i = 0; i < n; i++) {                           \
			GET_DATA_##itype(pcol[i], psl[i][x_off]);       \
		    }                                                   \
		    mlib_qsort_##itype(pcol, n);                        \
		    if (m != n)                                         \
			pd[0] =                                         \
			    mlib_find_median2p_##itype(prow, m,         \
			    pcol, n, rank, ps0[0]);                     \
		    else                                                \
			FIND_MEDIAN_2P(itype, pd[0], prow, pcol, m,     \
			    ps0[0]);                                    \
		    GET_DATA_##itype(data_old, ps0[-m1_nchan]);         \
		    pd += nchan;                                        \
		    ps0 += nchan;                                       \
		    x_off += nchan;                                     \
		    pcol += n;                                          \
		    GET_DATA_##itype(data_new, ps0[m1_nchan]);          \
		    REPLACE_ELEM(itype, prow, m, data_old, data_new);   \
		}                                                       \
	    }                                                           \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_ROW_MxNPLUS(itype)                                  \
	FUNC_MEDIAN_ROW_MxNPLUS_I(itype)                                \
	                                                                \
	static void                                                     \
	mlib_median_row_MxNplus_##itype(                                \
	    mlib_u8 *pdst, void **_psl, mlib_filter_state *fs)          \
	{                                                               \
	    DEF_VARS(itype);                                            \
	    ptype_##itype *dl = (ptype_##itype *) pdst;                   \
	    ptype_##itype **psl = (ptype_##itype **) _psl;                \
	    ptype_##itype *prow, *pcol, *ps0, *pd, data_old,             \
		data_new;                                               \
	    mlib_s32 m = fs->m;                                         \
	    mlib_s32 x_off, n = fs->n, rank = (m + n) / 2, m1_nchan =   \
		fs->m1_nchan, kh1 = fs->kh1, j;                         \
	    if (!fs->init_flag) {                                       \
		mlib_median_initrow_MxNplus_##itype(pdst, _psl, fs);    \
		fs->init_flag = 1;                                      \
		return;                                                 \
	    }                                                           \
	    pcol = (ptype_##itype *)fs->buff_n_rows;             \
	    prow = (ptype_##itype *)fs->pbuff_MxN;               \
	    for (k = 0; k < nchan; k++) {                               \
		if (!(cmask & (1 << (nchan - 1 - k))))                  \
		    continue;                                           \
		pd = dl + k;                                            \
		ps0 = psl[kh1] - m1_nchan + k;                          \
		for (j = 0; j < m; j++) {                               \
		    GET_DATA_##itype(prow[j], ps0[j * nchan]);          \
		}                                                       \
		mlib_qsort_##itype(prow, m);                            \
		x_off = k;                                              \
		ps0 = psl[kh1] + k;                                     \
		for (j = 0; j < size - 1; j++) {                        \
		    GET_DATA_##itype(data_old, psl[-1][x_off]);         \
		    GET_DATA_##itype(data_new, psl[n - 1][x_off]);      \
		    REPLACE_ELEM(itype, pcol, n, data_old, data_new);   \
		    if (m != n)                                         \
			pd[0] =                                         \
			    mlib_find_median2p_##itype(prow, m,         \
			    pcol, n, rank, ps0[0]);                     \
		    else                                                \
			FIND_MEDIAN_2P(itype, pd[0], prow, pcol, m,     \
			    ps0[0]);                                    \
		    GET_DATA_##itype(data_old, ps0[-m1_nchan]);         \
		    pd += nchan;                                        \
		    ps0 += nchan;                                       \
		    x_off += nchan;                                     \
		    pcol += n;                                          \
		    GET_DATA_##itype(data_new, ps0[m1_nchan]);          \
		    REPLACE_ELEM(itype, prow, m, data_old, data_new);   \
		}                                                       \
		GET_DATA_##itype(data_old, psl[-1][x_off]);             \
		GET_DATA_##itype(data_new, psl[n - 1][x_off]);          \
		REPLACE_ELEM(itype, pcol, n, data_old, data_new);       \
		if (m != n)                                             \
		    pd[0] =                                             \
			mlib_find_median2p_##itype(prow, m,             \
			pcol, n, rank, ps0[0]);                         \
		else                                                    \
		    FIND_MEDIAN_2P(itype, pd[0], prow, pcol, m,         \
			ps0[0]);                                        \
		pcol += n;                                              \
	    }                                                           \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_ROW_MxNXMSK_I(itype)                               \
	static void                                                    \
	mlib_median_initrow_MxNxmsk_##itype(mlib_u8 *pdst,             \
	    void **_psl, mlib_filter_state *fs)                        \
	{                                                              \
	    DEF_VARS(itype);                                           \
	    ptype_##itype *dl = (ptype_##itype *) pdst;                  \
	    ptype_##itype **psl = (ptype_##itype **) _psl;               \
	    ptype_##itype *pdiag0, *pdiag1, *pd;                        \
	    mlib_s32 m = fs->m;                                        \
	    mlib_s32 x_off, off0, off1, m_nchan = fs->m_nchan, kw1 =   \
		fs->kw1, m1_nchan = fs->m1_nchan, i, j;                \
	    fs->buff_n_rows =                                          \
		(ptype_##itype *) (fs->buff_n_rows) + size * m_nchan;   \
	    for (k = 0; k < nchan; k++) {                              \
		if (!(cmask & (1 << (nchan - 1 - k))))                 \
		    continue;                                          \
		pd = dl + k;                                           \
		pdiag1 = (ptype_##itype *) (fs->buff_n_rows) + k * m;   \
		pdiag0 = pdiag1 - m_nchan;                             \
		x_off = k;                                             \
		for (j = 0; j < size; j++) {                           \
		    off0 = x_off - m1_nchan;                           \
		    off1 = x_off + m1_nchan;                           \
		    for (i = 0; i < m; i++) {                          \
			GET_DATA_##itype(pdiag0[i], psl[i][off0]);     \
			GET_DATA_##itype(pdiag1[i], psl[i][off1]);     \
			off0 += nchan;                                 \
			off1 -= nchan;                                 \
		    }                                                  \
		    mlib_qsort_##itype(pdiag0, m);                     \
		    mlib_qsort_##itype(pdiag1, m);                     \
		    FIND_MEDIAN_2X(itype, pd[0], pdiag0, pdiag1, m,    \
			psl[kw1][x_off]);                              \
		    pd += nchan;                                       \
		    x_off += nchan;                                    \
		    pdiag0 -= m_nchan;                                 \
		    pdiag1 += m_nchan;                                 \
		}                                                      \
	    }                                                          \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_ROW_MxNXMSK(itype)                                    \
	FUNC_MEDIAN_ROW_MxNXMSK_I(itype)                                  \
	    static void mlib_median_row_MxNxmsk_##itype(mlib_u8           \
	    *pdst, void **_psl, mlib_filter_state *fs)                    \
	{                                                                 \
	    DEF_VARS(itype);                                              \
	    ptype_##itype *dl = (ptype_##itype *) pdst;                     \
	    ptype_##itype **psl = (ptype_##itype **) _psl;                  \
	    ptype_##itype *pdiag0, *pdiag1, *pd, data_old, data_new;       \
	    mlib_s32 m = fs->m;                                           \
	    mlib_s32 x_off, off0, off1, n = fs->n, kw1 =                  \
		fs->kw1, m_nchan = fs->m_nchan, m1_nchan =                \
		fs->m1_nchan, i, j;                                       \
	    if (!fs->init_flag) {                                         \
		mlib_median_initrow_MxNxmsk_##itype(pdst, _psl, fs);      \
		fs->init_flag = 1;                                        \
		return;                                                   \
	    }                                                             \
	    fs->buff_n_rows =                                             \
		(ptype_##itype *) (fs->buff_n_rows) + m_nchan;             \
	    for (k = 0; k < nchan; k++) {                                 \
		if (!(cmask & (1 << (nchan - 1 - k))))                    \
		    continue;                                             \
		pd = dl + k;                                              \
		pdiag1 = (ptype_##itype *) (fs->buff_n_rows) + k * m;      \
		pdiag0 = pdiag1 - m_nchan;                                \
		x_off = k;                                                \
		off0 = x_off - m1_nchan;                                  \
		for (i = 0; i < m; i++) {                                 \
		    GET_DATA_##itype(pdiag0[i], psl[i][off0]);            \
		    off0 += nchan;                                        \
		}                                                         \
		mlib_qsort_##itype(pdiag0, m);                            \
		off1 = x_off + size * nchan + m1_nchan;                   \
		pdiag1 += (size - 1) * m_nchan;                           \
		for (i = 0; i < m; i++) {                                 \
		    GET_DATA_##itype(pdiag1[i], psl[i - 1][off1]);        \
		    off1 -= nchan;                                        \
		}                                                         \
		mlib_qsort_##itype(pdiag1, m);                            \
		pdiag1 = pdiag0 + m_nchan;                                \
		{ /* j = 0 */                                             \
		    off0 = x_off - m1_nchan;                              \
		    off1 = x_off + m1_nchan;                              \
		    GET_DATA_##itype(data_old,                            \
			psl[-1][off1 + nchan]);                           \
		    GET_DATA_##itype(data_new, psl[n - 1][off0]);         \
		    REPLACE_ELEM(itype, pdiag1, m, data_old, data_new);   \
		    FIND_MEDIAN_2X(itype, pd[0], pdiag0, pdiag1, m,       \
			psl[kw1][x_off]);                                 \
		    pd += nchan;                                          \
		    x_off += nchan;                                       \
		    pdiag0 -= m_nchan;                                    \
		    pdiag1 += m_nchan;                                    \
		}                                                         \
		for (j = 1; j < size; j++) {                              \
		    off0 = x_off - m1_nchan;                              \
		    off1 = x_off + m1_nchan;                              \
		    GET_DATA_##itype(data_old,                            \
			psl[-1][off0 - nchan]);                           \
		    GET_DATA_##itype(data_new, psl[n - 1][off1]);         \
		    REPLACE_ELEM(itype, pdiag0, m, data_old, data_new);   \
		    GET_DATA_##itype(data_old,                            \
			psl[-1][off1 + nchan]);                           \
		    GET_DATA_##itype(data_new, psl[n - 1][off0]);         \
		    REPLACE_ELEM(itype, pdiag1, m, data_old, data_new);   \
		    FIND_MEDIAN_2X(itype, pd[0], pdiag0, pdiag1, m,       \
			psl[kw1][x_off]);                                 \
		    pd += nchan;                                          \
		    x_off += nchan;                                       \
		    pdiag0 -= m_nchan;                                    \
		    pdiag1 += m_nchan;                                    \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_COL_MxNRECT(itype)                                \
	static void                                                   \
	mlib_median_col_MxNrect_##itype(mlib_u8 *pdst, void **_psl,   \
	    mlib_filter_state *fs)                                    \
	{                                                             \
	    DEF_VARS(itype);                                          \
	    ptype_##itype *dl = (ptype_##itype *) pdst;                 \
	    ptype_##itype **psl = (ptype_##itype **) _psl;              \
	    mlib_s32 m = fs->m;                                       \
	    mlib_s32 *x_off = fs->x_off, dlb = fs->dlp;               \
	    ptype_##itype *sort_arr = fs->pbuff_MxN, *ps, *pd;         \
	    mlib_s32 l, i, j, n = fs->n, rank = (m * n + 1) / 2;      \
	                                                              \
	    for (k = 0; k < nchan; k++) {                             \
		if (!(cmask & (1 << (nchan - 1 - k))))                \
		    continue;                                         \
		pd = dl + k;                                          \
		for (l = 0; l < size; l++) {                          \
		    for (i = 0; i < n; i++) {                         \
			ps = psl[l + i] + k;                          \
			for (j = 0; j < m; j++) {                     \
			    ADD_DATA(ps[x_off[j]], itype);            \
			}                                             \
		    }                                                 \
		    FIND_MEDIAN(pd[0], itype);                        \
		    pd += dlb;                                        \
		}                                                     \
	    }                                                         \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_COL_MxNPLUS(itype)                                \
	static void                                                   \
	mlib_median_col_MxNplus_##itype(mlib_u8 *pdst, void **_psl,   \
	    mlib_filter_state *fs)                                    \
	{                                                             \
	    DEF_VARS(itype);                                          \
	    ptype_##itype *dl = (ptype_##itype *) pdst;                 \
	    ptype_##itype **psl = (ptype_##itype **) _psl;              \
	    mlib_s32 m = fs->m;                                       \
	    mlib_s32 *x_off = fs->x_off, dlb = fs->dlp;               \
	    mlib_s32 n = fs->n, kw1 = fs->kw1, i, j;                  \
	    ptype_##itype *sort_arr = fs->pbuff_MxN, *ps, *pd;         \
	    mlib_s32 l, x_off0, rank = (m + n) / 2, kh1 = fs->kh1;    \
	                                                              \
	    for (k = 0; k < nchan; k++) {                             \
		if (!(cmask & (1 << (nchan - 1 - k))))                \
		    continue;                                         \
		pd = dl + k;                                          \
		x_off0 = x_off[kw1];                                  \
		for (l = 0; l < size; l++) {                          \
		    ps = psl[l + kh1] + k;                            \
		    for (j = 0; j < m; j++) {                         \
			ADD_DATA(ps[x_off[j]], itype);                \
		    }                                                 \
		    for (i = 0; i < n; i++) {                         \
			if (i == kh1)                                 \
			    continue;                                 \
			ADD_DATA(psl[l + i][x_off0 + k], itype);      \
		    }                                                 \
		    FIND_MEDIAN(pd[0], itype);                        \
		    pd += dlb;                                        \
		}                                                     \
	    }                                                         \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_COL_MxNXMSK(itype)                                 \
	static void                                                    \
	mlib_median_col_MxNxmsk_##itype(mlib_u8 *pdst, void **_psl,    \
	    mlib_filter_state *fs)                                     \
	{                                                              \
	    DEF_VARS(itype);                                           \
	    ptype_##itype *dl = (ptype_##itype *) pdst;                  \
	    ptype_##itype **psl = (ptype_##itype **) _psl;               \
	    mlib_s32 m = fs->m;                                        \
	    mlib_s32 *x_off = fs->x_off, dlb = fs->dlp, i, j;          \
	    ptype_##itype *sort_arr = fs->pbuff_MxN, *ps, *pd;          \
	    mlib_s32 n = fs->n, rank = (m + n) / 2, kw1 = fs->kw1;     \
	                                                               \
	    for (k = 0; k < nchan; k++) {                              \
		if (!(cmask & (1 << (nchan - 1 - k))))                 \
		    continue;                                          \
		pd = dl + k;                                           \
		for (i = 0; i < size; i++) {                           \
		    for (j = 0; j < n; j++) {                          \
			ps = psl[i + j] + k;                           \
			ADD_DATA(ps[x_off[j]], itype);                 \
			if (j != kw1)                                  \
			    ADD_DATA(ps[x_off[(n - 1) - j]], itype);   \
		    }                                                  \
		    FIND_MEDIAN(pd[0], itype);                         \
		    pd += dlb;                                         \
		}                                                      \
	    }                                                          \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_INITROW_MxNRSEP(itype)                               \
	static void                                                      \
	mlib_median_initrow_MxNrsep_##itype(mlib_u8 *pdst,               \
	    void **_psl, mlib_filter_state *fs)                          \
	{                                                                \
	    DEF_VARS(itype);                                             \
	    ptype_##itype **psl = (ptype_##itype **) _psl;                 \
	    sorttype_##itype *buff;                                      \
	    ptype_##itype *pline =                                        \
		fs->pbuff_MxN, data_old, data_new, *ps;                  \
	    mlib_s32 m = fs->m;                                          \
	    mlib_s32 rank = (m + 1) / 2, n = fs->n, m_nchan =            \
		fs->m_nchan, m1_nchan = fs->m1_nchan, j;                 \
	    for (k = 0; k < nchan; k++) {                                \
		if (!(cmask & (1 << (nchan - 1 - k))))                   \
		    continue;                                            \
		ps = psl[0] - m1_nchan + k;                              \
		buff = (fs->buff_row)[fs->buff_ind];                     \
		buff += k;                                               \
		for (j = 0; j < m; j++) {                                \
		    GET_DATA_##itype(pline[j], ps[0]);                   \
		    ps += nchan;                                         \
		}                                                        \
		mlib_qsort_##itype(pline, m);                            \
		for (j = 0; j < size; j++) {                             \
		    buff[j * nchan] = pline[rank - 1];                   \
		    GET_DATA_##itype(data_old, ps[-m_nchan]);            \
		    GET_DATA_##itype(data_new, ps[0]);                   \
		    REPLACE_ELEM(itype, pline, m, data_old, data_new);   \
		    ps += nchan;                                         \
		}                                                        \
	    }                                                            \
	    if (++(fs->buff_ind) >= n)                                   \
		(fs->buff_ind) -= n;                                     \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_ROW_MxNRSEP(itype)                                \
	static void                                                   \
	mlib_median_row_MxNrsep_##itype(mlib_u8 *pdst, void **_psl,   \
	    mlib_filter_state *fs)                                    \
	{                                                             \
	    DEF_VARS(itype);                                          \
	    ptype_##itype *dl = (ptype_##itype *) pdst;                 \
	    ptype_##itype *sort_arr = fs->pbuff_MxN, *pd;              \
	    sorttype_##itype *buff0, *buff;                           \
	    mlib_s32 n = fs->n, i, j;                                 \
	    mlib_s32 buff_size = fs->buff_size, rank = (n + 1) / 2;   \
	                                                              \
	    mlib_median_initrow_MxNrsep_##itype(pdst,                 \
		_psl + (n - 1), fs);                                  \
	    for (k = 0; k < nchan; k++) {                             \
		if (!(cmask & (1 << (nchan - 1 - k))))                \
		    continue;                                         \
		buff0 = fs->buff_n_rows;                              \
		buff0 += k;                                           \
		pd = dl + k;                                          \
		for (j = 0; j < size; j++) {                          \
		    buff = buff0;                                     \
		    for (i = 0; i < n; i++) {                         \
			ADD_DATA(buff[0], itype);                     \
			buff += buff_size;                            \
		    }                                                 \
		    FIND_MEDIAN(pd[0], itype);                        \
		    buff0 += nchan;                                   \
		    pd += nchan;                                      \
		}                                                     \
	    }                                                         \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_COL_MxNRSEP(itype)                                \
	static void                                                   \
	mlib_median_col_MxNrsep_##itype(mlib_u8 *pdst, void **_psl,   \
	    mlib_filter_state *fs)                                    \
	{                                                             \
	    DEF_VARS(itype);                                          \
	    ptype_##itype *dl = (ptype_##itype *) pdst;                 \
	    ptype_##itype **psl = (ptype_##itype **) _psl;              \
	    mlib_s32 m = fs->m;                                       \
	    mlib_s32 *x_off = fs->x_off, dlb = fs->dlp, i, j;         \
	    ptype_##itype *sort_arr = fs->pbuff_MxN, *ps, *pd;         \
	    sorttype_##itype *buff;                                   \
	    mlib_s32 rank, l, n = fs->n;                              \
	                                                              \
	    for (k = 0; k < nchan; k++) {                             \
		if (!(cmask & (1 << (nchan - 1 - k))))                \
		    continue;                                         \
		buff = fs->buff_n_rows;                               \
/*                                                                    \
 * process row medians                                                \
 */                                                                   \
		rank = (m + 1) / 2;                                   \
		for (i = 0; i < size + n; i++) {                      \
		    ps = psl[i] + k;                                  \
		    for (j = 0; j < m; j++) {                         \
			ADD_DATA(ps[x_off[j]], itype);                \
		    }                                                 \
		    FIND_MEDIAN(buff[i], itype);                      \
		}                                                     \
	                                                              \
/*                                                                    \
 * process col medians                                                \
 */                                                                   \
		rank = (n + 1) / 2;                                   \
		pd = (void *)dl;                                      \
		pd += k;                                              \
		for (i = 0; i < size; i++) {                          \
		    for (l = 0; l < n; l++) {                         \
			ADD_DATA(buff[l], itype);                     \
		    }                                                 \
		    FIND_MEDIAN(pd[0], itype);                        \
		    pd += dlb;                                        \
		    buff++;                                           \
		}                                                     \
	    }                                                         \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_SEPROW(itype)                                        \
	static void                                                      \
	mlib_median_seprow_MxN_##itype(mlib_u8 *pdst, mlib_u8 *psrc,     \
	    mlib_filter_state *fs)                                       \
	{                                                                \
	    DEF_VARS(itype);                                             \
	    ptype_##itype *dl = (ptype_##itype *) pdst;                    \
	    ptype_##itype *pline =                                        \
		fs->pbuff_MxN, data_old, data_new, *ps, *pd;             \
	    mlib_s32 m = fs->m;                                          \
	    mlib_s32 rank = (m + 1) / 2, m1_nchan =                      \
		fs->m1_nchan, m_nchan = fs->m_nchan, j;                  \
	    for (k = 0; k < nchan; k++) {                                \
		if (!(cmask & (1 << (nchan - 1 - k))))                   \
		    continue;                                            \
		ps = (ptype_##itype *) psrc - m1_nchan + k;               \
		pd = dl + k;                                             \
		for (j = 0; j < m; j++) {                                \
		    GET_DATA_##itype(pline[j], ps[0]);                   \
		    ps += nchan;                                         \
		}                                                        \
		mlib_qsort_##itype(pline, m);                            \
		pd[0] = pline[rank - 1];                                 \
		for (j = 1; j < size; j++) {                             \
		    GET_DATA_##itype(data_old, ps[-m_nchan]);            \
		    GET_DATA_##itype(data_new, ps[0]);                   \
		    REPLACE_ELEM(itype, pline, m, data_old, data_new);   \
		    pd[j * nchan] = pline[rank - 1];                     \
		    ps += nchan;                                         \
		}                                                        \
	    }                                                            \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_SEPCOL(itype)                                        \
	static void                                                      \
	mlib_median_sepcol_MxN_##itype(mlib_u8 *pdst, mlib_u8 *psrc,     \
	    mlib_filter_state *fs)                                       \
	{                                                                \
	    DEF_VARS(itype);                                             \
	    mlib_s32 n = fs->n, i;                                       \
	    ptype_##itype *pline =                                        \
		fs->pbuff_MxN, data_old, data_new, *ps, *pd;             \
	    mlib_s32 slp = fs->wsize, dlp = fs->dlp, n_slp = n * slp;    \
	    mlib_s32 rank = (n + 1) / 2;                                 \
	                                                                 \
	    for (k = 0; k < nchan; k++) {                                \
		if (!(cmask & (1 << (nchan - 1 - k))))                   \
		    continue;                                            \
		ps = (ptype_##itype *) psrc + k;                          \
		pd = (ptype_##itype *) pdst + k;                          \
		for (i = 0; i < n; i++) {                                \
		    GET_DATA_##itype(pline[i], ps[0]);                   \
		    ps += slp;                                           \
		}                                                        \
		mlib_qsort_##itype(pline, n);                            \
		for (i = 0; i < size; i++) {                             \
		    pd[i * dlp] = pline[rank - 1];                       \
		    GET_DATA_##itype(data_old, ps[-n_slp]);              \
		    GET_DATA_##itype(data_new, ps[0]);                   \
		    REPLACE_ELEM(itype, pline, n, data_old, data_new);   \
		    ps += slp;                                           \
		}                                                        \
	    }                                                            \
	}

/* *********************************************************** */

FUNC_QSORT(u8)
FUNC_MEDIAN_ROW_MxNXMSK(u8)

/* *********************************************************** */

FUNC_QSORT(s16)
FUNC_FIND_MEDIAN2P(s16)

FUNC_MEDIAN_ROW_MxNPLUS(s16)
FUNC_MEDIAN_COL_MxNPLUS(s16)

FUNC_MEDIAN_ROW_MxNXMSK(s16)
FUNC_MEDIAN_COL_MxNXMSK(s16)

FUNC_MEDIAN_INITROW_MxNRSEP(s16)
FUNC_MEDIAN_ROW_MxNRSEP(s16)
FUNC_MEDIAN_COL_MxNRSEP(s16)

FUNC_MEDIAN_SEPROW(s16)
FUNC_MEDIAN_SEPCOL(s16)

/* *********************************************************** */

FUNC_QSORT(u16)
FUNC_FIND_MEDIAN2P(u16)

FUNC_MEDIAN_ROW_MxNPLUS(u16)
FUNC_MEDIAN_COL_MxNPLUS(u16)

FUNC_MEDIAN_ROW_MxNXMSK(u16)
FUNC_MEDIAN_COL_MxNXMSK(u16)

FUNC_MEDIAN_INITROW_MxNRSEP(u16)
FUNC_MEDIAN_ROW_MxNRSEP(u16)
FUNC_MEDIAN_COL_MxNRSEP(u16)

FUNC_MEDIAN_SEPROW(u16)
FUNC_MEDIAN_SEPCOL(u16)

/* *********************************************************** */

FUNC_QSORT(s32)
FUNC_FIND_MEDIAN2P(s32)

FUNC_MEDIAN_ROW_MxNRECT(s32)
FUNC_MEDIAN_COL_MxNRECT(s32)

FUNC_MEDIAN_ROW_MxNPLUS(s32)
FUNC_MEDIAN_COL_MxNPLUS(s32)

FUNC_MEDIAN_ROW_MxNXMSK(s32)
FUNC_MEDIAN_COL_MxNXMSK(s32)

FUNC_MEDIAN_INITROW_MxNRSEP(s32)
FUNC_MEDIAN_ROW_MxNRSEP(s32)
FUNC_MEDIAN_COL_MxNRSEP(s32)

FUNC_MEDIAN_SEPROW(s32)
FUNC_MEDIAN_SEPCOL(s32)

/* *********************************************************** */

FUNC_QSORT(f32)
FUNC_FIND_MEDIAN2P(f32)

FUNC_MEDIAN_ROW_MxNRECT(f32)
FUNC_MEDIAN_COL_MxNRECT(f32)

FUNC_MEDIAN_ROW_MxNPLUS(f32)
FUNC_MEDIAN_COL_MxNPLUS(f32)

FUNC_MEDIAN_ROW_MxNXMSK(f32)
FUNC_MEDIAN_COL_MxNXMSK(f32)

FUNC_MEDIAN_INITROW_MxNRSEP(f32)
FUNC_MEDIAN_ROW_MxNRSEP(f32)
FUNC_MEDIAN_COL_MxNRSEP(f32)

FUNC_MEDIAN_SEPROW(f32)
FUNC_MEDIAN_SEPCOL(f32)

/* *********************************************************** */

FUNC_QSORT(d64)
FUNC_FIND_MEDIAN2P(d64)

FUNC_MEDIAN_ROW_MxNRECT(d64)
FUNC_MEDIAN_COL_MxNRECT(d64)

FUNC_MEDIAN_ROW_MxNPLUS(d64)
FUNC_MEDIAN_COL_MxNPLUS(d64)

FUNC_MEDIAN_ROW_MxNXMSK(d64)
FUNC_MEDIAN_COL_MxNXMSK(d64)

FUNC_MEDIAN_INITROW_MxNRSEP(d64)
FUNC_MEDIAN_ROW_MxNRSEP(d64)
FUNC_MEDIAN_COL_MxNRSEP(d64)

FUNC_MEDIAN_SEPROW(d64)
FUNC_MEDIAN_SEPCOL(d64)

/* *********************************************************** */

static const mlib_median_fun_type median_row_MxN_funs[] = {
	mlib_median_row_MxNrect_u8,
	mlib_median_row_MxNplus_u8,
	mlib_median_row_MxNxmsk_u8,
	mlib_median_row_MxNrsep_u8,
	mlib_median_row_MxNrect_s16,
	mlib_median_row_MxNplus_s16,
	mlib_median_row_MxNxmsk_s16,
	mlib_median_row_MxNrsep_s16,
	mlib_median_row_MxNrect_u16,
	mlib_median_row_MxNplus_u16,
	mlib_median_row_MxNxmsk_u16,
	mlib_median_row_MxNrsep_u16,
	mlib_median_row_MxNrect_s32,
	mlib_median_row_MxNplus_s32,
	mlib_median_row_MxNxmsk_s32,
	mlib_median_row_MxNrsep_s32,
	mlib_median_row_MxNrect_f32,
	mlib_median_row_MxNplus_f32,
	mlib_median_row_MxNxmsk_f32,
	mlib_median_row_MxNrsep_f32,
	mlib_median_row_MxNrect_d64,
	mlib_median_row_MxNplus_d64,
	mlib_median_row_MxNxmsk_d64,
	mlib_median_row_MxNrsep_d64
};

static const mlib_median_fun_type median_col_MxN_funs[] = {
	mlib_median_col_MxNrect_u8,
	mlib_median_col_MxNplus_u8,
	mlib_median_col_MxNxmsk_u8,
	mlib_median_col_MxNrsep_u8,
	mlib_median_col_MxNrect_s16,
	mlib_median_col_MxNplus_s16,
	mlib_median_col_MxNxmsk_s16,
	mlib_median_col_MxNrsep_s16,
	mlib_median_col_MxNrect_u16,
	mlib_median_col_MxNplus_u16,
	mlib_median_col_MxNxmsk_u16,
	mlib_median_col_MxNrsep_u16,
	mlib_median_col_MxNrect_s32,
	mlib_median_col_MxNplus_s32,
	mlib_median_col_MxNxmsk_s32,
	mlib_median_col_MxNrsep_s32,
	mlib_median_col_MxNrect_f32,
	mlib_median_col_MxNplus_f32,
	mlib_median_col_MxNxmsk_f32,
	mlib_median_col_MxNrsep_f32,
	mlib_median_col_MxNrect_d64,
	mlib_median_col_MxNplus_d64,
	mlib_median_col_MxNxmsk_d64,
	mlib_median_col_MxNrsep_d64
};

static const mlib_median_fun_type median_init_MxN_funs[] = {
	mlib_median_initrow_MxNrsep_u8,
	mlib_median_initrow_MxNrsep_s16,
	mlib_median_initrow_MxNrsep_u16,
	mlib_median_initrow_MxNrsep_s32,
	mlib_median_initrow_MxNrsep_f32,
	mlib_median_initrow_MxNrsep_d64
};

/* *********************************************************** */

const mlib_median_sep_fun_type mlib_median_seprow_MxN_funs[] = {
	mlib_median_seprow_MxN_u8,
	mlib_median_seprow_MxN_s16,
	mlib_median_seprow_MxN_u16,
	mlib_median_seprow_MxN_s32,
	mlib_median_seprow_MxN_f32,
	mlib_median_seprow_MxN_d64
};

const mlib_median_sep_fun_type mlib_median_sepcol_MxN_funs[] = {
	mlib_median_sepcol_MxN_u8,
	mlib_median_sepcol_MxN_s16,
	mlib_median_sepcol_MxN_u16,
	mlib_median_sepcol_MxN_s32,
	mlib_median_sepcol_MxN_f32,
	mlib_median_sepcol_MxN_d64
};

/* *********************************************************** */

mlib_status
mlib_ImageMedianFilterMxN_alltypes(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 m,
    mlib_s32 n,
    mlib_median_mask mmask,
    mlib_s32 cmask,
    mlib_edge edge,
    mlib_s32 fp_version,
    mlib_s32 rank)
{
	mlib_d64 buff_MxN[BUFF_MxN_SIZE / sizeof (mlib_d64)];
	void *pbuff = NULL;
	mlib_filter_state fs;
	mlib_status res;
	mlib_type type;
	mlib_s32 i, buff_size, alloc_flag = 0;

	if (!(m & n & 1))
		return (MLIB_FAILURE);
	if (mmask == MLIB_MEDIAN_MASK_X && m != n)
		return (MLIB_FAILURE);

	fs.m = m;
	fs.n = n;
	fs.mmask = mmask;
	fs.cmask = cmask;
	fs.edge = edge;
	fs.fp_version = fp_version;
	fs.mn_version = 1;
	fs.row_funs = median_row_MxN_funs;
	fs.col_funs = median_col_MxN_funs;
	fs.init_funs = median_init_MxN_funs;
	fs.init_flag = 0;

	MLIB_IMAGE_CHECK(dst);
	type = mlib_ImageGetType(dst);

	switch (type) {
	case MLIB_BYTE:

		if (mmask != MLIB_MEDIAN_MASK_X)
			break;
/* else next case */

	case MLIB_SHORT:
	case MLIB_USHORT:

		if ((type == MLIB_SHORT || type == MLIB_USHORT) &&
		    mmask == MLIB_MEDIAN_MASK_RECT) {
			mlib_d64 *d_hist;

			buff_size = 2 * (0x10000 + 4096 + 256 + 16);

			pbuff = __mlib_malloc(buff_size);

			if (pbuff == NULL)
				return (MLIB_FAILURE);
			alloc_flag = 1;

			d_hist = (mlib_d64 *)pbuff;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < buff_size / 8; i++) {
				d_hist[i] = 0;
			}

			break;
		}

/* else next case */

	case MLIB_INT:
	case MLIB_FLOAT:
	case MLIB_DOUBLE:

		if (mmask == MLIB_MEDIAN_MASK_RECT) {
			mlib_s32 mn = m * n;

			buff_size = 4 * 2 * mn;
			fs.mn = mn;

			if (rank > 0) {
				fs.rank = rank;
			} else {
				fs.rank = (mn + 1) / 2;
			}

		} else {
			buff_size = 4 * 2 * (m + n);
		}

		if (type == MLIB_DOUBLE)
			buff_size *= 2;

		if (buff_size > BUFF_MxN_SIZE) {
			pbuff = __mlib_malloc(buff_size);

			if (pbuff == NULL)
				return (MLIB_FAILURE);
			alloc_flag = 1;
		} else {
			pbuff = buff_MxN;
		}

		break;
	default:
		return (MLIB_FAILURE);
	}

	fs.pbuff_MxN = pbuff;

	res = mlib_ImageMedianFilter_fun(dst, src, &fs);

	if (alloc_flag)
		__mlib_free(pbuff);

	return (res);
}

/* *********************************************************** */

mlib_status
__mlib_ImageMedianFilterMxN(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 m,
    mlib_s32 n,
    mlib_median_mask mmask,
    mlib_s32 cmask,
    mlib_edge edge)
{
	return mlib_ImageMedianFilterMxN_alltypes(dst, src, m, n, mmask, cmask,
	    edge, 0, -1);
}

/* *********************************************************** */
