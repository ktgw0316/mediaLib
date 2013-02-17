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

#pragma ident	"@(#)mlib_ImageAffineTable.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageAffineTable - image affine transformation using
 *                              interpolation table
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageAffineTable(mlib_image       *dst,
 *                                        const mlib_image *src,
 *                                        const mlib_d64   *mtx,
 *                                        const void       *table,
 *                                        mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image
 *      src       Pointer to source image
 *      mtx       Transformation matrix, where
 *                  mtx[0] holds a;  mtx[1] holds b;
 *                  mtx[2] holds tx; mtx[3] holds c;
 *                  mtx[4] holds d;  mtx[5] holds ty.
 *      table     Pointer to interpolation table
 *      edge      Type of edge condition
 *
 * DESCRIPTION
 *                      xd = a*xs + b*ys + tx
 *                      yd = c*xs + d*ys + ty
 *
 *  The upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *  The edge condition can be one of the following:
 *    MLIB_EDGE_DST_NO_WRITE  (default)
 *    MLIB_EDGE_DST_FILL_ZERO
 *    MLIB_EDGE_OP_NEAREST
 *    MLIB_EDGE_SRC_EXTEND
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
#include <mlib_ImageCreate.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageInterpTable.h>
#include <mlib_ImageAffine.h>
#include <mlib_ImageAffineTable.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageAffineTable = __mlib_ImageAffineTable

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageAffineTable) mlib_ImageAffineTable
    __attribute__((weak, alias("__mlib_ImageAffineTable")));

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
	mlib_ImageAffineTable_8nw,
	mlib_ImageAffineTable_8_3nw,
	mlib_ImageAffineTable_16nw,
	mlib_ImageAffineTable_16_3nw,
	mlib_ImageAffineTable_32nw,
	mlib_ImageAffineTable_32_3nw,
	mlib_ImageAffineTable_F32nw,
	mlib_ImageAffineTable_F32_3nw,
	mlib_ImageAffineTable_D64nw,
	mlib_ImageAffineTable_D64_3nw,
	mlib_ImageAffineTable_u16nw,
	mlib_ImageAffineTable_u16_3nw
};

static const fun_type_ext fun_array_ext[] = {
	mlib_ImageAffineTable_8ext,
	mlib_ImageAffineTable_16ext,
	mlib_ImageAffineTable_32ext,
	mlib_ImageAffineTable_F32ext,
	mlib_ImageAffineTable_D64ext,
	mlib_ImageAffineTable_u16ext
};

/* *********************************************************** */

#define	CALC_TABLES(srcData, srcStride)                         \
	                                                        \
	/*                                                      \
	 * calc line addresses                                  \
	 */                                                     \
	srcLinePtr = (mlib_u8 *)srcData;                        \
	lineAddr -= 2 * kh;                                     \
	for (i = 0; i < 2 * kh; i++)                            \
	    lineAddr[i] = srcLinePtr;                           \
	lineAddr += 2 * kh;                                     \
	for (i = 0; i < srcHeight - 1; i++) {                   \
	    lineAddr[i] = srcLinePtr;                           \
	    srcLinePtr += srcStride;                            \
	}                                                       \
	                                                        \
	for (i = srcHeight - 1; i < srcHeight + 2 * kh; i++)    \
	    lineAddr[i] = srcLinePtr

/* *********************************************************** */

#define	CALC_X_IND()                                              \
	                                                          \
	/*                                                        \
	 * calc x positions                                       \
	 */                                                       \
	pbuff = x_ind =                                           \
	__mlib_malloc((srcWidth + 4 * kw) * sizeof (mlib_s32));   \
	xpos = 0;                                                 \
	for (i = 0; i < 2 * kw; i++)                              \
	    x_ind[i] = xpos;                                      \
	x_ind += 2 * kw;                                          \
	for (i = 0; i < srcWidth - 1; i++) {                      \
	    x_ind[i] = xpos;                                      \
	    xpos += nchan;                                        \
	}                                                         \
	                                                          \
	for (i = srcWidth - 1; i < srcWidth + 2 * kw; i++)        \
	    x_ind[i] = xpos

/* *********************************************************** */

