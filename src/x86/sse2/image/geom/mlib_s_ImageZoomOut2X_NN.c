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

#pragma ident	"@(#)mlib_s_ImageZoomOut2X_NN.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomOut2X -  image 0.5X scaling with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoomOut2X(mlib_image       *dst,
 *                                      const mlib_image *src,
 *                                      mlib_filter      filter,
 *                                      mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      filter    Type of resampling filter.
 *      edge    Type of edge condition.
 *
 * DESCRIPTION
 *  The center of the source image is mapped to the center of the
 *  destination image.
 *
 *  The upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *  The resampling filter can be one of the following:
 *    MLIB_NEAREST
 *    MLIB_BILINEAR
 *    MLIB_BICUBIC
 *
 *  The edge condition can be one of the following:
 *    MLIB_EDGE_DST_NO_WRITE  (default)
 *    MLIB_EDGE_DST_FILL_ZERO
 *    MLIB_EDGE_OP_NEAREST
 *    MLIB_EDGE_SRC_EXTEND
 */

#include <mlib_ImageZoomOut2X.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	PSN	p_src[2 * i * src_stride + 2 * j]
#define	PDN	p_dst[i * dst_stride + j]

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_1_Nearest(
    NEAREST_PARAMETERS(mlib_u8))
{
	mlib_s32 i, i2;

	for (i = 0, i2 = 0; i < dst_height; i++, i2 += 2) {
		mlib_u8 *pd = p_dst + i * dst_stride;
		mlib_u8 *ps = p_src + i2 * src_stride;
		mlib_s32 j, j2, w = dst_width;
		__m128i x_sd0, x_sd1, x_st0, x_st1, x_dd;

		j = j2 = 0;

		if (0 == (((mlib_addr)ps) & 15)) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < w - 15; j += 16) {
				x_sd0 = _mm_load_si128((void *)(ps + j2));
				x_sd1 = _mm_load_si128((void *)(ps + j2 + 16));
				x_st0 = _mm_slli_epi16(x_sd0, 8);
				x_st0 = _mm_srli_epi16(x_st0, 8);
				x_st1 = _mm_slli_epi16(x_sd1, 8);
				x_st1 = _mm_srli_epi16(x_st1, 8);
				x_dd = _mm_packus_epi16(x_st0, x_st1);
				_mm_storeu_si128((void *)(pd + j), x_dd);
				j2 += 32;
			}

		} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < w - 15; j += 16) {
				x_sd0 = _mm_loadu_si128((void *)(ps + j2));
				x_sd1 = _mm_loadu_si128((void *)(ps + j2 + 16));
				x_st0 = _mm_slli_epi16(x_sd0, 8);
				x_st0 = _mm_srli_epi16(x_st0, 8);
				x_st1 = _mm_slli_epi16(x_sd1, 8);
				x_st1 = _mm_srli_epi16(x_st1, 8);
				x_dd = _mm_packus_epi16(x_st0, x_st1);
				_mm_storeu_si128((void *)(pd + j), x_dd);
				j2 += 32;
			}
		}

		for (; j < w; j++, j2 += 2) {
			pd[j] = ps[j2];
		}
	}
}

#undef  PSN
#undef  PDN

/* *********************************************************** */

