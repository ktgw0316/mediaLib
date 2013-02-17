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

#pragma ident	"@(#)mlib_v_VectorMul.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *
 *      mlib_VectorMul_[U8|S8]_[U8|S8]_[Sat, Mod],
 *      mlib_VectorMul_[U8|S8]_S16_[Sat, Mod],
 *      mlib_VectorMul_[U8|S8]_[Sat, Mod]  - multiplication of
 *           unsigned/signed 8-bit format vectors
 *
 *      mlib_VectorMul_S16_S16_[Sat, Mod],
 *      mlib_VectorMul_S32_S16_Mod,
 *      mlib_VectorMul_S16_[Sat, Mod] - multiplication of
 *           signed 16-bit format vectors
 *
 *      mlib_VectorMul_S32_S32_[Sat, Mod],
 *      mlib_VectorMul_S32_[Sat, Mod] - multiplication of
 *           signed 32-bit format vectors
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMul_S16_[Sat|Mod](mlib_s16       *xz,
 *                                               const mlib_s16 *y,
 *                                               mlib_s32       n);
 *      mlib_status mlib_VectorMul_S16_S16_[Sat|Mod](mlib_s16       *z,
 *                                                   const mlib_s16 *x,
 *                                                   const mlib_s16 *y,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_VectorMul_S16_U8_[Sat|Mod](mlib_s16      *z,
 *                                                  const mlib_u8 *x,
 *                                                  const mlib_u8 *y,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorMul_S32_[Sat|Mod](mlib_s32       *xz,
 *                                               const mlib_s32 *y,
 *                                               mlib_s32       n);
 *      mlib_status mlib_VectorMul_S32_S32_[Sat|Mod](mlib_s32       *z,
 *                                                   const mlib_s32 *x,
 *                                                   const mlib_s32 *y,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_VectorMul_S8_[Sat|Mod](mlib_s8       *xz,
 *                                              const mlib_s8 *y,
 *                                              mlib_s32      n);
 *      mlib_status mlib_VectorMul_S8_S8_[Sat|Mod](mlib_s8       *z,
 *                                                 const mlib_s8 *x,
 *                                                 const mlib_s8 *y,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_VectorMul_U8_[Sat|Mod](mlib_u8       *xz,
 *                                              const mlib_u8 *y,
 *                                              mlib_s32      n);
 *      mlib_status mlib_VectorMul_U8_U8_[Sat|Mod](mlib_u8       *z,
 *                                                 const mlib_u8 *x,
 *                                                 const mlib_u8 *y,
 *                                                 mlib_s32      n);
 *
 *      mlib_status mlib_VectorMul_S16_S8_Mod(mlib_s16      *z,
 *                                            const mlib_s8 *x,
 *                                            const mlib_s8 *y,
 *                                            mlib_s32      n);
 *      mlib_status mlib_VectorMul_S32_S16_Mod(mlib_s32       *z,
 *                                             const mlib_s16 *x,
 *                                             const mlib_s16 *y,
 *                                             mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      x    pointer to the first element of the first vector
 *      y    pointer to the first element of the second vector
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *      z[i] = x[i] * y[i]
 */

/*
 * FUNCTIONS
 *
 *      mlib_VectorMul_[U8C|S8C]_[U8C|S8C]_[Sat, Mod],
 *      mlib_VectorMul_[U8C|S8C]_[Sat, Mod]  - multiplication of
 *           unsigned/signed 8-bit format vectors
 *
 *      mlib_VectorMul_S16_S16_[Sat, Mod],
 *      mlib_VectorMul_S16_[Sat, Mod] - multiplication of
 *           signed 16-bit format vectors
 *
 *      mlib_VectorMul_S32_S32_[Sat, Mod],
 *      mlib_VectorMul_S32_[Sat, Mod] - multiplication of
 *           signed 32-bit format vectors
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMul_S16C_[Sat|Mod](mlib_s16       *xz,
 *                                                const mlib_s16 *y,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorMul_S16C_S16C_[Sat|Mod](mlib_s16       *z,
 *                                                     const mlib_s16 *x,
 *                                                     const mlib_s16 *y,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_VectorMul_S16C_S8C_[Sat|Mod](mlib_s16      *z,
 *                                                    const mlib_s8 *x,
 *                                                    const mlib_s8 *y,
 *                                                    mlib_s32      n);
 *      mlib_status mlib_VectorMul_S16C_U8C_[Sat|Mod](mlib_s16      *z,
 *                                                    const mlib_u8 *x,
 *                                                    const mlib_u8 *y,
 *                                                    mlib_s32      n);
 *      mlib_status mlib_VectorMul_S32C_[Sat|Mod](mlib_s32       *xz,
 *                                                const mlib_s32 *y,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorMul_S32C_S16C_[Sat|Mod](mlib_s32       *z,
 *                                                     const mlib_s16 *x,
 *                                                     const mlib_s16 *y,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_VectorMul_S32C_S32C_[Sat|Mod](mlib_s32       *z,
 *                                                     const mlib_s32 *x,
 *                                                     const mlib_s32 *y,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_VectorMul_S8C_[Sat|Mod](mlib_s8       *xz,
 *                                               const mlib_s8 *y,
 *                                               mlib_s32      n);
 *      mlib_status mlib_VectorMul_S8C_S8C_[Sat|Mod](mlib_s8       *z,
 *                                                   const mlib_s8 *x,
 *                                                   const mlib_s8 *y,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_VectorMul_U8C_[Sat|Mod](mlib_u8       *xz,
 *                                               const mlib_u8 *y,
 *                                               mlib_s32      n);
 *      mlib_status mlib_VectorMul_U8C_U8C_[Sat|Mod](mlib_u8       *z,
 *                                                   const mlib_u8 *x,
 *                                                   const mlib_u8 *y,
 *                                                   mlib_s32      n);
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
 *      z[i] = x[i] * y[i]
 */

#include <mlib_algebra.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorMul_S32C_S16C_Sat = __mlib_VectorMul_S32C_S16C_Sat
#pragma weak mlib_VectorMul_S16C_S16C_Sat = __mlib_VectorMul_S16C_S16C_Sat
#pragma weak mlib_VectorMul_S32C_Sat = __mlib_VectorMul_S32C_Sat
#pragma weak mlib_VectorMul_S16_S8_Mod = __mlib_VectorMul_S16_S8_Mod
#pragma weak mlib_VectorMul_S16C_Sat = __mlib_VectorMul_S16C_Sat
#pragma weak mlib_VectorMul_S16_U8_Mod = __mlib_VectorMul_S16_U8_Mod
#pragma weak mlib_VectorMul_S16C_S8C_Mod = __mlib_VectorMul_S16C_S8C_Mod
#pragma weak mlib_VectorMul_S8_Mod = __mlib_VectorMul_S8_Mod
#pragma weak mlib_VectorMul_S16C_U8C_Mod = __mlib_VectorMul_S16C_U8C_Mod
#pragma weak mlib_VectorMul_U8_Mod = __mlib_VectorMul_U8_Mod
#pragma weak mlib_VectorMul_S8C_Mod = __mlib_VectorMul_S8C_Mod
#pragma weak mlib_VectorMul_S16_U8_Sat = __mlib_VectorMul_S16_U8_Sat
#pragma weak mlib_VectorMul_U8C_Mod = __mlib_VectorMul_U8C_Mod
#pragma weak mlib_VectorMul_S16C_S8C_Sat = __mlib_VectorMul_S16C_S8C_Sat
#pragma weak mlib_VectorMul_S8_Sat = __mlib_VectorMul_S8_Sat
#pragma weak mlib_VectorMul_S16C_U8C_Sat = __mlib_VectorMul_S16C_U8C_Sat
#pragma weak mlib_VectorMul_U8_Sat = __mlib_VectorMul_U8_Sat
#pragma weak mlib_VectorMul_S8_S8_Mod = __mlib_VectorMul_S8_S8_Mod
#pragma weak mlib_VectorMul_S8C_Sat = __mlib_VectorMul_S8C_Sat
#pragma weak mlib_VectorMul_U8C_Sat = __mlib_VectorMul_U8C_Sat
#pragma weak mlib_VectorMul_S32_Mod = __mlib_VectorMul_S32_Mod
#pragma weak mlib_VectorMul_U8_U8_Mod = __mlib_VectorMul_U8_U8_Mod
#pragma weak mlib_VectorMul_S16_Mod = __mlib_VectorMul_S16_Mod
#pragma weak mlib_VectorMul_S8C_S8C_Mod = __mlib_VectorMul_S8C_S8C_Mod
#pragma weak mlib_VectorMul_S8_S8_Sat = __mlib_VectorMul_S8_S8_Sat
#pragma weak mlib_VectorMul_U8C_U8C_Mod = __mlib_VectorMul_U8C_U8C_Mod
#pragma weak mlib_VectorMul_S32_Sat = __mlib_VectorMul_S32_Sat
#pragma weak mlib_VectorMul_U8_U8_Sat = __mlib_VectorMul_U8_U8_Sat
#pragma weak mlib_VectorMul_S16_Sat = __mlib_VectorMul_S16_Sat
#pragma weak mlib_VectorMul_S8C_S8C_Sat = __mlib_VectorMul_S8C_S8C_Sat
#pragma weak mlib_VectorMul_U8C_U8C_Sat = __mlib_VectorMul_U8C_U8C_Sat
#pragma weak mlib_VectorMul_S32_S32_Mod = __mlib_VectorMul_S32_S32_Mod
#pragma weak mlib_VectorMul_S32_S16_Mod = __mlib_VectorMul_S32_S16_Mod
#pragma weak mlib_VectorMul_S16_S16_Mod = __mlib_VectorMul_S16_S16_Mod
#pragma weak mlib_VectorMul_S32C_S32C_Mod = __mlib_VectorMul_S32C_S32C_Mod
#pragma weak mlib_VectorMul_S32C_S16C_Mod = __mlib_VectorMul_S32C_S16C_Mod
#pragma weak mlib_VectorMul_S32_S32_Sat = __mlib_VectorMul_S32_S32_Sat
#pragma weak mlib_VectorMul_S16C_S16C_Mod = __mlib_VectorMul_S16C_S16C_Mod
#pragma weak mlib_VectorMul_S32C_Mod = __mlib_VectorMul_S32C_Mod
#pragma weak mlib_VectorMul_S16_S16_Sat = __mlib_VectorMul_S16_S16_Sat
#pragma weak mlib_VectorMul_S32C_S32C_Sat = __mlib_VectorMul_S32C_S32C_Sat
#pragma weak mlib_VectorMul_S16C_Mod = __mlib_VectorMul_S16C_Mod

#elif defined(__GNUC__)

__typeof__(__mlib_VectorMul_S32C_S16C_Sat) mlib_VectorMul_S32C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorMul_S32C_S16C_Sat")));
__typeof__(__mlib_VectorMul_S16C_S16C_Sat) mlib_VectorMul_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorMul_S16C_S16C_Sat")));
__typeof__(__mlib_VectorMul_S32C_Sat) mlib_VectorMul_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorMul_S32C_Sat")));
__typeof__(__mlib_VectorMul_S16_S8_Mod) mlib_VectorMul_S16_S8_Mod
	__attribute__((weak, alias("__mlib_VectorMul_S16_S8_Mod")));
__typeof__(__mlib_VectorMul_S16C_Sat) mlib_VectorMul_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorMul_S16C_Sat")));
__typeof__(__mlib_VectorMul_S16_U8_Mod) mlib_VectorMul_S16_U8_Mod
	__attribute__((weak, alias("__mlib_VectorMul_S16_U8_Mod")));
__typeof__(__mlib_VectorMul_S16C_S8C_Mod) mlib_VectorMul_S16C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorMul_S16C_S8C_Mod")));
__typeof__(__mlib_VectorMul_S8_Mod) mlib_VectorMul_S8_Mod
	__attribute__((weak, alias("__mlib_VectorMul_S8_Mod")));
__typeof__(__mlib_VectorMul_S16C_U8C_Mod) mlib_VectorMul_S16C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorMul_S16C_U8C_Mod")));
__typeof__(__mlib_VectorMul_U8_Mod) mlib_VectorMul_U8_Mod
	__attribute__((weak, alias("__mlib_VectorMul_U8_Mod")));
__typeof__(__mlib_VectorMul_S8C_Mod) mlib_VectorMul_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorMul_S8C_Mod")));
__typeof__(__mlib_VectorMul_S16_U8_Sat) mlib_VectorMul_S16_U8_Sat
	__attribute__((weak, alias("__mlib_VectorMul_S16_U8_Sat")));
__typeof__(__mlib_VectorMul_U8C_Mod) mlib_VectorMul_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorMul_U8C_Mod")));
__typeof__(__mlib_VectorMul_S16C_S8C_Sat) mlib_VectorMul_S16C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorMul_S16C_S8C_Sat")));
__typeof__(__mlib_VectorMul_S8_Sat) mlib_VectorMul_S8_Sat
	__attribute__((weak, alias("__mlib_VectorMul_S8_Sat")));
__typeof__(__mlib_VectorMul_S16C_U8C_Sat) mlib_VectorMul_S16C_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorMul_S16C_U8C_Sat")));
__typeof__(__mlib_VectorMul_U8_Sat) mlib_VectorMul_U8_Sat
	__attribute__((weak, alias("__mlib_VectorMul_U8_Sat")));
__typeof__(__mlib_VectorMul_S8_S8_Mod) mlib_VectorMul_S8_S8_Mod
	__attribute__((weak, alias("__mlib_VectorMul_S8_S8_Mod")));
__typeof__(__mlib_VectorMul_S8C_Sat) mlib_VectorMul_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorMul_S8C_Sat")));
__typeof__(__mlib_VectorMul_U8C_Sat) mlib_VectorMul_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorMul_U8C_Sat")));
__typeof__(__mlib_VectorMul_S32_Mod) mlib_VectorMul_S32_Mod
	__attribute__((weak, alias("__mlib_VectorMul_S32_Mod")));
__typeof__(__mlib_VectorMul_U8_U8_Mod) mlib_VectorMul_U8_U8_Mod
	__attribute__((weak, alias("__mlib_VectorMul_U8_U8_Mod")));
__typeof__(__mlib_VectorMul_S16_Mod) mlib_VectorMul_S16_Mod
	__attribute__((weak, alias("__mlib_VectorMul_S16_Mod")));
__typeof__(__mlib_VectorMul_S8C_S8C_Mod) mlib_VectorMul_S8C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorMul_S8C_S8C_Mod")));
__typeof__(__mlib_VectorMul_S8_S8_Sat) mlib_VectorMul_S8_S8_Sat
	__attribute__((weak, alias("__mlib_VectorMul_S8_S8_Sat")));
__typeof__(__mlib_VectorMul_U8C_U8C_Mod) mlib_VectorMul_U8C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorMul_U8C_U8C_Mod")));
__typeof__(__mlib_VectorMul_S32_Sat) mlib_VectorMul_S32_Sat
	__attribute__((weak, alias("__mlib_VectorMul_S32_Sat")));
__typeof__(__mlib_VectorMul_U8_U8_Sat) mlib_VectorMul_U8_U8_Sat
	__attribute__((weak, alias("__mlib_VectorMul_U8_U8_Sat")));
__typeof__(__mlib_VectorMul_S16_Sat) mlib_VectorMul_S16_Sat
	__attribute__((weak, alias("__mlib_VectorMul_S16_Sat")));
__typeof__(__mlib_VectorMul_S8C_S8C_Sat) mlib_VectorMul_S8C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorMul_S8C_S8C_Sat")));
__typeof__(__mlib_VectorMul_U8C_U8C_Sat) mlib_VectorMul_U8C_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorMul_U8C_U8C_Sat")));
__typeof__(__mlib_VectorMul_S32_S32_Mod) mlib_VectorMul_S32_S32_Mod
	__attribute__((weak, alias("__mlib_VectorMul_S32_S32_Mod")));
__typeof__(__mlib_VectorMul_S32_S16_Mod) mlib_VectorMul_S32_S16_Mod
	__attribute__((weak, alias("__mlib_VectorMul_S32_S16_Mod")));
__typeof__(__mlib_VectorMul_S16_S16_Mod) mlib_VectorMul_S16_S16_Mod
	__attribute__((weak, alias("__mlib_VectorMul_S16_S16_Mod")));
__typeof__(__mlib_VectorMul_S32C_S32C_Mod) mlib_VectorMul_S32C_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorMul_S32C_S32C_Mod")));
__typeof__(__mlib_VectorMul_S32C_S16C_Mod) mlib_VectorMul_S32C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMul_S32C_S16C_Mod")));
__typeof__(__mlib_VectorMul_S32_S32_Sat) mlib_VectorMul_S32_S32_Sat
	__attribute__((weak, alias("__mlib_VectorMul_S32_S32_Sat")));
__typeof__(__mlib_VectorMul_S16C_S16C_Mod) mlib_VectorMul_S16C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMul_S16C_S16C_Mod")));
__typeof__(__mlib_VectorMul_S32C_Mod) mlib_VectorMul_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorMul_S32C_Mod")));
__typeof__(__mlib_VectorMul_S16_S16_Sat) mlib_VectorMul_S16_S16_Sat
	__attribute__((weak, alias("__mlib_VectorMul_S16_S16_Sat")));
__typeof__(__mlib_VectorMul_S32C_S32C_Sat) mlib_VectorMul_S32C_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorMul_S32C_S32C_Sat")));
__typeof__(__mlib_VectorMul_S16C_Mod) mlib_VectorMul_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMul_S16C_Mod")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

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

#define	MUL_S16_U8_SAT                                          \
	MUL_U8_U16;                                             \
	mask = vis_fcmplt16(dr, dzero);                         \
	mask1 = vis_fcmplt16(dr1, dzero)

/* *********************************************************** */

#define	MUL_S16C_U8C_SAT_IN_C                                          \
	{                                                              \
	    for (i = 0; i < n; i++) {                                  \
		re_x = (*px++);                                        \
		im_x = (*px++);                                        \
		re_y = (*py++);                                        \
		im_y = (*py++);                                        \
		re_c = re_x * re_y - im_x * im_y;                      \
		im_c = im_x * re_y + re_x * im_y;                      \
		(*pz++) =                                              \
			re_c > MLIB_S16_MAX ? MLIB_S16_MAX : (re_c <   \
			MLIB_S16_MIN ? MLIB_S16_MIN : re_c);           \
		(*pz++) = im_c > MLIB_S16_MAX ? MLIB_S16_MAX : im_c;   \
	    }                                                          \
	    return (MLIB_SUCCESS);                                     \
	}

/* *********************************************************** */

#define	MUL_S16C_S8C_SAT_IN_C                                          \
	{                                                              \
	    for (i = 0; i < n; i++) {                                  \
		re_x = (*px++);                                        \
		im_x = (*px++);                                        \
		re_y = (*py++);                                        \
		im_y = (*py++);                                        \
		re_c = re_x * re_y - im_x * im_y;                      \
		im_c = im_x * re_y + re_x * im_y;                      \
		(*pz++) = re_c;                                        \
		(*pz++) = im_c > MLIB_S16_MAX ? MLIB_S16_MAX : im_c;   \
	    }                                                          \
	    return (MLIB_SUCCESS);                                     \
	}

/* *********************************************************** */

#define	MUL_S16C_S8C_MOD_IN_C                                   \
	{                                                       \
	    for (i = 0; i < n; i++) {                           \
		re_x = (*px++);                                 \
		im_x = (*px++);                                 \
		re_y = (*py++);                                 \
		im_y = (*py++);                                 \
		re_c = re_x * re_y - im_x * im_y;               \
		im_c = im_x * re_y + re_x * im_y;               \
		(*pz++) = re_c;                                 \
		(*pz++) = im_c;                                 \
	    }                                                   \
	    return (MLIB_SUCCESS);                              \
	}

/* *********************************************************** */

#define	MUL_S32C_S16C_MOD_IN_C	MUL_S16C_S8C_MOD_IN_C

/* *********************************************************** */

#define	MUL_S32C_S16C_SAT_IN_C                                          \
	{                                                               \
	    for (i = 0; i < n; i++) {                                   \
		re_x = (*px++);                                         \
		im_x = (*px++);                                         \
		re_y = (*py++);                                         \
		im_y = (*py++);                                         \
		re_c = re_x * re_y - im_x * im_y;                       \
		im_c = im_x * re_y + re_x * im_y;                       \
		(*pz++) = re_c;                                         \
		(*pz++) = im_c == MLIB_S32_MIN ? MLIB_S32_MAX : im_c;   \
	    }                                                           \
	    return (MLIB_SUCCESS);                                      \
	}

/* *********************************************************** */

#define	PACK_U16_U8                                                  \
	mask_over =                                                  \
	vis_fcmpgt16(dzero, dr1) | (vis_fcmpgt16(dzero, dr) << 4);   \
	dr = vis_fpack16_to_hi(dr, dr);                              \
	dr = vis_fpack16_to_lo(dr, dr1)

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
	}

/* *********************************************************** */

#define	PACK_S16_S8_SAT                                         \
	dr = vis_fpadd16(displacement, dr);                     \
	dr1 = vis_fpadd16(displacement, dr1);                   \
	dr = vis_fpack16_to_hi(dr, dr);                         \
	dr = vis_fpack16_to_lo(dr, dr1);                        \
	dr = vis_fxor(dr, restore)

/* *********************************************************** */

#define	MUL_S16_S8_MOD                                          \
	{                                                       \
	    mlib_d64 dr2;                                       \
	                                                        \
	    dr2 = vis_fpmerge(vis_read_hi(dx), fzero);          \
	    dr = vis_fmul8x16(vis_read_hi(dy), dr2);            \
	    dr1 = vis_fpmerge(vis_read_lo(dx), fzero);          \
	    dr1 = vis_fmul8x16(vis_read_lo(dy), dr1);           \
	}

#define	MUL_S8_S16	MUL_S16_S8_SAT

/* *********************************************************** */

#define	PACK_S16_S8_MOD                                         \
	dr = vis_fand(dr, without_control);                     \
	dr1 = vis_fand(dr1, without_control);                   \
	dr = vis_fpack16_to_hi(dr, dr);                         \
	dr = vis_fpack16_to_lo(dr, dr1)

/* *********************************************************** */

#define	PACK_S32_S16_MOD                                             \
	{                                                            \
	    t = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));      \
	    t = vis_fpmerge(vis_read_lo(t), vis_read_hi(t));         \
	    t = vis_fpmerge(vis_read_hi(t), vis_read_lo(t));         \
	    t = vis_fand(t, mask_clear_lo);                          \
	    t1 = vis_freg_pair(vis_read_lo(dr), vis_read_lo(dr1));   \
	    t1 = vis_fandnot(mask_clear_lo, t1);                     \
	    dr = vis_for(t, t1);                                     \
	    vis_pst_16(dr, dpz, emask);                              \
	}

/* *********************************************************** */

#define	PACK_S32_S16_ST_MOD(i)                                       \
	{                                                            \
	    t = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));      \
	    t = vis_fpmerge(vis_read_lo(t), vis_read_hi(t));         \
	    t = vis_fpmerge(vis_read_hi(t), vis_read_lo(t));         \
	    t = vis_fand(t, mask_clear_lo);                          \
	    t1 = vis_freg_pair(vis_read_lo(dr), vis_read_lo(dr1));   \
	    t1 = vis_fandnot(mask_clear_lo, t1);                     \
	    dr = vis_for(t, t1);                                     \
	    dpz[i] = dr;                                             \
	}

