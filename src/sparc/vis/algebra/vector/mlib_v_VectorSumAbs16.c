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

#pragma ident	"@(#)mlib_v_VectorSumAbs16.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorSumAbs_S16 - sum of the absolute values
 *                             of signed 16-bit format vector
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorSumAbs_S16_Sat(mlib_d64       * z,
 *                                            const mlib_s16 * x,
 *                                            mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the sum of the absolute values of the vector
 *      x    pointer to the first element of the input vector
 *      n    number of elements in the vector
 *
 * DESCRIPTION
 *           n
 *      z = SUM |x[i]|
 *          i = 1
 */

#include <mlib_algebra.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorSumAbs_S16_Sat = __mlib_VectorSumAbs_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorSumAbs_S16_Sat) mlib_VectorSumAbs_S16_Sat
	__attribute__((weak, alias("__mlib_VectorSumAbs_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	ABS_S16                                                 \
	dr1 = vis_fand(mask, dx);                               \
	dr1 = vis_fmul8ulx16(dtwo, dr1);                        \
	dr2 = vis_fxor(dr1, dx);                                \
	dr3 = vis_fpsub16(dr2, dr1)

/* *********************************************************** */

#define	ACCUM_S16                                               \
	dr4 = vis_fand(dr3, mask_lo);                           \
	dr5 = vis_fandnot(mask_lo, dr3);                        \
	dr5 = vis_faligndata(dr5, dr5);                         \
	ds = vis_fpadd32(ds, dr4);                              \
	ds1 = vis_fpadd32(ds1, dr5)

/* *********************************************************** */

mlib_status
__mlib_VectorSumAbs_S16_Sat(
	mlib_d64 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	mlib_s16 *px;
	mlib_d64 *dpx;
	mlib_d64 dx, dr1, dr2, dr3, dr4, dr5, fzero = vis_fzero();
	mlib_d64 dtwo = vis_to_double_dup(0x20002);
	mlib_d64 mask = vis_to_double_dup(0x80008000);
	mlib_d64 mask_lo = vis_to_double_dup(0x0000FFFF);
	mlib_d64 ds = fzero, ds1 = fzero;
	mlib_s32 sum = 0;
	mlib_s32 sr, src;
	mlib_f32 f;
	mlib_s32 i;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s16 *)x;

	if (n < 8) {
		for (i = 0; i < n; i++) {
			src = (mlib_s32)px[i];
			sr = (src >> 31);
			sum += (src ^ sr) - sr;
		}

		z[0] = (mlib_d64)sum;
		return (MLIB_SUCCESS);
	}

	dpx = (mlib_d64 *)vis_alignaddr(px, 0);
	dx = dpx[0];
	dx = vis_faligndata(dx, fzero);
	(void) vis_alignaddr(dpx, 6);
	ABS_S16;
	ACCUM_S16;

	i = (8 - ((mlib_addr)px & 7)) >> 1;

	for (; i <= (n - 4); i += 4) {
		dx = *((mlib_d64 *)(px + i));
		ABS_S16;
		ACCUM_S16;
	}

	dx = vis_ld_d64_nf(px + i);
	(void) vis_alignaddr(px, 2 * n);
	dx = vis_faligndata(fzero, dx);
	(void) vis_alignaddr(dpx, 6);
	ABS_S16;
	ACCUM_S16;

	ds = vis_fpadd32(ds, ds1);
	f = vis_read_lo(ds);
	z[0] = (mlib_d64)*(mlib_s32 *)&f;
	f = vis_read_hi(ds);
	z[0] += (mlib_d64)*(mlib_s32 *)&f;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
