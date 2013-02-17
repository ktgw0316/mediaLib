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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_i_ImagePolynomialWarp_BL.c	9.3	07/11/05 SMI"

#ifdef _MSC_VER
#pragma optimize("", off)
#endif /* _MSC_VER */

#ifndef	_NO_LONGLONG

#include <mlib_image.h>
#include <mlib_ImagePolynomialWarp.h>
#include <mlib_GeomBasicIntOp.h>

/* *********************************************************** */

#undef	SHIFT
#define	SHIFT	15
#define	DTOLL(l, d)	BASIC_DTOLL(l, d, SHIFT)

/* *********************************************************** */

/* #define	TABLE(X)	mlib_U82D64[X] */
#define	TABLE(X)	(X)
#define	TYPE_FMT	mlib_u8

/* *********************************************************** */

#if 1
#define	GET_POINT(N)                                            \
	xsrc = (*xs++);                                         \
	ysrc = (*ys++);                                         \
	xdst = (*xd++);                                         \
	pix = (TYPE_FMT *) (lineAddr[ysrc] + xsrc * N);         \
	pix1 = (TYPE_FMT *) ((mlib_u8 *)pix + stride);          \
	pix2 = (TYPE_FMT *) (dstData + xdst * N);               \
	DTOLL(dx, *dxs);                                        \
	dxs ++;                                                 \
	DTOLL(dy, *dys);                                        \
	dys ++
#else
#define	GET_POINT(N)                                            \
	xsrc = (*xs++);                                         \
	ysrc = (*ys++);                                         \
	xdst = (*xd++);                                         \
	pix = (TYPE_FMT *) (lineAddr[ysrc] + xsrc * N);         \
	pix1 = (TYPE_FMT *) ((mlib_u8 *)pix + stride);          \
	pix2 = (TYPE_FMT *) (dstData + xdst * N);               \
	dx = (*dxs++);                                          \
	dy = (*dys++)
#endif

/* *********************************************************** */

#define	LOAD_PIX(idx, idx0, idx1)                               \
	a0##idx = TABLE(pix[idx0]);                             \
	a1##idx = TABLE(pix[idx1]);                             \
	a2##idx = TABLE(pix1[idx1]);                            \
	a3##idx = TABLE(pix1[idx0])

/* *********************************************************** */

#if 0
#define	CALC(idx)                                               \
	r0##idx = a0##idx + (a3##idx - a0##idx) * dy;           \
	r1##idx = a1##idx + (a2##idx - a1##idx) * dy;           \
	r2##idx = r0##idx + (r1##idx - r0##idx) * dx
#else
#define	CALC(idx)                                                          \
	r0##idx = (a0##idx << SHIFT) + (a3##idx - a0##idx) * dy;           \
	r1##idx = (a1##idx << SHIFT) + (a2##idx - a1##idx) * dy;           \
	r2##idx = (r0##idx << SHIFT) + (r1##idx - r0##idx) * dx
#endif

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U8_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT * pix, *pix1, *pix2;
#if 0
	mlib_d64
	    *dxs = pws->dsrc_x,
	    *dys = pws->dsrc_y, a00, a10, a20, a30, r00, r10, r20, dx, dy;
#else
	mlib_d64
	    *dxs = pws->dsrc_x,
	    *dys = pws->dsrc_y;
	mlib_s64 a00, a10, a20, a30, r00, r10, r20, dx, dy;
