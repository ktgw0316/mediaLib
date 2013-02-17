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

#pragma ident	"@(#)mlib_s_VectorSubS.c	9.8	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorSubS_U8 - subtraction of unsigned 8-bit format vector
 *                           from a scalar
 *      mlib_VectorSubS_S8 - subtraction of signed 8-bit format vector
 *                           from a scalar
 *
 *      mlib_VectorSubS_S16 - subtraction of signed 16-bit format vector
 *                            from a scalar
 *      mlib_VectorSubS_S32 - subtraction of signed 32-bit format vector
 *                            from a scalar
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorSubS_U8_U8_[Sat|Mod](mlib_u8       *z,
 *                                                  const mlib_u8 *x,
 *                                                  const mlib_u8 *c,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorSubS_S16_U8_[Sat|Mod](mlib_s16      *z,
 *                                                   const mlib_u8 *x,
 *                                                   const mlib_u8 *c,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_VectorSubS_S8_S8_[Sat|Mod](mlib_s8       *z,
 *                                                  const mlib_s8 *x,
 *                                                  const mlib_s8 *c,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorSubS_S16_S8_[Sat|Mod](mlib_s16      *z,
 *                                                   const mlib_s8 *x,
 *                                                   const mlib_s8 *c,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_VectorSubS_S16_S16_[Sat|Mod](mlib_s16       *z,
 *                                                    const mlib_s16 *x,
 *                                                    const mlib_s16 *c,
 *                                                    mlib_s32       n);
 *      mlib_status mlib_VectorSubS_S32_S16_[Sat|Mod](mlib_s32       *z,
 *                                                    const mlib_s16 *x,
 *                                                    const mlib_s16 *c,
 *                                                    mlib_s32       n);
 *      mlib_status mlib_VectorSubS_S32_S32_[Sat|Mod](mlib_s32       *z,
 *                                                    const mlib_s32 *x,
 *                                                    const mlib_s32 *c,
 *                                                    mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      x    pointer to the first element of the first vector
 *      c    pointer to a scalar
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *      z = c - x
 */

/*
 * FUNCTIONS
 *      mlib_VectorSubS_U8C - subtraction of unsigned 8-bit format
 *                            complex vector from a scalar
 *      mlib_VectorSubS_S8C - subtraction of signed 8-bit format
 *                            complex vector from a scalar
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorSubS_U8C_U8C_[Sat|Mod](mlib_u8       *z,
 *                                                    const mlib_u8 *x,
 *                                                    const mlib_u8 *c,
 *                                                    mlib_s32      n);
 *      mlib_status mlib_VectorSubS_S16C_U8C_[Sat|Mod](mlib_s16      *z,
 *                                                     const mlib_u8 *x,
 *                                                     const mlib_u8 *c,
 *                                                     mlib_s32      n);
 *
 *      mlib_status mlib_VectorSubS_S8C_S8C_[Sat|Mod](mlib_u8       *z,
 *                                                    const mlib_s8 *x,
 *                                                    const mlib_s8 *c,
 *                                                    mlib_s32      n);
 *      mlib_status mlib_VectorSubS_S16C_S8C_[Sat|Mod](mlib_s16      *z,
 *                                                     const mlib_s8 *x,
 *                                                     const mlib_s8 *c,
 *                                                     mlib_s32      n);
 *
 *      mlib_status mlib_VectorSubS_S16C_S16C_[Sat|Mod](mlib_s16       *z,
 *                                                      const mlib_s16 *x,
 *                                                      const mlib_s16 *c,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorSubS_S32C_S16C_[Sat|Mod](mlib_s32       *z,
 *                                                      const mlib_s16 *x,
 *                                                      const mlib_s16 *c,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorSubS_S32C_S32C_[Sat|Mod](mlib_s32       *z,
 *                                                      const mlib_s32 *x,
 *                                                      const mlib_s32 *c,
 *                                                      mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result vector.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts
 *      x    pointer to the first complex element of the first vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      c    pointer to a complex scalar. c[0] hold the real part,
 *           and c[1] hold the imaginary part
 *      n    number of complex elements in the vectors
 *
 * DESCRIPTION
 *      z = c - x
 */

/*
 * FUNCTIONS
 *      mlib_VectorSubS_U8 - subtraction of unsigned 8-bit format vector
 *                           from a scalar
 *      mlib_VectorSubS_S8 - subtraction of signed 8-bit format vector
 *                           from a scalar
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorSubS_U8_[Sat|Mod](mlib_u8       *xz,
 *                                               const mlib_u8 *c,
 *                                               mlib_s32      n);
 *      mlib_status mlib_VectorSubS_S8_[Sat|Mod](mlib_s8       *xz,
 *                                               const mlib_s8 *c,
 *                                               mlib_s32      n);
 *
 * ARGUMENTS
 *      xz    pointer to the first element of the input and the output vector
 *      c     pointer to a scalar
 *      n     number of elements in the vectors
 *
 * DESCRIPTION
 *      z = c - x
 */

/*
 * FUNCTIONS
 *      mlib_VectorSubS_U8C - subtraction of unsigned 8-bit format
 *                            complex vector from a scalar
 *      mlib_VectorSubS_S8C - subtraction of signed 8-bit format
 *                            complex vector from a scalar
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorSubS_U8C_[Sat|Mod](mlib_u8       *xz,
 *                                                const mlib_u8 *c,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorSubS_S8C_[Sat|Mod](mlib_s8       *xz,
 *                                                const mlib_s8 *c,
 *                                                mlib_s32      n);
 *
 * ARGUMENTS
 *      xz    pointer to the first complex element of the input and
 *            the output vector.
 *            x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *            parts
 *      c     pointer to a complex scalar. c[0] hold the real part,
 *            and c[1] hold the imaginary part
 *      n     number of complex elements in the vectors
 *
 * DESCRIPTION
 *      z = c - x
 */

#include <mlib_algebra.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* ******************************************************************* */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorSubS_S32_S32_Mod = __mlib_VectorSubS_S32_S32_Mod
#pragma weak mlib_VectorSubS_S32_S16_Mod = __mlib_VectorSubS_S32_S16_Mod
#pragma weak mlib_VectorSubS_S16_S16_Mod = __mlib_VectorSubS_S16_S16_Mod
#pragma weak mlib_VectorSubS_S8_Sat = __mlib_VectorSubS_S8_Sat
#pragma weak mlib_VectorSubS_S32C_S32C_Mod = __mlib_VectorSubS_S32C_S32C_Mod
#pragma weak mlib_VectorSubS_U8_Sat = __mlib_VectorSubS_U8_Sat
#pragma weak mlib_VectorSubS_S32C_S16C_Mod = __mlib_VectorSubS_S32C_S16C_Mod
#pragma weak mlib_VectorSubS_S32_S32_Sat = __mlib_VectorSubS_S32_S32_Sat
#pragma weak mlib_VectorSubS_S16C_S16C_Mod = __mlib_VectorSubS_S16C_S16C_Mod
#pragma weak mlib_VectorSubS_S16_S16_Sat = __mlib_VectorSubS_S16_S16_Sat
#pragma weak mlib_VectorSubS_S32C_S32C_Sat = __mlib_VectorSubS_S32C_S32C_Sat
#pragma weak mlib_VectorSubS_S16C_S16C_Sat = __mlib_VectorSubS_S16C_S16C_Sat
#pragma weak mlib_VectorSubS_S8C_Mod = __mlib_VectorSubS_S8C_Mod
#pragma weak mlib_VectorSubS_U8C_Mod = __mlib_VectorSubS_U8C_Mod
#pragma weak mlib_VectorSubS_S8_S8_Mod = __mlib_VectorSubS_S8_S8_Mod
#pragma weak mlib_VectorSubS_S8C_Sat = __mlib_VectorSubS_S8C_Sat
#pragma weak mlib_VectorSubS_S32_Mod = __mlib_VectorSubS_S32_Mod
#pragma weak mlib_VectorSubS_U8C_Sat = __mlib_VectorSubS_U8C_Sat
#pragma weak mlib_VectorSubS_U8_U8_Mod = __mlib_VectorSubS_U8_U8_Mod
#pragma weak mlib_VectorSubS_S16_Mod = __mlib_VectorSubS_S16_Mod
#pragma weak mlib_VectorSubS_S32C_Mod = __mlib_VectorSubS_S32C_Mod
#pragma weak mlib_VectorSubS_S8C_S8C_Mod = __mlib_VectorSubS_S8C_S8C_Mod
#pragma weak mlib_VectorSubS_S16C_Mod = __mlib_VectorSubS_S16C_Mod
#pragma weak mlib_VectorSubS_S8_S8_Sat = __mlib_VectorSubS_S8_S8_Sat
#pragma weak mlib_VectorSubS_U8C_U8C_Mod = __mlib_VectorSubS_U8C_U8C_Mod
#pragma weak mlib_VectorSubS_S32_Sat = __mlib_VectorSubS_S32_Sat
#pragma weak mlib_VectorSubS_U8_U8_Sat = __mlib_VectorSubS_U8_U8_Sat
#pragma weak mlib_VectorSubS_S16_Sat = __mlib_VectorSubS_S16_Sat
#pragma weak mlib_VectorSubS_S32C_Sat = __mlib_VectorSubS_S32C_Sat
#pragma weak mlib_VectorSubS_S8C_S8C_Sat = __mlib_VectorSubS_S8C_S8C_Sat
#pragma weak mlib_VectorSubS_S16_S8_Mod = __mlib_VectorSubS_S16_S8_Mod
#pragma weak mlib_VectorSubS_S16C_Sat = __mlib_VectorSubS_S16C_Sat
#pragma weak mlib_VectorSubS_S16_U8_Mod = __mlib_VectorSubS_S16_U8_Mod
#pragma weak mlib_VectorSubS_U8C_U8C_Sat = __mlib_VectorSubS_U8C_U8C_Sat
#pragma weak mlib_VectorSubS_S16C_S8C_Mod = __mlib_VectorSubS_S16C_S8C_Mod
#pragma weak mlib_VectorSubS_S8_Mod = __mlib_VectorSubS_S8_Mod
#pragma weak mlib_VectorSubS_S16C_U8C_Mod = __mlib_VectorSubS_S16C_U8C_Mod
#pragma weak mlib_VectorSubS_U8_Mod = __mlib_VectorSubS_U8_Mod

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorSubS_S32_S32_Mod) mlib_VectorSubS_S32_S32_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S32_S32_Mod")));
__typeof__(__mlib_VectorSubS_S32_S16_Mod) mlib_VectorSubS_S32_S16_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S32_S16_Mod")));
__typeof__(__mlib_VectorSubS_S16_S16_Mod) mlib_VectorSubS_S16_S16_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S16_S16_Mod")));
__typeof__(__mlib_VectorSubS_S8_Sat) mlib_VectorSubS_S8_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S8_Sat")));
__typeof__(__mlib_VectorSubS_S32C_S32C_Mod) mlib_VectorSubS_S32C_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S32C_S32C_Mod")));
__typeof__(__mlib_VectorSubS_U8_Sat) mlib_VectorSubS_U8_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_U8_Sat")));
__typeof__(__mlib_VectorSubS_S32C_S16C_Mod) mlib_VectorSubS_S32C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S32C_S16C_Mod")));
__typeof__(__mlib_VectorSubS_S32_S32_Sat) mlib_VectorSubS_S32_S32_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S32_S32_Sat")));
__typeof__(__mlib_VectorSubS_S16C_S16C_Mod) mlib_VectorSubS_S16C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S16C_S16C_Mod")));
__typeof__(__mlib_VectorSubS_S16_S16_Sat) mlib_VectorSubS_S16_S16_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S16_S16_Sat")));
__typeof__(__mlib_VectorSubS_S32C_S32C_Sat) mlib_VectorSubS_S32C_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S32C_S32C_Sat")));
__typeof__(__mlib_VectorSubS_S16C_S16C_Sat) mlib_VectorSubS_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S16C_S16C_Sat")));
__typeof__(__mlib_VectorSubS_S8C_Mod) mlib_VectorSubS_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S8C_Mod")));
__typeof__(__mlib_VectorSubS_U8C_Mod) mlib_VectorSubS_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_U8C_Mod")));
__typeof__(__mlib_VectorSubS_S8_S8_Mod) mlib_VectorSubS_S8_S8_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S8_S8_Mod")));
__typeof__(__mlib_VectorSubS_S8C_Sat) mlib_VectorSubS_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S8C_Sat")));
__typeof__(__mlib_VectorSubS_S32_Mod) mlib_VectorSubS_S32_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S32_Mod")));
__typeof__(__mlib_VectorSubS_U8C_Sat) mlib_VectorSubS_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_U8C_Sat")));
__typeof__(__mlib_VectorSubS_U8_U8_Mod) mlib_VectorSubS_U8_U8_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_U8_U8_Mod")));
__typeof__(__mlib_VectorSubS_S16_Mod) mlib_VectorSubS_S16_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S16_Mod")));
__typeof__(__mlib_VectorSubS_S32C_Mod) mlib_VectorSubS_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S32C_Mod")));
__typeof__(__mlib_VectorSubS_S8C_S8C_Mod) mlib_VectorSubS_S8C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S8C_S8C_Mod")));
__typeof__(__mlib_VectorSubS_S16C_Mod) mlib_VectorSubS_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S16C_Mod")));
__typeof__(__mlib_VectorSubS_S8_S8_Sat) mlib_VectorSubS_S8_S8_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S8_S8_Sat")));
__typeof__(__mlib_VectorSubS_U8C_U8C_Mod) mlib_VectorSubS_U8C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_U8C_U8C_Mod")));
__typeof__(__mlib_VectorSubS_S32_Sat) mlib_VectorSubS_S32_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S32_Sat")));
__typeof__(__mlib_VectorSubS_U8_U8_Sat) mlib_VectorSubS_U8_U8_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_U8_U8_Sat")));
__typeof__(__mlib_VectorSubS_S16_Sat) mlib_VectorSubS_S16_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S16_Sat")));
__typeof__(__mlib_VectorSubS_S32C_Sat) mlib_VectorSubS_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S32C_Sat")));
__typeof__(__mlib_VectorSubS_S8C_S8C_Sat) mlib_VectorSubS_S8C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S8C_S8C_Sat")));
__typeof__(__mlib_VectorSubS_S16_S8_Mod) mlib_VectorSubS_S16_S8_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S16_S8_Mod")));
__typeof__(__mlib_VectorSubS_S16C_Sat) mlib_VectorSubS_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_S16C_Sat")));
__typeof__(__mlib_VectorSubS_S16_U8_Mod) mlib_VectorSubS_S16_U8_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S16_U8_Mod")));
__typeof__(__mlib_VectorSubS_U8C_U8C_Sat) mlib_VectorSubS_U8C_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorSubS_U8C_U8C_Sat")));
__typeof__(__mlib_VectorSubS_S16C_S8C_Mod) mlib_VectorSubS_S16C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S16C_S8C_Mod")));
__typeof__(__mlib_VectorSubS_S8_Mod) mlib_VectorSubS_S8_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S8_Mod")));
__typeof__(__mlib_VectorSubS_S16C_U8C_Mod) mlib_VectorSubS_S16C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_S16C_U8C_Mod")));
__typeof__(__mlib_VectorSubS_U8_Mod) mlib_VectorSubS_U8_Mod
	__attribute__((weak, alias("__mlib_VectorSubS_U8_Mod")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* ******************************************************************* */

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

/* ******************************************************************* */

#define	PARAMS(TYPE)						\
	if (n < 1)						\
		return (MLIB_FAILURE);				\
								\
	mlib_s32 i, sum, ax, az, nstep, n1, n2, n3, c0, c1;	\
	TYPE *px = (TYPE *)x, *pz = (TYPE *)z;			\
	__m128i xbuf, zbuf, cbuf;				\
								\
	ax = (mlib_addr)x & 15;					\
	az = (mlib_addr)z & 15;					\
								\
	nstep = 16 / sizeof (TYPE);				\
	n1 = ((16 - ((mlib_addr)x & 15)) & 15) / sizeof (TYPE);	\
	n2 = (nn - n1) / nstep;					\
	n3 = nn - n1 - n2 * nstep

/* ******************************************************************* */

#define	VECTOR_SUBS(mmsub)					\
	if (ax == az) {						\
		for (i = 0; i < n2; i++) {			\
			xbuf = _mm_load_si128((__m128i *)px);	\
			zbuf = mmsub(cbuf, xbuf);		\
			_mm_store_si128((__m128i *)pz, zbuf);	\
			px += nstep;				\
			pz += nstep;				\
		}						\
	} else {						\
		for (i = 0; i < n2; i++) {			\
			xbuf = _mm_load_si128((__m128i *)px);	\
			zbuf = mmsub(cbuf, xbuf);		\
			_mm_storeu_si128((__m128i *)pz, zbuf);	\
			px += nstep;				\
			pz += nstep;				\
		}						\
	}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_s32 nn = n;
	PARAMS(mlib_u8);

	c0 = *c;
	cbuf = _mm_set1_epi8(c0);

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = -(*px++) + c0;
			SAT_U8(sum);
			*pz++ = sum;
		}
	} else {
		for (i = 0; i < n1; i++) {
			sum = -(*px++) + c0;
			SAT_U8(sum);
			*pz++ = sum;
		}

		VECTOR_SUBS(_mm_subs_epu8);

		for (i = 0; i < n3; i++) {
			sum = -(*px++) + c0;
			SAT_U8(sum);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_s32 nn = n;
	PARAMS(mlib_u8);

	c0 = *c;
	cbuf = _mm_set1_epi8(c0);

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = -(*px++) + c0;
			*pz++ = sum;
		}
	} else {
		for (i = 0; i < n1; i++) {
			sum = -(*px++) + c0;
			*pz++ = sum;
		}

		VECTOR_SUBS(_mm_sub_epi8);

		for (i = 0; i < n3; i++) {
			sum = -(*px++) + c0;
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S16_U8_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, ax, az, nstep, n1, n2, n3, c0;
	mlib_u8 *px = (mlib_u8 *)x;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, zbuf, xbuf1, zbuf1, cbuf, zero;

	c0   = *c;
	cbuf = _mm_set1_epi16(c0);
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ((mlib_addr)x & 15)) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {

		for (i = 0; i < n; i++) {
			*pz++ = -(mlib_s32)(*px++) + c0;
		}
	} else {
		for (i = 0; i < n1; i++) {
			*pz++ = -(mlib_s32)(*px++) + c0;
		}

		if ((ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				xbuf  = _mm_load_si128((__m128i *)px);
				xbuf1 = _mm_unpacklo_epi8(xbuf, zero);
				xbuf  = _mm_unpackhi_epi8(xbuf, zero);
				zbuf1 = _mm_sub_epi16(cbuf, xbuf1);
				zbuf  = _mm_sub_epi16(cbuf, xbuf);
				_mm_store_si128((__m128i *)pz, zbuf1);
				_mm_store_si128((__m128i *)pz + 1, zbuf);
				px += nstep;
				pz += nstep;
			}
		} else {
			for (i = 0; i < n2; i++) {
				xbuf  = _mm_load_si128((__m128i *)px);
				xbuf1 = _mm_unpacklo_epi8(xbuf, zero);
				xbuf  = _mm_unpackhi_epi8(xbuf, zero);
				zbuf1 = _mm_sub_epi16(cbuf, xbuf1);
				zbuf  = _mm_sub_epi16(cbuf, xbuf);
				_mm_storeu_si128((__m128i *)pz, zbuf1);
				_mm_storeu_si128((__m128i *)pz + 1, zbuf);
				px += nstep;
				pz += nstep;
			}
		}

		for (i = 0; i < n3; i++) {
			*pz++ = -(mlib_s32)(*px++) + c0;
		}
	}

	return (MLIB_SUCCESS);
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_U8_Sat(
	mlib_u8 *xz,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_U8_U8_Sat(xz, xz, c, n));
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_U8_Mod(
	mlib_u8 *xz,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_U8_U8_Mod(xz, xz, c, n));
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_s32 nn = n;
	PARAMS(mlib_s8);

	c0 = *c;
	cbuf = _mm_set1_epi8(c0);

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = -(*px++) + c0;
			SAT_S8(sum);
			*pz++ = sum;
		}
	} else {
		for (i = 0; i < n1; i++) {
			sum = -(*px++) + c0;
			SAT_S8(sum);
			*pz++ = sum;
		}

		VECTOR_SUBS(_mm_subs_epi8);

		for (i = 0; i < n3; i++) {
			sum = -(*px++) + c0;
			SAT_S8(sum);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S8_S8_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_s32 nn = n;
	PARAMS(mlib_s8);

	c0 = *c;
	cbuf = _mm_set1_epi8(c0);

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = -(*px++) + c0;
			*pz++ = sum;
		}
	} else {
		for (i = 0; i < n1; i++) {
			sum = -(*px++) + c0;
			*pz++ = sum;
		}

		VECTOR_SUBS(_mm_sub_epi8);

		for (i = 0; i < n3; i++) {
			sum = -(*px++) + c0;
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S16_S8_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, ax, az, nstep, n1, n2, n3, c0;
	mlib_s8 *px = (mlib_s8 *)x;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, zbuf, xbuf1, zbuf1, cbuf, zero, xext;

	c0   = *c;
	cbuf = _mm_set1_epi16(c0);
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s8);
	n1 = ((16 - ((mlib_addr)x & 15)) & 15) / sizeof (mlib_s8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			*pz++ = -(mlib_s32)(*px++) + c0;
		}
	} else {
		for (i = 0; i < n1; i++) {
			*pz++ = -(mlib_s32)(*px++) + c0;
		}

		if ((ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				xbuf  = _mm_load_si128((__m128i *)px);
				xext  = _mm_cmpgt_epi8(zero, xbuf);
				xbuf1 = _mm_unpacklo_epi8(xbuf, xext);
				xbuf  = _mm_unpackhi_epi8(xbuf, xext);
				zbuf1 = _mm_sub_epi16(cbuf, xbuf1);
				zbuf  = _mm_sub_epi16(cbuf, xbuf);
				_mm_store_si128((__m128i *)pz, zbuf1);
				_mm_store_si128((__m128i *)pz + 1, zbuf);
				px += nstep;
				pz += nstep;
			}
		} else {
			for (i = 0; i < n2; i++) {
				xbuf  = _mm_load_si128((__m128i *)px);
				xext  = _mm_cmpgt_epi8(zero, xbuf);
				xbuf1 = _mm_unpacklo_epi8(xbuf, xext);
				xbuf  = _mm_unpackhi_epi8(xbuf, xext);
				zbuf1 = _mm_sub_epi16(cbuf, xbuf1);
				zbuf  = _mm_sub_epi16(cbuf, xbuf);
				_mm_storeu_si128((__m128i *)pz, zbuf1);
				_mm_storeu_si128((__m128i *)pz + 1, zbuf);
				px += nstep;
				pz += nstep;
			}
		}

		for (i = 0; i < n3; i++) {
			*pz++ = -(mlib_s32)(*px++) + c0;
		}
	}

	return (MLIB_SUCCESS);
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S8_Sat(
	mlib_s8 *xz,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S8_S8_Sat(xz, xz, c, n));
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S8_Mod(
	mlib_s8 *xz,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S8_S8_Mod(xz, xz, c, n));
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s32 nn = n;
	PARAMS(mlib_s16);

	c0 = *c;
	cbuf = _mm_set1_epi16(c0);

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = -(*px++) + c0;
			SAT_S16(sum);
			*pz++ = sum;
		}
	} else {
		for (i = 0; i < n1; i++) {
			sum = -(*px++) + c0;
			SAT_S16(sum);
			*pz++ = sum;
		}

		VECTOR_SUBS(_mm_subs_epi16);

		for (i = 0; i < n3; i++) {
			sum = -(*px++) + c0;
			SAT_S16(sum);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s32 nn = n;
	PARAMS(mlib_s16);

	c0 = *c;
	cbuf = _mm_set1_epi16(c0);

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = -(*px++) + c0;
			*pz++ = sum;
		}
	} else {
		for (i = 0; i < n1; i++) {
			sum = -(*px++) + c0;
			*pz++ = sum;
		}

		VECTOR_SUBS(_mm_sub_epi16);

		for (i = 0; i < n3; i++) {
			sum = -(*px++) + c0;
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S32_S16_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, ax, az, nstep, n1, n2, n3, c0;
	mlib_s16 *px = (mlib_s16 *)x;
	mlib_s32 *pz = (mlib_s32 *)z;
	__m128i xbuf, zbuf, xbuf1, zbuf1, cbuf, zero, xext;

	c0   = *c;
	cbuf = _mm_set1_epi32(c0);
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ((mlib_addr)x & 15)) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			*pz++ = -(mlib_s32)(*px++) + c0;
		}
	} else {
		for (i = 0; i < n1; i++) {
			*pz++ = -(mlib_s32)(*px++) + c0;
		}

		if ((ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				xbuf  = _mm_load_si128((__m128i *)px);
				xext  = _mm_cmpgt_epi16(zero, xbuf);
				xbuf1 = _mm_unpacklo_epi16(xbuf, xext);
				xbuf  = _mm_unpackhi_epi16(xbuf, xext);
				zbuf1 = _mm_sub_epi32(cbuf, xbuf1);
				zbuf  = _mm_sub_epi32(cbuf, xbuf);
				_mm_store_si128((__m128i *)pz, zbuf1);
				_mm_store_si128((__m128i *)pz + 1, zbuf);
				px += nstep;
				pz += nstep;
			}
		} else {
			for (i = 0; i < n2; i++) {
				xbuf  = _mm_load_si128((__m128i *)px);
				xext  = _mm_cmpgt_epi16(zero, xbuf);
				xbuf1 = _mm_unpacklo_epi16(xbuf, xext);
				xbuf  = _mm_unpackhi_epi16(xbuf, xext);
				zbuf1 = _mm_sub_epi32(cbuf, xbuf1);
				zbuf  = _mm_sub_epi32(cbuf, xbuf);
				_mm_storeu_si128((__m128i *)pz, zbuf1);
				_mm_storeu_si128((__m128i *)pz + 1, zbuf);
				px += nstep;
				pz += nstep;
			}
		}

		for (i = 0; i < n3; i++) {
			*pz++ = -(mlib_s32)(*px++) + c0;
		}
	}

	return (MLIB_SUCCESS);
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S16_Sat(
	mlib_s16 *xz,
	const mlib_s16 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S16_S16_Sat(xz, xz, c, n));
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S16_Mod(
	mlib_s16 *xz,
	const mlib_s16 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S16_S16_Mod(xz, xz, c, n));
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S32_S32_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	const mlib_s32 *px = x;
	mlib_s32 i, *pz = z;
	mlib_d64 sum, c0 = (mlib_d64)c[0];
	__m128i xbuf, ybuf, zbuf;
	__m128d buf1, buf2, buf3, dmin, dmax;
	dmin = _mm_set1_pd(MLIB_S32_MIN);
	dmax = _mm_set1_pd(MLIB_S32_MAX);
	buf2 = _mm_set1_pd((mlib_d64)c0);

	for (i = 0; i < n / 2; i++) {
		xbuf = _mm_loadl_epi64((__m128i *)px);

		buf1 = _mm_cvtepi32_pd(xbuf);
		buf3 = _mm_sub_pd(buf2, buf1);
		buf3 = _mm_max_pd(buf3, dmin);
		buf3 = _mm_min_pd(buf3, dmax);
		zbuf = _mm_cvtpd_epi32(buf3);

		_mm_storel_epi64((__m128i *)pz, zbuf);
		px += 2;
		pz += 2;
	}

	if (n & 1) {
		sum = c0 - (mlib_d64)(*px++);
		if (sum > MLIB_S32_MAX)
			*pz++ = MLIB_S32_MAX;
		else if (sum < MLIB_S32_MIN)
			*pz++ = MLIB_S32_MIN;
		else
			*pz++ = sum;
	}

	return (MLIB_SUCCESS);
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S32_S32_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n)
{
	mlib_s32 nn = n;
	PARAMS(mlib_s32);

	c0 = *c;
	cbuf = _mm_set1_epi32(c0);

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = -(*px++) + c0;
			*pz++ = sum;
		}
	} else {
		for (i = 0; i < n1; i++) {
			sum = -(*px++) + c0;
			*pz++ = sum;
		}

		VECTOR_SUBS(_mm_sub_epi32);

		for (i = 0; i < n3; i++) {
			sum = -(*px++) + c0;
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S32_Sat(
	mlib_s32 *xz,
	const mlib_s32 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S32_S32_Sat(xz, xz, c, n));
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S32_Mod(
	mlib_s32 *xz,
	const mlib_s32 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S32_S32_Mod(xz, xz, c, n));
}

/* ******************************************************************* */

#define	SUBSC_SAT(sat)			\
	sum = -(*px++) + c0;		\
	sat(sum);			\
	*pz++ = sum;			\
	sum = -(*px++) + c1;		\
	sat(sum);			\
	*pz++ = sum

#define	SUBSC_MOD()			\
	sum = -(*px++) + c0;		\
	*pz++ = sum;			\
	sum = -(*px++) + c1;		\
	*pz++ = sum

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_U8C_U8C_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_s32 nn = n + n;
	PARAMS(mlib_u8);

	c0 = c[0];
	c1 = c[1];
	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			SUBSC_SAT(SAT_U8);
		}
	} else {
		if (n1 & 1) {
			sum = -(*px++) + c0;
			SAT_U8(sum);
			*pz++ = sum;

			c0 ^= c1;
			c1 ^= c0;
			c0 ^= c1;
		}

		for (i = 0; i < n1 / 2; i++) {
			SUBSC_SAT(SAT_U8);
		}

		cbuf = _mm_set1_epi16(c0 & 0xff | c1 << 8);
		VECTOR_SUBS(_mm_subs_epu8);

		for (i = 0; i < n3 / 2; i++) {
			SUBSC_SAT(SAT_U8);
		}
		if (n3 & 1) {
			sum = -(*px++) + c0;
			SAT_U8(sum);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_s32 nn = n + n;
	PARAMS(mlib_u8);

	c0 = c[0];
	c1 = c[1];
	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			SUBSC_MOD();
		}
	} else {
		if (n1 & 1) {
			sum = -(*px++) + c0;
			*pz++ = sum;

			c0 ^= c1;
			c1 ^= c0;
			c0 ^= c1;
		}

		for (i = 0; i < n1 / 2; i++) {
			SUBSC_MOD();
		}

		cbuf = _mm_set1_epi16(c0 & 0xff | c1 << 8);
		VECTOR_SUBS(_mm_sub_epi8);

		for (i = 0; i < n3 / 2; i++) {
			SUBSC_MOD();
		}
		if (n3 & 1) {
			sum = -(*px++) + c0;
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_U8C_Sat(
	mlib_u8 *xz,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_U8C_U8C_Sat(xz, xz, c, n));
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S16C_U8C_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, ax, az, nstep, n1, n2, n3, c0, c1;
	mlib_u8 *px = (mlib_u8 *)x;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, zbuf, xbuf1, zbuf1, cbuf, zero;

	c0   = c[0];
	c1   = c[1];
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ((mlib_addr)x & 15)) & 15) / sizeof (mlib_u8);
	n2 = (n + n - n1) / nstep;
	n3 = n + n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			*pz++ = -(mlib_s32)(*px++) + c0;
			*pz++ = -(mlib_s32)(*px++) + c1;
		}
	} else {
		if (n1 & 1) {
			*pz++ = -(mlib_s32)(*px++) + c0;
			c0 ^= c1;
			c1 ^= c0;
			c0 ^= c1;
		}
		for (i = 0; i < n1 / 2; i++) {
			*pz++ = -(mlib_s32)(*px++) + c0;
			*pz++ = -(mlib_s32)(*px++) + c1;
		}

		cbuf = _mm_set1_epi32((c0 & 0xffff) | (c1 << 16));
		if ((ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				xbuf  = _mm_load_si128((__m128i *)px);
				xbuf1 = _mm_unpacklo_epi8(xbuf, zero);
				xbuf  = _mm_unpackhi_epi8(xbuf, zero);
				zbuf1 = _mm_sub_epi16(cbuf, xbuf1);
				zbuf  = _mm_sub_epi16(cbuf, xbuf);
				_mm_store_si128((__m128i *)pz, zbuf1);
				_mm_store_si128((__m128i *)pz + 1, zbuf);
				px += nstep;
				pz += nstep;
			}
		} else {
			for (i = 0; i < n2; i++) {
				xbuf  = _mm_load_si128((__m128i *)px);
				xbuf1 = _mm_unpacklo_epi8(xbuf, zero);
				xbuf  = _mm_unpackhi_epi8(xbuf, zero);
				zbuf1 = _mm_sub_epi16(cbuf, xbuf1);
				zbuf  = _mm_sub_epi16(cbuf, xbuf);
				_mm_storeu_si128((__m128i *)pz, zbuf1);
				_mm_storeu_si128((__m128i *)pz + 1, zbuf);
				px += nstep;
				pz += nstep;
			}
		}

		for (i = 0; i < n3 / 2; i++) {
			*pz++ = -(mlib_s32)(*px++) + c0;
			*pz++ = -(mlib_s32)(*px++) + c1;
		}
		if (n3 & 1) {
			*pz++ = -(mlib_s32)(*px++) + c0;
		}
	}
	return (MLIB_SUCCESS);
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_U8C_Mod(
	mlib_u8 *xz,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_U8C_U8C_Mod(xz, xz, c, n));
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_s32 nn = n + n;
	PARAMS(mlib_s8);

	c0 = c[0];
	c1 = c[1];
	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			SUBSC_SAT(SAT_S8);
		}
	} else {
		if (n1 & 1) {
			sum = -(*px++) + c0;
			SAT_S8(sum);
			*pz++ = sum;

			c0 ^= c1;
			c1 ^= c0;
			c0 ^= c1;
		}

		for (i = 0; i < n1 / 2; i++) {
			SUBSC_SAT(SAT_S8);
		}

		cbuf = _mm_set1_epi16(c0 & 0xff | c1 << 8);
		VECTOR_SUBS(_mm_subs_epi8);

		for (i = 0; i < n3 / 2; i++) {
			SUBSC_SAT(SAT_S8);
		}
		if (n3 & 1) {
			sum = -(*px++) + c0;
			SAT_S8(sum);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S8C_S8C_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_s32 nn = n + n;
	PARAMS(mlib_s8);

	c0 = c[0];
	c1 = c[1];
	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			SUBSC_MOD();
		}
	} else {
		if (n1 & 1) {
			sum = -(*px++) + c0;
			*pz++ = sum;

			c0 ^= c1;
			c1 ^= c0;
			c0 ^= c1;
		}

		for (i = 0; i < n1 / 2; i++) {
			SUBSC_MOD();
		}

		cbuf = _mm_set1_epi16(c0 & 0xff | c1 << 8);
		VECTOR_SUBS(_mm_sub_epi8);

		for (i = 0; i < n3 / 2; i++) {
			SUBSC_MOD();
		}
		if (n3 & 1) {
			sum = -(*px++) + c0;
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S16C_S8C_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, ax, az, nstep, n1, n2, n3, c0, c1;
	mlib_s8 *px = (mlib_s8 *)x;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, zbuf, xbuf1, zbuf1, cbuf, zero, xext;

	c0   = c[0];
	c1   = c[1];
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s8);
	n1 = ((16 - ((mlib_addr)x & 15)) & 15) / sizeof (mlib_s8);
	n2 = (n + n - n1) / nstep;
	n3 = n + n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			*pz++ = -(mlib_s32)(*px++) + c0;
			*pz++ = -(mlib_s32)(*px++) + c1;
		}
	} else {
		if (n1 & 1) {
			*pz++ = -(mlib_s32)(*px++) + c0;
			c0 ^= c1;
			c1 ^= c0;
			c0 ^= c1;
		}
		for (i = 0; i < n1 / 2; i++) {
			*pz++ = -(mlib_s32)(*px++) + c0;
			*pz++ = -(mlib_s32)(*px++) + c1;
		}

		cbuf = _mm_set1_epi32((c0 & 0xffff) | (c1 << 16));
		if ((ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				xbuf  = _mm_load_si128((__m128i *)px);
				xext  = _mm_cmpgt_epi8(zero, xbuf);
				xbuf1 = _mm_unpacklo_epi8(xbuf, xext);
				xbuf  = _mm_unpackhi_epi8(xbuf, xext);
				zbuf1 = _mm_sub_epi16(cbuf, xbuf1);
				zbuf  = _mm_sub_epi16(cbuf, xbuf);
				_mm_store_si128((__m128i *)pz, zbuf1);
				_mm_store_si128((__m128i *)pz + 1, zbuf);
				px += nstep;
				pz += nstep;
			}
		} else {
			for (i = 0; i < n2; i++) {
				xbuf  = _mm_load_si128((__m128i *)px);
				xext  = _mm_cmpgt_epi8(zero, xbuf);
				xbuf1 = _mm_unpacklo_epi8(xbuf, xext);
				xbuf  = _mm_unpackhi_epi8(xbuf, xext);
				zbuf1 = _mm_sub_epi16(cbuf, xbuf1);
				zbuf  = _mm_sub_epi16(cbuf, xbuf);
				_mm_storeu_si128((__m128i *)pz, zbuf1);
				_mm_storeu_si128((__m128i *)pz + 1, zbuf);
				px += nstep;
				pz += nstep;
			}
		}

		for (i = 0; i < n3 / 2; i++) {
			*pz++ = -(mlib_s32)(*px++) + c0;
			*pz++ = -(mlib_s32)(*px++) + c1;
		}
		if (n3 & 1) {
			*pz++ = -(mlib_s32)(*px++) + c0;
		}
	}
	return (MLIB_SUCCESS);
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S8C_Sat(
	mlib_s8 *xz,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S8C_S8C_Sat(xz, xz, c, n));
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S8C_Mod(
	mlib_s8 *xz,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S8C_S8C_Mod(xz, xz, c, n));
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s32 nn = n + n;
	PARAMS(mlib_s16);

	c0 = c[0];
	c1 = c[1];
	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			SUBSC_SAT(SAT_S16);
		}
	} else {
		if (n1 & 1) {
			sum = -(*px++) + c0;
			SAT_S16(sum);
			*pz++ = sum;

			c0 ^= c1;
			c1 ^= c0;
			c0 ^= c1;
		}

		for (i = 0; i < n1 / 2; i++) {
			SUBSC_SAT(SAT_S16);
		}

		cbuf = _mm_set1_epi32(c0 & 0xffff | c1 << 16);
		VECTOR_SUBS(_mm_subs_epi16);

		for (i = 0; i < n3 / 2; i++) {
			SUBSC_SAT(SAT_S16);
		}
		if (n3 & 1) {
			sum = -(*px++) + c0;
			SAT_S16(sum);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s32 nn = n + n;
	PARAMS(mlib_s16);

	c0 = c[0];
	c1 = c[1];
	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			SUBSC_MOD();
		}
	} else {
		if (n1 & 1) {
			sum = -(*px++) + c0;
			*pz++ = sum;

			c0 ^= c1;
			c1 ^= c0;
			c0 ^= c1;
		}

		for (i = 0; i < n1 / 2; i++) {
			SUBSC_MOD();
		}

		cbuf = _mm_set1_epi32(c0 & 0xffff | c1 << 16);
		VECTOR_SUBS(_mm_sub_epi16);

		for (i = 0; i < n3 / 2; i++) {
			SUBSC_MOD();
		}
		if (n3 & 1) {
			sum = -(*px++) + c0;
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, ax, az, nstep, n1, n2, n3, c0, c1;
	mlib_s16 *px = (mlib_s16 *)x;
	mlib_s32 *pz = (mlib_s32 *)z;
	__m128i xbuf, zbuf, xbuf1, zbuf1, cbuf, zero, xext;

	c0   = c[0];
	c1   = c[1];
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ((mlib_addr)x & 15)) & 15) / sizeof (mlib_s16);
	n2 = (n + n - n1) / nstep;
	n3 = n + n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			*pz++ = -(mlib_s32)(*px++) + c0;
			*pz++ = -(mlib_s32)(*px++) + c1;
		}
	} else {
		if (n1 & 1) {
			*pz++ = -(mlib_s32)(*px++) + c0;
			c0 ^= c1;
			c1 ^= c0;
			c0 ^= c1;
		}
		for (i = 0; i < n1 / 2; i++) {
			*pz++ = -(mlib_s32)(*px++) + c0;
			*pz++ = -(mlib_s32)(*px++) + c1;
		}

		cbuf = _mm_setr_epi32(c0, c1, c0, c1);
		if ((ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				xbuf  = _mm_load_si128((__m128i *)px);
				xext  = _mm_cmpgt_epi16(zero, xbuf);
				xbuf1 = _mm_unpacklo_epi16(xbuf, xext);
				xbuf  = _mm_unpackhi_epi16(xbuf, xext);
				zbuf1 = _mm_sub_epi32(cbuf, xbuf1);
				zbuf  = _mm_sub_epi32(cbuf, xbuf);
				_mm_store_si128((__m128i *)pz, zbuf1);
				_mm_store_si128((__m128i *)pz + 1, zbuf);
				px += nstep;
				pz += nstep;
			}
		} else {
			for (i = 0; i < n2; i++) {
				xbuf  = _mm_load_si128((__m128i *)px);
				xext  = _mm_cmpgt_epi16(zero, xbuf);
				xbuf1 = _mm_unpacklo_epi16(xbuf, xext);
				xbuf  = _mm_unpackhi_epi16(xbuf, xext);
				zbuf1 = _mm_sub_epi32(cbuf, xbuf1);
				zbuf  = _mm_sub_epi32(cbuf, xbuf);
				_mm_storeu_si128((__m128i *)pz, zbuf1);
				_mm_storeu_si128((__m128i *)pz + 1, zbuf);
				px += nstep;
				pz += nstep;
			}
		}

		for (i = 0; i < n3 / 2; i++) {
			*pz++ = -(mlib_s32)(*px++) + c0;
			*pz++ = -(mlib_s32)(*px++) + c1;
		}
		if (n3 & 1) {
			*pz++ = -(mlib_s32)(*px++) + c0;
		}
	}
	return (MLIB_SUCCESS);
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S16C_Sat(
	mlib_s16 *xz,
	const mlib_s16 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S16C_S16C_Sat(xz, xz, c, n));
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S16C_Mod(
	mlib_s16 *xz,
	const mlib_s16 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S16C_S16C_Mod(xz, xz, c, n));
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S32C_S32C_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	const mlib_s32 *px = x;
	mlib_s32 i, *pz = z;
	mlib_d64 c0 = (mlib_d64)c[0], c1 = (mlib_d64)c[1];
	__m128i xbuf, zbuf;
	__m128d buf1, buf2, buf3, dmin, dmax;
	dmin = _mm_set1_pd(MLIB_S32_MIN);
	dmax = _mm_set1_pd(MLIB_S32_MAX);
	buf2 = _mm_setr_pd(c0, c1);

	for (i = 0; i < n; i++) {
		xbuf = _mm_loadl_epi64((__m128i *)px);

		buf1 = _mm_cvtepi32_pd(xbuf);
		buf3 = _mm_sub_pd(buf2, buf1);
		buf3 = _mm_max_pd(buf3, dmin);
		buf3 = _mm_min_pd(buf3, dmax);
		zbuf = _mm_cvtpd_epi32(buf3);

		_mm_storel_epi64((__m128i *)pz, zbuf);
		px += 2;
		pz += 2;
	}

	return (MLIB_SUCCESS);
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S32C_S32C_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *c,
	mlib_s32 n)
{
	mlib_s32 nn = n + n;
	PARAMS(mlib_s32);

	c0 = c[0];
	c1 = c[1];
	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			SUBSC_MOD();
		}
	} else {
		if (n1 & 1) {
			sum = -(*px++) + c0;
			*pz++ = sum;

			c0 ^= c1;
			c1 ^= c0;
			c0 ^= c1;
		}

		for (i = 0; i < n1 / 2; i++) {
			SUBSC_MOD();
		}

		cbuf = _mm_setr_epi32(c0, c1, c0, c1);
		VECTOR_SUBS(_mm_sub_epi32);

		for (i = 0; i < n3 / 2; i++) {
			SUBSC_MOD();
		}
		if (n3 & 1) {
			sum = -(*px++) + c0;
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S32C_Sat(
	mlib_s32 *xz,
	const mlib_s32 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S32C_S32C_Sat(xz, xz, c, n));
}

/* ******************************************************************* */

mlib_status
__mlib_VectorSubS_S32C_Mod(
	mlib_s32 *xz,
	const mlib_s32 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSubS_S32C_S32C_Mod(xz, xz, c, n));
}

/* ******************************************************************* */
