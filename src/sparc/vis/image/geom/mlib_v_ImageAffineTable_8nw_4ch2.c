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

#pragma ident	"@(#)mlib_v_ImageAffineTable_8nw_4ch2.c	9.2	07/11/05 SMI"

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
	mlib_d64 sum0, sum1;                                    \
	mlib_d64 row00, row10;                                  \
	mlib_d64 v00, v10;                                      \
	mlib_d64 v01, v11;                                      \
	mlib_d64 d00, d10, d01, d11;                            \
	mlib_d64 data0, data1;                                  \
	mlib_d64 xFilter0, xFilter1;                            \
	mlib_d64 res0, res1

/* *********************************************************** */

#define	CALC_SRC_PTR                                            \
	xSrc = (x >> x_shift);                                  \
	ySrc = (y >> y_shift);                                  \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + (4 * xSrc)

/* *********************************************************** */

#define	LOAD_1PIXEL_4x2                                               \
	filterposy = (y >> yf_shift) & yf_mask;                       \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));   \
	filterposx = (x >> xf_shift) & xf_mask;                       \
	xPtr = ((mlib_d64 *)((mlib_u8 *)filterX + 4 * filterposx));   \
	x += dx;                                                      \
	y += dy;                                                      \
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
	row30 = vis_faligndata(data0, data1)

/* *********************************************************** */

#define	RESULT_1PIXEL_4x2(ind)                                            \
	xFilter0 = xPtr[0];                                               \
	xFilter1 = xPtr[1];                                               \
	v00 = vis_fmul8x16au(vis_read_hi(row00), vis_read_hi(yFilter));   \
	v01 = vis_fmul8x16au(vis_read_lo(row00), vis_read_hi(yFilter));   \
	v10 = vis_fmul8x16al(vis_read_hi(row10), vis_read_hi(yFilter));   \
	v11 = vis_fmul8x16al(vis_read_lo(row10), vis_read_hi(yFilter));   \
	sum0 = vis_fpadd16(v00, v10);                                     \
	sum1 = vis_fpadd16(v01, v11);                                     \
	v20 = vis_fmul8x16au(vis_read_hi(row20), vis_read_lo(yFilter));   \
	v21 = vis_fmul8x16au(vis_read_lo(row20), vis_read_lo(yFilter));   \
	sum0 = vis_fpadd16(sum0, v20);                                    \
	sum1 = vis_fpadd16(sum1, v21);                                    \
	v30 = vis_fmul8x16al(vis_read_hi(row30), vis_read_lo(yFilter));   \
	v31 = vis_fmul8x16al(vis_read_lo(row30), vis_read_lo(yFilter));   \
	sum0 = vis_fpadd16(sum0, v30);                                    \
	sum1 = vis_fpadd16(sum1, v31);                                    \
	v00 = vis_fmul8sux16(sum0, xFilter0);                             \
	v01 = vis_fmul8ulx16(sum0, xFilter0);                             \
	v10 = vis_fmul8sux16(sum1, xFilter1);                             \
	d0##ind = vis_fpadd16(v00, v01);                                  \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                             \
	d1##ind = vis_fpadd16(v10, v11)

/* *********************************************************** */

