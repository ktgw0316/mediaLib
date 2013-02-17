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

#define	TYPE_FILTER_X	mlib_d64
#define	TYPE_FILTER_Y	mlib_d64

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
	mlib_d64 v0, v1, v2, v3;                                \
	mlib_d64 xFilter;                                       \
	mlib_d64 res

/* *********************************************************** */

#define	DECLAREVAR1                                             \
	mlib_d64 d0, d1, d2, d3;                                \
	mlib_d64 *dpSrc;                                        \
	mlib_d64 data0, data1;                                  \
	TYPE_FILTER_Y yFilter

/* *********************************************************** */

#define	DECLAREVAR2                                             \
	mlib_d64 yFilter;                                       \
	mlib_d64 yFilter1;                                      \
	mlib_f32 *buff1 = (mlib_f32 *)buff;                     \
	mlib_f32 res1;                                          \
	mlib_u8 *sPtr1;                                         \
	mlib_f32 vis_scale = vis_to_float(0x00010001);          \
	mlib_d64 s0, s1;                                        \
	mlib_f32 fx0, fx1;                                      \
	TYPE_FILTER_Y fy0, fy1

/* *********************************************************** */

#define	CALC_SRC_PTR(Ptr)                                       \
	xSrc = (x >> x_shift);                                  \
	ySrc = (y >> y_shift);                                  \
	Ptr = (mlib_u8 *)lineAddr[ySrc] + xSrc

/* *********************************************************** */

#define	LOAD_FILTERS(fx, fy)                                           \
	filterposy = (y >> yf_shift) & yf_mask;                        \
	fy = *((TYPE_FILTER_Y *) ((mlib_u8 *)filterY + filterposy));   \
	filterposx = (x >> xf_shift) & xf_mask;                        \
	fx = *((TYPE_FILTER_X *) ((mlib_u8 *)filterX + filterposx));   \
	x += dx;                                                       \
	y += dy

/* *********************************************************** */

#define	LOAD_PIXELS(pix)                                        \
	vis_alignaddr(sPtr, 0);                                 \
	dpSrc = (mlib_d64 *)(((mlib_addr)sPtr) & (~7));         \
	data0 = dpSrc[0];                                       \
	data1 = dpSrc[1];                                       \
	pix = vis_faligndata(data0, data1)

/* *********************************************************** */

#define	LOAD_PIXEL_4                                            \
	LOAD_PIXELS(row00);                                     \
	sPtr += srcStride;                                      \
	LOAD_PIXELS(row10);                                     \
	sPtr += srcStride;                                      \
	LOAD_PIXELS(row20);                                     \
	sPtr += srcStride;                                      \
	LOAD_PIXELS(row30)

/* *********************************************************** */

#define	MAKE_4x4(dst)                                                    \
	CALC_SRC_PTR(sPtr);                                              \
	LOAD_PIXEL_4;                                                    \
	LOAD_FILTERS(xFilter, yFilter);                                  \
	v0 = vis_fmul8x16au(vis_read_hi(row00), vis_read_hi(yFilter));   \
	v1 = vis_fmul8x16al(vis_read_hi(row10), vis_read_hi(yFilter));   \
	sum = vis_fpadd16(v0, v1);                                       \
	v2 = vis_fmul8x16au(vis_read_hi(row20), vis_read_lo(yFilter));   \
	sum = vis_fpadd16(sum, v2);                                      \
	v3 = vis_fmul8x16al(vis_read_hi(row30), vis_read_lo(yFilter));   \
	sum = vis_fpadd16(sum, v3);                                      \
	v0 = vis_fmul8sux16(sum, xFilter);                               \
	v1 = vis_fmul8ulx16(sum, xFilter);                               \
	dst = vis_fpadd16(v1, v0)

/* *********************************************************** */

#define	FADD_4x1                                                        \
	v2 =                                                            \
	vis_freg_pair(vis_fpadd16s(vis_read_hi(d0), vis_read_lo(d0)),   \
	    vis_fpadd16s(vis_read_hi(d0), vis_read_lo(d0)));            \
	v3 = vis_bshuffle(v2, v2);                                      \
	res =                                                           \
	vis_write_lo(res, vis_fpadd16s(vis_read_hi(v3),                 \
	    vis_read_lo(v3)))

/* *********************************************************** */

