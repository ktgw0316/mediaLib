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

#pragma ident	"@(#)mlib_v_ImageZoom_BC.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoom - image scaling with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoom(mlib_image       *dst,
 *                                 const mlib_image *src,
 *                                 mlib_f32         zoomx,
 *                                 mlib_f32         zoomy,
 *                                 mlib_filter      filter,
 *                                 mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image
 *      src       Pointer to source image
 *      zoomx     X zoom factor.
 *      zoomy     Y zoom factor.
 *      filter    Type of resampling filter.
 *      edge      Type of edge condition.
 *
 * DESCRIPTION
 *  The center of the source image is mapped to the center of the
 *  destination image.
 *
 *  The upper-left corner pixel of an image is located at (0.5, 0.5).
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

#include <mlib_image.h>
#include <mlib_v_ImageFilters.h>
#include <vis_proto.h>

/* *********************************************************** */

#define	_MLIB_VIS_VER_

#include <mlib_ImageZoom.h>

/* *********************************************************** */

#define	FUNC_NAME(CC)	mlib_v_ImageZoomBicubic_U8_##CC

/* *********************************************************** */

#define	_LD_D64(sp, ind)	sp[ind]

#define	LD_D64(sp, ind)	vis_ld_d64_nf(sp + ind)

/* *********************************************************** */

#define	FILTER_1	filter1
#define	FILTER_3	filter3

#define	FILTER_4	filter4

/* *********************************************************** */

#define	VARIABLE(FORMAT)                                             \
	FORMAT *sp = GetElemSubStruct(current, sp),                  \
		*dp = GetElemSubStruct(current, dp);                 \
	mlib_s32 i, j, cx,                                           \
		dx = GetElemStruct(DX),                              \
		dy = GetElemStruct(DY),                              \
		x = GetElemSubStruct(current, srcX) & MLIB_MASK,     \
		y = GetElemSubStruct(current, srcY) & MLIB_MASK,     \
		src_stride = GetElemStruct(src_stride),              \
		dst_stride = GetElemStruct(dst_stride),              \
		width  = GetElemSubStruct(current, width),           \
		height = GetElemSubStruct(current, height)

/* *********************************************************** */

#define	MLIB_XY_SHIFT	5
#define	MLIB_XY_MASK	(((1 << 8) - 1) << 3)

/* *********************************************************** */

#define	FPMUL16(src1, src2)                                     \
	vis_fpadd16(vis_fmul8sux16((src1), (src2)),             \
	    vis_fmul8ulx16((src1), (src2)))

/* *********************************************************** */

#define	COEFF_D64_U8_3(r)                                       \
	fpos = (r >> MLIB_XY_SHIFT) & MLIB_XY_MASK;             \
	dfPtr = (mlib_d64 *)((mlib_u8 *)FILTER_3 + fpos * 3);   \
	d##r##f0 = dfPtr[0];                                    \
	d##r##f1 = dfPtr[1];                                    \
	d##r##f2 = dfPtr[2]

/* *********************************************************** */

#define	COEFF_D64_U8(r)                                         \
	fpos = (r >> MLIB_XY_SHIFT) & MLIB_XY_MASK;             \
	dfPtr = (mlib_d64 *)((mlib_u8 *)FILTER_4 + fpos * 4);   \
	d##r##f0 = dfPtr[0];                                    \
	d##r##f1 = dfPtr[1];                                    \
	d##r##f2 = dfPtr[2];                                    \
	d##r##f3 = dfPtr[3]

/* *********************************************************** */

#define	COEFF_D64_U8_1(r)                                       \
	fpos = (r >> MLIB_XY_SHIFT) & MLIB_XY_MASK;             \
	d##r##f = *(mlib_d64 *)((mlib_u8 *)FILTER_1 + fpos)

/* *********************************************************** */

#define	LOAD_U8_2_BC                                            \
	cx = (x >> MLIB_SHIFT) * 2;                             \
	x += dx;                                                \
	dsp = (mlib_d64 *)vis_alignaddr(tsp + cx, 0);           \
	dtmp0 = dsp[0];                                         \
	dtmp1 = dsp[1];                                         \
	A0 = vis_faligndata(dtmp0, dtmp1);                      \
	cx += src_stride;                                       \
	dsp = (mlib_d64 *)vis_alignaddr(tsp + cx, 0);           \
	dtmp0 = dsp[0];                                         \
	dtmp1 = dsp[1];                                         \
	B0 = vis_faligndata(dtmp0, dtmp1);                      \
	cx += src_stride;                                       \
	dsp = (mlib_d64 *)vis_alignaddr(tsp + cx, 0);           \
	dtmp0 = dsp[0];                                         \
	dtmp1 = dsp[1];                                         \
	C0 = vis_faligndata(dtmp0, dtmp1);                      \
	cx += src_stride;                                       \
	dsp = (mlib_d64 *)vis_alignaddr(tsp + cx, 0);           \
	dtmp0 = dsp[0];                                         \
	dtmp1 = dsp[1];                                         \
	D0 = vis_faligndata(dtmp0, dtmp1)

/* *********************************************************** */

