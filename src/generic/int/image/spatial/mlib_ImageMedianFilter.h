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

#ifndef __MLIB_IMAGEMEDIANFILTER_H
#define	__MLIB_IMAGEMEDIANFILTER_H

#pragma ident	"@(#)mlib_ImageMedianFilter.h	9.3	07/11/05 SMI"

/*
 *
 * DESCRIPTION
 *    Internal macros and types for mlib_ImageMedianFilter* functions.
 *
 */

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>

#ifdef __cplusplus
extern "C" {
#endif

struct mlib_filter_state_struct
{
	mlib_median_mask mmask;
	mlib_s32 cmask;
	mlib_edge edge;
	mlib_s32 m, n, kw1, kh1, m1_nchan, m_nchan;
	mlib_s32 fp_version, mn_version;
	mlib_type type;
	mlib_s32 nchan, type_sh, bchan, xsize, ysize, wsize, bsize;
	mlib_s32 slb, dlb, slp, dlp, tlp;
	mlib_u8 *pdst;
	void **_psl, **psl, *dl;
	mlib_s32 *_x_off, *x_off;
	mlib_s32 dx_l, dx_r, dy_t, dy_b;
	mlib_s32 size;

	mlib_u8 *pbuff;
	void *pbuff_MxN;

	mlib_s32 init_flag;
	void *buff_n_rows;
	void **buff_row;
	mlib_s32 buff_ind;
	mlib_s32 buff_size;

	void (
	    *const *row_funs) (
	    mlib_u8 *pdst,
	    void **_psl,
	    struct mlib_filter_state_struct *fs);
	void (
	    *const *col_funs) (
	    mlib_u8 *pdst,
	    void **_psl,
	    struct mlib_filter_state_struct *fs);
	void (
	    *const *init_funs) (
	    mlib_u8 *pdst,
	    void **_psl,
	    struct mlib_filter_state_struct *fs);

	mlib_s32 mn;
	mlib_s32 rank;
};

typedef struct mlib_filter_state_struct mlib_filter_state;
typedef void (*mlib_median_fun_type) (
    mlib_u8 *pdst,
    void **_psl,
    mlib_filter_state *fs);
typedef void (*mlib_median_sep_fun_type) (
    mlib_u8 *pdst,
    mlib_u8 *psrc,
    mlib_filter_state *fs);

/* *********************************************************** */

#define	SORT2L(a, b)                                            \
	mask = ((b - a) & ((b - a) >> 31));                     \
	a += mask;                                              \
	b -= mask

/* *********************************************************** */

#define	S_MAXL(a, b)	mask = ((b - a) & ((b - a) >> 31)); b = b - mask

/* *********************************************************** */

#define	S_MINL(a, b)	mask = ((b - a) & ((b - a) >> 31)); a = a + mask

/* *********************************************************** */

#define	SORT2C(a, b)	{ if (a > b) { mask = a; a = b; b = mask; }}

#define	SORT2F32(a, b)	{                                \
	if ((a & 0x80000000) && (b & 0x80000000)) {      \
		if (a < b) {                             \
			mask = a; a = b; b = mask;       \
		}                                        \
	} else {                                         \
		if (a > b) {                             \
			mask = a; a = b; b = mask;       \
		}                                        \
	}                                                \
}

#define	SORT2D64(a, b)	{                                \
	if ((a & 0x8000000000000000) &&                  \
		(b & 0x8000000000000000)) {              \
		if (a < b) {                             \
			mask = a; a = b; b = mask;       \
		}                                        \
	} else {                                         \
		if (a > b) {                             \
			mask = a; a = b; b = mask;       \
		}                                        \
	}                                                \
}

/* *********************************************************** */

#define	S_MAXC(a, b)	b = (a > b) ? a : b
#define	S_MINC(a, b)	a = (a < b) ? a : b

#define	S_MAXF32(a, b)                                   \
	b = ((a & 0x80000000) && (b & 0x80000000)) ?     \
		((a < b) ? (a) : (b)) :                  \
		((a <= b) ? (b) : (a))

