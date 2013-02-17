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

#pragma ident	"@(#)mlib_i_VectorMulS.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorMulS_U8_[Sat|Mod],
 *      mlib_VectorMulS_U8_[U8|S16]_[Sat|Mod]   - multiplication of
 *                                                unsigned 8-bit format vector
 *                                                to a scalar
 *      mlib_VectorMulS_S8_[Sat|Mod],
 *      mlib_VectorMulS_S8_[S8|S16]_[Sat|Mod]   - multiplication of
 *                                                signed 8-bit format vector
 *                                                to a scalar
 *      mlib_VectorMulS_S16_[Sat|Mod],
 *      mlib_VectorMulS_S16_[S16|S32]_[Sat|Mod] - multiplication of
 *                                                signed 16-bit format vector
 *                                                to a scalar
 *      mlib_VectorMulS_S32_[Sat|Mod],
 *      mlib_VectorMulS_S32_S32_[Sat|Mod]       - multiplication of
 *                                                signed 32-bit format vector
 *                                                to a scalar
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorMulS_U8_U8_[Sat|Mod](mlib_u8       * z,
 *                                                  const mlib_u8 * x,
 *                                                  const mlib_u8 * c,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorMulS_S8_S8_[Sat|Mod](mlib_s8       * z,
 *                                                  const mlib_s8 * x,
 *                                                  const mlib_s8 * c,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorMulS_S16_U8_[Sat|Mod](mlib_s16      * z,
 *                                                   const mlib_u8 * x,
 *                                                   const mlib_u8 * c,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_VectorMulS_S16_S8_[Sat|Mod](mlib_s16      * z,
 *                                                   const mlib_s8 * x,
 *                                                   const mlib_s8 * c,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_VectorMulS_S16_S16_[Sat|Mod](mlib_s16       * z,
 *                                                    const mlib_s16 * x,
 *                                                    const mlib_s16 * c,
 *                                                    mlib_s32       n);
 *      mlib_status mlib_VectorMulS_S32_S16_[Sat|Mod](mlib_s32       * z,
 *                                                    const mlib_s16 * x,
 *                                                    const mlib_s16 * c,
 *                                                    mlib_s32       n);
 *      mlib_status mlib_VectorMulS_S32_S32_[Sat|Mod](mlib_s32       * z,
 *                                                    const mlib_s32 * x,
 *                                                    const mlib_s32 * c,
 *                                                    mlib_s32       n);
 *
 *      mlib_status mlib_VectorMulS_U8_[Sat|Mod](mlib_u8       * xz,
 *                                               const mlib_u8 * c,
 *                                               mlib_s32      n);
 *      mlib_status mlib_VectorMulS_S8_[Sat|Mod](mlib_s8       * xz,
 *                                               const mlib_s8 * c,
 *                                               mlib_s32      n);
 *      mlib_status mlib_VectorMulS_S16_[Sat|Mod](mlib_s16       * xz,
 *                                                const mlib_s16 * c,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorMulS_S32_[Sat|Mod](mlib_s32       * xz,
 *                                                const mlib_s32 * c,
 *                                                mlib_s32       n);
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      x    pointer to the first element of the input vector
 *      c    pointer to the scalar
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *      z = x * c
 *
 */

/*
 * FUNCTIONS
 *      mlib_VectorMulS_U8C_[Sat|Mod],
 *      mlib_VectorMulS_U8C_[U8C|S16C]_[Sat|Mod] - multiplication of
 *                     unsigned 8-bit format complex vector to a scalar
 *      mlib_VectorMulS_S8C_[Sat|Mod],
 *      mlib_VectorMulS_S8C_[S8C|S16C]_[Sat|Mod] - multiplication of
 *                     signed 8-bit format complex vector to a scalar
 *      mlib_VectorMulS_S16C_[Sat|Mod],
 *      mlib_VectorMulS_S16C_[S16C|S32C]_[Sat|Mod] - multiplication of
 *                     signed 16-bit format complex vector to a scalar
 *      mlib_VectorMulS_S32C_[Sat|Mod],
 *      mlib_VectorMulS_S32C_S32C_[Sat|Mod] - multiplication of
 *                     signed 32-bit format complex vector to a scalar
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorMulS_U8C_U8C_[Sat|Mod](mlib_u8       * z,
 *                                                    const mlib_u8 * x,
 *                                                    const mlib_u8 * c,
 *                                                    mlib_s32      n);
 *      mlib_status mlib_VectorMulS_S8C_S8C_[Sat|Mod](mlib_s8       * z,
 *                                                    const mlib_s8 * x,
 *                                                    const mlib_s8 * c,
 *                                                    mlib_s32      n);
 *      mlib_status mlib_VectorMulS_S16C_U8C_[Sat|Mod](mlib_s16      * z,
 *                                                     const mlib_u8 * x,
 *                                                     const mlib_u8 * c,
 *                                                     mlib_s32      n);
 *      mlib_status mlib_VectorMulS_S16C_S8C_[Sat|Mod](mlib_s16      * z,
 *                                                     const mlib_s8 * x,
 *                                                     const mlib_s8 * c,
 *                                                     mlib_s32      n);
 *      mlib_status mlib_VectorMulS_S16C_S16C_[Sat|Mod](mlib_s16       * z,
 *                                                      const mlib_s16 * x,
 *                                                      const mlib_s16 * c,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorMulS_S32C_S16C_[Sat|Mod](mlib_s32       * z,
 *                                                      const mlib_s16 * x,
 *                                                      const mlib_s16 * c,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorMulS_S32C_S32C_[Sat|Mod](mlib_s32       * z,
 *                                                      const mlib_s32 * x,
 *                                                      const mlib_s32 * c,
 *                                                      mlib_s32       n);
 *
 *      mlib_status mlib_VectorMulS_U8C_[Sat|Mod](mlib_u8       * xz,
 *                                                const mlib_u8 * c,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorMulS_S8C_[Sat|Mod](mlib_s8       * xz,
 *                                                const mlib_s8 * c,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorMulS_S16C_[Sat|Mod](mlib_s16       * xz,
 *                                                 const mlib_s16 * c,
 *                                                 mlib_s32       n);
 *      mlib_status mlib_VectorMulS_S32C_[Sat|Mod](mlib_s32       * xz,
 *                                                 const mlib_s32 * c,
 *                                                 mlib_s32       n);
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

#define	MULS_SAT(form_in, form, MAX, MIN)                       \
	mlib_s32 i;                                             \
	form mul, val_c = *((form_in *) c);                     \
	                                                        \
	for (i = 0; i < n; i++) {                               \
	    mul = ((form_in *) x)[i] * val_c;                   \
	    if (mul > MAX)                                      \
		mul = MAX;                                      \
	    else if (mul < MIN)                                 \
		mul = MIN;                                      \
	    ((form_in *) z)[i] = mul;                           \
	}                                                       \
	                                                        \
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(X)	((mlib_s32)(X))

#else /* MLIB_USE_FTOI_CLAMPING */

#define	FLOAT2INT_CLAMP(X)                                       \
	(((X) >= MLIB_S32_MAX) ? MLIB_S32_MAX : (((X) <          \
		MLIB_S32_MIN) ? MLIB_S32_MIN : (mlib_s32)(X)))

#endif /* MLIB_USE_FTOI_CLAMPING */

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_u8 valc_u8 = *c;
	mlib_d64 mul1, mul0, valc = valc_u8;
	const mlib_d64 *table = mlib_SU82D64 + 128;
	mlib_s32 i_0, i_1, n2 = (n / 2) * 2, mul0_s32, mul1_s32;

	if (n <= 0)
		return (MLIB_FAILURE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1; i_0 < n2; i_0 += 2, i_1 += 2) {

		mul0 = table[x[i_0]];
		mul1 = table[x[i_1]];

		mul0 *= valc;
		mul1 *= valc;

		mul0_s32 = (mlib_s32)mul0;
		mul1_s32 = (mlib_s32)mul1;

		z[i_0] = ((MLIB_U8_MAX - mul0_s32) >> 31) | mul0_s32;
		z[i_1] = ((MLIB_U8_MAX - mul1_s32) >> 31) | mul1_s32;
	}

	if (n % 2)
		z[n - 1] =
			((x[n - 1] * valc_u8) >
			MLIB_U8_MAX) ? MLIB_U8_MAX : (x[n - 1] * valc_u8);

	return (MLIB_SUCCESS);
}

#else
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_u8 valc_u8 = *c;
	mlib_s32 i_0, i_1;
	mlib_s32 n2 = (n >> 1) << 1;
	mlib_s32 mul1, mul0, valc = valc_u8;

	if (n <= 0)
		return (MLIB_FAILURE);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */

	for (i_0 = 0, i_1 = 1; i_0 < n2; i_0 += 2, i_1 += 2) {
		mul0 = x[i_0] * valc;
		mul1 = x[i_1] * valc;
		z[i_0] = ((MLIB_U8_MAX - mul0) >> 31) | mul0;
		z[i_1] = ((MLIB_U8_MAX - mul1) >> 31) | mul1;
	}

	if (n != n2) {
		mul0 = x[n - 1] * valc;
		z[n - 1] = ((MLIB_U8_MAX - mul0) >> 31) | mul0;
	}

	return (MLIB_SUCCESS);
}

#endif

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_U8_Sat(
	mlib_u8 *xz,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulS_U8_U8_Sat(xz, xz, c, n));
}

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16_U8_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_u8 valc_u8 = *c;
	mlib_d64 mul1, mul0, valc = valc_u8, valc_shifted_16 = valc * (1 << 16);
	const mlib_d64 *table = mlib_SU82D64 + 128;
	mlib_s32 i_0, i_1, n2 = (n / 2) * 2, mul1_s32, *z_s32 = (mlib_s32 *)z;

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((mlib_addr)z & 3) {

		mlib_s32 mul = x[0] * valc_u8;

		z[0] = (mul > MLIB_S16_MAX) ? MLIB_S16_MAX : mul;
		z++;
		x++;
		n--;
		z_s32 = (mlib_s32 *)z;
	}

	n2 = (n / 2) * 2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1; i_0 < n2; i_0 += 2, i_1 += 2, z_s32++) {

		mul0 = table[x[i_0]];
		mul1 = table[x[i_1]];

		mul0 *= valc_shifted_16;
		mul1 *= valc;

		mul1_s32 = (mlib_s32)mul1;

#ifndef _LITTLE_ENDIAN

		z_s32[0] = FLOAT2INT_CLAMP(mul0);

#else /* _LITTLE_ENDIAN */

		z[i_0] = FLOAT2INT_CLAMP(mul0)
		>>16;

#endif /* _LITTLE_ENDIAN */

		z[i_1] = (((MLIB_S16_MAX -
			mul1_s32) >> 31) | mul1_s32) & MLIB_S16_MAX;
	}

	if (n % 2) {
		mlib_s32 mul = x[n - 1] * valc_u8;

		z[n - 1] = (mul > MLIB_S16_MAX) ? MLIB_S16_MAX : mul;
	}

	return (MLIB_SUCCESS);
}

