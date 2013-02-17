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

#pragma ident	"@(#)mlib_v_ImageAffineTable_8nw_2ch.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      Internal functions for mlib_ImageAffineTable.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageInterpTable.h>
#include <mlib_ImageAffineTable.h>
#include <mlib_v_ImageAffineTable_8nw.h>

/* *********************************************************** */

#define	DECLAREVAR                                              \
	mlib_u8 *sPtr;                                          \
	mlib_f32 *dstPixelPtr;                                  \
	mlib_d64 *dpSrc;                                        \
	mlib_s32 srcStride = ws->srcStride;                     \
	mlib_s32 i, filterposx, filterposy, xSrc, ySrc;         \
	mlib_s32 dx = ws->dx;                                   \
	mlib_s32 dy = ws->dy;                                   \
	mlib_s32 x_shift = ws->x_shift;                         \
	mlib_s32 y_shift = ws->y_shift;                         \
	mlib_s32 xf_shift = ws->xf_shift;                       \
	mlib_s32 xf_mask = ws->xf_mask;                         \
	mlib_s32 yf_shift = ws->yf_shift;                       \
	mlib_s32 yf_mask = ws->yf_mask;                         \
	mlib_s32 size = ws->size;                               \
	mlib_s32 x = ws->x0;                                    \
	mlib_s32 y = ws->y0;                                    \
	mlib_d64 sum0;                                          \
	mlib_d64 row0, row1;                                    \
	mlib_d64 dr, dr1;                                       \
	mlib_f32 p0, p1, p2, p3;                                \
	mlib_d64 e0, e1;                                        \
	mlib_d64 data0, data1;                                  \
	mlib_d64 xFilter0;                                      \
	mlib_d64 d0, d00, d01, d02, d03;                        \
	mlib_d64 v00, v10;                                      \
	mlib_f32 res

/* *********************************************************** */

#define	CALC_SRC_PTR                                            \
	xSrc = (x >> x_shift) & ~1;                             \
	ySrc = (y >> y_shift);                                  \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + (xSrc)

/* *********************************************************** */

#define	LOAD_1PIXEL_4x4                                               \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row0 = vis_faligndata(data0, data1);                          \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row1 = vis_faligndata(data0, data1);                          \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row2 = vis_faligndata(data0, data1);                          \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row3 = vis_faligndata(data0, data1);                          \
	filterposy = (y >> yf_shift) & yf_mask;                       \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));   \
	filterposx = (x >> xf_shift) & xf_mask;                       \
	xFilter = *((mlib_d64 *)((mlib_u8 *)filterX + filterposx));   \
	x += dx;                                                      \
	y += dy

/* *********************************************************** */

#define	RESULT_1PIXEL_4x4                                                \
	v00 = vis_fmul8x16au(vis_read_hi(row0), vis_read_hi(yFilter));   \
	dr = vis_fpmerge(vis_read_hi(xFilter), vis_read_lo(xFilter));    \
	v01 = vis_fmul8x16au(vis_read_lo(row0), vis_read_hi(yFilter));   \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));              \
	v10 = vis_fmul8x16al(vis_read_hi(row1), vis_read_hi(yFilter));   \
	dr1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr));             \
	v11 = vis_fmul8x16al(vis_read_lo(row1), vis_read_hi(yFilter));   \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr));              \
	v20 = vis_fmul8x16au(vis_read_hi(row2), vis_read_lo(yFilter));   \
	xFilter0 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));       \
	v21 = vis_fmul8x16au(vis_read_lo(row2), vis_read_lo(yFilter));   \
	xFilter1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr1));       \
	v30 = vis_fmul8x16al(vis_read_hi(row3), vis_read_lo(yFilter));   \
	sum0 = vis_fpadd16(v00, v10);                                    \
	v31 = vis_fmul8x16al(vis_read_lo(row3), vis_read_lo(yFilter));   \
	sum1 = vis_fpadd16(v01, v11);                                    \
	sum0 = vis_fpadd16(sum0, v20);                                   \
	sum1 = vis_fpadd16(sum1, v21);                                   \
	sum0 = vis_fpadd16(sum0, v30);                                   \
	sum1 = vis_fpadd16(sum1, v31);                                   \
	v00 = vis_fmul8sux16(sum0, xFilter0);                            \
	v01 = vis_fmul8sux16(sum1, xFilter1);                            \
	v10 = vis_fmul8ulx16(sum0, xFilter0);                            \
	sum0 = vis_fpadd16(v00, v10);                                    \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                            \
	sum1 = vis_fpadd16(v01, v11);                                    \
	d0 = vis_fpadd16(sum0, sum1);                                    \
	res = vis_fpadd16s(vis_read_hi(d0), vis_read_lo(d0))

/* *********************************************************** */