#define	MAKE_4x2(index)                                                   \
	v00 = vis_fmul8x16au(vis_read_hi(row00), vis_read_hi(yFilter));   \
	xFilter0 = xPtr[0];                                               \
	v01 = vis_fmul8x16au(vis_read_lo(row00), vis_read_hi(yFilter));   \
	xFilter1 = xPtr[1];                                               \
	vis_alignaddr(sPtr, 0);                                           \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                   \
	data0 = dpSrc[0];                                                 \
	filterposy = (y >> yf_shift);                                     \
	v10 = vis_fmul8x16al(vis_read_hi(row10), vis_read_hi(yFilter));   \
	data1 = dpSrc[1];                                                 \
	v11 = vis_fmul8x16al(vis_read_lo(row10), vis_read_hi(yFilter));   \
	sum0 = vis_fpadd16(v00, v10);                                     \
	row00 = vis_faligndata(data0, data1);                             \
	filterposx = (x >> xf_shift);                                     \
	sPtr += srcStride;                                                \
	vis_alignaddr(sPtr, 0);                                           \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                   \
	v20 = vis_fmul8x16au(vis_read_hi(row20), vis_read_lo(yFilter));   \
	sum1 = vis_fpadd16(v01, v11);                                     \
	data0 = dpSrc[0];                                                 \
	x += dx;                                                          \
	data1 = dpSrc[1];                                                 \
	v21 = vis_fmul8x16au(vis_read_lo(row20), vis_read_lo(yFilter));   \
	row10 = vis_faligndata(data0, data1);                             \
	sPtr += srcStride;                                                \
	vis_alignaddr(sPtr, 0);                                           \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                   \
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
	vis_alignaddr(sPtr, 0);                                           \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                   \
	sum0 = vis_fpadd16(sum0, v30);                                    \
	data0 = dpSrc[0];                                                 \
	sum1 = vis_fpadd16(sum1, v31);                                    \
	v00 = vis_fmul8sux16(sum0, xFilter0);                             \
	data1 = dpSrc[1];                                                 \
	v01 = vis_fmul8ulx16(sum0, xFilter0);                             \
	row30 = vis_faligndata(data0, data1);                             \
	v10 = vis_fmul8sux16(sum1, xFilter1);                             \
	d0##index = vis_fpadd16(v00, v01);                                \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));       \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                             \
	xPtr = ((mlib_d64 *)((mlib_u8 *)filterX + 4 * filterposx));       \
	d1##index = vis_fpadd16(v10, v11);                                \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + (4 * xSrc)

/* *********************************************************** */

#define	FADD_4x2                                                \
	res0 = vis_fpadd16(d00, d10);                           \
	res1 = vis_fpadd16(d01, d11)

/* *********************************************************** */

void
mlib_v_ImageAffineTableLine_8nw_4_2_4(
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

	if (i <= size - 6) {

		CALC_SRC_PTR;
		LOAD_1PIXEL_4x2;

		CALC_SRC_PTR;

		MAKE_4x2(0);
		MAKE_4x2(1);

		FADD_4x2;

		MAKE_4x2(0);
		MAKE_4x2(1);

#pragma pipeloop(0)
		for (; i <= size - 8; i += 2) {
			*buff = res0;
			buff++;
			*buff = res1;
			buff++;
			FADD_4x2;
			MAKE_4x2(0);
			MAKE_4x2(1);
		}

		*buff = res0;
		buff++;
		*buff = res1;
		buff++;
		FADD_4x2;
		*buff = res0;
		buff++;
		*buff = res1;
		buff++;

		RESULT_1PIXEL_4x2(0);
		LOAD_1PIXEL_4x2;
		RESULT_1PIXEL_4x2(1);
		FADD_4x2;
		*buff = res0;
		buff++;
		*buff = res1;
		buff++;

		i += 6;
	}
#pragma pipeloop(0)
	for (; i < size; i++) {
		CALC_SRC_PTR;
		LOAD_1PIXEL_4x2;
		RESULT_1PIXEL_4x2(0);
		res0 = vis_fpadd16(d00, d10);
		*buff = res0;
		buff++;
	}
}

/* *********************************************************** */

#define	LOAD_1PIXEL_3x2                                               \
	filterposy = (y >> yf_shift) & yf_mask;                       \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));   \
	filterposx = (x >> xf_shift) & xf_mask;                       \
	xPtr = ((mlib_d64 *)((mlib_u8 *)filterX + 4 * filterposx));   \
	x += dx;                                                      \
	y += dy;                                                      \
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
	row20 = vis_faligndata(data0, data1)

/* *********************************************************** */

