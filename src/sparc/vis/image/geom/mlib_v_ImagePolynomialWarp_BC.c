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

#pragma ident	"@(#)mlib_v_ImagePolynomialWarp_BC.c	9.2	07/11/05 SMI"

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImagePolynomialWarp.h>
#include <mlib_v_ImageFilters.h>

#define	VIS_ALIGNADDR(X, Y)	vis_alignaddr((void *)(X), (Y))

/* *********************************************************** */

#define	FPMUL16(x0, x1)                                               \
	vis_fpadd16(vis_fmul8sux16(x0, x1), vis_fmul8ulx16(x0, x1))

/* *********************************************************** */

#define	FPMULU16(x0, x1)                                          \
	vis_fpadd16(vis_fmul8sux16(vis_fxor(x0, vis_mone), x1),   \
	    vis_fmul8ulx16(vis_fxor(x0, vis_mone), x1))

/* *********************************************************** */

#define	FILTER_SHIFT	(5 - 1)
/* (((1 << 8) - 1) << 3) */
#define	FILTER_MASK	2040

/* *********************************************************** */

#define	LOAD_U8_1                                               \
	line = lineAddr[ysrc] + xsrc;                           \
	dline = VIS_ALIGNADDR(line, -stride - 1);               \
	a0 = vis_faligndata(dline[0], dline[1]);                \
	dline = VIS_ALIGNADDR(line, -1);                        \
	a1 = vis_faligndata(dline[0], dline[1]);                \
	dline = VIS_ALIGNADDR(line, stride - 1);                \
	a2 = vis_faligndata(dline[0], dline[1]);                \
	dline = VIS_ALIGNADDR(line, 2 * stride - 1);            \
	a3 = vis_faligndata(dline[0], dline[1])

/* *********************************************************** */

#define	LOAD_U8_2                                               \
	line = lineAddr[ysrc] + xsrc;                           \
	dline = VIS_ALIGNADDR(line, -stride - 2);               \
	a0 = vis_faligndata(dline[0], dline[1]);                \
	dline = VIS_ALIGNADDR(line, -2);                        \
	a1 = vis_faligndata(dline[0], dline[1]);                \
	dline = VIS_ALIGNADDR(line, 1 * stride - 2);            \
	a2 = vis_faligndata(dline[0], dline[1]);                \
	dline = VIS_ALIGNADDR(line, 2 * stride - 2);            \
	a3 = vis_faligndata(dline[0], dline[1])

/* *********************************************************** */

#define	LOAD_U8_3                                               \
	line = lineAddr[ysrc] + xsrc;                           \
	dline = VIS_ALIGNADDR(line, -stride - 3);               \
	a00 = vis_faligndata(dline[0], dline[1]);               \
	a01 = vis_faligndata(dline[1], dline[2]);               \
	dline = VIS_ALIGNADDR(line, -3);                        \
	a10 = vis_faligndata(dline[0], dline[1]);               \
	a11 = vis_faligndata(dline[1], dline[2]);               \
	dline = VIS_ALIGNADDR(line, 1 * stride - 3);            \
	a20 = vis_faligndata(dline[0], dline[1]);               \
	a21 = vis_faligndata(dline[1], dline[2]);               \
	dline = VIS_ALIGNADDR(line, 2 * stride - 3);            \
	a30 = vis_faligndata(dline[0], dline[1]);               \
	a31 = vis_faligndata(dline[1], dline[2])

/* *********************************************************** */

#define	LOAD_U8_4                                               \
	line = lineAddr[ysrc] + xsrc;                           \
	dline = VIS_ALIGNADDR(line, -stride - 4);               \
	a00 = vis_faligndata(dline[0], dline[1]);               \
	a01 = vis_faligndata(dline[1], dline[2]);               \
	dline = VIS_ALIGNADDR(line, -4);                        \
	a10 = vis_faligndata(dline[0], dline[1]);               \
	a11 = vis_faligndata(dline[1], dline[2]);               \
	dline = VIS_ALIGNADDR(line, 1 * stride - 4);            \
	a20 = vis_faligndata(dline[0], dline[1]);               \
	a21 = vis_faligndata(dline[1], dline[2]);               \
	dline = VIS_ALIGNADDR(line, 2 * stride - 4);            \
	a30 = vis_faligndata(dline[0], dline[1]);               \
	a31 = vis_faligndata(dline[1], dline[2])

/* *********************************************************** */

#define	LOAD_S16_3                                              \
	line = lineAddr[ysrc] + xsrc;                           \
	dline = VIS_ALIGNADDR(line, -stride - 6);               \
	a00 = vis_faligndata(dline[0], dline[1]);               \
	a01 = vis_faligndata(dline[1], dline[2]);               \
	a02 = vis_faligndata(dline[2], dline[3]);               \
	dline = VIS_ALIGNADDR(line, -6);                        \
	a10 = vis_faligndata(dline[0], dline[1]);               \
	a11 = vis_faligndata(dline[1], dline[2]);               \
	a12 = vis_faligndata(dline[2], dline[3]);               \
	dline = VIS_ALIGNADDR(line, 1 * stride - 6);            \
	a20 = vis_faligndata(dline[0], dline[1]);               \
	a21 = vis_faligndata(dline[1], dline[2]);               \
	a22 = vis_faligndata(dline[2], dline[3]);               \
	dline = VIS_ALIGNADDR(line, 2 * stride - 6);            \
	a30 = vis_faligndata(dline[0], dline[1]);               \
	a31 = vis_faligndata(dline[1], dline[2]);               \
	a32 = vis_faligndata(dline[2], dline[3])

/* *********************************************************** */

#define	LOAD_S16_4                                              \
	line = lineAddr[ysrc] + xsrc;                           \
	dline = VIS_ALIGNADDR(line, -stride - 8);               \
	a00 = vis_faligndata(dline[0], dline[1]);               \
	a01 = vis_faligndata(dline[1], dline[2]);               \
	a02 = vis_faligndata(dline[2], dline[3]);               \
	a03 = vis_faligndata(dline[3], dline[4]);               \
	dline = VIS_ALIGNADDR(line, -8);                        \
	a10 = vis_faligndata(dline[0], dline[1]);               \
	a11 = vis_faligndata(dline[1], dline[2]);               \
	a12 = vis_faligndata(dline[2], dline[3]);               \
	a13 = vis_faligndata(dline[3], dline[4]);               \
	dline = VIS_ALIGNADDR(line, 1 * stride - 8);            \
	a20 = vis_faligndata(dline[0], dline[1]);               \
	a21 = vis_faligndata(dline[1], dline[2]);               \
	a22 = vis_faligndata(dline[2], dline[3]);               \
	a23 = vis_faligndata(dline[3], dline[4]);               \
	dline = VIS_ALIGNADDR(line, 2 * stride - 8);            \
	a30 = vis_faligndata(dline[0], dline[1]);               \
	a31 = vis_faligndata(dline[1], dline[2]);               \
	a32 = vis_faligndata(dline[2], dline[3]);               \
	a33 = vis_faligndata(dline[3], dline[4])

/* *********************************************************** */

#define	LOAD_COEFF                                                   \
	posx = (((*dxs++)) >> FILTER_SHIFT) & FILTER_MASK;           \
	xfilter = *((mlib_d64 *)((mlib_u8 *)filter_table + posx));   \
	posy = (((*dys++)) >> FILTER_SHIFT) & FILTER_MASK;           \
	yfilter = *((mlib_d64 *)((mlib_u8 *)filter_table + posy))

/* *********************************************************** */

#define	DOUBLING_SHORT(src, dst0, dst1)                           \
	r10 = vis_fpmerge(vis_read_hi(src), vis_read_lo(src));    \
	r10 = vis_fpmerge(vis_read_hi(r10), vis_read_lo(r10));    \
	r11 = vis_fpmerge(vis_read_lo(r10), vis_read_lo(r10));    \
	r10 = vis_fpmerge(vis_read_hi(r10), vis_read_hi(r10));    \
	dst0 = vis_fpmerge(vis_read_hi(r10), vis_read_hi(r11));   \
	dst1 = vis_fpmerge(vis_read_lo(r10), vis_read_lo(r11))

/* *********************************************************** */

#define	EXPAND_U8_3(opr, idx, scale)                            \
	r##idx##0 = opr##(vis_read_hi(a##idx##0), scale);       \
	r##idx##1 = opr##(vis_read_lo(a##idx##0), scale);       \
	r##idx##2 = opr##(vis_read_hi(a##idx##1), scale)

/* *********************************************************** */

#define	EXPAND_U8_4(opr, idx, scale)                            \
	r##idx##0 = opr##(vis_read_hi(a##idx##0), scale);       \
	r##idx##1 = opr##(vis_read_lo(a##idx##0), scale);       \
	r##idx##2 = opr##(vis_read_hi(a##idx##1), scale);       \
	r##idx##3 = opr##(vis_read_lo(a##idx##1), scale)

/* *********************************************************** */