#define	MAKE_4x4(index)                                                  \
	v00 = vis_fmul8x16au(vis_read_hi(row0), vis_read_hi(yFilter));   \
	dr = vis_fpmerge(vis_read_hi(xFilter), vis_read_lo(xFilter));    \
	v01 = vis_fmul8x16au(vis_read_lo(row0), vis_read_hi(yFilter));   \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));              \
	v10 = vis_fmul8x16al(vis_read_hi(row1), vis_read_hi(yFilter));   \
	dr1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr));             \
	v11 = vis_fmul8x16al(vis_read_lo(row1), vis_read_hi(yFilter));   \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr));              \
	v20 = vis_fmul8x16au(vis_read_hi(row2), vis_read_lo(yFilter));   \
	xFilter0 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));       \
	v21 = vis_fmul8x16au(vis_read_lo(row2), vis_read_lo(yFilter));   \
	xFilter1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr1));       \
	v30 = vis_fmul8x16al(vis_read_hi(row3), vis_read_lo(yFilter));   \
	v31 = vis_fmul8x16al(vis_read_lo(row3), vis_read_lo(yFilter));   \
	vis_alignaddr(sPtr, 0);                                          \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                  \
	data0 = dpSrc[0];                                                \
	sum0 = vis_fpadd16(v00, v10);                                    \
	filterposy = (y >> yf_shift);                                    \
	data1 = dpSrc[1];                                                \
	row0 = vis_faligndata(data0, data1);                             \
	filterposx = (x >> xf_shift);                                    \
	sPtr += srcStride;                                               \
	vis_alignaddr(sPtr, 0);                                          \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                  \
	data0 = dpSrc[0];                                                \
	sum1 = vis_fpadd16(v01, v11);                                    \
	x += dx;                                                         \
	data1 = dpSrc[1];                                                \
	sum0 = vis_fpadd16(sum0, v20);                                   \
	row1 = vis_faligndata(data0, data1);                             \
	sPtr += srcStride;                                               \
	vis_alignaddr(sPtr, 0);                                          \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                  \
	y += dy;                                                         \
	sum1 = vis_fpadd16(sum1, v21);                                   \
	xSrc = (x >> x_shift) & ~1;                                      \
	data0 = dpSrc[0];                                                \
	ySrc = (y >> y_shift);                                           \
	sum0 = vis_fpadd16(sum0, v30);                                   \
	data1 = dpSrc[1];                                                \
	filterposy &= yf_mask;                                           \
	sum1 = vis_fpadd16(sum1, v31);                                   \
	v00 = vis_fmul8sux16(sum0, xFilter0);                            \
	row2 = vis_faligndata(data0, data1);                             \
	v01 = vis_fmul8sux16(sum1, xFilter1);                            \
	sPtr += srcStride;                                               \
	v10 = vis_fmul8ulx16(sum0, xFilter0);                            \
	filterposx &= xf_mask;                                           \
	vis_alignaddr(sPtr, 0);                                          \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                  \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                            \
	data0 = dpSrc[0];                                                \
	d0##index = vis_fpadd16(v00, v10);                               \
	data1 = dpSrc[1];                                                \
	row3 = vis_faligndata(data0, data1);                             \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));      \
	d1##index = vis_fpadd16(v01, v11);                               \
	xFilter = *((mlib_d64 *)((mlib_u8 *)filterX + filterposx));      \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + (xSrc)

/* *********************************************************** */

#define	FADD_4x4                                                \
	d0 = vis_fpadd16(d00, d10);                             \
	d1 = vis_fpadd16(d01, d11);                             \
	d2 = vis_fpadd16(d02, d12);                             \
	d3 = vis_fpadd16(d03, d13);                             \
	p0 = vis_fpadd16s(vis_read_hi(d0), vis_read_lo(d0));    \
	p1 = vis_fpadd16s(vis_read_hi(d1), vis_read_lo(d1));    \
	p2 = vis_fpadd16s(vis_read_hi(d2), vis_read_lo(d2));    \
	p3 = vis_fpadd16s(vis_read_hi(d3), vis_read_lo(d3));    \
	e0 = vis_freg_pair(p0, p1);                             \
	e1 = vis_freg_pair(p2, p3)

/* *********************************************************** */

void
mlib_v_ImageAffineTableLine_8nw_4_4_2(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws)
{
	DECLAREVAR;
	mlib_d64 d1, d2, d3;
	mlib_d64 d10, d11, d12, d13;
	mlib_d64 v20, v30;
	mlib_d64 v01, v11, v21, v31;
	mlib_d64 row2, row3, sum1;
	mlib_d64 xFilter, xFilter1;
	mlib_d64 yFilter;

	i = 0;

	dstPixelPtr = (mlib_f32 *)buff;

	if (i <= size - 10) {

		CALC_SRC_PTR;
		LOAD_1PIXEL_4x4;

		CALC_SRC_PTR;

		MAKE_4x4(0);
		MAKE_4x4(1);
		MAKE_4x4(2);
		MAKE_4x4(3);

		FADD_4x4;

		MAKE_4x4(0);
		MAKE_4x4(1);
		MAKE_4x4(2);
		MAKE_4x4(3);

#pragma pipeloop(0)
		for (; i <= size - 14; i += 4) {
			*buff = e0;
			buff++;
			*buff = e1;
			FADD_4x4;
			MAKE_4x4(0);
			MAKE_4x4(1);
			MAKE_4x4(2);
			MAKE_4x4(3);
			buff++;
		}

		*buff = e0;
		buff++;
		*buff = e1;
		buff++;
		FADD_4x4;
		*buff = e0;
		buff++;
		*buff = e1;
		buff++;

		dstPixelPtr = (mlib_f32 *)buff;

		RESULT_1PIXEL_4x4;
		*dstPixelPtr = res;
		dstPixelPtr++;

		LOAD_1PIXEL_4x4;
		RESULT_1PIXEL_4x4;
		*dstPixelPtr = res;
		dstPixelPtr++;
		i += 10;
	}
#pragma pipeloop(0)
	for (; i < size; i++) {
		CALC_SRC_PTR;
		LOAD_1PIXEL_4x4;
		RESULT_1PIXEL_4x4;
		*dstPixelPtr = res;
		dstPixelPtr++;
	}
}

