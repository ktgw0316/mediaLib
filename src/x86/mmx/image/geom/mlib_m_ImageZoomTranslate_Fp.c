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

#pragma ident	"@(#)mlib_m_ImageZoomTranslate_Fp.c	9.5	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomTranslate_Fp - image scaling with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoomTranslate_Fp(mlib_image       *dst,
 *                                             const mlib_image *src,
 *                                             mlib_d64         zoomx,
 *                                             mlib_d64         zoomy,
 *                                             mlib_d64         tx,
 *                                             mlib_d64         ty,
 *                                             mlib_filter      filter,
 *                                             mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      zoomx     X zoom factor.
 *      zoomy     Y zoom factor.
 *      tx        X translation.
 *      ty        Y translation.
 *      filter    Type of resampling filter.
 *      edge      Type of edge condition.
 *
 * DESCRIPTION
 *      xd = zoomx*xs + tx
 *      yd = zoomy*ys + ty
 *
 *      The upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *      The resampling filter can be one of the following:
 *        MLIB_NEAREST
 *        MLIB_BILINEAR
 *        MLIB_BICUBIC
 *        MLIB_BICUBIC2
 *
 *      The edge condition can be one of the following:
 *        MLIB_EDGE_DST_NO_WRITE  (default)
 *        MLIB_EDGE_DST_FILL_ZERO
 *        MLIB_EDGE_OP_NEAREST
 *        MLIB_EDGE_SRC_EXTEND
 *        MLIB_EDGE_SRC_EXTEND_INDEF
 *        MLIB_EDGE_SRC_PADDED
 *
 * RESTRICTION
 *      src and dst must be the same type and the same number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be in MLIB_FLOAT
 *      or MLIB_DOUBLE data type.
 *
 *      src image can not have width or height larger than 32767.
 */

/* *********************************************************** */

#include <mlib_image.h>
#include <mlib_SysMath.h>

/* *************************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageZoomTranslate_Fp = __mlib_ImageZoomTranslate_Fp

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageZoomTranslate_Fp) mlib_ImageZoomTranslate_Fp
    __attribute__((weak, alias("__mlib_ImageZoomTranslate_Fp")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *************************************************************** */

#define	_MLIB_C_VER_

#include <mlib_ImageZoom.h>
#include <mlib_ImageCheck.h>

FUNC_PROT(mlib_ImageZoomBilinear_F32);
FUNC_PROT(mlib_ImageZoomBilinear_D64);
FUNC_PROT(mlib_ImageZoomBicubic_F32);
FUNC_PROT(mlib_ImageZoomBicubic_D64);

/* *********************************************************** */

static mlib_zoom_fun_type mlib_zoom_nn_funs[] = {
	mlib_ImageZoom_S32_1_Nearest,
	mlib_ImageZoom_D64_1_Nearest,
	mlib_ImageZoom_S32_3_Nearest,
	mlib_ImageZoom_D64_2_Nearest,
	mlib_ImageZoom_D64_1_Nearest,
	mlib_ImageZoom_D64_2_Nearest,
	mlib_ImageZoom_D64_3_Nearest,
	mlib_ImageZoom_D64_4_Nearest
};

static mlib_zoom_fun_type mlib_zoom_bl_lo[] = {
	mlib_ImageZoomBilinear_F32_1s,
	mlib_ImageZoomBilinear_F32_2s,
	mlib_ImageZoomBilinear_F32_3s,
	mlib_ImageZoomBilinear_F32_4s,
	mlib_ImageZoomBilinear_D64_1s,
	mlib_ImageZoomBilinear_D64_2s,
	mlib_ImageZoomBilinear_D64_3s,
	mlib_ImageZoomBilinear_D64_4s
};

static mlib_zoom_fun_type mlib_zoom_bl_hi[] = {
	mlib_ImageZoomBilinear_F32_1,
	mlib_ImageZoomBilinear_F32_2,
	mlib_ImageZoomBilinear_F32_3,
	mlib_ImageZoomBilinear_F32_4,
	mlib_ImageZoomBilinear_D64_1,
	mlib_ImageZoomBilinear_D64_2,
	mlib_ImageZoomBilinear_D64_3,
	mlib_ImageZoomBilinear_D64_4
};