#endif
	mlib_s32
	    i,
	    xsrc, ysrc, xdst,
	    stride = lineAddr[1] - lineAddr[0],
	    *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		GET_POINT(1);
		LOAD_PIX(0, 0, 1);
		CALC(0);
		/* pix2[0] = (mlib_u8)r20; */
		pix2[0] = (mlib_u8)(r20>>(SHIFT+SHIFT));
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U8_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT * pix, *pix1, *pix2;
	mlib_d64
	    *dxs = pws->dsrc_x,
	    *dys = pws->dsrc_y;
	mlib_s64
	    a00, a10, a20, a30, r00, r10, r20,
	    a01, a11, a21, a31, r01, r11, r21, dx, dy;
	mlib_s32
	    i,
	    xsrc, ysrc, xdst,
	    stride = lineAddr[1] - lineAddr[0],
	    *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		GET_POINT(2);
		LOAD_PIX(0, 0, 2);
		LOAD_PIX(1, 1, 3);
		CALC(0);
		CALC(1);
		pix2[0] = (mlib_u8)(r20>>(SHIFT+SHIFT));
		pix2[1] = (mlib_u8)(r21>>(SHIFT+SHIFT));
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U8_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT * pix, *pix1, *pix2;
	mlib_d64
	    *dxs = pws->dsrc_x,
	    *dys = pws->dsrc_y;
	mlib_s64
	    a00, a10, a20, a30, r00, r10, r20,
	    a01, a11, a21, a31, r01, r11, r21,
	    a02, a12, a22, a32, r02, r12, r22, dx, dy;
	mlib_s32
	    i,
	    xsrc, ysrc, xdst,
	    stride = lineAddr[1] - lineAddr[0],
	    *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		GET_POINT(3);
		LOAD_PIX(0, 0, 3);
		LOAD_PIX(1, 1, 4);
		LOAD_PIX(2, 2, 5);
		CALC(0);
		CALC(1);
		CALC(2);
		pix2[0] = (mlib_u8)(r20>>(SHIFT+SHIFT));
		pix2[1] = (mlib_u8)(r21>>(SHIFT+SHIFT));
		pix2[2] = (mlib_u8)(r22>>(SHIFT+SHIFT));
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U8_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT * pix, *pix1, *pix2;
	mlib_d64
	    *dxs = pws->dsrc_x,
	    *dys = pws->dsrc_y;
	mlib_s64
	    a00, a10, a20, a30, r00, r10, r20,
	    a01, a11, a21, a31, r01, r11, r21,
	    a02, a12, a22, a32, r02, r12, r22,
	    a03, a13, a23, a33, r03, r13, r23, dx, dy;
	mlib_s32
	    i,
	    xsrc, ysrc, xdst,
	    stride = lineAddr[1] - lineAddr[0],
	    *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		GET_POINT(4);
		LOAD_PIX(0, 0, 4);
		LOAD_PIX(1, 1, 5);
		LOAD_PIX(2, 2, 6);
		LOAD_PIX(3, 3, 7);
		CALC(0);
		CALC(1);
		CALC(2);
		CALC(3);
		pix2[0] = (mlib_u8)(r20>>(SHIFT+SHIFT));
		pix2[1] = (mlib_u8)(r21>>(SHIFT+SHIFT));
		pix2[2] = (mlib_u8)(r22>>(SHIFT+SHIFT));
		pix2[3] = (mlib_u8)(r23>>(SHIFT+SHIFT));
	}
}

/* *********************************************************** */

#undef	TABLE
#undef	TYPE_FMT

/* *********************************************************** */

#define	TABLE(A)	(A)
#define	TYPE_FMT	mlib_s16

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_S16_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT * line;
	mlib_d64
	    *dxs = pws->dsrc_x, *dys = pws->dsrc_y;
	mlib_s64 dx, dy, dxdy, r00, res00;
	mlib_s32
	    i, xsrc, ysrc, xdst,
	    a00, a10, a20, a30, r01,
	    stride = (lineAddr[1] - lineAddr[0]) >> 1,
	    *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

	if (n <= 0)
		return;
	n--;

	xsrc = (*xs++);
	ysrc = (*ys++);
#if 0
	dx = (*dxs++);
	dy = (*dys++);
#else
	DTOLL(dx, *dxs);
	dxs ++;
	DTOLL(dy, *dys);
	dys ++;
