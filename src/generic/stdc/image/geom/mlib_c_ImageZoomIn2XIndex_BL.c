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

#pragma ident	"@(#)mlib_c_ImageZoomIn2XIndex_BL.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomIn2XIndex - indexed color image 2X scaling
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoomIn2XIndex(mlib_image       *dst,
 *                                          const mlib_image *src,
 *                                          mlib_filter      filter,
 *                                          mlib_edge        edge,
 *                                          const void       *colormap)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image
 *      src       Pointer to source image
 *      filter    Type of resampling filter.
 *      edge      Type of edge conditions.
 *      colormap  Internal data structure for inverse color mapping.
 *
 * DESCRIPTION
 *      The center of the source image is mapped to the center of the
 *      destination image.
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

/* *********************************************************** */

/*								*/
/*			filter = BILINEAR			*/
/*			(Internal functions)			*/
/*								*/

#include <mlib_image.h>
#include <mlib_ImageColormap.h>
#include <mlib_ImageZoomIn2XIndex.h>

/* *********************************************************** */

#define	MLIB_ZOOMIN2XINDEXDEFPARAM(MLIB_TYPE)                         \
	MLIB_TYPE	*pdst,                                        \
	const MLIB_TYPE	*psrc,                                        \
	mlib_s32	src_width,                                    \
	mlib_s32	src_height,                                   \
	mlib_s32	dst_stride,                                   \
	mlib_s32	src_stride,                                   \
	mlib_s32	src_w_beg,                                    \
	mlib_s32	src_h_beg,                                    \
	const void	*colormap

/* *********************************************************** */

#define	CLEAR_ONE_BIT	0x7F7F7F7F
#define	CLEAR_TWO_BIT	0x3F3F3F3F

#define	ONE	0x01010101

/* *********************************************************** */

