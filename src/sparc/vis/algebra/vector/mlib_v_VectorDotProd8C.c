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

#pragma ident	"@(#)mlib_v_VectorDotProd8C.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorDotProd_S8C_Sat - dot product of unsigned 8-bit format
 *                              complex vectors
 *      mlib_VectorDotProd_S8C_Sat - dot product of signed 8-bit format
 *                              complex vectors
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorDotProd_S8C_Sat(mlib_d64      *z,
 *                                             const mlib_s8 *x,
 *                                             const mlib_s8 *y,
 *                                             mlib_s32      n);
 *      mlib_status mlib_VectorDotProd_U8C_Sat(mlib_d64      *z,
 *                                             const mlib_u8 *x,
 *                                             const mlib_u8 *y,
 *                                             mlib_s32      n);
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
 *           n          *
 *      z = SUM (x[i] * y[i] )
 *          i = 1
 */

#include <mlib_algebra.h>
#include <mlib_AlgebraUtil.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorDotProd_U8C_Sat = __mlib_VectorDotProd_U8C_Sat
#pragma weak mlib_VectorDotProd_S8C_Sat = __mlib_VectorDotProd_S8C_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorDotProd_U8C_Sat) mlib_VectorDotProd_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorDotProd_U8C_Sat")));
__typeof__(__mlib_VectorDotProd_S8C_Sat) mlib_VectorDotProd_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorDotProd_S8C_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_VectorDotProd_U8C_al_x(
	mlib_d64 *z,
	const void *x,
	const void *y,
	mlib_s32 n);
static void mlib_VectorDotProd_U8C_notal(
	mlib_d64 *z,
	const void *x,
	const void *y,
	mlib_s32 n);
static void mlib_VectorDotProd_S8C_al_x(
	mlib_d64 *z,
	const void *x,
	const void *y,
	mlib_s32 n);
static void mlib_VectorDotProd_S8C_notal(
	mlib_d64 *z,
	const void *x,
	const void *y,
	mlib_s32 n);

/* *********************************************************** */

mlib_status
__mlib_VectorDotProd_U8C_Sat(
	mlib_d64 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	if (n <= 0)
		return (MLIB_FAILURE);

/* addr of x is even */
	if (!((mlib_addr)x & 1)) {
		mlib_VectorDotProd_U8C_al_x(z, x, y, n);
	} else if (!((mlib_addr)y & 1)) {
/* addr of y is even */
		mlib_VectorDotProd_U8C_al_x(z, y, x, n);
		z[1] = -z[1];
	} else {
/* both are odd */
		mlib_VectorDotProd_U8C_notal(z, x, y, n);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SET_ALIGN_U8C                                           \
	(void) vis_alignaddr((void *)dpx, 7);                   \
/* for shift right */

#define	DPROD_U8C                                                      \
	dx_r = vis_fmul8sux16(dx, done);                               \
	dy_i = vis_fand(dy, lb_mask);                                  \
	dy_r = vis_fmul8sux16(dy, done);                               \
	dy_r = vis_fand(dy_r, lb_mask);                                \
	d_iih = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dy_i));   \
	d_iil = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dy_i));   \
	d_irh = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dy_r));   \
	d_irl = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dy_r));   \
	d_rih = vis_fmuld8ulx16(vis_read_hi(dx_r), vis_read_hi(dy_i)); \
	d_ril = vis_fmuld8ulx16(vis_read_lo(dx_r), vis_read_lo(dy_i)); \
	d_rrh = vis_fmuld8ulx16(vis_read_hi(dx_r), vis_read_hi(dy_r)); \
	d_rrl = vis_fmuld8ulx16(vis_read_lo(dx_r), vis_read_lo(dy_r))

