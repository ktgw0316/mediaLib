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

#pragma ident	"@(#)mlib_i_VectorMul.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *
 *      mlib_VectorMul_[U8|S8]_[U8|S8]_[Sat, Mod],
 *      mlib_VectorMul_[U8|S8]_S16_[Sat, Mod],
 *      mlib_VectorMul_[U8|S8]_[Sat, Mod]  - multiplication of
 *                                          signed/unsigned 8-bit format vectors
 *
 *      mlib_VectorMul_S16_S16_[Sat, Mod],
 *      mlib_VectorMul_S32_S16_Mod,
 *      mlib_VectorMul_S16_[Sat, Mod] - multiplication of
 *                                     signed 16-bit format vectors
 *
 *      mlib_VectorMul_S32_S32_[Sat, Mod],
 *      mlib_VectorMul_S32_[Sat, Mod] - multiplication of
 *                                     signed 32-bit format vectors
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMul_U8_U8_[Sat|Mod](mlib_u8             *z,
 *                                                 const const mlib_u8 *x,
 *                                                 const const mlib_u8 *y,
 *                                                 mlib_s32            n);
 *      mlib_status mlib_VectorMul_S8_S8_[Sat|Mod](mlib_s8             *z,
 *                                                 const const mlib_s8 *x,
 *                                                 const const mlib_s8 *y,
 *                                                 mlib_s32            n);
 *      mlib_status mlib_VectorMul_S16_U8_[Sat|Mod](mlib_s16            *z,
 *                                                  const const mlib_u8 *x,
 *                                                  const const mlib_u8 *y,
 *                                                  mlib_s32            n);
 *      mlib_status mlib_VectorMul_S16_S8_[Sat|Mod](mlib_s16            *z,
 *                                                  const const mlib_s8 *x,
 *                                                  const const mlib_s8 *y,
 *                                                  mlib_s32            n);
 *      mlib_status mlib_VectorMul_S16_S16_[Sat|Mod](mlib_s16             *z,
 *                                                   const const mlib_s16 *x,
 *                                                   const const mlib_s16 *y,
 *                                                   mlib_s32             n);
 *      mlib_status mlib_VectorMul_S32_S16_Mod(mlib_s32             *z,
 *                                             const const mlib_s16 *x,
 *                                             const const mlib_s16 *y,
 *                                             mlib_s32             n);
 *      mlib_status mlib_VectorMul_S32_S32_[Sat|Mod](mlib_s32             *z,
 *                                                   const const mlib_s32 *x,
 *                                                   const const mlib_s32 *y,
 *                                                   mlib_s32             n);
 *
 *      mlib_status mlib_VectorMul_U8_[Sat|Mod](mlib_u8             *xz,
 *                                              const const mlib_u8 *y,
 *                                              mlib_s32            n);
 *      mlib_status mlib_VectorMul_S8_[Sat|Mod](mlib_s8             *xz,
 *                                              const const mlib_s8 *y,
 *                                              mlib_s32            n);
 *      mlib_status mlib_VectorMul_S16_[Sat|Mod](mlib_s16             *xz,
 *                                               const const mlib_s16 *y,
 *                                               mlib_s32             n);
 *      mlib_status mlib_VectorMul_S32_[Sat|Mod](mlib_s32             *xz,
 *                                               const const mlib_s32 *y,
 *                                               mlib_s32             n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      x    pointer to the first element of the first vector
 *      y    pointer to the first element of the second vector
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *      z[i] = x[i] * y[i]
 *
 */

/*
 * FUNCTIONS
 *
 *      mlib_VectorMul_[U8C|S8C]_[U8C|S8C]_[Sat, Mod],
 *      mlib_VectorMul_[U8C|S8C]_[Sat, Mod]  - multiplication of
 *                                             8-bit format complex vectors
 *
 *      mlib_VectorMul_S16_S16_[Sat, Mod],
 *      mlib_VectorMul_S16_[Sat, Mod] - multiplication of
 *                                     16-bit format complex vectors
 *
 *      mlib_VectorMul_S32_S32_[Sat, Mod],
 *      mlib_VectorMul_S32_[Sat, Mod] - multiplication of
 *                                      32-bit format complex vectors
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMul_U8C_U8C_[Sat|Mod](mlib_u8             *z,
 *                                                   const const mlib_u8 *x,
 *                                                   const const mlib_u8 *y,
 *                                                   mlib_s32            n);
 *      mlib_status mlib_VectorMul_S8C_S8C_[Sat|Mod](mlib_s8             *z,
 *                                                   const const mlib_s8 *x,
 *                                                   const const mlib_s8 *y,
 *                                                   mlib_s32            n);
 *      mlib_status mlib_VectorMul_S16C_U8C_[Sat|Mod](mlib_s16            *z,
 *                                                    const const mlib_u8 *x,
 *                                                    const const mlib_u8 *y,
 *                                                    mlib_s32            n);
 *      mlib_status mlib_VectorMul_S16C_S8C_[Sat|Mod](mlib_s16            *z,
 *                                                    const const mlib_s8 *x,
 *                                                    const const mlib_s8 *y,
 *                                                    mlib_s32            n);
 *      mlib_status mlib_VectorMul_S16C_S16C_[Sat|Mod](mlib_s16             *z,
 *                                                     const const mlib_s16 *x,
 *                                                     const const mlib_s16 *y,
 *                                                     mlib_s32             n);
 *      mlib_status mlib_VectorMul_S32C_S16C_[Sat|Mod](mlib_s32             *z,
 *                                                     const const mlib_s16 *x,
 *                                                     const const mlib_s16 *y,
 *                                                     mlib_s32             n);
 *      mlib_status mlib_VectorMul_S32C_S32C_[Sat|Mod](mlib_s32             *z,
 *                                                     const const mlib_s32 *x,
 *                                                     const const mlib_s32 *y,
 *                                                     mlib_s32             n);
 *
 *      mlib_status mlib_VectorMul_U8C_[Sat|Mod](mlib_u8             *xz,
 *                                               const const mlib_u8 *y,
 *                                               mlib_s32            n);
 *      mlib_status mlib_VectorMul_S8C_[Sat|Mod](mlib_s8             *xz,
 *                                               const const mlib_s8 *y,
 *                                               mlib_s32            n);
 *      mlib_status mlib_VectorMul_S16C_[Sat|Mod](mlib_s16             *xz,
 *                                                const const mlib_s16 *y,
 *                                                mlib_s32             n);
 *      mlib_status mlib_VectorMul_S32C_[Sat|Mod](mlib_s32             *xz,
 *                                                const const mlib_s32 *y,
 *                                                mlib_s32             n);
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
 *
 */

#include <mlib_algebra.h>
#include <mlib_AlgebraUtil.h>

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
#pragma weak mlib_VectorMul_S32_S32_Sat = __mlib_VectorMul_S32_S32_Sat
#pragma weak mlib_VectorMul_S32C_S16C_Mod = __mlib_VectorMul_S32C_S16C_Mod
#pragma weak mlib_VectorMul_S16C_S16C_Mod = __mlib_VectorMul_S16C_S16C_Mod
#pragma weak mlib_VectorMul_S32C_Mod = __mlib_VectorMul_S32C_Mod
#pragma weak mlib_VectorMul_S16_S16_Sat = __mlib_VectorMul_S16_S16_Sat
#pragma weak mlib_VectorMul_S16C_Mod = __mlib_VectorMul_S16C_Mod
#pragma weak mlib_VectorMul_S32C_S32C_Sat = __mlib_VectorMul_S32C_S32C_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

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
__typeof__(__mlib_VectorMul_S32_S32_Sat) mlib_VectorMul_S32_S32_Sat
	__attribute__((weak, alias("__mlib_VectorMul_S32_S32_Sat")));
__typeof__(__mlib_VectorMul_S32C_S16C_Mod) mlib_VectorMul_S32C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMul_S32C_S16C_Mod")));
__typeof__(__mlib_VectorMul_S16C_S16C_Mod) mlib_VectorMul_S16C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMul_S16C_S16C_Mod")));
__typeof__(__mlib_VectorMul_S32C_Mod) mlib_VectorMul_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorMul_S32C_Mod")));
__typeof__(__mlib_VectorMul_S16_S16_Sat) mlib_VectorMul_S16_S16_Sat
	__attribute__((weak, alias("__mlib_VectorMul_S16_S16_Sat")));
__typeof__(__mlib_VectorMul_S16C_Mod) mlib_VectorMul_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMul_S16C_Mod")));
__typeof__(__mlib_VectorMul_S32C_S32C_Sat) mlib_VectorMul_S32C_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorMul_S32C_S32C_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MUL_SAT(form, MAX, MIN)                                 \
	{                                                       \
	    mlib_s32 i;                                         \
	    form mul;                                           \
	                                                        \
	    for (i = 0; i < n; i++) {                           \
		mul = px[i] * (form) py[i];                     \
		if (mul > MAX)                                  \
		    mul = MAX;                                  \
		else if (mul < MIN)                             \
		    mul = MIN;                                  \
		pz[i] = mul;                                    \
	    }                                                   \
	    return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);     \
	}

