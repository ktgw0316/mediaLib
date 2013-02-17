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

#pragma ident	"@(#)mlib_m_ImagePolynomialWarp_NN.c	9.2	07/11/05 SMI"

/* *********************************************************** */
#include <mlib_image.h>
#include <mlib_ImagePolynomialWarp.h>
#include <mlib_mmx_utils.h>

/* *********************************************************** */
void
mlib_ImagePolynomialWarp_NN_U8_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	mlib_s32 i, *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

	for (i = 0; i < n; i++) {
		dstData[xd[i]] = *(lineAddr[ys[i]] + xs[i]);
	}
}

/* *********************************************************** */
void
mlib_ImagePolynomialWarp_NN_U8_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	mlib_ImagePolynomialWarp_NN_S16_1(dstData, lineAddr, pws, n);
}

/* *********************************************************** */
void
mlib_ImagePolynomialWarp_NN_U8_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	const mlib_u8 *pix;
	mlib_s32
	    i, dst_idx, *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

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
    mlib_PWS * pws,
    mlib_s32 n)
{
	mlib_ImagePolynomialWarp_NN_S32_1(dstData, lineAddr, pws, n);
}

/* *********************************************************** */
void
mlib_ImagePolynomialWarp_NN_S16_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	mlib_s32 i, *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;
	mlib_u16 *dstData1 = (mlib_u16 *)dstData;

	for (i = 0; i < n; i++) {
		dstData1[xd[i]] = *(mlib_u16 *)(lineAddr[ys[i]] + 2 * xs[i]);
	}
}

/* *********************************************************** */
void
mlib_ImagePolynomialWarp_NN_S16_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	mlib_ImagePolynomialWarp_NN_S32_1(dstData, lineAddr, pws, n);
}

/* *********************************************************** */
void
mlib_ImagePolynomialWarp_NN_S16_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	mlib_u16 *pix;
	mlib_s32
	    i, dst_idx, *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;
	mlib_u16 *dstData1 = (mlib_u16 *)dstData;

	for (i = 0; i < n; i++) {
		dst_idx = 3 * xd[i];
		pix = (mlib_u16 *)(lineAddr[ys[i]] + 6 * xs[i]);

		dstData1[dst_idx] = pix[0];
		dstData1[dst_idx + 1] = pix[1];
		dstData1[dst_idx + 2] = pix[2];
	}
}

/* *********************************************************** */
void
mlib_ImagePolynomialWarp_NN_S16_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	mlib_ImagePolynomialWarp_NN_S32_2(dstData, lineAddr, pws, n);
}

/* *********************************************************** */
void
mlib_ImagePolynomialWarp_NN_S32_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	mlib_s32 i, *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;
	mlib_s32 *dstData1 = (mlib_s32 *)dstData;

	for (i = 0; i < n; i++) {
		dstData1[xd[i]] = *(mlib_u32 *)(lineAddr[ys[i]] + 4 * xs[i]);
	}
}

/* *********************************************************** */
void
mlib_ImagePolynomialWarp_NN_S32_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	mlib_s32 i, *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;
	__m64 *dstData1 = (__m64 *) dstData;

	for (i = 0; i < n; i++) {
		dstData1[xd[i]] = *(__m64 *) (lineAddr[ys[i]] + 8 * xs[i]);
	}

	_mm_empty();
}

/* *********************************************************** */
void
mlib_ImagePolynomialWarp_NN_S32_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	mlib_s32 *pix;
	mlib_s32
	    i, dst_idx, *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;
	mlib_s32 *dstData1 = (mlib_s32 *)dstData;

	for (i = 0; i < n; i++) {
		dst_idx = 3 * xd[i];
		pix = (mlib_s32 *)(lineAddr[ys[i]] + 12 * xs[i]);

		dstData1[dst_idx] = pix[0];
		dstData1[dst_idx + 1] = pix[1];
		dstData1[dst_idx + 2] = pix[2];
	}
}

/* *********************************************************** */
void
mlib_ImagePolynomialWarp_NN_S32_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	__m64 * pix;
	mlib_s32
	    i, dst_idx, *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;
	__m64 *dstData1 = (__m64 *) dstData;

	for (i = 0; i < n; i++) {
		dst_idx = 2 * xd[i];
		pix = (__m64 *) (lineAddr[ys[i]] + 16 * xs[i]);

		dstData1[dst_idx] = pix[0];
		dstData1[dst_idx + 1] = pix[1];
	}

	_mm_empty();
}

/* *********************************************************** */
