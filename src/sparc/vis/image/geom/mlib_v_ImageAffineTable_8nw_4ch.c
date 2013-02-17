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

#pragma ident	"@(#)mlib_v_ImageAffineTable_8nw_4ch.c	9.2	07/11/05 SMI"

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
	mlib_d64 sum0, sum1, sum2;                              \
	mlib_d64 row00, row10;                                  \
	mlib_d64 row01, row11;                                  \
	mlib_d64 v00, v10, v20;                                 \
	mlib_d64 v01, v11, v21;                                 \
	mlib_d64 v02, v12;                                      \
	mlib_d64 d0, d2;                                        \
	mlib_d64 d00, d10, d20;                                 \
	mlib_d64 d01, d11, d21;                                 \
	mlib_d64 data0, data1, data2;                           \
	mlib_d64 xFilter0, xFilter1, xFilter2;                  \
	mlib_d64 res0, res1

/* *********************************************************** */

#define	CALC_SRC_PTR                                            \
	xSrc = (x >> x_shift);                                  \
	ySrc = (y >> y_shift);                                  \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + (4 * xSrc)

/* *********************************************************** */

#define	LOAD_1PIXEL_4x4                                               \
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
	data2 = dpSrc[2];                                             \
	row00 = vis_faligndata(data0, data1);                         \
	row01 = vis_faligndata(data1, data2);                         \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	data2 = dpSrc[2];                                             \
	row10 = vis_faligndata(data0, data1);                         \
	row11 = vis_faligndata(data1, data2);                         \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	data2 = dpSrc[2];                                             \
	row20 = vis_faligndata(data0, data1);                         \
	row21 = vis_faligndata(data1, data2);                         \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	data2 = dpSrc[2];                                             \
	row30 = vis_faligndata(data0, data1);                         \
	row31 = vis_faligndata(data1, data2)

/* *********************************************************** */

#define	RESULT_1PIXEL_4x4(ind)                                            \
	xFilter0 = xPtr[0];                                               \
	xFilter1 = xPtr[1];                                               \
	xFilter2 = xPtr[2];                                               \
	xFilter3 = xPtr[3];                                               \
	v00 = vis_fmul8x16au(vis_read_hi(row00), vis_read_hi(yFilter));   \
	v01 = vis_fmul8x16au(vis_read_lo(row00), vis_read_hi(yFilter));   \
	v02 = vis_fmul8x16au(vis_read_hi(row01), vis_read_hi(yFilter));   \
	v03 = vis_fmul8x16au(vis_read_lo(row01), vis_read_hi(yFilter));   \
	v10 = vis_fmul8x16al(vis_read_hi(row10), vis_read_hi(yFilter));   \
	v11 = vis_fmul8x16al(vis_read_lo(row10), vis_read_hi(yFilter));   \
	sum0 = vis_fpadd16(v00, v10);                                     \
	v12 = vis_fmul8x16al(vis_read_hi(row11), vis_read_hi(yFilter));   \
	sum1 = vis_fpadd16(v01, v11);                                     \
	v13 = vis_fmul8x16al(vis_read_lo(row11), vis_read_hi(yFilter));   \
	sum2 = vis_fpadd16(v02, v12);                                     \
	v20 = vis_fmul8x16au(vis_read_hi(row20), vis_read_lo(yFilter));   \
	sum3 = vis_fpadd16(v03, v13);                                     \
	v21 = vis_fmul8x16au(vis_read_lo(row20), vis_read_lo(yFilter));   \
	sum0 = vis_fpadd16(sum0, v20);                                    \
	v22 = vis_fmul8x16au(vis_read_hi(row21), vis_read_lo(yFilter));   \
	sum1 = vis_fpadd16(sum1, v21);                                    \
	v23 = vis_fmul8x16au(vis_read_lo(row21), vis_read_lo(yFilter));   \
	sum2 = vis_fpadd16(sum2, v22);                                    \
	v30 = vis_fmul8x16al(vis_read_hi(row30), vis_read_lo(yFilter));   \
	sum3 = vis_fpadd16(sum3, v23);                                    \
	v31 = vis_fmul8x16al(vis_read_lo(row30), vis_read_lo(yFilter));   \
	sum0 = vis_fpadd16(sum0, v30);                                    \
	v32 = vis_fmul8x16al(vis_read_hi(row31), vis_read_lo(yFilter));   \
	sum1 = vis_fpadd16(sum1, v31);                                    \
	v33 = vis_fmul8x16al(vis_read_lo(row31), vis_read_lo(yFilter));   \
	sum2 = vis_fpadd16(sum2, v32);                                    \
	v00 = vis_fmul8sux16(sum0, xFilter0);                             \
	sum3 = vis_fpadd16(sum3, v33);                                    \
	v01 = vis_fmul8ulx16(sum0, xFilter0);                             \
	v10 = vis_fmul8sux16(sum1, xFilter1);                             \
	d0##ind = vis_fpadd16(v00, v01);                                  \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                             \
	v20 = vis_fmul8sux16(sum2, xFilter2);                             \
	d1##ind = vis_fpadd16(v10, v11);                                  \
	v21 = vis_fmul8ulx16(sum2, xFilter2);                             \
	v30 = vis_fmul8sux16(sum3, xFilter3);                             \
	d2##ind = vis_fpadd16(v20, v21);                                  \
	v31 = vis_fmul8ulx16(sum3, xFilter3);                             \
	d3##ind = vis_fpadd16(v30, v31)

