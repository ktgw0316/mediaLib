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

#pragma ident	"@(#)mlib_v_ImagePolynomialWarp_BL.c	9.2	07/11/05 SMI"

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImagePolynomialWarp.h>

/* *********************************************************** */

#define	VIS_ALIGNADDR(X, Y)	vis_alignaddr((void *)(X), (Y))

/* *********************************************************** */

#define	VIS_LOAD_OPR	vis_ld_u8_i

/* *********************************************************** */

#define	LOAD_PIX(line, r0, r1, r2, r3)                             \
	a0 = vis_faligndata(VIS_LOAD_OPR((void *)line, r0), a0);   \
	a1 = vis_faligndata(VIS_LOAD_OPR((void *)line, r1), a1);   \
	a2 = vis_faligndata(VIS_LOAD_OPR((void *)line, r2), a2);   \
	a3 = vis_faligndata(VIS_LOAD_OPR((void *)line, r3), a3)

/* *********************************************************** */

#define	LOAD_8_PIX_U8_1                                               \
	xsrc = xs[7];                                                 \
	ysrc = ys[7];                                                 \
	LOAD_PIX(lineAddr[ysrc], xsrc, xsrc + 1, xsrc + stride + 1,   \
	    xsrc + stride);                                           \
	xsrc = xs[6];                                                 \
	ysrc = ys[6];                                                 \
	LOAD_PIX(lineAddr[ysrc], xsrc, xsrc + 1, xsrc + stride + 1,   \
	    xsrc + stride);                                           \
	xsrc = xs[5];                                                 \
	ysrc = ys[5];                                                 \
	LOAD_PIX(lineAddr[ysrc], xsrc, xsrc + 1, xsrc + stride + 1,   \
	    xsrc + stride);                                           \
	xsrc = xs[4];                                                 \
	ysrc = ys[4];                                                 \
	LOAD_PIX(lineAddr[ysrc], xsrc, xsrc + 1, xsrc + stride + 1,   \
	    xsrc + stride);                                           \
	xsrc = xs[3];                                                 \
	ysrc = ys[3];                                                 \
	LOAD_PIX(lineAddr[ysrc], xsrc, xsrc + 1, xsrc + stride + 1,   \
	    xsrc + stride);                                           \
	xsrc = xs[2];                                                 \
	ysrc = ys[2];                                                 \
	LOAD_PIX(lineAddr[ysrc], xsrc, xsrc + 1, xsrc + stride + 1,   \
	    xsrc + stride);                                           \
	xsrc = xs[1];                                                 \
	ysrc = ys[1];                                                 \
	LOAD_PIX(lineAddr[ysrc], xsrc, xsrc + 1, xsrc + stride + 1,   \
	    xsrc + stride);                                           \
	xsrc = xs[0];                                                 \
	ysrc = ys[0];                                                 \
	LOAD_PIX(lineAddr[ysrc], xsrc, xsrc + 1, xsrc + stride + 1,   \
	    xsrc + stride)

/* *********************************************************** */

#define	LOAD_8_PIX_U8_2                                                   \
	xsrc = xs[3] * 2;                                                 \
	ysrc = ys[3];                                                     \
	LOAD_PIX(lineAddr[ysrc], xsrc + 1, xsrc + 3, xsrc + stride + 3,   \
	    xsrc + stride + 1);                                           \
	LOAD_PIX(lineAddr[ysrc], xsrc + 0, xsrc + 2, xsrc + stride + 2,   \
	    xsrc + stride + 0);                                           \
	xsrc = xs[2] * 2;                                                 \
	ysrc = ys[2];                                                     \
	LOAD_PIX(lineAddr[ysrc], xsrc + 1, xsrc + 3, xsrc + stride + 3,   \
	    xsrc + stride + 1);                                           \
	LOAD_PIX(lineAddr[ysrc], xsrc + 0, xsrc + 2, xsrc + stride + 2,   \
	    xsrc + stride + 0);                                           \
	xsrc = xs[1] * 2;                                                 \
	ysrc = ys[1];                                                     \
	LOAD_PIX(lineAddr[ysrc], xsrc + 1, xsrc + 3, xsrc + stride + 3,   \
	    xsrc + stride + 1);                                           \
	LOAD_PIX(lineAddr[ysrc], xsrc + 0, xsrc + 2, xsrc + stride + 2,   \
	    xsrc + stride + 0);                                           \
	xsrc = xs[0] * 2;                                                 \
	ysrc = ys[0];                                                     \
	LOAD_PIX(lineAddr[ysrc], xsrc + 1, xsrc + 3, xsrc + stride + 3,   \
	    xsrc + stride + 1);                                           \
	LOAD_PIX(lineAddr[ysrc], xsrc + 0, xsrc + 2, xsrc + stride + 2,   \
	    xsrc + stride + 0)

/* *********************************************************** */

