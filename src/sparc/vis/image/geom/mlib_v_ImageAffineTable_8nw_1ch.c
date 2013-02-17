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

#pragma ident	"@(#)mlib_v_ImageAffineTable_8nw_1ch.c	9.2	07/11/05 SMI"

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
	mlib_s16 *dstPixelPtr;                                  \
	mlib_s32 srcStride = ws->srcStride;                     \
	mlib_s32 i, filterposx, filterposy, xSrc, ySrc;         \
	mlib_s32 dx = ws->dx;                                   \
	mlib_s32 dy = ws->dy;                                   \
	mlib_s32 x_shift = ws->x_shift0;                        \
	mlib_s32 y_shift = ws->y_shift;                         \
	mlib_s32 xf_shift = ws->xf_shift;                       \
	mlib_s32 xf_mask = ws->xf_mask;                         \
	mlib_s32 yf_shift = ws->yf_shift;                       \
	mlib_s32 yf_mask = ws->yf_mask;                         \
	mlib_s32 size = ws->size;                               \
	mlib_s32 x = ws->x0;                                    \
	mlib_s32 y = ws->y0;                                    \
	mlib_d64 sum;                                           \
	mlib_d64 row00, row10;                                  \
	mlib_f32 p0, p1, p2, p3;                                \
	mlib_d64 v0, v1, v2, v3;                                \
	mlib_d64 e0, e1;                                        \
	mlib_d64 res

/* *********************************************************** */

#define	DECLAREVAR1                                             \
	mlib_d64 d0, d1, d2, d3;                                \
	mlib_d64 m02, m13, m0213;                               \
	mlib_d64 row01, row11

/* *********************************************************** */

#define	CALC_SRC_PTR                                            \
	xSrc = (x >> x_shift);                                  \
	ySrc = (y >> y_shift);                                  \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + xSrc

/* *********************************************************** */

#define	LOAD_1PIXEL_4x4                                               \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row00 = vis_faligndata(data0, data1);                         \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row10 = vis_faligndata(data0, data1);                         \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row20 = vis_faligndata(data0, data1);                         \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row30 = vis_faligndata(data0, data1);                         \
	filterposy = (y >> yf_shift) & yf_mask;                       \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));   \
	filterposx = (x >> xf_shift) & xf_mask;                       \
	xFilter = *((mlib_d64 *)((mlib_u8 *)filterX + filterposx));   \
	x += dx;                                                      \
	y += dy

/* *********************************************************** */

#define	RESULT_1PIXEL_4x4                                                \
	v0 = vis_fmul8x16au(vis_read_hi(row00), vis_read_hi(yFilter));   \
	v1 = vis_fmul8x16al(vis_read_hi(row10), vis_read_hi(yFilter));   \
	sum = vis_fpadd16(v0, v1);                                       \
	v2 = vis_fmul8x16au(vis_read_hi(row20), vis_read_lo(yFilter));   \
	sum = vis_fpadd16(sum, v2);                                      \
	v3 = vis_fmul8x16al(vis_read_hi(row30), vis_read_lo(yFilter));   \
	sum = vis_fpadd16(sum, v3);                                      \
	v0 = vis_fmul8sux16(sum, xFilter);                               \
	v1 = vis_fmul8ulx16(sum, xFilter);                               \
	v3 = vis_fpadd16(v1, v0);                                        \
	vis_alignaddr((void *)2, 0);                                     \
	v0 = vis_faligndata(v3, v3);                                     \
	v2 = vis_fpadd16(v3, v0);                                        \
	res =                                                            \
	vis_write_lo(res, vis_fpadd16s(vis_read_hi(v2),                  \
	    vis_read_lo(v2)))

/* *********************************************************** */

#define	MAKE_4x4(index, ind1, ind2)                                   \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	v0 =                                                          \
	vis_fmul8x16au(vis_read_hi(row0##ind1),                       \
	    vis_read_hi(yFilter));                                    \
	filterposy = (y >> yf_shift);                                 \
	data1 = dpSrc[1];                                             \
	v1 =                                                          \
	vis_fmul8x16al(vis_read_hi(row1##ind1),                       \
	    vis_read_hi(yFilter));                                    \
	row0##ind2 = vis_faligndata(data0, data1);                    \
	filterposx = (x >> xf_shift);                                 \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	sum = vis_fpadd16(v0, v1);                                    \
	data0 = dpSrc[0];                                             \
	v2 =                                                          \
	vis_fmul8x16au(vis_read_hi(row2##ind1),                       \
	    vis_read_lo(yFilter));                                    \
	x += dx;                                                      \
	data1 = dpSrc[1];                                             \
	row1##ind2 = vis_faligndata(data0, data1);                    \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	y += dy;                                                      \
	sum = vis_fpadd16(sum, v2);                                   \
	xSrc = (x >> x_shift);                                        \
	v3 =                                                          \
	vis_fmul8x16al(vis_read_hi(row3##ind1),                       \
	    vis_read_lo(yFilter));                                    \
	data0 = dpSrc[0];                                             \
	ySrc = (y >> y_shift);                                        \
	sum = vis_fpadd16(sum, v3);                                   \
	data1 = dpSrc[1];                                             \
	filterposy &= yf_mask;                                        \
	v0 = vis_fmul8sux16(sum, xFilter);                            \
	row2##ind2 = vis_faligndata(data0, data1);                    \
	sPtr += srcStride;                                            \
	v1 = vis_fmul8ulx16(sum, xFilter);                            \
	filterposx &= xf_mask;                                        \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	d##index = vis_fpadd16(v0, v1);                               \
	data1 = dpSrc[1];                                             \
	row3##ind2 = vis_faligndata(data0, data1);                    \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));   \
	xFilter = *((mlib_d64 *)((mlib_u8 *)filterX + filterposx));   \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + xSrc

/* *********************************************************** */

#define	FADD_4x4                                                    \
	p0 = vis_fpadd16s(vis_read_hi(d0), vis_read_lo(d0));        \
	p1 = vis_fpadd16s(vis_read_hi(d1), vis_read_lo(d1));        \
	p2 = vis_fpadd16s(vis_read_hi(d2), vis_read_lo(d2));        \
	p3 = vis_fpadd16s(vis_read_hi(d3), vis_read_lo(d3));        \
	m02 = vis_fpmerge(p0, p2);                                  \
	m13 = vis_fpmerge(p1, p3);                                  \
	m0213 = vis_fpmerge(vis_read_hi(m02), vis_read_hi(m13));    \
	e0 = vis_fpmerge(vis_read_hi(m0213), vis_read_lo(m0213));   \
	m0213 = vis_fpmerge(vis_read_lo(m02), vis_read_lo(m13));    \
	e1 = vis_fpmerge(vis_read_hi(m0213), vis_read_lo(m0213));   \
	res = vis_fpadd16(e0, e1)

/* *********************************************************** */

void
mlib_v_ImageAffineTableLine_8nw_4_4_1(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws)
{
	DECLAREVAR;
	DECLAREVAR1;
	mlib_d64 *dpSrc;
	mlib_d64 row20, row21, row30, row31;
	mlib_d64 data0, data1;
	mlib_d64 xFilter;
	mlib_d64 yFilter;

	i = 0;

	dstPixelPtr = (mlib_s16 *)buff;

	if (i <= size - 10) {

		CALC_SRC_PTR;
		LOAD_1PIXEL_4x4;

		CALC_SRC_PTR;

		MAKE_4x4(0, 0, 1);
		MAKE_4x4(1, 1, 0);
		MAKE_4x4(2, 0, 1);
		MAKE_4x4(3, 1, 0);

		FADD_4x4;

		MAKE_4x4(0, 0, 1);
		MAKE_4x4(1, 1, 0);
		MAKE_4x4(2, 0, 1);
		MAKE_4x4(3, 1, 0);

#pragma pipeloop(0)
		for (; i <= size - 14; i += 4) {
			*buff = res;
			FADD_4x4;
			MAKE_4x4(0, 0, 1);
			MAKE_4x4(1, 1, 0);
			MAKE_4x4(2, 0, 1);
			MAKE_4x4(3, 1, 0);
			buff++;
		}

		*buff = res;
		buff++;
		FADD_4x4;
		*buff = res;
		buff++;

		dstPixelPtr = (mlib_s16 *)buff;

		RESULT_1PIXEL_4x4;
		vis_st_u16(res, dstPixelPtr++);

		LOAD_1PIXEL_4x4;
		RESULT_1PIXEL_4x4;
		vis_st_u16(res, dstPixelPtr++);
		i += 10;
	}

	for (; i < size; i++) {
		CALC_SRC_PTR;
		LOAD_1PIXEL_4x4;
		RESULT_1PIXEL_4x4;
		vis_st_u16(res, dstPixelPtr++);
	}
}

/* *********************************************************** */

#define	LOAD_1PIXEL_3x4                                               \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row00 = vis_faligndata(data0, data1);                         \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row10 = vis_faligndata(data0, data1);                         \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row20 = vis_faligndata(data0, data1);                         \
	filterposy = (y >> yf_shift) & yf_mask;                       \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));   \
	filterposx = (x >> xf_shift) & xf_mask;                       \
	xFilter = *((mlib_d64 *)((mlib_u8 *)filterX + filterposx));   \
	x += dx;                                                      \
	y += dy

/* *********************************************************** */

#define	RESULT_1PIXEL_3x4                                                \
	v0 = vis_fmul8x16au(vis_read_hi(row00), vis_read_hi(yFilter));   \
	v1 = vis_fmul8x16al(vis_read_hi(row10), vis_read_hi(yFilter));   \
	sum = vis_fpadd16(v0, v1);                                       \
	v2 = vis_fmul8x16au(vis_read_hi(row20), vis_read_lo(yFilter));   \
	sum = vis_fpadd16(sum, v2);                                      \
	v0 = vis_fmul8sux16(sum, xFilter);                               \
	v1 = vis_fmul8ulx16(sum, xFilter);                               \
	v3 = vis_fpadd16(v1, v0);                                        \
	vis_alignaddr((void *)2, 0);                                     \
	v0 = vis_faligndata(v3, v3);                                     \
	v2 = vis_fpadd16(v3, v0);                                        \
	res =                                                            \
	vis_write_lo(res, vis_fpadd16s(vis_read_hi(v2),                  \
	    vis_read_lo(v2)))

/* *********************************************************** */

#define	MAKE_3x4(index, ind1, ind2)                                   \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	v0 =                                                          \
	vis_fmul8x16au(vis_read_hi(row0##ind1),                       \
	    vis_read_hi(yFilter));                                    \
	filterposy = (y >> yf_shift);                                 \
	data1 = dpSrc[1];                                             \
	v1 =                                                          \
	vis_fmul8x16al(vis_read_hi(row1##ind1),                       \
	    vis_read_hi(yFilter));                                    \
	row0##ind2 = vis_faligndata(data0, data1);                    \
	filterposx = (x >> xf_shift);                                 \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	sum = vis_fpadd16(v0, v1);                                    \
	data0 = dpSrc[0];                                             \
	v2 =                                                          \
	vis_fmul8x16au(vis_read_hi(row2##ind1),                       \
	    vis_read_lo(yFilter));                                    \
	x += dx;                                                      \
	data1 = dpSrc[1];                                             \
	row1##ind2 = vis_faligndata(data0, data1);                    \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	y += dy;                                                      \
	sum = vis_fpadd16(sum, v2);                                   \
	xSrc = (x >> x_shift);                                        \
	data0 = dpSrc[0];                                             \
	v0 = vis_fmul8sux16(sum, xFilter);                            \
	ySrc = (y >> y_shift);                                        \
	data1 = dpSrc[1];                                             \
	filterposy &= yf_mask;                                        \
	row2##ind2 = vis_faligndata(data0, data1);                    \
	v1 = vis_fmul8ulx16(sum, xFilter);                            \
	filterposx &= xf_mask;                                        \
	d##index = vis_fpadd16(v0, v1);                               \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));   \
	xFilter = *((mlib_d64 *)((mlib_u8 *)filterX + filterposx));   \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + xSrc

/* *********************************************************** */

void
mlib_v_ImageAffineTableLine_8nw_3_4_1(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws)
{
	DECLAREVAR;
	DECLAREVAR1;
	mlib_d64 *dpSrc;
	mlib_d64 row20, row21;
	mlib_d64 data0, data1;
	mlib_d64 xFilter;
	mlib_d64 yFilter;

	i = 0;

	dstPixelPtr = (mlib_s16 *)buff;

	if (i <= size - 10) {

		CALC_SRC_PTR;
		LOAD_1PIXEL_3x4;

		CALC_SRC_PTR;

		MAKE_3x4(0, 0, 1);
		MAKE_3x4(1, 1, 0);
		MAKE_3x4(2, 0, 1);
		MAKE_3x4(3, 1, 0);

		FADD_4x4;

		MAKE_3x4(0, 0, 1);
		MAKE_3x4(1, 1, 0);
		MAKE_3x4(2, 0, 1);
		MAKE_3x4(3, 1, 0);

#pragma pipeloop(0)
		for (; i <= size - 14; i += 4) {
			*buff = res;
			FADD_4x4;
			MAKE_3x4(0, 0, 1);
			MAKE_3x4(1, 1, 0);
			MAKE_3x4(2, 0, 1);
			MAKE_3x4(3, 1, 0);
			buff++;
		}

		*buff = res;
		buff++;
		FADD_4x4;
		*buff = res;
		buff++;

		dstPixelPtr = (mlib_s16 *)buff;

		RESULT_1PIXEL_3x4;
		vis_st_u16(res, dstPixelPtr++);

		LOAD_1PIXEL_3x4;
		RESULT_1PIXEL_3x4;
		vis_st_u16(res, dstPixelPtr++);
		i += 10;
	}

	for (; i < size; i++) {
		CALC_SRC_PTR;
		LOAD_1PIXEL_3x4;
		RESULT_1PIXEL_3x4;
		vis_st_u16(res, dstPixelPtr++);
	}
}

/* *********************************************************** */

#define	LOAD_1PIXEL_2x4                                               \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row00 = vis_faligndata(data0, data1);                         \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row10 = vis_faligndata(data0, data1);                         \
	filterposy = (y >> yf_shift) & yf_mask;                       \
	yFilter = *((mlib_f32 *)((mlib_u8 *)filterY + filterposy));   \
	filterposx = (x >> xf_shift) & xf_mask;                       \
	xFilter = *((mlib_d64 *)((mlib_u8 *)filterX + filterposx));   \
	x += dx;                                                      \
	y += dy

/* *********************************************************** */

#define	RESULT_1PIXEL_2x4                                       \
	v0 = vis_fmul8x16au(vis_read_hi(row00), yFilter);       \
	v1 = vis_fmul8x16al(vis_read_hi(row10), yFilter);       \
	sum = vis_fpadd16(v0, v1);                              \
	v0 = vis_fmul8sux16(sum, xFilter);                      \
	v1 = vis_fmul8ulx16(sum, xFilter);                      \
	v3 = vis_fpadd16(v1, v0);                               \
	vis_alignaddr((void *)2, 0);                            \
	v0 = vis_faligndata(v3, v3);                            \
	v2 = vis_fpadd16(v3, v0);                               \
	res =                                                   \
	vis_write_lo(res, vis_fpadd16s(vis_read_hi(v2),         \
	    vis_read_lo(v2)))

/* *********************************************************** */

#define	MAKE_2x4(index, ind1, ind2)                                   \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	v0 = vis_fmul8x16au(vis_read_hi(row0##ind1), yFilter);        \
	filterposy = (y >> yf_shift);                                 \
	data1 = dpSrc[1];                                             \
	v1 = vis_fmul8x16al(vis_read_hi(row1##ind1), yFilter);        \
	row0##ind2 = vis_faligndata(data0, data1);                    \
	filterposx = (x >> xf_shift);                                 \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	sum = vis_fpadd16(v0, v1);                                    \
	data0 = dpSrc[0];                                             \
	x += dx;                                                      \
	data1 = dpSrc[1];                                             \
	row1##ind2 = vis_faligndata(data0, data1);                    \
	y += dy;                                                      \
	xSrc = (x >> x_shift);                                        \
	filterposy &= yf_mask;                                        \
	v0 = vis_fmul8sux16(sum, xFilter);                            \
	ySrc = (y >> y_shift);                                        \
	v1 = vis_fmul8ulx16(sum, xFilter);                            \
	filterposx &= xf_mask;                                        \
	d##index = vis_fpadd16(v0, v1);                               \
	yFilter = *((mlib_f32 *)((mlib_u8 *)filterY + filterposy));   \
	xFilter = *((mlib_d64 *)((mlib_u8 *)filterX + filterposx));   \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + xSrc

/* *********************************************************** */

void
mlib_v_ImageAffineTableLine_8nw_2_4_1(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws)
{
	DECLAREVAR;
	DECLAREVAR1;
	mlib_d64 *dpSrc;
	mlib_d64 data0, data1;
	mlib_d64 xFilter;
	mlib_f32 yFilter;

	i = 0;

	dstPixelPtr = (mlib_s16 *)buff;

	if (i <= size - 10) {

		CALC_SRC_PTR;
		LOAD_1PIXEL_2x4;

		CALC_SRC_PTR;

		MAKE_2x4(0, 0, 1);
		MAKE_2x4(1, 1, 0);
		MAKE_2x4(2, 0, 1);
		MAKE_2x4(3, 1, 0);

		FADD_4x4;

		MAKE_2x4(0, 0, 1);
		MAKE_2x4(1, 1, 0);
		MAKE_2x4(2, 0, 1);
		MAKE_2x4(3, 1, 0);

#pragma pipeloop(0)
		for (; i <= size - 14; i += 4) {
			*buff = res;
			FADD_4x4;
			MAKE_2x4(0, 0, 1);
			MAKE_2x4(1, 1, 0);
			MAKE_2x4(2, 0, 1);
			MAKE_2x4(3, 1, 0);
			buff++;
		}

		*buff = res;
		buff++;
		FADD_4x4;
		*buff = res;
		buff++;

		dstPixelPtr = (mlib_s16 *)buff;

		RESULT_1PIXEL_2x4;
		vis_st_u16(res, dstPixelPtr++);

		LOAD_1PIXEL_2x4;
		RESULT_1PIXEL_2x4;
		vis_st_u16(res, dstPixelPtr++);
		i += 10;
	}

	for (; i < size; i++) {
		CALC_SRC_PTR;
		LOAD_1PIXEL_2x4;
		RESULT_1PIXEL_2x4;
		vis_st_u16(res, dstPixelPtr++);
	}
}

/* *********************************************************** */

#define	CALC_SRC_PTR_4x2                                        \
	xSrc = (x >> x_shift);                                  \
	ySrc = (y >> y_shift);                                  \
	sPtr = (mlib_u8 *)lineAddr[ySrc + 3] + xSrc

/* *********************************************************** */

#define	LOAD_4x2                                                      \
	row00 = vis_faligndata(vis_ld_u8_i(sPtr, 0), row00);          \
	row10 = vis_faligndata(vis_ld_u8_i(sPtr, 1), row10);          \
	sPtr -= srcStride;                                            \
	row00 = vis_faligndata(vis_ld_u8_i(sPtr, 0), row00);          \
	row10 = vis_faligndata(vis_ld_u8_i(sPtr, 1), row10);          \
	sPtr -= srcStride;                                            \
	row00 = vis_faligndata(vis_ld_u8_i(sPtr, 0), row00);          \
	row10 = vis_faligndata(vis_ld_u8_i(sPtr, 1), row10);          \
	sPtr -= srcStride;                                            \
	row00 = vis_faligndata(vis_ld_u8_i(sPtr, 0), row00);          \
	row10 = vis_faligndata(vis_ld_u8_i(sPtr, 1), row10);          \
	filterposy = (y >> yf_shift) & yf_mask;                       \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));   \
	filterposx = (x >> xf_shift) & xf_mask;                       \
	xFilter = *((mlib_f32 *)((mlib_u8 *)filterX + filterposx));   \
	x += dx;                                                      \
	y += dy

/* *********************************************************** */

#define	RESULT_1PIXEL_4x2                                       \
	v0 = vis_fmul8x16au(vis_read_hi(row00), xFilter);       \
	v1 = vis_fmul8x16al(vis_read_hi(row10), xFilter);       \
	sum = vis_fpadd16(v0, v1);                              \
	v0 = vis_fmul8sux16(sum, yFilter);                      \
	v1 = vis_fmul8ulx16(sum, yFilter);                      \
	v3 = vis_fpadd16(v1, v0);                               \
	p0 = vis_fpadd16s(vis_read_hi(v3), vis_read_lo(v3));    \
	v2 = vis_fmuld8ulx16(vis_scale, p0);                    \
	res =                                                   \
	vis_write_lo(res, vis_fpadd32s(vis_read_hi(v2),         \
	    vis_read_lo(v2)))

/* *********************************************************** */

#define	MAKE_4x2(index, ind1, ind2)                                   \
	filterposy = (y >> yf_shift);                                 \
	data0 = vis_ld_u8_i(sPtr, 0);                                 \
	v0 = vis_fmul8x16au(vis_read_hi(row0##ind1), xFilter);        \
	filterposx = (x >> xf_shift);                                 \
	data1 = vis_ld_u8_i(sPtr, 1);                                 \
	row0##ind2 = vis_faligndata(data0, row0##ind2);               \
	sPtr -= srcStride;                                            \
	data0 = vis_ld_u8_i(sPtr, 0);                                 \
	v1 = vis_fmul8x16al(vis_read_hi(row1##ind1), xFilter);        \
	y += dy;                                                      \
	row1##ind2 = vis_faligndata(data1, row1##ind2);               \
	data1 = vis_ld_u8_i(sPtr, 1);                                 \
	sum = vis_fpadd16(v0, v1);                                    \
	filterposy &= yf_mask;                                        \
	sPtr -= srcStride;                                            \
	ySrc = (y >> y_shift);                                        \
	row0##ind2 = vis_faligndata(data0, row0##ind2);               \
	x += dx;                                                      \
	data0 = vis_ld_u8_i(sPtr, 0);                                 \
	row1##ind2 = vis_faligndata(data1, row1##ind2);               \
	v0 = vis_fmul8sux16(sum, yFilter);                            \
	ySrc += 3;                                                    \
	xSrc = (x >> x_shift);                                        \
	data1 = vis_ld_u8_i(sPtr, 1);                                 \
	v1 = vis_fmul8ulx16(sum, yFilter);                            \
	sPtr -= srcStride;                                            \
	row0##ind2 = vis_faligndata(data0, row0##ind2);               \
	filterposy &= yf_mask;                                        \
	data0 = vis_ld_u8_i(sPtr, 0);                                 \
	row1##ind2 = vis_faligndata(data1, row1##ind2);               \
	data1 = vis_ld_u8_i(sPtr, 1);                                 \
	d##index = vis_fpadd16(v0, v1);                               \
	sPtr = (mlib_u8 *)lineAddr[ySrc];                             \
	filterposx &= xf_mask;                                        \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));   \
	row0##ind2 = vis_faligndata(data0, row0##ind2);               \
	xFilter = *((mlib_f32 *)((mlib_u8 *)filterX + filterposx));   \
	row1##ind2 = vis_faligndata(data1, row1##ind2);               \
	sPtr += xSrc

/* *********************************************************** */

void
mlib_v_ImageAffineTableLine_8nw_4_2_1(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws)
{
	DECLAREVAR;
	DECLAREVAR1;
	mlib_d64 data0, data1;
	mlib_f32 xFilter;
	mlib_d64 yFilter;
	mlib_f32 vis_scale = vis_to_float(0x00010001);

	vis_alignaddr((void *)7, 0);

	i = 0;

	dstPixelPtr = (mlib_s16 *)buff;

	if (i <= size - 10) {

		CALC_SRC_PTR_4x2;
		LOAD_4x2;
		CALC_SRC_PTR_4x2;

		MAKE_4x2(0, 0, 1);
		MAKE_4x2(1, 1, 0);
		MAKE_4x2(2, 0, 1);
		MAKE_4x2(3, 1, 0);

		FADD_4x4;

		MAKE_4x2(0, 0, 1);
		MAKE_4x2(1, 1, 0);
		MAKE_4x2(2, 0, 1);
		MAKE_4x2(3, 1, 0);

#pragma pipeloop(0)
		for (; i <= size - 14; i += 4) {
			*buff = res;
			FADD_4x4;
			MAKE_4x2(0, 0, 1);
			MAKE_4x2(1, 1, 0);
			MAKE_4x2(2, 0, 1);
			MAKE_4x2(3, 1, 0);
			buff++;
		}

		*buff = res;
		buff++;
		FADD_4x4;
		*buff = res;
		buff++;

		dstPixelPtr = (mlib_s16 *)buff;

		RESULT_1PIXEL_4x2;
		vis_st_u16(res, dstPixelPtr++);

		LOAD_4x2;

		RESULT_1PIXEL_4x2;
		vis_st_u16(res, dstPixelPtr++);
		i += 10;
	}
#pragma pipeloop(0)
	for (; i < size; i++) {
		CALC_SRC_PTR_4x2;
		LOAD_4x2;
		RESULT_1PIXEL_4x2;
		vis_st_u16(res, dstPixelPtr++);
	}
}

/* *********************************************************** */

#define	CALC_SRC_PTR_3x2                                        \
	xSrc = (x >> x_shift);                                  \
	ySrc = (y >> y_shift);                                  \
	sPtr = (mlib_u8 *)lineAddr[ySrc + 2] + xSrc

/* *********************************************************** */

#define	LOAD_3x2                                                      \
	row00 = vis_faligndata(vis_ld_u8_i(sPtr, 0), row00);          \
	row10 = vis_faligndata(vis_ld_u8_i(sPtr, 1), row10);          \
	sPtr -= srcStride;                                            \
	row00 = vis_faligndata(vis_ld_u8_i(sPtr, 0), row00);          \
	row10 = vis_faligndata(vis_ld_u8_i(sPtr, 1), row10);          \
	sPtr -= srcStride;                                            \
	row00 = vis_faligndata(vis_ld_u8_i(sPtr, 0), row00);          \
	row10 = vis_faligndata(vis_ld_u8_i(sPtr, 1), row10);          \
	filterposy = (y >> yf_shift) & yf_mask;                       \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));   \
	filterposx = (x >> xf_shift) & xf_mask;                       \
	xFilter = *((mlib_f32 *)((mlib_u8 *)filterX + filterposx));   \
	x += dx;                                                      \
	y += dy

/* *********************************************************** */

#define	MAKE_3x2(index, ind1, ind2)                                   \
	row0##ind2 =                                                  \
	vis_faligndata(vis_ld_u8_i(sPtr, 0), row0##ind2);             \
	row1##ind2 =                                                  \
	vis_faligndata(vis_ld_u8_i(sPtr, 1), row1##ind2);             \
	sPtr -= srcStride;                                            \
	row0##ind2 =                                                  \
	vis_faligndata(vis_ld_u8_i(sPtr, 0), row0##ind2);             \
	row1##ind2 =                                                  \
	vis_faligndata(vis_ld_u8_i(sPtr, 1), row1##ind2);             \
	sPtr -= srcStride;                                            \
	row0##ind2 =                                                  \
	vis_faligndata(vis_ld_u8_i(sPtr, 0), row0##ind2);             \
	row1##ind2 =                                                  \
	vis_faligndata(vis_ld_u8_i(sPtr, 1), row1##ind2);             \
	v0 = vis_fmul8x16au(vis_read_hi(row0##ind1), xFilter);        \
	v1 = vis_fmul8x16al(vis_read_hi(row1##ind1), xFilter);        \
	sum = vis_fpadd16(v0, v1);                                    \
	v0 = vis_fmul8sux16(sum, yFilter);                            \
	v1 = vis_fmul8ulx16(sum, yFilter);                            \
	d##index = vis_fpadd16(v0, v1);                               \
	filterposy = (y >> yf_shift) & yf_mask;                       \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));   \
	filterposx = (x >> xf_shift) & xf_mask;                       \
	xFilter = *((mlib_f32 *)((mlib_u8 *)filterX + filterposx));   \
	x += dx;                                                      \
	y += dy;                                                      \
	xSrc = (x >> x_shift);                                        \
	ySrc = (y >> y_shift);                                        \
	sPtr = (mlib_u8 *)lineAddr[ySrc + 2] + xSrc

/* *********************************************************** */

void
mlib_v_ImageAffineTableLine_8nw_3_2_1(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws)
{
	DECLAREVAR;
	DECLAREVAR1;
	mlib_f32 xFilter;
	mlib_d64 yFilter;
	mlib_f32 vis_scale = vis_to_float(0x00010001);

	row00 = vis_to_double_dup(0);
	row10 = vis_to_double_dup(0);
	row01 = vis_to_double_dup(0);
	row11 = vis_to_double_dup(0);

	vis_alignaddr((void *)7, 0);

	i = 0;

	dstPixelPtr = (mlib_s16 *)buff;

	if (i <= size - 10) {

		CALC_SRC_PTR_3x2;
		LOAD_3x2;
		CALC_SRC_PTR_3x2;

		MAKE_3x2(0, 0, 1);
		MAKE_3x2(1, 1, 0);
		MAKE_3x2(2, 0, 1);
		MAKE_3x2(3, 1, 0);

		FADD_4x4;

		MAKE_3x2(0, 0, 1);
		MAKE_3x2(1, 1, 0);
		MAKE_3x2(2, 0, 1);
		MAKE_3x2(3, 1, 0);

#pragma pipeloop(0)
		for (i = 0; i <= size - 14; i += 4) {
			*buff = res;
			FADD_4x4;
			MAKE_3x2(0, 0, 1);
			MAKE_3x2(1, 1, 0);
			MAKE_3x2(2, 0, 1);
			MAKE_3x2(3, 1, 0);
			buff++;
		}

		*buff = res;
		buff++;
		FADD_4x4;
		*buff = res;
		buff++;

		dstPixelPtr = (mlib_s16 *)buff;

		RESULT_1PIXEL_4x2;
		vis_st_u16(res, dstPixelPtr++);

		LOAD_3x2;

		RESULT_1PIXEL_4x2;
		vis_st_u16(res, dstPixelPtr++);
		i += 10;
	}
#pragma pipeloop(0)
	for (; i < size; i++) {
		CALC_SRC_PTR_3x2;
		LOAD_3x2;
		RESULT_1PIXEL_4x2;
		vis_st_u16(res, dstPixelPtr++);
	}
}

/* *********************************************************** */

#define	CALC_SRC_PTR_2x2                                         \
	xSrc = (x >> x_shift);                                   \
	ySrc = (y >> y_shift);                                   \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + xSrc;                 \
	filterposy = (y >> yf_shift) & yf_mask;                  \
	p1 = *((mlib_f32 *)((mlib_u8 *)filterY + filterposy));   \
	filterposx = (x >> xf_shift) & xf_mask;                  \
	p0 = *((mlib_f32 *)((mlib_u8 *)filterX + filterposx));   \
	x += dx;                                                 \
	y += dy;                                                 \
	xSrc = (x >> x_shift);                                   \
	ySrc = (y >> y_shift);                                   \
	sPtr1 = (mlib_u8 *)lineAddr[ySrc] + xSrc;                \
	filterposy = (y >> yf_shift) & yf_mask;                  \
	p3 = *((mlib_f32 *)((mlib_u8 *)filterY + filterposy));   \
	filterposx = (x >> xf_shift) & xf_mask;                  \
	p2 = *((mlib_f32 *)((mlib_u8 *)filterX + filterposx));   \
	x += dx;                                                 \
	y += dy

/* *********************************************************** */

#define	LOAD_2x2                                                           \
	row00 =                                                            \
	vis_faligndata(vis_ld_u8_i(sPtr1, srcStride + 1), row00);          \
	row00 = vis_faligndata(vis_ld_u8_i(sPtr, srcStride + 1), row00);   \
	row00 = vis_faligndata(vis_ld_u8_i(sPtr1, 0), row00);              \
	row00 = vis_faligndata(vis_ld_u8_i(sPtr, 0), row00);               \
	row10 = vis_faligndata(vis_ld_u8_i(sPtr1, 1), row10);              \
	row10 = vis_faligndata(vis_ld_u8_i(sPtr, 1), row10);               \
	row10 = vis_faligndata(vis_ld_u8_i(sPtr1, srcStride), row10);      \
	row10 = vis_faligndata(vis_ld_u8_i(sPtr, srcStride), row10)

/* *********************************************************** */

#define	FILTER_MERGE                                                    \
	xFilter = vis_fpmerge(p0, p1);                                  \
	xFilter =                                                       \
	vis_fpmerge(vis_read_hi(xFilter), vis_read_lo(xFilter));        \
	yFilter = vis_fpmerge(p2, p3);                                  \
	yFilter =                                                       \
	vis_fpmerge(vis_read_hi(yFilter), vis_read_lo(yFilter));        \
	e0 = vis_fpmerge(vis_read_hi(xFilter), vis_read_hi(yFilter));   \
	e1 = vis_fpmerge(vis_read_lo(xFilter), vis_read_lo(yFilter));   \
	xFilter = vis_fpmerge(vis_read_hi(e0), vis_read_hi(e1));        \
	yFilter = vis_fpmerge(vis_read_lo(e0), vis_read_lo(e1));        \
	yFilter1 =                                                      \
	vis_freg_pair(vis_read_lo(yFilter), vis_read_hi(yFilter))

/* *********************************************************** */

#define	MAKE_2x2                                                \
	v0 = vis_fmul8x16(vis_read_hi(row00), yFilter);         \
	v1 = vis_fmul8x16(vis_read_hi(row10), yFilter1);        \
	sum = vis_fpadd16(v0, v1);                              \
	v0 = vis_fmul8sux16(sum, xFilter);                      \
	v1 = vis_fmul8ulx16(sum, xFilter);                      \
	v3 = vis_fpadd16(v1, v0);                               \
	res1 = vis_fpadd16s(vis_read_hi(v3), vis_read_lo(v3))

/* *********************************************************** */

void
mlib_v_ImageAffineTableLine_8nw_2_2_1(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws)
{
	DECLAREVAR;
	mlib_d64 xFilter;
	mlib_d64 yFilter;
	mlib_d64 yFilter1;
	mlib_f32 *buff1 = (mlib_f32 *)buff;
	mlib_f32 res1;
	mlib_u8 *sPtr1;
	mlib_f32 vis_scale = vis_to_float(0x00010001);

	vis_alignaddr((void *)7, 0);
	dstPixelPtr = (mlib_s16 *)buff;

	i = 0;

	if (i <= size - 4) {

		CALC_SRC_PTR_2x2;
		LOAD_2x2;
		FILTER_MERGE;

		CALC_SRC_PTR_2x2;

#pragma pipeloop(0)
		for (i = 0; i <= size - 6; i += 2) {
			MAKE_2x2;
			LOAD_2x2;
			*buff1 = res1;
			FILTER_MERGE;
			CALC_SRC_PTR_2x2;
			buff1++;
		}

		MAKE_2x2;
		LOAD_2x2;
		*buff1 = res1;
		buff1++;
		FILTER_MERGE;
		MAKE_2x2;
		*buff1 = res1;
		buff1++;

		dstPixelPtr = (mlib_s16 *)buff1;
		i += 4;
	}
#pragma pipeloop(0)
	for (; i < size; i++) {
		xSrc = (x >> x_shift);
		ySrc = (y >> y_shift);
		sPtr = (mlib_u8 *)lineAddr[ySrc] + xSrc;
		filterposy = (y >> yf_shift) & yf_mask;
		p1 = *((mlib_f32 *)((mlib_u8 *)filterY + filterposy));
		filterposx = (x >> xf_shift) & xf_mask;
		p0 = *((mlib_f32 *)((mlib_u8 *)filterX + filterposx));
		x += dx;
		y += dy;
		xFilter = vis_write_hi(xFilter, p0);

		row00 = vis_faligndata(vis_ld_u8_i(sPtr, 1), row00);
		row00 = vis_faligndata(vis_ld_u8_i(sPtr, 0), row00);
		row10 = vis_faligndata(vis_ld_u8_i(sPtr, srcStride + 1), row10);
		row10 = vis_faligndata(vis_ld_u8_i(sPtr, srcStride), row10);

		v0 = vis_fmul8x16au(vis_read_hi(row00), p1);
		v1 = vis_fmul8x16al(vis_read_hi(row10), p1);
		sum = vis_fpadd16(v0, v1);
		v0 = vis_fmul8sux16(sum, xFilter);
		v1 = vis_fmul8ulx16(sum, xFilter);
		v3 = vis_fpadd16(v1, v0);
		v2 = vis_fmuld8ulx16(vis_scale, vis_read_hi(v3));
		res =
		    vis_write_lo(res, vis_fpadd32s(vis_read_hi(v2),
		    vis_read_lo(v2)));

		vis_st_u16(res, dstPixelPtr++);
	}
}

/* *********************************************************** */