/* *********************************************************** */

#define	LOAD_1PIXEL_3x4                                               \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row0 = vis_faligndata(data0, data1);                          \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row1 = vis_faligndata(data0, data1);                          \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row2 = vis_faligndata(data0, data1);                          \
	filterposy = (y >> yf_shift) & yf_mask;                       \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));   \
	filterposx = (x >> xf_shift) & xf_mask;                       \
	xFilter = *((mlib_d64 *)((mlib_u8 *)filterX + filterposx));   \
	x += dx;                                                      \
	y += dy

/* *********************************************************** */

#define	RESULT_1PIXEL_3x4                                                \
	v00 = vis_fmul8x16au(vis_read_hi(row0), vis_read_hi(yFilter));   \
	dr = vis_fpmerge(vis_read_hi(xFilter), vis_read_lo(xFilter));    \
	v01 = vis_fmul8x16au(vis_read_lo(row0), vis_read_hi(yFilter));   \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));              \
	v10 = vis_fmul8x16al(vis_read_hi(row1), vis_read_hi(yFilter));   \
	dr1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr));             \
	v11 = vis_fmul8x16al(vis_read_lo(row1), vis_read_hi(yFilter));   \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr));              \
	v20 = vis_fmul8x16au(vis_read_hi(row2), vis_read_lo(yFilter));   \
	xFilter0 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));       \
	v21 = vis_fmul8x16au(vis_read_lo(row2), vis_read_lo(yFilter));   \
	xFilter1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr1));       \
	sum0 = vis_fpadd16(v00, v10);                                    \
	sum1 = vis_fpadd16(v01, v11);                                    \
	sum0 = vis_fpadd16(sum0, v20);                                   \
	sum1 = vis_fpadd16(sum1, v21);                                   \
	v00 = vis_fmul8sux16(sum0, xFilter0);                            \
	v01 = vis_fmul8sux16(sum1, xFilter1);                            \
	v10 = vis_fmul8ulx16(sum0, xFilter0);                            \
	sum0 = vis_fpadd16(v00, v10);                                    \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                            \
	sum1 = vis_fpadd16(v01, v11);                                    \
	d0 = vis_fpadd16(sum0, sum1);                                    \
	res = vis_fpadd16s(vis_read_hi(d0), vis_read_lo(d0))

/* *********************************************************** */

#define	MAKE_3x4(index)                                                  \
	v00 = vis_fmul8x16au(vis_read_hi(row0), vis_read_hi(yFilter));   \
	dr = vis_fpmerge(vis_read_hi(xFilter), vis_read_lo(xFilter));    \
	v01 = vis_fmul8x16au(vis_read_lo(row0), vis_read_hi(yFilter));   \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));              \
	v10 = vis_fmul8x16al(vis_read_hi(row1), vis_read_hi(yFilter));   \
	dr1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr));             \
	v11 = vis_fmul8x16al(vis_read_lo(row1), vis_read_hi(yFilter));   \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr));              \
	v20 = vis_fmul8x16au(vis_read_hi(row2), vis_read_lo(yFilter));   \
	xFilter0 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));       \
	v21 = vis_fmul8x16au(vis_read_lo(row2), vis_read_lo(yFilter));   \
	xFilter1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr1));       \
	vis_alignaddr(sPtr, 0);                                          \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                  \
	data0 = dpSrc[0];                                                \
	sum0 = vis_fpadd16(v00, v10);                                    \
	filterposy = (y >> yf_shift);                                    \
	data1 = dpSrc[1];                                                \
	row0 = vis_faligndata(data0, data1);                             \
	filterposx = (x >> xf_shift);                                    \
	sPtr += srcStride;                                               \
	vis_alignaddr(sPtr, 0);                                          \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                  \
	data0 = dpSrc[0];                                                \
	sum1 = vis_fpadd16(v01, v11);                                    \
	x += dx;                                                         \
	data1 = dpSrc[1];                                                \
	sum0 = vis_fpadd16(sum0, v20);                                   \
	row1 = vis_faligndata(data0, data1);                             \
	sPtr += srcStride;                                               \
	vis_alignaddr(sPtr, 0);                                          \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                  \
	y += dy;                                                         \
	sum1 = vis_fpadd16(sum1, v21);                                   \
	xSrc = (x >> x_shift) & ~1;                                      \
	data0 = dpSrc[0];                                                \
	ySrc = (y >> y_shift);                                           \
	data1 = dpSrc[1];                                                \
	filterposy &= yf_mask;                                           \
	v00 = vis_fmul8sux16(sum0, xFilter0);                            \
	row2 = vis_faligndata(data0, data1);                             \
	v01 = vis_fmul8sux16(sum1, xFilter1);                            \
	v10 = vis_fmul8ulx16(sum0, xFilter0);                            \
	filterposx &= xf_mask;                                           \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                            \
	d0##index = vis_fpadd16(v00, v10);                               \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));      \
	d1##index = vis_fpadd16(v01, v11);                               \
	xFilter = *((mlib_d64 *)((mlib_u8 *)filterX + filterposx));      \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + (xSrc)

