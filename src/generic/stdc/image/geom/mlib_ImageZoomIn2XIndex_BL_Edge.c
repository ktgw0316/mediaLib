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

#pragma ident	"@(#)mlib_ImageZoomIn2XIndex_BL_Edge.c	9.2	07/10/09 SMI"

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

#define	MLIB_ZOOMIN2XINDEXDEFEDGE_NN(MLIB_TYPE)                      \
	MLIB_TYPE *pdst,                                             \
	mlib_s32 src_width,                                          \
	mlib_s32 src_height,                                         \
	mlib_s32 dst_stride,                                         \
	mlib_s32 src_stride,                                         \
	MLIB_TYPE *pdst_near,                                        \
	const MLIB_TYPE *psrc_near,                                  \
	mlib_s32 src_width_near,                                     \
	mlib_s32 src_height_near,                                    \
	mlib_s32 src_w_beg_near,                                     \
	mlib_s32 src_h_beg_near

/* *********************************************************** */

#define	MLIB_ZOOMIN2XINDEXDEFEDGE_BL(MLIB_TYPE)                         \
	MLIB_ZOOMIN2XINDEXDEFEDGE_NN(MLIB_TYPE),                        \
	const void *colormap

/* *********************************************************** */

#define	ROW_COLUMN_BL_FL0_3(beg, amount, shift_src, src_format)         \
	count = 0;                                                      \
	j = (beg + 1) & 1;                                              \
	if (beg == 0) {                                                 \
	    pcolor0 = (src_format *) (lut + tpsrc[0]);                  \
	    pdst_buf[3 * count] = pcolor0[1];                           \
	    pdst_buf[3 * count + 1] = pcolor0[2];                       \
	    pdst_buf[3 * count + 2] = pcolor0[3];                       \
	    count++;                                                    \
	    flag = 0;                                                   \
	} else if (j == 1) {                                            \
	    pcolor0 = (src_format *) (lut + tpsrc[0]);                  \
	    pcolor1 = (src_format *) (lut + tpsrc[-shift_src]);         \
	    pdst_buf[3 * count] = (3 * pcolor0[1] + pcolor1[1]) >> 2;   \
	    pdst_buf[3 * count + 1] =                                   \
		    (3 * pcolor0[2] + pcolor1[2]) >> 2;                 \
	    pdst_buf[3 * count + 2] =                                   \
		    (3 * pcolor0[3] + pcolor1[3]) >> 2;                 \
	    count++;                                                    \
	    flag = 1;                                                   \
	}                                                               \
	pcolor0 = (src_format *) (lut + tpsrc[0]);                      \
	tmp1_0 = pcolor0[1];                                            \
	tmp1_1 = pcolor0[2];                                            \
	tmp1_2 = pcolor0[3];                                            \
	for (j++; j < amount; j += 2) {                                 \
	    pcolor1 = (src_format *) (lut + tpsrc[shift_src]);          \
	    tmp2_0 = pcolor1[1];                                        \
	    tmp2_1 = pcolor1[2];                                        \
	    tmp2_2 = pcolor1[3];                                        \
	    pdst_buf[3 * count] = (3 * tmp1_0 + tmp2_0) >> 2;           \
	    pdst_buf[3 * count + 1] = (3 * tmp1_1 + tmp2_1) >> 2;       \
	    pdst_buf[3 * count + 2] = (3 * tmp1_2 + tmp2_2) >> 2;       \
	    pdst_buf[3 * count + 3] = (tmp1_0 + 3 * tmp2_0) >> 2;       \
	    pdst_buf[3 * count + 4] = (tmp1_1 + 3 * tmp2_1) >> 2;       \
	    pdst_buf[3 * count + 5] = (tmp1_2 + 3 * tmp2_2) >> 2;       \
	    tpsrc += shift_src;                                         \
	    count += 2;                                                 \
	    tmp1_0 = tmp2_0;                                            \
	    tmp1_1 = tmp2_1;                                            \
	    tmp1_2 = tmp2_2;                                            \
	}                                                               \
	                                                                \
	if (j == amount) {                                              \
	    if (flag) {                                                 \
		pcolor1 = (src_format *) (lut + tpsrc[shift_src]);      \
		pdst_buf[3 * count] = (3 * tmp1_0 + pcolor1[1]) >> 2;   \
		pdst_buf[3 * count + 1] =                               \
			(3 * tmp1_1 + pcolor1[2]) >> 2;                 \
		pdst_buf[3 * count + 2] =                               \
			(3 * tmp1_2 + pcolor1[3]) >> 2;                 \
	    } else {                                                    \
		pdst_buf[3 * count] = tmp1_0;                           \
		pdst_buf[3 * count + 1] = tmp1_1;                       \
		pdst_buf[3 * count + 2] = tmp1_2;                       \
	    }                                                           \
	    count++;                                                    \
	}