#else
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16_U8_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_u8 valc_u8 = *c;
	mlib_s32 i_0, i_1;
	mlib_s32 n2 = (n >> 1) << 1;
	mlib_s32 mul1, mul0, valc = valc_u8;
	if (n <= 0)
		return (MLIB_FAILURE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1; i_0 < n2; i_0 += 2, i_1 += 2) {

		mul0 = x[i_0] * valc;
		mul1 = x[i_1] * valc;

		z[i_0] = ((((MLIB_S16_MAX -
			mul0) >> 31) | mul0) & MLIB_S16_MAX);
		z[i_1] = ((((MLIB_S16_MAX -
			mul1) >> 31) | mul1) & MLIB_S16_MAX);
	}

	if (n != n2) {
		mul0 = x[n - 1] * valc;
		z[n - 1] = ((((MLIB_S16_MAX - mul0) >> 31) | mul0) &
				MLIB_S16_MAX);
	}

	return (MLIB_SUCCESS);
}

#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_u8 valc_u8 = *c;
	mlib_d64 mul1, mul0, valc = valc_u8;
	const mlib_d64 *table = mlib_SU82D64 + 128;
	mlib_s32 i_0, i_1, n2 = (n / 2) * 2, mul0_s32, mul1_s32;

	if (n <= 0)
		return (MLIB_FAILURE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1; i_0 < n2; i_0 += 2, i_1 += 2) {

		mul0 = table[x[i_0]];
		mul1 = table[x[i_1]];

		mul0 *= valc;
		mul1 *= valc;

		mul0_s32 = (mlib_s32)mul0;
		mul1_s32 = (mlib_s32)mul1;

		z[i_0] = mul0_s32;
		z[i_1] = mul1_s32;
	}

	if (n % 2)
		z[n - 1] = (x[n - 1] * valc_u8);
	return (MLIB_SUCCESS);
}

#else
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_u8 valc_u8 = *c;
	mlib_s32 valc = valc_u8;
	mlib_s32 i_0, i_1, n2 = (n >> 1) << 1;

	if (n <= 0)
		return (MLIB_FAILURE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1; i_0 < n2; i_0 += 2, i_1 += 2) {
		z[i_0] = x[i_0] * valc;
		z[i_1] = x[i_1] * valc;
	}

	if (n != n2)
		z[n - 1] = x[n - 1] * valc_u8;

	return (MLIB_SUCCESS);
}

#endif
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_U8_Mod(
	mlib_u8 *xz,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulS_U8_U8_Mod(xz, xz, c, n));
}

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16_U8_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_u8 valc_u8 = *c;
	mlib_d64 mul1, mul0, valc = valc_u8;
	const mlib_d64 *table = mlib_SU82D64 + 128;
	mlib_s32 i_0, i_1, n2, mul0_s32, mul1_s32, *z_s32 = (mlib_s32 *)z;

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((mlib_addr)z & 3) {
		z[0] = x[0] * (mlib_s32)valc_u8;
		z++;
		x++;
		n--;
		z_s32 = (mlib_s32 *)z;
	}

	n2 = (n / 2) * 2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1; i_0 < n2; i_0 += 2, i_1 += 2, z_s32++) {

		mul0 = table[x[i_0]];
		mul1 = table[x[i_1]];

		mul0 *= valc;
		mul1 *= valc;

		mul0_s32 = (mlib_s32)mul0;
		mul1_s32 = (mlib_s32)mul1;

#ifndef _LITTLE_ENDIAN
		z_s32[0] = mul1_s32;
#else /* _LITTLE_ENDIAN */
		z[i_1] = mul1_s32;
#endif /* _LITTLE_ENDIAN */
		z[i_0] = mul0_s32;
	}

	if (n % 2) {
		z[n - 1] = x[n - 1] * (mlib_s32)valc_u8;
	}

	return (MLIB_SUCCESS);
}

