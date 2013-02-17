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

#pragma ident	"@(#)mlib_ImageZoomIn2X_Edge.c	9.2	07/11/05 SMI"

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
 *    MLIB_BICUBIC2
 *
 *  The edge condition can be one of the following:
 *    MLIB_EDGE_DST_NO_WRITE  (default)
 *    MLIB_EDGE_DST_FILL_ZERO
 *    MLIB_EDGE_OP_NEAREST
 *    MLIB_EDGE_SRC_EXTEND
 *    MLIB_EDGE_SRC_PADDED
 */

#include <mlib_image.h>
#include <mlib_ImageZoomIn2X.h>

/* *********************************************************** */

#define	MLIB_ZOOMIN2XDEFEDGE(MLIB_TYPE)                               \
	MLIB_TYPE	*pdst,                                        \
	const MLIB_TYPE *psrc,                                        \
	mlib_s32	src_width,                                    \
	mlib_s32	src_height,                                   \
	mlib_s32	dst_stride,                                   \
	mlib_s32	src_stride,                                   \
	mlib_s32	src_w_beg,                                    \
	mlib_s32	src_h_beg,                                    \
	mlib_s32	channels,                                     \
	MLIB_TYPE	*pdst_near,                                   \
	const MLIB_TYPE *psrc_near,                                   \
	mlib_s32	src_width_near,                               \
	mlib_s32	src_height_near,                              \
	mlib_s32	src_w_beg_near,                               \
	mlib_s32	src_h_beg_near

/* *********************************************************** */

#define	MLIB_ZOOMIN2XDEFEDGE_BC(MLIB_TYPE)                            \
	MLIB_ZOOMIN2XDEFEDGE(MLIB_TYPE),                              \
	const MLIB_TYPE *psrc_beg,                                    \
	mlib_s32  src_width_beg,                                      \
	mlib_s32  src_height_beg

/* *********************************************************** */

#define	ROW_COLUMN_NN(beg, amount, shift_dst, shift_src)        \
	if ((beg & 1) != 0) {                                   \
	    *tpdst = *tpsrc;                                    \
	    tpdst += shift_dst;                                 \
	    tpsrc += shift_src;                                 \
	    j = 2;                                              \
	} else                                                  \
	    j = 1;                                              \
	for (; j < amount; j += 2) {                            \
	    *tpdst = *(tpdst + shift_dst) = *tpsrc;             \
	    tpsrc += shift_src;                                 \
	    tpdst += (shift_dst << 1);                          \
	}                                                       \
	                                                        \
	if (j == amount)                                        \
	    *tpdst = *tpsrc

/* *********************************************************** */

#define	ROW_COLUMN_BL_FL0(beg, amount, shift_dst, shift_src)           \
	{                                                              \
	    mlib_s32 flag = 0;                                         \
	                                                               \
	    j = (beg + 1) & 1;                                         \
	    if (beg == 0) {                                            \
		*tpdst = (*tpsrc);                                     \
		tpdst += shift_dst;                                    \
		flag = 0;                                              \
	    } else if (j == 1) {                                       \
		*tpdst = (3 * (*tpsrc) + *(tpsrc - shift_src)) >> 2;   \
		tpdst += shift_dst;                                    \
		flag = 1;                                              \
	    }                                                          \
	    tmp1 = *tpsrc;                                             \
	    for (j++; j < amount; j += 2) {                            \
		tmp2 = *(tpsrc + shift_src);                           \
		*tpdst = (3 * tmp1 + tmp2) >> 2;                       \
		*(tpdst + shift_dst) = (tmp1 + 3 * tmp2) >> 2;         \
		tpsrc += shift_src;                                    \
		tpdst += (shift_dst << 1);                             \
		tmp1 = tmp2;                                           \
	    }                                                          \
	    if (j == amount)                                           \
		if (flag)                                              \
		    *tpdst = (3 * tmp1 + *(tpsrc + shift_src)) >> 2;   \
		else                                                   \
		    *tpdst = tmp1;                                     \
	}

/* *********************************************************** */

#define	ROW_COLUMN_BL_FL1(beg, amount, shift_dst, shift_src)    \
	j = (beg + 1) & 1;                                      \
	if (beg == 0) {                                         \
	    *tpdst = *tpsrc;                                    \
	    tpdst += shift_dst;                                 \
	} else if (j == 1) {                                    \
	    *tpdst = (*tpsrc + *(tpsrc - shift_src)) >> 1;      \
	    tpdst += shift_dst;                                 \
	}                                                       \
	tmp1 = *tpsrc;                                          \
	for (j++; j < amount; j += 2) {                         \
	    tmp2 = *(tpsrc + shift_src);                        \
	    *tpdst = tmp1;                                      \
	    *(tpdst + shift_dst) = (tmp1 + tmp2) >> 1;          \
	    tpsrc += shift_src;                                 \
	    tpdst += (shift_dst << 1);                          \
	    tmp1 = tmp2;                                        \
	}                                                       \
	                                                        \
	if (j == amount)                                        \
	    *tpdst = tmp1

/* *********************************************************** */

#define	PREPARE_BICUBIC(beg_i, j, chan)                                 \
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
	mlib_s32 a_1, a, a1, a2 = 0, l, rez, rez1, i;                   \
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
	    wx2 = src_width_beg - 1;                                    \
	wx_1 *= chan;                                                   \
	wx *= chan;                                                     \
	wx1 *= chan;                                                    \
	wx2 *= chan

/* *********************************************************** */

#define	ROW_BICUBIC_FL0(beg_i, j, chan, amount, MIN, MAX, STYPE)        \
	{                                                               \
	    const STYPE *psrc_end;                                      \
	                                                                \
	    PREPARE_BICUBIC(beg_i, j, chan);                            \
	    psrc_end = psrc_beg + hx + (src_width_beg - 1) * chan;      \
	    if (dh == 0) {                                              \
		for (l = 0; l < chan; l++) {                            \
		    a_1 = (111 * psrc_beg[hx + wx_1 + l] +              \
			    29 * psrc_beg[hx1 + wx_1 + l] -             \
			    9 * psrc_beg[hx_1 + wx_1 + l] -             \
			    3 * psrc_beg[hx2 + wx_1 + l]);              \
		    a = (111 * psrc_beg[hx + wx + l] +                  \
			    29 * psrc_beg[hx1 + wx + l] -               \
			    9 * psrc_beg[hx_1 + wx + l] -               \
			    3 * psrc_beg[hx2 + wx + l]);                \
		    a1 = (111 * psrc_beg[hx + wx1 + l] +                \
			    29 * psrc_beg[hx1 + wx1 + l] -              \
			    9 * psrc_beg[hx_1 + wx1 + l] -              \
			    3 * psrc_beg[hx2 + wx1 + l]);               \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                    \
		    p = psrc_beg + hx + wx2 + l;                        \
		    p1 = psrc_beg + hx1 + wx2 + l;                      \
		    p2 = psrc_beg + hx2 + wx2 + l;                      \
		    i = (beg_i);                                        \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;   \
		    if (dw == 1) {                                      \
			a2 = 111 * (*p) + 29 * (*p1) - 9 * (*p_1) -     \
				3 * (*p2);                              \
			rez = (29 * a + 111 * a1 - 3 * a_1 -            \
				9 * a2) >> 14;                          \
			if (rez > MAX)                                  \
			    rez = MAX;                                  \
			else if (rez < MIN)                             \
			    rez = MIN;                                  \
			*pd = rez;                                      \
			pd += chan;                                     \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			if (p < psrc_end) {                             \
			    p_1 += chan;                                \
			    p += chan;                                  \
			    p1 += chan;                                 \
			    p2 += chan;                                 \
			}                                               \
			i++;                                            \
		    }                                                   \
		    for (; i < (amount) - 2; i += 2) {                  \
			a2 = 111 * (*p) + 29 * (*p1) - 9 * (*p_1) -     \
				3 * (*p2);                              \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -           \
				3 * a2) >> 14;                          \
			if (rez1 > MAX)                                 \
			    rez1 = MAX;                                 \
			else if (rez1 < MIN)                            \
			    rez1 = MIN;                                 \
			*pd = rez1;                                     \
			rez = (29 * a + 111 * a1 - 3 * a_1 -            \
				9 * a2) >> 14;                          \
			if (rez > MAX)                                  \
			    rez = MAX;                                  \
			else if (rez < MIN)                             \
			    rez = MIN;                                  \
			*(pd + chan) = rez;                             \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			if (p < psrc_end) {                             \
			    p_1 += chan;                                \
			    p += chan;                                  \
			    p1 += chan;                                 \
			    p2 += chan;                                 \
			}                                               \
			pd += (chan << 1);                              \
		    }                                                   \
		    if (i < (amount)) {                                 \
			a2 = (111 * (*p) + 29 * (*p1) - 9 * (*p_1) -    \
				3 * (*p2));                             \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -           \
				3 * a2) >> 14;                          \
			if (rez1 > MAX)                                 \
			    rez1 = MAX;                                 \
			else if (rez1 < MIN)                            \
			    rez1 = MIN;                                 \
			*pd = rez1;                                     \
			pd += chan;                                     \
			i++;                                            \
		    }                                                   \
		    if (i < (amount)) {                                 \
			rez = (29 * a + 111 * a1 - 3 * a_1 -            \
				9 * a2) >> 14;                          \
			if (rez > MAX)                                  \
			    rez = MAX;                                  \
			else if (rez < MIN)                             \
			    rez = MIN;                                  \
			*pd = rez;                                      \
		    }                                                   \
		}                                                       \
	    } else {                                                    \
		for (l = 0; l < chan; l++) {                            \
		    a_1 = (29 * psrc_beg[hx + wx_1 + l] +               \
			    111 * psrc_beg[hx1 + wx_1 + l] -            \
			    3 * psrc_beg[hx_1 + wx_1 + l] -             \
			    9 * psrc_beg[hx2 + wx_1 + l]);              \
		    a = (29 * psrc_beg[hx + wx + l] +                   \
			    111 * psrc_beg[hx1 + wx + l] -              \
			    3 * psrc_beg[hx_1 + wx + l] -               \
			    9 * psrc_beg[hx2 + wx + l]);                \
		    a1 = (29 * psrc_beg[hx + wx1 + l] +                 \
			    111 * psrc_beg[hx1 + wx1 + l] -             \
			    3 * psrc_beg[hx_1 + wx1 + l] -              \
			    9 * psrc_beg[hx2 + wx1 + l]);               \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                    \
		    p = psrc_beg + hx + wx2 + l;                        \
		    p1 = psrc_beg + hx1 + wx2 + l;                      \
		    p2 = psrc_beg + hx2 + wx2 + l;                      \
		    i = (beg_i);                                        \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;   \
		    if (dw == 1) {                                      \
			a2 = 29 * (*p) + 111 * (*p1) - 3 * (*p_1) -     \
				9 * (*p2);                              \
			rez = (29 * a + 111 * a1 - 3 * a_1 -            \
				9 * a2) >> 14;                          \
			if (rez > MAX)                                  \
			    rez = MAX;                                  \
			else if (rez < MIN)                             \
			    rez = MIN;                                  \
			*pd = rez;                                      \
			pd += chan;                                     \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			if (p < psrc_end) {                             \
			    p_1 += chan;                                \
			    p += chan;                                  \
			    p1 += chan;                                 \
			    p2 += chan;                                 \
			}                                               \
			i++;                                            \
		    }                                                   \
		    for (; i < (amount) - 2; i += 2) {                  \
			a2 = 29 * (*p) + 111 * (*p1) - 3 * (*p_1) -     \
				9 * (*p2);                              \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -           \
				3 * a2) >> 14;                          \
			if (rez1 > MAX)                                 \
			    rez1 = MAX;                                 \
			else if (rez1 < MIN)                            \
			    rez1 = MIN;                                 \
			*pd = rez1;                                     \
			rez = (29 * a + 111 * a1 - 3 * a_1 -            \
				9 * a2) >> 14;                          \
			if (rez > MAX)                                  \
			    rez = MAX;                                  \
			else if (rez < MIN)                             \
			    rez = MIN;                                  \
			*(pd + chan) = rez;                             \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			if (p < psrc_end) {                             \
			    p_1 += chan;                                \
			    p += chan;                                  \
			    p1 += chan;                                 \
			    p2 += chan;                                 \
			}                                               \
			pd += (chan << 1);                              \
		    }                                                   \
		    if (i < (amount)) {                                 \
			a2 = 29 * (*p) + 111 * (*p1) - 3 * (*p_1) -     \
				9 * (*p2);                              \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -           \
				3 * a2) >> 14;                          \
			if (rez1 > MAX)                                 \
			    rez1 = MAX;                                 \
			else if (rez1 < MIN)                            \
			    rez1 = MIN;                                 \
			*pd = rez1;                                     \
			pd += chan;                                     \
			i++;                                            \
		    }                                                   \
		    if (i < (amount)) {                                 \
			rez = (29 * a + 111 * a1 - 3 * a_1 -            \
				9 * a2) >> 14;                          \
			if (rez > MAX)                                  \
			    rez = MAX;                                  \
			else if (rez < MIN)                             \
			    rez = MIN;                                  \
			*pd = rez;                                      \
		    }                                                   \
		}                                                       \
	    }                                                           \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC_FL0(beg_i, j, chan, amount, MIN, MAX, STYPE)      \
	{                                                                \
	    const STYPE *psrc_end;                                       \
	                                                                 \
	    PREPARE_BICUBIC(beg_i, j, chan);                             \
	    psrc_end =                                                   \
		    psrc_beg + wx + (src_height_beg - 1) * src_stride;   \
	    if (dw == 0) {                                               \
		for (l = 0; l < chan; l++) {                             \
		    a_1 = (111 * psrc_beg[hx_1 + wx + l] +               \
			    29 * psrc_beg[hx_1 + wx1 + l] -              \
			    9 * psrc_beg[hx_1 + wx_1 + l] -              \
			    3 * psrc_beg[hx_1 + wx2 + l]);               \
		    a = (111 * psrc_beg[hx + wx + l] +                   \
			    29 * psrc_beg[hx + wx1 + l] -                \
			    9 * psrc_beg[hx + wx_1 + l] -                \
			    3 * psrc_beg[hx + wx2 + l]);                 \
		    a1 = (111 * psrc_beg[hx1 + wx + l] +                 \
			    29 * psrc_beg[hx1 + wx1 + l] -               \
			    9 * psrc_beg[hx1 + wx_1 + l] -               \
			    3 * psrc_beg[hx1 + wx2 + l]);                \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                     \
		    p = psrc_beg + hx2 + wx + l;                         \
		    p1 = psrc_beg + hx2 + wx1 + l;                       \
		    p2 = psrc_beg + hx2 + wx2 + l;                       \
		    i = (j);                                             \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +   \
			    l;                                           \
		    if (dh == 1) {                                       \
			a2 = 111 * (*p) + 29 * (*p1) - 9 * (*p_1) -      \
				3 * (*p2);                               \
			rez = (29 * a + 111 * a1 - 3 * a_1 -             \
				9 * a2) >> 14;                           \
			if (rez > MAX)                                   \
			    rez = MAX;                                   \
			else if (rez < MIN)                              \
			    rez = MIN;                                   \
			*pd = rez;                                       \
			pd += dst_stride;                                \
			i++;                                             \
			a_1 = a;                                         \
			a = a1;                                          \
			a1 = a2;                                         \
			if (p < psrc_end) {                              \
			    p_1 += src_stride;                           \
			    p += src_stride;                             \
			    p1 += src_stride;                            \
			    p2 += src_stride;                            \
			}                                                \
		    }                                                    \
		    for (; i < (amount) - 2; i += 2) {                   \
			a2 = 111 * (*p) + 29 * (*p1) - 9 * (*p_1) -      \
				3 * (*p2);                               \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -            \
				3 * a2) >> 14;                           \
			if (rez1 > MAX)                                  \
			    rez1 = MAX;                                  \
			else if (rez1 < MIN)                             \
			    rez1 = MIN;                                  \
			*pd = rez1;                                      \
			rez = (29 * a + 111 * a1 - 3 * a_1 -             \
				9 * a2) >> 14;                           \
			if (rez > MAX)                                   \
			    rez = MAX;                                   \
			else if (rez < MIN)                              \
			    rez = MIN;                                   \
			*(pd + dst_stride) = rez;                        \
			a_1 = a;                                         \
			a = a1;                                          \
			a1 = a2;                                         \
			if (p < psrc_end) {                              \
			    p_1 += src_stride;                           \
			    p += src_stride;                             \
			    p1 += src_stride;                            \
			    p2 += src_stride;                            \
			}                                                \
			pd += (dst_stride << 1);                         \
		    }                                                    \
		    if (i < (amount)) {                                  \
			a2 = 111 * (*p) + 29 * (*p1) - 9 * (*p_1) -      \
				3 * (*p2);                               \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -            \
				3 * a2) >> 14;                           \
			if (rez1 > MAX)                                  \
			    rez1 = MAX;                                  \
			else if (rez1 < MIN)                             \
			    rez1 = MIN;                                  \
			*pd = rez1;                                      \
			pd += dst_stride;                                \
			i++;                                             \
		    }                                                    \
		    if (i < (amount)) {                                  \
			rez = (29 * a + 111 * a1 - 3 * a_1 -             \
				9 * a2) >> 14;                           \
			if (rez > MAX)                                   \
			    rez = MAX;                                   \
			else if (rez < MIN)                              \
			    rez = MIN;                                   \
			*pd = rez;                                       \
		    }                                                    \
		}                                                        \
	    } else {                                                     \
		for (l = 0; l < chan; l++) {                             \
		    a_1 = (29 * psrc_beg[hx_1 + wx + l] +                \
			    111 * psrc_beg[hx_1 + wx1 + l] -             \
			    3 * psrc_beg[hx_1 + wx_1 + l] -              \
			    9 * psrc_beg[hx_1 + wx2 + l]);               \
		    a = (29 * psrc_beg[hx + wx + l] +                    \
			    111 * psrc_beg[hx + wx1 + l] -               \
			    3 * psrc_beg[hx + wx_1 + l] -                \
			    9 * psrc_beg[hx + wx2 + l]);                 \
		    a1 = (29 * psrc_beg[hx1 + wx + l] +                  \
			    111 * psrc_beg[hx1 + wx1 + l] -              \
			    3 * psrc_beg[hx1 + wx_1 + l] -               \
			    9 * psrc_beg[hx1 + wx2 + l]);                \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                     \
		    p = psrc_beg + hx2 + wx + l;                         \
		    p1 = psrc_beg + hx2 + wx1 + l;                       \
		    p2 = psrc_beg + hx2 + wx2 + l;                       \
		    i = (j);                                             \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +   \
			    l;                                           \
		    if (dh == 1) {                                       \
			a2 = 29 * (*p) + 111 * (*p1) - 3 * (*p_1) -      \
				9 * (*p2);                               \
			rez = (29 * a + 111 * a1 - 3 * a_1 -             \
				9 * a2) >> 14;                           \
			if (rez > MAX)                                   \
			    rez = MAX;                                   \
			else if (rez < MIN)                              \
			    rez = MIN;                                   \
			*pd = rez;                                       \
			pd += dst_stride;                                \
			a_1 = a;                                         \
			a = a1;                                          \
			a1 = a2;                                         \
			if (p < psrc_end) {                              \
			    p_1 += src_stride;                           \
			    p += src_stride;                             \
			    p1 += src_stride;                            \
			    p2 += src_stride;                            \
			}                                                \
			i++;                                             \
		    }                                                    \
		    for (; i < (amount) - 2; i += 2) {                   \
			a2 = 29 * (*p) + 111 * (*p1) - 3 * (*p_1) -      \
				9 * (*p2);                               \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -            \
				3 * a2) >> 14;                           \
			if (rez1 > MAX)                                  \
			    rez1 = MAX;                                  \
			else if (rez1 < MIN)                             \
			    rez1 = MIN;                                  \
			*pd = rez1;                                      \
			rez = (29 * a + 111 * a1 - 3 * a_1 -             \
				9 * a2) >> 14;                           \
			if (rez > MAX)                                   \
			    rez = MAX;                                   \
			else if (rez < MIN)                              \
			    rez = MIN;                                   \
			*(pd + dst_stride) = rez;                        \
			a_1 = a;                                         \
			a = a1;                                          \
			a1 = a2;                                         \
			if (p < psrc_end) {                              \
			    p_1 += src_stride;                           \
			    p += src_stride;                             \
			    p1 += src_stride;                            \
			    p2 += src_stride;                            \
			}                                                \
			pd += (dst_stride << 1);                         \
		    }                                                    \
		    if (i < (amount)) {                                  \
			a2 = 29 * (*p) + 111 * (*p1) - 3 * (*p_1) -      \
				9 * (*p2);                               \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -            \
				3 * a2) >> 14;                           \
			if (rez1 > MAX)                                  \
			    rez1 = MAX;                                  \
			else if (rez1 < MIN)                             \
			    rez1 = MIN;                                  \
			*pd = rez1;                                      \
			pd += dst_stride;                                \
			i++;                                             \
		    }                                                    \
		    if (i < (amount)) {                                  \
			rez = (29 * a + 111 * a1 - 3 * a_1 -             \
				9 * a2) >> 14;                           \
			if (rez > MAX)                                   \
			    rez = MAX;                                   \
			else if (rez < MIN)                              \
			    rez = MIN;                                   \
			*pd = rez;                                       \
		    }                                                    \
		}                                                        \
	    }                                                            \
	}

/* *********************************************************** */

#define	ROW_BICUBIC_FL1(beg_i, j, chan, amount, MIN, MAX, STYPE)           \
	{                                                                  \
	    const STYPE *psrc_end;                                         \
	                                                                   \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    psrc_end = psrc_beg + hx + (src_width_beg - 1) * chan;         \
	    if (dh == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = psrc_beg[hx + wx_1 + l];                         \
		    a = psrc_beg[hx + wx + l];                             \
		    a1 = psrc_beg[hx + wx1 + l];                           \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                       \
		    p = psrc_beg + hx + wx2 + l;                           \
		    p1 = psrc_beg + hx1 + wx2 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 1) {                                         \
			a2 = (*p);                                         \
			rez = (29 * a + 111 * a1 - 3 * a_1 -               \
				9 * a2) >> 7;                              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
			pd += chan;                                        \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = (*p);                                         \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 7;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			rez = (29 * a + 111 * a1 - 3 * a_1 -               \
				9 * a2) >> 7;                              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + chan) = rez;                                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = (*p);                                         \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 7;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (29 * a + 111 * a1 - 3 * a_1 -               \
				9 * a2) >> 7;                              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = 9 * (psrc_beg[hx + wx_1 + l] + psrc_beg[hx1 +    \
			    wx_1 + l]) - psrc_beg[hx_1 + wx_1 + l] -       \
			    psrc_beg[hx2 + wx_1 + l];                      \
		    a = 9 * (psrc_beg[hx + wx + l] + psrc_beg[hx1 + wx +   \
			    l]) - psrc_beg[hx_1 + wx + l] -                \
			    psrc_beg[hx2 + wx + l];                        \
		    a1 = 9 * (psrc_beg[hx + wx1 + l] + psrc_beg[hx1 +      \
			    wx1 + l]) - psrc_beg[hx_1 + wx1 + l] -         \
			    psrc_beg[hx2 + wx1 + l];                       \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                       \
		    p = psrc_beg + hx + wx2 + l;                           \
		    p1 = psrc_beg + hx1 + wx2 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 1) {                                         \
			a2 = 9 * ((*p) + (*p1)) - (*p_1) - (*p2);          \
			rez = (29 * a + 111 * a1 - 3 * a_1 -               \
				9 * a2) >> 11;                             \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
			pd += chan;                                        \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 9 * ((*p) + (*p1)) - (*p_1) - (*p2);          \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 11;                             \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			rez = (29 * a + 111 * a1 - 3 * a_1 -               \
				9 * a2) >> 11;                             \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + chan) = rez;                                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = 9 * ((*p) + (*p1)) - (*p_1) - (*p2);          \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 11;                             \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (29 * a + 111 * a1 - 3 * a_1 -               \
				9 * a2) >> 11;                             \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC_FL1(beg_i, j, chan, amount, MIN, MAX, STYPE)      \
	{                                                                \
	    const STYPE *psrc_end;                                       \
	                                                                 \
	    PREPARE_BICUBIC(beg_i, j, chan);                             \
	    psrc_end =                                                   \
		    psrc_beg + wx + (src_height_beg - 1) * src_stride;   \
	    if (dw == 0) {                                               \
		for (l = 0; l < chan; l++) {                             \
		    a_1 = (111 * psrc_beg[hx_1 + wx + l] +               \
			    29 * psrc_beg[hx_1 + wx1 + l] -              \
			    9 * psrc_beg[hx_1 + wx_1 + l] -              \
			    3 * psrc_beg[hx_1 + wx2 + l]);               \
		    a = (111 * psrc_beg[hx + wx + l] +                   \
			    29 * psrc_beg[hx + wx1 + l] -                \
			    9 * psrc_beg[hx + wx_1 + l] -                \
			    3 * psrc_beg[hx + wx2 + l]);                 \
		    a1 = (111 * psrc_beg[hx1 + wx + l] +                 \
			    29 * psrc_beg[hx1 + wx1 + l] -               \
			    9 * psrc_beg[hx1 + wx_1 + l] -               \
			    3 * psrc_beg[hx1 + wx2 + l]);                \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                     \
		    p = psrc_beg + hx2 + wx + l;                         \
		    p1 = psrc_beg + hx2 + wx1 + l;                       \
		    p2 = psrc_beg + hx2 + wx2 + l;                       \
		    i = (j);                                             \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +   \
			    l;                                           \
		    if (dh == 1) {                                       \
			a2 = 111 * (*p) + 29 * (*p1) - 9 * (*p_1) -      \
				3 * (*p2);                               \
			rez = (9 * (a + a1) - a_1 - a2) >> 11;           \
			if (rez > MAX)                                   \
			    rez = MAX;                                   \
			else if (rez < MIN)                              \
			    rez = MIN;                                   \
			*pd = rez;                                       \
			pd += dst_stride;                                \
			i++;                                             \
			a_1 = a;                                         \
			a = a1;                                          \
			a1 = a2;                                         \
			if (p < psrc_end) {                              \
			    p_1 += src_stride;                           \
			    p += src_stride;                             \
			    p1 += src_stride;                            \
			    p2 += src_stride;                            \
			}                                                \
		    }                                                    \
		    for (; i < (amount) - 2; i += 2) {                   \
			a2 = 111 * (*p) + 29 * (*p1) - 9 * (*p_1) -      \
				3 * (*p2);                               \
			rez1 = a >> 7;                                   \
			if (rez1 > MAX)                                  \
			    rez1 = MAX;                                  \
			else if (rez1 < MIN)                             \
			    rez1 = MIN;                                  \
			*pd = rez1;                                      \
			rez = (9 * (a + a1) - a_1 - a2) >> 11;           \
			if (rez > MAX)                                   \
			    rez = MAX;                                   \
			else if (rez < MIN)                              \
			    rez = MIN;                                   \
			*(pd + dst_stride) = rez;                        \
			a_1 = a;                                         \
			a = a1;                                          \
			a1 = a2;                                         \
			if (p < psrc_end) {                              \
			    p_1 += src_stride;                           \
			    p += src_stride;                             \
			    p1 += src_stride;                            \
			    p2 += src_stride;                            \
			}                                                \
			pd += (dst_stride << 1);                         \
		    }                                                    \
		    if (i < (amount)) {                                  \
			rez1 = a >> 7;                                   \
			if (rez1 > MAX)                                  \
			    rez1 = MAX;                                  \
			else if (rez1 < MIN)                             \
			    rez1 = MIN;                                  \
			*pd = rez1;                                      \
			pd += dst_stride;                                \
			i++;                                             \
		    }                                                    \
		    if (i < (amount)) {                                  \
			a2 = 111 * (*p) + 29 * (*p1) - 9 * (*p_1) -      \
				3 * (*p2);                               \
			rez = (9 * (a + a1) - a_1 - a2) >> 11;           \
			if (rez > MAX)                                   \
			    rez = MAX;                                   \
			else if (rez < MIN)                              \
			    rez = MIN;                                   \
			*pd = rez;                                       \
		    }                                                    \
		}                                                        \
	    } else {                                                     \
		for (l = 0; l < chan; l++) {                             \
		    a_1 = (29 * psrc_beg[hx_1 + wx + l] +                \
			    111 * psrc_beg[hx_1 + wx1 + l] -             \
			    3 * psrc_beg[hx_1 + wx_1 + l] -              \
			    9 * psrc_beg[hx_1 + wx2 + l]);               \
		    a = (29 * psrc_beg[hx + wx + l] +                    \
			    111 * psrc_beg[hx + wx1 + l] -               \
			    3 * psrc_beg[hx + wx_1 + l] -                \
			    9 * psrc_beg[hx + wx2 + l]);                 \
		    a1 = (29 * psrc_beg[hx1 + wx + l] +                  \
			    111 * psrc_beg[hx1 + wx1 + l] -              \
			    3 * psrc_beg[hx1 + wx_1 + l] -               \
			    9 * psrc_beg[hx1 + wx2 + l]);                \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                     \
		    p = psrc_beg + hx2 + wx + l;                         \
		    p1 = psrc_beg + hx2 + wx1 + l;                       \
		    p2 = psrc_beg + hx2 + wx2 + l;                       \
		    i = (j);                                             \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +   \
			    l;                                           \
		    if (dh == 1) {                                       \
			a2 = 29 * (*p) + 111 * (*p1) - 3 * (*p_1) -      \
				9 * (*p2);                               \
			rez = (9 * (a + a1) - a_1 - a2) >> 11;           \
			if (rez > MAX)                                   \
			    rez = MAX;                                   \
			else if (rez < MIN)                              \
			    rez = MIN;                                   \
			*pd = rez;                                       \
			pd += dst_stride;                                \
			a_1 = a;                                         \
			a = a1;                                          \
			a1 = a2;                                         \
			if (p < psrc_end) {                              \
			    p_1 += src_stride;                           \
			    p += src_stride;                             \
			    p1 += src_stride;                            \
			    p2 += src_stride;                            \
			}                                                \
			i++;                                             \
		    }                                                    \
		    for (; i < (amount) - 2; i += 2) {                   \
			a2 = 29 * (*p) + 111 * (*p1) - 3 * (*p_1) -      \
				9 * (*p2);                               \
			rez1 = a >> 7;                                   \
			if (rez1 > MAX)                                  \
			    rez1 = MAX;                                  \
			else if (rez1 < MIN)                             \
			    rez1 = MIN;                                  \
			*pd = rez1;                                      \
			rez = (9 * (a + a1) - a_1 - a2) >> 11;           \
			if (rez > MAX)                                   \
			    rez = MAX;                                   \
			else if (rez < MIN)                              \
			    rez = MIN;                                   \
			*(pd + dst_stride) = rez;                        \
			a_1 = a;                                         \
			a = a1;                                          \
			a1 = a2;                                         \
			if (p < psrc_end) {                              \
			    p_1 += src_stride;                           \
			    p += src_stride;                             \
			    p1 += src_stride;                            \
			    p2 += src_stride;                            \
			}                                                \
			pd += (dst_stride << 1);                         \
		    }                                                    \
		    if (i < (amount)) {                                  \
			rez1 = a >> 7;                                   \
			if (rez1 > MAX)                                  \
			    rez1 = MAX;                                  \
			else if (rez1 < MIN)                             \
			    rez1 = MIN;                                  \
			*pd = rez1;                                      \
			pd += dst_stride;                                \
			i++;                                             \
		    }                                                    \
		    if (i < (amount)) {                                  \
			a2 = 29 * (*p) + 111 * (*p1) - 3 * (*p_1) -      \
				9 * (*p2);                               \
			rez = (9 * (a + a1) - a_1 - a2) >> 11;           \
			if (rez > MAX)                                   \
			    rez = MAX;                                   \
			else if (rez < MIN)                              \
			    rez = MIN;                                   \
			*pd = rez;                                       \
		    }                                                    \
		}                                                        \
	    }                                                            \
	}

/* *********************************************************** */

