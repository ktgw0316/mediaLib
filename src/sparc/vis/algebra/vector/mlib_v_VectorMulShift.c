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
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_v_VectorMulShift.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorMulShift - multiplication of vectors with shifting
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMulShift_U8_U8_Mod(mlib_u8       *z,
 *                                                const mlib_u8 *x,
 *                                                const mlib_u8 *y,
 *                                                mlib_s32      n,
 *                                                mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_U8_U8_Sat(mlib_u8       *z,
 *                                                const mlib_u8 *x,
 *                                                const mlib_u8 *y,
 *                                                mlib_s32      n,
 *                                                mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_U8C_U8C_Mod(mlib_u8       *z,
 *                                                  const mlib_u8 *x,
 *                                                  const mlib_u8 *y,
 *                                                  mlib_s32      n,
 *                                                  mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_U8C_U8C_Sat(mlib_u8       *z,
 *                                                  const mlib_u8 *x,
 *                                                  const mlib_u8 *y,
 *                                                  mlib_s32      n,
 *                                                  mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_S8_S8_Mod(mlib_s8       *z,
 *                                                const mlib_s8 *x,
 *                                                const mlib_s8 *y,
 *                                                mlib_s32      n,
 *                                                mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_S8_S8_Sat(mlib_s8       *z,
 *                                                const mlib_s8 *x,
 *                                                const mlib_s8 *y,
 *                                                mlib_s32      n,
 *                                                mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_S8C_S8C_Mod(mlib_s8       *z,
 *                                                  const mlib_s8 *x,
 *                                                  const mlib_s8 *y,
 *                                                  mlib_s32      n,
 *                                                  mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_S8C_S8C_Sat(mlib_s8       *z,
 *                                                  const mlib_s8 *x,
 *                                                  const mlib_s8 *y,
 *                                                  mlib_s32      n,
 *                                                  mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_S16_S16_Mod(mlib_s16       *z,
 *                                                  const mlib_s16 *x,
 *                                                  const mlib_s16 *y,
 *                                                  mlib_s32       n,
 *                                                  mlib_s32       shift);
 *      mlib_status mlib_VectorMulShift_S16_S16_Sat(mlib_s16       *z,
 *                                                  const mlib_s16 *x,
 *                                                  const mlib_s16 *y,
 *                                                  mlib_s32       n,
 *                                                  mlib_s32       shift);
 *      mlib_status mlib_VectorMulShift_S16C_S16C_Sat(mlib_s16       *z,
 *                                                    const mlib_s16 *x,
 *                                                    const mlib_s16 *y,
 *                                                    mlib_s32       n,
 *                                                    mlib_s32       shift);
 *      mlib_status mlib_VectorMulShift_S16C_S16C_Mod(mlib_s16       *z,
 *                                                    const mlib_s16 *x,
 *                                                    const mlib_s16 *y,
 *                                                    mlib_s32       n,
 *                                                    mlib_s32       shift);
 *      mlib_status mlib_VectorMulShift_S32_S32_Mod(mlib_s32       *z,
 *                                                  const mlib_s32 *x,
 *                                                  const mlib_s32 *y,
 *                                                  mlib_s32       n,
 *                                                  mlib_s32       shift);
 *      mlib_status mlib_VectorMulShift_S32_S32_Sat(mlib_s32       *z,
 *                                                  const mlib_s32 *x,
 *                                                  const mlib_s32 *y,
 *                                                  mlib_s32       n,
 *                                                  mlib_s32       shift);
 *      mlib_status mlib_VectorMulShift_S32C_S32C_Sat(mlib_s32       *z,
 *                                                    const mlib_s32 *x,
 *                                                    const mlib_s32 *y,
 *                                                    mlib_s32       n,
 *                                                    mlib_s32       shift);
 *      mlib_status mlib_VectorMulShift_S32C_S32C_Mod(mlib_s32       *z,
 *                                                    const mlib_s32 *x,
 *                                                    const mlib_s32 *y,
 *                                                    mlib_s32       n,
 *                                                    mlib_s32       shift);
 *
 * ARGUMENTS
 *      z      the pointer to the first element of the result vector
 *      xz     the pointer to the first element of the first source and
 *             result vector
 *      x      the pointer to the first element of the first source vector
 *      y      the pointer to the first element of the second source vector
 *      n      the number of elements in each vector
 *      shift  the right shifting factor
 *
 * DESCRIPTION
 *      For U8, S8, S16, S32 types
 *
 *           z[i] = x[i] * y[i] * 2^(-shift); 0 <= i < n
 *
 *      For U8C, S8C, S16C, S32C types
 *
 *           z[2*i] = (x[2*i] * y[2*i] - x[2*i + 1] * y[2*i + 1]) *
 *                         2^(-shift); 0 <= i < n
 *           z[2*i + 1] = (x[2*i + 1] * y[2*i] + x[2*i] * y[2*i + 1]) *
 *                         2^(-shift); 0 <= i < n
 *
 *      For _Mod mode, modulo operation is performed.  For _Sat mode,
 *      saturated operation is performed.
 *
 * RESTRICTIONS
 *      ranges of valid shift are:
 *           1 <= shift <= 8   for U8, S8, U8C, S8C types
 *           1 <= shift <= 16  for S16, S16C types
 *           1 <= shift <= 31  for S32, S32C types
 */

#include <mlib_algebra.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorMulShift_S16C_S16C_Mod = \
	__mlib_VectorMulShift_S16C_S16C_Mod
#pragma weak mlib_VectorMulShift_S16C_S16C_Sat = \
	__mlib_VectorMulShift_S16C_S16C_Sat
#pragma weak mlib_VectorMulShift_S16_S16_Mod = \
	__mlib_VectorMulShift_S16_S16_Mod
#pragma weak mlib_VectorMulShift_S16_S16_Sat = \
	__mlib_VectorMulShift_S16_S16_Sat
#pragma weak mlib_VectorMulShift_S8C_S8C_Mod = \
	__mlib_VectorMulShift_S8C_S8C_Mod
#pragma weak mlib_VectorMulShift_S8C_S8C_Sat = \
	__mlib_VectorMulShift_S8C_S8C_Sat
#pragma weak mlib_VectorMulShift_S8_S8_Mod = __mlib_VectorMulShift_S8_S8_Mod
#pragma weak mlib_VectorMulShift_S8_S8_Sat = __mlib_VectorMulShift_S8_S8_Sat
#pragma weak mlib_VectorMulShift_U8C_U8C_Mod = \
	__mlib_VectorMulShift_U8C_U8C_Mod
#pragma weak mlib_VectorMulShift_U8C_U8C_Sat = \
	__mlib_VectorMulShift_U8C_U8C_Sat
#pragma weak mlib_VectorMulShift_U8_U8_Mod = __mlib_VectorMulShift_U8_U8_Mod
#pragma weak mlib_VectorMulShift_U8_U8_Sat = __mlib_VectorMulShift_U8_U8_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VectorMulShift_S16C_S16C_Mod)
	mlib_VectorMulShift_S16C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_S16C_S16C_Mod")));
__typeof__(__mlib_VectorMulShift_S16C_S16C_Sat)
	mlib_VectorMulShift_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_S16C_S16C_Sat")));
__typeof__(__mlib_VectorMulShift_S16_S16_Mod) mlib_VectorMulShift_S16_S16_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_S16_S16_Mod")));
__typeof__(__mlib_VectorMulShift_S16_S16_Sat) mlib_VectorMulShift_S16_S16_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_S16_S16_Sat")));
__typeof__(__mlib_VectorMulShift_S8C_S8C_Mod) mlib_VectorMulShift_S8C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_S8C_S8C_Mod")));
__typeof__(__mlib_VectorMulShift_S8C_S8C_Sat) mlib_VectorMulShift_S8C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_S8C_S8C_Sat")));
__typeof__(__mlib_VectorMulShift_S8_S8_Mod) mlib_VectorMulShift_S8_S8_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_S8_S8_Mod")));
__typeof__(__mlib_VectorMulShift_S8_S8_Sat) mlib_VectorMulShift_S8_S8_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_S8_S8_Sat")));
__typeof__(__mlib_VectorMulShift_U8C_U8C_Mod) mlib_VectorMulShift_U8C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_U8C_U8C_Mod")));
__typeof__(__mlib_VectorMulShift_U8C_U8C_Sat) mlib_VectorMulShift_U8C_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_U8C_U8C_Sat")));
__typeof__(__mlib_VectorMulShift_U8_U8_Mod) mlib_VectorMulShift_U8_U8_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_U8_U8_Mod")));
__typeof__(__mlib_VectorMulShift_U8_U8_Sat) mlib_VectorMulShift_U8_U8_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_U8_U8_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MUL_S16_S32                                                    \
	{                                                              \
	    mlib_d64 dr2, dr3;                                         \
	                                                               \
	    dr = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dy));    \
	    dr1 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dy));   \
	    dr = vis_fpadd32(dr, dr1);                                 \
	    dr2 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dy));   \
	    dr3 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dy));   \
	    dr1 = vis_fpadd32(dr2, dr3);                               \
	}

#define	PACK_S32_S16_MOD                                        \
	{                                                       \
	    mlib_d64 sign, sign1;                               \
	                                                        \
	    sign = vis_fand(mask_sign, dr);                     \
	    dr = vis_fand(mask_clear, dr);                      \
	    sign1 = vis_fand(mask_sign, dr1);                   \
	    dr1 = vis_fand(mask_clear, dr1);                    \
	    dr = vis_fpackfix_pair(dr, dr1);                    \
	    sign = vis_fpack16_pair(sign, sign1);               \
	    sign = vis_fand(sign, mask_real_sign);              \
	    dr = vis_for(sign, dr);                             \
	}

/* *********************************************************** */

#define	PACK_S32_S16_STORE_SAT                                  \
	dr = vis_fpackfix_pair(dr, dr1);                        \
	vis_pst_16(dr, dpz, emask)

/* *********************************************************** */

#define	MUL_U8_U16                                              \
	{                                                       \
	    mlib_d64 dr2, dr3, dr4, dr5;                        \
	                                                        \
	    dr2 = vis_fxnor(restore, dy);                       \
	    dr4 = vis_fpmerge(vis_read_hi(dr2), fzero);         \
	    dr4 = vis_fmul8x16(vis_read_hi(dx), dr4);           \
	    dr3 = vis_fmul8x16al(vis_read_hi(dx), f127);        \
	    dr = vis_fpsub16(dr3, dr4);                         \
	    dr2 = vis_fpmerge(vis_read_lo(dr2), fzero);         \
	    dr2 = vis_fmul8x16(vis_read_lo(dx), dr2);           \
	    dr5 = vis_fmul8x16al(vis_read_lo(dx), f127);        \
	    dr1 = vis_fpsub16(dr5, dr2);                        \
	}

/* *********************************************************** */

#define	PACK_U16_U8                                                  \
	mask_over =                                                  \
	vis_fcmpgt16(dzero, dr1) | (vis_fcmpgt16(dzero, dr) << 4);   \
	dr = vis_fpack16_to_hi(dr, dr);                              \
	dr = vis_fpack16_to_lo(dr, dr1)

/* *********************************************************** */

#define	PACK_U16_U8_8                                           \
	r0 = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));     \
	r1 = vis_fpmerge(vis_read_hi(r0), vis_read_lo(r0));     \
	r2 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));   \
	r3 = vis_fpmerge(vis_read_hi(r2), vis_read_lo(r2));     \
	dr = vis_freg_pair(vis_read_hi(r1), vis_read_hi(r3))

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
/* edge masks */
	mlib_s32 emask, len, i;

/* overflow masks */
	mlib_s32 mask_over;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s8 *pzend, *px = (mlib_s8 *)x, *py = (mlib_s8 *)y;
	mlib_d64 *dpz, *dpx, *dpy, *dpzend;
	mlib_d64 dx, dy, dr, dr1, dx0, dx1, dy0, dy1, r0, r1, r2, r3;
	mlib_s32 k, N;
	mlib_d64 x_buff[256];

	mlib_d64 restore = vis_to_double_dup(0x80808080);
	mlib_d64 d_over = vis_fone();
	mlib_d64 dzero = vis_fzero();
	mlib_f32 f127 = vis_to_float(0x7f00), fzero = vis_fzeros();

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

	if (shift == 8) {

/*
 * prepare the destination address
 */
		pzend = (mlib_s8 *)z + n - 1;
		dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
		dpz = (mlib_d64 *)((mlib_addr)z & (~7));
		off = (mlib_addr)dpz - (mlib_addr)z;

/*
 * generate edge mask for the start point
 */
		emask = vis_edge8(z, pzend);

/*
 * prepare the source address
 */
		dpx = (mlib_d64 *)vis_alignaddr(px, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
		dpy = (mlib_d64 *)vis_alignaddr(py, off);
		dy0 = vis_ld_d64_nf(dpy);
		dy1 = vis_ld_d64_nf(dpy + 1);
		dy = vis_faligndata(dy0, dy1);

		MUL_U8_U16;
		PACK_U16_U8_8;
/* store first bytes of result */
		vis_pst_8(dr, dpz, emask);
		dpz++;
/*
 * len = (mlib_s32)(((mlib_addr)dpzend - (mlib_addr)dpz) >> 3);
 */
		len = dpzend - dpz;

		if (len < 0)
			return (MLIB_SUCCESS);

		dpx++;
		dpy++;

		if ((((mlib_addr)x ^ (mlib_addr)z) & 7) == 0) {
			if ((((mlib_addr)x ^ (mlib_addr)y) & 7) == 0) {
#pragma pipeloop(0)
				for (i = 0; i < len; i++) {
					dx = dpx[i];
					dy = dpy[i];
					MUL_U8_U16;
					PACK_U16_U8_8;
					dpz[i] = dr;
				}

				dpz = dpzend;
				dx = dpx[len];
				dy = dpy[len];
				MUL_U8_U16;
			} else {
#pragma pipeloop(0)
				for (i = 0; i < len; i++) {
					dx = dpx[i];
					dy0 = dpy[i];
					dy1 = vis_ld_d64_nf(dpy + i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_U8_U16;
					PACK_U16_U8_8;
					dpz[i] = dr;
				}

				dpz = dpzend;
				dx = dpx[len];
				dy0 = dpy[len];
				dy1 = vis_ld_d64_nf(dpy + len + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_U8_U16;
			}
		} else {
			px += 8 - ((mlib_addr)z & 7);

			for (k = 0; k < (len >> 8); k++) {
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + k * 256),
					256 * 8);
				(void *)vis_alignaddr(py, off);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = x_buff[i];
					dy0 = dpy[k * 256 + i];
					dy1 = vis_ld_d64_nf(dpy +
						k * 256 + i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_U8_U16;
					PACK_U16_U8_8;
					dpz[k * 256 + i] = dr;
				}
			}

			N = len & ~255;
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)px + N),
				(len - N) * 8 + 1 + ((mlib_addr)pzend & 7));
			vis_alignaddr(py, off);