/* *********************************************************** */

#define	MUL_MOD                                                 \
	{                                                       \
	    mlib_s32 i;                                         \
	                                                        \
	    for (i = 0; i < n; i++)                             \
		pz[i] = px[i] * py[i];                          \
	    return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);     \
	}

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMul_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;
	mlib_s32 i;

#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_s32 mul;

	for (i = 0; i < n; i++) {
		mul = px[i] * (mlib_s32)py[i];

		if (mul > MLIB_U8_MAX)
			mul = MLIB_U8_MAX;
		pz[i] = mul;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_d64 d = 1 << 24, mul;
	const mlib_d64 *table = mlib_SU82D64 + 128;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mul = table[px[i]] * table[py[i]];
		pz[i] = (((mlib_s32)((mul - 128) * d)) >> 24) + 128;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* MLIB_USE_FTOI_CLAMPING */
}

#else		/* else define _NO_LONGLONG */
/* *********************************************************** */

mlib_status
__mlib_VectorMul_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;
	mlib_s32 i;

#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_s32 mul;

	for (i = 0; i < n; i++) {
		mul = px[i] * (mlib_s32)py[i];

		if (mul > MLIB_U8_MAX)
			mul = MLIB_U8_MAX;
		pz[i] = mul;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 mul;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mul = (mlib_s32)px[i] * py[i];
		pz[i] = ((mul | ((255 - mul) >> 31))
			& (~(mul >> 31)));
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* MLIB_USE_FTOI_CLAMPING */
}
#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMul_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MUL_MOD
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 i;
	mlib_d64 mul;
	const mlib_d64 *table = mlib_SU82D64 + 128;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mul = table[px[i]] * table[py[i]];
		pz[i] = (mlib_s32)mul;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

#else	/* else define _NO_LONGLONG */
/* *********************************************************** */

mlib_status
__mlib_VectorMul_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MUL_MOD
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 i;
	mlib_s32 mul;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mul = (mlib_s32) px[i] * py[i];
		pz[i] = mul;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}
#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16_U8_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MUL_MOD
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 i;
	mlib_d64 mul;
	const mlib_d64 *table = mlib_SU82D64 + 128;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mul = table[px[i]] * table[py[i]];
		pz[i] = (mlib_s32)mul;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}
