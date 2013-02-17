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

#pragma ident	"@(#)mlib_v_VectorAdd16.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorAdd_S16 - addition of signed 16-bit format vectors
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorAdd_S16_[Sat|Mod](mlib_s16       * xz,
 *                                               const mlib_s16 * y,
 *                                               mlib_s32       n);
 *      mlib_status mlib_VectorAdd_S16_S16_[Sat|Mod](mlib_s16       * z,
 *                                                   const mlib_s16 * x,
 *                                                   const mlib_s16 * y,
 *                                                   mlib_s32       n);
 *
 *      mlib_status mlib_VectorAdd_S32_S16_Mod(mlib_s32       * z,
 *                                             const mlib_s16 * x,
 *                                             const mlib_s16 * y,
 *                                             mlib_s32       n);
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      x    pointer to the first element of the first vector
 *      y    pointer to the first element of the second vector
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *      z = x + y
 */

/*
 * FUNCTIONS
 *      mlib_VectorAdd_S16C - addition of signed 16-bit format complex vectors
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorAdd_S16C_[Sat|Mod](mlib_s16       * xz,
 *                                                const mlib_s16 * y,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorAdd_S16C_S16C_[Sat|Mod](mlib_s16       * z,
 *                                                     const mlib_s16 * x,
 *                                                     const mlib_s16 * y,
 *                                                     mlib_s32       n);
 *
 *      mlib_status mlib_VectorAdd_S32C_S16C_Mod(mlib_s32       * z,
 *                                               const mlib_s16 * x,
 *                                               const mlib_s16 * y,
 *                                               mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result vector.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts
 *      x    pointer to the first complex element of the first vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      y    pointer to the first complex element of the second vector.
 *           y[2*i] hold the real parts, and y[2*i + 1] hold the imaginary
 *           parts
 *      n    number of complex elements in the vectors
 *
 * DESCRIPTION
 *      z = x + y
 */

#include <mlib_algebra.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorAdd_S16_S16_Mod = __mlib_VectorAdd_S16_S16_Mod
#pragma weak mlib_VectorAdd_S16C_Sat = __mlib_VectorAdd_S16C_Sat
#pragma weak mlib_VectorAdd_S32C_S16C_Mod = __mlib_VectorAdd_S32C_S16C_Mod
#pragma weak mlib_VectorAdd_S16C_S16C_Mod = __mlib_VectorAdd_S16C_S16C_Mod
#pragma weak mlib_VectorAdd_S16_Mod = __mlib_VectorAdd_S16_Mod
#pragma weak mlib_VectorAdd_S16_S16_Sat = __mlib_VectorAdd_S16_S16_Sat
#pragma weak mlib_VectorAdd_S16C_Mod = __mlib_VectorAdd_S16C_Mod
#pragma weak mlib_VectorAdd_S32_S16_Mod = __mlib_VectorAdd_S32_S16_Mod
#pragma weak mlib_VectorAdd_S16C_S16C_Sat = __mlib_VectorAdd_S16C_S16C_Sat
#pragma weak mlib_VectorAdd_S16_Sat = __mlib_VectorAdd_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorAdd_S16_S16_Mod) mlib_VectorAdd_S16_S16_Mod
	__attribute__((weak, alias("__mlib_VectorAdd_S16_S16_Mod")));
__typeof__(__mlib_VectorAdd_S16C_Sat) mlib_VectorAdd_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorAdd_S16C_Sat")));
__typeof__(__mlib_VectorAdd_S32C_S16C_Mod) mlib_VectorAdd_S32C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorAdd_S32C_S16C_Mod")));
__typeof__(__mlib_VectorAdd_S16C_S16C_Mod) mlib_VectorAdd_S16C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorAdd_S16C_S16C_Mod")));
__typeof__(__mlib_VectorAdd_S16_Mod) mlib_VectorAdd_S16_Mod
	__attribute__((weak, alias("__mlib_VectorAdd_S16_Mod")));
