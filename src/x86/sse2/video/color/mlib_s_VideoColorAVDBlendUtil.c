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

#pragma ident	"@(#)mlib_s_VideoColorAVDBlendUtil.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorBlend_[ZERO, ONE, DA, SA, OMDA, OMSA]_
 *      [ZERO, ONE, DA, SA, OMDA, OMSA]
 *      blend two images with blend factors
 *
 * SYNOPSIS
 *      void mlib_VideoColorBlend_[ZERO, ONE, DA, SA, OMDA, OMSA]_
 *           [ZERO, ONE, DA, SA, OMDA, OMSA]
 *                                          (mlib_u32       * dst,
 *                                           const mlib_u32 * src1,
 *                                           const mlib_u32 * src2,
 *                                           mlib_s32       w,
 *                                           mlib_s32       h,
 *                                           mlib_s32       dlb,
 *                                           mlib_s32       slb1,
 *                                           mlib_s32       slb2)
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
 *
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

/* *********************************************************** */

#define	BLEND_VALIDATE_1                                        \
	mlib_u8 *psrc2, *pdst;                                  \
	mlib_s32 i, j;                                          \
	                                                        \
	pdst = (mlib_u8 *)dst;                                  \
	psrc2 = (mlib_u8 *)src2

/* *********************************************************** */

void
mlib_VideoColorCopy(
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
		__mlib_VectorCopy_S32((mlib_s32 *)dst, (mlib_s32 *)src, w);
		dst += dlb / 4;
		src += slb / 4;
	}
}

/* *********************************************************** */

void
mlib_VideoColorCopy2(
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
		__mlib_VectorCopy_S32((mlib_s32 *)dst, (mlib_s32 *)src, istop);
		__mlib_VectorCopy_S32((mlib_s32 *)dst + istart,
			(mlib_s32 *)src + istart, w - istart);
		dst += dlb / 4;
		src += slb / 4;
	}
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
/* alternative code, slower */
#if 0
	x_t1 = _mm_and_si128(s0, x_mask);
	x_t1 = _mm_or_si128(x_t1, _mm_slli_si128(x_t1, 1));
	x_t1 = _mm_or_si128(x_t1, _mm_slli_si128(x_t1, 2));
	x_t2 = _mm_and_si128(s1, x_mask);
	x_t2 = _mm_or_si128(x_t2, _mm_slli_si128(x_t2, 1));
	x_t2 = _mm_or_si128(x_t2, _mm_slli_si128(x_t2, 2));
	a0 = _mm_unpacklo_epi8(x_t1, x_zero);
	a1 = _mm_unpackhi_epi8(x_t1, x_zero);
	a2 = _mm_unpacklo_epi8(x_t2, x_zero);
	a3 = _mm_unpackhi_epi8(x_t2, x_zero);
#endif

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
	x_d3 = _mm_packus_epi16(x_d3, x_d4);	\
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
}