/* *********************************************************** */

#define	ROW_COLUMN_BL_FL0_4(beg, amount, shift_src, src_format)         \
	count = 0;                                                      \
	j = (beg + 1) & 1;                                              \
	if (beg == 0) {                                                 \
	    pcolor0 = (src_format *) (lut + tpsrc[0]);                  \
	    pdst_buf[4 * count] = pcolor0[0];                           \
	    pdst_buf[4 * count + 1] = pcolor0[1];                       \
	    pdst_buf[4 * count + 2] = pcolor0[2];                       \
	    pdst_buf[4 * count + 3] = pcolor0[3];                       \
	    count++;                                                    \
	    flag = 0;                                                   \
	} else if (j == 1) {                                            \
	    pcolor0 = (src_format *) (lut + tpsrc[0]);                  \
	    pcolor1 = (src_format *) (lut + tpsrc[-shift_src]);         \
	    pdst_buf[4 * count] = (3 * pcolor0[0] + pcolor1[0]) >> 2;   \
	    pdst_buf[4 * count + 1] =                                   \
		    (3 * pcolor0[1] + pcolor1[1]) >> 2;                 \
	    pdst_buf[4 * count + 2] =                                   \
		    (3 * pcolor0[2] + pcolor1[2]) >> 2;                 \
	    pdst_buf[4 * count + 3] =                                   \
		    (3 * pcolor0[3] + pcolor1[3]) >> 2;                 \
	    count++;                                                    \
	    flag = 1;                                                   \
	}                                                               \
	pcolor0 = (src_format *) (lut + tpsrc[0]);                      \
	tmp1_0 = pcolor0[0];                                            \
	tmp1_1 = pcolor0[1];                                            \
	tmp1_2 = pcolor0[2];                                            \
	tmp1_3 = pcolor0[3];                                            \
	for (j++; j < amount; j += 2) {                                 \
	    pcolor1 = (src_format *) (lut + tpsrc[shift_src]);          \
	    tmp2_0 = pcolor1[0];                                        \
	    tmp2_1 = pcolor1[1];                                        \
	    tmp2_2 = pcolor1[2];                                        \
	    tmp2_3 = pcolor1[3];                                        \
	    pdst_buf[4 * count] = (3 * tmp1_0 + tmp2_0) >> 2;           \
	    pdst_buf[4 * count + 1] = (3 * tmp1_1 + tmp2_1) >> 2;       \
	    pdst_buf[4 * count + 2] = (3 * tmp1_2 + tmp2_2) >> 2;       \
	    pdst_buf[4 * count + 3] = (3 * tmp1_3 + tmp2_3) >> 2;       \
	    pdst_buf[4 * count + 4] = (tmp1_0 + 3 * tmp2_0) >> 2;       \
	    pdst_buf[4 * count + 5] = (tmp1_1 + 3 * tmp2_1) >> 2;       \
	    pdst_buf[4 * count + 6] = (tmp1_2 + 3 * tmp2_2) >> 2;       \
	    pdst_buf[4 * count + 7] = (tmp1_3 + 3 * tmp2_3) >> 2;       \
	    tpsrc += shift_src;                                         \
	    count += 2;                                                 \
	    tmp1_0 = tmp2_0;                                            \
	    tmp1_1 = tmp2_1;                                            \
	    tmp1_2 = tmp2_2;                                            \
	    tmp1_3 = tmp2_3;                                            \
	}                                                               \
	                                                                \
	if (j == amount) {                                              \
	    if (flag) {                                                 \
		pcolor1 = (src_format *) (lut + tpsrc[shift_src]);      \
		pdst_buf[4 * count] = (3 * tmp1_0 + pcolor1[0]) >> 2;   \
		pdst_buf[4 * count + 1] =                               \
			(3 * tmp1_1 + pcolor1[1]) >> 2;                 \
		pdst_buf[4 * count + 2] =                               \
			(3 * tmp1_2 + pcolor1[2]) >> 2;                 \
		pdst_buf[4 * count + 3] =                               \
			(3 * tmp1_3 + pcolor1[3]) >> 2;                 \
	    } else {                                                    \
		pdst_buf[4 * count] = tmp1_0;                           \
		pdst_buf[4 * count + 1] = tmp1_1;                       \
		pdst_buf[4 * count + 2] = tmp1_2;                       \
		pdst_buf[4 * count + 3] = tmp1_3;                       \
	    }                                                           \
	    count++;                                                    \
	}

