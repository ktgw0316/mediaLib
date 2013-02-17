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

#pragma ident	"@(#)mlib_ImageMedianFilterFunc.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *   Internal functions for mlib_ImageMedianFilter*
 */

#include <mlib_image.h>
#include <mlib_ImageCreate.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageClipping.h>
#include <mlib_ImageMedianFilter.h>
#include <mlib_ImageConvEdge.h>

/* *********************************************************** */

mlib_status
mlib_ImageFilterClearEdge(
    mlib_image *dst,
    mlib_filter_state *fs)
{
	mlib_s32 zero[4] = { 0, 0, 0, 0 };
	mlib_d64 dzero[4] = { 0, 0, 0, 0 };
	mlib_type type = mlib_ImageGetType(dst);

	if (type == MLIB_FLOAT || type == MLIB_DOUBLE) {
		return mlib_ImageConvClearEdge_Fp(dst, fs->dx_l, fs->dx_r,
		    fs->dy_t, fs->dy_b, dzero, fs->cmask);
	} else {
		return mlib_ImageConvClearEdge(dst, fs->dx_l, fs->dx_r,
		    fs->dy_t, fs->dy_b, zero, fs->cmask);
	}
}

/* *********************************************************** */

mlib_status
mlib_ImageFilterCopyEdge(
    mlib_image *dst,
    const mlib_image *src,
    mlib_filter_state *fs)
{
	mlib_type type = mlib_ImageGetType(dst);

	if (type == MLIB_FLOAT || type == MLIB_DOUBLE) {
		return mlib_ImageConvCopyEdge_Fp(dst, src, fs->dx_l, fs->dx_r,
		    fs->dy_t, fs->dy_b, fs->cmask);
	} else {
		return mlib_ImageConvCopyEdge(dst, src, fs->dx_l, fs->dx_r,
		    fs->dy_t, fs->dy_b, fs->cmask);
	}
}

/* *********************************************************** */

void
mlib_ImageGetEdges(
    mlib_image *dst_e,
    mlib_image *src_e,
    const mlib_image *dst,
    const mlib_image *src,
    mlib_filter_state *fs)
{
	mlib_type type;
	mlib_s32 m = fs->m, n = fs->n;
	mlib_s32 kw1 = (m - 1) / 2, kw2 = m - 1 - kw1;
	mlib_s32 kh1 = (n - 1) / 2, kh2 = n - 1 - kh1;
	mlib_s32 slb, dlb, nchan, bchan, type_sh = 0, i;
	mlib_s32 src_wid, src_hgt, dst_wid, dst_hgt;
	mlib_s32 iw, ih, dw, dh;
	mlib_s32 dx_l, dx_r, dy_t, dy_b, old_dx_l, old_dy_t;
	mlib_u8 *sa, *da;
	void **psl;
	mlib_u8 *sp;
	mlib_s32 *x_off, off;
	mlib_image dst_i[1], src_i[1];
	mlib_s32 edg_sizes[8];
	mlib_status ret;

	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, nchan, dst_wid, dst_hgt, dlb, da);
	MLIB_IMAGE_GET_ALL_PARAMS(src, type, nchan, src_wid, src_hgt, slb, sa);
	fs->slb = slb;
	fs->dlb = dlb;
	fs->nchan = nchan;
	switch (type) {
	case MLIB_BIT:
	case MLIB_BYTE:
		fs->type_sh = type_sh = 0;
		break;
	case MLIB_SHORT:
	case MLIB_USHORT:
		fs->type_sh = type_sh = 1;
		break;
	case MLIB_INT:
	case MLIB_FLOAT:
		fs->type_sh = type_sh = 2;
		break;
	case MLIB_DOUBLE:
		fs->type_sh = type_sh = 3;
	}

	fs->bchan = bchan = nchan << type_sh;
	fs->slp = slb >> type_sh;
	fs->dlp = dlb >> type_sh;

	ret = mlib_ImageClippingMxN(dst_i, src_i, dst_e, src_e, edg_sizes,
		dst, src, m, n, kw1, kh1);

	if (ret != MLIB_SUCCESS)
		return;

	dx_l = edg_sizes[0];
	dx_r = edg_sizes[1];
	dy_t = edg_sizes[2];
	dy_b = edg_sizes[3];

	if (dx_l + dx_r + dy_t + dy_b == 0)
		fs->edge = MLIB_EDGE_DST_NO_WRITE;

	dw = mlib_ImageGetWidth(dst_e);
	dh = mlib_ImageGetHeight(dst_e);
	iw = dw - dx_l - dx_r;
	ih = dh - dy_t - dy_b;

	psl = (void **)fs->pbuff;
	fs->_psl = psl + 2 * kh1;
	x_off = (mlib_s32 *)(psl + 2 * n + ih);
	fs->_x_off = x_off + 2 * kw1;