#define	MLIB_ZOOMIN2XINDEX_U8_BL_FL0(dst_type, nchannels)                   \
	{                                                                   \
	    mlib_s32 i, j = 0, k, count, tmp, res;                          \
	    mlib_s32 a00, a01, a10, a11;                                    \
	    mlib_s32 a00_2, a01_2, a10_2, a11_2;                            \
	    mlib_s32 a00_4, a01_4, a10_4, a11_4;                            \
	                                                                    \
	    if (src_h_beg & 1) {                                            \
	                                                                    \
/*                                                                          \
 * upper bound of image                                                     \
 */                                                                         \
		a00 = *(lut + psrc[0]);                                     \
		a10 = *(lut + psrc[src_stride]);                            \
		a01 = *(lut + psrc[1]);                                     \
		a11 = *(lut + psrc[src_stride + 1]);                        \
		a00_2 = (a00 >> 1) & CLEAR_ONE_BIT;                         \
		a00_4 = (a00 >> 2) & CLEAR_TWO_BIT;                         \
		a01_2 = (a01 >> 1) & CLEAR_ONE_BIT;                         \
		a01_4 = (a01 >> 2) & CLEAR_TWO_BIT;                         \
		a10_2 = (a10 >> 1) & CLEAR_ONE_BIT;                         \
		a10_4 = (a10 >> 2) & CLEAR_TWO_BIT;                         \
		a11_2 = (a11 >> 1) & CLEAR_ONE_BIT;                         \
		a11_4 = (a11 >> 2) & CLEAR_TWO_BIT;                         \
		count = 0;                                                  \
		j = src_w_beg & 1;                                          \
		if (j) {                                                    \
		    tmp =                                                   \
			(((a00_4 + a10_4 + a01_4 +                          \
			a11_4) >> 2) & CLEAR_TWO_BIT) + ONE;                \
		    res =                                                   \
			a11_2 + (((a10_2 +                                  \
			a01_2) >> 2) & CLEAR_TWO_BIT) + tmp;                \
		    dst_buf1[0] = res;                                      \
		    a00_2 = a01_2;                                          \
		    a00_4 = a01_4;                                          \
		    a10_2 = a11_2;                                          \
		    a10_4 = a11_4;                                          \
		    count++;                                                \
		}                                                           \
		for (j++; j < src_width; j += 2) {                          \
		    a01 = *(lut + psrc[(j >> 1) + 1]);                      \
		    a11 = *(lut + psrc[src_stride + (j >> 1) + 1]);         \
		    a01_2 = (a01 >> 1) & CLEAR_ONE_BIT;                     \
		    a01_4 = (a01 >> 2) & CLEAR_TWO_BIT;                     \
		    a11_2 = (a11 >> 1) & CLEAR_ONE_BIT;                     \
		    a11_4 = (a11 >> 2) & CLEAR_TWO_BIT;                     \
		    tmp =                                                   \
			(((a00_4 + a10_4 + a01_4 +                          \
			a11_4) >> 2) & CLEAR_TWO_BIT) + ONE;                \
		    res =                                                   \
			a10_2 + (((a00_2 +                                  \
			a11_2) >> 2) & CLEAR_TWO_BIT) + tmp;                \
		    dst_buf1[count] = res;                                  \
		    res =                                                   \
			a11_2 + (((a10_2 +                                  \
			a01_2) >> 2) & CLEAR_TWO_BIT) + tmp;                \
		    dst_buf1[count + 1] = res;                              \
		    a00_2 = a01_2;                                          \
		    a00_4 = a01_4;                                          \
		    a10_2 = a11_2;                                          \
		    a10_4 = a11_4;                                          \
		    count += 2;                                             \
		}                                                           \
		if (j == src_width) {                                       \
		    a01 = *(lut + psrc[(j >> 1) + 1]);                      \
		    a11 = *(lut + psrc[src_stride + (j >> 1) + 1]);         \
		    a01_2 = (a01 >> 1) & CLEAR_ONE_BIT;                     \
		    a01_4 = (a01 >> 2) & CLEAR_TWO_BIT;                     \
		    a11_2 = (a11 >> 1) & CLEAR_ONE_BIT;                     \
		    a11_4 = (a11 >> 2) & CLEAR_TWO_BIT;                     \
		    tmp =                                                   \
			(((a00_4 + a10_4 + a01_4 +                          \
			a11_4) >> 2) & CLEAR_TWO_BIT) + ONE;                \
		    res =                                                   \
			a10_2 + (((a00_2 +                                  \
			a11_2) >> 2) & CLEAR_TWO_BIT) + tmp;                \
		    dst_buf1[count] = res;                                  \
		    count++;                                                \
		}                                                           \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_##nchannels   \
		    ((mlib_u8 *)dst_buf1, pdst, count, colormap);           \
		psrc += src_stride;                                         \
		pdst += dst_stride;                                         \
		src_height--;                                               \
	    }                                                               \
	    if (src_w_beg & 1) {                                            \
	                                                                    \
/*                                                                          \
 * left bound of image                                                      \
 */                                                                         \
		a00 = *(lut + psrc[0]);                                     \
		a01 = *(lut + psrc[1]);                                     \
		a00_2 = (a00 >> 1) & CLEAR_ONE_BIT;                         \
		a00_4 = (a00 >> 2) & CLEAR_TWO_BIT;                         \
		a01_2 = (a01 >> 1) & CLEAR_ONE_BIT;                         \
		a01_4 = (a01 >> 2) & CLEAR_TWO_BIT;                         \
		count = 0;                                                  \
		for (i = 0; i < (src_height / 2); i++) {                    \
		    a10 = *(lut + psrc[(i + 1) * src_stride]);              \
		    a11 = *(lut + psrc[(i + 1) * src_stride + 1]);          \
		    a10_2 = (a10 >> 1) & CLEAR_ONE_BIT;                     \
		    a10_4 = (a10 >> 2) & CLEAR_TWO_BIT;                     \
		    a11_2 = (a11 >> 1) & CLEAR_ONE_BIT;                     \
		    a11_4 = (a11 >> 2) & CLEAR_TWO_BIT;                     \
		    tmp =                                                   \
			(((a00_4 + a10_4 + a01_4 +                          \
			a11_4) >> 2) & CLEAR_TWO_BIT) + ONE;                \
		    res =                                                   \
			a01_2 + (((a00_2 +                                  \
			a11_2) >> 2) & CLEAR_TWO_BIT) + tmp;                \
		    dst_buf1[count] = res;                                  \
		    res =                                                   \
			a11_2 + (((a01_2 +                                  \
			a10_2) >> 2) & CLEAR_TWO_BIT) + tmp;                \
		    dst_buf1[count + 1] = res;                              \
		    a00_2 = a10_2;                                          \
		    a00_4 = a10_4;                                          \
		    a01_2 = a11_2;                                          \
		    a01_4 = a11_4;                                          \
		    count += 2;                                             \
		}                                                           \
		if (src_height & 1) {                                       \
		    a10 = *(lut + psrc[(i + 1) * src_stride]);              \
		    a11 = *(lut + psrc[(i + 1) * src_stride + 1]);          \
		    a10_2 = (a10 >> 1) & CLEAR_ONE_BIT;                     \
		    a10_4 = (a10 >> 2) & CLEAR_TWO_BIT;                     \
		    a11_2 = (a11 >> 1) & CLEAR_ONE_BIT;                     \
		    a11_4 = (a11 >> 2) & CLEAR_TWO_BIT;                     \
		    tmp =                                                   \
			(((a00_4 + a10_4 + a01_4 +                          \
			a11_4) >> 2) & CLEAR_TWO_BIT) + ONE;                \
		    res =                                                   \
			a01_2 + (((a00_2 +                                  \
			a11_2) >> 2) & CLEAR_TWO_BIT) + tmp;                \
		    dst_buf1[count] = res;                                  \
		    count++;                                                \
		}                                                           \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_##nchannels   \
		    ((mlib_u8 *)dst_buf1, pdst_buf, count, colormap);       \
		for (k = 0; k < count; k++)                                 \
		    pdst[k * dst_stride] = pdst_buf[k];                     \
		psrc += 1;                                                  \
		pdst += 1;                                                  \
		src_width--;                                                \
	    }                                                               \
	    for (i = 0; i < (src_height / 2); i++) {                        \
		a00 = *(lut + psrc[i * src_stride]);                        \
		a10 = *(lut + psrc[(i + 1) * src_stride]);                  \
		a00_2 = (a00 >> 1) & CLEAR_ONE_BIT;                         \
		a00_4 = (a00 >> 2) & CLEAR_TWO_BIT;                         \
		a10_2 = (a10 >> 1) & CLEAR_ONE_BIT;                         \
		a10_4 = (a10 >> 2) & CLEAR_TWO_BIT;                         \
		count = 0;                                                  \
		for (j = 0; j < (src_width / 2); j++) {                     \
		    a01 = *(lut + psrc[i * src_stride + j + 1]);            \
		    a11 = *(lut + psrc[(i + 1) * src_stride + j + 1]);      \
		    a01_2 = (a01 >> 1) & CLEAR_ONE_BIT;                     \
		    a01_4 = (a01 >> 2) & CLEAR_TWO_BIT;                     \
		    a11_2 = (a11 >> 1) & CLEAR_ONE_BIT;                     \
		    a11_4 = (a11 >> 2) & CLEAR_TWO_BIT;                     \
		    tmp =                                                   \
			(((a00_4 + a10_4 + a01_4 +                          \
			a11_4) >> 2) & CLEAR_TWO_BIT) + ONE;                \
		    res =                                                   \
			a00_2 + (((a01_2 +                                  \
			a10_2) >> 2) & CLEAR_TWO_BIT) + tmp;                \
		    dst_buf1[count] = res;                                  \
		    res =                                                   \
			a01_2 + (((a00_2 +                                  \
			a11_2) >> 2) & CLEAR_TWO_BIT) + tmp;                \
		    dst_buf1[count + 1] = res;                              \
		    res =                                                   \
			a10_2 + (((a00_2 +                                  \
			a11_2) >> 2) & CLEAR_TWO_BIT) + tmp;                \
		    dst_buf2[count] = res;                                  \
		    res =                                                   \
			a11_2 + (((a01_2 +                                  \
			a10_2) >> 2) & CLEAR_TWO_BIT) + tmp;                \
		    dst_buf2[count + 1] = res;                              \
		    a00_2 = a01_2;                                          \
		    a00_4 = a01_4;                                          \
		    a10_2 = a11_2;                                          \
		    a10_4 = a11_4;                                          \
		    count += 2;                                             \
		}                                                           \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_##nchannels   \
		    ((mlib_u8 *)dst_buf1, pdst + 2 * i * dst_stride,        \
		    count, colormap);                                       \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_##nchannels   \
		    ((mlib_u8 *)dst_buf2,                                   \
		    pdst + (2 * i + 1) * dst_stride, count, colormap);      \
	    }                                                               \
	    if (src_height & 1) {                                           \
	                                                                    \
/*                                                                          \
 * lower bound of image                                                     \
 */                                                                         \
		a00 = *(lut + psrc[i * src_stride]);                        \
		a10 = *(lut + psrc[(i + 1) * src_stride]);                  \
		a00_2 = (a00 >> 1) & CLEAR_ONE_BIT;                         \
		a00_4 = (a00 >> 2) & CLEAR_TWO_BIT;                         \
		a10_2 = (a10 >> 1) & CLEAR_ONE_BIT;                         \
		a10_4 = (a10 >> 2) & CLEAR_TWO_BIT;                         \
		count = 0;                                                  \
		for (j = 0; j < (src_width / 2); j++) {                     \
		    a01 = *(lut + psrc[i * src_stride + j + 1]);            \
		    a11 = *(lut + psrc[(i + 1) * src_stride + j + 1]);      \
		    a01_2 = (a01 >> 1) & CLEAR_ONE_BIT;                     \
		    a01_4 = (a01 >> 2) & CLEAR_TWO_BIT;                     \
		    a11_2 = (a11 >> 1) & CLEAR_ONE_BIT;                     \
		    a11_4 = (a11 >> 2) & CLEAR_TWO_BIT;                     \
		    tmp =                                                   \
			(((a00_4 + a10_4 + a01_4 +                          \
			a11_4) >> 2) & CLEAR_TWO_BIT) + ONE;                \
		    res =                                                   \
			a00_2 + (((a01_2 +                                  \
			a10_2) >> 2) & CLEAR_TWO_BIT) + tmp;                \
		    dst_buf1[count] = res;                                  \
		    res =                                                   \
			a01_2 + (((a00_2 +                                  \
			a11_2) >> 2) & CLEAR_TWO_BIT) + tmp;                \
		    dst_buf1[count + 1] = res;                              \
		    a00_2 = a01_2;                                          \
		    a00_4 = a01_4;                                          \
		    a10_2 = a11_2;                                          \
		    a10_4 = a11_4;                                          \
		    count += 2;                                             \
		}                                                           \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_##nchannels   \
		    ((mlib_u8 *)dst_buf1, pdst + 2 * i * dst_stride,        \
		    count, colormap);                                       \
	    }                                                               \
	    if (src_width & 1) {                                            \
	                                                                    \
/*                                                                          \
 * right bound of image                                                     \
 */                                                                         \
		a00 = *(lut + psrc[j]);                                     \
		a01 = *(lut + psrc[j + 1]);                                 \
		a00_2 = (a00 >> 1) & CLEAR_ONE_BIT;                         \
		a00_4 = (a00 >> 2) & CLEAR_TWO_BIT;                         \
		a01_2 = (a01 >> 1) & CLEAR_ONE_BIT;                         \
		a01_4 = (a01 >> 2) & CLEAR_TWO_BIT;                         \
		count = 0;                                                  \
		for (i = 0; i < (src_height / 2); i++) {                    \
		    a10 = *(lut + psrc[(i + 1) * src_stride + j]);          \
		    a11 = *(lut + psrc[(i + 1) * src_stride + j + 1]);      \
		    a10_2 = (a10 >> 1) & CLEAR_ONE_BIT;                     \
		    a10_4 = (a10 >> 2) & CLEAR_TWO_BIT;                     \
		    a11_2 = (a11 >> 1) & CLEAR_ONE_BIT;                     \
		    a11_4 = (a11 >> 2) & CLEAR_TWO_BIT;                     \
		    tmp =                                                   \
			(((a00_4 + a10_4 + a01_4 +                          \
			a11_4) >> 2) & CLEAR_TWO_BIT) + ONE;                \
		    res =                                                   \
			a00_2 + (((a01_2 +                                  \
			a10_2) >> 2) & CLEAR_TWO_BIT) + tmp;                \
		    dst_buf1[count] = res;                                  \
		    res =                                                   \
			a10_2 + (((a00_2 +                                  \
			a11_2) >> 2) & CLEAR_TWO_BIT) + tmp;                \
		    dst_buf1[count + 1] = res;                              \
		    a00_2 = a10_2;                                          \
		    a00_4 = a10_4;                                          \
		    a01_2 = a11_2;                                          \
		    a01_4 = a11_4;                                          \
		    count += 2;                                             \
		}                                                           \
		if (src_height & 1) {                                       \
		    a10 = *(lut + psrc[(i + 1) * src_stride + j]);          \
		    a11 = *(lut + psrc[(i + 1) * src_stride + j + 1]);      \
		    a10_2 = (a10 >> 1) & CLEAR_ONE_BIT;                     \
		    a10_4 = (a10 >> 2) & CLEAR_TWO_BIT;                     \
		    a11_2 = (a11 >> 1) & CLEAR_ONE_BIT;                     \
		    a11_4 = (a11 >> 2) & CLEAR_TWO_BIT;                     \
		    tmp =                                                   \
			(((a00_4 + a10_4 + a01_4 +                          \
			a11_4) >> 2) & CLEAR_TWO_BIT) + ONE;                \
		    res =                                                   \
			a00_2 + (((a01_2 +                                  \
			a10_2) >> 2) & CLEAR_TWO_BIT) + tmp;                \
		    dst_buf1[count] = res;                                  \
		    count++;                                                \
		}                                                           \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_##nchannels   \
		    ((mlib_u8 *)dst_buf1, pdst_buf, count, colormap);       \
		for (k = 0; k < count; k++)                                 \
		    pdst[k * dst_stride + (src_width - 1)] =                \
			pdst_buf[k];                                        \
	    }                                                               \
	}

/* *********************************************************** */

