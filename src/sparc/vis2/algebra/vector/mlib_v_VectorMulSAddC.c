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

#pragma ident	"@(#)mlib_v_VectorMulSAddC.c	9.5	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorMulSAdd_U8C_U8C_Sat   - linear function on unsigned 8-bit
 *                                         format complex vector
 *      mlib_VectorMulSAdd_U8C_Sat       - linear function on unsigned 8-bit
 *                                         format complex vector
 *                                         (inplace version)
 *      mlib_VectorMulSAdd_S8C_S8C_Sat   - linear function on signed 8-bit
 *                                         format complex vector
 *      mlib_VectorMulSAdd_S8C_Sat       - linear function on signed 8-bit
 *                                         format complex vector
 *                                         (inplace version)
 *      mlib_VectorMulSAdd_S16C_S16C_Sat - linear function on signed 16-bit
 *                                         format complex vector
 *      mlib_VectorMulSAdd_S16C_Sat      - linear function on signed 16-bit
 *                                         format complex vector
 *                                         (inplace version)
 *      mlib_VectorMulSAdd_S32C_S32C_Sat - linear function on signed 32-bit
 *                                         format complex vector
 *      mlib_VectorMulSAdd_S32C_Sat      - linear function on signed 32-bit
 *                                         format complex vector
 *                                         (inplace version)
 *
 *      mlib_VectorMulSAdd_U8C_U8C_Mod   - linear function on unsigned 8-bit
 *                                         format complex vector
 *      mlib_VectorMulSAdd_U8C_Mod       - linear function on unsigned 8-bit
 *                                         format complex vector
 *                                         (inplace version)
 *      mlib_VectorMulSAdd_S8C_S8C_Mod   - linear function on signed 8-bit
 *                                         format complex vector
 *      mlib_VectorMulSAdd_S8C_Mod       - linear function on signed 8-bit
 *                                         format complex vector
 *                                         (inplace version)
 *      mlib_VectorMulSAdd_S16C_S16C_Mod - linear function on signed 16-bit
 *                                         format complex vector
 *      mlib_VectorMulSAdd_S16C_Mod      - linear function on signed 16-bit
 *                                         format complex vector
 *                                         (inplace version)
 *      mlib_VectorMulSAdd_S32C_S32C_Mod - linear function on signed 32-bit
 *                                         format complex vector
 *      mlib_VectorMulSAdd_S32C_Mod      - linear function on signed 32-bit
 *                                         format complex vector
 *                                         (inplace version)
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMulSAdd_S16C_[Sat|Mod](mlib_s16       *xz,
 *                                                    const mlib_s16 *y,
 *                                                    const mlib_s16 *c,
 *                                                    mlib_s32       n);
 *      mlib_status mlib_VectorMulSAdd_S16C_S16C_[Sat|Mod](mlib_s16       *z,
 *                                                         const mlib_s16 *x,
 *                                                         const mlib_s16 *y,
 *                                                         const mlib_s16 *c,
 *                                                         mlib_s32       n);
 *      mlib_status mlib_VectorMulSAdd_S16C_S8C_[Sat|Mod](mlib_s16      *z,
 *                                                        const mlib_s8 *x,
 *                                                        const mlib_s8 *y,
 *                                                        const mlib_s8 *c,
 *                                                        mlib_s32      n);
 *      mlib_status mlib_VectorMulSAdd_S16C_U8C_[Sat|Mod](mlib_s16      *z,
 *                                                        const mlib_u8 *x,
 *                                                        const mlib_u8 *y,
 *                                                        const mlib_u8 *c,
 *                                                        mlib_s32      n);
 *      mlib_status mlib_VectorMulSAdd_S32C_[Sat|Mod](mlib_s32       *xz,
 *                                                    const mlib_s32 *y,
 *                                                    const mlib_s32 *c,
 *                                                    mlib_s32       n);
 *      mlib_status mlib_VectorMulSAdd_S32C_S16C_[Sat|Mod](mlib_s32       *z,
 *                                                         const mlib_s16 *x,
 *                                                         const mlib_s16 *y,
 *                                                         const mlib_s16 *c,
 *                                                         mlib_s32       n);
 *      mlib_status mlib_VectorMulSAdd_S32C_S32C_[Sat|Mod](mlib_s32       *z,
 *                                                         const mlib_s32 *x,
 *                                                         const mlib_s32 *y,
 *                                                         const mlib_s32 *c,
 *                                                         mlib_s32       n);
 *      mlib_status mlib_VectorMulSAdd_S8C_[Sat|Mod](mlib_s8       *xz,
 *                                                   const mlib_s8 *y,
 *                                                   const mlib_s8 *c,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_VectorMulSAdd_S8C_S8C_[Sat|Mod](mlib_s8       *z,
 *                                                       const mlib_s8 *x,
 *                                                       const mlib_s8 *y,
 *                                                       const mlib_s8 *c,
 *                                                       mlib_s32      n);
 *      mlib_status mlib_VectorMulSAdd_U8C_[Sat|Mod](mlib_u8       *xz,
 *                                                   const mlib_u8 *y,
 *                                                   const mlib_u8 *c,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_VectorMulSAdd_U8C_U8C_[Sat|Mod](mlib_u8       *z,
 *                                                       const mlib_u8 *x,
 *                                                       const mlib_u8 *y,
 *                                                       const mlib_u8 *c,
 *                                                       mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result vector.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts
 *      x    pointer to the first element of the first input complex vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      y    pointer to the first element of the second input complex vector.
 *           y[2*i] hold the real parts, and y[2*i + 1] hold the imaginary
 *           parts
 *      c    pointer to a complex input scalar. c[0] hold the real part,
 *           and c[1] hold the imaginary part
 *      n    number of complex elements in the vectors
 *
 * DESCRIPTION
 *      z = x + c * y
 */

#include <stdlib.h>
#include <mlib_algebra.h>
#include <mlib_SysMath.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorMulSAdd_S32C_Sat = __mlib_VectorMulSAdd_S32C_Sat
#pragma weak mlib_VectorMulSAdd_S8C_S8C_Sat = \
	__mlib_VectorMulSAdd_S8C_S8C_Sat
#pragma weak mlib_VectorMulSAdd_S16C_Sat = __mlib_VectorMulSAdd_S16C_Sat
#pragma weak mlib_VectorMulSAdd_U8C_U8C_Sat = \
	__mlib_VectorMulSAdd_U8C_U8C_Sat
#pragma weak mlib_VectorMulSAdd_S16C_S8C_Mod = \
	__mlib_VectorMulSAdd_S16C_S8C_Mod
#pragma weak mlib_VectorMulSAdd_S16C_U8C_Mod = \
	__mlib_VectorMulSAdd_S16C_U8C_Mod
#pragma weak mlib_VectorMulSAdd_S8C_Mod = __mlib_VectorMulSAdd_S8C_Mod
#pragma weak mlib_VectorMulSAdd_U8C_Mod = __mlib_VectorMulSAdd_U8C_Mod
#pragma weak mlib_VectorMulSAdd_S16C_S8C_Sat = \
	__mlib_VectorMulSAdd_S16C_S8C_Sat
#pragma weak mlib_VectorMulSAdd_S16C_U8C_Sat = \
	__mlib_VectorMulSAdd_S16C_U8C_Sat
#pragma weak mlib_VectorMulSAdd_S32C_S32C_Mod = \
	__mlib_VectorMulSAdd_S32C_S32C_Mod
#pragma weak mlib_VectorMulSAdd_S8C_Sat = __mlib_VectorMulSAdd_S8C_Sat
#pragma weak mlib_VectorMulSAdd_S32C_S16C_Mod = \
	__mlib_VectorMulSAdd_S32C_S16C_Mod
#pragma weak mlib_VectorMulSAdd_U8C_Sat = __mlib_VectorMulSAdd_U8C_Sat
#pragma weak mlib_VectorMulSAdd_S16C_S16C_Mod = \
	__mlib_VectorMulSAdd_S16C_S16C_Mod
#pragma weak mlib_VectorMulSAdd_S8C_S8C_Mod = \
	__mlib_VectorMulSAdd_S8C_S8C_Mod
#pragma weak mlib_VectorMulSAdd_S32C_Mod = __mlib_VectorMulSAdd_S32C_Mod
#pragma weak mlib_VectorMulSAdd_S32C_S32C_Sat = \
	__mlib_VectorMulSAdd_S32C_S32C_Sat
#pragma weak mlib_VectorMulSAdd_S16C_Mod = __mlib_VectorMulSAdd_S16C_Mod
#pragma weak mlib_VectorMulSAdd_S32C_S16C_Sat = \
	__mlib_VectorMulSAdd_S32C_S16C_Sat
#pragma weak mlib_VectorMulSAdd_U8C_U8C_Mod = \
	__mlib_VectorMulSAdd_U8C_U8C_Mod
#pragma weak mlib_VectorMulSAdd_S16C_S16C_Sat = \
	__mlib_VectorMulSAdd_S16C_S16C_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorMulSAdd_S32C_Sat) mlib_VectorMulSAdd_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S32C_Sat")));
__typeof__(__mlib_VectorMulSAdd_S8C_S8C_Sat) mlib_VectorMulSAdd_S8C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S8C_S8C_Sat")));
__typeof__(__mlib_VectorMulSAdd_S16C_Sat) mlib_VectorMulSAdd_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16C_Sat")));
__typeof__(__mlib_VectorMulSAdd_U8C_U8C_Sat) mlib_VectorMulSAdd_U8C_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_U8C_U8C_Sat")));
__typeof__(__mlib_VectorMulSAdd_S16C_S8C_Mod) mlib_VectorMulSAdd_S16C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16C_S8C_Mod")));
__typeof__(__mlib_VectorMulSAdd_S16C_U8C_Mod) mlib_VectorMulSAdd_S16C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16C_U8C_Mod")));
__typeof__(__mlib_VectorMulSAdd_S8C_Mod) mlib_VectorMulSAdd_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S8C_Mod")));
__typeof__(__mlib_VectorMulSAdd_U8C_Mod) mlib_VectorMulSAdd_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_U8C_Mod")));
__typeof__(__mlib_VectorMulSAdd_S16C_S8C_Sat) mlib_VectorMulSAdd_S16C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16C_S8C_Sat")));
__typeof__(__mlib_VectorMulSAdd_S16C_U8C_Sat) mlib_VectorMulSAdd_S16C_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16C_U8C_Sat")));
__typeof__(__mlib_VectorMulSAdd_S32C_S32C_Mod) mlib_VectorMulSAdd_S32C_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S32C_S32C_Mod")));
__typeof__(__mlib_VectorMulSAdd_S8C_Sat) mlib_VectorMulSAdd_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S8C_Sat")));
__typeof__(__mlib_VectorMulSAdd_S32C_S16C_Mod) mlib_VectorMulSAdd_S32C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S32C_S16C_Mod")));
__typeof__(__mlib_VectorMulSAdd_U8C_Sat) mlib_VectorMulSAdd_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_U8C_Sat")));
__typeof__(__mlib_VectorMulSAdd_S16C_S16C_Mod) mlib_VectorMulSAdd_S16C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16C_S16C_Mod")));
__typeof__(__mlib_VectorMulSAdd_S8C_S8C_Mod) mlib_VectorMulSAdd_S8C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S8C_S8C_Mod")));
__typeof__(__mlib_VectorMulSAdd_S32C_Mod) mlib_VectorMulSAdd_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S32C_Mod")));
__typeof__(__mlib_VectorMulSAdd_S32C_S32C_Sat) mlib_VectorMulSAdd_S32C_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S32C_S32C_Sat")));
__typeof__(__mlib_VectorMulSAdd_S16C_Mod) mlib_VectorMulSAdd_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16C_Mod")));
__typeof__(__mlib_VectorMulSAdd_S32C_S16C_Sat) mlib_VectorMulSAdd_S32C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S32C_S16C_Sat")));
__typeof__(__mlib_VectorMulSAdd_U8C_U8C_Mod) mlib_VectorMulSAdd_U8C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_U8C_U8C_Mod")));
__typeof__(__mlib_VectorMulSAdd_S16C_S16C_Sat) mlib_VectorMulSAdd_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16C_S16C_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MUL_S_ADD_U8C_SAT                                        \
	dr1 = vis_fpmerge(vis_read_hi(dy), vis_read_lo(dy));     \
	dr1 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));   \
	dx_r = vis_bshuffle(dx, vis_fzero());                    \
	dx_i = vis_fandnot(mask_for_r, dx);                      \
	dr2 = vis_fmul8x16au(vis_read_hi(dr1), fc);              \
	dr3 = vis_fmul8x16al(vis_read_hi(dr1), fc);              \
	dr4 = vis_fmul8x16au(vis_read_lo(dr1), fc);              \
	dr5 = vis_fmul8x16al(vis_read_lo(dr1), fc);              \
	dr2 = vis_fpsub16(dr2, dr5);                             \
	dr3 = vis_fpadd16(dr3, dr4);                             \
	dr2 = vis_fpadd16(dr2, dx_r);                            \
	dr3 = vis_fpadd16(dr3, dx_i);                            \
	dr = vis_fpack16_pair(dr2, dr3);                         \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr))

/* *********************************************************** */

#define	MUL_S_ADD_U8C_SAT_1                                      \
	dr1 = vis_fpmerge(vis_read_hi(dy), vis_read_lo(dy));     \
	dr1 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));   \
	dx_r = vis_fand(mask_for_r, dx);                         \
	dx_r = vis_bshuffle(dx_r, dx_r);                         \
	dx_i = vis_fandnot(mask_for_r, dx);                      \
	dr2 = vis_fmul8x16au(vis_read_hi(dr1), fc);              \
	dr3 = vis_fmul8x16al(vis_read_hi(dr1), fc);              \
	dr4 = vis_fmul8x16au(vis_read_lo(dr1), fc);              \
	dr5 = vis_fmul8x16al(vis_read_lo(dr1), fc);              \
	dr2 = vis_fpsub16(dr2, dr5);                             \
	dr3 = vis_fpadd16(dr3, dr4);                             \
	dr2 = vis_fpadd16(dr2, dx_r);                            \
	dr3 = vis_fpadd16(dr3, dx_i);                            \
	dr = vis_fpack16_pair(dr2, dr3);                         \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr))

/* *********************************************************** */

#define	MUL_S_ADD_U8C_SAT_SAT                                   \
	dr9 = vis_bshuffle(dy, dy);                             \
	dr1 = vis_fmuld8ulx16(vis_read_hi(dy), fc_r);           \
	dr2 = vis_fmuld8ulx16(vis_read_lo(dy), fc_r);           \
	dr3 = vis_fmuld8ulx16(vis_read_hi(dr9), fc_r);          \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dr9), fc_r);          \
	dr5 = vis_fmuld8ulx16(vis_read_hi(dy), fc_i);           \
	dr6 = vis_fmuld8ulx16(vis_read_lo(dy), fc_i);           \
	dr7 = vis_fmuld8ulx16(vis_read_hi(dr9), fc_i);          \
	dr8 = vis_fmuld8ulx16(vis_read_lo(dr9), fc_i);          \
	dx_i = vis_fandnot(mask_for_r, dx);                     \
	dx_r = vis_fand(mask_for_r, dx);                        \
	dx_r = vis_bshuffle(dx_r, dx_r);                        \
	dx_r_hi = vis_fpmerge(dzero, vis_read_hi(dx_r));        \
	dx_r_lo = vis_fpmerge(dzero, vis_read_lo(dx_r));        \
	dx_i_hi = vis_fmuld8ulx16(fone, vis_read_hi(dx_i));     \
	dx_i_lo = vis_fmuld8ulx16(fone, vis_read_lo(dx_i));     \
	dr3 = vis_fpsub32(dr3, dr5);                            \
	dr4 = vis_fpsub32(dr4, dr6);                            \
	dr1 = vis_fpadd32(dr1, dr7);                            \
	dr2 = vis_fpadd32(dr2, dr8);                            \
	dr7 = vis_fpadd32(dr3, dx_r_hi);                        \
	dr8 = vis_fpadd32(dr4, dx_r_lo);                        \
	dr9 = vis_fpadd32(dr1, dx_i_hi);                        \
	dr10 = vis_fpadd32(dr2, dx_i_lo);                       \
	dr = vis_fpack32(vis_fzero(), dr7);                     \
	dr = vis_fpack32(dr, dr8);                              \
	dr = vis_fpack32(dr, dr9);                              \
	dr = vis_fpack32(dr, dr10);                             \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));     \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr))

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(DST, SRC)	DST = ((mlib_s32)(SRC))

