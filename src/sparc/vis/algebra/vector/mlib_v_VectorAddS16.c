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

#pragma ident	"@(#)mlib_v_VectorAddS16.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorAddS_S16 - addition of signed 16-bit format vector
 *                            to a scalar
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorAddS_S16_[Sat|Mod](mlib_s16             *xz,
 *                                                const const mlib_s16 *c,
 *                                                mlib_s32             n);
 *      mlib_status mlib_VectorAddS_S16_S16_[Sat|Mod](mlib_s16             *z,
 *                                                    const const mlib_s16 *x,
 *                                                    const const mlib_s16 *c,
 *                                                    mlib_s32             n);
 *
 *      mlib_status mlib_VectorAddS_S32_S16_Mod(mlib_s32             *z,
 *                                              const const mlib_s16 *x,
 *                                              const const mlib_s16 *c,
 *                                              mlib_s32             n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      x    pointer to the first element of the first vector
 *      c    pointer to a scalar
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *      z = x + c
 */

/*
 * FUNCTIONS
 *      mlib_VectorAddS_S16C - addition of signed 16-bit format complex vector
 *                             to a scalar
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorAddS_S16C_[Sat|Mod](mlib_s16             *xz,
 *                                                 const const mlib_s16 *c,
 *                                                 mlib_s32             n);
 *      mlib_status mlib_VectorAddS_S16C_S16C_[Sat|Mod](mlib_s16             *z,
 *                                                      const const mlib_s16 *x,
 *                                                      const const mlib_s16 *c,
 *                                                      mlib_s32             n);
 *
 *      mlib_status mlib_VectorAddS_S32C_S16C_Mod(mlib_s32             *z,
 *                                                const const mlib_s16 *x,
 *                                                const const mlib_s16 *c,
 *                                                mlib_s32             n);
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
 *      z = x + c
 */

#include <mlib_algebra.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorAddS_S16C_S16C_Sat = __mlib_VectorAddS_S16C_S16C_Sat
#pragma weak mlib_VectorAddS_S32_S16_Mod = __mlib_VectorAddS_S32_S16_Mod
#pragma weak mlib_VectorAddS_S16_Sat = __mlib_VectorAddS_S16_Sat
#pragma weak mlib_VectorAddS_S16_S16_Mod = __mlib_VectorAddS_S16_S16_Mod
#pragma weak mlib_VectorAddS_S16C_Sat = __mlib_VectorAddS_S16C_Sat
#pragma weak mlib_VectorAddS_S32C_S16C_Mod = __mlib_VectorAddS_S32C_S16C_Mod
#pragma weak mlib_VectorAddS_S16C_S16C_Mod = __mlib_VectorAddS_S16C_S16C_Mod
#pragma weak mlib_VectorAddS_S16_Mod = __mlib_VectorAddS_S16_Mod
#pragma weak mlib_VectorAddS_S16_S16_Sat = __mlib_VectorAddS_S16_S16_Sat
#pragma weak mlib_VectorAddS_S16C_Mod = __mlib_VectorAddS_S16C_Mod

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorAddS_S16C_S16C_Sat) mlib_VectorAddS_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorAddS_S16C_S16C_Sat")));
__typeof__(__mlib_VectorAddS_S32_S16_Mod) mlib_VectorAddS_S32_S16_Mod
	__attribute__((weak, alias("__mlib_VectorAddS_S32_S16_Mod")));
__typeof__(__mlib_VectorAddS_S16_Sat) mlib_VectorAddS_S16_Sat
	__attribute__((weak, alias("__mlib_VectorAddS_S16_Sat")));
__typeof__(__mlib_VectorAddS_S16_S16_Mod) mlib_VectorAddS_S16_S16_Mod
	__attribute__((weak, alias("__mlib_VectorAddS_S16_S16_Mod")));
__typeof__(__mlib_VectorAddS_S16C_Sat) mlib_VectorAddS_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorAddS_S16C_Sat")));
__typeof__(__mlib_VectorAddS_S32C_S16C_Mod) mlib_VectorAddS_S32C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorAddS_S32C_S16C_Mod")));
__typeof__(__mlib_VectorAddS_S16C_S16C_Mod) mlib_VectorAddS_S16C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorAddS_S16C_S16C_Mod")));
__typeof__(__mlib_VectorAddS_S16_Mod) mlib_VectorAddS_S16_Mod
	__attribute__((weak, alias("__mlib_VectorAddS_S16_Mod")));
