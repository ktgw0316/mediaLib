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

#pragma ident	"@(#)mlib_s_ImageZoomIn2X_NN.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomIn2X -  image 2X scaling with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoomIn2X(mlib_image       *dst,
 *                                     const mlib_image *src,
 *                                     mlib_filter      filter,
 *                                     mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      filter    Type of resampling filter.
 *      edge      Type of edge condition.
 *
 * DESCRIPTION
 *      The center of the source image is mapped to the center of the
 *      destination image.
 *
 *      The upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *      The resampling filter can be one of the following:
 *              MLIB_NEAREST
 *              MLIB_BILINEAR
 *              MLIB_BICUBIC
 *
 *      The edge condition can be one of the following:
 *              MLIB_EDGE_DST_NO_WRITE  (default)
 *              MLIB_EDGE_DST_FILL_ZERO
 *              MLIB_EDGE_OP_NEAREST
 *              MLIB_EDGE_SRC_EXTEND
 */

#include <mlib_image.h>
#include <mlib_s_ImageZoomIn2X.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	MLIB_ZOOMIN2XDEFPARAM(MLIB_TYPE)                              \
	MLIB_TYPE	*pdst,                                        \
	const MLIB_TYPE	*psrc,                                        \
	mlib_s32	src_width,                                    \
	mlib_s32	src_height,                                   \
	mlib_s32	dst_stride,                                   \
	mlib_s32	src_stride,                                   \
	mlib_s32	src_w_beg,                                    \
	mlib_s32	src_h_beg,                                    \
	mlib_s32	channels

/* *********************************************************** */

#define	MLIB_ZOOMIN2XCALLPARAM(MLIB_TYPE)                             \
	(MLIB_TYPE *)pdst, (MLIB_TYPE *)psrc,                         \
	src_width,  src_height, dst_stride,  src_stride,              \
	src_w_beg, src_h_beg, channels

/* *********************************************************** */

static void mlib_s_ImageZoomIn2X_S16_NN_124(
    MLIB_ZOOMIN2XDEFPARAM(mlib_s16));

static void mlib_s_ImageZoomIn2X_S16_NN_3(
    MLIB_ZOOMIN2XDEFPARAM(mlib_s16));

static void mlib_s_ImageZoomIn2X_U8_NN_124(
    MLIB_ZOOMIN2XDEFPARAM(mlib_u8));

static void mlib_s_ImageZoomIn2X_U8_NN_3(
    MLIB_ZOOMIN2XDEFPARAM(mlib_u8));

/* *********************************************************** */

void
mlib_s_ImageZoomIn2X_U8_NN(
    MLIB_ZOOMIN2XDEFPARAM(mlib_u8))
{
/* indices */
	mlib_s32 i = 0, j, k;

	if ((((mlib_s32)psrc | (mlib_s32)pdst | src_stride | dst_stride |
	    channels) & 1) == 0) {
		dst_stride /= 2;
		src_stride /= 2;
		channels /= 2;
		mlib_s_ImageZoomIn2X_S16_NN(MLIB_ZOOMIN2XCALLPARAM(mlib_s16));

		return;
	}

/* upper bound of image */
	if (src_h_beg & 1) {
		for (k = 0; k < channels; k++) {
			if (j = src_w_beg & 1)
				pdst[k] = psrc[k];
			for (j++; j < src_width; j += 2)
				pdst[j * channels - channels + k] =
				    pdst[j * channels + k] =
				    psrc[(j >> 1) * channels + k];

			if (j == src_width)
				pdst[j * channels - channels + k] =
				    psrc[(j >> 1) * channels + k];
		}

		psrc += src_stride;
		pdst += dst_stride;
		src_height--;
	}

/* left bound of image */
	if (src_w_beg & 1) {
		for (k = 0; k < channels; k++)
			for (i = 0; i < (src_height / 2); i++) {
				pdst[2 * i * dst_stride + k] =
				    pdst[(2 * i + 1) * dst_stride + k] =
				    psrc[i * src_stride + k];
			}

		if (src_height & 1)
			for (k = 0; k < channels; k++)
				pdst[2 * i * dst_stride + k] =
				    psrc[i * src_stride + k];
		psrc += channels;
		pdst += channels;
		src_width--;
	}

/* center of image */
	if (channels == 3) {
		mlib_s_ImageZoomIn2X_U8_NN_3(MLIB_ZOOMIN2XCALLPARAM(mlib_u8));
	} else {
		mlib_s_ImageZoomIn2X_U8_NN_124(MLIB_ZOOMIN2XCALLPARAM(mlib_u8));
	}

	i = src_height / 2;
	j = src_width / 2;

/* lower bound of image */
	if (src_height & 1) {
		for (k = 0; k < channels; k++)
			for (j = 0; j < (src_width / 2); j++) {
				pdst[2 * i * dst_stride + 2 * j * channels +
				    k] =
				    pdst[2 * i * dst_stride + 2 * j * channels +
				    channels + k] =
				    psrc[i * src_stride + j * channels + k];
			}
	}

/* right bound of image */
	if (src_width & 1) {
		for (k = 0; k < channels; k++)
			for (i = 0; i < (src_height / 2); i++) {
				pdst[2 * i * dst_stride +
				    channels * (src_width - 1) + k] =
				    pdst[(2 * i + 1) * dst_stride +
				    channels * (src_width - 1) + k] =
				    psrc[i * src_stride +
				    channels * (src_width / 2) + k];
			}

		if (src_height & 1)
			for (k = 0; k < channels; k++)
				pdst[2 * i * dst_stride +
				    channels * (src_width - 1) + k] =
				    psrc[i * src_stride +
				    channels * (src_width / 2) + k];
	}
}

