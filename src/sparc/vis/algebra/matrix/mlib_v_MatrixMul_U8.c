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

#pragma ident	"@(#)mlib_v_MatrixMul_U8.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_MatrixMul_U8_[U8|S16]_[Sat|Mod] - multiplication of
 *                    unsigned 8-bit format element matrices
 *      mlib_MatrixMul_S8_[S8|S16]_[Sat|Mod] - multiplication of
 *                    signed 8-bit format element matrices
 *      mlib_MatrixMul_S16_[S16|S32]_[Sat|Mod] - multiplication of
 *                    signed 16-bit format element matrices
 *      mlib_MatrixMul_S32_S32_[Sat|Mod] - multiplication of
 *                    signed 32-bit format element matrices
 *
 * SYNOPSIS
 *      mlib_status mlib_MatrixMul_S16_S8_[Sat|Mod](mlib_s16      * z,
 *                                                  const mlib_s8 * x,
 *                                                  const mlib_s8 * y,
 *                                                  mlib_s32      m,
 *                                                  mlib_s32      l,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_MatrixMul_S16_U8_[Sat|Mod](mlib_s16      * z,
 *                                                  const mlib_u8 * x,
 *                                                  const mlib_u8 * y,
 *                                                  mlib_s32      m,
 *                                                  mlib_s32      l,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_MatrixMul_S8_S8_[Sat|Mod](mlib_s8       * z,
 *                                                 const mlib_s8 * x,
 *                                                 const mlib_s8 * y,
 *                                                 mlib_s32      m,
 *                                                 mlib_s32      l,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_MatrixMul_U8_U8_[Sat|Mod](mlib_u8       * z,
 *                                                 const mlib_u8 * x,
 *                                                 const mlib_u8 * y,
 *                                                 mlib_s32      m,
 *                                                 mlib_s32      l,
 *                                                 mlib_s32      n);
 *
 *      mlib_status mlib_MatrixMul_S16_S16_Mod(mlib_s16       * z,
 *                                             const mlib_s16 * x,
 *                                             const mlib_s16 * y,
 *                                             mlib_s32       m,
 *                                             mlib_s32       l,
 *                                             mlib_s32       n);
 *      mlib_status mlib_MatrixMul_S32_S16_Mod(mlib_s32       * z,
 *                                             const mlib_s16 * x,
 *                                             const mlib_s16 * y,
 *                                             mlib_s32       m,
 *                                             mlib_s32       l,
 *                                             mlib_s32       n);
 * ARGUMENTS
 *      z    pointer to the first element of the result matrix.
 *           The matrix is in row major order
 *      x    pointer to the first element of the first matrix.
 *           The matrix is in row major order
 *      y    pointer to the first element of the second matrix.
 *           The matrix is in row major order
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

#pragma weak mlib_MatrixMul_S16_U8_Mod = __mlib_MatrixMul_S16_U8_Mod
#pragma weak mlib_MatrixMul_S16_U8_Sat = __mlib_MatrixMul_S16_U8_Sat
#pragma weak mlib_MatrixMul_U8_U8_Mod = __mlib_MatrixMul_U8_U8_Mod
#pragma weak mlib_MatrixMul_U8_U8_Sat = __mlib_MatrixMul_U8_U8_Sat

#elif defined(__GNUC__)

__typeof__(__mlib_MatrixMul_S16_U8_Mod) mlib_MatrixMul_S16_U8_Mod
	__attribute__((weak, alias("__mlib_MatrixMul_S16_U8_Mod")));
__typeof__(__mlib_MatrixMul_S16_U8_Sat) mlib_MatrixMul_S16_U8_Sat
	__attribute__((weak, alias("__mlib_MatrixMul_S16_U8_Sat")));
__typeof__(__mlib_MatrixMul_U8_U8_Mod) mlib_MatrixMul_U8_U8_Mod
	__attribute__((weak, alias("__mlib_MatrixMul_U8_U8_Mod")));
__typeof__(__mlib_MatrixMul_U8_U8_Sat) mlib_MatrixMul_U8_U8_Sat
	__attribute__((weak, alias("__mlib_MatrixMul_U8_U8_Sat")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	STYPE	mlib_u8

#define	MAX_SIZE	400

/* *********************************************************** */

#define	LOAD	xx = (*px++); y0 = (*py0++); y1 = (*py1++)

/* *********************************************************** */

#define	MUL                                                        \
	dr0 = vis_fmuld8ulx16(vis_read_hi(xx), vis_read_hi(y0));   \
	dr1 = vis_fmuld8ulx16(vis_read_lo(xx), vis_read_lo(y0));   \
	dr2 = vis_fmuld8ulx16(vis_read_hi(xx), vis_read_hi(y1));   \
	dr3 = vis_fmuld8ulx16(vis_read_lo(xx), vis_read_lo(y1))

/* *********************************************************** */

#define	SUM                                                     \
	ds0 = vis_fpadd32(ds0, dr0);                            \
	ds0 = vis_fpadd32(ds0, dr1);                            \
	ds1 = vis_fpadd32(ds1, dr2);                            \
	ds1 = vis_fpadd32(ds1, dr3)

/* *********************************************************** */

#define	SATUR(s)	if (s > vmax) s = vmax

/* *********************************************************** */

static mlib_status
mlib_MatrixMul_U8xU8(
	void *z,
	const STYPE * x,
	const STYPE * y,
	mlib_s32 m,
	mlib_s32 l,
	mlib_s32 n,
	mlib_s32 dst_type)
{
	mlib_d64 *px, *buff_x, *buff_y, *pbuff_x, *pbuff_y;
	mlib_d64 array[MAX_SIZE];
	mlib_d64 xx, x0, x1, y0, y1, ds0, ds1, dr0, dr1, dr2, dr3;
	mlib_s32 size, i, j, k, l4;
	mlib_s32 vmax;

	if (!((m > 0) && (l > 0) && (n > 0))) {
		return (MLIB_FAILURE);
	}

	if (!dst_type) {
		vmax = MLIB_U8_MAX;
	} else {	/* if (dst_type == 1) */

		vmax = MLIB_S16_MAX;
	}

	l4 = (l + 3) / 4;
	size = l4 * n + l4 + 3;

	if (size <= MAX_SIZE) {
		buff_y = array;
	} else {
		buff_y = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (buff_y == NULL) {
			mlib_s32 type_z, mode;

			if (!dst_type) {
				type_z = type_U8;
				mode = mode_Sat;
			} else if (dst_type == 1) {
				type_z = type_S16;
				mode = mode_Sat;
			} else {	/* if (dst_type == 2) */

				type_z = type_S16;
				mode = mode_Mod;
			}

			return mlib_MatrixMul_type(type_U8, type_z, mode, x, y,
				m, l, n, n, z);
		}
	}

	buff_x = buff_y + l4 * n;
	pbuff_y = buff_y;

/* transpose y matrix */
	for (i = 0; i < n; i++) {
		STYPE *py = (STYPE *) y + i;
		mlib_s16 *pp = (mlib_s16 *)pbuff_y;

		for (j = 0; j <= (l - 4); j += 4) {
			((mlib_s32 *)pp)[0] = (py[0] << 16) | py[n];
			((mlib_s32 *)pp)[1] = (py[2 * n] << 16) | py[3 * n];
			py += 4 * n;
			pp += 4;
		}

		for (; j < l; j++) {
			(*pp++) = *py;
			py += n;
		}

		for (; j < 4 * l4; j++) {
			(*pp++) = 0;
		}

		pbuff_y += l4;
	}

	for (j = 0; j < m; j++) {
		pbuff_x = buff_x;
		pbuff_y = buff_y;

/* copy x line */
		px = vis_alignaddr((void *)x, j * l);
		x1 = vis_ld_d64_nf(px);
		px++;
		for (i = 0; i < (l + 7) / 8; i++) {
			x0 = x1;
			x1 = vis_ld_d64_nf(px);
			px++;
			xx = vis_faligndata(x0, x1);
			pbuff_x[2 * i] =
				vis_fpmerge(vis_read_hi(xx), vis_read_hi(xx));
			pbuff_x[2 * i + 1] =
				vis_fpmerge(vis_read_lo(xx), vis_read_lo(xx));
		}

/* loop on y lines */
		for (i = 0; i < n; i += 2) {
			mlib_d64 *px = pbuff_x;
			mlib_d64 *py0 = pbuff_y;
			mlib_d64 *py1 = (i + 1 < n) ? (py0 + l4) : py0;
			mlib_s32 s0, s1;

			ds0 = ds1 = vis_fzero();

			LOAD;
			MUL;
			LOAD;

#pragma pipeloop(0)
			for (k = 0; k < l4; k++) {
				SUM;
				MUL;
				LOAD;
			}

			s0 = ((mlib_s32 *)&ds0)[0] + ((mlib_s32 *)&ds0)[1];
			SATUR(s0);

			if (dst_type) {
				((mlib_s16 *)z)[i] = s0;
			} else {
				((mlib_u8 *)z)[i] = s0;
			}

			if (i + 1 < n) {
				s1 = ((mlib_s32 *)&ds1)[0] +
					((mlib_s32 *)&ds1)[1];
				SATUR(s1);

				if (dst_type) {
					((mlib_s16 *)z)[i + 1] = s1;
				} else {
					((mlib_u8 *)z)[i + 1] = s1;
				}
			}

			pbuff_y += 2 * l4;
		}

		z = (mlib_u8 *)z + ((dst_type) ? (2 * n) : n);
	}

	if (size > MAX_SIZE) {
		__mlib_free(buff_y);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#pragma no_inline(mlib_MatrixMul_U8xU8)

/* *********************************************************** */

mlib_status
__mlib_MatrixMul_U8_U8_Sat(
	STYPE * z,
	const STYPE * x,
	const STYPE * y,
	mlib_s32 m,
	mlib_s32 l,
	mlib_s32 n)
{
	return (mlib_MatrixMul_U8xU8(z, x, y, m, l, n, 0));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMul_S16_U8_Sat(
	mlib_s16 *z,
	const STYPE * x,
	const STYPE * y,
	mlib_s32 m,
	mlib_s32 l,
	mlib_s32 n)
{
	return (mlib_MatrixMul_U8xU8(z, x, y, m, l, n, 1));
}

/* *********************************************************** */

#undef LOAD
#undef MUL
#undef SUM

/* *********************************************************** */

#define	LOAD	x0 = (*px++); x1 = (*px++); y0 = (*py0++); y1 = (*py1++)

/* *********************************************************** */

#define	MUL                                                     \
	dr0 = vis_fmul8x16(vis_read_hi(y0), x0);                \
	dr1 = vis_fmul8x16(vis_read_lo(y0), x1);                \
	dr2 = vis_fmul8x16(vis_read_hi(y1), x0);                \
	dr3 = vis_fmul8x16(vis_read_lo(y1), x1)

/* *********************************************************** */

#define	SUM                                                     \
	ds0 = vis_fpadd16(ds0, dr0);                            \
	ds0 = vis_fpadd16(ds0, dr1);                            \
	ds1 = vis_fpadd16(ds1, dr2);                            \
	ds1 = vis_fpadd16(ds1, dr3)

/* *********************************************************** */

mlib_status
__mlib_MatrixMul_U8_U8_Mod(
	STYPE * z,
	const STYPE * x,
	const STYPE * y,
	mlib_s32 m,
	mlib_s32 l,
	mlib_s32 n)
{
	mlib_d64 *px, *buff_x, *buff_y, *pbuff_x, *pbuff_y;
	mlib_d64 array[MAX_SIZE];
	mlib_d64 xx, x0, x1, y0, y1, ds0, ds1, dr0, dr1, dr2, dr3;
	mlib_s32 size, i, j, k, l8;

	if (!((m > 0) && (l > 0) && (n > 0))) {
		return (MLIB_FAILURE);
	}

	l8 = (l + 7) / 8;
	size = l8 * n + 2 * l8 + 4;

	if (size <= MAX_SIZE) {
		buff_y = array;
	} else {
		buff_y = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (buff_y == NULL) {
			return mlib_MatrixMul_type(type_U8, type_U8, mode_Sat,
				x, y, m, l, n, n, z);
		}
	}

	buff_x = buff_y + l8 * n;
	pbuff_y = buff_y;

/* transpose y matrix */
	for (i = 0; i < n; i++) {
		STYPE *py = (STYPE *) y + i;
		STYPE *pp = (STYPE *) pbuff_y;

		for (j = 0; j <= (l - 4); j += 4) {
			((mlib_s16 *)pp)[0] = (py[0] << 8) | py[n];
			((mlib_s16 *)pp)[1] = (py[2 * n] << 8) | py[3 * n];
			py += 4 * n;
			pp += 4;
		}

		for (; j < l; j++) {
			(*pp++) = *py;
			py += n;
		}

		for (; j < 8 * l8; j++) {
			(*pp++) = 0;
		}

		pbuff_y += l8;
	}

	for (j = 0; j < m; j++) {
		pbuff_x = buff_x;
		pbuff_y = buff_y;

/* copy x line */
		px = vis_alignaddr((void *)x, j * l);
		x1 = vis_ld_d64_nf(px);
		px++;
		for (i = 0; i < l8; i++) {
			x0 = x1;
			x1 = vis_ld_d64_nf(px);
			px++;
			xx = vis_faligndata(x0, x1);
			pbuff_x[2 * i] =
				vis_fpmerge(vis_read_hi(xx), vis_fzeros());
			pbuff_x[2 * i + 1] =
				vis_fpmerge(vis_read_lo(xx), vis_fzeros());
		}

/* loop on y lines */
		for (i = 0; i < n; i += 2) {
			mlib_d64 *px = pbuff_x;
			mlib_d64 *py0 = pbuff_y;
			mlib_d64 *py1 = (i + 1 < n) ? (py0 + l8) : py0;

			ds0 = ds1 = vis_fzero();

			LOAD;
			MUL;
			LOAD;

#pragma pipeloop(0)
			for (k = 0; k < l8; k++) {
				SUM;
				MUL;
				LOAD;
			}

			ds0 = vis_freg_pair(vis_fpadd16s(vis_read_hi(ds0),
				vis_read_lo(ds0)),
				vis_fpadd16s(vis_read_hi(ds1),
				vis_read_lo(ds1)));

			z[i] = ((mlib_s16 *)&ds0)[0] + ((mlib_s16 *)&ds0)[1];

			if (i + 1 < n) {
				z[i + 1] =
					((mlib_s16 *)&ds0)[2] +
					((mlib_s16 *)&ds0)[3];
			}

			pbuff_y += 2 * l8;
		}

		z += n;
	}

	if (size > MAX_SIZE) {
		__mlib_free(buff_y);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMul_S16_U8_Mod(
	mlib_s16 *z,
	const STYPE * x,
	const STYPE * y,
	mlib_s32 m,
	mlib_s32 l,
	mlib_s32 n)
{
	mlib_d64 *px, *buff_x, *buff_y, *pbuff_x, *pbuff_y;
	mlib_d64 array[MAX_SIZE];
	mlib_d64 xx, x0, x1, y0, y1, ds0, ds1, dr0, dr1, dr2, dr3;
	mlib_d64 mask128 = vis_to_double_dup(0x80808080);
	mlib_s32 *y_sum;
	mlib_s32 size, i, j, k, l8;

	if (!((m > 0) && (l > 0) && (n > 0))) {
		return (MLIB_FAILURE);
	}

	l8 = (l + 7) / 8;
	size = l8 * n + 2 * l8 + (n + 1) / 2 + 1;

	if (size <= MAX_SIZE) {
		buff_y = array;
	} else {
		buff_y = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (buff_y == NULL) {
			return mlib_MatrixMul_type(type_U8, type_U8, mode_Sat,
				x, y, m, l, n, n, z);
		}
	}

	buff_x = buff_y + l8 * n;
	y_sum = (mlib_s32 *)(buff_x + 2 * l8);
	pbuff_y = buff_y;

/* transpose y matrix */
	for (i = 0; i < n; i++) {
		STYPE *py = (STYPE *) y + i;
		STYPE *pp = (STYPE *) pbuff_y;
		mlib_s32 sum = 0;

		for (j = 0; j <= (l - 4); j += 4) {
			sum += py[0] + py[n] + py[2 * n] + py[3 * n];
			((mlib_s16 *)pp)[0] = (py[0] << 8) | py[n];
			((mlib_s16 *)pp)[1] = (py[2 * n] << 8) | py[3 * n];
			py += 4 * n;
			pp += 4;
		}

		for (; j < l; j++) {
			sum += py[0];
			(*pp++) = *py;
			py += n;
		}

		y_sum[i] = sum << 7;

		for (; j < 8 * l8; j++) {
			(*pp++) = 0;
		}

		pbuff_y += l8;
	}

	for (j = 0; j < m; j++) {
		pbuff_x = buff_x;
		pbuff_y = buff_y;

/* copy x line */
		px = vis_alignaddr((void *)x, j * l);
		x1 = vis_ld_d64_nf(px);
		px++;
		for (i = 0; i < l8; i++) {
			x0 = x1;
			x1 = vis_ld_d64_nf(px);
			px++;
			xx = vis_faligndata(x0, x1);
			xx = vis_fxor(xx, mask128);
			pbuff_x[2 * i] =
				vis_fpmerge(vis_read_hi(xx), vis_fzeros());
			pbuff_x[2 * i + 1] =
				vis_fpmerge(vis_read_lo(xx), vis_fzeros());
		}

/* loop on y lines */
		for (i = 0; i < n; i += 2) {
			mlib_d64 *px = pbuff_x;
			mlib_d64 *py0 = pbuff_y;
			mlib_d64 *py1 = (i + 1 < n) ? (py0 + l8) : py0;

			ds0 = ds1 = vis_fzero();

			LOAD;
			MUL;
			LOAD;

#pragma pipeloop(0)
			for (k = 0; k < l8; k++) {
				SUM;
				MUL;
				LOAD;
			}

			ds0 = vis_freg_pair(vis_fpadd16s(vis_read_hi(ds0),
				vis_read_lo(ds0)),
				vis_fpadd16s(vis_read_hi(ds1),
				vis_read_lo(ds1)));

			z[i] = ((mlib_s16 *)&ds0)[0] + ((mlib_s16 *)&ds0)[1] +
				y_sum[i];

			if (i + 1 < n) {
				z[i + 1] =
					((mlib_s16 *)&ds0)[2] +
					((mlib_s16 *)&ds0)[3] + y_sum[i + 1];
			}

			pbuff_y += 2 * l8;
		}

		z += n;
	}

	if (size > MAX_SIZE) {
		__mlib_free(buff_y);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
