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

#pragma ident	"@(#)mlib_v_VectorDotProd16.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorDotProd_S16_Sat - dot product of signed 16-bit format vectors
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorDotProd_S16_Sat(mlib_d64       *z,
 *                                             const mlib_s16 *x,
 *                                             const mlib_s16 *y,
 *                                             mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the dot product of the vectors
 *      x    pointer to the first element of the first vector
 *      y    pointer to the first element of the second vector
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *           n
 *      z = SUM (x[i] * y[i])
 *          i = 1
 */

#include <mlib_algebra.h>
#include <mlib_AlgebraUtil.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorDotProd_S16_Sat = __mlib_VectorDotProd_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorDotProd_S16_Sat) mlib_VectorDotProd_S16_Sat
	__attribute__((weak, alias("__mlib_VectorDotProd_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	DPROD16                                                       \
	dr.db = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dy));    \
	dr1.db = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dy));   \
	dr.db = vis_fpadd32(dr.db, dr1.db);                           \
	dr2 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dy));      \
	dr3 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dy));      \
	dr1.db = vis_fpadd32(dr2, dr3);                               \
	ds1 = ds1 + (mlib_d64)dr.two_int.int0 +                       \
		(mlib_d64)dr.two_int.int1;                            \
	ds = ds + (mlib_d64)dr1.two_int.int0 +                        \
		(mlib_d64)dr1.two_int.int1

/* *********************************************************** */

mlib_status
__mlib_VectorDotProd_S16_Sat(
	mlib_d64 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_s16 *pxend, *px = (mlib_s16 *)x, *py = (mlib_s16 *)y;
	mlib_d64 *dpx, *dpy, *dpxend;
	mlib_d64 dx, dy, dy0, dy1;
	mlib_d64 ds, ds1, dr2, dr3;
	type_union_mlib_d64 dr, dr1;
	mlib_d64 edge[2];
	mlib_s32 d_left;
	mlib_s32 emask, off;

	edge[0] = edge[1] = 0;

	if (n <= 0)
		return (MLIB_FAILURE);

	ds = ds1 = 0.0;

	dpx = (mlib_d64 *)((mlib_addr)px & (~7));
	off = (mlib_addr)dpx - (mlib_addr)px;
	dpy = (mlib_d64 *)vis_alignaddr(py, off);
	pxend = px + n - 1;
	dpxend = (mlib_d64 *)((mlib_addr)pxend & (~7));
	emask = vis_edge16(px, pxend);
	vis_pst_16(dpx[0], edge, emask);
	dx = edge[0];
	dy = vis_ld_d64_nf(dpy);
	d_left = dpxend - dpx;

	if (((((mlib_addr)px) ^ ((mlib_addr)py)) & 7) == 0) {
		for (; d_left > 0; d_left--) {
			DPROD16;
			dpx++;
			dpy++;
			dx = dpx[0];
			dy = dpy[0];
		}
	} else {
		dy1 = vis_ld_d64_nf(dpy+1);
		dy = vis_faligndata(dy, dy1);
		for (; d_left > 0; d_left--) {
			dpx++;
			dpy++;
			dy0 = dy1;
			dy1 = vis_ld_d64_nf(dpy+1);
			DPROD16;
			dx = dpx[0];
			dy = vis_faligndata(dy0, dy1);
		}
	}

	if ((mlib_addr)dpx <= (mlib_addr)pxend) {
		emask = vis_edge16(dpx, pxend);
		vis_pst_16(dx, edge + 1, emask);
		dx = edge[1];
		DPROD16;
	}

	z[0] = ds + ds1;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
