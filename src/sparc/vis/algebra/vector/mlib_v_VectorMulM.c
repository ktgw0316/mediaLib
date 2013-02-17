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

#pragma ident	"@(#)mlib_v_VectorMulM.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *    mlib_VectorMulM_[U8|S16]_U8_[Sat|Mod] - multiplication of
 *                  unsigned 8-bit format vector by matrix
 *    mlib_VectorMulM_[S8|S16]_S8_[Sat|Mod] - multiplication of
 *                  signed 8-bit format vector by matrix
 *    mlib_VectorMulM_[S16|S32]_S16_[Sat|Mod] - multiplication of
 *                  signed 16-bit format vector by matrix
 *    mlib_VectorMulM_S32_S32_[Sat|Mod] - multiplication of
 *                  signed 32-bit format vector by matrix
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMulM_S16_S16_[Sat|Mod](mlib_s16       *z,
 *                                                    const mlib_s16 *x,
 *                                                    const mlib_s16 *y,
 *                                                    mlib_s32       m,
 *                                                    mlib_s32       n);
 *      mlib_status mlib_VectorMulM_S16_S8_[Sat|Mod](mlib_s16      *z,
 *                                                   const mlib_s8 *x,
 *                                                   const mlib_s8 *y,
 *                                                   mlib_s32      m,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_VectorMulM_S16_U8_[Sat|Mod](mlib_s16      *z,
 *                                                   const mlib_u8 *x,
 *                                                   const mlib_u8 *y,
 *                                                   mlib_s32      m,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_VectorMulM_S32_S16_[Sat|Mod](mlib_s32       *z,
 *                                                    const mlib_s16 *x,
 *                                                    const mlib_s16 *y,
 *                                                    mlib_s32       m,
 *                                                    mlib_s32       n);
 *      mlib_status mlib_VectorMulM_S32_S32_[Sat|Mod](mlib_s32       *z,
 *                                                    const mlib_s32 *x,
 *                                                    const mlib_s32 *y,
 *                                                    mlib_s32       m,
 *                                                    mlib_s32       n);
 *      mlib_status mlib_VectorMulM_S8_S8_[Sat|Mod](mlib_s8       *z,
 *                                                  const mlib_s8 *x,
 *                                                  const mlib_s8 *y,
 *                                                  mlib_s32      m,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorMulM_U8_U8_[Sat|Mod](mlib_u8       *z,
 *                                                  const mlib_u8 *x,
 *                                                  const mlib_u8 *y,
 *                                                  mlib_s32      m,
 *                                                  mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      x    pointer to the first element of the input vector
 *      y    pointer to the first element of the input matrix
 *      m    number of rows in the matrix, and number of elements
 *           in the input vector
 *      n    number of columns in the matrix, and number of elements
 *           in the result vector
 *
 * DESCRIPTION
 *              m
 *      z[i] = SUM (x[k] * y[k][i])
 *             k = 1
 */

/*
 * FUNCTIONS
 *    mlib_VectorMulM_[U8C|S16C]_U8C_[Sat|Mod] - multiplication of
 *                  unsigned 8-bit format complex vector by complex matrix
 *    mlib_VectorMulM_[S8C|S16C]_S8C_[Sat|Mod] - multiplication of
 *                  signed 8-bit format complex vector by complex matrix
 *    mlib_VectorMulM_[S16C|S32C]_S16C_[Sat|Mod] - multiplication of
 *                  signed 16-bit format complex vector by complex matrix
 *    mlib_VectorMulM_S32C_S32C_[Sat|Mod] - multiplication of
 *                  signed 32-bit format complex vector by complex matrix
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMulM_S16C_S16C_[Sat|Mod](mlib_s16       *z,
 *                                                      const mlib_s16 *x,
 *                                                      const mlib_s16 *y,
 *                                                      mlib_s32       m,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorMulM_S16C_S8C_[Sat|Mod](mlib_s16      * z,
 *                                                     const mlib_s8 * x,
 *                                                     const mlib_s8 * y,
 *                                                     mlib_s32      m,
 *                                                     mlib_s32      n);
 *      mlib_status mlib_VectorMulM_S16C_U8C_[Sat|Mod](mlib_s16      * z,
 *                                                     const mlib_u8 * x,
 *                                                     const mlib_u8 * y,
 *                                                     mlib_s32      m,
 *                                                     mlib_s32      n);
 *      mlib_status mlib_VectorMulM_S32C_S16C_[Sat|Mod](mlib_s32       * z,
 *                                                      const mlib_s16 * x,
 *                                                      const mlib_s16 * y,
 *                                                      mlib_s32       m,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorMulM_S32C_S32C_[Sat|Mod](mlib_s32       * z,
 *                                                      const mlib_s32 * x,
 *                                                      const mlib_s32 * y,
 *                                                      mlib_s32       m,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorMulM_S8C_S8C_[Sat|Mod](mlib_s8       * z,
 *                                                    const mlib_s8 * x,
 *                                                    const mlib_s8 * y,
 *                                                    mlib_s32      m,
 *                                                    mlib_s32      n);
 *      mlib_status mlib_VectorMulM_U8C_U8C_[Sat|Mod](mlib_u8       * z,
 *                                                    const mlib_u8 * x,
 *                                                    const mlib_u8 * y,
 *                                                    mlib_s32      m,
 *                                                    mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      x    pointer to the first element of the input vector
 *      y    pointer to the first element of the input matrix
 *      m    number of rows in the matrix, and number of elements
 *           in the input vector
 *      n    number of columns in the matrix, and number of elements
 *           in the result vector
 *
 * DESCRIPTION
 *              m
 *      z[i] = SUM (x[k] * y[k][i])
 *             k = 1
 */

#include <mlib_algebra.h>
#include <mlib_AlgebraUtil.h>
#include <mlib_VectorMulM_proto.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorMulM_S32_S16_Mod = __mlib_VectorMulM_S32_S16_Mod
#pragma weak mlib_VectorMulM_S16C_S8C_Sat = __mlib_VectorMulM_S16C_S8C_Sat
#pragma weak mlib_VectorMulM_S16_S16_Mod = __mlib_VectorMulM_S16_S16_Mod
#pragma weak mlib_VectorMulM_S16C_U8C_Sat = __mlib_VectorMulM_S16C_U8C_Sat
#pragma weak mlib_VectorMulM_S8_S8_Mod = __mlib_VectorMulM_S8_S8_Mod
#pragma weak mlib_VectorMulM_S32C_S16C_Mod = __mlib_VectorMulM_S32C_S16C_Mod
#pragma weak mlib_VectorMulM_S16C_S16C_Mod = __mlib_VectorMulM_S16C_S16C_Mod
#pragma weak mlib_VectorMulM_S32_S16_Sat = __mlib_VectorMulM_S32_S16_Sat
#pragma weak mlib_VectorMulM_U8_U8_Mod = __mlib_VectorMulM_U8_U8_Mod
#pragma weak mlib_VectorMulM_S8C_S8C_Mod = __mlib_VectorMulM_S8C_S8C_Mod
#pragma weak mlib_VectorMulM_S16_S16_Sat = __mlib_VectorMulM_S16_S16_Sat
#pragma weak mlib_VectorMulM_S8_S8_Sat = __mlib_VectorMulM_S8_S8_Sat
#pragma weak mlib_VectorMulM_S32C_S16C_Sat = __mlib_VectorMulM_S32C_S16C_Sat
#pragma weak mlib_VectorMulM_U8C_U8C_Mod = __mlib_VectorMulM_U8C_U8C_Mod
#pragma weak mlib_VectorMulM_S16C_S16C_Sat = __mlib_VectorMulM_S16C_S16C_Sat
#pragma weak mlib_VectorMulM_U8_U8_Sat = __mlib_VectorMulM_U8_U8_Sat
#pragma weak mlib_VectorMulM_S8C_S8C_Sat = __mlib_VectorMulM_S8C_S8C_Sat
#pragma weak mlib_VectorMulM_S16_S8_Mod = __mlib_VectorMulM_S16_S8_Mod
#pragma weak mlib_VectorMulM_S16_U8_Mod = __mlib_VectorMulM_S16_U8_Mod
#pragma weak mlib_VectorMulM_U8C_U8C_Sat = __mlib_VectorMulM_U8C_U8C_Sat
#pragma weak mlib_VectorMulM_S16C_S8C_Mod = __mlib_VectorMulM_S16C_S8C_Mod
#pragma weak mlib_VectorMulM_S16_S8_Sat = __mlib_VectorMulM_S16_S8_Sat
#pragma weak mlib_VectorMulM_S16C_U8C_Mod = __mlib_VectorMulM_S16C_U8C_Mod
#pragma weak mlib_VectorMulM_S16_U8_Sat = __mlib_VectorMulM_S16_U8_Sat

#elif defined(__GNUC__)

__typeof__(__mlib_VectorMulM_S32_S16_Mod) mlib_VectorMulM_S32_S16_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S32_S16_Mod")));
__typeof__(__mlib_VectorMulM_S16C_S8C_Sat) mlib_VectorMulM_S16C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S16C_S8C_Sat")));
__typeof__(__mlib_VectorMulM_S16_S16_Mod) mlib_VectorMulM_S16_S16_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S16_S16_Mod")));
__typeof__(__mlib_VectorMulM_S16C_U8C_Sat) mlib_VectorMulM_S16C_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S16C_U8C_Sat")));
__typeof__(__mlib_VectorMulM_S8_S8_Mod) mlib_VectorMulM_S8_S8_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S8_S8_Mod")));
__typeof__(__mlib_VectorMulM_S32C_S16C_Mod) mlib_VectorMulM_S32C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S32C_S16C_Mod")));
__typeof__(__mlib_VectorMulM_S16C_S16C_Mod) mlib_VectorMulM_S16C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S16C_S16C_Mod")));
__typeof__(__mlib_VectorMulM_S32_S16_Sat) mlib_VectorMulM_S32_S16_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S32_S16_Sat")));
__typeof__(__mlib_VectorMulM_U8_U8_Mod) mlib_VectorMulM_U8_U8_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_U8_U8_Mod")));
__typeof__(__mlib_VectorMulM_S8C_S8C_Mod) mlib_VectorMulM_S8C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S8C_S8C_Mod")));
__typeof__(__mlib_VectorMulM_S16_S16_Sat) mlib_VectorMulM_S16_S16_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S16_S16_Sat")));
__typeof__(__mlib_VectorMulM_S8_S8_Sat) mlib_VectorMulM_S8_S8_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S8_S8_Sat")));
__typeof__(__mlib_VectorMulM_S32C_S16C_Sat) mlib_VectorMulM_S32C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S32C_S16C_Sat")));
__typeof__(__mlib_VectorMulM_U8C_U8C_Mod) mlib_VectorMulM_U8C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_U8C_U8C_Mod")));
__typeof__(__mlib_VectorMulM_S16C_S16C_Sat) mlib_VectorMulM_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S16C_S16C_Sat")));
__typeof__(__mlib_VectorMulM_U8_U8_Sat) mlib_VectorMulM_U8_U8_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_U8_U8_Sat")));
__typeof__(__mlib_VectorMulM_S8C_S8C_Sat) mlib_VectorMulM_S8C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S8C_S8C_Sat")));
__typeof__(__mlib_VectorMulM_S16_S8_Mod) mlib_VectorMulM_S16_S8_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S16_S8_Mod")));
__typeof__(__mlib_VectorMulM_S16_U8_Mod) mlib_VectorMulM_S16_U8_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S16_U8_Mod")));
__typeof__(__mlib_VectorMulM_U8C_U8C_Sat) mlib_VectorMulM_U8C_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_U8C_U8C_Sat")));
__typeof__(__mlib_VectorMulM_S16C_S8C_Mod) mlib_VectorMulM_S16C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S16C_S8C_Mod")));
__typeof__(__mlib_VectorMulM_S16_S8_Sat) mlib_VectorMulM_S16_S8_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S16_S8_Sat")));
__typeof__(__mlib_VectorMulM_S16C_U8C_Mod) mlib_VectorMulM_S16C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S16C_U8C_Mod")));
__typeof__(__mlib_VectorMulM_S16_U8_Sat) mlib_VectorMulM_S16_U8_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S16_U8_Sat")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MAX_SIZE	400

/* *********************************************************** */

#define	VECTOR_MULM_U8_SAT                                      \
	dr = vis_fmuld8ulx16(vis_read_hi(dy), fx);              \
	dr1 = vis_fmuld8ulx16(vis_read_lo(dy), fx);             \
	dy = vis_faligndata(dy, dy);                            \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dy), fx);             \
	dr3 = vis_fmuld8ulx16(vis_read_lo(dy), fx)

/* *********************************************************** */

#define	VECTOR_SUM_S32                                          \
	ds = vis_fpadd32(ds, dr);                               \
	ds1 = vis_fpadd32(ds1, dr1);                            \
	ds2 = vis_fpadd32(ds2, dr2);                            \
	ds3 = vis_fpadd32(ds3, dr3)

/* *********************************************************** */

#define	PACK_S32_U8_SAT                                              \
	ds = vis_freg_pair(vis_fpackfix(ds), vis_fpackfix(ds1));     \
	ds2 = vis_freg_pair(vis_fpackfix(ds2), vis_fpackfix(ds3));   \
	vis_write_gsr(7 << 3);                                       \
	/*                                                           \
	 * scale factor for fpack16                                  \
	 */                                                          \
	ds = vis_fpack16_to_hi(ds, ds);                              \
	ds = vis_fpack16_to_lo(ds, ds2);                             \
	ds = vis_fpmerge(vis_read_lo(ds), vis_read_hi(ds));          \
	/* \ scale factor for fpackfix \ */                          \
	vis_write_gsr((15 << 3) | 7);

/* *********************************************************** */

#define	VECTOR_MULM_S8                                          \
	dr = vis_fmuld8sux16(vis_read_hi(dy), fx);              \
	dr1 = vis_fmuld8sux16(vis_read_lo(dy), fx);             \
	dy = vis_fpack32(dy, dy);                               \
	dr2 = vis_fmuld8sux16(vis_read_hi(dy), fx);             \
	dr3 = vis_fmuld8sux16(vis_read_lo(dy), fx)

/* *********************************************************** */

#define	VECTOR_SUM_S32                                          \
	ds = vis_fpadd32(ds, dr);                               \
	ds1 = vis_fpadd32(ds1, dr1);                            \
	ds2 = vis_fpadd32(ds2, dr2);                            \
	ds3 = vis_fpadd32(ds3, dr3)

/* *********************************************************** */

#define	PACK_S32_S8_SAT                                              \
	ds = vis_freg_pair(vis_fpackfix(ds), vis_fpackfix(ds1));     \
	ds2 = vis_freg_pair(vis_fpackfix(ds2), vis_fpackfix(ds3));   \
	ds = vis_fpack16_to_hi(ds, ds);                              \
	ds = vis_fpack16_to_lo(ds, ds2);                             \
	ds = vis_fxor(ds, restore);                                  \
	ds = vis_fpmerge(vis_read_hi(ds), vis_read_lo(ds))

/* *********************************************************** */

#define	M_MLIB_S8_MAX	255

/* *********************************************************** */

#define	VECTOR_MULM_S8_MOD                                       \
	dr = vis_fmul8x16al(vis_read_hi(dy), vis_read_lo(dx));   \
	dr1 = vis_fmul8x16al(vis_read_lo(dy), vis_read_lo(dx))

/* *********************************************************** */

#define	VECTOR_SUM_S16_MOD                                      \
	ds = vis_fpadd16(ds, dr);                               \
	ds1 = vis_fpadd16(ds1, dr1)

/* *********************************************************** */

#define	PACK_S16_S8_MOD                                         \
	ds = vis_fand(ds, without_control);                     \
	ds1 = vis_fand(ds1, without_control);                   \
	ds = vis_fpack16_to_hi(ds, ds);                         \
	ds = vis_fpack16_to_lo(ds, ds1)

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 size, i;
	mlib_s8 *pzend = (mlib_s8 *)z + n - 1;
	mlib_d64 array[MAX_SIZE], *dpx, *dpy, *dpz;
	mlib_d64 dy_hi, dy_lo, dy, ds, ds1, ds2, ds3, dr, dr1, dr2, dr3;
	mlib_f32 fx;

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = (m >> 1) + 1;

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_U8, type_U8, mode_Sat,
				x, y, 1, m, n, nstride, z);
	}

	for (i = 0; i < m; i++) {
		((mlib_s16 *)dpx)[2 * i] = ((mlib_s16 *)dpx)[2 * i + 1] =
			((mlib_u8 *)x)[i] * 2;
	}

/* initialize GSR */
	vis_write_gsr((15 << 3) | 7);
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));

	if (((n | nstride | (mlib_addr)y | (mlib_addr)z) & 7) == 0) {
		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s8 *py = (mlib_s8 *)y;

			ds = ds1 = ds2 = ds3 = 0;
			for (i = 0; i < m; i++) {
				dy = ((mlib_d64 *)py)[0];
				fx = ((mlib_f32 *)dpx)[i];
				VECTOR_MULM_U8_SAT;
				VECTOR_SUM_S32;
				py += nstride;
			}

			PACK_S32_U8_SAT;
/* store 8 bytes of result */
			dpz[0] = ds;
			y = (void *)((mlib_s8 *)y + 8);
			dpz++;
		}
	} else {
		mlib_s32 off = (mlib_addr)dpz - (mlib_addr)z,
/*
 * generate edge mask for the start point
 */
			emask = vis_edge8(z, pzend);

		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s8 *py = (mlib_s8 *)y;

			ds = ds1 = ds2 = ds3 = 0;

			dpy = (mlib_d64 *)vis_alignaddr(py, off);
			dy_hi = vis_ld_d64_nf(dpy);
			dy_lo = vis_ld_d64_nf(dpy + 1);
			dy = vis_faligndata(dy_hi, dy_lo);
			fx = ((mlib_f32 *)dpx)[0];
			(void) vis_alignaddr(dpy, 7);
			VECTOR_MULM_U8_SAT;
			py += nstride;
			for (i = 0; i < (m - 1); i++) {
				VECTOR_SUM_S32;
				dpy = (mlib_d64 *)vis_alignaddr(py, off);
				dy_hi = vis_ld_d64_nf(dpy);
				dy_lo = vis_ld_d64_nf(dpy + 1);
				dy = vis_faligndata(dy_hi, dy_lo);
				fx = ((mlib_f32 *)dpx)[i + 1];
				(void) vis_alignaddr(dpy, 7);
				VECTOR_MULM_U8_SAT;
				py += nstride;
			}

			VECTOR_SUM_S32;
			PACK_S32_U8_SAT;
/* store 8 bytes of result */
			vis_pst_8(ds, dpz, emask);
			y = (void *)((mlib_s8 *)y + 8);
			dpz++;
			emask = vis_edge8(dpz, pzend);
		}
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	return mlib_VectorMulM_S_S8_S8_Mod((mlib_s8 *)z, (mlib_s8 *)x,
		(mlib_s8 *)y, m, n, nstride);
}

/* *********************************************************** */

#define	VECTOR_MULM_U8_SAT16                                    \
	dy_h = vis_fpmerge(dzero, vis_read_hi(dy));             \
	dy_l = vis_fpmerge(dzero, vis_read_lo(dy));             \
	dr = vis_fmuld8ulx16(vis_read_hi(dy_h), fx);            \
	dr1 = vis_fmuld8ulx16(vis_read_lo(dy_h), fx);           \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dy_l), fx);           \
	dr3 = vis_fmuld8ulx16(vis_read_lo(dy_l), fx);

#define	PACK_S32_U8_SAT16                                              \
	ds  = vis_freg_pair(vis_fpackfix(ds), vis_fpackfix(ds1));      \
	ds2 = vis_freg_pair(vis_fpackfix(ds2), vis_fpackfix(ds3));

#define	PACK_S32_U8_SAT16_SWAP                                           \
	{                                                                \
	                                                                 \
	/*                                                               \
	 * for use with VECTOR_MULM_U8_SAT                               \
	 */                                                              \
	    mlib_d64 ds4;                                                \
	                                                                 \
	    ds4 = vis_freg_pair(vis_read_hi(ds2), vis_read_hi(ds));      \
	    ds = vis_freg_pair(vis_read_lo(ds2), vis_read_lo(ds));       \
	    ds2 = vis_freg_pair(vis_read_hi(ds3), vis_read_hi(ds1));     \
	    ds3 = vis_freg_pair(vis_read_lo(ds3), vis_read_lo(ds1));     \
	    ds = vis_freg_pair(vis_fpackfix(ds4), vis_fpackfix(ds));     \
	    ds2 = vis_freg_pair(vis_fpackfix(ds2), vis_fpackfix(ds3));   \
	}

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_S16_U8_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 size, i;
	mlib_s16 *pzend = (mlib_s16 *)z + n - 1;
	mlib_d64 array[MAX_SIZE], *dpx, *dpy, *dpz;
	mlib_d64 dy_hi, dy_lo, dy_h, dy_l, dy, ds, ds1, ds2, ds3, dr, dr1, dr2,
		dr3;
	mlib_d64 dzero = vis_fzero();
	mlib_f32 fx;

/* initialize GSR */
	vis_write_gsr((15 << 3) | 7);

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = (m >> 1) + 1;

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_U8, type_S16, mode_Sat,
				x, y, 1, m, n, nstride, z);
	}

	for (i = 0; i < m; i++) {
		((mlib_s16 *)dpx)[2 * i] = ((mlib_s16 *)dpx)[2 * i + 1] =
			((mlib_u8 *)x)[i] * 2;
	}

	dpz = (mlib_d64 *)((mlib_addr)z & (~7));

	if (((n | nstride | (mlib_addr)y | (mlib_addr)z) & 7) == 0) {
		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s8 *py = (mlib_s8 *)y;

			ds = ds1 = ds2 = ds3 = 0;
			for (i = 0; i < m; i++) {
				dy = ((mlib_d64 *)py)[0];
				fx = ((mlib_f32 *)dpx)[i];
				VECTOR_MULM_U8_SAT;
				VECTOR_SUM_S32;
				py += nstride;
			}

			PACK_S32_U8_SAT16_SWAP;
/* store 8 x mlib_s16 of result */
			dpz[0] = ds;
			dpz[1] = ds2;
			y = (void *)((mlib_s8 *)y + 8);
			dpz += 2;
		}
	} else {
		mlib_s32 off = (mlib_s16 *)dpz - z, emask =
			vis_edge16(z, pzend);

		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s8 *py = (mlib_s8 *)y;

			ds = ds1 = ds2 = ds3 = 0;
			dpy = (mlib_d64 *)vis_alignaddr(py, off);
			dy_hi = vis_ld_d64_nf(dpy);
			dy_lo = vis_ld_d64_nf(dpy + 1);
			dy = vis_faligndata(dy_hi, dy_lo);
			fx = ((mlib_f32 *)dpx)[0];
			VECTOR_MULM_U8_SAT16;
			py += nstride;
			for (i = 1; i < m; i++) {
				VECTOR_SUM_S32;
				dpy = (mlib_d64 *)vis_alignaddr(py, off);
				dy_hi = vis_ld_d64_nf(dpy);
				dy_lo = vis_ld_d64_nf(dpy + 1);
				dy = vis_faligndata(dy_hi, dy_lo);
				fx = ((mlib_f32 *)dpx)[i];
				VECTOR_MULM_U8_SAT16;
				py += nstride;
			}

			VECTOR_SUM_S32;
			PACK_S32_U8_SAT16;
/* store first 4 x mlib_s16 of result */
			vis_pst_16(ds, dpz, emask);
			dpz++;

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				emask = vis_edge16(dpz, pzend);
/* store second 4 x mlib_s16 of result */
				vis_pst_16(ds2, dpz, emask);
			}

			y = (void *)((mlib_s8 *)y + 8);
			dpz++;
			emask = vis_edge16(dpz, pzend);
		}
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	PACK_S32_U8_MOD16                                       \
	ds2 = vis_fpack32(ds2, dzero);                          \
	/*                                                      \
	 * shift left 8 bits                                    \
	 */                                                     \
	ds2 = vis_fpack32(ds2, dzero);                          \
	/*                                                      \
	 * shift left 8 bits                                    \
	 */                                                     \
	ds3 = vis_fpack32(ds3, dzero);                          \
	/*                                                      \
	 * shift left 8 bits                                    \
	 */                                                     \
	ds3 = vis_fpack32(ds3, dzero);                          \
	/*                                                      \
	 * shift left 8 bits                                    \
	 */                                                     \
	ds = vis_fand(ds, mask_lo);                             \
	ds = vis_for(ds, ds2);                                  \
	ds1 = vis_fand(ds1, mask_lo);                           \
	ds1 = vis_for(ds1, ds3)

/* *********************************************************** */

#define	PACK_S32_U8_MOD16_1                                        \
	/*                                                         \
	 * for use with VECTOR_MULM_U8_SAT16                       \
	 */                                                        \
	ds4 = vis_freg_pair(vis_read_hi(ds), vis_read_hi(ds1));    \
	ds4 = vis_fpack32(ds4, dzero);                             \
	ds4 = vis_fpack32(ds4, dzero);                             \
	ds = vis_freg_pair(vis_read_lo(ds), vis_read_lo(ds1));     \
	ds = vis_fand(ds, mask_lo);                                \
	ds = vis_for(ds, ds4);                                     \
	ds4 = vis_freg_pair(vis_read_hi(ds2), vis_read_hi(ds3));   \
	ds4 = vis_fpack32(ds4, dzero);                             \
	ds4 = vis_fpack32(ds4, dzero);                             \
	ds1 = vis_freg_pair(vis_read_lo(ds2), vis_read_lo(ds3));   \
	ds1 = vis_fand(ds1, mask_lo);                              \
	ds1 = vis_for(ds1, ds4)

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_S16_U8_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 size, i;
	mlib_s16 *pzend = (mlib_s16 *)z + n - 1;
	mlib_d64 array[MAX_SIZE], *dpx, *dpy, *dpz;
	mlib_d64 dy_hi, dy_lo, dy_h, dy_l, dy, ds, ds1, ds2, ds3, ds4, dr, dr1,
		dr2, dr3;
	mlib_d64 dzero = vis_fzero();
	mlib_d64 mask_lo = vis_to_double_dup(0x0000FFFF);
	mlib_f32 fx;

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = (m >> 1) + 1;

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_U8, type_S16, mode_Mod,
				x, y, 1, m, n, nstride, z);
	}

	for (i = 0; i < m; i++) {
		((mlib_s16 *)dpx)[2 * i] = ((mlib_s16 *)dpx)[2 * i + 1] =
			((mlib_u8 *)x)[i];
	}

	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
/* for shift in VECTOR_MULM_U8_SAT */
	(void) vis_alignaddr(dpx, 7);

	if (((n | nstride | (mlib_addr)y | (mlib_addr)z) & 7) == 0) {
		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s8 *py = (mlib_s8 *)y;

			ds = ds1 = ds2 = ds3 = 0;
			for (i = 0; i < m; i++) {
				dy = ((mlib_d64 *)py)[0];
				fx = ((mlib_f32 *)dpx)[i];
				VECTOR_MULM_U8_SAT;
				VECTOR_SUM_S32;
				py += nstride;
			}

			PACK_S32_U8_MOD16;
/* store 8 x mlib_s16 of result */
			dpz[0] = ds;
			dpz[1] = ds1;
			y = (void *)((mlib_s8 *)y + 8);
			dpz += 2;
		}
	} else {
		mlib_s32 off = (mlib_s16 *)dpz - z,
			emask =	vis_edge16(z, pzend);

/* initialize GSR scale factor */
		vis_write_gsr(15 << 3);

		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s8 *py = (mlib_s8 *)y;

			ds = ds1 = ds2 = ds3 = 0;

			dpy = (mlib_d64 *)vis_alignaddr(py, off);
			dy_hi = vis_ld_d64_nf(dpy);
			dy_lo = vis_ld_d64_nf(dpy + 1);
			dy = vis_faligndata(dy_hi, dy_lo);
			fx = ((mlib_f32 *)dpx)[0];
			VECTOR_MULM_U8_SAT16;
			py += nstride;
			for (i = 1; i < m; i++) {
				VECTOR_SUM_S32;
				dpy = (mlib_d64 *)vis_alignaddr(py, off);
				dy_hi = vis_ld_d64_nf(dpy);
				dy_lo = vis_ld_d64_nf(dpy + 1);
				dy = vis_faligndata(dy_hi, dy_lo);
				fx = ((mlib_f32 *)dpx)[i];
				VECTOR_MULM_U8_SAT16;
				py += nstride;
			}

			VECTOR_SUM_S32;
			PACK_S32_U8_MOD16_1;
/* store first 4 x mlib_s16 of result */
			vis_pst_16(ds, dpz, emask);
			dpz++;

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				emask = vis_edge16(dpz, pzend);
/* store second 4 x mlib_s16 of result */
				vis_pst_16(ds1, dpz, emask);
			}

			y = (void *)((mlib_s8 *)y + 8);
			dpz++;
			emask = vis_edge16(dpz, pzend);
		}
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 size, i;
	mlib_s8 *pzend = (mlib_s8 *)z + n - 1;
	mlib_d64 array[MAX_SIZE + 8], *dpx, *dpy;
	mlib_d64 dy_hi, dy_lo, dy, ds, ds1, ds2, ds3, dr, dr1, dr2, dr3;
	mlib_f32 fx;

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = (m >> 1) + 1;

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_S8, type_S8, mode_Sat,
				x, y, 1, m, n, nstride, z);
	}

	if (m > M_MLIB_S8_MAX) {
		mlib_s32 m_end, j, *psum = (mlib_s32 *)(array + MAX_SIZE);
		mlib_s8 *pz = (mlib_s8 *)z;

		for (i = 0; i < m; i++) {
			((mlib_s16 *)dpx)[2 * i] =
				((mlib_s16 *)dpx)[2 * i + 1] =
				((mlib_s8 *)x)[i];
		}

		if (((n | nstride | (mlib_addr)y | (mlib_addr)z) & 7) != 0) {
			while ((mlib_addr)pz <= (mlib_addr)pzend) {
				mlib_s8 *py = (mlib_s8 *)y;

				for (j = 0; j < 4; j++)
					((mlib_d64 *)psum)[j] = 0;
				i = 0;
				do {
					m_end = (m <
						(i +
						M_MLIB_S8_MAX * 2)) ? m : i +
						M_MLIB_S8_MAX * 2;
					ds = ds1 = ds2 = ds3 = 0;

					dpy = (mlib_d64 *)vis_alignaddr(py, 0);
					dy_hi = vis_ld_d64_nf(dpy);
					dy_lo = vis_ld_d64_nf(dpy + 1);
					dy = vis_faligndata(dy_hi, dy_lo);
					fx = ((mlib_f32 *)dpx)[i];
					VECTOR_MULM_S8;
					py += nstride;

					for (; i < (m_end - 1); i++) {
						VECTOR_SUM_S32;
						dpy = (mlib_d64 *)
							vis_alignaddr(py, 0);
						dy_hi = vis_ld_d64_nf(dpy);
						dy_lo = vis_ld_d64_nf(dpy + 1);
						dy = vis_faligndata(dy_hi,
							dy_lo);
						fx = ((mlib_f32 *)dpx)[i + 1];
						VECTOR_MULM_S8;
						py += nstride;
					}

					i++;
					VECTOR_SUM_S32;
					((mlib_d64 *)psum)[4] = ds;
					((mlib_d64 *)psum)[5] = ds1;
					((mlib_d64 *)psum)[6] = ds2;
					((mlib_d64 *)psum)[7] = ds3;
					for (j = 0; j < 8; j++)
						psum[j] += psum[j + 8] >> 8;
/* because of the compiler error */
					i = m_end;
				}

				while (m_end < m);
				for (j = 0;
					j < 4 &&
					((mlib_addr)pz + 2 * j) <=
					(mlib_addr)pzend; j++) {
					mlib_s32 s = psum[j];

					if (s > MLIB_S8_MAX)
						s = MLIB_S8_MAX;
					else if (s < MLIB_S8_MIN)
						s = MLIB_S8_MIN;
					((mlib_s8 *)pz)[2 * j] = s;

					if (((mlib_addr)pz + 2 * j + 1) <=
						(mlib_addr)pzend) {
						mlib_s32 s = psum[4 + j];

						if (s > MLIB_S8_MAX)
							s = MLIB_S8_MAX;
						else if (s < MLIB_S8_MIN)
							s = MLIB_S8_MIN;
						((mlib_s8 *)pz)[2 * j + 1] = s;
					}
				}

				y = (void *)((mlib_s8 *)y + 8);
				pz += 8;
			}
		} else {
			while ((mlib_addr)pz <= (mlib_addr)pzend) {
				mlib_s8 *py = (mlib_s8 *)y;

				for (j = 0; j < 4; j++)
					((mlib_d64 *)psum)[j] = 0;
				i = 0;
				do {
					m_end = (m <
						(i +
						M_MLIB_S8_MAX * 2)) ? m : i +
						M_MLIB_S8_MAX * 2;
					ds = ds1 = ds2 = ds3 = 0;
					for (; i < m_end; i++) {
						dy = ((mlib_d64 *)py)[0];
						fx = ((mlib_f32 *)dpx)[i];
						VECTOR_MULM_S8;
						VECTOR_SUM_S32;
						py += nstride;
					}

					((mlib_d64 *)psum)[4] = ds;
					((mlib_d64 *)psum)[5] = ds1;
					((mlib_d64 *)psum)[6] = ds2;
					((mlib_d64 *)psum)[7] = ds3;
					for (j = 0; j < 8; j++)
						psum[j] += psum[j + 8] >> 8;
/* because of the compiler error */
					i = m_end;
				}

				while (m_end < m);
				for (j = 0;
					j < 4 &&
					((mlib_addr)pz + 2 * j) <=
					(mlib_addr)pzend; j++) {
					mlib_s32 s = psum[j];

					if (s > MLIB_S8_MAX)
						s = MLIB_S8_MAX;
					else if (s < MLIB_S8_MIN)
						s = MLIB_S8_MIN;
					((mlib_s8 *)pz)[2 * j] = s;

					if (((mlib_addr)pz + 2 * j + 1) <=
						(mlib_addr)pzend) {
						mlib_s32 s = psum[4 + j];

						if (s > MLIB_S8_MAX)
							s = MLIB_S8_MAX;
						else if (s < MLIB_S8_MIN)
							s = MLIB_S8_MIN;
						((mlib_s8 *)pz)[2 * j + 1] = s;
					}
				}

				y = (void *)((mlib_s8 *)y + 8);
				pz += 8;
			}
		}
	} else {
		mlib_d64 displacement = vis_to_double_dup(0x10000);
		mlib_d64 restore = vis_to_double_dup(0x80808080);
		mlib_d64 *dpz = (mlib_d64 *)((mlib_addr)z & (~7));

/* initialize GSR scale factor */
		vis_write_gsr(7 << 3);

		for (i = 0; i < m; i++) {
			((mlib_s16 *)dpx)[2 * i] =
				((mlib_s16 *)dpx)[2 * i + 1] =
				((mlib_s8 *)x)[i] * 2;
		}

		if (((n | nstride | (mlib_addr)y | (mlib_addr)z) & 7) == 0) {
			while ((mlib_addr)dpz <= (mlib_addr)pzend) {
				mlib_s8 *py = (mlib_s8 *)y;

				ds = ds1 = ds2 = ds3 = displacement;
				for (i = 0; i < m; i++) {
					dy = ((mlib_d64 *)py)[0];
					fx = ((mlib_f32 *)dpx)[i];
					VECTOR_MULM_S8;
					VECTOR_SUM_S32;
					py += nstride;
				}

				PACK_S32_S8_SAT;
/* store 8 bytes of result */
				dpz[0] = ds;
				y = (void *)((mlib_s8 *)y + 8);
				dpz++;
			}
		} else {
			mlib_s32 off = (mlib_addr)dpz - (mlib_addr)z,
/*
 * generate edge mask for the start point
 */
				emask = vis_edge8(z, pzend);

			while ((mlib_addr)dpz <= (mlib_addr)pzend) {
				mlib_s8 *py = (mlib_s8 *)y;

				ds = ds1 = ds2 = ds3 = displacement;

				dpy = (mlib_d64 *)vis_alignaddr(py, off);
				dy_hi = vis_ld_d64_nf(dpy);
				dy_lo = vis_ld_d64_nf(dpy + 1);
				dy = vis_faligndata(dy_hi, dy_lo);
				fx = ((mlib_f32 *)dpx)[0];
				VECTOR_MULM_S8;
				py += nstride;

				for (i = 1; i < m; i++) {
					VECTOR_SUM_S32;
					dpy = (mlib_d64 *)vis_alignaddr(py,
						off);
					dy_hi = vis_ld_d64_nf(dpy);
					dy_lo = vis_ld_d64_nf(dpy + 1);
					dy = vis_faligndata(dy_hi, dy_lo);
					fx = ((mlib_f32 *)dpx)[i];
					VECTOR_MULM_S8;
					py += nstride;
				}

				VECTOR_SUM_S32;
				PACK_S32_S8_SAT;
/* store 8 bytes of result */
				vis_pst_8(ds, dpz, emask);
				y = (void *)((mlib_s8 *)y + 8);
				dpz++;
				emask = vis_edge8(dpz, pzend);
			}
		}
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_S8_S8_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 size, i;
	mlib_s8 *pzend = (mlib_s8 *)z + n - 1;
	mlib_d64 array[MAX_SIZE], *dpx, *dpy, *dpz;
	mlib_d64 dx, dy_hi, dy_lo, dy, ds, ds1, dr, dr1;
	mlib_d64 without_control = vis_to_double_dup(0xff00ff);

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = (m >> 2) + 1;

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_S8, type_S8, mode_Mod,
				x, y, 1, m, n, nstride, z);
	}

	for (i = 0; i < m; i++) {
		((mlib_s16 *)dpx)[i] = ((mlib_s8 *)x)[i] << 8;
	}

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));

	if (((n | nstride | (mlib_addr)y | (mlib_addr)z) & 7) != 0) {
		mlib_s32 off = (mlib_addr)dpz - (mlib_addr)z,
/*
 * generate edge mask for the start point
 */
			emask = vis_edge8(z, pzend);

		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s8 *py = (mlib_s8 *)y;

			ds = ds1 = 0;

			for (i = 0; i < m; i++) {
				dpy = (mlib_d64 *)vis_alignaddr(py, off);
				dy_hi = vis_ld_d64_nf(dpy);
				dy_lo = vis_ld_d64_nf(dpy + 1);
				dy = vis_faligndata(dy_hi, dy_lo);
				dx = vis_ld_u16((mlib_s16 *)dpx + i);
				VECTOR_MULM_S8_MOD;
				VECTOR_SUM_S16_MOD;
				py += nstride;
			}

			PACK_S16_S8_MOD;
/* store 8 bytes of result */
			vis_pst_8(ds, dpz, emask);
			y = (void *)((mlib_s8 *)y + 8);
			dpz++;
			emask = vis_edge8(dpz, pzend);
		}
	} else {
		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s8 *py = (mlib_s8 *)y;

			ds = ds1 = 0;
			for (i = 0; i < m; i++) {
				dy = ((mlib_d64 *)py)[0];
				dx = vis_ld_u16((mlib_s16 *)dpx + i);
				VECTOR_MULM_S8_MOD;
				VECTOR_SUM_S16_MOD;
				py += nstride;
			}

			PACK_S16_S8_MOD;
/* store 8 bytes of result */
			dpz[0] = ds;
			y = (void *)((mlib_s8 *)y + 8);
			dpz++;
		}
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	PACK_S32_S8_SAT16                                                \
	{                                                                \
	/*                                                               \
	 * for use with VECTOR_MULM_S8                                   \
	 */                                                              \
	    mlib_d64 ds4, ds5;                                           \
	                                                                 \
	    ds4 = vis_freg_pair(vis_read_hi(ds), vis_read_hi(ds2));      \
	    ds2 = vis_freg_pair(vis_read_lo(ds), vis_read_lo(ds2));      \
	    ds5 = vis_freg_pair(vis_read_hi(ds1), vis_read_hi(ds3));     \
	    ds1 = vis_freg_pair(vis_read_lo(ds1), vis_read_lo(ds3));     \
	    ds = vis_freg_pair(vis_fpackfix(ds4), vis_fpackfix(ds2));    \
	    ds2 = vis_freg_pair(vis_fpackfix(ds5), vis_fpackfix(ds1));   \
	}

#define	M_MLIB_S8_MLIB_S16_MAX	255

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_S16_S8_Sat(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 size, i;
	mlib_s16 *pzend = (mlib_s16 *)z + n - 1;
	mlib_d64 array[MAX_SIZE + 8], *dpx, *dpy;
	mlib_d64 dy_hi, dy_lo, dy, ds, ds1, ds2, ds3, dr, dr1, dr2, dr3;
	mlib_f32 fx;

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = (m >> 1) + 1;

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_S8, type_S16, mode_Sat,
				x, y, 1, m, n, nstride, z);
	}

	if (m > M_MLIB_S8_MLIB_S16_MAX) {
		mlib_s32 m_end, j, *psum = (mlib_s32 *)(array + MAX_SIZE);
		mlib_s16 *pz = (mlib_s16 *)z;

		for (i = 0; i < m; i++) {
			((mlib_s16 *)dpx)[2 * i] =
				((mlib_s16 *)dpx)[2 * i + 1] =
				((mlib_s8 *)x)[i];
		}

		if (((n | nstride | (mlib_addr)y | (mlib_addr)z) & 7) != 0) {
			while ((mlib_addr)pz <= (mlib_addr)pzend) {
				mlib_s8 *py = (mlib_s8 *)y;

				for (j = 0; j < 4; j++)
					((mlib_d64 *)psum)[j] = 0;
				i = 0;
				do {
					m_end = (m < (i +
						M_MLIB_S8_MLIB_S16_MAX *
						2)) ? m : i +
						M_MLIB_S8_MLIB_S16_MAX * 2;
					ds = ds1 = ds2 = ds3 = 0;

					for (; i < (m_end); i++) {
						fx = ((mlib_f32 *)dpx)[i];
						dpy = (mlib_d64 *)
							vis_alignaddr(py, 0);
						dy_hi = vis_ld_d64_nf(dpy);
						dy_lo = vis_ld_d64_nf(dpy + 1);
						dy = vis_faligndata(dy_hi,
							dy_lo);
						VECTOR_MULM_S8;
						py += nstride;
						VECTOR_SUM_S32;
					}

					((mlib_d64 *)psum)[4] = ds;
					((mlib_d64 *)psum)[5] = ds1;
					((mlib_d64 *)psum)[6] = ds2;
					((mlib_d64 *)psum)[7] = ds3;
					for (j = 0; j < 8; j++)
						psum[j] += psum[j + 8] >> 8;
/* because of the compiler error */
					i = m_end;
				}

				while (m_end < m);

				for (j = 0; j < 4 &&
					((mlib_addr)pz + 4 * j) <=
					(mlib_addr)pzend; j++) {
					mlib_s32 s = psum[j];

					if (s > MLIB_S16_MAX)
						s = MLIB_S16_MAX;
					else if (s < MLIB_S16_MIN)
						s = MLIB_S16_MIN;
					((mlib_s16 *)pz)[2 * j] = s;

					if (((mlib_addr)pz + 4 * j + 2) <=
						(mlib_addr)pzend) {
						mlib_s32 s = psum[4 + j];

						if (s > MLIB_S16_MAX)
							s = MLIB_S16_MAX;
						else if (s < MLIB_S16_MIN)
							s = MLIB_S16_MIN;
						((mlib_s16 *)pz)[2 * j + 1] = s;
					}
				}

				y = (void *)((mlib_s8 *)y + 8);
				pz += 8;
			}
		} else {
			while ((mlib_addr)pz <= (mlib_addr)pzend) {
				mlib_s8 *py = (mlib_s8 *)y;

				for (j = 0; j < 4; j++)
					((mlib_d64 *)psum)[j] = 0;
				i = 0;
				do {
					m_end = (m < (i +
						M_MLIB_S8_MLIB_S16_MAX *
						2)) ? m : i +
						M_MLIB_S8_MLIB_S16_MAX * 2;
					ds = ds1 = ds2 = ds3 = 0;
					for (; i < m_end; i++) {
						dy = ((mlib_d64 *)py)[0];
						fx = ((mlib_f32 *)dpx)[i];
						VECTOR_MULM_S8;
						VECTOR_SUM_S32;
						py += nstride;
					}

					((mlib_d64 *)psum)[4] = ds;
					((mlib_d64 *)psum)[5] = ds1;
					((mlib_d64 *)psum)[6] = ds2;
					((mlib_d64 *)psum)[7] = ds3;
					for (j = 0; j < 8; j++)
						psum[j] += psum[j + 8] >> 8;
/* because of the compiler error */
					i = m_end;
				}

				while (m_end < m);

				for (j = 0;
					j < 4 &&
					((mlib_addr)pz + 4 * j) <=
					(mlib_addr)pzend; j++) {
					mlib_s32 s = psum[j];

					if (s > MLIB_S16_MAX)
						s = MLIB_S16_MAX;
					else if (s < MLIB_S16_MIN)
						s = MLIB_S16_MIN;
					((mlib_s16 *)pz)[2 * j] = s;

					if (((mlib_addr)pz + 4 * j + 2) <=
						(mlib_addr)pzend) {
						mlib_s32 s = psum[4 + j];

						if (s > MLIB_S16_MAX)
							s = MLIB_S16_MAX;
						else if (s < MLIB_S16_MIN)
							s = MLIB_S16_MIN;
						((mlib_s16 *)pz)[2 * j + 1] = s;
					}
				}

				y = (void *)((mlib_s8 *)y + 8);
				pz += 8;
			}
		}
	} else {
		mlib_d64 *dpz = (mlib_d64 *)((mlib_addr)z & (~7));

/* initialize GSR scale factor */
		vis_write_gsr(7 << 3);

		for (i = 0; i < m; i++) {
			((mlib_s16 *)dpx)[2 * i] =
				((mlib_s16 *)dpx)[2 * i + 1] =
				((mlib_s8 *)x)[i] * 2;
		}

		if (((n | nstride | (mlib_addr)y | (mlib_addr)z) & 7) == 0) {
			while ((mlib_addr)dpz <= (mlib_addr)pzend) {
				mlib_s8 *py = (mlib_s8 *)y;

				ds = ds1 = ds2 = ds3 = 0;
				for (i = 0; i < m; i++) {
					dy = ((mlib_d64 *)py)[0];
					fx = ((mlib_f32 *)dpx)[i];
					VECTOR_MULM_S8;
					VECTOR_SUM_S32;
					py += nstride;
				}

				PACK_S32_S8_SAT16;
/* store 1st 4 mlib_s16 of result */
				dpz[0] = ds;
/* store 2nd 4 mlib_s16 of result */
				dpz[1] = ds2;
				y = (void *)((mlib_s8 *)y + 8);
				dpz += 2;
			}
		} else {
			mlib_s32 off = (mlib_s16 *)dpz - z,
/*
 * generate edge mask for the start point
 */
				emask = vis_edge16(z, pzend);

			while ((mlib_addr)dpz <= (mlib_addr)pzend) {
				mlib_s8 *py = (mlib_s8 *)y;

				ds = ds1 = ds2 = ds3 = 0;

				dpy = (mlib_d64 *)vis_alignaddr(py, off);
				dy_hi = vis_ld_d64_nf(dpy);
				dy_lo = vis_ld_d64_nf(dpy + 1);
				dy = vis_faligndata(dy_hi, dy_lo);
				fx = ((mlib_f32 *)dpx)[0];
				VECTOR_MULM_S8;
				py += nstride;
				for (i = 1; i < m; i++) {
					VECTOR_SUM_S32;
					dpy = (mlib_d64 *)vis_alignaddr(py,
						off);
					dy_hi = vis_ld_d64_nf(dpy);
					dy_lo = vis_ld_d64_nf(dpy + 1);
					dy = vis_faligndata(dy_hi, dy_lo);
					fx = ((mlib_f32 *)dpx)[i];
					VECTOR_MULM_S8;
					py += nstride;
				}

				VECTOR_SUM_S32;
				PACK_S32_S8_SAT16;
/* store 1st 4 x mlib_s16 of result */
				vis_pst_16(ds, dpz, emask);
				dpz++;

				if ((mlib_addr)dpz <= (mlib_addr)pzend) {
					emask = vis_edge16(dpz, pzend);
/* store 2nd 4 x mlib_s16 of result */
					vis_pst_16(ds2, dpz, emask);
				}

				y = (void *)((mlib_s8 *)y + 8);
				dpz++;
				emask = vis_edge16(dpz, pzend);
			}
		}
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	VECTOR_MULM_S8_MOD16                                    \
	dy_h = vis_fpmerge(vis_read_hi(dy), dzero);             \
	dy_l = vis_fpmerge(vis_read_lo(dy), dzero);             \
	dr = vis_fmul8sux16(dy_h, dx);                          \
	dr1 = vis_fmul8sux16(dy_l, dx)

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_S16_S8_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 size, i;
	mlib_s16 *pzend = (mlib_s16 *)z + n - 1;
	mlib_d64 array[MAX_SIZE], *dpx, *dpy, *dpz;
	mlib_d64 dx, dy_hi, dy_lo, dy_h, dy_l, dy, ds, ds1, dr, dr1;
	mlib_d64 dzero = vis_fzero();

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = m + 1;

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_S8, type_S16, mode_Mod,
				x, y, 1, m, n, nstride, z);
	}

	for (i = 0; i < m; i++) {
		((mlib_s16 *)dpx)[4 * i] =
			((mlib_s16 *)dpx)[4 * i + 1] =
			((mlib_s16 *)dpx)[4 * i + 2] =
			((mlib_s16 *)dpx)[4 * i + 3] = ((mlib_s8 *)x)[i] << 8;
	}

	dpz = (mlib_d64 *)((mlib_addr)z & (~7));

	if (((n | nstride | (mlib_addr)y | (mlib_addr)z) & 7) != 0) {
		mlib_s32 off = (mlib_s16 *)dpz - z,
/*
 * generate edge mask for the start point
 */
			emask = vis_edge16(z, pzend);

		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s8 *py = (mlib_s8 *)y;

			ds = ds1 = 0;

			dpy = (mlib_d64 *)vis_alignaddr(py, off);
			dy_hi = vis_ld_d64_nf(dpy);
			dy_lo = vis_ld_d64_nf(dpy + 1);
			dy = vis_faligndata(dy_hi, dy_lo);
			dx = dpx[0];
			VECTOR_MULM_S8_MOD16;
			py += nstride;
			for (i = 1; i < m; i++) {
				VECTOR_SUM_S16_MOD;
				dpy = (mlib_d64 *)vis_alignaddr(py, off);
				dy_hi = vis_ld_d64_nf(dpy);
				dy_lo = vis_ld_d64_nf(dpy + 1);
				dy = vis_faligndata(dy_hi, dy_lo);
				dx = dpx[i];
				VECTOR_MULM_S8_MOD16;
				py += nstride;
			}

			VECTOR_SUM_S16_MOD;
/* store 1st 4 x mlib_s16 of result */
			vis_pst_16(ds, dpz, emask);
			dpz++;

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				emask = vis_edge16(dpz, pzend);
/* store 2nd 4 x mlib_s16 of result */
				vis_pst_16(ds1, dpz, emask);
			}

			y = (void *)((mlib_s8 *)y + 8);
			dpz++;
			emask = vis_edge16(dpz, pzend);
		}
	} else {
		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s8 *py = (mlib_s8 *)y;

			ds = ds1 = 0;
			for (i = 0; i < m; i++) {
				dy = ((mlib_d64 *)py)[0];
				dx = dpx[i];
				VECTOR_MULM_S8_MOD16;
				VECTOR_SUM_S16_MOD;
				py += nstride;
			}

/* store 1st 4 x mlib_s16 of result */
			dpz[0] = ds;
/* store 2st 4 x mlib_s16 of result */
			dpz[1] = ds1;
			y = (void *)((mlib_s8 *)y + 8);
			dpz += 2;
		}
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	VECTOR_MULM_S16                                         \
	{                                                       \
	    mlib_d64 dr2, dr3, dr4, dr5;                        \
	                                                        \
	    dr4 = vis_fmuld8sux16(fx, vis_read_hi(dy));         \
	    dr5 = vis_fmuld8ulx16(fx, vis_read_hi(dy));         \
	    dr.db = vis_fpadd32(dr4, dr5);                      \
	    dr2 = vis_fmuld8sux16(fx, vis_read_lo(dy));         \
	    dr3 = vis_fmuld8ulx16(fx, vis_read_lo(dy));         \
	    dr1.db = vis_fpadd32(dr2, dr3);                     \
	}

/* *********************************************************** */

#ifndef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(DST, SRC)                               \
	{                                                       \
	    mlib_d64 dsrc = (mlib_d64)(SRC);                    \
	                                                        \
	    if (dsrc > (mlib_d64)MLIB_S32_MAX)                  \
		dsrc = (mlib_d64)MLIB_S32_MAX;                  \
	    if (dsrc < (mlib_d64)MLIB_S32_MIN)                  \
		dsrc = (mlib_d64)MLIB_S32_MIN;                  \
	    DST = (mlib_s32)dsrc;                               \
	}

#else

#define	FLOAT2INT_CLAMP(DST, SRC)	DST = ((mlib_s32)(SRC))

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	VECTOR_SUM_D64                                          \
	ds += (mlib_d64)dr.two_int.int0;                        \
	ds1 += (mlib_d64)dr.two_int.int1;                       \
	ds2 += (mlib_d64)dr1.two_int.int0;                      \
	ds3 += (mlib_d64)dr1.two_int.int1

/* *********************************************************** */

#define	PACK_D64_S16                                                    \
	FLOAT2INT_CLAMP(dr.two_int.int0, ds + ds);                      \
	FLOAT2INT_CLAMP(dr.two_int.int1, ds1 + ds1);                    \
	FLOAT2INT_CLAMP(dr1.two_int.int0, ds2 + ds2);                   \
	FLOAT2INT_CLAMP(dr1.two_int.int1, ds3 + ds3);                   \
	ds = vis_freg_pair(vis_fpackfix(dr.db), vis_fpackfix(dr1.db))

/* *********************************************************** */

#define	PACK_D64_S16_C                                          \
	{                                                       \
	    mlib_s16 s[4];                                      \
	                                                        \
	    if (ds > MLIB_S16_MAX)                              \
		s[0] = MLIB_S16_MAX;                            \
	    else if (ds < MLIB_S16_MIN)                         \
		s[0] = MLIB_S16_MIN;                            \
	    else                                                \
		s[0] = (mlib_s32)ds;                            \
	    if (ds1 > MLIB_S16_MAX)                             \
		s[1] = MLIB_S16_MAX;                            \
	    else if (ds1 < MLIB_S16_MIN)                        \
		s[1] = MLIB_S16_MIN;                            \
	    else                                                \
		s[1] = (mlib_s32)ds1;                           \
	    if (ds2 > MLIB_S16_MAX)                             \
		s[2] = MLIB_S16_MAX;                            \
	    else if (ds2 < MLIB_S16_MIN)                        \
		s[2] = MLIB_S16_MIN;                            \
	    else                                                \
		s[2] = (mlib_s32)ds2;                           \
	    if (ds3 > MLIB_S16_MAX)                             \
		s[3] = MLIB_S16_MAX;                            \
	    else if (ds3 < MLIB_S16_MIN)                        \
		s[3] = MLIB_S16_MIN;                            \
	    else                                                \
		s[3] = (mlib_s32)ds3;                           \
	    ds = *(mlib_d64 *)s;                                \
	}

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s16 *pzend = (mlib_s16 *)z + n - 1;
	mlib_d64 array[MAX_SIZE], *dpx, *dpy, *dpz;
	mlib_d64 dy_hi, dy_lo, dy, ds, ds1, ds2, ds3;
	type_union_mlib_d64 dr, dr1;
	mlib_f32 fx;
	mlib_s32 size, i;

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);

	size = (m >> 1) + 1;

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_S16, type_S16, mode_Sat,
				x, y, 1, m, n, nstride, z);
	}

	for (i = 0; i < m; i++) {
		((mlib_s16 *)dpx)[2 * i] = ((mlib_s16 *)dpx)[2 * i + 1] =
			((mlib_s16 *)x)[i];
	}

/* initialize GSR scale factor */
	vis_write_gsr(15 << 3);
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));

	if ((((nstride + nstride) | (n + n) |
		(mlib_addr)y | (mlib_addr)z) & 7) != 0) {
		mlib_s32 off = (mlib_addr)dpz - (mlib_addr)z,
/*
 * generate edge mask for the start point
 */
			emask = vis_edge16(z, pzend);

		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s16 *py = (mlib_s16 *)y;

			ds = ds1 = ds2 = ds3 = 0;

			for (i = 0; i < m; i++) {
				dpy = (mlib_d64 *)vis_alignaddr(py, off);
				dy_hi = vis_ld_d64_nf(dpy);
				dy_lo = vis_ld_d64_nf(dpy + 1);
				dy = vis_faligndata(dy_hi, dy_lo);
				fx = ((mlib_f32 *)dpx)[i];
				VECTOR_MULM_S16;
				VECTOR_SUM_D64;
				py += nstride;
			}

			PACK_D64_S16_C;
/* store 8 bytes of result */
			vis_pst_16(ds, dpz, emask);
			y = (void *)((mlib_s16 *)y + 4);
			dpz++;
			emask = vis_edge16(dpz, pzend);
		}
	} else {
		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s16 *py = (mlib_s16 *)y;

			ds = ds1 = ds2 = ds3 = 0;

			for (i = 0; i < m; i++) {
				dy = ((mlib_d64 *)py)[0];
				fx = ((mlib_f32 *)dpx)[i];
				VECTOR_MULM_S16;
				VECTOR_SUM_D64;
				py += nstride;
			}

			PACK_D64_S16;
/* store 8 bytes of result */
			dpz[0] = ds;
			y = (void *)((mlib_s16 *)y + 4);
			dpz++;
		}
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

#undef VECTOR_SUM_S32

/* *********************************************************** */

#define	VECTOR_SUM_S32                                          \
	ds = vis_fpadd32(ds, dr.db);                            \
	ds1 = vis_fpadd32(ds1, dr1.db)

/* *********************************************************** */

#define	PACK_S32_S16                                              \
	{                                                         \
	    mlib_f32 sr0, sr1;                                    \
	                                                          \
	    sr0 = vis_fands(vis_read_lo(ds), without_control);    \
	    sr1 = vis_fands(vis_read_hi(ds1), without_control);   \
	    ds = vis_write_lo(ds, sr1);                           \
	    ds = vis_faligndata(ds, dzero);                       \
	    sr1 = vis_fands(vis_read_lo(ds1), without_control);   \
	    ds = vis_for(ds, vis_freg_pair(sr0, sr1));            \
	}

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 size, i;
	mlib_s16 *pzend = (mlib_s16 *)z + n - 1;
	mlib_d64 array[MAX_SIZE], *dpx, *dpy, *dpz;
	mlib_d64 dy_hi, dy_lo, dy, ds, ds1;
	mlib_d64 dzero = vis_fzero();
	type_union_mlib_d64 dr, dr1;
	mlib_f32 fx, without_control = vis_to_float(0xffff);

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = (m >> 1) + 1;

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_S16, type_S16, mode_Mod,
				x, y, 1, m, n, nstride, z);
	}

	for (i = 0; i < m; i++) {
		((mlib_s16 *)dpx)[2 * i] = ((mlib_s16 *)dpx)[2 * i + 1] =
			((mlib_s16 *)x)[i];
	}

	dpz = (mlib_d64 *)((mlib_addr)z & (~7));

	if ((((nstride + nstride) | (n + n) |
		(mlib_addr)y | (mlib_addr)z) & 7) != 0) {
		mlib_s32 off = (mlib_addr)dpz - (mlib_addr)z,
/*
 * generate edge mask for the start point
 */
			emask = vis_edge16(z, pzend);

		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s16 *py = (mlib_s16 *)y;

			ds = ds1 = dzero;

			dpy = (mlib_d64 *)vis_alignaddr(py, off);
			dy_hi = vis_ld_d64_nf(dpy);
			dy_lo = vis_ld_d64_nf(dpy + 1);
			dy = vis_faligndata(dy_hi, dy_lo);
			fx = ((mlib_f32 *)dpx)[0];
			VECTOR_MULM_S16;
			py += nstride;
			for (i = 1; i < m; i++) {
				VECTOR_SUM_S32;
				dpy = (mlib_d64 *)vis_alignaddr(py, off);
				dy_hi = vis_ld_d64_nf(dpy);
				dy_lo = vis_ld_d64_nf(dpy + 1);
				dy = vis_faligndata(dy_hi, dy_lo);
				fx = ((mlib_f32 *)dpx)[i];
				VECTOR_MULM_S16;
				py += nstride;
			}

			VECTOR_SUM_S32;
			(void) vis_alignaddr(dpz, 2);
			PACK_S32_S16;
/* store 8 bytes of result */
			vis_pst_16(ds, dpz, emask);
			y = (void *)((mlib_s16 *)y + 4);
			dpz++;
			emask = vis_edge16(dpz, pzend);
		}
	} else {
		(void) vis_alignaddr(dpz, 2);
		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s16 *py = (mlib_s16 *)y;

			ds = ds1 = dzero;

			for (i = 0; i < m; i++) {
				dy = ((mlib_d64 *)py)[0];
				fx = ((mlib_f32 *)dpx)[i];
				VECTOR_MULM_S16;
				VECTOR_SUM_S32;
				py += nstride;
			}

			PACK_S32_S16;
/* store 8 bytes of result */
			dpz[0] = ds;
			y = (void *)((mlib_s16 *)y + 4);
			dpz++;
		}
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_S32_S16_Sat(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 size, i;
	mlib_s32 *pz = (mlib_s32 *)z, *pzend = (mlib_s32 *)z + n - 1;
	mlib_d64 array[MAX_SIZE], *dpx, *dpy;
	mlib_d64 dy_hi, dy_lo, dy, ds, ds1, ds2, ds3;
	type_union_mlib_d64 dr, dr1;
	mlib_f32 fx;

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = (m >> 1) + 1;

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_S16, type_S32, mode_Sat,
				x, y, 1, m, n, nstride, z);
	}

	for (i = 0; i < m; i++) {
		((mlib_s16 *)dpx)[2 * i] = ((mlib_s16 *)dpx)[2 * i + 1] =
			((mlib_s16 *)x)[i];
	}

	if ((((nstride + nstride) | (n + n) |
		(mlib_addr)y | (mlib_addr)z) & 7) != 0) {

		while ((mlib_addr)pz <= (mlib_addr)pzend) {
			mlib_s16 *py = (mlib_s16 *)y;

			ds = ds1 = ds2 = ds3 = 0;

			for (i = 0; i < m; i++) {
				dpy = (mlib_d64 *)vis_alignaddr(py, 0);
				dy_hi = vis_ld_d64_nf(dpy);
				dy_lo = vis_ld_d64_nf(dpy + 1);
				dy = vis_faligndata(dy_hi, dy_lo);
				fx = ((mlib_f32 *)dpx)[i];
				VECTOR_MULM_S16;
				VECTOR_SUM_D64;
				py += nstride;
			}

			if ((mlib_addr)pz <= (mlib_addr)pzend) {
				FLOAT2INT_CLAMP(pz[0], ds);
				pz++;

				if ((mlib_addr)pz <= (mlib_addr)pzend) {
					FLOAT2INT_CLAMP(pz[0], ds1);
					pz++;

					if ((mlib_addr)pz <= (mlib_addr)pzend) {
						FLOAT2INT_CLAMP(pz[0], ds2);
						pz++;

						if ((mlib_addr)pz <=
							(mlib_addr)pzend) {
							FLOAT2INT_CLAMP(pz[0],
								ds3);
							pz++;
						}
					}
				}
			}

			y = (void *)((mlib_s16 *)y + 4);
		}
	} else {
		while ((mlib_addr)pz <= (mlib_addr)pzend) {
			mlib_s16 *py = (mlib_s16 *)y;

			ds = ds1 = ds2 = ds3 = 0;

			for (i = 0; i < m; i++) {
				dy = ((mlib_d64 *)py)[0];
				fx = ((mlib_f32 *)dpx)[i];
				VECTOR_MULM_S16;
				VECTOR_SUM_D64;
				py += nstride;
			}

			FLOAT2INT_CLAMP(pz[0], ds);
			FLOAT2INT_CLAMP(pz[1], ds1);
			FLOAT2INT_CLAMP(pz[2], ds2);
			FLOAT2INT_CLAMP(pz[3], ds3);
			pz += 4;
			y = (void *)((mlib_s16 *)y + 4);
		}
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	VECTOR_SUM_S32                                          \
	ds = vis_fpadd32(ds, dr.db);                            \
	ds1 = vis_fpadd32(ds1, dr1.db)

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_S32_S16_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 size, i;
	mlib_s32 *pzend = (mlib_s32 *)z + n - 1;
	mlib_d64 array[MAX_SIZE], *dpx, *dpy, *dpz;
	mlib_d64 dy_hi, dy_lo, dy, ds, ds1;
	mlib_d64 dzero = vis_fzero();
	type_union_mlib_d64 dr, dr1;
	mlib_f32 fx;

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = (m >> 1) + 1;

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_S16, type_S32, mode_Mod,
				x, y, 1, m, n, nstride, z);
	}

	for (i = 0; i < m; i++) {
		((mlib_s16 *)dpx)[2 * i] = ((mlib_s16 *)dpx)[2 * i + 1] =
			((mlib_s16 *)x)[i];
	}

	dpz = (mlib_d64 *)((mlib_addr)z & (~7));

	if ((((nstride + nstride) | (n + n) |
		(mlib_addr)y | (mlib_addr)z) & 7) != 0) {
		mlib_s32 off = (mlib_s16 *)dpz - (mlib_s16 *)z,
/*
 * generate edge mask for the start point
 */
			emask = vis_edge32(z, pzend);

		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s16 *py = (mlib_s16 *)y;

			ds = ds1 = dzero;

			dpy = (mlib_d64 *)vis_alignaddr(py, off);
			dy_hi = vis_ld_d64_nf(dpy);
			dy_lo = vis_ld_d64_nf(dpy + 1);
			dy = vis_faligndata(dy_hi, dy_lo);
			fx = ((mlib_f32 *)dpx)[0];
			VECTOR_MULM_S16;
			py += nstride;
			for (i = 1; i < m; i++) {
				VECTOR_SUM_S32;
				dpy = (mlib_d64 *)vis_alignaddr(py, off);
				dy_hi = vis_ld_d64_nf(dpy);
				dy_lo = vis_ld_d64_nf(dpy + 1);
				dy = vis_faligndata(dy_hi, dy_lo);
				fx = ((mlib_f32 *)dpx)[i];
				VECTOR_MULM_S16;
				py += nstride;
			}

			VECTOR_SUM_S32;
/* store 1st 2 x mlib_s32 of result */
			vis_pst_32(ds, dpz, emask);
			dpz++;

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				emask = vis_edge32(dpz, pzend);
/* store 2nd 2 x mlib_s32 of result */
				vis_pst_32(ds1, dpz, emask);
			}

			dpz++;
			emask = vis_edge32(dpz, pzend);
			y = (void *)((mlib_s16 *)y + 4);
		}
	} else {
		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s16 *py = (mlib_s16 *)y;

			ds = ds1 = dzero;

			for (i = 0; i < m; i++) {
				dy = ((mlib_d64 *)py)[0];
				fx = ((mlib_f32 *)dpx)[i];
				VECTOR_MULM_S16;
				VECTOR_SUM_S32;
				py += nstride;
			}

/* store 1st 8 bytes of result */
			dpz[0] = ds;
/* store 2nd 8 bytes of result */
			dpz[1] = ds1;
			y = (void *)((mlib_s16 *)y + 4);
			dpz += 2;
		}
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	VECTOR_MULM_U8C                                            \
	dr = vis_fmuld8ulx16(vis_read_hi(dy), vis_read_hi(dx));    \
	dr1 = vis_fmuld8ulx16(vis_read_lo(dy), vis_read_hi(dx));   \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dy), vis_read_lo(dx));   \
	dr3 = vis_fmuld8ulx16(vis_read_lo(dy), vis_read_lo(dx));   \
	dy = vis_faligndata(dy, dy);                               \
	dr4 = vis_fmuld8ulx16(vis_read_hi(dy), vis_read_hi(dx));   \
	dr5 = vis_fmuld8ulx16(vis_read_lo(dy), vis_read_hi(dx));   \
	dr6 = vis_fmuld8ulx16(vis_read_hi(dy), vis_read_lo(dx));   \
	dr7 = vis_fmuld8ulx16(vis_read_lo(dy), vis_read_lo(dx))

/* *********************************************************** */

#define	VECTOR_SUM_S32C                                         \
	ds = vis_fpsub32(ds, dr2);                              \
	ds1 = vis_fpsub32(ds1, dr3);                            \
	ds2 = vis_fpadd32(ds2, dr);                             \
	ds3 = vis_fpadd32(ds3, dr1);                            \
	ds = vis_fpadd32(ds, dr4);                              \
	ds1 = vis_fpadd32(ds1, dr5);                            \
	ds2 = vis_fpadd32(ds2, dr6);                            \
	ds3 = vis_fpadd32(ds3, dr7)

/* *********************************************************** */

#define	PACK_S32C_U8C                                                \
	ds = vis_freg_pair(vis_fpackfix(ds), vis_fpackfix(ds1));     \
	ds2 = vis_freg_pair(vis_fpackfix(ds2), vis_fpackfix(ds3));   \
	vis_write_gsr((7 << 3) | 7);                                 \
	ds = vis_fpack16_to_hi(ds, ds);                              \
	ds = vis_fpack16_to_lo(ds, ds2);                             \
	ds = vis_fpmerge(vis_read_hi(ds), vis_read_lo(ds));          \
	vis_write_gsr((15 << 3) | 7)

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_U8C_U8C_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 size, i;
	mlib_s8 *pzend = (mlib_s8 *)z + n + n - 1;
	mlib_d64 array[MAX_SIZE], *dpx, *dpy, *dpz;
	mlib_d64 dx, dy_hi, dy_lo, dy, ds, ds1, ds2, ds3;
	mlib_d64 dr, dr1, dr2, dr3, dr4, dr5, dr6, dr7;

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = m;

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_U8C, type_U8C, mode_Sat,
				x, y, 1, m, n, nstride, z);
	}

	for (i = 0; i < (2 * m); i++) {
		((mlib_s16 *)dpx)[2 * i] = ((mlib_s16 *)dpx)[2 * i + 1] =
			((mlib_u8 *)x)[i] * 2;
	}

/* initialize GSR */
	vis_write_gsr((15 << 3) | 7);
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));

	if ((((n + n) | (nstride + nstride) |
		(mlib_addr)y | (mlib_addr)z) & 7) == 0) {
		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s8 *py = (mlib_s8 *)y;

			ds = ds1 = ds2 = ds3 = 0;
			for (i = 0; i < m; i++) {
				dy = ((mlib_d64 *)py)[0];
				dx = dpx[i];
				VECTOR_MULM_U8C;
				VECTOR_SUM_S32C;
				py += 2 * nstride;
			}

			PACK_S32C_U8C;
/* store 8 bytes of result */
			dpz[0] = ds;
			y = (void *)((mlib_s8 *)y + 8);
			dpz++;
		}
	} else {
		mlib_s32 off =
			(mlib_addr)dpz - (mlib_addr)z + (1 & (mlib_addr)z),
/*
 * generate edge mask for the start point
 */
			emask = vis_edge8(z, pzend);

		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s8 *py = (mlib_s8 *)y;

			ds = ds1 = ds2 = ds3 = 0;

			for (i = 0; i < m; i++) {
				dpy = (mlib_d64 *)vis_alignaddr(py, off);
				dy_hi = vis_ld_d64_nf(dpy);
				dy_lo = vis_ld_d64_nf(dpy + 1);
				dy = vis_faligndata(dy_hi, dy_lo);
				dx = dpx[i];
				(void) vis_alignaddr(dpy, 7);
				VECTOR_MULM_U8C;
				VECTOR_SUM_S32C;
				py += 2 * nstride;
			}

			PACK_S32C_U8C;

			if (1 & (mlib_addr)z) {
/* ds >>= 8 */
				ds = vis_faligndata(ds, ds);
/* store 7 bytes of result */
				vis_pst_8(ds, dpz, emask & 0x7f);

				if ((mlib_addr)++dpz <= (mlib_addr)pzend)
					vis_pst_8(ds, dpz, 0x80);
			} else
/* store 8 bytes of result */
				vis_pst_8(ds, dpz++, emask);
			y = (void *)((mlib_s8 *)y + 8);
			emask = vis_edge8(dpz, pzend);
		}
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	return mlib_VectorMulM_S_S8C_S8C_Mod((mlib_s8 *)z, (mlib_s8 *)x,
		(mlib_s8 *)y, m, n, nstride);
}

/* *********************************************************** */

#define	PACK_S32C_U8C_SAT16                                         \
	ds4 = vis_freg_pair(vis_read_hi(ds), vis_read_hi(ds2));     \
	ds = vis_freg_pair(vis_read_lo(ds), vis_read_lo(ds2));      \
	ds = vis_freg_pair(vis_fpackfix(ds4), vis_fpackfix(ds));    \
	ds2 = vis_freg_pair(vis_read_hi(ds1), vis_read_hi(ds3));    \
	ds1 = vis_freg_pair(vis_read_lo(ds1), vis_read_lo(ds3));    \
	ds1 = vis_freg_pair(vis_fpackfix(ds2), vis_fpackfix(ds1))

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_S16C_U8C_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 size, i;
	mlib_s16 *pzend = (mlib_s16 *)z + n + n - 1;
	mlib_d64 array[MAX_SIZE], *dpx, *dpy, *dpz;
	mlib_d64 dx, dy_hi, dy_lo, dy, ds, ds1, ds2, ds3, ds4;
	mlib_d64 dr, dr1, dr2, dr3, dr4, dr5, dr6, dr7;

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = m;

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_U8C, type_S16C,
				mode_Sat, x, y, 1, m, n, nstride, z);
	}

	for (i = 0; i < (2 * m); i++) {
		((mlib_s16 *)dpx)[2 * i] = ((mlib_s16 *)dpx)[2 * i + 1] =
			((mlib_u8 *)x)[i] * 2;
	}

/* initialize GSR */
	vis_write_gsr((15 << 3) | 7);
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));

	if ((((n + n) | (nstride + nstride) |
		(mlib_addr)y | (mlib_addr)z) & 7) == 0) {
		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s8 *py = (mlib_s8 *)y;

			ds = ds1 = ds2 = ds3 = 0;
			for (i = 0; i < m; i++) {
				dy = ((mlib_d64 *)py)[0];
				dx = dpx[i];
				VECTOR_MULM_U8C;
				VECTOR_SUM_S32C;
				py += 2 * nstride;
			}

			PACK_S32C_U8C_SAT16;
/* store 16 bytes of result */
			dpz[0] = ds;
			dpz[1] = ds1;
			y = (void *)((mlib_s8 *)y + 8);
			dpz += 2;
		}
	} else {
		mlib_s32 off =
			((2 & (mlib_addr)z) + (mlib_u8 *)dpz -
			(mlib_u8 *)z) >> 1,
/*
 * generate edge mask for the start point
 */
			emask = vis_edge16(z, pzend);

		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s8 *py = (mlib_s8 *)y;

			ds = ds1 = ds2 = ds3 = 0;

			for (i = 0; i < m; i++) {
				dpy = (mlib_d64 *)vis_alignaddr(py, off);
				dy_hi = vis_ld_d64_nf(dpy);
				dy_lo = vis_ld_d64_nf(dpy + 1);
				dy = vis_faligndata(dy_hi, dy_lo);
				dx = dpx[i];
				(void) vis_alignaddr(dpy, 7);
				VECTOR_MULM_U8C;
				VECTOR_SUM_S32C;
				py += 2 * nstride;
			}

			PACK_S32C_U8C_SAT16;

			if (2 & (mlib_addr)z) {
				(void) vis_alignaddr(dpy, 6);
/* ds >>= 16 */
				ds = vis_faligndata(ds, ds);
/* store 3 * mlib_s16 of result */
				vis_pst_16(ds, dpz, emask & 0x7);

				if ((mlib_addr)++dpz <= (mlib_addr)pzend)
					vis_pst_16(ds, dpz, 0x8);

				if ((2 + (mlib_addr)dpz) <= (mlib_addr)pzend) {
					emask = vis_edge16(dpz, pzend);
/* ds1 >>= 16 */
					ds1 = vis_faligndata(ds1, ds1);
/* store 3 * mlib_s16 of result */
					vis_pst_16(ds1, dpz, emask & 0x7);

					if ((mlib_addr)++dpz <=
						(mlib_addr)pzend)
						vis_pst_16(ds1, dpz, 0x8);
				}
			} else {
/* store 4 * mlib_s16 of result */
				vis_pst_16(ds, dpz++, emask);

				if ((mlib_addr)dpz <= (mlib_addr)pzend) {
					emask = vis_edge16(dpz, pzend);
/* store 4 * mlib_s16 of result */
					vis_pst_16(ds1, dpz++, emask);
				}
			}

			y = (void *)((mlib_s8 *)y + 8);
			emask = vis_edge16(dpz, pzend);
		}
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	PACK_S32C_U8C_MOD16                                     \
	ds2 = vis_fand(ds2, mask_lo);                           \
	ds = vis_fpack32(ds, dzero);                            \
	ds = vis_fpack32(ds, dzero);                            \
	ds = vis_for(ds, ds2);                                  \
	ds3 = vis_fand(ds3, mask_lo);                           \
	ds1 = vis_fpack32(ds1, dzero);                          \
	ds1 = vis_fpack32(ds1, dzero);                          \
	ds1 = vis_for(ds1, ds3)

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_S16C_U8C_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 size, i;
	mlib_s16 *pzend = (mlib_s16 *)z + n + n - 1;
	mlib_d64 array[MAX_SIZE], *dpx, *dpy, *dpz;
	mlib_d64 dx, dy_hi, dy_lo, dy, ds, ds1, ds2, ds3;
	mlib_d64 dr, dr1, dr2, dr3, dr4, dr5, dr6, dr7;
	mlib_d64 dzero = vis_fzero();
	mlib_d64 mask_lo = vis_to_double_dup(0x0000FFFF);

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = m;

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_U8C, type_S16C,
				mode_Mod, x, y, 1, m, n, nstride, z);
	}

	for (i = 0; i < (2 * m); i++) {
		((mlib_s16 *)dpx)[2 * i] = ((mlib_s16 *)dpx)[2 * i + 1] =
			((mlib_u8 *)x)[i];
	}

/* initialize GSR */
	vis_write_gsr((15 << 3) | 7);
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));

	if ((((nstride + nstride) | (n + n) |
		(mlib_addr)y | (mlib_addr)z) & 7) == 0) {
		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s8 *py = (mlib_s8 *)y;

			ds = ds1 = ds2 = ds3 = 0;
			for (i = 0; i < m; i++) {
				dy = ((mlib_d64 *)py)[0];
				dx = dpx[i];
				VECTOR_MULM_U8C;
				VECTOR_SUM_S32C;
				py += 2 * nstride;
			}

			PACK_S32C_U8C_MOD16;
/* store 16 bytes of result */
			dpz[0] = ds;
			dpz[1] = ds1;
			y = (void *)((mlib_s8 *)y + 8);
			dpz += 2;
		}
	} else {
		mlib_s32 off =
			((2 & (mlib_addr)z) + (mlib_u8 *)dpz -
			(mlib_u8 *)z) >> 1,
/*
 * generate edge mask for the start point
 */
			emask = vis_edge16(z, pzend);

		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s8 *py = (mlib_s8 *)y;

			ds = ds1 = ds2 = ds3 = 0;

			for (i = 0; i < m; i++) {
				dpy = (mlib_d64 *)vis_alignaddr(py, off);
				dy_hi = vis_ld_d64_nf(dpy);
				dy_lo = vis_ld_d64_nf(dpy + 1);
				dy = vis_faligndata(dy_hi, dy_lo);
				dx = dpx[i];
				(void) vis_alignaddr(dpy, 7);
				VECTOR_MULM_U8C;
				VECTOR_SUM_S32C;
				py += 2 * nstride;
			}

			PACK_S32C_U8C_MOD16;

			if (2 & (mlib_addr)z) {
				(void) vis_alignaddr(dpy, 6);
/* ds >>= 16 */
				ds = vis_faligndata(ds, ds);
/* store 3 * mlib_s16 of result */
				vis_pst_16(ds, dpz, emask & 0x7);

				if ((mlib_addr)++dpz <= (mlib_addr)pzend)
					vis_pst_16(ds, dpz, 0x8);

				if ((2 + (mlib_addr)dpz) <= (mlib_addr)pzend) {
					emask = vis_edge16(dpz, pzend);
/* ds1 >>= 16 */
					ds1 = vis_faligndata(ds1, ds1);
/* store 3 * mlib_s16 of result */
					vis_pst_16(ds1, dpz, emask & 0x7);

					if ((mlib_addr)++dpz <=
						(mlib_addr)pzend)
						vis_pst_16(ds1, dpz, 0x8);
				}
			} else {
/* store 4 * mlib_s16 of result */
				vis_pst_16(ds, dpz++, emask);

				if ((mlib_addr)dpz <= (mlib_addr)pzend) {
					emask = vis_edge16(dpz, pzend);
/* store 4 * mlib_s16 of result */
					vis_pst_16(ds1, dpz++, emask);
				}
			}

			y = (void *)((mlib_s8 *)y + 8);
			emask = vis_edge16(dpz, pzend);
		}
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	VECTOR_MULM_S8C                                            \
	dr4 = vis_fmuld8sux16(vis_read_hi(dy), vis_read_hi(dx));   \
	dr5 = vis_fmuld8sux16(vis_read_lo(dy), vis_read_hi(dx));   \
	dr6 = vis_fmuld8sux16(vis_read_hi(dy), vis_read_lo(dx));   \
	dr7 = vis_fmuld8sux16(vis_read_lo(dy), vis_read_lo(dx));   \
	dy = vis_fpack32(dy, dy);                                  \
	dr = vis_fmuld8sux16(vis_read_hi(dy), vis_read_hi(dx));    \
	dr1 = vis_fmuld8sux16(vis_read_lo(dy), vis_read_hi(dx));   \
	dr2 = vis_fmuld8sux16(vis_read_hi(dy), vis_read_lo(dx));   \
	dr3 = vis_fmuld8sux16(vis_read_lo(dy), vis_read_lo(dx))

/* *********************************************************** */

#define	M_MLIB_S8_MAXC	127

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 size, i;
	mlib_s8 *pzend = (mlib_s8 *)z + n + n - 1;
	mlib_d64 array[MAX_SIZE + 8], *dpx, *dpy, *dpz;
	mlib_d64 dx, dy_hi, dy_lo, dy, ds, ds1, ds2, ds3;
	mlib_d64 dr, dr1, dr2, dr3, dr4, dr5, dr6, dr7;

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = m;

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_S8C, type_S8C, mode_Sat,
				x, y, 1, m, n, nstride, z);
	}

	if (m > M_MLIB_S8_MAXC) {
		mlib_s32 m_end, j, *psum = (mlib_s32 *)(array + MAX_SIZE);

		dpz = (mlib_d64 *)z;

		for (i = 0; i < (2 * m); i++) {
			((mlib_s16 *)dpx)[2 * i] =
				((mlib_s16 *)dpx)[2 * i + 1] =
				((mlib_s8 *)x)[i];
		}

		if ((((n + n) | (nstride + nstride) |
			(mlib_addr)y | (mlib_addr)z) & 7) == 0) {
			while ((mlib_addr)dpz <= (mlib_addr)pzend) {
				mlib_s8 *py = (mlib_s8 *)y;

				for (j = 0; j < 4; j++)
					((mlib_d64 *)psum)[j] = 0;
				i = 0;
				do {
					m_end = (m < (i +
						M_MLIB_S8_MAXC * 2)) ? m : i +
						M_MLIB_S8_MAXC * 2;
					ds = ds1 = ds2 = ds3 = 0;
					for (; i < m_end; i++) {
						dy = ((mlib_d64 *)py)[0];
						dx = dpx[i];
						VECTOR_MULM_S8C;
						VECTOR_SUM_S32C;
						py += 2 * nstride;
					}

					((mlib_d64 *)psum)[4] = ds;
					((mlib_d64 *)psum)[5] = ds1;
					((mlib_d64 *)psum)[6] = ds2;
					((mlib_d64 *)psum)[7] = ds3;
					for (j = 0; j < 8; j++)
						psum[j] += psum[j + 8] >> 8;
/* because of the compiler error */
					i = m_end;
				}

				while (m_end < m);
				for (j = 0; j < 4; j++) {
					mlib_s32 s_r = psum[j],
						s_i = psum[j + 4];

					if (s_r > MLIB_S8_MAX)
						s_r = MLIB_S8_MAX;
					else if (s_r < MLIB_S8_MIN)
						s_r = MLIB_S8_MIN;

					if (s_i > MLIB_S8_MAX)
						s_i = MLIB_S8_MAX;
					else if (s_i < MLIB_S8_MIN)
						s_i = MLIB_S8_MIN;
					((mlib_s8 *)dpz)[2 * j] = s_r;
					((mlib_s8 *)dpz)[2 * j + 1] = s_i;
				}

				y = (void *)((mlib_s8 *)y + 8);
				dpz++;
			}
		} else {
			while ((mlib_addr)dpz <= (mlib_addr)pzend) {
				mlib_s8 *py = (mlib_s8 *)y;

				for (j = 0; j < 4; j++)
					((mlib_d64 *)psum)[j] = 0;
				i = 0;
				do {
					m_end = (m < (i +
						M_MLIB_S8_MAXC * 2)) ? m : i +
						M_MLIB_S8_MAXC * 2;
					ds = ds1 = ds2 = ds3 = 0;

					for (; i < m_end; i++) {
						dpy = (mlib_d64 *)
							vis_alignaddr(py, 0);
						dy_hi = vis_ld_d64_nf(dpy);
						dy_lo = vis_ld_d64_nf(dpy + 1);
						dy = vis_faligndata(dy_hi,
							dy_lo);
						dx = dpx[i];
						VECTOR_MULM_S8C;
						VECTOR_SUM_S32C;
						py += 2 * nstride;
					}

					((mlib_d64 *)psum)[4] = ds;
					((mlib_d64 *)psum)[5] = ds1;
					((mlib_d64 *)psum)[6] = ds2;
					((mlib_d64 *)psum)[7] = ds3;
					for (j = 0; j < 8; j++)
						psum[j] += psum[j + 8] >> 8;
/* because of the compiler error */
					i = m_end;
				}

				while (m_end < m);
				for (j = 0; j < 4 && ((mlib_addr)dpz + 2 * j) <=
					(mlib_addr)pzend; j++) {
					mlib_s32 s_r = psum[j],
						s_i = psum[j + 4];

					if (s_r > MLIB_S8_MAX)
						s_r = MLIB_S8_MAX;
					else if (s_r < MLIB_S8_MIN)
						s_r = MLIB_S8_MIN;

					if (s_i > MLIB_S8_MAX)
						s_i = MLIB_S8_MAX;
					else if (s_i < MLIB_S8_MIN)
						s_i = MLIB_S8_MIN;
					((mlib_s8 *)dpz)[2 * j] = s_r;
					((mlib_s8 *)dpz)[2 * j + 1] = s_i;
				}

				y = (void *)((mlib_s8 *)y + 8);
				dpz++;
			}
		}
	} else {
		mlib_d64 displacement = vis_to_double_dup(0x10000);
		mlib_d64 restore = vis_to_double_dup(0x80808080);

/* initialize GSR scale factor */
		vis_write_gsr(7 << 3);
		dpz = (mlib_d64 *)((mlib_addr)z & (~7));

		for (i = 0; i < (2 * m); i++) {
			((mlib_s16 *)dpx)[2 * i] =
				((mlib_s16 *)dpx)[2 * i + 1] =
				((mlib_s8 *)x)[i] * 2;
		}

		if ((((nstride + nstride) | (n + n) |
			(mlib_addr)y | (mlib_addr)z) & 7) == 0) {
			while ((mlib_addr)dpz <= (mlib_addr)pzend) {
				mlib_s8 *py = (mlib_s8 *)y;

				ds = ds1 = ds2 = ds3 = displacement;
				for (i = 0; i < m; i++) {
					dy = ((mlib_d64 *)py)[0];
					dx = dpx[i];
					VECTOR_MULM_S8C;
					VECTOR_SUM_S32C;
					py += 2 * nstride;
				}

				PACK_S32_S8_SAT;
/* store 8 bytes of result */
				dpz[0] = ds;
				y = (void *)((mlib_s8 *)y + 8);
				dpz++;
			}
		} else {
			mlib_s32 off = (mlib_addr)dpz - (mlib_addr)z +
				(1 & (mlib_addr)z),
/*
 * generate edge mask for the start point
 */
				emask = vis_edge8(z, pzend);

			while ((mlib_addr)dpz <= (mlib_addr)pzend) {
				mlib_s8 *py = (mlib_s8 *)y;

				ds = ds1 = ds2 = ds3 = displacement;

				for (i = 0; i < m; i++) {
					dpy = (mlib_d64 *)vis_alignaddr(py,
						off);
					dy_hi = vis_ld_d64_nf(dpy);
					dy_lo = vis_ld_d64_nf(dpy + 1);
					dy = vis_faligndata(dy_hi, dy_lo);
					dx = dpx[i];
					VECTOR_MULM_S8C;
					VECTOR_SUM_S32C;
					py += 2 * nstride;
				}

				PACK_S32_S8_SAT;

				if (1 & (mlib_addr)z) {
					(void) vis_alignaddr(dpz, 7);
/* ds >>= 8 */
					ds = vis_faligndata(ds, ds);
/* store 7 bytes of result */
					vis_pst_8(ds, dpz, emask & 0x7f);

					if ((mlib_addr)++dpz <=
						(mlib_addr)pzend)
						vis_pst_8(ds, dpz, 0x80);
				} else
/* store 8 bytes of result */
					vis_pst_8(ds, dpz++, emask);
				y = (void *)((mlib_s8 *)y + 8);
				emask = vis_edge8(dpz, pzend);
			}
		}
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	M_MLIB_S8_MAXC_SAT16	127

mlib_status
mlib_VectorMulM_S_S16C_S8C_Sat(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 size, i;
	mlib_s16 *pzend = (mlib_s16 *)z + n + n - 1;
	mlib_d64 array[MAX_SIZE + 8], *dpx, *dpy, *dpz;
	mlib_d64 dx, dy_hi, dy_lo, dy, ds, ds1, ds2, ds3, ds4;
	mlib_d64 dr, dr1, dr2, dr3, dr4, dr5, dr6, dr7;

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = m;

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_S8C, type_S16C,
				mode_Sat, x, y, 1, m, n, nstride, z);
	}

	if (m > M_MLIB_S8_MAXC_SAT16) {
		mlib_s32 m_end, j, *psum = (mlib_s32 *)(array + MAX_SIZE);

		dpz = (mlib_d64 *)z;

		for (i = 0; i < (2 * m); i++) {
			((mlib_s16 *)dpx)[2 * i] =
				((mlib_s16 *)dpx)[2 * i + 1] =
				((mlib_s8 *)x)[i];
		}

		if ((((nstride + nstride) | (n + n) |
			(mlib_addr)y | (mlib_addr)z) & 7) == 0) {
			while ((mlib_addr)dpz <= (mlib_addr)pzend) {
				mlib_s8 *py = (mlib_s8 *)y;

				for (j = 0; j < 4; j++)
					((mlib_d64 *)psum)[j] = 0;
				i = 0;
				do {
					m_end = (m < (i +
						M_MLIB_S8_MAXC_SAT16 *
						2)) ? m : i +
						M_MLIB_S8_MAXC_SAT16 * 2;
					ds = ds1 = ds2 = ds3 = 0;
					for (; i < m_end; i++) {
						dy = ((mlib_d64 *)py)[0];
						dx = dpx[i];
						VECTOR_MULM_S8C;
						VECTOR_SUM_S32C;
						py += 2 * nstride;
					}

					((mlib_d64 *)psum)[4] = ds;
					((mlib_d64 *)psum)[5] = ds1;
					((mlib_d64 *)psum)[6] = ds2;
					((mlib_d64 *)psum)[7] = ds3;
					for (j = 0; j < 8; j++)
						psum[j] += psum[j + 8] >> 8;
/* because of the compiler error */
					i = m_end;
				}

				while (m_end < m);
				for (j = 0; j < 4; j++) {
					mlib_s32 s_r = psum[j],
						s_i = psum[j + 4];

					if (s_r > MLIB_S16_MAX)
						s_r = MLIB_S16_MAX;
					else if (s_r < MLIB_S16_MIN)
						s_r = MLIB_S16_MIN;

					if (s_i > MLIB_S16_MAX)
						s_i = MLIB_S16_MAX;
					else if (s_i < MLIB_S16_MIN)
						s_i = MLIB_S16_MIN;
					((mlib_s16 *)dpz)[2 * j] = s_r;
					((mlib_s16 *)dpz)[2 * j + 1] = s_i;
				}

				y = (void *)((mlib_s8 *)y + 8);
				dpz += 2;
			}
		} else {
			while ((mlib_addr)dpz <= (mlib_addr)pzend) {
				mlib_s8 *py = (mlib_s8 *)y;

				for (j = 0; j < 4; j++)
					((mlib_d64 *)psum)[j] = 0;
				i = 0;
				do {
					m_end = (m < (i +
						M_MLIB_S8_MAXC_SAT16 *
						2)) ? m : i +
						M_MLIB_S8_MAXC_SAT16 * 2;
					ds = ds1 = ds2 = ds3 = 0;

					for (; i < m_end; i++) {
						dpy = (mlib_d64 *)
							vis_alignaddr(py, 0);
						dy_hi = vis_ld_d64_nf(dpy);
						dy_lo = vis_ld_d64_nf(dpy + 1);
						dy = vis_faligndata(dy_hi,
							dy_lo);
						dx = dpx[i];
						VECTOR_MULM_S8C;
						VECTOR_SUM_S32C;
						py += 2 * nstride;
					}

					((mlib_d64 *)psum)[4] = ds;
					((mlib_d64 *)psum)[5] = ds1;
					((mlib_d64 *)psum)[6] = ds2;
					((mlib_d64 *)psum)[7] = ds3;
					for (j = 0; j < 8; j++)
						psum[j] += psum[j + 8] >> 8;
/* because of the compiler error */
					i = m_end;
				}

				while (m_end < m);
				for (j = 0; j < 4 &&
					((mlib_addr)dpz + 2 * 2 * j) <=
					(mlib_addr)pzend; j++) {
					mlib_s32 s_r = psum[j],
						s_i = psum[j + 4];

					if (s_r > MLIB_S16_MAX)
						s_r = MLIB_S16_MAX;
					else if (s_r < MLIB_S16_MIN)
						s_r = MLIB_S16_MIN;

					if (s_i > MLIB_S16_MAX)
						s_i = MLIB_S16_MAX;
					else if (s_i < MLIB_S16_MIN)
						s_i = MLIB_S16_MIN;
					((mlib_s16 *)dpz)[2 * j] = s_r;
					((mlib_s16 *)dpz)[2 * j + 1] = s_i;
				}

				y = (void *)((mlib_s8 *)y + 8);
				dpz += 2;
			}
		}
	} else {
/* initialize GSR scale factor */
		vis_write_gsr(7 << 3);
		dpz = (mlib_d64 *)((mlib_addr)z & (~7));

		for (i = 0; i < (2 * m); i++) {
			((mlib_s16 *)dpx)[2 * i] =
				((mlib_s16 *)dpx)[2 * i + 1] =
				((mlib_s8 *)x)[i] * 2;
		}

		if ((((nstride + nstride) | (n + n) |
			(mlib_addr)y | (mlib_addr)z) & 7) == 0) {
			while ((mlib_addr)dpz <= (mlib_addr)pzend) {
				mlib_s8 *py = (mlib_s8 *)y;

/* displacement; */
				ds = ds1 = ds2 = ds3 = 0;
				for (i = 0; i < m; i++) {
					dy = ((mlib_d64 *)py)[0];
					dx = dpx[i];
					VECTOR_MULM_S8C;
					VECTOR_SUM_S32C;
					py += 2 * nstride;
				}

				PACK_S32C_U8C_SAT16;
/* store 16 bytes of result */
				dpz[0] = ds;
				dpz[1] = ds1;
				y = (void *)((mlib_s8 *)y + 8);
				dpz += 2;
			}
		} else {
			mlib_s32 off =
				((2 & (mlib_addr)z) + (mlib_u8 *)dpz -
				(mlib_u8 *)z) >> 1,
/*
 * generate edge mask for the start point
 */
				emask = vis_edge16(z, pzend);

			while ((mlib_addr)dpz <= (mlib_addr)pzend) {
				mlib_s8 *py = (mlib_s8 *)y;

				ds = ds1 = ds2 = ds3 = 0;

				for (i = 0; i < m; i++) {
					dpy = (mlib_d64 *)vis_alignaddr(py,
						off);
					dy_hi = vis_ld_d64_nf(dpy);
					dy_lo = vis_ld_d64_nf(dpy + 1);
					dy = vis_faligndata(dy_hi, dy_lo);
					dx = dpx[i];
					VECTOR_MULM_S8C;
					VECTOR_SUM_S32C;
					py += 2 * nstride;
				}

				PACK_S32C_U8C_SAT16;

				if (2 & (mlib_addr)z) {
					(void) vis_alignaddr(dpy, 6);
/* ds >>= 16 */
					ds = vis_faligndata(ds, ds);
/* store 3 * mlib_s16 of result */
					vis_pst_16(ds, dpz, emask & 0x7);

					if ((mlib_addr)++dpz <=
						(mlib_addr)pzend)
						vis_pst_16(ds, dpz, 0x8);

					if ((2 + (mlib_addr)dpz) <=
						(mlib_addr)pzend) {
						emask = vis_edge16(dpz, pzend);
/* ds1 >>= 16 */
						ds1 = vis_faligndata(ds1, ds1);
/* store 3 * mlib_s16 of result */
						vis_pst_16(ds1, dpz,
							emask & 0x7);

						if ((mlib_addr)++dpz <=
							(mlib_addr)pzend)
							vis_pst_16(ds1, dpz,
								0x8);
					}
				} else {
/* store 4 * mlib_s16 of result */
					vis_pst_16(ds, dpz++, emask);

					if ((mlib_addr)dpz <=
					    (mlib_addr)pzend) {
						emask = vis_edge16(dpz, pzend);
/* store 4 * mlib_s16 of result */
						vis_pst_16(ds1, dpz++, emask);
					}
				}

				y = (void *)((mlib_s8 *)y + 8);
				emask = vis_edge16(dpz, pzend);
			}
		}
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

#undef VECTOR_MULM_S8C

/* *********************************************************** */

#define	VECTOR_MULM_S8C                                         \
	dr = vis_fmul8sux16(dy, dx_r);                          \
	dr1 = vis_fmul8sux16(dy, dx_i);                         \
	dy = vis_fpack32(dy, dy);                               \
	dr2 = vis_fmul8sux16(dy, dx_r);                         \
	dr3 = vis_fmul8sux16(dy, dx_i)

/* *********************************************************** */

#define	VECTOR_SUM_S16C                                         \
	ds = vis_fpadd16(ds, dr);                               \
	ds1 = vis_fpadd16(ds1, dr1);                            \
	ds = vis_fpsub16(ds, dr3);                              \
	ds1 = vis_fpadd16(ds1, dr2)

/* *********************************************************** */

#define	PACK_S16_S8C                                            \
	PACK_S16_S8_MOD;                                        \
	ds = vis_fpmerge(vis_read_hi(ds), vis_read_lo(ds))

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_S8C_S8C_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 size, i;
	mlib_s8 *pzend = (mlib_s8 *)z + n + n - 1;
	mlib_d64 array[MAX_SIZE], *dpx, *dpy, *dpz;
	mlib_d64 dx_r, dx_i, dy_hi, dy_lo, dy, ds, ds1, dr, dr1, dr2, dr3;
	mlib_d64 without_control = vis_to_double_dup(0xff00ff);

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = m + m;

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_S8C, type_S8C, mode_Mod,
				x, y, 1, m, n, nstride, z);
	}

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));

	for (i = 0; i < (2 * m); i++) {
		((mlib_s16 *)dpx)[4 * i] =
			((mlib_s16 *)dpx)[4 * i + 1] =
			((mlib_s16 *)dpx)[4 * i + 2] =
			((mlib_s16 *)dpx)[4 * i + 3] = ((mlib_s8 *)x)[i] << 8;
	}

	if ((((nstride + nstride) | (n + n) |
		(mlib_addr)y | (mlib_addr)z) & 7) == 0) {
		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s8 *py = (mlib_s8 *)y;

			ds = ds1 = 0;
			for (i = 0; i < m; i++) {
				dy = ((mlib_d64 *)py)[0];
				dx_r = dpx[2 * i];
				dx_i = dpx[2 * i + 1];
				VECTOR_MULM_S8C;
				VECTOR_SUM_S16C;
				py += 2 * nstride;
			}

			PACK_S16_S8C;
/* store 8 bytes of result */
			dpz[0] = ds;
			y = (void *)((mlib_s8 *)y + 8);
			dpz++;
		}
	} else {
		mlib_s32 off =
			(mlib_addr)dpz - (mlib_addr)z + (1 & (mlib_addr)z),
/*
 * generate edge mask for the start point
 */
			emask = vis_edge8(z, pzend);

		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s8 *py = (mlib_s8 *)y;

			ds = ds1 = 0;

			dpy = (mlib_d64 *)vis_alignaddr(py, off);
			dy_hi = vis_ld_d64_nf(dpy);
			dy_lo = vis_ld_d64_nf(dpy + 1);
			dy = vis_faligndata(dy_hi, dy_lo);
			dx_r = dpx[0];
			dx_i = dpx[1];
			VECTOR_MULM_S8C;
			py += 2 * nstride;
			for (i = 1; i < m; i++) {
				VECTOR_SUM_S16C;
				dpy = (mlib_d64 *)vis_alignaddr(py, off);
				dy_hi = vis_ld_d64_nf(dpy);
				dy_lo = vis_ld_d64_nf(dpy + 1);
				dy = vis_faligndata(dy_hi, dy_lo);
				dx_r = dpx[2 * i];
				dx_i = dpx[2 * i + 1];
				VECTOR_MULM_S8C;
				py += 2 * nstride;
			}

			VECTOR_SUM_S16C;
			PACK_S16_S8C;

			if (1 & (mlib_addr)z) {
				(void) vis_alignaddr(dpz, 7);
/* ds >>= 8 */
				ds = vis_faligndata(ds, ds);
/* store 7 bytes of result */
				vis_pst_8(ds, dpz, emask & 0x7f);

				if ((mlib_addr)++dpz <= (mlib_addr)pzend)
					vis_pst_8(ds, dpz, 0x80);
			} else
/* store 8 bytes of result */
				vis_pst_8(ds, dpz++, emask);
			y = (void *)((mlib_s8 *)y + 8);
			emask = vis_edge8(dpz, pzend);
		}
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	PACK_S16_S8C_MOD16                                      \
	ds2 = vis_fmuld8sux16(fone_h, vis_read_hi(ds));         \
	ds2 = vis_fpack32(ds2, dzero);                          \
	ds3 = vis_fmuld8sux16(fone_h, vis_read_lo(ds));         \
	ds3 = vis_fpack32(ds3, dzero);                          \
	ds = vis_fmuld8ulx16(fone_l, vis_read_hi(ds1));         \
	ds = vis_fand(ds, mask_lo);                             \
	ds = vis_for(ds, ds2);                                  \
	ds1 = vis_fmuld8ulx16(fone_l, vis_read_lo(ds1));        \
	ds1 = vis_fand(ds1, mask_lo);                           \
	ds1 = vis_for(ds1, ds3)

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_S16C_S8C_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 size, i;
	mlib_s16 *pzend = (mlib_s16 *)z + n + n - 1;
	mlib_d64 array[MAX_SIZE], *dpx, *dpy, *dpz;
	mlib_d64 dx_r, dx_i, dy_hi, dy_lo, dy, ds, ds1, ds2, ds3, dr, dr1, dr2,
		dr3;
	mlib_d64 dzero = vis_fzero();
	mlib_d64 mask_lo = vis_to_double_dup(0x0000FFFF);
	mlib_f32 fone_h = vis_to_float(0x01000100);
	mlib_f32 fone_l = vis_to_float(0x00010001);

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = m + m;

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_S8C, type_S16C,
				mode_Mod, x, y, 1, m, n, nstride, z);
	}

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));

	for (i = 0; i < (2 * m); i++) {
		((mlib_s16 *)dpx)[4 * i] =
			((mlib_s16 *)dpx)[4 * i + 1] =
			((mlib_s16 *)dpx)[4 * i + 2] =
			((mlib_s16 *)dpx)[4 * i + 3] = ((mlib_s8 *)x)[i] << 8;
	}

	if ((((nstride + nstride) | (n + n) |
		(mlib_addr)y | (mlib_addr)z) & 7) == 0) {
		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s8 *py = (mlib_s8 *)y;

			ds = ds1 = 0;
			for (i = 0; i < m; i++) {
				dy = ((mlib_d64 *)py)[0];
				dx_r = dpx[2 * i];
				dx_i = dpx[2 * i + 1];
				VECTOR_MULM_S8C;
				VECTOR_SUM_S16C;
				py += 2 * nstride;
			}

			PACK_S16_S8C_MOD16;
/* store 16 bytes of result */
			dpz[0] = ds;
			dpz[1] = ds1;
			y = (void *)((mlib_s8 *)y + 8);
			dpz += 2;
		}
	} else {
		mlib_s32 off = ((2 & (mlib_addr)z) + (mlib_u8 *)dpz -
			(mlib_u8 *)z) >> 1,
/*
 * generate edge mask for the start point
 */
			emask = vis_edge16(z, pzend);

		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s8 *py = (mlib_s8 *)y;

			ds = ds1 = 0;

			dpy = (mlib_d64 *)vis_alignaddr(py, off);
			dy_hi = vis_ld_d64_nf(dpy);
			dy_lo = vis_ld_d64_nf(dpy + 1);
			dy = vis_faligndata(dy_hi, dy_lo);
			dx_r = dpx[0];
			dx_i = dpx[1];
			VECTOR_MULM_S8C;
			py += 2 * nstride;
			for (i = 1; i < m; i++) {
				VECTOR_SUM_S16C;
				dpy = (mlib_d64 *)vis_alignaddr(py, off);
				dy_hi = vis_ld_d64_nf(dpy);
				dy_lo = vis_ld_d64_nf(dpy + 1);
				dy = vis_faligndata(dy_hi, dy_lo);
				dx_r = dpx[2 * i];
				dx_i = dpx[2 * i + 1];
				VECTOR_MULM_S8C;
				py += 2 * nstride;
			}

			VECTOR_SUM_S16C;
			PACK_S16_S8C_MOD16;

			if (2 & (mlib_addr)z) {
				(void) vis_alignaddr(dpy, 6);
/* ds >>= 16 */
				ds = vis_faligndata(ds, ds);
/* store 3 * mlib_s16 of result */
				vis_pst_16(ds, dpz, emask & 0x7);

				if ((mlib_addr)++dpz <= (mlib_addr)pzend)
					vis_pst_16(ds, dpz, 0x8);

				if ((2 + (mlib_addr)dpz) <= (mlib_addr)pzend) {
					emask = vis_edge16(dpz, pzend);
/* ds1 >>= 16 */
					ds1 = vis_faligndata(ds1, ds1);
/* store 3 * mlib_s16 of result */
					vis_pst_16(ds1, dpz, emask & 0x7);

					if ((mlib_addr)++dpz <=
						(mlib_addr)pzend)
						vis_pst_16(ds1, dpz, 0x8);
				}
			} else {
/* store 4 * mlib_s16 of result */
				vis_pst_16(ds, dpz++, emask);

				if ((mlib_addr)dpz <= (mlib_addr)pzend) {
					emask = vis_edge16(dpz, pzend);
/* store 4 * mlib_s16 of result */
					vis_pst_16(ds1, dpz++, emask);
				}
			}

			y = (void *)((mlib_s8 *)y + 8);
			emask = vis_edge16(dpz, pzend);
		}
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

#undef VECTOR_SUM_S32C

/* *********************************************************** */

#define	VECTOR_MULM_S16C                                            \
	dr4 = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dy));    \
	dr5 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dy));    \
	dr6 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_hi(dy));    \
	dr7 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_hi(dy));    \
	dr8 = vis_fmuld8sux16(vis_read_hi(dx), vis_read_lo(dy));    \
	dr9 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_lo(dy));    \
	dr10 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dy));   \
	dr11 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dy))

/* *********************************************************** */

#define	VECTOR_SUM_S32C                                                   \
	dr.db = vis_fpadd32(dr4, dr5);                                    \
	dr.two_float.fl0 = vis_fpsub32s(                                  \
		vis_read_hi(dr.db), vis_read_lo(dr.db));                  \
	ds += (mlib_d64)dr.two_int.int0;                                  \
	dr2.db = vis_fpadd32(dr8, dr9);                                   \
	dr2.two_float.fl1 = vis_fpsub32s(                                 \
		vis_read_hi(dr2.db), vis_read_lo(dr2.db));                \
	ds2 += (mlib_d64)dr2.two_int.int1;                                \
	dr1.db = vis_fpadd32(dr6, dr7);                                   \
	ds1 += (mlib_d64)dr1.two_int.int0 + (mlib_d64)dr1.two_int.int1;   \
	dr3.db = vis_fpadd32(dr10, dr11);                                 \
	ds3 += (mlib_d64)dr3.two_int.int0 + (mlib_d64)dr3.two_int.int1

/* *********************************************************** */

#define	PACK_S32C_S16C	/* PACK_D64_S16; */ PACK_D64_S16_C

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 size, i;
	mlib_s16 *pzend = (mlib_s16 *)z + n + n - 1;
	mlib_d64 array[MAX_SIZE], *dpx, *dpy, *dpz;
	mlib_d64 dx, dy_hi, dy_lo, dy, ds, ds1, ds2, ds3;
	mlib_d64 dr4, dr5, dr6, dr7, dr8, dr9, dr10, dr11;
	type_union_mlib_d64 dr, dr1, dr2, dr3;

/* initialize GSR scale factor */
	vis_write_gsr(15 << 3);

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = m;

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_S16C, type_S16C,
				mode_Sat, x, y, 1, m, n, nstride, z);
	}

	for (i = 0; i < m; i++) {
		((mlib_s16 *)dpx)[4 * i] = ((mlib_s16 *)dpx)[4 * i + 3] =
			((mlib_s16 *)x)[2 * i];
		((mlib_s16 *)dpx)[4 * i + 1] = ((mlib_s16 *)dpx)[4 * i + 2] =
			((mlib_s16 *)x)[2 * i + 1];
	}

	dpz = (mlib_d64 *)((mlib_addr)z & (~7));

	if ((((nstride << 2) | (n << 2) |
		(mlib_addr)y | (mlib_addr)z) & 7) == 0) {
		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s16 *py = (mlib_s16 *)y;

			ds = ds1 = ds2 = ds3 = 0;

			for (i = 0; i < m; i++) {
				dy = ((mlib_d64 *)py)[0];
				dx = dpx[i];
				VECTOR_MULM_S16C;
				VECTOR_SUM_S32C;
				py += 2 * nstride;
			}

			PACK_D64_S16_C;
/* store 8 bytes of result */
			dpz[0] = ds;
			y = (void *)((mlib_s16 *)y + 4);
			dpz++;
		}
	} else {
		mlib_s32 off =
			(mlib_addr)dpz - (mlib_addr)z + (2 & (mlib_addr)z),
/*
 * generate edge mask for the start point
 */
			emask = vis_edge16(z, pzend);

		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s16 *py = (mlib_s16 *)y;

			ds = ds1 = ds2 = ds3 = 0;

			dpy = (mlib_d64 *)vis_alignaddr(py, off);
			dy_hi = vis_ld_d64_nf(dpy);
			dy_lo = vis_ld_d64_nf(dpy + 1);
			dy = vis_faligndata(dy_hi, dy_lo);
			dx = dpx[0];
			VECTOR_MULM_S16C;
			py += 2 * nstride;
			for (i = 1; i < m; i++) {
				VECTOR_SUM_S32C;
				dpy = (mlib_d64 *)vis_alignaddr(py, off);
				dy_hi = vis_ld_d64_nf(dpy);
				dy_lo = vis_ld_d64_nf(dpy + 1);
				dy = vis_faligndata(dy_hi, dy_lo);
				dx = dpx[i];
				VECTOR_MULM_S16C;
				py += 2 * nstride;
			}

			VECTOR_SUM_S32C;
			PACK_D64_S16;

			if (2 & (mlib_addr)z) {
				(void) vis_alignaddr(dpz, 6);
/* ds >>= 16 */
				ds = vis_faligndata(ds, ds);
/* store 6 bytes of result */
				vis_pst_16(ds, dpz, emask & 7);

				if ((mlib_addr)++dpz <= (mlib_addr)pzend)
					vis_pst_16(ds, dpz, 8);
			} else
/* store 8 bytes of result */
				vis_pst_16(ds, dpz++, emask);

			y = (void *)((mlib_s16 *)y + 4);
			emask = vis_edge16(dpz, pzend);
		}
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	PACK_D64_SAT32                                          \
	FLOAT2INT_CLAMP(dr.two_int.int0, ds);                   \
	FLOAT2INT_CLAMP(dr.two_int.int1, ds1);                  \
	FLOAT2INT_CLAMP(dr1.two_int.int0, ds2);                 \
	FLOAT2INT_CLAMP(dr1.two_int.int1, ds3)

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_S32C_S16C_Sat(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 size, i;
	mlib_s32 *pzend = (mlib_s32 *)z + n + n - 1;
	mlib_d64 array[MAX_SIZE], *dpx, *dpy, *dpz;
	mlib_d64 dx, dy_hi, dy_lo, dy, ds, ds1, ds2, ds3;
	mlib_d64 dr4, dr5, dr6, dr7, dr8, dr9, dr10, dr11;
	type_union_mlib_d64 dr, dr1, dr2, dr3;

/* initialize GSR scale factor */
	vis_write_gsr(15 << 3);

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = m;

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_S16C, type_S32C,
				mode_Sat, x, y, 1, m, n, nstride, z);
	}

	for (i = 0; i < m; i++) {
		((mlib_s16 *)dpx)[4 * i] = ((mlib_s16 *)dpx)[4 * i + 3] =
			((mlib_s16 *)x)[2 * i];
		((mlib_s16 *)dpx)[4 * i + 1] = ((mlib_s16 *)dpx)[4 * i + 2] =
			((mlib_s16 *)x)[2 * i + 1];
	}

	dpz = (mlib_d64 *)((mlib_addr)z & (~7));

	if ((((nstride << 2) | (n << 2) |
		(mlib_addr)y | (mlib_addr)z) & 7) == 0) {
		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s16 *py = (mlib_s16 *)y;

			ds = ds1 = ds2 = ds3 = 0;

			for (i = 0; i < m; i++) {
				dy = ((mlib_d64 *)py)[0];
				dx = dpx[i];
				VECTOR_MULM_S16C;
				VECTOR_SUM_S32C;
				py += 2 * nstride;
			}

			PACK_D64_SAT32;
/* store 16 bytes of result */
			dpz[0] = dr.db;
			dpz[1] = dr1.db;
			y = (void *)((mlib_s16 *)y + 4);
			dpz += 2;
		}
	} else {
		mlib_s32 off =
			((4 & (mlib_addr)z) + (mlib_u8 *)dpz -
			(mlib_u8 *)z) >> 1,
/*
 * generate edge mask for the start point
 */
			emask = vis_edge32(z, pzend);

		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s16 *py = (mlib_s16 *)y;

			ds = ds1 = ds2 = ds3 = 0;

			dpy = (mlib_d64 *)vis_alignaddr(py, off);
			dy_hi = vis_ld_d64_nf(dpy);
			dy_lo = vis_ld_d64_nf(dpy + 1);
			dy = vis_faligndata(dy_hi, dy_lo);
			dx = dpx[0];
			VECTOR_MULM_S16C;
			py += 2 * nstride;
			for (i = 1; i < m; i++) {
				VECTOR_SUM_S32C;
				dpy = (mlib_d64 *)vis_alignaddr(py, off);
				dy_hi = vis_ld_d64_nf(dpy);
				dy_lo = vis_ld_d64_nf(dpy + 1);
				dy = vis_faligndata(dy_hi, dy_lo);
				dx = dpx[i];
				VECTOR_MULM_S16C;
				py += 2 * nstride;
			}

			VECTOR_SUM_S32C;
			PACK_D64_SAT32;

			if (4 & (mlib_addr)z) {
				(void) vis_alignaddr(dpy, 4);
/* dr.db >>= 32 */
				dr.db = vis_faligndata(dr.db, dr.db);
/* store 1 * mlib_s32 of result */
				vis_pst_32(dr.db, dpz, emask & 1);

				if ((mlib_addr)++dpz <= (mlib_addr)pzend)
					vis_pst_32(dr.db, dpz, 2);

				if ((4 + (mlib_addr)dpz) <= (mlib_addr)pzend) {
					emask = vis_edge32(dpz, pzend);
/* dr1.db >>= 32 */
					dr1.db = vis_faligndata(dr1.db, dr1.db);
/* store 1 * mlib_s32 of result */
					vis_pst_32(dr1.db, dpz, emask & 1);

					if ((mlib_addr)++dpz <=
						(mlib_addr)pzend)
						vis_pst_32(dr1.db, dpz, 2);
				}
			} else {
/* store 2 * mlib_s32 of result */
				vis_pst_32(dr.db, dpz++, emask);

				if ((mlib_addr)dpz <= (mlib_addr)pzend) {
					emask = vis_edge32(dpz, pzend);
/* store 2 * mlib_s32 of result */
					vis_pst_32(dr1.db, dpz++, emask);
				}
			}

			y = (void *)((mlib_s16 *)y + 4);
			emask = vis_edge32(dpz, pzend);
		}
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

#undef VECTOR_SUM_S32C
#undef PACK_S32C_S16C

/* *********************************************************** */

#define	VECTOR_MULM_S16C                                            \
	dr4 = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dy));    \
	dr5 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dy));    \
	dr6 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_hi(dy));    \
	dr7 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_hi(dy));    \
	dr8 = vis_fmuld8sux16(vis_read_hi(dx), vis_read_lo(dy));    \
	dr9 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_lo(dy));    \
	dr10 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dy));   \
	dr11 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dy))

/* *********************************************************** */

#define	VECTOR_SUM_S32C                                         \
	ds = vis_fpadd32(ds, dr4);                              \
	ds1 = vis_fpadd32(ds1, dr6);                            \
	ds2 = vis_fpadd32(ds2, dr8);                            \
	ds3 = vis_fpadd32(ds3, dr10);                           \
	ds = vis_fpadd32(ds, dr5);                              \
	ds1 = vis_fpadd32(ds1, dr7);                            \
	ds2 = vis_fpadd32(ds2, dr9);                            \
	ds3 = vis_fpadd32(ds3, dr11)

/* *********************************************************** */

#define	PACK_S32C_S16C                                          \
	dr.two_float.fl0 =                                      \
	vis_fpsub32s(vis_read_hi(ds), vis_read_lo(ds));         \
	dr.two_float.fl1 =                                      \
	vis_fpsub32s(vis_read_hi(ds2), vis_read_lo(ds2));       \
	dr1.two_float.fl0 =                                     \
	vis_fpadd32s(vis_read_hi(ds1), vis_read_lo(ds1));       \
	dr1.two_float.fl1 =                                     \
	vis_fpadd32s(vis_read_hi(ds3), vis_read_lo(ds3));       \
	dr.db = vis_fand(dr.db, without_control);               \
	dr1.db = vis_fand(dr1.db, without_control);             \
	dr.db = vis_faligndata(dr.db, dr.db);                   \
	ds = vis_for(dr.db, dr1.db)

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 size, i;
	mlib_s16 *pzend = (mlib_s16 *)z + n + n - 1;
	mlib_d64 array[MAX_SIZE], *dpx, *dpy, *dpz;
	mlib_d64 dx, dy_hi, dy_lo, dy, ds, ds1, ds2, ds3;
	mlib_d64 dr4, dr5, dr6, dr7, dr8, dr9, dr10, dr11;
	type_union_mlib_d64 dr, dr1;
	mlib_d64 without_control = vis_to_double_dup(0xffff);

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = m;

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_S16C, type_S16C,
				mode_Mod, x, y, 1, m, n, nstride, z);
	}

	for (i = 0; i < m; i++) {
		((mlib_s16 *)dpx)[4 * i] = ((mlib_s16 *)dpx)[4 * i + 3] =
			((mlib_s16 *)x)[2 * i];
		((mlib_s16 *)dpx)[4 * i + 1] = ((mlib_s16 *)dpx)[4 * i + 2] =
			((mlib_s16 *)x)[2 * i + 1];
	}

	dpz = (mlib_d64 *)((mlib_addr)z & (~7));

	if ((((nstride << 2) | (n << 2) |
		(mlib_addr)y | (mlib_addr)z) & 7) == 0) {
		(void) vis_alignaddr(dpz, 2);
		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s16 *py = (mlib_s16 *)y;

			ds = ds1 = ds2 = ds3 = 0;

			for (i = 0; i < m; i++) {
				dy = ((mlib_d64 *)py)[0];
				dx = dpx[i];
				VECTOR_MULM_S16C;
				VECTOR_SUM_S32C;
				py += 2 * nstride;
			}

			PACK_S32C_S16C;
/* store 8 bytes of result */
			dpz[0] = ds;
			y = (void *)((mlib_s16 *)y + 4);
			dpz++;
		}
	} else {
		mlib_s32 off =
			(mlib_addr)dpz - (mlib_addr)z + (2 & (mlib_addr)z),
/*
 * generate edge mask for the start point
 */
			emask = vis_edge16(z, pzend);

		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s16 *py = (mlib_s16 *)y;

			ds = ds1 = ds2 = ds3 = 0;

			dpy = (mlib_d64 *)vis_alignaddr(py, off);
			dy_hi = vis_ld_d64_nf(dpy);
			dy_lo = vis_ld_d64_nf(dpy + 1);
			dy = vis_faligndata(dy_hi, dy_lo);
			dx = dpx[0];
			VECTOR_MULM_S16C;
			py += 2 * nstride;
			for (i = 1; i < m; i++) {
				VECTOR_SUM_S32C;
				dpy = (mlib_d64 *)vis_alignaddr(py, off);
				dy_hi = vis_ld_d64_nf(dpy);
				dy_lo = vis_ld_d64_nf(dpy + 1);
				dy = vis_faligndata(dy_hi, dy_lo);
				dx = dpx[i];
				VECTOR_MULM_S16C;
				py += 2 * nstride;
			}

			VECTOR_SUM_S32C;
			(void) vis_alignaddr(dpz, 2);
			PACK_S32C_S16C;

			if (2 & (mlib_addr)z) {
				(void) vis_alignaddr(dpz, 6);
/* ds >>= 16 */
				ds = vis_faligndata(ds, ds);
/* store 6 bytes of result */
				vis_pst_16(ds, dpz, emask & 7);

				if ((mlib_addr)++dpz <= (mlib_addr)pzend)
					vis_pst_16(ds, dpz, 8);
			} else
/* store 8 bytes of result */
				vis_pst_16(ds, dpz++, emask);
			y = (void *)((mlib_s16 *)y + 4);
			emask = vis_edge16(dpz, pzend);
		}
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	PACK_S32C_MOD32                                         \
	dr.two_float.fl0 = vis_fpsub32s(                        \
		vis_read_hi(ds), vis_read_lo(ds));              \
	dr1.two_float.fl0 = vis_fpsub32s(                       \
		vis_read_hi(ds2), vis_read_lo(ds2));            \
	dr.two_float.fl1 = vis_fpadd32s(                        \
		vis_read_hi(ds1), vis_read_lo(ds1));            \
	dr1.two_float.fl1 = vis_fpadd32s(                       \
		vis_read_hi(ds3), vis_read_lo(ds3))

/* *********************************************************** */

mlib_status
mlib_VectorMulM_S_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	mlib_s32 size, i;
	mlib_s32 *pzend = (mlib_s32 *)z + n + n - 1;
	mlib_d64 array[MAX_SIZE], *dpx, *dpy, *dpz;
	mlib_d64 dx, dy_hi, dy_lo, dy, ds, ds1, ds2, ds3;
	mlib_d64 dr4, dr5, dr6, dr7, dr8, dr9, dr10, dr11;
	type_union_mlib_d64 dr, dr1;

/* initialize GSR scale factor */
	vis_write_gsr(15 << 3);

	if (!((m > 0) && (n > 0)))
		return (MLIB_FAILURE);
	size = m;

	if (size <= MAX_SIZE)
		dpx = array;
	else {
		dpx = (mlib_d64 *)__mlib_malloc(size * sizeof (mlib_d64));

		if (dpx == NULL)
			return mlib_MatrixMul_type(type_S16C, type_S32C,
				mode_Mod, x, y, 1, m, n, nstride, z);
	}

	for (i = 0; i < m; i++) {
		((mlib_s16 *)dpx)[4 * i] = ((mlib_s16 *)dpx)[4 * i + 3] =
			((mlib_s16 *)x)[2 * i];
		((mlib_s16 *)dpx)[4 * i + 1] = ((mlib_s16 *)dpx)[4 * i + 2] =
			((mlib_s16 *)x)[2 * i + 1];
	}

	dpz = (mlib_d64 *)((mlib_addr)z & (~7));

	if ((((nstride << 2) | (n << 2) |
		(mlib_addr)y | (mlib_addr)z) & 7) == 0) {
		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s16 *py = (mlib_s16 *)y;

			ds = ds1 = ds2 = ds3 = 0;

			for (i = 0; i < m; i++) {
				dy = ((mlib_d64 *)py)[0];
				dx = dpx[i];
				VECTOR_MULM_S16C;
				VECTOR_SUM_S32C;
				py += 2 * nstride;
			}

			PACK_S32C_MOD32;
/* store 16 bytes of result */
			dpz[0] = dr.db;
			dpz[1] = dr1.db;
			y = (void *)((mlib_s16 *)y + 4);
			dpz += 2;
		}
	} else {
		mlib_s32 off = ((4 & (mlib_addr)z) + (mlib_u8 *)dpz -
			(mlib_u8 *)z) >> 1,
/*
 * generate edge mask for the start point
 */
			emask = vis_edge32(z, pzend);

		while ((mlib_addr)dpz <= (mlib_addr)pzend) {
			mlib_s16 *py = (mlib_s16 *)y;

			ds = ds1 = ds2 = ds3 = 0;

			dpy = (mlib_d64 *)vis_alignaddr(py, off);
			dy_hi = vis_ld_d64_nf(dpy);
			dy_lo = vis_ld_d64_nf(dpy + 1);
			dy = vis_faligndata(dy_hi, dy_lo);
			dx = dpx[0];
			VECTOR_MULM_S16C;
			py += 2 * nstride;
			for (i = 1; i < m; i++) {
				VECTOR_SUM_S32C;
				dpy = (mlib_d64 *)vis_alignaddr(py, off);
				dy_hi = vis_ld_d64_nf(dpy);
				dy_lo = vis_ld_d64_nf(dpy + 1);
				dy = vis_faligndata(dy_hi, dy_lo);
				dx = dpx[i];
				VECTOR_MULM_S16C;
				py += 2 * nstride;
			}

			VECTOR_SUM_S32C;
			PACK_S32C_MOD32;

			if (4 & (mlib_addr)z) {
				(void) vis_alignaddr(dpy, 4);
/* dr.db >>= 32 */
				dr.db = vis_faligndata(dr.db, dr.db);
/* store 1 * mlib_s32 of result */
				vis_pst_32(dr.db, dpz, emask & 1);

				if ((mlib_addr)++dpz <= (mlib_addr)pzend)
					vis_pst_32(dr.db, dpz, 2);

				if ((4 + (mlib_addr)dpz) <= (mlib_addr)pzend) {
					emask = vis_edge32(dpz, pzend);
/* dr1.db >>= 32 */
					dr1.db = vis_faligndata(dr1.db, dr1.db);
/* store 1 * mlib_s32 of result */
					vis_pst_32(dr1.db, dpz, emask & 1);

					if ((mlib_addr)++dpz <=
						(mlib_addr)pzend)
						vis_pst_32(dr1.db, dpz, 2);
				}
			} else {
/* store 2 * mlib_s32 of result */
				vis_pst_32(dr.db, dpz++, emask);

				if ((mlib_addr)dpz <= (mlib_addr)pzend) {
					emask = vis_edge32(dpz, pzend);
/* store 2 * mlib_s32 of result */
					vis_pst_32(dr1.db, dpz++, emask);
				}
			}

			y = (void *)((mlib_s16 *)y + 4);
			emask = vis_edge32(dpz, pzend);
		}
	}

	if (size > MAX_SIZE)
		__mlib_free(dpx);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulM_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_U8_U8_Sat(z,
		x, y, m, n, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulM_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_U8_U8_Mod(z,
		x, y, m, n, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulM_S16_U8_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S16_U8_Sat(z,
		x, y, m, n, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulM_S16_U8_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S16_U8_Mod(z,
		x, y, m, n, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulM_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S8_S8_Sat(z,
		x, y, m, n, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulM_S8_S8_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S8_S8_Mod(z,
		x, y, m, n, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulM_S16_S8_Sat(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S16_S8_Sat(z,
		x, y, m, n, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulM_S16_S8_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S16_S8_Mod(z,
		x, y, m, n, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulM_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S16_S16_Sat(z,
		x, y, m, n, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulM_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S16_S16_Mod(z,
		x, y, m, n, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulM_S32_S16_Sat(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S32_S16_Sat(z,
		x, y, m, n, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulM_S32_S16_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S32_S16_Mod(z,
		x, y, m, n, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulM_U8C_U8C_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_U8C_U8C_Sat(z,
		x, y, m, n, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulM_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_U8C_U8C_Mod(z,
		x, y, m, n, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulM_S16C_U8C_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S16C_U8C_Sat(z,
		x, y, m, n, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulM_S16C_U8C_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S16C_U8C_Mod(z,
		x, y, m, n, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulM_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S8C_S8C_Sat(z,
		x, y, m, n, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulM_S8C_S8C_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S8C_S8C_Mod(z,
		x, y, m, n, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulM_S16C_S8C_Sat(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S16C_S8C_Sat(z,
		x, y, m, n, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulM_S16C_S8C_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S16C_S8C_Mod(z,
		x, y, m, n, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulM_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S16C_S16C_Sat(z,
		x, y, m, n, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulM_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S16C_S16C_Mod(z,
		x, y, m, n, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulM_S32C_S16C_Sat(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S32C_S16C_Sat(z,
		x, y, m, n, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulM_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return mlib_VectorMulM_S_S32C_S16C_Mod(z,
		x, y, m, n, n);
}

/* *********************************************************** */
