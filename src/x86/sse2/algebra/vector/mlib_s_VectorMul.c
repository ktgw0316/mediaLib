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

#pragma ident	"@(#)mlib_s_VectorMul.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
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
 */

/*
 * FUNCTIONS
 *      mlib_VectorMul_[U8C|S8C]_[U8C|S8C]_[Sat, Mod],
 *      mlib_VectorMul_[U8C|S8C]_[Sat, Mod]  - multiplication of
 *                          unsigned/signed 8-bit format complex vectors
 *
 *      mlib_VectorMul_S16_S16_[Sat, Mod],
 *      mlib_VectorMul_S16_[Sat, Mod] - multiplication of
 *                           signed 16-bit format complex vectors
 *
 *      mlib_VectorMul_S32_S32_[Sat, Mod],
 *      mlib_VectorMul_S32_[Sat, Mod] - multiplication of
 *                           signed 32-bit format complex vectors
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

#define	MUL_U8_SAT(ld1, ld2, st)		\
	xbuf = _mm_##ld1##_si128((__m128i *)px);\
	ybuf = _mm_##ld2##_si128((__m128i *)py);\
						\
	xlo = _mm_unpacklo_epi8(xbuf, zero);	\
	xhi = _mm_unpackhi_epi8(xbuf, zero);	\
	ylo = _mm_unpacklo_epi8(ybuf, zero);	\
	yhi = _mm_unpackhi_epi8(ybuf, zero);	\
	xlo = _mm_mullo_epi16(xlo, ylo);	\
	xhi = _mm_mullo_epi16(xhi, yhi);	\
	xlo = _mm_max_epi16(xlo, mask1);	\
	xhi = _mm_max_epi16(xhi, mask1);	\
	xlo = _mm_and_si128(xlo, mask2);	\
	xhi = _mm_and_si128(xhi, mask2);	\
	zbuf = _mm_packus_epi16(xlo, xhi);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	py += nstep;				\
	pz += nstep

mlib_status
__mlib_VectorMul_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, ax, ay, az, nstep, n1, n2, n3;
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;
	__m128i xbuf, ybuf, zbuf, xhi, xlo, yhi, ylo, zero, mask1, mask2;

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			SAT_U8(sum);
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		mask1 = _mm_set1_epi16(-1);
		mask2 = _mm_set1_epi16(32767);

		for (i = 0; i < n1; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			SAT_U8(sum);
			*pz++ = sum;
		}

		if (ax == ay & ax == az) {
			for (i = 0; i < n2; i++) {
				MUL_U8_SAT(load, load, store);
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				MUL_U8_SAT(load, load, storeu);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MUL_U8_SAT(load, loadu, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			SAT_U8(sum);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_U8_MOD(ld1, ld2, st)		\
	xbuf = _mm_##ld1##_si128((__m128i *)px);\
	ybuf = _mm_##ld2##_si128((__m128i *)py);\
						\
	xlo = _mm_unpacklo_epi8(xbuf, zero);	\
	xhi = _mm_unpackhi_epi8(xbuf, zero);	\
	ylo = _mm_unpacklo_epi8(ybuf, zero);	\
	yhi = _mm_unpackhi_epi8(ybuf, zero);	\
	xlo = _mm_mullo_epi16(xlo, ylo);	\
	xhi = _mm_mullo_epi16(xhi, yhi);	\
	xlo = _mm_and_si128(xlo, mask);		\
	xhi = _mm_and_si128(xhi, mask);		\
	zbuf = _mm_packus_epi16(xlo, xhi);	\
						\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	py += nstep;				\
	pz += nstep

mlib_status
__mlib_VectorMul_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, ax, ay, az, nstep, n1, n2, n3;
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;
	__m128i xbuf, ybuf, zbuf, xhi, xlo, yhi, ylo, zero, mask;

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		mask = _mm_set1_epi16(0xff);

		for (i = 0; i < n1; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			*pz++ = sum;
		}

		if (ax == ay & ax == az) {
			for (i = 0; i < n2; i++) {
				MUL_U8_MOD(load, load, store);
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				MUL_U8_MOD(load, load, storeu);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MUL_U8_MOD(load, loadu, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S16U8_MOD(ld1, ld2, st)			\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
							\
	xlo = _mm_unpacklo_epi8(xbuf, zero);		\
	xhi = _mm_unpackhi_epi8(xbuf, zero);		\
	ylo = _mm_unpacklo_epi8(ybuf, zero);		\
	yhi = _mm_unpackhi_epi8(ybuf, zero);		\
	xlo = _mm_mullo_epi16(xlo, ylo);		\
	xhi = _mm_mullo_epi16(xhi, yhi);		\
							\
	_mm_##st##_si128((__m128i *)pz, xlo);		\
	_mm_##st##_si128((__m128i *)pz + 1, xhi);	\
	px += nstep;					\
	py += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorMul_S16_U8_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, ax, ay, az, nstep, n1, n2, n3;
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf, xhi, xlo, yhi, ylo, zero;

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();

		for (i = 0; i < n1; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			*pz++ = sum;
		}

		if (ax == ay && (ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				MUL_S16U8_MOD(load, load, store);
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				MUL_S16U8_MOD(load, load, storeu);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MUL_S16U8_MOD(load, loadu, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S16U8_SAT(ld1, ld2, st)			\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
							\
	xlo = _mm_unpacklo_epi8(xbuf, zero);		\
	xhi = _mm_unpackhi_epi8(xbuf, zero);		\
	ylo = _mm_unpacklo_epi8(ybuf, zero);		\
	yhi = _mm_unpackhi_epi8(ybuf, zero);		\
	xlo = _mm_mullo_epi16(xlo, ylo);		\
	xhi = _mm_mullo_epi16(xhi, yhi);		\
	xlo = _mm_max_epi16(xlo, mask1);		\
	xhi = _mm_max_epi16(xhi, mask1);		\
	xlo = _mm_and_si128(xlo, mask2);		\
	xhi = _mm_and_si128(xhi, mask2);		\
							\
	_mm_##st##_si128((__m128i *)pz, xlo);		\
	_mm_##st##_si128((__m128i *)pz + 1, xhi);	\
	px += nstep;					\
	py += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorMul_S16_U8_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, ax, ay, az, nstep, n1, n2, n3;
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf, xhi, xlo, yhi, ylo, zero, mask1, mask2;

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			SAT_S16(sum);
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		mask1 = _mm_set1_epi16(-1);
		mask2 = _mm_set1_epi16(32767);

		for (i = 0; i < n1; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			SAT_S16(sum);
			*pz++ = sum;
		}

		if (ax == ay && (ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				MUL_S16U8_SAT(load, load, store);
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				MUL_S16U8_SAT(load, load, storeu);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MUL_S16U8_SAT(load, loadu, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			SAT_S16(sum);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S8_SAT(ld1, ld2, st)		\
	xbuf = _mm_##ld1##_si128((__m128i *)px);\
	ybuf = _mm_##ld2##_si128((__m128i *)py);\
						\
	xlo = _mm_unpacklo_epi8(zero, xbuf);	\
	xhi = _mm_unpackhi_epi8(zero, xbuf);	\
	ylo = _mm_unpacklo_epi8(zero, ybuf);	\
	yhi = _mm_unpackhi_epi8(zero, ybuf);	\
	xlo = _mm_mulhi_epi16(xlo, ylo);	\
	xhi = _mm_mulhi_epi16(xhi, yhi);	\
	zbuf = _mm_packs_epi16(xlo, xhi);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	py += nstep;				\
	pz += nstep

mlib_status
__mlib_VectorMul_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, ax, ay, az, nstep, n1, n2, n3;
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y, *pz = (mlib_s8 *)z;
	__m128i xbuf, ybuf, zbuf, xhi, xlo, yhi, ylo, zero;

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			SAT_S8(sum);
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();

		for (i = 0; i < n1; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			SAT_S8(sum);
			*pz++ = sum;
		}

		if (ax == ay & ax == az) {
			for (i = 0; i < n2; i++) {
				MUL_S8_SAT(load, load, store);
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				MUL_S8_SAT(load, load, storeu);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MUL_S8_SAT(load, loadu, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			SAT_S8(sum);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

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

/* *********************************************************** */

