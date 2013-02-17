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

#pragma ident	"@(#)mlib_ImageZoomIn2XIndex_BC_Edge.c	9.2	07/10/09 SMI"

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

#include <mlib_image.h>
#include <mlib_ImageColormap.h>
#include <mlib_ImageZoomIn2XIndex.h>

/* *********************************************************** */

#define	MLIB_ZOOMIN2XINDEXDEFEDGE_CE(MLIB_TYPE)                      \
	MLIB_TYPE *pdst,                                             \
	mlib_s32 src_width,                                          \
	mlib_s32 src_height,                                         \
	mlib_s32 dst_stride,                                         \
	MLIB_TYPE *pdst_near,                                        \
	mlib_s32 src_width_near,                                     \
	mlib_s32 src_height_near,                                    \
	const void *colormap

/* *********************************************************** */

#define	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(MLIB_TYPE)                       \
	MLIB_ZOOMIN2XINDEXDEFEDGE_CE(MLIB_TYPE),                      \
	mlib_s32 src_stride,                                          \
	const MLIB_TYPE *psrc_beg,                                    \
	mlib_s32 src_width_beg,                                       \
	mlib_s32 src_height_beg,                                      \
	mlib_s32 src_w_beg_near,                                      \
	mlib_s32 src_h_beg_near

/* *********************************************************** */