/* *********************************************************** */

#define	MAKE_4x4(index)                                                   \
	v00 = vis_fmul8x16au(vis_read_hi(row00), vis_read_hi(yFilter));   \
	xFilter0 = xPtr[0];                                               \
	v01 = vis_fmul8x16au(vis_read_lo(row00), vis_read_hi(yFilter));   \
	xFilter1 = xPtr[1];                                               \
	v02 = vis_fmul8x16au(vis_read_hi(row01), vis_read_hi(yFilter));   \
	xFilter2 = xPtr[2];                                               \
	v03 = vis_fmul8x16au(vis_read_lo(row01), vis_read_hi(yFilter));   \
	xFilter3 = xPtr[3];                                               \
	vis_alignaddr(sPtr, 0);                                           \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                   \
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
	v13 = vis_fmul8x16al(vis_read_lo(row11), vis_read_hi(yFilter));   \
	sPtr += srcStride;                                                \
	vis_alignaddr(sPtr, 0);                                           \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                   \
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
	vis_alignaddr(sPtr, 0);                                           \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                   \
	v23 = vis_fmul8x16au(vis_read_lo(row21), vis_read_lo(yFilter));   \
	sum3 = vis_fpadd16(v03, v13);                                     \
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
	v33 = vis_fmul8x16al(vis_read_lo(row31), vis_read_lo(yFilter));   \
	sum2 = vis_fpadd16(sum2, v22);                                    \
	vis_alignaddr(sPtr, 0);                                           \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                   \
	sum3 = vis_fpadd16(sum3, v23);                                    \
	sum0 = vis_fpadd16(sum0, v30);                                    \
	data0 = dpSrc[0];                                                 \
	sum1 = vis_fpadd16(sum1, v31);                                    \
	v00 = vis_fmul8sux16(sum0, xFilter0);                             \
	data1 = dpSrc[1];                                                 \
	sum2 = vis_fpadd16(sum2, v32);                                    \
	v01 = vis_fmul8ulx16(sum0, xFilter0);                             \
	sum3 = vis_fpadd16(sum3, v33);                                    \
	data2 = dpSrc[2];                                                 \
	row30 = vis_faligndata(data0, data1);                             \
	v10 = vis_fmul8sux16(sum1, xFilter1);                             \
	d0##index = vis_fpadd16(v00, v01);                                \
	row31 = vis_faligndata(data1, data2);                             \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));       \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                             \
	xPtr = ((mlib_d64 *)((mlib_u8 *)filterX + 4 * filterposx));       \
	v20 = vis_fmul8sux16(sum2, xFilter2);                             \
	d1##index = vis_fpadd16(v10, v11);                                \
	v21 = vis_fmul8ulx16(sum2, xFilter2);                             \
	v30 = vis_fmul8sux16(sum3, xFilter3);                             \
	d2##index = vis_fpadd16(v20, v21);                                \
	v31 = vis_fmul8ulx16(sum3, xFilter3);                             \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + (4 * xSrc);                    \
	d3##index = vis_fpadd16(v30, v31)

/* *********************************************************** */

#define	FADD_4x4                                                \
	d0 = vis_fpadd16(d00, d10);                             \
	d1 = vis_fpadd16(d20, d30);                             \
	res0 = vis_fpadd16(d0, d1);                             \
	d2 = vis_fpadd16(d01, d11);                             \
	d3 = vis_fpadd16(d21, d31);                             \
	res1 = vis_fpadd16(d2, d3)

/* *********************************************************** */

void
mlib_v_ImageAffineTableLine_8nw_4_4_4(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws)
{
	DECLAREVAR;
	mlib_d64 sum3;
	mlib_d64 row20, row30;
	mlib_d64 row21, row31;
	mlib_d64 v30, v31, v22, v32;
	mlib_d64 v03, v13, v23, v33;
	mlib_d64 d1, d3;
	mlib_d64 d30, d31;
	mlib_d64 xFilter3;
	mlib_d64 yFilter;

	i = 0;

	if (i <= size - 6) {

		CALC_SRC_PTR;
		LOAD_1PIXEL_4x4;

		CALC_SRC_PTR;

		MAKE_4x4(0);
		MAKE_4x4(1);

		FADD_4x4;

		MAKE_4x4(0);
		MAKE_4x4(1);

#pragma pipeloop(0)
		for (; i <= size - 8; i += 2) {
			*buff = res0;
			buff++;
			*buff = res1;
			buff++;
			FADD_4x4;
			MAKE_4x4(0);
			MAKE_4x4(1);
		}

		*buff = res0;
		buff++;
		*buff = res1;
		buff++;
		FADD_4x4;
		*buff = res0;
		buff++;
		*buff = res1;
		buff++;

		RESULT_1PIXEL_4x4(0);
		LOAD_1PIXEL_4x4;
		RESULT_1PIXEL_4x4(1);
		FADD_4x4;
		*buff = res0;
		buff++;
		*buff = res1;
		buff++;

		i += 6;
	}
#pragma pipeloop(0)
	for (; i < size; i++) {
		CALC_SRC_PTR;
		LOAD_1PIXEL_4x4;
		RESULT_1PIXEL_4x4(0);
		d0 = vis_fpadd16(d00, d10);
		d1 = vis_fpadd16(d20, d30);
		res0 = vis_fpadd16(d0, d1);
		*buff = res0;
		buff++;
	}
}

