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

#pragma ident	"@(#)mlib_v_ImageZoomTranslate.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomTranslate - image scaling with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoomTranslate(mlib_image       *dst,
 *                                          const mlib_image *src,
 *                                          mlib_d64         zoomx,
 *                                          mlib_d64         zoomy,
 *                                          mlib_d64         tx,
 *                                          mlib_d64         ty,
 *                                          mlib_filter      filter,
 *                                          mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image
 *      src       Pointer to source image
 *      zoomx     X zoom factor.
 *      zoomy     Y zoom factor.
 *      tx        offset by X coordinate
 *      ty        offset by Y coordinate
 *      filter    Type of resampling filter.
 *      edge      Type of edge condition.
 *
 * DESCRIPTION
 *                      xd = zoomx*xs + tx
 *                      yd = zoomy*ys + ty
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
 *    MLIB_EDGE_SRC_EXTEND_INDEF
 *    MLIB_EDGE_SRC_PADDED
 *
 * RESTRICTION
 *      src and dst must be the same type and the same number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be in MLIB_BIT, MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 *      src image can not have width or height larger than 32767.
 */

#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_ImageZoom.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageFilters.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageZoomTranslate = __mlib_ImageZoomTranslate

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageZoomTranslate) mlib_ImageZoomTranslate
    __attribute__((weak, alias("__mlib_ImageZoomTranslate")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static const mlib_zoom_fun_type mlib_zoom_nn_funs[] = {
	mlib_ImageZoom_U8_1_Nearest,
	mlib_ImageZoom_U8_2_Nearest,
	mlib_ImageZoom_U8_3_Nearest,
	mlib_ImageZoom_U8_4_Nearest,
	mlib_ImageZoom_S16_1_Nearest,
	mlib_ImageZoom_S16_2_Nearest,
	mlib_ImageZoom_S16_3_Nearest,
	mlib_ImageZoom_S16_4_Nearest,
	mlib_ImageZoom_S32_1_Nearest,
	mlib_ImageZoom_S32_2_Nearest,
	mlib_ImageZoom_S32_3_Nearest,
	mlib_ImageZoom_S32_4_Nearest,
	mlib_ImageZoom_D64_1_Nearest,
	mlib_ImageZoom_D64_2_Nearest,
	mlib_ImageZoom_D64_3_Nearest,
	mlib_ImageZoom_D64_4_Nearest
};

/* *********************************************************** */

static const mlib_d64 mlib_zoom_bl_level[] = {
	0.0, 0.0, 0.0, 1.0
};

static const mlib_zoom_fun_type mlib_zoom_bl_lo[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	mlib_ImageZoomBilinear_S32_1s,
	mlib_ImageZoomBilinear_S32_2s,
	mlib_ImageZoomBilinear_S32_3s,
	mlib_ImageZoomBilinear_S32_4s
};

static const mlib_zoom_fun_type mlib_zoom_bl_hi[] = {
	mlib_v_ImageZoomBilinear_U8_1,
	mlib_v_ImageZoomBilinear_U8_2,
	mlib_v_ImageZoomBilinear_U8_3,
	mlib_v_ImageZoomBilinear_U8_4,
	mlib_v_ImageZoomBilinear_S16_1,
	mlib_v_ImageZoomBilinear_S16_2,
	mlib_v_ImageZoomBilinear_S16_3,
	mlib_v_ImageZoomBilinear_S16_4,
	mlib_v_ImageZoomBilinear_U16_1,
	mlib_v_ImageZoomBilinear_U16_2,
	mlib_v_ImageZoomBilinear_U16_3,
	mlib_v_ImageZoomBilinear_U16_4,
	mlib_ImageZoomBilinear_S32_1,
	mlib_ImageZoomBilinear_S32_2,
	mlib_ImageZoomBilinear_S32_3,
	mlib_ImageZoomBilinear_S32_4
};

/* *********************************************************** */

static const mlib_d64 mlib_zoom_bc_level[] = {
	0.2, 0.0, 0.0, 0.0
};

static const mlib_zoom_fun_type mlib_zoom_bc_lo[] = {
	mlib_v_ImageZoomBicubic_U8_1s,
	mlib_v_ImageZoomBicubic_U8_2s,
	mlib_v_ImageZoomBicubic_U8_3s,
	mlib_v_ImageZoomBicubic_U8_4s,
};

static const mlib_zoom_fun_type mlib_zoom_bc_hi[] = {
	mlib_v_ImageZoomBicubic_U8_1,
	mlib_v_ImageZoomBicubic_U8_2,
	mlib_v_ImageZoomBicubic_U8_3,
	mlib_v_ImageZoomBicubic_U8_4,
	mlib_v_ImageZoomBicubic_S16_1,
	mlib_v_ImageZoomBicubic_S16_2,
	mlib_v_ImageZoomBicubic_S16_3,
	mlib_v_ImageZoomBicubic_S16_4,
	mlib_v_ImageZoomBicubic_U16_1,
	mlib_v_ImageZoomBicubic_U16_2,
	mlib_v_ImageZoomBicubic_U16_3,
	mlib_v_ImageZoomBicubic_U16_4,
	mlib_ImageZoomBicubic_S32_1,
	mlib_ImageZoomBicubic_S32_2,
	mlib_ImageZoomBicubic_S32_3,
	mlib_ImageZoomBicubic_S32_4
};

/* *********************************************************** */

mlib_status
__mlib_ImageZoomTranslate(
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
	mlib_work_image border, *param = &border;

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

	mlib_ImageZoomClipping(dst, src, zoomx, zoomy,
	    tx, ty, filter, edge, &border);

	type = mlib_ImageGetType(src);
	nchan = mlib_ImageGetChannels(src);

	if (type == MLIB_BIT) {
		if (nchan != 1 || filter != MLIB_NEAREST)
			return (MLIB_FAILURE);
		if (current.width > 0) {
			int s_bitoff = mlib_ImageGetBitOffset(src);
			int d_bitoff = mlib_ImageGetBitOffset(dst);

			return mlib_ImageZoom_BIT_1_Nearest(&border, s_bitoff,
			    d_bitoff);
		}

		return (MLIB_SUCCESS);
	}

	switch (type) {
	case MLIB_BYTE:
		t_ind = 0;
		break;
	case MLIB_SHORT:
		t_ind = 1;
		break;
	case MLIB_USHORT:
		t_ind = 2;
		break;
	case MLIB_INT:
		t_ind = 3;
		break;
	default:
		return (MLIB_FAILURE);
	}

	if (current.width > 0) {
		switch (filter) {
		case MLIB_NEAREST:
			res = mlib_zoom_nn_funs[border.ind_fun_nn] (&border);
			break;

		case MLIB_BILINEAR:
			if (zoomy < mlib_zoom_bl_level[t_ind]) {
				res =
				    mlib_zoom_bl_lo[4 * t_ind + (nchan -
				    1)] (param);
			} else {
				res =
				    mlib_zoom_bl_hi[4 * t_ind + (nchan -
				    1)] (param);
			}
			break;

		case MLIB_BICUBIC:
		case MLIB_BICUBIC2:
			if (filter == MLIB_BICUBIC) {
				if (type == MLIB_BYTE) {
					border.filter1 =
					    (void *)mlib_filters_u8_bc;
					border.filter3 =
					    (void *)mlib_filters_u8_bc_3;
					border.filter4 =
					    (void *)mlib_filters_u8_bc_4;
				} else {
					border.filter1 =
					    (void *)mlib_filters_s16_bc;
					border.filter3 =
					    (void *)mlib_filters_s16_bc_3;
					border.filter4 =
					    (void *)mlib_filters_s16_bc_4;
				}
			} else {
				if (type == MLIB_BYTE) {
					border.filter1 =
					    (void *)mlib_filters_u8_bc2;
					border.filter3 =
					    (void *)mlib_filters_u8_bc2_3;
					border.filter4 =
					    (void *)mlib_filters_u8_bc2_4;
				} else {
					border.filter1 =
					    (void *)mlib_filters_s16_bc2;
					border.filter3 =
					    (void *)mlib_filters_s16_bc2_3;
					border.filter4 =
					    (void *)mlib_filters_s16_bc2_4;
				}
			}

			if (zoomy < mlib_zoom_bc_level[t_ind]) {
				res =
				    mlib_zoom_bc_lo[4 * t_ind + (nchan -
				    1)] (param);
			} else {
				res =
				    mlib_zoom_bc_hi[4 * t_ind + (nchan -
				    1)] (param);
			}
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

	MLIB_EDGE_RULES return (MLIB_SUCCESS);
}

/* *********************************************************** */