#endif
	xdst = (*xd++);

	line = (TYPE_FMT *) lineAddr[ysrc] + xsrc;
	a00 = line[0];
	a10 = line[1];
	a20 = line[stride + 1];
	a30 = line[stride];

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {

		dxdy = dx * dy;
		res00 = (a10 - a00) * dx + (a30 - a00) * dy;
		/* r00 = res00 + (a00 - a10 + a20 - a30) * dxdy; */
		r00 = (res00<<SHIFT) + (a00 - a10 + a20 - a30) * dxdy;

		xsrc = (*xs++);
		ysrc = (*ys++);
#if 0
		dx = (*dxs++);
		dy = (*dys++);
#else
		DTOLL(dx, *dxs);
		dxs ++;
		DTOLL(dy, *dys);
		dys ++;
#endif

		/* r01 = (mlib_s32)r00 + a00; */
		r01 = (mlib_s32)(r00>>(SHIFT+SHIFT)) + a00;
		line = (TYPE_FMT *) lineAddr[ysrc] + xsrc;
		a00 = line[0];
		a10 = line[1];
		a20 = line[stride + 1];
		a30 = line[stride];
		*((TYPE_FMT *) dstData + xdst) = r01;
		xdst = (*xd++);
	}

	dxdy = dx * dy;
	res00 = (a10 - a00) * dx + (a30 - a00) * dy;
	/* r00 = res00 + (a00 - a10 + a20 - a30) * dxdy; */
	r00 = (res00<<SHIFT) + (a00 - a10 + a20 - a30) * dxdy;
	/* r01 = (mlib_s32)r00 + a00; */
	r01 = (mlib_s32)(r00>>(SHIFT+SHIFT)) + a00;
	*((TYPE_FMT *) dstData + xdst) = r01;
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_S16_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT * pix, *pix1;
	mlib_d64
	    *dxs = pws->dsrc_x,
	    *dys = pws->dsrc_y;
	mlib_s64 r00, r10, res00, r01, r11, res01, dx, dy, dxdy;
	mlib_s32
	    i,
	    a00, a10, a20, a30,
	    a01, a11, a21, a31,
	    xsrc, ysrc, xdst,
	    stride = (lineAddr[1] - lineAddr[0]) >> 1,
	    *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {

		xsrc = (*xs++);
		ysrc = (*ys++);
		xdst = (*xd++);
		DTOLL(dx, *dxs);
		dxs ++;
		DTOLL(dy, *dys);
		dys ++;

		pix = (TYPE_FMT *) lineAddr[ysrc] + xsrc * 2;
		pix1 = (TYPE_FMT *) dstData + xdst * 2;

		a00 = pix[0];
		a01 = pix[1];
		a10 = pix[2];
		a11 = pix[3];
		a30 = pix[stride];
		a31 = pix[stride + 1];
		a20 = pix[stride + 2];
		a21 = pix[stride + 3];

		dxdy = dx * dy;
		res00 = (a10 - a00) * dx + (a30 - a00) * dy;
		r00 = (res00<<SHIFT) + (a00 - a10 + a20 - a30) * dxdy;
		r10 = (mlib_s32)(r00>>(SHIFT+SHIFT)) + a00;

		res01 = (a11 - a01) * dx + (a31 - a01) * dy;
		r01 = (res01<<SHIFT) + (a01 - a11 + a21 - a31) * dxdy;
		r11 = (mlib_s32)(r01>>(SHIFT+SHIFT)) + a01;

		pix1[0] = (mlib_s16)r10;
		pix1[1] = (mlib_s16)r11;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_S16_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT * pix, *pix1, *pix2;
	mlib_d64
	    *dxs = pws->dsrc_x,
	    *dys = pws->dsrc_y;
	mlib_s64
	    a00, a10, a20, a30, r00, r10, r20,
	    a01, a11, a21, a31, r01, r11, r21,
	    a02, a12, a22, a32, r02, r12, r22, dx, dy;
	mlib_s32
	    i,
	    xsrc, ysrc, xdst,
	    stride = lineAddr[1] - lineAddr[0],
	    *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		GET_POINT(6);
		LOAD_PIX(0, 0, 3);
		LOAD_PIX(1, 1, 4);
		LOAD_PIX(2, 2, 5);
		CALC(0);
		CALC(1);
		CALC(2);
		pix2[0] = (mlib_s16)(r20>>(SHIFT+SHIFT));
		pix2[1] = (mlib_s16)(r21>>(SHIFT+SHIFT));
		pix2[2] = (mlib_s16)(r22>>(SHIFT+SHIFT));
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_S16_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT * pix, *pix1, *pix2;
	mlib_d64
	    *dxs = pws->dsrc_x,
	    *dys = pws->dsrc_y;
	mlib_s64
	    a00, a10, a20, a30, r00, r10, r20,
	    a01, a11, a21, a31, r01, r11, r21,
	    a02, a12, a22, a32, r02, r12, r22,
	    a03, a13, a23, a33, r03, r13, r23, dx, dy;
	mlib_s32
	    i,
	    xsrc, ysrc, xdst,
	    stride = lineAddr[1] - lineAddr[0],
	    *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		GET_POINT(8);
		LOAD_PIX(0, 0, 4);
		LOAD_PIX(1, 1, 5);
		LOAD_PIX(2, 2, 6);
		LOAD_PIX(3, 3, 7);
		CALC(0);
		CALC(1);
		CALC(2);
		CALC(3);
		pix2[0] = (mlib_s16)(r20>>(SHIFT+SHIFT));
		pix2[1] = (mlib_s16)(r21>>(SHIFT+SHIFT));
		pix2[2] = (mlib_s16)(r22>>(SHIFT+SHIFT));
		pix2[3] = (mlib_s16)(r23>>(SHIFT+SHIFT));
	}
}

