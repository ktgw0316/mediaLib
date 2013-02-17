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

#pragma ident	"@(#)mlib_v_VectorSubS32.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorSubS_S32 - subtraction of signed 32-bit format vector
 *                           from a scalar
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorSubS_S32C_[Sat|Mod](mlib_s32       *xz,
 *                                                 const mlib_s32 *c,
 *                                                 mlib_s32       n);
 *      mlib_status mlib_VectorSubS_S32C_S32C_[Sat|Mod](mlib_s32       *z,
 *                                                      const mlib_s32 *x,
 *                                                      const mlib_s32 *c,
 *                                                      mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      x    pointer to the first element of the first vector
 *      c    pointer to a scalar
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *      z = c - x
 */

/*
 * FUNCTIONS
 *      mlib_VectorSubS_S32C - subtraction of signed 32-bit format
 *                            complex vector from a scalar
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorSubS_S32_[Sat|Mod](mlib_s32       *xz,
 *                                                const mlib_s32 *c,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorSubS_S32_S32_[Sat|Mod](mlib_s32       *z,
 *                                                    const mlib_s32 *x,
 *                                                    const mlib_s32 *c,
 *                                                    mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result vector.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts
 *      x    pointer to the first complex element of the first vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      c    pointer to a complex scalar. c[0] hold the real part,
 *           and c[1] hold the imaginary part
 *      n    number of complex elements in the vectors
 *
 * DESCRIPTION
 *      z = c - x
 */

#include <mlib_algebra.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorSubS_S32_S32_Mod = __mlib_VectorSubS_S32_S32_Mod
#pragma weak mlib_VectorSubS_S32_S32_Sat = __mlib_VectorSubS_S32_S32_Sat
#pragma weak mlib_VectorSubS_S32_Mod = __mlib_VectorSubS_S32_Mod
#pragma weak mlib_VectorSubS_S32_Sat = __mlib_VectorSubS_S32_Sat
#pragma weak mlib_VectorSubS_S32C_Mod = __mlib_VectorSubS_S32C_Mod
#pragma weak mlib_VectorSubS_S32C_Sat = __mlib_VectorSubS_S32C_Sat
#pragma weak mlib_VectorSubS_S32C_S32C_Mod = __mlib_VectorSubS_S32C_S32C_Mod
#pragma weak mlib_VectorSubS_S32C_S32C_Sat = __mlib_VectorSubS_S32C_S32C_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorSubS_S32_S32_Mod) mlib_VectorSubS_S32_S32_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S32_S32_Mod")));
__typeof__(__mlib_VectorSubS_S32_S32_Sat) mlib_VectorSubS_S32_S32_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S32_S32_Sat")));
__typeof__(__mlib_VectorSubS_S32_Mod) mlib_VectorSubS_S32_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S32_Mod")));
__typeof__(__mlib_VectorSubS_S32_Sat) mlib_VectorSubS_S32_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S32_Sat")));
__typeof__(__mlib_VectorSubS_S32C_Mod) mlib_VectorSubS_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S32C_Mod")));
__typeof__(__mlib_VectorSubS_S32C_Sat) mlib_VectorSubS_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S32C_Sat")));
__typeof__(__mlib_VectorSubS_S32C_S32C_Mod) mlib_VectorSubS_S32C_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S32C_S32C_Mod")));
__typeof__(__mlib_VectorSubS_S32C_S32C_Sat) mlib_VectorSubS_S32C_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S32C_S32C_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SUBS32_MOD                                              \
	dz = vis_fpsub32(dc, dx);                               \
	vis_pst_32(dz, dpz, emask)

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

#else /* MLIB_USE_FTOI_CLAMPING */

#define	FLOAT2INT_CLAMP(DST, SRC)	DST = ((mlib_s32)(SRC))

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S32_S32_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 diff;
	mlib_s32 *px = (mlib_s32 *)x, *pz = (mlib_s32 *)z;
	mlib_s32 cc = *((mlib_s32 *)c);

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		diff = cc - (mlib_d64)px[i];
		FLOAT2INT_CLAMP(pz[i], diff);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S32_S32_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx;
	mlib_d64 dx, dz, dx0, dx1;
	mlib_s32 *pz, *px, *pzend;

/* offset of address alignment in destination */
	mlib_s32 off;

/* edge masks */
	mlib_s32 emask;
	mlib_s32 len = n, i;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_u32 uc = *((mlib_s32 *)c);

/* prepare the scaling factors */
	mlib_d64 dc = vis_to_double_dup(uc);

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s32 *)x;
	pz = (mlib_s32 *)z;

	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z;
	pzend = pz + n - 1;