#define	S_MINF32(a, b)                                   \
	a = ((a & 0x80000000) && (b & 0x80000000)) ?     \
		((a > b) ? (a) : (b)) :                  \
		((a >= b) ? (b) : (a))

#define	S_MAXD64(a, b)                                   \
	b = ((a & 0x8000000000000000) &&                 \
		(b & 0x8000000000000000)) ?              \
		((a < b) ? (a) : (b)) :                  \
		((a <= b) ? (b) : (a))

#define	S_MIND64(a, b)                                   \
	a = ((a & 0x8000000000000000) &&                 \
		(b & 0x8000000000000000)) ?              \
		((a > b) ? (a) : (b)) :                  \
		((a >= b) ? (b) : (a))

/* *********************************************************** */
#define	T_MAXC(a, b)	(a > b) ? a : b
#define	T_MINC(a, b)	(a < b) ? a : b

#define	T_MAXF32(a, b)                                   \
	((a & 0x80000000) && (b & 0x80000000)) ?         \
		((a < b) ? (a) : (b)) :                  \
		((a <= b) ? (b) : (a))

#define	T_MINF32(a, b)                                   \
	((a & 0x80000000) && (b & 0x80000000)) ?         \
		((a > b) ? (a) : (b)) :                  \
		((a >= b) ? (b) : (a))

#define	T_MAXD64(a, b)                                   \
	((a & 0x8000000000000000) &&                     \
		(b & 0x8000000000000000)) ?              \
		((a < b) ? (a) : (b)) :                  \
		((a <= b) ? (b) : (a))

#define	T_MIND64(a, b)                                   \
	((a & 0x8000000000000000) &&                     \
		(b & 0x8000000000000000)) ?              \
		((a > b) ? (a) : (b)) :                  \
		((a >= b) ? (b) : (a))

/* *********************************************************** */

#define	R_MAXC(a, b)	(a > b)
#define	R_MINC(a, b)	(a < b)

#define	R_MAXF32(a, b)                                   \
	((a & 0x80000000) && (b & 0x80000000)) ?         \
		(a < b) : (a > b)

#define	R_MINF32(a, b)                                   \
	((a & 0x80000000) && (b & 0x80000000)) ?         \
		(a > b) : (a < b)

#define	R_MAXD64(a, b)                                   \
	((a & 0x8000000000000000) &&                     \
		(b & 0x8000000000000000)) ?              \
		(a < b) : (a > b)

#define	R_MIND64(a, b)                                   \
	((a & 0x8000000000000000) &&                     \
		(b & 0x8000000000000000)) ?              \
		(a > b) : (a < b)

/* *********************************************************** */

#define	R_MAXEQC(a, b)	(a >= b)
#define	R_MINEQC(a, b)	(a <= b)

#define	R_MAXEQF32(a, b)                                 \
	((a & 0x80000000) && (b & 0x80000000)) ?         \
		(a <= b) : (a >= b)

#define	R_MINEQF32(a, b)                                 \
	((a & 0x80000000) && (b & 0x80000000)) ?         \
		(a >= b) : (a <= b)

#define	R_MAXEQD64(a, b)                                 \
	((a & 0x8000000000000000) &&                     \
		(b & 0x8000000000000000)) ?              \
		(a <= b) : (a >= b)

#define	R_MINEQD64(a, b)                                 \
	((a & 0x8000000000000000) &&                     \
		(b & 0x8000000000000000)) ?              \
		(a >= b) : (a <= b)

/* *********************************************************** */
#if defined(_NO_LONGLONG)
/* *********************************************************** */
#define	SORT2_u8(a, b)	SORT2L(a, b)
#define	SORT2_s16(a, b)	SORT2L(a, b)
#define	SORT2_u16(a, b)	SORT2L(a, b)
#define	SORT2_s32(a, b)	SORT2C(a, b)
#define	SORT2_f32(a, b)	SORT2C(a, b)
#define	SORT2_d64(a, b)	SORT2C(a, b)

#define	SSORT2_u8(a, b)		SORT2C(a, b)
#define	SSORT2_s16(a, b)	SORT2C(a, b)
#define	SSORT2_u16(a, b)	SORT2C(a, b)
#define	SSORT2_s32(a, b)	SORT2C(a, b)
#define	SSORT2_f32(a, b)	SORT2C(a, b)
#define	SSORT2_d64(a, b)	SORT2C(a, b)