#define	MLIB_ZOOMIN2XINDEX_U8_BL_FL1(dst_type, nchannels)                   \
	{                                                                   \
	    mlib_s32 i, j = 0, k, count, tmp1, tmp2, res;                   \
	    mlib_s32 a00, a01, a10, a11;                                    \
	    mlib_s32 a00_2, a01_2, a10_2, a11_2;                            \
	    mlib_s32 a00_4, a01_4, a10_4, a11_4;                            \
	                                                                    \
	    if (src_h_beg & 1) {                                            \
	                                                                    \
/*                                                                          \
 * upper bound of image                                                     \
 */                                                                         \
		a00 = *(lut + psrc[0]);                                     \
		a10 = *(lut + psrc[src_stride]);                            \
		a01 = *(lut + psrc[1]);                                     \
		a11 = *(lut + psrc[src_stride + 1]);                        \
		count = 0;                                                  \
		a00_4 = (a00 >> 2) & CLEAR_TWO_BIT;                         \
		a01_4 = (a01 >> 2) & CLEAR_TWO_BIT;                         \
		a10_4 = (a10 >> 2) & CLEAR_TWO_BIT;                         \
		a11_4 = (a11 >> 2) & CLEAR_TWO_BIT;                         \
		j = src_w_beg & 1;                                          \
		if (j) {                                                    \
		    tmp2 =                                                  \
			(((a00_4 + a10_4 + a01_4 +                          \
			a11_4) >> 1) & CLEAR_ONE_BIT) + ONE;                \
		    res = a01_4 + a11_4 + tmp2;                             \
		    dst_buf1[0] = res;                                      \
		    a00_4 = a01_4;                                          \
		    a10_4 = a11_4;                                          \
		    count++;                                                \
		}                                                           \
		for (j++; j < src_width; j += 2) {                          \
		    a01 = *(lut + psrc[(j >> 1) + 1]);                      \
		    a11 = *(lut + psrc[src_stride + (j >> 1) + 1]);         \
		    a01_4 = (a01 >> 2) & CLEAR_TWO_BIT;                     \
		    a11_4 = (a11 >> 2) & CLEAR_TWO_BIT;                     \
		    tmp2 =                                                  \
			(((a00_4 + a10_4 + a01_4 +                          \
			a11_4) >> 1) & CLEAR_ONE_BIT) + ONE;                \
		    res = a00_4 + a10_4 + tmp2;                             \
		    dst_buf1[count] = res;                                  \
		    res = a01_4 + a11_4 + tmp2;                             \
		    dst_buf1[count + 1] = res;                              \
		    a00_4 = a01_4;                                          \
		    a10_4 = a11_4;                                          \
		    count += 2;                                             \
		}                                                           \
		if (j == src_width) {                                       \
		    a01 = *(lut + psrc[(j >> 1) + 1]);                      \
		    a11 = *(lut + psrc[src_stride + (j >> 1) + 1]);         \
		    a01_4 = (a01 >> 2) & CLEAR_TWO_BIT;                     \
		    a11_4 = (a11 >> 2) & CLEAR_TWO_BIT;                     \
		    tmp2 =                                                  \
			(((a00_4 + a10_4 + a01_4 +                          \
			a11_4) >> 1) & CLEAR_ONE_BIT) + ONE;                \
		    res = a00_4 + a10_4 + tmp2;                             \
		    dst_buf1[count] = res;                                  \
		    count++;                                                \
		}                                                           \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_##nchannels   \
		    ((mlib_u8 *)dst_buf1, pdst, count, colormap);           \
		psrc += src_stride;                                         \
		pdst += dst_stride;                                         \
		src_height--;                                               \
	    }                                                               \
	    if (src_w_beg & 1) {                                            \
	                                                                    \
/*                                                                          \
 * left bound of image                                                      \
 */                                                                         \
		a00 = *(lut + psrc[0]);                                     \
		a01 = *(lut + psrc[1]);                                     \
		a00_2 = (a00 >> 1) & CLEAR_ONE_BIT;                         \
		a00_4 = (a00 >> 2) & CLEAR_TWO_BIT;                         \
		a01_2 = (a01 >> 1) & CLEAR_ONE_BIT;                         \
		a01_4 = (a01 >> 2) & CLEAR_TWO_BIT;                         \
		count = 0;                                                  \
		for (i = 0; i < (src_height / 2); i++) {                    \
		    a10 = *(lut + psrc[(i + 1) * src_stride]);              \
		    a11 = *(lut + psrc[(i + 1) * src_stride + 1]);          \
		    a10_2 = (a10 >> 1) & CLEAR_ONE_BIT;                     \
		    a10_4 = (a10 >> 2) & CLEAR_TWO_BIT;                     \
		    a11_2 = (a11 >> 1) & CLEAR_ONE_BIT;                     \
		    a11_4 = (a11 >> 2) & CLEAR_TWO_BIT;                     \
		    tmp1 = (((a00_2 + a01_2) >> 1) & CLEAR_ONE_BIT);        \
		    tmp2 =                                                  \
			(((a00_4 + a10_4 + a01_4 +                          \
			a11_4) >> 1) & CLEAR_ONE_BIT) + ONE;                \
		    res = a01_2 + tmp1;                                     \
		    dst_buf1[count] = res;                                  \
		    res = a01_4 + a11_4 + tmp2;                             \
		    dst_buf1[count + 1] = res;                              \
		    a00_2 = a10_2;                                          \
		    a00_4 = a10_4;                                          \
		    a01_2 = a11_2;                                          \
		    a01_4 = a11_4;                                          \
		    count += 2;                                             \
		}                                                           \
		if (src_height & 1) {                                       \
		    tmp1 = (((a00_2 + a01_2) >> 1) & CLEAR_ONE_BIT);        \
		    res = a01_2 + tmp1;                                     \
		    dst_buf1[count] = res;                                  \
		    count++;                                                \
		}                                                           \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_##nchannels   \
		    ((mlib_u8 *)dst_buf1, pdst_buf, count, colormap);       \
		for (k = 0; k < count; k++)                                 \
		    pdst[k * dst_stride] = pdst_buf[k];                     \
		psrc += 1;                                                  \
		pdst += 1;                                                  \
		src_width--;                                                \
	    }                                                               \
	    for (i = 0; i < (src_height / 2); i++) {                        \
		a00 = *(lut + psrc[i * src_stride]);                        \
		a10 = *(lut + psrc[(i + 1) * src_stride]);                  \
		a00_2 = (a00 >> 1) & CLEAR_ONE_BIT;                         \
		a00_4 = (a00 >> 2) & CLEAR_TWO_BIT;                         \
		a10_4 = (a10 >> 2) & CLEAR_TWO_BIT;                         \
		count = 0;                                                  \
		for (j = 0; j < (src_width / 2); j++) {                     \
		    a01 = *(lut + psrc[i * src_stride + j + 1]);            \
		    a11 = *(lut + psrc[(i + 1) * src_stride + j + 1]);      \
		    a01_2 = (a01 >> 1) & CLEAR_ONE_BIT;                     \
		    a01_4 = (a01 >> 2) & CLEAR_TWO_BIT;                     \
		    a11_4 = (a11 >> 2) & CLEAR_TWO_BIT;                     \
		    tmp1 = (((a00_2 + a01_2) >> 1) & CLEAR_ONE_BIT);        \
		    tmp2 =                                                  \
			(((a00_4 + a10_4 + a01_4 +                          \
			a11_4) >> 1) & CLEAR_ONE_BIT) + ONE;                \
		    res = a00_2 + tmp1;                                     \
		    dst_buf1[count] = res;                                  \
		    res = a01_2 + tmp1;                                     \
		    dst_buf1[count + 1] = res;                              \
		    res = a00_4 + a10_4 + tmp2;                             \
		    dst_buf2[count] = res;                                  \
		    res = a01_4 + a11_4 + tmp2;                             \
		    dst_buf2[count + 1] = res;                              \
		    a00_2 = a01_2;                                          \
		    a00_4 = a01_4;                                          \
		    a10_4 = a11_4;                                          \
		    count += 2;                                             \
		}                                                           \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_##nchannels   \
		    ((mlib_u8 *)dst_buf1, pdst + 2 * i * dst_stride,        \
		    count, colormap);                                       \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_##nchannels   \
		    ((mlib_u8 *)dst_buf2,                                   \
		    pdst + (2 * i + 1) * dst_stride, count, colormap);      \
	    }                                                               \
	    if (src_height & 1) {                                           \
	                                                                    \
/*                                                                          \
 * lower bound of image                                                     \
 */                                                                         \
		a00 = *(lut + psrc[i * src_stride]);                        \
		a00_2 = (a00 >> 1) & CLEAR_ONE_BIT;                         \
		count = 0;                                                  \
		for (j = 0; j < (src_width / 2); j++) {                     \
		    a01 = *(lut + psrc[i * src_stride + j + 1]);            \
		    a01_2 = (a01 >> 1) & CLEAR_ONE_BIT;                     \
		    tmp1 = (((a00_2 + a01_2) >> 1) & CLEAR_ONE_BIT);        \
		    res = a00_2 + tmp1;                                     \
		    dst_buf1[count] = res;                                  \
		    res = a01_2 + tmp1;                                     \
		    dst_buf1[count + 1] = res;                              \
		    a00_2 = a01_2;                                          \
		    a00_4 = a01_4;                                          \
		    count += 2;                                             \
		}                                                           \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_##nchannels   \
		    ((mlib_u8 *)dst_buf1, pdst + 2 * i * dst_stride,        \
		    count, colormap);                                       \
	    }                                                               \
	    if (src_width & 1) {                                            \
	                                                                    \
/*                                                                          \
 * right bound of image                                                     \
 */                                                                         \
		a00 = *(lut + psrc[j]);                                     \
		a01 = *(lut + psrc[j + 1]);                                 \
		a00_2 = (a00 >> 1) & CLEAR_ONE_BIT;                         \
		a00_4 = (a00 >> 2) & CLEAR_TWO_BIT;                         \
		a01_2 = (a01 >> 1) & CLEAR_ONE_BIT;                         \
		a01_4 = (a01 >> 2) & CLEAR_TWO_BIT;                         \
		count = 0;                                                  \
		for (i = 0; i < (src_height / 2); i++) {                    \
		    a10 = *(lut + psrc[(i + 1) * src_stride + j]);          \
		    a11 = *(lut + psrc[(i + 1) * src_stride + j + 1]);      \
		    a10_2 = (a10 >> 1) & CLEAR_ONE_BIT;                     \
		    a10_4 = (a10 >> 2) & CLEAR_TWO_BIT;                     \
		    a11_2 = (a11 >> 1) & CLEAR_ONE_BIT;                     \
		    a11_4 = (a11 >> 2) & CLEAR_TWO_BIT;                     \
		    tmp1 = (((a00_2 + a01_2) >> 1) & CLEAR_ONE_BIT);        \
		    tmp2 =                                                  \
			(((a00_4 + a10_4 + a01_4 +                          \
			a11_4) >> 1) & CLEAR_ONE_BIT) + ONE;                \
		    res = a00_2 + tmp1;                                     \
		    dst_buf1[count] = res;                                  \
		    res = a00_4 + a10_4 + tmp2;                             \
		    dst_buf1[count + 1] = res;                              \
		    a00_2 = a10_2;                                          \
		    a00_4 = a10_4;                                          \
		    a01_2 = a11_2;                                          \
		    a01_4 = a11_4;                                          \
		    count += 2;                                             \
		}                                                           \
		if (src_height & 1) {                                       \
		    tmp1 = (((a00_2 + a01_2) >> 1) & CLEAR_ONE_BIT);        \
		    res = a00_2 + tmp1;                                     \
		    dst_buf1[count] = res;                                  \
		    count++;                                                \
		}                                                           \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_##nchannels   \
		    ((mlib_u8 *)dst_buf1, pdst_buf, count, colormap);       \
		for (k = 0; k < count; k++)                                 \
		    pdst[k * dst_stride + (src_width - 1)] =                \
			pdst_buf[k];                                        \
	    }                                                               \
	}

