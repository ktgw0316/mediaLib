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

#pragma ident	"@(#)mlib_ImageZoomIn2X_Edge_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *  mlib_ImageZoomIn2X_F32_SE_BL_FL0 - image 2X scaling,
 *                                     image type is mlib_f32,
 *                                     filter is MLIB_BILINEAR,
 *                                     edge condition is MLIB_EDGE_SRC_EXTEND,
 *                                     image width is even,
 *                                     image height is even.
 *  mlib_ImageZoomIn2X_F32_SE_BL_FL1 - image 2X scaling,
 *                                     image type is mlib_f32,
 *                                     filter is MLIB_BILINEAR,
 *                                     edge condition is MLIB_EDGE_SRC_EXTEND,
 *                                     image width is even,
 *                                     image height is odd.
 *  mlib_ImageZoomIn2X_F32_SE_BL_FL2 - image 2X scaling,
 *                                     image type is mlib_f32,
 *                                     filter is MLIB_BILINEAR,
 *                                     edge condition is MLIB_EDGE_SRC_EXTEND,
 *                                     image width is odd,
 *                                     image height is even.
 *  mlib_ImageZoomIn2X_F32_SE_BL_FL3 - image 2X scaling,
 *                                     image type is mlib_f32,
 *                                     filter is MLIB_BILINEAR,
 *                                     edge condition is MLIB_EDGE_SRC_EXTEND,
 *                                     image width is odd,
 *                                     image height is odd.
 *  mlib_ImageZoomIn2X_F32_SE_BC_FL0 - image 2X scaling,
 *                                     image type is mlib_f32,
 *                                     filter is MLIB_BICUBIC,
 *                                     edge condition is MLIB_EDGE_SRC_EXTEND,
 *                                     image width is even,
 *                                     image height is even.
 *  mlib_ImageZoomIn2X_F32_SE_BC_FL1 - image 2X scaling,
 *                                     image type is mlib_f32,
 *                                     filter is MLIB_BICUBIC,
 *                                     edge condition is MLIB_EDGE_SRC_EXTEND,
 *                                     image width is even,
 *                                     image height is odd.
 *  mlib_ImageZoomIn2X_F32_SE_BC_FL2 - image 2X scaling,
 *                                     image type is mlib_f32,
 *                                     filter is MLIB_BICUBIC,
 *                                     edge condition is MLIB_EDGE_SRC_EXTEND,
 *                                     image width is odd,
 *                                     image height is even.
 *  mlib_ImageZoomIn2X_F32_SE_BC_FL3 - image 2X scaling,
 *                                     image type is mlib_f32,
 *                                     filter is MLIB_BICUBIC,
 *                                     edge condition is MLIB_EDGE_SRC_EXTEND,
 *                                     image width is odd,
 *                                     image height is odd.
 *  mlib_ImageZoomIn2X_F32_SE_BC2_FL0 - image 2X scaling,
 *                                     image type is mlib_f32,
 *                                     filter is MLIB_BICUBIC2,
 *                                     edge condition is MLIB_EDGE_SRC_EXTEND,
 *                                     image width is even,
 *                                     image height is even.
 *  mlib_ImageZoomIn2X_F32_SE_BC2_FL1 - image 2X scaling,
 *                                     image type is mlib_f32,
 *                                     filter is MLIB_BICUBIC2,
 *                                     edge condition is MLIB_EDGE_SRC_EXTEND,
 *                                     image width is even,
 *                                     image height is odd.
 *  mlib_ImageZoomIn2X_F32_SE_BC2_FL2 - image 2X scaling,
 *                                     image type is mlib_f32,
 *                                     filter is MLIB_BICUBIC2,
 *                                     edge condition is MLIB_EDGE_SRC_EXTEND,
 *                                     image width is odd,
 *                                     image height is even.
 *  mlib_ImageZoomIn2X_F32_SE_BC2_FL3 - image 2X scaling,
 *                                     image type is mlib_f32,
 *                                     filter is MLIB_BICUBIC2,
 *                                     edge condition is MLIB_EDGE_SRC_EXTEND,
 *                                     image width is odd,
 *                                     image height is odd.
 *  mlib_ImageZoomIn2X_D64_SE_BL_FL0 - image 2X scaling,
 *                                     image type is mlib_d64,
 *                                     filter is MLIB_BILINEAR,
 *                                     edge condition is MLIB_EDGE_SRC_EXTEND,
 *                                     image width is even,
 *                                     image height is even.
 *  mlib_ImageZoomIn2X_D64_SE_BL_FL1 - image 2X scaling,
 *                                     image type is mlib_d64,
 *                                     filter is MLIB_BILINEAR,
 *                                     edge condition is MLIB_EDGE_SRC_EXTEND,
 *                                     image width is even,
 *                                     image height is odd.
 *  mlib_ImageZoomIn2X_D64_SE_BL_FL2 - image 2X scaling,
 *                                     image type is mlib_d64,
 *                                     filter is MLIB_BILINEAR,
 *                                     edge condition is MLIB_EDGE_SRC_EXTEND,
 *                                     image width is odd,
 *                                     image height is even.
 *  mlib_ImageZoomIn2X_D64_SE_BL_FL3 - image 2X scaling,
 *                                     image type is mlib_d64,
 *                                     filter is MLIB_BILINEAR,
 *                                     edge condition is MLIB_EDGE_SRC_EXTEND,
 *                                     image width is odd,
 *                                     image height is odd.
 *  mlib_ImageZoomIn2X_D64_SE_BC_FL0 - image 2X scaling,
 *                                     image type is mlib_d64,
 *                                     filter is MLIB_BICUBIC,
 *                                     edge condition is MLIB_EDGE_SRC_EXTEND,
 *                                     image width is even,
 *                                     image height is even.
 *  mlib_ImageZoomIn2X_D64_SE_BC_FL1 - image 2X scaling,
 *                                     image type is mlib_d64,
 *                                     filter is MLIB_BICUBIC,
 *                                     edge condition is MLIB_EDGE_SRC_EXTEND,
 *                                     image width is even,
 *                                     image height is odd.
 *  mlib_ImageZoomIn2X_D64_SE_BC_FL2 - image 2X scaling,
 *                                     image type is mlib_d64,
 *                                     filter is MLIB_BICUBIC,
 *                                     edge condition is MLIB_EDGE_SRC_EXTEND,
 *                                     image width is odd,
 *                                     image height is even.
 *  mlib_ImageZoomIn2X_D64_SE_BC_FL3 - image 2X scaling,
 *                                     image type is mlib_d64,
 *                                     filter is MLIB_BICUBIC,
 *                                     edge condition is MLIB_EDGE_SRC_EXTEND,
 *                                     image width is odd,
 *                                     image height is odd.
 *  mlib_ImageZoomIn2X_D64_SE_BC2_FL0 - image 2X scaling,
 *                                     image type is mlib_d64,
 *                                     filter is MLIB_BICUBIC2,
 *                                     edge condition is MLIB_EDGE_SRC_EXTEND,
 *                                     image width is even,
 *                                     image height is even.
 *  mlib_ImageZoomIn2X_D64_SE_BC2_FL1 - image 2X scaling,
 *                                     image type is mlib_d64,
 *                                     filter is MLIB_BICUBIC2,
 *                                     edge condition is MLIB_EDGE_SRC_EXTEND,
 *                                     image width is even,
 *                                     image height is odd.
 *  mlib_ImageZoomIn2X_D64_SE_BC2_FL2 - image 2X scaling,
 *                                     image type is mlib_d64,
 *                                     filter is MLIB_BICUBIC2,
 *                                     edge condition is MLIB_EDGE_SRC_EXTEND,
 *                                     image width is odd,
 *                                     image height is even.
 *  mlib_ImageZoomIn2X_D64_SE_BC2_FL3 - image 2X scaling,
 *                                     image type is mlib_d64,
 *                                     filter is MLIB_BICUBIC2,
 *                                     edge condition is MLIB_EDGE_SRC_EXTEND,
 *                                     image width is odd,
 *                                     image height is odd.
 */

#include <mlib_image.h>
#include <mlib_ImageZoomIn2X.h>

/* *********************************************************** */

#define	MLIB_ZOOMIN2XDEFPARAM(MLIB_TYPE)                              \
	MLIB_TYPE  *pdst,                                             \
	const MLIB_TYPE  *psrc,                                       \
	mlib_s32 src_width,                                           \
	mlib_s32 src_height,                                          \
	mlib_s32 dst_stride,                                          \
	mlib_s32 src_stride,                                          \
	mlib_s32 src_w_beg,                                           \
	mlib_s32 src_h_beg,                                           \
	mlib_s32 channels

/* *********************************************************** */

#define	MLIB_ZOOMIN2XDEFEDGE(MLIB_TYPE)                               \
	MLIB_ZOOMIN2XDEFPARAM(MLIB_TYPE),                             \
	MLIB_TYPE *pdst_near,                                         \
	const MLIB_TYPE *psrc_near,                                   \
	mlib_s32 src_width_near,                                      \
	mlib_s32 src_height_near,                                     \
	mlib_s32 src_w_beg_near,                                      \
	mlib_s32 src_h_beg_near                                       \

/* *********************************************************** */

#define	MLIB_ZOOMIN2XDEFEDGE_BC(MLIB_TYPE)                            \
	MLIB_ZOOMIN2XDEFEDGE(MLIB_TYPE),                              \
	const MLIB_TYPE *psrc_beg,                                    \
	mlib_s32 src_width_beg,                                       \
	mlib_s32 src_height_beg                                       \

/* *********************************************************** */

#define	ROW_COLUMN_BL_FL0(beg, amount, shift_dst, shift_src)               \
	j = (beg + 1) & 1;                                                 \
	if (beg == 0) {                                                    \
	    *tpdst = *tpsrc;                                               \
	    tpdst += shift_dst;                                            \
	    flag = 0;                                                      \
	} else if (j == 1) {                                               \
	    *tpdst = (3.0f * (*tpsrc) + (*(tpsrc - shift_src))) * 0.25f;   \
	    tpdst += shift_dst;                                            \
	    flag = 1;                                                      \
	}                                                                  \
	tmp1 = *tpsrc;                                                     \
	for (j++; j < amount; j += 2) {                                    \
	    tmp2 = *(tpsrc + shift_src);                                   \
	    *tpdst = (3.0f * tmp1 + tmp2) * 0.25f;                         \
	    *(tpdst + shift_dst) = (tmp1 + 3.0f * tmp2) * 0.25f;           \
	    tpsrc += shift_src;                                            \
	    tpdst += (shift_dst << 1);                                     \
	    tmp1 = tmp2;                                                   \
	}                                                                  \
	                                                                   \
	if (j == amount)                                                   \
	    if (flag) {                                                    \
		*tpdst = (3.0f * tmp1 + *(tpsrc + shift_src)) * 0.25f;     \
	    } else {                                                       \
		*tpdst = tmp1;                                             \
	    }

/* *********************************************************** */

#define	ROW_COLUMN_BL_FL1(beg, amount, shift_dst, shift_src)       \
	j = (beg + 1) & 1;                                         \
	if (beg == 0) {                                            \
	    *tpdst = *tpsrc;                                       \
	    tpdst += shift_dst;                                    \
	} else if (j == 1) {                                       \
	    *tpdst = ((*tpsrc) + (*(tpsrc - shift_src))) * 0.5f;   \
	    tpdst += shift_dst;                                    \
	}                                                          \
	tmp1 = *tpsrc;                                             \
	for (j++; j < amount; j += 2) {                            \
	    tmp2 = *(tpsrc + shift_src);                           \
	    *tpdst = tmp1;                                         \
	    *(tpdst + shift_dst) = (tmp1 + tmp2) * 0.5f;           \
	    tpsrc += shift_src;                                    \
	    tpdst += (shift_dst << 1);                             \
	    tmp1 = tmp2;                                           \
	}                                                          \
	                                                           \
	if (j == amount) {                                         \
	    *tpdst = tmp1;                                         \
	}

/* *********************************************************** */