/* *********************************************************** */

#define	S_MIN_u8(a, b)	S_MINL(a, b)
#define	S_MIN_s16(a, b)	S_MINL(a, b)
#define	S_MIN_u16(a, b)	S_MINL(a, b)
#define	S_MIN_s32(a, b)	S_MINC(a, b)
#define	S_MIN_f32(a, b)	S_MINC(a, b)
#define	S_MIN_d64(a, b)	S_MINC(a, b)

#define	S_MAX_u8(a, b)	S_MAXL(a, b)
#define	S_MAX_s16(a, b)	S_MAXL(a, b)
#define	S_MAX_u16(a, b)	S_MAXL(a, b)
#define	S_MAX_s32(a, b)	S_MAXC(a, b)
#define	S_MAX_f32(a, b)	S_MAXC(a, b)
#define	S_MAX_d64(a, b)	S_MAXC(a, b)
/* *********************************************************** */

#define	T_MIN_u8(a, b) 	T_MINC(a, b)
#define	T_MIN_s16(a, b)	T_MINC(a, b)
#define	T_MIN_u16(a, b)	T_MINC(a, b)
#define	T_MIN_s32(a, b)	T_MINC(a, b)
#define	T_MIN_f32(a, b)	T_MINC(a, b)
#define	T_MIN_d64(a, b)	T_MINC(a, b)

#define	T_MAX_u8(a, b) 	T_MAXC(a, b)
#define	T_MAX_s16(a, b)	T_MAXC(a, b)
#define	T_MAX_u16(a, b)	T_MAXC(a, b)
#define	T_MAX_s32(a, b)	T_MAXC(a, b)
#define	T_MAX_f32(a, b)	T_MAXC(a, b)
#define	T_MAX_d64(a, b)	T_MAXC(a, b)

/* *********************************************************** */

#define	R_MIN_u8(a, b)	R_MINC(a, b)
#define	R_MIN_s16(a, b)	R_MINC(a, b)
#define	R_MIN_u16(a, b)	R_MINC(a, b)
#define	R_MIN_s32(a, b)	R_MINC(a, b)
#define	R_MIN_f32(a, b)	R_MINC(a, b)
#define	R_MIN_d64(a, b)	R_MINC(a, b)

#define	R_MAX_u8(a, b)	R_MAXC(a, b)
#define	R_MAX_s16(a, b)	R_MAXC(a, b)
#define	R_MAX_u16(a, b)	R_MAXC(a, b)
#define	R_MAX_s32(a, b)	R_MAXC(a, b)
#define	R_MAX_f32(a, b)	R_MAXC(a, b)
#define	R_MAX_d64(a, b)	R_MAXC(a, b)

/* *********************************************************** */

#define	R_MINEQ_u8(a, b)	R_MINEQC(a, b)
#define	R_MINEQ_s16(a, b)	R_MINEQC(a, b)
#define	R_MINEQ_u16(a, b)	R_MINEQC(a, b)
#define	R_MINEQ_s32(a, b)	R_MINEQC(a, b)
#define	R_MINEQ_f32(a, b)	R_MINEQC(a, b)
#define	R_MINEQ_d64(a, b)	R_MINEQC(a, b)

#define	R_MAXEQ_u8(a, b)	R_MAXEQC(a, b)
#define	R_MAXEQ_s16(a, b)	R_MAXEQC(a, b)
#define	R_MAXEQ_u16(a, b)	R_MAXEQC(a, b)
#define	R_MAXEQ_s32(a, b)	R_MAXEQC(a, b)
#define	R_MAXEQ_f32(a, b)	R_MAXEQC(a, b)
#define	R_MAXEQ_d64(a, b)	R_MAXEQC(a, b)

/* *********************************************************** */
#else /* else if _NO_LONGLONG */
/* *********************************************************** */
#define	SORT2_u8(a, b)	SORT2L(a, b)
#define	SORT2_s16(a, b)	SORT2L(a, b)
#define	SORT2_u16(a, b)	SORT2L(a, b)
#define	SORT2_s32(a, b)	SORT2C(a, b)
#define	SORT2_f32(a, b)	SORT2F32(a, b)
#define	SORT2_d64(a, b)	SORT2D64(a, b)

#define	SSORT2_u8(a, b)		SORT2C(a, b)
#define	SSORT2_s16(a, b)	SORT2C(a, b)
#define	SSORT2_u16(a, b)	SORT2C(a, b)
#define	SSORT2_s32(a, b)	SORT2C(a, b)
#define	SSORT2_f32(a, b)	SORT2F32(a, b)
#define	SSORT2_d64(a, b)	SORT2D64(a, b)

/* *********************************************************** */

#define	S_MIN_u8(a, b)	S_MINL(a, b)
#define	S_MIN_s16(a, b)	S_MINL(a, b)
#define	S_MIN_u16(a, b)	S_MINL(a, b)
#define	S_MIN_s32(a, b)	S_MINC(a, b)
#define	S_MIN_f32(a, b)	S_MINF32(a, b)
#define	S_MIN_d64(a, b)	S_MIND64(a, b)

#define	S_MAX_u8(a, b)	S_MAXL(a, b)
#define	S_MAX_s16(a, b)	S_MAXL(a, b)
#define	S_MAX_u16(a, b)	S_MAXL(a, b)
#define	S_MAX_s32(a, b)	S_MAXC(a, b)
#define	S_MAX_f32(a, b)	S_MAXF32(a, b)
#define	S_MAX_d64(a, b)	S_MAXD64(a, b)
/* *********************************************************** */

#define	T_MIN_u8(a, b) 	T_MINC(a, b)
#define	T_MIN_s16(a, b)	T_MINC(a, b)
#define	T_MIN_u16(a, b)	T_MINC(a, b)
#define	T_MIN_s32(a, b)	T_MINC(a, b)
#define	T_MIN_f32(a, b)	T_MINF32(a, b)
#define	T_MIN_d64(a, b)	T_MIND64(a, b)

#define	T_MAX_u8(a, b) 	T_MAXC(a, b)
#define	T_MAX_s16(a, b)	T_MAXC(a, b)
#define	T_MAX_u16(a, b)	T_MAXC(a, b)
#define	T_MAX_s32(a, b)	T_MAXC(a, b)
#define	T_MAX_f32(a, b)	T_MAXF32(a, b)
#define	T_MAX_d64(a, b)	T_MAXD64(a, b)

/* *********************************************************** */

#define	R_MIN_u8(a, b)	R_MINC(a, b)
#define	R_MIN_s16(a, b)	R_MINC(a, b)
#define	R_MIN_u16(a, b)	R_MINC(a, b)
#define	R_MIN_s32(a, b)	R_MINC(a, b)
#define	R_MIN_f32(a, b)	R_MINF32(a, b)
#define	R_MIN_d64(a, b)	R_MIND64(a, b)

#define	R_MAX_u8(a, b)	R_MAXC(a, b)
#define	R_MAX_s16(a, b)	R_MAXC(a, b)
#define	R_MAX_u16(a, b)	R_MAXC(a, b)
#define	R_MAX_s32(a, b)	R_MAXC(a, b)
#define	R_MAX_f32(a, b)	R_MAXF32(a, b)
#define	R_MAX_d64(a, b)	R_MAXD64(a, b)

/* *********************************************************** */

#define	R_MINEQ_u8(a, b)	R_MINEQC(a, b)
#define	R_MINEQ_s16(a, b)	R_MINEQC(a, b)
#define	R_MINEQ_u16(a, b)	R_MINEQC(a, b)
#define	R_MINEQ_s32(a, b)	R_MINEQC(a, b)
#define	R_MINEQ_f32(a, b)	R_MINEQF32(a, b)
#define	R_MINEQ_d64(a, b)	R_MINEQD64(a, b)

#define	R_MAXEQ_u8(a, b)	R_MAXEQC(a, b)
#define	R_MAXEQ_s16(a, b)	R_MAXEQC(a, b)
#define	R_MAXEQ_u16(a, b)	R_MAXEQC(a, b)
#define	R_MAXEQ_s32(a, b)	R_MAXEQC(a, b)
#define	R_MAXEQ_f32(a, b)	R_MAXEQF32(a, b)
#define	R_MAXEQ_d64(a, b)	R_MAXEQD64(a, b)