#define	SUM_U8C                                                       \
	/*                                                            \
	 * (Xr + jXi) * (Yr - jYi) = (XrYr + XiYi) + j(XiYr - XrYi)   \
	 */                                                           \
	d_ih = vis_fpsub32(d_irh, d_rih);                             \
	d_il = vis_fpsub32(d_irl, d_ril);                             \
	d_rh = vis_fpadd32(d_rrh, d_iih);                             \
	d_rl = vis_fpadd32(d_rrl, d_iil);                             \
	ds_i = vis_fpadd32(ds_i, d_ih);                               \
	ds1_i = vis_fpadd32(ds1_i, d_il);                             \
	ds_r = vis_fpadd32(ds_r, d_rh);                               \
	ds1_r = vis_fpadd32(ds1_r, d_rl)

#define	SUM_U8C_TAIL                                                   \
	d_ih = vis_fpsub32(d_irh, d_rih);                              \
	d_il = vis_fpsub32(d_irl, d_ril);                              \
	d_rh = vis_fpadd32(d_rrh, d_iih);                              \
	d_rl = vis_fpadd32(d_rrl, d_iil);                              \
	ds_i = vis_fpadd32(d_ih, d_il);                                \
	ds_r = vis_fpadd32(d_rh, d_rl)

#define	MAX_LOOP	((MLIB_U16_MAX + 1)/8)

/* *********************************************************** */

static void
mlib_VectorDotProd_U8C_al_x(
	mlib_d64 *z,
	const void *x,
	const void *y,
	mlib_s32 n)
/* The case of even address of vector x */
{
	mlib_u8 *pxend, *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_d64 sum_r = 0.0, sum_i = 0.0;
	mlib_d64 *dpx, *dpy, *dpxend;
	mlib_d64 dx, dy, dy0, dy1;
	mlib_d64 dx_r, dy_r, dy_i;
	mlib_d64 d_iih, d_iil, d_irh, d_irl, d_rih, d_ril, d_rrh, d_rrl;
	mlib_d64 d_ih, d_il, d_rh, d_rl;
	mlib_d64 ds_r, ds_i, ds1_r, ds1_i;
	mlib_d64 lb_mask = vis_to_double_dup(0x00FF00FF);
	mlib_d64 edge[2];
	mlib_f32 fsum;
	mlib_s32 d_left;
	mlib_s32 emask, off;
	mlib_d64 done = vis_to_double_dup(0x1000100);

	edge[0] = edge[1] = 0;

	dpx = (mlib_d64 *)((mlib_addr)px & (~7));
	off = (mlib_addr)dpx - (mlib_addr)px;
	dpy = vis_alignaddr((void *)py, off);
	pxend = px + n + n - 1;
	dpxend = (mlib_d64 *)((mlib_addr)pxend & (~7));
	emask = vis_edge8(px, pxend);
	vis_pst_8(dpx[0], edge, emask);
	dx = edge[0];
	dy = vis_ld_d64_nf(dpy);

	if (((((mlib_addr)px) ^ ((mlib_addr)py)) & 7) == 0) {
		while ((mlib_addr)dpx < (mlib_addr)dpxend) {
			d_left = dpxend - dpx;

			if (d_left > MAX_LOOP)
				d_left = MAX_LOOP;
			ds_i = ds_r = ds1_i = ds1_r = 0.0;
			for (; d_left > 0; d_left--) {
				DPROD_U8C;
				SUM_U8C;
				dx = dpx[1];
				dy = dpy[1];
				dpx++;
				dpy++;
			}

			ds_i = vis_fpadd32(ds_i, ds1_i);
			ds_r = vis_fpadd32(ds_r, ds1_r);
			fsum = vis_read_hi(ds_r);
			sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_lo(ds_r);
			sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_hi(ds_i);
			sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_lo(ds_i);
			sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
		}

	} else {
		dy1 = vis_ld_d64_nf(dpy+1);
		dy = vis_faligndata(dy, dy1);
		while ((mlib_addr)dpx < (mlib_addr)dpxend) {
			d_left = dpxend - dpx;

			if (d_left > MAX_LOOP)
				d_left = MAX_LOOP;
			ds_i = ds_r = ds1_i = ds1_r = 0.0;
			for (; d_left > 0; d_left--) {
				DPROD_U8C;
				SUM_U8C;
				dy0 = dy1;
				dy1 = vis_ld_d64_nf(dpy+2);
				dx = vis_ld_d64_nf(dpx+1);
				dy = vis_faligndata(dy0, dy1);
				dpx++;
				dpy++;
			}

			ds_i = vis_fpadd32(ds_i, ds1_i);
			ds_r = vis_fpadd32(ds_r, ds1_r);
			fsum = vis_read_hi(ds_r);
			sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_lo(ds_r);
			sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_hi(ds_i);
			sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_lo(ds_i);
			sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
		}
	}

	if ((mlib_addr)dpx <= (mlib_addr)pxend) {
		emask = vis_edge8(dpx, pxend);
		vis_pst_8(dx, edge + 1, emask);
		dx = edge[1];
		DPROD_U8C;
		SUM_U8C_TAIL;
		fsum = vis_read_hi(ds_r);
		sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
		fsum = vis_read_lo(ds_r);
		sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
		fsum = vis_read_hi(ds_i);
		sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
		fsum = vis_read_lo(ds_i);
		sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
	}

	z[0] = sum_r;
	z[1] = sum_i;
#undef MAX_LOOP
}