#define	LOAD_COEFF_3                                                   \
	posx = (((*dxs++)) >> FILTER_SHIFT) & FILTER_MASK;             \
	xptr = (mlib_d64 *)((mlib_u8 *)filter_table_vis + 3 * posx);   \
	xfilter0 = xptr[0];                                            \
	xfilter1 = xptr[1];                                            \
	xfilter2 = xptr[2];                                            \
	posy = (((*dys++)) >> FILTER_SHIFT) & FILTER_MASK;             \
	yfilter = *((mlib_d64 *)((mlib_u8 *)filter_table + posy))

/* *********************************************************** */

#define	LOAD_COEFF_4                                                   \
	posx = (((*dxs++)) >> FILTER_SHIFT) & FILTER_MASK;             \
	xptr = (mlib_d64 *)((mlib_u8 *)filter_table_vis + 4 * posx);   \
	xfilter0 = xptr[0];                                            \
	xfilter1 = xptr[1];                                            \
	xfilter2 = xptr[2];                                            \
	xfilter3 = xptr[3];                                            \
	posy = (((*dys++)) >> FILTER_SHIFT) & FILTER_MASK;             \
	yfilter = *((mlib_d64 *)((mlib_u8 *)filter_table + posy))

/* *********************************************************** */

#define	ADD_3(idx0, idx1, idx2)                                 \
	r##idx0##0 = vis_fpadd16(r##idx1##0, r##idx2##0);       \
	r##idx0##1 = vis_fpadd16(r##idx1##1, r##idx2##1);       \
	r##idx0##2 = vis_fpadd16(r##idx1##2, r##idx2##2)

/* *********************************************************** */

#define	ADD_4(idx0, idx1, idx2)                                 \
	r##idx0##0 = vis_fpadd16(r##idx1##0, r##idx2##0);       \
	r##idx0##1 = vis_fpadd16(r##idx1##1, r##idx2##1);       \
	r##idx0##2 = vis_fpadd16(r##idx1##2, r##idx2##2);       \
	r##idx0##3 = vis_fpadd16(r##idx1##3, r##idx2##3)

/* *********************************************************** */

#define	MULU_3(idx, scale)                                      \
	r##idx##0 = FPMULU16(a##idx##0, scale);                 \
	r##idx##1 = FPMULU16(a##idx##1, scale);                 \
	r##idx##2 = FPMULU16(a##idx##2, scale)

/* *********************************************************** */

#define	MUL_3(idx, scale)                                       \
	r##idx##0 = FPMUL16(a##idx##0, scale);                  \
	r##idx##1 = FPMUL16(a##idx##1, scale);                  \
	r##idx##2 = FPMUL16(a##idx##2, scale)

/* *********************************************************** */

#define	MUL_4(idx, scale)                                       \
	r##idx##0 = FPMUL16(a##idx##0, scale);                  \
	r##idx##1 = FPMUL16(a##idx##1, scale);                  \
	r##idx##2 = FPMUL16(a##idx##2, scale);                  \
	r##idx##3 = FPMUL16(a##idx##3, scale)

/* *********************************************************** */

#define	MULU_4(idx, scale)                                      \
	r##idx##0 = FPMULU16(a##idx##0, scale);                 \
	r##idx##1 = FPMULU16(a##idx##1, scale);                 \
	r##idx##2 = FPMULU16(a##idx##2, scale);                 \
	r##idx##3 = FPMULU16(a##idx##3, scale)

/* *********************************************************** */

#define	LOAD_COEFF_S16_1                                               \
	posy = (((*dys++)) >> FILTER_SHIFT) & FILTER_MASK;             \
	yptr = (mlib_d64 *)((mlib_u8 *)filter_table_vis + 4 * posy);   \
	yfilter0 = yptr[0];                                            \
	yfilter1 = yptr[1];                                            \
	yfilter2 = yptr[2];                                            \
	yfilter3 = yptr[3];                                            \
	posx = (((*dxs++)) >> FILTER_SHIFT) & FILTER_MASK;             \
	xfilter = *((mlib_d64 *)((mlib_u8 *)filter_table + posx))

/* *********************************************************** */

#define	LOAD_COEFF_S16_3                                               \
	posy = (((*dys++)) >> FILTER_SHIFT) & FILTER_MASK;             \
	yptr = (mlib_d64 *)((mlib_u8 *)filter_table_vis + 4 * posy);   \
	yfilter0 = yptr[0];                                            \
	yfilter1 = yptr[1];                                            \
	yfilter2 = yptr[2];                                            \
	yfilter3 = yptr[3];                                            \
	posx = (((*dxs++)) >> FILTER_SHIFT) & FILTER_MASK;             \
	xptr = (mlib_d64 *)((mlib_u8 *)filter_table + 3 * posx);       \
	xfilter0 = xptr[0];                                            \
	xfilter1 = xptr[1];                                            \
	xfilter2 = xptr[2]

/* *********************************************************** */