/* *********************************************************** */

#define	MLIB_S_ImageZoomIn2X_U8_NN_124                    \
	switch (channels) {                               \
	case 1:                                           \
		x_st0 = _mm_unpacklo_epi8(x_sd0, x_sd0);  \
		x_st1 = _mm_unpackhi_epi8(x_sd0, x_sd0);  \
		break;                                    \
	case 2:                                           \
		x_st0 = _mm_unpacklo_epi16(x_sd0, x_sd0); \
		x_st1 = _mm_unpackhi_epi16(x_sd0, x_sd0); \
		break;                                    \
	case 4:                                           \
		x_st0 = _mm_unpacklo_epi32(x_sd0, x_sd0); \
		x_st1 = _mm_unpackhi_epi32(x_sd0, x_sd0); \
		break;                                    \
	}                                                 \

/* *********************************************************** */

#define	DSTIN0	2 * i * dst_stride + 2 * j * channels
#define	DSTIN1	(2 * i + 1) * dst_stride + 2 * j * channels

/* *********************************************************** */

void
mlib_s_ImageZoomIn2X_U8_NN_124(
    MLIB_ZOOMIN2XDEFPARAM(mlib_u8))
{
/* indices */
	mlib_s32 i, j, k, ch;
	__m128i x_sd0, x_st0, x_st1;

	ch = 16 / channels;
	for (i = 0; i < (src_height / 2); i++) {
		if (((((mlib_s32)psrc | src_stride) & 15) == 0) &&
		    ((((mlib_s32)pdst | dst_stride) & 15) == 0)) {
			for (j = 0; j < (src_width / 2 - ch + 1); j += ch) {
				x_sd0 = _mm_load_si128((void *)
				    (psrc + i * src_stride + j * channels));
				MLIB_S_ImageZoomIn2X_U8_NN_124;
				_mm_store_si128(
				    (void *)(pdst + DSTIN0), x_st0);
				_mm_store_si128(
				    (void *)(pdst + DSTIN1), x_st0);
				_mm_store_si128(
				    (void *)(pdst + DSTIN0 + 16), x_st1);
				_mm_store_si128(
				    (void *)(pdst + DSTIN1 + 16), x_st1);
			}
		} else {
			for (j = 0; j < (src_width / 2 - ch + 1); j += ch) {
				x_sd0 = _mm_loadu_si128((void *)
				    (psrc + i * src_stride + j * channels));
				MLIB_S_ImageZoomIn2X_U8_NN_124;
				_mm_storeu_si128(
				    (void *)(pdst + DSTIN0), x_st0);
				_mm_storeu_si128(
				    (void *)(pdst + DSTIN1), x_st0);
				_mm_storeu_si128(
				    (void *)(pdst + DSTIN0 + 16), x_st1);
				_mm_storeu_si128(
				    (void *)(pdst + DSTIN1 + 16), x_st1);
			}
		}

		for (; j < (src_width / 2); j++) {
			for (k = 0; k < channels; k++) {
				mlib_s32 l = k + channels;

				pdst[2 * i * dst_stride + 2 * channels * j +
				    k] =
				    pdst[2 * i * dst_stride + 2 * channels * j +
				    l] =
				    pdst[(2 * i + 1) * dst_stride +
				    2 * channels * j + k] =
				    pdst[(2 * i + 1) * dst_stride +
				    2 * channels * j + l] =
				    psrc[i * src_stride + j * channels + k];
			}
		}
	}
}

