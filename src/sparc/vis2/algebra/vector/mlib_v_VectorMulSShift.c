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

#pragma ident	"@(#)mlib_v_VectorMulSShift.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorMulSShift_U8_[Sat|Mod],
 *      mlib_VectorMulSShift_U8_U8_[Sat|Mod]   - multiplication of
 *                                               unsigned 8-bit format vector
 *                                               by a scalar with shifting
 *      mlib_VectorMulSShift_S8_[Sat|Mod],
 *      mlib_VectorMulSShift_S8_S8_[Sat|Mod]   - multiplication of
 *                                               signed 8-bit format vector
 *                                               by a scalar with shifting
 *      mlib_VectorMulSShift_S16_[Sat|Mod],
 *      mlib_VectorMulSShift_S16_S16_[Sat|Mod] - multiplication of
 *                                               signed 16-bit format vector
 *                                               by a scalar with shifting
 *      mlib_VectorMulSShift_S32_[Sat|Mod],
 *      mlib_VectorMulSShift_S32_S32_[Sat|Mod] - multiplication of
 *                                               signed 32-bit format vector
 *                                               by a scalar with shifting
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMulSShift_U8_U8_[Sat|Mod](mlib_u8       *z,
 *                                                       const mlib_u8 *x,
 *                                                       const mlib_u8 *c,
 *                                                       mlib_s32       n,
 *                                                       mlib_s32   shift);
 *      mlib_status mlib_VectorMulSShift_S8_S8_[Sat|Mod](mlib_s8       *z,
 *                                                       const mlib_s8 *x,
 *                                                       const mlib_s8 *c,
 *                                                       mlib_s32       n,
 *                                                       mlib_s32   shift);
 *      mlib_status mlib_VectorMulSShift_S16_S16_[Sat|Mod](mlib_s16       *z,
 *                                                         const mlib_s16 *x,
 *                                                         const mlib_s16 *c,
 *                                                         mlib_s32       n,
 *                                                         mlib_s32 shift);
 *      mlib_status mlib_VectorMulSShift_S32_S32_[Sat|Mod](mlib_s32       *z,
 *                                                         const mlib_s32 *x,
 *                                                         const mlib_s32 *c,
 *                                                         mlib_s32       n,
 *                                                         mlib_s32 shift);
 *
 *      mlib_status mlib_VectorMulSShift_U8_[Sat|Mod](mlib_u8       *xz,
 *                                                    const mlib_u8 *c,
 *                                                    mlib_s32      n,
 *                                                    mlib_s32  shift);
 *      mlib_status mlib_VectorMulSShift_S8_[Sat|Mod](mlib_s8       *xz,
 *                                                    const mlib_s8 *c,
 *                                                    mlib_s32      n,
 *                                                    mlib_s32  shift);
 *      mlib_status mlib_VectorMulSShift_S16_[Sat|Mod](mlib_s16       *xz,
 *                                                     const mlib_s16 *c,
 *                                                     mlib_s32       n,
 *                                                     mlib_s32 shift);
 *      mlib_status mlib_VectorMulSShift_S32_[Sat|Mod](mlib_s32       *xz,
 *                                                     const mlib_s32 *c,
 *                                                     mlib_s32       n,
 *                                                     mlib_s32   shift);
 *
 * ARGUMENTS
 *      z      the pointer to the first element of the result vector
 *      xz     the pointer to the first element of the source and result vector
 *      x      the pointer to the first element of the source vector
 *      c      the pointer to the scalar
 *      n      the number of elements in each vector
 *      shift  the right shifting factor
 *
 * DESCRIPTION
 *      z[i] = x[i] * c * 2^(-shift)
 *      xz[i] = xz[i] * c * 2^(-shift)
 */

/*
 * FUNCTIONS
 *      mlib_VectorMulSShift_U8C_[Sat|Mod],
 *      mlib_VectorMulSShift_U8C_U8C_[Sat|Mod]   - multiplication of unsigned
 *                                                 8-bit format complex vector
 *                                                 by a scalar with shifting
 *      mlib_VectorMulSShift_S8C_[Sat|Mod],
 *      mlib_VectorMulSShift_S8C_S8C_[Sat|Mod]   - multiplication of signed
 *                                                 8-bit format complex vector
 *                                                 by a scalar with shifting
 *      mlib_VectorMulSShift_S16C_[Sat|Mod],
 *      mlib_VectorMulSShift_S16C_S16C_[Sat|Mod] - multiplication of signed
 *                                                 16-bit format complex vector
 *                                                 by a scalar with shifting
 *      mlib_VectorMulSShift_S32C_[Sat|Mod],
 *      mlib_VectorMulSShift_S32C_S32C_[Sat|Mod] - multiplication of signed
 *                                                 32-bit format complex vector
 *                                                 by a scalar with shifting
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMulSShift_U8C_U8C_[Sat|Mod](mlib_u8       *z,
 *                                                        const mlib_u8 *x,
 *                                                        const mlib_u8 *c,
 *                                                        mlib_s32       n,
 *                                                        mlib_s32   shift);
 *      mlib_status mlib_VectorMulSShift_S8C_S8C_[Sat|Mod](mlib_s8       *z,
 *                                                        const mlib_s8 *x,
 *                                                        const mlib_s8 *c,
 *                                                        mlib_s32       n,
 *                                                        mlib_s32   shift);
 *      mlib_status mlib_VectorMulSShift_S16C_S16C_[Sat|Mod](mlib_s16       *z,
 *                                                          const mlib_s16 *x,
 *                                                          const mlib_s16 *c,
 *                                                          mlib_s32       n,
 *                                                          mlib_s32   shift);
 *      mlib_status mlib_VectorMulSShift_S32C_S32C_[Sat|Mod](mlib_s32       *z,
 *                                                           const mlib_s32 *x,
 *                                                           const mlib_s32 *c,
 *                                                           mlib_s32       n,
 *                                                           mlib_s32   shift);
 *
 *      mlib_status mlib_VectorMulSShift_S8C_[Sat|Mod](mlib_s8       *xz,
 *                                                     const mlib_s8 *c,
 *                                                     mlib_s32      n,
 *                                                     mlib_s32      shift);
 *      mlib_status mlib_VectorMulSShift_U8C_[Sat|Mod](mlib_u8       *xz,
 *                                                     const mlib_u8 *c,
 *                                                     mlib_s32      n,
 *                                                     mlib_s32      shift);
 *      mlib_status mlib_VectorMulSShift_S16C_[Sat|Mod](mlib_s16       *xz,
 *                                                      const mlib_s16 *c,
 *                                                      mlib_s32       n,
 *                                                      mlib_s32       shift);
 *      mlib_status mlib_VectorMulSShift_S32C_[Sat|Mod](mlib_s32       *xz,
 *                                                      const mlib_s32 *c,
 *                                                      mlib_s32       n,
 *                                                      mlib_s32       shift);
 *
 * ARGUMENTS
 *      z      the pointer to the first complex element of the result vector.
 *             z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *             parts
 *      xz     the pointer to the first complex element of the source and
 *             result vector. xz[2*i] hold the real parts,
 *             and xz[2*i + 1] hold the imaginary parts
 *      x      the pointer to the first complex element of the source vector.
 *             x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *             parts
 *      c      the pointer to the complex scalar. c[0] hold the real part,
 *             and c[1] hold the imaginary part
 *      n      the number of complex elements in each vector
 *      shift  the right shifting factor
 *
 * DESCRIPTION
 *      z[2*i] = (x[2*i] * c[0] - x[2*i + 1] * c[1]) * 2^(-shift)
 *      z[2*i + 1] = (x[2*i + 1] * c[0] + x[2*i] * c[1]) * 2^(-shift)
 *      xz[2*i] = (xz[2*i] * c[0] - xz[2*i + 1] * c[1]) * 2^(-shift)
 *      xz[2*i + 1] = (xz[2*i + 1] * c[0] + xz[2*i] * c[1]) * 2^(-shift)
 */

#include <mlib_algebra.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorMulSShift_S8_S8_Sat = \
	__mlib_VectorMulSShift_S8_S8_Sat
#pragma weak mlib_VectorMulSShift_S8_S8_Mod = \
	__mlib_VectorMulSShift_S8_S8_Mod
#pragma weak mlib_VectorMulSShift_S8C_S8C_Sat = \
	__mlib_VectorMulSShift_S8C_S8C_Sat
#pragma weak mlib_VectorMulSShift_S8C_S8C_Mod = \
	__mlib_VectorMulSShift_S8C_S8C_Mod
#pragma weak mlib_VectorMulSShift_S8_Sat = __mlib_VectorMulSShift_S8_Sat
#pragma weak mlib_VectorMulSShift_S8_Mod = __mlib_VectorMulSShift_S8_Mod
#pragma weak mlib_VectorMulSShift_S8C_Sat = __mlib_VectorMulSShift_S8C_Sat
#pragma weak mlib_VectorMulSShift_S8C_Mod = __mlib_VectorMulSShift_S8C_Mod
#pragma weak mlib_VectorMulSShift_U8_U8_Sat = \
	__mlib_VectorMulSShift_U8_U8_Sat
#pragma weak mlib_VectorMulSShift_U8_U8_Mod = \
	__mlib_VectorMulSShift_U8_U8_Mod
#pragma weak mlib_VectorMulSShift_U8C_U8C_Sat = \
	__mlib_VectorMulSShift_U8C_U8C_Sat
#pragma weak mlib_VectorMulSShift_U8C_U8C_Mod = \
	__mlib_VectorMulSShift_U8C_U8C_Mod
#pragma weak mlib_VectorMulSShift_U8_Sat = __mlib_VectorMulSShift_U8_Sat
#pragma weak mlib_VectorMulSShift_U8_Mod = __mlib_VectorMulSShift_U8_Mod
#pragma weak mlib_VectorMulSShift_U8C_Sat = __mlib_VectorMulSShift_U8C_Sat
#pragma weak mlib_VectorMulSShift_U8C_Mod = __mlib_VectorMulSShift_U8C_Mod
#pragma weak mlib_VectorMulSShift_S16_S16_Sat = \
	__mlib_VectorMulSShift_S16_S16_Sat
#pragma weak mlib_VectorMulSShift_S16_S16_Mod = \
	__mlib_VectorMulSShift_S16_S16_Mod
#pragma weak mlib_VectorMulSShift_S16C_S16C_Sat = \
	__mlib_VectorMulSShift_S16C_S16C_Sat
#pragma weak mlib_VectorMulSShift_S16C_S16C_Mod = \
	__mlib_VectorMulSShift_S16C_S16C_Mod
#pragma weak mlib_VectorMulSShift_S16_Sat = __mlib_VectorMulSShift_S16_Sat
#pragma weak mlib_VectorMulSShift_S16_Mod = __mlib_VectorMulSShift_S16_Mod
#pragma weak mlib_VectorMulSShift_S16C_Sat = __mlib_VectorMulSShift_S16C_Sat
#pragma weak mlib_VectorMulSShift_S16C_Mod = __mlib_VectorMulSShift_S16C_Mod
#pragma weak mlib_VectorMulSShift_S32_S32_Sat = \
	__mlib_VectorMulSShift_S32_S32_Sat
#pragma weak mlib_VectorMulSShift_S32_S32_Mod = \
	__mlib_VectorMulSShift_S32_S32_Mod
#pragma weak mlib_VectorMulSShift_S32C_S32C_Sat = \
	__mlib_VectorMulSShift_S32C_S32C_Sat
#pragma weak mlib_VectorMulSShift_S32C_S32C_Mod = \
	__mlib_VectorMulSShift_S32C_S32C_Mod
#pragma weak mlib_VectorMulSShift_S32_Sat = __mlib_VectorMulSShift_S32_Sat
#pragma weak mlib_VectorMulSShift_S32_Mod = __mlib_VectorMulSShift_S32_Mod
#pragma weak mlib_VectorMulSShift_S32C_Sat = __mlib_VectorMulSShift_S32C_Sat
#pragma weak mlib_VectorMulSShift_S32C_Mod = __mlib_VectorMulSShift_S32C_Mod

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorMulSShift_S8_S8_Sat) mlib_VectorMulSShift_S8_S8_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_S8_S8_Sat")));
__typeof__(__mlib_VectorMulSShift_S8_S8_Mod) mlib_VectorMulSShift_S8_S8_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_S8_S8_Mod")));
__typeof__(__mlib_VectorMulSShift_S8C_S8C_Sat) mlib_VectorMulSShift_S8C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_S8C_S8C_Sat")));
__typeof__(__mlib_VectorMulSShift_S8C_S8C_Mod) mlib_VectorMulSShift_S8C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_S8C_S8C_Mod")));
__typeof__(__mlib_VectorMulSShift_S8_Sat) mlib_VectorMulSShift_S8_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_S8_Sat")));
__typeof__(__mlib_VectorMulSShift_S8_Mod) mlib_VectorMulSShift_S8_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_S8_Mod")));
__typeof__(__mlib_VectorMulSShift_S8C_Sat) mlib_VectorMulSShift_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_S8C_Sat")));
__typeof__(__mlib_VectorMulSShift_S8C_Mod) mlib_VectorMulSShift_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_S8C_Mod")));
__typeof__(__mlib_VectorMulSShift_U8_U8_Sat) mlib_VectorMulSShift_U8_U8_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_U8_U8_Sat")));
__typeof__(__mlib_VectorMulSShift_U8_U8_Mod) mlib_VectorMulSShift_U8_U8_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_U8_U8_Mod")));
__typeof__(__mlib_VectorMulSShift_U8C_U8C_Sat) mlib_VectorMulSShift_U8C_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_U8C_U8C_Sat")));
__typeof__(__mlib_VectorMulSShift_U8C_U8C_Mod) mlib_VectorMulSShift_U8C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_U8C_U8C_Mod")));
__typeof__(__mlib_VectorMulSShift_U8_Sat) mlib_VectorMulSShift_U8_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_U8_Sat")));
__typeof__(__mlib_VectorMulSShift_U8_Mod) mlib_VectorMulSShift_U8_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_U8_Mod")));
__typeof__(__mlib_VectorMulSShift_U8C_Sat) mlib_VectorMulSShift_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_U8C_Sat")));
__typeof__(__mlib_VectorMulSShift_U8C_Mod) mlib_VectorMulSShift_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_U8C_Mod")));
__typeof__(__mlib_VectorMulSShift_S16_S16_Sat) mlib_VectorMulSShift_S16_S16_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_S16_S16_Sat")));
__typeof__(__mlib_VectorMulSShift_S16_S16_Mod) mlib_VectorMulSShift_S16_S16_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_S16_S16_Mod")));
__typeof__(__mlib_VectorMulSShift_S16C_S16C_Sat)
	mlib_VectorMulSShift_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_S16C_S16C_Sat")));
