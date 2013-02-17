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

#pragma ident	"@(#)mlib_s_ImageZoomTranslate.c	9.8	07/11/05 SMI"

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

/* *********************************************************** */

#include <mlib_s_ImageZoom.h>

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageZoomTranslate = __mlib_ImageZoomTranslate

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageZoomTranslate) mlib_ImageZoomTranslate
    __attribute__((weak, alias("__mlib_ImageZoomTranslate")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	RETURN(func_name)                                       \
	if (func_name(&border) != MLIB_SUCCESS)                 \
	    return MLIB_FAILURE

/* *********************************************************** */

    mlib_status mlib_ImageZoomBicubic_S32_1(
    mlib_work_image * param);
    mlib_status mlib_ImageZoomBicubic_S32_2(
    mlib_work_image * param);
    mlib_status mlib_ImageZoomBicubic_S32_3(
    mlib_work_image * param);
    mlib_status mlib_ImageZoomBicubic_S32_4(
    mlib_work_image * param);

/* *********************************************************** */

    static mlib_zoom_fun_type mlib_zoom_nn_funs[] = {
	    mlib_ImageZoom_U8_1_Nearest,
	    mlib_ImageZoom_S16_1_Nearest,
	    mlib_ImageZoom_U8_3_Nearest,
	    mlib_ImageZoom_S32_1_Nearest,
	    mlib_ImageZoom_S16_1_Nearest,
	    mlib_ImageZoom_S32_1_Nearest,
	    mlib_ImageZoom_S16_3_Nearest,
	    mlib_ImageZoom_D64_1_Nearest,
	    mlib_ImageZoom_S32_1_Nearest,
	    mlib_ImageZoom_D64_1_Nearest,
	    mlib_ImageZoom_S32_3_Nearest,
	    mlib_ImageZoom_D64_2_Nearest,
	    mlib_ImageZoom_D64_1_Nearest,
	    mlib_ImageZoom_D64_2_Nearest,
	    mlib_ImageZoom_D64_3_Nearest,
	    mlib_ImageZoom_D64_4_Nearest
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
	mlib_s32 nchan;
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

	if (filter == MLIB_BICUBIC) {
		if (type == MLIB_BYTE) {
			border.filter1 = (void *)mlib_filters_u8_bc;
			border.filter3 = (void *)mlib_filters_u8_bc_3;
			border.filter4 = (void *)mlib_filters_u8_bc_4;
		} else {
			border.filter1 = (void *)mlib_filters_s16_bc;
			border.filter3 = (void *)mlib_filters_s16_bc_3;
			border.filter4 = (void *)mlib_filters_s16_bc_4;
		}
	} else {
		if (type == MLIB_BYTE) {
			border.filter1 = (void *)mlib_filters_u8_bc2;
			border.filter3 = (void *)mlib_filters_u8_bc2_3;
			border.filter4 = (void *)mlib_filters_u8_bc2_4;
		} else {
			border.filter1 = (void *)mlib_filters_s16_bc2;
			border.filter3 = (void *)mlib_filters_s16_bc2_3;
			border.filter4 = (void *)mlib_filters_s16_bc2_4;
		}
	}

	if (current.width > 0) {
		if (filter == MLIB_NEAREST) {
			RETURN(mlib_zoom_nn_funs[border.ind_fun_nn]);
		} else

		switch (type) {

/* handle MLIB_BYTE data type of image */
		case MLIB_BYTE: {

			switch (mlib_ImageGetChannels(src)) {

			case 1:
				switch (filter) {

				case MLIB_BILINEAR:
					RETURN(mlib_s_ImageZoom_BL_U8_1);
					break;

				case MLIB_BICUBIC:
				case MLIB_BICUBIC2:
					RETURN(mlib_s_ImageZoom_BC_U8_12);
					break;

				default:
					return (MLIB_FAILURE);
				}
				break;

			case 2:
				switch (filter) {

				case MLIB_BILINEAR:
					RETURN(mlib_s_ImageZoom_BL_U8_2);
					break;

				case MLIB_BICUBIC:
				case MLIB_BICUBIC2:
					RETURN(mlib_s_ImageZoom_BC_U8_12);
					break;

				default:
					return (MLIB_FAILURE);
				}
				break;

			case 3:
				switch (filter) {

				case MLIB_BILINEAR:
					RETURN(mlib_s_ImageZoomBlend_BL_U8);
					break;

				case MLIB_BICUBIC:
				case MLIB_BICUBIC2:
					RETURN(mlib_s_ImageZoomBlend_BC_U8);
					break;

				default:
					return (MLIB_FAILURE);
				}
				break;

			case 4:
				switch (filter) {

				case MLIB_BILINEAR:
					RETURN(mlib_s_ImageZoomBlend_BL_U8);
					break;

				case MLIB_BICUBIC:
				case MLIB_BICUBIC2:
					RETURN(mlib_s_ImageZoomBlend_BC_U8);
					break;

				default:
					return (MLIB_FAILURE);
				}
				break;

			default:
				return (MLIB_FAILURE);

			}
		}
		break;

/* handle MLIB_SHORT data type of image */
		case MLIB_SHORT: {

			switch (mlib_ImageGetChannels(src)) {

			case 1:
				switch (filter) {

				case MLIB_BILINEAR:
					RETURN(mlib_s_ImageZoom_BL_S16_1);
					break;

				case MLIB_BICUBIC:
				case MLIB_BICUBIC2:
					RETURN(mlib_s_ImageZoom_BC_S16_12);
					break;

				default:
					return (MLIB_FAILURE);
				}
				break;

			case 2:
				switch (filter) {

				case MLIB_BILINEAR:
					RETURN(mlib_s_ImageZoom_BL_S16_2);
					break;

				case MLIB_BICUBIC:
				case MLIB_BICUBIC2:
					RETURN(mlib_s_ImageZoom_BC_S16_12);
					break;

				default:
					return (MLIB_FAILURE);
				}
				break;

			case 3:
				switch (filter) {

				case MLIB_BILINEAR:
					RETURN(mlib_s_ImageZoom_BL_S16_3);
					break;

				case MLIB_BICUBIC:
				case MLIB_BICUBIC2:
					RETURN(mlib_s_ImageZoom_BC_S16_3);
					break;

				default:
					return (MLIB_FAILURE);
				}
				break;

			case 4:
				switch (filter) {

				case MLIB_BILINEAR:
					RETURN(mlib_s_ImageZoom_BL_S16_4);
					break;

				case MLIB_BICUBIC:
				case MLIB_BICUBIC2:
					RETURN(mlib_s_ImageZoom_BC_S16_4);
					break;

				default:
					return (MLIB_FAILURE);
				}
				break;

			default:
				return (MLIB_FAILURE);

			}
		}
		break;

/* handle MLIB_USHORT data type of image */
		case MLIB_USHORT: {

			switch (mlib_ImageGetChannels(src)) {

			case 1:
				switch (filter) {

				case MLIB_BILINEAR:
					RETURN(mlib_s_ImageZoom_BL_U16_1);
					break;

				case MLIB_BICUBIC:
				case MLIB_BICUBIC2:
					RETURN(mlib_s_ImageZoom_BC_U16_1);
					break;

				default:
					return (MLIB_FAILURE);
				}
				break;

			case 2:
				switch (filter) {

				case MLIB_BILINEAR:
					RETURN(mlib_s_ImageZoom_BL_U16_2);
					break;

				case MLIB_BICUBIC:
				case MLIB_BICUBIC2:
					RETURN(mlib_s_ImageZoom_BC_U16_2);
					break;

				default:
					return (MLIB_FAILURE);
				}
				break;

			case 3:
				switch (filter) {

				case MLIB_BILINEAR:
					RETURN(mlib_s_ImageZoom_BL_U16_3);
					break;

				case MLIB_BICUBIC:
				case MLIB_BICUBIC2:
					RETURN(mlib_s_ImageZoom_BC_U16_3);
					break;

				default:
					return (MLIB_FAILURE);
				}
				break;

			case 4:
				switch (filter) {

				case MLIB_BILINEAR:
					RETURN(mlib_s_ImageZoom_BL_U16_4);
					break;

				case MLIB_BICUBIC:
				case MLIB_BICUBIC2:
					RETURN(mlib_s_ImageZoom_BC_U16_4);
					break;

				default:
					return (MLIB_FAILURE);
				}
				break;

			default:
				return (MLIB_FAILURE);

			}
		}
		break;

/* handle MLIB_INT data type of image */
		case MLIB_INT: {

			switch (mlib_ImageGetChannels(src)) {

			case 1:
				switch (filter) {

				case MLIB_BILINEAR:
					RETURN(mlib_ImageZoomBilinear_S32_1);
					break;

				case MLIB_BICUBIC:
				case MLIB_BICUBIC2:
					RETURN(mlib_ImageZoomBicubic_S32_1);
					break;

				default:
					return (MLIB_FAILURE);
				}
				break;

			case 2:
				switch (filter) {

				case MLIB_BILINEAR:
					if (zoomy < 2) {
					    RETURN
						(mlib_ImageZoomBilinear_S32_2s);
					} else {
					    RETURN
						(mlib_ImageZoomBilinear_S32_2);
					}
					break;

				case MLIB_BICUBIC:
				case MLIB_BICUBIC2:
					RETURN(mlib_ImageZoomBicubic_S32_2);
					break;

				default:
					return (MLIB_FAILURE);
				}
				break;

			case 3:
				switch (filter) {

				case MLIB_BILINEAR:
					if (zoomy < 2) {
						RETURN
						(mlib_ImageZoomBilinear_S32_3s);
					} else {
						RETURN
						(mlib_ImageZoomBilinear_S32_3);
					}
					break;

				case MLIB_BICUBIC:
				case MLIB_BICUBIC2:
					RETURN(mlib_ImageZoomBicubic_S32_3);
					break;

				default:
					return (MLIB_FAILURE);
				}
				break;

			    case 4:
				switch (filter) {

				case MLIB_BILINEAR:
					if (zoomy < 2) {
						RETURN
						(mlib_ImageZoomBilinear_S32_4s);
					} else {
						RETURN
						(mlib_ImageZoomBilinear_S32_4);
					}
					break;

				case MLIB_BICUBIC:
				case MLIB_BICUBIC2:
					RETURN(mlib_ImageZoomBicubic_S32_4);
					break;

				default:
					return (MLIB_FAILURE);
				}
				break;

			default:
				return (MLIB_FAILURE);

			}
		}
		break;

/* discard any other data types */
		default:
			return (MLIB_FAILURE);
		}
	}

	if (filter == MLIB_NEAREST && edge != MLIB_EDGE_SRC_EXTEND_INDEF) {
		return (MLIB_SUCCESS);
	}

	MLIB_EDGE_RULES
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
