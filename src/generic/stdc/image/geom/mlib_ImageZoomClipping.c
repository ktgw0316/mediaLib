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

#pragma ident	"@(#)mlib_ImageZoomClipping.c	9.2	07/10/09 SMI"

#include <mlib_image.h>
#include <mlib_ImageZoom.h>
#include <mlib_SysMath.h>

/* *********************************************************** */

#define	STOP_SCALE	(1 - 1.0e-14)

/* *********************************************************** */

#define	SET_PARAMETRS(current, xs_left, xs_right, ys_top,                   \
	ys_bot, dlt, xstart, ystart)                                        \
	{                                                                   \
	    mlib_d64 xdstart, xdstop, ydstart, ydstop, xstop, ystop;        \
	                                                                    \
	    xstart = xs_left;                                               \
	    xstop  = (src_width - xs_right)*STOP_SCALE;                     \
	    ystart = ys_top;                                                \
	    ystop  = (src_height - ys_bot)*STOP_SCALE;                      \
	    xdstart = mlib_ceil(xstart * zoomx + tx - 0.5) + 0.5;           \
	    xdstop = mlib_ceil(xstop * zoomx + tx - 1.5) + 0.5;             \
	    ydstart = mlib_ceil(ystart * zoomy + ty - 0.5) + 0.5;           \
	    ydstop = mlib_ceil(ystop * zoomy + ty - 1.5) + 0.5;             \
	    if (current == nearest && edge == MLIB_EDGE_SRC_EXTEND_INDEF) { \
		xdstart = 0;                                                \
		xdstop = dst_width;                                         \
		ydstart = 0;                                                \
		ydstop = dst_height;                                        \
	    }                                                               \
	    if (xdstart < 0.5)                                              \
		xdstart = 0.5;                                              \
	    if (xdstop > dst_width - 0.5)                                   \
		xdstop = dst_width - 0.5;                                   \
	    if (ydstart < 0.5)                                              \
		ydstart = 0.5;                                              \
	    if (ydstop > dst_height - 0.5)                                  \
		ydstop = dst_height - 0.5;                                  \
	    xstart = (xdstart - tx) / zoomx - dlt;                          \
	    ystart = (ydstart - ty) / zoomy - dlt;                          \
                                                                            \
/*                                                                          \
 * for BILINEAR/BICUBIC and FLOAT type                                      \
 */                                                                         \
	    if (dlt != 0.0 && type == MLIB_FLOAT) {                         \
		xstart = (mlib_f32)xstart;                                  \
		ystart = (mlib_f32)ystart;                                  \
	    }                                                               \
	    current->srcX = (mlib_s32)(xstart * MLIB_PREC);                 \
	    current->srcY = (mlib_s32)(ystart * MLIB_PREC);                 \
	    current->dstX = (mlib_s32)xdstart;                              \
	    current->dstY = (mlib_s32)ydstart;                              \
	    current->width = (mlib_s32)(xdstop - xdstart + 1);              \
	    current->height = (mlib_s32)(ydstop - ydstart + 1);             \
	    current->sp =                                                   \
		sp + (((mlib_s32)xstart * chan_s) << tshift) +              \
		((mlib_s32)ystart * src_stride);                            \
	    current->dp =                                                   \
		dp + (((mlib_s32)xdstart * chan_d) << tshift) +             \
		((mlib_s32)ydstart * dst_stride);                           \
/*                                                                          \
 * check integer overflow                                                   \
 */                                                                         \
	    if (current->srcX + param->DX * (current->width - 1) >=         \
		src_width << MLIB_SHIFT) {                                  \
		param->DX--;                                                \
	    }                                                               \
	    if (current->srcY + param->DY * (current->height - 1) >=        \
		src_height << MLIB_SHIFT) {                                 \
		param->DY--;                                                \
	    }                                                               \
	}

/* *********************************************************** */

#define	STORE_EDGE(N, ptr, ww, hh)                              \
	edges[N].dp = ptr;                                      \
	edges[N].w = ww;                                        \
	edges[N].h = hh;                                        \
	edges[N].dlb = param->dst_stride

/* *********************************************************** */

#define	sizex	current->width
#define	sizey	current->height

/* *********************************************************** */