#else

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
__mlib_VectorMulSAdd_U8C_U8C_Sat(
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
	mlib_s8 *pzend = (mlib_s8 *)z + n + n - 1;
	mlib_u16 uc_r = ((mlib_u8 *)c)[0], uc_i = ((mlib_u8 *)c)[1];

	mlib_d64 *dpx, *dpy, *dpz, *dpzend;
	mlib_d64 dx, dy, dx1, dy1, dx0, dy0;
	mlib_d64 dr, dr1, dr2, dr3, dr4, dr5, dr6, dr7, dr8, dr9;
	mlib_d64 dr10, dx_r, dx_i, dx_r_hi, dx_r_lo, dx_i_hi, dx_i_lo;
	mlib_d64 mask_for_r = vis_to_double_dup(0xff00ff00);
	mlib_f32 dzero = vis_fzeros(), fone = vis_to_float(0x10001);

	if (n <= 0)
		return (MLIB_FAILURE);

/*
 * prepare the destination address
 */
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z + (1 & (mlib_addr)z);

/*
 * generate edge mask for the start bytes
 */
	emask = vis_edge8(z, pzend);
/*
 * prepare the source address
 */

	if ((uc_r + uc_i) > MLIB_S8_MAX) {
		mlib_f32 fc_r = vis_to_float((uc_r << 16) | uc_r);
		mlib_f32 fc_i = vis_to_float((uc_i << 16) | uc_i);

		vis_write_gsr64(((mlib_s64)0x789abcde << 32) | (23 << 3));

		dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
		dx0 = dx1;
		dpy = (mlib_d64 *)vis_alignaddr((void *)y, off);
		dy0 = vis_ld_d64_nf(dpy);
		dy1 = vis_ld_d64_nf(dpy + 1);
		dy = vis_faligndata(dy0, dy1);
		dy0 = dy1;

		MUL_S_ADD_U8C_SAT_SAT;

		if (!(1 & (mlib_addr)z)) {
/* store first bytes of result */
			vis_pst_8(dr, dpz, emask);
			dpz++;
		} else {
			dr = vis_bshuffle(dr, dr);
/* store first 7 bytes of result */
			vis_pst_8(dr, dpz, emask);

			if ((mlib_addr)++dpz <= (mlib_addr)pzend)
				vis_pst_8(dr, dpz, 0x80);
		}

		len = (mlib_s32)((mlib_addr)dpzend - (mlib_addr)dpz);

		if (!(1 & (mlib_addr)z)) {

			if ((((mlib_addr)x ^ (mlib_addr)z) & 7) == 0) {
/*
 * while((mlib_u32) dpz < (mlib_u32) dpzend)
 */
				vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
				for (i = 0; i < len; i += 8) {
					dx = dpx[1];
					dy1 = vis_ld_d64_nf(dpy + 2);
					dy = vis_faligndata(dy0, dy1);
					dy0 = dy1;
					MUL_S_ADD_U8C_SAT_SAT;
					dpz[0] = dr;
					dpx++;
					dpy++;
					dpz++;
				}

				dx = vis_ld_d64_nf(dpx + 1);
				dy1 = vis_ld_d64_nf(dpy + 2);
				dy = vis_faligndata(dy0, dy1);
			} else {
/*
 * while ((mlib_u32) dpz < (mlib_u32) dpzend)
 */
#pragma pipeloop(0)
				for (i = 0; i < len; i += 8) {
					vis_alignaddr((void *)x, off);
					dx1 = vis_ld_d64_nf(dpx + 2);
					dx = vis_faligndata(dx0, dx1);
					dx0 = dx1;
					vis_alignaddr((void *)y, off);
					dy1 = vis_ld_d64_nf(dpy + 2);
					dy = vis_faligndata(dy0, dy1);
					dy0 = dy1;

					MUL_S_ADD_U8C_SAT_SAT;
/* store 8 bytes of result */
					dpz[0] = dr;
					dpx++;
					dpy++;
					dpz++;
				}

				(void *)vis_alignaddr((void *)x, off);
				dx1 = vis_ld_d64_nf(dpx + 2);
				dx = vis_faligndata(dx0, dx1);
				(void *)vis_alignaddr((void *)y, off);
				dy1 = vis_ld_d64_nf(dpy + 2);
				dy = vis_faligndata(dy0, dy1);
			}

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				MUL_S_ADD_U8C_SAT_SAT;
/* prepare edge mask for the last bytes */
				emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
				vis_pst_8(dr, dpz, emask);
			}
		} else {
/*
 * while ((mlib_u32) dpz < (mlib_u32) dpzend)
 */

#pragma pipeloop(0)
			for (i = 0; i < len; i += 8) {
				vis_alignaddr((void *)x, off);
				dx1 = vis_ld_d64_nf(dpx + 2);
				dx = vis_faligndata(dx0, dx1);
				dx0 = dx1;
				vis_alignaddr((void *)y, off);
				dy1 = vis_ld_d64_nf(dpy + 2);
				dy = vis_faligndata(dy0, dy1);
				dy0 = dy1;

				MUL_S_ADD_U8C_SAT_SAT;
				dr = vis_bshuffle(dr, dr);
/* store 7 bytes of result */
				vis_pst_8(dr, dpz, 0x7f);
				dpz++;
/* store 1 byte of result to the next word */
				vis_pst_8(dr, dpz, 0x80);
				dpx++;
				dpy++;
			}

			if ((mlib_addr)dpz < (mlib_addr)pzend) {
				vis_alignaddr((void *)x, off);
				dx1 = vis_ld_d64_nf(dpx + 2);
				dx = vis_faligndata(dx0, dx1);
				dx0 = dx1;
				vis_alignaddr((void *)y, off);
				dy1 = vis_ld_d64_nf(dpy + 2);
				dy = vis_faligndata(dy0, dy1);
				dy0 = dy1;

				MUL_S_ADD_U8C_SAT_SAT;
/* prepare edge mask for the last bytes */
				emask = vis_edge8(dpz, pzend);
				dr = vis_bshuffle(dr, dr);
/* store 7 bytes of result */
				vis_pst_8(dr, dpz, emask & 0x7f);
			}
		}
	}
/*
 * SMALL SCALE FACTORS
 */
	else {
		mlib_f32 fc = vis_to_float((uc_r << 24) | (uc_i << 8));

		dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
		dx0 = dx1;
		dpy = (mlib_d64 *)vis_alignaddr((void *)y, off);
		dy0 = vis_ld_d64_nf(dpy);
		dy1 = vis_ld_d64_nf(dpy + 1);
		dy = vis_faligndata(dy0, dy1);
		dy0 = dy1;

		vis_write_gsr64(((mlib_u64)0x80828486 << 32) | (7 << 3) | 7);

		MUL_S_ADD_U8C_SAT;

		if (!(1 & (mlib_addr)z)) {
/* store first bytes of result */
			vis_pst_8(dr, dpz, emask);
			dpz++;
		} else {
			dr = vis_faligndata(dr, dr);
/* store first 7 bytes of result */
			vis_pst_8(dr, dpz, emask);

			if ((mlib_addr)++dpz <= (mlib_addr)pzend)
				vis_pst_8(dr, dpz, 0x80);
		}

		if (!(1 & (mlib_addr)z)) {
			if ((((mlib_addr)x ^ (mlib_addr)z) & 7) == 0) {
				vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
				for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
					dx = dpx[1];
					dy1 = vis_ld_d64_nf(dpy + 2);
					dy = vis_faligndata(dy0, dy1);
					dy0 = dy1;
					MUL_S_ADD_U8C_SAT;
					dpz[0] = dr;
					dpx++;
					dpy++;
					dpz++;
				}

				dx = vis_ld_d64_nf(dpx + 1);
				dy1 = vis_ld_d64_nf(dpy + 2);
				dy = vis_faligndata(dy0, dy1);
			} else {
#pragma pipeloop(0)
				for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
					dx1 = vis_ld_d64_nf(dpx + 2);
					vis_alignaddr((void *)x, off);
					dx = vis_faligndata(dx0, dx1);
					dx0 = dx1;
					vis_alignaddr((void *)y, off);
					dy1 = vis_ld_d64_nf(dpy + 2);
					dy = vis_faligndata(dy0, dy1);
					dy0 = dy1;
					MUL_S_ADD_U8C_SAT;
/* store 8 bytes of result */
					dpz[0] = dr;
					dpy++;
					dpx++;
					dpz++;
				}

				dx1 = vis_ld_d64_nf(dpx + 2);
				vis_alignaddr((void *)x, off);
				dx = vis_faligndata(dx0, dx1);
				vis_alignaddr((void *)y, off);
				dy1 = vis_ld_d64_nf(dpy + 2);
				dy = vis_faligndata(dy0, dy1);
			}

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				MUL_S_ADD_U8C_SAT;
/* prepare edge mask for the last bytes */
				emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
				vis_pst_8(dr, dpz, emask);
			}

		} else {
			vis_write_bmask(0x789abcde, 0);
#pragma pipeloop(0)
			for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
				dx1 = vis_ld_d64_nf(dpx + 2);
				vis_alignaddr((void *)x, off);
				dx = vis_faligndata(dx0, dx1);
				dx0 = dx1;
				vis_alignaddr((void *)y, off);
				dy1 = vis_ld_d64_nf(dpy + 2);
				dy = vis_faligndata(dy0, dy1);
				dy0 = dy1;
				MUL_S_ADD_U8C_SAT_1;
				dr = vis_bshuffle(dr, dr);
/* store 7 bytes of result */
				vis_pst_8(dr, dpz, 0x7f);
				dpz++;
/* store 1 byte of result to the next word */
				vis_pst_8(dr, dpz, 0x80);
				dpx++;
				dpy++;
			}

			if ((mlib_addr)dpz < (mlib_addr)pzend) {
				dx1 = vis_ld_d64_nf(dpx + 2);
				vis_alignaddr((void *)x, off);
				dx = vis_faligndata(dx0, dx1);
				dx0 = dx1;
				vis_alignaddr((void *)y, off);
				dy1 = vis_ld_d64_nf(dpy + 2);
				dy = vis_faligndata(dy0, dy1);
				dy0 = dy1;
				MUL_S_ADD_U8C_SAT_1;
				dr = vis_bshuffle(dr, dr);
/* prepare edge mask for the last bytes */
				emask = vis_edge8(dpz, pzend);
/* store 7 bytes of result */
				vis_pst_8(dr, dpz, emask & 0x7f);
			}
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return __mlib_VectorMulSAdd_S8C_S8C_Mod((mlib_s8 *)z, (mlib_s8 *)x,
		(mlib_s8 *)y, (mlib_s8 *)c, n);
}

/* *********************************************************** */

#define	MASK_OVER_S8C0                                          \
	mask_r = mlib_mask_16_8[vis_fcmpgt16(dover_r, dr5)];    \
	mask_i = mlib_mask_16_8[vis_fcmpgt16(dover_i, dr6)];    \
	mask_r = (mask_r << 1) | mask_i

/* *********************************************************** */

#define	MASK_OVER_S8C1                                          \
	mask_r = mlib_mask_16_8[vis_fcmpgt16(dover_r, dr5)];    \
	mask_i = mlib_mask_16_8[vis_fcmpgt16(dover_i, dr6)];    \
	mask_r = mask_r | (mask_i >> 1);                        \
	mask_i <<= 7

/* *********************************************************** */

#define	MUL_S_ADD_S8C_SAT_1                                     \
	dr7 = vis_fmul8sux16(dy, dc_r);                         \
	dr8 = vis_fmul8sux16(dy, dc_i);                         \
	dy = vis_fpack32(dy, dy);                               \
	dr2 = vis_fmul8sux16(dy, dc_r);                         \
	dr1 = vis_fmul8sux16(dy, dc_i);                         \
	dx_r = vis_fand(mask_for_r, dx);                        \
	dx_r = vis_bshuffle(dx_r, dx_r);                        \
	dx_r = vis_fxor(restore1, dx_r);                        \
	dx_i = vis_fandnot(mask_for_r, dx);                     \
	dx_i = vis_fxor(restore1, dx_i)

/* *********************************************************** */

#define	MUL_S_ADD_S8C_SAT_2                                     \
	dr3 = vis_fpadd16(dr7, dx_r);                           \
	dr4 = vis_fpadd16(dr8, dx_i);                           \
	dr5 = vis_fpsub16(dr3, dr1);                            \
	dr6 = vis_fpadd16(dr4, dr2);                            \
	dr = vis_fpack16_pair(dr5, dr6);                        \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));     \
	dr = vis_fxnor(dr, restore)

static const mlib_u8 mlib_mask_16_8[] = {
	0, 1, 4, 5, 16, 17, 20, 21, 64, 65, 68, 69, 80, 81, 84, 85
};

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s8 *pzend = (mlib_s8 *)z + n + n - 1;
	mlib_u16 uc_r = ((mlib_u8 *)c)[0] << 8, uc_i = ((mlib_u8 *)c)[1] << 8;
	mlib_d64 *dpx, *dpy, *dpz, *dpzend;
	mlib_d64 dx, dx0, dx1, dr, dr1, dr2, dr3, dr4, dr5, dr6, dr7, dr8, dy,
		dy0, dy1, dx_r, dx_i;
	mlib_d64 restore = vis_to_double_dup(0x7f7f7f7f);
	mlib_d64 restore1 = vis_to_double_dup(0x800080);
	mlib_d64 mask_for_r = vis_to_double_dup(0xff00ff00);
	mlib_d64 dc_r = vis_to_double_dup(uc_r | (uc_r << 16));
	mlib_d64 dc_i = vis_to_double_dup(uc_i | (uc_i << 16));

	if (n <= 0)
		return (MLIB_FAILURE);
/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);

/*
 * prepare the destination address
 */
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z + (1 & (mlib_addr)z);

/*
 * prepare the source address
 */
	dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_faligndata(dx0, dx1);
	dx0 = dx1;

	dpy = (mlib_d64 *)vis_alignaddr((void *)y, off);
	dy0 = vis_ld_d64_nf(dpy);
	dy1 = vis_ld_d64_nf(dpy + 1);
	dy = vis_faligndata(dy0, dy1);
	dy0 = dy1;

/*
 * generate edge mask for the start bytes
 */
	emask = vis_edge8(z, pzend);
	vis_write_bmask(0x789abcde, 0);

	MUL_S_ADD_S8C_SAT_1;
	MUL_S_ADD_S8C_SAT_2;