/* *********************************************************** */
#endif /* end _NO_LONGLONG */
/* *********************************************************** */


/* *********************************************************** */
#if defined(_NO_LONGLONG)
/* *********************************************************** */
#define	sorttype_u8 	mlib_s32
#define	sorttype_s16	mlib_s32
#define	sorttype_u16	mlib_s32
#define	sorttype_s32	mlib_s32
#define	sorttype_f32	mlib_f32
#define	sorttype_d64	mlib_d64

/* add for integer version */
#define	ptype_u8	mlib_u8
#define	ptype_s16	mlib_s16
#define	ptype_u16	mlib_u16
#define	ptype_s32	mlib_s32
#define	ptype_f32	mlib_f32
#define	ptype_d64	mlib_d64

/* *********************************************************** */
#else /* else if _NO_LONGLONG */
/* *********************************************************** */
#define	sorttype_u8 	mlib_s32
#define	sorttype_s16	mlib_s32
#define	sorttype_u16	mlib_s32
#define	sorttype_s32	mlib_s32
#define	sorttype_f32	mlib_s32
#define	sorttype_d64	mlib_s64

/* add for integer version */
#define	ptype_u8	mlib_u8
#define	ptype_s16	mlib_s16
#define	ptype_u16	mlib_u16
#define	ptype_s32	mlib_s32
#define	ptype_f32	mlib_s32
#define	ptype_d64	mlib_s64

/* *********************************************************** */
#endif /* end _NO_LONGLONG */
/* *********************************************************** */

#define	SORT3(d1, d2, d3, sortF)                                \
	sortF(d1, d2);                                          \
	sortF(d1, d3);                                          \
	sortF(d2, d3)

/* *********************************************************** */

#define	SORT4(d1, d2, d3, d4, sortF)                            \
	sortF(d1, d2);                                          \
	sortF(d3, d4);                                          \
	sortF(d2, d4);                                          \
	sortF(d1, d3);                                          \
	sortF(d2, d3)

/* *********************************************************** */

#define	SORT5(d1, d2, d3, d4, d5, sortF)                        \
	sortF(d1, d2);                                          \
	sortF(d3, d4);                                          \
	sortF(d3, d5);                                          \
	sortF(d4, d5);                                          \
	sortF(d1, d3);                                          \
	sortF(d2, d5);                                          \
	sortF(d2, d3);                                          \
	sortF(d2, d4);                                          \
	sortF(d3, d4)

/* *********************************************************** */

#define	SORT5M(d1, d2, d3, d4, d5, sortF)                       \
	SORT4(d1, d2, d3, d4, sortF);                           \
	sortF(d3, d5);                                          \
	sortF(d2, d3)

/* *********************************************************** */

#define	SORT6(d1, d2, d3, d4, d5, d6, sortF)                    \
	sortF(d1, d2);                                          \
	sortF(d3, d4);                                          \
	sortF(d5, d6);                                          \
	sortF(d1, d3);                                          \
	sortF(d1, d5);                                          \
	sortF(d2, d6);                                          \
	sortF(d4, d6);                                          \
	sortF(d2, d3);                                          \
	sortF(d4, d5);                                          \
	sortF(d2, d4);                                          \
	sortF(d3, d5);                                          \
	sortF(d3, d4)

/* *********************************************************** */

#define	SORT9(d1, d2, d3, d4, d5, d6, d7, d8, d9, sortF)        \
	sortF(d1, d4);                                          \
	sortF(d2, d5);                                          \
	sortF(d3, d6);                                          \
	sortF(d1, d2);                                          \
	sortF(d1, d3);                                          \
	sortF(d4, d6);                                          \
	sortF(d5, d6);                                          \
	sortF(d2, d3);                                          \
	sortF(d4, d5);                                          \
	sortF(d2, d7);                                          \
	sortF(d2, d4);                                          \
	sortF(d3, d7);                                          \
	sortF(d5, d7);                                          \
	sortF(d3, d4);                                          \
	sortF(d5, d8);                                          \
	sortF(d3, d5);                                          \
	sortF(d4, d8);                                          \
	sortF(d4, d5);                                          \
	sortF(d4, d9);                                          \
	sortF(d5, d9)