/* *********************************************************** */

#define	MLIB_S_ZOOMIN2X_U8_NN_3(ld, st)                              \
	__m128i x_st0_0, x_st0_1, x_st0_2, x_st0_3;                  \
	__m128i x_st1_0, x_st1_1, x_st1_2, x_st1_3;                  \
	__m128i x_st2_0, x_st2_1, x_st2_2, x_st2_3, x_st2_4;         \
                                                                     \
	x_sd0 = _mm_##ld##_si128(                                    \
	    (void *)(psrc + i * src_stride + j * channels));         \
	x_sd1 = _mm_##ld##_si128(                                    \
	    (void *)(psrc + i * src_stride + j * channels + 16));    \
	x_sd2 = _mm_##ld##_si128(                                    \
	    (void *)(psrc + i * src_stride + j * channels + 32));    \
                                                                     \
	x_st0_0 = _mm_and_si128(x_sd0, mask3_13);                    \
	x_st0_1 = _mm_and_si128(_mm_slli_si128(x_sd0, 3), mask6_6);  \
	x_st0_2 = _mm_and_si128(_mm_slli_si128(x_sd0, 6), mask6);    \
	x_st0_3 = _mm_and_si128(_mm_slli_si128(x_sd0, 9), mask1);    \
	x_st0_0 = _mm_or_si128(x_st0_0, x_st0_2);                    \
	x_st0_1 = _mm_or_si128(x_st0_1, x_st0_3);                    \
	x_st0 = _mm_or_si128(x_st0_0, x_st0_1);                      \
                                                                     \
	x_st1_0 = _mm_and_si128(_mm_srli_si128(x_sd0, 7), mask5);    \
	x_st1_1 = _mm_and_si128(_mm_srli_si128(x_sd0, 4), mask6_4);  \
	x_st1_2 = _mm_and_si128(_mm_srli_si128(x_sd0, 1), mask4);    \
	x_st1_3 = _mm_and_si128(_mm_slli_si128(x_sd1, 15), mask1);   \
	x_st1_0 = _mm_or_si128(x_st1_0, x_st1_2);                    \
	x_st1_1 = _mm_or_si128(x_st1_1, x_st1_3);                    \
	x_st1 = _mm_or_si128(x_st1_0, x_st1_1);                      \
                                                                     \
	x_st2_0 = _mm_and_si128(_mm_slli_si128(x_sd1, 8), mask3);    \
	x_st2_1 = _mm_and_si128(_mm_slli_si128(x_sd1, 5), mask6_2);  \
	x_st2_2 = _mm_and_si128(_mm_slli_si128(x_sd1, 2), mask5_2);  \
	x_st2_3 = _mm_and_si128(_mm_srli_si128(x_sd0, 14), mask1_14);\
	x_st2_4 = _mm_and_si128(_mm_srli_si128(x_sd1, 1), mask1_15); \
	x_st2_0 = _mm_or_si128(x_st2_0, x_st2_2);                    \
	x_st2_1 = _mm_or_si128(x_st2_1, x_st2_3);                    \
	x_st2 = _mm_or_si128(x_st2_0, x_st2_1);                      \
	x_st2 = _mm_or_si128(x_st2, x_st2_4);                        \
                                                                     \
	_mm_##st##_si128((__m128i *)(pdst + 2 * i * dst_stride +     \
	    2 * j * channels), x_st0);                               \
	_mm_##st##_si128(((__m128i *)(pdst + 2 * i * dst_stride +    \
	    2 * j * channels) + 1), x_st1);                          \
	_mm_##st##_si128(((__m128i *)(pdst + 2 * i * dst_stride +    \
	    2 * j * channels) + 2), x_st2);                          \
	_mm_##st##_si128((__m128i *)(pdst +                          \
	    (2 * i + 1) * dst_stride + 2 * j * channels), x_st0);    \
	_mm_##st##_si128(((__m128i *)(pdst + (2 * i + 1) *           \
	    dst_stride + 2 * j * channels) + 1), x_st1);             \
	_mm_##st##_si128(((__m128i *)(pdst + (2 * i + 1) *           \
	    dst_stride + 2 * j * channels) + 2), x_st2);             \
                                                                     \
	x_st2_0 = _mm_and_si128(_mm_srli_si128(x_sd1, 8), mask3_13); \
	x_st2_1 = _mm_and_si128(_mm_srli_si128(x_sd1, 5), mask6_6);  \
	x_st2_2 = _mm_and_si128(_mm_srli_si128(x_sd1, 2), mask5_9);  \
	x_st2_3 = _mm_and_si128(_mm_slli_si128(x_sd1, 1), mask1);    \
	x_st2_4 = _mm_and_si128(_mm_slli_si128(x_sd2, 14), mask1_1); \
	x_st2_0 = _mm_or_si128(x_st2_0, x_st2_2);                    \
	x_st2_1 = _mm_or_si128(x_st2_1, x_st2_3);                    \
	x_st2 = _mm_or_si128(x_st2_0, x_st2_1);                      \
	x_st2 = _mm_or_si128(x_st2, x_st2_4);                        \
                                                                     \
	x_st1_0 = _mm_and_si128(_mm_slli_si128(x_sd2, 7), mask5_11); \
	x_st1_1 = _mm_and_si128(_mm_slli_si128(x_sd2, 4), mask6_4);  \
	x_st1_2 = _mm_and_si128(_mm_slli_si128(x_sd2, 1), mask4_10); \
	x_st1_3 = _mm_srli_si128(x_sd1, 15);                         \
	x_st1_0 = _mm_or_si128(x_st1_0, x_st1_2);                    \
	x_st1_1 = _mm_or_si128(x_st1_1, x_st1_3);                    \
	x_st1 = _mm_or_si128(x_st1_0, x_st1_1);                      \
                                                                     \
	x_st0_0 = _mm_and_si128(x_sd2, mask3);                       \
	x_st0_1 = _mm_and_si128(_mm_srli_si128(x_sd2, 3), mask6_2);  \
	x_st0_2 = _mm_and_si128(_mm_srli_si128(x_sd2, 6), mask6_8);  \
	x_st0_3 = _mm_and_si128(_mm_srli_si128(x_sd2, 9), mask1_15); \
	x_st0_0 = _mm_or_si128(x_st0_0, x_st0_2);                    \
	x_st0_1 = _mm_or_si128(x_st0_1, x_st0_3);                    \
	x_st0 = _mm_or_si128(x_st0_0, x_st0_1);                      \
                                                                     \
	_mm_##st##_si128(((__m128i *)(pdst + 2 * i * dst_stride +    \
	    2 * j * channels) + 3), x_st2);                          \
	_mm_##st##_si128(((__m128i *)(pdst + 2 * i * dst_stride +    \
	    2 * j * channels) + 4), x_st1);                          \
	_mm_##st##_si128(((__m128i *)(pdst + 2 * i * dst_stride +    \
	    2 * j * channels) + 5), x_st0);                          \
	_mm_##st##_si128(((__m128i *)(pdst + (2 * i + 1) *           \
	    dst_stride + 2 * j * channels) + 3), x_st2);             \
	_mm_##st##_si128(((__m128i *)(pdst + (2 * i + 1) *           \
	    dst_stride + 2 * j * channels) + 4), x_st1);             \
	_mm_##st##_si128(((__m128i *)(pdst + (2 * i + 1) *           \
	    dst_stride + 2 * j * channels) + 5), x_st0)

