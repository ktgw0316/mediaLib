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

#pragma ident	"@(#)mlib_v_VectorMulSAdd.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorMulSAdd_U8_U8_Sat - linear function on unsigned 8-bit
 *                                     format vector
 *      mlib_VectorMulSAdd_U8_U8_Mod - linear function on unsigned 8-bit
 *                                     format vector
 *      mlib_VectorMulSAdd_S8_S8_Sat - linear function on signed 8-bit
 *                                     format vector
 *      mlib_VectorMulSAdd_S8_S8_Mod - linear function on signed 8-bit
 *                                     format vector
 *      mlib_VectorMulSAdd_S16_S16_Sat - linear function on signed 16-bit
 *                                       format vector
 *      mlib_VectorMulSAdd_S16_S16_Mod - linear function on signed 16-bit
 *                                       format vector
 *      mlib_VectorMulSAdd_S32_S32_Sat- linear function on signed 32-bit
 *                                      format vector
 *      mlib_VectorMulSAdd_S32_S32_Mod- linear function on signed 32-bit
 *                                      format vector
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMulSAdd_S16_[Sat|Mod](mlib_s16 * xz,
 *                                                   const mlib_s16 * y,
 *                                                   const mlib_s16 * c,
 *                                                   mlib_s32 n);
 *      mlib_status mlib_VectorMulSAdd_S16_S16_[Sat|Mod](mlib_s16 * z,
 *                                                       const mlib_s16 * x,
 *                                                       const mlib_s16 * y,
 *                                                       const mlib_s16 * c,
 *                                                       mlib_s32 n);
 *      mlib_status mlib_VectorMulSAdd_S16_U8_[Sat|Mod](mlib_s16 * z,
 *                                                      const mlib_u8 * x,
 *                                                      const mlib_u8 * y,
 *                                                      const mlib_u8 * c,
 *                                                      mlib_s32 n);
 *      mlib_status mlib_VectorMulSAdd_S32_[Sat|Mod](mlib_s32 * xz,
 *                                                   const mlib_s32 * y,
 *                                                   const mlib_s32 * c,
 *                                                   mlib_s32 n);
 *      mlib_status mlib_VectorMulSAdd_S32_S32_[Sat|Mod](mlib_s32 * z,
 *                                                       const mlib_s32 * x,
 *                                                       const mlib_s32 * y,
 *                                                       const mlib_s32 * c,
 *                                                       mlib_s32 n);
 *      mlib_status mlib_VectorMulSAdd_S8_[Sat|Mod](mlib_s8 * xz,
 *                                                  const mlib_s8 * y,
 *                                                  const mlib_s8 * c,
 *                                                  mlib_s32 n);
 *      mlib_status mlib_VectorMulSAdd_S8_S8_[Sat|Mod](mlib_s8 * z,
 *                                                     const mlib_s8 * x,
 *                                                     const mlib_s8 * y,
 *                                                     const mlib_s8 * c,
 *                                                     mlib_s32 n);
 *      mlib_status mlib_VectorMulSAdd_U8_[Sat|Mod](mlib_u8 * xz,
 *                                                  const mlib_u8 * y,
 *                                                  const mlib_u8 * c,
 *                                                  mlib_s32 n);
 *      mlib_status mlib_VectorMulSAdd_U8_U8_[Sat|Mod](mlib_u8 * z,
 *                                                     const mlib_u8 * x,
 *                                                     const mlib_u8 * y,
 *                                                     const mlib_u8 * c,
 *                                                     mlib_s32 n);
 *
 *      mlib_status mlib_VectorMulSAdd_S16_S8_Mod(mlib_s16 * z,
 *                                                const mlib_s8 * x,
 *                                                const mlib_s8 * y,
 *                                                const mlib_s8 * c,
 *                                                mlib_s32 n);
 *      mlib_status mlib_VectorMulSAdd_S32_S16_Mod(mlib_s32 * z,
 *                                                 const mlib_s16 * x,
 *                                                 const mlib_s16 * y,
 *                                                 const mlib_s16 * c,
 *                                                 mlib_s32 n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      x    pointer to the first element of the first input vector
 *      y    pointer to the first element of the second input vector
 *      c    pointer to the scaling factor
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *      z = x + c * y
 */

#include <mlib_algebra.h>
#include <mlib_AlgebraUtil.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorMulSAdd_S16_Sat = __mlib_VectorMulSAdd_S16_Sat
#pragma weak mlib_VectorMulSAdd_S16_S8_Mod = __mlib_VectorMulSAdd_S16_S8_Mod
#pragma weak mlib_VectorMulSAdd_S16_U8_Mod = __mlib_VectorMulSAdd_S16_U8_Mod
#pragma weak mlib_VectorMulSAdd_S8_Mod = __mlib_VectorMulSAdd_S8_Mod
#pragma weak mlib_VectorMulSAdd_U8_Mod = __mlib_VectorMulSAdd_U8_Mod
#pragma weak mlib_VectorMulSAdd_S16_U8_Sat = __mlib_VectorMulSAdd_S16_U8_Sat
#pragma weak mlib_VectorMulSAdd_S32_S32_Mod = \
	__mlib_VectorMulSAdd_S32_S32_Mod
#pragma weak mlib_VectorMulSAdd_S32_S16_Mod = \
	__mlib_VectorMulSAdd_S32_S16_Mod
#pragma weak mlib_VectorMulSAdd_S16_S16_Mod = \
	__mlib_VectorMulSAdd_S16_S16_Mod
#pragma weak mlib_VectorMulSAdd_S8_Sat = __mlib_VectorMulSAdd_S8_Sat
#pragma weak mlib_VectorMulSAdd_S8_S8_Mod = __mlib_VectorMulSAdd_S8_S8_Mod
#pragma weak mlib_VectorMulSAdd_U8_Sat = __mlib_VectorMulSAdd_U8_Sat
#pragma weak mlib_VectorMulSAdd_S32_S32_Sat = \
	__mlib_VectorMulSAdd_S32_S32_Sat
#pragma weak mlib_VectorMulSAdd_S32_Mod = __mlib_VectorMulSAdd_S32_Mod
#pragma weak mlib_VectorMulSAdd_U8_U8_Mod = __mlib_VectorMulSAdd_U8_U8_Mod
#pragma weak mlib_VectorMulSAdd_S16_Mod = __mlib_VectorMulSAdd_S16_Mod
#pragma weak mlib_VectorMulSAdd_S16_S16_Sat = \
	__mlib_VectorMulSAdd_S16_S16_Sat
#pragma weak mlib_VectorMulSAdd_S8_S8_Sat = __mlib_VectorMulSAdd_S8_S8_Sat
#pragma weak mlib_VectorMulSAdd_S32_Sat = __mlib_VectorMulSAdd_S32_Sat
#pragma weak mlib_VectorMulSAdd_U8_U8_Sat = __mlib_VectorMulSAdd_U8_U8_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorMulSAdd_S16_Sat) mlib_VectorMulSAdd_S16_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16_Sat")));
__typeof__(__mlib_VectorMulSAdd_S16_S8_Mod) mlib_VectorMulSAdd_S16_S8_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16_S8_Mod")));
__typeof__(__mlib_VectorMulSAdd_S16_U8_Mod) mlib_VectorMulSAdd_S16_U8_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16_U8_Mod")));
__typeof__(__mlib_VectorMulSAdd_S8_Mod) mlib_VectorMulSAdd_S8_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S8_Mod")));
__typeof__(__mlib_VectorMulSAdd_U8_Mod) mlib_VectorMulSAdd_U8_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_U8_Mod")));
__typeof__(__mlib_VectorMulSAdd_S16_U8_Sat) mlib_VectorMulSAdd_S16_U8_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16_U8_Sat")));
__typeof__(__mlib_VectorMulSAdd_S32_S32_Mod) mlib_VectorMulSAdd_S32_S32_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S32_S32_Mod")));
__typeof__(__mlib_VectorMulSAdd_S32_S16_Mod) mlib_VectorMulSAdd_S32_S16_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S32_S16_Mod")));
__typeof__(__mlib_VectorMulSAdd_S16_S16_Mod) mlib_VectorMulSAdd_S16_S16_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16_S16_Mod")));
__typeof__(__mlib_VectorMulSAdd_S8_Sat) mlib_VectorMulSAdd_S8_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S8_Sat")));
__typeof__(__mlib_VectorMulSAdd_S8_S8_Mod) mlib_VectorMulSAdd_S8_S8_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S8_S8_Mod")));
__typeof__(__mlib_VectorMulSAdd_U8_Sat) mlib_VectorMulSAdd_U8_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_U8_Sat")));
__typeof__(__mlib_VectorMulSAdd_S32_S32_Sat) mlib_VectorMulSAdd_S32_S32_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S32_S32_Sat")));
__typeof__(__mlib_VectorMulSAdd_S32_Mod) mlib_VectorMulSAdd_S32_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S32_Mod")));
__typeof__(__mlib_VectorMulSAdd_U8_U8_Mod) mlib_VectorMulSAdd_U8_U8_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_U8_U8_Mod")));
__typeof__(__mlib_VectorMulSAdd_S16_Mod) mlib_VectorMulSAdd_S16_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16_Mod")));
__typeof__(__mlib_VectorMulSAdd_S16_S16_Sat) mlib_VectorMulSAdd_S16_S16_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16_S16_Sat")));
__typeof__(__mlib_VectorMulSAdd_S8_S8_Sat) mlib_VectorMulSAdd_S8_S8_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S8_S8_Sat")));
__typeof__(__mlib_VectorMulSAdd_S32_Sat) mlib_VectorMulSAdd_S32_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S32_Sat")));
__typeof__(__mlib_VectorMulSAdd_U8_U8_Sat) mlib_VectorMulSAdd_U8_U8_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_U8_U8_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MULSADD_U8_OVER_SAT                                     \
	dx_1 = vis_fpmerge(fzero, vis_read_hi(dx));             \
	dx_2 = vis_fpmerge(fzero, vis_read_lo(dx));             \
	dr3 = vis_fpadd16(dx_1, scale);                         \
	dr4 = vis_fpadd16(dx_2, scale);                         \
	dr1 = vis_fpmerge(fzero, vis_read_hi(dy));              \
	dr2 = vis_fpmerge(fzero, vis_read_lo(dy));              \
	rez_for_1 = vis_fpack16_pair(dr3, dr4);                 \
	mask_for_0 = vis_fcmpeq16(dr2, dzero);                  \
	mask_for_0 |= vis_fcmpeq16(dr1, dzero) << 4;            \
	mask_for_1 = vis_fcmpeq16(dr2, done);                   \
	mask_for_1 |= vis_fcmpeq16(dr1, done) << 4;

#define	MUL_S_ADD_U8_LO_SAT_I(i)                                \
	{                                                       \
	    drh##i = vis_fmul8x16al(vis_read_hi(dy##i), fc);    \
	    drl##i = vis_fmul8x16al(vis_read_lo(dy##i), fc);    \
	    dxh##i = vis_fpmerge(fzero, vis_read_hi(dx##i));    \
	    dxl##i = vis_fpmerge(fzero, vis_read_lo(dx##i));    \
	    drh##i = vis_fpadd16(dxh##i, drh##i);               \
	    drl##i = vis_fpadd16(dxl##i, drl##i);               \
	    drh##i = vis_fpack16_pair(drh##i, drl##i);          \
	}

#define	MUL_S_ADD_U8_LO_SAT                                     \
	{                                                       \
	    dr1 = vis_fmul8x16al(vis_read_hi(dy), fc);          \
	    dr2 = vis_fmul8x16al(vis_read_lo(dy), fc);          \
	    dx_1 = vis_fpmerge(fzero, vis_read_hi(dx));         \
	    dx_2 = vis_fpmerge(fzero, vis_read_lo(dx));         \
	    dr3 = vis_fpadd16(dx_1, dr1);                       \
	    dr4 = vis_fpadd16(dx_2, dr2);                       \
	    dr = vis_fpack16_pair(dr3, dr4);                    \
	}

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(DST, SRC)	DST = ((mlib_s32)(SRC))

#else /* MLIB_USE_FTOI_CLAMPING */

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

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask, len, i;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_u8 *pzend, *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_u8 uc = ((mlib_u8 *)c)[0];
	mlib_d64 *dpx, *dpy, *dpz, *dpzend;
	mlib_d64 dx, dy, dyn, dr, dx_1, dx_2, dr1, dr2, dr3, dr4;
	mlib_d64 dx0, dx1, dy0, dy1;
	mlib_d64 drh0, drh1, dxh0, dxh1;
	mlib_d64 drl0, drl1, dxl0, dxl1;
	mlib_f32 fc, fzero = 0.0;
	mlib_s32 mask_for_0, mask_for_1;
	mlib_d64 rez_for_1, scale, dzero = 0.0,
		done = vis_to_double_dup(0x10001),
		dmax_u8 = vis_fone();
	mlib_s32 k, N;
	mlib_d64 x_buff[256];

	if (n <= 0)
		return (MLIB_FAILURE);
/* initialize gsr scale factor */
	vis_write_gsr(7 << 3);

/*
 * prepare the destination address
 */
	pzend = (mlib_u8 *)z + n - 1;
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z;

/*
 * generate edge mask for the start point
 */
	emask = vis_edge8(z, pzend);
	dpx = (mlib_d64 *)vis_alignaddr(px, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_faligndata(dx0, dx1);
	dpy = (mlib_d64 *)vis_alignaddr(py, off);
	dy0 = vis_ld_d64_nf(dpy);
	dy1 = vis_ld_d64_nf(dpy + 1);
	dy = vis_faligndata(dy0, dy1);

/* c >= 128 */
	if (uc & MLIB_S8_MIN) {
		vis_pst_8(dmax_u8, dpz, emask);
		scale = vis_to_double_dup(uc | (uc << 16));
		MULSADD_U8_OVER_SAT;
		vis_pst_8(dx, dpz, mask_for_0 & emask);
		vis_pst_8(rez_for_1, dpz, mask_for_1 & emask);
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
					MULSADD_U8_OVER_SAT;
					vis_pst_8(dx, dpz + i, mask_for_0);
					vis_pst_8(dmax_u8, dpz + i,
						~mask_for_0);
					vis_pst_8(rez_for_1, dpz + i,
						mask_for_1);
				}

				dpz = dpzend;
				dx = dpx[len];
				dy = dpy[len];
			} else {
#pragma pipeloop(0)
				for (i = 0; i < len; i++) {
					dx = dpx[i];
					dy0 = dpy[i];
					dy1 = vis_ld_d64_nf(dpy + i + 1);
					dy = vis_faligndata(dy0, dy1);
					MULSADD_U8_OVER_SAT;
					vis_pst_8(dx, dpz + i, mask_for_0);
					vis_pst_8(dmax_u8, dpz + i,
						~mask_for_0);
					vis_pst_8(rez_for_1, dpz + i,
						mask_for_1);
				}

				dpz = dpzend;
				dx = dpx[len];
				dy0 = dpy[len];
				dy1 = vis_ld_d64_nf(dpy + len + 1);
				dy = vis_faligndata(dy0, dy1);
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
					dy1 = vis_ld_d64_nf(dpy + k * 256 +
						i + 1);
					dy = vis_faligndata(dy0, dy1);
					MULSADD_U8_OVER_SAT;
					vis_pst_8(dx, dpz + 256 * k + i,
						mask_for_0);
					vis_pst_8(dmax_u8, dpz + 256 * k + i,
						~mask_for_0);
					vis_pst_8(rez_for_1, dpz + 256 * k + i,
						mask_for_1 & ~mask_for_0);
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
				MULSADD_U8_OVER_SAT;
				vis_pst_8(dx, dpz + i, mask_for_0);
				vis_pst_8(dmax_u8, dpz + i, ~mask_for_0);
				vis_pst_8(rez_for_1, dpz + i,
					mask_for_1 & ~mask_for_0);
			}

			dpz = dpzend;
			dx = x_buff[len - N];
			dy0 = vis_ld_d64_nf(dpy + len);
			dy1 = vis_ld_d64_nf(dpy + len + 1);
			dy = vis_faligndata(dy0, dy1);
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {

/*
 * prepare the edge mask for the last point
 */
			emask = vis_edge8(dpz, pzend);
			vis_pst_8(dmax_u8, dpz, emask);
			MULSADD_U8_OVER_SAT;
			vis_pst_8(dx, dpz, mask_for_0 & emask);
			vis_pst_8(rez_for_1, dpz, mask_for_1 & emask);
		}
	} else {
/* prepare the scaling factor */
		fc = vis_to_float(uc << 8);
		MUL_S_ADD_U8_LO_SAT;
		vis_pst_8(dr, dpz, emask);
		dpz++;
		len = dpzend - dpz;

		if (len < 0)
			return (MLIB_SUCCESS);
		dpx++;
		dpy++;

		if ((((mlib_addr)x ^ (mlib_addr)z) & 7) == 0) {
			if ((((mlib_addr)x ^ (mlib_addr)y) & 7) == 0) {
				dx0 = vis_ld_d64_nf(dpx);
				dy0 = vis_ld_d64_nf(dpy);
				dx1 = vis_ld_d64_nf(dpx + 1);
				dy1 = vis_ld_d64_nf(dpy + 1);
				MUL_S_ADD_U8_LO_SAT_I(0);
				MUL_S_ADD_U8_LO_SAT_I(1);
				dx0 = vis_ld_d64_nf(dpx + 2);
				dy0 = vis_ld_d64_nf(dpy + 2);
				dx1 = vis_ld_d64_nf(dpx + 3);
				dy1 = vis_ld_d64_nf(dpy + 3);

#pragma pipeloop(0)
				for (i = 0; i < len - 1; i += 2) {
					dpz[i] = drh0;
					dpz[i + 1] = drh1;
					MUL_S_ADD_U8_LO_SAT_I(0);
					MUL_S_ADD_U8_LO_SAT_I(1);
					dx0 = vis_ld_d64_nf(dpx + i + 4);
					dy0 = vis_ld_d64_nf(dpy + i + 4);
					dx1 = vis_ld_d64_nf(dpx + i + 5);
					dy1 = vis_ld_d64_nf(dpy + i + 5);
				}

				if (len & 1)
					dpz[len - 1] = drh0;
				dpz = dpzend;
				dx = dpx[len];
				dy = dpy[len];
			} else {
				dx0 = vis_ld_d64_nf(dpx);
				dy0 = vis_ld_d64_nf(dpy);
				dyn = vis_ld_d64_nf(dpy + 1);
				dy0 = vis_faligndata(dy0, dyn);
				dx1 = vis_ld_d64_nf(dpx + 1);
				dy1 = vis_ld_d64_nf(dpy + 1);
				dyn = vis_ld_d64_nf(dpy + 2);
				dy1 = vis_faligndata(dy1, dyn);
				MUL_S_ADD_U8_LO_SAT_I(0);
				MUL_S_ADD_U8_LO_SAT_I(1);
				dx0 = vis_ld_d64_nf(dpx + 2);
				dy0 = vis_ld_d64_nf(dpy + 2);
				dyn = vis_ld_d64_nf(dpy + 3);
				dy0 = vis_faligndata(dy0, dyn);
				dx1 = vis_ld_d64_nf(dpx + 3);
				dy1 = vis_ld_d64_nf(dpy + 3);
				dyn = vis_ld_d64_nf(dpy + 4);
				dy1 = vis_faligndata(dy1, dyn);

#pragma pipeloop(0)
				for (i = 0; i < len - 1; i += 2) {
					dpz[i] = drh0;
					dpz[i + 1] = drh1;
					MUL_S_ADD_U8_LO_SAT_I(0);
					MUL_S_ADD_U8_LO_SAT_I(1);
					dx0 = vis_ld_d64_nf(dpx + i + 4);
					dy0 = vis_ld_d64_nf(dpy + i + 4);
					dyn = vis_ld_d64_nf(dpy + i + 5);
					dy0 = vis_faligndata(dy0, dyn);
					dx1 = vis_ld_d64_nf(dpx + i + 5);
					dy1 = vis_ld_d64_nf(dpy + i + 5);
					dyn = vis_ld_d64_nf(dpy + i + 6);
					dy1 = vis_faligndata(dy1, dyn);
				}

				if (len & 1)
					dpz[len - 1] = drh0;
			}

			dpz = dpzend;
			dx = dpx[len];
			dy0 = dpy[len];
			dy1 = vis_ld_d64_nf(dpy + len + 1);
			dy = vis_faligndata(dy0, dy1);
		} else {
			px += (mlib_s32)8 - ((mlib_addr)z & 7);
			for (k = 0; k < (len >> 8); k++) {
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + k * 256),
					256 << 3);
				(void *)vis_alignaddr(py, off);
#pragma pipeloop(0)
				for (i = 0; i < 256; i += 2) {
					dx0 = x_buff[i];
					dy0 = dpy[256 * k + i];
					dyn = vis_ld_d64_nf(dpy + 256 * k +
						i + 1);
					dy0 = vis_faligndata(dy0, dyn);
					dx1 = x_buff[i + 1];
					dy1 = vis_ld_d64_nf(dpy + 256 * k +
						i + 1);
					dyn = vis_ld_d64_nf(dpy + 256 * k +
						i + 2);
					dy1 = vis_faligndata(dy1, dyn);
					MUL_S_ADD_U8_LO_SAT_I(0);
					MUL_S_ADD_U8_LO_SAT_I(1);
					dpz[256 * k + i] = drh0;
					dpz[256 * k + i + 1] = drh1;
				}
			}

			N = len & ~255;
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)px + N),
				(len - N) * 8 + 1 + ((mlib_addr)pzend & 7));
			vis_alignaddr(py, off);
			dx0 = x_buff[0];
			dy0 = vis_ld_d64_nf(dpy + N);
			dyn = vis_ld_d64_nf(dpy + N + 1);
			dy0 = vis_faligndata(dy0, dyn);
			dx1 = x_buff[1];
			dy1 = vis_ld_d64_nf(dpy + N + 1);
			dyn = vis_ld_d64_nf(dpy + N + 2);
			dy1 = vis_faligndata(dy1, dyn);
			MUL_S_ADD_U8_LO_SAT_I(0);
			MUL_S_ADD_U8_LO_SAT_I(1);
			dx0 = x_buff[2];
			dy0 = vis_ld_d64_nf(dpy + N + 2);
			dyn = vis_ld_d64_nf(dpy + N + 3);
			dy0 = vis_faligndata(dy0, dyn);
			dx1 = x_buff[3];
			dy1 = vis_ld_d64_nf(dpy + N + 3);
			dyn = vis_ld_d64_nf(dpy + N + 4);
			dy1 = vis_faligndata(dy1, dyn);
#pragma pipeloop(0)
			for (i = N; i < len - 1; i += 2) {
				dpz[i] = drh0;
				dpz[i + 1] = drh1;
				MUL_S_ADD_U8_LO_SAT_I(0);
				MUL_S_ADD_U8_LO_SAT_I(1);
				dx0 = x_buff[i - N + 4];
				dy0 = vis_ld_d64_nf(dpy + i + 4);
				dyn = vis_ld_d64_nf(dpy + i + 5);
				dy0 = vis_faligndata(dy0, dyn);
				dx1 = x_buff[i - N + 5];
				dy1 = vis_ld_d64_nf(dpy + i + 5);
				dyn = vis_ld_d64_nf(dpy + i + 6);
				dy1 = vis_faligndata(dy1, dyn);
			}

			if (len & 1)
				dpz[len - 1] = drh0;
			dpz = dpzend;
			dx = x_buff[len - N];
			dy0 = vis_ld_d64_nf(dpy + len);
			dy1 = vis_ld_d64_nf(dpy + len + 1);
			dy = vis_faligndata(dy0, dy1);
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			MUL_S_ADD_U8_LO_SAT;
/* prepare edge mask for the last bytes */
			emask = vis_edge8(dpz, pzend);
			vis_pst_8(dr, dpz, emask);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S_ADD_U8_MOD                                        \
	dr1 = vis_fmul8x16al(vis_read_hi(dy), fc);              \
	dr2 = vis_fmul8x16al(vis_read_lo(dy), fc);              \
	dx_1 = vis_fmul8x16al(vis_read_hi(dx), factor);         \
	dx_2 = vis_fpmerge(dzero, vis_read_lo(dx));             \
	dr3 = vis_fpadd16(dx_1, dr1);                           \
	dr4 = vis_fpadd16(dx_2, dr2);                           \
	dr3 = vis_fand(dr3, without_control);                   \
	dr4 = vis_fand(dr4, without_control);                   \
	dr = vis_fpack16_pair(dr3, dr4);

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask, len, i;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_u8 *pzend, *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_u8 uc = ((mlib_u8 *)c)[0];
	mlib_d64 *dpz, *dpx, *dpy, *dpzend;
	mlib_d64 dx, dy, dr;
	mlib_d64 dx0, dx1, dy0, dy1;
	mlib_d64 dr1, dr2, dr3, dr4;
	mlib_f32 factor = vis_to_float(0x0100);
	mlib_s32 k, N;
	mlib_d64 x_buff[256];
	mlib_d64 dx_1, dx_2, dzero = vis_fzero();
	mlib_f32 fc;
	mlib_d64 without_control = vis_to_double_dup(0xff00ff);

	if (n <= 0)
		return (MLIB_FAILURE);
/* prepare the scaling factor */
	fc = vis_to_float(uc << 8);
/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);

/*
 * prepare the destination address
 */
	pzend = (mlib_u8 *)z + n - 1;
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
	MUL_S_ADD_U8_MOD;
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
				MUL_S_ADD_U8_MOD;
				dpz[i] = dr;
			}

			dpz = dpzend;
			dx = dpx[len];
			dy = dpy[len];
		} else {
#pragma pipeloop(0)
			for (i = 0; i < len; i++) {
				dx = dpx[i];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S_ADD_U8_MOD;
				dpz[i] = dr;
			}

			dpz = dpzend;
			dx = dpx[len];
			dy0 = dpy[len];
			dy1 = vis_ld_d64_nf(dpy + len + 1);
			dy = vis_faligndata(dy0, dy1);
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
				MUL_S_ADD_U8_MOD;
				dpz[256 * k + i] = dr;
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
			MUL_S_ADD_U8_MOD;
			dpz[i] = dr;
		}

		dpz = dpzend;
		dx = x_buff[len - N];
		dy0 = vis_ld_d64_nf(dpy + len);
		dy1 = vis_ld_d64_nf(dpy + len + 1);
		dy = vis_faligndata(dy0, dy1);
	}

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
		MUL_S_ADD_U8_MOD;
/* prepare edge mask for the last bytes */
		emask = vis_edge8(dpz, pzend);
		vis_pst_8(dr, dpz, emask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S_ADD_S8_SAT                                        \
	dr1 = vis_fpmerge(vis_read_hi(dy), vis_read_hi(dy));    \
	dr2 = vis_fpmerge(vis_read_lo(dy), vis_read_lo(dy));    \
	dr1 = vis_fmul8sux16(dr1, dc);                          \
	dr2 = vis_fmul8sux16(dr2, dc);                          \
	dx = vis_fxor(dx, restore);                             \
	dr3 = vis_fmul8x16al(vis_read_hi(dx), factor);          \
	dr4 = vis_fmul8x16al(vis_read_lo(dx), factor);          \
	dr3 = vis_fpadd16(dr1, dr3);                            \
	dr4 = vis_fpadd16(dr2, dr4);                            \
	dr = vis_fpack16_pair(dr3, dr4);                        \
	dr = vis_fxor(dr, restore);

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask, len, i;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s8 *pzend, *px = (mlib_s8 *)x, *py = (mlib_s8 *)y;
	mlib_u16 uc = ((mlib_u8 *)c)[0] << 8;
	mlib_d64 *dpx, *dpy, *dpz, *dpzend;
	mlib_d64 dx, dx0, dx1, dy, dy0, dy1, dr, dr1, dr2, dr3, dr4;
	mlib_d64 restore = vis_to_double_dup(0x80808080);

/* prepare the scaling factor */
	mlib_d64 dc = vis_to_double_dup((uc << 16) | uc);
	mlib_f32 factor = vis_to_float(0x0100);
	mlib_s32 k, N;
	mlib_d64 x_buff[256];

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
	MUL_S_ADD_S8_SAT;
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
				MUL_S_ADD_S8_SAT;
				dpz[i] = dr;
			}

			dpz = dpzend;
			dx = dpx[len];
			dy = dpy[len];
		} else {
#pragma pipeloop(0)
			for (i = 0; i < len; i++) {
				dx = dpx[i];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S_ADD_S8_SAT;
				dpz[i] = dr;
			}

			dpz = dpzend;
			dx = dpx[len];
			dy0 = dpy[len];
			dy1 = vis_ld_d64_nf(dpy + len + 1);
			dy = vis_faligndata(dy0, dy1);
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
				MUL_S_ADD_S8_SAT;
				dpz[256 * k + i] = dr;
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
			MUL_S_ADD_S8_SAT;
			dpz[i] = dr;
		}

		dpz = dpzend;
		dx = x_buff[len - N];
		dy0 = vis_ld_d64_nf(dpy + len);
		dy1 = vis_ld_d64_nf(dpy + len + 1);
		dy = vis_faligndata(dy0, dy1);
	}

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
		MUL_S_ADD_S8_SAT;
/* prepare edge mask for the last bytes */
		emask = vis_edge8(dpz, pzend);
		vis_pst_8(dr, dpz, emask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S8_S8_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return __mlib_VectorMulSAdd_U8_U8_Mod((mlib_u8 *)z, (mlib_u8 *)x,
		(mlib_u8 *)y, (mlib_u8 *)c, n);
}

/* *********************************************************** */

#define	MUL_S16                                                 \
	dr1 = vis_fmuld8sux16(vis_read_hi(dy), fc);             \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dy), fc);             \
	dr3 = vis_fmuld8sux16(vis_read_lo(dy), fc);             \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dy), fc);             \
	dx_hi = vis_fmuld8ulx16(fone, vis_read_hi(dx));         \
	dx_lo = vis_fmuld8ulx16(fone, vis_read_lo(dx));

#define	ADD_AND_PACK_S16                                        \
	dr1 = vis_fpadd32(dr1, dr2);                            \
	dr3 = vis_fpadd32(dr3, dr4);                            \
	dr5 = vis_fpadd32(dr1, dx_hi);                          \
	dr6 = vis_fpadd32(dr3, dx_lo);                          \
	dr2 = vis_fpadd32(dr5, dr5);                            \
	dr4 = vis_fpadd32(dr6, dr6);                            \
	dr = vis_fpackfix_pair(dr2, dr4);

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s16 sc = ((mlib_s16 *)c)[0];
	mlib_s16 *pzend, *px = (void *)x, *py = (void *)y;
	mlib_d64 *dpx, *dpy, *dpz, *dpzend;
	mlib_d64 dx, dy, dx0, dx1, dy0, dy1, dx_hi, dx_lo, dr, dr1, dr2, dr3,
		dr4, dr5, dr6;
	mlib_f32 fone = vis_to_float(0x10001), fc;
	mlib_s32 k, N, i, len;
	mlib_d64 x_buff[256];

	if (n <= 0)
		return (MLIB_FAILURE);

/* initialize GSR scale factor */
	vis_write_gsr(15 << 3);

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
	dpx = (mlib_d64 *)vis_alignaddr(px, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_faligndata(dx0, dx1);
	dpy = (mlib_d64 *)vis_alignaddr(py, off);
	dy0 = vis_ld_d64_nf(dpy);
	dy1 = vis_ld_d64_nf(dpy + 1);
	dy = vis_faligndata(dy0, dy1);

/* c != -32768 */
	if (sc != MLIB_S16_MIN) {
		fc = vis_to_float((sc << 16) | (sc & MLIB_U16_MAX));
		MUL_S16;
		ADD_AND_PACK_S16;
/* store first bytes of result */
		vis_pst_16(dr, dpz, emask);
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
					MUL_S16;
					ADD_AND_PACK_S16;
					dpz[i] = dr;
				}

				dpz = dpzend;
				dx = dpx[len];
				dy = dpy[len];
			} else {
#pragma pipeloop(0)
				for (i = 0; i < len; i++) {
					dx = dpx[i];
					dy0 = dpy[i];
					dy1 = vis_ld_d64_nf(dpy + i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S16;
					ADD_AND_PACK_S16;
					dpz[i] = dr;
				}

				dpz = dpzend;
				dx = dpx[len];
				dy0 = dpy[len];
				dy1 = vis_ld_d64_nf(dpy + len + 1);
				dy = vis_faligndata(dy0, dy1);
			}
		} else {
			px += 4 - (((mlib_addr)z & 6) >> 1);
			for (k = 0; k < (len >> 8); k++) {
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + k * 256),
					256 * 8);
				(void *)vis_alignaddr(py, off);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = x_buff[i];
					dy0 = dpy[k * 256 + i];
					dy1 = vis_ld_d64_nf(dpy + k * 256 +
						i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S16;
					ADD_AND_PACK_S16;
					dpz[256 * k + i] = dr;
				}
			}

			N = len & ~255;
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)px + N),
				(len - N) * 8 + 2 + ((mlib_addr)pzend & 7));
			vis_alignaddr(py, off);
#pragma pipeloop(0)
			for (i = N; i < len; i++) {
				dx = x_buff[i - N];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S16;
				ADD_AND_PACK_S16;
				dpz[i] = dr;
			}

			dpz = dpzend;
			dx = x_buff[len - N];
			dy0 = vis_ld_d64_nf(dpy + len);
			dy1 = vis_ld_d64_nf(dpy + len + 1);
			dy = vis_faligndata(dy0, dy1);
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			MUL_S16;
			ADD_AND_PACK_S16;
/* prepare edge mask for the last bytes */
			emask = vis_edge16(dpz, pzend);
/* store last bytes of result */
			vis_pst_16(dr, dpz, emask);
		}

		return (MLIB_SUCCESS);
	} else {
/* c == MLIB_S16_MIN */
		mlib_s32 mask_over;
		mlib_d64 dmax = vis_to_double_dup(MLIB_S32_MAX / 2);
		mlib_d64 d_over = vis_to_double_dup(0x7fff7fff);

		fc = vis_fnots(vis_read_hi(d_over));

		MUL_S16;
		ADD_AND_PACK_S16;
		mask_over = (vis_fcmpgt32(dr5, dmax) << 2)
			| vis_fcmpgt32(dr6, dmax);
/* store first bytes of result */
		vis_pst_16(dr, dpz, emask);
		vis_pst_16(d_over, dpz, mask_over & emask);
		dpz++;
		len = dpzend - dpz;

		if (len < 0)
			return (MLIB_SUCCESS);
		dpx++;
		dpy++;

		dy = vis_faligndata(dy0, dy1);

		if ((((mlib_addr)x ^ (mlib_addr)z) & 7) == 0) {
			if ((((mlib_addr)x ^ (mlib_addr)y) & 7) == 0) {
#pragma pipeloop(0)
				for (i = 0; i < len; i++) {
					dx = dpx[i];
					dy = dpy[i];
					MUL_S16;
					ADD_AND_PACK_S16;
					mask_over =
						(vis_fcmpgt32(dr5, dmax) << 2)
						| vis_fcmpgt32(dr6, dmax);
					dpz[i] = dr;
					vis_pst_16(d_over, dpz + i, mask_over);
				}

				dpz = dpzend;
				dx = dpx[len];
				dy = dpy[len];
			} else {
#pragma pipeloop(0)
				for (i = 0; i < len; i++) {
					dx = dpx[i];
					dy0 = dpy[i];
					dy1 = vis_ld_d64_nf(dpy + i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S16;
					ADD_AND_PACK_S16;
					mask_over =
						(vis_fcmpgt32(dr5, dmax) << 2)
						| vis_fcmpgt32(dr6, dmax);
					dpz[i] = dr;
					vis_pst_16(d_over, dpz + i, mask_over);
				}

				dpz = dpzend;
				dx = dpx[len];
				dy0 = dpy[len];
				dy1 = vis_ld_d64_nf(dpy + len + 1);
				dy = vis_faligndata(dy0, dy1);
			}
		} else {
			px += 4 - (((mlib_addr)z & 6) >> 1);
			for (k = 0; k < (len >> 8); k++) {
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + k * 256),
					256 * 8);
				(void *)vis_alignaddr(py, off);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = x_buff[i];
					dy0 = dpy[k * 256 + i];
					dy1 = vis_ld_d64_nf(dpy + k * 256 +
						i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S16;
					ADD_AND_PACK_S16;
					mask_over =
						(vis_fcmpgt32(dr5, dmax) << 2)
						| vis_fcmpgt32(dr6, dmax);
					dpz[256 * k + i] = dr;
					vis_pst_16(d_over, dpz + k * 256 + i,
						mask_over);
				}
			}

			N = len & ~255;
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)px + N),
				(len - N) * 8 + 2 + ((mlib_addr)pzend & 7));
			vis_alignaddr(py, off);
#pragma pipeloop(0)
			for (i = N; i < len; i++) {
				dx = x_buff[i - N];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S16;
				ADD_AND_PACK_S16;
				mask_over = (vis_fcmpgt32(dr5, dmax) << 2)
					| vis_fcmpgt32(dr6, dmax);
				dpz[i] = dr;
				vis_pst_16(d_over, dpz + i, mask_over);
			}

			dpz = dpzend;
			dx = x_buff[len - N];
			dy0 = vis_ld_d64_nf(dpy + len);
			dy1 = vis_ld_d64_nf(dpy + len + 1);
			dy = vis_faligndata(dy0, dy1);
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			MUL_S16;
			ADD_AND_PACK_S16;
			mask_over = (vis_fcmpgt32(dr5, dmax) << 2)
				| vis_fcmpgt32(dr6, dmax);
/* prepare edge mask for the last bytes */
			emask = vis_edge16(dpz, pzend);
/* store last bytes of result */
			vis_pst_16(dr, dpz, emask);
			vis_pst_16(d_over, dpz, mask_over & emask);
		}

		return (MLIB_SUCCESS);
	}
}

/* *********************************************************** */

#define	MUL_S16_MOD                                             \
	dr1 = vis_fmuld8sux16(vis_read_hi(dy), fc);             \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dy), fc);             \
	dr3 = vis_fmuld8sux16(vis_read_lo(dy), fc);             \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dy), fc);             \
	dr5 = vis_fpadd32(dr1, dr2);                            \
	dr6 = vis_fpadd32(dr3, dr4);                            \
	dxx = dx;

#define	ADD_AND_PACK_S16_MOD                                      \
	sr0 = vis_fands(vis_read_lo(dr5), without_control);       \
	sr1 = vis_fands(vis_read_lo(dr6), without_control);       \
	dr = vis_freg_pair(vis_read_hi(dr5), vis_read_hi(dr6));   \
	dr = vis_fpack32(dr, dzero);                              \
	dr = vis_fpack32(dr, dzero);                              \
	dr = vis_fxor(dr, vis_freg_pair(sr0, sr1));               \
	dr = vis_fpadd16(dr, dxx);

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask, len, i;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_u16 uc = ((mlib_u16 *)c)[0];
	mlib_s16 *pzend = (mlib_s16 *)z + n - 1, *px = (void *)x;
	mlib_d64 *dpx, *dpy, *dpz, *dpzend;
	mlib_d64 dx, dy, dx0, dx1, dy0, dy1, dr, dr1, dr2, dr3, dr4, dr5, dr6,
		dxx;
	mlib_f32 without_control = vis_to_float(0xffff),
		fc = vis_to_float((uc << 16) | uc), sr0, sr1;
	mlib_d64 dzero = vis_fzero();
	mlib_d64 x_buff[256];
	mlib_s32 N, k;

	if (n <= 0)
		return (MLIB_FAILURE);
/*
 * prepare the destination address
 */
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z;

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

/*
 * generate edge mask for the start point
 */
	emask = vis_edge16(z, pzend);

	MUL_S16_MOD;
	ADD_AND_PACK_S16_MOD;
/* store first bytes of result */
	vis_pst_16(dr, dpz, emask);
	dpz++;

	dy0 = dy1;
	dy1 = vis_ld_d64_nf(dpy + 2);
	dy = vis_faligndata(dy0, dy1);
	(void *)(mlib_d64 *)vis_alignaddr((void *)x, off);
	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx + 2);
	dx = vis_faligndata(dx0, dx1);
	MUL_S16_MOD;
	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx + 3);
	dx = vis_faligndata(dx0, dx1);
	(void *)(mlib_d64 *)vis_alignaddr((void *)y, off);
	dy0 = dy1;
	dy1 = vis_ld_d64_nf(dpy + 3);
	dy = vis_faligndata(dy0, dy1);
	len = dpzend - dpz;

	if (len < 0)
		return (MLIB_SUCCESS);

	if ((((mlib_addr)x ^ (mlib_addr)z) & 7) == 0) {
		if ((((mlib_addr)x ^ (mlib_addr)y) & 7) == 0) {
#pragma pipeloop(0)
			for (i = 0; i < len; i++) {
				ADD_AND_PACK_S16_MOD;
				MUL_S16_MOD;
				dpz[0] = dr;
				dx = vis_ld_d64_nf(dpx + 3);
				dy = vis_ld_d64_nf(dpy + 3);
				dpx++;
				dpy++;
				dpz++;
			}
		} else {
#pragma pipeloop(0)
			for (i = 0; i < len; i++) {
				ADD_AND_PACK_S16_MOD;
				MUL_S16_MOD;
				dpz[0] = dr;
				dx = vis_ld_d64_nf(dpx + 3);
				dy0 = dy1;
				dy1 = vis_ld_d64_nf(dpy + 4);
				dy = vis_faligndata(dy0, dy1);
				dpx++;
				dpy++;
				dpz++;
			}
		}
	} else {
		if (len > 0) {
			len--;
			px += 4 - (((mlib_addr)z & 6) >> 1) + 8;
			for (k = 0; k < (len >> 8); k++) {
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + k * 256),
					(256) * 8);
				(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					ADD_AND_PACK_S16_MOD;
					MUL_S16_MOD;
					dpz[0] = dr;
					dx = x_buff[i];
					dy0 = dy1;
					dy1 = vis_ld_d64_nf(dpy + 4);
					dy = vis_faligndata(dy0, dy1);
					dpx++;
					dpy++;
					dpz++;
				}
			}

			N = len & ~255;
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)px + N),
				(len - N) * 8 - 6 + ((mlib_addr)pzend & 7));
			(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
			for (i = 0; i < len - N; i++) {
				ADD_AND_PACK_S16_MOD;
				MUL_S16_MOD;
				dpz[0] = dr;
				dx = x_buff[i];
				dy0 = dy1;
				dy1 = vis_ld_d64_nf(dpy + 4);
				dy = vis_faligndata(dy0, dy1);
				dpx++;
				dpy++;
				dpz++;
			}

			ADD_AND_PACK_S16_MOD;
			MUL_S16_MOD;
			dpz[0] = dr;
			dpz++;
		}
	}

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
		ADD_AND_PACK_S16_MOD;
/* prepare edge mask for the last bytes */
		emask = vis_edge16(dpz, pzend);
/* store last bytes of result */
		vis_pst_16(dr, dpz, emask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32_S32_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	const mlib_s32 *c,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_s32 *px = (mlib_s32 *)x, *py = (mlib_s32 *)y, *pz = (mlib_s32 *)z;
	mlib_d64 val_c = *((mlib_s32 *)c), mul_add;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		mul_add = (mlib_d64)px[i] + val_c * py[i];
		FLOAT2INT_CLAMP(pz[i], mul_add);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32_S32_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	const mlib_s32 *c,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 d16 = 0x10000, _d16 = 1 / d16;
	mlib_d64 d32 = d16 * d16;
	mlib_d64 mul_hi, mul_lo;
	mlib_d64 dc_hi = ((mlib_s16 *)(c))[0], dy, dx;
	mlib_d64 dc_lo = ((mlib_u16 *)(c))[1];
	mlib_d64 cst = (1 << 30) * (mlib_d64)(3 << 21);
	type_union_mlib_d64 r;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		dy = y[i];
		dx = x[i];

		mul_hi = dc_hi * dy;
		mul_lo = dc_lo * dy;

		mul_hi = mul_hi * d16 - (mlib_s32)(mul_hi * _d16) * d32;
		r.db = mul_hi + mul_lo + dx + cst;
		z[i] = r.two_int.int1;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_U8_Sat(
	mlib_u8 *xz,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_U8_U8_Sat(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_U8_Mod(
	mlib_u8 *xz,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_U8_U8_Mod(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S8_Sat(
	mlib_s8 *xz,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S8_S8_Sat(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S8_Mod(
	mlib_s8 *xz,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S8_S8_Mod(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16_Sat(
	mlib_s16 *xz,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S16_S16_Sat(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16_Mod(
	mlib_s16 *xz,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S16_S16_Mod(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32_Sat(
	mlib_s32 *xz,
	const mlib_s32 *y,
	const mlib_s32 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S32_S32_Sat(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32_Mod(
	mlib_s32 *xz,
	const mlib_s32 *y,
	const mlib_s32 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S32_S32_Mod(xz, xz, y, c, n));
}

/* *********************************************************** */

#define	MUL_S_ADD_S16_U8_SAT_SAT                                \
	dr1 = vis_fmul8x16al(vis_read_hi(dy), fc);              \
	dr2 = vis_fmul8x16al(vis_read_lo(dy), fc);              \
	dr3 = vis_fmul8x16al(vis_read_hi(dy), f127);            \
	dr4 = vis_fmul8x16al(vis_read_lo(dy), f127);            \
	dx_hi = vis_fpmerge(fzero, vis_read_hi(dx));            \
	dx_lo = vis_fpmerge(fzero, vis_read_lo(dx));            \
	dr3 = vis_fpsub16(dr3, dr1);                            \
	dr4 = vis_fpsub16(dr4, dr2);                            \
	dzh = vis_fpadd16(dx_hi, dr3);                          \
	dzl = vis_fpadd16(dx_lo, dr4);                          \
	mask = vis_fcmplt16(dzh, dzero);                        \
	mask1 = vis_fcmplt16(dzl, dzero);

#define	MUL_S_ADD_S16_U8_SAT                                    \
	dr1 = vis_fmul8x16al(vis_read_hi(dy), fc);              \
	dr2 = vis_fmul8x16al(vis_read_lo(dy), fc);              \
	dx_hi = vis_fpmerge(dzero, vis_read_hi(dx));            \
	dx_lo = vis_fpmerge(dzero, vis_read_lo(dx));            \
	dzh = vis_fpadd16(dx_hi, dr1);                          \
	dzl = vis_fpadd16(dx_lo, dr2);                          \
	mask = vis_fcmplt16(dzh, dzero);                        \
	mask1 = vis_fcmplt16(dzl, dzero);

/* *********************************************************** */

#pragma align 64(umasks)
static const mlib_u32 umasks[] = {
	0x0, 0x0, 0x0, 0x7FFF,
	0x0, 0x7FFF0000, 0x0, 0x7FFF7FFF,
	0x7FFF, 0x0, 0x7FFF, 0x7FFF,
	0x7FFF, 0x7FFF0000, 0x7FFF, 0x7FFF7FFF,
	0x7FFF0000, 0x0, 0x7FFF0000, 0x7FFF,
	0x7FFF0000, 0x7FFF0000, 0x7FFF0000, 0x7FFF7FFF,
	0x7FFF7FFF, 0x0, 0x7FFF7FFF, 0x7FFF,
	0x7FFF7FFF, 0x7FFF0000, 0x7FFF7FFF, 0x7FFF7FFF
};

/* *********************************************************** */

#pragma align 64(umasks)
static const mlib_u32 unotmasks[] = {
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000,
	0xFFFFFFFF, 0xFFFF, 0xFFFFFFFF, 0x0,
	0xFFFF0000, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFFFF0000, 0xFFFF, 0xFFFF0000, 0x0,
	0xFFFF, 0xFFFFFFFF, 0xFFFF, 0xFFFF0000,
	0xFFFF, 0xFFFF, 0xFFFF, 0x0,
	0x0, 0xFFFFFFFF, 0x0, 0xFFFF0000,
	0x0, 0xFFFF, 0x0, 0x0
};

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16_U8_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_s32 len = n, i, k, N;
	mlib_d64 *dpx, *dpy, *dpz;
	mlib_u8 *px, *py;
	mlib_s16 *pz;
	mlib_d64 dx, dy, dx_hi, dx_lo, dr1, dr2, dr3, dr4, dzh, dzl, dy0, dy1;
	mlib_u16 uc = *((mlib_u8 *)c);
	mlib_f32 f127 = vis_to_float(0x7f00);
	mlib_d64 *dmasks = (mlib_d64 *)umasks;
	mlib_d64 *dnotmasks = (mlib_d64 *)unotmasks;
	mlib_f32 fc;
	mlib_s32 mul_add;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_d64 dzero = +0.0;
	mlib_f32 fzero = vis_fzero();
	mlib_d64 control_ovl =
		vis_to_double_dup((MLIB_S16_MAX << 16) | MLIB_S16_MAX);
	mlib_s32 mask, mask1;
	mlib_d64 x_buff[256];

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_u8 *)x;
	py = (mlib_u8 *)y;
	pz = (mlib_s16 *)z;

	if (n <= 8) {
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			mul_add = ((mlib_s32)(*py)) * uc + ((mlib_s32)(*px));
			(*pz++) =
				mul_add > MLIB_S16_MAX ? MLIB_S16_MAX : mul_add;
			px++;
			py++;
		}

		return (MLIB_SUCCESS);
	}

	while ((mlib_addr)pz & 7) {
		mul_add = ((mlib_s32)(*py)) * uc + ((mlib_s32)(*px));
		(*pz++) = mul_add > MLIB_S16_MAX ? MLIB_S16_MAX : mul_add;
		px++;
		py++;
		len--;
	}

	dpz = (mlib_d64 *)pz;
	even_8 = len >> 3;
	rest_8 = len & 7;
	dpx = vis_alignaddr(px, 0);
	dpy = vis_alignaddr(py, 0);

	if (uc & 0x80) {
/*
 * uc > 128
 */
		fc = vis_to_float((uc ^ 0x7f) << 8);

		if (!((mlib_addr)px & 7)) {
			if (!((mlib_addr)py & 7)) {
#pragma pipeloop(0)
				for (i = 0; i < even_8; i++) {
					dx = dpx[i];
					dy = dpy[i];
					MUL_S_ADD_S16_U8_SAT_SAT;
					dzh = vis_for(dmasks[mask],
						vis_fand(dzh, dnotmasks[mask]));
					dzl = vis_for(dmasks[mask1],
						vis_fand(dzl,
						dnotmasks[mask1]));
					dpz[2 * i] = dzh;
					dpz[2 * i + 1] = dzl;
				}
			} else {
				vis_alignaddr(py, 0);
#pragma pipeloop(0)
				for (i = 0; i < even_8; i++) {
					dx = dpx[i];
					dy0 = dpy[i];
					dy1 = vis_ld_d64_nf(dpy + i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S_ADD_S16_U8_SAT_SAT;
					dzh = vis_for(dmasks[mask],
						vis_fand(dzh, dnotmasks[mask]));
					dzl = vis_for(dmasks[mask1],
						vis_fand(dzl,
						dnotmasks[mask1]));
					dpz[2 * i] = dzh;
					dpz[2 * i + 1] = dzl;
				}
			}
		} else {
			for (k = 0; k < (even_8 >> 8); k++) {
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + k * 256),
					256 * 8);
				(void *)vis_alignaddr(py, 0);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = x_buff[i];
					dy0 = dpy[k * 256 + i];
					dy1 = vis_ld_d64_nf(dpy + k * 256 +
						i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S_ADD_S16_U8_SAT_SAT;
					vis_pst_16(dzh,
						dpz + 512 * k + 2 * i,
						~mask);
					vis_pst_16(dzl,
						dpz + 512 * k + 2 * i + 1,
						~mask1);
					vis_pst_16(control_ovl,
						dpz + 512 * k + 2 * i,
						mask);
					vis_pst_16(control_ovl,
						dpz + 512 * k + 2 * i + 1,
						mask1);
				}
			}

			N = even_8 & ~255;
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)px + N), (even_8 - N) * 8);
			vis_alignaddr(py, 0);
#pragma pipeloop(0)
			for (i = N; i < even_8; i++) {
				dx = x_buff[i - N];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S_ADD_S16_U8_SAT_SAT;
				vis_pst_16(dzh, dpz + 2 * i, ~mask);
				vis_pst_16(dzl, dpz + 2 * i + 1, ~mask1);
				vis_pst_16(control_ovl, dpz + 2 * i, mask);
				vis_pst_16(control_ovl, dpz + 2 * i + 1, mask1);
			}
		}
	} else {
/*
 * uc <128
 */
		fc = vis_to_float((uc) << 8);

		if (!((mlib_addr)px & 7)) {
			if (!((mlib_addr)py & 7)) {
#pragma pipeloop(0)
				for (i = 0; i < even_8; i++) {
					dx = dpx[i];
					dy = dpy[i];
					MUL_S_ADD_S16_U8_SAT;
					dzh = vis_for(dmasks[mask],
						vis_fand(dzh, dnotmasks[mask]));
					dzl = vis_for(dmasks[mask1],
						vis_fand(dzl,
						dnotmasks[mask1]));
					dpz[2 * i] = dzh;
					dpz[2 * i + 1] = dzl;
				}
			} else {
				vis_alignaddr(py, 0);
#pragma pipeloop(0)
				for (i = 0; i < even_8; i++) {
					dx = dpx[i];
					dy0 = dpy[i];
					dy1 = vis_ld_d64_nf(dpy + i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S_ADD_S16_U8_SAT;
					dzh = vis_for(dmasks[mask],
						vis_fand(dzh, dnotmasks[mask]));
					dzl = vis_for(dmasks[mask1],
						vis_fand(dzl,
						dnotmasks[mask1]));
					dpz[2 * i] = dzh;
					dpz[2 * i + 1] = dzl;
				}
			}
		} else {
			for (k = 0; k < (even_8 >> 8); k++) {
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + k * 256),
					256 * 8);
				(void *)vis_alignaddr(py, 0);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = x_buff[i];
					dy0 = dpy[k * 256 + i];
					dy1 = vis_ld_d64_nf(dpy + k * 256 +
						i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S_ADD_S16_U8_SAT;
					vis_pst_16(dzh,
						dpz + 512 * k + 2 * i,
						~mask);
					vis_pst_16(dzl,
						dpz + 512 * k + 2 * i + 1,
						~mask1);
					vis_pst_16(control_ovl,
						dpz + 512 * k + 2 * i,
						mask);
					vis_pst_16(control_ovl,
						dpz + 512 * k + 2 * i + 1,
						mask1);
				}
			}

			N = even_8 & ~255;
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)px + N), (even_8 - N) * 8);
			vis_alignaddr(py, 0);
#pragma pipeloop(0)
			for (i = N; i < even_8; i++) {
				dx = x_buff[i - N];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S_ADD_S16_U8_SAT;
				vis_pst_16(dzh, dpz + 2 * i, ~mask);
				vis_pst_16(dzl, dpz + 2 * i + 1, ~mask1);
				vis_pst_16(control_ovl, dpz + 2 * i, mask);
				vis_pst_16(control_ovl, dpz + 2 * i + 1, mask1);
			}
		}
	}

	px += (even_8 << 3);
	py += (even_8 << 3);
	pz += (even_8 << 3);

	while (rest_8--) {
		mul_add = ((mlib_s32)(*py)) * uc + ((mlib_s32)(*px));
		(*pz++) = mul_add > MLIB_S16_MAX ? MLIB_S16_MAX : mul_add;
		py++;
		px++;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S_ADD_S16_U8_MOD_MOD                                \
	dr1 = vis_fmul8x16al(vis_read_hi(dy), fc);              \
	dr2 = vis_fmul8x16al(vis_read_lo(dy), fc);              \
	dr3 = vis_fmul8x16al(vis_read_hi(dy), f127);            \
	dr4 = vis_fmul8x16al(vis_read_lo(dy), f127);            \
	dx_hi = vis_fmul8x16al(vis_read_hi(dx), factor);        \
	dx_lo = vis_fpmerge(dzero, vis_read_lo(dx));            \
	dr3 = vis_fpsub16(dr3, dr1);                            \
	dr4 = vis_fpsub16(dr4, dr2);                            \
	dzh = vis_fpadd16(dx_hi, dr3);                          \
	dzl = vis_fpadd16(dx_lo, dr4);

#define	MUL_S_ADD_S16_U8_MOD                                    \
	dr1 = vis_fmul8x16al(vis_read_hi(dy), fc);              \
	dr2 = vis_fmul8x16al(vis_read_lo(dy), fc);              \
	dx_hi = vis_fmul8x16al(vis_read_hi(dx), factor);        \
	dx_lo = vis_fpmerge(dzero, vis_read_lo(dx));            \
	dzh = vis_fpadd16(dx_hi, dr1);                          \
	dzl = vis_fpadd16(dx_lo, dr2);

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16_U8_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_s32 len = n, i, k, N;
	mlib_d64 *dpx, *dpy, *dpz;
	mlib_u8 *px, *py;
	mlib_s16 *pz;
	mlib_d64 dx, dy, dx_hi, dx_lo, dr1, dr2, dr3, dr4, dzh, dzl, dy0, dy1;
	mlib_u16 uc = *((mlib_u8 *)c);
	mlib_f32 f127 = vis_to_float(0x7f00);
	mlib_f32 factor = vis_to_float(0x0100);
	mlib_f32 fc;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_d64 dzero = vis_fzero();
	mlib_d64 x_buff[256];

	if (n <= 0)
		return (MLIB_FAILURE);
	px = (mlib_u8 *)x;
	py = (mlib_u8 *)y;
	pz = (mlib_s16 *)z;

	if (n <= 8) {
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			(*pz++) = ((mlib_s32)(*py)) * uc + ((mlib_s32)(*px));
			px++;
			py++;
		}

		return (MLIB_SUCCESS);
	}

	while ((mlib_addr)pz & 7) {
		(*pz++) = ((mlib_s32)(*py)) * uc + ((mlib_s32)(*px));
		px++;
		py++;
		len--;
	}

	dpz = (mlib_d64 *)pz;
	even_8 = len >> 3;
	rest_8 = len & 7;
	dpx = vis_alignaddr(px, 0);
	dpy = vis_alignaddr(py, 0);

	if (uc & 0x80) {

/*
 * uc > 128
 */
		fc = vis_to_float((uc ^ 0x7f) << 8);

		if (!((mlib_addr)px & 7)) {
			if (!((mlib_addr)py & 7)) {
#pragma pipeloop(0)
				for (i = 0; i < even_8; i++) {
					dx = dpx[i];
					dy = dpy[i];
					MUL_S_ADD_S16_U8_MOD_MOD;
					dpz[2 * i] = dzh;
					dpz[2 * i + 1] = dzl;
				}
			} else {
				vis_alignaddr(py, 0);
#pragma pipeloop(0)
				for (i = 0; i < even_8; i++) {
					dx = dpx[i];
					dy0 = dpy[i];
					dy1 = vis_ld_d64_nf(dpy + i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S_ADD_S16_U8_MOD_MOD;
					dpz[2 * i] = dzh;
					dpz[2 * i + 1] = dzl;
				}
			}
		} else {
			for (k = 0; k < (even_8 >> 8); k++) {
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + k * 256),
					256 * 8);
				(void *)vis_alignaddr(py, 0);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = x_buff[i];
					dy0 = dpy[k * 256 + i];
					dy1 = vis_ld_d64_nf(dpy + k * 256 +
						i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S_ADD_S16_U8_MOD_MOD;
					dpz[512 * k + 2 * i] = dzh;
					dpz[512 * k + 2 * i + 1] = dzl;
				}
			}

			N = even_8 & ~255;
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)px + N), (even_8 - N) * 8);
			vis_alignaddr(py, 0);
#pragma pipeloop(0)
			for (i = N; i < even_8; i++) {
				dx = x_buff[i - N];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S_ADD_S16_U8_MOD_MOD;
				dpz[2 * i] = dzh;
				dpz[2 * i + 1] = dzl;
			}
		}
	} else {
/*
 * uc <128
 */
		fc = vis_to_float((uc) << 8);

		if (!((mlib_addr)px & 7)) {
			if (!((mlib_addr)py & 7)) {
#pragma pipeloop(0)
				for (i = 0; i < even_8; i++) {
					dx = dpx[i];
					dy = dpy[i];
					MUL_S_ADD_S16_U8_MOD;
					dpz[2 * i] = dzh;
					dpz[2 * i + 1] = dzl;
				}
			} else {
				vis_alignaddr(py, 0);
#pragma pipeloop(0)
				for (i = 0; i < even_8; i++) {
					dx = dpx[i];
					dy0 = dpy[i];
					dy1 = vis_ld_d64_nf(dpy + i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S_ADD_S16_U8_MOD;
					dpz[2 * i] = dzh;
					dpz[2 * i + 1] = dzl;
				}
			}
		} else {
			for (k = 0; k < (even_8 >> 8); k++) {
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + k * 256),
					256 * 8);
				(void *)vis_alignaddr(py, 0);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = x_buff[i];
					dy0 = dpy[k * 256 + i];
					dy1 = vis_ld_d64_nf(dpy + k * 256 +
						i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S_ADD_S16_U8_MOD;
					dpz[512 * k + 2 * i] = dzh;
					dpz[512 * k + 2 * i + 1] = dzl;
				}
			}

			N = even_8 & ~255;
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)px + N), (even_8 - N) * 8);
			vis_alignaddr(py, 0);
#pragma pipeloop(0)
			for (i = N; i < even_8; i++) {
				dx = x_buff[i - N];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S_ADD_S16_U8_MOD;
				dpz[2 * i] = dzh;
				dpz[2 * i + 1] = dzl;
			}
		}
	}

	px += (even_8 << 3);
	py += (even_8 << 3);
	pz += (even_8 << 3);

	while (rest_8--) {
		(*pz++) = ((mlib_s32)(*py)) * uc + ((mlib_s32)(*px));
		py++;
		px++;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S_ADD_S16_S8_MOD_I(i)                               \
	dyh##i = vis_fpmerge(vis_read_hi(dy##i), fzero);        \
	dyl##i = vis_fpmerge(vis_read_lo(dy##i), fzero);        \
	dyh##i = vis_fmul8sux16(dyh##i, dc);                    \
	dyl##i = vis_fmul8sux16(dyl##i, dc);                    \
	dxh##i = vis_fpmerge(vis_read_hi(dx##i), fzero);        \
	dxl##i = vis_fpmerge(vis_read_lo(dx##i), fzero);        \
	dxh##i = vis_fmul8x16(fone, dxh##i);                    \
	dxl##i = vis_fmul8x16(fone, dxl##i);                    \
	dxh##i = vis_fpadd16(dyh##i, dxh##i);                   \
	dxl##i = vis_fpadd16(dyl##i, dxl##i);

#define	MUL_S_ADD_S16_S8_MOD                                    \
	dr1 = vis_fpmerge(vis_read_hi(dy), fzero);              \
	dr2 = vis_fpmerge(vis_read_lo(dy), fzero);              \
	dr1 = vis_fmul8sux16(dr1, dc);                          \
	dr2 = vis_fmul8sux16(dr2, dc);                          \
	dx_hi = vis_fpmerge(vis_read_hi(dx), fzero);            \
	dx_lo = vis_fpmerge(vis_read_lo(dx), fzero);            \
	dx_hi = vis_fmul8x16(fone, dx_hi);                      \
	dx_lo = vis_fmul8x16(fone, dx_lo);                      \
	dzh = vis_fpadd16(dr1, dx_hi);                          \
	dzl = vis_fpadd16(dr2, dx_lo);

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16_S8_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_s32 len = n, i, k, N;
	mlib_d64 *dpx, *dpy, *dpz;
	mlib_s8 *px, *py;
	mlib_s16 *pz;
	mlib_d64 dx, dy, dyn, dx_hi, dx_lo, dr1, dr2, dzh, dzl, dx0, dx1, dy0,
		dy1;
	mlib_s16 uc = *((mlib_s8 *)c);
	mlib_d64 dxh0, dxh1, dyh0, dyh1;
	mlib_d64 dxl0, dxl1, dyl0, dyl1;
	mlib_d64 dc = vis_to_double_dup((uc << 24) | ((uc & 0xff) << 8));

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_f32 fzero = 0.0, fone = vis_to_float(0x01010101);
	mlib_d64 x_buff[256];

	if (n <= 0)
		return (MLIB_FAILURE);
	px = (mlib_s8 *)x;
	py = (mlib_s8 *)y;
	pz = (mlib_s16 *)z;

	if (n <= 8) {
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			(*pz++) = ((mlib_s32)(*py)) * uc + ((mlib_s32)(*px));
			px++;
			py++;
		}
		return (MLIB_SUCCESS);
	}

	while ((mlib_addr)pz & 7) {
		(*pz++) = ((mlib_s32)(*py)) * uc + ((mlib_s32)(*px));
		px++;
		py++;
		len--;
	}

	dpz = (mlib_d64 *)pz;
	even_8 = len >> 3;
	rest_8 = len & 0x7;
	len = even_8;
	dpx = vis_alignaddr(px, 0);
	dpy = vis_alignaddr(py, 0);

	if (!((mlib_addr)px & 7)) {
		if (!((mlib_addr)py & 7)) {
			dx0 = vis_ld_d64_nf(dpx);
			dy0 = vis_ld_d64_nf(dpy);
			dx1 = vis_ld_d64_nf(dpx + 1);
			dy1 = vis_ld_d64_nf(dpy + 1);
			MUL_S_ADD_S16_S8_MOD_I(0);
			MUL_S_ADD_S16_S8_MOD_I(1);
			dx0 = vis_ld_d64_nf(dpx + 2);
			dy0 = vis_ld_d64_nf(dpy + 2);
			dx1 = vis_ld_d64_nf(dpx + 3);
			dy1 = vis_ld_d64_nf(dpy + 3);

#pragma pipeloop(0)
			for (i = 0; i < len - 1; i += 2) {
				dpz[2 * i] = dxh0;
				dpz[2 * i + 1] = dxl0;
				dpz[2 * i + 2] = dxh1;
				dpz[2 * i + 3] = dxl1;
				MUL_S_ADD_S16_S8_MOD_I(0);
				MUL_S_ADD_S16_S8_MOD_I(1);
				dx0 = vis_ld_d64_nf(dpx + i + 4);
				dy0 = vis_ld_d64_nf(dpy + i + 4);
				dx1 = vis_ld_d64_nf(dpx + i + 5);
				dy1 = vis_ld_d64_nf(dpy + i + 5);
			}

			if (len & 1) {
				dpz[2 * len - 2] = dxh0;
				dpz[2 * len - 1] = dxl0;
			}
		} else {
			dx0 = vis_ld_d64_nf(dpx);
			dy0 = vis_ld_d64_nf(dpy);
			dyn = vis_ld_d64_nf(dpy + 1);
			dy0 = vis_faligndata(dy0, dyn);
			dx1 = vis_ld_d64_nf(dpx + 1);
			dy1 = vis_ld_d64_nf(dpy + 1);
			dyn = vis_ld_d64_nf(dpy + 2);
			dy1 = vis_faligndata(dy1, dyn);
			MUL_S_ADD_S16_S8_MOD_I(0);
			MUL_S_ADD_S16_S8_MOD_I(1);
			dx0 = vis_ld_d64_nf(dpx + 2);
			dy0 = vis_ld_d64_nf(dpy + 2);
			dyn = vis_ld_d64_nf(dpy + 3);
			dy0 = vis_faligndata(dy0, dyn);
			dx1 = vis_ld_d64_nf(dpx + 3);
			dy1 = vis_ld_d64_nf(dpy + 3);
			dyn = vis_ld_d64_nf(dpy + 4);
			dy1 = vis_faligndata(dy1, dyn);

#pragma pipeloop(0)
			for (i = 0; i < len - 1; i += 2) {
				dpz[2 * i] = dxh0;
				dpz[2 * i + 1] = dxl0;
				dpz[2 * i + 2] = dxh1;
				dpz[2 * i + 3] = dxl1;
				MUL_S_ADD_S16_S8_MOD_I(0);
				MUL_S_ADD_S16_S8_MOD_I(1);
				dx0 = vis_ld_d64_nf(dpx + i + 4);
				dy0 = vis_ld_d64_nf(dpy + i + 4);
				dyn = vis_ld_d64_nf(dpy + i + 5);
				dy0 = vis_faligndata(dy0, dyn);
				dx1 = vis_ld_d64_nf(dpx + i + 5);
				dy1 = vis_ld_d64_nf(dpy + i + 5);
				dyn = vis_ld_d64_nf(dpy + i + 6);
				dy1 = vis_faligndata(dy1, dyn);
			}

			if (len & 1) {
				dpz[2 * len - 2] = dxh0;
				dpz[2 * len - 1] = dxl0;
			}
		}
	} else {
		for (k = 0; k < (even_8 >> 8); k++) {
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)px + k * 256), 256 * 8);
			(void *)vis_alignaddr(py, 0);
#pragma pipeloop(0)
			for (i = 0; i < 256; i++) {
				dx = x_buff[i];
				dy0 = dpy[k * 256 + i];
				dy1 = vis_ld_d64_nf(dpy + k * 256 + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S_ADD_S16_S8_MOD;
				dpz[512 * k + 2 * i] = dzh;
				dpz[512 * k + 2 * i + 1] = dzl;
			}
		}

		N = even_8 & ~255;
		__mlib_VectorCopy_U8((void *)x_buff,
			(void *)((mlib_d64 *)px + N), (even_8 - N) * 8);
		vis_alignaddr(py, 0);
		dx0 = x_buff[0];
		dy0 = vis_ld_d64_nf(dpy + N);
		dyn = vis_ld_d64_nf(dpy + N + 1);
		dy0 = vis_faligndata(dy0, dyn);
		dx1 = x_buff[1];
		dy1 = vis_ld_d64_nf(dpy + N + 1);
		dyn = vis_ld_d64_nf(dpy + N + 2);
		dy1 = vis_faligndata(dy1, dyn);
		MUL_S_ADD_S16_S8_MOD_I(0);
		MUL_S_ADD_S16_S8_MOD_I(1);
		dx0 = x_buff[2];
		dy0 = vis_ld_d64_nf(dpy + N + 2);
		dyn = vis_ld_d64_nf(dpy + N + 3);
		dy0 = vis_faligndata(dy0, dyn);
		dx1 = x_buff[3];
		dy1 = vis_ld_d64_nf(dpy + N + 3);
		dyn = vis_ld_d64_nf(dpy + N + 4);
		dy1 = vis_faligndata(dy1, dyn);
#pragma pipeloop(0)
		for (i = N; i < len - 1; i += 2) {
			dpz[2 * i] = dxh0;
			dpz[2 * i + 1] = dxl0;
			dpz[2 * i + 2] = dxh1;
			dpz[2 * i + 3] = dxl1;
			MUL_S_ADD_S16_S8_MOD_I(0);
			MUL_S_ADD_S16_S8_MOD_I(1);
			dx0 = x_buff[i - N + 4];
			dy0 = vis_ld_d64_nf(dpy + i + 4);
			dyn = vis_ld_d64_nf(dpy + i + 5);
			dy0 = vis_faligndata(dy0, dyn);
			dx1 = x_buff[i - N + 5];
			dy1 = vis_ld_d64_nf(dpy + i + 5);
			dyn = vis_ld_d64_nf(dpy + i + 6);
			dy1 = vis_faligndata(dy1, dyn);
		}

		if (len & 1) {
			dpz[2 * len - 2] = dxh0;
			dpz[2 * len - 1] = dxl0;
		}
	}

	px += (even_8 << 3);
	py += (even_8 << 3);
	pz += (even_8 << 3);

	while (rest_8--) {
		(*pz++) = ((mlib_s32)(*py)) * uc + ((mlib_s32)(*px));
		py++;
		px++;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S_ADD_S32_S16_MOD                                      \
	dr1 = vis_fmuld8sux16(vis_read_hi(dy), vis_read_hi(dc));   \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dy), vis_read_hi(dc));   \
	dr3 = vis_fmuld8sux16(vis_read_lo(dy), vis_read_lo(dc));   \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dy), vis_read_lo(dc));   \
	dx_hi = vis_fmuld8ulx16(fone, vis_read_hi(dx));            \
	dx_lo = vis_fmuld8ulx16(fone, vis_read_lo(dx));            \
	dr2 = vis_fpadd32(dr1, dr2);                               \
	dr4 = vis_fpadd32(dr3, dr4);                               \
	dzh = vis_fpadd32(dr2, dx_hi);                             \
	dzl = vis_fpadd32(dr4, dx_lo);

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32_S16_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_d64 *dpx, *dpy, *dpz;
	mlib_s16 *px, *py;
	mlib_s32 *pz;
	mlib_d64 dx, dy, dx_hi, dx_lo, dr1, dr2, dr3, dr4, dzh, dzl, dy0, dy1;
	mlib_s16 uc = *((mlib_s16 *)c);
	mlib_d64 dc = vis_to_double_dup((uc << 16) | ((uc) & 0xffff));

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_f32 fone = vis_to_float(0x10001);
	mlib_s32 len = n, i, k, N;
	mlib_d64 x_buff[256];

	px = (mlib_s16 *)x;
	py = (mlib_s16 *)y;
	pz = (mlib_s32 *)z;

	if (n <= 8) {
		if (n <= 0)
			return (MLIB_FAILURE);
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			(*pz++) = ((mlib_s32)(*py)) * uc + ((mlib_s32)(*px));
			px++;
			py++;
		}
		return (MLIB_SUCCESS);
	}

	while ((mlib_addr)pz & 7) {
		(*pz++) = ((mlib_s32)(*py)) * uc + ((mlib_s32)(*px));
		px++;
		py++;
		len--;
	}

	dpz = (mlib_d64 *)pz;

	even_8 = len >> 2;
	rest_8 = len & 0x3;
	dpx = (mlib_d64 *)((mlib_addr)px & ~7);
	dpy = vis_alignaddr(py, 0);

	if (!((mlib_addr)px & 7)) {
		if (!((mlib_addr)py & 7)) {
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = dpx[i];
				dy = dpy[i];
				MUL_S_ADD_S32_S16_MOD;
				dpz[2 * i] = dzh;
				dpz[2 * i + 1] = dzl;
			}
		} else {
			(void *)(mlib_d64 *)vis_alignaddr(py, 0);
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = dpx[i];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S_ADD_S32_S16_MOD;
				dpz[2 * i] = dzh;
				dpz[2 * i + 1] = dzl;
			}
		}
	} else {
		for (k = 0; k < (even_8 >> 8); k++) {
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)px + k * 256), 256 * 8);
			(void *)vis_alignaddr(py, 0);
#pragma pipeloop(0)
			for (i = 0; i < 256; i++) {
				dx = x_buff[i];
				dy0 = dpy[k * 256 + i];
				dy1 = vis_ld_d64_nf(dpy + k * 256 + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S_ADD_S32_S16_MOD;
				dpz[512 * k + 2 * i] = dzh;
				dpz[512 * k + 2 * i + 1] = dzl;
			}
		}

		N = even_8 & ~255;
		__mlib_VectorCopy_U8((void *)x_buff,
			(void *)((mlib_d64 *)px + N), (even_8 - N) * 8);
		vis_alignaddr(py, 0);
#pragma pipeloop(0)
		for (i = N; i < even_8; i++) {
			dx = x_buff[i - N];
			dy0 = dpy[i];
			dy1 = vis_ld_d64_nf(dpy + i + 1);
			dy = vis_faligndata(dy0, dy1);
			MUL_S_ADD_S32_S16_MOD;
			dpz[2 * i] = dzh;
			dpz[2 * i + 1] = dzl;
		}
	}

	px += (even_8 << 2);
	py += (even_8 << 2);
	pz += (even_8 << 2);

	while (rest_8--) {
		(*pz++) = ((mlib_s32)(*py)) * uc + ((mlib_s32)(*px));
		py++;
		px++;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