/* *********************************************************** */

#define	MAX_LOOP	((MLIB_U16_MAX + 1)/8)

union table
{
	mlib_u16 sh[12];
	mlib_d64 db[3];
};

static union table mlib_tail_mask = {
	0xFFFF, 0x0000, 0x0000, 0x0000,
	0xFFFF, 0xFFFF, 0x0000, 0x0000,
	0xFFFF, 0xFFFF, 0xFFFF, 0x0000
};

/* *********************************************************** */

static void
mlib_VectorDotProd_U8C_notal(
	mlib_d64 *z,
	const void *x,
	const void *y,
	mlib_s32 n)
/* The case of odd address of vector x */
{
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_d64 sum_r = 0.0, sum_i = 0.0;
	mlib_d64 *dpx, *dpy;
	mlib_d64 dx, dx0, dx1, dy, dy0, dy1;
	mlib_d64 dx_r, dy_r, dy_i;
	mlib_d64 d_iih, d_iil, d_irh, d_irl, d_rih, d_ril, d_rrh, d_rrl;
	mlib_d64 d_ih, d_il, d_rh, d_rl;
	mlib_d64 ds_r, ds_i, ds1_r, ds1_i;
	mlib_d64 lb_mask = vis_to_double_dup(0x00FF00FF);
	mlib_f32 fsum;
	mlib_s32 d_left, d_left_a;
	mlib_d64 done = vis_to_double_dup(0x1000100);

	dpx = (mlib_d64 *)vis_alignaddr(px, 0);
	dpy = (mlib_d64 *)((mlib_addr)py & (~7));
	dx0 = dpx[0];
	dy0 = dpy[0];
	dpx++;
	dpy++;

	d_left_a = n >> 2;

	if (((((mlib_addr)px) ^ ((mlib_addr)py)) & 7) == 0) {
		for (; d_left_a > 0; d_left_a -= MAX_LOOP) {
			d_left = d_left_a;

			if (d_left > MAX_LOOP)
				d_left = MAX_LOOP;
			ds_i = ds_r = ds1_i = ds1_r = 0.0;
			for (; d_left > 0; d_left--) {
				dx1 = dpx[0];
				dy1 = dpy[0];
				dx = vis_faligndata(dx0, dx1);
				dy = vis_faligndata(dy0, dy1);
				dpx++;
				dpy++;
				DPROD_U8C;
				SUM_U8C;
				dx0 = dx1;
				dy0 = dy1;
			}

			ds_i = vis_fpadd32(ds_i, ds1_i);
			ds_r = vis_fpadd32(ds_r, ds1_r);
			fsum = vis_read_hi(ds_r);
			sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_lo(ds_r);
			sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_hi(ds_i);
			sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_lo(ds_i);
			sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
		}

		if (n & 3) {
			dx1 = vis_ld_d64_nf(dpx);
			dy1 = vis_ld_d64_nf(dpy);
			dx = vis_faligndata(dx0, dx1);
			dx = vis_fand(dx, mlib_tail_mask.db[(n & 3) - 1]);
			dy = vis_faligndata(dy0, dy1);
			SET_ALIGN_U8C;
			DPROD_U8C;
			SUM_U8C_TAIL;
			fsum = vis_read_hi(ds_r);
			sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_lo(ds_r);
			sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_hi(ds_i);
			sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_lo(ds_i);
			sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
		}

	} else {
		for (; d_left_a > 0; d_left_a -= MAX_LOOP) {
			d_left = d_left_a;

			if (d_left > MAX_LOOP)
				d_left = MAX_LOOP;
			ds_i = ds_r = ds1_i = ds1_r = 0.0;
			for (; d_left > 0; d_left--) {
				dx1 = dpx[0];
				dx = vis_faligndata(dx0, dx1);
				(void) vis_alignaddr((void *)py, 0);
				dy1 = dpy[0];
				dy = vis_faligndata(dy0, dy1);
				dx0 = dx1;
				dy0 = dy1;
				dpx++;
				dpy++;
				DPROD_U8C;
				(void) vis_alignaddr((void *)px, 0);
				SUM_U8C;
			}

			ds_i = vis_fpadd32(ds_i, ds1_i);
			ds_r = vis_fpadd32(ds_r, ds1_r);
			fsum = vis_read_hi(ds_r);
			sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_lo(ds_r);
			sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_hi(ds_i);
			sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_lo(ds_i);
			sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
		}

		if (n & 3) {
			dx1 = vis_ld_d64_nf(dpx);
			dy1 = vis_ld_d64_nf(dpy);
			dx = vis_faligndata(dx0, dx1);
			dx = vis_fand(dx, mlib_tail_mask.db[(n & 3) - 1]);
			(void) vis_alignaddr((void *)py, 0);
			dy = vis_faligndata(dy0, dy1);
			DPROD_U8C;
			SUM_U8C_TAIL;
			fsum = vis_read_hi(ds_r);
			sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_lo(ds_r);
			sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_hi(ds_i);
			sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_lo(ds_i);
			sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
		}
	}

	z[0] = sum_r;
	z[1] = sum_i;
#undef MAX_LOOP
}

/* *********************************************************** */

mlib_status
__mlib_VectorDotProd_S8C_Sat(
	mlib_d64 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	if (n <= 0)
		return (MLIB_FAILURE);

/* addr of x is even */
	if (!((mlib_addr)x & 1)) {
		mlib_VectorDotProd_S8C_al_x(z, x, y, n);
	} else if (!((mlib_addr)y & 1)) {
/* addr of y is even */
		mlib_VectorDotProd_S8C_al_x(z, y, x, n);
		z[1] = -z[1];
	} else {
/* both are odd */
		mlib_VectorDotProd_S8C_notal(z, x, y, n);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	DPROD_S8C                                                        \
	dx_i = vis_fpack32(dx, fzero);                                   \
	/*                                                               \
	 * shift one byte left                                           \
	 */                                                              \
	dy_r = vis_fmul8sux16(dy, done);                                 \
	/*                                                               \
	 * mlib_s32 unpack 8 to 16                                       \
	 */                                                              \
	dy_i = vis_fpack32(dy, fzero);                                   \
	/*                                                               \
	 * shift one byte left                                           \
	 */                                                              \
	dy_i = vis_fmul8sux16(dy_i, done);                               \
	/*                                                               \
	 * mlib_s32 unpack 8 to 16                                       \
	 */                                                              \
	d_rrh = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dy_r));     \
	d_rrl = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dy_r));     \
	d_rih = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dy_i));     \
	d_ril = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dy_i));     \
	d_irh = vis_fmuld8sux16(vis_read_hi(dx_i), vis_read_hi(dy_r));   \
	d_irl = vis_fmuld8sux16(vis_read_lo(dx_i), vis_read_lo(dy_r));   \
	d_iih = vis_fmuld8sux16(vis_read_hi(dx_i), vis_read_hi(dy_i));   \
	d_iil = vis_fmuld8sux16(vis_read_lo(dx_i), vis_read_lo(dy_i));   \
	/*                                                               \
	 * (Xr + jXi) * (Yr - jYi) = (XrYr + XiYi) + j(XiYr - XrYi)      \
	 */                                                              \
	d_ih = vis_fpsub32(d_irh, d_rih);                                \
	d_il = vis_fpsub32(d_irl, d_ril);                                \
	d_rh = vis_fpadd32(d_rrh, d_iih);                                \
	d_rl = vis_fpadd32(d_rrl, d_iil);                                \
	ds_i = vis_fpadd32(ds_i, d_ih);                                  \
	ds1_i = vis_fpadd32(ds1_i, d_il);                                \
	ds_r = vis_fpadd32(ds_r, d_rh);                                  \
	ds1_r = vis_fpadd32(ds1_r, d_rl)

#define	DPROD_S8C_TAIL                                                   \
	/* shift one byte left */                                        \
	dx_i = vis_fpack32(dx, fzero);                                   \
	/* mlib_s32 unpack 8 to 16 */                                    \
	dy_r = vis_fmul8sux16(dy, done);                                 \
	/* shift one byte left */                                        \
	dy_i = vis_fpack32(dy, fzero);                                   \
	/* mlib_s32 unpack 8 to 16 */                                    \
	dy_i = vis_fmul8sux16(dy_i, done);                               \
	d_rrh = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dy_r));     \
	d_rrl = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dy_r));     \
	d_rih = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dy_i));     \
	d_ril = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dy_i));     \
	d_irh = vis_fmuld8sux16(vis_read_hi(dx_i), vis_read_hi(dy_r));   \
	d_irl = vis_fmuld8sux16(vis_read_lo(dx_i), vis_read_lo(dy_r));   \
	d_iih = vis_fmuld8sux16(vis_read_hi(dx_i), vis_read_hi(dy_i));   \
	d_iil = vis_fmuld8sux16(vis_read_lo(dx_i), vis_read_lo(dy_i));   \
	d_ih = vis_fpsub32(d_irh, d_rih);                                \
	d_il = vis_fpsub32(d_irl, d_ril);                                \
	d_rh = vis_fpadd32(d_rrh, d_iih);                                \
	d_rl = vis_fpadd32(d_rrl, d_iil);                                \
	ds_i = vis_fpadd32(d_ih, d_il);                                  \
	ds_r = vis_fpadd32(d_rh, d_rl)

#define	MAX_LOOP	(255)

/* *********************************************************** */

static void
mlib_VectorDotProd_S8C_al_x(
	mlib_d64 *z,
	const void *x,
	const void *y,
	mlib_s32 n)
/* The case of even address of vector x */
{
	mlib_s8 *pxend, *px = (mlib_s8 *)x, *py = (mlib_s8 *)y;
	mlib_d64 sum_r = 0.0, sum_i = 0.0;
	mlib_d64 *dpx, *dpy, *dpxend;
	mlib_d64 dx, dy, dy0, dy1;
	mlib_d64 dx_i, dy_r, dy_i;
	mlib_d64 d_iih, d_iil, d_irh, d_irl, d_rih, d_ril, d_rrh, d_rrl;
	mlib_d64 d_ih, d_il, d_rh, d_rl;
	mlib_d64 ds_r, ds_i, ds1_r, ds1_i;
	mlib_d64 edge[2];
	mlib_d64 fzero = vis_fzero();
	mlib_d64 done = vis_to_double_dup(0x01000100);
	mlib_f32 fsum;
	mlib_s32 d_left;
	mlib_s32 emask, off;

	edge[0] = edge[1] = 0;

	dpx = (mlib_d64 *)((mlib_addr)px & (~7));
	dpy = (mlib_d64 *)((mlib_addr)py & (~7));
	pxend = px + n + n - 1;
	dpxend = (mlib_d64 *)((mlib_addr)pxend & (~7));
	emask = vis_edge8(px, pxend);
	vis_pst_8(dpx[0], edge, emask);
	dx = edge[0];

	if (((((mlib_addr)px) ^ ((mlib_addr)py)) & 7) == 0) {
		dy = vis_ld_d64_nf(dpy);
		while ((mlib_addr)dpx < (mlib_addr)dpxend) {
			d_left = dpxend - dpx;

			if (d_left > MAX_LOOP)
				d_left = MAX_LOOP;
			ds_i = ds_r = ds1_i = ds1_r = 0.0;
			for (; d_left > 0; d_left--) {
				DPROD_S8C;
				dx = dpx[1];
				dy = dpy[1];
				dpx++;
				dpy++;
			}

			ds_i = vis_fpadd32(ds_i, ds1_i);
			ds_r = vis_fpadd32(ds_r, ds1_r);
			fsum = vis_read_hi(ds_r);
			sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_lo(ds_r);
			sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_hi(ds_i);
			sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_lo(ds_i);
			sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
		}

	} else {
		off = (mlib_addr)dpx - (mlib_addr)px;
		dpy = (mlib_d64 *)vis_alignaddr(py, off);
		dy0 = vis_ld_d64_nf(dpy);
		dy1 = vis_ld_d64_nf(dpy+1);
		dy = vis_faligndata(dy0, dy1);
		while ((mlib_addr)dpx < (mlib_addr)dpxend) {
			d_left = dpxend - dpx;

			if (d_left > MAX_LOOP)
				d_left = MAX_LOOP;
			ds_i = ds_r = ds1_i = ds1_r = 0.0;
			for (; d_left > 0; d_left--) {
				DPROD_S8C;
				dpx++;
				dpy++;
				dy0 = dy1;
				dy1 = vis_ld_d64_nf(dpy+1);
				dx = vis_ld_d64_nf(dpx);
				dy = vis_faligndata(dy0, dy1);
			}

			ds_i = vis_fpadd32(ds_i, ds1_i);
			ds_r = vis_fpadd32(ds_r, ds1_r);
			fsum = vis_read_hi(ds_r);
			sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_lo(ds_r);
			sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_hi(ds_i);
			sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_lo(ds_i);
			sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
		}
	}

	if ((mlib_addr)dpx <= (mlib_addr)pxend) {
		emask = vis_edge8(dpx, pxend);
		vis_pst_8(dx, edge + 1, emask);
		dx = edge[1];
		DPROD_S8C_TAIL;
		fsum = vis_read_hi(ds_r);
		sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
		fsum = vis_read_lo(ds_r);
		sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
		fsum = vis_read_hi(ds_i);
		sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
		fsum = vis_read_lo(ds_i);
		sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
	}

	z[0] = sum_r / 256.0;
	z[1] = sum_i / 256.0;
#undef MAX_LOOP
}

/* *********************************************************** */

#define	MAX_LOOP	(255)

static void
mlib_VectorDotProd_S8C_notal(
	mlib_d64 *z,
	const void *x,
	const void *y,
	mlib_s32 n)