void
mlib_VideoColorBlend_DA_OMSA(
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
	const __m128i x_c = _mm_set1_epi16(0x100);
	mlib_s32 off = 0;
	mlib_f32 alpha1, alpha2;
	mlib_f32 src20, src21, src22, src23;
	mlib_f32 src10, src11, src12, src13;

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
				alpha1 = mlib_c_blend_Q8[psrc1[4 * i + off]];
				alpha2 =
				    mlib_c_blend_Q8[256 - psrc2[4 * i + off]];

				src10 = mlib_c_blend_u8_sat[psrc1[4 * i + 0]];
				src20 = mlib_c_blend_u8_sat[psrc2[4 * i + 0]];

				src11 = mlib_c_blend_u8_sat[psrc1[4 * i + 1]];
				src21 = mlib_c_blend_u8_sat[psrc2[4 * i + 1]];

				src12 = mlib_c_blend_u8_sat[psrc1[4 * i + 2]];
				src22 = mlib_c_blend_u8_sat[psrc2[4 * i + 2]];

				src13 = mlib_c_blend_u8_sat[psrc1[4 * i + 3]];
				src23 = mlib_c_blend_u8_sat[psrc2[4 * i + 3]];

				pdst[4 * i + 0] =
					FLOAT2INT_CLAMP(src10 * alpha1 +
					src20 * alpha2) >> 23;
				pdst[4 * i + 1] =
					FLOAT2INT_CLAMP(src11 * alpha1 +
					src21 * alpha2) >> 23;
				pdst[4 * i + 2] =
					FLOAT2INT_CLAMP(src12 * alpha1 +
					src22 * alpha2) >> 23;
				pdst[4 * i + 3] =
					FLOAT2INT_CLAMP(src13 * alpha1 +
					src23 * alpha2) >> 23;
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
				alpha1 = mlib_c_blend_Q8[psrc1[4 * i + off]];
				alpha2 =
				    mlib_c_blend_Q8[256 - psrc2[4 * i + off]];

				src10 = mlib_c_blend_u8_sat[psrc1[4 * i + 0]];
				src20 = mlib_c_blend_u8_sat[psrc2[4 * i + 0]];

				src11 = mlib_c_blend_u8_sat[psrc1[4 * i + 1]];
				src21 = mlib_c_blend_u8_sat[psrc2[4 * i + 1]];

				src12 = mlib_c_blend_u8_sat[psrc1[4 * i + 2]];
				src22 = mlib_c_blend_u8_sat[psrc2[4 * i + 2]];

				src13 = mlib_c_blend_u8_sat[psrc1[4 * i + 3]];
				src23 = mlib_c_blend_u8_sat[psrc2[4 * i + 3]];

				pdst[4 * i + 0] =
					FLOAT2INT_CLAMP(src10 * alpha1 +
					src20 * alpha2) >> 23;
				pdst[4 * i + 1] =
					FLOAT2INT_CLAMP(src11 * alpha1 +
					src21 * alpha2) >> 23;
				pdst[4 * i + 2] =
					FLOAT2INT_CLAMP(src12 * alpha1 +
					src22 * alpha2) >> 23;
				pdst[4 * i + 3] =
					FLOAT2INT_CLAMP(src13 * alpha1 +
					src23 * alpha2) >> 23;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_DA_SA(
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
	mlib_s32 off = 0;
	mlib_f32 alpha1, alpha2;
	mlib_f32 src20, src21, src22, src23;
	mlib_f32 src10, src11, src12, src13;

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
				alpha1 = mlib_c_blend_Q8[psrc1[4 * i + off]];
				alpha2 = mlib_c_blend_Q8[psrc2[4 * i + off]];

				src10 = mlib_c_blend_u8_sat[psrc1[4 * i + 0]];
				src20 = mlib_c_blend_u8_sat[psrc2[4 * i + 0]];

				src11 = mlib_c_blend_u8_sat[psrc1[4 * i + 1]];
				src21 = mlib_c_blend_u8_sat[psrc2[4 * i + 1]];

				src12 = mlib_c_blend_u8_sat[psrc1[4 * i + 2]];
				src22 = mlib_c_blend_u8_sat[psrc2[4 * i + 2]];

				src13 = mlib_c_blend_u8_sat[psrc1[4 * i + 3]];
				src23 = mlib_c_blend_u8_sat[psrc2[4 * i + 3]];

				pdst[4 * i + 0] =
					FLOAT2INT_CLAMP(src10 * alpha1 +
					src20 * alpha2) >> 23;
				pdst[4 * i + 1] =
					FLOAT2INT_CLAMP(src11 * alpha1 +
					src21 * alpha2) >> 23;
				pdst[4 * i + 2] =
					FLOAT2INT_CLAMP(src12 * alpha1 +
					src22 * alpha2) >> 23;
				pdst[4 * i + 3] =
					FLOAT2INT_CLAMP(src13 * alpha1 +
					src23 * alpha2) >> 23;
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
				alpha1 = mlib_c_blend_Q8[psrc1[4 * i + off]];
				alpha2 = mlib_c_blend_Q8[psrc2[4 * i + off]];

				src10 = mlib_c_blend_u8_sat[psrc1[4 * i + 0]];
				src20 = mlib_c_blend_u8_sat[psrc2[4 * i + 0]];

				src11 = mlib_c_blend_u8_sat[psrc1[4 * i + 1]];
				src21 = mlib_c_blend_u8_sat[psrc2[4 * i + 1]];

				src12 = mlib_c_blend_u8_sat[psrc1[4 * i + 2]];
				src22 = mlib_c_blend_u8_sat[psrc2[4 * i + 2]];

				src13 = mlib_c_blend_u8_sat[psrc1[4 * i + 3]];
				src23 = mlib_c_blend_u8_sat[psrc2[4 * i + 3]];

				pdst[4 * i + 0] =
					FLOAT2INT_CLAMP(src10 * alpha1 +
					src20 * alpha2) >> 23;
				pdst[4 * i + 1] =
					FLOAT2INT_CLAMP(src11 * alpha1 +
					src21 * alpha2) >> 23;
				pdst[4 * i + 2] =
					FLOAT2INT_CLAMP(src12 * alpha1 +
					src22 * alpha2) >> 23;
				pdst[4 * i + 3] =
					FLOAT2INT_CLAMP(src13 * alpha1 +
					src23 * alpha2) >> 23;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMDA_OMSA(
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
	const __m128i x_c = _mm_set1_epi16(0x100);
	mlib_s32 off = 0;
	mlib_f32 alpha1, alpha2;
	mlib_f32 src20, src21, src22, src23;
	mlib_f32 src10, src11, src12, src13;

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
				alpha1 =
				    mlib_c_blend_Q8[256 - psrc1[4 * i + off]];
				alpha2 =
				    mlib_c_blend_Q8[256 - psrc2[4 * i + off]];

				src10 = mlib_c_blend_u8_sat[psrc1[4 * i + 0]];
				src20 = mlib_c_blend_u8_sat[psrc2[4 * i + 0]];

				src11 = mlib_c_blend_u8_sat[psrc1[4 * i + 1]];
				src21 = mlib_c_blend_u8_sat[psrc2[4 * i + 1]];

				src12 = mlib_c_blend_u8_sat[psrc1[4 * i + 2]];
				src22 = mlib_c_blend_u8_sat[psrc2[4 * i + 2]];

				src13 = mlib_c_blend_u8_sat[psrc1[4 * i + 3]];
				src23 = mlib_c_blend_u8_sat[psrc2[4 * i + 3]];

				pdst[4 * i + 0] =
					FLOAT2INT_CLAMP(src10 * alpha1 +
					src20 * alpha2) >> 23;
				pdst[4 * i + 1] =
					FLOAT2INT_CLAMP(src11 * alpha1 +
					src21 * alpha2) >> 23;
				pdst[4 * i + 2] =
					FLOAT2INT_CLAMP(src12 * alpha1 +
					src22 * alpha2) >> 23;
				pdst[4 * i + 3] =
					FLOAT2INT_CLAMP(src13 * alpha1 +
					src23 * alpha2) >> 23;
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
				alpha1 =
				    mlib_c_blend_Q8[256- psrc1[4 * i + off]];
				alpha2 =
				    mlib_c_blend_Q8[256 - psrc2[4 * i + off]];

				src10 = mlib_c_blend_u8_sat[psrc1[4 * i + 0]];
				src20 = mlib_c_blend_u8_sat[psrc2[4 * i + 0]];

				src11 = mlib_c_blend_u8_sat[psrc1[4 * i + 1]];
				src21 = mlib_c_blend_u8_sat[psrc2[4 * i + 1]];

				src12 = mlib_c_blend_u8_sat[psrc1[4 * i + 2]];
				src22 = mlib_c_blend_u8_sat[psrc2[4 * i + 2]];

				src13 = mlib_c_blend_u8_sat[psrc1[4 * i + 3]];
				src23 = mlib_c_blend_u8_sat[psrc2[4 * i + 3]];

				pdst[4 * i + 0] =
					FLOAT2INT_CLAMP(src10 * alpha1 +
					src20 * alpha2) >> 23;
				pdst[4 * i + 1] =
					FLOAT2INT_CLAMP(src11 * alpha1 +
					src21 * alpha2) >> 23;
				pdst[4 * i + 2] =
					FLOAT2INT_CLAMP(src12 * alpha1 +
					src22 * alpha2) >> 23;
				pdst[4 * i + 3] =
					FLOAT2INT_CLAMP(src13 * alpha1 +
					src23 * alpha2) >> 23;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMSA_OMDA(
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
	const __m128i x_c = _mm_set1_epi16(0x100);
	mlib_s32 off = 0;
	mlib_f32 alpha1, alpha2;
	mlib_f32 src20, src21, src22, src23;
	mlib_f32 src10, src11, src12, src13;

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
				alpha1 =
				    mlib_c_blend_Q8[256 - psrc1[4 * i + off]];
				alpha2 =
				    mlib_c_blend_Q8[256 - psrc2[4 * i + off]];

				src10 = mlib_c_blend_u8_sat[psrc1[4 * i + 0]];
				src20 = mlib_c_blend_u8_sat[psrc2[4 * i + 0]];

				src11 = mlib_c_blend_u8_sat[psrc1[4 * i + 1]];
				src21 = mlib_c_blend_u8_sat[psrc2[4 * i + 1]];

				src12 = mlib_c_blend_u8_sat[psrc1[4 * i + 2]];
				src22 = mlib_c_blend_u8_sat[psrc2[4 * i + 2]];

				src13 = mlib_c_blend_u8_sat[psrc1[4 * i + 3]];
				src23 = mlib_c_blend_u8_sat[psrc2[4 * i + 3]];

				pdst[4 * i + 0] =
					FLOAT2INT_CLAMP(src10 * alpha2 +
					src20 * alpha1) >> 23;
				pdst[4 * i + 1] =
					FLOAT2INT_CLAMP(src11 * alpha2 +
					src21 * alpha1) >> 23;
				pdst[4 * i + 2] =
					FLOAT2INT_CLAMP(src12 * alpha2 +
					src22 * alpha1) >> 23;
				pdst[4 * i + 3] =
					FLOAT2INT_CLAMP(src13 * alpha2 +
					src23 * alpha1) >> 23;
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
				alpha1 =
				    mlib_c_blend_Q8[256- psrc1[4 * i + off]];
				alpha2 =
				    mlib_c_blend_Q8[256 - psrc2[4 * i + off]];

				src10 = mlib_c_blend_u8_sat[psrc1[4 * i + 0]];
				src20 = mlib_c_blend_u8_sat[psrc2[4 * i + 0]];

				src11 = mlib_c_blend_u8_sat[psrc1[4 * i + 1]];
				src21 = mlib_c_blend_u8_sat[psrc2[4 * i + 1]];

				src12 = mlib_c_blend_u8_sat[psrc1[4 * i + 2]];
				src22 = mlib_c_blend_u8_sat[psrc2[4 * i + 2]];

				src13 = mlib_c_blend_u8_sat[psrc1[4 * i + 3]];
				src23 = mlib_c_blend_u8_sat[psrc2[4 * i + 3]];

				pdst[4 * i + 0] =
					FLOAT2INT_CLAMP(src10 * alpha2 +
					src20 * alpha1) >> 23;
				pdst[4 * i + 1] =
					FLOAT2INT_CLAMP(src11 * alpha2 +
					src21 * alpha1) >> 23;
				pdst[4 * i + 2] =
					FLOAT2INT_CLAMP(src12 * alpha2 +
					src22 * alpha1) >> 23;
				pdst[4 * i + 3] =
					FLOAT2INT_CLAMP(src13 * alpha2 +
					src23 * alpha1) >> 23;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMSA_OMSA(
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
	const __m128i x_c = _mm_set1_epi16(0x100);
	mlib_s32 off = 0;
	mlib_f32 fsrc0, fsrc1, fsrc2, fsrc3, alpha2;

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
				alpha2 = mlib_c_blend_u8_sat[256 -
					psrc2[4 * i + off]];
				fsrc0 = mlib_c_blend_Q8[psrc1[4 * i + 0] +
					psrc2[4 * i + 0]];
				fsrc1 = mlib_c_blend_Q8[psrc1[4 * i + 1] +
					psrc2[4 * i + 1]];
				fsrc2 = mlib_c_blend_Q8[psrc1[4 * i + 2] +
					psrc2[4 * i + 2]];
				fsrc3 = mlib_c_blend_Q8[psrc1[4 * i + 3] +
					psrc2[4 * i + 3]];

				pdst[4 * i + 0] = FLOAT2INT_CLAMP(
					fsrc0 * alpha2) >> 23;
				pdst[4 * i + 1] = FLOAT2INT_CLAMP(
					fsrc1 * alpha2) >> 23;
				pdst[4 * i + 2] = FLOAT2INT_CLAMP(
					fsrc2 * alpha2) >> 23;
				pdst[4 * i + 3] = FLOAT2INT_CLAMP(
					fsrc3 * alpha2) >> 23;
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
				alpha2 = mlib_c_blend_u8_sat[256 -
					psrc2[4 * i + off]];

				fsrc0 = mlib_c_blend_Q8[psrc1[4 * i + 0] +
					psrc2[4 * i + 0]];
				fsrc1 = mlib_c_blend_Q8[psrc1[4 * i + 1] +
					psrc2[4 * i + 1]];
				fsrc2 = mlib_c_blend_Q8[psrc1[4 * i + 2] +
					psrc2[4 * i + 2]];
				fsrc3 = mlib_c_blend_Q8[psrc1[4 * i + 3] +
					psrc2[4 * i + 3]];

				pdst[4 * i + 0] = FLOAT2INT_CLAMP(
					fsrc0 * alpha2) >> 23;
				pdst[4 * i + 1] = FLOAT2INT_CLAMP(
					fsrc1 * alpha2) >> 23;
				pdst[4 * i + 2] = FLOAT2INT_CLAMP(
					fsrc2 * alpha2) >> 23;
				pdst[4 * i + 3] = FLOAT2INT_CLAMP(
					fsrc3 * alpha2) >> 23;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMSA_SA(
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
	const __m128i x_c = _mm_set1_epi16(0x100);
	mlib_s32 off = 0;
	mlib_f32 src10, src20, src11, src21, src12, src22, src13, src23;
	mlib_f32 alpha2;

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
				alpha2 = mlib_c_blend_Q8[psrc2[4 * i + off]];

				src10 = mlib_c_blend_u8_sat[psrc1[4 * i + 0]];
				src20 = mlib_c_blend_u8_sat[psrc2[4 * i + 0]];

				src11 = mlib_c_blend_u8_sat[psrc1[4 * i + 1]];
				src21 = mlib_c_blend_u8_sat[psrc2[4 * i + 1]];

				src12 = mlib_c_blend_u8_sat[psrc1[4 * i + 2]];
				src22 = mlib_c_blend_u8_sat[psrc2[4 * i + 2]];

				src13 = mlib_c_blend_u8_sat[psrc1[4 * i + 3]];
				src23 = mlib_c_blend_u8_sat[psrc2[4 * i + 3]];

				pdst[4 * i + 0] =
					(mlib_s32)(src10 + (src20 -
					src10) * alpha2) >> 23;
				pdst[4 * i + 1] =
					(mlib_s32)(src11 + (src21 -
					src11) * alpha2) >> 23;
				pdst[4 * i + 2] =
					(mlib_s32)(src12 + (src22 -
					src12) * alpha2) >> 23;
				pdst[4 * i + 3] =
					(mlib_s32)(src13 + (src23 -
					src13) * alpha2) >> 23;

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
				alpha2 = mlib_c_blend_Q8[psrc2[4 * i + off]];

				src10 = mlib_c_blend_u8_sat[psrc1[4 * i + 0]];
				src20 = mlib_c_blend_u8_sat[psrc2[4 * i + 0]];

				src11 = mlib_c_blend_u8_sat[psrc1[4 * i + 1]];
				src21 = mlib_c_blend_u8_sat[psrc2[4 * i + 1]];

				src12 = mlib_c_blend_u8_sat[psrc1[4 * i + 2]];
				src22 = mlib_c_blend_u8_sat[psrc2[4 * i + 2]];

				src13 = mlib_c_blend_u8_sat[psrc1[4 * i + 3]];
				src23 = mlib_c_blend_u8_sat[psrc2[4 * i + 3]];

				pdst[4 * i + 0] =
					(mlib_s32)(src10 + (src20 -
					src10) * alpha2) >> 23;
				pdst[4 * i + 1] =
					(mlib_s32)(src11 + (src21 -
					src11) * alpha2) >> 23;
				pdst[4 * i + 2] =
					(mlib_s32)(src12 + (src22 -
					src12) * alpha2) >> 23;
				pdst[4 * i + 3] =
					(mlib_s32)(src13 + (src23 -
					src13) * alpha2) >> 23;

			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */
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
	x_d3 = _mm_adds_epu8(x_d3, x_s13);		\
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
}

void
mlib_VideoColorBlend_ONE_DA(
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
	mlib_s32 alpha;
	mlib_s32 help_alpha;
	mlib_f32 d_s0, d_s1, d_s2, a;

	alpha = 0;
	help_alpha = 3;

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

			mlib_u8 *ps1, *ps2, *pd;

			ps1 = (mlib_u8 *)px_s1;
			ps2 = (mlib_u8 *)px_s2;
			pd = (mlib_u8 *)px_d;
			for (; i < w; i++) {
				d_s0 = mlib_c_blend_u8_sat[ps1[1]];
				d_s1 = mlib_c_blend_u8_sat[ps1[2]];
				d_s2 = mlib_c_blend_u8_sat[ps1[help_alpha]];
				a = mlib_c_blend_u8_sat[ps1[alpha]];

				d_s0 += mlib_c_blend_Q8[ps2[1]] * a;
				d_s1 += mlib_c_blend_Q8[ps2[2]] * a;
				d_s2 += mlib_c_blend_Q8[ps2[help_alpha]] * a;

				pd[alpha] =
					FLOAT2INT_CLAMP(a +
					mlib_c_blend_Q8[ps2[alpha]] * a) >> 23;
				pd[1] = FLOAT2INT_CLAMP(d_s0) >> 23;
				pd[2] = FLOAT2INT_CLAMP(d_s1) >> 23;
				pd[help_alpha] = FLOAT2INT_CLAMP(d_s2) >> 23;

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

			mlib_u8 *ps1, *ps2, *pd;

			ps1 = (mlib_u8 *)px_s1;
			ps2 = (mlib_u8 *)px_s2;
			pd = (mlib_u8 *)px_d;
			for (; i < w; i++) {
				d_s0 = mlib_c_blend_u8_sat[ps1[1]];
				d_s1 = mlib_c_blend_u8_sat[ps1[2]];
				d_s2 = mlib_c_blend_u8_sat[ps1[help_alpha]];
				a = mlib_c_blend_u8_sat[ps1[alpha]];

				d_s0 += mlib_c_blend_Q8[ps2[1]] * a;
				d_s1 += mlib_c_blend_Q8[ps2[2]] * a;
				d_s2 += mlib_c_blend_Q8[ps2[help_alpha]] * a;

				pd[alpha] =
					FLOAT2INT_CLAMP(a +
					mlib_c_blend_Q8[ps2[alpha]] * a) >> 23;
				pd[1] = FLOAT2INT_CLAMP(d_s0) >> 23;
				pd[2] = FLOAT2INT_CLAMP(d_s1) >> 23;
				pd[help_alpha] = FLOAT2INT_CLAMP(d_s2) >> 23;

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

void
mlib_VideoColorBlend_ONE_OMDA(
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
	const __m128i x_c = _mm_set1_epi16(0x100);
	mlib_s32 alpha, help_alpha;
	mlib_f32 d_s0, d_s1, d_s2, a;

	alpha = 0;
	help_alpha = 3;

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

			mlib_u8 *ps1, *ps2, *pd;

			ps1 = (mlib_u8 *)px_s1;
			ps2 = (mlib_u8 *)px_s2;
			pd = (mlib_u8 *)px_d;
			for (; i < w; i++) {
				d_s0 = mlib_c_blend_u8_sat[ps1[1]];
				d_s1 = mlib_c_blend_u8_sat[ps1[2]];
				d_s2 = mlib_c_blend_u8_sat[ps1[help_alpha]];
				a = mlib_c_blend_u8_sat[256 - ps1[alpha]];

				d_s0 += mlib_c_blend_Q8[ps2[1]] * a;
				d_s1 += mlib_c_blend_Q8[ps2[2]] * a;
				d_s2 += mlib_c_blend_Q8[ps2[help_alpha]] * a;

				pd[alpha] =
					FLOAT2INT_CLAMP(mlib_c_blend_u8_sat[ps1
					[alpha]] +
					mlib_c_blend_Q8[ps2[alpha]] * a) >> 23;
				pd[1] = FLOAT2INT_CLAMP(d_s0) >> 23;
				pd[2] = FLOAT2INT_CLAMP(d_s1) >> 23;
				pd[help_alpha] = FLOAT2INT_CLAMP(d_s2) >> 23;

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

			mlib_u8 *ps1, *ps2, *pd;

			ps1 = (mlib_u8 *)px_s1;
			ps2 = (mlib_u8 *)px_s2;
			pd = (mlib_u8 *)px_d;
			for (; i < w; i++) {
				d_s0 = mlib_c_blend_u8_sat[ps1[1]];
				d_s1 = mlib_c_blend_u8_sat[ps1[2]];
				d_s2 = mlib_c_blend_u8_sat[ps1[help_alpha]];
				a = mlib_c_blend_u8_sat[256 - ps1[alpha]];

				d_s0 += mlib_c_blend_Q8[ps2[1]] * a;
				d_s1 += mlib_c_blend_Q8[ps2[2]] * a;
				d_s2 += mlib_c_blend_Q8[ps2[help_alpha]] * a;

				pd[alpha] =
					FLOAT2INT_CLAMP(mlib_c_blend_u8_sat[ps1
					[alpha]] +
					mlib_c_blend_Q8[ps2[alpha]] * a) >> 23;
				pd[1] = FLOAT2INT_CLAMP(d_s0) >> 23;
				pd[2] = FLOAT2INT_CLAMP(d_s1) >> 23;
				pd[help_alpha] = FLOAT2INT_CLAMP(d_s2) >> 23;

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

void
mlib_VideoColorBlend_ONE_OMSA(
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
	const __m128i x_c = _mm_set1_epi16(0x100);
	mlib_s32 alpha, help_alpha;
	mlib_f32 d_s0, d_s1, d_s2, a;

	alpha = 0;
	help_alpha = 3;

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

			mlib_u8 *ps1, *ps2, *pd;

			ps1 = (mlib_u8 *)px_s1;
			ps2 = (mlib_u8 *)px_s2;
			pd = (mlib_u8 *)px_d;
			for (; i < w; i++) {
				d_s0 = mlib_c_blend_u8_sat[ps1[1]];
				d_s1 = mlib_c_blend_u8_sat[ps1[2]];
				d_s2 = mlib_c_blend_u8_sat[ps1[help_alpha]];
				a = mlib_c_blend_u8_sat[256 - ps2[alpha]];

				d_s0 += mlib_c_blend_Q8[ps2[1]] * a;
				d_s1 += mlib_c_blend_Q8[ps2[2]] * a;
				d_s2 += mlib_c_blend_Q8[ps2[help_alpha]] * a;

				pd[alpha] = FLOAT2INT_CLAMP(
				    mlib_c_blend_u8_sat[ps1[alpha]] +
				    mlib_c_blend_Q8[ps2[alpha]] * a) >> 23;
				pd[1] = FLOAT2INT_CLAMP(d_s0) >> 23;
				pd[2] = FLOAT2INT_CLAMP(d_s1) >> 23;
				pd[help_alpha] = FLOAT2INT_CLAMP(d_s2) >> 23;

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

			mlib_u8 *ps1, *ps2, *pd;

			ps1 = (mlib_u8 *)px_s1;
			ps2 = (mlib_u8 *)px_s2;
			pd = (mlib_u8 *)px_d;
			for (; i < w; i++) {
				d_s0 = mlib_c_blend_u8_sat[ps1[1]];
				d_s1 = mlib_c_blend_u8_sat[ps1[2]];
				d_s2 = mlib_c_blend_u8_sat[ps1[help_alpha]];
				a = mlib_c_blend_u8_sat[256 - ps2[alpha]];

				d_s0 += mlib_c_blend_Q8[ps2[1]] * a;
				d_s1 += mlib_c_blend_Q8[ps2[2]] * a;
				d_s2 += mlib_c_blend_Q8[ps2[help_alpha]] * a;

				pd[alpha] = FLOAT2INT_CLAMP(
				    mlib_c_blend_u8_sat[ps1[alpha]] +
				    mlib_c_blend_Q8[ps2[alpha]] * a) >> 23;
				pd[1] = FLOAT2INT_CLAMP(d_s0) >> 23;
				pd[2] = FLOAT2INT_CLAMP(d_s1) >> 23;
				pd[help_alpha] = FLOAT2INT_CLAMP(d_s2) >> 23;

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

void
mlib_VideoColorBlend_ONE_ONE(
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
		__mlib_VectorAdd_U8_U8_Sat((mlib_u8 *)dst, (mlib_u8 *)src1,
			(mlib_u8 *)src2, w * 4);
		dst += dlb / 4;
		src1 += slb1 / 4;
		src2 += slb2 / 4;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ONE_SA(
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
	mlib_s32 off = 0;

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
				mlib_f32 alpha =
				    mlib_c_blend_Q8[psrc2[4 * i + off]];

				pdst[4 * i] =
				    FLOAT2INT_CLAMP(mlib_c_blend_u8_sat[
				    psrc2[4 * i]] * alpha +
				    mlib_c_blend_u8_sat[psrc1[4 * i]])
				    >> 23;
				pdst[4 * i + 1] =
				    FLOAT2INT_CLAMP(mlib_c_blend_u8_sat[
				    psrc2[4 * i + 1]] * alpha +
				    mlib_c_blend_u8_sat[psrc1[4 * i + 1]])
				    >> 23;
				pdst[4 * i + 2] =
				    FLOAT2INT_CLAMP(mlib_c_blend_u8_sat[
				    psrc2[4 * i + 2]] * alpha +
				    mlib_c_blend_u8_sat[psrc1[4 * i + 2]])
				    >>23;
				pdst[4 * i + 3] =
				    FLOAT2INT_CLAMP(mlib_c_blend_u8_sat[
				    psrc2[4 * i + 3]] * alpha +
				    mlib_c_blend_u8_sat[psrc1[4 * i + 3]])
				    >> 23;
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
				mlib_f32 alpha =
					mlib_c_blend_Q8[psrc2[4 * i + off]];

				pdst[4 * i] =
					FLOAT2INT_CLAMP(mlib_c_blend_u8_sat[
					psrc2[4 * i]] * alpha +
					mlib_c_blend_u8_sat[psrc1[4 * i]])
					>> 23;
				pdst[4 * i + 1] =
					FLOAT2INT_CLAMP(mlib_c_blend_u8_sat[
					psrc2[4 * i + 1]] * alpha +
					mlib_c_blend_u8_sat[psrc1[4 * i + 1]])
					>> 23;
				pdst[4 * i + 2] =
					FLOAT2INT_CLAMP(mlib_c_blend_u8_sat[
					psrc2[4 * i + 2]] * alpha +
					mlib_c_blend_u8_sat[psrc1[4 * i + 2]])
					>> 23;
				pdst[4 * i + 3] =
					FLOAT2INT_CLAMP(mlib_c_blend_u8_sat[
					psrc2[4 * i + 3]] * alpha +
					mlib_c_blend_u8_sat[psrc1[4 * i + 3]])
					>> 23;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ONE_ZERO(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorCopy(dst, src1, w, h, dlb, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_SA_DA(
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
	mlib_s32 off = 0;
	mlib_f32 alpha1, alpha2;
	mlib_f32 src20, src21, src22, src23;
	mlib_f32 src10, src11, src12, src13;

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
				alpha1 = mlib_c_blend_Q8[psrc1[4 * i + off]];
				alpha2 = mlib_c_blend_Q8[psrc2[4 * i + off]];

				src10 = mlib_c_blend_u8_sat[psrc1[4 * i + 0]];
				src20 = mlib_c_blend_u8_sat[psrc2[4 * i + 0]];

				src11 = mlib_c_blend_u8_sat[psrc1[4 * i + 1]];
				src21 = mlib_c_blend_u8_sat[psrc2[4 * i + 1]];

				src12 = mlib_c_blend_u8_sat[psrc1[4 * i + 2]];
				src22 = mlib_c_blend_u8_sat[psrc2[4 * i + 2]];

				src13 = mlib_c_blend_u8_sat[psrc1[4 * i + 3]];
				src23 = mlib_c_blend_u8_sat[psrc2[4 * i + 3]];

				pdst[4 * i + 0] =
					FLOAT2INT_CLAMP(src10 * alpha2 +
					src20 * alpha1) >> 23;
				pdst[4 * i + 1] =
					FLOAT2INT_CLAMP(src11 * alpha2 +
					src21 * alpha1) >> 23;
				pdst[4 * i + 2] =
					FLOAT2INT_CLAMP(src12 * alpha2 +
					src22 * alpha1) >> 23;
				pdst[4 * i + 3] =
					FLOAT2INT_CLAMP(src13 * alpha2 +
					src23 * alpha1) >> 23;
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
				alpha1 = mlib_c_blend_Q8[psrc1[4 * i + off]];
				alpha2 = mlib_c_blend_Q8[psrc2[4 * i + off]];

				src10 = mlib_c_blend_u8_sat[psrc1[4 * i + 0]];
				src20 = mlib_c_blend_u8_sat[psrc2[4 * i + 0]];

				src11 = mlib_c_blend_u8_sat[psrc1[4 * i + 1]];
				src21 = mlib_c_blend_u8_sat[psrc2[4 * i + 1]];

				src12 = mlib_c_blend_u8_sat[psrc1[4 * i + 2]];
				src22 = mlib_c_blend_u8_sat[psrc2[4 * i + 2]];

				src13 = mlib_c_blend_u8_sat[psrc1[4 * i + 3]];
				src23 = mlib_c_blend_u8_sat[psrc2[4 * i + 3]];

				pdst[4 * i + 0] =
					FLOAT2INT_CLAMP(src10 * alpha2 +
					src20 * alpha1) >> 23;
				pdst[4 * i + 1] =
					FLOAT2INT_CLAMP(src11 * alpha2 +
					src21 * alpha1) >> 23;
				pdst[4 * i + 2] =
					FLOAT2INT_CLAMP(src12 * alpha2 +
					src22 * alpha1) >> 23;
				pdst[4 * i + 3] =
					FLOAT2INT_CLAMP(src13 * alpha2 +
					src23 * alpha1) >> 23;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_SA_OMDA(
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
	const __m128i x_c = _mm_set1_epi16(0x100);
	mlib_s32 off = 0;
	mlib_f32 alpha1, alpha2;

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
			    alpha1 = mlib_c_blend_Q8[256 - psrc1[4 * i + off]];
			    alpha2 = mlib_c_blend_Q8[psrc2[4 * i + off]];

			    pdst[4 * i + 0] = FLOAT2INT_CLAMP(
				mlib_c_blend_u8_sat[psrc1[4 * i + 0]] * alpha2 +
				mlib_c_blend_u8_sat[psrc2[4 * i + 0]] * alpha1)
				>> 23;
			    pdst[4 * i + 1] = FLOAT2INT_CLAMP(
				mlib_c_blend_u8_sat[psrc1[4 * i + 1]] * alpha2 +
				mlib_c_blend_u8_sat[psrc2[4 * i + 1]] * alpha1)
				>> 23;
			    pdst[4 * i + 2] = FLOAT2INT_CLAMP(
				mlib_c_blend_u8_sat[psrc1[4 * i + 2]] * alpha2 +
				mlib_c_blend_u8_sat[psrc2[4 * i + 2]] * alpha1)
				>> 23;
			    pdst[4 * i + 3] = FLOAT2INT_CLAMP(
				mlib_c_blend_u8_sat[psrc1[4 * i + 3]] * alpha2 +
				mlib_c_blend_u8_sat[psrc2[4 * i + 3]] * alpha1)
				>> 23;
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
			    alpha1 = mlib_c_blend_Q8[256 - psrc1[4 * i + off]];
			    alpha2 = mlib_c_blend_Q8[psrc2[4 * i + off]];

			    pdst[4 * i + 0] = FLOAT2INT_CLAMP(
				mlib_c_blend_u8_sat[psrc1[4 * i + 0]] * alpha2 +
				mlib_c_blend_u8_sat[psrc2[4 * i + 0]] * alpha1)
				>> 23;
			    pdst[4 * i + 1] = FLOAT2INT_CLAMP(
				mlib_c_blend_u8_sat[psrc1[4 * i + 1]] * alpha2 +
				mlib_c_blend_u8_sat[psrc2[4 * i + 1]] * alpha1)
				>> 23;
			    pdst[4 * i + 2] = FLOAT2INT_CLAMP(
				mlib_c_blend_u8_sat[psrc1[4 * i + 2]] * alpha2 +
				mlib_c_blend_u8_sat[psrc2[4 * i + 2]] * alpha1)
				>> 23;
			    pdst[4 * i + 3] = FLOAT2INT_CLAMP(
				mlib_c_blend_u8_sat[psrc1[4 * i + 3]] * alpha2 +
				mlib_c_blend_u8_sat[psrc2[4 * i + 3]] * alpha1)
				>> 23;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_SA_OMSA(
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
	const __m128i x_c = _mm_set1_epi16(0x100);
	mlib_s32 off = 0;
	mlib_f32 alpha;
	mlib_f32 src20, src21, src22, src23;
	mlib_f32 src10, src11, src12, src13;

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
				alpha = mlib_c_blend_Q8[psrc2[4 * i + off]];

				src10 = mlib_c_blend_u8_sat[psrc1[4 * i + 0]];
				src20 = mlib_c_blend_u8_sat[psrc2[4 * i + 0]];
				src11 = mlib_c_blend_u8_sat[psrc1[4 * i + 1]];
				src21 = mlib_c_blend_u8_sat[psrc2[4 * i + 1]];
				src12 = mlib_c_blend_u8_sat[psrc1[4 * i + 2]];
				src22 = mlib_c_blend_u8_sat[psrc2[4 * i + 2]];
				src13 = mlib_c_blend_u8_sat[psrc1[4 * i + 3]];
				src23 = mlib_c_blend_u8_sat[psrc2[4 * i + 3]];

				pdst[4 * i + 0] =
					(mlib_s32)((src10 - src20) * alpha +
					src20) >> 23;
				pdst[4 * i + 1] =
					(mlib_s32)((src11 - src21) * alpha +
					src21) >> 23;
				pdst[4 * i + 2] =
					(mlib_s32)((src12 - src22) * alpha +
					src22) >> 23;
				pdst[4 * i + 3] =
					(mlib_s32)((src13 - src23) * alpha +
					src23) >> 23;
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
				alpha = mlib_c_blend_Q8[psrc2[4 * i + off]];

				src10 = mlib_c_blend_u8_sat[psrc1[4 * i + 0]];
				src20 = mlib_c_blend_u8_sat[psrc2[4 * i + 0]];
				src11 = mlib_c_blend_u8_sat[psrc1[4 * i + 1]];
				src21 = mlib_c_blend_u8_sat[psrc2[4 * i + 1]];
				src12 = mlib_c_blend_u8_sat[psrc1[4 * i + 2]];
				src22 = mlib_c_blend_u8_sat[psrc2[4 * i + 2]];
				src13 = mlib_c_blend_u8_sat[psrc1[4 * i + 3]];
				src23 = mlib_c_blend_u8_sat[psrc2[4 * i + 3]];

				pdst[4 * i + 0] =
					(mlib_s32)((src10 - src20) * alpha +
					src20) >> 23;
				pdst[4 * i + 1] =
					(mlib_s32)((src11 - src21) * alpha +
					src21) >> 23;
				pdst[4 * i + 2] =
					(mlib_s32)((src12 - src22) * alpha +
					src22) >> 23;
				pdst[4 * i + 3] =
					(mlib_s32)((src13 - src23) * alpha +
					src23) >> 23;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_SA_SA(
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
	mlib_s32 off = 0;
	mlib_f32 fsrc0, fsrc1, fsrc2, fsrc3, alpha2;

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
				alpha2 = mlib_c_blend_u8_sat[
					psrc2[4 * i + off]];
				fsrc0 = mlib_c_blend_Q8[psrc1[4 * i + 0] +
					psrc2[4 * i + 0]];
				fsrc1 = mlib_c_blend_Q8[psrc1[4 * i + 1] +
					psrc2[4 * i + 1]];
				fsrc2 = mlib_c_blend_Q8[psrc1[4 * i + 2] +
					psrc2[4 * i + 2]];
				fsrc3 = mlib_c_blend_Q8[psrc1[4 * i + 3] +
					psrc2[4 * i + 3]];

				pdst[4 * i + 0] = FLOAT2INT_CLAMP(
					fsrc0 * alpha2) >> 23;
				pdst[4 * i + 1] = FLOAT2INT_CLAMP(
					fsrc1 * alpha2) >> 23;
				pdst[4 * i + 2] = FLOAT2INT_CLAMP(
					fsrc2 * alpha2) >> 23;
				pdst[4 * i + 3] = FLOAT2INT_CLAMP(
					fsrc3 * alpha2) >> 23;
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
				alpha2 = mlib_c_blend_u8_sat[
					psrc2[4 * i + off]];
				fsrc0 = mlib_c_blend_Q8[psrc1[4 * i + 0] +
					psrc2[4 * i + 0]];
				fsrc1 = mlib_c_blend_Q8[psrc1[4 * i + 1] +
					psrc2[4 * i + 1]];
				fsrc2 = mlib_c_blend_Q8[psrc1[4 * i + 2] +
					psrc2[4 * i + 2]];
				fsrc3 = mlib_c_blend_Q8[psrc1[4 * i + 3] +
					psrc2[4 * i + 3]];

				pdst[4 * i + 0] = FLOAT2INT_CLAMP(
					fsrc0 * alpha2) >> 23;
				pdst[4 * i + 1] = FLOAT2INT_CLAMP(
					fsrc1 * alpha2) >> 23;
				pdst[4 * i + 2] = FLOAT2INT_CLAMP(
					fsrc2 * alpha2) >> 23;
				pdst[4 * i + 3] = FLOAT2INT_CLAMP(
					fsrc3 * alpha2) >> 23;
			}

			psrc1 += slb1;
			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */
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
	x_d3 = _mm_packus_epi16(x_d3, x_d4);	\
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
}

void
mlib_VideoColorBlend_ZERO_DA(
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
	mlib_u8 a0, a1, a2, a3;
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

			mlib_u8 *ps1, *ps2, *pd;

			ps1  = (mlib_u8 *)px_s1;
			ps2  = (mlib_u8 *)px_s2;
			pd  = (mlib_u8 *)px_d;
			for (; i < w; i++) {
				a = mlib_c_blend_Q8[ps1[0]];
				a0 = mlib_U82F32[ps2[0]] * a;
				a1 = mlib_U82F32[ps2[1]] * a;
				a2 = mlib_U82F32[ps2[2]] * a;
				a3 = mlib_U82F32[ps2[3]] * a;
				pd[0] = a0;
				pd[1] = a1;
				pd[2] = a2;
				pd[3] = a3;
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

			mlib_u8 *ps1, *ps2, *pd;

			ps1  = (mlib_u8 *)px_s1;
			ps2  = (mlib_u8 *)px_s2;
			pd  = (mlib_u8 *)px_d;
			for (; i < w; i++) {
				a = mlib_c_blend_Q8[ps1[0]];
				a0 = mlib_U82F32[ps2[0]] * a;
				a1 = mlib_U82F32[ps2[1]] * a;
				a2 = mlib_U82F32[ps2[2]] * a;
				a3 = mlib_U82F32[ps2[3]] * a;
				pd[0] = a0;
				pd[1] = a1;
				pd[2] = a2;
				pd[3] = a3;
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

void
mlib_VideoColorBlend_ZERO_OMDA(
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
	const __m128i x_c = _mm_set1_epi16(0x100);
	mlib_u8 a0, a1, a2, a3;
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

			mlib_u8 *ps1, *ps2, *pd;

			ps1  = (mlib_u8 *)px_s1;
			ps2  = (mlib_u8 *)px_s2;
			pd  = (mlib_u8 *)px_d;
			for (; i < w; i++) {
				a = 1 - mlib_c_blend_Q8[ps1[0]];
				a0 = mlib_U82F32[ps2[0]] * a;
				a1 = mlib_U82F32[ps2[1]] * a;
				a2 = mlib_U82F32[ps2[2]] * a;
				a3 = mlib_U82F32[ps2[3]] * a;
				pd[0] = a0;
				pd[1] = a1;
				pd[2] = a2;
				pd[3] = a3;
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

			mlib_u8 *ps1, *ps2, *pd;

			ps1  = (mlib_u8 *)px_s1;
			ps2  = (mlib_u8 *)px_s2;
			pd  = (mlib_u8 *)px_d;
			for (; i < w; i++) {
				a = 1 - mlib_c_blend_Q8[ps1[0]];
				a0 = mlib_U82F32[ps2[0]] * a;
				a1 = mlib_U82F32[ps2[1]] * a;
				a2 = mlib_U82F32[ps2[2]] * a;
				a3 = mlib_U82F32[ps2[3]] * a;
				pd[0] = a0;
				pd[1] = a1;
				pd[2] = a2;
				pd[3] = a3;
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

void
mlib_VideoColorBlend_ZERO_OMSA(
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
	const __m128i x_c = _mm_set1_epi16(0x100);
	mlib_u8 a0, a1, a2, a3;
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

			mlib_u8 *ps2, *pd;

			ps2  = (mlib_u8 *)px_s2;
			pd  = (mlib_u8 *)px_d;
			for (; i < w; i++) {
				a = 1 - mlib_c_blend_Q8[ps2[0]];
				a0 = mlib_U82F32[ps2[0]] * a;
				a1 = mlib_U82F32[ps2[1]] * a;
				a2 = mlib_U82F32[ps2[2]] * a;
				a3 = mlib_U82F32[ps2[3]] * a;
				pd[0] = a0;
				pd[1] = a1;
				pd[2] = a2;
				pd[3] = a3;
				ps2 += 4;
				pd += 4;
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

			mlib_u8 *ps2, *pd;

			ps2  = (mlib_u8 *)px_s2;
			pd  = (mlib_u8 *)px_d;
			for (; i < w; i++) {
				a = 1 - mlib_c_blend_Q8[ps2[0]];
				a0 = mlib_U82F32[ps2[0]] * a;
				a1 = mlib_U82F32[ps2[1]] * a;
				a2 = mlib_U82F32[ps2[2]] * a;
				a3 = mlib_U82F32[ps2[3]] * a;
				pd[0] = a0;
				pd[1] = a1;
				pd[2] = a2;
				pd[3] = a3;
				ps2 += 4;
				pd += 4;
			}

			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ZERO_ONE(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorCopy(dst, src2, w, h, dlb, slb2);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ZERO_SA(
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
	mlib_u8 a0, a1, a2, a3;
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

			mlib_u8 *ps2, *pd;

			ps2  = (mlib_u8 *)px_s2;
			pd  = (mlib_u8 *)px_d;
			for (; i < w; i++) {
				a = mlib_c_blend_Q8[ps2[0]];
				a0 = mlib_U82F32[ps2[0]] * a;
				a1 = mlib_U82F32[ps2[1]] * a;
				a2 = mlib_U82F32[ps2[2]] * a;
				a3 = mlib_U82F32[ps2[3]] * a;
				pd[0] = a0;
				pd[1] = a1;
				pd[2] = a2;
				pd[3] = a3;
				ps2 += 4;
				pd += 4;
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

			mlib_u8 *ps2, *pd;

			ps2  = (mlib_u8 *)px_s2;
			pd  = (mlib_u8 *)px_d;
			for (; i < w; i++) {
				a = mlib_c_blend_Q8[ps2[0]];
				a0 = mlib_U82F32[ps2[0]] * a;
				a1 = mlib_U82F32[ps2[1]] * a;
				a2 = mlib_U82F32[ps2[2]] * a;
				a3 = mlib_U82F32[ps2[3]] * a;
				pd[0] = a0;
				pd[1] = a1;
				pd[2] = a2;
				pd[3] = a3;
				ps2 += 4;
				pd += 4;
			}

			psrc2 += slb2;
			pdst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ZERO_ZERO(
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

void
mlib_VideoColorBlend_DA_DA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_SA_SA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_DA_OMDA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_OMSA_SA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_DA_ONE(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_ONE_SA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_DA_ZERO(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_ZERO_SA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMDA_DA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_SA_OMSA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMDA_OMDA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_OMSA_OMSA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMDA_ONE(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_ONE_OMSA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMDA_SA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_DA_OMSA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMDA_ZERO(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_ZERO_OMSA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMSA_DA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_SA_OMDA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMSA_ONE(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_ONE_OMDA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMSA_ZERO(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_ZERO_OMDA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_SA_ONE(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_ONE_DA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */

void
mlib_VideoColorBlend_SA_ZERO(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorBlend_ZERO_DA(dst, src2, src1, w, h, dlb, slb2, slb1);
}

/* *********************************************************** */