#else
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16_U8_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MUL_MOD
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 i;
	mlib_s32 mul;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mul = (mlib_s32) px[i] * py[i];
		pz[i] = mul;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}
#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16_U8_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	mlib_s32 i;

#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_s32 mul;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mul = px[i] * (mlib_s32)py[i];

		if (mul > MLIB_S16_MAX)
			mul = MLIB_S16_MAX;
		pz[i] = mul;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_d64 d = 1 << 16, mul;
	const mlib_d64 *table = mlib_SU82D64 + 128;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mul = table[px[i]] * table[py[i]];
		pz[i] = (mlib_s32)(mul * d) >> 16;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* MLIB_USE_FTOI_CLAMPING */
}
#else
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16_U8_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	mlib_s32 i;

#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_s32 mul;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mul = px[i] * (mlib_s32)py[i];

		if (mul > MLIB_S16_MAX)
			mul = MLIB_S16_MAX;
		pz[i] = mul;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 mul;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mul = (mlib_s32) px[i] * py[i];
		if (mul > MLIB_S16_MAX)
			mul = MLIB_S16_MAX;
		pz[i] = mul;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* MLIB_USE_FTOI_CLAMPING */
}
#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y, *pz = (mlib_s8 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MUL_SAT(mlib_s32,
		MLIB_S8_MAX,
		    (-1 - MLIB_S8_MAX))
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 i;
	mlib_d64 d = 1 << 24, mul;
	const mlib_d64 *table;

	table = mlib_SU82D64 + 128;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mul = table[px[i]] * table[py[i]];
		pz[i] = ((mlib_s32)(mul * d)) >> 24;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* MLIB_USE_FTOI_CLAMPING */
}
#else
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y, *pz = (mlib_s8 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MUL_SAT(mlib_s32,
		MLIB_S8_MAX,
		    (-1 - MLIB_S8_MAX))
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 i;
	mlib_s32 mul;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mul = (mlib_s32) px[i] * py[i];
		if (mul > MLIB_S8_MAX)
			pz[i] = MLIB_S8_MAX;
		else if (mul < MLIB_S8_MIN)
			pz[i] = MLIB_S8_MIN;
		else
			pz[i] = mul;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* MLIB_USE_FTOI_CLAMPING */
}
#endif
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S8_S8_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return __mlib_VectorMul_U8_U8_Mod((mlib_u8 *)z, (mlib_u8 *)x,
		(mlib_u8 *)y, n);
}
#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16_S8_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MUL_MOD
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 i;
	mlib_d64 mul;
	const mlib_d64 *table = mlib_SU82D64 + 128;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mul = table[px[i]] * table[py[i]];
		pz[i] = (mlib_s32)mul;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

#else
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16_S8_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MUL_MOD
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 i;
	mlib_s32 mul;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mul = (mlib_s32) px[i] * py[i];
		pz[i] = mul;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}
#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_d64 d = 1 << 16, mul;
	mlib_s32 i;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MUL_SAT(mlib_s32,
		MLIB_S16_MAX,
		    (-1 - MLIB_S16_MAX))
#else /* MLIB_USE_FTOI_CLAMPING */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mul = px[i] * (mlib_d64)py[i];
		pz[i] = (mlib_s32)(mul * d) >> 16;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

#else
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_d64 d = 1 << 16, mul;
	mlib_s32 d1 = (mlib_s32)1 << 16;
	mlib_s32 mul1;
	mlib_s32 i;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MUL_SAT(mlib_s32,
		MLIB_S16_MAX,
		    (-1 - MLIB_S16_MAX))
#else /* MLIB_USE_FTOI_CLAMPING */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mul1 = (mlib_s32)px[i] * py[i];
		if (mul1 > MLIB_S16_MAX)
			pz[i] = MLIB_S16_MAX;
		else if (mul1 < MLIB_S16_MIN)
			pz[i] = MLIB_S16_MIN;
		else
			pz[i] = mul1;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 d = 1 << 16, _d = 1 / d, mul;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MUL_MOD
#else /* MLIB_USE_FTOI_CLAMPING */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mul = px[i] * (mlib_d64)py[i];
		pz[i] = mul - (mlib_s32)(mul * _d) * d;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

#else
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 d = 1 << 16, _d = 1 / d, mul;
	mlib_s64 mul1;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MUL_MOD
#else /* MLIB_USE_FTOI_CLAMPING */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mul1 = (mlib_s64)px[i] * py[i];
		pz[i] = mul1 - (mul1 & 0xffff0000);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}
#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S32_S16_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y;
	mlib_s32 *pz = (mlib_s32 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MUL_MOD
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 i;
	mlib_d64 mul;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mul = px[i] * (mlib_d64)py[i];
		pz[i] = (mlib_s32)mul;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

#else
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S32_S16_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y;
	mlib_s32 *pz = (mlib_s32 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MUL_MOD
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 i;
	mlib_s64 mul;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mul = (mlib_s64) px[i] * py[i];
		pz[i] = mul;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S32_S32_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	mlib_s32 *px = (mlib_s32 *)x, *py = (mlib_s32 *)y, *pz = (mlib_s32 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING

	MUL_SAT(mlib_d64,
		MLIB_S32_MAX,
		MLIB_S32_MIN)
#else /* MLIB_USE_FTOI_CLAMPING */

	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++)
		pz[i] = px[i] * (mlib_d64)py[i];
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* MLIB_USE_FTOI_CLAMPING */
}

#else
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S32_S32_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	mlib_s32 *px = (mlib_s32 *)x, *py = (mlib_s32 *)y, *pz = (mlib_s32 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING

	MUL_SAT(mlib_d64,
		MLIB_S32_MAX,
		MLIB_S32_MIN)
#else /* MLIB_USE_FTOI_CLAMPING */

	mlib_s32 i;

	mlib_s64 mul;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mul = (mlib_s64)px[i] * py[i];
		if (mul > MLIB_S32_MAX)
			pz[i] = MLIB_S32_MAX;
		else if (mul < MLIB_S32_MIN)
			pz[i] = MLIB_S32_MIN;
		else
			pz[i] = mul;
	}
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* MLIB_USE_FTOI_CLAMPING */
}

#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S32_S32_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	mlib_s32 *px = (mlib_s32 *)x, *py = (mlib_s32 *)y, *pz = (mlib_s32 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MUL_MOD
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 i;
	mlib_d64 d16 = 0x10000, _d16 = 1 / d16;
	mlib_d64 d15 = d16 * 0.5;
	mlib_d64 d31 = d16 * d15, _d31 = 1 / d31;
	mlib_d64 mul_hi, mul_lo;
	mlib_s32 sx, sy, one;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mul_hi = ((mlib_s16 *)(py + i))[0];
		mul_lo = ((mlib_u16 *)(py + i))[1];

		sy = py[i];
		sx = px[i];
		one = sy & sx & 1;

		mul_hi *= sx;
		mul_lo = (mul_lo * (mlib_d64)sx - one) * 0.5;

		mul_hi = mul_hi * d15 - (mlib_s32)(mul_hi * _d16) * d31 +
			mul_lo;
		mul_hi = mul_hi - (mlib_s32)(mul_hi * _d31) * d31;
		pz[i] = ((mlib_s32)mul_hi << 1) | one;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

#else
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S32_S32_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	mlib_s32 *px = (mlib_s32 *)x, *py = (mlib_s32 *)y, *pz = (mlib_s32 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MUL_MOD
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 i;
	mlib_s64 s64;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		s64 = (mlib_s64)px[i] * py[i];
		pz[i] = s64 - (s64 & 0xffffffff00000000);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

#endif
/* *********************************************************** */

#define	MULC_SAT(MAX, MIN)                                         \
	{                                                          \
	    mlib_s32 i;                                            \
	    mlib_s32 mul_r, mul_i, x_r, x_i, y_r, y_i;             \
	                                                           \
	    for (i = 0; i < n; i++) {                              \
		x_r = px[2 * i];                                   \
		x_i = px[2 * i + 1];                               \
		y_r = py[2 * i];                                   \
		y_i = py[2 * i + 1];                               \
		mul_r = x_r * y_r - x_i * y_i;                     \
		mul_i = x_i * y_r + x_r * y_i;                     \
		pz[2 * i] =                                        \
			(mul_r > MAX) ? MAX : (mul_r <             \
			MIN ? MIN : mul_r);                        \
		pz[2 * i + 1] = (mul_i > MAX) ||                   \
			(mul_i == MLIB_S32_MIN) ? MAX : (mul_i <   \
			MIN ? MIN : mul_i);                        \
	    }                                                      \
	    return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);        \
	}

/* *********************************************************** */

#define	MULC_MOD                                                \
	{                                                       \
	    mlib_s32 i;                                         \
	    mlib_s32 mul_r, mul_i, x_r, x_i, y_r, y_i;          \
	                                                        \
	    for (i = 0; i < n; i++) {                           \
		x_r = px[2 * i];                                \
		x_i = px[2 * i + 1];                            \
		y_r = py[2 * i];                                \
		y_i = py[2 * i + 1];                            \
		mul_r = x_r * y_r - x_i * y_i;                  \
		mul_i = x_i * y_r + x_r * y_i;                  \
		pz[2 * i] = mul_r;                              \
		pz[2 * i + 1] = mul_i;                          \
	    }                                                   \
	    return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);     \
	}

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMul_U8C_U8C_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;
	mlib_s32 i;

#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_s32 mul_r, mul_i, x_r, x_i, y_r, y_i;

	for (i = 0; i < n; i++) {
		x_r = px[2 * i];
		x_i = px[2 * i + 1];
		y_r = py[2 * i];
		y_i = py[2 * i + 1];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;
		pz[2 * i] =
			(mul_r > MLIB_U8_MAX) ? MLIB_U8_MAX : (mul_r <
			0 ? 0 : mul_r);
		pz[2 * i + 1] = (mul_i > MLIB_U8_MAX) ? MLIB_U8_MAX : mul_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_d64 mul_r, mul_i, x_r, x_i, y_r, y_i;
	mlib_d64 d = 1 << 24;
	const mlib_d64 *table = mlib_SU82D64 + 128;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		x_r = table[px[2 * i]];
		x_i = table[px[2 * i + 1]];
		y_r = table[py[2 * i]];
		y_i = table[py[2 * i + 1]];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;
		pz[2 * i] = (((mlib_s32)((mul_r - 128) * d)) >> 24) + 128;
		pz[2 * i + 1] = (((mlib_s32)((mul_i - 128) * d)) >> 24) + 128;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

#else
/* *********************************************************** */

mlib_status
__mlib_VectorMul_U8C_U8C_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;
	mlib_s32 i;

#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_s32 mul_r, mul_i, x_r, x_i, y_r, y_i;

	for (i = 0; i < n; i++) {
		x_r = px[2 * i];
		x_i = px[2 * i + 1];
		y_r = py[2 * i];
		y_i = py[2 * i + 1];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;
		pz[2 * i] =
			(mul_r > MLIB_U8_MAX) ? MLIB_U8_MAX : (mul_r <
			0 ? 0 : mul_r);
		pz[2 * i + 1] = (mul_i > MLIB_U8_MAX) ? MLIB_U8_MAX : mul_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 mul_r, mul_i;
	mlib_s32 x_r, x_i, y_r, y_i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	int index1, index2;
	for (i = 0; i < n; i++) {
		index1 = i << 1;
		index2 = index1 + 1;
		x_r = px[index1];
		x_i = px[index2];
		y_r = py[index1];
		y_i = py[index2];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;
		pz[index1] = ((mul_r | ((255 - mul_r) >> 31))
			& (~(mul_r >> 31)));
		pz[index2] = ((mul_i | ((255 - mul_i) >> 31))
			& (~(mul_i >> 31)));
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16C_U8C_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	mlib_s32 i;

#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_s32 mul_r, mul_i, x_r, x_i, y_r, y_i;

	for (i = 0; i < n; i++) {
		x_r = px[2 * i];
		x_i = px[2 * i + 1];
		y_r = py[2 * i];
		y_i = py[2 * i + 1];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;
		pz[2 * i] =
			(mul_r > MLIB_S16_MAX) ? MLIB_S16_MAX : (mul_r <
			MLIB_S16_MIN ? MLIB_S16_MIN : mul_r);
		pz[2 * i + 1] = (mul_i > MLIB_S16_MAX) ? MLIB_S16_MAX : mul_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_d64 mul_r, mul_i, x_r, x_i, y_r, y_i;
	mlib_d64 d = 1 << 16;
	const mlib_d64 *table = mlib_SU82D64 + 128;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		x_r = table[px[2 * i]];
		x_i = table[px[2 * i + 1]];
		y_r = table[py[2 * i]];
		y_i = table[py[2 * i + 1]];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;
		pz[2 * i] = (mlib_s32)(mul_r * d) >> 16;
		pz[2 * i + 1] = (mlib_s32)(mul_i * d) >> 16;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

#else
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16C_U8C_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	mlib_s32 i;

#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_s32 mul_r, mul_i, x_r, x_i, y_r, y_i;

	for (i = 0; i < n; i++) {
		x_r = px[2 * i];
		x_i = px[2 * i + 1];
		y_r = py[2 * i];
		y_i = py[2 * i + 1];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;
		pz[2 * i] =
			(mul_r > MLIB_S16_MAX) ? MLIB_S16_MAX : (mul_r <
			MLIB_S16_MIN ? MLIB_S16_MIN : mul_r);
		pz[2 * i + 1] = (mul_i > MLIB_S16_MAX) ? MLIB_S16_MAX : mul_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 mul_r, mul_i, x_r, x_i, y_r, y_i;
	mlib_s32 index1, index2;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		index1 = i << 1;
		index2 = index1 + 1;
		x_r = px[index1];
		x_i = px[index2];
		y_r = py[index1];
		y_i = py[index2];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;

		if (mul_r > MLIB_S16_MAX)
			pz[index1] = MLIB_S16_MAX;
		else if (mul_r < MLIB_S16_MIN)
			pz[index1] = MLIB_S16_MIN;
		else
			pz[index1] = mul_r;

		if (mul_i > MLIB_S16_MAX)
			pz[index2] = MLIB_S16_MAX;
		else if (mul_i < MLIB_S16_MIN)
			pz[index2] = MLIB_S16_MIN;
		else
			pz[index2] = mul_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}
#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMul_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MULC_MOD
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_d64 mul_r, mul_i, x_r, x_i, y_r, y_i;
	mlib_s32 i;
	const mlib_d64 *table = mlib_SU82D64 + 128;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		x_r = table[px[2 * i]];
		x_i = table[px[2 * i + 1]];
		y_r = table[py[2 * i]];
		y_i = table[py[2 * i + 1]];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;
		pz[2 * i] = (mlib_s32)mul_r;
		pz[2 * i + 1] = (mlib_s32)mul_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

#else
/* *********************************************************** */

mlib_status
__mlib_VectorMul_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MULC_MOD
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 mul_r, mul_i, x_r, x_i, y_r, y_i;
	mlib_s32 i, index1, index2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		index1 = i << 1;
		index2 = index1 + 1;
		x_r = px[index1];
		x_i = px[index2];
		y_r = py[index1];
		y_i = py[index2];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;
		pz[index1] = mul_r;
		pz[index2] = mul_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16C_U8C_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MULC_MOD
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_d64 mul_r, mul_i, x_r, x_i, y_r, y_i;
	mlib_s32 i;
	const mlib_d64 *table = mlib_SU82D64 + 128;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		x_r = table[px[2 * i]];
		x_i = table[px[2 * i + 1]];
		y_r = table[py[2 * i]];
		y_i = table[py[2 * i + 1]];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;
		pz[2 * i] = (mlib_s32)mul_r;
		pz[2 * i + 1] = (mlib_s32)mul_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

#else
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16C_U8C_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MULC_MOD
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 mul_r, mul_i, x_r, x_i, y_r, y_i;
	mlib_s32 i;
	mlib_s32 index1, index2;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		index1 = i << 1;
		index2 = index1 + 1;
		x_r = px[index1];
		x_i = px[index2];
		y_r = py[index1];
		y_i = py[index2];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;
		pz[index1] = mul_r;
		pz[index2] = mul_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y, *pz = (mlib_s8 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MULC_SAT(MLIB_S8_MAX, MLIB_S8_MIN)
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 i;
	mlib_d64 mul_r, mul_i, x_r, x_i, y_r, y_i;
	mlib_d64 d = 1 << 24;
	const mlib_d64 *table;

	table = mlib_SU82D64 + 128;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		x_r = table[px[2 * i]];
		x_i = table[px[2 * i + 1]];
		y_r = table[py[2 * i]];
		y_i = table[py[2 * i + 1]];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;
		pz[2 * i] = (mlib_s32)(mul_r * d) >> 24;
		pz[2 * i + 1] = (mlib_s32)(mul_i * d) >> 24;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* MLIB_USE_FTOI_CLAMPING */
}

#else
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y, *pz = (mlib_s8 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MULC_SAT(MLIB_S8_MAX, MLIB_S8_MIN)
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 i;
	mlib_s32 mul_r, mul_i, x_r, x_i, y_r, y_i;
	mlib_s32 index1, index2;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		index1 = i << 1;
		index2 = index1 + 1;
		x_r = px[index1];
		x_i = px[index2];
		y_r = py[index1];
		y_i = py[index2];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;
		if (mul_r > MLIB_S8_MAX)
			pz[index1] = MLIB_S8_MAX;
		else if (mul_r < MLIB_S8_MIN)
			pz[index1] = MLIB_S8_MIN;
		else
			pz[index1] = mul_r;
		if (mul_i > MLIB_S8_MAX)
			pz[index2] = MLIB_S8_MAX;
		else if (mul_i < MLIB_S8_MIN)
			pz[index2] = MLIB_S8_MIN;
		else
			pz[index2] = mul_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* MLIB_USE_FTOI_CLAMPING */
}

#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16C_S8C_Sat(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	mlib_s32 i;

#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_s32 mul_r, mul_i, x_r, x_i, y_r, y_i;

	for (i = 0; i < n; i++) {
		x_r = px[2 * i];
		x_i = px[2 * i + 1];
		y_r = py[2 * i];
		y_i = py[2 * i + 1];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;
		pz[2 * i] = mul_r;
		pz[2 * i + 1] = (mul_i > MLIB_S16_MAX) ? MLIB_S16_MAX : mul_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_d64 mul_r, mul_i, x_r, x_i, y_r, y_i;
	mlib_d64 d = 1 << 16;
	const mlib_d64 *table;

	table = mlib_SU82D64 + 128;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		x_r = table[px[2 * i]];
		x_i = table[px[2 * i + 1]];
		y_r = table[py[2 * i]];
		y_i = table[py[2 * i + 1]];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;
		pz[2 * i] = (mlib_s32)(mul_r * d) >> 16;
		pz[2 * i + 1] = (mlib_s32)(mul_i * d) >> 16;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* MLIB_USE_FTOI_CLAMPING */
}

#else
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16C_S8C_Sat(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	mlib_s32 i;

#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_s32 mul_r, mul_i, x_r, x_i, y_r, y_i;

	for (i = 0; i < n; i++) {
		x_r = px[2 * i];
		x_i = px[2 * i + 1];
		y_r = py[2 * i];
		y_i = py[2 * i + 1];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;
		pz[2 * i] = mul_r;
		pz[2 * i + 1] = (mul_i > MLIB_S16_MAX) ? MLIB_S16_MAX : mul_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 mul_r, mul_i, x_r, x_i, y_r, y_i;
	mlib_s32 index1, index2;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		index1 = i << 1;
		index2 = index1 + 1;
		x_r = px[index1];
		x_i = px[index2];
		y_r = py[index1];
		y_i = py[index2];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;
		pz[index1] = mul_r;
		pz[index2] = mul_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* MLIB_USE_FTOI_CLAMPING */
}
#endif
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S8C_S8C_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return __mlib_VectorMul_U8C_U8C_Mod((mlib_u8 *)z, (mlib_u8 *)x,
		(mlib_u8 *)y, n);
}

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16C_S8C_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MULC_MOD
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_d64 mul_r, mul_i, x_r, x_i, y_r, y_i;
	mlib_s32 i;
	const mlib_d64 *table = mlib_SU82D64 + 128;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		x_r = table[px[2 * i]];
		x_i = table[px[2 * i + 1]];
		y_r = table[py[2 * i]];
		y_i = table[py[2 * i + 1]];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;
		pz[2 * i] = (mlib_s32)mul_r;
		pz[2 * i + 1] = (mlib_s32)mul_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

#else
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16C_S8C_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MULC_MOD
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 mul_r, mul_i, x_r, x_i, y_r, y_i;
	mlib_s32 i, index1, index2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		index1 = i << 1;
		index2 = index1 + 1;
		x_r = px[index1];
		x_i = px[index2];
		y_r = py[index1];
		y_i = py[index2];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;
		pz[index1] = mul_r;
		pz[index2] = mul_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MULC_SAT(MLIB_S16_MAX, MLIB_S16_MIN)
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_d64 mul_r, mul_i, x_r, x_i, y_r, y_i, d = 1 << 16;
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		x_r = px[2 * i];
		x_i = px[2 * i + 1];
		y_r = py[2 * i];
		y_i = py[2 * i + 1];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;
		pz[2 * i] = (mlib_s32)(mul_r * d) >> 16;
		pz[2 * i + 1] = (mlib_s32)(mul_i * d) >> 16;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

#else
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MULC_SAT(MLIB_S16_MAX, MLIB_S16_MIN)
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s64 mul_r, mul_i;
	mlib_s64 x_r, x_i, y_r, y_i;
	mlib_s32 i, index1, index2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		index1 = i << 1;
		index2 = index1 + 1;
		x_r = px[index1];
		x_i = px[index2];
		y_r = py[index1];
		y_i = py[index2];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;

		if (mul_r > MLIB_S16_MAX)
			pz[index1] = MLIB_S16_MAX;
		else if (mul_r < MLIB_S16_MIN)
			pz[index1] = MLIB_S16_MIN;
		else
			pz[index1] = mul_r;

		if (mul_i > MLIB_S16_MAX)
			pz[index2] = MLIB_S16_MAX;
		else if (mul_i < MLIB_S16_MIN)
			pz[index2] = MLIB_S16_MIN;
		else
			pz[index2] = mul_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S32C_S16C_Sat(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y;
	mlib_s32 *pz = (mlib_s32 *)z;
	mlib_s32 i;

#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_s32 mul_r, mul_i, x_r, x_i, y_r, y_i;

	for (i = 0; i < n; i++) {
		x_r = px[2 * i];
		x_i = px[2 * i + 1];
		y_r = py[2 * i];
		y_i = py[2 * i + 1];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;
		pz[2 * i] = mul_r;
		pz[2 * i + 1] = (mul_i == MLIB_S32_MIN) ? MLIB_S32_MAX : mul_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_d64 mul_r, mul_i, x_r, x_i, y_r, y_i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		x_r = px[2 * i];
		x_i = px[2 * i + 1];
		y_r = py[2 * i];
		y_i = py[2 * i + 1];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;
		pz[2 * i] = (mlib_s32)mul_r;
		pz[2 * i + 1] = (mlib_s32)mul_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

#else
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S32C_S16C_Sat(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y;
	mlib_s32 *pz = (mlib_s32 *)z;
	mlib_s32 i;

#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_s32 mul_r, mul_i, x_r, x_i, y_r, y_i;

	for (i = 0; i < n; i++) {
		x_r = px[2 * i];
		x_i = px[2 * i + 1];
		y_r = py[2 * i];
		y_i = py[2 * i + 1];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;
		pz[2 * i] = mul_r;
		pz[2 * i + 1] = (mul_i == MLIB_S32_MIN) ? MLIB_S32_MAX : mul_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 mul_r, mul_i, x_r, x_i, y_r, y_i;
	mlib_s32 index1, index2;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		index1 = i << 1;
		index2 = index1 + 1;
		x_r = px[index1];
		x_i = px[index2];
		y_r = py[index1];
		y_i = py[index2];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;
		pz[index1] = mul_r;
		pz[index2] = mul_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}
#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y;
	mlib_s32 *pz = (mlib_s32 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MULC_MOD
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_d64 mul_r, mul_i;
	mlib_s32 x_r, x_i, y_r, y_i, z_r, z_i;
	mlib_s32 i;
	mlib_s32 sx_r, sx_i, sy_r, sy_i, sz_r, sz_i;
	mlib_s32 mask = 0x00000001;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		x_r = px[2 * i];
		x_i = px[2 * i + 1];
		y_r = py[2 * i];
		y_i = py[2 * i + 1];
		sx_r = x_r & mask;
		sx_i = x_i & mask;
		sy_r = y_r & mask;
		sy_i = y_i & mask;
		sz_r = (sx_r & sy_r) ^ (sx_i & sy_i);
		sz_i = (sx_r & sy_i) ^ (sx_i & sy_r);
		mul_r = x_r * (mlib_d64)y_r - x_i * (mlib_d64)y_i;
		mul_i = x_i * (mlib_d64)y_r + x_r * (mlib_d64)y_i;
		mul_r -= sz_r;
		mul_i -= sz_i;
		z_r = (mlib_s32)(mul_r * 0.5);
		z_i = (mlib_s32)(mul_i * 0.5);
		pz[2 * i] = (z_r << 1) | sz_r;
		pz[2 * i + 1] = (z_i << 1) | sz_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

#else
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y;
	mlib_s32 *pz = (mlib_s32 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MULC_MOD
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 x_r, x_i, y_r, y_i, z_r, z_i;
	mlib_s32 i, index1, index2;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		index1 = i << 1;
		index2 = index1 + 1;
		x_r = px[index1];
		x_i = px[index2];
		y_r = py[index1];
		y_i = py[index2];
		pz[index1] = x_r * y_r - x_i * y_i;
		pz[index2] = x_r * y_i + x_i * y_r;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MULC_MOD
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_d64 d16 = 0x10000, _d16 = 1 / d16;
	mlib_d64 mul_r, mul_i, x_r, x_i, y_r, y_i;
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		x_r = px[2 * i];
		x_i = px[2 * i + 1];
		y_r = py[2 * i];
		y_i = py[2 * i + 1];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;
		pz[2 * i] = mul_r - (mlib_s32)(mul_r * _d16) * d16;
		pz[2 * i + 1] = mul_i - (mlib_s32)(mul_i * _d16) * d16;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

#else
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MULC_MOD
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 mul_r, mul_i, x_r, x_i, y_r, y_i;
	mlib_s32 i;
	mlib_s32 index1, index2;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		index1 = i << 1;
		index2 = index1 + 1;
		x_r = px[index1];
		x_i = px[index2];
		y_r = py[index1];
		y_i = py[index2];
		mul_r = x_r * y_r - x_i * y_i;
		mul_i = x_i * y_r + x_r * y_i;
		pz[index1] = mul_r - (mul_r & 0xffff0000);
		pz[index2] = mul_i - (mul_i & 0xffff0000);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

#endif

#ifdef _NO_LONGLONG
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

	for (i = 0; i < n; i++) {
#define	MAX	0x100000
		mlib_d64 mul_r, mul_i;
		mlib_d64 dy0 = py[2 * i];
		mlib_d64 dy0_lo = py[2 * i] & (MAX - 1);
		mlib_d64 dy1 = py[2 * i + 1];
		mlib_d64 dy1_lo = py[2 * i + 1] & (MAX - 1);

		dy0 -= dy0_lo;
		dy1 -= dy1_lo;
		mul_r = px[2 * i] * dy0 - px[2 * i + 1] * dy1 +
			(px[2 * i] * dy0_lo - px[2 * i + 1] * dy1_lo);
		mul_i = px[2 * i + 1] * dy0 + px[2 * i] * dy1 +
			(px[2 * i + 1] * dy0_lo + px[2 * i] * dy1_lo);

#ifndef MLIB_USE_FTOI_CLAMPING

		if (mul_r > MLIB_S32_MAX)
			mul_r = MLIB_S32_MAX;
		else if (mul_r < MLIB_S32_MIN)
			mul_r = MLIB_S32_MIN;

		if (mul_i > MLIB_S32_MAX)
			mul_i = MLIB_S32_MAX;
		else if (mul_i < MLIB_S32_MIN)
			mul_i = MLIB_S32_MIN;

#endif /* MLIB_USE_FTOI_CLAMPING */

		pz[2 * i] = mul_r;
		pz[2 * i + 1] = mul_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

#else
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S32C_S32C_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	mlib_s32 *px = (mlib_s32 *)x, *py = (mlib_s32 *)y, *pz = (mlib_s32 *)z;
	mlib_s32 i, index1, index2;
	mlib_s64 mul_r, mul_i;

	for (i = 0; i < n; i++) {
		index1 = i << 1;
		index2 = index1 + 1;
		if (px[index1] == 0x80000000 &&
				px[index2] == 0x80000000 &&
				py[index1] == 0x80000000 &&
				py[index2] == 0x80000000) {
			pz[index1] = 0;
			pz[index2] = MLIB_S32_MAX;
		} else {
			mul_r = (mlib_s64)px[index1] * py[index1]
				- (mlib_s64)px[index2] * py[index2];
			mul_i = (mlib_s64)px[index1] * py[index2]
				+ (mlib_s64)px[index2] * py[index1];
		}
		if (mul_r > MLIB_S32_MAX)
			mul_r = MLIB_S32_MAX;
		else if (mul_r < MLIB_S32_MIN)
			mul_r = MLIB_S32_MIN;

		if (mul_i > MLIB_S32_MAX)
			mul_i = MLIB_S32_MAX;
		else if (mul_i < MLIB_S32_MIN)
			mul_i = MLIB_S32_MIN;

		pz[index1] = mul_r;
		pz[index2] = mul_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

#endif
#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S32C_S32C_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	mlib_s32 *px = (mlib_s32 *)x, *py = (mlib_s32 *)y, *pz = (mlib_s32 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MULC_MOD
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 i;
	mlib_d64 d16 = 0x10000, _d16 = 1 / d16;
	mlib_d64 d15 = d16 * 0.5;
	mlib_d64 d31 = d16 * d15, _d31 = 1 / d31;
	mlib_d64 yr_hi, yr_lo, yi_hi, yi_lo;
	mlib_d64 mulr_hi, mulr_lo, muli_hi, muli_lo;
	mlib_s32 xr, yr, oner;
	mlib_s32 xi, yi, onei;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
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
			(mlib_d64)px[2 * i + 0] * yr_hi - (mlib_d64)px[2 * i +
			1] * yi_hi;
		mulr_lo =
			((mlib_d64)px[2 * i + 0] * yr_lo - (mlib_d64)px[2 * i +
			1] * yi_lo - (mlib_d64)oner) * 0.5;

		muli_hi =
			(mlib_d64)px[2 * i + 0] * yi_hi + (mlib_d64)px[2 * i +
			1] * yr_hi;
		muli_lo =
			((mlib_d64)px[2 * i + 0] * yi_lo + (mlib_d64)px[2 * i +
			1] * yr_lo - (mlib_d64)onei) * 0.5;

		mulr_hi =
			mulr_hi * d15 - (mlib_s32)(mulr_hi * _d16) * d31 +
			mulr_lo;
		mulr_hi = mulr_hi - (mlib_s32)(mulr_hi * _d31) * d31;
		muli_hi =
			muli_hi * d15 - (mlib_s32)(muli_hi * _d16) * d31 +
			muli_lo;
		muli_hi = muli_hi - (mlib_s32)(muli_hi * _d31) * d31;

		pz[2 * i] = ((mlib_s32)mulr_hi << 1) | oner;
		pz[2 * i + 1] = ((mlib_s32)muli_hi << 1) | onei;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

#else
/* *********************************************************** */

mlib_status
__mlib_VectorMul_S32C_S32C_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	mlib_s32 *px = (mlib_s32 *)x, *py = (mlib_s32 *)y, *pz = (mlib_s32 *)z;

#ifndef MLIB_USE_FTOI_CLAMPING
	MULC_MOD
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 i, index1, index2;
	mlib_s64 mul_r, mul_i;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		index1 = i << 1;
		index2 = index1 + 1;
		mul_r = px[index1] * (mlib_s64)py[index1]
			- (mlib_s64)px[index2] * py[index2];
		mul_i = (mlib_s64)px[index1] * py[index2] +
			(mlib_s64)px[index2] * py[index1];
		pz[index1] = mul_r;
		pz[index2] = mul_i;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

#endif
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
__mlib_VectorMul_U8_Mod(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorMul_U8_U8_Mod(xz, xz, y, n));
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
__mlib_VectorMul_S8_Sat(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorMul_S8_S8_Sat(xz, xz, y, n));
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
__mlib_VectorMul_S16_Sat(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorMul_S16_S16_Sat(xz, xz, y, n));
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
__mlib_VectorMul_S32_Sat(
	mlib_s32 *xz,
	const mlib_s32 *y,
	mlib_s32 n)
{
	return (__mlib_VectorMul_S32_S32_Sat(xz, xz, y, n));
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
__mlib_VectorMul_U8C_Mod(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorMul_U8C_U8C_Mod(xz, xz, y, n));
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
__mlib_VectorMul_S8C_Mod(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorMul_S8C_S8C_Mod(xz, xz, y, n));
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
__mlib_VectorMul_S16C_Mod(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorMul_S16C_S16C_Mod(xz, xz, y, n));
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
__mlib_VectorMul_S32C_Mod(
	mlib_s32 *xz,
	const mlib_s32 *y,
	mlib_s32 n)
{
	return (__mlib_VectorMul_S32C_S32C_Mod(xz, xz, y, n));
}

/* *********************************************************** */