#define	PSN(A)	p_src[2 * i * src_stride + 4 * j + A]
#define	PDN(A)	p_dst[i * dst_stride + 2 * j + A]

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_2_Nearest(
    NEAREST_PARAMETERS(mlib_u8))
{
	mlib_s32 i, j, j2;
	const __m128i x_zero = _mm_setzero_si128();
	__m128i x_sd0, x_sd1, x_st0, x_st1, x_st2, x_st3, x_dd;

	for (i = 0; i < dst_height; i++) {
		mlib_u8 *pd = p_dst + i * dst_stride;
		mlib_u8 *ps = p_src + 2 * i * src_stride;

		j = j2 = 0;

		if (0 == (((mlib_addr)ps) & 15)) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < dst_width * 2 - 15; j += 16, j2 += 32) {
				x_sd0 = _mm_load_si128((void *)(ps + j2));
				x_sd1 = _mm_load_si128((void *)(ps + j2 + 16));
				x_st0 = _mm_unpacklo_epi16(x_sd0, x_sd1);
				x_st1 = _mm_unpackhi_epi16(x_sd0, x_sd1);
				x_st2 = _mm_unpacklo_epi16(x_st0, x_st1);
				x_st3 = _mm_unpackhi_epi16(x_st0, x_st1);
				x_dd = _mm_unpacklo_epi16(x_st2, x_st3);
				_mm_storeu_si128((void *)(pd + j), x_dd);
			}

			if (j < dst_width * 2 - 7) {
				x_sd0 = _mm_load_si128((void *)(ps + j2));
				x_st0 = _mm_unpacklo_epi16(x_sd0, x_zero);
				x_st1 = _mm_unpackhi_epi16(x_sd0, x_zero);
				x_st2 = _mm_unpacklo_epi16(x_st0, x_st1);
				x_st3 = _mm_unpackhi_epi16(x_st0, x_st1);
				x_dd = _mm_unpacklo_epi16(x_st2, x_st3);
				_mm_storel_epi64((void *)(pd + j), x_dd);
				j += 8;
				j2 += 16;
			}

		} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < dst_width * 2 - 15; j += 16, j2 += 32) {
				x_sd0 = _mm_loadu_si128((void *)(ps + j2));
				x_sd1 = _mm_loadu_si128((void *)(ps + j2 + 16));
				x_st0 = _mm_unpacklo_epi16(x_sd0, x_sd1);
				x_st1 = _mm_unpackhi_epi16(x_sd0, x_sd1);
				x_st2 = _mm_unpacklo_epi16(x_st0, x_st1);
				x_st3 = _mm_unpackhi_epi16(x_st0, x_st1);
				x_dd = _mm_unpacklo_epi16(x_st2, x_st3);
				_mm_storeu_si128((void *)(pd + j), x_dd);
			}

			if (j < dst_width * 2 - 7) {
				x_sd0 = _mm_loadu_si128((void *)(ps + j2));
				x_st0 = _mm_unpacklo_epi16(x_sd0, x_zero);
				x_st1 = _mm_unpackhi_epi16(x_sd0, x_zero);
				x_st2 = _mm_unpacklo_epi16(x_st0, x_st1);
				x_st3 = _mm_unpackhi_epi16(x_st0, x_st1);
				x_dd = _mm_unpacklo_epi16(x_st2, x_st3);
				_mm_storel_epi64((void *)(pd + j), x_dd);
				j += 8;
				j2 += 16;
			}
		}

		for (; j < dst_width * 2; j += 2, j2 += 4) {
			pd[j] = ps[j2];
			pd[j + 1] = ps[j2 + 1];
		}
	}
}

#undef  PSN
#undef  PDN

/* *********************************************************** */

#define	PSN(A)	p_src[2 * i * src_stride + 6 * j + A]
#define	PDN(A)	p_dst[i * dst_stride + 3 * j + A]

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_3_Nearest(
    NEAREST_PARAMETERS(mlib_u8))
{
	mlib_s32 i, j;

	for (i = 0; i < dst_height; i++) {
		mlib_s32 j2;
		mlib_u8 *pd = p_dst + i * dst_stride;
		mlib_u8 *ps = p_src + 2 * i * src_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0, j2 = 0; j < dst_width * 3; j += 3, j2 += 6) {
			pd[j] = ps[j2];
			pd[j + 1] = ps[j2 + 1];
			pd[j + 2] = ps[j2 + 2];
		}
	}
}

#undef  PSN
#undef  PDN

/* *********************************************************** */

#define	PSN(A)	((mlib_u32)p_src[2 * i * src_stride + 8 * j + A])
#define	PDN(A)	p_dst[i * dst_stride + 4 * j + A]

