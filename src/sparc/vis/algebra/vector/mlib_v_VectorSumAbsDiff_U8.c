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

#pragma ident	"@(#)mlib_v_VectorSumAbsDiff_U8.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *     mlib_VectorSumAbsDiff_U8_Sat - sum of absolute difference
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorSumAbsDiff_U8_Sat(mlib_d64      *z,
 *                                               const mlib_u8 *x,
 *                                               const mlib_u8 *y,
 *                                               mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the sum of the absolute difference
 *      x    pointer to the first input vector
 *      y    pointer to the second input vector
 *      n    number of elements in the vector
 *
 * DESCRIPTION
 *          n - 1
 *      z = SUM |x[i] - y[i]|
 *          i = 0
 */

#include <mlib_algebra.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorSumAbsDiff_U8_Sat = __mlib_VectorSumAbsDiff_U8_Sat

#elif defined(__GNUC__)

__typeof__(__mlib_VectorSumAbsDiff_U8_Sat) mlib_VectorSumAbsDiff_U8_Sat
	__attribute__((weak, alias("__mlib_VectorSumAbsDiff_U8_Sat")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	ABS_VALUE(x)	(((x) ^ ((x) >> 31)) - ((x) >> 31))

/* *********************************************************** */

mlib_status
__mlib_VectorSumAbsDiff_U8_Sat(
	mlib_d64 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_d64 *pdx, dx, dx0, dx1, dx2, dx3, dx4;
	mlib_d64 *pdy, dy, dy1, dy2, dy3;
	mlib_d64 accd, accd1, accd2, accd3, accum = 0.0;
	mlib_f32 accf;
	mlib_s32 acc = 0;
	mlib_s32 off;
	mlib_s32 i, nd8, nm8;

	if (n < 8) {

		if (n <= 0)
			return (MLIB_FAILURE);

		for (i = 0; i < n; i++) {
			acc += (mlib_s32)ABS_VALUE(px[i] - py[i]);
		}

		z[0] = (mlib_d64)acc;
		return (MLIB_SUCCESS);
	}

	pdy = (mlib_d64 *)((mlib_addr)py & (~7));
	off = (mlib_addr)pdy - (mlib_addr)py;

	if (off != 0) {
		off += 8;
		for (i = 0; i < off; i++) {
			acc += (mlib_s32)ABS_VALUE(*px - *py);
			px++;
			py++;
		}

		accum += (mlib_d64)acc;
	}

	nd8 = (n - off) >> 3;
	nm8 = (n - off) & 7;

	pdx = (mlib_d64 *)vis_alignaddr(px, 0);
	pdy = (mlib_d64 *)py;
	accd = accd1 = accd2 = accd3 = vis_fzero();

	for (i = 0; i <= (nd8 - 4); i += 4) {
		dx0 = pdx[0];
		dx1 = pdx[1];
		dy = pdy[0];
		dx = vis_faligndata(dx0, dx1);
		accd = vis_pdist(dx, dy, accd);
		dx2 = pdx[2];
		dy1 = pdy[1];
		dx1 = vis_faligndata(dx1, dx2);
		accd1 = vis_pdist(dx1, dy1, accd1);
		dx3 = pdx[3];
		dy2 = pdy[2];
		dx2 = vis_faligndata(dx2, dx3);
		accd2 = vis_pdist(dx2, dy2, accd2);
		dx4 = vis_ld_d64_nf(pdx + 4);
		dy3 = pdy[3];
		dx3 = vis_faligndata(dx3, dx4);
		accd3 = vis_pdist(dx3, dy3, accd3);
		pdx += 4;
		pdy += 4;
	}

#pragma unroll(1)
	for (; i < nd8; i++) {
		dx0 = pdx[0];
		dx1 = vis_ld_d64_nf(pdx + 1);
		dy = pdy[0];
		dx = vis_faligndata(dx0, dx1);
		accd = vis_pdist(dx, dy, accd);
		pdx++;
		pdy++;
	}

	accd = vis_fpadd32(accd, accd1);
	accd2 = vis_fpadd32(accd2, accd3);
	accd = vis_fpadd32(accd, accd2);
	accf = vis_read_lo(accd);
	accum += (mlib_d64)(*((mlib_s32 *)&accf));

	if (nm8 != 0) {
		px += nd8 * 8;
		py = (mlib_u8 *)pdy;
		acc = 0;
		for (i = 0; i < nm8; i++) {
			acc += (mlib_s32)ABS_VALUE(*px - *py);
			px++;
			py++;
		}

		accum += (mlib_d64)acc;
	}

	z[0] = accum;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