#define	ROW_BICUBIC_FL2(beg_i, j, chan, amount, MIN, MAX, STYPE)        \
	{                                                               \
	    const STYPE *psrc_end;                                      \
	                                                                \
	    PREPARE_BICUBIC(beg_i, j, chan);                            \
	    psrc_end = psrc_beg + hx + (src_width_beg - 1) * chan;      \
	    if (dh == 0) {                                              \
		for (l = 0; l < chan; l++) {                            \
		    a_1 = (111 * psrc_beg[hx + wx_1 + l] +              \
			    29 * psrc_beg[hx1 + wx_1 + l] -             \
			    9 * psrc_beg[hx_1 + wx_1 + l] -             \
			    3 * psrc_beg[hx2 + wx_1 + l]);              \
		    a = (111 * psrc_beg[hx + wx + l] +                  \
			    29 * psrc_beg[hx1 + wx + l] -               \
			    9 * psrc_beg[hx_1 + wx + l] -               \
			    3 * psrc_beg[hx2 + wx + l]);                \
		    a1 = (111 * psrc_beg[hx + wx1 + l] +                \
			    29 * psrc_beg[hx1 + wx1 + l] -              \
			    9 * psrc_beg[hx_1 + wx1 + l] -              \
			    3 * psrc_beg[hx2 + wx1 + l]);               \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                    \
		    p = psrc_beg + hx + wx2 + l;                        \
		    p1 = psrc_beg + hx1 + wx2 + l;                      \
		    p2 = psrc_beg + hx2 + wx2 + l;                      \
		    i = (beg_i);                                        \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;   \
		    if (dw == 1) {                                      \
			a2 = 111 * (*p) + 29 * (*p1) - 9 * (*p_1) -     \
				3 * (*p2);                              \
			rez = (9 * (a + a1) - a_1 - a2) >> 11;          \
			if (rez > MAX)                                  \
			    rez = MAX;                                  \
			else if (rez < MIN)                             \
			    rez = MIN;                                  \
			*pd = rez;                                      \
			pd += chan;                                     \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			if (p < psrc_end) {                             \
			    p_1 += chan;                                \
			    p += chan;                                  \
			    p1 += chan;                                 \
			    p2 += chan;                                 \
			}                                               \
			i++;                                            \
		    }                                                   \
		    for (; i < (amount) - 2; i += 2) {                  \
			a2 = 111 * (*p) + 29 * (*p1) - 9 * (*p_1) -     \
				3 * (*p2);                              \
			rez1 = a >> 7;                                  \
			if (rez1 > MAX)                                 \
			    rez1 = MAX;                                 \
			else if (rez1 < MIN)                            \
			    rez1 = MIN;                                 \
			*pd = rez1;                                     \
			rez = (9 * (a + a1) - a_1 - a2) >> 11;          \
			if (rez > MAX)                                  \
			    rez = MAX;                                  \
			else if (rez < MIN)                             \
			    rez = MIN;                                  \
			*(pd + chan) = rez;                             \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			if (p < psrc_end) {                             \
			    p_1 += chan;                                \
			    p += chan;                                  \
			    p1 += chan;                                 \
			    p2 += chan;                                 \
			}                                               \
			pd += (chan << 1);                              \
		    }                                                   \
		    if (i < (amount)) {                                 \
			rez1 = a >> 7;                                  \
			if (rez1 > MAX)                                 \
			    rez1 = MAX;                                 \
			else if (rez1 < MIN)                            \
			    rez1 = MIN;                                 \
			*pd = rez1;                                     \
			pd += chan;                                     \
			i++;                                            \
		    }                                                   \
		    if (i < (amount)) {                                 \
			a2 = (111 * (*p) + 29 * (*p1) - 9 * (*p_1) -    \
				3 * (*p2));                             \
			rez = (9 * (a + a1) - a_1 - a2) >> 11;          \
			if (rez > MAX)                                  \
			    rez = MAX;                                  \
			else if (rez < MIN)                             \
			    rez = MIN;                                  \
			*pd = rez;                                      \
		    }                                                   \
		}                                                       \
	    } else {                                                    \
		for (l = 0; l < chan; l++) {                            \
		    a_1 = (29 * psrc_beg[hx + wx_1 + l] +               \
			    111 * psrc_beg[hx1 + wx_1 + l] -            \
			    3 * psrc_beg[hx_1 + wx_1 + l] -             \
			    9 * psrc_beg[hx2 + wx_1 + l]);              \
		    a = (29 * psrc_beg[hx + wx + l] +                   \
			    111 * psrc_beg[hx1 + wx + l] -              \
			    3 * psrc_beg[hx_1 + wx + l] -               \
			    9 * psrc_beg[hx2 + wx + l]);                \
		    a1 = (29 * psrc_beg[hx + wx1 + l] +                 \
			    111 * psrc_beg[hx1 + wx1 + l] -             \
			    3 * psrc_beg[hx_1 + wx1 + l] -              \
			    9 * psrc_beg[hx2 + wx1 + l]);               \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                    \
		    p = psrc_beg + hx + wx2 + l;                        \
		    p1 = psrc_beg + hx1 + wx2 + l;                      \
		    p2 = psrc_beg + hx2 + wx2 + l;                      \
		    i = (beg_i);                                        \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;   \
		    if (dw == 1) {                                      \
			a2 = 29 * (*p) + 111 * (*p1) - 3 * (*p_1) -     \
				9 * (*p2);                              \
			rez = (9 * (a + a1) - a_1 - a2) >> 11;          \
			if (rez > MAX)                                  \
			    rez = MAX;                                  \
			else if (rez < MIN)                             \
			    rez = MIN;                                  \
			*pd = rez;                                      \
			pd += chan;                                     \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			if (p < psrc_end) {                             \
			    p_1 += chan;                                \
			    p += chan;                                  \
			    p1 += chan;                                 \
			    p2 += chan;                                 \
			}                                               \
			i++;                                            \
		    }                                                   \
		    for (; i < (amount) - 2; i += 2) {                  \
			a2 = 29 * (*p) + 111 * (*p1) - 3 * (*p_1) -     \
				9 * (*p2);                              \
			rez1 = a >> 7;                                  \
			if (rez1 > MAX)                                 \
			    rez1 = MAX;                                 \
			else if (rez1 < MIN)                            \
			    rez1 = MIN;                                 \
			*pd = rez1;                                     \
			rez = (9 * (a + a1) - a_1 - a2) >> 11;          \
			if (rez > MAX)                                  \
			    rez = MAX;                                  \
			else if (rez < MIN)                             \
			    rez = MIN;                                  \
			*(pd + chan) = rez;                             \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			if (p < psrc_end) {                             \
			    p_1 += chan;                                \
			    p += chan;                                  \
			    p1 += chan;                                 \
			    p2 += chan;                                 \
			}                                               \
			pd += (chan << 1);                              \
		    }                                                   \
		    if (i < (amount)) {                                 \
			rez1 = a >> 7;                                  \
			if (rez1 > MAX)                                 \
			    rez1 = MAX;                                 \
			else if (rez1 < MIN)                            \
			    rez1 = MIN;                                 \
			*pd = rez1;                                     \
			pd += chan;                                     \
			i++;                                            \
		    }                                                   \
		    if (i < (amount)) {                                 \
			a2 = 29 * (*p) + 111 * (*p1) - 3 * (*p_1) -     \
				9 * (*p2);                              \
			rez = (9 * (a + a1) - a_1 - a2) >> 11;          \
			if (rez > MAX)                                  \
			    rez = MAX;                                  \
			else if (rez < MIN)                             \
			    rez = MIN;                                  \
			*pd = rez;                                      \
		    }                                                   \
		}                                                       \
	    }                                                           \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC_FL2(beg_i, j, chan, amount, MIN, MAX, STYPE)        \
	{                                                                  \
	    const STYPE *psrc_end;                                         \
	                                                                   \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    psrc_end =                                                     \
		    psrc_beg + wx + (src_height_beg - 1) * src_stride;     \
	    if (dw == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = psrc_beg[hx_1 + wx + l];                         \
		    a = psrc_beg[hx + wx + l];                             \
		    a1 = psrc_beg[hx1 + wx + l];                           \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 1) {                                         \
			a2 = (*p);                                         \
			rez = (29 * a + 111 * a1 - 3 * a_1 -               \
				9 * a2) >> 7;                              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
			pd += dst_stride;                                  \
			i++;                                               \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = (*p);                                         \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 7;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			rez = (29 * a + 111 * a1 - 3 * a_1 -               \
				9 * a2) >> 7;                              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + dst_stride) = rez;                          \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = (*p);                                         \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 7;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (29 * a + 111 * a1 - 3 * a_1 -               \
				9 * a2) >> 7;                              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = 9 * (psrc_beg[hx_1 + wx + l] + psrc_beg[hx_1 +   \
			    wx1 + l]) - psrc_beg[hx_1 + wx_1 + l] -        \
			    psrc_beg[hx_1 + wx2 + l];                      \
		    a = 9 * (psrc_beg[hx + wx + l] + psrc_beg[hx + wx1 +   \
			    l]) - psrc_beg[hx + wx_1 + l] -                \
			    psrc_beg[hx + wx2 + l];                        \
		    a1 = 9 * (psrc_beg[hx1 + wx + l] + psrc_beg[hx1 +      \
			    wx1 + l]) - psrc_beg[hx1 + wx_1 + l] -         \
			    psrc_beg[hx1 + wx2 + l];                       \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 1) {                                         \
			a2 = 9 * ((*p) + (*p1)) - (*p_1) - (*p2);          \
			rez = (29 * a + 111 * a1 - 3 * a_1 -               \
				9 * a2) >> 11;                             \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
			pd += dst_stride;                                  \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 9 * ((*p) + (*p1)) - (*p_1) - (*p2);          \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 11;                             \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			rez = (29 * a + 111 * a1 - 3 * a_1 -               \
				9 * a2) >> 11;                             \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + dst_stride) = rez;                          \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = 9 * ((*p) + (*p1)) - (*p_1) - (*p2);          \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 11;                             \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (29 * a + 111 * a1 - 3 * a_1 -               \
				9 * a2) >> 11;                             \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	ROW_BICUBIC_FL3(beg_i, j, chan, amount, MIN, MAX, STYPE)           \
	{                                                                  \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    if (dh == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = psrc_beg[hx + wx_1 + l];                         \
		    a = psrc_beg[hx + wx + l];                             \
		    a1 = psrc_beg[hx + wx1 + l];                           \
		    p = psrc_beg + hx + wx2 + l;                           \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 0) {                                         \
			*pd = a;                                           \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = *p;                                           \
			rez = (9 * (a + a1) - a_1 - a2) >> 4;              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
			*(pd + chan) = a1;                                 \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p += chan;                                         \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			if (p >= (psrc_beg + hx + l +                      \
				src_width_beg * chan))                     \
			    a2 = psrc_beg[hx + l + (src_width_beg -        \
				    1) * chan];                            \
			else                                               \
			    a2 = *p;                                       \
			rez = (9 * (a + a1) - a_1 - a2) >> 4;              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount))                                      \
			*pd = a1;                                          \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (9 * (psrc_beg[hx + wx_1 + l] +                  \
			    (mlib_s32)psrc_beg[hx1 + wx_1 + l]) -          \
			    psrc_beg[hx_1 + wx_1 + l] - psrc_beg[hx2 +     \
			    wx_1 + l]);                                    \
		    a = (9 * (psrc_beg[hx + wx + l] +                      \
			    (mlib_s32)psrc_beg[hx1 + wx + l]) -            \
			    psrc_beg[hx_1 + wx + l] - psrc_beg[hx2 +       \
			    wx + l]);                                      \
		    a1 = (9 * (psrc_beg[hx + wx1 + l] +                    \
			    (mlib_s32)psrc_beg[hx1 + wx1 + l]) -           \
			    psrc_beg[hx_1 + wx1 + l] - psrc_beg[hx2 +      \
			    wx1 + l]);                                     \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                       \
		    p = psrc_beg + hx + wx2 + l;                           \
		    p1 = psrc_beg + hx1 + wx2 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 0) {                                         \
			rez = a >> 4;                                      \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = (9 * (*p + (mlib_s32)(*p1)) - *p_1 - *p2);    \
			rez1 = (9 * (a + a1) - a_1 - a2) >> 8;             \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			rez = a1 >> 4;                                     \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + chan) = rez;                                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1 += chan;                                       \
			p += chan;                                         \
			p1 += chan;                                        \
			p2 += chan;                                        \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			if (p >= (psrc_beg + hx + l +                      \
				src_width_beg * chan))                     \
			    a2 = (9 * (psrc_beg[hx + l +                   \
				    (src_width_beg - 1) * chan] +          \
				    (mlib_s32)psrc_beg[hx1 + l +           \
				    (src_width_beg - 1) * chan]) -         \
				    psrc_beg[hx_1 + l + (src_width_beg -   \
				    1) * chan] - psrc_beg[hx2 + l +        \
				    (src_width_beg - 1) * chan]);          \
			else                                               \
			    a2 = (9 * (*p + (mlib_s32)(*p1)) - *p_1 -      \
				    *p2);                                  \
			rez1 = (9 * (a + a1) - a_1 - a2) >> 8;             \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = a1 >> 4;                                     \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC_FL3(beg_i, j, chan, amount, MIN, MAX, STYPE)        \
	{                                                                  \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    if (dw == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = psrc_beg[hx_1 + wx + l];                         \
		    a = psrc_beg[hx + wx + l];                             \
		    a1 = psrc_beg[hx1 + wx + l];                           \
		    p = psrc_beg + hx2 + wx + l;                           \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 0) {                                         \
			*pd = a;                                           \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = *p;                                           \
			rez = (9 * (a + a1) - a_1 - a2) >> 4;              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
			*(pd + dst_stride) = a1;                           \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p += src_stride;                                   \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			if (p >= (psrc_beg + wx + l +                      \
				src_height_beg * src_stride))              \
			    a2 = psrc_beg[wx + l + (src_height_beg -       \
				    1) * src_stride];                      \
			else                                               \
			    a2 = *p;                                       \
			rez = (9 * (a + a1) - a_1 - a2) >> 4;              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount))                                      \
			*pd = a1;                                          \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (9 * (psrc_beg[hx_1 + wx + l] +                  \
			    (mlib_s32)psrc_beg[hx_1 + wx1 + l]) -          \
			    psrc_beg[hx_1 + wx_1 + l] - psrc_beg[hx_1 +    \
			    wx2 + l]);                                     \
		    a = (9 * (psrc_beg[hx + wx + l] +                      \
			    (mlib_s32)psrc_beg[hx + wx1 + l]) -            \
			    psrc_beg[hx + wx_1 + l] - psrc_beg[hx +        \
			    wx2 + l]);                                     \
		    a1 = (9 * (psrc_beg[hx1 + wx + l] +                    \
			    (mlib_s32)psrc_beg[hx1 + wx1 + l]) -           \
			    psrc_beg[hx1 + wx_1 + l] - psrc_beg[hx1 +      \
			    wx2 + l]);                                     \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 0) {                                         \
			rez = a >> 4;                                      \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = (9 * (*p + (mlib_s32)(*p1)) - *p_1 - *p2);    \
			rez1 = (9 * (a + a1) - a_1 - a2) >> 8;             \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			rez = a1 >> 4;                                     \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + dst_stride) = rez;                          \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1 += src_stride;                                 \
			p += src_stride;                                   \
			p1 += src_stride;                                  \
			p2 += src_stride;                                  \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			if (p >= (psrc_beg + wx + l +                      \
				src_height_beg * src_stride))              \
			    a2 = (9 * (psrc_beg[wx + l +                   \
				    (src_height_beg - 1) * src_stride] +   \
				    (mlib_s32)psrc_beg[wx1 + l +           \
				    (src_height_beg -                      \
				    1) * src_stride]) - psrc_beg[wx_1 +    \
				    l + (src_height_beg -                  \
				    1) * src_stride] - psrc_beg[wx2 +      \
				    l + (src_height_beg -                  \
				    1) * src_stride]);                     \
			else                                               \
			    a2 = (9 * (*p + (mlib_s32)(*p1)) - *p_1 -      \
				    *p2);                                  \
			rez1 = (9 * (a + a1) - a_1 - a2) >> 8;             \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = a1 >> 4;                                     \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	ROW_BICUBIC2_FL0(beg_i, j, chan, amount, MIN, MAX, STYPE)       \
	{                                                               \
	    const STYPE *psrc_end;                                      \
	                                                                \
	    PREPARE_BICUBIC(beg_i, j, chan);                            \
	    psrc_end = psrc_beg + hx + (src_width_beg - 1) * chan;      \
	    if (dh == 0) {                                              \
		for (l = 0; l < chan; l++) {                            \
		    a_1 = (57 * psrc_beg[hx + wx_1 + l] +               \
			    19 * psrc_beg[hx1 + wx_1 + l] -             \
			    9 * psrc_beg[hx_1 + wx_1 + l] -             \
			    3 * psrc_beg[hx2 + wx_1 + l]);              \
		    a = (57 * psrc_beg[hx + wx + l] +                   \
			    19 * psrc_beg[hx1 + wx + l] -               \
			    9 * psrc_beg[hx_1 + wx + l] -               \
			    3 * psrc_beg[hx2 + wx + l]);                \
		    a1 = (57 * psrc_beg[hx + wx1 + l] +                 \
			    19 * psrc_beg[hx1 + wx1 + l] -              \
			    9 * psrc_beg[hx_1 + wx1 + l] -              \
			    3 * psrc_beg[hx2 + wx1 + l]);               \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                    \
		    p = psrc_beg + hx + wx2 + l;                        \
		    p1 = psrc_beg + hx1 + wx2 + l;                      \
		    p2 = psrc_beg + hx2 + wx2 + l;                      \
		    i = (beg_i);                                        \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;   \
		    if (dw == 1) {                                      \
			a2 = 57 * (*p) + 19 * (*p1) - 9 * (*p_1) -      \
				3 * (*p2);                              \
			rez = (19 * a + 57 * a1 - 3 * a_1 -             \
				9 * a2) >> 12;                          \
			if (rez > MAX)                                  \
			    rez = MAX;                                  \
			else if (rez < MIN)                             \
			    rez = MIN;                                  \
			*pd = rez;                                      \
			pd += chan;                                     \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			if (p < psrc_end) {                             \
			    p_1 += chan;                                \
			    p += chan;                                  \
			    p1 += chan;                                 \
			    p2 += chan;                                 \
			}                                               \
			i++;                                            \
		    }                                                   \
		    for (; i < (amount) - 2; i += 2) {                  \
			a2 = 57 * (*p) + 19 * (*p1) - 9 * (*p_1) -      \
				3 * (*p2);                              \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -            \
				3 * a2) >> 12;                          \
			if (rez1 > MAX)                                 \
			    rez1 = MAX;                                 \
			else if (rez1 < MIN)                            \
			    rez1 = MIN;                                 \
			*pd = rez1;                                     \
			rez = (19 * a + 57 * a1 - 3 * a_1 -             \
				9 * a2) >> 12;                          \
			if (rez > MAX)                                  \
			    rez = MAX;                                  \
			else if (rez < MIN)                             \
			    rez = MIN;                                  \
			*(pd + chan) = rez;                             \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			if (p < psrc_end) {                             \
			    p_1 += chan;                                \
			    p += chan;                                  \
			    p1 += chan;                                 \
			    p2 += chan;                                 \
			}                                               \
			pd += (chan << 1);                              \
		    }                                                   \
		    if (i < (amount)) {                                 \
			a2 = (57 * (*p) + 19 * (*p1) - 9 * (*p_1) -     \
				3 * (*p2));                             \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -            \
				3 * a2) >> 12;                          \
			if (rez1 > MAX)                                 \
			    rez1 = MAX;                                 \
			else if (rez1 < MIN)                            \
			    rez1 = MIN;                                 \
			*pd = rez1;                                     \
			pd += chan;                                     \
			i++;                                            \
		    }                                                   \
		    if (i < (amount)) {                                 \
			rez = (19 * a + 57 * a1 - 3 * a_1 -             \
				9 * a2) >> 12;                          \
			if (rez > MAX)                                  \
			    rez = MAX;                                  \
			else if (rez < MIN)                             \
			    rez = MIN;                                  \
			*pd = rez;                                      \
		    }                                                   \
		}                                                       \
	    } else {                                                    \
		for (l = 0; l < chan; l++) {                            \
		    a_1 = (19 * psrc_beg[hx + wx_1 + l] +               \
			    57 * psrc_beg[hx1 + wx_1 + l] -             \
			    3 * psrc_beg[hx_1 + wx_1 + l] -             \
			    9 * psrc_beg[hx2 + wx_1 + l]);              \
		    a = (19 * psrc_beg[hx + wx + l] +                   \
			    57 * psrc_beg[hx1 + wx + l] -               \
			    3 * psrc_beg[hx_1 + wx + l] -               \
			    9 * psrc_beg[hx2 + wx + l]);                \
		    a1 = (19 * psrc_beg[hx + wx1 + l] +                 \
			    57 * psrc_beg[hx1 + wx1 + l] -              \
			    3 * psrc_beg[hx_1 + wx1 + l] -              \
			    9 * psrc_beg[hx2 + wx1 + l]);               \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                    \
		    p = psrc_beg + hx + wx2 + l;                        \
		    p1 = psrc_beg + hx1 + wx2 + l;                      \
		    p2 = psrc_beg + hx2 + wx2 + l;                      \
		    i = (beg_i);                                        \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;   \
		    if (dw == 1) {                                      \
			a2 = 19 * (*p) + 57 * (*p1) - 3 * (*p_1) -      \
				9 * (*p2);                              \
			rez = (19 * a + 57 * a1 - 3 * a_1 -             \
				9 * a2) >> 12;                          \
			if (rez > MAX)                                  \
			    rez = MAX;                                  \
			else if (rez < MIN)                             \
			    rez = MIN;                                  \
			*pd = rez;                                      \
			pd += chan;                                     \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			if (p < psrc_end) {                             \
			    p_1 += chan;                                \
			    p += chan;                                  \
			    p1 += chan;                                 \
			    p2 += chan;                                 \
			}                                               \
			i++;                                            \
		    }                                                   \
		    for (; i < (amount) - 2; i += 2) {                  \
			a2 = 19 * (*p) + 57 * (*p1) - 3 * (*p_1) -      \
				9 * (*p2);                              \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -            \
				3 * a2) >> 12;                          \
			if (rez1 > MAX)                                 \
			    rez1 = MAX;                                 \
			else if (rez1 < MIN)                            \
			    rez1 = MIN;                                 \
			*pd = rez1;                                     \
			rez = (19 * a + 57 * a1 - 3 * a_1 -             \
				9 * a2) >> 12;                          \
			if (rez > MAX)                                  \
			    rez = MAX;                                  \
			else if (rez < MIN)                             \
			    rez = MIN;                                  \
			*(pd + chan) = rez;                             \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			if (p < psrc_end) {                             \
			    p_1 += chan;                                \
			    p += chan;                                  \
			    p1 += chan;                                 \
			    p2 += chan;                                 \
			}                                               \
			pd += (chan << 1);                              \
		    }                                                   \
		    if (i < (amount)) {                                 \
			a2 = 19 * (*p) + 57 * (*p1) - 3 * (*p_1) -      \
				9 * (*p2);                              \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -            \
				3 * a2) >> 12;                          \
			if (rez1 > MAX)                                 \
			    rez1 = MAX;                                 \
			else if (rez1 < MIN)                            \
			    rez1 = MIN;                                 \
			*pd = rez1;                                     \
			pd += chan;                                     \
			i++;                                            \
		    }                                                   \
		    if (i < (amount)) {                                 \
			rez = (19 * a + 57 * a1 - 3 * a_1 -             \
				9 * a2) >> 12;                          \
			if (rez > MAX)                                  \
			    rez = MAX;                                  \
			else if (rez < MIN)                             \
			    rez = MIN;                                  \
			*pd = rez;                                      \
		    }                                                   \
		}                                                       \
	    }                                                           \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC2_FL0(beg_i, j, chan, amount, MIN, MAX, STYPE)       \
	{                                                                  \
	    const STYPE *psrc_end;                                         \
	                                                                   \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    psrc_end =                                                     \
		    psrc_beg + wx + (src_height_beg - 1) * src_stride;     \
	    if (dw == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (57 * psrc_beg[hx_1 + wx + l] +                  \
			    19 * psrc_beg[hx_1 + wx1 + l] -                \
			    9 * psrc_beg[hx_1 + wx_1 + l] -                \
			    3 * psrc_beg[hx_1 + wx2 + l]);                 \
		    a = (57 * psrc_beg[hx + wx + l] + 19 * psrc_beg[hx +   \
			    wx1 + l] - 9 * psrc_beg[hx + wx_1 + l] -       \
			    3 * psrc_beg[hx + wx2 + l]);                   \
		    a1 = (57 * psrc_beg[hx1 + wx + l] +                    \
			    19 * psrc_beg[hx1 + wx1 + l] -                 \
			    9 * psrc_beg[hx1 + wx_1 + l] -                 \
			    3 * psrc_beg[hx1 + wx2 + l]);                  \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 1) {                                         \
			a2 = 57 * (*p) + 19 * (*p1) - 9 * (*p_1) -         \
				3 * (*p2);                                 \
			rez = (19 * a + 57 * a1 - 3 * a_1 -                \
				9 * a2) >> 12;                             \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
			pd += dst_stride;                                  \
			i++;                                               \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 57 * (*p) + 19 * (*p1) - 9 * (*p_1) -         \
				3 * (*p2);                                 \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -               \
				3 * a2) >> 12;                             \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			rez = (19 * a + 57 * a1 - 3 * a_1 -                \
				9 * a2) >> 12;                             \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + dst_stride) = rez;                          \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = 57 * (*p) + 19 * (*p1) - 9 * (*p_1) -         \
				3 * (*p2);                                 \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -               \
				3 * a2) >> 12;                             \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (19 * a + 57 * a1 - 3 * a_1 -                \
				9 * a2) >> 12;                             \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (19 * psrc_beg[hx_1 + wx + l] +                  \
			    57 * psrc_beg[hx_1 + wx1 + l] -                \
			    3 * psrc_beg[hx_1 + wx_1 + l] -                \
			    9 * psrc_beg[hx_1 + wx2 + l]);                 \
		    a = (19 * psrc_beg[hx + wx + l] + 57 * psrc_beg[hx +   \
			    wx1 + l] - 3 * psrc_beg[hx + wx_1 + l] -       \
			    9 * psrc_beg[hx + wx2 + l]);                   \
		    a1 = (19 * psrc_beg[hx1 + wx + l] +                    \
			    57 * psrc_beg[hx1 + wx1 + l] -                 \
			    3 * psrc_beg[hx1 + wx_1 + l] -                 \
			    9 * psrc_beg[hx1 + wx2 + l]);                  \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 1) {                                         \
			a2 = 19 * (*p) + 57 * (*p1) - 3 * (*p_1) -         \
				9 * (*p2);                                 \
			rez = (19 * a + 57 * a1 - 3 * a_1 -                \
				9 * a2) >> 12;                             \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
			pd += dst_stride;                                  \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 19 * (*p) + 57 * (*p1) - 3 * (*p_1) -         \
				9 * (*p2);                                 \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -               \
				3 * a2) >> 12;                             \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			rez = (19 * a + 57 * a1 - 3 * a_1 -                \
				9 * a2) >> 12;                             \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + dst_stride) = rez;                          \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = 19 * (*p) + 57 * (*p1) - 3 * (*p_1) -         \
				9 * (*p2);                                 \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -               \
				3 * a2) >> 12;                             \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (19 * a + 57 * a1 - 3 * a_1 -                \
				9 * a2) >> 12;                             \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	ROW_BICUBIC2_FL1(beg_i, j, chan, amount, MIN, MAX, STYPE)          \
	{                                                                  \
	    const STYPE *psrc_end;                                         \
	                                                                   \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    psrc_end = psrc_beg + hx + (src_width_beg - 1) * chan;         \
	    if (dh == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = psrc_beg[hx + wx_1 + l];                         \
		    a = psrc_beg[hx + wx + l];                             \
		    a1 = psrc_beg[hx + wx1 + l];                           \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                       \
		    p = psrc_beg + hx + wx2 + l;                           \
		    p1 = psrc_beg + hx1 + wx2 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 1) {                                         \
			a2 = (*p);                                         \
			rez = (19 * a + 57 * a1 - 3 * a_1 -                \
				9 * a2) >> 6;                              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
			pd += chan;                                        \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = (*p);                                         \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -               \
				3 * a2) >> 6;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			rez = (19 * a + 57 * a1 - 3 * a_1 -                \
				9 * a2) >> 6;                              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + chan) = rez;                                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = (*p);                                         \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -               \
				3 * a2) >> 6;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (19 * a + 57 * a1 - 3 * a_1 -                \
				9 * a2) >> 6;                              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = 5 * (psrc_beg[hx + wx_1 + l] + psrc_beg[hx1 +    \
			    wx_1 + l]) - psrc_beg[hx_1 + wx_1 + l] -       \
			    psrc_beg[hx2 + wx_1 + l];                      \
		    a = 5 * (psrc_beg[hx + wx + l] + psrc_beg[hx1 + wx +   \
			    l]) - psrc_beg[hx_1 + wx + l] -                \
			    psrc_beg[hx2 + wx + l];                        \
		    a1 = 5 * (psrc_beg[hx + wx1 + l] + psrc_beg[hx1 +      \
			    wx1 + l]) - psrc_beg[hx_1 + wx1 + l] -         \
			    psrc_beg[hx2 + wx1 + l];                       \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                       \
		    p = psrc_beg + hx + wx2 + l;                           \
		    p1 = psrc_beg + hx1 + wx2 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 1) {                                         \
			a2 = 5 * ((*p) + (*p1)) - (*p_1) - (*p2);          \
			rez = (19 * a + 57 * a1 - 3 * a_1 -                \
				9 * a2) >> 9;                              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
			pd += chan;                                        \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 5 * ((*p) + (*p1)) - (*p_1) - (*p2);          \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -               \
				3 * a2) >> 9;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			rez = (19 * a + 57 * a1 - 3 * a_1 -                \
				9 * a2) >> 9;                              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + chan) = rez;                                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = 5 * ((*p) + (*p1)) - (*p_1) - (*p2);          \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -               \
				3 * a2) >> 9;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (19 * a + 57 * a1 - 3 * a_1 -                \
				9 * a2) >> 9;                              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC2_FL1(beg_i, j, chan, amount, MIN, MAX, STYPE)       \
	{                                                                  \
	    const STYPE *psrc_end;                                         \
	                                                                   \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    psrc_end =                                                     \
		    psrc_beg + wx + (src_height_beg - 1) * src_stride;     \
	    if (dw == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (57 * psrc_beg[hx_1 + wx + l] +                  \
			    19 * psrc_beg[hx_1 + wx1 + l] -                \
			    9 * psrc_beg[hx_1 + wx_1 + l] -                \
			    3 * psrc_beg[hx_1 + wx2 + l]);                 \
		    a = (57 * psrc_beg[hx + wx + l] + 19 * psrc_beg[hx +   \
			    wx1 + l] - 9 * psrc_beg[hx + wx_1 + l] -       \
			    3 * psrc_beg[hx + wx2 + l]);                   \
		    a1 = (57 * psrc_beg[hx1 + wx + l] +                    \
			    19 * psrc_beg[hx1 + wx1 + l] -                 \
			    9 * psrc_beg[hx1 + wx_1 + l] -                 \
			    3 * psrc_beg[hx1 + wx2 + l]);                  \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 1) {                                         \
			a2 = 57 * (*p) + 19 * (*p1) - 9 * (*p_1) -         \
				3 * (*p2);                                 \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
			pd += dst_stride;                                  \
			i++;                                               \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 57 * (*p) + 19 * (*p1) - 9 * (*p_1) -         \
				3 * (*p2);                                 \
			rez1 = a >> 6;                                     \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + dst_stride) = rez;                          \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez1 = a >> 6;                                     \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = 57 * (*p) + 19 * (*p1) - 9 * (*p_1) -         \
				3 * (*p2);                                 \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (19 * psrc_beg[hx_1 + wx + l] +                  \
			    57 * psrc_beg[hx_1 + wx1 + l] -                \
			    3 * psrc_beg[hx_1 + wx_1 + l] -                \
			    9 * psrc_beg[hx_1 + wx2 + l]);                 \
		    a = (19 * psrc_beg[hx + wx + l] + 57 * psrc_beg[hx +   \
			    wx1 + l] - 3 * psrc_beg[hx + wx_1 + l] -       \
			    9 * psrc_beg[hx + wx2 + l]);                   \
		    a1 = (19 * psrc_beg[hx1 + wx + l] +                    \
			    57 * psrc_beg[hx1 + wx1 + l] -                 \
			    3 * psrc_beg[hx1 + wx_1 + l] -                 \
			    9 * psrc_beg[hx1 + wx2 + l]);                  \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 1) {                                         \
			a2 = 19 * (*p) + 57 * (*p1) - 3 * (*p_1) -         \
				9 * (*p2);                                 \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
			pd += dst_stride;                                  \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 19 * (*p) + 57 * (*p1) - 3 * (*p_1) -         \
				9 * (*p2);                                 \
			rez1 = a >> 6;                                     \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + dst_stride) = rez;                          \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez1 = a >> 6;                                     \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = 19 * (*p) + 57 * (*p1) - 3 * (*p_1) -         \
				9 * (*p2);                                 \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	ROW_BICUBIC2_FL2(beg_i, j, chan, amount, MIN, MAX, STYPE)       \
	{                                                               \
	    const STYPE *psrc_end;                                      \
	                                                                \
	    PREPARE_BICUBIC(beg_i, j, chan);                            \
	    psrc_end = psrc_beg + hx + (src_width_beg - 1) * chan;      \
	    if (dh == 0) {                                              \
		for (l = 0; l < chan; l++) {                            \
		    a_1 = (57 * psrc_beg[hx + wx_1 + l] +               \
			    19 * psrc_beg[hx1 + wx_1 + l] -             \
			    9 * psrc_beg[hx_1 + wx_1 + l] -             \
			    3 * psrc_beg[hx2 + wx_1 + l]);              \
		    a = (57 * psrc_beg[hx + wx + l] +                   \
			    19 * psrc_beg[hx1 + wx + l] -               \
			    9 * psrc_beg[hx_1 + wx + l] -               \
			    3 * psrc_beg[hx2 + wx + l]);                \
		    a1 = (57 * psrc_beg[hx + wx1 + l] +                 \
			    19 * psrc_beg[hx1 + wx1 + l] -              \
			    9 * psrc_beg[hx_1 + wx1 + l] -              \
			    3 * psrc_beg[hx2 + wx1 + l]);               \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                    \
		    p = psrc_beg + hx + wx2 + l;                        \
		    p1 = psrc_beg + hx1 + wx2 + l;                      \
		    p2 = psrc_beg + hx2 + wx2 + l;                      \
		    i = (beg_i);                                        \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;   \
		    if (dw == 1) {                                      \
			a2 = 57 * (*p) + 19 * (*p1) - 9 * (*p_1) -      \
				3 * (*p2);                              \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;           \
			if (rez > MAX)                                  \
			    rez = MAX;                                  \
			else if (rez < MIN)                             \
			    rez = MIN;                                  \
			*pd = rez;                                      \
			pd += chan;                                     \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			if (p < psrc_end) {                             \
			    p_1 += chan;                                \
			    p += chan;                                  \
			    p1 += chan;                                 \
			    p2 += chan;                                 \
			}                                               \
			i++;                                            \
		    }                                                   \
		    for (; i < (amount) - 2; i += 2) {                  \
			a2 = 57 * (*p) + 19 * (*p1) - 9 * (*p_1) -      \
				3 * (*p2);                              \
			rez1 = a >> 6;                                  \
			if (rez1 > MAX)                                 \
			    rez1 = MAX;                                 \
			else if (rez1 < MIN)                            \
			    rez1 = MIN;                                 \
			*pd = rez1;                                     \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;           \
			if (rez > MAX)                                  \
			    rez = MAX;                                  \
			else if (rez < MIN)                             \
			    rez = MIN;                                  \
			*(pd + chan) = rez;                             \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			if (p < psrc_end) {                             \
			    p_1 += chan;                                \
			    p += chan;                                  \
			    p1 += chan;                                 \
			    p2 += chan;                                 \
			}                                               \
			pd += (chan << 1);                              \
		    }                                                   \
		    if (i < (amount)) {                                 \
			rez1 = a >> 6;                                  \
			if (rez1 > MAX)                                 \
			    rez1 = MAX;                                 \
			else if (rez1 < MIN)                            \
			    rez1 = MIN;                                 \
			*pd = rez1;                                     \
			pd += chan;                                     \
			i++;                                            \
		    }                                                   \
		    if (i < (amount)) {                                 \
			a2 = (57 * (*p) + 19 * (*p1) - 9 * (*p_1) -     \
				3 * (*p2));                             \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;           \
			if (rez > MAX)                                  \
			    rez = MAX;                                  \
			else if (rez < MIN)                             \
			    rez = MIN;                                  \
			*pd = rez;                                      \
		    }                                                   \
		}                                                       \
	    } else {                                                    \
		for (l = 0; l < chan; l++) {                            \
		    a_1 = (19 * psrc_beg[hx + wx_1 + l] +               \
			    57 * psrc_beg[hx1 + wx_1 + l] -             \
			    3 * psrc_beg[hx_1 + wx_1 + l] -             \
			    9 * psrc_beg[hx2 + wx_1 + l]);              \
		    a = (19 * psrc_beg[hx + wx + l] +                   \
			    57 * psrc_beg[hx1 + wx + l] -               \
			    3 * psrc_beg[hx_1 + wx + l] -               \
			    9 * psrc_beg[hx2 + wx + l]);                \
		    a1 = (19 * psrc_beg[hx + wx1 + l] +                 \
			    57 * psrc_beg[hx1 + wx1 + l] -              \
			    3 * psrc_beg[hx_1 + wx1 + l] -              \
			    9 * psrc_beg[hx2 + wx1 + l]);               \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                    \
		    p = psrc_beg + hx + wx2 + l;                        \
		    p1 = psrc_beg + hx1 + wx2 + l;                      \
		    p2 = psrc_beg + hx2 + wx2 + l;                      \
		    i = (beg_i);                                        \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;   \
		    if (dw == 1) {                                      \
			a2 = 19 * (*p) + 57 * (*p1) - 3 * (*p_1) -      \
				9 * (*p2);                              \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;           \
			if (rez > MAX)                                  \
			    rez = MAX;                                  \
			else if (rez < MIN)                             \
			    rez = MIN;                                  \
			*pd = rez;                                      \
			pd += chan;                                     \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			if (p < psrc_end) {                             \
			    p_1 += chan;                                \
			    p += chan;                                  \
			    p1 += chan;                                 \
			    p2 += chan;                                 \
			}                                               \
			i++;                                            \
		    }                                                   \
		    for (; i < (amount) - 2; i += 2) {                  \
			a2 = 19 * (*p) + 57 * (*p1) - 3 * (*p_1) -      \
				9 * (*p2);                              \
			rez1 = a >> 6;                                  \
			if (rez1 > MAX)                                 \
			    rez1 = MAX;                                 \
			else if (rez1 < MIN)                            \
			    rez1 = MIN;                                 \
			*pd = rez1;                                     \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;           \
			if (rez > MAX)                                  \
			    rez = MAX;                                  \
			else if (rez < MIN)                             \
			    rez = MIN;                                  \
			*(pd + chan) = rez;                             \
			a_1 = a;                                        \
			a = a1;                                         \
			a1 = a2;                                        \
			if (p < psrc_end) {                             \
			    p_1 += chan;                                \
			    p += chan;                                  \
			    p1 += chan;                                 \
			    p2 += chan;                                 \
			}                                               \
			pd += (chan << 1);                              \
		    }                                                   \
		    if (i < (amount)) {                                 \
			rez1 = a >> 6;                                  \
			if (rez1 > MAX)                                 \
			    rez1 = MAX;                                 \
			else if (rez1 < MIN)                            \
			    rez1 = MIN;                                 \
			*pd = rez1;                                     \
			pd += chan;                                     \
			i++;                                            \
		    }                                                   \
		    if (i < (amount)) {                                 \
			a2 = 19 * (*p) + 57 * (*p1) - 3 * (*p_1) -      \
				9 * (*p2);                              \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;           \
			if (rez > MAX)                                  \
			    rez = MAX;                                  \
			else if (rez < MIN)                             \
			    rez = MIN;                                  \
			*pd = rez;                                      \
		    }                                                   \
		}                                                       \
	    }                                                           \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC2_FL2(beg_i, j, chan, amount, MIN, MAX, STYPE)       \
	{                                                                  \
	    const STYPE *psrc_end;                                         \
	                                                                   \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    psrc_end =                                                     \
		    psrc_beg + wx + (src_height_beg - 1) * src_stride;     \
	    if (dw == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = psrc_beg[hx_1 + wx + l];                         \
		    a = psrc_beg[hx + wx + l];                             \
		    a1 = psrc_beg[hx1 + wx + l];                           \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 1) {                                         \
			a2 = (*p);                                         \
			rez = (19 * a + 57 * a1 - 3 * a_1 -                \
				9 * a2) >> 6;                              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
			pd += dst_stride;                                  \
			i++;                                               \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = (*p);                                         \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -               \
				3 * a2) >> 6;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			rez = (19 * a + 57 * a1 - 3 * a_1 -                \
				9 * a2) >> 6;                              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + dst_stride) = rez;                          \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = (*p);                                         \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -               \
				3 * a2) >> 6;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (19 * a + 57 * a1 - 3 * a_1 -                \
				9 * a2) >> 6;                              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = 5 * (psrc_beg[hx_1 + wx + l] + psrc_beg[hx_1 +   \
			    wx1 + l]) - psrc_beg[hx_1 + wx_1 + l] -        \
			    psrc_beg[hx_1 + wx2 + l];                      \
		    a = 5 * (psrc_beg[hx + wx + l] + psrc_beg[hx + wx1 +   \
			    l]) - psrc_beg[hx + wx_1 + l] -                \
			    psrc_beg[hx + wx2 + l];                        \
		    a1 = 5 * (psrc_beg[hx1 + wx + l] + psrc_beg[hx1 +      \
			    wx1 + l]) - psrc_beg[hx1 + wx_1 + l] -         \
			    psrc_beg[hx1 + wx2 + l];                       \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 1) {                                         \
			a2 = 5 * ((*p) + (*p1)) - (*p_1) - (*p2);          \
			rez = (19 * a + 57 * a1 - 3 * a_1 -                \
				9 * a2) >> 9;                              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
			pd += dst_stride;                                  \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 5 * ((*p) + (*p1)) - (*p_1) - (*p2);          \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -               \
				3 * a2) >> 9;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			rez = (19 * a + 57 * a1 - 3 * a_1 -                \
				9 * a2) >> 9;                              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + dst_stride) = rez;                          \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = 5 * ((*p) + (*p1)) - (*p_1) - (*p2);          \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -               \
				3 * a2) >> 9;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (19 * a + 57 * a1 - 3 * a_1 -                \
				9 * a2) >> 9;                              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	ROW_BICUBIC2_FL3(beg_i, j, chan, amount, MIN, MAX, STYPE)          \
	{                                                                  \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    if (dh == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = psrc_beg[hx + wx_1 + l];                         \
		    a = psrc_beg[hx + wx + l];                             \
		    a1 = psrc_beg[hx + wx1 + l];                           \
		    p = psrc_beg + hx + wx2 + l;                           \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 0) {                                         \
			*pd = a;                                           \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = *p;                                           \
			rez = (5 * (a + a1) - a_1 - a2) >> 3;              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
			*(pd + chan) = a1;                                 \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p += chan;                                         \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			if (p >= (psrc_beg + hx + l +                      \
				src_width_beg * chan))                     \
			    a2 = psrc_beg[hx + l + (src_width_beg -        \
				    1) * chan];                            \
			else                                               \
			    a2 = *p;                                       \
			rez = (5 * (a + a1) - a_1 - a2) >> 3;              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount))                                      \
			*pd = a1;                                          \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (5 * (psrc_beg[hx + wx_1 + l] +                  \
			    (mlib_s32)psrc_beg[hx1 + wx_1 + l]) -          \
			    psrc_beg[hx_1 + wx_1 + l] - psrc_beg[hx2 +     \
			    wx_1 + l]);                                    \
		    a = (5 * (psrc_beg[hx + wx + l] +                      \
			    (mlib_s32)psrc_beg[hx1 + wx + l]) -            \
			    psrc_beg[hx_1 + wx + l] - psrc_beg[hx2 +       \
			    wx + l]);                                      \
		    a1 = (5 * (psrc_beg[hx + wx1 + l] +                    \
			    (mlib_s32)psrc_beg[hx1 + wx1 + l]) -           \
			    psrc_beg[hx_1 + wx1 + l] - psrc_beg[hx2 +      \
			    wx1 + l]);                                     \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                       \
		    p = psrc_beg + hx + wx2 + l;                           \
		    p1 = psrc_beg + hx1 + wx2 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 0) {                                         \
			rez = a >> 3;                                      \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = (5 * (*p + (mlib_s32)(*p1)) - *p_1 - *p2);    \
			rez1 = (5 * (a + a1) - a_1 - a2) >> 6;             \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			rez = a1 >> 3;                                     \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + chan) = rez;                                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1 += chan;                                       \
			p += chan;                                         \
			p1 += chan;                                        \
			p2 += chan;                                        \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			if (p >= (psrc_beg + hx + l +                      \
				src_width_beg * chan))                     \
			    a2 = (5 * (psrc_beg[hx + l +                   \
				    (src_width_beg - 1) * chan] +          \
				    (mlib_s32)psrc_beg[hx1 + l +           \
				    (src_width_beg - 1) * chan]) -         \
				    psrc_beg[hx_1 + l + (src_width_beg -   \
				    1) * chan] - psrc_beg[hx2 + l +        \
				    (src_width_beg - 1) * chan]);          \
			else                                               \
			    a2 = (5 * (*p + (mlib_s32)(*p1)) - *p_1 -      \
				    *p2);                                  \
			rez1 = (5 * (a + a1) - a_1 - a2) >> 6;             \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = a1 >> 3;                                     \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC2_FL3(beg_i, j, chan, amount, MIN, MAX, STYPE)       \
	{                                                                  \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    if (dw == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = psrc_beg[hx_1 + wx + l];                         \
		    a = psrc_beg[hx + wx + l];                             \
		    a1 = psrc_beg[hx1 + wx + l];                           \
		    p = psrc_beg + hx2 + wx + l;                           \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 0) {                                         \
			*pd = a;                                           \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = *p;                                           \
			rez = (5 * (a + a1) - a_1 - a2) >> 3;              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
			*(pd + dst_stride) = a1;                           \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p += src_stride;                                   \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			if (p >= (psrc_beg + wx + l +                      \
				src_height_beg * src_stride))              \
			    a2 = psrc_beg[wx + l + (src_height_beg -       \
				    1) * src_stride];                      \
			else                                               \
			    a2 = *p;                                       \
			rez = (5 * (a + a1) - a_1 - a2) >> 3;              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount))                                      \
			*pd = a1;                                          \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (5 * (psrc_beg[hx_1 + wx + l] +                  \
			    (mlib_s32)psrc_beg[hx_1 + wx1 + l]) -          \
			    psrc_beg[hx_1 + wx_1 + l] - psrc_beg[hx_1 +    \
			    wx2 + l]);                                     \
		    a = (5 * (psrc_beg[hx + wx + l] +                      \
			    (mlib_s32)psrc_beg[hx + wx1 + l]) -            \
			    psrc_beg[hx + wx_1 + l] - psrc_beg[hx +        \
			    wx2 + l]);                                     \
		    a1 = (5 * (psrc_beg[hx1 + wx + l] +                    \
			    (mlib_s32)psrc_beg[hx1 + wx1 + l]) -           \
			    psrc_beg[hx1 + wx_1 + l] - psrc_beg[hx1 +      \
			    wx2 + l]);                                     \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 0) {                                         \
			rez = a >> 3;                                      \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = (5 * (*p + (mlib_s32)(*p1)) - *p_1 - *p2);    \
			rez1 = (5 * (a + a1) - a_1 - a2) >> 6;             \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			rez = a1 >> 3;                                     \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + dst_stride) = rez;                          \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1 += src_stride;                                 \
			p += src_stride;                                   \
			p1 += src_stride;                                  \
			p2 += src_stride;                                  \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			if (p >= (psrc_beg + wx + l +                      \
				src_height_beg * src_stride))              \
			    a2 = (5 * (psrc_beg[wx + l +                   \
				    (src_height_beg - 1) * src_stride] +   \
				    (mlib_s32)psrc_beg[wx1 + l +           \
				    (src_height_beg -                      \
				    1) * src_stride]) - psrc_beg[wx_1 +    \
				    l + (src_height_beg -                  \
				    1) * src_stride] - psrc_beg[wx2 +      \
				    l + (src_height_beg -                  \
				    1) * src_stride]);                     \
			else                                               \
			    a2 = (5 * (*p + (mlib_s32)(*p1)) - *p_1 -      \
				    *p2);                                  \
			rez1 = (5 * (a + a1) - a_1 - a2) >> 6;             \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = rez1;                                        \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = a1 >> 3;                                     \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = rez;                                         \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	FUNC_ZOOM_CE(TT, STYPE)                                           \
	void                                                              \
	mlib_ImageZoomIn2X_##TT##_CE(MLIB_ZOOMIN2XDEFEDGE                 \
		(STYPE))                                                  \
	{                                                                 \
	    mlib_s32 i, j;                                                \
	                                                                  \
	    while (((pdst - pdst_near) >= dst_stride) &&                  \
		    (src_height_near > 0)) {                              \
	                                                                  \
/*                                                                        \
 * upper bound                                                            \
 */                                                                       \
		for (i = 0; i < src_width_near * channels; i++)           \
		    pdst_near[i] = 0;                                     \
		pdst_near += dst_stride;                                  \
		src_height_near--;                                        \
	    }                                                             \
	    while (src_height_near > src_height) {                        \
	                                                                  \
/*                                                                        \
 * lower bound of image                                                   \
 */                                                                       \
		for (i = 0; i < src_width_near * channels; i++) {         \
		    pdst_near[i + (src_height_near - 1) * dst_stride] =   \
			    0;                                            \
		}                                                         \
		src_height_near--;                                        \
	    }                                                             \
	    while ((pdst - pdst_near > 0) && (src_width_near > 0)) {      \
	                                                                  \
/*                                                                        \
 * left bound of image                                                    \
 */                                                                       \
		for (i = 0; i < channels; i++) {                          \
		    for (j = 0; j < src_height_near; j++)                 \
			pdst_near[i + j * dst_stride] = 0;                \
		}                                                         \
		src_width_near--;                                         \
		pdst_near += channels;                                    \
	    }                                                             \
	    while (src_width_near > src_width) {                          \
	                                                                  \
/*                                                                        \
 * right bound of image                                                   \
 */                                                                       \
		for (i = 0; i < channels; i++) {                          \
		    for (j = 0; j < src_height_near; j++) {               \
			pdst_near[i + (src_width_near - 1) * channels +   \
				j * dst_stride] = 0;                      \
		    }                                                     \
		}                                                         \
		src_width_near--;                                         \
	    }                                                             \
	}

