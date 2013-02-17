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

#pragma ident	"@(#)mlib_i_VectorMulSShift.c	9.5	07/11/05 SMI"

/*
 * FUNCTIONS
 *
 *      mlib_VectorMulSShift_U8_[Sat|Mod],
 *      mlib_VectorMulSShift_U8_U8_[Sat|Mod]   - multiplication of
 *                                               unsigned 8-bit format vector
 *                                               by a scalar with shifting
 *      mlib_VectorMulSShift_S8_[Sat|Mod],
 *      mlib_VectorMulSShift_S8_S8_[Sat|Mod]   - multiplication of
 *                                               signed 8-bit format vector
 *                                               by a scalar with shifting
 *      mlib_VectorMulSShift_S16_[Sat|Mod],
 *      mlib_VectorMulSShift_S16_S16_[Sat|Mod] - multiplication of
 *                                               signed 16-bit format vector
 *                                               by a scalar with shifting
 *      mlib_VectorMulSShift_S32_[Sat|Mod],
 *      mlib_VectorMulSShift_S32_S32_[Sat|Mod] - multiplication of
 *                                               signed 32-bit format vector
 *                                               by a scalar with shifting
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorMulSShift_U8_U8_[Sat|Mod](mlib_u8       *z,
 *                                                       const mlib_u8 *x,
 *                                                       const mlib_u8 *c,
 *                                                       mlib_s32       n,
 *                                                       mlib_s32   shift);
 *      mlib_status mlib_VectorMulSShift_S8_S8_[Sat|Mod](mlib_s8       *z,
 *                                                       const mlib_s8 *x,
 *                                                       const mlib_s8 *c,
 *                                                       mlib_s32       n,
 *                                                       mlib_s32   shift);
 *      mlib_status mlib_VectorMulSShift_S16_S16_[Sat|Mod](mlib_s16       *z,
 *                                                         const mlib_s16 *x,
 *                                                         const mlib_s16 *c,
 *                                                         mlib_s32       n,
 *                                                         mlib_s32   shift);
 *      mlib_status mlib_VectorMulSShift_S32_S32_[Sat|Mod](mlib_s32       *z,
 *                                                         const mlib_s32 *x,
 *                                                         const mlib_s32 *c,
 *                                                         mlib_s32       n,
 *                                                         mlib_s32   shift);
 *
 *      mlib_status mlib_VectorMulSShift_U8_[Sat|Mod](mlib_u8       *xz,
 *                                                    const mlib_u8 *c,
 *                                                    mlib_s32      n,
 *                                                    mlib_s32      shift);
 *      mlib_status mlib_VectorMulSShift_S8_[Sat|Mod](mlib_s8       *xz,
 *                                                    const mlib_s8 *c,
 *                                                    mlib_s32      n,
 *                                                    mlib_s32      shift);
 *      mlib_status mlib_VectorMulSShift_S16_[Sat|Mod](mlib_s16       *xz,
 *                                                     const mlib_s16 *c,
 *                                                     mlib_s32       n,
 *                                                     mlib_s32       shift);
 *      mlib_status mlib_VectorMulSShift_S32_[Sat|Mod](mlib_s32       *xz,
 *                                                     const mlib_s32 *c,
 *                                                     mlib_s32       n,
 *                                                     mlib_s32       shift);
 * ARGUMENTS
 *      z      the pointer to the first element of the result vector
 *      xz     the pointer to the first element of the source and result vector
 *      x      the pointer to the first element of the source vector
 *      c      the pointer to the scalar
 *      n      the number of elements in each vector
 *      shift  the right shifting factor
 *
 * DESCRIPTION
 *      z[i] = x[i] * c * 2^(-shift)
 *      xz[i] = xz[i] * c * 2^(-shift)
 */

/*
 * FUNCTIONS
 *
 *      mlib_VectorMulSShift_U8C_[Sat|Mod],
 *      mlib_VectorMulSShift_U8C_U8C_[Sat|Mod]   - multiplication of unsigned
 *                                                 8-bit format complex vector
 *                                                 by a scalar with shifting
 *      mlib_VectorMulSShift_S8C_[Sat|Mod],
 *      mlib_VectorMulSShift_S8C_S8C_[Sat|Mod]   - multiplication of signed
 *                                                 8-bit format complex vector
 *                                                 by a scalar with shifting
 *      mlib_VectorMulSShift_S16C_[Sat|Mod],
 *      mlib_VectorMulSShift_S16C_S16C_[Sat|Mod] - multiplication of signed
 *                                                 16-bit format complex vector
 *                                                 by a scalar with shifting
 *      mlib_VectorMulSShift_S32C_[Sat|Mod],
 *      mlib_VectorMulSShift_S32C_S32C_[Sat|Mod] - multiplication of signed
 *                                                 32-bit format complex vector
 *                                                 by a scalar with shifting
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorMulSShift_U8C_U8C_[Sat|Mod](mlib_u8       *z,
 *                                                        const mlib_u8 *x,
 *                                                        const mlib_u8 *c,
 *                                                        mlib_s32       n,
 *                                                        mlib_s32   shift);
 *      mlib_status mlib_VectorMulSShift_S8C_S8C_[Sat|Mod](mlib_s8       *z,
 *                                                        const mlib_s8 *x,
 *                                                        const mlib_s8 *c,
 *                                                        mlib_s32       n,
 *                                                        mlib_s32   shift);
 *      mlib_status mlib_VectorMulSShift_S16C_S16C_[Sat|Mod](mlib_s16       *z,
 *                                                          const mlib_s16 *x,
 *                                                          const mlib_s16 *c,
 *                                                          mlib_s32       n,
 *                                                          mlib_s32   shift);
 *      mlib_status mlib_VectorMulSShift_S32C_S32C_[Sat|Mod](mlib_s32       *z,
 *                                                           const mlib_s32 *x,
 *                                                           const mlib_s32 *c,
 *                                                           mlib_s32       n,
 *                                                           mlib_s32   shift);
 *
 *      mlib_status mlib_VectorMulSShift_S8C_[Sat|Mod](mlib_s8       *xz,
 *                                                     const mlib_s8 *c,
 *                                                     mlib_s32      n,
 *                                                     mlib_s32      shift);
 *      mlib_status mlib_VectorMulSShift_U8C_[Sat|Mod](mlib_u8       *xz,
 *                                                     const mlib_u8 *c,
 *                                                     mlib_s32      n,
 *                                                     mlib_s32      shift);
 *      mlib_status mlib_VectorMulSShift_S16C_[Sat|Mod](mlib_s16       *xz,
 *                                                      const mlib_s16 *c,
 *                                                      mlib_s32       n,
 *                                                      mlib_s32       shift);
 *      mlib_status mlib_VectorMulSShift_S32C_[Sat|Mod](mlib_s32       *xz,
 *                                                      const mlib_s32 *c,
 *                                                      mlib_s32       n,
 *                                                      mlib_s32       shift);
 * ARGUMENTS
 *      z      the pointer to the first complex element of the result vector.
 *             z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *             parts
 *      xz     the pointer to the first complex element of the source and
 *             result vector. xz[2*i] hold the real parts,
 *             and xz[2*i + 1] hold the imaginary parts
 *      x      the pointer to the first complex element of the source vector.
 *             x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *             parts
 *      c      the pointer to the complex scalar. c[0] hold the real part,
 *             and c[1] hold the imaginary part
 *      n      the number of complex elements in each vector
 *      shift  the right shifting factor
 *
 * DESCRIPTION
 *      z[2*i + 0] = (x[2*i + 0] * c[0] - x[2*i + 1] * c[1]) * 2^(-shift)
 *      z[2*i + 1] = (x[2*i + 1] * c[0] + x[2*i + 0] * c[1]) * 2^(-shift)
 *      xz[2*i + 0] = (xz[2*i + 0] * c[0] - xz[2*i + 1] * c[1]) * 2^(-shift)
 *      xz[2*i + 1] = (xz[2*i + 1] * c[0] + xz[2*i + 0] * c[1]) * 2^(-shift)
 */