/*
 * if (|c_r| + |c_i|) > 254   then overflow possibly
 */

	if ((abs(((mlib_s8 *)c)[0]) + abs(((mlib_s8 *)c)[1])) >
		(MLIB_U8_MAX - 1)) {
		mlib_d64 dover_r = vis_to_double_dup(0x80808080), dover_i =
			vis_to_double_dup(0x81008100);
		mlib_s32 mask_r, mask_i;

		if (!(1 & (mlib_addr)z)) {

/* store first bytes of result */
			vis_pst_8(dr, dpz, emask);
			MASK_OVER_S8C0;
			vis_pst_8(restore, dpz, mask_r & emask);
			dpz++;

#pragma pipeloop(0)
			for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {

				vis_alignaddr((void *)x, off);
				dx1 = vis_ld_d64_nf(dpx + 2);
				dx = vis_faligndata(dx0, dx1);
				dx0 = dx1;
				vis_alignaddr((void *)y, off);
				dy1 = vis_ld_d64_nf(dpy + 2);
				dy = vis_faligndata(dy0, dy1);
				dy0 = dy1;

				MUL_S_ADD_S8C_SAT_1;
				MUL_S_ADD_S8C_SAT_2;

/* store 8 bytes of result */
				dpz[0] = dr;
				MASK_OVER_S8C0;
				vis_pst_8(restore, dpz, mask_r);
				dpx++;
				dpy++;
				dpz++;
			}

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {

				vis_alignaddr((void *)x, off);
				dx1 = vis_ld_d64_nf(dpx + 2);
				dx = vis_faligndata(dx0, dx1);
				dx0 = dx1;
				vis_alignaddr((void *)y, off);
				dy1 = vis_ld_d64_nf(dpy + 2);
				dy = vis_faligndata(dy0, dy1);
				dy0 = dy1;

				MUL_S_ADD_S8C_SAT_1;
				MUL_S_ADD_S8C_SAT_2;
/* prepare edge mask for the last bytes */
				emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
				vis_pst_8(dr, dpz, emask);
				MASK_OVER_S8C0;
				vis_pst_8(restore, dpz, mask_r & emask);
			}

		} else {

			MASK_OVER_S8C1;
			dr = vis_bshuffle(dr, dr);
/* store first 7 bytes of result */
			vis_pst_8(dr, dpz, emask);
			vis_pst_8(restore, dpz, mask_r & emask);

			if ((mlib_addr)++dpz <= (mlib_addr)pzend) {
				vis_pst_8(dr, dpz, 0x80);
				vis_pst_8(restore, dpz, mask_i);
			}
#pragma pipeloop(0)
			for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {

				vis_alignaddr((void *)x, off);
				dx1 = vis_ld_d64_nf(dpx + 2);
				dx = vis_faligndata(dx0, dx1);
				dx0 = dx1;
				vis_alignaddr((void *)y, off);
				dy1 = vis_ld_d64_nf(dpy + 2);
				dy = vis_faligndata(dy0, dy1);
				dy0 = dy1;

				MUL_S_ADD_S8C_SAT_1;
				MUL_S_ADD_S8C_SAT_2;
				MASK_OVER_S8C1;
				dr = vis_bshuffle(dr, dr);
/* store 7 bytes of result */
				vis_pst_8(dr, dpz, 0x7f);
				vis_pst_8(restore, dpz, mask_r);
				dpz++;
/* store 1 byte of result to the next word */
				vis_pst_8(dr, dpz, 0x80);
				vis_pst_8(restore, dpz, mask_i);
				dpx++;
				dpy++;
			}

			if ((mlib_addr)dpz < (mlib_addr)pzend) {

				vis_alignaddr((void *)x, off);
				dx1 = vis_ld_d64_nf(dpx + 2);
				dx = vis_faligndata(dx0, dx1);
				dx0 = dx1;
				vis_alignaddr((void *)y, off);
				dy1 = vis_ld_d64_nf(dpy + 2);
				dy = vis_faligndata(dy0, dy1);
				dy0 = dy1;

				MUL_S_ADD_S8C_SAT_1;
				MUL_S_ADD_S8C_SAT_2;
/* prepare edge mask for the last bytes */
				emask = vis_edge8(dpz, pzend);
				MASK_OVER_S8C1;
				dr = vis_bshuffle(dr, dr);
/* store 7 bytes of result */
				vis_pst_8(dr, dpz, emask & 0x7f);
				vis_pst_8(restore, dpz, mask_r & emask);
			}
		}
	} else {

		if (!(1 & (mlib_addr)z)) {

/* store first bytes of result */
			vis_pst_8(dr, dpz, emask);
			dpz++;

			if ((((mlib_addr)x ^ (mlib_addr)z) & 7) == 0) {

				vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
				for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
					dx = dpx[1];
					dy1 = vis_ld_d64_nf(dpy + 2);
					dy = vis_faligndata(dy0, dy1);
					dy0 = dy1;

					MUL_S_ADD_S8C_SAT_1;
					MUL_S_ADD_S8C_SAT_2;
					dpz[0] = dr;
					dpx++;
					dpy++;
					dpz++;
				}

				dx = vis_ld_d64_nf(dpx + 1);
				dy1 = vis_ld_d64_nf(dpy + 2);
				dy = vis_faligndata(dy0, dy1);
			} else {
#pragma pipeloop(0)
				for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
					vis_alignaddr((void *)x, off);
					dx1 = vis_ld_d64_nf(dpx + 2);
					dx = vis_faligndata(dx0, dx1);
					dx0 = dx1;
					vis_alignaddr((void *)y, off);
					dy1 = vis_ld_d64_nf(dpy + 2);
					dy = vis_faligndata(dy0, dy1);
					dy0 = dy1;
					MUL_S_ADD_S8C_SAT_1;
					MUL_S_ADD_S8C_SAT_2;
					dpz[0] = dr;
					dpx++;
					dpy++;
					dpz++;
				}

				vis_alignaddr((void *)x, off);
				dx1 = vis_ld_d64_nf(dpx + 2);
				dx = vis_faligndata(dx0, dx1);
				vis_alignaddr((void *)y, off);
				dy1 = vis_ld_d64_nf(dpy + 2);
				dy = vis_faligndata(dy0, dy1);
			}

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				MUL_S_ADD_S8C_SAT_1;
				MUL_S_ADD_S8C_SAT_2;
/* prepare edge mask for the last bytes */
				emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
				vis_pst_8(dr, dpz, emask);
			}
		} else {

			dr = vis_bshuffle(dr, dr);
/* store first 7 bytes of result */
			vis_pst_8(dr, dpz, emask);

			if ((mlib_addr)++dpz <= (mlib_addr)pzend)
				vis_pst_8(dr, dpz, 0x80);

#pragma pipeloop(0)
			for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
				vis_alignaddr((void *)x, off);
				dx1 = vis_ld_d64_nf(dpx + 2);
				dx = vis_faligndata(dx0, dx1);
				dx0 = dx1;
				vis_alignaddr((void *)y, off);
				dy1 = vis_ld_d64_nf(dpy + 2);
				dy = vis_faligndata(dy0, dy1);
				dy0 = dy1;

				MUL_S_ADD_S8C_SAT_1;
				MUL_S_ADD_S8C_SAT_2;
				dr = vis_bshuffle(dr, dr);
/* store 7 bytes of result */
				vis_pst_8(dr, dpz, 0x7f);
				dpz++;
/* store 1 byte of result to the next word */
				vis_pst_8(dr, dpz, 0x80);
				dpx++;
				dpy++;
			}

			if ((mlib_addr)dpz < (mlib_addr)pzend) {
				vis_alignaddr((void *)x, off);
				dx1 = vis_ld_d64_nf(dpx + 2);
				dx = vis_faligndata(dx0, dx1);
				dx0 = dx1;
				vis_alignaddr((void *)y, off);
				dy1 = vis_ld_d64_nf(dpy + 2);
				dy = vis_faligndata(dy0, dy1);
				dy0 = dy1;

				MUL_S_ADD_S8C_SAT_1;
				MUL_S_ADD_S8C_SAT_2;
/* prepare edge mask for the last bytes */
				emask = vis_edge8(dpz, pzend);
				dr = vis_bshuffle(dr, dr);
/* store 7 bytes of result */
				vis_pst_8(dr, dpz, emask & 0x7f);
			}
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S_ADD_S8C_1                                         \
	dr5 = vis_fmul8sux16(dy, dc_r);                         \
	dr6 = vis_fmul8sux16(dy, dc_i);                         \
	dy = vis_fpack32(dy, dy);                               \
	dr2 = vis_fmul8sux16(dy, dc_r);                         \
	dr1 = vis_fmul8sux16(dy, dc_i);                         \
	dx_r = vis_fand(mask_for_r, dx);                        \
	dx_r = vis_faligndata(dx_r, dx_r);                      \
	dx_i = vis_fandnot(mask_for_r, dx);                     \
	dr3 = vis_fpadd16(dr5, dx_r);                           \
	dr4 = vis_fpadd16(dr6, dx_i)

/* *********************************************************** */

#define	MUL_S_ADD_S8C_1_0                                       \
	dr5 = vis_fmul8sux16(dy, dc_r);                         \
	dr6 = vis_fmul8sux16(dy, dc_i);                         \
	dy = vis_fpack32(dy, dy);                               \
	dr2 = vis_fmul8sux16(dy, dc_r);                         \
	dr1 = vis_fmul8sux16(dy, dc_i);                         \
	dx_r = vis_bshuffle(dx, vis_fzero());                   \
	dx_i = vis_fandnot(mask_for_r, dx);                     \
	dr3 = vis_fpadd16(dr5, dx_r);                           \
	dr4 = vis_fpadd16(dr6, dx_i)

/* *********************************************************** */

#define	MUL_S_ADD_S8C_1_1                                       \
	dr5 = vis_fmul8sux16(dy, dc_r);                         \
	dr6 = vis_fmul8sux16(dy, dc_i);                         \
	dy = vis_fpack32(dy, dy);                               \
	dr2 = vis_fmul8sux16(dy, dc_r);                         \
	dr1 = vis_fmul8sux16(dy, dc_i);                         \
	dx_r = vis_fand(mask_for_r, dx);                        \
	dx_r = vis_bshuffle(dx_r, dx_r);                        \
	dx_i = vis_fandnot(mask_for_r, dx);                     \
	dr3 = vis_fpadd16(dr5, dx_r);                           \
	dr4 = vis_fpadd16(dr6, dx_i)

/* *********************************************************** */

#define	MUL_S_ADD_S8C_2                                         \
	dr5 = vis_fpsub16(dr3, dr1);                            \
	dr6 = vis_fpadd16(dr4, dr2);                            \
	dr5 = vis_fand(dr5, without_control);                   \
	dr6 = vis_fand(dr6, without_control);                   \
	dr5 = vis_fpack16_pair(dr5, dr6);                       \
	dr = vis_fpmerge(vis_read_hi(dr5), vis_read_lo(dr5))

/* *********************************************************** */

#define	MUL_S_ADD_S8C_2_0                                       \
	dr5 = vis_fpsub16(dr3, dr1);                            \
	dr6 = vis_fpadd16(dr4, dr2);                            \
	dr = vis_bshuffle(dr5, dr6)

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S8C_S8C_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s8 *pzend = (mlib_s8 *)z + n + n - 1;
	mlib_u16 uc_r = ((mlib_u8 *)c)[0] << 8, uc_i = ((mlib_u8 *)c)[1] << 8;
	mlib_d64 *dpx, *dpy, *dpz, *dpzend;
	mlib_d64 dx, dx0, dx1, dy, dy0, dy1, dr, dr1, dr2, dr3, dr4, dr5, dr6;
	mlib_d64 dx_r, dx_i;
	mlib_d64 without_control = vis_to_double_dup(0xff00ff);
	mlib_d64 mask_for_r = vis_to_double_dup(0xff00ff00);
	mlib_d64 dc_r = vis_to_double_dup(uc_r | (uc_r << 16));
	mlib_d64 dc_i = vis_to_double_dup(uc_i | (uc_i << 16));

	if (n <= 0)
		return (MLIB_FAILURE);
/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);

/*
 * prepare the destination address
 */
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z + (1 & (mlib_addr)z);

/*
 * prepare the source address
 */
	if (!(1 & (mlib_addr)z)) {
		dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);

		dpy = (mlib_d64 *)vis_alignaddr((void *)y, off);
		dy0 = vis_ld_d64_nf(dpy);
		dy1 = vis_ld_d64_nf(dpy + 1);
		dy = vis_faligndata(dy0, dy1);

/*
 * generate edge mask for the start bytes
 */
		emask = vis_edge8(z, pzend);

		vis_alignaddr(dpx, 7);
		MUL_S_ADD_S8C_1;
		MUL_S_ADD_S8C_2;
/* store first bytes of result */
		vis_pst_8(dr, dpz, emask);
		dpz++;

		if ((((mlib_addr)x ^ (mlib_addr)z) & 7) == 0) {
			if ((((mlib_addr)x ^ (mlib_addr)y) & 7) == 0) {
				vis_alignaddr(dpy, 7);
				vis_write_bmask(0x193b5d7f, 0);
#pragma pipeloop(0)
				for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
					dx = dpx[1];
					dy = dpy[1];
					MUL_S_ADD_S8C_1;
					MUL_S_ADD_S8C_2_0;
					dpz[0] = dr;
					dpx++;
					dpy++;
					dpz++;
				}

				dx = vis_ld_d64_nf(dpx + 1);
				dy = vis_ld_d64_nf(dpy + 1);
				MUL_S_ADD_S8C_1;
			} else {
				dy0 = dy1;
				vis_alignaddr((void *)y, off);
				vis_write_bmask(0x80828486, 0);
#pragma pipeloop(0)
				for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
					dx = dpx[1];
					dy1 = vis_ld_d64_nf(dpy + 2);
					dy = vis_faligndata(dy0, dy1);
					dy0 = dy1;
					MUL_S_ADD_S8C_1_0;
					MUL_S_ADD_S8C_2;
					dpz[0] = dr;
					dpx++;
					dpy++;
					dpz++;
				}

				dx = vis_ld_d64_nf(dpx + 1);
				dy1 = vis_ld_d64_nf(dpy + 2);
				dy = vis_faligndata(dy0, dy1);
				MUL_S_ADD_S8C_1_0;
			}
		} else {
			dx0 = dx1;
			dy0 = dy1;
			vis_write_bmask(0x193b5d7f, 0);
#pragma pipeloop(0)
			for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
				dx1 = vis_ld_d64_nf(dpx + 2);
				vis_alignaddr((void *)x, off);
				dx = vis_faligndata(dx0, dx1);
				dx0 = dx1;
				dy1 = vis_ld_d64_nf(dpy + 2);
				vis_alignaddr((void *)y, off);
				dy = vis_faligndata(dy0, dy1);
				dy0 = dy1;
				vis_alignaddr(dpy, 7);
				MUL_S_ADD_S8C_1;
				MUL_S_ADD_S8C_2_0;
/* store 8 bytes of result */
				dpz[0] = dr;
				dpx++;
				dpy++;
				dpz++;
			}

			dx1 = vis_ld_d64_nf(dpx + 2);
			vis_alignaddr((void *)x, off);
			dx = vis_faligndata(dx0, dx1);
			dy1 = vis_ld_d64_nf(dpy + 2);
			vis_alignaddr((void *)y, off);
			dy = vis_faligndata(dy0, dy1);
			vis_alignaddr(dpy, 7);
			MUL_S_ADD_S8C_1;
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			MUL_S_ADD_S8C_2;
/* prepare edge mask for the last bytes */
			emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
			vis_pst_8(dr, dpz, emask);
		}
	} else {
		dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);

		dpy = (mlib_d64 *)vis_alignaddr((void *)y, off);
		dy0 = vis_ld_d64_nf(dpy);
		dy1 = vis_ld_d64_nf(dpy + 1);
		dy = vis_faligndata(dy0, dy1);

/*
 * generate edge mask for the start bytes
 */
		emask = vis_edge8(z, pzend);

		vis_alignaddr(dpx, 7);
		MUL_S_ADD_S8C_1;
		MUL_S_ADD_S8C_2;

		dr = vis_faligndata(dr, dr);
/* store first 7 bytes of result */
		vis_pst_8(dr, dpz, emask);

		if ((mlib_addr)++dpz <= (mlib_addr)pzend)
			vis_pst_8(dr, dpz, 0x80);

		dx0 = dx1;
		dy0 = dy1;

		vis_write_bmask(0x789abcde, 0);
#pragma pipeloop(0)
		for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
			vis_alignaddr((void *)x, off);
			dx1 = vis_ld_d64_nf(dpx + 2);
			dx = vis_faligndata(dx0, dx1);
			dx0 = dx1;

			vis_alignaddr((void *)y, off);
			dy1 = vis_ld_d64_nf(dpy + 2);
			dy = vis_faligndata(dy0, dy1);
			dy0 = dy1;

			MUL_S_ADD_S8C_1_1;
			MUL_S_ADD_S8C_2;
			dr = vis_bshuffle(dr, dr);
/* store 7 bytes of result */
			vis_pst_8(dr, dpz, 0x7f);
			dpz++;