__typeof__(__mlib_VectorMulSShift_S16C_S16C_Mod)
	mlib_VectorMulSShift_S16C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_S16C_S16C_Mod")));
__typeof__(__mlib_VectorMulSShift_S16_Sat) mlib_VectorMulSShift_S16_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_S16_Sat")));
__typeof__(__mlib_VectorMulSShift_S16_Mod) mlib_VectorMulSShift_S16_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_S16_Mod")));
__typeof__(__mlib_VectorMulSShift_S16C_Sat) mlib_VectorMulSShift_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_S16C_Sat")));
__typeof__(__mlib_VectorMulSShift_S16C_Mod) mlib_VectorMulSShift_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_S16C_Mod")));
__typeof__(__mlib_VectorMulSShift_S32_S32_Sat) mlib_VectorMulSShift_S32_S32_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_S32_S32_Sat")));
__typeof__(__mlib_VectorMulSShift_S32_S32_Mod) mlib_VectorMulSShift_S32_S32_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_S32_S32_Mod")));
__typeof__(__mlib_VectorMulSShift_S32C_S32C_Sat)
	mlib_VectorMulSShift_S32C_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_S32C_S32C_Sat")));
__typeof__(__mlib_VectorMulSShift_S32C_S32C_Mod)
	mlib_VectorMulSShift_S32C_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_S32C_S32C_Mod")));
__typeof__(__mlib_VectorMulSShift_S32_Sat) mlib_VectorMulSShift_S32_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_S32_Sat")));
__typeof__(__mlib_VectorMulSShift_S32_Mod) mlib_VectorMulSShift_S32_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_S32_Mod")));
__typeof__(__mlib_VectorMulSShift_S32C_Sat) mlib_VectorMulSShift_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_S32C_Sat")));
__typeof__(__mlib_VectorMulSShift_S32C_Mod) mlib_VectorMulSShift_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_S32C_Mod")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MUL_U8_U16                                              \
	{                                                       \
	    mlib_d64 dr2, dr3, dr4, dr5;                        \
	                                                        \
	    dr3 = vis_fmul8x16(vis_read_hi(dx), dc_hi);         \
	    dr2 = vis_fmul8x16al(vis_read_hi(dx), f127);        \
	    dr0 = vis_fpsub16(dr2, dr3);                        \
	    dr5 = vis_fmul8x16(vis_read_lo(dx), dc_lo);         \
	    dr4 = vis_fmul8x16al(vis_read_lo(dx), f127);        \
	    dr1 = vis_fpsub16(dr4, dr5);                        \
	}

/* *********************************************************** */

#define	PACK_U16_U8_8	dr0 = vis_bshuffle(dr0, dr1);

/* *********************************************************** */

#define	PACK_U16_U8                                                   \
	mask_over =                                                   \
	vis_fcmpgt16(dzero, dr1) | (vis_fcmpgt16(dzero, dr0) << 4);   \
	dr0 = vis_fpack16_to_hi(dr0, dr0);                            \
	dr0 = vis_fpack16_to_lo(dr0, dr1);

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s32 emask, mask_over;
	mlib_s32 off;
	mlib_s32 i;
	mlib_u8 *pzend;
	mlib_u8 uc = ((mlib_u8 *)c)[0];
	mlib_u16 ucc = (uc | (uc << 8));
	mlib_d64 *dpx = (mlib_d64 *)x, *dpz = (mlib_d64 *)z, *dpzend;
	mlib_d64 dx, dx0, dx1, dst0, dst1, dr0, dr1;
	mlib_s32 len;

	mlib_f32 f127 = vis_to_float(0x7F00);
	mlib_f32 fzero = vis_fzeros();
	mlib_d64 dzero = vis_fzero();
	mlib_d64 d_over = vis_fone();
	mlib_d64 restore = vis_to_double_dup(0x80808080);
	mlib_d64 dc = vis_to_double_dup(ucc | (ucc << 16));
	mlib_d64 dc_r = vis_fxnor(restore, dc);
	mlib_d64 dc_hi = vis_fpmerge(vis_read_hi(dc_r), fzero);
	mlib_d64 dc_lo = vis_fpmerge(vis_read_lo(dc_r), fzero);

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

	if (shift == 8) {

		vis_write_bmask(0x02468ACE, 0);
		pzend = (mlib_u8 *)z + n - 1;
		dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
		dpz = (mlib_d64 *)((mlib_addr)z & (~7));
		off = (mlib_addr)dpz - (mlib_addr)z;

		emask = vis_edge8(z, pzend);

		dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);

		MUL_U8_U16;
		PACK_U16_U8_8;
		vis_pst_8(dr0, dpz, emask);

		dpx++;
		dpz++;

		len = dpzend - dpz;
		if (len < 0)
			return (MLIB_SUCCESS);

		if (!((mlib_addr)x & 7) && !((mlib_addr)z & 7)) {

#pragma pipeloop(0)
			for (i = 0; i < len - 1; i += 2) {
				mlib_d64 dr0a, dr1a, dr2a, dr3a, dr4a, dr5a;
				mlib_d64 dr0b, dr1b, dr2b, dr3b, dr4b, dr5b;
				mlib_s32 mask_over0, mask_over1;

				dx0 = dpx[i];
				dx1 = dpx[i + 1];

				dr3a = vis_fmul8x16(vis_read_hi(dx0), dc_hi);
				dr2a = vis_fmul8x16al(vis_read_hi(dx0), f127);
				dr0a = vis_fpsub16(dr2a, dr3a);
				dr5a = vis_fmul8x16(vis_read_lo(dx0), dc_lo);
				dr4a = vis_fmul8x16al(vis_read_lo(dx0), f127);
				dr1a = vis_fpsub16(dr4a, dr5a);

				dr3b = vis_fmul8x16(vis_read_hi(dx1), dc_hi);
				dr2b = vis_fmul8x16al(vis_read_hi(dx1), f127);
				dr0b = vis_fpsub16(dr2b, dr3b);
				dr5b = vis_fmul8x16(vis_read_lo(dx1), dc_lo);
				dr4b = vis_fmul8x16al(vis_read_lo(dx1), f127);
				dr1b = vis_fpsub16(dr4b, dr5b);

				dr0a = vis_bshuffle(dr0a, dr1a);
				dr0b = vis_bshuffle(dr0b, dr1b);

				dpz[i] = dr0a;
				dpz[i + 1] = dr0b;
			}

			if (len % 2) {
				dx = dpx[len - 1];
				MUL_U8_U16;
				PACK_U16_U8_8;
				dpz[len - 1] = dr0;
			}

			dpz = dpzend;
			dx = vis_ld_d64_nf(dpx + len);

		} else {

			dx0 = vis_ld_d64_nf(dpx);
			dx1 = vis_ld_d64_nf(dpx + 1);
			dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
			for (i = 0; i < len; i++) {
				dx0 = dx1;
				MUL_U8_U16;
				PACK_U16_U8_8;
				dx1 = vis_ld_d64_nf(dpx + i + 2);
				dpz[i] = dr0;
				dx = vis_faligndata(dx0, dx1);
			}

			dpz = dpzend;
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			MUL_U8_U16;
			PACK_U16_U8_8;
/* prepare edge mask for the last bytes */
			emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
			vis_pst_8(dr0, dpz, emask);
		}

	} else {

		vis_write_gsr((7 - shift) << 3);
		pzend = (mlib_u8 *)z + n - 1;
		dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
		dpz = (mlib_d64 *)((mlib_addr)z & (~7));
		off = (mlib_addr)dpz - (mlib_addr)z;

		emask = vis_edge8(z, pzend);

		dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);

		MUL_U8_U16;
		PACK_U16_U8;
		vis_pst_8(dr0, dpz, emask);
		vis_pst_8(d_over, dpz, mask_over & emask);

		dpx++;
		dpz++;
		len = dpzend - dpz;
		if (len < 0)
			return (MLIB_SUCCESS);

		if (!((mlib_addr)x & 7) && !((mlib_addr)z & 7)) {

#pragma pipeloop(0)
			for (i = 0; i < len - 1; i += 2) {
				mlib_d64 dr0a, dr1a, dr2a, dr3a, dr4a, dr5a;
				mlib_d64 dr0b, dr1b, dr2b, dr3b, dr4b, dr5b;
				mlib_s32 mask_over0, mask_over1;

				dx0 = dpx[i];
				dx1 = dpx[i + 1];

				dr3a = vis_fmul8x16(vis_read_hi(dx0), dc_hi);
				dr2a = vis_fmul8x16al(vis_read_hi(dx0), f127);
				dr0a = vis_fpsub16(dr2a, dr3a);
				dr5a = vis_fmul8x16(vis_read_lo(dx0), dc_lo);
				dr4a = vis_fmul8x16al(vis_read_lo(dx0), f127);
				dr1a = vis_fpsub16(dr4a, dr5a);

				dr3b = vis_fmul8x16(vis_read_hi(dx1), dc_hi);
				dr2b = vis_fmul8x16al(vis_read_hi(dx1), f127);
				dr0b = vis_fpsub16(dr2b, dr3b);
				dr5b = vis_fmul8x16(vis_read_lo(dx1), dc_lo);
				dr4b = vis_fmul8x16al(vis_read_lo(dx1), f127);
				dr1b = vis_fpsub16(dr4b, dr5b);

				mask_over0 = vis_fcmpgt16(dzero, dr1a) |
					(vis_fcmpgt16(dzero, dr0a) << 4);
				dr0a = vis_fpack16_to_hi(dr0a, dr0a);
				dr0a = vis_fpack16_to_lo(dr0a, dr1a);

				mask_over1 = vis_fcmpgt16(dzero, dr1b) |
					(vis_fcmpgt16(dzero, dr0b) << 4);
				dr0b = vis_fpack16_to_hi(dr0b, dr0b);
				dr0b = vis_fpack16_to_lo(dr0b, dr1b);

				dpz[i] = dr0a;
				vis_pst_8(d_over, dpz + i, mask_over0);

				dpz[i + 1] = dr0b;
				vis_pst_8(d_over, dpz + i + 1, mask_over1);
			}

			if (len % 2) {
				dx = dpx[len - 1];
				MUL_U8_U16;
				PACK_U16_U8;
				dpz[len - 1] = dr0;
				vis_pst_8(d_over, dpz + len - 1, mask_over);
			}
			dpz = dpzend;
			dx = vis_ld_d64_nf(dpx + len);

		} else {

			dx0 = vis_ld_d64_nf(dpx);
			dx1 = vis_ld_d64_nf(dpx + 1);
			dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
			for (i = 0; i < len; i++) {
				dx0 = dx1;
				MUL_U8_U16;
				PACK_U16_U8;
				dx1 = vis_ld_d64_nf(dpx + i + 2);
				dpz[i] = dr0;
				vis_pst_8(d_over, dpz + i, mask_over);
				dx = vis_faligndata(dx0, dx1);
			}
			dpz = dpzend;
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			MUL_U8_U16;
			PACK_U16_U8;
/* prepare edge mask for the last bytes */
			emask = vis_edge8(dpz, pzend);
			vis_pst_8(dr0, dpz, emask);
			vis_pst_8(d_over, dpz, mask_over & emask);
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_U8_Sat(
	mlib_u8 *xz,
	const mlib_u8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_U8_U8_Sat(xz, xz, c, n, shift));
}

/* *********************************************************** */