/* *********************************************************** */

#define	LOAD_1PIXEL_3x4                                               \
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
	data2 = dpSrc[2];                                             \
	row00 = vis_faligndata(data0, data1);                         \
	row01 = vis_faligndata(data1, data2);                         \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	data2 = dpSrc[2];                                             \
	row10 = vis_faligndata(data0, data1);                         \
	row11 = vis_faligndata(data1, data2);                         \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	data2 = dpSrc[2];                                             \
	row20 = vis_faligndata(data0, data1);                         \
	row21 = vis_faligndata(data1, data2)

/* *********************************************************** */

#define	RESULT_1PIXEL_3x4(ind)                                            \
	xFilter0 = xPtr[0];                                               \
	xFilter1 = xPtr[1];                                               \
	xFilter2 = xPtr[2];                                               \
	xFilter3 = xPtr[3];                                               \
	v00 = vis_fmul8x16au(vis_read_hi(row00), vis_read_hi(yFilter));   \
	v01 = vis_fmul8x16au(vis_read_lo(row00), vis_read_hi(yFilter));   \
	v02 = vis_fmul8x16au(vis_read_hi(row01), vis_read_hi(yFilter));   \
	v03 = vis_fmul8x16au(vis_read_lo(row01), vis_read_hi(yFilter));   \
	v10 = vis_fmul8x16al(vis_read_hi(row10), vis_read_hi(yFilter));   \
	v11 = vis_fmul8x16al(vis_read_lo(row10), vis_read_hi(yFilter));   \
	sum0 = vis_fpadd16(v00, v10);                                     \
	v12 = vis_fmul8x16al(vis_read_hi(row11), vis_read_hi(yFilter));   \
	sum1 = vis_fpadd16(v01, v11);                                     \
	v13 = vis_fmul8x16al(vis_read_lo(row11), vis_read_hi(yFilter));   \
	sum2 = vis_fpadd16(v02, v12);                                     \
	v20 = vis_fmul8x16au(vis_read_hi(row20), vis_read_lo(yFilter));   \
	sum3 = vis_fpadd16(v03, v13);                                     \
	v21 = vis_fmul8x16au(vis_read_lo(row20), vis_read_lo(yFilter));   \
	sum0 = vis_fpadd16(sum0, v20);                                    \
	v22 = vis_fmul8x16au(vis_read_hi(row21), vis_read_lo(yFilter));   \
	sum1 = vis_fpadd16(sum1, v21);                                    \
	v23 = vis_fmul8x16au(vis_read_lo(row21), vis_read_lo(yFilter));   \
	sum2 = vis_fpadd16(sum2, v22);                                    \
	sum3 = vis_fpadd16(sum3, v23);                                    \
	v00 = vis_fmul8sux16(sum0, xFilter0);                             \
	v01 = vis_fmul8ulx16(sum0, xFilter0);                             \
	v10 = vis_fmul8sux16(sum1, xFilter1);                             \
	d0##ind = vis_fpadd16(v00, v01);                                  \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                             \
	v20 = vis_fmul8sux16(sum2, xFilter2);                             \
	d1##ind = vis_fpadd16(v10, v11);                                  \
	v21 = vis_fmul8ulx16(sum2, xFilter2);                             \
	v30 = vis_fmul8sux16(sum3, xFilter3);                             \
	d2##ind = vis_fpadd16(v20, v21);                                  \
	v31 = vis_fmul8ulx16(sum3, xFilter3);                             \
	d3##ind = vis_fpadd16(v30, v31)

/* *********************************************************** */