/* *********************************************************** */

void
mlib_s_ImageZoomIn2X_U8_NN_3(
    MLIB_ZOOMIN2XDEFPARAM(mlib_u8))
{
/* indices */
	mlib_s32 i, j, k, ch;
	__m128i x_sd0, x_sd1, x_sd2;
	__m128i x_st0, x_st1, x_st2;
	__m128i mask1, mask3, mask4, mask5, mask6;
	mask1 = _mm_set_epi16(0xff00, 0, 0, 0, 0, 0, 0, 0);
	mask3 = _mm_set_epi16(0xffff, 0xff00, 0, 0, 0, 0, 0, 0);
	mask4 = _mm_set_epi16(0x00ff, 0xffff, 0xff00, 0, 0, 0, 0, 0);
	mask5 = _mm_set_epi16(0, 0, 0, 0, 0, 0x00ff, 0xffff, 0xffff);
	mask6 = _mm_set_epi16(0x00ff, 0xffff, 0xffff, 0xff00, 0, 0, 0, 0);
	__m128i mask1_1 = _mm_srli_si128(mask1, 1);
	__m128i mask1_14 = _mm_srli_si128(mask1, 14);
	__m128i mask1_15 = _mm_srli_si128(mask1, 15);
	__m128i mask3_13 = _mm_srli_si128(mask3, 13);
	__m128i mask4_10 = _mm_srli_si128(mask4, 10);
	__m128i mask5_2 = _mm_slli_si128(mask5, 2);
	__m128i mask5_9 = _mm_slli_si128(mask5, 9);
	__m128i mask5_11 = _mm_slli_si128(mask5, 11);
	__m128i mask6_2 = _mm_srli_si128(mask6, 2);
	__m128i mask6_4 = _mm_srli_si128(mask6, 4);
	__m128i mask6_6 = _mm_srli_si128(mask6, 6);
	__m128i mask6_8 = _mm_srli_si128(mask6, 8);

	for (i = 0; i < (src_height / 2); i++) {
		if (((((mlib_s32)psrc | src_stride) & 15) == 0) &&
		    ((((mlib_s32)pdst | dst_stride) & 15) == 0)) {
			for (j = 0; j < (src_width / 2 - 15); j += 16) {
				MLIB_S_ZOOMIN2X_U8_NN_3(load, store);
			}

		} else {
			for (j = 0; j < (src_width / 2 - 15); j += 16) {
				MLIB_S_ZOOMIN2X_U8_NN_3(loadu, storeu);
			}
		}

		for (; j < (src_width / 2); j++) {
			for (k = 0; k < channels; k++) {
				mlib_s32 l = k + channels;

				pdst[2 * i * dst_stride + 2 * channels * j +
				    k] =
				    pdst[2 * i * dst_stride + 2 * channels * j +
				    l] =
				    pdst[(2 * i + 1) * dst_stride +
				    2 * channels * j + k] =
				    pdst[(2 * i + 1) * dst_stride +
				    2 * channels * j + l] =
				    psrc[i * src_stride + j * channels + k];
			}
		}
	}
}

