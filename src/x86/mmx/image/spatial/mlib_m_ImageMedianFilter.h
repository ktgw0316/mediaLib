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

#ifndef _MLIB_M_IMAGEMEDIANFILTER_H
#define	_MLIB_M_IMAGEMEDIANFILTER_H

#pragma ident	"@(#)mlib_m_ImageMedianFilter.h	9.2	07/11/05 SMI"

/*
 * DESCRIPTION
 *    Internal macros and types for mlib_ImageMedianFilter* functions.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_mmx_utils.h>

/* *********************************************************** */
#define	SORT2L(a, b)	mask = ((b - a) & ((b - a) >> 31)); a += mask; b -= mask
#define	SORT2C(a, b)	{ if (a > b) { mask = a; a = b; b = mask; } }
#define	SORT2_u8(a, b)	SORT2L(a, b)
#define	SORT2_s16(a, b)	SORT2L(a, b)
#define	SORT2_u16(a, b)	SORT2L(a, b)
#define	SORT2_s32(a, b)	SORT2C(a, b)
#define	SORT2_f32(a, b)	SORT2C(a, b)
#define	SORT2_d64(a, b)	SORT2C(a, b)
/* *********************************************************** */
#define	MSORT_u8(a, b)                                          \
	mask = _mm_cmpgt_pi8(a, b);                             \
	mask = _mm_and_si64(mask, _mm_sub_pi8(b, a));           \
	a = _mm_add_pi8(a, mask);                               \
	b = _mm_sub_pi8(b, mask)
#define	MSORT_s16(a, b)                                         \
	mask = _mm_cmpgt_pi16(a, b);                            \
	mask = _mm_and_si64(mask, _mm_sub_pi16(b, a));          \
	a = _mm_add_pi16(a, mask);                              \
	b = _mm_sub_pi16(b, mask)
#define	MSORT_u16(a, b)	MSORT_s16(a, b)
#define	MSORT_s32(a, b)                                         \
	mask = _mm_cmpgt_pi32(a, b);                            \
	mask = _mm_and_si64(mask, _mm_sub_pi32(b, a));          \
	a = _mm_add_pi32(a, mask);                              \
	b = _mm_sub_pi32(b, mask)
/* *********************************************************** */
#define	sorttype_u8	mlib_s32
#define	sorttype_s16	mlib_s32
#define	sorttype_u16	mlib_s32
#define	sorttype_s32	mlib_s32
#define	sorttype_f32	mlib_f32
#define	sorttype_d64	mlib_d64
/* *********************************************************** */
#define	SORT3(d1, d2, d3, sortF)                                \
	sortF(d1, d2);                                          \
	sortF(d1, d3);                                          \
	sortF(d2, d3)
#define	SORT4(d1, d2, d3, d4, sortF)                            \
	sortF(d1, d2);                                          \
	sortF(d3, d4);                                          \
	sortF(d2, d4);                                          \
	sortF(d1, d3);                                          \
	sortF(d2, d3)
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
#define	SORT5M(d1, d2, d3, d4, d5, sortF)                       \
	SORT4(d1, d2, d3, d4, sortF);                           \
	sortF(d3, d5);                                          \
	sortF(d2, d3)
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
#define	SORT7(d1, d2, d3, d4, d5, d6, d7, sortF)                \
	SORT6(d1, d2, d3, d5, d6, d7, sortF);                   \
	sortF(d3, d4);                                          \
	sortF(d4, d5);
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
#define	MMX_XOR_u8(x)	x = _mm_xor_si64(x, mask80);
#define	MMX_XOR_u16(x)	x = _mm_xor_si64(x, mask80);
#define	MMX_XOR_s16(x)
#define	MMX_XOR_s32(x)
#define	MMX_SORT3(d1, d2, d3, T)                                \
	MMX_XOR_##T(d1) MMX_XOR_##T(d2) MMX_XOR_##T(d3)         \
	MSORT_##T(d1, d2);                                      \
	MSORT_##T(d1, d3);                                      \
	MSORT_##T(d2, d3);                                      \
	MMX_XOR_##T(d2)
#define	MMX_SORT5(d1, d2, d3, d4, d5, T)                        \
	MMX_XOR_##T(d1) MMX_XOR_##T(d2) MMX_XOR_##T(d3)         \
	MMX_XOR_##T(d4) MMX_XOR_##T(d5) MSORT_##T(d1, d2);      \
	MSORT_##T(d3, d4);                                      \
	MSORT_##T(d3, d5);                                      \
	MSORT_##T(d4, d5);                                      \
	MSORT_##T(d1, d3);                                      \
	MSORT_##T(d2, d5);                                      \
	MSORT_##T(d2, d3);                                      \
	MSORT_##T(d2, d4);                                      \
	MSORT_##T(d3, d4);                                      \
	MMX_XOR_##T(d3)
