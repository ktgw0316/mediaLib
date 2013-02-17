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
 *
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
 * SYNOPSIS
 *
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
 *                                                         mlib_s32   shift);
 *      mlib_status mlib_VectorMulSShift_S32_S32_[Sat|Mod](mlib_s32       *z,
 *                                                         const mlib_s32 *x,
 *                                                         const mlib_s32 *c,
 *                                                         mlib_s32       n,
 *                                                         mlib_s32   shift);
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
 *                                                     mlib_s32   shift);
 *      mlib_status mlib_VectorMulSShift_S32_[Sat|Mod](mlib_s32       *xz,
 *                                                     const mlib_s32 *c,
 *                                                     mlib_s32       n,
 *                                                     mlib_s32   shift);
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
 *
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
 * SYNOPSIS
 *
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
 *                                                     mlib_s32  shift);
 *      mlib_status mlib_VectorMulSShift_U8C_[Sat|Mod](mlib_u8       *xz,
 *                                                     const mlib_u8 *c,
 *                                                     mlib_s32      n,
 *                                                     mlib_s32  shift);
 *      mlib_status mlib_VectorMulSShift_S16C_[Sat|Mod](mlib_s16       *xz,
 *                                                      const mlib_s16 *c,
 *                                                      mlib_s32       n,
 *                                                      mlib_s32   shift);
 *      mlib_status mlib_VectorMulSShift_S32C_[Sat|Mod](mlib_s32       *xz,
 *                                                      const mlib_s32 *c,
 *                                                      mlib_s32       n,
 *                                                      mlib_s32   shift);
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
 *      z[2*i + 0] = (x[2*i + 0] * c[0] - x[2*i + 1] * c[1]) * 2^(-shift)
 *      z[2*i + 1] = (x[2*i + 1] * c[0] + x[2*i + 0] * c[1]) * 2^(-shift)
 *      xz[2*i + 0] = (xz[2*i + 0] * c[0] - xz[2*i + 1] * c[1]) * 2^(-shift)
 *      xz[2*i + 1] = (xz[2*i + 1] * c[0] + xz[2*i + 0] * c[1]) * 2^(-shift)
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

#define	PACK_U16_U8_8                                                \
	{                                                            \
	    mlib_d64 r0, r1, r2, r3;                                 \
	                                                             \
	    r0 = vis_fpmerge(vis_read_hi(dr0), vis_read_lo(dr0));    \
	    r1 = vis_fpmerge(vis_read_hi(r0), vis_read_lo(r0));      \
	    r2 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));    \
	    r3 = vis_fpmerge(vis_read_hi(r2), vis_read_lo(r2));      \
	    dr0 = vis_freg_pair(vis_read_hi(r1), vis_read_hi(r3));   \
	}

/* *********************************************************** */

#define	PACK_U16_U8                                                  \
	mask_over = vis_fcmpgt16(dzero, dr1) |                       \
		(vis_fcmpgt16(dzero, dr0) << 4);                     \
	dr0 = vis_fpack16_to_hi(dr0, dr0);                           \
	dr0 = vis_fpack16_to_lo(dr0, dr1)

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
	mlib_s32 n8 = n >> 3, n1;

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

		if (!((mlib_addr)x & 7) && !((mlib_addr)z & 7)) {

#pragma pipeloop(0)
			for (i = 0; i < n8; i++) {
				dx = dpx[i];
				MUL_U8_U16;
				PACK_U16_U8_8;
				dpz[i] = dr0;
			}

#pragma pipeloop(0)
			for (i = 0; i < n % 8; i++) {
				mlib_s32 mul;

				mul = ((mlib_s32)(uc * (mlib_d64)x[n - i -
					1])) >> shift;
				z[n - i - 1] =
					(mul > MLIB_U8_MAX) ? MLIB_U8_MAX : mul;
			}
		} else {

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
			n1 = n - (8 + off);
			n8 = n1 >> 3;

			dx0 = vis_ld_d64_nf(dpx);
			dx1 = vis_ld_d64_nf(dpx + 1);
			dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
			for (i = 0; i < n8; i++) {
				dx0 = dx1;
				MUL_U8_U16;
				PACK_U16_U8_8;
				dx1 = vis_ld_d64_nf(dpx + i + 2);
				dpz[i] = dr0;
				dx = vis_faligndata(dx0, dx1);
			}
#pragma pipeloop(0)
			for (i = 0; i < n1 % 8; i++) {
				mlib_s32 mul;

				mul = ((mlib_s32)(uc * (mlib_d64)x[n - i -
					1])) >> shift;
				z[n - i - 1] =
					(mul > MLIB_U8_MAX) ? MLIB_U8_MAX : mul;
			}
		}
	} else {

		vis_write_gsr((7 - shift) << 3);

		if (!((mlib_addr)x & 7) && !((mlib_addr)z & 7)) {

			dx = dpx[0];
#pragma pipeloop(0)
			for (i = 0; i < n8; i++) {
				MUL_U8_U16;
				PACK_U16_U8;
				dx = vis_ld_d64_nf(dpx + i + 1);
				dpz[i] = dr0;
				vis_pst_8(d_over, dpz + i, mask_over);
			}

#pragma pipeloop(0)
			for (i = 0; i < n % 8; i++) {
				mlib_s32 mul;

				mul = ((mlib_s32)(uc * (mlib_d64)x[n - i -
					1])) >> shift;
				z[n - i - 1] =
					(mul > MLIB_U8_MAX) ? MLIB_U8_MAX : mul;
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
			n1 = n - (8 + off);
			n8 = n1 >> 3;

			dx0 = vis_ld_d64_nf(dpx);
			dx1 = vis_ld_d64_nf(dpx + 1);
			dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
			for (i = 0; i < n8; i++) {
				dx0 = dx1;
				MUL_U8_U16;
				PACK_U16_U8;
				dx1 = vis_ld_d64_nf(dpx + i + 2);
				dpz[i] = dr0;
				vis_pst_8(d_over, dpz + i, mask_over);
				dx = vis_faligndata(dx0, dx1);
			}

#pragma pipeloop(0)
			for (i = 0; i < n1 % 8; i++) {
				mlib_s32 mul;

				mul = ((mlib_s32)(uc * (mlib_d64)x[n - i -
					1])) >> shift;
				z[n - i - 1] =
					(mul > MLIB_U8_MAX) ? MLIB_U8_MAX : mul;
			}
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
	dr = vis_fpack16_to_lo(dr, dr1)

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
	mlib_s32 n8 = n >> 3, n1;
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

	if (!((mlib_addr)x & 7) && !((mlib_addr)z & 7)) {

#pragma pipeloop(0)
		for (i = 0; i < n8; i++) {
			dx = dpx[i];
			MUL_S16_U8_MOD;
			PACK_U16_U8_MOD;
			dpz[i] = dr;
		}
#pragma pipeloop(0)
		for (i = 0; i < n % 8; i++) {
			z[n - i - 1] =
				((mlib_s32)(c[0] * (mlib_d64)x[n - i -
				1])) >> shift;
		}
	} else {
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
		n1 = n - (8 + off);
		n8 = n1 >> 3;

		if (n1 < 0)
			return (MLIB_SUCCESS);

		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
		for (i = 0; i < n8; i++) {
			dx0 = dx1;
			MUL_S16_U8_MOD;
			PACK_U16_U8_MOD;
			dx1 = vis_ld_d64_nf(dpx + i + 2);
			dpz[i] = dr;
			dx = vis_faligndata(dx0, dx1);
		}

#pragma pipeloop(0)
		for (i = 0; i < n1 % 8; i++) {
			z[n - i - 1] =
				((mlib_s32)(c[0] * (mlib_d64)x[n - i -
				1])) >> shift;
		}
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

#define	EXPAND_U8C_FOR_MUL            \
	/* x_r */                     \
	dr2 = vis_faligndata(dx, dx)

/* *********************************************************** */

#define	EXPAND_U8C_FOR_MUL_8	dr2 = vis_faligndata(dx, dx)	/* x_r */

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
	}

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
					vis_write_gsr(gsr_for_mul_8);
#pragma pipeloop(0)
					for (i = 0; i < 256; i++) {
						dx = dpx[256 * k + i];
						EXPAND_U8C_FOR_0_MUL_8;
						MUL_U8C_S16C_8;
						dr1_buff[i] = dr1;
						dr_buff[i] = dr;
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
				vis_write_gsr(gsr_for_mul_8);
#pragma pipeloop(0)
				for (i = 0; i < len - N; i++) {
					dx = dpx[i + N];
					EXPAND_U8C_FOR_0_MUL_8;
					MUL_U8C_S16C_8;
					dr1_buff[i] = dr1;
					dr_buff[i] = dr;
				}

				vis_write_gsr(gsr_for_pack_8);
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
					vis_write_gsr(gsr_for_mul_8);
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
				vis_write_gsr(gsr_for_mul_8);
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
					PACK_S16C_U8C0;
					dpz[i + N] = dr;
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
			vis_write_gsr(gsr_for_mul_8);
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
		vis_write_gsr(gsr_for_mul_8);
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
					vis_write_gsr(gsr_for_mul);
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
				vis_write_gsr(gsr_for_mul);
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
					vis_write_gsr(gsr_for_mul);
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
				vis_write_gsr(gsr_for_mul);
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
			vis_write_gsr(gsr_for_mul);
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
		vis_write_gsr(gsr_for_mul);
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
	dr2 = vis_faligndata(dx, dx)                            \
	/* \ x_r \ */

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
	}

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
	mlib_u8 *pzend, *px = (void *)x;
	mlib_d64 *dpz, *dpx, *dpzend;
	mlib_d64 dx, dr, dr1, dr2, dr3, dx0, dx1, dc_hi, dc_lo;
	mlib_d64 clear = vis_to_double_dup(0xFF00FF00);
	mlib_d64 dzero = vis_fzero();
	mlib_s32 N, k, off;
	mlib_d64 x_buff[256], r_buff[256];
	mlib_d64 mod_mask = vis_to_double_dup(0xFF << shift);

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
			dx = dpx[0];

		} else {

			for (k = 0; k < (len >> 8); k++) {
/* initialize GSR */
				vis_write_gsr(gsr_for_mul);
				vis_alignaddr((void *)x, off);
				dx0 = dpx[256 * k];
				dx1 = vis_ld_d64_nf(dpx + 256 * k + 1);
				dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx0 = dx1;
					EXPAND_U8C_FOR_0_MUL_MOD;
					MUL_U8C_S16C_MOD;
					PACK_S16C_U8C0_MOD;
					dx1 = vis_ld_d64_nf(dpx + 256 * k +
						i + 2);
					dpz[256 * k + i] = dr;
					dx = vis_faligndata(dx0, dx1);
				}

			}

			N = len & ~255;
/* initialize GSR */
			vis_write_gsr(gsr_for_mul);
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
	vis_alignaddr((void *)0, 7);
	PACK_S16C_U8C1_MOD;
	vis_pst_8(dr1, dpz, emask);

	if ((mlib_addr)++dpz <= (mlib_addr)pzend) {
		vis_pst_8(dr1, dpz, 0x80);
	}

	len = dpzend - dpz;

	if (len < 0)
		return (MLIB_SUCCESS);

	for (k = 0; k < (len >> 8); k++) {
/* initialize GSR */
		vis_write_gsr(gsr_for_mul);
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
/* initialize GSR */
	vis_write_gsr(gsr_for_mul);
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

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
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
	dr = vis_fxor(dr, restore)

/* *********************************************************** */

#define	PACK_S16_S8_MOD                                         \
	dr = vis_fand(dr, without_control);                     \
	dr1 = vis_fand(dr1, without_control);                   \
	dr = vis_fpack16_to_hi(dr, dr);                         \
	dr = vis_fpack16_to_lo(dr, dr1)

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
	mlib_s32 off;
	mlib_s8 *pzend, *px = (mlib_s8 *)x;
	mlib_d64 *dpz = (mlib_d64 *)z, *dpx = (mlib_d64 *)x, *dpzend;
	mlib_d64 dx, dr, dr1, dx0, dx1;
	mlib_s32 n8 = n >> 3, n1;
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

#pragma pipeloop(0)
		for (i = 0; i < n8; i++) {
			dx = dpx[i];
			MUL_S16_S8_SAT;
			PACK_S16_S8_SAT;
			dpz[i] = dr;
		}

#pragma pipeloop(0)
		for (i = 0; i < n % 8; i++) {
			mlib_s32 mul;

			mul = ((mlib_s32)(c[0] * (mlib_d64)x[n - i -
				1])) >> shift;
			z[n - i - 1] =
				(mul > MLIB_S8_MAX) ? MLIB_S8_MAX : (mul <
				MLIB_S8_MIN ? MLIB_S8_MIN : mul);
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
		n1 = n - (8 + off);
		n8 = n1 >> 3;

		if (n1 < 0)
			return (MLIB_SUCCESS);

		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
		for (i = 0; i < n8; i++) {
			dx0 = dx1;
			MUL_S16_S8_SAT;
			PACK_S16_S8_SAT;
			dx1 = vis_ld_d64_nf(dpx + i + 2);
			dpz[i] = dr;
			dx = vis_faligndata(dx0, dx1);
		}

#pragma pipeloop(0)
		for (i = 0; i < n1 % 8; i++) {
			mlib_s32 mul;

			mul = ((mlib_s32)(c[0] * (mlib_d64)x[n - i -
				1])) >> shift;
			z[n - i - 1] =
				(mul > MLIB_S8_MAX) ? MLIB_S8_MAX : (mul <
				MLIB_S8_MIN ? MLIB_S8_MIN : mul);
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
	mlib_s32 n8 = n >> 3, n1;
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

#pragma pipeloop(0)
		for (i = 0; i < n8; i++) {
			dx = dpx[i];
			MUL_S16_S8_SAT;
			PACK_S16_S8_MOD;
			dpz[i] = dr;
		}

#pragma pipeloop(0)
		for (i = 0; i < n % 8; i++) {
			z[n - i - 1] =
				((mlib_s32)(c[0] * (mlib_d64)x[n - i -
				1])) >> shift;
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
		n1 = n - (8 + off);
		n8 = n1 >> 3;

		if (n1 < 0)
			return (MLIB_SUCCESS);

		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
		for (i = 0; i < n8; i++) {
			dx0 = dx1;
			MUL_S16_S8_SAT;
			PACK_S16_S8_MOD;
			dx1 = vis_ld_d64_nf(dpx + i + 2);
			dpz[i] = dr;
			dx = vis_faligndata(dx0, dx1);
		}

#pragma pipeloop(0)
		for (i = 0; i < n1 % 8; i++) {
			z[n - i - 1] =
				((mlib_s32)(c[0] * (mlib_d64)x[n - i -
				1])) >> shift;
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

			dx = dpx[0];
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
				dx = dpx[0];
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
	dr = vis_for(dr, dr1)

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
	mlib_s32 off;
	mlib_s8 *pzend, *px = (void *)x;
	mlib_d64 *dpz = (mlib_d64 *)z,
		*dpx = (mlib_d64 *)x,
		*dpx1 =	(mlib_d64 *)x, *dpzend;
	mlib_d64 dx, dr, dr1, dx0, dx1;
	mlib_d64 dr_buff[256], dr1_buff[256];
	mlib_s32 n4 = n >> 2;
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

		if (!((mlib_addr)x & 7) && !((mlib_addr)z & 7)) {

			dx = dpx[0];
#pragma pipeloop(0)
			for (i = 0; i < n4; i++) {
				MUL_S8C_S16C;
				PACK_S16C_S8C0_MOD;
				dx = vis_ld_d64_nf(dpx + i + 1);
				dpz[i] = dr;
			}
			dpx += n4;
			dpz += n4;

			pzend = (mlib_s8 *)z + n + n - 1;
			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				dx = dpx[0];
				MUL_S8C_S16C;
				PACK_S16C_S8C0_MOD;
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
			PACK_S16C_S8C0_MOD;
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
				PACK_S16C_S8C0_MOD;
				dx1 = vis_ld_d64_nf(dpx + i + 2);
				dpz[i] = dr;
				dx = vis_faligndata(dx0, dx1);
			}

			dpz = dpzend;

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				MUL_S8C_S16C;
				PACK_S16C_S8C0_MOD;
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

	for (k = 0; k < (len >> 8); k++) {

		(void *)vis_alignaddr(px, off);
		dx0 = vis_ld_d64_nf(dpx + 256 * k);
		dx1 = vis_ld_d64_nf(dpx + 256 * k + 1);
		dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
		for (i = 0; i < 256; i++) {
			dx0 = dx1;
			MUL_S8C_S16C;
			dx1 = vis_ld_d64_nf(dpx + 256 * k + i + 2);
			dr_buff[i] = dr;
			dr1_buff[i] = dr1;
			dx = vis_faligndata(dx0, dx1);
		}

		vis_alignaddr(dpx, 7);
#pragma pipeloop(0)
		for (i = 0; i < 256; i++) {
			dr = dr_buff[i];
			dr1 = dr1_buff[i];
			PACK_S16C_S8C1_MOD;
			vis_pst_8(dr, dpz + 256 * k + i, 0x7f);
			vis_pst_8(dr1, dpz + 256 * k + i + 1, 0x80);
		}

	}

	N = len & ~255;
	(void *)vis_alignaddr(px, off);
	dx0 = vis_ld_d64_nf(dpx + N);
	dx1 = vis_ld_d64_nf(dpx + N + 1);
	dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
	for (i = N; i < len; i++) {
		dx0 = dx1;
		MUL_S8C_S16C;
		dx1 = vis_ld_d64_nf(dpx + i + 2);
		dr_buff[i - N] = dr;
		dr1_buff[i - N] = dr1;
		dx = vis_faligndata(dx0, dx1);
	}

	vis_alignaddr(dpx, 7);
#pragma pipeloop(0)
	for (i = N; i < len; i++) {
		dr = dr_buff[i - N];
		dr1 = dr1_buff[i - N];
		PACK_S16C_S8C1_MOD;
		vis_pst_8(dr, dpz + i, 0x7f);
		vis_pst_8(dr1, dpz + i + 1, 0x80);
	}

	dpz = dpzend;

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
		MUL_S8C_S16C;
		vis_alignaddr(dpx, 7);
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
	vis_pst_16(dr, dpz, emask)

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
	mlib_s32 i, n4 = n >> 2, valc = c[0], len, end;

	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

	vis_write_gsr((16 - shift) << 3);

	if (!((mlib_addr)z & 7) && !((mlib_addr)x & 7)) {

#pragma pipeloop(0)
		dx = dpx[0];
		for (i = 0; i < n4; i++) {
			MUL_S16_S32;
			dx = vis_ld_d64_nf(dpx + i + 1);
			dpz[i] = vis_fpackfix_pair(dr, dr1);
		}

		for (i = 0; i < n % 4; i++) {
			mlib_s32 mul;

			mul = ((mlib_s32)(valc * (mlib_d64)x[n - i -
				1])) >> shift;
			z[n - i - 1] =
				(mul > MLIB_S16_MAX) ? MLIB_S16_MAX : (mul <
				MLIB_S16_MIN ? MLIB_S16_MIN : mul);
		}
	} else {
		mlib_s32 emask, off;

		pzend = (mlib_s16 *)z + n - 1;
		dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
		dpz = (mlib_d64 *)((mlib_addr)z & (~7));
		off = (mlib_addr)dpz - (mlib_addr)z;
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

		end = (mlib_s16 *)pzend - (mlib_s16 *)dpzend + 1;

		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
		for (i = 0; i < len; i++) {
			dx0 = dx1;
			MUL_S16_S32;
			dx1 = vis_ld_d64_nf(dpx + i + 2);
			dpz[i] = vis_fpackfix_pair(dr, dr1);
			dx = vis_faligndata(dx0, dx1);
		}

		for (i = 0; i < end; i++) {
			mlib_s32 mul;

			mul = ((mlib_s32)(valc * (mlib_d64)x[n - i -
				1])) >> shift;
			z[n - i - 1] =
				(mul > MLIB_S16_MAX) ? MLIB_S16_MAX : (mul <
				MLIB_S16_MIN ? MLIB_S16_MIN : mul);
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

#define	PACK_S32_S16_STORE_SAT                                  \
	dr = vis_fpackfix_pair(dr, dr1);                        \
	vis_pst_16(dr, dpz, emask)

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
	mlib_s32 off, emask, len, end;
	mlib_f32 c_dir = vis_to_float((c[0] << 16) | (c[0] & 0xFFFF));
	mlib_d64 *dpz = (mlib_d64 *)z, *dpx = (mlib_d64 *)x, *dpzend;
	mlib_d64 dx, dx0, dx1, dr, dr1;
	mlib_d64 mask_clear =
		vis_to_double_dup(((mlib_u32)0xffffffff >> (16 - shift + 1)));
	mlib_d64 mask_sign = vis_to_double_dup((mlib_u32)0x00008000 << shift);
	mlib_d64 mask_real_sign = vis_to_double_dup(0x80008000);
	mlib_s16 *pzend;
	mlib_s16 valc = c[0];
	mlib_s32 i, n4 = n >> 2;

	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);

	if (n <= 0)
		return (MLIB_FAILURE);

	vis_write_gsr((16 - shift) << 3);

	if (shift == 16) {

		if (!((mlib_addr)x & 7) && !((mlib_addr)z & 7)) {

#pragma pipeloop(0)
			dx = dpx[0];
			for (i = 0; i < n4; i++) {
				MUL_S16_S32;
				dx = vis_ld_d64_nf(dpx + i + 1);
				dpz[i] = vis_fpackfix_pair(dr, dr1);
			}

#pragma pipeloop(0)
			for (i = 0; i < n % 4; i++) {
				z[n - i - 1] =
					((mlib_s32)(valc * (mlib_d64)x[n - i -
					1])) >> shift;
			}
		} else {
			pzend = (mlib_s16 *)z + n - 1;
			dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
			dpz = (mlib_d64 *)((mlib_addr)z & (~7));
			off = (mlib_addr)dpz - (mlib_addr)z;

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

			end = (mlib_s16 *)pzend - (mlib_s16 *)dpzend + 1;

			dx0 = vis_ld_d64_nf(dpx);
			dx1 = vis_ld_d64_nf(dpx + 1);
			dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
			for (i = 0; i < len; i++) {
				dx0 = dx1;
				MUL_S16_S32;
				dx1 = vis_ld_d64_nf(dpx + i + 2);
				dpz[i] = vis_fpackfix_pair(dr, dr1);
				dx = vis_faligndata(dx0, dx1);
			}

#pragma pipeloop(0)
			for (i = 0; i < end; i++) {
				z[n - i - 1] =
					((mlib_s32)(valc * (mlib_d64)x[n - i -
					1])) >> shift;
			}
		}
	} else {

		if (!((mlib_addr)x & 7) && !((mlib_addr)z & 7)) {

			dx = dpx[0];
#pragma pipeloop(0)
			for (i = 0; i < n4; i++) {
				MUL_S16_S32;
				PACK_S32_S16_MOD;
				dx = vis_ld_d64_nf(dpx + i + 1);
				dpz[i] = dr;
			}

#pragma pipeloop(0)
			for (i = 0; i < n % 4; i++) {
				z[n - i - 1] =
					((mlib_s32)(valc * (mlib_d64)x[n - i -
					1])) >> shift;
			}
		} else {
			pzend = (mlib_s16 *)z + n - 1;
			dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
			dpz = (mlib_d64 *)((mlib_addr)z & (~7));
			off = (mlib_addr)dpz - (mlib_addr)z;

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
			end = (mlib_s16 *)pzend - (mlib_s16 *)dpzend + 1;

			if (len < 0)
				return (MLIB_SUCCESS);

			dx0 = vis_ld_d64_nf(dpx);
			dx1 = vis_ld_d64_nf(dpx + 1);
			dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
			for (i = 0; i < len; i++) {
				dx0 = dx1;
				MUL_S16_S32;
				PACK_S32_S16_MOD;
				dx1 = vis_ld_d64_nf(dpx + i + 2);
				dpz[i] = dr;
				dx = vis_faligndata(dx0, dx1);
			}

#pragma pipeloop(0)
			for (i = 0; i < end; i++) {
				z[n - i - 1] =
					((mlib_s32)(valc * (mlib_d64)x[n - i -
					1])) >> shift;
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

#define	PACK_S32C_S16C_SHIFT16_MOD	dr = vis_fpackfix_pair(dr, dr1)

/* *********************************************************** */

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
	mlib_s32 off;
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
	mlib_s32 smul_buff[1024], s0, s1, s2, s3,
		_shift = 16 - shift, *spz, old_s;
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

#pragma pipeloop(0)
				for (i = 0; i < end; i++) {
					dx = dpx[i];
					MUL_S16C_S32C;
					PAIR_S32C_S16C0_MOD;
					PACK_S32C_S16C_SHIFT16_MOD;
					dpz[i] = dr;
				}

				dx = dpx[end];
				MUL_S16C_S32C;
				PAIR_S32C_S16C0_MOD;
				PACK_S32C_S16C_SHIFT16_MOD;
/* prepare edge mask for the last bytes */
				emask = vis_edge16(dpzend, pzend);
/* store last bytes of result */
				vis_pst_16(dr, dpzend, emask);
				return (MLIB_SUCCESS);
			}

			else {

				for (k = 0; k < (end >> 8); k++) {

					(void *)vis_alignaddr((void *)x, off);
					dx0 = dpx[k * 256];
					dx1 = dpx[k * 256 + 1];
					dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
					for (i = 0; i < 256; i++) {
						dx0 = dx1;
						MUL_S16C_S32C;
						PAIR_S32C_S16C0_MOD;
						PACK_S32C_S16C_SHIFT16_MOD;
						dx1 = vis_ld_d64_nf(dpx +
							k * 256 + i + 2);
						dpz[k * 256 + i] = dr;
						dx = vis_faligndata(dx0, dx1);
					}
				}

				N = end & ~255;
				(void *)vis_alignaddr((void *)x, off);
				dx0 = vis_ld_d64_nf(dpx + N);
				dx1 = vis_ld_d64_nf(dpx + N + 1);
				dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
				for (i = N; i < end; i++) {
					dx0 = dx1;
					MUL_S16C_S32C;
					PAIR_S32C_S16C0_MOD;
					PACK_S32C_S16C_SHIFT16_MOD;
					dx1 = vis_ld_d64_nf(dpx + i + 2);
					dpz[i] = dr;
					dx = vis_faligndata(dx0, dx1);
				}
			}

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

		for (k = 0; k < (end >> 8); k++) {

			(void *)vis_alignaddr((void *)x, off);
			dx0 = dpx[k * 256];
			dx1 = dpx[k * 256 + 1];
			dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
			for (i = 0; i < 256; i++) {
				dx0 = dx1;
				MUL_S16C_S32C;
				PAIR_S32C_S16C1_MOD;
				PACK_S32C_S16C_SHIFT16_MOD;
				dx1 = vis_ld_d64_nf(dpx + k * 256 + i + 2);
				vis_pst_16(dr, dpz + k * 256 + i, 0x7);
				vis_pst_16(dr, dpz + k * 256 + i + 1, 0x8);
				dx = vis_faligndata(dx0, dx1);
			}
		}

		N = end & ~255;
		(void *)vis_alignaddr((void *)x, off);
		dx0 = vis_ld_d64_nf(dpx + N);
		dx1 = vis_ld_d64_nf(dpx + N + 1);
		dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
		for (i = N; i < end; i++) {
			dx0 = dx1;
			MUL_S16C_S32C;
			PAIR_S32C_S16C1_MOD;
			PACK_S32C_S16C_SHIFT16_MOD;
			dx1 = vis_ld_d64_nf(dpx + i + 2);
			vis_pst_16(dr, dpz + i, 0x7);
			vis_pst_16(dr, dpz + i + 1, 0x8);
			dx = vis_faligndata(dx0, dx1);
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
	} else {
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
				spz = (mlib_s32 *)dpz;
				for (k = 0; k < (end >> 8); k++) {

					dx = dpx[k * 256];
#pragma pipeloop(0)
					for (i = 0; i < 256; i++) {
						MUL_S16C_S32C;
						PAIR_S32C_S16C0_MOD;
						dx = vis_ld_d64_nf(dpx +
							k * 256 + i + 1);
						dmul_buff[2 * i] = dr;
						dmul_buff[2 * i + 1] = dr1;
					}
#pragma pipeloop(0)
					for (i = 0; i < 512; i++) {
						s0 = smul_buff[2 * i + 0];
						s1 = smul_buff[2 * i + 1];
						s0 = (s0 << _shift) & ~0xFFFF;
						s1 = (s1 >> shift) & 0xFFFF;
						spz[k * 512 + i] = (s0 | s1);
					}
				}

				N = end & ~255;
				dx = vis_ld_d64_nf(dpx + N);
#pragma pipeloop(0)
				for (i = N; i < end; i++) {
					MUL_S16C_S32C;
					PAIR_S32C_S16C0_MOD;
					dx = vis_ld_d64_nf(dpx + i + 1);
					dmul_buff[2 * (i - N)] = dr;
					dmul_buff[2 * (i - N) + 1] = dr1;
				}

				spz = (mlib_s32 *)(dpz + N);
#pragma pipeloop(0)
				for (i = 0; i < (end - N) * 2; i++) {
					s0 = smul_buff[2 * i + 0];
					s1 = smul_buff[2 * i + 1];
					s0 = (s0 << _shift) & ~0xFFFF;
					s1 = (s1 >> shift) & 0xFFFF;
					spz[i] = (s0 | s1);
				}

				dx = dpx[end];
				MUL_S16C_S32C;
				PAIR_S32C_S16C0_MOD;
				PACK_S32C_S16C_SHIFT_MOD;
/* prepare edge mask for the last bytes */
				emask = vis_edge16(dpzend, pzend);
/* store last bytes of result */
				vis_pst_16(dr, dpzend, emask);
				return (MLIB_SUCCESS);
			} else {
				spz = (mlib_s32 *)dpz;
				for (k = 0; k < (end >> 8); k++) {

					(void *)vis_alignaddr((void *)x, off);
					dx0 = dpx[k * 256];
					dx1 = dpx[k * 256 + 1];
					dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
					for (i = 0; i < 256; i++) {
						dx0 = dx1;
						MUL_S16C_S32C;
						PAIR_S32C_S16C0_MOD;
						dx1 = vis_ld_d64_nf(dpx +
							k * 256 + i + 2);
						dmul_buff[2 * i] = dr;
						dmul_buff[2 * i + 1] = dr1;
						dx = vis_faligndata(dx0, dx1);
					}
#pragma pipeloop(0)
					for (i = 0; i < 512; i++) {
						s0 = smul_buff[2 * i + 0];
						s1 = smul_buff[2 * i + 1];
						s0 = (s0 << _shift) & ~0xFFFF;
						s1 = (s1 >> shift) & 0xFFFF;
						spz[k * 512 + i] = (s0 | s1);
					}
				}

				N = end & ~255;
				(void *)vis_alignaddr((void *)x, off);
				dx0 = vis_ld_d64_nf(dpx + N);
				dx1 = vis_ld_d64_nf(dpx + N + 1);
				dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
				for (i = N; i < end; i++) {
					dx0 = dx1;
					MUL_S16C_S32C;
					PAIR_S32C_S16C0_MOD;
					dx1 = vis_ld_d64_nf(dpx + i + 2);
					dmul_buff[2 * (i - N)] = dr;
					dmul_buff[2 * (i - N) + 1] = dr1;
					dx = vis_faligndata(dx0, dx1);
				}

				spz = (mlib_s32 *)(dpz + N);
#pragma pipeloop(0)
				for (i = 0; i < (end - N) * 2; i++) {
					s0 = smul_buff[2 * i + 0];
					s1 = smul_buff[2 * i + 1];
					s0 = (s0 << _shift) & ~0xFFFF;
					s1 = (s1 >> shift) & 0xFFFF;
					spz[i] = (s0 | s1);
				}
			}
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
		spz = (mlib_s32 *)dpz;

		old_s = spz[0] & ~0xFFFF;
		for (k = 0; k < (end >> 8); k++) {

			(void *)vis_alignaddr((void *)x, off);
			dx0 = dpx[k * 256];
			dx1 = dpx[k * 256 + 1];
			dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
			for (i = 0; i < 256; i++) {
				dx0 = dx1;
				MUL_S16C_S32C;
				PAIR_S32C_S16C0_MOD;
				dx1 = vis_ld_d64_nf(dpx + k * 256 + i + 2);
				dmul_buff[2 * i] = dr;
				dmul_buff[2 * i + 1] = dr1;
				dx = vis_faligndata(dx0, dx1);
			}
#pragma pipeloop(0)
			for (i = 0; i < 256; i++) {
				s0 = smul_buff[4 * i + 0];
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
		(void *)vis_alignaddr((void *)x, off);
		dx0 = vis_ld_d64_nf(dpx + N);
		dx1 = vis_ld_d64_nf(dpx + N + 1);
		dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
		for (i = N; i < end; i++) {
			dx0 = dx1;
			MUL_S16C_S32C;
			PAIR_S32C_S16C0_MOD;
			dx1 = vis_ld_d64_nf(dpx + i + 2);
			dmul_buff[2 * (i - N)] = dr;
			dmul_buff[2 * (i - N) + 1] = dr1;
			dx = vis_faligndata(dx0, dx1);
		}

		spz = (mlib_s32 *)(dpz + N);
#pragma pipeloop(0)
		for (i = 0; i < end - N; i++) {
			s0 = smul_buff[4 * i + 0];
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

#define	OVER_MUL_S16C_SAT                                       \
	mask_over = (vis_fcmpeq32(dr, d_min32) << 2) |          \
		(vis_fcmpeq32(dr1, d_min32));                   \
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
	mlib_s32 off;
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

	mlib_d64 dc_1 = vis_to_double_dup((((mlib_u16 *)c)[0] << 16) |
		((mlib_u16 *)c)[1]);
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

			dx = dpx[0];
#pragma pipeloop(0)
			for (i = 0; i < end; i++) {
				MUL_S16C_S32C;
				PACK_S32C_S16C0_SAT;
				dx = vis_ld_d64_nf(dpx + i + 1);
				vis_pst_16(d_over, dpz + i, mask_over);
				vis_pst_16(dr, dpz + i, ~mask_over);
			}

			MUL_S16C_S32C;
			PACK_S32C_S16C0_SAT;
/* prepare edge mask for the last bytes */
			emask = vis_edge16(dpzend, pzend);
/* store last bytes of result */
			vis_pst_16(dr, dpzend, emask);
			vis_pst_16(d_over, dpzend, mask_over & emask);
			return (MLIB_SUCCESS);
		} else {

			for (k = 0; k < (end >> 8); k++) {

				(void *)vis_alignaddr((void *)x, off);
				dx0 = dpx[k * 256];
				dx1 = dpx[k * 256 + 1];
				dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx0 = dx1;
					MUL_S16C_S32C;
					PACK_S32C_S16C0_SAT;
					dx1 = vis_ld_d64_nf(dpx + k * 256 +
						i + 2);
					vis_pst_16(d_over, dpz + 256 * k + i,
						mask_over);
					vis_pst_16(dr, dpz + 256 * k + i,
						~mask_over);
					dx = vis_faligndata(dx0, dx1);
				}
			}

			N = end & ~255;
			(void *)vis_alignaddr((void *)x, off);
			dx0 = vis_ld_d64_nf(dpx + N);
			dx1 = vis_ld_d64_nf(dpx + N + 1);
			dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
			for (i = N; i < end; i++) {
				dx0 = dx1;
				MUL_S16C_S32C;
				PACK_S32C_S16C0_SAT;
				dx1 = vis_ld_d64_nf(dpx + i + 2);
				vis_pst_16(d_over, dpz + i, mask_over);
				vis_pst_16(dr, dpz + i, ~mask_over);
				dx = vis_faligndata(dx0, dx1);
			}
		}

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

	for (k = 0; k < (end >> 8); k++) {

		(void *)vis_alignaddr((void *)x, off);
		dx0 = dpx[k * 256];
		dx1 = dpx[k * 256 + 1];
		dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
		for (i = 0; i < 256; i++) {
			dx0 = dx1;
			MUL_S16C_S32C;
			PACK_S32C_S16C1_SAT;
			dx1 = vis_ld_d64_nf(dpx + k * 256 + i + 2);
			vis_pst_16(d_over, dpz + 256 * k + i, mask_over & 0x7);
			vis_pst_16(dr, dpz + 256 * k + i, ~mask_over & 0x7);
			vis_pst_16(d_over, dpz + 256 * k + i + 1,
				mask_over & 0x8);
			vis_pst_16(dr, dpz + 256 * k + i + 1, ~mask_over & 0x8);
			dx = vis_faligndata(dx0, dx1);
		}
	}

	N = end & ~255;
	(void *)vis_alignaddr((void *)x, off);
	dx0 = vis_ld_d64_nf(dpx + N);
	dx1 = vis_ld_d64_nf(dpx + N + 1);
	dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
	for (i = N; i < end; i++) {
		dx0 = dx1;
		MUL_S16C_S32C;
		PACK_S32C_S16C1_SAT;
		dx1 = vis_ld_d64_nf(dpx + i + 2);
		vis_pst_16(d_over, dpz + i, mask_over & 0x7);
		vis_pst_16(dr, dpz + i, ~mask_over & 0x7);
		vis_pst_16(d_over, dpz + i + 1, mask_over & 0x8);
		vis_pst_16(dr, dpz + i + 1, ~mask_over & 0x8);
		dx = vis_faligndata(dx0, dx1);
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
	mlib_s32 n2 = (n / 2) * 2;
	mlib_d64 mul0, mul1;
	mlib_s32 i0, i1;

	if ((shift < 1) || (shift > 31))
		return (MLIB_OUTOFRANGE);

#pragma pipeloop(0)
	for (i0 = 0, i1 = 1; i0 < n2; i0 += 2, i1 += 2) {
		mul0 = x[i0] * scalar * sh;
		mul1 = x[i1] * scalar * sh;
		z[i0] = FLOAT2INT_CLAMP(mul0);
		z[i1] = FLOAT2INT_CLAMP(mul1);
	}

	if (n % 2) {
		z[n - 1] = FLOAT2INT_CLAMP(x[n - 1] * scalar * sh);
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
#if (defined _NO_LONGLONG)
	mlib_s32 scal = *c;

/* 2^47 */
	mlib_d64 acc = 0x800000000000;

/* 2^32 */
	mlib_d64 acc1 = 0x100000000;
	mlib_d64 mv = 1. / acc1;
	mlib_s32 acc4 = 1 << 15;

/* 2^63 */
	mlib_d64 acc0 = 0x8000000000000000;
	mlib_d64 mulhi, mullo;
	mlib_d64 lo, hi;
	mlib_s32 shihi, shilo, slohi, slolo, s0, s1, one, i;
	mlib_s32 valx;

	if ((shift < 1) || (shift > 31))
		return (MLIB_OUTOFRANGE);

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		valx = x[i];
		lo = scal & 0xFFFF;
		hi = scal & ~0xFFFF;

		mullo = (valx * lo);
		slohi = (mlib_s32)((mullo + acc) * mv) - acc4;
		slolo = (mlib_s32)(mullo - (slohi * acc1) +
			(mlib_d64)MLIB_S32_MIN) - MLIB_S32_MIN;

		mulhi = (valx * hi);
		mulhi = (mulhi + acc0) * 0.5;
		shihi = (mlib_s32)(mulhi * mv);
		shilo = (mlib_s32)(mulhi - (shihi * acc1) +
			(mlib_d64)MLIB_S32_MIN) - MLIB_S32_MIN;

		one = (((mlib_u32)slolo >> 1) + (shilo & 0x7FFFFFFF)) >> 31;
		shihi = (shihi << 1) | ((mlib_u32)shilo >> 31);
		shilo = shilo << 1;

		s0 = shilo + slolo;
		s1 = shihi + slohi + one;
		z[i] = (s1 << (32 - shift)) | ((mlib_u32)s0 >> shift);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#else /* _NO_LONGLONG */
	mlib_s32 scal = *c;
	mlib_s32 n2 = (n / 2) * 2;
	mlib_s32 i0, i1;
	mlib_d64 scal_lo = (scal < 0) ? (mlib_s32)(scal | 0xFFFF0000) :
		(mlib_s32)(scal & 0xFFFF);
	mlib_d64 scal_hi = scal - scal_lo, x0, x1;
	mlib_s64 mul0, mul1;

	if ((shift < 1) || (shift > 31))
		return (MLIB_OUTOFRANGE);

#pragma pipeloop(0)
	for (i0 = 0, i1 = 1; i0 < n2; i0 += 2, i1 += 2) {
		x0 = x[i0];
		mul0 = x0 * scal_hi;
		z[i0] = (mul0 + (mlib_s64)(x0 * scal_lo)) >> shift;

		x1 = x[i1];
		mul1 = x1 * scal_hi;
		z[i1] = (mul1 + (mlib_s64)(x1 * scal_lo)) >> shift;
	}

	if (n % 2)
		z[n - 1] = (x[n - 1] * (mlib_s64)scal) >> shift;

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* _NO_LONGLONG */
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
	mlib_d64 sh = 1.0 / (1u << shift);
	mlib_d64 c_r = c[0] * sh;
	mlib_d64 c_i = c[1] * sh;
	mlib_s32 n2 = (n / 2) * 4;
	mlib_d64 x_r0, x_i0, x_r1, x_i1;
	mlib_d64 mul_r0, mul_i0, mul_r1, mul_i1;
	mlib_s32 i0, i1, i2, i3;

	if ((shift < 1) || (shift > 31))
		return (MLIB_OUTOFRANGE);

#pragma pipeloop(0)
	for (i0 = 0, i1 = 1, i2 = 2, i3 = 3; i0 < n2;
		i0 += 4, i1 += 4, i2 += 4, i3 += 4) {

		x_r0 = x[i0];
		x_i0 = x[i1];
		mul_r0 = (x_r0 * c_r - x_i0 * c_i);
		mul_i0 = (x_i0 * c_r + x_r0 * c_i);
		z[i0] = FLOAT2INT_CLAMP(mul_r0);
		z[i1] = FLOAT2INT_CLAMP(mul_i0);

		x_r1 = x[i2];
		x_i1 = x[i3];
		mul_r1 = (x_r1 * c_r - x_i1 * c_i);
		mul_i1 = (x_i1 * c_r + x_r1 * c_i);
		z[i2] = FLOAT2INT_CLAMP(mul_r1);
		z[i3] = FLOAT2INT_CLAMP(mul_i1);

	}

	if (n % 2) {
		x_r0 = x[2 * n - 2];
		x_i0 = x[2 * n - 1];
		mul_r0 = (x_r0 * c_r - x_i0 * c_i);
		mul_i0 = (x_i0 * c_r + x_r0 * c_i);
		z[2 * n - 2] = FLOAT2INT_CLAMP(mul_r0);
		z[2 * n - 1] = FLOAT2INT_CLAMP(mul_i0);
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
#if (defined _NO_LONGLONG)
	mlib_s32 c_re = c[0];
	mlib_s32 c_im = c[1];

/* 2^48 */
	mlib_d64 acc = 0x1000000000000;

/* 2^32 */
	mlib_d64 acc1 = 0x100000000;
	mlib_d64 mv = 1. / acc1;
	mlib_s32 acc4 = 1 << 16;

/* 2^63 */
	mlib_d64 acc0 = 0x8000000000000000;
	mlib_d64 mul_re_hi, mul_re_lo, mul_im_hi, mul_im_lo;
	mlib_d64 c_re_lo, c_re_hi, c_im_lo, c_im_hi;
	mlib_s32 shihi_re, shilo_re, slohi_re, slolo_re, shihi_im, shilo_im,
		slohi_im, slolo_im;
	mlib_s32 s0_re, s1_re, s0_im, s1_im, one_re, one_im, i0, i1, n2 = n * 2;
	mlib_s32 x_re, x_im;

	if ((shift < 1) || (shift > 31))
		return (MLIB_OUTOFRANGE);

#pragma pipeloop(0)
	for (i0 = 0, i1 = 1; i0 < n2; i0 += 2, i1 += 2) {
		x_re = x[i0];
		x_im = x[i1];
		c_re_lo = c_re & 0xFFFF;
		c_re_hi = c_re & ~0xFFFF;
		c_im_lo = c_im & 0xFFFF;
		c_im_hi = c_im & ~0xFFFF;

		mul_re_lo = (x_re * c_re_lo - x_im * c_im_lo);
		slohi_re = (mlib_s32)((mul_re_lo + acc) * mv) - acc4;
		slolo_re =
			(mlib_s32)(mul_re_lo - (slohi_re * acc1) +
			(mlib_d64)MLIB_S32_MIN) - MLIB_S32_MIN;

		mul_re_hi = (x_re * c_re_hi - x_im * c_im_hi);
		mul_re_hi = (mul_re_hi + acc0) * 0.5;
		shihi_re = (mlib_s32)(mul_re_hi * mv);
		shilo_re =
			(mlib_s32)(mul_re_hi - (shihi_re * acc1) +
			(mlib_d64)MLIB_S32_MIN) - MLIB_S32_MIN;

		one_re = (((mlib_u32)slolo_re >> 1) +
			(shilo_re & 0x7FFFFFFF)) >> 31;
		shihi_re = (shihi_re << 1) | ((mlib_u32)shilo_re >> 31);
		shilo_re = shilo_re << 1;

		s0_re = shilo_re + slolo_re;
		s1_re = shihi_re + slohi_re + one_re;
		z[i0] = (s1_re << (32 - shift)) | ((mlib_u32)s0_re >> shift);

		mul_im_lo = (x_im * c_re_lo + x_re * c_im_lo);
		slohi_im = (mlib_s32)((mul_im_lo + acc) * mv) - acc4;
		slolo_im =
			(mlib_s32)(mul_im_lo - (slohi_im * acc1) +
			(mlib_d64)MLIB_S32_MIN) - MLIB_S32_MIN;

		mul_im_hi = (x_im * c_re_hi + x_re * c_im_hi);
		mul_im_hi = (mul_im_hi + acc0) * 0.5;
		shihi_im = (mlib_s32)(mul_im_hi * mv);
		shilo_im =
			(mlib_s32)(mul_im_hi - (shihi_im * acc1) +
			(mlib_d64)MLIB_S32_MIN) - MLIB_S32_MIN;

		one_im = (((mlib_u32)slolo_im >> 1) +
			(shilo_im & 0x7FFFFFFF)) >> 31;
		shihi_im = (shihi_im << 1) | ((mlib_u32)shilo_im >> 31);
		shilo_im = shilo_im << 1;

		s0_im = shilo_im + slolo_im;
		s1_im = shihi_im + slohi_im + one_im;
		z[i1] = (s1_im << (32 - shift)) | ((mlib_u32)s0_im >> shift);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#else /* _NO_LONGLONG */
	mlib_s32 c_re = c[0];
	mlib_s32 c_im = c[1];
	mlib_d64 c_re_lo = (mlib_s32)(c_re & 0xFFFF);
	mlib_d64 c_re_hi = (mlib_s32)(c_re & ~0xFFFF);
	mlib_d64 c_im_lo = (mlib_s32)(c_im & 0xFFFF);
	mlib_d64 c_im_hi = (mlib_s32)(c_im & ~0xFFFF);
	mlib_s32 n2 = n * 2;
	mlib_d64 x_re, x_im;
	mlib_s64 r0, r1, r2, r3;
	mlib_s32 i0, i1;

	if ((shift < 1) || (shift > 31))
		return (MLIB_OUTOFRANGE);

#pragma pipeloop(0)
	for (i0 = 0, i1 = 1; i0 < n2; i0 += 2, i1 += 2) {

		x_re = x[i0];
		x_im = x[i1];

		r0 = x_re * c_re_hi - x_im * c_im_hi;
		r1 = x_re * c_re_lo - x_im * c_im_lo;

		r2 = x_re * c_im_hi + x_im * c_re_hi;
		r3 = x_re * c_im_lo + x_im * c_re_lo;

		z[i0] = (r0 + r1) >> shift;
		z[i1] = (r2 + r3) >> shift;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* _NO_LONGLONG */
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
