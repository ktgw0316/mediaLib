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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_VideoColorAVDBlendUtil_RA.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorBlend_[ZERO, ONE, DA, SA, OMDA, OMSA]_
 *      [ZERO, ONE, DA, SA, OMDA, OMSA]_RA
 *          blend two images with blend factors with alpha reset
 *
 * SYNOPSIS
 *      void mlib_VideoColorBlend_[ZERO, ONE, DA, SA, OMDA, OMSA]_
 *           [ZERO, ONE, DA, SA, OMDA, OMSA]_RA
 *                                             (mlib_u32 *dst,
 *                                              mlib_u32 *src1,
 *                                              mlib_u32 *src2,
 *                                              mlib_s32 w,
 *                                              mlib_s32 h,
 *                                              mlib_s32 dlb,
 *                                              mlib_s32 slb1,
 *                                              mlib_s32 slb2)
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src1    pointer to first input image
 *      src2    pointer to second input image
 *      w       image width
 *      h       image height
 *      dlb     linebytes for output image
 *      slb1    linebytes for 1st input image
 *      slb2    linebytes for 2nd input image
 *
 * DESCRIPTION
 *        dst = (src1 * src1_blend) + (src2 * src2_blend)
 */

#include <mlib_algebra.h>
#include <mlib_VideoColorAVDBlend_proto.h>
#include <mlib_c_ImageBlendTable.h>
#include <mlib_ImageDivTables.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

void
mlib_VideoColorBlend_ZERO_ZERO_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 i;

	for (i = 0; i < h; i++) {
		__mlib_VectorZero_S32((mlib_s32 *)dst, w);
		dst += dlb / 4;
	}
}

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(X)	((mlib_s32)(X))

#else /* MLIB_USE_FTOI_CLAMPING */

#define	FLOAT2INT_CLAMP(X)                                       \
	(((X) >= MLIB_S32_MAX) ? MLIB_S32_MAX : (mlib_s32)(X))

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	BLEND_VALIDATE                                          \
	mlib_u8 *psrc1, *psrc2, *pdst;                          \
	mlib_s32 i, j;                                          \
	                                                        \
	pdst = (mlib_u8 *)dst;                                  \
	psrc1 = (mlib_u8 *)src1;                                \
	psrc2 = (mlib_u8 *)src2

#define	BLEND_VALIDATE_1                                        \
	mlib_u8 *psrc2, *pdst;	                                \
	mlib_s32 i, j;                                          \
	                                                        \
	pdst = (mlib_u8 *)dst;                                  \
	psrc2 = (mlib_u8 *)src2

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	COPY_CONST32	0xFFFFFF00
#define	COPY_CONST64	MLIB_U64_CONST(0xFFFFFF00FFFFFF00)

#else /* _LITTLE_ENDIAN */

#define	COPY_CONST32	0xFFFFFF
#define	COPY_CONST64	MLIB_U64_CONST(0xFFFFFF00FFFFFF)

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

static void
mlib_VideoColorCopy_RA_U32(
	mlib_u32 *dst,
	const mlib_u32 *src,
	mlib_s32 n)
{
#if 0
	__m128i x_s, x_d, *px_s, *px_d;
	const __m128i x_mask = _mm_set1_epi32(0xffffff00);
	mlib_s32 i;

	if (n <= 0)
		return;

	px_s = (__m128i *)src;
	px_d = (__m128i *)dst;
	if (dst == src) {
		if (0 == (0xf & (mlib_addr)src)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (n - 4); i += 4) {
				x_s = _mm_load_si128(px_s);
				x_s = _mm_and_si128(x_s, x_mask);
				_mm_store_si128(px_s, x_s);
				px_s++;
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (n - 4); i += 4) {
				x_s = _mm_loadu_si128(px_s);
				x_s = _mm_and_si128(x_s, x_mask);
				_mm_storeu_si128(px_s, x_s);
				px_s++;
			}
		}

		for (; i <= (n - 1); i++) {
			((mlib_u8 *)dst)[4 * i] = 0;
		}
	} else {
		if (0 == (0xf & ((mlib_addr)src | (mlib_addr)dst))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (n - 4); i += 4) {
				x_s = _mm_load_si128(px_s);
				px_s++;
				x_d = _mm_and_si128(x_s, x_mask);
				_mm_store_si128(px_d, x_d);
				px_d++;
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (n - 4); i += 4) {
				x_s = _mm_loadu_si128(px_s);
				px_s++;
				x_d = _mm_and_si128(x_s, x_mask);
				_mm_storeu_si128(px_d, x_d);
				px_d++;
			}
		}

		for (; i <= (n - 1); i++) {
			dst[i] = src[i] & COPY_CONST32;
		}
	}
#endif

/* workaround bug 6399351 */
#if 1
	mlib_s32 i;

	if (n <= 0)
		return;

	if (dst != src) {

#ifndef _NO_LONGLONG

		if ((((mlib_addr)dst | (mlib_addr)src) & 7) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n / 2; i++) {
				((mlib_u64 *)dst)[i] =
					((mlib_u64 *)src)[i] & COPY_CONST64;
			}
		} else
#endif /* _NO_LONGLONG */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n / 2; i++) {
				dst[2 * i + 1] = src[2 * i + 1] & COPY_CONST32;
				dst[2 * i] = src[2 * i] & COPY_CONST32;
			}

		if (n & 1) {
			dst[2 * i] = src[2 * i] & COPY_CONST32;
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++)
			((mlib_u8 *)dst)[4 * i] = 0;
	}
#endif
}

/* *********************************************************** */

void
mlib_VideoColorCopy_RA(
	mlib_u32 *dst,
	const mlib_u32 *src,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	mlib_s32 i;

	if (dlb == (4 * w) && slb == dlb) {
		w *= h;
		h = 1;
	}

	for (i = 0; i < h; i++) {
		mlib_VideoColorCopy_RA_U32(dst, src, w);
		dst += dlb / 4;
		src += slb / 4;
	}
}

/* *********************************************************** */

