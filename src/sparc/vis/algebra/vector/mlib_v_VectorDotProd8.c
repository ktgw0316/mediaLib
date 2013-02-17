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

#pragma ident	"@(#)mlib_v_VectorDotProd8.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorDotProd_U8_Sat - dot product of unsigned 8-bit format vectors
 *      mlib_VectorDotProd_S8_Sat - dot product of signed 8-bit format vectors
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorDotProd_S8_Sat(mlib_d64      *z,
 *                                            const mlib_s8 *x,
 *                                            const mlib_s8 *y,
 *                                            mlib_s32      n);
 *      mlib_status mlib_VectorDotProd_U8_Sat(mlib_d64      *z,
 *                                            const mlib_u8 *x,
 *                                            const mlib_u8 *y,
 *                                            mlib_s32      n);
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
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorDotProd_S8_Sat = __mlib_VectorDotProd_S8_Sat
#pragma weak mlib_VectorDotProd_U8_Sat = __mlib_VectorDotProd_U8_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorDotProd_S8_Sat) mlib_VectorDotProd_S8_Sat
	__attribute__((weak, alias("__mlib_VectorDotProd_S8_Sat")));
__typeof__(__mlib_VectorDotProd_U8_Sat) mlib_VectorDotProd_U8_Sat
	__attribute__((weak, alias("__mlib_VectorDotProd_U8_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	DPROD_U8                                                     \
	dr5 = vis_fmul8x16al(vis_read_hi(dx), fone);                 \
	dr6 = vis_fmul8x16al(vis_read_hi(dy), fone);                 \
	dr7 = vis_fpmerge(fzero, vis_read_lo(dx));                   \
	dr8 = vis_fpmerge(fzero, vis_read_lo(dy));                   \
	dr1 = vis_fmuld8ulx16(vis_read_hi(dr5), vis_read_hi(dr6));   \
	dr2 = vis_fmuld8ulx16(vis_read_lo(dr5), vis_read_lo(dr6));   \
	dr3 = vis_fmuld8ulx16(vis_read_hi(dr7), vis_read_hi(dr8));   \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dr7), vis_read_lo(dr8))

/* *********************************************************** */

#define	SUM_U8                                                  \
	{                                                       \
	    ds = vis_fpadd32(ds, dr3);                          \
	    ds = vis_fpadd32(ds, dr4);                          \
	    ds1 = vis_fpadd32(ds1, dr1);                        \
	    ds1 = vis_fpadd32(ds1, dr2);                        \
	}

#define	MAX_LOOP	((MLIB_U16_MAX + 1)/16)

/* *********************************************************** */

mlib_status
__mlib_VectorDotProd_U8_Sat(
	mlib_d64 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_u8 *pxend, *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_d64 sum = 0.0;
	mlib_d64 *dpx, *dpy, *dpxend;
	mlib_d64 dx, dy, dy0, dy1;
	mlib_d64 ds, ds1, dr1, dr2, dr3, dr4, dr5, dr6, dr7, dr8;
	mlib_d64 edge[2];
	mlib_d64 fzero = vis_fzero();
	mlib_f32 fone = vis_to_float(0x100);
	mlib_f32 fsum;
	mlib_s32 d_left;
	mlib_s32 emask, off;

	if (n <= 0)
		return (MLIB_FAILURE);

	edge[0] = edge[1] = 0;

	dpx = (mlib_d64 *)((mlib_addr)px & (~7));
	off = (mlib_addr)dpx - (mlib_addr)px;
	dpy = (mlib_d64 *)vis_alignaddr(py, off);
	pxend = px + n - 1;
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
			ds = ds1 = 0.0;
			for (; d_left > 0; d_left--) {
				DPROD_U8;
				SUM_U8;
				dx = dpx[1];
				dy = dpy[1];
				dpx++;
				dpy++;
			}

			ds = vis_fpadd32(ds, ds1);
			fsum = vis_fpadd32s(vis_read_hi(ds), vis_read_lo(ds));
			sum += (mlib_d64)*((mlib_s32 *)&fsum);
		}
	} else {
		dy1 = vis_ld_d64_nf(dpy+1);
		dy = vis_faligndata(dy, dy1);
		dy0 = dy1;
		dy1 = vis_ld_d64_nf(dpy+2);
		while ((mlib_addr)dpx < (mlib_addr)dpxend) {
			d_left = dpxend - dpx;

			if (d_left > MAX_LOOP)
				d_left = MAX_LOOP;
			ds = ds1 = 0.0;
			for (; d_left > 0; d_left--) {
				DPROD_U8;
				SUM_U8;
				dx = dpx[1];
				dy = vis_faligndata(dy0, dy1);
				dy0 = dy1;
				dy1 = vis_ld_d64_nf(dpy+3);
				dpx++;
				dpy++;
			}

			ds = vis_fpadd32(ds, ds1);
			fsum = vis_fpadd32s(vis_read_hi(ds), vis_read_lo(ds));
			sum += (mlib_d64)*((mlib_s32 *)&fsum);
		}
	}

	if ((mlib_addr)dpx <= (mlib_addr)pxend) {
		emask = vis_edge8(dpx, pxend);
		vis_pst_8(dx, edge + 1, emask);
		dx = edge[1];
		DPROD_U8;
		ds = vis_fpadd32(dr1, dr2);
		ds1 = vis_fpadd32(dr3, dr4);
		ds = vis_fpadd32(ds, ds1);
		fsum = vis_fpadd32s(vis_read_hi(ds), vis_read_lo(ds));
		sum += (mlib_d64)*((mlib_s32 *)&fsum);
	}

	z[0] = sum;
	return (MLIB_SUCCESS);
#undef MAX_LOOP
}

/* *********************************************************** */

#define	DPROD_S8                                                \
	dr1 = vis_fmul8x16al(vis_read_hi(dx), one);             \
	dr2 = vis_fpmerge(vis_read_hi(dy), fzero);              \
	dr1 = vis_fpack32(dr1, fzero);                          \
	dr = vis_fmul8sux16(dr1, dr2);                          \
	dr2 = vis_fmuld8ulx16(fone, vis_read_hi(dr));           \
	dr3 = vis_fmuld8ulx16(fone, vis_read_lo(dr));           \
	dr4 = vis_fpmerge(vis_read_lo(dx), fzero);              \
	dr5 = vis_fpmerge(vis_read_lo(dy), fzero);              \
	dr4 = vis_fmul8sux16(dr4, dr5);                         \
	dr5 = vis_fmuld8ulx16(fone, vis_read_hi(dr4));          \
	dr4 = vis_fmuld8ulx16(fone, vis_read_lo(dr4))

/* *********************************************************** */

#define	SUM_S8                                                  \
	dr3 = vis_fpadd32(dr2, dr3);                            \
	ds = vis_fpadd32(ds, dr3);                              \
	dr5 = vis_fpadd32(dr4, dr5);                            \
	ds1 = vis_fpadd32(ds1, dr5)

#define	MAX_LOOP	((MLIB_U16_MAX + 1)/8)

/* *********************************************************** */

mlib_status
__mlib_VectorDotProd_S8_Sat(
	mlib_d64 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_s8 *pxend, *px = (mlib_s8 *)x, *py = (mlib_s8 *)y;
	mlib_d64 sum = 0.0;
	mlib_d64 *dpx, *dpy, *dpxend;
	mlib_d64 dx, dy, dy0, dy1;
	mlib_d64 ds, ds1, dr, dr1, dr2, dr3, dr4, dr5;
	mlib_d64 edge[2];
	mlib_d64 fzero = vis_fzero();
	mlib_f32 fone = vis_to_float(0x10001), one = vis_to_float(0x100);
	mlib_f32 fsum;
	mlib_s32 d_left;
	mlib_s32 emask, off;

	if (n <= 0)
		return (MLIB_FAILURE);

	edge[0] = edge[1] = 0;

	dpx = (mlib_d64 *)((mlib_addr)px & (~7));
	off = (mlib_addr)dpx - (mlib_addr)px;
	dpy = (mlib_d64 *)vis_alignaddr(py, off);
	pxend = px + n - 1;
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
			ds = ds1 = 0.0;
			for (; d_left > 0; d_left--) {
				DPROD_S8;
				SUM_S8;
				dx = dpx[1];
				dy = dpy[1];
				dpx++;
				dpy++;
			}

			ds = vis_fpadd32(ds, ds1);
			fsum = vis_fpadd32s(vis_read_hi(ds), vis_read_lo(ds));
			sum += (mlib_d64)*((mlib_s32 *)&fsum);
		}
	} else {
		dy1 = vis_ld_d64_nf(dpy+1);
		dy = vis_faligndata(dy, dy1);
		while ((mlib_addr)dpx < (mlib_addr)dpxend) {
			d_left = dpxend - dpx;

			if (d_left > MAX_LOOP)
				d_left = MAX_LOOP;
			ds = ds1 = 0.0;
			for (; d_left > 0; d_left--) {
				dpx++;
				dpy++;
				dy0 = dy1;
				dy1 = vis_ld_d64_nf(dpy+1);
				DPROD_S8;
				SUM_S8;
				dx = dpx[0];
				dy = vis_faligndata(dy0, dy1);
			}

			ds = vis_fpadd32(ds, ds1);
			fsum = vis_fpadd32s(vis_read_hi(ds), vis_read_lo(ds));
			sum += (mlib_d64)*((mlib_s32 *)&fsum);
		}
	}

	if ((mlib_addr)dpx <= (mlib_addr)pxend) {
		emask = vis_edge8(dpx, pxend);
		vis_pst_8(dx, edge + 1, emask);
		dx = edge[1];
		DPROD_S8;
		ds = vis_fpadd32(dr2, dr3);
		ds1 = vis_fpadd32(dr4, dr5);
		ds = vis_fpadd32(ds, ds1);
		fsum = vis_fpadd32s(vis_read_hi(ds), vis_read_lo(ds));
		sum += (mlib_d64)*((mlib_s32 *)&fsum);
	}

	z[0] = sum;
	return (MLIB_SUCCESS);
#undef MAX_LOOP
}

/* *********************************************************** */