static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_U8_0_1(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_U8_0_2(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_U8_0_3(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_U8_0_4(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_S16_0_1(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_S16_0_2(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_S16_0_3(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_S16_0_4(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_U8_1_1(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_U8_1_2(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_U8_1_3(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_U8_1_4(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_S16_1_1(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_S16_1_2(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_S16_1_3(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_S16_1_4(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_U8_2_1(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_U8_2_2(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_U8_2_3(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_U8_2_4(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_S16_2_1(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_S16_2_2(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_S16_2_3(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_S16_2_4(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_U8_3_1(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_U8_3_2(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_U8_3_3(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_U8_3_4(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_S16_3_1(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_S16_3_2(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_S16_3_3(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16));
static mlib_status MLIB_ZOOMIN2XINDEX_SE_BC_S16_3_4(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16));

/* *********************************************************** */

#define	FREE() __mlib_free(pdst_buf1); __mlib_free(pdst_buf)

/* *********************************************************** */

#define	SAT_U8()                                                \
	imask = (rez >> 5) & 0x18;                              \
	rez = (rez & (mask1 >> imask)) | (mask2 >> imask)

/* *********************************************************** */

#define	SAT_S16()                                               \
	mask = *((mlib_u32 *)(pmask + ((rez >> 13) & 0xc)));    \
	rez = (rez & mask) | (mask >> 16)

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

#define	PREPARE_BICUBIC(beg_i, j)                                       \
	mlib_s32 dh = (src_h_beg_near - 1 + (j)) & 1,                   \
		hx = (src_h_beg_near - 1 + (j)) >> 1,                   \
		hx_1 = hx - 1,                                          \
		hx1 = hx + 1,                                           \
		hx2 = hx + 2,                                           \
		dw = (src_w_beg_near - 1 + (beg_i)) & 1,                \
		wx = (src_w_beg_near - 1 + (beg_i)) >> 1,               \
		wx_1 = wx - 1,                                          \
		wx1 = wx + 1,                                           \
		wx2 = wx + 2;                                           \
	mlib_s32 a_1, a, a1, a2 = 0, l, rez, i;                         \
	                                                                \
	if (hx < 0)                                                     \
	    hx = 0;                                                     \
	if (hx_1 < 0)                                                   \
	    hx_1 = 0;                                                   \
	if (hx1 < 0)                                                    \
	    hx1 = 0;                                                    \
	if (hx2 < 0)                                                    \
	    hx2 = 0;                                                    \
	if (hx >= src_height_beg)                                       \
	    hx = src_height_beg - 1;                                    \
	if (hx_1 >= src_height_beg)                                     \
	    hx_1 = src_height_beg - 1;                                  \
	if (hx1 >= src_height_beg)                                      \
	    hx1 = src_height_beg - 1;                                   \
	if (hx2 >= src_height_beg)                                      \
	    hx2 = src_height_beg - 1;                                   \
	hx_1 *= src_stride;                                             \
	hx *= src_stride;                                               \
	hx1 *= src_stride;                                              \
	hx2 *= src_stride;                                              \
	if (wx < 0)                                                     \
	    wx = 0;                                                     \
	if (wx_1 < 0)                                                   \
	    wx_1 = 0;                                                   \
	if (wx1 < 0)                                                    \
	    wx1 = 0;                                                    \
	if (wx2 < 0)                                                    \
	    wx2 = 0;                                                    \
	if (wx >= src_width_beg)                                        \
	    wx = src_width_beg - 1;                                     \
	if (wx_1 >= src_width_beg)                                      \
	    wx_1 = src_width_beg - 1;                                   \
	if (wx1 >= src_width_beg)                                       \
	    wx1 = src_width_beg - 1;                                    \
	if (wx2 >= src_width_beg)                                       \
	    wx2 = src_width_beg - 1

/* *********************************************************** */

#define	ROW_BICUBIC_FL0(beg_i, j, chan, amount, src_type)                 \
	{                                                                 \
	    PREPARE_BICUBIC(beg_i, j);                                    \
	    psrc_end = psrc_beg + hx + src_width_beg;                     \
	    if (dh == 0) {                                                \
		for (l = 0; l < chan; l++) {                              \
		    count = 0;                                            \
		    pcolor = table[l] - offset;                           \
		    a_1 = (111 * pcolor[psrc_beg[hx + wx_1]] +            \
			    29 * pcolor[psrc_beg[hx1 + wx_1]] -           \
			    9 * pcolor[psrc_beg[hx_1 + wx_1]] -           \
			    3 * pcolor[psrc_beg[hx2 + wx_1]]);            \
		    a = (111 * pcolor[psrc_beg[hx + wx]] +                \
			    29 * pcolor[psrc_beg[hx1 + wx]] -             \
			    9 * pcolor[psrc_beg[hx_1 + wx]] -             \
			    3 * pcolor[psrc_beg[hx2 + wx]]);              \
		    a1 = (111 * pcolor[psrc_beg[hx + wx1]] +              \
			    29 * pcolor[psrc_beg[hx1 + wx1]] -            \
			    9 * pcolor[psrc_beg[hx_1 + wx1]] -            \
			    3 * pcolor[psrc_beg[hx2 + wx1]]);             \
		    p_1 = psrc_beg + hx_1 + wx2;                          \
		    p = psrc_beg + hx + wx2;                              \
		    p1 = psrc_beg + hx1 + wx2;                            \
		    p2 = psrc_beg + hx2 + wx2;                            \
		    i = (beg_i);                                          \
		    if (dw == 1) {                                        \
			a2 = 111 * pcolor[(*p)] + 29 * pcolor[(*p1)] -    \
				9 * pcolor[(*p_1)] - 3 * pcolor[(*p2)];   \
			rez = (29 * a + 111 * a1 - 3 * a_1 -              \
				9 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			count++;                                          \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			p_1++;                                            \
			p++;                                              \
			p1++;                                             \
			p2++;                                             \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 4; i += 2) {                    \
			a2 = 111 * pcolor[(*p)] + 29 * pcolor[(*p1)] -    \
				9 * pcolor[(*p_1)] - 3 * pcolor[(*p2)];   \
			rez = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			rez = (29 * a + 111 * a1 - 3 * a_1 -              \
				9 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * (count + 1) + l] = rez;           \
			count += 2;                                       \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			p_1++;                                            \
			p++;                                              \
			p1++;                                             \
			p2++;                                             \
		    }                                                     \
		    if (i < (amount) - 2) {                               \
			if (p >= psrc_end) {                              \
			    p_1--;                                        \
			    p--;                                          \
			    p1--;                                         \
			    p2--;                                         \
			}                                                 \
			a2 = 111 * pcolor[(*p)] + 29 * pcolor[(*p1)] -    \
				9 * pcolor[(*p_1)] - 3 * pcolor[(*p2)];   \
			rez = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			rez = (29 * a + 111 * a1 - 3 * a_1 -              \
				9 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * (count + 1) + l] = rez;           \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			p_1++;                                            \
			p++;                                              \
			p1++;                                             \
			p2++;                                             \
			i += 2;                                           \
			count += 2;                                       \
		    }                                                     \
		    if (i < (amount)) {                                   \
			if (p >= psrc_end) {                              \
			    p_1--;                                        \
			    p--;                                          \
			    p1--;                                         \
			    p2--;                                         \
			}                                                 \
			a2 = 111 * pcolor[(*p)] + 29 * pcolor[(*p1)] -    \
				9 * pcolor[(*p_1)] - 3 * pcolor[(*p2)];   \
			rez = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			count++;                                          \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (29 * a + 111 * a1 - 3 * a_1 -              \
				9 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			count++;                                          \
		    }                                                     \
		}                                                         \
	    } else {                                                      \
		for (l = 0; l < chan; l++) {                              \
		    count = 0;                                            \
		    pcolor = table[l] - offset;                           \
		    a_1 = (29 * pcolor[psrc_beg[hx + wx_1]] +             \
			    111 * pcolor[psrc_beg[hx1 + wx_1]] -          \
			    3 * pcolor[psrc_beg[hx_1 + wx_1]] -           \
			    9 * pcolor[psrc_beg[hx2 + wx_1]]);            \
		    a = (29 * pcolor[psrc_beg[hx + wx]] +                 \
			    111 * pcolor[psrc_beg[hx1 + wx]] -            \
			    3 * pcolor[psrc_beg[hx_1 + wx]] -             \
			    9 * pcolor[psrc_beg[hx2 + wx]]);              \
		    a1 = (29 * pcolor[psrc_beg[hx + wx1]] +               \
			    111 * pcolor[psrc_beg[hx1 + wx1]] -           \
			    3 * pcolor[psrc_beg[hx_1 + wx1]] -            \
			    9 * pcolor[psrc_beg[hx2 + wx1]]);             \
		    p_1 = psrc_beg + hx_1 + wx2;                          \
		    p = psrc_beg + hx + wx2;                              \
		    p1 = psrc_beg + hx1 + wx2;                            \
		    p2 = psrc_beg + hx2 + wx2;                            \
		    i = (beg_i);                                          \
		    if (dw == 1) {                                        \
			a2 = 29 * pcolor[(*p)] + 111 * pcolor[(*p1)] -    \
				3 * pcolor[(*p_1)] - 9 * pcolor[(*p2)];   \
			rez = (29 * a + 111 * a1 - 3 * a_1 -              \
				9 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			count++;                                          \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			p_1++;                                            \
			p++;                                              \
			p1++;                                             \
			p2++;                                             \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 4; i += 2) {                    \
			a2 = 29 * pcolor[(*p)] + 111 * pcolor[(*p1)] -    \
				3 * pcolor[(*p_1)] - 9 * pcolor[(*p2)];   \
			rez = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			rez = (29 * a + 111 * a1 - 3 * a_1 -              \
				9 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * (count + 1) + l] = rez;           \
			count += 2;                                       \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			p_1++;                                            \
			p++;                                              \
			p1++;                                             \
			p2++;                                             \
		    }                                                     \
		    if (i < (amount) - 2) {                               \
			if (p >= psrc_end) {                              \
			    p_1--;                                        \
			    p--;                                          \
			    p1--;                                         \
			    p2--;                                         \
			}                                                 \
			a2 = 29 * pcolor[(*p)] + 111 * pcolor[(*p1)] -    \
				3 * pcolor[(*p_1)] - 9 * pcolor[(*p2)];   \
			rez = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			rez = (29 * a + 111 * a1 - 3 * a_1 -              \
				9 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * (count + 1) + l] = rez;           \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			p_1++;                                            \
			p++;                                              \
			p1++;                                             \
			p2++;                                             \
			i += 2;                                           \
			count += 2;                                       \
		    }                                                     \
		    if (i < (amount)) {                                   \
			if (p >= psrc_end) {                              \
			    p_1--;                                        \
			    p--;                                          \
			    p1--;                                         \
			    p2--;                                         \
			}                                                 \
			a2 = 29 * pcolor[(*p)] + 111 * pcolor[(*p1)] -    \
				3 * pcolor[(*p_1)] - 9 * pcolor[(*p2)];   \
			rez = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			count++;                                          \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (29 * a + 111 * a1 - 3 * a_1 -              \
				9 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			count++;                                          \
		    }                                                     \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC_FL0(beg_i, j, chan, amount, src_type)              \
	{                                                                 \
	    PREPARE_BICUBIC(beg_i, j);                                    \
	    psrc_end = psrc_beg + wx + src_height_beg * src_stride;       \
	    if (dw == 0) {                                                \
		for (l = 0; l < chan; l++) {                              \
		    count = 0;                                            \
		    pcolor = table[l] - offset;                           \
		    a_1 = (111 * pcolor[psrc_beg[hx_1 + wx]] +            \
			    29 * pcolor[psrc_beg[hx_1 + wx1]] -           \
			    9 * pcolor[psrc_beg[hx_1 + wx_1]] -           \
			    3 * pcolor[psrc_beg[hx_1 + wx2]]);            \
		    a = (111 * pcolor[psrc_beg[hx + wx]] +                \
			    29 * pcolor[psrc_beg[hx + wx1]] -             \
			    9 * pcolor[psrc_beg[hx + wx_1]] -             \
			    3 * pcolor[psrc_beg[hx + wx2]]);              \
		    a1 = (111 * pcolor[psrc_beg[hx1 + wx]] +              \
			    29 * pcolor[psrc_beg[hx1 + wx1]] -            \
			    9 * pcolor[psrc_beg[hx1 + wx_1]] -            \
			    3 * pcolor[psrc_beg[hx1 + wx2]]);             \
		    p_1 = psrc_beg + hx2 + wx_1;                          \
		    p = psrc_beg + hx2 + wx;                              \
		    p1 = psrc_beg + hx2 + wx1;                            \
		    p2 = psrc_beg + hx2 + wx2;                            \
		    i = (j);                                              \
		    if (dh == 1) {                                        \
			a2 = 111 * pcolor[(*p)] + 29 * pcolor[(*p1)] -    \
				9 * pcolor[(*p_1)] - 3 * pcolor[(*p2)];   \
			rez = (29 * a + 111 * a1 - 3 * a_1 -              \
				9 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			count++;                                          \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			p_1 += src_stride;                                \
			p += src_stride;                                  \
			p1 += src_stride;                                 \
			p2 += src_stride;                                 \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 4; i += 2) {                    \
			a2 = 111 * pcolor[(*p)] + 29 * pcolor[(*p1)] -    \
				9 * pcolor[(*p_1)] - 3 * pcolor[(*p2)];   \
			rez = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			rez = (29 * a + 111 * a1 - 3 * a_1 -              \
				9 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * (count + 1) + l] = rez;           \
			count += 2;                                       \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			p_1 += src_stride;                                \
			p += src_stride;                                  \
			p1 += src_stride;                                 \
			p2 += src_stride;                                 \
		    }                                                     \
		    if (i < (amount) - 2) {                               \
			if (p >= psrc_end) {                              \
			    p_1 -= src_stride;                            \
			    p -= src_stride;                              \
			    p1 -= src_stride;                             \
			    p2 -= src_stride;                             \
			}                                                 \
			a2 = 111 * pcolor[(*p)] + 29 * pcolor[(*p1)] -    \
				9 * pcolor[(*p_1)] - 3 * pcolor[(*p2)];   \
			rez = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			rez = (29 * a + 111 * a1 - 3 * a_1 -              \
				9 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * (count + 1) + l] = rez;           \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			p_1 += src_stride;                                \
			p += src_stride;                                  \
			p1 += src_stride;                                 \
			p2 += src_stride;                                 \
			i += 2;                                           \
			count += 2;                                       \
		    }                                                     \
		    if (i < (amount)) {                                   \
			if (p >= psrc_end) {                              \
			    p_1 -= src_stride;                            \
			    p -= src_stride;                              \
			    p1 -= src_stride;                             \
			    p2 -= src_stride;                             \
			}                                                 \
			a2 = 111 * pcolor[(*p)] + 29 * pcolor[(*p1)] -    \
				9 * pcolor[(*p_1)] - 3 * pcolor[(*p2)];   \
			rez = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			count++;                                          \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (29 * a + 111 * a1 - 3 * a_1 -              \
				9 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			count++;                                          \
		    }                                                     \
		}                                                         \
	    } else {                                                      \
		for (l = 0; l < chan; l++) {                              \
		    count = 0;                                            \
		    pcolor = table[l] - offset;                           \
		    a_1 = (29 * pcolor[psrc_beg[hx_1 + wx]] +             \
			    111 * pcolor[psrc_beg[hx_1 + wx1]] -          \
			    3 * pcolor[psrc_beg[hx_1 + wx_1]] -           \
			    9 * pcolor[psrc_beg[hx_1 + wx2]]);            \
		    a = (29 * pcolor[psrc_beg[hx + wx]] +                 \
			    111 * pcolor[psrc_beg[hx + wx1]] -            \
			    3 * pcolor[psrc_beg[hx + wx_1]] -             \
			    9 * pcolor[psrc_beg[hx + wx2]]);              \
		    a1 = (29 * pcolor[psrc_beg[hx1 + wx]] +               \
			    111 * pcolor[psrc_beg[hx1 + wx1]] -           \
			    3 * pcolor[psrc_beg[hx1 + wx_1]] -            \
			    9 * pcolor[psrc_beg[hx1 + wx2]]);             \
		    p_1 = psrc_beg + hx2 + wx_1;                          \
		    p = psrc_beg + hx2 + wx;                              \
		    p1 = psrc_beg + hx2 + wx1;                            \
		    p2 = psrc_beg + hx2 + wx2;                            \
		    i = (j);                                              \
		    if (dh == 1) {                                        \
			a2 = 29 * pcolor[(*p)] + 111 * pcolor[(*p1)] -    \
				3 * pcolor[(*p_1)] - 9 * pcolor[(*p2)];   \
			rez = (29 * a + 111 * a1 - 3 * a_1 -              \
				9 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			count++;                                          \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			p_1 += src_stride;                                \
			p += src_stride;                                  \
			p1 += src_stride;                                 \
			p2 += src_stride;                                 \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 4; i += 2) {                    \
			a2 = 29 * pcolor[(*p)] + 111 * pcolor[(*p1)] -    \
				3 * pcolor[(*p_1)] - 9 * pcolor[(*p2)];   \
			rez = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			rez = (29 * a + 111 * a1 - 3 * a_1 -              \
				9 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * (count + 1) + l] = rez;           \
			count += 2;                                       \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			p_1 += src_stride;                                \
			p += src_stride;                                  \
			p1 += src_stride;                                 \
			p2 += src_stride;                                 \
		    }                                                     \
		    if (i < (amount) - 2) {                               \
			if (p >= psrc_end) {                              \
			    p_1 -= src_stride;                            \
			    p -= src_stride;                              \
			    p1 -= src_stride;                             \
			    p2 -= src_stride;                             \
			}                                                 \
			a2 = 29 * pcolor[(*p)] + 111 * pcolor[(*p1)] -    \
				3 * pcolor[(*p_1)] - 9 * pcolor[(*p2)];   \
			rez = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			rez = (29 * a + 111 * a1 - 3 * a_1 -              \
				9 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * (count + 1) + l] = rez;           \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			p_1 += src_stride;                                \
			p += src_stride;                                  \
			p1 += src_stride;                                 \
			p2 += src_stride;                                 \
			i += 2;                                           \
			count += 2;                                       \
		    }                                                     \
		    if (i < (amount)) {                                   \
			if (p >= psrc_end) {                              \
			    p_1 -= src_stride;                            \
			    p -= src_stride;                              \
			    p1 -= src_stride;                             \
			    p2 -= src_stride;                             \
			}                                                 \
			a2 = 29 * pcolor[(*p)] + 111 * pcolor[(*p1)] -    \
				3 * pcolor[(*p_1)] - 9 * pcolor[(*p2)];   \
			rez = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			count++;                                          \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (29 * a + 111 * a1 - 3 * a_1 -              \
				9 * a2) >> 14;                            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			count++;                                          \
		    }                                                     \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

#define	ROW_BICUBIC_FL1(beg_i, j, chan, amount, src_type)               \
	{                                                               \
	    PREPARE_BICUBIC(beg_i, j);                                  \
	    psrc_end = psrc_beg + hx + src_width_beg;                   \
	    if (dh == 0) {                                              \
		for (l = 0; l < chan; l++) {                            \
		    count = 0;                                          \
		    pcolor = table[l] - offset;                         \
		    a_1 = pcolor[psrc_beg[hx + wx_1]];                  \
		    a = pcolor[psrc_beg[hx + wx]];                      \
		    a1 = pcolor[psrc_beg[hx + wx1]];                    \
		    p = psrc_beg + hx + wx2;                            \
		    i = (beg_i);                                        \
		    if (dw == 1) {                                      \
			a2 = pcolor[(*p)];                              \
			rez = (29 * a + 111 * a1 - 3 * a_1 -            \
				9 * a2) >> 7;                           \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			count++;                                        \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			p++;                                            \
			i++;                                            \
		    }                                                   \
		    for (; i < (amount) - 4; i += 2) {                  \
			a2 = pcolor[(*p)];                              \
			rez = (111 * a + 29 * a1 - 9 * a_1 -            \
				3 * a2) >> 7;                           \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			rez = (29 * a + 111 * a1 - 3 * a_1 -            \
				9 * a2) >> 7;                           \
			SAT_##src_type();                               \
			pdst_buf[chan * (count + 1) + l] = rez;         \
			count += 2;                                     \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			p++;                                            \
		    }                                                   \
		    if (i < (amount) - 2) {                             \
			if (p >= psrc_end) {                            \
			    p--;                                        \
			}                                               \
			a2 = pcolor[(*p)];                              \
			rez = (111 * a + 29 * a1 - 9 * a_1 -            \
				3 * a2) >> 7;                           \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			rez = (29 * a + 111 * a1 - 3 * a_1 -            \
				9 * a2) >> 7;                           \
			SAT_##src_type();                               \
			pdst_buf[chan * (count + 1) + l] = rez;         \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			p++;                                            \
			i += 2;                                         \
			count += 2;                                     \
		    }                                                   \
		    if (i < (amount)) {                                 \
			if (p >= psrc_end) {                            \
			    p--;                                        \
			}                                               \
			a2 = pcolor[(*p)];                              \
			rez = (111 * a + 29 * a1 - 9 * a_1 -            \
				3 * a2) >> 7;                           \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			count++;                                        \
			i++;                                            \
		    }                                                   \
		    if (i < (amount)) {                                 \
			rez = (29 * a + 111 * a1 - 3 * a_1 -            \
				9 * a2) >> 7;                           \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			count++;                                        \
		    }                                                   \
		}                                                       \
	    } else {                                                    \
		for (l = 0; l < chan; l++) {                            \
		    count = 0;                                          \
		    pcolor = table[l] - offset;                         \
		    a_1 = (9 * (pcolor[psrc_beg[hx + wx_1]] +           \
			    (mlib_s32)pcolor[psrc_beg[hx1 + wx_1]]) -   \
			    pcolor[psrc_beg[hx_1 + wx_1]] -             \
			    pcolor[psrc_beg[hx2 + wx_1]]);              \
		    a = (9 * (pcolor[psrc_beg[hx + wx]] +               \
			    (mlib_s32)pcolor[psrc_beg[hx1 + wx]]) -     \
			    pcolor[psrc_beg[hx_1 + wx]] -               \
			    pcolor[psrc_beg[hx2 + wx]]);                \
		    a1 = (9 * (pcolor[psrc_beg[hx + wx1]] +             \
			    (mlib_s32)pcolor[psrc_beg[hx1 + wx1]]) -    \
			    pcolor[psrc_beg[hx_1 + wx1]] -              \
			    pcolor[psrc_beg[hx2 + wx1]]);               \
		    p_1 = psrc_beg + hx_1 + wx2;                        \
		    p = psrc_beg + hx + wx2;                            \
		    p1 = psrc_beg + hx1 + wx2;                          \
		    p2 = psrc_beg + hx2 + wx2;                          \
		    i = (beg_i);                                        \
		    if (dw == 1) {                                      \
			a2 = 9 * (pcolor[(*p)] +                        \
				(mlib_s32)pcolor[(*p1)]) -              \
				pcolor[(*p_1)] - pcolor[(*p2)];         \
			rez = (29 * a + 111 * a1 - 3 * a_1 -            \
				9 * a2) >> 11;                          \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			count++;                                        \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			p_1++;                                          \
			p++;                                            \
			p1++;                                           \
			p2++;                                           \
			i++;                                            \
		    }                                                   \
		    for (; i < (amount) - 4; i += 2) {                  \
			a2 = 9 * (pcolor[(*p)] +                        \
				(mlib_s32)pcolor[(*p1)]) -              \
				pcolor[(*p_1)] - pcolor[(*p2)];         \
			rez = (111 * a + 29 * a1 - 9 * a_1 -            \
				3 * a2) >> 11;                          \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			rez = (29 * a + 111 * a1 - 3 * a_1 -            \
				9 * a2) >> 11;                          \
			SAT_##src_type();                               \
			pdst_buf[chan * (count + 1) + l] = rez;         \
			count += 2;                                     \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			p_1++;                                          \
			p++;                                            \
			p1++;                                           \
			p2++;                                           \
		    }                                                   \
		    if (i < (amount) - 2) {                             \
			if (p >= psrc_end) {                            \
			    p_1--;                                      \
			    p--;                                        \
			    p1--;                                       \
			    p2--;                                       \
			}                                               \
			a2 = 9 * (pcolor[(*p)] +                        \
				(mlib_s32)pcolor[(*p1)]) -              \
				pcolor[(*p_1)] - pcolor[(*p2)];         \
			rez = (111 * a + 29 * a1 - 9 * a_1 -            \
				3 * a2) >> 11;                          \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			rez = (29 * a + 111 * a1 - 3 * a_1 -            \
				9 * a2) >> 11;                          \
			SAT_##src_type();                               \
			pdst_buf[chan * (count + 1) + l] = rez;         \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			p_1++;                                          \
			p++;                                            \
			p1++;                                           \
			p2++;                                           \
			i += 2;                                         \
			count += 2;                                     \
		    }                                                   \
		    if (i < (amount)) {                                 \
			if (p >= psrc_end) {                            \
			    p_1--;                                      \
			    p--;                                        \
			    p1--;                                       \
			    p2--;                                       \
			}                                               \
			a2 = 9 * (pcolor[(*p)] +                        \
				(mlib_s32)pcolor[(*p1)]) -              \
				pcolor[(*p_1)] - pcolor[(*p2)];         \
			rez = (111 * a + 29 * a1 - 9 * a_1 -            \
				3 * a2) >> 11;                          \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			count++;                                        \
			i++;                                            \
		    }                                                   \
		    if (i < (amount)) {                                 \
			rez = (29 * a + 111 * a1 - 3 * a_1 -            \
				9 * a2) >> 11;                          \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			count++;                                        \
		    }                                                   \
		}                                                       \
	    }                                                           \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC_FL1(beg_i, j, chan, amount, src_type)              \
	{                                                                 \
	    PREPARE_BICUBIC(beg_i, j);                                    \
	    psrc_end = psrc_beg + wx + src_height_beg * src_stride;       \
	    if (dw == 0) {                                                \
		for (l = 0; l < chan; l++) {                              \
		    count = 0;                                            \
		    pcolor = table[l] - offset;                           \
		    a_1 = (111 * pcolor[psrc_beg[hx_1 + wx]] +            \
			    29 * pcolor[psrc_beg[hx_1 + wx1]] -           \
			    9 * pcolor[psrc_beg[hx_1 + wx_1]] -           \
			    3 * pcolor[psrc_beg[hx_1 + wx2]]);            \
		    a = (111 * pcolor[psrc_beg[hx + wx]] +                \
			    29 * pcolor[psrc_beg[hx + wx1]] -             \
			    9 * pcolor[psrc_beg[hx + wx_1]] -             \
			    3 * pcolor[psrc_beg[hx + wx2]]);              \
		    a1 = (111 * pcolor[psrc_beg[hx1 + wx]] +              \
			    29 * pcolor[psrc_beg[hx1 + wx1]] -            \
			    9 * pcolor[psrc_beg[hx1 + wx_1]] -            \
			    3 * pcolor[psrc_beg[hx1 + wx2]]);             \
		    p_1 = psrc_beg + hx2 + wx_1;                          \
		    p = psrc_beg + hx2 + wx;                              \
		    p1 = psrc_beg + hx2 + wx1;                            \
		    p2 = psrc_beg + hx2 + wx2;                            \
		    i = (j);                                              \
		    if (dh == 0) {                                        \
			rez = a >> 7;                                     \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			count++;                                          \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = 111 * pcolor[(*p)] + 29 * pcolor[(*p1)] -    \
				9 * pcolor[(*p_1)] - 3 * pcolor[(*p2)];   \
			rez = (9 * (a + a1) - a_1 - a2) >> 11;            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			rez = a1 >> 7;                                    \
			SAT_##src_type();                                 \
			pdst_buf[chan * (count + 1) + l] = rez;           \
			count += 2;                                       \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			p_1 += src_stride;                                \
			p += src_stride;                                  \
			p1 += src_stride;                                 \
			p2 += src_stride;                                 \
		    }                                                     \
		    if (i < (amount)) {                                   \
			if (p >= psrc_end) {                              \
			    p_1 -= src_stride;                            \
			    p -= src_stride;                              \
			    p1 -= src_stride;                             \
			    p2 -= src_stride;                             \
			}                                                 \
			a2 = 111 * pcolor[(*p)] + 29 * pcolor[(*p1)] -    \
				9 * pcolor[(*p_1)] - 3 * pcolor[(*p2)];   \
			rez = (9 * (a + a1) - a_1 - a2) >> 11;            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			count++;                                          \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = a1 >> 7;                                    \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			count++;                                          \
		    }                                                     \
		}                                                         \
	    } else {                                                      \
		for (l = 0; l < chan; l++) {                              \
		    count = 0;                                            \
		    pcolor = table[l] - offset;                           \
		    a_1 = (29 * pcolor[psrc_beg[hx_1 + wx]] +             \
			    111 * pcolor[psrc_beg[hx_1 + wx1]] -          \
			    3 * pcolor[psrc_beg[hx_1 + wx_1]] -           \
			    9 * pcolor[psrc_beg[hx_1 + wx2]]);            \
		    a = (29 * pcolor[psrc_beg[hx + wx]] +                 \
			    111 * pcolor[psrc_beg[hx + wx1]] -            \
			    3 * pcolor[psrc_beg[hx + wx_1]] -             \
			    9 * pcolor[psrc_beg[hx + wx2]]);              \
		    a1 = (29 * pcolor[psrc_beg[hx1 + wx]] +               \
			    111 * pcolor[psrc_beg[hx1 + wx1]] -           \
			    3 * pcolor[psrc_beg[hx1 + wx_1]] -            \
			    9 * pcolor[psrc_beg[hx1 + wx2]]);             \
		    p_1 = psrc_beg + hx2 + wx_1;                          \
		    p = psrc_beg + hx2 + wx;                              \
		    p1 = psrc_beg + hx2 + wx1;                            \
		    p2 = psrc_beg + hx2 + wx2;                            \
		    i = (j);                                              \
		    if (dh == 0) {                                        \
			rez = a >> 7;                                     \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			count++;                                          \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = 29 * pcolor[(*p)] + 111 * pcolor[(*p1)] -    \
				3 * pcolor[(*p_1)] - 9 * pcolor[(*p2)];   \
			rez = (9 * (a + a1) - a_1 - a2) >> 11;            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			rez = a1 >> 7;                                    \
			SAT_##src_type();                                 \
			pdst_buf[chan * (count + 1) + l] = rez;           \
			count += 2;                                       \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			p_1 += src_stride;                                \
			p += src_stride;                                  \
			p1 += src_stride;                                 \
			p2 += src_stride;                                 \
		    }                                                     \
		    if (i < (amount)) {                                   \
			if (p >= psrc_end) {                              \
			    p_1 -= src_stride;                            \
			    p -= src_stride;                              \
			    p1 -= src_stride;                             \
			    p2 -= src_stride;                             \
			}                                                 \
			a2 = 29 * pcolor[(*p)] + 111 * pcolor[(*p1)] -    \
				3 * pcolor[(*p_1)] - 9 * pcolor[(*p2)];   \
			rez = (9 * (a + a1) - a_1 - a2) >> 11;            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			count++;                                          \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = a1 >> 7;                                    \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			count++;                                          \
		    }                                                     \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

#define	ROW_BICUBIC_FL2(beg_i, j, chan, amount, src_type)                 \
	{                                                                 \
	    PREPARE_BICUBIC(beg_i, j);                                    \
	    psrc_end = psrc_beg + hx + src_width_beg;                     \
	    if (dh == 0) {                                                \
		for (l = 0; l < chan; l++) {                              \
		    count = 0;                                            \
		    pcolor = table[l] - offset;                           \
		    a_1 = (111 * pcolor[psrc_beg[hx + wx_1]] +            \
			    29 * pcolor[psrc_beg[hx1 + wx_1]] -           \
			    9 * pcolor[psrc_beg[hx_1 + wx_1]] -           \
			    3 * pcolor[psrc_beg[hx2 + wx_1]]);            \
		    a = (111 * pcolor[psrc_beg[hx + wx]] +                \
			    29 * pcolor[psrc_beg[hx1 + wx]] -             \
			    9 * pcolor[psrc_beg[hx_1 + wx]] -             \
			    3 * pcolor[psrc_beg[hx2 + wx]]);              \
		    a1 = (111 * pcolor[psrc_beg[hx + wx1]] +              \
			    29 * pcolor[psrc_beg[hx1 + wx1]] -            \
			    9 * pcolor[psrc_beg[hx_1 + wx1]] -            \
			    3 * pcolor[psrc_beg[hx2 + wx1]]);             \
		    p_1 = psrc_beg + hx_1 + wx2;                          \
		    p = psrc_beg + hx + wx2;                              \
		    p1 = psrc_beg + hx1 + wx2;                            \
		    p2 = psrc_beg + hx2 + wx2;                            \
		    i = (beg_i);                                          \
		    if (dw == 0) {                                        \
			rez = a >> 7;                                     \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			count++;                                          \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = 111 * pcolor[(*p)] + 29 * pcolor[(*p1)] -    \
				9 * pcolor[(*p_1)] - 3 * pcolor[(*p2)];   \
			rez = (9 * (a + a1) - a_1 - a2) >> 11;            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			rez = a1 >> 7;                                    \
			SAT_##src_type();                                 \
			pdst_buf[chan * (count + 1) + l] = rez;           \
			count += 2;                                       \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			p_1++;                                            \
			p++;                                              \
			p1++;                                             \
			p2++;                                             \
		    }                                                     \
		    if (i < (amount)) {                                   \
			if (p >= psrc_end) {                              \
			    p_1--;                                        \
			    p--;                                          \
			    p1--;                                         \
			    p2--;                                         \
			}                                                 \
			a2 = 111 * pcolor[(*p)] + 29 * pcolor[(*p1)] -    \
				9 * pcolor[(*p_1)] - 3 * pcolor[(*p2)];   \
			rez = (9 * (a + a1) - a_1 - a2) >> 11;            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			count++;                                          \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = a1 >> 7;                                    \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			count++;                                          \
		    }                                                     \
		}                                                         \
	    } else {                                                      \
		for (l = 0; l < chan; l++) {                              \
		    count = 0;                                            \
		    pcolor = table[l] - offset;                           \
		    a_1 = (29 * pcolor[psrc_beg[hx + wx_1]] +             \
			    111 * pcolor[psrc_beg[hx1 + wx_1]] -          \
			    3 * pcolor[psrc_beg[hx_1 + wx_1]] -           \
			    9 * pcolor[psrc_beg[hx2 + wx_1]]);            \
		    a = (29 * pcolor[psrc_beg[hx + wx]] +                 \
			    111 * pcolor[psrc_beg[hx1 + wx]] -            \
			    3 * pcolor[psrc_beg[hx_1 + wx]] -             \
			    9 * pcolor[psrc_beg[hx2 + wx]]);              \
		    a1 = (29 * pcolor[psrc_beg[hx + wx1]] +               \
			    111 * pcolor[psrc_beg[hx1 + wx1]] -           \
			    3 * pcolor[psrc_beg[hx_1 + wx1]] -            \
			    9 * pcolor[psrc_beg[hx2 + wx1]]);             \
		    p_1 = psrc_beg + hx_1 + wx2;                          \
		    p = psrc_beg + hx + wx2;                              \
		    p1 = psrc_beg + hx1 + wx2;                            \
		    p2 = psrc_beg + hx2 + wx2;                            \
		    i = (beg_i);                                          \
		    if (dw == 0) {                                        \
			rez = a >> 7;                                     \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			count++;                                          \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = 29 * pcolor[(*p)] + 111 * pcolor[(*p1)] -    \
				3 * pcolor[(*p_1)] - 9 * pcolor[(*p2)];   \
			rez = (9 * (a + a1) - a_1 - a2) >> 11;            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			rez = a1 >> 7;                                    \
			SAT_##src_type();                                 \
			pdst_buf[chan * (count + 1) + l] = rez;           \
			count += 2;                                       \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			p_1++;                                            \
			p++;                                              \
			p1++;                                             \
			p2++;                                             \
		    }                                                     \
		    if (i < (amount)) {                                   \
			if (p >= psrc_end) {                              \
			    p_1--;                                        \
			    p--;                                          \
			    p1--;                                         \
			    p2--;                                         \
			}                                                 \
			a2 = 29 * pcolor[(*p)] + 111 * pcolor[(*p1)] -    \
				3 * pcolor[(*p_1)] - 9 * pcolor[(*p2)];   \
			rez = (9 * (a + a1) - a_1 - a2) >> 11;            \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			count++;                                          \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = a1 >> 7;                                    \
			SAT_##src_type();                                 \
			pdst_buf[chan * count + l] = rez;                 \
			count++;                                          \
		    }                                                     \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC_FL2(beg_i, j, chan, amount, src_type)            \
	{                                                               \
	    PREPARE_BICUBIC(beg_i, j);                                  \
	    psrc_end = psrc_beg + wx + src_height_beg * src_stride;     \
	    if (dw == 0) {                                              \
		for (l = 0; l < chan; l++) {                            \
		    count = 0;                                          \
		    pcolor = table[l] - offset;                         \
		    a_1 = pcolor[psrc_beg[hx_1 + wx]];                  \
		    a = pcolor[psrc_beg[hx + wx]];                      \
		    a1 = pcolor[psrc_beg[hx1 + wx]];                    \
		    p = psrc_beg + hx2 + wx;                            \
		    i = (j);                                            \
		    if (dh == 1) {                                      \
			a2 = pcolor[(*p)];                              \
			rez = (29 * a + 111 * a1 - 3 * a_1 -            \
				9 * a2) >> 7;                           \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			count++;                                        \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			p += src_stride;                                \
			i++;                                            \
		    }                                                   \
		    for (; i < (amount) - 4; i += 2) {                  \
			a2 = pcolor[(*p)];                              \
			rez = (111 * a + 29 * a1 - 9 * a_1 -            \
				3 * a2) >> 7;                           \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			rez = (29 * a + 111 * a1 - 3 * a_1 -            \
				9 * a2) >> 7;                           \
			SAT_##src_type();                               \
			pdst_buf[chan * (count + 1) + l] = rez;         \
			count += 2;                                     \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			p += src_stride;                                \
		    }                                                   \
		    if (i < (amount) - 2) {                             \
			if (p >= psrc_end) {                            \
			    p -= src_stride;                            \
			}                                               \
			a2 = pcolor[(*p)];                              \
			rez = (111 * a + 29 * a1 - 9 * a_1 -            \
				3 * a2) >> 7;                           \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			rez = (29 * a + 111 * a1 - 3 * a_1 -            \
				9 * a2) >> 7;                           \
			SAT_##src_type();                               \
			pdst_buf[chan * (count + 1) + l] = rez;         \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			p += src_stride;                                \
			i += 2;                                         \
			count += 2;                                     \
		    }                                                   \
		    if (i < (amount)) {                                 \
			if (p >= psrc_end) {                            \
			    p -= src_stride;                            \
			}                                               \
			a2 = pcolor[(*p)];                              \
			rez = (111 * a + 29 * a1 - 9 * a_1 -            \
				3 * a2) >> 7;                           \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			count++;                                        \
			i++;                                            \
		    }                                                   \
		    if (i < (amount)) {                                 \
			rez = (29 * a + 111 * a1 - 3 * a_1 -            \
				9 * a2) >> 7;                           \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			count++;                                        \
		    }                                                   \
		}                                                       \
	    } else {                                                    \
		for (l = 0; l < chan; l++) {                            \
		    count = 0;                                          \
		    pcolor = table[l] - offset;                         \
		    a_1 = (9 * (pcolor[psrc_beg[hx_1 + wx]] +           \
			    (mlib_s32)pcolor[psrc_beg[hx_1 + wx1]]) -   \
			    pcolor[psrc_beg[hx_1 + wx_1]] -             \
			    pcolor[psrc_beg[hx_1 + wx2]]);              \
		    a = (9 * (pcolor[psrc_beg[hx + wx]] +               \
			    (mlib_s32)pcolor[psrc_beg[hx + wx1]]) -     \
			    pcolor[psrc_beg[hx + wx_1]] -               \
			    pcolor[psrc_beg[hx + wx2]]);                \
		    a1 = (9 * (pcolor[psrc_beg[hx1 + wx]] +             \
			    (mlib_s32)pcolor[psrc_beg[hx1 + wx1]]) -    \
			    pcolor[psrc_beg[hx1 + wx_1]] -              \
			    pcolor[psrc_beg[hx1 + wx2]]);               \
		    p_1 = psrc_beg + hx2 + wx_1;                        \
		    p = psrc_beg + hx2 + wx;                            \
		    p1 = psrc_beg + hx2 + wx1;                          \
		    p2 = psrc_beg + hx2 + wx2;                          \
		    i = (j);                                            \
		    if (dh == 1) {                                      \
			a2 = 9 * (pcolor[(*p)] +                        \
				(mlib_s32)pcolor[(*p1)]) -              \
				pcolor[(*p_1)] - pcolor[(*p2)];         \
			rez = (29 * a + 111 * a1 - 3 * a_1 -            \
				9 * a2) >> 11;                          \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			count++;                                        \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			p_1 += src_stride;                              \
			p += src_stride;                                \
			p1 += src_stride;                               \
			p2 += src_stride;                               \
			i++;                                            \
		    }                                                   \
		    for (; i < (amount) - 4; i += 2) {                  \
			a2 = 9 * (pcolor[(*p)] +                        \
				(mlib_s32)pcolor[(*p1)]) -              \
				pcolor[(*p_1)] - pcolor[(*p2)];         \
			rez = (111 * a + 29 * a1 - 9 * a_1 -            \
				3 * a2) >> 11;                          \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			rez = (29 * a + 111 * a1 - 3 * a_1 -            \
				9 * a2) >> 11;                          \
			SAT_##src_type();                               \
			pdst_buf[chan * (count + 1) + l] = rez;         \
			count += 2;                                     \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			p_1 += src_stride;                              \
			p += src_stride;                                \
			p1 += src_stride;                               \
			p2 += src_stride;                               \
		    }                                                   \
		    if (i < (amount) - 2) {                             \
			if (p >= psrc_end) {                            \
			    p_1 -= src_stride;                          \
			    p -= src_stride;                            \
			    p1 -= src_stride;                           \
			    p2 -= src_stride;                           \
			}                                               \
			a2 = 9 * (pcolor[(*p)] +                        \
				(mlib_s32)pcolor[(*p1)]) -              \
				pcolor[(*p_1)] - pcolor[(*p2)];         \
			rez = (111 * a + 29 * a1 - 9 * a_1 -            \
				3 * a2) >> 11;                          \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			rez = (29 * a + 111 * a1 - 3 * a_1 -            \
				9 * a2) >> 11;                          \
			SAT_##src_type();                               \
			pdst_buf[chan * (count + 1) + l] = rez;         \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			p_1 += src_stride;                              \
			p += src_stride;                                \
			p1 += src_stride;                               \
			p2 += src_stride;                               \
			i += 2;                                         \
			count += 2;                                     \
		    }                                                   \
		    if (i < (amount)) {                                 \
			if (p >= psrc_end) {                            \
			    p_1 -= src_stride;                          \
			    p -= src_stride;                            \
			    p1 -= src_stride;                           \
			    p2 -= src_stride;                           \
			}                                               \
			a2 = 9 * (pcolor[(*p)] +                        \
				(mlib_s32)pcolor[(*p1)]) -              \
				pcolor[(*p_1)] - pcolor[(*p2)];         \
			rez = (111 * a + 29 * a1 - 9 * a_1 -            \
				3 * a2) >> 11;                          \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			count++;                                        \
			i++;                                            \
		    }                                                   \
		    if (i < (amount)) {                                 \
			rez = (29 * a + 111 * a1 - 3 * a_1 -            \
				9 * a2) >> 11;                          \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			count++;                                        \
		    }                                                   \
		}                                                       \
	    }                                                           \
	}

/* *********************************************************** */

#define	ROW_BICUBIC_FL3(beg_i, j, chan, amount, src_type)               \
	{                                                               \
	    PREPARE_BICUBIC(beg_i, j);                                  \
	    psrc_end = psrc_beg + hx + src_width_beg;                   \
	    if (dh == 0) {                                              \
		for (l = 0; l < chan; l++) {                            \
		    count = 0;                                          \
		    pcolor = table[l] - offset;                         \
		    a_1 = pcolor[psrc_beg[hx + wx_1]];                  \
		    a = pcolor[psrc_beg[hx + wx]];                      \
		    a1 = pcolor[psrc_beg[hx + wx1]];                    \
		    p = psrc_beg + hx + wx2;                            \
		    i = (beg_i);                                        \
		    if (dw == 0) {                                      \
			pdst_buf[chan * count + l] = a;                 \
			count++;                                        \
			i++;                                            \
		    }                                                   \
		    for (; i < (amount) - 2; i += 2) {                  \
			a2 = pcolor[(*p)];                              \
			rez = (9 * (a + a1) - a_1 - a2) >> 4;           \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			pdst_buf[chan * (count + 1) + l] = a1;          \
			count += 2;                                     \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			p++;                                            \
		    }                                                   \
		    if (i < (amount)) {                                 \
			if (p >= psrc_end) {                            \
			    p--;                                        \
			}                                               \
			a2 = pcolor[(*p)];                              \
			rez = (9 * (a + a1) - a_1 - a2) >> 4;           \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			count++;                                        \
			i++;                                            \
		    }                                                   \
		    if (i < (amount)) {                                 \
			pdst_buf[chan * count + l] = a1;                \
			count++;                                        \
		    }                                                   \
		}                                                       \
	    } else {                                                    \
		for (l = 0; l < chan; l++) {                            \
		    count = 0;                                          \
		    pcolor = table[l] - offset;                         \
		    a_1 = (9 * (pcolor[psrc_beg[hx + wx_1]] +           \
			    (mlib_s32)pcolor[psrc_beg[hx1 + wx_1]]) -   \
			    pcolor[psrc_beg[hx_1 + wx_1]] -             \
			    pcolor[psrc_beg[hx2 + wx_1]]);              \
		    a = (9 * (pcolor[psrc_beg[hx + wx]] +               \
			    (mlib_s32)pcolor[psrc_beg[hx1 + wx]]) -     \
			    pcolor[psrc_beg[hx_1 + wx]] -               \
			    pcolor[psrc_beg[hx2 + wx]]);                \
		    a1 = (9 * (pcolor[psrc_beg[hx + wx1]] +             \
			    (mlib_s32)pcolor[psrc_beg[hx1 + wx1]]) -    \
			    pcolor[psrc_beg[hx_1 + wx1]] -              \
			    pcolor[psrc_beg[hx2 + wx1]]);               \
		    p_1 = psrc_beg + hx_1 + wx2;                        \
		    p = psrc_beg + hx + wx2;                            \
		    p1 = psrc_beg + hx1 + wx2;                          \
		    p2 = psrc_beg + hx2 + wx2;                          \
		    i = (beg_i);                                        \
		    if (dw == 0) {                                      \
			rez = a >> 4;                                   \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			count++;                                        \
			i++;                                            \
		    }                                                   \
		    for (; i < (amount) - 2; i += 2) {                  \
			a2 = 9 * (pcolor[(*p)] +                        \
				(mlib_s32)pcolor[(*p1)]) -              \
				pcolor[(*p_1)] - pcolor[(*p2)];         \
			rez = (9 * (a + a1) - a_1 - a2) >> 8;           \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			rez = a1 >> 4;                                  \
			SAT_##src_type();                               \
			pdst_buf[chan * (count + 1) + l] = rez;         \
			count += 2;                                     \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			p_1++;                                          \
			p++;                                            \
			p1++;                                           \
			p2++;                                           \
		    }                                                   \
		    if (i < (amount)) {                                 \
			if (p >= psrc_end) {                            \
			    p_1--;                                      \
			    p--;                                        \
			    p1--;                                       \
			    p2--;                                       \
			}                                               \
			a2 = 9 * (pcolor[(*p)] +                        \
				(mlib_s32)pcolor[(*p1)]) -              \
				pcolor[(*p_1)] - pcolor[(*p2)];         \
			rez = (9 * (a + a1) - a_1 - a2) >> 8;           \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			count++;                                        \
			i++;                                            \
		    }                                                   \
		    if (i < (amount)) {                                 \
			rez = a1 >> 4;                                  \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			count++;                                        \
		    }                                                   \
		}                                                       \
	    }                                                           \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC_FL3(beg_i, j, chan, amount, src_type)            \
	{                                                               \
	    PREPARE_BICUBIC(beg_i, j);                                  \
	    psrc_end = psrc_beg + wx + src_height_beg * src_stride;     \
	    if (dw == 0) {                                              \
		for (l = 0; l < chan; l++) {                            \
		    count = 0;                                          \
		    pcolor = table[l] - offset;                         \
		    a_1 = pcolor[psrc_beg[hx_1 + wx]];                  \
		    a = pcolor[psrc_beg[hx + wx]];                      \
		    a1 = pcolor[psrc_beg[hx1 + wx]];                    \
		    p = psrc_beg + hx2 + wx;                            \
		    i = (j);                                            \
		    if (dh == 0) {                                      \
			pdst_buf[chan * count + l] = a;                 \
			count++;                                        \
			i++;                                            \
		    }                                                   \
		    for (; i < (amount) - 2; i += 2) {                  \
			a2 = pcolor[(*p)];                              \
			rez = (9 * (a + a1) - a_1 - a2) >> 4;           \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			pdst_buf[chan * (count + 1) + l] = a1;          \
			count += 2;                                     \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			p += src_stride;                                \
		    }                                                   \
		    if (i < (amount)) {                                 \
			if (p >= psrc_end) {                            \
			    p -= src_stride;                            \
			}                                               \
			a2 = pcolor[(*p)];                              \
			rez = (9 * (a + a1) - a_1 - a2) >> 4;           \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			count++;                                        \
			i++;                                            \
		    }                                                   \
		    if (i < (amount)) {                                 \
			pdst_buf[chan * count + l] = a1;                \
			count++;                                        \
		    }                                                   \
		}                                                       \
	    } else {                                                    \
		for (l = 0; l < chan; l++) {                            \
		    count = 0;                                          \
		    pcolor = table[l] - offset;                         \
		    a_1 = (9 * (pcolor[psrc_beg[hx_1 + wx]] +           \
			    (mlib_s32)pcolor[psrc_beg[hx_1 + wx1]]) -   \
			    pcolor[psrc_beg[hx_1 + wx_1]] -             \
			    pcolor[psrc_beg[hx_1 + wx2]]);              \
		    a = (9 * (pcolor[psrc_beg[hx + wx]] +               \
			    (mlib_s32)pcolor[psrc_beg[hx + wx1]]) -     \
			    pcolor[psrc_beg[hx + wx_1]] -               \
			    pcolor[psrc_beg[hx + wx2]]);                \
		    a1 = (9 * (pcolor[psrc_beg[hx1 + wx]] +             \
			    (mlib_s32)pcolor[psrc_beg[hx1 + wx1]]) -    \
			    pcolor[psrc_beg[hx1 + wx_1]] -              \
			    pcolor[psrc_beg[hx1 + wx2]]);               \
		    p_1 = psrc_beg + hx2 + wx_1;                        \
		    p = psrc_beg + hx2 + wx;                            \
		    p1 = psrc_beg + hx2 + wx1;                          \
		    p2 = psrc_beg + hx2 + wx2;                          \
		    i = (j);                                            \
		    if (dh == 0) {                                      \
			rez = a >> 4;                                   \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			count++;                                        \
			i++;                                            \
		    }                                                   \
		    for (; i < (amount) - 2; i += 2) {                  \
			a2 = 9 * (pcolor[(*p)] +                        \
				(mlib_s32)pcolor[(*p1)]) -              \
				pcolor[(*p_1)] - pcolor[(*p2)];         \
			rez = (9 * (a + a1) - a_1 - a2) >> 8;           \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			rez = a1 >> 4;                                  \
			SAT_##src_type();                               \
			pdst_buf[chan * (count + 1) + l] = rez;         \
			count += 2;                                     \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			p_1 += src_stride;                              \
			p += src_stride;                                \
			p1 += src_stride;                               \
			p2 += src_stride;                               \
		    }                                                   \
		    if (i < (amount)) {                                 \
			if (p >= psrc_end) {                            \
			    p_1 -= src_stride;                          \
			    p -= src_stride;                            \
			    p1 -= src_stride;                           \
			    p2 -= src_stride;                           \
			}                                               \
			a2 = 9 * (pcolor[(*p)] +                        \
				(mlib_s32)pcolor[(*p1)]) -              \
				pcolor[(*p_1)] - pcolor[(*p2)];         \
			rez = (9 * (a + a1) - a_1 - a2) >> 8;           \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			count++;                                        \
			i++;                                            \
		    }                                                   \
		    if (i < (amount)) {                                 \
			rez = a1 >> 4;                                  \
			SAT_##src_type();                               \
			pdst_buf[chan * count + l] = rez;               \
			count++;                                        \
		    }                                                   \
		}                                                       \
	    }                                                           \
	}

/* *********************************************************** */

#define	MLIB_ZOOMIN2XINDEX_SE_BC(src_format, src_type,                    \
	dst_format, dst_type, nchannels, SHIFT, FUNC_NAME)                \
	{                                                                 \
	    dst_format *pd;                                               \
	    const dst_format *pdst_count, *p_1, *p, *p1, *p2, *psrc_end;  \
	    src_format *pcolor;                                           \
	    mlib_s32 ii, jj, k, count;                                    \
	                                                                  \
	    IDENTIFICATORS_FOR_SATURATION_##src_type();                   \
	    pdst_count = pdst_near;                                       \
	    ii = 0;                                                       \
	    jj = 0;                                                       \
	    while (((pdst - pdst_count) >= dst_stride) &&                 \
		    (src_height_near > 0)) {                              \
		pd = pdst_near + jj * dst_stride + ii;                    \
		ROW_BICUBIC_FL##SHIFT(ii, jj, nchannels,                  \
			src_width_near, src_type);                        \
		FUNC_NAME##src_type##_##dst_type##_##nchannels            \
			(pdst_buf, pd, count, colormap);                  \
		jj++;                                                     \
		pdst_count += dst_stride;                                 \
		src_height_near--;                                        \
	    }                                                             \
	    while (src_height_near > src_height) {                        \
		pd = pdst_near + (jj + src_height_near -                  \
			1) * dst_stride + ii;                             \
		ROW_BICUBIC_FL##SHIFT(ii, jj + src_height_near - 1,       \
			nchannels, src_width_near, src_type);             \
		FUNC_NAME##src_type##_##dst_type##_##nchannels            \
			(pdst_buf, pd, count, colormap);                  \
		src_height_near--;                                        \
	    }                                                             \
	    if (src_height_near > 0) {                                    \
		while ((pdst - pdst_count > 0) && (src_width_near > 0)) { \
		    pd = pdst_near + jj * dst_stride + ii;                \
		    COLUMN_BICUBIC_FL##SHIFT(ii, jj, nchannels,           \
			    (src_height_near + jj), src_type);            \
		    FUNC_NAME##src_type##_##dst_type##_##nchannels        \
			    (pdst_buf, pdst_buf1, count, colormap);       \
		    for (k = 0; k < count; k++)                           \
			pd[k * dst_stride] = pdst_buf1[k];                \
		    src_width_near--;                                     \
		    pdst_count++;                                         \
		    ii++;                                                 \
		}                                                         \
		while (src_width_near > src_width) {                      \
		    pd = pdst_near + jj * dst_stride + ii +               \
			    src_width_near - 1;                           \
		    COLUMN_BICUBIC_FL##SHIFT(ii + src_width_near -        \
			    1, jj, nchannels, (src_height_near + jj),     \
			    src_type);                                    \
		    FUNC_NAME##src_type##_##dst_type##_##nchannels        \
			    (pdst_buf, pdst_buf1, count, colormap);       \
		    for (k = 0; k < count; k++)                           \
			pd[k * dst_stride] = pdst_buf1[k];                \
		    src_width_near--;                                     \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

#define	ALLOC_BC_U8(nchannels)                                          \
	mlib_u8 *pdst_buf;                                              \
	mlib_s32 offset = mlib_ImageGetLutOffset(colormap);             \
	mlib_u8 **table = (mlib_u8 **)mlib_ImageGetLutData(colormap);   \
	                                                                \
	pdst_buf = (mlib_u8 *)__mlib_malloc((size + 4) * nchannels);    \
	if (pdst_buf == NULL) {                                         \
	    __mlib_free(pdst_buf1);                                     \
	    return (MLIB_FAILURE);                                      \
	}

/* *********************************************************** */

#define	ALLOC_BC_S16(nchannels)                                           \
	mlib_s16 *pdst_buf;                                               \
	mlib_s32 offset = mlib_ImageGetLutOffset(colormap);               \
	mlib_s16 **table = (mlib_s16 **)mlib_ImageGetLutData(colormap);   \
	                                                                  \
	pdst_buf =                                                        \
		(mlib_s16 *)__mlib_malloc((size + 4) * 2 * nchannels);    \
	if (pdst_buf == NULL) {                                           \
	    __mlib_free(pdst_buf1);                                       \
	    return (MLIB_FAILURE);                                        \
	}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_U8_0_1(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_u8 *pdst_buf1;

	pdst_buf1 =
		(mlib_u8 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_u8) * 3);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_U8(3);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_u8,
			U8,
			mlib_u8,
			U8,
			3,
			0,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_U8_0_2(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_u8 *pdst_buf1;

	pdst_buf1 =
		(mlib_u8 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_u8) * 4);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_U8(4);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_u8,
			U8,
			mlib_u8,
			U8,
			4,
			0,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_U8_0_3(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_u8 *pdst_buf1;

	pdst_buf1 =
		(mlib_u8 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_u8) * 3);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_S16(3);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_s16,
			S16,
			mlib_u8,
			U8,
			3,
			0,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_U8_0_4(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_u8 *pdst_buf1;

	pdst_buf1 =
		(mlib_u8 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_u8) * 4);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_S16(4);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_s16,
			S16,
			mlib_u8,
			U8,
			4,
			0,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_S16_0_1(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_s16 *pdst_buf1;

	pdst_buf1 =
		(mlib_s16 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_s16) * 3);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_U8(3);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_u8,
			U8,
			mlib_s16,
			S16,
			3,
			0,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_S16_0_2(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_s16 *pdst_buf1;

	pdst_buf1 =
		(mlib_s16 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_s16) * 4);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_U8(4);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_u8,
			U8,
			mlib_s16,
			S16,
			4,
			0,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_S16_0_3(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_s16 *pdst_buf1;

	pdst_buf1 =
		(mlib_s16 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_s16) * 3);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_S16(3);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_s16,
			S16,
			mlib_s16,
			S16,
			3,
			0,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_S16_0_4(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_s16 *pdst_buf1;

	pdst_buf1 =
		(mlib_s16 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_s16) * 4);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_S16(4);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_s16,
			S16,
			mlib_s16,
			S16,
			4,
			0,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_U8_1_1(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_u8 *pdst_buf1;

	pdst_buf1 =
		(mlib_u8 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_u8) * 3);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_U8(3);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_u8,
			U8,
			mlib_u8,
			U8,
			3,
			1,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_U8_1_2(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_u8 *pdst_buf1;

	pdst_buf1 =
		(mlib_u8 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_u8) * 4);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_U8(4);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_u8,
			U8,
			mlib_u8,
			U8,
			4,
			1,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_U8_1_3(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_u8 *pdst_buf1;

	pdst_buf1 =
		(mlib_u8 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_u8) * 3);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_S16(3);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_s16,
			S16,
			mlib_u8,
			U8,
			3,
			1,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_U8_1_4(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_u8 *pdst_buf1;

	pdst_buf1 =
		(mlib_u8 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_u8) * 4);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_S16(4);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_s16,
			S16,
			mlib_u8,
			U8,
			4,
			1,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_S16_1_1(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_s16 *pdst_buf1;

	pdst_buf1 =
		(mlib_s16 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_s16) * 3);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_U8(3);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_u8,
			U8,
			mlib_s16,
			S16,
			3,
			1,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_S16_1_2(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_s16 *pdst_buf1;

	pdst_buf1 =
		(mlib_s16 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_s16) * 4);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_U8(4);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_u8,
			U8,
			mlib_s16,
			S16,
			4,
			1,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_S16_1_3(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_s16 *pdst_buf1;

	pdst_buf1 =
		(mlib_s16 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_s16) * 3);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_S16(3);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_s16,
			S16,
			mlib_s16,
			S16,
			3,
			1,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_S16_1_4(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_s16 *pdst_buf1;

	pdst_buf1 =
		(mlib_s16 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_s16) * 4);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_S16(4);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_s16,
			S16,
			mlib_s16,
			S16,
			4,
			1,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_U8_2_1(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_u8 *pdst_buf1;

	pdst_buf1 =
		(mlib_u8 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_u8) * 3);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_U8(3);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_u8,
			U8,
			mlib_u8,
			U8,
			3,
			2,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_U8_2_2(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_u8 *pdst_buf1;

	pdst_buf1 =
		(mlib_u8 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_u8) * 4);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_U8(4);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_u8,
			U8,
			mlib_u8,
			U8,
			4,
			2,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_U8_2_3(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_u8 *pdst_buf1;

	pdst_buf1 =
		(mlib_u8 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_u8) * 3);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_S16(3);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_s16,
			S16,
			mlib_u8,
			U8,
			3,
			2,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_U8_2_4(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_u8 *pdst_buf1;

	pdst_buf1 =
		(mlib_u8 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_u8) * 4);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_S16(4);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_s16,
			S16,
			mlib_u8,
			U8,
			4,
			2,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_S16_2_1(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_s16 *pdst_buf1;

	pdst_buf1 =
		(mlib_s16 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_s16) * 3);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_U8(3);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_u8,
			U8,
			mlib_s16,
			S16,
			3,
			2,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_S16_2_2(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_s16 *pdst_buf1;

	pdst_buf1 =
		(mlib_s16 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_s16) * 4);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_U8(4);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_u8,
			U8,
			mlib_s16,
			S16,
			4,
			2,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_S16_2_3(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_s16 *pdst_buf1;

	pdst_buf1 =
		(mlib_s16 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_s16) * 3);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_S16(3);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_s16,
			S16,
			mlib_s16,
			S16,
			3,
			2,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_S16_2_4(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_s16 *pdst_buf1;

	pdst_buf1 =
		(mlib_s16 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_s16) * 4);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_S16(4);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_s16,
			S16,
			mlib_s16,
			S16,
			4,
			2,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_U8_3_1(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_u8 *pdst_buf1;

	pdst_buf1 =
		(mlib_u8 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_u8) * 3);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_U8(3);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_u8,
			U8,
			mlib_u8,
			U8,
			3,
			3,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_U8_3_2(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_u8 *pdst_buf1;

	pdst_buf1 =
		(mlib_u8 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_u8) * 4);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_U8(4);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_u8,
			U8,
			mlib_u8,
			U8,
			4,
			3,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_U8_3_3(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_u8 *pdst_buf1;

	pdst_buf1 =
		(mlib_u8 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_u8) * 3);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_S16(3);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_s16,
			S16,
			mlib_u8,
			U8,
			3,
			3,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_U8_3_4(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_u8 *pdst_buf1;

	pdst_buf1 =
		(mlib_u8 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_u8) * 4);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_S16(4);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_s16,
			S16,
			mlib_u8,
			U8,
			4,
			3,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_S16_3_1(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_s16 *pdst_buf1;

	pdst_buf1 =
		(mlib_s16 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_s16) * 3);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_U8(3);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_u8,
			U8,
			mlib_s16,
			S16,
			3,
			3,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_S16_3_2(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_s16 *pdst_buf1;

	pdst_buf1 =
		(mlib_s16 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_s16) * 4);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_U8(4);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_u8,
			U8,
			mlib_s16,
			S16,
			4,
			3,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_S16_3_3(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_s16 *pdst_buf1;

	pdst_buf1 =
		(mlib_s16 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_s16) * 3);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_S16(3);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_s16,
			S16,
			mlib_s16,
			S16,
			3,
			3,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
MLIB_ZOOMIN2XINDEX_SE_BC_S16_3_4(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16))
{
	mlib_s32 size =
		(src_height_near >
		src_width_near ? src_height_near : src_width_near);
	mlib_s16 *pdst_buf1;

	pdst_buf1 =
		(mlib_s16 *)__mlib_malloc((src_height_near +
		4) * sizeof (mlib_s16) * 4);

	if (pdst_buf1 == NULL)
		return (MLIB_FAILURE);

	if (1) {
		ALLOC_BC_S16(4);
		MLIB_ZOOMIN2XINDEX_SE_BC(mlib_s16,
			S16,
			mlib_s16,
			S16,
			4,
			3,
			mlib_ImageColorTrue2IndexLine_);

		FREE();
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	PARAMS_LIST                                             \
	pdst, src_width, src_height, dst_stride, pdst_near,     \
	src_width_near, src_height_near, colormap, src_stride,  \
	psrc_beg, src_width_beg, src_height_beg, src_w_beg_near,\
	src_h_beg_near

#define	MLIB_IMAGEZOOMIN2XINDEX_SE_BC(dst_format, dst_type, SHIFT)          \
	mlib_s32 lut_type = mlib_ImageGetLutType(colormap);                 \
	mlib_s32 nchannels = mlib_ImageGetLutChannels(colormap);            \
	if ((nchannels != 3) && (nchannels != 4))                           \
	    return (MLIB_FAILURE);                                          \
	if (lut_type == MLIB_BYTE) {                                        \
	    if (nchannels == 3)                                             \
		return (MLIB_ZOOMIN2XINDEX_SE_BC_##dst_type##_##SHIFT##_1(  \
			PARAMS_LIST));                                      \
	    else                                                            \
		return (MLIB_ZOOMIN2XINDEX_SE_BC_##dst_type##_##SHIFT##_2(  \
			PARAMS_LIST));                                      \
	}                                                                   \
	else if (lut_type == MLIB_SHORT) {                                  \
	    if (nchannels == 3)                                             \
		return (MLIB_ZOOMIN2XINDEX_SE_BC_##dst_type##_##SHIFT##_3(  \
			PARAMS_LIST));                                      \
	    else                                                            \
		return (MLIB_ZOOMIN2XINDEX_SE_BC_##dst_type##_##SHIFT##_4(  \
			PARAMS_LIST));                                      \
	}                                                                   \
                                                                            \
	return MLIB_SUCCESS

/* *********************************************************** */

mlib_status
mlib_ImageZoomIn2XIndex_U8_SE_BC_FL0(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8))
{
	MLIB_IMAGEZOOMIN2XINDEX_SE_BC(mlib_u8,
		U8,
		0);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomIn2XIndex_U8_SE_BC_FL1(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8))
{
	MLIB_IMAGEZOOMIN2XINDEX_SE_BC(mlib_u8,
		U8,
		1);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomIn2XIndex_U8_SE_BC_FL2(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8))
{
	MLIB_IMAGEZOOMIN2XINDEX_SE_BC(mlib_u8,
		U8,
		2);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomIn2XIndex_U8_SE_BC_FL3(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_u8))
{
	MLIB_IMAGEZOOMIN2XINDEX_SE_BC(mlib_u8,
		U8,
		3);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomIn2XIndex_S16_SE_BC_FL0(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16))
{
	MLIB_IMAGEZOOMIN2XINDEX_SE_BC(mlib_s16,
		S16,
		0);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomIn2XIndex_S16_SE_BC_FL1(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16))
{
	MLIB_IMAGEZOOMIN2XINDEX_SE_BC(mlib_s16,
		S16,
		1);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomIn2XIndex_S16_SE_BC_FL2(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16))
{
	MLIB_IMAGEZOOMIN2XINDEX_SE_BC(mlib_s16,
		S16,
		2);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomIn2XIndex_S16_SE_BC_FL3(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BC(mlib_s16))
{
	MLIB_IMAGEZOOMIN2XINDEX_SE_BC(mlib_s16,
		S16,
		3);
}

/* *********************************************************** */
