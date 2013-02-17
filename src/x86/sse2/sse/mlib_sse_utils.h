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

#ifndef _MLIB_SSE_UTILS_H
#define	_MLIB_SSE_UTILS_H

#pragma ident	"@(#)mlib_sse_utils.h	9.5	07/11/05 SMI"

/*
 * Macro definitions for SSE.
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

#include <mlib_image.h>
#include <mlib_mmx_utils.h>

/* *********************************************************** */
#define	_ltab_8(sh)	(0xffff >> ((int)(sh) & 15))
#define	_rtab_8(sh)	(0xffff << (15 - ((int)(sh) & 15)))
/* *********************************************************** */
#define	sse_edge8(s1, s2)                                             \
	((((mlib_addr)s1) & ~0xf) ==                                  \
	    (((mlib_addr)s2) & ~0xf)) ? _ltab_8(s1) & _rtab_8(s2) :   \
	_ltab_8(s1)
/* *********************************************************** */
#define	sse_edge16(s1, s2)                                              \
	((((mlib_addr)s1) & ~0xf) ==                                    \
	    (((mlib_addr)s2) & ~0xf)) ? _ltab_16(s1) & _rtab_16(s2) :   \
	_ltab_16(s1)
/* *********************************************************** */
#define	sse_edge32(s1, s2)                                              \
	((((mlib_addr)s1) & ~0xf) ==                                    \
	    (((mlib_addr)s2) & ~0xf)) ? _ltab_32(s1) & _rtab_32(s2) :   \
	_ltab_32(s1)
/* *********************************************************** */
/* sse2 instruction is used */
#define	LOAD_2x64(ptr, ind0, ind1)                                        \
	_mm_unpacklo_epi64(_mm_loadl_epi64((void *)((mlib_d64 *)(ptr) +   \
	    (ind0))),                                                     \
	    _mm_loadl_epi64((void *)((mlib_d64 *)(ptr) + (ind1))))
/* *********************************************************** */
/* sse2 instruction is used */
#define	sse_pst_32(src, dst, mask)                                       \
	{                                                                \
	    __m128i __mask64 =                                           \
		LOAD_2x64(mask_pst_323, (mask >> 2) & 0x3, mask & 0x);   \
	    __m128i __s0 = _mm_and_si128(__mask64, (src));               \
	    __m128i __d0 =                                               \
		_mm_andnot_si128(__mask64, *(__m128i *) (dst));          \
	    *(__m128i *) (dst) = _mm_or_si128(__d0, __s0);               \
	}
/* *********************************************************** */
/* sse2 instruction is used */
#define	sse_pst_16(src, dst, mask)                                       \
	{                                                                \
	    __m128i __mask64 =                                           \
		LOAD_2x64(mask_pst_16, (mask >> 4) & 0xf, mask & 0xf);   \
	    __m128i __s0 = _mm_and_si128(__mask64, (src));               \
	    __m128i __d0 =                                               \
		_mm_andnot_si128(__mask64, *(__m128i *) (dst));          \
	    *(__m128i *) (dst) = _mm_or_si128(__d0, __s0);               \
	}
/* *********************************************************** */
/* sse2 instruction is used */
#define	sse_pst_8(src, dst, mask)                                         \
	{                                                                 \
	    __m128i __mask64 =                                            \
		LOAD_2x64(mask_pst_8, (mask >> 8) & 0xff, mask & 0xff);   \
	    __m128i __s0 =                                                \
		_mm_and_si128(__mask64, *(__m128i *) & (src));            \
	    __m128i __d0 =                                                \
		_mm_andnot_si128(__mask64, *(__m128i *) (dst));           \
	    *(__m128i *) (dst) = _mm_or_si128(__d0, __s0);                \
	}

/* *********************************************************** */
#define	sse_pst_u8(src, dst, mask)					\
	{								\
	    __m64 __mask64_0 = mask_pst_8[(mask >> 8) & 0xff];		\
	    __m64 __mask64_1 = mask_pst_8[mask & 0xff];			\
	    char * _dst = (char *) dst;					\
	    __m64 * _src = (__m64 *) & src;				\
	    _mm_maskmove_si64(_src[0], __mask64_0, _dst);		\
	    _mm_maskmove_si64(_src[1], __mask64_1, _dst + 8);		\
	}

/* *********************************************************** */
/* sse2 instruction is used */
#define	sse2_pst_8(src, dst, mask)                                      \
	{                                                               \
	    __m128i __mask64 =                                          \
		LOAD_2x64(mask_pst_8, (mask >> 8) & 0xff, mask & 0xff); \
	    _mm_maskmoveu_si128(*(__m128i *) & (src),			\
				__mask64, (char *) dst);		\
	}

/* *********************************************************** */

#if 0
#define	LOADU__m128i(res, ptr)	res = _mm_loadu_si128((void*)(ptr))
#define	LOADU__m128	(res, ptr)  res = _mm_loadu_ps((void*)(ptr))
#define	LOADU__m128d(res, ptr)	res = _mm_loadu_pd((void*)(ptr))
#else

/* sse2 instruction is used */
#define	LOADU__m128i(res, ptr)                                  \
	res =                                                   \
	_mm_unpacklo_epi64(_mm_loadl_epi64((void *)(ptr)),      \
	    _mm_loadl_epi64((void *)((mlib_u8 *)(ptr) + 8)))
/* sse instruction is used */
#define	LOADU__m128(res, ptr)                                   \
	res =                                                   \
	_mm_loadh_pi(_mm_loadl_pi(res, (void *)(ptr)),          \
	    (void *)((mlib_u8 *)(ptr) + 8))
/* sse2 instruction is used */
#define	LOADU__m128d(res, ptr)                                  \
	res =                                                   \
	_mm_loadh_pd(_mm_loadl_pd(res, (void *)(ptr)),          \
	    (void *)((mlib_u8 *)(ptr) + 8))
#endif
/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_SSE_UTILS_H */
