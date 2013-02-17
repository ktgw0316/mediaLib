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

#pragma ident	"@(#)mlib_c_ImageZoomIn2XIndex_BC2.c	9.3	07/10/09 SMI"

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
 *              MLIB_BICUBIC2
 *
 *      The edge condition can be one of the following:
 *              MLIB_EDGE_DST_NO_WRITE  (default)
 *              MLIB_EDGE_DST_FILL_ZERO
 *              MLIB_EDGE_OP_NEAREST
 *              MLIB_EDGE_SRC_EXTEND
 *              MLIB_EDGE_SRC_PADDED
 */

/* *********************************************************** */

/*								*/
/*			filter = BICUBIC2			*/
/*			(Internal functions)			*/
/*								*/

#include <mlib_image.h>
#include <mlib_ImageColormap.h>
#include <mlib_ImageZoomIn2XIndex.h>

/* *********************************************************** */

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_U8_0_1(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_U8_0_2(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_U8_0_3(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_U8_0_4(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_S16_0_1(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_S16_0_2(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_S16_0_3(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_S16_0_4(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_U8_1_1(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_U8_1_2(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_U8_1_3(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_U8_1_4(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_S16_1_1(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_S16_1_2(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_S16_1_3(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_S16_1_4(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_U8_2_1(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_U8_2_2(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_U8_2_3(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_U8_2_4(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_S16_2_1(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_S16_2_2(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_S16_2_3(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_S16_2_4(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_U8_3_1(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_U8_3_2(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_U8_3_3(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_U8_3_4(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_S16_3_1(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_S16_3_2(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_S16_3_3(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

static mlib_status MLIB_ZOOMIN2XINDEX_BC2_S16_3_4(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

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

#define	ROW_BICUBIC_FL0(j, chan, src_format, src_type)                     \
	{                                                                  \
	    mlib_s32 dh = (src_h_beg + j) & 1,                             \
		hx = (j + (src_h_beg & 1)) >> 1,                           \
		dw = (src_w_beg) & 1,                                      \
		wx = (src_w_beg & 1) >> 1,                                 \
		hx_1 = hx - 1,                                             \
		hx1 = hx + 1,                                              \
		hx2 = hx + 2,                                              \
		wx_1 = wx - 1,                                             \
		wx1 = wx + 1,                                              \
		wx2 = wx + 2;                                              \
	    mlib_s32 a_1, a, a1, a2, l, rez, i;                            \
	    src_format *pcolor;                                            \
	                                                                   \
	    hx_1 *= src_stride;                                            \
	    hx *= src_stride;                                              \
	    hx1 *= src_stride;                                             \
	    hx2 *= src_stride;                                             \
	    if (dh == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    pcolor = table[l] - offset;                            \
		    a_1 =                                                  \
			(57 * pcolor[psrc[hx + wx_1]] +                    \
			19 * pcolor[psrc[hx1 + wx_1]] -                    \
			9 * pcolor[psrc[hx_1 + wx_1]] -                    \
			3 * pcolor[psrc[hx2 + wx_1]]);                     \
		    a = (57 * pcolor[psrc[hx + wx]] +                      \
			19 * pcolor[psrc[hx1 + wx]] -                      \
			9 * pcolor[psrc[hx_1 + wx]] -                      \
			3 * pcolor[psrc[hx2 + wx]]);                       \
		    a1 = (57 * pcolor[psrc[hx + wx1]] +                    \
			19 * pcolor[psrc[hx1 + wx1]] -                     \
			9 * pcolor[psrc[hx_1 + wx1]] -                     \
			3 * pcolor[psrc[hx2 + wx1]]);                      \
		    p_1 = psrc + hx_1 + wx2;                               \
		    p = psrc + hx + wx2;                                   \
		    p1 = psrc + hx1 + wx2;                                 \
		    p2 = psrc + hx2 + wx2;                                 \
		    i = 0;                                                 \
		    if (dw == 1) {                                         \
			a2 = 57 * pcolor[(*p)] + 19 * pcolor[(*p1)] -      \
			    9 * pcolor[(*p_1)] - 3 * pcolor[(*p2)];        \
			rez =                                              \
			    (19 * a + 57 * a1 - 3 * a_1 - 9 * a2) >> 12;   \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1++;                                             \
			p++;                                               \
			p1++;                                              \
			p2++;                                              \
			i++;                                               \
		    }                                                      \
		    for (; i < src_width - 1; i += 2) {                    \
			a2 = 57 * pcolor[(*p)] + 19 * pcolor[(*p1)] -      \
			    9 * pcolor[(*p_1)] - 3 * pcolor[(*p2)];        \
			rez =                                              \
			    (57 * a + 19 * a1 - 9 * a_1 - 3 * a2) >> 12;   \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			rez =                                              \
			    (19 * a + 57 * a1 - 3 * a_1 - 9 * a2) >> 12;   \
			SAT_##src_type();                                  \
			dst_buf1[chan * (i + 1) + l] = rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1++;                                             \
			p++;                                               \
			p1++;                                              \
			p2++;                                              \
		    }                                                      \
		    if (i < src_width) {                                   \
			a2 = 57 * pcolor[(*p)] + 19 * pcolor[(*p1)] -      \
			    9 * pcolor[(*p_1)] - 3 * pcolor[(*p2)];        \
			rez =                                              \
			    (57 * a + 19 * a1 - 9 * a_1 - 3 * a2) >> 12;   \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    pcolor = table[l] - offset;                            \
		    a_1 =                                                  \
			(19 * pcolor[psrc[hx + wx_1]] +                    \
			57 * pcolor[psrc[hx1 + wx_1]] -                    \
			3 * pcolor[psrc[hx_1 + wx_1]] -                    \
			9 * pcolor[psrc[hx2 + wx_1]]);                     \
		    a = (19 * pcolor[psrc[hx + wx]] +                      \
			57 * pcolor[psrc[hx1 + wx]] -                      \
			3 * pcolor[psrc[hx_1 + wx]] -                      \
			9 * pcolor[psrc[hx2 + wx]]);                       \
		    a1 = (19 * pcolor[psrc[hx + wx1]] +                    \
			57 * pcolor[psrc[hx1 + wx1]] -                     \
			3 * pcolor[psrc[hx_1 + wx1]] -                     \
			9 * pcolor[psrc[hx2 + wx1]]);                      \
		    p_1 = psrc + hx_1 + wx2;                               \
		    p = psrc + hx + wx2;                                   \
		    p1 = psrc + hx1 + wx2;                                 \
		    p2 = psrc + hx2 + wx2;                                 \
		    i = 0;                                                 \
		    if (dw == 1) {                                         \
			a2 = 19 * pcolor[(*p)] + 57 * pcolor[(*p1)] -      \
			    3 * pcolor[(*p_1)] - 9 * pcolor[(*p2)];        \
			rez =                                              \
			    (19 * a + 57 * a1 - 3 * a_1 - 9 * a2) >> 12;   \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1++;                                             \
			p++;                                               \
			p1++;                                              \
			p2++;                                              \
			i++;                                               \
		    }                                                      \
		    for (; i < src_width - 1; i += 2) {                    \
			a2 = 19 * pcolor[(*p)] + 57 * pcolor[(*p1)] -      \
			    3 * pcolor[(*p_1)] - 9 * pcolor[(*p2)];        \
			rez =                                              \
			    (57 * a + 19 * a1 - 9 * a_1 - 3 * a2) >> 12;   \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			rez =                                              \
			    (19 * a + 57 * a1 - 3 * a_1 - 9 * a2) >> 12;   \
			SAT_##src_type();                                  \
			dst_buf1[chan * (i + 1) + l] = rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1++;                                             \
			p++;                                               \
			p1++;                                              \
			p2++;                                              \
		    }                                                      \
		    if (i < src_width) {                                   \
			a2 = 19 * pcolor[(*p)] + 57 * pcolor[(*p1)] -      \
			    3 * pcolor[(*p_1)] - 9 * pcolor[(*p2)];        \
			rez =                                              \
			    (57 * a + 19 * a1 - 9 * a_1 - 3 * a2) >> 12;   \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC_FL0(j, chan, src_format, src_type)                  \
	{                                                                  \
	    mlib_s32 dh = (src_h_beg) & 1,                                 \
		hx = (src_h_beg & 1) >> 1,                                 \
		dw = (src_w_beg + j) & 1,                                  \
		wx = (j + (src_w_beg & 1)) >> 1,                           \
		hx_1 = hx - 1,                                             \
		hx1 = hx + 1,                                              \
		hx2 = hx + 2,                                              \
		wx_1 = wx - 1,                                             \
		wx1 = wx + 1,                                              \
		wx2 = wx + 2;                                              \
	    mlib_s32 a_1, a, a1, a2, l, rez, i;                            \
	    src_format *pcolor;                                            \
	                                                                   \
	    hx_1 *= src_stride;                                            \
	    hx *= src_stride;                                              \
	    hx1 *= src_stride;                                             \
	    hx2 *= src_stride;                                             \
	    if (dw == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    pcolor = table[l] - offset;                            \
		    a_1 =                                                  \
			(57 * pcolor[psrc[hx_1 + wx]] +                    \
			19 * pcolor[psrc[hx_1 + wx1]] -                    \
			9 * pcolor[psrc[hx_1 + wx_1]] -                    \
			3 * pcolor[psrc[hx_1 + wx2]]);                     \
		    a = (57 * pcolor[psrc[hx + wx]] +                      \
			19 * pcolor[psrc[hx + wx1]] -                      \
			9 * pcolor[psrc[hx + wx_1]] -                      \
			3 * pcolor[psrc[hx + wx2]]);                       \
		    a1 = (57 * pcolor[psrc[hx1 + wx]] +                    \
			19 * pcolor[psrc[hx1 + wx1]] -                     \
			9 * pcolor[psrc[hx1 + wx_1]] -                     \
			3 * pcolor[psrc[hx1 + wx2]]);                      \
		    p_1 = psrc + hx2 + wx_1;                               \
		    p = psrc + hx2 + wx;                                   \
		    p1 = psrc + hx2 + wx1;                                 \
		    p2 = psrc + hx2 + wx2;                                 \
		    i = 0;                                                 \
		    if (dh == 1) {                                         \
			a2 = 57 * pcolor[(*p)] + 19 * pcolor[(*p1)] -      \
			    9 * pcolor[(*p_1)] - 3 * pcolor[(*p2)];        \
			rez =                                              \
			    (19 * a + 57 * a1 - 3 * a_1 - 9 * a2) >> 12;   \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1 += src_stride;                                 \
			p += src_stride;                                   \
			p1 += src_stride;                                  \
			p2 += src_stride;                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < src_height - 1; i += 2) {                   \
			a2 = 57 * pcolor[(*p)] + 19 * pcolor[(*p1)] -      \
			    9 * pcolor[(*p_1)] - 3 * pcolor[(*p2)];        \
			rez =                                              \
			    (57 * a + 19 * a1 - 9 * a_1 - 3 * a2) >> 12;   \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			rez =                                              \
			    (19 * a + 57 * a1 - 3 * a_1 - 9 * a2) >> 12;   \
			SAT_##src_type();                                  \
			dst_buf1[chan * (i + 1) + l] = rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1 += src_stride;                                 \
			p += src_stride;                                   \
			p1 += src_stride;                                  \
			p2 += src_stride;                                  \
		    }                                                      \
		    if (i < src_height) {                                  \
			a2 = 57 * pcolor[(*p)] + 19 * pcolor[(*p1)] -      \
			    9 * pcolor[(*p_1)] - 3 * pcolor[(*p2)];        \
			rez =                                              \
			    (57 * a + 19 * a1 - 9 * a_1 - 3 * a2) >> 12;   \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    pcolor = table[l] - offset;                            \
		    a_1 =                                                  \
			(19 * pcolor[psrc[hx_1 + wx]] +                    \
			57 * pcolor[psrc[hx_1 + wx1]] -                    \
			3 * pcolor[psrc[hx_1 + wx_1]] -                    \
			9 * pcolor[psrc[hx_1 + wx2]]);                     \
		    a = (19 * pcolor[psrc[hx + wx]] +                      \
			57 * pcolor[psrc[hx + wx1]] -                      \
			3 * pcolor[psrc[hx + wx_1]] -                      \
			9 * pcolor[psrc[hx + wx2]]);                       \
		    a1 = (19 * pcolor[psrc[hx1 + wx]] +                    \
			57 * pcolor[psrc[hx1 + wx1]] -                     \
			3 * pcolor[psrc[hx1 + wx_1]] -                     \
			9 * pcolor[psrc[hx1 + wx2]]);                      \
		    p_1 = psrc + hx2 + wx_1;                               \
		    p = psrc + hx2 + wx;                                   \
		    p1 = psrc + hx2 + wx1;                                 \
		    p2 = psrc + hx2 + wx2;                                 \
		    i = 0;                                                 \
		    if (dh == 1) {                                         \
			a2 = 19 * pcolor[(*p)] + 57 * pcolor[(*p1)] -      \
			    3 * pcolor[(*p_1)] - 9 * pcolor[(*p2)];        \
			rez =                                              \
			    (19 * a + 57 * a1 - 3 * a_1 - 9 * a2) >> 12;   \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1 += src_stride;                                 \
			p += src_stride;                                   \
			p1 += src_stride;                                  \
			p2 += src_stride;                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < src_height - 1; i += 2) {                   \
			a2 = 19 * pcolor[(*p)] + 57 * pcolor[(*p1)] -      \
			    3 * pcolor[(*p_1)] - 9 * pcolor[(*p2)];        \
			rez =                                              \
			    (57 * a + 19 * a1 - 9 * a_1 - 3 * a2) >> 12;   \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			rez =                                              \
			    (19 * a + 57 * a1 - 3 * a_1 - 9 * a2) >> 12;   \
			SAT_##src_type();                                  \
			dst_buf1[chan * (i + 1) + l] = rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1 += src_stride;                                 \
			p += src_stride;                                   \
			p1 += src_stride;                                  \
			p2 += src_stride;                                  \
		    }                                                      \
		    if (i < src_height) {                                  \
			a2 = 19 * pcolor[(*p)] + 57 * pcolor[(*p1)] -      \
			    3 * pcolor[(*p_1)] - 9 * pcolor[(*p2)];        \
			rez =                                              \
			    (57 * a + 19 * a1 - 9 * a_1 - 3 * a2) >> 12;   \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	ROW_BICUBIC_FL1(j, chan, src_format, src_type)                     \
	{                                                                  \
	    mlib_s32 dh = (src_h_beg + j) & 1,                             \
		hx = (j + (src_h_beg & 1)) >> 1,                           \
		dw = (src_w_beg) & 1,                                      \
		wx = (src_w_beg & 1) >> 1,                                 \
		hx_1 = hx - 1,                                             \
		hx1 = hx + 1,                                              \
		hx2 = hx + 2,                                              \
		wx_1 = wx - 1,                                             \
		wx1 = wx + 1,                                              \
		wx2 = wx + 2;                                              \
	    mlib_s32 a_1, a, a1, a2, l, rez, i;                            \
	    src_format *pcolor;                                            \
	                                                                   \
	    hx_1 *= src_stride;                                            \
	    hx *= src_stride;                                              \
	    hx1 *= src_stride;                                             \
	    hx2 *= src_stride;                                             \
	    if (dh == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    pcolor = table[l] - offset;                            \
		    a_1 = pcolor[psrc[hx + wx_1]];                         \
		    a = pcolor[psrc[hx + wx]];                             \
		    a1 = pcolor[psrc[hx + wx1]];                           \
		    p = psrc + hx + wx2;                                   \
		    i = 0;                                                 \
		    if (dw == 1) {                                         \
			a2 = pcolor[(*p)];                                 \
			rez =                                              \
			    (19 * a + 57 * a1 - 3 * a_1 - 9 * a2) >> 6;    \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p++;                                               \
			i++;                                               \
		    }                                                      \
		    for (; i < src_width - 1; i += 2) {                    \
			a2 = pcolor[(*p)];                                 \
			rez =                                              \
			    (57 * a + 19 * a1 - 9 * a_1 - 3 * a2) >> 6;    \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			rez =                                              \
			    (19 * a + 57 * a1 - 3 * a_1 - 9 * a2) >> 6;    \
			SAT_##src_type();                                  \
			dst_buf1[chan * (i + 1) + l] = rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p++;                                               \
		    }                                                      \
		    if (i < src_width) {                                   \
			a2 = pcolor[(*p)];                                 \
			rez =                                              \
			    (57 * a + 19 * a1 - 9 * a_1 - 3 * a2) >> 6;    \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    pcolor = table[l] - offset;                            \
		    a_1 =                                                  \
			(5 * (pcolor[psrc[hx + wx_1]] +                    \
			(mlib_s32)pcolor[psrc[hx1 + wx_1]]) -              \
			pcolor[psrc[hx_1 + wx_1]] - pcolor[psrc[hx2 +      \
			wx_1]]);                                           \
		    a = (5 * (pcolor[psrc[hx + wx]] +                      \
			(mlib_s32)pcolor[psrc[hx1 + wx]]) -                \
			pcolor[psrc[hx_1 + wx]] - pcolor[psrc[hx2 +        \
			wx]]);                                             \
		    a1 = (5 * (pcolor[psrc[hx + wx1]] +                    \
			(mlib_s32)pcolor[psrc[hx1 + wx1]]) -               \
			pcolor[psrc[hx_1 + wx1]] - pcolor[psrc[hx2 +       \
			wx1]]);                                            \
		    p_1 = psrc + hx_1 + wx2;                               \
		    p = psrc + hx + wx2;                                   \
		    p1 = psrc + hx1 + wx2;                                 \
		    p2 = psrc + hx2 + wx2;                                 \
		    i = 0;                                                 \
		    if (dw == 1) {                                         \
			a2 = 5 * (pcolor[(*p)] +                           \
			    (mlib_s32)pcolor[(*p1)]) - pcolor[(*p_1)] -    \
			    pcolor[(*p2)];                                 \
			rez =                                              \
			    (19 * a + 57 * a1 - 3 * a_1 - 9 * a2) >> 9;    \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1++;                                             \
			p++;                                               \
			p1++;                                              \
			p2++;                                              \
			i++;                                               \
		    }                                                      \
		    for (; i < src_width - 1; i += 2) {                    \
			a2 = 5 * (pcolor[(*p)] +                           \
			    (mlib_s32)pcolor[(*p1)]) - pcolor[(*p_1)] -    \
			    pcolor[(*p2)];                                 \
			rez =                                              \
			    (57 * a + 19 * a1 - 9 * a_1 - 3 * a2) >> 9;    \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			rez =                                              \
			    (19 * a + 57 * a1 - 3 * a_1 - 9 * a2) >> 9;    \
			SAT_##src_type();                                  \
			dst_buf1[chan * (i + 1) + l] = rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1++;                                             \
			p++;                                               \
			p1++;                                              \
			p2++;                                              \
		    }                                                      \
		    if (i < src_width) {                                   \
			a2 = 5 * (pcolor[(*p)] +                           \
			    (mlib_s32)pcolor[(*p1)]) - pcolor[(*p_1)] -    \
			    pcolor[(*p2)];                                 \
			rez =                                              \
			    (57 * a + 19 * a1 - 9 * a_1 - 3 * a2) >> 9;    \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC_FL1(j, chan, src_format, src_type)                  \
	{                                                                  \
	    mlib_s32 dh = (src_h_beg) & 1,                                 \
		hx = (src_h_beg & 1) >> 1,                                 \
		dw = (src_w_beg + j) & 1,                                  \
		wx = (j + (src_w_beg & 1)) >> 1,                           \
		hx_1 = hx - 1,                                             \
		hx1 = hx + 1,                                              \
		hx2 = hx + 2,                                              \
		wx_1 = wx - 1,                                             \
		wx1 = wx + 1,                                              \
		wx2 = wx + 2;                                              \
	    mlib_s32 a_1, a, a1, a2, l, rez, i;                            \
	    src_format *pcolor;                                            \
	                                                                   \
	    hx_1 *= src_stride;                                            \
	    hx *= src_stride;                                              \
	    hx1 *= src_stride;                                             \
	    hx2 *= src_stride;                                             \
	    if (dw == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    pcolor = table[l] - offset;                            \
		    a_1 =                                                  \
			(57 * pcolor[psrc[hx_1 + wx]] +                    \
			19 * pcolor[psrc[hx_1 + wx1]] -                    \
			9 * pcolor[psrc[hx_1 + wx_1]] -                    \
			3 * pcolor[psrc[hx_1 + wx2]]);                     \
		    a = (57 * pcolor[psrc[hx + wx]] +                      \
			19 * pcolor[psrc[hx + wx1]] -                      \
			9 * pcolor[psrc[hx + wx_1]] -                      \
			3 * pcolor[psrc[hx + wx2]]);                       \
		    a1 = (57 * pcolor[psrc[hx1 + wx]] +                    \
			19 * pcolor[psrc[hx1 + wx1]] -                     \
			9 * pcolor[psrc[hx1 + wx_1]] -                     \
			3 * pcolor[psrc[hx1 + wx2]]);                      \
		    p_1 = psrc + hx2 + wx_1;                               \
		    p = psrc + hx2 + wx;                                   \
		    p1 = psrc + hx2 + wx1;                                 \
		    p2 = psrc + hx2 + wx2;                                 \
		    i = 0;                                                 \
		    if (dh == 0) {                                         \
			rez = a >> 6;                                      \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			i++;                                               \
		    }                                                      \
		    for (; i < src_height - 1; i += 2) {                   \
			a2 = 57 * pcolor[(*p)] + 19 * pcolor[(*p1)] -      \
			    9 * pcolor[(*p_1)] - 3 * pcolor[(*p2)];        \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;              \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			rez = a1 >> 6;                                     \
			SAT_##src_type();                                  \
			dst_buf1[chan * (i + 1) + l] = rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1 += src_stride;                                 \
			p += src_stride;                                   \
			p1 += src_stride;                                  \
			p2 += src_stride;                                  \
		    }                                                      \
		    if (i < src_height) {                                  \
			a2 = 57 * pcolor[(*p)] + 19 * pcolor[(*p1)] -      \
			    9 * pcolor[(*p_1)] - 3 * pcolor[(*p2)];        \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;              \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    pcolor = table[l] - offset;                            \
		    a_1 =                                                  \
			(19 * pcolor[psrc[hx_1 + wx]] +                    \
			57 * pcolor[psrc[hx_1 + wx1]] -                    \
			3 * pcolor[psrc[hx_1 + wx_1]] -                    \
			9 * pcolor[psrc[hx_1 + wx2]]);                     \
		    a = (19 * pcolor[psrc[hx + wx]] +                      \
			57 * pcolor[psrc[hx + wx1]] -                      \
			3 * pcolor[psrc[hx + wx_1]] -                      \
			9 * pcolor[psrc[hx + wx2]]);                       \
		    a1 = (19 * pcolor[psrc[hx1 + wx]] +                    \
			57 * pcolor[psrc[hx1 + wx1]] -                     \
			3 * pcolor[psrc[hx1 + wx_1]] -                     \
			9 * pcolor[psrc[hx1 + wx2]]);                      \
		    p_1 = psrc + hx2 + wx_1;                               \
		    p = psrc + hx2 + wx;                                   \
		    p1 = psrc + hx2 + wx1;                                 \
		    p2 = psrc + hx2 + wx2;                                 \
		    i = 0;                                                 \
		    if (dh == 0) {                                         \
			rez = a >> 6;                                      \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			i++;                                               \
		    }                                                      \
		    for (; i < src_height - 1; i += 2) {                   \
			a2 = 19 * pcolor[(*p)] + 57 * pcolor[(*p1)] -      \
			    3 * pcolor[(*p_1)] - 9 * pcolor[(*p2)];        \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;              \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			rez = a1 >> 6;                                     \
			SAT_##src_type();                                  \
			dst_buf1[chan * (i + 1) + l] = rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1 += src_stride;                                 \
			p += src_stride;                                   \
			p1 += src_stride;                                  \
			p2 += src_stride;                                  \
		    }                                                      \
		    if (i < src_height) {                                  \
			a2 = 19 * pcolor[(*p)] + 57 * pcolor[(*p1)] -      \
			    3 * pcolor[(*p_1)] - 9 * pcolor[(*p2)];        \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;              \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	ROW_BICUBIC_FL2(j, chan, src_format, src_type)                     \
	{                                                                  \
	    mlib_s32 dh = (src_h_beg + j) & 1,                             \
		hx = (j + (src_h_beg & 1)) >> 1,                           \
		dw = (src_w_beg) & 1,                                      \
		wx = (src_w_beg & 1) >> 1,                                 \
		hx_1 = hx - 1,                                             \
		hx1 = hx + 1,                                              \
		hx2 = hx + 2,                                              \
		wx_1 = wx - 1,                                             \
		wx1 = wx + 1,                                              \
		wx2 = wx + 2;                                              \
	    mlib_s32 a_1, a, a1, a2, l, rez, i;                            \
	    src_format *pcolor;                                            \
	                                                                   \
	    hx_1 *= src_stride;                                            \
	    hx *= src_stride;                                              \
	    hx1 *= src_stride;                                             \
	    hx2 *= src_stride;                                             \
	    if (dh == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    pcolor = table[l] - offset;                            \
		    a_1 =                                                  \
			(57 * pcolor[psrc[hx + wx_1]] +                    \
			19 * pcolor[psrc[hx1 + wx_1]] -                    \
			9 * pcolor[psrc[hx_1 + wx_1]] -                    \
			3 * pcolor[psrc[hx2 + wx_1]]);                     \
		    a = (57 * pcolor[psrc[hx + wx]] +                      \
			19 * pcolor[psrc[hx1 + wx]] -                      \
			9 * pcolor[psrc[hx_1 + wx]] -                      \
			3 * pcolor[psrc[hx2 + wx]]);                       \
		    a1 = (57 * pcolor[psrc[hx + wx1]] +                    \
			19 * pcolor[psrc[hx1 + wx1]] -                     \
			9 * pcolor[psrc[hx_1 + wx1]] -                     \
			3 * pcolor[psrc[hx2 + wx1]]);                      \
		    p_1 = psrc + hx_1 + wx2;                               \
		    p = psrc + hx + wx2;                                   \
		    p1 = psrc + hx1 + wx2;                                 \
		    p2 = psrc + hx2 + wx2;                                 \
		    i = 0;                                                 \
		    if (dw == 0) {                                         \
			rez = a >> 6;                                      \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			i++;                                               \
		    }                                                      \
		    for (; i < src_width - 1; i += 2) {                    \
			a2 = 57 * pcolor[(*p)] + 19 * pcolor[(*p1)] -      \
			    9 * pcolor[(*p_1)] - 3 * pcolor[(*p2)];        \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;              \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			rez = a1 >> 6;                                     \
			SAT_##src_type();                                  \
			dst_buf1[chan * (i + 1) + l] = rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1++;                                             \
			p++;                                               \
			p1++;                                              \
			p2++;                                              \
		    }                                                      \
		    if (i < src_width) {                                   \
			a2 = 57 * pcolor[(*p)] + 19 * pcolor[(*p1)] -      \
			    9 * pcolor[(*p_1)] - 3 * pcolor[(*p2)];        \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;              \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    pcolor = table[l] - offset;                            \
		    a_1 =                                                  \
			(19 * pcolor[psrc[hx + wx_1]] +                    \
			57 * pcolor[psrc[hx1 + wx_1]] -                    \
			3 * pcolor[psrc[hx_1 + wx_1]] -                    \
			9 * pcolor[psrc[hx2 + wx_1]]);                     \
		    a = (19 * pcolor[psrc[hx + wx]] +                      \
			57 * pcolor[psrc[hx1 + wx]] -                      \
			3 * pcolor[psrc[hx_1 + wx]] -                      \
			9 * pcolor[psrc[hx2 + wx]]);                       \
		    a1 = (19 * pcolor[psrc[hx + wx1]] +                    \
			57 * pcolor[psrc[hx1 + wx1]] -                     \
			3 * pcolor[psrc[hx_1 + wx1]] -                     \
			9 * pcolor[psrc[hx2 + wx1]]);                      \
		    p_1 = psrc + hx_1 + wx2;                               \
		    p = psrc + hx + wx2;                                   \
		    p1 = psrc + hx1 + wx2;                                 \
		    p2 = psrc + hx2 + wx2;                                 \
		    i = 0;                                                 \
		    if (dw == 0) {                                         \
			rez = a >> 6;                                      \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			i++;                                               \
		    }                                                      \
		    for (; i < src_width - 1; i += 2) {                    \
			a2 = 19 * pcolor[(*p)] + 57 * pcolor[(*p1)] -      \
			    3 * pcolor[(*p_1)] - 9 * pcolor[(*p2)];        \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;              \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			rez = a1 >> 6;                                     \
			SAT_##src_type();                                  \
			dst_buf1[chan * (i + 1) + l] = rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1++;                                             \
			p++;                                               \
			p1++;                                              \
			p2++;                                              \
		    }                                                      \
		    if (i < src_width) {                                   \
			a2 = 19 * pcolor[(*p)] + 57 * pcolor[(*p1)] -      \
			    3 * pcolor[(*p_1)] - 9 * pcolor[(*p2)];        \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;              \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC_FL2(j, chan, src_format, src_type)                  \
	{                                                                  \
	    mlib_s32 dh = (src_h_beg) & 1,                                 \
		hx = (src_h_beg & 1) >> 1,                                 \
		dw = (src_w_beg + j) & 1,                                  \
		wx = (j + (src_w_beg & 1)) >> 1,                           \
		hx_1 = hx - 1,                                             \
		hx1 = hx + 1,                                              \
		hx2 = hx + 2,                                              \
		wx_1 = wx - 1,                                             \
		wx1 = wx + 1,                                              \
		wx2 = wx + 2;                                              \
	    mlib_s32 a_1, a, a1, a2, l, rez, i;                            \
	    src_format *pcolor;                                            \
	                                                                   \
	    hx_1 *= src_stride;                                            \
	    hx *= src_stride;                                              \
	    hx1 *= src_stride;                                             \
	    hx2 *= src_stride;                                             \
	    if (dw == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    pcolor = table[l] - offset;                            \
		    a_1 = pcolor[psrc[hx_1 + wx]];                         \
		    a = pcolor[psrc[hx + wx]];                             \
		    a1 = pcolor[psrc[hx1 + wx]];                           \
		    p = psrc + hx2 + wx;                                   \
		    i = 0;                                                 \
		    if (dh == 1) {                                         \
			a2 = pcolor[(*p)];                                 \
			rez =                                              \
			    (19 * a + 57 * a1 - 3 * a_1 - 9 * a2) >> 6;    \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p += src_stride;                                   \
			i++;                                               \
		    }                                                      \
		    for (; i < src_height - 1; i += 2) {                   \
			a2 = pcolor[(*p)];                                 \
			rez =                                              \
			    (57 * a + 19 * a1 - 9 * a_1 - 3 * a2) >> 6;    \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			rez =                                              \
			    (19 * a + 57 * a1 - 3 * a_1 - 9 * a2) >> 6;    \
			SAT_##src_type();                                  \
			dst_buf1[chan * (i + 1) + l] = rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p += src_stride;                                   \
		    }                                                      \
		    if (i < src_height) {                                  \
			a2 = pcolor[(*p)];                                 \
			rez =                                              \
			    (57 * a + 19 * a1 - 9 * a_1 - 3 * a2) >> 6;    \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    pcolor = table[l] - offset;                            \
		    a_1 =                                                  \
			(5 * (pcolor[psrc[hx_1 + wx]] +                    \
			(mlib_s32)pcolor[psrc[hx_1 + wx1]]) -              \
			pcolor[psrc[hx_1 + wx_1]] - pcolor[psrc[hx_1 +     \
			wx2]]);                                            \
		    a = (5 * (pcolor[psrc[hx + wx]] +                      \
			(mlib_s32)pcolor[psrc[hx + wx1]]) -                \
			pcolor[psrc[hx + wx_1]] - pcolor[psrc[hx +         \
			wx2]]);                                            \
		    a1 = (5 * (pcolor[psrc[hx1 + wx]] +                    \
			(mlib_s32)pcolor[psrc[hx1 + wx1]]) -               \
			pcolor[psrc[hx1 + wx_1]] - pcolor[psrc[hx1 +       \
			wx2]]);                                            \
		    p_1 = psrc + hx2 + wx_1;                               \
		    p = psrc + hx2 + wx;                                   \
		    p1 = psrc + hx2 + wx1;                                 \
		    p2 = psrc + hx2 + wx2;                                 \
		    i = 0;                                                 \
		    if (dh == 1) {                                         \
			a2 = 5 * (pcolor[(*p)] +                           \
			    (mlib_s32)pcolor[(*p1)]) - pcolor[(*p_1)] -    \
			    pcolor[(*p2)];                                 \
			rez =                                              \
			    (19 * a + 57 * a1 - 3 * a_1 - 9 * a2) >> 9;    \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1 += src_stride;                                 \
			p += src_stride;                                   \
			p1 += src_stride;                                  \
			p2 += src_stride;                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < src_height - 1; i += 2) {                   \
			a2 = 5 * (pcolor[(*p)] +                           \
			    (mlib_s32)pcolor[(*p1)]) - pcolor[(*p_1)] -    \
			    pcolor[(*p2)];                                 \
			rez =                                              \
			    (57 * a + 19 * a1 - 9 * a_1 - 3 * a2) >> 9;    \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			rez =                                              \
			    (19 * a + 57 * a1 - 3 * a_1 - 9 * a2) >> 9;    \
			SAT_##src_type();                                  \
			dst_buf1[chan * (i + 1) + l] = rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1 += src_stride;                                 \
			p += src_stride;                                   \
			p1 += src_stride;                                  \
			p2 += src_stride;                                  \
		    }                                                      \
		    if (i < src_height) {                                  \
			a2 = 5 * (pcolor[(*p)] +                           \
			    (mlib_s32)pcolor[(*p1)]) - pcolor[(*p_1)] -    \
			    pcolor[(*p2)];                                 \
			rez =                                              \
			    (57 * a + 19 * a1 - 9 * a_1 - 3 * a2) >> 9;    \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	ROW_BICUBIC_FL3(j, chan, src_format, src_type)                     \
	{                                                                  \
	    mlib_s32 dh = (src_h_beg + j) & 1,                             \
		hx = (j + (src_h_beg & 1)) >> 1,                           \
		dw = (src_w_beg) & 1,                                      \
		wx = (src_w_beg & 1) >> 1,                                 \
		hx_1 = hx - 1,                                             \
		hx1 = hx + 1,                                              \
		hx2 = hx + 2,                                              \
		wx_1 = wx - 1,                                             \
		wx1 = wx + 1,                                              \
		wx2 = wx + 2;                                              \
	    mlib_s32 a_1, a, a1, a2, l, rez, i;                            \
	    src_format *pcolor;                                            \
	                                                                   \
	    hx_1 *= src_stride;                                            \
	    hx *= src_stride;                                              \
	    hx1 *= src_stride;                                             \
	    hx2 *= src_stride;                                             \
	    if (dh == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    pcolor = table[l] - offset;                            \
		    a_1 = pcolor[psrc[hx + wx_1]];                         \
		    a = pcolor[psrc[hx + wx]];                             \
		    a1 = pcolor[psrc[hx + wx1]];                           \
		    p = psrc + hx + wx2;                                   \
		    i = 0;                                                 \
		    if (dw == 0) {                                         \
			dst_buf1[chan * i + l] = a;                        \
			i++;                                               \
		    }                                                      \
		    for (; i < src_width - 1; i += 2) {                    \
			a2 = pcolor[(*p)];                                 \
			rez = (5 * (a + a1) - a_1 - a2) >> 3;              \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			dst_buf1[chan * (i + 1) + l] = a1;                 \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p++;                                               \
		    }                                                      \
		    if (i < src_width) {                                   \
			a2 = pcolor[(*p)];                                 \
			rez = (5 * (a + a1) - a_1 - a2) >> 3;              \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    pcolor = table[l] - offset;                            \
		    a_1 =                                                  \
			(5 * (pcolor[psrc[hx + wx_1]] +                    \
			(mlib_s32)pcolor[psrc[hx1 + wx_1]]) -              \
			pcolor[psrc[hx_1 + wx_1]] - pcolor[psrc[hx2 +      \
			wx_1]]);                                           \
		    a = (5 * (pcolor[psrc[hx + wx]] +                      \
			(mlib_s32)pcolor[psrc[hx1 + wx]]) -                \
			pcolor[psrc[hx_1 + wx]] - pcolor[psrc[hx2 +        \
			wx]]);                                             \
		    a1 = (5 * (pcolor[psrc[hx + wx1]] +                    \
			(mlib_s32)pcolor[psrc[hx1 + wx1]]) -               \
			pcolor[psrc[hx_1 + wx1]] - pcolor[psrc[hx2 +       \
			wx1]]);                                            \
		    p_1 = psrc + hx_1 + wx2;                               \
		    p = psrc + hx + wx2;                                   \
		    p1 = psrc + hx1 + wx2;                                 \
		    p2 = psrc + hx2 + wx2;                                 \
		    i = 0;                                                 \
		    if (dw == 0) {                                         \
			rez = a >> 3;                                      \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			i++;                                               \
		    }                                                      \
		    for (; i < src_width - 1; i += 2) {                    \
			a2 = 5 * (pcolor[(*p)] +                           \
			    (mlib_s32)pcolor[(*p1)]) - pcolor[(*p_1)] -    \
			    pcolor[(*p2)];                                 \
			rez = (5 * (a + a1) - a_1 - a2) >> 6;              \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			rez = a1 >> 3;                                     \
			SAT_##src_type();                                  \
			dst_buf1[chan * (i + 1) + l] = rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1++;                                             \
			p++;                                               \
			p1++;                                              \
			p2++;                                              \
		    }                                                      \
		    if (i < src_width) {                                   \
			a2 = 5 * (pcolor[(*p)] +                           \
			    (mlib_s32)pcolor[(*p1)]) - pcolor[(*p_1)] -    \
			    pcolor[(*p2)];                                 \
			rez = (5 * (a + a1) - a_1 - a2) >> 6;              \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC_FL3(j, chan, src_format, src_type)                  \
	{                                                                  \
	    mlib_s32 dh = (src_h_beg) & 1,                                 \
		hx = (src_h_beg & 1) >> 1,                                 \
		dw = (src_w_beg + j) & 1,                                  \
		wx = (j + (src_w_beg & 1)) >> 1,                           \
		hx_1 = hx - 1,                                             \
		hx1 = hx + 1,                                              \
		hx2 = hx + 2,                                              \
		wx_1 = wx - 1,                                             \
		wx1 = wx + 1,                                              \
		wx2 = wx + 2;                                              \
	    mlib_s32 a_1, a, a1, a2, l, rez, i;                            \
	    src_format *pcolor;                                            \
	                                                                   \
	    hx_1 *= src_stride;                                            \
	    hx *= src_stride;                                              \
	    hx1 *= src_stride;                                             \
	    hx2 *= src_stride;                                             \
	    if (dw == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    pcolor = table[l] - offset;                            \
		    a_1 = pcolor[psrc[hx_1 + wx]];                         \
		    a = pcolor[psrc[hx + wx]];                             \
		    a1 = pcolor[psrc[hx1 + wx]];                           \
		    p = psrc + hx2 + wx;                                   \
		    i = 0;                                                 \
		    if (dh == 0) {                                         \
			dst_buf1[chan * i + l] = a;                        \
			i++;                                               \
		    }                                                      \
		    for (; i < src_height - 1; i += 2) {                   \
			a2 = pcolor[(*p)];                                 \
			rez = (5 * (a + a1) - a_1 - a2) >> 3;              \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			dst_buf1[chan * (i + 1) + l] = a1;                 \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p += src_stride;                                   \
		    }                                                      \
		    if (i < src_height) {                                  \
			a2 = pcolor[(*p)];                                 \
			rez = (5 * (a + a1) - a_1 - a2) >> 3;              \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    pcolor = table[l] - offset;                            \
		    a_1 =                                                  \
			(5 * (pcolor[psrc[hx_1 + wx]] +                    \
			(mlib_s32)pcolor[psrc[hx_1 + wx1]]) -              \
			pcolor[psrc[hx_1 + wx_1]] - pcolor[psrc[hx_1 +     \
			wx2]]);                                            \
		    a = (5 * (pcolor[psrc[hx + wx]] +                      \
			(mlib_s32)pcolor[psrc[hx + wx1]]) -                \
			pcolor[psrc[hx + wx_1]] - pcolor[psrc[hx +         \
			wx2]]);                                            \
		    a1 = (5 * (pcolor[psrc[hx1 + wx]] +                    \
			(mlib_s32)pcolor[psrc[hx1 + wx1]]) -               \
			pcolor[psrc[hx1 + wx_1]] - pcolor[psrc[hx1 +       \
			wx2]]);                                            \
		    p_1 = psrc + hx2 + wx_1;                               \
		    p = psrc + hx2 + wx;                                   \
		    p1 = psrc + hx2 + wx1;                                 \
		    p2 = psrc + hx2 + wx2;                                 \
		    i = 0;                                                 \
		    if (dh == 0) {                                         \
			rez = a >> 3;                                      \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			i++;                                               \
		    }                                                      \
		    for (; i < src_height - 1; i += 2) {                   \
			a2 = 5 * (pcolor[(*p)] +                           \
			    (mlib_s32)pcolor[(*p1)]) - pcolor[(*p_1)] -    \
			    pcolor[(*p2)];                                 \
			rez = (5 * (a + a1) - a_1 - a2) >> 6;              \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
			rez = a1 >> 3;                                     \
			SAT_##src_type();                                  \
			dst_buf1[chan * (i + 1) + l] = rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1 += src_stride;                                 \
			p += src_stride;                                   \
			p1 += src_stride;                                  \
			p2 += src_stride;                                  \
		    }                                                      \
		    if (i < src_height) {                                  \
			a2 = 5 * (pcolor[(*p)] +                           \
			    (mlib_s32)pcolor[(*p1)]) - pcolor[(*p_1)] -    \
			    pcolor[(*p2)];                                 \
			rez = (5 * (a + a1) - a_1 - a2) >> 6;              \
			SAT_##src_type();                                  \
			dst_buf1[chan * i + l] = rez;                      \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	SAT_U8()                                                \
	imask = (rez >> 5) & 0x18;                              \
	rez = (rez & (mask1 >> imask)) | (mask2 >> imask)

/* *********************************************************** */

#define	SAT_S16()                                               \
	mask = *((mlib_u32 *)(pmask + ((rez >> 13) & 0xc)));    \
	rez = (rez & mask) | (mask >> 16)

/* *********************************************************** */

#define	DO_BICUBIC_TWO_ROW_FL0(src_format, src_type, channels)           \
	for (k = 0; k < channels; k++) {                                 \
	    src_format *pcolor = table[k] - offset;                      \
	    mlib_s32 a, b;                                               \
	    mlib_s32 st0, st0_, st1, st1_, st2, st2_, st3, st3_;         \
	                                                                 \
	    a = 19 * pcolor[psrc[i * src_stride - 1]];                   \
	    b = 19 * pcolor[psrc[(i + 1) * src_stride - 1]];             \
	    st0 =                                                        \
		a + b + (a << 1) - 9 * pcolor[psrc[(i -                  \
		1) * src_stride - 1]] - 3 * pcolor[psrc[(i +             \
		2) * src_stride - 1]];                                   \
	    st0_ =                                                       \
		a + b + (b << 1) - 3 * pcolor[psrc[(i -                  \
		1) * src_stride - 1]] - 9 * pcolor[psrc[(i +             \
		2) * src_stride - 1]];                                   \
	    a = 19 * pcolor[psrc[i * src_stride]];                       \
	    b = 19 * pcolor[psrc[(i + 1) * src_stride]];                 \
	    st1 =                                                        \
		a + b + (a << 1) - 9 * pcolor[psrc[(i -                  \
		1) * src_stride]] - 3 * pcolor[psrc[(i +                 \
		2) * src_stride]];                                       \
	    st1_ =                                                       \
		a + b + (b << 1) - 3 * pcolor[psrc[(i -                  \
		1) * src_stride]] - 9 * pcolor[psrc[(i +                 \
		2) * src_stride]];                                       \
	    a = 19 * pcolor[psrc[i * src_stride + 1]];                   \
	    b = 19 * pcolor[psrc[(i + 1) * src_stride + 1]];             \
	    st2 =                                                        \
		a + b + (a << 1) - 9 * pcolor[psrc[(i -                  \
		1) * src_stride + 1]] - 3 * pcolor[psrc[(i +             \
		2) * src_stride + 1]];                                   \
	    st2_ =                                                       \
		a + b + (b << 1) - 3 * pcolor[psrc[(i -                  \
		1) * src_stride + 1]] - 9 * pcolor[psrc[(i +             \
		2) * src_stride + 1]];                                   \
	    for (j = 0; j < (src_width / 2); j++) {                      \
		a = 19 * pcolor[psrc[i * src_stride + (j + 2)]];         \
		b = 19 * pcolor[psrc[(i + 1) * src_stride + (j + 2)]];   \
		st3 =                                                    \
		    a + b + (a << 1) - 9 * pcolor[psrc[(i -              \
		    1) * src_stride + (j + 2)]] - 3 * pcolor[psrc[(i +   \
		    2) * src_stride + (j + 2)]];                         \
		st3_ =                                                   \
		    a + b + (b << 1) - 3 * pcolor[psrc[(i -              \
		    1) * src_stride + (j + 2)]] - 9 * pcolor[psrc[(i +   \
		    2) * src_stride + (j + 2)]];                         \
		a = 19 * st1;                                            \
		b = 19 * st2;                                            \
		rez = (a + b + (a << 1) - 9 * st0 - 3 * st3) >> 12;      \
		SAT_##src_type();                                        \
		dst_buf1[2 * j * channels + k] = rez;                    \
		rez = (a + b + (b << 1) - 3 * st0 - 9 * st3) >> 12;      \
		SAT_##src_type();                                        \
		dst_buf1[(2 * j + 1) * channels + k] = rez;              \
		a = 19 * st1_;                                           \
		b = 19 * st2_;                                           \
		rez = (a + b + (a << 1) - 9 * st0_ - 3 * st3_) >> 12;    \
		SAT_##src_type();                                        \
		dst_buf2[2 * j * channels + k] = rez;                    \
		rez = (a + b + (b << 1) - 3 * st0_ - 9 * st3_) >> 12;    \
		SAT_##src_type();                                        \
		dst_buf2[(2 * j + 1) * channels + k] = rez;              \
		st0 = st1;                                               \
		st1 = st2;                                               \
		st2 = st3;                                               \
		st0_ = st1_;                                             \
		st1_ = st2_;                                             \
		st2_ = st3_;                                             \
	    }                                                            \
	}

/* *********************************************************** */

#define	DO_BICUBIC_TWO_ROW_FL1(src_format, src_type, channels)           \
	for (k = 0; k < channels; k++) {                                 \
	    src_format *pcolor = table[k] - offset;                      \
	    mlib_s32 st0 = pcolor[psrc[i * src_stride - 1]];             \
	    mlib_s32 st0_ =                                              \
		(5 * (pcolor[psrc[i * src_stride - 1]] +                 \
		(mlib_s32)pcolor[psrc[(i + 1) * src_stride - 1]]) -      \
		pcolor[psrc[(i - 1) * src_stride - 1]] -                 \
		pcolor[psrc[(i + 2) * src_stride - 1]]);                 \
	    mlib_s32 st1 = pcolor[psrc[i * src_stride]];                 \
	    mlib_s32 st1_ =                                              \
		(5 * (pcolor[psrc[i * src_stride]] +                     \
		(mlib_s32)pcolor[psrc[(i + 1) * src_stride]]) -          \
		pcolor[psrc[(i - 1) * src_stride]] - pcolor[psrc[(i +    \
		2) * src_stride]]);                                      \
	    mlib_s32 st2 = pcolor[psrc[i * src_stride + 1]];             \
	    mlib_s32 st2_ =                                              \
		(5 * (pcolor[psrc[i * src_stride + 1]] +                 \
		(mlib_s32)pcolor[psrc[(i + 1) * src_stride + 1]]) -      \
		pcolor[psrc[(i - 1) * src_stride + 1]] -                 \
		pcolor[psrc[(i + 2) * src_stride + 1]]);                 \
	    for (j = 0; j < (src_width / 2); j++) {                      \
		mlib_s32 st3 = pcolor[psrc[i * src_stride + (j + 2)]];   \
		mlib_s32 st3_ =                                          \
		    (5 * (pcolor[psrc[i * src_stride + (j + 2)]] +       \
		    (mlib_s32)pcolor[psrc[(i + 1) * src_stride + (j +    \
		    2)]]) - pcolor[psrc[(i - 1) * src_stride + (j +      \
		    2)]] - pcolor[psrc[(i + 2) * src_stride + (j +       \
		    2)]]);                                               \
		mlib_s32 a = 19 * st1;                                   \
		mlib_s32 b = 19 * st2;                                   \
	                                                                 \
		rez = (a + b + (a << 1) - 9 * st0 - 3 * st3) >> 6;       \
		SAT_##src_type();                                        \
		dst_buf1[2 * j * channels + k] = rez;                    \
		rez = (a + b + (b << 1) - 3 * st0 - 9 * st3) >> 6;       \
		SAT_##src_type();                                        \
		dst_buf1[(2 * j + 1) * channels + k] = rez;              \
		a = 19 * st1_;                                           \
		b = 19 * st2_;                                           \
		rez = (a + b + (a << 1) - 9 * st0_ - 3 * st3_) >> 9;     \
		SAT_##src_type();                                        \
		dst_buf2[2 * j * channels + k] = rez;                    \
		rez = (a + b + (b << 1) - 3 * st0_ - 9 * st3_) >> 9;     \
		SAT_##src_type();                                        \
		dst_buf2[(2 * j + 1) * channels + k] = rez;              \
		st0 = st1;                                               \
		st1 = st2;                                               \
		st2 = st3;                                               \
		st0_ = st1_;                                             \
		st1_ = st2_;                                             \
		st2_ = st3_;                                             \
	    }                                                            \
	}

/* *********************************************************** */

#define	DO_BICUBIC_TWO_ROW_FL2(src_format, src_type, channels)           \
	for (k = 0; k < channels; k++) {                                 \
	    src_format *pcolor = table[k] - offset;                      \
	    mlib_s32 a, b, c, d;                                         \
	    mlib_s32 st0, st0_, st1, st1_, st2, st2_, st3, st3_;         \
	                                                                 \
	    a = 19 * pcolor[psrc[i * src_stride - 1]];                   \
	    b = 19 * pcolor[psrc[(i + 1) * src_stride - 1]];             \
	    c = 3 * pcolor[psrc[(i - 1) * src_stride - 1]];              \
	    d = 3 * pcolor[psrc[(i + 2) * src_stride - 1]];              \
	    st0 = a + b + (a << 1) - c - (c << 1) - d;                   \
	    st0_ = a + b + (b << 1) - c - d - (d << 1);                  \
	    a = 19 * pcolor[psrc[i * src_stride]];                       \
	    b = 19 * pcolor[psrc[(i + 1) * src_stride]];                 \
	    c = 3 * pcolor[psrc[(i - 1) * src_stride]];                  \
	    d = 3 * pcolor[psrc[(i + 2) * src_stride]];                  \
	    st1 = a + b + (a << 1) - c - (c << 1) - d;                   \
	    st1_ = a + b + (b << 1) - c - d - (d << 1);                  \
	    a = 19 * pcolor[psrc[i * src_stride + 1]];                   \
	    b = 19 * pcolor[psrc[(i + 1) * src_stride + 1]];             \
	    c = 3 * pcolor[psrc[(i - 1) * src_stride + 1]];              \
	    d = 3 * pcolor[psrc[(i + 2) * src_stride + 1]];              \
	    st2 = a + b + (a << 1) - c - (c << 1) - d;                   \
	    st2_ = a + b + (b << 1) - c - d - (d << 1);                  \
	    for (j = 0; j < (src_width / 2); j++) {                      \
		a = 19 * pcolor[psrc[i * src_stride + (j + 2)]];         \
		b = 19 * pcolor[psrc[(i + 1) * src_stride + (j + 2)]];   \
		c = 3 * pcolor[psrc[(i - 1) * src_stride + (j + 2)]];    \
		d = 3 * pcolor[psrc[(i + 2) * src_stride + (j + 2)]];    \
		st3 = a + b + (a << 1) - c - (c << 1) - d;               \
		st3_ = a + b + (b << 1) - c - d - (d << 1);              \
		rez = st1 >> 6;                                          \
		SAT_##src_type();                                        \
		dst_buf1[2 * j * channels + k] = rez;                    \
		rez = (5 * st1 + 5 * st2 - st0 - st3) >> 9;              \
		SAT_##src_type();                                        \
		dst_buf1[(2 * j + 1) * channels + k] = rez;              \
		rez = st1_ >> 6;                                         \
		SAT_##src_type();                                        \
		dst_buf2[2 * j * channels + k] = rez;                    \
		rez = (5 * st1_ + 5 * st2_ - st0_ - st3_) >> 9;          \
		SAT_##src_type();                                        \
		dst_buf2[(2 * j + 1) * channels + k] = rez;              \
		st0 = st1;                                               \
		st1 = st2;                                               \
		st2 = st3;                                               \
		st0_ = st1_;                                             \
		st1_ = st2_;                                             \
		st2_ = st3_;                                             \
	    }                                                            \
	}

/* *********************************************************** */

#define	DO_BICUBIC_TWO_ROW_FL3(src_format, src_type, channels)            \
	for (k = 0; k < channels; k++) {                                  \
	    src_format *pcolor = table[k] - offset;                       \
	    mlib_s32 a10 = pcolor[psrc[i * src_stride - 1]], a11 =        \
		pcolor[psrc[i * src_stride]], a12 =                       \
		pcolor[psrc[i * src_stride + 1]];                         \
	    mlib_s32 st0 =                                                \
		(5 * (a10 + pcolor[psrc[(i + 1) * src_stride - 1]]) -     \
		pcolor[psrc[(i - 1) * src_stride - 1]] -                  \
		pcolor[psrc[(i + 2) * src_stride - 1]]);                  \
	    mlib_s32 st1 =                                                \
		(5 * (a11 + pcolor[psrc[(i + 1) * src_stride]]) -         \
		pcolor[psrc[(i - 1) * src_stride]] - pcolor[psrc[(i +     \
		2) * src_stride]]);                                       \
	    mlib_s32 st2 =                                                \
		(5 * (a12 + pcolor[psrc[(i + 1) * src_stride + 1]]) -     \
		pcolor[psrc[(i - 1) * src_stride + 1]] -                  \
		pcolor[psrc[(i + 2) * src_stride + 1]]);                  \
	    for (j = 0; j < (src_width / 2); j++) {                       \
		mlib_s32 a13 = pcolor[psrc[i * src_stride + (j + 2)]];    \
		mlib_s32 st3 =                                            \
		    (5 * (a13 + pcolor[psrc[(i + 1) * src_stride + (j +   \
		    2)]]) - pcolor[psrc[(i - 1) * src_stride + (j +       \
		    2)]] - pcolor[psrc[(i + 2) * src_stride + (j +        \
		    2)]]);                                                \
		dst_buf1[2 * j * channels + k] = a11;                     \
		rez = (5 * (a11 + a12) - a10 - a13) >> 3;                 \
		SAT_##src_type();                                         \
		dst_buf1[(2 * j + 1) * channels + k] = rez;               \
		rez = st1 >> 3;                                           \
		SAT_##src_type();                                         \
		dst_buf2[2 * j * channels + k] = rez;                     \
		rez = (5 * (st1 + st2) - st0 - st3) >> 6;                 \
		SAT_##src_type();                                         \
		dst_buf2[(2 * j + 1) * channels + k] = rez;               \
		st0 = st1;                                                \
		st1 = st2;                                                \
		st2 = st3;                                                \
		a10 = a11;                                                \
		a11 = a12;                                                \
		a12 = a13;                                                \
	    }                                                             \
	}

/* *********************************************************** */

#define	IDENTIFICATORS_FOR_SATURATION_U8()                       \
	mlib_s32 imask, mask1 = 0x000000ff, mask2 = 0x0000ff00

/* *********************************************************** */

#define	IDENTIFICATORS_FOR_SATURATION_S16()                     \
	mlib_u32 mlib_mask32[] = {                              \
	    0x0000ffff, 0x7fff0000, 0x80000000, 0x0000ffff      \
	};                                                      \
	mlib_s32 mask;                                          \
	mlib_u8 *pmask = (mlib_u8 *)mlib_mask32

/* *********************************************************** */

#define	MLIB_ZOOMIN2XINDEX_BC(src_format, src_type, dst_type,       \
	channels, SHIFT, FUNC_NAME)                                 \
	{                                                           \
	    mlib_s32 i, j, k;                                       \
	                                                            \
/*                                                                  \
 * indices                                                          \
 */                                                                 \
	    mlib_s32 rez;                                           \
	                                                            \
	    IDENTIFICATORS_FOR_SATURATION_##src_type();             \
	    if (src_h_beg & 1) {                                    \
	                                                            \
/*                                                                  \
 * upper bound of image                                             \
 */                                                                 \
		ROW_BICUBIC_FL##SHIFT(0, channels, src_format,      \
		    src_type);                                      \
		FUNC_NAME##src_type##_##dst_type##_##channels       \
		    (dst_buf1, pdst, src_width, colormap);          \
		src_h_beg = 0;                                      \
		psrc += src_stride;                                 \
		pdst += dst_stride;                                 \
		src_height--;                                       \
	    }                                                       \
	    if (src_w_beg & 1) {                                    \
	                                                            \
/*                                                                  \
 * left bound of image                                              \
 */                                                                 \
		COLUMN_BICUBIC_FL##SHIFT(0, channels, src_format,   \
		    src_type);                                      \
		FUNC_NAME##src_type##_##dst_type##_##channels       \
		    (dst_buf1, pdst_buf, src_height, colormap);     \
		for (k = 0; k < src_height; k++)                    \
		    pdst[k * dst_stride] = pdst_buf[k];             \
		src_w_beg = 0;                                      \
		psrc += 1;                                          \
		pdst += 1;                                          \
		src_width--;                                        \
	    }                                                       \
	    for (i = 0; i < (src_height / 2); i++) {                \
		DO_BICUBIC_TWO_ROW_FL##SHIFT(src_format, src_type,  \
		    channels);                                      \
		FUNC_NAME##src_type##_##dst_type##_##channels       \
		    (dst_buf1, pdst + 2 * i * dst_stride,           \
		    2 * (src_width / 2), colormap);                 \
		FUNC_NAME##src_type##_##dst_type##_##channels       \
		    (dst_buf2, pdst + (2 * i + 1) * dst_stride,     \
		    2 * (src_width / 2), colormap);                 \
	    }                                                       \
	    if ((src_height & 1) && (src_height > 0)) {             \
	                                                            \
/*                                                                  \
 * lower bound of image                                             \
 */                                                                 \
		src_height--;                                       \
		ROW_BICUBIC_FL##SHIFT(src_height, channels,         \
		    src_format, src_type);                          \
		FUNC_NAME##src_type##_##dst_type##_##channels       \
		    (dst_buf1, pdst + src_height * dst_stride,      \
		    src_width, colormap);                           \
	    }                                                       \
	    if ((src_width & 1) && (src_width > 0)) {               \
	                                                            \
/*                                                                  \
 * right bound of image                                             \
 */                                                                 \
		src_width--;                                        \
		COLUMN_BICUBIC_FL##SHIFT(src_width, channels,       \
		    src_format, src_type);                          \
		FUNC_NAME##src_type##_##dst_type##_##channels       \
		    (dst_buf1, pdst_buf, src_height, colormap);     \
		for (k = 0; k < src_height; k++)                    \
		    pdst[k * dst_stride + src_width] = pdst_buf[k]; \
	    }                                                       \
	}

/* *********************************************************** */

#define	ALLOC_BC_U8(nchannels)                                          \
	mlib_u8 *dst_buf1, *dst_buf2;                                   \
	mlib_s32 offset = mlib_ImageGetLutOffset(colormap);             \
	mlib_u8 **table = (mlib_u8 **)mlib_ImageGetLutData(colormap);   \
	                                                                \
	dst_buf1 =                                                      \
	    (mlib_u8 *)__mlib_malloc((size + src_width +                \
	    4) * nchannels);                                            \
	if (dst_buf1 == NULL) {                                         \
	    __mlib_free(pdst_buf);                                      \
	    return (MLIB_FAILURE);                                      \
	}                                                               \
	dst_buf2 = dst_buf1 + (size + 2) * nchannels

/* *********************************************************** */

#define	ALLOC_BC_S16(nchannels)                                           \
	mlib_s16 *dst_buf1, *dst_buf2;                                    \
	mlib_s32 offset = mlib_ImageGetLutOffset(colormap);               \
	mlib_s16 **table = (mlib_s16 **)mlib_ImageGetLutData(colormap);   \
	                                                                  \
	dst_buf1 =                                                        \
	    (mlib_s16 *)__mlib_malloc((size + src_width +                 \
	    4) * 2 * nchannels);                                          \
	if (dst_buf1 == NULL) {                                           \
	    __mlib_free(pdst_buf);                                        \
	    return (MLIB_FAILURE);                                        \
	}                                                                 \
	dst_buf2 = dst_buf1 + (size + 2) * nchannels

/* *********************************************************** */

#define	FREE() __mlib_free(pdst_buf); __mlib_free(dst_buf1)

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_U8_0_1(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_u8 *pdst_buf;
	const mlib_u8 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_u8 *)__mlib_malloc((src_height + 2) * sizeof (mlib_u8) * 3);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_U8(3);
		MLIB_ZOOMIN2XINDEX_BC(mlib_u8, U8, U8, 3, 0,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_U8_0_2(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_u8 *pdst_buf;
	const mlib_u8 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_u8 *)__mlib_malloc((src_height + 2) * sizeof (mlib_u8) * 4);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_U8(4);
		MLIB_ZOOMIN2XINDEX_BC(mlib_u8, U8, U8, 4, 0,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_U8_0_3(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_u8 *pdst_buf;
	const mlib_u8 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_u8 *)__mlib_malloc((src_height + 2) * sizeof (mlib_u8) * 3);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_S16(3);
		MLIB_ZOOMIN2XINDEX_BC(mlib_s16, S16, U8, 3, 0,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_U8_0_4(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_u8 *pdst_buf;
	const mlib_u8 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_u8 *)__mlib_malloc((src_height + 2) * sizeof (mlib_u8) * 4);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_S16(4);
		MLIB_ZOOMIN2XINDEX_BC(mlib_s16, S16, U8, 4, 0,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_S16_0_1(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_s16 *pdst_buf;
	const mlib_s16 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_s16 *)__mlib_malloc((src_height + 2) * sizeof (mlib_s16) * 3);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_U8(3);
		MLIB_ZOOMIN2XINDEX_BC(mlib_u8, U8, S16, 3, 0,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_S16_0_2(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_s16 *pdst_buf;
	const mlib_s16 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_s16 *)__mlib_malloc((src_height + 2) * sizeof (mlib_s16) * 4);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_U8(4);
		MLIB_ZOOMIN2XINDEX_BC(mlib_u8, U8, S16, 4, 0,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_S16_0_3(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_s16 *pdst_buf;
	const mlib_s16 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_s16 *)__mlib_malloc((src_height + 2) * sizeof (mlib_s16) * 3);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_S16(3);
		MLIB_ZOOMIN2XINDEX_BC(mlib_s16, S16, S16, 3, 0,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_S16_0_4(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_s16 *pdst_buf;
	const mlib_s16 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_s16 *)__mlib_malloc((src_height + 2) * sizeof (mlib_s16) * 4);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_S16(4);
		MLIB_ZOOMIN2XINDEX_BC(mlib_s16, S16, S16, 4, 0,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_U8_1_1(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_u8 *pdst_buf;
	const mlib_u8 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_u8 *)__mlib_malloc((src_height + 2) * sizeof (mlib_u8) * 3);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_U8(3);
		MLIB_ZOOMIN2XINDEX_BC(mlib_u8, U8, U8, 3, 1,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_U8_1_2(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_u8 *pdst_buf;
	const mlib_u8 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_u8 *)__mlib_malloc((src_height + 2) * sizeof (mlib_u8) * 4);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_U8(4);
		MLIB_ZOOMIN2XINDEX_BC(mlib_u8, U8, U8, 4, 1,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_U8_1_3(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_u8 *pdst_buf;
	const mlib_u8 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_u8 *)__mlib_malloc((src_height + 2) * sizeof (mlib_u8) * 3);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_S16(3);
		MLIB_ZOOMIN2XINDEX_BC(mlib_s16, S16, U8, 3, 1,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_U8_1_4(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_u8 *pdst_buf;
	const mlib_u8 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_u8 *)__mlib_malloc((src_height + 2) * sizeof (mlib_u8) * 4);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_S16(4);
		MLIB_ZOOMIN2XINDEX_BC(mlib_s16, S16, U8, 4, 1,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_S16_1_1(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_s16 *pdst_buf;
	const mlib_s16 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_s16 *)__mlib_malloc((src_height + 2) * sizeof (mlib_s16) * 3);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_U8(3);
		MLIB_ZOOMIN2XINDEX_BC(mlib_u8, U8, S16, 3, 1,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_S16_1_2(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_s16 *pdst_buf;
	const mlib_s16 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_s16 *)__mlib_malloc((src_height + 2) * sizeof (mlib_s16) * 4);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_U8(4);
		MLIB_ZOOMIN2XINDEX_BC(mlib_u8, U8, S16, 4, 1,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_S16_1_3(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_s16 *pdst_buf;
	const mlib_s16 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_s16 *)__mlib_malloc((src_height + 2) * sizeof (mlib_s16) * 3);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_S16(3);
		MLIB_ZOOMIN2XINDEX_BC(mlib_s16, S16, S16, 3, 1,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_S16_1_4(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_s16 *pdst_buf;
	const mlib_s16 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_s16 *)__mlib_malloc((src_height + 2) * sizeof (mlib_s16) * 4);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_S16(4);
		MLIB_ZOOMIN2XINDEX_BC(mlib_s16, S16, S16, 4, 1,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_U8_2_1(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_u8 *pdst_buf;
	const mlib_u8 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_u8 *)__mlib_malloc((src_height + 2) * sizeof (mlib_u8) * 3);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_U8(3);
		MLIB_ZOOMIN2XINDEX_BC(mlib_u8, U8, U8, 3, 2,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_U8_2_2(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_u8 *pdst_buf;
	const mlib_u8 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_u8 *)__mlib_malloc((src_height + 2) * sizeof (mlib_u8) * 4);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_U8(4);
		MLIB_ZOOMIN2XINDEX_BC(mlib_u8, U8, U8, 4, 2,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_U8_2_3(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_u8 *pdst_buf;
	const mlib_u8 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_u8 *)__mlib_malloc((src_height + 2) * sizeof (mlib_u8) * 3);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_S16(3);
		MLIB_ZOOMIN2XINDEX_BC(mlib_s16, S16, U8, 3, 2,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_U8_2_4(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_u8 *pdst_buf;
	const mlib_u8 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_u8 *)__mlib_malloc((src_height + 2) * sizeof (mlib_u8) * 4);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_S16(4);
		MLIB_ZOOMIN2XINDEX_BC(mlib_s16, S16, U8, 4, 2,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_S16_2_1(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_s16 *pdst_buf;
	const mlib_s16 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_s16 *)__mlib_malloc((src_height + 2) * sizeof (mlib_s16) * 3);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_U8(3);
		MLIB_ZOOMIN2XINDEX_BC(mlib_u8, U8, S16, 3, 2,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_S16_2_2(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_s16 *pdst_buf;
	const mlib_s16 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_s16 *)__mlib_malloc((src_height + 2) * sizeof (mlib_s16) * 4);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_U8(4);
		MLIB_ZOOMIN2XINDEX_BC(mlib_u8, U8, S16, 4, 2,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_S16_2_3(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_s16 *pdst_buf;
	const mlib_s16 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_s16 *)__mlib_malloc((src_height + 2) * sizeof (mlib_s16) * 3);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_S16(3);
		MLIB_ZOOMIN2XINDEX_BC(mlib_s16, S16, S16, 3, 2,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_S16_2_4(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_s16 *pdst_buf;
	const mlib_s16 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_s16 *)__mlib_malloc((src_height + 2) * sizeof (mlib_s16) * 4);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_S16(4);
		MLIB_ZOOMIN2XINDEX_BC(mlib_s16, S16, S16, 4, 2,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_U8_3_1(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_u8 *pdst_buf;
	const mlib_u8 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_u8 *)__mlib_malloc((src_height + 2) * sizeof (mlib_u8) * 3);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_U8(3);
		MLIB_ZOOMIN2XINDEX_BC(mlib_u8, U8, U8, 3, 3,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_U8_3_2(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_u8 *pdst_buf;
	const mlib_u8 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_u8 *)__mlib_malloc((src_height + 2) * sizeof (mlib_u8) * 4);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_U8(4);
		MLIB_ZOOMIN2XINDEX_BC(mlib_u8, U8, U8, 4, 3,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_U8_3_3(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_u8 *pdst_buf;
	const mlib_u8 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_u8 *)__mlib_malloc((src_height + 2) * sizeof (mlib_u8) * 3);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_S16(3);
		MLIB_ZOOMIN2XINDEX_BC(mlib_s16, S16, U8, 3, 3,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_U8_3_4(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_u8 *pdst_buf;
	const mlib_u8 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_u8 *)__mlib_malloc((src_height + 2) * sizeof (mlib_u8) * 4);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_S16(4);
		MLIB_ZOOMIN2XINDEX_BC(mlib_s16, S16, U8, 4, 3,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_S16_3_1(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_s16 *pdst_buf;
	const mlib_s16 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_s16 *)__mlib_malloc((src_height + 2) * sizeof (mlib_s16) * 3);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_U8(3);
		MLIB_ZOOMIN2XINDEX_BC(mlib_u8, U8, S16, 3, 3,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_S16_3_2(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_s16 *pdst_buf;
	const mlib_s16 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_s16 *)__mlib_malloc((src_height + 2) * sizeof (mlib_s16) * 4);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_U8(4);
		MLIB_ZOOMIN2XINDEX_BC(mlib_u8, U8, S16, 4, 3,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_S16_3_3(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_s16 *pdst_buf;
	const mlib_s16 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_s16 *)__mlib_malloc((src_height + 2) * sizeof (mlib_s16) * 3);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_S16(3);
		MLIB_ZOOMIN2XINDEX_BC(mlib_s16, S16, S16, 3, 3,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_BC2_S16_3_4(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	mlib_s32 size = (src_height > src_width ? src_height : src_width);
	mlib_s16 *pdst_buf;
	const mlib_s16 *p_1, *p, *p1, *p2;

	pdst_buf =
	    (mlib_s16 *)__mlib_malloc((src_height + 2) * sizeof (mlib_s16) * 4);

	if (pdst_buf == NULL)
		return (MLIB_FAILURE);

	{
		ALLOC_BC_S16(4);
		MLIB_ZOOMIN2XINDEX_BC(mlib_s16, S16, S16, 4, 3,
			mlib_ImageColorTrue2IndexLine_);
		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MLIB_IMAGEZOOMIN2XINDEX_BC(dst_format, dst_type, SHIFT)           \
	mlib_s32 lut_type = mlib_ImageGetLutType(colormap);               \
	mlib_s32 nchannels = mlib_ImageGetLutChannels(colormap);          \
	                                                                  \
	if ((nchannels != 3) && (nchannels != 4))                         \
	    return (MLIB_FAILURE);                                        \
	switch (lut_type) {                                               \
	case MLIB_BYTE: {                                                 \
		switch (nchannels) {                                      \
		case 3:                                                   \
		    return                                                \
			MLIB_ZOOMIN2XINDEX_BC2_##dst_type##_##SHIFT##_1   \
			(pdst, psrc, src_width, src_height, dst_stride,   \
			src_stride, src_w_beg, src_h_beg, colormap);      \
		case 4:                                                   \
		    return                                                \
			MLIB_ZOOMIN2XINDEX_BC2_##dst_type##_##SHIFT##_2   \
			(pdst, psrc, src_width, src_height, dst_stride,   \
			src_stride, src_w_beg, src_h_beg, colormap);      \
		}                                                         \
		break;                                                    \
	    }                                                             \
	case MLIB_SHORT: {                                                \
		switch (nchannels) {                                      \
		case 3:                                                   \
		    return                                                \
			MLIB_ZOOMIN2XINDEX_BC2_##dst_type##_##SHIFT##_3   \
			(pdst, psrc, src_width, src_height, dst_stride,   \
			src_stride, src_w_beg, src_h_beg, colormap);      \
		case 4:                                                   \
		    return                                                \
			MLIB_ZOOMIN2XINDEX_BC2_##dst_type##_##SHIFT##_4   \
			(pdst, psrc, src_width, src_height, dst_stride,   \
			src_stride, src_w_beg, src_h_beg, colormap);      \
		}                                                         \
		break;                                                    \
	    }                                                             \
	}                                                                 \
	                                                                  \
	return MLIB_SUCCESS

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomIn2XIndex_U8_BC2_FL0(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	MLIB_IMAGEZOOMIN2XINDEX_BC(mlib_u8,
	    U8,
	    0);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomIn2XIndex_U8_BC2_FL1(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	MLIB_IMAGEZOOMIN2XINDEX_BC(mlib_u8,
	    U8,
	    1);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomIn2XIndex_U8_BC2_FL2(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	MLIB_IMAGEZOOMIN2XINDEX_BC(mlib_u8,
	    U8,
	    2);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomIn2XIndex_U8_BC2_FL3(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_u8))
{
	MLIB_IMAGEZOOMIN2XINDEX_BC(mlib_u8,
	    U8,
	    3);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomIn2XIndex_S16_BC2_FL0(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	MLIB_IMAGEZOOMIN2XINDEX_BC(mlib_s16,
	    S16,
	    0);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomIn2XIndex_S16_BC2_FL1(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	MLIB_IMAGEZOOMIN2XINDEX_BC(mlib_s16,
	    S16,
	    1);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomIn2XIndex_S16_BC2_FL2(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	MLIB_IMAGEZOOMIN2XINDEX_BC(mlib_s16,
	    S16,
	    2);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageZoomIn2XIndex_S16_BC2_FL3(
    MLIB_ZOOMIN2XINDEXDEFPARAM(mlib_s16))
{
	MLIB_IMAGEZOOMIN2XINDEX_BC(mlib_s16,
	    S16,
	    3);
}

/* *********************************************************** */