#define	RESULT_1PIXEL_3x2(ind)                                            \
	xFilter0 = xPtr[0];                                               \
	xFilter1 = xPtr[1];                                               \
	v00 = vis_fmul8x16au(vis_read_hi(row00), vis_read_hi(yFilter));   \
	v01 = vis_fmul8x16au(vis_read_lo(row00), vis_read_hi(yFilter));   \
	v10 = vis_fmul8x16al(vis_read_hi(row10), vis_read_hi(yFilter));   \
	v11 = vis_fmul8x16al(vis_read_lo(row10), vis_read_hi(yFilter));   \
	sum0 = vis_fpadd16(v00, v10);                                     \
	sum1 = vis_fpadd16(v01, v11);                                     \
	v20 = vis_fmul8x16au(vis_read_hi(row20), vis_read_lo(yFilter));   \
	v21 = vis_fmul8x16au(vis_read_lo(row20), vis_read_lo(yFilter));   \
	sum0 = vis_fpadd16(sum0, v20);                                    \
	sum1 = vis_fpadd16(sum1, v21);                                    \
	v00 = vis_fmul8sux16(sum0, xFilter0);                             \
	v01 = vis_fmul8ulx16(sum0, xFilter0);                             \
	v10 = vis_fmul8sux16(sum1, xFilter1);                             \
	d0##ind = vis_fpadd16(v00, v01);                                  \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                             \
	d1##ind = vis_fpadd16(v10, v11)

/* *********************************************************** */

#define	MAKE_3x2(index)                                                   \
	v00 = vis_fmul8x16au(vis_read_hi(row00), vis_read_hi(yFilter));   \
	xFilter0 = xPtr[0];                                               \
	v01 = vis_fmul8x16au(vis_read_lo(row00), vis_read_hi(yFilter));   \
	xFilter1 = xPtr[1];                                               \
	vis_alignaddr(sPtr, 0);                                           \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                   \
	data0 = dpSrc[0];                                                 \
	filterposy = (y >> yf_shift);                                     \
	v10 = vis_fmul8x16al(vis_read_hi(row10), vis_read_hi(yFilter));   \
	data1 = dpSrc[1];                                                 \
	v11 = vis_fmul8x16al(vis_read_lo(row10), vis_read_hi(yFilter));   \
	sum0 = vis_fpadd16(v00, v10);                                     \
	row00 = vis_faligndata(data0, data1);                             \
	filterposx = (x >> xf_shift);                                     \
	sPtr += srcStride;                                                \
	vis_alignaddr(sPtr, 0);                                           \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                   \
	v20 = vis_fmul8x16au(vis_read_hi(row20), vis_read_lo(yFilter));   \
	sum1 = vis_fpadd16(v01, v11);                                     \
	data0 = dpSrc[0];                                                 \
	x += dx;                                                          \
	data1 = dpSrc[1];                                                 \
	v21 = vis_fmul8x16au(vis_read_lo(row20), vis_read_lo(yFilter));   \
	row10 = vis_faligndata(data0, data1);                             \
	sPtr += srcStride;                                                \
	vis_alignaddr(sPtr, 0);                                           \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                   \
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
	d0##index = vis_fpadd16(v00, v01);                                \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));       \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                             \
	xPtr = ((mlib_d64 *)((mlib_u8 *)filterX + 4 * filterposx));       \
	d1##index = vis_fpadd16(v10, v11);                                \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + (4 * xSrc)

/* *********************************************************** */

void
mlib_v_ImageAffineTableLine_8nw_3_2_4(
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

	if (i <= size - 6) {

		CALC_SRC_PTR;
		LOAD_1PIXEL_3x2;

		CALC_SRC_PTR;

		MAKE_3x2(0);
		MAKE_3x2(1);

		FADD_4x2;

		MAKE_3x2(0);
		MAKE_3x2(1);

#pragma pipeloop(0)
		for (; i <= size - 8; i += 2) {
			*buff = res0;
			buff++;
			*buff = res1;
			buff++;
			FADD_4x2;
			MAKE_3x2(0);
			MAKE_3x2(1);
		}

		*buff = res0;
		buff++;
		*buff = res1;
		buff++;
		FADD_4x2;
		*buff = res0;
		buff++;
		*buff = res1;
		buff++;

		RESULT_1PIXEL_3x2(0);
		LOAD_1PIXEL_3x2;
		RESULT_1PIXEL_3x2(1);
		FADD_4x2;
		*buff = res0;
		buff++;
		*buff = res1;
		buff++;

		i += 6;
	}
#pragma pipeloop(0)
	for (; i < size; i++) {
		CALC_SRC_PTR;
		LOAD_1PIXEL_3x2;
		RESULT_1PIXEL_3x2(0);
		res0 = vis_fpadd16(d00, d10);
		*buff = res0;
		buff++;
	}
}

/* *********************************************************** */