/* *********************************************************** */

#undef	TYPE_FMT

#define	TYPE_FMT	mlib_u16

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U16_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT * line;
	mlib_d64
	    *dxs = pws->dsrc_x, *dys = pws->dsrc_y;
	mlib_s64 dx, dy, dxdy, r00, res00;
	mlib_s32
	    i, xsrc, ysrc, xdst,
	    a00, a10, a20, a30, r01,
	    stride = (lineAddr[1] - lineAddr[0]) >> 1,
	    *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

	if (n <= 0)
		return;
	n--;

	xsrc = (*xs++);
	ysrc = (*ys++);
	DTOLL(dx, *dxs);
	dxs ++;
	DTOLL(dy, *dys);
	dys ++;
	xdst = (*xd++);

	line = (TYPE_FMT *) lineAddr[ysrc] + xsrc;
	a00 = line[0];
	a10 = line[1];
	a20 = line[stride + 1];
	a30 = line[stride];

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {

		dxdy = dx * dy;
		res00 = (a10 - a00) * dx + (a30 - a00) * dy;
		r00 = (res00<<SHIFT) + (a00 - a10 + a20 - a30) * dxdy;

		xsrc = (*xs++);
		ysrc = (*ys++);
		DTOLL(dx, *dxs);
		dxs ++;
		DTOLL(dy, *dys);
		dys ++;

		r01 = (mlib_s32)(r00>>(SHIFT+SHIFT)) + a00;
		line = (TYPE_FMT *) lineAddr[ysrc] + xsrc;
		a00 = line[0];
		a10 = line[1];
		a20 = line[stride + 1];
		a30 = line[stride];
		*((TYPE_FMT *) dstData + xdst) = r01;
		xdst = (*xd++);
	}

	dxdy = dx * dy;
	res00 = (a10 - a00) * dx + (a30 - a00) * dy;
	r00 = (res00<<SHIFT) + (a00 - a10 + a20 - a30) * dxdy;
	r01 = (mlib_s32)(r00>>(SHIFT+SHIFT)) + a00;
	*((TYPE_FMT *) dstData + xdst) = r01;
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U16_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT * pix, *pix1;
	mlib_d64
	    *dxs = pws->dsrc_x,
	    *dys = pws->dsrc_y;
	mlib_s64 r00, r10, res00, r01, r11, res01, dx, dy, dxdy;
	mlib_s32
	    i,
	    a00, a10, a20, a30,
	    a01, a11, a21, a31,
	    xsrc, ysrc, xdst,
	    stride = (lineAddr[1] - lineAddr[0]) >> 1,
	    *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {

		xsrc = (*xs++);
		ysrc = (*ys++);
		xdst = (*xd++);
		DTOLL(dx, *dxs);
		dxs ++;
		DTOLL(dy, *dys);
		dys ++;

		pix = (TYPE_FMT *) lineAddr[ysrc] + xsrc * 2;
		pix1 = (TYPE_FMT *) dstData + xdst * 2;

		a00 = pix[0];
		a01 = pix[1];
		a10 = pix[2];
		a11 = pix[3];
		a30 = pix[stride];
		a31 = pix[stride + 1];
		a20 = pix[stride + 2];
		a21 = pix[stride + 3];

		dxdy = dx * dy;
		res00 = (a10 - a00) * dx + (a30 - a00) * dy;
		r00 = (res00<<SHIFT) + (a00 - a10 + a20 - a30) * dxdy;
		r10 = (mlib_s32)(r00>>(SHIFT+SHIFT)) + a00;

		res01 = (a11 - a01) * dx + (a31 - a01) * dy;
		r01 = (res01<<SHIFT) + (a01 - a11 + a21 - a31) * dxdy;
		r11 = (mlib_s32)(r01>>(SHIFT+SHIFT)) + a01;

		pix1[0] = (mlib_u16)r10;
		pix1[1] = (mlib_u16)r11;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U16_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT * pix, *pix1, *pix2;
	mlib_d64
	    *dxs = pws->dsrc_x,
	    *dys = pws->dsrc_y;
	mlib_s64
	    a00, a10, a20, a30, r00, r10, r20,
	    a01, a11, a21, a31, r01, r11, r21,
	    a02, a12, a22, a32, r02, r12, r22, dx, dy;
	mlib_s32
	    i,
	    xsrc, ysrc, xdst,
	    stride = lineAddr[1] - lineAddr[0],
	    *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		GET_POINT(6);
		LOAD_PIX(0, 0, 3);
		LOAD_PIX(1, 1, 4);
		LOAD_PIX(2, 2, 5);
		CALC(0);
		CALC(1);
		CALC(2);
		pix2[0] = (mlib_u16)(r20>>(SHIFT+SHIFT));
		pix2[1] = (mlib_u16)(r21>>(SHIFT+SHIFT));
		pix2[2] = (mlib_u16)(r22>>(SHIFT+SHIFT));
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U16_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT * pix, *pix1, *pix2;
	mlib_d64
	    *dxs = pws->dsrc_x,
	    *dys = pws->dsrc_y;
	mlib_s64
	    a00, a10, a20, a30, r00, r10, r20,
	    a01, a11, a21, a31, r01, r11, r21,
	    a02, a12, a22, a32, r02, r12, r22,
	    a03, a13, a23, a33, r03, r13, r23, dx, dy;
	mlib_s32
	    i,
	    xsrc, ysrc, xdst,
	    stride = lineAddr[1] - lineAddr[0],
	    *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		GET_POINT(8);
		LOAD_PIX(0, 0, 4);
		LOAD_PIX(1, 1, 5);
		LOAD_PIX(2, 2, 6);
		LOAD_PIX(3, 3, 7);
		CALC(0);
		CALC(1);
		CALC(2);
		CALC(3);
		pix2[0] = (mlib_u16)(r20>>(SHIFT+SHIFT));
		pix2[1] = (mlib_u16)(r21>>(SHIFT+SHIFT));
		pix2[2] = (mlib_u16)(r22>>(SHIFT+SHIFT));
		pix2[3] = (mlib_u16)(r23>>(SHIFT+SHIFT));
	}
}

