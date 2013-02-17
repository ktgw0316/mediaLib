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

#pragma ident	"@(#)mlib_c_VectorScale.c	9.4	07/10/09 SMI"

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
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorScale_U8_[Sat|Mod](mlib_u8       * xz,
 *                                                const mlib_u8 * a,
 *                                                const mlib_u8 * b,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorScale_U8_U8_Mod(mlib_u8       * z,
 *                                             const mlib_u8 * x,
 *                                             const mlib_u8 * a,
 *                                             const mlib_u8 * b,
 *                                             mlib_s32      n);
 *      mlib_status mlib_VectorScale_S8_[Sat|Mod](mlib_s8       * xz,
 *                                                const mlib_s8 * a,
 *                                                const mlib_s8 * b,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorScale_S8_S8_[Sat|Mod](mlib_s8       * z,
 *                                                   const mlib_s8 * x,
 *                                                   const mlib_s8 * a,
 *                                                   const mlib_s8 * b,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_VectorScale_S16_U8_[Sat|Mod](mlib_s16      * z,
 *                                                    const mlib_u8 * x,
 *                                                    const mlib_u8 * a,
 *                                                    const mlib_u8 * b,
 *                                                    mlib_s32      n);
 *      mlib_status mlib_VectorScale_S16_S8_Mod(mlib_s16      * z,
 *                                              const mlib_s8 * x,
 *                                              const mlib_s8 * a,
 *                                              const mlib_s8 * b,
 *                                              mlib_s32      n);
 *      mlib_status mlib_VectorScale_S16_[Sat|Mod](mlib_s16       * xz,
 *                                                 const mlib_s16 * a,
 *                                                 const mlib_s16 * b,
 *                                                 mlib_s32       n);
 *      mlib_status mlib_VectorScale_S16_S16_[Sat|Mod](mlib_s16       * z,
 *                                                     const mlib_s16 * x,
 *                                                     const mlib_s16 * a,
 *                                                     const mlib_s16 * b,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_VectorScale_S32_[Sat|Mod](mlib_s32       * xz,
 *                                                 const mlib_s32 * a,
 *                                                 const mlib_s32 * b,
 *                                                 mlib_s32       n);
 *      mlib_status mlib_VectorScale_S32_S16_Mod(mlib_s32       * z,
 *                                               const mlib_s16 * x,
 *                                               const mlib_s16 * a,
 *                                               const mlib_s16 * b,
 *                                               mlib_s32       n);
 *      mlib_status mlib_VectorScale_S32_S32_[Sat|Mod](mlib_s32       * z,
 *                                                     const mlib_s32 * x,
 *                                                     const mlib_s32 * a,
 *                                                     const mlib_s32 * b,
 *                                                     mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      xz   pointer to the first element of the input and the output vector
 *      x    pointer to the first element of the input vector
 *      a    pointer to the scaling factor
 *      b    pointer to the offset
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *      Z = a * X + b
 *      XZ = a * XZ + b
 *
 */

/*
 * FUNCTIONS
 *      mlib_VectorScale_U8C_[Sat|Mod]
 *      mlib_VectorScale_[U8C|S16C]_U8C_[Sat|Mod]   - linear function on
 *                                                    unsigned 8-bit format
 *                                                    complex vector
 *      mlib_VectorScale_S8C_[Sat|Mod]
 *      mlib_VectorScale_[S8C|S16C]_S8C_[Sat|Mod]   - linear function on
 *                                                    signed 8-bit format
 *                                                    complex vector
 *      mlib_VectorScale_S16C_[Sat|Mod]
 *      mlib_VectorScale_[S16C|S32C]_S16C_[Sat|Mod] - linear function on
 *                                                    signed 16-bit format
 *                                                    complex vector
 *      mlib_VectorScale_S32C_[Sat|Mod]
 *      mlib_VectorScale_S32C_S32C_[Sat|Mod]        - linear function on
 *                                                    signed 32-bit format
 *                                                    complex vector
 *
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorScale_S8C_[Sat|Mod](mlib_s8       * xz,
 *                                                 const mlib_s8 * a,
 *                                                 const mlib_s8 * b,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_VectorScale_S8C_S8C_[Sat|Mod](mlib_s8       * z,
 *                                                     const mlib_s8 * x,
 *                                                     const mlib_s8 * a,
 *                                                     const mlib_s8 * b,
 *                                                     mlib_s32      n);
 *      mlib_status mlib_VectorScale_U8C_[Sat|Mod](mlib_u8       * xz,
 *                                                 const mlib_u8 * a,
 *                                                 const mlib_u8 * b,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_VectorScale_U8C_U8C_[Sat|Mod](mlib_u8       * z,
 *                                                     const mlib_u8 * x,
 *                                                     const mlib_u8 * a,
 *                                                     const mlib_u8 * b,
 *                                                     mlib_s32      n);
 *      mlib_status mlib_VectorScale_S16C_[Sat|Mod](mlib_s16       * xz,
 *                                                  const mlib_s16 * a,
 *                                                  const mlib_s16 * b,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_VectorScale_S16C_S16C_[Sat|Mod](mlib_s16       * z,
 *                                                       const mlib_s16 * x,
 *                                                       const mlib_s16 * a,
 *                                                       const mlib_s16 * b,
 *                                                       mlib_s32       n);
 *      mlib_status mlib_VectorScale_S16C_S8C_[Sat|Mod](mlib_s16      * z,
 *                                                      const mlib_s8 * x,
 *                                                      const mlib_s8 * a,
 *                                                      const mlib_s8 * b,
 *                                                      mlib_s32      n);
 *      mlib_status mlib_VectorScale_S16C_U8C_[Sat|Mod](mlib_s16      * z,
 *                                                      const mlib_u8 * x,
 *                                                      const mlib_u8 * a,
 *                                                      const mlib_u8 * b,
 *                                                      mlib_s32      n);
 *      mlib_status mlib_VectorScale_S32C_[Sat|Mod](mlib_s32       * xz,
 *                                                  const mlib_s32 * a,
 *                                                  const mlib_s32 * b,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_VectorScale_S32C_S16C_[Sat|Mod](mlib_s32       * z,
 *                                                       const mlib_s16 * x,
 *                                                       const mlib_s16 * a,
 *                                                       const mlib_s16 * b,
 *                                                       mlib_s32       n);
 *      mlib_status mlib_VectorScale_S32C_S32C_[Sat|Mod](mlib_s32       * z,
 *                                                       const mlib_s32 * x,
 *                                                       const mlib_s32 * a,
 *                                                       const mlib_s32 * b,
 *                                                       mlib_s32       n);
 *
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result vector.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts
 *      xz   pointer to the first element of the input and the output
 *           complex vector. xz[2*i] hold the real parts, and xz[2*i + 1]
 *           hold the imaginary parts
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
 *      XZ = a * XZ + b
 *
 */

#include <mlib_algebra.h>
#include <mlib_AlgebraUtil.h>

/*
 * Fix selftest failure caused by bug 6282734.
 * The expression px[i + 3] is aliased with the expression
 * *(int *) (pz + i + 2) while the type of the first expression
 * is short and the type of the second one is int.  Thus this
 * code does not meet the requirement of -xalias_level=basic
 * which becomes part of -fast in venus.
 */
#if __SUNPRO_C >= 0x520
#pragma alias(int, short)
#endif

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

#elif defined(__GNUC__)

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

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static const mlib_d64 *table = mlib_SU82D64 + 128;
static const mlib_d64 mul24bit = 1 << 24;
static const mlib_d64 mul16bit = 1 << 16;

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(X)	((mlib_s32)(X))

#else

#define	FLOAT2INT_CLAMP(X)                                       \
	(((X) >= MLIB_S32_MAX) ? MLIB_S32_MAX : (((X) <          \
		MLIB_S32_MIN) ? MLIB_S32_MIN : (mlib_s32)(X)))

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	PARAM2REG(type_out, type_in)                            \
	type_out *pz = z;                                       \
	const type_in *px = x, ia = *a, ib = *b

#define	PARAM2REGX(type_out, type_in)                            \
	type_out *pz = z;                                        \
	const type_in *px = x;                                   \
	type_in iar = a[0], iai = a[1], ibr = b[0], ibi = b[1]

#define	RETURN	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/* *********************************************************** */

#ifndef _LITTLE_ENDIAN

/* BIG ENDIAN */

#define	STORE2MOD(addr, val0, val1)                             \
	* (mlib_s32 *)(addr) = (mlib_s32)val1;                  \
	*(addr) = ((mlib_s32)val0)

#define	STORE2SAT(addr, val0, val1)                             \
	* (mlib_s32 *)(addr) = (mlib_s32)val0;                  \
	*((addr) + 1) = ((mlib_s32)val1) >> 16

#else

/* _LITTLE ENDIAN */
#define	STORE2MOD(addr, val0, val1)                             \
	* (mlib_s32 *)(addr) = (mlib_s32)val0;                  \
	*((addr) + 1) = ((mlib_s32)val1)

#define	STORE2SAT(addr, val0, val1)                             \
	* (mlib_s32 *)(addr) = (mlib_s32)val1;                  \
	*(addr) = ((mlib_s32)val0) >> 16

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

mlib_status
__mlib_VectorScale_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *a,
	const mlib_u8 *b,
	mlib_s32 n)
{
	mlib_s32 i;

#ifndef MLIB_USE_FTOI_CLAMPING

	mlib_s32 val_a = *((mlib_u8 *)a), val_b = *((mlib_u8 *)b);
	mlib_s32 scal;

	for (i = 0; i < n; i++) {
		scal = val_a * ((mlib_u8 *)x)[i] + val_b;
		scal = ((255 - (mlib_s32)scal) >> 31) | scal;
		((mlib_u8 *)z)[i] = scal;
	}

#else

	PARAM2REG(mlib_u8,
		mlib_u8);
	mlib_d64 scal, da = table[ia] * mul24bit, db =
		table[ib] * mul24bit + MLIB_S32_MIN;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= (n - 4); i += 4) {
		scal = table[px[i + 0]] * da + db;
		pz[i + 0] = ((mlib_s32)scal >> 24) + 128;
		scal = table[px[i + 1]] * da + db;
		pz[i + 1] = ((mlib_s32)scal >> 24) + 128;
		scal = table[px[i + 2]] * da + db;
		pz[i + 2] = ((mlib_s32)scal >> 24) + 128;
		scal = table[px[i + 3]] * da + db;
		pz[i + 3] = ((mlib_s32)scal >> 24) + 128;
	}

	for (; i < n; i++) {
		scal = table[px[i]] * da + db;
		pz[i] = ((mlib_s32)scal >> 24) + 128;
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

	RETURN;
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S8_S8_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *a,
	const mlib_s8 *b,
	mlib_s32 n)
{
	PARAM2REG(mlib_s8,
		mlib_s8);
	mlib_s32 i;
	mlib_d64 scal, da = table[ia], db = table[ib];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= (n - 4); i += 4) {
		scal = table[px[i + 0]] * da + db;
		pz[i + 0] = (mlib_s32)scal;
		scal = table[px[i + 1]] * da + db;
		pz[i + 1] = (mlib_s32)scal;
		scal = table[px[i + 2]] * da + db;
		pz[i + 2] = (mlib_s32)scal;
		scal = table[px[i + 3]] * da + db;
		pz[i + 3] = (mlib_s32)scal;
	}

	for (; i < n; i++) {
		scal = table[px[i]] * da + db;
		pz[i] = (mlib_s32)scal;
	}

	RETURN;
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
__mlib_VectorScale_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *a,
	const mlib_s8 *b,
	mlib_s32 n)
{
	mlib_s32 i;

#ifndef MLIB_USE_FTOI_CLAMPING

	mlib_s32 sign, sign1, scal, val_a = *a, val_b = *b;

	for (i = 0; i < n; i++) {
		scal = val_a * x[i] + val_b;

		sign = scal >> 31;
		sign1 = (scal >> 7) ^ sign;

		if (sign1 != 0)
			scal = sign ^ 127;

		z[i] = scal;
	}

#else

	PARAM2REG(mlib_s8,
		mlib_s8);
	mlib_d64 scal, da = table[ia] * mul24bit, db = table[ib] * mul24bit;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#define	NULLADD	+ 256
#else
#define	NULLADD

#endif /* __SUNPRO_C */
	for (i = 0; i <= (n - 4); i += 4) {
		scal = table[px[i + 0]] * da + db;
		pz[i + 0] = ((mlib_s32)scal >> 24) NULLADD;
		scal = table[px[i + 1]] * da + db;
		pz[i + 1] = ((mlib_s32)scal >> 24) NULLADD;
		scal = table[px[i + 2]] * da + db;
		pz[i + 2] = ((mlib_s32)scal >> 24) NULLADD;
		scal = table[px[i + 3]] * da + db;
		pz[i + 3] = ((mlib_s32)scal >> 24) NULLADD;
	}

	for (; i < n; i++) {
		scal = table[px[i]] * da + db;
		pz[i] = ((mlib_s32)scal >> 24) NULLADD;
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

	RETURN;
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
	mlib_s32 i;

#ifndef MLIB_USE_FTOI_CLAMPING

	mlib_s32 scal, val_a = *a, val_b = *b;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		scal = val_a * x[i] + val_b;
		scal = (((32767 - scal) >> 31) | scal) & 32767;
		z[i] = scal;
	}

	if (n <= 0)
		return (MLIB_FAILURE);

#else

	PARAM2REG(mlib_s16,
		mlib_u8);
	mlib_d64 scal, scal1, da = table[ia] * mul16bit, db =
		table[ib] * mul16bit;

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((mlib_addr)pz & 3) {
		scal = table[*px] * da + db;
		*pz = ((mlib_s32)scal) >> 16;
		n--;
		pz++;
		px++;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= (n - 2); i += 2) {
		scal = table[px[i + 0]] * da + db;
		scal1 = table[px[i + 1]] * da + db;
		STORE2SAT(pz + i, scal, scal1);
	}

	for (; i < n; i++) {
		scal = table[px[i]] * da + db;
		pz[i] = ((mlib_s32)scal) >> 16;
	}

#endif /* MLIB_USE_FTOI_CLAMPING */
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
	mlib_s32 i;

	PARAM2REG(mlib_s16,
		mlib_u8);
	mlib_d64 scal, scal1, da = table[ia], db = table[ib];

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((mlib_addr)pz & 3) {
		scal = table[*px] * da + db;
		*pz = (mlib_s32)scal;
		pz++;
		px++;
		n--;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= (n - 4); i += 4) {
		scal1 = table[px[i + 1]] * da + db;
		scal = table[px[i + 0]] * da + db;
		STORE2MOD(pz + i, scal, scal1);

		scal1 = table[px[i + 3]] * da + db;
		scal = table[px[i + 2]] * da + db;
		STORE2MOD(pz + i + 2, scal, scal1);
	}

	for (; i < n; i++) {
		scal = table[px[i]] * da + db;
		pz[i] = (mlib_s32)scal;
	}

	return (MLIB_SUCCESS);
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
	mlib_s32 i;

	PARAM2REG(mlib_s16,
		mlib_s8);
	mlib_d64 scal, scal1, da = table[ia], db = table[ib];

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((mlib_addr)pz & 3) {
		scal = table[*px] * da + db;
		*pz = (mlib_s32)scal;
		pz++;
		px++;
		n--;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= (n - 4); i += 4) {
		scal1 = table[px[i + 1]] * da + db;
		scal = table[px[i + 0]] * da + db;
		STORE2MOD(pz + i, scal, scal1);
		scal1 = table[px[i + 3]] * da + db;
		scal = table[px[i + 2]] * da + db;
		STORE2MOD(pz + i + 2, scal, scal1);
	}

	for (; i < n; i++) {
		scal = table[px[i]] * da + db;
		pz[i] = ((mlib_s32)scal);
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
	mlib_s32 i;

	PARAM2REG(mlib_s16,
		mlib_s16);

#ifndef MLIB_USE_FTOI_CLAMPING

	mlib_s32 scal, sign, sign1;

	if (n <= 0)
		return (MLIB_FAILURE);

	for (i = 0; i < n; i++) {
		scal = px[i] * ia + ib;

		sign = scal >> 31;
		sign1 = (scal >> 15) ^ sign;

		if (sign1 != 0)
			scal = sign ^ 32767;

		z[i] = scal;
	}

#else

	mlib_d64 scal, scal1, da = ia << 16, db = ib << 16;

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((mlib_addr)pz & 3) {
		scal = (mlib_d64)*px * da + db;
		*pz = ((mlib_s32)scal) >> 16;
		n--;
		px++;
		pz++;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= (n - 4); i += 4) {
		scal = (mlib_d64)px[i + 0] * da + db;
		scal1 = (mlib_d64)px[i + 1] * da + db;
		STORE2SAT(pz + i, scal, scal1);
		scal = (mlib_d64)px[i + 2] * da + db;
		scal1 = (mlib_d64)px[i + 3] * da + db;
		STORE2SAT(pz + i + 2, scal, scal1);
	}

	for (; i < n; i++) {
		scal = ((mlib_d64)px[i]) * da + db;
		pz[i] = ((mlib_s32)scal) >> 16;
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

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
	mlib_s32 i;

	PARAM2REG(mlib_s16,
		mlib_s16);
	mlib_d64 scal, scal1, da = (mlib_d64)ia, db = (mlib_d64)ib;

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((mlib_addr)pz & 3) {
		scal = (mlib_d64)(*px) * da + db;
		*pz = (mlib_s32)scal;
		n--;
		pz++;
		px++;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= (n - 4); i += 4) {
		scal1 = (mlib_d64)px[i + 1] * da + db;
		scal = (mlib_d64)px[i + 0] * da + db;
		STORE2MOD(pz + i, scal, scal1);
		scal1 = (mlib_d64)px[i + 3] * da + db;
		scal = (mlib_d64)px[i + 2] * da + db;
		STORE2MOD(pz + i + 2, scal, scal1);
	}

	for (; i < n; i++) {
		scal = (mlib_d64)px[i] * da + db;
		pz[i] = (mlib_s32)scal;
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
	mlib_s32 i;

	PARAM2REG(mlib_s32,
		mlib_s16);
	mlib_d64 scal, da = (mlib_d64)ia, db = (mlib_d64)ib;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= (n - 4); i += 4) {
		scal = (mlib_d64)px[i + 0] * da + db;
		pz[i + 0] = (mlib_s32)scal;
		scal = (mlib_d64)px[i + 1] * da + db;
		pz[i + 1] = (mlib_s32)scal;
		scal = (mlib_d64)px[i + 2] * da + db;
		pz[i + 2] = (mlib_s32)scal;
		scal = (mlib_d64)px[i + 3] * da + db;
		pz[i + 3] = (mlib_s32)scal;
	}

	for (; i < n; i++) {
		scal = (mlib_d64)px[i] * da + db;
		pz[i] = (mlib_s32)scal;
	}

	RETURN;
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

	PARAM2REG(mlib_s32,
		mlib_s32);
	mlib_d64 da = (mlib_s32)ia, db = (mlib_s32)ib;

#ifndef MLIB_USE_FTOI_CLAMPING

	for (i = 0; i < n; i++) {
		mlib_d64 scal = da * px[i] + db;

		if (scal > MLIB_S32_MAX)
			scal = MLIB_S32_MAX;
		else if (scal < MLIB_S32_MIN)
			scal = MLIB_S32_MIN;
		pz[i] = (mlib_s32)scal;
	}

#else

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		pz[i] = da * (mlib_d64)px[i] + db;
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

	RETURN;
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

	PARAM2REG(mlib_s32,
		mlib_s32);

#if (defined i386) || (defined _LITTLE_ENDIAN)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		pz[i] = px[i] * ia + ib;
	}

#else

	mlib_d64 d16 = 0x10000, _d16 = 1 / d16;
	mlib_d64 d32 = d16 * d16;
	mlib_d64 scalh, scall;
	mlib_d64 dah = ia >> 16, dx;
	mlib_d64 dal = ia & 0xFFFF;
	mlib_d64 db = ib;

	db += (mlib_d64)(1 << 26) * (mlib_d64)(3 << 25);

	for (i = 0; i < n; i++) {
		union
		{
			mlib_d64 d;
			struct
			{
				mlib_s32 s0, s1;
			} two_int;
		} res;

		dx = px[i];
		scalh = dah * dx;
		scall = dal * dx;

		scalh = scalh * d16 - (mlib_s32)(scalh * _d16) * d32;
		res.d = scalh + scall + db;
		pz[i] = res.two_int.s1;
	}

#endif /* (defined i386) || (defined _LITTLE_ENDIAN) */

	RETURN;
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

#define	SCALEC_SAT(form_in, form, MAX, MIN)                            \
	mlib_s32 i;                                                    \
	form_in *px = (form_in *) x, *pz = (form_in *) z;              \
	form val_a_r = ((form_in *) a)[0], val_a_i =                   \
		((form_in *) a)[1], val_b_r =                          \
		((form_in *) b)[0], val_b_i = ((form_in *) b)[1];      \
	form scal_r, scal_i;                                           \
	                                                               \
	/*                                                             \
	 * (a + jb) * (c + jd) = (ac - bd) + j(bc + ad)                \
	 */                                                            \
	for (i = 0; i < n; i++) {                                      \
	    scal_r = px[2 * i] * val_a_r - px[2 * i + 1] * val_a_i +   \
		    val_b_r;                                           \
	    scal_i = px[2 * i + 1] * val_a_r + px[2 * i] * val_a_i +   \
		    val_b_i;                                           \
	    if (scal_r > MAX)                                          \
		scal_r = MAX;                                          \
	    else if (scal_r < MIN)                                     \
		scal_r = MIN;                                          \
	    if (scal_i > MAX)                                          \
		scal_i = MAX;                                          \
	    else if (scal_i < MIN)                                     \
		scal_i = MIN;                                          \
	    pz[2 * i] = scal_r;                                        \
	    pz[2 * i + 1] = scal_i;                                    \
	}                                                              \
	                                                               \
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

/* *********************************************************** */

#define	SCALEC_MOD(form_in, form, MAX, MIN)                              \
	mlib_s32 i;                                                      \
	form_in *px = (form_in *) x, *pz = (form_in *) z;                \
	mlib_d64 val_a_r = ((form_in *) a)[0], val_a_i =                 \
		((form_in *) a)[1], val_b_r =                            \
		((form_in *) b)[0], val_b_i = ((form_in *) b)[1];        \
	mlib_d64 scal_r, scal_i;                                         \
	                                                                 \
	/*                                                               \
	 * (a + jb) * (c + jd) = (ac - bd) + j(bc + ad)                  \
	 */                                                              \
	if (val_a_r == MIN && val_a_i == MIN)                            \
	    for (i = 0; i < n; i++) {                                    \
		scal_r = (px[2 * i] - (mlib_d64)px[2 * i + 1]) * MIN +   \
			val_b_r;                                         \
		scal_i = (px[2 * i] + (mlib_d64)px[2 * i + 1]) * MIN +   \
			val_b_i;                                         \
		if (scal_i > MLIB_S32_MAX)                               \
		    scal_i += 2.0 * MLIB_S32_MIN;                        \
		pz[2 * i] = scal_r;                                      \
		pz[2 * i + 1] = scal_i;                                  \
	} else                                                           \
	    for (i = 0; i < n; i++) {                                    \
		scal_r = val_b_r + px[2 * i] * val_a_r - px[2 * i +      \
			1] * val_a_i;                                    \
		scal_i = val_b_i + px[2 * i + 1] * val_a_r +             \
			px[2 * i] * val_a_i;                             \
		pz[2 * i] = scal_r;                                      \
		pz[2 * i + 1] = scal_i;                                  \
	    }                                                            \
	                                                                 \
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

/* *********************************************************** */

mlib_status
__mlib_VectorScale_U8C_U8C_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *a,
	const mlib_u8 *b,
	mlib_s32 n)
{
	mlib_s32 i;

#ifndef MLIB_USE_FTOI_CLAMPING

	mlib_s32 scalr, scali, ixr, ixi;

	mlib_s32 iar = a[0], iai = a[1], ibr = b[0], ibi = b[1];
	mlib_u8 *pz = z, *px = (void *)x;

	for (i = 0; i < n; i++) {
		ixr = px[i * 2 + 0];
		ixi = px[i * 2 + 1];
		scalr = ixr * iar - ixi * iai + ibr;
		scali = ixr * iai + ixi * iar + ibi;
		pz[i * 2 + 0] =
			(scalr | ((mlib_s32)(255 - scalr) >> 31)) & ((-1 -
			scalr) >> 31);
		pz[i * 2 + 1] =
			(scali | ((mlib_s32)(255 - scali) >> 31)) & ((-1 -
			scali) >> 31);
	}

#else

	PARAM2REGX(mlib_u8,
		mlib_u8);

	mlib_d64 dar = table[iar] * mul24bit;
	mlib_d64 dai = table[iai] * mul24bit;
	mlib_d64 dbr = table[ibr] * mul24bit + MLIB_S32_MIN;
	mlib_d64 dbi = table[ibi] * mul24bit + MLIB_S32_MIN;
	mlib_d64 scalr, scali, dxr, dxi;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		dxr = table[px[i * 2 + 0]];
		dxi = table[px[i * 2 + 1]];
		scalr = dbr + dxr * dar - dxi * dai;
		scali = dbi + dxr * dai + dxi * dar;
		pz[i * 2 + 0] = ((mlib_s32)scalr >> 24) + 128;
		pz[i * 2 + 1] = ((mlib_s32)scali >> 24) + 128;
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

	RETURN;
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
	mlib_s32 i;

	PARAM2REGX(mlib_s16,
		mlib_u8);
	mlib_d64 dar = table[iar], dai = table[iai];
	mlib_d64 dbr = table[ibr], dbi = table[ibi];
	mlib_d64 scalr, scali, dxr, dxi;

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((mlib_addr)pz & 3) {

#ifndef _LITTLE_ENDIAN

		px += (n - 1) * 2;
		pz += (n - 1) * 2 - 1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; n > 1; n--) {
			dxr = table[px[0]];
			dxi = table[px[1]];
			scalr = dbr + dxr * dar - dxi * dai;
			scali = dbi + dxr * dai + dxi * dar;
			*(mlib_s32 *)pz = scalr;
			pz[2] = scali;
			pz -= 2;
			px -= 2;
		}

		dxr = table[px[0]];
		dxi = table[px[1]];
		scalr = dbr + dxr * dar - dxi * dai;
		scali = dbi + dxr * dai + dxi * dar;
		pz[1] = scalr;
		pz[2] = scali;

#else

		for (i = 0; i < n; i++) {
			dxr = table[px[i * 2 + 0]];
			dxi = table[px[i * 2 + 1]];
			scalr = dbr + dxr * dar - dxi * dai;
			scali = dbi + dxr * dai + dxi * dar;
			pz[i * 2] = scalr;
			pz[i * 2 + 1] = scali;
		}

#endif /* _LITTLE_ENDIAN */
	} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			dxi = table[px[i * 2 + 1]];
			dxr = table[px[i * 2 + 0]];
			scali = dbi + dxr * dai + dxi * dar;
			scalr = dbr + dxr * dar - dxi * dai;
			STORE2MOD(pz + i * 2, scalr, scali);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S16C_U8C_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *a,
	const mlib_u8 *b,
	mlib_s32 n)
{
	mlib_s32 i;

#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_u8 *px = (mlib_u8 *)x;
	mlib_s16 *pz = (mlib_s16 *)z;
	mlib_s32 val_a_r = ((mlib_u8 *)a)[0];
	mlib_s32 val_a_i = ((mlib_u8 *)a)[1];
	mlib_s32 val_b_r = ((mlib_u8 *)b)[0];
	mlib_s32 val_b_i = ((mlib_u8 *)b)[1];
	mlib_s32 scal_r, scal_i;

/* (a + jb) * (c + jd) = (ac - bd) + j(bc + ad) */

	if ((val_a_r + val_a_i) <= MLIB_S8_MAX)
		return (__mlib_VectorScale_S16C_U8C_Mod(z, x, a, b, n));
	for (i = 0; i < n; i++) {
		scal_r = px[2 * i] * val_a_r - px[2 * i + 1] * val_a_i +
			val_b_r;
		scal_i = px[2 * i + 1] * val_a_r + px[2 * i] * val_a_i +
			val_b_i;

		if (scal_r > MLIB_S16_MAX)
			scal_r = MLIB_S16_MAX;
		else if (scal_r < MLIB_S16_MIN)
			scal_r = MLIB_S16_MIN;

		if (scal_i > MLIB_S16_MAX)
			scal_i = MLIB_S16_MAX;
		pz[2 * i] = scal_r;
		pz[2 * i + 1] = scal_i;
	}

	if (n <= 0)
		return (MLIB_FAILURE);

#else

	PARAM2REGX(mlib_s16,
		mlib_u8);
	mlib_d64 dar = table[iar] * mul16bit, dai = table[iai] * mul16bit;
	mlib_d64 dbr = table[ibr] * mul16bit, dbi = table[ibi] * mul16bit;
	mlib_d64 scalr, scali, dxr, dxi;

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((mlib_addr)pz & 3) {

#ifndef _LITTLE_ENDIAN
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n - 1; i++) {
			dxr = table[px[i * 2 + 0]];
			dxi = table[px[i * 2 + 1]];
			scalr = dbr + dxr * dar - dxi * dai;
			scali = dbi + dxr * dai + dxi * dar;
			pz[i * 2 + 0] = (mlib_s32)scalr >> 16;
			*(mlib_s32 *)(pz + i * 2 + 1) = (mlib_s32)scali;
		}

		dxr = table[px[i * 2 + 0]];
		dxi = table[px[i * 2 + 1]];
		scalr = dbr + dxr * dar - dxi * dai;
		scali = dbi + dxr * dai + dxi * dar;
		pz[i * 2 + 0] = (mlib_s32)scalr >> 16;
		pz[i * 2 + 1] = (mlib_s32)scali >> 16;

#else

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n - 1; i++) {
			dxr = table[px[i * 2 + 0]];
			dxi = table[px[i * 2 + 1]];
			scalr = dbr + dxr * dar - dxi * dai;
			scali = dbi + dxr * dai + dxi * dar;
			pz[i * 2] = (mlib_s32)scalr >> 16;
			pz[i * 2 + 1] = (mlib_s32)scali >> 16;
		}

#endif /* _LITTLE_ENDIAN */
	} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			dxr = table[px[i * 2 + 0]];
			dxi = table[px[i * 2 + 1]];
			scalr = dbr + dxr * dar - dxi * dai;
			scali = dbi + dxr * dai + dxi * dar;
			STORE2SAT(pz + i * 2, scalr, scali);
		}
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *a,
	const mlib_s8 *b,
	mlib_s32 n)
{
#ifndef MLIB_USE_FTOI_CLAMPING

	SCALEC_SAT(mlib_s8,
		mlib_s32,
		MLIB_S8_MAX,
		MLIB_S8_MIN)
#else

	PARAM2REGX(mlib_s8,
		mlib_s8);
	mlib_d64 dar = table[iar] * mul24bit;
	mlib_d64 dai = table[iai] * mul24bit;
	mlib_d64 dbr = table[ibr] * mul24bit;
	mlib_d64 dbi = table[ibi] * mul24bit;
	mlib_d64 scalr, scali, dxr, dxi;
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		dxr = table[px[i * 2 + 0]];
		dxi = table[px[i * 2 + 1]];
		scalr = dbr + dxr * dar - dxi * dai;
		scali = dbi + dxr * dai + dxi * dar;
		pz[i * 2 + 0] = (mlib_s32)scalr >> 24;
		pz[i * 2 + 1] = (mlib_s32)scali >> 24;
	}

	RETURN;
