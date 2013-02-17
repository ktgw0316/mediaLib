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

#pragma ident	"@(#)mlib_ImagePolynomialWarp_NN_Fp.c	9.2	07/10/09 SMI"

#include <mlib_image.h>
#include <mlib_ImagePolynomialWarp.h>

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_NN_D64_1(
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
		*(mlib_d64 *)(dstData + dst_idx) = *(mlib_d64 *)pix;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_NN_D64_2(
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

		*(mlib_d64 *)(dstData + dst_idx) = *(mlib_d64 *)pix;
		*(mlib_d64 *)(dstData + dst_idx + 8) = *(mlib_d64 *)(pix + 8);
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_NN_D64_3(
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
		dst_idx = 24 * xd[i];
		pix = lineAddr[ys[i]] + 24 * xs[i];

		*(mlib_d64 *)(dstData + dst_idx) = *(mlib_d64 *)pix;
		*(mlib_d64 *)(dstData + dst_idx + 8) = *(mlib_d64 *)(pix + 8);
		*(mlib_d64 *)(dstData + dst_idx + 16) = *(mlib_d64 *)(pix + 16);
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_NN_D64_4(
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
		dst_idx = 32 * xd[i];
		pix = lineAddr[ys[i]] + 32 * xs[i];

		*(mlib_d64 *)(dstData + dst_idx) = *(mlib_d64 *)pix;
		*(mlib_d64 *)(dstData + dst_idx + 8) = *(mlib_d64 *)(pix + 8);
		*(mlib_d64 *)(dstData + dst_idx + 16) = *(mlib_d64 *)(pix + 16);
		*(mlib_d64 *)(dstData + dst_idx + 24) = *(mlib_d64 *)(pix + 24);
	}
}

/* *********************************************************** */