#else /* _NO_LONGLONG */

#include <mlib_image.h>
#include <mlib_ImagePolynomialWarp.h>

/* *********************************************************** */

#define	TABLE(X)	mlib_U82D64[X]
#define	TYPE_FMT	mlib_u8

/* *********************************************************** */

#define	GET_POINT(N)                                            \
	xsrc = (*xs++);                                         \
	ysrc = (*ys++);                                         \
	xdst = (*xd++);                                         \
	pix = (TYPE_FMT *) (lineAddr[ysrc] + xsrc * N);         \
	pix1 = (TYPE_FMT *) ((mlib_u8 *)pix + stride);          \
	pix2 = (TYPE_FMT *) (dstData + xdst * N);               \
	dx = (*dxs++);                                          \
	dy = (*dys++)

/* *********************************************************** */

#define	LOAD_PIX(idx, idx0, idx1)                               \
	a0##idx = TABLE(pix[idx0]);                             \
	a1##idx = TABLE(pix[idx1]);                             \
	a2##idx = TABLE(pix1[idx1]);                            \
	a3##idx = TABLE(pix1[idx0])

/* *********************************************************** */

#define	CALC(idx)                                               \
	r0##idx = a0##idx + (a3##idx - a0##idx) * dy;           \
	r1##idx = a1##idx + (a2##idx - a1##idx) * dy;           \
	r2##idx = r0##idx + (r1##idx - r0##idx) * dx

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U8_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT * pix, *pix1, *pix2;
	mlib_d64
	    *dxs = pws->dsrc_x,
	    *dys = pws->dsrc_y, a00, a10, a20, a30, r00, r10, r20, dx, dy;
	mlib_s32
	    i,
	    xsrc, ysrc, xdst,
	    stride = lineAddr[1] - lineAddr[0],
	    *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		GET_POINT(1);
		LOAD_PIX(0, 0, 1);
		CALC(0);
		pix2[0] = (mlib_u8)r20;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U8_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT * pix, *pix1, *pix2;
	mlib_d64
	    *dxs = pws->dsrc_x,
	    *dys = pws->dsrc_y,
	    a00, a10, a20, a30, r00, r10, r20,
	    a01, a11, a21, a31, r01, r11, r21, dx, dy;
	mlib_s32
	    i,
	    xsrc, ysrc, xdst,
	    stride = lineAddr[1] - lineAddr[0],
	    *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		GET_POINT(2);
		LOAD_PIX(0, 0, 2);
		LOAD_PIX(1, 1, 3);
		CALC(0);
		CALC(1);
		pix2[0] = (mlib_u8)r20;
		pix2[1] = (mlib_u8)r21;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U8_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT * pix, *pix1, *pix2;
	mlib_d64
	    *dxs = pws->dsrc_x,
	    *dys = pws->dsrc_y,
	    a00, a10, a20, a30, r00, r10, r20,
	    a01, a11, a21, a31, r01, r11, r21,
	    a02, a12, a22, a32, r02, r12, r22, dx, dy;
	mlib_s32
	    i,
	    xsrc, ysrc, xdst,
	    stride = lineAddr[1] - lineAddr[0],
	    *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		GET_POINT(3);
		LOAD_PIX(0, 0, 3);
		LOAD_PIX(1, 1, 4);
		LOAD_PIX(2, 2, 5);
		CALC(0);
		CALC(1);
		CALC(2);
		pix2[0] = (mlib_u8)r20;
		pix2[1] = (mlib_u8)r21;
		pix2[2] = (mlib_u8)r22;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U8_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT * pix, *pix1, *pix2;
	mlib_d64
	    *dxs = pws->dsrc_x,
	    *dys = pws->dsrc_y,
	    a00, a10, a20, a30, r00, r10, r20,
	    a01, a11, a21, a31, r01, r11, r21,
	    a02, a12, a22, a32, r02, r12, r22,
	    a03, a13, a23, a33, r03, r13, r23, dx, dy;
	mlib_s32
	    i,
	    xsrc, ysrc, xdst,
	    stride = lineAddr[1] - lineAddr[0],
	    *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		GET_POINT(4);
		LOAD_PIX(0, 0, 4);
		LOAD_PIX(1, 1, 5);
		LOAD_PIX(2, 2, 6);
		LOAD_PIX(3, 3, 7);
		CALC(0);
		CALC(1);
		CALC(2);
		CALC(3);
		pix2[0] = (mlib_u8)r20;
		pix2[1] = (mlib_u8)r21;
		pix2[2] = (mlib_u8)r22;
		pix2[3] = (mlib_u8)r23;
	}
}

