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

#pragma ident	"@(#)mlib_v_VectorDotProd16C.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorDotProd_S16C_Sat - dot product of signed 16-bit format
 *                                    complex vectors
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorDotProd_S16C_Sat(mlib_d64       *z,
 *                                              const mlib_s16 *x,
 *                                              const mlib_s16 *y,
 *                                              mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the dot product of the vectors. z[0] hold the
 *           real parts, and z[1] hold the imaginary parts
 *      x    pointer to the first complex element of the first vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      y    pointer to the first complex element of the second vector.
 *           y[2*i] hold the real parts, and y[2*i + 1] hold the imaginary
 *           parts
 *      n    number of complex elements in the vectors
 *
 * DESCRIPTION
 *           n              *
 *      z = SUM (x[i] * y[i] )
 *          i = 1
 */

#include <mlib_algebra.h>
#include <mlib_AlgebraUtil.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorDotProd_S16C_Sat = __mlib_VectorDotProd_S16C_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorDotProd_S16C_Sat) mlib_VectorDotProd_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorDotProd_S16C_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	DPROD_S16C_OLD                                                    \
	d_sw2 = vis_bshuffle(dx, dx);                                     \
	d_rrii.db = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dy));    \
	d_rrii1 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dy));      \
	d_rrii.db = vis_fpadd32(d_rrii.db, d_rrii1);                      \
	d_irri.db =                                                       \
	vis_fmuld8sux16(vis_read_hi(d_sw2), vis_read_hi(dy));             \
	d_irri1 = vis_fmuld8ulx16(vis_read_hi(d_sw2), vis_read_hi(dy));   \
	d_irri.db = vis_fpadd32(d_irri.db, d_irri1);                      \
	ds_r = ds_r + (mlib_d64)d_rrii.two_int.int0 +                     \
		(mlib_d64)d_rrii.two_int.int1;                            \
	d_irri.two_float.fl0 =                                            \
	vis_fpsub32s(d_irri.two_float.fl0, d_irri.two_float.fl1);         \
	ds_i = ds_i + (mlib_d64)d_irri.two_int.int0;                      \
	d_rrii.db = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dy));    \
	d_rrii1 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dy));      \
	d_rrii.db = vis_fpadd32(d_rrii.db, d_rrii1);                      \
	d_irri.db =                                                       \
	vis_fmuld8sux16(vis_read_lo(d_sw2), vis_read_lo(dy));             \
	d_irri1 = vis_fmuld8ulx16(vis_read_lo(d_sw2), vis_read_lo(dy));   \
	d_irri.db = vis_fpadd32(d_irri.db, d_irri1);                      \
	ds_r = ds_r + (mlib_d64)d_rrii.two_int.int0 +                     \
		(mlib_d64)d_rrii.two_int.int1;                            \
	d_irri.two_float.fl0 =                                            \
	vis_fpsub32s(d_irri.two_float.fl0, d_irri.two_float.fl1);         \
	ds_i = ds_i + (mlib_d64)d_irri.two_int.int0

#define	DPROD_S16C_P                                                      \
	d_sw2 = vis_bshuffle(dy, dy);                                     \
	d_rrii.db = vis_fmuld8sux16(vis_read_hi(dy), vis_read_hi(dx));    \
	d_rrii1 = vis_fmuld8ulx16(vis_read_hi(dy), vis_read_hi(dx));      \
	d_rrii.db = vis_fpadd32(d_rrii.db, d_rrii1);                      \
	d_irri.db =                                                       \
	vis_fmuld8sux16(vis_read_lo(d_sw2), vis_read_hi(dx));             \
	d_irri1 = vis_fmuld8ulx16(vis_read_lo(d_sw2), vis_read_hi(dx));   \
	d_irri.db = vis_fpadd32(d_irri.db, d_irri1);                      \
	ds_r = ds_r + (mlib_d64)d_rrii.two_int.int0 +                     \
		(mlib_d64)d_rrii.two_int.int1;                            \
	ds_i = ds_i + (mlib_d64)(-d_irri.two_int.int0 +                   \
		d_irri.two_int.int1);                                     \
	d_rrii.db = vis_fmuld8sux16(vis_read_lo(dy), vis_read_lo(dx));    \
	d_rrii1 = vis_fmuld8ulx16(vis_read_lo(dy), vis_read_lo(dx));      \
	d_rrii.db = vis_fpadd32(d_rrii.db, d_rrii1);                      \
	d_irri.db =                                                       \
	vis_fmuld8sux16(vis_read_hi(d_sw2), vis_read_lo(dx));             \
	d_irri1 = vis_fmuld8ulx16(vis_read_hi(d_sw2), vis_read_lo(dx));   \
	d_irri.db = vis_fpadd32(d_irri.db, d_irri1);                      \
	ds_r = ds_r + (mlib_d64)d_rrii.two_int.int0 +                     \
		(mlib_d64)d_rrii.two_int.int1;                            \
	ds_i = ds_i + (mlib_d64)(-d_irri.two_int.int0 +                   \
		d_irri.two_int.int1)

/* *********************************************************** */

#define	DPROD_S16C                                                        \
	d_sw = vis_fpmerge(vis_read_lo(dx), vis_read_hi(dx));             \
	d_sw1 = vis_fpmerge(vis_read_lo(d_sw), vis_read_hi(d_sw));        \
	d_sw2 = vis_fpmerge(vis_read_hi(d_sw1), vis_read_lo(d_sw1));      \
	d_rrii.db = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dy));    \
	d_rrii1 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dy));      \
	d_rrii.db = vis_fpadd32(d_rrii.db, d_rrii1);                      \
	d_irri.db =                                                       \
	vis_fmuld8sux16(vis_read_lo(d_sw2), vis_read_hi(dy));             \
	d_irri1 = vis_fmuld8ulx16(vis_read_lo(d_sw2), vis_read_hi(dy));   \
	d_irri.db = vis_fpadd32(d_irri.db, d_irri1);                      \
	ds_r = ds_r + (mlib_d64)d_rrii.two_int.int0 +                     \
		(mlib_d64)d_rrii.two_int.int1;                            \
	ds_i = ds_i + (mlib_d64)((mlib_d64)d_irri.two_int.int0 -          \
		(mlib_d64)d_irri.two_int.int1);                           \
	d_rrii.db = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dy));    \
	d_rrii1 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dy));      \
	d_rrii.db = vis_fpadd32(d_rrii.db, d_rrii1);                      \
	d_irri.db =                                                       \
	vis_fmuld8sux16(vis_read_hi(d_sw2), vis_read_lo(dy));             \
	d_irri1 = vis_fmuld8ulx16(vis_read_hi(d_sw2), vis_read_lo(dy));   \
	d_irri.db = vis_fpadd32(d_irri.db, d_irri1);                      \
	ds_r = ds_r + (mlib_d64)d_rrii.two_int.int0 +                     \
		(mlib_d64)d_rrii.two_int.int1;                            \
	ds_i = ds_i + (mlib_d64)((mlib_d64)d_irri.two_int.int0 -          \
		(mlib_d64)d_irri.two_int.int1)

/* *********************************************************** */

static void mlib_VectorDotProd_S16C_al_x(
	mlib_d64 *z,
	const void *x,
	const void *y,
	mlib_s32 n);
static void mlib_VectorDotProd_S16C_notal(
	mlib_d64 *z,
	const void *x,
	const void *y,
	mlib_s32 n);

/* *********************************************************** */