/* *********************************************************** */

void
mlib_ImageZoomOut2X_U8_4_Nearest(
    NEAREST_PARAMETERS(mlib_u8))
{
	mlib_s32 i;
	mlib_u8 *pz_row;
	mlib_u32 *dx, dz;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < dst_height; i++) {
		mlib_s32 j, j2;
		__m128i x_sd0, x_sd1, x_st0, x_st1, x_st2, x_st3, x_dd;
		mlib_u8 *pd = p_dst + i * dst_stride;
		mlib_u8 *ps = p_src + 2 * i * src_stride;

		j = j2 = 0;

		if (0 == (((mlib_addr)ps) & 15)) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0, j2 = 0; j < dst_width * 4 - 15;
			    j += 16, j2 += 32) {
				x_sd0 = _mm_load_si128((void *)(ps + j2));
				x_sd1 = _mm_load_si128((void *)(ps + j2 + 16));
				x_st0 = _mm_unpacklo_epi32(x_sd0, x_sd1);
				x_st1 = _mm_unpackhi_epi32(x_sd0, x_sd1);
				x_dd = _mm_unpacklo_epi32(x_st0, x_st1);
				_mm_storeu_si128((void *)(pd + j), x_dd);
			}

			if (j < dst_width * 4 - 7) {
				x_sd0 = _mm_load_si128((void *)(ps + j2));
				x_st0 = _mm_unpacklo_epi32(x_sd0, x_zero);
				x_st1 = _mm_unpackhi_epi32(x_sd0, x_zero);
				x_dd = _mm_unpacklo_epi32(x_st0, x_st1);
				_mm_storel_epi64((void *)(pd + j), x_dd);
				j += 8;
				j2 += 16;
			}

		} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0, j2 = 0; j < dst_width * 4 - 15;
			    j += 16, j2 += 32) {
				x_sd0 = _mm_loadu_si128((void *)(ps + j2));
				x_sd1 = _mm_loadu_si128((void *)(ps + j2 + 16));
				x_st0 = _mm_unpacklo_epi32(x_sd0, x_sd1);
				x_st1 = _mm_unpackhi_epi32(x_sd0, x_sd1);
				x_dd = _mm_unpacklo_epi32(x_st0, x_st1);
				_mm_storeu_si128((void *)(pd + j), x_dd);
			}

			if (j < dst_width * 4 - 7) {
				x_sd0 = _mm_loadu_si128((void *)(ps + j2));
				x_st0 = _mm_unpacklo_epi32(x_sd0, x_zero);
				x_st1 = _mm_unpackhi_epi32(x_sd0, x_zero);
				x_dd = _mm_unpacklo_epi32(x_st0, x_st1);
				_mm_storel_epi64((void *)(pd + j), x_dd);
				j += 8;
				j2 += 16;
			}
		}

		for (; j < dst_width * 4; j += 4, j2 += 8) {
			pd[j] = ps[j2];
			pd[j + 1] = ps[j2 + 1];
			pd[j + 2] = ps[j2 + 2];
			pd[j + 3] = ps[j2 + 3];
		}
	}
}