#define	MAKE_3x4(index)                                                   \
	v00 = vis_fmul8x16au(vis_read_hi(row00), vis_read_hi(yFilter));   \
	xFilter0 = xPtr[0];                                               \
	v01 = vis_fmul8x16au(vis_read_lo(row00), vis_read_hi(yFilter));   \
	xFilter1 = xPtr[1];                                               \
	v02 = vis_fmul8x16au(vis_read_hi(row01), vis_read_hi(yFilter));   \
	xFilter2 = xPtr[2];                                               \
	v03 = vis_fmul8x16au(vis_read_lo(row01), vis_read_hi(yFilter));   \
	xFilter3 = xPtr[3];                                               \
	vis_alignaddr(sPtr, 0);                                           \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                   \
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
	v13 = vis_fmul8x16al(vis_read_lo(row11), vis_read_hi(yFilter));   \
	sPtr += srcStride;                                                \
	vis_alignaddr(sPtr, 0);                                           \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                   \
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
	vis_alignaddr(sPtr, 0);                                           \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                   \
	v23 = vis_fmul8x16au(vis_read_lo(row21), vis_read_lo(yFilter));   \
	sum3 = vis_fpadd16(v03, v13);                                     \
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
	sum3 = vis_fpadd16(sum3, v23);                                    \
	v00 = vis_fmul8sux16(sum0, xFilter0);                             \
	v01 = vis_fmul8ulx16(sum0, xFilter0);                             \
	v10 = vis_fmul8sux16(sum1, xFilter1);                             \
	d0##index = vis_fpadd16(v00, v01);                                \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));       \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                             \
	xPtr = ((mlib_d64 *)((mlib_u8 *)filterX + 4 * filterposx));       \
	v20 = vis_fmul8sux16(sum2, xFilter2);                             \
	d1##index = vis_fpadd16(v10, v11);                                \
	v21 = vis_fmul8ulx16(sum2, xFilter2);                             \
	v30 = vis_fmul8sux16(sum3, xFilter3);                             \
	d2##index = vis_fpadd16(v20, v21);                                \
	v31 = vis_fmul8ulx16(sum3, xFilter3);                             \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + (4 * xSrc);                    \
	d3##index = vis_fpadd16(v30, v31)

/* *********************************************************** */

void
mlib_v_ImageAffineTableLine_8nw_3_4_4(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws)
{
	DECLAREVAR;
	mlib_d64 sum3;
	mlib_d64 row20, row21;
	mlib_d64 v30, v31, v22;
	mlib_d64 v03, v13, v23;
	mlib_d64 d1, d3;
	mlib_d64 d30, d31;
	mlib_d64 xFilter3;
	mlib_d64 yFilter;

	i = 0;

	if (i <= size - 6) {

		CALC_SRC_PTR;
		LOAD_1PIXEL_3x4;

		CALC_SRC_PTR;

		MAKE_3x4(0);
		MAKE_3x4(1);

		FADD_4x4;

		MAKE_3x4(0);
		MAKE_3x4(1);

#pragma pipeloop(0)
		for (; i <= size - 8; i += 2) {
			*buff = res0;
			buff++;
			*buff = res1;
			buff++;
			FADD_4x4;
			MAKE_3x4(0);
			MAKE_3x4(1);
		}

		*buff = res0;
		buff++;
		*buff = res1;
		buff++;
		FADD_4x4;
		*buff = res0;
		buff++;
		*buff = res1;
		buff++;

		RESULT_1PIXEL_3x4(0);
		LOAD_1PIXEL_3x4;
		RESULT_1PIXEL_3x4(1);
		FADD_4x4;
		*buff = res0;
		buff++;
		*buff = res1;
		buff++;

		i += 6;
	}
#pragma pipeloop(0)
	for (; i < size; i++) {
		CALC_SRC_PTR;
		LOAD_1PIXEL_3x4;
		RESULT_1PIXEL_3x4(0);
		d0 = vis_fpadd16(d00, d10);
		d1 = vis_fpadd16(d20, d30);
		res0 = vis_fpadd16(d0, d1);
		*buff = res0;
		buff++;
	}
}

/* *********************************************************** */

#define	LOAD_1PIXEL_2x4                                               \
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
	data2 = dpSrc[2];                                             \
	row00 = vis_faligndata(data0, data1);                         \
	row01 = vis_faligndata(data1, data2);                         \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	data0 = dpSrc[0];                                             \
	data1 = dpSrc[1];                                             \
	data2 = dpSrc[2];                                             \
	row10 = vis_faligndata(data0, data1);                         \
	row11 = vis_faligndata(data1, data2)

/* *********************************************************** */

#define	RESULT_1PIXEL_2x4(ind)                                  \
	xFilter0 = xPtr[0];                                     \
	xFilter1 = xPtr[1];                                     \
	xFilter2 = xPtr[2];                                     \
	xFilter3 = xPtr[3];                                     \
	v00 = vis_fmul8x16au(vis_read_hi(row00), yFilter);      \
	v01 = vis_fmul8x16au(vis_read_lo(row00), yFilter);      \
	v02 = vis_fmul8x16au(vis_read_hi(row01), yFilter);      \
	v03 = vis_fmul8x16au(vis_read_lo(row01), yFilter);      \
	v10 = vis_fmul8x16al(vis_read_hi(row10), yFilter);      \
	v11 = vis_fmul8x16al(vis_read_lo(row10), yFilter);      \
	sum0 = vis_fpadd16(v00, v10);                           \
	v12 = vis_fmul8x16al(vis_read_hi(row11), yFilter);      \
	sum1 = vis_fpadd16(v01, v11);                           \
	v13 = vis_fmul8x16al(vis_read_lo(row11), yFilter);      \
	sum2 = vis_fpadd16(v02, v12);                           \
	sum3 = vis_fpadd16(v03, v13);                           \
	v00 = vis_fmul8sux16(sum0, xFilter0);                   \
	v01 = vis_fmul8ulx16(sum0, xFilter0);                   \
	v10 = vis_fmul8sux16(sum1, xFilter1);                   \
	d0##ind = vis_fpadd16(v00, v01);                        \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                   \
	v20 = vis_fmul8sux16(sum2, xFilter2);                   \
	d1##ind = vis_fpadd16(v10, v11);                        \
	v21 = vis_fmul8ulx16(sum2, xFilter2);                   \
	v30 = vis_fmul8sux16(sum3, xFilter3);                   \
	d2##ind = vis_fpadd16(v20, v21);                        \
	v31 = vis_fmul8ulx16(sum3, xFilter3);                   \
	d3##ind = vis_fpadd16(v30, v31)