#define	FADD_4x4                                                        \
	v0 =                                                            \
	vis_freg_pair(vis_fpadd16s(vis_read_hi(d0), vis_read_lo(d0)),   \
	    vis_fpadd16s(vis_read_hi(d1), vis_read_lo(d1)));            \
	v1 =                                                            \
	vis_freg_pair(vis_fpadd16s(vis_read_hi(d2), vis_read_lo(d2)),   \
	    vis_fpadd16s(vis_read_hi(d3), vis_read_lo(d3)));            \
	v2 = vis_bshuffle(v0, v0);                                      \
	v3 = vis_bshuffle(v1, v1);                                      \
	res =                                                           \
	vis_freg_pair(vis_fpadd16s(vis_read_hi(v2), vis_read_lo(v2)),   \
	    vis_fpadd16s(vis_read_hi(v3), vis_read_lo(v3)))

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
	mlib_d64 row20, row30;

	vis_write_bmask(0x0145ABEF, 0);
	dstPixelPtr = (mlib_s16 *)buff;

#pragma pipeloop(0)
	for (i = 0; i <= size - 4; i += 4) {
		MAKE_4x4(d0);
		MAKE_4x4(d1);
		MAKE_4x4(d2);
		MAKE_4x4(d3);

		FADD_4x4;
		*buff = res;
		buff++;
	}

	dstPixelPtr = (mlib_s16 *)buff;

	for (; i < size; i++) {
		MAKE_4x4(d0);
		FADD_4x1;
		vis_st_u16(res, dstPixelPtr++);
	}
}

/* *********************************************************** */

#define	LOAD_PIXEL_3                                            \
	LOAD_PIXELS(row00);                                     \
	sPtr += srcStride;                                      \
	LOAD_PIXELS(row10);                                     \
	sPtr += srcStride;                                      \
	LOAD_PIXELS(row20)

/* *********************************************************** */

#define	MAKE_3x4(dst)                                                    \
	CALC_SRC_PTR(sPtr);                                              \
	LOAD_PIXEL_3;                                                    \
	LOAD_FILTERS(xFilter, yFilter);                                  \
	v0 = vis_fmul8x16au(vis_read_hi(row00), vis_read_hi(yFilter));   \
	v1 = vis_fmul8x16al(vis_read_hi(row10), vis_read_hi(yFilter));   \
	sum = vis_fpadd16(v0, v1);                                       \
	v2 = vis_fmul8x16au(vis_read_hi(row20), vis_read_lo(yFilter));   \
	sum = vis_fpadd16(sum, v2);                                      \
	v0 = vis_fmul8sux16(sum, xFilter);                               \
	v1 = vis_fmul8ulx16(sum, xFilter);                               \
	dst = vis_fpadd16(v1, v0)

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
	mlib_d64 row20;

	vis_write_bmask(0x0145ABEF, 0);
	dstPixelPtr = (mlib_s16 *)buff;

#pragma pipeloop(0)
	for (i = 0; i <= size - 4; i += 4) {
		MAKE_3x4(d0);
		MAKE_3x4(d1);
		MAKE_3x4(d2);
		MAKE_3x4(d3);

		FADD_4x4;
		*buff = res;
		buff++;
	}

	dstPixelPtr = (mlib_s16 *)buff;

	for (; i < size; i++) {
		MAKE_3x4(d0);
		FADD_4x1;
		vis_st_u16(res, dstPixelPtr++);
	}
}

/* *********************************************************** */

#undef  TYPE_FILTER_Y
#define	TYPE_FILTER_Y	mlib_f32

/* *********************************************************** */

#define	MAKE_2x4(dst)                                           \
	CALC_SRC_PTR(sPtr);                                     \
	LOAD_PIXELS(row00);                                     \
	sPtr += srcStride;                                      \
	LOAD_PIXELS(row10);                                     \
	LOAD_FILTERS(xFilter, yFilter);                         \
	v0 = vis_fmul8x16au(vis_read_hi(row00), yFilter);       \
	v1 = vis_fmul8x16al(vis_read_hi(row10), yFilter);       \
	sum = vis_fpadd16(v0, v1);                              \
	v0 = vis_fmul8sux16(sum, xFilter);                      \
	v1 = vis_fmul8ulx16(sum, xFilter);                      \
	dst = vis_fpadd16(v1, v0)

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

	vis_write_bmask(0x0145ABEF, 0);
	dstPixelPtr = (mlib_s16 *)buff;

#pragma pipeloop(0)
	for (i = 0; i <= size - 4; i += 4) {
		MAKE_2x4(d0);
		MAKE_2x4(d1);
		MAKE_2x4(d2);
		MAKE_2x4(d3);

		FADD_4x4;
		*buff = res;
		buff++;
	}

	dstPixelPtr = (mlib_s16 *)buff;

	for (; i < size; i++) {
		MAKE_2x4(d0);
		FADD_4x1;
		vis_st_u16(res, dstPixelPtr++);
	}
}

/* *********************************************************** */

#undef  TYPE_FILTER_X
#define	TYPE_FILTER_X	mlib_f32
#undef  TYPE_FILTER_Y