/* store 1 byte of result to the next word */
			vis_pst_8(dr, dpz, 0x80);
			dpx++;
			dpy++;
		}

		if ((mlib_addr)dpz < (mlib_addr)pzend) {
			vis_alignaddr((void *)x, off);
			dx1 = vis_ld_d64_nf(dpx + 2);
			dx = vis_faligndata(dx0, dx1);

			vis_alignaddr((void *)y, off);
			dy1 = vis_ld_d64_nf(dpy + 2);
			dy = vis_faligndata(dy0, dy1);

			MUL_S_ADD_S8C_1_1;
			MUL_S_ADD_S8C_2;
/* prepare edge mask for the last bytes */
			emask = vis_edge8(dpz, pzend);
			dr = vis_bshuffle(dr, dr);
/* store 7 bytes of result */
			vis_pst_8(dr, dpz, emask & 0x7f);
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S_ADD_S16C_SAT_1                                    \
	dr7 = vis_fmuld8sux16(vis_read_hi(dy), fc_r);           \
	dr8 = vis_fmuld8ulx16(vis_read_hi(dy), fc_r);           \
	dr9 = vis_fmuld8sux16(vis_read_hi(dy), fc_i);           \
	dr10 = vis_fmuld8ulx16(vis_read_hi(dy), fc_i);          \
	dr1 = vis_fpadd32(dr7, dr8);                            \
	dr2 = vis_fpadd32(dr9, dr10);                           \
	dr7 = vis_fmuld8sux16(vis_read_lo(dy), fc_r);           \
	dr8 = vis_fmuld8ulx16(vis_read_lo(dy), fc_r);           \
	dr9 = vis_fmuld8sux16(vis_read_lo(dy), fc_i);           \
	dr10 = vis_fmuld8ulx16(vis_read_lo(dy), fc_i);          \
	dr3 = vis_fpadd32(dr7, dr8);                            \
	dr4 = vis_fpadd32(dr9, dr10);                           \
	dxx = dx

/* *********************************************************** */

#define	MUL_S_ADD_S16C_SAT_2                                              \
	dr7 =                                                             \
	vis_freg_pair(vis_fpsub32s(vis_read_hi(dr1), vis_read_lo(dr1)),   \
		vis_fpadd32s(vis_read_hi(dr2), vis_read_lo(dr2)));        \
	dr8 =                                                             \
	vis_freg_pair(vis_fpsub32s(vis_read_hi(dr3), vis_read_lo(dr3)),   \
		vis_fpadd32s(vis_read_hi(dr4), vis_read_lo(dr4)));        \
	dx_hi = vis_fmuld8ulx16(fone, vis_read_hi(dxx));                  \
	dx_lo = vis_fmuld8ulx16(fone, vis_read_lo(dxx));                  \
	dr5 = vis_fpadd32(dr7, dx_hi);                                    \
	dr6 = vis_fpadd32(dr8, dx_lo);                                    \
	dr7 = vis_fpadd32(dr5, dr5);                                      \
	dr8 = vis_fpadd32(dr6, dr6);                                      \
	dr = vis_freg_pair(vis_fpackfix(dr7), vis_fpackfix(dr8))

/* *********************************************************** */

#define	MUL_S_ADD_S16C_SAT_2_ODD                                          \
	dr7 =                                                             \
	vis_freg_pair(vis_fpadd32s(vis_read_hi(dr4), vis_read_lo(dr4)),   \
		vis_fpsub32s(vis_read_hi(dr1), vis_read_lo(dr1)));        \
	dr8 =                                                             \
	vis_freg_pair(vis_fpadd32s(vis_read_hi(dr2), vis_read_lo(dr2)),   \
		vis_fpsub32s(vis_read_hi(dr3), vis_read_lo(dr3)));        \
	dx_hi = vis_fmuld8ulx16(fone, vis_read_hi(dxx));                  \
	dx_lo = vis_fmuld8ulx16(fone, vis_read_lo(dxx));                  \
	dx1_hi = vis_freg_pair(vis_read_lo(dx_lo), vis_read_hi(dx_hi));   \
	dx1_lo = vis_freg_pair(vis_read_lo(dx_hi), vis_read_hi(dx_lo));   \
	dr5 = vis_fpadd32(dr7, dx1_hi);                                   \
	dr6 = vis_fpadd32(dr8, dx1_lo);                                   \
	dr7 = vis_fpadd32(dr5, dr5);                                      \
	dr8 = vis_fpadd32(dr6, dr6);                                      \
	dr = vis_freg_pair(vis_fpackfix(dr7), vis_fpackfix(dr8))

/* *********************************************************** */

#define	MUL_S_ADD_S16C_SAT_OVER                                  \
	mask_under = vis_fcmplt32(dr5, d_min16);                 \
	mask_under1 = vis_fcmplt32(dr6, d_min16);                \
	mask_over =                                              \
	vis_fcmplt32(dr5, d_min) | vis_fcmpgt32(dr5, d_max16);   \
	mask_over1 =                                             \
	vis_fcmplt32(dr6, d_min) | vis_fcmpgt32(dr6, d_max16);   \
	mask_under = (mask_under << 2) | mask_under1;            \
	mask_over = (mask_over << 2) | mask_over1

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off, boff;
	mlib_s32 mask_under1, mask_over1;
	mlib_s16 *pzend = (mlib_s16 *)z + n + n - 1;
	mlib_u16 uc_r = ((mlib_u16 *)c)[0], uc_i = ((mlib_u16 *)c)[1];
	mlib_d64 *dpx, *dpy, *dpz, *dpzend;
	mlib_d64 dx, dx0, dx1, dr, dr1, dr2, dr3, dr4, dr5, dr6;
	mlib_d64 dy, dy0, dy1, dr7, dr8, dr9, dr10, dx_hi, dx_lo;
	mlib_d64 dx1_hi, dx1_lo, dxx;
	mlib_f32 fc_r = vis_to_float((uc_r << 16) | uc_i);
	mlib_f32 fc_i = vis_to_float((uc_i << 16) | uc_r);
	mlib_f32 fone = vis_to_float(0x10001);

	if (n <= 0)
		return (MLIB_FAILURE);

/* initialize GSR scale factor */
	vis_write_gsr(15 << 3);

/*
 * prepare the destination address
 */
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z + (2 & (mlib_addr)z);

/*
 * prepare the source address
 */
	dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_faligndata(dx0, dx1);
	dx0 = dx1;
	dpy = (mlib_d64 *)vis_alignaddr((void *)y, off);
	dy0 = vis_ld_d64_nf(dpy);
	dy1 = vis_ld_d64_nf(dpy + 1);
	dy = vis_faligndata(dy0, dy1);
	dy0 = dy1;

/*
 * generate edge mask for the start bytes
 */
	emask = vis_edge16(z, pzend);

	if ((abs(((mlib_s16 *)c)[0]) +
	    abs(((mlib_s16 *)c)[1])) > MLIB_S16_MAX) {
/*
 * may be overflow and underflow
 */
		mlib_s32 mask_under, mask_over;
		mlib_d64 d_over = vis_to_double_dup(0x7fff7fff);
		mlib_d64 d_under = vis_fnot(d_over);
		mlib_d64 d_min16 = vis_to_double_dup(MLIB_S16_MIN);
		mlib_d64 d_min = vis_to_double(MLIB_S32_MIN,
			MLIB_S32_MIN + MLIB_S16_MAX + 1);
		mlib_d64 d_max16 = vis_to_double_dup(MLIB_S16_MAX);

		MUL_S_ADD_S16C_SAT_1;
		MUL_S_ADD_S16C_SAT_2;
		MUL_S_ADD_S16C_SAT_OVER;

		if (!(2 & (mlib_addr)z)) {
/* store first bytes of result */
			vis_pst_16(dr, dpz, emask);
			vis_pst_16(d_under, dpz, mask_under & emask);
			vis_pst_16(d_over, dpz, mask_over & emask);
			dpz++;
		} else {
			(void *)vis_alignaddr(dpx, 6);
			dr = vis_faligndata(dr, dr);
/* store first 7 bytes of result */
			vis_pst_16(dr, dpz, emask);
			vis_pst_16(d_under, dpz, (mask_under >> 1) & emask);
			vis_pst_16(d_over, dpz, (mask_over >> 1) & emask);

			if ((mlib_addr)++dpz <= (mlib_addr)pzend) {
				vis_pst_16(dr, dpz, 0x8);
				vis_pst_16(d_under, dpz, mask_under << 3);
				vis_pst_16(d_over, dpz, mask_over << 3);
			}
		}

		boff = ((mlib_addr)((mlib_u8 *)y + off)) & 7;
		vis_alignaddr((void *)x, off);
		vis_write_bmask(boff * 0x11111111, 0x01234567);

		if (!(2 & (mlib_addr)z)) {
#pragma pipeloop(0)
			for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
				dx1 = vis_ld_d64_nf(dpx + 2);
				dx = vis_faligndata(dx0, dx1);
				dx0 = dx1;
				dy1 = vis_ld_d64_nf(dpy + 2);
				dy = vis_bshuffle(dy0, dy1);
				dy0 = dy1;
				MUL_S_ADD_S16C_SAT_1;
				MUL_S_ADD_S16C_SAT_2;
				MUL_S_ADD_S16C_SAT_OVER;
/* store 16 bytes of result */
				dpz[0] = dr;
				vis_pst_16(d_under, dpz, mask_under);
				vis_pst_16(d_over, dpz, mask_over);
				dpx++;
				dpy++;
				dpz++;
			}

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				dx1 = vis_ld_d64_nf(dpx + 2);
				dx = vis_faligndata(dx0, dx1);
				dx0 = dx1;
				dy1 = vis_ld_d64_nf(dpy + 2);
				dy = vis_bshuffle(dy0, dy1);
				dy0 = dy1;
				MUL_S_ADD_S16C_SAT_1;
				MUL_S_ADD_S16C_SAT_2;
				MUL_S_ADD_S16C_SAT_OVER;
/* prepare edge mask for the last bytes */
				emask = vis_edge16(dpz, pzend);
/* store last bytes of result */
				vis_pst_16(dr, dpz, emask);
				vis_pst_16(d_under, dpz, mask_under & emask);
				vis_pst_16(d_over, dpz, mask_over & emask);
			}
		} else {
			d_min = vis_to_double(MLIB_S32_MIN + MLIB_S16_MAX + 1,
				MLIB_S32_MIN);
#pragma pipeloop(0)
			for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
				dx1 = vis_ld_d64_nf(dpx + 2);
				dx = vis_faligndata(dx0, dx1);
				dx0 = dx1;
				dy1 = vis_ld_d64_nf(dpy + 2);
				dy = vis_bshuffle(dy0, dy1);
				dy0 = dy1;
				MUL_S_ADD_S16C_SAT_1;
				MUL_S_ADD_S16C_SAT_2_ODD;
				MUL_S_ADD_S16C_SAT_OVER;
/* store 7 bytes of result */
				vis_pst_16(dr, dpz, 0x7);
				vis_pst_16(d_under, dpz, mask_under & 0x7);
				vis_pst_16(d_over, dpz, mask_over & 0x7);
				dpz++;
/* store 1 byte of result to the next word */
				vis_pst_16(dr, dpz, 0x8);
				vis_pst_16(d_under, dpz, mask_under & 0x8);
				vis_pst_16(d_over, dpz, mask_over & 0x8);
				dpx++;
				dpy++;
			}

			if ((mlib_addr)dpz < (mlib_addr)pzend) {
				dx1 = vis_ld_d64_nf(dpx + 2);
				dx = vis_faligndata(dx0, dx1);
				dx0 = dx1;
				dy1 = vis_ld_d64_nf(dpy + 2);
				dy = vis_bshuffle(dy0, dy1);
				dy0 = dy1;
				MUL_S_ADD_S16C_SAT_1;
				MUL_S_ADD_S16C_SAT_2_ODD;
				MUL_S_ADD_S16C_SAT_OVER;
/* store 4 bytes of result */
				vis_pst_16(dr, dpz, 6);
				vis_pst_16(d_under, dpz, mask_under & 6);
				vis_pst_16(d_over, dpz, mask_over & 6);
			}
		}
		return (MLIB_SUCCESS);
	}

	MUL_S_ADD_S16C_SAT_1;
	MUL_S_ADD_S16C_SAT_2;

	if (!(2 & (mlib_u32)z)) {
/* store first bytes of result */
		vis_pst_16(dr, dpz, emask);
		dpz++;
	} else {
		(void *)vis_alignaddr(dpx, 6);
		dr = vis_faligndata(dr, dr);
/* store first 6 bytes of result */
		vis_pst_16(dr, dpz, emask);

		if ((mlib_addr)++dpz <= (mlib_addr)pzend)
			vis_pst_16(dr, dpz, 0x8);
	}

	boff = ((mlib_addr)((mlib_u8 *)y + off)) & 7;
	vis_alignaddr((void *)x, off);
	vis_write_bmask(boff * 0x11111111, 0x01234567);

	if (!(2 & (mlib_addr)z)) {
		if ((((mlib_addr)x ^ (mlib_addr)z) & 7) == 0) {
#pragma pipeloop(0)
			for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
				dx = dpx[1];
				dy1 = vis_ld_d64_nf(dpy + 2);
				dy = vis_bshuffle(dy0, dy1);
				dy0 = dy1;
				MUL_S_ADD_S16C_SAT_1;
				MUL_S_ADD_S16C_SAT_2;
				dpz[0] = dr;
				dpx++;
				dpy++;
				dpz++;
			}

			dx = vis_ld_d64_nf(dpx + 1);
			dy1 = vis_ld_d64_nf(dpy + 2);
			dy = vis_bshuffle(dy0, dy1);
		} else {
#pragma pipeloop(0)
			for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
				dx1 = vis_ld_d64_nf(dpx + 2);
				dx = vis_faligndata(dx0, dx1);
				dx0 = dx1;
				dy1 = vis_ld_d64_nf(dpy + 2);
				dy = vis_bshuffle(dy0, dy1);
				dy0 = dy1;
				MUL_S_ADD_S16C_SAT_1;
				MUL_S_ADD_S16C_SAT_2;
				dpz[0] = dr;
				dpx++;
				dpy++;
				dpz++;
			}

			dx1 = vis_ld_d64_nf(dpx + 2);
			dx = vis_faligndata(dx0, dx1);
			dy1 = vis_ld_d64_nf(dpy + 2);
			dy = vis_bshuffle(dy0, dy1);
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			MUL_S_ADD_S16C_SAT_1;
			MUL_S_ADD_S16C_SAT_2;
/* prepare edge mask for the last bytes */
			emask = vis_edge16(dpz, pzend);
