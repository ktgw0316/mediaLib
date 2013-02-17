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

#pragma ident	"@(#)mlib_s_ImageZoomOut2X_NN_S32.c	9.2	07/11/05 SMI"

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

#define	DTYPE	mlib_s32

#define	FUNC_NAME(SUFF)	mlib_ImageZoomOut2X_S32_##SUFF

/* *********************************************************** */

void FUNC_NAME(
    1_Nearest) (
    NEAREST_PARAMETERS(DTYPE))
{
	mlib_s32 i, i2, j, j2;

	for (i = 0, i2 = 0; i < dst_height; i++, i2 += 2) {
		mlib_s32 *pd = p_dst + i * dst_stride;
		mlib_s32 *ps = p_src + i2 * src_stride;
		mlib_s32 j, j2, off;
		__m128i x_sd0, x_sd1, x_st0, x_st1, x_dd;

		j = j2 = 0;
		off = (16 - ((mlib_addr)ps & 15)) >> 2;

		switch (off) {
		case 1:
			pd[j] = ps[j2];
			j++;
			j2++;
			break;
		case 2:
			pd[j] = ps[j2];
			j++;
			j2 += 2;
			break;
		case 3:
			pd[j] = ps[j2];
			j++;
			j2 += 2;
			if (dst_width > 1) {
				pd[j] = ps[j2];
				j++;
				j2++;
			}
		}

		switch (off) {
		case 1:
		case 3:

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < dst_width - 4; j += 4, j2 += 8) {
				x_sd0 = _mm_load_si128((void *)(ps + j2));
				x_sd1 = _mm_load_si128((void *)(ps + j2 + 4));
				x_st0 = _mm_unpacklo_epi32(x_sd0, x_sd1);
				x_st1 = _mm_unpackhi_epi32(x_sd0, x_sd1);
				x_dd = _mm_unpackhi_epi32(x_st0, x_st1);
				_mm_storeu_si128((void *)(pd + j), x_dd);
			}
			j2++;
			break;
		case 2:
		case 4:

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < dst_width - 4; j += 4, j2 += 8) {
				x_sd0 = _mm_load_si128((void *)(ps + j2));
				x_sd1 = _mm_load_si128((void *)(ps + j2 + 4));
				x_st0 = _mm_unpacklo_epi32(x_sd0, x_sd1);
				x_st1 = _mm_unpackhi_epi32(x_sd0, x_sd1);
				x_dd = _mm_unpacklo_epi32(x_st0, x_st1);
				_mm_storeu_si128((void *)(pd + j), x_dd);
			}
			break;
		}

		for (; j < dst_width; j++, j2 += 2) {
			pd[j] = ps[j2];
		}
	}
}

void FUNC_NAME(
    2_Nearest) (
    NEAREST_PARAMETERS(DTYPE))
{
	mlib_s32 i;

	for (i = 0; i < dst_height; i++) {
		mlib_s32 *pd = p_dst + i * dst_stride;
		mlib_s32 *ps = p_src + 2 * i * src_stride;
		mlib_s32 j, j2, off;
		__m128i x_sd0, x_sd1, x_st0, x_st1, x_st2, x_dd;

		j = j2 = 0;
		off = (16 - ((mlib_addr)ps & 15)) >> 2;

		switch (off) {
		case 1:
			pd[j] = ps[j2];
			j++;
			j2++;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < dst_width * 2 - 4; j += 4, j2 += 8) {
				x_sd0 = _mm_load_si128((void *)(ps + j2));
				x_sd1 = _mm_load_si128((void *)(ps + j2 + 4));
				x_st0 = _mm_unpacklo_epi32(x_sd0, x_sd1);
				x_st1 = _mm_unpackhi_epi32(x_sd0, x_sd1);
				x_st2 = _mm_srli_si128(x_st1, 8);
				x_dd = _mm_unpacklo_epi32(x_st0, x_st2);
				_mm_storeu_si128((void *)(pd + j), x_dd);
			}
			pd[j] = ps[j2];
			j++;
			j2 += 3;
			break;
		case 2:
			pd[j] = ps[j2];
			pd[j + 1] = ps[j2 + 1];
			j += 2;
			j2 += 2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < dst_width * 2 - 4; j += 4, j2 += 8) {
				x_sd0 = _mm_load_si128((void *)(ps + j2));
				x_sd1 = _mm_load_si128((void *)(ps + j2 + 4));
				x_dd = _mm_unpackhi_epi64(x_sd0, x_sd1);
				_mm_storeu_si128((void *)(pd + j), x_dd);
			}
			j2 += 2;
			break;
		case 3:
			pd[j] = ps[j2];
			pd[j + 1] = ps[j2 + 1];
			j += 2;
			j2 += 3;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < dst_width * 2 - 4; j += 4, j2 += 8) {
				x_sd0 = _mm_load_si128((void *)(ps + j2));
				x_sd1 = _mm_load_si128((void *)(ps + j2 + 4));
				x_st0 = _mm_srli_si128(x_sd0, 4);
				x_st1 = _mm_srli_si128(x_sd1, 4);
				x_dd = _mm_unpacklo_epi64(x_st0, x_st1);
				_mm_storeu_si128((void *)(pd + j), x_dd);
			}
			j2++;
			break;
		case 4:

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < dst_width * 2 - 4; j += 4, j2 += 8) {
				x_sd0 = _mm_load_si128((void *)(ps + j2));
				x_sd1 = _mm_load_si128((void *)(ps + j2 + 4));
				x_dd = _mm_unpacklo_epi64(x_sd0, x_sd1);
				_mm_storeu_si128((void *)(pd + j), x_dd);
			}
			break;
		}

		for (; j < dst_width * 2; j += 2, j2 += 4) {
			pd[j] = ps[j2];
			pd[j + 1] = ps[j2 + 1];
		}
	}
}

