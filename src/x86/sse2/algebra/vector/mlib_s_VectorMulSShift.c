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

#pragma ident	"@(#)mlib_s_VectorMulSShift.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
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
 *
 * SYNOPSIS
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
 *
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
 *
 * SYNOPSIS
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
 *
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

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

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

#define	MULS_S8_SAT(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);	\
						\
	xlo = _mm_unpacklo_epi8(zero, xbuf);	\
	xhi = _mm_unpackhi_epi8(zero, xbuf);	\
	xlo = _mm_mulhi_epi16(xlo, cbuf);	\
	xhi = _mm_mulhi_epi16(xhi, cbuf);	\
	xlo = _mm_srai_epi16(xlo, shift);	\
	xhi = _mm_srai_epi16(xhi, shift);	\
	zbuf = _mm_packs_epi16(xlo, xhi);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	pz += nstep

mlib_status
__mlib_VectorMulSShift_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);
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
			sum >>= shift;
			SAT_S8(sum);
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		cbuf = _mm_set1_epi16(c0 << 8);

		for (i = 0; i < n1; i++) {
			sum = (*px++) * c0;
			sum >>= shift;
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
			sum >>= shift;
			SAT_S8(sum);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MULS_S8_MOD(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);	\
						\
	xlo = _mm_unpacklo_epi8(zero, xbuf);	\
	xhi = _mm_unpackhi_epi8(zero, xbuf);	\
	xlo = _mm_mulhi_epi16(xlo, cbuf);	\
	xhi = _mm_mulhi_epi16(xhi, cbuf);	\
	xlo = _mm_srai_epi16(xlo, shift);	\
	xhi = _mm_srai_epi16(xhi, shift);	\
	xlo = _mm_and_si128(xlo, mask);		\
	xhi = _mm_and_si128(xhi, mask);		\
	zbuf = _mm_packus_epi16(xlo, xhi);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	pz += nstep

mlib_status
__mlib_VectorMulSShift_S8_S8_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, c0, ax, az, nstep, n1, n2, n3;
	mlib_s8 *px = (mlib_s8 *)x, *pz = (mlib_s8 *)z;
	__m128i xbuf, zbuf, cbuf, xlo, xhi, zero, mask;
	mask = _mm_set1_epi16(0xff);

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
			sum >>= shift;
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		cbuf = _mm_set1_epi16(c0 << 8);

		for (i = 0; i < n1; i++) {
			sum = (*px++) * c0;
			sum >>= shift;
			*pz++ = sum;
		}

		if (ax == az) {
			for (i = 0; i < n2; i++) {
				MULS_S8_MOD(load, store);
			}
		} else {
			for (i = 0; i < n2; i++) {
				MULS_S8_MOD(load, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) * c0;
			sum >>= shift;
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
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

#define	MULS_U8_SAT(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);	\
						\
	xlo = _mm_unpacklo_epi8(xbuf, zero);	\
	xhi = _mm_unpackhi_epi8(xbuf, zero);	\
	xlo = _mm_mullo_epi16(xlo, cbuf);	\
	xhi = _mm_mullo_epi16(xhi, cbuf);	\
	xlo = _mm_srli_epi16(xlo, shift);	\
	xhi = _mm_srli_epi16(xhi, shift);	\
	zbuf = _mm_packus_epi16(xlo, xhi);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	pz += nstep

mlib_status
__mlib_VectorMulSShift_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);
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
			sum >>= shift;
			SAT_U8(sum);
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		cbuf = _mm_set1_epi16(c0);

		for (i = 0; i < n1; i++) {
			sum = (*px++) * c0;
			sum >>= shift;
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
			sum >>= shift;
			SAT_U8(sum);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MULS_U8_MOD(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);	\
						\
	xlo = _mm_unpacklo_epi8(xbuf, zero);	\
	xhi = _mm_unpackhi_epi8(xbuf, zero);	\
	xlo = _mm_mullo_epi16(xlo, cbuf);	\
	xhi = _mm_mullo_epi16(xhi, cbuf);	\
	xlo = _mm_srli_epi16(xlo, shift);	\
	xhi = _mm_srli_epi16(xhi, shift);	\
	xlo = _mm_and_si128(xlo, mask);		\
	xhi = _mm_and_si128(xhi, mask);		\
	zbuf = _mm_packus_epi16(xlo, xhi);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	pz += nstep

mlib_status
__mlib_VectorMulSShift_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);
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
			sum >>= shift;
			*pz++ = sum;
		}
	} else {
		zero = _mm_setzero_si128();
		cbuf = _mm_set1_epi16(c0);
		mask = _mm_set1_epi16(0xff);

		for (i = 0; i < n1; i++) {
			sum = (*px++) * c0;
			sum >>= shift;
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
			sum >>= shift;
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
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

#define	MULS_S16_SAT(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px); \
						\
	xlo = _mm_mullo_epi16(xbuf, cbuf);	\
	xhi = _mm_mulhi_epi16(xbuf, cbuf);	\
	zbuf = _mm_unpacklo_epi16(xlo, xhi);	\
	xlo  = _mm_unpackhi_epi16(xlo, xhi);	\
	zbuf = _mm_srai_epi32(zbuf, shift);	\
	xlo  = _mm_srai_epi32(xlo, shift);	\
	zbuf = _mm_packs_epi32(zbuf, xlo);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	pz += nstep

mlib_status
__mlib_VectorMulSShift_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);
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
			sum >>= shift;
			SAT_S16(sum);
			*pz++ = sum;
		}
	} else {
		cbuf = _mm_set1_epi16(c0);

		for (i = 0; i < n1; i++) {
			sum = (*px++) * c0;
			sum >>= shift;
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
			sum >>= shift;
			SAT_S16(sum);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MULS_S16_MOD(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);	\
						\
	hbuf = _mm_mulhi_epi16(xbuf, cbuf);	\
	zbuf = _mm_mullo_epi16(xbuf, cbuf);	\
	hbuf = _mm_slli_epi16(hbuf, lshift);	\
	zbuf = _mm_srli_epi16(zbuf, shift);	\
	zbuf = _mm_or_si128(zbuf, hbuf);	\
	_mm_##st##_si128((__m128i *)pz, zbuf);	\
	px += nstep;				\
	pz += nstep

mlib_status
__mlib_VectorMulSShift_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, c0, ax, az, nstep, n1, n2, n3;
	mlib_s32 lshift = 16 - shift;
	mlib_s16 *px = (mlib_s16 *)x, *pz = (mlib_s16 *)z;
	__m128i xbuf, zbuf, hbuf, cbuf;

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
			sum >>= shift;
			*pz++ = sum;
		}
	} else {
		cbuf = _mm_set1_epi16(c0);

		for (i = 0; i < n1; i++) {
			sum = (*px++) * c0;
			sum >>= shift;
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
			sum >>= shift;
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
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

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S32_S32_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
/*  Note: performance mistake here is 1  */
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

#define	MULS_MOD						\
	sum0 = (mlib_s32)px[0] * c0 - (mlib_s32)px[1] * c1;	\
	sum1 = (mlib_s32)px[0] * c1 + (mlib_s32)px[1] * c0;	\
	sum0 >>= shift;						\
	sum1 >>= shift;						\
	pz[0] = sum0;						\
	pz[1] = sum1;						\
	px += 2;						\
	pz += 2

#define	MULS_SAT(TYPE)						\
	sum0 = (mlib_s32)px[0] * c0 - (mlib_s32)px[1] * c1;	\
	sum1 = (mlib_s32)px[0] * c1 + (mlib_s32)px[1] * c0;	\
	sum0 >>= shift;						\
	sum1 >>= shift;						\
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
	buf3 = _mm_srai_epi32(buf3, shift);		\
	buf4 = _mm_srai_epi32(buf4, shift);		\
	buf1 = _mm_srai_epi32(buf1, shift);		\
	buf2 = _mm_srai_epi32(buf2, shift);		\
							\
	rbuf = _mm_packs_epi32(buf3, buf4);		\
	ibuf = _mm_packs_epi32(buf1, buf2);		\
	buf1 = _mm_unpacklo_epi16(rbuf, ibuf);		\
	buf2 = _mm_unpackhi_epi16(rbuf, ibuf);		\
	buf1 = _mm_packus_epi16(buf1, buf2);		\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
							\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorMulSShift_U8C_U8C_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);
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

#define	MULS_U8C_MOD_128(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	buf1 = _mm_unpacklo_epi8(xbuf, zero);		\
	buf2 = _mm_unpackhi_epi8(xbuf, zero);		\
							\
	buf3 = _mm_madd_epi16(buf1, cbuf0);		\
	buf4 = _mm_madd_epi16(buf2, cbuf0);		\
	buf1 = _mm_madd_epi16(buf1, cbuf1);		\
	buf2 = _mm_madd_epi16(buf2, cbuf1);		\
	buf3 = _mm_srai_epi32(buf3, shift);		\
	buf4 = _mm_srai_epi32(buf4, shift);		\
	buf1 = _mm_srai_epi32(buf1, shift);		\
	buf2 = _mm_srai_epi32(buf2, shift);		\
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
__mlib_VectorMulSShift_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);
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
	buf3 = _mm_srai_epi32(buf3, shift);		\
	buf4 = _mm_srai_epi32(buf4, shift);		\
	buf1 = _mm_srai_epi32(buf1, shift);		\
	buf2 = _mm_srai_epi32(buf2, shift);		\
							\
	rbuf = _mm_packs_epi32(buf3, buf4);		\
	ibuf = _mm_packs_epi32(buf1, buf2);		\
	buf1 = _mm_unpacklo_epi16(rbuf, ibuf);		\
	buf2 = _mm_unpackhi_epi16(rbuf, ibuf);		\
	buf1 = _mm_packs_epi16(buf1, buf2);		\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
							\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorMulSShift_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);
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

