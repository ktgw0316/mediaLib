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

#pragma ident	"@(#)mlib_ImageMedianFilterMxNFunc.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *   Internal functions for mlib_ImageMedianFilterMxN, based on histogram method
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageMedianFilter.h>

/* *********************************************************** */

#define	DEF_HIST_u8                                             \
	mlib_d64 hist_buff[64];                                 \
	mlib_u16 *phist = (mlib_u16 *)hist_buff;                \
	mlib_s32 nchan = fs->nchan, cmask = fs->cmask;          \
	mlib_s32 size = fs->size, data, count, index;           \
	mlib_s32 i, k

/* *********************************************************** */

#define	INIT_HIST_u8                                            \
	for (i = 0; i < 64; i++)                                \
	    hist_buff[i] = 0;                                   \
	index = 128;                                            \
	count = 0

/* *********************************************************** */

#define	ADD_DATA_u8(x)                                          \
	data = x;                                               \
	phist[data]++;                                          \
	count -= (data - index) >> 31

/* *********************************************************** */

#define	DEL_DATA_u8(x)                                          \
	data = x;                                               \
	phist[data]--;                                          \
	count += (data - index) >> 31

/* *********************************************************** */

#define	CLR_DATA_u8(x)	data = x; phist[data] = 0

/* *********************************************************** */

#define	FIND_MEDIAN_u8(res)                                     \
	if (count < rank) {                                     \
	    for (; count < rank; index++)                       \
		count += phist[index];                          \
	} else {                                                \
	    for (; count >= rank; index--)                      \
		count -= phist[index - 1];                      \
	    count += phist[index++];                            \
	}                                                       \
	                                                        \
	res = index - 1

/* *********************************************************** */

#define	DEF_HIST_s16                                            \
	mlib_u16 *phist = fs->pbuff_MxN;                        \
	mlib_u16 *phist1 = phist + 0x10000;                     \
	mlib_u16 *phist2 = phist1 + 1024;                       \
	mlib_s32 nchan = fs->nchan, cmask = fs->cmask;          \
	mlib_s32 size = fs->size, data, count, index;           \
	mlib_s32 i, k

/* *********************************************************** */

#define	INIT_HIST_s16

/* *********************************************************** */

#define	ADD_DATA_s16(x)                                         \
	data = 32768 + x;                                       \
	phist2[data >> 11]++;                                   \
	phist1[data >> 6]++;                                    \
	phist[data]++

/* *********************************************************** */

#define	DEL_DATA_s16(x)                                         \
	data = 32768 + x;                                       \
	phist2[data >> 11]--;                                   \
	phist1[data >> 6]--;                                    \
	phist[data]--

/* *********************************************************** */

#define	CLR_DATA_s16(x)                                         \
	data = 32768 + x;                                       \
	phist2[data >> 11] = 0;                                 \
	phist1[data >> 6] = 0;                                  \
	phist[data] = 0

/* *********************************************************** */

#define	FIND_MEDIAN_s16(res)                                    \
	for (count = index = 0; count < rank; index++)          \
	    count += phist2[index];                             \
	count -= phist2[--index];                               \
	index = (index << 5);                                   \
	for (; count < rank; index++)                           \
	    count += phist1[index];                             \
	count -= phist1[--index];                               \
	index = (index << 6);                                   \
	for (; count < rank; index++)                           \
	    count += phist[index];                              \
	res = index - (1 + 32768)

/* *********************************************************** */

#define	DEF_HIST_u16                                            \
	mlib_u16 *phist = fs->pbuff_MxN;                        \
	mlib_u16 *phist1 = phist + 0x10000;                     \
	mlib_u16 *phist2 = phist1 + 1024;                       \
	mlib_s32 nchan = fs->nchan, cmask = fs->cmask;          \
	mlib_s32 size = fs->size, data, count, index;           \
	mlib_s32 i, k

/* *********************************************************** */

#define	INIT_HIST_u16

/* *********************************************************** */

#define	ADD_DATA_u16(x)                                         \
	data = x;                                               \
	phist2[data >> 11]++;                                   \
	phist1[data >> 6]++;                                    \
	phist[data]++

/* *********************************************************** */

#define	DEL_DATA_u16(x)                                         \
	data = x;                                               \
	phist2[data >> 11]--;                                   \
	phist1[data >> 6]--;                                    \
	phist[data]--