/* *********************************************************** */

#define	ROW_COLUMN_BL_FL1_3(beg, amount, shift_src, src_format)         \
	count = 0;                                                      \
	j = (beg + 1) & 1;                                              \
	if (beg == 0) {                                                 \
	    pcolor0 = (src_format *) (lut + tpsrc[0]);                  \
	    pdst_buf[3 * count] = pcolor0[1];                           \
	    pdst_buf[3 * count + 1] = pcolor0[2];                       \
	    pdst_buf[3 * count + 2] = pcolor0[3];                       \
	    count++;                                                    \
	} else if (j == 1) {                                            \
	    pcolor0 = (src_format *) (lut + tpsrc[0]);                  \
	    pcolor1 = (src_format *) (lut + tpsrc[-shift_src]);         \
	    pdst_buf[3 * count] = (pcolor0[1] + pcolor1[1]) >> 1;       \
	    pdst_buf[3 * count + 1] = (pcolor0[2] + pcolor1[2]) >> 1;   \
	    pdst_buf[3 * count + 2] = (pcolor0[3] + pcolor1[3]) >> 1;   \
	    count++;                                                    \
	}                                                               \
	pcolor0 = (src_format *) (lut + tpsrc[0]);                      \
	tmp1_0 = pcolor0[1];                                            \
	tmp1_1 = pcolor0[2];                                            \
	tmp1_2 = pcolor0[3];                                            \
	for (j++; j < amount; j += 2) {                                 \
	    pcolor1 = (src_format *) (lut + tpsrc[shift_src]);          \
	    tmp2_0 = pcolor1[1];                                        \
	    tmp2_1 = pcolor1[2];                                        \
	    tmp2_2 = pcolor1[3];                                        \
	    pdst_buf[3 * count] = tmp1_0;                               \
	    pdst_buf[3 * count + 1] = tmp1_1;                           \
	    pdst_buf[3 * count + 2] = tmp1_2;                           \
	    pdst_buf[3 * count + 3] = (tmp1_0 + tmp2_0) >> 1;           \
	    pdst_buf[3 * count + 4] = (tmp1_1 + tmp2_1) >> 1;           \
	    pdst_buf[3 * count + 5] = (tmp1_2 + tmp2_2) >> 1;           \
	    tpsrc += shift_src;                                         \
	    count += 2;                                                 \
	    tmp1_0 = tmp2_0;                                            \
	    tmp1_1 = tmp2_1;                                            \
	    tmp1_2 = tmp2_2;                                            \
	}                                                               \
	                                                                \
	if (j == amount) {                                              \
	    pdst_buf[3 * count] = tmp1_0;                               \
	    pdst_buf[3 * count + 1] = tmp1_1;                           \
	    pdst_buf[3 * count + 2] = tmp1_2;                           \
	    count++;                                                    \
	}

/* *********************************************************** */