#else
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16_U8_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_u8  valc_u8 = *c;
	mlib_s32 valc = valc_u8;
	mlib_s32 i_0, i_1, n2;

	if (n <= 0)
		return (MLIB_FAILURE);

	n2 = (n >> 1) << 1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1; i_0 < n2; i_0 += 2, i_1 += 2) {
		z[i_0] = x[i_0] * valc;
		z[i_1] = x[i_1] * valc;
	}

	if (n != n2) {
		z[n - 1] = x[n - 1] * valc;
	}

	return (MLIB_SUCCESS);
}
#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_s8 valc_s8 = *c;
	mlib_d64 mul1, mul0, valc = valc_s8;
	const mlib_d64 *table = mlib_SU82D64 + 128;
	mlib_s32 i_0, i_1, n2 = (n / 2) * 2;

	valc *= 0x1000000;

	if (n <= 0)
		return (MLIB_FAILURE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1; i_0 < n2; i_0 += 2, i_1 += 2) {

		mul0 = table[x[i_0]];
		mul1 = table[x[i_1]];

		mul0 *= valc;
		mul1 *= valc;

		z[i_0] = FLOAT2INT_CLAMP(mul0) >> 24;
		z[i_1] = FLOAT2INT_CLAMP(mul1) >> 24;
	}

	if (n % 2) {
		mlib_s32 mul = x[n - 1] * valc_s8;

		z[n - 1] =
			(mul > MLIB_S8_MAX) ? MLIB_S8_MAX : (mul <
			MLIB_S8_MIN ? MLIB_S8_MIN : mul);
	}

	return (MLIB_SUCCESS);
}
#else
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_s8 valc_s8 = *c;
	mlib_s32 mul1, mul0, valc = valc_s8;
	mlib_s32 i_0, i_1, n2 = (n >> 1) << 1;

	if (n <= 0)
		return (MLIB_FAILURE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1; i_0 < n2; i_0 += 2, i_1 += 2) {

		mul0 = x[i_0] * valc;
		mul1 = x[i_1] * valc;

		z[i_0] =
			(mul0 > MLIB_S8_MAX) ? MLIB_S8_MAX : (mul0 <
			MLIB_S8_MIN ? MLIB_S8_MIN : mul0);
		z[i_1] =
			(mul1 > MLIB_S8_MAX) ? MLIB_S8_MAX : (mul1 <
			MLIB_S8_MIN ? MLIB_S8_MIN : mul1);
	}

	if (n != n2) {
		mul0 = x[n - 1] * valc_s8;

		z[n - 1] =
			(mul0 > MLIB_S8_MAX) ? MLIB_S8_MAX : (mul0 <
			MLIB_S8_MIN ? MLIB_S8_MIN : mul0);
	}

	return (MLIB_SUCCESS);
}

#endif
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
	return __mlib_VectorMulS_U8_U8_Mod((mlib_u8 *)z, (mlib_u8 *)x,
		(mlib_u8 *)c, n);
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

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16_S8_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_s8 valc_s8 = *c;
	mlib_d64 mul1, mul0, valc = valc_s8;
	const mlib_d64 *table = mlib_SU82D64 + 128;
	mlib_s32 i_0, i_1, n2 = (n / 2) * 2, mul1_s32, *z_s32 = (mlib_s32 *)z;

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((mlib_addr)z & 3) {
		z[0] = x[0] * valc;
		z++;
		x++;
		n--;
		z_s32 = (mlib_s32 *)z;
	}

	n2 = (n / 2) * 2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1; i_0 < n2; i_0 += 2, i_1 += 2, z_s32++) {

		mul0 = table[x[i_0]];
		mul1 = table[x[i_1]];

		mul0 *= valc;
		mul1_s32 = mul1 * valc;

#ifndef _LITTLE_ENDIAN

		z_s32[0] = mul1_s32;

#else /* _LITTLE_ENDIAN */

		z[i_1] = mul1_s32;

#endif /* _LITTLE_ENDIAN */

		z[i_0] = mul0;
	}

	if (n % 2) {
		mlib_s32 mul = x[n - 1] * valc_s8;

		z[n - 1] = mul;
	}

	return (MLIB_SUCCESS);
}

#else
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16_S8_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_s8 valc_s8 = *c;
	mlib_s32 valc = valc_s8;
	mlib_s32 i_0, i_1, n2 = (n >> 1) << 1;

	if (n <= 0)
		return (MLIB_FAILURE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1; i_0 < n2; i_0 += 2, i_1 += 2) {
		z[i_0] = x[i_0] * valc;
		z[i_1] = x[i_1] * valc;
	}

	if (n != n2) {
		z[n - 1] = x[n - 1] * valc_s8;
	}

	return (MLIB_SUCCESS);
}

#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s16 valc_s16 = *c;
	mlib_d64 mul1, mul0, shift16 = 0x10000, valc = valc_s16 * shift16;
	mlib_s32 i_0, i_1, n2 = (n / 2) * 2, *z_s32 = (mlib_s32 *)z;

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((mlib_addr)z & 3) {
		mlib_s32 mul = x[0] * (mlib_s32)valc_s16;

		z[0] = (mul > MLIB_S16_MAX) ? MLIB_S16_MAX : (mul <
			MLIB_S16_MIN ? MLIB_S16_MIN : mul);
		z++;
		x++;
		n--;
		z_s32 = (mlib_s32 *)z;
	}

	n2 = (n / 2) * 2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1; i_0 < n2; i_0 += 2, i_1 += 2, z_s32++) {

		mul0 = valc * x[i_0];
		mul1 = valc * x[i_1];

#ifndef _LITTLE_ENDIAN
		z_s32[0] = FLOAT2INT_CLAMP(mul0);
#else /* _LITTLE_ENDIAN */
		z[i_0] = FLOAT2INT_CLAMP(mul0) >> 16;
#endif /* _LITTLE_ENDIAN */
		z[i_1] = FLOAT2INT_CLAMP(mul1) >> 16;
	}

	if (n % 2) {
		mlib_s32 mul = x[n - 1] * valc_s16;

		z[n - 1] =
			(mul > MLIB_S16_MAX) ? MLIB_S16_MAX : (mul <
			MLIB_S16_MIN ? MLIB_S16_MIN : mul);
	}

	return (MLIB_SUCCESS);
}
#else
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s16 valc_s16 = *c;
	mlib_s32 mul1, mul0, valc = valc_s16;
	mlib_s32 i_0, i_1, n2 = (n >> 1) << 1;

	if (n <= 0)
		return (MLIB_FAILURE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1; i_0 < n2; i_0 += 2, i_1 += 2) {

		mul0 = valc * x[i_0];
		mul1 = valc * x[i_1];

		z[i_0] =
			(mul0 > MLIB_S16_MAX) ? MLIB_S16_MAX : (mul0 <
			MLIB_S16_MIN ? MLIB_S16_MIN : mul0);
		z[i_1] =
			(mul1 > MLIB_S16_MAX) ? MLIB_S16_MAX : (mul1 <
			MLIB_S16_MIN ? MLIB_S16_MIN : mul1);
	}

	if (n != n2) {
		mul0 = x[n - 1] * valc;

		z[n - 1] =
			(mul0 > MLIB_S16_MAX) ? MLIB_S16_MAX : (mul0 <
			MLIB_S16_MIN ? MLIB_S16_MIN : mul0);
	}

	return (MLIB_SUCCESS);
}

#endif
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16_Sat(
	mlib_s16 *xz,
	const mlib_s16 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulS_S16_S16_Sat(xz, xz, c, n));
}
#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S32_S16_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s16 valc_s16 = *c;
	mlib_d64 mul1, mul0, valc = valc_s16;
	mlib_s32 i_0, i_1, n2 = (n / 2) * 2;

	if (n <= 0)
		return (MLIB_FAILURE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1; i_0 < n2; i_0 += 2, i_1 += 2) {

		mul0 = valc * x[i_0];
		mul1 = valc * x[i_1];

		z[i_0] = mul0;
		z[i_1] = mul1;
	}

	if (n % 2) {
		mlib_s32 mul = x[n - 1] * valc_s16;

		z[n - 1] = mul;
	}

	return (MLIB_SUCCESS);
}

#else
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S32_S16_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s16 valc_s16 = *c;
	mlib_s32 valc = valc_s16;
	mlib_s32 i_0, i_1, n2 = (n >> 1) << 1;

	if (n <= 0)
		return (MLIB_FAILURE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1; i_0 < n2; i_0 += 2, i_1 += 2) {
		z[i_0] = valc * x[i_0];
		z[i_1] = valc * x[i_1];
	}

	if (n != n2) {
		z[n - 1] = x[n - 1] * valc;
	}

	return (MLIB_SUCCESS);
}

#endif

#ifdef _NOLONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s16 valc_s16 = *c;
	mlib_d64 mul1, valc = valc_s16;
	mlib_s32 i_0, i_1, n2, *z_s32 = (mlib_s32 *)z, mul0;

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((mlib_addr)z & 3) {
		z[0] = x[0] * valc;
		z++;
		x++;
		n--;
		z_s32 = (mlib_s32 *)z;
	}

	n2 = (n / 2) * 2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1; i_0 < n2; i_0 += 2, i_1 += 2, z_s32++) {

		mul0 = valc * x[i_0];
		mul1 = valc * x[i_1];

#ifndef _LITTLE_ENDIAN

		z_s32[0] = mul1;

#else /* _LITTLE_ENDIAN */

		z[i_1] = mul1;

#endif /* _LITTLE_ENDIAN */

		z[i_0] = mul0;
	}

	if (n % 2) {
		mlib_s32 mul = x[n - 1] * valc_s16;

		z[n - 1] = mul;
	}

	return (MLIB_SUCCESS);
}

#else
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s16 valc_s16 = *c;
	mlib_s32 valc = valc_s16;
	mlib_s32 i_0, i_1, n2;

	if (n <= 0)
		return (MLIB_FAILURE);

	n2 = (n >> 1) << 1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1; i_0 < n2; i_0 += 2, i_1 += 2) {
		z[i_0] = valc * x[i_0];
		z[i_1] = valc * x[i_1];
	}

	if (n != n2) {
		z[n - 1] = x[n - 1] * valc;
	}

	return (MLIB_SUCCESS);
}
#endif
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16_Mod(
	mlib_s16 *xz,
	const mlib_s16 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulS_S16_S16_Mod(xz, xz, c, n));
}
#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S32_S32_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n)
{
#ifndef MLIB_USE_FTOI_CLAMPING

	MULS_SAT(mlib_s32,
		mlib_d64,
		MLIB_S32_MAX,
		MLIB_S32_MIN)
#else /* MLIB_USE_FTOI_CLAMPING */

	mlib_s32 i;
	mlib_d64 scal = *((mlib_s32 *)c);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++)
		((mlib_s32 *)z)[i] = ((mlib_s32 *)x)[i] * scal;
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* MLIB_USE_FTOI_CLAMPING */
}

#else
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S32_S32_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n)
{
#ifndef MLIB_USE_FTOI_CLAMPING

	MULS_SAT(mlib_s32,
		mlib_d64,
		MLIB_S32_MAX,
		MLIB_S32_MIN)
#else /* MLIB_USE_FTOI_CLAMPING */

	mlib_s32 i, i0, i1, n2;
	mlib_s64 scal = *c;
	mlib_s64 mul;
	n2 = (n >> 1) << 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i0 = 0, i1 = 1; i0 < n2; i0 += 2, i1 += 2) {
		mul = (mlib_s64) x[i0] * scal;
		z[i0] =
			(mul > MLIB_S32_MAX) ? MLIB_S32_MAX : (mul <
			MLIB_S32_MIN ? MLIB_S32_MIN : mul);
		mul = (mlib_s64) x[i1] * scal;
		z[i1] =
			(mul > MLIB_S32_MAX) ? MLIB_S32_MAX : (mul <
			MLIB_S32_MIN ? MLIB_S32_MIN : mul);
	}
	if (n2 != n) {
		mul = (mlib_s64) x[n-1] * scal;
		z[n-1] =
			(mul > MLIB_S32_MAX) ? MLIB_S32_MAX : (mul <
			MLIB_S32_MIN ? MLIB_S32_MIN : mul);
	}
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* MLIB_USE_FTOI_CLAMPING */
}

#endif
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S32_Sat(
	mlib_s32 *xz,
	const mlib_s32 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulS_S32_S32_Sat(xz, xz, c, n));
}

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S32_S32_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n)
{
#if (defined i386) || (defined _NO_LONGLONG)

	mlib_s32 i, valc = c[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		z[i] = x[i] * valc;
	}

#else /* (defined i386) || (defined _NO_LONGLONG) */

	mlib_s32 valc = *c, i_0, i_1, n2 = (n / 2) * 2;
	mlib_d64 valc_lo = (valc < 0) ? (mlib_s32)(valc | 0xFFFF0000) :
		(mlib_s32)(valc & 0xFFFF);
	mlib_d64 valc_hi = valc - valc_lo, valx_0, valx_1;
	mlib_s64 valz_0, valz_1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1; i_0 < n2; i_0 += 2, i_1 += 2) {

		valx_0 = x[i_0];
		valz_0 = valx_0 * valc_hi;
		z[i_0] = valz_0 + (mlib_s64)(valx_0 * valc_lo);

		valx_1 = x[i_1];
		valz_1 = valx_1 * valc_hi;
		z[i_1] = valz_1 + (mlib_s64)(valx_1 * valc_lo);
	}

	if (n % 2) {
		z[n - 1] = valc * x[n - 1];
	}
#endif /* (defined i386) || (defined _NO_LONGLONG) */

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}
#else
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S32_S32_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n)
{

	mlib_s32 i0, i1, valc = c[0];
	mlib_s32 n2 = (n >> 1) << 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i0 = 0, i1 = 1; i0 < n2; i0 += 2, i1 += 2) {
		z[i0] = (mlib_s64)x[i0] * valc;
		z[i1] = (mlib_s64)x[i1] * valc;
	}

	if (n != n2)
		z[n-1] = (mlib_s64)x[n-1] * valc;

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

#endif
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
	mlib_s32 n2 = (n >> 1) << 2, i_0, i_1, i_2, i_3, z_im, z_im_1;
	mlib_s32 c_re = c[0], c_im = c[1], x_re, x_im, x_re_1, x_im_1;
	mlib_s32 z_re, z_re_1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1, i_2 = 2, i_3 = 3; i_0 < n2;
		i_0 += 4, i_1 += 4, i_2 += 4, i_3 += 4) {

		x_re = x[i_0];
		x_im = x[i_1];

		z_re = x_re * c_re - x_im * c_im;
		z_im = x_re * c_im + x_im * c_re;
		z[i_0] = (((MLIB_U8_MAX - z_re) >> 31) | z_re) &
			(~(z_re >> 31));
		z[i_1] = ((MLIB_U8_MAX - z_im) >> 31) | z_im;

		x_im_1 = x[i_3];
		x_re_1 = x[i_2];

		z_re_1 = x_re_1 * c_re - x_im_1 * c_im;
		z_im_1 = (x_re_1 * c_im + x_im_1 * c_re);
		z[i_2] = (((MLIB_U8_MAX - z_re_1) >> 31) | z_re_1)
			& (~(z_re_1 >> 31));
		z[i_3] = ((MLIB_U8_MAX - z_im_1) >> 31) | z_im_1;
	}

	if ((n << 1) != n2) {
		mlib_s32 index1 = (n - 1) << 1;
		mlib_s32 index2 = index1 + 1;
		x_re = x[index1];
		x_im = x[index2];

		z_re = x_re * c_re - x_im * c_im;
		z_im = (x_re * c_im + x_im * c_re);
		z[index1] = (((MLIB_U8_MAX - z_re) >> 31) | z_re)
			& (~(z_re >> 31));
		z[index2] = ((MLIB_U8_MAX - z_im) >> 31) | z_im;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
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
__mlib_VectorMulS_S16C_U8C_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_s32 n2 = (n >> 1) << 2, i_0, i_1, i_2, i_3;
	mlib_s32 c_re = c[0], c_im = c[1];
	const mlib_d64 *table = mlib_SU82D64 + 128;
	mlib_s32 mul1, mul2;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1, i_2 = 2, i_3 = 3; i_0 < n2;
		i_0 += 4, i_1 += 4, i_2 += 4, i_3 += 4) {

		mul1 = x[i_0] * c_re - x[i_1] * c_im;
		mul2 = (x[i_0] * c_im + x[i_1] * c_re);
		if (mul1 > MLIB_S16_MAX)
			z[i_0] = MLIB_S16_MAX;
		else if (mul1 < MLIB_S16_MIN)
			z[i_0] = MLIB_S16_MIN;
		else
			z[i_0] = mul1;

		z[i_1] = ((((MLIB_S16_MAX - mul2) >>  31) | mul2) &
				MLIB_S16_MAX);

		mul1 = x[i_2] * c_re - x[i_3] * c_im;
		mul2 = (x[i_2] * c_im + x[i_3] * c_re);
		if (mul1 > MLIB_S16_MAX)
			z[i_2] = MLIB_S16_MAX;
		else if (mul1 < MLIB_S16_MIN)
			z[i_2] = MLIB_S16_MIN;
		else
			z[i_2] = mul1;

		z[i_3] = ((((MLIB_S16_MAX - mul2) >>  31) | mul2) &
				MLIB_S16_MAX);
	}

	if ((n << 1) != n2) {
		mlib_s32 index1 = (n-1) << 1;
		mlib_s32 index2 = index1 + 1;
		mul1 = x[index1] * c[0] - x[index2] * c[1];
		mul2 = x[index1] * c[1] + x[index2] * c[0];
		if (mul1 > MLIB_S16_MAX)
			z[index1] = MLIB_S16_MAX;
		else if (mul1 < MLIB_S16_MIN)
			z[index1] = MLIB_S16_MIN;
		else
			z[index1] = mul1;
		z[index2] = ((((MLIB_S16_MAX - mul2) >>  31) | mul2) &
				MLIB_S16_MAX);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_s32 n2 = (n >> 1) << 2, i_0, i_1, i_2, i_3;
	mlib_s32 x_re, x_im, x_re_1, x_im_1;
	mlib_s32 c_re = c[0], c_im = c[1];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1, i_2 = 2, i_3 = 3; i_0 < n2;
		i_0 += 4, i_1 += 4, i_2 += 4, i_3 += 4) {

		x_re = x[i_0];
		x_im = x[i_1];
		x_im_1 = x[i_3];
		x_re_1 = x[i_2];
		z[i_0] = x_re * c_re - x_im * c_im;
		z[i_1] = x_re * c_im + x_im * c_re;
		z[i_2] = x_re_1 * c_re - x_im_1 * c_im;
		z[i_3] = x_re_1 * c_im + x_im_1 * c_re;
	}

	if ((n << 1) != n2) {
		mlib_s32 index1 = (n - 1) << 1;
		mlib_s32 index2 = index1 + 1;
		mlib_s32 im, re;
		re = x[index1] * c_re - x[index2] * c_im;
		im = x[index1] * c_im + x[index2] * c_re;
		z[index1] = re;
		z[index2] = im;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16C_U8C_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_s32 n2 = (n >> 1) << 2, i_0, i_1, i_2, i_3;
	mlib_s32 c_re = c[0], c_im = c[1], x_re, x_im, x_re_1, x_im_1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1, i_2 = 2, i_3 = 3; i_0 < n2;
		i_0 += 4, i_1 += 4, i_2 += 4, i_3 += 4) {

		x_re = x[i_0];
		x_im = x[i_1];
		x_im_1 = x[i_3];
		x_re_1 = x[i_2];

		z[i_0] = x_re * c_re - x_im * c_im;
		z[i_1] = x_re * c_im + x_im * c_re;
		z[i_2] = x_re_1 * c_re - x_im_1 * c_im;
		z[i_3] = x_re_1 * c_im + x_im_1 * c_re;
	}

	if ((n << 1) != n2) {
		mlib_s32 im, re;
		mlib_s32 index1 = (n - 1) << 1;
		mlib_s32 index2 = index1 + 1;
		re = x[index1] * c_re - x[index2] * c_im;
		im = x[index1] * c_im + x[index2] * c_re;
		z[index1] = re;
		z[index2] = im;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
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

mlib_status
__mlib_VectorMulS_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_s32 n2 = (n >> 1) << 2, i_0, i_1, i_2, i_3;
	mlib_s32 c_re = c[0], c_im = c[1];
	mlib_s32 x_re, x_im, mul1, mul2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1, i_2 = 2, i_3 = 3; i_0 < n2;
		i_0 += 4, i_1 += 4, i_2 += 4, i_3 += 4) {

		x_re = x[i_0];
		x_im = x[i_1];

		mul1 = x_re * c_re - x_im * c_im;
		mul2 = x_re * c_im + x_im * c_re;
		if (mul1 > MLIB_S8_MAX)
			z[i_0] = MLIB_S8_MAX;
		else if (mul1 < MLIB_S8_MIN)
			z[i_0] = MLIB_S8_MIN;
		else
			z[i_0] = mul1;

		if (mul2 > MLIB_S8_MAX)
			z[i_1] = MLIB_S8_MAX;
		else if (mul2 < MLIB_S8_MIN)
			z[i_1] = MLIB_S8_MIN;
		else
			z[i_1] = mul2;

		x_im = x[i_3];
		x_re = x[i_2];

		mul1 = x_re * c_re - x_im * c_im;
		mul2 = x_re * c_im + x_im * c_re;
		if (mul1 > MLIB_S8_MAX)
			z[i_2] = MLIB_S8_MAX;
		else if (mul1 < MLIB_S8_MIN)
			z[i_2] = MLIB_S8_MIN;
		else
			z[i_2] = mul1;

		if (mul2 > MLIB_S8_MAX)
			z[i_3] = MLIB_S8_MAX;
		else if (mul2 < MLIB_S8_MIN)
			z[i_3] = MLIB_S8_MIN;
		else
			z[i_3] = mul2;
	}

	if ((n << 1) != n2) {
		mlib_s32 index1 = (n - 1) << 1;
		mlib_s32 index2 = index1 + 1;
		int mul2;
		mul1 = (mlib_s32)x[index1] * c[0] - (mlib_s32)x[index2] * c[1];
		mul2 = (mlib_s32)x[index1] * c[1] + (mlib_s32)x[index2] * c[0];
		if (mul1 > MLIB_S8_MAX)
			z[index1] = MLIB_S8_MAX;
		else if (mul1 < MLIB_S8_MIN)
			z[index1] = MLIB_S8_MIN;
		else
			z[index1] = mul1;

		if (mul2 > MLIB_S8_MAX)
			z[index2] = MLIB_S8_MAX;
		else if (mul2 < MLIB_S8_MIN)
			z[index2] = MLIB_S8_MIN;
		else
			z[index2] = mul2;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16C_S8C_Sat(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_s32 n2 = (n  >> 1) << 2, i_0, i_1, i_2, i_3;
	mlib_s8 c_re = c[0], c_im = c[1];
	mlib_s8 x_re, x_im;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1, i_2 = 2, i_3 = 3; i_0 < n2;
		i_0 += 4, i_1 += 4, i_2 += 4, i_3 += 4) {

		x_re = x[i_0];
		x_im = x[i_1];

		z[i_0] = (mlib_s32)x_re * c_re - (mlib_s32)x_im * c_im;
		if (((x_re == 0x80) && (c_re == 0x80) &&
					(x_im == 0x80) && (c_im == 0x80)))
			z[i_1] = 32768;
		else
			z[i_1] = (mlib_s32)x_re * c_im + (mlib_s32)x_im * c_re;

		x_im = x[i_3];
		x_re = x[i_2];

		z[i_2] = x_re * (mlib_s32)c_re - (mlib_s32)x_im * c_im;
		if (((x_re == 0x80) && (c_re == 0x80) &&
					(x_im == 0x80) && (c_im == 0x80)))
			z[i_3] = 32768;
		else
			z[i_3] = x_re * (mlib_s32)c_im + (mlib_s32)x_im * c_re;
	}

	if ((n << 1) != n2) {
		mlib_s32 index1 = (n - 1) << 1;
		mlib_s32 index2 = index1 + 1;
		x_re = x[index1];
		x_im = x[index2];
		z[index1] = x_re * (mlib_s32)c_re - (mlib_s32)x_im * c_im;
		if (((x_im == 0x80) && (x_re == 0x80) &&
				(c_re == 0x80) && (c_im == 0x80)))
			z[index2] = 32768;
		else
			z[index2] = (mlib_s32)x_re * c_im
				+ (mlib_s32)x_im * c_re;
	}
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
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
	return __mlib_VectorMulS_U8C_U8C_Mod((mlib_u8 *)z, (mlib_u8 *)x,
		(mlib_u8 *)c, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16C_S8C_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_s32 n2 = (n >> 1) << 2, i_0, i_1, i_2, i_3;
	mlib_s32 c_re = c[0], c_im = c[1], x_re, x_im, x_re_1, x_im_1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1, i_2 = 2, i_3 = 3; i_0 < n2;
		i_0 += 4, i_1 += 4, i_2 += 4, i_3 += 4) {

		x_re = x[i_0];
		x_im = x[i_1];
		x_re_1 = x[i_2];
		x_im_1 = x[i_3];

		z[i_0] = x_re * c_re - x_im * c_im;
		z[i_1] = x_re * c_im + x_im * c_re;
		z[i_2] = x_re_1 * c_re - x_im_1 * c_im;
		z[i_3] = x_re_1 * c_im + x_im_1 * c_re;
	}

	if ((n << 1) != n2) {
		mlib_s32 index1 = (n - 1) << 1;
		mlib_s32 index2 = index1 + 1;
		x_re = x[index1];
		x_im = x[index2];
		z[index1] = x_re * c_re - x_im * c_im;
		z[index2] = x_re * c_im + x_im * c_re;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
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

#ifndef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s32 n2 = (n >> 1) << 2, i_0, i_1, i_2, i_3;
	mlib_s64 c_re = c[0], c_im = c[1];
	mlib_s64 x_re, x_im;
	mlib_s64 mul1, mul2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1, i_2 = 2, i_3 = 3; i_0 < n2;
		i_0 += 4, i_1 += 4, i_2 += 4, i_3 += 4) {

		x_re = x[i_0];
		x_im = x[i_1];

		mul1 = x_re * c_re - x_im * c_im;
		mul2 = x_re * c_im + x_im * c_re;
		if (mul1 > MLIB_S16_MAX)
			z[i_0] = MLIB_S16_MAX;
		else if (mul1 < MLIB_S16_MIN)
			z[i_0] = MLIB_S16_MIN;
		else
			z[i_0] = mul1;

		if (mul2 > MLIB_S16_MAX)
			z[i_1] = MLIB_S16_MAX;
		else if (mul2 < MLIB_S16_MIN)
			z[i_1] = MLIB_S16_MIN;
		else
			z[i_1] = mul2;

		x_im = x[i_3];
		x_re = x[i_2];
		mul1 = x_re * c_re - x_im * c_im;
		mul2 = x_re * c_im + x_im * c_re;
		if (mul1 > MLIB_S16_MAX)
			z[i_2] = MLIB_S16_MAX;
		else if (mul1 < MLIB_S16_MIN)
			z[i_2] = MLIB_S16_MIN;
		else
			z[i_2] = mul1;

		if (mul2 > MLIB_S16_MAX)
			z[i_3] = MLIB_S16_MAX;
		else if (mul2 < MLIB_S16_MIN)
			z[i_3] = MLIB_S16_MIN;
		else
			z[i_3] = mul2;
	}

	if ((n << 1) != n2) {
		mlib_s32 index1 = (n - 1) << 1;
		mlib_s32 index2 = index1 + 1;

		x_im = x[index2];
		x_re = x[index1];
		mul1 = x_re * c_re - x_im * c_im;
		mul2 = x_re * c_im + x_im * c_re;
		if (mul1 > MLIB_S16_MAX)
			z[index1] = MLIB_S16_MAX;
		else if (mul1 < MLIB_S16_MIN)
			z[index1] = MLIB_S16_MIN;
		else
			z[index1] = mul1;

		if (mul2 > MLIB_S16_MAX)
			z[index2] = MLIB_S16_MAX;
		else if (mul2 < MLIB_S16_MIN)
			z[index2] = MLIB_S16_MIN;
		else
			z[index2] = mul2;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}
#else
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s32 n2 = (n / 2) * 4, i_0, i_1, i_2, i_3;
	mlib_d64 c_re = c[0] << 16, c_im = c[1] << 16;
	mlib_d64 x_re, x_im, x_re_1, x_im_1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1, i_2 = 2, i_3 = 3; i_0 < n2;
		i_0 += 4, i_1 += 4, i_2 += 4, i_3 += 4) {
		x_re = x[i_0];
		x_im = x[i_1];
		z[i_0] = FLOAT2INT_CLAMP(x_re * c_re - x_im * c_im)
		>>16;
		z[i_1] = FLOAT2INT_CLAMP(x_re * c_im + x_im * c_re)
		>>16;
		x_im_1 = x[i_3];
		x_re_1 = x[i_2];
		z[i_2] = FLOAT2INT_CLAMP(x_re_1 * c_re - x_im_1 * c_im)
		>>16;
		z[i_3] = FLOAT2INT_CLAMP(x_re_1 * c_im + x_im_1 * c_re)
		>>16;
	}
	if (n % 2) {
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

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

#endif
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S32C_S16C_Sat(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s32 i_0, i_1, i_2, i_3, n2 = (n >> 1) << 2;
	mlib_s32 mul;
	mlib_s16 x_re, x_im;
	mlib_s16 c0 = c[0], c1 = c[1];

	for (i_0 = 0, i_1 = 1, i_2 = 2, i_3 = 3; i_0 < n2;
			i_0 += 4, i_1 += 4, i_2 += 4, i_3 += 4) {
		x_re = x[i_0];
		x_im = x[i_1];
		z[i_0] = (mlib_s32)x_re * c0 - (mlib_s32)x_im * c1;
		if ((x_re == 0x8000) && (x_im == 0x8000) &&
				(c0 == 0x8000) && (c1 == 0x8000))
			z[i_1] = 2147483647;
		else
			z[i_1] = (mlib_s32)x_im * c0 + (mlib_s32)x_re * c1;

		x_re = x[i_2];
		x_im = x[i_3];
		z[i_2] = (mlib_s32)x_re * c0 - (mlib_s32)x_im * c1;
		if ((x_re == 0x8000) && (x_im == 0x8000) &&
				(c0 == 0x8000) && (c1 == 0x8000))
			z[i_3] = 2147483647;
		else
			z[i_3] = (mlib_s32)x_im * c0 + (mlib_s32)x_re * c1;
	}

	if ((n << 1) != n2) {
		mlib_s32 index1 = (n - 1) << 1;
		mlib_s32 index2 = index1 + 1;
		x_re = x[index1];
		x_im = x[index2];
		z[index1] = (mlib_s32)x_re * c0 - (mlib_s32)x_im * c1;
		if ((x_re == 0x8000) && (x_im == 0x8000) &&
				(c0 == 0x8000) && (c1 == 0x8000))
			z[index2] = 2147483647;
		else
			z[index2] = (mlib_s32)x_im * c0
				+ (mlib_s32)x_re * c1;

	}
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
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

#ifndef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s32 i, i_1, i_0, i_2, i_3, n2 = (n >> 1) << 2;
	const mlib_s16 *px = x;
	mlib_s16 *pz = z;
	mlib_s32 val_c_r = c[0], val_c_i = c[1];
	mlib_s64 x_im, x_re;

	for (i_0 = 0, i_1 = 1, i_2 = 2, i_3 = 3; i_0 < n2;
			i_0 += 4, i_1 += 4, i_2 += 4, i_3 += 4) {
		x_re = px[i_0];
		x_im = px[i_1];
		pz[i_0] = x_re * val_c_r - x_im * val_c_i;
		pz[i_1] = x_im * val_c_r + x_re * val_c_i;
		x_re = px[i_2];
		x_im = px[i_3];
		pz[i_2] = x_re * val_c_r - x_im * val_c_i;
		pz[i_3] = x_im * val_c_r + x_re * val_c_i;
	}

	if ((n << 1) != n2) {
		mlib_s64 mul1, mul2;
		i_0 = (n - 1) << 1;
		i_1 = i_0 + 1;
		x_re = px[i_0];
		x_im = px[i_1];
		mul1 = x_re * val_c_r - x_im * val_c_i;
		mul2 = x_im * val_c_r + x_re * val_c_i;
		pz[i_0] = mul1;
		pz[i_1] = mul2;
	}
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}
#else
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s32 i, i_1, i_0, n2 = n * 2;
	const mlib_s16 *px = x;
	mlib_s16 *pz = z;
	mlib_d64 val_c_r = c[0], val_c_i = c[1];
	mlib_d64 scal_r, scal_i, x_im, x_re;

	if (val_c_r == MLIB_S16_MIN && val_c_i == MLIB_S16_MIN)
		for (i = 0; i < n; i++) {
			scal_r = (px[2 * i] - (mlib_d64)px[2 * i +
				1]) * MLIB_S16_MIN;
			scal_i = (px[2 * i] + (mlib_d64)px[2 * i +
				1]) * MLIB_S16_MIN;

			if (scal_i > MLIB_S32_MAX)
				scal_i += 2.0 * MLIB_S32_MIN;
			pz[2 * i] = scal_r;
			pz[2 * i + 1] = scal_i;
		} else
			for (i_0 = 0, i_1 = 1; i_0 < n2; i_0 += 2, i_1 += 2) {
				x_re = px[i_0];
				x_im = px[i_1];
				scal_r = x_re * val_c_r - x_im * val_c_i;
				scal_i = x_im * val_c_r + x_re * val_c_i;
				pz[i_0] = scal_r;
				pz[i_1] = scal_i;
		}
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

#endif

#ifndef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s32 i, i_1, i_0, i_2, i_3, n2 = (n >> 1) << 2;
	const mlib_s16 *px = x;
	mlib_s32 *pz = z;
	mlib_s32 val_c_r = c[0], val_c_i = c[1];
	mlib_s64 x_im, x_re;

	for (i_0 = 0, i_1 = 1, i_2 = 2, i_3 = 3; i_0 < n2;
			i_0 += 4, i_1 += 4, i_2 += 4, i_3 += 4) {
		x_re = px[i_0];
		x_im = px[i_1];
		pz[i_0] = x_re * val_c_r - x_im * val_c_i;
		pz[i_1] = x_im * val_c_r + x_re * val_c_i;
		x_re = px[i_2];
		x_im = px[i_3];
		pz[i_2] = x_re * val_c_r - x_im * val_c_i;
		pz[i_3] = x_im * val_c_r + x_re * val_c_i;
	}

	if ((n << 1) != n2) {
		i_0 = (n - 1) << 1;
		i_1 = i_0 + 1;
		x_re = px[i_0];
		x_im = px[i_1];
		pz[i_0] = x_re * val_c_r - x_im * val_c_i;
		pz[i_1] = x_im * val_c_r + x_re * val_c_i;
	}
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}
#else
mlib_status
__mlib_VectorMulS_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_s16 *px = (mlib_s16 *)x;
	mlib_s32 *pz = (mlib_s32 *)z;
	mlib_d64 val_c_r = ((mlib_s16 *)c)[0];
	mlib_d64 val_c_i = ((mlib_s16 *)c)[1], scal_r, scal_i;
/* (a + jb) * (c + jd) = (ac - bd) + j(bc + ad) */
	if (val_c_r == MLIB_S16_MIN && val_c_i == MLIB_S16_MIN)
		for (i = 0; i < n; i++) {
			scal_r = (px[2 * i] - (mlib_d64)px[2 * i +
				1]) * MLIB_S16_MIN;
			scal_i = (px[2 * i] + (mlib_d64)px[2 * i +
				1]) * MLIB_S16_MIN;
			if (scal_i > MLIB_S32_MAX)
				scal_i += 2.0 * MLIB_S32_MIN;
			pz[2 * i] = scal_r;
			pz[2 * i + 1] = scal_i;
		} else
			for (i = 0; i < n; i++) {
				scal_r = px[2 * i] * val_c_r -
					px[2 * i + 1] * val_c_i;
				scal_i = px[2 * i + 1] * val_c_r
					+ px[2 * i] * val_c_i;
				pz[2 * i] = scal_r;
				pz[2 * i + 1] = scal_i;
		}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}
#endif
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S16C_Mod(
	mlib_s16 *xz,
	const mlib_s16 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulS_S16C_S16C_Mod(xz, xz, c, n));
}
#ifndef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S32C_S32C_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n)
{
	mlib_s32 i_0, i_1, i_2, i_3, n2 = (n >> 1) << 2;
	mlib_s64 mul1, mul2;
	mlib_s64 c0 = c[0], c1 = c[1];
	mlib_s32 x_re, x_im;
	for (i_0 = 0, i_1 = 1, i_2 = 2, i_3 = 3; i_0 < n2;
			i_0 += 4, i_1 += 4, i_2 += 4, i_3 += 4) {
		x_re = x[i_0];
		x_im = x[i_1];
		mul1 = x_re * c0 - x_im * c1;
		if (mul1 > MLIB_S32_MAX)
			z[i_0] = MLIB_S32_MAX;
		else if (mul1 < MLIB_S32_MIN)
			z[i_0] = MLIB_S32_MIN;
		else
			z[i_0] = mul1;
		if (((x_re == 0x80000000) && (c[0] == 0x80000000) &&
				(x_im == 0x80000000) && (c[1] == 0x80000000)))
			z[i_1] = MLIB_S32_MAX;
		else {
			mul2 = x_im * c0 + x_re * c1;
			if (mul2 > MLIB_S32_MAX)
				z[i_1] = MLIB_S32_MAX;
			else if (mul2 < MLIB_S32_MIN)
				z[i_1] = MLIB_S32_MIN;
			else
				z[i_1] = mul2;
		}

		x_re = x[i_2];
		x_im = x[i_3];
		mul1 = x_re * c0 - x_im * c1;
		if (mul1 > MLIB_S32_MAX)
			z[i_2] = MLIB_S32_MAX;
		else if (mul1 < MLIB_S32_MIN)
			z[i_2] = MLIB_S32_MIN;
		else
			z[i_2] = mul1;

		if (((x_re == 0x80000000) && (c[0] == 0x80000000) &&
				(x_im == 0x80000000) && (c[1] == 0x80000000)))
			z[i_3] = MLIB_S32_MAX;
		else {
			mul2 = x_im * c0 + x_re * c1;
			if (mul2 > MLIB_S32_MAX)
				z[i_3] = MLIB_S32_MAX;
			else if (mul2 < MLIB_S32_MIN)
				z[i_3] = MLIB_S32_MIN;
			else
				z[i_3] = mul2;
		}
	}

	if ((n << 1) != n2) {
		i_2 = (n - 1) << 1;
		i_3 = i_2 + 1;
		x_re = x[i_2];
		x_im = x[i_3];
		mul1 = x_re * c0 - x_im * c1;
		if (mul1 > MLIB_S32_MAX)
			z[i_2] = MLIB_S32_MAX;
		else if (mul1 < MLIB_S32_MIN)
			z[i_2] = MLIB_S32_MIN;
		else
			z[i_2] = mul1;

		if (((x_re == 0x80000000) && (c[0] == 0x80000000) &&
				(x_im == 0x80000000) && (c[1] == 0x80000000)))
			z[i_3] = MLIB_S32_MAX;
		else {
			mul2 = x_im * c0 + x_re * c1;
			if (mul2 > MLIB_S32_MAX)
				z[i_3] = MLIB_S32_MAX;
			else if (mul2 < MLIB_S32_MIN)
				z[i_3] = MLIB_S32_MIN;
			else
				z[i_3] = mul2;
		}
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}
#else
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
			pz[2 * i] = FLOAT2INT_CLAMP(mul_r);
			pz[2 * i + 1] = FLOAT2INT_CLAMP(mul_i);
		} else {
			mlib_d64 dc0_lo = c0 & MAX, dc1_lo = c1 & MAX;
			dc0 -= dc0_lo;
			dc1 -= dc1_lo;
			for (i = 0; i < n; i++) {
				mlib_d64 mul_r_lo =
					px[2 * i] * dc0_lo -
					px[2 * i + 1] * dc1_lo;
				mlib_d64 mul_r_hi =
					px[2 * i] * dc0 - px[2 * i + 1] * dc1;
				mlib_d64 mul_r = mul_r_hi + mul_r_lo;
				mlib_d64 mul_i_lo =
					px[2 * i + 1] * dc0_lo +
					px[2 * i] * dc1_lo;
				mlib_d64 mul_i_hi =
					px[2 * i + 1] * dc0 + px[2 * i] * dc1;
				mlib_d64 mul_i = mul_i_hi + mul_i_lo;
				pz[2 * i] = FLOAT2INT_CLAMP(mul_r);
				pz[2 * i + 1] = FLOAT2INT_CLAMP(mul_i);
		}
	}
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

#endif
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
	mlib_s32 c_re = c[0], c_im = c[1], n2 = (n >> 1) << 2;
	mlib_s32 i_0, i_1, i_2, i_3;
	mlib_s32 index1, index2;
	mlib_s32 mul1, mul2, x_re, x_im;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1, i_2 = 2, i_3 = 3; i_0 < n2;
			i_0 += 4, i_1 += 4, i_2 += 4, i_3 += 4) {
		x_re = x[i_0];
		x_im = x[i_1];
		z[i_0] = x_re * c_re - x_im * c_im;
		z[i_1] = x_im * c_re + x_re * c_im;
		x_re = x[i_2];
		x_im = x[i_3];
		z[i_2] = x_re * c_re - x_im * c_im;
		z[i_3] = x_im * c_re + x_re * c_im;
	}
	if ((n << 1) != n2) {
		index1 = (n - 1) << 1;
		index2 = index1 + 1;
		x_re = x[index1];
		x_im = x[index2];
		z[index1] = x_re * c_re - x_im * c_im;
		z[index2] = x_im * c_re + x_re * c_im;
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
