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

#pragma ident	"@(#)mlib_v_VectorMulS.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorMulS_U8_[Sat|Mod],
 *      mlib_VectorMulS_[U8|S16]_U8_[Sat|Mod] - multiplication of
 *                     unsigned 8-bit format vector to a scalar
 *      mlib_VectorMulS_S8_[Sat|Mod],
 *      mlib_VectorMulS_[S8|S16]_S8_[Sat|Mod] - multiplication of
 *                     signed 8-bit format vector to a scalar
 *      mlib_VectorMulS_S16_[Sat|Mod],
 *      mlib_VectorMulS_[S16|S32]_S16_[Sat|Mod] - multiplication of
 *                     signed 16-bit format vector to a scalar
 *      mlib_VectorMulS_S32_[Sat|Mod],
 *      mlib_VectorMulS_S32_S32_[Sat|Mod] - multiplication of
 *                     signed 32-bit format vector to a scalar
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMulS_S16_[Sat|Mod](mlib_s16       *xz,
 *                                                const mlib_s16 *c,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorMulS_S16_S16_[Sat|Mod](mlib_s16       *z,
 *                                                    const mlib_s16 *x,
 *                                                    const mlib_s16 *c,
 *                                                    mlib_s32       n);
 *      mlib_status mlib_VectorMulS_S16_U8_[Sat|Mod](mlib_s16      *z,
 *                                                   const mlib_u8 *x,
 *                                                   const mlib_u8 *pc,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_VectorMulS_S32_[Sat|Mod](mlib_s32       *xz,
 *                                                const mlib_s32 *c,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorMulS_S32_S32_[Sat|Mod](mlib_s32       *z,
 *                                                    const mlib_s32 *x,
 *                                                    const mlib_s32 *c,
 *                                                    mlib_s32       n);
 *      mlib_status mlib_VectorMulS_S8_[Sat|Mod](mlib_s8       *xz,
 *                                               const mlib_s8 *c,
 *                                               mlib_s32      n);
 *      mlib_status mlib_VectorMulS_S8_S8_[Sat|Mod](mlib_s8       *z,
 *                                                  const mlib_s8 *x,
 *                                                  const mlib_s8 *c,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorMulS_U8_[Sat|Mod](mlib_u8       *xz,
 *                                               const mlib_u8 *c,
 *                                               mlib_s32      n);
 *      mlib_status mlib_VectorMulS_U8_U8_[Sat|Mod](mlib_u8       *z,
 *                                                  const mlib_u8 *x,
 *                                                  const mlib_u8 *c,
 *                                                  mlib_s32      n);
 *
 *      mlib_status mlib_VectorMulS_S16_S8_Mod(mlib_s16      *z,
 *                                             const mlib_s8 *x,
 *                                             const mlib_s8 *c,
 *                                             mlib_s32      n);
 *      mlib_status mlib_VectorMulS_S32_S16_Mod(mlib_s32       *z,
 *                                              const mlib_s16 *x,
 *                                              const mlib_s16 *c,
 *                                              mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      x    pointer to the first element of the input vector
 *      c    pointer to the scalar
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *      z = x * c
 */

/*
 * FUNCTIONS
 *      mlib_VectorMulS_U8C_[Sat|Mod],
 *      mlib_VectorMulS_[U8C|S16C]_U8C_[Sat|Mod] - multiplication of
 *                     unsigned 8-bit format complex vector to a scalar
 *      mlib_VectorMulS_S8C_[Sat|Mod],
 *      mlib_VectorMulS_[S8C|S16C]_S8C_[Sat|Mod] - multiplication of
 *                     signed 8-bit format complex vector to a scalar
 *      mlib_VectorMulS_S16C_[Sat|Mod],
 *      mlib_VectorMulS_[S16C|S32C]_S16C_[Sat|Mod] - multiplication of
 *                     signed 16-bit format complex vector to a scalar
 *      mlib_VectorMulS_S32C_[Sat|Mod],
 *      mlib_VectorMulS_S32C_S32C_[Sat|Mod] - multiplication of
 *                     signed 32-bit format complex vector to a scalar
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMulS_S16C_[Sat|Mod](mlib_s16       *xz,
 *                                                 const mlib_s16 *c,
 *                                                 mlib_s32       n);
 *      mlib_status mlib_VectorMulS_S16C_S16C_[Sat|Mod](mlib_s16       *z,
 *                                                      const mlib_s16 *x,
 *                                                      const mlib_s16 *c,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorMulS_S16C_S8C_[Sat|Mod](mlib_s16      *z,
 *                                                     const mlib_s8 *x,
 *                                                     const mlib_s8 *c,
 *                                                     mlib_s32      n);
 *      mlib_status mlib_VectorMulS_S16C_U8C_[Sat|Mod](mlib_s16      *z,
 *                                                     const mlib_u8 *x,
 *                                                     const mlib_u8 *c,
 *                                                     mlib_s32      n);
 *      mlib_status mlib_VectorMulS_S32C_[Sat|Mod](mlib_s32       *xz,
 *                                                 const mlib_s32 *c,
 *                                                 mlib_s32       n);
 *      mlib_status mlib_VectorMulS_S32C_S16C_[Sat|Mod](mlib_s32       *z,
 *                                                      const mlib_s16 *x,
 *                                                      const mlib_s16 *c,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorMulS_S32C_S32C_[Sat|Mod](mlib_s32       *z,
 *                                                      const mlib_s32 *x,
 *                                                      const mlib_s32 *c,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorMulS_S8C_[Sat|Mod](mlib_s8       *xz,
 *                                                const mlib_s8 *c,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorMulS_S8C_S8C_[Sat|Mod](mlib_s8       *z,
 *                                                    const mlib_s8 *x,
 *                                                    const mlib_s8 *c,
 *                                                    mlib_s32      n);
 *      mlib_status mlib_VectorMulS_U8C_[Sat|Mod](mlib_u8       *xz,
 *                                                const mlib_u8 *c,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorMulS_U8C_U8C_[Sat|Mod](mlib_u8       *z,
 *                                                    const mlib_u8 *x,
 *                                                    const mlib_u8 *c,
 *                                                    mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result vector.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts
 *      x    pointer to the first complex element of the input vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      c    pointer to a complex scalar. c[0] hold the real part,
 *           and c[1] hold the imaginary part
 *      n    number of complex elements in the vectors
 *
 * DESCRIPTION
 *      z = x * c
 */

#include <mlib_algebra.h>
#include <mlib_AlgebraUtil.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorMulS_S8_Mod = __mlib_VectorMulS_S8_Mod
#pragma weak mlib_VectorMulS_S16C_U8C_Mod = __mlib_VectorMulS_S16C_U8C_Mod
#pragma weak mlib_VectorMulS_U8_Mod = __mlib_VectorMulS_U8_Mod
#pragma weak mlib_VectorMulS_S16_U8_Sat = __mlib_VectorMulS_S16_U8_Sat
#pragma weak mlib_VectorMulS_S32_S32_Mod = __mlib_VectorMulS_S32_S32_Mod
#pragma weak mlib_VectorMulS_S32_S16_Mod = __mlib_VectorMulS_S32_S16_Mod
#pragma weak mlib_VectorMulS_S16C_S8C_Sat = __mlib_VectorMulS_S16C_S8C_Sat
#pragma weak mlib_VectorMulS_S16_S16_Mod = __mlib_VectorMulS_S16_S16_Mod
#pragma weak mlib_VectorMulS_S8_Sat = __mlib_VectorMulS_S8_Sat
#pragma weak mlib_VectorMulS_S16C_U8C_Sat = __mlib_VectorMulS_S16C_U8C_Sat
#pragma weak mlib_VectorMulS_S32C_S32C_Mod = __mlib_VectorMulS_S32C_S32C_Mod
#pragma weak mlib_VectorMulS_U8_Sat = __mlib_VectorMulS_U8_Sat
#pragma weak mlib_VectorMulS_S32C_S16C_Mod = __mlib_VectorMulS_S32C_S16C_Mod
#pragma weak mlib_VectorMulS_S32_S32_Sat = __mlib_VectorMulS_S32_S32_Sat
#pragma weak mlib_VectorMulS_S16C_S16C_Mod = __mlib_VectorMulS_S16C_S16C_Mod
#pragma weak mlib_VectorMulS_S16_S16_Sat = __mlib_VectorMulS_S16_S16_Sat
#pragma weak mlib_VectorMulS_S32C_S32C_Sat = __mlib_VectorMulS_S32C_S32C_Sat
#pragma weak mlib_VectorMulS_S32C_S16C_Sat = __mlib_VectorMulS_S32C_S16C_Sat
#pragma weak mlib_VectorMulS_S16C_S16C_Sat = __mlib_VectorMulS_S16C_S16C_Sat
#pragma weak mlib_VectorMulS_S8C_Mod = __mlib_VectorMulS_S8C_Mod
#pragma weak mlib_VectorMulS_U8C_Mod = __mlib_VectorMulS_U8C_Mod
#pragma weak mlib_VectorMulS_S8_S8_Mod = __mlib_VectorMulS_S8_S8_Mod
#pragma weak mlib_VectorMulS_S8C_Sat = __mlib_VectorMulS_S8C_Sat
#pragma weak mlib_VectorMulS_S32_Mod = __mlib_VectorMulS_S32_Mod
#pragma weak mlib_VectorMulS_U8C_Sat = __mlib_VectorMulS_U8C_Sat
#pragma weak mlib_VectorMulS_U8_U8_Mod = __mlib_VectorMulS_U8_U8_Mod
#pragma weak mlib_VectorMulS_S16_Mod = __mlib_VectorMulS_S16_Mod
#pragma weak mlib_VectorMulS_S32C_Mod = __mlib_VectorMulS_S32C_Mod
#pragma weak mlib_VectorMulS_S8C_S8C_Mod = __mlib_VectorMulS_S8C_S8C_Mod
#pragma weak mlib_VectorMulS_S16C_Mod = __mlib_VectorMulS_S16C_Mod
#pragma weak mlib_VectorMulS_S8_S8_Sat = __mlib_VectorMulS_S8_S8_Sat
#pragma weak mlib_VectorMulS_U8C_U8C_Mod = __mlib_VectorMulS_U8C_U8C_Mod
#pragma weak mlib_VectorMulS_S32_Sat = __mlib_VectorMulS_S32_Sat
#pragma weak mlib_VectorMulS_U8_U8_Sat = __mlib_VectorMulS_U8_U8_Sat
#pragma weak mlib_VectorMulS_S16_Sat = __mlib_VectorMulS_S16_Sat
#pragma weak mlib_VectorMulS_S32C_Sat = __mlib_VectorMulS_S32C_Sat
#pragma weak mlib_VectorMulS_S8C_S8C_Sat = __mlib_VectorMulS_S8C_S8C_Sat
#pragma weak mlib_VectorMulS_S16_S8_Mod = __mlib_VectorMulS_S16_S8_Mod
#pragma weak mlib_VectorMulS_S16C_Sat = __mlib_VectorMulS_S16C_Sat
#pragma weak mlib_VectorMulS_S16_U8_Mod = __mlib_VectorMulS_S16_U8_Mod
#pragma weak mlib_VectorMulS_U8C_U8C_Sat = __mlib_VectorMulS_U8C_U8C_Sat
#pragma weak mlib_VectorMulS_S16C_S8C_Mod = __mlib_VectorMulS_S16C_S8C_Mod

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorMulS_S8_Mod) mlib_VectorMulS_S8_Mod
    __attribute__((weak, alias("__mlib_VectorMulS_S8_Mod")));
__typeof__(__mlib_VectorMulS_S16C_U8C_Mod) mlib_VectorMulS_S16C_U8C_Mod
    __attribute__((weak, alias("__mlib_VectorMulS_S16C_U8C_Mod")));
__typeof__(__mlib_VectorMulS_U8_Mod) mlib_VectorMulS_U8_Mod
    __attribute__((weak, alias("__mlib_VectorMulS_U8_Mod")));
__typeof__(__mlib_VectorMulS_S16_U8_Sat) mlib_VectorMulS_S16_U8_Sat
    __attribute__((weak, alias("__mlib_VectorMulS_S16_U8_Sat")));
__typeof__(__mlib_VectorMulS_S32_S32_Mod) mlib_VectorMulS_S32_S32_Mod
    __attribute__((weak, alias("__mlib_VectorMulS_S32_S32_Mod")));
__typeof__(__mlib_VectorMulS_S32_S16_Mod) mlib_VectorMulS_S32_S16_Mod
    __attribute__((weak, alias("__mlib_VectorMulS_S32_S16_Mod")));
__typeof__(__mlib_VectorMulS_S16C_S8C_Sat) mlib_VectorMulS_S16C_S8C_Sat
    __attribute__((weak, alias("__mlib_VectorMulS_S16C_S8C_Sat")));
__typeof__(__mlib_VectorMulS_S16_S16_Mod) mlib_VectorMulS_S16_S16_Mod
    __attribute__((weak, alias("__mlib_VectorMulS_S16_S16_Mod")));
__typeof__(__mlib_VectorMulS_S8_Sat) mlib_VectorMulS_S8_Sat
    __attribute__((weak, alias("__mlib_VectorMulS_S8_Sat")));
__typeof__(__mlib_VectorMulS_S16C_U8C_Sat) mlib_VectorMulS_S16C_U8C_Sat
    __attribute__((weak, alias("__mlib_VectorMulS_S16C_U8C_Sat")));
__typeof__(__mlib_VectorMulS_S32C_S32C_Mod) mlib_VectorMulS_S32C_S32C_Mod
    __attribute__((weak, alias("__mlib_VectorMulS_S32C_S32C_Mod")));
__typeof__(__mlib_VectorMulS_U8_Sat) mlib_VectorMulS_U8_Sat
    __attribute__((weak, alias("__mlib_VectorMulS_U8_Sat")));
__typeof__(__mlib_VectorMulS_S32C_S16C_Mod) mlib_VectorMulS_S32C_S16C_Mod
    __attribute__((weak, alias("__mlib_VectorMulS_S32C_S16C_Mod")));
__typeof__(__mlib_VectorMulS_S32_S32_Sat) mlib_VectorMulS_S32_S32_Sat
    __attribute__((weak, alias("__mlib_VectorMulS_S32_S32_Sat")));
__typeof__(__mlib_VectorMulS_S16C_S16C_Mod) mlib_VectorMulS_S16C_S16C_Mod
    __attribute__((weak, alias("__mlib_VectorMulS_S16C_S16C_Mod")));
__typeof__(__mlib_VectorMulS_S16_S16_Sat) mlib_VectorMulS_S16_S16_Sat
    __attribute__((weak, alias("__mlib_VectorMulS_S16_S16_Sat")));
__typeof__(__mlib_VectorMulS_S32C_S32C_Sat) mlib_VectorMulS_S32C_S32C_Sat
    __attribute__((weak, alias("__mlib_VectorMulS_S32C_S32C_Sat")));
__typeof__(__mlib_VectorMulS_S32C_S16C_Sat) mlib_VectorMulS_S32C_S16C_Sat
    __attribute__((weak, alias("__mlib_VectorMulS_S32C_S16C_Sat")));
__typeof__(__mlib_VectorMulS_S16C_S16C_Sat) mlib_VectorMulS_S16C_S16C_Sat
    __attribute__((weak, alias("__mlib_VectorMulS_S16C_S16C_Sat")));
__typeof__(__mlib_VectorMulS_S8C_Mod) mlib_VectorMulS_S8C_Mod
    __attribute__((weak, alias("__mlib_VectorMulS_S8C_Mod")));
__typeof__(__mlib_VectorMulS_U8C_Mod) mlib_VectorMulS_U8C_Mod
    __attribute__((weak, alias("__mlib_VectorMulS_U8C_Mod")));
__typeof__(__mlib_VectorMulS_S8_S8_Mod) mlib_VectorMulS_S8_S8_Mod
    __attribute__((weak, alias("__mlib_VectorMulS_S8_S8_Mod")));
__typeof__(__mlib_VectorMulS_S8C_Sat) mlib_VectorMulS_S8C_Sat
    __attribute__((weak, alias("__mlib_VectorMulS_S8C_Sat")));
__typeof__(__mlib_VectorMulS_S32_Mod) mlib_VectorMulS_S32_Mod
    __attribute__((weak, alias("__mlib_VectorMulS_S32_Mod")));
__typeof__(__mlib_VectorMulS_U8C_Sat) mlib_VectorMulS_U8C_Sat
    __attribute__((weak, alias("__mlib_VectorMulS_U8C_Sat")));
__typeof__(__mlib_VectorMulS_U8_U8_Mod) mlib_VectorMulS_U8_U8_Mod
    __attribute__((weak, alias("__mlib_VectorMulS_U8_U8_Mod")));
__typeof__(__mlib_VectorMulS_S16_Mod) mlib_VectorMulS_S16_Mod
    __attribute__((weak, alias("__mlib_VectorMulS_S16_Mod")));
__typeof__(__mlib_VectorMulS_S32C_Mod) mlib_VectorMulS_S32C_Mod
    __attribute__((weak, alias("__mlib_VectorMulS_S32C_Mod")));
__typeof__(__mlib_VectorMulS_S8C_S8C_Mod) mlib_VectorMulS_S8C_S8C_Mod
    __attribute__((weak, alias("__mlib_VectorMulS_S8C_S8C_Mod")));
__typeof__(__mlib_VectorMulS_S16C_Mod) mlib_VectorMulS_S16C_Mod
    __attribute__((weak, alias("__mlib_VectorMulS_S16C_Mod")));
__typeof__(__mlib_VectorMulS_S8_S8_Sat) mlib_VectorMulS_S8_S8_Sat
    __attribute__((weak, alias("__mlib_VectorMulS_S8_S8_Sat")));
__typeof__(__mlib_VectorMulS_U8C_U8C_Mod) mlib_VectorMulS_U8C_U8C_Mod
    __attribute__((weak, alias("__mlib_VectorMulS_U8C_U8C_Mod")));
__typeof__(__mlib_VectorMulS_S32_Sat) mlib_VectorMulS_S32_Sat
    __attribute__((weak, alias("__mlib_VectorMulS_S32_Sat")));
__typeof__(__mlib_VectorMulS_U8_U8_Sat) mlib_VectorMulS_U8_U8_Sat
    __attribute__((weak, alias("__mlib_VectorMulS_U8_U8_Sat")));
__typeof__(__mlib_VectorMulS_S16_Sat) mlib_VectorMulS_S16_Sat
    __attribute__((weak, alias("__mlib_VectorMulS_S16_Sat")));
__typeof__(__mlib_VectorMulS_S32C_Sat) mlib_VectorMulS_S32C_Sat
    __attribute__((weak, alias("__mlib_VectorMulS_S32C_Sat")));
__typeof__(__mlib_VectorMulS_S8C_S8C_Sat) mlib_VectorMulS_S8C_S8C_Sat
    __attribute__((weak, alias("__mlib_VectorMulS_S8C_S8C_Sat")));
__typeof__(__mlib_VectorMulS_S16_S8_Mod) mlib_VectorMulS_S16_S8_Mod
    __attribute__((weak, alias("__mlib_VectorMulS_S16_S8_Mod")));
__typeof__(__mlib_VectorMulS_S16C_Sat) mlib_VectorMulS_S16C_Sat
    __attribute__((weak, alias("__mlib_VectorMulS_S16C_Sat")));
__typeof__(__mlib_VectorMulS_S16_U8_Mod) mlib_VectorMulS_S16_U8_Mod
    __attribute__((weak, alias("__mlib_VectorMulS_S16_U8_Mod")));
__typeof__(__mlib_VectorMulS_U8C_U8C_Sat) mlib_VectorMulS_U8C_U8C_Sat
    __attribute__((weak, alias("__mlib_VectorMulS_U8C_U8C_Sat")));
__typeof__(__mlib_VectorMulS_S16C_S8C_Mod) mlib_VectorMulS_S16C_S8C_Mod
    __attribute__((weak, alias("__mlib_VectorMulS_S16C_S8C_Mod")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	PACK_S16_U8	dr = vis_fpack16_pair(dr, dr1)

/* *********************************************************** */

#define	PACK8_SAT                                               \
	dr = vis_fpadd16(displacement, dr);                     \
	dr1 = vis_fpadd16(displacement, dr1);                   \
	dr = vis_fpack16_pair(dr, dr1);                         \
	dr = vis_fxor(dr, restore)

/* *********************************************************** */

#define	MULS_U8_U16_MOD                                         \
	dr = vis_fmul8x16al(vis_read_hi(dx), fc);               \
	dr1 = vis_fmul8x16al(vis_read_lo(dx), fc);              \
	dr2 = vis_fmul8x16al(vis_read_hi(dx), f127);            \
	dr3 = vis_fmul8x16al(vis_read_lo(dx), f127);            \
	dzh = vis_fpsub16(dr2, dr);                             \
	dzl = vis_fpsub16(dr3, dr1)

/* *********************************************************** */

#define	MULS_U8_U16_MOD_SHORT                                   \
	dzh = vis_fmul8x16al(vis_read_hi(dx), fc);              \
	dzl = vis_fmul8x16al(vis_read_lo(dx), fc)

/* *********************************************************** */

#define	MULS_U8_U16_SAT                                         \
	MULS_U8_U16_MOD;                                        \
	mask = vis_fcmplt16(dzh, dzero);                        \
	mask1 = vis_fcmplt16(dzl, dzero)

/* *********************************************************** */

#define	MULS_U8_U16_SAT_SHORT                                   \
	MULS_U8_U16_MOD_SHORT;                                  \
	mask = vis_fcmplt16(dzh, dzero);                        \
	mask1 = vis_fcmplt16(dzl, dzero)

/* *********************************************************** */

#define	MULS_S16_S8_MOD                                         \
	dr = vis_fpmerge(vis_read_hi(dx), fzero);               \
	dr1 = vis_fpmerge(vis_read_lo(dx), fzero);              \
	dzh = vis_fmul8sux16(dr, dc);                           \
	dzl = vis_fmul8sux16(dr1, dc)

/* *********************************************************** */

#define	MUL_S16_S32                                                    \
	{                                                              \
	    mlib_d64 dr2, dr3;                                         \
	                                                               \
	    dr = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dy));    \
	    dr1 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dy));   \
	    dzh = vis_fpadd32(dr, dr1);                                \
	    dr2 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dy));   \
	    dr3 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dy));   \
	    dzl = vis_fpadd32(dr2, dr3);                               \
	}

/* *********************************************************** */

#define	MULS_S16C_U8C_LO                                         \
	dr3 = vis_fpmerge(vis_read_hi(dx), vis_read_lo(dx));     \
	dr3 = vis_fpmerge(vis_read_hi(dr3), vis_read_lo(dr3));   \
	dr4 = vis_fmul8x16au(vis_read_hi(dr3), fc);              \
	dr5 = vis_fmul8x16al(vis_read_hi(dr3), fc);              \
	dr2 = vis_fmul8x16au(vis_read_lo(dr3), fc);              \
	dr3 = vis_fmul8x16al(vis_read_lo(dr3), fc);              \
	dr = vis_fpsub16(dr4, dr3);                              \
	dr1 = vis_fpadd16(dr5, dr2)

/* *********************************************************** */

#define	PACK_U8C0                                               \
	PACK_S16_U8;                                            \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr))

/* *********************************************************** */

#define	MULS_U8C_S16C                                                    \
	{                                                                \
	    mlib_d64 dr2, dr3, dr4, dr5, dr6, dr7;                       \
	                                                                 \
	    dr4 = vis_fmuld8ulx16(vis_read_hi(dx), fc0);                 \
	    dr6 = vis_fmuld8ulx16(vis_read_hi(dx), fc1);                 \
	    dr5 = vis_fmuld8ulx16(vis_read_lo(dx), fc0);                 \
	    dr7 = vis_fmuld8ulx16(vis_read_lo(dx), fc1);                 \
	    dr = vis_faligndata(dx, dx);                                 \
	    dr1 = vis_fmuld8ulx16(vis_read_lo(dr), fc0);                 \
	    dr3 = vis_fmuld8ulx16(vis_read_lo(dr), fc1);                 \
	    dr2 = vis_fmuld8ulx16(vis_read_hi(dr), fc1);                 \
	    dr = vis_fmuld8ulx16(vis_read_hi(dr), fc0);                  \
	    dr = vis_fpsub32(dr, dr6);                                   \
	    dr1 = vis_fpsub32(dr1, dr7);                                 \
	    dr2 = vis_fpadd32(dr2, dr4);                                 \
	    dr3 = vis_fpadd32(dr3, dr5);                                 \
	    dr = vis_freg_pair(vis_fpackfix(dr), vis_fpackfix(dr1));     \
	    dr1 = vis_freg_pair(vis_fpackfix(dr2), vis_fpackfix(dr3));   \
	}

/* *********************************************************** */

#define	MULS_S16C_U8C_FOR16_MOD                                 \
	{                                                       \
	    dr4 = vis_fmuld8ulx16(vis_read_hi(dx), fc0);        \
	    dr6 = vis_fmuld8ulx16(vis_read_hi(dx), fc1);        \
	    dr5 = vis_fmuld8ulx16(vis_read_lo(dx), fc0);        \
	    dr7 = vis_fmuld8ulx16(vis_read_lo(dx), fc1);        \
	    dr = vis_faligndata(dx, dx);                        \
	    dr1 = vis_fmuld8ulx16(vis_read_lo(dr), fc0);        \
	    dr3 = vis_fmuld8ulx16(vis_read_lo(dr), fc1);        \
	    dr2 = vis_fmuld8ulx16(vis_read_hi(dr), fc1);        \
	    dr = vis_fmuld8ulx16(vis_read_hi(dr), fc0);         \
	    dr = vis_fpsub32(dr, dr6);                          \
	    dr1 = vis_fpsub32(dr1, dr7);                        \
	    dr2 = vis_fpadd32(dr2, dr4);                        \
	    dr3 = vis_fpadd32(dr3, dr5);                        \
	}

/* *********************************************************** */

#define	MULS_S16C_U8C_FOR16                                          \
	{                                                            \
	    dr4 = vis_fmuld8ulx16(vis_read_hi(dx), fc0);             \
	    dr6 = vis_fmuld8ulx16(vis_read_hi(dx), fc1);             \
	    dr5 = vis_fmuld8ulx16(vis_read_lo(dx), fc0);             \
	    dr7 = vis_fmuld8ulx16(vis_read_lo(dx), fc1);             \
	    dr = vis_faligndata(dx, dx);                             \
	    dr1 = vis_fmuld8ulx16(vis_read_lo(dr), fc0);             \
	    dr3 = vis_fmuld8ulx16(vis_read_lo(dr), fc1);             \
	    dr2 = vis_fmuld8ulx16(vis_read_hi(dr), fc1);             \
	    dr = vis_fmuld8ulx16(vis_read_hi(dr), fc0);              \
	    dr = vis_fpsub32(dr, dr6);                               \
	    dr1 = vis_fpsub32(dr1, dr7);                             \
	    dr2 = vis_fpadd32(dr2, dr4);                             \
	    dr3 = vis_fpadd32(dr3, dr5);                             \
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

#define	MULS_S16C_U8C_FOR16_WITH_ODD                                 \
	{                                                            \
	    dr4 = vis_fmuld8ulx16(vis_read_hi(dx), fc0);             \
	    dr6 = vis_fmuld8ulx16(vis_read_hi(dx), fc1);             \
	    dr5 = vis_fmuld8ulx16(vis_read_lo(dx), fc0);             \
	    dr7 = vis_fmuld8ulx16(vis_read_lo(dx), fc1);             \
	    dr = vis_faligndata(dx, dx);                             \
	    dr1 = vis_fmuld8ulx16(vis_read_lo(dr), fc0);             \
	    dr3 = vis_fmuld8ulx16(vis_read_lo(dr), fc1);             \
	    dr2 = vis_fmuld8ulx16(vis_read_hi(dr), fc1);             \
	    dr = vis_fmuld8ulx16(vis_read_hi(dr), fc0);              \
	    dr = vis_fpsub32(dr, dr6);                               \
	    dr1 = vis_fpsub32(dr1, dr7);                             \
	    dr2 = vis_fpadd32(dr2, dr4);                             \
	    dr3 = vis_fpadd32(dr3, dr5);                             \
	    dr = vis_fpackfix_pair(dr, dr1);                         \
	    dr1 = vis_fpackfix_pair(dr2, dr3);                       \
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
	    rest = dr1;                                              \
	}

/* *********************************************************** */

#define	MULS_S16C_U8C_SHORT_EVEN                                 \
	dr3 = vis_fpmerge(vis_read_hi(dx), vis_read_lo(dx));     \
	dr3 = vis_fpmerge(vis_read_hi(dr3), vis_read_lo(dr3));   \
	dr4 = vis_fmul8x16au(vis_read_hi(dr3), fc);              \
	dr5 = vis_fmul8x16al(vis_read_hi(dr3), fc);              \
	dr2 = vis_fmul8x16au(vis_read_lo(dr3), fc);              \
	dr3 = vis_fmul8x16al(vis_read_lo(dr3), fc);              \
	dr = vis_fpsub16(dr4, dr3);                              \
	dr1 = vis_fpadd16(dr5, dr2);                             \
	dzh = vis_bshuffle(dr, dr1);                             \
	dr = vis_faligndata(dr, dr);                             \
	dr1 = vis_faligndata(dr1, dr1);                          \
	dzl = vis_bshuffle(dr, dr1)

/* *********************************************************** */

#define	MULS8C                                                  \
	{                                                       \
	    mlib_d64 dr2, dr3;                                  \
	                                                        \
	    dr = vis_fmul8sux16(dx, dc0);                       \
	    dr1 = vis_fmul8sux16(dx, dc1);                      \
	    dx = vis_fpack32(dx, dx);                           \
	    dr2 = vis_fmul8sux16(dx, dc0);                      \
	    dr3 = vis_fmul8sux16(dx, dc1);                      \
	    dr = vis_fpsub16(dr, dr3);                          \
	    dr1 = vis_fpadd16(dr2, dr1);                        \
	}

/* *********************************************************** */

#define	MULS_S16C_S32C                                            \
	dr = vis_fmuld8sux16(vis_read_hi(dx), fc0);               \
	dr1 = vis_fmuld8ulx16(vis_read_hi(dx), fc0);              \
	dr = vis_fpadd32(dr, dr1);                                \
	dr2 = vis_fmuld8sux16(vis_read_hi(dx), fc1);              \
	dr3 = vis_fmuld8ulx16(vis_read_hi(dx), fc1);              \
	dr1 = vis_fpadd32(dr2, dr3);                              \
	sr0 = vis_fpsub32s(vis_read_hi(dr), vis_read_lo(dr));     \
	sr1 = vis_fpadd32s(vis_read_hi(dr1), vis_read_lo(dr1));   \
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
	dzh = vis_freg_pair(sr0, sr1);                          \
	dzl = vis_freg_pair(sr2, sr3)

/* *********************************************************** */

#define	COMPOSE_S32C_FROM32_MOD_WITH_ODD                        \
	dzh = vis_freg_pair(rest, sr0);                         \
	dzl = vis_freg_pair(sr1, sr2);                          \
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

#define	MULS_S16_U8_IN_C                                        \
	{                                                       \
	    for (i = 0; i < n; i++) {                           \
		((*pz++)) = ((mlib_s32)(*px)) * uc;             \
		px++;                                           \
	    }                                                   \
	    return (n > 0 ? MLIB_SUCCESS : MLIB_FAILURE);       \
	}

/* *********************************************************** */

#define	MULS_S16_U8_SAT_IN_C                                       \
	{                                                          \
	    mlib_s32 c;                                            \
	                                                           \
	    for (i = 0; i < n; i++) {                              \
		c = ((mlib_s32)(*px)) * uc;                        \
		((*pz++)) = c > MLIB_S16_MAX ? MLIB_S16_MAX : c;   \
		px++;                                              \
	    }                                                      \
	    return (n > 0 ? MLIB_SUCCESS : MLIB_FAILURE);          \
	}

/* *********************************************************** */

#define	MULS_S16_S8_IN_C                                        \
	{                                                       \
	    for (i = 0; i < n; i++) {                           \
		((*pz++)) = ((mlib_s32)(*px)) * uc;             \
		px++;                                           \
	    }                                                   \
	    return (n > 0 ? MLIB_SUCCESS : MLIB_FAILURE);       \
	}

/* *********************************************************** */

#define	MULS_S32_S16_IN_C                                       \
	{                                                       \
	    for (i = 0; i < n; i++) {                           \
		((*pz++)) = ((mlib_s32)(*px)) * uc;             \
		px++;                                           \
	    }                                                   \
	    return (n > 0 ? MLIB_SUCCESS : MLIB_FAILURE);       \
	}

/* *********************************************************** */

#define	MULS_S16C_U8C_SAT_IN_C                                      \
	{                                                           \
	    for (i = 0; i < n; i++) {                               \
		re_x = ((*px++));                                   \
		im_x = ((*px++));                                   \
		re_c = re_x * uc0 - im_x * uc1;                     \
		im_c = im_x * uc0 + re_x * uc1;                     \
		((*pz++)) =                                         \
		    re_c < MLIB_S16_MIN ? MLIB_S16_MIN : (re_c >    \
		    MLIB_S16_MAX ? MLIB_S16_MAX : re_c);            \
		im_c = im_c > MLIB_S16_MAX ? MLIB_S16_MAX : im_c;   \
		((*pz++)) = im_c;                                   \
	    }                                                       \
	    return (n > 0 ? MLIB_SUCCESS : MLIB_FAILURE);           \
	}

/* *********************************************************** */

#define	MULS_S16C_U8C_MOD_IN_C                                  \
	{                                                       \
	    for (i = 0; i < n; i++) {                           \
		re_x = ((*px++));                               \
		im_x = ((*px++));                               \
		re_c = re_x * uc0 - im_x * uc1;                 \
		im_c = im_x * uc0 + re_x * uc1;                 \
		((*pz++)) = re_c;                               \
		((*pz++)) = im_c;                               \
	    }                                                   \
	    return (n > 0 ? MLIB_SUCCESS : MLIB_FAILURE);       \
	}

/* *********************************************************** */

#define	MULS_S16C_S8C_MOD_IN_C                                  \
	{                                                       \
	    for (i = 0; i < n; i++) {                           \
		re_x = ((*px++));                               \
		im_x = ((*px++));                               \
		re_c = re_x * sc0 - im_x * sc1;                 \
		im_c = im_x * sc0 + re_x * sc1;                 \
		((*pz++)) = re_c;                               \
		((*pz++)) = im_c;                               \
	    }                                                   \
	    return (MLIB_SUCCESS);                              \
	}

/* *********************************************************** */

#define	MULS_S32C_S16C_SAT_IN_C                                           \
	{                                                                 \
	    for (i = 0; i < n; i++) {                                     \
		re_x = ((*px++));                                         \
		im_x = ((*px++));                                         \
		re_c = re_x * sc0 - im_x * sc1;                           \
		im_c = im_x * sc0 + re_x * sc1;                           \
		((*pz++)) = re_c;                                         \
		((*pz++)) = (im_c == 0x80000000 ? MLIB_S32_MAX : im_c);   \
	    }                                                             \
	    return (n > 0 ? MLIB_SUCCESS : MLIB_FAILURE);                 \
	}

/* *********************************************************** */

#define	MULS_S32C_S16C_MOD_IN_C                                 \
	{                                                       \
	    for (i = 0; i < n; i++) {                           \
		re_x = ((*px++));                               \
		im_x = ((*px++));                               \
		re_c = re_x * sc0 - im_x * sc1;                 \
		im_c = im_x * sc0 + re_x * sc1;                 \
		((*pz++)) = re_c;                               \
		((*pz++)) = im_c;                               \
	    }                                                   \
	    return (n > 0 ? MLIB_SUCCESS : MLIB_FAILURE);       \
	}

/* *********************************************************** */

#define	MULS_U8_OVER_SAT_1                                      \
	dr1 = vis_fpmerge(dzero, vis_read_hi(dx));              \
	dr2 = vis_fpmerge(dzero, vis_read_lo(dx))

/* *********************************************************** */

#define	MULS_U8_OVER_SAT_2                                      \
	mask_for_0 = vis_fcmpeq16(dr2, dzero);                  \
	mask_for_0 |= vis_fcmpeq16(dr1, dzero) << 4;            \
	mask_for_1 = vis_fcmpeq16(dr2, done);                   \
	mask_for_1 |= vis_fcmpeq16(dr1, done) << 4

/* *********************************************************** */

#define	MUL_S_U8_LO_1_SAT                                       \
	dr1 = vis_fmul8x16al(vis_read_hi(dx), fc);              \
	dr2 = vis_fmul8x16al(vis_read_lo(dx), fc)

/* *********************************************************** */

#define	MUL_S_U8_LO_2_SAT	dr = vis_fpack16_pair(dr1, dr2)

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
__mlib_VectorMulS_U8_U8_Sat(
    mlib_u8 *z,
    const mlib_u8 *x,
    const mlib_u8 *c,
    mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s32 len, i;
	mlib_u8 *pzend;
	mlib_u8 uc = ((mlib_u8 *)c)[0];
	mlib_u16 ucc = (uc | (uc << 8));
	mlib_d64 *dpx, *dpz, *dpzend;
	mlib_d64 dx, dx0, dx1, dr, dr1, dr2;
	mlib_f32 fc;

	mlib_s32 mask_for_0, mask_for_1;
	mlib_d64 scale;
	mlib_d64 dzero = vis_fzero();
	mlib_d64 done = vis_to_double_dup(0x10001);
	mlib_d64 dmax_u8 = vis_fone();

	if (n <= 0)
		return (MLIB_FAILURE);

/*
 * prepare the destination address
 */
	pzend = (mlib_u8 *)z + n - 1;
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));

/* c >= 128 */
	if (uc & MLIB_S8_MIN) {

		vis_write_gsr(7 << 3);
		dpz = (mlib_d64 *)((mlib_addr)z & (~7));
		off = (mlib_addr)dpz - (mlib_addr)z;

/*
 * generate edge mask for the start point
 */
		emask = vis_edge8(z, pzend);
/*
 * prepare the scale factor
 */
		scale = vis_to_double_dup(ucc | (ucc << 16));
/*
 * prepare the source address
 */

		dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);

		vis_pst_8(dmax_u8, dpz, emask);
		MULS_U8_OVER_SAT_1;
		MULS_U8_OVER_SAT_2;
		vis_pst_8(dzero, dpz, mask_for_0 & emask);
		vis_pst_8(scale, dpz, mask_for_1 & emask);
		dpz++;

		dx0 = dx1;
		len = (mlib_s32)((mlib_addr)dpzend - (mlib_addr)dpz);

#pragma pipeloop(0)
		for (i = 0; i < len; i += 8) {
			dx1 = vis_ld_d64_nf(dpx + 2);
			dx = vis_faligndata(dx0, dx1);
			MULS_U8_OVER_SAT_1;
			MULS_U8_OVER_SAT_2;
			dpz[0] = dmax_u8;
			vis_pst_8(dzero, dpz, mask_for_0);
			vis_pst_8(scale, dpz, mask_for_1);
			dx0 = dx1;
			dpx++;
			dpz++;
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
/*
 * prepare the edge mask for the last point
 */
			emask = vis_edge8(dpz, pzend);
			vis_pst_8(dmax_u8, dpz, emask);
			dx1 = vis_ld_d64_nf(dpx + 2);
			dx = vis_faligndata(dx0, dx1);
			MULS_U8_OVER_SAT_1;
			MULS_U8_OVER_SAT_2;
			vis_pst_8(dzero, dpz, mask_for_0 & emask);
			vis_pst_8(scale, dpz, mask_for_1 & emask);
		}

	} else {
/* prepare the scaling factor */
		fc = vis_to_float(uc << 8);
		vis_write_gsr(7 << 3);
		dpz = (mlib_d64 *)((mlib_addr)z & (~7));
		off = (mlib_addr)dpz - (mlib_addr)z;

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

		MUL_S_U8_LO_1_SAT;
		MUL_S_U8_LO_2_SAT;
/* store first bites */
		vis_pst_8(dr, dpz, emask);
		dpz++;
		dx0 = dx1;

		len = (mlib_s32)((mlib_addr)dpzend - (mlib_addr)dpz);

#pragma pipeloop(0)
		for (i = 0; i < len; i += 8) {
			dx1 = vis_ld_d64_nf(dpx + 2);
			dx = vis_faligndata(dx0, dx1);
			MUL_S_U8_LO_1_SAT;
			MUL_S_U8_LO_2_SAT;
/* store 8 bytes of result */
			dpz[0] = dr;
			dx0 = dx1;
			dpx++;
			dpz++;
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			dx1 = vis_ld_d64_nf(dpx + 2);
			dx = vis_faligndata(dx0, dx1);
			MUL_S_U8_LO_1_SAT;
			MUL_S_U8_LO_2_SAT;
/* prepare edge mask for the last bytes */
			emask = vis_edge8(dpz, pzend);
			vis_pst_8(dr, dpz, emask);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_U8_Sat(
    mlib_u8 *xz,
    const mlib_u8 *c,
    mlib_s32 n)
{
	return (__mlib_VectorMulS_U8_U8_Sat(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_U8_U8_Mod(
    mlib_u8 *z,
    const mlib_u8 *x,
    const mlib_u8 *c,
    mlib_s32 n)
{
	return __mlib_VectorMulS_S8_S8_Mod((mlib_s8 *)z, (mlib_s8 *)x,
	    (mlib_s8 *)c, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16_U8_Mod(
    mlib_s16 *z,
    const mlib_u8 *x,
    const mlib_u8 *c,
    mlib_s32 n)
{
	mlib_d64 *dpz, *dpx;
	mlib_d64 dx, dx0, dx1, dr, dr1, dr2, dr3;
	mlib_d64 dzh, dzl;
	mlib_u8 *px;
	mlib_u16 uc = *((mlib_u8 *)c);
	mlib_s16 *pz;
	mlib_f32 f127 = vis_to_float(0x7f00);
	mlib_f32 fc;
	mlib_s32 len = n, i;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_u8 *)x;
	pz = (mlib_s16 *)z;

	if (n <= 8)
		MULS_S16_U8_IN_C;

/*
 * prepare the destination address
 */

	while ((mlib_addr)pz & 7) {
		((*pz++)) = ((mlib_s32)(*px)) * uc;
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

		if (uc & 0x80) {

/*
 * uc > 128
 */

			fc = vis_to_float((uc ^ 0x7f) << 8);

#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = (*dpx++);
				MULS_U8_U16_MOD;
/*
 * store 16 bytes of result
 */
				(*dpz++) = dzh;
				(*dpz++) = dzl;
			}

		} else {

/*
 * uc < 128 -> Very mlib_s16 case.
 */

			fc = vis_to_float((uc) << 8);

#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = (*dpx++);
				MULS_U8_U16_MOD_SHORT;
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

		if (uc & 0x80) {

/*
 * uc > 128
 */

			fc = vis_to_float((uc ^ 0x7f) << 8);

#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx); dpx++;
				dx = vis_faligndata(dx0, dx1);
				MULS_U8_U16_MOD;
/*
 * store 16 bytes of result
 */
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
				dx0 = dx1;
			}

		} else {

/*
 * uc < 128 -> Very mlib_s16 case.
 */

			fc = vis_to_float((uc) << 8);
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx); dpx++;
				dx = vis_faligndata(dx0, dx1);
				MULS_U8_U16_MOD_SHORT;
/*
 * store 16 bytes of result
 */
				dpz[0] = dzh;
				dpz[1] = dzl;
				dx0 = dx1;
				dpz += 2;
			}
		}
	}

	if (!rest_8)
		return (MLIB_SUCCESS);

	px += (even_8 << 3);
	pz += (even_8 << 3);

	while (rest_8--) {
		((*pz++)) = ((mlib_s32)(*px)) * uc;
		px++;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16_U8_Sat(
    mlib_s16 *z,
    const mlib_u8 *x,
    const mlib_u8 *pc,
    mlib_s32 n)
{
	mlib_d64 *dpz, *dpx;
	mlib_d64 dx, dx0, dx1, dr, dr1, dr2, dr3;
	mlib_d64 dzh, dzl;
	mlib_u8 *px;
	mlib_u16 uc = *((mlib_u8 *)pc);
	mlib_s16 *pz;
	mlib_f32 f127 = vis_to_float(0x7f00);
	mlib_f32 fc;
	mlib_s32 len = n, i;
	mlib_s32 c;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_d64 dzero = vis_to_double_dup(0);
	mlib_d64 control_ovl =
	    vis_to_double_dup((MLIB_S16_MAX << 16) | MLIB_S16_MAX);
	mlib_s32 mask, mask1;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_u8 *)x;
	pz = (mlib_s16 *)z;

	if (n <= 8)
		MULS_S16_U8_SAT_IN_C;

/*
 * prepare the destination address
 */

	while ((mlib_addr)pz & 7) {
		c = ((mlib_s32)(*px)) * uc;
		((*pz++)) = c > MLIB_S16_MAX ? MLIB_S16_MAX : c;
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
		dx = dpx[0];
		dpx++;

		if (uc & 0x80) {

/*
 * uc > 128
 */

			fc = vis_to_float((uc ^ 0x7f) << 8);

/*
 * Make loop unrolling for 2 iterations.
 */

			if (i = (even_8 & 1)) {
				dx1 = vis_ld_d64_nf(dpx);
				MULS_U8_U16_SAT;
				dx = dx1;
/*
 * store 16 bytes of result
 */
				dpz[0] = dzh;
				dpz[1] = dzl;
				vis_pst_16(control_ovl, dpz, mask);
				vis_pst_16(control_ovl, dpz + 1, mask1);
				dpz += 2;
				dpx++;
			}
#pragma pipeloop(0)
			for (; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
				MULS_U8_U16_SAT;
				dx = dx1;
/*
 * store 16 bytes of result
 */
				dpz[0] = dzh;
				dpz[1] = dzl;
				vis_pst_16(control_ovl, dpz, mask);
				vis_pst_16(control_ovl, dpz + 1, mask1);
				dpz += 2;
				dpx++;
			}

		} else {

/*
 * uc < 128 -> Very mlib_s16 case.
 */

			fc = vis_to_float((uc) << 8);

/*
 * Make loop unrolling for 2 iterations.
 */

			if (i = (even_8 & 1)) {
				dx1 = vis_ld_d64_nf(dpx);
				MULS_U8_U16_MOD_SHORT;
				dx = dx1;
/*
 * store 16 bytes of result
 */
				(*dpz++) = dzh;
				(*dpz++) = dzl;
				dpx++;
			}
#pragma pipeloop(0)
			for (; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
				MULS_U8_U16_MOD_SHORT;
				dx = dx1;
/*
 * store 16 bytes of result
 */
				(*dpz++) = dzh;
				(*dpz++) = dzl;
				dpx++;
			}
		}

	} else {

/*
 * "x" address is arbitrary aligned.
 */

		dpx = vis_alignaddr(px, 0);
		dx0 = (*dpx++);

		if (uc & 0x80) {

/*
 * uc > 128
 */

			fc = vis_to_float((uc ^ 0x7f) << 8);

#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
				dx = vis_faligndata(dx0, dx1);
				MULS_U8_U16_SAT;
				dx0 = dx1;
/*
 * store 16 bytes of result
 */
				dpz[0] = dzh;
				dpz[1] = dzl;
				vis_pst_16(control_ovl, dpz, mask);
				vis_pst_16(control_ovl, dpz + 1, mask1);
				dpz += 2;
				dpx++;
			}

		} else {

/*
 * uc < 128 -> Very mlib_s16 case.
 */

			fc = vis_to_float((uc) << 8);
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
				dx = vis_faligndata(dx0, dx1);
				MULS_U8_U16_MOD_SHORT;
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

	if (!rest_8)
		return (MLIB_SUCCESS);

	px += (even_8 << 3);
	pz += (even_8 << 3);

	while (rest_8) {
		c = ((mlib_s32)(*px)) * uc;
		((*pz++)) = c > MLIB_S16_MAX ? MLIB_S16_MAX : c;
		px++;
		rest_8--;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_U8_Mod(
    mlib_u8 *xz,
    const mlib_u8 *c,
    mlib_s32 n)
{
	return (__mlib_VectorMulS_U8_U8_Mod(xz, xz, c, n));
}

/* *********************************************************** */

#define	MUL_S_S8_1_SAT                                          \
	dr1 = vis_fpmerge(vis_read_hi(dx), vis_read_hi(dx));    \
	dr2 = vis_fpmerge(vis_read_lo(dx), vis_read_lo(dx));    \
	dr3 = vis_fmul8sux16(dr1, dc);                          \
	dr4 = vis_fmul8sux16(dr2, dc)

/* *********************************************************** */

#define	MUL_S_S8_2_SAT                                          \
	dr5 = vis_fpadd16(dr3, displacement);                   \
	dr6 = vis_fpadd16(dr4, displacement);                   \
	dr = vis_fpack16_pair(dr5, dr6);                        \
	dr = vis_fxor(dr, restore)

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S8_S8_Sat(
    mlib_s8 *z,
    const mlib_s8 *x,
    const mlib_s8 *c,
    mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask, len;

/* offset of address alignment in destination */
	mlib_s32 off, i;
	mlib_s8 *pzend;
	mlib_u16 uc = ((mlib_u8 *)c)[0] << 8;
	mlib_d64 *dpx, *dpz, *dpzend;
	mlib_d64 dx, dx0, dx1, dr, dr1, dr2, dr3, dr4, dr5, dr6;
	mlib_d64 restore = vis_to_double_dup(0x80808080);
	mlib_d64 displacement = vis_to_double_dup(0x800080);

/* prepare the scaling factor */
	mlib_d64 dc = vis_to_double_dup((uc << 16) | uc);

	if (n <= 0)
		return (MLIB_FAILURE);

/*
 * prepare the destination address
 */
	pzend = (mlib_s8 *)z + n - 1;
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z;

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);

/*
 * generate edge mask for the start point
 */
	emask = vis_edge8(z, pzend);
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));

/*
 * prepare the source address
 */

	dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);
	dx = vis_faligndata(dx0, dx1);

	MUL_S_S8_1_SAT;
	MUL_S_S8_2_SAT;

/* store first bytes of result */
	vis_pst_8(dr, dpz, emask);
	dpz++;
	dx0 = dx1;

	len = (mlib_s32)((mlib_addr)dpzend - (mlib_addr)dpz);

#pragma pipeloop(0)
	for (i = 0; i < len; i += 8) {
		dx1 = vis_ld_d64_nf(dpx + 2);
		dx = vis_faligndata(dx0, dx1);
		MUL_S_S8_1_SAT;
		MUL_S_S8_2_SAT;
/* store 8 bytes of result */
		dpz[0] = dr;
		dx0 = dx1;
		dpx++;
		dpz++;
	}

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
		dx1 = vis_ld_d64_nf(dpx + 2);
		dx = vis_faligndata(dx0, dx1);
		MUL_S_S8_1_SAT;
		MUL_S_S8_2_SAT;
/* prepare edge mask for the last bytes */
		emask = vis_edge8(dpz, pzend);
		vis_pst_8(dr, dpz, emask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16_S8_Mod(
    mlib_s16 *z,
    const mlib_s8 *x,
    const mlib_s8 *c,
    mlib_s32 n)
{
	mlib_d64 *dpz, *dpx;
	mlib_d64 dx, dx0, dx1, dr, dr1;
	mlib_d64 dzh, dzl;
	mlib_s8 *px;
	mlib_s16 uc = *((mlib_s8 *)c);
	mlib_s16 *pz;
	mlib_d64 dc;
	mlib_s32 len = n, i;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_f32 fzero = vis_fzeros();

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s8 *)x;
	pz = (mlib_s16 *)z;

	if (n <= 8)
		MULS_S16_S8_IN_C;

/*
 * prepare the destination address
 */

	while ((mlib_addr)pz & 7) {
		((*pz++)) = ((mlib_s32)(*px)) * uc;
		px++;
		len--;
	}

	dpz = (mlib_d64 *)pz;

	even_8 = len >> 3;
	rest_8 = len & 0x7;
	dc = vis_to_double_dup((uc << 24) | ((uc << 8) & 0xffff));

	if (!((mlib_addr)px & 7)) {

/*
 * Source address is 8-byte aligned.
 * No  vis_alignaddr and  vis_faligndata at all.
 */

		dpx = (mlib_d64 *)px;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = dpx[0];
			MULS_S16_S8_MOD;
/*
 * store 16 bytes of result
 */
			(*dpz++) = dzh;
			(*dpz++) = dzl;
			dpx++;
		}

	} else {

/*
 * "x" address is arbitrary aligned.
 */

		dpx = vis_alignaddr(px, 0);
		dx0 = (*dpx++);

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx);
			dx = vis_faligndata(dx0, dx1);
			MULS_S16_S8_MOD;
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

	if (!rest_8)
		return (MLIB_SUCCESS);

	px += (even_8 << 3);
	pz += (even_8 << 3);

	while (rest_8--) {
		((*pz++)) = ((mlib_s32)(*px)) * uc;
		px++;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S8_Sat(
    mlib_s8 *xz,
    const mlib_s8 *c,
    mlib_s32 n)
{
	return (__mlib_VectorMulS_S8_S8_Sat(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S8_S8_Mod(
    mlib_s8 *z,
    const mlib_s8 *x,
    const mlib_s8 *c,
    mlib_s32 n)
{
	mlib_s32 n8 = n >> 3, i;
	mlib_f32 fvalc = vis_to_float(c[0] << 8);
	mlib_d64 dvalx, *dx = (mlib_d64 *)x, *dz = (mlib_d64 *)z, hi, lo;
	mlib_s32 ez;
	mlib_d64 valc = c[0], ld0, ld1;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!((mlib_addr)z & 7) && !((mlib_addr)x & 7)) {

#pragma pipeloop(0)
		for (i = 0; i < n8; i++) {
			dvalx = dx[i];
			dvalx = vis_fpmerge(vis_read_hi(dvalx),
			    vis_read_lo(dvalx));
			dvalx = vis_fpmerge(vis_read_hi(dvalx),
			    vis_read_lo(dvalx));

			hi = vis_fmul8x16al(vis_read_hi(dvalx), fvalc);
			lo = vis_fmul8x16al(vis_read_lo(dvalx), fvalc);
			hi = vis_fpack32(hi, 0);

			dz[i] = lo;
			vis_pst_8(hi, dz + i, 0xAA);
		}

		if (n % 8)
			for (i = 0; i < n % 8; i++)
				z[n - i - 1] = x[n - i - 1] * valc;

	} else {
		mlib_s32 off;

		dz = vis_alignaddr(z, 0);
		ez = z - (mlib_s8 *)dz;
		off = (mlib_addr)(x - ez) & 7;
		dx = (mlib_d64 *)(x - ez - off);
		vis_write_bmask(off * 0x11111111, 0x02461357);
		for (i = 0; i < 8 - ez && i < n; i++)
			z[i] = x[i] * valc;

		dx++;
		dz++;
		n -= 8 - ez;
		x += 8 - ez;
		z += 8 - ez;
		n8 = n >> 3;

		ld0 = vis_ld_d64_nf(dx); dx++;
#pragma pipeloop(0)
		for (i = 0; i < n8; i++) {
			ld1 = vis_ld_d64_nf(dx + i);
			dvalx = vis_bshuffle(ld0, ld1);

			hi = vis_fmul8x16al(vis_read_hi(dvalx), fvalc);
			lo = vis_fmul8x16al(vis_read_lo(dvalx), fvalc);
			hi = vis_fpack32(hi, 0);

			dz[i] = lo;
			vis_pst_8(hi, dz + i, 0xAA);
			ld0 = ld1;
		}

		if (n % 8)
			for (i = 0; i < n % 8; i++)
				z[n - i - 1] = x[n - i - 1] * valc;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S8_Mod(
    mlib_s8 *xz,
    const mlib_s8 *c,
    mlib_s32 n)
{
	return (__mlib_VectorMulS_S8_S8_Mod(xz, xz, c, n));
}

/* *********************************************************** */

#define	MULS_S16_SAT_1                                          \
	dr1 = vis_fmuld8sux16(vis_read_hi(dx), fc);             \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dx), fc);             \
	dr3 = vis_fmuld8sux16(vis_read_lo(dx), fc);             \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dx), fc)

/* *********************************************************** */

#define	MULS_S16_SAT_2                                          \
	dr1 = vis_fpadd32(dr1, dr2);                            \
	dr3 = vis_fpadd32(dr3, dr4);                            \
	dr2 = vis_fpadd32(dr1, dr1);                            \
	dr4 = vis_fpadd32(dr3, dr3);                            \
	dr = vis_fpackfix_pair(dr2, dr4)

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16_S16_Sat(
    mlib_s16 *z,
    const mlib_s16 *x,
    const mlib_s16 *c,
    mlib_s32 n)
{
	mlib_f32 c_dir = vis_to_float((c[0] << 16) | (c[0] & 0xffff));
	mlib_d64 hi0, lo0, hi1, lo1, res, *z_d64 = (mlib_d64 *)z;
	mlib_d64 *x_d64 = (mlib_d64 *)x, val_x_d64, pair0, pair1;
	mlib_s32 i, n4 = n >> 2;

	if (n <= 0)
		return (MLIB_FAILURE);

	vis_write_gsr(16 << 3);

	if (!((mlib_addr)z & 7) && !((mlib_addr)x & 7) &&
	    c[0] != MLIB_S16_MIN) {

#pragma pipeloop(0)
		for (i = 0; i < n4; i++) {
			val_x_d64 = x_d64[i];

			hi0 = vis_fmuld8sux16(vis_read_hi(val_x_d64), c_dir);
			lo0 = vis_fmuld8ulx16(vis_read_hi(val_x_d64), c_dir);
			hi1 = vis_fmuld8sux16(vis_read_lo(val_x_d64), c_dir);
			lo1 = vis_fmuld8ulx16(vis_read_lo(val_x_d64), c_dir);

			pair0 = vis_fpadd32(hi0, lo0);
			pair1 = vis_fpadd32(hi1, lo1);

			z_d64[i] = vis_fpackfix_pair(pair0, pair1);
		}

		for (i = 0; i < n % 4; i++) {
			mlib_d64 mul;

			mul = c[0] * (mlib_d64)x[n - i - 1];
			z[n - i - 1] =
			    (mul > MLIB_S16_MAX) ? MLIB_S16_MAX : (mul <
			    MLIB_S16_MIN ? MLIB_S16_MIN : mul);
		}

	} else {

		if (c[0] == MLIB_S16_MIN) {
#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				z[i] = (x[i] > 0) ? MLIB_S16_MIN : (x[i] <
				    0) ? MLIB_S16_MAX : 0;
			}

		} else {
			mlib_s32 ez, mask;
			mlib_d64 ld0, ld1;

			z_d64 = vis_alignaddr(z, 0);
			ez = z - (mlib_s16 *)z_d64;

			x_d64 = vis_alignaddr((void *)(x - ez), 0);

			val_x_d64 = vis_faligndata(vis_ld_d64_nf(x_d64),
				vis_ld_d64_nf(x_d64 + 1));

			hi0 = vis_fmuld8sux16(vis_read_hi(val_x_d64), c_dir);
			lo0 = vis_fmuld8ulx16(vis_read_hi(val_x_d64), c_dir);
			hi1 = vis_fmuld8sux16(vis_read_lo(val_x_d64), c_dir);
			lo1 = vis_fmuld8ulx16(vis_read_lo(val_x_d64), c_dir);

			pair0 = vis_fpadd32(hi0, lo0);
			pair1 = vis_fpadd32(hi1, lo1);

			res = vis_fpackfix_pair(pair0, pair1);

			mask = 0xf >> ez;

			if (ez + n < 4)
				mask &= 0xf << (4 - ez - n);

			vis_pst_16(res, z_d64, mask);

			x_d64++;
			z_d64++;

			n -= 4 - ez;
			z += 4 - ez;
			x += 4 - ez;

			n4 = (n / 4);
			ld0 = vis_ld_d64_nf(x_d64); x_d64++;
#pragma pipeloop(0)
			for (i = 0; i < n4; i++) {
				ld1 = vis_ld_d64_nf(x_d64 + i);
				val_x_d64 = vis_faligndata(ld0, ld1);

				hi0 = vis_fmuld8sux16(vis_read_hi(val_x_d64),
				    c_dir);
				lo0 = vis_fmuld8ulx16(vis_read_hi(val_x_d64),
				    c_dir);
				hi1 = vis_fmuld8sux16(vis_read_lo(val_x_d64),
				    c_dir);
				lo1 = vis_fmuld8ulx16(vis_read_lo(val_x_d64),
				    c_dir);

				pair0 = vis_fpadd32(hi0, lo0);
				pair1 = vis_fpadd32(hi1, lo1);

				z_d64[i] = vis_fpackfix_pair(pair0, pair1);
				ld0 = ld1;
			}

			for (i = 0; i < n % 4; i++) {
				mlib_d64 mul;

				mul = c[0] * (mlib_d64)x[n - i - 1];
				z[n - i - 1] =
				    (mul >
				    MLIB_S16_MAX) ? MLIB_S16_MAX : (mul <
				    MLIB_S16_MIN ? MLIB_S16_MIN : mul);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16_Sat(
    mlib_s16 *xz,
    const mlib_s16 *c,
    mlib_s32 n)
{
	return (__mlib_VectorMulS_S16_S16_Sat(xz, xz, c, n));
}

/* *********************************************************** */

#define	MULS16                                                  \
	{                                                       \
	    mlib_d64 dr2, dr3;                                  \
	                                                        \
	    dr = vis_fmuld8sux16(vis_read_hi(dx), fc);          \
	    dr1 = vis_fmuld8ulx16(vis_read_hi(dx), fc);         \
	    dr = vis_fpadd32(dr, dr1);                          \
	    dr2 = vis_fmuld8sux16(vis_read_lo(dx), fc);         \
	    dr3 = vis_fmuld8ulx16(vis_read_lo(dx), fc);         \
	    dr1 = vis_fpadd32(dr2, dr3);                        \
	}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16_S16_Mod(
    mlib_s16 *z,
    const mlib_s16 *x,
    const mlib_s16 *c,
    mlib_s32 n)
{
	mlib_s32 i, n4;
	mlib_d64 *dx, *dz;
	mlib_d64 dvalx, hi0, lo0, hi1, lo1, valc = c[0];
	mlib_f32 c_dir = vis_to_float((c[0] << 16) | (c[0] & 0xffff));
	mlib_d64 s0, s1;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!((mlib_addr)z & 7) && !((mlib_addr)x & 7) &&
	    c[0] != MLIB_S16_MIN) {
		n4 = n >> 2;
		dx = (mlib_d64 *)x;
		dz = (mlib_d64 *)z;
		vis_write_bmask(0x2367abef, 0);

#pragma pipeloop(0)
		for (i = 0; i < n4; i++) {

			dvalx = dx[i];

			hi0 = vis_fmuld8sux16(vis_read_hi(dvalx), c_dir);
			lo0 = vis_fmuld8ulx16(vis_read_hi(dvalx), c_dir);
			hi1 = vis_fmuld8sux16(vis_read_lo(dvalx), c_dir);
			lo1 = vis_fmuld8ulx16(vis_read_lo(dvalx), c_dir);

			s0 = vis_fpadd32(hi0, lo0);
			s1 = vis_fpadd32(hi1, lo1);

			dz[i] = vis_bshuffle(s0, s1);
		}

		for (i = 0; i < (n & 3); i++) {
			z[n - i - 1] = c[0] * x[n - i - 1];
		}

	} else {

		if (c[0] == MLIB_S16_MIN) {
#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				z[i] = valc * x[i];
			}

		} else {
			mlib_d64 ld0, ld1;

			while (((mlib_addr)z & 7) && (n > 0)) {
				((*z++)) = c[0] * x[0];
				x++;
				n--;
			}

			n4 = n >> 2;
			dx = vis_alignaddr((void *)x, 0);
			dz = (mlib_d64 *)z;
			ld0 = vis_ld_d64_nf(dx); dx++;
			vis_write_bmask(0x2367abef, 0);

#pragma pipeloop(0)
			for (i = 0; i < n4; i++) {

				ld1 = vis_ld_d64_nf(dx); dx++;
				dvalx = vis_faligndata(ld0, ld1);
				ld0 = ld1;

				hi0 = vis_fmuld8sux16(vis_read_hi(dvalx),
				    c_dir);
				lo0 = vis_fmuld8ulx16(vis_read_hi(dvalx),
				    c_dir);
				hi1 = vis_fmuld8sux16(vis_read_lo(dvalx),
				    c_dir);
				lo1 = vis_fmuld8ulx16(vis_read_lo(dvalx),
				    c_dir);

				s0 = vis_fpadd32(hi0, lo0);
				s1 = vis_fpadd32(hi1, lo1);

				dz[i] = vis_bshuffle(s0, s1);
			}

			for (i = 0; i < (n & 3); i++) {
				z[n - i - 1] = c[0] * x[n - i - 1];
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16_Mod(
    mlib_s16 *xz,
    const mlib_s16 *c,
    mlib_s32 n)
{
	return (__mlib_VectorMulS_S16_S16_Mod(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S32_S16_Mod(
    mlib_s32 *z,
    const mlib_s16 *x,
    const mlib_s16 *c,
    mlib_s32 n)
{
	mlib_d64 *dpz, *dpx;
	mlib_d64 dx, dx0, dx1, dr, dr1;
	mlib_d64 dzh, dzl;
	mlib_s16 *px;
	mlib_s16 uc = *((mlib_s16 *)c);
	mlib_s32 *pz;
	mlib_d64 dy;
	mlib_s32 len = n, i;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s16 *)x;
	pz = (mlib_s32 *)z;

	if (n <= 8)
		MULS_S32_S16_IN_C;

/*
 * prepare the destination address
 */

	while ((mlib_addr)pz & 7) {
		((*pz++)) = ((mlib_s32)(*px)) * uc;
		px++;
		len--;
	}

	dpz = (mlib_d64 *)pz;
	even_8 = len >> 2;
	rest_8 = len & 0x3;
	dy = vis_to_double_dup((uc << 16) | ((uc) & 0xffff));

	if (!((mlib_addr)px & 7)) {

/*
 * Source address is 8-byte aligned.
 * No  vis_alignaddr and  vis_faligndata at all.
 */

		dpx = (mlib_d64 *)px;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = dpx[0];
			MUL_S16_S32;
/*
 * store 16 bytes of result
 */
			(*dpz++) = dzh;
			(*dpz++) = dzl;
			dpx++;
		}

	} else {

/*
 * "x" address is arbitrary aligned.
 */

		dpx = vis_alignaddr(px, 0);
		dx0 = vis_ld_d64_nf(dpx); dpx++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx);
			dx = vis_faligndata(dx0, dx1);
			MUL_S16_S32;
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

	if (!rest_8)
		return (MLIB_SUCCESS);

	px += (even_8 << 2);
	pz += (even_8 << 2);

	while (rest_8--) {
		((*pz++)) = ((mlib_s32)(*px)) * uc;
		px++;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S32_S32_Sat(
    mlib_s32 *z,
    const mlib_s32 *x,
    const mlib_s32 *c,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 scal = *((mlib_s32 *)c);

#pragma pipeloop(0)
	for (i = 0; i < n; i++)
		FLOAT2INT_CLAMP(((mlib_s32 *)z)[i], ((mlib_s32 *)x)[i] * scal);

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S32_Sat(
    mlib_s32 *xz,
    const mlib_s32 *c,
    mlib_s32 n)
{
	return (__mlib_VectorMulS_S32_S32_Sat(xz, xz, c, n));
}

/* *********************************************************** */

typedef union
{
	mlib_d64 d64;
	mlib_s64 s64;
	struct
	{
		mlib_s16 s0, s1, s2, s3;
	} four_short;
	struct
	{
		mlib_s32 s0, s1;
	} two_int;
} type_mlib_d64;

mlib_status
__mlib_VectorMulS_S32_S32_Mod(
    mlib_s32 *z,
    const mlib_s32 *x,
    const mlib_s32 *c,
    mlib_s32 n)
{
	mlib_s32 valc = *c, i;
	mlib_d64 valc_lo = valc & 0xFFFF;
	mlib_d64 valc_hi = (valc >> 16) & 0xFFFF;
	mlib_d64 valx;
	type_mlib_d64 ld0, ld1;

	vis_write_bmask(0x23456789, 0);
#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		valx = x[i];
		ld0.s64 = valx * valc_hi;
		ld1.d64 = vis_bshuffle(ld0.d64, vis_fzero());
		z[i] = ld1.s64 + (mlib_s64)(valx * valc_lo);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S32_Mod(
    mlib_s32 *xz,
    const mlib_s32 *c,
    mlib_s32 n)
{
	return (__mlib_VectorMulS_S32_S32_Mod(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_U8C_U8C_Sat(
    mlib_u8 *z,
    const mlib_u8 *x,
    const mlib_u8 *c,
    mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s32 shift = 7;
	mlib_s8 *pzend;
	mlib_u16 uc0 = ((mlib_u8 *)c)[0], uc1 = ((mlib_u8 *)c)[1];
	mlib_d64 *dpx, *dpz, *dpzend;
	mlib_d64 dx, dx0, dx1, dr, dr1, dr2, dr3, dr4, dr5, dr_;
	mlib_d64 st0;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!(uc0 | uc1))
		return (__mlib_VectorZero_U8C(z, n));

	if (uc1 == 0)
		return (__mlib_VectorMulS_U8_U8_Sat(z, x, c, n + n));

/*
 * prepare the destination address
 */
	pzend = (mlib_s8 *)z + n + n - 1;
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

	dpx++;
	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx + 1);

	while (((uc0 | uc1) & 1) == 0) {
		uc0 >>= 1;
		uc1 >>= 1;
		shift++;
	}

	if ((uc0 + uc1) <= MLIB_S8_MAX) {
/*
 * prepare the scaling factor
 */
		mlib_f32 fc = vis_to_float((uc0 << 24) | (uc1 << 8));

/* initialize GSR scale factor */
		vis_write_gsr(shift << 3);
		vis_alignaddr((void *)x, off);
		MULS_S16C_U8C_LO;

		if (!(1 & (mlib_addr)z)) {
			PACK_U8C0;
/* store first bytes of result */
			vis_pst_8(dr, dpz, emask);
			dpz++;
			vis_write_bmask(0x02461357, 0);
#pragma pipeloop(0)
			for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
				dx = vis_faligndata(dx0, dx1);
				dr3 = vis_bshuffle(dx, dx);
				dr4 = vis_fmul8x16au(vis_read_hi(dr3), fc);
				dr5 = vis_fmul8x16al(vis_read_hi(dr3), fc);
				dr2 = vis_fmul8x16au(vis_read_lo(dr3), fc);
				dr3 = vis_fmul8x16al(vis_read_lo(dr3), fc);
				dr = vis_fpsub16(dr4, dr3);
				dr1 = vis_fpadd16(dr5, dr2);
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dpx + 2);
				PACK_U8C0;
/* store 8 bytes of result */
				dpz[0] = dr;
				dpx++;
				dpz++;
			}

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				dx = vis_faligndata(dx0, dx1);
				dr3 = vis_bshuffle(dx, dx);
				dr4 = vis_fmul8x16au(vis_read_hi(dr3), fc);
				dr5 = vis_fmul8x16al(vis_read_hi(dr3), fc);
				dr2 = vis_fmul8x16au(vis_read_lo(dr3), fc);
				dr3 = vis_fmul8x16al(vis_read_lo(dr3), fc);
				dr = vis_fpsub16(dr4, dr3);
				dr1 = vis_fpadd16(dr5, dr2);
				PACK_U8C0;
/*
 * prepare edge mask for the last bytes
 */
				emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
				vis_pst_8(dr, dpz, emask);
			}

			return (MLIB_SUCCESS);
		}

		vis_write_bmask(0x78c9daeb, 0);

		off = ((mlib_addr)x + off) & 7;
		vis_alignaddr(dpx, off);

		dr_ = vis_fpack16_pair(dr, dr1);
		st0 = vis_bshuffle(dr_, dr_);
/* store first bytes of result */
		vis_pst_8(st0, dpz, emask & 0x7f);
		dpz++;

#pragma pipeloop(0)
		for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
			dx = vis_faligndata(dx0, dx1);
			MULS_S16C_U8C_LO;
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx + 2);
			dr = vis_fpack16_pair(dr, dr1);
			st0 = vis_bshuffle(dr_, dr);
			(*dpz++) = st0;
			dr_ = dr;
			dpx++;
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			st0 = vis_bshuffle(dr_, dr_);
			vis_pst_8(st0, dpz, 0x80);
		}

		if ((mlib_addr)dpz < (mlib_addr)pzend) {
			dx = vis_faligndata(dx0, dx1);
			MULS_S16C_U8C_LO;
			dr = vis_fpack16_pair(dr, dr1);
			st0 = vis_bshuffle(dr_, dr);
/*
 * prepare edge mask for the last bytes
 */
			emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
			vis_pst_8(st0, dpz, emask & 0x7f);
		}

		return (MLIB_SUCCESS);
	}

	{
		mlib_f32 fc0, fc1;
		mlib_s32 gsr_for_mul = (9 << 3) | 7;
		mlib_s64 gsr_for_pack;

		uc0 <<= shift;
		uc1 <<= shift;
		fc0 = vis_to_float((uc0 << 16) | uc0);
		fc1 = vis_to_float((uc1 << 16) | uc1);
/* initialize GSR for MULS_U8C_S16C */
		vis_write_gsr(gsr_for_mul);
		MULS_U8C_S16C;
		off = (off + (mlib_addr)x) & 7;
		gsr_for_pack = ((mlib_s64)0x78c9daeb << 32) | ((7 << 3) | off);
		vis_write_gsr64(gsr_for_pack);
		dx = vis_faligndata(dx0, dx1);

		if (!(1 & (mlib_addr)z)) {
			PACK_U8C0;
/* store first bytes of result */
			vis_pst_8(dr, dpz, emask);
			dpz++;
			vis_write_gsr(gsr_for_mul);
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx + 2);
			MULS_U8C_S16C;
			vis_write_gsr64(gsr_for_pack);

#pragma pipeloop(0)
			for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
				PACK_U8C0;
/* store 8 bytes of result */
				dpz[0] = dr;
				dx = vis_faligndata(dx0, dx1);
				vis_write_gsr(gsr_for_mul);
				MULS_U8C_S16C;
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dpx + 3);
				vis_write_gsr64(gsr_for_pack);
				dpx++;
				dpz++;
			}

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				PACK_U8C0;
				emask = vis_edge8(dpz, pzend);
				vis_pst_8(dr, dpz, emask);
			}

			return (MLIB_SUCCESS);
		}

		dr_ = vis_fpack16_pair(dr, dr1);
		st0 = vis_bshuffle(dr_, dr_);
/* store first bytes of result */
		vis_pst_8(st0, dpz, emask & 0x7f);
		dpz++;

#pragma pipeloop(0)
		for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
			vis_write_gsr(gsr_for_mul);
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx + 2);
			MULS_U8C_S16C;
			vis_write_gsr64(gsr_for_pack);
			dx = vis_faligndata(dx0, dx1);
			dr = vis_fpack16_pair(dr, dr1);
			st0 = vis_bshuffle(dr_, dr);
			(*dpz++) = st0;
			dr_ = dr;
			dpx++;
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			st0 = vis_bshuffle(dr_, dr_);
			vis_pst_8(st0, dpz, 0x80);
		}

		if ((mlib_addr)dpz < (mlib_addr)pzend) {
			vis_write_gsr(gsr_for_mul);
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx + 2);
			MULS_U8C_S16C;
			vis_write_gsr64(gsr_for_pack);
			dx = vis_faligndata(dx0, dx1);
			dr = vis_fpack16_pair(dr, dr1);
			st0 = vis_bshuffle(dr_, dr);
			emask = vis_edge8(dpz, pzend);
			vis_pst_8(st0, dpz, emask & 0x7f);
		}

		return (MLIB_SUCCESS);
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16C_U8C_Sat(
    mlib_s16 *z,
    const mlib_u8 *x,
    const mlib_u8 *c,
    mlib_s32 n)
{
	mlib_d64 *dpz, *dpx;
	mlib_d64 dx, dx0, dx1;
	mlib_d64 dr, dr1, dr2, dr3, dr4, dr5, dr6, dr7, dzh, dzl;
	mlib_u8 *px;
	mlib_s16 *pz;
	mlib_u16 uc0 = ((mlib_u8 *)c)[0], ucc0 = uc0, uc1 =
	    ((mlib_u8 *)c)[1], ucc1 = uc1;
/* odd = 1 means "aligned" resultant vector starts with Im. part. */
	mlib_s32 odd = 0;
	mlib_s32 len = n << 1, i;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_d64 dzero = vis_to_double_dup(0), rest;
	mlib_s32 re_x, im_x, re_c, im_c;
	mlib_f32 fc0, fc1;
	mlib_s32 gsr_for_mul = (9 << 3) | 7;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_u8 *)x;
	pz = (mlib_s16 *)z;

	if (n <= 4)
		MULS_S16C_U8C_SAT_IN_C;

	if (!(uc0 | uc1))

		return (__mlib_VectorZero_S16C(z, n));

/*
 * prepare the destination address trying to align it for 8 bytes.
 */

	while ((mlib_addr)pz & 7) {
		re_x = ((*px++));
		im_x = ((*px++));
		re_c = re_x * uc0 - im_x * uc1;
		im_c = im_x * uc0 + re_x * uc1;
		len -= 2;
		((*pz++)) =
		    re_c > MLIB_S16_MAX ? MLIB_S16_MAX : (re_c <
		    MLIB_S16_MIN ? MLIB_S16_MIN : re_c);
		im_c = im_c > MLIB_S16_MAX ? MLIB_S16_MAX : im_c;

		if (!((mlib_addr)pz & 7)) {
			odd = 1;
			rest = vis_to_double(0, im_c);
			break;
		}

		((*pz++)) = im_c;
	}

	dpz = (mlib_d64 *)pz;
	even_8 = len >> 3;
	rest_8 = len & 0x7;
	pz += (even_8 << 3);

	if ((uc0 + uc1) <= MLIB_S8_MAX) {

/*
 * For small Im and Re values of scale factor we can use very fast
 * multiplication algorithm: 8*16 = 16 without over- and underflow.
 */

		mlib_f32 fc = vis_to_float((uc0 << 24) | (uc1 << 8));

		if ((!((mlib_addr)px & 7))) {

/*
 * X address is 8-byte aligned.
 * No  vis_alignaddr and vis_faligndata.
 */

			dpx = (mlib_d64 *)px;

			if (!odd) {
/*
 * Start with real part.
 */
/* For multiplication. */
				vis_alignaddr((void *)4, 0);
				vis_write_bmask(0x018923ab, 0);
#pragma pipeloop(0)
				for (i = 0; i < even_8; i++) {
					dx = dpx[0];
					MULS_S16C_U8C_SHORT_EVEN;
					(*dpz++) = dzh;
					(*dpz++) = dzl;
					dpx++;
				}

			} else {
/*
 * Start with imaginary part
 */
/* For multiplication. */
				vis_alignaddr((void *)6, 0);
#pragma pipeloop(0)
				for (i = 0; i < even_8; i++) {
					dx = dpx[0];
					dr3 = vis_fpmerge(vis_read_hi(dx),
					    vis_read_lo(dx));
					dr3 = vis_fpmerge(vis_read_hi(dr3),
					    vis_read_lo(dr3));
					dr4 = vis_fmul8x16au(vis_read_hi(dr3),
					    fc);
					dr5 = vis_fmul8x16al(vis_read_hi(dr3),
					    fc);
					dr2 = vis_fmul8x16au(vis_read_lo(dr3),
					    fc);
					dr3 = vis_fmul8x16al(vis_read_lo(dr3),
					    fc);
					dr = vis_fpsub16(dr4, dr3);
					dr1 = vis_fpadd16(dr5, dr2);

					vis_write_bmask(0x018923ab, 0);
					dr3 = vis_bshuffle(dr, dr1);
					dzh = vis_faligndata(rest, dr3);
					vis_write_bmask(0x45cd67ef, 0);
					dr4 = vis_bshuffle(dr, dr1);
					dzl = vis_faligndata(dr3, dr4);
					rest = dr4;
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
			dx0 = vis_ld_d64_nf(dpx); dpx++;

			if (odd) {
				vis_write_bmask(0x23456789, 0);
#pragma pipeloop(0)
				for (i = 0; i < even_8; i++) {
					dx1 = vis_ld_d64_nf(dpx);
					dx = vis_faligndata(dx0, dx1);
					dr3 = vis_fpmerge(vis_read_hi(dx),
					    vis_read_lo(dx));
					dr3 = vis_fpmerge(vis_read_hi(dr3),
					    vis_read_lo(dr3));
					dr4 = vis_fmul8x16au(vis_read_hi(dr3),
					    fc);
					dr5 = vis_fmul8x16al(vis_read_hi(dr3),
					    fc);
					dr2 = vis_fmul8x16au(vis_read_lo(dr3),
					    fc);
					dr3 = vis_fmul8x16al(vis_read_lo(dr3),
					    fc);
					dr = vis_fpsub16(dr4, dr3);
					dr1 = vis_fpadd16(dr5, dr2);
					{
						mlib_d64 re2, re3, re4, im2,
						    im3, im4;
						re2 = vis_bshuffle(dr, dr);
						re3 = vis_bshuffle(re2, re2);
						re4 = vis_bshuffle(re3, re3);
						im2 = vis_bshuffle(dr1, dr1);
						im3 = vis_bshuffle(im2, im2);
						im4 = vis_bshuffle(im3, im3);
						dzh = rest;
						dzh = vis_bshuffle(dzh, dr);
						dzh = vis_bshuffle(dzh, dr1);
						dzh = vis_bshuffle(dzh, re2);
						dzl = vis_bshuffle(dzero, im2);
						dzl = vis_bshuffle(dzl, re3);
						dzl = vis_bshuffle(dzl, im3);
						dzl = vis_bshuffle(dzl, re4);
						rest = vis_bshuffle(dzero, im4);
					}

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
				vis_write_bmask(0x018923ab, 0);
#pragma pipeloop(0)
				for (i = 0; i < even_8; i++) {
					dx1 = vis_ld_d64_nf(dpx);
					dx = vis_faligndata(dx0, dx1);
					vis_alignaddr((void *)4, 0);
					MULS_S16C_U8C_SHORT_EVEN;
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

	} else {

/*
 * General case: 8*16 --> 32 --> 16 .
 */

		uc0 <<= 7;
		uc1 <<= 7;
		fc0 = vis_to_float((uc0 << 16) | uc0);
		fc1 = vis_to_float((uc1 << 16) | uc1);
/* initialize GSR for MULS_U8C_S16C */
		vis_write_gsr(gsr_for_mul);

		if ((!((mlib_addr)px & 7))) {

/*
 * X address is 8-byte aligned.
 * No  vis_alignaddr and vis_faligndata.
 */

			dpx = (mlib_d64 *)px;

			if (!odd) {
/*
 * Start with real part.
 */
/* initialize GSR for MULS_U8C_S16C */
				vis_alignaddr((void *)7, 0);
#pragma pipeloop(0)
				for (i = 0; i < even_8; i++) {
					dx = dpx[0];
					dr4 = vis_fmuld8ulx16(vis_read_hi(dx),
					    fc0);
					dr6 = vis_fmuld8ulx16(vis_read_hi(dx),
					    fc1);
					dr5 = vis_fmuld8ulx16(vis_read_lo(dx),
					    fc0);
					dr7 = vis_fmuld8ulx16(vis_read_lo(dx),
					    fc1);
					dr = vis_faligndata(dx, dx);
					dr1 = vis_fmuld8ulx16(vis_read_lo(dr),
					    fc0);
					dr3 = vis_fmuld8ulx16(vis_read_lo(dr),
					    fc1);
					dr2 = vis_fmuld8ulx16(vis_read_hi(dr),
					    fc1);
					dr = vis_fmuld8ulx16(vis_read_hi(dr),
					    fc0);
					dr = vis_fpsub32(dr, dr6);
					dr1 = vis_fpsub32(dr1, dr7);
					dr2 = vis_fpadd32(dr2, dr4);
					dr3 = vis_fpadd32(dr3, dr5);
					dr = vis_fpackfix_pair(dr, dr1);
					dr1 = vis_fpackfix_pair(dr2, dr3);
					vis_write_bmask(0x018923ab, 0);
					dzh = vis_bshuffle(dr, dr1);
					vis_write_bmask(0x45cd67ef, 0);
					dzl = vis_bshuffle(dr, dr1);
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
					dx = dpx[0];
/* initialize GSR for MULS_U8C_S16C */
					vis_alignaddr((void *)7, 0);
					dr4 = vis_fmuld8ulx16(vis_read_hi(dx),
					    fc0);
					dr6 = vis_fmuld8ulx16(vis_read_hi(dx),
					    fc1);
					dr5 = vis_fmuld8ulx16(vis_read_lo(dx),
					    fc0);
					dr7 = vis_fmuld8ulx16(vis_read_lo(dx),
					    fc1);
					dr = vis_faligndata(dx, dx);
					dr1 = vis_fmuld8ulx16(vis_read_lo(dr),
					    fc0);
					dr3 = vis_fmuld8ulx16(vis_read_lo(dr),
					    fc1);
					dr2 = vis_fmuld8ulx16(vis_read_hi(dr),
					    fc1);
					dr = vis_fmuld8ulx16(vis_read_hi(dr),
					    fc0);
					dr = vis_fpsub32(dr, dr6);
					dr1 = vis_fpsub32(dr1, dr7);
					dr2 = vis_fpadd32(dr2, dr4);
					dr3 = vis_fpadd32(dr3, dr5);
					dr = vis_fpackfix_pair(dr, dr1);
					dr1 = vis_fpackfix_pair(dr2, dr3);
					(void *)vis_alignaddr(dpx, 6);
					vis_write_bmask(0x018923ab, 0);
					dr3 = vis_bshuffle(dr, dr1);
					dzh = vis_faligndata(rest, dr3);
					vis_write_bmask(0x45cd67ef, 0);
					dr4 = vis_bshuffle(dr, dr1);
					dzl = vis_faligndata(dr3, dr4);
					rest = dr4;
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

			mlib_s32 off = (mlib_addr)px & 7, bmask;
			mlib_s64 gsr_for_mul_64;

			dpx = (mlib_d64 *)((mlib_u8 *)px - off);
			bmask = off * 0x11111111 + 0x01234567;
			gsr_for_mul_64 = ((mlib_s64)bmask << 32) | gsr_for_mul;
			dx0 = vis_ld_d64_nf(dpx); dpx++;
/* initialize GSR for MULS_U8C_S16C */
			vis_write_gsr64(gsr_for_mul_64);

			if (odd) {
#pragma pipeloop(0)
				for (i = 0; i < even_8; i++) {
					dx1 = vis_ld_d64_nf(dpx);
					dx = vis_bshuffle(dx0, dx1);
					MULS_S16C_U8C_FOR16_WITH_ODD;
/* initialize GSR for MULS_U8C_S16C */
					vis_write_gsr64(gsr_for_mul_64);
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
					dx = vis_bshuffle(dx0, dx1);
/* initialize GSR for MULS_U8C_S16C */
					vis_write_gsr64(gsr_for_mul_64);
					MULS_S16C_U8C_FOR16;
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
	}

	if (!rest_8)
		return (MLIB_SUCCESS);

	px += (even_8 << 3);

	while (rest_8 > 0) {
		re_x = ((*px++));
		im_x = ((*px++));
		re_c = re_x * ucc0 - im_x * ucc1;
		im_c = im_x * ucc0 + re_x * ucc1;
		((*pz++)) =
		    re_c > MLIB_S16_MAX ? MLIB_S16_MAX : (re_c <
		    MLIB_S16_MIN ? MLIB_S16_MIN : re_c);
		((*pz++)) = im_c > MLIB_S16_MAX ? MLIB_S16_MAX : im_c;
		rest_8 -= 2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_U8C_Sat(
    mlib_u8 *xz,
    const mlib_u8 *c,
    mlib_s32 n)
{
	return (__mlib_VectorMulS_U8C_U8C_Sat(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_U8C_U8C_Mod(
    mlib_u8 *z,
    const mlib_u8 *x,
    const mlib_u8 *c,
    mlib_s32 n)
{
	return __mlib_VectorMulS_S8C_S8C_Mod((mlib_s8 *)z, (mlib_s8 *)x,
	    (mlib_s8 *)c, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16C_U8C_Mod(
    mlib_s16 *z,
    const mlib_u8 *x,
    const mlib_u8 *c,
    mlib_s32 n)
{
	mlib_d64 *dpz, *dpx;
	mlib_d64 dx, dx0, dx1;
	mlib_d64 dr, dr1, dr2, dr3, dr4, dr5, dr6, dr7, dzh, dzl;
	mlib_u8 *px;
	mlib_s16 *pz;
	mlib_u16 uc0 = ((mlib_u8 *)c)[0], uc1 = ((mlib_u8 *)c)[1];

/* odd = 1 means "aligned" resultant vector starts with Im. part. */
	mlib_s32 odd = 0;
	mlib_s32 len = n, i;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_d64 dzero = vis_to_double_dup(0), rest;
	mlib_s32 re_x, im_x, re_c, im_c;
	mlib_f32 fc0, fc1;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_u8 *)x;
	pz = (mlib_s16 *)z;

	if (n <= 8)
		MULS_S16C_U8C_MOD_IN_C;

	if (!(uc0 | uc1))

		return (__mlib_VectorZero_S16C(z, n));

/*
 * prepare the destination address trying to align it for 8 bytes.
 */

	fc0 = vis_to_float((uc0 << 16) | uc0);
	fc1 = vis_to_float((uc1 << 16) | uc1);

	while ((mlib_addr)pz & 7) {
		re_x = ((*px++));
		im_x = ((*px++));
		re_c = re_x * uc0 - im_x * uc1;
		im_c = im_x * uc0 + re_x * uc1;
		len--;
		((*pz++)) = re_c;

		if (!((mlib_addr)pz & 7)) {
			odd = 1;
			rest = vis_to_double(0, im_c);
			break;
		}

		((*pz++)) = im_c;
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

		if (!odd) {
/*
 * Start with real part.
 */
/* initialize GSR */
			vis_alignaddr((void *)7, 0);
			vis_write_bmask(0x23ab67ef, 0);
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = dpx[0];
				MULS_S16C_U8C_FOR16_MOD;
				dzh = vis_bshuffle(dr, dr2);
				dzl = vis_bshuffle(dr1, dr3);
				(*dpz++) = dzh;
				(*dpz++) = dzl;
				dpx++;
			}

		} else {
/*
 * Start with imaginary part
 */
			vis_write_bmask(0x23ab67ef, 0);
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = dpx[0];
/* initialize GSR */
				vis_alignaddr((void *)7, 0);
				MULS_S16C_U8C_FOR16_MOD;
				dzh = vis_bshuffle(dr, dr2);
				dzl = vis_bshuffle(dr1, dr3);

				vis_alignaddr((void *)6, 0);
				dr = vis_faligndata(rest, dzh);
				rest = dzl;
				dzl = vis_faligndata(dzh, rest);
				dzh = dr;

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
		dx0 = vis_ld_d64_nf(dpx); dpx++;
		vis_write_bmask(0x23ab67ef, 0);

		if (odd) {
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
				dx = vis_faligndata(dx0, dx1);
/* initialize GSR */
				vis_alignaddr((void *)7, 0);
				MULS_S16C_U8C_FOR16_MOD;
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
				MULS_S16C_U8C_FOR16_MOD;
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

	if (!rest_8)
		return (MLIB_SUCCESS);

	px += (even_8 << 3);

	while (rest_8 > 0) {
		re_x = ((*px++));
		im_x = ((*px++));
		re_c = re_x * uc0 - im_x * uc1;
		im_c = im_x * uc0 + re_x * uc1;
		((*pz++)) = re_c;
		((*pz++)) = im_c;
		rest_8 -= 2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_U8C_Mod(
    mlib_u8 *xz,
    const mlib_u8 *c,
    mlib_s32 n)
{
	return (__mlib_VectorMulS_U8C_U8C_Mod(xz, xz, c, n));
}

/* *********************************************************** */

union table
{
	mlib_u16 sh[64];
	mlib_d64 db[16];
};

static union table mlib_mask16 = {
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0xffff,
	0x0000, 0x0000, 0xffff, 0x0000,
	0x0000, 0x0000, 0xffff, 0xffff,
	0x0000, 0xffff, 0x0000, 0x0000,
	0x0000, 0xffff, 0x0000, 0xffff,
	0x0000, 0xffff, 0xffff, 0x0000,
	0x0000, 0xffff, 0xffff, 0xffff,
	0xffff, 0x0000, 0x0000, 0x0000,
	0xffff, 0x0000, 0x0000, 0xffff,
	0xffff, 0x0000, 0xffff, 0x0000,
	0xffff, 0x0000, 0xffff, 0xffff,
	0xffff, 0xffff, 0x0000, 0x0000,
	0xffff, 0xffff, 0x0000, 0xffff,
	0xffff, 0xffff, 0xffff, 0x0000,
	0xffff, 0xffff, 0xffff, 0xffff
};

/* *********************************************************** */

#define	PACK8C0_SAT                                             \
	PACK8_SAT;                                              \
	dr = vis_fpmerge(vis_read_hi(dr), vis_read_lo(dr))

/* *********************************************************** */

#define	PACK8C1_SAT                                             \
	PACK8_SAT;                                              \
	dpx = (mlib_d64 *)vis_alignaddr(dpx, 7);                \
	dr1 = vis_faligndata(dr, dr);                           \
	dr1 = vis_fpmerge(vis_read_lo(dr1), vis_read_hi(dr))

/* *********************************************************** */

#define	OVER_MULS8C                                                      \
	{                                                                \
	    mlib_s32 mask_r, mask_i;                                     \
	    mlib_d64 mmm, dr2;                                           \
	                                                                 \
	    mask_r = vis_fcmpeq16(dr, dmax_r);                           \
	    mmm = mlib_mask16.db[mask_r];                                \
	    dr2 = vis_fand(displacement, mmm);                           \
	    dr = vis_fandnot(mmm, dr);                                   \
	    dr = vis_for(dr, dr2);                                       \
	    mask_i =                                                     \
		vis_fcmpeq16(dr1, dmax_i) | vis_fcmpeq16(dr1, dmax_r);   \
	    mmm = mlib_mask16.db[mask_i];                                \
	    dr2 = vis_fand(displacement, mmm);                           \
	    dr1 = vis_fandnot(mmm, dr1);                                 \
	    dr1 = vis_for(dr1, dr2);                                     \
	}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S8C_S8C_Sat(
    mlib_s8 *z,
    const mlib_s8 *x,
    const mlib_s8 *c,
    mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s8 *pzend = (mlib_s8 *)z + n + n - 1;
	mlib_d64 *dpx, *dpz, *dpzend;
	mlib_d64 dx, dx0, dx1, dr, dr1;
	mlib_d64 displacement = vis_to_double_dup(0x800080);
	mlib_d64 restore = vis_to_double_dup(0x80808080);
	mlib_u16 uc0 = ((mlib_u8 *)c)[0] << 8, uc1 = ((mlib_u8 *)c)[1] << 8;

/* prepare the scaling factor */
	mlib_d64 dc0 = vis_to_double_dup((uc0 << 16) | uc0);
	mlib_d64 dc1 = vis_to_double_dup((uc1 << 16) | uc1);

	if (n <= 0)
		return (MLIB_FAILURE);

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);

/*
 * prepare the destination address
 */
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z;

/*
 * generate edge mask for the start bytes
 */
	emask = vis_edge8(z, pzend);

/*
 * prepare the source address
 */
	dpx = (mlib_d64 *)vis_alignaddr((void *)x, off + (1 & (mlib_addr)z));
	dx0 = vis_ld_d64_nf(dpx);
	dx1 = vis_ld_d64_nf(dpx + 1);

	dx = vis_faligndata(dx0, dx1);
	dpx++;
	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx + 1);
	MULS8C;

	if (!(1 & (mlib_addr)z)) {
		if ((abs(((mlib_s8 *)c)[0]) + abs(((mlib_s8 *)c)[1])) >=
		    MLIB_U8_MAX) {
			mlib_d64 dmax_i = vis_to_double_dup(0x80008000);
			mlib_d64 dmax_r = vis_to_double_dup(0x7f807f80);

			OVER_MULS8C;
			PACK8C0_SAT;
/* store first bytes of result */
			vis_pst_8(dr, dpz, emask);
			dpz++;
			dx = vis_faligndata(dx0, dx1);

#pragma pipeloop(0)
			for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dpx + 2);
				MULS8C;
				OVER_MULS8C;
				PACK8C0_SAT;
/* store 8 bytes of result */
				dpz[0] = dr;
				dx = vis_faligndata(dx0, dx1);
				dpx++;
				dpz++;
			}

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				MULS8C;
				OVER_MULS8C;
				PACK8C0_SAT;
				emask = vis_edge8(dpz, pzend);
				vis_pst_8(dr, dpz, emask);
			}

			return (MLIB_SUCCESS);
		}

		PACK8C0_SAT;
/* store first bytes of result */
		vis_pst_8(dr, dpz, emask);
		dpz++;
		dx = vis_faligndata(dx0, dx1);

#pragma pipeloop(0)
		for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx + 2);
			MULS8C;
			PACK8C0_SAT;
/* store 8 bytes of result */
			dpz[0] = dr;
			dx = vis_faligndata(dx0, dx1);
			dpx++;
			dpz++;
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			MULS8C;
			PACK8C0_SAT;
/* prepare edge mask for the last bytes */
			emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
			vis_pst_8(dr, dpz, emask);
		}

		return (MLIB_SUCCESS);
	} else {
		off = ((mlib_addr)x + off + 1) & 7;

		if ((abs(((mlib_s8 *)c)[0]) + abs(((mlib_s8 *)c)[1])) >=
		    MLIB_U8_MAX) {
			mlib_d64 dmax_i = vis_to_double_dup(0x80008000);
			mlib_d64 dmax_r = vis_to_double_dup(0x7f807f80);

			OVER_MULS8C;
			PACK8C1_SAT;
/* store first bytes of result */
			vis_pst_8(dr1, dpz, emask & 0x7f);
			dpz++;

			if ((mlib_addr)dpz <= (mlib_addr)pzend)
				vis_st_u8_i(dr, dpz, 0);
			dpx = (mlib_d64 *)vis_alignaddr(dpx, off);
			dx = vis_faligndata(dx0, dx1);
			vis_write_bmask(0x30415263, 0);

#pragma pipeloop(0)
			for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dpx + 2);
				MULS8C;
				OVER_MULS8C;
				dr = vis_fpadd16(displacement, dr);
				dr1 = vis_fpadd16(displacement, dr1);
				dr = vis_fpack16_pair(dr, dr1);
				dr = vis_fxor(dr, restore);
				dr1 = vis_bshuffle(dr, dr);
/* store 7 bytes of result */
				vis_pst_8(dr1, dpz, 0x7f);
				dpz++;
				vis_st_u8_i(dr, dpz, 0);
				dx = vis_faligndata(dx0, dx1);
				dpx++;
			}

			if ((mlib_addr)dpz < (mlib_addr)pzend) {
				MULS8C;
				OVER_MULS8C;
				PACK8C1_SAT;
				emask = vis_edge8(dpz, pzend);
				vis_pst_8(dr1, dpz, emask & 0x7f);
			}

			return (MLIB_SUCCESS);
		}

		PACK8C1_SAT;
/* store first bytes of result */
		vis_pst_8(dr1, dpz, emask & 0x7f);
		dpz++;

		if ((mlib_addr)dpz <= (mlib_addr)pzend)
			vis_st_u8_i(dr, dpz, 0);
		dpx = (mlib_d64 *)vis_alignaddr(dpx, off);
		dx = vis_faligndata(dx0, dx1);
		vis_write_bmask(0x30415263, 0);

#pragma pipeloop(0)
		for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx + 2);
			MULS8C;
			dr = vis_fpadd16(displacement, dr);
			dr1 = vis_fpadd16(displacement, dr1);
			dr = vis_fpack16_pair(dr, dr1);
			dr = vis_fxor(dr, restore);
			dr1 = vis_bshuffle(dr, dr);
/* store 7 bytes of result */
			vis_pst_8(dr1, dpz, 0x7f);
			dpz++;
			vis_st_u8_i(dr, dpz, 0);
			dx = vis_faligndata(dx0, dx1);
			dpx++;
		}

		if ((mlib_addr)dpz < (mlib_addr)pzend) {
			MULS8C;
			PACK8C1_SAT;
/* prepare edge mask for the last bytes */
			emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
			vis_pst_8(dr1, dpz, emask & 0x7f);
		}

		return (MLIB_SUCCESS);
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16C_S8C_Sat(
    mlib_s16 *z,
    const mlib_s8 *x,
    const mlib_s8 *c,
    mlib_s32 n)
{
/*
 * prepare the scaling factor
 */
	mlib_s16 uc0 = ((mlib_s8 *)c)[0], uc1 = ((mlib_s8 *)c)[1];
	mlib_u16 ucc0 = ((mlib_u8 *)c)[0] << 8, ucc1 = ((mlib_u8 *)c)[1] << 8;

	mlib_s32 n2 = (n / 2) * 4, i_0, i_1, i_2, i_3, i;
	mlib_d64 c_re = c[0], c_im = c[1];
	mlib_d64 x_re, x_im, x_re_1, x_im_1;
	type_mlib_d64 str1, str2, dst;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!(ucc0 | ucc1))

		return (__mlib_VectorZero_S16C(z, n));

	if (!(uc0 == MLIB_S8_MIN && uc1 == MLIB_S8_MIN))

		return (__mlib_VectorMulS_S16C_S8C_Mod(z, x, c, n));

/* initialize GSR scale factor */
	vis_write_gsr(16 << 3);

	if (((mlib_addr)z & 7) == 0) {
#pragma pipeloop(0)
		for (i = 0; i < n2; i += 4) {

			x_re = x[i];
			x_im = x[i + 1];
			x_re_1 = x[i + 2];
			x_im_1 = x[i + 3];

			str1.two_int.s0 = (mlib_s32)(x_re * c_re - x_im * c_im);
			str1.two_int.s1 = (mlib_s32)(x_re * c_im + x_im * c_re);

			str2.two_int.s0 =
			    (mlib_s32)(x_re_1 * c_re - x_im_1 * c_im);
			str2.two_int.s1 =
			    (mlib_s32)(x_re_1 * c_im + x_im_1 * c_re);
			dst.d64 = vis_fpackfix_pair(str1.d64, str2.d64);

			*((mlib_d64 *)(z + i)) = dst.d64;
		}

	} else {
#pragma pipeloop(0)
		for (i_0 = 0, i_1 = 1, i_2 = 2, i_3 = 3; i_0 < n2;
		    i_0 += 4, i_1 += 4, i_2 += 4, i_3 += 4) {

			x_re = x[i_0];
			x_im = x[i_1];
			x_im_1 = x[i_3];
			x_re_1 = x[i_2];

			str1.two_int.s0 = (mlib_s32)(x_re * c_re - x_im * c_im);
			str1.two_int.s1 = (mlib_s32)(x_re * c_im + x_im * c_re);

			str2.two_int.s0 =
			    (mlib_s32)(x_re_1 * c_re - x_im_1 * c_im);
			str2.two_int.s1 =
			    (mlib_s32)(x_re_1 * c_im + x_im_1 * c_re);
			dst.d64 = vis_fpackfix_pair(str1.d64, str2.d64);

			z[i_0] = dst.four_short.s0;
			z[i_1] = dst.four_short.s1;
			z[i_2] = dst.four_short.s2;
			z[i_3] = dst.four_short.s3;
		}
	}

	if (n % 2) {
		mlib_s32 im, re;

		re = x[2 * (n - 1)] * c_re - x[2 * (n - 1) + 1] * c_im;
		im = x[2 * (n - 1)] * c_im + x[2 * (n - 1) + 1] * c_re;

		z[2 * (n - 1)] =
		    (re > MLIB_S16_MAX) ? MLIB_S16_MAX : ((re <
		    MLIB_S16_MIN) ? MLIB_S16_MIN : re);
		z[2 * (n - 1) + 1] =
		    (im > MLIB_S16_MAX) ? MLIB_S16_MAX : ((im <
		    MLIB_S16_MIN) ? MLIB_S16_MIN : im);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S8C_Sat(
    mlib_s8 *xz,
    const mlib_s8 *c,
    mlib_s32 n)
{
	return (__mlib_VectorMulS_S8C_S8C_Sat(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S8C_S8C_Mod(
    mlib_s8 *z,
    const mlib_s8 *x,
    const mlib_s8 *c,
    mlib_s32 n)
{
	mlib_s32 n4 = n >> 2, i;
	mlib_f32 c_re = vis_to_float(c[0] << 8), c_im = vis_to_float(c[1] << 8);
	mlib_d64 dvalx, *dx = (mlib_d64 *)x, *dz = (mlib_d64 *)z;
	mlib_d64 re_re, im_im, re_im, im_re, re, im;
	mlib_d64 valc_re = c[0], valc_im = c[1], dx_re, dx_im;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!((mlib_addr)z & 7) && !((mlib_addr)x & 7)) {

		vis_write_bmask(0x02461357, 0);
#pragma pipeloop(0)
		for (i = 0; i < n4; i++) {
			dvalx = dx[i];
			dvalx = vis_bshuffle(dvalx, dvalx);

			re_re = vis_fmul8x16al(vis_read_hi(dvalx), c_re);
			im_im = vis_fmul8x16al(vis_read_lo(dvalx), c_im);
			re = vis_fpsub16(re_re, im_im);

			re_im = vis_fmul8x16al(vis_read_hi(dvalx), c_im);
			im_re = vis_fmul8x16al(vis_read_lo(dvalx), c_re);
			im = vis_fpadd16(re_im, im_re);

			re = vis_fpack32(re, 0);

			vis_pst_8(im, dz + i, 0x55);
			vis_pst_8(re, dz + i, 0xAA);
		}

		if (n % 4) {
			for (i = 0; i < n % 4; i++) {
				dx_re = x[2 * (n - i - 1)];
				dx_im = x[2 * (n - i - 1) + 1];
				z[2 * (n - i - 1)] =
				    dx_re * valc_re - dx_im * valc_im;
				z[2 * (n - i - 1) + 1] =
				    dx_re * valc_im + dx_im * valc_re;
			}
		}

	} else {

/* edge masks */
		mlib_s32 emask;

/* offset of address alignment in destination */
		mlib_s32 off;
		mlib_s8 *pzend = (mlib_s8 *)z + n + n - 1;
		mlib_d64 *dpx, *dpz, *dpzend;
		mlib_d64 dx, dx0, dx1, dr, dr1;
		mlib_u16 uc0 = ((mlib_u8 *)c)[0] << 8, uc1 =
		    ((mlib_u8 *)c)[1] << 8;
/* prepare the scaling factor */
		mlib_d64 dc0 = vis_to_double_dup((uc0 << 16) | uc0);
		mlib_d64 dc1 = vis_to_double_dup((uc1 << 16) | uc1);

		if (n <= 0)
			return (MLIB_FAILURE);

/* initialize GSR scale factor */
		vis_write_gsr(7 << 3);

/*
 * prepare the destination address
 */
		dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
		dpz = (mlib_d64 *)((mlib_addr)z & (~7));
		off = (mlib_addr)dpz - (mlib_addr)z;

/*
 * generate edge mask for the start bytes
 */
		emask = vis_edge8(z, pzend);

/*
 * prepare the source address
 */
		dpx = (mlib_d64 *)vis_alignaddr((void *)x,
		    off + (1 & (mlib_addr)z));
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);

		dx = vis_faligndata(dx0, dx1);
		dpx++;
		dx0 = dx1;
		dx1 = vis_ld_d64_nf(dpx + 1);
		MULS8C;

		if (!(1 & (mlib_addr)z)) {
			vis_write_bmask(0x193b5d7f, 0);
			dr = vis_bshuffle(dr, dr1);
/* store first bytes of result */
			vis_pst_8(dr, dpz, emask);
			dpz++;
			dx = vis_faligndata(dx0, dx1);

			if ((((mlib_addr)x ^ (mlib_addr)z) & 7) == 0)
#pragma pipeloop(0)
				for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
					dx1 = vis_ld_d64_nf(dpx + 1);
					MULS8C;
					dpz[0] = vis_bshuffle(dr, dr1);
					dx = dx1;
					dpx++;
					dpz++;
			} else
#pragma pipeloop(0)
				for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
					dx0 = dx1;
					dx1 = vis_ld_d64_nf(dpx + 2);
					MULS8C;
					dpz[0] = vis_bshuffle(dr, dr1);
					dx = vis_faligndata(dx0, dx1);
					dpx++;
					dpz++;
				}

			if ((mlib_addr)dpz <= (mlib_addr)pzend) {
				MULS8C;
				dr = vis_bshuffle(dr, dr1);
/* prepare edge mask for the last bytes */
				emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
				vis_pst_8(dr, dpz, emask);
			}

		} else {
			vis_write_bmask(0xf193b5d7, 0);
			off = ((mlib_addr)x + off + 1) & 7;
			dr = vis_bshuffle(dr, dr1);
/* store first bytes of result */
			vis_pst_8(dr, dpz, emask & 0x7f);
			dpz++;

			if ((mlib_addr)dpz <= (mlib_addr)pzend)
				vis_pst_8(dr, dpz, 0x80);
			dpx = (mlib_d64 *)vis_alignaddr(dpx, off);
			dx = vis_faligndata(dx0, dx1);
#pragma pipeloop(0)
			for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
				dx0 = dx1;
				dx1 = vis_ld_d64_nf(dpx + 2);
				MULS8C;
				dr = vis_bshuffle(dr, dr1);
/* store 7 bytes of result */
				vis_pst_8(dr, dpz, 0x7f);
				dpz++;
				vis_pst_8(dr, dpz, 0x80);
				dx = vis_faligndata(dx0, dx1);
				dpx++;
			}

			if ((mlib_addr)dpz < (mlib_addr)pzend) {
				MULS8C;
				dr = vis_bshuffle(dr, dr1);
/* prepare edge mask for the last bytes */
				emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
				vis_pst_8(dr, dpz, emask & 0x7f);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16C_S8C_Mod(
    mlib_s16 *z,
    const mlib_s8 *x,
    const mlib_s8 *c,
    mlib_s32 n)
{
	mlib_d64 *dpz, *dpx;
	mlib_d64 dx, dx0, dx1;
	mlib_d64 dr, dr1, dr2, dr3, dzh, dzl;
	mlib_s8 *px;
	mlib_s16 *pz;

/* odd = 1 means "aligned" resultant vector starts with Im. part. */
	mlib_s32 odd = 0;
	mlib_s32 len = n, i;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_s32 re_x, im_x, re_y, im_y, re_c, im_c;
	mlib_d64 dzero = vis_to_double_dup(0), rest;

/*
 * prepare the scaling factor
 */
	mlib_u16 sc0 = ((mlib_s8 *)c)[0], sc1 = ((mlib_s8 *)c)[1];
	mlib_u16 uc0 = ((mlib_u8 *)c)[0], uc1 = ((mlib_u8 *)c)[1];
	mlib_u16 ucc0 = uc0 << 8, ucc1 = uc1 << 8;
	mlib_d64 dc0 = vis_to_double_dup((ucc0 << 16) | ucc0);
	mlib_d64 dc1 = vis_to_double_dup((ucc1 << 16) | ucc1);

	if (n <= 0)
		return (MLIB_FAILURE);

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);
	px = (mlib_s8 *)x;
	pz = (mlib_s16 *)z;

	if (n <= 8)
		MULS_S16C_S8C_MOD_IN_C;

	if (!(ucc0 | ucc1))

		return (__mlib_VectorZero_S16C(z, n));

/*
 * prepare the destination address trying to align it for 8 bytes.
 */

	while ((mlib_addr)pz & 7) {
		re_x = ((*px++));
		im_x = ((*px++));
		re_y = sc0;
		im_y = sc1;
		re_c = re_x * re_y - im_x * im_y;
		im_c = im_x * re_y + re_x * im_y;
		len--;
		((*pz++)) = re_c;

		if (!((mlib_addr)pz & 7)) {
			odd = 1;
			rest = vis_to_double(0, im_c);
			break;
		}

		((*pz++)) = im_c;
	}

	dpz = (mlib_d64 *)pz;
	len <<= 1;
	even_8 = len >> 3;
	rest_8 = len & 0x7;
	pz += (even_8 << 3);

	if ((!((mlib_addr)px & 7))) {

/*
 * 'x' address ais 8-byte aligned.
 * No  vis_alignaddr and  vis_faligndata at all.
 */

		dpx = (mlib_d64 *)px;

		if (!odd) {
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = dpx[0];
				MULS8C;
				vis_write_bmask(0x018923ab, 0);
				dzh = vis_bshuffle(dr, dr1);
				vis_write_bmask(0x45cd67ef, 0);
				dzl = vis_bshuffle(dr, dr1);
				(*dpz++) = dzh;
				(*dpz++) = dzl;
				dpx++;
			}

		} else {
			vis_alignaddr((void *)6, 0);
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = dpx[0];
				MULS8C;

				vis_write_bmask(0x018923ab, 0);
				dr2 = vis_bshuffle(dr, dr1);
				dzh = vis_faligndata(rest, dr2);
				vis_write_bmask(0x45cd67ef, 0);
				dr3 = vis_bshuffle(dr, dr1);
				dzl = vis_faligndata(dr2, dr3);
				rest = dr3;
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
 * Both ("x" and "y") address are arbitrary aligned.
 * 2 vis_alignaddr(s) and 2 vis_faligndata(s) in the loop.
 */

		dpx = vis_alignaddr(px, 0);
		dx0 = vis_ld_d64_nf(dpx); dpx++;

		if (odd) {
			vis_write_bmask(0x23456789, 0);
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				vis_alignaddr(px, 0);
				dx1 = vis_ld_d64_nf(dpx);
				dx = vis_faligndata(dx0, dx1);
				MULS8C;
				{
					mlib_d64 re2, re3, re4, im2, im3, im4;

					re2 = vis_bshuffle(dr, dr);
					re3 = vis_bshuffle(re2, re2);
					re4 = vis_bshuffle(re3, re3);
					im2 = vis_bshuffle(dr1, dr1);
					im3 = vis_bshuffle(im2, im2);
					im4 = vis_bshuffle(im3, im3);
					dzh = rest;
					dzh = vis_bshuffle(dzh, dr);
					dzh = vis_bshuffle(dzh, dr1);
					dzh = vis_bshuffle(dzh, re2);
					dzl = vis_bshuffle(dzero, im2);
					dzl = vis_bshuffle(dzl, re3);
					dzl = vis_bshuffle(dzl, im3);
					dzl = vis_bshuffle(dzl, re4);
					rest = vis_bshuffle(dzero, im4);
				};
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
				MULS8C;
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

	if (!rest_8)
		return (MLIB_SUCCESS);

	px += (even_8 << 3);

	while (rest_8 > 0) {
		re_x = ((*px++));
		im_x = ((*px++));
		re_y = sc0;
		im_y = sc1;
		re_c = re_x * re_y - im_x * im_y;
		im_c = im_x * re_y + re_x * im_y;
		((*pz++)) = re_c;
		((*pz++)) = im_c;
		rest_8 -= 2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S8C_Mod(
    mlib_s8 *xz,
    const mlib_s8 *c,
    mlib_s32 n)
{
	return (__mlib_VectorMulS_S8C_S8C_Mod(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16C_S16C_Sat(
    mlib_s16 *z,
    const mlib_s16 *x,
    const mlib_s16 *c,
    mlib_s32 n)
{
	mlib_s32 n2 = (n / 2) * 4, i;

	mlib_f32 c_dir, c_rev, *x_f32 = (mlib_f32 *)x, x_dir0, x_dir1;
	mlib_d64 hi0, lo0, hi1, lo1, hi2, lo2, hi3, lo3, src1, src2;
	mlib_d64 Re_Re_Im_Im0, Re_Im_Re_Im0, Re_Re_Im_Im1, Re_Im_Re_Im1, *x_d64,
	    val_x_d64;
	mlib_f32 Re0, Im0, Re1, Im1;
	type_mlib_d64 dst, sr0, sr1;

	if (n <= 0)
		return (MLIB_FAILURE);

/* initialize GSR scale factor */
	vis_write_gsr(16 << 3);

	if ((((mlib_addr)x & 3) == 0) && !(c[0] == MLIB_S16_MIN &&
	    c[1] == MLIB_S16_MIN)) {
		c_dir = vis_to_float((c[0] << 16) | (c[1] & 0xffff));
		c_rev = vis_to_float((c[1] << 16) | (c[0] & 0xffff));

		if (((mlib_addr)z & 7) == 0) {

#pragma pipeloop(0)
			for (i = 0; i < n2; i += 4, x_f32 += 2) {

				x_dir0 = x_f32[0];
				x_dir1 = x_f32[1];

				hi0 = vis_fmuld8sux16(x_dir0, c_dir);
				lo0 = vis_fmuld8ulx16(x_dir0, c_dir);
				hi1 = vis_fmuld8sux16(x_dir0, c_rev);
				lo1 = vis_fmuld8ulx16(x_dir0, c_rev);
				Re_Re_Im_Im0 = vis_fpadd32(hi0, lo0);
				Re_Im_Re_Im0 = vis_fpadd32(hi1, lo1);

				hi2 = vis_fmuld8sux16(x_dir1, c_dir);
				lo2 = vis_fmuld8ulx16(x_dir1, c_dir);
				hi3 = vis_fmuld8sux16(x_dir1, c_rev);
				lo3 = vis_fmuld8ulx16(x_dir1, c_rev);
				Re_Re_Im_Im1 = vis_fpadd32(hi2, lo2);
				Re_Im_Re_Im1 = vis_fpadd32(hi3, lo3);

				Re0 = vis_fpsub32s(vis_read_hi(Re_Re_Im_Im0),
				    vis_read_lo(Re_Re_Im_Im0));
				Re1 = vis_fpsub32s(vis_read_hi(Re_Re_Im_Im1),
				    vis_read_lo(Re_Re_Im_Im1));

				Im0 = vis_fpadd32s(vis_read_hi(Re_Im_Re_Im0),
				    vis_read_lo(Re_Im_Re_Im0));
				Im1 = vis_fpadd32s(vis_read_hi(Re_Im_Re_Im1),
				    vis_read_lo(Re_Im_Re_Im1));

				src1 = vis_freg_pair(Re0, Im0);
				src2 = vis_freg_pair(Re1, Im1);

				dst.d64 = vis_fpackfix_pair(src1, src2);

				*((mlib_d64 *)(z + i)) = dst.d64;
			}

		} else {

#pragma pipeloop(0)
			for (i = 0; i < n2; i += 4, x_f32 += 2) {

				x_dir0 = x_f32[0];
				x_dir1 = x_f32[1];

				hi0 = vis_fmuld8sux16(x_dir0, c_dir);
				lo0 = vis_fmuld8ulx16(x_dir0, c_dir);
				hi1 = vis_fmuld8sux16(x_dir0, c_rev);
				lo1 = vis_fmuld8ulx16(x_dir0, c_rev);
				Re_Re_Im_Im0 = vis_fpadd32(hi0, lo0);
				Re_Im_Re_Im0 = vis_fpadd32(hi1, lo1);

				hi2 = vis_fmuld8sux16(x_dir1, c_dir);
				lo2 = vis_fmuld8ulx16(x_dir1, c_dir);
				hi3 = vis_fmuld8sux16(x_dir1, c_rev);
				lo3 = vis_fmuld8ulx16(x_dir1, c_rev);
				Re_Re_Im_Im1 = vis_fpadd32(hi2, lo2);
				Re_Im_Re_Im1 = vis_fpadd32(hi3, lo3);

				Re0 = vis_fpsub32s(vis_read_hi(Re_Re_Im_Im0),
				    vis_read_lo(Re_Re_Im_Im0));
				Re1 = vis_fpsub32s(vis_read_hi(Re_Re_Im_Im1),
				    vis_read_lo(Re_Re_Im_Im1));

				Im0 = vis_fpadd32s(vis_read_hi(Re_Im_Re_Im0),
				    vis_read_lo(Re_Im_Re_Im0));
				Im1 = vis_fpadd32s(vis_read_hi(Re_Im_Re_Im1),
				    vis_read_lo(Re_Im_Re_Im1));

				src1 = vis_freg_pair(Re0, Im0);
				src2 = vis_freg_pair(Re1, Im1);

				dst.d64 = vis_fpackfix_pair(src1, src2);

				z[i] = dst.four_short.s0;
				z[i + 1] = dst.four_short.s1;
				z[i + 2] = dst.four_short.s2;
				z[i + 3] = dst.four_short.s3;
			}
		}

		if (n & 1) {
			mlib_d64 im, re;

			re = x[2 * (n - 1)] * c[0] - x[2 * (n - 1) + 1] * c[1];
			im = x[2 * (n - 1)] * c[1] + x[2 * (n - 1) + 1] * c[0];

			z[2 * (n - 1)] =
			    (re > MLIB_S16_MAX) ? MLIB_S16_MAX : ((re <
			    MLIB_S16_MIN) ? MLIB_S16_MIN : re);
			z[2 * (n - 1) + 1] =
			    (im > MLIB_S16_MAX) ? MLIB_S16_MAX : ((im <
			    MLIB_S16_MIN) ? MLIB_S16_MIN : im);
		}

	} else {

		if (c[0] == MLIB_S16_MIN && c[1] == MLIB_S16_MIN) {
			mlib_d64 mulc = MLIB_S16_MIN;

#pragma pipeloop(0)
			for (i = 0; i < n2; i += 4) {
				mlib_d64 x_re, x_im, x_re_1, x_im_1;

				x_re = x[i];
				x_im = x[i + 1];
				x_re_1 = x[i + 2];
				x_im_1 = x[i + 3];

				sr0.two_int.s0 =
				    (mlib_s32)((x_re - x_im) * mulc);
				sr0.two_int.s1 =
				    (mlib_s32)((x_re + x_im) * mulc);
				sr1.two_int.s0 =
				    (mlib_s32)((x_re_1 - x_im_1) * mulc);
				sr1.two_int.s1 =
				    (mlib_s32)((x_re_1 + x_im_1) * mulc);
				dst.d64 = vis_fpackfix_pair(sr0.d64, sr1.d64);

				z[i] = dst.four_short.s0;
				z[i + 1] = dst.four_short.s1;
				z[i + 2] = dst.four_short.s2;
				z[i + 3] = dst.four_short.s3;
			}

			if (n & 1) {
				mlib_s32 im, re;

				re = x[2 * (n - 1)] - x[2 * (n - 1) + 1];
				im = x[2 * (n - 1)] + x[2 * (n - 1) + 1];

				z[2 * (n - 1)] =
				    (re < 0) ? MLIB_S16_MAX : ((re >
				    0) ? MLIB_S16_MIN : 0);
				z[2 * (n - 1) + 1] =
				    (im < 0) ? MLIB_S16_MAX : ((im >
				    0) ? MLIB_S16_MIN : 0);
			}

		} else {
			c_dir = vis_to_float((c[0] << 16) | (c[1] & 0xffff));
			c_rev = vis_to_float((c[1] << 16) | (c[0] & 0xffff));

			x_d64 = vis_alignaddr((void *)x, 0);

			if (((mlib_addr)z & 7) == 0) {
#pragma pipeloop(0)
				for (i = 0; i < n2; i += 4, x_d64++) {

					val_x_d64 =
					    vis_faligndata(vis_ld_d64_nf(x_d64),
						vis_ld_d64_nf(x_d64 + 1));
					x_dir0 = vis_read_hi(val_x_d64);
					x_dir1 = vis_read_lo(val_x_d64);

					hi0 = vis_fmuld8sux16(x_dir0, c_dir);
					lo0 = vis_fmuld8ulx16(x_dir0, c_dir);
					hi1 = vis_fmuld8sux16(x_dir0, c_rev);
					lo1 = vis_fmuld8ulx16(x_dir0, c_rev);

					Re_Re_Im_Im0 = vis_fpadd32(hi0, lo0);
					Re_Im_Re_Im0 = vis_fpadd32(hi1, lo1);

					hi2 = vis_fmuld8sux16(x_dir1, c_dir);
					lo2 = vis_fmuld8ulx16(x_dir1, c_dir);
					hi3 = vis_fmuld8sux16(x_dir1, c_rev);
					lo3 = vis_fmuld8ulx16(x_dir1, c_rev);

					Re_Re_Im_Im1 = vis_fpadd32(hi2, lo2);
					Re_Im_Re_Im1 = vis_fpadd32(hi3, lo3);

					Re0 = vis_fpsub32s(vis_read_hi
					    (Re_Re_Im_Im0),
					    vis_read_lo(Re_Re_Im_Im0));
					Re1 = vis_fpsub32s(vis_read_hi
					    (Re_Re_Im_Im1),
					    vis_read_lo(Re_Re_Im_Im1));

					Im0 = vis_fpadd32s(vis_read_hi
					    (Re_Im_Re_Im0),
					    vis_read_lo(Re_Im_Re_Im0));
					Im1 = vis_fpadd32s(vis_read_hi
					    (Re_Im_Re_Im1),
					    vis_read_lo(Re_Im_Re_Im1));

					src1 = vis_freg_pair(Re0, Im0);
					src2 = vis_freg_pair(Re1, Im1);

					dst.d64 = vis_fpackfix_pair(src1, src2);

					*((mlib_d64 *)(z + i)) = dst.d64;
				}

			} else {
#pragma pipeloop(0)
				for (i = 0; i < n2; i += 4, x_d64++) {

					val_x_d64 =
					    vis_faligndata(vis_ld_d64_nf(x_d64),
						vis_ld_d64_nf(x_d64 + 1));
					x_dir0 = vis_read_hi(val_x_d64);
					x_dir1 = vis_read_lo(val_x_d64);

					hi0 = vis_fmuld8sux16(x_dir0, c_dir);
					lo0 = vis_fmuld8ulx16(x_dir0, c_dir);
					hi1 = vis_fmuld8sux16(x_dir0, c_rev);
					lo1 = vis_fmuld8ulx16(x_dir0, c_rev);

					Re_Re_Im_Im0 = vis_fpadd32(hi0, lo0);
					Re_Im_Re_Im0 = vis_fpadd32(hi1, lo1);

					hi2 = vis_fmuld8sux16(x_dir1, c_dir);
					lo2 = vis_fmuld8ulx16(x_dir1, c_dir);
					hi3 = vis_fmuld8sux16(x_dir1, c_rev);
					lo3 = vis_fmuld8ulx16(x_dir1, c_rev);

					Re_Re_Im_Im1 = vis_fpadd32(hi2, lo2);
					Re_Im_Re_Im1 = vis_fpadd32(hi3, lo3);

					Re0 = vis_fpsub32s(vis_read_hi
					    (Re_Re_Im_Im0),
					    vis_read_lo(Re_Re_Im_Im0));
					Re1 = vis_fpsub32s(vis_read_hi
					    (Re_Re_Im_Im1),
					    vis_read_lo(Re_Re_Im_Im1));

					Im0 = vis_fpadd32s(vis_read_hi
					    (Re_Im_Re_Im0),
					    vis_read_lo(Re_Im_Re_Im0));
					Im1 = vis_fpadd32s(vis_read_hi
					    (Re_Im_Re_Im1),
					    vis_read_lo(Re_Im_Re_Im1));

					src1 = vis_freg_pair(Re0, Im0);
					src2 = vis_freg_pair(Re1, Im1);

					dst.d64 = vis_fpackfix_pair(src1, src2);

					z[i] = dst.four_short.s0;
					z[i + 1] = dst.four_short.s1;
					z[i + 2] = dst.four_short.s2;
					z[i + 3] = dst.four_short.s3;
				}
			}

			if (n & 1) {
				mlib_d64 im, re;

				re = x[2 * (n - 1)] * (mlib_d64)c[0] -
				    x[2 * (n - 1) + 1] * (mlib_d64)c[1];
				im = x[2 * (n - 1)] * (mlib_d64)c[1] +
				    x[2 * (n - 1) + 1] * (mlib_d64)c[0];

				z[2 * (n - 1)] =
				    (re >
				    MLIB_S16_MAX) ? MLIB_S16_MAX : ((re <
				    MLIB_S16_MIN) ? MLIB_S16_MIN : re);
				z[2 * (n - 1) + 1] =
				    (im >
				    MLIB_S16_MAX) ? MLIB_S16_MAX : ((im <
				    MLIB_S16_MIN) ? MLIB_S16_MIN : im);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S32C_S16C_Sat(
    mlib_s32 *z,
    const mlib_s16 *x,
    const mlib_s16 *c,
    mlib_s32 n)
{
	mlib_d64 *dpz, *dpx;
	mlib_d64 dx, dx0, dx1;
	mlib_d64 dr, dr1, dr2, dr3, dr4, dr5, dzh, dzl, drest;
	mlib_s16 *px;
	mlib_s32 *pz;

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
	mlib_s16 sc0 = ((mlib_s16 *)c)[0], sc1 = ((mlib_s16 *)c)[1];
	mlib_u16 uc0 = ((mlib_u16 *)c)[0], uc1 = ((mlib_u16 *)c)[1];
	mlib_f32 fc0 = vis_to_float((uc0 << 16) | uc1);
	mlib_f32 fc1 = vis_to_float((uc1 << 16) | uc0);
	mlib_d64 cntr_ovlp = vis_to_double_dup(0x80000000);
	mlib_d64 const_max_32 = vis_to_double_dup(MLIB_S32_MAX);
	mlib_s32 mask, mask1;
	mlib_s32 re_x, im_x, re_y, im_y, re_c, im_c;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!(uc0 | uc1))

		return (__mlib_VectorZero_S32C(z, n));

	px = (mlib_s16 *)x;
	pz = (mlib_s32 *)z;

	if (n <= 4)
		MULS_S32C_S16C_SAT_IN_C;

	if (!(sc0 == MLIB_S16_MIN && sc1 == MLIB_S16_MIN))

		return (__mlib_VectorMulS_S32C_S16C_Mod(z, x, c, n));

/*
 * prepare the destination address trying to align it for 8 bytes.
 */

	if ((mlib_addr)pz & 7) {
		re_x = ((*px++));
		im_x = ((*px++));
		re_y = sc0;
		im_y = sc1;
		re_c = re_x * re_y - im_x * im_y;
		im_c = im_x * re_y + re_x * im_y;
		im_c = (im_c == 0x80000000 ? MLIB_S32_MAX : im_c);
		len -= 2;
		((*pz++)) = re_c;
		odd = 1;
		rest = vis_to_float(im_c);
	}

	dpz = (mlib_d64 *)pz;
	even_8 = len >> 2;
	rest_8 = len & 0x3;
	pz += (even_8 << 2);

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
				MULS_S16C_S32C;
				COMPOSE_S32C_FROM32_SAT;
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
				dx = dpx[0];
				MULS_S16C_S32C;
				COMPOSE_S32C_FROM32_SAT_WITH_ODD;
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
			mask = vis_fcmpeq32(drest, cntr_ovlp);
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

		if (odd) {
/*
 * Start with imaginary part.
 */
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
				dx = vis_faligndata(dx0, dx1);
				MULS_S16C_S32C;
				COMPOSE_S32C_FROM32_SAT_WITH_ODD;
				dx0 = dx1;
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
			mask = vis_fcmpeq32(drest, cntr_ovlp);
			vis_pst_32(drest, pz, 0x2);
			vis_pst_32(const_max_32, pz, mask);
			pz++;
		} else {
/*
 * Start with real part.
 */
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
				dx = vis_faligndata(dx0, dx1);
				MULS_S16C_S32C;
				COMPOSE_S32C_FROM32_SAT;
				dx0 = dx1;
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

	if (!rest_8)
		return (MLIB_SUCCESS);

	px += (even_8 << 2);

	while (rest_8 > 0) {
		re_x = ((*px++));
		im_x = ((*px++));
		re_y = sc0;
		im_y = sc1;
		re_c = re_x * re_y - im_x * im_y;
		im_c = im_x * re_y + re_x * im_y;
		im_c = (im_c == 0x80000000 ? MLIB_S32_MAX : im_c);
		((*pz++)) = re_c;
		((*pz++)) = im_c;
		rest_8 -= 2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16C_Sat(
    mlib_s16 *xz,
    const mlib_s16 *c,
    mlib_s32 n)
{
	return (__mlib_VectorMulS_S16C_S16C_Sat(xz, xz, c, n));
}

/* *********************************************************** */

#define	MUL_S16C_S16C                                            \
	hi00 = vis_fmuld8sux16(vis_read_hi(val_x_d64), c_dir);   \
	lo00 = vis_fmuld8ulx16(vis_read_hi(val_x_d64), c_dir);   \
	hi10 = vis_fmuld8sux16(vis_read_hi(val_x_d64), c_rev);   \
	lo10 = vis_fmuld8ulx16(vis_read_hi(val_x_d64), c_rev);   \
	hi01 = vis_fmuld8sux16(vis_read_lo(val_x_d64), c_dir);   \
	lo01 = vis_fmuld8ulx16(vis_read_lo(val_x_d64), c_dir);   \
	hi11 = vis_fmuld8sux16(vis_read_lo(val_x_d64), c_rev);   \
	lo11 = vis_fmuld8ulx16(vis_read_lo(val_x_d64), c_rev);   \
	d0 = vis_fpadd32(hi00, lo00);                            \
	d1 = vis_fpadd32(hi10, lo10);                            \
	d2 = vis_fpadd32(hi01, lo01);                            \
	d3 = vis_fpadd32(hi11, lo11);                            \
	re0 = vis_fpsub32s(vis_read_hi(d0), vis_read_lo(d0));    \
	im0 = vis_fpadd32s(vis_read_hi(d1), vis_read_lo(d1));    \
	re1 = vis_fpsub32s(vis_read_hi(d2), vis_read_lo(d2));    \
	im1 = vis_fpadd32s(vis_read_hi(d3), vis_read_lo(d3))

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16C_S16C_Mod(
    mlib_s16 *z,
    const mlib_s16 *x,
    const mlib_s16 *c,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_s16 *px = (void *)x, *pz = z;
	mlib_d64 val_c_r = c[0], val_c_i = c[1];
	mlib_d64 scal_r, scal_i;

	mlib_f32 c_dir, c_rev;
	mlib_d64 hi00, lo00, hi10, lo10, hi01, lo01, hi11, lo11;
	mlib_d64 d0, d1, d2, d3, val_x_d64;
	mlib_d64 *dz, *dx;
	mlib_f32 re0, re1, im0, im1;
	mlib_d64 ld0, ld1;
	mlib_s16 x_re0, x_im0, x_re1, x_im1;
	union
	{
		mlib_f32 f32;
		mlib_s32 s32;
	} res0,
	    res1,
	    res2;

	if (n <= 0)
		return (MLIB_FAILURE);

	c_dir = vis_to_float((c[0] << 16) | (c[1] & 0xffff));
	c_rev = vis_to_float((c[1] << 16) | (c[0] & 0xffff));

	if (n == 1) {
		mlib_s16 x_re = x[0], x_im = x[1];

		z[0] = x_re * val_c_r - x_im * val_c_i;
		z[1] = x_re * val_c_i + x_im * val_c_r;

		return (MLIB_SUCCESS);
	}

	if (!((mlib_addr)z & 7) && !(c[0] == MLIB_S16_MIN &&
	    c[1] == MLIB_S16_MIN)) {
		dz = (mlib_d64 *)z;

		if (!((mlib_addr)x & 7)) {
			dx = (mlib_d64 *)x;
			vis_write_bmask(0x23ab67ef, 0);
#pragma pipeloop(0)
			for (i = 0; i < (n / 2); i++) {
				val_x_d64 = dx[i];

				MUL_S16C_S16C;
				dz[i] = vis_bshuffle(vis_freg_pair(re0, re1),
				    vis_freg_pair(im0, im1));
			}

		} else {
			dx = vis_alignaddr((void *)x, 0);
			ld0 = (*dx++);
			vis_write_bmask(0x23ab67ef, 0);
#pragma pipeloop(0)
			for (i = 0; i < (n / 2); i++) {
				ld1 = vis_ld_d64_nf(dx); dx++;
				val_x_d64 = vis_faligndata(ld0, ld1);
				ld0 = ld1;

				MUL_S16C_S16C;
				dz[i] = vis_bshuffle(vis_freg_pair(re0, re1),
				    vis_freg_pair(im0, im1));
			}
		}

		if (n % 2) {
			mlib_s16 x_re = x[2 * (n - 1)], x_im =
			    x[2 * (n - 1) + 1];

			z[2 * (n - 1)] =
			    x_re * (mlib_s32)c[0] - x_im * (mlib_s32)c[1];
			z[2 * (n - 1) + 1] =
			    x_re * (mlib_s32)c[1] + x_im * (mlib_s32)c[0];
		}

	} else {

		if (val_c_r == MLIB_S16_MIN && val_c_i == MLIB_S16_MIN) {
#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				scal_r = (px[2 * i] - (mlib_d64)px[2 * i +
				    1]) * MLIB_S16_MIN;
				scal_i = (px[2 * i] + (mlib_d64)px[2 * i +
				    1]) * MLIB_S16_MIN;

				if (scal_i > MLIB_S32_MAX)
					scal_i += 2.0 * MLIB_S32_MIN;
				pz[2 * i] = scal_r;
				pz[2 * i + 1] = scal_i;
			}

		} else {
			mlib_s32 ez, tail_off;
			mlib_f32 im1_prev, re1_prev;
			mlib_s16 x_im, x_re;

			dz = vis_alignaddr(z, 0);
			ez = z - (mlib_s16 *)dz;

			tail_off = (2 * n - (4 - ez)) & 3;

			if (tail_off == 1) {
				x_re0 = x[2 * (n - 1)];
			} else if (tail_off == 3) {
				x_re0 = x[2 * (n - 2)];
			}

			if (ez == 2) {

				x_re = x[0];
				x_im = x[1];
				z[0] = x_re * (mlib_s32)c[0] -
				    x_im * (mlib_s32)c[1];
				z[1] = x_re * (mlib_s32)c[1] +
				    x_im * (mlib_s32)c[0];

				__mlib_VectorMulS_S16C_S16C_Mod(z + 2,
				    (mlib_s16 *)x + 2, c, n - 1);
				return (MLIB_SUCCESS);
			}

			dx = vis_alignaddr((void *)x, 0);

			if (ez == 1) {
				ld0 = (*dx++);
				ld1 = vis_ld_d64_nf(dx); dx++;

				val_x_d64 = vis_faligndata(ld0, ld1);
				ld0 = ld1;

				MUL_S16C_S16C;
				res0.f32 = re0;
				res1.f32 = im0;
				res2.f32 = re1;

				z[0] = res0.s32;
				z[1] = res1.s32;
				z[2] = res2.s32;
				dz++;

				im1_prev = im1;
				vis_write_bmask(0x23ab67ef, 0);

#pragma pipeloop(0)
				for (i = 0; i < (2 * n - 3) / 4; i++) {
					ld1 = vis_ld_d64_nf(dx); dx++;
					val_x_d64 = vis_faligndata(ld0, ld1);
					ld0 = ld1;

					MUL_S16C_S16C;
					dz[i] = vis_bshuffle(vis_freg_pair
					    (im1_prev, im0),
					    vis_freg_pair(re0, re1));
					im1_prev = im1;
				}

			} else {
/* ez == 3 */
				ld0 = (*dx++);
				ld1 = vis_ld_d64_nf(dx); dx++;

				val_x_d64 = vis_faligndata(ld0, ld1);
				ld0 = ld1;

				MUL_S16C_S16C;
				res0.f32 = re0;

				z[0] = res0.s32;
				dz++;
				vis_write_bmask(0x23ab67ef, 0);

#pragma pipeloop(0)
				for (i = 0; i < (2 * n - 1) / 4; i++) {
					mlib_f32 im0_ = im0, im1_ = im1;

					re1_prev = re1;
					ld1 = vis_ld_d64_nf(dx); dx++;
					val_x_d64 = vis_faligndata(ld0, ld1);
					ld0 = ld1;

					MUL_S16C_S16C;
					dz[i] = vis_bshuffle(vis_freg_pair(im0_,
					    im1_), vis_freg_pair(re1_prev,
					    re0));
				}
			}

			if (tail_off == 1) {
				x_im0 = x[2 * (n - 1) + 1];

				z[2 * (n - 1) + 1] =
				    x_re0 * val_c_i + x_im0 * val_c_r;
			} else if (tail_off == 3) {
				x_im0 = x[2 * (n - 2) + 1];
				x_re1 = x[2 * (n - 1)];
				x_im1 = x[2 * (n - 1) + 1];

				z[2 * (n - 2) + 1] =
				    x_re0 * val_c_i + x_im0 * val_c_r;
				z[2 * (n - 1)] =
				    x_re1 * val_c_r - x_im1 * val_c_i;
				z[2 * (n - 1) + 1] =
				    x_re1 * val_c_i + x_im1 * val_c_r;
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S32C_S16C_Mod(
    mlib_s32 *z,
    const mlib_s16 *x,
    const mlib_s16 *c,
    mlib_s32 n)
{
	mlib_d64 *dpz, *dpx;
	mlib_d64 dx, dx0, dx1;
	mlib_d64 dr, dr1, dr2, dr3, dr4, dr5, dzh, dzl;
	mlib_s16 *px;
	mlib_s32 *pz;

/* odd = 1 means 8-byte aligned resultant vector starts with Im. part. */
	mlib_s32 odd = 0;
	mlib_s32 len = n * 2, i;

/* rest and length in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_f32 sr0, sr1, sr2, sr3;
	mlib_f32 rest;

/*
 * prepare the scaling factor
 */
	mlib_s16 sc0 = ((mlib_s16 *)c)[0], sc1 = ((mlib_s16 *)c)[1];
	mlib_u16 uc0 = ((mlib_u16 *)c)[0], uc1 = ((mlib_u16 *)c)[1];
	mlib_f32 fc0 = vis_to_float((uc0 << 16) | uc1);
	mlib_f32 fc1 = vis_to_float((uc1 << 16) | uc0);
	mlib_s32 re_x, im_x, re_y, im_y, re_c, im_c;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!(uc0 | uc1))

		return (__mlib_VectorZero_S32C(z, n));

	px = (mlib_s16 *)x;
	pz = (mlib_s32 *)z;

	if (n <= 4)
		MULS_S32C_S16C_MOD_IN_C;

/*
 * prepare the destination address trying to align it for 8 bytes.
 */

	if ((mlib_addr)pz & 7) {
		re_x = ((*px++));
		im_x = ((*px++));
		re_y = sc0;
		im_y = sc1;
		re_c = re_x * re_y - im_x * im_y;
		im_c = im_x * re_y + re_x * im_y;
		len -= 2;
		((*pz++)) = re_c;
		odd = 1;
		rest = vis_to_float(im_c);
	}

	dpz = (mlib_d64 *)pz;
	even_8 = len >> 2;
	rest_8 = len & 0x3;
	pz += (even_8 << 2);

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
				MULS_S16C_S32C;
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
				MULS_S16C_S32C;
				COMPOSE_S32C_FROM32_MOD_WITH_ODD;
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
				dpx++;
			}

/*
 * Storing of the last imag. part which was in 'rest'.
 */
			*((mlib_f32 *)pz) = rest;
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
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
				dx = vis_faligndata(dx0, dx1);
				MULS_S16C_S32C;
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
			*((mlib_f32 *)pz) = rest;
			pz++;
		} else {
/*
 * Start with real part.
 */
#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
				dx = vis_faligndata(dx0, dx1);
				MULS_S16C_S32C;
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

	if (!rest_8)
		return (MLIB_SUCCESS);

	px += (even_8 << 2);

	re_x = ((*px++));
	im_x = ((*px++));
	re_y = sc0;
	im_y = sc1;
	re_c = re_x * re_y - im_x * im_y;
	im_c = im_x * re_y + re_x * im_y;
	((*pz++)) = re_c;
	*pz = im_c;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16C_Mod(
    mlib_s16 *xz,
    const mlib_s16 *c,
    mlib_s32 n)
{
	return (__mlib_VectorMulS_S16C_S16C_Mod(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S32C_S32C_Sat(
    mlib_s32 *z,
    const mlib_s32 *x,
    const mlib_s32 *c,
    mlib_s32 n)
{
#define	MAX	0x1fffff
	mlib_s32 i;
	mlib_s32 *px = (mlib_s32 *)x, *pz = (mlib_s32 *)z;
	mlib_s32 c0 = ((mlib_s32 *)c)[0], c1 = ((mlib_s32 *)c)[1];
	mlib_d64 dc0 = c0, dc1 = c1;

	if ((abs(c0) <= MAX) && (abs(c1) <= MAX))
		for (i = 0; i < n; i++) {
			mlib_d64 mul_r = dc0 * px[2 * i] - dc1 * px[2 * i + 1];
			mlib_d64 mul_i = dc0 * px[2 * i + 1] + dc1 * px[2 * i];

			FLOAT2INT_CLAMP(pz[2 * i], mul_r);
			FLOAT2INT_CLAMP(pz[2 * i + 1], mul_i);
	} else {
		mlib_d64 dc0_lo = c0 & MAX, dc1_lo = c1 & MAX;

		dc0 -= dc0_lo;
		dc1 -= dc1_lo;
		for (i = 0; i < n; i++) {
			mlib_d64 mul_r = px[2 * i] * dc0 - px[2 * i + 1] * dc1 +
			    (px[2 * i] * dc0_lo - px[2 * i + 1] * dc1_lo);
			mlib_d64 mul_i = px[2 * i + 1] * dc0 + px[2 * i] * dc1 +
			    (px[2 * i + 1] * dc0_lo + px[2 * i] * dc1_lo);

			FLOAT2INT_CLAMP(pz[2 * i], mul_r);
			FLOAT2INT_CLAMP(pz[2 * i + 1], mul_i);
		}
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S32C_Sat(
    mlib_s32 *xz,
    const mlib_s32 *c,
    mlib_s32 n)
{
	return (__mlib_VectorMulS_S32C_S32C_Sat(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S32C_S32C_Mod(
    mlib_s32 *z,
    const mlib_s32 *x,
    const mlib_s32 *c,
    mlib_s32 n)
{
	mlib_s32 c_re = c[0], c_im = c[1], i_0, i_1, n2 = n * 2;

	mlib_d64 c_re_lo = c_re & 0xFFFF;
	mlib_d64 c_re_hi = (c_re >> 16) & 0xFFFF;
	mlib_d64 x_re_0;
	mlib_d64 c_im_lo = c_im & 0xFFFF;
	mlib_d64 c_im_hi = (c_im >> 16) & 0xFFFF;
	mlib_d64 x_im_0;
	mlib_s64 r0, r1, r2, r3;

#pragma pipeloop(0)
	for (i_0 = 0, i_1 = 1; i_0 < n2; i_0 += 2, i_1 += 2) {

		x_re_0 = x[i_0];
		x_im_0 = x[i_1];

		r0 = x_re_0 * c_re_hi - x_im_0 * c_im_hi;
		r1 = x_re_0 * c_re_lo - x_im_0 * c_im_lo;

		r2 = x_re_0 * c_im_hi + x_im_0 * c_re_hi;
		r3 = x_re_0 * c_im_lo + x_im_0 * c_re_lo;

		z[i_0] = (r0 << 16) + r1;
		z[i_1] = (r2 << 16) + r3;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S32C_Mod(
    mlib_s32 *xz,
    const mlib_s32 *c,
    mlib_s32 n)
{
	return (__mlib_VectorMulS_S32C_S32C_Mod(xz, xz, c, n));
}

/* *********************************************************** */