/* The case of odd address of vector x */
{
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y;
	mlib_d64 sum_r = 0.0, sum_i = 0.0;
	mlib_d64 *dpx, *dpy;
	mlib_d64 dx, dx0, dx1, dy, dy0, dy1;
	mlib_d64 dx_i, dy_r, dy_i;
	mlib_d64 d_iih, d_iil, d_irh, d_irl, d_rih, d_ril, d_rrh, d_rrl;
	mlib_d64 d_ih, d_il, d_rh, d_rl;
	mlib_d64 ds_r, ds_i, ds1_r, ds1_i;
	mlib_d64 fzero = vis_fzero();
	mlib_d64 done = vis_to_double_dup(0x01000100);
	mlib_f32 fsum;
	mlib_s32 d_left, d_left_a;

	dpx = (mlib_d64 *)vis_alignaddr(px, 0);
	dpy = (mlib_d64 *)((mlib_addr)py & (~7));
	dx0 = dpx[0];
	dy0 = dpy[0];
	dpx++;
	dpy++;

	d_left_a = n >> 2;

	if (((((mlib_addr)px) ^ ((mlib_addr)py)) & 7) == 0) {
		for (; d_left_a > 0; d_left_a -= MAX_LOOP) {
			d_left = d_left_a;

			if (d_left > MAX_LOOP)
				d_left = MAX_LOOP;
			ds_i = ds_r = ds1_i = ds1_r = 0.0;
			for (; d_left > 0; d_left--) {
				dx1 = dpx[0];
				dy1 = dpy[0];
				dx = vis_faligndata(dx0, dx1);
				dy = vis_faligndata(dy0, dy1);
				DPROD_S8C;
				dx0 = dx1;
				dy0 = dy1;
				dpx++;
				dpy++;
			}

			ds_i = vis_fpadd32(ds_i, ds1_i);
			ds_r = vis_fpadd32(ds_r, ds1_r);
			fsum = vis_read_hi(ds_r);
			sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_lo(ds_r);
			sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_hi(ds_i);
			sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_lo(ds_i);
			sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
		}

		if (n & 3) {
			dx1 = vis_ld_d64_nf(dpx);
			dy1 = vis_ld_d64_nf(dpy);
			dx = vis_faligndata(dx0, dx1);
			dx = vis_fand(dx, mlib_tail_mask.db[(n & 3) - 1]);
			dy = vis_faligndata(dy0, dy1);
			DPROD_S8C_TAIL;
			fsum = vis_read_hi(ds_r);
			sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_lo(ds_r);
			sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_hi(ds_i);
			sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_lo(ds_i);
			sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
		}

	} else {
		for (; d_left_a > 0; d_left_a -= MAX_LOOP) {
			d_left = d_left_a;

			if (d_left > MAX_LOOP)
				d_left = MAX_LOOP;
			ds_i = ds_r = ds1_i = ds1_r = 0.0;
			for (; d_left > 0; d_left--) {
				(void) vis_alignaddr((void *)px, 0);
				dx1 = dpx[0];
				dx = vis_faligndata(dx0, dx1);
				(void) vis_alignaddr((void *)py, 0);
				dy1 = dpy[0];
				dy = vis_faligndata(dy0, dy1);
				DPROD_S8C;
				dx0 = dx1;
				dy0 = dy1;
				dpx++;
				dpy++;
			}

			ds_i = vis_fpadd32(ds_i, ds1_i);
			ds_r = vis_fpadd32(ds_r, ds1_r);
			fsum = vis_read_hi(ds_r);
			sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_lo(ds_r);
			sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_hi(ds_i);
			sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_lo(ds_i);
			sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
		}

		if (n & 3) {
			dx1 = vis_ld_d64_nf(dpx);
			dy1 = vis_ld_d64_nf(dpy);
			(void) vis_alignaddr((void *)px, 0);
			dx = vis_faligndata(dx0, dx1);
			dx = vis_fand(dx, mlib_tail_mask.db[(n & 3) - 1]);
			(void) vis_alignaddr((void *)py, 0);
			dy = vis_faligndata(dy0, dy1);
			DPROD_S8C_TAIL;
			fsum = vis_read_hi(ds_r);
			sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_lo(ds_r);
			sum_r += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_hi(ds_i);
			sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
			fsum = vis_read_lo(ds_i);
			sum_i += (mlib_d64)*((mlib_s32 *)&fsum);
		}
	}

	z[0] = sum_r / 256.0;
	z[1] = sum_i / 256.0;
#undef MAX_LOOP
}

/* *********************************************************** */
