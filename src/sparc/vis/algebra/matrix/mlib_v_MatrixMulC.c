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

#pragma ident	"@(#)mlib_v_MatrixMulC.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_MatrixMul_[U8C|S16C]_U8C_[Sat|Mod]   - multiplication of
 *                                                  unsigned 8-bit
 *                                                  format element
 *                                                  complex matrices
 *      mlib_MatrixMul_[S8C|S16C]_S8C_[Sat|Mod]   - multiplication of
 *                                                  signed 8-bit format element
 *                                                  complex matrices
 *      mlib_MatrixMul_[S16C|S32C]_S16C_[Sat|Mod] - multiplication of
 *                                                  signed 16-bit format element
 *                                                  complex matrices
 *      mlib_MatrixMul_S32C_S32C_[Sat|Mod]        - multiplication of
 *                                                  signed 32-bit format element
 *                                                  complex matrices
 *
 * SYNOPSIS
 *      mlib_status mlib_MatrixMul_S16C_S16C_[Sat|Mod](mlib_s16       *z,
 *                                                     const mlib_s16 *x,
 *                                                     const mlib_s16 *y,
 *                                                     mlib_s32       m,
 *                                                     mlib_s32       l,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_MatrixMul_S16C_S8C_[Sat|Mod](mlib_s16      *z,
 *                                                    const mlib_s8 *x,
 *                                                    const mlib_s8 *y,
 *                                                    mlib_s32      m,
 *                                                    mlib_s32      l,
 *                                                    mlib_s32      n);
 *      mlib_status mlib_MatrixMul_S16C_U8C_[Sat|Mod](mlib_s16      *z,
 *                                                    const mlib_u8 *x,
 *                                                    const mlib_u8 *y,
 *                                                    mlib_s32      m,
 *                                                    mlib_s32      l,
 *                                                    mlib_s32      n);
 *      mlib_status mlib_MatrixMul_S32C_S16C_[Sat|Mod](mlib_s32       *z,
 *                                                     const mlib_s16 *x,
 *                                                     const mlib_s16 *y,
 *                                                     mlib_s32       m,
 *                                                     mlib_s32       l,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_MatrixMul_S32C_S32C_[Sat|Mod](mlib_s32       *z,
 *                                                     const mlib_s32 *x,
 *                                                     const mlib_s32 *y,
 *                                                     mlib_s32       m,
 *                                                     mlib_s32       l,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_MatrixMul_S8C_S8C_[Sat|Mod](mlib_s8       *z,
 *                                                   const mlib_s8 *x,
 *                                                   const mlib_s8 *y,
 *                                                   mlib_s32      m,
 *                                                   mlib_s32      l,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_MatrixMul_U8C_U8C_[Sat|Mod](mlib_u8       *z,
 *                                                   const mlib_u8 *x,
 *                                                   const mlib_u8 *y,
 *                                                   mlib_s32      m,
 *                                                   mlib_s32      l,
 *                                                   mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result matrix.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts. The matrix is in row major order
 *      x    pointer to the first complex element of the first matrix.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts. The matrix is in row major order
 *      y    pointer to the first complex element of the second matrix.
 *           y[2*i] hold the real parts, and y[2*i + 1] hold the imaginary
 *           parts. The matrix is in row major order
 *      m    number of rows in the first matrix
 *      l    number of columns in the first matrix, and number of rows
 *           in the second matrix
 *      n    number of columns in the second matrix
 *
 * DESCRIPTION
 *                 l
 *      z[i][j] = SUM (x[i][k] * y[k][j])
 *                k = 1
 */

#include <mlib_algebra.h>
#include <mlib_AlgebraUtil.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_MatrixMul_S32C_S16C_Mod = __mlib_MatrixMul_S32C_S16C_Mod
#pragma weak mlib_MatrixMul_S16C_S16C_Mod = __mlib_MatrixMul_S16C_S16C_Mod

#elif defined(__GNUC__)

__typeof__(__mlib_MatrixMul_S32C_S16C_Mod) mlib_MatrixMul_S32C_S16C_Mod
	__attribute__((weak, alias("__mlib_MatrixMul_S32C_S16C_Mod")));
__typeof__(__mlib_MatrixMul_S16C_S16C_Mod) mlib_MatrixMul_S16C_S16C_Mod
	__attribute__((weak, alias("__mlib_MatrixMul_S16C_S16C_Mod")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_MatrixMul_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 l,
	mlib_s32 n)
{
	mlib_s16 *buff_x, *buff_y;
	mlib_s32 size, i, j, k;

	if (!((m > 0) && (l > 0) && (n > 0)))
		return (MLIB_FAILURE);

	size = 4 * l + n * 2 * l;
	buff_x = (mlib_s16 *)__mlib_malloc(size * sizeof (mlib_s16));

	if (buff_x == NULL)
		return mlib_MatrixMul_type(type_S16C, type_S16C, mode_Mod, x, y,
			m, l, n, n, z);

	buff_y = buff_x + 4 * l;

	__mlib_MatrixTranspose_S16C_S16C(buff_y, y, l, n);

	for (i = 0; i < m; i++) {
		mlib_f32 *px0 = (mlib_f32 *)buff_x;
		mlib_f32 *px1 = (mlib_f32 *)(buff_x + 2 * l);

		for (j = 0; j < l; j++) {
			mlib_u32 xr = ((mlib_u16 *)x)[2 * j];
			mlib_u32 xi = ((mlib_u16 *)x)[2 * j + 1];

			((mlib_u32 *)px0)[j] = (xr << 16) | xi;
			((mlib_u32 *)px1)[j] = (xi << 16) | xr;
		}

		for (j = 0; j < n; j++) {
			mlib_d64 sh0, sh1, sl0, sl1, dres;
			mlib_f32 *py = (mlib_f32 *)(buff_y + j * 2 * l);

			sh0 = sh1 = sl0 = sl1 = vis_fzero();

#pragma pipeloop(0)
			for (k = 0; k < l; k++) {
				mlib_f32 x0, x1, fy;
				mlib_d64 h0, l0, h1, l1;

				x0 = px0[k];
				x1 = px1[k];
				fy = py[k];

				h0 = vis_fmuld8sux16(x0, fy);
				l0 = vis_fmuld8ulx16(x0, fy);
				h1 = vis_fmuld8sux16(x1, fy);
				l1 = vis_fmuld8ulx16(x1, fy);

				sh0 = vis_fpadd32(sh0, h0);
				sh1 = vis_fpadd32(sh1, h1);
				sl0 = vis_fpadd32(sl0, l0);
				sl1 = vis_fpadd32(sl1, l1);
			}

			dres = vis_fpadd32(sh0, sl0);
			z[2 * j] =
				((mlib_s32 *)&dres)[0] - ((mlib_s32 *)&dres)[1];

			dres = vis_fpadd32(sh1, sl1);
			z[2 * j + 1] =
				((mlib_s32 *)&dres)[0] + ((mlib_s32 *)&dres)[1];
		}

		x += 2 * l;
		z += 2 * n;
	}

	__mlib_free(buff_x);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMul_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 l,
	mlib_s32 n)
{
	mlib_s16 *buff_x, *buff_y;
	mlib_s32 size, i, j, k;

	if (!((m > 0) && (l > 0) && (n > 0)))
		return (MLIB_FAILURE);

	size = 4 * l + n * 2 * l;
	buff_x = (mlib_s16 *)__mlib_malloc(size * sizeof (mlib_s16));
	buff_y = buff_x + 4 * l;

	__mlib_MatrixTranspose_S16C_S16C(buff_y, y, l, n);

	for (i = 0; i < m; i++) {
		mlib_f32 *px0 = (mlib_f32 *)buff_x;
		mlib_f32 *px1 = (mlib_f32 *)(buff_x + 2 * l);

		for (j = 0; j < l; j++) {
			mlib_u32 xr = ((mlib_u16 *)x)[2 * j];
			mlib_u32 xi = ((mlib_u16 *)x)[2 * j + 1];

			((mlib_u32 *)px0)[j] = (xr << 16) | xi;
			((mlib_u32 *)px1)[j] = (xi << 16) | xr;
		}

		for (j = 0; j < n; j++) {
			mlib_d64 sh0, sh1, sl0, sl1, dres;
			mlib_f32 *py = (mlib_f32 *)(buff_y + j * 2 * l);

			sh0 = sh1 = sl0 = sl1 = vis_fzero();

#pragma pipeloop(0)
			for (k = 0; k < l; k++) {
				mlib_f32 x0, x1, fy;
				mlib_d64 h0, l0, h1, l1;

				x0 = px0[k];
				x1 = px1[k];
				fy = py[k];

				h0 = vis_fmuld8sux16(x0, fy);
				l0 = vis_fmuld8ulx16(x0, fy);
				h1 = vis_fmuld8sux16(x1, fy);
				l1 = vis_fmuld8ulx16(x1, fy);

				sh0 = vis_fpadd32(sh0, h0);
				sh1 = vis_fpadd32(sh1, h1);
				sl0 = vis_fpadd32(sl0, l0);
				sl1 = vis_fpadd32(sl1, l1);
			}

			dres = vis_fpadd32(sh0, sl0);
			z[2 * j] =
				((mlib_s32 *)&dres)[0] - ((mlib_s32 *)&dres)[1];

			dres = vis_fpadd32(sh1, sl1);
			z[2 * j + 1] =
				((mlib_s32 *)&dres)[0] + ((mlib_s32 *)&dres)[1];
		}

		x += 2 * l;
		z += 2 * n;
	}

	__mlib_free(buff_x);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