__typeof__(__mlib_VectorAdd_S16_S16_Sat) mlib_VectorAdd_S16_S16_Sat
	__attribute__((weak, alias("__mlib_VectorAdd_S16_S16_Sat")));
__typeof__(__mlib_VectorAdd_S16C_Mod) mlib_VectorAdd_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorAdd_S16C_Mod")));
__typeof__(__mlib_VectorAdd_S32_S16_Mod) mlib_VectorAdd_S32_S16_Mod
	__attribute__((weak, alias("__mlib_VectorAdd_S32_S16_Mod")));
__typeof__(__mlib_VectorAdd_S16C_S16C_Sat) mlib_VectorAdd_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorAdd_S16C_S16C_Sat")));
__typeof__(__mlib_VectorAdd_S16_Sat) mlib_VectorAdd_S16_Sat
	__attribute__((weak, alias("__mlib_VectorAdd_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	ADD16_SAT                                               \
	dz = vis_fpadd16(dx, dy);                               \
	dr0 = vis_fxor(dx, dy);                                 \
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

#define	ADD_S16_S32                                             \
	dxh = vis_fmuld8ulx16(two_16_ones, vis_read_hi(dx));    \
	dxl = vis_fmuld8ulx16(two_16_ones, vis_read_lo(dx));    \
	dyh = vis_fmuld8ulx16(two_16_ones, vis_read_hi(dy));    \
	dyl = vis_fmuld8ulx16(two_16_ones, vis_read_lo(dy));    \
	dzh = vis_fpadd32(dxh, dyh);                            \
	dzl = vis_fpadd32(dxl, dyl)

/* *********************************************************** */

#define	ADD16_MOD                                               \
	dz = vis_fpadd16(dx, dy);                               \
	vis_pst_16(dz, dpz, emask)

/* *********************************************************** */

mlib_status
__mlib_VectorAdd_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dz, dx0, dx1, dy0, dy1, dr0, dr1, dr2;
	mlib_s16 *pz, *px, *py, *pzend;

/* offset of address alignment in destination */
	mlib_s32 off;

/* edge masks */
	mlib_s32 emask;
	mlib_s32 mask1, mask2;
	mlib_s32 ovl, und;
	mlib_d64 fzero = vis_fzero();
	mlib_d64 const_ovl = vis_to_double_dup(0x7fff7fff);
	mlib_d64 const_und = vis_fnot(const_ovl);
	mlib_s32 len = n, i;

/* rest and leng in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s16 *)x;
	py = (mlib_s16 *)y;
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
		dpy = (mlib_d64 *)vis_alignaddr(py, off);
		dy0 = vis_ld_d64_nf(dpy);
		dy1 = vis_ld_d64_nf(dpy + 1);
		dy = vis_faligndata(dy0, dy1);
		dpx = (mlib_d64 *)vis_alignaddr(px, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
		ADD16_SAT;

		px += (8 + off) >> 1;
		py += (8 + off) >> 1;
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

	if ((!((mlib_addr)px & 7)) && (!((mlib_addr)py & 7))) {
/*
 * Both addresses are 8-byte aligned. No  vis_alignaddr
 * and  vis_faligndata at all.
 */

		dpx = (mlib_d64 *)px;
		dpy = (mlib_d64 *)py;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = (*dpx++);
			dy = (*dpy++);
			ADD16_SAT;
			dpz++;
		}

		dx1 = vis_ld_d64_nf(dpx); dpx++;
		dy1 = vis_ld_d64_nf(dpy); dpy++;

	} else
	if (!((mlib_addr)px & 7)) {

/*
 * First ("x") address is 8-byte aligned. vis_alignaddr
 * and vis_faligndata only for "y".
 */

		dpx = (mlib_d64 *)px;
		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy); dpy++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = (*dpx++);
			dy1 = vis_ld_d64_nf(dpy); dpy++;
			dy = vis_faligndata(dy0, dy1);
			ADD16_SAT;
			dy0 = dy1;
			dpz++;
		}

		dx1 = vis_ld_d64_nf(dpx); dpx++;
		dy1 = dy0;

	} else
	if (!((mlib_addr)py & 7)) {

/*
 * Second ("y") address is 8-byte aligned. vis_alignaddr
 * and vis_faligndata only for "x".
 */

		dpy = (mlib_d64 *)py;
		dpx = vis_alignaddr(px, 0);
		dx0 = vis_ld_d64_nf(dpx); dpx++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx); dpx++;
			dx = vis_faligndata(dx0, dx1);
			dy = (*dpy++);
			ADD16_SAT;
			dx0 = dx1;
			dpz++;
		}

		dx1 = dx0;
		dy1 = vis_ld_d64_nf(dpy); dpy++;

	} else
	if (((mlib_addr)px & 7) == ((mlib_addr)py & 7)) {

/*
 * Both ("x" and "y") address are identically aligned.
 * There are 1 vis_alignaddr and 2 vis_faligndata(s) in the loop.
 */

		dpx = vis_alignaddr(px, 0);
		dx0 = vis_ld_d64_nf(dpx); dpx++;
		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy); dpy++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx); dpx++;
			dx = vis_faligndata(dx0, dx1);
			dy1 = vis_ld_d64_nf(dpy); dpy++;
			dy = vis_faligndata(dy0, dy1);
			ADD16_SAT;
			dx0 = dx1;
			dy0 = dy1;
			dpz++;
		}

		dx1 = dx0;
		dy1 = dy0;

	} else {

		off = (mlib_addr)px & 7;
		dpx = (mlib_d64 *)((mlib_u8 *)px - off);
		vis_write_bmask(off * 0x11111111, 0x01234567);
		dx0 = vis_ld_d64_nf(dpx); dpx++;
		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy); dpy++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx); dpx++;
			dx = vis_bshuffle(dx0, dx1);
			dy1 = vis_ld_d64_nf(dpy); dpy++;
			dy = vis_faligndata(dy0, dy1);
			ADD16_SAT;
			dx0 = dx1;
			dy0 = dy1;
			dpz++;
		}

		dx1 = dx0;
		dy1 = dy0;
	}

	if (!rest_8)
		return (MLIB_SUCCESS);

/*
 * prepare edge mask for the last bytes
 */

	emask = ~(vis_edge16((void *)(rest_8 << 1), pzend));

	off = (mlib_addr)px & 7;
	vis_write_bmask(off * 0x11111111, 0x01234567);
	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx);
	dx = vis_bshuffle(dx0, dx1);
	vis_alignaddr(py, 0);
	dy0 = dy1;
	dy1 = vis_ld_d64_nf(dpy);
	dy = vis_faligndata(dy0, dy1);

	ADD16_SAT;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAdd_S16_Sat(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorAdd_S16_S16_Sat(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAdd_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dz, dx0, dx1, dy0, dy1;
	mlib_s16 *pz, *px, *py, *pzend;

/* offset of address alignment in destination */
	mlib_s32 off;

/* edge masks */
	mlib_s32 emask;
	mlib_s32 len = n, i;

/* rest and leng in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s16 *)x;
	py = (mlib_s16 *)y;
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
		dpy = (mlib_d64 *)vis_alignaddr(py, off);
		dy0 = vis_ld_d64_nf(dpy);
		dy1 = vis_ld_d64_nf(dpy + 1);
		dy = vis_faligndata(dy0, dy1);
		dpx = (mlib_d64 *)vis_alignaddr(px, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
		ADD16_MOD;
		px += (8 + off) >> 1;
		py += (8 + off) >> 1;
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

	if ((!((mlib_addr)px & 7)) && (!((mlib_addr)py & 7))) {

/*
 * Both addresses are 8-byte aligned. No  vis_alignaddr
 * and  vis_faligndata at all.
 */

		dpx = (mlib_d64 *)px;
		dpy = (mlib_d64 *)py;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = (*dpx++);
			dy = (*dpy++);
			ADD16_MOD;
			dpz++;
		}

		dx1 = vis_ld_d64_nf(dpx); dpx++;
		dy1 = vis_ld_d64_nf(dpy); dpy++;

	} else
	if (!((mlib_addr)px & 7)) {

/*
 * First ("x") address is 8-byte aligned. vis_alignaddr
 * and vis_faligndata only for "y".
 */

		dpx = (mlib_d64 *)px;
		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy); dpy++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = (*dpx++);
			dy1 = vis_ld_d64_nf(dpy); dpy++;
			dy = vis_faligndata(dy0, dy1);
			ADD16_MOD;
			dy0 = dy1;
			dpz++;
		}

		dx1 = vis_ld_d64_nf(dpx); dpx++;
		dy1 = dy0;

	} else
	if (!((mlib_addr)py & 7)) {

/*
 * Second ("y") address is 8-byte aligned. vis_alignaddr
 * and vis_faligndata only for "x".
 */

		dpy = (mlib_d64 *)py;
		dpx = vis_alignaddr(px, 0);
		dx0 = vis_ld_d64_nf(dpx); dpx++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx); dpx++;
			dx = vis_faligndata(dx0, dx1);
			dy = (*dpy++);
			ADD16_MOD;
			dx0 = dx1;
			dpz++;
		}

		dx1 = dx0;
		dy1 = vis_ld_d64_nf(dpy); dpy++;

	} else
	if (((mlib_addr)px & 7) == ((mlib_addr)py & 7)) {

/*
 * Both ("x" and "y") address are identically aligned.
 * There are 1 vis_alignaddr and 2 vis_faligndata(s) in the loop.
 */

		dpx = vis_alignaddr(px, 0);
		dx0 = vis_ld_d64_nf(dpx); dpx++;
		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy); dpy++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx); dpx++;
			dx = vis_faligndata(dx0, dx1);
			dy1 = vis_ld_d64_nf(dpy); dpy++;
			dy = vis_faligndata(dy0, dy1);
			ADD16_MOD;
			dpz++;
			dx0 = dx1;
			dy0 = dy1;
		}

		dx1 = dx0;
		dy1 = dy0;
	} else {

/*
 * Both ("x" and "y") address are arbitrary aligned.
 */

		off = (mlib_addr)px & 7;
		dpx = (mlib_d64 *)((mlib_u8 *)px - off);
		vis_write_bmask(off * 0x11111111, 0x01234567);
		dx0 = vis_ld_d64_nf(dpx); dpx++;
		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy); dpy++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx); dpx++;
			dx = vis_bshuffle(dx0, dx1);
			dy1 = vis_ld_d64_nf(dpy); dpy++;
			dy = vis_faligndata(dy0, dy1);
			ADD16_MOD;
			dx0 = dx1;
			dy0 = dy1;
			dpz++;
		}

		dx1 = dx0;
		dy1 = dy0;
	}

	if (!rest_8)
		return (MLIB_SUCCESS);

/*
 * prepare edge mask for the last bytes
 */

	emask = ~(vis_edge16((void *)(rest_8 << 1), pzend));

	off = (mlib_addr)px & 7;
	vis_write_bmask(off * 0x11111111, 0x01234567);
	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx);
	dx = vis_bshuffle(dx0, dx1);
	vis_alignaddr(py, 0);
	dy0 = dy1;
	dy1 = vis_ld_d64_nf(dpy);
	dy = vis_faligndata(dy0, dy1);

	ADD16_MOD;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAdd_S32_S16_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dx0, dx1, dy0, dy1;
	mlib_d64 dxh, dxl, dyh, dyl, dzh, dzl;
	mlib_s16 *px, *py;
	mlib_s32 *pz;
	mlib_s32 len = n, i, off;
	mlib_s32 rest_8, even_8;

/*
 * rest and leng in terms of 8 bytes.
 */
	mlib_f32 two_16_ones = vis_to_float(0x10001);

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s16 *)x;
	py = (mlib_s16 *)y;
	pz = (mlib_s32 *)z;

	if (n <= 4) {
		while (n--) {
			(*pz++) = ((mlib_s32)(*px) + *py);
			px++;
			py++;
		}

		return (MLIB_SUCCESS);
	}

/*
 * prepare the destination address
 */

	if ((mlib_addr)pz & 7) {
		(*pz++) = ((mlib_s32)(*px)) + (*py);
		px++;
		py++;
		len--;
	}

	dpz = (mlib_d64 *)pz;

	even_8 = len >> 2;
	rest_8 = len & 0x3;

	if ((!((mlib_addr)px & 7)) && (!((mlib_addr)py & 7))) {

/*
 * Both addresses are 8-byte aligned.
 * * No  vis_alignaddr and  vis_faligndata at all.
 */

		dpx = (mlib_d64 *)px;
		dpy = (mlib_d64 *)py;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = (*dpx++);
			dy = (*dpy++);
			ADD_S16_S32;

/*
 * store 16 bytes of result
 */
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
		}

	} else
	if (!((mlib_addr)px & 7)) {

/*
 * First ("x") address is 8-byte aligned.
 * * vis_alignaddr and vis_faligndata only for "y".**
 */

		dpx = (mlib_d64 *)px;
		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy); dpy++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = (*dpx++);
			dy1 = vis_ld_d64_nf(dpy); dpy++;
			dy = vis_faligndata(dy0, dy1);
			ADD_S16_S32;
			dy0 = dy1;
/*
 * store 16 bytes of result
 */
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
		}

	} else
	if (!((mlib_addr)py & 7)) {

/*
 * Second ("y") address is 8-byte aligned.
 * * vis_alignaddr and vis_faligndata only for "x".**
 */

		dpy = (mlib_d64 *)py;
		dpx = vis_alignaddr(px, 0);
		dx0 = vis_ld_d64_nf(dpx); dpx++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx); dpx++;
			dx = vis_faligndata(dx0, dx1);
			dy = (*dpy++);
			ADD_S16_S32;
			dx0 = dx1;

/*
 * store16 bytes of result
 */
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
		}

	} else
	if (((mlib_addr)px & 7) == ((mlib_addr)py & 7)) {

/*
 * Both ("x" and "y") address are identically aligned.
 * * There are 1 vis_alignaddr and 2 vis_faligndata(s) in the loop.
 */

		dpx = vis_alignaddr(px, 0);
		dx0 = vis_ld_d64_nf(dpx); dpx++;
		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy); dpy++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx); dpx++;
			dx = vis_faligndata(dx0, dx1);
			dy1 = vis_ld_d64_nf(dpy); dpy++;
			dy = vis_faligndata(dy0, dy1);
			ADD_S16_S32;
			dx0 = dx1;
			dy0 = dy1;

/*
 * store 16 bytes of result
 */
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
		}

	} else {

/*
 * Both ("x" and "y") address are arbitrary aligned.
 */

		off = (mlib_addr)px & 7;
		dpx = (mlib_d64 *)((mlib_u8 *)px - off);
		dx0 = vis_ld_d64_nf(dpx); dpx++;
		vis_write_bmask(off * 0x11111111, 0x01234567);
		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy); dpy++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx); dpx++;
			dx = vis_bshuffle(dx0, dx1);
			dy1 = vis_ld_d64_nf(dpy); dpy++;
			dy = vis_faligndata(dy0, dy1);
			ADD_S16_S32;
			dx0 = dx1;
			dy0 = dy1;
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
	py += (even_8 << 2);
	pz += (even_8 << 2);

	while (rest_8--) {
		(*pz++) = ((mlib_s32)(*px)) + (*py);
		px++;
		py++;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAdd_S16_Mod(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorAdd_S16_S16_Mod(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAdd_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorAdd_S16_S16_Sat(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAdd_S16C_Sat(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorAdd_S16C_S16C_Sat(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAdd_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorAdd_S16_S16_Mod(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAdd_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorAdd_S32_S16_Mod(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAdd_S16C_Mod(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorAdd_S16C_S16C_Mod(xz, xz, y, n));
}

/* *********************************************************** */