/* *********************************************************** */

#define	SORT25(sortF)                                           \
	sortF(d1, d2);                                          \
	sortF(d4, d5);                                          \
	sortF(d3, d5);                                          \
	sortF(d3, d4);                                          \
	sortF(d7, d8);                                          \
	sortF(d6, d8);                                          \
	sortF(d6, d7);                                          \
	sortF(d10, d11);                                        \
	sortF(d9, d11);                                         \
	sortF(d9, d10);                                         \
	sortF(d13, d14);                                        \
	sortF(d12, d14);                                        \
	sortF(d12, d13);                                        \
	sortF(d16, d17);                                        \
	sortF(d15, d17);                                        \
	sortF(d15, d16);                                        \
	sortF(d19, d20);                                        \
	sortF(d18, d20);                                        \
	sortF(d18, d19);                                        \
	sortF(d22, d23);                                        \
	sortF(d21, d23);                                        \
	sortF(d21, d22);                                        \
	sortF(d24, d25);                                        \
	sortF(d3, d6);                                          \
	sortF(d4, d7);                                          \
	sortF(d1, d7);                                          \
	sortF(d1, d4);                                          \
	sortF(d5, d8);                                          \
	sortF(d2, d8);                                          \
	sortF(d2, d5);                                          \
	sortF(d12, d15);                                        \
	sortF(d9, d15);                                         \
	sortF(d9, d12);                                         \
	sortF(d13, d16);                                        \
	sortF(d10, d16);                                        \
	sortF(d10, d13);                                        \
	sortF(d14, d17);                                        \
	sortF(d11, d17);                                        \
	sortF(d11, d14);                                        \
	sortF(d21, d24);                                        \
	sortF(d18, d24);                                        \
	sortF(d18, d21);                                        \
	sortF(d22, d25);                                        \
	sortF(d19, d25);                                        \
	sortF(d19, d22);                                        \
	sortF(d20, d23);                                        \
	sortF(d9, d18);                                         \
	sortF(d10, d19);                                        \
	sortF(d1, d19);                                         \
	sortF(d1, d10);                                         \
	sortF(d11, d20);                                        \
	sortF(d2, d20);                                         \
	sortF(d2, d11);                                         \
	sortF(d12, d21);                                        \
	sortF(d3, d21);                                         \
	sortF(d3, d12);                                         \
	sortF(d13, d22);                                        \
	sortF(d4, d22);                                         \
	sortF(d4, d13);                                         \
	sortF(d14, d23);                                        \
	sortF(d5, d23);                                         \
	sortF(d5, d14);                                         \
	sortF(d15, d24);                                        \
	sortF(d6, d24);                                         \
	sortF(d6, d15);                                         \
	sortF(d16, d25);                                        \
	sortF(d7, d25);                                         \
	sortF(d7, d16);                                         \
	sortF(d8, d17);                                         \
	sortF(d8, d20);                                         \
	sortF(d14, d22);                                        \
	sortF(d16, d24);                                        \
	sortF(d8, d14);                                         \
	sortF(d8, d16);                                         \
	sortF(d2, d10);                                         \
	sortF(d4, d12);                                         \
	sortF(d6, d18);                                         \
	sortF(d12, d18);                                        \
	sortF(d10, d18);                                        \
	sortF(d5, d11);                                         \
	sortF(d7, d13);                                         \
	sortF(d8, d15);                                         \
	sortF(d5, d7);                                          \
	sortF(d5, d8);                                          \
	sortF(d13, d15);                                        \
	sortF(d11, d15);                                        \
	sortF(d7, d8);                                          \
	sortF(d11, d13);                                        \
	sortF(d7, d11);                                         \
	sortF(d7, d18);                                         \
	sortF(d13, d18);                                        \
	sortF(d8, d18);                                         \
	sortF(d8, d11);                                         \
	sortF(d13, d19);                                        \
	sortF(d8, d13);                                         \
	sortF(d11, d19);                                        \
	sortF(d13, d21);                                        \
	sortF(d11, d21);                                        \
	sortF(d11, d13)