/* *********************************************************** */

#define	CLR_DATA_u16(x)                                         \
	data = x;                                               \
	phist2[data >> 11] = 0;                                 \
	phist1[data >> 6] = 0;                                  \
	phist[data] = 0

/* *********************************************************** */

#define	FIND_MEDIAN_u16(res)                                    \
	for (count = index = 0; count < rank; index++)          \
	    count += phist2[index];                             \
	count -= phist2[--index];                               \
	index = (index << 5);                                   \
	for (; count < rank; index++)                           \
	    count += phist1[index];                             \
	count -= phist1[--index];                               \
	index = (index << 6);                                   \
	for (; count < rank; index++)                           \
	    count += phist[index];                              \
	res = index - 1

/* *********************************************************** */

#define	CLEAR_HIST_RECT_u8

/* *********************************************************** */

#define	CLEAR_HIST_RECT_s16                                     \
	for (i = 0; i < n; i++) {                               \
	    ps = psl[i] + x_off;                                \
	    for (j = 0; j < m; j++) {                           \
		CLR_DATA_s16(ps[0]);                            \
		ps -= nchan;                                    \
	    }                                                   \
	}

/* *********************************************************** */

#define	CLEAR_HIST_RECT_u16                                     \
	for (i = 0; i < n; i++) {                               \
	    ps = psl[i] + x_off;                                \
	    for (j = 0; j < m; j++) {                           \
		CLR_DATA_u16(ps[0]);                            \
		ps -= nchan;                                    \
	    }                                                   \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_ROW_MxNRECT(itype)                                \
	void                                                          \
	mlib_median_row_MxNrect_##itype(mlib_u8 *pdst, void **_psl,   \
	    mlib_filter_state *fs)                                    \
	{                                                             \
	    DEF_HIST_##itype;                                         \
	    mlib_s32 m = fs->m, j;                                    \
	    mlib_s32 n = fs->n;                                       \
	    mlib_##itype *ps, *pd;                                    \
	    mlib_##itype **psl = (mlib_##itype **) _psl;              \
	    mlib_s32 x_off, rank = (m * n + 1) / 2, kw1 = fs->kw1;    \
	    mlib_s32 m_nchan = fs->m_nchan;                           \
	                                                              \
	    for (k = 0; k < nchan; k++) {                             \
		if (!(cmask & (1 << (nchan - 1 - k))))                \
		    continue;                                         \
		pd = (mlib_##itype *) pdst + k;                       \
		INIT_HIST_##itype;                                    \
		x_off = kw1 * nchan;                                  \
		for (i = 0; i < n; i++) {                             \
		    ps = psl[i] - x_off + k;                          \
		    for (j = 0; j < m; j++) {                         \
			ADD_DATA_##itype(ps[0]);                      \
			ps += nchan;                                  \
		    }                                                 \
		}                                                     \
		x_off += k;                                           \
		FIND_MEDIAN_##itype(pd[0]);                           \
		for (j = 0; j < size - 1; j++) {                      \
		    pd += nchan;                                      \
		    x_off += nchan;                                   \
		    for (i = 0; i < n; i++) {                         \
			ps = psl[i] + x_off;                          \
			DEL_DATA_##itype(ps[-m_nchan]);               \
			ADD_DATA_##itype(ps[0]);                      \
		    }                                                 \
		    FIND_MEDIAN_##itype(pd[0]);                       \
		}                                                     \
	    CLEAR_HIST_RECT_##itype}                                  \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_ROW_MxNPLUS(itype)                                 \
	void                                                           \
	mlib_median_row_MxNplus_##itype(mlib_u8 *pdst, void **_psl,    \
	    mlib_filter_state *fs)                                     \
	{                                                              \
	    DEF_HIST_##itype;                                          \
	    mlib_##itype *ps, *pd;                                     \
	    mlib_s32 n = fs->n;                                        \
	    mlib_s32 m = fs->m, j;                                     \
	    mlib_##itype **psl = (mlib_##itype **) _psl;               \
	    mlib_##itype *dl = (mlib_##itype *) pdst;                  \
	    mlib_##itype *ps0;                                         \
	    mlib_s32 x_off, rank = (m + n) / 2, kh1 = fs->kh1, kw1 =   \
		fs->kw1;                                               \
	    mlib_s32 m_nchan = fs->m_nchan;                            \
	                                                               \
	    for (k = 0; k < nchan; k++) {                              \
		if (!(cmask & (1 << (nchan - 1 - k))))                 \
		    continue;                                          \
		pd = dl + k;                                           \
		INIT_HIST_##itype;                                     \
		ps0 = psl[kh1] - kw1 * nchan + k;                      \
		for (j = 0; j < m; j++) {                              \
		    ADD_DATA_##itype(ps0[0]);                          \
		    ps0 += nchan;                                      \
		}                                                      \
		for (i = 0; i < n; i++) {                              \
		    ADD_DATA_##itype(psl[i][k]);                       \
		}                                                      \
		DEL_DATA_##itype(psl[kh1][k]);                         \
		x_off = k;                                             \
		FIND_MEDIAN_##itype(pd[0]);                            \
		pd += nchan;                                           \
		for (j = 1; j < size; j++) {                           \
		    DEL_DATA_##itype(ps0[-m_nchan]);                   \
		    ADD_DATA_##itype(ps0[0]);                          \
		    for (i = 0; i < kh1; i++) {                        \
			ps = psl[i] + x_off;                           \
			DEL_DATA_##itype(ps[0]);                       \
			ADD_DATA_##itype(ps[nchan]);                   \
		    }                                                  \
		    for (i = kh1 + 1; i < n; i++) {                    \
			ps = psl[i] + x_off;                           \
			DEL_DATA_##itype(ps[0]);                       \
			ADD_DATA_##itype(ps[nchan]);                   \
		    }                                                  \
		    FIND_MEDIAN_##itype(pd[0]);                        \
		    pd += nchan;                                       \
		    ps0 += nchan;                                      \
		    x_off += nchan;                                    \
		}                                                      \
	    }                                                          \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_ROW_MxNXMSK(itype)                                \
	void                                                          \
	mlib_median_row_MxNxmsk_##itype(mlib_u8 *pdst, void **_psl,   \
	    mlib_filter_state *fs)                                    \
	{                                                             \
	    DEF_HIST_##itype;                                         \
	    mlib_s32 n = fs->n;                                       \
	    mlib_##itype *ps, *pd;                                    \
	    mlib_s32 rank = (m + n) / 2, j;                           \
	    mlib_s32 x_off, off0, off1, k1_nchan = kw1 * nchan;       \
	                                                              \
	    INIT_HIST_##itype;                                        \
	    for (k = 0; k < nchan; k++) {                             \
		if (!(cmask & (1 << (nchan - 1 - k))))                \
		    continue;                                         \
		pd = dl + k;                                          \
		x_off = k;                                            \
		for (j = 0; j < size; j++) {                          \
		    index = 128;                                      \
		    count = 0;                                        \
		    off0 = x_off - k1_nchan;                          \
		    off1 = x_off + k1_nchan;                          \
		    for (i = 0; i < n; i++) {                         \
			ps = psl[i];                                  \
			ADD_DATA_##itype(ps[off0]);                   \
			ADD_DATA_##itype(ps[off1]);                   \
			off0 += nchan;                                \
			off1 -= nchan;                                \
		    }                                                 \
		    DEL_DATA_##itype(psl[kw1][x_off]);                \
		    FIND_MEDIAN_##itype(pd[0]);                       \
		    off0 = x_off - k1_nchan;                          \
		    off1 = x_off + k1_nchan;                          \
		    for (i = 0; i < n; i++) {                         \
			ps = psl[i];                                  \
			CLR_DATA_##itype(ps[off0]);                   \
			CLR_DATA_##itype(ps[off1]);                   \
			off0 += nchan;                                \
			off1 -= nchan;                                \
		    }                                                 \
		    pd += nchan;                                      \
		    x_off += nchan;                                   \
		}                                                     \
	    }                                                         \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_COL_MxNRECT(itype)                                \
	void                                                          \
	mlib_median_col_MxNrect_##itype(mlib_u8 *pdst, void **_psl,   \
	    mlib_filter_state *fs)                                    \
	{                                                             \
	    DEF_HIST_##itype;                                         \
	    mlib_s32 n = fs->n;                                       \
	    mlib_##itype *ps, *pd;                                    \
	    mlib_s32 m = fs->m, j;                                    \
	    mlib_##itype **psl = (mlib_##itype **) _psl;              \
	    mlib_##itype *dl = (mlib_##itype *) pdst;                 \
	    mlib_s32 *x_off = fs->x_off;                              \
	    mlib_s32 l, rank = (m * n + 1) / 2, dlb = fs->dlp;        \
	                                                              \
	    for (k = 0; k < nchan; k++) {                             \
		if (!(cmask & (1 << (nchan - 1 - k))))                \
		    continue;                                         \
		pd = dl + k;                                          \
		INIT_HIST_##itype;                                    \
		for (i = 0; i < n - 1; i++) {                         \
		    ps = psl[i] + k;                                  \
		    for (j = 0; j < m; j++) {                         \
			ADD_DATA_##itype(ps[x_off[j]]);               \
		    }                                                 \
		}                                                     \
		for (i = 0; i < size; i++) {                          \
		    ps = psl[i + (n - 1)] + k;                        \
		    for (j = 0; j < m; j++) {                         \
			ADD_DATA_##itype(ps[x_off[j]]);               \
		    }                                                 \
		    FIND_MEDIAN_##itype(pd[0]);                       \
		    pd += dlb;                                        \
		    ps = psl[i] + k;                                  \
		    for (j = 0; j < m; j++) {                         \
			DEL_DATA_##itype(ps[x_off[j]]);               \
		    }                                                 \
		}                                                     \
	                                                              \
/*                                                                    \
 * clear histogram                                                    \
 */                                                                   \
		for (l = 0; l < n - 1; l++) {                         \
		    ps = psl[i + l] + k;                              \
		    for (j = 0; j < m; j++) {                         \
			CLR_DATA_##itype(ps[x_off[j]]);               \
		    }                                                 \
		}                                                     \
	    }                                                         \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_COL_MxNPLUS(itype)                                  \
	void                                                            \
	mlib_median_col_MxNplus_##itype(mlib_u8 *pdst, void **_psl,     \
	    mlib_filter_state *fs)                                      \
	{                                                               \
	    DEF_HIST_##itype;                                           \
	    mlib_s32 n = fs->n;                                         \
	    mlib_##itype *ps, *pd;                                      \
	    mlib_s32 m = fs->m, j;                                      \
	    mlib_##itype **psl = (mlib_##itype **) _psl;                \
	    mlib_##itype *dl = (mlib_##itype *) pdst;                   \
	    mlib_s32 *x_off = fs->x_off;                                \
	    mlib_s32 x_off0, rank = (m + n) / 2, kh1 = fs->kh1, kw1 =   \
		fs->kw1, dlb = fs->dlp;                                 \
	    for (k = 0; k < nchan; k++) {                               \
		if (!(cmask & (1 << (nchan - 1 - k))))                  \
		    continue;                                           \
		pd = dl + k;                                            \
		INIT_HIST_##itype;                                      \
		ps = psl[kh1] + k;                                      \
		for (j = 0; j < m; j++) {                               \
		    ADD_DATA_##itype(ps[x_off[j]]);                     \
		}                                                       \
		x_off0 = x_off[kw1];                                    \
		for (i = 0; i < n; i++) {                               \
		    ADD_DATA_##itype(psl[i][x_off0 + k]);               \
		}                                                       \
		DEL_DATA_##itype(psl[kh1][x_off0 + k]);                 \
		for (i = 0; i < size; i++) {                            \
		    FIND_MEDIAN_##itype(pd[0]);                         \
		    pd += dlb;                                          \
		    DEL_DATA_##itype(psl[i][x_off0 + k]);               \
		    ADD_DATA_##itype(psl[i + n][x_off0 + k]);           \
		    ps = psl[i + kh1] + k;                              \
		    for (j = 0; j < m; j++) {                           \
			DEL_DATA_##itype(ps[x_off[j]]);                 \
		    }                                                   \
		    ADD_DATA_##itype(ps[x_off0]);                       \
		    ps = psl[i + kh1 + 1] + k;                          \
		    for (j = 0; j < m; j++) {                           \
			ADD_DATA_##itype(ps[x_off[j]]);                 \
		    }                                                   \
		    DEL_DATA_##itype(ps[x_off0]);                       \
		}                                                       \
	    }                                                           \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_COL_MxNXMSK(itype)                                \
	void                                                          \
	mlib_median_col_MxNxmsk_##itype(mlib_u8 *pdst, void **_psl,   \
	    mlib_filter_state *fs)                                    \
	{                                                             \
	    DEF_HIST_##itype;                                         \
	    mlib_s32 n = fs->n;                                       \
	    mlib_##itype *ps, *pd;                                    \
	    mlib_s32 m = fs->m, j;                                    \
	    mlib_##itype **psl = (mlib_##itype **) _psl;              \
	    mlib_##itype *dl = (mlib_##itype *) pdst;                 \
	    mlib_s32 *x_off = fs->x_off, dlb = fs->dlp;               \
	    mlib_s32 rank = (m + n) / 2, kw1 = fs->kw1;               \
	                                                              \
	    INIT_HIST_##itype;                                        \
	    for (k = 0; k < nchan; k++) {                             \
		if (!(cmask & (1 << (nchan - 1 - k))))                \
		    continue;                                         \
		pd = dl + k;                                          \
		for (i = 0; i < size; i++) {                          \
		    index = 128;                                      \
		    count = 0;                                        \
		    for (j = 0; j < n; j++) {                         \
			ps = psl[i + j] + k;                          \
			ADD_DATA_##itype(ps[x_off[j]]);               \
			ADD_DATA_##itype(ps[x_off[(n - 1) - j]]);     \
		    }                                                 \
		    DEL_DATA_##itype(psl[i + kw1][x_off[kw1] + k]);   \
		    FIND_MEDIAN_##itype(pd[0]);                       \
		    pd += dlb;                                        \
		    for (j = 0; j < n; j++) {                         \
			ps = psl[i + j] + k;                          \
			CLR_DATA_##itype(ps[x_off[j]]);               \
			CLR_DATA_##itype(ps[x_off[(n - 1) - j]]);     \
		    }                                                 \
		}                                                     \
	    }                                                         \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_INITROW_MxNRSEP(itype)                                 \
	void                                                               \
	mlib_median_initrow_MxNrsep_##itype(mlib_u8 *pdst,                 \
	    void **_psl, mlib_filter_state *fs)                            \
	{                                                                  \
	    DEF_HIST_##itype;                                              \
	    mlib_s32 n = fs->n;                                            \
	    mlib_##itype *ps;                                              \
	    mlib_s32 m = fs->m, j;                                         \
	    mlib_##itype **psl = (mlib_##itype **) _psl;                   \
	    mlib_s32 *buff, rank = (m + 1) / 2, m1_nchan = fs->m1_nchan;   \
	    mlib_s32 m_nchan = fs->m_nchan;                                \
	                                                                   \
	    for (k = 0; k < nchan; k++) {                                  \
		if (!(cmask & (1 << (nchan - 1 - k))))                     \
		    continue;                                              \
		ps = psl[0] - m1_nchan + k;                                \
		buff = (mlib_s32 *)(fs->buff_row)[fs->buff_ind] + k;       \
		INIT_HIST_##itype;                                         \
		for (j = 0; j < m; j++) {                                  \
		    ADD_DATA_##itype(ps[0]);                               \
		    ps += nchan;                                           \
		}                                                          \
		for (j = 0; j < size; j++) {                               \
		    FIND_MEDIAN_##itype(buff[j * nchan]);                  \
		    DEL_DATA_##itype(ps[-m_nchan]);                        \
		    ADD_DATA_##itype(ps[0]);                               \
		    ps += nchan;                                           \
		}                                                          \
	    }                                                              \
	    if (++(fs->buff_ind) >= n)                                     \
		(fs->buff_ind) -= n;                                       \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_ROW_MxNRSEP(itype)                                \
	void                                                          \
	mlib_median_row_MxNrsep_##itype(mlib_u8 *pdst, void **_psl,   \
	    mlib_filter_state *fs)                                    \
	{                                                             \
	    DEF_HIST_##itype;                                         \
	    mlib_s32 n = fs->n;                                       \
	    mlib_##itype *pd;                                         \
	    mlib_##itype *dl = (mlib_##itype *) pdst;                 \
	    mlib_s32 *buff0, *buff, j;                                \
	    mlib_s32 buff_size = fs->buff_size, rank = (n + 1) / 2;   \
	                                                              \
	    mlib_median_initrow_MxNrsep_##itype(pdst,                 \
		_psl + (n - 1), fs);                                  \
	    INIT_HIST_##itype;                                        \
	    for (k = 0; k < nchan; k++) {                             \
		if (!(cmask & (1 << (nchan - 1 - k))))                \
		    continue;                                         \
		buff0 = (mlib_s32 *)(fs->buff_n_rows) + k;            \
		pd = dl + k;                                          \
		for (j = 0; j < size; j++) {                          \
		    index = 128;                                      \
		    count = 0;                                        \
		    buff = buff0;                                     \
		    for (i = 0; i < n; i++) {                         \
			ADD_DATA_##itype(buff[0]);                    \
			buff += buff_size;                            \
		    }                                                 \
		    FIND_MEDIAN_##itype(pd[0]);                       \
		    buff = buff0;                                     \
		    for (i = 0; i < n; i++) {                         \
			CLR_DATA_##itype(buff[0]);                    \
			buff += buff_size;                            \
		    }                                                 \
		    buff0 += nchan;                                   \
		    pd += nchan;                                      \
		}                                                     \
	    }                                                         \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_COL_MxNRSEP(itype)                                \
	void                                                          \
	mlib_median_col_MxNrsep_##itype(mlib_u8 *pdst, void **_psl,   \
	    mlib_filter_state *fs)                                    \
	{                                                             \
	    DEF_HIST_##itype;                                         \
	    mlib_s32 n = fs->n;                                       \
	    mlib_##itype *ps, *pd;                                    \
	    mlib_s32 m = fs->m, j;                                    \
	    mlib_##itype **psl = (mlib_##itype **) _psl;              \
	    mlib_##itype *dl = (mlib_##itype *) pdst;                 \
	    mlib_s32 *x_off = fs->x_off;                              \
	    mlib_s32 *buff, rank, dlb = fs->dlp;                      \
	                                                              \
	    INIT_HIST_##itype;                                        \
	    for (k = 0; k < nchan; k++) {                             \
		if (!(cmask & (1 << (nchan - 1 - k))))                \
		    continue;                                         \
		buff = fs->buff_n_rows;                               \
/*                                                                    \
 * process row medians                                                \
 */                                                                   \
		rank = (m + 1) / 2;                                   \
		for (i = 0; i < size + n; i++) {                      \
		    index = 128;                                      \
		    count = 0;                                        \
		    ps = psl[i] + k;                                  \
		    for (j = 0; j < m; j++) {                         \
			ADD_DATA_##itype(ps[x_off[j]]);               \
		    }                                                 \
		    FIND_MEDIAN_##itype(buff[i]);                     \
		    for (j = 0; j < m; j++) {                         \
			CLR_DATA_##itype(ps[x_off[j]]);               \
		    }                                                 \
		}                                                     \
	                                                              \
/*                                                                    \
 * process col medians                                                \
 */                                                                   \
		rank = (n + 1) / 2;                                   \
		index = 128;                                          \
		count = 0;                                            \
		for (i = 0; i < n; i++) {                             \
		    ADD_DATA_##itype(buff[0]);                        \
		    buff++;                                           \
		}                                                     \
		pd = (void *)dl;                                      \
		pd += k;                                              \
		for (i = 0; i < size; i++) {                          \
		    FIND_MEDIAN_##itype(pd[0]);                       \
		    pd += dlb;                                        \
		    DEL_DATA_##itype(buff[-n]);                       \
		    ADD_DATA_##itype(buff[0]);                        \
		    buff++;                                           \
		}                                                     \
		for (i = 0; i < n; i++) {                             \
		    buff--;                                           \
		    CLR_DATA_##itype(buff[0]);                        \
		}                                                     \
	    }                                                         \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_SEPROW(itype)                                      \
	void                                                           \
	mlib_median_seprow_MxN_##itype(mlib_u8 *pdst, mlib_u8 *psrc,   \
	    mlib_filter_state *fs)                                     \
	{                                                              \
	    DEF_HIST_##itype;                                          \
	    mlib_##itype *ps, *pd;                                     \
	    mlib_s32 m = fs->m, j;                                     \
	    mlib_s32 rank = (m + 1) / 2, m1_nchan = fs->m1_nchan;      \
	    mlib_s32 m_nchan = fs->m_nchan;                            \
	                                                               \
	    INIT_HIST_##itype;                                         \
	    for (k = 0; k < nchan; k++) {                              \
		if (!(cmask & (1 << (nchan - 1 - k))))                 \
		    continue;                                          \
		ps = (mlib_##itype *) psrc - m1_nchan + k;             \
		pd = (mlib_##itype *) pdst + k;                        \
		index = 128;                                           \
		count = 0;                                             \
		for (j = 0; j < m; j++) {                              \
		    ADD_DATA_##itype(ps[0]);                           \
		    ps += nchan;                                       \
		}                                                      \
		for (j = 0; j < (size - 1); j++) {                     \
		    FIND_MEDIAN_##itype(pd[j * nchan]);                \
		    DEL_DATA_##itype(ps[-m_nchan]);                    \
		    ADD_DATA_##itype(ps[0]);                           \
		    ps += nchan;                                       \
		}                                                      \
		FIND_MEDIAN_##itype(pd[(size - 1) * nchan]);           \
		for (j = 0; j < m; j++) {                              \
		    ps -= nchan;                                       \
		    CLR_DATA_##itype(ps[0]);                           \
		}                                                      \
	    }                                                          \
	}

/* *********************************************************** */

#define	FUNC_MEDIAN_SEPCOL(itype)                                       \
	void                                                            \
	mlib_median_sepcol_MxN_##itype(mlib_u8 *pdst, mlib_u8 *psrc,    \
	    mlib_filter_state *fs)                                      \
	{                                                               \
	    DEF_HIST_##itype;                                           \
	    mlib_s32 n = fs->n;                                         \
	    mlib_##itype *ps, *pd;                                      \
	    mlib_s32 slp = fs->wsize, dlp = fs->dlp, n_slp = n * slp;   \
	    mlib_s32 rank = (n + 1) / 2;                                \
	                                                                \
	    INIT_HIST_##itype;                                          \
	    for (k = 0; k < nchan; k++) {                               \
		if (!(cmask & (1 << (nchan - 1 - k))))                  \
		    continue;                                           \
		ps = (mlib_##itype *) psrc + k;                         \
		pd = (mlib_##itype *) pdst + k;                         \
		index = 128;                                            \
		count = 0;                                              \
		for (i = 0; i < n; i++) {                               \
		    ADD_DATA_##itype(ps[0]);                            \
		    ps += slp;                                          \
		}                                                       \
		for (i = 0; i < (size - 1); i++) {                      \
		    FIND_MEDIAN_##itype(pd[i * dlp]);                   \
		    DEL_DATA_##itype(ps[-n_slp]);                       \
		    ADD_DATA_##itype(ps[0]);                            \
		    ps += slp;                                          \
		}                                                       \
		FIND_MEDIAN_##itype(pd[(size - 1) * dlp]);              \
		for (i = 0; i < n; i++) {                               \
		    ps -= slp;                                          \
		    CLR_DATA_##itype(ps[0]);                            \
		}                                                       \
	    }                                                           \
	}

/* *********************************************************** */

FUNC_MEDIAN_ROW_MxNRECT(u8)
FUNC_MEDIAN_COL_MxNRECT(u8)

FUNC_MEDIAN_ROW_MxNPLUS(u8)
FUNC_MEDIAN_COL_MxNPLUS(u8)

FUNC_MEDIAN_COL_MxNXMSK(u8)

FUNC_MEDIAN_INITROW_MxNRSEP(u8)
FUNC_MEDIAN_ROW_MxNRSEP(u8)
FUNC_MEDIAN_COL_MxNRSEP(u8)

FUNC_MEDIAN_SEPROW(u8)
FUNC_MEDIAN_SEPCOL(u8)

/* *********************************************************** */

FUNC_MEDIAN_ROW_MxNRECT(s16)
FUNC_MEDIAN_COL_MxNRECT(s16)

/* *********************************************************** */

FUNC_MEDIAN_ROW_MxNRECT(u16)
FUNC_MEDIAN_COL_MxNRECT(u16)

/* *********************************************************** */