/* *********************************************************** */

void
mlib_s_ImageZoomIn2X_S16_NN(
    MLIB_ZOOMIN2XDEFPARAM(mlib_s16))
{
/* indices */
	mlib_s32 i = 0, j, k;

	if ((((((mlib_s32)psrc | (mlib_s32)pdst) >> 1) | src_stride | dst_stride
	    | channels) & 1) == 0) {
		dst_stride /= 2;
		src_stride /= 2;
		channels /= 2;
		mlib_ImageZoomIn2X_S32_NN(MLIB_ZOOMIN2XCALLPARAM(mlib_s32));

		return;
	}

/* upper bound of image */
	if (src_h_beg & 1) {
		for (k = 0; k < channels; k++) {
			if (j = src_w_beg & 1)
				pdst[k] = psrc[k];
			for (j++; j < src_width; j += 2)
				pdst[j * channels - channels + k] =
				    pdst[j * channels + k] =
				    psrc[(j >> 1) * channels + k];

			if (j == src_width)
				pdst[j * channels - channels + k] =
				    psrc[(j >> 1) * channels + k];
		}

		psrc += src_stride;
		pdst += dst_stride;
		src_height--;
	}

/* left bound of image */
	if (src_w_beg & 1) {
		for (k = 0; k < channels; k++)
			for (i = 0; i < (src_height / 2); i++) {
				pdst[2 * i * dst_stride + k] =
				    pdst[(2 * i + 1) * dst_stride + k] =
				    psrc[i * src_stride + k];
			}

		if (src_height & 1)
			for (k = 0; k < channels; k++)
				pdst[2 * i * dst_stride + k] =
				    psrc[i * src_stride + k];
		psrc += channels;
		pdst += channels;
		src_width--;
	}

	if (channels == 3) {
		mlib_s_ImageZoomIn2X_S16_NN_3(
		    MLIB_ZOOMIN2XCALLPARAM(mlib_s16));
	} else {
		mlib_s_ImageZoomIn2X_S16_NN_124(
		    MLIB_ZOOMIN2XCALLPARAM(mlib_s16));
	}

	i = src_height / 2;

/* lower bound of image */
	if (src_height & 1) {
		for (k = 0; k < channels; k++)
			for (j = 0; j < (src_width / 2); j++) {
				pdst[2 * i * dst_stride + 2 * j * channels +
				    k] =
				    pdst[2 * i * dst_stride + 2 * j * channels +
				    channels + k] =
				    psrc[i * src_stride + j * channels + k];
			}
	}

/* right bound of image */
	if (src_width & 1) {
		for (k = 0; k < channels; k++)
			for (i = 0; i < (src_height / 2); i++) {
				pdst[2 * i * dst_stride +
				    channels * (src_width - 1) + k] =
				    pdst[(2 * i + 1) * dst_stride +
				    channels * (src_width - 1) + k] =
				    psrc[i * src_stride +
				    channels * (src_width / 2) + k];
			}

		if (src_height & 1)
			for (k = 0; k < channels; k++)
				pdst[2 * i * dst_stride +
				    channels * (src_width - 1) + k] =
				    psrc[i * src_stride +
				    channels * (src_width / 2) + k];
	}
}