#define	LOAD_COEFF_S16_4                                           \
	posy = (((*dys++)) >> FILTER_SHIFT) & FILTER_MASK;         \
	yptr = (mlib_d64 *)((mlib_u8 *)filter_table + 4 * posy);   \
	yfilter0 = yptr[0];                                        \
	yfilter1 = yptr[1];                                        \
	yfilter2 = yptr[2];                                        \
	yfilter3 = yptr[3];                                        \
	posx = (((*dxs++)) >> FILTER_SHIFT) & FILTER_MASK;         \
	xptr = (mlib_d64 *)((mlib_u8 *)filter_table + 4 * posx);   \
	xfilter0 = xptr[0];                                        \
	xfilter1 = xptr[1];                                        \
	xfilter2 = xptr[2];                                        \
	xfilter3 = xptr[3]

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_U8_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	void *filter_table = pws->filter_table;
	const mlib_u8 *line;
	mlib_s32 i;
	mlib_s32 xsrc, ysrc;
	mlib_s32 posx, posy;
	mlib_s32 stride = lineAddr[1] - lineAddr[0];
	mlib_s32 *xd = pws->dst_x;
	mlib_s32 *xs = pws->src_x;
	mlib_s32 *ys = pws->src_y;
	mlib_u16 *dxs = pws->dsrc_x;
	mlib_u16 *dys = pws->dsrc_y;
	mlib_d64 *dline;
	mlib_d64 xfilter, yfilter;
	mlib_d64 a0, a1, a2, a3;
	mlib_d64 r0, r1, r2, r3;
	mlib_d64 r4, r5, r6, r7;
	mlib_d64 r8, r9, r10;
	mlib_f32 fr0, fr1;
	mlib_f32 vis_scale = vis_to_float(0x00010001);

	if (n <= 0)
		return;

	vis_write_gsr(3 << 3);

	xsrc = (*xs++);
	ysrc = (*ys++);

	LOAD_U8_1;
	LOAD_COEFF;

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < n - 1; i++) {

		r0 = vis_fmul8x16au(vis_read_hi(a0), vis_read_hi(yfilter));
		r1 = vis_fmul8x16al(vis_read_hi(a1), vis_read_hi(yfilter));
		r2 = vis_fmul8x16au(vis_read_hi(a2), vis_read_lo(yfilter));
		r3 = vis_fmul8x16al(vis_read_hi(a3), vis_read_lo(yfilter));
		r4 = vis_fpadd16(r0, r1);
		r5 = vis_fpadd16(r2, r3);
		r6 = vis_fpadd16(r4, r5);
		r7 = FPMUL16(r6, xfilter);
		fr0 = vis_fpadd16s(vis_read_hi(r7), vis_read_lo(r7));
		r8 = vis_fmuld8ulx16(vis_scale, fr0);
		fr1 = vis_fpadd32s(vis_read_hi(r8), vis_read_lo(r8));
		r9 = vis_write_lo(r9, fr1);

		xsrc = (*xs++);
		ysrc = (*ys++);

		LOAD_U8_1;
		LOAD_COEFF;

		r10 = vis_fpack16_pair(r9, r9);
		vis_st_u8(r10, dstData + xd[i]);
	}

	r0 = vis_fmul8x16au(vis_read_hi(a0), vis_read_hi(yfilter));
	r1 = vis_fmul8x16al(vis_read_hi(a1), vis_read_hi(yfilter));
	r2 = vis_fmul8x16au(vis_read_hi(a2), vis_read_lo(yfilter));
	r3 = vis_fmul8x16al(vis_read_hi(a3), vis_read_lo(yfilter));
	r4 = vis_fpadd16(r0, r1);
	r5 = vis_fpadd16(r2, r3);
	r6 = vis_fpadd16(r4, r5);
	r7 = FPMUL16(r6, xfilter);
	fr0 = vis_fpadd16s(vis_read_hi(r7), vis_read_lo(r7));
	r8 = vis_fmuld8ulx16(vis_scale, fr0);
	fr1 = vis_fpadd32s(vis_read_hi(r8), vis_read_lo(r8));
	r9 = vis_write_lo(r9, fr1);
	r10 = vis_fpack16_pair(r9, r9);
	vis_st_u8(r10, dstData + xd[i]);
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_U8_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	void *filter_table = pws->filter_table;
	const mlib_u8 *line;
	mlib_s32 i;
	mlib_s32 xsrc, ysrc;
	mlib_s32 posx, posy;
	mlib_s32 stride = lineAddr[1] - lineAddr[0];
	mlib_s32 *xd = pws->dst_x;
	mlib_s32 *xs = pws->src_x;
	mlib_s32 *ys = pws->src_y;
	mlib_u16 *dxs = pws->dsrc_x;
	mlib_u16 *dys = pws->dsrc_y;
	mlib_d64 *dline;
	mlib_d64 xfilter, xfilter0, xfilter1;
	mlib_d64 yfilter;
	mlib_d64 a0, a1, a2, a3;
	mlib_d64 r0, r1, r2, r3;
	mlib_d64 r4, r5, r6, r7;
	mlib_d64 r8, r9, r10, r11;
	mlib_d64 r12, r13, r14, r15;
	mlib_d64 r16, r17, r18, r19, r20;

	mlib_f32 vis_scale = vis_to_float(0x00010001);

	if (n <= 0)
		return;

	vis_write_gsr(3 << 3);

	xsrc = ((*xs++)) * 2;
	ysrc = (*ys++);

	LOAD_U8_2;
	LOAD_COEFF;

	DOUBLING_SHORT(xfilter, xfilter0, xfilter1);

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < n - 1; i++) {

		r0 = vis_fmul8x16au(vis_read_hi(a0), vis_read_hi(yfilter));
		r1 = vis_fmul8x16au(vis_read_lo(a0), vis_read_hi(yfilter));
		r2 = vis_fmul8x16al(vis_read_hi(a1), vis_read_hi(yfilter));
		r3 = vis_fmul8x16al(vis_read_lo(a1), vis_read_hi(yfilter));
		r4 = vis_fmul8x16au(vis_read_hi(a2), vis_read_lo(yfilter));
		r5 = vis_fmul8x16au(vis_read_lo(a2), vis_read_lo(yfilter));
		r6 = vis_fmul8x16al(vis_read_hi(a3), vis_read_lo(yfilter));
		r7 = vis_fmul8x16al(vis_read_lo(a3), vis_read_lo(yfilter));

		r8 = vis_fpadd16(r0, r2);
		r9 = vis_fpadd16(r1, r3);
		r10 = vis_fpadd16(r4, r6);
		r11 = vis_fpadd16(r5, r7);
		r12 = vis_fpadd16(r8, r10);
		r13 = vis_fpadd16(r9, r11);

		r14 = FPMUL16(r12, xfilter0);
		r15 = FPMUL16(r13, xfilter1);

		r16 = vis_fpadd16(r14, r15);
		r17 = vis_fmuld8ulx16(vis_scale, vis_read_hi(r16));
		r18 = vis_fmuld8ulx16(vis_scale, vis_read_lo(r16));
		r19 = vis_fpadd32(r17, r18);
		r20 = vis_fpack16_pair(r19, r19);

		xsrc = ((*xs++)) * 2;
		ysrc = (*ys++);

		LOAD_U8_2;
		LOAD_COEFF;

		DOUBLING_SHORT(xfilter, xfilter0, xfilter1);

		VIS_ALIGNADDR(dline, 6);
		vis_st_u8(r20, dstData + xd[i] * 2 + 1);
		r20 = vis_faligndata(r20, r20);
		vis_st_u8(r20, dstData + xd[i] * 2);
	}

	r0 = vis_fmul8x16au(vis_read_hi(a0), vis_read_hi(yfilter));
	r1 = vis_fmul8x16au(vis_read_lo(a0), vis_read_hi(yfilter));
	r2 = vis_fmul8x16al(vis_read_hi(a1), vis_read_hi(yfilter));
	r3 = vis_fmul8x16al(vis_read_lo(a1), vis_read_hi(yfilter));
	r4 = vis_fmul8x16au(vis_read_hi(a2), vis_read_lo(yfilter));
	r5 = vis_fmul8x16au(vis_read_lo(a2), vis_read_lo(yfilter));
	r6 = vis_fmul8x16al(vis_read_hi(a3), vis_read_lo(yfilter));
	r7 = vis_fmul8x16al(vis_read_lo(a3), vis_read_lo(yfilter));

	r8 = vis_fpadd16(r0, r2);
	r9 = vis_fpadd16(r1, r3);
	r10 = vis_fpadd16(r4, r6);
	r11 = vis_fpadd16(r5, r7);
	r12 = vis_fpadd16(r8, r10);
	r13 = vis_fpadd16(r9, r11);

	r14 = FPMUL16(r12, xfilter0);
	r15 = FPMUL16(r13, xfilter1);

	r16 = vis_fpadd16(r14, r15);
	r17 = vis_fmuld8ulx16(vis_scale, vis_read_hi(r16));
	r18 = vis_fmuld8ulx16(vis_scale, vis_read_lo(r16));
	r19 = vis_fpadd32(r17, r18);
	r20 = vis_fpack16_pair(r19, r19);
	VIS_ALIGNADDR(dline, 6);
	vis_st_u8(r20, dstData + xd[i] * 2 + 1);
	r20 = vis_faligndata(r20, r20);
	vis_st_u8(r20, dstData + xd[i] * 2);
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_U8_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	void *filter_table = pws->filter_table;
	void *filter_table_vis = pws->filter_table_vis;
	const mlib_u8 *line;
	mlib_s32 i;
	mlib_s32 xsrc, ysrc;
	mlib_s32 posx, posy;
	mlib_s32 stride = lineAddr[1] - lineAddr[0];
	mlib_s32 *xd = pws->dst_x;
	mlib_s32 *xs = pws->src_x;
	mlib_s32 *ys = pws->src_y;
	mlib_u16 *dxs = pws->dsrc_x;
	mlib_u16 *dys = pws->dsrc_y;
	mlib_d64 *dline, *xptr;
	mlib_d64 xfilter0, xfilter1;
	mlib_d64 xfilter2;
	mlib_d64 yfilter;
	mlib_d64 a00, a01;
	mlib_d64 a10, a11;
	mlib_d64 a20, a21;
	mlib_d64 a30, a31;
	mlib_d64 r00, r01, r02;
	mlib_d64 r10, r11, r12;
	mlib_d64 r20, r21, r22;
	mlib_d64 r30, r31, r32;
	mlib_d64 r40, r41, r42;
	mlib_d64 r50, r51, r52;
	mlib_d64 r60, r61, r62;
	mlib_d64 r70, r71, r72;
	mlib_d64 r80, r81, r82;
	mlib_d64 r90, r91;
	mlib_d64 r100, r101, r102, r103;
	mlib_d64 r110, r111;
	mlib_d64 r120;
	mlib_f32 vis_scale = vis_to_float(0x00010001);

	if (n <= 0)
		return;

	vis_write_gsr(3 << 3);

	xsrc = ((*xs++)) * 3;
	ysrc = (*ys++);

	LOAD_U8_3;
	LOAD_COEFF_3;

	EXPAND_U8_3(vis_fmul8x16au, 0, vis_read_hi(yfilter));
	EXPAND_U8_3(vis_fmul8x16al, 1, vis_read_hi(yfilter));
	EXPAND_U8_3(vis_fmul8x16au, 2, vis_read_lo(yfilter));
	EXPAND_U8_3(vis_fmul8x16al, 3, vis_read_lo(yfilter));

	ADD_3(4, 0, 1);
	ADD_3(5, 2, 3);
	ADD_3(6, 4, 5);

	r70 = FPMUL16(r60, xfilter0);
	r71 = FPMUL16(r61, xfilter1);
	r72 = FPMUL16(r62, xfilter2);

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < n - 1; i++) {

		VIS_ALIGNADDR(dline, 6);
		r80 = vis_faligndata(r70, r71);
		VIS_ALIGNADDR(dline, 4);
		r81 = vis_faligndata(r71, r72);
		VIS_ALIGNADDR(dline, 2);
		r82 = vis_faligndata(r72, r72);

		r90 = vis_fpadd16(r70, r80);
		r91 = vis_fpadd16(r81, r82);

		r100 = vis_fmuld8ulx16(vis_scale, vis_read_hi(r90));
		r101 = vis_fmuld8ulx16(vis_scale, vis_read_lo(r90));
		r102 = vis_fmuld8ulx16(vis_scale, vis_read_hi(r91));
		r103 = vis_fmuld8ulx16(vis_scale, vis_read_lo(r91));

		r110 = vis_fpadd32(r100, r102);
		r111 = vis_fpadd32(r101, r103);
		r120 = vis_fpack16_pair(r110, r111);

		xsrc = ((*xs++)) * 3;
		ysrc = (*ys++);

		LOAD_U8_3;
		LOAD_COEFF_3;
		VIS_ALIGNADDR(dline, 2);

		EXPAND_U8_3(vis_fmul8x16au, 0, vis_read_hi(yfilter));
		EXPAND_U8_3(vis_fmul8x16al, 1, vis_read_hi(yfilter));
		EXPAND_U8_3(vis_fmul8x16au, 2, vis_read_lo(yfilter));
		EXPAND_U8_3(vis_fmul8x16al, 3, vis_read_lo(yfilter));

		ADD_3(4, 0, 1);
		ADD_3(5, 2, 3);
		ADD_3(6, 4, 5);

		r70 = FPMUL16(r60, xfilter0);
		r71 = FPMUL16(r61, xfilter1);
		r72 = FPMUL16(r62, xfilter2);

		r120 = vis_faligndata(r120, r120);
		vis_st_u8(r120, dstData + xd[i] * 3 + 0);
		r120 = vis_faligndata(r120, r120);
		vis_st_u8(r120, dstData + xd[i] * 3 + 1);
		r120 = vis_faligndata(r120, r120);
		vis_st_u8(r120, dstData + xd[i] * 3 + 2);
	}

	VIS_ALIGNADDR(dline, 6);
	r80 = vis_faligndata(r70, r71);
	VIS_ALIGNADDR(dline, 4);
	r81 = vis_faligndata(r71, r72);
	VIS_ALIGNADDR(dline, 2);
	r82 = vis_faligndata(r72, r72);

	r90 = vis_fpadd16(r70, r80);
	r91 = vis_fpadd16(r81, r82);

	r100 = vis_fmuld8ulx16(vis_scale, vis_read_hi(r90));
	r101 = vis_fmuld8ulx16(vis_scale, vis_read_lo(r90));
	r102 = vis_fmuld8ulx16(vis_scale, vis_read_hi(r91));
	r103 = vis_fmuld8ulx16(vis_scale, vis_read_lo(r91));

	r110 = vis_fpadd32(r100, r102);
	r111 = vis_fpadd32(r101, r103);
	r120 = vis_fpack16_pair(r110, r111);
	r120 = vis_faligndata(r120, r120);
	vis_st_u8(r120, dstData + xd[i] * 3 + 0);
	r120 = vis_faligndata(r120, r120);
	vis_st_u8(r120, dstData + xd[i] * 3 + 1);
	r120 = vis_faligndata(r120, r120);
	vis_st_u8(r120, dstData + xd[i] * 3 + 2);
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_U8_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	void *filter_table = pws->filter_table;
	void *filter_table_vis = pws->filter_table_vis;
	const mlib_u8 *line;
	mlib_s32 i;
	mlib_s32 xsrc, ysrc;
	mlib_s32 posx, posy;
	mlib_s32 stride = lineAddr[1] - lineAddr[0];
	mlib_s32 *xd = pws->dst_x;
	mlib_s32 *xs = pws->src_x;
	mlib_s32 *ys = pws->src_y;
	mlib_u16 *dxs = pws->dsrc_x;
	mlib_u16 *dys = pws->dsrc_y;
	mlib_d64 *dline, *xptr;
	mlib_d64 xfilter0, xfilter1;
	mlib_d64 xfilter2, xfilter3;
	mlib_d64 yfilter;
	mlib_d64 a00, a01;
	mlib_d64 a10, a11;
	mlib_d64 a20, a21;
	mlib_d64 a30, a31;
	mlib_d64 r00, r01, r02, r03;
	mlib_d64 r10, r11, r12, r13;
	mlib_d64 r20, r21, r22, r23;
	mlib_d64 r30, r31, r32, r33;
	mlib_d64 r40, r41, r42, r43;
	mlib_d64 r50, r51, r52, r53;
	mlib_d64 r60, r61, r62, r63;
	mlib_d64 r70, r71, r72, r73;
	mlib_d64 r80, r81;
	mlib_d64 r90, r91, r92, r93;
	mlib_d64 r100, r101;
	mlib_d64 r110;
	mlib_f32 vis_scale = vis_to_float(0x00010001);

	if (n <= 0)
		return;

	vis_write_gsr(3 << 3);

	xsrc = ((*xs++)) * 4;
	ysrc = (*ys++);

	LOAD_U8_4;
	LOAD_COEFF_4;

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < n - 1; i++) {

		EXPAND_U8_4(vis_fmul8x16au, 0, vis_read_hi(yfilter));
		EXPAND_U8_4(vis_fmul8x16al, 1, vis_read_hi(yfilter));
		EXPAND_U8_4(vis_fmul8x16au, 2, vis_read_lo(yfilter));
		EXPAND_U8_4(vis_fmul8x16al, 3, vis_read_lo(yfilter));

		ADD_4(4, 0, 1);
		ADD_4(5, 2, 3);
		ADD_4(6, 4, 5);

		r70 = FPMUL16(r60, xfilter0);
		r71 = FPMUL16(r61, xfilter1);
		r72 = FPMUL16(r62, xfilter2);
		r73 = FPMUL16(r63, xfilter3);

		r80 = vis_fpadd16(r70, r71);
		r81 = vis_fpadd16(r72, r73);

		r90 = vis_fmuld8ulx16(vis_scale, vis_read_hi(r80));
		r91 = vis_fmuld8ulx16(vis_scale, vis_read_lo(r80));
		r92 = vis_fmuld8ulx16(vis_scale, vis_read_hi(r81));
		r93 = vis_fmuld8ulx16(vis_scale, vis_read_lo(r81));

		r100 = vis_fpadd32(r90, r92);
		r101 = vis_fpadd32(r91, r93);

		xsrc = ((*xs++)) * 4;
		ysrc = (*ys++);

		LOAD_U8_4;
		LOAD_COEFF_4;

		r110 = vis_fpack16_pair(r100, r101);
		VIS_ALIGNADDR(dline, 6);
		vis_st_u8(r110, dstData + xd[i] * 4 + 3);
		r110 = vis_faligndata(r110, r110);
		vis_st_u8(r110, dstData + xd[i] * 4 + 2);
		r110 = vis_faligndata(r110, r110);
		vis_st_u8(r110, dstData + xd[i] * 4 + 1);
		r110 = vis_faligndata(r110, r110);
		vis_st_u8(r110, dstData + xd[i] * 4 + 0);
	}

	EXPAND_U8_4(vis_fmul8x16au, 0, vis_read_hi(yfilter));
	EXPAND_U8_4(vis_fmul8x16al, 1, vis_read_hi(yfilter));
	EXPAND_U8_4(vis_fmul8x16au, 2, vis_read_lo(yfilter));
	EXPAND_U8_4(vis_fmul8x16al, 3, vis_read_lo(yfilter));

	ADD_4(4, 0, 1);
	ADD_4(5, 2, 3);
	ADD_4(6, 4, 5);

	r70 = FPMUL16(r60, xfilter0);
	r71 = FPMUL16(r61, xfilter1);
	r72 = FPMUL16(r62, xfilter2);
	r73 = FPMUL16(r63, xfilter3);

	r80 = vis_fpadd16(r70, r71);
	r81 = vis_fpadd16(r72, r73);

	r90 = vis_fmuld8ulx16(vis_scale, vis_read_hi(r80));
	r91 = vis_fmuld8ulx16(vis_scale, vis_read_lo(r80));
	r92 = vis_fmuld8ulx16(vis_scale, vis_read_hi(r81));
	r93 = vis_fmuld8ulx16(vis_scale, vis_read_lo(r81));

	r100 = vis_fpadd32(r90, r92);
	r101 = vis_fpadd32(r91, r93);
	r110 = vis_fpack16_pair(r100, r101);
	VIS_ALIGNADDR(dline, 6);
	vis_st_u8(r110, dstData + xd[i] * 4 + 3);
	r110 = vis_faligndata(r110, r110);
	vis_st_u8(r110, dstData + xd[i] * 4 + 2);
	r110 = vis_faligndata(r110, r110);
	vis_st_u8(r110, dstData + xd[i] * 4 + 1);
	r110 = vis_faligndata(r110, r110);
	vis_st_u8(r110, dstData + xd[i] * 4 + 0);
}