#include <mlib_algebra.h>
#include <mlib_AlgebraUtil.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorMulSShift_S8_S8_Sat = \
	__mlib_VectorMulSShift_S8_S8_Sat
#pragma weak mlib_VectorMulSShift_S8_S8_Mod = \
	__mlib_VectorMulSShift_S8_S8_Mod
#pragma weak mlib_VectorMulSShift_S8C_S8C_Sat = \
	__mlib_VectorMulSShift_S8C_S8C_Sat
#pragma weak mlib_VectorMulSShift_S8C_S8C_Mod = \
	__mlib_VectorMulSShift_S8C_S8C_Mod
#pragma weak mlib_VectorMulSShift_S8_Sat = __mlib_VectorMulSShift_S8_Sat
#pragma weak mlib_VectorMulSShift_S8_Mod = __mlib_VectorMulSShift_S8_Mod
#pragma weak mlib_VectorMulSShift_S8C_Sat = __mlib_VectorMulSShift_S8C_Sat
#pragma weak mlib_VectorMulSShift_S8C_Mod = __mlib_VectorMulSShift_S8C_Mod
#pragma weak mlib_VectorMulSShift_U8_U8_Sat = \
	__mlib_VectorMulSShift_U8_U8_Sat
#pragma weak mlib_VectorMulSShift_U8_U8_Mod = \
	__mlib_VectorMulSShift_U8_U8_Mod
#pragma weak mlib_VectorMulSShift_U8C_U8C_Sat = \
	__mlib_VectorMulSShift_U8C_U8C_Sat
#pragma weak mlib_VectorMulSShift_U8C_U8C_Mod = \
	__mlib_VectorMulSShift_U8C_U8C_Mod
#pragma weak mlib_VectorMulSShift_U8_Sat = __mlib_VectorMulSShift_U8_Sat
#pragma weak mlib_VectorMulSShift_U8_Mod = __mlib_VectorMulSShift_U8_Mod
#pragma weak mlib_VectorMulSShift_U8C_Sat = __mlib_VectorMulSShift_U8C_Sat
#pragma weak mlib_VectorMulSShift_U8C_Mod = __mlib_VectorMulSShift_U8C_Mod
#pragma weak mlib_VectorMulSShift_S16_S16_Sat = \
	__mlib_VectorMulSShift_S16_S16_Sat
#pragma weak mlib_VectorMulSShift_S16_S16_Mod = \
	__mlib_VectorMulSShift_S16_S16_Mod
#pragma weak mlib_VectorMulSShift_S16C_S16C_Sat = \
	__mlib_VectorMulSShift_S16C_S16C_Sat
#pragma weak mlib_VectorMulSShift_S16C_S16C_Mod = \
	__mlib_VectorMulSShift_S16C_S16C_Mod
#pragma weak mlib_VectorMulSShift_S16_Sat = __mlib_VectorMulSShift_S16_Sat
#pragma weak mlib_VectorMulSShift_S16_Mod = __mlib_VectorMulSShift_S16_Mod
#pragma weak mlib_VectorMulSShift_S16C_Sat = __mlib_VectorMulSShift_S16C_Sat
#pragma weak mlib_VectorMulSShift_S16C_Mod = __mlib_VectorMulSShift_S16C_Mod
#pragma weak mlib_VectorMulSShift_S32_S32_Sat = \
	__mlib_VectorMulSShift_S32_S32_Sat
#pragma weak mlib_VectorMulSShift_S32_S32_Mod = \
	__mlib_VectorMulSShift_S32_S32_Mod
#pragma weak mlib_VectorMulSShift_S32C_S32C_Sat = \
	__mlib_VectorMulSShift_S32C_S32C_Sat
#pragma weak mlib_VectorMulSShift_S32C_S32C_Mod = \
	__mlib_VectorMulSShift_S32C_S32C_Mod