/* *********************************************************** */

#define	MAKE_2x4(index)                                               \
	v00 = vis_fmul8x16au(vis_read_hi(row00), yFilter);            \
	xFilter0 = xPtr[0];                                           \
	v01 = vis_fmul8x16au(vis_read_lo(row00), yFilter);            \
	xFilter1 = xPtr[1];                                           \
	v02 = vis_fmul8x16au(vis_read_hi(row01), yFilter);            \
	xFilter2 = xPtr[2];                                           \
	v03 = vis_fmul8x16au(vis_read_lo(row01), yFilter);            \
	xFilter3 = xPtr[3];                                           \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
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
	v13 = vis_fmul8x16al(vis_read_lo(row11), yFilter);            \
	sPtr += srcStride;                                            \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
	sum1 = vis_fpadd16(v01, v11);                                 \
	data0 = dpSrc[0];                                             \
	x += dx;                                                      \
	data1 = dpSrc[1];                                             \
	sum2 = vis_fpadd16(v02, v12);                                 \
	data2 = dpSrc[2];                                             \
	row10 = vis_faligndata(data0, data1);                         \
	row11 = vis_faligndata(data1, data2);                         \
	sum3 = vis_fpadd16(v03, v13);                                 \
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
	v20 = vis_fmul8sux16(sum2, xFilter2);                         \
	d1##index = vis_fpadd16(v10, v11);                            \
	v21 = vis_fmul8ulx16(sum2, xFilter2);                         \
	v30 = vis_fmul8sux16(sum3, xFilter3);                         \
	d2##index = vis_fpadd16(v20, v21);                            \
	v31 = vis_fmul8ulx16(sum3, xFilter3);                         \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + (4 * xSrc);                \
	d3##index = vis_fpadd16(v30, v31)

/* *********************************************************** */

void
mlib_v_ImageAffineTableLine_8nw_2_4_4(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws)
{
	DECLAREVAR;
	mlib_d64 sum3;
	mlib_d64 v30, v31, v03, v13;
	mlib_d64 d1, d3;
	mlib_d64 d30, d31;
	mlib_d64 xFilter3;
	mlib_f32 yFilter;

	i = 0;

	if (i <= size - 6) {

		CALC_SRC_PTR;
		LOAD_1PIXEL_2x4;

		CALC_SRC_PTR;

		MAKE_2x4(0);
		MAKE_2x4(1);

		FADD_4x4;

		MAKE_2x4(0);
		MAKE_2x4(1);

#pragma pipeloop(0)
		for (; i <= size - 8; i += 2) {
			*buff = res0;
			buff++;
			*buff = res1;
			buff++;
			FADD_4x4;
			MAKE_2x4(0);
			MAKE_2x4(1);
		}

		*buff = res0;
		buff++;
		*buff = res1;
		buff++;
		FADD_4x4;
		*buff = res0;
		buff++;
		*buff = res1;
		buff++;

		RESULT_1PIXEL_2x4(0);
		LOAD_1PIXEL_2x4;
		RESULT_1PIXEL_2x4(1);
		FADD_4x4;
		*buff = res0;
		buff++;
		*buff = res1;
		buff++;

		i += 6;
	}
#pragma pipeloop(0)
	for (; i < size; i++) {
		CALC_SRC_PTR;
		LOAD_1PIXEL_2x4;
		RESULT_1PIXEL_2x4(0);
		d0 = vis_fpadd16(d00, d10);
		d1 = vis_fpadd16(d20, d30);
		res0 = vis_fpadd16(d0, d1);
		*buff = res0;
		buff++;
	}
}

/* *********************************************************** */

