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

#pragma ident	"@(#)mlib_v_ImageZoomIndex_BC.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomIndex - indexed color image scaling
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoomIndex(mlib_image       *dst,
 *                                      const mlib_image *src,
 *                                      mlib_f32         zoomx,
 *                                      mlib_f32         zoomy,
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
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageColormap.h>
#include <mlib_v_ImageFilters.h>

/* *********************************************************** */

#define	_MLIB_VIS_VER_

#include <mlib_ImageZoom.h>

/* *********************************************************** */

/*			Bicubic					*/

/* *********************************************************** */

#define	FUNC_NAME(format, ver)                                  \
	mlib_v_ImageZoomIndex_##format##_Bicubic_##ver##_Line

/* *********************************************************** */

#define	MLIB_XY_SHIFT	3
#define	MLIB_XY_MASK	(((1 << 8) - 1) << 5)

/* *********************************************************** */

#define	FPMUL16(src1, src2)                                     \
	vis_fpadd16(vis_fmul8sux16((src1), (src2)),             \
	    vis_fmul8ulx16((src1), (src2)))

/* *********************************************************** */

#define	BICUBIC_U8_ONE_LINE                                     \
	a0 = colortable[tsp[cx]];                               \
	a1 = colortable[tsp[cx + 1]];                           \
	a2 = colortable[tsp[cx + 2]];                           \
	a3 = colortable[tsp[cx + 3]];                           \
	r0 = vis_fmul8x16(a0, dxf0);                            \
	r1 = vis_fmul8x16(a1, dxf1);                            \
	r2 = vis_fmul8x16(a2, dxf2);                            \
	r3 = vis_fmul8x16(a3, dxf3);                            \
	r4 = vis_fpadd16(r0, r1);                               \
	r5 = vis_fpadd16(r2, r3);                               \
	r6 = vis_fpadd16(r4, r5)

/* *********************************************************** */

#define	BICUBIC_S16_ONE_LINE                                    \
	a0 = colortable[tsp[cx]];                               \
	a1 = colortable[tsp[cx + 1]];                           \
	a2 = colortable[tsp[cx + 2]];                           \
	a3 = colortable[tsp[cx + 3]];                           \
	r0 = FPMUL16(a0, dxf0);                                 \
	r1 = FPMUL16(a1, dxf1);                                 \
	r2 = FPMUL16(a2, dxf2);                                 \
	r3 = FPMUL16(a3, dxf3);                                 \
	r4 = vis_fpadd16(r0, r1);                               \
	r5 = vis_fpadd16(r2, r3);                               \
	r6 = vis_fpadd16(r4, r5)

/* *********************************************************** */

#define	IDX_U8_U8

#include <mlib_v_ImageZoomIndex_BC.h>

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIndex_U8_U8_Bicubic(
    mlib_work_image * param)
{
	mlib_s32 filter = GetElemStruct(filter);
	mlib_u8 *filter_tbl =
	    (void *)((filter ==
	    MLIB_BICUBIC) ? mlib_filters_u8_bc_4 : mlib_filters_u8_bc2_4);
	void *colormap = GetElemStruct(colormap);
	mlib_f32 *colortable =
	    (mlib_f32 *)mlib_ImageGetLutNormalTable(colormap) -
	    mlib_ImageGetLutOffset(colormap);
	mlib_u8 *tsp,
		*dpbuf = GetElemStruct(buffer_dp),
		*sp = GetElemSubStruct(current, sp),
		*dp = GetElemSubStruct(current, dp);
	mlib_s32 j, cx, next_call,
		dx = GetElemStruct(DX),
		dy = GetElemStruct(DY),
		x = GetElemSubStruct(current, srcX) & MLIB_MASK,
		y = GetElemSubStruct(current, srcY) & MLIB_MASK,
		src_stride = GetElemStruct(src_stride),
		channels = mlib_ImageGetLutChannels(colormap),
		dst_stride = GetElemStruct(dst_stride),
		width  = GetElemSubStruct(current, width),
		height = GetElemSubStruct(current, height);
	mlib_d64 *buff, *buf0, *buf1, *buf2, *buf3, *bufT, buffer[4096];
	mlib_pack_func func_pack;

	sp -= src_stride + 1;
	tsp = sp;
	vis_write_gsr(3 << 3);

	func_pack =
	    (channels ==
	    3) ? (mlib_pack_func) mlib_ImageColorTrue2IndexLine_U8_U8_3_in_4
	    : (mlib_pack_func) mlib_ImageColorTrue2IndexLine_U8_U8_4;

	if (width * 4 > 4096) {
		buff = __mlib_malloc(sizeof (mlib_d64) * width * 4);

		if (buff == NULL)
			return (MLIB_FAILURE);
	} else
		buff = buffer;
	buf0 = buff;
	buf1 = buf0 + width;
	buf2 = buf1 + width;
	buf3 = buf2 + width;

	next_call = 4;

	for (j = 0; j < height; j++) {

		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		cx = next_call > 4 ? 4 : next_call;
		next_call = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);

		if (cx == 0) {
			FUNC_NAME(U8_U8, 0) (width, y,
			    (void *)dpbuf, buf0, buf1, buf2, buf3, filter_tbl);
		} else {
			if (cx == 1) {
				bufT = buf0;
				buf0 = buf1;
				buf1 = buf2;
				buf2 = buf3;
				buf3 = bufT;
				mlib_v_ImageZoomIndex_U8_U8_Bicubic_1_Line
				    (width, x, dx, y, src_stride, tsp,
				    (void *)dpbuf, colortable, buf0, buf1, buf2,
				    buf3, filter_tbl);
			} else {
				if (cx == 2) {
					bufT = buf0;
					buf0 = buf2, buf2 = bufT;
					bufT = buf1;
					buf1 = buf3, buf3 = bufT;
					FUNC_NAME(U8_U8, 2) (width, x, dx, y,
					    src_stride, tsp, (void *)dpbuf,
					    colortable, buf0, buf1, buf2, buf3,
					    filter_tbl);
				} else {
					if (cx == 3) {
						bufT = buf0;
						buf0 = buf3, buf3 = bufT;
						FUNC_NAME(U8_U8, 3) (width, x,
						    dx, y, src_stride, tsp,
						    (void *)dpbuf, colortable,
						    buf0, buf1, buf2, buf3,
						    filter_tbl);
					} else {
						FUNC_NAME(U8_U8, 4) (width, x,
						    dx, y, src_stride, tsp,
						    (void *)dpbuf, colortable,
						    buf0, buf1, buf2, buf3,
						    filter_tbl);
					}
				}
			}
		}

		func_pack(dpbuf, dp, width, colormap);

		y += dy;
		dp += dst_stride;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	if (buff != buffer)
		__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  IDX_U8_U8
#define	IDX_S16_U8

#include <mlib_v_ImageZoomIndex_BC.h>

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIndex_S16_U8_Bicubic(
    mlib_work_image * param)
{
	mlib_s32 filter = GetElemStruct(filter);
	mlib_u8 *filter_tbl =
	    (void *)((filter ==
	    MLIB_BICUBIC) ? mlib_filters_u8_bc_4 : mlib_filters_u8_bc2_4);
	void *colormap = GetElemStruct(colormap);
	mlib_f32 *colortable =
	    (mlib_f32 *)mlib_ImageGetLutNormalTable(colormap) -
	    mlib_ImageGetLutOffset(colormap);
	mlib_u8 *dpbuf = GetElemStruct(buffer_dp);
	mlib_s16 *tsp,
	    *sp = GetElemSubStruct(current, sp),
	    *dp = GetElemSubStruct(current, dp);
	mlib_s32 j, cx, next_call,
	    dx = GetElemStruct(DX),
	    dy = GetElemStruct(DY),
	    x = GetElemSubStruct(current, srcX) & MLIB_MASK,
	    y = GetElemSubStruct(current, srcY) & MLIB_MASK,
	    src_stride = GetElemStruct(src_stride),
	    channels = mlib_ImageGetLutChannels(colormap),
	    dst_stride = GetElemStruct(dst_stride),
	    width = GetElemSubStruct(current, width),
	    height = GetElemSubStruct(current, height);
	mlib_d64 *buff, *buf0, *buf1, *buf2, *buf3, *bufT, buffer[4096];
	mlib_pack_func func_pack;

	sp -= src_stride + 1;
	tsp = sp;
	vis_write_gsr(3 << 3);

	func_pack =
	    (channels ==
	    3) ? (mlib_pack_func) mlib_ImageColorTrue2IndexLine_U8_S16_3_in_4
	    : (mlib_pack_func) mlib_ImageColorTrue2IndexLine_U8_S16_4;

	if (width * 4 > 4096) {
		buff = __mlib_malloc(sizeof (mlib_d64) * width * 4);

		if (buff == NULL)
			return (MLIB_FAILURE);
	} else
		buff = buffer;
	buf0 = buff;
	buf1 = buf0 + width;
	buf2 = buf1 + width;
	buf3 = buf2 + width;

	next_call = 4;

	for (j = 0; j < height; j++) {

		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		cx = next_call > 4 ? 4 : next_call;
		next_call = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);

		if (cx == 0) {
			FUNC_NAME(S16_U8, 0) (width, y,
			    (void *)dpbuf, buf0, buf1, buf2, buf3, filter_tbl);
		} else {
			if (cx == 1) {
				bufT = buf0;
				buf0 = buf1;
				buf1 = buf2;
				buf2 = buf3;
				buf3 = bufT;
				FUNC_NAME(S16_U8, 1)
				    (width, x, dx, y, src_stride, tsp,
				    (void *)dpbuf, colortable, buf0, buf1, buf2,
				    buf3, filter_tbl);
			} else {
				if (cx == 2) {
					bufT = buf0;
					buf0 = buf2, buf2 = bufT;
					bufT = buf1;
					buf1 = buf3, buf3 = bufT;
					FUNC_NAME(S16_U8, 2)
					    (width, x, dx, y, src_stride, tsp,
					    (void *)dpbuf, colortable, buf0,
					    buf1, buf2, buf3, filter_tbl);
				} else {
					if (cx == 3) {
						bufT = buf0;
						buf0 = buf3, buf3 = bufT;
						FUNC_NAME(S16_U8, 3)
						    (width, x, dx, y,
						    src_stride, tsp,
						    (void *)dpbuf, colortable,
						    buf0, buf1, buf2, buf3,
						    filter_tbl);
					} else {
						FUNC_NAME(S16_U8, 4)
						    (width, x, dx, y,
						    src_stride, tsp,
						    (void *)dpbuf, colortable,
						    buf0, buf1, buf2, buf3,
						    filter_tbl);
					}
				}
			}
		}

		func_pack(dpbuf, dp, width, colormap);

		y += dy;
		dp += dst_stride;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	if (buff != buffer)
		__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef MLIB_XY_SHIFT
#undef MLIB_XY_MASK

/* *********************************************************** */

#define	MLIB_XY_SHIFT	2
#define	MLIB_XY_MASK	(((1 << 9) - 1) << 5)

/* *********************************************************** */

#undef  IDX_S16_U8
#define	IDX_S16_S16

#include <mlib_v_ImageZoomIndex_BC.h>

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIndex_S16_S16_Bicubic(
    mlib_work_image * param)
{
	mlib_s32 filter = GetElemStruct(filter);
	mlib_u8 *filter_tbl =
	    (void *)((filter ==
	    MLIB_BICUBIC) ? mlib_filters_s16_bc_4 : mlib_filters_s16_bc2_4);
	void *colormap = GetElemStruct(colormap);
	mlib_d64 *colortable =
	    (mlib_d64 *)mlib_ImageGetLutNormalTable(colormap) -
	    mlib_ImageGetLutOffset(colormap);
	mlib_s16 *dpbuf = GetElemStruct(buffer_dp);
	mlib_s16 *tsp,
	    *sp = GetElemSubStruct(current, sp),
	    *dp = GetElemSubStruct(current, dp);
	mlib_s32 j, cx, next_call,
	    dx = GetElemStruct(DX),
	    dy = GetElemStruct(DY),
	    x = GetElemSubStruct(current, srcX) & MLIB_MASK,
	    y = GetElemSubStruct(current, srcY) & MLIB_MASK,
	    src_stride = GetElemStruct(src_stride),
	    dst_stride = GetElemStruct(dst_stride),
	    channels = mlib_ImageGetLutChannels(colormap),
	    width = GetElemSubStruct(current, width),
	    height = GetElemSubStruct(current, height);
	mlib_d64 *buff, *buf0, *buf1, *buf2, *buf3, *bufT, buffer[4096];
	mlib_pack_func func_pack;

	sp -= (src_stride + 1);
	tsp = sp;
	vis_write_gsr(10 << 3);

	func_pack =
	    (channels ==
	    3) ? (mlib_pack_func) mlib_ImageColorTrue2IndexLine_S16_S16_3_in_4
	    : (mlib_pack_func) mlib_ImageColorTrue2IndexLine_S16_S16_4;

	if (width * 4 > 4096) {
		buff = __mlib_malloc(sizeof (mlib_d64) * width * 4);

		if (buff == NULL)
			return (MLIB_FAILURE);
	} else
		buff = buffer;
	buf0 = buff;
	buf1 = buf0 + width;
	buf2 = buf1 + width;
	buf3 = buf2 + width;

	next_call = 4;

	for (j = 0; j < height; j++) {

		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		cx = next_call > 4 ? 4 : next_call;
		next_call = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);

		if (cx == 0) {
			FUNC_NAME(S16_S16, 0) (width, y,
			    (void *)dpbuf, buf0, buf1, buf2, buf3, filter_tbl);
		} else {
			if (cx == 1) {
				bufT = buf0;
				buf0 = buf1;
				buf1 = buf2;
				buf2 = buf3;
				buf3 = bufT;
				FUNC_NAME(S16_S16, 1)
				    (width, x, dx, y, src_stride, tsp,
				    (void *)dpbuf, colortable, buf0, buf1, buf2,
				    buf3, filter_tbl);
			} else {
				if (cx == 2) {
					bufT = buf0;
					buf0 = buf2, buf2 = bufT;
					bufT = buf1;
					buf1 = buf3, buf3 = bufT;
					FUNC_NAME(S16_S16, 2)
					    (width, x, dx, y, src_stride, tsp,
					    (void *)dpbuf, colortable, buf0,
					    buf1, buf2, buf3, filter_tbl);
				} else {
					if (cx == 3) {
						bufT = buf0;
						buf0 = buf3, buf3 = bufT;
						FUNC_NAME(S16_S16, 3)
						    (width, x, dx, y,
						    src_stride, tsp,
						    (void *)dpbuf, colortable,
						    buf0, buf1, buf2, buf3,
						    filter_tbl);
					} else {
						FUNC_NAME(S16_S16, 4)
						    (width, x, dx, y,
						    src_stride, tsp,
						    (void *)dpbuf, colortable,
						    buf0, buf1, buf2, buf3,
						    filter_tbl);
					}
				}
			}
		}

		func_pack(dpbuf, dp, width, colormap);

		y += dy;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		dp += dst_stride;
	}

	if (buff != buffer)
		__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  IDX_S16_S16
#define	IDX_U8_S16

#include <mlib_v_ImageZoomIndex_BC.h>

/* *********************************************************** */

mlib_status
mlib_v_ImageZoomIndex_U8_S16_Bicubic(
    mlib_work_image * param)
{
	mlib_s32 filter = GetElemStruct(filter);
	mlib_u8 *filter_tbl =
	    (void *)((filter ==
	    MLIB_BICUBIC) ? mlib_filters_s16_bc_4 : mlib_filters_s16_bc2_4);
	void *colormap = GetElemStruct(colormap);
	mlib_d64 *colortable =
	    (mlib_d64 *)mlib_ImageGetLutNormalTable(colormap) -
	    mlib_ImageGetLutOffset(colormap);
	mlib_s16 *dpbuf = GetElemStruct(buffer_dp);
	mlib_u8 *tsp,
	    *sp = GetElemSubStruct(current, sp),
	    *dp = GetElemSubStruct(current, dp);
	mlib_s32 j, cx, next_call,
	    dx = GetElemStruct(DX),
	    dy = GetElemStruct(DY),
	    x = GetElemSubStruct(current, srcX) & MLIB_MASK,
	    y = GetElemSubStruct(current, srcY) & MLIB_MASK,
	    channels = mlib_ImageGetLutChannels(colormap),
	    src_stride = GetElemStruct(src_stride),
	    dst_stride = GetElemStruct(dst_stride),
	    width = GetElemSubStruct(current, width),
	    height = GetElemSubStruct(current, height);
	mlib_d64 *buff, *buf0, *buf1, *buf2, *buf3, *bufT, buffer[4096];
	mlib_pack_func func_pack;

	sp -= (src_stride + 1);
	tsp = sp;
	vis_write_gsr(10 << 3);

	func_pack =
	    (channels ==
	    3) ? (mlib_pack_func) mlib_ImageColorTrue2IndexLine_S16_U8_3_in_4
	    : (mlib_pack_func) mlib_ImageColorTrue2IndexLine_S16_U8_4;

	if (width * 4 > 4096) {
		buff = __mlib_malloc(sizeof (mlib_d64) * width * 4);

		if (buff == NULL)
			return (MLIB_FAILURE);
	} else
		buff = buffer;
	buf0 = buff;
	buf1 = buf0 + width;
	buf2 = buf1 + width;
	buf3 = buf2 + width;

	next_call = 4;

	for (j = 0; j < height; j++) {

		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		cx = next_call > 4 ? 4 : next_call;
		next_call = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);

		if (cx == 0) {
			FUNC_NAME(U8_S16, 0) (width, y,
			    (void *)dpbuf, buf0, buf1, buf2, buf3, filter_tbl);
		} else {
			if (cx == 1) {
				bufT = buf0;
				buf0 = buf1;
				buf1 = buf2;
				buf2 = buf3;
				buf3 = bufT;
				FUNC_NAME(U8_S16, 1)
				    (width, x, dx, y, src_stride, tsp,
				    (void *)dpbuf, colortable, buf0, buf1, buf2,
				    buf3, filter_tbl);
			} else {
				if (cx == 2) {
					bufT = buf0;
					buf0 = buf2, buf2 = bufT;
					bufT = buf1;
					buf1 = buf3, buf3 = bufT;
					FUNC_NAME(U8_S16, 2)
					    (width, x, dx, y, src_stride, tsp,
					    (void *)dpbuf, colortable, buf0,
					    buf1, buf2, buf3, filter_tbl);
				} else {
					if (cx == 3) {
						bufT = buf0;
						buf0 = buf3, buf3 = bufT;
						FUNC_NAME(U8_S16, 3)
						    (width, x, dx, y,
						    src_stride, tsp,
						    (void *)dpbuf, colortable,
						    buf0, buf1, buf2, buf3,
						    filter_tbl);
					} else {
						FUNC_NAME(U8_S16, 4)
						    (width, x, dx, y,
						    src_stride, tsp,
						    (void *)dpbuf, colortable,
						    buf0, buf1, buf2, buf3,
						    filter_tbl);
					}
				}
			}
		}

		func_pack(dpbuf, dp, width, colormap);

		y += dy;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		dp += dst_stride;
	}

	if (buff != buffer)
		__mlib_free(buff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