/* *********************************************************** */

#define	MLIB_S_ImageZoomIn2X_S16_NN_124                   \
	switch (channels) {                               \
	case 1:                                           \
		x_st0 = _mm_unpacklo_epi16(x_sd0, x_sd0); \
		x_st1 = _mm_unpackhi_epi16(x_sd0, x_sd0); \
		break;                                    \
	case 2:                                           \
		x_st0 = _mm_unpacklo_epi32(x_sd0, x_sd0); \
		x_st1 = _mm_unpackhi_epi32(x_sd0, x_sd0); \
		break;                                    \
	case 4:                                           \
		x_st0 = _mm_unpacklo_epi64(x_sd0, x_sd0); \
		x_st1 = _mm_unpackhi_epi64(x_sd0, x_sd0); \
		break;                                    \
	}                                                 \

/* *********************************************************** */

void
mlib_s_ImageZoomIn2X_S16_NN_124(
    MLIB_ZOOMIN2XDEFPARAM(mlib_s16))
{
/* indices */
	mlib_s32 i, j, ch, k;
	__m128i x_sd0, x_st0, x_st1;

	ch = 8 / channels;
	for (i = 0; i < (src_height / 2); i++) {
		if (((((mlib_s32)psrc | src_stride) & 15) == 0) &&
		    ((((mlib_s32)pdst | dst_stride) & 15) == 0)) {
			for (j = 0; j < (src_width / 2 - ch + 1); j += ch) {
				x_sd0 = _mm_load_si128((void *)
				    (psrc + i * src_stride + j * channels));
				MLIB_S_ImageZoomIn2X_S16_NN_124;
				_mm_store_si128(
				    (void *)(pdst + DSTIN0), x_st0);
				_mm_store_si128(
				    (void *)(pdst + DSTIN1), x_st0);
				_mm_store_si128(
				    (void *)(pdst + DSTIN0 + 8), x_st1);
				_mm_store_si128(
				    (void *)(pdst + DSTIN1 + 8), x_st1);
			}
		} else {
			for (j = 0; j < (src_width / 2 - ch + 1); j += ch) {
				x_sd0 = _mm_loadu_si128((void *)
				    (psrc + i * src_stride + j * channels));
				MLIB_S_ImageZoomIn2X_S16_NN_124;
				_mm_storeu_si128(
				    (void *)(pdst + DSTIN0), x_st0);
				_mm_storeu_si128(
				    (void *)(pdst + DSTIN1), x_st0);
				_mm_storeu_si128(
				    (void *)(pdst + DSTIN0 + 8), x_st1);
				_mm_storeu_si128(
				    (void *)(pdst + DSTIN1 + 8), x_st1);
			}
		}

		for (; j < (src_width / 2); j++) {
			for (k = 0; k < channels; k++) {
				mlib_s32 l = k + channels;

					pdst[2 * i * dst_stride +
					    2 * channels * j + k] =
					    pdst[2 * i * dst_stride +
					    2 * channels * j + l] =
					    pdst[(2 * i + 1) * dst_stride +
					    2 * channels * j + k] =
					    pdst[(2 * i + 1) * dst_stride +
					    2 * channels * j + l] =
					    psrc[i * src_stride + j * channels +
					    k];
			}
		}
	}
}