#define	RESULT_1PIXEL_4x3(ind)                                            \
	xFilter0 = xPtr[0];                                               \
	xFilter1 = xPtr[1];                                               \
	xFilter2 = xPtr[2];                                               \
	v00 = vis_fmul8x16au(vis_read_hi(row00), vis_read_hi(yFilter));   \
	v01 = vis_fmul8x16au(vis_read_lo(row00), vis_read_hi(yFilter));   \
	v02 = vis_fmul8x16au(vis_read_hi(row01), vis_read_hi(yFilter));   \
	v10 = vis_fmul8x16al(vis_read_hi(row10), vis_read_hi(yFilter));   \
	v11 = vis_fmul8x16al(vis_read_lo(row10), vis_read_hi(yFilter));   \
	sum0 = vis_fpadd16(v00, v10);                                     \
	v12 = vis_fmul8x16al(vis_read_hi(row11), vis_read_hi(yFilter));   \
	sum1 = vis_fpadd16(v01, v11);                                     \
	sum2 = vis_fpadd16(v02, v12);                                     \
	v20 = vis_fmul8x16au(vis_read_hi(row20), vis_read_lo(yFilter));   \
	v21 = vis_fmul8x16au(vis_read_lo(row20), vis_read_lo(yFilter));   \
	sum0 = vis_fpadd16(sum0, v20);                                    \
	v22 = vis_fmul8x16au(vis_read_hi(row21), vis_read_lo(yFilter));   \
	sum1 = vis_fpadd16(sum1, v21);                                    \
	sum2 = vis_fpadd16(sum2, v22);                                    \
	v30 = vis_fmul8x16al(vis_read_hi(row30), vis_read_lo(yFilter));   \
	v31 = vis_fmul8x16al(vis_read_lo(row30), vis_read_lo(yFilter));   \
	sum0 = vis_fpadd16(sum0, v30);                                    \
	v32 = vis_fmul8x16al(vis_read_hi(row31), vis_read_lo(yFilter));   \
	sum1 = vis_fpadd16(sum1, v31);                                    \
	sum2 = vis_fpadd16(sum2, v32);                                    \
	v00 = vis_fmul8sux16(sum0, xFilter0);                             \
	v01 = vis_fmul8ulx16(sum0, xFilter0);                             \
	v10 = vis_fmul8sux16(sum1, xFilter1);                             \
	d0##ind = vis_fpadd16(v00, v01);                                  \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                             \
	v20 = vis_fmul8sux16(sum2, xFilter2);                             \
	d1##ind = vis_fpadd16(v10, v11);                                  \
	v21 = vis_fmul8ulx16(sum2, xFilter2);                             \
	d2##ind = vis_fpadd16(v20, v21)

/* *********************************************************** */

#define	MAKE_4x3(index)                                                   \
	v00 = vis_fmul8x16au(vis_read_hi(row00), vis_read_hi(yFilter));   \
	xFilter0 = xPtr[0];                                               \
	v01 = vis_fmul8x16au(vis_read_lo(row00), vis_read_hi(yFilter));   \
	xFilter1 = xPtr[1];                                               \
	v02 = vis_fmul8x16au(vis_read_hi(row01), vis_read_hi(yFilter));   \
	xFilter2 = xPtr[2];                                               \
	vis_alignaddr(sPtr, 0);                                           \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                   \
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
	vis_alignaddr(sPtr, 0);                                           \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                   \
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
	data2 = dpSrc[2];                                                 \
	filterposy &= yf_mask;                                            \
	row20 = vis_faligndata(data0, data1);                             \
	v32 = vis_fmul8x16al(vis_read_hi(row31), vis_read_lo(yFilter));   \
	row21 = vis_faligndata(data1, data2);                             \
	sPtr += srcStride;                                                \
	filterposx &= xf_mask;                                            \
	sum2 = vis_fpadd16(sum2, v22);                                    \
	vis_alignaddr(sPtr, 0);                                           \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                   \
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
	d0##index = vis_fpadd16(v00, v01);                                \
	row31 = vis_faligndata(data1, data2);                             \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));       \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                             \
	xPtr = ((mlib_d64 *)((mlib_u8 *)filterX + 4 * filterposx));       \
	v20 = vis_fmul8sux16(sum2, xFilter2);                             \
	d1##index = vis_fpadd16(v10, v11);                                \
	v21 = vis_fmul8ulx16(sum2, xFilter2);                             \
	d2##index = vis_fpadd16(v20, v21);                                \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + (4 * xSrc)

/* *********************************************************** */

#define	FADD_4x3                                                \
	d0 = vis_fpadd16(d00, d10);                             \
	res0 = vis_fpadd16(d0, d20);                            \
	d2 = vis_fpadd16(d01, d11);                             \
	res1 = vis_fpadd16(d2, d21)

/* *********************************************************** */

void
mlib_v_ImageAffineTableLine_8nw_4_3_4(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws)
{
	DECLAREVAR;
	mlib_d64 row20, row30;
	mlib_d64 row21, row31;
	mlib_d64 v30, v31, v22, v32;
	mlib_d64 yFilter;

	i = 0;

	if (i <= size - 6) {

		CALC_SRC_PTR;
		LOAD_1PIXEL_4x4;

		CALC_SRC_PTR;

		MAKE_4x3(0);
		MAKE_4x3(1);

		FADD_4x3;

		MAKE_4x3(0);
		MAKE_4x3(1);

#pragma pipeloop(0)
		for (; i <= size - 8; i += 2) {
			*buff = res0;
			buff++;
			*buff = res1;
			buff++;
			FADD_4x3;
			MAKE_4x3(0);
			MAKE_4x3(1);
		}

		*buff = res0;
		buff++;
		*buff = res1;
		buff++;
		FADD_4x3;
		*buff = res0;
		buff++;
		*buff = res1;
		buff++;

		RESULT_1PIXEL_4x3(0);
		LOAD_1PIXEL_4x4;
		RESULT_1PIXEL_4x3(1);
		FADD_4x3;
		*buff = res0;
		buff++;
		*buff = res1;
		buff++;

		i += 6;
	}
#pragma pipeloop(0)
	for (; i < size; i++) {
		CALC_SRC_PTR;
		LOAD_1PIXEL_4x4;
		RESULT_1PIXEL_4x3(0);
		d0 = vis_fpadd16(d00, d10);
		res0 = vis_fpadd16(d0, d20);
		*buff = res0;
		buff++;
	}
}