/* *********************************************************** */

#undef	TABLE
#undef	TYPE_FMT

/* *********************************************************** */

#define	TABLE(A)	(A)
#define	TYPE_FMT	mlib_s16

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_S16_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT * line;
	mlib_d64
	    *dxs = pws->dsrc_x, *dys = pws->dsrc_y, dx, dy, dxdy, r00, res00;
	mlib_s32
	    i, xsrc, ysrc, xdst,
	    a00, a10, a20, a30, r01,
	    stride = (lineAddr[1] - lineAddr[0]) >> 1,
	    *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

	if (n <= 0)
		return;
	n--;

	xsrc = (*xs++);
	ysrc = (*ys++);
	dx = (*dxs++);
	dy = (*dys++);
	xdst = (*xd++);

	line = (TYPE_FMT *) lineAddr[ysrc] + xsrc;
	a00 = line[0];
	a10 = line[1];
	a20 = line[stride + 1];
	a30 = line[stride];

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {

		dxdy = dx * dy;
		res00 = (a10 - a00) * dx + (a30 - a00) * dy;
		r00 = res00 + (a00 - a10 + a20 - a30) * dxdy;

		xsrc = (*xs++);
		ysrc = (*ys++);
		dx = (*dxs++);
		dy = (*dys++);

		r01 = (mlib_s32)r00 + a00;
		line = (TYPE_FMT *) lineAddr[ysrc] + xsrc;
		a00 = line[0];
		a10 = line[1];
		a20 = line[stride + 1];
		a30 = line[stride];
		*((TYPE_FMT *) dstData + xdst) = r01;
		xdst = (*xd++);
	}

	dxdy = dx * dy;
	res00 = (a10 - a00) * dx + (a30 - a00) * dy;
	r00 = res00 + (a00 - a10 + a20 - a30) * dxdy;
	r01 = (mlib_s32)r00 + a00;
	*((TYPE_FMT *) dstData + xdst) = r01;
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_S16_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT * pix, *pix1;
	mlib_d64
	    *dxs = pws->dsrc_x,
	    *dys = pws->dsrc_y, r00, r10, res00, r01, r11, res01, dx, dy, dxdy;
	mlib_s32
	    i,
	    a00, a10, a20, a30,
	    a01, a11, a21, a31,
	    xsrc, ysrc, xdst,
	    stride = (lineAddr[1] - lineAddr[0]) >> 1,
	    *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {

		xsrc = (*xs++);
		ysrc = (*ys++);
		xdst = (*xd++);
		dx = (*dxs++);
		dy = (*dys++);

		pix = (TYPE_FMT *) lineAddr[ysrc] + xsrc * 2;
		pix1 = (TYPE_FMT *) dstData + xdst * 2;

		a00 = pix[0];
		a01 = pix[1];
		a10 = pix[2];
		a11 = pix[3];
		a30 = pix[stride];
		a31 = pix[stride + 1];
		a20 = pix[stride + 2];
		a21 = pix[stride + 3];

		dxdy = dx * dy;
		res00 = (a10 - a00) * dx + (a30 - a00) * dy;
		r00 = res00 + (a00 - a10 + a20 - a30) * dxdy;
		r10 = (mlib_s32)r00 + a00;

		res01 = (a11 - a01) * dx + (a31 - a01) * dy;
		r01 = res01 + (a01 - a11 + a21 - a31) * dxdy;
		r11 = (mlib_s32)r01 + a01;

		pix1[0] = (mlib_s16)r10;
		pix1[1] = (mlib_s16)r11;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_S16_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT * pix, *pix1, *pix2;
	mlib_d64
	    *dxs = pws->dsrc_x,
	    *dys = pws->dsrc_y,
	    a00, a10, a20, a30, r00, r10, r20,
	    a01, a11, a21, a31, r01, r11, r21,
	    a02, a12, a22, a32, r02, r12, r22, dx, dy;
	mlib_s32
	    i,
	    xsrc, ysrc, xdst,
	    stride = lineAddr[1] - lineAddr[0],
	    *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		GET_POINT(6);
		LOAD_PIX(0, 0, 3);
		LOAD_PIX(1, 1, 4);
		LOAD_PIX(2, 2, 5);
		CALC(0);
		CALC(1);
		CALC(2);
		pix2[0] = (mlib_s16)r20;
		pix2[1] = (mlib_s16)r21;
		pix2[2] = (mlib_s16)r22;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_S16_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT * pix, *pix1, *pix2;
	mlib_d64
	    *dxs = pws->dsrc_x,
	    *dys = pws->dsrc_y,
	    a00, a10, a20, a30, r00, r10, r20,
	    a01, a11, a21, a31, r01, r11, r21,
	    a02, a12, a22, a32, r02, r12, r22,
	    a03, a13, a23, a33, r03, r13, r23, dx, dy;
	mlib_s32
	    i,
	    xsrc, ysrc, xdst,
	    stride = lineAddr[1] - lineAddr[0],
	    *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		GET_POINT(8);
		LOAD_PIX(0, 0, 4);
		LOAD_PIX(1, 1, 5);
		LOAD_PIX(2, 2, 6);
		LOAD_PIX(3, 3, 7);
		CALC(0);
		CALC(1);
		CALC(2);
		CALC(3);
		pix2[0] = (mlib_s16)r20;
		pix2[1] = (mlib_s16)r21;
		pix2[2] = (mlib_s16)r22;
		pix2[3] = (mlib_s16)r23;
	}
}