#pragma weak mlib_VectorMulSShift_S32_Sat = __mlib_VectorMulSShift_S32_Sat
#pragma weak mlib_VectorMulSShift_S32_Mod = __mlib_VectorMulSShift_S32_Mod
#pragma weak mlib_VectorMulSShift_S32C_Sat = __mlib_VectorMulSShift_S32C_Sat
#pragma weak mlib_VectorMulSShift_S32C_Mod = __mlib_VectorMulSShift_S32C_Mod

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorMulSShift_S8_S8_Sat) mlib_VectorMulSShift_S8_S8_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_S8_S8_Sat")));
__typeof__(__mlib_VectorMulSShift_S8_S8_Mod) mlib_VectorMulSShift_S8_S8_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_S8_S8_Mod")));
__typeof__(__mlib_VectorMulSShift_S8C_S8C_Sat) mlib_VectorMulSShift_S8C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_S8C_S8C_Sat")));
__typeof__(__mlib_VectorMulSShift_S8C_S8C_Mod) mlib_VectorMulSShift_S8C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_S8C_S8C_Mod")));
__typeof__(__mlib_VectorMulSShift_S8_Sat) mlib_VectorMulSShift_S8_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_S8_Sat")));
__typeof__(__mlib_VectorMulSShift_S8_Mod) mlib_VectorMulSShift_S8_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_S8_Mod")));
__typeof__(__mlib_VectorMulSShift_S8C_Sat) mlib_VectorMulSShift_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_S8C_Sat")));
__typeof__(__mlib_VectorMulSShift_S8C_Mod) mlib_VectorMulSShift_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_S8C_Mod")));
__typeof__(__mlib_VectorMulSShift_U8_U8_Sat) mlib_VectorMulSShift_U8_U8_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_U8_U8_Sat")));
__typeof__(__mlib_VectorMulSShift_U8_U8_Mod) mlib_VectorMulSShift_U8_U8_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_U8_U8_Mod")));
__typeof__(__mlib_VectorMulSShift_U8C_U8C_Sat) mlib_VectorMulSShift_U8C_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_U8C_U8C_Sat")));
__typeof__(__mlib_VectorMulSShift_U8C_U8C_Mod) mlib_VectorMulSShift_U8C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_U8C_U8C_Mod")));
__typeof__(__mlib_VectorMulSShift_U8_Sat) mlib_VectorMulSShift_U8_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_U8_Sat")));
__typeof__(__mlib_VectorMulSShift_U8_Mod) mlib_VectorMulSShift_U8_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_U8_Mod")));
__typeof__(__mlib_VectorMulSShift_U8C_Sat) mlib_VectorMulSShift_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_U8C_Sat")));
__typeof__(__mlib_VectorMulSShift_U8C_Mod) mlib_VectorMulSShift_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_U8C_Mod")));
__typeof__(__mlib_VectorMulSShift_S16_S16_Sat) mlib_VectorMulSShift_S16_S16_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_S16_S16_Sat")));
__typeof__(__mlib_VectorMulSShift_S16_S16_Mod) mlib_VectorMulSShift_S16_S16_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_S16_S16_Mod")));
__typeof__(__mlib_VectorMulSShift_S16C_S16C_Sat)
	mlib_VectorMulSShift_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_S16C_S16C_Sat")));
__typeof__(__mlib_VectorMulSShift_S16C_S16C_Mod)
	mlib_VectorMulSShift_S16C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_S16C_S16C_Mod")));
__typeof__(__mlib_VectorMulSShift_S16_Sat) mlib_VectorMulSShift_S16_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_S16_Sat")));
__typeof__(__mlib_VectorMulSShift_S16_Mod) mlib_VectorMulSShift_S16_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_S16_Mod")));
__typeof__(__mlib_VectorMulSShift_S16C_Sat) mlib_VectorMulSShift_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_S16C_Sat")));
__typeof__(__mlib_VectorMulSShift_S16C_Mod) mlib_VectorMulSShift_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_S16C_Mod")));
__typeof__(__mlib_VectorMulSShift_S32_S32_Sat) mlib_VectorMulSShift_S32_S32_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_S32_S32_Sat")));
__typeof__(__mlib_VectorMulSShift_S32_S32_Mod) mlib_VectorMulSShift_S32_S32_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_S32_S32_Mod")));
__typeof__(__mlib_VectorMulSShift_S32C_S32C_Sat)
	mlib_VectorMulSShift_S32C_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_S32C_S32C_Sat")));
__typeof__(__mlib_VectorMulSShift_S32C_S32C_Mod)
	mlib_VectorMulSShift_S32C_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_S32C_S32C_Mod")));
__typeof__(__mlib_VectorMulSShift_S32_Sat) mlib_VectorMulSShift_S32_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_S32_Sat")));
__typeof__(__mlib_VectorMulSShift_S32_Mod) mlib_VectorMulSShift_S32_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_S32_Mod")));
__typeof__(__mlib_VectorMulSShift_S32C_Sat) mlib_VectorMulSShift_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorMulSShift_S32C_Sat")));
__typeof__(__mlib_VectorMulSShift_S32C_Mod) mlib_VectorMulSShift_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorMulSShift_S32C_Mod")));

#else /* defined(__SUNPRO_C) */

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

