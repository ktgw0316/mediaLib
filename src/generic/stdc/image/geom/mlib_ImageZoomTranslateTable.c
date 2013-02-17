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

#pragma ident	"@(#)mlib_ImageZoomTranslateTable.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomTranslateTable - image scaling using interpolation table
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageZoomTranslateTable(mlib_image       *dst,
 *                                               const mlib_image *src,
 *                                               mlib_d64         zoomx,
 *                                               mlib_d64         zoomy,
 *                                               mlib_d64         tx,
 *                                               mlib_d64         ty,
 *                                               const void       *table,
 *                                               mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      zoomx     X zoom factor.
 *      zoomy     Y zoom factor.
 *      tx        X translation.
 *      ty        Y translation.
 *      table     Pointer to interpolation table structure.
 *      edge      Type of edge condition.
 *
 * DESCRIPTION
 *                      xd = zoomx*xs + tx
 *                      yd = zoomy*ys + ty
 *
 *  The upper-left corner pixel of an image is located at (0.5, 0.5).
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
 *      They can have 1, 2, 3 or 4 channels. They can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 *      src image can not have width or height larger than 32767.
 */

#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageAffine.h>
#include <mlib_ImageInterpTable.h>
#include <mlib_ImageZoomTranslateTable.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageZoomTranslateTable = __mlib_ImageZoomTranslateTable

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageZoomTranslateTable) mlib_ImageZoomTranslateTable
    __attribute__((weak, alias("__mlib_ImageZoomTranslateTable")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	BUFF_SIZE	4096

/* *********************************************************** */

#define	INT_BITS	29

/* *********************************************************** */

static const fun_type_nw fun_array_nw[] = {
	mlib_ImageZoomTranslateTable_8nw,
	mlib_ImageZoomTranslateTable_16nw,
	mlib_ImageZoomTranslateTable_32nw,
	mlib_ImageZoomTranslateTable_F32nw,
	mlib_ImageZoomTranslateTable_D64nw,
	mlib_ImageZoomTranslateTable_u16nw,
};

/* *********************************************************** */

static const fun_type_ext fun_array_ext[] = {
	mlib_ImageZoomTranslateTable_8ext,
	mlib_ImageZoomTranslateTable_16ext,
	mlib_ImageZoomTranslateTable_32ext,
	mlib_ImageZoomTranslateTable_F32ext,
	mlib_ImageZoomTranslateTable_D64ext,
	mlib_ImageZoomTranslateTable_u16ext,
};

#define	CREATE_X_IND()                                            \
	                                                          \
/*                                                                \
 * calc x positions                                               \
 */                                                               \
	p_x_ind = x_ind =                                         \
	__mlib_malloc((srcWidth + 8 * kw) * sizeof (mlib_s32));   \
	if (p_x_ind == NULL) {                                    \
	    if (param_e->buff_malloc != NULL)                     \
		__mlib_free(param_e->buff_malloc);                \
	    if (param->buff_malloc != NULL)                       \
		__mlib_free(param->buff_malloc);                  \
	    return (MLIB_FAILURE);                                \
	}                                                         \
	xpos = 0;                                                 \
	for (i = 0; i < 4 * kw; i++)                              \
	    x_ind[i] = xpos;                                      \
	x_ind += 4 * kw;                                          \
	for (i = 0; i < srcWidth - 1; i++) {                      \
	    x_ind[i] = xpos;                                      \
	    xpos += nchan;                                        \
	}                                                         \
	                                                          \
	for (i = srcWidth - 1; i < srcWidth + 4 * kw; i++)        \
	    x_ind[i] = xpos

/* *********************************************************** */

#define	CREATE_X_EXT()                                                   \
	if (x_tab == NULL) {                                             \
	    x_tab = __mlib_malloc((dstWidth + 1) * sizeof (mlib_s32));   \
	    if (x_tab == NULL) {                                         \
		__mlib_free(p_x_ind);                                    \
		if (param_e->buff_malloc != NULL)                        \
		    __mlib_free(param_e->buff_malloc);                   \
		if (param->buff_malloc != NULL)                          \
		    __mlib_free(param->buff_malloc);                     \
		return (MLIB_FAILURE);                                   \
	    }                                                            \
	}                                                                \
	xpos = xStarts[yStart] - ws->x_move;                             \
	dx = ws->dx;                                                     \
	for (i = 0; i <= xRight_e - xLeft_e; i++) {                      \
	    x_tab[i] = xpos;                                             \
	    xpos += dx;                                                  \
	}
/* *********************************************************** */

#define	CREATE_X(xLeft, xRight)                                          \
	if (x_tab == NULL) {                                             \
	    x_tab = __mlib_malloc((dstWidth + 1) * sizeof (mlib_s32));   \
	    if (x_tab == NULL) {                                         \
		__mlib_free(p_x_ind);                                    \
		if (param_e->buff_malloc != NULL)                        \
		    __mlib_free(param_e->buff_malloc);                   \
		if (param->buff_malloc != NULL)                          \
		    __mlib_free(param->buff_malloc);                     \
		return (MLIB_FAILURE);                                   \
	    }                                                            \
	}                                                                \
	div = 1.0 / zoomx;                                               \
	dxs = ((xLeft) + 0.5 - tx) * div - (kw1 + 0.5);                  \
	for (i = 0; i <= (xRight) - (xLeft); i++) {                      \
	    tmp_dxs = dxs;                                               \
	    if (tmp_dxs < -kw)                                           \
		tmp_dxs = -kw;                                           \
	    else if (tmp_dxs > srcWidth - 1)                             \
		tmp_dxs = srcWidth - 1;                                  \
	    x_tab[i] = (mlib_s32)(tmp_dxs * (1 << x_shift));             \
	    dxs += div;                                                  \
	}

/* *********************************************************** */

mlib_status
mlib_ImageZoomTranslateTable_alltypes(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 zoomx,
    mlib_d64 zoomy,
    mlib_d64 tx,
    mlib_d64 ty,
    const void *table,
    mlib_edge edge)
{
	mlib_affine_param param[1];
	mlib_affine_param *cur_param;
	mlib_zoom_workspace ws[1];
	mlib_d64 buff_lcl[BUFF_SIZE / 8];
	mlib_type type;
	mlib_u8 *srcData, *dstData;
	mlib_s32 srcWidth, dstWidth, srcHeight, dstHeight;
	mlib_s32 srcStride, dstStride, nchan, elt_size = 1;
	mlib_s32 *leftEdges, *rightEdges, *xStarts, *yStarts;
	mlib_s32 *p_x_ind = NULL, *x_ind, *x_tab = NULL, xpos;
	mlib_u8 **lineAddr = NULL;
	mlib_s32 kw, kh, kw1, kh1;
	mlib_status res = MLIB_SUCCESS;
	fun_type_nw fun_nw = NULL;
	fun_type_ext fun_ext = NULL;
	mlib_interp_table *tbl = (mlib_interp_table *) table;
	mlib_d64 mtx[6], dxs, tmp_dxs, div;
	mlib_s32 i, x_shift, y_shift;
	mlib_s32 affine = 0, yStart;
	mlib_s32 xLeft_e, xRight_e, xLeft, xRight, dx;

	mtx[0] = zoomx;
	mtx[1] = 0;
	mtx[2] = tx;
	mtx[3] = 0;
	mtx[4] = zoomy;
	mtx[5] = ty;

	ws->zoomx = zoomx;
	ws->zoomy = zoomy;

/* check for obvious errors */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);

	if (zoomx <= 0 || zoomy <= 0)
		return (MLIB_OUTOFRANGE);

	MLIB_IMAGE_GET_ALL_PARAMS(src, type, nchan, srcWidth, srcHeight,
	    srcStride, srcData);
	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, nchan, dstWidth, dstHeight,
	    dstStride, dstData);

	kw = tbl->width;
	kh = tbl->height;
	kw1 = tbl->leftPadding;
	kh1 = tbl->topPadding;

	x_shift = INT_BITS - mlib_ilogb(srcWidth + kw);
	y_shift = INT_BITS - mlib_ilogb(srcHeight + kh);

	if (type == MLIB_BYTE)
		elt_size = 1;
	else if (type == MLIB_SHORT)
		elt_size = 2;
	else if (type == MLIB_INT)
		elt_size = 3;
	else if (type == MLIB_FLOAT)
		elt_size = 4;
	else if (type == MLIB_DOUBLE)
		elt_size = 5;
	else if (type == MLIB_USHORT)
		elt_size = 6;

	ws->type = type;
	ws->srcData = srcData;
	ws->dstData = dstData;
	ws->srcWidth = srcWidth;
	ws->srcHeight = srcHeight;
	ws->srcStride = srcStride;
	ws->dstStride = dstStride;
	ws->nchan = nchan;
	ws->edge = edge;
	ws->x_shift = x_shift;
	ws->y_shift = y_shift;
	ws->x_move = (kw1 << x_shift);
	ws->y_move = (kh1 << y_shift);
/* no blend */
	ws->alpha_shift = 0;

/* VIS version of non NULL */
	fun_nw = mlib_ImageZoomTranslate_GetFunc(ws, table);

	if (fun_ext == NULL) {
		fun_ext = fun_array_ext[elt_size - 1];
	}

	if (fun_nw == NULL) {
		fun_nw = fun_array_nw[elt_size - 1];
	}

/* NULL */
	STORE_PARAM(param, affine);
	STORE_PARAM(param, lineAddr);
	param->buff_malloc = NULL;

/* process internal pixels */

	res = mlib_AffineEdges(param, dst, src, buff_lcl, BUFF_SIZE,
	    kw, kh, kw1, kh1, edge, mtx, x_shift, y_shift);

	if (res != MLIB_SUCCESS)
		return (res);

	ws->yStart = param->yStart;
	ws->yFinish = param->yFinish;
	ws->max_xsize = param->max_xsize;
	ws->dx = param->dX;
	ws->dy = param->dY;

	LOAD_PARAM(param, lineAddr);
	LOAD_PARAM(param, leftEdges);
	LOAD_PARAM(param, rightEdges);
	LOAD_PARAM(param, xStarts);
	LOAD_PARAM(param, yStarts);

	if (edge == MLIB_EDGE_SRC_EXTEND)
		ws->y_move += (1 << (y_shift - 1));

	if ((ws->max_xsize) > 0) {
/* RTC */
		yStarts[(ws->yFinish) + 1] = 0;

		res = fun_nw(param->dstData, lineAddr,
		    leftEdges, rightEdges, xStarts, yStarts, ws, tbl);

		if (res != MLIB_SUCCESS) {
			if (param->buff_malloc != NULL)
				__mlib_free(param->buff_malloc);
			return (res);
		}
	}

/* process edge pixels */

	if (edge != MLIB_EDGE_DST_NO_WRITE && edge != MLIB_EDGE_SRC_PADDED) {
		mlib_affine_param param_e[1];

		param_e->buff_malloc = NULL;

		if (edge == MLIB_EDGE_DST_FILL_ZERO ||
		    edge == MLIB_EDGE_OP_NEAREST) {
			x_shift = 16;
			y_shift = 16;
		}

		STORE_PARAM(param_e, lineAddr);

		if (edge != MLIB_EDGE_SRC_EXTEND_INDEF) {
			res = mlib_AffineEdges(param_e, dst, src, NULL, 0,
			    kw, kh, kw1, kh1, -1, mtx, x_shift, y_shift);
		}

		if (res == MLIB_SUCCESS)
			switch (edge) {
			case MLIB_EDGE_DST_FILL_ZERO:
				mlib_ImageAffineEdgeZero(param, param_e, NULL);
				break;

			case MLIB_EDGE_OP_NEAREST:
				mlib_ImageAffineEdgeNearest(param, param_e);
				break;

			case MLIB_EDGE_SRC_EXTEND:
			case MLIB_EDGE_SRC_EXTEND_INDEF:

				ws->x_shift = x_shift;
				ws->x_move += (1 << (x_shift - 1));

				if (edge == MLIB_EDGE_SRC_EXTEND) {
					ws->yStart = param_e->yStart;
					ws->yFinish = param_e->yFinish;
					yStart = ws->yStart;
					ws->dx = param_e->dX;
					xLeft_e = param_e->leftEdges[yStart];
					xRight_e = param_e->rightEdges[yStart];
					cur_param = param_e;

				} else {
					cur_param = param;
					ws->yStart = param->yStart;
					yStart = ws->yStart;
					xLeft_e = 0;
					xRight_e = dstWidth - 1;
				}

				xLeft = param->leftEdges[param->yStart];
				xRight = param->rightEdges[param->yStart];

				if ((xLeft > xRight) ||
				    (param->yStart > param->yFinish)) {
					xLeft = xRight_e + 1;
					xRight = xRight_e;
				}

				if (((xRight_e - xLeft_e + 1) > 0) &&
				    (ws->yStart <= ws->yFinish)) {
					CREATE_X_IND();

					LOAD_PARAM(cur_param, lineAddr);
					LOAD_PARAM(cur_param, leftEdges);
					LOAD_PARAM(cur_param, xStarts);
					LOAD_PARAM(cur_param, yStarts);

/* RTC */
					yStarts[(ws->yFinish) + 1] = 0;

					if (edge == MLIB_EDGE_SRC_EXTEND) {
						CREATE_X_EXT()
					}

					ws->max_xsize = xLeft - xLeft_e;

					if ((ws->max_xsize) > 0) {
						if (edge !=
						    MLIB_EDGE_SRC_EXTEND) {
							CREATE_X(xLeft_e,
							    xLeft - 1);
						}

						leftEdges[0] = ws->max_xsize;
						leftEdges[1] = xLeft_e;

						res = fun_ext
						    (cur_param->dstData,
						    lineAddr, x_ind, leftEdges,
						    x_tab, xStarts, yStarts, ws,
						    tbl);
					}

					ws->max_xsize = xRight_e - xRight;

					if ((ws->max_xsize) > 0) {
						mlib_s32 shift = 0;

						if (edge !=
						    MLIB_EDGE_SRC_EXTEND) {
							CREATE_X(xRight + 1,
							    xRight_e);
						} else {
							shift =
							    xRight + 1 -
							    xLeft_e;
						}

						leftEdges[0] = ws->max_xsize;
						leftEdges[1] = xRight + 1;

						res = fun_ext
						    (cur_param->dstData,
						    lineAddr, x_ind, leftEdges,
						    x_tab + shift, xStarts,
						    yStarts, ws, tbl);
					}

					__mlib_free(p_x_ind);

					if (x_tab != NULL)
						__mlib_free(x_tab);
				}
				break;
			default:
				res = MLIB_FAILURE;
			}

		if (param_e->buff_malloc != NULL)
			__mlib_free(param_e->buff_malloc);
	}

	if (param->buff_malloc != NULL)
		__mlib_free(param->buff_malloc);

	return (res);
}

/* *********************************************************** */

mlib_status
__mlib_ImageZoomTranslateTable(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 zoomx,
    mlib_d64 zoomy,
    mlib_d64 tx,
    mlib_d64 ty,
    const void *table,
    mlib_edge edge)
{
	mlib_type type = mlib_ImageGetType(dst);

	if (type != MLIB_BYTE && type != MLIB_SHORT && type != MLIB_USHORT &&
	    type != MLIB_INT) {
		return (MLIB_FAILURE);
	}

	return mlib_ImageZoomTranslateTable_alltypes(dst, src, zoomx, zoomy, tx,
	    ty, table, edge);
}

/* *********************************************************** */