/* *********************************************************** */

#define	RESULT_1PIXEL_3x3(ind)                                            \
	xFilter0 = xPtr[0];                                               \
	xFilter1 = xPtr[1];                                               \
	xFilter2 = xPtr[2];                                               \
	v00 = vis_fmul8x16au(vis_read_hi(row00), vis_read_hi(yFilter));   \
	v01 = vis_fmul8x16au(vis_read_lo(row00), vis_read_hi(yFilter));   \
	v02 = vis_fmul8x16au(vis_read_hi(row01), vis_read_hi(yFilter));   \
	v10 = vis_fmul8x16al(vis_read_hi(row10), vis_read_hi(yFilter));   \
	v11 = vis_fmul8x16al(vis_read_lo(row10), vis_read_hi(yFilter));   \
	sum0 = vis_fpadd16(v00, v10);                                     \
	v12 = vis_fmul8x16al(vis_read_hi(row11), vis_read_hi(yFilter));   \
	sum1 = vis_fpadd16(v01, v11);                                     \
	sum2 = vis_fpadd16(v02, v12);                                     \
	v20 = vis_fmul8x16au(vis_read_hi(row20), vis_read_lo(yFilter));   \
	v21 = vis_fmul8x16au(vis_read_lo(row20), vis_read_lo(yFilter));   \
	sum0 = vis_fpadd16(sum0, v20);                                    \
	v22 = vis_fmul8x16au(vis_read_hi(row21), vis_read_lo(yFilter));   \
	sum1 = vis_fpadd16(sum1, v21);                                    \
	sum2 = vis_fpadd16(sum2, v22);                                    \
	v00 = vis_fmul8sux16(sum0, xFilter0);                             \
	v01 = vis_fmul8ulx16(sum0, xFilter0);                             \
	v10 = vis_fmul8sux16(sum1, xFilter1);                             \
	d0##ind = vis_fpadd16(v00, v01);                                  \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                             \
	v20 = vis_fmul8sux16(sum2, xFilter2);                             \
	d1##ind = vis_fpadd16(v10, v11);                                  \
	v21 = vis_fmul8ulx16(sum2, xFilter2);                             \
	d2##ind = vis_fpadd16(v20, v21)

/* *********************************************************** */

#define	MAKE_3x3(index)                                                   \
	v00 = vis_fmul8x16au(vis_read_hi(row00), vis_read_hi(yFilter));   \
	xFilter0 = xPtr[0];                                               \
	v01 = vis_fmul8x16au(vis_read_lo(row00), vis_read_hi(yFilter));   \
	xFilter1 = xPtr[1];                                               \
	v02 = vis_fmul8x16au(vis_read_hi(row01), vis_read_hi(yFilter));   \
	xFilter2 = xPtr[2];                                               \
	vis_alignaddr(sPtr, 0);                                           \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                   \
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
	vis_alignaddr(sPtr, 0);                                           \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                   \
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
	vis_alignaddr(sPtr, 0);                                           \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));                   \
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
	d0##index = vis_fpadd16(v00, v01);                                \
	yFilter = *((mlib_d64 *)((mlib_u8 *)filterY + filterposy));       \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                             \
	xPtr = ((mlib_d64 *)((mlib_u8 *)filterX + 4 * filterposx));       \
	v20 = vis_fmul8sux16(sum2, xFilter2);                             \
	d1##index = vis_fpadd16(v10, v11);                                \
	v21 = vis_fmul8ulx16(sum2, xFilter2);                             \
	d2##index = vis_fpadd16(v20, v21);                                \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + (4 * xSrc)

/* *********************************************************** */

void
mlib_v_ImageAffineTableLine_8nw_3_3_4(
    mlib_d64 *buff,
    const mlib_d64 *filterX,
    const mlib_d64 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws)
{
	DECLAREVAR;
	mlib_d64 row20, row21;
	mlib_d64 v22;
	mlib_d64 yFilter;

	i = 0;

	if (i <= size - 6) {

		CALC_SRC_PTR;
		LOAD_1PIXEL_3x4;

		CALC_SRC_PTR;

		MAKE_3x3(0);
		MAKE_3x3(1);

		FADD_4x3;

		MAKE_3x3(0);
		MAKE_3x3(1);

#pragma pipeloop(0)
		for (; i <= size - 8; i += 2) {
			*buff = res0;
			buff++;
			*buff = res1;
			buff++;
			FADD_4x3;
			MAKE_3x3(0);
			MAKE_3x3(1);
		}

		*buff = res0;
		buff++;
		*buff = res1;
		buff++;
		FADD_4x3;
		*buff = res0;
		buff++;
		*buff = res1;
		buff++;

		RESULT_1PIXEL_3x3(0);
		LOAD_1PIXEL_3x4;
		RESULT_1PIXEL_3x3(1);
		FADD_4x3;
		*buff = res0;
		buff++;
		*buff = res1;
		buff++;

		i += 6;
	}
#pragma pipeloop(0)
	for (; i < size; i++) {
		CALC_SRC_PTR;
		LOAD_1PIXEL_3x4;
		RESULT_1PIXEL_3x3(0);
		d0 = vis_fpadd16(d00, d10);
		res0 = vis_fpadd16(d0, d20);
		*buff = res0;
		buff++;
	}
}

