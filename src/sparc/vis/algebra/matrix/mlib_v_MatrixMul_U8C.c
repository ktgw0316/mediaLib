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

#pragma ident	"@(#)mlib_v_MatrixMul_U8C.c	9.4	07/11/05 SMI"

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

#pragma weak mlib_MatrixMul_S16C_U8C_Mod = __mlib_MatrixMul_S16C_U8C_Mod
#pragma weak mlib_MatrixMul_S16C_U8C_Sat = __mlib_MatrixMul_S16C_U8C_Sat
#pragma weak mlib_MatrixMul_U8C_U8C_Mod = __mlib_MatrixMul_U8C_U8C_Mod
#pragma weak mlib_MatrixMul_U8C_U8C_Sat = __mlib_MatrixMul_U8C_U8C_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_MatrixMul_S16C_U8C_Mod) mlib_MatrixMul_S16C_U8C_Mod
	__attribute__((weak, alias("__mlib_MatrixMul_S16C_U8C_Mod")));
__typeof__(__mlib_MatrixMul_S16C_U8C_Sat) mlib_MatrixMul_S16C_U8C_Sat
	__attribute__((weak, alias("__mlib_MatrixMul_S16C_U8C_Sat")));
__typeof__(__mlib_MatrixMul_U8C_U8C_Mod) mlib_MatrixMul_U8C_U8C_Mod
	__attribute__((weak, alias("__mlib_MatrixMul_U8C_U8C_Mod")));
__typeof__(__mlib_MatrixMul_U8C_U8C_Sat) mlib_MatrixMul_U8C_U8C_Sat
	__attribute__((weak, alias("__mlib_MatrixMul_U8C_U8C_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	STYPE	mlib_u8
#define	MAX_SIZE	400

/* *********************************************************** */

#define	LOAD	xx = (*px++); y0 = (*py0++); y1 = (*py1++)

/* *********************************************************** */

#define	MUL                                                          \
	d0_rr = vis_fmuld8ulx16(vis_read_hi(xx), vis_read_hi(y0));   \
	d1_rr = vis_fmuld8ulx16(vis_read_hi(xx), vis_read_hi(y1));   \
	d0_ri = vis_fmuld8ulx16(vis_read_hi(xx), vis_read_lo(y0));   \
	d1_ri = vis_fmuld8ulx16(vis_read_hi(xx), vis_read_lo(y1));   \
	d0_ir = vis_fmuld8ulx16(vis_read_lo(xx), vis_read_hi(y0));   \
	d1_ir = vis_fmuld8ulx16(vis_read_lo(xx), vis_read_hi(y1));   \
	d0_ii = vis_fmuld8ulx16(vis_read_lo(xx), vis_read_lo(y0));   \
	d1_ii = vis_fmuld8ulx16(vis_read_lo(xx), vis_read_lo(y1))

/* *********************************************************** */

#define	SUM                                                     \
	d0_r = vis_fpadd32(d0_r, d0_rr);                        \
	d0_r = vis_fpsub32(d0_r, d0_ii);                        \
	d1_r = vis_fpadd32(d1_r, d1_rr);                        \
	d1_r = vis_fpsub32(d1_r, d1_ii);                        \
	d0_i = vis_fpadd32(d0_i, d0_ri);                        \
	d0_i = vis_fpadd32(d0_i, d0_ir);                        \
	d1_i = vis_fpadd32(d1_i, d1_ri);                        \
	d1_i = vis_fpadd32(d1_i, d1_ir)

/* *********************************************************** */

#define	SATUR(s)	if (s < vmin) s = vmin; if (s > vmax) s = vmax

/* *********************************************************** */

#define	STORE(ind, d_r, d_i)                                    \
	s_r = ((mlib_s32 *)&d_r)[0] + ((mlib_s32 *)&d_r)[1];    \
	s_i = ((mlib_s32 *)&d_i)[0] + ((mlib_s32 *)&d_i)[1];    \
	SATUR(s_r);                                             \
	SATUR(s_i);                                             \
	if (dst_type) {                                         \
	    ((mlib_s16 *)z)[2 * (ind)] = s_r;                   \
	    ((mlib_s16 *)z)[2 * (ind) + 1] = s_i;               \
	} else {                                                \
	    ((mlib_u8 *)z)[2 * (ind)] = s_r;                    \
	    ((mlib_u8 *)z)[2 * (ind) + 1] = s_i;                \
	}

/* *********************************************************** */

static mlib_status
mlib_MatrixMul_U8CxU8C(
	void *z,
	const STYPE * x,
	const STYPE * y,
	mlib_s32 m,
	mlib_s32 l,
	mlib_s32 n,
	mlib_s32 dst_type)
{
	mlib_d64 *buff_x, *buff_y, *pbuff_x, *pbuff_y;
	mlib_d64 array[MAX_SIZE];
	mlib_d64 xx, y0, y1;
	mlib_d64 d0_rr, d0_ri, d0_ir, d0_ii, d0_r, d0_i;
	mlib_d64 d1_rr, d1_ri, d1_ir, d1_ii, d1_r, d1_i;
	mlib_s32 size, i, j, k, l4, s_r, s_i;
	mlib_s32 vmin, vmax;

	if (!((m > 0) && (l > 0) && (n > 0))) {
		return (MLIB_FAILURE);
	}

	if (!dst_type) {
		vmin = MLIB_U8_MIN;
		vmax = MLIB_U8_MAX;
	} else if (dst_type == 1) {
		vmin = MLIB_S16_MIN;
		vmax = MLIB_S16_MAX;
	} else {	/* if (dst_type == 2) */

		vmin = MLIB_S32_MIN;
		vmax = MLIB_S32_MAX;
	}

/* complex */
	l *= 2;

	l4 = (l + 3) / 4;
	size = l4 * n + l4 + 3;

	if (size <= MAX_SIZE) {
		buff_y = array;
	} else {
		buff_y = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (buff_y == NULL) {
			mlib_s32 type_z, mode;

			if (!dst_type) {
				type_z = type_U8C;
				mode = mode_Sat;
			} else if (dst_type == 1) {
				type_z = type_S16C;
				mode = mode_Sat;
			} else {	/* if (dst_type == 2) */

				type_z = type_S16C;
				mode = mode_Mod;
			}

			return mlib_MatrixMul_type(type_U8C, type_z, mode, x, y,
				m, l, n, n, z);
		}
	}

	buff_x = buff_y + l4 * n;
	pbuff_y = buff_y;

/* transpose y matrix */
	for (i = 0; i < n; i++) {
		STYPE *py = (STYPE *) y + 2 * i;
		mlib_s16 *pp = (mlib_s16 *)pbuff_y;

		for (j = 0; j <= (l - 4); j += 4) {
			((mlib_s32 *)pp)[0] = (py[0] << 16) | py[2 * n];
			((mlib_s32 *)pp)[1] = (py[1] << 16) | py[2 * n + 1];
			py += 4 * n;
			pp += 4;
		}

		if (j < l) {
			((mlib_s32 *)pp)[0] = (py[0] << 16);
			((mlib_s32 *)pp)[1] = (py[1] << 16);
		}

		pbuff_y += l4;
	}

	for (j = 0; j < m; j++) {
		const STYPE *px = x;

		pbuff_x = buff_x;
		pbuff_y = buff_y;

/* copy x line */
		for (i = 0; i <= (l - 4); i += 4) {
			((mlib_s32 *)pbuff_x)[0] = (px[0] << 16) + px[2];
			((mlib_s32 *)pbuff_x)[1] = (px[1] << 16) + px[3];
			px += 4;
			pbuff_x++;
		}
		if (i < l) {
			((mlib_s32 *)pbuff_x)[0] = (px[0] << 16);
			((mlib_s32 *)pbuff_x)[1] = (px[1] << 16);
		}

/* loop on y lines */
		for (i = 0; i < n; i += 2) {
			mlib_d64 *px = buff_x;
			mlib_d64 *py0 = pbuff_y;
			mlib_d64 *py1 = (i + 1 < n) ? (py0 + l4) : py0;

			d0_r = d0_i = d1_r = d1_i = vis_fzero();

			LOAD;
			MUL;
			LOAD;

#pragma pipeloop(0)
			for (k = 0; k < l4; k++) {
				SUM;
				MUL;
				LOAD;
			}

			STORE(i, d0_r, d0_i);

			if (i + 1 < n) {
				STORE(i + 1, d1_r, d1_i);
			}

			pbuff_y += 2 * l4;
		}

		z = (mlib_u8 *)z + ((dst_type) ? (4 * n) : (2 * n));
		x += l;
	}

	if (size > MAX_SIZE) {
		__mlib_free(buff_y);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#pragma no_inline(mlib_MatrixMul_U8CxU8C)

/* *********************************************************** */

mlib_status
__mlib_MatrixMul_U8C_U8C_Sat(
	STYPE * z,
	const STYPE * x,
	const STYPE * y,
	mlib_s32 m,
	mlib_s32 l,
	mlib_s32 n)
{
	return (mlib_MatrixMul_U8CxU8C(z, x, y, m, l, n, 0));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixMul_S16C_U8C_Sat(
	mlib_s16 *z,
	const STYPE * x,
	const STYPE * y,
	mlib_s32 m,
	mlib_s32 l,
	mlib_s32 n)
{
	return (mlib_MatrixMul_U8CxU8C(z, x, y, m, l, n, 1));
}

/* *********************************************************** */

#undef LOAD
#undef MUL
#undef SUM

/* *********************************************************** */

#define	LOAD	x0 = (*px++); x1 = (*px++); y0 = (*py0++); y1 = (*py1++)

/* *********************************************************** */

#define	MUL                                                     \
	d0_rr = vis_fmul8x16(vis_read_hi(y0), x0);              \
	d1_rr = vis_fmul8x16(vis_read_hi(y1), x0);              \
	d0_ri = vis_fmul8x16(vis_read_lo(y0), x0);              \
	d1_ri = vis_fmul8x16(vis_read_lo(y1), x0);              \
	d0_ir = vis_fmul8x16(vis_read_hi(y0), x1);              \
	d1_ir = vis_fmul8x16(vis_read_hi(y1), x1);              \
	d0_ii = vis_fmul8x16(vis_read_lo(y0), x1);              \
	d1_ii = vis_fmul8x16(vis_read_lo(y1), x1)

/* *********************************************************** */

#define	SUM                                                     \
	d0_r = vis_fpadd16(d0_r, d0_rr);                        \
	d0_r = vis_fpsub16(d0_r, d0_ii);                        \
	d1_r = vis_fpadd16(d1_r, d1_rr);                        \
	d1_r = vis_fpsub16(d1_r, d1_ii);                        \
	d0_i = vis_fpadd16(d0_i, d0_ri);                        \
	d0_i = vis_fpadd16(d0_i, d0_ir);                        \
	d1_i = vis_fpadd16(d1_i, d1_ri);                        \
	d1_i = vis_fpadd16(d1_i, d1_ir)

/* *********************************************************** */

#undef  STORE
#define	STORE(ind, d_r, d_i)                                              \
	d_r =                                                             \
	vis_freg_pair(vis_fpadd16s(vis_read_hi(d_r), vis_read_lo(d_r)),   \
		vis_fpadd16s(vis_read_hi(d_i), vis_read_lo(d_i)));        \
	s_r = ((mlib_s16 *)&d_r)[0] + ((mlib_s16 *)&d_r)[1];              \
	s_i = ((mlib_s16 *)&d_r)[2] + ((mlib_s16 *)&d_r)[3];              \
	z[2 * (ind)] = s_r;                                               \
	z[2 * (ind) + 1] = s_i

/* *********************************************************** */

mlib_status
__mlib_MatrixMul_U8C_U8C_Mod(
	STYPE * z,
	const STYPE * x,
	const STYPE * y,
	mlib_s32 m,
	mlib_s32 l,
	mlib_s32 n)
{
	mlib_d64 *px, *buff_x, *buff_y, *pbuff_x, *pbuff_y;
	mlib_d64 array[MAX_SIZE];
	mlib_d64 xx, x0, x1, y0, y1;
	mlib_d64 d0_rr, d0_ri, d0_ir, d0_ii, d0_r, d0_i;
	mlib_d64 d1_rr, d1_ri, d1_ir, d1_ii, d1_r, d1_i;
	mlib_s32 size, i, j, k, l8, s_r, s_i;

	if (!((m > 0) && (l > 0) && (n > 0))) {
		return (MLIB_FAILURE);
	}

/* complex */
	l *= 2;

	l8 = (l + 7) / 8;
	size = l8 * n + 2 * l8 + 4;

	if (size <= MAX_SIZE) {
		buff_y = array;
	} else {
		buff_y = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (buff_y == NULL) {
			return mlib_MatrixMul_type(type_U8C, type_U8C, mode_Mod,
				x, y, m, l, n, n, z);
		}
	}

	buff_x = buff_y + l8 * n;
	pbuff_y = buff_y;

/* transpose y matrix */
	for (i = 0; i < n; i++) {
		STYPE *py = (STYPE *) y + 2 * i;
		STYPE *pp = (STYPE *) pbuff_y;

		for (j = 0; j <= (l - 8); j += 8) {
			((mlib_s16 *)pp)[0] = (py[0] << 8) | py[2 * n];
			((mlib_s16 *)pp)[1] = (py[4 * n] << 8) | py[6 * n];
			((mlib_s16 *)pp)[2] = (py[1] << 8) | py[2 * n + 1];
			((mlib_s16 *)pp)[3] =
				(py[4 * n + 1] << 8) | py[6 * n + 1];
			py += 8 * n;
			pp += 8;
		}

		for (; j < l; j += 2) {
			pp[0] = py[0];
			pp[4] = py[1];
			pp++;
			py += 2 * n;
		}

		for (; j < 8 * l8; j += 2) {
			pp[0] = 0;
			pp[4] = 0;
			pp++;
		}

		pbuff_y += l8;
	}

	for (j = 0; j < m; j++) {
		pbuff_x = buff_x;
		pbuff_y = buff_y;

/* copy x line */
		px = vis_alignaddr((void *)x, 0);
		x1 = vis_ld_d64_nf(px);
		px++;
		for (i = 0; i < l8; i++) {
			x0 = x1;
			x1 = vis_ld_d64_nf(px);
			px++;
			xx = vis_faligndata(x0, x1);
			xx = vis_fpmerge(vis_read_hi(xx), vis_read_lo(xx));
			xx = vis_fpmerge(vis_read_hi(xx), vis_read_lo(xx));
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

			d0_r = d0_i = d1_r = d1_i = vis_fzero();

			LOAD;
			MUL;
			LOAD;

#pragma pipeloop(0)
			for (k = 0; k < l8; k++) {
				SUM;
				MUL;
				LOAD;
			}

			STORE(i, d0_r, d0_i);

			if (i + 1 < n) {
				STORE(i + 1, d1_r, d1_i);
			}

			pbuff_y += 2 * l8;
		}

		z += 2 * n;
		x += l;
	}

	if (size > MAX_SIZE) {
		__mlib_free(buff_y);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  STORE
#define	STORE(ind, d_r, d_i)                                               \
	d_r =                                                              \
	vis_freg_pair(vis_fpadd16s(vis_read_hi(d_r), vis_read_lo(d_r)),    \
		vis_fpadd16s(vis_read_hi(d_i), vis_read_lo(d_i)));         \
	s_r = ((mlib_s16 *)&d_r)[0] + ((mlib_s16 *)&d_r)[1];               \
	s_i = ((mlib_s16 *)&d_r)[2] + ((mlib_s16 *)&d_r)[3];               \
	z[2 * (ind)] = s_r + y_sum[2 * (ind)] - y_sum[2 * (ind) + 1];      \
	z[2 * (ind) + 1] = s_i + y_sum[2 * (ind)] + y_sum[2 * (ind) + 1]

/* *********************************************************** */

mlib_status
__mlib_MatrixMul_S16C_U8C_Mod(
	mlib_s16 *z,
	const STYPE * x,
	const STYPE * y,
	mlib_s32 m,
	mlib_s32 l,
	mlib_s32 n)
{
	mlib_d64 *px, *buff_x, *buff_y, *pbuff_x, *pbuff_y;
	mlib_d64 array[MAX_SIZE];
	mlib_d64 xx, x0, x1, y0, y1;
	mlib_d64 d0_rr, d0_ri, d0_ir, d0_ii, d0_r, d0_i;
	mlib_d64 d1_rr, d1_ri, d1_ir, d1_ii, d1_r, d1_i;
	mlib_d64 mask128 = vis_to_double_dup(0x80808080);
	mlib_s32 *y_sum;
	mlib_s32 size, i, j, k, l8, s_r, s_i;

	if (!((m > 0) && (l > 0) && (n > 0))) {
		return (MLIB_FAILURE);
	}

/* complex */
	l *= 2;

	l8 = (l + 7) / 8;
	size = l8 * n + 2 * l8 + n;

	if (size <= MAX_SIZE) {
		buff_y = array;
	} else {
		buff_y = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (buff_y == NULL) {
			return mlib_MatrixMul_type(type_U8C, type_S16C,
				mode_Mod, x, y, m, l, n, n, z);
		}
	}

	buff_x = buff_y + l8 * n;
	y_sum = (mlib_s32 *)(buff_x + 2 * l8);
	pbuff_y = buff_y;

/* transpose y matrix */
	for (i = 0; i < n; i++) {
		STYPE *py = (STYPE *) y + 2 * i;
		STYPE *pp = (STYPE *) pbuff_y;
		mlib_s32 yr_sum = 0, yi_sum = 0;

		for (j = 0; j <= (l - 8); j += 8) {
			((mlib_s16 *)pp)[0] = (py[0] << 8) | py[2 * n];
			((mlib_s16 *)pp)[1] = (py[4 * n] << 8) | py[6 * n];
			((mlib_s16 *)pp)[2] = (py[1] << 8) | py[2 * n + 1];
			((mlib_s16 *)pp)[3] =
				(py[4 * n + 1] << 8) | py[6 * n + 1];
			yr_sum += py[0] + py[2 * n] + py[4 * n] + py[6 * n];
			yi_sum +=
				py[1] + py[2 * n + 1] + py[4 * n + 1] +
				py[6 * n + 1];
			py += 8 * n;
			pp += 8;
		}

		for (; j < l; j += 2) {
			pp[0] = py[0];
			pp[4] = py[1];
			yr_sum += py[0];
			yi_sum += py[1];
			pp++;
			py += 2 * n;
		}

		for (; j < 8 * l8; j += 2) {
			pp[0] = 0;
			pp[4] = 0;
			pp++;
		}

		y_sum[2 * i] = yr_sum << 7;
		y_sum[2 * i + 1] = yi_sum << 7;

		pbuff_y += l8;
	}

	for (j = 0; j < m; j++) {
		pbuff_x = buff_x;
		pbuff_y = buff_y;

/* copy x line */
		px = vis_alignaddr((void *)x, 0);
		x1 = vis_ld_d64_nf(px);
		px++;
		for (i = 0; i < l8; i++) {
			x0 = x1;
			x1 = vis_ld_d64_nf(px);
			px++;
			xx = vis_faligndata(x0, x1);
			xx = vis_fxor(xx, mask128);
			xx = vis_fpmerge(vis_read_hi(xx), vis_read_lo(xx));
			xx = vis_fpmerge(vis_read_hi(xx), vis_read_lo(xx));
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

			d0_r = d0_i = d1_r = d1_i = vis_fzero();

			LOAD;
			MUL;
			LOAD;

#pragma pipeloop(0)
			for (k = 0; k < l8; k++) {
				SUM;
				MUL;
				LOAD;
			}

			STORE(i, d0_r, d0_i);

			if (i + 1 < n) {
				STORE(i + 1, d1_r, d1_i);
			}

			pbuff_y += 2 * l8;
		}

		z += 2 * n;
		x += l;
	}

	if (size > MAX_SIZE) {
		__mlib_free(buff_y);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