mlib_status
__mlib_VectorMulSShift_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s8 valc_s8 = *c;
	mlib_s32 mul1, mul0, valc = valc_s8;
	mlib_s32 i_0, i_1, n2 = (n >> 1) << 1;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);
	if (n <= 0)
		return (MLIB_FAILURE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1; i_0 < n2; i_0 += 2, i_1 += 2) {

		mul0 = (x[i_0] * valc) >> shift;
		mul1 = (x[i_1] * valc) >> shift;
		z[i_0] =
			(mul0 > MLIB_S8_MAX) ? MLIB_S8_MAX : (mul0 <
			MLIB_S8_MIN ? MLIB_S8_MIN : mul0);
		z[i_1] =
			(mul1 > MLIB_S8_MAX) ? MLIB_S8_MAX : (mul1 <
			MLIB_S8_MIN ? MLIB_S8_MIN : mul1);
	}

	if (n != n2) {
		mul0 = (x[n - 1] * valc_s8) >> shift;

		z[n - 1] =
			(mul0 > MLIB_S8_MAX) ? MLIB_S8_MAX : (mul0 <
			MLIB_S8_MIN ? MLIB_S8_MIN : mul0);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S8_S8_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s32 scalar = *c;
	mlib_s32 n2 = (n >> 1) << 1;
	mlib_s32 mul0, mul1;
	mlib_s32 i0, i1;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i0 = 0, i1 = 1; i0 < n2; i0 += 2, i1 += 2) {
		mul0 = (x[i0] * scalar) >> shift;
		mul1 = (x[i1] * scalar) >> shift;
		z[i0] = mul0;
		z[i1] = mul1;
	}

	if (n != n2)
		z[n - 1] = (x[n - 1] * scalar) >> shift;

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S8_Sat(
	mlib_s8 *xz,
	const mlib_s8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_S8_S8_Sat(xz, xz, c, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S8_Mod(
	mlib_s8 *xz,
	const mlib_s8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_S8_S8_Mod(xz, xz, c, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_u8 valc_u8 = *c;
	mlib_s32 i_0, i_1;
	mlib_s32 n2 = (n >> 1) << 1;
	mlib_s32 mul1, mul0, valc = valc_u8;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);
	if (n <= 0)
		return (MLIB_FAILURE);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */

	for (i_0 = 0, i_1 = 1; i_0 < n2; i_0 += 2, i_1 += 2) {
		mul0 = (x[i_0] * valc) >> shift;
		mul1 = (x[i_1] * valc) >> shift;
		z[i_0] = ((MLIB_U8_MAX - mul0) >> 31) | mul0;
		z[i_1] = ((MLIB_U8_MAX - mul1) >> 31) | mul1;
	}

	if (n != n2) {
		mul0 = (x[n - 1] * valc) >> shift;
		z[n - 1] = ((MLIB_U8_MAX - mul0) >> 31) | mul0;
	}
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s32 scalar = *c;
	mlib_s32 n2 = (n >> 1) << 1;
	mlib_s32 mul0, mul1;
	mlib_s32 i0, i1;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i0 = 0, i1 = 1; i0 < n2; i0 += 2, i1 += 2) {
		mul0 = (x[i0] * scalar) >> shift;
		mul1 = (x[i1] * scalar) >> shift;
		z[i0] = mul0;
		z[i1] = mul1;
	}

	if (n != n2)
		z[n - 1] = (x[n - 1] * scalar) >> shift;

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_U8_Sat(
	mlib_u8 *xz,
	const mlib_u8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_U8_U8_Sat(xz, xz, c, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_U8_Mod(
	mlib_u8 *xz,
	const mlib_u8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_U8_U8_Mod(xz, xz, c, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s16 valc_s16 = *c;
	mlib_s32 mul1, mul0, valc = valc_s16;
	mlib_s32 i_0, i_1, n2 = (n >> 1) << 1;

	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);
	if (n <= 0)
		return (MLIB_FAILURE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1; i_0 < n2; i_0 += 2, i_1 += 2) {

		mul0 = (valc * x[i_0]) >> shift;
		mul1 = (valc * x[i_1]) >> shift;
		z[i_0] =
			(mul0 > MLIB_S16_MAX) ? MLIB_S16_MAX : (mul0 <
			MLIB_S16_MIN ? MLIB_S16_MIN : mul0);
		z[i_1] =
			(mul1 > MLIB_S16_MAX) ? MLIB_S16_MAX : (mul1 <
			MLIB_S16_MIN ? MLIB_S16_MIN : mul1);
	}

	if (n != n2) {
		mul0 = (x[n - 1] * valc) >> shift;

		z[n - 1] =
			(mul0 > MLIB_S16_MAX) ? MLIB_S16_MAX : (mul0 <
			MLIB_S16_MIN ? MLIB_S16_MIN : mul0);
	}
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s32 scalar = *c;
	mlib_s32 n2 = (n >> 1) << 1;
	mlib_s32 mul0, mul1;
	mlib_s32 i0, i1;

	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i0 = 0, i1 = 1; i0 < n2; i0 += 2, i1 += 2) {
		z[i0] = (x[i0] * scalar) >> shift;
		z[i1] = (x[i1] * scalar) >> shift;
	}

	if (n != n2)
		z[n - 1] = (x[n - 1] * scalar) >> shift;

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S16_Sat(
	mlib_s16 *xz,
	const mlib_s16 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_S16_S16_Sat(xz, xz, c, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S16_Mod(
	mlib_s16 *xz,
	const mlib_s16 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_S16_S16_Mod(xz, xz, c, n, shift));
}
#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S32_S32_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n,
	mlib_s32 shift)
/*  Note: performance mistake here is 1  */
{
	mlib_d64 scalar = *c;
	mlib_d64 sh = (mlib_d64)(scalar / (1u << shift));
	mlib_s32 n2 = (n / 2) * 2;
	mlib_d64 mul0, mul1;
	mlib_s32 i0, i1;

	if ((shift < 1) || (shift > 31))
		return (MLIB_OUTOFRANGE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i0 = 0, i1 = 1; i0 < n2; i0 += 2, i1 += 2) {
		mul0 = x[i0] * sh;
		mul1 = x[i1] * sh;
		z[i0] = FLOAT2INT_CLAMP(mul0);
		z[i1] = FLOAT2INT_CLAMP(mul1);
	}

	if (n % 2) {
		z[n - 1] = FLOAT2INT_CLAMP(x[n - 1] * sh);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}
#else
/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S32_S32_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n,
	mlib_s32 shift)
/*  Note: performance mistake here is 1  */
{
    mlib_s32 i, i0, i1, n2;
	mlib_s64 scal = *c;
	mlib_s64 mul;
	n2 = (n >> 1) << 1;

	if ((shift < 1) || (shift > 31))
		return (MLIB_OUTOFRANGE);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i0 = 0, i1 = 1; i0 < n2; i0 += 2, i1 += 2) {
		mul = ((mlib_s64) x[i0] * scal) >> shift;
		z[i0] =
			(mul > MLIB_S32_MAX) ? MLIB_S32_MAX : (mul <
			MLIB_S32_MIN ? MLIB_S32_MIN : mul);
		mul = ((mlib_s64) x[i1] * scal) >> shift;
		z[i1] =
			(mul > MLIB_S32_MAX) ? MLIB_S32_MAX : (mul <
			MLIB_S32_MIN ? MLIB_S32_MIN : mul);
	}
	if (n2 != n) {
		mul = ((mlib_s64) x[n-1] * scal) >> shift;
		z[n-1] =
			(mul > MLIB_S32_MAX) ? MLIB_S32_MAX : (mul <
			MLIB_S32_MIN ? MLIB_S32_MIN : mul);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S32_S32_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
#if (defined _NO_LONGLONG)
	mlib_s32 scal = *c;

/* 2^47 */
	mlib_d64 acc = 0x800000000000;

/* 2^32 */
	mlib_d64 acc1 = 0x100000000;
	mlib_d64 mv = 1. / acc1;
	mlib_s32 acc4 = 1 << 15;

/* 2^63 */
#ifdef _MSC_VER
	mlib_d64 acc0 = (mlib_d64)(0x8000000000000000UI64);
#else
	mlib_d64 acc0 = 0x8000000000000000;
#endif /* _MSC_VER */
	mlib_d64 mulhi, mullo;
	mlib_d64 lo, hi;
	mlib_s32 shihi, shilo, slohi, slolo, s0, s1, one, i;
	mlib_s32 valx;

	if ((shift < 1) || (shift > 31))
		return (MLIB_OUTOFRANGE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		valx = x[i];
		lo = scal & 0xFFFF;
		hi = scal & ~0xFFFF;

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
		z[i] = (s1 << (32 - shift)) | ((mlib_u32)s0 >> shift);

	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#else /* _NO_LONGLONG */
	mlib_s32 scal = *c;
	mlib_s32 n2 = (n / 2) * 2;
	mlib_s32 i0, i1;
	mlib_d64 scal_lo =
		(scal <
		0) ? (mlib_s32)(scal | 0xFFFF0000) : (mlib_s32)(scal & 0xFFFF);
	mlib_d64 scal_hi = scal - scal_lo, x0, x1;
	mlib_s64 mul0, mul1;

	if ((shift < 1) || (shift > 31))
		return (MLIB_OUTOFRANGE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i0 = 0, i1 = 1; i0 < n2; i0 += 2, i1 += 2) {
		x0 = x[i0];
		mul0 = x0 * scal_hi;
		z[i0] = (mul0 + (mlib_s64)(x0 * scal_lo)) >> shift;

		x1 = x[i1];
		mul1 = x1 * scal_hi;
		z[i1] = (mul1 + (mlib_s64)(x1 * scal_lo)) >> shift;
	}

	if (n % 2)
		z[n - 1] = (x[n - 1] * (mlib_s64)scal) >> shift;

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* _NO_LONGLONG */
}
#else
/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S32_S32_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s32 i0, i1, valc = c[0];
	mlib_s32 n2 = (n >> 1) << 1;
	if ((shift < 1) || (shift > 31))
		return (MLIB_OUTOFRANGE);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i0 = 0, i1 = 1; i0 < n2; i0 += 2, i1 += 2) {
		z[i0] = ((mlib_s64)x[i0] * valc) >> shift;
		z[i1] = ((mlib_s64)x[i1] * valc) >> shift;
	}

	if (n != n2)
		z[n-1] = ((mlib_s64)x[n-1] * valc) >> shift;

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

#endif
/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S32_Sat(
	mlib_s32 *xz,
	const mlib_s32 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_S32_S32_Sat(xz, xz, c, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S32_Mod(
	mlib_s32 *xz,
	const mlib_s32 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_S32_S32_Mod(xz, xz, c, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{

	mlib_s32 n2 = (n >> 1) << 2, i_0, i_1, i_2, i_3;
	mlib_s32 c_re = c[0], c_im = c[1];
	mlib_s32 x_re, x_im, mul1, mul2;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1, i_2 = 2, i_3 = 3; i_0 < n2;
		i_0 += 4, i_1 += 4, i_2 += 4, i_3 += 4) {

		x_re = x[i_0];
		x_im = x[i_1];

		mul1 = (x_re * c_re - x_im * c_im) >> shift;
		mul2 = (x_re * c_im + x_im * c_re) >> shift;
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

		mul1 = (x_re * c_re - x_im * c_im) >> shift;
		mul2 = (x_re * c_im + x_im * c_re) >> shift;
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
		mul1 = ((mlib_s32)x[index1] * c[0] -
				(mlib_s32)x[index2] * c[1]) >> shift;
		mul2 = ((mlib_s32)x[index1] * c[1]
				+ (mlib_s32)x[index2] * c[0]) >> shift;
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
__mlib_VectorMulSShift_S8C_S8C_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s32 c_r = c[0];
	mlib_s32 c_i = c[1];
	mlib_s32 n2 = (n >> 1) << 2;
	mlib_s32 x_r0, x_i0, x_r1, x_i1;
	mlib_s32 i0, i1, i2, i3;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i0 = 0, i1 = 1, i2 = 2, i3 = 3; i0 < n2;
		i0 += 4, i1 += 4, i2 += 4, i3 += 4) {

		x_r0 = x[i0];
		x_i0 = x[i1];
		z[i0] = (x_r0 * c_r - x_i0 * c_i) >> shift;
		z[i1] = (x_i0 * c_r + x_r0 * c_i) >> shift;

		x_r1 = x[i2];
		x_i1 = x[i3];
		z[i2] = (x_r1 * c_r - x_i1 * c_i) >> shift;
		z[i3] = (x_i1 * c_r + x_r1 * c_i) >> shift;

	}

	if ((n << 1) != n2) {
		x_r0 = x[2 * n - 2];
		x_i0 = x[2 * n - 1];
		z[2 * n - 2] = (x_r0 * c_r - x_i0 * c_i) >> shift;
		z[2 * n - 1] = (x_i0 * c_r + x_r0 * c_i) >> shift;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S8C_Sat(
	mlib_s8 *xz,
	const mlib_s8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_S8C_S8C_Sat(xz, xz, c, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S8C_Mod(
	mlib_s8 *xz,
	const mlib_s8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_S8C_S8C_Mod(xz, xz, c, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_U8C_U8C_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s32 n2 = (n >> 1) << 2, i_0, i_1, i_2, i_3, z_im, z_im_1;
	mlib_s32 c_re = c[0], c_im = c[1], x_re, x_im, x_re_1, x_im_1;
	mlib_s32 z_re, z_re_1;
	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1, i_2 = 2, i_3 = 3; i_0 < n2;
		i_0 += 4, i_1 += 4, i_2 += 4, i_3 += 4) {

		x_re = x[i_0];
		x_im = x[i_1];

		z_re = (x_re * c_re - x_im * c_im) >> shift;
		z_im = (x_re * c_im + x_im * c_re) >> shift;
		z[i_0] = (((MLIB_U8_MAX - z_re) >> 31) | z_re) &
				(~(z_re >> 31));
		z[i_1] = ((MLIB_U8_MAX - z_im) >> 31) | z_im;

		x_im_1 = x[i_3];
		x_re_1 = x[i_2];

		z_re_1 = (x_re_1 * c_re - x_im_1 * c_im) >> shift;
		z_im_1 = (x_re_1 * c_im + x_im_1 * c_re) >> shift;
		z[i_2] = (((MLIB_U8_MAX - z_re_1) >> 31) | z_re_1)
					& (~(z_re_1 >> 31));
		z[i_3] = ((MLIB_U8_MAX - z_im_1) >> 31) | z_im_1;
	}

	if ((n << 1) != n2) {
		mlib_s32 index1 = (n - 1) << 1;
		mlib_s32 index2 = index1 + 1;
		x_re = x[index1];
		x_im = x[index2];

		z_re = (x_re * c_re - x_im * c_im) >> shift;
		z_im = (x_re * c_im + x_im * c_re) >> shift;
		z[index1] = (((MLIB_U8_MAX - z_re) >> 31) | z_re)
				& (~(z_re >> 31));
		z[index2] = ((MLIB_U8_MAX - z_im) >> 31) | z_im;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s32 c_r = c[0];
	mlib_s32 c_i = c[1];
	mlib_s32 n2 = (n >> 1) << 2;
	mlib_s32 x_r0, x_i0, x_r1, x_i1;
	mlib_s32 mul_r0, mul_i0, mul_r1, mul_i1;
	mlib_s32 i0, i1, i2, i3;

	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i0 = 0, i1 = 1, i2 = 2, i3 = 3; i0 < n2;
		i0 += 4, i1 += 4, i2 += 4, i3 += 4) {

		x_r0 = x[i0];
		x_i0 = x[i1];
		mul_r0 = (x_r0 * c_r - x_i0 * c_i) >> shift;
		mul_i0 = (x_i0 * c_r + x_r0 * c_i) >> shift;
		z[i0] = mul_r0;
		z[i1] = mul_i0;

		x_r1 = x[i2];
		x_i1 = x[i3];
		mul_r1 = (x_r1 * c_r - x_i1 * c_i) >> shift;
		mul_i1 = (x_i1 * c_r + x_r1 * c_i) >> shift;
		z[i2] = mul_r1;
		z[i3] = mul_i1;

	}

	if ((n << 1) != n2) {
		x_r0 = x[2 * n - 2];
		x_i0 = x[2 * n - 1];
		mul_r0 = (x_r0 * c_r - x_i0 * c_i) >> shift;
		mul_i0 = (x_i0 * c_r + x_r0 * c_i) >> shift;
		z[2 * n - 2] = mul_r0;
		z[2 * n - 1] = mul_i0;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_U8C_Sat(
	mlib_u8 *xz,
	const mlib_u8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_U8C_U8C_Sat(xz, xz, c, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_U8C_Mod(
	mlib_u8 *xz,
	const mlib_u8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_U8C_U8C_Mod(xz, xz, c, n, shift));
}

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_d64 sh = 1 << (16 - shift);
	mlib_d64 c_r = c[0] * sh;
	mlib_d64 c_i = c[1] * sh;
	mlib_s32 n2 = (n / 2) * 4;
	mlib_d64 x_r0, x_i0, x_r1, x_i1;
	mlib_d64 mul_r0, mul_i0, mul_r1, mul_i1;
	mlib_s32 i0, i1, i2, i3;
	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i0 = 0, i1 = 1, i2 = 2, i3 = 3; i0 < n2;
			i0 += 4, i1 += 4, i2 += 4, i3 += 4) {
		x_r0 = x[i0];
		x_i0 = x[i1];
		mul_r0 = (x_r0 * c_r - x_i0 * c_i);
		mul_i0 = (x_i0 * c_r + x_r0 * c_i);
		z[i0] = FLOAT2INT_CLAMP(mul_r0)
			>>16;
		z[i1] = FLOAT2INT_CLAMP(mul_i0)
			>>16;

		x_r1 = x[i2];
		x_i1 = x[i3];
		mul_r1 = (x_r1 * c_r - x_i1 * c_i);
		mul_i1 = (x_i1 * c_r + x_r1 * c_i);
		z[i2] = FLOAT2INT_CLAMP(mul_r1)
		>>16;
		z[i3] = FLOAT2INT_CLAMP(mul_i1)
		>>16;
	}

	if (n % 2) {
		x_r0 = x[2 * n - 2];
		x_i0 = x[2 * n - 1];
		mul_r0 = (x_r0 * c_r - x_i0 * c_i);
		mul_i0 = (x_i0 * c_r + x_r0 * c_i);
		z[2 * n - 2] = FLOAT2INT_CLAMP(mul_r0)
		>>16;
		z[2 * n - 1] = FLOAT2INT_CLAMP(mul_i0)
		>>16;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}
#else
/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s32 n2 = (n >> 1) << 2, i_0, i_1, i_2, i_3;
	mlib_s64 c_re = c[0], c_im = c[1];
	mlib_s64 x_re, x_im;
	mlib_s64 mul1, mul2;
	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1, i_2 = 2, i_3 = 3; i_0 < n2;
		i_0 += 4, i_1 += 4, i_2 += 4, i_3 += 4) {

		x_re = x[i_0];
		x_im = x[i_1];

		mul1 = (x_re * c_re - x_im * c_im) >> shift;
		mul2 = (x_re * c_im + x_im * c_re) >> shift;
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
		mul1 = (x_re * c_re - x_im * c_im) >> shift;
		mul2 = (x_re * c_im + x_im * c_re) >> shift;
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
		mul1 = (x_re * c_re - x_im * c_im) >> shift;
		mul2 = (x_re * c_im + x_im * c_re) >> shift;
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
#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_d64 c_r = c[0];
	mlib_d64 c_i = c[1];
	mlib_s32 n2 = (n / 2) * 4;
	mlib_d64 x_r0, x_i0, x_r1, x_i1;
	mlib_s32 mul_r0, mul_i0, mul_r1, mul_i1;
	mlib_s32 i0, i1, i2, i3;

	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i0 = 0, i1 = 1, i2 = 2, i3 = 3; i0 < n2;
		i0 += 4, i1 += 4, i2 += 4, i3 += 4) {

		x_r0 = x[i0];
		x_i0 = x[i1];
		mul_r0 = ((mlib_s32)(x_r0 * c_r - x_i0 * c_i)) >> shift;
		mul_i0 = ((mlib_s32)(x_i0 * c_r + x_r0 * c_i)) >> shift;
		z[i0] = (mlib_s16)mul_r0;
		z[i1] = (mlib_s16)mul_i0;

		x_r1 = x[i2];
		x_i1 = x[i3];
		mul_r1 = ((mlib_s32)(x_r1 * c_r - x_i1 * c_i)) >> shift;
		mul_i1 = ((mlib_s32)(x_i1 * c_r + x_r1 * c_i)) >> shift;
		z[i2] = (mlib_s16)mul_r1;
		z[i3] = (mlib_s16)mul_i1;
	}

	if (n % 2) {
		x_r0 = x[2 * n - 2];
		x_i0 = x[2 * n - 1];
		mul_r0 = ((mlib_s32)(x_r0 * c_r - x_i0 * c_i)) >> shift;
		mul_i0 = ((mlib_s32)(x_i0 * c_r + x_r0 * c_i)) >> shift;
		z[2 * n - 2] = (mlib_s16)mul_r0;
		z[2 * n - 1] = (mlib_s16)mul_i0;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

}
#else
/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n,
	mlib_s32 shift)
{

	mlib_s32 i, i_1, i_0, i_2, i_3, n2 = (n >> 1) << 2;
	const mlib_s16 *px = x;
	mlib_s16 *pz = z;
	mlib_s32 val_c_r = c[0], val_c_i = c[1];
	mlib_s64 x_im, x_re;

	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1, i_2 = 2, i_3 = 3; i_0 < n2;
		i_0 += 4, i_1 += 4, i_2 += 4, i_3 += 4) {
		x_re = px[i_0];
		x_im = px[i_1];
		pz[i_0] = (x_re * val_c_r - x_im * val_c_i) >> shift;
		pz[i_1] = (x_im * val_c_r + x_re * val_c_i) >> shift;
		x_re = px[i_2];
		x_im = px[i_3];
		pz[i_2] = (x_re * val_c_r - x_im * val_c_i) >> shift;
		pz[i_3] = (x_im * val_c_r + x_re * val_c_i) >> shift;
	}

	if ((n << 1) != n2) {
		mlib_s64 mul1, mul2;
		i_0 = (n - 1) << 1;
		i_1 = i_0 + 1;
		x_re = px[i_0];
		x_im = px[i_1];
		mul1 = (x_re * val_c_r - x_im * val_c_i) >> shift;
		mul2 = (x_im * val_c_r + x_re * val_c_i) >> shift;
		pz[i_0] = mul1;
		pz[i_1] = mul2;
	}
	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}
#endif
/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S16C_Sat(
	mlib_s16 *xz,
	const mlib_s16 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_S16C_S16C_Sat(xz, xz, c, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S16C_Mod(
	mlib_s16 *xz,
	const mlib_s16 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_S16C_S16C_Mod(xz, xz, c, n, shift));
}
#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S32C_S32C_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n,
	mlib_s32 shift)
/*  Note: performance mistake here is 1  */
{
	mlib_d64 sh = 1.0 / (1u << shift);
	mlib_d64 c_r = c[0] * sh;
	mlib_d64 c_i = c[1] * sh;
	mlib_s32 n2 = (n / 2) * 4;
	mlib_d64 x_r0, x_i0, x_r1, x_i1;
	mlib_d64 mul_r0, mul_i0, mul_r1, mul_i1;
	mlib_s32 i0, i1, i2, i3;

	if ((shift < 1) || (shift > 31))
		return (MLIB_OUTOFRANGE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i0 = 0, i1 = 1, i2 = 2, i3 = 3; i0 < n2;
		i0 += 4, i1 += 4, i2 += 4, i3 += 4) {

		x_r0 = x[i0];
		x_i0 = x[i1];
		mul_r0 = (x_r0 * c_r - x_i0 * c_i);
		mul_i0 = (x_i0 * c_r + x_r0 * c_i);
		z[i0] = FLOAT2INT_CLAMP(mul_r0);
		z[i1] = FLOAT2INT_CLAMP(mul_i0);

		x_r1 = x[i2];
		x_i1 = x[i3];
		mul_r1 = (x_r1 * c_r - x_i1 * c_i);
		mul_i1 = (x_i1 * c_r + x_r1 * c_i);
		z[i2] = FLOAT2INT_CLAMP(mul_r1);
		z[i3] = FLOAT2INT_CLAMP(mul_i1);

	}

	if (n % 2) {
		x_r0 = x[2 * n - 2];
		x_i0 = x[2 * n - 1];
		mul_r0 = (x_r0 * c_r - x_i0 * c_i);
		mul_i0 = (x_i0 * c_r + x_r0 * c_i);
		z[2 * n - 2] = FLOAT2INT_CLAMP(mul_r0);
		z[2 * n - 1] = FLOAT2INT_CLAMP(mul_i0);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

}
#else
/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S32C_S32C_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n,
	mlib_s32 shift)
/*  Note: performance mistake here is 1  */
{
	mlib_s32 *px = (mlib_s32 *)x, *pz = (mlib_s32 *)z;
	mlib_s32 i, index1, index2;
	mlib_s64 mul_r, mul_i;
	mlib_s64 c_re, c_im;
	if ((shift < 1) || (shift > 31))
		return (MLIB_OUTOFRANGE);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */

	for (i = 0; i < n; i++) {
		index1 = i << 1;
		index2 = index1 + 1;
		if ((px[index1] == 0x80000000 && px[index2] == 0x80000000 &&
				c[0] == 0x80000000 && c[1] == 0x80000000)) {
			pz[index1] = 0;
			pz[index2] = MLIB_S32_MAX;
		} else {
			mul_r = (mlib_s64)((mlib_s64)px[index1] * c[0]
					- (mlib_s64)px[index2] * c[1])
					>> shift;
			mul_i = (mlib_s64)((mlib_s64)px[index1] * c[1]
					+ (mlib_s64)px[index2] * c[0])
					>> shift;
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
__mlib_VectorMulSShift_S32C_S32C_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
#if (defined _NO_LONGLONG)
	mlib_s32 c_re = c[0];
	mlib_s32 c_im = c[1];

/* 2^48 */
	mlib_d64 acc = 0x1000000000000;

/* 2^32 */
	mlib_d64 acc1 = 0x100000000;
	mlib_d64 mv = 1. / acc1;
	mlib_s32 acc4 = 1 << 16;

/* 2^63 */
#ifdef _MSC_VER
	mlib_d64 acc0 = (mlib_d64)(0x8000000000000000UI64);
#else
	mlib_d64 acc0 = 0x8000000000000000;
#endif /* _MSC_VER */
	mlib_d64 mul_re_hi, mul_re_lo, mul_im_hi, mul_im_lo;
	mlib_d64 c_re_lo, c_re_hi, c_im_lo, c_im_hi;
	mlib_s32 shihi_re, shilo_re, slohi_re, slolo_re, shihi_im, shilo_im,
		slohi_im, slolo_im;
	mlib_s32 s0_re, s1_re, s0_im, s1_im, one_re, one_im, i0, i1, n2 = n * 2;
	mlib_s32 x_re, x_im;

	if ((shift < 1) || (shift > 31))
		return (MLIB_OUTOFRANGE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i0 = 0, i1 = 1; i0 < n2; i0 += 2, i1 += 2) {
		x_re = x[i0];
		x_im = x[i1];
		c_re_lo = c_re & 0xFFFF;
		c_re_hi = c_re & ~0xFFFF;
		c_im_lo = c_im & 0xFFFF;
		c_im_hi = c_im & ~0xFFFF;

		mul_re_lo = (x_re * c_re_lo - x_im * c_im_lo);
		slohi_re = (mlib_s32)((mul_re_lo + acc) * mv) - acc4;
		slolo_re =
			(mlib_s32)(mul_re_lo - (slohi_re * acc1) +
			(mlib_d64)MLIB_S32_MIN) - MLIB_S32_MIN;

		mul_re_hi = (x_re * c_re_hi - x_im * c_im_hi);
		mul_re_hi = (mul_re_hi + acc0) * 0.5;
		shihi_re = (mlib_s32)(mul_re_hi * mv);
		shilo_re =
			(mlib_s32)(mul_re_hi - (shihi_re * acc1) +
			(mlib_d64)MLIB_S32_MIN) - MLIB_S32_MIN;

		one_re = (((mlib_u32)slolo_re >> 1) +
			(shilo_re & 0x7FFFFFFF)) >> 31;
		shihi_re = (shihi_re << 1) | ((mlib_u32)shilo_re >> 31);
		shilo_re = shilo_re << 1;

		s0_re = shilo_re + slolo_re;
		s1_re = shihi_re + slohi_re + one_re;
		z[i0] = (s1_re << (32 - shift)) | ((mlib_u32)s0_re >> shift);

		mul_im_lo = (x_im * c_re_lo + x_re * c_im_lo);
		slohi_im = (mlib_s32)((mul_im_lo + acc) * mv) - acc4;
		slolo_im =
			(mlib_s32)(mul_im_lo - (slohi_im * acc1) +
			(mlib_d64)MLIB_S32_MIN) - MLIB_S32_MIN;

		mul_im_hi = (x_im * c_re_hi + x_re * c_im_hi);
		mul_im_hi = (mul_im_hi + acc0) * 0.5;
		shihi_im = (mlib_s32)(mul_im_hi * mv);
		shilo_im =
			(mlib_s32)(mul_im_hi - (shihi_im * acc1) +
			(mlib_d64)MLIB_S32_MIN) - MLIB_S32_MIN;

		one_im = (((mlib_u32)slolo_im >> 1) +
			(shilo_im & 0x7FFFFFFF)) >> 31;
		shihi_im = (shihi_im << 1) | ((mlib_u32)shilo_im >> 31);
		shilo_im = shilo_im << 1;

		s0_im = shilo_im + slolo_im;
		s1_im = shihi_im + slohi_im + one_im;
		z[i1] = (s1_im << (32 - shift)) | ((mlib_u32)s0_im >> shift);

	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#else /* _NO_LONGLONG */
	mlib_s32 c_re = c[0];
	mlib_s32 c_im = c[1];
	mlib_d64 c_re_lo = (mlib_s32)(c_re & 0xFFFF);
	mlib_d64 c_re_hi = (mlib_s32)(c_re & ~0xFFFF);
	mlib_d64 c_im_lo = (mlib_s32)(c_im & 0xFFFF);
	mlib_d64 c_im_hi = (mlib_s32)(c_im & ~0xFFFF);
	mlib_s32 n2 = n * 2;
	mlib_d64 x_re, x_im;
	mlib_s64 r0, r1, r2, r3;
	mlib_s32 i0, i1;

	if ((shift < 1) || (shift > 31))
		return (MLIB_OUTOFRANGE);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i0 = 0, i1 = 1; i0 < n2; i0 += 2, i1 += 2) {

		x_re = x[i0];
		x_im = x[i1];

		r0 = x_re * c_re_hi - x_im * c_im_hi;
		r1 = x_re * c_re_lo - x_im * c_im_lo;

		r2 = x_re * c_im_hi + x_im * c_re_hi;
		r3 = x_re * c_im_lo + x_im * c_re_lo;

		z[i0] = (r0 + r1) >> shift;
		z[i1] = (r2 + r3) >> shift;

	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* _NO_LONGLONG */
}
#else
/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S32C_S32C_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	mlib_s64 c_re = c[0], c_im = c[1], n2 = (n >> 1) << 2;
	mlib_s32 i_0, i_1, i_2, i_3;
	mlib_s32 index1, index2;
	mlib_s64 mul1, mul2, x_re, x_im;
	if ((shift < 1) || (shift > 31))
		return (MLIB_OUTOFRANGE);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i_0 = 0, i_1 = 1, i_2 = 2, i_3 = 3; i_0 < n2;
		i_0 += 4, i_1 += 4, i_2 += 4, i_3 += 4) {
		x_re = x[i_0];
		x_im = x[i_1];
		z[i_0] = (x_re * c_re - x_im * c_im) >> shift;
		z[i_1] = (x_im * c_re + x_re * c_im) >> shift;
		x_re = x[i_2];
		x_im = x[i_3];
		z[i_2] = (x_re * c_re - x_im * c_im) >> shift;
		z[i_3] = (x_im * c_re + x_re * c_im) >> shift;
	}
	if ((n << 1) != n2) {
		index1 = (n - 1) << 1;
		index2 = index1 + 1;
		x_re = x[index1];
		x_im = x[index2];
		z[index1] = (x_re * c_re - x_im * c_im) >> shift;
		z[index2] = (x_im * c_re + x_re * c_im) >> shift;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}
#endif
/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S32C_Sat(
	mlib_s32 *xz,
	const mlib_s32 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_S32C_S32C_Sat(xz, xz, c, n, shift));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S32C_Mod(
	mlib_s32 *xz,
	const mlib_s32 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	return (__mlib_VectorMulSShift_S32C_S32C_Mod(xz, xz, c, n, shift));
}

/* *********************************************************** */