#undef  PSN
#undef  PDN

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_1_Nearest(
    NEAREST_PARAMETERS(mlib_s16))
{
	mlib_s32 i;

	for (i = 0; i < dst_height; i++) {
		mlib_s32 j, j2;
		mlib_u16 *pd = (mlib_u16 *)p_dst + i * dst_stride;
		mlib_u16 *ps = (mlib_u16 *)p_src + 2 * i * src_stride;

		__m128i x_sd0, x_sd1, x_st0, x_st1, x_st2, x_st3, x_dd;
		const __m128i x_zero = _mm_setzero_si128();

		if (0 == (((mlib_addr)ps) & 15)) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0, j2 = 0; j < dst_width - 7; j += 8) {
				x_sd0 = _mm_load_si128((void *)(ps + j2));
				x_sd1 = _mm_load_si128((void *)(ps + j2 + 8));
				x_st0 = _mm_unpacklo_epi16(x_sd0, x_sd1);
				x_st1 = _mm_unpackhi_epi16(x_sd0, x_sd1);
				x_st2 = _mm_unpacklo_epi16(x_st0, x_st1);
				x_st3 = _mm_unpackhi_epi16(x_st0, x_st1);
				x_dd = _mm_unpacklo_epi16(x_st2, x_st3);

				_mm_storeu_si128((void *)(pd + j), x_dd);
				j2 += 16;
			}

			if (j < dst_width - 3) {
				x_sd0 = _mm_load_si128((void *)(ps + j2));
				x_st0 = _mm_unpacklo_epi16(x_sd0, x_zero);
				x_st1 = _mm_unpackhi_epi16(x_sd0, x_zero);
				x_st2 = _mm_unpacklo_epi16(x_st0, x_st1);
				x_st3 = _mm_unpackhi_epi16(x_st0, x_st1);
				x_dd = _mm_unpacklo_epi16(x_st2, x_st3);

				_mm_storel_epi64((void *)(pd + j), x_dd);
				j += 4;
				j2 += 8;
			}

		} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0, j2 = 0; j < dst_width - 7; j += 8) {
				x_sd0 = _mm_loadu_si128((void *)(ps + j2));
				x_sd1 = _mm_loadu_si128((void *)(ps + j2 + 8));
				x_st0 = _mm_unpacklo_epi16(x_sd0, x_sd1);
				x_st1 = _mm_unpackhi_epi16(x_sd0, x_sd1);
				x_st2 = _mm_unpacklo_epi16(x_st0, x_st1);
				x_st3 = _mm_unpackhi_epi16(x_st0, x_st1);
				x_dd = _mm_unpacklo_epi16(x_st2, x_st3);

				_mm_storeu_si128((void *)(pd + j), x_dd);
				j2 += 16;
			}

			if (j < dst_width - 3) {
				x_sd0 = _mm_loadu_si128((void *)(ps + j2));
				x_st0 = _mm_unpacklo_epi16(x_sd0, x_zero);
				x_st1 = _mm_unpackhi_epi16(x_sd0, x_zero);
				x_st2 = _mm_unpacklo_epi16(x_st0, x_st1);
				x_st3 = _mm_unpackhi_epi16(x_st0, x_st1);
				x_dd = _mm_unpacklo_epi16(x_st2, x_st3);

				_mm_storel_epi64((void *)(pd + j), x_dd);
				j += 4;
				j2 += 8;
			}
		}

		for (; j < dst_width; j++, j2 += 2) {
			pd[j] = ps[j2];
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_2_Nearest(
    NEAREST_PARAMETERS(mlib_s16))
{
	mlib_s32 i;
	const __m128i x_zero = _mm_setzero_si128();

	for (i = 0; i < dst_height; i++) {
		mlib_s32 j, j2;
		__m128i x_sd0, x_sd1, x_st0, x_st1, x_st2, x_st3, x_dd;
		mlib_u16 *pd = (mlib_u16 *)p_dst + i * dst_stride;
		mlib_u16 *ps = (mlib_u16 *)p_src + 2 * i * src_stride;

		j = j2 = 0;

		if (0 == ((mlib_addr)ps & 15)) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < dst_width * 2 - 6; j += 8) {
				x_sd0 = _mm_load_si128((void *)(ps + j2));
				x_sd1 = _mm_load_si128((void *)(ps + j2 + 8));
				x_st0 = _mm_unpacklo_epi32(x_sd0, x_sd1);
				x_st1 = _mm_unpackhi_epi32(x_sd0, x_sd1);
				x_st2 = _mm_unpacklo_epi32(x_st0, x_st1);
				_mm_storeu_si128((void *)(pd + j), x_st2);
				j2 += 16;
			}

			if (j <= dst_width * 2 - 4) {
				x_sd0 = _mm_load_si128((void *)(ps + j2));
				x_st1 = _mm_unpacklo_epi32(x_sd0, x_zero);
				x_st2 = _mm_unpackhi_epi32(x_sd0, x_zero);
				x_dd = _mm_unpacklo_epi32(x_st1, x_st2);
				_mm_storel_epi64((void *)(pd + j), x_dd);
				j += 4;
				j2 += 8;
			}

		} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < dst_width * 2 - 6; j += 8) {
				x_sd0 = _mm_loadu_si128((void *)(ps + j2));
				x_sd1 = _mm_loadu_si128((void *)(ps + j2 + 8));
				x_st0 = _mm_unpacklo_epi32(x_sd0, x_sd1);
				x_st1 = _mm_unpackhi_epi32(x_sd0, x_sd1);
				x_st2 = _mm_unpacklo_epi32(x_st0, x_st1);
				_mm_storeu_si128((void *)(pd + j), x_st2);
				j2 += 16;
			}

			if (j <= dst_width * 2 - 4) {
				x_sd0 = _mm_loadu_si128((void *)(ps + j2));
				x_st1 = _mm_unpacklo_epi32(x_sd0, x_zero);
				x_st2 = _mm_unpackhi_epi32(x_sd0, x_zero);
				x_dd = _mm_unpacklo_epi32(x_st1, x_st2);
				_mm_storel_epi64((void *)(pd + j), x_dd);
				j += 4;
				j2 += 8;
			}
		}

		if (j < dst_width * 2) {
			pd[j] = ps[j2];
			pd[j + 1] = ps[j2 + 1];
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_3_Nearest(
    NEAREST_PARAMETERS(mlib_s16))
{
	mlib_s32 i;

	for (i = 0; i < dst_height; i++) {
		mlib_s32 j, j2;
		mlib_u16 *pd = (mlib_u16 *)p_dst + i * dst_stride;
		mlib_u16 *ps = (mlib_u16 *)p_src + 2 * i * src_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0, j2 = 0; j < dst_width * 3; j += 3, j2 += 6) {
			pd[j] = ps[j2];
			pd[j + 1] = ps[j2 + 1];
			pd[j + 2] = ps[j2 + 2];
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomOut2X_S16_4_Nearest(
    NEAREST_PARAMETERS(mlib_s16))
{
	mlib_s32 i;

	for (i = 0; i < dst_height; i++) {
		mlib_s32 j, j2;
		__m128i x_sd0, x_sd1, x_dd0, x_sd2, x_sd3, x_dd1;
		mlib_s64 *ps = (mlib_s64 *)(p_src + 2 * i * src_stride);
		mlib_s64 *pd = (mlib_s64 *)(p_dst + i * dst_stride);

		j = j2 = 0;

		if (0 == ((mlib_addr)ps & 15)) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < dst_width - 1; j += 2) {
				x_sd0 = _mm_load_si128((void *)(ps + j2));
				x_sd1 = _mm_load_si128((void *)(ps + j2 + 2));
				x_dd0 = _mm_unpacklo_epi64(x_sd0, x_sd1);
				_mm_storeu_si128((void *)(pd + j), x_dd0);
				j2 += 4;
			}

			if (j < dst_width) {
				x_sd0 = _mm_load_si128((void *)(ps + j2));
				_mm_storel_epi64((void *)(pd + j), x_sd0);
			}

		} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < dst_width - 1; j += 2) {
				x_sd0 = _mm_loadu_si128((void *)(ps + j2));
				x_sd1 = _mm_loadu_si128((void *)(ps + j2 + 2));
				x_dd0 = _mm_unpacklo_epi64(x_sd0, x_sd1);
				_mm_storeu_si128((void *)(pd + j), x_dd0);
				j2 += 4;
			}

			if (j < dst_width) {
				x_sd0 = _mm_loadu_si128((void *)(ps + j2));
				_mm_storel_epi64((void *)(pd + j), x_sd0);
			}
		}
	}
}

/* *********************************************************** */