/* *********************************************************** */

#define	MLIB_S_ImageZoomIn2X_S16_NN_3(ld, st)                           \
	x_sd0 = _mm_##ld##_si128(                                       \
	    (void *)(psrc + i * src_stride + j * channels));            \
	x_sd1 = _mm_##ld##_si128(                                       \
	    (void *)(psrc + i * src_stride + j * channels + 8));        \
	x_sd2 = _mm_##ld##_si128(                                       \
	    (void *)(psrc + i * src_stride + j * channels + 16));       \
                                                                        \
	x_st0_0 = _mm_slli_si128(x_sd0, 6);                             \
	x_st0 = _mm_or_si128(x_st0_0, _mm_and_si128(x_sd0, mask0));     \
                                                                        \
	x_st1_0 = _mm_and_si128(mask1_0, _mm_srli_si128(x_sd0, 4));     \
	x_st1_1 = _mm_and_si128(mask1_1, _mm_srli_si128(x_sd0, 10));    \
	x_st1_2 = _mm_and_si128(mask1_2, _mm_slli_si128(x_sd0, 2));     \
	x_st1_3 = _mm_and_si128(mask1_3, _mm_slli_si128(x_sd1, 12));    \
	x_st1_0 = _mm_or_si128(x_st1_0, x_st1_2);                       \
	x_st1_1 = _mm_or_si128(x_st1_1, x_st1_3);                       \
	x_st1 = _mm_or_si128(x_st1_0, x_st1_1);                         \
                                                                        \
	x_st2_0 = _mm_and_si128(mask2_0, _mm_slli_si128(x_sd1, 2));     \
	x_st2_1 = _mm_and_si128(mask2_1, _mm_slli_si128(x_sd1, 8));     \
	x_st2_2 = _mm_srli_si128(x_sd0, 14);                            \
	x_st2_0 = _mm_or_si128(x_st2_0, x_st2_1);                       \
	x_st2 = _mm_or_si128(x_st2_0, x_st2_2);                         \
                                                                        \
	_mm_##st##_si128(((__m128i *)(pdst + 2 * i * dst_stride +       \
	    2 * j * channels) + 0), x_st0);                             \
	_mm_##st##_si128(((__m128i *)(pdst + 2 * i * dst_stride +       \
	    2 * j * channels) + 1), x_st1);                             \
	_mm_##st##_si128(((__m128i *)(pdst + 2 * i * dst_stride +       \
	    2 * j * channels) + 2), x_st2);                             \
	_mm_##st##_si128(((__m128i *)(pdst + (2 * i + 1) * dst_stride + \
	    2 * j * channels) + 0), x_st0);                             \
	_mm_##st##_si128(((__m128i *)(pdst + (2 * i + 1) * dst_stride + \
	    2 * j * channels) + 1), x_st1);                             \
	_mm_##st##_si128(((__m128i *)(pdst + (2 * i + 1) * dst_stride + \
	    2 * j * channels) + 2), x_st2);                             \
                                                                        \
	x_st2_0 = _mm_and_si128(mask0, _mm_srli_si128(x_sd1, 8));       \
	x_st2_1 = _mm_and_si128(mask2_0_l4, _mm_srli_si128(x_sd1, 2));  \
	x_st2_2 = _mm_slli_si128(x_sd2, 14);                            \
	x_st2_0 = _mm_or_si128(x_st2_0, x_st2_1);                       \
	x_st2 = _mm_or_si128(x_st2_0, x_st2_2);                         \
                                                                        \
	x_st1_0 = _mm_and_si128(mask1_0_l2, _mm_slli_si128(x_sd2, 4));  \
	x_st1_1 = _mm_and_si128(mask1_2, _mm_slli_si128(x_sd2, 10));    \
	x_st1_2 = _mm_and_si128(mask1_1, _mm_srli_si128(x_sd2, 2));     \
	x_st1_3 = _mm_and_si128(mask1_1_l2, _mm_srli_si128(x_sd1, 12)); \
	x_st1_0 = _mm_or_si128(x_st1_0, x_st1_2);                       \
	x_st1_1 = _mm_or_si128(x_st1_1, x_st1_3);                       \
	x_st1 = _mm_or_si128(x_st1_0, x_st1_1);                         \
                                                                        \
	x_st0_0 = _mm_srli_si128(x_sd2, 6);                             \
	x_st0 = _mm_or_si128(x_st0_0, _mm_and_si128(x_sd2, mask2_1));   \
                                                                        \
	_mm_##st##_si128(((__m128i *)(pdst + 2 * i * dst_stride +       \
	    2 * j * channels) + 3), x_st2);                             \
	_mm_##st##_si128(((__m128i *)(pdst + 2 * i * dst_stride +       \
	    2 * j * channels) + 4), x_st1);                             \
	_mm_##st##_si128(((__m128i *)(pdst + 2 * i * dst_stride +       \
	    2 * j * channels) + 5), x_st0);                             \
	_mm_##st##_si128(((__m128i *)(pdst + (2 * i + 1) * dst_stride + \
	    2 * j * channels) + 3), x_st2);                             \
	_mm_##st##_si128(((__m128i *)(pdst + (2 * i + 1) * dst_stride + \
	    2 * j * channels) + 4), x_st1);                             \
	_mm_##st##_si128(((__m128i *)(pdst + (2 * i + 1) * dst_stride + \
	    2 * j * channels) + 5), x_st0)

