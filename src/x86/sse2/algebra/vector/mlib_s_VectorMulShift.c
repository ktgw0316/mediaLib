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

#pragma ident	"@(#)mlib_s_VectorMulShift.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorMulShift - multiplication of vectors with shifting
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMulShift_U8_U8_Mod(mlib_u8       *z,
 *                                                const mlib_u8 *x,
 *                                                const mlib_u8 *y,
 *                                                mlib_s32      n,
 *                                                mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_U8_U8_Sat(mlib_u8       *z,
 *                                                const mlib_u8 *x,
 *                                                const mlib_u8 *y,
 *                                                mlib_s32      n,
 *                                                mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_U8C_U8C_Mod(mlib_u8       *z,
 *                                                  const mlib_u8 *x,
 *                                                  const mlib_u8 *y,
 *                                                  mlib_s32      n,
 *                                                  mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_U8C_U8C_Sat(mlib_u8       *z,
 *                                                  const mlib_u8 *x,
 *                                                  const mlib_u8 *y,
 *                                                  mlib_s32      n,
 *                                                  mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_S8_S8_Mod(mlib_s8       *z,
 *                                                const mlib_s8 *x,
 *                                                const mlib_s8 *y,
 *                                                mlib_s32      n,
 *                                                mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_S8_S8_Sat(mlib_s8       *z,
 *                                                const mlib_s8 *x,
 *                                                const mlib_s8 *y,
 *                                                mlib_s32      n,
 *                                                mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_S8C_S8C_Mod(mlib_s8       *z,
 *                                                  const mlib_s8 *x,
 *                                                  const mlib_s8 *y,
 *                                                  mlib_s32      n,
 *                                                  mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_S8C_S8C_Sat(mlib_s8       *z,
 *                                                  const mlib_s8 *x,
 *                                                  const mlib_s8 *y,
 *                                                  mlib_s32      n,
 *                                                  mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_S16_S16_Mod(mlib_s16       *z,
 *                                                  const mlib_s16 *x,
 *                                                  const mlib_s16 *y,
 *                                                  mlib_s32       n,
 *                                                  mlib_s32       shift);
 *      mlib_status mlib_VectorMulShift_S16_S16_Sat(mlib_s16       *z,
 *                                                  const mlib_s16 *x,
 *                                                  const mlib_s16 *y,
 *                                                  mlib_s32       n,
 *                                                  mlib_s32       shift);
 *      mlib_status mlib_VectorMulShift_S16C_S16C_Sat(mlib_s16       *z,
 *                                                    const mlib_s16 *x,
 *                                                    const mlib_s16 *y,
 *                                                    mlib_s32       n,
 *                                                    mlib_s32       shift);
 *      mlib_status mlib_VectorMulShift_S16C_S16C_Mod(mlib_s16       *z,
 *                                                    const mlib_s16 *x,
 *                                                    const mlib_s16 *y,
 *                                                    mlib_s32       n,
 *                                                    mlib_s32       shift);
 *      mlib_status mlib_VectorMulShift_S32_S32_Mod(mlib_s32       *z,
 *                                                  const mlib_s32 *x,
 *                                                  const mlib_s32 *y,
 *                                                  mlib_s32       n,
 *                                                  mlib_s32       shift);
 *      mlib_status mlib_VectorMulShift_S32_S32_Sat(mlib_s32       *z,
 *                                                  const mlib_s32 *x,
 *                                                  const mlib_s32 *y,
 *                                                  mlib_s32       n,
 *                                                  mlib_s32       shift);
 *      mlib_status mlib_VectorMulShift_S32C_S32C_Sat(mlib_s32       *z,
 *                                                    const mlib_s32 *x,
 *                                                    const mlib_s32 *y,
 *                                                    mlib_s32       n,
 *                                                    mlib_s32       shift);
 *      mlib_status mlib_VectorMulShift_S32C_S32C_Mod(mlib_s32       *z,
 *                                                    const mlib_s32 *x,
 *                                                    const mlib_s32 *y,
 *                                                    mlib_s32       n,
 *                                                    mlib_s32       shift);
 *      mlib_status mlib_VectorMulShift_U8_Mod(mlib_u8       *xz,
 *                                             const mlib_u8 *y,
 *                                             mlib_s32      n,
 *                                             mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_U8_Sat(mlib_u8       *xz,
 *                                             const mlib_u8 *y,
 *                                             mlib_s32      n,
 *                                             mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_U8C_Mod(mlib_u8       *xz,
 *                                              const mlib_u8 *y,
 *                                              mlib_s32      n,
 *                                              mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_U8C_Sat(mlib_u8       *xz,
 *                                              const mlib_u8 *y,
 *                                              mlib_s32      n,
 *                                              mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_S8_Mod(mlib_s8       *xz,
 *                                             const mlib_s8 *y,
 *                                             mlib_s32      n,
 *                                             mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_S8_Sat(mlib_s8       *xz,
 *                                             const mlib_s8 *y,
 *                                             mlib_s32      n,
 *                                             mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_S8C_Mod(mlib_s8       *xz,
 *                                              const mlib_s8 *y,
 *                                              mlib_s32      n,
 *                                              mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_S8C_Sat(mlib_s8       *xz,
 *                                              const mlib_s8 *y,
 *                                              mlib_s32      n,
 *                                              mlib_s32      shift);
 *      mlib_status mlib_VectorMulShift_S16_Mod(mlib_s16       *xz,
 *                                              const mlib_s16 *y,
 *                                              mlib_s32       n,
 *                                              mlib_s32       shift);
 *      mlib_status mlib_VectorMulShift_S16_Sat(mlib_s16       *xz,
 *                                              const mlib_s16 *y,
 *                                              mlib_s32       n,
 *                                              mlib_s32       shift);
 *      mlib_status mlib_VectorMulShift_S16C_Mod(mlib_s16       *xz,
 *                                               const mlib_s16 *y,
 *                                               mlib_s32       n,
 *                                               mlib_s32       shift);
 *      mlib_status mlib_VectorMulShift_S16C_Sat(mlib_s16       *xz,
 *                                               const mlib_s16 *y,
 *                                               mlib_s32       n,
 *                                               mlib_s32       shift);
 *      mlib_status mlib_VectorMulShift_S32_Mod(mlib_s32       *xz,
 *                                              const mlib_s32 *y,
 *                                              mlib_s32       n,
 *                                              mlib_s32       shift);
 *      mlib_status mlib_VectorMulShift_S32_Sat(mlib_s32       *xz,
 *                                              const mlib_s32 *y,
 *                                              mlib_s32       n,
 *                                              mlib_s32       shift);
 *      mlib_status mlib_VectorMulShift_S32C_Mod(mlib_s32       *xz,
 *                                               const mlib_s32 *y,
 *                                               mlib_s32       n,
 *                                               mlib_s32       shift);
 *      mlib_status mlib_VectorMulShift_S32C_Sat(mlib_s32       *xz,
 *                                               const mlib_s32 *y,
 *                                               mlib_s32       n,
 *                                               mlib_s32       shift);
 * ARGUMENTS
 *      z      the pointer to the first element of the result vector
 *      xz     the pointer to the first element of the first source and
 *             result vector
 *      x      the pointer to the first element of the first source vector
 *      y      the pointer to the first element of the second source vector
 *      n      the number of elements in each vector
 *      shift  the right shifting factor
 *
 * DESCRIPTION
 *      For U8, S8, S16, S32 types
 *
 *           z[i] = x[i] * y[i] * 2^(-shift); 0 <= i < n
 *
 *      For U8C, S8C, S16C, S32C types
 *
 *           z[2*i + 0] = (x[2*i + 0] * y[2*i + 0] - x[2*i + 1] * y[2*i + 1]) *
 *                        2^(-shift); 0 <= i < n
 *           z[2*i + 1] = (x[2*i + 1] * y[2*i + 0] + x[2*i + 0] * y[2*i + 1]) *
 *                        2^(-shift); 0 <= i < n
 *
 *      For _Mod mode, modulo operation is performed.  For _Sat mode,
 *      saturated operation is performed.
 *
 * RESTRICTIONS
 *      ranges of valid shift are:
 *           1 <= shift <= 8   for U8, S8, U8C, S8C types
 *           1 <= shift <= 16  for S16, S16C types
 *           1 <= shift <= 31  for S32, S32C types
 */

#include <mlib_algebra.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorMulShift_S16C_S16C_Mod = \
	__mlib_VectorMulShift_S16C_S16C_Mod
#pragma weak mlib_VectorMulShift_S16C_S16C_Sat = \
	__mlib_VectorMulShift_S16C_S16C_Sat
#pragma weak mlib_VectorMulShift_S16_S16_Mod = \
	__mlib_VectorMulShift_S16_S16_Mod
#pragma weak mlib_VectorMulShift_S16_S16_Sat = \
	__mlib_VectorMulShift_S16_S16_Sat
#pragma weak mlib_VectorMulShift_S8C_S8C_Mod = \
	__mlib_VectorMulShift_S8C_S8C_Mod
#pragma weak mlib_VectorMulShift_S8C_S8C_Sat = \
	__mlib_VectorMulShift_S8C_S8C_Sat
#pragma weak mlib_VectorMulShift_S8_S8_Mod = __mlib_VectorMulShift_S8_S8_Mod
#pragma weak mlib_VectorMulShift_S8_S8_Sat = __mlib_VectorMulShift_S8_S8_Sat
#pragma weak mlib_VectorMulShift_U8C_U8C_Mod = \
	__mlib_VectorMulShift_U8C_U8C_Mod
#pragma weak mlib_VectorMulShift_U8C_U8C_Sat = \
	__mlib_VectorMulShift_U8C_U8C_Sat
#pragma weak mlib_VectorMulShift_U8_U8_Mod = __mlib_VectorMulShift_U8_U8_Mod
#pragma weak mlib_VectorMulShift_U8_U8_Sat = __mlib_VectorMulShift_U8_U8_Sat
#pragma weak mlib_VectorMulShift_S16C_Mod = __mlib_VectorMulShift_S16C_Mod
#pragma weak mlib_VectorMulShift_S16C_Sat = __mlib_VectorMulShift_S16C_Sat
#pragma weak mlib_VectorMulShift_S16_Mod = __mlib_VectorMulShift_S16_Mod
#pragma weak mlib_VectorMulShift_S16_Sat = __mlib_VectorMulShift_S16_Sat
#pragma weak mlib_VectorMulShift_S32C_Mod = __mlib_VectorMulShift_S32C_Mod
#pragma weak mlib_VectorMulShift_S32C_S32C_Mod = \
	__mlib_VectorMulShift_S32C_S32C_Mod
#pragma weak mlib_VectorMulShift_S32C_S32C_Sat = \
	__mlib_VectorMulShift_S32C_S32C_Sat
#pragma weak mlib_VectorMulShift_S32C_Sat = __mlib_VectorMulShift_S32C_Sat
#pragma weak mlib_VectorMulShift_S32_Mod = __mlib_VectorMulShift_S32_Mod
#pragma weak mlib_VectorMulShift_S32_S32_Mod = \
	__mlib_VectorMulShift_S32_S32_Mod
#pragma weak mlib_VectorMulShift_S32_S32_Sat = \
	__mlib_VectorMulShift_S32_S32_Sat
#pragma weak mlib_VectorMulShift_S32_Sat = __mlib_VectorMulShift_S32_Sat
#pragma weak mlib_VectorMulShift_S8C_Mod = __mlib_VectorMulShift_S8C_Mod
#pragma weak mlib_VectorMulShift_S8C_Sat = __mlib_VectorMulShift_S8C_Sat
#pragma weak mlib_VectorMulShift_S8_Mod = __mlib_VectorMulShift_S8_Mod
#pragma weak mlib_VectorMulShift_S8_Sat = __mlib_VectorMulShift_S8_Sat
#pragma weak mlib_VectorMulShift_U8C_Mod = __mlib_VectorMulShift_U8C_Mod
#pragma weak mlib_VectorMulShift_U8C_Sat = __mlib_VectorMulShift_U8C_Sat
#pragma weak mlib_VectorMulShift_U8_Mod = __mlib_VectorMulShift_U8_Mod
#pragma weak mlib_VectorMulShift_U8_Sat = __mlib_VectorMulShift_U8_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VectorMulShift_S16C_S16C_Mod)
	mlib_VectorMulShift_S16C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_S16C_S16C_Mod")));
__typeof__(__mlib_VectorMulShift_S16C_S16C_Sat)
	mlib_VectorMulShift_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_S16C_S16C_Sat")));
__typeof__(__mlib_VectorMulShift_S16_S16_Mod) mlib_VectorMulShift_S16_S16_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_S16_S16_Mod")));
__typeof__(__mlib_VectorMulShift_S16_S16_Sat) mlib_VectorMulShift_S16_S16_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_S16_S16_Sat")));
__typeof__(__mlib_VectorMulShift_S8C_S8C_Mod) mlib_VectorMulShift_S8C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_S8C_S8C_Mod")));
__typeof__(__mlib_VectorMulShift_S8C_S8C_Sat) mlib_VectorMulShift_S8C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_S8C_S8C_Sat")));
__typeof__(__mlib_VectorMulShift_S8_S8_Mod) mlib_VectorMulShift_S8_S8_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_S8_S8_Mod")));
__typeof__(__mlib_VectorMulShift_S8_S8_Sat) mlib_VectorMulShift_S8_S8_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_S8_S8_Sat")));
__typeof__(__mlib_VectorMulShift_U8C_U8C_Mod) mlib_VectorMulShift_U8C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_U8C_U8C_Mod")));
__typeof__(__mlib_VectorMulShift_U8C_U8C_Sat) mlib_VectorMulShift_U8C_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_U8C_U8C_Sat")));
__typeof__(__mlib_VectorMulShift_U8_U8_Mod) mlib_VectorMulShift_U8_U8_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_U8_U8_Mod")));
__typeof__(__mlib_VectorMulShift_U8_U8_Sat) mlib_VectorMulShift_U8_U8_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_U8_U8_Sat")));
__typeof__(__mlib_VectorMulShift_S16C_Mod) mlib_VectorMulShift_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_S16C_Mod")));
__typeof__(__mlib_VectorMulShift_S16C_Sat) mlib_VectorMulShift_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_S16C_Sat")));
__typeof__(__mlib_VectorMulShift_S16_Mod) mlib_VectorMulShift_S16_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_S16_Mod")));
__typeof__(__mlib_VectorMulShift_S16_Sat) mlib_VectorMulShift_S16_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_S16_Sat")));
__typeof__(__mlib_VectorMulShift_S32C_Mod) mlib_VectorMulShift_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_S32C_Mod")));
__typeof__(__mlib_VectorMulShift_S32C_S32C_Mod)
	mlib_VectorMulShift_S32C_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_S32C_S32C_Mod")));
__typeof__(__mlib_VectorMulShift_S32C_S32C_Sat)
	mlib_VectorMulShift_S32C_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_S32C_S32C_Sat")));
__typeof__(__mlib_VectorMulShift_S32C_Sat) mlib_VectorMulShift_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_S32C_Sat")));
__typeof__(__mlib_VectorMulShift_S32_Mod) mlib_VectorMulShift_S32_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_S32_Mod")));
__typeof__(__mlib_VectorMulShift_S32_S32_Mod) mlib_VectorMulShift_S32_S32_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_S32_S32_Mod")));
__typeof__(__mlib_VectorMulShift_S32_S32_Sat) mlib_VectorMulShift_S32_S32_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_S32_S32_Sat")));
__typeof__(__mlib_VectorMulShift_S32_Sat) mlib_VectorMulShift_S32_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_S32_Sat")));
__typeof__(__mlib_VectorMulShift_S8C_Mod) mlib_VectorMulShift_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_S8C_Mod")));
__typeof__(__mlib_VectorMulShift_S8C_Sat) mlib_VectorMulShift_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_S8C_Sat")));
__typeof__(__mlib_VectorMulShift_S8_Mod) mlib_VectorMulShift_S8_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_S8_Mod")));
__typeof__(__mlib_VectorMulShift_S8_Sat) mlib_VectorMulShift_S8_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_S8_Sat")));
__typeof__(__mlib_VectorMulShift_U8C_Mod) mlib_VectorMulShift_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_U8C_Mod")));
__typeof__(__mlib_VectorMulShift_U8C_Sat) mlib_VectorMulShift_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_U8C_Sat")));
__typeof__(__mlib_VectorMulShift_U8_Mod) mlib_VectorMulShift_U8_Mod
	__attribute__((weak, alias("__mlib_VectorMulShift_U8_Mod")));
__typeof__(__mlib_VectorMulShift_U8_Sat) mlib_VectorMulShift_U8_Sat
	__attribute__((weak, alias("__mlib_VectorMulShift_U8_Sat")));
#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

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
	xlo = _mm_srli_epi16(xlo, shift);	\
	xhi = _mm_srli_epi16(xhi, shift);	\
	zbuf = _mm_packus_epi16(xlo, xhi);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	py += nstep;				\
	pz += nstep

mlib_status
__mlib_VectorMulShift_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);
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
			sum >>= shift;
			SAT_U8(sum);
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();

		for (i = 0; i < n1; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			sum >>= shift;
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
			sum >>= shift;
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
	xlo = _mm_srli_epi16(xlo, shift);	\
	xhi = _mm_srli_epi16(xhi, shift);	\
	xlo = _mm_and_si128(xlo, mask);		\
	xhi = _mm_and_si128(xhi, mask);		\
	zbuf = _mm_packus_epi16(xlo, xhi);	\
						\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	py += nstep;				\
	pz += nstep

mlib_status
__mlib_VectorMulShift_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);
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
			sum >>= shift;
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		mask = _mm_set1_epi16(0xff);

		for (i = 0; i < n1; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			sum >>= shift;
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
			sum >>= shift;
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_U8C_U8C_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);
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
		rbuf = _mm_srai_epi32(rbuf, shift);
		zbuf = _mm_srai_epi32(zbuf, shift);
		rbuf = _mm_packs_epi32(zbuf, rbuf);

		buf4 = _mm_unpackhi_epi16(buf2, buf1);
		buf3 = _mm_unpacklo_epi16(buf2, buf1);
		ibuf = _mm_madd_epi16(buf4, buf6);
		zbuf = _mm_madd_epi16(buf3, buf5);
		ibuf = _mm_srai_epi32(ibuf, shift);
		zbuf = _mm_srai_epi32(zbuf, shift);
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
		sum0 >>= shift;
		sum1 >>= shift;
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
__mlib_VectorMulShift_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);
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
		rbuf = _mm_srai_epi32(rbuf, shift);
		xbuf = _mm_srai_epi32(xbuf, shift);

		buf4 = _mm_unpackhi_epi16(buf2, buf1);
		buf3 = _mm_unpacklo_epi16(buf2, buf1);
		ibuf = _mm_madd_epi16(buf4, buf6);
		ybuf = _mm_madd_epi16(buf3, buf5);
		ibuf = _mm_srai_epi32(ibuf, shift);
		ybuf = _mm_srai_epi32(ybuf, shift);

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
		sum0 >>= shift;
		sum1 >>= shift;
		pz[0] = sum0;
		pz[1] = sum1;
		px += 2;
		py += 2;
		pz += 2;
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
	xlo = _mm_srai_epi16(xlo, shift);	\
	xhi = _mm_srai_epi16(xhi, shift);	\
	zbuf = _mm_packs_epi16(xlo, xhi);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	py += nstep;				\
	pz += nstep

mlib_status
__mlib_VectorMulShift_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);
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
			sum >>= shift;
			SAT_S8(sum);
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();

		for (i = 0; i < n1; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			sum >>= shift;
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
			sum >>= shift;
			SAT_S8(sum);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_S8_MOD(ld1, ld2, st)		\
	xbuf = _mm_##ld1##_si128((__m128i *)px);\
	ybuf = _mm_##ld2##_si128((__m128i *)py);\
						\
	xlo = _mm_unpacklo_epi8(zero, xbuf);	\
	xhi = _mm_unpackhi_epi8(zero, xbuf);	\
	ylo = _mm_unpacklo_epi8(zero, ybuf);	\
	yhi = _mm_unpackhi_epi8(zero, ybuf);	\
	xlo = _mm_mulhi_epi16(xlo, ylo);	\
	xhi = _mm_mulhi_epi16(xhi, yhi);	\
	xlo = _mm_srai_epi16(xlo, shift);	\
	xhi = _mm_srai_epi16(xhi, shift);	\
	xlo = _mm_and_si128(xlo, mask);		\
	xhi = _mm_and_si128(xhi, mask);		\
	zbuf = _mm_packus_epi16(xlo, xhi);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	py += nstep;				\
	pz += nstep

mlib_status
__mlib_VectorMulShift_S8_S8_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, ax, ay, az, nstep, n1, n2, n3;
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y, *pz = (mlib_s8 *)z;
	__m128i xbuf, ybuf, zbuf, xhi, xlo, yhi, ylo, zero, mask;

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
			sum >>= shift;
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		mask = _mm_set1_epi16(0xff);

		for (i = 0; i < n1; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			sum >>= shift;
			*pz++ = sum;
		}

		if (ax == ay & ax == az) {
			for (i = 0; i < n2; i++) {
				MUL_S8_MOD(load, load, store);
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				MUL_S8_MOD(load, load, storeu);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MUL_S8_MOD(load, loadu, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			sum >>= shift;
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);
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
		rbuf = _mm_srai_epi32(rbuf, shift);
		zbuf = _mm_srai_epi32(zbuf, shift);
		rbuf = _mm_packs_epi32(zbuf, rbuf);

		buf4 = _mm_unpackhi_epi16(buf2, buf1);
		buf3 = _mm_unpacklo_epi16(buf2, buf1);
		ibuf = _mm_madd_epi16(buf4, buf6);
		zbuf = _mm_madd_epi16(buf3, buf5);
		ibuf = _mm_srai_epi32(ibuf, shift);
		zbuf = _mm_srai_epi32(zbuf, shift);
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
		sum0 >>= shift;
		sum1 >>= shift;
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
__mlib_VectorMulShift_S8C_S8C_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1;
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y, *pz = (mlib_s8 *)z;
	__m128i xbuf, ybuf, zbuf, buf1, buf2, buf3, buf4, buf5, buf6;
	__m128i rbuf, ibuf, zero, mask;
	zero = _mm_setzero_si128();
	mask = _mm_set1_epi16(0xff);

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
		rbuf = _mm_srai_epi32(rbuf, shift);
		zbuf = _mm_srai_epi32(zbuf, shift);
		rbuf = _mm_packs_epi32(zbuf, rbuf);

		buf4 = _mm_unpackhi_epi16(buf2, buf1);
		buf3 = _mm_unpacklo_epi16(buf2, buf1);
		ibuf = _mm_madd_epi16(buf4, buf6);
		zbuf = _mm_madd_epi16(buf3, buf5);
		ibuf = _mm_srai_epi32(ibuf, shift);
		zbuf = _mm_srai_epi32(zbuf, shift);
		ibuf = _mm_packs_epi32(zbuf, ibuf);

		buf1 = _mm_unpacklo_epi16(rbuf, ibuf);
		buf2 = _mm_unpackhi_epi16(rbuf, ibuf);
		buf1 = _mm_and_si128(buf1, mask);
		buf2 = _mm_and_si128(buf2, mask);
		zbuf = _mm_packus_epi16(buf1, buf2);
		_mm_storeu_si128((__m128i *)pz, zbuf);

		px += 16;
		py += 16;
		pz += 16;
	}

	for (i = 0; i < n % 8; i++) {
		sum0 = (mlib_s32)px[0] * py[0] - (mlib_s32)px[1] * py[1];
		sum1 = (mlib_s32)px[0] * py[1] + (mlib_s32)px[1] * py[0];
		sum0 >>= shift;
		sum1 >>= shift;
		pz[0] = sum0;
		pz[1] = sum1;
		px += 2;
		py += 2;
		pz += 2;
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
	ylo = _mm_srai_epi32(ylo, shift);	\
	yhi = _mm_srai_epi32(yhi, shift);	\
	zbuf = _mm_packs_epi32(ylo, yhi);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	py += nstep;				\
	pz += nstep

mlib_status
__mlib_VectorMulShift_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);
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
			sum >>= shift;
			SAT_S16(sum);
			*pz++ = sum;
		}
	} else {
		for (i = 0; i < n1; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			sum >>= shift;
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
			sum >>= shift;
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
	xlo = _mm_mullo_epi16(xbuf, ybuf);	\
	xhi = _mm_mulhi_epi16(xbuf, ybuf);	\
	xlo = _mm_srli_epi16(xlo, shift);	\
	xhi = _mm_slli_epi16(xhi, lshift);	\
	zbuf = _mm_or_si128(xlo, xhi);		\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	py += nstep;				\
	pz += nstep

mlib_status
__mlib_VectorMulShift_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, ax, ay, az, nstep, n1, n2, n3;
	mlib_s32 lshift = 16 - shift;
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
			sum >>= shift;
			*pz++ = sum;
		}
	} else {
		for (i = 0; i < n1; i++) {
			sum = (*px++) * (mlib_s32)(*py++);
			sum >>= shift;
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
			sum >>= shift;
			*pz++ = sum;
		}
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
	buf3 = _mm_srai_epi32(buf3, shift);			\
	/* S16_MIN*S16_MIN + S16_MIN*S16_MIN > S32_MAX */	\
	buf4 = _mm_madd_epi16(buf2, ybuf);			\
	buf2 = _mm_cmpeq_epi32(buf4, mask2);			\
	buf4 = _mm_xor_si128(buf4, buf2);			\
	buf4 = _mm_srai_epi32(buf4, shift);			\
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
	sum0 >>= shift;							\
	if (sum1 == MLIB_S32_MIN)					\
		sum1 = (mlib_u32)sum1 >> shift;				\
	else								\
		sum1 >>= shift;						\
	SAT_S16(sum0);							\
	SAT_S16(sum1);							\
	pz[0] = sum0;							\
	pz[1] = sum1;							\
	px += 2;							\
	py += 2;							\
	pz += 2

mlib_status
__mlib_VectorMulShift_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);
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
	buf3 = _mm_srai_epi32(buf3, shift);			\
	buf4 = _mm_srai_epi32(buf4, shift);			\
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
	sum0 >>= shift;							\
	sum1 >>= shift;							\
	pz[0] = sum0;							\
	pz[1] = sum1;							\
	px += 2;							\
	py += 2;							\
	pz += 2

mlib_status
__mlib_VectorMulShift_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1;
	mlib_s32 ax, ay, az, nstep, n1, n2, n3;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf, buf1, buf2, buf3, buf4, mask0, mask1, mask2;
	mask0 = _mm_set1_epi32(0x0000ffff);
	mask1 = _mm_set1_epi32(0xffff0000);
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

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(X)	((mlib_s32)(X))

#else /* MLIB_USE_FTOI_CLAMPING */

#define	FLOAT2INT_CLAMP(X)                                       \
	(((X) >= MLIB_S32_MAX) ? MLIB_S32_MAX : (((X) <          \
		MLIB_S32_MIN) ? MLIB_S32_MIN : (mlib_s32)(X)))

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S32_S32_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
/*  Note: performance mistake here is 1  */
	mlib_s32 *px = (mlib_s32 *)x, *py = (mlib_s32 *)y, *pz = (mlib_s32 *)z;
	mlib_d64 mul0, mul1, mul2, mul3, mul4, mul5, mul6, mul7;
	mlib_s32 i;
	mlib_d64 sh = 1. / (mlib_d64)(1u << shift);

	if ((shift < 1) || (shift > 31))
		return (MLIB_OUTOFRANGE);

/*  unroll is needed for UIII version only  */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n - 7; i += 8) {
		mul0 = px[i] * (mlib_d64)py[i] * sh;
		mul1 = px[i + 1] * (mlib_d64)py[i + 1] * sh;
		mul2 = px[i + 2] * (mlib_d64)py[i + 2] * sh;
		mul3 = px[i + 3] * (mlib_d64)py[i + 3] * sh;
		mul4 = px[i + 4] * (mlib_d64)py[i + 4] * sh;
		mul5 = px[i + 5] * (mlib_d64)py[i + 5] * sh;
		mul6 = px[i + 6] * (mlib_d64)py[i + 6] * sh;
		mul7 = px[i + 7] * (mlib_d64)py[i + 7] * sh;
		pz[i] = FLOAT2INT_CLAMP(mul0);
		pz[i + 1] = FLOAT2INT_CLAMP(mul1);
		pz[i + 2] = FLOAT2INT_CLAMP(mul2);
		pz[i + 3] = FLOAT2INT_CLAMP(mul3);
		pz[i + 4] = FLOAT2INT_CLAMP(mul4);
		pz[i + 5] = FLOAT2INT_CLAMP(mul5);
		pz[i + 6] = FLOAT2INT_CLAMP(mul6);
		pz[i + 7] = FLOAT2INT_CLAMP(mul7);
	}

	for (; i < n; i++) {
		mul0 = px[i] * (mlib_d64)py[i] * sh;
		pz[i] = FLOAT2INT_CLAMP(mul0);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S32_S32_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s32 *px = (mlib_s32 *)x, *py = (mlib_s32 *)y, *pz = (mlib_s32 *)z;

#ifdef _NO_LONGLONG

	mlib_d64 mulhi, mullo;
	mlib_s32 valx, valy, i;
	mlib_d64 lo, hi;

/* 2^47 */
	mlib_d64 acc = 0x800000000000;

/* 2^32 */
	mlib_d64 acc1 = 0x100000000;
	mlib_s32 acc4 = 1 << 15;
	mlib_d64 mv = 1. / acc1;
	mlib_s32 shihi, shilo, slohi, slolo, s0, s1, one;

/* 2^63 */
#ifdef _MSC_VER
	mlib_d64 acc0 = (mlib_d64)(0x8000000000000000UI64);
#else
	mlib_d64 acc0 = 0x8000000000000000;
#endif /* _MSC_VER */

	if ((shift < 1) || (shift > 31))
		return (MLIB_OUTOFRANGE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		valx = px[i];
		valy = py[i];
		lo = (valy & 0xFFFF);
		hi = valy & ~0xFFFF;

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
		pz[i] = (s1 << (32 - shift)) | ((mlib_u32)s0 >> shift);

	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#else /* _NO_LONGLONG */
	mlib_d64 mulhi, mullo;
	mlib_s32 valx, valy, i;
	mlib_d64 lo, hi;

	if ((shift < 1) || (shift > 31))
		return (MLIB_OUTOFRANGE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		valx = px[i];
		valy = py[i];
		lo = valy & 0xFFFF;
		hi = (valy & ~0xFFFF) >> 16;
		mulhi = (valx * hi);
		mullo = (valx * lo);
		pz[i] = (((mlib_s64)mulhi << 16) + (mlib_s64)mullo) >> shift;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* _NO_LONGLONG */
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S32C_S32C_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
/*  Note: performance mistake here is 1  */
	mlib_s32 *px = (mlib_s32 *)x, *py = (mlib_s32 *)y, *pz = (mlib_s32 *)z;
	mlib_s32 i;
	mlib_d64 mul_r0, mul_i0;
	mlib_s32 x_r0, x_i0, y_r0, y_i0;
	mlib_d64 mul_r1, mul_i1;
	mlib_s32 x_r2, x_i2, y_r2, y_i2;
	mlib_d64 mul_r2, mul_i2;
	mlib_s32 x_r3, x_i3, y_r3, y_i3;
	mlib_d64 mul_r3, mul_i3;
	mlib_s32 x_r1, x_i1, y_r1, y_i1;
	mlib_d64 sh = 1. / (1u << shift);

	if ((shift < 1) || (shift > 31))
		return (MLIB_OUTOFRANGE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n - 3; i += 4) {
		x_r0 = px[2 * i];
		y_r0 = py[2 * i];
		x_i0 = px[2 * i + 1];
		y_i0 = py[2 * i + 1];
		x_r1 = px[2 * i + 2];
		y_r1 = py[2 * i + 2];
		x_i1 = px[2 * i + 3];
		y_i1 = py[2 * i + 3];
		x_r2 = px[2 * i + 4];
		y_r2 = py[2 * i + 4];
		x_i2 = px[2 * i + 5];
		y_i2 = py[2 * i + 5];
		x_r3 = px[2 * i + 6];
		y_r3 = py[2 * i + 6];
		x_i3 = px[2 * i + 7];
		y_i3 = py[2 * i + 7];
		mul_r0 = (x_r0 * (mlib_d64)y_r0 - x_i0 * (mlib_d64)y_i0) * sh;
		mul_i0 = (x_i0 * (mlib_d64)y_r0 + x_r0 * (mlib_d64)y_i0) * sh;
		mul_r1 = (x_r1 * (mlib_d64)y_r1 - x_i1 * (mlib_d64)y_i1) * sh;
		mul_i1 = (x_i1 * (mlib_d64)y_r1 + x_r1 * (mlib_d64)y_i1) * sh;
		mul_r2 = (x_r2 * (mlib_d64)y_r2 - x_i2 * (mlib_d64)y_i2) * sh;
		mul_i2 = (x_i2 * (mlib_d64)y_r2 + x_r2 * (mlib_d64)y_i2) * sh;
		mul_r3 = (x_r3 * (mlib_d64)y_r3 - x_i3 * (mlib_d64)y_i3) * sh;
		mul_i3 = (x_i3 * (mlib_d64)y_r3 + x_r3 * (mlib_d64)y_i3) * sh;
		pz[2 * i] = FLOAT2INT_CLAMP(mul_r0);
		pz[2 * i + 1] = FLOAT2INT_CLAMP(mul_i0);
		pz[2 * i + 2] = FLOAT2INT_CLAMP(mul_r1);
		pz[2 * i + 3] = FLOAT2INT_CLAMP(mul_i1);
		pz[2 * i + 4] = FLOAT2INT_CLAMP(mul_r2);
		pz[2 * i + 5] = FLOAT2INT_CLAMP(mul_i2);
		pz[2 * i + 6] = FLOAT2INT_CLAMP(mul_r3);
		pz[2 * i + 7] = FLOAT2INT_CLAMP(mul_i3);
	}

	for (; i < n; i++) {
		x_r0 = px[2 * i];
		y_r0 = py[2 * i];
		x_i0 = px[2 * i + 1];
		y_i0 = py[2 * i + 1];
		mul_r0 = (x_r0 * (mlib_d64)y_r0 - x_i0 * (mlib_d64)y_i0) * sh;
		mul_i0 = (x_i0 * (mlib_d64)y_r0 + x_r0 * (mlib_d64)y_i0) * sh;
		pz[2 * i] = FLOAT2INT_CLAMP(mul_r0);
		pz[2 * i + 1] = FLOAT2INT_CLAMP(mul_i0);
	}
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S32C_S32C_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s32 *px = (mlib_s32 *)x, *py = (mlib_s32 *)y, *pz = (mlib_s32 *)z;

#ifdef _NO_LONGLONG
	mlib_s32 x_r, x_i, y_r, y_i;
	mlib_s32 i;
	mlib_d64 mul0, mul1, mul2, mul3;
	mlib_d64 y_r_lo, y_r_hi, y_i_lo, y_i_hi;

/* 2^48 */
	mlib_d64 acc = 0x1000000000000;

/* 2^63 */
#ifdef _MSC_VER
	mlib_d64 acc0 = (mlib_d64)(0x8000000000000000UI64);
#else
	mlib_d64 acc0 = 0x8000000000000000;
#endif /* _MSC_VER */

/* 2^32 */
	mlib_d64 acc1 = 0x100000000;
	mlib_d64 mv = 1. / acc1;
	mlib_s32 acc4 = 1 << 16;
	mlib_s32 shihi, shilo, slohi, slolo, s0, s1, one;
	mlib_d64 mulhi, mullo;

	if ((shift < 1) || (shift > 31))
		return (MLIB_OUTOFRANGE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		x_r = px[2 * i];
		x_i = px[2 * i + 1];
		y_r = py[2 * i];
		y_i = py[2 * i + 1];

		y_r_lo = (y_r & 0xFFFF);
		y_r_hi = y_r & ~0xFFFF;
		y_i_lo = (y_i & 0xFFFF);
		y_i_hi = y_i & ~0xFFFF;

		mul0 = (x_r * y_r_lo);
		mul1 = (x_r * y_r_hi);
		mul2 = (x_i * y_i_lo);
		mul3 = (x_i * y_i_hi);

		mul0 -= mul2;
		mul1 -= mul3;

		slohi = (mlib_s32)((mul0 + acc) * mv) - acc4;
		slolo = (mlib_s32)(mul0 - (slohi * acc1) +
			(mlib_d64)MLIB_S32_MIN) - MLIB_S32_MIN;

		mulhi = (mul1 + acc0) * 0.5;
		shihi = (mlib_s32)(mulhi * mv);
		shilo = (mlib_s32)(mulhi - (shihi * acc1) +
			(mlib_d64)MLIB_S32_MIN) - MLIB_S32_MIN;

		one = (((mlib_u32)slolo >> 1) + (shilo & 0x7FFFFFFF)) >> 31;
		shihi = (shihi << 1) | ((mlib_u32)shilo >> 31);
		shilo = shilo << 1;

		s0 = shilo + slolo;
		s1 = shihi + slohi + one;
		pz[2 * i] = (s1 << (32 - shift)) | ((mlib_u32)s0 >> shift);

		mul0 = (x_i * y_r_lo);
		mul1 = (x_i * y_r_hi);
		mul2 = (x_r * y_i_lo);
		mul3 = (x_r * y_i_hi);

		mul0 += mul2;
		mul1 += mul3;

		slohi = (mlib_s32)((mul0 + acc) * mv) - acc4;
		slolo = (mlib_s32)(mul0 - (slohi * acc1) +
			(mlib_d64)MLIB_S32_MIN) - MLIB_S32_MIN;

		mulhi = (mul1 + acc0) * 0.5;
		shihi = (mlib_s32)(mulhi * mv);
		shilo = (mlib_s32)(mulhi - (shihi * acc1) +
			(mlib_d64)MLIB_S32_MIN) - MLIB_S32_MIN;

		one = (((mlib_u32)slolo >> 1) + (shilo & 0x7FFFFFFF)) >> 31;
		shihi = (shihi << 1) | ((mlib_u32)shilo >> 31);
		shilo = shilo << 1;

		s0 = shilo + slolo;
		s1 = shihi + slohi + one;
		pz[2 * i + 1] = (s1 << (32 - shift)) | ((mlib_u32)s0 >> shift);

	}
#else /* _NO_LONGLONG */

	mlib_s32 x_r, x_i, y_r, y_i;
	mlib_s32 i;
	mlib_d64 mul0, mul1, mul2, mul3;
	mlib_d64 y_r_lo, y_r_hi, y_i_lo, y_i_hi;
	mlib_s64 mulhi_r, mulhi_i;

	if ((shift < 1) || (shift > 31))
		return (MLIB_OUTOFRANGE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		x_r = px[2 * i];
		x_i = px[2 * i + 1];
		y_r = py[2 * i];
		y_i = py[2 * i + 1];

		y_r_lo = (y_r & 0xFFFF);
/* to avoid traps on UIII */
		y_r_hi = (y_r & ~0xFFFF) >> 16;
		y_i_lo = (y_i & 0xFFFF);
		y_i_hi = (y_i & ~0xFFFF) >> 16;

		mul0 = (x_r * y_r_lo);
		mul1 = (x_r * y_r_hi);
		mul2 = (x_i * y_i_lo);
		mul3 = (x_i * y_i_hi);

		mul0 -= mul2;
		mul1 -= mul3;

		mulhi_r = (mlib_s64)mul0 + ((mlib_s64)mul1 << 16);

		mul0 = (x_i * y_r_lo);
		mul1 = (x_i * y_r_hi);
		mul2 = (x_r * y_i_lo);
		mul3 = (x_r * y_i_hi);

		mul0 += mul2;
		mul1 += mul3;

		mulhi_i = (mlib_s64)mul0 + ((mlib_s64)mul1 << 16);

		pz[2 * i] = mulhi_r >> shift;
		pz[2 * i + 1] = mulhi_i >> shift;
	}
#endif /* _NO_LONGLONG */

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_U8_Sat(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulShift_U8_U8_Sat(xz, xz, y, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_U8_Mod(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulShift_U8_U8_Mod(xz, xz, y, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_U8C_Sat(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulShift_U8C_U8C_Sat(xz, xz, y, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_U8C_Mod(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulShift_U8C_U8C_Mod(xz, xz, y, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S8_Sat(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulShift_S8_S8_Sat(xz, xz, y, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S8_Mod(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulShift_S8_S8_Mod(xz, xz, y, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S8C_Sat(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulShift_S8C_S8C_Sat(xz, xz, y, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S8C_Mod(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulShift_S8C_S8C_Mod(xz, xz, y, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S16_Sat(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulShift_S16_S16_Sat(xz, xz, y, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S16_Mod(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulShift_S16_S16_Mod(xz, xz, y, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S16C_Sat(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulShift_S16C_S16C_Sat(xz, xz, y, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S16C_Mod(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulShift_S16C_S16C_Mod(xz, xz, y, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S32_Mod(
	mlib_s32 *xz,
	const mlib_s32 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulShift_S32_S32_Mod(xz, xz, y, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S32_Sat(
	mlib_s32 *xz,
	const mlib_s32 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulShift_S32_S32_Sat(xz, xz, y, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S32C_Sat(
	mlib_s32 *xz,
	const mlib_s32 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulShift_S32C_S32C_Sat(xz, xz, y, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulShift_S32C_Mod(
	mlib_s32 *xz,
	const mlib_s32 *y,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulShift_S32C_S32C_Mod(xz, xz, y, n, shift));
}

/* *********************************************************** */
