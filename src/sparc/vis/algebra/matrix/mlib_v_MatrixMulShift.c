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

#pragma ident	"@(#)mlib_v_MatrixMulShift.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *
 *      mlib_MatrixMulShift_S16_S16_Mod - multiplication of signed 16-bit
 *                                        matrices with shifting
 *
 * SYNOPSIS
 *      mlib_status mlib_MatrixMulShift_S16_S16_Mod(mlib_s16       * z,
 *                                                  const mlib_s16 * x,
 *                                                  const mlib_s16 * y,
 *                                                  mlib_s32       m,
 *                                                  mlib_s32       l,
 *                                                  mlib_s32       n,
 *                                                  mlib_s32       shift);
 *
 * ARGUMENTS
 *      z      pointer to the first element of the result matrix.
 *             The matrix is in row major order
 *      x      pointer to the first element of the first matrix.
 *             The matrix is in row major order
 *      y      pointer to the first element of the second matrix.
 *             The matrix is in row major order
 *      m      number of rows in the first matrix
 *      l      number of columns in the first matrix, and number of rows
 *             in the second matrix
 *      n      number of columns in the second matrix
 *  shift      the right shifting factor
 *
 * DESCRIPTION
 *                 l
 *      z[i][j] = SUM (x[i][k] * y[k][j]) * 2^(-shift)
 *                k = 1
 */

#include <mlib_algebra.h>
#include <mlib_AlgebraUtil.h>
#include <mlib_MatrixTranspose.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_MatrixMulShift_S16_S16_Mod = \
	__mlib_MatrixMulShift_S16_S16_Mod

#elif defined(__GNUC__)

__typeof__(__mlib_MatrixMulShift_S16_S16_Mod) mlib_MatrixMulShift_S16_S16_Mod
	__attribute__((weak, alias("__mlib_MatrixMulShift_S16_S16_Mod")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_MatrixMulShift_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 l,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s16 *buff_x, *buff_y;
	mlib_s32 size, i, j, k, l2, l4;

	if (!((m > 0) && (l > 0) && (n > 0)))
		return (MLIB_FAILURE);

	if (shift < 1 || shift > 16)
		return (MLIB_OUTOFRANGE);

	l2 = (l + 1) & ~1;
	l4 = (l + 3) & ~3;

	size = l4 + l2 * n;
	buff_x = (mlib_s16 *)__mlib_malloc(size * sizeof (mlib_s16));
	buff_y = buff_x + l4;

	if (buff_x == NULL)
		return mlib_MatrixMulShift_type(type_S16, type_S16, mode_Mod, x,
			y, m, l, n, n, z, shift);

	mlib_MatrixTranspose_S_S16_S16(buff_y, y, l, n, l2);

	if (l & 1) {
		mlib_s16 *py = buff_y + l;

		for (j = 0; j < n; j++)
			py[j * l2] = 0;
	}

	for (i = 0; i < m; i++) {
		mlib_f32 *px = (mlib_f32 *)x;

		if ((mlib_addr)x & 3) {
			mlib_d64 *dx = vis_alignaddr((void *)x, 0), dx0, dx1;
			mlib_d64 *ax = (mlib_d64 *)buff_x;

			dx1 = (*dx++);
			for (j = 0; j < l4 / 4; j++) {
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dx);
				ax[j] = vis_faligndata(dx0, dx1);
				dx++;
			}

			px = (mlib_f32 *)ax;
		}

		for (j = 0; j < n; j++) {
			mlib_d64 s0, s1, dres;
			mlib_f32 *py = (mlib_f32 *)(buff_y + j * l2);

			s0 = s1 = vis_fzero();

#pragma pipeloop(0)
			for (k = 0; k < l2 / 2; k++) {
				mlib_f32 fx, fy;
				mlib_d64 rh, rl;

				fx = px[k];
				fy = py[k];

				rh = vis_fmuld8sux16(fx, fy);
				rl = vis_fmuld8ulx16(fx, fy);
				s0 = vis_fpadd32(s0, rh);
				s1 = vis_fpadd32(s1, rl);
			}

			dres = vis_fpadd32(s0, s1);
			z[j] = (((mlib_s32 *)&dres)[0] +
				((mlib_s32 *)&dres)[1]) >> shift;
		}

		x += l;
		z += n;
	}

	__mlib_free(buff_x);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