#define	MUL_S16S8_MOD(ld1, ld2, st)			\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
							\
	xlo = _mm_unpacklo_epi8(zero, xbuf);		\
	xhi = _mm_unpackhi_epi8(zero, xbuf);		\
	ylo = _mm_unpacklo_epi8(zero, ybuf);		\
	yhi = _mm_unpackhi_epi8(zero, ybuf);		\
	xlo = _mm_mulhi_epi16(xlo, ylo);		\
	xhi = _mm_mulhi_epi16(xhi, yhi);		\
							\
	_mm_##st##_si128((__m128i *)pz, xlo);		\
	_mm_##st##_si128((__m128i *)pz + 1, xhi);	\
	px += nstep;					\
	py += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorMul_S16_S8_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, ax, ay, az, nstep, n1, n2, n3;
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf, xhi, xlo, yhi, ylo, zero;

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();

		for (i = 0; i < n1; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			*pz++ = sum;
		}

		if (ax == ay && (ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				MUL_S16S8_MOD(load, load, store);
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				MUL_S16S8_MOD(load, load, storeu);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MUL_S16S8_MOD(load, loadu, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S16_SAT(ld1, ld2, st)		\
	xbuf = _mm_##ld1##_si128((__m128i *)px);\
	ybuf = _mm_##ld2##_si128((__m128i *)py);\
						\
	xlo = _mm_mullo_epi16(xbuf, ybuf);	\
	xhi = _mm_mulhi_epi16(xbuf, ybuf);	\
	ylo = _mm_unpacklo_epi16(xlo, xhi);	\
	yhi = _mm_unpackhi_epi16(xlo, xhi);	\
	zbuf = _mm_packs_epi32(ylo, yhi);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	py += nstep;				\
	pz += nstep

mlib_status
__mlib_VectorMul_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, ax, ay, az, nstep, n1, n2, n3;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf, xhi, xlo, yhi, ylo;

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			SAT_S16(sum);
			*pz++ = sum;
		}
	} else {
		for (i = 0; i < n1; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			SAT_S16(sum);
			*pz++ = sum;
		}

		if (ax == ay & ax == az) {
			for (i = 0; i < n2; i++) {
				MUL_S16_SAT(load, load, store);
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				MUL_S16_SAT(load, load, storeu);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MUL_S16_SAT(load, loadu, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			SAT_S16(sum);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S16_MOD(ld1, ld2, st)		\
	xbuf = _mm_##ld1##_si128((__m128i *)px);\
	ybuf = _mm_##ld2##_si128((__m128i *)py);\
						\
	zbuf = _mm_mullo_epi16(xbuf, ybuf);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	py += nstep;				\
	pz += nstep

mlib_status
__mlib_VectorMul_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, ax, ay, az, nstep, n1, n2, n3;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf, xhi, xlo, yhi, ylo;

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			*pz++ = sum;
		}
	} else {
		for (i = 0; i < n1; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			*pz++ = sum;
		}

		if (ax == ay & ax == az) {
			for (i = 0; i < n2; i++) {
				MUL_S16_MOD(load, load, store);
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				MUL_S16_MOD(load, load, storeu);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MUL_S16_MOD(load, loadu, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S32S16_MOD(ld1, ld2, st)			\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
							\
	xlo = _mm_mullo_epi16(xbuf, ybuf);		\
	xhi = _mm_mulhi_epi16(xbuf, ybuf);		\
	ylo = _mm_unpacklo_epi16(xlo, xhi);		\
	yhi = _mm_unpackhi_epi16(xlo, xhi);		\
	_mm_##st##_si128((__m128i *)pz, ylo);		\
	_mm_##st##_si128((__m128i *)pz + 1, yhi);	\
	px += nstep;					\
	py += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorMul_S32_S16_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, ax, ay, az, nstep, n1, n2, n3;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y;
	mlib_s32 *pz = (mlib_s32 *)z;
	__m128i xbuf, ybuf, zbuf, xhi, xlo, yhi, ylo;

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			*pz++ = sum;
		}
	} else {
		for (i = 0; i < n1; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			*pz++ = sum;
		}

		if (ax == ay & (ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				MUL_S32S16_MOD(load, load, store);
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				MUL_S32S16_MOD(load, load, storeu);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MUL_S32S16_MOD(load, loadu, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
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

/* *********************************************************** */

#if 0
#define	MUL_S32_MOD(ld1, ld2, st)			\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
							\
	xlo = _mm_mullo_epi16(xbuf, ybuf);		\
	xhi = _mm_mulhi_epu16(xbuf, ybuf);		\
	xlo = _mm_and_si128(xlo, mask1);		\
	xhi = _mm_slli_epi32(xhi, 16);			\
	zbuf = _mm_or_si128(xlo, xhi);			\
							\
	xlo  = _mm_slli_epi32(xbuf, 16);		\
	ylo  = _mm_slli_epi32(ybuf, 16);		\
	xlo  = _mm_mullo_epi16(xlo, ybuf);		\
	ylo  = _mm_mullo_epi16(ylo, xbuf);		\
							\
	zbuf = _mm_add_epi32(zbuf, xlo);		\
	zbuf = _mm_add_epi32(zbuf, ylo);		\
	_mm_##st##_si128((__m128i *)pz, zbuf);		\
	px += nstep;					\
	py += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorMul_S32_S32_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, ax, ay, az, nstep, n1, n2, n3;
	mlib_s32 *px = (mlib_s32 *)x, *py = (mlib_s32 *)y, *pz = (mlib_s32 *)z;
	__m128i xbuf, ybuf, zbuf, xhi, xlo, yhi, ylo, mask1;

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s32);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s32);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			*pz++ = sum;
		}
	} else {
		mask1 = _mm_set1_epi32(0x0000ffff);

		for (i = 0; i < n1; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			*pz++ = sum;
		}

		if (ax == ay & ax == az) {
			for (i = 0; i < n2; i++) {
				MUL_S32_MOD(load, load, store);
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				MUL_S32_MOD(load, load, storeu);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MUL_S32_MOD(load, loadu, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

#else

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

/* *********************************************************** */

mlib_status
__mlib_VectorMul_U8C_U8C_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1;
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;
	__m128i xbuf, ybuf, zbuf, buf1, buf2, buf3, buf4, buf5, buf6;
	__m128i rbuf, ibuf, zero, mask1;
	zero = _mm_setzero_si128();
	mask1 = _mm_set1_epi16(0x00ff);

	for (i = 0; i < n / 8; i++) {
		xbuf = _mm_loadu_si128((__m128i *)px);
		ybuf = _mm_loadu_si128((__m128i *)py);
		buf1 = _mm_and_si128(xbuf, mask1);
		buf2 = _mm_srli_epi16(xbuf, 8);

		buf6 = _mm_unpackhi_epi8(ybuf, zero);
		buf5 = _mm_unpacklo_epi8(ybuf, zero);

		buf3 = _mm_sub_epi16(zero, buf2);
		buf4 = _mm_unpackhi_epi16(buf1, buf3);
		buf3 = _mm_unpacklo_epi16(buf1, buf3);
		rbuf = _mm_madd_epi16(buf4, buf6);
		zbuf = _mm_madd_epi16(buf3, buf5);
		rbuf = _mm_packs_epi32(zbuf, rbuf);

		buf4 = _mm_unpackhi_epi16(buf2, buf1);
		buf3 = _mm_unpacklo_epi16(buf2, buf1);
		ibuf = _mm_madd_epi16(buf4, buf6);
		zbuf = _mm_madd_epi16(buf3, buf5);
		ibuf = _mm_packs_epi32(zbuf, ibuf);

		buf1 = _mm_unpacklo_epi16(rbuf, ibuf);
		buf2 = _mm_unpackhi_epi16(rbuf, ibuf);
		zbuf = _mm_packus_epi16(buf1, buf2);
		_mm_storeu_si128((__m128i *)pz, zbuf);

		px += 16;
		py += 16;
		pz += 16;
	}

	for (i = 0; i < n % 8; i++) {
		sum0 = (mlib_s32)px[0] * py[0] - (mlib_s32)px[1] * py[1];
		sum1 = (mlib_s32)px[0] * py[1] + (mlib_s32)px[1] * py[0];
		SAT_U8(sum0);
		SAT_U8(sum1);
		pz[0] = sum0;
		pz[1] = sum1;
		px += 2;
		py += 2;
		pz += 2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1;
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;
	__m128i xbuf, ybuf, buf1, buf2, buf3, buf4, buf5, buf6;
	__m128i rbuf, ibuf, zero, mask1, mask2;
	zero = _mm_setzero_si128();
	mask1 = _mm_set1_epi16(0x00ff);
	mask2 = _mm_set1_epi32(0xffff);

	for (i = 0; i < n / 8; i++) {
		xbuf = _mm_loadu_si128((__m128i *)px);
		ybuf = _mm_loadu_si128((__m128i *)py);
		buf1 = _mm_and_si128(xbuf, mask1);
		buf2 = _mm_srli_epi16(xbuf, 8);

		buf6 = _mm_unpackhi_epi8(ybuf, zero);
		buf5 = _mm_unpacklo_epi8(ybuf, zero);

		buf3 = _mm_sub_epi16(zero, buf2);
		buf4 = _mm_unpackhi_epi16(buf1, buf3);
		buf3 = _mm_unpacklo_epi16(buf1, buf3);
		rbuf = _mm_madd_epi16(buf4, buf6);
		xbuf = _mm_madd_epi16(buf3, buf5);

		buf4 = _mm_unpackhi_epi16(buf2, buf1);
		buf3 = _mm_unpacklo_epi16(buf2, buf1);
		ibuf = _mm_madd_epi16(buf4, buf6);
		ybuf = _mm_madd_epi16(buf3, buf5);

		rbuf = _mm_and_si128(rbuf, mask1);
		ibuf = _mm_slli_epi32(ibuf, 8);
		xbuf = _mm_and_si128(xbuf, mask1);
		ybuf = _mm_slli_epi32(ybuf, 8);
		rbuf = _mm_or_si128(rbuf, ibuf);
		xbuf = _mm_or_si128(xbuf, ybuf);

		buf1 = _mm_unpacklo_epi32(xbuf, rbuf);
		buf2 = _mm_unpackhi_epi32(xbuf, rbuf);
		buf3 = _mm_unpacklo_epi32(buf1, buf2);
		buf4 = _mm_unpackhi_epi32(buf1, buf2);
		buf3 = _mm_and_si128(buf3, mask2);
		buf4 = _mm_slli_epi32(buf4, 16);
		buf3 = _mm_or_si128(buf3, buf4);

		_mm_storeu_si128((__m128i *)pz, buf3);

		px += 16;
		py += 16;
		pz += 16;
	}

	for (i = 0; i < n % 8; i++) {
		sum0 = (mlib_s32)px[0] * py[0] - (mlib_s32)px[1] * py[1];
		sum1 = (mlib_s32)px[0] * py[1] + (mlib_s32)px[1] * py[0];
		pz[0] = sum0;
		pz[1] = sum1;
		px += 2;
		py += 2;
		pz += 2;
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
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1;
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf, buf1, buf2, buf3, buf4, buf5, buf6;
	__m128i rbuf, ibuf, zero, mask1;
	zero = _mm_setzero_si128();
	mask1 = _mm_set1_epi16(0x00ff);

	for (i = 0; i < n / 8; i++) {
		xbuf = _mm_loadu_si128((__m128i *)px);
		ybuf = _mm_loadu_si128((__m128i *)py);
		buf1 = _mm_and_si128(xbuf, mask1);
		buf2 = _mm_srli_epi16(xbuf, 8);

		buf6 = _mm_unpackhi_epi8(ybuf, zero);
		buf5 = _mm_unpacklo_epi8(ybuf, zero);

		buf3 = _mm_sub_epi16(zero, buf2);
		buf4 = _mm_unpackhi_epi16(buf1, buf3);
		buf3 = _mm_unpacklo_epi16(buf1, buf3);
		rbuf = _mm_madd_epi16(buf4, buf6);
		zbuf = _mm_madd_epi16(buf3, buf5);
		rbuf = _mm_packs_epi32(zbuf, rbuf);

		buf4 = _mm_unpackhi_epi16(buf2, buf1);
		buf3 = _mm_unpacklo_epi16(buf2, buf1);
		ibuf = _mm_madd_epi16(buf4, buf6);
		zbuf = _mm_madd_epi16(buf3, buf5);
		ibuf = _mm_packs_epi32(zbuf, ibuf);

		buf1 = _mm_unpacklo_epi16(rbuf, ibuf);
		buf2 = _mm_unpackhi_epi16(rbuf, ibuf);
		_mm_storeu_si128((__m128i *)pz, buf1);
		_mm_storeu_si128((__m128i *)pz + 1, buf2);

		px += 16;
		py += 16;
		pz += 16;
	}

	for (i = 0; i < n % 8; i++) {
		sum0 = (mlib_s32)px[0] * py[0] - (mlib_s32)px[1] * py[1];
		sum1 = (mlib_s32)px[0] * py[1] + (mlib_s32)px[1] * py[0];
		SAT_S16(sum0);
		SAT_S16(sum1);
		pz[0] = sum0;
		pz[1] = sum1;
		px += 2;
		py += 2;
		pz += 2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16C_U8C_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1;
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf, buf1, buf2, buf3, buf4, buf5, buf6;
	__m128i rbuf, ibuf, zero, mask1, mask2;
	zero = _mm_setzero_si128();
	mask1 = _mm_set1_epi16(0x00ff);
	mask2 = _mm_set1_epi32(0xffff);

	for (i = 0; i < n / 8; i++) {
		xbuf = _mm_loadu_si128((__m128i *)px);
		ybuf = _mm_loadu_si128((__m128i *)py);
		buf1 = _mm_and_si128(xbuf, mask1);
		buf2 = _mm_srli_epi16(xbuf, 8);

		buf6 = _mm_unpackhi_epi8(ybuf, zero);
		buf5 = _mm_unpacklo_epi8(ybuf, zero);

		buf3 = _mm_sub_epi16(zero, buf2);
		buf4 = _mm_unpackhi_epi16(buf1, buf3);
		buf3 = _mm_unpacklo_epi16(buf1, buf3);
		rbuf = _mm_madd_epi16(buf4, buf6);
		xbuf = _mm_madd_epi16(buf3, buf5);
		rbuf = _mm_and_si128(rbuf, mask2);
		xbuf = _mm_and_si128(xbuf, mask2);

		buf4 = _mm_unpackhi_epi16(buf2, buf1);
		buf3 = _mm_unpacklo_epi16(buf2, buf1);
		ibuf = _mm_madd_epi16(buf4, buf6);
		ybuf = _mm_madd_epi16(buf3, buf5);
		ibuf = _mm_slli_epi32(ibuf, 16);
		ybuf = _mm_slli_epi32(ybuf, 16);

		rbuf = _mm_or_si128(rbuf, ibuf);
		xbuf = _mm_or_si128(xbuf, ybuf);

		_mm_storeu_si128((__m128i *)pz, xbuf);
		_mm_storeu_si128((__m128i *)pz + 1, rbuf);

		px += 16;
		py += 16;
		pz += 16;
	}

	for (i = 0; i < n % 8; i++) {
		sum0 = (mlib_s32)px[0] * py[0] - (mlib_s32)px[1] * py[1];
		sum1 = (mlib_s32)px[0] * py[1] + (mlib_s32)px[1] * py[0];
		pz[0] = sum0;
		pz[1] = sum1;
		px += 2;
		py += 2;
		pz += 2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1;
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y, *pz = (mlib_s8 *)z;
	__m128i xbuf, ybuf, zbuf, buf1, buf2, buf3, buf4, buf5, buf6;
	__m128i rbuf, ibuf, zero;
	zero = _mm_setzero_si128();

	for (i = 0; i < n / 8; i++) {
		xbuf = _mm_loadu_si128((__m128i *)px);
		ybuf = _mm_loadu_si128((__m128i *)py);
		buf1 = _mm_slli_epi16(xbuf, 8);
		buf1 = _mm_srai_epi16(buf1, 8);
		buf2 = _mm_srai_epi16(xbuf, 8);

		buf6 = _mm_unpackhi_epi8(zero, ybuf);
		buf5 = _mm_unpacklo_epi8(zero, ybuf);
		buf6 = _mm_srai_epi16(buf6, 8);
		buf5 = _mm_srai_epi16(buf5, 8);

		buf3 = _mm_sub_epi16(zero, buf2);
		buf4 = _mm_unpackhi_epi16(buf1, buf3);
		buf3 = _mm_unpacklo_epi16(buf1, buf3);
		rbuf = _mm_madd_epi16(buf4, buf6);
		zbuf = _mm_madd_epi16(buf3, buf5);
		rbuf = _mm_packs_epi32(zbuf, rbuf);

		buf4 = _mm_unpackhi_epi16(buf2, buf1);
		buf3 = _mm_unpacklo_epi16(buf2, buf1);
		ibuf = _mm_madd_epi16(buf4, buf6);
		zbuf = _mm_madd_epi16(buf3, buf5);
		ibuf = _mm_packs_epi32(zbuf, ibuf);

		buf1 = _mm_unpacklo_epi16(rbuf, ibuf);
		buf2 = _mm_unpackhi_epi16(rbuf, ibuf);
		zbuf = _mm_packs_epi16(buf1, buf2);
		_mm_storeu_si128((__m128i *)pz, zbuf);

		px += 16;
		py += 16;
		pz += 16;
	}

	for (i = 0; i < n % 8; i++) {
		sum0 = (mlib_s32)px[0] * py[0] - (mlib_s32)px[1] * py[1];
		sum1 = (mlib_s32)px[0] * py[1] + (mlib_s32)px[1] * py[0];
		SAT_S8(sum0);
		SAT_S8(sum1);
		pz[0] = sum0;
		pz[1] = sum1;
		px += 2;
		py += 2;
		pz += 2;
	}

	return (MLIB_SUCCESS);
}

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

/* *********************************************************** */

mlib_status
__mlib_VectorMul_S16C_S8C_Sat(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1;
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf, buf1, buf2, buf3, buf4, buf5, buf6;
	__m128i rbuf, ibuf, zero;
	zero = _mm_setzero_si128();

	for (i = 0; i < n / 8; i++) {
		xbuf = _mm_loadu_si128((__m128i *)px);
		ybuf = _mm_loadu_si128((__m128i *)py);
		buf1 = _mm_slli_epi16(xbuf, 8);
		buf1 = _mm_srai_epi16(buf1, 8);
		buf2 = _mm_srai_epi16(xbuf, 8);

		buf6 = _mm_unpackhi_epi8(zero, ybuf);
		buf5 = _mm_unpacklo_epi8(zero, ybuf);
		buf6 = _mm_srai_epi16(buf6, 8);
		buf5 = _mm_srai_epi16(buf5, 8);

		buf3 = _mm_sub_epi16(zero, buf2);
		buf4 = _mm_unpackhi_epi16(buf1, buf3);
		buf3 = _mm_unpacklo_epi16(buf1, buf3);
		rbuf = _mm_madd_epi16(buf4, buf6);
		zbuf = _mm_madd_epi16(buf3, buf5);
		rbuf = _mm_packs_epi32(zbuf, rbuf);

		buf4 = _mm_unpackhi_epi16(buf2, buf1);
		buf3 = _mm_unpacklo_epi16(buf2, buf1);
		ibuf = _mm_madd_epi16(buf4, buf6);
		zbuf = _mm_madd_epi16(buf3, buf5);
		ibuf = _mm_packs_epi32(zbuf, ibuf);

		buf1 = _mm_unpacklo_epi16(rbuf, ibuf);
		buf2 = _mm_unpackhi_epi16(rbuf, ibuf);
		_mm_storeu_si128((__m128i *)pz, buf1);
		_mm_storeu_si128((__m128i *)pz + 1, buf2);

		px += 16;
		py += 16;
		pz += 16;
	}

	for (i = 0; i < n % 8; i++) {
		sum0 = (mlib_s32)px[0] * py[0] - (mlib_s32)px[1] * py[1];
		sum1 = (mlib_s32)px[0] * py[1] + (mlib_s32)px[1] * py[0];
		SAT_S16(sum0);
		SAT_S16(sum1);
		pz[0] = sum0;
		pz[1] = sum1;
		px += 2;
		py += 2;
		pz += 2;
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
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1;
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf, buf1, buf2, buf3, buf4, buf5, buf6;
	__m128i rbuf, ibuf, zero, mask;
	zero = _mm_setzero_si128();
	mask = _mm_set1_epi32(0xffff);

	for (i = 0; i < n / 8; i++) {
		xbuf = _mm_loadu_si128((__m128i *)px);
		ybuf = _mm_loadu_si128((__m128i *)py);
		buf1 = _mm_slli_epi16(xbuf, 8);
		buf1 = _mm_srai_epi16(buf1, 8);
		buf2 = _mm_srai_epi16(xbuf, 8);

		buf6 = _mm_unpackhi_epi8(zero, ybuf);
		buf5 = _mm_unpacklo_epi8(zero, ybuf);
		buf6 = _mm_srai_epi16(buf6, 8);
		buf5 = _mm_srai_epi16(buf5, 8);

		buf3 = _mm_sub_epi16(zero, buf2);
		buf4 = _mm_unpackhi_epi16(buf1, buf3);
		buf3 = _mm_unpacklo_epi16(buf1, buf3);
		rbuf = _mm_madd_epi16(buf4, buf6);
		xbuf = _mm_madd_epi16(buf3, buf5);
		rbuf = _mm_and_si128(rbuf, mask);
		xbuf = _mm_and_si128(xbuf, mask);

		buf4 = _mm_unpackhi_epi16(buf2, buf1);
		buf3 = _mm_unpacklo_epi16(buf2, buf1);
		ibuf = _mm_madd_epi16(buf4, buf6);
		ybuf = _mm_madd_epi16(buf3, buf5);
		ibuf = _mm_slli_epi32(ibuf, 16);
		ybuf = _mm_slli_epi32(ybuf, 16);

		rbuf = _mm_or_si128(rbuf, ibuf);
		xbuf = _mm_or_si128(xbuf, ybuf);

		_mm_storeu_si128((__m128i *)pz, xbuf);
		_mm_storeu_si128((__m128i *)pz + 1, rbuf);

		px += 16;
		py += 16;
		pz += 16;
	}

	for (i = 0; i < n % 8; i++) {
		sum0 = (mlib_s32)px[0] * py[0] - (mlib_s32)px[1] * py[1];
		sum1 = (mlib_s32)px[0] * py[1] + (mlib_s32)px[1] * py[0];
		pz[0] = sum0;
		pz[1] = sum1;
		px += 2;
		py += 2;
		pz += 2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S16C_SAT_128(ld1, ld2, st)				\
	xbuf = _mm_##ld1##_si128((__m128i *)px);		\
	ybuf = _mm_##ld2##_si128((__m128i *)py);		\
								\
	buf2 = _mm_shufflehi_epi16(xbuf, 177);			\
	buf1 = _mm_and_si128(xbuf, mask1);			\
	buf2 = _mm_shufflelo_epi16(buf2, 177);			\
	xbuf = _mm_and_si128(xbuf, mask0);			\
								\
	/* -S16_MIN*S16_MIN = S16_MIN*S16_MIN */		\
	buf3 = _mm_madd_epi16(buf1, ybuf);			\
	xbuf = _mm_madd_epi16(xbuf, ybuf);			\
	buf3 = _mm_sub_epi32(xbuf, buf3);			\
	/* S16_MIN*S16_MIN + S16_MIN*S16_MIN > S32_MAX */	\
	buf4 = _mm_madd_epi16(buf2, ybuf);			\
	buf2 = _mm_cmpeq_epi32(buf4, mask2);			\
	buf4 = _mm_xor_si128(buf4, buf2);			\
								\
	zbuf = _mm_packs_epi32(buf3, buf4);			\
	buf1 = _mm_srli_si128(zbuf, 8);				\
	zbuf = _mm_unpacklo_epi16(zbuf, buf1);			\
	_mm_##st##_si128((__m128i *)pz, zbuf);			\
								\
	px += nstep;						\
	py += nstep;						\
	pz += nstep

#define	MUL_S16C_SAT							\
	sum0 = (mlib_s32)px[0] * py[0] - (mlib_s32)px[1] * py[1];	\
	sum1 = (mlib_s32)px[0] * py[1] + (mlib_s32)px[1] * py[0];	\
	if (sum1 == MLIB_S32_MIN)					\
		sum1 = MLIB_S32_MAX;					\
	SAT_S16(sum0);							\
	SAT_S16(sum1);							\
	pz[0] = sum0;							\
	pz[1] = sum1;							\
	px += 2;							\
	py += 2;							\
	pz += 2

mlib_status
__mlib_VectorMul_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1;
	mlib_s32 ax, ay, az, nstep, n1, n2, n3;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf, buf1, buf2, buf3, buf4, mask0, mask1, mask2;
	mask0 = _mm_set1_epi32(0x0000ffff);
	mask1 = _mm_set1_epi32(0xffff0000);
	mask2 = _mm_set1_epi32(0x80000000);

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_s16);

	if (ax & 3) {
		for (i = 0; i < n / 4; i++) {
			MUL_S16C_SAT_128(loadu, loadu, storeu);
		}

		for (i = 0; i < n % 4; i++) {
			MUL_S16C_SAT;
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				MUL_S16C_SAT;
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				MUL_S16C_SAT;
			}
			if (ax == ay && ax == az) {
				for (i = 0; i < n2; i++) {
					MUL_S16C_SAT_128(load, load, store);
				}
			} else if (ax == ay) {
				for (i = 0; i < n2; i++) {
					MUL_S16C_SAT_128(load, load, storeu);
				}
			} else {
				for (i = 0; i < n2; i++) {
					MUL_S16C_SAT_128(loadu, loadu, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				MUL_S16C_SAT;
			}
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S16C_MOD_128(ld1, ld2, st)				\
	xbuf = _mm_##ld1##_si128((__m128i *)px);		\
	ybuf = _mm_##ld2##_si128((__m128i *)py);		\
								\
	buf1 = _mm_mullo_epi16(xbuf, mask1);			\
	buf2 = _mm_shufflehi_epi16(xbuf, 177);			\
	buf2 = _mm_shufflelo_epi16(buf2, 177);			\
								\
	buf3 = _mm_madd_epi16(buf1, ybuf);			\
	buf4 = _mm_madd_epi16(buf2, ybuf);			\
	buf3 = _mm_and_si128(buf3, mask2);			\
	buf4 = _mm_slli_epi32(buf4, 16);			\
	zbuf = _mm_or_si128(buf3, buf4);			\
	_mm_##st##_si128((__m128i *)pz, zbuf);			\
								\
	px += nstep;						\
	py += nstep;						\
	pz += nstep

#define	MUL_S16C_MOD							\
	sum0 = (mlib_s32)px[0] * py[0] - (mlib_s32)px[1] * py[1];	\
	sum1 = (mlib_s32)px[0] * py[1] + (mlib_s32)px[1] * py[0];	\
	pz[0] = sum0;							\
	pz[1] = sum1;							\
	px += 2;							\
	py += 2;							\
	pz += 2

mlib_status
__mlib_VectorMul_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1;
	mlib_s32 ax, ay, az, nstep, n1, n2, n3;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf, buf1, buf2, buf3, buf4, mask1, mask2;
	mask1 = _mm_set1_epi32(0xffff0001);
	mask2 = _mm_set1_epi32(0xffff);

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_s16);

	if (ax & 3) {
		for (i = 0; i < n / 4; i++) {
			MUL_S16C_MOD_128(loadu, loadu, storeu);
		}
		for (i = 0; i < n % 4; i++) {
			MUL_S16C_MOD;
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				MUL_S16C_MOD;
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				MUL_S16C_MOD;
			}
			if (ax == ay && ax == az) {
				for (i = 0; i < n2; i++) {
					MUL_S16C_MOD_128(load, load, store);
				}
			} else if (ax == ay) {
				for (i = 0; i < n2; i++) {
					MUL_S16C_MOD_128(load, load, storeu);
				}
			} else {
				for (i = 0; i < n2; i++) {
					MUL_S16C_MOD_128(loadu, loadu, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				MUL_S16C_MOD;
			}
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S32CS16C_SAT_128(ld1, ld2, st)			\
	xbuf = _mm_##ld1##_si128((__m128i *)px);		\
	ybuf = _mm_##ld2##_si128((__m128i *)py);		\
								\
	buf2 = _mm_shufflehi_epi16(xbuf, 177);			\
	buf1 = _mm_and_si128(xbuf, mask1);			\
	buf2 = _mm_shufflelo_epi16(buf2, 177);			\
	xbuf = _mm_and_si128(xbuf, mask0);			\
								\
	/* -S16_MIN*S16_MIN = S16_MIN*S16_MIN */		\
	buf3 = _mm_madd_epi16(buf1, ybuf);			\
	xbuf = _mm_madd_epi16(xbuf, ybuf);			\
	buf3 = _mm_sub_epi32(xbuf, buf3);			\
	/* S16_MIN*S16_MIN + S16_MIN*S16_MIN > S32_MAX */	\
	buf4 = _mm_madd_epi16(buf2, ybuf);			\
	buf2 = _mm_cmpeq_epi32(buf4, mask2);			\
	buf4 = _mm_xor_si128(buf4, buf2);			\
								\
	buf1 = _mm_unpacklo_epi32(buf3, buf4);			\
	buf2 = _mm_unpackhi_epi32(buf3, buf4);			\
								\
	_mm_##st##_si128((__m128i *)pz, buf1);			\
	_mm_##st##_si128((__m128i *)pz + 1, buf2);		\
								\
	px += nstep;						\
	py += nstep;						\
	pz += nstep

#define	MUL_S32CS16C_SAT						\
	sum0 = (mlib_s32)px[0] * py[0] - (mlib_s32)px[1] * py[1];	\
	sum1 = (mlib_s32)px[0] * py[1] + (mlib_s32)px[1] * py[0];	\
	if (sum1 == MLIB_S32_MIN)					\
		sum1 = MLIB_S32_MAX;					\
	pz[0] = sum0;							\
	pz[1] = sum1;							\
	px += 2;							\
	py += 2;							\
	pz += 2

mlib_status
__mlib_VectorMul_S32C_S16C_Sat(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1;
	mlib_s32 ax, ay, az, nstep, n1, n2, n3;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y;
	mlib_s32 *pz = (mlib_s32 *)z;
	__m128i xbuf, ybuf, zbuf, buf1, buf2, buf3, buf4, mask0, mask1, mask2;
	mask0 = _mm_set1_epi32(0x0000ffff);
	mask1 = _mm_set1_epi32(0xffff0000);
	mask2 = _mm_set1_epi32(0x80000000);

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_s16);

	if (ax & 3) {
		for (i = 0; i < n / 4; i++) {
			MUL_S32CS16C_SAT_128(loadu, loadu, storeu);
		}

		for (i = 0; i < n % 4; i++) {
			MUL_S32CS16C_SAT;
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				MUL_S32CS16C_SAT;
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				MUL_S32CS16C_SAT;
			}
			if (ax == ay && (ax * 2 & 15) == az) {
				for (i = 0; i < n2; i++) {
				    MUL_S32CS16C_SAT_128(load, load, store);
				}
			} else if (ax == ay) {
				for (i = 0; i < n2; i++) {
				    MUL_S32CS16C_SAT_128(load, load, storeu);
				}
			} else {
				for (i = 0; i < n2; i++) {
				    MUL_S32CS16C_SAT_128(loadu, loadu, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				MUL_S32CS16C_SAT;
			}
		}
	}
	return (MLIB_SUCCESS);
}


/* *********************************************************** */

#define	MUL_S32CS16C_MOD_128(ld1, ld2, st)			\
	xbuf = _mm_##ld1##_si128((__m128i *)px);		\
	ybuf = _mm_##ld2##_si128((__m128i *)py);		\
								\
	buf2 = _mm_shufflehi_epi16(xbuf, 177);			\
	buf1 = _mm_and_si128(xbuf, mask1);			\
	buf2 = _mm_shufflelo_epi16(buf2, 177);			\
	xbuf = _mm_and_si128(xbuf, mask0);			\
								\
	/* -S16_MIN*S16_MIN = S16_MIN*S16_MIN */		\
	buf3 = _mm_madd_epi16(buf1, ybuf);			\
	xbuf = _mm_madd_epi16(xbuf, ybuf);			\
	buf3 = _mm_sub_epi32(xbuf, buf3);			\
	buf4 = _mm_madd_epi16(buf2, ybuf);			\
								\
	buf1 = _mm_unpacklo_epi32(buf3, buf4);			\
	buf2 = _mm_unpackhi_epi32(buf3, buf4);			\
								\
	_mm_##st##_si128((__m128i *)pz, buf1);			\
	_mm_##st##_si128((__m128i *)pz + 1, buf2);		\
								\
	px += nstep;						\
	py += nstep;						\
	pz += nstep

mlib_status
__mlib_VectorMul_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1;
	mlib_s32 ax, ay, az, nstep, n1, n2, n3;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y;
	mlib_s32 *pz = (mlib_s32 *)z;
	__m128i xbuf, ybuf, zbuf, buf1, buf2, buf3, buf4, mask0, mask1;
	mask0 = _mm_set1_epi32(0x0000ffff);
	mask1 = _mm_set1_epi32(0xffff0000);

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;
	nstep = 16 / sizeof (mlib_s16);

	if (ax & 3) {
		for (i = 0; i < n / 4; i++) {
			MUL_S32CS16C_MOD_128(loadu, loadu, storeu);
		}

		for (i = 0; i < n % 4; i++) {
			MUL_S16C_MOD;
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				MUL_S16C_MOD;
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				MUL_S16C_MOD;
			}
			if (ax == ay && (ax * 2 & 15) == az) {
				for (i = 0; i < n2; i++) {
				    MUL_S32CS16C_MOD_128(load, load, store);
				}
			} else if (ax == ay) {
				for (i = 0; i < n2; i++) {
				    MUL_S32CS16C_MOD_128(load, load, storeu);
				}
			} else {
				for (i = 0; i < n2; i++) {
				    MUL_S32CS16C_MOD_128(loadu, loadu, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				MUL_S16C_MOD;
			}
		}
	}
	return (MLIB_SUCCESS);
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
