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

#pragma ident	"@(#)mlib_v_VectorNorm.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorNorm_U8_Sat  - unsigned 8-bit format vector norm
 *      mlib_VectorNorm_S8_Sat  - signed   8-bit format vector norm
 *      mlib_VectorNorm_S16_Sat - 16-bit format vector norm
 *      mlib_VectorNorm_S32_Sat - 32-bit format vector norm
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorNorm_S16_Sat(mlib_d64       *z,
 *                                          const mlib_s16 *x,
 *                                          mlib_s32       n);
 *      mlib_status mlib_VectorNorm_S32_Sat(mlib_d64       *z,
 *                                          const mlib_s32 *x,
 *                                          mlib_s32       n);
 *      mlib_status mlib_VectorNorm_S8_Sat(mlib_d64      *z,
 *                                         const mlib_s8 *x,
 *                                         mlib_s32      n);
 *      mlib_status mlib_VectorNorm_U8_Sat(mlib_d64      *z,
 *                                         const mlib_u8 *x,
 *                                         mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the norm of the vector
 *      x    pointer to the first element of the input vector
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *                 n
 *      z = sqrt( SUM(x[i] * x[i]) )
 *                i = 1
 */

#include <mlib_algebra.h>
#include <mlib_AlgebraUtil.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorNorm_S32_Sat = __mlib_VectorNorm_S32_Sat
#pragma weak mlib_VectorNorm_S16_Sat = __mlib_VectorNorm_S16_Sat
#pragma weak mlib_VectorNorm_S8_Sat = __mlib_VectorNorm_S8_Sat
#pragma weak mlib_VectorNorm_U8_Sat = __mlib_VectorNorm_U8_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorNorm_S32_Sat) mlib_VectorNorm_S32_Sat
	__attribute__((weak, alias("__mlib_VectorNorm_S32_Sat")));
__typeof__(__mlib_VectorNorm_S16_Sat) mlib_VectorNorm_S16_Sat
	__attribute__((weak, alias("__mlib_VectorNorm_S16_Sat")));
__typeof__(__mlib_VectorNorm_S8_Sat) mlib_VectorNorm_S8_Sat
	__attribute__((weak, alias("__mlib_VectorNorm_S8_Sat")));
__typeof__(__mlib_VectorNorm_U8_Sat) mlib_VectorNorm_U8_Sat
	__attribute__((weak, alias("__mlib_VectorNorm_U8_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	NORM_U8                                                      \
	dr1 = vis_fmul8x16al(vis_read_hi(dx), fone);                 \
	dr2 = vis_fpmerge(fzero, vis_read_lo(dx));                   \
	dr3 = vis_fmuld8ulx16(vis_read_hi(dr1), vis_read_hi(dr1));   \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dr1), vis_read_lo(dr1));   \
	dr5 = vis_fmuld8ulx16(vis_read_hi(dr2), vis_read_hi(dr2));   \
	dr6 = vis_fmuld8ulx16(vis_read_lo(dr2), vis_read_lo(dr2))

/* *********************************************************** */

#define	SUM_U8                                                  \
	ds = vis_fpadd32(ds, dr3);                              \
	ds = vis_fpadd32(ds, dr4);                              \
	ds1 = vis_fpadd32(ds1, dr5);                            \
	ds1 = vis_fpadd32(ds1, dr6)

#define	MAX_LOOP	((MLIB_U16_MAX + 1)/16)

/* *********************************************************** */

mlib_status
__mlib_VectorNorm_U8_Sat(
	mlib_d64 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	mlib_u8 *pxend, *px = (mlib_u8 *)x;
	mlib_d64 *dpx, *dpxend;
	mlib_d64 sum = 0.0;
	mlib_d64 dx, dr1, dr2, dr3, dr4, dr5, dr6;
	mlib_d64 ds, ds1;
	mlib_d64 edge[2];
	mlib_f32 fone = vis_to_float(0x100);
	mlib_f32 fzero = vis_fzeros();
	mlib_f32 fsum;
	mlib_s32 d_left;
	mlib_s32 emask;

	if (n <= 0)
		return (MLIB_FAILURE);

	edge[0] = edge[1] = 0;

	dpx = (mlib_d64 *)((mlib_addr)px & (~7));
	pxend = px + n - 1;
	dpxend = (mlib_d64 *)((mlib_addr)pxend & (~7));
	emask = vis_edge8(px, pxend);
	vis_pst_8(dpx[0], edge, emask);
	dx = edge[0];
	while ((mlib_addr)dpx < (mlib_addr)dpxend) {
		d_left = dpxend - dpx;

		if (d_left > MAX_LOOP)
			d_left = MAX_LOOP;
		ds = ds1 = 0.0;
		for (; d_left > 0; d_left--) {
			NORM_U8;
			SUM_U8;
			dpx++;
			dx = dpx[0];
		}

		ds = vis_fpadd32(ds, ds1);
		fsum = vis_fpadd32s(vis_read_hi(ds), vis_read_lo(ds));
		sum += (mlib_d64)*((mlib_s32 *)&fsum);
	}

	if ((mlib_addr)dpx <= (mlib_addr)pxend) {
		emask = vis_edge8(dpx, pxend);
		vis_pst_8(dx, edge + 1, emask);
		dx = edge[1];
		NORM_U8;
		ds = vis_fpadd32(dr3, dr4);
		ds1 = vis_fpadd32(dr5, dr6);
		ds = vis_fpadd32(ds, ds1);
		fsum = vis_fpadd32s(vis_read_hi(ds), vis_read_lo(ds));
		sum += (mlib_d64)*((mlib_s32 *)&fsum);
	}

	z[0] = mlib_sqrt(sum);
	return (MLIB_SUCCESS);
#undef MAX_LOOP
}

/* *********************************************************** */

#define	NORM_S8                                                      \
	dr1 = vis_fpmerge(vis_read_hi(dx), fzero);                   \
	dr2 = vis_fpmerge(vis_read_lo(dx), fzero);                   \
	dr3 = vis_fmul8x16(f4ones, dr1);                             \
	dr4 = vis_fmul8x16(f4ones, dr2);                             \
	dr5 = vis_fmuld8sux16(vis_read_hi(dr1), vis_read_hi(dr3));   \
	dr6 = vis_fmuld8sux16(vis_read_lo(dr1), vis_read_lo(dr3));   \
	dr7 = vis_fmuld8sux16(vis_read_hi(dr2), vis_read_hi(dr4));   \
	dr8 = vis_fmuld8sux16(vis_read_lo(dr2), vis_read_lo(dr4))

/* *********************************************************** */

#define	SUM_S8                                                  \
	ds1 = vis_fpadd32(ds1, dr5);                            \
	ds1 = vis_fpadd32(ds1, dr6);                            \
	ds2 = vis_fpadd32(ds2, dr7);                            \
	ds2 = vis_fpadd32(ds2, dr8)

/* *********************************************************** */

#define	MAX_LOOP	255

/* *********************************************************** */

mlib_status
__mlib_VectorNorm_S8_Sat(
	mlib_d64 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	mlib_s8 *pxend, *px = (mlib_s8 *)x;
	mlib_d64 *dpx, *dpxend;
	mlib_d64 sum = 0.0;
	mlib_d64 dx, dr1, dr2, dr3, dr4, dr5, dr6, dr7, dr8;
	mlib_d64 ds1, ds2;
	mlib_d64 edge[2];
	mlib_d64 fzero = vis_fzero();
	mlib_f32 f4ones = vis_to_float(0x01010101);
	mlib_f32 fsum;
	mlib_s32 d_left;
	mlib_s32 emask;

	if (n <= 0)
		return (MLIB_FAILURE);

	edge[0] = edge[1] = 0;
	dpx = (mlib_d64 *)((mlib_addr)px & (~7));
	pxend = px + n - 1;
	dpxend = (mlib_d64 *)((mlib_addr)pxend & (~7));
	emask = vis_edge8(px, pxend);
	vis_pst_8(dpx[0], edge, emask);
	dx = edge[0];

	while ((mlib_addr)dpx < (mlib_addr)dpxend) {
		d_left = dpxend - dpx;

		if (d_left > MAX_LOOP)
			d_left = MAX_LOOP;
		ds1 = ds2 = 0.0;
		for (; d_left > 0; d_left--) {
			NORM_S8;
			SUM_S8;
			dpx++;
			dx = dpx[0];
		}

		fsum = vis_read_hi(ds1);
		sum += (mlib_d64)*((mlib_s32 *)&fsum);
		fsum = vis_read_lo(ds1);
		sum += (mlib_d64)*((mlib_s32 *)&fsum);
		fsum = vis_read_hi(ds2);
		sum += (mlib_d64)*((mlib_s32 *)&fsum);
		fsum = vis_read_lo(ds2);
		sum += (mlib_d64)*((mlib_s32 *)&fsum);
	}

	if ((mlib_addr)dpx <= (mlib_addr)pxend) {
		emask = vis_edge8(dpx, pxend);
		vis_pst_8(dx, edge + 1, emask);
		dx = edge[1];
		NORM_S8;
		ds1 = vis_fpadd32(dr5, dr6);
		ds2 = vis_fpadd32(dr7, dr8);
		fsum = vis_read_hi(ds1);
		sum += (mlib_d64)*((mlib_s32 *)&fsum);
		fsum = vis_read_lo(ds1);
		sum += (mlib_d64)*((mlib_s32 *)&fsum);
		fsum = vis_read_hi(ds2);
		sum += (mlib_d64)*((mlib_s32 *)&fsum);
		fsum = vis_read_lo(ds2);
		sum += (mlib_d64)*((mlib_s32 *)&fsum);
	}

	z[0] = mlib_sqrt(sum / 256.0);
	return (MLIB_SUCCESS);
#undef MAX_LOOP
}

/* *********************************************************** */

#define	NORM16                                                            \
	{                                                                 \
	    mlib_d64 dr2, dr3;                                            \
	                                                                  \
	    dr.db = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dx));    \
	    dr1.db = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dx));   \
	    dr.db = vis_fpadd32(dr.db, dr1.db);                           \
	    dr2 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dx));      \
	    dr3 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dx));      \
	    dr1.db = vis_fpadd32(dr2, dr3);                               \
	    ds1 = ds1 + (mlib_d64)dr.two_int.int0 +                       \
		    (mlib_d64)dr.two_int.int1;                            \
	    ds = ds + (mlib_d64)dr1.two_int.int0 +                        \
		    (mlib_d64)dr1.two_int.int1;                           \
	}

