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

#pragma ident	"@(#)mlib_s_VectorScale.c	9.4	07/11/05 SMI"

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

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

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

#define	SAT_U8(x)		\
	if (x < 0)		\
	    x = 0;		\
	else if (x > 255)	\
	    x = 255

#define	SAT_S8(x)		\
	if (x < -128)		\
	    x = -128;		\
	else if (x > 127)	\
	    x = 127

#define	SAT_S16(x)			\
	if (x < MLIB_S16_MIN)		\
	    x = MLIB_S16_MIN;		\
	else if (x > MLIB_S16_MAX)	\
	    x = MLIB_S16_MAX

#define	SAT_S32(x)			\
	if (x < MLIB_S32_MIN)		\
	    x = MLIB_S32_MIN;		\
	else if (x > MLIB_S32_MAX)	\
	    x = MLIB_S32_MAX

/* ****************************************************************** */

#define	SCALE_U8_SAT(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);	\
						\
	xlo = _mm_unpacklo_epi8(zero, xbuf);	\
	xhi = _mm_unpackhi_epi8(zero, xbuf);	\
	xlo = _mm_mulhi_epu16(xlo, abuf);	\
	xhi = _mm_mulhi_epu16(xhi, abuf);	\
	xlo = _mm_max_epi16(xlo, mask1);	\
	xhi = _mm_max_epi16(xhi, mask1);	\
	xlo = _mm_and_si128(xlo, mask2);	\
	xhi = _mm_and_si128(xhi, mask2);	\
	xlo = _mm_adds_epi16(xlo, bbuf);	\
	xhi = _mm_adds_epi16(xhi, bbuf);	\
	zbuf = _mm_packus_epi16(xlo, xhi);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	pz += nstep

mlib_status
__mlib_VectorScale_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *a,
	const mlib_u8 *b,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, a0, b0, ax, az, nstep, n1, n2, n3;
	mlib_u8 *px = (mlib_u8 *)x, *pz = (mlib_u8 *)z;
	__m128i xbuf, ybuf, zbuf, abuf, bbuf, xlo, xhi, ylo, yhi;
	__m128i zero, mask1, mask2;

	a0 = a[0];
	b0 = b[0];

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = a0 * (*px++) + b0;
			SAT_U8(sum);
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		abuf = _mm_set1_epi16(a0 << 8);
		bbuf = _mm_set1_epi16(b0);
		mask1 = _mm_set1_epi16(-1);
		mask2 = _mm_set1_epi16(32767);

		for (i = 0; i < n1; i++) {
			sum = a0 * (*px++) + b0;
			SAT_U8(sum);
			*pz++ = sum;
		}

		if (ax == az) {
			for (i = 0; i < n2; i++) {
				SCALE_U8_SAT(load, store);
			}
		} else {
			for (i = 0; i < n2; i++) {
				SCALE_U8_SAT(load, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = a0 * (*px++) + b0;
			SAT_U8(sum);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SCALE_U8_MOD(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);	\
						\
	xlo = _mm_unpacklo_epi8(zero, xbuf);	\
	xhi = _mm_unpackhi_epi8(zero, xbuf);	\
	xlo = _mm_mulhi_epu16(xlo, abuf);	\
	xhi = _mm_mulhi_epu16(xhi, abuf);	\
	xlo = _mm_and_si128(xlo, mask);		\
	xhi = _mm_and_si128(xhi, mask);		\
	zbuf = _mm_packus_epi16(xlo, xhi);	\
	zbuf = _mm_add_epi8(zbuf, bbuf);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	pz += nstep

mlib_status
__mlib_VectorScale_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *a,
	const mlib_u8 *b,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, a0, b0, ax, az, nstep, n1, n2, n3;
	mlib_u8 *px = (mlib_u8 *)x, *pz = (mlib_u8 *)z;
	__m128i xbuf, ybuf, zbuf, abuf, bbuf, xlo, xhi, ylo, yhi;
	__m128i zero, mask;

	a0 = a[0];
	b0 = b[0];

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = a0 * (*px++) + b0;
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		abuf = _mm_set1_epi16(a0 << 8);
		bbuf = _mm_set1_epi8(b0);
		mask = _mm_set1_epi16(0xff);

		for (i = 0; i < n1; i++) {
			sum = a0 * (*px++) + b0;
			*pz++ = sum;
		}

		if (ax == az) {
			for (i = 0; i < n2; i++) {
				SCALE_U8_MOD(load, store);
			}
		} else {
			for (i = 0; i < n2; i++) {
				SCALE_U8_MOD(load, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = a0 * (*px++) + b0;
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SCALE_S8_SAT(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);	\
						\
	xlo = _mm_unpacklo_epi8(zero, xbuf);	\
	xhi = _mm_unpackhi_epi8(zero, xbuf);	\
	xlo = _mm_mulhi_epi16(xlo, abuf);	\
	xhi = _mm_mulhi_epi16(xhi, abuf);	\
	xlo = _mm_adds_epi16(xlo, bbuf);	\
	xhi = _mm_adds_epi16(xhi, bbuf);	\
	zbuf = _mm_packs_epi16(xlo, xhi);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	pz += nstep

mlib_status
__mlib_VectorScale_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *a,
	const mlib_s8 *b,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, a0, b0, ax, az, nstep, n1, n2, n3;
	mlib_s8 *px = (mlib_s8 *)x, *pz = (mlib_s8 *)z;
	__m128i xbuf, ybuf, zbuf, abuf, bbuf, xlo, xhi, ylo, yhi;
	__m128i zero, mask1, mask2;

	a0 = a[0];
	b0 = b[0];

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = a0 * (*px++) + b0;
			SAT_S8(sum);
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		abuf = _mm_set1_epi16(a0 << 8);
		bbuf = _mm_set1_epi16(b0);

		for (i = 0; i < n1; i++) {
			sum = a0 * (*px++) + b0;
			SAT_S8(sum);
			*pz++ = sum;
		}

		if (ax == az) {
			for (i = 0; i < n2; i++) {
				SCALE_S8_SAT(load, store);
			}
		} else {
			for (i = 0; i < n2; i++) {
				SCALE_S8_SAT(load, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = a0 * (*px++) + b0;
			SAT_S8(sum);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
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
	return (__mlib_VectorScale_U8_U8_Mod((mlib_u8 *)z, (mlib_u8 *)x,
				(mlib_u8 *)a, (mlib_u8 *)b, n));
}

/* *********************************************************** */

#define	SCALE_S16U8_SAT(ld, st)				\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	xlo = _mm_unpacklo_epi8(zero, xbuf);		\
	xhi = _mm_unpackhi_epi8(zero, xbuf);		\
	xlo = _mm_mulhi_epu16(xlo, abuf);		\
	xhi = _mm_mulhi_epu16(xhi, abuf);		\
	xlo = _mm_max_epi16(xlo, mask1);		\
	xhi = _mm_max_epi16(xhi, mask1);		\
	xlo = _mm_and_si128(xlo, mask2);		\
	xhi = _mm_and_si128(xhi, mask2);		\
	xlo = _mm_adds_epi16(xlo, bbuf);		\
	xhi = _mm_adds_epi16(xhi, bbuf);		\
	_mm_##st##_si128((__m128i *)pz, xlo);		\
	_mm_##st##_si128((__m128i *)pz + 1, xhi);	\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorScale_S16_U8_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *a,
	const mlib_u8 *b,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, a0, b0, ax, az, nstep, n1, n2, n3;
	mlib_u8 *px = (mlib_u8 *)x;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf, abuf, bbuf, xlo, xhi, ylo, yhi;
	__m128i zero, mask1, mask2;

	a0 = a[0];
	b0 = b[0];

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = a0 * (*px++) + b0;
			SAT_S16(sum);
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		abuf = _mm_set1_epi16(a0 << 8);
		bbuf = _mm_set1_epi16(b0);
		mask1 = _mm_set1_epi16(-1);
		mask2 = _mm_set1_epi16(32767);

		for (i = 0; i < n1; i++) {
			sum = a0 * (*px++) + b0;
			SAT_S16(sum);
			*pz++ = sum;
		}

		if ((ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				SCALE_S16U8_SAT(load, store);
			}
		} else {
			for (i = 0; i < n2; i++) {
				SCALE_S16U8_SAT(load, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = a0 * (*px++) + b0;
			SAT_S16(sum);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SCALE_S16U8_MOD(ld, st)				\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	xlo = _mm_unpacklo_epi8(zero, xbuf);		\
	xhi = _mm_unpackhi_epi8(zero, xbuf);		\
	xlo = _mm_mulhi_epu16(xlo, abuf);		\
	xhi = _mm_mulhi_epu16(xhi, abuf);		\
	xlo = _mm_add_epi16(xlo, bbuf);			\
	xhi = _mm_add_epi16(xhi, bbuf);			\
	_mm_##st##_si128((__m128i *)pz, xlo);		\
	_mm_##st##_si128((__m128i *)pz + 1, xhi);	\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorScale_S16_U8_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *a,
	const mlib_u8 *b,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, a0, b0, ax, az, nstep, n1, n2, n3;
	mlib_u8 *px = (mlib_u8 *)x;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf, abuf, bbuf, xlo, xhi, ylo, yhi;
	__m128i zero, mask1, mask2;

	a0 = a[0];
	b0 = b[0];

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = a0 * (*px++) + b0;
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		abuf = _mm_set1_epi16(a0 << 8);
		bbuf = _mm_set1_epi16(b0);

		for (i = 0; i < n1; i++) {
			sum = a0 * (*px++) + b0;
			*pz++ = sum;
		}

		if ((ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				SCALE_S16U8_MOD(load, store);
			}
		} else {
			for (i = 0; i < n2; i++) {
				SCALE_S16U8_MOD(load, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = a0 * (*px++) + b0;
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SCALE_S16S8_MOD(ld, st)				\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	xlo = _mm_unpacklo_epi8(zero, xbuf);		\
	xhi = _mm_unpackhi_epi8(zero, xbuf);		\
	xlo = _mm_mulhi_epi16(xlo, abuf);		\
	xhi = _mm_mulhi_epi16(xhi, abuf);		\
	xlo = _mm_add_epi16(xlo, bbuf);			\
	xhi = _mm_add_epi16(xhi, bbuf);			\
	_mm_##st##_si128((__m128i *)pz, xlo);		\
	_mm_##st##_si128((__m128i *)pz + 1, xhi);	\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorScale_S16_S8_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *a,
	const mlib_s8 *b,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, a0, b0, ax, az, nstep, n1, n2, n3;
	mlib_s8 *px = (mlib_s8 *)x;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf, abuf, bbuf, xlo, xhi, ylo, yhi;
	__m128i zero, mask1, mask2;

	a0 = a[0];
	b0 = b[0];

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = a0 * (*px++) + b0;
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		abuf = _mm_set1_epi16(a0 << 8);
		bbuf = _mm_set1_epi16(b0);

		for (i = 0; i < n1; i++) {
			sum = a0 * (*px++) + b0;
			*pz++ = sum;
		}

		if ((ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				SCALE_S16S8_MOD(load, store);
			}
		} else {
			for (i = 0; i < n2; i++) {
				SCALE_S16S8_MOD(load, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = a0 * (*px++) + b0;
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SCALE_S16_SAT(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);	\
						\
	xlo = _mm_unpacklo_epi16(xbuf, bbuf);	\
	xhi = _mm_unpackhi_epi16(xbuf, bbuf);	\
	xlo = _mm_madd_epi16(xlo, abuf);	\
	xhi = _mm_madd_epi16(xhi, abuf);	\
	zbuf = _mm_packs_epi32(xlo, xhi);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	pz += nstep

mlib_status
__mlib_VectorScale_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *a,
	const mlib_s16 *b,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, a0, b0, ax, az, nstep, n1, n2, n3;
	mlib_s16 *px = (mlib_s16 *)x, *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf, abuf, bbuf, xlo, xhi, ylo, yhi;
	__m128i zero, mask1, mask2;

	a0 = a[0];
	b0 = b[0];

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = a0 * (*px++) + b0;
			SAT_S16(sum);
			*pz++ = sum;
		}
	} else {
		abuf = _mm_set1_epi32(a0 & 0xffff | 0x10000);
		bbuf = _mm_set1_epi16(b0);

		for (i = 0; i < n1; i++) {
			sum = a0 * (*px++) + b0;
			SAT_S16(sum);
			*pz++ = sum;
		}

		if (ax == az) {
			for (i = 0; i < n2; i++) {
				SCALE_S16_SAT(load, store);
			}
		} else {
			for (i = 0; i < n2; i++) {
				SCALE_S16_SAT(load, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = a0 * (*px++) + b0;
			SAT_S16(sum);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SCALE_S16_MOD(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);	\
						\
	zbuf = _mm_mullo_epi16(xbuf, abuf);	\
	zbuf = _mm_add_epi16(zbuf, bbuf);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	pz += nstep

mlib_status
__mlib_VectorScale_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *a,
	const mlib_s16 *b,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, a0, b0, ax, az, nstep, n1, n2, n3;
	mlib_s16 *px = (mlib_s16 *)x, *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf, abuf, bbuf, xlo, xhi, ylo, yhi;
	__m128i zero, mask1, mask2;

	a0 = a[0];
	b0 = b[0];

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = a0 * (*px++) + b0;
			*pz++ = sum;
		}
	} else {
		abuf = _mm_set1_epi16(a0);
		bbuf = _mm_set1_epi16(b0);

		for (i = 0; i < n1; i++) {
			sum = a0 * (*px++) + b0;
			*pz++ = sum;
		}

		if (ax == az) {
			for (i = 0; i < n2; i++) {
				SCALE_S16_MOD(load, store);
			}
		} else {
			for (i = 0; i < n2; i++) {
				SCALE_S16_MOD(load, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = a0 * (*px++) + b0;
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SCALE_S32S16_MOD(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	xlo = _mm_unpacklo_epi16(xbuf, bbuf);		\
	xhi = _mm_unpackhi_epi16(xbuf, bbuf);		\
	xlo = _mm_madd_epi16(xlo, abuf);		\
	xhi = _mm_madd_epi16(xhi, abuf);		\
	_mm_##st##_si128((__m128i *)pz, xlo);		\
	_mm_##st##_si128((__m128i *)pz + 1, xhi);	\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorScale_S32_S16_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *a,
	const mlib_s16 *b,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, a0, b0, ax, az, nstep, n1, n2, n3;
	mlib_s16 *px = (mlib_s16 *)x;
	mlib_s32 *pz = (mlib_s32 *)z;
	__m128i xbuf, ybuf, zbuf, abuf, bbuf, xlo, xhi, ylo, yhi;

	a0 = a[0];
	b0 = b[0];

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = a0 * (*px++) + b0;
			*pz++ = sum;
		}
	} else {
		abuf = _mm_set1_epi32(a0 & 0xffff | 0x10000);
		bbuf = _mm_set1_epi16(b0);

		for (i = 0; i < n1; i++) {
			sum = a0 * (*px++) + b0;
			*pz++ = sum;
		}

		if ((ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				SCALE_S32S16_MOD(load, store);
			}
		} else {
			for (i = 0; i < n2; i++) {
				SCALE_S32S16_MOD(load, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = a0 * (*px++) + b0;
			*pz++ = sum;
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

#define	SCALE_MOD						\
	sum0 = (mlib_s32)px[0] * a0 - (mlib_s32)px[1] * a1;	\
	sum1 = (mlib_s32)px[0] * a1 + (mlib_s32)px[1] * a0;	\
	pz[0] = sum0 + b0;					\
	pz[1] = sum1 + b1;					\
	px += 2;						\
	pz += 2

#define	SCALE_SAT(TYPE)						\
	sum0 = (mlib_s32)px[0] * a0 - (mlib_s32)px[1] * a1;	\
	sum1 = (mlib_s32)px[0] * a1 + (mlib_s32)px[1] * a0;	\
	sum0 += b0;						\
	sum1 += b1;						\
	SAT_##TYPE(sum0);					\
	SAT_##TYPE(sum1);					\
	pz[0] = sum0;						\
	pz[1] = sum1;						\
	px += 2;						\
	pz += 2

/* *********************************************************** */

#define	SCALE_U8C_SAT_128(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	buf1 = _mm_unpacklo_epi8(xbuf, zero);		\
	buf2 = _mm_unpackhi_epi8(xbuf, zero);		\
	buf3 = _mm_madd_epi16(buf1, abuf0);		\
	buf4 = _mm_madd_epi16(buf2, abuf0);		\
	buf1 = _mm_madd_epi16(buf1, abuf1);		\
	buf2 = _mm_madd_epi16(buf2, abuf1);		\
	rbuf = _mm_packs_epi32(buf3, buf4);		\
	ibuf = _mm_packs_epi32(buf1, buf2);		\
							\
	buf1 = _mm_unpacklo_epi16(rbuf, ibuf);		\
	buf2 = _mm_unpackhi_epi16(rbuf, ibuf);		\
	buf1 = _mm_adds_epi16(buf1, bbuf0);		\
	buf2 = _mm_adds_epi16(buf2, bbuf0);		\
	buf1 = _mm_packus_epi16(buf1, buf2);		\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
							\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorScale_U8C_U8C_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *a,
	const mlib_u8 *b,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, a0, a1, b0, b1;
	mlib_s32 ax, az, n1, n2, n3, nstep;
	mlib_u8 *px = (mlib_u8 *)x, *pz = (mlib_u8 *)z;
	__m128i xbuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i abuf0, abuf1, bbuf0, bbuf1, zero;

	a0 = a[0];
	a1 = a[1];
	b0 = b[0];
	b1 = b[1];
	abuf0 = _mm_set1_epi32((a0 & 0xffff) | ((-a1) << 16));
	abuf1 = _mm_set1_epi32((a0 << 16) | (a1 & 0xffff));
	bbuf0 = _mm_set1_epi32((b0 & 0xffff) | (b1 << 16));
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_u8);

	if (ax & 1) {
		for (i = 0; i < n / 8; i++) {
			SCALE_U8C_SAT_128(loadu, storeu);
		}
		for (i = 0; i < n % 8; i++) {
			SCALE_SAT(U8);
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				SCALE_SAT(U8);
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				SCALE_SAT(U8);
			}
			if (ax == az) {
				for (i = 0; i < n2; i++) {
					SCALE_U8C_SAT_128(load, store);
				}
			} else {
				for (i = 0; i < n2; i++) {
					SCALE_U8C_SAT_128(load, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				SCALE_SAT(U8);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SCALE_U8C_MOD_128(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	buf1 = _mm_unpacklo_epi8(xbuf, zero);		\
	buf2 = _mm_unpackhi_epi8(xbuf, zero);		\
	buf3 = _mm_madd_epi16(buf1, abuf0);		\
	buf4 = _mm_madd_epi16(buf2, abuf0);		\
	buf1 = _mm_madd_epi16(buf1, abuf1);		\
	buf2 = _mm_madd_epi16(buf2, abuf1);		\
							\
	buf3 = _mm_and_si128(buf3, mask1);		\
	buf4 = _mm_and_si128(buf4, mask1);		\
	buf1 = _mm_slli_epi32(buf1, 8);			\
	buf2 = _mm_slli_epi32(buf2, 8);			\
	buf3 = _mm_or_si128(buf3, buf1);		\
	buf4 = _mm_or_si128(buf4, buf2);		\
							\
	buf1 = _mm_unpacklo_epi32(buf3, buf4);		\
	buf2 = _mm_unpackhi_epi32(buf3, buf4);		\
	buf3 = _mm_unpacklo_epi32(buf1, buf2);		\
	buf4 = _mm_unpackhi_epi32(buf1, buf2);		\
	buf3 = _mm_and_si128(buf3, mask2);		\
	buf4 = _mm_slli_epi32(buf4, 16);		\
	buf3 = _mm_or_si128(buf3, buf4);		\
	buf1 = _mm_add_epi8(buf3, bbuf0);		\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
							\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorScale_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *a,
	const mlib_u8 *b,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, a0, a1, b0, b1;
	mlib_s32 ax, az, n1, n2, n3, nstep;
	mlib_u8 *px = (mlib_u8 *)x, *pz = (mlib_u8 *)z;
	__m128i xbuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i abuf0, abuf1, bbuf0, bbuf1, zero, mask1, mask2;

	a0 = a[0];
	a1 = a[1];
	b0 = b[0];
	b1 = b[1];
	abuf0 = _mm_set1_epi32((a0 & 0xffff) | ((-a1) << 16));
	abuf1 = _mm_set1_epi32((a0 << 16) | (a1 & 0xffff));
	bbuf0 = _mm_set1_epi16((b0 & 0xff) | (b1 << 8));
	mask1 = _mm_set1_epi16(0x00ff);
	mask2 = _mm_set1_epi32(0xffff);
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_u8);

	if (ax & 1) {
		for (i = 0; i < n / 8; i++) {
			SCALE_U8C_MOD_128(loadu, storeu);
		}
		for (i = 0; i < n % 8; i++) {
			SCALE_MOD;
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				SCALE_MOD;
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				SCALE_MOD;
			}
			if (ax == az) {
				for (i = 0; i < n2; i++) {
					SCALE_U8C_MOD_128(load, store);
				}
			} else {
				for (i = 0; i < n2; i++) {
					SCALE_U8C_MOD_128(load, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				SCALE_MOD;
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SCALE_S16CU8C_SAT_128(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	buf1 = _mm_unpacklo_epi8(xbuf, zero);		\
	buf2 = _mm_unpackhi_epi8(xbuf, zero);		\
	buf3 = _mm_madd_epi16(buf1, abuf0);		\
	buf4 = _mm_madd_epi16(buf2, abuf0);		\
	buf1 = _mm_madd_epi16(buf1, abuf1);		\
	buf2 = _mm_madd_epi16(buf2, abuf1);		\
							\
	buf3 = _mm_add_epi32(buf3, bbuf0);		\
	buf4 = _mm_add_epi32(buf4, bbuf0);		\
	buf1 = _mm_add_epi32(buf1, bbuf1);		\
	buf2 = _mm_add_epi32(buf2, bbuf1);		\
	rbuf = _mm_packs_epi32(buf3, buf4);		\
	ibuf = _mm_packs_epi32(buf1, buf2);		\
	buf1 = _mm_unpacklo_epi16(rbuf, ibuf);		\
	buf2 = _mm_unpackhi_epi16(rbuf, ibuf);		\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
	_mm_##st##_si128((__m128i *)pz + 1, buf2);	\
							\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorScale_S16C_U8C_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *a,
	const mlib_u8 *b,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, a0, a1, b0, b1;
	mlib_s32 ax, az, n1, n2, n3, nstep;
	mlib_u8 *px = (mlib_u8 *)x;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i abuf0, abuf1, bbuf0, bbuf1, zero, mask;

	a0 = a[0];
	a1 = a[1];
	b0 = b[0];
	b1 = b[1];
	abuf0 = _mm_set1_epi32((a0 & 0xffff) | ((-a1) << 16));
	abuf1 = _mm_set1_epi32((a0 << 16) | (a1 & 0xffff));
	bbuf0 = _mm_set1_epi32(b0);
	bbuf1 = _mm_set1_epi32(b1);
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_u8);

	if (ax & 1) {
		for (i = 0; i < n / 8; i++) {
			SCALE_S16CU8C_SAT_128(loadu, storeu);
		}
		for (i = 0; i < n % 8; i++) {
			SCALE_SAT(S16);
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				SCALE_SAT(S16);
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				SCALE_SAT(S16);
			}
			if ((ax * 2 & 15) == az) {
				for (i = 0; i < n2; i++) {
					SCALE_S16CU8C_SAT_128(load, store);
				}
			} else {
				for (i = 0; i < n2; i++) {
					SCALE_S16CU8C_SAT_128(load, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				SCALE_SAT(S16);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SCALE_S16CU8C_MOD_128(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	buf1 = _mm_unpacklo_epi8(xbuf, zero);		\
	buf2 = _mm_unpackhi_epi8(xbuf, zero);		\
	buf3 = _mm_madd_epi16(buf1, abuf0);		\
	buf4 = _mm_madd_epi16(buf2, abuf0);		\
	buf1 = _mm_madd_epi16(buf1, abuf1);		\
	buf2 = _mm_madd_epi16(buf2, abuf1);		\
							\
	buf3 = _mm_and_si128(buf3, mask);		\
	buf4 = _mm_and_si128(buf4, mask);		\
	buf1 = _mm_slli_epi32(buf1, 16);		\
	buf2 = _mm_slli_epi32(buf2, 16);		\
	buf1 = _mm_or_si128(buf3, buf1);		\
	buf2 = _mm_or_si128(buf4, buf2);		\
							\
	buf1 = _mm_add_epi16(buf1, bbuf0);		\
	buf2 = _mm_add_epi16(buf2, bbuf0);		\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
	_mm_##st##_si128((__m128i *)pz + 1, buf2);	\
							\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorScale_S16C_U8C_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *a,
	const mlib_u8 *b,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, a0, a1, b0, b1;
	mlib_s32 ax, az, n1, n2, n3, nstep;
	mlib_u8 *px = (mlib_u8 *)x;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i abuf0, abuf1, bbuf0, bbuf1, zero, mask;

	a0 = a[0];
	a1 = a[1];
	b0 = b[0];
	b1 = b[1];
	abuf0 = _mm_set1_epi32((a0 & 0xffff) | ((-a1) << 16));
	abuf1 = _mm_set1_epi32((a0 << 16) | (a1 & 0xffff));
	bbuf0 = _mm_set1_epi32((b0 & 0xffff) | (b1 << 16));
	mask = _mm_set1_epi32(0xffff);
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_u8);

	if (ax & 1) {
		for (i = 0; i < n / 8; i++) {
			SCALE_S16CU8C_MOD_128(loadu, storeu);
		}
		for (i = 0; i < n % 8; i++) {
			SCALE_MOD;
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				SCALE_MOD;
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				SCALE_MOD;
			}
			if ((ax * 2 & 15) == az) {
				for (i = 0; i < n2; i++) {
					SCALE_S16CU8C_MOD_128(load, store);
				}
			} else {
				for (i = 0; i < n2; i++) {
					SCALE_S16CU8C_MOD_128(load, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				SCALE_MOD;
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SCALE_S8C_SAT_128(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	buf1 = _mm_unpacklo_epi8(zero, xbuf);		\
	buf2 = _mm_unpackhi_epi8(zero, xbuf);		\
	buf1 = _mm_srai_epi16(buf1, 8);			\
	buf2 = _mm_srai_epi16(buf2, 8);			\
	buf3 = _mm_madd_epi16(buf1, abuf0);		\
	buf4 = _mm_madd_epi16(buf2, abuf0);		\
	buf1 = _mm_madd_epi16(buf1, abuf1);		\
	buf2 = _mm_madd_epi16(buf2, abuf1);		\
	rbuf = _mm_packs_epi32(buf3, buf4);		\
	ibuf = _mm_packs_epi32(buf1, buf2);		\
							\
	buf1 = _mm_unpacklo_epi16(rbuf, ibuf);		\
	buf2 = _mm_unpackhi_epi16(rbuf, ibuf);		\
	buf1 = _mm_adds_epi16(buf1, bbuf0);		\
	buf2 = _mm_adds_epi16(buf2, bbuf0);		\
	buf1 = _mm_packs_epi16(buf1, buf2);		\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
							\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorScale_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *a,
	const mlib_s8 *b,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, a0, a1, b0, b1;
	mlib_s32 ax, az, n1, n2, n3, nstep;
	mlib_s8 *px = (mlib_s8 *)x, *pz = (mlib_s8 *)z;
	__m128i xbuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i abuf0, abuf1, bbuf0, bbuf1, zero;

	a0 = a[0];
	a1 = a[1];
	b0 = b[0];
	b1 = b[1];
	abuf0 = _mm_set1_epi32((a0 & 0xffff) | ((-a1) << 16));
	abuf1 = _mm_set1_epi32((a0 << 16) | (a1 & 0xffff));
	bbuf0 = _mm_set1_epi32((b0 & 0xffff) | (b1 << 16));
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_s8);

	if (ax & 1) {
		for (i = 0; i < n / 8; i++) {
			SCALE_S8C_SAT_128(loadu, storeu);
		}
		for (i = 0; i < n % 8; i++) {
			SCALE_SAT(S8);
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s8);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				SCALE_SAT(S8);
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				SCALE_SAT(S8);
			}
			if (ax == az) {
				for (i = 0; i < n2; i++) {
					SCALE_S8C_SAT_128(load, store);
				}
			} else {
				for (i = 0; i < n2; i++) {
					SCALE_S8C_SAT_128(load, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				SCALE_SAT(S8);
			}
		}
	}

	return (MLIB_SUCCESS);
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
	return __mlib_VectorScale_U8C_U8C_Mod((mlib_u8 *)z, (mlib_u8 *)x,
		(mlib_u8 *)a, (mlib_u8 *)b, n);
}

/* *********************************************************** */

#define	SCALE_S16CS8C_SAT_128(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	buf1 = _mm_unpacklo_epi8(zero, xbuf);		\
	buf2 = _mm_unpackhi_epi8(zero, xbuf);		\
	buf1 = _mm_srai_epi16(buf1, 8);			\
	buf2 = _mm_srai_epi16(buf2, 8);			\
	buf3 = _mm_madd_epi16(buf1, abuf0);		\
	buf4 = _mm_madd_epi16(buf2, abuf0);		\
	buf1 = _mm_madd_epi16(buf1, abuf1);		\
	buf2 = _mm_madd_epi16(buf2, abuf1);		\
							\
	buf3 = _mm_add_epi32(buf3, bbuf0);		\
	buf4 = _mm_add_epi32(buf4, bbuf0);		\
	buf1 = _mm_add_epi32(buf1, bbuf1);		\
	buf2 = _mm_add_epi32(buf2, bbuf1);		\
	rbuf = _mm_packs_epi32(buf3, buf4);		\
	ibuf = _mm_packs_epi32(buf1, buf2);		\
	buf1 = _mm_unpacklo_epi16(rbuf, ibuf);		\
	buf2 = _mm_unpackhi_epi16(rbuf, ibuf);		\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
	_mm_##st##_si128((__m128i *)pz + 1, buf2);	\
							\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorScale_S16C_S8C_Sat(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *a,
	const mlib_s8 *b,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, a0, a1, b0, b1;
	mlib_s32 ax, az, n1, n2, n3, nstep;
	mlib_s8 *px = (mlib_s8 *)x;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i abuf0, abuf1, bbuf0, bbuf1, zero, mask;

	a0 = a[0];
	a1 = a[1];
	b0 = b[0];
	b1 = b[1];
	abuf0 = _mm_set1_epi32((a0 & 0xffff) | ((-a1) << 16));
	abuf1 = _mm_set1_epi32((a0 << 16) | (a1 & 0xffff));
	bbuf0 = _mm_set1_epi32(b0);
	bbuf1 = _mm_set1_epi32(b1);
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_s8);

	if (ax & 1) {
		for (i = 0; i < n / 8; i++) {
			SCALE_S16CS8C_SAT_128(loadu, storeu);
		}
		for (i = 0; i < n % 8; i++) {
			SCALE_SAT(S16);
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s8);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				SCALE_SAT(S16);
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				SCALE_SAT(S16);
			}
			if ((ax * 2 & 15) == az) {
				for (i = 0; i < n2; i++) {
					SCALE_S16CS8C_SAT_128(load, store);
				}
			} else {
				for (i = 0; i < n2; i++) {
					SCALE_S16CS8C_SAT_128(load, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				SCALE_SAT(S16);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SCALE_S16CS8C_MOD_128(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	buf1 = _mm_unpacklo_epi8(zero, xbuf);		\
	buf2 = _mm_unpackhi_epi8(zero, xbuf);		\
	buf1 = _mm_srai_epi16(buf1, 8);			\
	buf2 = _mm_srai_epi16(buf2, 8);			\
	buf3 = _mm_madd_epi16(buf1, abuf0);		\
	buf4 = _mm_madd_epi16(buf2, abuf0);		\
	buf1 = _mm_madd_epi16(buf1, abuf1);		\
	buf2 = _mm_madd_epi16(buf2, abuf1);		\
							\
	buf3 = _mm_and_si128(buf3, mask);		\
	buf4 = _mm_and_si128(buf4, mask);		\
	buf1 = _mm_slli_epi32(buf1, 16);		\
	buf2 = _mm_slli_epi32(buf2, 16);		\
	buf1 = _mm_or_si128(buf3, buf1);		\
	buf2 = _mm_or_si128(buf4, buf2);		\
							\
	buf1 = _mm_add_epi16(buf1, bbuf0);		\
	buf2 = _mm_add_epi16(buf2, bbuf0);		\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
	_mm_##st##_si128((__m128i *)pz + 1, buf2);	\
							\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorScale_S16C_S8C_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *a,
	const mlib_s8 *b,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, a0, a1, b0, b1;
	mlib_s32 ax, az, n1, n2, n3, nstep;
	mlib_s8 *px = (mlib_s8 *)x;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i abuf0, abuf1, bbuf0, bbuf1, zero, mask;

	a0 = a[0];
	a1 = a[1];
	b0 = b[0];
	b1 = b[1];
	abuf0 = _mm_set1_epi32((a0 & 0xffff) | ((-a1) << 16));
	abuf1 = _mm_set1_epi32((a0 << 16) | (a1 & 0xffff));
	bbuf0 = _mm_set1_epi32((b0 & 0xffff) | (b1 << 16));
	mask = _mm_set1_epi32(0xffff);
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_s8);

	if (ax & 1) {
		for (i = 0; i < n / 8; i++) {
			SCALE_S16CS8C_MOD_128(loadu, storeu);
		}
		for (i = 0; i < n % 8; i++) {
			SCALE_MOD;
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s8);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				SCALE_MOD;
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				SCALE_MOD;
			}
			if ((ax * 2 & 15) == az) {
				for (i = 0; i < n2; i++) {
					SCALE_S16CS8C_MOD_128(load, store);
				}
			} else {
				for (i = 0; i < n2; i++) {
					SCALE_S16CS8C_MOD_128(load, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				SCALE_MOD;
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SCALE_S16C_SAT_128(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	/* -S16_MIN*S16_MIN = S16_MIN*S16_MIN */	\
	buf1 = _mm_madd_epi16(xbuf, abuf00);		\
	buf2 = _mm_madd_epi16(xbuf, abuf01);		\
	rbuf = _mm_sub_epi32(buf1, buf2);		\
	/* S16_MIN*S16_MIN+S16_MIN*S16_MIN > S32_MAX */	\
	ibuf = _mm_madd_epi16(xbuf, abuf1);		\
	buf3 = _mm_cmpeq_epi32(ibuf, mask);		\
	buf3 = _mm_slli_epi32(buf3, 16);		\
	ibuf = _mm_xor_si128(ibuf, buf3);		\
							\
	buf1 = _mm_unpacklo_epi32(rbuf, ibuf);		\
	buf2 = _mm_unpackhi_epi32(rbuf, ibuf);		\
	buf1 = _mm_add_epi32(buf1, bbuf0);		\
	buf2 = _mm_add_epi32(buf2, bbuf0);		\
	buf1 = _mm_packs_epi32(buf1, buf2);		\
							\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
							\
	px += nstep;					\
	pz += nstep

#define	SCALE_S16C_SAT						\
	sum0 = (mlib_s32)px[0] * a0 - (mlib_s32)px[1] * a1;	\
	sum1 = (mlib_s32)px[0] * a1 + (mlib_s32)px[1] * a0;	\
	if (sum1 == MLIB_S32_MIN)				\
		pz[1] = MLIB_S16_MAX;				\
	else {							\
		sum1 += b1;					\
		SAT_S16(sum1);					\
		pz[1] = sum1;					\
	}							\
	sum0 += b0;						\
	SAT_S16(sum0);						\
	pz[0] = sum0;						\
	px += 2;						\
	pz += 2

mlib_status
__mlib_VectorScale_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *a,
	const mlib_s16 *b,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, a0, a1, b0, b1;
	mlib_s32 ax, az, n1, n2, n3, nstep;
	mlib_s16 *px = (mlib_s16 *)x;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i abuf00, abuf01, abuf1, bbuf0, bbuf1, zero, mask;

	a0 = a[0];
	a1 = a[1];
	b0 = b[0];
	b1 = b[1];
	abuf00 = _mm_set1_epi32(a0 & 0xffff);
	abuf01 = _mm_set1_epi32(a1 << 16);
	abuf1 = _mm_set1_epi32((a0 << 16) | (a1 & 0xffff));
	bbuf0 = _mm_setr_epi32(b0, b1, b0, b1);
	mask = _mm_set1_epi32(0x80000000);

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_s16);

	if (ax & 3) {
		for (i = 0; i < n / 4; i++) {
			SCALE_S16C_SAT_128(loadu, storeu);
		}
		for (i = 0; i < n % 4; i++) {
			SCALE_S16C_SAT;
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				SCALE_S16C_SAT;
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				SCALE_S16C_SAT;
			}
			if (ax == az) {
				for (i = 0; i < n2; i++) {
					SCALE_S16C_SAT_128(load, store);
				}
			} else {
				for (i = 0; i < n2; i++) {
					SCALE_S16C_SAT_128(load, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				SCALE_S16C_SAT;
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SCALE_S16C_MOD_128(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	/* -S16_MIN*S16_MIN = S16_MIN*S16_MIN */	\
	buf1 = _mm_madd_epi16(xbuf, abuf00);		\
	buf2 = _mm_madd_epi16(xbuf, abuf01);		\
	rbuf = _mm_sub_epi32(buf1, buf2);		\
	ibuf = _mm_madd_epi16(xbuf, abuf1);		\
							\
	rbuf = _mm_and_si128(rbuf, mask);		\
	ibuf = _mm_slli_epi32(ibuf, 16);		\
	buf1 = _mm_or_si128(rbuf, ibuf);		\
	buf1 = _mm_add_epi16(buf1, bbuf0);		\
							\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
							\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorScale_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *a,
	const mlib_s16 *b,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, a0, a1, b0, b1;
	mlib_s32 ax, az, n1, n2, n3, nstep;
	mlib_s16 *px = (mlib_s16 *)x;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i abuf00, abuf01, abuf1, bbuf0, bbuf1, zero, mask;

	a0 = a[0];
	a1 = a[1];
	b0 = b[0];
	b1 = b[1];
	abuf00 = _mm_set1_epi32(a0 & 0xffff);
	abuf01 = _mm_set1_epi32(a1 << 16);
	abuf1 = _mm_set1_epi32((a0 << 16) | (a1 & 0xffff));
	bbuf0 = _mm_set1_epi32((b1 << 16) | (b0 & 0xffff));
	mask = _mm_set1_epi32(0xffff);

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_s16);

	if (ax & 3) {
		for (i = 0; i < n / 4; i++) {
			SCALE_S16C_MOD_128(loadu, storeu);
		}
		for (i = 0; i < n % 4; i++) {
			SCALE_MOD;
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				SCALE_MOD;
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				SCALE_MOD;
			}
			if (ax == az) {
				for (i = 0; i < n2; i++) {
					SCALE_S16C_MOD_128(load, store);
				}
			} else {
				for (i = 0; i < n2; i++) {
					SCALE_S16C_MOD_128(load, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				SCALE_MOD;
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SCALE_S32CS16C_SAT_128(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	/* -S16_MIN*S16_MIN = S16_MIN*S16_MIN */	\
	buf1 = _mm_madd_epi16(xbuf, abuf00);		\
	buf2 = _mm_madd_epi16(xbuf, abuf01);		\
	rbuf = _mm_sub_epi32(buf1, buf2);		\
	/* S16_MIN*S16_MIN+S16_MIN*S16_MIN > S32_MAX */	\
	ibuf = _mm_madd_epi16(xbuf, abuf1);		\
	buf1 = _mm_cmpeq_epi32(ibuf, mask1);		\
	buf1 = _mm_and_si128(buf1, mask2);		\
	buf1 = _mm_or_si128(buf1, bbuf1);		\
							\
	rbuf = _mm_add_epi32(rbuf, bbuf0);		\
	ibuf = _mm_add_epi32(ibuf, buf1);		\
	buf1 = _mm_unpacklo_epi32(rbuf, ibuf);		\
	buf2 = _mm_unpackhi_epi32(rbuf, ibuf);		\
							\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
	_mm_##st##_si128((__m128i *)pz + 1, buf2);	\
							\
	px += nstep;					\
	pz += nstep

#define	SCALE_S32CS16C_SAT					\
	sum0 = (mlib_s32)px[0] * a0 - (mlib_s32)px[1] * a1;	\
	sum1 = (mlib_s32)px[0] * a1 + (mlib_s32)px[1] * a0;	\
	pz[0] = sum0 + b0;					\
	if (sum1 == MLIB_S32_MIN && b1 > -1)			\
		pz[1] = MLIB_S32_MAX;				\
	else							\
		pz[1] = sum1 + b1;				\
	px += 2;						\
	pz += 2

mlib_status
__mlib_VectorScale_S32C_S16C_Sat(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *a,
	const mlib_s16 *b,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, a0, a1, b0, b1;
	mlib_s32 ax, az, n1, n2, n3, nstep;
	mlib_s16 *px = (mlib_s16 *)x;
	mlib_s32 *pz = (mlib_s32 *)z;
	__m128i xbuf, rbuf, ibuf, buf1, buf2;
	__m128i abuf00, abuf01, abuf1, bbuf0, bbuf1, mask1, mask2;

	a0 = a[0];
	a1 = a[1];
	b0 = b[0];
	b1 = b[1];
	abuf00 = _mm_set1_epi32(a0 & 0xffff);
	abuf01 = _mm_set1_epi32(a1 << 16);
	abuf1 = _mm_set1_epi32((a0 << 16) | (a1 & 0xffff));
	bbuf0 = _mm_set1_epi32(b0);
	bbuf1 = _mm_set1_epi32(b1);
	mask1 = _mm_set1_epi32(0x80000000);
	mask2 = _mm_set1_epi32(-1);
	mask2 = _mm_cmpgt_epi32(bbuf1, mask2);

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_s16);

	if (ax & 3) {
		for (i = 0; i < n / 4; i++) {
			SCALE_S32CS16C_SAT_128(loadu, storeu);
		}
		for (i = 0; i < n % 4; i++) {
			SCALE_S32CS16C_SAT;
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				SCALE_S32CS16C_SAT;
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				SCALE_S32CS16C_SAT;
			}
			if ((ax * 2 & 15) == az) {
				for (i = 0; i < n2; i++) {
					SCALE_S32CS16C_SAT_128(load, store);
				}
			} else {
				for (i = 0; i < n2; i++) {
					SCALE_S32CS16C_SAT_128(load, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				SCALE_S32CS16C_SAT;
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SCALE_S32CS16C_MOD_128(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	/* -S16_MIN*S16_MIN = S16_MIN*S16_MIN */	\
	buf1 = _mm_madd_epi16(xbuf, abuf00);		\
	buf2 = _mm_madd_epi16(xbuf, abuf01);		\
	rbuf = _mm_sub_epi32(buf1, buf2);		\
	ibuf = _mm_madd_epi16(xbuf, abuf1);		\
							\
	buf1 = _mm_unpacklo_epi32(rbuf, ibuf);		\
	buf2 = _mm_unpackhi_epi32(rbuf, ibuf);		\
	buf1 = _mm_add_epi32(buf1, bbuf0);		\
	buf2 = _mm_add_epi32(buf2, bbuf0);		\
							\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
	_mm_##st##_si128((__m128i *)pz + 1, buf2);	\
							\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorScale_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *a,
	const mlib_s16 *b,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, a0, a1, b0, b1;
	mlib_s32 ax, az, n1, n2, n3, nstep;
	mlib_s16 *px = (mlib_s16 *)x;
	mlib_s32 *pz = (mlib_s32 *)z;
	__m128i xbuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i abuf00, abuf01, abuf1, bbuf0, bbuf1, zero, mask;

	a0 = a[0];
	a1 = a[1];
	b0 = b[0];
	b1 = b[1];
	abuf00 = _mm_set1_epi32(a0 & 0xffff);
	abuf01 = _mm_set1_epi32(a1 << 16);
	abuf1 = _mm_set1_epi32((a0 << 16) | (a1 & 0xffff));
	bbuf0 = _mm_setr_epi32(b0, b1, b0, b1);

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_s16);

	if (ax & 3) {
		for (i = 0; i < n / 4; i++) {
			SCALE_S32CS16C_MOD_128(loadu, storeu);
		}
		for (i = 0; i < n % 4; i++) {
			SCALE_MOD;
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				SCALE_MOD;
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				SCALE_MOD;
			}
			if ((ax * 2 & 15) == az) {
				for (i = 0; i < n2; i++) {
					SCALE_S32CS16C_MOD_128(load, store);
				}
			} else {
				for (i = 0; i < n2; i++) {
					SCALE_S32CS16C_MOD_128(load, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				SCALE_MOD;
			}
		}
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