/* *********************************************************** */

#define	MLIB_ZOOMIN2XINDEX_U8_BL_FL2(dst_type, nchannels)                   \
	{                                                                   \
	    mlib_s32 i, j = 0, k, count, tmp1, tmp2, res;                   \
	    mlib_s32 a00, a01, a10, a11;                                    \
	    mlib_s32 a00_2, a01_2, a10_2, a11_2;                            \
	    mlib_s32 a00_4, a01_4, a10_4, a11_4;                            \
	                                                                    \
	    if (src_h_beg & 1) {                                            \
	                                                                    \
/*                                                                          \
 * upper bound of image                                                     \
 */                                                                         \
		a00 = *(lut + psrc[0]);                                     \
		a10 = *(lut + psrc[src_stride]);                            \
		a01 = *(lut + psrc[1]);                                     \
		a11 = *(lut + psrc[src_stride + 1]);                        \
		count = 0;                                                  \
		a00_2 = (a00 >> 1) & CLEAR_ONE_BIT;                         \
		a00_4 = (a00 >> 2) & CLEAR_TWO_BIT;                         \
		a01_2 = (a01 >> 1) & CLEAR_ONE_BIT;                         \
		a01_4 = (a01 >> 2) & CLEAR_TWO_BIT;                         \
		a10_2 = (a10 >> 1) & CLEAR_ONE_BIT;                         \
		a10_4 = (a10 >> 2) & CLEAR_TWO_BIT;                         \
		a11_2 = (a11 >> 1) & CLEAR_ONE_BIT;                         \
		a11_4 = (a11 >> 2) & CLEAR_TWO_BIT;                         \
		j = src_w_beg & 1;                                          \
		if (j) {                                                    \
		    tmp2 =                                                  \
			(((a00_4 + a10_4 + a01_4 +                          \
			a11_4) >> 1) & CLEAR_ONE_BIT) + ONE;                \
		    res = a10_4 + a11_4 + tmp2;                             \
		    dst_buf1[0] = res;                                      \
		    a00_2 = a01_2;                                          \
		    a00_4 = a01_4;                                          \
		    a10_2 = a11_2;                                          \
		    a10_4 = a11_4;                                          \
		    count++;                                                \
		}                                                           \
		for (j++; j < src_width; j += 2) {                          \
		    a01 = *(lut + psrc[(j >> 1) + 1]);                      \
		    a11 = *(lut + psrc[src_stride + (j >> 1) + 1]);         \
		    a01_2 = (a01 >> 1) & CLEAR_ONE_BIT;                     \
		    a01_4 = (a01 >> 2) & CLEAR_TWO_BIT;                     \
		    a11_2 = (a11 >> 1) & CLEAR_ONE_BIT;                     \
		    a11_4 = (a11 >> 2) & CLEAR_TWO_BIT;                     \
		    tmp1 = (((a00_2 + a10_2) >> 1) & CLEAR_ONE_BIT);        \
		    tmp2 =                                                  \
			(((a00_4 + a10_4 + a01_4 +                          \
			a11_4) >> 1) & CLEAR_ONE_BIT) + ONE;                \
		    res = a10_2 + tmp1;                                     \
		    dst_buf1[count] = res;                                  \
		    res = a10_4 + a11_4 + tmp2;                             \
		    dst_buf1[count + 1] = res;                              \
		    a00_2 = a01_2;                                          \
		    a00_4 = a01_4;                                          \
		    a10_2 = a11_2;                                          \
		    a10_4 = a11_4;                                          \
		    count += 2;                                             \
		}                                                           \
		if (j == src_width) {                                       \
		    tmp1 = (((a00_2 + a10_2) >> 1) & CLEAR_ONE_BIT);        \
		    res = a10_2 + tmp1;                                     \
		    dst_buf1[count] = res;                                  \
		    count++;                                                \
		}                                                           \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_##nchannels   \
		    ((mlib_u8 *)dst_buf1, pdst, count, colormap);           \
		psrc += src_stride;                                         \
		pdst += dst_stride;                                         \
		src_height--;                                               \
	    }                                                               \
	    if (src_w_beg & 1) {                                            \
	                                                                    \
/*                                                                          \
 * left bound of image                                                      \
 */                                                                         \
		a00 = *(lut + psrc[0]);                                     \
		a01 = *(lut + psrc[1]);                                     \
		a00_2 = (a00 >> 1) & CLEAR_ONE_BIT;                         \
		a00_4 = (a00 >> 2) & CLEAR_TWO_BIT;                         \
		a01_2 = (a01 >> 1) & CLEAR_ONE_BIT;                         \
		a01_4 = (a01 >> 2) & CLEAR_TWO_BIT;                         \
		count = 0;                                                  \
		for (i = 0; i < (src_height / 2); i++) {                    \
		    a10 = *(lut + psrc[(i + 1) * src_stride]);              \
		    a11 = *(lut + psrc[(i + 1) * src_stride + 1]);          \
		    a10_4 = (a10 >> 2) & CLEAR_TWO_BIT;                     \
		    a11_4 = (a11 >> 2) & CLEAR_TWO_BIT;                     \
		    tmp2 =                                                  \
			(((a00_4 + a10_4 + a01_4 +                          \
			a11_4) >> 1) & CLEAR_ONE_BIT) + ONE;                \
		    res = a00_4 + a01_4 + tmp2;                             \
		    dst_buf1[count] = res;                                  \
		    res = a10_4 + a11_4 + tmp2;                             \
		    dst_buf1[count + 1] = res;                              \
		    a00_4 = a10_4;                                          \
		    a01_4 = a11_4;                                          \
		    count += 2;                                             \
		}                                                           \
		if (src_height & 1) {                                       \
		    a10 = *(lut + psrc[(i + 1) * src_stride]);              \
		    a11 = *(lut + psrc[(i + 1) * src_stride + 1]);          \
		    a10_4 = (a10 >> 2) & CLEAR_TWO_BIT;                     \
		    a11_4 = (a11 >> 2) & CLEAR_TWO_BIT;                     \
		    tmp2 =                                                  \
			(((a00_4 + a10_4 + a01_4 +                          \
			a11_4) >> 1) & CLEAR_ONE_BIT) + ONE;                \
		    res = a00_4 + a01_4 + tmp2;                             \
		    dst_buf1[count] = res;                                  \
		    count++;                                                \
		}                                                           \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_##nchannels   \
		    ((mlib_u8 *)dst_buf1, pdst_buf, count, colormap);       \
		for (k = 0; k < count; k++)                                 \
		    pdst[k * dst_stride] = pdst_buf[k];                     \
		psrc += 1;                                                  \
		pdst += 1;                                                  \
		src_width--;                                                \
	    }                                                               \
	    for (i = 0; i < (src_height / 2); i++) {                        \
		a00 = *(lut + psrc[i * src_stride]);                        \
		a10 = *(lut + psrc[(i + 1) * src_stride]);                  \
		a00_2 = (a00 >> 1) & CLEAR_ONE_BIT;                         \
		a00_4 = (a00 >> 2) & CLEAR_TWO_BIT;                         \
		a10_2 = (a10 >> 1) & CLEAR_ONE_BIT;                         \
		a10_4 = (a10 >> 2) & CLEAR_TWO_BIT;                         \
		count = 0;                                                  \
		for (j = 0; j < (src_width / 2); j++) {                     \
		    a01 = *(lut + psrc[i * src_stride + j + 1]);            \
		    a11 = *(lut + psrc[(i + 1) * src_stride + j + 1]);      \
		    a01_2 = (a01 >> 1) & CLEAR_ONE_BIT;                     \
		    a01_4 = (a01 >> 2) & CLEAR_TWO_BIT;                     \
		    a11_2 = (a11 >> 1) & CLEAR_ONE_BIT;                     \
		    a11_4 = (a11 >> 2) & CLEAR_TWO_BIT;                     \
		    tmp1 = (((a00_2 + a10_2) >> 1) & CLEAR_ONE_BIT);        \
		    tmp2 =                                                  \
			(((a00_4 + a10_4 + a01_4 +                          \
			a11_4) >> 1) & CLEAR_ONE_BIT) + ONE;                \
		    res = a00_2 + tmp1;                                     \
		    dst_buf1[count] = res;                                  \
		    res = a00_4 + a01_4 + tmp2;                             \
		    dst_buf1[count + 1] = res;                              \
		    res = a10_2 + tmp1;                                     \
		    dst_buf2[count] = res;                                  \
		    res = a10_4 + a11_4 + tmp2;                             \
		    dst_buf2[count + 1] = res;                              \
		    a00_2 = a01_2;                                          \
		    a00_4 = a01_4;                                          \
		    a10_2 = a11_2;                                          \
		    a10_4 = a11_4;                                          \
		    count += 2;                                             \
		}                                                           \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_##nchannels   \
		    ((mlib_u8 *)dst_buf1, pdst + 2 * i * dst_stride,        \
		    count, colormap);                                       \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_##nchannels   \
		    ((mlib_u8 *)dst_buf2,                                   \
		    pdst + (2 * i + 1) * dst_stride, count, colormap);      \
	    }                                                               \
	    if (src_height & 1) {                                           \
	                                                                    \
/*                                                                          \
 * lower bound of image                                                     \
 */                                                                         \
		a00 = *(lut + psrc[i * src_stride]);                        \
		a10 = *(lut + psrc[(i + 1) * src_stride]);                  \
		a00_2 = (a00 >> 1) & CLEAR_ONE_BIT;                         \
		a00_4 = (a00 >> 2) & CLEAR_TWO_BIT;                         \
		a10_2 = (a10 >> 1) & CLEAR_ONE_BIT;                         \
		a10_4 = (a10 >> 2) & CLEAR_TWO_BIT;                         \
		count = 0;                                                  \
		for (j = 0; j < (src_width / 2); j++) {                     \
		    a01 = *(lut + psrc[i * src_stride + j + 1]);            \
		    a11 = *(lut + psrc[(i + 1) * src_stride + j + 1]);      \
		    a01_2 = (a01 >> 1) & CLEAR_ONE_BIT;                     \
		    a01_4 = (a01 >> 2) & CLEAR_TWO_BIT;                     \
		    a11_2 = (a11 >> 1) & CLEAR_ONE_BIT;                     \
		    a11_4 = (a11 >> 2) & CLEAR_TWO_BIT;                     \
		    tmp1 = (((a00_2 + a10_2) >> 1) & CLEAR_ONE_BIT);        \
		    tmp2 =                                                  \
			(((a00_4 + a10_4 + a01_4 +                          \
			a11_4) >> 1) & CLEAR_ONE_BIT) + ONE;                \
		    res = a00_2 + tmp1;                                     \
		    dst_buf1[count] = res;                                  \
		    res = a00_4 + a01_4 + tmp2;                             \
		    dst_buf1[count + 1] = res;                              \
		    a00_2 = a01_2;                                          \
		    a00_4 = a01_4;                                          \
		    a10_2 = a11_2;                                          \
		    a10_4 = a11_4;                                          \
		    count += 2;                                             \
		}                                                           \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_##nchannels   \
		    ((mlib_u8 *)dst_buf1, pdst + 2 * i * dst_stride,        \
		    count, colormap);                                       \
	    }                                                               \
	    if (src_width & 1) {                                            \
	                                                                    \
/*                                                                          \
 * right bound of image                                                     \
 */                                                                         \
		a00 = *(lut + psrc[j]);                                     \
		a00_2 = (a00 >> 1) & CLEAR_ONE_BIT;                         \
		count = 0;                                                  \
		for (i = 0; i < (src_height / 2); i++) {                    \
		    a10 = *(lut + psrc[(i + 1) * src_stride + j]);          \
		    a10_2 = (a10 >> 1) & CLEAR_ONE_BIT;                     \
		    tmp1 = (((a00_2 + a10_2) >> 1) & CLEAR_ONE_BIT);        \
		    res = a00_2 + tmp1;                                     \
		    dst_buf1[count] = res;                                  \
		    res = a10_2 + tmp1;                                     \
		    dst_buf1[count + 1] = res;                              \
		    a00_2 = a10_2;                                          \
		    count += 2;                                             \
		}                                                           \
		if (src_height & 1) {                                       \
		    a10 = *(lut + psrc[(i + 1) * src_stride + j]);          \
		    a10_2 = (a10 >> 1) & CLEAR_ONE_BIT;                     \
		    tmp1 = (((a00_2 + a10_2) >> 1) & CLEAR_ONE_BIT);        \
		    res = a00_2 + tmp1;                                     \
		    dst_buf1[count] = res;                                  \
		    count++;                                                \
		}                                                           \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_##nchannels   \
		    ((mlib_u8 *)dst_buf1, pdst_buf, count, colormap);       \
		for (k = 0; k < count; k++)                                 \
		    pdst[k * dst_stride + (src_width - 1)] =                \
			pdst_buf[k];                                        \
	    }                                                               \
	}