#define	FMUL_U8_BC_2                                            \
	r00 = vis_fmul8x16(vis_read_hi(A0), dyf0);              \
	r02 = vis_fmul8x16(vis_read_lo(A0), dyf0);              \
	r10 = vis_fmul8x16(vis_read_hi(B0), dyf1);              \
	r12 = vis_fmul8x16(vis_read_lo(B0), dyf1);              \
	r20 = vis_fmul8x16(vis_read_hi(C0), dyf2);              \
	r22 = vis_fmul8x16(vis_read_lo(C0), dyf2);              \
	r30 = vis_fmul8x16(vis_read_hi(D0), dyf3);              \
	r32 = vis_fmul8x16(vis_read_lo(D0), dyf3)

/* *********************************************************** */

#define	FADD_U8_BC_2                                            \
	r40 = vis_fpadd16(r00, r10);                            \
	r42 = vis_fpadd16(r02, r12);                            \
	r50 = vis_fpadd16(r20, r30);                            \
	r52 = vis_fpadd16(r22, r32);                            \
	r60 = vis_fpadd16(r40, r50);                            \
	r62 = vis_fpadd16(r42, r52);                            \
	r70 = FPMUL16(r60, dxx0);                               \
	r72 = FPMUL16(r62, dxx1);                               \
	r80 = vis_fpadd16(r70, r72);                            \
	res =                                                   \
	vis_fmuld8ulx16(fone, vis_fpadd16s(vis_read_lo(r80),    \
	    vis_read_hi(r80)))

/* *********************************************************** */

#define	COEFF_D64_U8_2                                                     \
	dxx0 =                                                             \
	vis_for(vis_fandnot(mask_ffff, dxf0), vis_fand(mask_ffff,          \
	    dxf1));                                                        \
	dxx1 =                                                             \
	vis_for(vis_fandnot(mask_ffff, dxf2), vis_fand(mask_ffff, dxf3))

/* *********************************************************** */

#define	LOAD_U8_3_BC(A0, A1, B0, B1, C0, C1, D0, D1)            \
	cx = (x >> MLIB_SHIFT) * 3;                             \
	x += dx;                                                \
	dsp = (mlib_d64 *)vis_alignaddr(tsp + cx, 0);           \
	dtmp0 = dsp[0];                                         \
	dtmp1 = dsp[1];                                         \
	dtmp2 = LD_D64(dsp, 2);                                 \
	A0 = vis_faligndata(dtmp0, dtmp1);                      \
	A1 = vis_faligndata(dtmp1, dtmp2);                      \
	cx += src_stride;                                       \
	dsp = (mlib_d64 *)vis_alignaddr(tsp + cx, 0);           \
	dtmp0 = dsp[0];                                         \
	dtmp1 = dsp[1];                                         \
	dtmp2 = LD_D64(dsp, 2);                                 \
	B0 = vis_faligndata(dtmp0, dtmp1);                      \
	B1 = vis_faligndata(dtmp1, dtmp2);                      \
	cx += src_stride;                                       \
	dsp = (mlib_d64 *)vis_alignaddr(tsp + cx, 0);           \
	dtmp0 = dsp[0];                                         \
	dtmp1 = dsp[1];                                         \
	dtmp2 = LD_D64(dsp, 2);                                 \
	C0 = vis_faligndata(dtmp0, dtmp1);                      \
	C1 = vis_faligndata(dtmp1, dtmp2);                      \
	cx += src_stride;                                       \
	dsp = (mlib_d64 *)vis_alignaddr(tsp + cx, 0);           \
	dtmp0 = dsp[0];                                         \
	dtmp1 = dsp[1];                                         \
	dtmp2 = LD_D64(dsp, 2);                                 \
	D0 = vis_faligndata(dtmp0, dtmp1);                      \
	D1 = vis_faligndata(dtmp1, dtmp2)

/* *********************************************************** */

#define	FMUL_U8_3_BC(r)                                              \
	r##00 = vis_fmul8x16au(vis_read_hi(A0), vis_read_hi(dyf));   \
	r##01 = vis_fmul8x16au(vis_read_lo(A0), vis_read_hi(dyf));   \
	r##02 = vis_fmul8x16au(vis_read_hi(A1), vis_read_hi(dyf));   \
	r##10 = vis_fmul8x16al(vis_read_hi(B0), vis_read_hi(dyf));   \
	r##11 = vis_fmul8x16al(vis_read_lo(B0), vis_read_hi(dyf));   \
	r##12 = vis_fmul8x16al(vis_read_hi(B1), vis_read_hi(dyf));   \
	r##20 = vis_fmul8x16au(vis_read_hi(C0), vis_read_lo(dyf));   \
	r##21 = vis_fmul8x16au(vis_read_lo(C0), vis_read_lo(dyf));   \
	r##22 = vis_fmul8x16au(vis_read_hi(C1), vis_read_lo(dyf));   \
	r##30 = vis_fmul8x16al(vis_read_hi(D0), vis_read_lo(dyf));   \
	r##31 = vis_fmul8x16al(vis_read_lo(D0), vis_read_lo(dyf));   \
	r##32 = vis_fmul8x16al(vis_read_hi(D1), vis_read_lo(dyf))

/* *********************************************************** */

#define	FADD_U8_3_BC(r)                                         \
	r##40 = vis_fpadd16(r##00, r##10);                      \
	r##41 = vis_fpadd16(r##01, r##11);                      \
	r##42 = vis_fpadd16(r##02, r##12);                      \
	r##50 = vis_fpadd16(r##20, r##30);                      \
	r##51 = vis_fpadd16(r##21, r##31);                      \
	r##52 = vis_fpadd16(r##22, r##32);                      \
	r##60 = vis_fpadd16(r##40, r##50);                      \
	r##61 = vis_fpadd16(r##41, r##51);                      \
	r##62 = vis_fpadd16(r##42, r##52);                      \
	r##70 = FPMUL16(r##60, dxf0);                           \
	r##71 = FPMUL16(r##61, dxf1);                           \
	r##72 = FPMUL16(r##62, dxf2)