static mlib_zoom_fun_type mlib_zoom_bc_funs[] = {
	mlib_ImageZoomBicubic_F32_1,
	mlib_ImageZoomBicubic_F32_2,
	mlib_ImageZoomBicubic_F32_3,
	mlib_ImageZoomBicubic_F32_4,
	mlib_ImageZoomBicubic_D64_1,
	mlib_ImageZoomBicubic_D64_2,
	mlib_ImageZoomBicubic_D64_3,
	mlib_ImageZoomBicubic_D64_4
};

/* *********************************************************** */

mlib_status
__mlib_ImageZoomTranslate_Fp(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 zoomx,
    mlib_d64 zoomy,
    mlib_d64 tx,
    mlib_d64 ty,
    mlib_filter filter,
    mlib_edge edge)
{
	mlib_type type;
	mlib_s32 nchan, t_ind;
	mlib_status res;
	mlib_clipping nearest, current;
	mlib_work_image border;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);

	if (zoomx <= 0 || zoomy <= 0)
		return (MLIB_OUTOFRANGE);

	if (mlib_ImageGetWidth(src) >= (1 << 15) ||
	    mlib_ImageGetHeight(src) >= (1 << 15)) {
		return (MLIB_FAILURE);
	}

	border.nearest = &nearest;
	border.current = &current;

	mlib_ImageZoomClipping(dst, src,
	    zoomx, zoomy, tx, ty, filter, edge, &border);

	type = mlib_ImageGetType(src);
	nchan = mlib_ImageGetChannels(src);

	if (type == MLIB_FLOAT) {
		t_ind = 0;
	} else if (type == MLIB_DOUBLE) {
		t_ind = 1;
	} else {
		return (MLIB_FAILURE);
	}

	if (current.width > 0) {
		switch (filter) {
		case MLIB_NEAREST:
			res =
			    (mlib_zoom_nn_funs[border.ind_fun_nn -
			    2 * 4]) (&border);
			break;

		case MLIB_BILINEAR:
			if (zoomy < 1.0) {
				res =
				    mlib_zoom_bl_lo[4 * t_ind + (nchan -
				    1)] (&border);
			} else {
				res =
				    mlib_zoom_bl_hi[4 * t_ind + (nchan -
				    1)] (&border);
			}
			break;

		case MLIB_BICUBIC:
		case MLIB_BICUBIC2:
			res =
			    mlib_zoom_bc_funs[4 * t_ind + (nchan -
			    1)] (&border);
			break;

		default:
			return (MLIB_FAILURE);
		}
		if (res != MLIB_SUCCESS)
			return (res);
	}

	if (filter == MLIB_NEAREST && edge != MLIB_EDGE_SRC_EXTEND_INDEF) {
		return (MLIB_SUCCESS);
	}

	switch (edge) {

/* handle edge condition of image */
	case MLIB_EDGE_DST_FILL_ZERO:

		switch (mlib_ImageGetType(src)) {
		case MLIB_FLOAT:
			mlib_ImageZoomZeroEdge_F32(&border);
			break;

		case MLIB_DOUBLE:
			mlib_ImageZoomZeroEdge_D64(&border);
			break;
		}
		break;

	case MLIB_EDGE_OP_NEAREST:

		switch (mlib_ImageGetType(src)) {
		case MLIB_FLOAT:
			mlib_ImageZoomUpNearest_F32(&border);
			break;

		case MLIB_DOUBLE:
			mlib_ImageZoomUpNearest_D64(&border);
			break;
		}
		break;

	case MLIB_EDGE_SRC_EXTEND:

		switch (mlib_ImageGetType(src)) {
		case MLIB_FLOAT:
			switch (filter) {
			case MLIB_BILINEAR:
				mlib_ImageZoomExtend_F32_Bilinear(&border);
				break;

			case MLIB_BICUBIC:
				mlib_ImageZoomExtend_F32_Bicubic(&border);
				break;

			case MLIB_BICUBIC2:
				mlib_ImageZoomExtend_F32_Bicubic2(&border);
				break;
			}
			break;

		case MLIB_DOUBLE:
			switch (filter) {
			case MLIB_BILINEAR:
				mlib_ImageZoomExtend_D64_Bilinear(&border);
				break;

			case MLIB_BICUBIC:
				mlib_ImageZoomExtend_D64_Bicubic(&border);
				break;

			case MLIB_BICUBIC2:
				mlib_ImageZoomExtend_D64_Bicubic2(&border);
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
	default:
		return (MLIB_SUCCESS);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