/* *********************************************************** */

#define	FUNC_ZOOM_ON(TT, STYPE)                                            \
	void                                                               \
	mlib_ImageZoomIn2X_##TT##_ON(MLIB_ZOOMIN2XDEFEDGE                  \
		(STYPE))                                                   \
	{                                                                  \
	    const STYPE *psrc_low, *psrc_right, *psrc_left, *tpsrc;        \
	    STYPE *tpdst;                                                  \
	    mlib_s32 i, j;                                                 \
	                                                                   \
	    while (((pdst - pdst_near) >= dst_stride) &&                   \
		    (src_height_near > 0)) {                               \
	                                                                   \
/*                                                                         \
 * upper bound                                                             \
 */                                                                        \
		for (i = 0; i < channels; i++) {                           \
		    tpdst = pdst_near + i;                                 \
		    tpsrc = psrc_near + i;                                 \
		    ROW_COLUMN_NN(src_w_beg_near, src_width_near,          \
			    channels, channels);                           \
		}                                                          \
		if ((src_h_beg_near & 1) != 0)                             \
		    psrc_near += src_stride;                               \
		src_h_beg_near = (src_h_beg_near + 1);                     \
		pdst_near += dst_stride;                                   \
		src_height_near--;                                         \
	    }                                                              \
	    psrc_left = psrc_near;                                         \
	    psrc_low =                                                     \
		    psrc_near + ((src_height_near - 1 +                    \
		    (src_h_beg_near & 1)) >> 1) * src_stride;              \
	    while (src_height_near > src_height) {                         \
	                                                                   \
/*                                                                         \
 * lower bound of image                                                    \
 */                                                                        \
		for (i = 0; i < channels; i++) {                           \
		    tpdst = pdst_near + i + (src_height_near -             \
			    1) * dst_stride;                               \
		    tpsrc = psrc_low + i;                                  \
		    ROW_COLUMN_NN(src_w_beg_near, src_width_near,          \
			    channels, channels);                           \
		}                                                          \
		if (((src_height_near - 1 + src_h_beg_near) & 1) == 0)     \
		    psrc_low -= src_stride;                                \
		src_height_near--;                                         \
	    }                                                              \
	    if (src_height_near > 0) {                                     \
		while ((pdst - pdst_near > 0) && (src_width_near > 0)) {   \
	                                                                   \
/*                                                                         \
 * left bound of image                                                     \
 */                                                                        \
		    for (i = 0; i < channels; i++) {                       \
			tpdst = pdst_near + i;                             \
			tpsrc = psrc_left + i;                             \
			ROW_COLUMN_NN(src_h_beg_near, src_height_near,     \
				dst_stride, src_stride);                   \
		    }                                                      \
		    if ((src_w_beg_near & 1) != 0)                         \
			psrc_left += channels;                             \
		    src_width_near--;                                      \
		    pdst_near += channels;                                 \
		    src_w_beg_near = (src_w_beg_near + 1);                 \
		}                                                          \
		psrc_right =                                               \
			psrc_left + ((src_width_near - 1 +                 \
			(src_w_beg_near & 1)) >> 1) * channels;            \
		while (src_width_near > src_width) {                       \
	                                                                   \
/*                                                                         \
 * right bound of image                                                    \
 */                                                                        \
		    for (i = 0; i < channels; i++) {                       \
			tpdst = pdst_near + i + (src_width_near -          \
				1) * channels;                             \
			tpsrc = psrc_right + i;                            \
			ROW_COLUMN_NN(src_h_beg_near, src_height_near,     \
				dst_stride, src_stride);                   \
		    }                                                      \
		    if (((src_width_near - 1 + src_w_beg_near) & 1) ==     \
			    0)                                             \
			psrc_right -= channels;                            \
		    src_width_near--;                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	FUNC_ZOOM_BL(TT, STYPE, N, H, V)                                   \
	void                                                               \
	mlib_ImageZoomIn2X_##TT##_SE_BL_FL##N                              \
		(MLIB_ZOOMIN2XDEFEDGE(STYPE))                              \
	{                                                                  \
	    const STYPE *psrc_low, *psrc_right, *psrc_left, *tpsrc;        \
	    STYPE *tpdst;                                                  \
	    STYPE tmp1, tmp2;                                              \
	    mlib_s32 i, j;                                                 \
	                                                                   \
	    while (((pdst - pdst_near) >= dst_stride) &&                   \
		    (src_height_near > 0)) {                               \
	                                                                   \
/*                                                                         \
 * upper bound                                                             \
 */                                                                        \
		for (i = 0; i < channels; i++) {                           \
		    tpdst = pdst_near + i;                                 \
		    tpsrc = psrc_near + i;                                 \
		    ROW_COLUMN_BL_FL##H(src_w_beg_near,                    \
			    src_width_near, channels, channels);           \
		}                                                          \
		if ((src_h_beg_near & 1) != 0)                             \
		    psrc_near += src_stride;                               \
		src_h_beg_near = (src_h_beg_near + 1);                     \
		pdst_near += dst_stride;                                   \
		src_height_near--;                                         \
	    }                                                              \
	    psrc_left = psrc_near;                                         \
	    psrc_low =                                                     \
		    psrc_near + ((src_height_near -                        \
		    1) >> 1) * src_stride;                                 \
	    while (src_height_near > src_height) {                         \
	                                                                   \
/*                                                                         \
 * lower bound of image                                                    \
 */                                                                        \
		for (i = 0; i < channels; i++) {                           \
		    tpdst = pdst_near + i + (src_height_near -             \
			    1) * dst_stride;                               \
		    tpsrc = psrc_low + i;                                  \
		    ROW_COLUMN_BL_FL##H(src_w_beg_near,                    \
			    src_width_near, channels, channels);           \
		}                                                          \
		if (((src_height_near - 1 + src_h_beg_near) & 1) == 0)     \
		    psrc_low -= src_stride;                                \
		src_height_near--;                                         \
	    }                                                              \
	    if (src_height_near > 0) {                                     \
		while ((pdst - pdst_near > 0) && (src_width_near > 0)) {   \
	                                                                   \
/*                                                                         \
 * left bound of image                                                     \
 */                                                                        \
		    for (i = 0; i < channels; i++) {                       \
			tpdst = pdst_near + i;                             \
			tpsrc = psrc_left + i;                             \
			ROW_COLUMN_BL_FL##V(src_h_beg_near,                \
				src_height_near, dst_stride,               \
				src_stride);                               \
		    }                                                      \
		    if (src_w_beg_near != 0)                               \
			psrc_left += channels;                             \
		    src_width_near--;                                      \
		    pdst_near += channels;                                 \
		    src_w_beg_near = (src_w_beg_near + 1) & 1;             \
		}                                                          \
		psrc_right =                                               \
			psrc_left + ((src_width_near -                     \
			1) >> 1) * channels;                               \
		while (src_width_near > src_width) {                       \
	                                                                   \
/*                                                                         \
 * right bound of image                                                    \
 */                                                                        \
		    for (i = 0; i < channels; i++) {                       \
			tpdst = pdst_near + i + (src_width_near -          \
				1) * channels;                             \
			tpsrc = psrc_right + i;                            \
			ROW_COLUMN_BL_FL##V(src_h_beg_near,                \
				src_height_near, dst_stride,               \
				src_stride);                               \
		    }                                                      \
		    if (((src_width_near - 1 + src_w_beg_near) & 1) ==     \
			    0)                                             \
			psrc_right -= channels;                            \
		    src_width_near--;                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	FUNC_ZOOM_BC(TT, STYPE, FL)                                         \
	void                                                                \
	mlib_ImageZoomIn2X_##TT##_SE_BC##FL                                 \
		(MLIB_ZOOMIN2XDEFEDGE_BC(STYPE))                            \
	{                                                                   \
	    const STYPE *p_1, *p, *p1, *p2;                                 \
	    STYPE *pdst_count, *pd;                                         \
	    mlib_s32 ii, jj;                                                \
	                                                                    \
	    pdst_count = pdst_near;                                         \
	    ii = 0;                                                         \
	    jj = 0;                                                         \
	    while (((pdst - pdst_count) >= dst_stride) &&                   \
		    (src_height_near > 0)) {                                \
		ROW_BICUBIC##FL(ii, jj, channels, src_width_near,           \
			MLIB_##TT##_MIN,                                    \
			MLIB_##TT##_MAX, STYPE);                            \
		jj++;                                                       \
		pdst_count += dst_stride;                                   \
		src_height_near--;                                          \
	    }                                                               \
	    while (src_height_near > src_height) {                          \
		ROW_BICUBIC##FL(ii, jj + src_height_near - 1,               \
			channels, src_width_near,                           \
			MLIB_##TT##_MIN,                                    \
			MLIB_##TT##_MAX, STYPE);                            \
		src_height_near--;                                          \
	    }                                                               \
	    if (src_height_near > 0) {                                      \
		while ((pdst - pdst_count > 0) && (src_width_near > 0)) {   \
		    COLUMN_BICUBIC##FL(ii, jj, channels,                    \
			    (src_height_near + jj),                         \
			    MLIB_##TT##_MIN,                                \
			    MLIB_##TT##_MAX, STYPE);                        \
		    src_width_near--;                                       \
		    pdst_count += channels;                                 \
		    ii++;                                                   \
		}                                                           \
		while (src_width_near > src_width) {                        \
		    COLUMN_BICUBIC##FL(ii + src_width_near - 1, jj,         \
			    channels, (src_height_near + jj),               \
			    MLIB_##TT##_MIN,                                \
			    MLIB_##TT##_MAX, STYPE);                        \
		    src_width_near--;                                       \
		}                                                           \
	    }                                                               \
	}

/* *********************************************************** */

FUNC_ZOOM_CE(U8, mlib_u8)
FUNC_ZOOM_ON(
	U8,
	mlib_u8)
FUNC_ZOOM_BL(
	U8,
	mlib_u8,
	0,
	0,
	0)
FUNC_ZOOM_BL(
	U8,
	mlib_u8,
	1,
	0,
	1)
FUNC_ZOOM_BL(
	U8,
	mlib_u8,
	2,
	1,
	0)
FUNC_ZOOM_BL(
	U8,
	mlib_u8,
	3,
	1,
	1)
FUNC_ZOOM_BC(
	U8,
	mlib_u8,
	_FL0)
FUNC_ZOOM_BC(
	U8,
	mlib_u8,
	_FL1)
FUNC_ZOOM_BC(
	U8,
	mlib_u8,
	_FL2)
FUNC_ZOOM_BC(
	U8,
	mlib_u8,
	_FL3)
FUNC_ZOOM_BC(
	U8,
	mlib_u8,
	2_FL0)
FUNC_ZOOM_BC(
	U8,
	mlib_u8,
	2_FL1)
FUNC_ZOOM_BC(
	U8,
	mlib_u8,
	2_FL2)
FUNC_ZOOM_BC(
	U8,
	mlib_u8,
	2_FL3)

/* *********************************************************** */
FUNC_ZOOM_CE(
	S16,
	mlib_s16)
FUNC_ZOOM_ON(
	S16,
	mlib_s16)
FUNC_ZOOM_BL(
	S16,
	mlib_s16,
	0,
	0,
	0)
FUNC_ZOOM_BL(
	S16,
	mlib_s16,
	1,
	0,
	1)
FUNC_ZOOM_BL(
	S16,
	mlib_s16,
	2,
	1,
	0)
FUNC_ZOOM_BL(
	S16,
	mlib_s16,
	3,
	1,
	1)
FUNC_ZOOM_BC(
	S16,
	mlib_s16,
	_FL0)
FUNC_ZOOM_BC(
	S16,
	mlib_s16,
	_FL1)
FUNC_ZOOM_BC(
	S16,
	mlib_s16,
	_FL2)
FUNC_ZOOM_BC(
	S16,
	mlib_s16,
	_FL3)
FUNC_ZOOM_BC(
	S16,
	mlib_s16,
	2_FL0)
FUNC_ZOOM_BC(
	S16,
	mlib_s16,
	2_FL1)
FUNC_ZOOM_BC(
	S16,
	mlib_s16,
	2_FL2)
FUNC_ZOOM_BC(
	S16,
	mlib_s16,
	2_FL3)

/* *********************************************************** */
FUNC_ZOOM_CE(
	U16,
	mlib_u16)
FUNC_ZOOM_ON(
	U16,
	mlib_u16)
FUNC_ZOOM_BL(
	U16,
	mlib_u16,
	0,
	0,
	0)
FUNC_ZOOM_BL(
	U16,
	mlib_u16,
	1,
	0,
	1)
FUNC_ZOOM_BL(
	U16,
	mlib_u16,
	2,
	1,
	0)
FUNC_ZOOM_BL(
	U16,
	mlib_u16,
	3,
	1,
	1)
FUNC_ZOOM_BC(
	U16,
	mlib_u16,
	_FL0)
FUNC_ZOOM_BC(
	U16,
	mlib_u16,
	_FL1)
FUNC_ZOOM_BC(
	U16,
	mlib_u16,
	_FL2)
FUNC_ZOOM_BC(
	U16,
	mlib_u16,
	_FL3)
FUNC_ZOOM_BC(
	U16,
	mlib_u16,
	2_FL0)
FUNC_ZOOM_BC(
	U16,
	mlib_u16,
	2_FL1)
FUNC_ZOOM_BC(
	U16,
	mlib_u16,
	2_FL2)
FUNC_ZOOM_BC(
	U16,
	mlib_u16,
	2_FL3)

/* *********************************************************** */
FUNC_ZOOM_CE(
	S32,
	mlib_s32)
FUNC_ZOOM_ON(
	S32,
	mlib_s32)

/* *********************************************************** */
#undef ROW_COLUMN_BL_FL0
#undef ROW_COLUMN_BL_FL1

#ifndef	_NO_LONGLONG

/* *********************************************************** */
#define	ROW_COLUMN_BL_FL0(beg, amount, shift_dst, shift_src)           \
	j = (beg + 1) & 1;                                             \
	if (beg == 0) {                                                \
	    *tpdst = (*tpsrc);                                         \
	    tpdst += shift_dst;                                        \
	    flag = 0;                                                  \
	} else if (j == 1) {                                           \
	    *tpdst = (mlib_s32)((3 * (mlib_s64)(*tpsrc) + (*(tpsrc -   \
		    shift_src))) >> 2);                                \
	    tpdst += shift_dst;                                        \
	    flag = 1;                                                  \
	}                                                              \
	tmp1 = *tpsrc;                                                 \
	for (j++; j < amount; j += 2) {                                \
	    tmp2 = *(tpsrc + shift_src);                               \
	    *tpdst = (mlib_s32)((3 * tmp1 + tmp2) >> 2);               \
	    *(tpdst + shift_dst) =                                     \
		    (mlib_s32)((tmp1 + 3 * tmp2) >> 2);                \
	    tpsrc += shift_src;                                        \
	    tpdst += (shift_dst << 1);                                 \
	    tmp1 = tmp2;                                               \
	}                                                              \
	                                                               \
	if (j == amount)                                               \
	    if (flag) {                                                \
		*tpdst = (mlib_s32)((3 * tmp1 + *(tpsrc +              \
			shift_src)) >> 2);                           \
	    } else {                                                   \
		*tpdst = (mlib_s32)tmp1;                               \
	    }
/* *********************************************************** */
#define	ROW_COLUMN_BL_FL1(beg, amount, shift_dst, shift_src)          \
	j = (beg + 1) & 1;                                            \
	if (beg == 0) {                                               \
	    *tpdst = *tpsrc;                                          \
	    tpdst += shift_dst;                                       \
	} else if (j == 1) {                                          \
	    *tpdst = (mlib_s32)(((mlib_s64)(*tpsrc) +                 \
		    (mlib_s64)(*(tpsrc - shift_src))) >> 1);          \
	    tpdst += shift_dst;                                       \
	}                                                             \
	tmp1 = *tpsrc;                                                \
	for (j++; j < amount; j += 2) {                               \
	    tmp2 = *(tpsrc + shift_src);                              \
	    *tpdst = (mlib_s32)tmp1;                                  \
	    *(tpdst + shift_dst) = (mlib_s32)((tmp1 + tmp2) >> 1);    \
	    tpsrc += shift_src;                                       \
	    tpdst += (shift_dst << 1);                                \
	    tmp1 = tmp2;                                              \
	}                                                             \
	                                                              \
	if (j == amount)                                              \
	    *tpdst = (mlib_s32)tmp1
/* *********************************************************** */

#else /* _NO_LONGLONG */

/* *********************************************************** */
#define	ROW_COLUMN_BL_FL0(beg, amount, shift_dst, shift_src)           \
	j = (beg + 1) & 1;                                             \
	if (beg == 0) {                                                \
	    *tpdst = (*tpsrc);                                         \
	    tpdst += shift_dst;                                        \
	    flag = 0;                                                  \
	} else if (j == 1) {                                           \
	    *tpdst = (mlib_s32)((3 * (mlib_d64)(*tpsrc) + (*(tpsrc -   \
		    shift_src))) * 0.25);                              \
	    tpdst += shift_dst;                                        \
	    flag = 1;                                                  \
	}                                                              \
	tmp1 = *tpsrc;                                                 \
	for (j++; j < amount; j += 2) {                                \
	    tmp2 = *(tpsrc + shift_src);                               \
	    *tpdst = (mlib_s32)((3 * tmp1 + tmp2) * 0.25);           \
	    *(tpdst + shift_dst) =                                     \
		    (mlib_s32)((tmp1 + 3 * tmp2) * 0.25);            \
	    tpsrc += shift_src;                                        \
	    tpdst += (shift_dst << 1);                                 \
	    tmp1 = tmp2;                                               \
	}                                                              \
	                                                               \
	if (j == amount)                                               \
	    if (flag) {                                                \
		*tpdst = (mlib_s32)((3 * tmp1 + *(tpsrc +            \
			shift_src)) * 0.25);                           \
	    } else {                                                   \
		*tpdst = (mlib_s32)tmp1;                               \
	    }
/* *********************************************************** */
#define	ROW_COLUMN_BL_FL1(beg, amount, shift_dst, shift_src)          \
	j = (beg + 1) & 1;                                            \
	if (beg == 0) {                                               \
	    *tpdst = *tpsrc;                                          \
	    tpdst += shift_dst;                                       \
	} else if (j == 1) {                                          \
	    *tpdst = (mlib_s32)(((mlib_d64)(*tpsrc) +                 \
		    (mlib_d64)(*(tpsrc - shift_src))) * 0.5);         \
	    tpdst += shift_dst;                                       \
	}                                                             \
	tmp1 = *tpsrc;                                                \
	for (j++; j < amount; j += 2) {                               \
	    tmp2 = *(tpsrc + shift_src);                              \
	    *tpdst = (mlib_s32)tmp1;                                  \
	    *(tpdst + shift_dst) = (mlib_s32)((tmp1 + tmp2) * 0.5);   \
	    tpsrc += shift_src;                                       \
	    tpdst += (shift_dst << 1);                                \
	    tmp1 = tmp2;                                              \
	}                                                             \
	                                                              \
	if (j == amount)                                              \
	    *tpdst = (mlib_s32)tmp1
/* *********************************************************** */

#endif /* _NO_LONGLONG */

    void mlib_ImageZoomIn2X_S32_SE_BL_FL0(
	MLIB_ZOOMIN2XDEFEDGE(mlib_s32))
{
	const mlib_s32 *psrc_low, *psrc_right, *psrc_left, *tpsrc;
	mlib_s32 *tpdst;
#ifndef	_NO_LONGLONG
	mlib_s64 tmp1, tmp2;
#else /* _NO_LONGLONG */
	mlib_d64 tmp1, tmp2;
#endif /* _NO_LONGLONG */
	mlib_s32 i, j, flag = 0;

/* upper bound of image */
	while (((pdst - pdst_near) >= dst_stride) && (src_height_near > 0)) {
		for (i = 0; i < channels; i++) {
			tpdst = pdst_near + i;
			tpsrc = psrc_near + i;
			ROW_COLUMN_BL_FL0(src_w_beg_near, src_width_near,
				channels, channels);
		}

		if ((src_h_beg_near & 1) != 0)
			psrc_near += src_stride;
		src_h_beg_near = (src_h_beg_near + 1);
		pdst_near += dst_stride;
		src_height_near--;
	}

	psrc_left = psrc_near;
	psrc_low = psrc_near + ((src_height_near - 1) >> 1) * src_stride;

	while (src_height_near > src_height) {
/* lower bound of image */
		for (i = 0; i < channels; i++) {
			tpdst = pdst_near + i + (src_height_near -
				1) * dst_stride;
			tpsrc = psrc_low + i;
			ROW_COLUMN_BL_FL0(src_w_beg_near, src_width_near,
				channels, channels);
		}

		if (((src_height_near - 1 + src_h_beg_near) & 1) == 0)
			psrc_low -= src_stride;
		src_height_near--;
	}

	if (src_height_near > 0) {
		while ((pdst - pdst_near > 0) && (src_width_near > 0)) {
/* left bound of image */
			for (i = 0; i < channels; i++) {
				tpdst = pdst_near + i;
				tpsrc = psrc_left + i;
				ROW_COLUMN_BL_FL0(src_h_beg_near,
					src_height_near, dst_stride,
					src_stride);
			}

			if (src_w_beg_near != 0)
				psrc_left += channels;
			src_width_near--;
			pdst_near += channels;
			src_w_beg_near = (src_w_beg_near + 1) & 1;
		}

		psrc_right = psrc_left + ((src_width_near - 1) >> 1) * channels;

		while (src_width_near > src_width) {
/* right bound of image */
			for (i = 0; i < channels; i++) {
				tpdst = pdst_near + i + (src_width_near -
					1) * channels;
				tpsrc = psrc_right + i;
				ROW_COLUMN_BL_FL0(src_h_beg_near,
					src_height_near, dst_stride,
					src_stride);
			}

			if (((src_width_near - 1 + src_w_beg_near) & 1) == 0)
				psrc_right -= channels;
			src_width_near--;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIn2X_S32_SE_BL_FL1(
	MLIB_ZOOMIN2XDEFEDGE(mlib_s32))
{
	const mlib_s32 *psrc_low, *psrc_right, *psrc_left, *tpsrc;
	mlib_s32 *tpdst;
#ifndef	_NO_LONGLONG
	mlib_s64 tmp1, tmp2;
#else /* _NO_LONGLONG */
	mlib_d64 tmp1, tmp2;
#endif /* _NO_LONGLONG */
	mlib_s32 i, j, flag = 0;

	while (((pdst - pdst_near) >= dst_stride) && (src_height_near > 0)) {
/* upper bound of image */
		for (i = 0; i < channels; i++) {
			tpdst = pdst_near + i;
			tpsrc = psrc_near + i;
			ROW_COLUMN_BL_FL0(src_w_beg_near, src_width_near,
				channels, channels);
		}

		if ((src_h_beg_near & 1) != 0)
			psrc_near += src_stride;
		src_h_beg_near = (src_h_beg_near + 1);
		pdst_near += dst_stride;
		src_height_near--;
	}

	psrc_left = psrc_near;
	psrc_low = psrc_near + ((src_height_near - 1) >> 1) * src_stride;

	while (src_height_near > src_height) {
/* lower bound of image */
		for (i = 0; i < channels; i++) {
			tpdst = pdst_near + i + (src_height_near -
				1) * dst_stride;
			tpsrc = psrc_low + i;
			ROW_COLUMN_BL_FL0(src_w_beg_near, src_width_near,
				channels, channels);
		}

		if (((src_height_near - 1 + src_h_beg_near) & 1) == 0)
			psrc_low -= src_stride;
		src_height_near--;
	}

	if (src_height_near > 0) {
		while ((pdst - pdst_near > 0) && (src_width_near > 0)) {
/* left bound of image */
			for (i = 0; i < channels; i++) {
				tpdst = pdst_near + i;
				tpsrc = psrc_left + i;
				ROW_COLUMN_BL_FL1(src_h_beg_near,
					src_height_near, dst_stride,
					src_stride);
			}

			if (src_w_beg_near != 0)
				psrc_left += channels;
			src_width_near--;
			pdst_near += channels;
			src_w_beg_near = (src_w_beg_near + 1) & 1;
		}

		psrc_right = psrc_left + ((src_width_near - 1) >> 1) * channels;

		while (src_width_near > src_width) {
/* right bound of image */
			for (i = 0; i < channels; i++) {
				tpdst = pdst_near + i + (src_width_near -
					1) * channels;
				tpsrc = psrc_right + i;
				ROW_COLUMN_BL_FL1(src_h_beg_near,
					src_height_near, dst_stride,
					src_stride);
			}

			if (((src_width_near - 1 + src_w_beg_near) & 1) == 0)
				psrc_right -= channels;
			src_width_near--;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIn2X_S32_SE_BL_FL2(
	MLIB_ZOOMIN2XDEFEDGE(mlib_s32))
{
	const mlib_s32 *psrc_low, *psrc_right, *psrc_left, *tpsrc;
	mlib_s32 *tpdst;
#ifndef	_NO_LONGLONG
	mlib_s64 tmp1, tmp2;
#else /* _NO_LONGLONG */
	mlib_d64 tmp1, tmp2;
#endif /* _NO_LONGLONG */
	mlib_s32 i, j, flag = 0;

/* upper bound of image */
	while (((pdst - pdst_near) >= dst_stride) && (src_height_near > 0)) {
		for (i = 0; i < channels; i++) {
			tpdst = pdst_near + i;
			tpsrc = psrc_near + i;
			ROW_COLUMN_BL_FL1(src_w_beg_near, src_width_near,
				channels, channels);
		}

		if ((src_h_beg_near & 1) != 0)
			psrc_near += src_stride;
		src_h_beg_near = (src_h_beg_near + 1);
		pdst_near += dst_stride;
		src_height_near--;
	}

	psrc_left = psrc_near;
	psrc_low = psrc_near + ((src_height_near - 1) >> 1) * src_stride;

/* lower bound of image */
	while (src_height_near > src_height) {
		for (i = 0; i < channels; i++) {
			tpdst = pdst_near + i + (src_height_near -
				1) * dst_stride;
			tpsrc = psrc_low + i;
			ROW_COLUMN_BL_FL1(src_w_beg_near, src_width_near,
				channels, channels);
		}

		if (((src_height_near - 1 + src_h_beg_near) & 1) == 0)
			psrc_low -= src_stride;
		src_height_near--;
	}

	if (src_height_near > 0) {
/* left bound of image */
		while ((pdst - pdst_near > 0) && (src_width_near > 0)) {
			for (i = 0; i < channels; i++) {
				tpdst = pdst_near + i;
				tpsrc = psrc_left + i;
				ROW_COLUMN_BL_FL0(src_h_beg_near,
					src_height_near, dst_stride,
					src_stride);
			}

			if (src_w_beg_near != 0)
				psrc_left += channels;
			src_width_near--;
			pdst_near += channels;
			src_w_beg_near = (src_w_beg_near + 1) & 1;
		}

		psrc_right = psrc_left + ((src_width_near - 1) >> 1) * channels;

/* right bound of image */
		while (src_width_near > src_width) {
			for (i = 0; i < channels; i++) {
				tpdst = pdst_near + i + (src_width_near -
					1) * channels;
				tpsrc = psrc_right + i;
				ROW_COLUMN_BL_FL0(src_h_beg_near,
					src_height_near, dst_stride,
					src_stride);
			}

			if (((src_width_near - 1 + src_w_beg_near) & 1) == 0)
				psrc_right -= channels;
			src_width_near--;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIn2X_S32_SE_BL_FL3(
	MLIB_ZOOMIN2XDEFEDGE(mlib_s32))
{
	const mlib_s32 *psrc_low, *psrc_right, *psrc_left, *tpsrc;
	mlib_s32 *tpdst;
#ifndef	_NO_LONGLONG
	mlib_s64 tmp1, tmp2;
#else /* _NO_LONGLONG */
	mlib_d64 tmp1, tmp2;
#endif /* _NO_LONGLONG */
	mlib_s32 i, j;

	while (((pdst - pdst_near) >= dst_stride) && (src_height_near > 0)) {
/* upper bound of image */
		for (i = 0; i < channels; i++) {
			tpdst = pdst_near + i;
			tpsrc = psrc_near + i;
			ROW_COLUMN_BL_FL1(src_w_beg_near, src_width_near,
				channels, channels);
		}

		if ((src_h_beg_near & 1) != 0) {
			psrc_near += src_stride;
		}

		src_h_beg_near = (src_h_beg_near + 1);
		pdst_near += dst_stride;
		src_height_near--;
	}

	psrc_left = psrc_near;
	psrc_low = psrc_near + ((src_height_near - 1) >> 1) * src_stride;

	while (src_height_near > src_height) {
/* lower bound of image */
		for (i = 0; i < channels; i++) {
			tpdst = pdst_near + i + (src_height_near -
				1) * dst_stride;
			tpsrc = psrc_low + i;
			ROW_COLUMN_BL_FL1(src_w_beg_near, src_width_near,
				channels, channels);
		}

		if (((src_height_near - 1 + src_h_beg_near) & 1) == 0)
			psrc_low -= src_stride;
		src_height_near--;
	}

	if (src_height_near > 0) {
		while ((pdst - pdst_near > 0) && (src_width_near > 0)) {
/* left bound of image */
			for (i = 0; i < channels; i++) {
				tpdst = pdst_near + i;
				tpsrc = psrc_left + i;
				ROW_COLUMN_BL_FL1(src_h_beg_near,
					src_height_near, dst_stride,
					src_stride);
			}

			if (src_w_beg_near != 0)
				psrc_left += channels;
			src_width_near--;
			pdst_near += channels;
			src_w_beg_near = (src_w_beg_near + 1) & 1;
		}

		psrc_right = psrc_left + ((src_width_near - 1) >> 1) * channels;

		while (src_width_near > src_width) {
/* right bound of image */
			for (i = 0; i < channels; i++) {
				tpdst = pdst_near + i + (src_width_near -
					1) * channels;
				tpsrc = psrc_right + i;
				ROW_COLUMN_BL_FL1(src_h_beg_near,
					src_height_near, dst_stride,
					src_stride);
			}

			if (((src_width_near - 1 + src_w_beg_near) & 1) == 0)
				psrc_right -= channels;
			src_width_near--;
		}
	}
}

/* *********************************************************** */

#undef PREPARE_BICUBIC
#undef ROW_BICUBIC_FL0
#undef COLUMN_BICUBIC_FL0
#undef ROW_BICUBIC_FL1
#undef COLUMN_BICUBIC_FL1
#undef ROW_BICUBIC_FL2
#undef COLUMN_BICUBIC_FL2
#undef ROW_BICUBIC_FL3
#undef COLUMN_BICUBIC_FL3
#undef ROW_BICUBIC2_FL0
#undef COLUMN_BICUBIC2_FL0
#undef ROW_BICUBIC2_FL1
#undef COLUMN_BICUBIC2_FL1
#undef ROW_BICUBIC2_FL2
#undef COLUMN_BICUBIC2_FL2
#undef ROW_BICUBIC2_FL3
#undef COLUMN_BICUBIC2_FL3

#ifndef	_NO_LONGLONG

/* *********************************************************** */

#define	PREPARE_BICUBIC(beg_i, j, chan)                                 \
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
	mlib_s32 i, l;                                                  \
	mlib_s64 a_1, a, a1, a2 = 0, rez, rez1;                         \
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
	    wx2 = src_width_beg - 1;                                    \
	wx_1 *= chan;                                                   \
	wx *= chan;                                                     \
	wx1 *= chan;                                                    \
	wx2 *= chan

/* *********************************************************** */

#define	ROW_BICUBIC_FL0(beg_i, j, chan, amount, MIN, MAX)                  \
	{                                                                  \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    psrc_end = psrc_beg + hx + (src_width_beg - 1) * chan;         \
	    if (dh == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (111 * (mlib_s64)psrc_beg[hx + wx_1 + l] +       \
			    29 * (mlib_s64)psrc_beg[hx1 + wx_1 + l] -      \
			    9 * (mlib_s64)psrc_beg[hx_1 + wx_1 + l] -      \
			    3 * (mlib_s64)psrc_beg[hx2 + wx_1 + l]);       \
		    a = (111 * (mlib_s64)psrc_beg[hx + wx + l] +           \
			    29 * (mlib_s64)psrc_beg[hx1 + wx + l] -        \
			    9 * (mlib_s64)psrc_beg[hx_1 + wx + l] -        \
			    3 * (mlib_s64)psrc_beg[hx2 + wx + l]);         \
		    a1 = (111 * (mlib_s64)psrc_beg[hx + wx1 + l] +         \
			    29 * (mlib_s64)psrc_beg[hx1 + wx1 + l] -       \
			    9 * (mlib_s64)psrc_beg[hx_1 + wx1 + l] -       \
			    3 * (mlib_s64)psrc_beg[hx2 + wx1 + l]);        \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                       \
		    p = psrc_beg + hx + wx2 + l;                           \
		    p1 = psrc_beg + hx1 + wx2 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 1) {                                         \
			a2 = 111 * (mlib_s64)(*p) +                        \
				29 * (mlib_s64)(*p1) -                     \
				9 * (mlib_s64)(*p_1) -                     \
				3 * (mlib_s64)(*p2);                       \
			rez = (29 * a + 111 * a1 - 3 * a_1 -               \
				9 * a2) >> 14;                             \
				/* 9 * a2) * 0.00006103515625; */          \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += chan;                                        \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 111 * (mlib_s64)(*p) +                        \
				29 * (mlib_s64)(*p1) -                     \
				9 * (mlib_s64)(*p_1) -                     \
				3 * (mlib_s64)(*p2);                       \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 14;                             \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = (29 * a + 111 * a1 - 3 * a_1 -               \
				9 * a2) >> 14;                             \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + chan) = (mlib_s32)rez;                      \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = (111 * (mlib_s64)(*p) +                       \
				29 * (mlib_s64)(*p1) -                     \
				9 * (mlib_s64)(*p_1) -                     \
				3 * (mlib_s64)(*p2));                      \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 14;                             \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (29 * a + 111 * a1 - 3 * a_1 -               \
				9 * a2) >> 14;                             \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (29 * (mlib_s64)psrc_beg[hx + wx_1 + l] +        \
			    111 * (mlib_s64)psrc_beg[hx1 + wx_1 + l] -     \
			    3 * (mlib_s64)psrc_beg[hx_1 + wx_1 + l] -      \
			    9 * (mlib_s64)psrc_beg[hx2 + wx_1 + l]);       \
		    a = (29 * (mlib_s64)psrc_beg[hx + wx + l] +            \
			    111 * (mlib_s64)psrc_beg[hx1 + wx + l] -       \
			    3 * (mlib_s64)psrc_beg[hx_1 + wx + l] -        \
			    9 * (mlib_s64)psrc_beg[hx2 + wx + l]);         \
		    a1 = (29 * (mlib_s64)psrc_beg[hx + wx1 + l] +          \
			    111 * (mlib_s64)psrc_beg[hx1 + wx1 + l] -      \
			    3 * (mlib_s64)psrc_beg[hx_1 + wx1 + l] -       \
			    9 * (mlib_s64)psrc_beg[hx2 + wx1 + l]);        \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                       \
		    p = psrc_beg + hx + wx2 + l;                           \
		    p1 = psrc_beg + hx1 + wx2 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 1) {                                         \
			a2 = 29 * (mlib_s64)(*p) +                         \
				111 * (mlib_s64)(*p1) -                    \
				3 * (mlib_s64)(*p_1) -                     \
				9 * (mlib_s64)(*p2);                       \
			rez = (29 * a + 111 * a1 - 3 * a_1 -               \
				9 * a2) >> 14;                             \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += chan;                                        \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 29 * (mlib_s64)(*p) +                         \
				111 * (mlib_s64)(*p1) -                    \
				3 * (mlib_s64)(*p_1) -                     \
				9 * (mlib_s64)(*p2);                       \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 14;                             \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = (29 * a + 111 * a1 - 3 * a_1 -               \
				9 * a2) >> 14;                             \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + chan) = (mlib_s32)rez;                      \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = 29 * (mlib_s64)(*p) +                         \
				111 * (mlib_s64)(*p1) -                    \
				3 * (mlib_s64)(*p_1) -                     \
				9 * (mlib_s64)(*p2);                       \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 14;                             \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (29 * a + 111 * a1 - 3 * a_1 -               \
				9 * a2) >> 14;                             \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC_FL0(beg_i, j, chan, amount, MIN, MAX)               \
	{                                                                  \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    psrc_end =                                                     \
		    psrc_beg + wx + (src_height_beg - 1) * src_stride;     \
	    if (dw == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (111 * (mlib_s64)psrc_beg[hx_1 + wx + l] +       \
			    29 * (mlib_s64)psrc_beg[hx_1 + wx1 + l] -      \
			    9 * (mlib_s64)psrc_beg[hx_1 + wx_1 + l] -      \
			    3 * (mlib_s64)psrc_beg[hx_1 + wx2 + l]);       \
		    a = (111 * (mlib_s64)psrc_beg[hx + wx + l] +           \
			    29 * (mlib_s64)psrc_beg[hx + wx1 + l] -        \
			    9 * (mlib_s64)psrc_beg[hx + wx_1 + l] -        \
			    3 * (mlib_s64)psrc_beg[hx + wx2 + l]);         \
		    a1 = (111 * (mlib_s64)psrc_beg[hx1 + wx + l] +         \
			    29 * (mlib_s64)psrc_beg[hx1 + wx1 + l] -       \
			    9 * (mlib_s64)psrc_beg[hx1 + wx_1 + l] -       \
			    3 * (mlib_s64)psrc_beg[hx1 + wx2 + l]);        \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 1) {                                         \
			a2 = 111 * (mlib_s64)(*p) +                        \
				29 * (mlib_s64)(*p1) -                     \
				9 * (mlib_s64)(*p_1) -                     \
				3 * (mlib_s64)(*p2);                       \
			rez = (29 * a + 111 * a1 - 3 * a_1 -               \
				9 * a2) >> 14;                             \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += dst_stride;                                  \
			i++;                                               \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 111 * (mlib_s64)(*p) +                        \
				29 * (mlib_s64)(*p1) -                     \
				9 * (mlib_s64)(*p_1) -                     \
				3 * (mlib_s64)(*p2);                       \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 14;                             \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = (29 * a + 111 * a1 - 3 * a_1 -               \
				9 * a2) >> 14;                             \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + dst_stride) = (mlib_s32)rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = 111 * (mlib_s64)(*p) +                        \
				29 * (mlib_s64)(*p1) -                     \
				9 * (mlib_s64)(*p_1) -                     \
				3 * (mlib_s64)(*p2);                       \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 14;                             \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (29 * a + 111 * a1 - 3 * a_1 -               \
				9 * a2) >> 14;                             \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (29 * (mlib_s64)psrc_beg[hx_1 + wx + l] +        \
			    111 * (mlib_s64)psrc_beg[hx_1 + wx1 + l] -     \
			    3 * (mlib_s64)psrc_beg[hx_1 + wx_1 + l] -      \
			    9 * (mlib_s64)psrc_beg[hx_1 + wx2 + l]);       \
		    a = (29 * (mlib_s64)psrc_beg[hx + wx + l] +            \
			    111 * (mlib_s64)psrc_beg[hx + wx1 + l] -       \
			    3 * (mlib_s64)psrc_beg[hx + wx_1 + l] -        \
			    9 * (mlib_s64)psrc_beg[hx + wx2 + l]);         \
		    a1 = (29 * (mlib_s64)psrc_beg[hx1 + wx + l] +          \
			    111 * (mlib_s64)psrc_beg[hx1 + wx1 + l] -      \
			    3 * (mlib_s64)psrc_beg[hx1 + wx_1 + l] -       \
			    9 * (mlib_s64)psrc_beg[hx1 + wx2 + l]);        \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 1) {                                         \
			a2 = 29 * (mlib_s64)(*p) +                         \
				111 * (mlib_s64)(*p1) -                    \
				3 * (mlib_s64)(*p_1) -                     \
				9 * (mlib_s64)(*p2);                       \
			rez = (29 * a + 111 * a1 - 3 * a_1 -               \
				9 * a2) >> 14;                             \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += dst_stride;                                  \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 29 * (mlib_s64)(*p) +                         \
				111 * (mlib_s64)(*p1) -                    \
				3 * (mlib_s64)(*p_1) -                     \
				9 * (mlib_s64)(*p2);                       \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 14;                             \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = (29 * a + 111 * a1 - 3 * a_1 -               \
				9 * a2) >> 14;                             \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + dst_stride) = (mlib_s32)rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = 29 * (mlib_s64)(*p) +                         \
				111 * (mlib_s64)(*p1) -                    \
				3 * (mlib_s64)(*p_1) -                     \
				9 * (mlib_s64)(*p2);                       \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -              \
				3 * a2) >> 14;                             \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (29 * a + 111 * a1 - 3 * a_1 -               \
				9 * a2) >> 14;                             \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	ROW_BICUBIC_FL1(beg_i, j, chan, amount, MIN, MAX)                 \
	{                                                                 \
	    PREPARE_BICUBIC(beg_i, j, chan);                              \
	    psrc_end = psrc_beg + hx + (src_width_beg - 1) * chan;        \
	    if (dh == 0) {                                                \
		for (l = 0; l < chan; l++) {                              \
		    a_1 = (mlib_s64)psrc_beg[hx + wx_1 + l];              \
		    a = (mlib_s64)psrc_beg[hx + wx + l];                  \
		    a1 = (mlib_s64)psrc_beg[hx + wx1 + l];                \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                      \
		    p = psrc_beg + hx + wx2 + l;                          \
		    p1 = psrc_beg + hx1 + wx2 + l;                        \
		    p2 = psrc_beg + hx2 + wx2 + l;                        \
		    i = (beg_i);                                          \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;     \
		    if (dw == 1) {                                        \
			a2 = (mlib_s64)(*p);                              \
			rez = (29 * a + 111 * a1 - 3 * a_1 -              \
				9 * a2) >> 7;                             \
				/* 9 * a2) * 0.0078125; */                \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
			pd += chan;                                       \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += chan;                                  \
			    p += chan;                                    \
			    p1 += chan;                                   \
			    p2 += chan;                                   \
			}                                                 \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = (mlib_s64)(*p);                              \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -             \
				3 * a2) >> 7;                             \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			rez = (29 * a + 111 * a1 - 3 * a_1 -              \
				9 * a2) >> 7;                             \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*(pd + chan) = (mlib_s32)rez;                     \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += chan;                                  \
			    p += chan;                                    \
			    p1 += chan;                                   \
			    p2 += chan;                                   \
			}                                                 \
			pd += (chan << 1);                                \
		    }                                                     \
		    if (i < (amount)) {                                   \
			a2 = (mlib_s64)(*p);                              \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -             \
				3 * a2) >> 7;                             \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			pd += chan;                                       \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (29 * a + 111 * a1 - 3 * a_1 -              \
				9 * a2) >> 7;                             \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
		    }                                                     \
		}                                                         \
	    } else {                                                      \
		for (l = 0; l < chan; l++) {                              \
		    a_1 = 9 * ((mlib_s64)psrc_beg[hx + wx_1 + l] +      \
			    (mlib_s64)psrc_beg[hx1 + wx_1 + l]) -         \
			    (mlib_s64)psrc_beg[hx_1 + wx_1 + l] -         \
			    (mlib_s64)psrc_beg[hx2 + wx_1 + l];           \
		    a = 9 * ((mlib_s64)psrc_beg[hx + wx + l] +          \
			    (mlib_s64)psrc_beg[hx1 + wx + l]) -           \
			    (mlib_s64)psrc_beg[hx_1 + wx + l] -           \
			    (mlib_s64)psrc_beg[hx2 + wx + l];             \
		    a1 = 9 * ((mlib_s64)psrc_beg[hx + wx1 + l] +        \
			    (mlib_s64)psrc_beg[hx1 + wx1 + l]) -          \
			    (mlib_s64)psrc_beg[hx_1 + wx1 + l] -          \
			    (mlib_s64)psrc_beg[hx2 + wx1 + l];            \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                      \
		    p = psrc_beg + hx + wx2 + l;                          \
		    p1 = psrc_beg + hx1 + wx2 + l;                        \
		    p2 = psrc_beg + hx2 + wx2 + l;                        \
		    i = (beg_i);                                          \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;     \
		    if (dw == 1) {                                        \
			a2 = 9 * ((mlib_s64)(*p) + (mlib_s64)(*p1)) -     \
				(mlib_s64)(*p_1) - (mlib_s64)(*p2);       \
			rez = (29 * a + 111 * a1 - 3 * a_1 -              \
				9 * a2) >> 11;                            \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
			pd += chan;                                       \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += chan;                                  \
			    p += chan;                                    \
			    p1 += chan;                                   \
			    p2 += chan;                                   \
			}                                                 \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = 9 * ((mlib_s64)(*p) + (mlib_s64)(*p1)) -     \
				(mlib_s64)(*p_1) - (mlib_s64)(*p2);       \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -             \
				3 * a2) >> 11;                            \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			rez = (29 * a + 111 * a1 - 3 * a_1 -              \
				9 * a2) >> 11;                            \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*(pd + chan) = (mlib_s32)rez;                     \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += chan;                                  \
			    p += chan;                                    \
			    p1 += chan;                                   \
			    p2 += chan;                                   \
			}                                                 \
			pd += (chan << 1);                                \
		    }                                                     \
		    if (i < (amount)) {                                   \
			a2 = 9 * ((mlib_s64)(*p) + (mlib_s64)(*p1)) -     \
				(mlib_s64)(*p_1) - (mlib_s64)(*p2);       \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -             \
				3 * a2) >> 11;                            \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			pd += chan;                                       \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (29 * a + 111 * a1 - 3 * a_1 -              \
				9 * a2) >> 11;                            \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
		    }                                                     \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC_FL1(beg_i, j, chan, amount, MIN, MAX)               \
	{                                                                  \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    psrc_end =                                                     \
		    psrc_beg + wx + (src_height_beg - 1) * src_stride;     \
	    if (dw == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (111 * (mlib_s64)psrc_beg[hx_1 + wx + l] +     \
			    29 * (mlib_s64)psrc_beg[hx_1 + wx1 + l] -    \
			    9 * (mlib_s64)psrc_beg[hx_1 + wx_1 + l] -    \
			    3 * (mlib_s64)psrc_beg[hx_1 + wx2 + l]);     \
		    a = (111 * (mlib_s64)psrc_beg[hx + wx + l] +         \
			    29 * (mlib_s64)psrc_beg[hx + wx1 + l] -      \
			    9 * (mlib_s64)psrc_beg[hx + wx_1 + l] -      \
			    3 * (mlib_s64)psrc_beg[hx + wx2 + l]);       \
		    a1 = (111 * (mlib_s64)psrc_beg[hx1 + wx + l] +       \
			    29 * (mlib_s64)psrc_beg[hx1 + wx1 + l] -     \
			    9 * (mlib_s64)psrc_beg[hx1 + wx_1 + l] -     \
			    3 * (mlib_s64)psrc_beg[hx1 + wx2 + l]);      \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 1) {                                         \
			a2 = 111 * (mlib_s64)(*p) +                      \
				29 * (mlib_s64)(*p1) -                   \
				9 * (mlib_s64)(*p_1) -                   \
				3 * (mlib_s64)(*p2);                     \
			rez = (9 * (a + a1) - a_1 -                      \
				a2) >> 11;                       \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += dst_stride;                                  \
			i++;                                               \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 111 * (mlib_s64)(*p) +                      \
				29 * (mlib_s64)(*p1) -                   \
				9 * (mlib_s64)(*p_1) -                   \
				3 * (mlib_s64)(*p2);                     \
			rez1 = a >> 7;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = (9 * (a + a1) - a_1 -                      \
				a2) >> 11;                       \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + dst_stride) = (mlib_s32)rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = 111 * (mlib_s64)(*p) +                      \
				29 * (mlib_s64)(*p1) -                   \
				9 * (mlib_s64)(*p_1) -                   \
				3 * (mlib_s64)(*p2);                     \
			rez1 = a >> 7;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (9 * (a + a1) - a_1 -                      \
				a2) >> 11;                       \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (29 * (mlib_s64)psrc_beg[hx_1 + wx + l] +      \
			    111 * (mlib_s64)psrc_beg[hx_1 + wx1 + l] -   \
			    3 * (mlib_s64)psrc_beg[hx_1 + wx_1 + l] -    \
			    9 * (mlib_s64)psrc_beg[hx_1 + wx2 + l]);     \
		    a = (29 * (mlib_s64)psrc_beg[hx + wx + l] +          \
			    111 * (mlib_s64)psrc_beg[hx + wx1 + l] -     \
			    3 * (mlib_s64)psrc_beg[hx + wx_1 + l] -      \
			    9 * (mlib_s64)psrc_beg[hx + wx2 + l]);       \
		    a1 = (29 * (mlib_s64)psrc_beg[hx1 + wx + l] +        \
			    111 * (mlib_s64)psrc_beg[hx1 + wx1 + l] -    \
			    3 * (mlib_s64)psrc_beg[hx1 + wx_1 + l] -     \
			    9 * (mlib_s64)psrc_beg[hx1 + wx2 + l]);      \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 1) {                                         \
			a2 = 29 * (mlib_s64)(*p) +                       \
				111 * (mlib_s64)(*p1) -                  \
				3 * (mlib_s64)(*p_1) -                   \
				9 * (mlib_s64)(*p2);                     \
			rez = (9 * (a + a1) - a_1 -                      \
				a2) >> 11;                       \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += dst_stride;                                  \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 29 * (mlib_s64)(*p) +                       \
				111 * (mlib_s64)(*p1) -                  \
				3 * (mlib_s64)(*p_1) -                   \
				9 * (mlib_s64)(*p2);                     \
			rez1 = a >> 7;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = (9 * (a + a1) - a_1 -                      \
				a2) >> 11;                       \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + dst_stride) = (mlib_s32)rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = 29 * (mlib_s64)(*p) +                       \
				111 * (mlib_s64)(*p1) -                  \
				3 * (mlib_s64)(*p_1) -                   \
				9 * (mlib_s64)(*p2);                     \
			rez1 = a >> 7;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (9 * (a + a1) - a_1 -                      \
				a2) >> 11;                       \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	ROW_BICUBIC_FL2(beg_i, j, chan, amount, MIN, MAX)                  \
	{                                                                  \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    psrc_end = psrc_beg + hx + (src_width_beg - 1) * chan;         \
	    if (dh == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (111 * (mlib_s64)psrc_beg[hx + wx_1 + l] +     \
			    29 * (mlib_s64)psrc_beg[hx1 + wx_1 + l] -    \
			    9 * (mlib_s64)psrc_beg[hx_1 + wx_1 + l] -    \
			    3 * (mlib_s64)psrc_beg[hx2 + wx_1 + l]);     \
		    a = (111 * (mlib_s64)psrc_beg[hx + wx + l] +         \
			    29 * (mlib_s64)psrc_beg[hx1 + wx + l] -      \
			    9 * (mlib_s64)psrc_beg[hx_1 + wx + l] -      \
			    3 * (mlib_s64)psrc_beg[hx2 + wx + l]);       \
		    a1 = (111 * (mlib_s64)psrc_beg[hx + wx1 + l] +       \
			    29 * (mlib_s64)psrc_beg[hx1 + wx1 + l] -     \
			    9 * (mlib_s64)psrc_beg[hx_1 + wx1 + l] -     \
			    3 * (mlib_s64)psrc_beg[hx2 + wx1 + l]);      \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                       \
		    p = psrc_beg + hx + wx2 + l;                           \
		    p1 = psrc_beg + hx1 + wx2 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 1) {                                         \
			a2 = 111 * (mlib_s64)(*p) +                      \
				29 * (mlib_s64)(*p1) -                   \
				9 * (mlib_s64)(*p_1) -                   \
				3 * (mlib_s64)(*p2);                     \
			rez = (9 * (a + a1) - a_1 -                      \
				a2) >> 11;                       \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += chan;                                        \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 111 * (mlib_s64)(*p) +                      \
				29 * (mlib_s64)(*p1) -                   \
				9 * (mlib_s64)(*p_1) -                   \
				3 * (mlib_s64)(*p2);                     \
			rez1 = a >> 7;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = (9 * (a + a1) - a_1 -                      \
				a2) >> 11;                       \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + chan) = (mlib_s32)rez;                      \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = (111 * (mlib_s64)(*p) +                     \
				29 * (mlib_s64)(*p1) -                   \
				9 * (mlib_s64)(*p_1) -                   \
				3 * (mlib_s64)(*p2));                    \
			rez1 = a >> 7;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (9 * (a + a1) - a_1 -                      \
				a2) >> 11;                       \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (29 * (mlib_s64)psrc_beg[hx + wx_1 + l] +      \
			    111 * (mlib_s64)psrc_beg[hx1 + wx_1 + l] -   \
			    3 * (mlib_s64)psrc_beg[hx_1 + wx_1 + l] -    \
			    9 * (mlib_s64)psrc_beg[hx2 + wx_1 + l]);     \
		    a = (29 * (mlib_s64)psrc_beg[hx + wx + l] +          \
			    111 * (mlib_s64)psrc_beg[hx1 + wx + l] -     \
			    3 * (mlib_s64)psrc_beg[hx_1 + wx + l] -      \
			    9 * (mlib_s64)psrc_beg[hx2 + wx + l]);       \
		    a1 = (29 * (mlib_s64)psrc_beg[hx + wx1 + l] +        \
			    111 * (mlib_s64)psrc_beg[hx1 + wx1 + l] -    \
			    3 * (mlib_s64)psrc_beg[hx_1 + wx1 + l] -     \
			    9 * (mlib_s64)psrc_beg[hx2 + wx1 + l]);      \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                       \
		    p = psrc_beg + hx + wx2 + l;                           \
		    p1 = psrc_beg + hx1 + wx2 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 1) {                                         \
			a2 = 29 * (mlib_s64)(*p) +                       \
				111 * (mlib_s64)(*p1) -                  \
				3 * (mlib_s64)(*p_1) -                   \
				9 * (mlib_s64)(*p2);                     \
			rez = (9 * (a + a1) - a_1 -                      \
				a2) >> 11;                       \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += chan;                                        \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 29 * (mlib_s64)(*p) +                       \
				111 * (mlib_s64)(*p1) -                  \
				3 * (mlib_s64)(*p_1) -                   \
				9 * (mlib_s64)(*p2);                     \
			rez1 = a >> 7;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = (9 * (a + a1) - a_1 -                      \
				a2) >> 11;                       \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + chan) = (mlib_s32)rez;                      \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = 29 * (mlib_s64)(*p) +                       \
				111 * (mlib_s64)(*p1) -                  \
				3 * (mlib_s64)(*p_1) -                   \
				9 * (mlib_s64)(*p2);                     \
			rez1 = a >> 7;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (9 * (a + a1) - a_1 -                      \
				a2) >> 11;                       \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC_FL2(beg_i, j, chan, amount, MIN, MAX)              \
	{                                                                 \
	    PREPARE_BICUBIC(beg_i, j, chan);                              \
	    psrc_end =                                                    \
		    psrc_beg + wx + (src_height_beg - 1) * src_stride;    \
	    if (dw == 0) {                                                \
		for (l = 0; l < chan; l++) {                              \
		    a_1 = (mlib_s64)psrc_beg[hx_1 + wx + l];              \
		    a = (mlib_s64)psrc_beg[hx + wx + l];                  \
		    a1 = (mlib_s64)psrc_beg[hx1 + wx + l];                \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                      \
		    p = psrc_beg + hx2 + wx + l;                          \
		    p1 = psrc_beg + hx2 + wx1 + l;                        \
		    p2 = psrc_beg + hx2 + wx2 + l;                        \
		    i = (j);                                              \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +    \
			    l;                                            \
		    if (dh == 1) {                                        \
			a2 = (mlib_s64)(*p);                              \
			rez = (29 * a + 111 * a1 - 3 * a_1 -        \
				9 * a2) >> 7;                    \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
			pd += dst_stride;                                 \
			i++;                                              \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += src_stride;                            \
			    p += src_stride;                              \
			    p1 += src_stride;                             \
			    p2 += src_stride;                             \
			}                                                 \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = (mlib_s64)(*p);                              \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -       \
				3 * a2) >> 7;                    \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			rez = (29 * a + 111 * a1 - 3 * a_1 -        \
				9 * a2) >> 7;                    \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*(pd + dst_stride) = (mlib_s32)rez;               \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += src_stride;                            \
			    p += src_stride;                              \
			    p1 += src_stride;                             \
			    p2 += src_stride;                             \
			}                                                 \
			pd += (dst_stride << 1);                          \
		    }                                                     \
		    if (i < (amount)) {                                   \
			a2 = (mlib_s64)(*p);                              \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -       \
				3 * a2) >> 7;                    \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			pd += dst_stride;                                 \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (29 * a + 111 * a1 - 3 * a_1 -        \
				9 * a2) >> 7;                    \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
		    }                                                     \
		}                                                         \
	    } else {                                                      \
		for (l = 0; l < chan; l++) {                              \
		    a_1 = 9 * ((mlib_s64)psrc_beg[hx_1 + wx + l] +      \
			    (mlib_s64)psrc_beg[hx_1 + wx1 + l]) -         \
			    (mlib_s64)psrc_beg[hx_1 + wx_1 + l] -         \
			    (mlib_s64)psrc_beg[hx_1 + wx2 + l];           \
		    a = 9 * ((mlib_s64)psrc_beg[hx + wx + l] +          \
			    (mlib_s64)psrc_beg[hx + wx1 + l]) -           \
			    (mlib_s64)psrc_beg[hx + wx_1 + l] -           \
			    (mlib_s64)psrc_beg[hx + wx2 + l];             \
		    a1 = 9 * ((mlib_s64)psrc_beg[hx1 + wx + l] +        \
			    (mlib_s64)psrc_beg[hx1 + wx1 + l]) -          \
			    (mlib_s64)psrc_beg[hx1 + wx_1 + l] -          \
			    (mlib_s64)psrc_beg[hx1 + wx2 + l];            \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                      \
		    p = psrc_beg + hx2 + wx + l;                          \
		    p1 = psrc_beg + hx2 + wx1 + l;                        \
		    p2 = psrc_beg + hx2 + wx2 + l;                        \
		    i = (j);                                              \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +    \
			    l;                                            \
		    if (dh == 1) {                                        \
			a2 = 9 * ((mlib_s64)(*p) + (mlib_s64)(*p1)) -   \
				(mlib_s64)(*p_1) - (mlib_s64)(*p2);       \
			rez = (29 * a + 111 * a1 - 3 * a_1 -        \
				9 * a2) >> 11;                \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
			pd += dst_stride;                                 \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += src_stride;                            \
			    p += src_stride;                              \
			    p1 += src_stride;                             \
			    p2 += src_stride;                             \
			}                                                 \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = 9 * ((mlib_s64)(*p) + (mlib_s64)(*p1)) -   \
				(mlib_s64)(*p_1) - (mlib_s64)(*p2);       \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -       \
				3 * a2) >> 11;                \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			rez = (29 * a + 111 * a1 - 3 * a_1 -        \
				9 * a2) >> 11;                \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*(pd + dst_stride) = (mlib_s32)rez;               \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += src_stride;                            \
			    p += src_stride;                              \
			    p1 += src_stride;                             \
			    p2 += src_stride;                             \
			}                                                 \
			pd += (dst_stride << 1);                          \
		    }                                                     \
		    if (i < (amount)) {                                   \
			a2 = 9 * ((mlib_s64)(*p) + (mlib_s64)(*p1)) -   \
				(mlib_s64)(*p_1) - (mlib_s64)(*p2);       \
			rez1 = (111 * a + 29 * a1 - 9 * a_1 -       \
				3 * a2) >> 11;                \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			pd += dst_stride;                                 \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (29 * a + 111 * a1 - 3 * a_1 -        \
				9 * a2) >> 11;                \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
		    }                                                     \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

#define	ROW_BICUBIC_FL3(beg_i, j, chan, amount, MIN, MAX)                  \
	{                                                                  \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    if (dh == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (mlib_s64)psrc_beg[hx + wx_1 + l];               \
		    a = (mlib_s64)psrc_beg[hx + wx + l];                   \
		    a1 = (mlib_s64)psrc_beg[hx + wx1 + l];                 \
		    p = psrc_beg + hx + wx2 + l;                           \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 0) {                                         \
			*pd = (mlib_s32)a;                                 \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = *p;                                           \
			rez = (9 * (a + a1) - a_1 - a2) >> 4;        \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			*(pd + chan) = (mlib_s32)a1;                       \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p += chan;                                         \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			if (p >= (psrc_beg + hx + l +                      \
				src_width_beg * chan))                     \
			    a2 = (mlib_s64)psrc_beg[hx + l +               \
				    (src_width_beg - 1) * chan];           \
			else                                               \
			    a2 = *p;                                       \
			rez = (9 * (a + a1) - a_1 - a2) >> 4;        \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount))                                      \
			*pd = (mlib_s32)a1;                                \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (9 * ((mlib_s64)psrc_beg[hx + wx_1 + l] +      \
			    (mlib_s64)psrc_beg[hx1 + wx_1 + l]) -          \
			    (mlib_s64)psrc_beg[hx_1 + wx_1 + l] -          \
			    (mlib_s64)psrc_beg[hx2 + wx_1 + l]);           \
		    a = (9 * ((mlib_s64)psrc_beg[hx + wx + l] +          \
			    (mlib_s64)psrc_beg[hx1 + wx + l]) -            \
			    (mlib_s64)psrc_beg[hx_1 + wx + l] -            \
			    (mlib_s64)psrc_beg[hx2 + wx + l]);             \
		    a1 = (9 * ((mlib_s64)psrc_beg[hx + wx1 + l] +        \
			    (mlib_s64)psrc_beg[hx1 + wx1 + l]) -           \
			    (mlib_s64)psrc_beg[hx_1 + wx1 + l] -           \
			    (mlib_s64)psrc_beg[hx2 + wx1 + l]);            \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                       \
		    p = psrc_beg + hx + wx2 + l;                           \
		    p1 = psrc_beg + hx1 + wx2 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 0) {                                         \
			rez = a >> 4;                                  \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = (9 * ((mlib_s64)(*p) + (mlib_s64)(*p1)) -   \
				(mlib_s64)(*p_1) - (mlib_s64)(*p2));       \
			rez1 = (9 * (a + a1) - a_1 - a2) >> 8;   \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = a1 >> 4;                                 \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + chan) = (mlib_s32)rez;                      \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1 += chan;                                       \
			p += chan;                                         \
			p1 += chan;                                        \
			p2 += chan;                                        \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			if (p >= (psrc_beg + hx + l +                      \
				src_width_beg * chan))                     \
			    a2 = (9 * ((mlib_s64)psrc_beg[hx + l +       \
				    (src_width_beg - 1) * chan] +          \
				    (mlib_s64)psrc_beg[hx1 + l +           \
				    (src_width_beg - 1) * chan]) -         \
				    (mlib_s64)psrc_beg[hx_1 + l +          \
				    (src_width_beg - 1) * chan] -          \
				    (mlib_s64)psrc_beg[hx2 + l +           \
				    (src_width_beg - 1) * chan]);          \
			else                                               \
			    a2 = (9 * ((mlib_s64)(*p) +                  \
				    (mlib_s64)(*p1)) -                     \
				    (mlib_s64)(*p_1) - (mlib_s64)(*p2));   \
			rez1 = (9 * (a + a1) - a_1 - a2) >> 8;   \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = a1 >> 4;                                 \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC_FL3(beg_i, j, chan, amount, MIN, MAX)               \
	{                                                                  \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    if (dw == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (mlib_s64)psrc_beg[hx_1 + wx + l];               \
		    a = (mlib_s64)psrc_beg[hx + wx + l];                   \
		    a1 = (mlib_s64)psrc_beg[hx1 + wx + l];                 \
		    p = psrc_beg + hx2 + wx + l;                           \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 0) {                                         \
			*pd = (mlib_s32)a;                                 \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = *p;                                           \
			rez = (9 * (a + a1) - a_1 - a2) >> 4;        \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			*(pd + dst_stride) = (mlib_s32)a1;                 \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p += src_stride;                                   \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			if (p >= (psrc_beg + wx + l +                      \
				src_height_beg * src_stride))              \
			    a2 = (mlib_s64)psrc_beg[wx + l +               \
				    (src_height_beg - 1) * src_stride];    \
			else                                               \
			    a2 = *p;                                       \
			rez = (9 * (a + a1) - a_1 - a2) >> 4;        \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount))                                      \
			*pd = (mlib_s32)a1;                                \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (9 * ((mlib_s64)psrc_beg[hx_1 + wx + l] +      \
			    (mlib_s64)psrc_beg[hx_1 + wx1 + l]) -          \
			    (mlib_s64)psrc_beg[hx_1 + wx_1 + l] -          \
			    (mlib_s64)psrc_beg[hx_1 + wx2 + l]);           \
		    a = (9 * ((mlib_s64)psrc_beg[hx + wx + l] +          \
			    (mlib_s64)psrc_beg[hx + wx1 + l]) -            \
			    (mlib_s64)psrc_beg[hx + wx_1 + l] -            \
			    (mlib_s64)psrc_beg[hx + wx2 + l]);             \
		    a1 = (9 * ((mlib_s64)psrc_beg[hx1 + wx + l] +        \
			    (mlib_s64)psrc_beg[hx1 + wx1 + l]) -           \
			    (mlib_s64)psrc_beg[hx1 + wx_1 + l] -           \
			    (mlib_s64)psrc_beg[hx1 + wx2 + l]);            \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 0) {                                         \
			rez = a >> 4;                                  \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = (9 * ((mlib_s64)(*p) + (mlib_s64)(*p1)) -   \
				(mlib_s64)(*p_1) - (mlib_s64)(*p2));       \
			rez1 = (9 * (a + a1) - a_1 - a2) >> 8;   \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = a1 >> 4;                                 \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + dst_stride) = (mlib_s32)rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1 += src_stride;                                 \
			p += src_stride;                                   \
			p1 += src_stride;                                  \
			p2 += src_stride;                                  \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			if (p >= (psrc_beg + wx + l +                      \
				src_height_beg * src_stride))              \
			    a2 = (9 * ((mlib_s64)psrc_beg[wx + l +       \
				    (src_height_beg - 1) * src_stride] +   \
				    (mlib_s64)psrc_beg[wx1 + l +           \
				    (src_height_beg -                      \
				    1) * src_stride]) -                    \
				    (mlib_s64)psrc_beg[wx_1 + l +          \
				    (src_height_beg - 1) * src_stride] -   \
				    (mlib_s64)psrc_beg[wx2 + l +           \
				    (src_height_beg - 1) * src_stride]);   \
			else                                               \
			    a2 = (9 * ((mlib_s64)(*p) +                  \
				    (mlib_s64)(*p1)) -                     \
				    (mlib_s64)(*p_1) - (mlib_s64)(*p2));   \
			rez1 = (9 * (a + a1) - a_1 - a2) >> 8;   \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = a1 >> 4;                                 \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#else /* _NO_LONGLONG */