#define	CHECK_OVERFLOW(xy, FP_TYPE, I_TYPE)                            \
	{                                                              \
	    mlib_s32 size = size##xy - 1, k;                           \
	                                                               \
	    if (size > 0) {                                            \
		FP_TYPE xx, dx = (FP_TYPE) rzoom##xy;                  \
	                                                               \
		for (k = 0; k < 3; k++) {                              \
		    xx = (FP_TYPE) (param->xy##start + (ksize - 1));   \
		    for (i = 0; i < size; i++)                         \
			xx += dx;                                      \
		    if (xx >= xy##_max) {                              \
			(*(I_TYPE *) & dx)--;                          \
			rzoom##xy = dx;                                \
			continue;                                      \
		    }                                                  \
		}                                                      \
	    }                                                          \
	}

/* *********************************************************** */

mlib_status
mlib_ImageZoomClipping(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 zoomx,
    mlib_d64 zoomy,
    mlib_d64 tx,
    mlib_d64 ty,
    mlib_filter filter,
    mlib_edge edge,
    mlib_work_image * param)
{
	mlib_u8 *sp = (void *)mlib_ImageGetData(src);
	mlib_u8 *dp = (void *)mlib_ImageGetData(dst);
	mlib_s32 i, tshift;
	mlib_s32 type = mlib_ImageGetType(src);
	mlib_s32 chan_s = mlib_ImageGetChannels(src);
	mlib_s32 chan_d = mlib_ImageGetChannels(dst);
	mlib_s32 src_height = mlib_ImageGetHeight(src);
	mlib_s32 src_width = mlib_ImageGetWidth(src);
	mlib_s32 src_stride = mlib_ImageGetStride(src);
	mlib_s32 dst_height = mlib_ImageGetHeight(dst);
	mlib_s32 dst_width = mlib_ImageGetWidth(dst);
	mlib_s32 dst_stride = mlib_ImageGetStride(dst);
	mlib_s32 ksize = 1, x0, y0, x_max, y_max, sline_size;
	mlib_d64 rzoomx, rzoomy;
	mlib_d64 xstart, ystart;
	mlib_d64 xs_left = 0, xs_right = 0, ys_top = 0, ys_bot = 0, dlt = 0;
	mlib_d64 xs_left_nn = 0, xs_right_nn = 0, ys_top_nn = 0, ys_bot_nn = 0;
	mlib_clipping *nearest = param->nearest;
	mlib_clipping *current = param->current;
	mlib_f32 f_one = 1;

	(*(mlib_u32*)&f_one) -= 1024;

	rzoomx = 1.0 / zoomx;
	rzoomy = 1.0 / zoomy;

	param->width = src_width;
	param->height = src_height;
	param->DX = (mlib_s32)(MLIB_PREC * rzoomx);
	param->DY = (mlib_s32)(MLIB_PREC * rzoomy);
	param->sp = sp;
	param->dp = dp;
	param->color = 0;
	param->zoomx = zoomx;
	param->zoomy = zoomy;
	param->src_stride = src_stride;
	param->dst_stride = dst_stride;
	param->filter = filter;

	param->channels = chan_s;

	switch (type) {
	case MLIB_DOUBLE:
		tshift = 3;
		break;
	case MLIB_FLOAT:
		tshift = 2;
		break;
	case MLIB_INT:
		tshift = 2;
		break;
	case MLIB_SHORT:
		tshift = 1;
		break;
	case MLIB_USHORT:
		tshift = 1;
		break;
	default:
		tshift = 0;
		break;
	}

	if (filter == MLIB_NEAREST && type != MLIB_BIT && chan_s == chan_d) {
		mlib_s32 align =
		    ((mlib_s32)sp | (mlib_s32)dp | src_stride | dst_stride);
		while ((chan_s & 1) == 0 && tshift < 3) {
			mlib_s32 mask = (2 << tshift) - 1;

/* unaligned */
			if (align & mask) {
				break;
			}

/* two channels as one channel of next type */
			chan_s >>= 1;
			chan_d >>= 1;
			tshift++;
		}
	} else {
		param->src_stride >>= tshift;
		param->dst_stride >>= tshift;
	}

	param->ind_fun_nn = 4 * tshift + (chan_s - 1);

	switch (filter) {
	case MLIB_NEAREST:
		xs_left = xs_right = 0;
		ys_top = ys_bot = 0;
		dlt = 0;
		break;
	case MLIB_BILINEAR:
		xs_left = xs_right = 0.5;
		ys_top = ys_bot = 0.5;
		dlt = 0.5;
		break;
	case MLIB_BICUBIC:
	case MLIB_BICUBIC2:
		xs_left = xs_right = 1.5;
		ys_top = ys_bot = 1.5;
		dlt = 0.5;
		break;
	}

	if (edge == MLIB_EDGE_SRC_PADDED) {
		mlib_u8 *padd = mlib_ImageGetPaddings(src);
		mlib_s32 minX = padd[0];
		mlib_s32 minY = padd[1];
		mlib_s32 maxX = padd[2];
		mlib_s32 maxY = padd[3];

		if (xs_left < minX)
			xs_left = minX;

		if (ys_top < minY)
			ys_top = minY;

		if (maxX > xs_right)
			xs_right = maxX;

		if (maxY > ys_bot)
			ys_bot = maxY;

		xs_left_nn = xs_left;
		xs_right_nn = xs_right;
		ys_top_nn = ys_top;
		ys_bot_nn = ys_bot;
	}

	SET_PARAMETRS(current, xs_left, xs_right, ys_top, ys_bot, dlt, xstart,
	    ystart);


	xstart -= (mlib_d64)((mlib_s32)xstart);
	if (xstart < 0) xstart += 1;
	if ((mlib_f32)xstart == 1) xstart = f_one;

	ystart -= (mlib_d64)((mlib_s32)ystart);
	if (ystart < 0) ystart += 1;
	if ((mlib_f32)ystart == 1) ystart = f_one;

	param->xstart = xstart;
	param->ystart = ystart;

	if (edge != MLIB_EDGE_DST_NO_WRITE && edge != MLIB_EDGE_SRC_PADDED) {
		SET_PARAMETRS(nearest, 0, 0, 0, 0, 0, xstart, ystart);
	} else {
		SET_PARAMETRS(nearest, xs_left_nn, xs_right_nn, ys_top_nn,
		    ys_bot_nn, 0, xstart, ystart);
	}

	if (edge == MLIB_EDGE_SRC_EXTEND ||
	    edge == MLIB_EDGE_SRC_EXTEND_INDEF) {
		SetElemSubStruct(nearest, srcX,
		    (nearest->srcX) - (MLIB_PREC >> 1));
		SetElemSubStruct(nearest, srcY,
		    (nearest->srcY) - (MLIB_PREC >> 1));
	}

	if (current->width < 0) {
		current->width = 0;
		current->dstX = nearest->dstX;
		current->srcX = nearest->srcX;
		current->sp = nearest->sp;
		current->dp = nearest->dp;
	}

	if (current->height < 0) {
		current->height = 0;
		current->dstY = nearest->dstY;
		current->srcY = nearest->srcY;
		current->sp = nearest->sp;
		current->dp = nearest->dp;
	}

/* Calc edge boxes */

	if (edge == MLIB_EDGE_DST_FILL_ZERO || edge == MLIB_EDGE_OP_NEAREST ||
	    edge == MLIB_EDGE_SRC_EXTEND ||
	    edge == MLIB_EDGE_SRC_EXTEND_INDEF) {
		mlib_edge_box *edges = param->edges;
		mlib_s32 w1 = nearest->width;
		mlib_s32 w2 = current->dstX - nearest->dstX;
		mlib_s32 w3 = w1 - w2 - current->width;
		mlib_s32 h1 = current->dstY - nearest->dstY;
		mlib_s32 h2 = current->height;
		mlib_s32 h3 = nearest->height - h1 - h2;
		mlib_u8 *dp0 = nearest->dp;
		mlib_u8 *dp1 = dp0 + h1 * dst_stride;
		mlib_u8 *dp2 =
		    dp1 + ((chan_d * (w2 + current->width)) << tshift);
		mlib_u8 *dp3 = dp1 + current->height * dst_stride;

		if (w2 < 0)
			w2 = 0;
		if (w3 < 0)
			w3 = 0;
		if (h1 < 0)
			h1 = 0;
		if (h3 < 0)
			h3 = 0;

		STORE_EDGE(0, dp0, w1, h1);
		STORE_EDGE(1, dp1, w2, h2);
		STORE_EDGE(2, dp2, w3, h2);
		STORE_EDGE(3, dp3, w1, h3);
	}

	param->chan_d = chan_d;
	param->alp_ind = 0;

/* sline_size */
	x0 = current->srcX;
	y0 = current->srcY;
	x_max = src_width - (x0 >> MLIB_SHIFT);
	y_max = src_height - (y0 >> MLIB_SHIFT);
	sline_size =
	    ((x0 & MLIB_MASK) + (current->width - 1) * param->DX) >> MLIB_SHIFT;
	switch (filter) {
	case MLIB_NEAREST:
		ksize = 1;
		break;
	case MLIB_BILINEAR:
		ksize = 2;
		break;
	case MLIB_BICUBIC:
	case MLIB_BICUBIC2:
		ksize = 4;
		break;
	}

	sline_size += ksize;

	if (ksize == 4) {
		x_max++;
		y_max++;
	}

	if (sline_size > x_max)
		sline_size = x_max;
	param->sline_size = sline_size;
	param->y_max = y_max;

	if (type == MLIB_FLOAT) {
		CHECK_OVERFLOW(x, mlib_f32,
		    mlib_u32);
		CHECK_OVERFLOW(y, mlib_f32,
		    mlib_u32);
	}

	param->rzoomx = rzoomx;
	param->rzoomy = rzoomy;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