/* *********************************************************** */

void
mlib_v_ImageAffineTableLine_8nw_3_4_2(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws)
{
	DECLAREVAR;
	mlib_d64 d1, d2, d3;
	mlib_d64 d10, d11, d12, d13;
	mlib_d64 v20;
	mlib_d64 v01, v11, v21;
	mlib_d64 row2, sum1;
	mlib_d64 xFilter, xFilter1;
	mlib_d64 yFilter;

	i = 0;

	dstPixelPtr = (mlib_f32 *)buff;

	if (i <= size - 10) {

		CALC_SRC_PTR;
		LOAD_1PIXEL_3x4;

		CALC_SRC_PTR;

		MAKE_3x4(0);
		MAKE_3x4(1);
		MAKE_3x4(2);
		MAKE_3x4(3);

		FADD_4x4;

		MAKE_3x4(0);
		MAKE_3x4(1);
		MAKE_3x4(2);
		MAKE_3x4(3);

#pragma pipeloop(0)
		for (; i <= size - 14; i += 4) {
			*buff = e0;
			buff++;
			*buff = e1;
			FADD_4x4;
			MAKE_3x4(0);
			MAKE_3x4(1);
			MAKE_3x4(2);
			MAKE_3x4(3);
			buff++;
		}

		*buff = e0;
		buff++;
		*buff = e1;
		buff++;
		FADD_4x4;
		*buff = e0;
		buff++;
		*buff = e1;
		buff++;

		dstPixelPtr = (mlib_f32 *)buff;

		RESULT_1PIXEL_3x4;
		*dstPixelPtr = res;
		dstPixelPtr++;

		LOAD_1PIXEL_3x4;
		RESULT_1PIXEL_3x4;
		*dstPixelPtr = res;
		dstPixelPtr++;
		i += 10;
	}
#pragma pipeloop(0)
	for (; i < size; i++) {
		CALC_SRC_PTR;
		LOAD_1PIXEL_3x4;
		RESULT_1PIXEL_3x4;
		*dstPixelPtr = res;
		dstPixelPtr++;
	}
}

/* *********************************************************** */

#define	LOAD_1PIXEL_2x4                                               \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row0 = vis_faligndata(data0, data1);                          \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row1 = vis_faligndata(data0, data1);                          \
	filterposy = (y >> yf_shift) & yf_mask;                       \
	yFilter = *((mlib_f32 *)((mlib_u8 *)filterY + filterposy));   \
	filterposx = (x >> xf_shift) & xf_mask;                       \
	xFilter = *((mlib_d64 *)((mlib_u8 *)filterX + filterposx));   \
	x += dx;                                                      \
	y += dy

/* *********************************************************** */

#define	RESULT_1PIXEL_2x4                                               \
	v00 = vis_fmul8x16au(vis_read_hi(row0), yFilter);               \
	dr = vis_fpmerge(vis_read_hi(xFilter), vis_read_lo(xFilter));   \
	v01 = vis_fmul8x16au(vis_read_lo(row0), yFilter);               \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));             \
	v10 = vis_fmul8x16al(vis_read_hi(row1), yFilter);               \
	dr1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr));            \
	v11 = vis_fmul8x16al(vis_read_lo(row1), yFilter);               \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr));             \
	xFilter0 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));      \
	xFilter1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr1));      \
	sum0 = vis_fpadd16(v00, v10);                                   \
	sum1 = vis_fpadd16(v01, v11);                                   \
	v00 = vis_fmul8sux16(sum0, xFilter0);                           \
	v01 = vis_fmul8sux16(sum1, xFilter1);                           \
	v10 = vis_fmul8ulx16(sum0, xFilter0);                           \
	sum0 = vis_fpadd16(v00, v10);                                   \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                           \
	sum1 = vis_fpadd16(v01, v11);                                   \
	d0 = vis_fpadd16(sum0, sum1);                                   \
	res = vis_fpadd16s(vis_read_hi(d0), vis_read_lo(d0))

/* *********************************************************** */