/* *********************************************************** */

#define	MLIB_ZOOMIN2XINDEX_U8_BL_FL3(dst_type, nchannels)                   \
	{                                                                   \
	    mlib_s32 i, j = 0, k, count, res;                               \
	    mlib_s32 a00, a01, a10, a11;                                    \
	    mlib_s32 a00_2, a01_2, a10_2, a11_2;                            \
	    mlib_s32 a00_4, a01_4, a10_4, a11_4;                            \
	                                                                    \
	    if (src_h_beg & 1) {                                            \
	                                                                    \
/*                                                                          \
 * upper bound of image                                                     \
 */                                                                         \
		a00 = *(lut + psrc[0]);                                     \
		a10 = *(lut + psrc[src_stride]);                            \
		a01 = *(lut + psrc[1]);                                     \
		a11 = *(lut + psrc[src_stride + 1]);                        \
		count = 0;                                                  \
		a00_2 = (a00 >> 1) & CLEAR_ONE_BIT;                         \
		a00_4 = (a00 >> 2) & CLEAR_TWO_BIT;                         \
		a01_2 = (a01 >> 1) & CLEAR_ONE_BIT;                         \
		a01_4 = (a01 >> 2) & CLEAR_TWO_BIT;                         \
		a10_2 = (a10 >> 1) & CLEAR_ONE_BIT;                         \
		a10_4 = (a10 >> 2) & CLEAR_TWO_BIT;                         \
		a11_2 = (a11 >> 1) & CLEAR_ONE_BIT;                         \
		a11_4 = (a11 >> 2) & CLEAR_TWO_BIT;                         \
		j = src_w_beg & 1;                                          \
		if (j) {                                                    \
		    res = a00_4 + a10_4 + a01_4 + a11_4 + ONE;              \
		    dst_buf1[0] = res;                                      \
		    a00_2 = a01_2;                                          \
		    a00_4 = a01_4;                                          \
		    a10_2 = a11_2;                                          \
		    a10_4 = a11_4;                                          \
		    count++;                                                \
		}                                                           \
		for (j++; j < src_width; j += 2) {                          \
		    a01 = *(lut + psrc[(j >> 1) + 1]);                      \
		    a11 = *(lut + psrc[src_stride + (j >> 1) + 1]);         \
		    a01_2 = (a01 >> 1) & CLEAR_ONE_BIT;                     \
		    a01_4 = (a01 >> 2) & CLEAR_TWO_BIT;                     \
		    a11_2 = (a11 >> 1) & CLEAR_ONE_BIT;                     \
		    a11_4 = (a11 >> 2) & CLEAR_TWO_BIT;                     \
		    res = a00_2 + a10_2;                                    \
		    dst_buf1[count] = res;                                  \
		    res = a00_4 + a10_4 + a01_4 + a11_4 + ONE;              \
		    dst_buf1[count + 1] = res;                              \
		    a00_2 = a01_2;                                          \
		    a00_4 = a01_4;                                          \
		    a10_2 = a11_2;                                          \
		    a10_4 = a11_4;                                          \
		    count += 2;                                             \
		}                                                           \
		if (j == src_width) {                                       \
		    res = a00_2 + a10_2;                                    \
		    dst_buf1[count] = res;                                  \
		    count++;                                                \
		}                                                           \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_##nchannels   \
		    ((mlib_u8 *)dst_buf1, pdst, count, colormap);           \
		psrc += src_stride;                                         \
		pdst += dst_stride;                                         \
		src_height--;                                               \
	    }                                                               \
	    if (src_w_beg & 1) {                                            \
	                                                                    \
/*                                                                          \
 * left bound of image                                                      \
 */                                                                         \
		a00 = *(lut + psrc[0]);                                     \
		a01 = *(lut + psrc[1]);                                     \
		a00_2 = (a00 >> 1) & CLEAR_ONE_BIT;                         \
		a00_4 = (a00 >> 2) & CLEAR_TWO_BIT;                         \
		a01_2 = (a01 >> 1) & CLEAR_ONE_BIT;                         \
		a01_4 = (a01 >> 2) & CLEAR_TWO_BIT;                         \
		count = 0;                                                  \
		for (i = 0; i < (src_height / 2); i++) {                    \
		    a10 = *(lut + psrc[(i + 1) * src_stride]);              \
		    a11 = *(lut + psrc[(i + 1) * src_stride + 1]);          \
		    a10_2 = (a10 >> 1) & CLEAR_ONE_BIT;                     \
		    a10_4 = (a10 >> 2) & CLEAR_TWO_BIT;                     \
		    a11_2 = (a11 >> 1) & CLEAR_ONE_BIT;                     \
		    a11_4 = (a11 >> 2) & CLEAR_TWO_BIT;                     \
		    res = a00_2 + a01_2;                                    \
		    dst_buf1[count] = res;                                  \
		    res = a00_4 + a10_4 + a01_4 + a11_4 + ONE;              \
		    dst_buf1[count + 1] = res;                              \
		    a00_2 = a10_2;                                          \
		    a00_4 = a10_4;                                          \
		    a01_2 = a11_2;                                          \
		    a01_4 = a11_4;                                          \
		    count += 2;                                             \
		}                                                           \
		if (src_height & 1) {                                       \
		    res = a00_2 + a01_2;                                    \
		    dst_buf1[count] = res;                                  \
		    count++;                                                \
		}                                                           \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_##nchannels   \
		    ((mlib_u8 *)dst_buf1, pdst_buf, count, colormap);       \
		for (k = 0; k < count; k++)                                 \
		    pdst[k * dst_stride] = pdst_buf[k];                     \
		psrc += 1;                                                  \
		pdst += 1;                                                  \
		src_width--;                                                \
	    }                                                               \
	    for (i = 0; i < (src_height / 2); i++) {                        \
		a00 = *(lut + psrc[i * src_stride]);                        \
		a10 = *(lut + psrc[(i + 1) * src_stride]);                  \
		a00_2 = (a00 >> 1) & CLEAR_ONE_BIT;                         \
		a00_4 = (a00 >> 2) & CLEAR_TWO_BIT;                         \
		a10_2 = (a10 >> 1) & CLEAR_ONE_BIT;                         \
		a10_4 = (a10 >> 2) & CLEAR_TWO_BIT;                         \
		count = 0;                                                  \
		for (j = 0; j < (src_width / 2); j++) {                     \
		    a01 = *(lut + psrc[i * src_stride + j + 1]);            \
		    a11 = *(lut + psrc[(i + 1) * src_stride + j + 1]);      \
		    a01_2 = (a01 >> 1) & CLEAR_ONE_BIT;                     \
		    a01_4 = (a01 >> 2) & CLEAR_TWO_BIT;                     \
		    a11_2 = (a11 >> 1) & CLEAR_ONE_BIT;                     \
		    a11_4 = (a11 >> 2) & CLEAR_TWO_BIT;                     \
		    res = a00;                                              \
		    dst_buf1[count] = res;                                  \
		    res = a00_2 + a01_2;                                    \
		    dst_buf1[count + 1] = res;                              \
		    res = a00_2 + a10_2;                                    \
		    dst_buf2[count] = res;                                  \
		    res = a00_4 + a10_4 + a01_4 + a11_4 + ONE;              \
		    dst_buf2[count + 1] = res;                              \
		    a00 = a01;                                              \
		    a00_2 = a01_2;                                          \
		    a00_4 = a01_4;                                          \
		    a10_2 = a11_2;                                          \
		    a10_4 = a11_4;                                          \
		    count += 2;                                             \
		}                                                           \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_##nchannels   \
		    ((mlib_u8 *)dst_buf1, pdst + 2 * i * dst_stride,        \
		    count, colormap);                                       \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_##nchannels   \
		    ((mlib_u8 *)dst_buf2,                                   \
		    pdst + (2 * i + 1) * dst_stride, count, colormap);      \
	    }                                                               \
	    if (src_height & 1) {                                           \
	                                                                    \
/*                                                                          \
 * lower bound of image                                                     \
 */                                                                         \
		a00 = *(lut + psrc[i * src_stride]);                        \
		a00_2 = (a00 >> 1) & CLEAR_ONE_BIT;                         \
		count = 0;                                                  \
		for (j = 0; j < (src_width / 2); j++) {                     \
		    a01 = *(lut + psrc[i * src_stride + j + 1]);            \
		    a01_2 = (a01 >> 1) & CLEAR_ONE_BIT;                     \
		    res = a00;                                              \
		    dst_buf1[count] = res;                                  \
		    res = a00_2 + a01_2;                                    \
		    dst_buf1[count + 1] = res;                              \
		    a00 = a01;                                              \
		    a00_2 = a01_2;                                          \
		    count += 2;                                             \
		}                                                           \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_##nchannels   \
		    ((mlib_u8 *)dst_buf1, pdst + 2 * i * dst_stride,        \
		    count, colormap);                                       \
	    }                                                               \
	    if (src_width & 1) {                                            \
	                                                                    \
/*                                                                          \
 * right bound of image                                                     \
 */                                                                         \
		a00 = *(lut + psrc[j]);                                     \
		a00_2 = (a00 >> 1) & CLEAR_ONE_BIT;                         \
		count = 0;                                                  \
		for (i = 0; i < (src_height / 2); i++) {                    \
		    a10 = *(lut + psrc[(i + 1) * src_stride + j]);          \
		    a10_2 = (a10 >> 1) & CLEAR_ONE_BIT;                     \
		    res = a00;                                              \
		    dst_buf1[count] = res;                                  \
		    res = a00_2 + a10_2;                                    \
		    dst_buf1[count + 1] = res;                              \
		    a00 = a10;                                              \
		    a00_2 = a10_2;                                          \
		    count += 2;                                             \
		}                                                           \
		if (src_height & 1) {                                       \
		    res = a00;                                              \
		    dst_buf1[count] = res;                                  \
		    count++;                                                \
		}                                                           \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_##nchannels   \
		    ((mlib_u8 *)dst_buf1, pdst_buf, count, colormap);       \
		for (k = 0; k < count; k++)                                 \
		    pdst[k * dst_stride + (src_width - 1)] =                \
			pdst_buf[k];                                        \
	    }                                                               \
	}

