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

#pragma ident	"@(#)mlib_v_ImageZoomIndex.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomIndex - indexed color image scaling
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoomIndex(mlib_image       *dst,
 *                                      const mlib_image *src,
 *                                      mlib_d64         zoomx,
 *                                      mlib_d64         zoomy,
 *                                      mlib_filter      filter,
 *                                      mlib_edge        edge,
 *                                      const void       *colormap)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image
 *      src       Pointer to source image
 *      zoomx     X zoom factor.
 *      zoomy     Y zoom factor.
 *      filter    Type of resampling filter.
 *      edge      Type of edge conditions.
 *      colormap  Internal structure.
 *
 * DESCRIPTION
 *  The center of the source image is mapped to the center of the
 *  destination image.
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
 *
 * RESTRICTION
 *      src and dst must be the same type and have just one channel.
 *      They can be in MLIB_BYTE or MLIB_SHORT data type.
 *
 *      src image can not have width or height larger than 32767.
 */

#include <mlib_image.h>
#include <mlib_ImageColormap.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageZoomIndex = __mlib_ImageZoomIndex

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageZoomIndex) mlib_ImageZoomIndex
    __attribute__((weak, alias("__mlib_ImageZoomIndex")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	_MLIB_VIS_VER_

#include <mlib_ImageZoom.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageFilters.h>

/* *********************************************************** */

mlib_status
__mlib_ImageZoomTranslateIndex(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 zoomx,
    mlib_d64 zoomy,
    mlib_d64 tx,
    mlib_d64 ty,
    mlib_filter filter,
    mlib_edge edge,
    const void *colormap)
{
	mlib_s32 mask, len;
	mlib_clipping nearest, current;
	mlib_work_image border;

	if (filter == MLIB_NEAREST)
		return __mlib_ImageZoomTranslate(dst, src, zoomx, zoomy, tx, ty,
		    filter, edge);

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_HAVE_CHAN(src, 1);
	MLIB_IMAGE_HAVE_CHAN(dst, 1);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);

	if (zoomx <= 0 || zoomy <= 0)
		return (MLIB_OUTOFRANGE);

	if (mlib_ImageGetWidth(src) >= (1 << 15) ||
	    mlib_ImageGetHeight(src) >= (1 << 15)) {
		return (MLIB_FAILURE);
	}

	border.nearest = &nearest;
	border.current = &current;

	mlib_ImageZoomClipping(dst, src, zoomx, zoomy, tx, ty, filter, edge,
	    &border);
	border.colormap = (void *)colormap;
	border.color = mlib_ImageGetLutOffset(colormap);

	len = mlib_ImageGetWidth(dst);

	if (len < mlib_ImageGetHeight(dst))
		len = mlib_ImageGetHeight(dst);
	border.buffer_dp =
	    __mlib_malloc(len * (mlib_ImageGetLutType(colormap) ==
	    MLIB_SHORT ? sizeof (mlib_d64) : sizeof (mlib_f32)));

	if (border.buffer_dp == NULL)
		return (MLIB_FAILURE);

	mask = (mlib_ImageGetType(src) << 8) | mlib_ImageGetLutType(colormap);

	if (current.width == 0)
		goto jmp_edge;

	switch (filter) {

	case MLIB_BILINEAR:

		switch (mask) {
/* index: MLIB_BYTE, pal: MLIB_BYTE */
		case (MLIB_BYTE << 8) | MLIB_BYTE:

			if (mlib_v_ImageZoomIndex_U8_U8_Bilinear(&border) !=
			    MLIB_SUCCESS)
				return (MLIB_FAILURE);
			break;

/* index: MLIB_SHORT, pal: MLIB_SHORT */
		case (MLIB_SHORT << 8) | MLIB_SHORT:

			if (mlib_v_ImageZoomIndex_S16_S16_Bilinear(&border) !=
			    MLIB_SUCCESS)
				return (MLIB_FAILURE);
			break;

/* index: MLIB_BYTE, pal: MLIB_SHORT */
		case (MLIB_BYTE << 8) | MLIB_SHORT:

			if (mlib_v_ImageZoomIndex_U8_S16_Bilinear(&border) !=
			    MLIB_SUCCESS)
				return (MLIB_FAILURE);
			break;

/* index: MLIB_SHORT, pal: MLIB_BYTE */
		case (MLIB_SHORT << 8) | MLIB_BYTE:

			if (mlib_v_ImageZoomIndex_S16_U8_Bilinear(&border) !=
			    MLIB_SUCCESS)
				return (MLIB_FAILURE);
			break;

		default:
			__mlib_free(border.buffer_dp);
			return (MLIB_FAILURE);
		}

/* case MLIB_BILINEAR */
		break;

	case MLIB_BICUBIC:
	case MLIB_BICUBIC2:

		switch (mask) {
/* index: MLIB_BYTE, pal: MLIB_BYTE */
		case (MLIB_BYTE << 8) | MLIB_BYTE:

			if (mlib_v_ImageZoomIndex_U8_U8_Bicubic(&border) !=
			    MLIB_SUCCESS)
				return (MLIB_FAILURE);
			break;

/* index: MLIB_SHORT, pal: MLIB_SHORT */
		case (MLIB_SHORT << 8) | MLIB_SHORT:

			if (mlib_v_ImageZoomIndex_S16_S16_Bicubic(&border) !=
			    MLIB_SUCCESS)
				return (MLIB_FAILURE);
			break;

/* index: MLIB_BYTE, pal: MLIB_SHORT */
		case (MLIB_BYTE << 8) | MLIB_SHORT:

			if (mlib_v_ImageZoomIndex_U8_S16_Bicubic(&border) !=
			    MLIB_SUCCESS)
				return (MLIB_FAILURE);
			break;

/* index: MLIB_SHORT, pal: MLIB_BYTE */
		case (MLIB_SHORT << 8) | MLIB_BYTE:

			if (mlib_v_ImageZoomIndex_S16_U8_Bicubic(&border) !=
			    MLIB_SUCCESS)
				return (MLIB_FAILURE);
			break;

		default:
			__mlib_free(border.buffer_dp);
			return (MLIB_FAILURE);
		}

/* case MLIB_BICUBIC */
		break;

	default:
		__mlib_free(border.buffer_dp);
		return (MLIB_FAILURE);
	}

jmp_edge:
	switch (edge) {

	case MLIB_EDGE_DST_FILL_ZERO:

		switch (mlib_ImageGetType(src)) {
		case MLIB_BYTE:
			mlib_ImageZoomZeroEdge_U8(&border);
			break;

		case MLIB_SHORT:
			mlib_ImageZoomZeroEdge_S16(&border);
			break;
		}

		break;

	case MLIB_EDGE_OP_NEAREST:

		switch (mlib_ImageGetType(src)) {
		case MLIB_BYTE:
			mlib_ImageZoomUpNearest_U8(&border);
			break;

		case MLIB_SHORT:
			mlib_ImageZoomUpNearest_S16(&border);
			break;
		}

		break;

	case MLIB_EDGE_SRC_EXTEND:

		switch (mlib_ImageGetType(src)) {
		case MLIB_BYTE:

			switch (filter) {
			case MLIB_BILINEAR:
				mlib_ImageZoomIndexExtend_U8_Bilinear(&border);
				break;

			case MLIB_BICUBIC:
				mlib_ImageZoomIndexExtend_U8_Bicubic(&border);
				break;

			case MLIB_BICUBIC2:
				mlib_ImageZoomIndexExtend_U8_Bicubic2(&border);
				break;
			}

			break;

		case MLIB_SHORT:
			switch (filter) {
			case MLIB_BILINEAR:
				mlib_ImageZoomIndexExtend_S16_Bilinear(&border);
				break;

			case MLIB_BICUBIC:
				mlib_ImageZoomIndexExtend_S16_Bicubic(&border);
				break;

			case MLIB_BICUBIC2:
				mlib_ImageZoomIndexExtend_S16_Bicubic2(&border);
				break;
			}

			break;
		}

		break;

	case MLIB_EDGE_DST_NO_WRITE:
	case MLIB_EDGE_DST_COPY_SRC:
	case MLIB_EDGE_OP_DEGRADED:
	case MLIB_EDGE_SRC_EXTEND_ZERO:
	case MLIB_EDGE_SRC_EXTEND_MIRROR:
	case MLIB_EDGE_SRC_PADDED:
	default:
		__mlib_free(border.buffer_dp);
		return (MLIB_SUCCESS);
	}

	__mlib_free(border.buffer_dp);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_ImageZoomIndex(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 zoomx,
    mlib_d64 zoomy,
    mlib_filter filter,
    mlib_edge edge,
    const void *colormap)
{
	mlib_d64 tx, ty;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);

	tx = (mlib_ImageGetWidth(dst) - zoomx * mlib_ImageGetWidth(src)) * 0.5;
	ty = (mlib_ImageGetHeight(dst) -
	    zoomy * mlib_ImageGetHeight(src)) * 0.5;

	return __mlib_ImageZoomTranslateIndex(dst, src, zoomx, zoomy, tx, ty,
	    filter, edge, colormap);
}

/* *********************************************************** */
