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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_ImageZoomOut2X_Fp.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomOut2X_Fp - image 0.5X scaling with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoomOut2X_Fp(mlib_image       *dst,
 *                                         const mlib_image *src,
 *                                         mlib_filter      filter,
 *                                         mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      filter    Type of resampling filter.
 *      edge      Type of edge condition.
 *
 * DESCRIPTION
 *    The center of the source image is mapped to the center of the
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
 *
 * RESTRICTION
 *      src and dst must be the same type and the same number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be in MLIB_FLOAT or
 *      MLIB_DOUBLE data type.
 */

#include <mlib_image.h>
#include <mlib_ImageZoomOut2X.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageZoomOut2X_Fp = __mlib_ImageZoomOut2X_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageZoomOut2X_Fp) mlib_ImageZoomOut2X_Fp
    __attribute__((weak, alias("__mlib_ImageZoomOut2X_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageZoomOut2X_Fp(
    mlib_image *dst,
    const mlib_image *src,
    mlib_filter filter,
    mlib_edge edge)
{
	mlib_type type;
	mlib_s32 tshift = 0;
	mlib_u8 *p_src, *p_dst;
	mlib_s32 src_stride, dst_stride;
	mlib_s32 src_height, dst_height, src_width, dst_width;
	mlib_s32 chan, bchan, dh, dw;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);

	MLIB_IMAGE_GET_ALL_PARAMS(src, type, chan, src_width, src_height,
	    src_stride, p_src);
	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, chan, dst_width, dst_height,
	    dst_stride, p_dst);

	switch (type) {
	case MLIB_FLOAT:
		tshift = 2;
		break;
	case MLIB_DOUBLE:
		tshift = 3;
		break;
	default:
		return (MLIB_FAILURE);
	}

	bchan = chan << tshift;

	switch (filter) {
	case MLIB_NEAREST:
		BOUND_NEAREST();
		src_stride >>= tshift;
		dst_stride >>= tshift;

		if (dst_width > 0 && dst_height > 0) {
			mlib_ImageZoomOut2X_Nearest(NEAREST_P, tshift, chan);
		}

		return (MLIB_SUCCESS);

	case MLIB_BILINEAR:
		BOUND_BILINEAR();
		src_stride >>= tshift;
		dst_stride >>= tshift;
		switch (type) {
		case MLIB_FLOAT:
			SWITCH_BL_CHANNELS(F32)
		case MLIB_DOUBLE:
			SWITCH_BL_CHANNELS(D64)
		default:
			return (MLIB_FAILURE);
		}

	case MLIB_BICUBIC:
		BOUND_BICUBIC();
		src_stride >>= tshift;
		dst_stride >>= tshift;
		switch (type) {
		case MLIB_FLOAT:
			SWITCH_BC_CHANNELS(F32, Bicubic)
		case MLIB_DOUBLE:
			SWITCH_BC_CHANNELS(D64, Bicubic)
		default:
			return (MLIB_FAILURE);
		}

	case MLIB_BICUBIC2:
		BOUND_BICUBIC();
		src_stride >>= tshift;
		dst_stride >>= tshift;
		switch (type) {
		case MLIB_FLOAT:
			SWITCH_BC_CHANNELS(F32, Bicubic2)
		case MLIB_DOUBLE:
			SWITCH_BC_CHANNELS(D64, Bicubic2)
		default:
			return (MLIB_FAILURE);
		}

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */
