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

#pragma ident	"@(#)mlib_s_VectorCopy.c	9.5	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorCopy_U8   - copy of 8-bit format vector
 *      mlib_VectorCopy_S8   - copy of 8-bit format vector
 *      mlib_VectorCopy_S16  - copy of 16-bit format vector
 *      mlib_VectorCopy_S32  - copy of 32-bit format vector
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorCopy_U8(mlib_u8      *z,
 *                                    const mlib_u8 *x,
 *                                    mlib_s32      n);
 *      mlib_status mlib_VectorCopy_S8(mlib_s8      *z,
 *                                    const mlib_s8 *x,
 *                                    mlib_s32      n);
 *      mlib_status mlib_VectorCopy_S16(mlib_s16       *z,
 *                                      const mlib_s16 *x,
 *                                      mlib_s32       n);
 *      mlib_status mlib_VectorCopy_S32(mlib_s32       *z,
 *                                      const mlib_s32 *x,
 *                                      mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      x    pointer to the first element of the input vector
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *      z[i] = x[i]
 *
 */

/*
 * FUNCTIONS
 *      mlib_VectorCopy_U8C  - copy of 8-bit format complex vector
 *      mlib_VectorCopy_S8C  - copy of 8-bit format complex vector
 *      mlib_VectorCopy_S16C - copy of 16-bit format complex vector
 *      mlib_VectorCopy_S32C - copy of 32-bit format complex vector
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorCopy_U8C(mlib_u8       *z,
 *                                      const mlib_u8 *x,
 *                                      mlib_s32      n);
 *      mlib_status mlib_VectorCopy_S8C(mlib_s8       *z,
 *                                      const mlib_s8 *x,
 *                                      mlib_s32      n);
 *      mlib_status mlib_VectorCopy_S16C(mlib_s16      *z,
 *                                      const mlib_s16 *x,
 *                                      mlib_s32       n);
 *      mlib_status mlib_VectorCopy_S32C(mlib_s32      *z,
 *                                      const mlib_s32 *x,
 *                                      mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result vector.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts
 *      x    pointer to the first complex element of the input vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      n    number of complex elements in the vectors
 *
 * DESCRIPTION
 *      z[i] = x[i]
 *
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

#pragma weak mlib_VectorCopy_U8C = __mlib_VectorCopy_U8C
#pragma weak mlib_VectorCopy_S32 = __mlib_VectorCopy_S32
#pragma weak mlib_VectorCopy_S16 = __mlib_VectorCopy_S16
#pragma weak mlib_VectorCopy_S32C = __mlib_VectorCopy_S32C
#pragma weak mlib_VectorCopy_S8 = __mlib_VectorCopy_S8
#pragma weak mlib_VectorCopy_S16C = __mlib_VectorCopy_S16C
#pragma weak mlib_VectorCopy_U8 = __mlib_VectorCopy_U8
#pragma weak mlib_VectorCopy_S8C = __mlib_VectorCopy_S8C

#elif defined(__GNUC__)

__typeof__(__mlib_VectorCopy_U8C) mlib_VectorCopy_U8C
	__attribute__((weak, alias("__mlib_VectorCopy_U8C")));
__typeof__(__mlib_VectorCopy_S32) mlib_VectorCopy_S32
	__attribute__((weak, alias("__mlib_VectorCopy_S32")));
__typeof__(__mlib_VectorCopy_S16) mlib_VectorCopy_S16
	__attribute__((weak, alias("__mlib_VectorCopy_S16")));
__typeof__(__mlib_VectorCopy_S32C) mlib_VectorCopy_S32C
	__attribute__((weak, alias("__mlib_VectorCopy_S32C")));
__typeof__(__mlib_VectorCopy_S8) mlib_VectorCopy_S8
	__attribute__((weak, alias("__mlib_VectorCopy_S8")));
__typeof__(__mlib_VectorCopy_S16C) mlib_VectorCopy_S16C
	__attribute__((weak, alias("__mlib_VectorCopy_S16C")));
__typeof__(__mlib_VectorCopy_U8) mlib_VectorCopy_U8
	__attribute__((weak, alias("__mlib_VectorCopy_U8")));
__typeof__(__mlib_VectorCopy_S8C) mlib_VectorCopy_S8C
	__attribute__((weak, alias("__mlib_VectorCopy_S8C")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	COPY_VECTOR(TYPE)						\
	mlib_s32 i, nstep, n1, n2, n3, align;				\
	TYPE *pdst = z;							\
	const TYPE *psrc = x;						\
	__m128i buf, buf2;						\
									\
	if (n < 1)							\
		return (MLIB_FAILURE);					\
									\
	nstep = 16 / sizeof (TYPE);					\
	n1 = ((16 - ((mlib_addr)x & 15)) & 15) / sizeof (TYPE);		\
	n2 = (n - n1) / nstep;						\
	n3 = n - n1 - n2 * nstep;					\
	align = ((mlib_addr)x & 15) - ((mlib_addr)z & 15);		\
									\
	if (n2 < 1) { 							\
		for (i = 0; i < n; i++)					\
			*pdst++ = *psrc++;				\
	} else {							\
		for (i = 0; i < n1; i++)				\
			*pdst++ = *psrc++;				\
		if (align == 0)	{					\
		    for (i = 0; i < n2; i++) {				\
			buf = _mm_load_si128((__m128i *)psrc);		\
			_mm_store_si128((__m128i *)pdst, buf);		\
			psrc += nstep;					\
			pdst += nstep;					\
		    }							\
		} else {						\
		    for (i = 0; i < n2 / 2; i++) {			\
			buf = _mm_load_si128((__m128i *)psrc);		\
			buf2 = _mm_load_si128((__m128i *)(psrc + nstep));\
			_mm_storeu_si128((__m128i *)pdst, buf);		\
			_mm_storeu_si128((__m128i *)(pdst + nstep), buf2);\
			psrc += 2 * nstep;				\
			pdst += 2 * nstep;				\
		    }							\
		    if (n2 & 1) {					\
			buf = _mm_load_si128((__m128i *)psrc);		\
			_mm_storeu_si128((__m128i *)pdst, buf);		\
			psrc += nstep;					\
			pdst += nstep;					\
		    }							\
		}							\
		for (i = 0; i < n3; i++)				\
			*pdst++ = *psrc++;				\
	}								\
									\
	return (MLIB_SUCCESS)

/* ****************************************************** */

mlib_status
__mlib_VectorCopy_U8(
	mlib_u8 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	COPY_VECTOR(mlib_u8);
}

/* *********************************************************** */

mlib_status
__mlib_VectorCopy_S8(
	mlib_s8 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	COPY_VECTOR(mlib_s8);
}

/* *********************************************************** */

mlib_status
__mlib_VectorCopy_S16(
	mlib_s16 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	COPY_VECTOR(mlib_s16);
}

/* *********************************************************** */

mlib_status
__mlib_VectorCopy_S32(
	mlib_s32 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	COPY_VECTOR(mlib_s32);
}

/* *********************************************************** */

mlib_status
__mlib_VectorCopy_U8C(
	mlib_u8 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	return (__mlib_VectorCopy_U8(z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorCopy_S8C(
	mlib_s8 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	return (__mlib_VectorCopy_U8((mlib_u8 *)z, (mlib_u8 *)x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorCopy_S16C(
	mlib_s16 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	return (__mlib_VectorCopy_S16(z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorCopy_S32C(
	mlib_s32 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	return (__mlib_VectorCopy_S32(z, x, n + n));
}

/* *********************************************************** */