/* *********************************************************** */

#define	PACK_S32_S16_SAT                                                 \
	mask_over = vis_fcmpeq16(dx, d_min) & vis_fcmpeq16(dy, d_min);   \
	dr = vis_fpadd32(dr, dr);                                        \
	dr1 = vis_fpadd32(dr1, dr1);                                     \
	dr = vis_fpackfix_pair(dr, dr1)

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

/* *********************************************************** */

static const mlib_u8 mlib_mask_16_8[] = {
	0, 1, 4, 5, 16, 17, 20, 21, 64, 65, 68, 69, 80, 81, 84, 85
};

/* *********************************************************** */

#define	PACK_S16C_S8C_SAT                                       \
	mask_r = mlib_mask_16_8[vis_fcmpeq16(dr, dover_r)];     \
	mask_i = mlib_mask_16_8[vis_fcmpeq16(dr1,               \
		dover_i) | vis_fcmpeq16(dr1, dover_r)];         \
	dr = vis_fpadd16(displacement, dr);                     \
	dr1 = vis_fpadd16(displacement, dr1);                   \
	dr = vis_fpack16_to_hi(dr, dr);                         \
	dr = vis_fpack16_to_lo(dr, dr1);                        \
	dr = vis_fxnor(dr, restore)

/* *********************************************************** */

#define	PACK_S16C_S8C0_SAT                                      \
	PACK_S16C_S8C_SAT;                                      \
	mask_r = (mask_r << 1) | mask_i;                        \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr))

/* *********************************************************** */

#define	PACK_S16C_S8C1_SAT                                       \
	PACK_S16C_S8C_SAT;                                       \
	mask_r |= mask_i >> 1;                                   \
	mask_i <<= 7;                                            \
	dr1 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr));     \
	fr2 = vis_fands(vis_read_hi(dr1), mask);                 \
	dr1 = vis_fpmerge(vis_read_lo(dr1), vis_read_lo(dr1));   \
	fr3 = vis_fandnots(mask, vis_read_hi(dr1));              \
	fr2 = vis_fors(fr3, fr2);                                \
	dr1 = vis_fpmerge(vis_read_lo(dr1), vis_read_lo(dr1));   \
	dr = vis_fpmerge(fr2, vis_read_hi(dr))

/* *********************************************************** */

#define	ST_S8C0_SAT	dpz[0] = dr; vis_pst_8(restore, dpz, mask_r)

/* *********************************************************** */

#define	ST_S8C1_SAT                                             \
	vis_pst_8(dr, dpz, 0x7f);                               \
	vis_pst_8(restore, dpz, mask_r);                        \
	dpz++;                                                  \
	vis_pst_8(dr1, dpz, 0x80);                              \
	vis_pst_8(restore, dpz, mask_i)

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

#define	ST_S8C0_MOD	dpz[0] = dr

/* *********************************************************** */

#define	ST_S8C1_MOD                                             \
	vis_pst_8(dr, dpz, 0x7f);                               \
	dpz++;                                                  \
	vis_pst_8(dr1, dpz, 0x80)

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
	}

/* *********************************************************** */

/*
 * Makes from 4_16==4*Re  and  4_16==4*Im --> dzh_4_16==Re1_Im1_Re2_Im2 and
 * dzl_4_16==Re3_Im3_Re4_Im4.
 */

/* *********************************************************** */

#define	COMPOSE_S16C_MOD                                             \
	{                                                            \
	    dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));      \
	    dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));      \
	    dr1 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));   \
	    dr1 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));   \
	    dr2 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));    \
	    dr3 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr1));    \
	    dzh = vis_fpmerge(vis_read_hi(dr2), vis_read_hi(dr3));   \
	    dzl = vis_fpmerge(vis_read_lo(dr2), vis_read_lo(dr3));   \
	}

/* *********************************************************** */

#define	COMPOSE_S16C_MOD_WITH_ODD                                    \
	{                                                            \
	    dr2 = vis_faligndata(rest, dr1);                         \
	    dr2 = vis_fpmerge(vis_read_hi(dr2), vis_read_lo(dr2));   \
	    dr2 = vis_fpmerge(vis_read_hi(dr2), vis_read_lo(dr2));   \
	    dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));      \
	    dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));      \
	    dr3 = vis_fpmerge(vis_read_hi(dr2), vis_read_hi(dr));    \
	    dr2 = vis_fpmerge(vis_read_lo(dr2), vis_read_lo(dr));    \
	    dzh = vis_fpmerge(vis_read_hi(dr3), vis_read_hi(dr2));   \
	    dzl = vis_fpmerge(vis_read_lo(dr3), vis_read_lo(dr2));   \
	    rest = dr1;                                              \
	}

/* *********************************************************** */

#define	COMPOSE_S16C_SAT                                        \
	{                                                       \
	    re2 = vis_faligndata(dr, dr);                       \
	    re3 = vis_faligndata(re2, re2);                     \
	    re4 = vis_faligndata(re3, re3);                     \
	    im2 = vis_faligndata(dr1, dr1);                     \
	    im3 = vis_faligndata(im2, im2);                     \
	    im4 = vis_faligndata(im3, im3);                     \
	    dzh = vis_faligndata(dzero, dr);                    \
	    dzh = vis_faligndata(dzh, dr1);                     \
	    dzh = vis_faligndata(dzh, re2);                     \
	    dzh = vis_faligndata(dzh, im2);                     \
	    dzl = vis_faligndata(dzero, re3);                   \
	    dzl = vis_faligndata(dzl, im3);                     \
	    dzl = vis_faligndata(dzl, re4);                     \
	    dzl = vis_faligndata(dzl, im4);                     \
	    mask = vis_fcmpeq16(dzh, cntr_ovlp);                \
	    mask1 = vis_fcmpeq16(dzl, cntr_ovlp);               \
	}

/* *********************************************************** */

#define	COMPOSE_S16C_SAT_WITH_ODD                               \
	{                                                       \
	    re2 = vis_faligndata(dr, dr);                       \
	    re3 = vis_faligndata(re2, re2);                     \
	    re4 = vis_faligndata(re3, re3);                     \
	    im2 = vis_faligndata(dr1, dr1);                     \
	    im3 = vis_faligndata(im2, im2);                     \
	    im4 = vis_faligndata(im3, im3);                     \
	    dzh = rest;                                         \
	    dzh = vis_faligndata(dzh, dr);                      \
	    dzh = vis_faligndata(dzh, dr1);                     \
	    dzh = vis_faligndata(dzh, re2);                     \
	    dzl = vis_faligndata(dzero, im2);                   \
	    dzl = vis_faligndata(dzl, re3);                     \
	    dzl = vis_faligndata(dzl, im3);                     \
	    dzl = vis_faligndata(dzl, re4);                     \
	    rest = vis_faligndata(dzero, im4);                  \
	    mask = vis_fcmpeq16(dzh, cntr_ovlp);                \
	    mask1 = vis_fcmpeq16(dzl, cntr_ovlp);               \
	}

/* *********************************************************** */

#define	PACK_S16C_U8C0                                          \
	dr = vis_fpack16_to_hi(dr, dr);                         \
	dr = vis_fpack16_to_lo(dr, dr1);                        \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr))

/* *********************************************************** */

#define	PACK_S16C_U8C1                                          \
	dr = vis_fpack16_to_hi(dr, dr);                         \
	dr = vis_fpack16_to_lo(dr, dr1);                        \
	dr1 = vis_faligndata(dr, dr);                           \
	dr = vis_fpmerge(vis_read_lo(dr1), vis_read_hi(dr))

#define	ST_U8C0	dpz[0] = dr

/* *********************************************************** */

#define	ST_U8C1                                                 \
	vis_pst_8(dr, dpz, 0x7f);                               \
	dpz++;                                                  \
	vis_pst_8(dr1, dpz, 0x80)

/* *********************************************************** */

#define	EXPAND_U8C_FOR_0_MUL                                           \
	{                                                              \
	    mlib_d64 dr22;                                             \
	    mlib_d64 fhi, flo;                                         \
	                                                               \
	    dr22 = vis_fand(dx, clear);                                \
	    dr2 = vis_fand(dx, clear);                                 \
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
dy = vis_fexpand(vis_read_lo(dy));

/* *********************************************************** */

#define	EXPAND_U8C_FOR_MUL_0_MOD                                       \
	{                                                              \
	    mlib_d64 dr22;                                             \
	    mlib_d64 fhi, flo;                                         \
	                                                               \
	    dr22 = vis_fand(dx, clear);                                \
	    dr2 = vis_fand(dx, clear);                                 \
	    dr22 = vis_fpack32(dr22, dzero);                           \
	    dr2 = vis_for(dr2, dr22);                                  \
	    fhi = vis_fpmerge(vis_read_hi(dr2), vis_read_hi(dr2));     \
	    flo = vis_fpmerge(vis_read_lo(dr2), vis_read_lo(dr2));     \
	    dr2 = vis_freg_pair(vis_read_hi(fhi), vis_read_hi(flo));   \
	    dy = vis_fpmerge(vis_read_hi(dy), vis_read_lo(dy));        \
	    dy = vis_fpmerge(vis_read_hi(dy), vis_read_lo(dy));        \
	    dr3 = vis_fpmerge(fzero, vis_read_hi(dy));                 \
	/*                                                             \
	 * y_r * 16                                                    \
	 */                                                            \
	    dy = vis_fpmerge(fzero, vis_read_lo(dy));                  \
	/*                                                             \
	 * y_i * 16                                                    \
	 */                                                            \
	}

/* *********************************************************** */

#define	EXPAND_U8C_FOR_MUL_MOD                                  \
	dr2 = vis_faligndata(dx, dx);                           \
	/*                                                      \
	 * x_r                                                  \
	 */                                                     \
	dy = vis_fpmerge(vis_read_hi(dy), vis_read_lo(dy));     \
	dy = vis_fpmerge(vis_read_hi(dy), vis_read_lo(dy));     \
	dr3 = vis_fpmerge(fzero, vis_read_hi(dy));              \
	/*                                                      \
	 * y_r * 16                                             \
	 */                                                     \
	                                                        \
	/* y_i * 16 */                                          \
dy = vis_fpmerge(fzero, vis_read_lo(dy));

/* *********************************************************** */

#define	MUL_S16C_U8C_FOR16_MOD                                           \
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
	}

/* *********************************************************** */

#define	COMPOSE_S16C_FROM32_MOD                                 \
	{                                                       \
	    dr = vis_fpack32(dr, dzero);                        \
	    dr = vis_fpack32(dr, dzero);                        \
	    dr1 = vis_fpack32(dr1, dzero);                      \
	    dr1 = vis_fpack32(dr1, dzero);                      \
	    dr = vis_fand(dr, const_clear);                     \
	    dr1 = vis_fand(dr1, const_clear);                   \
	    dr2 = vis_fandnot(const_clear, dr2);                \
	    dr3 = vis_fandnot(const_clear, dr3);                \
	    dzh = vis_for(dr, dr2);                             \
	    dzl = vis_for(dr1, dr3);                            \
	}

/* *********************************************************** */

#define	COMPOSE_S16C_FROM32_MOD_WITH_ODD                        \
	{                                                       \
	    COMPOSE_S16C_FROM32_MOD;                            \
	    dr = vis_faligndata(rest, dzh);                     \
	    rest = dzl;                                         \
	    dzl = vis_faligndata(dzh, rest);                    \
	    dzh = dr;                                           \
	}

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

#define	MUL_S16C_U8C_0_FOR16                                             \
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
	}

/* *********************************************************** */

#define	MUL_S16C_U8C_1_FOR16                                         \
	{                                                            \
	    dr = vis_fpackfix_pair(dr, dr1);                         \
	    dr1 = vis_fpackfix_pair(dr2, dr3);                       \
	    dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));      \
	    dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));      \
	    dr1 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));   \
	    dr1 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));   \
	    dr2 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));    \
	    dr3 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr1));    \
	    dzh = vis_fpmerge(vis_read_hi(dr2), vis_read_hi(dr3));   \
	    dzl = vis_fpmerge(vis_read_lo(dr2), vis_read_lo(dr3));   \
	}

/* *********************************************************** */

#define	MUL_S16C_U8C_FOR16                                               \
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
	    dr = vis_fpackfix_pair(dr, dr1);                             \
	    dr1 = vis_fpackfix_pair(dr2, dr3);                           \
	    dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));          \
	    dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));          \
	    dr1 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));       \
	    dr1 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));       \
	    dr2 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));        \
	    dr3 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr1));        \
	    dzh = vis_fpmerge(vis_read_hi(dr2), vis_read_hi(dr3));       \
	    dzl = vis_fpmerge(vis_read_lo(dr2), vis_read_lo(dr3));       \
	}

/* *********************************************************** */

#define	MUL_S16C_U8C_FOR16_WITH_0_ODD                                    \
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
	}

/* *********************************************************** */

#define	MUL_S16C_U8C_FOR16_WITH_1_ODD                                \
	{                                                            \
	    dr = vis_fpackfix_pair(dr, dr1);                         \
	    dr1 = vis_fpackfix_pair(dr2, dr3);                       \
	    dr2 = vis_faligndata(rest, dr1);                         \
	    dr2 = vis_fpmerge(vis_read_hi(dr2), vis_read_lo(dr2));   \
	    dr2 = vis_fpmerge(vis_read_hi(dr2), vis_read_lo(dr2));   \
	    dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));      \
	    dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));      \
	    dr3 = vis_fpmerge(vis_read_hi(dr2), vis_read_hi(dr));    \
	    dr2 = vis_fpmerge(vis_read_lo(dr2), vis_read_lo(dr));    \
	    dzh = vis_fpmerge(vis_read_hi(dr3), vis_read_hi(dr2));   \
	    dzl = vis_fpmerge(vis_read_lo(dr3), vis_read_lo(dr2));   \
	    rest = dr1;                                              \
	}

/* *********************************************************** */

#define	MUL_S16C_U8C_FOR16_WITH_ODD                                      \
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
	    dr = vis_fpackfix_pair(dr, dr1);                             \
	    dr1 = vis_fpackfix_pair(dr2, dr3);                           \
	    dr2 = vis_faligndata(rest, dr1);                             \
	    dr2 = vis_fpmerge(vis_read_hi(dr2), vis_read_lo(dr2));       \
	    dr2 = vis_fpmerge(vis_read_hi(dr2), vis_read_lo(dr2));       \
	    dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));          \
	    dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));          \
	    dr3 = vis_fpmerge(vis_read_hi(dr2), vis_read_hi(dr));        \
	    dr2 = vis_fpmerge(vis_read_lo(dr2), vis_read_lo(dr));        \
	    dzh = vis_fpmerge(vis_read_hi(dr3), vis_read_hi(dr2));       \
	    dzl = vis_fpmerge(vis_read_lo(dr3), vis_read_lo(dr2));       \
	    rest = dr1;                                                  \
	}

/* *********************************************************** */

#define	COMPOSE_S32C_FROM32_MOD                                 \
	dzh = vis_freg_pair(sr0, sr2);                          \
	dzl = vis_freg_pair(sr1, sr3)

/* *********************************************************** */

#define	COMPOSE_S32C_FROM32_MOD_WITH_ODD                        \
	dzh = vis_freg_pair(rest, sr0);                         \
	dzl = vis_freg_pair(sr2, sr1);                          \
	rest = sr3

/* *********************************************************** */

#define	COMPOSE_S32C_FROM32_SAT                                 \
	COMPOSE_S32C_FROM32_MOD;                                \
	mask = vis_fcmpeq32(cntr_ovlp, dzh);                    \
	mask1 = vis_fcmpeq32(cntr_ovlp, dzl)

/* *********************************************************** */

#define	COMPOSE_S32C_FROM32_SAT_WITH_ODD                        \
	COMPOSE_S32C_FROM32_MOD_WITH_ODD;                       \
	mask = vis_fcmpeq32(cntr_ovlp, dzh);                    \
	mask1 = vis_fcmpeq32(cntr_ovlp, dzl)

/* *********************************************************** */

#define	PACK_S32C_S16C0_MOD                                     \
	dr = vis_freg_pair(sr0, sr1);                           \
	dr1 = vis_freg_pair(sr2, sr3);                          \
	dr = vis_fpack32(dr, dzero);                            \
	dr = vis_fpack32(dr, dzero);                            \
	dr1 = vis_fand(without_control, dr1);                   \
	dr = vis_for(dr, dr1)

/* *********************************************************** */

#define	PACK_S32C_S16C1_MOD                                      \
	dr1 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));   \
	dr1 = vis_fandnot(without_control, dr1);                 \
	dr = vis_fand(without_control, dr);                      \
	dr = vis_for(dr, dr1)

/* *********************************************************** */

#define	ST_S16C0_MOD	dpz[0] = dr

/* *********************************************************** */

#define	ST_S16C1_MOD                                            \
	vis_pst_16(dr, dpz, 0x7);                               \
	dpz++;                                                  \
	vis_pst_16(dr, dpz, 0x8)

/* *********************************************************** */

#define	OVER_MUL_S16C_SAT                                              \
	mask_under =                                                   \
		(vis_fcmplt32(dr, d_min16) << 2) | vis_fcmplt32(dr1,   \
		d_min16);                                              \
	mask_over =                                                    \
		((vis_fcmpeq32(dr, d_min32) | vis_fcmpgt32(dr,         \
		d_max16)) << 2) | (vis_fcmpeq32(dr1,                   \
		d_min32) | vis_fcmpgt32(dr1, d_max16));                \
	dr = vis_fpadd32(dr, dr);                                      \
	dr1 = vis_fpadd32(dr1, dr1);                                   \
	dr = vis_fpackfix_pair(dr, dr1)

/* *********************************************************** */

#define	PACK_S32C_S16C0_SAT                                     \
	dr = vis_freg_pair(sr0, sr2);                           \
	dr1 = vis_freg_pair(sr1, sr3);                          \
	OVER_MUL_S16C_SAT

/* *********************************************************** */

#define	PACK_S32C_S16C1_SAT                                     \
	dr = vis_freg_pair(sr3, sr0);                           \
	dr1 = vis_freg_pair(sr2, sr1);                          \
	OVER_MUL_S16C_SAT

/* *********************************************************** */

#define	ST_S16C0_SAT                                            \
	dpz[0] = dr;                                            \
	vis_pst_16(d_under, dpz, mask_under);                   \
	vis_pst_16(d_over, dpz, mask_over)

/* *********************************************************** */

#define	ST_S16C1_SAT                                            \
	vis_pst_16(dr, dpz, 0x7);                               \
	vis_pst_16(d_under, dpz, mask_under & 0x7);             \
	vis_pst_16(d_over, dpz, mask_over & 0x7);               \
	dpz++;                                                  \
	vis_pst_16(dr, dpz, 0x8);                               \
	vis_pst_16(d_under, dpz, mask_under & 0x8);             \
	vis_pst_16(d_over, dpz, mask_over & 0x8)

/* *********************************************************** */

#define	MUL_S16C_S32C_1                                                \
	{                                                              \
	    mlib_d64 dr2, dr3, dr4, dr5;                               \
	                                                               \
	    dr2 = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dy));   \
	    dr3 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dy));   \
	    dr2 = vis_fpadd32(dr2, dr3);                               \
	    sr0 = vis_fpsub32s(vis_read_hi(dr2), vis_read_lo(dr2));    \
	    dr4 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dy));   \
	    dr5 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dy));   \
	    dr4 = vis_fpadd32(dr4, dr5);                               \
	    sr1 = vis_fpsub32s(vis_read_hi(dr4), vis_read_lo(dr4));    \
	}

/* *********************************************************** */

#define	MUL_S16C_S32C_0                                                 \
	{                                                               \
	    mlib_d64 dr6, dr7, dr8, dr9;                                \
	                                                                \
	    dr6 = vis_fpmerge(vis_read_hi(dy), vis_read_lo(dy));        \
	    dr6 = vis_fpmerge(vis_read_lo(dr6), vis_read_hi(dr6));      \
	    dr6 = vis_fpmerge(vis_read_hi(dr6), vis_read_lo(dr6));      \
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

#define	MUL_S16C_S32C                                                   \
	{                                                               \
	    mlib_d64 dr2, dr3, dr4, dr5, dr6, dr7, dr8, dr9;            \
	                                                                \
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

#else

#define	FLOAT2INT_CLAMP(DST, SRC)	DST = ((mlib_s32)(SRC))

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

mlib_status
__mlib_VectorMul_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask, len, i;

/* overflow masks */
	mlib_s32 mask_over;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s8 *pzend, *px = (mlib_s8 *)x, *py = (mlib_s8 *)y;
	mlib_d64 *dpz, *dpx, *dpy, *dpzend;
	mlib_d64 dx, dy, dr, dr1, dx0, dx1, dy0, dy1;
	mlib_s32 k, N;
	mlib_d64 x_buff[256];

	mlib_d64 restore = vis_to_double_dup(0x80808080);
	mlib_d64 d_over = vis_fone();
	mlib_d64 dzero = vis_fzero();
	mlib_f32 f127 = vis_to_float(0x7f00), fzero = vis_fzeros();

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);

	if (n <= 0)
		return (MLIB_FAILURE);

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
	PACK_U16_U8;
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
			MUL_U8_U16
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
				MUL_U8_U16;
				PACK_U16_U8;
				vis_pst_8(dr, dpz + k * 256 + i, ~mask_over);
				vis_pst_8(d_over, dpz + k * 256 + i, mask_over);
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

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_U8_Sat(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorMul_U8_U8_Sat(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return __mlib_VectorMul_S8_S8_Mod((mlib_s8 *)z, (mlib_s8 *)x,
		(mlib_s8 *)y, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16_U8_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dx0, dx1, dy0, dy1;
	mlib_d64 dr, dr1;
	mlib_u8 *px, *py;
	mlib_s16 *pz;
	mlib_s32 len = n, i, k, N;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_d64 restore = vis_to_double_dup(0x80808080);
	mlib_f32 f127 = vis_to_float(0x7f00), fzero = vis_fzeros();
	mlib_d64 x_buff[256];

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_u8 *)x;
	py = (mlib_u8 *)y;
	pz = (mlib_s16 *)z;

	if (n <= 8) {
		for (i = 0; i < n; i++) {
			(*pz++) = ((mlib_s32)(*px)) * (*py);
			px++;
			py++;
		}

		return (MLIB_SUCCESS);
	}

/*
 * prepare the destination address
 */

	while ((mlib_addr)pz & 7) {
		(*pz++) = ((mlib_s32)((*px++))) * ((*py++));
		len--;
	}

	dpz = (mlib_d64 *)pz;
	even_8 = len >> 3;
	rest_8 = len & 7;
	dpx = vis_alignaddr(px, 0);
	dpy = vis_alignaddr(py, 0);

	if (!(((mlib_addr)px | (mlib_addr)py) & 7)) {
/*
 * Both addresses are 8-byte aligned.
 * No  vis_alignaddr and  vis_faligndata at all.
 */
#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = dpx[i];
			dy = dpy[i];
			MUL_U8_U16;
			dpz[2 * i] = dr;
			dpz[2 * i + 1] = dr1;
		}
	} else {
		if (!((mlib_addr)px & 7)) {
/*
 * First ("x") address is 8-byte aligned.
 * vis_alignaddr and vis_faligndata only for "y".**
 */
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = dpx[i];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_U8_U16;
				dpz[2 * i] = dr;
				dpz[2 * i + 1] = dr1;
			}
		} else {
			if (!((mlib_addr)py & 7)) {
/*
 * Second ("y") address is 8-byte aligned.
 * vis_alignaddr and vis_faligndata only for "x".**
 */
				vis_alignaddr(px, 0);
#pragma pipeloop(0)
				for (i = 0; i < even_8; i++) {
					dy = dpy[i];
					dx0 = dpx[i];
					dx1 = vis_ld_d64_nf(dpx + i + 1);
					dx = vis_faligndata(dx0, dx1);
					MUL_U8_U16;
					dpz[2 * i] = dr;
					dpz[2 * i + 1] = dr1;
				}
			} else {
				if (!(((mlib_addr)px ^ (mlib_addr)py) & 7)) {
/*
 * Both ("x" and "y") address are identically aligned.
 * There are 1 vis_alignaddr and 2 vis_faligndata(s) in the loop.
 */
#pragma pipeloop(0)
					for (i = 0; i < even_8; i++) {
					    dx0 = dpx[i];
					    dx1 = vis_ld_d64_nf(dpx + i + 1);
					    dx = vis_faligndata(dx0, dx1);
					    dy0 = dpy[i];
					    dy1 = vis_ld_d64_nf(dpy + i + 1);
					    dy = vis_faligndata(dy0, dy1);
					    MUL_U8_U16;
					    dpz[2 * i] = dr;
					    dpz[2 * i + 1] = dr1;
					}
				} else {
/*
 * Both ("x" and "y") address are arbitrary aligned.
 * 2 vis_alignaddr(s) and 2 vis_faligndata(s) in the loop.
 */
/*
 * store 16 bytes of result
 */

					for (k = 0; k < (even_8 >> 8); k++) {
						__mlib_VectorCopy_U8((void *)
							x_buff,
							(void *)((mlib_d64 *)px
							+ k * 256), 256 * 8);
						(void *)vis_alignaddr(py, 0);
#pragma pipeloop(0)
						for (i = 0; i < 256; i++) {
						    dx = x_buff[i];
						    dy0 = dpy[k * 256 + i];
						    dy1 = vis_ld_d64_nf(dpy +
							k * 256 + i + 1);
						    dy = vis_faligndata(dy0,
							dy1);
						    MUL_U8_U16;
						    dpz[512 * k + 2 * i] = dr;
						    dpz[512 * k + 2 * i + 1] =
							dr1;
						}
					}

					N = even_8 & ~255;
					__mlib_VectorCopy_U8((void *)x_buff,
						(void *)((mlib_d64 *)px + N),
						(even_8 - N) * 8);
					vis_alignaddr(py, 0);
#pragma pipeloop(0)
					for (i = N; i < even_8; i++) {
					    dx = x_buff[i - N];
					    dy0 = dpy[i];
					    dy1 = vis_ld_d64_nf(dpy + i + 1);
					    dy = vis_faligndata(dy0, dy1);
					    MUL_U8_U16;
					    dpz[2 * i] = dr;
					    dpz[2 * i + 1] = dr1;
					}
				}
			}
		}
	}

	px += (even_8 << 3);
	py += (even_8 << 3);
	pz += (even_8 << 3);

	while (rest_8--)
		(*pz++) = ((mlib_s32)((*px++))) * ((*py++));
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16_U8_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dx0, dx1, dy0, dy1;
	mlib_d64 dr, dr1;
	mlib_u8 *px, *py;
	mlib_s16 *pz;
	mlib_s32 len = n, i, k, N;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_d64 restore = vis_to_double_dup(0x80808080);
	mlib_f32 f127 = vis_to_float(0x7f00);
	mlib_d64 fzero = vis_fzeros();
	mlib_d64 control_ovl =
		vis_to_double_dup((MLIB_S16_MAX << 16) | MLIB_S16_MAX);
	mlib_d64 dzero = vis_to_double_dup(0);

/* Masks for pstore16 in case of overflow. */
	mlib_s32 mask, mask1;
	mlib_d64 x_buff[256];

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_u8 *)x;
	py = (mlib_u8 *)y;
	pz = (mlib_s16 *)z;

	if (n <= 8) {
		mlib_s32 c;

		for (i = 0; i < n; i++) {
			(*pz++) = (c =
				((mlib_s32)((*px++))) * ((*py++))) >
				MLIB_S16_MAX ? MLIB_S16_MAX : c;
		}

		return (MLIB_SUCCESS);
	}

/*
 * prepare the destination address
 */

	while ((mlib_addr)pz & 7) {
		mlib_s32 c;

		(*pz++) = (c =
			((mlib_s32)((*px++))) * ((*py++))) >
			MLIB_S16_MAX ? MLIB_S16_MAX : c;
		len--;
	}

	dpz = (mlib_d64 *)pz;
	even_8 = len >> 3;
	rest_8 = len & 7;
	dpx = vis_alignaddr(px, 0);
	dpy = vis_alignaddr(py, 0);

	if (!(((mlib_addr)px | (mlib_addr)py) & 7)) {
/*
 * Both addresses are 8-byte aligned.
 * No  vis_alignaddr and  vis_faligndata at all.
 */
#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = dpx[i];
			dy = dpy[i];
			MUL_S16_U8_SAT;
			vis_pst_16(dr, dpz + 2 * i, ~mask);
			vis_pst_16(dr1, dpz + 2 * i + 1, ~mask1);
			vis_pst_16(control_ovl, dpz + 2 * i, mask);
			vis_pst_16(control_ovl, dpz + 2 * i + 1, mask1);
		}
	} else {
		if (!((mlib_addr)px & 7)) {
/*
 * First ("x") address is 8-byte aligned.
 * vis_alignaddr and vis_faligndata only for "y".**
 */
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = dpx[i];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S16_U8_SAT;
				vis_pst_16(dr, dpz + 2 * i, ~mask);
				vis_pst_16(dr1, dpz + 2 * i + 1, ~mask1);
				vis_pst_16(control_ovl, dpz + 2 * i, mask);
				vis_pst_16(control_ovl, dpz + 2 * i + 1, mask1);
			}
		} else {
			if (!((mlib_addr)py & 7)) {
/*
 * Second ("y") address is 8-byte aligned.
 * vis_alignaddr and vis_faligndata only for "x".**
 */
				vis_alignaddr(px, 0);
#pragma pipeloop(0)
				for (i = 0; i < even_8; i++) {
					dy = dpy[i];
					dx0 = dpx[i];
					dx1 = vis_ld_d64_nf(dpx + i + 1);
					dx = vis_faligndata(dx0, dx1);
					MUL_S16_U8_SAT;
					vis_pst_16(dr, dpz + 2 * i, ~mask);
					vis_pst_16(dr1, dpz + 2 * i + 1,
						~mask1);
					vis_pst_16(control_ovl, dpz + 2 * i,
						mask);
					vis_pst_16(control_ovl, dpz + 2 * i + 1,
						mask1);
				}
			} else {
				if (!(((mlib_addr)px ^ (mlib_addr)py) & 7)) {
/*
 * Both ("x" and "y") address are identically aligned.
 * There are 1 vis_alignaddr and 2 vis_faligndata(s) in the loop.
 */
#pragma pipeloop(0)
					for (i = 0; i < even_8; i++) {
					    dx0 = dpx[i];
					    dx1 = vis_ld_d64_nf(dpx + i + 1);
					    dx = vis_faligndata(dx0, dx1);
					    dy0 = dpy[i];
					    dy1 = vis_ld_d64_nf(dpy + i + 1);
					    dy = vis_faligndata(dy0, dy1);
					    MUL_S16_U8_SAT;
					    vis_pst_16(dr, dpz + 2 * i,
						~mask);
					    vis_pst_16(dr1, dpz + 2 * i + 1,
						~mask1);
					    vis_pst_16(control_ovl,
						dpz + 2 * i, mask);
					    vis_pst_16(control_ovl,
						dpz + 2 * i + 1, mask1);
					}
				} else {
/*
 * Both ("x" and "y") address are arbitrary aligned.
 * 2 vis_alignaddr(s) and 2 vis_faligndata(s) in the loop.
 */
/*
 * store 16 bytes of result
 */

					for (k = 0; k < (even_8 >> 8); k++) {
						__mlib_VectorCopy_U8((void *)
							x_buff,
							(void *)((mlib_d64 *)px
							+ k * 256), 256 * 8);
						(void *)vis_alignaddr(py, 0);
#pragma pipeloop(0)
						for (i = 0; i < 256; i++) {
						    dx = x_buff[i];
						    dy0 = dpy[k * 256 + i];
						    dy1 = vis_ld_d64_nf(dpy +
							k * 256 + i + 1);
						    dy = vis_faligndata(dy0,
							dy1);
						    MUL_S16_U8_SAT;
						    vis_pst_16(dr,
							dpz + 512 * k +
							2 * i, ~mask);
						    vis_pst_16(dr1,
							dpz + 512 * k +
							2 * i + 1, ~mask1);
						    vis_pst_16(control_ovl,
							dpz + 512 * k +
							2 * i, mask);
						    vis_pst_16(control_ovl,
							dpz + 512 * k +
							2 * i + 1, mask1);
						}
					}

					N = even_8 & ~255;
					__mlib_VectorCopy_U8((void *)x_buff,
						(void *)((mlib_d64 *)px + N),
						(even_8 - N) * 8);
					vis_alignaddr(py, 0);
#pragma pipeloop(0)
					for (i = N; i < even_8; i++) {
					    dx = x_buff[i - N];
					    dy0 = dpy[i];
					    dy1 = vis_ld_d64_nf(dpy + i + 1);
					    dy = vis_faligndata(dy0, dy1);
					    MUL_S16_U8_SAT;
					    vis_pst_16(dr, dpz + 2 * i,
						~mask);
					    vis_pst_16(dr1, dpz + 2 * i + 1,
						~mask1);
					    vis_pst_16(control_ovl,
						dpz + 2 * i, mask);
					    vis_pst_16(control_ovl,
						dpz + 2 * i + 1, mask1);
					}
				}
			}
		}
	}

	px += (even_8 << 3);
	py += (even_8 << 3);
	pz += (even_8 << 3);

	while (rest_8--) {
		mlib_s32 c;

		(*pz++) = (c =
			((mlib_s32)((*px++))) * ((*py++))) >
			MLIB_S16_MAX ? MLIB_S16_MAX : c;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_U8_Mod(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorMul_U8_U8_Mod(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S8_S8_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
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

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);

	if (n <= 0)
		return (MLIB_FAILURE);

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
	MUL_S16_S8_MOD;
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
				MUL_S16_S8_MOD;
				PACK_S16_S8_MOD;
				dpz[i] = dr;
			}

			dpz = dpzend;
			dx = dpx[len];
			dy = dpy[len];
			MUL_S16_S8_MOD;
		} else {
#pragma pipeloop(0)
			for (i = 0; i < len; i++) {
				dx = dpx[i];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S16_S8_MOD;
				PACK_S16_S8_MOD;
				dpz[i] = dr;
			}

			dpz = dpzend;
			dx = dpx[len];
			dy0 = dpy[len];
			dy1 = vis_ld_d64_nf(dpy + len + 1);
			dy = vis_faligndata(dy0, dy1);
			MUL_S16_S8_MOD;
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
				MUL_S16_S8_MOD;
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
			MUL_S16_S8_MOD;
			PACK_S16_S8_MOD;
			dpz[i] = dr;
		}

		dpz = dpzend;
		dx = x_buff[len - N];
		dy0 = vis_ld_d64_nf(dpy + len);
		dy1 = vis_ld_d64_nf(dpy + len + 1);
		dy = vis_faligndata(dy0, dy1);
		MUL_S16_S8_MOD;
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

mlib_status
__mlib_VectorMul_S16_S8_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dx0, dx1, dy0, dy1;
	mlib_d64 dr, dr1;
	mlib_s8 *px, *py;
	mlib_s16 *pz;
	mlib_s32 len = n, i, k, N;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_f32 fzero = vis_fzeros();
	mlib_d64 x_buff[256];

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s8 *)x;
	py = (mlib_s8 *)y;
	pz = (mlib_s16 *)z;

	if (n <= 8) {
		for (i = 0; i < n; i++) {
			(*pz++) = ((mlib_s32)(*px)) * (*py);
			px++;
			py++;
		}

		return (MLIB_SUCCESS);
	}

/*
 * prepare the destination address
 */

	while ((mlib_addr)pz & 7) {
		(*pz++) = ((mlib_s32)((*px++))) * ((*py++));
		len--;
	}

	dpz = (mlib_d64 *)pz;
	even_8 = len >> 3;
	rest_8 = len & 7;
	dpx = vis_alignaddr(px, 0);
	dpy = vis_alignaddr(py, 0);

	if (!(((mlib_addr)px | (mlib_addr)py) & 7)) {
/*
 * Both addresses are 8-byte aligned.
 * No  vis_alignaddr and  vis_faligndata at all.
 */
#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = dpx[i];
			dy = dpy[i];
			MUL_S8_S16;
			dpz[2 * i] = dr;
			dpz[2 * i + 1] = dr1;
		}
	} else {
		if (!((mlib_addr)px & 7)) {
/*
 * First ("x") address is 8-byte aligned.
 * vis_alignaddr and vis_faligndata only for "y".**
 */
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = dpx[i];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S8_S16;
				dpz[2 * i] = dr;
				dpz[2 * i + 1] = dr1;
			}
		} else {
			if (!((mlib_addr)py & 7)) {
/*
 * Second ("y") address is 8-byte aligned.
 * vis_alignaddr and vis_faligndata only for "x".**
 */
				vis_alignaddr(px, 0);
#pragma pipeloop(0)
				for (i = 0; i < even_8; i++) {
					dy = dpy[i];
					dx0 = dpx[i];
					dx1 = vis_ld_d64_nf(dpx + i + 1);
					dx = vis_faligndata(dx0, dx1);
					MUL_S8_S16;
					dpz[2 * i] = dr;
					dpz[2 * i + 1] = dr1;
				}
			} else {
				if (!(((mlib_addr)px ^ (mlib_addr)py) & 7)) {
/*
 * Both ("x" and "y") address are identically aligned.
 * There are 1 vis_alignaddr and 2 vis_faligndata(s) in the loop.
 */
#pragma pipeloop(0)
					for (i = 0; i < even_8; i++) {
					    dx0 = dpx[i];
					    dx1 = vis_ld_d64_nf(dpx + i + 1);
					    dx = vis_faligndata(dx0, dx1);
					    dy0 = dpy[i];
					    dy1 = vis_ld_d64_nf(dpy + i + 1);
					    dy = vis_faligndata(dy0, dy1);
					    MUL_S8_S16;
					    dpz[2 * i] = dr;
					    dpz[2 * i + 1] = dr1;
					}
				} else {
/*
 * Both ("x" and "y") address are arbitrary aligned.
 * 2 vis_alignaddr(s) and 2 vis_faligndata(s) in the loop.
 * store 16 bytes of result
 */

					for (k = 0; k < (even_8 >> 8); k++) {
						__mlib_VectorCopy_U8((void *)
							x_buff,
							(void *)((mlib_d64 *)px
							+ k * 256), 256 * 8);
						(void *)vis_alignaddr(py, 0);
#pragma pipeloop(0)
						for (i = 0; i < 256; i++) {
						    dx = x_buff[i];
						    dy0 = dpy[k * 256 + i +
							0];
						    dy1 = vis_ld_d64_nf(dpy +
							k * 256 + i + 1);
						    dy = vis_faligndata(dy0,
							dy1);
						    MUL_S8_S16;
						    dpz[512 * k + 2 * i +
							0] = dr;
						    dpz[512 * k + 2 * i +
							1] = dr1;
						}
					}

					N = even_8 & ~255;
					__mlib_VectorCopy_U8((void *)x_buff,
						(void *)((mlib_d64 *)px + N),
						(even_8 - N) * 8);
					vis_alignaddr(py, 0);
#pragma pipeloop(0)
					for (i = N; i < even_8; i++) {
					    dx = x_buff[i - N];
					    dy0 = dpy[i];
					    dy1 = vis_ld_d64_nf(dpy + i + 1);
					    dy = vis_faligndata(dy0, dy1);
					    MUL_S8_S16;
					    dpz[2 * i] = dr;
					    dpz[2 * i + 1] = dr1;
					}
				}
			}
		}
	}

	px += (even_8 << 3);
	py += (even_8 << 3);
	pz += (even_8 << 3);

	while (rest_8--)
		(*pz++) = ((mlib_s32)((*px++))) * ((*py++));
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S8_Mod(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorMul_S8_S8_Mod(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
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

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);

	if (n <= 0)
		return (MLIB_FAILURE);

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
		MUL_S16_S8_SAT} else {
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

mlib_status
__mlib_VectorMul_S8_Sat(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorMul_S8_S8_Sat(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s16 *pzend, *px = (void *)x, *py = (void *)y;
	mlib_d64 *dpz, *dpx, *dpy, *dpzend;
	mlib_d64 dx, dy, dr, dr1, dx0, dx1, dy0, dy1;
	mlib_d64 mask_clear_lo = vis_to_double_dup(0xFFFF0000);
	mlib_d64 t, t1;

	mlib_s32 k, N, i, len;
	mlib_d64 x_buff[256];

/* initialize GSR scale factor */
	vis_write_gsr(15 << 3);

	if (n <= 0)
		return (MLIB_FAILURE);

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
	MUL_S16_S32;
	PACK_S32_S16_MOD;
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
				MUL_S16_S32;
				PACK_S32_S16_ST_MOD(i);
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
				MUL_S16_S32;
				PACK_S32_S16_ST_MOD(i);
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
				(void *)((mlib_d64 *)px + k * 256), 256 * 8);
			(void *)vis_alignaddr(py, off);
#pragma pipeloop(0)
			for (i = 0; i < 256; i++) {
				dx = x_buff[i];
				dy0 = dpy[k * 256 + i];
				dy1 = vis_ld_d64_nf(dpy + k * 256 + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S16_S32;
				PACK_S32_S16_ST_MOD(k * 256 + i);
			}
		}

		N = len & ~255;
		__mlib_VectorCopy_U8((void *)x_buff,
			(void *)((mlib_d64 *)px + N), (len - N) * 8 +
			2 + ((mlib_addr)pzend & 7));
		vis_alignaddr(py, off);
#pragma pipeloop(0)
		for (i = N; i < len; i++) {
			dx = x_buff[i - N];
			dy0 = dpy[i];
			dy1 = vis_ld_d64_nf(dpy + i + 1);
			dy = vis_faligndata(dy0, dy1);
			MUL_S16_S32;
			PACK_S32_S16_ST_MOD(i);
		}

		dpz = dpzend;
		dx = x_buff[len - N];
		dy0 = vis_ld_d64_nf(dpy + len);
		dy1 = vis_ld_d64_nf(dpy + len + 1);
		dy = vis_faligndata(dy0, dy1);
	}

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
/* prepare edge mask for the last bytes */
		emask = vis_edge16(dpz, pzend);
		MUL_S16_S32;
		PACK_S32_S16_MOD;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S32_S16_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dx0, dx1, dy0, dy1;
	mlib_d64 dr1, dr;
	mlib_s16 *px, *py;
	mlib_s32 *pz;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_s32 len = n, i, k, N;
	mlib_d64 x_buff[256];

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s16 *)x;
	py = (mlib_s16 *)y;
	pz = (mlib_s32 *)z;

	if (n <= 4) {
		for (i = 0; i < n; i++) {
			(*pz++) = ((mlib_s32)(*px)) * (*py);
			px++;
			py++;
		}

		return (MLIB_SUCCESS);
	}

/*
 * prepare the destination address
 */

	if ((mlib_addr)pz & 7) {
		(*pz++) = ((mlib_s32)(*px)) * (*py);
		px++;
		py++;
		len--;
	}

	dpz = (mlib_d64 *)pz;

	even_8 = len >> 2;
	rest_8 = len & 0x3;
	dpx = vis_alignaddr(px, 0);
	dpy = vis_alignaddr(py, 0);

	if (!(((mlib_addr)px | (mlib_addr)py) & 7)) {
/*
 * Both addresses are 8-byte aligned.
 * No  vis_alignaddr and  vis_faligndata at all.
 */
#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = dpx[i];
			dy = dpy[i];
			MUL_S16_S32;
			dpz[2 * i] = dr;
			dpz[2 * i + 1] = dr1;
		}
	} else {
		if (!((mlib_addr)px & 7)) {
/*
 * First ("x") address is 8-byte aligned.
 * vis_alignaddr and vis_faligndata only for "y".**
 */
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = dpx[i];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S16_S32;
				dpz[2 * i] = dr;
				dpz[2 * i + 1] = dr1;
			}
		} else {
			if (!((mlib_addr)py & 7)) {
/*
 * Second ("y") address is 8-byte aligned.
 * vis_alignaddr and vis_faligndata only for "x".**
 */
				vis_alignaddr(px, 0);
#pragma pipeloop(0)
				for (i = 0; i < even_8; i++) {
					dy = dpy[i];
					dx0 = dpx[i];
					dx1 = vis_ld_d64_nf(dpx + i + 1);
					dx = vis_faligndata(dx0, dx1);
					MUL_S16_S32;
					dpz[2 * i] = dr;
					dpz[2 * i + 1] = dr1;
				}
			} else {
				if (!(((mlib_addr)px ^ (mlib_addr)py) & 7)) {
/*
 * Both ("x" and "y") address are identically aligned.
 * There are 1 vis_alignaddr and 2 vis_faligndata(s) in the loop.
 */
/*
 * vis_alignaddr(px, 0);
 */
#pragma pipeloop(0)
					for (i = 0; i < even_8; i++) {
					    dx0 = dpx[i];
					    dx1 = vis_ld_d64_nf(dpx + i + 1);
					    dx = vis_faligndata(dx0, dx1);
					    dy0 = dpy[i];
					    dy1 = vis_ld_d64_nf(dpy + i + 1);
					    dy = vis_faligndata(dy0, dy1);
					    MUL_S16_S32;
					    dpz[2 * i] = dr;
					    dpz[2 * i + 1] = dr1;
					}
				} else {
/*
 * Both ("x" and "y") address are arbitrary aligned.
 * 2 vis_alignaddr(s) and 2 vis_faligndata(s) in the loop.
 */
/*
 * store 16 bytes of result
 */

					for (k = 0; k < (even_8 >> 8); k++) {
						__mlib_VectorCopy_U8((void *)
							x_buff,
							(void *)((mlib_d64 *)px
							+ k * 256), 256 * 8);
						(void *)vis_alignaddr(py, 0);
#pragma pipeloop(0)
						for (i = 0; i < 256; i++) {
						    dx = x_buff[i];
						    dy0 = dpy[k * 256 + i];
						    dy1 = vis_ld_d64_nf(dpy +
							k * 256 + i + 1);
						    dy = vis_faligndata(dy0,
							dy1);
						    MUL_S16_S32;
						    dpz[512 * k + 2 * i +
							0] = dr;
						    dpz[512 * k + 2 * i +
							1] = dr1;
						}
					}

					N = even_8 & ~255;
					__mlib_VectorCopy_U8((void *)x_buff,
						(void *)((mlib_d64 *)px + N),
						(even_8 - N) * 8);
					vis_alignaddr(py, 0);
#pragma pipeloop(0)
					for (i = N; i < even_8; i++) {
					    dx = x_buff[i - N];
					    dy0 = dpy[i];
					    dy1 = vis_ld_d64_nf(dpy + i + 1);
					    dy = vis_faligndata(dy0, dy1);
					    MUL_S16_S32;
					    dpz[2 * i] = dr;
					    dpz[2 * i + 1] = dr1;
					}
				}
			}
		}
	}

	if (!rest_8)
		return (MLIB_SUCCESS);

	px += (even_8 << 2);
	py += (even_8 << 2);
	pz += (even_8 << 2);

	while (rest_8--) {
		(*pz++) = ((mlib_s32)(*px)) * (*py);
		px++;
		py++;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16_Mod(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorMul_S16_S16_Mod(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* overflow masks */
	mlib_s32 mask_over;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s16 *pzend, *px = (void *)x, *py = (void *)y;
	mlib_d64 *dpz, *dpx, *dpy, *dpzend;
	mlib_d64 dx, dy, dr, dr1, dx0, dx1, dy0, dy1;

	mlib_d64 d_min = vis_to_double_dup(0x80008000);
	mlib_d64 d_over = vis_fnot(d_min);
	mlib_s32 k, N, i, len;
	mlib_d64 x_buff[256];

/* initialize GSR scale factor */
	vis_write_gsr(15 << 3);

	if (n <= 0)
		return (MLIB_FAILURE);

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
	MUL_S16_S32;
	PACK_S32_S16_SAT;
	vis_pst_16(dr, dpz, emask);
	vis_pst_16(d_over, dpz, emask & mask_over);
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
				MUL_S16_S32;
				PACK_S32_S16_SAT;
				vis_pst_16(d_over, dpz + i, mask_over);
				vis_pst_16(dr, dpz + i, ~mask_over);
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
				MUL_S16_S32;
				PACK_S32_S16_SAT;
				vis_pst_16(d_over, dpz + i, mask_over);
				vis_pst_16(dr, dpz + i, ~mask_over);
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
				(void *)((mlib_d64 *)px + k * 256), 256 * 8);
			(void *)vis_alignaddr(py, off);
#pragma pipeloop(0)
			for (i = 0; i < 256; i++) {
				dx = x_buff[i];
				dy0 = dpy[k * 256 + i];
				dy1 = vis_ld_d64_nf(dpy + k * 256 + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S16_S32;
				PACK_S32_S16_SAT;
				vis_pst_16(d_over, dpz + k * 256 + i,
					mask_over);
				vis_pst_16(dr, dpz + k * 256 + i, ~mask_over);
			}
		}

		N = len & ~255;
		__mlib_VectorCopy_U8((void *)x_buff,
			(void *)((mlib_d64 *)px + N), (len - N) * 8 +
			2 + ((mlib_addr)pzend & 7));
		vis_alignaddr(py, off);
#pragma pipeloop(0)
		for (i = N; i < len; i++) {
			dx = x_buff[i - N];
			dy0 = dpy[i];
			dy1 = vis_ld_d64_nf(dpy + i + 1);
			dy = vis_faligndata(dy0, dy1);
			MUL_S16_S32;
			PACK_S32_S16_SAT;
			vis_pst_16(d_over, dpz + i, mask_over);
			vis_pst_16(dr, dpz + i, ~mask_over);
		}

		dpz = dpzend;
		dx = x_buff[len - N];
		dy0 = vis_ld_d64_nf(dpy + len);
		dy1 = vis_ld_d64_nf(dpy + len + 1);
		dy = vis_faligndata(dy0, dy1);
	}

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
/* prepare edge mask for the last bytes */
		emask = vis_edge16(dpz, pzend);
		MUL_S16_S32;
		PACK_S32_S16_SAT;
		vis_pst_16(dr, dpz, emask);
		vis_pst_16(d_over, dpz, emask & mask_over);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16_Sat(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorMul_S16_S16_Sat(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S32_S32_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	mlib_s32 *px = (mlib_s32 *)x;
	mlib_s32 *py = (mlib_s32 *)y;
	mlib_s32 *pz = (mlib_s32 *)z;
	mlib_s32 i;
	mlib_d64 d16 = 0x10000, _d16 = 1 / d16;
	mlib_d64 d15 = d16 * 0.5;
	mlib_d64 d31 = d16 * d15, _d31 = 1 / d31;
	mlib_d64 mul_hi, mul_lo, tmp;
	mlib_s32 sx, sy, one;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		mul_hi = ((mlib_s16 *)(py + i))[0];
		mul_lo = ((mlib_u16 *)(py + i))[1];

		sy = py[i];
		sx = px[i];
		one = sy & sx & 1;

		mul_hi *= (mlib_d64)sx;
		mul_lo = (mul_lo * (mlib_d64)sx - one) * 0.5;

		FLOAT2INT_CLAMP(tmp, mul_hi * _d16);
		mul_hi = mul_hi * d15 - tmp * d31 + mul_lo;
		FLOAT2INT_CLAMP(tmp, mul_hi * _d31);
		mul_hi = mul_hi - tmp * d31;
		pz[i] = ((mlib_s32)mul_hi << 1) | one;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S32_Mod(
	mlib_s32 *xz,
	const mlib_s32 *y,
	mlib_s32 n)
{
	return (__mlib_VectorMul_S32_S32_Mod(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S32_S32_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	mlib_s32 *px = (mlib_s32 *)x, *py = (mlib_s32 *)y, *pz = (mlib_s32 *)z;
	mlib_d64 mul;
	mlib_s32 i;

	for (i = 0; i < n; i++) {
		mul = px[i] * (mlib_d64)py[i];
		FLOAT2INT_CLAMP(pz[i], mul);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S32_Sat(
	mlib_s32 *xz,
	const mlib_s32 *y,
	mlib_s32 n)
{
	return (__mlib_VectorMul_S32_S32_Sat(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_U8C_U8C_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask, len, i;
	mlib_s32 gsr_for_mul = (12 << 3) | 7;

/* offset of address alignment in destination */
	mlib_s32 gsr_for_pack = (7 << 3) | 7, off;
	mlib_u8 *pzend, *px = (void *)x;
	mlib_d64 *dpz, *dpx, *dpy, *dpzend;
	mlib_d64 dx, dy, dr, dr1, dr2, dr3, dx0, dx1, dy0, dy1;
	mlib_d64 clear = vis_to_double_dup(0xFF00FF00);
	mlib_d64 dzero = vis_fzero();
	mlib_s32 N, k;
	mlib_d64 x_buff[256], r_buff[256];

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
					(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
					for (i = 0; i < 256; i++) {
						dx = dpx[256 * k + i];
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
/* initialize GSR */
				vis_write_gsr(gsr_for_mul);
				(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
				for (i = 0; i < len - N; i++) {
					dx = dpx[i + N];
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

				dpx += len;
				dpy += len;
				dpz += len;
				dx = dpx[0];
				(void *)vis_alignaddr((void *)y, off);
				dy0 = dpy[0];
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
				(len - N) * 8 + 1 + ((mlib_addr)pzend & 7));
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
	__mlib_VectorCopy_U8((void *)x_buff, (void *)((mlib_d64 *)px + N),
		(len - N) * 8 + ((mlib_addr)pzend & 7));
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

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16C_U8C_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dy0, dy1;
	mlib_d64 dr, dr1, dr2, dr3, dzh, dzl;
	mlib_s32 gsr_for_mul = (12 << 3) | 7;
	mlib_u8 *px, *py;
	mlib_s16 *pz;

/* odd = 1 means "aligned" resultant vector starts with Im. part. */
	mlib_s32 odd = 0;
	mlib_s32 len = n, i;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_d64 dzero = vis_to_double_dup(0), rest;
	mlib_s32 re_x, im_x, re_y, im_y, re_c, im_c;
	mlib_d64 clear = vis_to_double_dup(0xFF00FF00);
	mlib_d64 buff0[256], buff1[256], buff2[256], buff3[256];
	mlib_s32 N, k;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_u8 *)x;
	py = (mlib_u8 *)y;
	pz = (mlib_s16 *)z;

	if (n <= 8)
		MUL_S16C_U8C_SAT_IN_C;

/*
 * prepare the destination address trying to align it for 8 bytes.
 */

	while ((mlib_addr)pz & 7) {
		re_x = (*px++);
		im_x = (*px++);
		re_y = (*py++);
		im_y = (*py++);
		re_c = re_x * re_y - im_x * im_y;
		im_c = im_x * re_y + re_x * im_y;
		len--;
		(*pz++) =
			re_c > MLIB_S16_MAX ? MLIB_S16_MAX : (re_c <
			MLIB_S16_MIN ? MLIB_S16_MIN : re_c);
		im_c = im_c > MLIB_S16_MAX ? MLIB_S16_MAX : (im_c <
			MLIB_S16_MIN ? MLIB_S16_MIN : im_c);

		if (!((mlib_addr)pz & 7)) {
			odd = 1;
			rest = vis_to_double(0, im_c);
			break;
		}

		(*pz++) = im_c;
	}

	dpz = (mlib_d64 *)pz;
	len <<= 1;
	even_8 = len >> 3;
	rest_8 = len & 0x7;
	pz += (even_8 << 3);
/* initialize GSR */
	vis_write_gsr(gsr_for_mul);

	if (!(((mlib_addr)px | (mlib_addr)py) & 7)) {

/*
 * Both addresses are 8-byte aligned.
 * No  vis_alignaddr and  vis_faligndata at all.
 */

		dpx = (mlib_d64 *)px;
		dpy = (mlib_d64 *)py;

		if (!odd) {

			for (k = 0; k < (even_8 >> 8); k++) {
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = dpx[256 * k + i];
					dy = dpy[256 * k + i];
					EXPAND_U8C_FOR_MUL;
					MUL_S16C_U8C_0_FOR16;
					buff0[i] = dr;
					buff1[i] = dr1;
					buff2[i] = dr2;
					buff3[i] = dr3;
				}

#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dr = buff0[i];
					dr1 = buff1[i];
					dr2 = buff2[i];
					dr3 = buff3[i];
					MUL_S16C_U8C_1_FOR16;
					dpz[512 * k + 2 * i] = dzh;
					dpz[512 * k + 2 * i + 1] = dzl;
				}
			}

			N = even_8 & ~255;
#pragma pipeloop(0)
			for (i = 0; i < even_8 - N; i++) {
				dx = dpx[i + N];
				dy = dpy[i + N];
				EXPAND_U8C_FOR_MUL;
				MUL_S16C_U8C_0_FOR16;
				buff0[i] = dr;
				buff1[i] = dr1;
				buff2[i] = dr2;
				buff3[i] = dr3;
			}

#pragma pipeloop(0)
			for (i = 0; i < even_8 - N; i++) {
				dr = buff0[i];
				dr1 = buff1[i];
				dr2 = buff2[i];
				dr3 = buff3[i];
				MUL_S16C_U8C_1_FOR16;
				dpz[2 * N + 2 * i] = dzh;
				dpz[2 * N + 2 * i + 1] = dzl;
			}
		} else {

			for (k = 0; k < (even_8 >> 8); k++) {
#pragma pipeloop(0)
				(void *)vis_alignaddr(dpx, 7);
				for (i = 0; i < 256; i++) {
					dx = dpx[256 * k + i];
					dy = dpy[256 * k + i];
					EXPAND_U8C_FOR_MUL;
					MUL_S16C_U8C_FOR16_WITH_0_ODD;
					buff0[i] = dr;
					buff1[i] = dr1;
					buff2[i] = dr2;
					buff3[i] = dr3;
				}

				(void *)vis_alignaddr(dpx, 6);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dr = buff0[i];
					dr1 = buff1[i];
					dr2 = buff2[i];
					dr3 = buff3[i];
					MUL_S16C_U8C_FOR16_WITH_1_ODD;
					dpz[512 * k + 2 * i] = dzh;
					dpz[512 * k + 2 * i + 1] = dzl;
				}
			}

			N = even_8 & ~255;
			(void *)vis_alignaddr(dpx, 7);
#pragma pipeloop(0)
			for (i = 0; i < even_8 - N; i++) {
				dx = dpx[i + N];
				dy = dpy[i + N];
				EXPAND_U8C_FOR_MUL;
				MUL_S16C_U8C_FOR16_WITH_0_ODD;
				buff0[i] = dr;
				buff1[i] = dr1;
				buff2[i] = dr2;
				buff3[i] = dr3;
			}

			(void *)vis_alignaddr(dpx, 6);
#pragma pipeloop(0)
			for (i = 0; i < even_8 - N; i++) {
				dr = buff0[i];
				dr1 = buff1[i];
				dr2 = buff2[i];
				dr3 = buff3[i];
				MUL_S16C_U8C_FOR16_WITH_1_ODD;
				dpz[2 * N + 2 * i] = dzh;
				dpz[2 * N + 2 * i + 1] = dzl;
			}

			vis_alignaddr((void *)6, 0);
			rest = vis_faligndata(rest, dzero);
			vis_pst_16(rest, pz, 0x8);
			pz++;
		}
	} else {

/*
 * Both ("x" and "y") address are arbitrary aligned.
 * 2 vis_alignaddr(s) and 2 vis_faligndata(s) in the loop.
 */

		dpx = (void *)((mlib_addr)px & ~7);
		dpy = (void *)((mlib_addr)py & ~7);

		if (odd) {

			for (k = 0; k < (even_8 >> 8); k++) {
				__mlib_VectorCopy_U8((void *)buff0,
					(void *)((mlib_d64 *)px + k * 256),
					256 * 8);
				(void *)vis_alignaddr(py, 0);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = buff0[i];
					dy0 = dpy[k * 256 + i];
					dy1 = vis_ld_d64_nf(dpy +
					    k * 256 + i + 1);
					dy = vis_faligndata(dy0, dy1);
					EXPAND_U8C_FOR_0_MUL;
					MUL_S16C_U8C_FOR16_WITH_0_ODD;
					buff0[i] = dr;
					buff1[i] = dr1;
					buff2[i] = dr2;
					buff3[i] = dr3;
				}

				vis_alignaddr((void *)6, 0);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dr = buff0[i];
					dr1 = buff1[i];
					dr2 = buff2[i];
					dr3 = buff3[i];
					MUL_S16C_U8C_FOR16_WITH_1_ODD;
					dpz[512 * k + 2 * i] = dzh;
					dpz[512 * k + 2 * i + 1] = dzl;
				}
			}

			N = even_8 & ~255;
			__mlib_VectorCopy_U8((void *)buff0,
				(void *)((mlib_d64 *)px + N), (even_8 - N) * 8);
			vis_alignaddr(py, 0);
#pragma pipeloop(0)
			for (i = 0; i < even_8 - N; i++) {
				dx = buff0[i];
				dy0 = dpy[N + i];
				dy1 = vis_ld_d64_nf(dpy + N + i + 1);
				dy = vis_faligndata(dy0, dy1);
				EXPAND_U8C_FOR_0_MUL;
				MUL_S16C_U8C_FOR16_WITH_0_ODD;
				buff0[i] = dr;
				buff1[i] = dr1;
				buff2[i] = dr2;
				buff3[i] = dr3;
			}

			vis_alignaddr((void *)6, 0);
#pragma pipeloop(0)
			for (i = 0; i < (even_8 - N); i++) {
				dr = buff0[i];
				dr1 = buff1[i];
				dr2 = buff2[i];
				dr3 = buff3[i];
				MUL_S16C_U8C_FOR16_WITH_1_ODD;
				dpz[2 * N + 2 * i] = dzh;
				dpz[2 * N + 2 * i + 1] = dzl;
			}

			vis_alignaddr((void *)6, 0);
			rest = vis_faligndata(rest, dzero);
			vis_pst_16(rest, pz, 0x8);
			pz++;
		} else {

			for (k = 0; k < (even_8 >> 8); k++) {
				__mlib_VectorCopy_U8((void *)buff0,
					(void *)((mlib_d64 *)px + k * 256),
					256 * 8);
				(void *)vis_alignaddr(py, 0);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = buff0[i];
					dy0 = dpy[k * 256 + i];
					dy1 = vis_ld_d64_nf(dpy +
					    k * 256 + i + 1);
					dy = vis_faligndata(dy0, dy1);
					EXPAND_U8C_FOR_0_MUL;
					MUL_S16C_U8C_0_FOR16;
					buff0[i] = dr;
					buff1[i] = dr1;
					buff2[i] = dr2;
					buff3[i] = dr3;
				}

				vis_alignaddr((void *)6, 0);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dr = buff0[i];
					dr1 = buff1[i];
					dr2 = buff2[i];
					dr3 = buff3[i];
					MUL_S16C_U8C_1_FOR16;
					dpz[512 * k + 2 * i] = dzh;
					dpz[512 * k + 2 * i + 1] = dzl;
				}
			}

			N = even_8 & ~255;
			__mlib_VectorCopy_U8((void *)buff0,
				(void *)((mlib_d64 *)px + N), (even_8 - N) * 8);
			vis_alignaddr(py, 0);
#pragma pipeloop(0)
			for (i = 0; i < even_8 - N; i++) {
				dx = buff0[i];
				dy0 = dpy[N + i];
				dy1 = vis_ld_d64_nf(dpy + N + i + 1);
				dy = vis_faligndata(dy0, dy1);
				EXPAND_U8C_FOR_0_MUL;
				MUL_S16C_U8C_0_FOR16;
				buff0[i] = dr;
				buff1[i] = dr1;
				buff2[i] = dr2;
				buff3[i] = dr3;
			}

			vis_alignaddr((void *)6, 0);
#pragma pipeloop(0)
			for (i = 0; i < (even_8 - N); i++) {
				dr = buff0[i];
				dr1 = buff1[i];
				dr2 = buff2[i];
				dr3 = buff3[i];
				MUL_S16C_U8C_1_FOR16;
				dpz[2 * N + 2 * i] = dzh;
				dpz[2 * N + 2 * i + 1] = dzl;
			}
		}
	}

	px += (even_8 << 3);
	py += (even_8 << 3);

	while (rest_8 > 0) {
		re_x = (*px++);
		im_x = (*px++);
		re_y = (*py++);
		im_y = (*py++);
		re_c = re_x * re_y - im_x * im_y;
		im_c = im_x * re_y + re_x * im_y;
		(*pz++) =
			re_c > MLIB_S16_MAX ? MLIB_S16_MAX : (re_c <
			MLIB_S16_MIN ? MLIB_S16_MIN : re_c);
		(*pz++) =
			im_c > MLIB_S16_MAX ? MLIB_S16_MAX : (im_c <
			MLIB_S16_MIN ? MLIB_S16_MIN : im_c);
		rest_8 -= 2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_U8C_Sat(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorMul_U8C_U8C_Sat(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return __mlib_VectorMul_S8C_S8C_Mod((mlib_s8 *)z, (mlib_s8 *)x,
		(mlib_s8 *)y, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16C_U8C_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dy0, dy1;
	mlib_d64 dr, dr1, dr2, dr3, dzh, dzl;
	mlib_u8 *px, *py;
	mlib_s16 *pz;

/* odd = 1 means "aligned" resultant vector starts with Im. part. */
	mlib_s32 odd = 0;
	mlib_s32 len = n, i;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_d64 dzero = vis_fzero(), rest;
	mlib_d64 clear = vis_to_double_dup(0xFF00FF00);
	mlib_f32 fzero = vis_read_hi(dzero);
	mlib_s32 re_x, im_x, re_y, im_y, re_c, im_c;
	mlib_d64 const_clear = vis_to_double_dup(0xffff0000);
	mlib_d64 buff0[256], buff1[256], buff2[256], buff3[256];
	mlib_s32 k, N;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_u8 *)x;
	py = (mlib_u8 *)y;
	pz = (mlib_s16 *)z;

	if (n <= 8)
		MUL_S16C_S8C_MOD_IN_C;

/*
 * prepare the destination address trying to align it for 8 bytes.
 */

	while ((mlib_addr)pz & 7) {
		re_x = (*px++);
		im_x = (*px++);
		re_y = (*py++);
		im_y = (*py++);
		re_c = re_x * re_y - im_x * im_y;
		im_c = im_x * re_y + re_x * im_y;
		len--;
		(*pz++) = re_c;

		if (!((mlib_addr)pz & 7)) {
			odd = 1;
			rest = vis_to_double(0, im_c);
			break;
		}

		(*pz++) = im_c;
	}

	dpz = (mlib_d64 *)pz;
	len <<= 1;
	even_8 = len >> 3;
	rest_8 = len & 0x7;
	pz += (even_8 << 3);

	if (!(((mlib_addr)px | (mlib_addr)py) & 7)) {

/*
 * Both addresses are 8-byte aligned.
 * No  vis_alignaddr and  vis_faligndata at all.
 */

		dpx = (mlib_d64 *)px;
		dpy = (mlib_d64 *)py;

		if (!odd) {
/* initialize GSR */
			vis_alignaddr((void *)7, 0);

			for (k = 0; k < (even_8 >> 8); k++) {
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = dpx[256 * k + i];
					dy = dpy[256 * k + i];
					EXPAND_U8C_FOR_MUL_MOD;
					MUL_S16C_U8C_FOR16_MOD;
					COMPOSE_S16C_FROM32_MOD;
					dpz[512 * k + 2 * i] = dzh;
					dpz[512 * k + 2 * i + 1] = dzl;
				}
			}

			N = even_8 & ~255;
#pragma pipeloop(0)
			for (i = N; i < even_8; i++) {
				dx = dpx[i];
				dy = dpy[i];
				EXPAND_U8C_FOR_MUL_MOD;
				MUL_S16C_U8C_FOR16_MOD;
				COMPOSE_S16C_FROM32_MOD;
				dpz[2 * i] = dzh;
				dpz[2 * i + 1] = dzl;
			}
		} else {

			for (k = 0; k < (even_8 >> 8); k++) {
/* initialize GSR */
				vis_alignaddr((void *)7, 0);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = dpx[256 * k + i];
					dy = dpy[256 * k + i];
					EXPAND_U8C_FOR_MUL_MOD;
					MUL_S16C_U8C_FOR16_MOD;
					buff0[i] = dr;
					buff1[i] = dr1;
					buff2[i] = dr2;
					buff3[i] = dr3;
				}

				vis_alignaddr((void *)6, 0);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dr = buff0[i];
					dr1 = buff1[i];
					dr2 = buff2[i];
					dr3 = buff3[i];
					COMPOSE_S16C_FROM32_MOD_WITH_ODD;
					dpz[512 * k + 2 * i] = dzh;
					dpz[512 * k + 2 * i + 1] = dzl;
				}
			}

			N = even_8 & ~255;
/* initialize GSR */
			vis_alignaddr((void *)7, 0);
#pragma pipeloop(0)
			for (i = 0; i < even_8 - N; i++) {
				dx = dpx[i + N];
				dy = dpy[i + N];
				EXPAND_U8C_FOR_MUL_MOD;
				MUL_S16C_U8C_FOR16_MOD;
				buff0[i] = dr;
				buff1[i] = dr1;
				buff2[i] = dr2;
				buff3[i] = dr3;
			}

			vis_alignaddr((void *)6, 0);
#pragma pipeloop(0)
			for (i = 0; i < even_8 - N; i++) {
				dr = buff0[i];
				dr1 = buff1[i];
				dr2 = buff2[i];
				dr3 = buff3[i];
				COMPOSE_S16C_FROM32_MOD_WITH_ODD;
				dpz[2 * i + 2 * N] = dzh;
				dpz[2 * i + 2 * N + 1] = dzl;
			}

			vis_alignaddr((void *)6, 0);
			rest = vis_faligndata(rest, dzero);
			vis_pst_16(rest, pz, 0x8);
			pz++;
		}
	} else {

/*
 * Both ("x" and "y") address are arbitrary aligned.
 * 2 vis_alignaddr(s) and 2 vis_faligndata(s) in the loop.
 */

		dpx = (void *)((mlib_addr)px & ~7);
		dpy = (void *)((mlib_addr)py & ~7);

		if (odd) {

			for (k = 0; k < (even_8 >> 8); k++) {
				__mlib_VectorCopy_U8((void *)buff0,
					(void *)((mlib_d64 *)px + k * 256),
					256 * 8);
				(void *)vis_alignaddr(py, 0);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = buff0[i];
					dy0 = dpy[k * 256 + i];
					dy1 = vis_ld_d64_nf(dpy +
					    k * 256 + i + 1);
					dy = vis_faligndata(dy0, dy1);
					EXPAND_U8C_FOR_MUL_0_MOD;
					MUL_S16C_U8C_FOR16_MOD;
					buff0[i] = dr;
					buff1[i] = dr1;
					buff2[i] = dr2;
					buff3[i] = dr3;
				}

				vis_alignaddr((void *)6, 0);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dr = buff0[i];
					dr1 = buff1[i];
					dr2 = buff2[i];
					dr3 = buff3[i];
					COMPOSE_S16C_FROM32_MOD_WITH_ODD;
					dpz[512 * k + 2 * i] = dzh;
					dpz[512 * k + 2 * i + 1] = dzl;
				}
			}

			N = even_8 & ~255;
			__mlib_VectorCopy_U8((void *)buff0,
				(void *)((mlib_d64 *)px + N), (even_8 - N) * 8);
			vis_alignaddr(py, 0);
#pragma pipeloop(0)
			for (i = 0; i < even_8 - N; i++) {
				dx = buff0[i];
				dy0 = dpy[N + i];
				dy1 = vis_ld_d64_nf(dpy + N + i + 1);
				dy = vis_faligndata(dy0, dy1);
				EXPAND_U8C_FOR_MUL_0_MOD;
				MUL_S16C_U8C_FOR16_MOD;
				buff0[i] = dr;
				buff1[i] = dr1;
				buff2[i] = dr2;
				buff3[i] = dr3;
			}

			vis_alignaddr((void *)6, 0);
#pragma pipeloop(0)
			for (i = 0; i < (even_8 - N); i++) {
				dr = buff0[i];
				dr1 = buff1[i];
				dr2 = buff2[i];
				dr3 = buff3[i];
				COMPOSE_S16C_FROM32_MOD_WITH_ODD;
				dpz[2 * N + 2 * i] = dzh;
				dpz[2 * N + 2 * i + 1] = dzl;
			}

			vis_alignaddr((void *)6, 0);
			rest = vis_faligndata(rest, dzero);
			vis_pst_16(rest, pz, 0x8);
			pz++;
		} else {

			for (k = 0; k < (even_8 >> 8); k++) {
				__mlib_VectorCopy_U8((void *)buff0,
					(void *)((mlib_d64 *)px + k * 256),
					256 * 8);
				(void *)vis_alignaddr(py, 0);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = buff0[i];
					dy0 = dpy[k * 256 + i];
					dy1 = vis_ld_d64_nf(dpy +
					    k * 256 + i + 1);
					dy = vis_faligndata(dy0, dy1);
					EXPAND_U8C_FOR_MUL_0_MOD;
					MUL_S16C_U8C_FOR16_MOD;
					buff0[i] = dr;
					buff1[i] = dr1;
					buff2[i] = dr2;
					buff3[i] = dr3;
				}

				vis_alignaddr((void *)6, 0);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dr = buff0[i];
					dr1 = buff1[i];
					dr2 = buff2[i];
					dr3 = buff3[i];
					COMPOSE_S16C_FROM32_MOD;
					dpz[512 * k + 2 * i] = dzh;
					dpz[512 * k + 2 * i + 1] = dzl;
				}
			}

			N = even_8 & ~255;
			__mlib_VectorCopy_U8((void *)buff0,
				(void *)((mlib_d64 *)px + N), (even_8 - N) * 8);
			vis_alignaddr(py, 0);
#pragma pipeloop(0)
			for (i = 0; i < even_8 - N; i++) {
				dx = buff0[i];
				dy0 = dpy[i + N];
				dy1 = vis_ld_d64_nf(dpy + i + N + 1);
				dy = vis_faligndata(dy0, dy1);
				EXPAND_U8C_FOR_MUL_0_MOD;
				MUL_S16C_U8C_FOR16_MOD;
				buff0[i] = dr;
				buff1[i] = dr1;
				buff2[i] = dr2;
				buff3[i] = dr3;
			}

			vis_alignaddr((void *)6, 0);
#pragma pipeloop(0)
			for (i = 0; i < (even_8 - N); i++) {
				dr = buff0[i];
				dr1 = buff1[i];
				dr2 = buff2[i];
				dr3 = buff3[i];
				COMPOSE_S16C_FROM32_MOD;
				dpz[2 * N + 2 * i] = dzh;
				dpz[2 * N + 2 * i + 1] = dzl;
			}
		}
	}

	px += (even_8 << 3);
	py += (even_8 << 3);

	while (rest_8 > 0) {
		re_x = (*px++);
		im_x = (*px++);
		re_y = (*py++);
		im_y = (*py++);
		re_c = re_x * re_y - im_x * im_y;
		im_c = im_x * re_y + re_x * im_y;
		(*pz++) = re_c;
		(*pz++) = im_c;
		rest_8 -= 2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_U8C_Mod(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorMul_U8C_U8C_Mod(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
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
	mlib_d64 dover_i = vis_to_double_dup(0x80008000);
	mlib_d64 dover_r = vis_to_double_dup(0x7f807f80);
	mlib_s32 mask_r, mask_i;
	mlib_s32 N, k;
	mlib_d64 x_buff[256];
	mlib_f32 mask = vis_to_float(0xFFFFFF00);
	mlib_f32 fr2, fr3;

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);

	if (n <= 0)
		return (MLIB_FAILURE);

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
		vis_pst_8(restore, dpz, mask_r & emask);
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
					vis_pst_8(restore, dpz + i, mask_r);
				}

				dpx += len;
				dpy += len;
				dpz += len;
				dx = dpx[0];
				dy = dpy[0];
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
					vis_pst_8(restore, dpz + i, mask_r);
				}

				dpx += len;
				dpy += len;
				dpz += len;
				dx = dpx[0];
				dy0 = dpy[0];
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
					dy1 = vis_ld_d64_nf(dpy +
					    k * 256 + i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S8C_S16C;
					PACK_S16C_S8C0_SAT;
					dpz[256 * k + i] = dr;
					vis_pst_8(restore, dpz + 256 * k + i,
						mask_r);
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
				vis_pst_8(restore, dpz + i, mask_r);
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
			vis_pst_8(restore, dpz, mask_r & emask);
		}

		return (MLIB_SUCCESS);
	}

	dpx++;
	dpy++;
	PACK_S16C_S8C1_SAT;
	vis_pst_8(dr, dpz, emask);
	vis_pst_8(restore, dpz, mask_r & emask);

	if ((mlib_addr)++dpz <= (mlib_addr)pzend) {
		vis_pst_8(dr1, dpz, 0x80);
		vis_pst_8(restore, dpz, mask_i);
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
			vis_pst_8(restore, dpz + 256 * k + i, mask_r);
			vis_pst_8(dr1, dpz + 256 * k + i + 1, 0x80);
			vis_pst_8(restore, dpz + 256 * k + i + 1, mask_i);
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
		vis_pst_8(restore, dpz + i, mask_r);
		vis_pst_8(dr1, dpz + i + 1, 0x80);
		vis_pst_8(restore, dpz + i + 1, mask_i);
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
		vis_pst_8(restore, dpz, mask_r & emask);

		if ((mlib_addr)++dpz <= (mlib_addr)pzend) {
			vis_pst_8(dr1, dpz, 0x80);
			vis_pst_8(restore, dpz, mask_i);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16C_S8C_Sat(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dy0, dy1;
	mlib_d64 dr, dr1, dzh, dzl, re2, re3, re4, im2, im3, im4;
	mlib_s8 *px, *py;
	mlib_s16 *pz;

/* odd = 1 means "aligned" resultant vector starts with Im. part. */
	mlib_s32 odd = 0;
	mlib_s32 len = n, i;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_d64 dzero = vis_to_double_dup(0), rest;
	mlib_d64 without_control = vis_to_double_dup(0xff00ff);
	mlib_d64 cntr_ovlp = vis_to_double_dup(0x80008000);
	mlib_d64 const_max_16 = vis_to_double_dup(0x7fff7fff);
	mlib_s32 mask, mask1;
	mlib_s32 re_x, im_x, re_y, im_y, re_c, im_c;
	mlib_d64 x_buff[256], r_buff[256];
	mlib_s32 N, k;

	if (n <= 0)
		return (MLIB_FAILURE);

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);
	px = (mlib_s8 *)x;
	py = (mlib_s8 *)y;
	pz = (mlib_s16 *)z;

	if (n <= 8)
		MUL_S16C_S8C_SAT_IN_C;

/*
 * prepare the destination address trying to align it for 8 bytes.
 */

	while ((mlib_addr)pz & 7) {
		re_x = (*px++);
		im_x = (*px++);
		re_y = (*py++);
		im_y = (*py++);
		re_c = re_x * re_y - im_x * im_y;
		im_c = im_x * re_y + re_x * im_y;
		re_c = re_c > MLIB_S16_MAX ? MLIB_S16_MAX : (re_c <
			MLIB_S16_MIN ? MLIB_S16_MIN : re_c);
		im_c = im_c > MLIB_S16_MAX ? MLIB_S16_MAX : (im_c <
			MLIB_S16_MIN ? MLIB_S16_MIN : im_c);
		len--;
		(*pz++) = re_c;

		if (!((mlib_addr)pz & 7)) {
			odd = 1;
			rest = vis_to_double(0, im_c);
			break;
		}

		(*pz++) = im_c;
	}

	dpz = (mlib_d64 *)pz;
	len <<= 1;
	even_8 = len >> 3;
	rest_8 = len & 0x7;
	pz += (even_8 << 3);

	if (!(((mlib_addr)px | (mlib_addr)py) & 7)) {

/*
 * Both addresses are 8-byte aligned.
 * No  vis_alignaddr and  vis_faligndata at all.
 */

		dpx = (mlib_d64 *)px;
		dpy = (mlib_d64 *)py;
		vis_alignaddr((void *)2, 0);

		if (!odd) {
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = dpx[i];
				dy = dpy[i];
				MUL_S8C_S16C;
				COMPOSE_S16C_SAT;
				vis_pst_16(const_max_16, dpz + 2 * i, mask);
				vis_pst_16(const_max_16, dpz + 2 * i + 1,
					mask1);
				vis_pst_16(dzh, dpz + 2 * i, ~mask);
				vis_pst_16(dzl, dpz + 2 * i + 1, ~mask1);
			}
		} else {
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = dpx[i];
				dy = dpy[i];
				MUL_S8C_S16C;
				COMPOSE_S16C_SAT_WITH_ODD;
				vis_pst_16(const_max_16, dpz + 2 * i, mask);
				vis_pst_16(const_max_16, dpz + 2 * i + 1,
					mask1);
				vis_pst_16(dzh, dpz + 2 * i, ~mask);
				vis_pst_16(dzl, dpz + 2 * i + 1, ~mask1);
			}

			vis_alignaddr((void *)6, 0);
			rest = vis_faligndata(rest, dzero);
			mask = vis_fcmpeq16(rest, cntr_ovlp);
			vis_pst_16(rest, pz, 0x8);
			vis_pst_16(const_max_16, pz, mask);
			pz++;
		}
	} else {

/*
 * Both ("x" and "y") address are arbitrary aligned.
 * 2 vis_alignaddr(s) and 2 vis_faligndata(s) in the loop.
 */

		dpx = (void *)((mlib_addr)px & ~7);
		dpy = (void *)((mlib_addr)py & ~7);

		if (odd) {

			for (k = 0; k < (even_8 >> 8); k++) {
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + k * 256),
					256 * 8);
				(void *)vis_alignaddr(py, 0);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = x_buff[i];
					dy0 = dpy[k * 256 + i];
					dy1 = vis_ld_d64_nf(dpy +
					    k * 256 + i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S8C_S16C;
					x_buff[i] = dr1;
					r_buff[i] = dr;
				}

				vis_alignaddr((void *)2, 0);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dr1 = x_buff[i];
					dr = r_buff[i];
					COMPOSE_S16C_SAT_WITH_ODD;
					vis_pst_16(const_max_16,
						dpz + 512 * k + 2 * i, mask);
					vis_pst_16(const_max_16,
						dpz + 512 * k + 2 * i + 1,
						mask1);
					vis_pst_16(dzh, dpz + 512 * k + 2 * i,
						~mask);
					vis_pst_16(dzl,
						dpz + 512 * k + 2 * i + 1,
						~mask1);
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
				MUL_S8C_S16C;
				x_buff[i - N] = dr1;
				r_buff[i - N] = dr;
			}

			vis_alignaddr((void *)2, 0);
#pragma pipeloop(0)
			for (i = N; i < even_8; i++) {
				dr = r_buff[i - N];
				dr1 = x_buff[i - N];
				COMPOSE_S16C_SAT_WITH_ODD;
				vis_pst_16(const_max_16, dpz + 2 * i, mask);
				vis_pst_16(const_max_16, dpz + 2 * i + 1,
					mask1);
				vis_pst_16(dzh, dpz + 2 * i, ~mask);
				vis_pst_16(dzl, dpz + 2 * i + 1, ~mask1);
			}

			vis_alignaddr((void *)6, 0);
			rest = vis_faligndata(rest, dzero);
			mask = vis_fcmpeq16(rest, cntr_ovlp);
			vis_pst_16(rest, pz, 0x8);
			vis_pst_16(const_max_16, pz, mask);
			pz++;
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
					dy1 = vis_ld_d64_nf(dpy +
					    k * 256 + i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S8C_S16C;
					x_buff[i] = dr1;
					r_buff[i] = dr;
				}

				vis_alignaddr((void *)2, 0);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dr1 = x_buff[i];
					dr = r_buff[i];
					COMPOSE_S16C_SAT;
					vis_pst_16(const_max_16,
						dpz + 512 * k + 2 * i, mask);
					vis_pst_16(const_max_16,
						dpz + 512 * k + 2 * i + 1,
						mask1);
					vis_pst_16(dzh, dpz + 512 * k + 2 * i,
						~mask);
					vis_pst_16(dzl,
						dpz + 512 * k + 2 * i + 1,
						~mask1);
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
				MUL_S8C_S16C;
				x_buff[i - N] = dr1;
				r_buff[i - N] = dr;
			}

			vis_alignaddr((void *)2, 0);
#pragma pipeloop(0)
			for (i = N; i < even_8; i++) {
				dr = r_buff[i - N];
				dr1 = x_buff[i - N];
				COMPOSE_S16C_SAT;
				vis_pst_16(const_max_16, dpz + 2 * i, mask);
				vis_pst_16(const_max_16, dpz + 2 * i + 1,
					mask1);
				vis_pst_16(dzh, dpz + 2 * i, ~mask);
				vis_pst_16(dzl, dpz + 2 * i + 1, ~mask1);
			}
		}
	}

	px += (even_8 << 3);
	py += (even_8 << 3);

	while (rest_8 > 0) {
		re_x = (*px++);
		im_x = (*px++);
		re_y = (*py++);
		im_y = (*py++);
		re_c = re_x * re_y - im_x * im_y;
		im_c = im_x * re_y + re_x * im_y;
		re_c = re_c > MLIB_S16_MAX ? MLIB_S16_MAX : (re_c <
			MLIB_S16_MIN ? MLIB_S16_MIN : re_c);
		im_c = im_c > MLIB_S16_MAX ? MLIB_S16_MAX : (im_c <
			MLIB_S16_MIN ? MLIB_S16_MIN : im_c);
		(*pz++) = re_c;
		(*pz++) = im_c;
		rest_8 -= 2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16C_S8C_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dy0, dy1;
	mlib_d64 dr, dr1, dr2, dr3, dzh, dzl;
	mlib_s8 *px, *py;
	mlib_s16 *pz;

/* odd = 1 means "aligned" resultant vector starts with Im. part. */
	mlib_s32 odd = 0;
	mlib_s32 len = n, i;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_d64 dzero = vis_to_double_dup(0), rest;
	mlib_d64 without_control = vis_to_double_dup(0xff00ff);
	mlib_s32 re_x, im_x, re_y, im_y, re_c, im_c;
	mlib_d64 x_buff[256], r_buff[256];
	mlib_s32 N, k;

	if (n <= 0)
		return (MLIB_FAILURE);

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);
	px = (mlib_s8 *)x;
	py = (mlib_s8 *)y;
	pz = (mlib_s16 *)z;

	if (n <= 8)
		MUL_S16C_S8C_MOD_IN_C;

/*
 * prepare the destination address trying to align it for 8 bytes.
 */

#pragma pipeloop(0)
	while ((mlib_addr)pz & 7) {
		re_x = (*px++);
		im_x = (*px++);
		re_y = (*py++);
		im_y = (*py++);
		re_c = re_x * re_y - im_x * im_y;
		im_c = im_x * re_y + re_x * im_y;
		len--;
		(*pz++) = re_c;

		if (!((mlib_addr)pz & 7)) {
			odd = 1;
			rest = vis_to_double(0, im_c);
			break;
		}

		(*pz++) = im_c;
	}

	dpz = (mlib_d64 *)pz;
	len <<= 1;
	even_8 = len >> 3;
	rest_8 = len & 0x7;
	pz += (even_8 << 3);

	if (!(((mlib_addr)px | (mlib_addr)py) & 7)) {

/*
 * Both addresses are 8-byte aligned.
 * No  vis_alignaddr and  vis_faligndata at all.
 */

		dpx = (mlib_d64 *)px;
		dpy = (mlib_d64 *)py;

		if (!odd) {
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = dpx[i];
				dy = dpy[i];
				MUL_S8C_S16C;
				COMPOSE_S16C_MOD;
				dpz[2 * i] = dzh;
				dpz[2 * i + 1] = dzl;
			}
		} else {
			(void *)vis_alignaddr(dpx, 6);
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = dpx[i];
				dy = dpy[i];
				MUL_S8C_S16C;
				COMPOSE_S16C_MOD_WITH_ODD;
				dpz[2 * i] = dzh;
				dpz[2 * i + 1] = dzl;
			}

			vis_alignaddr((void *)6, 0);
			rest = vis_faligndata(rest, dzero);
			vis_pst_16(rest, pz, 0x8);
			pz++;
		}
	} else {

/*
 * Both ("x" and "y") address are arbitrary aligned.
 * 2 vis_alignaddr(s) and 2 vis_faligndata(s) in the loop.
 */

		dpx = (void *)((mlib_addr)px & ~7);
		dpy = (void *)((mlib_addr)py & ~7);

		if (odd) {

			for (k = 0; k < (even_8 >> 8); k++) {
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + k * 256),
					256 * 8);
				(void *)vis_alignaddr(py, 0);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = x_buff[i];
					dy0 = dpy[k * 256 + i];
					dy1 = vis_ld_d64_nf(dpy +
					    k * 256 + i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S8C_S16C;
					x_buff[i] = dr1;
					r_buff[i] = dr;
				}

				(void *)vis_alignaddr(dpx, 6);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dr1 = x_buff[i];
					dr = r_buff[i];
					COMPOSE_S16C_MOD_WITH_ODD;
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
				MUL_S8C_S16C;
				x_buff[i - N] = dr1;
				r_buff[i - N] = dr;
			}

			(void *)vis_alignaddr(dpx, 6);
#pragma pipeloop(0)
			for (i = N; i < even_8; i++) {
				dr = r_buff[i - N];
				dr1 = x_buff[i - N];
				COMPOSE_S16C_MOD_WITH_ODD;
				dpz[2 * i] = dzh;
				dpz[2 * i + 1] = dzl;
			}

			vis_alignaddr((void *)6, 0);
			rest = vis_faligndata(rest, dzero);
			vis_pst_16(rest, pz, 0x8);
			pz++;
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
					dy1 = vis_ld_d64_nf(dpy +
					    k * 256 + i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S8C_S16C;
					COMPOSE_S16C_MOD;
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
				MUL_S8C_S16C;
				COMPOSE_S16C_MOD;
				dpz[2 * i] = dzh;
				dpz[2 * i + 1] = dzl;
			}
		}
	}

	px += (even_8 << 3);
	py += (even_8 << 3);

	while (rest_8 > 0) {
		re_x = (*px++);
		im_x = (*px++);
		re_y = (*py++);
		im_y = (*py++);
		re_c = re_x * re_y - im_x * im_y;
		im_c = im_x * re_y + re_x * im_y;
		(*pz++) = re_c;
		(*pz++) = im_c;
		rest_8 -= 2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S8C_Sat(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorMul_S8C_S8C_Sat(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S8C_S8C_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
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

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);

	if (n <= 0)
		return (MLIB_FAILURE);

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
				dx = dpx[0];
				dy = dpy[0];
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
				dy0 = dpy[0];
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
					dy1 = vis_ld_d64_nf(dpy +
					    k * 256 + i + 1);
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
__mlib_VectorMul_S8C_Mod(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorMul_S8C_S8C_Mod(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s16 *pzend;
	mlib_f32 sr0, sr1, sr2, sr3;
	mlib_d64 *dpz, *dpx, *dpy, *dpzend;
	mlib_d64 dx, dy, dr, dr1, dx0, dx1, dy0, dy1;

	mlib_d64 d_over = vis_to_double_dup(0x7fff7fff);
	mlib_d64 d_under = vis_fnot(d_over);
	mlib_d64 d_min16 = vis_to_double_dup(MLIB_S16_MIN);
	mlib_d64 d_min32 = vis_to_double_dup(MLIB_S32_MIN);
	mlib_d64 d_max16 = vis_to_double_dup(MLIB_S16_MAX);
	mlib_s32 mask_under = 0, mask_over = 0;
	mlib_s32 i, k, N, delta_x;
	mlib_s32 end;
	mlib_d64 x_buff[256], y_buff[256], r_buff[256];

	if (n <= 0)
		return (MLIB_FAILURE);
/* initialize GSR scale factor */
	vis_write_gsr(15 << 3);

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
	dpy = (mlib_d64 *)vis_alignaddr((void *)y, off);
	dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);

	if (!(2 & (mlib_addr)z)) {
		if ((((mlib_addr)x | (mlib_addr)y | (mlib_addr)z) & 7) == 0) {
			end = dpzend - dpz;

			for (k = 0; k < (end >> 8); k++) {
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = dpx[256 * k + i];
					dy = dpy[256 * k + i];
					MUL_S16C_S32C_0;
					((mlib_f32 *)r_buff)[2 * i] = sr2;
					((mlib_f32 *)r_buff)[2 * i + 1] = sr3;
				}

#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = dpx[256 * k + i];
					dy = dpy[256 * k + i];
					sr2 = ((mlib_f32 *)r_buff)[2 * i];
					sr3 = ((mlib_f32 *)r_buff)[2 * i + 1];
					MUL_S16C_S32C_1;
					PACK_S32C_S16C0_SAT;
					dpz[256 * k + i] = dr;
					vis_pst_16(d_under, dpz + 256 * k + i,
						mask_under);
					vis_pst_16(d_over, dpz + 256 * k + i,
						mask_over);
				}
			}

			N = end & ~255;
#pragma pipeloop(0)
			for (i = 0; i < end - N; i++) {
				dx = dpx[N + i];
				dy = dpy[N + i];
				MUL_S16C_S32C_0;
				((mlib_f32 *)r_buff)[2 * i] = sr2;
				((mlib_f32 *)r_buff)[2 * i + 1] = sr3;
			}

#pragma pipeloop(0)
			for (i = 0; i < end - N; i++) {
				dx = dpx[N + i];
				dy = dpy[N + i];
				sr2 = ((mlib_f32 *)r_buff)[2 * i];
				sr3 = ((mlib_f32 *)r_buff)[2 * i + 1];
				MUL_S16C_S32C_1;
				PACK_S32C_S16C0_SAT;
				dpz[N + i] = dr;
				vis_pst_16(d_under, dpz + i + N, mask_under);
				vis_pst_16(d_over, dpz + i + N, mask_over);
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
			vis_pst_16(d_under, dpzend, mask_under & emask);
			return (MLIB_SUCCESS);
		} else {
			(void *)vis_alignaddr((void *)x, off);
			dx0 = vis_ld_d64_nf(dpx);
			dx1 = vis_ld_d64_nf(dpx + 1);
			dx = vis_faligndata(dx0, dx1);
			(void *)vis_alignaddr((void *)y, off);
			dy0 = vis_ld_d64_nf(dpy);
			dy1 = vis_ld_d64_nf(dpy + 1);
			dy = vis_faligndata(dy0, dy1);
			MUL_S16C_S32C;
			PACK_S32C_S16C0_SAT;
			vis_pst_16(dr, dpz, emask);
			vis_pst_16(d_under, dpz, mask_under & emask);
			vis_pst_16(d_over, dpz, mask_over & emask);
			dpz++;
			dpx++;
			dpy++;
			delta_x = 4 - (((mlib_addr)z & 6) >> 1);
			x += delta_x;
			end = dpzend - dpz;

			if (end < 0)
				return (MLIB_SUCCESS);

			for (k = 0; k < (end >> 8); k++) {
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)x + k * 256),
					256 * 8);
				(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = x_buff[i];
					dy0 = dpy[k * 256 + i];
					dy1 = vis_ld_d64_nf(dpy +
					    k * 256 + i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S16C_S32C_0;
					((mlib_f32 *)r_buff)[2 * i] = sr2;
					((mlib_f32 *)r_buff)[2 * i + 1] = sr3;
					y_buff[i] = dy;
				}

#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = x_buff[i];
					dy = y_buff[i];
					sr2 = ((mlib_f32 *)r_buff)[2 * i];
					sr3 = ((mlib_f32 *)r_buff)[2 * i + 1];
					MUL_S16C_S32C_1;
					PACK_S32C_S16C0_SAT;
					dpz[256 * k + i] = dr;
					vis_pst_16(d_under, dpz + 256 * k + i,
						mask_under);
					vis_pst_16(d_over, dpz + 256 * k + i,
						mask_over);
				}
			}

			N = end & ~255;
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)x + N), (end - N) * 8 +
				2 + ((mlib_addr)pzend & 7));
			(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
			for (i = 0; i < end - N; i++) {
				dx = x_buff[i];
				dy0 = dpy[N + i];
				dy1 = vis_ld_d64_nf(dpy + N + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S16C_S32C_0;
				((mlib_f32 *)r_buff)[2 * i] = sr2;
				((mlib_f32 *)r_buff)[2 * i + 1] = sr3;
				y_buff[i] = dy;
			}

#pragma pipeloop(0)
			for (i = 0; i < end - N; i++) {
				dx = x_buff[i];
				dy = y_buff[i];
				sr2 = ((mlib_f32 *)r_buff)[2 * i];
				sr3 = ((mlib_f32 *)r_buff)[2 * i + 1];
				MUL_S16C_S32C_1;
				PACK_S32C_S16C0_SAT;
				dpz[N + i] = dr;
				vis_pst_16(d_under, dpz + N + i, mask_under);
				vis_pst_16(d_over, dpz + N + i, mask_over);
			}

			x -= delta_x;
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
				vis_pst_16(d_under, dpzend, mask_under & emask);
			}

			return (MLIB_SUCCESS);
		}
	}

	(void *)vis_alignaddr((void *)x, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_faligndata(dx0, dx1);
	(void *)vis_alignaddr((void *)y, off);
	dy0 = vis_ld_d64_nf(dpy);
	dy1 = vis_ld_d64_nf(dpy + 1);
	dy = vis_faligndata(dy0, dy1);
	MUL_S16C_S32C;

	PACK_S32C_S16C1_SAT;
	vis_pst_16(dr, dpz, emask & 0x7);
	vis_pst_16(d_over, dpz, mask_over & emask & 0x7);
	vis_pst_16(d_under, dpz, mask_under & emask & 0x7);

	if ((mlib_addr)++dpz <= (mlib_addr)pzend) {
		vis_pst_16(dr, dpz, 0x8);
		vis_pst_16(d_over, dpz, mask_over & 0x8);
		vis_pst_16(d_under, dpz, mask_under & 0x8);
	}

	dpy = (mlib_d64 *)vis_alignaddr((void *)y, off);
	dy0 = vis_ld_d64_nf(dpy);
	dy1 = vis_ld_d64_nf(dpy + 1);
	dy = vis_faligndata(dy0, dy1);
	dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_faligndata(dx0, dx1);
	(void) vis_alignaddr(dpx, 6);
	MUL_S16C_S32C;

	(void *)vis_alignaddr((void *)x, off);
	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx + 2);
	dx = vis_faligndata(dx0, dx1);
	(void *)vis_alignaddr((void *)y, off);
	dy0 = dy1;
	dy1 = vis_ld_d64_nf(dpy + 2);
	dy = vis_faligndata(dy0, dy1);
	(void) vis_alignaddr(dpx, 6);

	MUL_S16C_S32C;
	(void) vis_alignaddr(dpx, 6);
#pragma pipeloop(0)
	while ((mlib_addr)dpz < (mlib_addr)dpzend) {
		PACK_S32C_S16C1_SAT;
		(void *)vis_alignaddr((void *)x, off);
		dx0 = dx1;
		dx1 = vis_ld_d64_nf(dpx + 3);
		dx = vis_faligndata(dx0, dx1);
		(void *)vis_alignaddr((void *)y, off);
		dy0 = dy1;
		dy1 = vis_ld_d64_nf(dpy + 3);
		dy = vis_faligndata(dy0, dy1);
		ST_S16C1_SAT;
		(void) vis_alignaddr(dpx, 6);
		MUL_S16C_S32C;
		dpx++;
		dpy++;
	}

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
		PACK_S32C_S16C1_SAT;
/* prepare edge mask for the last bytes */
		emask = vis_edge16(dpz, pzend);
/* store last bytes of result */
		vis_pst_16(dr, dpz, emask & 0x7);
		vis_pst_16(d_under, dpz, mask_under & emask & 0x7);
		vis_pst_16(d_over, dpz, mask_over & emask & 0x7);

		if ((mlib_addr)++dpz <= (mlib_addr)pzend) {
			vis_pst_16(dr, dpz, 0x8);
			vis_pst_16(d_under, dpz, mask_under & 0x8);
			vis_pst_16(d_over, dpz, mask_over & 0x8);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S32C_S16C_Sat(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dy0, dy1;
	mlib_d64 dzh, dzl;
	mlib_s16 *px, *py;
	mlib_s32 *pz;

/* odd = 1 means 8-byte aligned resultant vector starts with Im. part. */
	mlib_s32 odd = 0;
	mlib_s32 len = n, i;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_f32 fzero = vis_fzeros();
	mlib_f32 sr0, sr1, sr2, sr3;
	mlib_f32 rest;
	mlib_d64 drest;
	mlib_d64 cntr_ovlp = vis_to_double_dup(0x80000000);
	mlib_d64 const_max_32 = vis_to_double_dup(MLIB_S32_MAX);
	mlib_s32 mask, mask1;
	mlib_s32 re_x, im_x, re_y, im_y, re_c, im_c;
	mlib_d64 x_buff[256];
	mlib_s32 k, N;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s16 *)x;
	py = (mlib_s16 *)y;
	pz = (mlib_s32 *)z;

	if (n <= 8)
		MUL_S32C_S16C_SAT_IN_C;

/*
 * prepare the destination address trying to align it for 8 bytes.
 */

	while ((mlib_addr)pz & 7) {
		re_x = (*px++);
		im_x = (*px++);
		re_y = (*py++);
		im_y = (*py++);
		re_c = re_x * re_y - im_x * im_y;
		im_c = im_x * re_y + re_x * im_y;
		im_c = (im_c == 0x80000000 ? MLIB_S32_MAX : im_c);
		len--;
		(*pz++) = re_c;

		if (!((mlib_addr)pz & 7)) {
			odd = 1;
			rest = vis_read_hi(vis_to_double(im_c, 0));
			break;
		}

		(*pz++) = im_c;
	}

	dpz = (mlib_d64 *)pz;
	len <<= 2;
	even_8 = len >> 3;
	rest_8 = len & 0x7;
	pz += (even_8 << 2);

	if (!(((mlib_addr)px | (mlib_addr)py) & 7)) {

/*
 * Both addresses are 8-byte aligned.
 * No  vis_alignaddr and  vis_faligndata at all.
 */

		dpx = (mlib_d64 *)px;
		dpy = (mlib_d64 *)py;
		(void) vis_alignaddr(dpx, 6);

		if (!odd) {
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = dpx[i];
				dy = dpy[i];
				MUL_S16C_S32C;
				COMPOSE_S32C_FROM32_SAT;
				vis_pst_32(const_max_32, dpz + 2 * i, mask);
				vis_pst_32(const_max_32, dpz + 2 * i + 1,
					mask1);
				vis_pst_32(dzh, dpz + 2 * i, ~mask);
				vis_pst_32(dzl, dpz + 2 * i + 1, ~mask1);
			}
		} else {
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = dpx[i];
				dy = dpy[i];
				MUL_S16C_S32C;
				COMPOSE_S32C_FROM32_SAT_WITH_ODD;
				vis_pst_32(const_max_32, dpz + 2 * i, mask);
				vis_pst_32(const_max_32, dpz + 2 * i + 1,
					mask1);
				vis_pst_32(dzh, dpz + 2 * i, ~mask);
				vis_pst_32(dzl, dpz + 2 * i + 1, ~mask1);
			}

			vis_alignaddr((void *)6, 0);
			drest = vis_freg_pair(rest, fzero);
			mask = vis_fcmpeq32(drest, cntr_ovlp);
			vis_pst_32(drest, pz, 0x2);
			vis_pst_32(const_max_32, pz, mask);
			pz++;
		}
	} else {

/*
 * Both ("x" and "y") address are arbitrary aligned.
 * 2 vis_alignaddr(s) and 2 vis_faligndata(s) in the loop.
 */
		dpx = (void *)((mlib_addr)px & ~7);
		dpy = (void *)((mlib_addr)py & ~7);

		if (odd) {

			for (k = 0; k < (even_8 >> 8); k++) {
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + k * 256),
					256 * 8);
				vis_alignaddr(py, 0);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = x_buff[i];
					dy0 = dpy[k * 256 + i];
					dy1 = vis_ld_d64_nf(dpy +
					    k * 256 + i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S16C_S32C;
					COMPOSE_S32C_FROM32_SAT_WITH_ODD;
					vis_pst_32(const_max_32,
						dpz + 512 * k + 2 * i, mask);
					vis_pst_32(const_max_32,
						dpz + 512 * k + 2 * i + 1,
						mask1);
					vis_pst_32(dzh, dpz + 512 * k + 2 * i,
						~mask);
					vis_pst_32(dzl,
						dpz + 512 * k + 2 * i + 1,
						~mask1);
				}
			}

			N = even_8 & ~255;

			if (even_8 > 0)
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + N),
					(even_8 - N) * 8);
			vis_alignaddr(py, 0);
#pragma pipeloop(0)
			for (i = N; i < even_8; i++) {
				dx = x_buff[i - N];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S16C_S32C;
				COMPOSE_S32C_FROM32_SAT_WITH_ODD;
				vis_pst_32(const_max_32, dpz + 2 * i, mask);
				vis_pst_32(const_max_32, dpz + 2 * i + 1,
					mask1);
				vis_pst_32(dzh, dpz + 2 * i, ~mask);
				vis_pst_32(dzl, dpz + 2 * i + 1, ~mask1);
			}

			drest = vis_freg_pair(rest, fzero);
			mask = vis_fcmpeq32(drest, cntr_ovlp);
			vis_pst_32(drest, pz, 0x2);
			vis_pst_32(const_max_32, pz, mask);
			pz++;
		} else {

			for (k = 0; k < (even_8 >> 8); k++) {
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + k * 256),
					256 * 8);
				vis_alignaddr(py, 0);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = x_buff[i];
					dy0 = dpy[k * 256 + i];
					dy1 = vis_ld_d64_nf(dpy +
					    k * 256 + i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S16C_S32C;
					COMPOSE_S32C_FROM32_SAT;
					vis_pst_32(const_max_32,
						dpz + 512 * k + 2 * i, mask);
					vis_pst_32(const_max_32,
						dpz + 512 * k + 2 * i + 1,
						mask1);
					vis_pst_32(dzh, dpz + 512 * k + 2 * i,
						~mask);
					vis_pst_32(dzl,
						dpz + 512 * k + 2 * i + 1,
						~mask1);
				}
			}

			N = even_8 & ~255;

			if (even_8 > 0)
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + N),
					(even_8 - N) * 8);
			vis_alignaddr(py, 0);
#pragma pipeloop(0)
			for (i = N; i < even_8; i++) {
				dx = x_buff[i - N];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S16C_S32C;
				COMPOSE_S32C_FROM32_SAT;
				dpz[2 * i] = dzh;
				dpz[2 * i + 1] = dzl;
				vis_pst_32(const_max_32, dpz + 2 * i, mask);
				vis_pst_32(const_max_32, dpz + 2 * i + 1,
					mask1);
				vis_pst_32(dzh, dpz + 2 * i, ~mask);
				vis_pst_32(dzl, dpz + 2 * i + 1, ~mask1);
			}
		}
	}

	if (!rest_8)
		return (MLIB_SUCCESS);

	px += (even_8 << 2);
	py += (even_8 << 2);

	while (rest_8 > 0) {
		re_x = (*px++);
		im_x = (*px++);
		re_y = (*py++);
		im_y = (*py++);
		re_c = re_x * re_y - im_x * im_y;
		im_c = im_x * re_y + re_x * im_y;
		im_c = (im_c == 0x80000000 ? MLIB_S32_MAX : im_c);
		(*pz++) = re_c;
		(*pz++) = im_c;
		rest_8 -= 4;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16C_Sat(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorMul_S16C_S16C_Sat(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s16 *pzend;
	mlib_f32 sr0, sr1, sr2, sr3;
	mlib_d64 *dpz, *dpx, *dpy, *dpzend;
	mlib_d64 dx, dy, dr, dr1, dx0, dx1, dy0, dy1;

	mlib_d64 without_control = vis_to_double_dup(0xffff);
	mlib_d64 dzero = vis_fzero();
	mlib_s32 i, k, N, delta_x;
	mlib_s32 end;
	mlib_d64 x_buff[256], r_buff[256];

	if (n <= 0)
		return (MLIB_FAILURE);

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
	dpy = (mlib_d64 *)vis_alignaddr((void *)y, off);
	dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);

	if (!(2 & (mlib_addr)z)) {
		if ((((mlib_addr)x | (mlib_addr)y | (mlib_addr)z) & 7) == 0) {
			end = dpzend - dpz;
#pragma pipeloop(0)
			for (i = 0; i < end; i++) {
				dx = dpx[i];
				dy = dpy[i];
				MUL_S16C_S32C;
				PACK_S32C_S16C0_MOD;
				dpz[i] = dr;
			}

			dx = dpx[end];
			dy = dpy[end];
			MUL_S16C_S32C;
			PACK_S32C_S16C0_MOD;
/* prepare edge mask for the last bytes */
			emask = vis_edge16(dpzend, pzend);
/* store last bytes of result */
			vis_pst_16(dr, dpzend, emask);
			return (MLIB_SUCCESS);
		} else {
			(void *)vis_alignaddr((void *)x, off);
			dx0 = vis_ld_d64_nf(dpx);
			dx1 = vis_ld_d64_nf(dpx + 1);
			dx = vis_faligndata(dx0, dx1);
			(void *)vis_alignaddr((void *)y, off);
			dy0 = vis_ld_d64_nf(dpy);
			dy1 = vis_ld_d64_nf(dpy + 1);
			dy = vis_faligndata(dy0, dy1);
			MUL_S16C_S32C;
			PACK_S32C_S16C0_MOD;
			vis_pst_16(dr, dpz, emask);
			dpz++;
			dpx++;
			dpy++;
			delta_x = 4 - (((mlib_addr)z & 6) >> 1);
			x += delta_x;
			end = dpzend - dpz;

			if (end < 0)
				return (MLIB_SUCCESS);

			for (k = 0; k < (end >> 8); k++) {
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)x + k * 256),
					256 * 8);
				(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = x_buff[i];
					dy0 = dpy[k * 256 + i];
					dy1 = vis_ld_d64_nf(dpy +
					    k * 256 + i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S16C_S32C;
					PACK_S32C_S16C0_MOD;
					dpz[256 * k + i] = dr;
				}
			}

			N = end & ~255;
			__mlib_VectorCopy_U8((void *)x_buff,
				(void *)((mlib_d64 *)x + N), (end - N) * 8 +
				2 + ((mlib_addr)pzend & 7));
			(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
			for (i = N; i < end; i++) {
				dx = x_buff[i - N];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S16C_S32C;
				PACK_S32C_S16C0_MOD;
				dpz[i] = dr;
			}

			x -= delta_x;
		}

		dx = x_buff[end - N];
		dy0 = vis_ld_d64_nf(dpy + end);
		dy1 = vis_ld_d64_nf(dpy + end + 1);
		dy = vis_faligndata(dy0, dy1);
		MUL_S16C_S32C;
		dpz = dpzend;

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			PACK_S32C_S16C0_MOD;
/* prepare edge mask for the last bytes */
			emask = vis_edge16(dpz, pzend);
/* store last bytes of result */
			vis_pst_16(dr, dpz, emask);
		}

		return (MLIB_SUCCESS);
	}

	(void *)vis_alignaddr((void *)x, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_faligndata(dx0, dx1);
	(void *)vis_alignaddr((void *)y, off);
	dy0 = vis_ld_d64_nf(dpy);
	dy1 = vis_ld_d64_nf(dpy + 1);
	dy = vis_faligndata(dy0, dy1);
	MUL_S16C_S32C;

	dr = vis_freg_pair(sr0, sr1);
	dr1 = vis_fpmerge(sr3, sr2);
	dr1 = vis_fpmerge(vis_read_lo(dr1), vis_read_hi(dr1));
	PACK_S32C_S16C1_MOD;
	vis_pst_16(dr, dpz, emask & 0x7);

	if ((mlib_addr)++dpz <= (mlib_addr)pzend) {
		vis_pst_16(dr, dpz, 0x8);
	}

	end = dpzend - dpz;

	if (end < 0)
		return (MLIB_SUCCESS);
	dpx++;
	dpy++;
	delta_x = (((mlib_addr)z & 7) - 2) ? 2 : 4;
	x += delta_x;

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
			dr = vis_freg_pair(sr0, sr1);
			dr1 = vis_fpmerge(sr3, sr2);
			dr1 = vis_fpmerge(vis_read_lo(dr1), vis_read_hi(dr1));
			r_buff[i] = dr;
			x_buff[i] = dr1;
		}

		for (i = 0; i < 256; i++) {
			dr = r_buff[i];
			dr1 = x_buff[i];
			PACK_S32C_S16C1_MOD;
			vis_pst_16(dr, dpz + 256 * k + i, 0x7);
			vis_pst_16(dr, dpz + 256 * k + i + 1, 0x8);
		}
	}

	N = end & ~255;
	__mlib_VectorCopy_U8((void *)x_buff, (void *)((mlib_d64 *)x + N),
		(end - N) * 8 + ((mlib_addr)pzend & 7));
	(void *)vis_alignaddr((void *)y, off);
#pragma pipeloop(0)
	for (i = N; i < end; i++) {
		dx = x_buff[i - N];
		dy0 = dpy[i];
		dy1 = vis_ld_d64_nf(dpy + i + 1);
		dy = vis_faligndata(dy0, dy1);
		MUL_S16C_S32C;
		dr = vis_freg_pair(sr0, sr1);
		dr1 = vis_fpmerge(sr3, sr2);
		dr1 = vis_fpmerge(vis_read_lo(dr1), vis_read_hi(dr1));
		r_buff[i - N] = dr;
		x_buff[i - N] = dr1;
	}

	for (i = N; i < end; i++) {
		dr = r_buff[i - N];
		dr1 = x_buff[i - N];
		PACK_S32C_S16C1_MOD;
		vis_pst_16(dr, dpz + i, 0x7);
		vis_pst_16(dr, dpz + i + 1, 0x8);
	}

	x -= delta_x;
	dpx--;
	dpy--;

	if ((mlib_addr)dpzend < (mlib_addr)pzend) {
		dx = x_buff[end - N];
		dy0 = vis_ld_d64_nf(dpy + end + 1);
		dy1 = vis_ld_d64_nf(dpy + end + 2);
		dy = vis_faligndata(dy0, dy1);
		MUL_S16C_S32C;
		dr = vis_freg_pair(sr0, sr1);
		dr1 = vis_fpmerge(sr3, sr2);
		dr1 = vis_fpmerge(vis_read_lo(dr1), vis_read_hi(dr1));
		PACK_S32C_S16C1_MOD;
/* prepare edge mask for the last bytes */
		emask = vis_edge16(dpzend, pzend);
/* store last bytes of result */
		vis_pst_16(dr, dpzend, emask & 0x7);

		if ((mlib_addr)++dpzend <= (mlib_addr)pzend) {
			vis_pst_16(dr, dpzend, 0x8);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dy0, dy1;
	mlib_d64 dzh, dzl;
	mlib_s16 *px, *py;
	mlib_s32 *pz;

/* odd = 1 means 8-byte aligned resultant vector starts with Im. part. */
	mlib_s32 odd = 0;
	mlib_s32 len = n, i;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_f32 fzero = vis_fzeros();
	mlib_f32 sr0, sr1, sr2, sr3;
	mlib_f32 rest;
	mlib_d64 drest;
	mlib_s32 re_x, im_x, re_y, im_y, re_c, im_c;
	mlib_d64 x_buff[256];
	mlib_s32 N, k;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s16 *)x;
	py = (mlib_s16 *)y;
	pz = (mlib_s32 *)z;

	if (n <= 8)
		MUL_S32C_S16C_MOD_IN_C;

/*
 * prepare the destination address trying to align it for 8 bytes.
 */

	while ((mlib_addr)pz & 7) {
		re_x = (*px++);
		im_x = (*px++);
		re_y = (*py++);
		im_y = (*py++);
		re_c = re_x * re_y - im_x * im_y;
		im_c = im_x * re_y + re_x * im_y;
		re_c = re_c;
		im_c = im_c;
		len--;
		(*pz++) = re_c;

		if (!((mlib_addr)pz & 7)) {
			odd = 1;
			rest = vis_read_hi(vis_to_double(im_c, 0));
			break;
		}

		(*pz++) = im_c;
	}

	dpz = (mlib_d64 *)pz;
	len <<= 2;
	even_8 = len >> 3;
	rest_8 = len & 0x7;
	pz += (even_8 << 2);

	if (!(((mlib_addr)px | (mlib_addr)py) & 7)) {

/*
 * Both addresses are 8-byte aligned.
 * No  vis_alignaddr and  vis_faligndata at all.
 */

		dpx = (mlib_d64 *)px;
		dpy = (mlib_d64 *)py;

		(void) vis_alignaddr(dpx, 6);

		if (!odd) {
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = dpx[i];
				dy = dpy[i];
				MUL_S16C_S32C;
				COMPOSE_S32C_FROM32_MOD;
				dpz[2 * i] = dzh;
				dpz[2 * i + 1] = dzl;
			}
		} else {
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = dpx[i];
				dy = dpy[i];
				MUL_S16C_S32C;
				COMPOSE_S32C_FROM32_MOD_WITH_ODD;
				dpz[2 * i] = dzh;
				dpz[2 * i + 1] = dzl;
			}

			vis_alignaddr((void *)6, 0);
			drest = vis_freg_pair(rest, fzero);
			vis_pst_32(drest, pz, 0x2);
			pz++;
		}
	} else {

/*
 * Both ("x" and "y") address are arbitrary aligned.
 * 2 vis_alignaddr(s) and 2 vis_faligndata(s) in the loop.
 */
		dpx = (void *)((mlib_addr)px & ~7);
		dpy = (void *)((mlib_addr)py & ~7);

		if (odd) {

			for (k = 0; k < (even_8 >> 8); k++) {
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + k * 256),
					256 * 8);
				vis_alignaddr(py, 0);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = x_buff[i];
					dy0 = dpy[k * 256 + i];
					dy1 = vis_ld_d64_nf(dpy +
					    k * 256 + i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S16C_S32C;
					COMPOSE_S32C_FROM32_MOD_WITH_ODD;
					dpz[512 * k + 2 * i] = dzh;
					dpz[512 * k + 2 * i + 1] = dzl;
				}
			}

			N = even_8 & ~255;

			if (even_8 > 0)
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + N),
					(even_8 - N) * 8);
			vis_alignaddr(py, 0);
#pragma pipeloop(0)
			for (i = N; i < even_8; i++) {
				dx = x_buff[i - N];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S16C_S32C;
				COMPOSE_S32C_FROM32_MOD_WITH_ODD;
				dpz[2 * i] = dzh;
				dpz[2 * i + 1] = dzl;
			}

			drest = vis_freg_pair(rest, fzero);
			vis_pst_32(drest, pz, 0x2);
			pz++;
		} else {

			for (k = 0; k < (even_8 >> 8); k++) {
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + k * 256),
					256 * 8);
				vis_alignaddr(py, 0);
#pragma pipeloop(0)
				for (i = 0; i < 256; i++) {
					dx = x_buff[i];
					dy0 = dpy[k * 256 + i];
					dy1 = vis_ld_d64_nf(dpy +
					    k * 256 + i + 1);
					dy = vis_faligndata(dy0, dy1);
					MUL_S16C_S32C;
					COMPOSE_S32C_FROM32_MOD;
					dpz[512 * k + 2 * i] = dzh;
					dpz[512 * k + 2 * i + 1] = dzl;
				}
			}

			N = even_8 & ~255;

			if (even_8 > 0)
				__mlib_VectorCopy_U8((void *)x_buff,
					(void *)((mlib_d64 *)px + N),
					(even_8 - N) * 8);
			vis_alignaddr(py, 0);
#pragma pipeloop(0)
			for (i = N; i < even_8; i++) {
				dx = x_buff[i - N];
				dy0 = dpy[i];
				dy1 = vis_ld_d64_nf(dpy + i + 1);
				dy = vis_faligndata(dy0, dy1);
				MUL_S16C_S32C;
				COMPOSE_S32C_FROM32_MOD;
				dpz[2 * i] = dzh;
				dpz[2 * i + 1] = dzl;
			}
		}
	}

	px += (even_8 << 2);
	py += (even_8 << 2);

	while (rest_8 > 0) {
		re_x = (*px++);
		im_x = (*px++);
		re_y = (*py++);
		im_y = (*py++);
		re_c = re_x * re_y - im_x * im_y;
		im_c = im_x * re_y + re_x * im_y;
		re_c = re_c;
		im_c = im_c;
		(*pz++) = re_c;
		(*pz++) = im_c;
		rest_8 -= 4;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16C_Mod(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorMul_S16C_S16C_Mod(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S32C_S32C_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	mlib_s32 *px = (mlib_s32 *)x, *py = (mlib_s32 *)y, *pz = (mlib_s32 *)z;
	mlib_s32 i;

#define	MAX	0x100000
#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		mlib_d64 mul_r, mul_i, dy0 = py[2 * i];
		mlib_d64 dy0_lo = py[2 * i] & (MAX - 1);
		mlib_d64 dy1 = py[2 * i + 1];
		mlib_d64 dy1_lo = py[2 * i + 1] & (MAX - 1);

		dy0 -= dy0_lo;
		dy1 -= dy1_lo;
		mul_r = px[2 * i] * dy0 - px[2 * i + 1] * dy1 +
			(px[2 * i] * dy0_lo - px[2 * i + 1] * dy1_lo);
		mul_i = px[2 * i + 1] * dy0 + px[2 * i] * dy1 +
			(px[2 * i + 1] * dy0_lo + px[2 * i] * dy1_lo);
		FLOAT2INT_CLAMP(pz[2 * i], mul_r);
		FLOAT2INT_CLAMP(pz[2 * i + 1], mul_i);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S32C_Sat(
	mlib_s32 *xz,
	const mlib_s32 *y,
	mlib_s32 n)
{
	return (__mlib_VectorMul_S32C_S32C_Sat(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S32C_S32C_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	mlib_s32 *px = (mlib_s32 *)x, *py = (mlib_s32 *)y, *pz = (mlib_s32 *)z;
	mlib_s32 i;
	mlib_d64 d16 = 0x10000, _d16 = 1 / d16;
	mlib_d64 d15 = d16 * 0.5;
	mlib_d64 d31 = d16 * d15, _d31 = 1 / d31;
	mlib_d64 yr_hi, yr_lo, yi_hi, yi_lo, tmp;
	mlib_d64 mulr_hi, mulr_lo, muli_hi, muli_lo;
	mlib_s32 xr, yr, oner;
	mlib_s32 xi, yi, onei;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		yr_hi = ((mlib_s16 *)(py + 2 * i))[0];
		yr_lo = ((mlib_u16 *)(py + 2 * i))[1];
		yi_hi = ((mlib_s16 *)(py + 2 * i))[2];
		yi_lo = ((mlib_u16 *)(py + 2 * i))[3];

		yr = py[2 * i];
		xr = px[2 * i];
		yi = py[2 * i + 1];
		xi = px[2 * i + 1];

		oner = ((xr & yr) ^ (xi & yi)) & 1;
		onei = ((xr & yi) ^ (xi & yr)) & 1;

		mulr_hi =
			(mlib_d64)px[2 * i] * yr_hi - (mlib_d64)px[2 * i +
			1] * yi_hi;
		mulr_lo =
			((mlib_d64)px[2 * i] * yr_lo - (mlib_d64)px[2 * i +
			1] * yi_lo - (mlib_d64)oner) * 0.5;

		muli_hi =
			(mlib_d64)px[2 * i] * yi_hi + (mlib_d64)px[2 * i +
			1] * yr_hi;
		muli_lo =
			((mlib_d64)px[2 * i] * yi_lo + (mlib_d64)px[2 * i +
			1] * yr_lo - (mlib_d64)onei) * 0.5;

		FLOAT2INT_CLAMP(tmp, mulr_hi * _d16);
		mulr_hi = mulr_hi * d15 - tmp * d31 + mulr_lo;
		FLOAT2INT_CLAMP(tmp, mulr_hi * _d31);
		mulr_hi = mulr_hi - tmp * d31;
		FLOAT2INT_CLAMP(tmp, muli_hi * _d16);
		muli_hi = muli_hi * d15 - tmp * d31 + muli_lo;
		FLOAT2INT_CLAMP(tmp, muli_hi * _d31);
		muli_hi = muli_hi - tmp * d31;

		pz[2 * i] = ((mlib_s32)mulr_hi << 1) | oner;
		pz[2 * i + 1] = ((mlib_s32)muli_hi << 1) | onei;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S32C_Mod(
	mlib_s32 *xz,
	const mlib_s32 *y,
	mlib_s32 n)
{
	return (__mlib_VectorMul_S32C_S32C_Mod(xz, xz, y, n));
}

/* *********************************************************** */