#pragma pipeloop(0)
			for (i = N; i < len; i++) {
				dx = x_buff[i - N];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_U8_U16;
				PACK_U16_U8_8;
				dpz[i] = dr;
			}

			dpz = dpzend;
			dx = x_buff[len - N];
			dy0 = vis_ld_d64_nf(dpy + len);
			dy1 = vis_ld_d64_nf(dpy + len + 1);
			dy = vis_faligndata(dy0, dy1);
			MUL_U8_U16;
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			PACK_U16_U8_8;
/* prepare edge mask for the last bytes */
			emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
			vis_pst_8(dr, dpz, emask);
		}

	} else {

/* initialize GSR scale factor */
		vis_write_gsr((7 - shift) << 3);

/*
 * prepare the destination address
 */
		pzend = (mlib_s8 *)z + n - 1;
		dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
		dpz = (mlib_d64 *)((mlib_addr)z & (~7));
		off = (mlib_addr)dpz - (mlib_addr)z;

/*
 * generate edge mask for the start point
 */
		emask = vis_edge8(z, pzend);

/*
 * prepare the source address
 */
		dpx = (mlib_d64 *)vis_alignaddr(px, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
		dpy = (mlib_d64 *)vis_alignaddr(py, off);
		dy0 = vis_ld_d64_nf(dpy);
		dy1 = vis_ld_d64_nf(dpy + 1);
		dy = vis_faligndata(dy0, dy1);

		MUL_U8_U16 PACK_U16_U8;
/* store first bytes of result */
		vis_pst_8(dr, dpz, emask);
		vis_pst_8(d_over, dpz, mask_over & emask);
		dpz++;
/*
 * len = (mlib_s32)(((mlib_addr)dpzend - (mlib_addr)dpz) >> 3);
 */
		len = dpzend - dpz;

		if (len < 0)
			return (MLIB_SUCCESS);

		dpx++;
		dpy++;

		if ((((mlib_addr)x ^ (mlib_addr)z) & 7) == 0) {
			if ((((mlib_addr)x ^ (mlib_addr)y) & 7) == 0) {
#pragma pipeloop(0)
				for (i = 0; i < len; i++) {
					dx = dpx[i];
					dy = dpy[i];
					MUL_U8_U16;
					PACK_U16_U8;
					vis_pst_8(dr, dpz + i, ~mask_over);
					vis_pst_8(d_over, dpz + i, mask_over);
				}

				dpz = dpzend;
				dx = dpx[len];
				dy = dpy[len];
				MUL_U8_U16;
			} else {
#pragma pipeloop(0)
				for (i = 0; i < len; i++) {
					dx = dpx[i];
					dy0 = dpy[i];
					dy1 = vis_ld_d64_nf(dpy + i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_U8_U16;
					PACK_U16_U8;
					vis_pst_8(dr, dpz + i, ~mask_over);
					vis_pst_8(d_over, dpz + i, mask_over);
				}

				dpz = dpzend;
				dx = dpx[len];
				dy0 = dpy[len];
				dy1 = vis_ld_d64_nf(dpy + len + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_U8_U16;
			}
		} else {
			px += 8 - ((mlib_addr)z & 7);

			for (k = 0; k < (len >> 8); k++) {
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + k * 256),
					256 * 8);
				(void *)vis_alignaddr(py, off);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = x_buff[i];
					dy0 = dpy[k * 256 + i];
					dy1 = vis_ld_d64_nf(dpy +
						k * 256 + i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_U8_U16;
					PACK_U16_U8;
					vis_pst_8(dr, dpz + k * 256 + i,
						~mask_over);
					vis_pst_8(d_over, dpz + k * 256 + i,
						mask_over);
				}
			}

			N = len & ~255;
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)px + N),
				(len - N) * 8 + 1 + ((mlib_addr)pzend & 7));
			vis_alignaddr(py, off);
#pragma pipeloop(0)
			for (i = N; i < len; i++) {
				dx = x_buff[i - N];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_U8_U16;
				PACK_U16_U8;
				vis_pst_8(dr, dpz + i, ~mask_over);
				vis_pst_8(d_over, dpz + i, mask_over);
			}

			dpz = dpzend;
			dx = x_buff[len - N];
			dy0 = vis_ld_d64_nf(dpy + len);
			dy1 = vis_ld_d64_nf(dpy + len + 1);
			dy = vis_faligndata(dy0, dy1);
			MUL_U8_U16;
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			PACK_U16_U8;
/* prepare edge mask for the last bytes */
			emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
			vis_pst_8(dr, dpz, emask);
			vis_pst_8(d_over, dpz, mask_over & emask);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S16_S8_SAT                                          \
	{                                                       \
	    mlib_d64 dr2, dr3, dr4, dr5;                        \
	                                                        \
	    dr2 = vis_fpmerge(vis_read_hi(dx), fzero);          \
	    dr3 = vis_fpmerge(vis_read_hi(dy), fzero);          \
	    dr = vis_fmul8sux16(dr2, dr3);                      \
	    dr4 = vis_fpmerge(vis_read_lo(dx), fzero);          \
	    dr5 = vis_fpmerge(vis_read_lo(dy), fzero);          \
	    dr1 = vis_fmul8sux16(dr4, dr5);                     \
	    dr = vis_fandnot(anti_rounding_mask, dr);           \
	    dr1 = vis_fandnot(anti_rounding_mask, dr1);         \
	    dr = vis_fmul8x16(scale, dr);                       \
	    dr1 = vis_fmul8x16(scale, dr1);                     \
	}

/* *********************************************************** */

#define	PACK_S16_S8_SAT                                         \
	dr = vis_fpadd16(displacement, dr);                     \
	dr1 = vis_fpadd16(displacement, dr1);                   \
	dr = vis_fpack16_to_hi(dr, dr);                         \
	dr = vis_fpack16_to_lo(dr, dr1);                        \
	dr = vis_fxor(dr, restore)

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
/* edge masks */
	mlib_s32 emask, len, i;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s8 *pzend, *px = (mlib_s8 *)x, *py = (mlib_s8 *)y;
	mlib_d64 *dpz, *dpx, *dpy, *dpzend;
	mlib_d64 dx, dy, dr, dr1, dx0, dx1, dy0, dy1;
	mlib_s32 k, N;
	mlib_d64 x_buff[256];
	mlib_f32 fzero = vis_fzeros();
	mlib_d64 displacement = vis_to_double_dup(0x800080);
	mlib_d64 restore = vis_to_double_dup(0x80808080);
	mlib_d64 anti_rounding_mask = vis_to_double_dup(0x10001 << (shift - 1));
	mlib_f32 scale = vis_to_float(0x80808080 >> (shift - 1));

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);

/*
 * prepare the destination address
 */
	pzend = (mlib_s8 *)z + n - 1;
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z;

/*
 * generate edge mask for the start point
 */
	emask = vis_edge8(z, pzend);

/*
 * prepare the source address
 */
	dpx = (mlib_d64 *)vis_alignaddr(px, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_faligndata(dx0, dx1);
	dpy = (mlib_d64 *)vis_alignaddr(py, off);
	dy0 = vis_ld_d64_nf(dpy);
	dy1 = vis_ld_d64_nf(dpy + 1);
	dy = vis_faligndata(dy0, dy1);
	MUL_S16_S8_SAT;
	PACK_S16_S8_SAT;
/* store first bytes of result */
	vis_pst_8(dr, dpz, emask);
	dpz++;
	len = dpzend - dpz;

	if (len < 0)
		return (MLIB_SUCCESS);
	dpx++;
	dpy++;

	if ((((mlib_addr)x ^ (mlib_addr)z) & 7) == 0) {
		if ((((mlib_addr)x ^ (mlib_addr)y) & 7) == 0) {
#pragma pipeloop(0)
			for (i = 0; i < len; i++) {
				dx = dpx[i];
				dy = dpy[i];
				MUL_S16_S8_SAT;
				PACK_S16_S8_SAT;
				dpz[i] = dr;
			}

			dpz = dpzend;
			dx = dpx[len];
			dy = dpy[len];
			MUL_S16_S8_SAT;
		} else {
#pragma pipeloop(0)
			for (i = 0; i < len; i++) {
				dx = dpx[i];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S16_S8_SAT;
				PACK_S16_S8_SAT;
				dpz[i] = dr;
			}

			dpz = dpzend;
			dx = dpx[len];
			dy0 = dpy[len];
			dy1 = vis_ld_d64_nf(dpy + len + 1);
			dy = vis_faligndata(dy0, dy1);
			MUL_S16_S8_SAT;
		}
	} else {
		px += 8 - ((mlib_addr)z & 7);

		for (k = 0; k < (len >> 8); k++) {
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)px + k * 256), 256 * 8);
			(void *)vis_alignaddr(py, off);
#pragma pipeloop(0)
			for (i = 0; i < 256; i++) {
				dx = x_buff[i];
				dy0 = dpy[k * 256 + i];
				dy1 = vis_ld_d64_nf(dpy + k * 256 + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S16_S8_SAT;
				PACK_S16_S8_SAT;
				dpz[256 * k + i] = dr;
			}
		}

		N = len & ~255;
		__mlib_VectorCopy_U8((void *)x_buff,
			(void *)((mlib_d64 *)px + N), (len - N) * 8 +
			1 + ((mlib_addr)pzend & 7));
		vis_alignaddr(py, off);
#pragma pipeloop(0)
		for (i = N; i < len; i++) {
			dx = x_buff[i - N];
			dy0 = dpy[i];
			dy1 = vis_ld_d64_nf(dpy + i + 1);
			dy = vis_faligndata(dy0, dy1);
			MUL_S16_S8_SAT;
			PACK_S16_S8_SAT;
			dpz[i] = dr;
		}

		dpz = dpzend;
		dx = x_buff[len - N];
		dy0 = vis_ld_d64_nf(dpy + len);
		dy1 = vis_ld_d64_nf(dpy + len + 1);
		dy = vis_faligndata(dy0, dy1);
		MUL_S16_S8_SAT;
	}

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
		PACK_S16_S8_SAT;
/* prepare edge mask for the last bytes */
		emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
		vis_pst_8(dr, dpz, emask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S16_U8_MOD                                          \
	{                                                       \
	    mlib_d64 dr2, dr3, dr4, dr5;                        \
	                                                        \
	    dr2 = vis_fxnor(restore, dy);                       \
	    dr4 = vis_fpmerge(vis_read_hi(dr2), fzero);         \
	    dr4 = vis_fmul8x16(vis_read_hi(dx), dr4);           \
	    dr3 = vis_fmul8x16al(vis_read_hi(dx), f127);        \
	    dr = vis_fpsub16(dr3, dr4);                         \
	    dr2 = vis_fpmerge(vis_read_lo(dr2), fzero);         \
	    dr2 = vis_fmul8x16(vis_read_lo(dx), dr2);           \
	    dr5 = vis_fmul8x16al(vis_read_lo(dx), f127);        \
	    dr1 = vis_fpsub16(dr5, dr2);                        \
	    dr = vis_fandnot(anti_rounding_mask, dr);           \
	    dr1 = vis_fandnot(anti_rounding_mask, dr1);         \
	    dr = vis_fmul8x16(scale, dr);                       \
	    dr1 = vis_fmul8x16(scale, dr1);                     \
	}

/* *********************************************************** */

#define	PACK_U16_U8_MOD                                         \
	dr = vis_fand(dr, without_control);                     \
	dr1 = vis_fand(dr1, without_control);                   \
	dr = vis_fpack16_to_hi(dr, dr);                         \
	dr = vis_fpack16_to_lo(dr, dr1)

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
/* edge masks */
	mlib_s32 emask, len, i;

/* overflow masks */
	mlib_s32 mask_over;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s8 *pzend, *px = (mlib_s8 *)x, *py = (mlib_s8 *)y;
	mlib_d64 *dpz, *dpx, *dpy, *dpzend;
	mlib_d64 dx, dy, dr, dr1, dx0, dx1, dy0, dy1, r0, r1, r2, r3;
	mlib_s32 k, N;
	mlib_d64 x_buff[256];

	mlib_d64 restore = vis_to_double_dup(0x80808080);
	mlib_f32 f127 = vis_to_float(0x7f00), fzero = vis_fzeros();
	mlib_d64 without_control = vis_to_double_dup(0xff00ff);
	mlib_d64 anti_rounding_mask = vis_to_double_dup(0x10001 << (shift - 1));
	mlib_f32 scale = vis_to_float(0x80808080 >> (shift - 1));

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);

/*
 * prepare the destination address
 */
	pzend = (mlib_s8 *)z + n - 1;
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z;

/*
 * generate edge mask for the start point
 */
	emask = vis_edge8(z, pzend);

/*
 * prepare the source address
 */
	dpx = (mlib_d64 *)vis_alignaddr(px, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_faligndata(dx0, dx1);
	dpy = (mlib_d64 *)vis_alignaddr(py, off);
	dy0 = vis_ld_d64_nf(dpy);
	dy1 = vis_ld_d64_nf(dpy + 1);
	dy = vis_faligndata(dy0, dy1);
	MUL_S16_U8_MOD;
	PACK_U16_U8_MOD;
/* store first bytes of result */
	vis_pst_8(dr, dpz, emask);
	dpz++;
/*
 * len = (mlib_s32)(((mlib_addr)dpzend - (mlib_addr)dpz) >> 3);
 */
	len = dpzend - dpz;

	if (len < 0)
		return (MLIB_SUCCESS);

	dpx++;
	dpy++;

	if ((((mlib_addr)x ^ (mlib_addr)z) & 7) == 0) {
		if ((((mlib_addr)x ^ (mlib_addr)y) & 7) == 0) {
#pragma pipeloop(0)
			for (i = 0; i < len; i++) {
				dx = dpx[i];
				dy = dpy[i];
				MUL_S16_U8_MOD;
				PACK_U16_U8_MOD;
				dpz[i] = dr;
			}

			dpz = dpzend;
			dx = dpx[len];
			dy = dpy[len];
			MUL_S16_U8_MOD;
		} else {
#pragma pipeloop(0)
			for (i = 0; i < len; i++) {
				dx = dpx[i];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S16_U8_MOD;
				PACK_U16_U8_MOD;
				dpz[i] = dr;
			}

			dpz = dpzend;
			dx = dpx[len];
			dy0 = dpy[len];
			dy1 = vis_ld_d64_nf(dpy + len + 1);
			dy = vis_faligndata(dy0, dy1);
			MUL_S16_U8_MOD;
		}
	} else {
		px += 8 - ((mlib_addr)z & 7);

		for (k = 0; k < (len >> 8); k++) {
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)px + k * 256), 256 * 8);
			(void *)vis_alignaddr(py, off);
#pragma pipeloop(0)
			for (i = 0; i < 256; i++) {
				dx = x_buff[i];
				dy0 = dpy[k * 256 + i];
				dy1 = vis_ld_d64_nf(dpy + k * 256 + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S16_U8_MOD;
				PACK_U16_U8_MOD;
				dpz[k * 256 + i] = dr;
			}
		}

		N = len & ~255;
		__mlib_VectorCopy_U8((void *)x_buff,
			(void *)((mlib_d64 *)px + N), (len - N) * 8 +
			1 + ((mlib_addr)pzend & 7));
		vis_alignaddr(py, off);
#pragma pipeloop(0)
		for (i = N; i < len; i++) {
			dx = x_buff[i - N];
			dy0 = dpy[i];
			dy1 = vis_ld_d64_nf(dpy + i + 1);
			dy = vis_faligndata(dy0, dy1);
			MUL_S16_U8_MOD;
			PACK_U16_U8_MOD;
			dpz[i] = dr;
		}

		dpz = dpzend;
		dx = x_buff[len - N];
		dy0 = vis_ld_d64_nf(dpy + len);
		dy1 = vis_ld_d64_nf(dpy + len + 1);
		dy = vis_faligndata(dy0, dy1);
		MUL_S16_U8_MOD;
	}

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
		PACK_U16_U8_MOD;
/* prepare edge mask for the last bytes */
		emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
		vis_pst_8(dr, dpz, emask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	PACK_S16_S8_MOD                                         \
	dr = vis_fand(dr, without_control);                     \
	dr1 = vis_fand(dr1, without_control);                   \
	dr = vis_fpack16_to_hi(dr, dr);                         \
	dr = vis_fpack16_to_lo(dr, dr1)

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S8_S8_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
/* edge masks */
	mlib_s32 emask, len, i;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s8 *pzend, *px = (mlib_s8 *)x, *py = (mlib_s8 *)y;
	mlib_d64 *dpz, *dpx, *dpy, *dpzend;
	mlib_d64 dx, dy, dr, dr1, dx0, dx1, dy0, dy1;
	mlib_s32 k, N;
	mlib_d64 x_buff[256];
	mlib_f32 fzero = vis_fzeros();
	mlib_d64 without_control = vis_to_double_dup(0xff00ff);
	mlib_d64 anti_rounding_mask = vis_to_double_dup(0x10001 << (shift - 1));
	mlib_f32 scale = vis_to_float(0x80808080 >> (shift - 1));

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);

/*
 * prepare the destination address
 */
	pzend = (mlib_s8 *)z + n - 1;
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z;

/*
 * generate edge mask for the start point
 */
	emask = vis_edge8(z, pzend);