/* *********************************************************** */

#define	RESULT_1PIXEL_2x3(ind)                                  \
	xFilter0 = xPtr[0];                                     \
	xFilter1 = xPtr[1];                                     \
	xFilter2 = xPtr[2];                                     \
	v00 = vis_fmul8x16au(vis_read_hi(row00), yFilter);      \
	v01 = vis_fmul8x16au(vis_read_lo(row00), yFilter);      \
	v02 = vis_fmul8x16au(vis_read_hi(row01), yFilter);      \
	v10 = vis_fmul8x16al(vis_read_hi(row10), yFilter);      \
	v11 = vis_fmul8x16al(vis_read_lo(row10), yFilter);      \
	sum0 = vis_fpadd16(v00, v10);                           \
	v12 = vis_fmul8x16al(vis_read_hi(row11), yFilter);      \
	sum1 = vis_fpadd16(v01, v11);                           \
	sum2 = vis_fpadd16(v02, v12);                           \
	v00 = vis_fmul8sux16(sum0, xFilter0);                   \
	v01 = vis_fmul8ulx16(sum0, xFilter0);                   \
	v10 = vis_fmul8sux16(sum1, xFilter1);                   \
	d0##ind = vis_fpadd16(v00, v01);                        \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                   \
	v20 = vis_fmul8sux16(sum2, xFilter2);                   \
	d1##ind = vis_fpadd16(v10, v11);                        \
	v21 = vis_fmul8ulx16(sum2, xFilter2);                   \
	d2##ind = vis_fpadd16(v20, v21)

/* *********************************************************** */

#define	MAKE_2x3(index)                                               \
	v00 = vis_fmul8x16au(vis_read_hi(row00), yFilter);            \
	xFilter0 = xPtr[0];                                           \
	v01 = vis_fmul8x16au(vis_read_lo(row00), yFilter);            \
	xFilter1 = xPtr[1];                                           \
	v02 = vis_fmul8x16au(vis_read_hi(row01), yFilter);            \
	xFilter2 = xPtr[2];                                           \
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
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
	vis_alignaddr(sPtr, 0);                                       \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));               \
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
	d0##index = vis_fpadd16(v00, v01);                            \
	yFilter = *((mlib_f32 *)((mlib_u8 *)filterY + filterposy));   \
	v11 = vis_fmul8ulx16(sum1, xFilter1);                         \
	xPtr = ((mlib_d64 *)((mlib_u8 *)filterX + 4 * filterposx));   \
	v20 = vis_fmul8sux16(sum2, xFilter2);                         \
	d1##index = vis_fpadd16(v10, v11);                            \
	v21 = vis_fmul8ulx16(sum2, xFilter2);                         \
	d2##index = vis_fpadd16(v20, v21);                            \
	sPtr = (mlib_u8 *)lineAddr[ySrc] + (4 * xSrc)

/* *********************************************************** */

void
mlib_v_ImageAffineTableLine_8nw_2_3_4(
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
		LOAD_1PIXEL_2x4;

		CALC_SRC_PTR;

		MAKE_2x3(0);
		MAKE_2x3(1);

		FADD_4x3;

		MAKE_2x3(0);
		MAKE_2x3(1);

#pragma pipeloop(0)
		for (; i <= size - 8; i += 2) {
			*buff = res0;
			buff++;
			*buff = res1;
			buff++;
			FADD_4x3;
			MAKE_2x3(0);
			MAKE_2x3(1);
		}

		*buff = res0;
		buff++;
		*buff = res1;
		buff++;
		FADD_4x3;
		*buff = res0;
		buff++;
		*buff = res1;
		buff++;

		RESULT_1PIXEL_2x3(0);
		LOAD_1PIXEL_2x4;
		RESULT_1PIXEL_2x3(1);
		FADD_4x3;
		*buff = res0;
		buff++;
		*buff = res1;
		buff++;

		i += 6;
	}
#pragma pipeloop(0)
	for (; i < size; i++) {
		CALC_SRC_PTR;
		LOAD_1PIXEL_2x4;
		RESULT_1PIXEL_2x3(0);
		d0 = vis_fpadd16(d00, d10);
		res0 = vis_fpadd16(d0, d20);
		*buff = res0;
		buff++;
	}
}

/* *********************************************************** */