#define	MAKE_2x4(index)                                                 \
	v00 = vis_fmul8x16au(vis_read_hi(row0), yFilter);               \
	dr = vis_fpmerge(vis_read_hi(xFilter), vis_read_lo(xFilter));   \
	v01 = vis_fmul8x16au(vis_read_lo(row0), yFilter);               \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));             \
	v10 = vis_fmul8x16al(vis_read_hi(row1), yFilter);               \
	dr1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr));            \
	v11 = vis_fmul8x16al(vis_read_lo(row1), yFilter);               \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr));             \
	xFilter0 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));      \
	xFilter1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr1));      \
	vis_alignaddr(sPtr, 0);                                         \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                 \
	data0 = dpSrc[0];                                               \
	sum0 = vis_fpadd16(v00, v10);                                   \
	filterposy = (y >> yf_shift);                                   \
	data1 = dpSrc[1];                                               \
	row0 = vis_faligndata(data0, data1);                            \
	filterposx = (x >> xf_shift);                                   \
	sPtr += srcStride;                                              \
	vis_alignaddr(sPtr, 0);                                         \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                 \
	data0 = dpSrc[0];                                               \
	sum1 = vis_fpadd16(v01, v11);                                   \
	x += dx;                                                        \
	data1 = dpSrc[1];                                               \
	row1 = vis_faligndata(data0, data1);                            \
	y += dy;                                                        \
	xSrc = (x >> x_shift) & ~1;                                     \
	ySrc = (y >> y_shift);                                          \
	filterposy &= yf_mask;                                          \
	v00 = vis_fmul8sux16(sum0, xFilter0);                           \
	v01 = vis_fmul8sux16(sum1, xFilter1);                           \
	v10 = vis_fmul8ulx16(sum0, xFilter0);                           \
	filterposx &= xf_mask;                                          \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                           \
	d0##index = vis_fpadd16(v00, v10);                              \
	yFilter = *((mlib_f32 *)((mlib_u8 *)filterY + filterposy));     \
	d1##index = vis_fpadd16(v01, v11);                              \
	xFilter = *((mlib_d64 *)((mlib_u8 *)filterX + filterposx));     \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + (xSrc)

/* *********************************************************** */

void
mlib_v_ImageAffineTableLine_8nw_2_4_2(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws)
{
	DECLAREVAR;
	mlib_d64 d1, d2, d3;
	mlib_d64 d10, d11, d12, d13;
	mlib_d64 v01, v11;
	mlib_d64 sum1;
	mlib_d64 xFilter, xFilter1;
	mlib_f32 yFilter;

	i = 0;

	dstPixelPtr = (mlib_f32 *)buff;

	if (i <= size - 10) {

		CALC_SRC_PTR;
		LOAD_1PIXEL_2x4;

		CALC_SRC_PTR;

		MAKE_2x4(0);
		MAKE_2x4(1);
		MAKE_2x4(2);
		MAKE_2x4(3);

		FADD_4x4;

		MAKE_2x4(0);
		MAKE_2x4(1);
		MAKE_2x4(2);
		MAKE_2x4(3);

#pragma pipeloop(0)
		for (; i <= size - 14; i += 4) {
			*buff = e0;
			buff++;
			*buff = e1;
			FADD_4x4;
			MAKE_2x4(0);
			MAKE_2x4(1);
			MAKE_2x4(2);
			MAKE_2x4(3);
			buff++;
		}

		*buff = e0;
		buff++;
		*buff = e1;
		buff++;
		FADD_4x4;
		*buff = e0;
		buff++;
		*buff = e1;
		buff++;

		dstPixelPtr = (mlib_f32 *)buff;

		RESULT_1PIXEL_2x4;
		*dstPixelPtr = res;
		dstPixelPtr++;

		LOAD_1PIXEL_2x4;
		RESULT_1PIXEL_2x4;
		*dstPixelPtr = res;
		dstPixelPtr++;
		i += 10;
	}
#pragma pipeloop(0)
	for (; i < size; i++) {
		CALC_SRC_PTR;
		LOAD_1PIXEL_2x4;
		RESULT_1PIXEL_2x4;
		*dstPixelPtr = res;
		dstPixelPtr++;
	}
}

/* *********************************************************** */

#define	LOAD_1PIXEL_4x2                                               \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row0 = vis_faligndata(data0, data1);                          \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row1 = vis_faligndata(data0, data1);                          \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row2 = vis_faligndata(data0, data1);                          \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row3 = vis_faligndata(data0, data1);                          \
	filterposy = (y >> yf_shift) & yf_mask;                       \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));   \
	filterposx = (x >> xf_shift) & xf_mask;                       \
	xFilter = *((mlib_f32 *)((mlib_u8 *)filterX + filterposx));   \
	x += dx;                                                      \
	y += dy

/* *********************************************************** */

#define	RESULT_1PIXEL_4x2                                                \
	v00 = vis_fmul8x16au(vis_read_hi(row0), vis_read_hi(yFilter));   \
	dr = vis_fpmerge(xFilter, xFilter);                              \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));              \
	v10 = vis_fmul8x16al(vis_read_hi(row1), vis_read_hi(yFilter));   \
	dr1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr));             \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr));              \
	v20 = vis_fmul8x16au(vis_read_hi(row2), vis_read_lo(yFilter));   \
	xFilter0 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));       \
	v30 = vis_fmul8x16al(vis_read_hi(row3), vis_read_lo(yFilter));   \
	sum0 = vis_fpadd16(v00, v10);                                    \
	sum0 = vis_fpadd16(sum0, v20);                                   \
	sum0 = vis_fpadd16(sum0, v30);                                   \
	v00 = vis_fmul8sux16(sum0, xFilter0);                            \
	v10 = vis_fmul8ulx16(sum0, xFilter0);                            \
	d0 = vis_fpadd16(v00, v10);                                      \
	res = vis_fpadd16s(vis_read_hi(d0), vis_read_lo(d0))

