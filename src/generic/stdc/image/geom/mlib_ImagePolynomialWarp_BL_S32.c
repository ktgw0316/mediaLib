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

#pragma ident	"@(#)mlib_ImagePolynomialWarp_BL_S32.c	9.2	07/10/09 SMI"

#include <mlib_image.h>
#include <mlib_ImagePolynomialWarp.h>

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
	a0##idx = pix[idx0];                                    \
	a1##idx = pix[idx1];                                    \
	a2##idx = pix1[idx1];                                   \
	a3##idx = pix1[idx0]

/* *********************************************************** */

#define	CALC(idx)                                               \
	r0##idx = a0##idx + (a3##idx - a0##idx) * dy;           \
	r1##idx = a1##idx + (a2##idx - a1##idx) * dy;           \
	r2##idx = r0##idx + (r1##idx - r0##idx) * dx

/* *********************************************************** */

#define	TYPE_FMT	mlib_s32

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_S32_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT *pix, *pix1, *pix2;
	mlib_d64 *dxs = pws->dsrc_x, *dys = pws->dsrc_y;
	mlib_d64 a00, a10, a20, a30, r00, r10, r20;
	mlib_d64 dx, dy;
	mlib_s32 i, xsrc, ysrc, xdst, stride = lineAddr[1] - lineAddr[0];
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		GET_POINT(4);
		LOAD_PIX(0, 0, 1);
		CALC(0);
		pix2[0] = (mlib_s32)r20;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_S32_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT *pix, *pix1, *pix2;
	mlib_d64 *dxs = pws->dsrc_x, *dys = pws->dsrc_y;
	mlib_d64 a00, a10, a20, a30, r00, r10, r20;
	mlib_d64 a01, a11, a21, a31, r01, r11, r21;
	mlib_d64 dx, dy;
	mlib_s32 i, xsrc, ysrc, xdst, stride = lineAddr[1] - lineAddr[0];
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		GET_POINT(8);
		LOAD_PIX(0, 0, 2);
		LOAD_PIX(1, 1, 3);
		CALC(0);
		CALC(1);
		pix2[0] = (mlib_s32)r20;
		pix2[1] = (mlib_s32)r21;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_S32_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT *pix, *pix1, *pix2;
	mlib_d64 *dxs = pws->dsrc_x, *dys = pws->dsrc_y;
	mlib_d64 a00, a10, a20, a30, r00, r10, r20;
	mlib_d64 a01, a11, a21, a31, r01, r11, r21;
	mlib_d64 a02, a12, a22, a32, r02, r12, r22;
	mlib_d64 dx, dy;
	mlib_s32 i, xsrc, ysrc, xdst, stride = lineAddr[1] - lineAddr[0];
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		GET_POINT(12);
		LOAD_PIX(0, 0, 3);
		LOAD_PIX(1, 1, 4);
		LOAD_PIX(2, 2, 5);
		CALC(0);
		CALC(1);
		CALC(2);
		pix2[0] = (mlib_s32)r20;
		pix2[1] = (mlib_s32)r21;
		pix2[2] = (mlib_s32)r22;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BL_S32_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	TYPE_FMT *pix, *pix1, *pix2;
	mlib_d64 *dxs = pws->dsrc_x, *dys = pws->dsrc_y;
	mlib_d64 a00, a10, a20, a30, r00, r10, r20;
	mlib_d64 a01, a11, a21, a31, r01, r11, r21;
	mlib_d64 a02, a12, a22, a32, r02, r12, r22;
	mlib_d64 a03, a13, a23, a33, r03, r13, r23;
	mlib_d64 dx, dy;
	mlib_s32 i, xsrc, ysrc, xdst, stride = lineAddr[1] - lineAddr[0];
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		GET_POINT(16);
		LOAD_PIX(0, 0, 4);
		LOAD_PIX(1, 1, 5);
		LOAD_PIX(2, 2, 6);
		LOAD_PIX(3, 3, 7);
		CALC(0);
		CALC(1);
		CALC(2);
		CALC(3);
		pix2[0] = (mlib_s32)r20;
		pix2[1] = (mlib_s32)r21;
		pix2[2] = (mlib_s32)r22;
		pix2[3] = (mlib_s32)r23;
	}
}

/* *********************************************************** */