#define	MMX_SORT6(d1, d2, d3, d4, d5, d6, T)                    \
	MMX_XOR_##T(d1) MMX_XOR_##T(d2) MMX_XOR_##T(d3)         \
	MMX_XOR_##T(d4) MMX_XOR_##T(d5) MMX_XOR_##T(d6)         \
	MSORT_##T(d1, d2);                                      \
	MSORT_##T(d3, d4);                                      \
	MSORT_##T(d5, d6);                                      \
	MSORT_##T(d1, d3);                                      \
	MSORT_##T(d1, d5);                                      \
	MSORT_##T(d2, d6);                                      \
	MSORT_##T(d4, d6);                                      \
	MSORT_##T(d2, d3);                                      \
	MSORT_##T(d4, d5);                                      \
	MSORT_##T(d2, d4);                                      \
	MSORT_##T(d3, d5);                                      \
	MSORT_##T(d3, d4)
#define	MMX_SORT7(d1, d2, d3, d4, d5, d6, d7, T)                \
	MMX_SORT6(d1, d2, d3, d5, d6, d7, T);                   \
	MMX_XOR_##T(d4) MSORT_##T(d3, d4);                      \
	MSORT_##T(d4, d5);                                      \
	MMX_XOR_##T(d4)
#define	MMX_SORT9(d1, d2, d3, d4, d5, d6, d7, d8, d9, T)        \
	MMX_XOR_##T(d1) MMX_XOR_##T(d2) MMX_XOR_##T(d3)         \
	MMX_XOR_##T(d4) MMX_XOR_##T(d5) MMX_XOR_##T(d6)         \
	MMX_XOR_##T(d7) MMX_XOR_##T(d8) MMX_XOR_##T(d9)         \
	MSORT_##T(d1, d4);                                      \
	MSORT_##T(d2, d5);                                      \
	MSORT_##T(d3, d6);                                      \
	MSORT_##T(d1, d2);                                      \
	MSORT_##T(d1, d3);                                      \
	MSORT_##T(d4, d6);                                      \
	MSORT_##T(d5, d6);                                      \
	MSORT_##T(d2, d3);                                      \
	MSORT_##T(d4, d5);                                      \
	MSORT_##T(d2, d7);                                      \
	MSORT_##T(d2, d4);                                      \
	MSORT_##T(d3, d7);                                      \
	MSORT_##T(d5, d7);                                      \
	MSORT_##T(d3, d4);                                      \
	MSORT_##T(d5, d8);                                      \
	MSORT_##T(d3, d5);                                      \
	MSORT_##T(d4, d8);                                      \
	MSORT_##T(d4, d5);                                      \
	MSORT_##T(d4, d9);                                      \
	MSORT_##T(d5, d9);                                      \
	MMX_XOR_##T(d5)
