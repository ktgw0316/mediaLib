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

#pragma ident	"@(#)mlib_c_ImageZoomIn2XIndex.c	9.2	07/10/09 SMI"

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
 *
 * RESTRICTION
 *      src and dst must be the same type and have just one channel.
 *      They can be in MLIB_BYTE or MLIB_SHORT data type.
 *
 *      src image can not have width or height larger than 32767.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageZoomIn2XIndex.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageZoomIn2XIndex = __mlib_ImageZoomIn2XIndex

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageZoomIn2XIndex) mlib_ImageZoomIn2XIndex
    __attribute__((weak, alias("__mlib_ImageZoomIn2XIndex")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_ZOOMIN2XINDEXCALLPARAM_NN(MLIB_TYPE)                     \
	(MLIB_TYPE *)pdst, (MLIB_TYPE *)psrc, src_width,              \
	src_height, dst_stride,  src_stride,                          \
	src_w_beg, src_h_beg

/* *********************************************************** */

#define	MLIB_ZOOMIN2XINDEXCALLPARAM(MLIB_TYPE)                  \
	MLIB_ZOOMIN2XINDEXCALLPARAM_NN(MLIB_TYPE), colormap

/* *********************************************************** */

#define	MLIB_ZOOMIN2XINDEXCALLEDGE_CE(MLIB_TYPE)                      \
	(MLIB_TYPE *)pdst, src_width, src_height,                     \
	dst_stride, (MLIB_TYPE *)pdst_near,                           \
	src_width_near, src_height_near, colormap

/* *********************************************************** */

#define	MLIB_ZOOMIN2XINDEXCALLEDGE_NN(MLIB_TYPE)                      \
	(MLIB_TYPE *)pdst, src_width, src_height,                     \
	dst_stride, src_stride, (MLIB_TYPE *)pdst_near,               \
	(MLIB_TYPE *)psrc_near, src_width_near, src_height_near,      \
	src_w_beg_near, src_h_beg_near

/* *********************************************************** */

#define	MLIB_ZOOMIN2XINDEXCALLEDGE_BL(MLIB_TYPE)                \
	MLIB_ZOOMIN2XINDEXCALLEDGE_NN(MLIB_TYPE), colormap

/* *********************************************************** */

#define	MLIB_ZOOMIN2XINDEXCALLEDGE_BC(MLIB_TYPE)                      \
	MLIB_ZOOMIN2XINDEXCALLEDGE_CE(MLIB_TYPE),                     \
	src_stride, (MLIB_TYPE *)psrc_beg, src_width_beg,             \
	src_height_beg, src_w_beg_near, src_h_beg_near

/* *********************************************************** */

#define	SIZE_NN(chan)                                                  \
	pdst_near = pdst;                                              \
	psrc_near = psrc;                                              \
	if ((2 * src_height) > dst_height) {                           \
	    src_h_beg_near = (2 * src_height - dst_height + 1) >> 1;   \
	    src_height_near = dst_height;                              \
	} else {                                                       \
	    pdst_near +=                                               \
		((dst_height - 2 * src_height) >> 1) * dst_stride;     \
	    src_h_beg_near = 0;                                        \
	    src_height_near = 2 * src_height;                          \
	}                                                              \
	                                                               \
	if ((2 * src_width) > dst_width) {                             \
	    src_w_beg_near = (2 * src_width - dst_width + 1) >> 1;     \
	    src_width_near = dst_width;                                \
	} else {                                                       \
	    pdst_near += ((dst_width - 2 * src_width) >> 1) * chan;    \
	    src_w_beg_near = 0;                                        \
	    src_width_near = 2 * src_width;                            \
	}                                                              \
	                                                               \
	psrc_near +=                                                   \
	    (src_h_beg_near >> 1) * src_stride +                       \
	    (src_w_beg_near >> 1) * chan

/* *********************************************************** */

#define	SIZE_BL(chan)                                                      \
	if (src_height >= (dst_height + 2) >> 1) {                         \
	    src_h_beg = src_height - ((dst_height + 2) >> 1);              \
	    if (src_height == (dst_height + 1) >> 1)                       \
		src_height = dst_height - 1;                               \
	    else                                                           \
		src_height = dst_height;                                   \
	} else {                                                           \
	    pdst += (((dst_height + 2) >> 1) - src_height) * dst_stride;   \
	    src_h_beg = 0;                                                 \
	    src_height = 2 * src_height - 2;                               \
	}                                                                  \
	                                                                   \
	if (src_width >= (dst_width + 2) >> 1) {                           \
	    src_w_beg = src_width - ((dst_width + 2) >> 1);                \
	    if (src_width == (dst_width + 1) >> 1)                         \
		src_width = dst_width - 1;                                 \
	    else                                                           \
		src_width = dst_width;                                     \
	} else {                                                           \
	    pdst += (((dst_width + 2) >> 1) - src_width) * chan;           \
	    src_w_beg = 0;                                                 \
	    src_width = 2 * src_width - 2;                                 \
	}                                                                  \
	                                                                   \
	psrc += (src_h_beg >> 1) * src_stride + (src_w_beg >> 1) * chan

/* *********************************************************** */

#define	SIZE_BC(chan)                                                      \
	if (src_height >= (((dst_height + 2) >> 1) + 2)) {                 \
	    src_h_beg = src_height - ((dst_height + 2) >> 1);              \
	    if (src_height == (((dst_height + 1) >> 1) + 2))               \
		src_height = dst_height - 1;                               \
	    else                                                           \
		src_height = dst_height;                                   \
	} else {                                                           \
	    pdst +=                                                        \
		(((dst_height + 2) >> 1) - src_height + 2) * dst_stride;   \
	    src_h_beg = 2;                                                 \
	    src_height = 2 * src_height - 6;                               \
	}                                                                  \
	                                                                   \
	if (src_width >= (((dst_width + 2) >> 1) + 2)) {                   \
	    src_w_beg = src_width - ((dst_width + 2) >> 1);                \
	    if (src_width == (((dst_width + 1) >> 1) + 2))                 \
		src_width = dst_width - 1;                                 \
	    else                                                           \
		src_width = dst_width;                                     \
	} else {                                                           \
	    pdst += (((dst_width + 2) >> 1) - src_width + 2) * chan;       \
	    src_w_beg = 2;                                                 \
	    src_width = 2 * src_width - 6;                                 \
	}                                                                  \
	                                                                   \
	psrc += (src_h_beg >> 1) * src_stride + (src_w_beg >> 1) * chan

/* *********************************************************** */

#define	SIZE_SRC_PADDED(chan)                                           \
	{                                                               \
	    mlib_s32 minX, minY, maxX, maxY, deltaX, deltaY, right =    \
		1, bottom = 1;                                          \
	    mlib_u8 *paddings = mlib_ImageGetPaddings(src);             \
	                                                                \
	    minX = 2 * src_width_beg - src_width;                       \
	    minY = 2 * src_height_beg - src_height;                     \
	    maxX = 2 * src_width_beg + src_width;                       \
	    maxY = 2 * src_height_beg + src_height;                     \
	    if (minX < 4 * paddings[0]) {                               \
		deltaX = 4 * paddings[0] - minX;                        \
		if (filter == MLIB_NEAREST) {                           \
		    psrc +=                                             \
			((deltaX >> 2) + ((deltaX & 2) >> 1)) * chan;   \
		    src_w_beg = 0;                                      \
		} else {                                                \
		    psrc += (deltaX >> 2) * chan;                       \
		    src_w_beg = 1;                                      \
		}                                                       \
		pdst += (deltaX >> 1) * chan;                           \
		minX = 4 * paddings[0];                                 \
	    }                                                           \
	    if (minY < 4 * paddings[1]) {                               \
		deltaY = 4 * paddings[1] - minY;                        \
		if (filter == MLIB_NEAREST) {                           \
		    psrc +=                                             \
			((deltaY >> 2) +                                \
			((deltaY & 2) >> 1)) * src_stride;              \
		    src_h_beg = 0;                                      \
		} else {                                                \
		    psrc += (deltaY >> 2) * src_stride;                 \
		    src_h_beg = 1;                                      \
		}                                                       \
		pdst += (deltaY >> 1) * dst_stride;                     \
		minY = 4 * paddings[1];                                 \
	    }                                                           \
	    if (maxX >= 4 * (src_width_beg - paddings[2])) {            \
		maxX = 4 * (src_width_beg - paddings[2]);               \
		right = 0;                                              \
	    }                                                           \
	    if (maxY >= 4 * (src_height_beg - paddings[3])) {           \
		maxY = 4 * (src_height_beg - paddings[3]);              \
		bottom = 0;                                             \
	    }                                                           \
	    src_width = (maxX - minX + right) / 2;                      \
	    src_height = (maxY - minY + bottom) / 2;                    \
	}

/* *********************************************************** */

mlib_status
__mlib_ImageZoomIn2XIndex(
    mlib_image *dst,
    const mlib_image *src,
    mlib_filter filter,
    mlib_edge edge,
    const void *colormap)
{
	mlib_u8 *pdst, *psrc, *pdst_near, *psrc_near, *psrc_beg = NULL;
	mlib_s32 src_height, dst_height, src_width, dst_width, src_stride,
	    dst_stride;
	mlib_s32 type, channels;
	mlib_s32 src_w_beg, src_h_beg, flag = 0;
	mlib_s32 src_width_near, src_height_near, src_w_beg_near,
	    src_h_beg_near;
	mlib_s32 src_width_beg, src_height_beg;
	mlib_status res;

/*  check for obvious errors  */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_HAVE_CHAN(src, 1);
	MLIB_IMAGE_HAVE_CHAN(dst, 1);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);

	pdst = (mlib_u8 *)mlib_ImageGetData(dst);
	psrc = (mlib_u8 *)mlib_ImageGetData(src);
/* height of destination image */
	dst_height = mlib_ImageGetHeight(dst);
/* height of source image */
	src_height = mlib_ImageGetHeight(src);
/* width of destination image */
	dst_width = mlib_ImageGetWidth(dst);
/* width of source image */
	src_width = mlib_ImageGetWidth(src);
/* bytes to next row */
	dst_stride = mlib_ImageGetStride(dst);
/* bytes to next row */
	src_stride = mlib_ImageGetStride(src);
/* channels in destination image */
	channels = mlib_ImageGetChannels(dst);
/* type of destination image */
	type = mlib_ImageGetType(dst);

	if (type != MLIB_BYTE && type != MLIB_SHORT)
		return (MLIB_FAILURE);

	if (type == MLIB_SHORT)
		channels *= 2;

	SIZE_NN(channels);

	src_width_beg = src_width;
	src_height_beg = src_height;

	if (filter == MLIB_NEAREST) {
		pdst = pdst_near;
		psrc = psrc_near;
		src_width = src_width_near;
		src_height = src_height_near;
		src_w_beg = src_w_beg_near;
		src_h_beg = src_h_beg_near;
	} else {
		flag = (dst_height & 1) | ((dst_width & 1) << 1);
		psrc_beg = psrc;

		if (filter == MLIB_BILINEAR) {
			SIZE_BL(channels);
		} else {
			SIZE_BC(channels);
		}
	}

	if (edge == MLIB_EDGE_SRC_PADDED) {
		SIZE_SRC_PADDED(channels);
	}

	if (src_width < 0)
		src_width = 0;

	if (src_height < 0)
		src_height = 0;

	if (type == MLIB_SHORT) {
		channels >>= 1;
		dst_stride >>= 1;
		src_stride >>= 1;
	}

	res = MLIB_SUCCESS;

	if ((src_width > 0) && (src_height > 0))
		switch (type) {
/* handle MLIB_BYTE data type of image */
		case MLIB_BYTE:
			switch (filter) {
			case MLIB_NEAREST:
				res =
				    __mlib_ImageZoomIn2X(dst, src, filter,
				    edge);
				break;
			case MLIB_BILINEAR:
				switch (flag) {
				case 0:
					res =
					    mlib_c_ImageZoomIn2XIndex_U8_BL_FL0
					    (MLIB_ZOOMIN2XINDEXCALLPARAM
					    (mlib_u8));
					break;
				case 1:
					res =
					    mlib_c_ImageZoomIn2XIndex_U8_BL_FL1
					    (MLIB_ZOOMIN2XINDEXCALLPARAM
					    (mlib_u8));
					break;
				case 2:
					res =
					    mlib_c_ImageZoomIn2XIndex_U8_BL_FL2
					    (MLIB_ZOOMIN2XINDEXCALLPARAM
					    (mlib_u8));
					break;
				case 3:
					res =
					    mlib_c_ImageZoomIn2XIndex_U8_BL_FL3
					    (MLIB_ZOOMIN2XINDEXCALLPARAM
					    (mlib_u8));
					break;
				}

				break;
			case MLIB_BICUBIC:
				switch (flag) {
				case 0:
					res =
					    mlib_c_ImageZoomIn2XIndex_U8_BC_FL0
					    (MLIB_ZOOMIN2XINDEXCALLPARAM
					    (mlib_u8));
					break;
				case 1:
					res =
					    mlib_c_ImageZoomIn2XIndex_U8_BC_FL1
					    (MLIB_ZOOMIN2XINDEXCALLPARAM
					    (mlib_u8));
					break;
				case 2:
					res =
					    mlib_c_ImageZoomIn2XIndex_U8_BC_FL2
					    (MLIB_ZOOMIN2XINDEXCALLPARAM
					    (mlib_u8));
					break;
				case 3:
					res =
					    mlib_c_ImageZoomIn2XIndex_U8_BC_FL3
					    (MLIB_ZOOMIN2XINDEXCALLPARAM
					    (mlib_u8));
					break;
				}

				break;
			case MLIB_BICUBIC2:
				switch (flag) {
				case 0:
					res =
					    mlib_c_ImageZoomIn2XIndex_U8_BC2_FL0
					    (MLIB_ZOOMIN2XINDEXCALLPARAM
					    (mlib_u8));
					break;
				case 1:
					res =
					    mlib_c_ImageZoomIn2XIndex_U8_BC2_FL1
					    (MLIB_ZOOMIN2XINDEXCALLPARAM
					    (mlib_u8));
					break;
				case 2:
					res =
					    mlib_c_ImageZoomIn2XIndex_U8_BC2_FL2
					    (MLIB_ZOOMIN2XINDEXCALLPARAM
					    (mlib_u8));
					break;
				case 3:
					res =
					    mlib_c_ImageZoomIn2XIndex_U8_BC2_FL3
					    (MLIB_ZOOMIN2XINDEXCALLPARAM
					    (mlib_u8));
					break;
				}

				break;
			}

			break;
/* handle MLIB_SHORT data type of image */
		case MLIB_SHORT:
			switch (filter) {
			case MLIB_NEAREST:
				res =
				    __mlib_ImageZoomIn2X(dst, src, filter,
				    edge);
				break;
			case MLIB_BILINEAR:
				switch (flag) {
				case 0:
					res =
					    mlib_c_ImageZoomIn2XIndex_S16_BL_FL0
					    (MLIB_ZOOMIN2XINDEXCALLPARAM
					    (mlib_s16));
					break;
				case 1:
					res =
					    mlib_c_ImageZoomIn2XIndex_S16_BL_FL1
					    (MLIB_ZOOMIN2XINDEXCALLPARAM
					    (mlib_s16));
					break;
				case 2:
					res =
					    mlib_c_ImageZoomIn2XIndex_S16_BL_FL2
					    (MLIB_ZOOMIN2XINDEXCALLPARAM
					    (mlib_s16));
					break;
				case 3:
					res =
					    mlib_c_ImageZoomIn2XIndex_S16_BL_FL3
					    (MLIB_ZOOMIN2XINDEXCALLPARAM
					    (mlib_s16));
					break;
				}

				break;
			case MLIB_BICUBIC:
				switch (flag) {
				case 0:
					res =
					    mlib_c_ImageZoomIn2XIndex_S16_BC_FL0
					    (MLIB_ZOOMIN2XINDEXCALLPARAM
					    (mlib_s16));
					break;
				case 1:
					res =
					    mlib_c_ImageZoomIn2XIndex_S16_BC_FL1
					    (MLIB_ZOOMIN2XINDEXCALLPARAM
					    (mlib_s16));
					break;
				case 2:
					res =
					    mlib_c_ImageZoomIn2XIndex_S16_BC_FL2
					    (MLIB_ZOOMIN2XINDEXCALLPARAM
					    (mlib_s16));
					break;
				case 3:
					res =
					    mlib_c_ImageZoomIn2XIndex_S16_BC_FL3
					    (MLIB_ZOOMIN2XINDEXCALLPARAM
					    (mlib_s16));
					break;
				}

				break;
			case MLIB_BICUBIC2:
				switch (flag) {
				case 0:
				    res =
					mlib_c_ImageZoomIn2XIndex_S16_BC2_FL0
					(MLIB_ZOOMIN2XINDEXCALLPARAM
					(mlib_s16));
					break;
				case 1:
				    res =
					mlib_c_ImageZoomIn2XIndex_S16_BC2_FL1
					(MLIB_ZOOMIN2XINDEXCALLPARAM
					(mlib_s16));
					break;
				case 2:
				    res =
					mlib_c_ImageZoomIn2XIndex_S16_BC2_FL2
					(MLIB_ZOOMIN2XINDEXCALLPARAM
					(mlib_s16));
					break;
				case 3:
				    res =
					mlib_c_ImageZoomIn2XIndex_S16_BC2_FL3
					(MLIB_ZOOMIN2XINDEXCALLPARAM
					(mlib_s16));
					break;
				}

				break;
			}

			break;
		}

	if ((filter == MLIB_NEAREST) || (edge == MLIB_EDGE_DST_NO_WRITE) ||
	    (edge == MLIB_EDGE_SRC_PADDED) || (src_width_near <= 0) ||
	    (src_height_near <= 0) || (res != MLIB_SUCCESS)) {
		return (res);
	}
	switch (type) {
	case MLIB_BYTE:
		switch (edge) {
		case MLIB_EDGE_DST_FILL_ZERO:
			mlib_ImageZoomIn2XIndex_U8_CE
			    (MLIB_ZOOMIN2XINDEXCALLEDGE_CE(mlib_u8));
			break;
		case MLIB_EDGE_OP_NEAREST:
			mlib_ImageZoomIn2XIndex_U8_ON
			    (MLIB_ZOOMIN2XINDEXCALLEDGE_NN(mlib_u8));
			break;
		case MLIB_EDGE_SRC_EXTEND:
			switch (filter) {
			case MLIB_BILINEAR:
				switch (flag) {
				case 0:
					res =
					    mlib_ImageZoomIn2XIndex_U8_SE_BL_FL0
					    (MLIB_ZOOMIN2XINDEXCALLEDGE_BL
					    (mlib_u8));
					break;
				case 1:
					res =
					    mlib_ImageZoomIn2XIndex_U8_SE_BL_FL1
					    (MLIB_ZOOMIN2XINDEXCALLEDGE_BL
					    (mlib_u8));
					break;
				case 2:
					res =
					    mlib_ImageZoomIn2XIndex_U8_SE_BL_FL2
					    (MLIB_ZOOMIN2XINDEXCALLEDGE_BL
					    (mlib_u8));
					break;
				case 3:
					res =
					    mlib_ImageZoomIn2XIndex_U8_SE_BL_FL3
					    (MLIB_ZOOMIN2XINDEXCALLEDGE_BL
					    (mlib_u8));
					break;
				}

				break;
			case MLIB_BICUBIC:
				switch (flag) {
				case 0:
					res =
					    mlib_ImageZoomIn2XIndex_U8_SE_BC_FL0
					    (MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					    (mlib_u8));
					break;
				case 1:
					res =
					    mlib_ImageZoomIn2XIndex_U8_SE_BC_FL1
					    (MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					    (mlib_u8));
					break;
				case 2:
					res =
					    mlib_ImageZoomIn2XIndex_U8_SE_BC_FL2
					    (MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					    (mlib_u8));
					break;
				case 3:
					res =
					    mlib_ImageZoomIn2XIndex_U8_SE_BC_FL3
					    (MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					    (mlib_u8));
					break;
				}

				break;
			case MLIB_BICUBIC2:
				switch (flag) {
				case 0:
				    res =
					mlib_ImageZoomIn2XIndex_U8_SE_BC2_FL0
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					(mlib_u8));
					break;
				case 1:
				    res =
					mlib_ImageZoomIn2XIndex_U8_SE_BC2_FL1
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					(mlib_u8));
					break;
				case 2:
				    res =
					mlib_ImageZoomIn2XIndex_U8_SE_BC2_FL2
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					(mlib_u8));
					break;
				case 3:
				    res =
					mlib_ImageZoomIn2XIndex_U8_SE_BC2_FL3
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					(mlib_u8));
					break;
				}

				break;
			default :
				return (MLIB_FAILURE);
			}
			break;
		default :
			return (MLIB_FAILURE);
		}

		break;
	case MLIB_SHORT:
		switch (edge) {
		case MLIB_EDGE_DST_FILL_ZERO:
			mlib_ImageZoomIn2XIndex_S16_CE
			    (MLIB_ZOOMIN2XINDEXCALLEDGE_CE(mlib_s16));
			break;
		case MLIB_EDGE_OP_NEAREST:
			mlib_ImageZoomIn2XIndex_S16_ON
			    (MLIB_ZOOMIN2XINDEXCALLEDGE_NN(mlib_s16));
			break;
		case MLIB_EDGE_SRC_EXTEND:
			switch (filter) {
			case MLIB_BILINEAR:
				switch (flag) {
				case 0:
				    res =
					mlib_ImageZoomIn2XIndex_S16_SE_BL_FL0
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BL
					(mlib_s16));
					break;
				case 1:
				    res =
					mlib_ImageZoomIn2XIndex_S16_SE_BL_FL1
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BL
					(mlib_s16));
					break;
				case 2:
				    res =
					mlib_ImageZoomIn2XIndex_S16_SE_BL_FL2
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BL
					(mlib_s16));
					break;
				case 3:
				    res =
					mlib_ImageZoomIn2XIndex_S16_SE_BL_FL3
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BL
					(mlib_s16));
					break;
				}

				break;
			case MLIB_BICUBIC:
				switch (flag) {
				case 0:
				    res =
					mlib_ImageZoomIn2XIndex_S16_SE_BC_FL0
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					(mlib_s16));
					break;
				case 1:
				    res =
					mlib_ImageZoomIn2XIndex_S16_SE_BC_FL1
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					(mlib_s16));
					break;
				case 2:
				    res =
					mlib_ImageZoomIn2XIndex_S16_SE_BC_FL2
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					(mlib_s16));
					break;
				case 3:
				    res =
					mlib_ImageZoomIn2XIndex_S16_SE_BC_FL3
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					(mlib_s16));
					break;
				}

				break;
			case MLIB_BICUBIC2:
				switch (flag) {
				case 0:
				    res =
					mlib_ImageZoomIn2XIndex_S16_SE_BC2_FL0
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					(mlib_s16));
					break;
				case 1:
				    res =
					mlib_ImageZoomIn2XIndex_S16_SE_BC2_FL1
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					(mlib_s16));
					break;
				case 2:
				    res =
					mlib_ImageZoomIn2XIndex_S16_SE_BC2_FL2
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					(mlib_s16));
					break;
				case 3:
				    res =
					mlib_ImageZoomIn2XIndex_S16_SE_BC2_FL3
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					(mlib_s16));
					break;
				}

				break;
			default :
				return (MLIB_FAILURE);
			}
			break;
		default :
			return (MLIB_FAILURE);
		}

		break;
	default :
		return (MLIB_FAILURE);
	}

	return (res);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomIn2XIndex_Edge(
    mlib_image *dst,
    const mlib_image *src,
    mlib_filter filter,
    mlib_edge edge,
    const void *colormap)
{
	mlib_u8 *pdst, *psrc, *pdst_near, *psrc_near, *psrc_beg = NULL;
	mlib_s32 src_height, dst_height, src_width, dst_width, src_stride,
	    dst_stride;
	mlib_s32 type, channels;
	mlib_s32 src_w_beg, src_h_beg, flag = 0;
	mlib_s32 src_width_near, src_height_near, src_w_beg_near,
	    src_h_beg_near;
	mlib_s32 src_width_beg, src_height_beg;
	mlib_status res;

/*  check for obvious errors  */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_HAVE_CHAN(src, 1);
	MLIB_IMAGE_HAVE_CHAN(dst, 1);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);

	pdst = (mlib_u8 *)mlib_ImageGetData(dst);
	psrc = (mlib_u8 *)mlib_ImageGetData(src);