static mlib_status
mlib_ImageDataTypeConvert3(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_u8 *sl;
	mlib_f32 *dl;
	mlib_s32 srcWidth, dstWidth, dstHeight;
	mlib_s32 srcStride, dstStride;
	mlib_s32 i, j;

	srcWidth = mlib_ImageGetWidth(src);
	srcStride = mlib_ImageGetStride(src);
	sl = (void *)mlib_ImageGetData(src);

	dstWidth = mlib_ImageGetWidth(dst);
	dstHeight = mlib_ImageGetHeight(dst);
	dstStride = mlib_ImageGetStride(dst);
	dl = (void *)mlib_ImageGetData(dst);

	dstStride >>= 2;

	for (j = 0; j < dstHeight; j++) {
		mlib_f32 *dl1 = dl + 2 * srcWidth;

		for (i = 0; i < dstWidth; i++) {
			dl[2 * i] = mlib_U82F32[sl[3 * i]];
			dl[2 * i + 1] = mlib_U82F32[sl[3 * i + 1]];
			dl1[i] = mlib_U82F32[sl[3 * i + 2]];
		}

		sl += srcStride;
		dl += dstStride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

fun_type_nw
mlib_ImageAffine_ConvertImage(
    const mlib_image *src,
    mlib_affine_workspace * ws,
    mlib_interp_table * tbl)
{
	mlib_s32 nchan = ws->nchan;
	mlib_s32 srcWidth = ws->srcWidth;
	mlib_s32 srcHeight = ws->srcHeight;
	mlib_image new_src[1];
	void *srcData;
	mlib_s32 srcStride;

	if (ws->type == MLIB_BYTE && ws->edge == MLIB_EDGE_DST_NO_WRITE) {
		if (nchan == 3 && ((tbl->width | tbl->height) & 1))
			return (NULL);

		srcStride = 4 * nchan * srcWidth;

		if (srcStride & 7)
			srcStride += 4;
		srcData = __mlib_malloc(srcHeight * srcStride);

		if (srcData == NULL)
			return (NULL);

		ws->srcData = srcData;
		ws->srcStride = srcStride;
		mlib_ImageSet(new_src, MLIB_FLOAT, nchan, srcWidth, srcHeight,
		    srcStride, ws->srcData);

		if (nchan == 3)
			mlib_ImageDataTypeConvert3(new_src, src);
		else
			__mlib_ImageDataTypeConvert(new_src, src);

		return ((fun_type_nw)&mlib_ImageAffineTable_F32nw);
	}

	return (NULL);
}

/* *********************************************************** */

#define	CHECK_STATUS(status)                                    \
	if (status != MLIB_SUCCESS) {                           \
	    if (param->buff_malloc != NULL)                     \
		__mlib_free(param->buff_malloc);                \
	    if (param_e->buff_malloc != NULL)                   \
		__mlib_free(param_e->buff_malloc);              \
	    if (pbuff != NULL)                                  \
		__mlib_free(pbuff);                             \
	    return (status);                                    \
	}

/* *********************************************************** */

mlib_status
mlib_ImageAffineTable_alltypes(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *mtx,
    const void *table,
    mlib_edge edge)
{
	mlib_affine_param param[1];
	mlib_affine_workspace ws[1];
	mlib_type type;
	mlib_u8 *srcLinePtr, *srcData, *dstData;
	mlib_s32 srcWidth, srcHeight;
	mlib_s32 srcStride, dstStride, nchan;
	mlib_s32 *pbuff = NULL, *x_ind, xpos;
	const mlib_u8 **lineAddr = NULL;
	mlib_d64 buff_lcl[BUFF_SIZE / 8];
	mlib_s32 kw, kh, kw1, kh1;
	mlib_status res;
	fun_type_nw fun_nw = NULL;
	fun_type_ext fun_ext = NULL;
	mlib_interp_table *tbl = (mlib_interp_table *) table;
	mlib_d64 div, ddx, ddy;
	mlib_s32 i, x_shift, y_shift;
	mlib_s32 elt_size = 1, int_dx, int_dy, vis_ver;
	mlib_s32 affine = 1;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);

	MLIB_IMAGE_GET_ALL_PARAMS(src, type, nchan, srcWidth, srcHeight,
	    srcStride, srcData);
	dstStride = mlib_ImageGetStride(dst);
	dstData = (void *)mlib_ImageGetData(dst);

	kw = tbl->width;
	kh = tbl->height;
	kw1 = tbl->leftPadding;
	kh1 = tbl->topPadding;

	x_shift = INT_BITS - mlib_ilogb((mlib_d64)(srcWidth + kw));
	y_shift = INT_BITS - mlib_ilogb((mlib_d64)(srcHeight + kh));

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
		elt_size = 0;

	ws->type = elt_size;
	ws->srcData = srcData;
	ws->dstData = dstData;
	ws->srcWidth = srcWidth;
	ws->srcHeight = srcHeight;
	ws->srcStride = srcStride;
	ws->dstStride = dstStride;
	ws->nchan = nchan;
	ws->edge = edge;
	ws->x_shift0 = x_shift;
	ws->y_shift = y_shift;
	ws->x_move = (kw1 << x_shift);
	ws->y_move = (kh1 << y_shift);
	ws->affine_mask = 0;

	vis_ver = 0;
/* VIS version if no NULL */
	fun_nw = mlib_ImageAffine_GetFunc(ws, tbl);

	if (fun_nw != NULL)
		vis_ver = 1;

	if ((fun_nw == NULL) && (edge != MLIB_EDGE_SRC_EXTEND)) {
/* for U8 via F32 */
		fun_nw = mlib_ImageAffine_ConvertImage(src, ws, tbl);
	}

	if (!elt_size)
		elt_size = 6;
	fun_ext = fun_array_ext[elt_size - 1];

	if (fun_nw == NULL) {
		if (nchan == 3) {
			fun_nw = fun_array_nw[2 * elt_size - 1];
		} else {
			fun_nw = fun_array_nw[2 * elt_size - 2];
		}
	}

	div = mtx[0] * mtx[4] - mtx[1] * mtx[3];
	ddx = mtx[4] / div;
	ddy = -mtx[3] / div;
	ws->dx = (mlib_s32)(ddx * (1 << x_shift));
	ws->dy = (mlib_s32)(ddy * (1 << y_shift));

/*
 *     STORE_PARAM(param, lineAddr);
 */
/* NULL */
	STORE_PARAM(param, affine);
	param->lineAddr = (mlib_u8 **)lineAddr;
	param->buff_malloc = NULL;

/*
 * process internal pixels
 */

	if ((edge != MLIB_EDGE_SRC_EXTEND) || vis_ver == 1) {
		res = mlib_AffineEdges(param, dst, src, buff_lcl, BUFF_SIZE,
		    kw, kh, kw1, kh1, edge, mtx, x_shift, y_shift);

		if (res != MLIB_SUCCESS) {
			if (ws->srcData != srcData)
				__mlib_free(ws->srcData);
			return (res);
		}

/*
 *     LOAD_PARAM(param, lineAddr);
 */
		lineAddr = (const mlib_u8 **)param->lineAddr;

		if (ws->srcData != srcData) {
/* U8 via F32 used */
			CALC_TABLES(ws->srcData, ws->srcStride);
		}

		ws->yStart = param->yStart;
		ws->yFinish = param->yFinish;
		ws->max_xsize = param->max_xsize;

		if (ws->max_xsize > 0) {
			res = fun_nw(param->dstData, lineAddr,
			    param->leftEdges, param->rightEdges,
			    param->xStarts, param->yStarts, ws, tbl);
		}

		if (ws->srcData != srcData)
			__mlib_free(ws->srcData);

		if (res != MLIB_SUCCESS) {
			if (param->buff_malloc != NULL)
				__mlib_free(param->buff_malloc);
			return (MLIB_FAILURE);
		}
	}

/*
 * process edge pixels
 */

	if (edge != MLIB_EDGE_DST_NO_WRITE && edge != MLIB_EDGE_SRC_PADDED) {
		mlib_affine_param param_e[1];

		if (edge == MLIB_EDGE_DST_FILL_ZERO ||
		    edge == MLIB_EDGE_OP_NEAREST) {
			x_shift = MLIB_SHIFT;
			y_shift = MLIB_SHIFT;
		}

/*
 *     STORE_PARAM(param_e, lineAddr);
 */
		param_e->lineAddr = (mlib_u8 **)lineAddr;

		res = mlib_AffineEdges(param_e, dst, src, NULL, 0,
		    kw, kh, kw1, kh1, -1, mtx, x_shift, y_shift);

		if (res != MLIB_SUCCESS) {
			if (param->buff_malloc != NULL)
				__mlib_free(param->buff_malloc);
			return (res);
		}

/*
 *     LOAD_PARAM(param_e, lineAddr);
 */
		lineAddr = (const mlib_u8 **)param_e->lineAddr;

		switch (edge) {
		    case MLIB_EDGE_DST_FILL_ZERO:
			    mlib_ImageAffineEdgeZero(param, param_e, NULL);
			    break;

		    case MLIB_EDGE_OP_NEAREST:
			    mlib_ImageAffineEdgeNearest(param, param_e);
			    break;

		    case MLIB_EDGE_SRC_EXTEND:
			    CALC_X_IND();

			    ws->yStart = param_e->yStart;
			    ws->yFinish = param_e->yFinish;
			    ws->max_xsize = param_e->max_xsize;
			    ws->x_move += (1 << (x_shift - 1));
			    ws->y_move += (1 << (y_shift - 1));

			    if (vis_ver == 0) {
				    mlib_s32 *leftEdgesE = param_e->leftEdges;
				    mlib_s32 *rightEdgesE = param_e->rightEdges;
				    mlib_s32 *xStartsE = param_e->xStarts;
				    mlib_s32 *yStartsE = param_e->yStarts;

				    if (param_e->max_xsize > 0) {
					    res =
						fun_ext(param_e->dstData,
						lineAddr, x_ind, leftEdgesE,
						rightEdgesE, xStartsE, yStartsE,
						ws, tbl);
					    CHECK_STATUS(res);
				    }
			    } else if (param_e->max_xsize > 0) {
				    mlib_s32 *leftEdgesI = param->leftEdges;
				    mlib_s32 *rightEdgesI = param->rightEdges;
				    mlib_s32 *xStartsI = param->xStarts;
				    mlib_s32 *yStartsI = param->yStarts;
				    mlib_s32 *leftEdgesE = param_e->leftEdges;
				    mlib_s32 *rightEdgesE = param_e->rightEdges;
				    mlib_s32 *xStartsE = param_e->xStarts;
				    mlib_s32 *yStartsE = param_e->yStarts;

				    if (param->max_xsize > 0) {
					    mlib_s32 tmp;

					    ws->yStart = param_e->yStart;
					    ws->yFinish = param->yStart - 1;
					    res =
						fun_ext(param_e->dstData,
						lineAddr, x_ind, leftEdgesE,
						rightEdgesE, xStartsE, yStartsE,
						ws, tbl);
					    CHECK_STATUS(res);

					    ws->yStart = param->yStart;
					    ws->yFinish = param->yFinish;
					    int_dx = ws->dx;
					    int_dy = ws->dy;

					    tmp = (ws->yFinish);
					    for (i = (ws->yStart);
						i <= tmp; i++) {
						    if (leftEdgesI[i] <=
							rightEdgesI[i]) {
							    leftEdgesI[i] -= 1;
							    rightEdgesI[i] += 1;
						    } else {
							    leftEdgesI[i] =
								leftEdgesE[i] -
								1;
							    rightEdgesI[i] =
								leftEdgesE[i];
						    }

						    xStartsI[i] =
							xStartsE[i] +
							int_dx *
							(rightEdgesI[i] -
							leftEdgesE[i]);
						    yStartsI[i] =
							yStartsE[i] +
							int_dy *
							(rightEdgesI[i] -
							leftEdgesE[i]);
					    }

					    res =
						fun_ext(param->dstData,
						lineAddr, x_ind, leftEdgesE,
						leftEdgesI, xStartsE, yStartsE,
						ws, tbl);
					    CHECK_STATUS(res);

					    res =
						fun_ext(param->dstData,
						lineAddr, x_ind, rightEdgesI,
						rightEdgesE, xStartsI, yStartsI,
						ws, tbl);
					    CHECK_STATUS(res);

					    ws->yStart = (param->yFinish) + 1;
					    ws->yFinish = param_e->yFinish;

					    res =
						fun_ext(dstData +
						(param->yFinish) * dstStride,
						lineAddr, x_ind, leftEdgesE,
						rightEdgesE, xStartsE, yStartsE,
						ws, tbl);
					    CHECK_STATUS(res);
				    } else {
					    res =
						fun_ext(param_e->dstData,
						lineAddr, x_ind, leftEdgesE,
						rightEdgesE, xStartsE, yStartsE,
						ws, tbl);
					    CHECK_STATUS(res);
				    }
			    }

/* buffer for x_ind */
			    if (pbuff != NULL)
				    __mlib_free(pbuff);
			    break;
		    default:
			    res = MLIB_FAILURE;
			    CHECK_STATUS(res);
		}

		if (param_e->buff_malloc != NULL)
			__mlib_free(param_e->buff_malloc);
	}

	if (param->buff_malloc != NULL)
		__mlib_free(param->buff_malloc);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_ImageAffineTable(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *mtx,
    const void *table,
    mlib_edge edge)
{
	mlib_type type = mlib_ImageGetType(dst);

	if (type != MLIB_BYTE && type != MLIB_SHORT && type != MLIB_USHORT &&
	    type != MLIB_INT) {
		return (MLIB_FAILURE);
	}

	return (mlib_ImageAffineTable_alltypes(dst, src, mtx, table, edge));
}

/* *********************************************************** */