/* *********************************************************** */

#define	PREPARE_BICUBIC(beg_i, j, chan)                                 \
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
	mlib_s32 i, l;                                                  \
	mlib_d64 a_1, a, a1, a2 = 0., rez, rez1;                        \
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
	    wx2 = src_width_beg - 1;                                    \
	wx_1 *= chan;                                                   \
	wx *= chan;                                                     \
	wx1 *= chan;                                                    \
	wx2 *= chan

/* *********************************************************** */

#define	ROW_BICUBIC_FL0(beg_i, j, chan, amount, MIN, MAX)                  \
	{                                                                  \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    psrc_end = psrc_beg + hx + (src_width_beg - 1) * chan;         \
	    if (dh == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (111.0 * (mlib_d64)psrc_beg[hx + wx_1 + l] +     \
			    29.0 * (mlib_d64)psrc_beg[hx1 + wx_1 + l] -    \
			    9.0 * (mlib_d64)psrc_beg[hx_1 + wx_1 + l] -    \
			    3.0 * (mlib_d64)psrc_beg[hx2 + wx_1 + l]);     \
		    a = (111.0 * (mlib_d64)psrc_beg[hx + wx + l] +         \
			    29.0 * (mlib_d64)psrc_beg[hx1 + wx + l] -      \
			    9.0 * (mlib_d64)psrc_beg[hx_1 + wx + l] -      \
			    3.0 * (mlib_d64)psrc_beg[hx2 + wx + l]);       \
		    a1 = (111.0 * (mlib_d64)psrc_beg[hx + wx1 + l] +       \
			    29.0 * (mlib_d64)psrc_beg[hx1 + wx1 + l] -     \
			    9.0 * (mlib_d64)psrc_beg[hx_1 + wx1 + l] -     \
			    3.0 * (mlib_d64)psrc_beg[hx2 + wx1 + l]);      \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                       \
		    p = psrc_beg + hx + wx2 + l;                           \
		    p1 = psrc_beg + hx1 + wx2 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 1) {                                         \
			a2 = 111.0 * (mlib_d64)(*p) +                      \
				29.0 * (mlib_d64)(*p1) -                   \
				9.0 * (mlib_d64)(*p_1) -                   \
				3.0 * (mlib_d64)(*p2);                     \
			rez = (29.0 * a + 111.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.00006103515625;              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += chan;                                        \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 111.0 * (mlib_d64)(*p) +                      \
				29.0 * (mlib_d64)(*p1) -                   \
				9.0 * (mlib_d64)(*p_1) -                   \
				3.0 * (mlib_d64)(*p2);                     \
			rez1 = (111.0 * a + 29.0 * a1 - 9.0 * a_1 -        \
				3.0 * a2) * 0.00006103515625;              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = (29.0 * a + 111.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.00006103515625;              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + chan) = (mlib_s32)rez;                      \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = (111.0 * (mlib_d64)(*p) +                     \
				29.0 * (mlib_d64)(*p1) -                   \
				9.0 * (mlib_d64)(*p_1) -                   \
				3.0 * (mlib_d64)(*p2));                    \
			rez1 = (111.0 * a + 29.0 * a1 - 9.0 * a_1 -        \
				3.0 * a2) * 0.00006103515625;              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (29.0 * a + 111.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.00006103515625;              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (29.0 * (mlib_d64)psrc_beg[hx + wx_1 + l] +      \
			    111.0 * (mlib_d64)psrc_beg[hx1 + wx_1 + l] -   \
			    3.0 * (mlib_d64)psrc_beg[hx_1 + wx_1 + l] -    \
			    9.0 * (mlib_d64)psrc_beg[hx2 + wx_1 + l]);     \
		    a = (29.0 * (mlib_d64)psrc_beg[hx + wx + l] +          \
			    111.0 * (mlib_d64)psrc_beg[hx1 + wx + l] -     \
			    3.0 * (mlib_d64)psrc_beg[hx_1 + wx + l] -      \
			    9.0 * (mlib_d64)psrc_beg[hx2 + wx + l]);       \
		    a1 = (29.0 * (mlib_d64)psrc_beg[hx + wx1 + l] +        \
			    111.0 * (mlib_d64)psrc_beg[hx1 + wx1 + l] -    \
			    3.0 * (mlib_d64)psrc_beg[hx_1 + wx1 + l] -     \
			    9.0 * (mlib_d64)psrc_beg[hx2 + wx1 + l]);      \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                       \
		    p = psrc_beg + hx + wx2 + l;                           \
		    p1 = psrc_beg + hx1 + wx2 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 1) {                                         \
			a2 = 29.0 * (mlib_d64)(*p) +                       \
				111.0 * (mlib_d64)(*p1) -                  \
				3.0 * (mlib_d64)(*p_1) -                   \
				9.0 * (mlib_d64)(*p2);                     \
			rez = (29.0 * a + 111.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.00006103515625;              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += chan;                                        \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 29.0 * (mlib_d64)(*p) +                       \
				111.0 * (mlib_d64)(*p1) -                  \
				3.0 * (mlib_d64)(*p_1) -                   \
				9.0 * (mlib_d64)(*p2);                     \
			rez1 = (111.0 * a + 29.0 * a1 - 9.0 * a_1 -        \
				3.0 * a2) * 0.00006103515625;              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = (29.0 * a + 111.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.00006103515625;              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + chan) = (mlib_s32)rez;                      \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = 29.0 * (mlib_d64)(*p) +                       \
				111.0 * (mlib_d64)(*p1) -                  \
				3.0 * (mlib_d64)(*p_1) -                   \
				9.0 * (mlib_d64)(*p2);                     \
			rez1 = (111.0 * a + 29.0 * a1 - 9.0 * a_1 -        \
				3.0 * a2) * 0.00006103515625;              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (29.0 * a + 111.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.00006103515625;              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC_FL0(beg_i, j, chan, amount, MIN, MAX)               \
	{                                                                  \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    psrc_end =                                                     \
		    psrc_beg + wx + (src_height_beg - 1) * src_stride;     \
	    if (dw == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (111.0 * (mlib_d64)psrc_beg[hx_1 + wx + l] +     \
			    29.0 * (mlib_d64)psrc_beg[hx_1 + wx1 + l] -    \
			    9.0 * (mlib_d64)psrc_beg[hx_1 + wx_1 + l] -    \
			    3.0 * (mlib_d64)psrc_beg[hx_1 + wx2 + l]);     \
		    a = (111.0 * (mlib_d64)psrc_beg[hx + wx + l] +         \
			    29.0 * (mlib_d64)psrc_beg[hx + wx1 + l] -      \
			    9.0 * (mlib_d64)psrc_beg[hx + wx_1 + l] -      \
			    3.0 * (mlib_d64)psrc_beg[hx + wx2 + l]);       \
		    a1 = (111.0 * (mlib_d64)psrc_beg[hx1 + wx + l] +       \
			    29.0 * (mlib_d64)psrc_beg[hx1 + wx1 + l] -     \
			    9.0 * (mlib_d64)psrc_beg[hx1 + wx_1 + l] -     \
			    3.0 * (mlib_d64)psrc_beg[hx1 + wx2 + l]);      \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 1) {                                         \
			a2 = 111.0 * (mlib_d64)(*p) +                      \
				29.0 * (mlib_d64)(*p1) -                   \
				9.0 * (mlib_d64)(*p_1) -                   \
				3.0 * (mlib_d64)(*p2);                     \
			rez = (29.0 * a + 111.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.00006103515625;              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += dst_stride;                                  \
			i++;                                               \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 111.0 * (mlib_d64)(*p) +                      \
				29.0 * (mlib_d64)(*p1) -                   \
				9.0 * (mlib_d64)(*p_1) -                   \
				3.0 * (mlib_d64)(*p2);                     \
			rez1 = (111.0 * a + 29.0 * a1 - 9.0 * a_1 -        \
				3.0 * a2) * 0.00006103515625;              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = (29.0 * a + 111.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.00006103515625;              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + dst_stride) = (mlib_s32)rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = 111.0 * (mlib_d64)(*p) +                      \
				29.0 * (mlib_d64)(*p1) -                   \
				9.0 * (mlib_d64)(*p_1) -                   \
				3.0 * (mlib_d64)(*p2);                     \
			rez1 = (111.0 * a + 29.0 * a1 - 9.0 * a_1 -        \
				3.0 * a2) * 0.00006103515625;              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (29.0 * a + 111.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.00006103515625;              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (29.0 * (mlib_d64)psrc_beg[hx_1 + wx + l] +      \
			    111.0 * (mlib_d64)psrc_beg[hx_1 + wx1 + l] -   \
			    3.0 * (mlib_d64)psrc_beg[hx_1 + wx_1 + l] -    \
			    9.0 * (mlib_d64)psrc_beg[hx_1 + wx2 + l]);     \
		    a = (29.0 * (mlib_d64)psrc_beg[hx + wx + l] +          \
			    111.0 * (mlib_d64)psrc_beg[hx + wx1 + l] -     \
			    3.0 * (mlib_d64)psrc_beg[hx + wx_1 + l] -      \
			    9.0 * (mlib_d64)psrc_beg[hx + wx2 + l]);       \
		    a1 = (29.0 * (mlib_d64)psrc_beg[hx1 + wx + l] +        \
			    111.0 * (mlib_d64)psrc_beg[hx1 + wx1 + l] -    \
			    3.0 * (mlib_d64)psrc_beg[hx1 + wx_1 + l] -     \
			    9.0 * (mlib_d64)psrc_beg[hx1 + wx2 + l]);      \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 1) {                                         \
			a2 = 29.0 * (mlib_d64)(*p) +                       \
				111.0 * (mlib_d64)(*p1) -                  \
				3.0 * (mlib_d64)(*p_1) -                   \
				9.0 * (mlib_d64)(*p2);                     \
			rez = (29.0 * a + 111.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.00006103515625;              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += dst_stride;                                  \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 29.0 * (mlib_d64)(*p) +                       \
				111.0 * (mlib_d64)(*p1) -                  \
				3.0 * (mlib_d64)(*p_1) -                   \
				9.0 * (mlib_d64)(*p2);                     \
			rez1 = (111.0 * a + 29.0 * a1 - 9.0 * a_1 -        \
				3.0 * a2) * 0.00006103515625;              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = (29.0 * a + 111.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.00006103515625;              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + dst_stride) = (mlib_s32)rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = 29.0 * (mlib_d64)(*p) +                       \
				111.0 * (mlib_d64)(*p1) -                  \
				3.0 * (mlib_d64)(*p_1) -                   \
				9.0 * (mlib_d64)(*p2);                     \
			rez1 = (111.0 * a + 29.0 * a1 - 9.0 * a_1 -        \
				3.0 * a2) * 0.00006103515625;              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (29.0 * a + 111.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.00006103515625;              \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	ROW_BICUBIC_FL1(beg_i, j, chan, amount, MIN, MAX)                 \
	{                                                                 \
	    PREPARE_BICUBIC(beg_i, j, chan);                              \
	    psrc_end = psrc_beg + hx + (src_width_beg - 1) * chan;        \
	    if (dh == 0) {                                                \
		for (l = 0; l < chan; l++) {                              \
		    a_1 = (mlib_d64)psrc_beg[hx + wx_1 + l];              \
		    a = (mlib_d64)psrc_beg[hx + wx + l];                  \
		    a1 = (mlib_d64)psrc_beg[hx + wx1 + l];                \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                      \
		    p = psrc_beg + hx + wx2 + l;                          \
		    p1 = psrc_beg + hx1 + wx2 + l;                        \
		    p2 = psrc_beg + hx2 + wx2 + l;                        \
		    i = (beg_i);                                          \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;     \
		    if (dw == 1) {                                        \
			a2 = (mlib_d64)(*p);                              \
			rez = (29.0 * a + 111.0 * a1 - 3.0 * a_1 -        \
				9.0 * a2) * 0.0078125;                    \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
			pd += chan;                                       \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += chan;                                  \
			    p += chan;                                    \
			    p1 += chan;                                   \
			    p2 += chan;                                   \
			}                                                 \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = (mlib_d64)(*p);                              \
			rez1 = (111.0 * a + 29.0 * a1 - 9.0 * a_1 -       \
				3.0 * a2) * 0.0078125;                    \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			rez = (29.0 * a + 111.0 * a1 - 3.0 * a_1 -        \
				9.0 * a2) * 0.0078125;                    \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*(pd + chan) = (mlib_s32)rez;                     \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += chan;                                  \
			    p += chan;                                    \
			    p1 += chan;                                   \
			    p2 += chan;                                   \
			}                                                 \
			pd += (chan << 1);                                \
		    }                                                     \
		    if (i < (amount)) {                                   \
			a2 = (mlib_d64)(*p);                              \
			rez1 = (111.0 * a + 29.0 * a1 - 9.0 * a_1 -       \
				3.0 * a2) * 0.0078125;                    \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			pd += chan;                                       \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (29.0 * a + 111.0 * a1 - 3.0 * a_1 -        \
				9.0 * a2) * 0.0078125;                    \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
		    }                                                     \
		}                                                         \
	    } else {                                                      \
		for (l = 0; l < chan; l++) {                              \
		    a_1 = 9.0 * ((mlib_d64)psrc_beg[hx + wx_1 + l] +      \
			    (mlib_d64)psrc_beg[hx1 + wx_1 + l]) -         \
			    (mlib_d64)psrc_beg[hx_1 + wx_1 + l] -         \
			    (mlib_d64)psrc_beg[hx2 + wx_1 + l];           \
		    a = 9.0 * ((mlib_d64)psrc_beg[hx + wx + l] +          \
			    (mlib_d64)psrc_beg[hx1 + wx + l]) -           \
			    (mlib_d64)psrc_beg[hx_1 + wx + l] -           \
			    (mlib_d64)psrc_beg[hx2 + wx + l];             \
		    a1 = 9.0 * ((mlib_d64)psrc_beg[hx + wx1 + l] +        \
			    (mlib_d64)psrc_beg[hx1 + wx1 + l]) -          \
			    (mlib_d64)psrc_beg[hx_1 + wx1 + l] -          \
			    (mlib_d64)psrc_beg[hx2 + wx1 + l];            \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                      \
		    p = psrc_beg + hx + wx2 + l;                          \
		    p1 = psrc_beg + hx1 + wx2 + l;                        \
		    p2 = psrc_beg + hx2 + wx2 + l;                        \
		    i = (beg_i);                                          \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;     \
		    if (dw == 1) {                                        \
			a2 = 9.0 * ((mlib_d64)(*p) + (mlib_d64)(*p1)) -   \
				(mlib_d64)(*p_1) - (mlib_d64)(*p2);       \
			rez = (29.0 * a + 111.0 * a1 - 3.0 * a_1 -        \
				9.0 * a2) * 0.00048828125;                \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
			pd += chan;                                       \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += chan;                                  \
			    p += chan;                                    \
			    p1 += chan;                                   \
			    p2 += chan;                                   \
			}                                                 \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = 9.0 * ((mlib_d64)(*p) + (mlib_d64)(*p1)) -   \
				(mlib_d64)(*p_1) - (mlib_d64)(*p2);       \
			rez1 = (111.0 * a + 29.0 * a1 - 9.0 * a_1 -       \
				3.0 * a2) * 0.00048828125;                \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			rez = (29.0 * a + 111.0 * a1 - 3.0 * a_1 -        \
				9.0 * a2) * 0.00048828125;                \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*(pd + chan) = (mlib_s32)rez;                     \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += chan;                                  \
			    p += chan;                                    \
			    p1 += chan;                                   \
			    p2 += chan;                                   \
			}                                                 \
			pd += (chan << 1);                                \
		    }                                                     \
		    if (i < (amount)) {                                   \
			a2 = 9.0 * ((mlib_d64)(*p) + (mlib_d64)(*p1)) -   \
				(mlib_d64)(*p_1) - (mlib_d64)(*p2);       \
			rez1 = (111.0 * a + 29.0 * a1 - 9.0 * a_1 -       \
				3.0 * a2) * 0.00048828125;                \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			pd += chan;                                       \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (29.0 * a + 111.0 * a1 - 3.0 * a_1 -        \
				9.0 * a2) * 0.00048828125;                \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
		    }                                                     \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC_FL1(beg_i, j, chan, amount, MIN, MAX)               \
	{                                                                  \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    psrc_end =                                                     \
		    psrc_beg + wx + (src_height_beg - 1) * src_stride;     \
	    if (dw == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (111.0 * (mlib_d64)psrc_beg[hx_1 + wx + l] +     \
			    29.0 * (mlib_d64)psrc_beg[hx_1 + wx1 + l] -    \
			    9.0 * (mlib_d64)psrc_beg[hx_1 + wx_1 + l] -    \
			    3.0 * (mlib_d64)psrc_beg[hx_1 + wx2 + l]);     \
		    a = (111.0 * (mlib_d64)psrc_beg[hx + wx + l] +         \
			    29.0 * (mlib_d64)psrc_beg[hx + wx1 + l] -      \
			    9.0 * (mlib_d64)psrc_beg[hx + wx_1 + l] -      \
			    3.0 * (mlib_d64)psrc_beg[hx + wx2 + l]);       \
		    a1 = (111.0 * (mlib_d64)psrc_beg[hx1 + wx + l] +       \
			    29.0 * (mlib_d64)psrc_beg[hx1 + wx1 + l] -     \
			    9.0 * (mlib_d64)psrc_beg[hx1 + wx_1 + l] -     \
			    3.0 * (mlib_d64)psrc_beg[hx1 + wx2 + l]);      \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 1) {                                         \
			a2 = 111.0 * (mlib_d64)(*p) +                      \
				29.0 * (mlib_d64)(*p1) -                   \
				9.0 * (mlib_d64)(*p_1) -                   \
				3.0 * (mlib_d64)(*p2);                     \
			rez = (9.0 * (a + a1) - a_1 -                      \
				a2) * 0.00048828125;                       \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += dst_stride;                                  \
			i++;                                               \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 111.0 * (mlib_d64)(*p) +                      \
				29.0 * (mlib_d64)(*p1) -                   \
				9.0 * (mlib_d64)(*p_1) -                   \
				3.0 * (mlib_d64)(*p2);                     \
			rez1 = a * 0.0078125;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = (9.0 * (a + a1) - a_1 -                      \
				a2) * 0.00048828125;                       \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + dst_stride) = (mlib_s32)rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = 111.0 * (mlib_d64)(*p) +                      \
				29.0 * (mlib_d64)(*p1) -                   \
				9.0 * (mlib_d64)(*p_1) -                   \
				3.0 * (mlib_d64)(*p2);                     \
			rez1 = a * 0.0078125;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (9.0 * (a + a1) - a_1 -                      \
				a2) * 0.00048828125;                       \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (29.0 * (mlib_d64)psrc_beg[hx_1 + wx + l] +      \
			    111.0 * (mlib_d64)psrc_beg[hx_1 + wx1 + l] -   \
			    3.0 * (mlib_d64)psrc_beg[hx_1 + wx_1 + l] -    \
			    9.0 * (mlib_d64)psrc_beg[hx_1 + wx2 + l]);     \
		    a = (29.0 * (mlib_d64)psrc_beg[hx + wx + l] +          \
			    111.0 * (mlib_d64)psrc_beg[hx + wx1 + l] -     \
			    3.0 * (mlib_d64)psrc_beg[hx + wx_1 + l] -      \
			    9.0 * (mlib_d64)psrc_beg[hx + wx2 + l]);       \
		    a1 = (29.0 * (mlib_d64)psrc_beg[hx1 + wx + l] +        \
			    111.0 * (mlib_d64)psrc_beg[hx1 + wx1 + l] -    \
			    3.0 * (mlib_d64)psrc_beg[hx1 + wx_1 + l] -     \
			    9.0 * (mlib_d64)psrc_beg[hx1 + wx2 + l]);      \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 1) {                                         \
			a2 = 29.0 * (mlib_d64)(*p) +                       \
				111.0 * (mlib_d64)(*p1) -                  \
				3.0 * (mlib_d64)(*p_1) -                   \
				9.0 * (mlib_d64)(*p2);                     \
			rez = (9.0 * (a + a1) - a_1 -                      \
				a2) * 0.00048828125;                       \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += dst_stride;                                  \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 29.0 * (mlib_d64)(*p) +                       \
				111.0 * (mlib_d64)(*p1) -                  \
				3.0 * (mlib_d64)(*p_1) -                   \
				9.0 * (mlib_d64)(*p2);                     \
			rez1 = a * 0.0078125;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = (9.0 * (a + a1) - a_1 -                      \
				a2) * 0.00048828125;                       \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + dst_stride) = (mlib_s32)rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = 29.0 * (mlib_d64)(*p) +                       \
				111.0 * (mlib_d64)(*p1) -                  \
				3.0 * (mlib_d64)(*p_1) -                   \
				9.0 * (mlib_d64)(*p2);                     \
			rez1 = a * 0.0078125;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (9.0 * (a + a1) - a_1 -                      \
				a2) * 0.00048828125;                       \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	ROW_BICUBIC_FL2(beg_i, j, chan, amount, MIN, MAX)                  \
	{                                                                  \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    psrc_end = psrc_beg + hx + (src_width_beg - 1) * chan;         \
	    if (dh == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (111.0 * (mlib_d64)psrc_beg[hx + wx_1 + l] +     \
			    29.0 * (mlib_d64)psrc_beg[hx1 + wx_1 + l] -    \
			    9.0 * (mlib_d64)psrc_beg[hx_1 + wx_1 + l] -    \
			    3.0 * (mlib_d64)psrc_beg[hx2 + wx_1 + l]);     \
		    a = (111.0 * (mlib_d64)psrc_beg[hx + wx + l] +         \
			    29.0 * (mlib_d64)psrc_beg[hx1 + wx + l] -      \
			    9.0 * (mlib_d64)psrc_beg[hx_1 + wx + l] -      \
			    3.0 * (mlib_d64)psrc_beg[hx2 + wx + l]);       \
		    a1 = (111.0 * (mlib_d64)psrc_beg[hx + wx1 + l] +       \
			    29.0 * (mlib_d64)psrc_beg[hx1 + wx1 + l] -     \
			    9.0 * (mlib_d64)psrc_beg[hx_1 + wx1 + l] -     \
			    3.0 * (mlib_d64)psrc_beg[hx2 + wx1 + l]);      \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                       \
		    p = psrc_beg + hx + wx2 + l;                           \
		    p1 = psrc_beg + hx1 + wx2 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 1) {                                         \
			a2 = 111.0 * (mlib_d64)(*p) +                      \
				29.0 * (mlib_d64)(*p1) -                   \
				9.0 * (mlib_d64)(*p_1) -                   \
				3.0 * (mlib_d64)(*p2);                     \
			rez = (9.0 * (a + a1) - a_1 -                      \
				a2) * 0.00048828125;                       \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += chan;                                        \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 111.0 * (mlib_d64)(*p) +                      \
				29.0 * (mlib_d64)(*p1) -                   \
				9.0 * (mlib_d64)(*p_1) -                   \
				3.0 * (mlib_d64)(*p2);                     \
			rez1 = a * 0.0078125;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = (9.0 * (a + a1) - a_1 -                      \
				a2) * 0.00048828125;                       \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + chan) = (mlib_s32)rez;                      \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = (111.0 * (mlib_d64)(*p) +                     \
				29.0 * (mlib_d64)(*p1) -                   \
				9.0 * (mlib_d64)(*p_1) -                   \
				3.0 * (mlib_d64)(*p2));                    \
			rez1 = a * 0.0078125;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (9.0 * (a + a1) - a_1 -                      \
				a2) * 0.00048828125;                       \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (29.0 * (mlib_d64)psrc_beg[hx + wx_1 + l] +      \
			    111.0 * (mlib_d64)psrc_beg[hx1 + wx_1 + l] -   \
			    3.0 * (mlib_d64)psrc_beg[hx_1 + wx_1 + l] -    \
			    9.0 * (mlib_d64)psrc_beg[hx2 + wx_1 + l]);     \
		    a = (29.0 * (mlib_d64)psrc_beg[hx + wx + l] +          \
			    111.0 * (mlib_d64)psrc_beg[hx1 + wx + l] -     \
			    3.0 * (mlib_d64)psrc_beg[hx_1 + wx + l] -      \
			    9.0 * (mlib_d64)psrc_beg[hx2 + wx + l]);       \
		    a1 = (29.0 * (mlib_d64)psrc_beg[hx + wx1 + l] +        \
			    111.0 * (mlib_d64)psrc_beg[hx1 + wx1 + l] -    \
			    3.0 * (mlib_d64)psrc_beg[hx_1 + wx1 + l] -     \
			    9.0 * (mlib_d64)psrc_beg[hx2 + wx1 + l]);      \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                       \
		    p = psrc_beg + hx + wx2 + l;                           \
		    p1 = psrc_beg + hx1 + wx2 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 1) {                                         \
			a2 = 29.0 * (mlib_d64)(*p) +                       \
				111.0 * (mlib_d64)(*p1) -                  \
				3.0 * (mlib_d64)(*p_1) -                   \
				9.0 * (mlib_d64)(*p2);                     \
			rez = (9.0 * (a + a1) - a_1 -                      \
				a2) * 0.00048828125;                       \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += chan;                                        \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 29.0 * (mlib_d64)(*p) +                       \
				111.0 * (mlib_d64)(*p1) -                  \
				3.0 * (mlib_d64)(*p_1) -                   \
				9.0 * (mlib_d64)(*p2);                     \
			rez1 = a * 0.0078125;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = (9.0 * (a + a1) - a_1 -                      \
				a2) * 0.00048828125;                       \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + chan) = (mlib_s32)rez;                      \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = 29.0 * (mlib_d64)(*p) +                       \
				111.0 * (mlib_d64)(*p1) -                  \
				3.0 * (mlib_d64)(*p_1) -                   \
				9.0 * (mlib_d64)(*p2);                     \
			rez1 = a * 0.0078125;                              \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (9.0 * (a + a1) - a_1 -                      \
				a2) * 0.00048828125;                       \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC_FL2(beg_i, j, chan, amount, MIN, MAX)              \
	{                                                                 \
	    PREPARE_BICUBIC(beg_i, j, chan);                              \
	    psrc_end =                                                    \
		    psrc_beg + wx + (src_height_beg - 1) * src_stride;    \
	    if (dw == 0) {                                                \
		for (l = 0; l < chan; l++) {                              \
		    a_1 = (mlib_d64)psrc_beg[hx_1 + wx + l];              \
		    a = (mlib_d64)psrc_beg[hx + wx + l];                  \
		    a1 = (mlib_d64)psrc_beg[hx1 + wx + l];                \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                      \
		    p = psrc_beg + hx2 + wx + l;                          \
		    p1 = psrc_beg + hx2 + wx1 + l;                        \
		    p2 = psrc_beg + hx2 + wx2 + l;                        \
		    i = (j);                                              \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +    \
			    l;                                            \
		    if (dh == 1) {                                        \
			a2 = (mlib_d64)(*p);                              \
			rez = (29.0 * a + 111.0 * a1 - 3.0 * a_1 -        \
				9.0 * a2) * 0.0078125;                    \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
			pd += dst_stride;                                 \
			i++;                                              \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += src_stride;                            \
			    p += src_stride;                              \
			    p1 += src_stride;                             \
			    p2 += src_stride;                             \
			}                                                 \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = (mlib_d64)(*p);                              \
			rez1 = (111.0 * a + 29.0 * a1 - 9.0 * a_1 -       \
				3.0 * a2) * 0.0078125;                    \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			rez = (29.0 * a + 111.0 * a1 - 3.0 * a_1 -        \
				9.0 * a2) * 0.0078125;                    \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*(pd + dst_stride) = (mlib_s32)rez;               \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += src_stride;                            \
			    p += src_stride;                              \
			    p1 += src_stride;                             \
			    p2 += src_stride;                             \
			}                                                 \
			pd += (dst_stride << 1);                          \
		    }                                                     \
		    if (i < (amount)) {                                   \
			a2 = (mlib_d64)(*p);                              \
			rez1 = (111.0 * a + 29.0 * a1 - 9.0 * a_1 -       \
				3.0 * a2) * 0.0078125;                    \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			pd += dst_stride;                                 \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (29.0 * a + 111.0 * a1 - 3.0 * a_1 -        \
				9.0 * a2) * 0.0078125;                    \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
		    }                                                     \
		}                                                         \
	    } else {                                                      \
		for (l = 0; l < chan; l++) {                              \
		    a_1 = 9.0 * ((mlib_d64)psrc_beg[hx_1 + wx + l] +      \
			    (mlib_d64)psrc_beg[hx_1 + wx1 + l]) -         \
			    (mlib_d64)psrc_beg[hx_1 + wx_1 + l] -         \
			    (mlib_d64)psrc_beg[hx_1 + wx2 + l];           \
		    a = 9.0 * ((mlib_d64)psrc_beg[hx + wx + l] +          \
			    (mlib_d64)psrc_beg[hx + wx1 + l]) -           \
			    (mlib_d64)psrc_beg[hx + wx_1 + l] -           \
			    (mlib_d64)psrc_beg[hx + wx2 + l];             \
		    a1 = 9.0 * ((mlib_d64)psrc_beg[hx1 + wx + l] +        \
			    (mlib_d64)psrc_beg[hx1 + wx1 + l]) -          \
			    (mlib_d64)psrc_beg[hx1 + wx_1 + l] -          \
			    (mlib_d64)psrc_beg[hx1 + wx2 + l];            \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                      \
		    p = psrc_beg + hx2 + wx + l;                          \
		    p1 = psrc_beg + hx2 + wx1 + l;                        \
		    p2 = psrc_beg + hx2 + wx2 + l;                        \
		    i = (j);                                              \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +    \
			    l;                                            \
		    if (dh == 1) {                                        \
			a2 = 9.0 * ((mlib_d64)(*p) + (mlib_d64)(*p1)) -   \
				(mlib_d64)(*p_1) - (mlib_d64)(*p2);       \
			rez = (29.0 * a + 111.0 * a1 - 3.0 * a_1 -        \
				9.0 * a2) * 0.00048828125;                \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
			pd += dst_stride;                                 \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += src_stride;                            \
			    p += src_stride;                              \
			    p1 += src_stride;                             \
			    p2 += src_stride;                             \
			}                                                 \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = 9.0 * ((mlib_d64)(*p) + (mlib_d64)(*p1)) -   \
				(mlib_d64)(*p_1) - (mlib_d64)(*p2);       \
			rez1 = (111.0 * a + 29.0 * a1 - 9.0 * a_1 -       \
				3.0 * a2) * 0.00048828125;                \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			rez = (29.0 * a + 111.0 * a1 - 3.0 * a_1 -        \
				9.0 * a2) * 0.00048828125;                \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*(pd + dst_stride) = (mlib_s32)rez;               \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += src_stride;                            \
			    p += src_stride;                              \
			    p1 += src_stride;                             \
			    p2 += src_stride;                             \
			}                                                 \
			pd += (dst_stride << 1);                          \
		    }                                                     \
		    if (i < (amount)) {                                   \
			a2 = 9.0 * ((mlib_d64)(*p) + (mlib_d64)(*p1)) -   \
				(mlib_d64)(*p_1) - (mlib_d64)(*p2);       \
			rez1 = (111.0 * a + 29.0 * a1 - 9.0 * a_1 -       \
				3.0 * a2) * 0.00048828125;                \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			pd += dst_stride;                                 \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (29.0 * a + 111.0 * a1 - 3.0 * a_1 -        \
				9.0 * a2) * 0.00048828125;                \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
		    }                                                     \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

#define	ROW_BICUBIC_FL3(beg_i, j, chan, amount, MIN, MAX)                  \
	{                                                                  \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    if (dh == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (mlib_d64)psrc_beg[hx + wx_1 + l];               \
		    a = (mlib_d64)psrc_beg[hx + wx + l];                   \
		    a1 = (mlib_d64)psrc_beg[hx + wx1 + l];                 \
		    p = psrc_beg + hx + wx2 + l;                           \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 0) {                                         \
			*pd = (mlib_s32)a;                                 \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = *p;                                           \
			rez = (9.0 * (a + a1) - a_1 - a2) * 0.0625;        \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			*(pd + chan) = (mlib_s32)a1;                       \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p += chan;                                         \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			if (p >= (psrc_beg + hx + l +                      \
				src_width_beg * chan))                     \
			    a2 = (mlib_d64)psrc_beg[hx + l +               \
				    (src_width_beg - 1) * chan];           \
			else                                               \
			    a2 = *p;                                       \
			rez = (9.0 * (a + a1) - a_1 - a2) * 0.0625;        \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount))                                      \
			*pd = (mlib_s32)a1;                                \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (9.0 * ((mlib_d64)psrc_beg[hx + wx_1 + l] +      \
			    (mlib_d64)psrc_beg[hx1 + wx_1 + l]) -          \
			    (mlib_d64)psrc_beg[hx_1 + wx_1 + l] -          \
			    (mlib_d64)psrc_beg[hx2 + wx_1 + l]);           \
		    a = (9.0 * ((mlib_d64)psrc_beg[hx + wx + l] +          \
			    (mlib_d64)psrc_beg[hx1 + wx + l]) -            \
			    (mlib_d64)psrc_beg[hx_1 + wx + l] -            \
			    (mlib_d64)psrc_beg[hx2 + wx + l]);             \
		    a1 = (9.0 * ((mlib_d64)psrc_beg[hx + wx1 + l] +        \
			    (mlib_d64)psrc_beg[hx1 + wx1 + l]) -           \
			    (mlib_d64)psrc_beg[hx_1 + wx1 + l] -           \
			    (mlib_d64)psrc_beg[hx2 + wx1 + l]);            \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                       \
		    p = psrc_beg + hx + wx2 + l;                           \
		    p1 = psrc_beg + hx1 + wx2 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 0) {                                         \
			rez = a * 0.0625;                                  \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = (9.0 * ((mlib_d64)(*p) + (mlib_d64)(*p1)) -   \
				(mlib_d64)(*p_1) - (mlib_d64)(*p2));       \
			rez1 = (9.0 * (a + a1) - a_1 - a2) * 0.00390625;   \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = a1 * 0.0625;                                 \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + chan) = (mlib_s32)rez;                      \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1 += chan;                                       \
			p += chan;                                         \
			p1 += chan;                                        \
			p2 += chan;                                        \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			if (p >= (psrc_beg + hx + l +                      \
				src_width_beg * chan))                     \
			    a2 = (9.0 * ((mlib_d64)psrc_beg[hx + l +       \
				    (src_width_beg - 1) * chan] +          \
				    (mlib_d64)psrc_beg[hx1 + l +           \
				    (src_width_beg - 1) * chan]) -         \
				    (mlib_d64)psrc_beg[hx_1 + l +          \
				    (src_width_beg - 1) * chan] -          \
				    (mlib_d64)psrc_beg[hx2 + l +           \
				    (src_width_beg - 1) * chan]);          \
			else                                               \
			    a2 = (9.0 * ((mlib_d64)(*p) +                  \
				    (mlib_d64)(*p1)) -                     \
				    (mlib_d64)(*p_1) - (mlib_d64)(*p2));   \
			rez1 = (9.0 * (a + a1) - a_1 - a2) * 0.00390625;   \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = a1 * 0.0625;                                 \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC_FL3(beg_i, j, chan, amount, MIN, MAX)               \
	{                                                                  \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    if (dw == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (mlib_d64)psrc_beg[hx_1 + wx + l];               \
		    a = (mlib_d64)psrc_beg[hx + wx + l];                   \
		    a1 = (mlib_d64)psrc_beg[hx1 + wx + l];                 \
		    p = psrc_beg + hx2 + wx + l;                           \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 0) {                                         \
			*pd = (mlib_s32)a;                                 \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = *p;                                           \
			rez = (9.0 * (a + a1) - a_1 - a2) * 0.0625;        \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			*(pd + dst_stride) = (mlib_s32)a1;                 \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p += src_stride;                                   \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			if (p >= (psrc_beg + wx + l +                      \
				src_height_beg * src_stride))              \
			    a2 = (mlib_d64)psrc_beg[wx + l +               \
				    (src_height_beg - 1) * src_stride];    \
			else                                               \
			    a2 = *p;                                       \
			rez = (9.0 * (a + a1) - a_1 - a2) * 0.0625;        \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount))                                      \
			*pd = (mlib_s32)a1;                                \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (9.0 * ((mlib_d64)psrc_beg[hx_1 + wx + l] +      \
			    (mlib_d64)psrc_beg[hx_1 + wx1 + l]) -          \
			    (mlib_d64)psrc_beg[hx_1 + wx_1 + l] -          \
			    (mlib_d64)psrc_beg[hx_1 + wx2 + l]);           \
		    a = (9.0 * ((mlib_d64)psrc_beg[hx + wx + l] +          \
			    (mlib_d64)psrc_beg[hx + wx1 + l]) -            \
			    (mlib_d64)psrc_beg[hx + wx_1 + l] -            \
			    (mlib_d64)psrc_beg[hx + wx2 + l]);             \
		    a1 = (9.0 * ((mlib_d64)psrc_beg[hx1 + wx + l] +        \
			    (mlib_d64)psrc_beg[hx1 + wx1 + l]) -           \
			    (mlib_d64)psrc_beg[hx1 + wx_1 + l] -           \
			    (mlib_d64)psrc_beg[hx1 + wx2 + l]);            \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 0) {                                         \
			rez = a * 0.0625;                                  \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = (9.0 * ((mlib_d64)(*p) + (mlib_d64)(*p1)) -   \
				(mlib_d64)(*p_1) - (mlib_d64)(*p2));       \
			rez1 = (9.0 * (a + a1) - a_1 - a2) * 0.00390625;   \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = a1 * 0.0625;                                 \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + dst_stride) = (mlib_s32)rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1 += src_stride;                                 \
			p += src_stride;                                   \
			p1 += src_stride;                                  \
			p2 += src_stride;                                  \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			if (p >= (psrc_beg + wx + l +                      \
				src_height_beg * src_stride))              \
			    a2 = (9.0 * ((mlib_d64)psrc_beg[wx + l +       \
				    (src_height_beg - 1) * src_stride] +   \
				    (mlib_d64)psrc_beg[wx1 + l +           \
				    (src_height_beg -                      \
				    1) * src_stride]) -                    \
				    (mlib_d64)psrc_beg[wx_1 + l +          \
				    (src_height_beg - 1) * src_stride] -   \
				    (mlib_d64)psrc_beg[wx2 + l +           \
				    (src_height_beg - 1) * src_stride]);   \
			else                                               \
			    a2 = (9.0 * ((mlib_d64)(*p) +                  \
				    (mlib_d64)(*p1)) -                     \
				    (mlib_d64)(*p_1) - (mlib_d64)(*p2));   \
			rez1 = (9.0 * (a + a1) - a_1 - a2) * 0.00390625;   \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = a1 * 0.0625;                                 \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#endif /* _NO_LONGLONG */

void
mlib_ImageZoomIn2X_S32_SE_BC_FL0(
	MLIB_ZOOMIN2XDEFEDGE_BC(mlib_s32))
{
	const mlib_s32 *pdst_count, *p_1, *p, *p1, *p2, *psrc_end;
	mlib_s32 *pd;
	mlib_s32 ii, jj;

	pdst_count = pdst_near;
	ii = 0;
	jj = 0;

	while (((pdst - pdst_count) >= dst_stride) && (src_height_near > 0)) {
		ROW_BICUBIC_FL0(ii, jj, channels, src_width_near, MLIB_S32_MIN,
			MLIB_S32_MAX);
		jj++;
		pdst_count += dst_stride;
		src_height_near--;
	}

	while (src_height_near > src_height) {
		ROW_BICUBIC_FL0(ii, jj + src_height_near - 1, channels,
			src_width_near, MLIB_S32_MIN, MLIB_S32_MAX);
		src_height_near--;
	}

	if (src_height_near > 0) {
		while ((pdst - pdst_count > 0) && (src_width_near > 0)) {
			COLUMN_BICUBIC_FL0(ii, jj, channels,
				(src_height_near + jj), MLIB_S32_MIN,
				MLIB_S32_MAX);
			src_width_near--;
			pdst_count += channels;
			ii++;
		}

		while (src_width_near > src_width) {
			COLUMN_BICUBIC_FL0(ii + src_width_near - 1, jj,
				channels, (src_height_near + jj), MLIB_S32_MIN,
				MLIB_S32_MAX);
			src_width_near--;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIn2X_S32_SE_BC_FL1(
	MLIB_ZOOMIN2XDEFEDGE_BC(mlib_s32))
{
	const mlib_s32 *pdst_count, *p_1, *p, *p1, *p2, *psrc_end;
	mlib_s32 *pd;
	mlib_s32 ii, jj;

	pdst_count = pdst_near;
	ii = 0;
	jj = 0;

	while (((pdst - pdst_count) >= dst_stride) && (src_height_near > 0)) {
		ROW_BICUBIC_FL1(ii, jj, channels, src_width_near, MLIB_S32_MIN,
			MLIB_S32_MAX);
		jj++;
		pdst_count += dst_stride;
		src_height_near--;
	}

	while (src_height_near > src_height) {
		ROW_BICUBIC_FL1(ii, jj + src_height_near - 1, channels,
			src_width_near, MLIB_S32_MIN, MLIB_S32_MAX);
		src_height_near--;
	}

	if (src_height_near > 0) {
		while ((pdst - pdst_count > 0) && (src_width_near > 0)) {
			COLUMN_BICUBIC_FL1(ii, jj, channels,
				(src_height_near + jj), MLIB_S32_MIN,
				MLIB_S32_MAX);
			src_width_near--;
			pdst_count += channels;
			ii++;
		}

		while (src_width_near > src_width) {
			COLUMN_BICUBIC_FL1(ii + src_width_near - 1, jj,
				channels, (src_height_near + jj), MLIB_S32_MIN,
				MLIB_S32_MAX);
			src_width_near--;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIn2X_S32_SE_BC_FL2(
	MLIB_ZOOMIN2XDEFEDGE_BC(mlib_s32))
{
	mlib_s32 *pd;
	const mlib_s32 *pdst_count, *p_1, *p, *p1, *p2, *psrc_end;
	mlib_s32 ii, jj;

	pdst_count = pdst_near;
	ii = 0;
	jj = 0;

	while (((pdst - pdst_count) >= dst_stride) && (src_height_near > 0)) {
		ROW_BICUBIC_FL2(ii, jj, channels, src_width_near, MLIB_S32_MIN,
			MLIB_S32_MAX);
		jj++;
		pdst_count += dst_stride;
		src_height_near--;
	}

	while (src_height_near > src_height) {
		ROW_BICUBIC_FL2(ii, jj + src_height_near - 1, channels,
			src_width_near, MLIB_S32_MIN, MLIB_S32_MAX);
		src_height_near--;
	}

	if (src_height_near > 0) {
		while ((pdst - pdst_count > 0) && (src_width_near > 0)) {
			COLUMN_BICUBIC_FL2(ii, jj, channels,
				(src_height_near + jj), MLIB_S32_MIN,
				MLIB_S32_MAX);
			src_width_near--;
			pdst_count += channels;
			ii++;
		}

		while (src_width_near > src_width) {
			COLUMN_BICUBIC_FL2(ii + src_width_near - 1, jj,
				channels, (src_height_near + jj), MLIB_S32_MIN,
				MLIB_S32_MAX);
			src_width_near--;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIn2X_S32_SE_BC_FL3(
	MLIB_ZOOMIN2XDEFEDGE_BC(mlib_s32))
{
	const mlib_s32 *pdst_count, *p_1, *p, *p1, *p2;
	mlib_s32 *pd;
	mlib_s32 ii, jj;

	pdst_count = pdst_near;
	ii = 0;
	jj = 0;

	while (((pdst - pdst_count) >= dst_stride) && (src_height_near > 0)) {
		ROW_BICUBIC_FL3(ii, jj, channels, src_width_near, MLIB_S32_MIN,
			MLIB_S32_MAX);
		jj++;
		pdst_count += dst_stride;
		src_height_near--;
	}

	while (src_height_near > src_height) {
		ROW_BICUBIC_FL3(ii, jj + src_height_near - 1, channels,
			src_width_near, MLIB_S32_MIN, MLIB_S32_MAX);
		src_height_near--;
	}

	if (src_height_near > 0) {
		while ((pdst - pdst_count > 0) && (src_width_near > 0)) {
			COLUMN_BICUBIC_FL3(ii, jj, channels,
				(src_height_near + jj), MLIB_S32_MIN,
				MLIB_S32_MAX);
			src_width_near--;
			pdst_count += channels;
			ii++;
		}

		while (src_width_near > src_width) {
			COLUMN_BICUBIC_FL3(ii + src_width_near - 1, jj,
				channels, (src_height_near + jj), MLIB_S32_MIN,
				MLIB_S32_MAX);
			src_width_near--;
		}
	}
}

#ifndef	_NO_LONGLONG

/* *********************************************************** */

#define	ROW_BICUBIC2_FL0(beg_i, j, chan, amount, MIN, MAX)                \
	{                                                                 \
	    PREPARE_BICUBIC(beg_i, j, chan);                              \
	    psrc_end = psrc_beg + hx + (src_width_beg - 1) * chan;        \
	    if (dh == 0) {                                                \
		for (l = 0; l < chan; l++) {                              \
		    a_1 = (57 * (mlib_s64)psrc_beg[hx + wx_1 + l] +     \
			    19 * (mlib_s64)psrc_beg[hx1 + wx_1 + l] -   \
			    9 * (mlib_s64)psrc_beg[hx_1 + wx_1 + l] -   \
			    3 * (mlib_s64)psrc_beg[hx2 + wx_1 + l]);    \
		    a = (57 * (mlib_s64)psrc_beg[hx + wx + l] +         \
			    19 * (mlib_s64)psrc_beg[hx1 + wx + l] -     \
			    9 * (mlib_s64)psrc_beg[hx_1 + wx + l] -     \
			    3 * (mlib_s64)psrc_beg[hx2 + wx + l]);      \
		    a1 = (57 * (mlib_s64)psrc_beg[hx + wx1 + l] +       \
			    19 * (mlib_s64)psrc_beg[hx1 + wx1 + l] -    \
			    9 * (mlib_s64)psrc_beg[hx_1 + wx1 + l] -    \
			    3 * (mlib_s64)psrc_beg[hx2 + wx1 + l]);     \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                      \
		    p = psrc_beg + hx + wx2 + l;                          \
		    p1 = psrc_beg + hx1 + wx2 + l;                        \
		    p2 = psrc_beg + hx2 + wx2 + l;                        \
		    i = (beg_i);                                          \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;     \
		    if (dw == 1) {                                        \
			a2 = 57 * (mlib_s64)(*p) +                      \
				19 * (mlib_s64)(*p1) -                  \
				9 * (mlib_s64)(*p_1) -                  \
				3 * (mlib_s64)(*p2);                    \
			rez = (19 * a + 57 * a1 - 3 * a_1 -         \
				9 * a2) >> 12;               \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
			pd += chan;                                       \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += chan;                                  \
			    p += chan;                                    \
			    p1 += chan;                                   \
			    p2 += chan;                                   \
			}                                                 \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = 57 * (mlib_s64)(*p) +                      \
				19 * (mlib_s64)(*p1) -                  \
				9 * (mlib_s64)(*p_1) -                  \
				3 * (mlib_s64)(*p2);                    \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -        \
				3 * a2) >> 12;               \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			rez = (19 * a + 57 * a1 - 3 * a_1 -         \
				9 * a2) >> 12;               \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*(pd + chan) = (mlib_s32)rez;                     \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += chan;                                  \
			    p += chan;                                    \
			    p1 += chan;                                   \
			    p2 += chan;                                   \
			}                                                 \
			pd += (chan << 1);                                \
		    }                                                     \
		    if (i < (amount)) {                                   \
			a2 = (57 * (mlib_s64)(*p) +                     \
				19 * (mlib_s64)(*p1) -                  \
				9 * (mlib_s64)(*p_1) -                  \
				3 * (mlib_s64)(*p2));                   \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -        \
				3 * a2) >> 12;               \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			pd += chan;                                       \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (19 * a + 57 * a1 - 3 * a_1 -         \
				9 * a2) >> 12;               \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
		    }                                                     \
		}                                                         \
	    } else {                                                      \
		for (l = 0; l < chan; l++) {                              \
		    a_1 = (19 * (mlib_s64)psrc_beg[hx + wx_1 + l] +     \
			    57 * (mlib_s64)psrc_beg[hx1 + wx_1 + l] -   \
			    3 * (mlib_s64)psrc_beg[hx_1 + wx_1 + l] -   \
			    9 * (mlib_s64)psrc_beg[hx2 + wx_1 + l]);    \
		    a = (19 * (mlib_s64)psrc_beg[hx + wx + l] +         \
			    57 * (mlib_s64)psrc_beg[hx1 + wx + l] -     \
			    3 * (mlib_s64)psrc_beg[hx_1 + wx + l] -     \
			    9 * (mlib_s64)psrc_beg[hx2 + wx + l]);      \
		    a1 = (19 * (mlib_s64)psrc_beg[hx + wx1 + l] +       \
			    57 * (mlib_s64)psrc_beg[hx1 + wx1 + l] -    \
			    3 * (mlib_s64)psrc_beg[hx_1 + wx1 + l] -    \
			    9 * (mlib_s64)psrc_beg[hx2 + wx1 + l]);     \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                      \
		    p = psrc_beg + hx + wx2 + l;                          \
		    p1 = psrc_beg + hx1 + wx2 + l;                        \
		    p2 = psrc_beg + hx2 + wx2 + l;                        \
		    i = (beg_i);                                          \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;     \
		    if (dw == 1) {                                        \
			a2 = 19 * (mlib_s64)(*p) +                      \
				57 * (mlib_s64)(*p1) -                  \
				3 * (mlib_s64)(*p_1) -                  \
				9 * (mlib_s64)(*p2);                    \
			rez = (19 * a + 57 * a1 - 3 * a_1 -         \
				9 * a2) >> 12;               \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
			pd += chan;                                       \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += chan;                                  \
			    p += chan;                                    \
			    p1 += chan;                                   \
			    p2 += chan;                                   \
			}                                                 \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = 19 * (mlib_s64)(*p) +                      \
				57 * (mlib_s64)(*p1) -                  \
				3 * (mlib_s64)(*p_1) -                  \
				9 * (mlib_s64)(*p2);                    \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -        \
				3 * a2) >> 12;               \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			rez = (19 * a + 57 * a1 - 3 * a_1 -         \
				9 * a2) >> 12;               \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*(pd + chan) = (mlib_s32)rez;                     \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += chan;                                  \
			    p += chan;                                    \
			    p1 += chan;                                   \
			    p2 += chan;                                   \
			}                                                 \
			pd += (chan << 1);                                \
		    }                                                     \
		    if (i < (amount)) {                                   \
			a2 = 19 * (mlib_s64)(*p) +                      \
				57 * (mlib_s64)(*p1) -                  \
				3 * (mlib_s64)(*p_1) -                  \
				9 * (mlib_s64)(*p2);                    \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -        \
				3 * a2) >> 12;               \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			pd += chan;                                       \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (19 * a + 57 * a1 - 3 * a_1 -         \
				9 * a2) >> 12;               \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
		    }                                                     \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC2_FL0(beg_i, j, chan, amount, MIN, MAX)             \
	{                                                                 \
	    PREPARE_BICUBIC(beg_i, j, chan);                              \
	    psrc_end =                                                    \
		    psrc_beg + wx + (src_height_beg - 1) * src_stride;    \
	    if (dw == 0) {                                                \
		for (l = 0; l < chan; l++) {                              \
		    a_1 = (57 * (mlib_s64)psrc_beg[hx_1 + wx + l] +     \
			    19 * (mlib_s64)psrc_beg[hx_1 + wx1 + l] -   \
			    9 * (mlib_s64)psrc_beg[hx_1 + wx_1 + l] -   \
			    3 * (mlib_s64)psrc_beg[hx_1 + wx2 + l]);    \
		    a = (57 * (mlib_s64)psrc_beg[hx + wx + l] +         \
			    19 * (mlib_s64)psrc_beg[hx + wx1 + l] -     \
			    9 * (mlib_s64)psrc_beg[hx + wx_1 + l] -     \
			    3 * (mlib_s64)psrc_beg[hx + wx2 + l]);      \
		    a1 = (57 * (mlib_s64)psrc_beg[hx1 + wx + l] +       \
			    19 * (mlib_s64)psrc_beg[hx1 + wx1 + l] -    \
			    9 * (mlib_s64)psrc_beg[hx1 + wx_1 + l] -    \
			    3 * (mlib_s64)psrc_beg[hx1 + wx2 + l]);     \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                      \
		    p = psrc_beg + hx2 + wx + l;                          \
		    p1 = psrc_beg + hx2 + wx1 + l;                        \
		    p2 = psrc_beg + hx2 + wx2 + l;                        \
		    i = (j);                                              \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +    \
			    l;                                            \
		    if (dh == 1) {                                        \
			a2 = 57 * (mlib_s64)(*p) +                      \
				19 * (mlib_s64)(*p1) -                  \
				9 * (mlib_s64)(*p_1) -                  \
				3 * (mlib_s64)(*p2);                    \
			rez = (19 * a + 57 * a1 - 3 * a_1 -         \
				9 * a2) >> 12;               \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
			pd += dst_stride;                                 \
			i++;                                              \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += src_stride;                            \
			    p += src_stride;                              \
			    p1 += src_stride;                             \
			    p2 += src_stride;                             \
			}                                                 \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = 57 * (mlib_s64)(*p) +                      \
				19 * (mlib_s64)(*p1) -                  \
				9 * (mlib_s64)(*p_1) -                  \
				3 * (mlib_s64)(*p2);                    \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -        \
				3 * a2) >> 12;               \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			rez = (19 * a + 57 * a1 - 3 * a_1 -         \
				9 * a2) >> 12;               \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*(pd + dst_stride) = (mlib_s32)rez;               \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += src_stride;                            \
			    p += src_stride;                              \
			    p1 += src_stride;                             \
			    p2 += src_stride;                             \
			}                                                 \
			pd += (dst_stride << 1);                          \
		    }                                                     \
		    if (i < (amount)) {                                   \
			a2 = 57 * (mlib_s64)(*p) +                      \
				19 * (mlib_s64)(*p1) -                  \
				9 * (mlib_s64)(*p_1) -                  \
				3 * (mlib_s64)(*p2);                    \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -        \
				3 * a2) >> 12;               \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			pd += dst_stride;                                 \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (19 * a + 57 * a1 - 3 * a_1 -         \
				9 * a2) >> 12;               \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
		    }                                                     \
		}                                                         \
	    } else {                                                      \
		for (l = 0; l < chan; l++) {                              \
		    a_1 = (19 * (mlib_s64)psrc_beg[hx_1 + wx + l] +     \
			    57 * (mlib_s64)psrc_beg[hx_1 + wx1 + l] -   \
			    3 * (mlib_s64)psrc_beg[hx_1 + wx_1 + l] -   \
			    9 * (mlib_s64)psrc_beg[hx_1 + wx2 + l]);    \
		    a = (19 * (mlib_s64)psrc_beg[hx + wx + l] +         \
			    57 * (mlib_s64)psrc_beg[hx + wx1 + l] -     \
			    3 * (mlib_s64)psrc_beg[hx + wx_1 + l] -     \
			    9 * (mlib_s64)psrc_beg[hx + wx2 + l]);      \
		    a1 = (19 * (mlib_s64)psrc_beg[hx1 + wx + l] +       \
			    57 * (mlib_s64)psrc_beg[hx1 + wx1 + l] -    \
			    3 * (mlib_s64)psrc_beg[hx1 + wx_1 + l] -    \
			    9 * (mlib_s64)psrc_beg[hx1 + wx2 + l]);     \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                      \
		    p = psrc_beg + hx2 + wx + l;                          \
		    p1 = psrc_beg + hx2 + wx1 + l;                        \
		    p2 = psrc_beg + hx2 + wx2 + l;                        \
		    i = (j);                                              \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +    \
			    l;                                            \
		    if (dh == 1) {                                        \
			a2 = 19 * (mlib_s64)(*p) +                      \
				57 * (mlib_s64)(*p1) -                  \
				3 * (mlib_s64)(*p_1) -                  \
				9 * (mlib_s64)(*p2);                    \
			rez = (19 * a + 57 * a1 - 3 * a_1 -         \
				9 * a2) >> 12;               \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
			pd += dst_stride;                                 \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += src_stride;                            \
			    p += src_stride;                              \
			    p1 += src_stride;                             \
			    p2 += src_stride;                             \
			}                                                 \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = 19 * (mlib_s64)(*p) +                      \
				57 * (mlib_s64)(*p1) -                  \
				3 * (mlib_s64)(*p_1) -                  \
				9 * (mlib_s64)(*p2);                    \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -        \
				3 * a2) >> 12;               \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			rez = (19 * a + 57 * a1 - 3 * a_1 -         \
				9 * a2) >> 12;               \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*(pd + dst_stride) = (mlib_s32)rez;               \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += src_stride;                            \
			    p += src_stride;                              \
			    p1 += src_stride;                             \
			    p2 += src_stride;                             \
			}                                                 \
			pd += (dst_stride << 1);                          \
		    }                                                     \
		    if (i < (amount)) {                                   \
			a2 = 19 * (mlib_s64)(*p) +                      \
				57 * (mlib_s64)(*p1) -                  \
				3 * (mlib_s64)(*p_1) -                  \
				9 * (mlib_s64)(*p2);                    \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -        \
				3 * a2) >> 12;               \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			pd += dst_stride;                                 \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (19 * a + 57 * a1 - 3 * a_1 -         \
				9 * a2) >> 12;               \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
		    }                                                     \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

#define	ROW_BICUBIC2_FL1(beg_i, j, chan, amount, MIN, MAX)                \
	{                                                                 \
	    PREPARE_BICUBIC(beg_i, j, chan);                              \
	    psrc_end = psrc_beg + hx + (src_width_beg - 1) * chan;        \
	    if (dh == 0) {                                                \
		for (l = 0; l < chan; l++) {                              \
		    a_1 = (mlib_s64)psrc_beg[hx + wx_1 + l];              \
		    a = (mlib_s64)psrc_beg[hx + wx + l];                  \
		    a1 = (mlib_s64)psrc_beg[hx + wx1 + l];                \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                      \
		    p = psrc_beg + hx + wx2 + l;                          \
		    p1 = psrc_beg + hx1 + wx2 + l;                        \
		    p2 = psrc_beg + hx2 + wx2 + l;                        \
		    i = (beg_i);                                          \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;     \
		    if (dw == 1) {                                        \
			a2 = (mlib_s64)(*p);                              \
			rez = (19 * a + 57 * a1 - 3 * a_1 -         \
				9 * a2) >> 6;                     \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
			pd += chan;                                       \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += chan;                                  \
			    p += chan;                                    \
			    p1 += chan;                                   \
			    p2 += chan;                                   \
			}                                                 \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = (mlib_s64)(*p);                              \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -        \
				3 * a2) >> 6;                     \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			rez = (19 * a + 57 * a1 - 3 * a_1 -         \
				9 * a2) >> 6;                     \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*(pd + chan) = (mlib_s32)rez;                     \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += chan;                                  \
			    p += chan;                                    \
			    p1 += chan;                                   \
			    p2 += chan;                                   \
			}                                                 \
			pd += (chan << 1);                                \
		    }                                                     \
		    if (i < (amount)) {                                   \
			a2 = (mlib_s64)(*p);                              \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -        \
				3 * a2) >> 6;                     \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			pd += chan;                                       \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (19 * a + 57 * a1 - 3 * a_1 -         \
				9 * a2) >> 6;                     \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
		    }                                                     \
		}                                                         \
	    } else {                                                      \
		for (l = 0; l < chan; l++) {                              \
		    a_1 = 5 * ((mlib_s64)psrc_beg[hx + wx_1 + l] +      \
			    (mlib_s64)psrc_beg[hx1 + wx_1 + l]) -         \
			    (mlib_s64)psrc_beg[hx_1 + wx_1 + l] -         \
			    (mlib_s64)psrc_beg[hx2 + wx_1 + l];           \
		    a = 5 * ((mlib_s64)psrc_beg[hx + wx + l] +          \
			    (mlib_s64)psrc_beg[hx1 + wx + l]) -           \
			    (mlib_s64)psrc_beg[hx_1 + wx + l] -           \
			    (mlib_s64)psrc_beg[hx2 + wx + l];             \
		    a1 = 5 * ((mlib_s64)psrc_beg[hx + wx1 + l] +        \
			    (mlib_s64)psrc_beg[hx1 + wx1 + l]) -          \
			    (mlib_s64)psrc_beg[hx_1 + wx1 + l] -          \
			    (mlib_s64)psrc_beg[hx2 + wx1 + l];            \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                      \
		    p = psrc_beg + hx + wx2 + l;                          \
		    p1 = psrc_beg + hx1 + wx2 + l;                        \
		    p2 = psrc_beg + hx2 + wx2 + l;                        \
		    i = (beg_i);                                          \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;     \
		    if (dw == 1) {                                        \
			a2 = 5 * ((mlib_s64)(*p) + (mlib_s64)(*p1)) -   \
				(mlib_s64)(*p_1) - (mlib_s64)(*p2);       \
			rez = (19 * a + 57 * a1 - 3 * a_1 -         \
				9 * a2) >> 9;                  \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
			pd += chan;                                       \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += chan;                                  \
			    p += chan;                                    \
			    p1 += chan;                                   \
			    p2 += chan;                                   \
			}                                                 \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = 5 * ((mlib_s64)(*p) + (mlib_s64)(*p1)) -   \
				(mlib_s64)(*p_1) - (mlib_s64)(*p2);       \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -        \
				3 * a2) >> 9;                  \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			rez = (19 * a + 57 * a1 - 3 * a_1 -         \
				9 * a2) >> 9;                  \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*(pd + chan) = (mlib_s32)rez;                     \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += chan;                                  \
			    p += chan;                                    \
			    p1 += chan;                                   \
			    p2 += chan;                                   \
			}                                                 \
			pd += (chan << 1);                                \
		    }                                                     \
		    if (i < (amount)) {                                   \
			a2 = 5 * ((mlib_s64)(*p) + (mlib_s64)(*p1)) -   \
				(mlib_s64)(*p_1) - (mlib_s64)(*p2);       \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -        \
				3 * a2) >> 9;                  \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			pd += chan;                                       \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (19 * a + 57 * a1 - 3 * a_1 -         \
				9 * a2) >> 9;                  \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
		    }                                                     \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC2_FL1(beg_i, j, chan, amount, MIN, MAX)              \
	{                                                                  \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    psrc_end =                                                     \
		    psrc_beg + wx + (src_height_beg - 1) * src_stride;     \
	    if (dw == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (57 * (mlib_s64)psrc_beg[hx_1 + wx + l] +      \
			    19 * (mlib_s64)psrc_beg[hx_1 + wx1 + l] -    \
			    9 * (mlib_s64)psrc_beg[hx_1 + wx_1 + l] -    \
			    3 * (mlib_s64)psrc_beg[hx_1 + wx2 + l]);     \
		    a = (57 * (mlib_s64)psrc_beg[hx + wx + l] +          \
			    19 * (mlib_s64)psrc_beg[hx + wx1 + l] -      \
			    9 * (mlib_s64)psrc_beg[hx + wx_1 + l] -      \
			    3 * (mlib_s64)psrc_beg[hx + wx2 + l]);       \
		    a1 = (57 * (mlib_s64)psrc_beg[hx1 + wx + l] +        \
			    19 * (mlib_s64)psrc_beg[hx1 + wx1 + l] -     \
			    9 * (mlib_s64)psrc_beg[hx1 + wx_1 + l] -     \
			    3 * (mlib_s64)psrc_beg[hx1 + wx2 + l]);      \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 1) {                                         \
			a2 = 57 * (mlib_s64)(*p) +                       \
				19 * (mlib_s64)(*p1) -                   \
				9 * (mlib_s64)(*p_1) -                   \
				3 * (mlib_s64)(*p2);                     \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += dst_stride;                                  \
			i++;                                               \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 57 * (mlib_s64)(*p) +                       \
				19 * (mlib_s64)(*p1) -                   \
				9 * (mlib_s64)(*p_1) -                   \
				3 * (mlib_s64)(*p2);                     \
			rez1 = a >> 6;                               \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + dst_stride) = (mlib_s32)rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = 57 * (mlib_s64)(*p) +                       \
				19 * (mlib_s64)(*p1) -                   \
				9 * (mlib_s64)(*p_1) -                   \
				3 * (mlib_s64)(*p2);                     \
			rez1 = a >> 6;                               \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (19 * (mlib_s64)psrc_beg[hx_1 + wx + l] +      \
			    57 * (mlib_s64)psrc_beg[hx_1 + wx1 + l] -    \
			    3 * (mlib_s64)psrc_beg[hx_1 + wx_1 + l] -    \
			    9 * (mlib_s64)psrc_beg[hx_1 + wx2 + l]);     \
		    a = (19 * (mlib_s64)psrc_beg[hx + wx + l] +          \
			    57 * (mlib_s64)psrc_beg[hx + wx1 + l] -      \
			    3 * (mlib_s64)psrc_beg[hx + wx_1 + l] -      \
			    9 * (mlib_s64)psrc_beg[hx + wx2 + l]);       \
		    a1 = (19 * (mlib_s64)psrc_beg[hx1 + wx + l] +        \
			    57 * (mlib_s64)psrc_beg[hx1 + wx1 + l] -     \
			    3 * (mlib_s64)psrc_beg[hx1 + wx_1 + l] -     \
			    9 * (mlib_s64)psrc_beg[hx1 + wx2 + l]);      \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 1) {                                         \
			a2 = 19 * (mlib_s64)(*p) +                       \
				57 * (mlib_s64)(*p1) -                   \
				3 * (mlib_s64)(*p_1) -                   \
				9 * (mlib_s64)(*p2);                     \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += dst_stride;                                  \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 19 * (mlib_s64)(*p) +                       \
				57 * (mlib_s64)(*p1) -                   \
				3 * (mlib_s64)(*p_1) -                   \
				9 * (mlib_s64)(*p2);                     \
			rez1 = a >> 6;                               \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + dst_stride) = (mlib_s32)rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = 19 * (mlib_s64)(*p) +                       \
				57 * (mlib_s64)(*p1) -                   \
				3 * (mlib_s64)(*p_1) -                   \
				9 * (mlib_s64)(*p2);                     \
			rez1 = a >> 6;                               \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	ROW_BICUBIC2_FL2(beg_i, j, chan, amount, MIN, MAX)                 \
	{                                                                  \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    psrc_end = psrc_beg + hx + (src_width_beg - 1) * chan;         \
	    if (dh == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (57 * (mlib_s64)psrc_beg[hx + wx_1 + l] +      \
			    19 * (mlib_s64)psrc_beg[hx1 + wx_1 + l] -    \
			    9 * (mlib_s64)psrc_beg[hx_1 + wx_1 + l] -    \
			    3 * (mlib_s64)psrc_beg[hx2 + wx_1 + l]);     \
		    a = (57 * (mlib_s64)psrc_beg[hx + wx + l] +          \
			    19 * (mlib_s64)psrc_beg[hx1 + wx + l] -      \
			    9 * (mlib_s64)psrc_beg[hx_1 + wx + l] -      \
			    3 * (mlib_s64)psrc_beg[hx2 + wx + l]);       \
		    a1 = (57 * (mlib_s64)psrc_beg[hx + wx1 + l] +        \
			    19 * (mlib_s64)psrc_beg[hx1 + wx1 + l] -     \
			    9 * (mlib_s64)psrc_beg[hx_1 + wx1 + l] -     \
			    3 * (mlib_s64)psrc_beg[hx2 + wx1 + l]);      \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                       \
		    p = psrc_beg + hx + wx2 + l;                           \
		    p1 = psrc_beg + hx1 + wx2 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 1) {                                         \
			a2 = 57 * (mlib_s64)(*p) +                       \
				19 * (mlib_s64)(*p1) -                   \
				9 * (mlib_s64)(*p_1) -                   \
				3 * (mlib_s64)(*p2);                     \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += chan;                                        \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 57 * (mlib_s64)(*p) +                       \
				19 * (mlib_s64)(*p1) -                   \
				9 * (mlib_s64)(*p_1) -                   \
				3 * (mlib_s64)(*p2);                     \
			rez1 = a >> 6;                               \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + chan) = (mlib_s32)rez;                      \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = (57 * (mlib_s64)(*p) +                      \
				19 * (mlib_s64)(*p1) -                   \
				9 * (mlib_s64)(*p_1) -                   \
				3 * (mlib_s64)(*p2));                    \
			rez1 = a >> 6;                               \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (19 * (mlib_s64)psrc_beg[hx + wx_1 + l] +      \
			    57 * (mlib_s64)psrc_beg[hx1 + wx_1 + l] -    \
			    3 * (mlib_s64)psrc_beg[hx_1 + wx_1 + l] -    \
			    9 * (mlib_s64)psrc_beg[hx2 + wx_1 + l]);     \
		    a = (19 * (mlib_s64)psrc_beg[hx + wx + l] +          \
			    57 * (mlib_s64)psrc_beg[hx1 + wx + l] -      \
			    3 * (mlib_s64)psrc_beg[hx_1 + wx + l] -      \
			    9 * (mlib_s64)psrc_beg[hx2 + wx + l]);       \
		    a1 = (19 * (mlib_s64)psrc_beg[hx + wx1 + l] +        \
			    57 * (mlib_s64)psrc_beg[hx1 + wx1 + l] -     \
			    3 * (mlib_s64)psrc_beg[hx_1 + wx1 + l] -     \
			    9 * (mlib_s64)psrc_beg[hx2 + wx1 + l]);      \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                       \
		    p = psrc_beg + hx + wx2 + l;                           \
		    p1 = psrc_beg + hx1 + wx2 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 1) {                                         \
			a2 = 19 * (mlib_s64)(*p) +                       \
				57 * (mlib_s64)(*p1) -                   \
				3 * (mlib_s64)(*p_1) -                   \
				9 * (mlib_s64)(*p2);                     \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += chan;                                        \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 19 * (mlib_s64)(*p) +                       \
				57 * (mlib_s64)(*p1) -                   \
				3 * (mlib_s64)(*p_1) -                   \
				9 * (mlib_s64)(*p2);                     \
			rez1 = a >> 6;                               \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + chan) = (mlib_s32)rez;                      \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = 19 * (mlib_s64)(*p) +                       \
				57 * (mlib_s64)(*p1) -                   \
				3 * (mlib_s64)(*p_1) -                   \
				9 * (mlib_s64)(*p2);                     \
			rez1 = a >> 6;                               \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (5 * (a + a1) - a_1 - a2) >> 9;   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC2_FL2(beg_i, j, chan, amount, MIN, MAX)             \
	{                                                                 \
	    PREPARE_BICUBIC(beg_i, j, chan);                              \
	    psrc_end =                                                    \
		    psrc_beg + wx + (src_height_beg - 1) * src_stride;    \
	    if (dw == 0) {                                                \
		for (l = 0; l < chan; l++) {                              \
		    a_1 = (mlib_s64)psrc_beg[hx_1 + wx + l];              \
		    a = (mlib_s64)psrc_beg[hx + wx + l];                  \
		    a1 = (mlib_s64)psrc_beg[hx1 + wx + l];                \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                      \
		    p = psrc_beg + hx2 + wx + l;                          \
		    p1 = psrc_beg + hx2 + wx1 + l;                        \
		    p2 = psrc_beg + hx2 + wx2 + l;                        \
		    i = (j);                                              \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +    \
			    l;                                            \
		    if (dh == 1) {                                        \
			a2 = (mlib_s64)(*p);                              \
			rez = (19 * a + 57 * a1 - 3 * a_1 -         \
				9 * a2) >> 6;                     \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
			pd += dst_stride;                                 \
			i++;                                              \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += src_stride;                            \
			    p += src_stride;                              \
			    p1 += src_stride;                             \
			    p2 += src_stride;                             \
			}                                                 \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = (mlib_s64)(*p);                              \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -        \
				3 * a2) >> 6;                     \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			rez = (19 * a + 57 * a1 - 3 * a_1 -         \
				9 * a2) >> 6;                     \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*(pd + dst_stride) = (mlib_s32)rez;               \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += src_stride;                            \
			    p += src_stride;                              \
			    p1 += src_stride;                             \
			    p2 += src_stride;                             \
			}                                                 \
			pd += (dst_stride << 1);                          \
		    }                                                     \
		    if (i < (amount)) {                                   \
			a2 = (mlib_s64)(*p);                              \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -        \
				3 * a2) >> 6;                     \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			pd += dst_stride;                                 \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (19 * a + 57 * a1 - 3 * a_1 -         \
				9 * a2) >> 6;                     \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
		    }                                                     \
		}                                                         \
	    } else {                                                      \
		for (l = 0; l < chan; l++) {                              \
		    a_1 = 5 * ((mlib_s64)psrc_beg[hx_1 + wx + l] +      \
			    (mlib_s64)psrc_beg[hx_1 + wx1 + l]) -         \
			    (mlib_s64)psrc_beg[hx_1 + wx_1 + l] -         \
			    (mlib_s64)psrc_beg[hx_1 + wx2 + l];           \
		    a = 5 * ((mlib_s64)psrc_beg[hx + wx + l] +          \
			    (mlib_s64)psrc_beg[hx + wx1 + l]) -           \
			    (mlib_s64)psrc_beg[hx + wx_1 + l] -           \
			    (mlib_s64)psrc_beg[hx + wx2 + l];             \
		    a1 = 5 * ((mlib_s64)psrc_beg[hx1 + wx + l] +        \
			    (mlib_s64)psrc_beg[hx1 + wx1 + l]) -          \
			    (mlib_s64)psrc_beg[hx1 + wx_1 + l] -          \
			    (mlib_s64)psrc_beg[hx1 + wx2 + l];            \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                      \
		    p = psrc_beg + hx2 + wx + l;                          \
		    p1 = psrc_beg + hx2 + wx1 + l;                        \
		    p2 = psrc_beg + hx2 + wx2 + l;                        \
		    i = (j);                                              \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +    \
			    l;                                            \
		    if (dh == 1) {                                        \
			a2 = 5 * ((mlib_s64)(*p) + (mlib_s64)(*p1)) -   \
				(mlib_s64)(*p_1) - (mlib_s64)(*p2);       \
			rez = (19 * a + 57 * a1 - 3 * a_1 -         \
				9 * a2) >> 9;                  \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
			pd += dst_stride;                                 \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += src_stride;                            \
			    p += src_stride;                              \
			    p1 += src_stride;                             \
			    p2 += src_stride;                             \
			}                                                 \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = 5 * ((mlib_s64)(*p) + (mlib_s64)(*p1)) -   \
				(mlib_s64)(*p_1) - (mlib_s64)(*p2);       \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -        \
				3 * a2) >> 9;                  \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			rez = (19 * a + 57 * a1 - 3 * a_1 -         \
				9 * a2) >> 9;                  \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*(pd + dst_stride) = (mlib_s32)rez;               \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += src_stride;                            \
			    p += src_stride;                              \
			    p1 += src_stride;                             \
			    p2 += src_stride;                             \
			}                                                 \
			pd += (dst_stride << 1);                          \
		    }                                                     \
		    if (i < (amount)) {                                   \
			a2 = 5 * ((mlib_s64)(*p) + (mlib_s64)(*p1)) -   \
				(mlib_s64)(*p_1) - (mlib_s64)(*p2);       \
			rez1 = (57 * a + 19 * a1 - 9 * a_1 -        \
				3 * a2) >> 9;                  \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			pd += dst_stride;                                 \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (19 * a + 57 * a1 - 3 * a_1 -         \
				9 * a2) >> 9;                  \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
		    }                                                     \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