/* prepare pointers to internal and edge lines */
	sp = (mlib_u8*)mlib_ImageGetData(src_i) + kw1 * bchan;

	for (i = 0; i < kh1; i++) {
		(*psl++) = sp;
	}

	for (i = 0; i < kh1 + ih + kh2 - 1; i++) {
		(*psl++) = sp;
		sp += slb;
	}

	for (i = 0; i < kh2 + 3; i++) {
		(*psl++) = sp;
	}

/* prepare x offsets on internal and edge columns */
	off = -kw1 * nchan;

	for (i = 0; i < kw1; i++) {
		(*x_off++) = off;
	}

	for (i = 0; i < kw1 + iw + kw2 - 1; i++) {
		(*x_off++) = off;
		off += nchan;
	}

	for (i = 0; i < kw2 + 3; i++) {
		(*x_off++) = off;
	}

	old_dx_l = dx_l;
	old_dy_t = dy_t;

/* correct edges */

	if (iw < 0) {
		dx_r += iw;
		iw = 0;

		if (dx_r < 0) {
			dx_l += dx_r;
			dx_r = 0;
		}
	}

	if (ih < 0) {
		dy_b += ih;
		ih = 0;

		if (dy_b < 0) {
			dy_t += dy_b;
			dy_b = 0;
		}
	}

	fs->_x_off += dx_l - old_dx_l;
	fs->_psl += dy_t - old_dy_t;

	fs->xsize = iw;
	fs->ysize = ih;
	fs->wsize = iw * nchan;
	fs->pdst = (mlib_u8 *)mlib_ImageGetData(dst_e) + dy_t *
					dlb + dx_l * bchan;

	fs->dx_l = dx_l;
	fs->dx_r = dx_r;
	fs->dy_t = dy_t;
	fs->dy_b = dy_b;

	fs->kw1 = m / 2;
	fs->kh1 = n / 2;
	fs->m1_nchan = (fs->kw1) * nchan;
	fs->m_nchan = 2 * (fs->m1_nchan) + nchan;
}

/* *********************************************************** */