/* height of destination image */
	dst_height = mlib_ImageGetHeight(dst);
/* height of source image */
	src_height = mlib_ImageGetHeight(src);
/* width of destination image */
	dst_width = mlib_ImageGetWidth(dst);
/* width of source image */
	src_width = mlib_ImageGetWidth(src);
/* bytes to next row */
	dst_stride = mlib_ImageGetStride(dst);
/* bytes to next row */
	src_stride = mlib_ImageGetStride(src);
/* channels in destination image */
	channels = mlib_ImageGetChannels(dst);
/* type of destination image */
	type = mlib_ImageGetType(dst);

	if (type != MLIB_BYTE && type != MLIB_SHORT)
		return (MLIB_FAILURE);

	if (type == MLIB_SHORT)
		channels *= 2;

	SIZE_NN(channels);

	src_width_beg = src_width;
	src_height_beg = src_height;

	if (filter == MLIB_NEAREST) {
		pdst = pdst_near;
		psrc = psrc_near;
		src_width = src_width_near;
		src_height = src_height_near;
		src_w_beg = src_w_beg_near;
		src_h_beg = src_h_beg_near;
	} else {
		flag = (dst_height & 1) | ((dst_width & 1) << 1);
		psrc_beg = psrc;

		if (filter == MLIB_BILINEAR) {
			SIZE_BL(channels);
		} else {
			SIZE_BC(channels);
		}
	}

	if (edge == MLIB_EDGE_SRC_PADDED) {
		SIZE_SRC_PADDED(channels);
	}

	if (src_width < 0)
		src_width = 0;

	if (src_height < 0)
		src_height = 0;

	if (type == MLIB_SHORT) {
		channels >>= 1;
		dst_stride >>= 1;
		src_stride >>= 1;
	}

	res = MLIB_SUCCESS;

	if ((src_width_near <= 0) || (src_height_near <= 0)) {
		return (res);
	}

	switch (type) {
	case MLIB_BYTE:
		switch (edge) {
		case MLIB_EDGE_DST_FILL_ZERO:
			mlib_ImageZoomIn2XIndex_U8_CE
			    (MLIB_ZOOMIN2XINDEXCALLEDGE_CE(mlib_u8));
			break;
		case MLIB_EDGE_OP_NEAREST:
			mlib_ImageZoomIn2XIndex_U8_ON
			    (MLIB_ZOOMIN2XINDEXCALLEDGE_NN(mlib_u8));
			break;
		case MLIB_EDGE_SRC_EXTEND:
			switch (filter) {
			case MLIB_BILINEAR:
				switch (flag) {
				case 0:
					res =
					    mlib_ImageZoomIn2XIndex_U8_SE_BL_FL0
					    (MLIB_ZOOMIN2XINDEXCALLEDGE_BL
					    (mlib_u8));
					break;
				case 1:
					res =
					    mlib_ImageZoomIn2XIndex_U8_SE_BL_FL1
					    (MLIB_ZOOMIN2XINDEXCALLEDGE_BL
					    (mlib_u8));
					break;
				case 2:
					res =
					    mlib_ImageZoomIn2XIndex_U8_SE_BL_FL2
					    (MLIB_ZOOMIN2XINDEXCALLEDGE_BL
					    (mlib_u8));
					break;
				case 3:
					res =
					    mlib_ImageZoomIn2XIndex_U8_SE_BL_FL3
					    (MLIB_ZOOMIN2XINDEXCALLEDGE_BL
					    (mlib_u8));
					break;
				}

				break;
			case MLIB_BICUBIC:
				switch (flag) {
				case 0:
					res =
					    mlib_ImageZoomIn2XIndex_U8_SE_BC_FL0
					    (MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					    (mlib_u8));
					break;
				case 1:
					res =
					    mlib_ImageZoomIn2XIndex_U8_SE_BC_FL1
					    (MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					    (mlib_u8));
					break;
				case 2:
					res =
					    mlib_ImageZoomIn2XIndex_U8_SE_BC_FL2
					    (MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					    (mlib_u8));
					break;
				case 3:
					res =
					    mlib_ImageZoomIn2XIndex_U8_SE_BC_FL3
					    (MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					    (mlib_u8));
					break;
				}

				break;
			case MLIB_BICUBIC2:
				switch (flag) {
				case 0:
				    res =
					mlib_ImageZoomIn2XIndex_U8_SE_BC2_FL0
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					(mlib_u8));
					break;
				case 1:
				    res =
					mlib_ImageZoomIn2XIndex_U8_SE_BC2_FL1
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					(mlib_u8));
					break;
				case 2:
				    res =
					mlib_ImageZoomIn2XIndex_U8_SE_BC2_FL2
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					(mlib_u8));
					break;
				case 3:
				    res =
					mlib_ImageZoomIn2XIndex_U8_SE_BC2_FL3
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					(mlib_u8));
					break;
				}

				break;
			default :
				return (MLIB_FAILURE);
			}
			break;
		default :
			return (MLIB_FAILURE);
		}

		break;
	case MLIB_SHORT:
		switch (edge) {
		case MLIB_EDGE_DST_FILL_ZERO:
			mlib_ImageZoomIn2XIndex_S16_CE
			    (MLIB_ZOOMIN2XINDEXCALLEDGE_CE(mlib_s16));
			break;
		case MLIB_EDGE_OP_NEAREST:
			mlib_ImageZoomIn2XIndex_S16_ON
			    (MLIB_ZOOMIN2XINDEXCALLEDGE_NN(mlib_s16));
			break;
		case MLIB_EDGE_SRC_EXTEND:
			switch (filter) {
			case MLIB_BILINEAR:
				switch (flag) {
				case 0:
				    res =
					mlib_ImageZoomIn2XIndex_S16_SE_BL_FL0
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BL
					(mlib_s16));
					break;
				case 1:
				    res =
					mlib_ImageZoomIn2XIndex_S16_SE_BL_FL1
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BL
					(mlib_s16));
					break;
				case 2:
				    res =
					mlib_ImageZoomIn2XIndex_S16_SE_BL_FL2
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BL
					(mlib_s16));
					break;
				case 3:
				    res =
					mlib_ImageZoomIn2XIndex_S16_SE_BL_FL3
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BL
					(mlib_s16));
					break;
				}

				break;
			case MLIB_BICUBIC:
				switch (flag) {
				case 0:
				    res =
					mlib_ImageZoomIn2XIndex_S16_SE_BC_FL0
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					(mlib_s16));
					break;
				case 1:
				    res =
					mlib_ImageZoomIn2XIndex_S16_SE_BC_FL1
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					(mlib_s16));
					break;
				case 2:
				    res =
					mlib_ImageZoomIn2XIndex_S16_SE_BC_FL2
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					(mlib_s16));
					break;
				case 3:
				    res =
					mlib_ImageZoomIn2XIndex_S16_SE_BC_FL3
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					(mlib_s16));
					break;
				}

				break;
			case MLIB_BICUBIC2:
				switch (flag) {
				case 0:
				    res =
					mlib_ImageZoomIn2XIndex_S16_SE_BC2_FL0
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					(mlib_s16));
					break;
				case 1:
				    res =
					mlib_ImageZoomIn2XIndex_S16_SE_BC2_FL1
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					(mlib_s16));
					break;
				case 2:
				    res =
					mlib_ImageZoomIn2XIndex_S16_SE_BC2_FL2
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					(mlib_s16));
					break;
				case 3:
				    res =
					mlib_ImageZoomIn2XIndex_S16_SE_BC2_FL3
					(MLIB_ZOOMIN2XINDEXCALLEDGE_BC
					(mlib_s16));
					break;
				}

				break;
			default :
				return (MLIB_FAILURE);
			}
			break;
		default :
			return (MLIB_FAILURE);
		}

		break;
	default :
		return (MLIB_FAILURE);
	}

	return (res);
}

/* *********************************************************** */