__typeof__(__mlib_VectorAddS_S16_S16_Sat) mlib_VectorAddS_S16_S16_Sat
	__attribute__((weak, alias("__mlib_VectorAddS_S16_S16_Sat")));
__typeof__(__mlib_VectorAddS_S16C_Mod) mlib_VectorAddS_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorAddS_S16C_Mod")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	ADDS16_SAT                                              \
	dz = vis_fpadd16(dx, dc);                               \
	dr0 = vis_fxor(dx, dc);                                 \
	dr1 = vis_fxor(dx, dz);                                 \
	dr2 = vis_fandnot(dr0, dr1);                            \
	vis_pst_16(dz, dpz, emask);                             \
	mask1 = vis_fcmplt16(dr2, fzero);                       \
	mask2 = vis_fcmpge16(dz, fzero);                        \
	mask1 &= emask;                                         \
	und = mask1 & mask2;                                    \
	ovl = mask1 & ~mask2;                                   \
	vis_pst_16(const_ovl, dpz, ovl);                        \
	vis_pst_16(const_und, dpz, und)

/* *********************************************************** */

#define	ADDS16_MOD                                              \
	dz = vis_fpadd16(dx, dc);                               \
	vis_pst_16(dz, dpz, emask)

/* *********************************************************** */

#define	ADDS_S32_S16_MOD                                        \
	dr1 = vis_fmuld8ulx16(fone, vis_read_hi(dx));           \
	dr2 = vis_fmuld8ulx16(fone, vis_read_lo(dx));           \
	dzh = vis_fpadd32(dr1, dc);                             \
	dzl = vis_fpadd32(dr2, dc)

/* *********************************************************** */

#define	ADDS_S16C_S32C                                          \
	dr1 = vis_fmuld8ulx16(fone, vis_read_hi(dx));           \
	dr2 = vis_fmuld8ulx16(fone, vis_read_lo(dx));           \
	dzh = vis_fpadd32(dr1, dc);                             \
	dzl = vis_fpadd32(dr2, dc)

/* *********************************************************** */

#define	ADDS_S32C_S16C_WITH_ODD                                 \
	dr1 = vis_fmuld8ulx16(fone, vis_read_hi(dx));           \
	dr2 = vis_fmuld8ulx16(fone, vis_read_lo(dx));           \
	dzh = vis_fpadd32(dr1, dc_odd);                         \
	dzl = vis_fpadd32(dr2, dc_odd)

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx;
	mlib_d64 dx, dz, dx0, dx1, dr0, dr1, dr2;
	mlib_s16 *pz, *px, *pzend;

/* offset of address alignment in destination */
	mlib_s32 off;

/* edge masks */
	mlib_s32 emask;
	mlib_s32 mask1, mask2;
	mlib_s32 ovl, und;
	mlib_u16 uc = *((mlib_s16 *)c);

/* prepare the scaling factors */
	mlib_d64 dc = vis_to_double_dup((uc << 16) | uc);
	mlib_d64 fzero = vis_fzero();
	mlib_d64 const_ovl = vis_to_double_dup(0x7fff7fff);
	mlib_d64 const_und = vis_fnot(const_ovl);
	mlib_s32 len = n, i;

/* rest and leng in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s16 *)x;
	pz = (mlib_s16 *)z;

	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z;
	pzend = pz + n - 1;
/*
 * generate edge mask for the start point
 */
	emask = vis_edge16(pz, pzend);

/*
 * prepare the destination address
 */

	if (off) {
		dpx = (mlib_d64 *)vis_alignaddr(px, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
		ADDS16_SAT;

		px += (8 + off) >> 1;
		len -= (8 + off) >> 1;
		dpz++;
	}

	if (len <= 0)
		return (MLIB_SUCCESS);

	even_8 = len >> 2;
	rest_8 = len & 0x3;
	emask = 0xf;

/*
 * Now try to analyze source "x" and "y" addresses.
 */

	if (!((mlib_addr)px & 7)) {

		dpx = (mlib_d64 *)px;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = (*dpx++);
			ADDS16_SAT;
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
			ADDS16_SAT;
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

	emask = ~(vis_edge16((void *)(rest_8 << 1), pzend));

	vis_alignaddr(px, 0);
	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx);
	dx = vis_faligndata(dx0, dx1);

	ADDS16_SAT;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx;
	mlib_d64 dx, dz, dx0, dx1;
	mlib_s16 *pz, *px, *pzend;

/* offset of address alignment in destination */
	mlib_s32 off;

/* edge masks */
	mlib_s32 emask;
	mlib_s32 len = n, i;

/* rest and leng in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_u16 uc = *((mlib_s16 *)c);

/* prepare the scaling factors */
	mlib_d64 dc = vis_to_double_dup((uc << 16) | uc);

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s16 *)x;
	pz = (mlib_s16 *)z;

	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z;
	pzend = pz + n - 1;
/*
 * generate edge mask for the start point
 */
	emask = vis_edge16(pz, pzend);

/*
 * prepare the destination address
 */

	if (off) {
		dpx = (mlib_d64 *)vis_alignaddr(px, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
		ADDS16_MOD;
		px += (8 + off) >> 1;
		len -= (8 + off) >> 1;
		dpz++;
	}

	if (len <= 0)
		return (MLIB_SUCCESS);

	even_8 = len >> 2;
	rest_8 = len & 0x3;
	emask = 0xf;

/*
 * Now try to analyze source "x" and "y" addresses.
 */

	if (!((mlib_addr)px & 7)) {

		dpx = (mlib_d64 *)px;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = (*dpx++);
			ADDS16_MOD;
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
			ADDS16_MOD;
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

	emask = ~(vis_edge16((void *)(rest_8 << 1), pzend));

	vis_alignaddr(px, 0);
	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx);
	dx = vis_faligndata(dx0, dx1);

	ADDS16_MOD;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_S32_S16_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx;
	mlib_d64 dx, dx0, dx1;
	mlib_d64 dr1, dr2, dzh, dzl;
	mlib_f32 fone = vis_to_float(0x10001);
	mlib_s32 uc = *((mlib_s16 *)c);
	mlib_s16 *px;
	mlib_s32 *pz;
	mlib_s32 len = n, i;

/* rest and leng in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_d64 dc = vis_to_double_dup(uc);

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!uc)
		return (__mlib_VectorConvert_S32_S16_Mod(z, x, n));

	px = (mlib_s16 *)x;
	pz = (mlib_s32 *)z;

	if (n <= 4) {
		for (i = 0; i < n; i++)
			(*pz++) = (*px++) + uc;
		return (MLIB_SUCCESS);
	}

/*
 * prepare the destination address
 */

	while ((mlib_addr)pz & 7) {
		(*pz++) = ((mlib_s32)(*px)) + uc;
		px++;
		len--;
	}

	dpz = (mlib_d64 *)pz;

	even_8 = len >> 2;
	rest_8 = len & 0x3;

	if (!((mlib_addr)px & 7)) {

/*
 * 'x' address is 8-byte aligned.
 * No  vis_alignaddr and  vis_faligndata at all.
 */

		dpx = (mlib_d64 *)px;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = (*dpx++);
			ADDS_S32_S16_MOD;

/*
 * store 16 bytes of result
 */
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
		}

	} else {

/*
 * "x"  address is arbitrary aligned.
 * 1 vis_alignaddr and 1 vis_faligndata in the loop.
 */

		dpx = vis_alignaddr(px, 0);
		dx0 = vis_ld_d64_nf(dpx);
		dpx++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx);
			dpx++;
			dx = vis_faligndata(dx0, dx1);
			ADDS_S32_S16_MOD;
			dx0 = dx1;
/*
 * store 16 bytes of result
 */
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
		}
	}

	if (!rest_8)
		return (MLIB_SUCCESS);

	px += (even_8 << 2);
	pz += (even_8 << 2);

	while (rest_8--) {
		(*pz++) = ((mlib_s32)(*px)) + uc;
		px++;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_S16_Sat(
	mlib_s16 *xz,
	const mlib_s16 *c,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S16_S16_Sat(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_S16_Mod(
	mlib_s16 *xz,
	const mlib_s16 *c,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S16_S16_Mod(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx;
	mlib_d64 dx, dz, dx0, dx1, dr0, dr1, dr2;
	mlib_s16 *pz, *px, *pzend;

/* offset of address alignment in destination */
	mlib_s32 off;

/* edge masks */
	mlib_s32 emask;
	mlib_s32 mask1, mask2;
	mlib_s32 ovl, und;
	mlib_u16 uc0 = *((mlib_s16 *)c);
	mlib_u16 uc1 = *((mlib_s16 *)c + 1);
	mlib_d64 dc = ((mlib_addr)z & 2) ? vis_to_double_dup((uc1 << 16) | uc0)
		: vis_to_double_dup((uc0 << 16) | uc1);
	mlib_d64 fzero = vis_fzero();
	mlib_d64 const_ovl = vis_to_double_dup(0x7fff7fff);
	mlib_d64 const_und = vis_fnot(const_ovl);
	mlib_s32 len = n + n, i;

/* rest and leng in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s16 *)x;
	pz = (mlib_s16 *)z;

	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z;
	pzend = pz + n + n - 1;
/*
 * generate edge mask for the start point
 */
	emask = vis_edge16(pz, pzend);

/*
 * prepare the destination address
 */

	if (off) {
		dpx = (mlib_d64 *)vis_alignaddr(px, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
		ADDS16_SAT;

		px += (8 + off) >> 1;
		len -= (8 + off) >> 1;
		dpz++;
	}

	if (len <= 0)
		return (MLIB_SUCCESS);

	even_8 = len >> 2;
	rest_8 = len & 0x3;
	emask = 0xf;

/*
 * Now try to analyze source "x" and "y" addresses.
 */

	if (!((mlib_addr)px & 7)) {

		dpx = (mlib_d64 *)px;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = (*dpx++);
			ADDS16_SAT;
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
			ADDS16_SAT;
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

	emask = ~(vis_edge16((void *)(rest_8 << 1), pzend));

	vis_alignaddr(px, 0);
	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx);
	dx = vis_faligndata(dx0, dx1);

	ADDS16_SAT;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx;
	mlib_d64 dx, dz, dx0, dx1;
	mlib_s16 *pz, *px, *pzend;

/* offset of address alignment in destination */
	mlib_s32 off;

/* edge masks */
	mlib_s32 emask;
	mlib_s32 len = n + n, i;

/* rest and leng in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_u16 uc0 = *((mlib_s16 *)c);
	mlib_u16 uc1 = *((mlib_s16 *)c + 1);
	mlib_d64 dc = ((mlib_addr)z & 2) ? vis_to_double_dup((uc1 << 16) | uc0)
		: vis_to_double_dup((uc0 << 16) | uc1);

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s16 *)x;
	pz = (mlib_s16 *)z;

	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z;
	pzend = pz + n + n - 1;
/*
 * generate edge mask for the start point
 */
	emask = vis_edge16(pz, pzend);

/*
 * prepare the destination address
 */

	if (off) {
		dpx = (mlib_d64 *)vis_alignaddr(px, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
		ADDS16_MOD;
		px += (8 + off) >> 1;
		len -= (8 + off) >> 1;
		dpz++;
	}

	if (len <= 0)
		return (MLIB_SUCCESS);

	even_8 = len >> 2;
	rest_8 = len & 0x3;
	emask = 0xf;

/*
 * Now try to analyze source "x" and "y" addresses.
 */

	if (!((mlib_addr)px & 7)) {

		dpx = (mlib_d64 *)px;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = (*dpx++);
			ADDS16_MOD;
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
			ADDS16_MOD;
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

	emask = ~(vis_edge16((void *)(rest_8 << 1), pzend));

	vis_alignaddr(px, 0);
	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx);
	dx = vis_faligndata(dx0, dx1);

	ADDS16_MOD;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx;
	mlib_d64 dx, dx0, dx1;
	mlib_d64 dzh, dzl;
	mlib_f32 fone = vis_to_float(0x10001);
	mlib_s16 uc = *((mlib_s16 *)c);
	mlib_s16 uc1 = *((mlib_s16 *)c + 1);
	mlib_s16 *px;
	mlib_s32 *pz;
	mlib_s32 len = n << 1, i;

/* rest and leng in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;

/*
 * Prepare scale == mlib_d64 (Re_Im)
 */
	mlib_d64 dc = vis_to_double(uc, uc1);

/*
 * Prepare scale == mlib_d64 (Im_Re)
 */
	mlib_d64 dc_odd = vis_to_double(uc1, uc);
	mlib_d64 dr1, dr2;
	mlib_s32 odd = 0;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!(uc | uc1))
		return (__mlib_VectorConvert_S32C_S16C_Mod(z, x, n));

	px = (mlib_s16 *)x;
	pz = (mlib_s32 *)z;

	if (len <= 4) {
		for (i = 0; i < n; i++) {
			(*pz++) = (*px++) + (mlib_s32)uc;
			(*pz++) = (*px++) + (mlib_s32)uc1;
		}

		return (MLIB_SUCCESS);
	}

/*
 * prepare the destination address
 */

	while ((mlib_addr)pz & 7) {
		(*pz++) = ((mlib_s32)(*px)) + uc;
		px++;
		len -= 2;

		if (!((mlib_addr)pz & 7)) {
			odd = 1;
			break;
		}
	}

	dpz = (mlib_d64 *)pz;

	even_8 = len >> 2;
	rest_8 = len & 0x3;

	if (!odd) {

/*
 * Start with real part.
 */

		if (!((mlib_addr)px & 7)) {

/*
 * 'x' address is 8-byte aligned.
 * No  vis_alignaddr and  vis_faligndata at all.
 */

			dpx = (mlib_d64 *)px;
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = (*dpx++);
				ADDS_S16C_S32C;

/*
 * store 16 bytes of result
 */
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
			}

		} else {

/*
 * "x"  address is arbitrary aligned.
 * 1 vis_alignaddr and 1 vis_faligndata in the loop.
 */

			dpx = vis_alignaddr(px, 0);
			dx0 = vis_ld_d64_nf(dpx);
			dpx++;

#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
				dpx++;
				dx = vis_faligndata(dx0, dx1);
				ADDS_S16C_S32C;
				vis_alignaddr(px, 0);
				dx0 = dx1;
/*
 * store 16 bytes of result
 */
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
			}
		}

		px += (even_8 << 2);
		pz = (mlib_s32 *)dpz;

	} else {

/*
 * odd==1. Start with imaginary part.
 */

		if (!((mlib_addr)px & 7)) {

/*
 * 'x' address is 8-byte aligned.
 * No  vis_alignaddr and  vis_faligndata at all.
 */

			dpx = (mlib_d64 *)px;

#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = (*dpx++);
				ADDS_S32C_S16C_WITH_ODD;

/*
 * store 16 bytes of result
 */
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
			}

		} else {

/*
 * "x"  address is arbitrary aligned.
 * 1 vis_alignaddr and 1 vis_faligndata in the loop.
 */

			dpx = vis_alignaddr(px, 0);
			dx0 = vis_ld_d64_nf(dpx);
			dpx++;

#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
				dpx++;
				dx = vis_faligndata(dx0, dx1);
				ADDS_S32C_S16C_WITH_ODD;
				dx0 = dx1;
/*
 * store 16 bytes of result
 */
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
			}
		}

		px += (even_8 << 2);
		pz = (mlib_s32 *)dpz;
		(*pz++) = *px + uc1;
		px++;
	}

	if (!rest_8)
		return (MLIB_SUCCESS);

	while (rest_8) {
		(*pz++) = ((mlib_s32)(*px)) + uc;
		px++;
		(*pz++) = ((mlib_s32)(*px)) + uc1;
		px++;
		rest_8 -= 2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_S16C_Sat(
	mlib_s16 *xz,
	const mlib_s16 *c,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S16C_S16C_Sat(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_S16C_Mod(
	mlib_s16 *xz,
	const mlib_s16 *c,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S16C_S16C_Mod(xz, xz, c, n));
}

/* *********************************************************** */