#define	MULS_S8C_MOD_128(ld, st)			\
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
	buf3 = _mm_srai_epi32(buf3, shift);		\
	buf4 = _mm_srai_epi32(buf4, shift);		\
	buf1 = _mm_srai_epi32(buf1, shift);		\
	buf2 = _mm_srai_epi32(buf2, shift);		\
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
__mlib_VectorMulSShift_S8C_S8C_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	if ((shift < 1) || (shift > 8))
		return (MLIB_OUTOFRANGE);
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, c0, c1;
	mlib_s32 ax, az, n1, n2, n3, nstep;
	mlib_s8 *px = (mlib_s8 *)x, *pz = (mlib_s8 *)z;
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
	nstep = 16 / sizeof (mlib_s8);

	if (ax & 1) {
		for (i = 0; i < n / 8; i++) {
			MULS_S8C_MOD_128(loadu, storeu);
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
			if (ax == az) {
				for (i = 0; i < n2; i++) {
					MULS_S8C_MOD_128(load, store);
				}
			} else {
				for (i = 0; i < n2; i++) {
					MULS_S8C_MOD_128(load, storeu);
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
	rbuf = _mm_srai_epi32(rbuf, shift);		\
	ibuf = _mm_srai_epi32(ibuf, shift);		\
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
	sum0 >>= shift;						\
	if (sum1 == MLIB_S32_MIN)				\
		sum1 = (mlib_u32)sum1 >> shift;			\
	else							\
		sum1 >>= shift;					\
	SAT_S16(sum0);						\
	SAT_S16(sum1);						\
	pz[0] = sum0;						\
	pz[1] = sum1;						\
	px += 2;						\
	pz += 2

mlib_status
__mlib_VectorMulSShift_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);
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

#define	MULS_S16C_MOD_128(ld, st)			\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
							\
	buf1 = _mm_madd_epi16(xbuf, cbuf00);		\
	buf2 = _mm_madd_epi16(xbuf, cbuf01);		\
	rbuf = _mm_sub_epi32(buf1, buf2);		\
	ibuf = _mm_madd_epi16(xbuf, cbuf1);		\
	rbuf = _mm_srai_epi32(rbuf, shift);		\
	ibuf = _mm_srai_epi32(ibuf, shift);		\
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
__mlib_VectorMulSShift_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
	if ((shift < 1) || (shift > 16))
		return (MLIB_OUTOFRANGE);
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum0, sum1, c0, c1;
	mlib_s32 ax, az, n1, n2, n3, nstep;
	mlib_s16 *px = (mlib_s16 *)x, *pz = (mlib_s16 *)z;
	__m128i xbuf, rbuf, ibuf, buf1, buf2, buf3, buf4;
	__m128i cbuf00, cbuf01, cbuf1, zero, mask;

	c0 = c[0];
	c1 = c[1];
	cbuf00 = _mm_set1_epi32(c0 & 0xffff);
	cbuf01 = _mm_set1_epi32(c1 << 16);
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

/* *********************************************************** */

mlib_status
__mlib_VectorMulSShift_S32C_S32C_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n,
	mlib_s32 shift)
{
/*  Note: performance mistake here is 1  */
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