/* *********************************************************** */

#undef FILTER_SHIFT
#undef FILTER_MASK

/* *********************************************************** */

#define	FILTER_SHIFT	(4 - 1)
/* (((1 << 9) - 1) << 3) */
#define	FILTER_MASK	4088

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_S16_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	void *filter_table = pws->filter_table;
	void *filter_table_vis = pws->filter_table_vis;
	const mlib_u8 *line;
	mlib_s32 i;
	mlib_s32 xsrc, ysrc, posx, posy;
	mlib_s32 stride = lineAddr[1] - lineAddr[0];
	mlib_s32 *xd = pws->dst_x;
	mlib_s32 *xs = pws->src_x;
	mlib_s32 *ys = pws->src_y;
	mlib_u16 *dxs = pws->dsrc_x;
	mlib_u16 *dys = pws->dsrc_y;
	mlib_d64 *dline, *yptr;
	mlib_d64 a0, a1, a2, a3;
	mlib_d64 r0, r1, r2, r3;
	mlib_d64 r4, r5, r6, r7;
	mlib_d64 r8, r9, r10;
	mlib_d64 xfilter;
	mlib_d64 yfilter0, yfilter1, yfilter2, yfilter3;
	mlib_f32 fr0, fr1;
	mlib_f32 vis_scale = vis_to_float(0x01000100);

	if (n <= 0)
		return;

	vis_write_gsr(10 << 3);

	xsrc = ((*xs++)) * 2;
	ysrc = (*ys++);

	LOAD_U8_2;
	LOAD_COEFF_S16_1;

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < n - 1; i++) {

		r0 = FPMUL16(a0, yfilter0);
		r1 = FPMUL16(a1, yfilter1);
		r2 = FPMUL16(a2, yfilter2);
		r3 = FPMUL16(a3, yfilter3);

		r4 = vis_fpadd16(r0, r1);
		r5 = vis_fpadd16(r2, r3);
		r6 = vis_fpadd16(r4, r5);
		r7 = FPMUL16(r6, xfilter);
		fr0 = vis_fpadd16s(vis_read_hi(r7), vis_read_lo(r7));
		r8 = vis_fmuld8sux16(vis_scale, fr0);
		fr1 = vis_fpadd32s(vis_read_hi(r8), vis_read_lo(r8));
		r9 = vis_write_lo(r9, fr1);

		xsrc = ((*xs++)) * 2;
		ysrc = (*ys++);

		LOAD_U8_2;
		LOAD_COEFF_S16_1;

		r10 = vis_fpackfix_pair(r9, r9);
		vis_st_u16(r10, dstData + xd[i] * 2);
	}

	r0 = FPMUL16(a0, yfilter0);
	r1 = FPMUL16(a1, yfilter1);
	r2 = FPMUL16(a2, yfilter2);
	r3 = FPMUL16(a3, yfilter3);

	r4 = vis_fpadd16(r0, r1);
	r5 = vis_fpadd16(r2, r3);
	r6 = vis_fpadd16(r4, r5);
	r7 = FPMUL16(r6, xfilter);
	fr0 = vis_fpadd16s(vis_read_hi(r7), vis_read_lo(r7));
	r8 = vis_fmuld8sux16(vis_scale, fr0);
	fr1 = vis_fpadd32s(vis_read_hi(r8), vis_read_lo(r8));
	r9 = vis_write_lo(r9, fr1);
	r10 = vis_fpackfix_pair(r9, r9);
	vis_st_u16(r10, dstData + xd[i] * 2);
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_S16_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	void *filter_table = pws->filter_table;
	void *filter_table_vis = pws->filter_table_vis;
	const mlib_u8 *line;
	mlib_s32 i;
	mlib_s32 xsrc, ysrc, posx, posy;
	mlib_s32 stride = lineAddr[1] - lineAddr[0];
	mlib_s32 *xd = pws->dst_x;
	mlib_s32 *xs = pws->src_x;
	mlib_s32 *ys = pws->src_y;
	mlib_u16 *dxs = pws->dsrc_x;
	mlib_u16 *dys = pws->dsrc_y;
	mlib_d64 *dline, *yptr;
	mlib_d64 a00, a01;
	mlib_d64 a10, a11;
	mlib_d64 a20, a21;
	mlib_d64 a30, a31;
	mlib_d64 r00, r01, r10, r11;
	mlib_d64 r20, r21, r30, r31;
	mlib_d64 r40, r41, r50, r51;
	mlib_d64 r60, r61, r70, r71;
	mlib_d64 r80, r90, r91, r100, r110;
	mlib_d64 xfilter, xfilter0, xfilter1;
	mlib_d64 yfilter0, yfilter1, yfilter2, yfilter3;
	mlib_f32 vis_scale = vis_to_float(0x01000100);

	if (n <= 0)
		return;

	vis_write_gsr(10 << 3);

	xsrc = ((*xs++)) * 4;
	ysrc = (*ys++);

	LOAD_U8_4;
	LOAD_COEFF_S16_1;
	DOUBLING_SHORT(xfilter, xfilter0, xfilter1);

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < n - 1; i++) {

		r00 = FPMUL16(a00, yfilter0);
		r01 = FPMUL16(a01, yfilter0);
		r10 = FPMUL16(a10, yfilter1);
		r11 = FPMUL16(a11, yfilter1);
		r20 = FPMUL16(a20, yfilter2);
		r21 = FPMUL16(a21, yfilter2);
		r30 = FPMUL16(a30, yfilter3);
		r31 = FPMUL16(a31, yfilter3);
		r40 = vis_fpadd16(r00, r10);
		r41 = vis_fpadd16(r01, r11);
		r50 = vis_fpadd16(r20, r30);
		r51 = vis_fpadd16(r21, r31);
		r60 = vis_fpadd16(r40, r50);
		r61 = vis_fpadd16(r41, r51);
		r70 = FPMUL16(r60, xfilter0);
		r71 = FPMUL16(r61, xfilter1);
		r80 = vis_fpadd16(r70, r71);
		r90 = vis_fmuld8sux16(vis_scale, vis_read_hi(r80));
		r91 = vis_fmuld8sux16(vis_scale, vis_read_lo(r80));
		r100 = vis_fpadd32(r90, r91);

		xsrc = ((*xs++)) * 4;
		ysrc = (*ys++);

		LOAD_U8_4;
		LOAD_COEFF_S16_1;
		DOUBLING_SHORT(xfilter, xfilter0, xfilter1);

		r110 = vis_fpackfix_pair(r100, r100);
		VIS_ALIGNADDR(dline, 6);
		vis_st_u16(r110, dstData + xd[i] * 4 + 2);
		r110 = vis_faligndata(r110, r110);
		vis_st_u16(r110, dstData + xd[i] * 4 + 0);
	}

	r00 = FPMUL16(a00, yfilter0);
	r01 = FPMUL16(a01, yfilter0);
	r10 = FPMUL16(a10, yfilter1);
	r11 = FPMUL16(a11, yfilter1);
	r20 = FPMUL16(a20, yfilter2);
	r21 = FPMUL16(a21, yfilter2);
	r30 = FPMUL16(a30, yfilter3);
	r31 = FPMUL16(a31, yfilter3);
	r40 = vis_fpadd16(r00, r10);
	r41 = vis_fpadd16(r01, r11);
	r50 = vis_fpadd16(r20, r30);
	r51 = vis_fpadd16(r21, r31);
	r60 = vis_fpadd16(r40, r50);
	r61 = vis_fpadd16(r41, r51);
	r70 = FPMUL16(r60, xfilter0);
	r71 = FPMUL16(r61, xfilter1);
	r80 = vis_fpadd16(r70, r71);
	r90 = vis_fmuld8sux16(vis_scale, vis_read_hi(r80));
	r91 = vis_fmuld8sux16(vis_scale, vis_read_lo(r80));
	r100 = vis_fpadd32(r90, r91);
	r110 = vis_fpackfix_pair(r100, r100);
	VIS_ALIGNADDR(dline, 6);
	vis_st_u16(r110, dstData + xd[i] * 4 + 2);
	r110 = vis_faligndata(r110, r110);
	vis_st_u16(r110, dstData + xd[i] * 4 + 0);
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_S16_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	void *filter_table = pws->filter_table;
	void *filter_table_vis = pws->filter_table_vis;
	const mlib_u8 *line;
	mlib_s32 i;
	mlib_s32 xsrc, ysrc, posx, posy;
	mlib_s32 stride = lineAddr[1] - lineAddr[0];
	mlib_s32 *xd = pws->dst_x;
	mlib_s32 *xs = pws->src_x;
	mlib_s32 *ys = pws->src_y;
	mlib_u16 *dxs = pws->dsrc_x;
	mlib_u16 *dys = pws->dsrc_y;
	mlib_d64 *dline;
	mlib_d64 *yptr, *xptr;
	mlib_d64 a00, a01, a02;
	mlib_d64 a10, a11, a12;
	mlib_d64 a20, a21, a22;
	mlib_d64 a30, a31, a32;
	mlib_d64 r00, r01, r02;
	mlib_d64 r10, r11, r12;
	mlib_d64 r20, r21, r22;
	mlib_d64 r30, r31, r32;
	mlib_d64 r40, r41, r42;
	mlib_d64 r50, r51, r52;
	mlib_d64 r60, r61, r62;
	mlib_d64 r70, r71, r72;
	mlib_d64 r80, r81, r82;
	mlib_d64 r90, r91;
	mlib_d64 r100, r101, r102, r103;
	mlib_d64 r110, r111;
	mlib_d64 r120;
	mlib_d64 xfilter0, xfilter1, xfilter2;
	mlib_d64 yfilter0, yfilter1, yfilter2, yfilter3;
	mlib_f32 vis_scale = vis_to_float(0x01000100);

	if (n <= 0)
		return;

	vis_write_gsr(10 << 3);

	xsrc = ((*xs++)) * 6;
	ysrc = (*ys++);

	LOAD_S16_3;
	LOAD_COEFF_S16_3;

	MUL_3(0, yfilter0);
	MUL_3(1, yfilter1);
	MUL_3(2, yfilter2);
	MUL_3(3, yfilter3);

	ADD_3(4, 0, 1);
	ADD_3(5, 2, 3);
	ADD_3(6, 4, 5);

	r70 = FPMUL16(r60, xfilter0);
	r71 = FPMUL16(r61, xfilter1);
	r72 = FPMUL16(r62, xfilter2);

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < n - 1; i++) {

		VIS_ALIGNADDR(dline, 6);
		r80 = vis_faligndata(r70, r71);
		VIS_ALIGNADDR(dline, 4);
		r81 = vis_faligndata(r71, r72);
		VIS_ALIGNADDR(dline, 2);
		r82 = vis_faligndata(r72, r72);

		r90 = vis_fpadd16(r70, r80);
		r91 = vis_fpadd16(r81, r82);

		r100 = vis_fmuld8sux16(vis_scale, vis_read_hi(r90));
		r101 = vis_fmuld8sux16(vis_scale, vis_read_lo(r90));
		r102 = vis_fmuld8sux16(vis_scale, vis_read_hi(r91));
		r103 = vis_fmuld8sux16(vis_scale, vis_read_lo(r91));

		r110 = vis_fpadd32(r100, r102);
		r111 = vis_fpadd32(r101, r103);

		xsrc = ((*xs++)) * 6;
		ysrc = (*ys++);

		LOAD_S16_3;
		LOAD_COEFF_S16_3;

		MUL_3(0, yfilter0);
		MUL_3(1, yfilter1);
		MUL_3(2, yfilter2);
		MUL_3(3, yfilter3);

		ADD_3(4, 0, 1);
		ADD_3(5, 2, 3);
		ADD_3(6, 4, 5);

		r70 = FPMUL16(r60, xfilter0);
		r71 = FPMUL16(r61, xfilter1);
		r72 = FPMUL16(r62, xfilter2);

		r120 = vis_fpackfix_pair(r110, r111);
		VIS_ALIGNADDR(dline, 2);
		r120 = vis_faligndata(r120, r120);
		vis_st_u16(r120, dstData + xd[i] * 6 + 0);
		r120 = vis_faligndata(r120, r120);
		vis_st_u16(r120, dstData + xd[i] * 6 + 2);
		r120 = vis_faligndata(r120, r120);
		vis_st_u16(r120, dstData + xd[i] * 6 + 4);
	}

	VIS_ALIGNADDR(dline, 6);
	r80 = vis_faligndata(r70, r71);
	VIS_ALIGNADDR(dline, 4);
	r81 = vis_faligndata(r71, r72);
	VIS_ALIGNADDR(dline, 2);
	r82 = vis_faligndata(r72, r72);

	r90 = vis_fpadd16(r70, r80);
	r91 = vis_fpadd16(r81, r82);

	r100 = vis_fmuld8sux16(vis_scale, vis_read_hi(r90));
	r101 = vis_fmuld8sux16(vis_scale, vis_read_lo(r90));
	r102 = vis_fmuld8sux16(vis_scale, vis_read_hi(r91));
	r103 = vis_fmuld8sux16(vis_scale, vis_read_lo(r91));

	r110 = vis_fpadd32(r100, r102);
	r111 = vis_fpadd32(r101, r103);
	r120 = vis_fpackfix_pair(r110, r111);
	r120 = vis_faligndata(r120, r120);
	vis_st_u16(r120, dstData + xd[i] * 6 + 0);
	r120 = vis_faligndata(r120, r120);
	vis_st_u16(r120, dstData + xd[i] * 6 + 2);
	r120 = vis_faligndata(r120, r120);
	vis_st_u16(r120, dstData + xd[i] * 6 + 4);
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_S16_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	void *filter_table = pws->filter_table;
	const mlib_u8 *line;
	mlib_u8 *sline;
	mlib_s32 i;
	mlib_s32 xsrc, ysrc, posx, posy;
	mlib_s32 stride = lineAddr[1] - lineAddr[0];
	mlib_s32 mask, pgsr;
	mlib_s32 *xd = pws->dst_x;
	mlib_s32 *xs = pws->src_x;
	mlib_s32 *ys = pws->src_y;
	mlib_u16 *dxs = pws->dsrc_x;
	mlib_u16 *dys = pws->dsrc_y;
	mlib_d64 *dline, *pline;
	mlib_d64 *yptr, *xptr;
	mlib_d64 a00, a01, a02, a03;
	mlib_d64 a10, a11, a12, a13;
	mlib_d64 a20, a21, a22, a23;
	mlib_d64 a30, a31, a32, a33;
	mlib_d64 r00, r01, r02, r03;
	mlib_d64 r10, r11, r12, r13;
	mlib_d64 r20, r21, r22, r23;
	mlib_d64 r30, r31, r32, r33;
	mlib_d64 r40, r41, r42, r43;
	mlib_d64 r50, r51, r52, r53;
	mlib_d64 r60, r61, r62, r63;
	mlib_d64 r70, r71, r72, r73;
	mlib_d64 r80, r81;
	mlib_d64 r90, r91, r92, r93;
	mlib_d64 r100, r101;
	mlib_d64 r110;
	mlib_d64 xfilter0, xfilter1, xfilter2, xfilter3;
	mlib_d64 yfilter0, yfilter1, yfilter2, yfilter3;
	mlib_f32 vis_scale = vis_to_float(0x01000100);

	if (n <= 0)
		return;

	vis_write_gsr(10 << 3);
	sline = dstData;
	pline = (mlib_d64 *)((mlib_addr)sline & ~7);
	pgsr = 8 - (mlib_addr)sline;
	mask = vis_edge16(sline, pline + 1);

	xsrc = ((*xs++)) * 8;
	ysrc = (*ys++);

	LOAD_S16_4;
	LOAD_COEFF_S16_4;

	MUL_4(0, yfilter0);
	MUL_4(1, yfilter1);
	MUL_4(2, yfilter2);
	MUL_4(3, yfilter3);

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < n - 1; i++) {

		ADD_4(4, 0, 1);
		ADD_4(5, 2, 3);
		ADD_4(6, 4, 5);

		r70 = FPMUL16(r60, xfilter0);
		r71 = FPMUL16(r61, xfilter1);
		r72 = FPMUL16(r62, xfilter2);
		r73 = FPMUL16(r63, xfilter3);

		r80 = vis_fpadd16(r70, r71);
		r81 = vis_fpadd16(r72, r73);
		r90 = vis_fmuld8sux16(vis_scale, vis_read_hi(r80));
		r91 = vis_fmuld8sux16(vis_scale, vis_read_lo(r80));
		r92 = vis_fmuld8sux16(vis_scale, vis_read_hi(r81));
		r93 = vis_fmuld8sux16(vis_scale, vis_read_lo(r81));
		r100 = vis_fpadd32(r90, r92);
		r101 = vis_fpadd32(r91, r93);

		xsrc = ((*xs++)) * 8;
		ysrc = (*ys++);

		LOAD_S16_4;
		LOAD_COEFF_S16_4;

		MUL_4(0, yfilter0);
		MUL_4(1, yfilter1);
		MUL_4(2, yfilter2);
		MUL_4(3, yfilter3);

		r110 = vis_fpackfix_pair(r100, r101);
		VIS_ALIGNADDR(dline, pgsr);
		r110 = vis_faligndata(r110, r110);
		vis_pst_16(r110, pline + xd[i], mask);
		vis_pst_16(r110, pline + xd[i] + 1, ~mask);
	}

	ADD_4(4, 0, 1);
	ADD_4(5, 2, 3);
	ADD_4(6, 4, 5);

	r70 = FPMUL16(r60, xfilter0);
	r71 = FPMUL16(r61, xfilter1);
	r72 = FPMUL16(r62, xfilter2);
	r73 = FPMUL16(r63, xfilter3);

	r80 = vis_fpadd16(r70, r71);
	r81 = vis_fpadd16(r72, r73);
	r90 = vis_fmuld8sux16(vis_scale, vis_read_hi(r80));
	r91 = vis_fmuld8sux16(vis_scale, vis_read_lo(r80));
	r92 = vis_fmuld8sux16(vis_scale, vis_read_hi(r81));
	r93 = vis_fmuld8sux16(vis_scale, vis_read_lo(r81));
	r100 = vis_fpadd32(r90, r92);
	r101 = vis_fpadd32(r91, r93);
	r110 = vis_fpackfix_pair(r100, r101);
	VIS_ALIGNADDR(dline, pgsr);
	r110 = vis_faligndata(r110, r110);
	vis_pst_16(r110, pline + xd[i], mask);
	vis_pst_16(r110, pline + xd[i] + 1, ~mask);
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_U16_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	void *filter_table = pws->filter_table;
	void *filter_table_vis = pws->filter_table_vis;
	const mlib_u8 *line;
	mlib_s32 i;
	mlib_s32 xsrc, ysrc, posx, posy;
	mlib_s32 stride = lineAddr[1] - lineAddr[0];
	mlib_s32 *xd = pws->dst_x;
	mlib_s32 *xs = pws->src_x;
	mlib_s32 *ys = pws->src_y;
	mlib_u16 *dxs = pws->dsrc_x;
	mlib_u16 *dys = pws->dsrc_y;
	mlib_d64 *dline, *yptr;
	mlib_d64 a0, a1, a2, a3;
	mlib_d64 r0, r1, r2, r3;
	mlib_d64 r4, r5, r6, r7;
	mlib_d64 r8, r9, r10;
	mlib_d64 xfilter;
	mlib_d64 yfilter0, yfilter1, yfilter2, yfilter3;
	mlib_f32 fr0, fr1;
	mlib_f32 vis_scale = vis_to_float(0x01000100);
	mlib_d64 vis_mone = vis_to_double_dup(0x80008000);

	if (n <= 0)
		return;

	vis_write_gsr(10 << 3);

	xsrc = ((*xs++)) * 2;
	ysrc = (*ys++);

	LOAD_U8_2;
	LOAD_COEFF_S16_1;

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < n - 1; i++) {

		r0 = FPMULU16(a0, yfilter0);
		r1 = FPMULU16(a1, yfilter1);
		r2 = FPMULU16(a2, yfilter2);
		r3 = FPMULU16(a3, yfilter3);

		r4 = vis_fpadd16(r0, r1);
		r5 = vis_fpadd16(r2, r3);
		r6 = vis_fpadd16(r4, r5);
		r7 = FPMUL16(r6, xfilter);
		fr0 = vis_fpadd16s(vis_read_hi(r7), vis_read_lo(r7));
		r8 = vis_fmuld8sux16(vis_scale, fr0);
		fr1 = vis_fpadd32s(vis_read_hi(r8), vis_read_lo(r8));
		r9 = vis_write_lo(r9, fr1);

		xsrc = ((*xs++)) * 2;
		ysrc = (*ys++);

		LOAD_U8_2;
		LOAD_COEFF_S16_1;

		r10 = vis_fxor(vis_fpackfix_pair(r9, r9), vis_mone);
		vis_st_u16(r10, dstData + xd[i] * 2);
	}

	r0 = FPMULU16(a0, yfilter0);
	r1 = FPMULU16(a1, yfilter1);
	r2 = FPMULU16(a2, yfilter2);
	r3 = FPMULU16(a3, yfilter3);

	r4 = vis_fpadd16(r0, r1);
	r5 = vis_fpadd16(r2, r3);
	r6 = vis_fpadd16(r4, r5);
	r7 = FPMUL16(r6, xfilter);
	fr0 = vis_fpadd16s(vis_read_hi(r7), vis_read_lo(r7));
	r8 = vis_fmuld8sux16(vis_scale, fr0);
	fr1 = vis_fpadd32s(vis_read_hi(r8), vis_read_lo(r8));
	r9 = vis_write_lo(r9, fr1);
	r10 = vis_fxor(vis_fpackfix_pair(r9, r9), vis_mone);
	vis_st_u16(r10, dstData + xd[i] * 2);
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_U16_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	void *filter_table = pws->filter_table;
	void *filter_table_vis = pws->filter_table_vis;
	const mlib_u8 *line;
	mlib_s32 i;
	mlib_s32 xsrc, ysrc, posx, posy;
	mlib_s32 stride = lineAddr[1] - lineAddr[0];
	mlib_s32 *xd = pws->dst_x;
	mlib_s32 *xs = pws->src_x;
	mlib_s32 *ys = pws->src_y;
	mlib_u16 *dxs = pws->dsrc_x;
	mlib_u16 *dys = pws->dsrc_y;
	mlib_d64 *dline, *yptr;
	mlib_d64 a00, a01;
	mlib_d64 a10, a11;
	mlib_d64 a20, a21;
	mlib_d64 a30, a31;
	mlib_d64 r00, r01, r10, r11;
	mlib_d64 r20, r21, r30, r31;
	mlib_d64 r40, r41, r50, r51;
	mlib_d64 r60, r61, r70, r71;
	mlib_d64 r80, r90, r91, r100, r110;
	mlib_d64 xfilter, xfilter0, xfilter1;
	mlib_d64 yfilter0, yfilter1, yfilter2, yfilter3;
	mlib_f32 vis_scale = vis_to_float(0x01000100);
	mlib_d64 vis_mone = vis_to_double_dup(0x80008000);

	if (n <= 0)
		return;

	vis_write_gsr(10 << 3);

	xsrc = ((*xs++)) * 4;
	ysrc = (*ys++);

	LOAD_U8_4;
	LOAD_COEFF_S16_1;
	DOUBLING_SHORT(xfilter, xfilter0, xfilter1);

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < n - 1; i++) {

		r00 = FPMULU16(a00, yfilter0);
		r01 = FPMULU16(a01, yfilter0);
		r10 = FPMULU16(a10, yfilter1);
		r11 = FPMULU16(a11, yfilter1);
		r20 = FPMULU16(a20, yfilter2);
		r21 = FPMULU16(a21, yfilter2);
		r30 = FPMULU16(a30, yfilter3);
		r31 = FPMULU16(a31, yfilter3);
		r40 = vis_fpadd16(r00, r10);
		r41 = vis_fpadd16(r01, r11);
		r50 = vis_fpadd16(r20, r30);
		r51 = vis_fpadd16(r21, r31);
		r60 = vis_fpadd16(r40, r50);
		r61 = vis_fpadd16(r41, r51);
		r70 = FPMUL16(r60, xfilter0);
		r71 = FPMUL16(r61, xfilter1);
		r80 = vis_fpadd16(r70, r71);
		r90 = vis_fmuld8sux16(vis_scale, vis_read_hi(r80));
		r91 = vis_fmuld8sux16(vis_scale, vis_read_lo(r80));
		r100 = vis_fpadd32(r90, r91);

		xsrc = ((*xs++)) * 4;
		ysrc = (*ys++);

		LOAD_U8_4;
		LOAD_COEFF_S16_1;
		DOUBLING_SHORT(xfilter, xfilter0, xfilter1);

		r110 = vis_fxor(vis_fpackfix_pair(r100, r100), vis_mone);
		VIS_ALIGNADDR(dline, 6);
		vis_st_u16(r110, dstData + xd[i] * 4 + 2);
		r110 = vis_faligndata(r110, r110);
		vis_st_u16(r110, dstData + xd[i] * 4 + 0);
	}

	r00 = FPMULU16(a00, yfilter0);
	r01 = FPMULU16(a01, yfilter0);
	r10 = FPMULU16(a10, yfilter1);
	r11 = FPMULU16(a11, yfilter1);
	r20 = FPMULU16(a20, yfilter2);
	r21 = FPMULU16(a21, yfilter2);
	r30 = FPMULU16(a30, yfilter3);
	r31 = FPMULU16(a31, yfilter3);
	r40 = vis_fpadd16(r00, r10);
	r41 = vis_fpadd16(r01, r11);
	r50 = vis_fpadd16(r20, r30);
	r51 = vis_fpadd16(r21, r31);
	r60 = vis_fpadd16(r40, r50);
	r61 = vis_fpadd16(r41, r51);
	r70 = FPMUL16(r60, xfilter0);
	r71 = FPMUL16(r61, xfilter1);
	r80 = vis_fpadd16(r70, r71);
	r90 = vis_fmuld8sux16(vis_scale, vis_read_hi(r80));
	r91 = vis_fmuld8sux16(vis_scale, vis_read_lo(r80));
	r100 = vis_fpadd32(r90, r91);
	r110 = vis_fxor(vis_fpackfix_pair(r100, r100), vis_mone);
	VIS_ALIGNADDR(dline, 6);
	vis_st_u16(r110, dstData + xd[i] * 4 + 2);
	r110 = vis_faligndata(r110, r110);
	vis_st_u16(r110, dstData + xd[i] * 4 + 0);
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_U16_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	void *filter_table = pws->filter_table;
	void *filter_table_vis = pws->filter_table_vis;
	const mlib_u8 *line;
	mlib_s32 i;
	mlib_s32 xsrc, ysrc, posx, posy;
	mlib_s32 stride = lineAddr[1] - lineAddr[0];
	mlib_s32 *xd = pws->dst_x;
	mlib_s32 *xs = pws->src_x;
	mlib_s32 *ys = pws->src_y;
	mlib_u16 *dxs = pws->dsrc_x;
	mlib_u16 *dys = pws->dsrc_y;
	mlib_d64 *dline;
	mlib_d64 *yptr, *xptr;
	mlib_d64 a00, a01, a02;
	mlib_d64 a10, a11, a12;
	mlib_d64 a20, a21, a22;
	mlib_d64 a30, a31, a32;
	mlib_d64 r00, r01, r02;
	mlib_d64 r10, r11, r12;
	mlib_d64 r20, r21, r22;
	mlib_d64 r30, r31, r32;
	mlib_d64 r40, r41, r42;
	mlib_d64 r50, r51, r52;
	mlib_d64 r60, r61, r62;
	mlib_d64 r70, r71, r72;
	mlib_d64 r80, r81, r82;
	mlib_d64 r90, r91;
	mlib_d64 r100, r101, r102, r103;
	mlib_d64 r110, r111;
	mlib_d64 r120;
	mlib_d64 xfilter0, xfilter1, xfilter2;
	mlib_d64 yfilter0, yfilter1, yfilter2, yfilter3;
	mlib_f32 vis_scale = vis_to_float(0x01000100);
	mlib_d64 vis_mone = vis_to_double_dup(0x80008000);

	if (n <= 0)
		return;

	vis_write_gsr(10 << 3);

	xsrc = ((*xs++)) * 6;
	ysrc = (*ys++);

	LOAD_S16_3;
	LOAD_COEFF_S16_3;

	MULU_3(0, yfilter0);
	MULU_3(1, yfilter1);
	MULU_3(2, yfilter2);
	MULU_3(3, yfilter3);

	ADD_3(4, 0, 1);
	ADD_3(5, 2, 3);
	ADD_3(6, 4, 5);

	r70 = FPMUL16(r60, xfilter0);
	r71 = FPMUL16(r61, xfilter1);
	r72 = FPMUL16(r62, xfilter2);

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < n - 1; i++) {

		VIS_ALIGNADDR(dline, 6);
		r80 = vis_faligndata(r70, r71);
		VIS_ALIGNADDR(dline, 4);
		r81 = vis_faligndata(r71, r72);
		VIS_ALIGNADDR(dline, 2);
		r82 = vis_faligndata(r72, r72);

		r90 = vis_fpadd16(r70, r80);
		r91 = vis_fpadd16(r81, r82);

		r100 = vis_fmuld8sux16(vis_scale, vis_read_hi(r90));
		r101 = vis_fmuld8sux16(vis_scale, vis_read_lo(r90));
		r102 = vis_fmuld8sux16(vis_scale, vis_read_hi(r91));
		r103 = vis_fmuld8sux16(vis_scale, vis_read_lo(r91));

		r110 = vis_fpadd32(r100, r102);
		r111 = vis_fpadd32(r101, r103);

		xsrc = ((*xs++)) * 6;
		ysrc = (*ys++);

		LOAD_S16_3;
		LOAD_COEFF_S16_3;

		MULU_3(0, yfilter0);
		MULU_3(1, yfilter1);
		MULU_3(2, yfilter2);
		MULU_3(3, yfilter3);

		ADD_3(4, 0, 1);
		ADD_3(5, 2, 3);
		ADD_3(6, 4, 5);

		r70 = FPMUL16(r60, xfilter0);
		r71 = FPMUL16(r61, xfilter1);
		r72 = FPMUL16(r62, xfilter2);

		r120 = vis_fxor(vis_fpackfix_pair(r110, r111), vis_mone);

		VIS_ALIGNADDR(dline, 2);
		r120 = vis_faligndata(r120, r120);
		vis_st_u16(r120, dstData + xd[i] * 6 + 0);
		r120 = vis_faligndata(r120, r120);
		vis_st_u16(r120, dstData + xd[i] * 6 + 2);
		r120 = vis_faligndata(r120, r120);
		vis_st_u16(r120, dstData + xd[i] * 6 + 4);
	}

	VIS_ALIGNADDR(dline, 6);
	r80 = vis_faligndata(r70, r71);
	VIS_ALIGNADDR(dline, 4);
	r81 = vis_faligndata(r71, r72);
	VIS_ALIGNADDR(dline, 2);
	r82 = vis_faligndata(r72, r72);

	r90 = vis_fpadd16(r70, r80);
	r91 = vis_fpadd16(r81, r82);

	r100 = vis_fmuld8sux16(vis_scale, vis_read_hi(r90));
	r101 = vis_fmuld8sux16(vis_scale, vis_read_lo(r90));
	r102 = vis_fmuld8sux16(vis_scale, vis_read_hi(r91));
	r103 = vis_fmuld8sux16(vis_scale, vis_read_lo(r91));

	r110 = vis_fpadd32(r100, r102);
	r111 = vis_fpadd32(r101, r103);

	r120 = vis_fxor(vis_fpackfix_pair(r110, r111), vis_mone);

	VIS_ALIGNADDR(dline, 2);
	r120 = vis_faligndata(r120, r120);
	vis_st_u16(r120, dstData + xd[i] * 6 + 0);
	r120 = vis_faligndata(r120, r120);
	vis_st_u16(r120, dstData + xd[i] * 6 + 2);
	r120 = vis_faligndata(r120, r120);
	vis_st_u16(r120, dstData + xd[i] * 6 + 4);
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_U16_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	void *filter_table = pws->filter_table;
	const mlib_u8 *line;
	mlib_u8 *sline;
	mlib_s32 i;
	mlib_s32 xsrc, ysrc, posx, posy;
	mlib_s32 stride = lineAddr[1] - lineAddr[0];
	mlib_s32 mask, pgsr;
	mlib_s32 *xd = pws->dst_x;
	mlib_s32 *xs = pws->src_x;
	mlib_s32 *ys = pws->src_y;
	mlib_u16 *dxs = pws->dsrc_x;
	mlib_u16 *dys = pws->dsrc_y;
	mlib_d64 *dline, *pline;
	mlib_d64 *yptr, *xptr;
	mlib_d64 a00, a01, a02, a03;
	mlib_d64 a10, a11, a12, a13;
	mlib_d64 a20, a21, a22, a23;
	mlib_d64 a30, a31, a32, a33;
	mlib_d64 r00, r01, r02, r03;
	mlib_d64 r10, r11, r12, r13;
	mlib_d64 r20, r21, r22, r23;
	mlib_d64 r30, r31, r32, r33;
	mlib_d64 r40, r41, r42, r43;
	mlib_d64 r50, r51, r52, r53;
	mlib_d64 r60, r61, r62, r63;
	mlib_d64 r70, r71, r72, r73;
	mlib_d64 r80, r81;
	mlib_d64 r90, r91, r92, r93;
	mlib_d64 r100, r101;
	mlib_d64 r110;
	mlib_d64 xfilter0, xfilter1, xfilter2, xfilter3;
	mlib_d64 yfilter0, yfilter1, yfilter2, yfilter3;
	mlib_f32 vis_scale = vis_to_float(0x01000100);
	mlib_d64 vis_mone = vis_to_double_dup(0x80008000);

	if (n <= 0)
		return;

	vis_write_gsr(10 << 3);
	sline = dstData;
	pline = (mlib_d64 *)((mlib_addr)sline & ~7);
	pgsr = 8 - (mlib_addr)sline;
	mask = vis_edge16(sline, pline + 1);

	xsrc = ((*xs++)) * 8;
	ysrc = (*ys++);

	LOAD_S16_4;
	LOAD_COEFF_S16_4;

	MULU_4(0, yfilter0);
	MULU_4(1, yfilter1);
	MULU_4(2, yfilter2);
	MULU_4(3, yfilter3);

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < n - 1; i++) {

		ADD_4(4, 0, 1);
		ADD_4(5, 2, 3);
		ADD_4(6, 4, 5);

		r70 = FPMUL16(r60, xfilter0);
		r71 = FPMUL16(r61, xfilter1);
		r72 = FPMUL16(r62, xfilter2);
		r73 = FPMUL16(r63, xfilter3);

		r80 = vis_fpadd16(r70, r71);
		r81 = vis_fpadd16(r72, r73);
		r90 = vis_fmuld8sux16(vis_scale, vis_read_hi(r80));
		r91 = vis_fmuld8sux16(vis_scale, vis_read_lo(r80));
		r92 = vis_fmuld8sux16(vis_scale, vis_read_hi(r81));
		r93 = vis_fmuld8sux16(vis_scale, vis_read_lo(r81));
		r100 = vis_fpadd32(r90, r92);
		r101 = vis_fpadd32(r91, r93);

		xsrc = ((*xs++)) * 8;
		ysrc = (*ys++);

		LOAD_S16_4;
		LOAD_COEFF_S16_4;

		MULU_4(0, yfilter0);
		MULU_4(1, yfilter1);
		MULU_4(2, yfilter2);
		MULU_4(3, yfilter3);

		r110 = vis_fxor(vis_fpackfix_pair(r100, r101), vis_mone);
		VIS_ALIGNADDR(dline, pgsr);
		r110 = vis_faligndata(r110, r110);
		vis_pst_16(r110, pline + xd[i], mask);
		vis_pst_16(r110, pline + xd[i] + 1, ~mask);
	}

	ADD_4(4, 0, 1);
	ADD_4(5, 2, 3);
	ADD_4(6, 4, 5);

	r70 = FPMUL16(r60, xfilter0);
	r71 = FPMUL16(r61, xfilter1);
	r72 = FPMUL16(r62, xfilter2);
	r73 = FPMUL16(r63, xfilter3);

	r80 = vis_fpadd16(r70, r71);
	r81 = vis_fpadd16(r72, r73);
	r90 = vis_fmuld8sux16(vis_scale, vis_read_hi(r80));
	r91 = vis_fmuld8sux16(vis_scale, vis_read_lo(r80));
	r92 = vis_fmuld8sux16(vis_scale, vis_read_hi(r81));
	r93 = vis_fmuld8sux16(vis_scale, vis_read_lo(r81));
	r100 = vis_fpadd32(r90, r92);
	r101 = vis_fpadd32(r91, r93);
	r110 = vis_fxor(vis_fpackfix_pair(r100, r101), vis_mone);
	VIS_ALIGNADDR(dline, pgsr);
	r110 = vis_faligndata(r110, r110);
	vis_pst_16(r110, pline + xd[i], mask);
	vis_pst_16(r110, pline + xd[i] + 1, ~mask);
}

/* *********************************************************** */