/* *********************************************************** */

#undef	TYPE_FMT

#define	TYPE_FMT	mlib_u16

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U16_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT * line;
	mlib_d64
	    *dxs = pws->dsrc_x, *dys = pws->dsrc_y, dx, dy, dxdy, r00, res00;
	mlib_s32
	    i, xsrc, ysrc, xdst,
	    a00, a10, a20, a30, r01,
	    stride = (lineAddr[1] - lineAddr[0]) >> 1,
	    *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

	if (n <= 0)
		return;
	n--;

	xsrc = (*xs++);
	ysrc = (*ys++);
	dx = (*dxs++);
	dy = (*dys++);
	xdst = (*xd++);

	line = (TYPE_FMT *) lineAddr[ysrc] + xsrc;
	a00 = line[0];
	a10 = line[1];
	a20 = line[stride + 1];
	a30 = line[stride];

#ifdef __SUNPRO_C
#pragma nomemorydepend
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {

		dxdy = dx * dy;
		res00 = (a10 - a00) * dx + (a30 - a00) * dy;
		r00 = res00 + (a00 - a10 + a20 - a30) * dxdy;

		xsrc = (*xs++);
		ysrc = (*ys++);
		dx = (*dxs++);
		dy = (*dys++);

		r01 = (mlib_s32)r00 + a00;
		line = (TYPE_FMT *) lineAddr[ysrc] + xsrc;
		a00 = line[0];
		a10 = line[1];
		a20 = line[stride + 1];
		a30 = line[stride];
		*((TYPE_FMT *) dstData + xdst) = r01;
		xdst = (*xd++);
	}

	dxdy = dx * dy;
	res00 = (a10 - a00) * dx + (a30 - a00) * dy;
	r00 = res00 + (a00 - a10 + a20 - a30) * dxdy;
	r01 = (mlib_s32)r00 + a00;
	*((TYPE_FMT *) dstData + xdst) = r01;
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U16_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT * pix, *pix1;
	mlib_d64
	    *dxs = pws->dsrc_x,
	    *dys = pws->dsrc_y, r00, r10, res00, r01, r11, res01, dx, dy, dxdy;
	mlib_s32
	    i,
	    a00, a10, a20, a30,
	    a01, a11, a21, a31,
	    xsrc, ysrc, xdst,
	    stride = (lineAddr[1] - lineAddr[0]) >> 1,
	    *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {

		xsrc = (*xs++);
		ysrc = (*ys++);
		xdst = (*xd++);
		dx = (*dxs++);
		dy = (*dys++);

		pix = (TYPE_FMT *) lineAddr[ysrc] + xsrc * 2;
		pix1 = (TYPE_FMT *) dstData + xdst * 2;

		a00 = pix[0];
		a01 = pix[1];
		a10 = pix[2];
		a11 = pix[3];
		a30 = pix[stride];
		a31 = pix[stride + 1];
		a20 = pix[stride + 2];
		a21 = pix[stride + 3];

		dxdy = dx * dy;
		res00 = (a10 - a00) * dx + (a30 - a00) * dy;
		r00 = res00 + (a00 - a10 + a20 - a30) * dxdy;
		r10 = (mlib_s32)r00 + a00;

		res01 = (a11 - a01) * dx + (a31 - a01) * dy;
		r01 = res01 + (a01 - a11 + a21 - a31) * dxdy;
		r11 = (mlib_s32)r01 + a01;

		pix1[0] = (mlib_u16)r10;
		pix1[1] = (mlib_u16)r11;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U16_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT * pix, *pix1, *pix2;
	mlib_d64
	    *dxs = pws->dsrc_x,
	    *dys = pws->dsrc_y,
	    a00, a10, a20, a30, r00, r10, r20,
	    a01, a11, a21, a31, r01, r11, r21,
	    a02, a12, a22, a32, r02, r12, r22, dx, dy;
	mlib_s32
	    i,
	    xsrc, ysrc, xdst,
	    stride = lineAddr[1] - lineAddr[0],
	    *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		GET_POINT(6);
		LOAD_PIX(0, 0, 3);
		LOAD_PIX(1, 1, 4);
		LOAD_PIX(2, 2, 5);
		CALC(0);
		CALC(1);
		CALC(2);
		pix2[0] = (mlib_u16)r20;
		pix2[1] = (mlib_u16)r21;
		pix2[2] = (mlib_u16)r22;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_U16_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT * pix, *pix1, *pix2;
	mlib_d64
	    *dxs = pws->dsrc_x,
	    *dys = pws->dsrc_y,
	    a00, a10, a20, a30, r00, r10, r20,
	    a01, a11, a21, a31, r01, r11, r21,
	    a02, a12, a22, a32, r02, r12, r22,
	    a03, a13, a23, a33, r03, r13, r23, dx, dy;
	mlib_s32
	    i,
	    xsrc, ysrc, xdst,
	    stride = lineAddr[1] - lineAddr[0],
	    *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		GET_POINT(8);
		LOAD_PIX(0, 0, 4);
		LOAD_PIX(1, 1, 5);
		LOAD_PIX(2, 2, 6);
		LOAD_PIX(3, 3, 7);
		CALC(0);
		CALC(1);
		CALC(2);
		CALC(3);
		pix2[0] = (mlib_u16)r20;
		pix2[1] = (mlib_u16)r21;
		pix2[2] = (mlib_u16)r22;
		pix2[3] = (mlib_u16)r23;
	}
}

#endif /* _NO_LONGLONG */

/* *********************************************************** */

#ifdef _MSC_VER
#pragma optimize("", on)
#endif /* _MSC_VER */

/* *********************************************************** */