void FUNC_NAME(
    3_Nearest) (
    NEAREST_PARAMETERS(DTYPE))
{
	mlib_s32 i, j;

	for (j = 0; j < dst_height; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dst_width; i++) {
			p_dst[3 * i] = p_src[6 * i];
			p_dst[3 * i + 1] = p_src[6 * i + 1];
			p_dst[3 * i + 2] = p_src[6 * i + 2];
		}

		p_src += 2 * src_stride;
		p_dst += dst_stride;
	}
}

void FUNC_NAME(
    4_Nearest) (
    NEAREST_PARAMETERS(DTYPE))
{
	mlib_s32 i;


	for (i = 0; i < dst_height; i++) {
		mlib_s32 *ps = p_src + 2 * i * src_stride;
		mlib_s32 *pd = p_dst + i * dst_stride;
		mlib_s32 j, j2, off;
		__m128i x_sd0, x_sd1, x_st0, x_st1, x_dd, x_mask;

		j = j2 = 0;
		off = (16 - ((mlib_addr)ps & 15)) >> 2;

		switch (off) {
		case 1:
			pd[j] = ps[j2];
			j++;
			j2++;
			x_mask = _mm_setr_epi32(
			    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < dst_width * 4 - 4; j += 4, j2 += 8) {
				x_sd0 = _mm_load_si128((void *)(ps + j2));
				x_sd1 = _mm_load_si128((void *)(ps + j2 + 4));
				x_st0 = _mm_and_si128(x_sd0, x_mask);
				x_st1 = _mm_andnot_si128(x_mask, x_sd1);
				x_dd = _mm_or_si128(x_st0, x_st1);
				_mm_storeu_si128((void *)(pd + j), x_dd);
			}
			pd[j] = ps[j2];
			pd[j + 1] = ps[j2 + 1];
			pd[j + 2] = ps[j2 + 2];
			break;
		case 2:
			pd[j] = ps[j2];
			pd[j + 1] = ps[j2 + 1];
			j += 2;
			j2 += 2;
			x_mask = _mm_setr_epi32(0xFFFFFFFF, 0xFFFFFFFF, 0, 0);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < dst_width * 4 - 4; j += 4, j2 += 8) {
				x_sd0 = _mm_load_si128((void *)(ps + j2));
				x_sd1 = _mm_load_si128((void *)(ps + j2 + 4));
				x_st0 = _mm_and_si128(x_sd0, x_mask);
				x_st1 = _mm_andnot_si128(x_mask, x_sd1);
				x_dd = _mm_or_si128(x_st0, x_st1);
				_mm_storeu_si128((void *)(pd + j), x_dd);
			}
			pd[j] = ps[j2];
			pd[j + 1] = ps[j2 + 1];
			break;
		case 3:
			pd[j] = ps[j2];
			pd[j + 1] = ps[j2 + 1];
			pd[j + 2] = ps[j2 + 2];
			j += 3;
			j2 += 3;
			x_mask = _mm_setr_epi32(0xFFFFFFFF, 0, 0, 0);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < dst_width * 4 - 4; j += 4, j2 += 8) {
				x_sd0 = _mm_load_si128((void *)(ps + j2));
				x_sd1 = _mm_load_si128((void *)(ps + j2 + 4));
				x_st0 = _mm_and_si128(x_sd0, x_mask);
				x_st1 = _mm_andnot_si128(x_mask, x_sd1);
				x_dd = _mm_or_si128(x_st0, x_st1);
				_mm_storeu_si128((void *)(pd + j), x_dd);
			}
			pd[j] = ps[j2];
			break;
		case 4:

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < dst_width * 4; j += 4, j2 += 8) {
				x_sd0 = _mm_load_si128((void *)(ps + j2));
				_mm_storeu_si128((void *)(pd + j), x_sd0);
			}
			break;
		}
	}
}

/* *********************************************************** */