#endif /* MLIB_USE_FTOI_CLAMPING */
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
	mlib_s32 i;

	PARAM2REGX(mlib_s16,
		mlib_s8);

	mlib_d64 dar = table[iar], dai = table[iai];
	mlib_d64 dbr = table[ibr], dbi = table[ibi];
	mlib_d64 scalr, scali, dxr, dxi;

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((mlib_s32)pz & 3) {

#ifndef _LITTLE_ENDIAN

		pz += (n - 1) * 2 - 1;
		px += (n - 1) * 2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; n > 1; n--) {
			dxr = table[px[0]];
			dxi = table[px[1]];
			scalr = dbr + dxr * dar - dxi * dai;
			scali = dbi + dxr * dai + dxi * dar;
			*(mlib_s32 *)pz = scalr;
			pz[2] = scali;
			pz -= 2;
			px -= 2;
		}

		dxr = table[px[0]];
		dxi = table[px[1]];
		scalr = dbr + dxr * dar - dxi * dai;
		scali = dbi + dxr * dai + dxi * dar;
		pz[1] = scalr;
		pz[2] = scali;

#else

		for (i = 0; i < n; i++) {
			dxr = table[px[i * 2]];
			dxi = table[px[i * 2 + 1]];
			scalr = dbr + dxr * dar - dxi * dai;
			scali = dbi + dxr * dai + dxi * dar;
			pz[i * 2 + 0] = scalr;
			pz[i * 2 + 1] = scali;
		}