#define	TYPE_FILTER_Y	mlib_d64

/* *********************************************************** */

#define	LD_U8(sp, x)	vis_read_lo(vis_ld_u8(sp + x))

/* *********************************************************** */

#define	CALC_2_SRC_PTR                                          \
	CALC_SRC_PTR(sPtr);                                     \
	LOAD_FILTERS(fx0, fy0);                                 \
	CALC_SRC_PTR(sPtr1);                                    \
	LOAD_FILTERS(fx1, fy1)

/* *********************************************************** */

#define	LOAD_2x2(r0, r1)                                                  \
	s0 = vis_fpmerge(LD_U8(sPtr, 0), LD_U8(sPtr, srcStride + 1));     \
	s1 = vis_fpmerge(LD_U8(sPtr1, 0), LD_U8(sPtr1, srcStride + 1));   \
	r0 = vis_fpmerge(vis_read_lo(s0), vis_read_lo(s1));               \
	s0 = vis_fpmerge(LD_U8(sPtr, srcStride), LD_U8(sPtr, 1));         \
	s1 = vis_fpmerge(LD_U8(sPtr1, srcStride), LD_U8(sPtr1, 1));       \
	r1 = vis_fpmerge(vis_read_lo(s0), vis_read_lo(s1))

/* *********************************************************** */

#define	LOAD_4x2                                                \
	LOAD_2x2(row00, row10);                                 \
	sPtr += 2 * srcStride;                                  \
	sPtr1 += 2 * srcStride;                                 \
	LOAD_2x2(row20, row30)

/* *********************************************************** */

#define	FILTER_MERGE_4x2                                                \
	xFilter = vis_freg_pair(fx0, fx1);                              \
	yFilter = vis_freg_pair(vis_read_hi(fy0), vis_read_hi(fy1));    \
	xFilter = vis_bshuffle(xFilter, xFilter);                       \
	yFilter = vis_bshuffle(yFilter, yFilter);                       \
	yFilter1 = vis_faligndata(yFilter, yFilter);                    \
	yFilter2 = vis_freg_pair(vis_read_lo(fy0), vis_read_lo(fy1));   \
	yFilter2 = vis_bshuffle(yFilter2, yFilter2);                    \
	yFilter3 = vis_faligndata(yFilter2, yFilter2)

/* *********************************************************** */

#define	MAKE_4x2                                                \
	v0 = vis_fmul8x16(vis_read_lo(row00), yFilter);         \
	v1 = vis_fmul8x16(vis_read_lo(row10), yFilter1);        \
	sum = vis_fpadd16(v0, v1);                              \
	v0 = vis_fmul8x16(vis_read_lo(row20), yFilter2);        \
	sum = vis_fpadd16(v0, sum);                             \
	v1 = vis_fmul8x16(vis_read_lo(row30), yFilter3);        \
	sum = vis_fpadd16(v1, sum);                             \
	v0 = vis_fmul8sux16(sum, xFilter);                      \
	v1 = vis_fmul8ulx16(sum, xFilter);                      \
	v3 = vis_fpadd16(v1, v0);                               \
	res1 = vis_fpadd16s(vis_read_hi(v3), vis_read_lo(v3))

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
	DECLAREVAR2;
	mlib_d64 yFilter2;
	mlib_d64 yFilter3;
	mlib_d64 row20, row30;
	mlib_d64 *dpSrc;
	mlib_d64 data0, data1;

	vis_write_gsr64((((mlib_u64)0x0145ABEF) << 32) + 4);
	dstPixelPtr = (mlib_s16 *)buff;