/* *********************************************************** */

#define	MLIB_ZOOMIN2XINDEX_U8_BL(dst_type, nchannels, SHIFT)      \
	MLIB_ZOOMIN2XINDEX_U8_BL_FL##SHIFT(dst_type, nchannels)

/* *********************************************************** */

#define	MLIB_ZOOMIN2X_UL_FL0(b00, b01, b10, b11)	\
	(9*b00 + 3*b01 + 3*b10 + b11) >> 4
#define	MLIB_ZOOMIN2X_UR_FL0(b00, b01, b10, b11)	\
	(3*b00 + 9*b01 + b10 + 3*b11) >> 4

#define	MLIB_ZOOMIN2X_DL_FL0(b00, b01, b10, b11)	\
	(3*b00 + b01 + 9*b10 + 3*b11) >> 4

#define	MLIB_ZOOMIN2X_DR_FL0(b00, b01, b10, b11)	\
	(b00 + 3*b01 + 3*b10 + 9*b11) >> 4

#define	MLIB_ZOOMIN2X_UL_FL1(b00, b01, b10, b11)	\
	(3*b00 + b01) >> 2

#define	MLIB_ZOOMIN2X_UR_FL1(b00, b01, b10, b11)	\
	(b00 + 3*b01) >> 2

#define	MLIB_ZOOMIN2X_DL_FL1(b00, b01, b10, b11)	\
	(3*b00 + b01 + 3*b10 + b11) >> 3

#define	MLIB_ZOOMIN2X_DR_FL1(b00, b01, b10, b11)	\
	(b00 + 3*b01 + b10 + 3*b11) >> 3

#define	MLIB_ZOOMIN2X_UL_FL2(b00, b01, b10, b11)	\
	(3*b00 + b10) >> 2

#define	MLIB_ZOOMIN2X_UR_FL2(b00, b01, b10, b11)	\
	(3*b00 + 3*b01 + b10 + b11) >> 3

#define	MLIB_ZOOMIN2X_DL_FL2(b00, b01, b10, b11)	\
	(b00 + 3*b10) >> 2

#define	MLIB_ZOOMIN2X_DR_FL2(b00, b01, b10, b11)	\
	(b00 + b01 + 3*b10 + 3*b11) >> 3

#define	MLIB_ZOOMIN2X_UL_FL3(b00, b01, b10, b11)	\
	b00

#define	MLIB_ZOOMIN2X_UR_FL3(b00, b01, b10, b11)	\
	(b00 + b01) >> 1

#define	MLIB_ZOOMIN2X_DL_FL3(b00, b01, b10, b11)	\
	(b00 + b10) >> 1

#define	MLIB_ZOOMIN2X_DR_FL3(b00, b01, b10, b11)	\
	(b00 + b01 + b10 + b11) >> 2

/* *********************************************************** */

#define	COUNT_COLOR_STORE_INTO_INTERM_BUF_3(buf_number,         \
	index, MACRO)                                           \
	dst_buf##buf_number[3 * (index)] =                      \
	MACRO(a00_0, a01_0, a10_0, a11_0);                      \
	dst_buf##buf_number[3 * (index) + 1] =                  \
	MACRO(a00_1, a01_1, a10_1, a11_1);                      \
	dst_buf##buf_number[3 * (index) + 2] =                  \
	MACRO(a00_2, a01_2, a10_2, a11_2)

/* *********************************************************** */

#define	COUNT_COLOR_STORE_INTO_INTERM_BUF_4(buf_number,         \
	index, MACRO)                                           \
	dst_buf##buf_number[4 * (index)] =                      \
	MACRO(a00_0, a01_0, a10_0, a11_0);                      \
	dst_buf##buf_number[4 * (index) + 1] =                  \
	MACRO(a00_1, a01_1, a10_1, a11_1);                      \
	dst_buf##buf_number[4 * (index) + 2] =                  \
	MACRO(a00_2, a01_2, a10_2, a11_2);                      \
	dst_buf##buf_number[4 * (index) + 3] =                  \
	MACRO(a00_3, a01_3, a10_3, a11_3)

/* *********************************************************** */

#define	LEFT_SHIFT_COLORS_3()                                   \
	a00_0 = a01_0;                                          \
	a10_0 = a11_0;                                          \
	a00_1 = a01_1;                                          \
	a10_1 = a11_1;                                          \
	a00_2 = a01_2;                                          \
	a10_2 = a11_2

/* *********************************************************** */

#define	LEFT_SHIFT_COLORS_4()                                   \
	a00_0 = a01_0;                                          \
	a10_0 = a11_0;                                          \
	a00_1 = a01_1;                                          \
	a10_1 = a11_1;                                          \
	a00_2 = a01_2;                                          \
	a10_2 = a11_2;                                          \
	a00_3 = a01_3;                                          \
	a10_3 = a11_3