/* *********************************************************** */

mlib_status
__mlib_VectorNorm_S16_Sat(
	mlib_d64 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	mlib_s16 *px = (mlib_s16 *)x;
	mlib_s16 *pxend;
	mlib_d64 *dpx, *dpxend;
	mlib_d64 dx, ds, ds1;
	mlib_d64 edge[2];
	type_union_mlib_d64 dr, dr1;
	mlib_s32 d_left;
	mlib_u8 emask;

	edge[0] = edge[1] = 0;

	if (n <= 0)
		return (MLIB_FAILURE);

	ds = ds1 = 0;
	dpx = (mlib_d64 *)((mlib_addr)px & (~7));
	pxend = px + n - 1;

	emask = vis_edge16(px, pxend);
	vis_pst_16(dpx[0], edge, emask);
	dx = edge[0];

	dpxend = (mlib_d64 *)((mlib_addr)pxend & (~7));
	d_left = dpxend - dpx;

	for (; d_left > 0; d_left--) {
		NORM16;
		dpx++;
		dx = dpx[0];
	}

	if ((mlib_addr)dpx <= (mlib_addr)pxend) {
		emask = vis_edge16(dpx, pxend);
		vis_pst_16(dx, edge + 1, emask);
		dx = edge[1];
		NORM16;
	}

	z[0] = mlib_sqrt(ds + ds1);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorNorm_S32_Sat(
	mlib_d64 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	mlib_s32 *px = (mlib_s32 *)x;
	mlib_s32 i;
	mlib_d64 sum = 0;

	if (n <= 0)
		return (MLIB_FAILURE);
	for (i = 0; i < n; i++)
		sum += px[i] * (mlib_d64)px[i];
	z[0] = mlib_sqrt(sum);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