/* *********************************************************** */

void
mlib_s_ImageZoomIn2X_S16_NN_3(
    MLIB_ZOOMIN2XDEFPARAM(mlib_s16))
{
/* indices */
	mlib_s32 i, j, ch, k;

	__m128i x_sd0, x_sd1, x_sd2;
	__m128i mask0, mask1_0, mask1_1, mask1_2, mask1_3, mask2_0, mask2_1;
	__m128i x_st0, x_st0_0;
	__m128i x_st1, x_st1_0, x_st1_1, x_st1_2, x_st1_3;
	__m128i x_st2, x_st2_0, x_st2_1, x_st2_2;

	mask0 = _mm_set_epi32(0, 0, 0x00000FFFF, 0xFFFFFFFF);
	mask1_0 = _mm_set_epi32(0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000);
	mask1_1 = _mm_set_epi32(0, 0, 0, 0x0000FFFF);
	mask1_2 = _mm_set_epi32(0xFFFF0000, 0, 0, 0);
	mask1_3 = _mm_set_epi32(0x0000FFFF, 0, 0, 0);
	mask2_0 = _mm_set_epi32(0, 0x0000FFFF, 0xFFFFFFFF, 0xFFFF0000);
	mask2_1 = _mm_set_epi32(0xFFFFFFFF, 0xFFFF0000, 0, 0);
	__m128i mask2_0_l4 = _mm_slli_si128(mask2_0, 4);
	__m128i mask1_0_l2 = _mm_slli_si128(mask1_0, 2);
	__m128i mask1_1_l2 = _mm_slli_si128(mask1_1, 2);

	for (i = 0; i < (src_height / 2); i++) {
		if (((((mlib_s32)psrc | src_stride) & 15) == 0) &&
		    ((((mlib_s32)pdst | dst_stride) & 15) == 0)) {
			for (j = 0; j < (src_width / 2 - 7); j += 8) {
				MLIB_S_ImageZoomIn2X_S16_NN_3(load, store);
			}
		} else {
			for (j = 0; j < (src_width / 2 - 7); j += 8) {
				MLIB_S_ImageZoomIn2X_S16_NN_3(loadu, storeu);
			}
		}

		for (; j < (src_width / 2); j++) {
			for (k = 0; k < channels; k++) {
				mlib_s32 l = k + channels;

					pdst[2 * i * dst_stride +
					    2 * channels * j + k] =
					    pdst[2 * i * dst_stride +
					    2 * channels * j + l] =
					    pdst[(2 * i + 1) * dst_stride +
					    2 * channels * j + k] =
					    pdst[(2 * i + 1) * dst_stride +
					    2 * channels * j + l] =
					    psrc[i * src_stride + j * channels +
					    k];
			}
		}
	}
}

/* *********************************************************** */