/* *********************************************************** */

#define	MAKE_4x2(index)                                                  \
	v00 = vis_fmul8x16au(vis_read_hi(row0), vis_read_hi(yFilter));   \
	dr = vis_fpmerge(xFilter, xFilter);                              \
	v10 = vis_fmul8x16al(vis_read_hi(row1), vis_read_hi(yFilter));   \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));              \
	v20 = vis_fmul8x16au(vis_read_hi(row2), vis_read_lo(yFilter));   \
	dr1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr));             \
	v30 = vis_fmul8x16al(vis_read_hi(row3), vis_read_lo(yFilter));   \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr));              \
	xFilter0 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));       \
	vis_alignaddr(sPtr, 0);                                          \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                  \
	data0 = dpSrc[0];                                                \
	sum0 = vis_fpadd16(v00, v10);                                    \
	filterposy = (y >> yf_shift);                                    \
	data1 = dpSrc[1];                                                \
	row0 = vis_faligndata(data0, data1);                             \
	filterposx = (x >> xf_shift);                                    \
	sPtr += srcStride;                                               \
	vis_alignaddr(sPtr, 0);                                          \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                  \
	data0 = dpSrc[0];                                                \
	x += dx;                                                         \
	data1 = dpSrc[1];                                                \
	sum0 = vis_fpadd16(sum0, v20);                                   \
	row1 = vis_faligndata(data0, data1);                             \
	sPtr += srcStride;                                               \
	vis_alignaddr(sPtr, 0);                                          \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                  \
	y += dy;                                                         \
	xSrc = (x >> x_shift) & ~1;                                      \
	data0 = dpSrc[0];                                                \
	ySrc = (y >> y_shift);                                           \
	sum0 = vis_fpadd16(sum0, v30);                                   \
	data1 = dpSrc[1];                                                \
	filterposy &= yf_mask;                                           \
	v00 = vis_fmul8sux16(sum0, xFilter0);                            \
	row2 = vis_faligndata(data0, data1);                             \
	sPtr += srcStride;                                               \
	v10 = vis_fmul8ulx16(sum0, xFilter0);                            \
	filterposx &= xf_mask;                                           \
	vis_alignaddr(sPtr, 0);                                          \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                  \
	data0 = dpSrc[0];                                                \
	d0##index = vis_fpadd16(v00, v10);                               \
	data1 = dpSrc[1];                                                \
	row3 = vis_faligndata(data0, data1);                             \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));      \
	xFilter = *((mlib_f32 *)((mlib_u8 *)filterX + filterposx));      \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + (xSrc)

/* *********************************************************** */

#define	FADD_4x2                                                 \
	p0 = vis_fpadd16s(vis_read_hi(d00), vis_read_lo(d00));   \
	p1 = vis_fpadd16s(vis_read_hi(d01), vis_read_lo(d01));   \
	p2 = vis_fpadd16s(vis_read_hi(d02), vis_read_lo(d02));   \
	p3 = vis_fpadd16s(vis_read_hi(d03), vis_read_lo(d03));   \
	e0 = vis_freg_pair(p0, p1);                              \
	e1 = vis_freg_pair(p2, p3)

/* *********************************************************** */

void
mlib_v_ImageAffineTableLine_8nw_4_2_2(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws)
{
	DECLAREVAR;
	mlib_d64 v20, v30;
	mlib_d64 row2, row3;
	mlib_f32 xFilter;
	mlib_d64 yFilter;

	i = 0;

	dstPixelPtr = (mlib_f32 *)buff;

	if (i <= size - 10) {

		CALC_SRC_PTR;
		LOAD_1PIXEL_4x2;

		CALC_SRC_PTR;

		MAKE_4x2(0);
		MAKE_4x2(1);
		MAKE_4x2(2);
		MAKE_4x2(3);

		FADD_4x2;

		MAKE_4x2(0);
		MAKE_4x2(1);
		MAKE_4x2(2);
		MAKE_4x2(3);

#pragma pipeloop(0)
		for (; i <= size - 14; i += 4) {
			*buff = e0;
			buff++;
			*buff = e1;
			FADD_4x2;
			MAKE_4x2(0);
			MAKE_4x2(1);
			MAKE_4x2(2);
			MAKE_4x2(3);
			buff++;
		}

		*buff = e0;
		buff++;
		*buff = e1;
		buff++;
		FADD_4x2;
		*buff = e0;
		buff++;
		*buff = e1;
		buff++;

		dstPixelPtr = (mlib_f32 *)buff;

		RESULT_1PIXEL_4x2;
		*dstPixelPtr = res;
		dstPixelPtr++;

		LOAD_1PIXEL_4x2;
		RESULT_1PIXEL_4x2;
		*dstPixelPtr = res;
		dstPixelPtr++;
		i += 10;
	}

	if (i >= size)
		return;

	CALC_SRC_PTR;
	LOAD_1PIXEL_4x2;

#pragma pipeloop(0)
	for (; i < size - 1; i++) {
		RESULT_1PIXEL_4x2;
		*dstPixelPtr = res;
		dstPixelPtr++;
		CALC_SRC_PTR;
		LOAD_1PIXEL_4x2;
	}

	RESULT_1PIXEL_4x2;
	*dstPixelPtr = res;
}