#endif /* _LITTLE_ENDIAN */
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			dxi = table[px[i * 2 + 1]];
			dxr = table[px[i * 2 + 0]];
			scali = dbi + dxr * dai + dxi * dar;
			scalr = dbr + dxr * dar - dxi * dai;
			STORE2MOD(pz + i * 2, scalr, scali);
		}
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
	mlib_s32 i;

	PARAM2REGX(mlib_s16,
		mlib_s8);

#ifndef MLIB_USE_FTOI_CLAMPING

	mlib_s32 scalr, scali;

	if ((iar > MLIB_S8_MIN) || (iai > MLIB_S8_MIN))
		return (__mlib_VectorScale_S16C_S8C_Mod(z, x, a, b, n));
	for (i = 0; i < n; i++) {
		scalr = (px[2 * i] - px[2 * i + 1]) * MLIB_S8_MIN + ibr;
		scali = (px[2 * i] + px[2 * i + 1]) * MLIB_S8_MIN + ibi;

		if (scali > MLIB_S16_MAX)
			scali = MLIB_S16_MAX;
		pz[2 * i] = scalr;
		pz[2 * i + 1] = scali;
	}

	if (n <= 0)
		return (MLIB_FAILURE);

#else

	mlib_d64 dar = table[iar] * mul16bit, dai = table[iai] * mul16bit;
	mlib_d64 dbr = table[ibr] * mul16bit, dbi = table[ibi] * mul16bit;
	mlib_d64 scalr, scali, dxr, dxi;

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((mlib_addr)pz & 3) {

#ifndef _LITTLE_ENDIAN
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n - 1; i++) {
			dxr = table[px[i * 2 + 0]];
			dxi = table[px[i * 2 + 1]];
			scalr = dbr + dxr * dar - dxi * dai;
			scali = dbi + dxr * dai + dxi * dar;
			pz[i * 2 + 0] = (mlib_s32)scalr >> 16;
			*(mlib_s32 *)(pz + i * 2 + 1) = (mlib_s32)scali;
		}

		dxr = table[px[i * 2 + 0]];
		dxi = table[px[i * 2 + 1]];
		scalr = dbr + dxr * dar - dxi * dai;
		scali = dbi + dxr * dai + dxi * dar;
		pz[i * 2 + 0] = (mlib_s32)scalr >> 16;
		pz[i * 2 + 1] = (mlib_s32)scali >> 16;

#else

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			dxr = table[px[i * 2 + 0]];
			dxi = table[px[i * 2 + 1]];
			scalr = dbr + dxr * dar - dxi * dai;
			scali = dbi + dxr * dai + dxi * dar;
			pz[i * 2] = (mlib_s32)scalr >> 16;
			pz[i * 2 + 1] = (mlib_s32)scali >> 16;
		}

#endif /* _LITTLE_ENDIAN */
	} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			dxr = table[px[i * 2 + 0]];
			dxi = table[px[i * 2 + 1]];
			scalr = dbr + dxr * dar - dxi * dai;
			scali = dbi + dxr * dai + dxi * dar;
			STORE2SAT(pz + i * 2, scalr, scali);
		}
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *a,
	const mlib_s16 *b,
	mlib_s32 n)
{
#ifndef MLIB_USE_FTOI_CLAMPING

	SCALEC_SAT(mlib_s16,
		mlib_d64,
		MLIB_S16_MAX,
		MLIB_S16_MIN)
#else

	mlib_s32 i;

	PARAM2REGX(mlib_s16,
		mlib_s16);
	mlib_d64 dar = iar << 16, dai = iai << 16;
	mlib_d64 dbr = ibr << 16, dbi = ibi << 16;
	mlib_d64 scalr, scali, dxr, dxi;

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((mlib_addr)pz & 3) {

#ifndef _LITTLE_ENDIAN

		dxr = px[0];
		dxi = px[1];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n - 1; i++) {
			scalr = dxr * dar - dxi * dai + dbr;
			scali = dxr * dai + dxi * dar + dbi;
			dxr = px[i * 2 + 2];
			dxi = px[i * 2 + 3];
			pz[i * 2 + 0] = (mlib_s32)scalr >> 16;
			*(mlib_s32 *)(pz + i * 2 + 1) = scali;
		}

		scalr = dxr * dar - dxi * dai + dbr;
		scali = dxr * dai + dxi * dar + dbi;
		pz[i * 2 + 0] = (mlib_s32)scalr >> 16;
		pz[i * 2 + 1] = (mlib_s32)scali >> 16;

#else

		for (i = 0; i < n; i++) {
			dxr = px[i * 2 + 0];
			dxi = px[i * 2 + 1];
			scalr = dxr * dar - dxi * dai + dbr;
			scali = dxr * dai + dxi * dar + dbi;
			pz[i * 2] = (mlib_s32)scalr >> 16;
			pz[i * 2 + 1] = (mlib_s32)scali >> 16;
		}

#endif /* _LITTLE_ENDIAN */
	} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			dxr = px[i * 2 + 0];
			dxi = px[i * 2 + 1];
			scalr = dxr * dar - dxi * dai + dbr;
			scali = dxr * dai + dxi * dar + dbi;
			STORE2SAT(pz + i * 2, scalr, scali);
		}
	}

	return (MLIB_SUCCESS);

#endif /* MLIB_USE_FTOI_CLAMPING */
}

/* *********************************************************** */

mlib_status
__mlib_VectorScale_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *a,
	const mlib_s16 *b,
	mlib_s32 n)
{
	PARAM2REGX(mlib_s16,
		mlib_s16);
	mlib_d64 scalr, scali, dxr, dxi;
	mlib_d64 dar = (mlib_s32)iar, dai = (mlib_s32)iai;
	mlib_d64 dbr = (mlib_s32)ibr, dbi = (mlib_s32)ibi;
	mlib_s32 i, x0, x1;

	if (dar == MLIB_S16_MIN && dai == MLIB_S16_MIN) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			x0 = px[i * 2 + 0];
			x1 = px[i * 2 + 1];
			scalr = dbr + (x0 - x1) * dar;
			scali = dbi + (x0 + x1) * dar;

			if (scali > MLIB_S32_MAX)
				scali += 2.0 * MLIB_S32_MIN;
			pz[i * 2 + 0] = scalr;
			pz[i * 2 + 1] = scali;
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			dxr = px[i * 2 + 0];
			dxi = px[i * 2 + 1];
			scalr = dbr + dxr * dar - dxi * dai;
			scali = dbi + dxr * dai + dxi * dar;
			pz[i * 2 + 0] = scalr;
			pz[i * 2 + 1] = scali;
		}
	}

	RETURN;
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
	mlib_s32 i;
	mlib_s16 *px = (mlib_s16 *)x;
	mlib_s32 *pz = (mlib_s32 *)z;
	mlib_d64 val_a_r = ((mlib_s16 *)a)[0];
	mlib_d64 val_a_i = ((mlib_s16 *)a)[1];
	mlib_d64 val_b_r = ((mlib_s16 *)b)[0];
	mlib_d64 val_b_i = ((mlib_s16 *)b)[1];
	mlib_d64 scal_r, scal_i;

/* (a + jb) * (c + jd) = (ac - bd) + j(bc + ad) */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		scal_r = px[2 * i] * val_a_r - px[2 * i + 1] * val_a_i +
			val_b_r;
		scal_i = px[2 * i + 1] * val_a_r + px[2 * i] * val_a_i +
			val_b_i;
		pz[2 * i] = scal_r;

#ifdef MLIB_USE_FTOI_CLAMPING

		pz[2 * i + 1] = scal_i;

#else

		if (scal_i > MLIB_S32_MAX)
			pz[2 * i + 1] = MLIB_S32_MAX;
		else
			pz[2 * i + 1] = scal_i;

#endif /* MLIB_USE_FTOI_CLAMPING */
	}

	RETURN;
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
	mlib_s32 i;
	mlib_s16 *px = (mlib_s16 *)x;
	mlib_s32 *pz = (mlib_s32 *)z;
	mlib_d64 val_a_r = ((mlib_s16 *)a)[0];
	mlib_d64 val_a_i = ((mlib_s16 *)a)[1];
	mlib_d64 val_b_r = ((mlib_s16 *)b)[0];
	mlib_d64 val_b_i = ((mlib_s16 *)b)[1];
	mlib_d64 scal_r, scal_i;

/* (a + jb) * (c + jd) = (ac - bd) + j(bc + ad) */

	if (val_a_r == MLIB_S16_MIN && val_a_i == MLIB_S16_MIN)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			scal_r = (px[2 * i] - (mlib_d64)px[2 * i +
				1]) * MLIB_S16_MIN + val_b_r;
			scal_i = (px[2 * i] + (mlib_d64)px[2 * i +
				1]) * MLIB_S16_MIN + val_b_i;

			if (scal_i > MLIB_S32_MAX)
				scal_i += 2.0 * MLIB_S32_MIN;
			pz[2 * i] = scal_r;
			pz[2 * i + 1] = scal_i;
	} else
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			scal_r = val_b_r + px[2 * i] * val_a_r - px[2 * i +
				1] * val_a_i;
			scal_i = val_b_i + px[2 * i + 1] * val_a_r +
				px[2 * i] * val_a_i;
			pz[2 * i] = scal_r;
			pz[2 * i + 1] = scal_i;
		}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
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
	mlib_s32 *px = (mlib_s32 *)x, *pz = (mlib_s32 *)z;

	mlib_d64 val_a_r = a[0];
	mlib_d64 val_a_i = a[1];
	mlib_d64 val_b_r = b[0];
	mlib_d64 val_b_i = b[1];
	mlib_d64 scal_r, scal_i, scal_r_lo, scal_i_lo;
	mlib_d64 scal_r_hi, scal_i_hi;

	if ((abs(a[0]) < MAX) && (abs(a[1]) < MAX)) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			scal_r = px[2 * i] * val_a_r - px[2 * i + 1] * val_a_i;
			scal_i = px[2 * i + 1] * val_a_r + px[2 * i] * val_a_i;
			scal_r += val_b_r;
			scal_i += val_b_i;
			pz[2 * i] = FLOAT2INT_CLAMP(scal_r);
			pz[2 * i + 1] = FLOAT2INT_CLAMP(scal_i);
		}
	} else {
		mlib_d64 val_a_r_lo = a[0] & MAX, val_a_i_lo = a[1] & MAX;

		val_a_r -= val_a_r_lo;
		val_a_i -= val_a_i_lo;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			scal_r_lo =
				px[2 * i] * val_a_r_lo - px[2 * i +
				1] * val_a_i_lo + val_b_r;
			scal_r_hi =
				px[2 * i] * val_a_r - px[2 * i + 1] * val_a_i;
			scal_i_lo =
				px[2 * i + 1] * val_a_r_lo +
				px[2 * i] * val_a_i_lo + val_b_i;
			scal_i_hi =
				px[2 * i + 1] * val_a_r + px[2 * i] * val_a_i;
			scal_r = scal_r_hi + scal_r_lo;
			scal_i = scal_i_hi + scal_i_lo;
			pz[2 * i] = FLOAT2INT_CLAMP(scal_r);
			pz[2 * i + 1] = FLOAT2INT_CLAMP(scal_i);
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
	mlib_s32 i;
	const mlib_s32 *px = x;
	mlib_s32 *pz = z;

#if (defined i386) || (defined _LITTLE_ENDIAN)

	mlib_s32 val_b_r = ((mlib_s32 *)b)[0];
	mlib_s32 val_b_i = ((mlib_s32 *)b)[1];
	mlib_s32 val_a_r = ((mlib_s32 *)a)[0];
	mlib_s32 val_a_i = ((mlib_s32 *)a)[1];
	mlib_s32 scal_r, scal_i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		scal_r = px[2 * i] * val_a_r - px[2 * i + 1] * val_a_i +
			val_b_r;
		scal_i = px[2 * i + 1] * val_a_r + px[2 * i] * val_a_i +
			val_b_i;
		pz[2 * i] = scal_r;
		pz[2 * i + 1] = scal_i;
	}

#else

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

	dbr += (mlib_d64)(1 << 26) * (mlib_d64)(3 << 25);
	dbi += (mlib_d64)(1 << 26) * (mlib_d64)(3 << 25);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		union
		{
			mlib_d64 d;
			struct
			{
				mlib_s32 s0, s1;
			} two_int;
		} resr,
		    resi;

		dxr = px[i * 2];
		dxi = px[i * 2 + 1];

		scalrh = dxr * darh - dxi * daih;
		scalrl = dxr * darl - dxi * dail + dbr;
		scalih = dxr * daih + dxi * darh;
		scalil = dxr * dail + dxi * darl + dbi;

		scalrh = scalrh * d16 - (mlib_s32)(scalrh * _d16) * d32;
		resr.d = scalrh + scalrl;
		pz[i * 2] = resr.two_int.s1;

		scalih = scalih * d16 - (mlib_s32)(scalih * _d16) * d32;
		resi.d = scalih + scalil;
		pz[i * 2 + 1] = resi.two_int.s1;
	}

#endif /* (defined i386) || (defined _LITTLE_ENDIAN) */

	RETURN;
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
__mlib_VectorScale_S8C_S8C_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *a,
	const mlib_s8 *b,
	mlib_s32 n)
{
	PARAM2REGX(mlib_s8,
		mlib_s8);
	mlib_s32 i;
	mlib_d64 dar = table[iar], dai = table[iai];
	mlib_d64 dbr = table[ibr], dbi = table[ibi];
	mlib_d64 dxr, dxi, scalr, scali;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		dxr = table[px[i * 2 + 0]];
		dxi = table[px[i * 2 + 1]];
		scalr = dxr * dar - dxi * dai + dbr;
		scali = dxr * dai + dxi * dar + dbi;
		pz[i * 2 + 0] = scalr;
		pz[i * 2 + 1] = scali;
	}

	RETURN;
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