/* store last bytes of result */
			vis_pst_16(dr, dpz, emask);
		}
	} else {
#pragma pipeloop(0)
		for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
			dx1 = vis_ld_d64_nf(dpx + 2);
			dx = vis_faligndata(dx0, dx1);
			dx0 = dx1;
			dy1 = vis_ld_d64_nf(dpy + 2);
			dy = vis_bshuffle(dy0, dy1);
			dy0 = dy1;

			MUL_S_ADD_S16C_SAT_1;
			MUL_S_ADD_S16C_SAT_2_ODD;
/* store 7 bytes of result */
			vis_pst_16(dr, dpz, 0x7);
			dpz++;
/* store 1 byte of result to the next word */
			vis_pst_16(dr, dpz, 0x8);
			dpx++;
			dpy++;
		}

		if ((mlib_addr)dpz < (mlib_addr)pzend) {
			dx1 = vis_ld_d64_nf(dpx + 2);
			dx = vis_faligndata(dx0, dx1);
			dx0 = dx1;
			dy1 = vis_ld_d64_nf(dpy + 2);
			dy = vis_bshuffle(dy0, dy1);
			dy0 = dy1;

			MUL_S_ADD_S16C_SAT_1;
			MUL_S_ADD_S16C_SAT_2_ODD;
/* store 4 bytes of result */
			vis_pst_16(dr, dpz, 6);
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S_ADD_S16C_MOD_1                                    \
	dr5 = vis_fmuld8sux16(vis_read_hi(dy), fc_r);           \
	dr6 = vis_fmuld8ulx16(vis_read_hi(dy), fc_r);           \
	dr7 = vis_fmuld8sux16(vis_read_hi(dy), fc_i);           \
	dr8 = vis_fmuld8ulx16(vis_read_hi(dy), fc_i);           \
	dr1 = vis_fpadd32(dr5, dr6);                            \
	dr2 = vis_fpadd32(dr7, dr8);                            \
	dr5 = vis_fmuld8sux16(vis_read_lo(dy), fc_r);           \
	dr6 = vis_fmuld8ulx16(vis_read_lo(dy), fc_r);           \
	dr7 = vis_fmuld8sux16(vis_read_lo(dy), fc_i);           \
	dr8 = vis_fmuld8ulx16(vis_read_lo(dy), fc_i);           \
	dr3 = vis_fpadd32(dr5, dr6);                            \
	dr4 = vis_fpadd32(dr7, dr8);                            \
	dxx = dx

/* *********************************************************** */

#define	MUL_S_ADD_S16C_MOD_2                                              \
	dr5 =                                                             \
	vis_freg_pair(vis_fpsub32s(vis_read_hi(dr1), vis_read_lo(dr1)),   \
		vis_fpsub32s(vis_read_hi(dr3), vis_read_lo(dr3)));        \
	dr5 = vis_fpack32(dr5, dzero);                                    \
	dr5 = vis_fpack32(dr5, dzero);                                    \
	dr6 = vis_fand(without_control,                                   \
		vis_freg_pair(vis_fpadd32s(vis_read_hi(dr2),              \
		vis_read_lo(dr2)), vis_fpadd32s(vis_read_hi(dr4),         \
		vis_read_lo(dr4))));                                      \
	dr5 = vis_for(dr5, dr6);                                          \
	dr = vis_fpadd16(dr5, dxx)

/* *********************************************************** */

#define	MUL_S_ADD_S16C_MOD_2_ODD                                          \
	dr5 =                                                             \
	vis_freg_pair(vis_fpadd32s(vis_read_hi(dr4), vis_read_lo(dr4)),   \
		vis_fpadd32s(vis_read_hi(dr2), vis_read_lo(dr2)));        \
	dr6 =                                                             \
	vis_freg_pair(vis_fpsub32s(vis_read_hi(dr1), vis_read_lo(dr1)),   \
		vis_fpsub32s(vis_read_hi(dr3), vis_read_lo(dr3)));        \
	dr5 = vis_bshuffle(dr5, dr6);                                     \
	vis_alignaddr(dpx, 6);                                            \
	dxx = vis_faligndata(dxx, dxx);                                   \
	dr = vis_fpadd16(dr5, dxx)

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16C_S16C_Mod(
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
	mlib_s16 *pzend = (mlib_s16 *)z + n + n - 1;
	mlib_u16 uc_r = ((mlib_u16 *)c)[0], uc_i = ((mlib_u16 *)c)[1];
	mlib_d64 *dpx, *dpy, *dpz, *dpzend;
	mlib_d64 dx, dx0, dx1, dr, dr1, dr2, dr3, dr4, dr5, dr6, dr7, dr8, dy,
		dy0, dy1, dxx;
	mlib_d64 without_control = vis_to_double_dup(0xffff);
	mlib_d64 dzero = vis_fzero();
	mlib_f32 fc_r = vis_to_float((uc_r << 16) | uc_i);
	mlib_f32 fc_i = vis_to_float((uc_i << 16) | uc_r);

	if (n <= 0)
		return (MLIB_FAILURE);

/*
 * prepare the destination address
 */
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z + (2 & (mlib_addr)z);

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
 * generate edge mask for the start bytes
 */
	emask = vis_edge16(z, pzend);
	MUL_S_ADD_S16C_MOD_1;
	MUL_S_ADD_S16C_MOD_2;

	if (!(2 & (mlib_addr)z)) {
/* store first bytes of result */
		vis_pst_16(dr, dpz, emask);
		dpz++;
	} else {
		(void *)vis_alignaddr(dpx, 6);
		dr = vis_faligndata(dr, dr);
/* store first 7 bytes of result */
		vis_pst_16(dr, dpz, emask);

		if ((mlib_addr)++dpz <= (mlib_addr)pzend)
			vis_pst_16(dr, dpz, 0x8);
	}

	if (!(2 & (mlib_addr)z)) {
		if ((((mlib_addr)x ^ (mlib_addr)z) & 7) == 0) {

			dy0 = dy1;
			vis_alignaddr((void *)y, off);
			vis_write_bmask(0x23ab67ef, 0);
#pragma pipeloop(0)
			for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
				dx = dpx[1];
				dy1 = vis_ld_d64_nf(dpy + 2);
				dy = vis_faligndata(dy0, dy1);
				dy0 = dy1;
				MUL_S_ADD_S16C_MOD_1;
				dr5 = vis_freg_pair(vis_fpsub32s(vis_read_hi
					(dr1), vis_read_lo(dr1)),
					vis_fpsub32s(vis_read_hi(dr3),
					vis_read_lo(dr3)));
				dr6 = vis_freg_pair(vis_fpadd32s(vis_read_hi
					(dr2), vis_read_lo(dr2)),
					vis_fpadd32s(vis_read_hi(dr4),
					vis_read_lo(dr4)));
				dr5 = vis_bshuffle(dr5, dr6);
				dr = vis_fpadd16(dr5, dxx);
/* store 16 bytes of result */
				dpz[0] = dr;
				dpx++;
				dpy++;
				dpz++;
			}

			dx = vis_ld_d64_nf(dpx + 1);
			dy1 = vis_ld_d64_nf(dpy + 2);
			dy = vis_faligndata(dy0, dy1);
		} else {

			dx0 = dx1;
			dy0 = dy1;
			vis_alignaddr((void *)x, off);
			vis_write_bmask(((mlib_addr)((mlib_u8 *)y +
				off) & 7) * 0x11111111, 0x01234567);
#pragma pipeloop(0)
			for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
				dx1 = vis_ld_d64_nf(dpx + 2);
				dx = vis_faligndata(dx0, dx1);
				dx0 = dx1;
				dy1 = vis_ld_d64_nf(dpy + 2);
				dy = vis_bshuffle(dy0, dy1);
				dy0 = dy1;
				MUL_S_ADD_S16C_MOD_1;
				MUL_S_ADD_S16C_MOD_2;
/* store 16 bytes of result */
				dpz[0] = dr;
				dpx++;
				dpy++;
				dpz++;
			}

			vis_alignaddr((void *)x, off);
			dx1 = vis_ld_d64_nf(dpx + 2);
			dx = vis_faligndata(dx0, dx1);
			vis_alignaddr((void *)y, off);
			dy1 = vis_ld_d64_nf(dpy + 2);
			dy = vis_faligndata(dy0, dy1);
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			MUL_S_ADD_S16C_MOD_1;
			MUL_S_ADD_S16C_MOD_2;
/* prepare edge mask for the last bytes */
			emask = vis_edge16(dpz, pzend);
/* store last bytes of result */
			vis_pst_16(dr, dpz, emask);
		}
	} else {

		dx0 = dx1;
		dy0 = dy1;
		vis_write_bmask(0x23ab67ef, 0);
#pragma pipeloop(0)
		for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
			vis_alignaddr((void *)x, off);
			dx1 = vis_ld_d64_nf(dpx + 2);
			dx = vis_faligndata(dx0, dx1);
			dx0 = dx1;
			vis_alignaddr((void *)y, off);
			dy1 = vis_ld_d64_nf(dpy + 2);
			dy = vis_faligndata(dy0, dy1);
			dy0 = dy1;
			MUL_S_ADD_S16C_MOD_1;
			MUL_S_ADD_S16C_MOD_2_ODD;
/* store 7 bytes of result */
			vis_pst_16(dr, dpz, 0x7);
			dpz++;
/* store 1 byte of result to the next word */
			vis_pst_16(dr, dpz, 0x8);
			dpx++;
			dpy++;
		}

		if ((mlib_addr)dpz < (mlib_addr)pzend) {
			vis_alignaddr((void *)x, off);
			dx1 = vis_ld_d64_nf(dpx + 2);
			dx = vis_faligndata(dx0, dx1);
			dx0 = dx1;
			vis_alignaddr((void *)y, off);
			dy1 = vis_ld_d64_nf(dpy + 2);
			dy = vis_faligndata(dy0, dy1);
			dy0 = dy1;
			MUL_S_ADD_S16C_MOD_1;
			MUL_S_ADD_S16C_MOD_2_ODD;
/* store 4 bytes of result */
			vis_pst_16(dr, dpz, 6);
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32C_S32C_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	const mlib_s32 *c,
	mlib_s32 n)
{
#define	MAX	0x1fffff
	mlib_s32 i;
	mlib_s32 *px = (mlib_s32 *)x, *py = (mlib_s32 *)y, *pz = (mlib_s32 *)z;
	mlib_d64 val_c_r = ((mlib_s32 *)c)[0];
	mlib_d64 val_c_i = ((mlib_s32 *)c)[1];
	mlib_d64 mul_add_r;
	mlib_d64 mul_add_i;

	if ((mlib_fabs(val_c_r) < MAX) && (mlib_fabs(val_c_i) < MAX))
		for (i = 0; i < n; i++) {
			mul_add_r =
				px[2 * i] + py[2 * i] * val_c_r - py[2 * i +
				1] * val_c_i;
			mul_add_i =
				px[2 * i + 1] + py[2 * i] * val_c_i + py[2 * i +
				1] * val_c_r;
			FLOAT2INT_CLAMP(pz[2 * i], mul_add_r);
			FLOAT2INT_CLAMP(pz[2 * i + 1], mul_add_i);
	} else {
		mlib_d64 val_c_r_lo = ((mlib_s32 *)c)[0] & MAX;
		mlib_d64 val_c_i_lo = ((mlib_s32 *)c)[1] & MAX;

		val_c_r -= val_c_r_lo;
		val_c_i -= val_c_i_lo;
		for (i = 0; i < n; i++) {
			mul_add_r =
				px[2 * i] + (py[2 * i] * val_c_r_lo - py[2 * i +
				1] * val_c_i_lo)
				+ (py[2 * i] * val_c_r - py[2 * i +
				1] * val_c_i);
			mul_add_i =
				px[2 * i + 1] + (py[2 * i] * val_c_i_lo +
				py[2 * i + 1] * val_c_r_lo)
				+ (py[2 * i] * val_c_i + py[2 * i +
				1] * val_c_r);
			FLOAT2INT_CLAMP(pz[2 * i], mul_add_r);
			FLOAT2INT_CLAMP(pz[2 * i + 1], mul_add_i);
		}
	}
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32C_S32C_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	const mlib_s32 *c,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_s32 *px = (mlib_s32 *)x;
	mlib_s32 *py = (mlib_s32 *)y;
	mlib_s32 *pz = (mlib_s32 *)z;
	mlib_s32 val_c_r = ((mlib_s32 *)c)[0];
	mlib_s32 val_c_i = ((mlib_s32 *)c)[1];
	mlib_s32 mul_add_r;
	mlib_s32 mul_add_i;

	for (i = 0; i < n; i++) {
		mul_add_r =
			py[2 * i] * val_c_r - py[2 * i + 1] * val_c_i +
			px[2 * i];
		mul_add_i =
			py[2 * i + 1] * val_c_r + py[2 * i] * val_c_i +
			px[2 * i + 1];
		pz[2 * i] = mul_add_r;
		pz[2 * i + 1] = mul_add_i;
	}
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_U8C_Sat(
	mlib_u8 *xz,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_U8C_U8C_Sat(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_U8C_Mod(
	mlib_u8 *xz,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_U8C_U8C_Mod(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S8C_Sat(
	mlib_s8 *xz,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S8C_S8C_Sat(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S8C_Mod(
	mlib_s8 *xz,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S8C_S8C_Mod(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16C_Sat(
	mlib_s16 *xz,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S16C_S16C_Sat(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16C_Mod(
	mlib_s16 *xz,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S16C_S16C_Mod(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32C_Sat(
	mlib_s32 *xz,
	const mlib_s32 *y,
	const mlib_s32 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S32C_S32C_Sat(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32C_Mod(
	mlib_s32 *xz,
	const mlib_s32 *y,
	const mlib_s32 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S32C_S32C_Mod(xz, xz, y, c, n));
}

/* *********************************************************** */

#define	MUL_S_ADD_S16C_U8C_SAT_LONG1                            \
	dr4 = vis_fmuld8ulx16(vis_read_hi(dy), fc0);            \
	dr6 = vis_fmuld8ulx16(vis_read_hi(dy), fc1);            \
	dr5 = vis_fmuld8ulx16(vis_read_lo(dy), fc0);            \
	dr7 = vis_fmuld8ulx16(vis_read_lo(dy), fc1);            \
	dr = vis_bshuffle(dy, dy);                              \
	dr1 = vis_fmuld8ulx16(vis_read_lo(dr), fc0);            \
	dr3 = vis_fmuld8ulx16(vis_read_lo(dr), fc1);            \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dr), fc1);            \
	dr = vis_fmuld8ulx16(vis_read_hi(dr), fc0);             \
	dx_i = vis_fandnot(mask_for_r, dx);                     \
	dx_r = vis_fand(mask_for_r, dx);                        \
	dx_r = vis_bshuffle(dx_r, dx_r);                        \
	dx_r_hi = vis_fpmerge(fzero, vis_read_hi(dx_r));        \
	dx_r_lo = vis_fpmerge(fzero, vis_read_lo(dx_r));        \
	dx_i_hi = vis_fpmerge(fzero, vis_read_hi(dx_i));        \
	dx_i_lo = vis_fpmerge(fzero, vis_read_lo(dx_i))

/* *********************************************************** */

#define	MUL_S_ADD_S16C_U8C_SAT_LONG2                            \
	dr = vis_fpsub32(dr, dr6);                              \
	dr1 = vis_fpsub32(dr1, dr7);                            \
	dr2 = vis_fpadd32(dr2, dr4);                            \
	dr3 = vis_fpadd32(dr3, dr5);                            \
	dr = vis_fpadd32(dr, dx_r_hi);                          \
	dr1 = vis_fpadd32(dr1, dx_r_lo);                        \
	dr2 = vis_fpadd32(dr2, dx_i_hi);                        \
	dr3 = vis_fpadd32(dr3, dx_i_lo)

/* *********************************************************** */

#define	EVEN_LONG                                                    \
	dzh = vis_fpackfix_pair(vis_freg_pair(vis_read_hi(dr),       \
		vis_read_hi(dr2)), vis_freg_pair(vis_read_lo(dr),    \
		vis_read_lo(dr2)));                                  \
	dzl = vis_fpackfix_pair(vis_freg_pair(vis_read_hi(dr1),      \
		vis_read_hi(dr3)), vis_freg_pair(vis_read_lo(dr1),   \
		vis_read_lo(dr3)))

/* *********************************************************** */

#define	ODD_LONG                                                     \
	dzh = vis_fpackfix_pair(vis_freg_pair(vis_read_lo(rest),     \
		vis_read_hi(dr)), vis_freg_pair(vis_read_hi(dr2),    \
		vis_read_lo(dr)));                                   \
	dzl = vis_fpackfix_pair(vis_freg_pair(vis_read_lo(dr2),      \
		vis_read_hi(dr1)), vis_freg_pair(vis_read_hi(dr3),   \
		vis_read_lo(dr1)));                                  \
	rest = dr3

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16C_U8C_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dx0, dx1, dy0, dy1;
	mlib_d64 dr, dr1, dr2, dr3, dr4, dr5, dr6, dr7, dzh, dzl, dx_r, dx_i;
	mlib_d64 dx_r_hi, dx_r_lo, dx_i_hi, dx_i_lo;
	mlib_d64 mask_for_r = vis_to_double_dup(0xff00ff00);
	mlib_u8 *px, *py;
	mlib_s16 *pz;
	mlib_u16 uc0 = ((mlib_u8 *)c)[0], uc1 = ((mlib_u8 *)c)[1];

/* odd = 1 means "aligned" resultant vector starts with Im. part. */
	mlib_s32 odd = 0;
	mlib_s32 len = n, i;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_f32 fzero = 0.0f;
	mlib_d64 dzero = 0.0, rest;
	mlib_s32 re_x, im_x, re_y, im_y, re_c, im_c;
	mlib_f32 fc0, fc1;

	if ((uc0 + uc1) < MLIB_S8_MAX)
		return (__mlib_VectorMulSAdd_S16C_U8C_Mod(z, x, y, c, n));

	if (n <= 0)
		return (MLIB_FAILURE);

/* initialize GSR for MULS_U8C_S16C */
	vis_write_gsr((16 << 3) | 7);
	px = (mlib_u8 *)x;
	py = (mlib_u8 *)y;
	pz = (mlib_s16 *)z;

	if (n <= 8) {
		for (i = 0; i < n; i++) {
			re_x = (*px++);
			im_x = (*px++);
			re_y = (*py++);
			im_y = (*py++);
			re_c = re_y * uc0 - im_y * uc1 + re_x;
			im_c = im_y * uc0 + re_y * uc1 + im_x;
			(*pz++) =
				re_c < MLIB_S16_MIN ? MLIB_S16_MIN : re_c >
				MLIB_S16_MAX ? MLIB_S16_MAX : re_c;
			im_c = im_c > MLIB_S16_MAX ? MLIB_S16_MAX : im_c;
			(*pz++) = im_c;
		}
		return (MLIB_SUCCESS);
	}

/*
 * prepare the destination address trying to align it for 8 bytes.
 */
	while ((mlib_addr)pz & 7) {
		re_x = (*px++);
		im_x = (*px++);
		re_y = (*py++);
		im_y = (*py++);
		re_c = re_y * uc0 - im_y * uc1 + re_x;
		im_c = im_y * uc0 + re_y * uc1 + im_x;
		re_c = re_c < MLIB_S16_MIN ? MLIB_S16_MIN : re_c >
			MLIB_S16_MAX ? MLIB_S16_MAX : re_c;
		im_c = im_c > MLIB_S16_MAX ? MLIB_S16_MAX : im_c;
		len--;
		(*pz++) = re_c;

		if (!((mlib_addr)pz & 7)) {
			odd = 1;
			rest = vis_to_double(0, im_c);
		} else
			(*pz++) = im_c;
	}

	dpz = (mlib_d64 *)pz;
	len <<= 1;
	even_8 = len >> 3;
	rest_8 = len & 0x7;
	pz += (even_8 << 3);
/*
 * General case: 8*16 --> 32 --> 16 .
 */
/*
 * uc0 <<= 7; uc1 <<= 7;
 */
	fc0 = vis_to_float((uc0 << 16) | uc0);
	fc1 = vis_to_float((uc1 << 16) | uc1);

	dpx = vis_alignaddr(px, 0);
	dpy = vis_alignaddr(py, 0);
	vis_write_bmask(0x789abcde, 0);

	if (!odd) {
		if (!((mlib_addr)px & 7)) {
			dy0 = (*dpy++);
			vis_alignaddr(py, 0);
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = (*dpx++);
				dy1 = vis_ld_d64_nf(dpy); dpy++;
				dy = vis_faligndata(dy0, dy1);
				dy0 = dy1;
				MUL_S_ADD_S16C_U8C_SAT_LONG1;
				MUL_S_ADD_S16C_U8C_SAT_LONG2;
				EVEN_LONG;
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
			}
		} else {
			dx0 = (*dpx++);
			dy0 = (*dpy++);
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				vis_alignaddr(px, 0);
				dx1 = vis_ld_d64_nf(dpx); dpx++;
				dx = vis_faligndata(dx0, dx1);
				dx0 = dx1;
				vis_alignaddr(py, 0);
				dy1 = vis_ld_d64_nf(dpy); dpy++;
				dy = vis_faligndata(dy0, dy1);
				dy0 = dy1;
				MUL_S_ADD_S16C_U8C_SAT_LONG1;
				MUL_S_ADD_S16C_U8C_SAT_LONG2;
				EVEN_LONG;
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
			}
		}
	} else {
/* ODD */
		mlib_d64 dzh_, dzl_;

		dx0 = (*dpx++);
		dy0 = (*dpy++);
		rest = vis_fpackfix_pair(dzero, rest);
#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			vis_alignaddr(px, 0);
			dx1 = vis_ld_d64_nf(dpx); dpx++;
			dx = vis_faligndata(dx0, dx1);
			dx0 = dx1;
			vis_alignaddr(py, 0);
			dy1 = vis_ld_d64_nf(dpy); dpy++;
			dy = vis_faligndata(dy0, dy1);
			dy0 = dy1;
			vis_write_bmask(0x789abcde, 0);
			MUL_S_ADD_S16C_U8C_SAT_LONG1;
			MUL_S_ADD_S16C_U8C_SAT_LONG2;
			vis_write_bmask(0x6789cdab, 0);
			dzh_ = vis_fpackfix_pair(dr, dr2);
			dzl_ = vis_fpackfix_pair(dr1, dr3);
			dzh = vis_bshuffle(rest, dzh_);
			dzl = vis_bshuffle(dzh_, dzl_);
			rest = dzl_;
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
		}

		vis_st_u16(rest, pz);
		pz++;
	}

	if (!rest_8)
		return (MLIB_SUCCESS);
	px += (even_8 << 3);
	py += (even_8 << 3);
	while (rest_8 > 0) {
		re_x = (*px++);
		im_x = (*px++);
		re_y = (*py++);
		im_y = (*py++);
		re_c = re_y * uc0 - im_y * uc1 + re_x;
		im_c = im_y * uc0 + re_y * uc1 + im_x;
		(*pz++) =
			re_c < MLIB_S16_MIN ? MLIB_S16_MIN : re_c >
			MLIB_S16_MAX ? MLIB_S16_MAX : re_c;
		(*pz++) = im_c > MLIB_S16_MAX ? MLIB_S16_MAX : im_c;
		rest_8 -= 2;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S_ADD_S16C_U8C_MOD_SHORT1                            \
	dr3 = vis_fpmerge(vis_read_hi(dy), vis_read_lo(dy));     \
	dr3 = vis_fpmerge(vis_read_hi(dr3), vis_read_lo(dr3));   \
	dr4 = vis_fmul8x16au(vis_read_hi(dr3), fc);              \
	dr5 = vis_fmul8x16al(vis_read_hi(dr3), fc);              \
	dr2 = vis_fmul8x16au(vis_read_lo(dr3), fc);              \
	dr3 = vis_fmul8x16al(vis_read_lo(dr3), fc);              \
	dx_r = vis_bshuffle(dx, vis_fzero());                    \
	dx_i = vis_fandnot(mask_for_r, dx);                      \
	dr = vis_fpsub16(dr4, dr3);                              \
	dr1 = vis_fpadd16(dr5, dr2);                             \
	dr = vis_fpadd16(dr, dx_r);                              \
	dr1 = vis_fpadd16(dr1, dx_i)

/* *********************************************************** */

#define	EVEN_SHORT                                               \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));      \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));      \
	dr1 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));   \
	dr1 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));   \
	dr2 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));    \
	dr3 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr1));    \
	dzh = vis_fpmerge(vis_read_hi(dr2), vis_read_hi(dr3));   \
	dzl = vis_fpmerge(vis_read_lo(dr2), vis_read_lo(dr3))

/* *********************************************************** */

#define	ODD_SHORT                                                \
	(void *)vis_alignaddr(dpx, 6);                           \
	dr2 = vis_faligndata(rest, dr1);                         \
	dr2 = vis_fpmerge(vis_read_hi(dr2), vis_read_lo(dr2));   \
	dr2 = vis_fpmerge(vis_read_hi(dr2), vis_read_lo(dr2));   \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));      \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));      \
	dr3 = vis_fpmerge(vis_read_hi(dr2), vis_read_hi(dr));    \
	dr2 = vis_fpmerge(vis_read_lo(dr2), vis_read_lo(dr));    \
	dzh = vis_fpmerge(vis_read_hi(dr3), vis_read_hi(dr2));   \
	dzl = vis_fpmerge(vis_read_lo(dr3), vis_read_lo(dr2));   \
	rest = dr1

/* *********************************************************** */

#define	MUL_S_ADD_S16C_U8C_MOD1                                 \
	dr4 = vis_fmuld8ulx16(vis_read_hi(dy), fc0);            \
	dr6 = vis_fmuld8ulx16(vis_read_hi(dy), fc1);            \
	dr5 = vis_fmuld8ulx16(vis_read_lo(dy), fc0);            \
	dr7 = vis_fmuld8ulx16(vis_read_lo(dy), fc1);            \
	dr = vis_faligndata(dy, dy);                            \
	dr1 = vis_fmuld8ulx16(vis_read_lo(dr), fc0);            \
	dr3 = vis_fmuld8ulx16(vis_read_lo(dr), fc1);            \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dr), fc1);            \
	dr = vis_fmuld8ulx16(vis_read_hi(dr), fc0);             \
	dx_i = vis_fandnot(mask_for_r, dx);                     \
	dx_r = vis_fand(mask_for_r, dx);                        \
	dx_r = vis_faligndata(dx_r, dx_r);                      \
	dx_r_hi = vis_fpmerge(fzero, vis_read_hi(dx_r));        \
	dx_r_lo = vis_fpmerge(fzero, vis_read_lo(dx_r));        \
	dx_i_hi = vis_fpmerge(fzero, vis_read_hi(dx_i));        \
	dx_i_lo = vis_fpmerge(fzero, vis_read_lo(dx_i))

/* *********************************************************** */

#define	MUL_S_ADD_S16C_U8C_MOD2                                 \
	dr = vis_fpsub32(dr, dr6);                              \
	dr1 = vis_fpsub32(dr1, dr7);                            \
	dr2 = vis_fpadd32(dr2, dr4);                            \
	dr3 = vis_fpadd32(dr3, dr5);                            \
	dr = vis_fpadd32(dr, dx_r_hi);                          \
	dr1 = vis_fpadd32(dr1, dx_r_lo);                        \
	dr2 = vis_fpadd32(dr2, dx_i_hi);                        \
	dr3 = vis_fpadd32(dr3, dx_i_lo)

/* *********************************************************** */

#define	EVEN_S16C_U8C_MOD                                       \
	dzh = vis_bshuffle(dr, dr2);                            \
	dzl = vis_bshuffle(dr1, dr3)

/* *********************************************************** */

#define	ODD_S16C_U8C_MOD                                        \
	dzh = vis_bshuffle(dr, dr2);                            \
	dzl = vis_bshuffle(dr1, dr3);                           \
	vis_alignaddr(dpx, 6);                                  \
	dr = vis_faligndata(rest, dzh);                         \
	rest = dzl;                                             \
	dzl = vis_faligndata(dzh, rest);                        \
	dzh = dr

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16C_U8C_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dx0, dx1, dy0, dy1;
	mlib_d64 dr, dr1, dr2, dr3, dr4, dr5, dr6, dr7, dzh, dzl, dx_r, dx_i;
	mlib_d64 dx_r_hi, dx_r_lo, dx_i_hi, dx_i_lo;
	mlib_d64 mask_for_r = vis_to_double_dup(0xff00ff00);

	mlib_u8 *px, *py;
	mlib_s16 *pz;
	mlib_u16 uc0 = ((mlib_u8 *)c)[0], uc1 = ((mlib_u8 *)c)[1];

/* odd = 1 means "aligned" resultant vector starts with Im. part. */
	mlib_s32 odd = 0;
	mlib_s32 len = n, i;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_f32 fzero = vis_fzeros();
	mlib_d64 dzero = vis_fzero(), rest;
	mlib_s32 re_x, im_x, re_y, im_y, re_c, im_c;
	mlib_f32 fc0, fc1;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_u8 *)x;
	py = (mlib_u8 *)y;
	pz = (mlib_s16 *)z;

	if (n <= 8) {
		for (i = 0; i < n; i++) {
			re_x = (*px++);
			im_x = (*px++);
			re_y = (*py++);
			im_y = (*py++);
			re_c = re_y * uc0 - im_y * uc1 + re_x;
			im_c = im_y * uc0 + re_y * uc1 + im_x;
			(*pz++) = re_c;
			(*pz++) = im_c;
		}
		return (MLIB_SUCCESS);
	}

/*
 * prepare the destination address trying to align it for 8 bytes.
 */
	while ((mlib_addr)pz & 7) {
		re_x = (*px++);
		im_x = (*px++);
		re_y = (*py++);
		im_y = (*py++);
		re_c = re_y * uc0 - im_y * uc1 + re_x;
		im_c = im_y * uc0 + re_y * uc1 + im_x;
		len--;
		(*pz++) = re_c;

		if (!((mlib_addr)pz & 7)) {
			odd = 1;
			rest = vis_to_double(0, im_c);
		} else
			(*pz++) = im_c;
	}

	dpz = (mlib_d64 *)pz;
	len <<= 1;
	even_8 = len >> 3;
	rest_8 = len & 0x7;
	pz += (even_8 << 3);

	if ((uc0 + uc1) <= MLIB_S8_MAX) {

/*
 * For small Im and Re values of scale factor we can use very fast
 * multiplication algorithm: 8*16 = 16 without over- and underflow.
 */

		mlib_f32 fc = vis_to_float((uc0 << 24) | (uc1 << 8));

/*
 * X address is arbitrary aligned.
 * 1 vis_alignaddr and 1 vis_faligndata in the loop.
 */
		dpx = vis_alignaddr(px, 0);
		dpy = vis_alignaddr(py, 0);
		vis_write_bmask(0x80828486, 0);

		if (!odd) {
			if (!((mlib_addr)px & 7)) {
				dy0 = (*dpy++);
				vis_alignaddr(py, 0);
#pragma pipeloop(0)
				for (i = 0; i < even_8; i++) {
					dx = (*dpx++);
					dy1 = vis_ld_d64_nf(dpy); dpy++;
					dy = vis_faligndata(dy0, dy1);
					dy0 = dy1;
					MUL_S_ADD_S16C_U8C_MOD_SHORT1;
					EVEN_SHORT;
					dpz[0] = dzh;
					dpz[1] = dzl;
					dpz += 2;
				}

			} else {
				dx0 = (*dpx++);
				dy0 = (*dpy++);
#pragma pipeloop(0)
				for (i = 0; i < even_8; i++) {
					dx1 = vis_ld_d64_nf(dpx); dpx++;
					vis_alignaddr(px, 0);
					dx = vis_faligndata(dx0, dx1);
					dx0 = dx1;
					dy1 = vis_ld_d64_nf(dpy); dpy++;
					vis_alignaddr(py, 0);
					dy = vis_faligndata(dy0, dy1);
					dy0 = dy1;
					MUL_S_ADD_S16C_U8C_MOD_SHORT1;
					EVEN_SHORT;
					dpz[0] = dzh;
					dpz[1] = dzl;
					dpz += 2;
				}
			}

		} else {
/* ODD */
			dx0 = (*dpx++);
			dy0 = (*dpy++);
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx); dpx++;
				vis_alignaddr(px, 0);
				dx = vis_faligndata(dx0, dx1);
				dx0 = dx1;
				dy1 = vis_ld_d64_nf(dpy); dpy++;
				vis_alignaddr(py, 0);
				dy = vis_faligndata(dy0, dy1);
				dy0 = dy1;
				MUL_S_ADD_S16C_U8C_MOD_SHORT1;
				ODD_SHORT;
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
			}

			vis_st_u16(rest, pz);
			pz++;
		}
	} else {
		fc0 = vis_to_float((uc0 << 16) | uc0);
		fc1 = vis_to_float((uc1 << 16) | uc1);

		dpx = (mlib_d64 *)vis_alignaddr(px, 0);
		dpy = (mlib_d64 *)vis_alignaddr(py, 0);
		vis_write_bmask(0x23ab67ef, 0);

		if (!odd) {
			if (!((mlib_addr)px & 7)) {
				if (!((mlib_addr)py & 7)) {
#pragma pipeloop(0)
					for (i = 0; i < even_8; i++) {
						dx = (*dpx++);
						dy = (*dpy++);
						vis_alignaddr(dpx, 7);
						MUL_S_ADD_S16C_U8C_MOD1;
						MUL_S_ADD_S16C_U8C_MOD2;
						EVEN_S16C_U8C_MOD;
						dpz[0] = dzh;
						dpz[1] = dzl;
						dpz += 2;
					}

				} else {
					dy0 = (*dpy++);
#pragma pipeloop(0)
					for (i = 0; i < even_8; i++) {
						dx = (*dpx++);
						dy1 = vis_ld_d64_nf(dpy); dpy++;
						vis_alignaddr(py, 0);
						dy = vis_faligndata(dy0, dy1);
						dy0 = dy1;
						vis_alignaddr(dpx, 7);
						MUL_S_ADD_S16C_U8C_MOD1;
						MUL_S_ADD_S16C_U8C_MOD2;
						EVEN_S16C_U8C_MOD;
						dpz[0] = dzh;
						dpz[1] = dzl;
						dpz += 2;
					}
				}
			} else {
				dx0 = (*dpx++);
				dy0 = (*dpy++);
#pragma pipeloop(0)
				for (i = 0; i < even_8; i++) {
					dx1 = vis_ld_d64_nf(dpx); dpx++;
					vis_alignaddr(px, 0);
					dx = vis_faligndata(dx0, dx1);
					dx0 = dx1;
					dy1 = vis_ld_d64_nf(dpy); dpy++;
					vis_alignaddr(py, 0);
					dy = vis_faligndata(dy0, dy1);
					dy0 = dy1;
					vis_alignaddr(dpx, 7);
					MUL_S_ADD_S16C_U8C_MOD1;
					MUL_S_ADD_S16C_U8C_MOD2;
					EVEN_S16C_U8C_MOD;
					dpz[0] = dzh;
					dpz[1] = dzl;
					dpz += 2;
				}
			}
		} else {
/* ODD */
			dx0 = (*dpx++);
			dy0 = (*dpy++);
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx); dpx++;
				vis_alignaddr(px, 0);
				dx = vis_faligndata(dx0, dx1);
				dx0 = dx1;
				dy1 = vis_ld_d64_nf(dpy); dpy++;
				vis_alignaddr(py, 0);
				dy = vis_faligndata(dy0, dy1);
				dy0 = dy1;
				vis_alignaddr(dpx, 7);
				MUL_S_ADD_S16C_U8C_MOD1;
				MUL_S_ADD_S16C_U8C_MOD2;
				ODD_S16C_U8C_MOD;
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
			}

			vis_alignaddr(dpx, 6);
			rest = vis_faligndata(rest, dzero);
			vis_pst_16(rest, pz, 0x8);
			pz++;
		}
	}

	if (!rest_8)
		return (MLIB_SUCCESS);
	px += (even_8 << 3);
	py += (even_8 << 3);
	while (rest_8 > 0) {
		re_x = (*px++);
		im_x = (*px++);
		re_y = (*py++);
		im_y = (*py++);
		re_c = re_y * uc0 - im_y * uc1 + re_x;
		im_c = im_y * uc0 + re_y * uc1 + im_x;
		(*pz++) = re_c;
		(*pz++) = im_c;
		rest_8 -= 2;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	S8C_S16C_SAT                                            \
	dr = vis_fmul8sux16(dy, dc0);                           \
	dr1 = vis_fmul8sux16(dy, dc1);                          \
	dy = vis_fpack32(dy, dy);                               \
	dr2 = vis_fmul8sux16(dy, dc0);                          \
	dr3 = vis_fmul8sux16(dy, dc1);                          \
	dr = vis_fpsub16(dr, dr3);                              \
	dr1 = vis_fpadd16(dr2, dr1);                            \
	dx_r = vis_fand(mask_for_r, dx);                        \
	dx_i = vis_bshuffle(dx, vis_fzero());                   \
	dx_r = vis_fmul8x16(fone, dx_r);                        \
	dx_i = vis_fmul8x16(fone, dx_i);                        \
	dr = vis_fpadd16(dr, dx_r);                             \
	dr1 = vis_fpadd16(dr1, dx_i)

/* *********************************************************** */

#define	EVEN_S16C_S8C_SAT                                        \
	dr = vis_fmul8sux16(dy, dc0);                            \
	dr1 = vis_fmul8sux16(dy, dc1);                           \
	dy = vis_fpack32(dy, dy);                                \
	dr2 = vis_fmul8sux16(dy, dc0);                           \
	dr3 = vis_fmul8sux16(dy, dc1);                           \
	dr = vis_fpsub16(dr, dr3);                               \
	dr1 = vis_fpadd16(dr2, dr1);                             \
	dx_r = vis_fand(mask_for_r, dx);                         \
	dx_i = vis_bshuffle(dx, vis_fzero());                    \
	dx_r = vis_fmul8x16(fone, dx_r);                         \
	dx_i = vis_fmul8x16(fone, dx_i);                         \
	dr = vis_fpadd16(dr, dx_r);                              \
	dr1 = vis_fpadd16(dr1, dx_i);                            \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));      \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));      \
	dr1 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));   \
	dr1 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));   \
	dr2 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));    \
	dr3 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr1));    \
	dzh = vis_fpmerge(vis_read_hi(dr2), vis_read_hi(dr3));   \
	dzl = vis_fpmerge(vis_read_lo(dr2), vis_read_lo(dr3));   \
	mask = vis_fcmpgt16(cntr_ovlp_e, dzh);                   \
	mask1 = vis_fcmpgt16(cntr_ovlp_e, dzl);                  \
	dpz[0] = dzh;                                            \
	dpz[1] = dzl;                                            \
	vis_pst_16(const_max_16, dpz, mask);                     \
	vis_pst_16(const_max_16, dpz + 1, mask1);                \
	dpz += 2

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16C_S8C_Sat(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dx0, dx1, dy0, dy1;
	mlib_d64 dr, dr1, dr2, dr3, dzh, dzl;
	mlib_s8 *px, *py;
	mlib_s16 *pz;

/* odd = 1 means "aligned" resultant vector starts with Im. part. */
	mlib_s32 odd = 0;
	mlib_s32 len = n, i;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_f32 fone = vis_to_float(0x1010101);
	mlib_d64 dzero = vis_to_double_dup(0), rest;
	mlib_d64 dx_r, dx_i;
	mlib_d64 mask_for_r = vis_to_double_dup(0xff00ff00);
	mlib_d64 cntr_ovlp_e = vis_to_double_dup(0x80008080);
	mlib_d64 cntr_ovlp_o = vis_to_double_dup(0x80808000);
	mlib_d64 const_max_16 =
		vis_to_double_dup((MLIB_S16_MAX << 16) | MLIB_S16_MAX);
	mlib_s32 mask, mask1;
	mlib_s32 re_x, im_x, re_y, im_y, re_c, im_c;

/*
 * prepare the scaling factor
 */
	mlib_s8 uc0 = ((mlib_s8 *)c)[0], uc1 = ((mlib_s8 *)c)[1];
	mlib_d64 dc0 = vis_to_double_dup((uc0 << 24) | ((uc0 << 8) & 0xffff));
	mlib_d64 dc1 = vis_to_double_dup((uc1 << 24) | ((uc1 << 8) & 0xffff));

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s8 *)x;
	py = (mlib_s8 *)y;
	pz = (mlib_s16 *)z;

	if (!(uc0 == MLIB_S8_MIN && uc1 == MLIB_S8_MIN))
		return (__mlib_VectorMulSAdd_S16C_S8C_Mod(z, x, y, c, n));

	if (n <= 8) {
		for (i = 0; i < n; i++) {
			re_x = (*px++);
			im_x = (*px++);
			re_y = (*py++);
			im_y = (*py++);
			re_c = re_y * uc0 - im_y * uc1 + re_x;
			im_c = im_y * uc0 + re_y * uc1 + im_x;
			(*pz++) =
				re_c < MLIB_S16_MIN ? MLIB_S16_MIN : re_c >
				MLIB_S16_MAX ? MLIB_S16_MAX : re_c;
			(*pz++) =
				im_c < MLIB_S16_MIN ? MLIB_S16_MIN : im_c >
				MLIB_S16_MAX ? MLIB_S16_MAX : im_c;
		}
		return (MLIB_SUCCESS);
	}

/*
 * prepare the destination address trying to align it for 8 bytes.
 */
	while ((mlib_addr)pz & 7) {
		re_x = (*px++);
		im_x = (*px++);
		re_y = (*py++);
		im_y = (*py++);
		re_c = re_y * uc0 - im_y * uc1 + re_x;
		im_c = im_y * uc0 + re_y * uc1 + im_x;
		len--;
		(*pz++) =
			re_c < MLIB_S16_MIN ? MLIB_S16_MIN : re_c >
			MLIB_S16_MAX ? MLIB_S16_MAX : re_c;
		im_c = im_c < MLIB_S16_MIN ? MLIB_S16_MIN : im_c >
			MLIB_S16_MAX ? MLIB_S16_MAX : im_c;

		if (!((mlib_addr)pz & 7)) {
			odd = 1;
			rest = vis_to_double(0, im_c);
		} else
			(*pz++) = im_c;
	}

	dpz = (mlib_d64 *)pz;
	len <<= 1;
	even_8 = len >> 3;
	rest_8 = len & 0x7;
	pz += (even_8 << 3);

	if (!odd) {
		if (!((mlib_addr)px & 7)) {
			dpx = (mlib_d64 *)px;
			dpy = (mlib_d64 *)vis_alignaddr(py, 0);
			dy0 = (*dpy++);
			vis_alignaddr(py, 0);
			vis_write_bmask(0x18385878, 0);
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = (*dpx++);
				dy1 = vis_ld_d64_nf(dpy); dpy++;
				dy = vis_faligndata(dy0, dy1);
				dy0 = dy1;

				EVEN_S16C_S8C_SAT;
			}

		} else {
			dpx = (mlib_d64 *)vis_alignaddr(px, 0);
			dx0 = (*dpx++);
			dpy = (mlib_d64 *)vis_alignaddr(py, 0);
			dy0 = (*dpy++);
			vis_write_bmask(0x18385878, 0);
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				vis_alignaddr(px, 0);
				dx1 = vis_ld_d64_nf(dpx); dpx++;
				dx = vis_faligndata(dx0, dx1);
				dx0 = dx1;
				vis_alignaddr(py, 0);
				dy1 = vis_ld_d64_nf(dpy); dpy++;
				dy = vis_faligndata(dy0, dy1);
				dy0 = dy1;
				EVEN_S16C_S8C_SAT;
			}
		}

	} else {
/* ODD */
		dpx = (mlib_d64 *)vis_alignaddr(px, 0);
		dx0 = (*dpx++);
		dpy = (mlib_d64 *)vis_alignaddr(py, 0);
		dy0 = (*dpy++);
		vis_write_bmask(0x18385878, 0);
#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			vis_alignaddr(px, 0);
			dx1 = vis_ld_d64_nf(dpx); dpx++;
			dx = vis_faligndata(dx0, dx1);
			dx0 = dx1;
			vis_alignaddr(py, 0);
			dy1 = vis_ld_d64_nf(dpy); dpy++;
			dy = vis_faligndata(dy0, dy1);
			dy0 = dy1;

			S8C_S16C_SAT;
			vis_alignaddr(dpx, 6);
			dr2 = vis_faligndata(rest, dr1);
			dr2 = vis_fpmerge(vis_read_hi(dr2), vis_read_lo(dr2));
			dr2 = vis_fpmerge(vis_read_hi(dr2), vis_read_lo(dr2));
			dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));
			dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));
			dr3 = vis_fpmerge(vis_read_hi(dr2), vis_read_hi(dr));
			dr2 = vis_fpmerge(vis_read_lo(dr2), vis_read_lo(dr));
			dzh = vis_fpmerge(vis_read_hi(dr3), vis_read_hi(dr2));
			dzl = vis_fpmerge(vis_read_lo(dr3), vis_read_lo(dr2));
			rest = dr1;
			mask = vis_fcmpgt16(cntr_ovlp_o, dzh);
			mask1 = vis_fcmpgt16(cntr_ovlp_o, dzl);
			dpz[0] = dzh;
			dpz[1] = dzl;
			vis_pst_16(const_max_16, dpz, mask);
			vis_pst_16(const_max_16, dpz + 1, mask1);
			dpz += 2;
		}

		vis_alignaddr(px, 0);
		dx1 = vis_ld_d64_nf(dpx); dpx++;
		dx = vis_faligndata(dx0, dx1);
		dx0 = dx1;
		vis_alignaddr(py, 0);
		dy1 = vis_ld_d64_nf(dpy); dpy++;
		dy = vis_faligndata(dy0, dy1);
		dy0 = dy1;

		S8C_S16C_SAT;
		vis_alignaddr((void *)6, 0);
		rest = vis_faligndata(rest, dzero);
		mask = vis_fcmpgt16(cntr_ovlp_o, rest);
		vis_pst_16(rest, pz, 0x8);
		vis_pst_16(const_max_16, pz, mask);
		pz++;
	}

	if (!rest_8)
		return (MLIB_SUCCESS);
	px += (even_8 << 3);
	py += (even_8 << 3);
	while (rest_8 > 0) {
		re_x = (*px++);
		im_x = (*px++);
		re_y = (*py++);
		im_y = (*py++);
		re_c = re_y * uc0 - im_y * uc1 + re_x;
		im_c = im_y * uc0 + re_y * uc1 + im_x;
		(*pz++) =
			re_c < MLIB_S16_MIN ? MLIB_S16_MIN : re_c >
			MLIB_S16_MAX ? MLIB_S16_MAX : re_c;
		(*pz++) =
			im_c < MLIB_S16_MIN ? MLIB_S16_MIN : im_c >
			MLIB_S16_MAX ? MLIB_S16_MAX : im_c;
		rest_8 -= 2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	S8C_S16C_MOD                                            \
	dr = vis_fmul8sux16(dy, dc0);                           \
	dr1 = vis_fmul8sux16(dy, dc1);                          \
	dy = vis_fpack32(dy, dy);                               \
	dr2 = vis_fmul8sux16(dy, dc0);                          \
	dr3 = vis_fmul8sux16(dy, dc1);                          \
	dr = vis_fpsub16(dr, dr3);                              \
	dr1 = vis_fpadd16(dr2, dr1);                            \
	dx_r = vis_fand(mask_for_r, dx);                        \
	dx_i = vis_bshuffle(dx, vis_fzero());                   \
	dx_r = vis_fmul8x16(fone, dx_r);                        \
	dx_i = vis_fmul8x16(fone, dx_i);                        \
	dr = vis_fpadd16(dr, dx_r);                             \
	dr1 = vis_fpadd16(dr1, dx_i)

/* *********************************************************** */

#define	EVEN_S16C_S8C_MOD                                        \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));      \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));      \
	dr1 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));   \
	dr1 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));   \
	dr2 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));    \
	dr3 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr1));    \
	dzh = vis_fpmerge(vis_read_hi(dr2), vis_read_hi(dr3));   \
	dzl = vis_fpmerge(vis_read_lo(dr2), vis_read_lo(dr3))

/* *********************************************************** */

#define	ODD_S16C_S8C_MOD                                         \
	(void *)vis_alignaddr(dpx, 6);                           \
	dr2 = vis_faligndata(rest, dr1);                         \
	dr2 = vis_fpmerge(vis_read_hi(dr2), vis_read_lo(dr2));   \
	dr2 = vis_fpmerge(vis_read_hi(dr2), vis_read_lo(dr2));   \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));      \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));      \
	dr3 = vis_fpmerge(vis_read_hi(dr2), vis_read_hi(dr));    \
	dr2 = vis_fpmerge(vis_read_lo(dr2), vis_read_lo(dr));    \
	dzh = vis_fpmerge(vis_read_hi(dr3), vis_read_hi(dr2));   \
	dzl = vis_fpmerge(vis_read_lo(dr3), vis_read_lo(dr2));   \
	rest = dr1

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16C_S8C_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dx0, dx1, dy0, dy1;
	mlib_d64 dr, dr1, dr2, dr3, dzh, dzl;
	mlib_s8 *px, *py;
	mlib_s16 *pz;

/* odd = 1 means "aligned" resultant vector starts with Im. part. */
	mlib_s32 odd = 0;
	mlib_s32 len = n, i;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_f32 fone = vis_to_float(0x1010101);
	mlib_d64 dzero = vis_to_double_dup(0), rest;
	mlib_d64 dx_r, dx_i;
	mlib_d64 mask_for_r = vis_to_double_dup(0xff00ff00);
	mlib_s32 re_x, im_x, re_y, im_y, re_c, im_c;

/*
 * prepare the scaling factor
 */
	mlib_s8 uc0 = ((mlib_s8 *)c)[0], uc1 = ((mlib_s8 *)c)[1];
	mlib_d64 dc0 = vis_to_double_dup((uc0 << 24) | ((uc0 << 8) & 0xffff));
	mlib_d64 dc1 = vis_to_double_dup((uc1 << 24) | ((uc1 << 8) & 0xffff));

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s8 *)x;
	py = (mlib_s8 *)y;
	pz = (mlib_s16 *)z;

	if (n <= 8) {
		for (i = 0; i < n; i++) {
			re_x = (*px++);
			im_x = (*px++);
			re_y = (*py++);
			im_y = (*py++);
			re_c = re_y * uc0 - im_y * uc1 + re_x;
			im_c = im_y * uc0 + re_y * uc1 + im_x;
			(*pz++) = re_c;
			(*pz++) = im_c;
		}

		return (MLIB_SUCCESS);
	}

/*
 * prepare the destination address trying to align it for 8 bytes.
 */
	while ((mlib_addr)pz & 7) {
		re_x = (*px++);
		im_x = (*px++);
		re_y = (*py++);
		im_y = (*py++);
		re_c = re_y * uc0 - im_y * uc1 + re_x;
		im_c = im_y * uc0 + re_y * uc1 + im_x;
		len--;
		(*pz++) = re_c;

		if (!((mlib_addr)pz & 7)) {
			odd = 1;
			rest = vis_to_double(0, im_c);
		} else
			(*pz++) = im_c;
	}

	dpz = (mlib_d64 *)pz;
	len <<= 1;
	even_8 = len >> 3;
	rest_8 = len & 0x7;
	pz += (even_8 << 3);

	dpx = (mlib_d64 *)vis_alignaddr(px, 0);
	dpy = (mlib_d64 *)vis_alignaddr(py, 0);
	vis_write_bmask(0x18385878, 0);

	if (!odd) {
		if (!((mlib_addr)px & 7)) {
			dy0 = (*dpy++);
			vis_alignaddr(py, 0);
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = (*dpx++);
				dy1 = vis_ld_d64_nf(dpy); dpy++;
				dy = vis_faligndata(dy0, dy1);
				dy0 = dy1;
				S8C_S16C_MOD;
				EVEN_S16C_S8C_MOD;
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
			}

		} else {
			dx0 = (*dpx++);
			dy0 = (*dpy++);
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx); dpx++;
				vis_alignaddr(px, 0);
				dx = vis_faligndata(dx0, dx1);
				dx0 = dx1;
				dy1 = vis_ld_d64_nf(dpy); dpy++;
				vis_alignaddr(py, 0);
				dy = vis_faligndata(dy0, dy1);
				dy0 = dy1;
				S8C_S16C_MOD;
				EVEN_S16C_S8C_MOD;
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
			}
		}

	} else {
/* ODD */
		dx0 = (*dpx++);
		dy0 = (*dpy++);
#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx); dpx++;
			vis_alignaddr(px, 0);
			dx = vis_faligndata(dx0, dx1);
			dx0 = dx1;
			dy1 = vis_ld_d64_nf(dpy); dpy++;
			vis_alignaddr(py, 0);
			dy = vis_faligndata(dy0, dy1);
			dy0 = dy1;
			S8C_S16C_MOD;
			ODD_S16C_S8C_MOD;
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
		}

		vis_alignaddr((void *)6, 0);
		rest = vis_faligndata(rest, dzero);
		vis_pst_16(rest, pz, 0x8);
		pz++;
	}

	if (!rest_8)
		return (MLIB_SUCCESS);
	px += (even_8 << 3);
	py += (even_8 << 3);
	while (rest_8 > 0) {
		re_x = (*px++);
		im_x = (*px++);
		re_y = (*py++);
		im_y = (*py++);
		re_c = re_y * uc0 - im_y * uc1 + re_x;
		im_c = im_y * uc0 + re_y * uc1 + im_x;
		(*pz++) = re_c;
		(*pz++) = im_c;
		rest_8 -= 2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S_ADD_S32C_S16C_SAT                                   \
	dr = vis_fmuld8sux16(vis_read_hi(dy), fc0);               \
	dr1 = vis_fmuld8ulx16(vis_read_hi(dy), fc0);              \
	dr = vis_fpadd32(dr, dr1);                                \
	dr2 = vis_fmuld8sux16(vis_read_hi(dy), fc1);              \
	dr3 = vis_fmuld8ulx16(vis_read_hi(dy), fc1);              \
	dr1 = vis_fpadd32(dr2, dr3);                              \
	sr0 = vis_fpsub32s(vis_read_hi(dr), vis_read_lo(dr));     \
	sr1 = vis_fpadd32s(vis_read_hi(dr1), vis_read_lo(dr1));   \
	dr4 = vis_fmuld8sux16(vis_read_lo(dy), fc0);              \
	dr5 = vis_fmuld8ulx16(vis_read_lo(dy), fc0);              \
	dr4 = vis_fpadd32(dr4, dr5);                              \
	dr2 = vis_fmuld8sux16(vis_read_lo(dy), fc1);              \
	dr3 = vis_fmuld8ulx16(vis_read_lo(dy), fc1);              \
	dr5 = vis_fpadd32(dr2, dr3);                              \
	sr2 = vis_fpsub32s(vis_read_hi(dr4), vis_read_lo(dr4));   \
	sr3 = vis_fpadd32s(vis_read_hi(dr5), vis_read_lo(dr5));   \
	dxx = dx

/* *********************************************************** */

#define	EVEN_S32C_S16C_SAT                                      \
	dx_hi = vis_fmuld8ulx16(fone, vis_read_hi(dxx));        \
	dx_lo = vis_fmuld8ulx16(fone, vis_read_lo(dxx));        \
	dr = vis_freg_pair(sr0, sr1);                           \
	dr1 = vis_freg_pair(sr2, sr3);                          \
	dzh = vis_fpadd32(dr, dx_hi);                           \
	dzl = vis_fpadd32(dr1, dx_lo);                          \
	mask = vis_fcmpgt32(cntr_ovlp, dzh);                    \
	mask1 = vis_fcmpgt32(cntr_ovlp, dzl)

/* *********************************************************** */

#define	ODD_S32C_S16C_SAT                                       \
	dx_hi = vis_fmuld8ulx16(fone, vis_read_hi(dxx));        \
	dx_lo = vis_fmuld8ulx16(fone, vis_read_lo(dxx));        \
	dr = vis_freg_pair(rest, sr0);                          \
	dr1 = vis_freg_pair(sr1, sr2);                          \
	dzh = vis_fpadd32(dr, dx_hi);                           \
	dzl = vis_fpadd32(dr1, dx_lo);                          \
	rest = sr3;                                             \
	mask = vis_fcmpgt32(cntr_ovlp1, dzh);                   \
	mask1 = vis_fcmpgt32(cntr_ovlp1, dzl)

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32C_S16C_Sat(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dx0, dx1, dy0, dy1;
	mlib_d64 dr, dr1, dr2, dr3, dr4, dr5, dzh, dzl;
	mlib_d64 dxx, dx_hi, dx_lo;
	mlib_s16 *px, *py;
	mlib_s32 *pz;

/* odd = 1 means 8-byte aligned resultant vector starts with Im. part. */
	mlib_s32 odd;
	mlib_s32 len = n << 1, i;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_f32 fzero = vis_fzeros();
	mlib_f32 sr0, sr1, sr2, sr3;
	mlib_f32 rest;

/*
 * prepare the scaling factor
 */
	mlib_d64 uc00 = ((mlib_s16 *)c)[0], uc01 = ((mlib_s16 *)c)[1];
	mlib_u16 uc0 = ((mlib_u16 *)c)[0], uc1 = ((mlib_u16 *)c)[1];
	mlib_f32 fc0 = vis_to_float((uc0 << 16) | uc1);
	mlib_f32 fc1 = vis_to_float((uc1 << 16) | uc0);
	mlib_f32 fone = vis_to_float(0x10001);
	mlib_d64 drest;
	mlib_d64 cntr_ovlp = vis_to_double(0x80000000, 0x80008000);
	mlib_d64 cntr_ovlp1 = vis_to_double(0x80008000, 0x80000000);
	mlib_d64 const_max_32 = vis_to_double_dup(MLIB_S32_MAX);
	mlib_s32 mask, mask1;
	mlib_d64 re_x, im_x, re_y, im_y, im_c;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!(((mlib_s16 *)c)[0] == MLIB_S16_MIN &&
		((mlib_s16 *)c)[1] == MLIB_S16_MIN))

		return (__mlib_VectorMulSAdd_S32C_S16C_Mod(z, x, y, c, n));

	px = (mlib_s16 *)x;
	py = (mlib_s16 *)y;
	pz = (mlib_s32 *)z;

	if (n <= 4) {
		while (n--) {
			re_x = (*px++);
			im_x = (*px++);
			re_y = (*py++);
			im_y = (*py++);
			FLOAT2INT_CLAMP((*pz++),
				re_y * uc00 - im_y * uc01 + re_x);
			FLOAT2INT_CLAMP((*pz++),
				im_y * uc00 + re_y * uc01 + im_x);
		}

		return (MLIB_SUCCESS);
	}

/*
 * prepare the destination address trying to align it for 8 bytes.
 */

	if (!((mlib_addr)pz & 7)) {
		odd = 0;
	} else {
		re_x = (*px++);
		re_y = (*py++);
		im_y = (*py++);
		FLOAT2INT_CLAMP((*pz++), re_y * uc00 - im_y * uc01 + re_x);
		im_c = im_y * uc00 + re_y * uc01 - 1.0;
		rest = vis_to_float(1 + (mlib_s32)im_c);
		odd = 1;
		len -= 2;
	}

	dpz = (mlib_d64 *)pz;
	even_8 = len >> 2;
	rest_8 = len & 0x3;
	pz += (even_8 << 2);

	if (!odd) {
		if (!((mlib_addr)px & 7)) {
			dpx = vis_alignaddr(px, 0);
			dpy = vis_alignaddr(py, 0);
			dy0 = (*dpy++);
			vis_alignaddr(py, 0);
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = (*dpx++);
				dy1 = vis_ld_d64_nf(dpy); dpy++;
				dy = vis_faligndata(dy0, dy1);
				dy0 = dy1;
				MUL_S_ADD_S32C_S16C_SAT;
				EVEN_S32C_S16C_SAT;
				dpz[0] = dzh;
				dpz[1] = dzl;
				vis_pst_32(const_max_32, dpz, mask);
				vis_pst_32(const_max_32, dpz + 1, mask1);
				dpz += 2;
			}

		} else {
			mlib_s32 off = (mlib_addr)py & 7;

			dpx = vis_alignaddr(px, 0);
			dx0 = (*dpx++);
			dpy = (mlib_d64 *)((mlib_u8 *)py - off);
			dy0 = (*dpy++);
			vis_write_bmask(off * 0x11111111, 0x01234567);
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx); dpx++;
				dx = vis_faligndata(dx0, dx1);
				dx0 = dx1;
				dy1 = vis_ld_d64_nf(dpy); dpy++;
				dy = vis_bshuffle(dy0, dy1);
				dy0 = dy1;
				MUL_S_ADD_S32C_S16C_SAT;
				EVEN_S32C_S16C_SAT;
				dpz[0] = dzh;
				dpz[1] = dzl;
				vis_pst_32(const_max_32, dpz, mask);
				vis_pst_32(const_max_32, dpz + 1, mask1);
				dpz += 2;
			}
		}

	} else {
/* ODD */
		mlib_s32 off = (mlib_addr)py & 7;

		dpx = vis_alignaddr(px, 0);
		dx0 = (*dpx++);
		dpy = (mlib_d64 *)((mlib_u8 *)py - off);
		dy0 = (*dpy++);
		vis_write_bmask(off * 0x11111111, 0x01234567);
#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx); dpx++;
			dx = vis_faligndata(dx0, dx1);
			dx0 = dx1;
			dy1 = vis_ld_d64_nf(dpy); dpy++;
			dy = vis_bshuffle(dy0, dy1);
			dy0 = dy1;
			MUL_S_ADD_S32C_S16C_SAT;
			ODD_S32C_S16C_SAT;
			dpz[0] = dzh;
			dpz[1] = dzl;
			vis_pst_32(const_max_32, dpz, mask);
			vis_pst_32(const_max_32, dpz + 1, mask1);
			dpz += 2;
		}

/*
 * Storing of the last imag. part which was in 'rest'.
 */
		dx1 = vis_ld_d64_nf(dpx); dpx++;
		dx = vis_faligndata(dx0, dx1);
		dx0 = dx1;
		dy1 = vis_ld_d64_nf(dpy); dpy++;
		dy = vis_bshuffle(dy0, dy1);
		dy0 = dy1;
		MUL_S_ADD_S32C_S16C_SAT;
		drest = vis_freg_pair(rest, fzero);
		dx_hi = vis_fmuld8ulx16(fone, vis_read_hi(dxx));
		drest = vis_fpadd32(drest, dx_hi);
		mask = vis_fcmpgt32(cntr_ovlp1, drest);
		vis_pst_32(drest, pz, 0x2);
		vis_pst_32(const_max_32, pz, mask & 0x2);
		pz++;
	}

	if (!rest_8)
		return (MLIB_SUCCESS);

	px += (even_8 << 2) + odd;
	py += (even_8 << 2);

	re_x = (*px++);
	im_x = *px;
	re_y = (*py++);
	im_y = *py;
	FLOAT2INT_CLAMP((*pz++), re_y * uc00 - im_y * uc01 + re_x);
	FLOAT2INT_CLAMP(*pz, im_y * uc00 + re_y * uc01 + im_x);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S_ADD_S32C_S16C_MOD                                   \
	dr = vis_fmuld8sux16(vis_read_hi(dy), fc0);               \
	dr1 = vis_fmuld8ulx16(vis_read_hi(dy), fc0);              \
	dr = vis_fpadd32(dr, dr1);                                \
	dr2 = vis_fmuld8sux16(vis_read_hi(dy), fc1);              \
	dr3 = vis_fmuld8ulx16(vis_read_hi(dy), fc1);              \
	dr1 = vis_fpadd32(dr2, dr3);                              \
	sr0 = vis_fpsub32s(vis_read_hi(dr), vis_read_lo(dr));     \
	sr1 = vis_fpadd32s(vis_read_hi(dr1), vis_read_lo(dr1));   \
	dr4 = vis_fmuld8sux16(vis_read_lo(dy), fc0);              \
	dr5 = vis_fmuld8ulx16(vis_read_lo(dy), fc0);              \
	dr4 = vis_fpadd32(dr4, dr5);                              \
	dr2 = vis_fmuld8sux16(vis_read_lo(dy), fc1);              \
	dr3 = vis_fmuld8ulx16(vis_read_lo(dy), fc1);              \
	dr5 = vis_fpadd32(dr2, dr3);                              \
	sr2 = vis_fpsub32s(vis_read_hi(dr4), vis_read_lo(dr4));   \
	sr3 = vis_fpadd32s(vis_read_hi(dr5), vis_read_lo(dr5));   \
	dxx = dx

/* *********************************************************** */

#define	EVEN_S32C_S16C_MOD                                      \
	dx_hi = vis_fmuld8ulx16(fone, vis_read_hi(dxx));        \
	dx_lo = vis_fmuld8ulx16(fone, vis_read_lo(dxx));        \
	dr = vis_freg_pair(sr0, sr1);                           \
	dr1 = vis_freg_pair(sr2, sr3);                          \
	dzh = vis_fpadd32(dr, dx_hi);                           \
	dzl = vis_fpadd32(dr1, dx_lo)

/* *********************************************************** */

#define	ODD_S32C_S16C_MOD                                       \
	dx_hi = vis_fmuld8ulx16(fone, vis_read_hi(dxx));        \
	dx_lo = vis_fmuld8ulx16(fone, vis_read_lo(dxx));        \
	dr = vis_freg_pair(rest, sr0);                          \
	dr1 = vis_freg_pair(sr1, sr2);                          \
	dzh = vis_fpadd32(dr, dx_hi);                           \
	dzl = vis_fpadd32(dr1, dx_lo);                          \
	rest = sr3

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dx0, dx1, dy0, dy1;
	mlib_d64 dr, dr1, dr2, dr3, dr4, dr5, dzh, dzl;
	mlib_d64 dxx, dx_hi, dx_lo;
	mlib_s16 *px, *py;
	mlib_s32 *pz;

/* odd = 1 means 8-byte aligned resultant vector starts with Im. part. */
	mlib_s32 odd;
	mlib_s32 len = n << 1, i;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_f32 fzero = vis_fzeros();
	mlib_f32 sr0, sr1, sr2, sr3;
	mlib_f32 rest;

/*
 * prepare the scaling factor
 */
	mlib_s16 uc00 = ((mlib_s16 *)c)[0], uc01 = ((mlib_s16 *)c)[1];
	mlib_u16 uc0 = ((mlib_u16 *)c)[0], uc1 = ((mlib_u16 *)c)[1];
	mlib_f32 fc0 = vis_to_float((uc0 << 16) | uc1);
	mlib_f32 fc1 = vis_to_float((uc1 << 16) | uc0);
	mlib_f32 fone = vis_to_float(0x10001);
	mlib_d64 drest;
	mlib_s32 re_x, im_x, re_y, im_y, im_c;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s16 *)x;
	py = (mlib_s16 *)y;
	pz = (mlib_s32 *)z;

	if (n <= 4) {
		while (n--) {
			re_x = (*px++);
			im_x = (*px++);
			re_y = (*py++);
			im_y = (*py++);
			(*pz++) = re_y * uc00 - im_y * uc01 + re_x;
			(*pz++) = im_y * uc00 + re_y * uc01 + im_x;
		}

		return (MLIB_SUCCESS);
	}

/*
 * prepare the destination address trying to align it for 8 bytes.
 */

	if (!((mlib_addr)pz & 7)) {
		odd = 0;
	} else {
		re_x = (*px++);
		re_y = (*py++);
		im_y = (*py++);
		(*pz++) = re_y * uc00 - im_y * uc01 + re_x;
		im_c = im_y * uc00 + re_y * uc01;
		rest = vis_to_float(im_c);
		odd = 1;
		len -= 2;
	}

	dpz = (mlib_d64 *)pz;
	even_8 = len >> 2;
	rest_8 = len & 0x3;
	pz += (even_8 << 2);

	dpx = vis_alignaddr(px, 0);
	dpy = vis_alignaddr(py, 0);

	if (!odd) {
		if (!((mlib_addr)px & 7)) {
			dy0 = (*dpy++);
			vis_alignaddr(py, 0);
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = (*dpx++);
				dy1 = vis_ld_d64_nf(dpy); dpy++;
				dy = vis_faligndata(dy0, dy1);
				dy0 = dy1;
				MUL_S_ADD_S32C_S16C_MOD;
				EVEN_S32C_S16C_MOD;
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
			}

		} else {
			mlib_s32 boff;

			boff = (mlib_addr)px & 7;
			vis_alignaddr(py, 0);
			vis_write_bmask(boff * 0x11111111, 0x01234567);
			dx0 = (*dpx++);
			dy0 = (*dpy++);
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx); dpx++;
				dx = vis_bshuffle(dx0, dx1);
				dx0 = dx1;
				dy1 = vis_ld_d64_nf(dpy); dpy++;
				dy = vis_faligndata(dy0, dy1);
				dy0 = dy1;
				MUL_S_ADD_S32C_S16C_MOD;
				EVEN_S32C_S16C_MOD;
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
			}
		}

	} else {
/* ODD */
		mlib_s32 boff;

		boff = (mlib_addr)px & 7;
		vis_alignaddr(py, 0);
		vis_write_bmask(boff * 0x11111111, 0x01234567);
		dx0 = (*dpx++);
		dy0 = (*dpy++);
#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx); dpx++;
			dx = vis_bshuffle(dx0, dx1);
			dx0 = dx1;
			dy1 = vis_ld_d64_nf(dpy); dpy++;
			dy = vis_faligndata(dy0, dy1);
			dy0 = dy1;
			MUL_S_ADD_S32C_S16C_MOD;
			ODD_S32C_S16C_MOD;
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
		}

/*
 * Storing of the last imag. part which was in 'rest'.
 */
		dx1 = vis_ld_d64_nf(dpx); dpx++;
		dx = vis_bshuffle(dx0, dx1);
		dy1 = vis_ld_d64_nf(dpy); dpy++;
		dy = vis_faligndata(dy0, dy1);
		MUL_S_ADD_S32C_S16C_MOD;
		drest = vis_freg_pair(rest, fzero);
		dx_hi = vis_fmuld8ulx16(fone, vis_read_hi(dxx));
		drest = vis_fpadd32(drest, dx_hi);
		vis_pst_32(drest, pz, 0x2);
		pz++;
	}

	if (!rest_8)
		return (MLIB_SUCCESS);

	px += (even_8 << 2) + odd;
	py += (even_8 << 2);

	re_x = (*px++);
	im_x = *px;
	re_y = (*py++);
	im_y = *py;
	(*pz++) = re_y * uc00 - im_y * uc01 + re_x;
	*pz = im_y * uc00 + re_y * uc01 + im_x;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