#define	ROW_BICUBIC2_FL3(beg_i, j, chan, amount, MIN, MAX)                 \
	{                                                                  \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    if (dh == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (mlib_s64)psrc_beg[hx + wx_1 + l];               \
		    a = (mlib_s64)psrc_beg[hx + wx + l];                   \
		    a1 = (mlib_s64)psrc_beg[hx + wx1 + l];                 \
		    p = psrc_beg + hx + wx2 + l;                           \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 0) {                                         \
			*pd = (mlib_s32)a;                                 \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = *p;                                           \
			rez = (5 * (a + a1) - a_1 - a2) >> 3;         \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			*(pd + chan) = (mlib_s32)a1;                       \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p += chan;                                         \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			if (p >= (psrc_beg + hx + l +                      \
				src_width_beg * chan))                     \
			    a2 = (mlib_s64)psrc_beg[hx + l +               \
				    (src_width_beg - 1) * chan];           \
			else                                               \
			    a2 = *p;                                       \
			rez = (5 * (a + a1) - a_1 - a2) >> 3;         \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount))                                      \
			*pd = (mlib_s32)a1;                                \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (5 * ((mlib_s64)psrc_beg[hx + wx_1 + l] +      \
			    (mlib_s64)psrc_beg[hx1 + wx_1 + l]) -          \
			    (mlib_s64)psrc_beg[hx_1 + wx_1 + l] -          \
			    (mlib_s64)psrc_beg[hx2 + wx_1 + l]);           \
		    a = (5 * ((mlib_s64)psrc_beg[hx + wx + l] +          \
			    (mlib_s64)psrc_beg[hx1 + wx + l]) -            \
			    (mlib_s64)psrc_beg[hx_1 + wx + l] -            \
			    (mlib_s64)psrc_beg[hx2 + wx + l]);             \
		    a1 = (5 * ((mlib_s64)psrc_beg[hx + wx1 + l] +        \
			    (mlib_s64)psrc_beg[hx1 + wx1 + l]) -           \
			    (mlib_s64)psrc_beg[hx_1 + wx1 + l] -           \
			    (mlib_s64)psrc_beg[hx2 + wx1 + l]);            \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                       \
		    p = psrc_beg + hx + wx2 + l;                           \
		    p1 = psrc_beg + hx1 + wx2 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 0) {                                         \
			rez = a >> 3;                                   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = (5 * ((mlib_s64)(*p) + (mlib_s64)(*p1)) -   \
				(mlib_s64)(*p_1) - (mlib_s64)(*p2));       \
			rez1 = (5 * (a + a1) - a_1 - a2) >> 6;     \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = a1 >> 3;                                  \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + chan) = (mlib_s32)rez;                      \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1 += chan;                                       \
			p += chan;                                         \
			p1 += chan;                                        \
			p2 += chan;                                        \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			if (p >= (psrc_beg + hx + l +                      \
				src_width_beg * chan))                     \
			    a2 = (5 * ((mlib_s64)psrc_beg[hx + l +       \
				    (src_width_beg - 1) * chan] +          \
				    (mlib_s64)psrc_beg[hx1 + l +           \
				    (src_width_beg - 1) * chan]) -         \
				    (mlib_s64)psrc_beg[hx_1 + l +          \
				    (src_width_beg - 1) * chan] -          \
				    (mlib_s64)psrc_beg[hx2 + l +           \
				    (src_width_beg - 1) * chan]);          \
			else                                               \
			    a2 = (5 * ((mlib_s64)(*p) +                  \
				    (mlib_s64)(*p1)) -                     \
				    (mlib_s64)(*p_1) - (mlib_s64)(*p2));   \
			rez1 = (5 * (a + a1) - a_1 - a2) >> 6;     \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = a1 >> 3;                                  \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC2_FL3(beg_i, j, chan, amount, MIN, MAX)              \
	{                                                                  \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    if (dw == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (mlib_s64)psrc_beg[hx_1 + wx + l];               \
		    a = (mlib_s64)psrc_beg[hx + wx + l];                   \
		    a1 = (mlib_s64)psrc_beg[hx1 + wx + l];                 \
		    p = psrc_beg + hx2 + wx + l;                           \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 0) {                                         \
			*pd = (mlib_s32)a;                                 \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = *p;                                           \
			rez = (5 * (a + a1) - a_1 - a2) >> 3;         \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			*(pd + dst_stride) = (mlib_s32)a1;                 \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p += src_stride;                                   \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			if (p >= (psrc_beg + wx + l +                      \
				src_height_beg * src_stride))              \
			    a2 = (mlib_s64)psrc_beg[wx + l +               \
				    (src_height_beg - 1) * src_stride];    \
			else                                               \
			    a2 = *p;                                       \
			rez = (5 * (a + a1) - a_1 - a2) >> 3;         \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount))                                      \
			*pd = (mlib_s32)a1;                                \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (5 * ((mlib_s64)psrc_beg[hx_1 + wx + l] +      \
			    (mlib_s64)psrc_beg[hx_1 + wx1 + l]) -          \
			    (mlib_s64)psrc_beg[hx_1 + wx_1 + l] -          \
			    (mlib_s64)psrc_beg[hx_1 + wx2 + l]);           \
		    a = (5 * ((mlib_s64)psrc_beg[hx + wx + l] +          \
			    (mlib_s64)psrc_beg[hx + wx1 + l]) -            \
			    (mlib_s64)psrc_beg[hx + wx_1 + l] -            \
			    (mlib_s64)psrc_beg[hx + wx2 + l]);             \
		    a1 = (5 * ((mlib_s64)psrc_beg[hx1 + wx + l] +        \
			    (mlib_s64)psrc_beg[hx1 + wx1 + l]) -           \
			    (mlib_s64)psrc_beg[hx1 + wx_1 + l] -           \
			    (mlib_s64)psrc_beg[hx1 + wx2 + l]);            \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 0) {                                         \
			rez = a >> 3;                                   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = (5 * ((mlib_s64)(*p) + (mlib_s64)(*p1)) -   \
				(mlib_s64)(*p_1) - (mlib_s64)(*p2));       \
			rez1 = (5 * (a + a1) - a_1 - a2) >> 6;     \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = a1 >> 3;                                  \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + dst_stride) = (mlib_s32)rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1 += src_stride;                                 \
			p += src_stride;                                   \
			p1 += src_stride;                                  \
			p2 += src_stride;                                  \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			if (p >= (psrc_beg + wx + l +                      \
				src_height_beg * src_stride))              \
			    a2 = (5 * ((mlib_s64)psrc_beg[wx + l +       \
				    (src_height_beg - 1) * src_stride] +   \
				    (mlib_s64)psrc_beg[wx1 + l +           \
				    (src_height_beg -                      \
				    1) * src_stride]) -                    \
				    (mlib_s64)psrc_beg[wx_1 + l +          \
				    (src_height_beg - 1) * src_stride] -   \
				    (mlib_s64)psrc_beg[wx2 + l +           \
				    (src_height_beg - 1) * src_stride]);   \
			else                                               \
			    a2 = (5 * ((mlib_s64)(*p) +                  \
				    (mlib_s64)(*p1)) -                     \
				    (mlib_s64)(*p_1) - (mlib_s64)(*p2));   \
			rez1 = (5 * (a + a1) - a_1 - a2) >> 6;     \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = a1 >> 3;                                  \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#else /* _NO_LONGLONG */