mlib_status
__mlib_VectorDotProd_S16C_Sat(
	mlib_d64 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	if (n <= 0)
		return (MLIB_FAILURE);

/* addr of x is multiple of 4 */
	if (!((mlib_addr)x & 2)) {
		mlib_VectorDotProd_S16C_al_x(z, x, y, n);
	} else if (!((mlib_addr)y & 2)) {
/* addr of y is multiple of 4 */
		mlib_VectorDotProd_S16C_al_x(z, y, x, n);
		z[1] = -z[1];
	} else {
/* both are not multiple of 4 */
		mlib_VectorDotProd_S16C_notal(z, x, y, n);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

/* The case when address of vector x is multiple of 4 */

static void
mlib_VectorDotProd_S16C_al_x(
	mlib_d64 *z,
	const void *x,
	const void *y,
	mlib_s32 n)
{
	mlib_s16 *pxend, *px = (mlib_s16 *)x, *py = (mlib_s16 *)y;
	mlib_d64 *dpx, *dpy, *dpxend;
	mlib_d64 dx, dy, dy0, dy1;
	mlib_d64 d_sw2, d_rrii1, d_irri1;
	type_union_mlib_d64 d_rrii, d_irri;
	mlib_d64 ds_r = 0.0, ds_i = 0.0;
	mlib_d64 edge[2];
	mlib_s32 d_left;
	mlib_s32 emask, off;

	edge[0] = edge[1] = 0;

	dpx = (mlib_d64 *)((mlib_addr)px & (~7));
	off = (mlib_addr)dpx - (mlib_u32)px;
	dpy = (mlib_d64 *)vis_alignaddr(py, off);
	pxend = px + n + n - 1;
	dpxend = (mlib_d64 *)((mlib_addr)pxend & (~7));
	d_left = dpxend - dpx;

	emask = vis_edge16(px, pxend);
	vis_pst_16(dpx[0], edge, emask);
	dx = edge[0];
	dy = vis_ld_d64_nf(dpy);

	vis_write_bmask(0, 0x67452301);

	if (((((mlib_addr)px) ^ ((mlib_addr)py)) & 7) == 0) {
		for (; d_left > 0; d_left--) {
			DPROD_S16C_P;

			dpx++;
			dpy++;
			dx = dpx[0];
			dy = dpy[0];
		}
	} else {
		dy1 = vis_ld_d64_nf(dpy+1);
		dy = vis_faligndata(dy, dy1);
		vis_write_bmask(0, 0x23016745);
		for (; d_left > 0; d_left--) {
			DPROD_S16C_OLD;
			dpx++;
			dpy++;
			dy0 = dy1;
			dy1 = vis_ld_d64_nf(dpy + 1);
			dy = vis_faligndata(dy0, dy1);
			dx = dpx[0];
		}

		vis_write_bmask(0, 0x67452301);
	}

	if ((mlib_addr)dpx <= (mlib_addr)pxend) {
		emask = vis_edge16(dpx, pxend);
		vis_pst_16(dx, edge + 1, emask);
		dx = edge[1];
		DPROD_S16C_P;
	}

	z[0] = ds_r;
	z[1] = ds_i;
}

/* *********************************************************** */

static void
mlib_VectorDotProd_S16C_notal(
	mlib_d64 *z,
	const void *x,
	const void *y,
	mlib_s32 n)
/* The case when address of vector x is not multiple of 4 */
{
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y;
	mlib_d64 *dpx, *dpy;
	mlib_d64 dx, dx0, dx1, dy, dy0, dy1;
	mlib_d64 d_sw, d_sw1, d_sw2, d_rrii1, d_irri1;
	type_union_mlib_d64 d_rrii, d_irri;
	mlib_d64 ds_r = 0.0, ds_i = 0.0;
	mlib_d64 mlib_tail_mask = vis_to_double(0xFFFFFFFF, 0);
	mlib_s32 d_left;
	mlib_s32 m;

	vis_write_bmask(0, 0x67452301);

	dpx = (mlib_d64 *)vis_alignaddr(px, 0);
	dpy = (mlib_d64 *)((mlib_addr)py & (~7));
	dx0 = dpx[0];
	dy0 = dpy[0];
	dpx++;
	dpy++;

	d_left = n >> 1;

	if (((((mlib_addr)px) ^ ((mlib_addr)py)) & 7) == 0) {
		for (; d_left > 0; d_left--) {
			dx1 = dpx[0];
			dy1 = dpy[0];
			dx = vis_faligndata(dx0, dx1);
			dy = vis_faligndata(dy0, dy1);
			dx0 = dx1;
			dy0 = dy1;
			dpx++;
			dpy++;
			DPROD_S16C_P;
		}

		if (n & 1) {
			dx1 = vis_ld_d64_nf(dpx);
			dy1 = vis_ld_d64_nf(dpy);
			dx = vis_faligndata(dx0, dx1);
			dx = vis_fand(dx, mlib_tail_mask);
			dy = vis_faligndata(dy0, dy1);
			DPROD_S16C_P;
		}

	} else {
		m = (mlib_s32)py & 7;
		vis_write_bmask(m * 0x11111111, 0x01234567);
		(void) vis_alignaddr(px, 0);
		for (; d_left > 0; d_left--) {
			dx1 = dpx[0];
			dx = vis_faligndata(dx0, dx1);
			dy1 = dpy[0];
			dy = vis_bshuffle(dy0, dy1);
			DPROD_S16C;
			dx0 = dx1;
			dy0 = dy1;
			dpx++;
			dpy++;
		}

		vis_write_bmask(0, 0x67452301);

		if (n & 1) {
			dx1 = vis_ld_d64_nf(dpx);
			dy1 = vis_ld_d64_nf(dpy);
			(void) vis_alignaddr(px, 0);
			dx = vis_faligndata(dx0, dx1);
			dx = vis_fand(dx, mlib_tail_mask);
			(void) vis_alignaddr(py, 0);
			dy = vis_faligndata(dy0, dy1);
			DPROD_S16C_P;
		}
	}

	z[0] = ds_r;
	z[1] = ds_i;
}

/* *********************************************************** */
