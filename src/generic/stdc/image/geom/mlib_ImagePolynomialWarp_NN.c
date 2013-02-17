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

#pragma ident	"@(#)mlib_ImagePolynomialWarp_NN.c	9.2	07/10/09 SMI"

#include <mlib_image.h>
#include <mlib_ImagePolynomialWarp.h>

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_NN_U8_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		dstData[xd[i]] = *(lineAddr[ys[i]] + xs[i]);
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_NN_U8_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	const mlib_u8 *pix;
	mlib_s32 i, dst_idx;
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		dst_idx = 2 * xd[i];
		pix = lineAddr[ys[i]] + 2 * xs[i];

		dstData[dst_idx] = pix[0];
		dstData[dst_idx + 1] = pix[1];
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_NN_U8_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	const mlib_u8 *pix;
	mlib_s32 i, dst_idx;
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		dst_idx = 3 * xd[i];
		pix = lineAddr[ys[i]] + 3 * xs[i];

		dstData[dst_idx] = pix[0];
		dstData[dst_idx + 1] = pix[1];
		dstData[dst_idx + 2] = pix[2];
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_NN_U8_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	const mlib_u8 *pix;
	mlib_s32 i, dst_idx;
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		dst_idx = 4 * xd[i];
		pix = lineAddr[ys[i]] + 4 * xs[i];

		dstData[dst_idx] = pix[0];
		dstData[dst_idx + 1] = pix[1];
		dstData[dst_idx + 2] = pix[2];
		dstData[dst_idx + 3] = pix[3];
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_NN_S16_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	const mlib_u8 *pix;
	mlib_s32 i, dst_idx;
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		dst_idx = 2 * xd[i];
		pix = lineAddr[ys[i]] + 2 * xs[i];
		*(mlib_u16 *)(dstData + dst_idx) = *(mlib_u16 *)pix;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_NN_S16_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	const mlib_u8 *pix;
	mlib_s32 i, dst_idx;
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		dst_idx = 4 * xd[i];
		pix = lineAddr[ys[i]] + 4 * xs[i];

		*(mlib_u16 *)(dstData + dst_idx) = *(mlib_u16 *)pix;
		*(mlib_u16 *)(dstData + dst_idx + 2) = *(mlib_u16 *)(pix + 2);
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_NN_S16_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	const mlib_u8 *pix;
	mlib_s32 i, dst_idx;
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		dst_idx = 6 * xd[i];
		pix = lineAddr[ys[i]] + 6 * xs[i];

		*(mlib_u16 *)(dstData + dst_idx) = *(mlib_u16 *)pix;
		*(mlib_u16 *)(dstData + dst_idx + 2) = *(mlib_u16 *)(pix + 2);
		*(mlib_u16 *)(dstData + dst_idx + 4) = *(mlib_u16 *)(pix + 4);
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_NN_S16_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	const mlib_u8 *pix;
	mlib_s32 i, dst_idx;
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		dst_idx = 8 * xd[i];
		pix = lineAddr[ys[i]] + 8 * xs[i];

		*(mlib_u16 *)(dstData + dst_idx) = *(mlib_u16 *)pix;
		*(mlib_u16 *)(dstData + dst_idx + 2) = *(mlib_u16 *)(pix + 2);
		*(mlib_u16 *)(dstData + dst_idx + 4) = *(mlib_u16 *)(pix + 4);
		*(mlib_u16 *)(dstData + dst_idx + 6) = *(mlib_u16 *)(pix + 6);
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_NN_S32_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	const mlib_u8 *pix;
	mlib_s32 i, dst_idx;
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		dst_idx = 4 * xd[i];
		pix = lineAddr[ys[i]] + 4 * xs[i];
		*(mlib_s32 *)(dstData + dst_idx) = *(mlib_s32 *)pix;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_NN_S32_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	const mlib_u8 *pix;
	mlib_s32 i, dst_idx;
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		dst_idx = 8 * xd[i];
		pix = lineAddr[ys[i]] + 8 * xs[i];

		*(mlib_s32 *)(dstData + dst_idx) = *(mlib_s32 *)pix;
		*(mlib_s32 *)(dstData + dst_idx + 4) = *(mlib_s32 *)(pix + 4);
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_NN_S32_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	const mlib_u8 *pix;
	mlib_s32 i, dst_idx;
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		dst_idx = 12 * xd[i];
		pix = lineAddr[ys[i]] + 12 * xs[i];

		*(mlib_s32 *)(dstData + dst_idx) = *(mlib_s32 *)pix;
		*(mlib_s32 *)(dstData + dst_idx + 4) = *(mlib_s32 *)(pix + 4);
		*(mlib_s32 *)(dstData + dst_idx + 8) = *(mlib_s32 *)(pix + 8);
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_NN_S32_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	const mlib_u8 *pix;
	mlib_s32 i, dst_idx;
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		dst_idx = 16 * xd[i];
		pix = lineAddr[ys[i]] + 16 * xs[i];

		*(mlib_s32 *)(dstData + dst_idx) = *(mlib_s32 *)pix;
		*(mlib_s32 *)(dstData + dst_idx + 4) = *(mlib_s32 *)(pix + 4);
		*(mlib_s32 *)(dstData + dst_idx + 8) = *(mlib_s32 *)(pix + 8);
		*(mlib_s32 *)(dstData + dst_idx + 12) = *(mlib_s32 *)(pix + 12);
	}
}

/* *********************************************************** */