#define	LOAD_8_PIX_U8_3(var)                                    \
	xsrc = ((*xs++));                                       \
	ysrc = ((*ys++));                                       \
	line = lineAddr[ysrc] + xsrc * 3 - 1;                   \
	dline = VIS_ALIGNADDR(line, 0);                         \
	var##01 = vis_faligndata(dline[0], dline[1]);           \
	dline = VIS_ALIGNADDR(line, stride);                    \
	var##32 = vis_faligndata(dline[0], dline[1]);           \
	VIS_ALIGNADDR(dline, 1);                                \
	var##01 =                                               \
	vis_write_hi(var##01,                                   \
	    vis_read_hi(vis_faligndata(var##01, var##01)));     \
	var##32 =                                               \
	vis_write_hi(var##32,                                   \
	    vis_read_hi(vis_faligndata(var##32, var##32)))

/* *********************************************************** */

#define	LOAD_8_PIX_U8_4(var)                                    \
	xsrc = ((*xs++)) * 4;                                   \
	ysrc = ((*ys++));                                       \
	line = lineAddr[ysrc] + xsrc;                           \
	dline = VIS_ALIGNADDR(line, 0);                         \
	var##01 = vis_faligndata(dline[0], dline[1]);           \
	dline = VIS_ALIGNADDR(line, stride);                    \
	var##32 = vis_faligndata(dline[0], dline[1])

/* *********************************************************** */

#define	LOAD_4_PIX_S16_1                                              \
	xsrc = xs[3] * 2;                                             \
	ysrc = ys[3];                                                 \
	LOAD_PIX(lineAddr[ysrc], xsrc, xsrc + 2, xsrc + stride + 2,   \
	    xsrc + stride);                                           \
	xsrc = xs[2] * 2;                                             \
	ysrc = ys[2];                                                 \
	LOAD_PIX(lineAddr[ysrc], xsrc, xsrc + 2, xsrc + stride + 2,   \
	    xsrc + stride);                                           \
	xsrc = xs[1] * 2;                                             \
	ysrc = ys[1];                                                 \
	LOAD_PIX(lineAddr[ysrc], xsrc, xsrc + 2, xsrc + stride + 2,   \
	    xsrc + stride);                                           \
	xsrc = xs[0] * 2;                                             \
	ysrc = ys[0];                                                 \
	LOAD_PIX(lineAddr[ysrc], xsrc, xsrc + 2, xsrc + stride + 2,   \
	    xsrc + stride)

/* *********************************************************** */

#define	LOAD_4_PIX_S16_2                                                  \
	xsrc = xs[1] * 4;                                                 \
	ysrc = ys[1];                                                     \
	LOAD_PIX(lineAddr[ysrc], xsrc + 2, xsrc + 6, xsrc + stride + 6,   \
	    xsrc + stride + 2);                                           \
	LOAD_PIX(lineAddr[ysrc], xsrc + 0, xsrc + 4, xsrc + stride + 4,   \
	    xsrc + stride + 0);                                           \
	xsrc = xs[0] * 4;                                                 \
	ysrc = ys[0];                                                     \
	LOAD_PIX(lineAddr[ysrc], xsrc + 2, xsrc + 6, xsrc + stride + 6,   \
	    xsrc + stride + 2);                                           \
	LOAD_PIX(lineAddr[ysrc], xsrc + 0, xsrc + 4, xsrc + stride + 4,   \
	    xsrc + stride + 0)

/* *********************************************************** */

#define	LOAD_4_PIX_S16_4(var)                                   \
	xsrc = ((*xs++)) * 8;                                   \
	ysrc = ((*ys++));                                       \
	line = lineAddr[ysrc] + xsrc;                           \
	dline = VIS_ALIGNADDR(line, 0);                         \
	var##0 = vis_faligndata(dline[0], dline[1]);            \
	var##1 = vis_faligndata(dline[1], dline[2]);            \
	dline = VIS_ALIGNADDR(line, stride);                    \
	var##3 = vis_faligndata(dline[0], dline[1]);            \
	var##2 = vis_faligndata(dline[1], dline[2])

/* *********************************************************** */

#define	LOAD_4_PIX_S16_3(var)                                   \
	xsrc = ((*xs++)) * 6 - 2;                               \
	ysrc = ((*ys++));                                       \
	line = lineAddr[ysrc] + xsrc;                           \
	dline = VIS_ALIGNADDR(line, 0);                         \
	var##0 = vis_faligndata(dline[0], dline[1]);            \
	dline = VIS_ALIGNADDR(line, 6);                         \
	var##1 = vis_faligndata(dline[0], dline[1]);            \
	dline = VIS_ALIGNADDR(line, stride);                    \
	var##3 = vis_faligndata(dline[0], dline[1]);            \
	dline = VIS_ALIGNADDR(line, stride + 6);                \
	var##2 = vis_faligndata(dline[0], dline[1])

/* *********************************************************** */

#define	FPMUL16(x0, x1)                                               \
	vis_fpadd16(vis_fmul8sux16(x0, x1), vis_fmul8ulx16(x0, x1))

/* *********************************************************** */

#define	FPMULU16(x0, x1)                                          \
	vis_fpadd16(vis_fmul8sux16(vis_fxor(x0, vis_mone), x1),   \
	    vis_fmul8ulx16(vis_fxor(x0, vis_mone), x1))

/* *********************************************************** */

#define	COMPUTING_POINT_U8(opr, idx, dx_1, dx)                  \
	r0##idx = opr##(vis_read_##idx(a0), dx_1);              \
	r1##idx = opr##(vis_read_##idx(a1), dx);                \
	r2##idx = opr##(vis_read_##idx(a2), dx);                \
	r3##idx = opr##(vis_read_##idx(a3), dx_1);              \
	r4##idx = vis_fpadd16(r0##idx, r1##idx);                \
	r5##idx = vis_fpadd16(r2##idx, r3##idx);                \
	r6##idx = FPMUL16(r4##idx, fdy##idx##_1);               \
	r7##idx = FPMUL16(r5##idx, fdy##idx);                   \
	r8##idx = vis_fpadd16(r6##idx, r7##idx)

/* *********************************************************** */

#define	COMPUTING_POINT_U8_4(opr, idx, var)                     \
	r0##idx = opr##(vis_read_hi(var##01), fdx_1);           \
	r1##idx = opr##(vis_read_lo(var##01), fdx);             \
	r2##idx = opr##(vis_read_lo(var##32), fdx);             \
	r3##idx = opr##(vis_read_hi(var##32), fdx_1);           \
	r4##idx = vis_fpadd16(r0##idx, r1##idx);                \
	r5##idx = vis_fpadd16(r2##idx, r3##idx);                \
	r6##idx = FPMUL16(r4##idx, fdy##idx##_1);               \
	r7##idx = FPMUL16(r5##idx, fdy##idx);                   \
	r8##idx = vis_fpadd16(r6##idx, r7##idx)

/* *********************************************************** */

#define	COMPUTING_POINT_S16_4(idx, var)                         \
	r0##idx = FPMUL16(var##0, fdx##idx##_1);                \
	r1##idx = FPMUL16(var##1, fdx##idx);                    \
	r4##idx = vis_fpadd16(r0##idx, r1##idx);                \
	r4##idx = vis_fpadd16(r4##idx, r4##idx);                \
	r2##idx = FPMUL16(var##2, fdx##idx);                    \
	r3##idx = FPMUL16(var##3, fdx##idx##_1);                \
	r5##idx = vis_fpadd16(r2##idx, r3##idx);                \
	r5##idx = vis_fpadd16(r5##idx, r5##idx);                \
	r6##idx = FPMUL16(r4##idx, fdy##idx##_1);               \
	r7##idx = FPMUL16(r5##idx, fdy##idx);                   \
	r8##idx = vis_fpadd16(r6##idx, r7##idx);                \
	r8##idx = vis_fpadd16(r8##idx, r8##idx)

/* *********************************************************** */

#define	COMPUTING_POINT_U16_4(idx, var)                         \
	r0##idx = FPMULU16(var##0, fdx##idx##_1);               \
	r1##idx = FPMULU16(var##1, fdx##idx);                   \
	r4##idx = vis_fpadd16(r0##idx, r1##idx);                \
	r4##idx = vis_fpadd16(r4##idx, r4##idx);                \
	r2##idx = FPMULU16(var##2, fdx##idx);                   \
	r3##idx = FPMULU16(var##3, fdx##idx##_1);               \
	r5##idx = vis_fpadd16(r2##idx, r3##idx);                \
	r5##idx = vis_fpadd16(r5##idx, r5##idx);                \
	r6##idx = FPMUL16(r4##idx, fdy##idx##_1);               \
	r7##idx = FPMUL16(r5##idx, fdy##idx);                   \
	r8##idx = vis_fpadd16(r6##idx, r7##idx);                \
	r8##idx = vis_fpadd16(r8##idx, r8##idx);                \
	r8##idx = vis_fpadd16(r8##idx, vis_mone)

/* *********************************************************** */

#define	DOUBLING_SHORT(src, dst0, dst1)                           \
	r10 = vis_fpmerge(vis_read_hi(src), vis_read_lo(src));    \
	r10 = vis_fpmerge(vis_read_hi(r10), vis_read_lo(r10));    \
	r11 = vis_fpmerge(vis_read_lo(r10), vis_read_lo(r10));    \
	r10 = vis_fpmerge(vis_read_hi(r10), vis_read_hi(r10));    \
	dst0 = vis_fpmerge(vis_read_hi(r10), vis_read_hi(r11));   \
	dst1 = vis_fpmerge(vis_read_lo(r10), vis_read_lo(r11))

/* *********************************************************** */

#define	MAKE_COEFF_S16_4(var)                                    \
	r10 = vis_fpmerge(fd##var, fd##var);                     \
	r11 = vis_fpmerge(vis_read_hi(r10), vis_read_hi(r10));   \
	r10 = vis_fpmerge(vis_read_lo(r10), vis_read_lo(r10));   \
	fd##var##lo =                                            \
	vis_fpmerge(vis_read_hi(r10), vis_read_lo(r10));         \
	fd##var##hi =                                            \
	vis_fpmerge(vis_read_hi(r11), vis_read_lo(r11));         \
	fd##var##hi_1 = vis_fpsub16(vis_one, fd##var##hi);       \
	fd##var##lo_1 = vis_fpsub16(vis_one, fd##var##lo)

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U8_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	mlib_s32 i, count, xsrc, ysrc;
	mlib_s32 *xs = pws->src_x;
	mlib_s32 *ys = pws->src_y;
	mlib_s32 *xd = pws->dst_x;
	mlib_s32 stride = lineAddr[1] - lineAddr[0];
	mlib_d64 a0, a1, a2, a3, r9;
	mlib_d64 r0hi, r1hi, r2hi, r3hi, r4hi, r5hi, r6hi, r7hi, r8hi;
	mlib_d64 r0lo, r1lo, r2lo, r3lo, r4lo, r5lo, r6lo, r7lo, r8lo;
	mlib_d64 fdxhi, fdyhi, fdxlo, fdylo;
	mlib_d64 fdxhi_1, fdyhi_1, fdxlo_1, fdylo_1;
	mlib_d64 vis_one = vis_to_double_dup(0x7FFF7FFF);
	mlib_d64 *buf = pws->buffer;
	mlib_d64 *coeffx = pws->dsrc_x;
	mlib_d64 *coeffy = pws->dsrc_y;

	if (n <= 0)
		return;

	count = (n >> 3) + ((n & 7) != 0);
	vis_write_gsr((1 << 3) | 7);

	fdxhi = (*coeffx++);
	fdyhi = (*coeffy++);
	fdxlo = (*coeffx++);
	fdylo = (*coeffy++);

	LOAD_8_PIX_U8_1;

	xs += 8;
	ys += 8;

	fdxhi_1 = vis_fpsub16(vis_one, fdxhi);
	fdyhi_1 = vis_fpsub16(vis_one, fdyhi);
	fdxlo_1 = vis_fpsub16(vis_one, fdxlo);
	fdylo_1 = vis_fpsub16(vis_one, fdylo);

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < count - 1; i++) {

		COMPUTING_POINT_U8(vis_fmul8x16, hi, fdxhi_1, fdxhi);
		COMPUTING_POINT_U8(vis_fmul8x16, lo, fdxlo_1, fdxlo);

		r9 = vis_fpack16_pair(r8hi, r8lo);

		LOAD_8_PIX_U8_1;
		xs += 8;

		ys += 8;

		fdxhi = (*coeffx++);
		fdyhi = (*coeffy++);
		fdxlo = (*coeffx++);
		fdylo = (*coeffy++);

		fdxhi_1 = vis_fpsub16(vis_one, fdxhi);
		fdyhi_1 = vis_fpsub16(vis_one, fdyhi);
		fdxlo_1 = vis_fpsub16(vis_one, fdxlo);
		fdylo_1 = vis_fpsub16(vis_one, fdylo);

		buf[i] = r9;
	}

	COMPUTING_POINT_U8(vis_fmul8x16, hi, fdxhi_1, fdxhi);
	COMPUTING_POINT_U8(vis_fmul8x16, lo, fdxlo_1, fdxlo);
	r9 = vis_fpack16_pair(r8hi, r8lo);
	buf[i] = r9;

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < n; i++) {
		dstData[xd[i]] = *((mlib_u8 *)buf + i);
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U8_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	mlib_s32 i, count, xsrc, ysrc;
	mlib_s32 *xs = pws->src_x;
	mlib_s32 *ys = pws->src_y;
	mlib_s32 *xd = pws->dst_x;
	mlib_s32 stride = lineAddr[1] - lineAddr[0];
	mlib_d64 a0, a1, a2, a3;
	mlib_d64 r0hi, r1hi, r2hi, r3hi, r4hi, r5hi, r6hi, r7hi, r8hi;
	mlib_d64 r0lo, r1lo, r2lo, r3lo, r4lo, r5lo, r6lo, r7lo, r8lo;
	mlib_d64 r9, r10, r11;
	mlib_d64 fdx, fdy;
	mlib_d64 fdyhi, fdyhi_1, fdylo, fdylo_1, fdxhi, fdxhi_1, fdxlo, fdxlo_1;
	mlib_d64 vis_one = vis_to_double_dup(0x7FFF7FFF);
	mlib_d64 *coeffx = pws->dsrc_x;
	mlib_d64 *coeffy = pws->dsrc_y;
	mlib_d64 *buf = pws->buffer;

	if (n <= 0)
		return;

	count = (n >> 2) + ((n & 3) != 0);
	vis_write_gsr((1 << 3) | 7);

	fdx = (*coeffx++);
	fdy = (*coeffy++);

	LOAD_8_PIX_U8_2;

	xs += 4;
	ys += 4;

	DOUBLING_SHORT(fdx, fdxhi, fdxlo);
	DOUBLING_SHORT(fdy, fdyhi, fdylo);

	fdxhi_1 = vis_fpsub16(vis_one, fdxhi);
	fdyhi_1 = vis_fpsub16(vis_one, fdyhi);
	fdxlo_1 = vis_fpsub16(vis_one, fdxlo);
	fdylo_1 = vis_fpsub16(vis_one, fdylo);

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < count - 1; i++) {

		COMPUTING_POINT_U8(vis_fmul8x16, hi, fdxhi_1, fdxhi);
		COMPUTING_POINT_U8(vis_fmul8x16, lo, fdxlo_1, fdxlo);

		r9 = vis_fpack16_pair(r8hi, r8lo);
		fdx = (*coeffx++);
		fdy = (*coeffy++);

		LOAD_8_PIX_U8_2;
		xs += 4;

		ys += 4;

		DOUBLING_SHORT(fdx, fdxhi, fdxlo);
		DOUBLING_SHORT(fdy, fdyhi, fdylo);

		fdxhi_1 = vis_fpsub16(vis_one, fdxhi);
		fdyhi_1 = vis_fpsub16(vis_one, fdyhi);
		fdxlo_1 = vis_fpsub16(vis_one, fdxlo);
		fdylo_1 = vis_fpsub16(vis_one, fdylo);

		buf[i] = r9;
	}

	COMPUTING_POINT_U8(vis_fmul8x16, hi, fdxhi_1, fdxhi);
	COMPUTING_POINT_U8(vis_fmul8x16, lo, fdxlo_1, fdxlo);

	r9 = vis_fpack16_pair(r8hi, r8lo);
	buf[i] = r9;

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < n; i++) {
		xsrc = xd[i] * 2;
		ysrc = *((mlib_u16 *)buf + i);

		dstData[xsrc] = ysrc >> 8;
		dstData[xsrc + 1] = ysrc;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U8_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	const mlib_u8 *line;
	mlib_s32 i, count, xsrc, ysrc;
	mlib_s32 *xs = pws->src_x;
	mlib_s32 *ys = pws->src_y;
	mlib_s32 *xd = pws->dst_x;
	mlib_s32 stride = lineAddr[1] - lineAddr[0];
	mlib_f32 *coeffx = pws->dsrc_x;
	mlib_f32 *coeffy = pws->dsrc_y;
	mlib_f32 fdx, fdy, fdx_1;
	mlib_f32 vis_one = vis_to_float(0x7FFF7FFF);
	mlib_d64 *dline, a01, a32, b01, b32;
	mlib_d64 r0hi, r1hi, r2hi, r3hi, r4hi, r5hi, r6hi, r7hi, r8hi;
	mlib_d64 r0lo, r1lo, r2lo, r3lo, r4lo, r5lo, r6lo, r7lo, r8lo;
	mlib_d64 r9, r10, r11;
	mlib_d64 fdyhi, fdyhi_1, fdylo, fdylo_1;
	mlib_d64 vis_done = vis_to_double_dup(0x7FFF7FFF);
	mlib_d64 *buf = pws->buffer;

	if (n <= 0)
		return;

	count = (n >> 1) + ((n & 1) != 0);
	vis_write_gsr((1 << 3) | 7);

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < count; i++) {

		fdx = (*coeffx++);
		fdy = (*coeffy++);

		LOAD_8_PIX_U8_3(a);
		LOAD_8_PIX_U8_3(b);

		r10 = vis_fpmerge(fdy, fdy);
		r11 = vis_fpmerge(vis_read_hi(r10), vis_read_hi(r10));
		r10 = vis_fpmerge(vis_read_lo(r10), vis_read_lo(r10));
		fdylo = vis_fpmerge(vis_read_hi(r10), vis_read_lo(r10));
		fdyhi = vis_fpmerge(vis_read_hi(r11), vis_read_lo(r11));

		fdx_1 = vis_fpsub16s(vis_one, fdx);
		fdyhi_1 = vis_fpsub16(vis_done, fdyhi);
		fdylo_1 = vis_fpsub16(vis_done, fdylo);

		COMPUTING_POINT_U8_4(vis_fmul8x16au, hi, a);
		COMPUTING_POINT_U8_4(vis_fmul8x16al, lo, b);

		r9 = vis_fpack16_pair(r8hi, r8lo);

		buf[i] = r9;
	}

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < n; i++) {
		ysrc = *((mlib_u32 *)buf + i);
		xsrc = xd[i] * 3;

		dstData[xsrc] = ysrc >> 24;
		dstData[xsrc + 1] = ysrc >> 16;
		dstData[xsrc + 2] = ysrc >> 8;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U8_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	const mlib_u8 *line;
	mlib_s32 i, count, xsrc, ysrc;
	mlib_s32 *xs = pws->src_x;
	mlib_s32 *ys = pws->src_y;
	mlib_s32 *xd = pws->dst_x;
	mlib_s32 stride = lineAddr[1] - lineAddr[0];
	mlib_f32 *coeffx = pws->dsrc_x;
	mlib_f32 *coeffy = pws->dsrc_y;
	mlib_f32 fdx, fdy, fdx_1;
	mlib_f32 vis_one = vis_to_float(0x7FFF7FFF);
	mlib_d64 *dline;
	mlib_d64 a01, a32, b01, b32;
	mlib_d64 r0hi, r1hi, r2hi, r3hi, r4hi, r5hi, r6hi, r7hi, r8hi;
	mlib_d64 r0lo, r1lo, r2lo, r3lo, r4lo, r5lo, r6lo, r7lo, r8lo;
	mlib_d64 r9, r10, r11;
	mlib_d64 fdyhi, fdyhi_1, fdylo, fdylo_1;
	mlib_d64 vis_done = vis_to_double_dup(0x7FFF7FFF);
	mlib_d64 *buf = pws->buffer;

	if (n <= 0)
		return;

	count = (n >> 1) + ((n & 1) != 0);
	vis_write_gsr((1 << 3) | 7);

	fdx = (*coeffx++);
	fdy = (*coeffy++);

	LOAD_8_PIX_U8_4(a);
	LOAD_8_PIX_U8_4(b);

	r10 = vis_fpmerge(fdy, fdy);
	r11 = vis_fpmerge(vis_read_hi(r10), vis_read_hi(r10));
	r10 = vis_fpmerge(vis_read_lo(r10), vis_read_lo(r10));
	fdylo = vis_fpmerge(vis_read_hi(r10), vis_read_lo(r10));
	fdyhi = vis_fpmerge(vis_read_hi(r11), vis_read_lo(r11));

	fdx_1 = vis_fpsub16s(vis_one, fdx);
	fdyhi_1 = vis_fpsub16(vis_done, fdyhi);
	fdylo_1 = vis_fpsub16(vis_done, fdylo);

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < count - 1; i++) {

		COMPUTING_POINT_U8_4(vis_fmul8x16au, hi, a);
		COMPUTING_POINT_U8_4(vis_fmul8x16al, lo, b);

		r9 = vis_fpack16_pair(r8hi, r8lo);

		fdx = (*coeffx++);
		fdy = (*coeffy++);

		LOAD_8_PIX_U8_4(a);
		LOAD_8_PIX_U8_4(b);

		r10 = vis_fpmerge(fdy, fdy);
		r11 = vis_fpmerge(vis_read_hi(r10), vis_read_hi(r10));
		r10 = vis_fpmerge(vis_read_lo(r10), vis_read_lo(r10));
		fdylo = vis_fpmerge(vis_read_hi(r10), vis_read_lo(r10));
		fdyhi = vis_fpmerge(vis_read_hi(r11), vis_read_lo(r11));

		fdx_1 = vis_fpsub16s(vis_one, fdx);
		fdyhi_1 = vis_fpsub16(vis_done, fdyhi);
		fdylo_1 = vis_fpsub16(vis_done, fdylo);

		buf[i] = r9;
	}

	COMPUTING_POINT_U8_4(vis_fmul8x16au, hi, a);
	COMPUTING_POINT_U8_4(vis_fmul8x16al, lo, b);

	r9 = vis_fpack16_pair(r8hi, r8lo);
	buf[i] = r9;

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < n; i++) {
		xsrc = xd[i] * 4;
		ysrc = *((mlib_s32 *)buf + i);

		dstData[xsrc] = ysrc >> 24;
		dstData[xsrc + 1] = ysrc >> 16;
		dstData[xsrc + 2] = ysrc >> 8;
		dstData[xsrc + 3] = ysrc;
	}
}

/* *********************************************************** */

#undef VIS_LOAD_OPR
#define	VIS_LOAD_OPR	vis_ld_u16_i

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_S16_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	mlib_u16 *dp = (void *)dstData;
	mlib_s32 i, count, xsrc, ysrc;
	mlib_s32 *xs = pws->src_x;
	mlib_s32 *ys = pws->src_y;
	mlib_s32 *xd = pws->dst_x;
	mlib_s32 stride = lineAddr[1] - lineAddr[0];
	mlib_d64 a0, a1, a2, a3;
	mlib_d64 r0, r1, r2, r3, r4, r5, r6, r7, r8;
	mlib_d64 fdx, fdy, fdx_1, fdy_1;
	mlib_d64 vis_one = vis_to_double_dup(0x7FFF7FFF);
	mlib_d64 *buf = pws->buffer;
	mlib_d64 *coeffx = pws->dsrc_x;
	mlib_d64 *coeffy = pws->dsrc_y;

	if (n <= 0)
		return;

	count = (n >> 2) + ((n & 3) != 0);
	vis_write_gsr(6);

	LOAD_4_PIX_S16_1;

	fdx = *coeffx;
	fdy = *coeffy;
	fdx_1 = vis_fpsub16(vis_one, fdx);
	fdy_1 = vis_fpsub16(vis_one, fdy);

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < count - 1; i++) {

		xs += 4;
		ys += 4;
		coeffx++;
		coeffy++;

		r0 = FPMUL16(a0, fdx_1);
		r1 = FPMUL16(a1, fdx);
		r4 = vis_fpadd16(r0, r1);
		r4 = vis_fpadd16(r4, r4);

		r2 = FPMUL16(a2, fdx);
		r3 = FPMUL16(a3, fdx_1);
		r5 = vis_fpadd16(r2, r3);
		r5 = vis_fpadd16(r5, r5);

		r6 = FPMUL16(r4, fdy_1);
		r7 = FPMUL16(r5, fdy);
		r8 = vis_fpadd16(r6, r7);
		r8 = vis_fpadd16(r8, r8);

		LOAD_4_PIX_S16_1;

		fdx = *coeffx;
		fdy = *coeffy;
		fdx_1 = vis_fpsub16(vis_one, fdx);
		fdy_1 = vis_fpsub16(vis_one, fdy);

		buf[i] = r8;
	}

	r0 = FPMUL16(a0, fdx_1);
	r1 = FPMUL16(a1, fdx);
	r4 = vis_fpadd16(r0, r1);
	r4 = vis_fpadd16(r4, r4);

	r2 = FPMUL16(a2, fdx);
	r3 = FPMUL16(a3, fdx_1);
	r5 = vis_fpadd16(r2, r3);
	r5 = vis_fpadd16(r5, r5);

	r6 = FPMUL16(r4, fdy_1);
	r7 = FPMUL16(r5, fdy);
	r8 = vis_fpadd16(r6, r7);
	r8 = vis_fpadd16(r8, r8);

	buf[i] = r8;

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < n; i++) {
		dp[xd[i]] = *((mlib_u16 *)buf + i);
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_S16_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	mlib_u16 *dp = (void *)dstData;
	mlib_s32 i, count, xsrc, ysrc;
	mlib_s32 *xs = pws->src_x;
	mlib_s32 *ys = pws->src_y;
	mlib_s32 *xd = pws->dst_x;
	mlib_s32 stride = lineAddr[1] - lineAddr[0];
	mlib_d64 a0, a1, a2, a3;
	mlib_d64 r0, r1, r2, r3, r4, r5, r6, r7, r8, r10, r11;
	mlib_d64 fdxhi, fdyhi, fdxhi_1, fdyhi_1;
	mlib_d64 vis_one = vis_to_double_dup(0x7FFF7FFF);
	mlib_d64 *buf = pws->buffer;
	mlib_f32 fdx, fdy;
	mlib_f32 *coeffx = pws->dsrc_x;
	mlib_f32 *coeffy = pws->dsrc_y;

	if (n <= 0)
		return;

	count = (n >> 1) + ((n & 1) != 0);
	vis_write_gsr(6);

	LOAD_4_PIX_S16_2;

	fdx = (*coeffx++);
	fdy = (*coeffy++);

	r10 = vis_fpmerge(fdx, fdy);
	r10 = vis_fpmerge(vis_read_hi(r10), vis_read_lo(r10));
	r11 = vis_fpmerge(vis_read_lo(r10), vis_read_lo(r10));
	r10 = vis_fpmerge(vis_read_hi(r10), vis_read_hi(r10));
	fdxhi = vis_fpmerge(vis_read_hi(r10), vis_read_hi(r11));
	fdyhi = vis_fpmerge(vis_read_lo(r10), vis_read_lo(r11));

	fdxhi_1 = vis_fpsub16(vis_one, fdxhi);
	fdyhi_1 = vis_fpsub16(vis_one, fdyhi);

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < count - 1; i++) {

		xs += 2;
		ys += 2;

		r0 = FPMUL16(a0, fdxhi_1);
		r1 = FPMUL16(a1, fdxhi);
		r4 = vis_fpadd16(r0, r1);
		r4 = vis_fpadd16(r4, r4);

		r2 = FPMUL16(a2, fdxhi);
		r3 = FPMUL16(a3, fdxhi_1);
		r5 = vis_fpadd16(r2, r3);
		r5 = vis_fpadd16(r5, r5);

		r6 = FPMUL16(r4, fdyhi_1);
		r7 = FPMUL16(r5, fdyhi);
		r8 = vis_fpadd16(r6, r7);
		r8 = vis_fpadd16(r8, r8);

		LOAD_4_PIX_S16_2;

		fdx = (*coeffx++);
		fdy = (*coeffy++);

		r10 = vis_fpmerge(fdx, fdy);
		r10 = vis_fpmerge(vis_read_hi(r10), vis_read_lo(r10));
		r11 = vis_fpmerge(vis_read_lo(r10), vis_read_lo(r10));
		r10 = vis_fpmerge(vis_read_hi(r10), vis_read_hi(r10));
		fdxhi = vis_fpmerge(vis_read_hi(r10), vis_read_hi(r11));
		fdyhi = vis_fpmerge(vis_read_lo(r10), vis_read_lo(r11));

		fdxhi_1 = vis_fpsub16(vis_one, fdxhi);
		fdyhi_1 = vis_fpsub16(vis_one, fdyhi);

		buf[i] = r8;
	}

	r0 = FPMUL16(a0, fdxhi_1);
	r1 = FPMUL16(a1, fdxhi);
	r4 = vis_fpadd16(r0, r1);
	r4 = vis_fpadd16(r4, r4);

	r2 = FPMUL16(a2, fdxhi);
	r3 = FPMUL16(a3, fdxhi_1);
	r5 = vis_fpadd16(r2, r3);
	r5 = vis_fpadd16(r5, r5);

	r6 = FPMUL16(r4, fdyhi_1);
	r7 = FPMUL16(r5, fdyhi);
	r8 = vis_fpadd16(r6, r7);
	r8 = vis_fpadd16(r8, r8);
	buf[i] = r8;

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < n; i++) {
		xsrc = xd[i] * 2;
		ysrc = *((mlib_s32 *)buf + i);

		dp[xsrc] = ysrc >> 16;
		dp[xsrc + 1] = ysrc;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_S16_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	const mlib_u8 *line;
	mlib_s32 i, xsrc, ysrc, xdst;
	mlib_s32 *xs = pws->src_x;
	mlib_s32 *ys = pws->src_y;
	mlib_s32 *xd = pws->dst_x;
	mlib_s32 stride = lineAddr[1] - lineAddr[0];
	mlib_d64 *dline;
	mlib_d64 a0, a1, a2, a3;
	mlib_d64 b0, b1, b2, b3;
	mlib_d64 r0hi, r1hi, r2hi, r3hi, r4hi, r5hi, r6hi, r7hi, r8hi;
	mlib_d64 r0lo, r1lo, r2lo, r3lo, r4lo, r5lo, r6lo, r7lo, r8lo;
	mlib_d64 r10, r11;
	mlib_d64 fdxhi, fdyhi, fdxlo, fdylo;
	mlib_d64 fdxhi_1, fdyhi_1, fdxlo_1, fdylo_1;
	mlib_d64 vis_one = vis_to_double_dup(0x7FFF7FFF);
	mlib_f32 fdx, fdy;
	mlib_f32 *coeffx = pws->dsrc_x;
	mlib_f32 *coeffy = pws->dsrc_y;

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < (n >> 1); i++) {

		LOAD_4_PIX_S16_3(a);
		LOAD_4_PIX_S16_3(b);

		fdx = (*coeffx++);
		fdy = (*coeffy++);

		MAKE_COEFF_S16_4(x);
		MAKE_COEFF_S16_4(y);

		COMPUTING_POINT_S16_4(hi, a);
		COMPUTING_POINT_S16_4(lo, b);

		VIS_ALIGNADDR(dline, 6);
		xdst = xd[2 * i] * 6;
		vis_st_u16(r8hi, dstData + xdst + 4);
		r8hi = vis_faligndata(r8hi, r8hi);
		vis_st_u16(r8hi, dstData + xdst + 2);
		r8hi = vis_faligndata(r8hi, r8hi);
		vis_st_u16(r8hi, dstData + xdst + 0);

		xdst = xd[2 * i + 1] * 6;
		vis_st_u16(r8lo, dstData + xdst + 4);
		r8lo = vis_faligndata(r8lo, r8lo);
		vis_st_u16(r8lo, dstData + xdst + 2);
		r8lo = vis_faligndata(r8lo, r8lo);
		vis_st_u16(r8lo, dstData + xdst + 0);
	}

	if (n & 1) {

		LOAD_4_PIX_S16_3(a);

		fdx = (*coeffx++);
		fdy = (*coeffy++);

		r10 = vis_fpmerge(fdx, fdx);
		r11 = vis_fpmerge(vis_read_hi(r10), vis_read_hi(r10));
		fdxhi = vis_fpmerge(vis_read_hi(r11), vis_read_lo(r11));
		fdxhi_1 = vis_fpsub16(vis_one, fdxhi);

		r10 = vis_fpmerge(fdy, fdy);
		r11 = vis_fpmerge(vis_read_hi(r10), vis_read_hi(r10));
		fdyhi = vis_fpmerge(vis_read_hi(r11), vis_read_lo(r11));
		fdyhi_1 = vis_fpsub16(vis_one, fdyhi);

		COMPUTING_POINT_S16_4(hi, a);

		VIS_ALIGNADDR(dline, 6);
		xdst = xd[2 * i] * 6;
		vis_st_u16(r8hi, dstData + xdst + 4);
		r8hi = vis_faligndata(r8hi, r8hi);
		vis_st_u16(r8hi, dstData + xdst + 2);
		r8hi = vis_faligndata(r8hi, r8hi);
		vis_st_u16(r8hi, dstData + xdst + 0);
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_S16_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	const mlib_u8 *line;
	mlib_s32 i, xsrc, ysrc, xdst;
	mlib_s32 *xs = pws->src_x;
	mlib_s32 *ys = pws->src_y;
	mlib_s32 *xd = pws->dst_x;
	mlib_s32 stride = lineAddr[1] - lineAddr[0];
	mlib_d64 *dline;
	mlib_d64 a0, a1, a2, a3;
	mlib_d64 b0, b1, b2, b3;
	mlib_d64 r0hi, r1hi, r2hi, r3hi, r4hi, r5hi, r6hi, r7hi, r8hi;
	mlib_d64 r0lo, r1lo, r2lo, r3lo, r4lo, r5lo, r6lo, r7lo, r8lo;
	mlib_d64 r10, r11;
	mlib_d64 fdxhi, fdyhi, fdxlo, fdylo;
	mlib_d64 fdxhi_1, fdyhi_1, fdxlo_1, fdylo_1;
	mlib_d64 vis_one = vis_to_double_dup(0x7FFF7FFF);
	mlib_f32 fdx, fdy;
	mlib_f32 *coeffx = pws->dsrc_x;
	mlib_f32 *coeffy = pws->dsrc_y;

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < (n >> 1); i++) {

		LOAD_4_PIX_S16_4(a);
		LOAD_4_PIX_S16_4(b);

		fdx = (*coeffx++);
		fdy = (*coeffy++);

		MAKE_COEFF_S16_4(x);
		MAKE_COEFF_S16_4(y);

		COMPUTING_POINT_S16_4(hi, a);
		COMPUTING_POINT_S16_4(lo, b);

		VIS_ALIGNADDR(dline, 6);
		xdst = xd[2 * i] * 8;
		vis_st_u16(r8hi, dstData + xdst + 6);
		r8hi = vis_faligndata(r8hi, r8hi);
		vis_st_u16(r8hi, dstData + xdst + 4);
		r8hi = vis_faligndata(r8hi, r8hi);
		vis_st_u16(r8hi, dstData + xdst + 2);
		r8hi = vis_faligndata(r8hi, r8hi);
		vis_st_u16(r8hi, dstData + xdst + 0);

		xdst = xd[2 * i + 1] * 8;
		vis_st_u16(r8lo, dstData + xdst + 6);
		r8lo = vis_faligndata(r8lo, r8lo);
		vis_st_u16(r8lo, dstData + xdst + 4);
		r8lo = vis_faligndata(r8lo, r8lo);
		vis_st_u16(r8lo, dstData + xdst + 2);
		r8lo = vis_faligndata(r8lo, r8lo);
		vis_st_u16(r8lo, dstData + xdst + 0);
	}

	if (n & 1) {

		LOAD_4_PIX_S16_4(a);

		fdx = (*coeffx++);
		fdy = (*coeffy++);

		r10 = vis_fpmerge(fdx, fdx);
		r11 = vis_fpmerge(vis_read_hi(r10), vis_read_hi(r10));
		fdxhi = vis_fpmerge(vis_read_hi(r11), vis_read_lo(r11));
		fdxhi_1 = vis_fpsub16(vis_one, fdxhi);

		r10 = vis_fpmerge(fdy, fdy);
		r11 = vis_fpmerge(vis_read_hi(r10), vis_read_hi(r10));
		fdyhi = vis_fpmerge(vis_read_hi(r11), vis_read_lo(r11));
		fdyhi_1 = vis_fpsub16(vis_one, fdyhi);

		COMPUTING_POINT_S16_4(hi, a);

		VIS_ALIGNADDR(dline, 6);
		xdst = xd[2 * i] * 8;
		vis_st_u16(r8hi, dstData + xdst + 6);
		r8hi = vis_faligndata(r8hi, r8hi);
		vis_st_u16(r8hi, dstData + xdst + 4);
		r8hi = vis_faligndata(r8hi, r8hi);
		vis_st_u16(r8hi, dstData + xdst + 2);
		r8hi = vis_faligndata(r8hi, r8hi);
		vis_st_u16(r8hi, dstData + xdst + 0);
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U16_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	mlib_u16 *dp = (void *)dstData;
	mlib_s32 i, count, xsrc, ysrc;
	mlib_s32 *xs = pws->src_x;
	mlib_s32 *ys = pws->src_y;
	mlib_s32 *xd = pws->dst_x;
	mlib_s32 stride = lineAddr[1] - lineAddr[0];
	mlib_d64 a0, a1, a2, a3;
	mlib_d64 r0, r1, r2, r3;
	mlib_d64 r4, r5, r6, r7, r8;
	mlib_d64 fdx, fdy, fdx_1, fdy_1;
	mlib_d64 *buf = pws->buffer;
	mlib_d64 *coeffx = pws->dsrc_x;
	mlib_d64 *coeffy = pws->dsrc_y;
	mlib_d64 vis_mone = vis_to_double_dup(0x80008000);
	mlib_d64 vis_one = vis_to_double_dup(0x7FFF7FFF);

	if (n <= 0)
		return;

	count = (n >> 2) + ((n & 3) != 0);
	vis_write_gsr(6);

	LOAD_4_PIX_S16_1;
	fdx = *coeffx;
	fdy = *coeffy;
	fdx_1 = vis_fpsub16(vis_one, fdx);
	fdy_1 = vis_fpsub16(vis_one, fdy);

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < count - 1; i++) {

		xs += 4;
		ys += 4;
		coeffx++;
		coeffy++;

		r0 = FPMULU16(a0, fdx_1);
		r1 = FPMULU16(a1, fdx);
		r4 = vis_fpadd16(r0, r1);
		r4 = vis_fpadd16(r4, r4);

		r2 = FPMULU16(a2, fdx);
		r3 = FPMULU16(a3, fdx_1);
		r5 = vis_fpadd16(r2, r3);
		r5 = vis_fpadd16(r5, r5);

		r6 = FPMUL16(r4, fdy_1);
		r7 = FPMUL16(r5, fdy);
		r8 = vis_fpadd16(r6, r7);
		r8 = vis_fpadd16(r8, r8);
		r8 = vis_fpadd16(r8, vis_mone);

		LOAD_4_PIX_S16_1;
		fdx = *coeffx;
		fdy = *coeffy;
		fdx_1 = vis_fpsub16(vis_one, fdx);
		fdy_1 = vis_fpsub16(vis_one, fdy);

		buf[i] = r8;
	}

	r0 = FPMULU16(a0, fdx_1);
	r1 = FPMULU16(a1, fdx);
	r4 = vis_fpadd16(r0, r1);
	r4 = vis_fpadd16(r4, r4);

	r2 = FPMULU16(a2, fdx);
	r3 = FPMULU16(a3, fdx_1);
	r5 = vis_fpadd16(r2, r3);
	r5 = vis_fpadd16(r5, r5);

	r6 = FPMUL16(r4, fdy_1);
	r7 = FPMUL16(r5, fdy);
	r8 = vis_fpadd16(r6, r7);
	r8 = vis_fpadd16(r8, r8);
	r8 = vis_fpadd16(r8, vis_mone);

	buf[i] = r8;

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < n; i++) {
		dp[xd[i]] = *((mlib_u16 *)buf + i);
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U16_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	mlib_u16 *dp = (void *)dstData;
	mlib_s32 i, count, xsrc, ysrc;
	mlib_s32 *xs = pws->src_x;
	mlib_s32 *ys = pws->src_y;
	mlib_s32 *xd = pws->dst_x;
	mlib_s32 stride = lineAddr[1] - lineAddr[0];
	mlib_d64 a0, a1, a2, a3;
	mlib_d64 r0, r1, r2, r3, r4, r5, r6, r7, r8, r10, r11;
	mlib_d64 fdxhi, fdyhi, fdxhi_1, fdyhi_1;
	mlib_d64 vis_one = vis_to_double_dup(0x7FFF7FFF);
	mlib_d64 vis_mone = vis_to_double_dup(0x80008000);
	mlib_d64 *buf = pws->buffer;
	mlib_f32 fdx, fdy;
	mlib_f32 *coeffx = pws->dsrc_x;
	mlib_f32 *coeffy = pws->dsrc_y;

	if (n <= 0)
		return;

	count = (n >> 1) + ((n & 1) != 0);
	vis_write_gsr(6);

	LOAD_4_PIX_S16_2;

	fdx = (*coeffx++);
	fdy = (*coeffy++);

	r10 = vis_fpmerge(fdx, fdy);
	r10 = vis_fpmerge(vis_read_hi(r10), vis_read_lo(r10));
	r11 = vis_fpmerge(vis_read_lo(r10), vis_read_lo(r10));
	r10 = vis_fpmerge(vis_read_hi(r10), vis_read_hi(r10));
	fdxhi = vis_fpmerge(vis_read_hi(r10), vis_read_hi(r11));
	fdyhi = vis_fpmerge(vis_read_lo(r10), vis_read_lo(r11));

	fdxhi_1 = vis_fpsub16(vis_one, fdxhi);
	fdyhi_1 = vis_fpsub16(vis_one, fdyhi);

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < count - 1; i++) {

		xs += 2;
		ys += 2;

		r0 = FPMULU16(a0, fdxhi_1);
		r1 = FPMULU16(a1, fdxhi);
		r4 = vis_fpadd16(r0, r1);
		r4 = vis_fpadd16(r4, r4);

		r2 = FPMULU16(a2, fdxhi);
		r3 = FPMULU16(a3, fdxhi_1);
		r5 = vis_fpadd16(r2, r3);
		r5 = vis_fpadd16(r5, r5);

		r6 = FPMUL16(r4, fdyhi_1);
		r7 = FPMUL16(r5, fdyhi);
		r8 = vis_fpadd16(r6, r7);
		r8 = vis_fpadd16(r8, r8);
		r8 = vis_fpadd16(r8, vis_mone);

		LOAD_4_PIX_S16_2;

		fdx = (*coeffx++);
		fdy = (*coeffy++);

		r10 = vis_fpmerge(fdx, fdy);
		r10 = vis_fpmerge(vis_read_hi(r10), vis_read_lo(r10));
		r11 = vis_fpmerge(vis_read_lo(r10), vis_read_lo(r10));
		r10 = vis_fpmerge(vis_read_hi(r10), vis_read_hi(r10));
		fdxhi = vis_fpmerge(vis_read_hi(r10), vis_read_hi(r11));
		fdyhi = vis_fpmerge(vis_read_lo(r10), vis_read_lo(r11));

		fdxhi_1 = vis_fpsub16(vis_one, fdxhi);
		fdyhi_1 = vis_fpsub16(vis_one, fdyhi);

		buf[i] = r8;
	}

	r0 = FPMULU16(a0, fdxhi_1);
	r1 = FPMULU16(a1, fdxhi);
	r4 = vis_fpadd16(r0, r1);
	r4 = vis_fpadd16(r4, r4);

	r2 = FPMULU16(a2, fdxhi);
	r3 = FPMULU16(a3, fdxhi_1);
	r5 = vis_fpadd16(r2, r3);
	r5 = vis_fpadd16(r5, r5);

	r6 = FPMUL16(r4, fdyhi_1);
	r7 = FPMUL16(r5, fdyhi);
	r8 = vis_fpadd16(r6, r7);
	r8 = vis_fpadd16(r8, r8);
	r8 = vis_fpadd16(r8, vis_mone);

	buf[i] = r8;

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < n; i++) {
		xsrc = xd[i] * 2;
		ysrc = *((mlib_s32 *)buf + i);

		dp[xsrc] = ysrc >> 16;
		dp[xsrc + 1] = ysrc;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U16_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	const mlib_u8 *line;
	mlib_s32 i, xsrc, ysrc, xdst;
	mlib_s32 *xs = pws->src_x;
	mlib_s32 *ys = pws->src_y;
	mlib_s32 *xd = pws->dst_x;
	mlib_s32 stride = lineAddr[1] - lineAddr[0];
	mlib_d64 *dline;
	mlib_d64 a0, a1, a2, a3;
	mlib_d64 b0, b1, b2, b3;
	mlib_d64 r0hi, r1hi, r2hi, r3hi, r4hi, r5hi, r6hi, r7hi, r8hi;
	mlib_d64 r0lo, r1lo, r2lo, r3lo, r4lo, r5lo, r6lo, r7lo, r8lo;
	mlib_d64 r10, r11;
	mlib_d64 fdxhi, fdyhi, fdxlo, fdylo;
	mlib_d64 fdxhi_1, fdyhi_1, fdxlo_1, fdylo_1;
	mlib_d64 vis_one = vis_to_double_dup(0x7FFF7FFF);
	mlib_d64 vis_mone = vis_to_double_dup(0x80008000);
	mlib_f32 fdx, fdy;
	mlib_f32 *coeffx = pws->dsrc_x;
	mlib_f32 *coeffy = pws->dsrc_y;

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < (n >> 1); i++) {

		LOAD_4_PIX_S16_3(a);
		LOAD_4_PIX_S16_3(b);

		fdx = (*coeffx++);
		fdy = (*coeffy++);

		MAKE_COEFF_S16_4(x);
		MAKE_COEFF_S16_4(y);

		COMPUTING_POINT_U16_4(hi, a);
		COMPUTING_POINT_U16_4(lo, b);

		VIS_ALIGNADDR(dline, 6);
		xdst = xd[2 * i] * 6;
		vis_st_u16(r8hi, dstData + xdst + 4);
		r8hi = vis_faligndata(r8hi, r8hi);
		vis_st_u16(r8hi, dstData + xdst + 2);
		r8hi = vis_faligndata(r8hi, r8hi);
		vis_st_u16(r8hi, dstData + xdst + 0);

		xdst = xd[2 * i + 1] * 6;
		vis_st_u16(r8lo, dstData + xdst + 4);
		r8lo = vis_faligndata(r8lo, r8lo);
		vis_st_u16(r8lo, dstData + xdst + 2);
		r8lo = vis_faligndata(r8lo, r8lo);
		vis_st_u16(r8lo, dstData + xdst + 0);
	}

	if (n & 1) {

		LOAD_4_PIX_S16_3(a);

		fdx = (*coeffx++);
		fdy = (*coeffy++);

		r10 = vis_fpmerge(fdx, fdx);
		r11 = vis_fpmerge(vis_read_hi(r10), vis_read_hi(r10));
		fdxhi = vis_fpmerge(vis_read_hi(r11), vis_read_lo(r11));
		fdxhi_1 = vis_fpsub16(vis_one, fdxhi);

		r10 = vis_fpmerge(fdy, fdy);
		r11 = vis_fpmerge(vis_read_hi(r10), vis_read_hi(r10));
		fdyhi = vis_fpmerge(vis_read_hi(r11), vis_read_lo(r11));
		fdyhi_1 = vis_fpsub16(vis_one, fdyhi);

		COMPUTING_POINT_U16_4(hi, a);

		VIS_ALIGNADDR(dline, 6);
		xdst = xd[2 * i] * 6;
		vis_st_u16(r8hi, dstData + xdst + 4);
		r8hi = vis_faligndata(r8hi, r8hi);
		vis_st_u16(r8hi, dstData + xdst + 2);
		r8hi = vis_faligndata(r8hi, r8hi);
		vis_st_u16(r8hi, dstData + xdst + 0);
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U16_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	const mlib_u8 *line;
	mlib_s32 i, xsrc, ysrc, xdst;
	mlib_s32 *xs = pws->src_x;
	mlib_s32 *ys = pws->src_y;
	mlib_s32 *xd = pws->dst_x;
	mlib_s32 stride = lineAddr[1] - lineAddr[0];
	mlib_d64 *dline;
	mlib_d64 a0, a1, a2, a3;
	mlib_d64 b0, b1, b2, b3;
	mlib_d64 r0hi, r1hi, r2hi, r3hi, r4hi, r5hi, r6hi, r7hi, r8hi;
	mlib_d64 r0lo, r1lo, r2lo, r3lo, r4lo, r5lo, r6lo, r7lo, r8lo;
	mlib_d64 r10, r11;
	mlib_d64 fdxhi, fdyhi, fdxlo, fdylo;
	mlib_d64 fdxhi_1, fdyhi_1, fdxlo_1, fdylo_1;
	mlib_d64 vis_one = vis_to_double_dup(0x7FFF7FFF);
	mlib_f32 fdx, fdy;
	mlib_f32 *coeffx = pws->dsrc_x;
	mlib_f32 *coeffy = pws->dsrc_y;
	mlib_d64 vis_mone = vis_to_double_dup(0x80008000);

#pragma pipeloop(0)
#pragma nomemorydepend
	for (i = 0; i < (n >> 1); i++) {

		LOAD_4_PIX_S16_4(a);
		LOAD_4_PIX_S16_4(b);

		fdx = (*coeffx++);
		fdy = (*coeffy++);

		MAKE_COEFF_S16_4(x);
		MAKE_COEFF_S16_4(y);

		COMPUTING_POINT_U16_4(hi, a);
		COMPUTING_POINT_U16_4(lo, b);

		VIS_ALIGNADDR(dline, 6);
		xdst = xd[2 * i] * 8;
		vis_st_u16(r8hi, dstData + xdst + 6);
		r8hi = vis_faligndata(r8hi, r8hi);
		vis_st_u16(r8hi, dstData + xdst + 4);
		r8hi = vis_faligndata(r8hi, r8hi);
		vis_st_u16(r8hi, dstData + xdst + 2);
		r8hi = vis_faligndata(r8hi, r8hi);
		vis_st_u16(r8hi, dstData + xdst + 0);

		xdst = xd[2 * i + 1] * 8;
		vis_st_u16(r8lo, dstData + xdst + 6);
		r8lo = vis_faligndata(r8lo, r8lo);
		vis_st_u16(r8lo, dstData + xdst + 4);
		r8lo = vis_faligndata(r8lo, r8lo);
		vis_st_u16(r8lo, dstData + xdst + 2);
		r8lo = vis_faligndata(r8lo, r8lo);
		vis_st_u16(r8lo, dstData + xdst + 0);
	}

	if (n & 1) {

		LOAD_4_PIX_S16_4(a);

		fdx = (*coeffx++);
		fdy = (*coeffy++);

		r10 = vis_fpmerge(fdx, fdx);
		r11 = vis_fpmerge(vis_read_hi(r10), vis_read_hi(r10));
		fdxhi = vis_fpmerge(vis_read_hi(r11), vis_read_lo(r11));
		fdxhi_1 = vis_fpsub16(vis_one, fdxhi);

		r10 = vis_fpmerge(fdy, fdy);
		r11 = vis_fpmerge(vis_read_hi(r10), vis_read_hi(r10));
		fdyhi = vis_fpmerge(vis_read_hi(r11), vis_read_lo(r11));
		fdyhi_1 = vis_fpsub16(vis_one, fdyhi);

		COMPUTING_POINT_U16_4(hi, a);

		VIS_ALIGNADDR(dline, 6);
		xdst = xd[2 * i] * 8;
		vis_st_u16(r8hi, dstData + xdst + 6);
		r8hi = vis_faligndata(r8hi, r8hi);
		vis_st_u16(r8hi, dstData + xdst + 4);
		r8hi = vis_faligndata(r8hi, r8hi);
		vis_st_u16(r8hi, dstData + xdst + 2);
		r8hi = vis_faligndata(r8hi, r8hi);
		vis_st_u16(r8hi, dstData + xdst + 0);
	}
}

/* *********************************************************** */
