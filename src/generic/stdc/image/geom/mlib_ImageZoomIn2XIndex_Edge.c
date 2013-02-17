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

#pragma ident	"@(#)mlib_ImageZoomIn2XIndex_Edge.c	9.2	07/10/09 SMI"

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

void
mlib_ImageZoomIn2XIndex_U8_CE(
    MLIB_ZOOMIN2XINDEXDEFEDGE_CE(mlib_u8))
{
	mlib_s32 i, j;
	mlib_s32 offset = mlib_ImageGetLutOffset(colormap);

	if (src_height < 0)
		src_height = 0;

	if (src_width < 0)
		src_width = 0;

/* upper bound of image */
	while (((pdst - pdst_near) >= dst_stride) && (src_height_near > 0)) {
		for (i = 0; i < src_width_near; i++)
			pdst_near[i] = offset;
		pdst_near += dst_stride;
		src_height_near--;
	}

/* lower bound of image */
	while (src_height_near > src_height) {
		for (i = 0; i < src_width_near; i++)
			pdst_near[i + (src_height_near - 1) * dst_stride] =
			    offset;
		src_height_near--;
	}

/* left bound of image */
	while ((pdst - pdst_near > 0) && (src_width_near > 0)) {
		for (j = 0; j < src_height_near; j++)
			pdst_near[j * dst_stride] = offset;
		src_width_near--;
		pdst_near += 1;
	}

/* right bound of image */
	while (src_width_near > src_width) {
		for (j = 0; j < src_height_near; j++)
			pdst_near[(src_width_near - 1) + j * dst_stride] =
			    offset;
		src_width_near--;
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIn2XIndex_S16_CE(
    MLIB_ZOOMIN2XINDEXDEFEDGE_CE(mlib_s16))
{
	mlib_s32 i, j;
	mlib_s32 offset = mlib_ImageGetLutOffset(colormap);

	if (src_height < 0)
		src_height = 0;

	if (src_width < 0)
		src_width = 0;

/* upper bound of image */
	while (((pdst - pdst_near) >= dst_stride) && (src_height_near > 0)) {
		for (i = 0; i < src_width_near; i++)
			pdst_near[i] = offset;
		pdst_near += dst_stride;
		src_height_near--;
	}

/* lower bound of image */
	while (src_height_near > src_height) {
		for (i = 0; i < src_width_near; i++)
			pdst_near[i + (src_height_near - 1) * dst_stride] =
			    offset;
		src_height_near--;
	}

/* left bound of image */
	while ((pdst - pdst_near > 0) && (src_width_near > 0)) {
		for (j = 0; j < src_height_near; j++)
			pdst_near[j * dst_stride] = offset;
		src_width_near--;
		pdst_near += 1;
	}

/* right bound of image */
	while (src_width_near > src_width) {
		for (j = 0; j < src_height_near; j++)
			pdst_near[(src_width_near - 1) + j * dst_stride] =
			    offset;
		src_width_near--;
	}
}

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

void
mlib_ImageZoomIn2XIndex_U8_ON(
    MLIB_ZOOMIN2XINDEXDEFEDGE_NN(mlib_u8))
{
	const mlib_u8 *psrc_low, *psrc_right, *psrc_left;
	const mlib_u8 *tpsrc;
	mlib_u8 *tpdst;
	mlib_s32 j;

	if (src_height < 0)
		src_height = 0;

	if (src_width < 0)
		src_width = 0;

/* upper bound of image */
	while (((pdst - pdst_near) >= dst_stride) && (src_height_near > 0)) {
		tpdst = pdst_near;
		tpsrc = psrc_near;
		ROW_COLUMN_NN(src_w_beg_near, src_width_near, 1, 1);

		if ((src_h_beg_near & 1) != 0)
			psrc_near += src_stride;
		src_h_beg_near = (src_h_beg_near + 1);
		pdst_near += dst_stride;
		src_height_near--;
	}

	psrc_left = psrc_near;
	psrc_low =
	    psrc_near + ((src_height_near - 1 +
	    (src_h_beg_near & 1)) >> 1) * src_stride;

/* lower bound of image */
	while (src_height_near > src_height) {
		tpdst = pdst_near + (src_height_near - 1) * dst_stride;
		tpsrc = psrc_low;
		ROW_COLUMN_NN(src_w_beg_near, src_width_near, 1, 1);

		if (((src_height_near - 1 + src_h_beg_near) & 1) == 0)
			psrc_low -= src_stride;
		src_height_near--;
	}

	if (src_height_near > 0) {
/* left bound of image */
		while ((pdst - pdst_near > 0) && (src_width_near > 0)) {
			tpdst = pdst_near;
			tpsrc = psrc_left;
			ROW_COLUMN_NN(src_h_beg_near, src_height_near,
			    dst_stride, src_stride);

			if ((src_w_beg_near & 1) != 0)
				psrc_left += 1;
			src_width_near--;
			pdst_near += 1;
			src_w_beg_near = (src_w_beg_near + 1);
		}

		psrc_right =
		    psrc_left + ((src_width_near - 1 +
		    (src_w_beg_near & 1)) >> 1);

/* right bound of image */
		while (src_width_near > src_width) {
			tpdst = pdst_near + (src_width_near - 1);
			tpsrc = psrc_right;
			ROW_COLUMN_NN(src_h_beg_near, src_height_near,
			    dst_stride, src_stride);

			if (((src_width_near - 1 + src_w_beg_near) & 1) == 0)
				psrc_right -= 1;
			src_width_near--;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIn2XIndex_S16_ON(
    MLIB_ZOOMIN2XINDEXDEFEDGE_NN(mlib_s16))
{
	const mlib_s16 *psrc_low, *psrc_right, *psrc_left;
	const mlib_s16 *tpsrc;
	mlib_s16 *tpdst;
	mlib_s32 j;

	if (src_height < 0)
		src_height = 0;

	if (src_width < 0)
		src_width = 0;

/* upper bound of image */
	while (((pdst - pdst_near) >= dst_stride) && (src_height_near > 0)) {
		tpdst = pdst_near;
		tpsrc = psrc_near;
		ROW_COLUMN_NN(src_w_beg_near, src_width_near, 1, 1);

		if ((src_h_beg_near & 1) != 0)
			psrc_near += src_stride;
		src_h_beg_near = (src_h_beg_near + 1);
		pdst_near += dst_stride;
		src_height_near--;
	}

	psrc_left = psrc_near;
	psrc_low =
	    psrc_near + ((src_height_near - 1 +
	    (src_h_beg_near & 1)) >> 1) * src_stride;

/* lower bound of image */
	while (src_height_near > src_height) {
		tpdst = pdst_near + (src_height_near - 1) * dst_stride;
		tpsrc = psrc_low;
		ROW_COLUMN_NN(src_w_beg_near, src_width_near, 1, 1);

		if (((src_height_near - 1 + src_h_beg_near) & 1) == 0)
			psrc_low -= src_stride;
		src_height_near--;
	}

	if (src_height_near > 0) {
/* left bound of image */
		while ((pdst - pdst_near > 0) && (src_width_near > 0)) {
			tpdst = pdst_near;
			tpsrc = psrc_left;
			ROW_COLUMN_NN(src_h_beg_near, src_height_near,
			    dst_stride, src_stride);

			if ((src_w_beg_near & 1) != 0)
				psrc_left += 1;
			src_width_near--;
			pdst_near += 1;
			src_w_beg_near = (src_w_beg_near + 1);
		}

		psrc_right =
		    psrc_left + ((src_width_near - 1 +
		    (src_w_beg_near & 1)) >> 1);

/* right bound of image */
		while (src_width_near > src_width) {
			tpdst = pdst_near + (src_width_near - 1);
			tpsrc = psrc_right;
			ROW_COLUMN_NN(src_h_beg_near, src_height_near,
			    dst_stride, src_stride);

			if (((src_width_near - 1 + src_w_beg_near) & 1) == 0)
				psrc_right -= 1;
			src_width_near--;
		}
	}
}

/* *********************************************************** */