#pragma pipeloop(0)
	for (i = 0; i <= size - 2; i += 2) {
		CALC_2_SRC_PTR;
		LOAD_4x2;
		FILTER_MERGE_4x2;
		MAKE_4x2;
		*buff1 = res1;
		buff1++;
	}

	dstPixelPtr = (mlib_s16 *)buff1;

	for (; i < size; i++) {
		CALC_SRC_PTR(sPtr);
		LOAD_FILTERS(fx0, yFilter);
		xFilter = vis_write_hi(xFilter, fx0);
		LOAD_PIXEL_4;

		v0 = vis_fmul8x16au(vis_read_hi(row00), vis_read_hi(yFilter));
		v1 = vis_fmul8x16al(vis_read_hi(row10), vis_read_hi(yFilter));
		sum = vis_fpadd16(v0, v1);
		v0 = vis_fmul8x16au(vis_read_hi(row20), vis_read_lo(yFilter));
		sum = vis_fpadd16(v0, sum);
		v1 = vis_fmul8x16al(vis_read_hi(row30), vis_read_lo(yFilter));
		sum = vis_fpadd16(v1, sum);

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

#define	LOAD_3x2                                                 \
	LOAD_2x2(row00, row10);                                  \
	sPtr += 2 * srcStride;                                   \
	sPtr1 += 2 * srcStride;                                  \
	s0 = vis_fpmerge(LD_U8(sPtr, 0), zero);                  \
	s1 = vis_fpmerge(LD_U8(sPtr1, 0), zero);                 \
	row20 = vis_fpmerge(vis_read_lo(s0), vis_read_lo(s1));   \
	s0 = vis_fpmerge(zero, LD_U8(sPtr, 1));                  \
	s1 = vis_fpmerge(zero, LD_U8(sPtr1, 1));                 \
	row30 = vis_fpmerge(vis_read_lo(s0), vis_read_lo(s1))

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
	DECLAREVAR2;
	mlib_d64 yFilter2;
	mlib_d64 yFilter3;
	mlib_d64 row20, row30;
	mlib_d64 *dpSrc;
	mlib_d64 data0, data1, zero;

	vis_write_gsr64((((mlib_u64)0x0145ABEF) << 32) + 4);
	dstPixelPtr = (mlib_s16 *)buff;

	zero = vis_to_double_dup(0);

#pragma pipeloop(0)
	for (i = 0; i <= size - 2; i += 2) {
		CALC_2_SRC_PTR;
		LOAD_3x2;
		FILTER_MERGE_4x2;
		MAKE_4x2;
		*buff1 = res1;
		buff1++;
	}

	dstPixelPtr = (mlib_s16 *)buff1;

	for (; i < size; i++) {
		CALC_SRC_PTR(sPtr);
		LOAD_FILTERS(fx0, yFilter);
		xFilter = vis_write_hi(xFilter, fx0);
		LOAD_PIXEL_3;

		v0 = vis_fmul8x16au(vis_read_hi(row00), vis_read_hi(yFilter));
		v1 = vis_fmul8x16al(vis_read_hi(row10), vis_read_hi(yFilter));
		sum = vis_fpadd16(v0, v1);
		v0 = vis_fmul8x16au(vis_read_hi(row20), vis_read_lo(yFilter));
		sum = vis_fpadd16(v0, sum);

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

#undef  TYPE_FILTER_Y
#define	TYPE_FILTER_Y	mlib_f32

/* *********************************************************** */

#define	FILTER_MERGE                                            \
	xFilter = vis_freg_pair(fx0, fx1);                      \
	yFilter = vis_freg_pair(fy0, fy1);                      \
	xFilter = vis_bshuffle(xFilter, xFilter);               \
	yFilter = vis_bshuffle(yFilter, yFilter);               \
	yFilter1 = vis_faligndata(yFilter, yFilter)

/* *********************************************************** */

#define	MAKE_2x2                                                \
	v0 = vis_fmul8x16(vis_read_lo(row00), yFilter);         \
	v1 = vis_fmul8x16(vis_read_lo(row10), yFilter1);        \
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
	DECLAREVAR2;

	vis_write_gsr64((((mlib_u64)0x0145ABEF) << 32) + 4);
	dstPixelPtr = (mlib_s16 *)buff;

#pragma pipeloop(0)
	for (i = 0; i <= size - 2; i += 2) {
		CALC_2_SRC_PTR;
		LOAD_2x2(row00, row10);
		FILTER_MERGE;
		MAKE_2x2;
		*buff1 = res1;
		buff1++;
	}

	dstPixelPtr = (mlib_s16 *)buff1;

#pragma pipeloop(0)
	for (; i < size; i++) {
		CALC_SRC_PTR(sPtr);
		LOAD_FILTERS(fx0, fy0);
		xFilter = vis_write_lo(xFilter, fx0);

		row00 = vis_fpmerge(LD_U8(sPtr, 0), LD_U8(sPtr, 1));
		row10 =
		    vis_fpmerge(LD_U8(sPtr, srcStride), LD_U8(sPtr,
		    srcStride + 1));

		v0 = vis_fmul8x16au(vis_read_lo(row00), fy0);
		v1 = vis_fmul8x16al(vis_read_lo(row10), fy0);
		sum = vis_fpadd16(v0, v1);
		v0 = vis_fmul8sux16(sum, xFilter);
		v1 = vis_fmul8ulx16(sum, xFilter);
		v3 = vis_fpadd16(v1, v0);
		v2 = vis_fmuld8ulx16(vis_scale, vis_read_lo(v3));
		res =
		    vis_write_lo(res, vis_fpadd32s(vis_read_hi(v2),
		    vis_read_lo(v2)));

		vis_st_u16(res, dstPixelPtr++);
	}
}

/* *********************************************************** */