#define	ROW_COLUMN_BL_FL1_4(beg, amount, shift_src, src_format)         \
	count = 0;                                                      \
	j = (beg + 1) & 1;                                              \
	if (beg == 0) {                                                 \
	    pcolor0 = (src_format *) (lut + tpsrc[0]);                  \
	    pdst_buf[4 * count] = pcolor0[0];                           \
	    pdst_buf[4 * count + 1] = pcolor0[1];                       \
	    pdst_buf[4 * count + 2] = pcolor0[2];                       \
	    pdst_buf[4 * count + 3] = pcolor0[3];                       \
	    count++;                                                    \
	} else if (j == 1) {                                            \
	    pcolor0 = (src_format *) (lut + tpsrc[0]);                  \
	    pcolor1 = (src_format *) (lut + tpsrc[-shift_src]);         \
	    pdst_buf[4 * count] = (pcolor0[0] + pcolor1[0]) >> 1;       \
	    pdst_buf[4 * count + 1] = (pcolor0[1] + pcolor1[1]) >> 1;   \
	    pdst_buf[4 * count + 2] = (pcolor0[2] + pcolor1[2]) >> 1;   \
	    pdst_buf[4 * count + 3] = (pcolor0[3] + pcolor1[3]) >> 1;   \
	    count++;                                                    \
	}                                                               \
	pcolor0 = (src_format *) (lut + tpsrc[0]);                      \
	tmp1_0 = pcolor0[0];                                            \
	tmp1_1 = pcolor0[1];                                            \
	tmp1_2 = pcolor0[2];                                            \
	tmp1_3 = pcolor0[3];                                            \
	for (j++; j < amount; j += 2) {                                 \
	    pcolor1 = (src_format *) (lut + tpsrc[shift_src]);          \
	    tmp2_0 = pcolor1[0];                                        \
	    tmp2_1 = pcolor1[1];                                        \
	    tmp2_2 = pcolor1[2];                                        \
	    tmp2_3 = pcolor1[3];                                        \
	    pdst_buf[4 * count] = tmp1_0;                               \
	    pdst_buf[4 * count + 1] = tmp1_1;                           \
	    pdst_buf[4 * count + 2] = tmp1_2;                           \
	    pdst_buf[4 * count + 3] = tmp1_3;                           \
	    pdst_buf[4 * count + 4] = (tmp1_0 + tmp2_0) >> 1;           \
	    pdst_buf[4 * count + 5] = (tmp1_1 + tmp2_1) >> 1;           \
	    pdst_buf[4 * count + 6] = (tmp1_2 + tmp2_2) >> 1;           \
	    pdst_buf[4 * count + 7] = (tmp1_3 + tmp2_3) >> 1;           \
	    tpsrc += shift_src;                                         \
	    count += 2;                                                 \
	    tmp1_0 = tmp2_0;                                            \
	    tmp1_1 = tmp2_1;                                            \
	    tmp1_2 = tmp2_2;                                            \
	    tmp1_3 = tmp2_3;                                            \
	}                                                               \
	                                                                \
	if (j == amount) {                                              \
	    pdst_buf[4 * count] = tmp1_0;                               \
	    pdst_buf[4 * count + 1] = tmp1_1;                           \
	    pdst_buf[4 * count + 2] = tmp1_2;                           \
	    pdst_buf[4 * count + 3] = tmp1_3;                           \
	    count++;                                                    \
	}

/* *********************************************************** */

#define	DEFINE_IDENTIFICATORS_FOR_MACRO_3()                       \
	mlib_s32 tmp1_0, tmp1_1, tmp1_2, tmp2_0, tmp2_1, tmp2_2

/* *********************************************************** */

#define	DEFINE_IDENTIFICATORS_FOR_MACRO_4()                                \
	mlib_s32 tmp1_0, tmp1_1, tmp1_2, tmp1_3, tmp2_0, tmp2_1, tmp2_2,   \
		tmp2_3

/* *********************************************************** */

#define	ROW_COLUMN_BL_BEG_0_3	ROW_COLUMN_BL_FL0_3
#define	ROW_COLUMN_BL_END_0_3	ROW_COLUMN_BL_FL0_3