#define	MUL_S16_U8_MOD                                          \
	{                                                       \
	    mlib_d64 dr2, dr3, dr4, dr5;                        \
	                                                        \
	    dr3 = vis_fmul8x16(vis_read_hi(dx), dc_hi);         \
	    dr2 = vis_fmul8x16al(vis_read_hi(dx), f127);        \
	    dr = vis_fpsub16(dr2, dr3);                         \
	    dr4 = vis_fmul8x16(vis_read_lo(dx), dc_lo);         \
	    dr5 = vis_fmul8x16al(vis_read_lo(dx), f127);        \
	    dr1 = vis_fpsub16(dr5, dr4);                        \
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
	dr = vis_fpack16_to_lo(dr, dr1);

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s32 emask, i;
	mlib_s32 off;
	mlib_u8 *pzend, *px = (mlib_u8 *)x;
	mlib_d64 *dpz = (mlib_d64 *)z, *dpx = (mlib_d64 *)x, *dpzend;
	mlib_d64 dx, dy, dr, dr1, dx0, dx1, dy0, dy1;
	mlib_s32 len;
	mlib_f32 fzero = vis_fzeros(), f127 = vis_to_float(0x7f00);
	mlib_d64 without_control = vis_to_double_dup(0xff00ff);
	mlib_d64 restore = vis_to_double_dup(0x80808080);
	mlib_d64 anti_rounding_mask = vis_to_double_dup(0x10001 << (shift - 1));
	mlib_f32 scale = vis_to_float(0x80808080 >> (shift - 1));

	mlib_u8 uc = ((mlib_u8 *)c)[0];
	mlib_u16 ucc = uc | (uc << 8);
	mlib_d64 dc = vis_to_double_dup(ucc | (ucc << 16));
	mlib_d64 dc_r = vis_fxnor(restore, dc);
	mlib_d64 dc_hi = vis_fpmerge(vis_read_hi(dc_r), fzero);
	mlib_d64 dc_lo = vis_fpmerge(vis_read_lo(dc_r), fzero);

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);

	pzend = (mlib_u8 *)z + n - 1;
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z;

	emask = vis_edge8(z, pzend);

	dpx = (mlib_d64 *)vis_alignaddr(px, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_faligndata(dx0, dx1);
	MUL_S16_U8_MOD;
	PACK_U16_U8_MOD;
/* store first bytes of result */
	vis_pst_8(dr, dpz, emask);
	dpx++;
	dpz++;
	len = dpzend - dpz;

	if (len < 0)
		return (MLIB_SUCCESS);

	if (!((mlib_addr)x & 7) && !((mlib_addr)z & 7)) {

#pragma pipeloop(0)
		for (i = 0; i < len - 1; i += 2) {
			mlib_d64 dr0a, dr1a, dr2a, dr3a, dr4a, dr5a;
			mlib_d64 dr0b, dr1b, dr2b, dr3b, dr4b, dr5b;

			dx0 = dpx[i];
			dx1 = dpx[i + 1];

			dr3a = vis_fmul8x16(vis_read_hi(dx0), dc_hi);
			dr2a = vis_fmul8x16al(vis_read_hi(dx0), f127);
			dr0a = vis_fpsub16(dr2a, dr3a);
			dr4a = vis_fmul8x16(vis_read_lo(dx0), dc_lo);
			dr5a = vis_fmul8x16al(vis_read_lo(dx0), f127);
			dr1a = vis_fpsub16(dr5a, dr4a);
			dr0a = vis_fandnot(anti_rounding_mask, dr0a);
			dr1a = vis_fandnot(anti_rounding_mask, dr1a);
			dr0a = vis_fmul8x16(scale, dr0a);
			dr1a = vis_fmul8x16(scale, dr1a);

			dr3b = vis_fmul8x16(vis_read_hi(dx1), dc_hi);
			dr2b = vis_fmul8x16al(vis_read_hi(dx1), f127);
			dr0b = vis_fpsub16(dr2b, dr3b);
			dr4b = vis_fmul8x16(vis_read_lo(dx1), dc_lo);
			dr5b = vis_fmul8x16al(vis_read_lo(dx1), f127);
			dr1b = vis_fpsub16(dr5b, dr4b);
			dr0b = vis_fandnot(anti_rounding_mask, dr0b);
			dr1b = vis_fandnot(anti_rounding_mask, dr1b);
			dr0b = vis_fmul8x16(scale, dr0b);
			dr1b = vis_fmul8x16(scale, dr1b);

			dr0a = vis_fand(dr0a, without_control);
			dr1a = vis_fand(dr1a, without_control);
			dr0a = vis_fpack16_to_hi(dr0a, dr0a);
			dr0a = vis_fpack16_to_lo(dr0a, dr1a);

			dr0b = vis_fand(dr0b, without_control);
			dr1b = vis_fand(dr1b, without_control);
			dr0b = vis_fpack16_to_hi(dr0b, dr0b);
			dr0b = vis_fpack16_to_lo(dr0b, dr1b);

			dpz[i] = dr0a;
			dpz[i + 1] = dr0b;
		}

		if (len % 2) {
			dx = dpx[len - 1];
			MUL_S16_U8_MOD;
			PACK_U16_U8_MOD;
			dpz[len - 1] = dr;
		}

		dpz = dpzend;
		dx = vis_ld_d64_nf(dpx + len);

	} else {

		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
		for (i = 0; i < len; i++) {
			dx0 = dx1;
			MUL_S16_U8_MOD;
			PACK_U16_U8_MOD;
			dx1 = vis_ld_d64_nf(dpx + i + 2);
			dpz[i] = dr;
			dx = vis_faligndata(dx0, dx1);
		}

		dpz = dpzend;

	}

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
		MUL_S16_U8_MOD;
		PACK_U16_U8_MOD;
/* prepare edge mask for the last bytes */
		emask = vis_edge8(dpz, pzend);
		vis_pst_8(dr, dpz, emask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_U8_Mod(
	mlib_u8 *xz,
	const mlib_u8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_U8_U8_Mod(xz, xz, c, n, shift));
}

/* *********************************************************** */

#define	EXPAND_U8C_FOR_MUL             \
/* x_r */                              \
	dr2 = vis_faligndata(dx, dx);

/* *********************************************************** */

#define	EXPAND_U8C_FOR_MUL_8	dr2 = vis_faligndata(dx, dx);	/* x_r */

/* *********************************************************** */

#define	MUL_U8C_S16C                                                       \
	{                                                                  \
	    mlib_d64 dr4, dr5, dr6, dr7, dr8;                              \
	                                                                   \
	    dr1 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dc_hi));    \
	    dr5 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dc_hi));    \
	    dr = vis_fmuld8ulx16(vis_read_hi(dr2), vis_read_hi(dc_hi));    \
	    dr3 = vis_fmuld8ulx16(vis_read_lo(dr2), vis_read_lo(dc_hi));   \
	    dr6 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dc_lo));    \
	    dr7 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dc_lo));    \
	    dr8 = vis_fmuld8ulx16(vis_read_hi(dr2), vis_read_hi(dc_lo));   \
	    dr4 = vis_fmuld8ulx16(vis_read_lo(dr2), vis_read_lo(dc_lo));   \
	    dr = vis_fpsub32(dr, dr6);                                     \
	    dr2 = vis_fpadd32(dr1, dr8);                                   \
	    dr1 = vis_fpsub32(dr3, dr7);                                   \
	    dr3 = vis_fpadd32(dr5, dr4);                                   \
	    dr = vis_freg_pair(vis_fpackfix(dr), vis_fpackfix(dr1));       \
	    dr1 = vis_freg_pair(vis_fpackfix(dr2), vis_fpackfix(dr3));     \
	}

/* *********************************************************** */

#define	MUL_U8C_S16C_8                                                   \
	{                                                                \
	    mlib_d64 dr4, dr5, dr6, dr7, dr8;                            \
	                                                                 \
	    dr1 = vis_fmuld8ulx16(vis_read_hi(dx),                       \
		    vis_read_hi(dc_hi_8));                               \
	    dr5 = vis_fmuld8ulx16(vis_read_lo(dx),                       \
		    vis_read_lo(dc_hi_8));                               \
	    dr = vis_fmuld8ulx16(vis_read_hi(dr2),                       \
		    vis_read_hi(dc_hi_8));                               \
	    dr3 = vis_fmuld8ulx16(vis_read_lo(dr2),                      \
		    vis_read_lo(dc_hi_8));                               \
	    dr6 = vis_fmuld8ulx16(vis_read_hi(dx),                       \
		    vis_read_hi(dc_lo_8));                               \
	    dr7 = vis_fmuld8ulx16(vis_read_lo(dx),                       \
		    vis_read_lo(dc_lo_8));                               \
	    dr8 = vis_fmuld8ulx16(vis_read_hi(dr2),                      \
		    vis_read_hi(dc_lo_8));                               \
	    dr4 = vis_fmuld8ulx16(vis_read_lo(dr2),                      \
		    vis_read_lo(dc_lo_8));                               \
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
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));

/* *********************************************************** */

#define	PACK_S16C_U8C0_8                                         \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));      \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));      \
	dr1 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));   \
	dr1 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));   \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));

/* *********************************************************** */

#define	PACK_S16C_U8C1                                          \
	dr = vis_fpack16_to_hi(dr, dr);                         \
	dr = vis_fpack16_to_lo(dr, dr1);                        \
	dr1 = vis_faligndata(dr, dr);                           \
	dr = vis_fpmerge(vis_read_lo(dr1), vis_read_hi(dr));

/* *********************************************************** */

#define	EXPAND_U8C_FOR_0_MUL	dr2 = vis_bshuffle(dx, dx);

/* *********************************************************** */

#define	EXPAND_U8C_FOR_0_MUL_8	dr2 = vis_bshuffle(dx, dx);

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_U8C_U8C_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
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
	mlib_d64 *dpz, *dpx, *dpzend;
	mlib_d64 dx, dr, dr1, dr2, dr3, dr3_8, dx0, dx1;
	mlib_d64 dc_hi, dc_lo, dc_hi_8, dc_lo_8;
	mlib_d64 clear = vis_to_double_dup(0xFF00FF00);
	mlib_d64 dzero = vis_fzero();
	mlib_s32 N, k;
	mlib_d64 dr1_buff[256], dr_buff[256];
	mlib_u64 gsr_for_mul_64 = (((mlib_u64)0x00224466 << 32) | gsr_for_mul);
	mlib_u64 gsr_for_mul_64_8 =
		(((mlib_u64)0x00224466 << 32) | gsr_for_mul_8);
	mlib_u64 gsr_for_pack_64_8 =
		(((mlib_u64)0x082A4C6E << 32) | gsr_for_pack_8);

	mlib_u16 uc = ((mlib_u8 *)c)[1] | (((mlib_u8 *)c)[0] << 8);
	mlib_d64 dc = vis_to_double_dup(uc | (uc << 16));

	dc = vis_fpmerge(vis_read_hi(dc), vis_read_lo(dc));
	dc = vis_fpmerge(vis_read_hi(dc), vis_read_lo(dc));
	dc_hi = vis_fexpand(vis_read_hi(dc));
	dc_lo = vis_fexpand(vis_read_lo(dc));
	dc_hi_8 = vis_fpmerge(vis_fzeros(), vis_read_hi(dc));
	dc_lo_8 = vis_fpmerge(vis_fzeros(), vis_read_lo(dc));

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

	if (shift == 8) {

		pzend = (mlib_u8 *)z + n + n - 1;
		dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
		dpz = (mlib_d64 *)((mlib_addr)z & (~7));
		off = (mlib_addr)dpz - (mlib_addr)z + (1 & (mlib_addr)z);

		emask = vis_edge8(z, pzend);

		dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);

/* initialize GSR */
		vis_write_gsr(gsr_for_mul_8);
		EXPAND_U8C_FOR_MUL_8;
		MUL_U8C_S16C_8;
		if (!(1 & (mlib_addr)z)) {
			vis_write_gsr(gsr_for_pack_8);
			PACK_S16C_U8C0;
			vis_pst_8(dr, dpz, emask);
			dpz++;
			dpx++;
			len = dpzend - dpz;

			if (len < 0)
				return (MLIB_SUCCESS);

			if (!((mlib_addr)x & 7) && !((mlib_addr)z & 7)) {

				for (k = 0; k < (len >> 8); k++) {
/* initialize GSR */
					vis_write_gsr64(gsr_for_mul_64_8);
#pragma pipeloop(0)
					for (i = 0; i < 256; i++) {
						dx = dpx[256 * k + i];
						EXPAND_U8C_FOR_0_MUL_8;
						MUL_U8C_S16C_8;
						dr1_buff[i] = dr1;
						dr_buff[i] = dr;
					}

					vis_write_gsr64(gsr_for_pack_64_8);
#pragma pipeloop(0)
					for (i = 0; i < 256; i++) {
						dr1 = dr1_buff[i];
						dr = dr_buff[i];
						PACK_S16C_U8C0;
						dpz[256 * k + i] = dr;
					}
				}

				N = len & ~255;
/* initialize GSR */
				vis_write_gsr64(gsr_for_mul_64_8);
#pragma pipeloop(0)
				for (i = 0; i < len - N; i++) {
					dx = dpx[i + N];
					EXPAND_U8C_FOR_0_MUL_8;
					MUL_U8C_S16C_8;
					dr1_buff[i] = dr1;
					dr_buff[i] = dr;
				}

				vis_write_gsr64(gsr_for_pack_64_8);
#pragma pipeloop(0)
				for (i = 0; i < len - N; i++) {
					dr1 = dr1_buff[i];
					dr = dr_buff[i];
					PACK_S16C_U8C0;
					dpz[i + N] = dr;
				}

				dpx += len;
				dpz += len;
				dx = vis_ld_d64_nf(dpx);

			} else {

				for (k = 0; k < (len >> 8); k++) {
/* initialize GSR */
					vis_write_gsr64(gsr_for_mul_64_8);
					(void *)vis_alignaddr((void *)x, off);
					dx0 = vis_ld_d64_nf(dpx + 256 * k);
					dx1 = vis_ld_d64_nf(dpx + 256 * k + 1);
					dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
					for (i = 0; i < 256; i++) {
						dx0 = dx1;
						EXPAND_U8C_FOR_0_MUL_8;
						MUL_U8C_S16C_8;
						dx1 = vis_ld_d64_nf(dpx +
							256 * k + i + 2);
						dr1_buff[i] = dr1;
						dr_buff[i] = dr;
						dx = vis_faligndata(dx0, dx1);
					}

					vis_write_gsr(gsr_for_pack_8);
#pragma pipeloop(0)
					for (i = 0; i < 256; i++) {
						dr1 = dr1_buff[i];
						dr = dr_buff[i];
						PACK_S16C_U8C0;
						dpz[256 * k + i] = dr;
					}
				}

				N = len & ~255;
/* initialize GSR */
				vis_write_gsr64(gsr_for_mul_64_8);
				(void *)vis_alignaddr((void *)x, off);
				dx0 = vis_ld_d64_nf(dpx + N);
				dx1 = vis_ld_d64_nf(dpx + N + 1);
				dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
				for (i = N; i < len; i++) {
					dx0 = dx1;
					EXPAND_U8C_FOR_0_MUL_8;
					MUL_U8C_S16C_8;
					dx1 = vis_ld_d64_nf(dpx + i + 2);
					dr1_buff[i - N] = dr1;
					dr_buff[i - N] = dr;
					dx = vis_faligndata(dx0, dx1);
				}

				vis_write_gsr(gsr_for_pack_8);
#pragma pipeloop(0)
				for (i = 0; i < len - N; i++) {
					dr1 = dr1_buff[i];
					dr = dr_buff[i];
					PACK_S16C_U8C0 dpz[i + N] = dr;
				}

				dpz = dpzend;
			}

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
/* initialize GSR */
				vis_write_gsr(gsr_for_mul_8);
				EXPAND_U8C_FOR_MUL_8;
				MUL_U8C_S16C_8;
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
/* initialize GSR */
		vis_write_gsr(gsr_for_pack_8);
		PACK_S16C_U8C1;
		vis_pst_8(dr, dpz, emask);

		if ((mlib_addr)++dpz <= (mlib_addr)pzend) {
			vis_pst_8(dr1, dpz, 0x80);
		}

		len = dpzend - dpz;

		if (len < 0)
			return (MLIB_SUCCESS);

		for (k = 0; k < (len >> 8); k++) {
/* initialize GSR */
			vis_write_gsr64(gsr_for_mul_64_8);
			(void *)vis_alignaddr((void *)x, off);
			dx0 = vis_ld_d64_nf(dpx + 256 * k);
			dx1 = vis_ld_d64_nf(dpx + 256 * k + 1);
			dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
			for (i = 0; i < 256; i++) {
				dx0 = dx1;
				EXPAND_U8C_FOR_0_MUL_8;
				MUL_U8C_S16C_8;
				dx1 = vis_ld_d64_nf(dpx + 256 * k + i + 2);
				dr1_buff[i] = dr1;
				dr_buff[i] = dr;
				dx = vis_faligndata(dx0, dx1);
			}

			vis_write_gsr(gsr_for_pack_8);
#pragma pipeloop(0)
			for (i = 0; i < 256; i++) {
				dr1 = dr1_buff[i];
				dr = dr_buff[i];
				PACK_S16C_U8C1;
				vis_pst_8(dr, dpz + 256 * k + i, 0x7f);
				vis_pst_8(dr1, dpz + 256 * k + i + 1, 0x80);
			}
		}

		N = len & ~255;
/* initialize GSR */
		vis_write_gsr64(gsr_for_mul_64_8);
		(void *)vis_alignaddr((void *)x, off);
		dx0 = vis_ld_d64_nf(dpx + N);
		dx1 = vis_ld_d64_nf(dpx + N + 1);
		dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
		for (i = N; i < len; i++) {
			dx0 = dx1;
			EXPAND_U8C_FOR_0_MUL_8;
			MUL_U8C_S16C_8;
			dx1 = vis_ld_d64_nf(dpx + i + 2);
			dr1_buff[i - N] = dr1;
			dr_buff[i - N] = dr;
			dx = vis_faligndata(dx0, dx1);
		}

		vis_write_gsr(gsr_for_pack_8);
#pragma pipeloop(0)
		for (i = 0; i < len - N; i++) {
			dr1 = dr1_buff[i];
			dr = dr_buff[i];
			PACK_S16C_U8C1;
			vis_pst_8(dr, dpz + i + N, 0x7f);
			vis_pst_8(dr1, dpz + i + N + 1, 0x80);
		}

		dpz = dpzend;

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
/* initialize GSR */
			vis_write_gsr(gsr_for_mul_8);
			EXPAND_U8C_FOR_MUL_8;
			MUL_U8C_S16C_8;
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

		pzend = (mlib_u8 *)z + n + n - 1;
		dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
		dpz = (mlib_d64 *)((mlib_addr)z & (~7));
		off = (mlib_addr)dpz - (mlib_addr)z + (1 & (mlib_addr)z);

		emask = vis_edge8(z, pzend);

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
			len = dpzend - dpz;

			if (len < 0)
				return (MLIB_SUCCESS);

			if (!((mlib_addr)x & 7) && !((mlib_addr)z & 7)) {

				for (k = 0; k < (len >> 8); k++) {
/* initialize GSR */
					vis_write_gsr64(gsr_for_mul_64);
#pragma pipeloop(0)
					for (i = 0; i < 256; i++) {
						dx = dpx[256 * k + i];
						EXPAND_U8C_FOR_0_MUL;
						MUL_U8C_S16C;
						dr1_buff[i] = dr1;
						dr_buff[i] = dr;
					}

					vis_write_gsr(gsr_for_pack);
#pragma pipeloop(0)
					for (i = 0; i < 256; i++) {
						dr1 = dr1_buff[i];
						dr = dr_buff[i];
						PACK_S16C_U8C0;
						dpz[256 * k + i] = dr;
					}
				}

				N = len & ~255;
/* initialize GSR */
				vis_write_gsr64(gsr_for_mul_64);
#pragma pipeloop(0)
				for (i = 0; i < len - N; i++) {
					dx = dpx[i + N];
					EXPAND_U8C_FOR_0_MUL;
					MUL_U8C_S16C;
					dr1_buff[i] = dr1;
					dr_buff[i] = dr;
				}

				vis_write_gsr(gsr_for_pack);
#pragma pipeloop(0)
				for (i = 0; i < len - N; i++) {
					dr1 = dr1_buff[i];
					dr = dr_buff[i];
					PACK_S16C_U8C0;
					dpz[i + N] = dr;
				}

				dpx += len;
				dpz += len;
				dx = vis_ld_d64_nf(dpx);

			} else {

				for (k = 0; k < (len >> 8); k++) {
/* initialize GSR */
					vis_write_gsr64(gsr_for_mul_64);
					(void *)vis_alignaddr((void *)x, off);
					dx0 = vis_ld_d64_nf(dpx + 256 * k);
					dx1 = vis_ld_d64_nf(dpx + 256 * k + 1);
					dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
					for (i = 0; i < 256; i++) {
						dx0 = dx1;
						EXPAND_U8C_FOR_0_MUL;
						MUL_U8C_S16C;
						dx1 = vis_ld_d64_nf(dpx +
							256 * k + i + 2);
						dr1_buff[i] = dr1;
						dr_buff[i] = dr;
						dx = vis_faligndata(dx0, dx1);
					}

					vis_write_gsr(gsr_for_pack);
#pragma pipeloop(0)
					for (i = 0; i < 256; i++) {
						dr1 = dr1_buff[i];
						dr = dr_buff[i];
						PACK_S16C_U8C0;
						dpz[256 * k + i] = dr;
					}
				}

				N = len & ~255;
/* initialize GSR */
				vis_write_gsr64(gsr_for_mul_64);
				(void *)vis_alignaddr((void *)x, off);
				dx0 = vis_ld_d64_nf(dpx + N);
				dx1 = vis_ld_d64_nf(dpx + N + 1);
				dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
				for (i = N; i < len; i++) {
					dx0 = dx1;
					EXPAND_U8C_FOR_0_MUL;
					MUL_U8C_S16C;
					dx1 = vis_ld_d64_nf(dpx + i + 2);
					dr1_buff[i - N] = dr1;
					dr_buff[i - N] = dr;
					dx = vis_faligndata(dx0, dx1);
				}

				vis_write_gsr(gsr_for_pack);
#pragma pipeloop(0)
				for (i = N; i < len; i++) {
					dr1 = dr1_buff[i - N];
					dr = dr_buff[i - N];
					PACK_S16C_U8C0;
					dpz[i] = dr;
				}

				dpz = dpzend;
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
/* initialize GSR */
		vis_write_gsr(gsr_for_pack);
		PACK_S16C_U8C1;
		vis_pst_8(dr, dpz, emask);

		if ((mlib_addr)++dpz <= (mlib_addr)pzend) {
			vis_pst_8(dr1, dpz, 0x80);
		}

		len = dpzend - dpz;

		if (len < 0)
			return (MLIB_SUCCESS);

		for (k = 0; k < (len >> 8); k++) {
/* initialize GSR */
			vis_write_gsr64(gsr_for_mul_64);
			(void *)vis_alignaddr((void *)x, off);
			dx0 = vis_ld_d64_nf(dpx + 256 * k);
			dx1 = vis_ld_d64_nf(dpx + 256 * k + 1);
			dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
			for (i = 0; i < 256; i++) {
				dx0 = dx1;
				EXPAND_U8C_FOR_0_MUL;
				MUL_U8C_S16C;
				dx1 = vis_ld_d64_nf(dpx + 256 * k + i + 2);
				dr1_buff[i] = dr1;
				dr_buff[i] = dr;
				dx = vis_faligndata(dx0, dx1);
			}

			vis_write_gsr(gsr_for_pack);
#pragma pipeloop(0)
			for (i = 0; i < 256; i++) {
				dr1 = dr1_buff[i];
				dr = dr_buff[i];
				PACK_S16C_U8C1;
				vis_pst_8(dr, dpz + 256 * k + i, 0x7f);
				vis_pst_8(dr1, dpz + 256 * k + i + 1, 0x80);
			}
		}

		N = len & ~255;
/* initialize GSR */
		vis_write_gsr64(gsr_for_mul_64);
		(void *)vis_alignaddr((void *)x, off);
		dx0 = vis_ld_d64_nf(dpx + N);
		dx1 = vis_ld_d64_nf(dpx + N + 1);
		dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
		for (i = N; i < len; i++) {
			dx0 = dx1;
			EXPAND_U8C_FOR_0_MUL;
			MUL_U8C_S16C;
			dx1 = vis_ld_d64_nf(dpx + i + 2);
			dr1_buff[i - N] = dr1;
			dr_buff[i - N] = dr;
			dx = vis_faligndata(dx0, dx1);
		}

		vis_write_gsr(gsr_for_pack);
#pragma pipeloop(0)
		for (i = N; i < len; i++) {
			dr1 = dr1_buff[i - N];
			dr = dr_buff[i - N];
			PACK_S16C_U8C1;
			vis_pst_8(dr, dpz + i, 0x7f);
			vis_pst_8(dr1, dpz + i + 1, 0x80);
		}

		dpz = dpzend;

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
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

mlib_status
__mlib_VectorMulSShift_U8C_Mod(
	mlib_u8 *xz,
	const mlib_u8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_U8C_U8C_Mod(xz, xz, c, n, shift));
}

/* *********************************************************** */

#define	EXPAND_U8C_FOR_MUL_MOD                                  \
	dr2 = vis_faligndata(dx, dx);                           \
	/* \ x_r \ */
/* *********************************************************** */

#define	PACK_S16C_U8C0_MOD                                      \
	dr = vis_fpack32(dr, dzero);                            \
	dr = vis_for(dr, dr1);

/* *********************************************************** */

#define	PACK_S16C_U8C1_MOD_BSH	dr1 = vis_bshuffle(dr, dr1);

/* *********************************************************** */

#define	EXPAND_U8C_FOR_0_MUL_MOD	dr2 = vis_bshuffle(dx, dx);

/* *********************************************************** */

#define	MUL_U8C_S16C_MOD                                                   \
	{                                                                  \
	    mlib_d64 dr4, dr5, dr6, dr7, dr8;                              \
	                                                                   \
	    dr1 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dc_hi));    \
	    dr5 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dc_hi));    \
	    dr = vis_fmuld8ulx16(vis_read_hi(dr2), vis_read_hi(dc_hi));    \
	    dr3 = vis_fmuld8ulx16(vis_read_lo(dr2), vis_read_lo(dc_hi));   \
	    dr6 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dc_lo));    \
	    dr7 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dc_lo));    \
	    dr8 = vis_fmuld8ulx16(vis_read_hi(dr2), vis_read_hi(dc_lo));   \
	    dr4 = vis_fmuld8ulx16(vis_read_lo(dr2), vis_read_lo(dc_lo));   \
	    dr = vis_fpsub32(dr, dr6);                                     \
	    dr2 = vis_fpadd32(dr1, dr8);                                   \
	    dr1 = vis_fpsub32(dr3, dr7);                                   \
	    dr3 = vis_fpadd32(dr5, dr4);                                   \
	    dr = vis_fand(dr, mod_mask);                                   \
	    dr1 = vis_fand(dr1, mod_mask);                                 \
	    dr2 = vis_fand(dr2, mod_mask);                                 \
	    dr3 = vis_fand(dr3, mod_mask);                                 \
	    dr = vis_freg_pair(vis_fpackfix(dr), vis_fpackfix(dr1));       \
	    dr1 = vis_freg_pair(vis_fpackfix(dr2), vis_fpackfix(dr3));     \
	}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
/* edge masks */
	mlib_s32 emask, len, i;
	mlib_s32 gsr_for_mul = ((16 - shift) << 3) | 7;
	mlib_s32 gsr_for_pack = (0 << 3) | 7;
	mlib_u8 *pzend, *px = (void *)x;
	mlib_d64 *dpz, *dpx, *dpzend;
	mlib_d64 dx, dr, dr1, dr2, dr3, dx0, dx1, dc_hi, dc_lo;
	mlib_d64 clear = vis_to_double_dup(0xFF00FF00);
	mlib_d64 dzero = vis_fzero();
	mlib_s32 N, k, off;
	mlib_d64 x_buff[256], r_buff[256];
	mlib_d64 mod_mask = vis_to_double_dup(0xFF << shift);
	mlib_u64 gsr_for_mul64 = (((mlib_u64)0x00224466 << 32) | gsr_for_mul);
	mlib_u64 gsr_for_pack64 = (((mlib_u64)0xF193B5D7 << 32) | gsr_for_pack);

	mlib_u16 uc = ((mlib_u8 *)c)[1] | (((mlib_u8 *)c)[0] << 8);
	mlib_d64 dc = vis_to_double_dup(uc | (uc << 16));

	dc = vis_fpmerge(vis_read_hi(dc), vis_read_lo(dc));
	dc = vis_fpmerge(vis_read_hi(dc), vis_read_lo(dc));
	dc_hi = vis_fpmerge(vis_fzeros(), vis_read_hi(dc));
	dc_lo = vis_fpmerge(vis_fzeros(), vis_read_lo(dc));

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
		len = dpzend - dpz;

		if (len < 0)
			return (MLIB_SUCCESS);

/* initialize GSR */
		vis_write_gsr64(gsr_for_mul64);
		if ((((mlib_addr)x ^ (mlib_addr)z) & 7) == 0) {

#pragma pipeloop(0)
			for (i = 0; i < len; i++) {
				dx = dpx[i];
				EXPAND_U8C_FOR_0_MUL_MOD;
				MUL_U8C_S16C_MOD;
				PACK_S16C_U8C0_MOD;
				dpz[i] = dr;
			}

			dpx += len;
			dpz += len;
			dx = vis_ld_d64_nf(dpx);

		} else {

			for (k = 0; k < (len >> 8); k++) {
				vis_alignaddr((void *)x, off);
				dx0 = vis_ld_d64_nf(dpx + 256 * k);
				dx1 = vis_ld_d64_nf(dpx + 256 * k + 1);
				dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx0 = dx1;
					EXPAND_U8C_FOR_0_MUL_MOD;
					MUL_U8C_S16C_MOD;
					PACK_S16C_U8C0_MOD;
					dx1 = vis_ld_d64_nf(dpx +
						256 * k + i + 2);
					dpz[256 * k + i] = dr;
					dx = vis_faligndata(dx0, dx1);
				}

			}

			N = len & ~255;
			vis_alignaddr((void *)x, off);
			dx0 = vis_ld_d64_nf(dpx + N);
			dx1 = vis_ld_d64_nf(dpx + N + 1);
			dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
			for (i = 0; i < len - N; i++) {
				dx0 = dx1;
				EXPAND_U8C_FOR_0_MUL_MOD;
				MUL_U8C_S16C_MOD;
				PACK_S16C_U8C0_MOD;
				dx1 = vis_ld_d64_nf(dpx + i + N + 2);
				dpz[i + N] = dr;
				dx = vis_faligndata(dx0, dx1);
			}

			dpz = dpzend;
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
/* initialize GSR */
	vis_write_gsr64(gsr_for_pack64);
	PACK_S16C_U8C1_MOD_BSH;
	vis_pst_8(dr1, dpz, emask);

	if ((mlib_addr)++dpz <= (mlib_addr)pzend) {
		vis_pst_8(dr1, dpz, 0x80);
	}

	len = dpzend - dpz;

	if (len < 0)
		return (MLIB_SUCCESS);

	for (k = 0; k < (len >> 8); k++) {
/* initialize GSR */
		vis_write_gsr64(gsr_for_mul64);
		(void *)vis_alignaddr((void *)x, off);
		dx0 = vis_ld_d64_nf(dpx + 256 * k);
		dx1 = vis_ld_d64_nf(dpx + 256 * k + 1);
		dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
		for (i = 0; i < 256; i++) {
			dx0 = dx1;
			EXPAND_U8C_FOR_0_MUL_MOD;
			MUL_U8C_S16C_MOD;
			dx1 = vis_ld_d64_nf(dpx + 256 * k + i + 2);
			x_buff[i] = dr1;
			r_buff[i] = dr;
			dx = vis_faligndata(dx0, dx1);
		}

/* initialize GSR */
		vis_write_gsr64(gsr_for_pack64);
#pragma pipeloop(0)
		for (i = 0; i < 256; i++) {
			dr1 = x_buff[i];
			dr = r_buff[i];
			PACK_S16C_U8C1_MOD_BSH;
			vis_pst_8(dr1, dpz + 256 * k + i, 0x7f);
			vis_pst_8(dr1, dpz + 256 * k + i + 1, 0x80);
		}
	}

	N = len & ~255;
/* initialize GSR */
	vis_write_gsr64(gsr_for_mul64);
	vis_alignaddr((void *)x, off);
	dx0 = vis_ld_d64_nf(dpx + N);
	dx1 = vis_ld_d64_nf(dpx + N + 1);
	dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
	for (i = 0; i < len - N; i++) {
		dx0 = dx1;
		EXPAND_U8C_FOR_0_MUL_MOD;
		MUL_U8C_S16C_MOD;
		dx1 = vis_ld_d64_nf(dpx + i + N + 2);
		x_buff[i] = dr1;
		r_buff[i] = dr;
		dx = vis_faligndata(dx0, dx1);
	}

/* initialize GSR */
	vis_write_gsr64(gsr_for_pack64);
#pragma pipeloop(0)
	for (i = 0; i < len - N; i++) {
		dr1 = x_buff[i];
		dr = r_buff[i];
		PACK_S16C_U8C1_MOD_BSH;
		vis_pst_8(dr1, dpz + i + N, 0x7f);
		vis_pst_8(dr1, dpz + i + N + 1, 0x80);
	}

	dpz = dpzend;

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
/* initialize GSR */
		vis_write_gsr(gsr_for_mul);
		EXPAND_U8C_FOR_MUL_MOD;
		MUL_U8C_S16C_MOD;
/* initialize GSR */
		vis_write_gsr64(gsr_for_pack64);
		PACK_S16C_U8C1_MOD_BSH;
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

mlib_status
__mlib_VectorMulSShift_U8C_Sat(
	mlib_u8 *xz,
	const mlib_u8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_U8C_U8C_Sat(xz, xz, c, n, shift));
}

/* *********************************************************** */

#define	MUL_S16_S8_SAT                                          \
	{                                                       \
	    mlib_d64 dr2, dr3, dr4, dr5;                        \
	                                                        \
	    dr2 = vis_fpmerge(vis_read_hi(dx), fzero);          \
	    dr = vis_fmul8sux16(dr2, dc_hi);                    \
	    dr4 = vis_fpmerge(vis_read_lo(dx), fzero);          \
	    dr1 = vis_fmul8sux16(dr4, dc_lo);                   \
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
	dr = vis_fxor(dr, restore);

/* *********************************************************** */

#define	PACK_S16_S8_MOD                                         \
	dr = vis_fand(dr, without_control);                     \
	dr1 = vis_fand(dr1, without_control);                   \
	dr = vis_fpack16_to_hi(dr, dr);                         \
	dr = vis_fpack16_to_lo(dr, dr1);

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s32 emask, i;
	mlib_s32 off, xoff = (mlib_addr)x & 7;
	mlib_s8 *pzend, *px = (mlib_s8 *)x;
	mlib_d64 *dpz = (mlib_d64 *)z, *dpx = (mlib_d64 *)x, *dpzend;
	mlib_d64 dx, dr, dr1, dx0, dx1;
	mlib_s32 len;
	mlib_f32 fzero = vis_fzeros();
	mlib_d64 restore = vis_to_double_dup(0x80808080);
	mlib_d64 displacement = vis_to_double_dup(0x800080);
	mlib_d64 anti_rounding_mask = vis_to_double_dup(0x10001 << (shift - 1));
	mlib_f32 scale = vis_to_float(0x80808080 >> (shift - 1));

	mlib_u8 uc = ((mlib_u8 *)c)[0];
	mlib_u16 ucc = uc | (uc << 8);
	mlib_d64 dc = vis_to_double_dup(ucc | (ucc << 16));
	mlib_d64 dc_hi = vis_fpmerge(vis_read_hi(dc), fzero);
	mlib_d64 dc_lo = vis_fpmerge(vis_read_lo(dc), fzero);

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);

	if (!((mlib_addr)x & 7) && !((mlib_addr)z & 7)) {

		len = n >> 3;
		dx = vis_ld_d64_nf(dpx);
#pragma pipeloop(0)
		for (i = 0; i < len; i++) {
			MUL_S16_S8_SAT;
			PACK_S16_S8_SAT;
			dx = vis_ld_d64_nf(dpx + i + 1);
			dpz[i] = dr;
		}

		pzend = (mlib_s8 *)z + n - 1;
		dpz += len;

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			MUL_S16_S8_SAT;
			PACK_S16_S8_SAT;
/* prepare edge mask for the last bytes */
			emask = vis_edge8(dpz, pzend);
			vis_pst_8(dr, dpz, emask);
		}
	} else {

		pzend = (mlib_s8 *)z + n - 1;
		dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
		dpz = (mlib_d64 *)((mlib_addr)z & (~7));
		off = (mlib_addr)dpz - (mlib_addr)z;

		emask = vis_edge8(z, pzend);

		dpx = (mlib_d64 *)vis_alignaddr(px, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
		MUL_S16_S8_SAT;
		PACK_S16_S8_SAT;
/* store first bytes of result */
		vis_pst_8(dr, dpz, emask);
		dpx++;
		dpz++;
		len = dpzend - dpz;

		if (len < 0)
			return (MLIB_SUCCESS);

		px += 8 + off;
		off = (mlib_addr)px & 7;
		vis_write_bmask(0x11111111 * off, 0x01234567);

		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_bshuffle(dx0, dx1);
#pragma pipeloop(0)
		for (i = 0; i < len; i++) {
			dx0 = dx1;
			MUL_S16_S8_SAT;
			PACK_S16_S8_SAT;
			dx1 = vis_ld_d64_nf(dpx + i + 2);
			dpz[i] = dr;
			dx = vis_bshuffle(dx0, dx1);
		}

		dpz = dpzend;

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			MUL_S16_S8_SAT;
			PACK_S16_S8_SAT;
/* prepare edge mask for the last bytes */
			emask = vis_edge8(dpz, pzend);
			vis_pst_8(dr, dpz, emask);
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S8_Sat(
	mlib_s8 *xz,
	const mlib_s8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_S8_S8_Sat(xz, xz, c, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S8_S8_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s32 emask, i;
	mlib_s32 off;
	mlib_s8 *pzend, *px = (mlib_s8 *)x;
	mlib_d64 *dpz = (mlib_d64 *)z, *dpx = (mlib_d64 *)x, *dpzend;
	mlib_d64 dx, dr, dr1, dx0, dx1;
	mlib_s32 len;
	mlib_f32 fzero = vis_fzeros();
	mlib_d64 displacement = vis_to_double_dup(0x800080);
	mlib_d64 anti_rounding_mask = vis_to_double_dup(0x10001 << (shift - 1));
	mlib_f32 scale = vis_to_float(0x80808080 >> (shift - 1));
	mlib_d64 without_control = vis_to_double_dup(0xff00ff);

	mlib_u8 uc = ((mlib_u8 *)c)[0];
	mlib_u16 ucc = uc | (uc << 8);
	mlib_d64 dc = vis_to_double_dup(ucc | (ucc << 16));
	mlib_d64 dc_hi = vis_fpmerge(vis_read_hi(dc), fzero);
	mlib_d64 dc_lo = vis_fpmerge(vis_read_lo(dc), fzero);

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);

	if (!((mlib_addr)x & 7) && !((mlib_addr)z & 7)) {

		len = n >> 3;
		dx = vis_ld_d64_nf(dpx);
#pragma pipeloop(0)
		for (i = 0; i < len; i++) {
			MUL_S16_S8_SAT;
			PACK_S16_S8_MOD;
			dx = vis_ld_d64_nf(dpx + i + 1);
			dpz[i] = dr;
		}

		pzend = (mlib_s8 *)z + n - 1;
		dpz += len;

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			MUL_S16_S8_SAT;
			PACK_S16_S8_MOD;
/* prepare edge mask for the last bytes */
			emask = vis_edge8(dpz, pzend);
			vis_pst_8(dr, dpz, emask);
		}

	} else {

		pzend = (mlib_s8 *)z + n - 1;
		dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
		dpz = (mlib_d64 *)((mlib_addr)z & (~7));
		off = (mlib_addr)dpz - (mlib_addr)z;

		emask = vis_edge8(z, pzend);

		dpx = (mlib_d64 *)vis_alignaddr(px, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
		MUL_S16_S8_SAT;
		PACK_S16_S8_MOD;
/* store first bytes of result */
		vis_pst_8(dr, dpz, emask);
		dpx++;
		dpz++;
		len = dpzend - dpz;

		if (len < 0)
			return (MLIB_SUCCESS);

		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
		for (i = 0; i < len; i++) {
			dx0 = dx1;
			MUL_S16_S8_SAT;
			PACK_S16_S8_MOD;
			dx1 = vis_ld_d64_nf(dpx + i + 2);
			dpz[i] = dr;
			dx = vis_faligndata(dx0, dx1);
		}

		dpz = dpzend;

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			MUL_S16_S8_SAT;
			PACK_S16_S8_MOD;
/* prepare edge mask for the last bytes */
			emask = vis_edge8(dpz, pzend);
			vis_pst_8(dr, dpz, emask);
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S8_Mod(
	mlib_s8 *xz,
	const mlib_s8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_S8_S8_Mod(xz, xz, c, n, shift));
}

/* *********************************************************** */

#define	MUL_S8C_S16C                                            \
	{                                                       \
	    mlib_d64 dr2, dr3, dr4;                             \
	                                                        \
	    dr2 = vis_fpack32(dx, dx);                          \
	    dr = vis_fmul8sux16(dx, dc_re);                     \
	    dr1 = vis_fmul8sux16(dr2, dc_re);                   \
	    dr2 = vis_fmul8sux16(dr2, dc_im);                   \
	    dr3 = vis_fmul8sux16(dx, dc_im);                    \
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
	dr = vis_fxnor(dr, restore);

/* *********************************************************** */

#define	PACK_S16C_S8C0_SAT                                      \
	PACK_S16C_S8C_SAT;                                      \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));

/* *********************************************************** */

#define	PACK_S16C_S8C1_SAT                                       \
	PACK_S16C_S8C_SAT;                                       \
	dr1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr));     \
	fr2 = vis_fands(vis_read_hi(dr1), mask);                 \
	dr1 = vis_fpmerge(vis_read_lo(dr1), vis_read_lo(dr1));   \
	fr3 = vis_fandnots(mask, vis_read_hi(dr1));              \
	fr2 = vis_fors(fr3, fr2);                                \
	dr1 = vis_fpmerge(vis_read_lo(dr1), vis_read_lo(dr1));   \
	dr = vis_fpmerge(fr2, vis_read_hi(dr));

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s32 emask, len, i;
	mlib_s32 off;
	mlib_s8 *pzend, *px = (mlib_s8 *)x;
	mlib_d64 *dpz = (mlib_d64 *)z, *dpx = (mlib_d64 *)x, *dpzend;
	mlib_d64 dx, dr, dr1, dx0, dx1;
	mlib_s32 n4 = n >> 2;
	mlib_d64 without_control = vis_to_double_dup(0xff00ff);
	mlib_d64 restore = vis_to_double_dup(0x7f7f7f7f);
	mlib_d64 displacement = vis_to_double_dup(0x800080);
	mlib_d64 anti_rounding_mask = vis_to_double_dup(0x10001 << (shift - 1));
	mlib_f32 scale = vis_to_float(0x80808080 >> (shift - 1));
	mlib_f32 mask = vis_to_float(0xFFFFFF00);
	mlib_f32 fr2, fr3;

	mlib_u16 uc_re = ((mlib_u8 *)c)[0] << 8;
	mlib_u16 uc_im = ((mlib_u8 *)c)[1] << 8;
	mlib_f32 fc_re = vis_to_float(uc_re | (uc_re << 16));
	mlib_f32 fc_im = vis_to_float(uc_im | (uc_im << 16));
	mlib_d64 dc_re = vis_freg_pair(fc_re, fc_re);
	mlib_d64 dc_im = vis_freg_pair(fc_im, fc_im);

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

	vis_write_gsr(7 << 3);

	if (!(1 & (mlib_addr)z)) {

		if (!((mlib_addr)x & 7) && !((mlib_addr)z & 7)) {

			dx = vis_ld_d64_nf(dpx);
#pragma pipeloop(0)
			for (i = 0; i < n4; i++) {
				MUL_S8C_S16C;
				PACK_S16C_S8C0_SAT;
				dx = vis_ld_d64_nf(dpx + i + 1);
				dpz[i] = dr;
			}

			dpx += n4;
			dpz += n4;

			pzend = (mlib_s8 *)z + n + n - 1;
			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				dx = vis_ld_d64_nf(dpx);
				MUL_S8C_S16C;
				PACK_S16C_S8C0_SAT;
				emask = vis_edge8(dpz, pzend);
				vis_pst_8(dr, dpz, emask);
			}

		} else {

			pzend = (mlib_s8 *)z + n + n - 1;
			dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
			dpz = (mlib_d64 *)((mlib_addr)z & (~7));
			off = (mlib_addr)dpz - (mlib_addr)z;

			emask = vis_edge8(z, pzend);

			dpx = (mlib_d64 *)vis_alignaddr(px, off);
			dx0 = vis_ld_d64_nf(dpx);
			dx1 = vis_ld_d64_nf(dpx + 1);
			dx = vis_faligndata(dx0, dx1);
			MUL_S8C_S16C;
			PACK_S16C_S8C0_SAT;
			vis_pst_8(dr, dpz, emask);

			dpx++;
			dpz++;
			len = dpzend - dpz;

			if (len < 0)
				return (MLIB_SUCCESS);

			dx0 = vis_ld_d64_nf(dpx);
			dx1 = vis_ld_d64_nf(dpx + 1);
			dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
			for (i = 0; i < len; i++) {
				dx0 = dx1;
				MUL_S8C_S16C;
				PACK_S16C_S8C0_SAT;
				dx1 = vis_ld_d64_nf(dpx + i + 2);
				dpz[i] = dr;
				dx = vis_faligndata(dx0, dx1);
			}

			dpz = dpzend;

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				MUL_S8C_S16C;
				PACK_S16C_S8C0_SAT;
				emask = vis_edge8(dpz, pzend);
				vis_pst_8(dr, dpz, emask);
			}
		}
		return (MLIB_SUCCESS);
	}

	pzend = (mlib_s8 *)z + n + n - 1;
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z + 1;

	emask = vis_edge8(z, pzend);

	dpx = (mlib_d64 *)vis_alignaddr(px, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_faligndata(dx0, dx1);
	MUL_S8C_S16C;
	PACK_S16C_S8C1_SAT;
	vis_pst_8(dr, dpz, emask);

	dpx++;
	dpz++;

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
		vis_pst_8(dr1, dpz, 0x80);
	}

	len = dpzend - dpz;

	if (len < 0)
		return (MLIB_SUCCESS);

	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
	for (i = 0; i < len; i++) {
		dx0 = dx1;
		MUL_S8C_S16C;
		PACK_S16C_S8C1_SAT;
		dx1 = vis_ld_d64_nf(dpx + i + 2);
		vis_pst_8(dr, dpz + i, 0x7f);
		vis_pst_8(dr1, dpz + i + 1, 0x80);
		dx = vis_faligndata(dx0, dx1);
	}

	dpz = dpzend;

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
		MUL_S8C_S16C;
		vis_alignaddr(dpx, 7);
		PACK_S16C_S8C1_SAT;
		emask = vis_edge8(dpz, pzend);
		vis_pst_8(dr, dpz, emask & 0x7f);

		if ((mlib_addr)++dpz <= (mlib_addr)pzend) {
			vis_pst_8(dr1, dpz, 0x80);
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S8C_Sat(
	mlib_s8 *xz,
	const mlib_s8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_S8C_S8C_Sat(xz, xz, c, n, shift));
}

/* *********************************************************** */

#define	PACK_S16C_S8C0_MOD                                      \
	dr = vis_fpack32(dr, dr);                               \
	dr = vis_fandnot(without_control, dr);                  \
	dr1 = vis_fand(dr1, without_control);                   \
	dr = vis_for(dr, dr1);

/* *********************************************************** */

#define	PACK_S16C_S8C1_MOD                                      \
	{                                                       \
	    mlib_d64 dr2, dr3, tmp;                             \
	                                                        \
	    dr1 = vis_faligndata(dr1, dr1);                     \
	    dr3 = vis_fand(dr, without_control);                \
	    dr2 = vis_fandnot(without_control, dr1);            \
	    dr = vis_for(dr3, dr2);                             \
	}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S8C_S8C_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s32 emask, len, i, k, N;
	mlib_s32 off, offx;
	mlib_s8 *pzend, *px = (void *)x;
	mlib_d64 *dpz = (mlib_d64 *)z, *dpx = (mlib_d64 *)x,
		*dpx1 =	(mlib_d64 *)x, *dpzend;
	mlib_d64 dx, dr, dr1, dx0, dx1;
	mlib_d64 dr_buff[256], dr1_buff[256];
	mlib_d64 without_control = vis_to_double_dup(0xff00ff);
	mlib_d64 anti_rounding_mask = vis_to_double_dup(0x10001 << (shift - 1));
	mlib_d64 dzero = vis_fzero();
	mlib_f32 scale = vis_to_float(0x80808080 >> (shift - 1));
	mlib_f32 fr2, fr3;

	mlib_u16 uc_re = ((mlib_u8 *)c)[0] << 8;
	mlib_u16 uc_im = ((mlib_u8 *)c)[1] << 8;
	mlib_f32 fc_re = vis_to_float(uc_re | (uc_re << 16));
	mlib_f32 fc_im = vis_to_float(uc_im | (uc_im << 16));
	mlib_d64 dc_re = vis_freg_pair(fc_re, fc_re);
	mlib_d64 dc_im = vis_freg_pair(fc_im, fc_im);

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

	vis_write_gsr(7 << 3);

	if (!(1 & (mlib_addr)z)) {

		pzend = (mlib_s8 *)z + n + n - 1;
		dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
		dpz = (mlib_d64 *)((mlib_addr)z & (~7));
		off = (mlib_addr)dpz - (mlib_addr)z;

		emask = vis_edge8(z, pzend);

		dpx = (mlib_d64 *)vis_alignaddr(px, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
		MUL_S8C_S16C;
		PACK_S16C_S8C0_MOD;
		vis_pst_8(dr, dpz, emask);

		dpx++;
		dpz++;
		len = dpzend - dpz;

		if (len < 0)
			return (MLIB_SUCCESS);

		if (!((mlib_addr)x & 7) && !((mlib_addr)z & 7)) {

			dx = vis_ld_d64_nf(dpx);
#pragma pipeloop(0)
			for (i = 0; i < len; i++) {
				MUL_S8C_S16C;
				PACK_S16C_S8C0_MOD;
				dx = vis_ld_d64_nf(dpx + i + 1);
				dpz[i] = dr;
			}

		} else {

			offx = (mlib_addr)((mlib_u8 *)x + off) & 7;
			vis_write_bmask(0x11111111 * offx, 0x01234567);

			dx0 = vis_ld_d64_nf(dpx);
			dx1 = vis_ld_d64_nf(dpx + 1);
			dx = vis_bshuffle(dx0, dx1);
#pragma pipeloop(0)
			for (i = 0; i < len; i++) {
				dx0 = dx1;
				MUL_S8C_S16C;
				PACK_S16C_S8C0_MOD;
				dx1 = vis_ld_d64_nf(dpx + i + 2);
				dpz[i] = dr;
				dx = vis_bshuffle(dx0, dx1);
			}

		}

		if ((mlib_addr)dpzend <= (mlib_addr)pzend) {
			MUL_S8C_S16C;
			PACK_S16C_S8C0_MOD;
			emask = vis_edge8(dpzend, pzend);
			vis_pst_8(dr, dpzend, emask);
		}
		return (MLIB_SUCCESS);
	}

	pzend = (mlib_s8 *)z + n + n - 1;
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z + 1;
	offx = (mlib_addr)((mlib_u8 *)x + off) & 7;
	vis_write_bmask(0x11111111 * offx, 0x01234567);

	emask = vis_edge8(z, pzend);

	dpx = (mlib_d64 *)vis_alignaddr(px, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_faligndata(dx0, dx1);
	MUL_S8C_S16C;
	vis_alignaddr(dpx, 7);
	PACK_S16C_S8C1_MOD;
	vis_pst_8(dr, dpz, emask);

	dpx++;
	dpz++;

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
		vis_pst_8(dr1, dpz, 0x80);
	}

	len = dpzend - dpz;

	if (len < 0)
		return (MLIB_SUCCESS);

	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_bshuffle(dx0, dx1);
#pragma pipeloop(0)
	for (i = 0; i < len; i++) {
		dx0 = dx1;
		MUL_S8C_S16C;
		dx1 = vis_ld_d64_nf(dpx + i + 2);
		PACK_S16C_S8C1_MOD;
		vis_pst_8(dr, dpz + i, 0x7f);
		vis_pst_8(dr1, dpz + i + 1, 0x80);
		dx = vis_bshuffle(dx0, dx1);
	}

	dpz = dpzend;

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
		MUL_S8C_S16C;
		PACK_S16C_S8C1_MOD;
		emask = vis_edge8(dpz, pzend);
		vis_pst_8(dr, dpz, emask & 0x7f);

		if ((mlib_addr)++dpz <= (mlib_addr)pzend) {
			vis_pst_8(dr1, dpz, 0x80);
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S8C_Mod(
	mlib_s8 *xz,
	const mlib_s8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_S8C_S8C_Mod(xz, xz, c, n, shift));
}

/* *********************************************************** */

#define	MUL_S16_S32                                             \
	{                                                       \
	    mlib_d64 dr2, dr3;                                  \
	                                                        \
	    dr = vis_fmuld8sux16(vis_read_hi(dx), c_dir);       \
	    dr1 = vis_fmuld8ulx16(vis_read_hi(dx), c_dir);      \
	    dr = vis_fpadd32(dr, dr1);                          \
	    dr2 = vis_fmuld8sux16(vis_read_lo(dx), c_dir);      \
	    dr3 = vis_fmuld8ulx16(vis_read_lo(dx), c_dir);      \
	    dr1 = vis_fpadd32(dr2, dr3);                        \
	}

/* *********************************************************** */

#define	PACK_S32_S16_STORE_SAT                                  \
	dr = vis_fpackfix_pair(dr, dr1);                        \
	vis_pst_16(dr, dpz, emask);

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_f32 c_dir = vis_to_float((c[0] << 16) | (c[0] & 0xFFFF));
	mlib_d64 *dpz = (mlib_d64 *)z, *dpx = (mlib_d64 *)x, *dpzend;
	mlib_d64 dx, dx0, dx1, dr, dr1;
	mlib_s16 *pzend;
	mlib_s32 i, len, valc = c[0], emask;

	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

	vis_write_gsr((16 - shift) << 3);

	if (!((mlib_addr)z & 7) && !((mlib_addr)x & 7)) {

		len = n >> 2;
		dx = vis_ld_d64_nf(dpx);
#pragma pipeloop(0)
		for (i = 0; i < len; i++) {
			MUL_S16_S32;
			dx = vis_ld_d64_nf(dpx + i + 1);
			dpz[i] = vis_fpackfix_pair(dr, dr1);
		}

		pzend = (mlib_s16 *)z + n - 1;
		dpz += len;

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			MUL_S16_S32;
/* prepare edge mask for the last bytes */
			emask = vis_edge16(dpz, pzend);
			PACK_S32_S16_STORE_SAT;
			vis_pst_16(dr, dpz, emask);
		}

	} else {
		mlib_s32 emask, off, offx;

		pzend = (mlib_s16 *)z + n - 1;
		dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
		dpz = (mlib_d64 *)((mlib_addr)z & (~7));
		off = (mlib_addr)dpz - (mlib_addr)z;
		offx = (mlib_addr)((mlib_u8 *)x + off) & 7;
		vis_write_bmask(0x11111111 * offx, 0x01234567);

		emask = vis_edge16(z, pzend);

		dpx = vis_alignaddr((void *)x, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
		MUL_S16_S32;
		PACK_S32_S16_STORE_SAT;

		dpx++;
		dpz++;

		len = dpzend - dpz;
		if (len < 0)
			return (MLIB_SUCCESS);

		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_bshuffle(dx0, dx1);
#pragma pipeloop(0)
		for (i = 0; i < len; i++) {
			dx0 = dx1;
			MUL_S16_S32;
			dx1 = vis_ld_d64_nf(dpx + i + 2);
			dpz[i] = vis_fpackfix_pair(dr, dr1);
			dx = vis_bshuffle(dx0, dx1);
		}

		dpz = dpzend;

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			MUL_S16_S32;
/* prepare edge mask for the last bytes */
			emask = vis_edge16(dpz, pzend);
			PACK_S32_S16_STORE_SAT;
			vis_pst_16(dr, dpz, emask);
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S16_Sat(
	mlib_s16 *xz,
	const mlib_s16 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_S16_S16_Sat(xz, xz, c, n, shift));
}

/* *********************************************************** */

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

mlib_status
__mlib_VectorMulSShift_S16_Mod(
	mlib_s16 *xz,
	const mlib_s16 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_S16_S16_Mod(xz, xz, c, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s32 off, offx, emask, len, i;
	mlib_f32 c_dir = vis_to_float((c[0] << 16) | (c[0] & 0xFFFF));
	mlib_d64 *dpz = (mlib_d64 *)z, *dpx = (mlib_d64 *)x, *dpzend;
	mlib_d64 dx, dx0, dx1, dr, dr1;
	mlib_d64 mask_clear =
		vis_to_double_dup(((mlib_u32)0xffffffff >> (16 - shift + 1)));
	mlib_d64 mask_sign = vis_to_double_dup((mlib_u32)0x00008000 << shift);
	mlib_d64 mask_real_sign = vis_to_double_dup(0x80008000);
	mlib_s16 *pzend;
	mlib_s16 valc = c[0];

	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

	vis_write_gsr((16 - shift) << 3);

	if (shift == 16) {

		if (!((mlib_addr)x & 7) && !((mlib_addr)z & 7)) {

			len = n >> 2;
			dx = vis_ld_d64_nf(dpx);
#pragma pipeloop(0)
			for (i = 0; i < len; i++) {
				MUL_S16_S32;
				dx = vis_ld_d64_nf(dpx + i + 1);
				dpz[i] = vis_fpackfix_pair(dr, dr1);
			}

			pzend = (mlib_s16 *)z + n - 1;
			dpz += len;

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				MUL_S16_S32;
/* prepare edge mask for the last bytes */
				emask = vis_edge16(dpz, pzend);
				PACK_S32_S16_STORE_SAT;
				vis_pst_16(dr, dpz, emask);
			}

		} else {

			pzend = (mlib_s16 *)z + n - 1;
			dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
			dpz = (mlib_d64 *)((mlib_addr)z & (~7));
			off = (mlib_addr)dpz - (mlib_addr)z;
			offx = (mlib_addr)((mlib_u8 *)x + off) & 7;
			vis_write_bmask(0x11111111 * offx, 0x01234567);

			emask = vis_edge16(z, pzend);

			dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
			dx0 = vis_ld_d64_nf(dpx);
			dx1 = vis_ld_d64_nf(dpx + 1);
			dx = vis_faligndata(dx0, dx1);

			MUL_S16_S32;
			PACK_S32_S16_STORE_SAT;
			dpx++;
			dpz++;

			len = dpzend - dpz;
			if (len < 0)
				return (MLIB_SUCCESS);

			dx0 = vis_ld_d64_nf(dpx);
			dx1 = vis_ld_d64_nf(dpx + 1);
			dx = vis_bshuffle(dx0, dx1);
#pragma pipeloop(0)
			for (i = 0; i < len; i++) {
				dx0 = dx1;
				MUL_S16_S32;
				dx1 = vis_ld_d64_nf(dpx + i + 2);
				dpz[i] = vis_fpackfix_pair(dr, dr1);
				dx = vis_bshuffle(dx0, dx1);
			}

			dpz = dpzend;

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				MUL_S16_S32;
/* prepare edge mask for the last bytes */
				emask = vis_edge16(dpz, pzend);
				PACK_S32_S16_STORE_SAT;
				vis_pst_16(dr, dpz, emask);
			}
		}
	} else {

		if (!((mlib_addr)x & 7) && !((mlib_addr)z & 7)) {

			len = n >> 2;
			dx = vis_ld_d64_nf(dpx);
#pragma pipeloop(0)
			for (i = 0; i < len; i++) {
				MUL_S16_S32;
				PACK_S32_S16_MOD;
				dx = vis_ld_d64_nf(dpx + i + 1);
				dpz[i] = dr;
			}

			pzend = (mlib_s16 *)z + n - 1;
			dpz += len;

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				MUL_S16_S32;
/* prepare edge mask for the last bytes */
				emask = vis_edge16(dpz, pzend);
				PACK_S32_S16_MOD;
				vis_pst_16(dr, dpz, emask);
			}

		} else {

			pzend = (mlib_s16 *)z + n - 1;
			dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
			dpz = (mlib_d64 *)((mlib_addr)z & (~7));
			off = (mlib_addr)dpz - (mlib_addr)z;
			offx = (mlib_addr)((mlib_u8 *)x + off) & 7;
			vis_write_bmask(0x11111111 * offx, 0x01234567);

			emask = vis_edge16(z, pzend);

			dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
			dx0 = vis_ld_d64_nf(dpx);
			dx1 = vis_ld_d64_nf(dpx + 1);
			dx = vis_faligndata(dx0, dx1);

			MUL_S16_S32;
			PACK_S32_S16_MOD;
			vis_pst_16(dr, dpz, emask);

			dpx++;
			dpz++;
			len = dpzend - dpz;

			if (len < 0)
				return (MLIB_SUCCESS);

			dx0 = vis_ld_d64_nf(dpx);
			dx1 = vis_ld_d64_nf(dpx + 1);
			dx = vis_bshuffle(dx0, dx1);
#pragma pipeloop(0)
			for (i = 0; i < len; i++) {
				dx0 = dx1;
				MUL_S16_S32;
				PACK_S32_S16_MOD;
				dx1 = vis_ld_d64_nf(dpx + i + 2);
				dpz[i] = dr;
				dx = vis_bshuffle(dx0, dx1);
			}
			dpz = dpzend;

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				MUL_S16_S32;
/* prepare edge mask for the last bytes */
				emask = vis_edge16(dpz, pzend);
				PACK_S32_S16_MOD;
				vis_pst_16(dr, dpz, emask);
			}
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S16C_S32C                                                    \
	{                                                                \
	    dr2 = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dc_1));   \
	    dr3 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dc_1));   \
	    dr2 = vis_fpadd32(dr2, dr3);                                 \
	    sr0 = vis_fpsub32s(vis_read_hi(dr2), vis_read_lo(dr2));      \
	    dr4 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dc_1));   \
	    dr5 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dc_1));   \
	    dr4 = vis_fpadd32(dr4, dr5);                                 \
	    sr1 = vis_fpsub32s(vis_read_hi(dr4), vis_read_lo(dr4));      \
	    dr7 = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dc_2));   \
	    dr8 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dc_2));   \
	    dr7 = vis_fpadd32(dr7, dr8);                                 \
	    sr2 = vis_fpadd32s(vis_read_hi(dr7), vis_read_lo(dr7));      \
	    dr9 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dc_2));   \
	    dr6 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dc_2));   \
	    dr6 = vis_fpadd32(dr9, dr6);                                 \
	    sr3 = vis_fpadd32s(vis_read_hi(dr6), vis_read_lo(dr6));      \
	}

/* *********************************************************** */

#define	PAIR_S32C_S16C0_MOD                                     \
	{                                                       \
	    dr = vis_freg_pair(sr0, sr2);                       \
	    dr1 = vis_freg_pair(sr1, sr3);                      \
	}

/* *********************************************************** */

#define	PAIR_S32C_S16C1_MOD                                     \
	{                                                       \
	    dr = vis_freg_pair(sr3, sr0);                       \
	    dr1 = vis_freg_pair(sr2, sr1);                      \
	}

/* *********************************************************** */

#define	PACK_S32C_S16C_SHIFT16_MOD	dr = vis_fpackfix_pair(dr, dr1);

/* *********************************************************** */

#define	PACK_S32C_S16C_SHIFT_MOD                                \
	sign = vis_fand(mask_sign, dr);                         \
	dr = vis_fand(mask_clear, dr);                          \
	sign1 = vis_fand(mask_sign, dr1);                       \
	dr1 = vis_fand(mask_clear, dr1);                        \
	dr = vis_fpackfix_pair(dr, dr1);                        \
	sign = vis_fpack16_pair(sign, sign1);                   \
	sign = vis_fand(sign, mask_real_sign);                  \
	dr = vis_for(sign, dr);

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off, offx;
	mlib_s16 *pzend, *px = (void *)x;
	mlib_f32 sr0, sr1, sr2, sr3;
	mlib_d64 *dpz, *dpx, *dpzend;
	mlib_d64 dx, dr, dr1, dx0, dx1;
	mlib_d64 mask_clear;
	mlib_d64 mask_sign;
	mlib_d64 mask_real_sign = vis_to_double_dup(0x80008000);
	mlib_d64 dr2, dr3, dr4, dr5, dr6, dr7, dr8, dr9;
	mlib_d64 sign, sign1;

	mlib_s32 i, k, N;
	mlib_s32 end;
	mlib_d64 x_buff[256];
	mlib_s32 smul_buff[1024], s0, s1, s2, s3, _shift = 16 - shift,
		*spz, old_s;
	mlib_d64 *dmul_buff = (mlib_d64 *)smul_buff;
	mlib_d64 dc_1 =	vis_to_double_dup((((mlib_u16 *)c)[0] << 16) |
		((mlib_u16 *)c)[1]);
	mlib_d64 dc_2 = vis_fpmerge(vis_read_hi(dc_1), vis_read_lo(dc_1));

	dc_2 = vis_fpmerge(vis_read_lo(dc_2), vis_read_hi(dc_2));
	dc_2 = vis_fpmerge(vis_read_hi(dc_2), vis_read_lo(dc_2));

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
	offx = (mlib_addr)((mlib_u8 *)x + off) & 7;
	vis_write_bmask(0x11111111 * offx, 0x01234567);

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
	MUL_S16C_S32C;

	if (shift == 16) {

		if (!(2 & (mlib_addr)z)) {
			PAIR_S32C_S16C0_MOD;
			PACK_S32C_S16C_SHIFT16_MOD;
			vis_pst_16(dr, dpz, emask);
			dpz++;
			dpx++;
			end = dpzend - dpz;

			if (end < 0)
				return (MLIB_SUCCESS);

			if (!((mlib_addr)x & 7) && !((mlib_addr)z & 7)) {

				dx = vis_ld_d64_nf(dpx);
#pragma pipeloop(0)
				for (i = 0; i < end; i++) {
					MUL_S16C_S32C;
					PAIR_S32C_S16C0_MOD;
					PACK_S32C_S16C_SHIFT16_MOD;
					dx = vis_ld_d64_nf(dpx + i + 1);
					dpz[i] = dr;
				}
				dpz = dpzend;

			} else {

				dx0 = vis_ld_d64_nf(dpx);
				dx1 = vis_ld_d64_nf(dpx + 1);
				dx = vis_bshuffle(dx0, dx1);
#pragma pipeloop(0)
				for (i = 0; i < end; i++) {
					dx0 = dx1;
					MUL_S16C_S32C;
					PAIR_S32C_S16C0_MOD;
					PACK_S32C_S16C_SHIFT16_MOD;
					dx1 = vis_ld_d64_nf(dpx + i + 2);
					dpz[i] = dr;
					dx = vis_bshuffle(dx0, dx1);
				}
				dpz = dpzend;
			}

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				MUL_S16C_S32C;
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

		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_bshuffle(dx0, dx1);
#pragma pipeloop(0)
		for (i = 0; i < end; i++) {
			dx0 = dx1;
			MUL_S16C_S32C;
			PAIR_S32C_S16C1_MOD;
			PACK_S32C_S16C_SHIFT16_MOD;
			dx1 = vis_ld_d64_nf(dpx + i + 2);
			vis_pst_16(dr, dpz + i, 0x7);
			vis_pst_16(dr, dpz + i + 1, 0x8);
			dx = vis_bshuffle(dx0, dx1);
		}

		if ((mlib_addr)dpzend <= (mlib_addr)pzend) {
			MUL_S16C_S32C;
			PAIR_S32C_S16C1_MOD;
			PACK_S32C_S16C_SHIFT16_MOD;
/* prepare edge mask for the last bytes */
			emask = vis_edge16(dpzend, pzend);
/* store last bytes of result */
			vis_pst_16(dr, dpzend, emask & 0x7);
		}

		return (MLIB_SUCCESS);
	}

	else {
/* SHIFT <  16 */

		if (!(2 & (mlib_addr)z)) {
			PAIR_S32C_S16C0_MOD;
			PACK_S32C_S16C_SHIFT_MOD;
			vis_pst_16(dr, dpz, emask);
			dpz++;
			dpx++;
			end = dpzend - dpz;

			if (end < 0)
				return (MLIB_SUCCESS);

			if (!((mlib_addr)x & 7) && !((mlib_addr)z & 7)) {
				dx = vis_ld_d64_nf(dpx);
#pragma pipeloop(0)
				for (i = 0; i < end; i++) {
					MUL_S16C_S32C;
					PAIR_S32C_S16C0_MOD;
					PACK_S32C_S16C_SHIFT_MOD;
					dx = vis_ld_d64_nf(dpx + i + 1);
					dpz[i] = dr;
				}
				dpz = dpzend;

			} else {

				(void *)vis_alignaddr((void *)x, off);
				dx0 = vis_ld_d64_nf(dpx);
				dx1 = vis_ld_d64_nf(dpx + 1);
				dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
				for (i = 0; i < end; i++) {
					dx0 = dx1;
					MUL_S16C_S32C;
					PAIR_S32C_S16C0_MOD;
					PACK_S32C_S16C_SHIFT_MOD;
					dx1 = vis_ld_d64_nf(dpx + i + 2);
					dpz[i] = dr;
					dx = vis_faligndata(dx0, dx1);
				}
				dpz = dpzend;
			}

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				MUL_S16C_S32C;
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

		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_bshuffle(dx0, dx1);
#pragma pipeloop(0)
		for (i = 0; i < end; i++) {
			dx0 = dx1;
			MUL_S16C_S32C;
			PAIR_S32C_S16C1_MOD;
			PACK_S32C_S16C_SHIFT_MOD;
			dx1 = vis_ld_d64_nf(dpx + i + 2);
			vis_pst_16(dr, dpz + i, 0x7);
			vis_pst_16(dr, dpz + i + 1, 0x8);
			dx = vis_bshuffle(dx0, dx1);
		}

		if ((mlib_addr)dpzend <= (mlib_addr)pzend) {
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

mlib_status
__mlib_VectorMulSShift_S16C_Mod(
	mlib_s16 *xz,
	const mlib_s16 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_S16C_S16C_Mod(xz, xz, c, n, shift));
}

/* *********************************************************** */

#define	OVER_MUL_S16C_SAT                                               \
	mask_over = (vis_fcmpeq32(dr, d_min32) << 2) |                  \
		(vis_fcmpeq32(dr1, d_min32));                           \
	dr = vis_freg_pair(vis_fpackfix(dr), vis_fpackfix(dr1));

#define	PACK_S32C_S16C0_SAT                                     \
	dr = vis_freg_pair(sr0, sr2);                           \
	dr1 = vis_freg_pair(sr1, sr3);                          \
	OVER_MUL_S16C_SAT;

#define	PACK_S32C_S16C1_SAT                                     \
	dr = vis_freg_pair(sr3, sr0);                           \
	dr1 = vis_freg_pair(sr2, sr1);                          \
	OVER_MUL_S16C_SAT;

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off, offx;
	mlib_s16 *pzend, *px = (void *)x;
	mlib_f32 sr0, sr1, sr2, sr3;
	mlib_d64 *dpz, *dpx, *dpzend;
	mlib_d64 dx, dr, dr1, dx0, dx1;
	mlib_d64 dr2, dr3, dr4, dr5, dr6, dr7, dr8, dr9;

	mlib_d64 d_over = vis_to_double_dup(0x7fff7fff);
	mlib_d64 d_min32 = vis_to_double_dup(0x80000000);
	mlib_s32 i, mask_over, k, N;
	mlib_s32 end;
	mlib_d64 x_buff[256];

	mlib_d64 dc_1 =
		vis_to_double_dup((((mlib_u16 *)c)[0] << 16) | ((mlib_u16 *)
		c)[1]);
	mlib_d64 dc_2 = vis_fpmerge(vis_read_hi(dc_1), vis_read_lo(dc_1));

	dc_2 = vis_fpmerge(vis_read_lo(dc_2), vis_read_hi(dc_2));
	dc_2 = vis_fpmerge(vis_read_hi(dc_2), vis_read_lo(dc_2));

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
	offx = (mlib_addr)((mlib_u8 *)x + off) & 7;
	vis_write_bmask(0x11111111 * offx, 0x01234567);

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
	MUL_S16C_S32C;

	if (!(2 & (mlib_addr)z)) {
		PACK_S32C_S16C0_SAT;
		vis_pst_16(dr, dpz, emask);
		vis_pst_16(d_over, dpz, mask_over & emask);
		dpz++;
		dpx++;
		end = dpzend - dpz;

		if (end < 0)
			return (MLIB_SUCCESS);

		if (!((mlib_addr)x & 7) && !((mlib_addr)z & 7)) {

#pragma pipeloop(0)
#pragma unroll(8)
			for (i = 0; i < end; i++) {
				dx = dpx[i];
				MUL_S16C_S32C;
				PACK_S32C_S16C0_SAT;
				vis_pst_16(d_over, dpz + i, mask_over);
				vis_pst_16(dr, dpz + i, ~mask_over);
			}

			dx = vis_ld_d64_nf(dpx + end);

		} else {

			dx0 = vis_ld_d64_nf(dpx);
			dx1 = vis_ld_d64_nf(dpx + 1);
			dx = vis_bshuffle(dx0, dx1);
#pragma pipeloop(0)
			for (i = 0; i < end; i++) {
				dx0 = dx1;
				MUL_S16C_S32C;
				PACK_S32C_S16C0_SAT;
				dx1 = vis_ld_d64_nf(dpx + i + 2);
				vis_pst_16(d_over, dpz + i, mask_over);
				vis_pst_16(dr, dpz + i, ~mask_over);
				dx = vis_bshuffle(dx0, dx1);
			}
		}

		dpz = dpzend;

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			MUL_S16C_S32C;
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

	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_bshuffle(dx0, dx1);
#pragma pipeloop(0)
	for (i = 0; i < end; i++) {
		dx0 = dx1;
		MUL_S16C_S32C;
		PACK_S32C_S16C1_SAT;
		dx1 = vis_ld_d64_nf(dpx + i + 2);
		vis_pst_16(d_over, dpz + i, mask_over & 0x7);
		vis_pst_16(dr, dpz + i, ~mask_over & 0x7);
		vis_pst_16(d_over, dpz + i + 1, mask_over & 0x8);
		vis_pst_16(dr, dpz + i + 1, ~mask_over & 0x8);
		dx = vis_bshuffle(dx0, dx1);
	}

	if ((mlib_addr)dpzend <= (mlib_addr)pzend) {
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

mlib_status
__mlib_VectorMulSShift_S16C_Sat(
	mlib_s16 *xz,
	const mlib_s16 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_S16C_S16C_Sat(xz, xz, c, n, shift));
}

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(X)	((mlib_s32)(X))

#else

#define	FLOAT2INT_CLAMP(X)                                       \
	(((X) >= MLIB_S32_MAX) ? MLIB_S32_MAX : (((X) <          \
		MLIB_S32_MIN) ? MLIB_S32_MIN : (mlib_s32)(X)))

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S32_S32_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n,
	mlib_s32 shift)
/*  Note: performance mistake here is 1  */
{
	mlib_d64 scalar = *c;
	mlib_d64 sh = (mlib_d64)(1.0 / (1u << shift));
	mlib_d64 mul0, mul1, mul2, mul3, mul4, mul5, mul6, mul7;
	mlib_s32 i;

	if ((shift < 1) || (shift > 31))
		return (MLIB_OUTOFRANGE);

#pragma pipeloop(0)
	for (i = 0; i < n - 7; i += 8) {
		mul0 = x[i] * scalar * sh;
		mul1 = x[i + 1] * scalar * sh;
		mul2 = x[i + 2] * scalar * sh;
		mul3 = x[i + 3] * scalar * sh;
		mul4 = x[i + 4] * scalar * sh;
		mul5 = x[i + 5] * scalar * sh;
		mul6 = x[i + 6] * scalar * sh;
		mul7 = x[i + 7] * scalar * sh;
		z[i] = FLOAT2INT_CLAMP(mul0);
		z[i + 1] = FLOAT2INT_CLAMP(mul1);
		z[i + 2] = FLOAT2INT_CLAMP(mul2);
		z[i + 3] = FLOAT2INT_CLAMP(mul3);
		z[i + 4] = FLOAT2INT_CLAMP(mul4);
		z[i + 5] = FLOAT2INT_CLAMP(mul5);
		z[i + 6] = FLOAT2INT_CLAMP(mul6);
		z[i + 7] = FLOAT2INT_CLAMP(mul7);
	}

	for (; i < n; i++) {
		mul0 = x[i] * scalar * sh;
		z[i] = FLOAT2INT_CLAMP(mul0);
	}
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S32_S32_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s32 scal = *c;
	mlib_s32 i;
	mlib_d64 scal_lo = scal & 0xFFFF;
	mlib_d64 scal_hi = (scal & ~0xFFFF) >> 16;
	mlib_d64 valx;
	mlib_d64 mullo, mulhi;

	if ((shift < 1) || (shift > 31))
		return (MLIB_OUTOFRANGE);

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		valx = x[i];
		mulhi = valx * scal_hi;
		mullo = valx * scal_lo;
		z[i] = (((mlib_s64)mulhi << 16) + (mlib_s64)mullo) >> shift;
	}
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S32_Sat(
	mlib_s32 *xz,
	const mlib_s32 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_S32_S32_Sat(xz, xz, c, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S32_Mod(
	mlib_s32 *xz,
	const mlib_s32 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_S32_S32_Mod(xz, xz, c, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S32C_S32C_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n,
	mlib_s32 shift)
/*  Note: performance mistake here is 1  */
{
	mlib_s32 c_re = c[0];
	mlib_s32 c_im = c[1];
	mlib_d64 sh = 1.0 / (1u << shift);
	mlib_d64 x_r0, x_i0, mul_re0, mul_im0;
	mlib_d64 x_r1, x_i1, mul_re1, mul_im1;
	mlib_d64 x_r2, x_i2, mul_re2, mul_im2;
	mlib_d64 x_r3, x_i3, mul_re3, mul_im3;
	mlib_s32 i;

	if ((shift < 1) || (shift > 31))
		return (MLIB_OUTOFRANGE);

#pragma pipeloop(0)
	for (i = 0; i < n - 3; i += 4) {
		x_r0 = x[2 * i];
		x_i0 = x[2 * i + 1];
		x_r1 = x[2 * i + 2];
		x_i1 = x[2 * i + 3];
		x_r2 = x[2 * i + 4];
		x_i2 = x[2 * i + 5];
		x_r3 = x[2 * i + 6];
		x_i3 = x[2 * i + 7];

		mul_re0 = (x_r0 * c_re - x_i0 * c_im) * sh;
		mul_im0 = (x_r0 * c_im + x_i0 * c_re) * sh;
		mul_re1 = (x_r1 * c_re - x_i1 * c_im) * sh;
		mul_im1 = (x_r1 * c_im + x_i1 * c_re) * sh;
		mul_re2 = (x_r2 * c_re - x_i2 * c_im) * sh;
		mul_im2 = (x_r2 * c_im + x_i2 * c_re) * sh;
		mul_re3 = (x_r3 * c_re - x_i3 * c_im) * sh;
		mul_im3 = (x_r3 * c_im + x_i3 * c_re) * sh;

		z[2 * i] = FLOAT2INT_CLAMP(mul_re0);
		z[2 * i + 1] = FLOAT2INT_CLAMP(mul_im0);
		z[2 * i + 2] = FLOAT2INT_CLAMP(mul_re1);
		z[2 * i + 3] = FLOAT2INT_CLAMP(mul_im1);
		z[2 * i + 4] = FLOAT2INT_CLAMP(mul_re2);
		z[2 * i + 5] = FLOAT2INT_CLAMP(mul_im2);
		z[2 * i + 6] = FLOAT2INT_CLAMP(mul_re3);
		z[2 * i + 7] = FLOAT2INT_CLAMP(mul_im3);
	}

	for (; i < n; i++) {
		x_r0 = x[2 * i];
		x_i0 = x[2 * i + 1];
		mul_re0 = (x_r0 * c_re - x_i0 * c_im) * sh;
		mul_im0 = (x_r0 * c_im + x_i0 * c_re) * sh;
		z[2 * i] = FLOAT2INT_CLAMP(mul_re0);
		z[2 * i + 1] = FLOAT2INT_CLAMP(mul_im0);
	}
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S32C_S32C_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s32 c_re = c[0];
	mlib_s32 c_im = c[1];
	mlib_d64 c_re_lo = (mlib_s32)(c_re & 0xFFFF);
	mlib_d64 c_re_hi = (mlib_s32)((c_re & ~0xFFFF) >> 16);
	mlib_d64 c_im_lo = (mlib_s32)(c_im & 0xFFFF);
	mlib_d64 c_im_hi = (mlib_s32)((c_im & ~0xFFFF) >> 16);
	mlib_d64 x_re, x_im;
	mlib_s64 r0, r1, r2, r3;
	mlib_s32 i;

	if ((shift < 1) || (shift > 31))
		return (MLIB_OUTOFRANGE);

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		x_re = x[2 * i];
		x_im = x[2 * i + 1];

		r0 = x_re * c_re_hi - x_im * c_im_hi;
		r1 = x_re * c_re_lo - x_im * c_im_lo;

		r2 = x_re * c_im_hi + x_im * c_re_hi;
		r3 = x_re * c_im_lo + x_im * c_re_lo;

		z[2 * i] = (((mlib_s64)r0 << 16) + (mlib_s64)r1) >> shift;
		z[2 * i + 1] = (((mlib_s64)r2 << 16) + (mlib_s64)r3) >> shift;
	}
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S32C_Sat(
	mlib_s32 *xz,
	const mlib_s32 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_S32C_S32C_Sat(xz, xz, c, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S32C_Mod(
	mlib_s32 *xz,
	const mlib_s32 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_S32C_S32C_Mod(xz, xz, c, n, shift));
}

/* *********************************************************** */