void
mlib_ImageMedianFilter_row(
    mlib_filter_state *fs)
{
	mlib_type type = fs->type;
	mlib_s32 n = fs->n;
	mlib_s32 xsize = fs->xsize, ysize = fs->ysize;
	mlib_s32 dlb = fs->dlb, tlb = xsize * fs->bchan;
	mlib_u8 *pdst = fs->pdst, *dl, *p_tdst = NULL;
	mlib_s32 i, y_off0, y_off1;
	void **psl = fs->_psl - (n / 2);
	mlib_median_fun_type row_fun = NULL;

	switch (type) {
	case MLIB_BYTE:
		row_fun = fs->row_funs[(fs->mmask)];
		break;
	case MLIB_SHORT:
		row_fun = fs->row_funs[(fs->mmask) + 4];
		break;
	case MLIB_USHORT:
		row_fun = fs->row_funs[(fs->mmask) + 8];
		break;
	case MLIB_INT:
		row_fun = fs->row_funs[(fs->mmask) + 12];
		break;
	case MLIB_FLOAT:
		row_fun = fs->row_funs[(fs->mmask) + 16];
		break;
	case MLIB_DOUBLE:
		row_fun = fs->row_funs[(fs->mmask) + 20];
		break;
	default:
		return;
	}

	if (!xsize)
		return;
	fs->size = xsize;

	y_off0 = 0;
	y_off1 = ysize;

	if (fs->edge == MLIB_EDGE_SRC_EXTEND) {
		y_off0 -= fs->dy_t;
		y_off1 += fs->dy_b;
		ysize = y_off1 - y_off0;
	}

	if (fs->mn_version && fs->mmask == MLIB_MEDIAN_MASK_RECT_SEPARABLE) {
		p_tdst = __mlib_malloc(tlb * (ysize + n));
	}

	if (p_tdst != NULL) {
		mlib_s32 bchan = fs->bchan;
		mlib_median_sep_fun_type seprow_fun = NULL, sepcol_fun = NULL;

		switch (type) {
		case MLIB_BYTE:
			seprow_fun = mlib_median_seprow_MxN_funs[0];
			sepcol_fun = mlib_median_sepcol_MxN_funs[0];
			break;
		case MLIB_SHORT:
			seprow_fun = mlib_median_seprow_MxN_funs[1];
			sepcol_fun = mlib_median_sepcol_MxN_funs[1];
			break;
		case MLIB_USHORT:
			seprow_fun = mlib_median_seprow_MxN_funs[2];
			sepcol_fun = mlib_median_sepcol_MxN_funs[2];
			break;
		case MLIB_INT:
			seprow_fun = mlib_median_seprow_MxN_funs[3];
			sepcol_fun = mlib_median_sepcol_MxN_funs[3];
			break;
		case MLIB_FLOAT:
			seprow_fun = mlib_median_seprow_MxN_funs[4];
			sepcol_fun = mlib_median_sepcol_MxN_funs[4];
			break;
		case MLIB_DOUBLE:
			seprow_fun = mlib_median_seprow_MxN_funs[5];
			sepcol_fun = mlib_median_sepcol_MxN_funs[5];
			break;
		default:
			__mlib_free(p_tdst);
			return;
		}

		if (ysize) {
			dl = p_tdst;
			for (i = y_off0; i < y_off1 + (n - 1); i++) {
				seprow_fun(dl, psl[i], fs);
				dl += tlb;
			}

			pdst += y_off0 * dlb;
			fs->size = ysize;

			for (i = 0; i < xsize; i++) {
				sepcol_fun(pdst + i * bchan, p_tdst + i * bchan,
				    fs);
			}
		}

		__mlib_free(p_tdst);
	} else {

		if (fs->mmask == MLIB_MEDIAN_MASK_RECT_SEPARABLE) {
			mlib_median_fun_type initrow_fun = NULL;

			switch (type) {
			case MLIB_BYTE:
				initrow_fun = fs->init_funs[0];
				break;
			case MLIB_SHORT:
				initrow_fun = fs->init_funs[1];
				break;
			case MLIB_USHORT:
				initrow_fun = fs->init_funs[2];
				break;
			case MLIB_INT:
				initrow_fun = fs->init_funs[3];
				break;
			case MLIB_FLOAT:
				initrow_fun = fs->init_funs[4];
				break;
			case MLIB_DOUBLE:
				initrow_fun = fs->init_funs[5];
				break;
			default:
				return;
			}

			for (i = y_off0; i < y_off0 + (n - 1); i++) {
				initrow_fun(NULL, psl + i, fs);
			}
		}

		for (i = y_off0; i < y_off1; i++) {
			row_fun(pdst + i * dlb, psl + i, fs);
		}
	}
}

/* *********************************************************** */

mlib_status
mlib_ImageMedianFilter_col(
    mlib_filter_state *fs)
{
	mlib_type type = fs->type;
	mlib_s32 xsize = fs->xsize, ysize = fs->ysize;
	mlib_s32 bchan = fs->bchan, dlb = fs->dlb;
	mlib_u8 *pdst = fs->pdst, *dl;
	void **psl = fs->_psl - fs->kh1;
	mlib_s32 *x_off = fs->_x_off - fs->kw1;
	mlib_s32 dx_l = fs->dx_l, dx_r = fs->dx_r, dy_t = fs->dy_t, dy_b =
	    fs->dy_b, i;
	mlib_median_fun_type col_fun = NULL;

	switch (type) {
	case MLIB_BYTE:
		col_fun = fs->col_funs[(fs->mmask)];
		break;
	case MLIB_SHORT:
		col_fun = fs->col_funs[(fs->mmask) + 4];
		break;
	case MLIB_USHORT:
		col_fun = fs->col_funs[(fs->mmask) + 8];
		break;
	case MLIB_INT:
		col_fun = fs->col_funs[(fs->mmask) + 12];
		break;
	case MLIB_FLOAT:
		col_fun = fs->col_funs[(fs->mmask) + 16];
		break;
	case MLIB_DOUBLE:
		col_fun = fs->col_funs[(fs->mmask) + 20];
		break;
	default:
		return (MLIB_FAILURE);
	}

	fs->size = ysize + dy_t + dy_b;
	psl -= dy_t;

	if (fs->size == 0)
		return (MLIB_SUCCESS);

	dl = pdst - dlb * dy_t;

	for (i = -dx_l; i < 0; i++) {
		fs->x_off = x_off + i;
		col_fun(dl + i * bchan, psl, fs);
	}

	for (i = xsize; i < xsize + dx_r; i++) {
		fs->x_off = x_off + i;
		col_fun(dl + i * bchan, psl, fs);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