#define	LOAD_1PIXEL_2x2                                               \
	filterposy = (y >> yf_shift) & yf_mask;                       \
	yFilter = *((mlib_f32 *)((mlib_u8 *)filterY + filterposy));   \
	filterposx = (x >> xf_shift) & xf_mask;                       \
	xPtr = ((mlib_d64 *)((mlib_u8 *)filterX + 4 * filterposx));   \
	x += dx;                                                      \
	y += dy;                                                      \
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
	row10 = vis_faligndata(data0, data1)

/* *********************************************************** */

#define	RESULT_1PIXEL_2x2(ind)                                  \
	xFilter0 = xPtr[0];                                     \
	xFilter1 = xPtr[1];                                     \
	v00 = vis_fmul8x16au(vis_read_hi(row00), yFilter);      \
	v01 = vis_fmul8x16au(vis_read_lo(row00), yFilter);      \
	v10 = vis_fmul8x16al(vis_read_hi(row10), yFilter);      \
	v11 = vis_fmul8x16al(vis_read_lo(row10), yFilter);      \
	sum0 = vis_fpadd16(v00, v10);                           \
	sum1 = vis_fpadd16(v01, v11);                           \
	v00 = vis_fmul8sux16(sum0, xFilter0);                   \
	v01 = vis_fmul8ulx16(sum0, xFilter0);                   \
	v10 = vis_fmul8sux16(sum1, xFilter1);                   \
	d0##ind = vis_fpadd16(v00, v01);                        \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                   \
	d1##ind = vis_fpadd16(v10, v11)

/* *********************************************************** */

#define	MAKE_2x2(index)                                               \
	v00 = vis_fmul8x16au(vis_read_hi(row00), yFilter);            \
	xFilter0 = xPtr[0];                                           \
	v01 = vis_fmul8x16au(vis_read_lo(row00), yFilter);            \
	xFilter1 = xPtr[1];                                           \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	filterposy = (y >> yf_shift);                                 \
	v10 = vis_fmul8x16al(vis_read_hi(row10), yFilter);            \
	data1 = dpSrc[1];                                             \
	v11 = vis_fmul8x16al(vis_read_lo(row10), yFilter);            \
	sum0 = vis_fpadd16(v00, v10);                                 \
	row00 = vis_faligndata(data0, data1);                         \
	filterposx = (x >> xf_shift);                                 \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
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
	d0##index = vis_fpadd16(v00, v01);                            \
	yFilter = *((mlib_f32 *)((mlib_u8 *)filterY + filterposy));   \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                         \
	xPtr = ((mlib_d64 *)((mlib_u8 *)filterX + 4 * filterposx));   \
	d1##index = vis_fpadd16(v10, v11);                            \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + (4 * xSrc)

/* *********************************************************** */

void
mlib_v_ImageAffineTableLine_8nw_2_2_4(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws)
{
	DECLAREVAR;
	mlib_f32 yFilter;

	i = 0;

	if (i <= size - 6) {

		CALC_SRC_PTR;
		LOAD_1PIXEL_2x2;

		CALC_SRC_PTR;

		MAKE_2x2(0);
		MAKE_2x2(1);

		FADD_4x2;

		MAKE_2x2(0);
		MAKE_2x2(1);

#pragma pipeloop(0)
		for (; i <= size - 8; i += 2) {
			*buff = res0;
			buff++;
			*buff = res1;
			buff++;
			FADD_4x2;
			MAKE_2x2(0);
			MAKE_2x2(1);
		}

		*buff = res0;
		buff++;
		*buff = res1;
		buff++;
		FADD_4x2;
		*buff = res0;
		buff++;
		*buff = res1;
		buff++;

		RESULT_1PIXEL_2x2(0);
		LOAD_1PIXEL_2x2;
		RESULT_1PIXEL_2x2(1);
		FADD_4x2;
		*buff = res0;
		buff++;
		*buff = res1;
		buff++;

		i += 6;
	}
#pragma pipeloop(0)
	for (; i < size; i++) {
		CALC_SRC_PTR;
		LOAD_1PIXEL_2x2;
		RESULT_1PIXEL_2x2(0);
		res0 = vis_fpadd16(d00, d10);
		*buff = res0;
		buff++;
	}
}

/* *********************************************************** */