/* *********************************************************** */

#define	MEDIAN3(res, s1, s2, s3, itype)                         \
	{                                                       \
	    sorttype_##itype mask, d1, d2, d3;                  \
	                                                        \
	    d1 = s1;                                            \
	    d2 = s2;                                            \
	    d3 = s3;                                            \
	    SORT3(d1, d2, d3, SORT2_##itype);                   \
	    res = d2;                                           \
	}

/* *********************************************************** */

#define	MEDIAN5(res, s1, s2, s3, s4, s5, itype)                 \
	{                                                       \
	    sorttype_##itype mask, d1, d2, d3, d4, d5;          \
	                                                        \
	    d1 = s1;                                            \
	    d2 = s2;                                            \
	    d3 = s3;                                            \
	    d4 = s4;                                            \
	    d5 = s5;                                            \
	    SORT5(d1, d2, d3, d4, d5, SORT2_##itype);           \
	    res = d3;                                           \
	}

/* *********************************************************** */

#define	MEDIAN7(res, s1, s2, s3, s4, s5, s6, s7, itype)          \
	{                                                        \
	    sorttype_##itype mask, d1, d2, d3, d4, d5, d6, d7;   \
	                                                         \
	    d1 = s1;                                             \
	    d2 = s2;                                             \
	    d3 = s3;                                             \
	    d4 = s4;                                             \
	    d5 = s5;                                             \
	    d6 = s6;                                             \
	    d7 = s7;                                             \
	    SORT6(d1, d2, d3, d4, d5, d6, SORT2_##itype);        \
	    SORT2_##itype(d3, d7);                               \
	    SORT2_##itype(d7, d4);                               \
	    res = d7;                                            \
	}

/* *********************************************************** */

extern const mlib_median_fun_type mlib_median_row_3x3_funs[];
extern const mlib_median_fun_type mlib_median_col_3x3_funs[];
extern const mlib_median_fun_type mlib_median_init_3x3_funs[];

extern const mlib_median_fun_type mlib_median_row_5x5_funs[];
extern const mlib_median_fun_type mlib_median_col_5x5_funs[];
extern const mlib_median_fun_type mlib_median_init_5x5_funs[];

extern const mlib_median_sep_fun_type mlib_median_seprow_MxN_funs[];
extern const mlib_median_sep_fun_type mlib_median_sepcol_MxN_funs[];

/* *********************************************************** */

void mlib_median_row_3x3rect_u8(
    mlib_u8 *dl,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_row_5x5rect_u8(
    mlib_u8 *dl,
    void **psl,
    mlib_filter_state *fs);

/* *********************************************************** */

void mlib_median_initrow_3x3rsep_u8(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_initrow_3x3rsep_s16(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_initrow_3x3rsep_u16(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_initrow_3x3rsep_s32(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_initrow_3x3rsep_f32(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_initrow_3x3rsep_d64(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_row_3x3rsep_u8(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_row_3x3rsep_s16(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_row_3x3rsep_u16(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_row_3x3rsep_s32(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_row_3x3rsep_f32(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_row_3x3rsep_d64(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_col_3x3rsep_u8(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_col_3x3rsep_s16(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_col_3x3rsep_u16(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_col_3x3rsep_s32(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_col_3x3rsep_f32(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_col_3x3rsep_d64(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

/* *********************************************************** */

void mlib_median_row_5x5rsep_u8(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_row_5x5rsep_s16(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_row_5x5rsep_u16(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_row_5x5rsep_s32(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_row_5x5rsep_f32(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_row_5x5rsep_d64(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_col_5x5rsep_u8(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_col_5x5rsep_s16(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_col_5x5rsep_u16(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_col_5x5rsep_s32(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_col_5x5rsep_f32(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_col_5x5rsep_d64(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_initrow_5x5rsep_u8(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_initrow_5x5rsep_s16(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_initrow_5x5rsep_u16(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_initrow_5x5rsep_s32(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_initrow_5x5rsep_f32(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

void mlib_median_initrow_5x5rsep_d64(
    mlib_u8 *pdst,
    void **psl,
    mlib_filter_state *fs);

/* *********************************************************** */

void mlib_median_row_MxNrect_u8(
    mlib_u8 *pdst,
    void **_psl,
    mlib_filter_state *fs);

void mlib_median_row_MxNrect_s16(
    mlib_u8 *pdst,
    void **_psl,
    mlib_filter_state *fs);

void mlib_median_row_MxNrect_u16(
    mlib_u8 *pdst,
    void **_psl,
    mlib_filter_state *fs);

void mlib_median_row_MxNplus_u8(
    mlib_u8 *pdst,
    void **_psl,
    mlib_filter_state *fs);

void mlib_median_row_MxNrsep_u8(
    mlib_u8 *pdst,
    void **_psl,
    mlib_filter_state *fs);

void mlib_median_col_MxNrect_u8(
    mlib_u8 *pdst,
    void **_psl,
    mlib_filter_state *fs);

void mlib_median_col_MxNplus_u8(
    mlib_u8 *pdst,
    void **_psl,
    mlib_filter_state *fs);

void mlib_median_col_MxNxmsk_u8(
    mlib_u8 *pdst,
    void **_psl,
    mlib_filter_state *fs);

void mlib_median_col_MxNrsep_u8(
    mlib_u8 *pdst,
    void **_psl,
    mlib_filter_state *fs);

void mlib_median_col_MxNrect_s16(
    mlib_u8 *pdst,
    void **_psl,
    mlib_filter_state *fs);

void mlib_median_initrow_MxNrsep_u8(
    mlib_u8 *pdst,
    void **_psl,
    mlib_filter_state *fs);

void mlib_median_seprow_MxN_u8(
    mlib_u8 *pdst,
    mlib_u8 *psrc,
    mlib_filter_state *fs);

void mlib_median_sepcol_MxN_u8(
    mlib_u8 *pdst,
    mlib_u8 *psrc,
    mlib_filter_state *fs);

void mlib_median_col_MxNrect_u16(
    mlib_u8 *pdst,
    void **_psl,
    mlib_filter_state *fs);

/* *********************************************************** */

void mlib_ImageMedianFilter_row(
    mlib_filter_state *fs);

mlib_status mlib_ImageMedianFilter_col(
    mlib_filter_state *fs);

mlib_status mlib_ImageMedianFilter_fun(
    mlib_image *dst,
    const mlib_image *src,
    mlib_filter_state *fs);

void mlib_ImageGetEdges(
    mlib_image *edst,
    mlib_image *esrc,
    const mlib_image *dst,
    const mlib_image *src,
    mlib_filter_state *fs);

mlib_status mlib_ImageFilterClearEdge(
    mlib_image *dst,
    mlib_filter_state *fs);

mlib_status mlib_ImageFilterCopyEdge(
    mlib_image *dst,
    const mlib_image *src,
    mlib_filter_state *fs);

/* *********************************************************** */

mlib_status mlib_ImageMedianFilter3x3_alltypes(
    mlib_image *dst,
    const mlib_image *src,
    mlib_median_mask mmask,
    mlib_s32 cmask,
    mlib_edge edge,
    mlib_s32 fp_version);

mlib_status mlib_ImageMedianFilter5x5_alltypes(
    mlib_image *dst,
    const mlib_image *src,
    mlib_median_mask mmask,
    mlib_s32 cmask,
    mlib_edge edge,
    mlib_s32 fp_version);

mlib_status mlib_ImageMedianFilter7x7_alltypes(
    mlib_image *dst,
    const mlib_image *src,
    mlib_median_mask mmask,
    mlib_s32 cmask,
    mlib_edge edge,
    mlib_s32 fp_version);

mlib_status mlib_ImageMedianFilterMxN_alltypes(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 m,
    mlib_s32 n,
    mlib_median_mask mmask,
    mlib_s32 cmask,
    mlib_edge edge,
    mlib_s32 fp_version,
    mlib_s32 rank);

#ifdef __cplusplus
}
#endif

#endif /* __MLIB_IMAGEMEDIANFILTER_H */
