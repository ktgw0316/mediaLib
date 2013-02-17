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

#pragma ident	"@(#)mlib_s_VectorMulS.c	9.3	07/11/05 SMI"

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

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

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

#define	MULS_sat(form_in, form, MAX, MIN)                       \
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

/* ****************************************************** */

#define	MULS_U8_SAT(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);	\
						\
	xlo = _mm_unpacklo_epi8(xbuf, zero);	\
	xhi = _mm_unpackhi_epi8(xbuf, zero);	\
	xlo = _mm_mullo_epi16(xlo, cbuf);	\
	xhi = _mm_mullo_epi16(xhi, cbuf);	\
	xlo = _mm_max_epi16(xlo, mask1);	\
	xhi = _mm_max_epi16(xhi, mask1);	\
	xlo = _mm_and_si128(xlo, mask2);	\
	xhi = _mm_and_si128(xhi, mask2);	\
	zbuf = _mm_packus_epi16(xlo, xhi);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	pz += nstep

mlib_status
__mlib_VectorMulS_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, c0, ax, az, nstep, n1, n2, n3;
	mlib_u8 *px = (mlib_u8 *)x, *pz = (mlib_u8 *)z;
	__m128i xbuf, zbuf, cbuf, xlo, xhi, zero, mask1, mask2;

	c0 = c[0];

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) * c0;
			SAT_U8(sum);
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		cbuf = _mm_set1_epi16(c0);
		mask1 = _mm_set1_epi16(-1);
		mask2 = _mm_set1_epi16(32767);

		for (i = 0; i < n1; i++) {
			sum = (*px++) * c0;
			SAT_U8(sum);
			*pz++ = sum;
		}

		if (ax == az) {
			for (i = 0; i < n2; i++) {
				MULS_U8_SAT(load, store);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MULS_U8_SAT(load, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) * c0;
			SAT_U8(sum);
			*pz++ = sum;
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

#define	MULS_U8_MOD(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);	\
						\
	xlo = _mm_unpacklo_epi8(xbuf, zero);	\
	xhi = _mm_unpackhi_epi8(xbuf, zero);	\
	xlo = _mm_mullo_epi16(xlo, cbuf);	\
	xhi = _mm_mullo_epi16(xhi, cbuf);	\
	xlo = _mm_and_si128(xlo, mask);		\
	xhi = _mm_and_si128(xhi, mask);		\
	zbuf = _mm_packus_epi16(xlo, xhi);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	pz += nstep

mlib_status
__mlib_VectorMulS_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, c0, ax, az, nstep, n1, n2, n3;
	mlib_u8 *px = (mlib_u8 *)x, *pz = (mlib_u8 *)z;
	__m128i xbuf, zbuf, cbuf, xlo, xhi, zero, mask;

	c0 = c[0];

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) * c0;
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		cbuf = _mm_set1_epi16(c0);
		mask = _mm_set1_epi16(0xff);

		for (i = 0; i < n1; i++) {
			sum = (*px++) * c0;
			*pz++ = sum;
		}

		if (ax == az) {
			for (i = 0; i < n2; i++) {
				MULS_U8_MOD(load, store);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MULS_U8_MOD(load, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) * c0;
			*pz++ = sum;
		}
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

#define	MULS_S16U8_SAT(ld, st)				\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	xlo = _mm_unpacklo_epi8(xbuf, zero);		\
	xhi = _mm_unpackhi_epi8(xbuf, zero);		\
	xlo = _mm_mullo_epi16(xlo, cbuf);		\
	xhi = _mm_mullo_epi16(xhi, cbuf);		\
	xlo = _mm_max_epi16(xlo, mask1);		\
	xhi = _mm_max_epi16(xhi, mask1);		\
	xlo = _mm_and_si128(xlo, mask2);		\
	xhi = _mm_and_si128(xhi, mask2);		\
	_mm_##st##_si128((__m128i *)pz, xlo);		\
	_mm_##st##_si128((__m128i *)pz + 1, xhi);	\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorMulS_S16_U8_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, c0, ax, az, nstep, n1, n2, n3;
	mlib_u8 *px = (mlib_u8 *)x;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, zbuf, cbuf, xlo, xhi, zero, mask1, mask2;

	c0 = c[0];

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) * c0;
			SAT_S16(sum);
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		cbuf = _mm_set1_epi16(c0);
		mask1 = _mm_set1_epi16(-1);
		mask2 = _mm_set1_epi16(32767);

		for (i = 0; i < n1; i++) {
			sum = (*px++) * c0;
			SAT_S16(sum);
			*pz++ = sum;
		}

		if ((ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				MULS_S16U8_SAT(load, store);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MULS_S16U8_SAT(load, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) * c0;
			SAT_S16(sum);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MULS_S16U8_MOD(ld, st)				\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	xlo = _mm_unpacklo_epi8(xbuf, zero);		\
	xhi = _mm_unpackhi_epi8(xbuf, zero);		\
	xlo = _mm_mullo_epi16(xlo, cbuf);		\
	xhi = _mm_mullo_epi16(xhi, cbuf);		\
	_mm_##st##_si128((__m128i *)pz, xlo);		\
	_mm_##st##_si128((__m128i *)pz + 1, xhi);	\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorMulS_S16_U8_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, c0, ax, az, nstep, n1, n2, n3;
	mlib_u8 *px = (mlib_u8 *)x;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, zbuf, cbuf, xlo, xhi, zero;

	c0 = c[0];

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) * c0;
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		cbuf = _mm_set1_epi16(c0);

		for (i = 0; i < n1; i++) {
			sum = (*px++) * c0;
			*pz++ = sum;
		}

		if ((ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				MULS_S16U8_MOD(load, store);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MULS_S16U8_MOD(load, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) * c0;
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MULS_S8_SAT(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);	\
						\
	xlo = _mm_unpacklo_epi8(zero, xbuf);	\
	xhi = _mm_unpackhi_epi8(zero, xbuf);	\
	xlo = _mm_mulhi_epi16(xlo, cbuf);	\
	xhi = _mm_mulhi_epi16(xhi, cbuf);	\
	zbuf = _mm_packs_epi16(xlo, xhi);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	pz += nstep

mlib_status
__mlib_VectorMulS_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, c0, ax, az, nstep, n1, n2, n3;
	mlib_s8 *px = (mlib_s8 *)x, *pz = (mlib_s8 *)z;
	__m128i xbuf, zbuf, cbuf, xlo, xhi, zero;

	c0 = c[0];

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) * c0;
			SAT_S8(sum);
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		cbuf = _mm_set1_epi16(c0 << 8);

		for (i = 0; i < n1; i++) {
			sum = (*px++) * c0;
			SAT_S8(sum);
			*pz++ = sum;
		}

		if (ax == az) {
			for (i = 0; i < n2; i++) {
				MULS_S8_SAT(load, store);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MULS_S8_SAT(load, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) * c0;
			SAT_S8(sum);
			*pz++ = sum;
		}
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

/* *********************************************************** */

#define	MULS_S16S8_MOD(ld, st)				\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	xlo = _mm_unpacklo_epi8(zero, xbuf);		\
	xhi = _mm_unpackhi_epi8(zero, xbuf);		\
	xlo = _mm_mulhi_epi16(xlo, cbuf);		\
	xhi = _mm_mulhi_epi16(xhi, cbuf);		\
	_mm_##st##_si128((__m128i *)pz, xlo);		\
	_mm_##st##_si128((__m128i *)pz + 1, xhi);	\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorMulS_S16_S8_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, c0, ax, az, nstep, n1, n2, n3;
	mlib_s8 *px = (mlib_s8 *)x;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, zbuf, cbuf, xlo, xhi, zero;

	c0 = c[0];

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) * c0;
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		cbuf = _mm_set1_epi16(c0 << 8);

		for (i = 0; i < n1; i++) {
			sum = (*px++) * c0;
			*pz++ = sum;
		}

		if ((ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				MULS_S16S8_MOD(load, store);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MULS_S16S8_MOD(load, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) * c0;
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MULS_S16_SAT(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);\
						\
	xlo = _mm_mullo_epi16(xbuf, cbuf);	\
	xhi = _mm_mulhi_epi16(xbuf, cbuf);	\
	zbuf = _mm_unpacklo_epi16(xlo, xhi);	\
	xlo  = _mm_unpackhi_epi16(xlo, xhi);	\
	zbuf = _mm_packs_epi32(zbuf, xlo);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	pz += nstep

mlib_status
__mlib_VectorMulS_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, c0, ax, az, nstep, n1, n2, n3;
	mlib_s16 *px = (mlib_s16 *)x, *pz = (mlib_s16 *)z;
	__m128i xbuf, zbuf, cbuf, xhi, xlo;

	c0 = c[0];
	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) * c0;
			SAT_S16(sum);
			*pz++ = sum;
		}
	} else {
		cbuf = _mm_set1_epi16(c0);

		for (i = 0; i < n1; i++) {
			sum = (*px++) * c0;
			SAT_S16(sum);
			*pz++ = sum;
		}

		if (ax == az) {
			for (i = 0; i < n2; i++) {
				MULS_S16_SAT(load, store);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MULS_S16_SAT(load, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) * c0;
			SAT_S16(sum);
			*pz++ = sum;
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

#define	MULS_S16_MOD(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);	\
						\
	zbuf = _mm_mullo_epi16(xbuf, cbuf);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	pz += nstep

mlib_status
__mlib_VectorMulS_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, c0, ax, az, nstep, n1, n2, n3;
	mlib_s16 *px = (mlib_s16 *)x, *pz = (mlib_s16 *)z;
	__m128i xbuf, zbuf, cbuf;

	c0 = c[0];
	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) * c0;
			*pz++ = sum;
		}
	} else {
		cbuf = _mm_set1_epi16(c0);

		for (i = 0; i < n1; i++) {
			sum = (*px++) * c0;
			*pz++ = sum;
		}

		if (ax == az) {
			for (i = 0; i < n2; i++) {
				MULS_S16_MOD(load, store);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MULS_S16_MOD(load, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) * c0;
			*pz++ = sum;
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

#define	MULS_S32S16_MOD(ld, st)				\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	xlo = _mm_mullo_epi16(xbuf, cbuf);		\
	xhi = _mm_mulhi_epi16(xbuf, cbuf);		\
	zbuf = _mm_unpacklo_epi16(xlo, xhi);		\
	xlo  = _mm_unpackhi_epi16(xlo, xhi);		\
	_mm_##st##_si128((__m128i *)pz, zbuf);		\
	_mm_##st##_si128((__m128i *)pz + 1, xlo);	\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorMulS_S32_S16_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, c0, ax, az, nstep, n1, n2, n3;
	mlib_s16 *px = (mlib_s16 *)x;
	mlib_s32 *pz = (mlib_s32 *)z;
	__m128i xbuf, zbuf, cbuf, xhi, xlo;

	c0 = c[0];
	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) * c0;
			*pz++ = sum;
		}
	} else {
		cbuf = _mm_set1_epi16(c0);

		for (i = 0; i < n1; i++) {
			sum = (*px++) * c0;
			*pz++ = sum;
		}

		if ((ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				MULS_S32S16_MOD(load, store);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MULS_S32S16_MOD(load, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) * c0;
			*pz++ = sum;
		}
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
#ifndef MLIB_USE_FTOI_CLAMPING

	MULS_sat(mlib_s32,
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

/* *********************************************************** */

mlib_status
__mlib_VectorMulS_S32_Mod(
	mlib_s32 *xz,
	const mlib_s32 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulS_S32_S32_Mod(xz, xz, c, n));
}

/* ****************************************************************** */

#define	MULS_MOD						\
	sum0 = (mlib_s32)px[0] * c0 - (mlib_s32)px[1] * c1;	\
	sum1 = (mlib_s32)px[0] * c1 + (mlib_s32)px[1] * c0;	\
	pz[0] = sum0;						\
	pz[1] = sum1;						\
	px += 2;						\
	pz += 2

#define	MULS_SAT(TYPE)						\
	sum0 = (mlib_s32)px[0] * c0 - (mlib_s32)px[1] * c1;	\
	sum1 = (mlib_s32)px[0] * c1 + (mlib_s32)px[1] * c0;	\
	SAT_##TYPE(sum0);					\
	SAT_##TYPE(sum1);					\
	pz[0] = sum0;						\
	pz[1] = sum1;						\
	px += 2;						\
	pz += 2

/* *********************************************************** */

#define	MULS_U8C_SAT_128(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	buf1 = _mm_unpacklo_epi8(xbuf, zero);		\
	buf2 = _mm_unpackhi_epi8(xbuf, zero);		\
							\
	buf3 = _mm_madd_epi16(buf1, cbuf0);		\
	buf4 = _mm_madd_epi16(buf2, cbuf0);		\
	buf1 = _mm_madd_epi16(buf1, cbuf1);		\
	buf2 = _mm_madd_epi16(buf2, cbuf1);		\
	rbuf = _mm_packs_epi32(buf3, buf4);		\
	ibuf = _mm_packs_epi32(buf1, buf2);		\
							\
	buf1 = _mm_unpacklo_epi16(rbuf, ibuf);		\
	buf2 = _mm_unpackhi_epi16(rbuf, ibuf);		\
	buf1 = _mm_packus_epi16(buf1, buf2);		\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
							\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorMulS_U8C_U8C_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, c0, c1;
	mlib_s32 ax, az, n1, n2, n3, nstep;
	mlib_u8 *px = (mlib_u8 *)x, *pz = (mlib_u8 *)z;
	__m128i xbuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i cbuf0, cbuf1, zero;

	c0 = c[0];
	c1 = c[1];
	cbuf0 = _mm_set1_epi32((c0 & 0xffff) | ((-c1) << 16));
	cbuf1 = _mm_set1_epi32((c0 << 16) | (c1 & 0xffff));
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_u8);

	if (ax & 1) {
		for (i = 0; i < n / 8; i++) {
			MULS_U8C_SAT_128(loadu, storeu);
		}
		for (i = 0; i < n % 8; i++) {
			MULS_SAT(U8);
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;
		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				MULS_SAT(U8);
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				MULS_SAT(U8);
			}
			if (ax == az) {
				for (i = 0; i < n2; i++) {
					MULS_U8C_SAT_128(load, store);
				}
			} else {
				for (i = 0; i < n2; i++) {
					MULS_U8C_SAT_128(load, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				MULS_SAT(U8);
			}
		}
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

#define	MULS_U8C_MOD_128(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	buf1 = _mm_unpacklo_epi8(xbuf, zero);		\
	buf2 = _mm_unpackhi_epi8(xbuf, zero);		\
							\
	/* real part */					\
	buf3 = _mm_madd_epi16(buf1, cbuf0);		\
	buf4 = _mm_madd_epi16(buf2, cbuf0);		\
	/* image part */				\
	buf1 = _mm_madd_epi16(buf1, cbuf1);		\
	buf2 = _mm_madd_epi16(buf2, cbuf1);		\
							\
	buf3 = _mm_and_si128(buf3, mask1);		\
	buf1 = _mm_slli_epi32(buf1, 8);			\
	buf4 = _mm_and_si128(buf4, mask1);		\
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
	_mm_##st##_si128((__m128i *)pz, buf3);		\
							\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorMulS_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, c0, c1;
	mlib_s32 ax, az, n1, n2, n3, nstep;
	mlib_u8 *px = (mlib_u8 *)x, *pz = (mlib_u8 *)z;
	__m128i xbuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i cbuf0, cbuf1, zero, mask1, mask2;

	c0 = c[0];
	c1 = c[1];
	cbuf0 = _mm_set1_epi32((c0 & 0xffff) | ((-c1) << 16));
	cbuf1 = _mm_set1_epi32((c0 << 16) | (c1 & 0xffff));
	zero = _mm_setzero_si128();
	mask1 = _mm_set1_epi16(0x00ff);
	mask2 = _mm_set1_epi32(0xffff);

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_u8);

	if (ax & 1) {
		for (i = 0; i < n / 8; i++) {
			MULS_U8C_MOD_128(loadu, storeu);
		}
		for (i = 0; i < n % 8; i++) {
			MULS_MOD;
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;
		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				MULS_MOD;
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				MULS_MOD;
			}
			if (ax == az) {
				for (i = 0; i < n2; i++) {
					MULS_U8C_MOD_128(load, store);
				}
			} else {
				for (i = 0; i < n2; i++) {
					MULS_U8C_MOD_128(load, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				MULS_MOD;
			}
		}
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

#define	MULS_S16CU8C_SAT_128(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	buf1 = _mm_unpacklo_epi8(xbuf, zero);		\
	buf2 = _mm_unpackhi_epi8(xbuf, zero);		\
							\
	buf3 = _mm_madd_epi16(buf1, cbuf0);		\
	buf4 = _mm_madd_epi16(buf2, cbuf0);		\
	buf1 = _mm_madd_epi16(buf1, cbuf1);		\
	buf2 = _mm_madd_epi16(buf2, cbuf1);		\
	rbuf = _mm_packs_epi32(buf3, buf4);		\
	ibuf = _mm_packs_epi32(buf1, buf2);		\
							\
	buf1 = _mm_unpacklo_epi16(rbuf, ibuf);		\
	buf2 = _mm_unpackhi_epi16(rbuf, ibuf);		\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
	_mm_##st##_si128((__m128i *)pz + 1, buf2);	\
							\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorMulS_S16C_U8C_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, c0, c1;
	mlib_s32 ax, az, n1, n2, n3, nstep;
	mlib_u8 *px = (mlib_u8 *)x;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i cbuf0, cbuf1, zero;

	c0 = c[0];
	c1 = c[1];
	cbuf0 = _mm_set1_epi32((c0 & 0xffff) | ((-c1) << 16));
	cbuf1 = _mm_set1_epi32((c0 << 16) | (c1 & 0xffff));
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_u8);

	if (ax & 1) {
		for (i = 0; i < n / 8; i++) {
			MULS_S16CU8C_SAT_128(loadu, storeu);
		}
		for (i = 0; i < n % 8; i++) {
			MULS_SAT(S16);
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;
		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				MULS_SAT(S16);
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				MULS_SAT(S16);
			}
			if ((ax * 2 & 15) == az) {
				for (i = 0; i < n2; i++) {
					MULS_S16CU8C_SAT_128(load, store);
				}
			} else {
				for (i = 0; i < n2; i++) {
					MULS_S16CU8C_SAT_128(load, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				MULS_SAT(S16);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MULS_S16CU8C_MOD_128(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	buf1 = _mm_unpacklo_epi8(xbuf, zero);		\
	buf2 = _mm_unpackhi_epi8(xbuf, zero);		\
							\
	buf3 = _mm_madd_epi16(buf1, cbuf0);		\
	buf4 = _mm_madd_epi16(buf2, cbuf0);		\
	buf1 = _mm_madd_epi16(buf1, cbuf1);		\
	buf2 = _mm_madd_epi16(buf2, cbuf1);		\
	buf3 = _mm_and_si128(buf3, mask);		\
	buf4 = _mm_and_si128(buf4, mask);		\
	buf1 = _mm_slli_epi32(buf1, 16);		\
	buf2 = _mm_slli_epi32(buf2, 16);		\
	buf1 = _mm_or_si128(buf1, buf3);		\
	buf2 = _mm_or_si128(buf2, buf4);		\
							\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
	_mm_##st##_si128((__m128i *)pz + 1, buf2);	\
							\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorMulS_S16C_U8C_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, c0, c1;
	mlib_s32 ax, az, n1, n2, n3, nstep;
	mlib_u8 *px = (mlib_u8 *)x;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i cbuf0, cbuf1, zero, mask;

	c0 = c[0];
	c1 = c[1];
	cbuf0 = _mm_set1_epi32((c0 & 0xffff) | ((-c1) << 16));
	cbuf1 = _mm_set1_epi32((c0 << 16) | (c1 & 0xffff));
	zero = _mm_setzero_si128();
	mask = _mm_set1_epi32(0xffff);

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_u8);

	if (ax & 1) {
		for (i = 0; i < n / 8; i++) {
			MULS_S16CU8C_MOD_128(loadu, storeu);
		}
		for (i = 0; i < n % 8; i++) {
			MULS_MOD;
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;
		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				MULS_MOD;
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				MULS_MOD;
			}
			if ((ax * 2 & 15) == az) {
				for (i = 0; i < n2; i++) {
					MULS_S16CU8C_MOD_128(load, store);
				}
			} else {
				for (i = 0; i < n2; i++) {
					MULS_S16CU8C_MOD_128(load, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				MULS_MOD;
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MULS_S8C_SAT_128(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	buf1 = _mm_unpacklo_epi8(zero, xbuf);		\
	buf2 = _mm_unpackhi_epi8(zero, xbuf);		\
	buf1 = _mm_srai_epi16(buf1, 8);			\
	buf2 = _mm_srai_epi16(buf2, 8);			\
							\
	buf3 = _mm_madd_epi16(buf1, cbuf0);		\
	buf4 = _mm_madd_epi16(buf2, cbuf0);		\
	buf1 = _mm_madd_epi16(buf1, cbuf1);		\
	buf2 = _mm_madd_epi16(buf2, cbuf1);		\
	rbuf = _mm_packs_epi32(buf3, buf4);		\
	ibuf = _mm_packs_epi32(buf1, buf2);		\
							\
	buf1 = _mm_unpacklo_epi16(rbuf, ibuf);		\
	buf2 = _mm_unpackhi_epi16(rbuf, ibuf);		\
	buf1 = _mm_packs_epi16(buf1, buf2);		\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
							\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorMulS_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, c0, c1;
	mlib_s32 ax, az, n1, n2, n3, nstep;
	mlib_s8 *px = (mlib_s8 *)x, *pz = (mlib_s8 *)z;
	__m128i xbuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i cbuf0, cbuf1, zero;

	c0 = c[0];
	c1 = c[1];
	cbuf0 = _mm_set1_epi32((c0 & 0xffff) | ((-c1) << 16));
	cbuf1 = _mm_set1_epi32((c0 << 16) | (c1 & 0xffff));
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_s8);

	if (ax & 1) {
		for (i = 0; i < n / 8; i++) {
			MULS_S8C_SAT_128(loadu, storeu);
		}
		for (i = 0; i < n % 8; i++) {
			MULS_SAT(S8);
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s8);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				MULS_SAT(S8);
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				MULS_SAT(S8);
			}
			if (ax == az) {
				for (i = 0; i < n2; i++) {
					MULS_S8C_SAT_128(load, store);
				}
			} else {
				for (i = 0; i < n2; i++) {
					MULS_S8C_SAT_128(load, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				MULS_SAT(S8);
			}
		}
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
	return __mlib_VectorMulS_U8C_U8C_Mod((mlib_u8 *)z, (mlib_u8 *)x,
		(mlib_u8 *)c, n);
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

#define	MULS_S16CS8C_SAT_128(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	buf1 = _mm_unpacklo_epi8(zero, xbuf);		\
	buf2 = _mm_unpackhi_epi8(zero, xbuf);		\
	buf1 = _mm_srai_epi16(buf1, 8);			\
	buf2 = _mm_srai_epi16(buf2, 8);			\
							\
	buf3 = _mm_madd_epi16(buf1, cbuf0);		\
	buf4 = _mm_madd_epi16(buf2, cbuf0);		\
	buf1 = _mm_madd_epi16(buf1, cbuf1);		\
	buf2 = _mm_madd_epi16(buf2, cbuf1);		\
	rbuf = _mm_packs_epi32(buf3, buf4);		\
	ibuf = _mm_packs_epi32(buf1, buf2);		\
							\
	buf1 = _mm_unpacklo_epi16(rbuf, ibuf);		\
	buf2 = _mm_unpackhi_epi16(rbuf, ibuf);		\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
	_mm_##st##_si128((__m128i *)pz + 1, buf2);	\
							\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorMulS_S16C_S8C_Sat(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, c0, c1;
	mlib_s32 ax, az, n1, n2, n3, nstep;
	mlib_s8 *px = (mlib_s8 *)x;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i cbuf0, cbuf1, zero;

	c0 = c[0];
	c1 = c[1];
	cbuf0 = _mm_set1_epi32((c0 & 0xffff) | ((-c1) << 16));
	cbuf1 = _mm_set1_epi32((c0 << 16) | (c1 & 0xffff));
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_s8);

	if (ax & 1) {
		for (i = 0; i < n / 8; i++) {
			MULS_S16CS8C_SAT_128(loadu, storeu);
		}
		for (i = 0; i < n % 8; i++) {
			MULS_SAT(S16);
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s8);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				MULS_SAT(S16);
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				MULS_SAT(S16);
			}
			if ((ax * 2 & 15) == az) {
				for (i = 0; i < n2; i++) {
					MULS_S16CS8C_SAT_128(load, store);
				}
			} else {
				for (i = 0; i < n2; i++) {
					MULS_S16CS8C_SAT_128(load, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				MULS_SAT(S16);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MULS_S16CS8C_MOD_128(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	buf1 = _mm_unpacklo_epi8(zero, xbuf);		\
	buf2 = _mm_unpackhi_epi8(zero, xbuf);		\
	buf1 = _mm_srai_epi16(buf1, 8);			\
	buf2 = _mm_srai_epi16(buf2, 8);			\
							\
	buf3 = _mm_madd_epi16(buf1, cbuf0);		\
	buf4 = _mm_madd_epi16(buf2, cbuf0);		\
	buf1 = _mm_madd_epi16(buf1, cbuf1);		\
	buf2 = _mm_madd_epi16(buf2, cbuf1);		\
	buf3 = _mm_and_si128(buf3, mask);		\
	buf4 = _mm_and_si128(buf4, mask);		\
	buf1 = _mm_slli_epi32(buf1, 16);		\
	buf2 = _mm_slli_epi32(buf2, 16);		\
	buf1 = _mm_or_si128(buf1, buf3);		\
	buf2 = _mm_or_si128(buf2, buf4);		\
							\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
	_mm_##st##_si128((__m128i *)pz + 1, buf2);	\
							\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorMulS_S16C_S8C_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, c0, c1;
	mlib_s32 ax, az, n1, n2, n3, nstep;
	mlib_s8 *px = (mlib_s8 *)x;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i cbuf0, cbuf1, zero, mask;

	c0 = c[0];
	c1 = c[1];
	cbuf0 = _mm_set1_epi32((c0 & 0xffff) | ((-c1) << 16));
	cbuf1 = _mm_set1_epi32((c0 << 16) | (c1 & 0xffff));
	zero = _mm_setzero_si128();
	mask = _mm_set1_epi32(0xffff);

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_s8);

	if (ax & 1) {
		for (i = 0; i < n / 8; i++) {
			MULS_S16CS8C_SAT_128(loadu, storeu);
		}
		for (i = 0; i < n % 8; i++) {
			MULS_MOD;
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s8);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				MULS_MOD;
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				MULS_MOD;
			}
			if ((ax * 2 & 15) == az) {
				for (i = 0; i < n2; i++) {
					MULS_S16CS8C_MOD_128(load, store);
				}
			} else {
				for (i = 0; i < n2; i++) {
					MULS_S16CS8C_MOD_128(load, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				MULS_MOD;
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MULS_S16C_SAT_128(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	rbuf = _mm_madd_epi16(xbuf, cbuf00);		\
	zbuf = _mm_madd_epi16(xbuf, cbuf01);		\
	ibuf = _mm_madd_epi16(xbuf, cbuf1);		\
	/* -S16_MIN*S16_MIN = S16_MIN*S16_MIN */	\
	rbuf = _mm_sub_epi32(rbuf, zbuf);		\
	/* S16_MIN*S16_MIN+S16_MIN*S16_MIN > S32_MAX */	\
	xbuf = _mm_cmpeq_epi32(ibuf, mask);		\
	ibuf = _mm_xor_si128(ibuf, xbuf);		\
							\
	rbuf = _mm_packs_epi32(rbuf, ibuf);		\
	ibuf = _mm_srli_si128(rbuf, 8);			\
	zbuf = _mm_unpacklo_epi16(rbuf, ibuf);		\
							\
	_mm_##st##_si128((__m128i *)pz, zbuf);		\
							\
	px += nstep;					\
	pz += nstep

#define	MULS_S16C_SAT						\
	sum0 = (mlib_s32)px[0] * c0 - (mlib_s32)px[1] * c1;	\
	sum1 = (mlib_s32)px[0] * c1 + (mlib_s32)px[1] * c0;	\
	if (sum1 == MLIB_S32_MIN)				\
		sum1 = MLIB_S32_MAX;				\
	SAT_S16(sum0);						\
	SAT_S16(sum1);						\
	pz[0] = sum0;						\
	pz[1] = sum1;						\
	px += 2;						\
	pz += 2

mlib_status
__mlib_VectorMulS_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, c0, c1;
	mlib_s32 ax, az, n1, n2, n3, nstep;
	mlib_s16 *px = (mlib_s16 *)x, *pz = (mlib_s16 *)z;
	__m128i xbuf, rbuf, ibuf, zbuf;
	__m128i cbuf00, cbuf01, cbuf1, zero, mask, delta;

	c0 = c[0];
	c1 = c[1];
	cbuf00 = _mm_set1_epi32(c0 & 0xffff);
	cbuf01 = _mm_set1_epi32(c1 << 16);
	cbuf1 = _mm_set1_epi32((c0 << 16) | (c1 & 0xffff));
	zero = _mm_setzero_si128();
	mask = _mm_set1_epi32(0x80000000);

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_s16);

	if (ax & 3) {
		for (i = 0; i < n / 4; i++) {
			MULS_S16C_SAT_128(loadu, storeu);
		}
		for (i = 0; i < n % 4; i++) {
			MULS_S16C_SAT;
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				MULS_S16C_SAT;
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				MULS_S16C_SAT;
			}
			if (ax == az) {
				for (i = 0; i < n2; i++) {
					MULS_S16C_SAT_128(load, store);
				}
			} else {
				for (i = 0; i < n2; i++) {
					MULS_S16C_SAT_128(load, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				MULS_S16C_SAT;
			}
		}
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

#define	MULS_S16C_MOD_128(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	rbuf = _mm_madd_epi16(xbuf, cbuf0);		\
	ibuf = _mm_madd_epi16(xbuf, cbuf1);		\
							\
	rbuf = _mm_and_si128(rbuf, mask);		\
	ibuf = _mm_slli_epi32(ibuf, 16);		\
	rbuf = _mm_or_si128(rbuf, ibuf);		\
							\
	_mm_##st##_si128((__m128i *)pz, rbuf);		\
							\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorMulS_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, c0, c1;
	mlib_s32 ax, az, n1, n2, n3, nstep;
	mlib_s16 *px = (mlib_s16 *)x, *pz = (mlib_s16 *)z;
	__m128i xbuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i cbuf0, cbuf1, zero, mask;

	c0 = c[0];
	c1 = c[1];
	cbuf0 = _mm_set1_epi32((c0 & 0xffff) | ((-c1) << 16));
	cbuf1 = _mm_set1_epi32((c0 << 16) | (c1 & 0xffff));
	zero = _mm_setzero_si128();
	mask = _mm_set1_epi32(0xffff);

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_s16);

	if (ax & 3) {
		for (i = 0; i < n / 4; i++) {
			MULS_S16C_MOD_128(loadu, storeu);
		}
		for (i = 0; i < n % 4; i++) {
			MULS_MOD;
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				MULS_MOD;
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				MULS_MOD;
			}

			if (ax == az) {
				for (i = 0; i < n2; i++) {
					MULS_S16C_MOD_128(load, store);
				}
			} else {
				for (i = 0; i < n2; i++) {
					MULS_S16C_MOD_128(load, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				MULS_MOD;
			}
		}
	}

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

#define	MULS_S32CS16C_SAT_128(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	rbuf = _mm_madd_epi16(xbuf, cbuf00);		\
	zbuf = _mm_madd_epi16(xbuf, cbuf01);		\
	ibuf = _mm_madd_epi16(xbuf, cbuf1);		\
	/* -S16_MIN*S16_MIN = S16_MIN*S16_MIN */	\
	rbuf = _mm_sub_epi32(rbuf, zbuf);		\
	/* S16_MIN*S16_MIN+S16_MIN*S16_MIN > S32_MAX */	\
	xbuf = _mm_cmpeq_epi32(ibuf, mask);		\
	ibuf = _mm_xor_si128(ibuf, xbuf);		\
							\
	zbuf = _mm_unpacklo_epi32(rbuf, ibuf);		\
	rbuf = _mm_unpackhi_epi32(rbuf, ibuf);		\
							\
	_mm_##st##_si128((__m128i *)pz, zbuf);		\
	_mm_##st##_si128((__m128i *)pz + 1, rbuf);	\
							\
	px += nstep;					\
	pz += nstep

#define	MULS_S32CS16C_SAT					\
	sum0 = (mlib_s32)px[0] * c0 - (mlib_s32)px[1] * c1;	\
	sum1 = (mlib_s32)px[0] * c1 + (mlib_s32)px[1] * c0;	\
	if (sum1 == MLIB_S32_MIN)				\
		sum1 = MLIB_S32_MAX;				\
	pz[0] = sum0;						\
	pz[1] = sum1;						\
	px += 2;						\
	pz += 2

mlib_status
__mlib_VectorMulS_S32C_S16C_Sat(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, c0, c1;
	mlib_s32 ax, az, n1, n2, n3, nstep;
	mlib_s16 *px = (mlib_s16 *)x;
	mlib_s32 *pz = (mlib_s32 *)z;
	__m128i xbuf, rbuf, ibuf, zbuf;
	__m128i cbuf00, cbuf01, cbuf1, zero, mask;

	c0 = c[0];
	c1 = c[1];
	cbuf00 = _mm_set1_epi32(c0 & 0xffff);
	cbuf01 = _mm_set1_epi32(c1 << 16);
	cbuf1 = _mm_set1_epi32((c0 << 16) | (c1 & 0xffff));
	zero = _mm_setzero_si128();
	mask = _mm_set1_epi32(0x80000000);

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_s16);

	if (ax & 3) {
		for (i = 0; i < n / 4; i++) {
			MULS_S32CS16C_SAT_128(loadu, storeu);
		}
		for (i = 0; i < n % 4; i++) {
			MULS_S32CS16C_SAT;
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				MULS_S32CS16C_SAT;
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				MULS_S32CS16C_SAT;
			}
			if ((ax * 2 & 15) == az) {
				for (i = 0; i < n2; i++) {
					MULS_S32CS16C_SAT_128(load, store);
				}
			} else {
				for (i = 0; i < n2; i++) {
					MULS_S32CS16C_SAT_128(load, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				MULS_S32CS16C_SAT;
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MULS_S32CS16C_MOD_128(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	rbuf = _mm_madd_epi16(xbuf, cbuf00);		\
	zbuf = _mm_madd_epi16(xbuf, cbuf01);		\
	ibuf = _mm_madd_epi16(xbuf, cbuf1);		\
	/* -S16_MIN*S16_MIN = S16_MIN*S16_MIN */	\
	rbuf = _mm_sub_epi32(rbuf, zbuf);		\
							\
	zbuf = _mm_unpacklo_epi32(rbuf, ibuf);		\
	rbuf = _mm_unpackhi_epi32(rbuf, ibuf);		\
							\
	_mm_##st##_si128((__m128i *)pz, zbuf);		\
	_mm_##st##_si128((__m128i *)pz + 1, rbuf);	\
							\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorMulS_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, c0, c1;
	mlib_s32 ax, az, n1, n2, n3, nstep;
	mlib_s16 *px = (mlib_s16 *)x;
	mlib_s32 *pz = (mlib_s32 *)z;
	__m128i xbuf, rbuf, ibuf, zbuf;
	__m128i cbuf00, cbuf01, cbuf1, zero, mask;

	c0 = c[0];
	c1 = c[1];
	cbuf00 = _mm_set1_epi32(c0 & 0xffff);
	cbuf01 = _mm_set1_epi32(c1 << 16);
	cbuf1 = _mm_set1_epi32((c0 << 16) | (c1 & 0xffff));
	zero = _mm_setzero_si128();
	mask = _mm_set1_epi32(0x80000000);

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_s16);

	if (ax & 3) {
		for (i = 0; i < n / 4; i++) {
			MULS_S32CS16C_MOD_128(loadu, storeu);
		}
		for (i = 0; i < n % 4; i++) {
			MULS_MOD;
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				MULS_MOD;
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				MULS_MOD;
			}
			if ((ax * 2 & 15) == az) {
				for (i = 0; i < n2; i++) {
					MULS_S32CS16C_MOD_128(load, store);
				}
			} else {
				for (i = 0; i < n2; i++) {
					MULS_S32CS16C_MOD_128(load, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				MULS_MOD;
			}
		}
	}

	return (MLIB_SUCCESS);
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

			pz[2 * i] = FLOAT2INT_CLAMP(mul_r);
			pz[2 * i + 1] = FLOAT2INT_CLAMP(mul_i);
	} else {
		mlib_d64 dc0_lo = c0 & MAX, dc1_lo = c1 & MAX;

		dc0 -= dc0_lo;
		dc1 -= dc1_lo;
		for (i = 0; i < n; i++) {
			mlib_d64 mul_r_lo =
				px[2 * i] * dc0_lo - px[2 * i + 1] * dc1_lo;
			mlib_d64 mul_r_hi =
				px[2 * i] * dc0 - px[2 * i + 1] * dc1;
			mlib_d64 mul_r = mul_r_hi + mul_r_lo;
			mlib_d64 mul_i_lo =
				px[2 * i + 1] * dc0_lo + px[2 * i] * dc1_lo;
			mlib_d64 mul_i_hi =
				px[2 * i + 1] * dc0 + px[2 * i] * dc1;
			mlib_d64 mul_i = mul_i_hi + mul_i_lo;

			pz[2 * i] = FLOAT2INT_CLAMP(mul_r);
			pz[2 * i + 1] = FLOAT2INT_CLAMP(mul_i);
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
#if (defined i386) || (defined _NO_LONGLONG)

	mlib_s32 c_re = c[0], c_im = c[1], scal_r, scal_i, i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		scal_r = x[2 * i] * c_re - x[2 * i + 1] * c_im;
		scal_i = x[2 * i + 1] * c_re + x[2 * i] * c_im;
		z[2 * i] = scal_r;
		z[2 * i + 1] = scal_i;
	}

#else /* (defined i386) || (defined _NO_LONGLONG) */

	mlib_s32 c_re = c[0], c_im = c[1], i_0, i_1, n2 = n * 2;
	mlib_d64 c_re_lo = (c_re < 0) ? (mlib_s32)(c_re | 0xFFFF0000) :
		(mlib_s32)(c_re & 0xFFFF);
	mlib_d64 c_re_hi = c_re - c_re_lo, x_re_0;
	mlib_d64 c_im_lo = (c_im < 0) ? (mlib_s32)(c_im | 0xFFFF0000) :
		(mlib_s32)(c_im & 0xFFFF);
	mlib_d64 c_im_hi = c_im - c_im_lo, x_im_0;
	mlib_s64 r0, r1, r2, r3;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1; i_0 < n2; i_0 += 2, i_1 += 2) {

		x_re_0 = x[i_0];
		x_im_0 = x[i_1];

		r0 = x_re_0 * c_re_hi - x_im_0 * c_im_hi;
		r1 = x_re_0 * c_re_lo - x_im_0 * c_im_lo;

		r2 = x_re_0 * c_im_hi + x_im_0 * c_re_hi;
		r3 = x_re_0 * c_im_lo + x_im_0 * c_re_lo;

		z[i_0] = r0 + r1;
		z[i_1] = r2 + r3;
	}

#endif /* (defined i386) || (defined _NO_LONGLONG) */

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