#define	ROW_COLUMN_BL_BEG_1_3	ROW_COLUMN_BL_FL0_3

#define	ROW_COLUMN_BL_END_1_3	ROW_COLUMN_BL_FL1_3

#define	ROW_COLUMN_BL_BEG_2_3	ROW_COLUMN_BL_FL1_3

#define	ROW_COLUMN_BL_END_2_3	ROW_COLUMN_BL_FL0_3

#define	ROW_COLUMN_BL_BEG_3_3	ROW_COLUMN_BL_FL1_3

#define	ROW_COLUMN_BL_END_3_3	ROW_COLUMN_BL_FL1_3

#define	ROW_COLUMN_BL_BEG_0_4	ROW_COLUMN_BL_FL0_4

#define	ROW_COLUMN_BL_END_0_4	ROW_COLUMN_BL_FL0_4

#define	ROW_COLUMN_BL_BEG_1_4	ROW_COLUMN_BL_FL0_4

#define	ROW_COLUMN_BL_END_1_4	ROW_COLUMN_BL_FL1_4

#define	ROW_COLUMN_BL_BEG_2_4	ROW_COLUMN_BL_FL1_4

#define	ROW_COLUMN_BL_END_2_4	ROW_COLUMN_BL_FL0_4

#define	ROW_COLUMN_BL_BEG_3_4	ROW_COLUMN_BL_FL1_4

#define	ROW_COLUMN_BL_END_3_4	ROW_COLUMN_BL_FL1_4

/* *********************************************************** */

#define	MLIB_ZOOMIN2XINDEX_SE_BL(src_format, src_type,                   \
	dst_format, dst_type, nchannels, SHIFT, FUNC_NAME)               \
	{                                                                \
	    const dst_format *psrc_low, *psrc_right, *psrc_left;         \
	    dst_format *tpdst;                                           \
	    const dst_format *tpsrc;                                     \
	    mlib_s32 i, j, count;                                        \
	    src_format *pcolor0, *pcolor1;                               \
	                                                                 \
	    DEFINE_IDENTIFICATORS_FOR_MACRO_##nchannels();               \
	    if (src_height < 0)                                          \
		src_height = 0;                                          \
	    if (src_width < 0)                                           \
		src_width = 0;                                           \
/*                                                                       \
 * upper bound of image                                                  \
 */                                                                      \
	    while (((pdst - pdst_near) >= dst_stride) &&                 \
		    (src_height_near > 0)) {                             \
		tpdst = pdst_near;                                       \
		tpsrc = psrc_near;                                       \
		ROW_COLUMN_BL_BEG_##SHIFT##_##nchannels                  \
			(src_w_beg_near, src_width_near, 1, src_format); \
		FUNC_NAME##src_type##_##dst_type##_##nchannels           \
			(pdst_buf, tpdst, count, colormap);              \
		if ((src_h_beg_near & 1) != 0)                           \
		    psrc_near += src_stride;                             \
		src_h_beg_near = (src_h_beg_near + 1);                   \
		pdst_near += dst_stride;                                 \
		src_height_near--;                                       \
	    }                                                            \
	    psrc_left = psrc_near;                                       \
	    psrc_low =                                                   \
		    psrc_near + ((src_height_near -                      \
		    1) >> 1) * src_stride;                               \
	    while (src_height_near > src_height) {                       \
	                                                                 \
/*                                                                       \
 * lower bound of image                                                  \
 */                                                                      \
		tpdst = pdst_near + (src_height_near - 1) * dst_stride;  \
		tpsrc = psrc_low;                                        \
		ROW_COLUMN_BL_BEG_##SHIFT##_##nchannels                  \
			(src_w_beg_near, src_width_near, 1, src_format); \
		FUNC_NAME##src_type##_##dst_type##_##nchannels           \
			(pdst_buf, tpdst, count, colormap);              \
		if (((src_height_near - 1 + src_h_beg_near) & 1) == 0)   \
		    psrc_low -= src_stride;                              \
		src_height_near--;                                       \
	    }                                                            \
	    if (src_height_near > 0) {                                   \
	                                                                 \
/*                                                                       \
 * left bound of image                                                   \
 */                                                                      \
		while ((pdst - pdst_near > 0) && (src_width_near > 0)) { \
		    tpdst = pdst_near;                                   \
		    tpsrc = psrc_left;                                   \
		    ROW_COLUMN_BL_END_##SHIFT##_##nchannels              \
			    (src_h_beg_near, src_height_near,            \
			    src_stride, src_format);                     \
		    FUNC_NAME##src_type##_##dst_type##_##nchannels       \
			    (pdst_buf, pdst_buf1, count, colormap);      \
		    for (i = 0; i < count; i++)                          \
			tpdst[i * dst_stride] = pdst_buf1[i];            \
		    if (src_w_beg_near != 0)                             \
			psrc_left += 1;                                  \
		    src_width_near--;                                    \
		    pdst_near += 1;                                      \
		    src_w_beg_near = (src_w_beg_near + 1) & 1;           \
		}                                                        \
		psrc_right = psrc_left + ((src_width_near - 1) >> 1);    \
		while (src_width_near > src_width) {                     \
	                                                                 \
/*                                                                       \
 * right bound of image                                                  \
 */                                                                      \
		    tpdst = pdst_near + (src_width_near - 1);            \
		    tpsrc = psrc_right;                                  \
		    ROW_COLUMN_BL_END_##SHIFT##_##nchannels              \
			    (src_h_beg_near, src_height_near,            \
			    src_stride, src_format);                     \
		    FUNC_NAME##src_type##_##dst_type##_##nchannels       \
			    (pdst_buf, pdst_buf1, count, colormap);      \
		    for (i = 0; i < count; i++)                          \
			tpdst[i * dst_stride] = pdst_buf1[i];            \
		    if (((src_width_near - 1 + src_w_beg_near) & 1) ==   \
			    0)                                           \
			psrc_right -= 1;                                 \
		    src_width_near--;                                    \
		}                                                        \
	    }                                                            \
	}