/* *********************************************************** */

#define	UP_SHIFT_COLORS_3()                                     \
	a00_0 = a10_0;                                          \
	a01_0 = a11_0;                                          \
	a00_1 = a10_1;                                          \
	a01_1 = a11_1;                                          \
	a00_2 = a10_2;                                          \
	a01_2 = a11_2

/* *********************************************************** */

#define	UP_SHIFT_COLORS_4()                                     \
	a00_0 = a10_0;                                          \
	a01_0 = a11_0;                                          \
	a00_1 = a10_1;                                          \
	a01_1 = a11_1;                                          \
	a00_2 = a10_2;                                          \
	a01_2 = a11_2;                                          \
	a00_3 = a10_3;                                          \
	a01_3 = a11_3

/* *********************************************************** */

#define	PREPARE_COLORS_3(number)                                \
	a##number##_0 = pcolor##number[1];                      \
	a##number##_1 = pcolor##number[2];                      \
	a##number##_2 = pcolor##number[3]

/* *********************************************************** */

#define	PREPARE_COLORS_4(number)                                \
	a##number##_0 = pcolor##number[0];                      \
	a##number##_1 = pcolor##number[1];                      \
	a##number##_2 = pcolor##number[2];                      \
	a##number##_3 = pcolor##number[3]

/* *********************************************************** */

#define	DEFINE_IDENT_BL_3()                                     \
	mlib_s32 a00_0, a01_0, a10_0, a11_0;                    \
	mlib_s32 a00_1, a01_1, a10_1, a11_1;                    \
	mlib_s32 a00_2, a01_2, a10_2, a11_2

/* *********************************************************** */

#define	DEFINE_IDENT_BL_4()                                     \
	mlib_s32 a00_0, a01_0, a10_0, a11_0;                    \
	mlib_s32 a00_1, a01_1, a10_1, a11_1;                    \
	mlib_s32 a00_2, a01_2, a10_2, a11_2;                    \
	mlib_s32 a00_3, a01_3, a10_3, a11_3

/* *********************************************************** */

#define	MLIB_ZOOMIN2XINDEX_S16_BL(dst_type, nchannels, SHIFT)                \
	{                                                                    \
	    mlib_s32 i, j = 0, k, count;                                     \
	                                                                     \
/*                                                                           \
 * indices                                                                   \
 */                                                                          \
	    mlib_s16 *pcolor00, *pcolor10, *pcolor01, *pcolor11;             \
	                                                                     \
	    DEFINE_IDENT_BL_##nchannels();                                   \
	    if (src_h_beg & 1) {                                             \
	                                                                     \
/*                                                                           \
 * upper bound of image                                                      \
 */                                                                          \
		pcolor00 = (mlib_s16 *)(lut + psrc[0]);                      \
		pcolor10 = (mlib_s16 *)(lut + psrc[src_stride]);             \
		pcolor01 = (mlib_s16 *)(lut + psrc[1]);                      \
		pcolor11 = (mlib_s16 *)(lut + psrc[src_stride + 1]);         \
		PREPARE_COLORS_##nchannels(00);                              \
		PREPARE_COLORS_##nchannels(01);                              \
		PREPARE_COLORS_##nchannels(10);                              \
		PREPARE_COLORS_##nchannels(11);                              \
		count = 0;                                                   \
		j = src_w_beg & 1;                                           \
		if (j) {                                                     \
		    COUNT_COLOR_STORE_INTO_INTERM_BUF_##nchannels(1,         \
			0, MLIB_ZOOMIN2X_DR_FL##SHIFT);                      \
		    LEFT_SHIFT_COLORS_##nchannels();                         \
		    count++;                                                 \
		}                                                            \
		for (j++; j < src_width; j += 2) {                           \
		    pcolor01 = (mlib_s16 *)(lut + psrc[(j >> 1) + 1]);       \
		    pcolor11 =                                               \
			(mlib_s16 *)(lut + psrc[src_stride + (j >> 1) +      \
			1]);                                                 \
		    PREPARE_COLORS_##nchannels(01);                          \
		    PREPARE_COLORS_##nchannels(11);                          \
		    COUNT_COLOR_STORE_INTO_INTERM_BUF_##nchannels(1,         \
			j - 1, MLIB_ZOOMIN2X_DL_FL##SHIFT);                  \
		    COUNT_COLOR_STORE_INTO_INTERM_BUF_##nchannels(1,         \
			j, MLIB_ZOOMIN2X_DR_FL##SHIFT);                      \
		    LEFT_SHIFT_COLORS_##nchannels();                         \
		    count += 2;                                              \
		}                                                            \
		if (j == src_width) {                                        \
		    pcolor01 = (mlib_s16 *)(lut + psrc[(j >> 1) + 1]);       \
		    pcolor11 =                                               \
			(mlib_s16 *)(lut + psrc[src_stride + (j >> 1) +      \
			1]);                                                 \
		    PREPARE_COLORS_##nchannels(01);                          \
		    PREPARE_COLORS_##nchannels(11);                          \
		    COUNT_COLOR_STORE_INTO_INTERM_BUF_##nchannels(1,         \
			j - 1, MLIB_ZOOMIN2X_DL_FL##SHIFT);                  \
		    count++;                                                 \
		}                                                            \
		mlib_ImageColorTrue2IndexLine_S16_##dst_type##_##nchannels   \
		    (dst_buf1, pdst, count, colormap);                       \
		psrc += src_stride;                                          \
		pdst += dst_stride;                                          \
		src_height--;                                                \
	    }                                                                \
	    if (src_w_beg & 1) {                                             \
	                                                                     \
/*                                                                           \
 * left bound of image                                                       \
 */                                                                          \
		pcolor00 = (mlib_s16 *)(lut + psrc[0]);                      \
		pcolor01 = (mlib_s16 *)(lut + psrc[1]);                      \
		PREPARE_COLORS_##nchannels(00);                              \
		PREPARE_COLORS_##nchannels(01);                              \
		count = 0;                                                   \
		for (i = 0; i < (src_height / 2); i++) {                     \
		    pcolor10 =                                               \
			(mlib_s16 *)(lut + psrc[(i + 1) * src_stride]);      \
		    pcolor11 =                                               \
			(mlib_s16 *)(lut + psrc[(i + 1) * src_stride +       \
			1]);                                                 \
		    PREPARE_COLORS_##nchannels(10);                          \
		    PREPARE_COLORS_##nchannels(11);                          \
		    COUNT_COLOR_STORE_INTO_INTERM_BUF_##nchannels(1,         \
			2 * i, MLIB_ZOOMIN2X_UR_FL##SHIFT);                  \
		    COUNT_COLOR_STORE_INTO_INTERM_BUF_##nchannels(1,         \
			2 * i + 1, MLIB_ZOOMIN2X_DR_FL##SHIFT);              \
		    UP_SHIFT_COLORS_##nchannels();                           \
		    count += 2;                                              \
		}                                                            \
		if (src_height & 1) {                                        \
		    pcolor10 =                                               \
			(mlib_s16 *)(lut + psrc[(i + 1) * src_stride]);      \
		    pcolor11 =                                               \
			(mlib_s16 *)(lut + psrc[(i + 1) * src_stride +       \
			1]);                                                 \
		    PREPARE_COLORS_##nchannels(10);                          \
		    PREPARE_COLORS_##nchannels(11);                          \
		    COUNT_COLOR_STORE_INTO_INTERM_BUF_##nchannels(1,         \
			2 * i, MLIB_ZOOMIN2X_UR_FL##SHIFT);                  \
		    count++;                                                 \
		}                                                            \
		mlib_ImageColorTrue2IndexLine_S16_##dst_type##_##nchannels   \
		    (dst_buf1, pdst_buf, count, colormap);                   \
		for (k = 0; k < count; k++)                                  \
		    pdst[k * dst_stride] = pdst_buf[k];                      \
		psrc += 1;                                                   \
		pdst += 1;                                                   \
		src_width--;                                                 \
	    }                                                                \
	    for (i = 0; i < (src_height / 2); i++) {                         \
		pcolor00 = (mlib_s16 *)(lut + psrc[i * src_stride]);         \
		pcolor10 =                                                   \
		    (mlib_s16 *)(lut + psrc[(i + 1) * src_stride]);          \
		PREPARE_COLORS_##nchannels(00);                              \
		PREPARE_COLORS_##nchannels(10);                              \
		count = 0;                                                   \
		for (j = 0; j < (src_width / 2); j++) {                      \
		    pcolor01 =                                               \
			(mlib_s16 *)(lut + psrc[i * src_stride + j +         \
			1]);                                                 \
		    pcolor11 =                                               \
			(mlib_s16 *)(lut + psrc[(i + 1) * src_stride +       \
			j + 1]);                                             \
		    PREPARE_COLORS_##nchannels(01);                          \
		    PREPARE_COLORS_##nchannels(11);                          \
		    COUNT_COLOR_STORE_INTO_INTERM_BUF_##nchannels(1,         \
			2 * j, MLIB_ZOOMIN2X_UL_FL##SHIFT);                  \
		    COUNT_COLOR_STORE_INTO_INTERM_BUF_##nchannels(1,         \
			2 * j + 1, MLIB_ZOOMIN2X_UR_FL##SHIFT);              \
		    COUNT_COLOR_STORE_INTO_INTERM_BUF_##nchannels(2,         \
			2 * j, MLIB_ZOOMIN2X_DL_FL##SHIFT);                  \
		    COUNT_COLOR_STORE_INTO_INTERM_BUF_##nchannels(2,         \
			2 * j + 1, MLIB_ZOOMIN2X_DR_FL##SHIFT);              \
		    LEFT_SHIFT_COLORS_##nchannels();                         \
		    count += 2;                                              \
		}                                                            \
		mlib_ImageColorTrue2IndexLine_S16_##dst_type##_##nchannels   \
		    (dst_buf1, pdst + 2 * i * dst_stride, count,             \
		    colormap);                                               \
		mlib_ImageColorTrue2IndexLine_S16_##dst_type##_##nchannels   \
		    (dst_buf2, pdst + (2 * i + 1) * dst_stride, count,       \
		    colormap);                                               \
	    }                                                                \
	    if (src_height & 1) {                                            \
	                                                                     \
/*                                                                           \
 * lower bound of image                                                      \
 */                                                                          \
		pcolor00 = (mlib_s16 *)(lut + psrc[i * src_stride]);         \
		pcolor10 =                                                   \
		    (mlib_s16 *)(lut + psrc[(i + 1) * src_stride]);          \
		PREPARE_COLORS_##nchannels(00);                              \
		PREPARE_COLORS_##nchannels(10);                              \
		count = 0;                                                   \
		for (j = 0; j < (src_width / 2); j++) {                      \
		    pcolor01 =                                               \
			(mlib_s16 *)(lut + psrc[i * src_stride + j +         \
			1]);                                                 \
		    pcolor11 =                                               \
			(mlib_s16 *)(lut + psrc[(i + 1) * src_stride +       \
			j + 1]);                                             \
		    PREPARE_COLORS_##nchannels(01);                          \
		    PREPARE_COLORS_##nchannels(11);                          \
		    COUNT_COLOR_STORE_INTO_INTERM_BUF_##nchannels(1,         \
			2 * j, MLIB_ZOOMIN2X_UL_FL##SHIFT);                  \
		    COUNT_COLOR_STORE_INTO_INTERM_BUF_##nchannels(1,         \
			2 * j + 1, MLIB_ZOOMIN2X_UR_FL##SHIFT);              \
		    LEFT_SHIFT_COLORS_##nchannels();                         \
		    count += 2;                                              \
		}                                                            \
		mlib_ImageColorTrue2IndexLine_S16_##dst_type##_##nchannels   \
		    (dst_buf1, pdst + 2 * i * dst_stride, count,             \
		    colormap);                                               \
	    }                                                                \
	    if (src_width & 1) {                                             \
	                                                                     \
/*                                                                           \
 * right bound of image                                                      \
 */                                                                          \
		pcolor00 = (mlib_s16 *)(lut + psrc[j]);                      \
		pcolor01 = (mlib_s16 *)(lut + psrc[j + 1]);                  \
		PREPARE_COLORS_##nchannels(00);                              \
		PREPARE_COLORS_##nchannels(01);                              \
		count = 0;                                                   \
		for (i = 0; i < (src_height / 2); i++) {                     \
		    pcolor10 =                                               \
			(mlib_s16 *)(lut + psrc[(i + 1) * src_stride +       \
			j]);                                                 \
		    pcolor11 =                                               \
			(mlib_s16 *)(lut + psrc[(i + 1) * src_stride +       \
			j + 1]);                                             \
		    PREPARE_COLORS_##nchannels(10);                          \
		    PREPARE_COLORS_##nchannels(11);                          \
		    COUNT_COLOR_STORE_INTO_INTERM_BUF_##nchannels(1,         \
			2 * i, MLIB_ZOOMIN2X_UL_FL##SHIFT);                  \
		    COUNT_COLOR_STORE_INTO_INTERM_BUF_##nchannels(1,         \
			2 * i + 1, MLIB_ZOOMIN2X_DL_FL##SHIFT);              \
		    UP_SHIFT_COLORS_##nchannels();                           \
		    count += 2;                                              \
		}                                                            \
		if (src_height & 1) {                                        \
		    pcolor10 =                                               \
			(mlib_s16 *)(lut + psrc[(i + 1) * src_stride +       \
			j]);                                                 \
		    pcolor11 =                                               \
			(mlib_s16 *)(lut + psrc[(i + 1) * src_stride +       \
			j + 1]);                                             \
		    PREPARE_COLORS_##nchannels(10);                          \
		    PREPARE_COLORS_##nchannels(11);                          \
		    COUNT_COLOR_STORE_INTO_INTERM_BUF_##nchannels(1,         \
			2 * i, MLIB_ZOOMIN2X_UL_FL##SHIFT);                  \
		    count++;                                                 \
		}                                                            \
		mlib_ImageColorTrue2IndexLine_S16_##dst_type##_##nchannels   \
		    (dst_buf1, pdst_buf, count, colormap);                   \
		for (k = 0; k < count; k++)                                  \
		    pdst[k * dst_stride + (src_width - 1)] =                 \
			pdst_buf[k];                                         \
	    }                                                                \
	}