/* *********************************************************** */

#define	LOAD_1PIXEL_3x2                                               \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row0 = vis_faligndata(data0, data1);                          \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row1 = vis_faligndata(data0, data1);                          \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row2 = vis_faligndata(data0, data1);                          \
	filterposy = (y >> yf_shift) & yf_mask;                       \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));   \
	filterposx = (x >> xf_shift) & xf_mask;                       \
	xFilter = *((mlib_f32 *)((mlib_u8 *)filterX + filterposx));   \
	x += dx;                                                      \
	y += dy

/* *********************************************************** */

#define	RESULT_1PIXEL_3x2                                                \
	v00 = vis_fmul8x16au(vis_read_hi(row0), vis_read_hi(yFilter));   \
	dr = vis_fpmerge(xFilter, xFilter);                              \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));              \
	v10 = vis_fmul8x16al(vis_read_hi(row1), vis_read_hi(yFilter));   \
	dr1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr));             \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr));              \
	v20 = vis_fmul8x16au(vis_read_hi(row2), vis_read_lo(yFilter));   \
	xFilter0 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));       \
	sum0 = vis_fpadd16(v00, v10);                                    \
	sum0 = vis_fpadd16(sum0, v20);                                   \
	v00 = vis_fmul8sux16(sum0, xFilter0);                            \
	v10 = vis_fmul8ulx16(sum0, xFilter0);                            \
	d0 = vis_fpadd16(v00, v10);                                      \
	res = vis_fpadd16s(vis_read_hi(d0), vis_read_lo(d0))

/* *********************************************************** */

#define	MAKE_3x2(index)                                                  \
	v00 = vis_fmul8x16au(vis_read_hi(row0), vis_read_hi(yFilter));   \
	dr = vis_fpmerge(xFilter, xFilter);                              \
	v10 = vis_fmul8x16al(vis_read_hi(row1), vis_read_hi(yFilter));   \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));              \
	v20 = vis_fmul8x16au(vis_read_hi(row2), vis_read_lo(yFilter));   \
	dr1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr));             \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr));              \
	xFilter0 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));       \
	vis_alignaddr(sPtr, 0);                                          \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                  \
	data0 = dpSrc[0];                                                \
	sum0 = vis_fpadd16(v00, v10);                                    \
	filterposy = (y >> yf_shift);                                    \
	data1 = dpSrc[1];                                                \
	row0 = vis_faligndata(data0, data1);                             \
	filterposx = (x >> xf_shift);                                    \
	sPtr += srcStride;                                               \
	vis_alignaddr(sPtr, 0);                                          \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                  \
	data0 = dpSrc[0];                                                \
	x += dx;                                                         \
	data1 = dpSrc[1];                                                \
	sum0 = vis_fpadd16(sum0, v20);                                   \
	row1 = vis_faligndata(data0, data1);                             \
	sPtr += srcStride;                                               \
	vis_alignaddr(sPtr, 0);                                          \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                  \
	y += dy;                                                         \
	xSrc = (x >> x_shift) & ~1;                                      \
	data0 = dpSrc[0];                                                \
	ySrc = (y >> y_shift);                                           \
	data1 = dpSrc[1];                                                \
	filterposy &= yf_mask;                                           \
	v00 = vis_fmul8sux16(sum0, xFilter0);                            \
	row2 = vis_faligndata(data0, data1);                             \
	v10 = vis_fmul8ulx16(sum0, xFilter0);                            \
	filterposx &= xf_mask;                                           \
	d0##index = vis_fpadd16(v00, v10);                               \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));      \
	xFilter = *((mlib_f32 *)((mlib_u8 *)filterX + filterposx));      \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + (xSrc)

/* *********************************************************** */

void
mlib_v_ImageAffineTableLine_8nw_3_2_2(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws)
{
	DECLAREVAR;
	mlib_d64 v20;
	mlib_d64 row2;
	mlib_f32 xFilter;
	mlib_d64 yFilter;

	i = 0;

	dstPixelPtr = (mlib_f32 *)buff;

	if (i <= size - 10) {

		CALC_SRC_PTR;
		LOAD_1PIXEL_3x2;

		CALC_SRC_PTR;

		MAKE_3x2(0);
		MAKE_3x2(1);
		MAKE_3x2(2);
		MAKE_3x2(3);

		FADD_4x2;

		MAKE_3x2(0);
		MAKE_3x2(1);
		MAKE_3x2(2);
		MAKE_3x2(3);

#pragma pipeloop(0)
		for (; i <= size - 14; i += 4) {
			*buff = e0;
			buff++;
			*buff = e1;
			FADD_4x2;
			MAKE_3x2(0);
			MAKE_3x2(1);
			MAKE_3x2(2);
			MAKE_3x2(3);
			buff++;
		}

		*buff = e0;
		buff++;
		*buff = e1;
		buff++;
		FADD_4x2;
		*buff = e0;
		buff++;
		*buff = e1;
		buff++;

		dstPixelPtr = (mlib_f32 *)buff;

		RESULT_1PIXEL_3x2;
		*dstPixelPtr = res;
		dstPixelPtr++;

		LOAD_1PIXEL_3x2;
		RESULT_1PIXEL_3x2;
		*dstPixelPtr = res;
		dstPixelPtr++;
		i += 10;
	}
#pragma pipeloop(0)
	for (; i < size; i++) {
		CALC_SRC_PTR;
		LOAD_1PIXEL_3x2;
		RESULT_1PIXEL_3x2;
		*dstPixelPtr = res;
		dstPixelPtr++;
	}
}