/* *********************************************************** */

#define	ALLOC_BL_U8()                                                  \
	mlib_u8 *pdst_buf;                                             \
	mlib_s32 *lut =                                                \
		(mlib_s32 *)mlib_ImageGetLutNormalTable(colormap) -    \
	mlib_ImageGetLutOffset(colormap);                              \
	pdst_buf = (mlib_u8 *)__mlib_malloc((size + 4) * nchannels);   \
	if (pdst_buf == NULL)                                          \
	{                                                              \
	__mlib_free(pdst_buf1);                                        \
	return (MLIB_FAILURE);                                         \
	}

/* *********************************************************** */

#define	ALLOC_BL_S16()                                                   \
	mlib_s16 *pdst_buf;                                              \
	mlib_d64 *lut =                                                  \
		(mlib_d64 *)mlib_ImageGetLutNormalTable(colormap) -      \
	mlib_ImageGetLutOffset(colormap);                                \
	pdst_buf =                                                       \
		(mlib_s16 *)__mlib_malloc((size + 4) * 2 * nchannels);   \
	if (pdst_buf == NULL)                                            \
	{                                                                \
	__mlib_free(pdst_buf1);                                          \
	return (MLIB_FAILURE);                                           \
	}

/* *********************************************************** */

#define	FREE() __mlib_free(pdst_buf1); __mlib_free(pdst_buf)

/* *********************************************************** */

