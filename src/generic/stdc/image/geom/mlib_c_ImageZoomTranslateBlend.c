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

#pragma ident	"@(#)mlib_c_ImageZoomTranslateBlend.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomTranslateBlend(), internal pixels.
 *
 */

#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageFilters.h>

/* *********************************************************** */

#define	_MLIB_C_VER_

#include <mlib_ImageZoom.h>

/* *********************************************************** */

static const mlib_zoomblend_fun_type mlib_zoom_nn_funs[] = {
	mlib_ImageZoom_U8_33_Nearest,
	mlib_ImageZoom_U8_43_Nearest,
	mlib_ImageZoom_U8_34_Nearest,
	mlib_ImageZoom_U8_44_Nearest,
};

/* *********************************************************** */

static const mlib_d64 mlib_zoom_bl_level[] = {
	1.0, 1.0, 1.0, 1.0
};

static const mlib_zoomblend_bl_type mlib_zoom_bl_lo[] = {
	mlib_c_ImageZoomBilinear_U8_33,
	mlib_c_ImageZoomBilinear_U8_43,
	mlib_c_ImageZoomBilinear_U8_34,
	mlib_c_ImageZoomBilinear_U8_44,
};

/* *********************************************************** */

static const mlib_d64 mlib_zoom_bc_level[] = {
	0.5, 0.5, 0.5, 0.5
};

static const mlib_zoomblend_bc_type mlib_zoom_bc_lo[] = {
	mlib_c_ImageZoomBicubic_U8_33,
	mlib_c_ImageZoomBicubic_U8_43,
	mlib_c_ImageZoomBicubic_U8_34,
	mlib_c_ImageZoomBicubic_U8_44,
};

static const mlib_zoom_fun2type mlib_zoom_bc_hi[] = {
	mlib_c_ImageZoomBicubic_U8_3,
	mlib_c_ImageZoomBicubic_U8_4,
	mlib_c_ImageZoomBicubic_U8_3,
	mlib_c_ImageZoomBicubic_U8_4,
};

/* *********************************************************** */

mlib_status
mlib_ImageZoomTranslateBlend_intern(
    mlib_work_image * param,
    mlib_filter filter,
    mlib_d64 zoomy,
    mlib_s32 t_ind)
{
	const mlib_f32 *flt_table;

	switch (filter) {
	case MLIB_NEAREST:
		return mlib_zoom_nn_funs[t_ind] (param, param->current->sp,
		    param->current->dp);

	case MLIB_BILINEAR:
		if (zoomy >= mlib_zoom_bl_level[t_ind] && t_ind == 0) {
			return (mlib_c_ImageZoomBilinear_U8_3to34(param));
		} else {
			return (mlib_zoom_bl_lo[t_ind] (param, param->alp_ind));
		}

	case MLIB_BICUBIC:
	case MLIB_BICUBIC2:
		if (filter == MLIB_BICUBIC) {
			flt_table = mlib_filters_u8f_bc;
		} else {
			flt_table = mlib_filters_u8f_bc2;
		}
		if (zoomy < mlib_zoom_bc_level[t_ind] && (!(t_ind & 1))) {
			return mlib_zoom_bc_lo[t_ind] (param, flt_table,
			    param->alp_ind);
		} else {
			return (mlib_zoom_bc_hi[t_ind] (param, flt_table));
		}

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */
