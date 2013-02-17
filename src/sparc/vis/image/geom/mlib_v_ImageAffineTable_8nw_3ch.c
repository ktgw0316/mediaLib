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

#pragma ident	"@(#)mlib_v_ImageAffineTable_8nw_3ch.c	9.2	07/11/05 SMI"

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
	mlib_d64 *xPtr;                                         \
	mlib_d64 *dpSrc;                                        \
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
	mlib_d64 row00, row10;                                  \
	mlib_d64 v00, v10;                                      \
	mlib_d64 v01, v11;                                      \
	mlib_d64 d0, d1, d2;                                    \
	mlib_d64 data0, data1;                                  \
	mlib_d64 xFilter0, xFilter1;                            \
	mlib_d64 sum0, sum1;                                    \
	mlib_d64 res

/* *********************************************************** */

#define	CALC_SRC_PTR                                            \
	xSrc = (x >> x_shift);                                  \
	ySrc = (y >> y_shift);                                  \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + (3 * xSrc)

/* *********************************************************** */

#define	LOAD_1PIXEL_4x4                                               \
	filterposy = (y >> yf_shift) & yf_mask;                       \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));   \
	filterposx = (x >> xf_shift) & xf_mask;                       \
	xPtr = ((mlib_d64 *)((mlib_u8 *)filterX + 3 * filterposx));   \
	xFilter0 = xPtr[0];                                           \
	xFilter1 = xPtr[1];                                           \
	xFilter2 = xPtr[2];                                           \
	x += dx;                                                      \
	y += dy;                                                      \
	dpSrc = vis_alignaddr(sPtr, 0);                               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	data2 = dpSrc[2];                                             \
	row00 = vis_faligndata(data0, data1);                         \
	row01 = vis_faligndata(data1, data2);                         \
	sPtr += srcStride;                                            \
	dpSrc = vis_alignaddr(sPtr, 0);                               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	data2 = dpSrc[2];                                             \
	row10 = vis_faligndata(data0, data1);                         \
	row11 = vis_faligndata(data1, data2);                         \
	sPtr += srcStride;                                            \
	dpSrc = vis_alignaddr(sPtr, 0);                               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	data2 = dpSrc[2];                                             \
	row20 = vis_faligndata(data0, data1);                         \
	row21 = vis_faligndata(data1, data2);                         \
	sPtr += srcStride;                                            \
	dpSrc = vis_alignaddr(sPtr, 0);                               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	data2 = dpSrc[2];                                             \
	row30 = vis_faligndata(data0, data1);                         \
	row31 = vis_faligndata(data1, data2)

/* *********************************************************** */

#define	RESULT_1PIXEL_4x4                                                 \
	v00 = vis_fmul8x16au(vis_read_hi(row00), vis_read_hi(yFilter));   \
	v01 = vis_fmul8x16au(vis_read_lo(row00), vis_read_hi(yFilter));   \
	v02 = vis_fmul8x16au(vis_read_hi(row01), vis_read_hi(yFilter));   \
	v10 = vis_fmul8x16al(vis_read_hi(row10), vis_read_hi(yFilter));   \
	v11 = vis_fmul8x16al(vis_read_lo(row10), vis_read_hi(yFilter));   \
	v12 = vis_fmul8x16al(vis_read_hi(row11), vis_read_hi(yFilter));   \
	v20 = vis_fmul8x16au(vis_read_hi(row20), vis_read_lo(yFilter));   \
	sum0 = vis_fpadd16(v00, v10);                                     \
	v21 = vis_fmul8x16au(vis_read_lo(row20), vis_read_lo(yFilter));   \
	sum1 = vis_fpadd16(v01, v11);                                     \
	v22 = vis_fmul8x16au(vis_read_hi(row21), vis_read_lo(yFilter));   \
	sum2 = vis_fpadd16(v02, v12);                                     \
	v30 = vis_fmul8x16al(vis_read_hi(row30), vis_read_lo(yFilter));   \
	sum0 = vis_fpadd16(sum0, v20);                                    \
	v31 = vis_fmul8x16al(vis_read_lo(row30), vis_read_lo(yFilter));   \
	sum1 = vis_fpadd16(sum1, v21);                                    \
	v32 = vis_fmul8x16al(vis_read_hi(row31), vis_read_lo(yFilter));   \
	sum2 = vis_fpadd16(sum2, v22);                                    \
	sum0 = vis_fpadd16(sum0, v30);                                    \
	sum1 = vis_fpadd16(sum1, v31);                                    \
	v00 = vis_fmul8sux16(sum0, xFilter0);                             \
	sum2 = vis_fpadd16(sum2, v32);                                    \
	v01 = vis_fmul8ulx16(sum0, xFilter0);                             \
	v10 = vis_fmul8sux16(sum1, xFilter1);                             \
	d0 = vis_fpadd16(v00, v01);                                       \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                             \
	v20 = vis_fmul8sux16(sum2, xFilter2);                             \
	d1 = vis_fpadd16(v10, v11);                                       \
	v21 = vis_fmul8ulx16(sum2, xFilter2);                             \
	d2 = vis_fpadd16(v20, v21);                                       \
	vis_alignaddr((void *)6, 0);                                      \
	d3 = vis_faligndata(d0, d1);                                      \
	vis_alignaddr((void *)2, 0);                                      \
	d4 = vis_faligndata(d1, d2);                                      \
	d0 = vis_fpadd16(d0, d3);                                         \
	d2 = vis_fpadd16(d2, d4);                                         \
	d1 = vis_faligndata(d2, d2);                                      \
	res = vis_fpadd16(d0, d1)

/* *********************************************************** */

#define	MAKE_4x4                                                          \
	v00 = vis_fmul8x16au(vis_read_hi(row00), vis_read_hi(yFilter));   \
	v01 = vis_fmul8x16au(vis_read_lo(row00), vis_read_hi(yFilter));   \
	v02 = vis_fmul8x16au(vis_read_hi(row01), vis_read_hi(yFilter));   \
	dpSrc = vis_alignaddr(sPtr, 0);                                   \
	data0 = dpSrc[0];                                                 \
	filterposy = (y >> yf_shift);                                     \
	v10 = vis_fmul8x16al(vis_read_hi(row10), vis_read_hi(yFilter));   \
	data1 = dpSrc[1];                                                 \
	v11 = vis_fmul8x16al(vis_read_lo(row10), vis_read_hi(yFilter));   \
	sum0 = vis_fpadd16(v00, v10);                                     \
	data2 = dpSrc[2];                                                 \
	row00 = vis_faligndata(data0, data1);                             \
	v12 = vis_fmul8x16al(vis_read_hi(row11), vis_read_hi(yFilter));   \
	row01 = vis_faligndata(data1, data2);                             \
	filterposx = (x >> xf_shift);                                     \
	sPtr += srcStride;                                                \
	dpSrc = vis_alignaddr(sPtr, 0);                                   \
	v20 = vis_fmul8x16au(vis_read_hi(row20), vis_read_lo(yFilter));   \
	sum1 = vis_fpadd16(v01, v11);                                     \
	data0 = dpSrc[0];                                                 \
	x += dx;                                                          \
	data1 = dpSrc[1];                                                 \
	v21 = vis_fmul8x16au(vis_read_lo(row20), vis_read_lo(yFilter));   \
	sum2 = vis_fpadd16(v02, v12);                                     \
	data2 = dpSrc[2];                                                 \
	row10 = vis_faligndata(data0, data1);                             \
	v22 = vis_fmul8x16au(vis_read_hi(row21), vis_read_lo(yFilter));   \
	row11 = vis_faligndata(data1, data2);                             \
	sPtr += srcStride;                                                \
	dpSrc = vis_alignaddr(sPtr, 0);                                   \
	y += dy;                                                          \
	xSrc = (x >> x_shift);                                            \
	v30 = vis_fmul8x16al(vis_read_hi(row30), vis_read_lo(yFilter));   \
	sum0 = vis_fpadd16(sum0, v20);                                    \
	data0 = dpSrc[0];                                                 \
	ySrc = (y >> y_shift);                                            \
	data1 = dpSrc[1];                                                 \
	v31 = vis_fmul8x16al(vis_read_lo(row30), vis_read_lo(yFilter));   \
	sum1 = vis_fpadd16(sum1, v21);                                    \
	data2 = dpSrc[2];                                                 \
	filterposy &= yf_mask;                                            \
	row20 = vis_faligndata(data0, data1);                             \
	v32 = vis_fmul8x16al(vis_read_hi(row31), vis_read_lo(yFilter));   \
	row21 = vis_faligndata(data1, data2);                             \
	sPtr += srcStride;                                                \
	filterposx &= xf_mask;                                            \
	sum2 = vis_fpadd16(sum2, v22);                                    \
	dpSrc = vis_alignaddr(sPtr, 0);                                   \
	sum0 = vis_fpadd16(sum0, v30);                                    \
	data0 = dpSrc[0];                                                 \
	sum1 = vis_fpadd16(sum1, v31);                                    \
	v00 = vis_fmul8sux16(sum0, xFilter0);                             \
	data1 = dpSrc[1];                                                 \
	sum2 = vis_fpadd16(sum2, v32);                                    \
	v01 = vis_fmul8ulx16(sum0, xFilter0);                             \
	data2 = dpSrc[2];                                                 \
	row30 = vis_faligndata(data0, data1);                             \
	v10 = vis_fmul8sux16(sum1, xFilter1);                             \
	d0 = vis_fpadd16(v00, v01);                                       \
	row31 = vis_faligndata(data1, data2);                             \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));       \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                             \
	xPtr = ((mlib_d64 *)((mlib_u8 *)filterX + 3 * filterposx));       \
	xFilter0 = xPtr[0];                                               \
	v20 = vis_fmul8sux16(sum2, xFilter2);                             \
	d1 = vis_fpadd16(v10, v11);                                       \
	xFilter1 = xPtr[1];                                               \
	v21 = vis_fmul8ulx16(sum2, xFilter2);                             \
	xFilter2 = xPtr[2];                                               \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + (3 * xSrc);                    \
	d2 = vis_fpadd16(v20, v21)

/* *********************************************************** */

#define	FADD_4x4                                                \
	vis_alignaddr((void *)6, 0);                            \
	d3 = vis_faligndata(d0, d1);                            \
	vis_alignaddr((void *)2, 0);                            \
	d4 = vis_faligndata(d1, d2);                            \
	d0 = vis_fpadd16(d0, d3);                               \
	d2 = vis_fpadd16(d2, d4);                               \
	d1 = vis_faligndata(d2, d2);                            \
	res = vis_fpadd16(d0, d1)

/* *********************************************************** */

void
mlib_v_ImageAffineTableLine_8nw_4_4_3(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws)
{
	DECLAREVAR;
	mlib_d64 row01, row11, row20, row21, row30, row31;
	mlib_d64 v20, v30;
	mlib_d64 v21, v31;
	mlib_d64 v02, v12, v22, v32;
	mlib_d64 d3, d4;
	mlib_d64 data2;
	mlib_d64 xFilter2;
	mlib_d64 sum2;
	mlib_d64 yFilter;

	i = 0;

	if (i <= size - 4) {

		CALC_SRC_PTR;
		LOAD_1PIXEL_4x4;

		CALC_SRC_PTR;
		MAKE_4x4;
		FADD_4x4;
		MAKE_4x4;

#pragma pipeloop(0)
		for (; i < size - 4; i++) {
			*buff = res;
			FADD_4x4;
			MAKE_4x4;
			buff++;
		}

		*buff = res;
		buff++;
		FADD_4x4;
		*buff = res;
		buff++;

		RESULT_1PIXEL_4x4;
		*buff = res;
		buff++;

		LOAD_1PIXEL_4x4;
		RESULT_1PIXEL_4x4;
		*buff = res;
		buff++;
		i += 4;
	}
#pragma pipeloop(0)
	for (; i < size; i++) {
		CALC_SRC_PTR;
		LOAD_1PIXEL_4x4;
		RESULT_1PIXEL_4x4;
		*buff = res;
		buff++;
	}
}

/* *********************************************************** */

#define	LOAD_1PIXEL_3x4                                               \
	filterposy = (y >> yf_shift) & yf_mask;                       \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));   \
	filterposx = (x >> xf_shift) & xf_mask;                       \
	xPtr = ((mlib_d64 *)((mlib_u8 *)filterX + 3 * filterposx));   \
	xFilter0 = xPtr[0];                                           \
	xFilter1 = xPtr[1];                                           \
	xFilter2 = xPtr[2];                                           \
	x += dx;                                                      \
	y += dy;                                                      \
	dpSrc = vis_alignaddr(sPtr, 0);                               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	data2 = dpSrc[2];                                             \
	row00 = vis_faligndata(data0, data1);                         \
	row01 = vis_faligndata(data1, data2);                         \
	sPtr += srcStride;                                            \
	dpSrc = vis_alignaddr(sPtr, 0);                               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	data2 = dpSrc[2];                                             \
	row10 = vis_faligndata(data0, data1);                         \
	row11 = vis_faligndata(data1, data2);                         \
	sPtr += srcStride;                                            \
	dpSrc = vis_alignaddr(sPtr, 0);                               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	data2 = dpSrc[2];                                             \
	row20 = vis_faligndata(data0, data1);                         \
	row21 = vis_faligndata(data1, data2)

/* *********************************************************** */

#define	RESULT_1PIXEL_3x4                                                 \
	v00 = vis_fmul8x16au(vis_read_hi(row00), vis_read_hi(yFilter));   \
	v01 = vis_fmul8x16au(vis_read_lo(row00), vis_read_hi(yFilter));   \
	v02 = vis_fmul8x16au(vis_read_hi(row01), vis_read_hi(yFilter));   \
	v10 = vis_fmul8x16al(vis_read_hi(row10), vis_read_hi(yFilter));   \
	v11 = vis_fmul8x16al(vis_read_lo(row10), vis_read_hi(yFilter));   \
	v12 = vis_fmul8x16al(vis_read_hi(row11), vis_read_hi(yFilter));   \
	v20 = vis_fmul8x16au(vis_read_hi(row20), vis_read_lo(yFilter));   \
	sum0 = vis_fpadd16(v00, v10);                                     \
	v21 = vis_fmul8x16au(vis_read_lo(row20), vis_read_lo(yFilter));   \
	sum1 = vis_fpadd16(v01, v11);                                     \
	v22 = vis_fmul8x16au(vis_read_hi(row21), vis_read_lo(yFilter));   \
	sum2 = vis_fpadd16(v02, v12);                                     \
	sum0 = vis_fpadd16(sum0, v20);                                    \
	sum1 = vis_fpadd16(sum1, v21);                                    \
	sum2 = vis_fpadd16(sum2, v22);                                    \
	v00 = vis_fmul8sux16(sum0, xFilter0);                             \
	v01 = vis_fmul8ulx16(sum0, xFilter0);                             \
	v10 = vis_fmul8sux16(sum1, xFilter1);                             \
	d0 = vis_fpadd16(v00, v01);                                       \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                             \
	v20 = vis_fmul8sux16(sum2, xFilter2);                             \
	d1 = vis_fpadd16(v10, v11);                                       \
	v21 = vis_fmul8ulx16(sum2, xFilter2);                             \
	d2 = vis_fpadd16(v20, v21);                                       \
	vis_alignaddr((void *)6, 0);                                      \
	d3 = vis_faligndata(d0, d1);                                      \
	vis_alignaddr((void *)2, 0);                                      \
	d4 = vis_faligndata(d1, d2);                                      \
	d0 = vis_fpadd16(d0, d3);                                         \
	d2 = vis_fpadd16(d2, d4);                                         \
	d1 = vis_faligndata(d2, d2);                                      \
	res = vis_fpadd16(d0, d1)

/* *********************************************************** */

#define	MAKE_3x4                                                          \
	v00 = vis_fmul8x16au(vis_read_hi(row00), vis_read_hi(yFilter));   \
	v01 = vis_fmul8x16au(vis_read_lo(row00), vis_read_hi(yFilter));   \
	v02 = vis_fmul8x16au(vis_read_hi(row01), vis_read_hi(yFilter));   \
	dpSrc = vis_alignaddr(sPtr, 0);                                   \
	data0 = dpSrc[0];                                                 \
	filterposy = (y >> yf_shift);                                     \
	v10 = vis_fmul8x16al(vis_read_hi(row10), vis_read_hi(yFilter));   \
	data1 = dpSrc[1];                                                 \
	v11 = vis_fmul8x16al(vis_read_lo(row10), vis_read_hi(yFilter));   \
	sum0 = vis_fpadd16(v00, v10);                                     \
	data2 = dpSrc[2];                                                 \
	row00 = vis_faligndata(data0, data1);                             \
	v12 = vis_fmul8x16al(vis_read_hi(row11), vis_read_hi(yFilter));   \
	row01 = vis_faligndata(data1, data2);                             \
	filterposx = (x >> xf_shift);                                     \
	sPtr += srcStride;                                                \
	dpSrc = vis_alignaddr(sPtr, 0);                                   \
	v20 = vis_fmul8x16au(vis_read_hi(row20), vis_read_lo(yFilter));   \
	sum1 = vis_fpadd16(v01, v11);                                     \
	data0 = dpSrc[0];                                                 \
	x += dx;                                                          \
	data1 = dpSrc[1];                                                 \
	v21 = vis_fmul8x16au(vis_read_lo(row20), vis_read_lo(yFilter));   \
	sum2 = vis_fpadd16(v02, v12);                                     \
	data2 = dpSrc[2];                                                 \
	row10 = vis_faligndata(data0, data1);                             \
	v22 = vis_fmul8x16au(vis_read_hi(row21), vis_read_lo(yFilter));   \
	row11 = vis_faligndata(data1, data2);                             \
	sPtr += srcStride;                                                \
	dpSrc = vis_alignaddr(sPtr, 0);                                   \
	y += dy;                                                          \
	xSrc = (x >> x_shift);                                            \
	sum0 = vis_fpadd16(sum0, v20);                                    \
	data0 = dpSrc[0];                                                 \
	ySrc = (y >> y_shift);                                            \
	data1 = dpSrc[1];                                                 \
	sum1 = vis_fpadd16(sum1, v21);                                    \
	data2 = dpSrc[2];                                                 \
	filterposy &= yf_mask;                                            \
	row20 = vis_faligndata(data0, data1);                             \
	row21 = vis_faligndata(data1, data2);                             \
	filterposx &= xf_mask;                                            \
	sum2 = vis_fpadd16(sum2, v22);                                    \
	v00 = vis_fmul8sux16(sum0, xFilter0);                             \
	v01 = vis_fmul8ulx16(sum0, xFilter0);                             \
	v10 = vis_fmul8sux16(sum1, xFilter1);                             \
	d0 = vis_fpadd16(v00, v01);                                       \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));       \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                             \
	xPtr = ((mlib_d64 *)((mlib_u8 *)filterX + 3 * filterposx));       \
	xFilter0 = xPtr[0];                                               \
	v20 = vis_fmul8sux16(sum2, xFilter2);                             \
	d1 = vis_fpadd16(v10, v11);                                       \
	xFilter1 = xPtr[1];                                               \
	v21 = vis_fmul8ulx16(sum2, xFilter2);                             \
	xFilter2 = xPtr[2];                                               \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + (3 * xSrc);                    \
	d2 = vis_fpadd16(v20, v21)

/* *********************************************************** */

void
mlib_v_ImageAffineTableLine_8nw_3_4_3(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws)
{
	DECLAREVAR;
	mlib_d64 row01, row11, row20, row21;
	mlib_d64 v02, v12, v22, v20, v21;
	mlib_d64 d3, d4;
	mlib_d64 data2;
	mlib_d64 xFilter2;
	mlib_d64 sum2;
	mlib_d64 yFilter;

	i = 0;

	if (i <= size - 4) {

		CALC_SRC_PTR;
		LOAD_1PIXEL_3x4;

		CALC_SRC_PTR;
		MAKE_3x4;
		FADD_4x4;
		MAKE_3x4;

#pragma pipeloop(0)
		for (; i < size - 4; i++) {
			*buff = res;
			FADD_4x4;
			MAKE_3x4;
			buff++;
		}

		*buff = res;
		buff++;
		FADD_4x4;
		*buff = res;
		buff++;

		RESULT_1PIXEL_3x4;
		*buff = res;
		buff++;

		LOAD_1PIXEL_3x4;
		RESULT_1PIXEL_3x4;
		*buff = res;
		buff++;
		i += 4;
	}
#pragma pipeloop(0)
	for (; i < size; i++) {
		CALC_SRC_PTR;
		LOAD_1PIXEL_3x4;
		RESULT_1PIXEL_3x4;
		*buff = res;
		buff++;
	}
}

/* *********************************************************** */

#define	LOAD_1PIXEL_2x4                                               \
	filterposy = (y >> yf_shift) & yf_mask;                       \
	yFilter = *((mlib_f32 *)((mlib_u8 *)filterY + filterposy));   \
	filterposx = (x >> xf_shift) & xf_mask;                       \
	xPtr = ((mlib_d64 *)((mlib_u8 *)filterX + 3 * filterposx));   \
	xFilter0 = xPtr[0];                                           \
	xFilter1 = xPtr[1];                                           \
	xFilter2 = xPtr[2];                                           \
	x += dx;                                                      \
	y += dy;                                                      \
	dpSrc = vis_alignaddr(sPtr, 0);                               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	data2 = dpSrc[2];                                             \
	row00 = vis_faligndata(data0, data1);                         \
	row01 = vis_faligndata(data1, data2);                         \
	sPtr += srcStride;                                            \
	dpSrc = vis_alignaddr(sPtr, 0);                               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	data2 = dpSrc[2];                                             \
	row10 = vis_faligndata(data0, data1);                         \
	row11 = vis_faligndata(data1, data2)

/* *********************************************************** */

#define	RESULT_1PIXEL_2x4                                       \
	v00 = vis_fmul8x16au(vis_read_hi(row00), yFilter);      \
	v01 = vis_fmul8x16au(vis_read_lo(row00), yFilter);      \
	v02 = vis_fmul8x16au(vis_read_hi(row01), yFilter);      \
	v10 = vis_fmul8x16al(vis_read_hi(row10), yFilter);      \
	v11 = vis_fmul8x16al(vis_read_lo(row10), yFilter);      \
	v12 = vis_fmul8x16al(vis_read_hi(row11), yFilter);      \
	sum0 = vis_fpadd16(v00, v10);                           \
	sum1 = vis_fpadd16(v01, v11);                           \
	sum2 = vis_fpadd16(v02, v12);                           \
	v00 = vis_fmul8sux16(sum0, xFilter0);                   \
	v01 = vis_fmul8ulx16(sum0, xFilter0);                   \
	v10 = vis_fmul8sux16(sum1, xFilter1);                   \
	d0 = vis_fpadd16(v00, v01);                             \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                   \
	v20 = vis_fmul8sux16(sum2, xFilter2);                   \
	d1 = vis_fpadd16(v10, v11);                             \
	v21 = vis_fmul8ulx16(sum2, xFilter2);                   \
	d2 = vis_fpadd16(v20, v21);                             \
	vis_alignaddr((void *)6, 0);                            \
	d3 = vis_faligndata(d0, d1);                            \
	vis_alignaddr((void *)2, 0);                            \
	d4 = vis_faligndata(d1, d2);                            \
	d0 = vis_fpadd16(d0, d3);                               \
	d2 = vis_fpadd16(d2, d4);                               \
	d1 = vis_faligndata(d2, d2);                            \
	res = vis_fpadd16(d0, d1)

/* *********************************************************** */

#define	MAKE_2x4                                                      \
	v00 = vis_fmul8x16au(vis_read_hi(row00), yFilter);            \
	v01 = vis_fmul8x16au(vis_read_lo(row00), yFilter);            \
	v02 = vis_fmul8x16au(vis_read_hi(row01), yFilter);            \
	dpSrc = vis_alignaddr(sPtr, 0);                               \
	data0 = dpSrc[0];                                             \
	filterposy = (y >> yf_shift);                                 \
	v10 = vis_fmul8x16al(vis_read_hi(row10), yFilter);            \
	data1 = dpSrc[1];                                             \
	v11 = vis_fmul8x16al(vis_read_lo(row10), yFilter);            \
	sum0 = vis_fpadd16(v00, v10);                                 \
	data2 = dpSrc[2];                                             \
	row00 = vis_faligndata(data0, data1);                         \
	v12 = vis_fmul8x16al(vis_read_hi(row11), yFilter);            \
	row01 = vis_faligndata(data1, data2);                         \
	filterposx = (x >> xf_shift);                                 \
	sPtr += srcStride;                                            \
	dpSrc = vis_alignaddr(sPtr, 0);                               \
	sum1 = vis_fpadd16(v01, v11);                                 \
	data0 = dpSrc[0];                                             \
	x += dx;                                                      \
	data1 = dpSrc[1];                                             \
	sum2 = vis_fpadd16(v02, v12);                                 \
	data2 = dpSrc[2];                                             \
	row10 = vis_faligndata(data0, data1);                         \
	row11 = vis_faligndata(data1, data2);                         \
	y += dy;                                                      \
	xSrc = (x >> x_shift);                                        \
	ySrc = (y >> y_shift);                                        \
	filterposy &= yf_mask;                                        \
	filterposx &= xf_mask;                                        \
	v00 = vis_fmul8sux16(sum0, xFilter0);                         \
	v01 = vis_fmul8ulx16(sum0, xFilter0);                         \
	v10 = vis_fmul8sux16(sum1, xFilter1);                         \
	d0 = vis_fpadd16(v00, v01);                                   \
	yFilter = *((mlib_f32 *)((mlib_u8 *)filterY + filterposy));   \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                         \
	xPtr = ((mlib_d64 *)((mlib_u8 *)filterX + 3 * filterposx));   \
	xFilter0 = xPtr[0];                                           \
	v20 = vis_fmul8sux16(sum2, xFilter2);                         \
	d1 = vis_fpadd16(v10, v11);                                   \
	xFilter1 = xPtr[1];                                           \
	v21 = vis_fmul8ulx16(sum2, xFilter2);                         \
	xFilter2 = xPtr[2];                                           \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + (3 * xSrc);                \
	d2 = vis_fpadd16(v20, v21)

/* *********************************************************** */

void
mlib_v_ImageAffineTableLine_8nw_2_4_3(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws)
{
	DECLAREVAR;
	mlib_d64 row01, row11;
	mlib_d64 v02, v12, v20, v21;
	mlib_d64 d3, d4;
	mlib_d64 data2;
	mlib_d64 xFilter2;
	mlib_d64 sum2;
	mlib_f32 yFilter;

	i = 0;

	if (i <= size - 4) {

		CALC_SRC_PTR;
		LOAD_1PIXEL_2x4;

		CALC_SRC_PTR;
		MAKE_2x4;
		FADD_4x4;
		MAKE_2x4;

#pragma pipeloop(0)
		for (; i < size - 4; i++) {
			*buff = res;
			FADD_4x4;
			MAKE_2x4;
			buff++;
		}

		*buff = res;
		buff++;
		FADD_4x4;
		*buff = res;
		buff++;

		RESULT_1PIXEL_2x4;
		*buff = res;
		buff++;

		LOAD_1PIXEL_2x4;
		RESULT_1PIXEL_2x4;
		*buff = res;
		buff++;
		i += 4;
	}
#pragma pipeloop(0)
	for (; i < size; i++) {
		CALC_SRC_PTR;
		LOAD_1PIXEL_2x4;
		RESULT_1PIXEL_2x4;
		*buff = res;
		buff++;
	}
}

/* *********************************************************** */

#define	LOAD_1PIXEL_4x2                                               \
	filterposy = (y >> yf_shift) & yf_mask;                       \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));   \
	filterposx = (x >> xf_shift) & xf_mask;                       \
	xPtr = ((mlib_d64 *)((mlib_u8 *)filterX + 3 * filterposx));   \
	xFilter0 = xPtr[0];                                           \
	xFilter1 = xPtr[1];                                           \
	x += dx;                                                      \
	y += dy;                                                      \
	dpSrc = vis_alignaddr(sPtr, 0);                               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row00 = vis_faligndata(data0, data1);                         \
	sPtr += srcStride;                                            \
	dpSrc = vis_alignaddr(sPtr, 0);                               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row10 = vis_faligndata(data0, data1);                         \
	sPtr += srcStride;                                            \
	dpSrc = vis_alignaddr(sPtr, 0);                               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row20 = vis_faligndata(data0, data1);                         \
	sPtr += srcStride;                                            \
	dpSrc = vis_alignaddr(sPtr, 0);                               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row30 = vis_faligndata(data0, data1)

/* *********************************************************** */

#define	RESULT_1PIXEL_4x2                                                 \
	v00 = vis_fmul8x16au(vis_read_hi(row00), vis_read_hi(yFilter));   \
	v01 = vis_fmul8x16au(vis_read_lo(row00), vis_read_hi(yFilter));   \
	v10 = vis_fmul8x16al(vis_read_hi(row10), vis_read_hi(yFilter));   \
	v11 = vis_fmul8x16al(vis_read_lo(row10), vis_read_hi(yFilter));   \
	v20 = vis_fmul8x16au(vis_read_hi(row20), vis_read_lo(yFilter));   \
	sum0 = vis_fpadd16(v00, v10);                                     \
	v21 = vis_fmul8x16au(vis_read_lo(row20), vis_read_lo(yFilter));   \
	sum1 = vis_fpadd16(v01, v11);                                     \
	v30 = vis_fmul8x16al(vis_read_hi(row30), vis_read_lo(yFilter));   \
	sum0 = vis_fpadd16(sum0, v20);                                    \
	v31 = vis_fmul8x16al(vis_read_lo(row30), vis_read_lo(yFilter));   \
	sum1 = vis_fpadd16(sum1, v21);                                    \
	sum0 = vis_fpadd16(sum0, v30);                                    \
	sum1 = vis_fpadd16(sum1, v31);                                    \
	v00 = vis_fmul8sux16(sum0, xFilter0);                             \
	v01 = vis_fmul8ulx16(sum0, xFilter0);                             \
	v10 = vis_fmul8sux16(sum1, xFilter1);                             \
	d0 = vis_fpadd16(v00, v01);                                       \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                             \
	d1 = vis_fpadd16(v10, v11);                                       \
	vis_alignaddr((void *)6, 0);                                      \
	d2 = vis_faligndata(d0, d1);                                      \
	res = vis_fpadd16(d0, d2)

/* *********************************************************** */

#define	MAKE_4x2                                                          \
	v00 = vis_fmul8x16au(vis_read_hi(row00), vis_read_hi(yFilter));   \
	v01 = vis_fmul8x16au(vis_read_lo(row00), vis_read_hi(yFilter));   \
	dpSrc = vis_alignaddr(sPtr, 0);                                   \
	data0 = dpSrc[0];                                                 \
	filterposy = (y >> yf_shift);                                     \
	v10 = vis_fmul8x16al(vis_read_hi(row10), vis_read_hi(yFilter));   \
	data1 = dpSrc[1];                                                 \
	v11 = vis_fmul8x16al(vis_read_lo(row10), vis_read_hi(yFilter));   \
	sum0 = vis_fpadd16(v00, v10);                                     \
	row00 = vis_faligndata(data0, data1);                             \
	filterposx = (x >> xf_shift);                                     \
	sPtr += srcStride;                                                \
	dpSrc = vis_alignaddr(sPtr, 0);                                   \
	v20 = vis_fmul8x16au(vis_read_hi(row20), vis_read_lo(yFilter));   \
	sum1 = vis_fpadd16(v01, v11);                                     \
	data0 = dpSrc[0];                                                 \
	x += dx;                                                          \
	data1 = dpSrc[1];                                                 \
	v21 = vis_fmul8x16au(vis_read_lo(row20), vis_read_lo(yFilter));   \
	row10 = vis_faligndata(data0, data1);                             \
	sPtr += srcStride;                                                \
	dpSrc = vis_alignaddr(sPtr, 0);                                   \
	y += dy;                                                          \
	xSrc = (x >> x_shift);                                            \
	v30 = vis_fmul8x16al(vis_read_hi(row30), vis_read_lo(yFilter));   \
	sum0 = vis_fpadd16(sum0, v20);                                    \
	data0 = dpSrc[0];                                                 \
	ySrc = (y >> y_shift);                                            \
	data1 = dpSrc[1];                                                 \
	v31 = vis_fmul8x16al(vis_read_lo(row30), vis_read_lo(yFilter));   \
	sum1 = vis_fpadd16(sum1, v21);                                    \
	filterposy &= yf_mask;                                            \
	row20 = vis_faligndata(data0, data1);                             \
	sPtr += srcStride;                                                \
	filterposx &= xf_mask;                                            \
	dpSrc = vis_alignaddr(sPtr, 0);                                   \
	sum0 = vis_fpadd16(sum0, v30);                                    \
	data0 = dpSrc[0];                                                 \
	sum1 = vis_fpadd16(sum1, v31);                                    \
	v00 = vis_fmul8sux16(sum0, xFilter0);                             \
	data1 = dpSrc[1];                                                 \
	v01 = vis_fmul8ulx16(sum0, xFilter0);                             \
	row30 = vis_faligndata(data0, data1);                             \
	v10 = vis_fmul8sux16(sum1, xFilter1);                             \
	d0 = vis_fpadd16(v00, v01);                                       \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));       \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                             \
	xPtr = ((mlib_d64 *)((mlib_u8 *)filterX + 3 * filterposx));       \
	xFilter0 = xPtr[0];                                               \
	d1 = vis_fpadd16(v10, v11);                                       \
	xFilter1 = xPtr[1];                                               \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + (3 * xSrc)

/* *********************************************************** */

#define	FADD_4x2                                                \
	vis_alignaddr((void *)6, 0);                            \
	d2 = vis_faligndata(d0, d1);                            \
	res = vis_fpadd16(d0, d2)

/* *********************************************************** */

void
mlib_v_ImageAffineTableLine_8nw_4_2_3(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws)
{
	DECLAREVAR;
	mlib_d64 row20, row30;
	mlib_d64 v20, v30, v21, v31;
	mlib_d64 yFilter;

	i = 0;

	if (i <= size - 4) {

		CALC_SRC_PTR;
		LOAD_1PIXEL_4x2;

		CALC_SRC_PTR;

		MAKE_4x2;
		FADD_4x2;
		MAKE_4x2;

#pragma pipeloop(0)
		for (; i < size - 4; i++) {
			*buff = res;
			buff++;
			FADD_4x2;
			MAKE_4x2;
		}

		*buff = res;
		buff++;
		FADD_4x2;
		*buff = res;
		buff++;

		RESULT_1PIXEL_4x2;
		*buff = res;
		buff++;

		LOAD_1PIXEL_4x2;
		RESULT_1PIXEL_4x2;
		*buff = res;
		buff++;
		i += 4;
	}
#pragma pipeloop(0)
	for (; i < size; i++) {
		CALC_SRC_PTR;
		LOAD_1PIXEL_4x2;
		RESULT_1PIXEL_4x2;
		*buff = res;
		buff++;
	}
}

/* *********************************************************** */

#define	LOAD_1PIXEL_3x2                                               \
	filterposy = (y >> yf_shift) & yf_mask;                       \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));   \
	filterposx = (x >> xf_shift) & xf_mask;                       \
	xPtr = ((mlib_d64 *)((mlib_u8 *)filterX + 3 * filterposx));   \
	xFilter0 = xPtr[0];                                           \
	xFilter1 = xPtr[1];                                           \
	x += dx;                                                      \
	y += dy;                                                      \
	dpSrc = vis_alignaddr(sPtr, 0);                               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row00 = vis_faligndata(data0, data1);                         \
	sPtr += srcStride;                                            \
	dpSrc = vis_alignaddr(sPtr, 0);                               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row10 = vis_faligndata(data0, data1);                         \
	sPtr += srcStride;                                            \
	dpSrc = vis_alignaddr(sPtr, 0);                               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row20 = vis_faligndata(data0, data1)

/* *********************************************************** */

#define	RESULT_1PIXEL_3x2                                                 \
	v00 = vis_fmul8x16au(vis_read_hi(row00), vis_read_hi(yFilter));   \
	v01 = vis_fmul8x16au(vis_read_lo(row00), vis_read_hi(yFilter));   \
	v10 = vis_fmul8x16al(vis_read_hi(row10), vis_read_hi(yFilter));   \
	v11 = vis_fmul8x16al(vis_read_lo(row10), vis_read_hi(yFilter));   \
	v20 = vis_fmul8x16au(vis_read_hi(row20), vis_read_lo(yFilter));   \
	sum0 = vis_fpadd16(v00, v10);                                     \
	v21 = vis_fmul8x16au(vis_read_lo(row20), vis_read_lo(yFilter));   \
	sum1 = vis_fpadd16(v01, v11);                                     \
	sum0 = vis_fpadd16(sum0, v20);                                    \
	sum1 = vis_fpadd16(sum1, v21);                                    \
	v00 = vis_fmul8sux16(sum0, xFilter0);                             \
	v01 = vis_fmul8ulx16(sum0, xFilter0);                             \
	v10 = vis_fmul8sux16(sum1, xFilter1);                             \
	d0 = vis_fpadd16(v00, v01);                                       \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                             \
	d1 = vis_fpadd16(v10, v11);                                       \
	vis_alignaddr((void *)6, 0);                                      \
	d2 = vis_faligndata(d0, d1);                                      \
	res = vis_fpadd16(d0, d2)

/* *********************************************************** */

#define	MAKE_3x2                                                          \
	v00 = vis_fmul8x16au(vis_read_hi(row00), vis_read_hi(yFilter));   \
	v01 = vis_fmul8x16au(vis_read_lo(row00), vis_read_hi(yFilter));   \
	dpSrc = vis_alignaddr(sPtr, 0);                                   \
	data0 = dpSrc[0];                                                 \
	filterposy = (y >> yf_shift);                                     \
	v10 = vis_fmul8x16al(vis_read_hi(row10), vis_read_hi(yFilter));   \
	data1 = dpSrc[1];                                                 \
	v11 = vis_fmul8x16al(vis_read_lo(row10), vis_read_hi(yFilter));   \
	sum0 = vis_fpadd16(v00, v10);                                     \
	row00 = vis_faligndata(data0, data1);                             \
	filterposx = (x >> xf_shift);                                     \
	sPtr += srcStride;                                                \
	dpSrc = vis_alignaddr(sPtr, 0);                                   \
	v20 = vis_fmul8x16au(vis_read_hi(row20), vis_read_lo(yFilter));   \
	sum1 = vis_fpadd16(v01, v11);                                     \
	data0 = dpSrc[0];                                                 \
	x += dx;                                                          \
	data1 = dpSrc[1];                                                 \
	v21 = vis_fmul8x16au(vis_read_lo(row20), vis_read_lo(yFilter));   \
	row10 = vis_faligndata(data0, data1);                             \
	sPtr += srcStride;                                                \
	dpSrc = vis_alignaddr(sPtr, 0);                                   \
	y += dy;                                                          \
	xSrc = (x >> x_shift);                                            \
	sum0 = vis_fpadd16(sum0, v20);                                    \
	data0 = dpSrc[0];                                                 \
	ySrc = (y >> y_shift);                                            \
	data1 = dpSrc[1];                                                 \
	sum1 = vis_fpadd16(sum1, v21);                                    \
	filterposy &= yf_mask;                                            \
	row20 = vis_faligndata(data0, data1);                             \
	filterposx &= xf_mask;                                            \
	v00 = vis_fmul8sux16(sum0, xFilter0);                             \
	v01 = vis_fmul8ulx16(sum0, xFilter0);                             \
	v10 = vis_fmul8sux16(sum1, xFilter1);                             \
	d0 = vis_fpadd16(v00, v01);                                       \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));       \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                             \
	xPtr = ((mlib_d64 *)((mlib_u8 *)filterX + 3 * filterposx));       \
	xFilter0 = xPtr[0];                                               \
	d1 = vis_fpadd16(v10, v11);                                       \
	xFilter1 = xPtr[1];                                               \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + (3 * xSrc)

/* *********************************************************** */

void
mlib_v_ImageAffineTableLine_8nw_3_2_3(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws)
{
	DECLAREVAR;
	mlib_d64 row20;
	mlib_d64 v20, v21;
	mlib_d64 yFilter;

	i = 0;

	if (i <= size - 4) {

		CALC_SRC_PTR;
		LOAD_1PIXEL_3x2;

		CALC_SRC_PTR;

		MAKE_3x2;
		FADD_4x2;
		MAKE_3x2;

#pragma pipeloop(0)
		for (; i < size - 4; i++) {
			*buff = res;
			buff++;
			FADD_4x2;
			MAKE_3x2;
		}

		*buff = res;
		buff++;
		FADD_4x2;
		*buff = res;
		buff++;

		RESULT_1PIXEL_3x2;
		*buff = res;
		buff++;

		LOAD_1PIXEL_3x2;
		RESULT_1PIXEL_3x2;
		*buff = res;
		buff++;
		i += 4;
	}
#pragma pipeloop(0)
	for (; i < size; i++) {
		CALC_SRC_PTR;
		LOAD_1PIXEL_3x2;
		RESULT_1PIXEL_3x2;
		*buff = res;
		buff++;
	}
}

/* *********************************************************** */

#define	LOAD_1PIXEL_2x2                                               \
	filterposy = (y >> yf_shift) & yf_mask;                       \
	yFilter = *((mlib_f32 *)((mlib_u8 *)filterY + filterposy));   \
	filterposx = (x >> xf_shift) & xf_mask;                       \
	xPtr = ((mlib_d64 *)((mlib_u8 *)filterX + 3 * filterposx));   \
	xFilter0 = xPtr[0];                                           \
	xFilter1 = xPtr[1];                                           \
	x += dx;                                                      \
	y += dy;                                                      \
	dpSrc = vis_alignaddr(sPtr, 0);                               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row00 = vis_faligndata(data0, data1);                         \
	sPtr += srcStride;                                            \
	dpSrc = vis_alignaddr(sPtr, 0);                               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	row10 = vis_faligndata(data0, data1)

/* *********************************************************** */

#define	RESULT_1PIXEL_2x2                                       \
	v00 = vis_fmul8x16au(vis_read_hi(row00), yFilter);      \
	v01 = vis_fmul8x16au(vis_read_lo(row00), yFilter);      \
	v10 = vis_fmul8x16al(vis_read_hi(row10), yFilter);      \
	v11 = vis_fmul8x16al(vis_read_lo(row10), yFilter);      \
	sum0 = vis_fpadd16(v00, v10);                           \
	sum1 = vis_fpadd16(v01, v11);                           \
	v00 = vis_fmul8sux16(sum0, xFilter0);                   \
	v01 = vis_fmul8ulx16(sum0, xFilter0);                   \
	v10 = vis_fmul8sux16(sum1, xFilter1);                   \
	d0 = vis_fpadd16(v00, v01);                             \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                   \
	d1 = vis_fpadd16(v10, v11);                             \
	vis_alignaddr((void *)6, 0);                            \
	d2 = vis_faligndata(d0, d1);                            \
	res = vis_fpadd16(d0, d2)

/* *********************************************************** */

#define	MAKE_2x2                                                      \
	v00 = vis_fmul8x16au(vis_read_hi(row00), yFilter);            \
	v01 = vis_fmul8x16au(vis_read_lo(row00), yFilter);            \
	dpSrc = vis_alignaddr(sPtr, 0);                               \
	data0 = dpSrc[0];                                             \
	filterposy = (y >> yf_shift);                                 \
	v10 = vis_fmul8x16al(vis_read_hi(row10), yFilter);            \
	data1 = dpSrc[1];                                             \
	v11 = vis_fmul8x16al(vis_read_lo(row10), yFilter);            \
	sum0 = vis_fpadd16(v00, v10);                                 \
	row00 = vis_faligndata(data0, data1);                         \
	filterposx = (x >> xf_shift);                                 \
	sPtr += srcStride;                                            \
	dpSrc = vis_alignaddr(sPtr, 0);                               \
	sum1 = vis_fpadd16(v01, v11);                                 \
	data0 = dpSrc[0];                                             \
	x += dx;                                                      \
	data1 = dpSrc[1];                                             \
	row10 = vis_faligndata(data0, data1);                         \
	y += dy;                                                      \
	xSrc = (x >> x_shift);                                        \
	ySrc = (y >> y_shift);                                        \
	filterposy &= yf_mask;                                        \
	filterposx &= xf_mask;                                        \
	v00 = vis_fmul8sux16(sum0, xFilter0);                         \
	v01 = vis_fmul8ulx16(sum0, xFilter0);                         \
	v10 = vis_fmul8sux16(sum1, xFilter1);                         \
	d0 = vis_fpadd16(v00, v01);                                   \
	yFilter = *((mlib_f32 *)((mlib_u8 *)filterY + filterposy));   \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                         \
	xPtr = ((mlib_d64 *)((mlib_u8 *)filterX + 3 * filterposx));   \
	xFilter0 = xPtr[0];                                           \
	d1 = vis_fpadd16(v10, v11);                                   \
	xFilter1 = xPtr[1];                                           \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + (3 * xSrc)

/* *********************************************************** */

void
mlib_v_ImageAffineTableLine_8nw_2_2_3(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws)
{
	DECLAREVAR;
	mlib_f32 yFilter;

	i = 0;

	if (i <= size - 4) {

		CALC_SRC_PTR;
		LOAD_1PIXEL_2x2;

		CALC_SRC_PTR;

		MAKE_2x2;
		FADD_4x2;
		MAKE_2x2;

#pragma pipeloop(0)
		for (; i < size - 4; i++) {
			*buff = res;
			buff++;
			FADD_4x2;
			MAKE_2x2;
		}

		*buff = res;
		buff++;
		FADD_4x2;
		*buff = res;
		buff++;

		RESULT_1PIXEL_2x2;
		*buff = res;
		buff++;

		LOAD_1PIXEL_2x2;
		RESULT_1PIXEL_2x2;
		*buff = res;
		buff++;
		i += 4;
	}
#pragma pipeloop(0)
	for (; i < size; i++) {
		CALC_SRC_PTR;
		LOAD_1PIXEL_2x2;
		RESULT_1PIXEL_2x2;
		*buff = res;
		buff++;
	}
}

/* *********************************************************** */