/*
 * generate edge mask for the start point
 */
	emask = vis_edge32(pz, pzend);

	if (((mlib_addr)pz & (~7)) == ((mlib_addr)pzend & (~7))) {
		dpx = (mlib_d64 *)vis_alignaddr(px, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
		SUBS32_MOD;
		return (MLIB_SUCCESS);
	}

/*
 * prepare the destination address
 */

	if (off) {
		dpx = (mlib_d64 *)vis_alignaddr(px, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
		SUBS32_MOD;

		px += (8 + off) >> 2;
		len -= (8 + off) >> 2;
		dpz++;
	}

	even_8 = len >> 1;
	rest_8 = len & 0x1;
	emask = 0xf;

/*
 * Now try to analyze source "x" and "y" addresses.  **
 */

	if (!((mlib_addr)px & 7)) {

/*
 * First ("x") address is 8-byte aligned. vis_alignaddr
 * and vis_faligndata only for "y".
 */

		dpx = (mlib_d64 *)px;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = (*dpx++);
			SUBS32_MOD;
			dpz++;
		}

		dx1 = vis_ld_d64_nf(dpx);
		dpx++;
	} else {

		dpx = vis_alignaddr(px, 0);
		dx0 = vis_ld_d64_nf(dpx);
		dpx++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx);
			dpx++;
			dx = vis_faligndata(dx0, dx1);
			SUBS32_MOD;
			dx0 = dx1;
			dpz++;
		}

		dx1 = dx0;
	}

	if (!rest_8)
		return (MLIB_SUCCESS);

/*
 * prepare edge mask for the last bytes
 */

	emask = ~(vis_edge32((void *)(rest_8 << 2), pzend));

	vis_alignaddr(px, 0);
	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx);
	dx = vis_faligndata(dx0, dx1);
	SUBS32_MOD;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S32_Sat(
	mlib_s32 *xz,
	const mlib_s32 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S32_S32_Sat(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S32_Mod(
	mlib_s32 *xz,
	const mlib_s32 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S32_S32_Mod(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S32C_S32C_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 diff_r, diff_i;
	mlib_s32 *px = (mlib_s32 *)x, *pz = (mlib_s32 *)z;
	mlib_s32 c0 = ((mlib_s32 *)c)[0], c1 = ((mlib_s32 *)c)[1];

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		diff_r = c0 - (mlib_d64)px[2 * i];
		diff_i = c1 - (mlib_d64)px[2 * i + 1];
		FLOAT2INT_CLAMP(pz[2 * i], diff_r);
		FLOAT2INT_CLAMP(pz[2 * i + 1], diff_i);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S32C_S32C_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx;
	mlib_d64 dx, dz, dx0, dx1;
	mlib_s32 *pz, *px, *pzend;

/* offset of address alignment in destination */
	mlib_s32 off;

/* edge masks */
	mlib_s32 emask;
	mlib_s32 len = n + n, i;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_u32 uc0 = *((mlib_s32 *)c);
	mlib_u32 uc1 = *((mlib_s32 *)c + 1);
	mlib_f32 fc0 = vis_to_float(uc0);
	mlib_f32 fc1 = vis_to_float(uc1);
	mlib_d64 dc =
		((mlib_addr)z & 4) ? vis_freg_pair(fc1,
		fc0) : vis_freg_pair(fc0, fc1);

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s32 *)x;
	pz = (mlib_s32 *)z;

	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z;
	pzend = pz + 2 * n - 1;
/*
 * generate edge mask for the start point
 */
	emask = vis_edge32(pz, pzend);

	if (((mlib_addr)pz & (~7)) == ((mlib_addr)pzend & (~7))) {
		dpx = (mlib_d64 *)vis_alignaddr(px, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
		SUBS32_MOD;
		return (MLIB_SUCCESS);
	}

/*
 * prepare the destination address
 */

	if (off) {
		dpx = (mlib_d64 *)vis_alignaddr(px, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
		SUBS32_MOD;

		px += (8 + off) >> 2;
		len -= (8 + off) >> 2;
		dpz++;
	}

	even_8 = len >> 1;
	rest_8 = len & 0x1;
	emask = 0xf;

/*
 * Now try to analyze source "x" and "y" addresses.  **
 */

	if (!((mlib_addr)px & 7)) {

/*
 * First ("x") address is 8-byte aligned. vis_alignaddr
 * and vis_faligndata only for "y".
 */

		dpx = (mlib_d64 *)px;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = (*dpx++);
			SUBS32_MOD;
			dpz++;
		}

		dx1 = vis_ld_d64_nf(dpx);
		dpx++;
	} else {

		dpx = vis_alignaddr(px, 0);
		dx0 = vis_ld_d64_nf(dpx);
		dpx++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx);
			dpx++;
			dx = vis_faligndata(dx0, dx1);
			SUBS32_MOD;
			dx0 = dx1;
			dpz++;
		}

		dx1 = dx0;
	}

	if (!rest_8)
		return (MLIB_SUCCESS);

/*
 * prepare edge mask for the last bytes
 */

	emask = ~(vis_edge32((void *)(rest_8 << 2), pzend));

	vis_alignaddr(px, 0);
	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx);
	dx = vis_faligndata(dx0, dx1);
	SUBS32_MOD;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S32C_Sat(
	mlib_s32 *xz,
	const mlib_s32 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S32C_S32C_Sat(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSubS_S32C_Mod(
	mlib_s32 *xz,
	const mlib_s32 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S32C_S32C_Mod(xz, xz, c, n));
}

/* *********************************************************** */