#define	MMX_SORT25(T)                                           \
	MMX_XOR_##T(d1) MMX_XOR_##T(d2) MMX_XOR_##T(d3)         \
	MMX_XOR_##T(d4) MMX_XOR_##T(d5) MMX_XOR_##T(d6)         \
	MMX_XOR_##T(d7) MMX_XOR_##T(d8) MMX_XOR_##T(d9)         \
	MMX_XOR_##T(d10) MMX_XOR_##T(d11) MMX_XOR_##T(d12)      \
	MMX_XOR_##T(d13) MMX_XOR_##T(d14) MMX_XOR_##T(d15)      \
	MMX_XOR_##T(d16) MMX_XOR_##T(d17) MMX_XOR_##T(d18)      \
	MMX_XOR_##T(d19) MMX_XOR_##T(d20) MMX_XOR_##T(d21)      \
	MMX_XOR_##T(d22) MMX_XOR_##T(d23) MMX_XOR_##T(d24)      \
	MMX_XOR_##T(d25) MSORT_##T(d1, d2);                     \
	MSORT_##T(d4, d5);                                      \
	MSORT_##T(d3, d5);                                      \
	MSORT_##T(d3, d4);                                      \
	MSORT_##T(d7, d8);                                      \
	MSORT_##T(d6, d8);                                      \
	MSORT_##T(d6, d7);                                      \
	MSORT_##T(d10, d11);                                    \
	MSORT_##T(d9, d11);                                     \
	MSORT_##T(d9, d10);                                     \
	MSORT_##T(d13, d14);                                    \
	MSORT_##T(d12, d14);                                    \
	MSORT_##T(d12, d13);                                    \
	MSORT_##T(d16, d17);                                    \
	MSORT_##T(d15, d17);                                    \
	MSORT_##T(d15, d16);                                    \
	MSORT_##T(d19, d20);                                    \
	MSORT_##T(d18, d20);                                    \
	MSORT_##T(d18, d19);                                    \
	MSORT_##T(d22, d23);                                    \
	MSORT_##T(d21, d23);                                    \
	MSORT_##T(d21, d22);                                    \
	MSORT_##T(d24, d25);                                    \
	MSORT_##T(d3, d6);                                      \
	MSORT_##T(d4, d7);                                      \
	MSORT_##T(d1, d7);                                      \
	MSORT_##T(d1, d4);                                      \
	MSORT_##T(d5, d8);                                      \
	MSORT_##T(d2, d8);                                      \
	MSORT_##T(d2, d5);                                      \
	MSORT_##T(d12, d15);                                    \
	MSORT_##T(d9, d15);                                     \
	MSORT_##T(d9, d12);                                     \
	MSORT_##T(d13, d16);                                    \
	MSORT_##T(d10, d16);                                    \
	MSORT_##T(d10, d13);                                    \
	MSORT_##T(d14, d17);                                    \
	MSORT_##T(d11, d17);                                    \
	MSORT_##T(d11, d14);                                    \
	MSORT_##T(d21, d24);                                    \
	MSORT_##T(d18, d24);                                    \
	MSORT_##T(d18, d21);                                    \
	MSORT_##T(d22, d25);                                    \
	MSORT_##T(d19, d25);                                    \
	MSORT_##T(d19, d22);                                    \
	MSORT_##T(d20, d23);                                    \
	MSORT_##T(d9, d18);                                     \
	MSORT_##T(d10, d19);                                    \
	MSORT_##T(d1, d19);                                     \
	MSORT_##T(d1, d10);                                     \
	MSORT_##T(d11, d20);                                    \
	MSORT_##T(d2, d20);                                     \
	MSORT_##T(d2, d11);                                     \
	MSORT_##T(d12, d21);                                    \
	MSORT_##T(d3, d21);                                     \
	MSORT_##T(d3, d12);                                     \
	MSORT_##T(d13, d22);                                    \
	MSORT_##T(d4, d22);                                     \
	MSORT_##T(d4, d13);                                     \
	MSORT_##T(d14, d23);                                    \
	MSORT_##T(d5, d23);                                     \
	MSORT_##T(d5, d14);                                     \
	MSORT_##T(d15, d24);                                    \
	MSORT_##T(d6, d24);                                     \
	MSORT_##T(d6, d15);                                     \
	MSORT_##T(d16, d25);                                    \
	MSORT_##T(d7, d25);                                     \
	MSORT_##T(d7, d16);                                     \
	MSORT_##T(d8, d17);                                     \
	MSORT_##T(d8, d20);                                     \
	MSORT_##T(d14, d22);                                    \
	MSORT_##T(d16, d24);                                    \
	MSORT_##T(d8, d14);                                     \
	MSORT_##T(d8, d16);                                     \
	MSORT_##T(d2, d10);                                     \
	MSORT_##T(d4, d12);                                     \
	MSORT_##T(d6, d18);                                     \
	MSORT_##T(d12, d18);                                    \
	MSORT_##T(d10, d18);                                    \
	MSORT_##T(d5, d11);                                     \
	MSORT_##T(d7, d13);                                     \
	MSORT_##T(d8, d15);                                     \
	MSORT_##T(d5, d7);                                      \
	MSORT_##T(d5, d8);                                      \
	MSORT_##T(d13, d15);                                    \
	MSORT_##T(d11, d15);                                    \
	MSORT_##T(d7, d8);                                      \
	MSORT_##T(d11, d13);                                    \
	MSORT_##T(d7, d11);                                     \
	MSORT_##T(d7, d18);                                     \
	MSORT_##T(d13, d18);                                    \
	MSORT_##T(d8, d18);                                     \
	MSORT_##T(d8, d11);                                     \
	MSORT_##T(d13, d19);                                    \
	MSORT_##T(d8, d13);                                     \
	MSORT_##T(d11, d19);                                    \
	MSORT_##T(d13, d21);                                    \
	MSORT_##T(d11, d21);                                    \
	MSORT_##T(d11, d13);                                    \
	MMX_XOR_##T(d13)
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

	void (**row_funs) (
	    mlib_u8 *pdst,
	    void **_psl,
	    struct mlib_filter_state_struct *fs);
	void (**col_funs) (
	    mlib_u8 *pdst,
	    void **_psl,
	    struct mlib_filter_state_struct *fs);
	void (**init_funs) (
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
    mlib_filter_state * fs);

typedef void (*mlib_median_sep_fun_type) (
    mlib_u8 *pdst,
    mlib_u8 *psrc,
    mlib_filter_state * fs);

/* *********************************************************** */

mlib_status mlib_ImageMedianFilter_fun(
    mlib_image *dst,
    mlib_image *src,
    mlib_filter_state * fs);

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_M_IMAGEMEDIANFILTER_H */