void
mlib_ImageZoomIn2X_F32_SE_BL_FL0(
	MLIB_ZOOMIN2XDEFEDGE(mlib_f32))
{
	const mlib_f32 *psrc_low, *psrc_right, *psrc_left, *tpsrc;
	mlib_f32 *tpdst;
	mlib_f32 tmp1, tmp2;
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
mlib_ImageZoomIn2X_F32_SE_BL_FL1(
	MLIB_ZOOMIN2XDEFEDGE(mlib_f32))
{
	const mlib_f32 *psrc_low, *psrc_right, *psrc_left, *tpsrc;
	mlib_f32 *tpdst;
	mlib_f32 tmp1, tmp2;
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
mlib_ImageZoomIn2X_F32_SE_BL_FL2(
	MLIB_ZOOMIN2XDEFEDGE(mlib_f32))
{
	const mlib_f32 *psrc_low, *psrc_right, *psrc_left, *tpsrc;
	mlib_f32 *tpdst;
	mlib_f32 tmp1, tmp2;
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
mlib_ImageZoomIn2X_F32_SE_BL_FL3(
	MLIB_ZOOMIN2XDEFEDGE(mlib_f32))
{
	const mlib_f32 *psrc_low, *psrc_right, *psrc_left, *tpsrc;
	mlib_f32 *tpdst;
	mlib_f32 tmp1, tmp2;
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

		if (((src_height_near - 1 + src_h_beg_near) & 1) == 0) {
			psrc_low -= src_stride;
		}

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

			if (((src_width_near - 1 + src_w_beg_near) & 1) == 0) {
				psrc_right -= channels;
			}

			src_width_near--;
		}
	}
}

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
	mlib_f32 a_1, a, a1, a2 = 0, rez, rez1;                         \
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

#define	ROW_BICUBIC_FL0(beg_i, j, chan, amount)                          \
	{                                                                \
	    PREPARE_BICUBIC(beg_i, j, chan);                             \
	    psrc_end = psrc_beg + hx + (src_width_beg - 1) * chan;       \
	    if (dh == 0) {                                               \
		for (l = 0; l < chan; l++) {                             \
		    a_1 = (111.0f * psrc_beg[hx + wx_1 + l] +            \
			    29.0f * psrc_beg[hx1 + wx_1 + l] -           \
			    9.0f * psrc_beg[hx_1 + wx_1 + l] -           \
			    3.0f * psrc_beg[hx2 + wx_1 + l]);            \
		    a = (111.0f * psrc_beg[hx + wx + l] +                \
			    29.0f * psrc_beg[hx1 + wx + l] -             \
			    9.0f * psrc_beg[hx_1 + wx + l] -             \
			    3.0f * psrc_beg[hx2 + wx + l]);              \
		    a1 = (111.0f * psrc_beg[hx + wx1 + l] +              \
			    29.0f * psrc_beg[hx1 + wx1 + l] -            \
			    9.0f * psrc_beg[hx_1 + wx1 + l] -            \
			    3.0f * psrc_beg[hx2 + wx1 + l]);             \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                     \
		    p = psrc_beg + hx + wx2 + l;                         \
		    p1 = psrc_beg + hx1 + wx2 + l;                       \
		    p2 = psrc_beg + hx2 + wx2 + l;                       \
		    i = (beg_i);                                         \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;    \
		    if (dw == 1) {                                       \
			a2 = 111.0f * (*p) + 29.0f * (*p1) -             \
				9.0f * (*p_1) - 3.0f * (*p2);            \
			rez = (29.0f * a + 111.0f * a1 - 3.0f * a_1 -    \
				9.0f * a2) * 0.00006103515625f;          \
			*pd = rez;                                       \
			pd += chan;                                      \
			a_1 = a;                                         \
			a = a1;                                          \
			a1 = a2;                                         \
			if (p < psrc_end) {                              \
			    p_1 += chan;                                 \
			    p += chan;                                   \
			    p1 += chan;                                  \
			    p2 += chan;                                  \
			}                                                \
			i++;                                             \
		    }                                                    \
		    for (; i < (amount) - 2; i += 2) {                   \
			a2 = 111.0f * (*p) + 29.0f * (*p1) -             \
				9.0f * (*p_1) - 3.0f * (*p2);            \
			rez1 = (111.0f * a + 29.0f * a1 - 9.0f * a_1 -   \
				3.0f * a2) * 0.00006103515625f;          \
			*pd = rez1;                                      \
			rez = (29.0f * a + 111.0f * a1 - 3.0f * a_1 -    \
				9.0f * a2) * 0.00006103515625f;          \
			*(pd + chan) = rez;                              \
			a_1 = a;                                         \
			a = a1;                                          \
			a1 = a2;                                         \
			if (p < psrc_end) {                              \
			    p_1 += chan;                                 \
			    p += chan;                                   \
			    p1 += chan;                                  \
			    p2 += chan;                                  \
			}                                                \
			pd += (chan << 1);                               \
		    }                                                    \
		    if (i < (amount)) {                                  \
			a2 = (111.0f * (*p) + 29.0f * (*p1) -            \
				9.0f * (*p_1) - 3.0f * (*p2));           \
			rez1 = (111.0f * a + 29.0f * a1 - 9.0f * a_1 -   \
				3.0f * a2) * 0.00006103515625f;          \
			*pd = rez1;                                      \
			pd += chan;                                      \
			i++;                                             \
		    }                                                    \
		    if (i < (amount)) {                                  \
			rez = (29.0f * a + 111.0f * a1 - 3.0f * a_1 -    \
				9.0f * a2) * 0.00006103515625f;          \
			*pd = rez;                                       \
		    }                                                    \
		}                                                        \
	    } else {                                                     \
		for (l = 0; l < chan; l++) {                             \
		    a_1 = (29.0f * psrc_beg[hx + wx_1 + l] +             \
			    111.0f * psrc_beg[hx1 + wx_1 + l] -          \
			    3.0f * psrc_beg[hx_1 + wx_1 + l] -           \
			    9.0f * psrc_beg[hx2 + wx_1 + l]);            \
		    a = (29.0f * psrc_beg[hx + wx + l] +                 \
			    111.0f * psrc_beg[hx1 + wx + l] -            \
			    3.0f * psrc_beg[hx_1 + wx + l] -             \
			    9.0f * psrc_beg[hx2 + wx + l]);              \
		    a1 = (29.0f * psrc_beg[hx + wx1 + l] +               \
			    111.0f * psrc_beg[hx1 + wx1 + l] -           \
			    3.0f * psrc_beg[hx_1 + wx1 + l] -            \
			    9.0f * psrc_beg[hx2 + wx1 + l]);             \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                     \
		    p = psrc_beg + hx + wx2 + l;                         \
		    p1 = psrc_beg + hx1 + wx2 + l;                       \
		    p2 = psrc_beg + hx2 + wx2 + l;                       \
		    i = (beg_i);                                         \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;    \
		    if (dw == 1) {                                       \
			a2 = 29.0f * (*p) + 111.0f * (*p1) -             \
				3.0f * (*p_1) - 9.0f * (*p2);            \
			rez = (29.0f * a + 111.0f * a1 - 3.0f * a_1 -    \
				9.0f * a2) * 0.00006103515625f;          \
			*pd = rez;                                       \
			pd += chan;                                      \
			a_1 = a;                                         \
			a = a1;                                          \
			a1 = a2;                                         \
			if (p < psrc_end) {                              \
			    p_1 += chan;                                 \
			    p += chan;                                   \
			    p1 += chan;                                  \
			    p2 += chan;                                  \
			}                                                \
			i++;                                             \
		    }                                                    \
		    for (; i < (amount) - 2; i += 2) {                   \
			a2 = 29.0f * (*p) + 111.0f * (*p1) -             \
				3.0f * (*p_1) - 9.0f * (*p2);            \
			rez1 = (111.0f * a + 29.0f * a1 - 9.0f * a_1 -   \
				3.0f * a2) * 0.00006103515625f;          \
			*pd = rez1;                                      \
			rez = (29.0f * a + 111.0f * a1 - 3.0f * a_1 -    \
				9.0f * a2) * 0.00006103515625f;          \
			*(pd + chan) = rez;                              \
			a_1 = a;                                         \
			a = a1;                                          \
			a1 = a2;                                         \
			if (p < psrc_end) {                              \
			    p_1 += chan;                                 \
			    p += chan;                                   \
			    p1 += chan;                                  \
			    p2 += chan;                                  \
			}                                                \
			pd += (chan << 1);                               \
		    }                                                    \
		    if (i < (amount)) {                                  \
			a2 = 29.0f * (*p) + 111.0f * (*p1) -             \
				3.0f * (*p_1) - 9.0f * (*p2);            \
			rez1 = (111.0f * a + 29.0f * a1 - 9.0f * a_1 -   \
				3.0f * a2) * 0.00006103515625f;          \
			*pd = rez1;                                      \
			pd += chan;                                      \
			i++;                                             \
		    }                                                    \
		    if (i < (amount)) {                                  \
			rez = (29.0f * a + 111.0f * a1 - 3.0f * a_1 -    \
				9.0f * a2) * 0.00006103515625f;          \
			*pd = rez;                                       \
		    }                                                    \
		}                                                        \
	    }                                                            \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC_FL0(beg_i, j, chan, amount)                       \
	{                                                                \
	    PREPARE_BICUBIC(beg_i, j, chan);                             \
	    psrc_end =                                                   \
		    psrc_beg + wx + (src_height_beg - 1) * src_stride;   \
	    if (dw == 0) {                                               \
		for (l = 0; l < chan; l++) {                             \
		    a_1 = (111.0f * psrc_beg[hx_1 + wx + l] +            \
			    29.0f * psrc_beg[hx_1 + wx1 + l] -           \
			    9.0f * psrc_beg[hx_1 + wx_1 + l] -           \
			    3.0f * psrc_beg[hx_1 + wx2 + l]);            \
		    a = (111.0f * psrc_beg[hx + wx + l] +                \
			    29.0f * psrc_beg[hx + wx1 + l] -             \
			    9.0f * psrc_beg[hx + wx_1 + l] -             \
			    3.0f * psrc_beg[hx + wx2 + l]);              \
		    a1 = (111.0f * psrc_beg[hx1 + wx + l] +              \
			    29.0f * psrc_beg[hx1 + wx1 + l] -            \
			    9.0f * psrc_beg[hx1 + wx_1 + l] -            \
			    3.0f * psrc_beg[hx1 + wx2 + l]);             \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                     \
		    p = psrc_beg + hx2 + wx + l;                         \
		    p1 = psrc_beg + hx2 + wx1 + l;                       \
		    p2 = psrc_beg + hx2 + wx2 + l;                       \
		    i = (j);                                             \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +   \
			    l;                                           \
		    if (dh == 1) {                                       \
			a2 = 111.0f * (*p) + 29.0f * (*p1) -             \
				9.0f * (*p_1) - 3.0f * (*p2);            \
			rez = (29.0f * a + 111.0f * a1 - 3.0f * a_1 -    \
				9.0f * a2) * 0.00006103515625f;          \
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
			a2 = 111.0f * (*p) + 29.0f * (*p1) -             \
				9.0f * (*p_1) - 3.0f * (*p2);            \
			rez1 = (111.0f * a + 29.0f * a1 - 9.0f * a_1 -   \
				3.0f * a2) * 0.00006103515625f;          \
			*pd = rez1;                                      \
			rez = (29.0f * a + 111.0f * a1 - 3.0f * a_1 -    \
				9.0f * a2) * 0.00006103515625f;          \
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
			a2 = 111.0f * (*p) + 29.0f * (*p1) -             \
				9.0f * (*p_1) - 3.0f * (*p2);            \
			rez1 = (111.0f * a + 29.0f * a1 - 9.0f * a_1 -   \
				3.0f * a2) * 0.00006103515625f;          \
			*pd = rez1;                                      \
			pd += dst_stride;                                \
			i++;                                             \
		    }                                                    \
		    if (i < (amount)) {                                  \
			rez = (29.0f * a + 111.0f * a1 - 3.0f * a_1 -    \
				9.0f * a2) * 0.00006103515625f;          \
			*pd = rez;                                       \
		    }                                                    \
		}                                                        \
	    } else {                                                     \
		for (l = 0; l < chan; l++) {                             \
		    a_1 = (29.0f * psrc_beg[hx_1 + wx + l] +             \
			    111.0f * psrc_beg[hx_1 + wx1 + l] -          \
			    3.0f * psrc_beg[hx_1 + wx_1 + l] -           \
			    9.0f * psrc_beg[hx_1 + wx2 + l]);            \
		    a = (29.0f * psrc_beg[hx + wx + l] +                 \
			    111.0f * psrc_beg[hx + wx1 + l] -            \
			    3.0f * psrc_beg[hx + wx_1 + l] -             \
			    9.0f * psrc_beg[hx + wx2 + l]);              \
		    a1 = (29.0f * psrc_beg[hx1 + wx + l] +               \
			    111.0f * psrc_beg[hx1 + wx1 + l] -           \
			    3.0f * psrc_beg[hx1 + wx_1 + l] -            \
			    9.0f * psrc_beg[hx1 + wx2 + l]);             \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                     \
		    p = psrc_beg + hx2 + wx + l;                         \
		    p1 = psrc_beg + hx2 + wx1 + l;                       \
		    p2 = psrc_beg + hx2 + wx2 + l;                       \
		    i = (j);                                             \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +   \
			    l;                                           \
		    if (dh == 1) {                                       \
			a2 = 29.0f * (*p) + 111.0f * (*p1) -             \
				3.0f * (*p_1) - 9.0f * (*p2);            \
			rez = (29.0f * a + 111.0f * a1 - 3.0f * a_1 -    \
				9.0f * a2) * 0.00006103515625f;          \
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
			a2 = 29.0f * (*p) + 111.0f * (*p1) -             \
				3.0f * (*p_1) - 9.0f * (*p2);            \
			rez1 = (111.0f * a + 29.0f * a1 - 9.0f * a_1 -   \
				3.0f * a2) * 0.00006103515625f;          \
			*pd = rez1;                                      \
			rez = (29.0f * a + 111.0f * a1 - 3.0f * a_1 -    \
				9.0f * a2) * 0.00006103515625f;          \
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
			a2 = 29.0f * (*p) + 111.0f * (*p1) -             \
				3.0f * (*p_1) - 9.0f * (*p2);            \
			rez1 = (111.0f * a + 29.0f * a1 - 9.0f * a_1 -   \
				3.0f * a2) * 0.00006103515625f;          \
			*pd = rez1;                                      \
			pd += dst_stride;                                \
			i++;                                             \
		    }                                                    \
		    if (i < (amount)) {                                  \
			rez = (29.0f * a + 111.0f * a1 - 3.0f * a_1 -    \
				9.0f * a2) * 0.00006103515625f;          \
			*pd = rez;                                       \
		    }                                                    \
		}                                                        \
	    }                                                            \
	}

/* *********************************************************** */

#define	ROW_BICUBIC_FL1(beg_i, j, chan, amount)                            \
	{                                                                  \
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
			rez = (29.0f * a + 111.0f * a1 - 3.0f * a_1 -      \
				9.0f * a2) * 0.0078125f;                   \
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
			rez1 = (111.0f * a + 29.0f * a1 - 9.0f * a_1 -     \
				3.0f * a2) * 0.0078125f;                   \
			*pd = rez1;                                        \
			rez = (29.0f * a + 111.0f * a1 - 3.0f * a_1 -      \
				9.0f * a2) * 0.0078125f;                   \
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
			rez1 = (111.0f * a + 29.0f * a1 - 9.0f * a_1 -     \
				3.0f * a2) * 0.0078125f;                   \
			*pd = rez1;                                        \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (29.0f * a + 111.0f * a1 - 3.0f * a_1 -      \
				9.0f * a2) * 0.0078125f;                   \
			*pd = rez;                                         \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = 9.0f * (psrc_beg[hx + wx_1 + l] +                \
			    psrc_beg[hx1 + wx_1 + l]) - psrc_beg[hx_1 +    \
			    wx_1 + l] - psrc_beg[hx2 + wx_1 + l];          \
		    a = 9.0f * (psrc_beg[hx + wx + l] + psrc_beg[hx1 +     \
			    wx + l]) - psrc_beg[hx_1 + wx + l] -           \
			    psrc_beg[hx2 + wx + l];                        \
		    a1 = 9.0f * (psrc_beg[hx + wx1 + l] + psrc_beg[hx1 +   \
			    wx1 + l]) - psrc_beg[hx_1 + wx1 + l] -         \
			    psrc_beg[hx2 + wx1 + l];                       \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                       \
		    p = psrc_beg + hx + wx2 + l;                           \
		    p1 = psrc_beg + hx1 + wx2 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 1) {                                         \
			a2 = 9.0f * ((*p) + (*p1)) - (*p_1) - (*p2);       \
			rez = (29.0f * a + 111.0f * a1 - 3.0f * a_1 -      \
				9.0f * a2) * 0.00048828125f;               \
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
			a2 = 9.0f * ((*p) + (*p1)) - (*p_1) - (*p2);       \
			rez1 = (111.0f * a + 29.0f * a1 - 9.0f * a_1 -     \
				3.0f * a2) * 0.00048828125f;               \
			*pd = rez1;                                        \
			rez = (29.0f * a + 111.0f * a1 - 3.0f * a_1 -      \
				9.0f * a2) * 0.00048828125f;               \
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
			a2 = 9.0f * ((*p) + (*p1)) - (*p_1) - (*p2);       \
			rez1 = (111.0f * a + 29.0f * a1 - 9.0f * a_1 -     \
				3.0f * a2) * 0.00048828125f;               \
			*pd = rez1;                                        \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (29.0f * a + 111.0f * a1 - 3.0f * a_1 -      \
				9.0f * a2) * 0.00048828125f;               \
			*pd = rez;                                         \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC_FL1(beg_i, j, chan, amount)                       \
	{                                                                \
	    PREPARE_BICUBIC(beg_i, j, chan);                             \
	    psrc_end =                                                   \
		    psrc_beg + wx + (src_height_beg - 1) * src_stride;   \
	    if (dw == 0) {                                               \
		for (l = 0; l < chan; l++) {                             \
		    a_1 = (111.0f * psrc_beg[hx_1 + wx + l] +            \
			    29.0f * psrc_beg[hx_1 + wx1 + l] -           \
			    9.0f * psrc_beg[hx_1 + wx_1 + l] -           \
			    3.0f * psrc_beg[hx_1 + wx2 + l]);            \
		    a = (111.0f * psrc_beg[hx + wx + l] +                \
			    29.0f * psrc_beg[hx + wx1 + l] -             \
			    9.0f * psrc_beg[hx + wx_1 + l] -             \
			    3.0f * psrc_beg[hx + wx2 + l]);              \
		    a1 = (111.0f * psrc_beg[hx1 + wx + l] +              \
			    29.0f * psrc_beg[hx1 + wx1 + l] -            \
			    9.0f * psrc_beg[hx1 + wx_1 + l] -            \
			    3.0f * psrc_beg[hx1 + wx2 + l]);             \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                     \
		    p = psrc_beg + hx2 + wx + l;                         \
		    p1 = psrc_beg + hx2 + wx1 + l;                       \
		    p2 = psrc_beg + hx2 + wx2 + l;                       \
		    i = (j);                                             \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +   \
			    l;                                           \
		    if (dh == 1) {                                       \
			a2 = 111.0f * (*p) + 29.0f * (*p1) -             \
				9.0f * (*p_1) - 3.0f * (*p2);            \
			rez = (9.0f * (a + a1) - a_1 -                   \
				a2) * 0.00048828125f;                    \
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
			a2 = 111.0f * (*p) + 29.0f * (*p1) -             \
				9.0f * (*p_1) - 3.0f * (*p2);            \
			rez1 = a * 0.0078125f;                           \
			*pd = rez1;                                      \
			rez = (9.0f * (a + a1) - a_1 -                   \
				a2) * 0.00048828125f;                    \
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
			a2 = 111.0f * (*p) + 29.0f * (*p1) -             \
				9.0f * (*p_1) - 3.0f * (*p2);            \
			rez1 = a * 0.0078125f;                           \
			*pd = rez1;                                      \
			pd += dst_stride;                                \
			i++;                                             \
		    }                                                    \
		    if (i < (amount)) {                                  \
			rez = (9.0f * (a + a1) - a_1 -                   \
				a2) * 0.00048828125f;                    \
			*pd = rez;                                       \
		    }                                                    \
		}                                                        \
	    } else {                                                     \
		for (l = 0; l < chan; l++) {                             \
		    a_1 = (29.0f * psrc_beg[hx_1 + wx + l] +             \
			    111.0f * psrc_beg[hx_1 + wx1 + l] -          \
			    3.0f * psrc_beg[hx_1 + wx_1 + l] -           \
			    9.0f * psrc_beg[hx_1 + wx2 + l]);            \
		    a = (29.0f * psrc_beg[hx + wx + l] +                 \
			    111.0f * psrc_beg[hx + wx1 + l] -            \
			    3.0f * psrc_beg[hx + wx_1 + l] -             \
			    9.0f * psrc_beg[hx + wx2 + l]);              \
		    a1 = (29.0f * psrc_beg[hx1 + wx + l] +               \
			    111.0f * psrc_beg[hx1 + wx1 + l] -           \
			    3.0f * psrc_beg[hx1 + wx_1 + l] -            \
			    9.0f * psrc_beg[hx1 + wx2 + l]);             \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                     \
		    p = psrc_beg + hx2 + wx + l;                         \
		    p1 = psrc_beg + hx2 + wx1 + l;                       \
		    p2 = psrc_beg + hx2 + wx2 + l;                       \
		    i = (j);                                             \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +   \
			    l;                                           \
		    if (dh == 1) {                                       \
			a2 = 29.0f * (*p) + 111.0f * (*p1) -             \
				3.0f * (*p_1) - 9.0f * (*p2);            \
			rez = (9.0f * (a + a1) - a_1 -                   \
				a2) * 0.00048828125f;                    \
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
			a2 = 29.0f * (*p) + 111.0f * (*p1) -             \
				3.0f * (*p_1) - 9.0f * (*p2);            \
			rez1 = a * 0.0078125f;                           \
			*pd = rez1;                                      \
			rez = (9.0f * (a + a1) - a_1 -                   \
				a2) * 0.00048828125f;                    \
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
			a2 = 29.0f * (*p) + 111.0f * (*p1) -             \
				3.0f * (*p_1) - 9.0f * (*p2);            \
			rez1 = a * 0.0078125f;                           \
			*pd = rez1;                                      \
			pd += dst_stride;                                \
			i++;                                             \
		    }                                                    \
		    if (i < (amount)) {                                  \
			rez = (9.0f * (a + a1) - a_1 -                   \
				a2) * 0.00048828125f;                    \
			*pd = rez;                                       \
		    }                                                    \
		}                                                        \
	    }                                                            \
	}

/* *********************************************************** */

#define	ROW_BICUBIC_FL2(beg_i, j, chan, amount)                         \
	{                                                               \
	    PREPARE_BICUBIC(beg_i, j, chan);                            \
	    psrc_end = psrc_beg + hx + (src_width_beg - 1) * chan;      \
	    if (dh == 0) {                                              \
		for (l = 0; l < chan; l++) {                            \
		    a_1 = (111.0f * psrc_beg[hx + wx_1 + l] +           \
			    29.0f * psrc_beg[hx1 + wx_1 + l] -          \
			    9.0f * psrc_beg[hx_1 + wx_1 + l] -          \
			    3.0f * psrc_beg[hx2 + wx_1 + l]);           \
		    a = (111.0f * psrc_beg[hx + wx + l] +               \
			    29.0f * psrc_beg[hx1 + wx + l] -            \
			    9.0f * psrc_beg[hx_1 + wx + l] -            \
			    3.0f * psrc_beg[hx2 + wx + l]);             \
		    a1 = (111.0f * psrc_beg[hx + wx1 + l] +             \
			    29.0f * psrc_beg[hx1 + wx1 + l] -           \
			    9.0f * psrc_beg[hx_1 + wx1 + l] -           \
			    3.0f * psrc_beg[hx2 + wx1 + l]);            \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                    \
		    p = psrc_beg + hx + wx2 + l;                        \
		    p1 = psrc_beg + hx1 + wx2 + l;                      \
		    p2 = psrc_beg + hx2 + wx2 + l;                      \
		    i = (beg_i);                                        \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;   \
		    if (dw == 1) {                                      \
			a2 = 111.0f * (*p) + 29.0f * (*p1) -            \
				9.0f * (*p_1) - 3.0f * (*p2);           \
			rez = (9.0f * (a + a1) - a_1 -                  \
				a2) * 0.00048828125f;                   \
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
			a2 = 111.0f * (*p) + 29.0f * (*p1) -            \
				9.0f * (*p_1) - 3.0f * (*p2);           \
			rez1 = a * 0.0078125f;                          \
			*pd = rez1;                                     \
			rez = (9.0f * (a + a1) - a_1 -                  \
				a2) * 0.00048828125f;                   \
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
			a2 = (111.0f * (*p) + 29.0f * (*p1) -           \
				9.0f * (*p_1) - 3.0f * (*p2));          \
			rez1 = a * 0.0078125f;                          \
			*pd = rez1;                                     \
			pd += chan;                                     \
			i++;                                            \
		    }                                                   \
		    if (i < (amount)) {                                 \
			rez = (9.0f * (a + a1) - a_1 -                  \
				a2) * 0.00048828125f;                   \
			*pd = rez;                                      \
		    }                                                   \
		}                                                       \
	    } else {                                                    \
		for (l = 0; l < chan; l++) {                            \
		    a_1 = (29.0f * psrc_beg[hx + wx_1 + l] +            \
			    111.0f * psrc_beg[hx1 + wx_1 + l] -         \
			    3.0f * psrc_beg[hx_1 + wx_1 + l] -          \
			    9.0f * psrc_beg[hx2 + wx_1 + l]);           \
		    a = (29.0f * psrc_beg[hx + wx + l] +                \
			    111.0f * psrc_beg[hx1 + wx + l] -           \
			    3.0f * psrc_beg[hx_1 + wx + l] -            \
			    9.0f * psrc_beg[hx2 + wx + l]);             \
		    a1 = (29.0f * psrc_beg[hx + wx1 + l] +              \
			    111.0f * psrc_beg[hx1 + wx1 + l] -          \
			    3.0f * psrc_beg[hx_1 + wx1 + l] -           \
			    9.0f * psrc_beg[hx2 + wx1 + l]);            \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                    \
		    p = psrc_beg + hx + wx2 + l;                        \
		    p1 = psrc_beg + hx1 + wx2 + l;                      \
		    p2 = psrc_beg + hx2 + wx2 + l;                      \
		    i = (beg_i);                                        \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;   \
		    if (dw == 1) {                                      \
			a2 = 29.0f * (*p) + 111.0f * (*p1) -            \
				3.0f * (*p_1) - 9.0f * (*p2);           \
			rez = (9.0f * (a + a1) - a_1 -                  \
				a2) * 0.00048828125f;                   \
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
			a2 = 29.0f * (*p) + 111.0f * (*p1) -            \
				3.0f * (*p_1) - 9.0f * (*p2);           \
			rez1 = a * 0.0078125f;                          \
			*pd = rez1;                                     \
			rez = (9.0f * (a + a1) - a_1 -                  \
				a2) * 0.00048828125f;                   \
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
			a2 = 29.0f * (*p) + 111.0f * (*p1) -            \
				3.0f * (*p_1) - 9.0f * (*p2);           \
			rez1 = a * 0.0078125f;                          \
			*pd = rez1;                                     \
			pd += chan;                                     \
			i++;                                            \
		    }                                                   \
		    if (i < (amount)) {                                 \
			rez = (9.0f * (a + a1) - a_1 -                  \
				a2) * 0.00048828125f;                   \
			*pd = rez;                                      \
		    }                                                   \
		}                                                       \
	    }                                                           \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC_FL2(beg_i, j, chan, amount)                         \
	{                                                                  \
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
			rez = (29.0f * a + 111.0f * a1 - 3.0f * a_1 -      \
				9.0f * a2) * 0.0078125f;                   \
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
			rez1 = (111.0f * a + 29.0f * a1 - 9.0f * a_1 -     \
				3.0f * a2) * 0.0078125f;                   \
			*pd = rez1;                                        \
			rez = (29.0f * a + 111.0f * a1 - 3.0f * a_1 -      \
				9.0f * a2) * 0.0078125f;                   \
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
			rez1 = (111.0f * a + 29.0f * a1 - 9.0f * a_1 -     \
				3.0f * a2) * 0.0078125f;                   \
			*pd = rez1;                                        \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (29.0f * a + 111.0f * a1 - 3.0f * a_1 -      \
				9.0f * a2) * 0.0078125f;                   \
			*pd = rez;                                         \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = 9.0f * (psrc_beg[hx_1 + wx + l] +                \
			    psrc_beg[hx_1 + wx1 + l]) - psrc_beg[hx_1 +    \
			    wx_1 + l] - psrc_beg[hx_1 + wx2 + l];          \
		    a = 9.0f * (psrc_beg[hx + wx + l] + psrc_beg[hx +      \
			    wx1 + l]) - psrc_beg[hx + wx_1 + l] -          \
			    psrc_beg[hx + wx2 + l];                        \
		    a1 = 9.0f * (psrc_beg[hx1 + wx + l] + psrc_beg[hx1 +   \
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
			a2 = 9.0f * ((*p) + (*p1)) - (*p_1) - (*p2);       \
			rez = (29.0f * a + 111.0f * a1 - 3.0f * a_1 -      \
				9.0f * a2) * 0.00048828125f;               \
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
			a2 = 9.0f * ((*p) + (*p1)) - (*p_1) - (*p2);       \
			rez1 = (111.0f * a + 29.0f * a1 - 9.0f * a_1 -     \
				3.0f * a2) * 0.00048828125f;               \
			*pd = rez1;                                        \
			rez = (29.0f * a + 111.0f * a1 - 3.0f * a_1 -      \
				9.0f * a2) * 0.00048828125f;               \
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
			a2 = 9.0f * ((*p) + (*p1)) - (*p_1) - (*p2);       \
			rez1 = (111.0f * a + 29.0f * a1 - 9.0f * a_1 -     \
				3.0f * a2) * 0.00048828125f;               \
			*pd = rez1;                                        \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (29.0f * a + 111.0f * a1 - 3.0f * a_1 -      \
				9.0f * a2) * 0.00048828125f;               \
			*pd = rez;                                         \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	ROW_BICUBIC_FL3(beg_i, j, chan, amount)                           \
	{                                                                 \
	    PREPARE_BICUBIC(beg_i, j, chan);                              \
	    if (dh == 0) {                                                \
		for (l = 0; l < chan; l++) {                              \
		    a_1 = psrc_beg[hx + wx_1 + l];                        \
		    a = psrc_beg[hx + wx + l];                            \
		    a1 = psrc_beg[hx + wx1 + l];                          \
		    p = psrc_beg + hx + wx2 + l;                          \
		    i = (beg_i);                                          \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;     \
		    if (dw == 0) {                                        \
			*pd = a;                                          \
			pd += chan;                                       \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = *p;                                          \
			rez = (9.0f * (a + a1) - a_1 - a2) * 0.0625f;     \
			*pd = rez;                                        \
			*(pd + chan) = a1;                                \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			p += chan;                                        \
			pd += (chan << 1);                                \
		    }                                                     \
		    if (i < (amount)) {                                   \
			if (p >= (psrc_beg + hx + l +                     \
				src_width_beg * chan))                    \
			    a2 = psrc_beg[hx + l + (src_width_beg -       \
				    1) * chan];                           \
			else                                              \
			    a2 = *p;                                      \
			rez = (9.0f * (a + a1) - a_1 - a2) * 0.0625f;     \
			*pd = rez;                                        \
			pd += chan;                                       \
			i++;                                              \
		    }                                                     \
		    if (i < (amount))                                     \
			*pd = a1;                                         \
		}                                                         \
	    } else {                                                      \
		for (l = 0; l < chan; l++) {                              \
		    a_1 = (9.0f * (psrc_beg[hx + wx_1 + l] +              \
			    psrc_beg[hx1 + wx_1 + l]) - psrc_beg[hx_1 +   \
			    wx_1 + l] - psrc_beg[hx2 + wx_1 + l]);        \
		    a = (9.0f * (psrc_beg[hx + wx + l] + psrc_beg[hx1 +   \
			    wx + l]) - psrc_beg[hx_1 + wx + l] -          \
			    psrc_beg[hx2 + wx + l]);                      \
		    a1 = (9.0f * (psrc_beg[hx + wx1 + l] +                \
			    psrc_beg[hx1 + wx1 + l]) - psrc_beg[hx_1 +    \
			    wx1 + l] - psrc_beg[hx2 + wx1 + l]);          \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                      \
		    p = psrc_beg + hx + wx2 + l;                          \
		    p1 = psrc_beg + hx1 + wx2 + l;                        \
		    p2 = psrc_beg + hx2 + wx2 + l;                        \
		    i = (beg_i);                                          \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;     \
		    if (dw == 0) {                                        \
			rez = a * 0.0625f;                                \
			*pd = rez;                                        \
			pd += chan;                                       \
			i++;                                              \
		    }                                                     \
		    for (; i < (amount) - 2; i += 2) {                    \
			a2 = (9.0f * ((*p) + (*p1)) - (*p_1) - (*p2));    \
			rez1 = (9.0f * (a + a1) - a_1 -                   \
				a2) * 0.00390625f;                        \
			*pd = rez1;                                       \
			rez = a1 * 0.0625f;                               \
			*(pd + chan) = rez;                               \
			a_1 = a;                                          \
			a = a1;                                           \
			a1 = a2;                                          \
			p_1 += chan;                                      \
			p += chan;                                        \
			p1 += chan;                                       \
			p2 += chan;                                       \
			pd += (chan << 1);                                \
		    }                                                     \
		    if (i < (amount)) {                                   \
			if (p >= (psrc_beg + hx + l +                     \
				src_width_beg * chan))                    \
			    a2 = (9.0f * (psrc_beg[hx + l +               \
				    (src_width_beg - 1) * chan] +         \
				    psrc_beg[hx1 + l + (src_width_beg -   \
				    1) * chan]) - psrc_beg[hx_1 + l +     \
				    (src_width_beg - 1) * chan] -         \
				    psrc_beg[hx2 + l + (src_width_beg -   \
				    1) * chan]);                          \
			else                                              \
			    a2 = (9.0f * ((*p) + (*p1)) - (*p_1) -        \
				    (*p2));                               \
			rez1 = (9.0f * (a + a1) - a_1 -                   \
				a2) * 0.00390625f;                        \
			*pd = rez1;                                       \
			pd += chan;                                       \
			i++;                                              \
		    }                                                     \
		    if (i < (amount)) {                                   \
			rez = a1 * 0.0625f;                               \
			*pd = rez;                                        \
		    }                                                     \
		}                                                         \
	    }                                                             \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC_FL3(beg_i, j, chan, amount)                         \
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
			rez = (9.0f * (a + a1) - a_1 - a2) * 0.0625f;      \
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
			rez = (9.0f * (a + a1) - a_1 - a2) * 0.0625f;      \
			*pd = rez;                                         \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount))                                      \
			*pd = a1;                                          \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (9.0f * (psrc_beg[hx_1 + wx + l] +               \
			    psrc_beg[hx_1 + wx1 + l]) - psrc_beg[hx_1 +    \
			    wx_1 + l] - psrc_beg[hx_1 + wx2 + l]);         \
		    a = (9.0f * (psrc_beg[hx + wx + l] + psrc_beg[hx +     \
			    wx1 + l]) - psrc_beg[hx + wx_1 + l] -          \
			    psrc_beg[hx + wx2 + l]);                       \
		    a1 = (9.0f * (psrc_beg[hx1 + wx + l] +                 \
			    psrc_beg[hx1 + wx1 + l]) - psrc_beg[hx1 +      \
			    wx_1 + l] - psrc_beg[hx1 + wx2 + l]);          \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 0) {                                         \
			rez = a * 0.0625f;                                 \
			*pd = rez;                                         \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = (9.0f * ((*p) + (*p1)) - (*p_1) - (*p2));     \
			rez1 = (9.0f * (a + a1) - a_1 -                    \
				a2) * 0.00390625f;                         \
			*pd = rez1;                                        \
			rez = a1 * 0.0625f;                                \
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
			    a2 = (9.0f * (psrc_beg[wx + l +                \
				    (src_height_beg - 1) * src_stride] +   \
				    psrc_beg[wx1 + l + (src_height_beg -   \
				    1) * src_stride]) - psrc_beg[wx_1 +    \
				    l + (src_height_beg -                  \
				    1) * src_stride] - psrc_beg[wx2 +      \
				    l + (src_height_beg -                  \
				    1) * src_stride]);                     \
			else                                               \
			    a2 = (9.0f * ((*p) + (*p1)) - (*p_1) -         \
				    (*p2));                                \
			rez1 = (9.0f * (a + a1) - a_1 -                    \
				a2) * 0.00390625f;                         \
			*pd = rez1;                                        \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = a1 * 0.0625f;                                \
			*pd = rez;                                         \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

void
mlib_ImageZoomIn2X_F32_SE_BC_FL0(
	MLIB_ZOOMIN2XDEFEDGE_BC(mlib_f32))
{
	const mlib_f32 *p_1, *p, *p1, *p2, *psrc_end;
	mlib_f32 *pdst_count, *pd;
	mlib_s32 ii, jj;

	pdst_count = pdst_near;
	ii = 0;
	jj = 0;

	while (((pdst - pdst_count) >= dst_stride) && (src_height_near > 0)) {
		ROW_BICUBIC_FL0(ii, jj, channels, src_width_near);
		jj++;
		pdst_count += dst_stride;
		src_height_near--;
	}

	while (src_height_near > src_height) {
		ROW_BICUBIC_FL0(ii, jj + src_height_near - 1, channels,
			src_width_near);
		src_height_near--;
	}

	if (src_height_near > 0) {
		while ((pdst - pdst_count > 0) && (src_width_near > 0)) {
			COLUMN_BICUBIC_FL0(ii, jj, channels,
				(src_height_near + jj));
			src_width_near--;
			pdst_count += channels;
			ii++;
		}

		while (src_width_near > src_width) {
			COLUMN_BICUBIC_FL0(ii + src_width_near - 1, jj,
				channels, (src_height_near + jj));
			src_width_near--;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIn2X_F32_SE_BC_FL1(
	MLIB_ZOOMIN2XDEFEDGE_BC(mlib_f32))
{
	const mlib_f32 *p_1, *p, *p1, *p2, *psrc_end;
	mlib_f32 *pdst_count, *pd;
	mlib_s32 ii, jj;

	pdst_count = pdst_near;
	ii = 0;
	jj = 0;

	while (((pdst - pdst_count) >= dst_stride) && (src_height_near > 0)) {
		ROW_BICUBIC_FL1(ii, jj, channels, src_width_near);
		jj++;
		pdst_count += dst_stride;
		src_height_near--;
	}

	while (src_height_near > src_height) {
		ROW_BICUBIC_FL1(ii, jj + src_height_near - 1, channels,
			src_width_near);
		src_height_near--;
	}

	if (src_height_near > 0) {
		while ((pdst - pdst_count > 0) && (src_width_near > 0)) {
			COLUMN_BICUBIC_FL1(ii, jj, channels,
				(src_height_near + jj));
			src_width_near--;
			pdst_count += channels;
			ii++;
		}

		while (src_width_near > src_width) {
			COLUMN_BICUBIC_FL1(ii + src_width_near - 1, jj,
				channels, (src_height_near + jj));
			src_width_near--;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIn2X_F32_SE_BC_FL2(
	MLIB_ZOOMIN2XDEFEDGE_BC(mlib_f32))
{
	const mlib_f32 *p_1, *p, *p1, *p2, *psrc_end;
	mlib_f32 *pdst_count, *pd;

	mlib_s32 ii, jj;

	pdst_count = pdst_near;
	ii = 0;
	jj = 0;

	while (((pdst - pdst_count) >= dst_stride) && (src_height_near > 0)) {
		ROW_BICUBIC_FL2(ii, jj, channels, src_width_near);
		jj++;
		pdst_count += dst_stride;
		src_height_near--;
	}

	while (src_height_near > src_height) {
		ROW_BICUBIC_FL2(ii, jj + src_height_near - 1, channels,
			src_width_near);
		src_height_near--;
	}

	if (src_height_near > 0) {
		while ((pdst - pdst_count > 0) && (src_width_near > 0)) {
			COLUMN_BICUBIC_FL2(ii, jj, channels,
				(src_height_near + jj));
			src_width_near--;
			pdst_count += channels;
			ii++;
		}

		while (src_width_near > src_width) {
			COLUMN_BICUBIC_FL2(ii + src_width_near - 1, jj,
				channels, (src_height_near + jj));
			src_width_near--;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIn2X_F32_SE_BC_FL3(
	MLIB_ZOOMIN2XDEFEDGE_BC(mlib_f32))
{
	const mlib_f32 *p_1, *p, *p1, *p2;
	mlib_f32 *pdst_count, *pd;

	mlib_s32 ii, jj;

	pdst_count = pdst_near;
	ii = 0;
	jj = 0;

	while (((pdst - pdst_count) >= dst_stride) && (src_height_near > 0)) {
		ROW_BICUBIC_FL3(ii, jj, channels, src_width_near);
		jj++;
		pdst_count += dst_stride;
		src_height_near--;
	}

	while (src_height_near > src_height) {
		ROW_BICUBIC_FL3(ii, jj + src_height_near - 1, channels,
			src_width_near);
		src_height_near--;
	}

	if (src_height_near > 0) {
		while ((pdst - pdst_count > 0) && (src_width_near > 0)) {
			COLUMN_BICUBIC_FL3(ii, jj, channels,
				(src_height_near + jj));
			src_width_near--;
			pdst_count += channels;
			ii++;
		}

		while (src_width_near > src_width) {
			COLUMN_BICUBIC_FL3(ii + src_width_near - 1, jj,
				channels, (src_height_near + jj));
			src_width_near--;
		}
	}
}

/* *********************************************************** */

#define	ROW_BICUBIC2_FL0(beg_i, j, chan, amount)                        \
	{                                                               \
	    PREPARE_BICUBIC(beg_i, j, chan);                            \
	    psrc_end = psrc_beg + hx + (src_width_beg - 1) * chan;      \
	    if (dh == 0) {                                              \
		for (l = 0; l < chan; l++) {                            \
		    a_1 = (57.0f * psrc_beg[hx + wx_1 + l] +            \
			    19.0f * psrc_beg[hx1 + wx_1 + l] -          \
			    9.0f * psrc_beg[hx_1 + wx_1 + l] -          \
			    3.0f * psrc_beg[hx2 + wx_1 + l]);           \
		    a = (57.0f * psrc_beg[hx + wx + l] +                \
			    19.0f * psrc_beg[hx1 + wx + l] -            \
			    9.0f * psrc_beg[hx_1 + wx + l] -            \
			    3.0f * psrc_beg[hx2 + wx + l]);             \
		    a1 = (57.0f * psrc_beg[hx + wx1 + l] +              \
			    19.0f * psrc_beg[hx1 + wx1 + l] -           \
			    9.0f * psrc_beg[hx_1 + wx1 + l] -           \
			    3.0f * psrc_beg[hx2 + wx1 + l]);            \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                    \
		    p = psrc_beg + hx + wx2 + l;                        \
		    p1 = psrc_beg + hx1 + wx2 + l;                      \
		    p2 = psrc_beg + hx2 + wx2 + l;                      \
		    i = (beg_i);                                        \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;   \
		    if (dw == 1) {                                      \
			a2 = 57.0f * (*p) + 19.0f * (*p1) -             \
				9.0f * (*p_1) - 3.0f * (*p2);           \
			rez = (19.0f * a + 57.0f * a1 - 3.0f * a_1 -    \
				9.0f * a2) * 0.000244140625f;           \
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
			a2 = 57.0f * (*p) + 19.0f * (*p1) -             \
				9.0f * (*p_1) - 3.0f * (*p2);           \
			rez1 = (57.0f * a + 19.0f * a1 - 9.0f * a_1 -   \
				3.0f * a2) * 0.000244140625f;           \
			*pd = rez1;                                     \
			rez = (19.0f * a + 57.0f * a1 - 3.0f * a_1 -    \
				9.0f * a2) * 0.000244140625f;           \
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
			a2 = (57.0f * (*p) + 19.0f * (*p1) -            \
				9.0f * (*p_1) - 3.0f * (*p2));          \
			rez1 = (57.0f * a + 19.0f * a1 - 9.0f * a_1 -   \
				3.0f * a2) * 0.000244140625f;           \
			*pd = rez1;                                     \
			pd += chan;                                     \
			i++;                                            \
		    }                                                   \
		    if (i < (amount)) {                                 \
			rez = (19.0f * a + 57.0f * a1 - 3.0f * a_1 -    \
				9.0f * a2) * 0.000244140625f;           \
			*pd = rez;                                      \
		    }                                                   \
		}                                                       \
	    } else {                                                    \
		for (l = 0; l < chan; l++) {                            \
		    a_1 = (19.0f * psrc_beg[hx + wx_1 + l] +            \
			    57.0f * psrc_beg[hx1 + wx_1 + l] -          \
			    3.0f * psrc_beg[hx_1 + wx_1 + l] -          \
			    9.0f * psrc_beg[hx2 + wx_1 + l]);           \
		    a = (19.0f * psrc_beg[hx + wx + l] +                \
			    57.0f * psrc_beg[hx1 + wx + l] -            \
			    3.0f * psrc_beg[hx_1 + wx + l] -            \
			    9.0f * psrc_beg[hx2 + wx + l]);             \
		    a1 = (19.0f * psrc_beg[hx + wx1 + l] +              \
			    57.0f * psrc_beg[hx1 + wx1 + l] -           \
			    3.0f * psrc_beg[hx_1 + wx1 + l] -           \
			    9.0f * psrc_beg[hx2 + wx1 + l]);            \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                    \
		    p = psrc_beg + hx + wx2 + l;                        \
		    p1 = psrc_beg + hx1 + wx2 + l;                      \
		    p2 = psrc_beg + hx2 + wx2 + l;                      \
		    i = (beg_i);                                        \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;   \
		    if (dw == 1) {                                      \
			a2 = 19.0f * (*p) + 57.0f * (*p1) -             \
				3.0f * (*p_1) - 9.0f * (*p2);           \
			rez = (19.0f * a + 57.0f * a1 - 3.0f * a_1 -    \
				9.0f * a2) * 0.000244140625f;           \
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
			a2 = 19.0f * (*p) + 57.0f * (*p1) -             \
				3.0f * (*p_1) - 9.0f * (*p2);           \
			rez1 = (57.0f * a + 19.0f * a1 - 9.0f * a_1 -   \
				3.0f * a2) * 0.000244140625f;           \
			*pd = rez1;                                     \
			rez = (19.0f * a + 57.0f * a1 - 3.0f * a_1 -    \
				9.0f * a2) * 0.000244140625f;           \
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
			a2 = 19.0f * (*p) + 57.0f * (*p1) -             \
				3.0f * (*p_1) - 9.0f * (*p2);           \
			rez1 = (57.0f * a + 19.0f * a1 - 9.0f * a_1 -   \
				3.0f * a2) * 0.000244140625f;           \
			*pd = rez1;                                     \
			pd += chan;                                     \
			i++;                                            \
		    }                                                   \
		    if (i < (amount)) {                                 \
			rez = (19.0f * a + 57.0f * a1 - 3.0f * a_1 -    \
				9.0f * a2) * 0.000244140625f;           \
			*pd = rez;                                      \
		    }                                                   \
		}                                                       \
	    }                                                           \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC2_FL0(beg_i, j, chan, amount)                      \
	{                                                                \
	    PREPARE_BICUBIC(beg_i, j, chan);                             \
	    psrc_end =                                                   \
		    psrc_beg + wx + (src_height_beg - 1) * src_stride;   \
	    if (dw == 0) {                                               \
		for (l = 0; l < chan; l++) {                             \
		    a_1 = (57.0f * psrc_beg[hx_1 + wx + l] +             \
			    19.0f * psrc_beg[hx_1 + wx1 + l] -           \
			    9.0f * psrc_beg[hx_1 + wx_1 + l] -           \
			    3.0f * psrc_beg[hx_1 + wx2 + l]);            \
		    a = (57.0f * psrc_beg[hx + wx + l] +                 \
			    19.0f * psrc_beg[hx + wx1 + l] -             \
			    9.0f * psrc_beg[hx + wx_1 + l] -             \
			    3.0f * psrc_beg[hx + wx2 + l]);              \
		    a1 = (57.0f * psrc_beg[hx1 + wx + l] +               \
			    19.0f * psrc_beg[hx1 + wx1 + l] -            \
			    9.0f * psrc_beg[hx1 + wx_1 + l] -            \
			    3.0f * psrc_beg[hx1 + wx2 + l]);             \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                     \
		    p = psrc_beg + hx2 + wx + l;                         \
		    p1 = psrc_beg + hx2 + wx1 + l;                       \
		    p2 = psrc_beg + hx2 + wx2 + l;                       \
		    i = (j);                                             \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +   \
			    l;                                           \
		    if (dh == 1) {                                       \
			a2 = 57.0f * (*p) + 19.0f * (*p1) -              \
				9.0f * (*p_1) - 3.0f * (*p2);            \
			rez = (19.0f * a + 57.0f * a1 - 3.0f * a_1 -     \
				9.0f * a2) * 0.000244140625f;            \
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
			a2 = 57.0f * (*p) + 19.0f * (*p1) -              \
				9.0f * (*p_1) - 3.0f * (*p2);            \
			rez1 = (57.0f * a + 19.0f * a1 - 9.0f * a_1 -    \
				3.0f * a2) * 0.000244140625f;            \
			*pd = rez1;                                      \
			rez = (19.0f * a + 57.0f * a1 - 3.0f * a_1 -     \
				9.0f * a2) * 0.000244140625f;            \
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
			a2 = 57.0f * (*p) + 19.0f * (*p1) -              \
				9.0f * (*p_1) - 3.0f * (*p2);            \
			rez1 = (57.0f * a + 19.0f * a1 - 9.0f * a_1 -    \
				3.0f * a2) * 0.000244140625f;            \
			*pd = rez1;                                      \
			pd += dst_stride;                                \
			i++;                                             \
		    }                                                    \
		    if (i < (amount)) {                                  \
			rez = (19.0f * a + 57.0f * a1 - 3.0f * a_1 -     \
				9.0f * a2) * 0.000244140625f;            \
			*pd = rez;                                       \
		    }                                                    \
		}                                                        \
	    } else {                                                     \
		for (l = 0; l < chan; l++) {                             \
		    a_1 = (19.0f * psrc_beg[hx_1 + wx + l] +             \
			    57.0f * psrc_beg[hx_1 + wx1 + l] -           \
			    3.0f * psrc_beg[hx_1 + wx_1 + l] -           \
			    9.0f * psrc_beg[hx_1 + wx2 + l]);            \
		    a = (19.0f * psrc_beg[hx + wx + l] +                 \
			    57.0f * psrc_beg[hx + wx1 + l] -             \
			    3.0f * psrc_beg[hx + wx_1 + l] -             \
			    9.0f * psrc_beg[hx + wx2 + l]);              \
		    a1 = (19.0f * psrc_beg[hx1 + wx + l] +               \
			    57.0f * psrc_beg[hx1 + wx1 + l] -            \
			    3.0f * psrc_beg[hx1 + wx_1 + l] -            \
			    9.0f * psrc_beg[hx1 + wx2 + l]);             \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                     \
		    p = psrc_beg + hx2 + wx + l;                         \
		    p1 = psrc_beg + hx2 + wx1 + l;                       \
		    p2 = psrc_beg + hx2 + wx2 + l;                       \
		    i = (j);                                             \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +   \
			    l;                                           \
		    if (dh == 1) {                                       \
			a2 = 19.0f * (*p) + 57.0f * (*p1) -              \
				3.0f * (*p_1) - 9.0f * (*p2);            \
			rez = (19.0f * a + 57.0f * a1 - 3.0f * a_1 -     \
				9.0f * a2) * 0.000244140625f;            \
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
			a2 = 19.0f * (*p) + 57.0f * (*p1) -              \
				3.0f * (*p_1) - 9.0f * (*p2);            \
			rez1 = (57.0f * a + 19.0f * a1 - 9.0f * a_1 -    \
				3.0f * a2) * 0.000244140625f;            \
			*pd = rez1;                                      \
			rez = (19.0f * a + 57.0f * a1 - 3.0f * a_1 -     \
				9.0f * a2) * 0.000244140625f;            \
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
			a2 = 19.0f * (*p) + 57.0f * (*p1) -              \
				3.0f * (*p_1) - 9.0f * (*p2);            \
			rez1 = (57.0f * a + 19.0f * a1 - 9.0f * a_1 -    \
				3.0f * a2) * 0.000244140625f;            \
			*pd = rez1;                                      \
			pd += dst_stride;                                \
			i++;                                             \
		    }                                                    \
		    if (i < (amount)) {                                  \
			rez = (19.0f * a + 57.0f * a1 - 3.0f * a_1 -     \
				9.0f * a2) * 0.000244140625f;            \
			*pd = rez;                                       \
		    }                                                    \
		}                                                        \
	    }                                                            \
	}

/* *********************************************************** */

#define	ROW_BICUBIC2_FL1(beg_i, j, chan, amount)                           \
	{                                                                  \
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
			rez = (19.0f * a + 57.0f * a1 - 3.0f * a_1 -       \
				9.0f * a2) * 0.015625f;                    \
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
			rez1 = (57.0f * a + 19.0f * a1 - 9.0f * a_1 -      \
				3.0f * a2) * 0.015625f;                    \
			*pd = rez1;                                        \
			rez = (19.0f * a + 57.0f * a1 - 3.0f * a_1 -       \
				9.0f * a2) * 0.015625f;                    \
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
			rez1 = (57.0f * a + 19.0f * a1 - 9.0f * a_1 -      \
				3.0f * a2) * 0.015625f;                    \
			*pd = rez1;                                        \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (19.0f * a + 57.0f * a1 - 3.0f * a_1 -       \
				9.0f * a2) * 0.015625f;                    \
			*pd = rez;                                         \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = 5.0f * (psrc_beg[hx + wx_1 + l] +                \
			    psrc_beg[hx1 + wx_1 + l]) - psrc_beg[hx_1 +    \
			    wx_1 + l] - psrc_beg[hx2 + wx_1 + l];          \
		    a = 5.0f * (psrc_beg[hx + wx + l] + psrc_beg[hx1 +     \
			    wx + l]) - psrc_beg[hx_1 + wx + l] -           \
			    psrc_beg[hx2 + wx + l];                        \
		    a1 = 5.0f * (psrc_beg[hx + wx1 + l] + psrc_beg[hx1 +   \
			    wx1 + l]) - psrc_beg[hx_1 + wx1 + l] -         \
			    psrc_beg[hx2 + wx1 + l];                       \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                       \
		    p = psrc_beg + hx + wx2 + l;                           \
		    p1 = psrc_beg + hx1 + wx2 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 1) {                                         \
			a2 = 5.0f * ((*p) + (*p1)) - (*p_1) - (*p2);       \
			rez = (19.0f * a + 57.0f * a1 - 3.0f * a_1 -       \
				9.0f * a2) * 0.001953125f;                 \
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
			a2 = 5.0f * ((*p) + (*p1)) - (*p_1) - (*p2);       \
			rez1 = (57.0f * a + 19.0f * a1 - 9.0f * a_1 -      \
				3.0f * a2) * 0.001953125f;                 \
			*pd = rez1;                                        \
			rez = (19.0f * a + 57.0f * a1 - 3.0f * a_1 -       \
				9.0f * a2) * 0.001953125f;                 \
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
			a2 = 5.0f * ((*p) + (*p1)) - (*p_1) - (*p2);       \
			rez1 = (57.0f * a + 19.0f * a1 - 9.0f * a_1 -      \
				3.0f * a2) * 0.001953125f;                 \
			*pd = rez1;                                        \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (19.0f * a + 57.0f * a1 - 3.0f * a_1 -       \
				9.0f * a2) * 0.001953125f;                 \
			*pd = rez;                                         \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC2_FL1(beg_i, j, chan, amount)                      \
	{                                                                \
	    PREPARE_BICUBIC(beg_i, j, chan);                             \
	    psrc_end =                                                   \
		    psrc_beg + wx + (src_height_beg - 1) * src_stride;   \
	    if (dw == 0) {                                               \
		for (l = 0; l < chan; l++) {                             \
		    a_1 = (57.0f * psrc_beg[hx_1 + wx + l] +             \
			    19.0f * psrc_beg[hx_1 + wx1 + l] -           \
			    9.0f * psrc_beg[hx_1 + wx_1 + l] -           \
			    3.0f * psrc_beg[hx_1 + wx2 + l]);            \
		    a = (57.0f * psrc_beg[hx + wx + l] +                 \
			    19.0f * psrc_beg[hx + wx1 + l] -             \
			    9.0f * psrc_beg[hx + wx_1 + l] -             \
			    3.0f * psrc_beg[hx + wx2 + l]);              \
		    a1 = (57.0f * psrc_beg[hx1 + wx + l] +               \
			    19.0f * psrc_beg[hx1 + wx1 + l] -            \
			    9.0f * psrc_beg[hx1 + wx_1 + l] -            \
			    3.0f * psrc_beg[hx1 + wx2 + l]);             \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                     \
		    p = psrc_beg + hx2 + wx + l;                         \
		    p1 = psrc_beg + hx2 + wx1 + l;                       \
		    p2 = psrc_beg + hx2 + wx2 + l;                       \
		    i = (j);                                             \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +   \
			    l;                                           \
		    if (dh == 1) {                                       \
			a2 = 57.0f * (*p) + 19.0f * (*p1) -              \
				9.0f * (*p_1) - 3.0f * (*p2);            \
			rez = (5.0f * (a + a1) - a_1 -                   \
				a2) * 0.001953125f;                      \
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
			a2 = 57.0f * (*p) + 19.0f * (*p1) -              \
				9.0f * (*p_1) - 3.0f * (*p2);            \
			rez1 = a * 0.015625f;                            \
			*pd = rez1;                                      \
			rez = (5.0f * (a + a1) - a_1 -                   \
				a2) * 0.001953125f;                      \
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
			a2 = 57.0f * (*p) + 19.0f * (*p1) -              \
				9.0f * (*p_1) - 3.0f * (*p2);            \
			rez1 = a * 0.015625f;                            \
			*pd = rez1;                                      \
			pd += dst_stride;                                \
			i++;                                             \
		    }                                                    \
		    if (i < (amount)) {                                  \
			rez = (5.0f * (a + a1) - a_1 -                   \
				a2) * 0.001953125f;                      \
			*pd = rez;                                       \
		    }                                                    \
		}                                                        \
	    } else {                                                     \
		for (l = 0; l < chan; l++) {                             \
		    a_1 = (19.0f * psrc_beg[hx_1 + wx + l] +             \
			    57.0f * psrc_beg[hx_1 + wx1 + l] -           \
			    3.0f * psrc_beg[hx_1 + wx_1 + l] -           \
			    9.0f * psrc_beg[hx_1 + wx2 + l]);            \
		    a = (19.0f * psrc_beg[hx + wx + l] +                 \
			    57.0f * psrc_beg[hx + wx1 + l] -             \
			    3.0f * psrc_beg[hx + wx_1 + l] -             \
			    9.0f * psrc_beg[hx + wx2 + l]);              \
		    a1 = (19.0f * psrc_beg[hx1 + wx + l] +               \
			    57.0f * psrc_beg[hx1 + wx1 + l] -            \
			    3.0f * psrc_beg[hx1 + wx_1 + l] -            \
			    9.0f * psrc_beg[hx1 + wx2 + l]);             \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                     \
		    p = psrc_beg + hx2 + wx + l;                         \
		    p1 = psrc_beg + hx2 + wx1 + l;                       \
		    p2 = psrc_beg + hx2 + wx2 + l;                       \
		    i = (j);                                             \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +   \
			    l;                                           \
		    if (dh == 1) {                                       \
			a2 = 19.0f * (*p) + 57.0f * (*p1) -              \
				3.0f * (*p_1) - 9.0f * (*p2);            \
			rez = (5.0f * (a + a1) - a_1 -                   \
				a2) * 0.001953125f;                      \
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
			a2 = 19.0f * (*p) + 57.0f * (*p1) -              \
				3.0f * (*p_1) - 9.0f * (*p2);            \
			rez1 = a * 0.015625f;                            \
			*pd = rez1;                                      \
			rez = (5.0f * (a + a1) - a_1 -                   \
				a2) * 0.001953125f;                      \
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
			a2 = 19.0f * (*p) + 57.0f * (*p1) -              \
				3.0f * (*p_1) - 9.0f * (*p2);            \
			rez1 = a * 0.015625f;                            \
			*pd = rez1;                                      \
			pd += dst_stride;                                \
			i++;                                             \
		    }                                                    \
		    if (i < (amount)) {                                  \
			rez = (5.0f * (a + a1) - a_1 -                   \
				a2) * 0.001953125f;                      \
			*pd = rez;                                       \
		    }                                                    \
		}                                                        \
	    }                                                            \
	}

/* *********************************************************** */

#define	ROW_BICUBIC2_FL2(beg_i, j, chan, amount)                        \
	{                                                               \
	    PREPARE_BICUBIC(beg_i, j, chan);                            \
	    psrc_end = psrc_beg + hx + (src_width_beg - 1) * chan;      \
	    if (dh == 0) {                                              \
		for (l = 0; l < chan; l++) {                            \
		    a_1 = (57.0f * psrc_beg[hx + wx_1 + l] +            \
			    19.0f * psrc_beg[hx1 + wx_1 + l] -          \
			    9.0f * psrc_beg[hx_1 + wx_1 + l] -          \
			    3.0f * psrc_beg[hx2 + wx_1 + l]);           \
		    a = (57.0f * psrc_beg[hx + wx + l] +                \
			    19.0f * psrc_beg[hx1 + wx + l] -            \
			    9.0f * psrc_beg[hx_1 + wx + l] -            \
			    3.0f * psrc_beg[hx2 + wx + l]);             \
		    a1 = (57.0f * psrc_beg[hx + wx1 + l] +              \
			    19.0f * psrc_beg[hx1 + wx1 + l] -           \
			    9.0f * psrc_beg[hx_1 + wx1 + l] -           \
			    3.0f * psrc_beg[hx2 + wx1 + l]);            \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                    \
		    p = psrc_beg + hx + wx2 + l;                        \
		    p1 = psrc_beg + hx1 + wx2 + l;                      \
		    p2 = psrc_beg + hx2 + wx2 + l;                      \
		    i = (beg_i);                                        \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;   \
		    if (dw == 1) {                                      \
			a2 = 57.0f * (*p) + 19.0f * (*p1) -             \
				9.0f * (*p_1) - 3.0f * (*p2);           \
			rez = (5.0f * (a + a1) - a_1 -                  \
				a2) * 0.001953125f;                     \
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
			a2 = 57.0f * (*p) + 19.0f * (*p1) -             \
				9.0f * (*p_1) - 3.0f * (*p2);           \
			rez1 = a * 0.015625f;                           \
			*pd = rez1;                                     \
			rez = (5.0f * (a + a1) - a_1 -                  \
				a2) * 0.001953125f;                     \
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
			a2 = (57.0f * (*p) + 19.0f * (*p1) -            \
				9.0f * (*p_1) - 3.0f * (*p2));          \
			rez1 = a * 0.015625f;                           \
			*pd = rez1;                                     \
			pd += chan;                                     \
			i++;                                            \
		    }                                                   \
		    if (i < (amount)) {                                 \
			rez = (5.0f * (a + a1) - a_1 -                  \
				a2) * 0.001953125f;                     \
			*pd = rez;                                      \
		    }                                                   \
		}                                                       \
	    } else {                                                    \
		for (l = 0; l < chan; l++) {                            \
		    a_1 = (19.0f * psrc_beg[hx + wx_1 + l] +            \
			    57.0f * psrc_beg[hx1 + wx_1 + l] -          \
			    3.0f * psrc_beg[hx_1 + wx_1 + l] -          \
			    9.0f * psrc_beg[hx2 + wx_1 + l]);           \
		    a = (19.0f * psrc_beg[hx + wx + l] +                \
			    57.0f * psrc_beg[hx1 + wx + l] -            \
			    3.0f * psrc_beg[hx_1 + wx + l] -            \
			    9.0f * psrc_beg[hx2 + wx + l]);             \
		    a1 = (19.0f * psrc_beg[hx + wx1 + l] +              \
			    57.0f * psrc_beg[hx1 + wx1 + l] -           \
			    3.0f * psrc_beg[hx_1 + wx1 + l] -           \
			    9.0f * psrc_beg[hx2 + wx1 + l]);            \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                    \
		    p = psrc_beg + hx + wx2 + l;                        \
		    p1 = psrc_beg + hx1 + wx2 + l;                      \
		    p2 = psrc_beg + hx2 + wx2 + l;                      \
		    i = (beg_i);                                        \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;   \
		    if (dw == 1) {                                      \
			a2 = 19.0f * (*p) + 57.0f * (*p1) -             \
				3.0f * (*p_1) - 9.0f * (*p2);           \
			rez = (5.0f * (a + a1) - a_1 -                  \
				a2) * 0.001953125f;                     \
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
			a2 = 19.0f * (*p) + 57.0f * (*p1) -             \
				3.0f * (*p_1) - 9.0f * (*p2);           \
			rez1 = a * 0.015625f;                           \
			*pd = rez1;                                     \
			rez = (5.0f * (a + a1) - a_1 -                  \
				a2) * 0.001953125f;                     \
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
			a2 = 19.0f * (*p) + 57.0f * (*p1) -             \
				3.0f * (*p_1) - 9.0f * (*p2);           \
			rez1 = a * 0.015625f;                           \
			*pd = rez1;                                     \
			pd += chan;                                     \
			i++;                                            \
		    }                                                   \
		    if (i < (amount)) {                                 \
			rez = (5.0f * (a + a1) - a_1 -                  \
				a2) * 0.001953125f;                     \
			*pd = rez;                                      \
		    }                                                   \
		}                                                       \
	    }                                                           \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC2_FL2(beg_i, j, chan, amount)                        \
	{                                                                  \
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
			rez = (19.0f * a + 57.0f * a1 - 3.0f * a_1 -       \
				9.0f * a2) * 0.015625f;                    \
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
			rez1 = (57.0f * a + 19.0f * a1 - 9.0f * a_1 -      \
				3.0f * a2) * 0.015625f;                    \
			*pd = rez1;                                        \
			rez = (19.0f * a + 57.0f * a1 - 3.0f * a_1 -       \
				9.0f * a2) * 0.015625f;                    \
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
			rez1 = (57.0f * a + 19.0f * a1 - 9.0f * a_1 -      \
				3.0f * a2) * 0.015625f;                    \
			*pd = rez1;                                        \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (19.0f * a + 57.0f * a1 - 3.0f * a_1 -       \
				9.0f * a2) * 0.015625f;                    \
			*pd = rez;                                         \
		    }                                                      \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = 5.0f * (psrc_beg[hx_1 + wx + l] +                \
			    psrc_beg[hx_1 + wx1 + l]) - psrc_beg[hx_1 +    \
			    wx_1 + l] - psrc_beg[hx_1 + wx2 + l];          \
		    a = 5.0f * (psrc_beg[hx + wx + l] + psrc_beg[hx +      \
			    wx1 + l]) - psrc_beg[hx + wx_1 + l] -          \
			    psrc_beg[hx + wx2 + l];                        \
		    a1 = 5.0f * (psrc_beg[hx1 + wx + l] + psrc_beg[hx1 +   \
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
			a2 = 5.0f * ((*p) + (*p1)) - (*p_1) - (*p2);       \
			rez = (19.0f * a + 57.0f * a1 - 3.0f * a_1 -       \
				9.0f * a2) * 0.001953125f;                 \
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
			a2 = 5.0f * ((*p) + (*p1)) - (*p_1) - (*p2);       \
			rez1 = (57.0f * a + 19.0f * a1 - 9.0f * a_1 -      \
				3.0f * a2) * 0.001953125f;                 \
			*pd = rez1;                                        \
			rez = (19.0f * a + 57.0f * a1 - 3.0f * a_1 -       \
				9.0f * a2) * 0.001953125f;                 \
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
			a2 = 5.0f * ((*p) + (*p1)) - (*p_1) - (*p2);       \
			rez1 = (57.0f * a + 19.0f * a1 - 9.0f * a_1 -      \
				3.0f * a2) * 0.001953125f;                 \
			*pd = rez1;                                        \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = (19.0f * a + 57.0f * a1 - 3.0f * a_1 -       \
				9.0f * a2) * 0.001953125f;                 \
			*pd = rez;                                         \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	ROW_BICUBIC2_FL3(beg_i, j, chan, amount)                           \
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
			rez = (5.0f * (a + a1) - a_1 - a2) * 0.125f;       \
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
			rez = (5.0f * (a + a1) - a_1 - a2) * 0.125f;       \
			*pd = rez;                                         \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount))                                      \
			*pd = a1;                                          \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (5.0f * (psrc_beg[hx + wx_1 + l] +               \
			    psrc_beg[hx1 + wx_1 + l]) - psrc_beg[hx_1 +    \
			    wx_1 + l] - psrc_beg[hx2 + wx_1 + l]);         \
		    a = (5.0f * (psrc_beg[hx + wx + l] + psrc_beg[hx1 +    \
			    wx + l]) - psrc_beg[hx_1 + wx + l] -           \
			    psrc_beg[hx2 + wx + l]);                       \
		    a1 = (5.0f * (psrc_beg[hx + wx1 + l] +                 \
			    psrc_beg[hx1 + wx1 + l]) - psrc_beg[hx_1 +     \
			    wx1 + l] - psrc_beg[hx2 + wx1 + l]);           \
		    p_1 = psrc_beg + hx_1 + wx2 + l;                       \
		    p = psrc_beg + hx + wx2 + l;                           \
		    p1 = psrc_beg + hx1 + wx2 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (beg_i);                                           \
		    pd = pdst_near + (j) * dst_stride + i * chan + l;      \
		    if (dw == 0) {                                         \
			rez = a * 0.125f;                                  \
			*pd = rez;                                         \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = (5.0f * ((*p) + (*p1)) - (*p_1) - (*p2));     \
			rez1 = (5.0f * (a + a1) - a_1 - a2) * 0.015625f;   \
			*pd = rez1;                                        \
			rez = a1 * 0.125f;                                 \
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
			    a2 = (5.0f * (psrc_beg[hx + l +                \
				    (src_width_beg - 1) * chan] +          \
				    psrc_beg[hx1 + l + (src_width_beg -    \
				    1) * chan]) - psrc_beg[hx_1 + l +      \
				    (src_width_beg - 1) * chan] -          \
				    psrc_beg[hx2 + l + (src_width_beg -    \
				    1) * chan]);                           \
			else                                               \
			    a2 = (5.0f * ((*p) + (*p1)) - (*p_1) -         \
				    (*p2));                                \
			rez1 = (5.0f * (a + a1) - a_1 - a2) * 0.015625f;   \
			*pd = rez1;                                        \
			pd += chan;                                        \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = a1 * 0.125f;                                 \
			*pd = rez;                                         \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	COLUMN_BICUBIC2_FL3(beg_i, j, chan, amount)                        \
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
			rez = (5.0f * (a + a1) - a_1 - a2) * 0.125f;       \
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
			rez = (5.0f * (a + a1) - a_1 - a2) * 0.125f;       \
			*pd = rez;                                         \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount))                                      \
			*pd = a1;                                          \
		}                                                          \
	    } else {                                                       \
		for (l = 0; l < chan; l++) {                               \
		    a_1 = (5.0f * (psrc_beg[hx_1 + wx + l] +               \
			    psrc_beg[hx_1 + wx1 + l]) - psrc_beg[hx_1 +    \
			    wx_1 + l] - psrc_beg[hx_1 + wx2 + l]);         \
		    a = (5.0f * (psrc_beg[hx + wx + l] + psrc_beg[hx +     \
			    wx1 + l]) - psrc_beg[hx + wx_1 + l] -          \
			    psrc_beg[hx + wx2 + l]);                       \
		    a1 = (5.0f * (psrc_beg[hx1 + wx + l] +                 \
			    psrc_beg[hx1 + wx1 + l]) - psrc_beg[hx1 +      \
			    wx_1 + l] - psrc_beg[hx1 + wx2 + l]);          \
		    p_1 = psrc_beg + hx2 + wx_1 + l;                       \
		    p = psrc_beg + hx2 + wx + l;                           \
		    p1 = psrc_beg + hx2 + wx1 + l;                         \
		    p2 = psrc_beg + hx2 + wx2 + l;                         \
		    i = (j);                                               \
		    pd = pdst_near + i * dst_stride + (beg_i) * chan +     \
			    l;                                             \
		    if (dh == 0) {                                         \
			rez = a * 0.125f;                                  \
			*pd = rez;                                         \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    for (; i < (amount) - 2; i += 2) {                     \
			a2 = (5.0f * ((*p) + (*p1)) - (*p_1) - (*p2));     \
			rez1 = (5.0f * (a + a1) - a_1 - a2) * 0.015625f;   \
			*pd = rez1;                                        \
			rez = a1 * 0.125f;                                 \
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
			    a2 = (5.0f * (psrc_beg[wx + l +                \
				    (src_height_beg - 1) * src_stride] +   \
				    psrc_beg[wx1 + l + (src_height_beg -   \
				    1) * src_stride]) - psrc_beg[wx_1 +    \
				    l + (src_height_beg -                  \
				    1) * src_stride] - psrc_beg[wx2 +      \
				    l + (src_height_beg -                  \
				    1) * src_stride]);                     \
			else                                               \
			    a2 = (5.0f * ((*p) + (*p1)) - (*p_1) -         \
				    (*p2));                                \
			rez1 = (5.0f * (a + a1) - a_1 - a2) * 0.015625f;   \
			*pd = rez1;                                        \
			pd += dst_stride;                                  \
			i++;                                               \
		    }                                                      \
		    if (i < (amount)) {                                    \
			rez = a1 * 0.125f;                                 \
			*pd = rez;                                         \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

void
mlib_ImageZoomIn2X_F32_SE_BC2_FL0(
	MLIB_ZOOMIN2XDEFEDGE_BC(mlib_f32))
{
	const mlib_f32 *p_1, *p, *p1, *p2, *psrc_end;
	mlib_f32 *pdst_count, *pd;
	mlib_s32 ii, jj;

	pdst_count = pdst_near;
	ii = 0;
	jj = 0;

	while (((pdst - pdst_count) >= dst_stride) && (src_height_near > 0)) {
		ROW_BICUBIC2_FL0(ii, jj, channels, src_width_near);
		jj++;
		pdst_count += dst_stride;
		src_height_near--;
	}

	while (src_height_near > src_height) {
		ROW_BICUBIC2_FL0(ii, jj + src_height_near - 1, channels,
			src_width_near);
		src_height_near--;
	}

	if (src_height_near > 0) {
		while ((pdst - pdst_count > 0) && (src_width_near > 0)) {
			COLUMN_BICUBIC2_FL0(ii, jj, channels,
				(src_height_near + jj));
			src_width_near--;
			pdst_count += channels;
			ii++;
		}

		while (src_width_near > src_width) {
			COLUMN_BICUBIC2_FL0(ii + src_width_near - 1, jj,
				channels, (src_height_near + jj));
			src_width_near--;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIn2X_F32_SE_BC2_FL1(
	MLIB_ZOOMIN2XDEFEDGE_BC(mlib_f32))
{
	const mlib_f32 *p_1, *p, *p1, *p2, *psrc_end;
	mlib_f32 *pdst_count, *pd;
	mlib_s32 ii, jj;

	pdst_count = pdst_near;
	ii = 0;
	jj = 0;

	while (((pdst - pdst_count) >= dst_stride) && (src_height_near > 0)) {
		ROW_BICUBIC2_FL1(ii, jj, channels, src_width_near);
		jj++;
		pdst_count += dst_stride;
		src_height_near--;
	}

	while (src_height_near > src_height) {
		ROW_BICUBIC2_FL1(ii, jj + src_height_near - 1, channels,
			src_width_near);
		src_height_near--;
	}

	if (src_height_near > 0) {
		while ((pdst - pdst_count > 0) && (src_width_near > 0)) {
			COLUMN_BICUBIC2_FL1(ii, jj, channels,
				(src_height_near + jj));
			src_width_near--;
			pdst_count += channels;
			ii++;
		}

		while (src_width_near > src_width) {
			COLUMN_BICUBIC2_FL1(ii + src_width_near - 1, jj,
				channels, (src_height_near + jj));
			src_width_near--;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIn2X_F32_SE_BC2_FL2(
	MLIB_ZOOMIN2XDEFEDGE_BC(mlib_f32))
{
	const mlib_f32 *p_1, *p, *p1, *p2, *psrc_end;
	mlib_f32 *pdst_count, *pd;
	mlib_s32 ii, jj;

	pdst_count = pdst_near;
	ii = 0;
	jj = 0;

	while (((pdst - pdst_count) >= dst_stride) && (src_height_near > 0)) {
		ROW_BICUBIC2_FL2(ii, jj, channels, src_width_near);
		jj++;
		pdst_count += dst_stride;
		src_height_near--;
	}

	while (src_height_near > src_height) {
		ROW_BICUBIC2_FL2(ii, jj + src_height_near - 1, channels,
			src_width_near);
		src_height_near--;
	}

	if (src_height_near > 0) {
		while ((pdst - pdst_count > 0) && (src_width_near > 0)) {
			COLUMN_BICUBIC2_FL2(ii, jj, channels,
				(src_height_near + jj));
			src_width_near--;
			pdst_count += channels;
			ii++;
		}

		while (src_width_near > src_width) {
			COLUMN_BICUBIC2_FL2(ii + src_width_near - 1, jj,
				channels, (src_height_near + jj));
			src_width_near--;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIn2X_F32_SE_BC2_FL3(
	MLIB_ZOOMIN2XDEFEDGE_BC(mlib_f32))
{
	const mlib_f32 *p_1, *p, *p1, *p2;
	mlib_f32 *pdst_count, *pd;
	mlib_s32 ii, jj;

	pdst_count = pdst_near;
	ii = 0;
	jj = 0;

	while (((pdst - pdst_count) >= dst_stride) && (src_height_near > 0)) {
		ROW_BICUBIC2_FL3(ii, jj, channels, src_width_near);
		jj++;
		pdst_count += dst_stride;
		src_height_near--;
	}

	while (src_height_near > src_height) {
		ROW_BICUBIC2_FL3(ii, jj + src_height_near - 1, channels,
			src_width_near);
		src_height_near--;
	}

	if (src_height_near > 0) {
		while ((pdst - pdst_count > 0) && (src_width_near > 0)) {
			COLUMN_BICUBIC2_FL3(ii, jj, channels,
				(src_height_near + jj));
			src_width_near--;
			pdst_count += channels;
			ii++;
		}

		while (src_width_near > src_width) {
			COLUMN_BICUBIC2_FL3(ii + src_width_near - 1, jj,
				channels, (src_height_near + jj));
			src_width_near--;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIn2X_D64_SE_BL_FL0(
	MLIB_ZOOMIN2XDEFEDGE(mlib_d64))
{
	const mlib_d64 *psrc_low, *psrc_right, *psrc_left, *tpsrc;
	mlib_d64 *tpdst;
	mlib_d64 tmp1, tmp2;
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
mlib_ImageZoomIn2X_D64_SE_BL_FL1(
	MLIB_ZOOMIN2XDEFEDGE(mlib_d64))
{
	const mlib_d64 *psrc_low, *psrc_right, *psrc_left, *tpsrc;
	mlib_d64 *tpdst;
	mlib_d64 tmp1, tmp2;
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
mlib_ImageZoomIn2X_D64_SE_BL_FL2(
	MLIB_ZOOMIN2XDEFEDGE(mlib_d64))
{
	const mlib_d64 *psrc_low, *psrc_right, *psrc_left, *tpsrc;
	mlib_d64 *tpdst;
	mlib_d64 tmp1, tmp2;
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
mlib_ImageZoomIn2X_D64_SE_BL_FL3(
	MLIB_ZOOMIN2XDEFEDGE(mlib_d64))
{
	const mlib_d64 *psrc_low, *psrc_right, *psrc_left, *tpsrc;
	mlib_d64 *tpdst;
	mlib_d64 tmp1, tmp2;
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

		if (((src_height_near - 1 + src_h_beg_near) & 1) == 0) {
			psrc_low -= src_stride;
		}

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

			if (((src_width_near - 1 + src_w_beg_near) & 1) == 0) {
				psrc_right -= channels;
			}

			src_width_near--;
		}
	}
}

/* *********************************************************** */

#undef PREPARE_BICUBIC

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
	mlib_d64 a_1, a, a1, a2 = 0, rez, rez1;                         \
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

void
mlib_ImageZoomIn2X_D64_SE_BC_FL0(
	MLIB_ZOOMIN2XDEFEDGE_BC(mlib_d64))
{
	const mlib_d64 *p_1, *p, *p1, *p2, *psrc_end;
	mlib_d64 *pdst_count, *pd;
	mlib_s32 ii, jj;

	pdst_count = pdst_near;
	ii = 0;
	jj = 0;

	while (((pdst - pdst_count) >= dst_stride) && (src_height_near > 0)) {
		ROW_BICUBIC_FL0(ii, jj, channels, src_width_near);
		jj++;
		pdst_count += dst_stride;
		src_height_near--;
	}

	while (src_height_near > src_height) {
		ROW_BICUBIC_FL0(ii, jj + src_height_near - 1, channels,
			src_width_near);
		src_height_near--;
	}

	if (src_height_near > 0) {
		while ((pdst - pdst_count > 0) && (src_width_near > 0)) {
			COLUMN_BICUBIC_FL0(ii, jj, channels,
				(src_height_near + jj));
			src_width_near--;
			pdst_count += channels;
			ii++;
		}

		while (src_width_near > src_width) {
			COLUMN_BICUBIC_FL0(ii + src_width_near - 1, jj,
				channels, (src_height_near + jj));
			src_width_near--;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIn2X_D64_SE_BC_FL1(
	MLIB_ZOOMIN2XDEFEDGE_BC(mlib_d64))
{
	const mlib_d64 *p_1, *p, *p1, *p2, *psrc_end;
	mlib_d64 *pdst_count, *pd;
	mlib_s32 ii, jj;

	pdst_count = pdst_near;
	ii = 0;
	jj = 0;

	while (((pdst - pdst_count) >= dst_stride) && (src_height_near > 0)) {
		ROW_BICUBIC_FL1(ii, jj, channels, src_width_near);
		jj++;
		pdst_count += dst_stride;
		src_height_near--;
	}

	while (src_height_near > src_height) {
		ROW_BICUBIC_FL1(ii, jj + src_height_near - 1, channels,
			src_width_near);
		src_height_near--;
	}

	if (src_height_near > 0) {
		while ((pdst - pdst_count > 0) && (src_width_near > 0)) {
			COLUMN_BICUBIC_FL1(ii, jj, channels,
				(src_height_near + jj));
			src_width_near--;
			pdst_count += channels;
			ii++;
		}

		while (src_width_near > src_width) {
			COLUMN_BICUBIC_FL1(ii + src_width_near - 1, jj,
				channels, (src_height_near + jj));
			src_width_near--;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIn2X_D64_SE_BC_FL2(
	MLIB_ZOOMIN2XDEFEDGE_BC(mlib_d64))
{
	const mlib_d64 *p_1, *p, *p1, *p2, *psrc_end;
	mlib_d64 *pdst_count, *pd;
	mlib_s32 ii, jj;

	pdst_count = pdst_near;
	ii = 0;
	jj = 0;

	while (((pdst - pdst_count) >= dst_stride) && (src_height_near > 0)) {
		ROW_BICUBIC_FL2(ii, jj, channels, src_width_near);
		jj++;
		pdst_count += dst_stride;
		src_height_near--;
	}

	while (src_height_near > src_height) {
		ROW_BICUBIC_FL2(ii, jj + src_height_near - 1, channels,
			src_width_near);
		src_height_near--;
	}

	if (src_height_near > 0) {
		while ((pdst - pdst_count > 0) && (src_width_near > 0)) {
			COLUMN_BICUBIC_FL2(ii, jj, channels,
				(src_height_near + jj));
			src_width_near--;
			pdst_count += channels;
			ii++;
		}

		while (src_width_near > src_width) {
			COLUMN_BICUBIC_FL2(ii + src_width_near - 1, jj,
				channels, (src_height_near + jj));
			src_width_near--;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIn2X_D64_SE_BC_FL3(
	MLIB_ZOOMIN2XDEFEDGE_BC(mlib_d64))
{
	const mlib_d64 *p_1, *p, *p1, *p2;
	mlib_d64 *pdst_count, *pd;
	mlib_s32 ii, jj;

	pdst_count = pdst_near;
	ii = 0;
	jj = 0;

	while (((pdst - pdst_count) >= dst_stride) && (src_height_near > 0)) {
		ROW_BICUBIC_FL3(ii, jj, channels, src_width_near);
		jj++;
		pdst_count += dst_stride;
		src_height_near--;
	}

	while (src_height_near > src_height) {
		ROW_BICUBIC_FL3(ii, jj + src_height_near - 1, channels,
			src_width_near);
		src_height_near--;
	}

	if (src_height_near > 0) {
		while ((pdst - pdst_count > 0) && (src_width_near > 0)) {
			COLUMN_BICUBIC_FL3(ii, jj, channels,
				(src_height_near + jj));
			src_width_near--;
			pdst_count += channels;
			ii++;
		}

		while (src_width_near > src_width) {
			COLUMN_BICUBIC_FL3(ii + src_width_near - 1, jj,
				channels, (src_height_near + jj));
			src_width_near--;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIn2X_D64_SE_BC2_FL0(
	MLIB_ZOOMIN2XDEFEDGE_BC(mlib_d64))
{
	const mlib_d64 *p_1, *p, *p1, *p2, *psrc_end;
	mlib_d64 *pdst_count, *pd;
	mlib_s32 ii, jj;

	pdst_count = pdst_near;
	ii = 0;
	jj = 0;

	while (((pdst - pdst_count) >= dst_stride) && (src_height_near > 0)) {
		ROW_BICUBIC2_FL0(ii, jj, channels, src_width_near);
		jj++;
		pdst_count += dst_stride;
		src_height_near--;
	}

	while (src_height_near > src_height) {
		ROW_BICUBIC2_FL0(ii, jj + src_height_near - 1, channels,
			src_width_near);
		src_height_near--;
	}

	if (src_height_near > 0) {
		while ((pdst - pdst_count > 0) && (src_width_near > 0)) {
			COLUMN_BICUBIC2_FL0(ii, jj, channels,
				(src_height_near + jj));
			src_width_near--;
			pdst_count += channels;
			ii++;
		}

		while (src_width_near > src_width) {
			COLUMN_BICUBIC2_FL0(ii + src_width_near - 1, jj,
				channels, (src_height_near + jj));
			src_width_near--;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIn2X_D64_SE_BC2_FL1(
	MLIB_ZOOMIN2XDEFEDGE_BC(mlib_d64))
{
	const mlib_d64 *p_1, *p, *p1, *p2, *psrc_end;
	mlib_d64 *pdst_count, *pd;
	mlib_s32 ii, jj;

	pdst_count = pdst_near;
	ii = 0;
	jj = 0;

	while (((pdst - pdst_count) >= dst_stride) && (src_height_near > 0)) {
		ROW_BICUBIC2_FL1(ii, jj, channels, src_width_near);
		jj++;
		pdst_count += dst_stride;
		src_height_near--;
	}

	while (src_height_near > src_height) {
		ROW_BICUBIC2_FL1(ii, jj + src_height_near - 1, channels,
			src_width_near);
		src_height_near--;
	}

	if (src_height_near > 0) {
		while ((pdst - pdst_count > 0) && (src_width_near > 0)) {
			COLUMN_BICUBIC2_FL1(ii, jj, channels,
				(src_height_near + jj));
			src_width_near--;
			pdst_count += channels;
			ii++;
		}

		while (src_width_near > src_width) {
			COLUMN_BICUBIC2_FL1(ii + src_width_near - 1, jj,
				channels, (src_height_near + jj));
			src_width_near--;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIn2X_D64_SE_BC2_FL2(
	MLIB_ZOOMIN2XDEFEDGE_BC(mlib_d64))
{
	const mlib_d64 *p_1, *p, *p1, *p2, *psrc_end;
	mlib_d64 *pdst_count, *pd;
	mlib_s32 ii, jj;

	pdst_count = pdst_near;
	ii = 0;
	jj = 0;

	while (((pdst - pdst_count) >= dst_stride) && (src_height_near > 0)) {
		ROW_BICUBIC2_FL2(ii, jj, channels, src_width_near);
		jj++;
		pdst_count += dst_stride;
		src_height_near--;
	}

	while (src_height_near > src_height) {
		ROW_BICUBIC2_FL2(ii, jj + src_height_near - 1, channels,
			src_width_near);
		src_height_near--;
	}

	if (src_height_near > 0) {
		while ((pdst - pdst_count > 0) && (src_width_near > 0)) {
			COLUMN_BICUBIC2_FL2(ii, jj, channels,
				(src_height_near + jj));
			src_width_near--;
			pdst_count += channels;
			ii++;
		}

		while (src_width_near > src_width) {
			COLUMN_BICUBIC2_FL2(ii + src_width_near - 1, jj,
				channels, (src_height_near + jj));
			src_width_near--;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIn2X_D64_SE_BC2_FL3(
	MLIB_ZOOMIN2XDEFEDGE_BC(mlib_d64))
{
	const mlib_d64 *p_1, *p, *p1, *p2;
	mlib_d64 *pdst_count, *pd;
	mlib_s32 ii, jj;

	pdst_count = pdst_near;
	ii = 0;
	jj = 0;

	while (((pdst - pdst_count) >= dst_stride) && (src_height_near > 0)) {
		ROW_BICUBIC2_FL3(ii, jj, channels, src_width_near);
		jj++;
		pdst_count += dst_stride;
		src_height_near--;
	}

	while (src_height_near > src_height) {
		ROW_BICUBIC2_FL3(ii, jj + src_height_near - 1, channels,
			src_width_near);
		src_height_near--;
	}

	if (src_height_near > 0) {
		while ((pdst - pdst_count > 0) && (src_width_near > 0)) {
			COLUMN_BICUBIC2_FL3(ii, jj, channels,
				(src_height_near + jj));
			src_width_near--;
			pdst_count += channels;
			ii++;
		}

		while (src_width_near > src_width) {
			COLUMN_BICUBIC2_FL3(ii + src_width_near - 1, jj,
				channels, (src_height_near + jj));
			src_width_near--;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIn2X_D64_CE(
	MLIB_ZOOMIN2XDEFEDGE(mlib_d64))
{
	mlib_s32 i, j;

/* upper bound of image */
	while (((pdst - pdst_near) >= dst_stride) && (src_height_near > 0)) {
		for (i = 0; i < src_width_near * channels; i++)
			pdst_near[i] = 0;
		pdst_near += dst_stride;
		src_height_near--;
	}

/* lower bound of image */
	while (src_height_near > src_height) {
		for (i = 0; i < src_width_near * channels; i++)
			pdst_near[i + (src_height_near - 1) * dst_stride] = 0;
		src_height_near--;
	}

/* left bound of image */
	while ((pdst - pdst_near > 0) && (src_width_near > 0)) {
		for (i = 0; i < channels; i++) {
			for (j = 0; j < src_height_near; j++)
				pdst_near[i + j * dst_stride] = 0;
		}

		src_width_near--;
		pdst_near += channels;
	}

/* right bound of image */
	while (src_width_near > src_width) {
		for (i = 0; i < channels; i++) {
			for (j = 0; j < src_height_near; j++)
				pdst_near[i + (src_width_near - 1) * channels +
					j * dst_stride] = 0;
		}

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
mlib_ImageZoomIn2X_D64_ON(
	MLIB_ZOOMIN2XDEFEDGE(mlib_d64))
{
	const mlib_d64 *psrc_low, *psrc_right, *psrc_left, *tpsrc;
	mlib_d64 *tpdst;
	mlib_s32 i, j;

/* upper bound of image */
	while (((pdst - pdst_near) >= dst_stride) && (src_height_near > 0)) {
		for (i = 0; i < channels; i++) {
			tpdst = pdst_near + i;
			tpsrc = psrc_near + i;
			ROW_COLUMN_NN(src_w_beg_near, src_width_near, channels,
				channels);
		}

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
		for (i = 0; i < channels; i++) {
			tpdst = pdst_near + i + (src_height_near -
				1) * dst_stride;
			tpsrc = psrc_low + i;
			ROW_COLUMN_NN(src_w_beg_near, src_width_near, channels,
				channels);
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
				ROW_COLUMN_NN(src_h_beg_near, src_height_near,
					dst_stride, src_stride);
			}

			if ((src_w_beg_near & 1) != 0)
				psrc_left += channels;
			src_width_near--;
			pdst_near += channels;
			src_w_beg_near = (src_w_beg_near + 1);
		}

		psrc_right =
			psrc_left + ((src_width_near - 1 +
			(src_w_beg_near & 1)) >> 1) * channels;

/* right bound of image */
		while (src_width_near > src_width) {
			for (i = 0; i < channels; i++) {
				tpdst = pdst_near + i + (src_width_near -
					1) * channels;
				tpsrc = psrc_right + i;
				ROW_COLUMN_NN(src_h_beg_near, src_height_near,
					dst_stride, src_stride);
			}

			if (((src_width_near - 1 + src_w_beg_near) & 1) == 0)
				psrc_right -= channels;
			src_width_near--;
		}
	}
}

/* *********************************************************** */