/* *********************************************************** */

#define	ROW_BICUBIC2_FL0(beg_i, j, chan, amount, MIN, MAX)                \
	{                                                                 \
	    PREPARE_BICUBIC(beg_i, j, chan);                              \
	    psrc_end = psrc_beg + hx + (src_width_beg - 1) * chan;        \
	    if (dh == 0) {                                                \
		for (l = 0; l < chan; l++) {                              \
		    a_1 = (57.0 * (mlib_d64)psrc_beg[hx + wx_1 + l] +     \
			    19.0 * (mlib_d64)psrc_beg[hx1 + wx_1 + l] -   \
			    9.0 * (mlib_d64)psrc_beg[hx_1 + wx_1 + l] -   \
			    3.0 * (mlib_d64)psrc_beg[hx2 + wx_1 + l]);    \
		    a = (57.0 * (mlib_d64)psrc_beg[hx + wx + l] +         \
			    19.0 * (mlib_d64)psrc_beg[hx1 + wx + l] -     \
			    9.0 * (mlib_d64)psrc_beg[hx_1 + wx + l] -     \
			    3.0 * (mlib_d64)psrc_beg[hx2 + wx + l]);      \
		    a1 = (57.0 * (mlib_d64)psrc_beg[hx + wx1 + l] +       \
			    19.0 * (mlib_d64)psrc_beg[hx1 + wx1 + l] -    \
			    9.0 * (mlib_d64)psrc_beg[hx_1 + wx1 + l] -    \
			    3.0 * (mlib_d64)psrc_beg[hx2 + wx1 + l]);     \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                      \
		    p = psrc_beg + hx + wx2 + l;                          \
		    p1 = psrc_beg + hx1 + wx2 + l;                        \
		    p2 = psrc_beg + hx2 + wx2 + l;                        \
		    i = (beg_i);                                          \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;     \
		    if (dw == 1) {                                        \
			a2 = 57.0 * (mlib_d64)(*p) +                      \
				19.0 * (mlib_d64)(*p1) -                  \
				9.0 * (mlib_d64)(*p_1) -                  \
				3.0 * (mlib_d64)(*p2);                    \
			rez = (19.0 * a + 57.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.000244140625;               \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
			pd += chan;                                       \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += chan;                                  \
			    p += chan;                                    \
			    p1 += chan;                                   \
			    p2 += chan;                                   \
			}                                                 \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = 57.0 * (mlib_d64)(*p) +                      \
				19.0 * (mlib_d64)(*p1) -                  \
				9.0 * (mlib_d64)(*p_1) -                  \
				3.0 * (mlib_d64)(*p2);                    \
			rez1 = (57.0 * a + 19.0 * a1 - 9.0 * a_1 -        \
				3.0 * a2) * 0.000244140625;               \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			rez = (19.0 * a + 57.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.000244140625;               \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*(pd + chan) = (mlib_s32)rez;                     \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += chan;                                  \
			    p += chan;                                    \
			    p1 += chan;                                   \
			    p2 += chan;                                   \
			}                                                 \
			pd += (chan << 1);                                \
		    }                                                     \
		    if (i < (amount)) {                                   \
			a2 = (57.0 * (mlib_d64)(*p) +                     \
				19.0 * (mlib_d64)(*p1) -                  \
				9.0 * (mlib_d64)(*p_1) -                  \
				3.0 * (mlib_d64)(*p2));                   \
			rez1 = (57.0 * a + 19.0 * a1 - 9.0 * a_1 -        \
				3.0 * a2) * 0.000244140625;               \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			pd += chan;                                       \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (19.0 * a + 57.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.000244140625;               \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
		    }                                                     \
		}                                                         \
	    } else {                                                      \
		for (l = 0; l < chan; l++) {                              \
		    a_1 = (19.0 * (mlib_d64)psrc_beg[hx + wx_1 + l] +     \
			    57.0 * (mlib_d64)psrc_beg[hx1 + wx_1 + l] -   \
			    3.0 * (mlib_d64)psrc_beg[hx_1 + wx_1 + l] -   \
			    9.0 * (mlib_d64)psrc_beg[hx2 + wx_1 + l]);    \
		    a = (19.0 * (mlib_d64)psrc_beg[hx + wx + l] +         \
			    57.0 * (mlib_d64)psrc_beg[hx1 + wx + l] -     \
			    3.0 * (mlib_d64)psrc_beg[hx_1 + wx + l] -     \
			    9.0 * (mlib_d64)psrc_beg[hx2 + wx + l]);      \
		    a1 = (19.0 * (mlib_d64)psrc_beg[hx + wx1 + l] +       \
			    57.0 * (mlib_d64)psrc_beg[hx1 + wx1 + l] -    \
			    3.0 * (mlib_d64)psrc_beg[hx_1 + wx1 + l] -    \
			    9.0 * (mlib_d64)psrc_beg[hx2 + wx1 + l]);     \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                      \
		    p = psrc_beg + hx + wx2 + l;                          \
		    p1 = psrc_beg + hx1 + wx2 + l;                        \
		    p2 = psrc_beg + hx2 + wx2 + l;                        \
		    i = (beg_i);                                          \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;     \
		    if (dw == 1) {                                        \
			a2 = 19.0 * (mlib_d64)(*p) +                      \
				57.0 * (mlib_d64)(*p1) -                  \
				3.0 * (mlib_d64)(*p_1) -                  \
				9.0 * (mlib_d64)(*p2);                    \
			rez = (19.0 * a + 57.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.000244140625;               \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
			pd += chan;                                       \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += chan;                                  \
			    p += chan;                                    \
			    p1 += chan;                                   \
			    p2 += chan;                                   \
			}                                                 \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = 19.0 * (mlib_d64)(*p) +                      \
				57.0 * (mlib_d64)(*p1) -                  \
				3.0 * (mlib_d64)(*p_1) -                  \
				9.0 * (mlib_d64)(*p2);                    \
			rez1 = (57.0 * a + 19.0 * a1 - 9.0 * a_1 -        \
				3.0 * a2) * 0.000244140625;               \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			rez = (19.0 * a + 57.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.000244140625;               \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*(pd + chan) = (mlib_s32)rez;                     \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += chan;                                  \
			    p += chan;                                    \
			    p1 += chan;                                   \
			    p2 += chan;                                   \
			}                                                 \
			pd += (chan << 1);                                \
		    }                                                     \
		    if (i < (amount)) {                                   \
			a2 = 19.0 * (mlib_d64)(*p) +                      \
				57.0 * (mlib_d64)(*p1) -                  \
				3.0 * (mlib_d64)(*p_1) -                  \
				9.0 * (mlib_d64)(*p2);                    \
			rez1 = (57.0 * a + 19.0 * a1 - 9.0 * a_1 -        \
				3.0 * a2) * 0.000244140625;               \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			pd += chan;                                       \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (19.0 * a + 57.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.000244140625;               \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
		    }                                                     \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC2_FL0(beg_i, j, chan, amount, MIN, MAX)             \
	{                                                                 \
	    PREPARE_BICUBIC(beg_i, j, chan);                              \
	    psrc_end =                                                    \
		    psrc_beg + wx + (src_height_beg - 1) * src_stride;    \
	    if (dw == 0) {                                                \
		for (l = 0; l < chan; l++) {                              \
		    a_1 = (57.0 * (mlib_d64)psrc_beg[hx_1 + wx + l] +     \
			    19.0 * (mlib_d64)psrc_beg[hx_1 + wx1 + l] -   \
			    9.0 * (mlib_d64)psrc_beg[hx_1 + wx_1 + l] -   \
			    3.0 * (mlib_d64)psrc_beg[hx_1 + wx2 + l]);    \
		    a = (57.0 * (mlib_d64)psrc_beg[hx + wx + l] +         \
			    19.0 * (mlib_d64)psrc_beg[hx + wx1 + l] -     \
			    9.0 * (mlib_d64)psrc_beg[hx + wx_1 + l] -     \
			    3.0 * (mlib_d64)psrc_beg[hx + wx2 + l]);      \
		    a1 = (57.0 * (mlib_d64)psrc_beg[hx1 + wx + l] +       \
			    19.0 * (mlib_d64)psrc_beg[hx1 + wx1 + l] -    \
			    9.0 * (mlib_d64)psrc_beg[hx1 + wx_1 + l] -    \
			    3.0 * (mlib_d64)psrc_beg[hx1 + wx2 + l]);     \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                      \
		    p = psrc_beg + hx2 + wx + l;                          \
		    p1 = psrc_beg + hx2 + wx1 + l;                        \
		    p2 = psrc_beg + hx2 + wx2 + l;                        \
		    i = (j);                                              \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +    \
			    l;                                            \
		    if (dh == 1) {                                        \
			a2 = 57.0 * (mlib_d64)(*p) +                      \
				19.0 * (mlib_d64)(*p1) -                  \
				9.0 * (mlib_d64)(*p_1) -                  \
				3.0 * (mlib_d64)(*p2);                    \
			rez = (19.0 * a + 57.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.000244140625;               \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
			pd += dst_stride;                                 \
			i++;                                              \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += src_stride;                            \
			    p += src_stride;                              \
			    p1 += src_stride;                             \
			    p2 += src_stride;                             \
			}                                                 \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = 57.0 * (mlib_d64)(*p) +                      \
				19.0 * (mlib_d64)(*p1) -                  \
				9.0 * (mlib_d64)(*p_1) -                  \
				3.0 * (mlib_d64)(*p2);                    \
			rez1 = (57.0 * a + 19.0 * a1 - 9.0 * a_1 -        \
				3.0 * a2) * 0.000244140625;               \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			rez = (19.0 * a + 57.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.000244140625;               \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*(pd + dst_stride) = (mlib_s32)rez;               \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += src_stride;                            \
			    p += src_stride;                              \
			    p1 += src_stride;                             \
			    p2 += src_stride;                             \
			}                                                 \
			pd += (dst_stride << 1);                          \
		    }                                                     \
		    if (i < (amount)) {                                   \
			a2 = 57.0 * (mlib_d64)(*p) +                      \
				19.0 * (mlib_d64)(*p1) -                  \
				9.0 * (mlib_d64)(*p_1) -                  \
				3.0 * (mlib_d64)(*p2);                    \
			rez1 = (57.0 * a + 19.0 * a1 - 9.0 * a_1 -        \
				3.0 * a2) * 0.000244140625;               \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			pd += dst_stride;                                 \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (19.0 * a + 57.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.000244140625;               \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
		    }                                                     \
		}                                                         \
	    } else {                                                      \
		for (l = 0; l < chan; l++) {                              \
		    a_1 = (19.0 * (mlib_d64)psrc_beg[hx_1 + wx + l] +     \
			    57.0 * (mlib_d64)psrc_beg[hx_1 + wx1 + l] -   \
			    3.0 * (mlib_d64)psrc_beg[hx_1 + wx_1 + l] -   \
			    9.0 * (mlib_d64)psrc_beg[hx_1 + wx2 + l]);    \
		    a = (19.0 * (mlib_d64)psrc_beg[hx + wx + l] +         \
			    57.0 * (mlib_d64)psrc_beg[hx + wx1 + l] -     \
			    3.0 * (mlib_d64)psrc_beg[hx + wx_1 + l] -     \
			    9.0 * (mlib_d64)psrc_beg[hx + wx2 + l]);      \
		    a1 = (19.0 * (mlib_d64)psrc_beg[hx1 + wx + l] +       \
			    57.0 * (mlib_d64)psrc_beg[hx1 + wx1 + l] -    \
			    3.0 * (mlib_d64)psrc_beg[hx1 + wx_1 + l] -    \
			    9.0 * (mlib_d64)psrc_beg[hx1 + wx2 + l]);     \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                      \
		    p = psrc_beg + hx2 + wx + l;                          \
		    p1 = psrc_beg + hx2 + wx1 + l;                        \
		    p2 = psrc_beg + hx2 + wx2 + l;                        \
		    i = (j);                                              \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +    \
			    l;                                            \
		    if (dh == 1) {                                        \
			a2 = 19.0 * (mlib_d64)(*p) +                      \
				57.0 * (mlib_d64)(*p1) -                  \
				3.0 * (mlib_d64)(*p_1) -                  \
				9.0 * (mlib_d64)(*p2);                    \
			rez = (19.0 * a + 57.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.000244140625;               \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
			pd += dst_stride;                                 \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += src_stride;                            \
			    p += src_stride;                              \
			    p1 += src_stride;                             \
			    p2 += src_stride;                             \
			}                                                 \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = 19.0 * (mlib_d64)(*p) +                      \
				57.0 * (mlib_d64)(*p1) -                  \
				3.0 * (mlib_d64)(*p_1) -                  \
				9.0 * (mlib_d64)(*p2);                    \
			rez1 = (57.0 * a + 19.0 * a1 - 9.0 * a_1 -        \
				3.0 * a2) * 0.000244140625;               \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			rez = (19.0 * a + 57.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.000244140625;               \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*(pd + dst_stride) = (mlib_s32)rez;               \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += src_stride;                            \
			    p += src_stride;                              \
			    p1 += src_stride;                             \
			    p2 += src_stride;                             \
			}                                                 \
			pd += (dst_stride << 1);                          \
		    }                                                     \
		    if (i < (amount)) {                                   \
			a2 = 19.0 * (mlib_d64)(*p) +                      \
				57.0 * (mlib_d64)(*p1) -                  \
				3.0 * (mlib_d64)(*p_1) -                  \
				9.0 * (mlib_d64)(*p2);                    \
			rez1 = (57.0 * a + 19.0 * a1 - 9.0 * a_1 -        \
				3.0 * a2) * 0.000244140625;               \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			pd += dst_stride;                                 \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (19.0 * a + 57.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.000244140625;               \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
		    }                                                     \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

#define	ROW_BICUBIC2_FL1(beg_i, j, chan, amount, MIN, MAX)                \
	{                                                                 \
	    PREPARE_BICUBIC(beg_i, j, chan);                              \
	    psrc_end = psrc_beg + hx + (src_width_beg - 1) * chan;        \
	    if (dh == 0) {                                                \
		for (l = 0; l < chan; l++) {                              \
		    a_1 = (mlib_d64)psrc_beg[hx + wx_1 + l];              \
		    a = (mlib_d64)psrc_beg[hx + wx + l];                  \
		    a1 = (mlib_d64)psrc_beg[hx + wx1 + l];                \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                      \
		    p = psrc_beg + hx + wx2 + l;                          \
		    p1 = psrc_beg + hx1 + wx2 + l;                        \
		    p2 = psrc_beg + hx2 + wx2 + l;                        \
		    i = (beg_i);                                          \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;     \
		    if (dw == 1) {                                        \
			a2 = (mlib_d64)(*p);                              \
			rez = (19.0 * a + 57.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.015625;                     \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
			pd += chan;                                       \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += chan;                                  \
			    p += chan;                                    \
			    p1 += chan;                                   \
			    p2 += chan;                                   \
			}                                                 \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = (mlib_d64)(*p);                              \
			rez1 = (57.0 * a + 19.0 * a1 - 9.0 * a_1 -        \
				3.0 * a2) * 0.015625;                     \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			rez = (19.0 * a + 57.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.015625;                     \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*(pd + chan) = (mlib_s32)rez;                     \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += chan;                                  \
			    p += chan;                                    \
			    p1 += chan;                                   \
			    p2 += chan;                                   \
			}                                                 \
			pd += (chan << 1);                                \
		    }                                                     \
		    if (i < (amount)) {                                   \
			a2 = (mlib_d64)(*p);                              \
			rez1 = (57.0 * a + 19.0 * a1 - 9.0 * a_1 -        \
				3.0 * a2) * 0.015625;                     \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			pd += chan;                                       \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (19.0 * a + 57.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.015625;                     \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
		    }                                                     \
		}                                                         \
	    } else {                                                      \
		for (l = 0; l < chan; l++) {                              \
		    a_1 = 5.0 * ((mlib_d64)psrc_beg[hx + wx_1 + l] +      \
			    (mlib_d64)psrc_beg[hx1 + wx_1 + l]) -         \
			    (mlib_d64)psrc_beg[hx_1 + wx_1 + l] -         \
			    (mlib_d64)psrc_beg[hx2 + wx_1 + l];           \
		    a = 5.0 * ((mlib_d64)psrc_beg[hx + wx + l] +          \
			    (mlib_d64)psrc_beg[hx1 + wx + l]) -           \
			    (mlib_d64)psrc_beg[hx_1 + wx + l] -           \
			    (mlib_d64)psrc_beg[hx2 + wx + l];             \
		    a1 = 5.0 * ((mlib_d64)psrc_beg[hx + wx1 + l] +        \
			    (mlib_d64)psrc_beg[hx1 + wx1 + l]) -          \
			    (mlib_d64)psrc_beg[hx_1 + wx1 + l] -          \
			    (mlib_d64)psrc_beg[hx2 + wx1 + l];            \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                      \
		    p = psrc_beg + hx + wx2 + l;                          \
		    p1 = psrc_beg + hx1 + wx2 + l;                        \
		    p2 = psrc_beg + hx2 + wx2 + l;                        \
		    i = (beg_i);                                          \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;     \
		    if (dw == 1) {                                        \
			a2 = 5.0 * ((mlib_d64)(*p) + (mlib_d64)(*p1)) -   \
				(mlib_d64)(*p_1) - (mlib_d64)(*p2);       \
			rez = (19.0 * a + 57.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.001953125;                  \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
			pd += chan;                                       \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += chan;                                  \
			    p += chan;                                    \
			    p1 += chan;                                   \
			    p2 += chan;                                   \
			}                                                 \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = 5.0 * ((mlib_d64)(*p) + (mlib_d64)(*p1)) -   \
				(mlib_d64)(*p_1) - (mlib_d64)(*p2);       \
			rez1 = (57.0 * a + 19.0 * a1 - 9.0 * a_1 -        \
				3.0 * a2) * 0.001953125;                  \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			rez = (19.0 * a + 57.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.001953125;                  \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*(pd + chan) = (mlib_s32)rez;                     \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += chan;                                  \
			    p += chan;                                    \
			    p1 += chan;                                   \
			    p2 += chan;                                   \
			}                                                 \
			pd += (chan << 1);                                \
		    }                                                     \
		    if (i < (amount)) {                                   \
			a2 = 5.0 * ((mlib_d64)(*p) + (mlib_d64)(*p1)) -   \
				(mlib_d64)(*p_1) - (mlib_d64)(*p2);       \
			rez1 = (57.0 * a + 19.0 * a1 - 9.0 * a_1 -        \
				3.0 * a2) * 0.001953125;                  \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			pd += chan;                                       \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (19.0 * a + 57.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.001953125;                  \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
		    }                                                     \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC2_FL1(beg_i, j, chan, amount, MIN, MAX)              \
	{                                                                  \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    psrc_end =                                                     \
		    psrc_beg + wx + (src_height_beg - 1) * src_stride;     \
	    if (dw == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (57.0 * (mlib_d64)psrc_beg[hx_1 + wx + l] +      \
			    19.0 * (mlib_d64)psrc_beg[hx_1 + wx1 + l] -    \
			    9.0 * (mlib_d64)psrc_beg[hx_1 + wx_1 + l] -    \
			    3.0 * (mlib_d64)psrc_beg[hx_1 + wx2 + l]);     \
		    a = (57.0 * (mlib_d64)psrc_beg[hx + wx + l] +          \
			    19.0 * (mlib_d64)psrc_beg[hx + wx1 + l] -      \
			    9.0 * (mlib_d64)psrc_beg[hx + wx_1 + l] -      \
			    3.0 * (mlib_d64)psrc_beg[hx + wx2 + l]);       \
		    a1 = (57.0 * (mlib_d64)psrc_beg[hx1 + wx + l] +        \
			    19.0 * (mlib_d64)psrc_beg[hx1 + wx1 + l] -     \
			    9.0 * (mlib_d64)psrc_beg[hx1 + wx_1 + l] -     \
			    3.0 * (mlib_d64)psrc_beg[hx1 + wx2 + l]);      \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 1) {                                         \
			a2 = 57.0 * (mlib_d64)(*p) +                       \
				19.0 * (mlib_d64)(*p1) -                   \
				9.0 * (mlib_d64)(*p_1) -                   \
				3.0 * (mlib_d64)(*p2);                     \
			rez = (5.0 * (a + a1) - a_1 - a2) * 0.001953125;   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += dst_stride;                                  \
			i++;                                               \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 57.0 * (mlib_d64)(*p) +                       \
				19.0 * (mlib_d64)(*p1) -                   \
				9.0 * (mlib_d64)(*p_1) -                   \
				3.0 * (mlib_d64)(*p2);                     \
			rez1 = a * 0.015625;                               \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = (5.0 * (a + a1) - a_1 - a2) * 0.001953125;   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + dst_stride) = (mlib_s32)rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = 57.0 * (mlib_d64)(*p) +                       \
				19.0 * (mlib_d64)(*p1) -                   \
				9.0 * (mlib_d64)(*p_1) -                   \
				3.0 * (mlib_d64)(*p2);                     \
			rez1 = a * 0.015625;                               \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (5.0 * (a + a1) - a_1 - a2) * 0.001953125;   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (19.0 * (mlib_d64)psrc_beg[hx_1 + wx + l] +      \
			    57.0 * (mlib_d64)psrc_beg[hx_1 + wx1 + l] -    \
			    3.0 * (mlib_d64)psrc_beg[hx_1 + wx_1 + l] -    \
			    9.0 * (mlib_d64)psrc_beg[hx_1 + wx2 + l]);     \
		    a = (19.0 * (mlib_d64)psrc_beg[hx + wx + l] +          \
			    57.0 * (mlib_d64)psrc_beg[hx + wx1 + l] -      \
			    3.0 * (mlib_d64)psrc_beg[hx + wx_1 + l] -      \
			    9.0 * (mlib_d64)psrc_beg[hx + wx2 + l]);       \
		    a1 = (19.0 * (mlib_d64)psrc_beg[hx1 + wx + l] +        \
			    57.0 * (mlib_d64)psrc_beg[hx1 + wx1 + l] -     \
			    3.0 * (mlib_d64)psrc_beg[hx1 + wx_1 + l] -     \
			    9.0 * (mlib_d64)psrc_beg[hx1 + wx2 + l]);      \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 1) {                                         \
			a2 = 19.0 * (mlib_d64)(*p) +                       \
				57.0 * (mlib_d64)(*p1) -                   \
				3.0 * (mlib_d64)(*p_1) -                   \
				9.0 * (mlib_d64)(*p2);                     \
			rez = (5.0 * (a + a1) - a_1 - a2) * 0.001953125;   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += dst_stride;                                  \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 19.0 * (mlib_d64)(*p) +                       \
				57.0 * (mlib_d64)(*p1) -                   \
				3.0 * (mlib_d64)(*p_1) -                   \
				9.0 * (mlib_d64)(*p2);                     \
			rez1 = a * 0.015625;                               \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = (5.0 * (a + a1) - a_1 - a2) * 0.001953125;   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + dst_stride) = (mlib_s32)rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += src_stride;                             \
			    p += src_stride;                               \
			    p1 += src_stride;                              \
			    p2 += src_stride;                              \
			}                                                  \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = 19.0 * (mlib_d64)(*p) +                       \
				57.0 * (mlib_d64)(*p1) -                   \
				3.0 * (mlib_d64)(*p_1) -                   \
				9.0 * (mlib_d64)(*p2);                     \
			rez1 = a * 0.015625;                               \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (5.0 * (a + a1) - a_1 - a2) * 0.001953125;   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	ROW_BICUBIC2_FL2(beg_i, j, chan, amount, MIN, MAX)                 \
	{                                                                  \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    psrc_end = psrc_beg + hx + (src_width_beg - 1) * chan;         \
	    if (dh == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (57.0 * (mlib_d64)psrc_beg[hx + wx_1 + l] +      \
			    19.0 * (mlib_d64)psrc_beg[hx1 + wx_1 + l] -    \
			    9.0 * (mlib_d64)psrc_beg[hx_1 + wx_1 + l] -    \
			    3.0 * (mlib_d64)psrc_beg[hx2 + wx_1 + l]);     \
		    a = (57.0 * (mlib_d64)psrc_beg[hx + wx + l] +          \
			    19.0 * (mlib_d64)psrc_beg[hx1 + wx + l] -      \
			    9.0 * (mlib_d64)psrc_beg[hx_1 + wx + l] -      \
			    3.0 * (mlib_d64)psrc_beg[hx2 + wx + l]);       \
		    a1 = (57.0 * (mlib_d64)psrc_beg[hx + wx1 + l] +        \
			    19.0 * (mlib_d64)psrc_beg[hx1 + wx1 + l] -     \
			    9.0 * (mlib_d64)psrc_beg[hx_1 + wx1 + l] -     \
			    3.0 * (mlib_d64)psrc_beg[hx2 + wx1 + l]);      \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                       \
		    p = psrc_beg + hx + wx2 + l;                           \
		    p1 = psrc_beg + hx1 + wx2 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 1) {                                         \
			a2 = 57.0 * (mlib_d64)(*p) +                       \
				19.0 * (mlib_d64)(*p1) -                   \
				9.0 * (mlib_d64)(*p_1) -                   \
				3.0 * (mlib_d64)(*p2);                     \
			rez = (5.0 * (a + a1) - a_1 - a2) * 0.001953125;   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += chan;                                        \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 57.0 * (mlib_d64)(*p) +                       \
				19.0 * (mlib_d64)(*p1) -                   \
				9.0 * (mlib_d64)(*p_1) -                   \
				3.0 * (mlib_d64)(*p2);                     \
			rez1 = a * 0.015625;                               \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = (5.0 * (a + a1) - a_1 - a2) * 0.001953125;   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + chan) = (mlib_s32)rez;                      \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = (57.0 * (mlib_d64)(*p) +                      \
				19.0 * (mlib_d64)(*p1) -                   \
				9.0 * (mlib_d64)(*p_1) -                   \
				3.0 * (mlib_d64)(*p2));                    \
			rez1 = a * 0.015625;                               \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (5.0 * (a + a1) - a_1 - a2) * 0.001953125;   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (19.0 * (mlib_d64)psrc_beg[hx + wx_1 + l] +      \
			    57.0 * (mlib_d64)psrc_beg[hx1 + wx_1 + l] -    \
			    3.0 * (mlib_d64)psrc_beg[hx_1 + wx_1 + l] -    \
			    9.0 * (mlib_d64)psrc_beg[hx2 + wx_1 + l]);     \
		    a = (19.0 * (mlib_d64)psrc_beg[hx + wx + l] +          \
			    57.0 * (mlib_d64)psrc_beg[hx1 + wx + l] -      \
			    3.0 * (mlib_d64)psrc_beg[hx_1 + wx + l] -      \
			    9.0 * (mlib_d64)psrc_beg[hx2 + wx + l]);       \
		    a1 = (19.0 * (mlib_d64)psrc_beg[hx + wx1 + l] +        \
			    57.0 * (mlib_d64)psrc_beg[hx1 + wx1 + l] -     \
			    3.0 * (mlib_d64)psrc_beg[hx_1 + wx1 + l] -     \
			    9.0 * (mlib_d64)psrc_beg[hx2 + wx1 + l]);      \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                       \
		    p = psrc_beg + hx + wx2 + l;                           \
		    p1 = psrc_beg + hx1 + wx2 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 1) {                                         \
			a2 = 19.0 * (mlib_d64)(*p) +                       \
				57.0 * (mlib_d64)(*p1) -                   \
				3.0 * (mlib_d64)(*p_1) -                   \
				9.0 * (mlib_d64)(*p2);                     \
			rez = (5.0 * (a + a1) - a_1 - a2) * 0.001953125;   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += chan;                                        \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = 19.0 * (mlib_d64)(*p) +                       \
				57.0 * (mlib_d64)(*p1) -                   \
				3.0 * (mlib_d64)(*p_1) -                   \
				9.0 * (mlib_d64)(*p2);                     \
			rez1 = a * 0.015625;                               \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = (5.0 * (a + a1) - a_1 - a2) * 0.001953125;   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + chan) = (mlib_s32)rez;                      \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			if (p < psrc_end) {                                \
			    p_1 += chan;                                   \
			    p += chan;                                     \
			    p1 += chan;                                    \
			    p2 += chan;                                    \
			}                                                  \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			a2 = 19.0 * (mlib_d64)(*p) +                       \
				57.0 * (mlib_d64)(*p1) -                   \
				3.0 * (mlib_d64)(*p_1) -                   \
				9.0 * (mlib_d64)(*p2);                     \
			rez1 = a * 0.015625;                               \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (5.0 * (a + a1) - a_1 - a2) * 0.001953125;   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC2_FL2(beg_i, j, chan, amount, MIN, MAX)             \
	{                                                                 \
	    PREPARE_BICUBIC(beg_i, j, chan);                              \
	    psrc_end =                                                    \
		    psrc_beg + wx + (src_height_beg - 1) * src_stride;    \
	    if (dw == 0) {                                                \
		for (l = 0; l < chan; l++) {                              \
		    a_1 = (mlib_d64)psrc_beg[hx_1 + wx + l];              \
		    a = (mlib_d64)psrc_beg[hx + wx + l];                  \
		    a1 = (mlib_d64)psrc_beg[hx1 + wx + l];                \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                      \
		    p = psrc_beg + hx2 + wx + l;                          \
		    p1 = psrc_beg + hx2 + wx1 + l;                        \
		    p2 = psrc_beg + hx2 + wx2 + l;                        \
		    i = (j);                                              \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +    \
			    l;                                            \
		    if (dh == 1) {                                        \
			a2 = (mlib_d64)(*p);                              \
			rez = (19.0 * a + 57.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.015625;                     \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
			pd += dst_stride;                                 \
			i++;                                              \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += src_stride;                            \
			    p += src_stride;                              \
			    p1 += src_stride;                             \
			    p2 += src_stride;                             \
			}                                                 \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = (mlib_d64)(*p);                              \
			rez1 = (57.0 * a + 19.0 * a1 - 9.0 * a_1 -        \
				3.0 * a2) * 0.015625;                     \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			rez = (19.0 * a + 57.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.015625;                     \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*(pd + dst_stride) = (mlib_s32)rez;               \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += src_stride;                            \
			    p += src_stride;                              \
			    p1 += src_stride;                             \
			    p2 += src_stride;                             \
			}                                                 \
			pd += (dst_stride << 1);                          \
		    }                                                     \
		    if (i < (amount)) {                                   \
			a2 = (mlib_d64)(*p);                              \
			rez1 = (57.0 * a + 19.0 * a1 - 9.0 * a_1 -        \
				3.0 * a2) * 0.015625;                     \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			pd += dst_stride;                                 \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (19.0 * a + 57.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.015625;                     \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
		    }                                                     \
		}                                                         \
	    } else {                                                      \
		for (l = 0; l < chan; l++) {                              \
		    a_1 = 5.0 * ((mlib_d64)psrc_beg[hx_1 + wx + l] +      \
			    (mlib_d64)psrc_beg[hx_1 + wx1 + l]) -         \
			    (mlib_d64)psrc_beg[hx_1 + wx_1 + l] -         \
			    (mlib_d64)psrc_beg[hx_1 + wx2 + l];           \
		    a = 5.0 * ((mlib_d64)psrc_beg[hx + wx + l] +          \
			    (mlib_d64)psrc_beg[hx + wx1 + l]) -           \
			    (mlib_d64)psrc_beg[hx + wx_1 + l] -           \
			    (mlib_d64)psrc_beg[hx + wx2 + l];             \
		    a1 = 5.0 * ((mlib_d64)psrc_beg[hx1 + wx + l] +        \
			    (mlib_d64)psrc_beg[hx1 + wx1 + l]) -          \
			    (mlib_d64)psrc_beg[hx1 + wx_1 + l] -          \
			    (mlib_d64)psrc_beg[hx1 + wx2 + l];            \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                      \
		    p = psrc_beg + hx2 + wx + l;                          \
		    p1 = psrc_beg + hx2 + wx1 + l;                        \
		    p2 = psrc_beg + hx2 + wx2 + l;                        \
		    i = (j);                                              \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +    \
			    l;                                            \
		    if (dh == 1) {                                        \
			a2 = 5.0 * ((mlib_d64)(*p) + (mlib_d64)(*p1)) -   \
				(mlib_d64)(*p_1) - (mlib_d64)(*p2);       \
			rez = (19.0 * a + 57.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.001953125;                  \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
			pd += dst_stride;                                 \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += src_stride;                            \
			    p += src_stride;                              \
			    p1 += src_stride;                             \
			    p2 += src_stride;                             \
			}                                                 \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = 5.0 * ((mlib_d64)(*p) + (mlib_d64)(*p1)) -   \
				(mlib_d64)(*p_1) - (mlib_d64)(*p2);       \
			rez1 = (57.0 * a + 19.0 * a1 - 9.0 * a_1 -        \
				3.0 * a2) * 0.001953125;                  \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			rez = (19.0 * a + 57.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.001953125;                  \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*(pd + dst_stride) = (mlib_s32)rez;               \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			if (p < psrc_end) {                               \
			    p_1 += src_stride;                            \
			    p += src_stride;                              \
			    p1 += src_stride;                             \
			    p2 += src_stride;                             \
			}                                                 \
			pd += (dst_stride << 1);                          \
		    }                                                     \
		    if (i < (amount)) {                                   \
			a2 = 5.0 * ((mlib_d64)(*p) + (mlib_d64)(*p1)) -   \
				(mlib_d64)(*p_1) - (mlib_d64)(*p2);       \
			rez1 = (57.0 * a + 19.0 * a1 - 9.0 * a_1 -        \
				3.0 * a2) * 0.001953125;                  \
			if (rez1 > MAX)                                   \
			    rez1 = MAX;                                   \
			else if (rez1 < MIN)                              \
			    rez1 = MIN;                                   \
			*pd = (mlib_s32)rez1;                             \
			pd += dst_stride;                                 \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = (19.0 * a + 57.0 * a1 - 3.0 * a_1 -         \
				9.0 * a2) * 0.001953125;                  \
			if (rez > MAX)                                    \
			    rez = MAX;                                    \
			else if (rez < MIN)                               \
			    rez = MIN;                                    \
			*pd = (mlib_s32)rez;                              \
		    }                                                     \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

#define	ROW_BICUBIC2_FL3(beg_i, j, chan, amount, MIN, MAX)                 \
	{                                                                  \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    if (dh == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (mlib_d64)psrc_beg[hx + wx_1 + l];               \
		    a = (mlib_d64)psrc_beg[hx + wx + l];                   \
		    a1 = (mlib_d64)psrc_beg[hx + wx1 + l];                 \
		    p = psrc_beg + hx + wx2 + l;                           \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 0) {                                         \
			*pd = (mlib_s32)a;                                 \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = *p;                                           \
			rez = (5.0 * (a + a1) - a_1 - a2) * 0.125;         \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			*(pd + chan) = (mlib_s32)a1;                       \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p += chan;                                         \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			if (p >= (psrc_beg + hx + l +                      \
				src_width_beg * chan))                     \
			    a2 = (mlib_d64)psrc_beg[hx + l +               \
				    (src_width_beg - 1) * chan];           \
			else                                               \
			    a2 = *p;                                       \
			rez = (5.0 * (a + a1) - a_1 - a2) * 0.125;         \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount))                                      \
			*pd = (mlib_s32)a1;                                \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (5.0 * ((mlib_d64)psrc_beg[hx + wx_1 + l] +      \
			    (mlib_d64)psrc_beg[hx1 + wx_1 + l]) -          \
			    (mlib_d64)psrc_beg[hx_1 + wx_1 + l] -          \
			    (mlib_d64)psrc_beg[hx2 + wx_1 + l]);           \
		    a = (5.0 * ((mlib_d64)psrc_beg[hx + wx + l] +          \
			    (mlib_d64)psrc_beg[hx1 + wx + l]) -            \
			    (mlib_d64)psrc_beg[hx_1 + wx + l] -            \
			    (mlib_d64)psrc_beg[hx2 + wx + l]);             \
		    a1 = (5.0 * ((mlib_d64)psrc_beg[hx + wx1 + l] +        \
			    (mlib_d64)psrc_beg[hx1 + wx1 + l]) -           \
			    (mlib_d64)psrc_beg[hx_1 + wx1 + l] -           \
			    (mlib_d64)psrc_beg[hx2 + wx1 + l]);            \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                       \
		    p = psrc_beg + hx + wx2 + l;                           \
		    p1 = psrc_beg + hx1 + wx2 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 0) {                                         \
			rez = a * 0.125;                                   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = (5.0 * ((mlib_d64)(*p) + (mlib_d64)(*p1)) -   \
				(mlib_d64)(*p_1) - (mlib_d64)(*p2));       \
			rez1 = (5.0 * (a + a1) - a_1 - a2) * 0.015625;     \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = a1 * 0.125;                                  \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + chan) = (mlib_s32)rez;                      \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1 += chan;                                       \
			p += chan;                                         \
			p1 += chan;                                        \
			p2 += chan;                                        \
			pd += (chan << 1);                                 \
		    }                                                      \
		    if (i < (amount)) {                                    \
			if (p >= (psrc_beg + hx + l +                      \
				src_width_beg * chan))                     \
			    a2 = (5.0 * ((mlib_d64)psrc_beg[hx + l +       \
				    (src_width_beg - 1) * chan] +          \
				    (mlib_d64)psrc_beg[hx1 + l +           \
				    (src_width_beg - 1) * chan]) -         \
				    (mlib_d64)psrc_beg[hx_1 + l +          \
				    (src_width_beg - 1) * chan] -          \
				    (mlib_d64)psrc_beg[hx2 + l +           \
				    (src_width_beg - 1) * chan]);          \
			else                                               \
			    a2 = (5.0 * ((mlib_d64)(*p) +                  \
				    (mlib_d64)(*p1)) -                     \
				    (mlib_d64)(*p_1) - (mlib_d64)(*p2));   \
			rez1 = (5.0 * (a + a1) - a_1 - a2) * 0.015625;     \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = a1 * 0.125;                                  \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC2_FL3(beg_i, j, chan, amount, MIN, MAX)              \
	{                                                                  \
	    PREPARE_BICUBIC(beg_i, j, chan);                               \
	    if (dw == 0) {                                                 \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (mlib_d64)psrc_beg[hx_1 + wx + l];               \
		    a = (mlib_d64)psrc_beg[hx + wx + l];                   \
		    a1 = (mlib_d64)psrc_beg[hx1 + wx + l];                 \
		    p = psrc_beg + hx2 + wx + l;                           \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 0) {                                         \
			*pd = (mlib_s32)a;                                 \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = *p;                                           \
			rez = (5.0 * (a + a1) - a_1 - a2) * 0.125;         \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			*(pd + dst_stride) = (mlib_s32)a1;                 \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p += src_stride;                                   \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			if (p >= (psrc_beg + wx + l +                      \
				src_height_beg * src_stride))              \
			    a2 = (mlib_d64)psrc_beg[wx + l +               \
				    (src_height_beg - 1) * src_stride];    \
			else                                               \
			    a2 = *p;                                       \
			rez = (5.0 * (a + a1) - a_1 - a2) * 0.125;         \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount))                                      \
			*pd = (mlib_s32)a1;                                \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (5.0 * ((mlib_d64)psrc_beg[hx_1 + wx + l] +      \
			    (mlib_d64)psrc_beg[hx_1 + wx1 + l]) -          \
			    (mlib_d64)psrc_beg[hx_1 + wx_1 + l] -          \
			    (mlib_d64)psrc_beg[hx_1 + wx2 + l]);           \
		    a = (5.0 * ((mlib_d64)psrc_beg[hx + wx + l] +          \
			    (mlib_d64)psrc_beg[hx + wx1 + l]) -            \
			    (mlib_d64)psrc_beg[hx + wx_1 + l] -            \
			    (mlib_d64)psrc_beg[hx + wx2 + l]);             \
		    a1 = (5.0 * ((mlib_d64)psrc_beg[hx1 + wx + l] +        \
			    (mlib_d64)psrc_beg[hx1 + wx1 + l]) -           \
			    (mlib_d64)psrc_beg[hx1 + wx_1 + l] -           \
			    (mlib_d64)psrc_beg[hx1 + wx2 + l]);            \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 0) {                                         \
			rez = a * 0.125;                                   \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = (5.0 * ((mlib_d64)(*p) + (mlib_d64)(*p1)) -   \
				(mlib_d64)(*p_1) - (mlib_d64)(*p2));       \
			rez1 = (5.0 * (a + a1) - a_1 - a2) * 0.015625;     \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			rez = a1 * 0.125;                                  \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*(pd + dst_stride) = (mlib_s32)rez;                \
			a_1 = a;                                           \
			a = a1;                                            \
			a1 = a2;                                           \
			p_1 += src_stride;                                 \
			p += src_stride;                                   \
			p1 += src_stride;                                  \
			p2 += src_stride;                                  \
			pd += (dst_stride << 1);                           \
		    }                                                      \
		    if (i < (amount)) {                                    \
			if (p >= (psrc_beg + wx + l +                      \
				src_height_beg * src_stride))              \
			    a2 = (5.0 * ((mlib_d64)psrc_beg[wx + l +       \
				    (src_height_beg - 1) * src_stride] +   \
				    (mlib_d64)psrc_beg[wx1 + l +           \
				    (src_height_beg -                      \
				    1) * src_stride]) -                    \
				    (mlib_d64)psrc_beg[wx_1 + l +          \
				    (src_height_beg - 1) * src_stride] -   \
				    (mlib_d64)psrc_beg[wx2 + l +           \
				    (src_height_beg - 1) * src_stride]);   \
			else                                               \
			    a2 = (5.0 * ((mlib_d64)(*p) +                  \
				    (mlib_d64)(*p1)) -                     \
				    (mlib_d64)(*p_1) - (mlib_d64)(*p2));   \
			rez1 = (5.0 * (a + a1) - a_1 - a2) * 0.015625;     \
			if (rez1 > MAX)                                    \
			    rez1 = MAX;                                    \
			else if (rez1 < MIN)                               \
			    rez1 = MIN;                                    \
			*pd = (mlib_s32)rez1;                              \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = a1 * 0.125;                                  \
			if (rez > MAX)                                     \
			    rez = MAX;                                     \
			else if (rez < MIN)                                \
			    rez = MIN;                                     \
			*pd = (mlib_s32)rez;                               \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#endif /* _NO_LONGLONG */

void
mlib_ImageZoomIn2X_S32_SE_BC2_FL0(
	MLIB_ZOOMIN2XDEFEDGE_BC(mlib_s32))
{
	const mlib_s32 *pdst_count, *p_1, *p, *p1, *p2, *psrc_end;
	mlib_s32 *pd;
	mlib_s32 ii, jj;

	pdst_count = pdst_near;
	ii = 0;
	jj = 0;

	while (((pdst - pdst_count) >= dst_stride) && (src_height_near > 0)) {
		ROW_BICUBIC2_FL0(ii, jj, channels, src_width_near, MLIB_S32_MIN,
			MLIB_S32_MAX);
		jj++;
		pdst_count += dst_stride;
		src_height_near--;
	}

	while (src_height_near > src_height) {
		ROW_BICUBIC2_FL0(ii, jj + src_height_near - 1, channels,
			src_width_near, MLIB_S32_MIN, MLIB_S32_MAX);
		src_height_near--;
	}

	if (src_height_near > 0) {
		while ((pdst - pdst_count > 0) && (src_width_near > 0)) {
			COLUMN_BICUBIC2_FL0(ii, jj, channels,
				(src_height_near + jj), MLIB_S32_MIN,
				MLIB_S32_MAX);
			src_width_near--;
			pdst_count += channels;
			ii++;
		}

		while (src_width_near > src_width) {
			COLUMN_BICUBIC2_FL0(ii + src_width_near - 1, jj,
				channels, (src_height_near + jj), MLIB_S32_MIN,
				MLIB_S32_MAX);
			src_width_near--;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIn2X_S32_SE_BC2_FL1(
	MLIB_ZOOMIN2XDEFEDGE_BC(mlib_s32))
{
	const mlib_s32 *pdst_count, *p_1, *p, *p1, *p2, *psrc_end;
	mlib_s32 *pd;
	mlib_s32 ii, jj;

	pdst_count = pdst_near;
	ii = 0;
	jj = 0;

	while (((pdst - pdst_count) >= dst_stride) && (src_height_near > 0)) {
		ROW_BICUBIC2_FL1(ii, jj, channels, src_width_near, MLIB_S32_MIN,
			MLIB_S32_MAX);
		jj++;
		pdst_count += dst_stride;
		src_height_near--;
	}

	while (src_height_near > src_height) {
		ROW_BICUBIC2_FL1(ii, jj + src_height_near - 1, channels,
			src_width_near, MLIB_S32_MIN, MLIB_S32_MAX);
		src_height_near--;
	}

	if (src_height_near > 0) {
		while ((pdst - pdst_count > 0) && (src_width_near > 0)) {
			COLUMN_BICUBIC2_FL1(ii, jj, channels,
				(src_height_near + jj), MLIB_S32_MIN,
				MLIB_S32_MAX);
			src_width_near--;
			pdst_count += channels;
			ii++;
		}

		while (src_width_near > src_width) {
			COLUMN_BICUBIC2_FL1(ii + src_width_near - 1, jj,
				channels, (src_height_near + jj), MLIB_S32_MIN,
				MLIB_S32_MAX);
			src_width_near--;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIn2X_S32_SE_BC2_FL2(
	MLIB_ZOOMIN2XDEFEDGE_BC(mlib_s32))
{
	const mlib_s32 *pdst_count, *p_1, *p, *p1, *p2, *psrc_end;
	mlib_s32 *pd;
	mlib_s32 ii, jj;

	pdst_count = pdst_near;
	ii = 0;
	jj = 0;

	while (((pdst - pdst_count) >= dst_stride) && (src_height_near > 0)) {
		ROW_BICUBIC2_FL2(ii, jj, channels, src_width_near, MLIB_S32_MIN,
			MLIB_S32_MAX);
		jj++;
		pdst_count += dst_stride;
		src_height_near--;
	}

	while (src_height_near > src_height) {
		ROW_BICUBIC2_FL2(ii, jj + src_height_near - 1, channels,
			src_width_near, MLIB_S32_MIN, MLIB_S32_MAX);
		src_height_near--;
	}

	if (src_height_near > 0) {
		while ((pdst - pdst_count > 0) && (src_width_near > 0)) {
			COLUMN_BICUBIC2_FL2(ii, jj, channels,
				(src_height_near + jj), MLIB_S32_MIN,
				MLIB_S32_MAX);
			src_width_near--;
			pdst_count += channels;
			ii++;
		}

		while (src_width_near > src_width) {
			COLUMN_BICUBIC2_FL2(ii + src_width_near - 1, jj,
				channels, (src_height_near + jj), MLIB_S32_MIN,
				MLIB_S32_MAX);
			src_width_near--;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIn2X_S32_SE_BC2_FL3(
	MLIB_ZOOMIN2XDEFEDGE_BC(mlib_s32))
{
	const mlib_s32 *pdst_count, *p_1, *p, *p1, *p2;
	mlib_s32 *pd;
	mlib_s32 ii, jj;

	pdst_count = pdst_near;
	ii = 0;
	jj = 0;

	while (((pdst - pdst_count) >= dst_stride) && (src_height_near > 0)) {
		ROW_BICUBIC2_FL3(ii, jj, channels, src_width_near, MLIB_S32_MIN,
			MLIB_S32_MAX);
		jj++;
		pdst_count += dst_stride;
		src_height_near--;
	}

	while (src_height_near > src_height) {
		ROW_BICUBIC2_FL3(ii, jj + src_height_near - 1, channels,
			src_width_near, MLIB_S32_MIN, MLIB_S32_MAX);
		src_height_near--;
	}

	if (src_height_near > 0) {
		while ((pdst - pdst_count > 0) && (src_width_near > 0)) {
			COLUMN_BICUBIC2_FL3(ii, jj, channels,
				(src_height_near + jj), MLIB_S32_MIN,
				MLIB_S32_MAX);
			src_width_near--;
			pdst_count += channels;
			ii++;
		}

		while (src_width_near > src_width) {
			COLUMN_BICUBIC2_FL3(ii + src_width_near - 1, jj,
				channels, (src_height_near + jj), MLIB_S32_MIN,
				MLIB_S32_MAX);
			src_width_near--;
		}
	}
}

/* *********************************************************** */