/* *********************************************************** */

#define	ALLOC_BL_U8()                                             \
	mlib_s32 *dst_buf1, *dst_buf2;                            \
	mlib_s32 *lut =                                           \
	    (mlib_s32 *)mlib_ImageGetLutNormalTable(colormap) -   \
	mlib_ImageGetLutOffset(colormap);                         \
	dst_buf1 =                                                \
	    (mlib_s32 *)__mlib_malloc((size + src_width +         \
	    4) * sizeof (mlib_s32));                              \
	if (dst_buf1 == NULL)                                     \
	{                                                         \
	__mlib_free(pdst_buf);                                    \
	return (MLIB_FAILURE);                                    \
	}                                                         \
	dst_buf2 = dst_buf1 + (size + 2)

/* *********************************************************** */

#define	ALLOC_BL_S16()                                            \
	mlib_s16 *dst_buf1, *dst_buf2;                            \
	mlib_d64 *lut =                                           \
	    (mlib_d64 *)mlib_ImageGetLutNormalTable(colormap) -   \
	mlib_ImageGetLutOffset(colormap);                         \
	dst_buf1 =                                                \
	    (mlib_s16 *)__mlib_malloc((size + src_width +         \
	    4) * 2 * nchannels);                                  \
	if (dst_buf1 == NULL)                                     \
	{                                                         \
	__mlib_free(pdst_buf);                                    \
	return (MLIB_FAILURE);                                    \
	}                                                         \
	dst_buf2 = dst_buf1 + (size + 2) * nchannels

/* *********************************************************** */

#define	FREE() __mlib_free(pdst_buf); __mlib_free(dst_buf1)

/* *********************************************************** */

#define	MLIB_IMAGEZOOMIN2XINDEX_BL(dst_format, dst_type, SHIFT)           \
	mlib_s32 lut_type = mlib_ImageGetLutType(colormap);               \
	mlib_s32 nchannels = mlib_ImageGetLutChannels(colormap);          \
	mlib_s32 size =                                                   \
	    (src_height > src_width ? src_height : src_width);            \
	dst_format *pdst_buf;                                             \
	                                                                  \
	if ((nchannels != 3) && (nchannels != 4))                         \
	    return (MLIB_FAILURE);                                        \
	pdst_buf =                                                        \
	    (dst_format *) __mlib_malloc((src_height +                    \
	    2) * sizeof (dst_format) * nchannels);                        \
	if (pdst_buf == NULL)                                             \
	    return (MLIB_FAILURE);                                        \
	switch (lut_type) {                                               \
	case MLIB_BYTE: {                                                 \
		ALLOC_BL_U8();                                            \
		switch (nchannels) {                                      \
		case 3:                                                   \
		    MLIB_ZOOMIN2XINDEX_U8_BL(dst_type, 3_in_4, SHIFT);    \
		    break;                                                \
		case 4:                                                   \
		    MLIB_ZOOMIN2XINDEX_U8_BL(dst_type, 4, SHIFT);         \
		    break;                                                \
		}                                                         \
		FREE();                                                   \
		break;                                                    \
	    }                                                             \
	case MLIB_SHORT: {                                                \
		ALLOC_BL_S16();                                           \
		switch (nchannels) {                                      \
		case 3:                                                   \
		    MLIB_ZOOMIN2XINDEX_S16_BL(dst_type, 3, SHIFT);        \
		    break;                                                \
		case 4:                                                   \
		    MLIB_ZOOMIN2XINDEX_S16_BL(dst_type, 4, SHIFT);        \
		    break;                                                \
		}                                                         \
		FREE();                                                   \
		break;                                                    \
	    }                                                             \
	}                                                                 \
	                                                                  \
	return MLIB_SUCCESS

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomIn2XIndex_U8_BL_FL0(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	MLIB_IMAGEZOOMIN2XINDEX_BL(mlib_u8,
	    U8,
	    0);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomIn2XIndex_U8_BL_FL1(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	MLIB_IMAGEZOOMIN2XINDEX_BL(mlib_u8,
	    U8,
	    1);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomIn2XIndex_U8_BL_FL2(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	MLIB_IMAGEZOOMIN2XINDEX_BL(mlib_u8,
	    U8,
	    2);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomIn2XIndex_U8_BL_FL3(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	MLIB_IMAGEZOOMIN2XINDEX_BL(mlib_u8,
	    U8,
	    3);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomIn2XIndex_S16_BL_FL0(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	MLIB_IMAGEZOOMIN2XINDEX_BL(mlib_s16,
	    S16,
	    0);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomIn2XIndex_S16_BL_FL1(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	MLIB_IMAGEZOOMIN2XINDEX_BL(mlib_s16,
	    S16,
	    1);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomIn2XIndex_S16_BL_FL2(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	MLIB_IMAGEZOOMIN2XINDEX_BL(mlib_s16,
	    S16,
	    2);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomIn2XIndex_S16_BL_FL3(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	MLIB_IMAGEZOOMIN2XINDEX_BL(mlib_s16,
	    S16,
	    3);
}

/* *********************************************************** */
