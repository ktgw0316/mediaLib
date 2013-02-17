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
 * Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#ifndef _MLIB_S_IMAGEZOOM_H
#define	_MLIB_S_IMAGEZOOM_H

#pragma ident	"@(#)mlib_s_ImageZoom.h	9.7	08/10/29 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageFilters.h>
#include <mlib_m_ImageFilters.h>

#define	_MLIB_C_VER_

#include <mlib_ImageZoom.h>

/* *************************************************************** */

typedef union
{
	__m128 m128;
	__m128d m128d;
	__m128i m128i;
} mlib_m128;

/* *************************************************************** */

void mlib_s_ImageChannelExtract_U8_43R_D1(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize);

void mlib_s_ImageChannelExtract_U8_43L_D1(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize);

void mlib_s_ImageChannelExtract_S16_43L_D1(
    mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize);

/* *************************************************************** */

mlib_status mlib_s_ImageZoomBlend_BL_U8(
    mlib_work_image * param);
mlib_status mlib_s_ImageZoomBlend_BC_U8(
    mlib_work_image * param);
mlib_status mlib_s_ImageZoom_BL_U8_1(
    mlib_work_image * param);
mlib_status mlib_s_ImageZoom_BL_U8_2(
    mlib_work_image * param);
mlib_status mlib_s_ImageZoom_BC_U8_12(
    mlib_work_image * param);
mlib_status mlib_s_ImageZoom_BL_S16_1(
    mlib_work_image * param);
mlib_status mlib_s_ImageZoom_BL_S16_2(
    mlib_work_image * param);
mlib_status mlib_s_ImageZoom_BL_S16_3(
    mlib_work_image * param);
mlib_status mlib_s_ImageZoom_BL_S16_4(
    mlib_work_image * param);
mlib_status mlib_s_ImageZoom_BL_U16_1(
    mlib_work_image * param);
mlib_status mlib_s_ImageZoom_BL_U16_2(
    mlib_work_image * param);
mlib_status mlib_s_ImageZoom_BL_U16_3(
    mlib_work_image * param);
mlib_status mlib_s_ImageZoom_BL_U16_4(
    mlib_work_image * param);
mlib_status mlib_s_ImageZoom_BC_S16_12(
    mlib_work_image * param);
mlib_status mlib_s_ImageZoom_BC_S16_3(
    mlib_work_image * param);
mlib_status mlib_s_ImageZoom_BC_S16_4(
    mlib_work_image * param);
mlib_status mlib_s_ImageZoom_BC_U16_1(
    mlib_work_image * param);
mlib_status mlib_s_ImageZoom_BC_U16_2(
    mlib_work_image * param);
mlib_status mlib_s_ImageZoom_BC_U16_3(
    mlib_work_image * param);
mlib_status mlib_s_ImageZoom_BC_U16_4(
    mlib_work_image * param);

void mlib_s_ImageBlendLine(
    mlib_work_image * param,
    mlib_u8 *tdp,
    __m128i * buffz,
    __m128i * buffd);

/* *************************************************************** */

#define	BUFF_SIZE	2048

/* *************************************************************** */

#define	VARIABLE(FORMAT)                                            \
	FORMAT *sp = GetElemSubStruct(current, sp), *dp =           \
	GetElemSubStruct(current, dp), *tsp = sp;                   \
	mlib_s32 i, j, dx = GetElemStruct(DX), dy =                 \
	GetElemStruct(DY), x =                                      \
	GetElemSubStruct(current, srcX) & MLIB_MASK, y =            \
	GetElemSubStruct(current, srcY) & MLIB_MASK, src_stride =   \
	GetElemStruct(src_stride), dst_stride =                     \
	GetElemStruct(dst_stride), width =                          \
	GetElemSubStruct(current, width), height =                  \
	GetElemSubStruct(current, height);

/* *************************************************************** */

#if 1

/* somewhat slower, but no ABR (Array Bounds Read) */

#define	LOAD_2s32_to_m128(res, ptr0, ptr1)                          \
	{                                                           \
	    mlib_m128 src_u0, src_u1;                               \
	                                                            \
	    src_u0.m128 = _mm_load_ss((mlib_f32 *)(ptr0));          \
	    src_u1.m128 = _mm_load_ss((mlib_f32 *)(ptr1));          \
	    res = _mm_unpacklo_epi32(src_u0.m128i, src_u1.m128i);   \
	    res = _mm_unpacklo_epi8(mzero, res);                    \
	}

#define	LOAD_s32_to_m128(res, ptr0)                                 \
	{                                                           \
	    mlib_m128 src_u0;                                       \
	                                                            \
	    src_u0.m128 = _mm_load_ss((mlib_f32 *)(ptr0));          \
	    res = _mm_unpacklo_epi8(mzero, src_u0.m128i);           \
	}

#define	LOAD_4s16_to_m128(res, ptr0, ptr1)                          \
	{                                                           \
	    mlib_m128 src_u0, src_u1;                               \
	                                                            \
	    src_u0.m128 = _mm_load_ss((mlib_f32 *)(ptr0));          \
	    src_u1.m128 = _mm_load_ss((mlib_f32 *)(ptr1));          \
	    res = _mm_unpacklo_epi16(src_u0.m128i, src_u1.m128i);   \
	}

#else

#define	LOAD_2s32_to_m128(res, ptr0, ptr1)                          \
	{                                                           \
	    res = _mm_unpacklo_epi32(                               \
		    _mm_loadl_epi64((void *)(ptr0)),                \
		    _mm_loadl_epi64((void *)(ptr1)));               \
	    res = _mm_unpacklo_epi8(mzero, res);                    \
	}

#define	LOAD_s32_to_m128(res, ptr0)                                 \
	{                                                           \
	    res = _mm_loadl_epi64((void *)(ptr0));                  \
	    res = _mm_unpacklo_epi8(mzero, res);                    \
	}

#define	LOAD_4s16_to_m128(res, ptr0, ptr1)                          \
	{                                                           \
	    res = _mm_unpacklo_epi16(                               \
		    _mm_loadl_epi64((void *)(ptr0)),                \
		    _mm_loadl_epi64((void *)(ptr1)));               \
	}

#endif

/* *************************************************************** */

#define	sse_loadl_epi32(res, ptr0)                                  \
	{                                                           \
	    mlib_m128 src_u0;                                       \
	                                                            \
	    src_u0.m128 = _mm_load_ss((mlib_f32 *)(ptr0));          \
	    res = src_u0.m128i;                                     \
	}

/* *************************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_IMAGEZOOM_H */