/*
 * prepare the source address
 */
	dpx = (mlib_d64 *)vis_alignaddr(px, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_faligndata(dx0, dx1);
	dpy = (mlib_d64 *)vis_alignaddr(py, off);
	dy0 = vis_ld_d64_nf(dpy);
	dy1 = vis_ld_d64_nf(dpy + 1);
	dy = vis_faligndata(dy0, dy1);
	MUL_S16_S8_SAT;
	PACK_S16_S8_MOD;
/* store first bytes of result */
	vis_pst_8(dr, dpz, emask);
	dpz++;
	len = dpzend - dpz;

	if (len < 0)
		return (MLIB_SUCCESS);
	dpx++;
	dpy++;

	if ((((mlib_addr)x ^ (mlib_addr)z) & 7) == 0) {
		if ((((mlib_addr)x ^ (mlib_addr)y) & 7) == 0) {
#pragma pipeloop(0)
			for (i = 0; i < len; i++) {
				dx = dpx[i];
				dy = dpy[i];
				MUL_S16_S8_SAT;
				PACK_S16_S8_MOD;
				dpz[i] = dr;
			}

			dpz = dpzend;
			dx = dpx[len];
			dy = dpy[len];
			MUL_S16_S8_SAT;
		} else {
#pragma pipeloop(0)
			for (i = 0; i < len; i++) {
				dx = dpx[i];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S16_S8_SAT;
				PACK_S16_S8_MOD;
				dpz[i] = dr;
			}

			dpz = dpzend;
			dx = dpx[len];
			dy0 = dpy[len];
			dy1 = vis_ld_d64_nf(dpy + len + 1);
			dy = vis_faligndata(dy0, dy1);
			MUL_S16_S8_SAT;
		}
	} else {
		px += 8 - ((mlib_addr)z & 7);

		for (k = 0; k < (len >> 8); k++) {
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)px + k * 256), 256 * 8);
			(void *)vis_alignaddr(py, off);
#pragma pipeloop(0)
			for (i = 0; i < 256; i++) {
				dx = x_buff[i];
				dy0 = dpy[k * 256 + i];
				dy1 = vis_ld_d64_nf(dpy + k * 256 + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S16_S8_SAT;
				PACK_S16_S8_MOD;
				dpz[256 * k + i] = dr;
			}
		}

		N = len & ~255;
		__mlib_VectorCopy_U8((void *)x_buff,
			(void *)((mlib_d64 *)px + N), (len - N) * 8 +
			1 + ((mlib_addr)pzend & 7));
		vis_alignaddr(py, off);
#pragma pipeloop(0)
		for (i = N; i < len; i++) {
			dx = x_buff[i - N];
			dy0 = dpy[i];
			dy1 = vis_ld_d64_nf(dpy + i + 1);
			dy = vis_faligndata(dy0, dy1);
			MUL_S16_S8_SAT;
			PACK_S16_S8_MOD;
			dpz[i] = dr;
		}

		dpz = dpzend;
		dx = x_buff[len - N];
		dy0 = vis_ld_d64_nf(dpy + len);
		dy1 = vis_ld_d64_nf(dpy + len + 1);
		dy = vis_faligndata(dy0, dy1);
		MUL_S16_S8_SAT;
	}

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
		PACK_S16_S8_MOD;
/* prepare edge mask for the last bytes */
		emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
		vis_pst_8(dr, dpz, emask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	EXPAND_U8C_FOR_MUL                                      \
	dr2 = vis_faligndata(dx, dx);                           \
	/*                                                      \
	 * x_r                                                  \
	 */                                                     \
	dy = vis_fpmerge(vis_read_hi(dy), vis_read_lo(dy));     \
	dy = vis_fpmerge(vis_read_hi(dy), vis_read_lo(dy));     \
	dr3 = vis_fexpand(vis_read_hi(dy));                     \
	/*                                                      \
	 * y_r * 16                                             \
	 */                                                     \
	                                                        \
	/* y_i * 16 */                                          \
	dy = vis_fexpand(vis_read_lo(dy))

/* *********************************************************** */

#define	EXPAND_U8C_FOR_MUL_8                                    \
	dr2 = vis_faligndata(dx, dx);                           \
	/*                                                      \
	 * x_r                                                  \
	 */                                                     \
	dy = vis_fpmerge(vis_read_hi(dy), vis_read_lo(dy));     \
	dy = vis_fpmerge(vis_read_hi(dy), vis_read_lo(dy));     \
	dr3 = vis_fpmerge(vis_fzeros(), vis_read_hi(dy));       \
	dy = vis_fpmerge(vis_fzeros(), vis_read_lo(dy))

/* *********************************************************** */

#define	MUL_U8C_S16C                                                     \
	{                                                                \
	    mlib_d64 dr4, dr5, dr6, dr7, dr8;                            \
	                                                                 \
	    dr1 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dr3));    \
	    dr5 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dr3));    \
	    dr = vis_fmuld8ulx16(vis_read_hi(dr2), vis_read_hi(dr3));    \
	    dr3 = vis_fmuld8ulx16(vis_read_lo(dr2), vis_read_lo(dr3));   \
	    dr6 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dy));     \
	    dr7 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dy));     \
	    dr8 = vis_fmuld8ulx16(vis_read_hi(dr2), vis_read_hi(dy));    \
	    dr4 = vis_fmuld8ulx16(vis_read_lo(dr2), vis_read_lo(dy));    \
	    dr = vis_fpsub32(dr, dr6);                                   \
	    dr2 = vis_fpadd32(dr1, dr8);                                 \
	    dr1 = vis_fpsub32(dr3, dr7);                                 \
	    dr3 = vis_fpadd32(dr5, dr4);                                 \
	    dr = vis_freg_pair(vis_fpackfix(dr), vis_fpackfix(dr1));     \
	    dr1 = vis_freg_pair(vis_fpackfix(dr2), vis_fpackfix(dr3));   \
	}

/* *********************************************************** */

#define	PACK_S16C_U8C0                                          \
	dr = vis_fpack16_to_hi(dr, dr);                         \
	dr = vis_fpack16_to_lo(dr, dr1);                        \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr))

/* *********************************************************** */

#define	PACK_S16C_U8C0_8                                         \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));      \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));      \
	dr1 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));   \
	dr1 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));   \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1))

/* *********************************************************** */

#define	PACK_S16C_U8C1                                          \
	dr = vis_fpack16_to_hi(dr, dr);                         \
	dr = vis_fpack16_to_lo(dr, dr1);                        \
	dr1 = vis_faligndata(dr, dr);                           \
	dr = vis_fpmerge(vis_read_lo(dr1), vis_read_hi(dr))

/* *********************************************************** */

#define	EXPAND_U8C_FOR_0_MUL                                           \
	{                                                              \
	    mlib_d64 dr22;                                             \
	    mlib_d64 fhi, flo;                                         \
	                                                               \
	    dr2 = dr22 = vis_fand(dx, clear);                          \
	    dr22 = vis_fpack32(dr22, dzero);                           \
	    dr2 = vis_for(dr2, dr22);                                  \
	    fhi = vis_fpmerge(vis_read_hi(dr2), vis_read_hi(dr2));     \
	    flo = vis_fpmerge(vis_read_lo(dr2), vis_read_lo(dr2));     \
	    dr2 = vis_freg_pair(vis_read_hi(fhi), vis_read_hi(flo));   \
	    dy = vis_fpmerge(vis_read_hi(dy), vis_read_lo(dy));        \
	    dy = vis_fpmerge(vis_read_hi(dy), vis_read_lo(dy));        \
	    dr3 = vis_fexpand(vis_read_hi(dy));                        \
	/*                                                             \
	 * y_r * 16                                                    \
	 */                                                            \
	    dy = vis_fexpand(vis_read_lo(dy));                         \
	/*                                                             \
	 * y_i * 16                                                    \
	 */                                                            \
	}

/* *********************************************************** */

#define	EXPAND_U8C_FOR_0_MUL_8                                         \
	{                                                              \
	    mlib_d64 dr22;                                             \
	    mlib_d64 fhi, flo;                                         \
	                                                               \
	    dr2 = dr22 = vis_fand(dx, clear);                          \
	    dr22 = vis_fpack32(dr22, dzero);                           \
	    dr2 = vis_for(dr2, dr22);                                  \
	    fhi = vis_fpmerge(vis_read_hi(dr2), vis_read_hi(dr2));     \
	    flo = vis_fpmerge(vis_read_lo(dr2), vis_read_lo(dr2));     \
	    dr2 = vis_freg_pair(vis_read_hi(fhi), vis_read_hi(flo));   \
	    dy = vis_fpmerge(vis_read_hi(dy), vis_read_lo(dy));        \
	    dy = vis_fpmerge(vis_read_hi(dy), vis_read_lo(dy));        \
	    dr3 = vis_fpmerge(vis_fzeros(), vis_read_hi(dy));          \
	    dy = vis_fpmerge(vis_fzeros(), vis_read_lo(dy));           \
	}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_U8C_U8C_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
/* edge masks */
	mlib_s32 emask, len, i;
	mlib_s32 gsr_for_mul = (12 << 3) | 7;
	mlib_s32 gsr_for_mul_8 = (15 << 3) | 7;

/* offset of address alignment in destination */
	mlib_s32 gsr_for_pack = ((7 - shift) << 3) | 7, off;

/* offset of address alignment in destination */
	mlib_s32 gsr_for_pack_8 = (0 << 3) | 7;
	mlib_u8 *pzend, *px = (void *)x;
	mlib_d64 *dpz, *dpx, *dpy, *dpzend;
	mlib_d64 dx, dy, dr, dr1, dr2, dr3, dx0, dx1, dy0, dy1;
	mlib_d64 clear = vis_to_double_dup(0xFF00FF00);
	mlib_d64 dzero = vis_fzero();
	mlib_s32 N, k;
	mlib_d64 x_buff[256], r_buff[256];

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

	if (shift == 8) {
/*
 * prepare the destination address
 */
		pzend = (mlib_u8 *)z + n + n - 1;
		dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));

		dpz = (mlib_d64 *)((mlib_addr)z & (~7));
		off = (mlib_addr)dpz - (mlib_addr)z + (1 & (mlib_addr)z);

/*
 * generate edge mask for the start point
 */
		emask = vis_edge8(z, pzend);

/*
 * prepare the source address
 */
		dpy = (mlib_d64 *)vis_alignaddr((void *)y, off);
		dy0 = vis_ld_d64_nf(dpy);
		dy1 = vis_ld_d64_nf(dpy + 1);
		dy = vis_faligndata(dy0, dy1);
		dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
/* initialize GSR */
		vis_write_gsr(gsr_for_mul_8);
		EXPAND_U8C_FOR_MUL_8;
		MUL_U8C_S16C;
		if (!(1 & (mlib_addr)z)) {
			vis_write_gsr(gsr_for_pack_8);
			PACK_S16C_U8C0;
			vis_pst_8(dr, dpz, emask);
			dpz++;
			dpx++;
			dpy++;
			len = dpzend - dpz;

			if (len < 0)
				return (MLIB_SUCCESS);

			if ((((mlib_addr)x ^ (mlib_addr)z) & 7) == 0) {
				if ((((mlib_addr)x ^ (mlib_addr)y) & 7) == 0) {

					for (k = 0; k < (len >> 8); k++) {
/* initialize GSR */
						vis_write_gsr(gsr_for_mul_8);
#pragma pipeloop(0)
						for (i = 0; i < 256; i++) {
							dx = dpx[256 * k + i];
							dy = dpy[256 * k + i];
							EXPAND_U8C_FOR_0_MUL_8;
							MUL_U8C_S16C;
							x_buff[i] = dr1;
							r_buff[i] = dr;
						}

						vis_write_gsr(gsr_for_pack_8);
#pragma pipeloop(0)
						for (i = 0; i < 256; i++) {
							dr1 = x_buff[i];
							dr = r_buff[i];
							PACK_S16C_U8C0;
							dpz[256 * k + i] = dr;
						}
					}

					N = len & ~255;
/* initialize GSR */
					vis_write_gsr(gsr_for_mul_8);
#pragma pipeloop(0)
					for (i = 0; i < len - N; i++) {
						dx = dpx[i + N];
						dy = dpy[i + N];
						EXPAND_U8C_FOR_0_MUL_8;
						MUL_U8C_S16C;
						x_buff[i] = dr1;
						r_buff[i] = dr;
					}

					vis_write_gsr(gsr_for_pack_8);
#pragma pipeloop(0)
					for (i = 0; i < len - N; i++) {
						dr1 = x_buff[i];
						dr = r_buff[i];
						PACK_S16C_U8C0;
						dpz[i + N] = dr;
					}

					dpx += len;
					dpy += len;
					dpz += len;
					dx = vis_ld_d64_nf(dpx);
					dy = vis_ld_d64_nf(dpy);
				} else {
					for (k = 0; k < (len >> 8); k++) {
/* initialize GSR */
						vis_write_gsr(gsr_for_mul_8);
						(void *)vis_alignaddr((void *)y,
							off);
#pragma pipeloop(0)
						for (i = 0; i < 256; i++) {
							dx = dpx[256 * k + i];
							dy0 = dpy[256 * k + i];
							dy1 = vis_ld_d64_nf(
								dpy + 256 * k +
								i + 1);
							dy = vis_faligndata(dy0,
								dy1);
							EXPAND_U8C_FOR_0_MUL_8;
							MUL_U8C_S16C;
							x_buff[i] = dr1;
							r_buff[i] = dr;
						}

						vis_write_gsr(gsr_for_pack_8);
#pragma pipeloop(0)
						for (i = 0; i < 256; i++) {
							dr1 = x_buff[i];
							dr = r_buff[i];
							PACK_S16C_U8C0;
							dpz[256 * k + i] = dr;
						}
					}

					N = len & ~255;
/* initialize GSR */
					vis_write_gsr(gsr_for_mul_8);
					(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
					for (i = 0; i < len - N; i++) {
						dx = dpx[i + N];
						dy0 = dpy[i + N];
						dy1 = vis_ld_d64_nf(dpy +
							i + N + 1);
						dy = vis_faligndata(dy0, dy1);
						EXPAND_U8C_FOR_0_MUL_8;
						MUL_U8C_S16C;
						x_buff[i] = dr1;
						r_buff[i] = dr;
					}

					vis_write_gsr(gsr_for_pack_8);
#pragma pipeloop(0)
					for (i = 0; i < len - N; i++) {
						dr1 = x_buff[i];
						dr = r_buff[i];
						PACK_S16C_U8C0;
						dpz[i + N] = dr;
					}

					dpx += len;
					dpy += len;
					dpz += len;
					dx = vis_ld_d64_nf(dpx);
					(void *)vis_alignaddr((void *)y, off);
					dy0 = vis_ld_d64_nf(dpy);
					dy1 = vis_ld_d64_nf(dpy + 1);
					dy = vis_faligndata(dy0, dy1);
				}
			} else {
				px += 8 - ((mlib_addr)z & 7);

				for (k = 0; k < (len >> 8); k++) {
					__mlib_VectorCopy_U8((void *)x_buff,
						(void *)((mlib_d64 *)px +
						k * 256), 256 * 8);
/* initialize GSR */
					vis_write_gsr(gsr_for_mul_8);
					(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
					for (i = 0; i < 256; i++) {
						dx = x_buff[i];
						dy0 = dpy[256 * k + i];
						dy1 = vis_ld_d64_nf(dpy +
							256 * k + i + 1);
						dy = vis_faligndata(dy0, dy1);
						EXPAND_U8C_FOR_0_MUL_8;
						MUL_U8C_S16C;
						x_buff[i] = dr1;
						r_buff[i] = dr;
					}

					vis_write_gsr(gsr_for_pack_8);
#pragma pipeloop(0)
					for (i = 0; i < 256; i++) {
						dr1 = x_buff[i];
						dr = r_buff[i];
						PACK_S16C_U8C0;
						dpz[256 * k + i] = dr;
					}
				}

				N = len & ~255;
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + N),
					(len - N) * 8 + 1 +
					((mlib_addr)pzend & 7));
/* initialize GSR */
				vis_write_gsr(gsr_for_mul_8);
				(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
				for (i = 0; i < len - N; i++) {
					dx = x_buff[i];
					dy0 = dpy[i + N];
					dy1 = vis_ld_d64_nf(dpy + i + N + 1);
					dy = vis_faligndata(dy0, dy1);
					EXPAND_U8C_FOR_0_MUL_8;
					MUL_U8C_S16C;
					x_buff[i] = dr1;
					r_buff[i] = dr;
				}

				vis_write_gsr(gsr_for_pack_8);
#pragma pipeloop(0)
				for (i = 0; i < len - N; i++) {
					dr1 = x_buff[i];
					dr = r_buff[i];
					PACK_S16C_U8C0;
					dpz[i + N] = dr;
				}

				dpz = dpzend;
				dx = x_buff[len - N];
				vis_alignaddr((void *)y, off);
				dy0 = vis_ld_d64_nf(dpy + len);
				dy1 = vis_ld_d64_nf(dpy + len + 1);
				dy = vis_faligndata(dy0, dy1);
			}

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
/* initialize GSR */
				vis_write_gsr(gsr_for_mul_8);
				EXPAND_U8C_FOR_MUL_8;
				MUL_U8C_S16C;
/* initialize GSR */
				vis_write_gsr(gsr_for_pack_8);
				PACK_S16C_U8C0;
/* prepare edge mask for the last bytes */
				emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
				vis_pst_8(dr, dpz, emask);
			}

			return (MLIB_SUCCESS);
		}

		dpx++;
		dpy++;
/* initialize GSR */
		vis_write_gsr(gsr_for_pack_8);
		PACK_S16C_U8C1;
		vis_pst_8(dr, dpz, emask);

		if ((mlib_addr)++dpz <= (mlib_addr)pzend) {
			vis_pst_8(dr1, dpz, 0x80);
		}

		px += 9 - ((mlib_addr)z & 7);
		len = dpzend - dpz;

		if (len < 0)
			return (MLIB_SUCCESS);

		for (k = 0; k < (len >> 8); k++) {
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)px + k * 256), 256 * 8);
/* initialize GSR */
			vis_write_gsr(gsr_for_mul_8);
			(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
			for (i = 0; i < 256; i++) {
				dx = x_buff[i];
				dy0 = dpy[256 * k + i];
				dy1 = vis_ld_d64_nf(dpy + 256 * k + i + 1);
				dy = vis_faligndata(dy0, dy1);
				EXPAND_U8C_FOR_0_MUL_8;
				MUL_U8C_S16C;
				x_buff[i] = dr1;
				r_buff[i] = dr;
			}

			vis_write_gsr(gsr_for_pack_8);
#pragma pipeloop(0)
			for (i = 0; i < 256; i++) {
				dr1 = x_buff[i];
				dr = r_buff[i];
				PACK_S16C_U8C1;
				vis_pst_8(dr, dpz + 256 * k + i, 0x7f);
				vis_pst_8(dr1, dpz + 256 * k + i + 1, 0x80);
			}
		}

		N = len & ~255;
		__mlib_VectorCopy_U8((void *)x_buff,
			(void *)((mlib_d64 *)px + N), (len - N) * 8 +
			((mlib_addr)pzend & 7));
/* initialize GSR */
		vis_write_gsr(gsr_for_mul_8);
		(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
		for (i = 0; i < len - N; i++) {
			dx = x_buff[i];
			dy0 = dpy[i + N];
			dy1 = vis_ld_d64_nf(dpy + i + N + 1);
			dy = vis_faligndata(dy0, dy1);
			EXPAND_U8C_FOR_0_MUL_8;
			MUL_U8C_S16C;
			x_buff[i] = dr1;
			r_buff[i] = dr;
		}

		vis_write_gsr(gsr_for_pack_8);
#pragma pipeloop(0)
		for (i = 0; i < len - N; i++) {
			dr1 = x_buff[i];
			dr = r_buff[i];
			PACK_S16C_U8C1;
			vis_pst_8(dr, dpz + i + N, 0x7f);
			vis_pst_8(dr1, dpz + i + N + 1, 0x80);
		}

		dpz = dpzend;
		dx = x_buff[len - N];
		vis_alignaddr((void *)y, off);
		dy0 = vis_ld_d64_nf(dpy + len);
		dy1 = vis_ld_d64_nf(dpy + len + 1);
		dy = vis_faligndata(dy0, dy1);

		if ((mlib_addr)dpz < (mlib_addr)pzend) {
/* initialize GSR */
			vis_write_gsr(gsr_for_mul_8);
			EXPAND_U8C_FOR_MUL_8;
			MUL_U8C_S16C;
/* initialize GSR */
			vis_write_gsr(gsr_for_pack_8);
			PACK_S16C_U8C1;
/* prepare edge mask for the last bytes */
			emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
			vis_pst_8(dr, dpz, emask & 0x7f);

			if ((mlib_addr)++dpz <= (mlib_addr)pzend) {
				vis_pst_8(dr1, dpz, 0x80);
			}
		}

	} else {
/*  shift < 8  */

/*
 * prepare the destination address
 */
		pzend = (mlib_u8 *)z + n + n - 1;
		dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));

		dpz = (mlib_d64 *)((mlib_addr)z & (~7));
		off = (mlib_addr)dpz - (mlib_addr)z + (1 & (mlib_addr)z);

/*
 * generate edge mask for the start point
 */
		emask = vis_edge8(z, pzend);

/*
 * prepare the source address
 */
		dpy = (mlib_d64 *)vis_alignaddr((void *)y, off);
		dy0 = vis_ld_d64_nf(dpy);
		dy1 = vis_ld_d64_nf(dpy + 1);
		dy = vis_faligndata(dy0, dy1);
		dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
/* initialize GSR */
		vis_write_gsr(gsr_for_mul);
		EXPAND_U8C_FOR_MUL;
		MUL_U8C_S16C;
		if (!(1 & (mlib_addr)z)) {
			vis_write_gsr(gsr_for_pack);
			PACK_S16C_U8C0;
			vis_pst_8(dr, dpz, emask);
			dpz++;
			dpx++;
			dpy++;
			len = dpzend - dpz;

			if (len < 0)
				return (MLIB_SUCCESS);

			if ((((mlib_addr)x ^ (mlib_addr)z) & 7) == 0) {
				if ((((mlib_addr)x ^ (mlib_addr)y) & 7) == 0) {

					for (k = 0; k < (len >> 8); k++) {
/* initialize GSR */
						vis_write_gsr(gsr_for_mul);
#pragma pipeloop(0)
						for (i = 0; i < 256; i++) {
							dx = dpx[256 * k + i];
							dy = dpy[256 * k + i];
							EXPAND_U8C_FOR_0_MUL;
							MUL_U8C_S16C;
							x_buff[i] = dr1;
							r_buff[i] = dr;
						}

						vis_write_gsr(gsr_for_pack);
#pragma pipeloop(0)
						for (i = 0; i < 256; i++) {
							dr1 = x_buff[i];
							dr = r_buff[i];
							PACK_S16C_U8C0;
							dpz[256 * k + i] = dr;
						}
					}

					N = len & ~255;
/* initialize GSR */
					vis_write_gsr(gsr_for_mul);
#pragma pipeloop(0)
					for (i = 0; i < len - N; i++) {
						dx = dpx[i + N];
						dy = dpy[i + N];
						EXPAND_U8C_FOR_0_MUL;
						MUL_U8C_S16C;
						x_buff[i] = dr1;
						r_buff[i] = dr;
					}

					vis_write_gsr(gsr_for_pack);
#pragma pipeloop(0)
					for (i = 0; i < len - N; i++) {
						dr1 = x_buff[i];
						dr = r_buff[i];
						PACK_S16C_U8C0;
						dpz[i + N] = dr;
					}

					dpx += len;
					dpy += len;
					dpz += len;
					dx = dpx[0];
					dy = dpy[0];
				} else {
					for (k = 0; k < (len >> 8); k++) {
/* initialize GSR */
						vis_write_gsr(gsr_for_mul);
						(void *)vis_alignaddr((void *)y,
							off);
#pragma pipeloop(0)
						for (i = 0; i < 256; i++) {
							dx = dpx[256 * k + i];
							dy0 = dpy[256 * k + i];
							dy1 = vis_ld_d64_nf(
								dpy + 256 * k +
								i + 1);
							dy = vis_faligndata(dy0,
								dy1);
							EXPAND_U8C_FOR_0_MUL;
							MUL_U8C_S16C;
							x_buff[i] = dr1;
							r_buff[i] = dr;
						}

						vis_write_gsr(gsr_for_pack);
#pragma pipeloop(0)
						for (i = 0; i < 256; i++) {
							dr1 = x_buff[i];
							dr = r_buff[i];
							PACK_S16C_U8C0;
							dpz[256 * k + i] = dr;
						}
					}

					N = len & ~255;
/* initialize GSR */
					vis_write_gsr(gsr_for_mul);
					(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
					for (i = 0; i < len - N; i++) {
						dx = dpx[i + N];
						dy0 = dpy[i + N];
						dy1 = vis_ld_d64_nf(dpy + i +
							N + 1);
						dy = vis_faligndata(dy0, dy1);
						EXPAND_U8C_FOR_0_MUL;
						MUL_U8C_S16C;
						x_buff[i] = dr1;
						r_buff[i] = dr;
					}

					vis_write_gsr(gsr_for_pack);
#pragma pipeloop(0)
					for (i = 0; i < len - N; i++) {
						dr1 = x_buff[i];
						dr = r_buff[i];
						PACK_S16C_U8C0;
						dpz[i + N] = dr;
					}

					dpx += len;
					dpy += len;
					dpz += len;
					dx = dpx[0];
					(void *)vis_alignaddr((void *)y, off);
					dy0 = vis_ld_d64_nf(dpy);
					dy1 = vis_ld_d64_nf(dpy + 1);
					dy = vis_faligndata(dy0, dy1);
				}
			} else {
				px += 8 - ((mlib_addr)z & 7);

				for (k = 0; k < (len >> 8); k++) {
					__mlib_VectorCopy_U8((void *)x_buff,
						(void *)((mlib_d64 *)px +
						k * 256), 256 * 8);
/* initialize GSR */
					vis_write_gsr(gsr_for_mul);
					(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
					for (i = 0; i < 256; i++) {
						dx = x_buff[i];
						dy0 = dpy[256 * k + i];
						dy1 = vis_ld_d64_nf(dpy +
							256 * k + i + 1);
						dy = vis_faligndata(dy0, dy1);
						EXPAND_U8C_FOR_0_MUL;
						MUL_U8C_S16C;
						x_buff[i] = dr1;
						r_buff[i] = dr;
					}

					vis_write_gsr(gsr_for_pack);
#pragma pipeloop(0)
					for (i = 0; i < 256; i++) {
						dr1 = x_buff[i];
						dr = r_buff[i];
						PACK_S16C_U8C0;
						dpz[256 * k + i] = dr;
					}
				}

				N = len & ~255;
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + N),
					(len - N) * 8 + 1 +
					((mlib_addr)pzend & 7));
/* initialize GSR */
				vis_write_gsr(gsr_for_mul);
				(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
				for (i = 0; i < len - N; i++) {
					dx = x_buff[i];
					dy0 = dpy[i + N];
					dy1 = vis_ld_d64_nf(dpy + i + N + 1);
					dy = vis_faligndata(dy0, dy1);
					EXPAND_U8C_FOR_0_MUL;
					MUL_U8C_S16C;
					x_buff[i] = dr1;
					r_buff[i] = dr;
				}

				vis_write_gsr(gsr_for_pack);
#pragma pipeloop(0)
				for (i = 0; i < len - N; i++) {
					dr1 = x_buff[i];
					dr = r_buff[i];
					PACK_S16C_U8C0;
					dpz[i + N] = dr;
				}

				dpz = dpzend;
				dx = x_buff[len - N];
				vis_alignaddr((void *)y, off);
				dy0 = vis_ld_d64_nf(dpy + len);
				dy1 = vis_ld_d64_nf(dpy + len + 1);
				dy = vis_faligndata(dy0, dy1);
			}

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
/* initialize GSR */
				vis_write_gsr(gsr_for_mul);
				EXPAND_U8C_FOR_MUL;
				MUL_U8C_S16C;
/* initialize GSR */
				vis_write_gsr(gsr_for_pack);
				PACK_S16C_U8C0;
/* prepare edge mask for the last bytes */
				emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
				vis_pst_8(dr, dpz, emask);
			}

			return (MLIB_SUCCESS);
		}

		dpx++;
		dpy++;
/* initialize GSR */
		vis_write_gsr(gsr_for_pack);
		PACK_S16C_U8C1;
		vis_pst_8(dr, dpz, emask);

		if ((mlib_addr)++dpz <= (mlib_addr)pzend) {
			vis_pst_8(dr1, dpz, 0x80);
		}

		px += 9 - ((mlib_addr)z & 7);
		len = dpzend - dpz;

		if (len < 0)
			return (MLIB_SUCCESS);

		for (k = 0; k < (len >> 8); k++) {
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)px + k * 256), 256 * 8);
/* initialize GSR */
			vis_write_gsr(gsr_for_mul);
			(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
			for (i = 0; i < 256; i++) {
				dx = x_buff[i];
				dy0 = dpy[256 * k + i];
				dy1 = vis_ld_d64_nf(dpy + 256 * k + i + 1);
				dy = vis_faligndata(dy0, dy1);
				EXPAND_U8C_FOR_0_MUL;
				MUL_U8C_S16C;
				x_buff[i] = dr1;
				r_buff[i] = dr;
			}

			vis_write_gsr(gsr_for_pack);
#pragma pipeloop(0)
			for (i = 0; i < 256; i++) {
				dr1 = x_buff[i];
				dr = r_buff[i];
				PACK_S16C_U8C1;
				vis_pst_8(dr, dpz + 256 * k + i, 0x7f);
				vis_pst_8(dr1, dpz + 256 * k + i + 1, 0x80);
			}
		}

		N = len & ~255;
		__mlib_VectorCopy_U8((void *)x_buff,
			(void *)((mlib_d64 *)px + N), (len - N) * 8 +
			((mlib_addr)pzend & 7));
/* initialize GSR */
		vis_write_gsr(gsr_for_mul);
		(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
		for (i = 0; i < len - N; i++) {
			dx = x_buff[i];
			dy0 = dpy[i + N];
			dy1 = vis_ld_d64_nf(dpy + i + N + 1);
			dy = vis_faligndata(dy0, dy1);
			EXPAND_U8C_FOR_0_MUL;
			MUL_U8C_S16C;
			x_buff[i] = dr1;
			r_buff[i] = dr;
		}

		vis_write_gsr(gsr_for_pack);
#pragma pipeloop(0)
		for (i = 0; i < len - N; i++) {
			dr1 = x_buff[i];
			dr = r_buff[i];
			PACK_S16C_U8C1;
			vis_pst_8(dr, dpz + i + N, 0x7f);
			vis_pst_8(dr1, dpz + i + N + 1, 0x80);
		}

		dpz = dpzend;
		dx = x_buff[len - N];
		vis_alignaddr((void *)y, off);
		dy0 = vis_ld_d64_nf(dpy + len);
		dy1 = vis_ld_d64_nf(dpy + len + 1);
		dy = vis_faligndata(dy0, dy1);

		if ((mlib_addr)dpz < (mlib_addr)pzend) {
/* initialize GSR */
			vis_write_gsr(gsr_for_mul);
			EXPAND_U8C_FOR_MUL;
			MUL_U8C_S16C;
/* initialize GSR */
			vis_write_gsr(gsr_for_pack);
			PACK_S16C_U8C1;
/* prepare edge mask for the last bytes */
			emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
			vis_pst_8(dr, dpz, emask & 0x7f);

			if ((mlib_addr)++dpz <= (mlib_addr)pzend) {
				vis_pst_8(dr1, dpz, 0x80);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	EXPAND_U8C_FOR_MUL_MOD                                  \
	dr2 = vis_faligndata(dx, dx);                           \
	/*                                                      \
	 * x_r                                                  \
	 */                                                     \
	dy = vis_fpmerge(vis_read_hi(dy), vis_read_lo(dy));     \
	dy = vis_fpmerge(vis_read_hi(dy), vis_read_lo(dy));     \
	dr3 = vis_fpmerge(vis_fzeros(), vis_read_hi(dy));       \
	dy = vis_fpmerge(vis_fzeros(), vis_read_lo(dy))

/* *********************************************************** */

#define	PACK_S16C_U8C0_MOD                                      \
	dr = vis_fpack32(dr, dzero);                            \
	dr = vis_for(dr, dr1)

/* *********************************************************** */

#define	PACK_S16C_U8C1_MOD                                       \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));      \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));      \
	dr1 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));   \
	dr1 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));   \
	dr = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr1));     \
	dr1 = vis_faligndata(dr, dr)

/* *********************************************************** */

#define	EXPAND_U8C_FOR_0_MUL_MOD                                       \
	{                                                              \
	    mlib_d64 dr22;                                             \
	    mlib_d64 fhi, flo;                                         \
	                                                               \
	    dr2 = dr22 = vis_fand(dx, clear);                          \
	    dr22 = vis_fpack32(dr22, dzero);                           \
	    dr2 = vis_for(dr2, dr22);                                  \
	    fhi = vis_fpmerge(vis_read_hi(dr2), vis_read_hi(dr2));     \
	    flo = vis_fpmerge(vis_read_lo(dr2), vis_read_lo(dr2));     \
	    dr2 = vis_freg_pair(vis_read_hi(fhi), vis_read_hi(flo));   \
	    dy = vis_fpmerge(vis_read_hi(dy), vis_read_lo(dy));        \
	    dy = vis_fpmerge(vis_read_hi(dy), vis_read_lo(dy));        \
	    dr3 = vis_fpmerge(vis_fzeros(), vis_read_hi(dy));          \
	    dy = vis_fpmerge(vis_fzeros(), vis_read_lo(dy));           \
	}

/* *********************************************************** */

#define	MUL_U8C_S16C_MOD                                                 \
	{                                                                \
	    mlib_d64 dr4, dr5, dr6, dr7, dr8;                            \
	                                                                 \
	    dr1 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dr3));    \
	    dr5 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dr3));    \
	    dr = vis_fmuld8ulx16(vis_read_hi(dr2), vis_read_hi(dr3));    \
	    dr3 = vis_fmuld8ulx16(vis_read_lo(dr2), vis_read_lo(dr3));   \
	    dr6 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dy));     \
	    dr7 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dy));     \
	    dr8 = vis_fmuld8ulx16(vis_read_hi(dr2), vis_read_hi(dy));    \
	    dr4 = vis_fmuld8ulx16(vis_read_lo(dr2), vis_read_lo(dy));    \
	    dr = vis_fpsub32(dr, dr6);                                   \
	    dr2 = vis_fpadd32(dr1, dr8);                                 \
	    dr1 = vis_fpsub32(dr3, dr7);                                 \
	    dr3 = vis_fpadd32(dr5, dr4);                                 \
	    dr = vis_fand(dr, mod_mask);                                 \
	    dr1 = vis_fand(dr1, mod_mask);                               \
	    dr2 = vis_fand(dr2, mod_mask);                               \
	    dr3 = vis_fand(dr3, mod_mask);                               \
	    dr = vis_freg_pair(vis_fpackfix(dr), vis_fpackfix(dr1));     \
	    dr1 = vis_freg_pair(vis_fpackfix(dr2), vis_fpackfix(dr3));   \
	}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
/* edge masks */
	mlib_s32 emask, len, i;
	mlib_s32 gsr_for_mul = ((16 - shift) << 3) | 7;
	mlib_u8 *pzend, *px = (void *)x;
	mlib_d64 *dpz, *dpx, *dpy, *dpzend;
	mlib_d64 dx, dy, dr, dr1, dr2, dr3, dx0, dx1, dy0, dy1;
	mlib_d64 clear = vis_to_double_dup(0xFF00FF00);
	mlib_d64 dzero = vis_fzero();
	mlib_s32 N, k, off;
	mlib_d64 x_buff[256], r_buff[256];
	mlib_d64 mod_mask = vis_to_double_dup(0xFF << shift);

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

/*
 * prepare the destination address
 */
	pzend = (mlib_u8 *)z + n + n - 1;
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));

	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z + (1 & (mlib_addr)z);

/*
 * generate edge mask for the start point
 */
	emask = vis_edge8(z, pzend);

/*
 * prepare the source address
 */
	dpy = (mlib_d64 *)vis_alignaddr((void *)y, off);
	dy0 = vis_ld_d64_nf(dpy);
	dy1 = vis_ld_d64_nf(dpy + 1);
	dy = vis_faligndata(dy0, dy1);
	dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_faligndata(dx0, dx1);
/* initialize GSR */
	vis_write_gsr(gsr_for_mul);
	EXPAND_U8C_FOR_MUL_MOD;
	MUL_U8C_S16C_MOD;
	if (!(1 & (mlib_addr)z)) {
		PACK_S16C_U8C0_MOD;
		vis_pst_8(dr, dpz, emask);
		dpz++;
		dpx++;
		dpy++;
		len = dpzend - dpz;

		if (len < 0)
			return (MLIB_SUCCESS);

		if ((((mlib_addr)x ^ (mlib_addr)z) & 7) == 0) {
			if ((((mlib_addr)x ^ (mlib_addr)y) & 7) == 0) {

#pragma pipeloop(0)
				for (i = 0; i < len; i++) {
					dx = dpx[i];
					dy = dpy[i];
					EXPAND_U8C_FOR_0_MUL_MOD;
					MUL_U8C_S16C_MOD;
					PACK_S16C_U8C0_MOD;
					dpz[i] = dr;
				}

				dpx += len;
				dpy += len;
				dpz += len;
				dx = vis_ld_d64_nf(dpx);
				dy = vis_ld_d64_nf(dpy);

			} else {

/* initialize GSR */
				vis_write_gsr(gsr_for_mul);
				vis_alignaddr((void *)y, off);
				dy0 = vis_ld_d64_nf(dpy);
#pragma pipeloop(0)
				for (i = 0; i < len; i++) {
					dx = dpx[i];
					dy1 = vis_ld_d64_nf(dpy + i + 1);
					dy = vis_faligndata(dy0, dy1);
					dy0 = dy1;
					EXPAND_U8C_FOR_0_MUL_MOD;
					MUL_U8C_S16C_MOD;
					PACK_S16C_U8C0_MOD;
					dpz[i] = dr;
				}

				dpx += len;
				dpy += len;
				dpz += len;
				dx = dpx[0];
				vis_alignaddr((void *)y, off);
				dy1 = vis_ld_d64_nf(dpy + 1);
				dy = vis_faligndata(dy0, dy1);

			}
		} else {
			px += 8 - ((mlib_addr)z & 7);

			for (k = 0; k < (len >> 8); k++) {
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + k * 256),
					256 * 8);
/* initialize GSR */
				vis_write_gsr(gsr_for_mul);
				vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = x_buff[i];
					dy0 = dpy[256 * k + i];
					dy1 = vis_ld_d64_nf(dpy + 256 * k +
						i + 1);
					dy = vis_faligndata(dy0, dy1);
					EXPAND_U8C_FOR_0_MUL_MOD;
					MUL_U8C_S16C_MOD;
					PACK_S16C_U8C0_MOD;
					dpz[256 * k + i] = dr;
				}

			}

			N = len & ~255;
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)px + N),
				(len - N) * 8 + 1 + ((mlib_addr)pzend & 7));
/* initialize GSR */
			vis_write_gsr(gsr_for_mul);
			vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
			for (i = 0; i < len - N; i++) {
				dx = x_buff[i];
				dy0 = dpy[i + N];
				dy1 = vis_ld_d64_nf(dpy + i + N + 1);
				dy = vis_faligndata(dy0, dy1);
				EXPAND_U8C_FOR_0_MUL_MOD;
				MUL_U8C_S16C_MOD;
				PACK_S16C_U8C0_MOD;
				dpz[i + N] = dr;
			}

			dpz = dpzend;
			dx = x_buff[len - N];
			vis_alignaddr((void *)y, off);
			dy0 = vis_ld_d64_nf(dpy + len);
			dy1 = vis_ld_d64_nf(dpy + len + 1);
			dy = vis_faligndata(dy0, dy1);
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
/* initialize GSR */
			vis_write_gsr(gsr_for_mul);
			EXPAND_U8C_FOR_MUL_MOD;
			MUL_U8C_S16C_MOD;
			PACK_S16C_U8C0_MOD;
/* prepare edge mask for the last bytes */
			emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
			vis_pst_8(dr, dpz, emask);
		}

		return (MLIB_SUCCESS);
	}

	dpx++;
	dpy++;
/* initialize GSR */
	vis_alignaddr((void *)0, 7);
	PACK_S16C_U8C1_MOD;
	vis_pst_8(dr1, dpz, emask);

	if ((mlib_addr)++dpz <= (mlib_addr)pzend) {
		vis_pst_8(dr1, dpz, 0x80);
	}

	px += 9 - ((mlib_addr)z & 7);
	len = dpzend - dpz;

	if (len < 0)
		return (MLIB_SUCCESS);

	for (k = 0; k < (len >> 8); k++) {
		__mlib_VectorCopy_U8((void *)x_buff,
			(void *)((mlib_d64 *)px + k * 256), 256 * 8);
/* initialize GSR */
		vis_write_gsr(gsr_for_mul);
		(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
		for (i = 0; i < 256; i++) {
			dx = x_buff[i];
			dy0 = dpy[256 * k + i];
			dy1 = vis_ld_d64_nf(dpy + 256 * k + i + 1);
			dy = vis_faligndata(dy0, dy1);
			EXPAND_U8C_FOR_0_MUL_MOD;
			MUL_U8C_S16C_MOD;
			x_buff[i] = dr1;
			r_buff[i] = dr;
		}

		vis_alignaddr((void *)0, 7);
#pragma pipeloop(0)
		for (i = 0; i < 256; i++) {
			dr1 = x_buff[i];
			dr = r_buff[i];
			PACK_S16C_U8C1_MOD;
			vis_pst_8(dr1, dpz + 256 * k + i, 0x7f);
			vis_pst_8(dr1, dpz + 256 * k + i + 1, 0x80);
		}
	}

	N = len & ~255;
	__mlib_VectorCopy_U8((void *)x_buff, (void *)((mlib_d64 *)px + N),
		(len - N) * 8 + ((mlib_addr)pzend & 7));
/* initialize GSR */
	vis_write_gsr(gsr_for_mul);
	vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
	for (i = 0; i < len - N; i++) {
		dx = x_buff[i];
		dy0 = dpy[i + N];
		dy1 = vis_ld_d64_nf(dpy + i + N + 1);
		dy = vis_faligndata(dy0, dy1);
		EXPAND_U8C_FOR_0_MUL_MOD;
		MUL_U8C_S16C_MOD;
		x_buff[i] = dr1;
		r_buff[i] = dr;
	}

	vis_alignaddr((void *)0, 7);
#pragma pipeloop(0)
	for (i = 0; i < len - N; i++) {
		dr1 = x_buff[i];
		dr = r_buff[i];
		PACK_S16C_U8C1_MOD;
		vis_pst_8(dr1, dpz + i + N, 0x7f);
		vis_pst_8(dr1, dpz + i + N + 1, 0x80);
	}

	dpz = dpzend;
	dx = x_buff[len - N];
	vis_alignaddr((void *)y, off);
	dy0 = vis_ld_d64_nf(dpy + len);
	dy1 = vis_ld_d64_nf(dpy + len + 1);
	dy = vis_faligndata(dy0, dy1);

	if ((mlib_addr)dpz < (mlib_addr)pzend) {
/* initialize GSR */
		vis_write_gsr(gsr_for_mul);
		EXPAND_U8C_FOR_MUL_MOD;
		MUL_U8C_S16C_MOD;
/* initialize GSR */
		vis_alignaddr((void *)0, 7);
		PACK_S16C_U8C1_MOD;
/* prepare edge mask for the last bytes */
		emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
		vis_pst_8(dr1, dpz, emask & 0x7f);

		if ((mlib_addr)++dpz <= (mlib_addr)pzend) {
			vis_pst_8(dr1, dpz, 0x80);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S8C_S16C                                            \
	{                                                       \
	    mlib_d64 dr2, dr3, dr4;                             \
	                                                        \
	    dr2 = vis_fpack32(dx, dx);                          \
	/*                                                      \
	 * x_i                                                  \
	 */                                                     \
	    dr3 = vis_fpack32(dy, dy);                          \
	    dr4 = vis_fandnot(without_control, dy);             \
	/*                                                      \
	 * y_r                                                  \
	 */                                                     \
	    dr3 = vis_fandnot(without_control, dr3);            \
	/*                                                      \
	 * y_i                                                  \
	 */                                                     \
	    dr = vis_fmul8sux16(dx, dr4);                       \
	    dr1 = vis_fmul8sux16(dr2, dr4);                     \
	    dr2 = vis_fmul8sux16(dr2, dr3);                     \
	    dr3 = vis_fmul8sux16(dx, dr3);                      \
	    dr = vis_fpsub16(dr, dr2);                          \
	    dr1 = vis_fpadd16(dr1, dr3);                        \
	    dr = vis_fandnot(anti_rounding_mask, dr);           \
	    dr1 = vis_fandnot(anti_rounding_mask, dr1);         \
	    dr = vis_fmul8x16(scale, dr);                       \
	    dr1 = vis_fmul8x16(scale, dr1);                     \
	}

/* *********************************************************** */

#define	PACK_S16C_S8C_SAT                                       \
	dr = vis_fpadd16(displacement, dr);                     \
	dr1 = vis_fpadd16(displacement, dr1);                   \
	dr = vis_fpack16_to_hi(dr, dr);                         \
	dr = vis_fpack16_to_lo(dr, dr1);                        \
	dr = vis_fxnor(dr, restore)

/* *********************************************************** */

#define	PACK_S16C_S8C0_SAT                                      \
	PACK_S16C_S8C_SAT;                                      \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr))

/* *********************************************************** */

#define	PACK_S16C_S8C1_SAT                                       \
	PACK_S16C_S8C_SAT;                                       \
	dr1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr));     \
	fr2 = vis_fands(vis_read_hi(dr1), mask);                 \
	dr1 = vis_fpmerge(vis_read_lo(dr1), vis_read_lo(dr1));   \
	fr3 = vis_fandnots(mask, vis_read_hi(dr1));              \
	fr2 = vis_fors(fr3, fr2);                                \
	dr1 = vis_fpmerge(vis_read_lo(dr1), vis_read_lo(dr1));   \
	dr = vis_fpmerge(fr2, vis_read_hi(dr))

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
/* edge masks */
	mlib_s32 emask, len, i;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s8 *pzend, *px = (void *)x;
	mlib_d64 *dpz, *dpx, *dpy, *dpzend;
	mlib_d64 dx, dy, dr, dr1, dx0, dx1, dy0, dy1;

	mlib_d64 without_control = vis_to_double_dup(0xff00ff);
	mlib_d64 displacement = vis_to_double_dup(0x800080);
	mlib_d64 restore = vis_to_double_dup(0x7f7f7f7f);
	mlib_s32 mask_r, mask_i;
	mlib_s32 N, k;
	mlib_d64 x_buff[256];
	mlib_f32 mask = vis_to_float(0xFFFFFF00);
	mlib_f32 fr2, fr3;
	mlib_d64 anti_rounding_mask = vis_to_double_dup(0x10001 << (shift - 1));
	mlib_f32 scale = vis_to_float(0x80808080 >> (shift - 1));

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);

/*
 * prepare the destination address
 */
	pzend = (mlib_s8 *)z + n + n - 1;
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));

	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z + (1 & (mlib_addr)z);

/*
 * generate edge mask for the start point
 */
	emask = vis_edge8(z, pzend);

/*
 * prepare the source address
 */
	dpy = (mlib_d64 *)vis_alignaddr((void *)y, off);
	dy0 = vis_ld_d64_nf(dpy);
	dy1 = vis_ld_d64_nf(dpy + 1);
	dy = vis_faligndata(dy0, dy1);
	dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_faligndata(dx0, dx1);
	MUL_S8C_S16C;
	if (!(1 & (mlib_addr)z)) {
		PACK_S16C_S8C0_SAT;
		vis_pst_8(dr, dpz, emask);
		dpz++;
		dpx++;
		dpy++;
		len = dpzend - dpz;

		if (len < 0)
			return (MLIB_SUCCESS);

		if ((((mlib_addr)x ^ (mlib_addr)z) & 7) == 0) {
			if ((((mlib_addr)x ^ (mlib_addr)y) & 7) == 0) {
#pragma pipeloop(0)
				for (i = 0; i < len; i++) {
					dx = dpx[i];
					dy = dpy[i];
					MUL_S8C_S16C;
					PACK_S16C_S8C0_SAT;
					dpz[i] = dr;
				}

				dpx += len;
				dpy += len;
				dpz += len;
				dx = vis_ld_d64_nf(dpx);
				dy = vis_ld_d64_nf(dpy);
			} else {
				(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
				for (i = 0; i < len; i++) {
					dx = dpx[i];
					dy0 = dpy[i];
					dy1 = vis_ld_d64_nf(dpy + i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S8C_S16C;
					PACK_S16C_S8C0_SAT;
					dpz[i] = dr;
				}

				dpx += len;
				dpy += len;
				dpz += len;
				dx = dpx[0];
				dy0 = vis_ld_d64_nf(dpy);
				dy1 = vis_ld_d64_nf(dpy + 1);
				dy = vis_faligndata(dy0, dy1);
			}
		} else {
			px += 8 - ((mlib_addr)z & 7);

			for (k = 0; k < (len >> 8); k++) {
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + k * 256),
					256 * 8);
				(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = x_buff[i];
					dy0 = dpy[k * 256 + i];
					dy1 = vis_ld_d64_nf(dpy + k * 256 +
						i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S8C_S16C;
					PACK_S16C_S8C0_SAT;
					dpz[256 * k + i] = dr;
				}
			}

			N = len & ~255;
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)px + N),
				(len - N) * 8 + 1 + ((mlib_addr)pzend & 7));
			vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
			for (i = N; i < len; i++) {
				dx = x_buff[i - N];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S8C_S16C;
				PACK_S16C_S8C0_SAT;
				dpz[i] = dr;
			}

			dpz = dpzend;
			dx = x_buff[len - N];
			dy0 = vis_ld_d64_nf(dpy + len);
			dy1 = vis_ld_d64_nf(dpy + len + 1);
			dy = vis_faligndata(dy0, dy1);
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			MUL_S8C_S16C;
			PACK_S16C_S8C0_SAT;
/* prepare edge mask for the last bytes */
			emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
			vis_pst_8(dr, dpz, emask);
		}

		return (MLIB_SUCCESS);
	}

	dpx++;
	dpy++;
	PACK_S16C_S8C1_SAT;
	vis_pst_8(dr, dpz, emask);

	if ((mlib_addr)++dpz <= (mlib_addr)pzend) {
		vis_pst_8(dr1, dpz, 0x80);
	}

	px += 9 - ((mlib_addr)z & 7);
	len = dpzend - dpz;

	if (len < 0)
		return (MLIB_SUCCESS);

	for (k = 0; k < (len >> 8); k++) {
		__mlib_VectorCopy_U8((void *)x_buff,
			(void *)((mlib_d64 *)px + k * 256), 256 * 8);
		(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
		for (i = 0; i < 256; i++) {
			dx = x_buff[i];
			dy0 = dpy[k * 256 + i];
			dy1 = vis_ld_d64_nf(dpy + k * 256 + i + 1);
			dy = vis_faligndata(dy0, dy1);
			MUL_S8C_S16C;
			PACK_S16C_S8C1_SAT;
			vis_pst_8(dr, dpz + 256 * k + i, 0x7f);
			vis_pst_8(dr1, dpz + 256 * k + i + 1, 0x80);
		}
	}

	N = len & ~255;
	__mlib_VectorCopy_U8((void *)x_buff, (void *)((mlib_d64 *)px + N),
		(len - N) * 8 + ((mlib_addr)pzend & 7));
	vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
	for (i = N; i < len; i++) {
		dx = x_buff[i - N];
		dy0 = dpy[i];
		dy1 = vis_ld_d64_nf(dpy + i + 1);
		dy = vis_faligndata(dy0, dy1);
		MUL_S8C_S16C;
		PACK_S16C_S8C1_SAT;
		vis_pst_8(dr, dpz + i, 0x7f);
		vis_pst_8(dr1, dpz + i + 1, 0x80);
	}

	dpz = dpzend;
	dx = x_buff[len - N];
	vis_alignaddr((void *)y, off);
	dy0 = vis_ld_d64_nf(dpy + len);
	dy1 = vis_ld_d64_nf(dpy + len + 1);
	dy = vis_faligndata(dy0, dy1);

	if ((mlib_addr)dpz < (mlib_addr)pzend) {
		MUL_S8C_S16C;
		vis_alignaddr(dpx, 7);
		PACK_S16C_S8C1_SAT;
/* prepare edge mask for the last bytes */
		emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
		vis_pst_8(dr, dpz, emask & 0x7f);

		if ((mlib_addr)++dpz <= (mlib_addr)pzend) {
			vis_pst_8(dr1, dpz, 0x80);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	PACK_S16C_S8C0_MOD                                      \
	dr = vis_fpack32(dr, dr);                               \
	dr = vis_fandnot(without_control, dr);                  \
	dr1 = vis_fand(dr1, without_control);                   \
	dr = vis_for(dr, dr1)

/* *********************************************************** */

#define	PACK_S16C_S8C1_MOD                                      \
	{                                                       \
	    mlib_d64 dr2, dr3;                                  \
	                                                        \
	    dr1 = vis_faligndata(dr1, dr1);                     \
	    dr3 = vis_fand(dr, without_control);                \
	    dr2 = vis_fandnot(without_control, dr1);            \
	    dr = vis_for(dr3, dr2);                             \
	}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S8C_S8C_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
/* edge masks */
	mlib_s32 emask, len, i;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s8 *pzend, *px = (void *)x;
	mlib_d64 *dpz, *dpx, *dpy, *dpzend;
	mlib_d64 dx, dy, dr, dr1, dx0, dx1, dy0, dy1;
	mlib_d64 without_control = vis_to_double_dup(0xff00ff);
	mlib_s32 N, k;
	mlib_d64 x_buff[256], r_buff[256];
	mlib_d64 anti_rounding_mask = vis_to_double_dup(0x10001 << (shift - 1));
	mlib_f32 scale = vis_to_float(0x80808080 >> (shift - 1));

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);

/*
 * prepare the destination address
 */
	pzend = (mlib_s8 *)z + n + n - 1;
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));

	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z + (1 & (mlib_addr)z);

/*
 * generate edge mask for the start point
 */
	emask = vis_edge8(z, pzend);

/*
 * prepare the source address
 */
	dpy = (mlib_d64 *)vis_alignaddr((void *)y, off);
	dy0 = vis_ld_d64_nf(dpy);
	dy1 = vis_ld_d64_nf(dpy + 1);
	dy = vis_faligndata(dy0, dy1);
	dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_faligndata(dx0, dx1);
	MUL_S8C_S16C;
	if (!(1 & (mlib_addr)z)) {
		PACK_S16C_S8C0_MOD;
		vis_pst_8(dr, dpz, emask);
		dpz++;
		dpx++;
		dpy++;
		len = dpzend - dpz;

		if (len < 0)
			return (MLIB_SUCCESS);

		if ((((mlib_addr)x ^ (mlib_addr)z) & 7) == 0) {
			if ((((mlib_addr)x ^ (mlib_addr)y) & 7) == 0) {
#pragma pipeloop(0)
				for (i = 0; i < len; i++) {
					dx = dpx[i];
					dy = dpy[i];
					MUL_S8C_S16C;
					PACK_S16C_S8C0_MOD;
					dpz[i] = dr;
				}

				dpx += len;
				dpy += len;
				dpz += len;
				dx = vis_ld_d64_nf(dpx);
				dy = vis_ld_d64_nf(dpy);
			} else {
				(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
				for (i = 0; i < len; i++) {
					dx = dpx[i];
					dy0 = dpy[i];
					dy1 = vis_ld_d64_nf(dpy + i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S8C_S16C;
					PACK_S16C_S8C0_MOD;
					dpz[i] = dr;
				}

				dpx += len;
				dpy += len;
				dpz += len;
				dx = dpx[0];
				dy0 = vis_ld_d64_nf(dpy);
				dy1 = vis_ld_d64_nf(dpy + 1);
				dy = vis_faligndata(dy0, dy1);
			}
		} else {
			px += 8 - ((mlib_addr)z & 7);

			for (k = 0; k < (len >> 8); k++) {
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + k * 256),
					256 * 8);
				(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = x_buff[i];
					dy0 = dpy[k * 256 + i];
					dy1 = vis_ld_d64_nf(dpy + k * 256 +
						i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S8C_S16C;
					PACK_S16C_S8C0_MOD;
					dpz[256 * k + i] = dr;
				}
			}

			N = len & ~255;
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)px + N),
				(len - N) * 8 + 1 + ((mlib_addr)pzend & 7));
			vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
			for (i = N; i < len; i++) {
				dx = x_buff[i - N];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S8C_S16C;
				PACK_S16C_S8C0_MOD;
				dpz[i] = dr;
			}

			dpz = dpzend;
			dx = x_buff[len - N];
			dy0 = vis_ld_d64_nf(dpy + len);
			dy1 = vis_ld_d64_nf(dpy + len + 1);
			dy = vis_faligndata(dy0, dy1);
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			MUL_S8C_S16C;
			PACK_S16C_S8C0_MOD;
/* prepare edge mask for the last bytes */
			emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
			vis_pst_8(dr, dpz, emask);
		}

		return (MLIB_SUCCESS);
	}

	dpx++;
	dpy++;
	vis_alignaddr(dpy, 7);
	PACK_S16C_S8C1_MOD;
	vis_pst_8(dr, dpz, emask);

	if ((mlib_addr)++dpz <= (mlib_addr)pzend) {
		vis_pst_8(dr1, dpz, 0x80);
	}

	px += 9 - ((mlib_addr)z & 7);
	len = dpzend - dpz;

	if (len < 0)
		return (MLIB_SUCCESS);

	for (k = 0; k < (len >> 8); k++) {
		__mlib_VectorCopy_U8((void *)x_buff,
			(void *)((mlib_d64 *)px + k * 256), 256 * 8);
		(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
		for (i = 0; i < 256; i++) {
			dx = x_buff[i];
			dy0 = dpy[k * 256 + i];
			dy1 = vis_ld_d64_nf(dpy + k * 256 + i + 1);
			dy = vis_faligndata(dy0, dy1);
			MUL_S8C_S16C;
			x_buff[i] = dr1;
			r_buff[i] = dr;
		}

		vis_alignaddr(dpy, 7);
#pragma pipeloop(0)
		for (i = 0; i < 256; i++) {
			dr1 = x_buff[i];
			dr = r_buff[i];
			PACK_S16C_S8C1_MOD;
			vis_pst_8(dr, dpz + 256 * k + i, 0x7f);
			vis_pst_8(dr1, dpz + 256 * k + i + 1, 0x80);
		}
	}

	N = len & ~255;
	__mlib_VectorCopy_U8((void *)x_buff, (void *)((mlib_d64 *)px + N),
		(len - N) * 8 + ((mlib_addr)pzend & 7));
	vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
	for (i = N; i < len; i++) {
		dx = x_buff[i - N];
		dy0 = dpy[i];
		dy1 = vis_ld_d64_nf(dpy + i + 1);
		dy = vis_faligndata(dy0, dy1);
		MUL_S8C_S16C;
		x_buff[i - N] = dr1;
		r_buff[i - N] = dr;
	}

	vis_alignaddr(dpy, 7);
#pragma pipeloop(0)
	for (i = N; i < len; i++) {
		dr = r_buff[i - N];
		dr1 = x_buff[i - N];
		PACK_S16C_S8C1_MOD;
		vis_pst_8(dr, dpz + i, 0x7f);
		vis_pst_8(dr1, dpz + i + 1, 0x80);
	}

	dpz = dpzend;
	dx = x_buff[len - N];
	vis_alignaddr((void *)y, off);
	dy0 = vis_ld_d64_nf(dpy + len);
	dy1 = vis_ld_d64_nf(dpy + len + 1);
	dy = vis_faligndata(dy0, dy1);

	if ((mlib_addr)dpz < (mlib_addr)pzend) {
		MUL_S8C_S16C;
		vis_alignaddr(dpy, 7);
		PACK_S16C_S8C1_MOD;
/* prepare edge mask for the last bytes */
		emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
		vis_pst_8(dr, dpz, emask & 0x7f);

		if ((mlib_addr)++dpz <= (mlib_addr)pzend) {
			vis_pst_8(dr1, dpz, 0x80);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s16 *pzend, *px = (void *)x;
	mlib_d64 mask_clear =
		vis_to_double_dup(((mlib_u32)0xffffffff >> (16 - shift + 1)));
	mlib_d64 mask_sign = vis_to_double_dup((mlib_u32)0x00008000 << shift);
	mlib_d64 mask_real_sign = vis_to_double_dup(0x80008000);
	mlib_d64 *dpz, *dpx, *dpy, *dpzend;
	mlib_d64 dx, dy, dr, dr1, dx0, dx1, dy0, dy1;
	mlib_s32 end, i, k, N;
	mlib_d64 x_buff[256];

	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);
/* initialize GSR scale factor */
	vis_write_gsr((16 - shift) << 3);

/*
 * prepare the destination address
 */
	pzend = (mlib_s16 *)z + n - 1;
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));

	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z;

/*
 * generate edge mask for the start point
 */
	emask = vis_edge16(z, pzend);

/*
 * prepare the source address
 */
	dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_faligndata(dx0, dx1);
	dpy = (mlib_d64 *)vis_alignaddr((void *)y, off);
	dy0 = vis_ld_d64_nf(dpy);
	dy1 = vis_ld_d64_nf(dpy + 1);
	dy = vis_faligndata(dy0, dy1);

	if (shift == 16) {
		MUL_S16_S32;
		PACK_S32_S16_STORE_SAT;
		dpz++;
		dpx++;
		dpy++;
		end = dpzend - dpz;

		if (end < 0)
			return (MLIB_SUCCESS);

		if ((((mlib_addr)x ^ (mlib_addr)z) & 7) == 0) {
			if ((((mlib_addr)x ^ (mlib_addr)y) & 7) == 0) {
#pragma pipeloop(0)
				for (i = 0; i < end; i++) {
					dx = dpx[i];
					dy = dpy[i];
					MUL_S16_S32;
					dpz[i] = vis_fpackfix_pair(dr, dr1);
				}

				dpz = dpzend;
				dx = dpx[end];
				dy = dpy[end];
			} else {
#pragma pipeloop(0)
				for (i = 0; i < end; i++) {
					dy0 = dpy[i];
					dy1 = vis_ld_d64_nf(dpy + i + 1);
					dy = vis_faligndata(dy0, dy1);
					dx = dpx[i];
					MUL_S16_S32;
					dpz[i] = vis_fpackfix_pair(dr, dr1);
				}

				dpz = dpzend;
				dy0 = dpy[end];
				dy1 = vis_ld_d64_nf(dpy + end + 1);
				dy = vis_faligndata(dy0, dy1);
				dx = dpx[end];
			}
		} else {
			px += 4 - (((mlib_addr)z & 6) >> 1);
			for (k = 0; k < (end >> 8); k++) {
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + k * 256),
					256 * 8);
				(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = x_buff[i];
					dy0 = dpy[k * 256 + i];
					dy1 = vis_ld_d64_nf(dpy + k * 256 +
						i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S16_S32;
					dpz[k * 256 + i] =
						vis_fpackfix_pair(dr, dr1);
				}
			}

			N = end & ~255;
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)px + N),
				(end - N) * 8 + 2 + ((mlib_addr)pzend & 7));
			(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
			for (i = N; i < end; i++) {
				dx = x_buff[i - N];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S16_S32;
				dpz[i] = vis_fpackfix_pair(dr, dr1);
			}

			dpz = dpzend;
			dy0 = vis_ld_d64_nf(dpy + end);
			dy1 = vis_ld_d64_nf(dpy + end + 1);
			dy = vis_faligndata(dy0, dy1);
			dx = x_buff[end - N];
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
/* prepare edge mask for the last bytes */
			emask = vis_edge16(dpz, pzend);
			MUL_S16_S32;
			PACK_S32_S16_STORE_SAT;
			return (MLIB_SUCCESS);
		}
	} else {
/* SHIFT < 16 */
		MUL_S16_S32;
		PACK_S32_S16_MOD;
		vis_pst_16(dr, dpz, emask);
		dpz++;
		dpx++;
		dpy++;
		end = dpzend - dpz;

		if (end < 0)
			return (MLIB_SUCCESS);

		if ((((mlib_addr)x ^ (mlib_addr)z) & 7) == 0) {
			if ((((mlib_addr)x ^ (mlib_addr)y) & 7) == 0) {
#pragma pipeloop(0)
				for (i = 0; i < end; i++) {
					dx = dpx[i];
					dy = dpy[i];
					MUL_S16_S32;
					PACK_S32_S16_MOD;
					dpz[i] = dr;
				}

				dpz = dpzend;
				dx = dpx[end];
				dy = dpy[end];
			} else {
#pragma pipeloop(0)
				for (i = 0; i < end; i++) {
					dy0 = dpy[i];
					dy1 = vis_ld_d64_nf(dpy + i + 1);
					dy = vis_faligndata(dy0, dy1);
					dx = dpx[i];
					MUL_S16_S32;
					PACK_S32_S16_MOD;
					dpz[i] = dr;
				}

				dpz = dpzend;
				dx = dpx[end];
				dy0 = dpy[end];
				dy1 = vis_ld_d64_nf(dpy + end + 1);
				dy = vis_faligndata(dy0, dy1);
			}
		} else {
			px += 4 - (((mlib_addr)z & 6) >> 1);
			end = dpzend - dpz;

			if (end < 0)
				return (MLIB_SUCCESS);
			for (k = 0; k < (end >> 8); k++) {
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + k * 256),
					256 * 8);
				(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = x_buff[i];
					dy0 = dpy[k * 256 + i];
					dy1 = vis_ld_d64_nf(dpy + k * 256 +
						i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S16_S32;
					PACK_S32_S16_MOD;
					dpz[256 * k + i] = dr;
				}
			}

			N = end & ~255;
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)px + N),
				(end - N) * 8 + 2 + ((mlib_addr)pzend & 7));
			(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
			for (i = N; i < end; i++) {
				dx = x_buff[i - N];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S16_S32;
				PACK_S32_S16_MOD;
				dpz[i] = dr;
			}

			dpz = dpzend;
			dx = x_buff[end - N];
			dy0 = vis_ld_d64_nf(dpy + end);
			dy1 = vis_ld_d64_nf(dpy + end + 1);
			dy = vis_faligndata(dy0, dy1);
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
/* prepare edge mask for the last bytes */
			emask = vis_edge16(dpz, pzend);
			MUL_S16_S32;
			PACK_S32_S16_MOD;
			vis_pst_16(dr, dpz, emask);
		}

		return (MLIB_SUCCESS);
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s16 *pzend;
	mlib_d64 *dpz, *dpx, *dpy, *dpzend;
	mlib_s16 *px = (void *)x;
	mlib_d64 dx, dy, dr, dr1, dx0, dx1, dy0, dy1;
	mlib_s32 end, i, k, N;
	mlib_d64 x_buff[256];

	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);
/* initialize GSR scale factor */
	vis_write_gsr((16 - shift) << 3);

/*
 * prepare the destination address
 */
	pzend = (mlib_s16 *)z + n - 1;
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));

	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z;

/*
 * generate edge mask for the start point
 */
	emask = vis_edge16(z, pzend);

/*
 * prepare the source address
 */
	dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_faligndata(dx0, dx1);
	dpy = (mlib_d64 *)vis_alignaddr((void *)y, off);
	dy0 = vis_ld_d64_nf(dpy);
	dy1 = vis_ld_d64_nf(dpy + 1);
	dy = vis_faligndata(dy0, dy1);
	MUL_S16_S32;
	PACK_S32_S16_STORE_SAT;
	dpz++;
	dpx++;
	dpy++;
	end = dpzend - dpz;

	if (end < 0)
		return (MLIB_SUCCESS);

	if ((((mlib_addr)x ^ (mlib_addr)z) & 7) == 0) {
		if ((((mlib_addr)x ^ (mlib_addr)y) & 7) == 0) {

#pragma pipeloop(0)
			for (i = 0; i < end; i++) {
				dx = dpx[i];
				dy = dpy[i];
				MUL_S16_S32;
				dpz[i] = vis_fpackfix_pair(dr, dr1);
			}

			dpz = dpzend;
			dx = dpx[end];
			dy = dpy[end];
		} else {
#pragma pipeloop(0)
			for (i = 0; i < end; i++) {
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				dx = dpx[i];
				MUL_S16_S32;
				dpz[i] = vis_fpackfix_pair(dr, dr1);
			}

			dpz = dpzend;
			dy0 = dpy[end];
			dy1 = vis_ld_d64_nf(dpy + end + 1);
			dy = vis_faligndata(dy0, dy1);
			dx = dpx[end];
		}
	} else {
		px += 4 - (((mlib_addr)z & 6) >> 1);
		for (k = 0; k < (end >> 8); k++) {
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)px + k * 256), 256 * 8);
			(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
			for (i = 0; i < 256; i++) {
				dx = x_buff[i];
				dy0 = dpy[k * 256 + i];
				dy1 = vis_ld_d64_nf(dpy + k * 256 + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S16_S32;
				dpz[k * 256 + i] = vis_fpackfix_pair(dr, dr1);
			}
		}

		N = end & ~255;
		__mlib_VectorCopy_U8((void *)x_buff,
			(void *)((mlib_d64 *)px + N), (end - N) * 8 +
			2 + ((mlib_addr)pzend & 7));
		(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
		for (i = N; i < end; i++) {
			dx = x_buff[i - N];
			dy0 = dpy[i];
			dy1 = vis_ld_d64_nf(dpy + i + 1);
			dy = vis_faligndata(dy0, dy1);
			MUL_S16_S32;
			dpz[i] = vis_fpackfix_pair(dr, dr1);
		}

		dpz = dpzend;
		dx = x_buff[end - N];
		dy0 = vis_ld_d64_nf(dpy + end);
		dy1 = vis_ld_d64_nf(dpy + end + 1);
		dy = vis_faligndata(dy0, dy1);
	}

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
/* prepare edge mask for the last bytes */
		emask = vis_edge16(dpz, pzend);
		MUL_S16_S32;
		PACK_S32_S16_STORE_SAT;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S16C_S32C                                                   \
	{                                                               \
	    dr6 = vis_fpmerge(vis_read_hi(dy), vis_read_lo(dy));        \
	    dr6 = vis_fpmerge(vis_read_lo(dr6), vis_read_hi(dr6));      \
	    dr6 = vis_fpmerge(vis_read_hi(dr6), vis_read_lo(dr6));      \
	    dr2 = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dy));    \
	    dr3 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dy));    \
	    dr2 = vis_fpadd32(dr2, dr3);                                \
	    sr0 = vis_fpsub32s(vis_read_hi(dr2), vis_read_lo(dr2));     \
	    dr4 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dy));    \
	    dr5 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dy));    \
	    dr4 = vis_fpadd32(dr4, dr5);                                \
	    sr1 = vis_fpsub32s(vis_read_hi(dr4), vis_read_lo(dr4));     \
	    dr7 = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dr6));   \
	    dr8 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dr6));   \
	    dr7 = vis_fpadd32(dr7, dr8);                                \
	    sr2 = vis_fpadd32s(vis_read_hi(dr7), vis_read_lo(dr7));     \
	    dr9 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dr6));   \
	    dr6 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dr6));   \
	    dr6 = vis_fpadd32(dr9, dr6);                                \
	    sr3 = vis_fpadd32s(vis_read_hi(dr6), vis_read_lo(dr6));     \
	}

/* *********************************************************** */

#define	PAIR_S32C_S16C0_MOD                                     \
	{                                                       \
	    dr = vis_freg_pair(sr0, sr2);                       \
	    dr1 = vis_freg_pair(sr1, sr3);                      \
	}

#define	PAIR_S32C_S16C1_MOD                                     \
	{                                                       \
	    dr = vis_freg_pair(sr3, sr0);                       \
	    dr1 = vis_freg_pair(sr2, sr1);                      \
	}

#define	PACK_S32C_S16C_SHIFT16_MOD	dr = vis_fpackfix_pair(dr, dr1)

#define	PACK_S32C_S16C_SHIFT_MOD                                \
	sign = vis_fand(mask_sign, dr);                         \
	dr = vis_fand(mask_clear, dr);                          \
	sign1 = vis_fand(mask_sign, dr1);                       \
	dr1 = vis_fand(mask_clear, dr1);                        \
	dr = vis_fpackfix_pair(dr, dr1);                        \
	sign = vis_fpack16_pair(sign, sign1);                   \
	sign = vis_fand(sign, mask_real_sign);                  \
	dr = vis_for(sign, dr)

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s16 *pzend, *px = (void *)x;
	mlib_f32 sr0, sr1, sr2, sr3;
	mlib_d64 *dpz, *dpx, *dpy, *dpzend;
	mlib_d64 dx, dy, dr, dr1, dx0, dx1, dy0, dy1;
	mlib_d64 mask_clear;
	mlib_d64 mask_sign;
	mlib_d64 mask_real_sign = vis_to_double_dup(0x80008000);
	mlib_d64 dr2, dr3, dr4, dr5, dr6, dr7, dr8, dr9;
	mlib_d64 sign, sign1;

	mlib_s32 i, k, N;
	mlib_s32 end;
	mlib_d64 x_buff[256];
	mlib_s32 smul_buff[1024], s0, s1, s2, s3,
		_shift = 16 - shift, *spz, old_s;
	mlib_d64 *dmul_buff = (mlib_d64 *)smul_buff;

	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);
	mask_clear =
		vis_to_double_dup(((mlib_u32)0xffffffff >> (16 - shift + 1)));
	mask_sign = vis_to_double_dup((mlib_u32)0x00008000 << shift);
/* initialize GSR scale factor */
	vis_write_gsr((16 - shift) << 3);

/*
 * prepare the destination address
 */
	pzend = (mlib_s16 *)z + n + n - 1;
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));

	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z + (2 & (mlib_addr)z);

/*
 * generate edge mask for the start point
 */
	emask = vis_edge16(z, pzend);

/*
 * prepare the source address
 */
	dpx = vis_alignaddr((void *)x, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_faligndata(dx0, dx1);
	dpy = vis_alignaddr((void *)y, off);
	dy0 = vis_ld_d64_nf(dpy);
	dy1 = vis_ld_d64_nf(dpy + 1);
	dy = vis_faligndata(dy0, dy1);
	MUL_S16C_S32C;

	if (shift == 16) {
		if (!(2 & (mlib_addr)z)) {
			PAIR_S32C_S16C0_MOD;
			PACK_S32C_S16C_SHIFT16_MOD;
			vis_pst_16(dr, dpz, emask);
			dpz++;
			dpx++;
			dpy++;
			end = dpzend - dpz;

			if (end < 0)
				return (MLIB_SUCCESS);

			if ((((mlib_addr)x | (mlib_addr)y | (mlib_addr)z) & 7)
				== 0) {
#pragma pipeloop(0)
				for (i = 0; i < end; i++) {
					dx = dpx[i];
					dy = dpy[i];
					MUL_S16C_S32C;
					PAIR_S32C_S16C0_MOD;
					PACK_S32C_S16C_SHIFT16_MOD;
					dpz[i] = dr;
				}

				dx = dpx[end];
				dy = dpy[end];
				MUL_S16C_S32C;
				PAIR_S32C_S16C0_MOD;
				PACK_S32C_S16C_SHIFT16_MOD;
/* prepare edge mask for the last bytes */
				emask = vis_edge16(dpzend, pzend);
/* store last bytes of result */
				vis_pst_16(dr, dpzend, emask);
				return (MLIB_SUCCESS);
			} else {
				px += 4 - (((mlib_addr)z & 6) >> 1);

				for (k = 0; k < (end >> 8); k++) {
					__mlib_VectorCopy_U8((void *)x_buff,
						(void *)((mlib_d64 *)px +
						k * 256), 256 * 8);
					(void *)vis_alignaddr((void *)y, off);

#pragma pipeloop(0)
					for (i = 0; i < 256; i++) {
						dx = x_buff[i];
						dy0 = dpy[k * 256 + i];
						dy1 = vis_ld_d64_nf(dpy +
							k * 256 + i + 1);
						dy = vis_faligndata(dy0, dy1);
						MUL_S16C_S32C;
						PAIR_S32C_S16C0_MOD;
						PACK_S32C_S16C_SHIFT16_MOD;
						dpz[k * 256 + i] = dr;
					}
				}

				N = end & ~255;
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + N),
					(end - N) * 8 + 2 +
					((mlib_addr)pzend & 7));
				(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
				for (i = N; i < end; i++) {
					dx = x_buff[i - N];
					dy0 = dpy[i];
					dy1 = vis_ld_d64_nf(dpy + i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S16C_S32C;
					PAIR_S32C_S16C0_MOD;
					PACK_S32C_S16C_SHIFT16_MOD;
					dpz[i] = dr;
				}
			}

			dx = x_buff[end - N];
			dy0 = vis_ld_d64_nf(dpy + end);
			dy1 = vis_ld_d64_nf(dpy + end + 1);
			dy = vis_faligndata(dy0, dy1);
			MUL_S16C_S32C;
			dpz = dpzend;

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				PAIR_S32C_S16C0_MOD;
				PACK_S32C_S16C_SHIFT16_MOD;
/* prepare edge mask for the last bytes */
				emask = vis_edge16(dpz, pzend);
/* store last bytes of result */
				vis_pst_16(dr, dpz, emask);
			}

			return (MLIB_SUCCESS);
		}

		PAIR_S32C_S16C1_MOD;
		PACK_S32C_S16C_SHIFT16_MOD;
		vis_pst_16(dr, dpz, emask & 0x7);

		if ((mlib_addr)++dpz <= (mlib_addr)pzend) {
			vis_pst_16(dr, dpz, 0x8);
		}

		end = dpzend - dpz;

		if (end < 0)
			return (MLIB_SUCCESS);
		dpx++;
		dpy++;
		px += ((((mlib_addr)(z + 1)) & 7) >> 1) + 2;

		for (k = 0; k < (end >> 8); k++) {
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)px + k * 256), 256 * 8);
			(void *)vis_alignaddr((void *)y, off);

#pragma pipeloop(0)
			for (i = 0; i < 256; i++) {
				dx = x_buff[i];
				dy0 = dpy[k * 256 + i];
				dy1 = vis_ld_d64_nf(dpy + k * 256 + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S16C_S32C;
				PAIR_S32C_S16C1_MOD;
				PACK_S32C_S16C_SHIFT16_MOD;
				vis_pst_16(dr, dpz + k * 256 + i, 0x7);
				vis_pst_16(dr, dpz + k * 256 + i + 1, 0x8);
			}
		}

		N = end & ~255;
		__mlib_VectorCopy_U8((void *)x_buff,
			(void *)((mlib_d64 *)px + N), (end - N) * 8 +
			((mlib_addr)pzend & 7));
		(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
		for (i = N; i < end; i++) {
			dx = x_buff[i - N];
			dy0 = dpy[i];
			dy1 = vis_ld_d64_nf(dpy + i + 1);
			dy = vis_faligndata(dy0, dy1);
			MUL_S16C_S32C;
			PAIR_S32C_S16C1_MOD;
			PACK_S32C_S16C_SHIFT16_MOD;
			vis_pst_16(dr, dpz + i, 0x7);
			vis_pst_16(dr, dpz + i + 1, 0x8);
		}

		if ((mlib_addr)dpzend < (mlib_addr)pzend) {
			dx = x_buff[end - N];
			dy0 = vis_ld_d64_nf(dpy + end);
			dy1 = vis_ld_d64_nf(dpy + end + 1);
			dy = vis_faligndata(dy0, dy1);
			MUL_S16C_S32C;
			PAIR_S32C_S16C1_MOD;
			PACK_S32C_S16C_SHIFT16_MOD;
/* prepare edge mask for the last bytes */
			emask = vis_edge16(dpzend, pzend);
/* store last bytes of result */
			vis_pst_16(dr, dpzend, emask & 0x7);
		}

		return (MLIB_SUCCESS);
	} else {
/* SHIFT <  16 */
		if (!(2 & (mlib_addr)z)) {
			PAIR_S32C_S16C0_MOD;
			PACK_S32C_S16C_SHIFT_MOD;
			vis_pst_16(dr, dpz, emask);
			dpz++;
			dpx++;
			dpy++;
			end = dpzend - dpz;

			if (end < 0)
				return (MLIB_SUCCESS);

			if ((((mlib_addr)x | (mlib_addr)y | (mlib_addr)z) & 7)
				== 0) {
				spz = (mlib_s32 *)dpz;
				for (k = 0; k < (end >> 8); k++) {
#pragma pipeloop(0)
					for (i = 0; i < 256; i++) {
						dx = dpx[k * 256 + i];
						dy = dpy[k * 256 + i];
						MUL_S16C_S32C;
						PAIR_S32C_S16C0_MOD;
						dmul_buff[2 * i] = dr;
						dmul_buff[2 * i + 1] = dr1;
					}

#pragma pipeloop(0)
					for (i = 0; i < 512; i++) {
						s0 = smul_buff[2 * i];
						s1 = smul_buff[2 * i + 1];
						s0 = (s0 << _shift) & ~0xFFFF;
						s1 = (s1 >> shift) & 0xFFFF;
						spz[k * 512 + i] = (s0 | s1);
					}
				}

				N = end & ~255;
#pragma pipeloop(0)
				for (i = N; i < end; i++) {
					dx = dpx[i];
					dy = dpy[i];
					MUL_S16C_S32C;
					PAIR_S32C_S16C0_MOD;
					dmul_buff[2 * (i - N)] = dr;
					dmul_buff[2 * (i - N) + 1] = dr1;
				}

				spz = (mlib_s32 *)(dpz + N);
#pragma pipeloop(0)
				for (i = 0; i < (end - N) * 2; i++) {
					s0 = smul_buff[2 * i];
					s1 = smul_buff[2 * i + 1];
					s0 = (s0 << _shift) & ~0xFFFF;
					s1 = (s1 >> shift) & 0xFFFF;
					spz[i] = (s0 | s1);
				}

				dx = dpx[end];
				dy = dpy[end];
				MUL_S16C_S32C;
				PAIR_S32C_S16C0_MOD;
				PACK_S32C_S16C_SHIFT_MOD;
/* prepare edge mask for the last bytes */
				emask = vis_edge16(dpzend, pzend);
/* store last bytes of result */
				vis_pst_16(dr, dpzend, emask);
				return (MLIB_SUCCESS);
			} else {
				px += 4 - (((mlib_addr)z & 6) >> 1);
				spz = (mlib_s32 *)dpz;

				for (k = 0; k < (end >> 8); k++) {
					__mlib_VectorCopy_U8((void *)x_buff,
						(void *)((mlib_d64 *)px +
						k * 256), 256 * 8);
					(void *)vis_alignaddr((void *)y, off);

#pragma pipeloop(0)
					for (i = 0; i < 256; i++) {
						dx = x_buff[i];
						dy0 = dpy[k * 256 + i];
						dy1 = vis_ld_d64_nf(dpy +
							k * 256 + i + 1);
						dy = vis_faligndata(dy0, dy1);
						MUL_S16C_S32C;
						PAIR_S32C_S16C0_MOD;
						dmul_buff[2 * i] = dr;
						dmul_buff[2 * i + 1] = dr1;
					}

#pragma pipeloop(0)
					for (i = 0; i < 512; i++) {
						s0 = smul_buff[2 * i];
						s1 = smul_buff[2 * i + 1];
						s0 = (s0 << _shift) & ~0xFFFF;
						s1 = (s1 >> shift) & 0xFFFF;
						spz[k * 512 + i] = (s0 | s1);
					}
				}

				N = end & ~255;
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + N),
					(end - N) * 8 + 2 +
					((mlib_addr)pzend & 7));
				(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
				for (i = N; i < end; i++) {
					dx = x_buff[i - N];
					dy0 = dpy[i];
					dy1 = vis_ld_d64_nf(dpy + i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S16C_S32C;
					PAIR_S32C_S16C0_MOD;
					dmul_buff[2 * (i - N)] = dr;
					dmul_buff[2 * (i - N) + 1] = dr1;
				}

				spz = (mlib_s32 *)(dpz + N);
#pragma pipeloop(0)
				for (i = 0; i < (end - N) * 2; i++) {
					s0 = smul_buff[2 * i];
					s1 = smul_buff[2 * i + 1];
					s0 = (s0 << _shift) & ~0xFFFF;
					s1 = (s1 >> shift) & 0xFFFF;
					spz[i] = (s0 | s1);
				}
			}

			dx = x_buff[end - N];
			dy0 = vis_ld_d64_nf(dpy + end);
			dy1 = vis_ld_d64_nf(dpy + end + 1);
			dy = vis_faligndata(dy0, dy1);
			MUL_S16C_S32C;
			dpz = dpzend;

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				PAIR_S32C_S16C0_MOD;
				PACK_S32C_S16C_SHIFT_MOD;
/* prepare edge mask for the last bytes */
				emask = vis_edge16(dpz, pzend);
/* store last bytes of result */
				vis_pst_16(dr, dpz, emask);
			}

			return (MLIB_SUCCESS);
		}

		PAIR_S32C_S16C1_MOD;
		PACK_S32C_S16C_SHIFT_MOD;
		vis_pst_16(dr, dpz, emask & 0x7);

		if ((mlib_addr)++dpz <= (mlib_addr)pzend) {
			vis_pst_16(dr, dpz, 0x8);
		}

		end = dpzend - dpz;

		if (end < 0)
			return (MLIB_SUCCESS);
		dpx++;
		dpy++;

		x += (((mlib_addr)z & 7) - 2) ? 2 : 4;
		spz = (mlib_s32 *)dpz;

		old_s = spz[0] & ~0xFFFF;
		for (k = 0; k < (end >> 8); k++) {
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)x + k * 256), 256 * 8);
			(void *)vis_alignaddr((void *)y, off);

#pragma pipeloop(0)
			for (i = 0; i < 256; i++) {
				dx = x_buff[i];
				dy0 = dpy[k * 256 + i];
				dy1 = vis_ld_d64_nf(dpy + k * 256 + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S16C_S32C;
				PAIR_S32C_S16C0_MOD;
				dmul_buff[2 * i] = dr;
				dmul_buff[2 * i + 1] = dr1;
			}

#pragma pipeloop(0)
			for (i = 0; i < 256; i++) {
				s0 = smul_buff[4 * i];
				s1 = smul_buff[4 * i + 1];
				s2 = smul_buff[4 * i + 2];
				s3 = smul_buff[4 * i + 3];

				s0 = (s0 >> shift) & 0xFFFF;
				s1 = (s1 << _shift) & ~0xFFFF;
				s2 = (s2 >> shift) & 0xFFFF;
				s3 = (s3 << _shift) & ~0xFFFF;

				spz[k * 512 + i * 2] = (old_s | s0);
				spz[k * 512 + i * 2 + 1] = (s1 | s2);
				old_s = s3;
			}
		}

		N = end & ~255;
		__mlib_VectorCopy_U8((void *)x_buff,
			(void *)((mlib_d64 *)x + N), (end - N) * 8 +
			((mlib_addr)pzend & 7));
		(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
		for (i = N; i < end; i++) {
			dx = x_buff[i - N];
			dy0 = dpy[i];
			dy1 = vis_ld_d64_nf(dpy + i + 1);
			dy = vis_faligndata(dy0, dy1);
			MUL_S16C_S32C;
			PAIR_S32C_S16C0_MOD;
			dmul_buff[2 * (i - N)] = dr;
			dmul_buff[2 * (i - N) + 1] = dr1;
		}

		spz = (mlib_s32 *)(dpz + N);
#pragma pipeloop(0)
		for (i = 0; i < end - N; i++) {
			s0 = smul_buff[4 * i];
			s1 = smul_buff[4 * i + 1];
			s2 = smul_buff[4 * i + 2];
			s3 = smul_buff[4 * i + 3];

			s0 = (s0 >> shift) & 0xFFFF;
			s1 = (s1 << _shift) & ~0xFFFF;
			s2 = (s2 >> shift) & 0xFFFF;
			s3 = (s3 << _shift) & ~0xFFFF;

			spz[i * 2] = (old_s | s0);
			spz[i * 2 + 1] = (s1 | s2);
			old_s = s3;
		}

		((mlib_s16 *)(dpz + end))[0] = (mlib_s16)(old_s >> 16);

		if ((mlib_addr)dpzend <= (mlib_addr)pzend) {
			dx = x_buff[end - N];
			dy0 = vis_ld_d64_nf(dpy + end);
			dy1 = vis_ld_d64_nf(dpy + end + 1);
			dy = vis_faligndata(dy0, dy1);
			MUL_S16C_S32C;
			PAIR_S32C_S16C1_MOD;
			PACK_S32C_S16C_SHIFT_MOD;
/* prepare edge mask for the last bytes */
			emask = vis_edge16(dpzend, pzend);
/* store last bytes of result */
			vis_pst_16(dr, dpzend, emask & 0x7);
		}

		return (MLIB_SUCCESS);
	}
}

/* *********************************************************** */

#define	OVER_MUL_S16C_SAT                                                 \
	mask_over = (vis_fcmpeq32(dr, d_min32) << 2) | (vis_fcmpeq32(dr1, \
		d_min32));                                                \
	dr = vis_freg_pair(vis_fpackfix(dr), vis_fpackfix(dr1))

#define	PACK_S32C_S16C0_SAT                                     \
	dr = vis_freg_pair(sr0, sr2);                           \
	dr1 = vis_freg_pair(sr1, sr3);                          \
	OVER_MUL_S16C_SAT

#define	PACK_S32C_S16C1_SAT                                     \
	dr = vis_freg_pair(sr3, sr0);                           \
	dr1 = vis_freg_pair(sr2, sr1);                          \
	OVER_MUL_S16C_SAT

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s16 *pzend, *px = (void *)x;
	mlib_f32 sr0, sr1, sr2, sr3;
	mlib_d64 *dpz, *dpx, *dpy, *dpzend;
	mlib_d64 dx, dy, dr, dr1, dx0, dx1, dy0, dy1;
	mlib_d64 dr2, dr3, dr4, dr5, dr6, dr7, dr8, dr9;

	mlib_d64 d_over = vis_to_double_dup(0x7fff7fff);
	mlib_d64 d_min32 = vis_to_double_dup(0x80000000);
	mlib_s32 i, mask_over, k, N;
	mlib_s32 end;
	mlib_d64 x_buff[256];

	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);
/* initialize GSR scale factor */
	vis_write_gsr((16 - shift) << 3);

/*
 * prepare the destination address
 */
	pzend = (mlib_s16 *)z + n + n - 1;
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));

	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z + (2 & (mlib_addr)z);

/*
 * generate edge mask for the start point
 */
	emask = vis_edge16(z, pzend);

/*
 * prepare the source address
 */
	dpx = vis_alignaddr((void *)x, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_faligndata(dx0, dx1);
	dpy = vis_alignaddr((void *)y, off);
	dy0 = vis_ld_d64_nf(dpy);
	dy1 = vis_ld_d64_nf(dpy + 1);
	dy = vis_faligndata(dy0, dy1);
	MUL_S16C_S32C;

	if (!(2 & (mlib_addr)z)) {
		PACK_S32C_S16C0_SAT;
		vis_pst_16(dr, dpz, emask);
		vis_pst_16(d_over, dpz, mask_over & emask);
		dpz++;
		dpx++;
		dpy++;
		end = dpzend - dpz;

		if (end < 0)
			return (MLIB_SUCCESS);

		if ((((mlib_addr)x | (mlib_addr)y | (mlib_addr)z) & 7) == 0) {
#pragma pipeloop(0)
			for (i = 0; i < end; i++) {
				dx = dpx[i];
				dy = dpy[i];
				MUL_S16C_S32C;
				PACK_S32C_S16C0_SAT;
				vis_pst_16(d_over, dpz + i, mask_over);
				vis_pst_16(dr, dpz + i, ~mask_over);
			}

			dx = dpx[end];
			dy = dpy[end];
			MUL_S16C_S32C;
			PACK_S32C_S16C0_SAT;
/* prepare edge mask for the last bytes */
			emask = vis_edge16(dpzend, pzend);
/* store last bytes of result */
			vis_pst_16(dr, dpzend, emask);
			vis_pst_16(d_over, dpzend, mask_over & emask);
			return (MLIB_SUCCESS);
		} else {
			px += 4 - (((mlib_addr)z & 6) >> 1);

			for (k = 0; k < (end >> 8); k++) {
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + k * 256),
					256 * 8);
				(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = x_buff[i];
					dy0 = dpy[k * 256 + i];
					dy1 = vis_ld_d64_nf(dpy + k * 256 +
						i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S16C_S32C;
					PACK_S32C_S16C0_SAT;
					vis_pst_16(d_over, dpz + 256 * k + i,
						mask_over);
					vis_pst_16(dr, dpz + 256 * k + i,
						~mask_over);
				}
			}

			N = end & ~255;
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)px + N),
				(end - N) * 8 + 2 + ((mlib_addr)pzend & 7));
			(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
			for (i = N; i < end; i++) {
				dx = x_buff[i - N];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S16C_S32C;
				PACK_S32C_S16C0_SAT;
				vis_pst_16(d_over, dpz + i, mask_over);
				vis_pst_16(dr, dpz + i, ~mask_over);
			}
		}

		dx = x_buff[end - N];
		dy0 = vis_ld_d64_nf(dpy + end);
		dy1 = vis_ld_d64_nf(dpy + end + 1);
		dy = vis_faligndata(dy0, dy1);
		MUL_S16C_S32C;
		dpz = dpzend;

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			PACK_S32C_S16C0_SAT;
/* prepare edge mask for the last bytes */
			emask = vis_edge16(dpz, pzend);
/* store last bytes of result */
			vis_pst_16(dr, dpz, emask);
			vis_pst_16(d_over, dpz, mask_over & emask);
		}

		return (MLIB_SUCCESS);
	}

	PACK_S32C_S16C1_SAT;
	vis_pst_16(dr, dpz, emask & 0x7);
	vis_pst_16(d_over, dpz, mask_over & emask & 0x7);

	if ((mlib_addr)++dpz <= (mlib_addr)pzend) {
		vis_pst_16(dr, dpz, 0x8);
		vis_pst_16(d_over, dpz, mask_over & 0x8);
	}

	end = dpzend - dpz;

	if (end < 0)
		return (MLIB_SUCCESS);
	dpx++;
	dpy++;
	px += (((mlib_addr)z & 7) - 2) ? 2 : 4;

	for (k = 0; k < (end >> 8); k++) {
		__mlib_VectorCopy_U8((void *)x_buff,
			(void *)((mlib_d64 *)px + k * 256), 256 * 8);
		(void *)vis_alignaddr((void *)y, off);

#pragma pipeloop(0)
		for (i = 0; i < 256; i++) {
			dx = x_buff[i];
			dy0 = dpy[k * 256 + i];
			dy1 = vis_ld_d64_nf(dpy + k * 256 + i + 1);
			dy = vis_faligndata(dy0, dy1);
			MUL_S16C_S32C;
			PACK_S32C_S16C1_SAT;
			vis_pst_16(d_over, dpz + 256 * k + i, mask_over & 0x7);
			vis_pst_16(dr, dpz + 256 * k + i, ~mask_over & 0x7);
			vis_pst_16(d_over, dpz + 256 * k + i + 1,
				mask_over & 0x8);
			vis_pst_16(dr, dpz + 256 * k + i + 1, ~mask_over & 0x8);
		}
	}

	N = end & ~255;
	__mlib_VectorCopy_U8((void *)x_buff, (void *)((mlib_d64 *)px + N),
		(end - N) * 8 + ((mlib_addr)pzend & 7));
	(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
	for (i = N; i < end; i++) {
		dx = x_buff[i - N];
		dy0 = dpy[i];
		dy1 = vis_ld_d64_nf(dpy + i + 1);
		dy = vis_faligndata(dy0, dy1);
		MUL_S16C_S32C;
		PACK_S32C_S16C1_SAT;
		vis_pst_16(d_over, dpz + i, mask_over & 0x7);
		vis_pst_16(dr, dpz + i, ~mask_over & 0x7);
		vis_pst_16(d_over, dpz + i + 1, mask_over & 0x8);
		vis_pst_16(dr, dpz + i + 1, ~mask_over & 0x8);
	}

	if ((mlib_addr)dpzend < (mlib_addr)pzend) {
		dx = x_buff[end - N];
		dy0 = vis_ld_d64_nf(dpy + end);
		dy1 = vis_ld_d64_nf(dpy + end + 1);
		dy = vis_faligndata(dy0, dy1);
		MUL_S16C_S32C;
		PACK_S32C_S16C1_SAT;
/* prepare edge mask for the last bytes */
		emask = vis_edge16(dpzend, pzend);
/* store last bytes of result */
		vis_pst_16(dr, dpzend, emask & 0x7);
		vis_pst_16(d_over, dpzend, mask_over & emask & 0x7);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