/* *********************************************************** */

#define	LOAD_1PIXEL_2x2                                               \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row0 = vis_faligndata(data0, data1);                          \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row1 = vis_faligndata(data0, data1);                          \
	filterposy = (y >> yf_shift) & yf_mask;                       \
	yFilter = *((mlib_f32 *)((mlib_u8 *)filterY + filterposy));   \
	filterposx = (x >> xf_shift) & xf_mask;                       \
	xFilter = *((mlib_f32 *)((mlib_u8 *)filterX + filterposx));   \
	x += dx;                                                      \
	y += dy

/* *********************************************************** */

#define	RESULT_1PIXEL_2x2                                            \
	v00 = vis_fmul8x16au(vis_read_hi(row0), yFilter);            \
	dr = vis_fpmerge(xFilter, xFilter);                          \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));          \
	v10 = vis_fmul8x16al(vis_read_hi(row1), yFilter);            \
	dr1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr));         \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr));          \
	xFilter0 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));   \
	sum0 = vis_fpadd16(v00, v10);                                \
	v00 = vis_fmul8sux16(sum0, xFilter0);                        \
	v10 = vis_fmul8ulx16(sum0, xFilter0);                        \
	d0 = vis_fpadd16(v00, v10);                                  \
	res = vis_fpadd16s(vis_read_hi(d0), vis_read_lo(d0))

/* *********************************************************** */

#define	MAKE_2x2(index)                                               \
	v00 = vis_fmul8x16au(vis_read_hi(row0), yFilter);             \
	dr = vis_fpmerge(xFilter, xFilter);                           \
	v10 = vis_fmul8x16al(vis_read_hi(row1), yFilter);             \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));           \
	dr1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr));          \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr));           \
	xFilter0 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));    \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	sum0 = vis_fpadd16(v00, v10);                                 \
	filterposy = (y >> yf_shift);                                 \
	data1 = dpSrc[1];                                             \
	row0 = vis_faligndata(data0, data1);                          \
	filterposx = (x >> xf_shift);                                 \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	x += dx;                                                      \
	data1 = dpSrc[1];                                             \
	row1 = vis_faligndata(data0, data1);                          \
	y += dy;                                                      \
	xSrc = (x >> x_shift) & ~1;                                   \
	ySrc = (y >> y_shift);                                        \
	filterposy &= yf_mask;                                        \
	v00 = vis_fmul8sux16(sum0, xFilter0);                         \
	v10 = vis_fmul8ulx16(sum0, xFilter0);                         \
	filterposx &= xf_mask;                                        \
	d0##index = vis_fpadd16(v00, v10);                            \
	yFilter = *((mlib_f32 *)((mlib_u8 *)filterY + filterposy));   \
	xFilter = *((mlib_f32 *)((mlib_u8 *)filterX + filterposx));   \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + (xSrc)

/* *********************************************************** */

void
mlib_v_ImageAffineTableLine_8nw_2_2_2(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws)
{
	DECLAREVAR;
	mlib_f32 xFilter;
	mlib_f32 yFilter;

	i = 0;

	dstPixelPtr = (mlib_f32 *)buff;

	if (i <= size - 10) {

		CALC_SRC_PTR;
		LOAD_1PIXEL_2x2;

		CALC_SRC_PTR;

		MAKE_2x2(0);
		MAKE_2x2(1);
		MAKE_2x2(2);
		MAKE_2x2(3);

		FADD_4x2;

		MAKE_2x2(0);
		MAKE_2x2(1);
		MAKE_2x2(2);
		MAKE_2x2(3);

#pragma pipeloop(0)
		for (; i <= size - 14; i += 4) {
			*buff = e0;
			buff++;
			*buff = e1;
			FADD_4x2;
			MAKE_2x2(0);
			MAKE_2x2(1);
			MAKE_2x2(2);
			MAKE_2x2(3);
			buff++;
		}

		*buff = e0;
		buff++;
		*buff = e1;
		buff++;
		FADD_4x2;
		*buff = e0;
		buff++;
		*buff = e1;
		buff++;

		dstPixelPtr = (mlib_f32 *)buff;

		RESULT_1PIXEL_2x2;
		*dstPixelPtr = res;
		dstPixelPtr++;

		LOAD_1PIXEL_2x2;
		RESULT_1PIXEL_2x2;
		*dstPixelPtr = res;
		dstPixelPtr++;
		i += 10;
	}
#pragma pipeloop(0)
	for (; i < size; i++) {
		CALC_SRC_PTR;
		LOAD_1PIXEL_2x2;
		RESULT_1PIXEL_2x2;
		*dstPixelPtr = res;
		dstPixelPtr++;
	}
}

/* *********************************************************** */