/* *********************************************************** */

#define	RES_U8_3_BC(r, res)                                     \
	vis_alignaddr(dsp, 6);                                  \
	r##71_1 = vis_faligndata(r##70, r##71);                 \
	vis_alignaddr(dsp, 2);                                  \
	r##71_2 = vis_faligndata(r##71, r##72);                 \
	r##80 = vis_fpadd16(r##70, r##71_1);                    \
	r##81 = vis_fpadd16(r##72, r##71_2);                    \
	r##81_1 = vis_faligndata(r##81, r##81);                 \
	res = vis_fpadd16(r##80, r##81_1)

/* *********************************************************** */

mlib_status FUNC_NAME(1s) (
    mlib_work_image *param)
{
	VARIABLE(mlib_u8);
	mlib_u8 *filter1 = GetElemStruct(filter1);
	mlib_u8 *tsp, *tdp;
	mlib_s32 fpos;
	mlib_f32 fsum, fsum0, fsum1, fsum2, fsum3,
	    fone = vis_to_float(0x00010001);
	mlib_d64 *dsp, dtmp0, dtmp1,
	    dxf, dyf,
	    r0, r1, r2, r3, r4, r5,
	    r6, r7, r8, r9, r10, r11,
	    r70, r71, r72, r73, res, res0, res1, A, B, C, D;

	sp -= src_stride + 1;
	tsp = sp;

	vis_write_gsr(3 << 3);

	for (j = 0; j < height; j++) {

		tdp = dp;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		fpos = (y >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
		dyf = *(mlib_d64 *)((mlib_u8 *)FILTER_1 + fpos);

		for (i = 0; i < width && ((mlib_addr)tdp & 3); i++) {
			fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
			dxf = *(mlib_d64 *)((mlib_u8 *)FILTER_1 + fpos);
			cx = x >> MLIB_SHIFT;
			x += dx;
			dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
			dtmp0 = LD_D64(dsp, 0);
			dtmp1 = LD_D64(dsp, 1);
			A = vis_faligndata(dtmp0, dtmp1);
			cx += src_stride;
			dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
			dtmp0 = LD_D64(dsp, 0);
			dtmp1 = LD_D64(dsp, 1);
			B = vis_faligndata(dtmp0, dtmp1);
			cx += src_stride;
			dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
			dtmp0 = LD_D64(dsp, 0);
			dtmp1 = LD_D64(dsp, 1);
			C = vis_faligndata(dtmp0, dtmp1);
			cx += src_stride;
			dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
			dtmp0 = LD_D64(dsp, 0);
			dtmp1 = LD_D64(dsp, 1);
			D = vis_faligndata(dtmp0, dtmp1);
			r0 = vis_fmul8x16au(vis_read_hi(A), vis_read_hi(dyf));
			r1 = vis_fmul8x16al(vis_read_hi(B), vis_read_hi(dyf));
			r2 = vis_fmul8x16au(vis_read_hi(C), vis_read_lo(dyf));
			r3 = vis_fmul8x16al(vis_read_hi(D), vis_read_lo(dyf));
			r4 = vis_fpadd16(r0, r1);
			r5 = vis_fpadd16(r2, r3);
			r6 = vis_fpadd16(r4, r5);
			r7 = vis_fpadd16(vis_fmul8sux16(r6, dxf),
			    vis_fmul8ulx16(r6, dxf));
			fsum = vis_fpadd16s(vis_read_hi(r7), vis_read_lo(r7));
			r8 = vis_fmuld8ulx16(fone, fsum);
			fsum = vis_fpadd16s(vis_read_hi(r8), vis_read_lo(r8));
			r9 = vis_fmuld8ulx16(fone, fsum);
			r11 = vis_fpack16_pair(r9, r9);
			vis_st_u8(r11, tdp);
			tdp++;
		}

		fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
		dxf = *(mlib_d64 *)((mlib_u8 *)FILTER_1 + fpos);
		cx = x >> MLIB_SHIFT;
		x += dx;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		A = vis_faligndata(dtmp0, dtmp1);
		cx += src_stride;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		B = vis_faligndata(dtmp0, dtmp1);
		cx += src_stride;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		C = vis_faligndata(dtmp0, dtmp1);
		cx += src_stride;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		D = vis_faligndata(dtmp0, dtmp1);
		r0 = vis_fmul8x16au(vis_read_hi(A), vis_read_hi(dyf));
		r1 = vis_fmul8x16al(vis_read_hi(B), vis_read_hi(dyf));
		r2 = vis_fmul8x16au(vis_read_hi(C), vis_read_lo(dyf));
		r3 = vis_fmul8x16al(vis_read_hi(D), vis_read_lo(dyf));
		r4 = vis_fpadd16(r0, r1);
		r5 = vis_fpadd16(r2, r3);
		r6 = vis_fpadd16(r4, r5);
		r70 =
		    vis_fpadd16(vis_fmul8sux16(r6, dxf), vis_fmul8ulx16(r6,
		    dxf));

		fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
		dxf = *(mlib_d64 *)((mlib_u8 *)FILTER_1 + fpos);
		cx = x >> MLIB_SHIFT;
		x += dx;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		A = vis_faligndata(dtmp0, dtmp1);
		cx += src_stride;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		B = vis_faligndata(dtmp0, dtmp1);
		cx += src_stride;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		C = vis_faligndata(dtmp0, dtmp1);
		cx += src_stride;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		D = vis_faligndata(dtmp0, dtmp1);
		r0 = vis_fmul8x16au(vis_read_hi(A), vis_read_hi(dyf));
		r1 = vis_fmul8x16al(vis_read_hi(B), vis_read_hi(dyf));
		r2 = vis_fmul8x16au(vis_read_hi(C), vis_read_lo(dyf));
		r3 = vis_fmul8x16al(vis_read_hi(D), vis_read_lo(dyf));
		r4 = vis_fpadd16(r0, r1);
		r5 = vis_fpadd16(r2, r3);
		r6 = vis_fpadd16(r4, r5);
		r71 =
		    vis_fpadd16(vis_fmul8sux16(r6, dxf), vis_fmul8ulx16(r6,
		    dxf));

		fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
		dxf = *(mlib_d64 *)((mlib_u8 *)FILTER_1 + fpos);
		cx = x >> MLIB_SHIFT;
		x += dx;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		A = vis_faligndata(dtmp0, dtmp1);
		cx += src_stride;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		B = vis_faligndata(dtmp0, dtmp1);
		cx += src_stride;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		C = vis_faligndata(dtmp0, dtmp1);
		cx += src_stride;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		D = vis_faligndata(dtmp0, dtmp1);
		r0 = vis_fmul8x16au(vis_read_hi(A), vis_read_hi(dyf));
		r1 = vis_fmul8x16al(vis_read_hi(B), vis_read_hi(dyf));
		r2 = vis_fmul8x16au(vis_read_hi(C), vis_read_lo(dyf));
		r3 = vis_fmul8x16al(vis_read_hi(D), vis_read_lo(dyf));
		r4 = vis_fpadd16(r0, r1);
		r5 = vis_fpadd16(r2, r3);
		r6 = vis_fpadd16(r4, r5);
		r72 =
		    vis_fpadd16(vis_fmul8sux16(r6, dxf), vis_fmul8ulx16(r6,
		    dxf));

		fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
		dxf = *(mlib_d64 *)((mlib_u8 *)FILTER_1 + fpos);
		cx = x >> MLIB_SHIFT;
		x += dx;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		A = vis_faligndata(dtmp0, dtmp1);
		cx += src_stride;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		B = vis_faligndata(dtmp0, dtmp1);
		cx += src_stride;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		C = vis_faligndata(dtmp0, dtmp1);
		cx += src_stride;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		D = vis_faligndata(dtmp0, dtmp1);
		r0 = vis_fmul8x16au(vis_read_hi(A), vis_read_hi(dyf));
		r1 = vis_fmul8x16al(vis_read_hi(B), vis_read_hi(dyf));
		r2 = vis_fmul8x16au(vis_read_hi(C), vis_read_lo(dyf));
		r3 = vis_fmul8x16al(vis_read_hi(D), vis_read_lo(dyf));
		r4 = vis_fpadd16(r0, r1);
		r5 = vis_fpadd16(r2, r3);
		r6 = vis_fpadd16(r4, r5);
		r73 =
		    vis_fpadd16(vis_fmul8sux16(r6, dxf), vis_fmul8ulx16(r6,
		    dxf));

		fsum0 = vis_fpadd16s(vis_read_hi(r70), vis_read_lo(r70));
		fsum1 = vis_fpadd16s(vis_read_hi(r71), vis_read_lo(r71));
		fsum2 = vis_fpadd16s(vis_read_hi(r72), vis_read_lo(r72));
		fsum3 = vis_fpadd16s(vis_read_hi(r73), vis_read_lo(r73));
		res0 = vis_fpmerge(fsum0, fsum2);
		res1 = vis_fpmerge(fsum1, fsum3);
		r8 = vis_fpmerge(vis_read_hi(res0), vis_read_hi(res1));
		r10 = vis_fpmerge(vis_read_hi(r8), vis_read_lo(r8));
		r9 = vis_fpmerge(vis_read_lo(res0), vis_read_lo(res1));
		r11 = vis_fpmerge(vis_read_hi(r9), vis_read_lo(r9));
		res = vis_fpadd16(r10, r11);

		fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
		dxf = *(mlib_d64 *)((mlib_u8 *)FILTER_1 + fpos);
		cx = x >> MLIB_SHIFT;
		x += dx;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		A = vis_faligndata(dtmp0, dtmp1);
		cx += src_stride;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		B = vis_faligndata(dtmp0, dtmp1);
		cx += src_stride;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		C = vis_faligndata(dtmp0, dtmp1);
		cx += src_stride;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		D = vis_faligndata(dtmp0, dtmp1);
		r0 = vis_fmul8x16au(vis_read_hi(A), vis_read_hi(dyf));
		r1 = vis_fmul8x16al(vis_read_hi(B), vis_read_hi(dyf));
		r2 = vis_fmul8x16au(vis_read_hi(C), vis_read_lo(dyf));
		r3 = vis_fmul8x16al(vis_read_hi(D), vis_read_lo(dyf));
		r4 = vis_fpadd16(r0, r1);
		r5 = vis_fpadd16(r2, r3);
		r6 = vis_fpadd16(r4, r5);
		r70 =
		    vis_fpadd16(vis_fmul8sux16(r6, dxf), vis_fmul8ulx16(r6,
		    dxf));

		fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
		dxf = *(mlib_d64 *)((mlib_u8 *)FILTER_1 + fpos);
		cx = x >> MLIB_SHIFT;
		x += dx;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		A = vis_faligndata(dtmp0, dtmp1);
		cx += src_stride;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		B = vis_faligndata(dtmp0, dtmp1);
		cx += src_stride;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		C = vis_faligndata(dtmp0, dtmp1);
		cx += src_stride;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		D = vis_faligndata(dtmp0, dtmp1);
		r0 = vis_fmul8x16au(vis_read_hi(A), vis_read_hi(dyf));
		r1 = vis_fmul8x16al(vis_read_hi(B), vis_read_hi(dyf));
		r2 = vis_fmul8x16au(vis_read_hi(C), vis_read_lo(dyf));
		r3 = vis_fmul8x16al(vis_read_hi(D), vis_read_lo(dyf));
		r4 = vis_fpadd16(r0, r1);
		r5 = vis_fpadd16(r2, r3);
		r6 = vis_fpadd16(r4, r5);
		r71 =
		    vis_fpadd16(vis_fmul8sux16(r6, dxf), vis_fmul8ulx16(r6,
		    dxf));

		fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
		dxf = *(mlib_d64 *)((mlib_u8 *)FILTER_1 + fpos);
		cx = x >> MLIB_SHIFT;
		x += dx;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		A = vis_faligndata(dtmp0, dtmp1);
		cx += src_stride;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		B = vis_faligndata(dtmp0, dtmp1);
		cx += src_stride;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		C = vis_faligndata(dtmp0, dtmp1);
		cx += src_stride;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		D = vis_faligndata(dtmp0, dtmp1);
		r0 = vis_fmul8x16au(vis_read_hi(A), vis_read_hi(dyf));
		r1 = vis_fmul8x16al(vis_read_hi(B), vis_read_hi(dyf));
		r2 = vis_fmul8x16au(vis_read_hi(C), vis_read_lo(dyf));
		r3 = vis_fmul8x16al(vis_read_hi(D), vis_read_lo(dyf));
		r4 = vis_fpadd16(r0, r1);
		r5 = vis_fpadd16(r2, r3);
		r6 = vis_fpadd16(r4, r5);
		r72 =
		    vis_fpadd16(vis_fmul8sux16(r6, dxf), vis_fmul8ulx16(r6,
		    dxf));

		fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
		dxf = *(mlib_d64 *)((mlib_u8 *)FILTER_1 + fpos);
		cx = x >> MLIB_SHIFT;
		x += dx;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		A = vis_faligndata(dtmp0, dtmp1);
		cx += src_stride;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		B = vis_faligndata(dtmp0, dtmp1);
		cx += src_stride;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		C = vis_faligndata(dtmp0, dtmp1);
		cx += src_stride;
		dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
		dtmp0 = LD_D64(dsp, 0);
		dtmp1 = LD_D64(dsp, 1);
		D = vis_faligndata(dtmp0, dtmp1);
		r0 = vis_fmul8x16au(vis_read_hi(A), vis_read_hi(dyf));
		r1 = vis_fmul8x16al(vis_read_hi(B), vis_read_hi(dyf));
		r2 = vis_fmul8x16au(vis_read_hi(C), vis_read_lo(dyf));
		r3 = vis_fmul8x16al(vis_read_hi(D), vis_read_lo(dyf));
		r4 = vis_fpadd16(r0, r1);
		r5 = vis_fpadd16(r2, r3);
		r6 = vis_fpadd16(r4, r5);
		r73 =
		    vis_fpadd16(vis_fmul8sux16(r6, dxf), vis_fmul8ulx16(r6,
		    dxf));

		for (; i < (width - 4); i += 4) {

			*(mlib_f32 *)tdp = vis_fpack16(res);

			fsum0 =
			    vis_fpadd16s(vis_read_hi(r70), vis_read_lo(r70));
			fsum1 =
			    vis_fpadd16s(vis_read_hi(r71), vis_read_lo(r71));
			fsum2 =
			    vis_fpadd16s(vis_read_hi(r72), vis_read_lo(r72));
			fsum3 =
			    vis_fpadd16s(vis_read_hi(r73), vis_read_lo(r73));
			res0 = vis_fpmerge(fsum0, fsum2);
			res1 = vis_fpmerge(fsum1, fsum3);
			r8 = vis_fpmerge(vis_read_hi(res0), vis_read_hi(res1));
			r10 = vis_fpmerge(vis_read_hi(r8), vis_read_lo(r8));
			r9 = vis_fpmerge(vis_read_lo(res0), vis_read_lo(res1));
			r11 = vis_fpmerge(vis_read_hi(r9), vis_read_lo(r9));
			res = vis_fpadd16(r10, r11);

			fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
			dxf = *(mlib_d64 *)((mlib_u8 *)FILTER_1 + fpos);
			cx = x >> MLIB_SHIFT;
			x += dx;
			dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
			dtmp0 = LD_D64(dsp, 0);
			dtmp1 = LD_D64(dsp, 1);
			A = vis_faligndata(dtmp0, dtmp1);
			cx += src_stride;
			dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
			dtmp0 = LD_D64(dsp, 0);
			dtmp1 = LD_D64(dsp, 1);
			B = vis_faligndata(dtmp0, dtmp1);
			cx += src_stride;
			dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
			dtmp0 = LD_D64(dsp, 0);
			dtmp1 = LD_D64(dsp, 1);
			C = vis_faligndata(dtmp0, dtmp1);
			cx += src_stride;
			dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
			dtmp0 = LD_D64(dsp, 0);
			dtmp1 = LD_D64(dsp, 1);
			D = vis_faligndata(dtmp0, dtmp1);
			r0 = vis_fmul8x16au(vis_read_hi(A), vis_read_hi(dyf));
			r1 = vis_fmul8x16al(vis_read_hi(B), vis_read_hi(dyf));
			r2 = vis_fmul8x16au(vis_read_hi(C), vis_read_lo(dyf));
			r3 = vis_fmul8x16al(vis_read_hi(D), vis_read_lo(dyf));

			cx = x >> MLIB_SHIFT;
			dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
			dtmp0 = LD_D64(dsp, 0);
			dtmp1 = LD_D64(dsp, 1);
			A = vis_faligndata(dtmp0, dtmp1);
			cx += src_stride;
			dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
			dtmp0 = LD_D64(dsp, 0);
			dtmp1 = LD_D64(dsp, 1);
			B = vis_faligndata(dtmp0, dtmp1);
			cx += src_stride;
			dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
			dtmp0 = LD_D64(dsp, 0);
			dtmp1 = LD_D64(dsp, 1);
			C = vis_faligndata(dtmp0, dtmp1);
			cx += src_stride;
			dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
			dtmp0 = LD_D64(dsp, 0);
			dtmp1 = LD_D64(dsp, 1);
			D = vis_faligndata(dtmp0, dtmp1);

			r4 = vis_fpadd16(r0, r1);
			r5 = vis_fpadd16(r2, r3);
			r6 = vis_fpadd16(r4, r5);
			r70 =
			    vis_fpadd16(vis_fmul8sux16(r6, dxf),
			    vis_fmul8ulx16(r6, dxf));

			fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
			dxf = *(mlib_d64 *)((mlib_u8 *)FILTER_1 + fpos);
			x += dx;

			r0 = vis_fmul8x16au(vis_read_hi(A), vis_read_hi(dyf));
			r1 = vis_fmul8x16al(vis_read_hi(B), vis_read_hi(dyf));
			r2 = vis_fmul8x16au(vis_read_hi(C), vis_read_lo(dyf));
			r3 = vis_fmul8x16al(vis_read_hi(D), vis_read_lo(dyf));

			cx = x >> MLIB_SHIFT;
			dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
			dtmp0 = LD_D64(dsp, 0);
			dtmp1 = LD_D64(dsp, 1);
			A = vis_faligndata(dtmp0, dtmp1);
			cx += src_stride;
			dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
			dtmp0 = LD_D64(dsp, 0);
			dtmp1 = LD_D64(dsp, 1);
			B = vis_faligndata(dtmp0, dtmp1);
			cx += src_stride;
			dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
			dtmp0 = LD_D64(dsp, 0);
			dtmp1 = LD_D64(dsp, 1);
			C = vis_faligndata(dtmp0, dtmp1);
			cx += src_stride;
			dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
			dtmp0 = LD_D64(dsp, 0);
			dtmp1 = LD_D64(dsp, 1);
			D = vis_faligndata(dtmp0, dtmp1);

			r4 = vis_fpadd16(r0, r1);
			r5 = vis_fpadd16(r2, r3);
			r6 = vis_fpadd16(r4, r5);
			r71 =
			    vis_fpadd16(vis_fmul8sux16(r6, dxf),
			    vis_fmul8ulx16(r6, dxf));

			fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
			dxf = *(mlib_d64 *)((mlib_u8 *)FILTER_1 + fpos);
			x += dx;

			r0 = vis_fmul8x16au(vis_read_hi(A), vis_read_hi(dyf));
			r1 = vis_fmul8x16al(vis_read_hi(B), vis_read_hi(dyf));
			r2 = vis_fmul8x16au(vis_read_hi(C), vis_read_lo(dyf));
			r3 = vis_fmul8x16al(vis_read_hi(D), vis_read_lo(dyf));

			cx = x >> MLIB_SHIFT;
			dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
			dtmp0 = LD_D64(dsp, 0);
			dtmp1 = LD_D64(dsp, 1);
			A = vis_faligndata(dtmp0, dtmp1);
			cx += src_stride;
			dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
			dtmp0 = LD_D64(dsp, 0);
			dtmp1 = LD_D64(dsp, 1);
			B = vis_faligndata(dtmp0, dtmp1);
			cx += src_stride;
			dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
			dtmp0 = LD_D64(dsp, 0);
			dtmp1 = LD_D64(dsp, 1);
			C = vis_faligndata(dtmp0, dtmp1);
			cx += src_stride;
			dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
			dtmp0 = LD_D64(dsp, 0);
			dtmp1 = LD_D64(dsp, 1);
			D = vis_faligndata(dtmp0, dtmp1);

			r4 = vis_fpadd16(r0, r1);
			r5 = vis_fpadd16(r2, r3);
			r6 = vis_fpadd16(r4, r5);
			r72 =
			    vis_fpadd16(vis_fmul8sux16(r6, dxf),
			    vis_fmul8ulx16(r6, dxf));

			fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
			dxf = *(mlib_d64 *)((mlib_u8 *)FILTER_1 + fpos);

			r0 = vis_fmul8x16au(vis_read_hi(A), vis_read_hi(dyf));
			r1 = vis_fmul8x16al(vis_read_hi(B), vis_read_hi(dyf));
			r2 = vis_fmul8x16au(vis_read_hi(C), vis_read_lo(dyf));
			r3 = vis_fmul8x16al(vis_read_hi(D), vis_read_lo(dyf));
			r4 = vis_fpadd16(r0, r1);
			r5 = vis_fpadd16(r2, r3);
			r6 = vis_fpadd16(r4, r5);
			r73 =
			    vis_fpadd16(vis_fmul8sux16(r6, dxf),
			    vis_fmul8ulx16(r6, dxf));
			x += dx;

			tdp += 4;
		}

		{
			mlib_f32 df = vis_fpack16(res);
			mlib_u8 *pp = (mlib_u8 *)&df;

			for (; i < width; i++) {
				(*tdp++) = (*pp++);
			}
		}

		y += dy;
		dp += dst_stride;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(2s) (
    mlib_work_image *param)
{
	VARIABLE(mlib_u8);
	mlib_u8 *filter4 = GetElemStruct(filter4);
	union
	{
		mlib_u8 t[4];
		mlib_f32 f;
	} f0;
	mlib_u8 *tsp, *tdp;
	mlib_f32 fone = vis_to_float(0x00010001);
	mlib_s32 fpos;
	mlib_d64 *dfPtr, *dsp, dtmp0, dtmp1,
	    dyf0, dyf1, dyf2, dyf3,
	    dxf0, dxf1, dxf2, dxf3, dxx0, dxx1,
	    r00, r10, r20, r30, r40, r50, r60, r70, r80,
	    r02, r12, r22, r32, r42, r52, r62, r72,
	    A0, B0, C0, D0, mask_ffff = vis_to_double(0, 0xFFFFFFFF), res;

	vis_write_gsr(3 << 3);
	sp -= src_stride + 2;
	tsp = sp;

	for (j = 0; j < height; j++) {

		x = GetElemSubStruct(current, srcX) & MLIB_MASK;
		COEFF_D64_U8(y);
		tdp = dp;

#pragma pipeloop(0)
		for (i = 0; i < width; i++, tdp += 2) {
			COEFF_D64_U8(x);
			COEFF_D64_U8_2;
			LOAD_U8_2_BC;
			FMUL_U8_BC_2;
			FADD_U8_BC_2;
			f0.f = vis_fpack16(res);

			tdp[0] = f0.t[1];
			tdp[1] = f0.t[3];
		}

		y += dy;
		dp += dst_stride;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(3s) (
    mlib_work_image *param)
{
	VARIABLE(mlib_u8);
	mlib_u8 *filter1 = GetElemStruct(filter1);
	mlib_u8 *filter3 = GetElemStruct(filter3);
	union
	{
		mlib_u8 t[4];
		mlib_f32 f;
	} f0;
	mlib_u8 *tsp, *tdp;
	mlib_s32 fpos;
	mlib_d64 *dfPtr,
	    *dsp, dtmp0, dtmp1, dtmp2,
	    dyf,
	    dxf0, dxf1, dxf2,
	    r00, r10, r20, r30, r40, r50, r60, r70, r80,
	    r01, r11, r21, r31, r41, r51, r61, r71, r81,
	    r02, r12, r22, r32, r42, r52, r62, r72,
	    r71_1, r71_2, r81_1, A0, A1, B0, B1, C0, C1, D0, D1, res;

	sp -= src_stride + 3;
	tsp = sp;

	vis_write_gsr(3 << 3);

	for (j = 0; j < height; j++) {

		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		COEFF_D64_U8_1(y);
		tdp = dp;

#pragma pipeloop(0)
		for (i = 0; i < width; i++, tdp += 3) {
			COEFF_D64_U8_3(x);
			LOAD_U8_3_BC(A0, A1, B0, B1, C0, C1, D0, D1);
			FMUL_U8_3_BC(r);
			FADD_U8_3_BC(r);
			RES_U8_3_BC(r, res);
			f0.f = vis_fpack16(res);

			tdp[0] = f0.t[0];
			tdp[1] = f0.t[1];
			tdp[2] = f0.t[2];
		}

		y += dy;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		dp += dst_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUNC_NAME(4s) (
    mlib_work_image *param)
{
	VARIABLE(mlib_u8);
	mlib_u8 *filter1 = GetElemStruct(filter1);
	mlib_u8 *filter4 = GetElemStruct(filter4);
	mlib_u8 *tsp;
	mlib_s32 fpos;
	mlib_d64 *dfPtr,
	    dyf,
	    dxf0, dxf1, dxf2, dxf3,
	    r00, r10, r20, r30, r40, r50, r60,
	    r01, r11, r21, r31, r41, r51, r61,
	    r02, r12, r22, r32, r42, r52, r62,
	    r03, r13, r23, r33, r43, r53, r63,
	    r70_0, r71_0, r72_0, r73_0,
	    r80_0, r81_0, A0, A1, B0, B1, C0, C1, D0, D1, res0;
	mlib_d64 *dsp, dtmp0, dtmp1, dtmp2;
	mlib_f32 fsum, *buf, *swpBUFFER;

	sp -= src_stride + 4;
	tsp = sp;

	vis_write_gsr(3 << 3);

	if (((mlib_addr)dp | dst_stride) & 3) {
		swpBUFFER =
		    (mlib_f32 *)__mlib_malloc(width * sizeof (mlib_f32));

		if (swpBUFFER == NULL)
			return (MLIB_FAILURE);
	} else {
		swpBUFFER = (mlib_f32 *)dp;
	}

	for (j = 0; j < height; j++) {

		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		fpos = (y >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
		dyf = *(mlib_d64 *)((mlib_u8 *)FILTER_1 + fpos);

		buf = (mlib_f32 *)swpBUFFER;

#pragma pipeloop(0)
		for (i = 0; i < width; i++) {
			cx = (x >> MLIB_SHIFT) * 4;
			fpos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
			dfPtr = (mlib_d64 *)((mlib_u8 *)FILTER_4 + fpos * 4);
			dxf0 = dfPtr[0];
			dxf1 = dfPtr[1];
			dxf2 = dfPtr[2];
			dxf3 = dfPtr[3];
			x += dx;

			dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
			dtmp0 = dsp[0];
			dtmp1 = dsp[1];
			dtmp2 = LD_D64(dsp, 2);
			A0 = vis_faligndata(dtmp0, dtmp1);
			A1 = vis_faligndata(dtmp1, dtmp2);
			cx += src_stride;
			dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
			dtmp0 = dsp[0];
			dtmp1 = dsp[1];
			dtmp2 = LD_D64(dsp, 2);
			B0 = vis_faligndata(dtmp0, dtmp1);
			B1 = vis_faligndata(dtmp1, dtmp2);
			cx += src_stride;
			dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
			dtmp0 = dsp[0];
			dtmp1 = dsp[1];
			dtmp2 = LD_D64(dsp, 2);
			C0 = vis_faligndata(dtmp0, dtmp1);
			C1 = vis_faligndata(dtmp1, dtmp2);
			cx += src_stride;
			dsp = (mlib_d64 *)vis_alignaddr(tsp, cx);
			dtmp0 = dsp[0];
			dtmp1 = dsp[1];
			dtmp2 = LD_D64(dsp, 2);
			D0 = vis_faligndata(dtmp0, dtmp1);
			D1 = vis_faligndata(dtmp1, dtmp2);

			r00 = vis_fmul8x16au(vis_read_hi(A0), vis_read_hi(dyf));
			r10 = vis_fmul8x16al(vis_read_hi(B0), vis_read_hi(dyf));
			r40 = vis_fpadd16(r00, r10);
			r02 = vis_fmul8x16au(vis_read_lo(A0), vis_read_hi(dyf));
			r12 = vis_fmul8x16al(vis_read_lo(B0), vis_read_hi(dyf));
			r42 = vis_fpadd16(r02, r12);
			r01 = vis_fmul8x16au(vis_read_hi(A1), vis_read_hi(dyf));
			r11 = vis_fmul8x16al(vis_read_hi(B1), vis_read_hi(dyf));
			r41 = vis_fpadd16(r01, r11);
			r03 = vis_fmul8x16au(vis_read_lo(A1), vis_read_hi(dyf));
			r13 = vis_fmul8x16al(vis_read_lo(B1), vis_read_hi(dyf));
			r43 = vis_fpadd16(r03, r13);
			r20 = vis_fmul8x16au(vis_read_hi(C0), vis_read_lo(dyf));
			r30 = vis_fmul8x16al(vis_read_hi(D0), vis_read_lo(dyf));
			r50 = vis_fpadd16(r20, r30);
			r60 = vis_fpadd16(r40, r50);
			r22 = vis_fmul8x16au(vis_read_lo(C0), vis_read_lo(dyf));
			r32 = vis_fmul8x16al(vis_read_lo(D0), vis_read_lo(dyf));
			r52 = vis_fpadd16(r22, r32);
			r62 = vis_fpadd16(r42, r52);
			r21 = vis_fmul8x16au(vis_read_hi(C1), vis_read_lo(dyf));
			r31 = vis_fmul8x16al(vis_read_hi(D1), vis_read_lo(dyf));
			r51 = vis_fpadd16(r21, r31);
			r61 = vis_fpadd16(r41, r51);
			r23 = vis_fmul8x16au(vis_read_lo(C1), vis_read_lo(dyf));
			r33 = vis_fmul8x16al(vis_read_lo(D1), vis_read_lo(dyf));
			r53 = vis_fpadd16(r23, r33);
			r63 = vis_fpadd16(r43, r53);
			r70_0 = FPMUL16(r60, dxf0);
			r71_0 = FPMUL16(r61, dxf2);
			r72_0 = FPMUL16(r62, dxf1);
			r73_0 = FPMUL16(r63, dxf3);
			r80_0 = vis_fpadd16(r70_0, r71_0);
			r81_0 = vis_fpadd16(r72_0, r73_0);
			res0 = vis_fpadd16(r80_0, r81_0);
			fsum = vis_fpack16(res0);

			(*buf++) = fsum;
		}

		y += dy;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		dp += dst_stride;

		if (((mlib_addr)dp | dst_stride) & 3) {
			mlib_ImageCopy_na((mlib_u8 *)swpBUFFER, dp - dst_stride,
			    width * 4);
		} else {
			swpBUFFER = (mlib_f32 *)dp;
		}
	}

	if (((mlib_addr)dp | dst_stride) & 3) {
		__mlib_free(swpBUFFER);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