void
mlib_VideoColorCopy2_RA(
	mlib_u32 *dst,
	const mlib_u32 *src,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 istop,
	mlib_s32 istart,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	mlib_s32 i;

	for (i = 0; i < h; i++) {
		mlib_VideoColorCopy_RA_U32(dst, src, istop);
		mlib_VideoColorCopy_RA_U32(dst + istart, src + istart,
			w - istart);
		dst += dlb / 4;
		src += slb / 4;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ZERO_ONE_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorCopy_RA(dst, src2, w, h, dlb, slb2);
}

/* *********************************************************** */

#define	DST_VAL	(v3 << 24) | (v2 << 16) | (v1 << 8)

#define	READ_SRC123                                             \
	{                                                       \
	    v1 = *(mlib_f32 *)((mlib_u8 *)mlib_U82F32 +         \
		    ((val0 >> 6) & 0x3FC)) * a;                 \
	    v2 = *(mlib_f32 *)((mlib_u8 *)mlib_U82F32 +         \
		    ((val0 >> 14) & 0x3FC)) * a;                \
	    v3 = *(mlib_f32 *)((mlib_u8 *)mlib_U82F32 +         \
		    ((val0 >> 22) & 0x3FC)) * a;                \
	}

/* *********************************************************** */
#define	GetAlpha1a(s0, s1, a0, a1, a2, a3)				\
{	\
	x_t1 = _mm_and_si128(s0, x_mask);		\
	x_t2 = _mm_and_si128(s1, x_mask);		\
	x_t1 = _mm_packs_epi32(x_t1, x_t2);		\
	x_t1 = _mm_packus_epi16(x_t1, x_t1);	\
	x_t1 = _mm_unpacklo_epi8(x_t1, x_t1);	\
	x_t3 = _mm_unpacklo_epi16(x_t1, x_t1);	\
	x_t4 = _mm_unpackhi_epi16(x_t1, x_t1);	\
	a0 = _mm_unpacklo_epi8(x_t3, x_zero);	\
	a1 = _mm_unpackhi_epi8(x_t3, x_zero);	\
	a2 = _mm_unpacklo_epi8(x_t4, x_zero);	\
	a3 = _mm_unpackhi_epi8(x_t4, x_zero);	\
}

#define	GetAlpha1b(s0, a0, a1)				\
{	\
	x_t1 = _mm_and_si128(s0, x_mask);		\
	x_t1 = _mm_packs_epi32(x_t1, x_t1);		\
	x_t1 = _mm_packus_epi16(x_t1, x_t1);	\
	x_t1 = _mm_unpacklo_epi8(x_t1, x_t1);	\
	x_t3 = _mm_unpacklo_epi16(x_t1, x_t1);	\
	a0 = _mm_unpacklo_epi8(x_t3, x_zero);	\
	a1 = _mm_unpackhi_epi8(x_t3, x_zero);	\
}

/* a * f1 + b * f2 */
#define	GetDst1a				\
{	\
	/* unpack src */	\
	x_s12 = _mm_unpackhi_epi8(x_s11, x_zero);	\
	x_s11 = _mm_unpacklo_epi8(x_s11, x_zero);	\
	x_s14 = _mm_unpackhi_epi8(x_s13, x_zero);	\
	x_s13 = _mm_unpacklo_epi8(x_s13, x_zero);	\
	x_s22 = _mm_unpackhi_epi8(x_s21, x_zero);	\
	x_s21 = _mm_unpacklo_epi8(x_s21, x_zero);	\
	x_s24 = _mm_unpackhi_epi8(x_s23, x_zero);	\
	x_s23 = _mm_unpacklo_epi8(x_s23, x_zero);	\
	\
	/* blend */			\
	x_t1 = _mm_mullo_epi16(x_s11, x_a11);	\
	x_t2 = _mm_mullo_epi16(x_s21, x_a21);	\
	x_d1 = _mm_adds_epu16(x_t1, x_t2);		\
	x_t1 = _mm_mullo_epi16(x_s12, x_a12);	\
	x_t2 = _mm_mullo_epi16(x_s22, x_a22);	\
	x_d2 = _mm_adds_epu16(x_t1, x_t2);		\
	x_t1 = _mm_mullo_epi16(x_s13, x_a13);	\
	x_t2 = _mm_mullo_epi16(x_s23, x_a23);	\
	x_d3 = _mm_adds_epu16(x_t1, x_t2);		\
	x_t1 = _mm_mullo_epi16(x_s14, x_a14);	\
	x_t2 = _mm_mullo_epi16(x_s24, x_a24);	\
	x_d4 = _mm_adds_epu16(x_t1, x_t2);		\
	\
	x_d1 = _mm_srli_epi16(x_d1, 8);			\
	x_d2 = _mm_srli_epi16(x_d2, 8);			\
	x_d3 = _mm_srli_epi16(x_d3, 8);			\
	x_d4 = _mm_srli_epi16(x_d4, 8);			\
	x_d1 = _mm_packus_epi16(x_d1, x_d2);	\
	x_d1 = _mm_and_si128(x_d1, x_mask1);	\
	x_d3 = _mm_packus_epi16(x_d3, x_d4);	\
	x_d3 = _mm_and_si128(x_d3, x_mask1);	\
}

#define	GetDst1b				\
{	\
	/* unpack src */	\
	x_s12 = _mm_unpackhi_epi8(x_s11, x_zero);	\
	x_s11 = _mm_unpacklo_epi8(x_s11, x_zero);	\
	x_s22 = _mm_unpackhi_epi8(x_s21, x_zero);	\
	x_s21 = _mm_unpacklo_epi8(x_s21, x_zero);	\
	\
	/* blend */			\
	x_t1 = _mm_mullo_epi16(x_s11, x_a11);	\
	x_t2 = _mm_mullo_epi16(x_s21, x_a21);	\
	x_d1 = _mm_adds_epu16(x_t1, x_t2);		\
	x_t1 = _mm_mullo_epi16(x_s12, x_a12);	\
	x_t2 = _mm_mullo_epi16(x_s22, x_a22);	\
	x_d2 = _mm_adds_epu16(x_t1, x_t2);		\
	\
	x_d1 = _mm_srli_epi16(x_d1, 8);			\
	x_d2 = _mm_srli_epi16(x_d2, 8);			\
	x_d1 = _mm_packus_epi16(x_d1, x_d2);	\
	x_d1 = _mm_and_si128(x_d1, x_mask1);	\
}

/* f1 + b * f2 */
#define	GetDst2a				\
{	\
	/* unpack src */	\
	x_s22 = _mm_unpackhi_epi8(x_s21, x_zero);	\
	x_s21 = _mm_unpacklo_epi8(x_s21, x_zero);	\
	x_s24 = _mm_unpackhi_epi8(x_s23, x_zero);	\
	x_s23 = _mm_unpacklo_epi8(x_s23, x_zero);	\
	\
	/* blend */			\
	x_d1 = _mm_mullo_epi16(x_s21, x_a21);	\
	x_d2 = _mm_mullo_epi16(x_s22, x_a22);	\
	x_d3 = _mm_mullo_epi16(x_s23, x_a23);	\
	x_d4 = _mm_mullo_epi16(x_s24, x_a24);	\
	\
	x_d1 = _mm_srli_epi16(x_d1, 8);			\
	x_d2 = _mm_srli_epi16(x_d2, 8);			\
	x_d3 = _mm_srli_epi16(x_d3, 8);			\
	x_d4 = _mm_srli_epi16(x_d4, 8);			\
	x_d1 = _mm_packus_epi16(x_d1, x_d2);	\
	x_d3 = _mm_packus_epi16(x_d3, x_d4);	\
	x_d1 = _mm_adds_epu8(x_d1, x_s11);		\
	x_d1 = _mm_and_si128(x_d1, x_mask1);		\
	x_d3 = _mm_adds_epu8(x_d3, x_s13);		\
	x_d3 = _mm_and_si128(x_d3, x_mask1);	\
}

#define	GetDst2b				\
{	\
	/* unpack src */	\
	x_s22 = _mm_unpackhi_epi8(x_s21, x_zero);	\
	x_s21 = _mm_unpacklo_epi8(x_s21, x_zero);	\
	\
	/* blend */			\
	x_d1 = _mm_mullo_epi16(x_s21, x_a21);	\
	x_d2 = _mm_mullo_epi16(x_s22, x_a22);	\
	\
	x_d1 = _mm_srli_epi16(x_d1, 8);			\
	x_d2 = _mm_srli_epi16(x_d2, 8);			\
	x_d1 = _mm_packus_epi16(x_d1, x_d2);	\
	x_d1 = _mm_adds_epu8(x_d1, x_s11);		\
	x_d1 = _mm_and_si128(x_d1, x_mask1);		\
}

/* b * f2 */
#define	GetDst3a				\
{	\
	/* unpack src */	\
	x_s22 = _mm_unpackhi_epi8(x_s21, x_zero);	\
	x_s21 = _mm_unpacklo_epi8(x_s21, x_zero);	\
	x_s24 = _mm_unpackhi_epi8(x_s23, x_zero);	\
	x_s23 = _mm_unpacklo_epi8(x_s23, x_zero);	\
	\
	/* blend */			\
	x_d1 = _mm_mullo_epi16(x_s21, x_a21);	\
	x_d2 = _mm_mullo_epi16(x_s22, x_a22);	\
	x_d3 = _mm_mullo_epi16(x_s23, x_a23);	\
	x_d4 = _mm_mullo_epi16(x_s24, x_a24);	\
	\
	x_d1 = _mm_srli_epi16(x_d1, 8);			\
	x_d2 = _mm_srli_epi16(x_d2, 8);			\
	x_d3 = _mm_srli_epi16(x_d3, 8);			\
	x_d4 = _mm_srli_epi16(x_d4, 8);			\
	x_d1 = _mm_packus_epi16(x_d1, x_d2);	\
	x_d1 = _mm_and_si128(x_d1, x_mask1);	\
	x_d3 = _mm_packus_epi16(x_d3, x_d4);	\
	x_d3 = _mm_and_si128(x_d3, x_mask1);	\
}

#define	GetDst3b				\
{	\
	/* unpack src */	\
	x_s22 = _mm_unpackhi_epi8(x_s21, x_zero);	\
	x_s21 = _mm_unpacklo_epi8(x_s21, x_zero);	\
	\
	/* blend */			\
	x_d1 = _mm_mullo_epi16(x_s21, x_a21);	\
	x_d2 = _mm_mullo_epi16(x_s22, x_a22);	\
	\
	x_d1 = _mm_srli_epi16(x_d1, 8);			\
	x_d2 = _mm_srli_epi16(x_d2, 8);			\
	x_d1 = _mm_packus_epi16(x_d1, x_d2);	\
	x_d1 = _mm_and_si128(x_d1, x_mask1);	\
}

/* f1 + f2 */
#define	GetDst4a				\
{	\
	x_d1 = _mm_adds_epu8(x_s11, x_s21);	\
	x_d3 = _mm_adds_epu8(x_s13, x_s23);	\
	x_d1 = _mm_and_si128(x_d1, x_mask1);	\
	x_d3 = _mm_and_si128(x_d3, x_mask1);	\
}

#define	GetDst4b				\
{	\
	x_d1 = _mm_adds_epu8(x_s11, x_s21);	\
	x_d1 = _mm_and_si128(x_d1, x_mask1);	\
}

void
mlib_VideoColorBlend_ZERO_SA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	__m128i x_s21, x_s22, x_s23, x_s24;
	__m128i x_a21, x_a22, x_a23, x_a24;
	__m128i x_d1, x_d2, x_d3, x_d4, x_t1, x_t2, x_t3, x_t4;
	__m128i *px_s2, *px_d;
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask = _mm_set1_epi32(0xff);
	const __m128i x_mask1 = _mm_set1_epi32(0xffffff00);
	mlib_u32 val0;
	mlib_s32 v1, v2, v3;
	mlib_f32 a;

	BLEND_VALIDATE_1;
	if ((0 == (0xf & ((mlib_addr)dst | (mlib_addr)src2))) &&
		((0 == (0xf & (dlb | slb2))) | (1 == h))) {
		for (j = 0; j < h; j++) {
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;
				x_s23 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s21, x_s23, x_a21,
					x_a22, x_a23, x_a24);

				/* blend */
				GetDst3a;
				_mm_store_si128(px_d, x_d1);
				px_d++;
				_mm_store_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s21, x_a21, x_a22);

				/* blend */
				GetDst3b;
				_mm_store_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			mlib_u32 *ps2, *pd;

			ps2  = (mlib_u32 *)px_s2;
			pd  = (mlib_u32 *)px_d;
			for (; i < w; i++) {
				val0 = *ps2;
				a = *(mlib_f32 *)((mlib_u8 *)mlib_c_blend_Q8 +
				((val0 << 2) & 0x3FC));
				READ_SRC123;
				*pd = DST_VAL;
				ps2++;
				pd++;
			}

			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;
				x_s23 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s21, x_s23, x_a21,
					x_a22, x_a23, x_a24);

				/* blend */
				GetDst3a;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;
				_mm_storeu_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s21, x_a21, x_a22);

				/* blend */
				GetDst3b;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			mlib_u32 *ps2, *pd;

			ps2  = (mlib_u32 *)px_s2;
			pd  = (mlib_u32 *)px_d;
			for (; i < w; i++) {
				val0 = *ps2;
				a = *(mlib_f32 *)((mlib_u8 *)mlib_c_blend_Q8 +
				((val0 << 2) & 0x3FC));
				READ_SRC123;
				*pd = DST_VAL;
				ps2++;
				pd++;
			}

			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ZERO_OMSA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	__m128i x_s21, x_s22, x_s23, x_s24;
	__m128i x_a21, x_a22, x_a23, x_a24;
	__m128i x_d1, x_d2, x_d3, x_d4, x_t1, x_t2, x_t3, x_t4;
	__m128i *px_s2, *px_d;
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask = _mm_set1_epi32(0xff);
	const __m128i x_mask1 = _mm_set1_epi32(0xffffff00);
	const __m128i x_c = _mm_set1_epi16(0x100);
	mlib_u8 *blend8 = (mlib_u8 *)mlib_c_blend_Q8 + 1024;
	mlib_u32 val0;
	mlib_s32 v1, v2, v3;
	mlib_f32 a;

	BLEND_VALIDATE_1;
	if ((0 == (0xf & ((mlib_addr)dst | (mlib_addr)src2))) &&
		((0 == (0xf & (dlb | slb2))) | (1 == h))) {
		for (j = 0; j < h; j++) {
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;
				x_s23 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s21, x_s23, x_a21,
					x_a22, x_a23, x_a24);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);
				x_a23 = _mm_sub_epi16(x_c, x_a23);
				x_a24 = _mm_sub_epi16(x_c, x_a24);

				/* blend */
				GetDst3a;
				_mm_store_si128(px_d, x_d1);
				px_d++;
				_mm_store_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s21, x_a21, x_a22);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);

				/* blend */
				GetDst3b;
				_mm_store_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			mlib_u32 *ps2, *pd;

			ps2  = (mlib_u32 *)px_s2;
			pd  = (mlib_u32 *)px_d;
			for (; i < w; i++) {
				val0 = *ps2;
				a = *(mlib_f32 *)(blend8 -
					(((val0 << 2) & 0x3FC)));
				READ_SRC123;
				*pd = DST_VAL;
				ps2 ++;
				pd ++;
			}

			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;
				x_s23 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s21, x_s23, x_a21,
					x_a22, x_a23, x_a24);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);
				x_a23 = _mm_sub_epi16(x_c, x_a23);
				x_a24 = _mm_sub_epi16(x_c, x_a24);

				/* blend */
				GetDst3a;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;
				_mm_storeu_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s21, x_a21, x_a22);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);

				/* blend */
				GetDst3b;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			mlib_u32 *ps2, *pd;

			ps2  = (mlib_u32 *)px_s2;
			pd  = (mlib_u32 *)px_d;
			for (; i < w; i++) {
				val0 = *ps2;
				a = *(mlib_f32 *)(blend8 -
					(((val0 << 2) & 0x3FC)));
				READ_SRC123;
				*pd = DST_VAL;
				ps2 ++;
				pd ++;
			}

			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ZERO_DA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	__m128i x_s11, x_s13, x_s21, x_s22, x_s23, x_s24;
	__m128i x_a21, x_a22, x_a23, x_a24;
	__m128i x_d1, x_d2, x_d3, x_d4, x_t1, x_t2, x_t3, x_t4;
	__m128i *px_s1, *px_s2, *px_d;
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask = _mm_set1_epi32(0xff);
	const __m128i x_mask1 = _mm_set1_epi32(0xffffff00);
	mlib_u32 val0, val1;
	mlib_s32 v1, v2, v3;
	mlib_f32 a;

	BLEND_VALIDATE;
	if ((0 == (0xf & ((mlib_addr)dst | (mlib_addr)src1 |
		(mlib_addr)src2))) &&
		((0 == (0xf & (dlb | slb1 | slb2))) | (1 == h))) {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s13 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;
				x_s23 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s11, x_s13, x_a21,
					x_a22, x_a23, x_a24);

				/* blend */
				GetDst3a;
				_mm_store_si128(px_d, x_d1);
				px_d++;
				_mm_store_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s11, x_a21, x_a22);

				/* blend */
				GetDst3b;
				_mm_store_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			mlib_u8 *ps1;
			mlib_u32 *ps2, *pd;

			ps1  = (mlib_u8 *)px_s1;
			ps2  = (mlib_u32 *)px_s2;
			pd  = (mlib_u32 *)px_d;
			for (; i < w; i++) {
				val1 = *ps1;
				val0 = *ps2;
				a = *(mlib_f32 *)((mlib_u8 *)mlib_c_blend_Q8 +
					(val1 << 2));
				READ_SRC123;
				*pd = DST_VAL;

				ps1 += 4;
				ps2++;
				pd++;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s13 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;
				x_s23 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s11, x_s13, x_a21,
					x_a22, x_a23, x_a24);

				/* blend */
				GetDst3a;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;
				_mm_storeu_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s11, x_a21, x_a22);

				/* blend */
				GetDst3b;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			mlib_u8 *ps1;
			mlib_u32 *ps2, *pd;

			ps1  = (mlib_u8 *)px_s1;
			ps2  = (mlib_u32 *)px_s2;
			pd  = (mlib_u32 *)px_d;
			for (; i < w; i++) {
				val1 = *ps1;
				val0 = *ps2;
				a = *(mlib_f32 *)((mlib_u8 *)mlib_c_blend_Q8 +
					(val1 << 2));
				READ_SRC123;
				*pd = DST_VAL;

				ps1 += 4;
				ps2++;
				pd++;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ZERO_OMDA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	__m128i x_s11, x_s13, x_s21, x_s22, x_s23, x_s24;
	__m128i x_a21, x_a22, x_a23, x_a24;
	__m128i x_d1, x_d2, x_d3, x_d4, x_t1, x_t2, x_t3, x_t4;
	__m128i *px_s1, *px_s2, *px_d;
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask = _mm_set1_epi32(0xff);
	const __m128i x_mask1 = _mm_set1_epi32(0xffffff00);
	const __m128i x_c = _mm_set1_epi16(0x100);
	mlib_u8 *blend8 = (mlib_u8 *)mlib_c_blend_Q8 + 1024;
	mlib_u32 val0, val1;
	mlib_s32 v1, v2, v3;
	mlib_f32 a;

	BLEND_VALIDATE;
	if ((0 == (0xf & ((mlib_addr)dst | (mlib_addr)src1 |
		(mlib_addr)src2))) &&
		((0 == (0xf & (dlb | slb1 | slb2))) | (1 == h))) {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s13 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;
				x_s23 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s11, x_s13, x_a21,
					x_a22, x_a23, x_a24);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);
				x_a23 = _mm_sub_epi16(x_c, x_a23);
				x_a24 = _mm_sub_epi16(x_c, x_a24);

				/* blend */
				GetDst3a;
				_mm_store_si128(px_d, x_d1);
				px_d++;
				_mm_store_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s11, x_a21, x_a22);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);

				/* blend */
				GetDst3b;
				_mm_store_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			mlib_u8 *ps1;
			mlib_u32 *ps2, *pd;

			ps1  = (mlib_u8 *)px_s1;
			ps2  = (mlib_u32 *)px_s2;
			pd  = (mlib_u32 *)px_d;
			for (; i < w; i++) {
				val1 = *ps1;
				val0 = *ps2;
				a = *(mlib_f32 *)(blend8 - (val1 << 2));
				READ_SRC123;
				*pd = DST_VAL;

				ps1 += 4;
				ps2++;
				pd++;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s13 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;
				x_s23 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s11, x_s13, x_a21,
					x_a22, x_a23, x_a24);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);
				x_a23 = _mm_sub_epi16(x_c, x_a23);
				x_a24 = _mm_sub_epi16(x_c, x_a24);

				/* blend */
				GetDst3a;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;
				_mm_storeu_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s11, x_a21, x_a22);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);

				/* blend */
				GetDst3b;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			mlib_u8 *ps1;
			mlib_u32 *ps2, *pd;

			ps1  = (mlib_u8 *)px_s1;
			ps2  = (mlib_u32 *)px_s2;
			pd  = (mlib_u32 *)px_d;
			for (; i < w; i++) {
				val1 = *ps1;
				val0 = *ps2;
				a = *(mlib_f32 *)(blend8 - (val1 << 2));
				READ_SRC123;
				*pd = DST_VAL;

				ps1 += 4;
				ps2++;
				pd++;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ONE_ONE_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	__m128i x_s11, x_s13, x_s21, x_s23;
	__m128i x_d1, x_d3;
	__m128i *px_s1, *px_s2, *px_d;
	const __m128i x_mask1 = _mm_set1_epi32(0xffffff00);

	BLEND_VALIDATE;
	if ((0 == (0xf & ((mlib_addr)dst | (mlib_addr)src1 |
		(mlib_addr)src2))) &&
		((0 == (0xf & (dlb | slb1 | slb2))) | (1 == h))) {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s13 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;
				x_s23 = _mm_load_si128(px_s2);
				px_s2++;

				/* blend */
				GetDst4a;
				_mm_store_si128(px_d, x_d1);
				px_d++;
				_mm_store_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;

				/* blend */
				GetDst4b;
				_mm_store_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			mlib_u8 *ps1, *ps2, *pd;

			ps1  = (mlib_u8 *)px_s1;
			ps2  = (mlib_u8 *)px_s2;
			pd  = (mlib_u8 *)px_d;
			for (; i < w; i++) {
				mlib_s32 v1, v2, v3, z1, z2, z3;
				mlib_s32 s1 = ps1[1];
				mlib_s32 s2 = ps2[1];
				mlib_s32 s3 = ps1[2];
				mlib_s32 s4 = ps2[2];
				mlib_s32 s5 = ps1[3];
				mlib_s32 s6 = ps2[3];
				v1 = s1 + s2;
				v2 = s3 + s4;
				v3 = s5 + s6;
				z1 = (v1 << 23) >> 31;
				z2 = (v2 << 23) >> 31;
				z3 = (v3 << 23) >> 31;
				pd[0] = 0;
				pd[1] = v1 | z1;
				pd[2] = v2 | z2;
				pd[3] = v3 | z3;

				ps1 += 4;
				ps2 += 4;
				pd += 4;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s13 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;
				x_s23 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* blend */
				GetDst4a;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;
				_mm_storeu_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* blend */
				GetDst4b;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			mlib_u8 *ps1, *ps2, *pd;

			ps1  = (mlib_u8 *)px_s1;
			ps2  = (mlib_u8 *)px_s2;
			pd  = (mlib_u8 *)px_d;
			for (; i < w; i++) {
				mlib_s32 v1, v2, v3, z1, z2, z3;
				mlib_s32 s1 = ps1[1];
				mlib_s32 s2 = ps2[1];
				mlib_s32 s3 = ps1[2];
				mlib_s32 s4 = ps2[2];
				mlib_s32 s5 = ps1[3];
				mlib_s32 s6 = ps2[3];
				v1 = s1 + s2;
				v2 = s3 + s4;
				v3 = s5 + s6;
				z1 = (v1 << 23) >> 31;
				z2 = (v2 << 23) >> 31;
				z3 = (v3 << 23) >> 31;
				pd[0] = 0;
				pd[1] = v1 | z1;
				pd[2] = v2 | z2;
				pd[3] = v3 | z3;

				ps1 += 4;
				ps2 += 4;
				pd += 4;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

#define	BLEND8	(mlib_u8*)mlib_c_blend_u8_sat

#define	GET_ALPHA_P(VAL)	((VAL << 2) & 0x3FC)

#define	ADD_F123                                                   \
	f1 = *(mlib_f32 *)(BLEND8 + ((val2 >> 6) & 0x3FC)) * a;    \
	f2 = *(mlib_f32 *)(BLEND8 + ((val2 >> 14) & 0x3FC)) * a;   \
	f3 = *(mlib_f32 *)(BLEND8 + ((val2 >> 22) & 0x3FC)) * a;   \
	f1 += *(mlib_f32 *)(BLEND8 + ((val1 >> 6) & 0x3FC));       \
	f2 += *(mlib_f32 *)(BLEND8 + ((val1 >> 14) & 0x3FC));      \
	f3 += *(mlib_f32 *)(BLEND8 + ((val1 >> 22) & 0x3FC))

#define	ADD_F123_SA                                                 \
	f1 = *(mlib_f32 *)(BLEND8 + ((val2 >> 6) & 0x3FC)) * a1;    \
	f2 = *(mlib_f32 *)(BLEND8 + ((val2 >> 14) & 0x3FC)) * a1;   \
	f3 = *(mlib_f32 *)(BLEND8 + ((val2 >> 22) & 0x3FC)) * a1;   \
	f1 += *(mlib_f32 *)(BLEND8 + ((val1 >> 6) & 0x3FC)) * a;    \
	f2 += *(mlib_f32 *)(BLEND8 + ((val1 >> 14) & 0x3FC)) * a;   \
	f3 += *(mlib_f32 *)(BLEND8 + ((val1 >> 22) & 0x3FC)) * a

#define	CLAMP_V123                                              \
	v1 = (FLOAT2INT_CLAMP(f1) >> 15) & 0xFF00;              \
	v2 = (FLOAT2INT_CLAMP(f2) >> 7) & 0xFF0000;             \
	v3 = (FLOAT2INT_CLAMP(f3) << 1) & 0xFF000000

/* *********************************************************** */

void
mlib_VideoColorBlend_ONE_SA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	__m128i x_s11, x_s13, x_s21, x_s22, x_s23, x_s24;
	__m128i x_a21, x_a22, x_a23, x_a24;
	__m128i x_d1, x_d2, x_d3, x_d4, x_t1, x_t2, x_t3, x_t4;
	__m128i *px_s1, *px_s2, *px_d;
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask = _mm_set1_epi32(0xff);
	const __m128i x_mask1 = _mm_set1_epi32(0xffffff00);

	BLEND_VALIDATE;

	if ((0 == (0xf & ((mlib_addr)dst | (mlib_addr)src1 |
		(mlib_addr)src2))) &&
		((0 == (0xf & (dlb | slb1 | slb2))) | (1 == h))) {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s13 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;
				x_s23 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s21, x_s23, x_a21,
					x_a22, x_a23, x_a24);

				/* blend */
				GetDst2a;
				_mm_store_si128(px_d, x_d1);
				px_d++;
				_mm_store_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s21, x_a21, x_a22);

				/* blend */
				GetDst2b;
				_mm_store_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a;

				a = *(mlib_f32 *)((mlib_u8 *)mlib_c_blend_Q8 +
					GET_ALPHA_P(val2));
				ADD_F123;
				CLAMP_V123;
				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s13 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;
				x_s23 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s21, x_s23, x_a21,
					x_a22, x_a23, x_a24);

				/* blend */
				GetDst2a;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;
				_mm_storeu_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s21, x_a21, x_a22);

				/* blend */
				GetDst2b;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a;

				a = *(mlib_f32 *)((mlib_u8 *)mlib_c_blend_Q8 +
					GET_ALPHA_P(val2));
				ADD_F123;
				CLAMP_V123;
				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ONE_OMSA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	__m128i x_s11, x_s13, x_s21, x_s22, x_s23, x_s24;
	__m128i x_a21, x_a22, x_a23, x_a24;
	__m128i x_d1, x_d2, x_d3, x_d4, x_t1, x_t2, x_t3, x_t4;
	__m128i *px_s1, *px_s2, *px_d;
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask = _mm_set1_epi32(0xff);
	const __m128i x_mask1 = _mm_set1_epi32(0xffffff00);
	const __m128i x_c = _mm_set1_epi16(0x100);
	mlib_u8 *blend8 = (mlib_u8 *)mlib_c_blend_Q8 + 1024;

	BLEND_VALIDATE;

	if ((0 == (0xf & ((mlib_addr)dst | (mlib_addr)src1 |
		(mlib_addr)src2))) &&
		((0 == (0xf & (dlb | slb1 | slb2))) | (1 == h))) {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s13 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;
				x_s23 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s21, x_s23, x_a21,
					x_a22, x_a23, x_a24);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);
				x_a23 = _mm_sub_epi16(x_c, x_a23);
				x_a24 = _mm_sub_epi16(x_c, x_a24);

				/* blend */
				GetDst2a;
				_mm_store_si128(px_d, x_d1);
				px_d++;
				_mm_store_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s21, x_a21, x_a22);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);

				/* blend */
				GetDst2b;
				_mm_store_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a;

				a = *(mlib_f32 *)(blend8 - GET_ALPHA_P(val2));
				ADD_F123;
				CLAMP_V123;
				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s13 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;
				x_s23 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s21, x_s23, x_a21,
					x_a22, x_a23, x_a24);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);
				x_a23 = _mm_sub_epi16(x_c, x_a23);
				x_a24 = _mm_sub_epi16(x_c, x_a24);

				/* blend */
				GetDst2a;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;
				_mm_storeu_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s21, x_a21, x_a22);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);

				/* blend */
				GetDst2b;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a;

				a = *(mlib_f32 *)(blend8 - GET_ALPHA_P(val2));
				ADD_F123;
				CLAMP_V123;
				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ONE_DA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	__m128i x_s11, x_s13, x_s21, x_s22, x_s23, x_s24;
	__m128i x_a21, x_a22, x_a23, x_a24;
	__m128i x_d1, x_d2, x_d3, x_d4, x_t1, x_t2, x_t3, x_t4;
	__m128i *px_s1, *px_s2, *px_d;
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask = _mm_set1_epi32(0xff);
	const __m128i x_mask1 = _mm_set1_epi32(0xffffff00);

	BLEND_VALIDATE;

	if ((0 == (0xf & ((mlib_addr)dst | (mlib_addr)src1 |
		(mlib_addr)src2))) &&
		((0 == (0xf & (dlb | slb1 | slb2))) | (1 == h))) {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s13 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;
				x_s23 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s11, x_s13, x_a21,
					x_a22, x_a23, x_a24);

				/* blend */
				GetDst2a;
				_mm_store_si128(px_d, x_d1);
				px_d++;
				_mm_store_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s11, x_a21, x_a22);

				/* blend */
				GetDst2b;
				_mm_store_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a;

				a = *(mlib_f32 *)((mlib_u8 *)mlib_c_blend_Q8 +
					GET_ALPHA_P(val1));
				ADD_F123;
				CLAMP_V123;
				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s13 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;
				x_s23 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s11, x_s13, x_a21,
					x_a22, x_a23, x_a24);

				/* blend */
				GetDst2a;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;
				_mm_storeu_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s11, x_a21, x_a22);

				/* blend */
				GetDst2b;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a;

				a = *(mlib_f32 *)((mlib_u8 *)mlib_c_blend_Q8 +
					GET_ALPHA_P(val1));
				ADD_F123;
				CLAMP_V123;
				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ONE_OMDA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	__m128i x_s11, x_s13, x_s21, x_s22, x_s23, x_s24;
	__m128i x_a21, x_a22, x_a23, x_a24;
	__m128i x_d1, x_d2, x_d3, x_d4, x_t1, x_t2, x_t3, x_t4;
	__m128i *px_s1, *px_s2, *px_d;
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask = _mm_set1_epi32(0xff);
	const __m128i x_mask1 = _mm_set1_epi32(0xffffff00);
	const __m128i x_c = _mm_set1_epi16(0x100);
	mlib_u8 *blend8 = (mlib_u8 *)mlib_c_blend_Q8 + 1024;

	BLEND_VALIDATE;

	if ((0 == (0xf & ((mlib_addr)dst | (mlib_addr)src1 |
		(mlib_addr)src2))) &&
		((0 == (0xf & (dlb | slb1 | slb2))) | (1 == h))) {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s13 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;
				x_s23 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s11, x_s13, x_a21,
					x_a22, x_a23, x_a24);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);
				x_a23 = _mm_sub_epi16(x_c, x_a23);
				x_a24 = _mm_sub_epi16(x_c, x_a24);

				/* blend */
				GetDst2a;
				_mm_store_si128(px_d, x_d1);
				px_d++;
				_mm_store_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s11, x_a21, x_a22);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);

				/* blend */
				GetDst2b;
				_mm_store_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a;

				a = *(mlib_f32 *)(blend8 - GET_ALPHA_P(val1));

				ADD_F123;
				CLAMP_V123;

				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s13 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;
				x_s23 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s11, x_s13, x_a21,
					x_a22, x_a23, x_a24);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);
				x_a23 = _mm_sub_epi16(x_c, x_a23);
				x_a24 = _mm_sub_epi16(x_c, x_a24);

				/* blend */
				GetDst2a;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;
				_mm_storeu_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s11, x_a21, x_a22);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);

				/* blend */
				GetDst2b;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a;

				a = *(mlib_f32 *)(blend8 - GET_ALPHA_P(val1));

				ADD_F123;
				CLAMP_V123;

				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_SA_SA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	__m128i x_s11, x_s12, x_s13, x_s14, x_s21, x_s22, x_s23, x_s24;
	__m128i x_a11, x_a12, x_a13, x_a14,  x_a21, x_a22, x_a23, x_a24;
	__m128i x_d1, x_d2, x_d3, x_d4, x_t1, x_t2, x_t3, x_t4;
	__m128i *px_s1, *px_s2, *px_d;
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask = _mm_set1_epi32(0xff);
	const __m128i x_mask1 = _mm_set1_epi32(0xffffff00);

	BLEND_VALIDATE;
	if ((0 == (0xf & ((mlib_addr)dst | (mlib_addr)src1 |
		(mlib_addr)src2))) &&
		((0 == (0xf & (dlb | slb1 | slb2))) | (1 == h))) {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s13 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;
				x_s23 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s21, x_s23, x_a21,
					x_a22, x_a23, x_a24);
				x_a11 = x_a21;
				x_a12 = x_a22;
				x_a13 = x_a23;
				x_a14 = x_a24;

				/* blend */
				GetDst1a;
				_mm_store_si128(px_d, x_d1);
				px_d++;
				_mm_store_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s21, x_a21, x_a22);
				x_a11 = x_a21;
				x_a12 = x_a22;

				/* blend */
				GetDst1b;
				_mm_store_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a, a1;

				a = *(mlib_f32 *)((mlib_u8 *)mlib_c_blend_Q8 +
					GET_ALPHA_P(val2));
				a1 = a;
				ADD_F123_SA;
				CLAMP_V123;
				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s13 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;
				x_s23 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s21, x_s23, x_a21,
					x_a22, x_a23, x_a24);
				x_a11 = x_a21;
				x_a12 = x_a22;
				x_a13 = x_a23;
				x_a14 = x_a24;

				/* blend */
				GetDst1a;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;
				_mm_storeu_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s21, x_a21, x_a22);
				x_a11 = x_a21;
				x_a12 = x_a22;

				/* blend */
				GetDst1b;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a, a1;

				a = *(mlib_f32 *)((mlib_u8 *)mlib_c_blend_Q8 +
					GET_ALPHA_P(val2));
				a1 = a;
				ADD_F123_SA;
				CLAMP_V123;
				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_SA_OMSA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	__m128i x_s11, x_s12, x_s13, x_s14, x_s21, x_s22, x_s23, x_s24;
	__m128i x_a11, x_a12, x_a13, x_a14,  x_a21, x_a22, x_a23, x_a24;
	__m128i x_d1, x_d2, x_d3, x_d4, x_t1, x_t2, x_t3, x_t4;
	__m128i *px_s1, *px_s2, *px_d;
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask = _mm_set1_epi32(0xff);
	const __m128i x_mask1 = _mm_set1_epi32(0xffffff00);
	const __m128i x_c = _mm_set1_epi16(0x100);

	BLEND_VALIDATE;
	if ((0 == (0xf & ((mlib_addr)dst | (mlib_addr)src1 |
		(mlib_addr)src2))) &&
		((0 == (0xf & (dlb | slb1 | slb2))) | (1 == h))) {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s13 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;
				x_s23 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s21, x_s23, x_a11,
					x_a12, x_a13, x_a14);
				x_a21 = _mm_sub_epi16(x_c, x_a11);
				x_a22 = _mm_sub_epi16(x_c, x_a12);
				x_a23 = _mm_sub_epi16(x_c, x_a13);
				x_a24 = _mm_sub_epi16(x_c, x_a14);

				/* blend */
				GetDst1a;
				_mm_store_si128(px_d, x_d1);
				px_d++;
				_mm_store_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s21, x_a11, x_a12);
				x_a21 = _mm_sub_epi16(x_c, x_a11);
				x_a22 = _mm_sub_epi16(x_c, x_a12);

				/* blend */
				GetDst1b;
				_mm_store_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a, a1;

				a = *(mlib_f32 *)((mlib_u8 *)mlib_c_blend_Q8 +
					GET_ALPHA_P(val2));
				a1 = 1.0f - a;
				ADD_F123_SA;
				CLAMP_V123;
				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s13 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;
				x_s23 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s21, x_s23, x_a11,
					x_a12, x_a13, x_a14);
				x_a21 = _mm_sub_epi16(x_c, x_a11);
				x_a22 = _mm_sub_epi16(x_c, x_a12);
				x_a23 = _mm_sub_epi16(x_c, x_a13);
				x_a24 = _mm_sub_epi16(x_c, x_a14);

				/* blend */
				GetDst1a;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;
				_mm_storeu_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s21, x_a11, x_a12);
				x_a21 = _mm_sub_epi16(x_c, x_a11);
				x_a22 = _mm_sub_epi16(x_c, x_a12);

				/* blend */
				GetDst1b;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a, a1;

				a = *(mlib_f32 *)((mlib_u8 *)mlib_c_blend_Q8 +
					GET_ALPHA_P(val2));
				a1 = 1.0f - a;
				ADD_F123_SA;
				CLAMP_V123;
				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_SA_DA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	__m128i x_s11, x_s12, x_s13, x_s14, x_s21, x_s22, x_s23, x_s24;
	__m128i x_a11, x_a12, x_a13, x_a14,  x_a21, x_a22, x_a23, x_a24;
	__m128i x_d1, x_d2, x_d3, x_d4, x_t1, x_t2, x_t3, x_t4;
	__m128i *px_s1, *px_s2, *px_d;
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask = _mm_set1_epi32(0xff);
	const __m128i x_mask1 = _mm_set1_epi32(0xffffff00);

	BLEND_VALIDATE;
	if ((0 == (0xf & ((mlib_addr)dst | (mlib_addr)src1 |
		(mlib_addr)src2))) &&
		((0 == (0xf & (dlb | slb1 | slb2))) | (1 == h))) {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s13 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;
				x_s23 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s21, x_s23, x_a11,
					x_a12, x_a13, x_a14);
				GetAlpha1a(x_s11, x_s13, x_a21,
					x_a22, x_a23, x_a24);

				/* blend */
				GetDst1a;
				_mm_store_si128(px_d, x_d1);
				px_d++;
				_mm_store_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s21, x_a11, x_a12);
				GetAlpha1b(x_s11, x_a21, x_a22);

				/* blend */
				GetDst1b;
				_mm_store_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a, a1;

				a = *(mlib_f32 *)((mlib_u8 *)mlib_c_blend_Q8 +
					GET_ALPHA_P(val2));
				a1 = *(mlib_f32 *)((mlib_u8 *)mlib_c_blend_Q8 +
					GET_ALPHA_P(val1));
				ADD_F123_SA;
				CLAMP_V123;
				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s13 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;
				x_s23 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s21, x_s23, x_a11,
					x_a12, x_a13, x_a14);
				GetAlpha1a(x_s11, x_s13, x_a21,
					x_a22, x_a23, x_a24);

				/* blend */
				GetDst1a;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;
				_mm_storeu_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s21, x_a11, x_a12);
				GetAlpha1b(x_s11, x_a21, x_a22);

				/* blend */
				GetDst1b;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a, a1;

				a = *(mlib_f32 *)((mlib_u8 *)mlib_c_blend_Q8 +
					GET_ALPHA_P(val2));
				a1 = *(mlib_f32 *)((mlib_u8 *)mlib_c_blend_Q8 +
					GET_ALPHA_P(val1));
				ADD_F123_SA;
				CLAMP_V123;
				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_SA_OMDA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	__m128i x_s11, x_s12, x_s13, x_s14, x_s21, x_s22, x_s23, x_s24;
	__m128i x_a11, x_a12, x_a13, x_a14,  x_a21, x_a22, x_a23, x_a24;
	__m128i x_d1, x_d2, x_d3, x_d4, x_t1, x_t2, x_t3, x_t4;
	__m128i *px_s1, *px_s2, *px_d;
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask = _mm_set1_epi32(0xff);
	const __m128i x_mask1 = _mm_set1_epi32(0xffffff00);
	const __m128i x_c = _mm_set1_epi16(0x100);
	mlib_u8 *blend8 = (mlib_u8 *)mlib_c_blend_Q8 + 1024;

	BLEND_VALIDATE;
	if ((0 == (0xf & ((mlib_addr)dst | (mlib_addr)src1 |
		(mlib_addr)src2))) &&
		((0 == (0xf & (dlb | slb1 | slb2))) | (1 == h))) {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s13 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;
				x_s23 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s21, x_s23, x_a11,
					x_a12, x_a13, x_a14);
				GetAlpha1a(x_s11, x_s13, x_a21,
					x_a22, x_a23, x_a24);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);
				x_a23 = _mm_sub_epi16(x_c, x_a23);
				x_a24 = _mm_sub_epi16(x_c, x_a24);

				/* blend */
				GetDst1a;
				_mm_store_si128(px_d, x_d1);
				px_d++;
				_mm_store_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s21, x_a11, x_a12);
				GetAlpha1b(x_s11, x_a21, x_a22);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);

				/* blend */
				GetDst1b;
				_mm_store_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a, a1;

				a = *(mlib_f32 *)((mlib_u8 *)mlib_c_blend_Q8 +
					GET_ALPHA_P(val2));
				a1 = *(mlib_f32 *)((mlib_u8 *)blend8 -
					GET_ALPHA_P(val1));
				ADD_F123_SA;
				CLAMP_V123;
				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s13 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;
				x_s23 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s21, x_s23, x_a11,
					x_a12, x_a13, x_a14);
				GetAlpha1a(x_s11, x_s13, x_a21,
					x_a22, x_a23, x_a24);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);
				x_a23 = _mm_sub_epi16(x_c, x_a23);
				x_a24 = _mm_sub_epi16(x_c, x_a24);

				/* blend */
				GetDst1a;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;
				_mm_storeu_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s21, x_a11, x_a12);
				GetAlpha1b(x_s11, x_a21, x_a22);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);

				/* blend */
				GetDst1b;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a, a1;

				a = *(mlib_f32 *)((mlib_u8 *)mlib_c_blend_Q8 +
					GET_ALPHA_P(val2));
				a1 = *(mlib_f32 *)((mlib_u8 *)blend8 -
					GET_ALPHA_P(val1));
				ADD_F123_SA;
				CLAMP_V123;
				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMSA_SA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	__m128i x_s11, x_s12, x_s13, x_s14, x_s21, x_s22, x_s23, x_s24;
	__m128i x_a11, x_a12, x_a13, x_a14,  x_a21, x_a22, x_a23, x_a24;
	__m128i x_d1, x_d2, x_d3, x_d4, x_t1, x_t2, x_t3, x_t4;
	__m128i *px_s1, *px_s2, *px_d;
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask = _mm_set1_epi32(0xff);
	const __m128i x_mask1 = _mm_set1_epi32(0xffffff00);
	const __m128i x_c = _mm_set1_epi16(0x100);

	BLEND_VALIDATE;
	if ((0 == (0xf & ((mlib_addr)dst | (mlib_addr)src1 |
		(mlib_addr)src2))) &&
		((0 == (0xf & (dlb | slb1 | slb2))) | (1 == h))) {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s13 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;
				x_s23 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s21, x_s23, x_a21,
					x_a22, x_a23, x_a24);
				x_a11 = _mm_sub_epi16(x_c, x_a21);
				x_a12 = _mm_sub_epi16(x_c, x_a22);
				x_a13 = _mm_sub_epi16(x_c, x_a23);
				x_a14 = _mm_sub_epi16(x_c, x_a24);

				/* blend */
				GetDst1a;
				_mm_store_si128(px_d, x_d1);
				px_d++;
				_mm_store_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s21, x_a21, x_a22);
				x_a11 = _mm_sub_epi16(x_c, x_a21);
				x_a12 = _mm_sub_epi16(x_c, x_a22);

				/* blend */
				GetDst1b;
				_mm_store_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a, a1;

				a1 = *(mlib_f32 *)((mlib_u8 *)mlib_c_blend_Q8 +
					GET_ALPHA_P(val2));
				a = 1.0f - a1;
				ADD_F123_SA;
				CLAMP_V123;
				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s13 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;
				x_s23 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s21, x_s23, x_a21,
					x_a22, x_a23, x_a24);
				x_a11 = _mm_sub_epi16(x_c, x_a21);
				x_a12 = _mm_sub_epi16(x_c, x_a22);
				x_a13 = _mm_sub_epi16(x_c, x_a23);
				x_a14 = _mm_sub_epi16(x_c, x_a24);

				/* blend */
				GetDst1a;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;
				_mm_storeu_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s21, x_a21, x_a22);
				x_a11 = _mm_sub_epi16(x_c, x_a21);
				x_a12 = _mm_sub_epi16(x_c, x_a22);

				/* blend */
				GetDst1b;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a, a1;

				a1 = *(mlib_f32 *)((mlib_u8 *)mlib_c_blend_Q8 +
					GET_ALPHA_P(val2));
				a = 1.0f - a1;
				ADD_F123_SA;
				CLAMP_V123;
				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMSA_OMSA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	__m128i x_s11, x_s12, x_s13, x_s14, x_s21, x_s22, x_s23, x_s24;
	__m128i x_a11, x_a12, x_a13, x_a14,  x_a21, x_a22, x_a23, x_a24;
	__m128i x_d1, x_d2, x_d3, x_d4, x_t1, x_t2, x_t3, x_t4;
	__m128i *px_s1, *px_s2, *px_d;
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask = _mm_set1_epi32(0xff);
	const __m128i x_mask1 = _mm_set1_epi32(0xffffff00);
	const __m128i x_c = _mm_set1_epi16(0x100);
	mlib_u8 *blend8 = (mlib_u8 *)mlib_c_blend_Q8 + 1024;

	BLEND_VALIDATE;
	if ((0 == (0xf & ((mlib_addr)dst | (mlib_addr)src1 |
		(mlib_addr)src2))) &&
		((0 == (0xf & (dlb | slb1 | slb2))) | (1 == h))) {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s13 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;
				x_s23 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s21, x_s23, x_a21,
					x_a22, x_a23, x_a24);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);
				x_a23 = _mm_sub_epi16(x_c, x_a23);
				x_a24 = _mm_sub_epi16(x_c, x_a24);
				x_a11 = x_a21;
				x_a12 = x_a22;
				x_a13 = x_a23;
				x_a14 = x_a24;

				/* blend */
				GetDst1a;
				_mm_store_si128(px_d, x_d1);
				px_d++;
				_mm_store_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s21, x_a21, x_a22);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);
				x_a11 = x_a21;
				x_a12 = x_a22;

				/* blend */
				GetDst1b;
				_mm_store_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a, a1;

				a = *(mlib_f32 *)((mlib_u8 *)blend8 -
					GET_ALPHA_P(val2));
				a1 = a;
				ADD_F123_SA;
				CLAMP_V123;
				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;

			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s13 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;
				x_s23 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s21, x_s23, x_a21,
					x_a22, x_a23, x_a24);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);
				x_a23 = _mm_sub_epi16(x_c, x_a23);
				x_a24 = _mm_sub_epi16(x_c, x_a24);
				x_a11 = x_a21;
				x_a12 = x_a22;
				x_a13 = x_a23;
				x_a14 = x_a24;

				/* blend */
				GetDst1a;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;
				_mm_storeu_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s21, x_a21, x_a22);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);
				x_a11 = x_a21;
				x_a12 = x_a22;

				/* blend */
				GetDst1b;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a, a1;

				a = *(mlib_f32 *)((mlib_u8 *)blend8 -
					GET_ALPHA_P(val2));
				a1 = a;
				ADD_F123_SA;
				CLAMP_V123;
				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;

			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMSA_OMDA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	__m128i x_s11, x_s12, x_s13, x_s14, x_s21, x_s22, x_s23, x_s24;
	__m128i x_a11, x_a12, x_a13, x_a14,  x_a21, x_a22, x_a23, x_a24;
	__m128i x_d1, x_d2, x_d3, x_d4, x_t1, x_t2, x_t3, x_t4;
	__m128i *px_s1, *px_s2, *px_d;
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask = _mm_set1_epi32(0xff);
	const __m128i x_mask1 = _mm_set1_epi32(0xffffff00);
	const __m128i x_c = _mm_set1_epi16(0x100);
	mlib_u8 *blend8 = (mlib_u8 *)mlib_c_blend_Q8 + 1024;

	BLEND_VALIDATE;
	if ((0 == (0xf & ((mlib_addr)dst | (mlib_addr)src1 |
		(mlib_addr)src2))) &&
		((0 == (0xf & (dlb | slb1 | slb2))) | (1 == h))) {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s13 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;
				x_s23 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s21, x_s23, x_a11,
					x_a12, x_a13, x_a14);
				GetAlpha1a(x_s11, x_s13, x_a21,
					x_a22, x_a23, x_a24);
				x_a11 = _mm_sub_epi16(x_c, x_a11);
				x_a12 = _mm_sub_epi16(x_c, x_a12);
				x_a13 = _mm_sub_epi16(x_c, x_a13);
				x_a14 = _mm_sub_epi16(x_c, x_a14);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);
				x_a23 = _mm_sub_epi16(x_c, x_a23);
				x_a24 = _mm_sub_epi16(x_c, x_a24);

				/* blend */
				GetDst1a;
				_mm_store_si128(px_d, x_d1);
				px_d++;
				_mm_store_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s21, x_a11, x_a12);
				GetAlpha1b(x_s11, x_a21, x_a22);
				x_a11 = _mm_sub_epi16(x_c, x_a11);
				x_a12 = _mm_sub_epi16(x_c, x_a12);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);

				/* blend */
				GetDst1b;
				_mm_store_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a, a1;

				a = *(mlib_f32 *)((mlib_u8 *)blend8 -
					GET_ALPHA_P(val2));
				a1 = *(mlib_f32 *)((mlib_u8 *)blend8 -
					GET_ALPHA_P(val1));
				ADD_F123_SA;
				CLAMP_V123;
				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s13 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;
				x_s23 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s21, x_s23, x_a11,
					x_a12, x_a13, x_a14);
				GetAlpha1a(x_s11, x_s13, x_a21,
					x_a22, x_a23, x_a24);
				x_a11 = _mm_sub_epi16(x_c, x_a11);
				x_a12 = _mm_sub_epi16(x_c, x_a12);
				x_a13 = _mm_sub_epi16(x_c, x_a13);
				x_a14 = _mm_sub_epi16(x_c, x_a14);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);
				x_a23 = _mm_sub_epi16(x_c, x_a23);
				x_a24 = _mm_sub_epi16(x_c, x_a24);

				/* blend */
				GetDst1a;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;
				_mm_storeu_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s21, x_a11, x_a12);
				GetAlpha1b(x_s11, x_a21, x_a22);
				x_a11 = _mm_sub_epi16(x_c, x_a11);
				x_a12 = _mm_sub_epi16(x_c, x_a12);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);

				/* blend */
				GetDst1b;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a, a1;

				a = *(mlib_f32 *)((mlib_u8 *)blend8 -
					GET_ALPHA_P(val2));
				a1 = *(mlib_f32 *)((mlib_u8 *)blend8 -
					GET_ALPHA_P(val1));
				ADD_F123_SA;
				CLAMP_V123;
				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_DA_SA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	__m128i x_s11, x_s12, x_s13, x_s14, x_s21, x_s22, x_s23, x_s24;
	__m128i x_a11, x_a12, x_a13, x_a14,  x_a21, x_a22, x_a23, x_a24;
	__m128i x_d1, x_d2, x_d3, x_d4, x_t1, x_t2, x_t3, x_t4;
	__m128i *px_s1, *px_s2, *px_d;
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask = _mm_set1_epi32(0xff);
	const __m128i x_mask1 = _mm_set1_epi32(0xffffff00);

	BLEND_VALIDATE;
	if ((0 == (0xf & ((mlib_addr)dst | (mlib_addr)src1 |
		(mlib_addr)src2))) &&
		((0 == (0xf & (dlb | slb1 | slb2))) | (1 == h))) {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s13 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;
				x_s23 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s11, x_s13, x_a11,
					x_a12, x_a13, x_a14);
				GetAlpha1a(x_s21, x_s23, x_a21,
					x_a22, x_a23, x_a24);

				/* blend */
				GetDst1a;
				_mm_store_si128(px_d, x_d1);
				px_d++;
				_mm_store_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s11, x_a11, x_a12);
				GetAlpha1b(x_s21, x_a21, x_a22);

				/* blend */
				GetDst1b;
				_mm_store_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a, a1;

				a = *(mlib_f32 *)((mlib_u8 *)mlib_c_blend_Q8 +
					GET_ALPHA_P(val1));
				a1 = *(mlib_f32 *)((mlib_u8 *)mlib_c_blend_Q8 +
					GET_ALPHA_P(val2));
				ADD_F123_SA;
				CLAMP_V123;
				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s13 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;
				x_s23 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s11, x_s13, x_a11,
					x_a12, x_a13, x_a14);
				GetAlpha1a(x_s21, x_s23, x_a21,
					x_a22, x_a23, x_a24);

				/* blend */
				GetDst1a;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;
				_mm_storeu_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s11, x_a11, x_a12);
				GetAlpha1b(x_s21, x_a21, x_a22);

				/* blend */
				GetDst1b;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a, a1;

				a = *(mlib_f32 *)((mlib_u8 *)mlib_c_blend_Q8 +
					GET_ALPHA_P(val1));
				a1 = *(mlib_f32 *)((mlib_u8 *)mlib_c_blend_Q8 +
					GET_ALPHA_P(val2));
				ADD_F123_SA;
				CLAMP_V123;
				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_DA_OMSA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	__m128i x_s11, x_s12, x_s13, x_s14, x_s21, x_s22, x_s23, x_s24;
	__m128i x_a11, x_a12, x_a13, x_a14,  x_a21, x_a22, x_a23, x_a24;
	__m128i x_d1, x_d2, x_d3, x_d4, x_t1, x_t2, x_t3, x_t4;
	__m128i *px_s1, *px_s2, *px_d;
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask = _mm_set1_epi32(0xff);
	const __m128i x_mask1 = _mm_set1_epi32(0xffffff00);
	const __m128i x_c = _mm_set1_epi16(0x100);
	mlib_u8 *blend8 = (mlib_u8 *)mlib_c_blend_Q8 + 1024;

	BLEND_VALIDATE;
	if ((0 == (0xf & ((mlib_addr)dst | (mlib_addr)src1 |
		(mlib_addr)src2))) &&
		((0 == (0xf & (dlb | slb1 | slb2))) | (1 == h))) {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s13 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;
				x_s23 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s11, x_s13, x_a11,
					x_a12, x_a13, x_a14);
				GetAlpha1a(x_s21, x_s23, x_a21,
					x_a22, x_a23, x_a24);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);
				x_a23 = _mm_sub_epi16(x_c, x_a23);
				x_a24 = _mm_sub_epi16(x_c, x_a24);

				/* blend */
				GetDst1a;
				_mm_store_si128(px_d, x_d1);
				px_d++;
				_mm_store_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s11, x_a11, x_a12);
				GetAlpha1b(x_s21, x_a21, x_a22);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);

				/* blend */
				GetDst1b;
				_mm_store_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a, a1;

				a = *(mlib_f32 *)((mlib_u8 *)mlib_c_blend_Q8 +
					GET_ALPHA_P(val1));
				a1 = *(mlib_f32 *)((mlib_u8 *)blend8 -
					GET_ALPHA_P(val2));
				ADD_F123_SA;
				CLAMP_V123;
				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s13 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;
				x_s23 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s11, x_s13, x_a11,
					x_a12, x_a13, x_a14);
				GetAlpha1a(x_s21, x_s23, x_a21,
					x_a22, x_a23, x_a24);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);
				x_a23 = _mm_sub_epi16(x_c, x_a23);
				x_a24 = _mm_sub_epi16(x_c, x_a24);

				/* blend */
				GetDst1a;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;
				_mm_storeu_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s11, x_a11, x_a12);
				GetAlpha1b(x_s21, x_a21, x_a22);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);

				/* blend */
				GetDst1b;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a, a1;

				a = *(mlib_f32 *)((mlib_u8 *)mlib_c_blend_Q8 +
					GET_ALPHA_P(val1));
				a1 = *(mlib_f32 *)((mlib_u8 *)blend8 -
					GET_ALPHA_P(val2));
				ADD_F123_SA;
				CLAMP_V123;
				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMDA_OMSA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	__m128i x_s11, x_s12, x_s13, x_s14, x_s21, x_s22, x_s23, x_s24;
	__m128i x_a11, x_a12, x_a13, x_a14,  x_a21, x_a22, x_a23, x_a24;
	__m128i x_d1, x_d2, x_d3, x_d4, x_t1, x_t2, x_t3, x_t4;
	__m128i *px_s1, *px_s2, *px_d;
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask = _mm_set1_epi32(0xff);
	const __m128i x_mask1 = _mm_set1_epi32(0xffffff00);
	const __m128i x_c = _mm_set1_epi16(0x100);
	mlib_u8 *blend8 = (mlib_u8 *)mlib_c_blend_Q8 + 1024;

	BLEND_VALIDATE;
	if ((0 == (0xf & ((mlib_addr)dst | (mlib_addr)src1 |
		(mlib_addr)src2))) &&
		((0 == (0xf & (dlb | slb1 | slb2))) | (1 == h))) {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s13 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;
				x_s23 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s11, x_s13, x_a11,
					x_a12, x_a13, x_a14);
				GetAlpha1a(x_s21, x_s23, x_a21,
					x_a22, x_a23, x_a24);
				x_a11 = _mm_sub_epi16(x_c, x_a11);
				x_a12 = _mm_sub_epi16(x_c, x_a12);
				x_a13 = _mm_sub_epi16(x_c, x_a13);
				x_a14 = _mm_sub_epi16(x_c, x_a14);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);
				x_a23 = _mm_sub_epi16(x_c, x_a23);
				x_a24 = _mm_sub_epi16(x_c, x_a24);

				/* blend */
				GetDst1a;
				_mm_store_si128(px_d, x_d1);
				px_d++;
				_mm_store_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_load_si128(px_s1);
				px_s1++;
				x_s21 = _mm_load_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s11, x_a11, x_a12);
				GetAlpha1b(x_s21, x_a21, x_a22);
				x_a11 = _mm_sub_epi16(x_c, x_a11);
				x_a12 = _mm_sub_epi16(x_c, x_a12);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);

				/* blend */
				GetDst1b;
				_mm_store_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a, a1;

				a = *(mlib_f32 *)((mlib_u8 *)blend8 -
					GET_ALPHA_P(val1));
				a1 = *(mlib_f32 *)((mlib_u8 *)blend8 -
					GET_ALPHA_P(val2));
				ADD_F123_SA;
				CLAMP_V123;
				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	} else {
		for (j = 0; j < h; j++) {
			px_s1 = (__m128i *)psrc1;
			px_s2 = (__m128i *)psrc2;
			px_d = (__m128i *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (w - 8); i += 8) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s13 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;
				x_s23 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1a(x_s11, x_s13, x_a11,
					x_a12, x_a13, x_a14);
				GetAlpha1a(x_s21, x_s23, x_a21,
					x_a22, x_a23, x_a24);
				x_a11 = _mm_sub_epi16(x_c, x_a11);
				x_a12 = _mm_sub_epi16(x_c, x_a12);
				x_a13 = _mm_sub_epi16(x_c, x_a13);
				x_a14 = _mm_sub_epi16(x_c, x_a14);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);
				x_a23 = _mm_sub_epi16(x_c, x_a23);
				x_a24 = _mm_sub_epi16(x_c, x_a24);

				/* blend */
				GetDst1a;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;
				_mm_storeu_si128(px_d, x_d3);
				px_d++;
			}

			if (i <= (w - 4)) {
				x_s11 = _mm_loadu_si128(px_s1);
				px_s1++;
				x_s21 = _mm_loadu_si128(px_s2);
				px_s2++;

				/* alpha */
				GetAlpha1b(x_s11, x_a11, x_a12);
				GetAlpha1b(x_s21, x_a21, x_a22);
				x_a11 = _mm_sub_epi16(x_c, x_a11);
				x_a12 = _mm_sub_epi16(x_c, x_a12);
				x_a21 = _mm_sub_epi16(x_c, x_a21);
				x_a22 = _mm_sub_epi16(x_c, x_a22);

				/* blend */
				GetDst1b;
				_mm_storeu_si128(px_d, x_d1);
				px_d++;

				i += 4;
			}

			for (; i < w; i++) {
				mlib_u32 val1 = ((mlib_u32 *)psrc1)[i];
				mlib_u32 val2 = ((mlib_u32 *)psrc2)[i];
				mlib_f32 f1, f2, f3;
				mlib_s32 v1, v2, v3;
				mlib_f32 a, a1;

				a = *(mlib_f32 *)((mlib_u8 *)blend8 -
					GET_ALPHA_P(val1));
				a1 = *(mlib_f32 *)((mlib_u8 *)blend8 -
					GET_ALPHA_P(val2));
				ADD_F123_SA;
				CLAMP_V123;
				((mlib_u32 *)pdst)[i] = v1 | v2 | v3;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */
