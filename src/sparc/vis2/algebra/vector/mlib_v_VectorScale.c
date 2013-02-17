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

#pragma ident	"@(#)mlib_v_VectorScale.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorScale_U8_[Sat|Mod]
 *      mlib_VectorScale_[U8|S16]_U8_[Sat|Mod]   - linear function on
 *                                                 unsigned 8-bit format vector
 *      mlib_VectorScale_S8_[Sat|Mod]
 *      mlib_VectorScale_[S8|S16]_S8_[Sat|Mod]   - linear function on
 *                                                 signed 8-bit format vector
 *      mlib_VectorScale_S16_[Sat|Mod]
 *      mlib_VectorScale_[S16|S32]_S16_[Sat|Mod] - linear function on
 *                                                 signed 16-bit format vector
 *      mlib_VectorScale_S32_[Sat|Mod]
 *      mlib_VectorScale_S32_S32_[Sat|Mod]       - linear function on
 *                                                 signed 32-bit format vector
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorScale_S16_[Sat|Mod](mlib_s16       *xz,
 *                                                 const mlib_s16 *a,
 *                                                 const mlib_s16 *b,
 *                                                 mlib_s32       n);
 *      mlib_status mlib_VectorScale_S16_S16_[Sat|Mod](mlib_s16       *z,
 *                                                     const mlib_s16 *x,
 *                                                     const mlib_s16 *a,
 *                                                     const mlib_s16 *b,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_VectorScale_S16_U8_[Sat|Mod](mlib_s16      *z,
 *                                                    const mlib_u8 *x,
 *                                                    const mlib_u8 *a,
 *                                                    const mlib_u8 *b,
 *                                                    mlib_s32      n);
 *      mlib_status mlib_VectorScale_S32_[Sat|Mod](mlib_s32       *xz,
 *                                                 const mlib_s32 *a,
 *                                                 const mlib_s32 *b,
 *                                                 mlib_s32       n);
 *      mlib_status mlib_VectorScale_S32_S32_[Sat|Mod](mlib_s32       *z,
 *                                                     const mlib_s32 *x,
 *                                                     const mlib_s32 *a,
 *                                                     const mlib_s32 *b,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_VectorScale_S8_[Sat|Mod](mlib_s8       *xz,
 *                                                const mlib_s8 *a,
 *                                                const mlib_s8 *b,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorScale_S8_S8_[Sat|Mod](mlib_s8       *z,
 *                                                   const mlib_s8 *x,
 *                                                   const mlib_s8 *a,
 *                                                   const mlib_s8 *b,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_VectorScale_U8_[Sat|Mod](mlib_u8       *xz,
 *                                                const mlib_u8 *a,
 *                                                const mlib_u8 *b,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorScale_U8_U8_[Sat|Mod](mlib_u8       *z,
 *                                                   const mlib_u8 *x,
 *                                                   const mlib_u8 *a,
 *                                                   const mlib_u8 *b,
 *                                                   mlib_s32      n);
 *
 *      mlib_status mlib_VectorScale_S16_S8_Mod(mlib_s16      *z,
 *                                              const mlib_s8 *x,
 *                                              const mlib_s8 *a,
 *                                              const mlib_s8 *b,
 *                                              mlib_s32      n);
 *      mlib_status mlib_VectorScale_S32_S16_Mod(mlib_s32       *z,
 *                                               const mlib_s16 *x,
 *                                               const mlib_s16 *a,
 *                                               const mlib_s16 *b,
 *                                               mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      x    pointer to the first element of the input vector
 *      a    pointer to the scaling factor
 *      b    pointer to the offset
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *      Z = a * X + b
 */

/*
 * FUNCTIONS
 *      mlib_VectorScale_U8C_[Sat|Mod]
 *      mlib_VectorScale_[U8C|S16C]_U8C_[Sat|Mod]- linear function on
 *                                                 unsigned 8-bit format vector
 *      mlib_VectorScale_S8C_[Sat|Mod]
 *      mlib_VectorScale_[S8C|S16C]_S8C_[Sat|Mod]- linear function on
 *                                                 signed 8-bit format vector
 *      mlib_VectorScale_S16C_[Sat|Mod]
 *      mlib_VectorScale_[S16C|S32C]_S16C_[Sat|Mod] - linear function on
 *                                                  signed 16-bit format vector
 *      mlib_VectorScale_S32C_[Sat|Mod]
 *      mlib_VectorScale_S32C_S32C_[Sat|Mod]     - linear function on
 *                                                 signed 32-bit format vector
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorScale_S16C_[Sat|Mod](mlib_s16       *xz,
 *                                                  const mlib_s16 *a,
 *                                                  const mlib_s16 *b,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_VectorScale_S16C_S16C_[Sat|Mod](mlib_s16       *z,
 *                                                       const mlib_s16 *x,
 *                                                       const mlib_s16 *a,
 *                                                       const mlib_s16 *b,
 *                                                       mlib_s32       n);
 *      mlib_status mlib_VectorScale_S16C_S8C_[Sat|Mod](mlib_s16      *z,
 *                                                      const mlib_s8 *x,
 *                                                      const mlib_s8 *a,
 *                                                      const mlib_s8 *b,
 *                                                      mlib_s32      n);
 *      mlib_status mlib_VectorScale_S16C_U8C_[Sat|Mod](mlib_s16      *z,
 *                                                      const mlib_u8 *x,
 *                                                      const mlib_u8 *a,
 *                                                      const mlib_u8 *b,
 *                                                      mlib_s32      n);
 *      mlib_status mlib_VectorScale_S32C_[Sat|Mod](mlib_s32       *xz,
 *                                                  const mlib_s32 *a,
 *                                                  const mlib_s32 *b,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_VectorScale_S32C_S16C_[Sat|Mod](mlib_s32       *z,
 *                                                       const mlib_s16 *x,
 *                                                       const mlib_s16 *a,
 *                                                       const mlib_s16 *b,
 *                                                       mlib_s32       n);
 *      mlib_status mlib_VectorScale_S32C_S32C_[Sat|Mod](mlib_s32       *z,
 *                                                       const mlib_s32 *x,
 *                                                       const mlib_s32 *a,
 *                                                       const mlib_s32 *b,
 *                                                       mlib_s32       n);
 *      mlib_status mlib_VectorScale_S8C_[Sat|Mod](mlib_s8       *xz,
 *                                                 const mlib_s8 *a,
 *                                                 const mlib_s8 *b,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_VectorScale_S8C_S8C_[Sat|Mod](mlib_s8       *z,
 *                                                     const mlib_s8 *x,
 *                                                     const mlib_s8 *a,
 *                                                     const mlib_s8 *b,
 *                                                     mlib_s32      n);
 *      mlib_status mlib_VectorScale_U8C_[Sat|Mod](mlib_u8       *xz,
 *                                                 const mlib_u8 *a,
 *                                                 const mlib_u8 *b,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_VectorScale_U8C_U8C_[Sat|Mod](mlib_u8       *z,
 *                                                     const mlib_u8 *x,
 *                                                     const mlib_u8 *a,
 *                                                     const mlib_u8 *b,
 *                                                     mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result vector.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts
 *      x    pointer to the first element of the input complex vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      a    pointer to a complex scaling factor. a[0] hold the real part,
 *           and a[1] hold the imaginary part
 *      b    pointer to a complex offset. b[0] hold the real part,
 *           and b[1] hold the imaginary part
 *      n    number of complex elements in the vectors
 *
 * DESCRIPTION
 *      Z = a * X + b
 */

#include <stdlib.h>
#include <mlib_algebra.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorScale_S16_S8_Mod = __mlib_VectorScale_S16_S8_Mod
#pragma weak mlib_VectorScale_S16C_Sat = __mlib_VectorScale_S16C_Sat
#pragma weak mlib_VectorScale_S16_U8_Mod = __mlib_VectorScale_S16_U8_Mod
#pragma weak mlib_VectorScale_S16C_S8C_Mod = __mlib_VectorScale_S16C_S8C_Mod
#pragma weak mlib_VectorScale_S8_Mod = __mlib_VectorScale_S8_Mod
#pragma weak mlib_VectorScale_S16C_U8C_Mod = __mlib_VectorScale_S16C_U8C_Mod
#pragma weak mlib_VectorScale_U8_Mod = __mlib_VectorScale_U8_Mod
#pragma weak mlib_VectorScale_S8C_Mod = __mlib_VectorScale_S8C_Mod
#pragma weak mlib_VectorScale_S16_U8_Sat = __mlib_VectorScale_S16_U8_Sat
#pragma weak mlib_VectorScale_U8C_Mod = __mlib_VectorScale_U8C_Mod
#pragma weak mlib_VectorScale_S16C_S8C_Sat = __mlib_VectorScale_S16C_S8C_Sat
#pragma weak mlib_VectorScale_S8_Sat = __mlib_VectorScale_S8_Sat
#pragma weak mlib_VectorScale_S16C_U8C_Sat = __mlib_VectorScale_S16C_U8C_Sat
#pragma weak mlib_VectorScale_S8_S8_Mod = __mlib_VectorScale_S8_S8_Mod
#pragma weak mlib_VectorScale_U8_Sat = __mlib_VectorScale_U8_Sat
#pragma weak mlib_VectorScale_S8C_Sat = __mlib_VectorScale_S8C_Sat
#pragma weak mlib_VectorScale_U8C_Sat = __mlib_VectorScale_U8C_Sat
#pragma weak mlib_VectorScale_S32_Mod = __mlib_VectorScale_S32_Mod
#pragma weak mlib_VectorScale_U8_U8_Mod = __mlib_VectorScale_U8_U8_Mod
#pragma weak mlib_VectorScale_S16_Mod = __mlib_VectorScale_S16_Mod
#pragma weak mlib_VectorScale_S8C_S8C_Mod = __mlib_VectorScale_S8C_S8C_Mod
#pragma weak mlib_VectorScale_S8_S8_Sat = __mlib_VectorScale_S8_S8_Sat
#pragma weak mlib_VectorScale_U8C_U8C_Mod = __mlib_VectorScale_U8C_U8C_Mod
#pragma weak mlib_VectorScale_S32_Sat = __mlib_VectorScale_S32_Sat
#pragma weak mlib_VectorScale_U8_U8_Sat = __mlib_VectorScale_U8_U8_Sat
#pragma weak mlib_VectorScale_S16_Sat = __mlib_VectorScale_S16_Sat
#pragma weak mlib_VectorScale_S8C_S8C_Sat = __mlib_VectorScale_S8C_S8C_Sat
#pragma weak mlib_VectorScale_U8C_U8C_Sat = __mlib_VectorScale_U8C_U8C_Sat
#pragma weak mlib_VectorScale_S32_S32_Mod = __mlib_VectorScale_S32_S32_Mod
#pragma weak mlib_VectorScale_S32_S16_Mod = __mlib_VectorScale_S32_S16_Mod
#pragma weak mlib_VectorScale_S16_S16_Mod = __mlib_VectorScale_S16_S16_Mod
#pragma weak mlib_VectorScale_S32C_S32C_Mod = \
	__mlib_VectorScale_S32C_S32C_Mod
#pragma weak mlib_VectorScale_S32_S32_Sat = __mlib_VectorScale_S32_S32_Sat
#pragma weak mlib_VectorScale_S32C_S16C_Mod = \
	__mlib_VectorScale_S32C_S16C_Mod
#pragma weak mlib_VectorScale_S16C_S16C_Mod = \
	__mlib_VectorScale_S16C_S16C_Mod
#pragma weak mlib_VectorScale_S32C_Mod = __mlib_VectorScale_S32C_Mod
#pragma weak mlib_VectorScale_S16_S16_Sat = __mlib_VectorScale_S16_S16_Sat
#pragma weak mlib_VectorScale_S16C_Mod = __mlib_VectorScale_S16C_Mod
#pragma weak mlib_VectorScale_S32C_S32C_Sat = \
	__mlib_VectorScale_S32C_S32C_Sat
#pragma weak mlib_VectorScale_S32C_S16C_Sat = \
	__mlib_VectorScale_S32C_S16C_Sat
#pragma weak mlib_VectorScale_S16C_S16C_Sat = \
	__mlib_VectorScale_S16C_S16C_Sat
#pragma weak mlib_VectorScale_S32C_Sat = __mlib_VectorScale_S32C_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorScale_S16_S8_Mod) mlib_VectorScale_S16_S8_Mod
	__attribute__((weak, alias("__mlib_VectorScale_S16_S8_Mod")));
__typeof__(__mlib_VectorScale_S16C_Sat) mlib_VectorScale_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorScale_S16C_Sat")));
__typeof__(__mlib_VectorScale_S16_U8_Mod) mlib_VectorScale_S16_U8_Mod
	__attribute__((weak, alias("__mlib_VectorScale_S16_U8_Mod")));
__typeof__(__mlib_VectorScale_S16C_S8C_Mod) mlib_VectorScale_S16C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorScale_S16C_S8C_Mod")));
__typeof__(__mlib_VectorScale_S8_Mod) mlib_VectorScale_S8_Mod
	__attribute__((weak, alias("__mlib_VectorScale_S8_Mod")));
__typeof__(__mlib_VectorScale_S16C_U8C_Mod) mlib_VectorScale_S16C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorScale_S16C_U8C_Mod")));
__typeof__(__mlib_VectorScale_U8_Mod) mlib_VectorScale_U8_Mod
	__attribute__((weak, alias("__mlib_VectorScale_U8_Mod")));
__typeof__(__mlib_VectorScale_S8C_Mod) mlib_VectorScale_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorScale_S8C_Mod")));
__typeof__(__mlib_VectorScale_S16_U8_Sat) mlib_VectorScale_S16_U8_Sat
	__attribute__((weak, alias("__mlib_VectorScale_S16_U8_Sat")));
__typeof__(__mlib_VectorScale_U8C_Mod) mlib_VectorScale_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorScale_U8C_Mod")));
__typeof__(__mlib_VectorScale_S16C_S8C_Sat) mlib_VectorScale_S16C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorScale_S16C_S8C_Sat")));
__typeof__(__mlib_VectorScale_S8_Sat) mlib_VectorScale_S8_Sat
	__attribute__((weak, alias("__mlib_VectorScale_S8_Sat")));
__typeof__(__mlib_VectorScale_S16C_U8C_Sat) mlib_VectorScale_S16C_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorScale_S16C_U8C_Sat")));
__typeof__(__mlib_VectorScale_S8_S8_Mod) mlib_VectorScale_S8_S8_Mod
	__attribute__((weak, alias("__mlib_VectorScale_S8_S8_Mod")));
__typeof__(__mlib_VectorScale_U8_Sat) mlib_VectorScale_U8_Sat
	__attribute__((weak, alias("__mlib_VectorScale_U8_Sat")));
__typeof__(__mlib_VectorScale_S8C_Sat) mlib_VectorScale_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorScale_S8C_Sat")));
__typeof__(__mlib_VectorScale_U8C_Sat) mlib_VectorScale_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorScale_U8C_Sat")));
__typeof__(__mlib_VectorScale_S32_Mod) mlib_VectorScale_S32_Mod
	__attribute__((weak, alias("__mlib_VectorScale_S32_Mod")));
__typeof__(__mlib_VectorScale_U8_U8_Mod) mlib_VectorScale_U8_U8_Mod
	__attribute__((weak, alias("__mlib_VectorScale_U8_U8_Mod")));
__typeof__(__mlib_VectorScale_S16_Mod) mlib_VectorScale_S16_Mod
	__attribute__((weak, alias("__mlib_VectorScale_S16_Mod")));
__typeof__(__mlib_VectorScale_S8C_S8C_Mod) mlib_VectorScale_S8C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorScale_S8C_S8C_Mod")));
__typeof__(__mlib_VectorScale_S8_S8_Sat) mlib_VectorScale_S8_S8_Sat
	__attribute__((weak, alias("__mlib_VectorScale_S8_S8_Sat")));
__typeof__(__mlib_VectorScale_U8C_U8C_Mod) mlib_VectorScale_U8C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorScale_U8C_U8C_Mod")));
__typeof__(__mlib_VectorScale_S32_Sat) mlib_VectorScale_S32_Sat
	__attribute__((weak, alias("__mlib_VectorScale_S32_Sat")));
__typeof__(__mlib_VectorScale_U8_U8_Sat) mlib_VectorScale_U8_U8_Sat
	__attribute__((weak, alias("__mlib_VectorScale_U8_U8_Sat")));
__typeof__(__mlib_VectorScale_S16_Sat) mlib_VectorScale_S16_Sat
	__attribute__((weak, alias("__mlib_VectorScale_S16_Sat")));
__typeof__(__mlib_VectorScale_S8C_S8C_Sat) mlib_VectorScale_S8C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorScale_S8C_S8C_Sat")));
__typeof__(__mlib_VectorScale_U8C_U8C_Sat) mlib_VectorScale_U8C_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorScale_U8C_U8C_Sat")));
__typeof__(__mlib_VectorScale_S32_S32_Mod) mlib_VectorScale_S32_S32_Mod
	__attribute__((weak, alias("__mlib_VectorScale_S32_S32_Mod")));
__typeof__(__mlib_VectorScale_S32_S16_Mod) mlib_VectorScale_S32_S16_Mod
	__attribute__((weak, alias("__mlib_VectorScale_S32_S16_Mod")));
__typeof__(__mlib_VectorScale_S16_S16_Mod) mlib_VectorScale_S16_S16_Mod
	__attribute__((weak, alias("__mlib_VectorScale_S16_S16_Mod")));
__typeof__(__mlib_VectorScale_S32C_S32C_Mod) mlib_VectorScale_S32C_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorScale_S32C_S32C_Mod")));
__typeof__(__mlib_VectorScale_S32_S32_Sat) mlib_VectorScale_S32_S32_Sat
	__attribute__((weak, alias("__mlib_VectorScale_S32_S32_Sat")));
__typeof__(__mlib_VectorScale_S32C_S16C_Mod) mlib_VectorScale_S32C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorScale_S32C_S16C_Mod")));
__typeof__(__mlib_VectorScale_S16C_S16C_Mod) mlib_VectorScale_S16C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorScale_S16C_S16C_Mod")));
__typeof__(__mlib_VectorScale_S32C_Mod) mlib_VectorScale_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorScale_S32C_Mod")));
__typeof__(__mlib_VectorScale_S16_S16_Sat) mlib_VectorScale_S16_S16_Sat
	__attribute__((weak, alias("__mlib_VectorScale_S16_S16_Sat")));
__typeof__(__mlib_VectorScale_S16C_Mod) mlib_VectorScale_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorScale_S16C_Mod")));
__typeof__(__mlib_VectorScale_S32C_S32C_Sat) mlib_VectorScale_S32C_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorScale_S32C_S32C_Sat")));
__typeof__(__mlib_VectorScale_S32C_S16C_Sat) mlib_VectorScale_S32C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorScale_S32C_S16C_Sat")));
__typeof__(__mlib_VectorScale_S16C_S16C_Sat) mlib_VectorScale_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorScale_S16C_S16C_Sat")));
__typeof__(__mlib_VectorScale_S32C_Sat) mlib_VectorScale_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorScale_S32C_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static const mlib_u8 mlib_mask_16_8[] = {
	0, 1, 4, 5, 16, 17, 20, 21, 64, 65, 68, 69, 80, 81, 84, 85
};

/* *********************************************************** */

#define	SCALE_S16_S8_MOD                                        \
	dr = vis_fpmerge(vis_read_hi(dx), fzero);               \
	dr1 = vis_fpmerge(vis_read_lo(dx), fzero);              \
	dr2 = vis_fmul8sux16(dr, da);                           \
	dr3 = vis_fmul8sux16(dr1, da);                          \
	dzh = vis_fpadd16(dr2, db);                             \
	dzl = vis_fpadd16(dr3, db)

/* *********************************************************** */

#define	SCALE_S16C_S8C_MOD                                      \
	{                                                       \
	    mlib_d64 dr2, dr3;                                  \
	                                                        \
	    dr = vis_fmul8sux16(dx, da0);                       \
	    dr1 = vis_fmul8sux16(dx, da1);                      \
	    dx = vis_fpack32(dx, dx);                           \
	    dr2 = vis_fmul8sux16(dx, da0);                      \
	    dr3 = vis_fmul8sux16(dx, da1);                      \
	    dzh = vis_fpsub16(dr, dr3);                         \
	    dzl = vis_fpadd16(dr2, dr1);                        \
	    dr = vis_fpadd16(dzh, db0);                         \
	    dr1 = vis_fpadd16(dzl, db1);                        \
	}

/* *********************************************************** */

#define	COMPOSE_S16C_MOD_WITH_ODD                               \
	{                                                       \
	    mlib_d64 re2, re3, re4, im2, im3, im4;              \
	                                                        \
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
	}

/* *********************************************************** */

#define	SCALE_S16_U8_MOD                                        \
	dr = vis_fmul8x16al(vis_read_hi(dx), fa);               \
	dr1 = vis_fmul8x16al(vis_read_lo(dx), fa);              \
	dr2 = vis_fmul8x16al(vis_read_hi(dx), f127);            \
	dr3 = vis_fmul8x16al(vis_read_lo(dx), f127);            \
	dr4 = vis_fpsub16(dr2, dr);                             \
	dr5 = vis_fpsub16(dr3, dr1);                            \
	dzh = vis_fpadd16(db, dr4);                             \
	dzl = vis_fpadd16(db, dr5)

/* *********************************************************** */

#define	SCALE_S16_U8_SAT                                        \
	dr = vis_fmul8x16al(vis_read_hi(dx), fa);               \
	dr1 = vis_fmul8x16al(vis_read_lo(dx), fa);              \
	dr2 = vis_fmul8x16al(vis_read_hi(dx), f127);            \
	dr3 = vis_fmul8x16al(vis_read_lo(dx), f127);            \
	dr4 = vis_fpsub16(dr2, dr);                             \
	dr5 = vis_fpsub16(dr3, dr1);                            \
	mask = vis_fcmplt16(dr4, dzero);                        \
	mask1 = vis_fcmplt16(dr5, dzero);                       \
	dzh = vis_fpadd16(db, dr4);                             \
	dzl = vis_fpadd16(db, dr5);                             \
	mask |= vis_fcmplt16(dzh, dzero);                       \
	mask1 |= vis_fcmplt16(dzl, dzero)

/* *********************************************************** */

#define	SCALE_S16_U8_MOD_SHORT                                  \
	dr1 = vis_fmul8x16al(vis_read_hi(dx), fa);              \
	dr2 = vis_fmul8x16al(vis_read_lo(dx), fa);              \
	dzh = vis_fpadd16(db, dr1);                             \
	dzl = vis_fpadd16(db, dr2)

/* *********************************************************** */

#define	SCALE_S16C_U8C_FOR16_MOD                                \
	{                                                       \
	    dr4 = vis_fmuld8ulx16(vis_read_hi(dx), fa0);        \
	    dr6 = vis_fmuld8ulx16(vis_read_hi(dx), fa1);        \
	    dr5 = vis_fmuld8ulx16(vis_read_lo(dx), fa0);        \
	    dr7 = vis_fmuld8ulx16(vis_read_lo(dx), fa1);        \
	    dr = vis_faligndata(dx, dx);                        \
	    dr1 = vis_fmuld8ulx16(vis_read_lo(dr), fa0);        \
	    dr3 = vis_fmuld8ulx16(vis_read_lo(dr), fa1);        \
	    dr2 = vis_fmuld8ulx16(vis_read_hi(dr), fa1);        \
	    dr = vis_fmuld8ulx16(vis_read_hi(dr), fa0);         \
	    dr = vis_fpsub32(dr, dr6);                          \
	    dr1 = vis_fpsub32(dr1, dr7);                        \
	    dr2 = vis_fpadd32(dr2, dr4);                        \
	    dr3 = vis_fpadd32(dr3, dr5);                        \
	    dr = vis_fpadd32(dr, db0);                          \
	    dr1 = vis_fpadd32(dr1, db0);                        \
	    dr2 = vis_fpadd32(dr2, db1);                        \
	    dr3 = vis_fpadd32(dr3, db1);                        \
	}

/* *********************************************************** */

#define	COMPOSE_S16C_FROM32_MOD                                 \
	{                                                       \
	    vis_alignaddr((void *)2, 0);                        \
	    dr = vis_faligndata(dr, dr);                        \
	/*                                                      \
	 * re_1_2                                               \
	 */                                                     \
	    dr1 = vis_faligndata(dr1, dr1);                     \
	/*                                                      \
	 * re_3_4                                               \
	 */                                                     \
	    dr = vis_fand(dr, const_clear);                     \
	    dr1 = vis_fand(dr1, const_clear);                   \
	    dr2 = vis_fandnot(const_clear, dr2);                \
	    dr3 = vis_fandnot(const_clear, dr3);                \
	    dzh = vis_for(dr, dr2);                             \
	    dzl = vis_for(dr1, dr3);                            \
	}

/* *********************************************************** */

#define	COMPOSE_S16C_FROM32_MOD_WITH_ODD                           \
	{                                                          \
	    COMPOSE_S16C_FROM32_MOD;                               \
	    vis_alignaddr((void *)6, 0);                           \
	                                                           \
	    dr = vis_faligndata(rest, dzh);                        \
	    rest = dzl;                                            \
	    dzl = vis_faligndata(dzh, rest);                       \
	    dzh = dr;                                              \
	}

/* *********************************************************** */

#define	SCALE_S16C_U8C_FOR16                                            \
	{                                                               \
	    SCALE_S16C_U8C_FOR16_MOD;                                   \
	    dr = vis_fpackfix_pair(dr, dr1);                            \
	                                                                \
	    dr1 = vis_fpackfix_pair(dr2, dr3);                          \
	    dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));         \
	    dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));         \
	    dr1 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));      \
	    dr1 = vis_fpmerge(vis_read_hi(dr1), vis_read_lo(dr1));      \
	    dr2 = vis_fpmerge(vis_read_hi(dr), vis_read_hi(dr1));       \
	    dr3 = vis_fpmerge(vis_read_lo(dr), vis_read_lo(dr1));       \
	    dzh = vis_fpmerge(vis_read_hi(dr2), vis_read_hi(dr3));      \
	    dzl = vis_fpmerge(vis_read_lo(dr2), vis_read_lo(dr3));      \
	}

/* *********************************************************** */

#define	SCALE_S16C_U8C_FOR16_WITH_ODD                                   \
	{                                                               \
	    SCALE_S16C_U8C_FOR16_MOD;                                   \
	    dr = vis_fpackfix_pair(dr, dr1);                            \
	                                                                \
	    dr1 = vis_fpackfix_pair(dr2, dr3);                          \
	    (void *)vis_alignaddr(dpx, 6);                              \
	    dr2 = vis_faligndata(rest, dr1);                            \
	    dr2 = vis_fpmerge(vis_read_hi(dr2), vis_read_lo(dr2));      \
	    dr2 = vis_fpmerge(vis_read_hi(dr2), vis_read_lo(dr2));      \
	    dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));         \
	    dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));         \
	    dr3 = vis_fpmerge(vis_read_hi(dr2), vis_read_hi(dr));       \
	    dr2 = vis_fpmerge(vis_read_lo(dr2), vis_read_lo(dr));       \
	    dzh = vis_fpmerge(vis_read_hi(dr3), vis_read_hi(dr2));      \
	    dzl = vis_fpmerge(vis_read_lo(dr3), vis_read_lo(dr2));      \
	    rest = dr1;                                                 \
	}

/* *********************************************************** */

/*
 * Makes from 4_16==4*Re  and  4_16==4*Im --> dzh_4_16==Re1_Im1_Re2_Im2 and
 * dzl_4_16==Re3_Im3_Re4_Im4.
 */

/* *********************************************************** */

#define	COMPOSE_S16C_MOD                                        \
	{                                                       \
	    mlib_d64 re2, re3, re4, im2, im3, im4;              \
	                                                        \
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
	}

/* *********************************************************** */

#define	COMPOSE_S16C_MOD_WITH_ODD                               \
	{                                                       \
	    mlib_d64 re2, re3, re4, im2, im3, im4;              \
	                                                        \
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
	}

/* *********************************************************** */

#define	SCALE_S16                                               \
	{                                                       \
	    mlib_d64 dr2, dr3, dr4, dr5;                        \
	                                                        \
	    dr2 = vis_fmuld8sux16(vis_read_hi(dx), fa);         \
	    dr3 = vis_fmuld8ulx16(vis_read_hi(dx), fa);         \
	    dr = vis_fpadd32(dr2, dr3);                         \
	    dr4 = vis_fmuld8sux16(vis_read_lo(dx), fa);         \
	    dr5 = vis_fmuld8ulx16(vis_read_lo(dx), fa);         \
	    dr1 = vis_fpadd32(dr4, dr5);                        \
	}

/* *********************************************************** */

#define	PACK_S32_S16_SAT                                            \
	dr = vis_fpadd32(dr, db);                                   \
	dr1 = vis_fpadd32(dr1, db);                                 \
	dr3 = vis_fpadd32(dr, dr);                                  \
	dr4 = vis_fpadd32(dr1, dr1);                                \
	dr2 = vis_freg_pair(vis_fpackfix(dr3), vis_fpackfix(dr4))

/* *********************************************************** */

#define	PACK_S32_S16_SAT_LO                                         \
	dr3 = vis_fpadd32(dr, db);                                  \
	dr4 = vis_fpadd32(dr1, db);                                 \
	dr2 = vis_freg_pair(vis_fpackfix(dr3), vis_fpackfix(dr4))

/* *********************************************************** */

#define	SCALE_S16_MOD                                           \
	{                                                       \
	    mlib_d64 dr2, dr3, dr4, dr5;                        \
	                                                        \
	    dr2 = vis_fmuld8sux16(vis_read_hi(dx), fa);         \
	    dr3 = vis_fmuld8ulx16(vis_read_hi(dx), fa);         \
	    dr2 = vis_fpadd32(dr2, db);                         \
	    dr = vis_fpadd32(dr2, dr3);                         \
	    dr4 = vis_fmuld8sux16(vis_read_lo(dx), fa);         \
	    dr5 = vis_fmuld8ulx16(vis_read_lo(dx), fa);         \
	    dr4 = vis_fpadd32(dr4, db);                         \
	    dr1 = vis_fpadd32(dr4, dr5);                        \
	}

/* *********************************************************** */

#define	PACK_S32_S16_MOD                                          \
	{                                                         \
	    mlib_f32 sr0, sr1;                                    \
	                                                          \
	    sr0 = vis_fands(vis_read_lo(dr), without_control);    \
	    dr2 = vis_write_lo(dr, vis_read_hi(dr1));             \
	    dr2 = vis_fpack32(dr2, dzero);                        \
	    sr1 = vis_fands(vis_read_lo(dr1), without_control);   \
	    dr2 = vis_fpack32(dr2, dzero);                        \
	    dr2 = vis_for(dr2, vis_freg_pair(sr0, sr1));          \
	}

/* *********************************************************** */

#define	SCALE_S32_S16_MOD                                       \
	{                                                       \
	    SCALE_S16;                                          \
	    dzh = vis_fpadd32(dr, db);                          \
	                                                        \
	    dzl = vis_fpadd32(dr1, db);                         \
	}

/* *********************************************************** */

#define	SCALE_S16C_S32C                                           \
	dr = vis_fmuld8sux16(vis_read_hi(dx), fc0);               \
	dr1 = vis_fmuld8ulx16(vis_read_hi(dx), fc0);              \
	dr = vis_fpadd32(dr, dr1);                                \
	dr2 = vis_fmuld8sux16(vis_read_hi(dx), fc1);              \
	dr3 = vis_fmuld8ulx16(vis_read_hi(dx), fc1);              \
	dr1 = vis_fpadd32(dr2, dr3);                              \
	sr0 = vis_fpsub32s(vis_read_hi(dr), vis_read_lo(dr));     \
	sr1 = vis_fpadd32s(vis_read_hi(dr1), vis_read_lo(dr1));   \
	dr = vis_freg_pair(sr0, sr1);                             \
	dr4 = vis_fmuld8sux16(vis_read_lo(dx), fc0);              \
	dr5 = vis_fmuld8ulx16(vis_read_lo(dx), fc0);              \
	dr4 = vis_fpadd32(dr4, dr5);                              \
	dr2 = vis_fmuld8sux16(vis_read_lo(dx), fc1);              \
	dr3 = vis_fmuld8ulx16(vis_read_lo(dx), fc1);              \
	dr5 = vis_fpadd32(dr2, dr3);                              \
	sr2 = vis_fpsub32s(vis_read_hi(dr4), vis_read_lo(dr4));   \
	sr3 = vis_fpadd32s(vis_read_hi(dr5), vis_read_lo(dr5))

/* *********************************************************** */

#define	COMPOSE_S32C_FROM32_MOD                                 \
	dr = vis_freg_pair(sr0, sr1);                           \
	dr1 = vis_freg_pair(sr2, sr3);                          \
	dzh = vis_fpadd32(dr, db0);                             \
	dzl = vis_fpadd32(dr1, db0)

/* *********************************************************** */

#define	COMPOSE_S32C_FROM32_MOD_WITH_ODD                        \
	dr = vis_freg_pair(rest, sr0);                          \
	dr1 = vis_freg_pair(sr1, sr2);                          \
	dzh = vis_fpadd32(dr, db1);                             \
	dzl = vis_fpadd32(dr1, db1);                            \
	rest = sr3

/* *********************************************************** */

#define	COMPOSE_S32C_FROM32_SAT                                 \
	COMPOSE_S32C_FROM32_MOD;                                \
	mask = vis_fcmpgt32(cntr_ovlp, dzh);                    \
	mask1 = vis_fcmpgt32(cntr_ovlp, dzl)

/* *********************************************************** */

#define	COMPOSE_S32C_FROM32_SAT_WITH_ODD                        \
	COMPOSE_S32C_FROM32_MOD_WITH_ODD;                       \
	mask = vis_fcmpgt32(cntr_ovlp1, dzh);                   \
	mask1 = vis_fcmpgt32(cntr_ovlp1, dzl)

/* *********************************************************** */

#define	SCALE_S16C_U8C_SAT_IN_C                                        \
	{                                                              \
	    mlib_s32 i;                                                \
	                                                               \
	    for (i = 0; i < n; i++) {                                  \
		re_x = px[2 * i];                                      \
		im_x = px[2 * i + 1];                                  \
		re_c = re_x * ua0 - im_x * ua1 + ub0;                  \
		im_c = im_x * ua0 + re_x * ua1 + ub1;                  \
		pz[2 * i] =                                            \
			re_c < MLIB_S16_MIN ? MLIB_S16_MIN : (re_c >   \
			MLIB_S16_MAX ? MLIB_S16_MAX : re_c);           \
		pz[2 * i + 1] =                                        \
			im_c > MLIB_S16_MAX ? MLIB_S16_MAX : im_c;     \
	    }                                                          \
	    return (MLIB_SUCCESS);                                     \
	}

/* *********************************************************** */

#define	SCALE_S16C_S8C_SAT_IN_C                                      \
	{                                                            \
	    mlib_s32 i;                                              \
	                                                             \
	    for (i = 0; i < n; i++) {                                \
		re_x = px[2 * i];                                    \
		im_x = px[2 * i + 1];                                \
		re_c = re_x * ua0 - im_x * ua1 + ub0;                \
		im_c = im_x * ua0 + re_x * ua1 + ub1;                \
		pz[2 * i] = re_c;                                    \
		pz[2 * i + 1] =                                      \
			im_c > MLIB_S16_MAX ? MLIB_S16_MAX : im_c;   \
	    }                                                        \
	    return (MLIB_SUCCESS);                                   \
	}

/* *********************************************************** */

#define	SCALE_S32_S16_MOD_IN_C                                  \
	{                                                       \
	    mlib_s32 i;                                         \
	                                                        \
	    for (i = 0; i < n; i++)                             \
		pz[i] = px[i] * ua + ub;                        \
	    return (MLIB_SUCCESS);                              \
	}

/* *********************************************************** */

#define	SCALE_S32C_S16C_SAT_IN_C                                \
	{                                                       \
	    mlib_s32 i;                                         \
	                                                        \
	    for (i = 0; i < n; i++) {                           \
		re_x = px[2 * i];                               \
		im_x = px[2 * i + 1];                           \
		re_c = re_x * ua0 - im_x * ua1 + ub0;           \
		im_c = im_x * ua0 + re_x * ua1 + ub1;           \
		pz[2 * i] = re_c;                               \
		pz[2 * i + 1] = im_c;                           \
	    }                                                   \
	    return (MLIB_SUCCESS);                              \
	}

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

#define	SCALE_U8_SAT_LO_1                                       \
	dr1 = vis_fmul8x16al(vis_read_hi(dx), fa);              \
	dr2 = vis_fmul8x16al(vis_read_lo(dx), fa)

/* *********************************************************** */

#define	SCALE_U8_SAT_LO_2                                       \
	dr3 = vis_fpadd16(db, dr1);                             \
	dr4 = vis_fpadd16(db, dr2);                             \
	dr = vis_fpack16_pair(dr3, dr4)

/* *********************************************************** */

#define	SCALE_U8_OVER                                           \
	dr = vis_fpmerge(dzero, vis_read_hi(dx));               \
	dr1 = vis_fpmerge(dzero, vis_read_lo(dx));              \
	mask_for_0 = vis_fcmpeq16(dr1, dzero);                  \
	mask_for_0 |= vis_fcmpeq16(dr, dzero) << 4;             \
	mask_for_1 = vis_fcmpeq16(dr1, done);                   \
	mask_for_1 |= vis_fcmpeq16(dr, done) << 4

/* *********************************************************** */

mlib_status
__mlib_VectorScale_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *a,
	const mlib_u8 *b,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s8 *pzend;
	mlib_u16 ua = ((mlib_u8 *)a)[0], ub = ((mlib_u8 *)b)[0];
	mlib_d64 *dpx, *dpz, *dpzend;
	mlib_d64 dx, dx0, dx1, dr, dr1, dr2, dr3, dr4;

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
 * prepare the source address
 */
	dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);

/*
 * generate edge mask for the start bytes
 */
	emask = vis_edge8(z, pzend);
	dx = vis_faligndata(dx0, dx1);

/* a >= 128 */
	if (ua & MLIB_S8_MIN) {
		mlib_s32 mask_for_0, mask_for_1;
		mlib_d64 rez_for_0;
		mlib_d64 rez_for_1;
		mlib_d64 dzero = vis_fzero();
		mlib_d64 done = vis_to_double_dup(0x10001);
		mlib_d64 dmax_u8 = vis_fone();

		ua += ub;

		if (ua > MLIB_U8_MAX)
			ua = MLIB_U8_MAX;
		ua |= ua << 8;
		rez_for_1 = vis_to_double_dup((ua << 16) | ua);
		ub |= ub << 8;
		rez_for_0 = vis_to_double_dup((ub << 16) | ub);

		vis_pst_8(dmax_u8, dpz, emask);
		SCALE_U8_OVER;
		vis_pst_8(rez_for_0, dpz, mask_for_0 & emask);
		vis_pst_8(rez_for_1, dpz, mask_for_1 & emask);

		if ((mlib_addr)dpx - (mlib_addr)x == off) {
/*
 * aligned
 */
#pragma pipeloop(0)
			for (dpx++, dpz++; dpz < dpzend; dpx++, dpz++) {
				dx = *dpx;
				*dpz = dmax_u8;
				SCALE_U8_OVER;
				vis_pst_8(rez_for_0, dpz, mask_for_0);
				vis_pst_8(rez_for_1, dpz, mask_for_1);
			}

			dx = vis_ld_d64_nf(dpx);
		} else {
/*
 * not aligned
 */
#pragma pipeloop(0)
			for (dpx++, dpz++; dpz < dpzend; dpx++, dpz++) {
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dpx + 1);
				dx = vis_faligndata(dx0, dx1);
				*dpz = dmax_u8;
				SCALE_U8_OVER;
				vis_pst_8(rez_for_0, dpz, mask_for_0);
				vis_pst_8(rez_for_1, dpz, mask_for_1);
			}

			dx = vis_faligndata(dx1, vis_ld_d64_nf(dpx + 1));
		}

		if ((mlib_s8 *)dpz <= pzend) {
			emask = vis_edge8(dpz, pzend);
			vis_pst_8(dmax_u8, dpz, emask);
			SCALE_U8_OVER;
			vis_pst_8(rez_for_0, dpz, mask_for_0 & emask);
			vis_pst_8(rez_for_1, dpz, mask_for_1 & emask);
		}

	} else {
		mlib_d64 db = vis_to_double_dup(ub | (ub << 16));

/* prepare the scaling factor */
		mlib_f32 fa = vis_to_float(ua << 8);

		SCALE_U8_SAT_LO_1;
		SCALE_U8_SAT_LO_2;
/* store first bytes of result */
		vis_pst_8(dr, dpz, emask);

		if ((mlib_addr)dpx - (mlib_addr)x == off) {
/*
 * aligned
 */
#pragma pipeloop(0)
			for (dpx++, dpz++; dpz < dpzend; dpx++, dpz++) {
				dx = *dpx;
				SCALE_U8_SAT_LO_1;
				SCALE_U8_SAT_LO_2;
				*dpz = dr;
			}

			dx = vis_ld_d64_nf(dpx);
		} else {
/*
 * not aligned
 */
#pragma pipeloop(0)
			for (dpx++, dpz++; dpz < dpzend; dpx++, dpz++) {
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dpx + 1);
				dx = vis_faligndata(dx0, dx1);
				SCALE_U8_SAT_LO_1;
				SCALE_U8_SAT_LO_2;
				*dpz = dr;
			}

			dx = vis_faligndata(dx1, vis_ld_d64_nf(dpx + 1));
		}

		if ((mlib_s8 *)dpz <= pzend) {
			emask = vis_edge8(dpz, pzend);
			SCALE_U8_SAT_LO_1;
			SCALE_U8_SAT_LO_2;
			vis_pst_8(dr, dpz, emask);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S16_U8_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *a,
	const mlib_u8 *b,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpzend;
	mlib_d64 dx, dx0, dx1, dr, dr1, dr2, dr3, dr4, dr5;
	mlib_d64 dzh, dzl;
	mlib_s32 off;
	mlib_u8 *px;
	mlib_s16 *pz, *pzend;
	mlib_u16 ua = ((mlib_u8 *)a)[0], ub = ((mlib_u8 *)b)[0];
	mlib_d64 db = vis_to_double_dup(ub | (ub << 16));

/*
 * prepare the scaling factor
 */
	mlib_f32 fa;
	mlib_f32 f127 = vis_to_float(0x7f00);
	mlib_d64 control_ovl =
		vis_to_double_dup((MLIB_S16_MAX << 16) | MLIB_S16_MAX);
	mlib_d64 dzero = vis_to_double_dup(0);
	mlib_s32 mask, mask1;
	mlib_s32 emask, emask1;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_u8 *)x;
	pz = (mlib_s16 *)z;

	pzend = (mlib_s16 *)((mlib_addr)(pz + n) - 1);
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));

	dpz = (mlib_d64 *)((mlib_addr)pz & (~7));
	dpzend = (mlib_d64 *)((mlib_addr)dpzend -
		(((mlib_addr)dpzend ^ (mlib_addr)dpz) & 8));

	off = -(((mlib_addr)pz - (mlib_addr)dpz) >> 1);

	dpx = (mlib_d64 *)vis_alignaddr(px, off);

	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);

	emask = vis_edge16(pz, pzend);
	emask1 = vis_edge16(dpz + 1, pzend);

	dx = vis_faligndata(dx0, dx1);

	if (ua & 0x80) {
/*
 * ua >=128 (possible saturation)*
 */

		fa = vis_to_float((ua ^ 0x7f) << 8);
		SCALE_S16_U8_SAT;
		vis_pst_16(dzh, dpz, emask);
		vis_pst_16(control_ovl, dpz, mask & emask);
		if (dpz + 1 <= (mlib_d64 *)pzend) {
			vis_pst_16(dzl, dpz + 1, emask1);
			vis_pst_16(control_ovl, dpz + 1, mask1 & emask1);
		}

		if ((mlib_addr)dpx - (mlib_addr)px == off) {
/*
 * aligned case
 */
#pragma pipeloop(0)
			for (dpx++, dpz += 2; dpz < dpzend; dpx++, dpz += 2) {
				dx = *dpx;
				SCALE_S16_U8_SAT;
				dpz[0] = dzh;
				dpz[1] = dzl;
				vis_pst_16(control_ovl, dpz, mask);
				vis_pst_16(control_ovl, dpz + 1, mask1);
			}

			dx = vis_ld_d64_nf(dpx);
		} else {
/*
 * not aligned case
 */
#pragma pipeloop(0)
			for (dpx++, dpz += 2; dpz < dpzend; dpx++, dpz += 2) {
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dpx + 1);
				dx = vis_faligndata(dx0, dx1);
				SCALE_S16_U8_SAT;
				dpz[0] = dzh;
				dpz[1] = dzl;
				vis_pst_16(control_ovl, dpz, mask);
				vis_pst_16(control_ovl, dpz + 1, mask1);
			}

			dx = vis_faligndata(dx1, vis_ld_d64_nf(dpx + 1));
		}

		if ((mlib_s16 *)dpz <= pzend) {
			emask = vis_edge16(dpz, pzend);
			emask1 = vis_edge16(dpz + 1, pzend);

			SCALE_S16_U8_SAT;
			vis_pst_16(dzh, dpz, emask);
			vis_pst_16(control_ovl, dpz, mask & emask);
			if (dpz + 1 <= (mlib_d64 *)pzend) {
				vis_pst_16(dzl, dpz + 1, emask1);
				vis_pst_16(control_ovl, dpz + 1,
					mask1 & emask1);
			}
		}

	} else {
/*
 * ua < 128 (no saturation)*
 */
		fa = vis_to_float(ua << 8);
		SCALE_S16_U8_MOD_SHORT;
		vis_pst_16(dzh, dpz, emask);

		if (dpz + 1 <= (mlib_d64 *)pzend) {
			vis_pst_16(dzl, dpz + 1, emask1);
		}

		if ((mlib_addr)dpx - (mlib_addr)px == off) {
#pragma pipeloop(0)
			for (dpx++, dpz += 2; dpz < dpzend; dpx++, dpz += 2) {
				dx = *dpx;
				SCALE_S16_U8_MOD_SHORT;
				dpz[0] = dzh;
				dpz[1] = dzl;
			}

			dx = vis_ld_d64_nf(dpx);
		} else {
#pragma pipeloop(0)
			for (dpx++, dpz += 2; dpz < dpzend; dpx++, dpz += 2) {
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dpx + 1);
				dx = vis_faligndata(dx0, dx1);
				SCALE_S16_U8_MOD_SHORT;
				dpz[0] = dzh;
				dpz[1] = dzl;
			}

			dx = vis_faligndata(dx1, vis_ld_d64_nf(dpx + 1));
		}

		if ((mlib_s16 *)dpz <= pzend) {
			emask = vis_edge16(dpz, pzend);
			emask1 = vis_edge16(dpz + 1, pzend);

			SCALE_S16_U8_MOD_SHORT;
			vis_pst_16(dzh, dpz, emask);
			if (dpz + 1 <= (mlib_d64 *)pzend) {
				vis_pst_16(dzl, dpz + 1, emask1);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S16_U8_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *a,
	const mlib_u8 *b,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx;
	mlib_d64 dx, dx0, dx1, dr, dr1, dr2, dr3, dr4, dr5;
	mlib_d64 dzh, dzl;
	mlib_u8 *px;
	mlib_s16 *pz;
	mlib_u16 ua = ((mlib_u8 *)a)[0], ub = ((mlib_u8 *)b)[0];
	mlib_d64 db = vis_to_double_dup(ub | (ub << 16));

/*
 * prepare the scaling factor
 */
	mlib_f32 fa;
	mlib_s32 len = n, i;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_f32 f127 = vis_to_float(0x7f00);

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_u8 *)x;
	pz = (mlib_s16 *)z;

	if (n <= 8)
		SCALE_S32_S16_MOD_IN_C;

/*
 * prepare the destination address
 */

	while ((mlib_addr)pz & 7) {
		(*pz++) = ((mlib_s32)(*px)) * ua + ub;
		px++;
		len--;
	}

	dpz = (mlib_d64 *)pz;

	even_8 = len >> 3;
	rest_8 = len & 0x7;

	if (!((mlib_addr)px & 7)) {

/*
 * Source address is 8-byte aligned.
 * No  vis_alignaddr and  vis_faligndata at all.
 */

		dpx = (mlib_d64 *)px;

		if (ua & 0x80) {
/*
 * ua > 128
 */
			fa = vis_to_float((ua ^ 0x7f) << 8);

#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = (*dpx++);
				SCALE_S16_U8_MOD;
/*
 * store 16 bytes of result
 */
				(*dpz++) = dzh;
				(*dpz++) = dzl;
			}

		} else {
/*
 * ua < 128 -> Very mlib_s16 case.
 */
			fa = vis_to_float((ua) << 8);

#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = (*dpx++);
				SCALE_S16_U8_MOD_SHORT;
/*
 * store 16 bytes of result
 */
				(*dpz++) = dzh;
				(*dpz++) = dzl;
			}
		}

	} else {
/*
 * "x" address is arbitrary aligned.
 */
		dpx = vis_alignaddr(px, 0);
		dx0 = (*dpx++);

		if (ua & 0x80) {
/*
 * ua > 128
 */
			fa = vis_to_float((ua ^ 0x7f) << 8);

#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx); dpx++;
				dx = vis_faligndata(dx0, dx1);
				SCALE_S16_U8_MOD;
				dx0 = dx1;
/*
 * store 16 bytes of result
 */
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
			}

		} else {
/*
 * ua < 128 -> Very mlib_s16 case.
 */
			fa = vis_to_float((ua) << 8);

#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx); dpx++;
				dx = vis_faligndata(dx0, dx1);
				SCALE_S16_U8_MOD_SHORT;
				dx0 = dx1;
/*
 * store 16 bytes of result
 */
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
			}
		}
	}

	px += (even_8 << 3);
	pz += (even_8 << 3);

	while (rest_8--) {
		(*pz++) = ((mlib_s32)(*px)) * ua + ub;
		px++;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SCALE_S8_SAT_1                                          \
	dr1 = vis_fpmerge(vis_read_hi(dx), vis_read_hi(dx));    \
	dr3 = vis_fmul8sux16(dr1, da);                          \
	dr2 = vis_fpmerge(vis_read_lo(dx), vis_read_lo(dx));    \
	dr4 = vis_fmul8sux16(dr2, da)

/* *********************************************************** */

#define	SCALE_S8_SAT_2                                          \
	dr5 = vis_fpadd16(db, dr3);                             \
	dr6 = vis_fpadd16(db, dr4);                             \
	dr = vis_fpack16_pair(dr5, dr6);                        \
	dr = vis_fxor(dr, restore)

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *a,
	const mlib_s8 *b,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s8 *pzend;
	mlib_u16 ua = ((mlib_s8 *)a)[0] << 8, ub = ((mlib_s8 *)b)[0] + 128;
	mlib_d64 *dpx, *dpz, *dpzend;
	mlib_d64 dx, dx0, dx1, dr, dr1, dr2, dr3, dr4, dr5, dr6;

	mlib_d64 restore = vis_to_double_dup(0x80808080);
	mlib_d64 db = vis_to_double_dup(ub | (ub << 16));

/*
 * prepare the scaling factor
 */
	mlib_d64 da = vis_to_double_dup((ua << 16) | ua);

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
 * prepare the source address
 */
	dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);

/*
 * generate edge mask for the start bytes
 */
	emask = vis_edge8(z, pzend);
	dx = vis_faligndata(dx0, dx1);
	SCALE_S8_SAT_1;
	SCALE_S8_SAT_2;
/* store first bytes of result */
	vis_pst_8(dr, dpz, emask);

	if ((mlib_addr)dpx - (mlib_addr)x == off) {
/*
 * aligned case
 */
#pragma pipeloop(0)
		for (dpx++, dpz++; dpz < dpzend; dpx++, dpz++) {
			dx = *dpx;
			SCALE_S8_SAT_1;
			SCALE_S8_SAT_2;
			dpz[0] = dr;
		}

		dx = vis_ld_d64_nf(dpx);
	} else {
/*
 * not aligned case
 */
#pragma pipeloop(0)
		for (dpx++, dpz++; dpz < dpzend; dpx++, dpz++) {
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx + 1);
			dx = vis_faligndata(dx0, dx1);
			SCALE_S8_SAT_1;
			SCALE_S8_SAT_2;
			dpz[0] = dr;
		}

		dx = vis_faligndata(dx1, vis_ld_d64_nf(dpx + 1));
	}

	if ((mlib_s8 *)dpz <= pzend) {
		emask = vis_edge8(dpz, pzend);
		SCALE_S8_SAT_1;
		SCALE_S8_SAT_2;
		vis_pst_8(dr, dpz, emask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SCALE_S8_MOD_1                                          \
	dr1 = vis_fmul8x16al(vis_read_hi(dx), fa);              \
	dr2 = vis_fmul8x16al(vis_read_lo(dx), fa);              \
	dr3 = vis_fpadd16(dr1, db);                             \
	dr4 = vis_fpadd16(dr2, db)

/* *********************************************************** */

#define	SCALE_S8_MOD_2                                          \
	dr3 = vis_fand(dr3, without_control);                   \
	dr4 = vis_fand(dr4, without_control);                   \
	dr = vis_fpack16_pair(dr3, dr4)

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S8_S8_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *a,
	const mlib_s8 *b,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s8 *pzend;
	mlib_u16 ua = ((mlib_s8 *)a)[0] << 8, ub = ((mlib_s8 *)b)[0];
	mlib_d64 *dpx, *dpz, *dpzend;
	mlib_d64 dx, dx0, dx1, dr, dr1, dr2, dr3, dr4;

	mlib_d64 db = vis_to_double_dup(ub | (ub << 16));

/* prepare the scaling factor */
	mlib_f32 fa = vis_to_float(ua);

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
 * prepare the source address
 */
	dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);

/*
 * generate edge mask for the start bytes
 */
	emask = vis_edge8(z, pzend);
	dx = vis_faligndata(dx0, dx1);
	vis_write_bmask(0x13579bdf, 0);

	SCALE_S8_MOD_1;
	dr = vis_bshuffle(dr3, dr4);
/* store first bytes of result */
	vis_pst_8(dr, dpz, emask);

	if ((mlib_addr)dpx - (mlib_addr)x == off) {
/*
 * aligned case
 */
#pragma pipeloop(0)
		for (dpx++, dpz++; dpz < dpzend; dpx++, dpz++) {
			dx = *dpx;
			SCALE_S8_MOD_1;
			dpz[0] = vis_bshuffle(dr3, dr4);
		}

		dx = vis_ld_d64_nf(dpx);
	} else {
/*
 * not aligned case
 */
#pragma pipeloop(0)
		for (dpx++, dpz++; dpz < dpzend; dpx++, dpz++) {
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx + 1);
			dx = vis_faligndata(dx0, dx1);
			SCALE_S8_MOD_1;
			dpz[0] = vis_bshuffle(dr3, dr4);
		}

		dx = vis_faligndata(dx1, vis_ld_d64_nf(dpx + 1));
	}

	if ((mlib_s8 *)dpz <= pzend) {
		emask = vis_edge8(dpz, pzend);
		SCALE_S8_MOD_1;
		dr = vis_bshuffle(dr3, dr4);
		vis_pst_8(dr, dpz, emask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *a,
	const mlib_u8 *b,
	mlib_s32 n)
{
	return __mlib_VectorScale_S8_S8_Mod((mlib_s8 *)z, (mlib_s8 *)x,
		(mlib_s8 *)a, (mlib_s8 *)b, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S8_Mod(
	mlib_s8 *xz,
	const mlib_s8 *a,
	const mlib_s8 *b,
	mlib_s32 n)
{
	return (__mlib_VectorScale_S8_S8_Mod(xz, xz, a, b, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S16_S8_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *a,
	const mlib_s8 *b,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpzend;
	mlib_s8 *px;
	mlib_s16 *pz, *pzend;
	mlib_s32 off, emask, emask1;
	mlib_d64 dx, dx0, dx1, dr, dr1, dr2, dr3;
	mlib_d64 dzh, dzl;
	mlib_s8 ua = ((mlib_s8 *)a)[0], ub = ((mlib_s8 *)b)[0];

/*
 * prepare the scaling factors 'a', 'b'
 */
	mlib_d64 db = vis_to_double_dup((ub & 0xffff) | (ub << 16));
	mlib_d64 da = vis_to_double_dup(((ua << 8) & 0xffff) | (ua << 24));
	mlib_f32 fzero = vis_fzeros();

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s8 *)x;
	pz = (mlib_s16 *)z;

	pzend = (mlib_s16 *)((mlib_addr)(pz + n) - 1);
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));

	dpz = (mlib_d64 *)((mlib_addr)pz & (~7));
	dpzend = (mlib_d64 *)((mlib_addr)dpzend -
		(((mlib_addr)dpzend ^ (mlib_addr)dpz) & 8));

	off = -(((mlib_addr)pz - (mlib_addr)dpz) >> 1);

	dpx = (mlib_d64 *)vis_alignaddr(px, off);

	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);

	emask = vis_edge16(pz, pzend);
	emask1 = vis_edge16(dpz + 1, pzend);

	dx = vis_faligndata(dx0, dx1);
	SCALE_S16_S8_MOD;
	vis_pst_16(dzh, dpz, emask);
	if (dpz + 1 <= (mlib_d64 *)pzend) {
		vis_pst_16(dzl, dpz + 1, emask1);
	}

	if ((mlib_addr)dpx - (mlib_addr)x == off) {
/*
 * aligned
 */
#pragma pipeloop(0)
		for (dpx++, dpz += 2; dpz < dpzend; dpx++, dpz += 2) {
			dx = *dpx;
			SCALE_S16_S8_MOD;
			dpz[0] = dzh;
			dpz[1] = dzl;
		}

		dx = vis_ld_d64_nf(dpx);
	} else {
/*
 * not aligned
 */
#pragma pipeloop(0)
		for (dpx++, dpz += 2; dpz < dpzend; dpx++, dpz += 2) {
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx + 1);
			dx = vis_faligndata(dx0, dx1);
			SCALE_S16_S8_MOD;
			dpz[0] = dzh;
			dpz[1] = dzl;
		}

		dx = vis_faligndata(dx1, vis_ld_d64_nf(dpx + 1));
	}

	if ((mlib_s16 *)dpz <= pzend) {
		emask = vis_edge16(dpz, pzend);
		emask1 = vis_edge16(dpz + 1, pzend);

		SCALE_S16_S8_MOD;
		vis_pst_16(dzh, dpz, emask);
		if (dpz + 1 <= (mlib_d64 *)pzend) {
			vis_pst_16(dzl, dpz + 1, emask1);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *a,
	const mlib_s16 *b,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s16 sa = ((mlib_s16 *)a)[0];
	mlib_s16 *pzend = (mlib_s16 *)z + n - 1;
	mlib_d64 *dpx, *dpz, *dpzend;
	mlib_d64 dx, dx0, dx1, dr, dr1, dr2, dr3, dr4;

	if (n <= 0)
		return (MLIB_FAILURE);

/* initialize GSR scale factor */
	vis_write_gsr(15 << 3);

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

/*
 * generate edge mask for the start point
 */
	emask = vis_edge16(z, pzend);

/* a != -32768 */
	if (sa != MLIB_S16_MIN) {
		if (sa <= (MLIB_S16_MAX / 2) && sa >= (MLIB_S16_MIN / 2)) {
			mlib_d64 db = vis_to_double_dup(((mlib_s16 *)b)[0] * 2);
			mlib_f32 fa =
				vis_to_float((sa << 17) | ((sa << 1) & 0xffff));

			SCALE_S16;
			PACK_S32_S16_SAT_LO;
/* store first bytes of result */
			vis_pst_16(dr2, dpz, emask);

			if ((mlib_addr)dpx - (mlib_addr)x == off) {
/*
 * aligned
 */
#pragma pipeloop(0)
				for (dpx++, dpz++; dpz < dpzend; dpx++, dpz++) {
					dx = *dpx;
					SCALE_S16;
					PACK_S32_S16_SAT_LO;
					dpz[0] = dr2;
				}

				dx = vis_ld_d64_nf(dpx);
			} else {
/*
 * not aligned
 */
#pragma pipeloop(0)
				for (dpx++, dpz++; dpz < dpzend; dpx++, dpz++) {
					dx0 = dx1;
					dx1 = vis_ld_d64_nf(dpx + 1);
					dx = vis_faligndata(dx0, dx1);
					SCALE_S16;
					PACK_S32_S16_SAT_LO;
					dpz[0] = dr2;
				}

				dx = vis_faligndata(dx1,
					vis_ld_d64_nf(dpx + 1));
			}

			SCALE_S16;
			PACK_S32_S16_SAT_LO;
		} else {
			mlib_d64 db = vis_to_double_dup(((mlib_s16 *)b)[0]);
			mlib_f32 fa = vis_to_float((sa << 16) | (sa & 0xffff));

			SCALE_S16;
			PACK_S32_S16_SAT;
/* store first bytes of result */
			vis_pst_16(dr2, dpz, emask);

			if ((mlib_addr)dpz - (mlib_addr)x == off) {
/*
 * aligned
 */
#pragma pipeloop(0)
				for (dpx++, dpz++; dpz < dpzend; dpx++, dpz++) {
					dx = *dpx;
					SCALE_S16;
					PACK_S32_S16_SAT;
					*dpz = dr2;
				}

				dx = vis_ld_d64_nf(dpx);
			} else {
/*
 * not aligned
 */
#pragma pipeloop(0)
				for (dpx++, dpz++; dpz < dpzend; dpx++, dpz++) {
					dx0 = dx1;
					dx1 = vis_ld_d64_nf(dpx + 1);
					dx = vis_faligndata(dx0, dx1);
					SCALE_S16;
					PACK_S32_S16_SAT;
					*dpz = dr2;
				}

				dx = vis_faligndata(dx1,
					vis_ld_d64_nf(dpx + 1));
			}

			SCALE_S16;
			PACK_S32_S16_SAT;
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
/* prepare edge mask for the last bytes */
			emask = vis_edge16(dpz, pzend);
/* store last bytes of result */
			vis_pst_16(dr2, dpz, emask);
		}

	} else {
/* a == MLIB_S16_MIN */
		mlib_s32 mask_over;
		mlib_d64 db = vis_to_double_dup(((mlib_s16 *)b)[0]);
		mlib_d64 dmax = vis_to_double_dup(MLIB_S32_MAX / 2);
		mlib_d64 d_over = vis_to_double_dup(0x7fff7fff);
		mlib_f32 fa = vis_fnots(vis_read_hi(d_over));

		SCALE_S16;
		dx0 = dx1;
		dx1 = vis_ld_d64_nf(dpx + 2);
		dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
		for (; (mlib_addr)dpz <= (mlib_addr)dpzend; ) {
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx + 3);
			PACK_S32_S16_SAT;
			mask_over =
				(vis_fcmpgt32(dr,
				dmax) << 2) | vis_fcmpgt32(dr1, dmax);
			SCALE_S16;
			vis_pst_16(dr2, dpz, emask);
			vis_pst_16(d_over, dpz, mask_over & emask);
			dpz++;
/*
 * prepare edge mask for the end point
 */
			emask = vis_edge16(dpz, pzend);
			dx = vis_faligndata(dx0, dx1);
			dpx++;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *a,
	const mlib_s16 *b,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_u16 ua = ((mlib_u16 *)a)[0];
	mlib_s16 *pzend;
	mlib_d64 *dpx, *dpz, *dpzend;
	mlib_d64 dx, dx0, dx1, dr, dr1, dr2;
	mlib_d64 db = vis_to_double_dup(((mlib_s16 *)b)[0]);
	mlib_f32 fa = vis_to_float((ua << 16) | ua);

	if (n <= 0)
		return (MLIB_FAILURE);

/*
 * prepare the destination address
 */
	pzend = (mlib_s16 *)z + n - 1;
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z;
	vis_write_bmask(0x2367abef, 0);

/*
 * prepare the source address
 */
	dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_faligndata(dx0, dx1);

/*
 * generate edge mask for the start point
 */
	emask = vis_edge16(z, pzend);

	SCALE_S16_MOD;
	dr2 = vis_bshuffle(dr, dr1);
/* store first bytes of result */
	vis_pst_16(dr2, dpz, emask);
	dpz++;

	if ((mlib_addr)dpx - (mlib_addr)x != off) {
/*
 * not aligned
 */
		dx0 = dx1;
		dx1 = vis_ld_d64_nf(dpx + 2);
		dx = vis_faligndata(dx0, dx1);
		SCALE_S16_MOD;
		dx0 = dx1;
		dx1 = vis_ld_d64_nf(dpx + 3);
		dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
		for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
			dr2 = vis_bshuffle(dr, dr1);
			SCALE_S16_MOD;
			dpz[0] = dr2;
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx + 4); dpx++;
			dx = vis_faligndata(dx0, dx1);
			dpz++;
		}

	} else {
/*
 * aligned
 */
		dx0 = dx1;
		dx1 = vis_ld_d64_nf(dpx + 2);
		dx = vis_faligndata(dx0, dx1);
		SCALE_S16_MOD;
		dx0 = dx1;
		dx1 = vis_ld_d64_nf(dpx + 3);
		dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
		for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
			dr2 = vis_bshuffle(dr, dr1);
			SCALE_S16_MOD;
			dpz[0] = dr2;
			dx = vis_ld_d64_nf(dpx + 3); dpx++;
			dpz++;
		}
	}

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
		dr2 = vis_bshuffle(dr, dr1);
/* prepare edge mask for the last bytes */
		emask = vis_edge16(dpz, pzend);
/* store last bytes of result */
		vis_pst_16(dr2, dpz, emask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S32_S16_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *a,
	const mlib_s16 *b,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpzend;
	mlib_s32 off, emask, emask1;
	mlib_s32 *pzend;
	mlib_d64 dx, dx0, dx1, dr, dr1;
	mlib_d64 dzh, dzl;
	mlib_s16 *px;
	mlib_s32 *pz;
	mlib_s16 ua = ((mlib_s16 *)a)[0], ub = ((mlib_s16 *)b)[0];

/*
 * prepare the scaling factors 'a', 'b'
 */
	mlib_d64 db = vis_to_double_dup(ub);
	mlib_f32 fa = vis_to_float((ua << 16) | (ua & 0xffff));

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s16 *)x;
	pz = (mlib_s32 *)z;

	pzend = (mlib_s32 *)((mlib_addr)(pz + n) - 1);
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpz = (mlib_d64 *)((mlib_addr)pz & (~7));

	dpzend = (mlib_d64 *)((mlib_addr)dpzend -
		(((mlib_addr)dpzend ^ (mlib_addr)dpz) & 8));

	off = -(((mlib_addr)pz - (mlib_addr)dpz) >> 1);

	dpx = (mlib_d64 *)vis_alignaddr(px, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);

	emask = vis_edge32(pz, pzend);
	emask1 = vis_edge32(dpz + 1, pzend);

	dx = vis_faligndata(dx0, dx1);
	SCALE_S32_S16_MOD;
	vis_pst_32(dzh, dpz, emask);
	if (dpz + 1 <= (mlib_d64 *)pzend) {
		vis_pst_32(dzl, dpz + 1, emask1);
	}

	if ((mlib_addr)dpx - (mlib_addr)px == off) {
/*
 * aligned
 */
#pragma pipeloop(0)
		for (dpx++, dpz += 2; dpz < dpzend; dpx++, dpz += 2) {
			dx = *dpx;
			SCALE_S32_S16_MOD;
			dpz[0] = dzh;
			dpz[1] = dzl;
		}

		dx = vis_ld_d64_nf(dpx);
	} else {
/*
 * not aligned
 */
#pragma pipeloop(0)
		for (dpx++, dpz += 2; dpz < dpzend; dpx++, dpz += 2) {
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx + 1);
			dx = vis_faligndata(dx0, dx1);
			SCALE_S32_S16_MOD;
			dpz[0] = dzh;
			dpz[1] = dzl;
		}

		dx = vis_faligndata(dx1, vis_ld_d64_nf(dpx + 1));
	}

	if ((mlib_s32 *)dpz <= pzend) {
		emask = vis_edge32(dpz, pzend);
		emask1 = vis_edge32(dpz + 1, pzend);

		SCALE_S32_S16_MOD;
		vis_pst_32(dzh, dpz, emask);
		if (dpz + 1 <= (mlib_d64 *)pzend) {
			vis_pst_32(dzl, dpz + 1, emask1);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S32_S32_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *a,
	const mlib_s32 *b,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 val_a = *((mlib_s32 *)a), val_b = *((mlib_s32 *)b);

#pragma pipeloop(0)
	for (i = 0; i < n; i++)
		FLOAT2INT_CLAMP(((mlib_s32 *)z)[i],
			val_a * ((mlib_s32 *)x)[i] + val_b);
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S32_S32_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *a,
	const mlib_s32 *b,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_s32 *pz = z;

	mlib_d64 d16 = 0x10000, _d16 = 1 / d16;
	mlib_d64 d32 = d16 * d16;
	mlib_d64 scalh, scall;
	mlib_d64 dah = ((mlib_s16 *)a)[0], dx;
	mlib_d64 dal = ((mlib_u16 *)a)[1];
	mlib_d64 db = *b;

	db += (mlib_d64)(1 << 26) * (mlib_d64)(3 << 25);

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		union
		{
			mlib_d64 d;
			struct
			{
				mlib_s32 s0, s1;
			} s;
		} res;

		dx = x[i];
		scalh = dah * dx;
		scall = dal * dx;

		scalh = scalh * d16 - (mlib_s32)(scalh * _d16) * d32;
		res.d = scalh + scall + db;
		pz[i] = res.s.s1;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_U8_Sat(
	mlib_u8 *xz,
	const mlib_u8 *a,
	const mlib_u8 *b,
	mlib_s32 n)
{
	return (__mlib_VectorScale_U8_U8_Sat(xz, xz, a, b, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_U8_Mod(
	mlib_u8 *xz,
	const mlib_u8 *a,
	const mlib_u8 *b,
	mlib_s32 n)
{
	return (__mlib_VectorScale_U8_U8_Mod(xz, xz, a, b, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S8_Sat(
	mlib_s8 *xz,
	const mlib_s8 *a,
	const mlib_s8 *b,
	mlib_s32 n)
{
	return (__mlib_VectorScale_S8_S8_Sat(xz, xz, a, b, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S16_Sat(
	mlib_s16 *xz,
	const mlib_s16 *a,
	const mlib_s16 *b,
	mlib_s32 n)
{
	return (__mlib_VectorScale_S16_S16_Sat(xz, xz, a, b, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S16_Mod(
	mlib_s16 *xz,
	const mlib_s16 *a,
	const mlib_s16 *b,
	mlib_s32 n)
{
	return (__mlib_VectorScale_S16_S16_Mod(xz, xz, a, b, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S32_Sat(
	mlib_s32 *xz,
	const mlib_s32 *a,
	const mlib_s32 *b,
	mlib_s32 n)
{
	return (__mlib_VectorScale_S32_S32_Sat(xz, xz, a, b, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S32_Mod(
	mlib_s32 *xz,
	const mlib_s32 *a,
	const mlib_s32 *b,
	mlib_s32 n)
{
	return (__mlib_VectorScale_S32_S32_Mod(xz, xz, a, b, n));
}

/* *********************************************************** */

#define	SCALE_U8C_SAT_1                                         \
	{                                                       \
	    mlib_d64 dr7, dr8, dr9;                             \
	                                                        \
	    dr9 = vis_faligndata(dx, dx);                       \
	    dr5 = vis_fmuld8ulx16(vis_read_hi(dx), fa_r);       \
	    dr6 = vis_fmuld8ulx16(vis_read_lo(dx), fa_r);       \
	    dr2 = vis_fmuld8ulx16(vis_read_lo(dr9), fa_r);      \
	    dr1 = vis_fmuld8ulx16(vis_read_hi(dr9), fa_r);      \
	    dr7 = vis_fmuld8ulx16(vis_read_hi(dx), fa_i);       \
	    dr8 = vis_fmuld8ulx16(vis_read_lo(dx), fa_i);       \
	    dr4 = vis_fmuld8ulx16(vis_read_lo(dr9), fa_i);      \
	    dr3 = vis_fmuld8ulx16(vis_read_hi(dr9), fa_i);      \
	    dr1 = vis_fpsub32(dr1, dr7);                        \
	    dr2 = vis_fpsub32(dr2, dr8);                        \
	}

/* *********************************************************** */

#define	SCALE_U8C_SAT_2                                         \
	{                                                       \
	    mlib_d64 dr7, dr8, dr9, dr10;                       \
	                                                        \
	    dr7 = vis_fpadd32(dr1, db_r);                       \
	    dr8 = vis_fpadd32(dr2, db_r);                       \
	    dr9 = vis_fpadd32(dr3, dr5);                        \
	    dr10 = vis_fpadd32(dr4, dr6);                       \
	    dr9 = vis_fpadd32(dr9, db_i);                       \
	    dr10 = vis_fpadd32(dr10, db_i);                     \
	    dr = vis_fpack32(dr, dr7);                          \
	    dr = vis_fpack32(dr, dr9);                          \
	    dr = vis_fpack32(dr, dr8);                          \
	    dr = vis_fpack32(dr, dr10);                         \
	    vis_write_bmask(0x01452367, 0);                     \
	    dr = vis_bshuffle(dr, dr);                          \
	}

/* *********************************************************** */

#define	SCALE_U8C_SAT_LO_1                                       \
	dr5 = vis_fpmerge(vis_read_hi(dx), vis_read_lo(dx));     \
	dr5 = vis_fpmerge(vis_read_hi(dr5), vis_read_lo(dr5));   \
	dr3 = vis_fmul8x16au(vis_read_hi(dr5), fa);              \
	dr4 = vis_fmul8x16al(vis_read_hi(dr5), fa);              \
	dr6 = vis_fmul8x16au(vis_read_lo(dr5), fa);              \
	dr5 = vis_fmul8x16al(vis_read_lo(dr5), fa);              \
	dr3 = vis_fpsub16(dr3, dr5);                             \
	dr4 = vis_fpadd16(dr4, dr6)

/* *********************************************************** */

#define	SCALE_U8C_SAT_LO_2                                      \
	dr1 = vis_fpadd16(dr3, db_r);                           \
	dr2 = vis_fpadd16(dr4, db_i);                           \
	dr = vis_fpack16_pair(dr1, dr2);                        \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr))

/* *********************************************************** */

mlib_status
__mlib_VectorScale_U8C_U8C_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *a,
	const mlib_u8 *b,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s8 *pzend = (mlib_s8 *)z + n + n - 1;
	mlib_u16 ua_r = ((mlib_u8 *)a)[0];
	mlib_u16 ua_i = ((mlib_u8 *)a)[1];
	mlib_u16 ub_r = ((mlib_u8 *)b)[0];
	mlib_u16 ub_i = ((mlib_u8 *)b)[1];
	mlib_d64 *dpx, *dpz, *dpzend;
	mlib_d64 dx, dx0, dx1, dr, dr_, dr1, dr2, dr3, dr4, dr5, dr6;
	mlib_s32 boff, bmask;

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
	dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_faligndata(dx0, dx1);

	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx + 2);

	if ((ua_r + ua_i) > MLIB_S8_MAX) {
		mlib_d64 db_r = vis_to_double_dup(ub_r), db_i =
			vis_to_double_dup(ub_i);
		mlib_f32 fa_r = vis_to_float((ua_r << 16) | (ua_r));
		mlib_f32 fa_i = vis_to_float((ua_i << 16) | (ua_i));
		mlib_s32 gsr_for_pack = (23 << 3) | 7;

		boff = (mlib_addr)((mlib_u8 *)x + off) & 7;
		bmask = boff * 0x11111111 + 0x01234567;
		vis_write_gsr(gsr_for_pack);
		vis_write_bmask(0x01452367, 0);

		SCALE_U8C_SAT_1;
		SCALE_U8C_SAT_2;
		vis_write_bmask(bmask, 0);
		dx = vis_bshuffle(dx0, dx1);

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

		SCALE_U8C_SAT_1;
		dx0 = dx1;
		dx1 = vis_ld_d64_nf(dpx + 3);
		vis_write_bmask(bmask, 0);
		dx = vis_bshuffle(dx0, dx1);

		if (!(1 & (mlib_addr)z)) {
#pragma pipeloop(0)
			for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dpx + 4);
				SCALE_U8C_SAT_2;
				SCALE_U8C_SAT_1;
				vis_write_bmask(bmask, 0);
				dx = vis_bshuffle(dx0, dx1);
				dpx++;
/* store 8 bytes of result */
				dpz[0] = dr;
				dpz++;
			}

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				SCALE_U8C_SAT_2;
/* prepare edge mask for the last bytes */
				emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
				vis_pst_8(dr, dpz, emask);
			}

		} else {
#pragma pipeloop(0)
			for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dpx + 4);
				SCALE_U8C_SAT_2;
				SCALE_U8C_SAT_1;
				dr = vis_faligndata(dr, dr);
/* store 7 bytes of result */
				vis_pst_8(dr, dpz, 0x7f);
				dpz++;
/* store 1 byte of result to the next word */
				vis_pst_8(dr, dpz, 0x80);
				vis_write_bmask(bmask, 0);
				dx = vis_bshuffle(dx0, dx1);
				dpx++;
			}

			if ((mlib_addr)dpz < (mlib_addr)pzend) {
				SCALE_U8C_SAT_2;
/* prepare edge mask for the last bytes */
				emask = vis_edge8(dpz, pzend);
				dr = vis_faligndata(dr, dr);
/* store 7 bytes of result */
				vis_pst_8(dr, dpz, emask & 0x7f);
			}
		}

	} else {
		mlib_f32 fa = vis_to_float((ua_r << 24) | (ua_i << 8));
		mlib_d64 db_r = vis_to_double_dup(ub_r | (ub_r << 16));
		mlib_d64 db_i = vis_to_double_dup(ub_i | (ub_i << 16));

/* initialize GSR scale factor */
		vis_write_gsr(7 << 3);
		SCALE_U8C_SAT_LO_1;
		SCALE_U8C_SAT_LO_2;
		(void *)vis_alignaddr((void *)x, off);
		dx = vis_faligndata(dx0, dx1);

		if (!(1 & (mlib_addr)z)) {
/* store first bytes of result */
			vis_pst_8(dr, dpz, emask);
			dpz++;
		} else {
			(void *)vis_alignaddr(dpx, 7);
			dr_ = vis_faligndata(dr, dr);
/* store first 7 bytes of result */
			vis_pst_8(dr_, dpz, emask);
			dr_ = dr;
			dpz++;
		}

		if (!(1 & (mlib_addr)z)) {
			vis_write_bmask(0x02461357, 0);
#pragma pipeloop(0)
			for (; (mlib_addr)dpz < (mlib_addr)dpzend;
			    dpx++, dpz++) {
				dr5 = vis_bshuffle(dx, dr5);
				dr3 = vis_fmul8x16au(vis_read_hi(dr5), fa);
				dr4 = vis_fmul8x16al(vis_read_hi(dr5), fa);
				dr6 = vis_fmul8x16au(vis_read_lo(dr5), fa);
				dr5 = vis_fmul8x16al(vis_read_lo(dr5), fa);
				dr3 = vis_fpsub16(dr3, dr5);
				dr4 = vis_fpadd16(dr4, dr6);
				SCALE_U8C_SAT_LO_2;
/* store 8 bytes of result */
				dpz[0] = dr;
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dpx + 3);
				dx = vis_faligndata(dx0, dx1);
			}

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				dr5 = vis_bshuffle(dx, dr5);
				dr3 = vis_fmul8x16au(vis_read_hi(dr5), fa);
				dr4 = vis_fmul8x16al(vis_read_hi(dr5), fa);
				dr6 = vis_fmul8x16au(vis_read_lo(dr5), fa);
				dr5 = vis_fmul8x16al(vis_read_lo(dr5), fa);
				dr3 = vis_fpsub16(dr3, dr5);
				dr4 = vis_fpadd16(dr4, dr6);
				SCALE_U8C_SAT_LO_2;
/* prepare edge mask for the last bytes */
				emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
				vis_pst_8(dr, dpz, emask);
			}

		} else {
			(void *)vis_alignaddr((void *)x, off);
			vis_write_bmask(0x78c9daeb, 0);
#pragma pipeloop(0)
			for (; (mlib_addr)dpz < (mlib_addr)dpzend;
			    dpx++, dpz++) {
				SCALE_U8C_SAT_LO_1;
				dr1 = vis_fpadd16(dr3, db_r);
				dr2 = vis_fpadd16(dr4, db_i);
				dr = vis_fpack16_pair(dr1, dr2);
				*dpz = vis_bshuffle(dr_, dr);
				dr_ = dr;
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dpx + 3);
				dx = vis_faligndata(dx0, dx1);
			}

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				SCALE_U8C_SAT_LO_1;
				dr1 = vis_fpadd16(dr3, db_r);
				dr2 = vis_fpadd16(dr4, db_i);
				dr = vis_fpack16_pair(dr1, dr2);
/* prepare edge mask for the last bytes */
				emask = vis_edge8(dpz, pzend);
				dr = vis_bshuffle(dr_, dr);
/* store 7 bytes of result */
				vis_pst_8(dr, dpz, emask);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SCALE_S16C_U8C_SAT_QUICK                                \
	                                                        \
	/*                                                      \
	 * dx = AaBbCcDd                                        \
	 */                                                     \
	xd = vis_fpmerge(vis_read_hi(dx), vis_read_lo(dx));     \
	/*                                                      \
	 * ACacBDbd                                             \
	 */                                                     \
	xd = vis_fpmerge(vis_read_hi(xd), vis_read_lo(xd));     \
	/*                                                      \
	 * ABCDabcd                                             \
	 */                                                     \
	xd = vis_fpmrege(vis_read_lo(xd), vis_read_hi(xd));     \
	/*                                                      \
	 * aAbBcCdD                                             \
	 */                                                     \
	z0_hi = vis_fmul8x16(vis_read_hi(dx), da0);             \
	z1_hi = vis_fmul8x16(vis_read_hi(xd), da1);             \
	z0_lo = vis_fmul8x16(vis_read_lo(dx), da0);             \
	z1_lo = vis_fmul8x16(vis_read_lo(xd), da1);             \
	z_hi = vis_fpadd16(z0_hi, z1_hi);                       \
	z_lo = vis_fpadd16(z0_lo, z1_lo);                       \
	z_hi = vis_fpadd16(z_hi, db);                           \
	z_lo = vis_fpadd16(z_lo, db)

/* *********************************************************** */

#define	SCALE_S16C_U8C_SAT_SLOW                                 \
	x_f2 = vis_fpmerge(vis_read_hi(dx),                     \
	/*                                                      \
	 * ACacBDbd                                             \
	 */                                                     \
		vis_read_lo(dx));                               \
	x_f1 = vis_fpmerge(vis_read_lo(dx),                     \
	/*                                                      \
	 * CAcaDBdb                                             \
	 */                                                     \
		vis_read_hi(dx));                               \
	x_r2 = vis_fpmerge(vis_read_hi(x_f2),                   \
	/*                                                      \
	 * ABCDabcd                                             \
	 */                                                     \
		vis_read_lo(x_f2));                             \
	x_r2 = vis_fpmerge(vis_read_lo(x_r2),                   \
	/*                                                      \
	 * aAbBcCdD                                             \
	 */                                                     \
		vis_read_hi(x_r2));                             \
	x_r1 = vis_fpmerge(vis_read_lo(x_r2),                   \
	/*                                                      \
	 * caCAdbDB                                             \
	 */                                                     \
		vis_read_hi(x_r2));                             \
	x_r2 = vis_fpmerge(vis_read_hi(x_r2),                   \
	/*                                                      \
	 * acACbdBD                                             \
	 */                                                     \
		vis_read_lo(x_r2));                             \
	z_f1h = vis_fmuld8ulx16(vis_read_hi(x_f1), a_f);        \
	z_f1l = vis_fmuld8ulx16(vis_read_lo(x_f1), a_f);        \
	z_r1h = vis_fmuld8ulx16(vis_read_hi(x_r1), a_r);        \
	z_r1l = vis_fmuld8ulx16(vis_read_lo(x_r1), a_r);        \
	z_f2h = vis_fmuld8ulx16(vis_read_hi(x_f2), a_f);        \
	z_f2l = vis_fmuld8ulx16(vis_read_lo(x_f2), a_f);        \
	z_r2h = vis_fmuld8ulx16(vis_read_hi(x_r2), a_r);        \
	z_r2l = vis_fmuld8ulx16(vis_read_lo(x_r2), a_r);        \
	z_1h = vis_fpadd32(z_f1h, z_r1h);                       \
	z_1l = vis_fpadd32(z_f1l, z_r1l);                       \
	z_2h = vis_fpadd32(z_f2h, z_r2h);                       \
	z_2l = vis_fpadd32(z_f2l, z_r2l);                       \
	z_1h = vis_fpadd32(z_1h, db);                           \
	z_1l = vis_fpadd32(z_1l, db);                           \
	z_2h = vis_fpadd32(z_2h, db);                           \
	z_2l = vis_fpadd32(z_2l, db);                           \
	z_1 = vis_fpackfix_pair(z_1h, z_1l);                    \
	z_2 = vis_fpackfix_pair(z_2h, z_2l)

/* *********************************************************** */

#define	SCALE_S16C_U8C_SAT_SLOW_UNROLL                                  \
	{                                                               \
	    mlib_d64 dx1, dx, x_r1, z_f1h, z_f1l, z_r1h, z_r1l, x_r2,   \
		    z_f2h, z_f2l, z_r2h, z_r2l;                         \
	    dx1 = vis_ld_d64_nf(dpx);                                   \
	    dx = vis_faligndata(dx0, dx1);                              \
	    dx0 = dx1;                                                  \
	    dpx++;                                                      \
	    x_f2 = vis_fpmerge(vis_read_hi(dx), vis_read_lo(dx));       \
	    x_f1 = vis_fpmerge(vis_read_lo(dx), vis_read_hi(dx));       \
	    z_f2h = vis_fmuld8ulx16(vis_read_hi(x_f2), a_f);            \
	    x_r2 = vis_fpmerge(vis_read_hi(x_f2), vis_read_lo(x_f2));   \
	    z_f2l = vis_fmuld8ulx16(vis_read_lo(x_f2), a_f);            \
	    x_r2 = vis_fpmerge(vis_read_lo(x_r2), vis_read_hi(x_r2));   \
	    z_f1h = vis_fmuld8ulx16(vis_read_hi(x_f1), a_f);            \
	    x_r1 = vis_fpmerge(vis_read_lo(x_r2), vis_read_hi(x_r2));   \
	    z_r1h = vis_fmuld8ulx16(vis_read_hi(x_r1), a_r);            \
	    x_r2 = vis_fpmerge(vis_read_hi(x_r2), vis_read_lo(x_r2));   \
	    z_r2h = vis_fmuld8ulx16(vis_read_hi(x_r2), a_r);            \
	    z_1h = vis_fpadd32(z_f1h, z_r1h);                           \
	    z_r2l = vis_fmuld8ulx16(vis_read_lo(x_r2), a_r);            \
	    z_2h = vis_fpadd32(z_f2h, z_r2h);                           \
	    z_f1l = vis_fmuld8ulx16(vis_read_lo(x_f1), a_f);            \
	    z_2l = vis_fpadd32(z_f2l, z_r2l);                           \
	    z_r1l = vis_fmuld8ulx16(vis_read_lo(x_r1), a_r);            \
	    z_1l = vis_fpadd32(z_f1l, z_r1l);                           \
	    dx1 = vis_ld_d64_nf(dpx);                                   \
	    dx = vis_faligndata(dx0, dx1);                              \
	    dx0 = dx1;                                                  \
	    dpx++;                                                      \
	    x_f2 = vis_fpmerge(vis_read_hi(dx), vis_read_lo(dx));       \
	    x_f1 = vis_fpmerge(vis_read_lo(dx), vis_read_hi(dx));       \
	}

/*
 * Symbol table:
 *  extern symbols:
 *     db    (d64) - b value (re32, im32)
 *     a_f   (f32) - a value (re16, im16)
 *     a_r   (f32) - a value (-im16, re16)
 *     dpx   (d64*)- x mlib_d64 ptr (source data)
 *     buf   (d64*)- output buffer
 *  trans loop symbols:
 *     dx0   (d64) - hi part of dx
 *     x_f1  (d64) - (ACacBDbd)
 *     x_f2  (d64) - (CAcaDBdb)
 *     z_1h  (d64) - (Z_A, Z_a)
 *     z_1l  (d64) - (Z_B, Z_b)
 *     z_2h  (d64) - (Z_C, Z_c)
 *     z_2l  (d64) - (Z_D, Z_d)
 *
 *  temp local symbols:
 *     dx1   (d64) - lo part of dx
 *     dx    (d64) - x aligned (re8, im8){4} (AaBbCcDd)
 *     x_r1  (d64) - (acACbdBD)
 *     x_r2  (d64) - (caCAdbDB)
 *     z_f1h (d64) - (A*f, a*f)
 *     z_f1l (d64) - (B*f, b*f)
 *     z_f2h (d64) - (C*f, c*f)
 *     z_f2l (d64) - (D*f, d*f)
 *     z_r1h (d64) - (A*r, a*r)
 *     z_r1l (d64) - (B*r, b*r)
 *     z_r2h (d64) - (C*r, c*r)
 *     z_r2l (d64) - (D*r, d*r)
 *     z_1hp (d64) - (Z_A + B, Z_a + b)
 *     z_1lp (d64) - (Z_B + B, Z_b + b)
 *     z_2hp (d64) - (Z_C + B, Z_c + b)
 *     z_2lp (d64) - (Z_D + B, Z_d + b)
 *     z_1   (d64) - (Z_A, Z_a, Z_B, Z_b)
 *     z_2   (d64) - (Z_C, Z_c, Z_D, Z_d)
 */

/* *********************************************************** */

/* dst[12] = *buf, inc = buf++; */
#define	SCALE_S16C_U8C_SAT_SLOW_UNROLL_LOOP(dst1, dst2, inc,            \
	mov1)                                                           \
	{                                                               \
	    mlib_d64 dx1, dx, x_r1, x_r2, z_f1h, z_f1l, z_f2h, z_f2l,   \
		    z_1hp, z_1lp, z_1, z_r1h, z_r1l, z_r2h, z_r2l,      \
		    z_2hp, z_2lp, z_2;                                  \
	    z_1hp = vis_fpadd32(z_1h, db);                              \
	    dx1 = vis_ld_d64_nf(dpx);                                   \
	    z_f2h = vis_fmuld8ulx16(vis_read_hi(x_f2), a_f);            \
	    x_r2 = vis_fpmerge(vis_read_hi(x_f2), vis_read_lo(x_f2));   \
	    z_1lp = vis_fpadd32(z_1l, db);                              \
	    z_f2l = vis_fmuld8ulx16(vis_read_lo(x_f2), a_f);            \
	    x_r2 = vis_fpmerge(vis_read_lo(x_r2), vis_read_hi(x_r2));   \
	    z_2hp = vis_fpadd32(z_2h, db);                              \
	    z_f1h = vis_fmuld8ulx16(vis_read_hi(x_f1), a_f);            \
	    x_r1 = vis_fpmerge(vis_read_lo(x_r2), vis_read_hi(x_r2));   \
	    dx = vis_faligndata(dx0, dx1);                              \
	    dx0 = dx1;                                                  \
	    z_2lp = vis_fpadd32(z_2l, db);                              \
	    z_r1h = vis_fmuld8ulx16(vis_read_hi(x_r1), a_r);            \
	    x_r2 = vis_fpmerge(vis_read_hi(x_r2), vis_read_lo(x_r2));   \
	    z_1 = vis_fpackfix_pair(z_1hp, z_1lp);                      \
	    z_r2h = vis_fmuld8ulx16(vis_read_hi(x_r2), a_r);            \
	    dpx++;                                                      \
	    z_1h = vis_fpadd32(z_f1h, z_r1h);                           \
	    z_2 = vis_fpackfix_pair(z_2hp, z_2lp);                      \
	    x_f2 = vis_fpmerge(vis_read_hi(dx), vis_read_lo(dx));       \
	    z_r2l = vis_fmuld8ulx16(vis_read_lo(x_r2), a_r);            \
	    z_2h = vis_fpadd32(z_f2h, z_r2h);                           \
	    dst1;                                                       \
	    z_f1l = vis_fmuld8ulx16(vis_read_lo(x_f1), a_f);            \
	    inc;                                                        \
	    z_2l = vis_fpadd32(z_f2l, z_r2l);                           \
	    x_f1 = vis_fpmerge(vis_read_lo(dx), vis_read_hi(dx));       \
	    dst2;                                                       \
	    mov1;                                                       \
	    z_r1l = vis_fmuld8ulx16(vis_read_lo(x_r1), a_r);            \
	    inc;                                                        \
	    z_1l = vis_fpadd32(z_f1l, z_r1l);                           \
	}

/* *********************************************************** */

#define	SCALE_S16C_U8C_SAT_FAST_LOOP(dst1, dst2)                        \
	{                                                               \
	    mlib_d64 dx1, dx, x_r1, x_r2, z_f1h, z_f1l, z_f2h, z_f2l,   \
		    z_1hp, z_1lp, z_1, z_r1h, z_r1l, z_r2h, z_r2l,      \
		    z_2hp, z_2lp, z_2;                                  \
	    dx1 = vis_ld_d64_nf(dpx); dpx++;                            \
	    dx = vis_faligndata(dx0, dx1);                              \
	    dx0 = dx1;                                                  \
	    x_f2 = vis_fpmerge(vis_read_hi(dx), vis_read_lo(dx));       \
	    x_f1 = vis_fpmerge(vis_read_lo(dx), vis_read_hi(dx));       \
	    z_f2h = vis_fmuld8ulx16(vis_read_hi(x_f2), a_f);            \
	    z_f1h = vis_fmuld8ulx16(vis_read_hi(x_f1), a_f);            \
	    z_f2l = vis_fmuld8ulx16(vis_read_lo(x_f2), a_f);            \
	    z_f1l = vis_fmuld8ulx16(vis_read_lo(x_f1), a_f);            \
	    vis_write_bmask(0x51407362, 0);                             \
	    x_r1 = vis_bshuffle(dx, dx);                                \
	    vis_write_bmask(0x15043726, 0);                             \
	    x_r2 = vis_bshuffle(dx, dx);                                \
	    z_r1h = vis_fmuld8ulx16(vis_read_hi(x_r1), a_r);            \
	    z_1h = vis_fpadd32(z_f1h, z_r1h);                           \
	    z_r1l = vis_fmuld8ulx16(vis_read_lo(x_r1), a_r);            \
	    z_1l = vis_fpadd32(z_f1l, z_r1l);                           \
	    z_r2h = vis_fmuld8ulx16(vis_read_hi(x_r2), a_r);            \
	    z_2h = vis_fpadd32(z_f2h, z_r2h);                           \
	    z_r2l = vis_fmuld8ulx16(vis_read_lo(x_r2), a_r);            \
	    z_2l = vis_fpadd32(z_f2l, z_r2l);                           \
	    z_1hp = vis_fpadd32(z_1h, db);                              \
	    z_1lp = vis_fpadd32(z_1l, db);                              \
	    z_2hp = vis_fpadd32(z_2h, db);                              \
	    z_2lp = vis_fpadd32(z_2l, db);                              \
	    dz0 = vis_fpackfix_pair(z_1hp, z_1lp);                      \
	    dz1 = vis_fpackfix_pair(z_2hp, z_2lp);                      \
	    dst1;                                                       \
	    dst2;                                                       \
	}
/* *********************************************************** */

#define	SCALE_S16C_U8C_SAT_QUICK_UNROLL                         \
	dx1 = vis_ld_d64_nf(dpx);                               \
	dx = vis_faligndata(dx0, dx1);                          \
	dx0 = dx1;                                              \
	xd = vis_fpmerge(vis_read_hi(dx), vis_read_lo(dx));     \
	xd = vis_fpmerge(vis_read_hi(xd), vis_read_lo(xd));     \
	xd = vis_fpmerge(vis_read_lo(xd), vis_read_hi(xd));     \
	z0_hi = vis_fmul8x16(vis_read_hi(dx), da0);             \
	z1_hi = vis_fmul8x16(vis_read_hi(xd), da1);             \
	z0_lo = vis_fmul8x16(vis_read_lo(dx), da0);             \
	z1_lo = vis_fmul8x16(vis_read_lo(xd), da1);             \
	lz_hi = vis_fpadd16(z0_hi, z1_hi);                      \
	dpx++;                                                  \
	dx1 = vis_ld_d64_nf(dpx);                               \
	dx = vis_faligndata(dx0, dx1);                          \
	dx0 = dx1;                                              \
	xd = vis_fpmerge(vis_read_hi(dx), vis_read_lo(dx));     \
	xd = vis_fpmerge(vis_read_hi(xd), vis_read_lo(xd));     \
	xd = vis_fpmerge(vis_read_lo(xd), vis_read_hi(xd));     \
	dpx++

/* *********************************************************** */

#define	DUMMY

#define	SCALE_S16C_U8C_SAT_QUICK_UNROLL_LOOP(dst0, dst1, inc,        \
	mov1)                                                        \
	{                                                            \
	    dx1 = vis_ld_d64_nf(dpx);                                \
	    z0_hi = vis_fmul8x16(vis_read_hi(dx), da0);              \
	    z_lo = vis_fpadd16(z0_lo, z1_lo);                        \
	    ldx = vis_faligndata(dx0, dx1);                          \
	    dx0 = dx1;                                               \
	    z1_hi = vis_fmul8x16(vis_read_hi(xd), da1);              \
	    z_hi = vis_fpadd16(lz_hi, db);                           \
	    dpx++;                                                   \
	    lxd = vis_fpmerge(vis_read_hi(ldx), vis_read_lo(ldx));   \
	    z0_lo = vis_fmul8x16(vis_read_lo(dx), da0);              \
	    z_lo = vis_fpadd16(z_lo, db);                            \
	    lxd = vis_fpmerge(vis_read_hi(lxd), vis_read_lo(lxd));   \
	    dst0;                                                    \
	    z1_lo = vis_fmul8x16(vis_read_lo(xd), da1);              \
	    lz_hi = vis_fpadd16(z0_hi, z1_hi);                       \
	    inc;                                                     \
	    dst1;                                                    \
	    mov1;                                                    \
	    xd = vis_fpmerge(vis_read_lo(lxd), vis_read_hi(lxd));    \
	    dx = ldx;                                                \
	    inc;                                                     \
	}

/* *********************************************************** */

#define	SCALE_BUF_SIZE_SHIFT	8
#define	SCALE_BUF_SIZE	(1 << SCALE_BUF_SIZE_SHIFT)

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S16C_U8C_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *a,
	const mlib_u8 *b,
	mlib_s32 n)
{
	mlib_s16 *pz, *pzend;
	mlib_u8 *px;
	mlib_d64 *dpz;

	if (n <= 0)
		return (MLIB_FAILURE);

	vis_write_gsr(15 << 3);
	pz = z;
	px = (void *)x;
	pzend = (mlib_s16 *)((mlib_addr)(pz + n + n) - 1);
	dpz = (mlib_d64 *)((mlib_addr)pz & (~7));

	if (abs(a[0]) + abs(a[1]) < 127) {
/*
 * Quick case
 */

		mlib_d64 xd, z0_hi, z1_hi, z0_lo, z1_lo, z_hi, z_lo;
		mlib_d64 ldx, lxd, lz_hi;

		mlib_d64 da0 = vis_to_double_dup((a[0] << 24) | (a[0] << 8));
		mlib_d64 da1 = vis_to_double_dup((-a[1] << 24) | (a[1] << 8));
		mlib_d64 db = vis_to_double_dup((b[0] << 16) | b[1]);

		mlib_d64 *dpx, dx0, dx, dx1;

		if (!(((mlib_addr)pz & 3) | ((mlib_addr)px & 1))) {
			mlib_d64 *dpzend =
				(mlib_d64 *)((mlib_addr)pzend & (~7));

			dpzend = (mlib_d64 *)((mlib_addr)dpzend -
				(((mlib_addr)dpzend ^ (mlib_addr)dpz) & 8));
			{
				mlib_s32 mask, mask1;
				mlib_d64 dz0, dz1;

				mask = vis_edge16(pz, pzend);
				mask1 = vis_edge16(dpz + 1, pzend);

				if ((mlib_addr)pz & 7)
					dpx = vis_alignaddr(px, -2);
				else
					dpx = vis_alignaddr(px, 0);

				dx0 = vis_ld_d64_nf(dpx);
				dpx++;

				SCALE_S16C_U8C_SAT_QUICK_UNROLL;
				SCALE_S16C_U8C_SAT_QUICK_UNROLL_LOOP(dz0 =
					z_hi, dz1 = z_lo, DUMMY, DUMMY)
					vis_pst_16(dz0, dpz, mask);

				dpz++;
				if (dpz <= (mlib_d64 *)pzend) {
					vis_pst_16(dz1, dpz, mask1);
				}
				dpz++;
			}

#pragma pipeloop(0)
			for (; dpz < dpzend; )
				SCALE_S16C_U8C_SAT_QUICK_UNROLL_LOOP(*dpz =
					z_hi, *dpz = z_lo, dpz++, DUMMY);

				if ((mlib_s16 *)dpz <= pzend) {
					mlib_s32 mask, mask1;
					mlib_d64 dz0, dz1;

					mask = vis_edge16(dpz, pzend);
					mask1 = vis_edge16(dpz + 1, pzend);

					SCALE_S16C_U8C_SAT_QUICK_UNROLL_LOOP(dz0
						= z_hi, dz1 = z_lo, DUMMY,
						DUMMY);
					vis_pst_16(dz0, dpz, mask);
					if (dpz + 1 <= (mlib_d64 *)pzend) {
						vis_pst_16(dz1, dpz + 1, mask1);
					}
				}

		} else {
/*
 * not aligned
 */
			mlib_d64 dz0;
			mlib_s32 off = (mlib_addr)dpz - (mlib_addr)pz, i,
				boff = (8 + off) & 7;
			mlib_s32 mask, mask1;
			mlib_d64 dz1, dza1, dza2;

			dpx = vis_alignaddr(px, 0);
			dx0 = *dpx;
			dpx++;
			vis_write_bmask(boff * 0x11111111, 0x01234567);

			SCALE_S16C_U8C_SAT_QUICK_UNROLL;

			mask = vis_edge16(pz, pzend);
			mask1 = vis_edge16(dpz + 1, pzend);

			SCALE_S16C_U8C_SAT_QUICK_UNROLL_LOOP(dz1 = z_hi, dz0 =
				z_lo, DUMMY, DUMMY);

			dza1 = vis_bshuffle(dz1, dz1);
			vis_pst_16(dza1, dpz, mask);
			dpz++;
			n -= 2;

			if ((dpz <= (mlib_d64 *)pzend) && off) {
				dza2 = vis_bshuffle(dz1, dz0);
				vis_pst_16(dza2, dpz, mask1);
				dpz++;
				n -= 3 + (off >> 2);
			}
#pragma pipeloop(0)
			for (i = 0; i < (n >> 2); i++)
				SCALE_S16C_U8C_SAT_QUICK_UNROLL_LOOP((*dpz++) =
					vis_bshuffle(dz0, z_hi), (*dpz++) =
					vis_bshuffle(z_hi, z_lo), DUMMY,
					dz0 = z_lo);

				if ((mlib_s16 *)dpz <= pzend) {
					mlib_d64 dza0, dza1, dz1, dz2;
					mlib_s32 mask, mask1;

					SCALE_S16C_U8C_SAT_QUICK_UNROLL_LOOP(dz1
						= z_hi, dz2 = z_lo, DUMMY,
						DUMMY);

					mask = vis_edge16(dpz, pzend);
					mask1 = vis_edge16(dpz + 1, pzend);

					dza0 = vis_bshuffle(dz0, dz1);
					dza1 = vis_bshuffle(dz1, dz2);
					vis_pst_16(dza0, dpz, mask);
					if (dpz + 1 <= (mlib_d64 *)pzend) {
						vis_pst_16(dza1, dpz + 1,
							mask1);
					}
				}
		}

	} else {
/*
 * Long case big A
 */
		mlib_d64 x_f1, x_f2, z_1h, z_1l, z_2h, z_2l;
		mlib_d64 db = vis_to_double(b[0], b[1]);
		mlib_f32 a_f = vis_to_float((a[0] << 16) | a[0]);
		mlib_f32 a_r = vis_to_float((-(mlib_s32)a[1] << 16) | a[1]);
		mlib_s32 off = (mlib_addr)dpz - (mlib_addr)pz, i,
			boff = (8 + off) & 7;
		mlib_d64 *dpx, dx0, dz0, dz1;

		vis_write_gsr(16 << 3);

		if (!((((mlib_addr)pz & 3) | ((mlib_addr)px) & 1))) {
/*
 * aligned
 */
			mlib_d64 *dpzend =
				(mlib_d64 *)((mlib_addr)pzend & (~7));

			dpzend = (mlib_d64 *)((mlib_addr)dpzend -
				(((mlib_addr)dpzend ^ (mlib_addr)dpz) & 8));
			{
				mlib_s32 mask, mask1;

				mask = vis_edge16(pz, pzend);
				mask1 = vis_edge16(dpz + 1, pzend);

				if ((mlib_addr)pz & 7)
					dpx = vis_alignaddr(px, -2);
				else
					dpx = vis_alignaddr(px, 0);

				dx0 = vis_ld_d64_nf(dpx);
				dpx++;

				SCALE_S16C_U8C_SAT_FAST_LOOP(
					vis_pst_16(dz0,	dpz++, mask),
					if (dpz <= (mlib_d64 *)pzend)
					vis_pst_16(dz1,	dpz++, mask1));
			}

#pragma pipeloop(0)
			for (; dpz < dpzend; )
				SCALE_S16C_U8C_SAT_FAST_LOOP((*dpz++) =
					dz0, (*dpz++) = dz1);

				if ((mlib_s16 *)dpz <= pzend) {
					mlib_s32 mask, mask1;

					mask = vis_edge16(dpz, pzend);
					mask1 = vis_edge16(dpz + 1, pzend);

					SCALE_S16C_U8C_SAT_FAST_LOOP(vis_pst_16
						(dz0, dpz, mask),
						if (dpz + 1 <=
						(mlib_d64 *)pzend)
						vis_pst_16(dz1, dpz + 1, mask1))
				}

		} else {
/*
 * not aligned
 */
			mlib_d64 dz0, dz1, dza1, dza2;
			mlib_s32 mask, mask1;

			dpx = vis_alignaddr(px, 0);
			dx0 = (*dpx++);
			vis_write_bmask(boff * 0x11111111, 0x01234567);

			SCALE_S16C_U8C_SAT_SLOW_UNROLL;
			mask = vis_edge16(pz, pzend);

			mask1 = vis_edge16(dpz + 1, pzend);
			SCALE_S16C_U8C_SAT_SLOW_UNROLL_LOOP(dz1 = z_1, dz0 =
				z_2, DUMMY, DUMMY);

			dza1 = vis_bshuffle(dz1, dz1);
			vis_pst_16(dza1, dpz, mask);
			dpz++;
			n -= 2;

			if ((dpz <= (mlib_d64 *)pzend) && off) {
				dza2 = vis_bshuffle(dz1, dz0);
				vis_pst_16(dza2, dpz, mask1);
				dpz++;
				n -= 3 + (off >> 2);
			}
#pragma pipeloop(0)
			for (i = 0; i < (n >> 2); i++)
				SCALE_S16C_U8C_SAT_SLOW_UNROLL_LOOP((*dpz++) =
					vis_bshuffle(dz0, z_1), (*dpz++) =
					vis_bshuffle(z_1, z_2), DUMMY,
					dz0 = z_2);

				if ((mlib_s16 *)dpz <= pzend) {
					mlib_d64 dza0, dza1, dz1, dz2;
					mlib_s32 mask, mask1;

					SCALE_S16C_U8C_SAT_SLOW_UNROLL_LOOP(dz1
						= z_1, dz2 = z_2, DUMMY, DUMMY);
					mask = vis_edge16(dpz, pzend);
					mask1 = vis_edge16(dpz + 1, pzend);

					dza0 = vis_bshuffle(dz0, dz1);
					dza1 = vis_bshuffle(dz1, dz2);
					vis_pst_16(dza0, dpz, mask);
					if (dpz + 1 <= (mlib_d64 *)pzend) {
						vis_pst_16(dza1, dpz + 1,
							mask1);
					}
				}
		}
	}
	return (MLIB_SUCCESS);
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

#define	SCALE_S8C_SAT_1                                         \
	{                                                       \
	    mlib_d64 dr7, dr8;                                  \
	                                                        \
	    dr7 = vis_fmul8sux16(dx, da_r);                     \
	    dr8 = vis_fmul8sux16(dx, da_i);                     \
	    dx = vis_fpack32(dx, dx);                           \
	    dr2 = vis_fmul8sux16(dx, da_r);                     \
	    dr1 = vis_fmul8sux16(dx, da_i);                     \
	    dr3 = vis_fpadd16(dr7, db_r);                       \
	    dr4 = vis_fpadd16(dr8, db_i);                       \
	}

/* *********************************************************** */

#define	SCALE_S8C_SAT_2                                         \
	dr5 = vis_fpsub16(dr3, dr1);                            \
	dr6 = vis_fpadd16(dr4, dr2);                            \
	dr = vis_fpack16_pair(dr5, dr6);                        \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr));     \
	dr = vis_fxnor(dr, restore)

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *a,
	const mlib_s8 *b,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s8 *pzend = (mlib_s8 *)z + n + n - 1;
	mlib_u16 ua_r = ((mlib_u8 *)a)[0] << 8;
	mlib_u16 ua_i = ((mlib_u8 *)a)[1] << 8;
	mlib_u16 ub_r = ((mlib_s8 *)b)[0] + 128;
	mlib_u16 ub_i = ((mlib_s8 *)b)[1] + 128;
	mlib_d64 *dpx, *dpz, *dpzend;
	mlib_d64 dx, dx0, dx1, dr, dr1, dr2, dr3, dr4, dr5, dr6;

	mlib_d64 restore = vis_to_double_dup(0x7f7f7f7f);
	mlib_d64 db_r = vis_to_double_dup(ub_r | (ub_r << 16));
	mlib_d64 db_i = vis_to_double_dup(ub_i | (ub_i << 16));
	mlib_d64 da_r = vis_to_double_dup(ua_r | (ua_r << 16));
	mlib_d64 da_i = vis_to_double_dup(ua_i | (ua_i << 16));

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

/*
 * generate edge mask for the start bytes
 */
	emask = vis_edge8(z, pzend);
	dx = vis_faligndata(dx0, dx1);
	SCALE_S8C_SAT_1;
	SCALE_S8C_SAT_2;

	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx + 2);
	dx = vis_faligndata(dx0, dx1);

/*
 * if (|a_r| + |a_i|) > 254   then overflow possibly
 */

	if ((abs(((mlib_s8 *)a)[0]) + abs(((mlib_s8 *)a)[1])) >
		(MLIB_U8_MAX - 1)) {
		mlib_d64 dover_r = vis_to_double_dup(0x80808080);
		mlib_d64 dover_i = vis_to_double_dup(0x81008100);
		mlib_s32 mask_r, mask_i;

		if (!(1 & (mlib_addr)z)) {
/* store first bytes of result */
			vis_pst_8(dr, dpz, emask);
			MASK_OVER_S8C0;
			vis_pst_8(restore, dpz, mask_r & emask);
			dpz++;

			SCALE_S8C_SAT_1;
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx + 3);
			dx = vis_faligndata(dx0, dx1);

#pragma pipeloop(0)
			for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dpx + 4);
				SCALE_S8C_SAT_2;
				SCALE_S8C_SAT_1;
/* store 8 bytes of result */
				dpz[0] = dr;
				MASK_OVER_S8C0;
				vis_pst_8(restore, dpz, mask_r);
				dx = vis_faligndata(dx0, dx1);
				dpx++;
				dpz++;
			}

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				SCALE_S8C_SAT_2;
/* prepare edge mask for the last bytes */
				emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
				vis_pst_8(dr, dpz, emask);
				MASK_OVER_S8C0;
				vis_pst_8(restore, dpz, mask_r & emask);
			}

		} else {
			MASK_OVER_S8C1;
			vis_write_bmask(0x789abcde, 0);
			dr = vis_bshuffle(dr, dr);
/* store first 7 bytes of result */
			vis_pst_8(dr, dpz, emask);
			vis_pst_8(restore, dpz, mask_r & emask);

			if ((mlib_addr)++dpz <= (mlib_addr)pzend) {
				vis_pst_8(dr, dpz, 0x80);
				vis_pst_8(restore, dpz, mask_i);
			}

			SCALE_S8C_SAT_1;
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx + 3);
			(void *)vis_alignaddr((void *)x, off);
			dx = vis_faligndata(dx0, dx1);

#pragma pipeloop(0)
			for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dpx + 4);
				SCALE_S8C_SAT_2;
				SCALE_S8C_SAT_1;
				MASK_OVER_S8C1;
				dr = vis_bshuffle(dr, dr);
/* store 7 bytes of result */
				vis_pst_8(dr, dpz, 0x7f);
				vis_pst_8(restore, dpz, mask_r);
				dpz++;
/* store 1 byte of result to the next word */
				vis_pst_8(dr, dpz, 0x80);
				vis_pst_8(restore, dpz, mask_i);
				dx = vis_faligndata(dx0, dx1);
				dpx++;
			}

			if ((mlib_addr)dpz < (mlib_addr)pzend) {
				SCALE_S8C_SAT_2;
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

			SCALE_S8C_SAT_1;
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx + 3);
			dx = vis_faligndata(dx0, dx1);

			if ((((mlib_addr)x ^ (mlib_addr)z) & 7) == 0) {
#pragma pipeloop(0)
				for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
					SCALE_S8C_SAT_2;
					SCALE_S8C_SAT_1;
					dx = vis_ld_d64_nf(dpx + 3);
/* store 8 bytes of result */
					dpz[0] = dr;
					dpx++;
					dpz++;
				}

			} else {
#pragma pipeloop(0)
				for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
					dx0 = dx1;
					dx1 = vis_ld_d64_nf(dpx + 4);
					SCALE_S8C_SAT_2;
					SCALE_S8C_SAT_1;
/* store 8 bytes of result */
					dpz[0] = dr;
					dx = vis_faligndata(dx0, dx1);
					dpx++;
					dpz++;
				}
			}

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				SCALE_S8C_SAT_2;
/* prepare edge mask for the last bytes */
				emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
				vis_pst_8(dr, dpz, emask);
			}

		} else {
			vis_write_bmask(0x789abcde, 0);
			dr = vis_bshuffle(dr, dr);
/* store first 7 bytes of result */
			vis_pst_8(dr, dpz, emask);

			if ((mlib_addr)++dpz <= (mlib_addr)pzend)
				vis_pst_8(dr, dpz, 0x80);

			SCALE_S8C_SAT_1;
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx + 3);
			(void *)vis_alignaddr((void *)x, off);
			dx = vis_faligndata(dx0, dx1);

#pragma pipeloop(0)
			for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dpx + 4);
				SCALE_S8C_SAT_2;
				SCALE_S8C_SAT_1;
				dr = vis_bshuffle(dr, dr);
/* store 7 bytes of result */
				vis_pst_8(dr, dpz, 0x7f);
				dpz++;
/* store 1 byte of result to the next word */
				vis_pst_8(dr, dpz, 0x80);
				dx = vis_faligndata(dx0, dx1);
				dpx++;
			}

			if ((mlib_addr)dpz < (mlib_addr)pzend) {
				SCALE_S8C_SAT_2;
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

mlib_status
__mlib_VectorScale_S16C_S8C_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *a,
	const mlib_s8 *b,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx;
	mlib_d64 dx, dx0, dx1;
	mlib_d64 dr, dr1, dzh, dzl;
	mlib_s8 *px;
	mlib_s16 *pz;
	mlib_s8 ua0 = ((mlib_s8 *)a)[0], ua1 = ((mlib_s8 *)a)[1];
	mlib_s8 ub0 = ((mlib_s8 *)b)[0], ub1 = ((mlib_s8 *)b)[1];

/*
 * prepare the scaling factors 'a', 'b'
 */
	mlib_d64 db0 = vis_to_double_dup((ub0 & 0xffff) | (ub0 << 16));
	mlib_d64 db1 = vis_to_double_dup((ub1 & 0xffff) | (ub1 << 16));
	mlib_d64 da0 = vis_to_double_dup(((ua0 << 8) & 0xffff) | (ua0 << 24));
	mlib_d64 da1 = vis_to_double_dup(((ua1 << 8) & 0xffff) | (ua1 << 24));
	mlib_d64 dzero = vis_to_double_dup(0), rest;

/*
 * odd = 1 means "aligned" resultant vector starts with Im. part.
 */
	mlib_s32 odd = 0;
	mlib_s32 len = n, i;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_s32 re_x, im_x, re_c, im_c;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s8 *)x;
	pz = (mlib_s16 *)z;

	if (n <= 8)
		SCALE_S32C_S16C_SAT_IN_C;

	if (!(ua0 | ua1 | ub0 | ub1))

		return (__mlib_VectorZero_S16C(z, n));
	else if (!(ua0 | ua1)) {
		mlib_s16 tmp_arr[2];

		tmp_arr[0] = ub0;
		tmp_arr[1] = ub1;

		return (__mlib_VectorSet_S16C(z, tmp_arr, n));
	} else if (!(ub0 | ub1))

		return (__mlib_VectorMulS_S16C_S8C_Mod(z, x, a, n));

/*
 * prepare the destination address trying to align it for 8 bytes.
 */

	while ((mlib_addr)pz & 7) {
		re_x = (*px++);
		im_x = (*px++);
		re_c = re_x * ua0 - im_x * ua1 + ub0;
		im_c = im_x * ua0 + re_x * ua1 + ub1;
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

	if ((!((mlib_addr)px & 7))) {

/*
 * X address is 8-byte aligned.
 * No  vis_alignaddr and  vis_faligndata.
 */

		dpx = (mlib_d64 *)px;
		dx = dpx[0];
		dpx++;

		if (!odd) {
/*
 * Start with real part.
 */
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
				SCALE_S16C_S8C_MOD;
				vis_write_bmask(0x018923ab, 0);
				dzh = vis_bshuffle(dr, dr1);
				vis_write_bmask(0x45cd67ef, 0);
				dzl = vis_bshuffle(dr, dr1);
				dx = dx1;
				(*dpz++) = dzh;
				(*dpz++) = dzl;
				dpx++;
			}

		} else {
			mlib_d64 dzh0;

/*
 * Start with imaginary part
 */
			vis_alignaddr((void *)6, 0);
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
				SCALE_S16C_S8C_MOD;
				vis_write_bmask(0x018923ab, 0);
				dzh0 = vis_bshuffle(dr, dr1);
				vis_write_bmask(0xab45cd67, 0);
				dzl = vis_bshuffle(dr, dr1);
				dzh = vis_faligndata(rest, dzh0);
				rest = dr1;
				dx = dx1;
				(*dpz++) = dzh;
				(*dpz++) = dzl;
				dpx++;
			}

			rest = vis_faligndata(rest, dzero);
			vis_pst_16(rest, pz, 0x8);
			pz++;
		}

	} else {

/*
 * X address is arbitrary aligned.
 * 1 vis_alignaddr and 1 vis_faligndata in the loop.
 */

		dpx = vis_alignaddr(px, 0);
		dx0 = (*dpx++);

		if (odd) {
			mlib_d64 dzh0;

#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				vis_alignaddr(px, 0);
				dx1 = vis_ld_d64_nf(dpx);
				dx = vis_faligndata(dx0, dx1);
				SCALE_S16C_S8C_MOD;
				vis_alignaddr((void *)6, 0);
				vis_write_bmask(0x018923ab, 0);
				dzh0 = vis_bshuffle(dr, dr1);
				vis_write_bmask(0xab45cd67, 0);
				dzl = vis_bshuffle(dr, dr1);
				dzh = vis_faligndata(rest, dzh0);
				rest = dr1;
				dx0 = dx1;
/*
 * store 16 bytes of result
 */
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
				dpx++;
			}

			rest = vis_faligndata(rest, dzero);
			vis_pst_16(rest, pz, 0x8);
			pz++;
		} else {
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
				dx = vis_faligndata(dx0, dx1);
				SCALE_S16C_S8C_MOD;
				vis_write_bmask(0x018923ab, 0);
				dzh = vis_bshuffle(dr, dr1);
				vis_write_bmask(0x45cd67ef, 0);
				dzl = vis_bshuffle(dr, dr1);
				dx0 = dx1;
/*
 * store 16 bytes of result
 */
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
				dpx++;
			}
		}
	}

	px += (even_8 << 3);

	while (rest_8 > 0) {
		re_x = (*px++);
		im_x = (*px++);
		re_c = re_x * ua0 - im_x * ua1 + ub0;
		im_c = im_x * ua0 + re_x * ua1 + ub1;
		(*pz++) = re_c;
		(*pz++) = im_c;
		rest_8 -= 2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S16C_S8C_Sat(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *a,
	const mlib_s8 *b,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx;
	mlib_d64 dx, dx0, dx1;
	mlib_d64 dr, dr1, dzh, dzl;
	mlib_s8 *px;
	mlib_s16 *pz;
	mlib_s8 ua0 = ((mlib_s8 *)a)[0], ua1 = ((mlib_s8 *)a)[1];
	mlib_s8 ub0 = ((mlib_s8 *)b)[0], ub1 = ((mlib_s8 *)b)[1];
	mlib_s32 len = n, i;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;

/*
 * prepare the scaling factors 'a', 'b'
 */
	mlib_d64 db0 = vis_to_double_dup((ub0 & 0xffff) | (ub0 << 16));
	mlib_d64 db1 = vis_to_double_dup((ub1 & 0xffff) | (ub1 << 16));
	mlib_d64 da0 = vis_to_double_dup(((ua0 << 8) & 0xffff) | (ua0 << 24));
	mlib_d64 da1 = vis_to_double_dup(((ua1 << 8) & 0xffff) | (ua1 << 24));
	mlib_d64 dzero = vis_to_double_dup(0), rest;
	mlib_d64 d_cntr_ovlp = vis_to_double_dup(0x80008080);
	mlib_d64 d_cntr_ovlp1 = vis_to_double_dup(0x80808000);
	mlib_d64 const_max_16 =
		vis_to_double_dup((MLIB_S16_MAX << 16) | MLIB_S16_MAX);

/*
 * odd = 1 means "aligned" resultant vector starts with Im. part.
 */
	mlib_s32 odd = 0;
	mlib_s32 re_x, im_x, re_c, im_c;
	mlib_s32 mask, mask1;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s8 *)x;
	pz = (mlib_s16 *)z;

	if (n <= 8)
		SCALE_S16C_S8C_SAT_IN_C;

	if (!(ua0 | ua1 | ub0 | ub1))

		return (__mlib_VectorZero_S16C(z, n));
	else if (!(ua0 | ua1)) {
		mlib_s16 tmp_arr[2];

		tmp_arr[0] = ub0;
		tmp_arr[1] = ub1;

		return (__mlib_VectorSet_S16C(z, tmp_arr, n));

	} else if (!(ub0 | ub1))

		return (__mlib_VectorMulS_S16C_S8C_Sat(z, x, a, n));
	else if (!(ua0 == MLIB_S8_MIN && ua1 == MLIB_S8_MIN))

		return (__mlib_VectorScale_S16C_S8C_Mod(z, x, a, b, n));

/*
 * prepare the destination address trying to align it for 8 bytes.
 */

	while ((mlib_addr)pz & 7) {
		re_x = (*px++);
		im_x = (*px++);
		re_c = re_x * ua0 - im_x * ua1 + ub0;
		im_c = im_x * ua0 + re_x * ua1 + ub1;
		len--;
		(*pz++) = re_c;
		im_c = im_c > MLIB_S16_MAX ? MLIB_S16_MAX : im_c;

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

	if ((!((mlib_addr)px & 7))) {

/*
 * X address is 8-byte aligned.
 * No  vis_alignaddr and vis_faligndata.
 */

		dpx = (mlib_d64 *)px;
		dx = dpx[0];
		dpx++;

		if (!odd) {
/*
 * Start with real part.
 */
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
/*
 * initialize GSR for SCALE_U8C_S16C
 */
				SCALE_S16C_S8C_MOD;
				vis_write_bmask(0x018923ab, 0);
				dzh = vis_bshuffle(dr, dr1);
				vis_write_bmask(0x45cd67ef, 0);
				dzl = vis_bshuffle(dr, dr1);
				mask = vis_fcmpgt16(d_cntr_ovlp, dzh);
				mask1 = vis_fcmpgt16(d_cntr_ovlp, dzl);
				dx = dx1;
				dpz[0] = dzh;
				dpz[1] = dzl;
				vis_pst_16(const_max_16, dpz, mask);
				vis_pst_16(const_max_16, dpz + 1, mask1);
				dpz += 2;
				dpx++;
			}

		} else {
			mlib_d64 dzh0;

/*
 * Start with imaginary part
 */
			vis_alignaddr((void *)6, 0);
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
/*
 * initialize GSR for SCALE_U8C_S16C
 */
				SCALE_S16C_S8C_MOD;
				vis_write_bmask(0x018923ab, 0);
				dzh0 = vis_bshuffle(dr, dr1);
				vis_write_bmask(0xab45cd67, 0);
				dzl = vis_bshuffle(dr, dr1);
				dzh = vis_faligndata(rest, dzh0);
				rest = dr1;
				mask = vis_fcmpgt16(d_cntr_ovlp1, dzh);
				mask1 = vis_fcmpgt16(d_cntr_ovlp1, dzl);
				dx = dx1;
				dpz[0] = dzh;
				dpz[1] = dzl;
				vis_pst_16(const_max_16, dpz, mask);
				vis_pst_16(const_max_16, dpz + 1, mask1);
				dpz += 2;
				dpx++;
			}

			rest = vis_faligndata(rest, dzero);
			mask1 = vis_fcmpgt16(d_cntr_ovlp1, rest);
			vis_pst_16(rest, pz, 0x8);
			vis_pst_16(const_max_16, pz, mask1);
			pz++;
		}

	} else {

/*
 * X address is arbitrary aligned.
 * 1 vis_alignaddr and 1 vis_faligndata in the loop.
 */

		dpx = vis_alignaddr(px, 0);
		dx0 = (*dpx++);

		if (odd) {
			mlib_d64 dzh0;

/*
 * Start with imaginary part
 */
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				vis_alignaddr(px, 0);
				dx1 = vis_ld_d64_nf(dpx);
				dx = vis_faligndata(dx0, dx1);
				SCALE_S16C_S8C_MOD;
				vis_alignaddr((void *)6, 0);
				vis_write_bmask(0x018923ab, 0);
				dzh0 = vis_bshuffle(dr, dr1);
				vis_write_bmask(0xab45cd67, 0);
				dzl = vis_bshuffle(dr, dr1);
				dzh = vis_faligndata(rest, dzh0);
				rest = dr1;
				mask = vis_fcmpgt16(d_cntr_ovlp1, dzh);
				mask1 = vis_fcmpgt16(d_cntr_ovlp1, dzl);
				dx0 = dx1;
/*
 * store 16 bytes of result
 */
				dpz[0] = dzh;
				dpz[1] = dzl;
				vis_pst_16(const_max_16, dpz, mask);
				vis_pst_16(const_max_16, dpz + 1, mask1);
				dpz += 2;
				dpx++;
			}

			rest = vis_faligndata(rest, dzero);
			mask1 = vis_fcmpgt16(d_cntr_ovlp1, rest);
			vis_pst_16(rest, pz, 0x8);
			vis_pst_16(const_max_16, pz, mask1);
			pz++;
		} else {
/*
 * Start with real part
 */
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
				dx = vis_faligndata(dx0, dx1);
				SCALE_S16C_S8C_MOD;
				vis_write_bmask(0x018923ab, 0);
				dzh = vis_bshuffle(dr, dr1);
				vis_write_bmask(0x45cd67ef, 0);
				dzl = vis_bshuffle(dr, dr1);
				mask = vis_fcmpgt16(d_cntr_ovlp, dzh);
				mask1 = vis_fcmpgt16(d_cntr_ovlp, dzl);
				dx0 = dx1;
/*
 * store 16 bytes of result
 */
				dpz[0] = dzh;
				dpz[1] = dzl;
				vis_pst_16(const_max_16, dpz, mask);
				vis_pst_16(const_max_16, dpz + 1, mask1);
				dpz += 2;
				dpx++;
			}
		}
	}

	px += (even_8 << 3);

	while (rest_8 > 0) {
		re_x = (*px++);
		im_x = (*px++);
		re_c = re_x * ua0 - im_x * ua1 + ub0;
		im_c = im_x * ua0 + re_x * ua1 + ub1;
		(*pz++) = re_c;
		(*pz++) = im_c > MLIB_S16_MAX ? MLIB_S16_MAX : im_c;
		rest_8 -= 2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S16C_U8C_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *a,
	const mlib_u8 *b,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx;
	mlib_d64 dx, dx0, dx1;
	mlib_d64 dr, dr1, dzh, dzl;
	mlib_u8 *px;
	mlib_s16 *pz;
	mlib_u16 ua0 = ((mlib_u8 *)a)[0], ua1 = ((mlib_u8 *)a)[1];
	mlib_u16 ub0 = ((mlib_u8 *)b)[0], ub1 = ((mlib_u8 *)b)[1];

/* odd = 1 means "aligned" resultant vector starts with Im. part. */
	mlib_s32 odd = 0;
	mlib_s32 len = n, i;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_d64 dzero = vis_to_double_dup(0), rest;
	mlib_s32 re_x, im_x, re_c, im_c;
	mlib_d64 dr2, dr3, dr4, dr5, dr6, dr7;
	mlib_f32 fa0, fa1;
	mlib_d64 db0 = vis_to_double_dup(ub0), db1 = vis_to_double_dup(ub1);

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_u8 *)x;
	pz = (mlib_s16 *)z;

	if (n <= 8)
		SCALE_S32C_S16C_SAT_IN_C;

	if (!(ua0 | ua1 | ub0 | ub1))

		return (__mlib_VectorZero_S16C(z, n));
	else if (!(ua0 | ua1)) {
		mlib_s16 tmp_arr[2];

		tmp_arr[0] = ub0;
		tmp_arr[1] = ub1;

		return (__mlib_VectorSet_S16C(z, tmp_arr, n));
	} else if (!(ub0 | ub1))

		return (__mlib_VectorMulS_S16C_U8C_Mod(z, x, a, n));

/*
 * prepare the destination address trying to align it for 8 bytes.
 */

	fa0 = vis_to_float((ua0 << 16) | ua0);
	fa1 = vis_to_float((ua1 << 16) | ua1);

	while ((mlib_addr)pz & 7) {
		re_x = (*px++);
		im_x = (*px++);
		re_c = re_x * ua0 - im_x * ua1 + ub0;
		im_c = im_x * ua0 + re_x * ua1 + ub1;
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
	vis_write_bmask(0x23ab67ef, 0);

	if ((!((mlib_addr)px & 7))) {

/*
 * X address is 8-byte aligned.
 * No  vis_alignaddr and  vis_faligndata.
 */

		dpx = (mlib_d64 *)px;
		dx = dpx[0];
		dpx++;

		if (!odd) {
/*
 * Start with real part.
 */
/* initialize GSR */
			vis_alignaddr((void *)7, 0);
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
				SCALE_S16C_U8C_FOR16_MOD;
				dzh = vis_bshuffle(dr, dr2);
				dzl = vis_bshuffle(dr1, dr3);
				dx = dx1;
				(*dpz++) = dzh;
				(*dpz++) = dzl;
				dpx++;
			}

		} else {
/*
 * Start with imaginary part
 */
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
/* initialize GSR */
				vis_alignaddr((void *)7, 0);
				SCALE_S16C_U8C_FOR16_MOD;
				dzh = vis_bshuffle(dr, dr2);
				dzl = vis_bshuffle(dr1, dr3);
				vis_alignaddr((void *)6, 0);
				dr = vis_faligndata(rest, dzh);
				rest = dzl;
				dzl = vis_faligndata(dzh, rest);
				dzh = dr;
				dx = dx1;
				(*dpz++) = dzh;
				(*dpz++) = dzl;
				dpx++;
			}

			vis_alignaddr((void *)6, 0);
			rest = vis_faligndata(rest, dzero);
			vis_pst_16(rest, pz, 0x8);
			pz++;
		}

	} else {

/*
 * X address is arbitrary aligned.
 * 1 vis_alignaddr and 1 vis_faligndata in the loop.
 */

		dpx = vis_alignaddr(px, 0);
		dx0 = (*dpx++);
		dx1 = vis_ld_d64_nf(dpx);
		dx = vis_faligndata(dx0, dx1);

		if (odd) {
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
				dx = vis_faligndata(dx0, dx1);
/* initialize GSR */
				vis_alignaddr((void *)7, 0);
				SCALE_S16C_U8C_FOR16_MOD;
				dzh = vis_bshuffle(dr, dr2);
				dzl = vis_bshuffle(dr1, dr3);
				vis_alignaddr((void *)6, 0);
				dr = vis_faligndata(rest, dzh);
				rest = dzl;
				dzl = vis_faligndata(dzh, rest);
				dzh = dr;
				vis_alignaddr(px, 0);
				dx0 = dx1;
/*
 * store 16 bytes of result
 */
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
				dpx++;
			}

			vis_alignaddr((void *)6, 0);
			rest = vis_faligndata(rest, dzero);
			vis_pst_16(rest, pz, 0x8);
			pz++;
		} else {
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
				dx = vis_faligndata(dx0, dx1);
/* initialize GSR */
				vis_alignaddr((void *)7, 0);
				SCALE_S16C_U8C_FOR16_MOD;
				dzh = vis_bshuffle(dr, dr2);
				dzl = vis_bshuffle(dr1, dr3);
				vis_alignaddr(px, 0);
				dx0 = dx1;
/*
 * store 16 bytes of result
 */
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
				dpx++;
			}
		}
	}

	px += (even_8 << 3);

	while (rest_8 > 0) {
		re_x = (*px++);
		im_x = (*px++);
		re_c = re_x * ua0 - im_x * ua1 + ub0;
		im_c = im_x * ua0 + re_x * ua1 + ub1;
		(*pz++) = re_c;
		(*pz++) = im_c;
		rest_8 -= 2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SCALE_S8C_MOD_1                                         \
	dr5 = vis_fmul8sux16(dx, da_r);                         \
	dr6 = vis_fmul8sux16(dx, da_i);                         \
	dx = vis_fpack32(dx, dx);                               \
	dr2 = vis_fmul8sux16(dx, da_r);                         \
	dr1 = vis_fmul8sux16(dx, da_i);                         \
	dr3 = vis_fpadd16(dr5, db_r);                           \
	dr4 = vis_fpadd16(dr6, db_i)

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S8C_S8C_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *a,
	const mlib_s8 *b,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s8 *pzend = (mlib_s8 *)z + n + n - 1;
	mlib_u16 ua_r = ((mlib_u8 *)a)[0] << 8;
	mlib_u16 ua_i = ((mlib_u8 *)a)[1] << 8;
	mlib_u16 ub_r = ((mlib_u8 *)b)[0];
	mlib_u16 ub_i = ((mlib_u8 *)b)[1];
	mlib_d64 *dpx, *dpz, *dpzend;
	mlib_d64 dx, dx0, dx1, dr, dr1, dr2, dr3, dr4, dr5, dr6;

	mlib_d64 db_r = vis_to_double_dup(ub_r | (ub_r << 16));
	mlib_d64 db_i = vis_to_double_dup(ub_i | (ub_i << 16));
	mlib_d64 da_r = vis_to_double_dup(ua_r | (ua_r << 16));
	mlib_d64 da_i = vis_to_double_dup(ua_i | (ua_i << 16));

	if (n <= 0)
		return (MLIB_FAILURE);

/*
 * prepare the destination address
 */
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z + (1 & (mlib_addr)z);

	vis_write_bmask(0x193b5d7f, 0);

/*
 * prepare the source address
 */
	dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);

/*
 * generate edge mask for the start bytes
 */
	emask = vis_edge8(z, pzend);
	dx = vis_faligndata(dx0, dx1);
	SCALE_S8C_MOD_1;
	dr5 = vis_fpsub16(dr3, dr1);
	dr6 = vis_fpadd16(dr4, dr2);
	dr = vis_bshuffle(dr5, dr6);

	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx + 2);
	dx = vis_faligndata(dx0, dx1);

	if (!(1 & (mlib_addr)z)) {
/* store first bytes of result */
		vis_pst_8(dr, dpz, emask);
		dpz++;

		SCALE_S8C_MOD_1;
		dx0 = dx1;

#pragma pipeloop(0)
		for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
			dx1 = vis_ld_d64_nf(dpx + 3);
			dx = vis_faligndata(dx0, dx1);
			dr5 = vis_fpsub16(dr3, dr1);
			dr6 = vis_fpadd16(dr4, dr2);
			dr = vis_bshuffle(dr5, dr6);
			SCALE_S8C_MOD_1;
			dx0 = dx1;
/* store 8 bytes of result */
			dpz[0] = dr;
			dpx++;
			dpz++;
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			dx1 = vis_ld_d64_nf(dpx + 3);
			dx = vis_faligndata(dx0, dx1);
			dr5 = vis_fpsub16(dr3, dr1);
			dr6 = vis_fpadd16(dr4, dr2);
			dr = vis_bshuffle(dr5, dr6);
/* prepare edge mask for the last bytes */
			emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
			vis_pst_8(dr, dpz, emask);
		}

	} else {
		(void *)vis_alignaddr(dpx, 7);
		dr = vis_faligndata(dr, dr);
/* store first 7 bytes of result */
		vis_pst_8(dr, dpz, emask);

		if ((mlib_addr)++dpz <= (mlib_addr)pzend)
			vis_pst_8(dr, dpz, 0x80);

		SCALE_S8C_MOD_1;
		dx0 = dx1;
		dx1 = vis_ld_d64_nf(dpx + 3);
		(void *)vis_alignaddr((void *)x, off);
		dx = vis_faligndata(dx0, dx1);
		vis_write_bmask(0xf193b5d7, 0);

#pragma pipeloop(0)
		for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx + 4);
			dr5 = vis_fpsub16(dr3, dr1);
			dr6 = vis_fpadd16(dr4, dr2);
			dr = vis_bshuffle(dr5, dr6);
			SCALE_S8C_MOD_1;
/* store 7 bytes of result */
			vis_pst_8(dr, dpz, 0x7f);
			dpz++;
/* store 1 byte of result to the next word */
			vis_pst_8(dr, dpz, 0x80);
			dx = vis_faligndata(dx0, dx1);
			dpx++;
		}

		if ((mlib_addr)dpz < (mlib_addr)pzend) {
			dr5 = vis_fpsub16(dr3, dr1);
			dr6 = vis_fpadd16(dr4, dr2);
			dr = vis_bshuffle(dr5, dr6);
/* prepare edge mask for the last bytes */
			emask = vis_edge8(dpz, pzend);
/* store 7 bytes of result */
			vis_pst_8(dr, dpz, emask & 0x7f);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *a,
	const mlib_u8 *b,
	mlib_s32 n)
{
	return __mlib_VectorScale_S8C_S8C_Mod((mlib_s8 *)z, (mlib_s8 *)x,
		(mlib_s8 *)a, (mlib_s8 *)b, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S8C_Mod(
	mlib_s8 *xz,
	const mlib_s8 *a,
	const mlib_s8 *b,
	mlib_s32 n)
{
	return (__mlib_VectorScale_S8C_S8C_Mod(xz, xz, a, b, n));
}

/* *********************************************************** */

/*
 * Quick algorithm: (fa_r = a[0] << 16 | a[1] ) etc.
 *
 *  z_r0h = vis_fmuld8sux16(vis_read_hi(dx), fa_r);
 *  z_r0l = vis_fmuld8ulx16(vis_read_hi(dx), fa_r);
 *  z_i0h = vis_fmuld8sux16(vis_read_hi(dx), fa_i);
 *  z_i0l = vis_fmuld8ulx16(vis_read_hi(dx), fa_i);
 *  z_r1h = vis_fmuld8sux16(vis_read_lo(dx), fa_r);
 *  z_r1l = vis_fmuld8ulx16(vis_read_lo(dx), fa_r);
 *  z_i1h = vis_fmuld8sux16(vis_read_lo(dx), fa_i);
 *  z_i1l = vis_fmuld8ulx16(vis_read_lo(dx), fa_i);
 *  z_r0  = vis_fpadd32(z_r0h, z_r0l);
 *  z_i0  = vis_fpadd32(z_i0h, z_i0l);
 *  z_r1  = vis_fpadd32(z_r1h, z_r1l);
 *  z_i1  = vis_fpadd32(z_i1h, z_i1l);
 *  z_0   = vis_freg_pair(vis_fpsub32s(vis_read_hi(z_r0), vis_read_lo(z_i0)),
 *      vis_fpadd32s(vis_read_lo(z_r0), vis_read_hi(z_i0)));
 *  z_1   = vis_freg_pair(vis_fpsub32s(vis_read_hi(z_r1), vis_read_lo(z_i1)),
 *      vis_fpadd32s(vis_read_lo(z_r1), vis_read_hi(z_i1)));
 *  z_0b  = vis_fpadd32(z_0, db);
 *  z_1b  = vis_fpadd32(z_1, db);
 *  z_0b  = vis_fpadd32(z_0b, z_0b);
 *  z_1b  = vis_fpadd32(z_1b, z_1b);
 *  dst   = vis_fpackfix_pair(z_0b, z_1b);
 *  inc
 */
/*
 *  external symbols:
 *     dx0 dx z_r0h z_r0l z_i0h z_r1 z_i1 z_0
 *
 *  internal symbols:
 *     dx1 z_i0l z_r1h z_i1h z_r1l z_i1l z_r0 z_i0
 */

/* *********************************************************** */

#define	SCALE_S16C_S16C_QUICK_START                                        \
	{                                                                  \
	    mlib_d64 dx1, z_i0l, z_r0, z_i0, z_r1h, z_i1h, z_r1l, z_i1l;   \
	                                                                   \
	    dx1 = vis_ld_d64_nf(dpx);                                      \
	    dx = vis_faligndata(dx0, dx1);                                 \
	    dx0 = dx1;                                                     \
	    dpx++;                                                         \
	    z_r0h = vis_fmuld8sux16(vis_read_hi(dx), fa_r);                \
	    z_r0l = vis_fmuld8ulx16(vis_read_hi(dx), fa_r);                \
	    z_i0h = vis_fmuld8sux16(vis_read_hi(dx), fa_i);                \
	    z_i0l = vis_fmuld8ulx16(vis_read_hi(dx), fa_i);                \
	    z_r1h = vis_fmuld8sux16(vis_read_lo(dx), fa_r);                \
	    z_r1l = vis_fmuld8ulx16(vis_read_lo(dx), fa_r);                \
	    z_i1h = vis_fmuld8sux16(vis_read_lo(dx), fa_i);                \
	    z_i1l = vis_fmuld8ulx16(vis_read_lo(dx), fa_i);                \
	    z_r0 = vis_fpadd32(z_r0h, z_r0l);                              \
	    z_i0 = vis_fpadd32(z_i0h, z_i0l);                              \
	    z_r1 = vis_fpadd32(z_r1h, z_r1l);                              \
	    z_i1 = vis_fpadd32(z_i1h, z_i1l);                              \
	    z_0 = vis_freg_pair(vis_fpsub32s(vis_read_hi(z_r0),            \
		    vis_read_lo(z_i0)), vis_fpadd32s(vis_read_lo(z_r0),    \
		    vis_read_hi(z_i0)));                                   \
	    dx1 = vis_ld_d64_nf(dpx);                                      \
	    dx = vis_faligndata(dx0, dx1);                                 \
	    dx0 = dx1;                                                     \
	    dpx++;                                                         \
	    z_r0h = vis_fmuld8sux16(vis_read_hi(dx), fa_r);                \
	    z_r0l = vis_fmuld8ulx16(vis_read_hi(dx), fa_r);                \
	    z_i0h = vis_fmuld8sux16(vis_read_hi(dx), fa_i);                \
	}

/* *********************************************************** */

#define	SCALE_S16C_S16C_QUICK_LOOP(dst, inc)                              \
	{                                                                 \
	    mlib_d64 dx1, z_i0l, z_r0, z_i0, ldx, z_r1h, z_i1h, z_1,      \
		    z_r1l, z_i1l, z_0b, z_1b;                             \
	    dx1 = vis_ld_d64_nf(dpx);                                     \
	    z_1 = vis_freg_pair(vis_fpsub32s(vis_read_hi(z_r1),           \
		    vis_read_lo(z_i1)), vis_fpadd32s(vis_read_lo(z_r1),   \
		    vis_read_hi(z_i1)));                                  \
	    z_i0l = vis_fmuld8ulx16(vis_read_hi(dx), fa_i);               \
	    z_r0 = vis_fpadd32(z_r0h, z_r0l);                             \
	    z_0b = vis_fpadd32(z_0, db);                                  \
	    z_r1h = vis_fmuld8sux16(vis_read_lo(dx), fa_r);               \
	    z_i0 = vis_fpadd32(z_i0h, z_i0l);                             \
	    z_r1l = vis_fmuld8ulx16(vis_read_lo(dx), fa_r);               \
	    ldx = vis_faligndata(dx0, dx1);                               \
	    z_i1h = vis_fmuld8sux16(vis_read_lo(dx), fa_i);               \
	    z_1b = vis_fpadd32(z_1, db);                                  \
	    z_r1 = vis_fpadd32(z_r1h, z_r1l);                             \
	    z_i1l = vis_fmuld8ulx16(vis_read_lo(dx), fa_i);               \
	    dx0 = dx1;                                                    \
	    dpx++;                                                        \
	    dx = ldx;                                                     \
	    z_i1 = vis_fpadd32(z_i1h, z_i1l);                             \
	    z_r0h = vis_fmuld8sux16(vis_read_hi(dx), fa_r);               \
	    dst = vis_fpackfix_pair(z_0b, z_1b);                          \
	    z_0 = vis_freg_pair(vis_fpsub32s(vis_read_hi(z_r0),           \
		    vis_read_lo(z_i0)), vis_fpadd32s(vis_read_lo(z_r0),   \
		    vis_read_hi(z_i0)));                                  \
	    z_r0l = vis_fmuld8ulx16(vis_read_hi(dx), fa_r);               \
	    inc z_i0h = vis_fmuld8sux16(vis_read_hi(dx), fa_i);           \
	}

/* *********************************************************** */

typedef union
{
	mlib_d64 db;
	struct
	{
		mlib_s32 int0, int1;
	} twoint;
	struct
	{
		mlib_f32 fl0, fl1;
	} twofloat;
	struct
	{
		mlib_s16 s0, s1, s2, s3;
	} fourshort;
} type_mlib_d64;

mlib_status
__mlib_VectorScale_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *a,
	const mlib_s16 *b,
	mlib_s32 n)
{
	mlib_s16 *px, *pz;
	type_mlib_d64 str1, str2, str3;
	mlib_d64 dz;

	if (n <= 0)
		return (MLIB_FAILURE);
	pz = z;
	px = (void *)x;

	if ((abs(a[0]) + abs(a[1]) < MLIB_S16_MAX) && !((mlib_addr)z & 3)) {
		mlib_s16 *pzend;
		mlib_d64 *dpzend, *dpz;
		mlib_s32 off;
		mlib_d64 *dpx;
		mlib_f32 fa_r = vis_to_float((a[0] << 16) | (a[0] & 0xFFFF));
		mlib_f32 fa_i = vis_to_float((a[1] << 16) | (a[1] & 0xFFFF));
		mlib_d64 db = vis_to_double(b[0], b[1]);
		mlib_d64 dx0, dx, z_r0h, z_r0l, z_i0h, z_r1, z_i1, z_0;
		mlib_s32 mask;
		mlib_d64 dx1, z_i0l, z_r0, z_i0, z_r1h, z_i1h, z_r1l, z_i1l,
			z_1, z_0b, z_1b;

		vis_write_gsr(16 << 3);
		pzend = (mlib_s16 *)((mlib_addr)(pz + n + n) - 1);
		dpz = (mlib_d64 *)((mlib_addr)pz & (~7));
		dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
		off = (mlib_addr)dpz - (mlib_addr)pz;
		dpx = vis_alignaddr(px, off);
		dx0 = vis_ld_d64_nf(dpx);
		dpx++;

		mask = vis_edge16(pz, pzend);
		dx1 = vis_ld_d64_nf(dpx);
		dx = vis_faligndata(dx0, dx1);
		dx0 = dx1;
		dpx++;
		z_r0h = vis_fmuld8sux16(vis_read_hi(dx), fa_r);
		z_r0l = vis_fmuld8ulx16(vis_read_hi(dx), fa_r);
		z_i0h = vis_fmuld8sux16(vis_read_hi(dx), fa_i);
		z_i0l = vis_fmuld8ulx16(vis_read_hi(dx), fa_i);
		z_r1h = vis_fmuld8sux16(vis_read_lo(dx), fa_r);
		z_r1l = vis_fmuld8ulx16(vis_read_lo(dx), fa_r);
		z_i1h = vis_fmuld8sux16(vis_read_lo(dx), fa_i);
		z_i1l = vis_fmuld8ulx16(vis_read_lo(dx), fa_i);
		z_r0 = vis_fpadd32(z_r0h, z_r0l);
		z_i0 = vis_fpadd32(z_i0h, z_i0l);
		z_r1 = vis_fpadd32(z_r1h, z_r1l);
		z_i1 = vis_fpadd32(z_i1h, z_i1l);
		z_0 = vis_freg_pair(vis_fpsub32s(vis_read_hi(z_r0),
			vis_read_lo(z_i0)), vis_fpadd32s(vis_read_lo(z_r0),
			vis_read_hi(z_i0)));
		z_1 = vis_freg_pair(vis_fpsub32s(vis_read_hi(z_r1),
			vis_read_lo(z_i1)), vis_fpadd32s(vis_read_lo(z_r1),
			vis_read_hi(z_i1)));
		z_0b = vis_fpadd32(z_0, db);
		z_1b = vis_fpadd32(z_1, db);
		dz = vis_fpackfix_pair(z_0b, z_1b);
		vis_pst_16(dz, dpz, mask);
		dpz++;

#pragma pipeloop(0)
		for (; dpz < dpzend; ) {
			mlib_d64 dx1, z_i0l, z_r0, z_i0, z_r1h, z_i1h, z_1,
				z_r1l, z_i1l, z_0b, z_1b;
			dx1 = vis_ld_d64_nf(dpx); dpx++;
			dx = vis_faligndata(dx0, dx1);
			dx0 = dx1;
			z_r0h = vis_fmuld8sux16(vis_read_hi(dx), fa_r);
			z_r0l = vis_fmuld8ulx16(vis_read_hi(dx), fa_r);
			z_r0 = vis_fpadd32(z_r0h, z_r0l);
			z_i0h = vis_fmuld8sux16(vis_read_hi(dx), fa_i);
			z_i0l = vis_fmuld8ulx16(vis_read_hi(dx), fa_i);
			z_i0 = vis_fpadd32(z_i0h, z_i0l);
			z_r1h = vis_fmuld8sux16(vis_read_lo(dx), fa_r);
			z_r1l = vis_fmuld8ulx16(vis_read_lo(dx), fa_r);
			z_r1 = vis_fpadd32(z_r1h, z_r1l);
			z_i1h = vis_fmuld8sux16(vis_read_lo(dx), fa_i);
			z_i1l = vis_fmuld8ulx16(vis_read_lo(dx), fa_i);
			z_i1 = vis_fpadd32(z_i1h, z_i1l);
			z_0 = vis_freg_pair(vis_fpsub32s(vis_read_hi(z_r0),
				vis_read_lo(z_i0)),
				vis_fpadd32s(vis_read_lo(z_r0),
				vis_read_hi(z_i0)));
			z_1 = vis_freg_pair(vis_fpsub32s(vis_read_hi(z_r1),
				vis_read_lo(z_i1)),
				vis_fpadd32s(vis_read_lo(z_r1),
				vis_read_hi(z_i1)));
			z_0b = vis_fpadd32(z_0, db);
			z_1b = vis_fpadd32(z_1, db);
			(*dpz++) = vis_fpackfix_pair(z_0b, z_1b);
		}

		if (dpz <= (mlib_d64 *)pzend) {
			mlib_s32 mask = vis_edge16(dpz, pzend);
			mlib_d64 dz;
			mlib_d64 dx1, z_i0l, z_r0, z_i0, z_r1h, z_i1h, z_1,
				z_r1l, z_i1l, z_0b, z_1b;
			dx1 = vis_ld_d64_nf(dpx); dpx++;
			dx = vis_faligndata(dx0, dx1);
			dx0 = dx1;
			z_r0h = vis_fmuld8sux16(vis_read_hi(dx), fa_r);
			z_r0l = vis_fmuld8ulx16(vis_read_hi(dx), fa_r);
			z_i0h = vis_fmuld8sux16(vis_read_hi(dx), fa_i);
			z_i0l = vis_fmuld8ulx16(vis_read_hi(dx), fa_i);
			z_r1h = vis_fmuld8sux16(vis_read_lo(dx), fa_r);
			z_r1l = vis_fmuld8ulx16(vis_read_lo(dx), fa_r);
			z_i1h = vis_fmuld8sux16(vis_read_lo(dx), fa_i);
			z_i1l = vis_fmuld8ulx16(vis_read_lo(dx), fa_i);
			z_r0 = vis_fpadd32(z_r0h, z_r0l);
			z_i0 = vis_fpadd32(z_i0h, z_i0l);
			z_r1 = vis_fpadd32(z_r1h, z_r1l);
			z_i1 = vis_fpadd32(z_i1h, z_i1l);
			z_0 = vis_freg_pair(vis_fpsub32s(vis_read_hi(z_r0),
				vis_read_lo(z_i0)),
				vis_fpadd32s(vis_read_lo(z_r0),
				vis_read_hi(z_i0)));
			z_1 = vis_freg_pair(vis_fpsub32s(vis_read_hi(z_r1),
				vis_read_lo(z_i1)),
				vis_fpadd32s(vis_read_lo(z_r1),
				vis_read_hi(z_i1)));
			z_0b = vis_fpadd32(z_0, db);
			z_1b = vis_fpadd32(z_1, db);
			dz = vis_fpackfix_pair(z_0b, z_1b);
			vis_pst_16(dz, dpz, mask);
		}

	} else {
		mlib_s32 i;
		mlib_s16 iar = a[0], iai = a[1], ibr = b[0], ibi = b[1];
		mlib_d64 scalr, scalr1;
		mlib_d64 scali, scali1;
		mlib_d64 dxr, dxr1, dxi, dxi1, *dpz;

		vis_write_gsr(16 << 3);

		if (!((mlib_addr)pz & 3)) {

			if (((mlib_addr)pz & 7)) {
				dxr = (*px++);
				dxi = (*px++);
				scalr = dxr * iar - dxi * iai + ibr;
				scali = dxr * iai + dxi * iar + ibi;
				str1.twoint.int0 = (mlib_s32)scalr;
				str1.twoint.int1 = (mlib_s32)scali;
				str2.twofloat.fl0 = vis_fpackfix(str1.db);
				(*pz++) = str2.fourshort.s0;
				(*pz++) = str2.fourshort.s1;
				n--;
			}

			dpz = (mlib_d64 *)pz;
#pragma pipeloop(0)
			for (i = 0; i < n - 1; i += 2) {
				dxr = px[2 * i];
				dxi = px[2 * i + 1];
				dxr1 = px[2 * i + 2];
				dxi1 = px[2 * i + 3];
				scalr = dxr * iar - dxi * iai + ibr;
				scali = dxr * iai + dxi * iar + ibi;
				scalr1 = dxr1 * iar - dxi1 * iai + ibr;
				scali1 = dxr1 * iai + dxi1 * iar + ibi;

				str1.twoint.int0 = (mlib_s32)scalr;
				str1.twoint.int1 = (mlib_s32)scali;
				str2.twoint.int0 = (mlib_s32)scalr1;
				str2.twoint.int1 = (mlib_s32)scali1;
				(*dpz++) = vis_fpackfix_pair(str1.db, str2.db);
			}

			pz = (mlib_s16 *)dpz;
			px += 2 * i;

			if (i < n) {
				dxr = (*px++);
				dxi = (*px++);
				scalr = dxr * iar - dxi * iai + ibr;
				scali = dxr * iai + dxi * iar + ibi;
				str1.twoint.int0 = (mlib_s32)scalr;
				str1.twoint.int1 = (mlib_s32)scali;
				str2.twofloat.fl0 = vis_fpackfix(str1.db);
				(*pz++) = str2.fourshort.s0;
				(*pz++) = str2.fourshort.s1;
			}

		} else {
/* ((mlib_addr) pz & 3) */

			if ((((mlib_addr)pz & 7) == 6) || (n == 1)) {
				dxr = (*px++);
				dxi = (*px++);
				scalr = dxr * iar - dxi * iai + ibr;
				scali = dxr * iai + dxi * iar + ibi;
				str1.twoint.int0 = (mlib_s32)scalr;
				str1.twoint.int1 = (mlib_s32)scali;
				str2.twofloat.fl1 = vis_fpackfix(str1.db);
				(*pz++) = str2.fourshort.s2;
				n--;
			} else {
				dxr = (*px++);
				dxi = (*px++);
				scalr = dxr * iar - dxi * iai + ibr;
				scali = dxr * iai + dxi * iar + ibi;
				str1.twoint.int0 = (mlib_s32)scalr;
				str1.twoint.int1 = (mlib_s32)scali;
				str2.twofloat.fl0 = vis_fpackfix(str1.db);
				(*pz++) = str2.fourshort.s0;
				(*pz++) = str2.fourshort.s1;
				dxr = (*px++);
				dxi = (*px++);
				scalr = dxr * iar - dxi * iai + ibr;
				scali = dxr * iai + dxi * iar + ibi;
				str1.twoint.int0 = (mlib_s32)scalr;
				str1.twoint.int1 = (mlib_s32)scali;
				str2.twofloat.fl1 = vis_fpackfix(str1.db);
				(*pz++) = str2.fourshort.s2;
				n -= 2;
			}

			dpz = (mlib_d64 *)pz;
			vis_alignaddr((void *)6, 0);
#pragma pipeloop(0)
			for (i = 0; i < n - 1; i += 2) {
				dxr = px[2 * i];
				dxi = px[2 * i + 1];
				dxr1 = px[2 * i + 2];
				dxi1 = px[2 * i + 3];
				scalr = dxr * iar - dxi * iai + ibr;
				scali = dxr * iai + dxi * iar + ibi;
				scalr1 = dxr1 * iar - dxi1 * iai + ibr;
				scali1 = dxr1 * iai + dxi1 * iar + ibi;

				str1.twoint.int0 = (mlib_s32)scalr;
				str1.twoint.int1 = (mlib_s32)scali;
				str3.twoint.int0 = (mlib_s32)scalr1;
				str3.twoint.int1 = (mlib_s32)scali1;
				dz = vis_fpackfix_pair(str1.db, str3.db);
				(*dpz++) = vis_faligndata(str2.db, dz);
				str2.db = dz;
			}

			pz = (mlib_s16 *)dpz;
			px += 2 * i;

			if (i < n) {
				(*pz++) = str2.fourshort.s3;
				dxr = (*px++);
				dxi = (*px++);
				scalr = dxr * iar - dxi * iai + ibr;
				scali = dxr * iai + dxi * iar + ibi;
				str1.twoint.int0 = (mlib_s32)scalr;
				str1.twoint.int1 = (mlib_s32)scali;
				str2.twofloat.fl1 = vis_fpackfix(str1.db);
				(*pz++) = str2.fourshort.s2;
			}

			*pz = str2.fourshort.s3;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SCALE_S16C_MOD_1                                        \
	dr5 = vis_fmuld8sux16(vis_read_hi(dx), fa_r);           \
	dr6 = vis_fmuld8ulx16(vis_read_hi(dx), fa_r);           \
	dr7 = vis_fmuld8sux16(vis_read_hi(dx), fa_i);           \
	dr8 = vis_fmuld8ulx16(vis_read_hi(dx), fa_i);           \
	dr1 = vis_fpadd32(dr5, dr6);                            \
	dr2 = vis_fpadd32(dr7, dr8);                            \
	dr5 = vis_fmuld8sux16(vis_read_lo(dx), fa_r);           \
	dr6 = vis_fmuld8ulx16(vis_read_lo(dx), fa_r);           \
	dr7 = vis_fmuld8sux16(vis_read_lo(dx), fa_i);           \
	dr8 = vis_fmuld8ulx16(vis_read_lo(dx), fa_i);           \
	dr3 = vis_fpadd32(dr5, dr6);                            \
	dr4 = vis_fpadd32(dr7, dr8)

/* *********************************************************** */

#define	SCALE_S16C_MOD_2                                                  \
	dr5 =                                                             \
	vis_freg_pair(vis_fpsub32s(vis_read_hi(dr1), vis_read_lo(dr1)),   \
		vis_fpsub32s(vis_read_hi(dr3), vis_read_lo(dr3)));        \
	dr5 = vis_fpack32(dr5, dzero);                                    \
	dr5 = vis_fpack32(dr5, dzero);                                    \
	dr6 =                                                             \
	vis_fand(without_control,                                         \
		vis_freg_pair(vis_fpadd32s(vis_read_hi(dr2),              \
		vis_read_lo(dr2)), vis_fpadd32s(vis_read_hi(dr4),         \
		vis_read_lo(dr4))));                                      \
	dr5 = vis_for(dr5, dr6);                                          \
	dr = vis_fpadd16(dr5, db)

/* *********************************************************** */

#define	SCALE_S16C_MOD_2_BSH                                              \
	dr5 =                                                             \
	vis_freg_pair(vis_fpsub32s(vis_read_hi(dr1), vis_read_lo(dr1)),   \
		vis_fpsub32s(vis_read_hi(dr3), vis_read_lo(dr3)));        \
	dr7 =                                                             \
	vis_freg_pair(vis_fpadd32s(vis_read_hi(dr2), vis_read_lo(dr2)),   \
		vis_fpadd32s(vis_read_hi(dr4), vis_read_lo(dr4)));        \
	dr5 = vis_bshuffle(dr5, dr7);                                     \
	dr = vis_fpadd16(dr5, db)

/* *********************************************************** */

#define	SCALE_S16C_MOD_2_ODD                                              \
	dr5 =                                                             \
	vis_freg_pair(vis_fpadd32s(vis_read_hi(dr4), vis_read_lo(dr4)),   \
		vis_fpadd32s(vis_read_hi(dr2), vis_read_lo(dr2)));        \
	dr7 =                                                             \
	vis_freg_pair(vis_fpsub32s(vis_read_hi(dr1), vis_read_lo(dr1)),   \
		vis_fpsub32s(vis_read_hi(dr3), vis_read_lo(dr3)));        \
	dr5 = vis_bshuffle(dr5, dr7);                                     \
	dr = vis_fpadd16(dr5, db)

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *a,
	const mlib_s16 *b,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s16 *pzend = (mlib_s16 *)z + n + n - 1;
	mlib_u16 ua_r = ((mlib_u16 *)a)[0];
	mlib_u16 ua_i = ((mlib_u16 *)a)[1];
	mlib_u16 ub_r = ((mlib_u16 *)b)[0];
	mlib_u16 ub_i = ((mlib_u16 *)b)[1];
	mlib_d64 *dpx, *dpz, *dpzend;
	mlib_d64 dx, dx0, dx1, dr, dr1, dr2, dr3, dr4, dr5, dr6, dr7, dr8;

	mlib_d64 db = vis_to_double_dup((ub_r << 16) | ub_i);
	mlib_d64 without_control = vis_to_double_dup(0xffff);
	mlib_d64 dzero = vis_fzero();
	mlib_f32 fa_r = vis_to_float((ua_r << 16) | ua_i);
	mlib_f32 fa_i = vis_to_float((ua_i << 16) | ua_r);

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

/*
 * generate edge mask for the start bytes
 */
	emask = vis_edge16(z, pzend);
	dx = vis_faligndata(dx0, dx1);
	SCALE_S16C_MOD_1;
	SCALE_S16C_MOD_2;

	vis_write_bmask(0x23ab67ef, 0);

	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx + 2);
	dx = vis_faligndata(dx0, dx1);

	if (!(2 & (mlib_addr)z)) {
/* store first bytes of result */
		vis_pst_16(dr, dpz, emask);
		dpz++;
	} else {
		(void *)vis_alignaddr(dpx, 6);
		dr = vis_faligndata(dr, dr);
/* store first bytes of result */
		vis_pst_16(dr, dpz, emask);

		if ((mlib_addr)++dpz <= (mlib_addr)pzend)
			vis_pst_16(dr, dpz, 0x8);
		(void *)vis_alignaddr((void *)x, off);
	}

	SCALE_S16C_MOD_1;
	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx + 3);
	dx = vis_faligndata(dx0, dx1);

	if (!(2 & (mlib_addr)z)) {
		if ((((mlib_addr)x ^ (mlib_addr)z) & 7) == 0) {

#pragma pipeloop(0)
			for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
				SCALE_S16C_MOD_2_BSH;
				SCALE_S16C_MOD_1;
				dx = vis_ld_d64_nf(dpx + 3);
/* store 8 bytes of result */
				dpz[0] = dr;
				dpx++;
				dpz++;
			}

		} else {

#pragma pipeloop(0)
			for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dpx + 4);
				SCALE_S16C_MOD_2_BSH;
				SCALE_S16C_MOD_1;
/* store 8 bytes of result */
				dpz[0] = dr;
				dx = vis_faligndata(dx0, dx1);
				dpx++;
				dpz++;
			}
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			SCALE_S16C_MOD_2_BSH;
/* prepare edge mask for the last bytes */
			emask = vis_edge16(dpz, pzend);
/* store last bytes of result */
			vis_pst_16(dr, dpz, emask);
		}

	} else {
		db = vis_to_double_dup((ub_i << 16) | ub_r);

#pragma pipeloop(0)
		for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx + 4);
			SCALE_S16C_MOD_2_ODD;
			SCALE_S16C_MOD_1;
/* store 6 bytes of result */
			vis_pst_16(dr, dpz, 0x7);
			dpz++;
/* store 2 bytes of result to the next word */
			vis_pst_16(dr, dpz, 0x8);
			dx = vis_faligndata(dx0, dx1);
			dpx++;
		}

		if ((mlib_addr)dpz < (mlib_addr)pzend) {
			SCALE_S16C_MOD_2_ODD;
/* store 4 bytes of result */
			vis_pst_16(dr, dpz, 6);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *a,
	const mlib_s16 *b,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx;
	mlib_d64 dx, dx0, dx1;
	mlib_d64 dr, dr1, dr2, dr3, dr4, dr5, dzh, dzl;
	mlib_s16 *px = (mlib_s16 *)x;
	mlib_s32 *pz = (mlib_s32 *)z;
/*
 * odd = 1 means 8-byte aligned resultant
 * vector starts with Im. part.
 */
	mlib_s32 odd = 0;
	mlib_s32 len = n << 1, i;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_f32 fzero = vis_fzeros();
	mlib_f32 sr0, sr1, sr2, sr3;
	mlib_f32 rest;

/*
 * prepare the scaling factor
 */
	mlib_s16 ua0 = ((mlib_s16 *)a)[0], ua1 = ((mlib_s16 *)a)[1];
	mlib_u16 uc0 = ((mlib_u16 *)a)[0], uc1 = ((mlib_u16 *)a)[1];
	mlib_s16 ub0 = ((mlib_s16 *)b)[0], ub1 = ((mlib_s16 *)b)[1];
	mlib_f32 fc0 = vis_to_float((uc0 << 16) | uc1);
	mlib_f32 fc1 = vis_to_float((uc1 << 16) | uc0);
	mlib_d64 db0 = vis_to_double(ub0, ub1), db1 = vis_to_double(ub1, ub0);
	mlib_d64 drest;
	mlib_s32 re_x, im_x, re_c, im_c;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!(ua0 | ua1 | ub0 | ub1))

		return (__mlib_VectorZero_S32C(z, n));
	else if (!(ua0 | ua1)) {
		mlib_s32 tmp_arr[2];

		tmp_arr[0] = ub0;
		tmp_arr[1] = ub1;

		return (__mlib_VectorSet_S32C(z, tmp_arr, n));
	} else if (!(ub0 | ub1))

		return (__mlib_VectorMulS_S32C_S16C_Mod(z, x, a, n));

	if (n <= 4)
		SCALE_S32C_S16C_SAT_IN_C;

/*
 * prepare the destination address trying to align it for 8 bytes.
 */

	if ((mlib_addr)pz & 7) {
		re_x = (*px++);
		im_x = (*px++);
		re_c = re_x * ua0 - im_x * ua1 + ub0;
		im_c = im_x * ua0 + re_x * ua1;
		len -= 2;
		(*pz++) = re_c;
		odd = 1;
		rest = vis_to_float(im_c);
	}

	dpz = (mlib_d64 *)pz;
	even_8 = len >> 2;
	rest_8 = len & 3;
	pz += even_8 << 2;

	if (!((mlib_addr)px & 7)) {

/*
 * 'x' address is 8-byte aligned.
 * No  vis_alignaddr and  vis_faligndata at all.
 */

		dpx = (mlib_d64 *)px;

		if (!odd) {
/*
 * Start with real part.
 */
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = dpx[0];
				SCALE_S16C_S32C;
				COMPOSE_S32C_FROM32_MOD;
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
				dpx++;
			}

		} else {
/*
 * Start with imaginary part.
 */
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = dpx[0];
				SCALE_S16C_S32C;
				COMPOSE_S32C_FROM32_MOD_WITH_ODD;
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
				dpx++;
			}

/*
 * Storing of the last imag. part which was in 'rest'.
 */
			drest = vis_freg_pair(rest, fzero);
			drest = vis_fpadd32(drest, db1);
			vis_pst_32(drest, pz, 0x2);
			pz++;
		}

	} else {

/*
 * "x" address is arbitrary aligned.
 * 1 vis_alignaddr(s) and 1 vis_faligndata(s) in the loop.
 */

		dpx = vis_alignaddr(px, 0);
		dx0 = (*dpx++);

		if (odd) {
/*
 * Start with imaginary part.
 */
/*
 * Loop unrolling for 2 iterations.
 */
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
				dx = vis_faligndata(dx0, dx1);
				SCALE_S16C_S32C;
				COMPOSE_S32C_FROM32_MOD_WITH_ODD;
				dx0 = dx1;
/*
 * store 16 bytes of result
 */
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
				dpx++;
			}

/*
 * Storing of the last imag. part which was in 'rest'.
 */
			drest = vis_freg_pair(rest, fzero);
			drest = vis_fpadd32(drest, db1);
			vis_pst_32(drest, pz, 0x2);
			pz++;
		} else {
/*
 * Start with real part.
 */
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
				dx = vis_faligndata(dx0, dx1);
				SCALE_S16C_S32C;
				COMPOSE_S32C_FROM32_MOD;
				dx0 = dx1;
/*
 * store 16 bytes of result
 */
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
				dpx++;
			}
		}
	}

	if (rest_8) {
		px += (even_8 << 2);
		re_x = px[0];
		im_x = px[1];
		re_c = re_x * ua0 - im_x * ua1 + ub0;
		im_c = im_x * ua0 + re_x * ua1 + ub1;
		pz[0] = re_c;
		pz[1] = im_c;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S32C_S16C_Sat(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *a,
	const mlib_s16 *b,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx;
	mlib_d64 dx, dx0, dx1;
	mlib_d64 dr, dr1, dr2, dr3, dr4, dr5, dzh, dzl;
	mlib_s16 *px = (mlib_s16 *)x;
	mlib_s32 *pz = (mlib_s32 *)z;

/* odd = 1 means 8-byte aligned resultant vector starts with Im. part. */
	mlib_s32 odd = 0;
	mlib_s32 len = n << 1, i;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_f32 fzero = vis_fzeros();
	mlib_f32 sr0, sr1, sr2, sr3;
	mlib_f32 rest;

/*
 * prepare the scaling factor
 */
	mlib_s16 ua0 = ((mlib_s16 *)a)[0], ua1 = ((mlib_s16 *)a)[1];
	mlib_u16 uc0 = ((mlib_u16 *)a)[0], uc1 = ((mlib_u16 *)a)[1];
	mlib_s16 ub0 = ((mlib_s16 *)b)[0], ub1 = ((mlib_s16 *)b)[1];
	mlib_f32 fc0 = vis_to_float((uc0 << 16) | uc1);
	mlib_f32 fc1 = vis_to_float((uc1 << 16) | uc0);
	mlib_d64 db0 = vis_to_double(ub0, ub1), db1 = vis_to_double(ub1, ub0);
	mlib_d64 drest;
	mlib_d64 cntr_ovlp = vis_to_double(MLIB_S32_MIN, 0x80008000);
	mlib_d64 cntr_ovlp1 = vis_to_double(0x80008000, MLIB_S32_MIN);
	mlib_d64 const_max_32 = vis_to_double_dup(MLIB_S32_MAX);
	mlib_s32 mask, mask1;
	mlib_d64 re_x, im_x, re_c, im_c;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!(ua0 | ua1 | ub0 | ub1))

		return (__mlib_VectorZero_S32C(z, n));
	else if (!(ua0 | ua1)) {
		mlib_s32 tmp_arr[2];

		tmp_arr[0] = ub0;
		tmp_arr[1] = ub1;

		return (__mlib_VectorSet_S32C(z, tmp_arr, n));
	} else if (!(ub0 | ub1))

		return (__mlib_VectorMulS_S32C_S16C_Sat(z, x, a, n));
	else if (!(ua0 == MLIB_S16_MIN && ua1 == MLIB_S16_MIN))

		return (__mlib_VectorScale_S32C_S16C_Mod(z, x, a, b, n));

	if (n <= 4)
		SCALE_S32C_S16C_SAT_IN_C;

/*
 * prepare the destination address trying to align it for 8 bytes.
 */

	if ((mlib_addr)pz & 7) {
		re_x = (*px++);
		im_x = (*px++);
		re_c = re_x * ua0 - im_x * ua1 + ub0;
		im_c = im_x * ua0 + re_x * ua1 - 1.0;
		len -= 2;
		(*pz++) = (mlib_s32)re_c;
		odd = 1;
		rest = vis_to_float(1 + (mlib_s32)im_c);
	}

	dpz = (mlib_d64 *)pz;
	even_8 = len >> 2;
	rest_8 = len & 3;
	pz += even_8 << 2;

	if (!((mlib_addr)px & 7)) {

/*
 * 'x' address is 8-byte aligned.
 * No  vis_alignaddr and  vis_faligndata at all.
 */

		dpx = (mlib_d64 *)px;
		dx = dpx[0];
		dpx++;

		if (!odd) {
/*
 * Start with real part.
 */
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
				SCALE_S16C_S32C;
				COMPOSE_S32C_FROM32_SAT;
				dx = dx1;
				dpz[0] = dzh;
				dpz[1] = dzl;
				vis_pst_32(const_max_32, dpz, mask);
				vis_pst_32(const_max_32, dpz + 1, mask1);
				dpz += 2;
				dpx++;
			}
		} else {
/*
 * Start with imaginary part.
 */
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
				SCALE_S16C_S32C;
				COMPOSE_S32C_FROM32_SAT_WITH_ODD;
				dx = dx1;
				dpz[0] = dzh;
				dpz[1] = dzl;
				vis_pst_32(const_max_32, dpz, mask);
				vis_pst_32(const_max_32, dpz + 1, mask1);
				dpz += 2;
				dpx++;
			}

/*
 * Storing of the last imag. part which was in 'rest'.
 */
			drest = vis_freg_pair(rest, fzero);
			drest = vis_fpadd32(drest, db1);
			mask = vis_fcmpgt32(cntr_ovlp1, drest);
			vis_pst_32(drest, pz, 0x2);
			vis_pst_32(const_max_32, pz, mask);
			pz++;
		}

	} else {

/*
 * "x" address is arbitrary aligned.
 * 1 vis_alignaddr(s) and 1 vis_faligndata(s) in the loop.
 */

		dpx = vis_alignaddr(px, 0);
		dx0 = (*dpx++);
		dx1 = vis_ld_d64_nf(dpx);
		dx = vis_faligndata(dx0, dx1);

		if (odd) {
/*
 * Start with imaginary part.
 */
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				SCALE_S16C_S32C;
				COMPOSE_S32C_FROM32_SAT_WITH_ODD;
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dpx + 1);
				dx = vis_faligndata(dx0, dx1);
/*
 * store 16 bytes of result
 */
				dpz[0] = dzh;
				dpz[1] = dzl;
				vis_pst_32(const_max_32, dpz, mask);
				vis_pst_32(const_max_32, dpz + 1, mask1);
				dpz += 2;
				dpx++;
			}

/*
 * Storing of the last imag. part which was in 'rest'.
 */
			drest = vis_freg_pair(rest, fzero);
			drest = vis_fpadd32(drest, db1);
			mask = vis_fcmpgt32(cntr_ovlp1, drest);
			vis_pst_32(drest, pz, 0x2);
			vis_pst_32(const_max_32, pz, mask);
			pz++;
		} else {
/*
 * Start with real part.
 */
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				SCALE_S16C_S32C;
				COMPOSE_S32C_FROM32_SAT;
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dpx + 1);
				dx = vis_faligndata(dx0, dx1);
/*
 * store 16 bytes of result
 */
				dpz[0] = dzh;
				dpz[1] = dzl;
				vis_pst_32(const_max_32, dpz, mask);
				vis_pst_32(const_max_32, dpz + 1, mask1);
				dpz += 2;
				dpx++;
			}
		}
	}

	if (rest_8) {
		px += (even_8 << 2);
		re_x = px[0];
		im_x = px[1];
		re_c = re_x * ua0 - im_x * ua1 + ub0;
		im_c = im_x * ua0 + re_x * ua1 + ub1;
		pz[0] = re_c;
		pz[1] = im_c;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S32C_S32C_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *a,
	const mlib_s32 *b,
	mlib_s32 n)
{
#define	MAX	0x1fffff
	mlib_s32 i;
	mlib_s32 *px = (mlib_s32 *)x;
	mlib_s32 *pz = (mlib_s32 *)z;
	mlib_s32 val_b_r = b[0];
	mlib_s32 val_b_i = b[1];

	mlib_d64 val_a_r = a[0], val_a_i = a[1], scal_r, scal_i;

	if ((abs(a[0]) < MAX) && (abs(a[1]) < MAX))
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			scal_r = px[2 * i] * val_a_r - px[2 * i + 1] * val_a_i +
				val_b_r;
			scal_i = px[2 * i + 1] * val_a_r + px[2 * i] * val_a_i +
				val_b_i;
			FLOAT2INT_CLAMP(pz[2 * i], scal_r);
			FLOAT2INT_CLAMP(pz[2 * i + 1], scal_i);
	} else {
		mlib_d64 val_a_r_lo = a[0] & MAX, val_a_i_lo = a[1] & MAX;

		val_a_r -= val_a_r_lo;
		val_a_i -= val_a_i_lo;
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			scal_r = px[2 * i] * val_a_r - px[2 * i + 1] * val_a_i +
				(px[2 * i] * val_a_r_lo - px[2 * i +
				1] * val_a_i_lo + val_b_r);
			scal_i = px[2 * i + 1] * val_a_r + px[2 * i] * val_a_i +
				(px[2 * i + 1] * val_a_r_lo +
				px[2 * i] * val_a_i_lo + val_b_i);
			FLOAT2INT_CLAMP(pz[2 * i], scal_r);
			FLOAT2INT_CLAMP(pz[2 * i + 1], scal_i);
		}
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S32C_S32C_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *a,
	const mlib_s32 *b,
	mlib_s32 n)
{
	mlib_s32 i, tmp;

	mlib_d64 d16 = 0x10000, _d16 = 1 / d16;
	mlib_d64 d32 = d16 * d16;
	mlib_d64 scalrh, scalrl;
	mlib_d64 scalih, scalil;
	mlib_d64 darh = ((mlib_s16 *)a)[0];
	mlib_d64 darl = ((mlib_u16 *)a)[1];
	mlib_d64 daih = ((mlib_s16 *)a)[2];
	mlib_d64 dail = ((mlib_u16 *)a)[3];
	mlib_d64 dbr = b[0], dbi = b[1];
	mlib_d64 dxr, dxi;
	mlib_s32 *px = (void *)x, *pz = z;

	dbr += (mlib_d64)(1 << 26) * (mlib_d64)(3 << 25);
	dbi += (mlib_d64)(1 << 26) * (mlib_d64)(3 << 25);

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		union
		{
			mlib_d64 d;
			struct
			{
				mlib_s32 f0, f1;
			} f;
		} resr,
		    resi;

		dxr = px[i * 2];
		dxi = px[i * 2 + 1];

		scalrh = dxr * darh - dxi * daih;
		scalrl = dxr * darl - dxi * dail + dbr;
		scalih = dxr * daih + dxi * darh;
		scalil = dxr * dail + dxi * darl + dbi;

		FLOAT2INT_CLAMP(tmp, scalrh * _d16);
		scalrh = scalrh * d16 - tmp * d32;
		resr.d = scalrh + scalrl;
		pz[i * 2] = resr.f.f1;

		FLOAT2INT_CLAMP(tmp, scalih * _d16);
		scalih = scalih * d16 - tmp * d32;
		resi.d = scalih + scalil;
		pz[i * 2 + 1] = resi.f.f1;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_U8C_Sat(
	mlib_u8 *xz,
	const mlib_u8 *a,
	const mlib_u8 *b,
	mlib_s32 n)
{
	return (__mlib_VectorScale_U8C_U8C_Sat(xz, xz, a, b, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_U8C_Mod(
	mlib_u8 *xz,
	const mlib_u8 *a,
	const mlib_u8 *b,
	mlib_s32 n)
{
	return (__mlib_VectorScale_U8C_U8C_Mod(xz, xz, a, b, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S8C_Sat(
	mlib_s8 *xz,
	const mlib_s8 *a,
	const mlib_s8 *b,
	mlib_s32 n)
{
	return (__mlib_VectorScale_S8C_S8C_Sat(xz, xz, a, b, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S16C_Sat(
	mlib_s16 *xz,
	const mlib_s16 *a,
	const mlib_s16 *b,
	mlib_s32 n)
{
	return (__mlib_VectorScale_S16C_S16C_Sat(xz, xz, a, b, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S16C_Mod(
	mlib_s16 *xz,
	const mlib_s16 *a,
	const mlib_s16 *b,
	mlib_s32 n)
{
	return (__mlib_VectorScale_S16C_S16C_Mod(xz, xz, a, b, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S32C_Sat(
	mlib_s32 *xz,
	const mlib_s32 *a,
	const mlib_s32 *b,
	mlib_s32 n)
{
	return (__mlib_VectorScale_S32C_S32C_Sat(xz, xz, a, b, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S32C_Mod(
	mlib_s32 *xz,
	const mlib_s32 *a,
	const mlib_s32 *b,
	mlib_s32 n)
{
	return (__mlib_VectorScale_S32C_S32C_Mod(xz, xz, a, b, n));
}

/* *********************************************************** */