#define	MLIB_IMAGEZOOMIN2XINDEX_SE_BL(dst_format, dst_type, SHIFT)     \
	mlib_s32 lut_type = mlib_ImageGetLutType(colormap);            \
	mlib_s32 nchannels = mlib_ImageGetLutChannels(colormap);       \
	mlib_s32 size =                                                \
		(src_height_near >                                     \
		src_width_near ? src_height_near : src_width_near);    \
	dst_format *pdst_buf1;                                         \
	                                                               \
	if ((nchannels != 3) && (nchannels != 4))                      \
	    return (MLIB_FAILURE);                                     \
	pdst_buf1 =                                                    \
		(dst_format *) __mlib_malloc((src_height_near +        \
		4) * sizeof (dst_format) * nchannels);                 \
	if (pdst_buf1 == NULL)                                         \
	    return (MLIB_FAILURE);                                     \
	switch (lut_type) {                                            \
	    case MLIB_BYTE: {                                          \
		    ALLOC_BL_U8();                                     \
		    switch (nchannels) {                               \
			case 3:                                        \
			    MLIB_ZOOMIN2XINDEX_SE_BL(mlib_u8, U8,      \
				    dst_format, dst_type, 3, SHIFT,    \
				    mlib_ImageColorTrue2IndexLine_);   \
			    break;                                     \
			case 4:                                        \
			    MLIB_ZOOMIN2XINDEX_SE_BL(mlib_u8, U8,      \
				    dst_format, dst_type, 4, SHIFT,    \
				    mlib_ImageColorTrue2IndexLine_);   \
			    break;                                     \
		    }                                                  \
		    FREE();                                            \
		    break;                                             \
		}                                                      \
	    case MLIB_SHORT: {                                         \
		    ALLOC_BL_S16();                                    \
		    switch (nchannels) {                               \
			case 3:                                        \
			    MLIB_ZOOMIN2XINDEX_SE_BL(mlib_s16, S16,    \
				    dst_format, dst_type, 3, SHIFT,    \
				    mlib_ImageColorTrue2IndexLine_);   \
			    break;                                     \
			case 4:                                        \
			    MLIB_ZOOMIN2XINDEX_SE_BL(mlib_s16, S16,    \
				    dst_format, dst_type, 4, SHIFT,    \
				    mlib_ImageColorTrue2IndexLine_);   \
			    break;                                     \
		    }                                                  \
		    FREE();                                            \
		    break;                                             \
		}                                                      \
	}                                                              \
	                                                               \
	return MLIB_SUCCESS

/* *********************************************************** */

mlib_status
mlib_ImageZoomIn2XIndex_U8_SE_BL_FL0(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BL(mlib_u8))
{
	mlib_s32 flag = 0;
	MLIB_IMAGEZOOMIN2XINDEX_SE_BL(mlib_u8,
		U8,
		0);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomIn2XIndex_U8_SE_BL_FL1(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BL(mlib_u8))
{
	mlib_s32 flag = 0;
	MLIB_IMAGEZOOMIN2XINDEX_SE_BL(mlib_u8,
		U8,
		1);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomIn2XIndex_U8_SE_BL_FL2(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BL(mlib_u8))
{
	mlib_s32 flag = 0;
	MLIB_IMAGEZOOMIN2XINDEX_SE_BL(mlib_u8,
		U8,
		2);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomIn2XIndex_U8_SE_BL_FL3(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BL(mlib_u8))
{
	MLIB_IMAGEZOOMIN2XINDEX_SE_BL(mlib_u8,
		U8,
		3);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomIn2XIndex_S16_SE_BL_FL0(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BL(mlib_s16))
{
	mlib_s32 flag = 0;
	MLIB_IMAGEZOOMIN2XINDEX_SE_BL(mlib_s16,
		S16,
		0);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomIn2XIndex_S16_SE_BL_FL1(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BL(mlib_s16))
{
	mlib_s32 flag = 0;
	MLIB_IMAGEZOOMIN2XINDEX_SE_BL(mlib_s16,
		S16,
		1);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomIn2XIndex_S16_SE_BL_FL2(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BL(mlib_s16))
{
	mlib_s32 flag = 0;
	MLIB_IMAGEZOOMIN2XINDEX_SE_BL(mlib_s16,
		S16,
		2);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomIn2XIndex_S16_SE_BL_FL3(
	MLIB_ZOOMIN2XINDEXDEFEDGE_BL(mlib_s16))
{
	MLIB_IMAGEZOOMIN2XINDEX_SE_BL(mlib_s16,
		S16,
		3);
}

/* *********************************************************** */
