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

#pragma ident	"@(#)mlib_s_VectorSet.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorSet_U8  - set of 8-bit format vector to a scalar
 *      mlib_VectorSet_S8  - set of 8-bit format vector to a scalar
 *      mlib_VectorSet_S16 - set of 16-bit format vector to a scalar
 *      mlib_VectorSet_S32 - set of 32-bit format vector to a scalar
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorSet_S16(mlib_s16       *z,
 *                                     const mlib_s16 *c,
 *                                     mlib_s32       n);
 *      mlib_status mlib_VectorSet_S32(mlib_s32       *z,
 *                                     const mlib_s32 *c,
 *                                     mlib_s32       n);
 *      mlib_status mlib_VectorSet_S8(mlib_s8       *z,
 *                                    const mlib_s8 *c,
 *                                    mlib_s32      n);
 *      mlib_status mlib_VectorSet_U8(mlib_u8       *z,
 *                                    const mlib_u8 *c,
 *                                    mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector.
 *      c    a scalar.
 *      n    number of elements in the vector.
 *
 * DESCRIPTION
 *      z[i] = c
 *
 */

/*
 * FUNCTIONS
 *      mlib_VectorSet_U8C  - set of 8-bit format complex vector to a scalar
 *      mlib_VectorSet_S8C  - set of 8-bit format complex vector to a scalar
 *      mlib_VectorSet_S16C - set of 16-bit format complex vector to a scalar
 *      mlib_VectorSet_S32C - set of 32-bit format complex vector to a scalar
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorSet_S16C(mlib_s16       *z,
 *                                      const mlib_s16 *c,
 *                                      mlib_s32       n);
 *      mlib_status mlib_VectorSet_S32C(mlib_s32       *z,
 *                                      const mlib_s32 *c,
 *                                      mlib_s32       n);
 *      mlib_status mlib_VectorSet_S8C(mlib_s8       *z,
 *                                     const mlib_s8 *c,
 *                                     mlib_s32      n);
 *      mlib_status mlib_VectorSet_U8C(mlib_u8       *z,
 *                                     const mlib_u8 *c,
 *                                     mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result vector.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts.
 *      c    pointer to a complex scalar. c[0] holds the real part,
 *           and c[1] holds the imaginary part.
 *      n    number of complex elements in the vector.
 *
 * DESCRIPTION
 *      z[i] = c
 *
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

#pragma weak mlib_VectorSet_U8C = __mlib_VectorSet_U8C
#pragma weak mlib_VectorSet_S32 = __mlib_VectorSet_S32
#pragma weak mlib_VectorSet_S16 = __mlib_VectorSet_S16
#pragma weak mlib_VectorSet_S32C = __mlib_VectorSet_S32C
#pragma weak mlib_VectorSet_S8 = __mlib_VectorSet_S8
#pragma weak mlib_VectorSet_S16C = __mlib_VectorSet_S16C
#pragma weak mlib_VectorSet_U8 = __mlib_VectorSet_U8
#pragma weak mlib_VectorSet_S8C = __mlib_VectorSet_S8C

#elif defined(__GNUC__)

__typeof__(__mlib_VectorSet_U8C) mlib_VectorSet_U8C
	__attribute__((weak, alias("__mlib_VectorSet_U8C")));
__typeof__(__mlib_VectorSet_S32) mlib_VectorSet_S32
	__attribute__((weak, alias("__mlib_VectorSet_S32")));
__typeof__(__mlib_VectorSet_S16) mlib_VectorSet_S16
	__attribute__((weak, alias("__mlib_VectorSet_S16")));
__typeof__(__mlib_VectorSet_S32C) mlib_VectorSet_S32C
	__attribute__((weak, alias("__mlib_VectorSet_S32C")));
__typeof__(__mlib_VectorSet_S8) mlib_VectorSet_S8
	__attribute__((weak, alias("__mlib_VectorSet_S8")));
__typeof__(__mlib_VectorSet_S16C) mlib_VectorSet_S16C
	__attribute__((weak, alias("__mlib_VectorSet_S16C")));
__typeof__(__mlib_VectorSet_U8) mlib_VectorSet_U8
	__attribute__((weak, alias("__mlib_VectorSet_U8")));
__typeof__(__mlib_VectorSet_S8C) mlib_VectorSet_S8C
	__attribute__((weak, alias("__mlib_VectorSet_S8C")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	F_8	6
#define	F_16	5
#define	F_32	4

/* *********************************************************** */

/*
 * do not perform the copying by mlib_d64 data type for x86
 */
#ifdef i386

typedef struct
{
	mlib_s32 int0, int1;
} two_int;

#define	TYPE_64BIT	two_int

#else /* i386 */

#define	TYPE_64BIT	mlib_d64

#endif /* i386 */

/* *********************************************************** */

#define	SET_BY_LOOP(vec, lim, c)                                \
	{                                                       \
	    for (i = 0; i < lim; i++)                           \
		vec[i] = c;                                     \
	    return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);     \
	}

/* *********************************************************** */

#define	SET_BY_LOOP_C(vec, lim)                                 \
	{                                                       \
	    for (i = 0; i < lim; i += 2) {                      \
		vec[i] = c_r;                                   \
		vec[i + 1] = c_i;                               \
	    }                                                   \
	    return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);     \
	}

/* *********************************************************** */

#define	SET_LARGE(format)                                        \
	{                                                        \
	    type_union_mlib_d64 what2;                           \
	                                                         \
	    while ((mlib_addr)dst & 7) {                         \
		(*dst++) = cc;                                   \
		n--;                                             \
	    }                                                    \
	    rest_64 = n & ((1 << (format - 3)) - 1);             \
	    length_64 = n >> (format - 3);                       \
	    n = length_64 << (format - 3);                       \
	    what2.two_int.int0 = what2.two_int.int1 = what;      \
	    mlib_c_set_64((mlib_d64 *)dst, (mlib_d64 *)&what2,   \
		    length_64);                                  \
	    for (i = 0; i < rest_64; i++)                        \
		dst[n + i] = cc;                                 \
	    return (MLIB_SUCCESS);                               \
	}

/* *********************************************************** */

#ifndef _LITTLE_ENDIAN

#define	SET_LARGE_C(format)                                      \
	{                                                        \
	    type_union_mlib_d64 what2;                           \
	                                                         \
	    while ((mlib_addr)dst & 7) {                         \
		(*dst++) = (i & 1) ? c_i : c_r;                  \
		i++;                                             \
	    }                                                    \
	    n -= i;                                              \
	    rest_64 = n & ((1 << (format - 3)) - 1);             \
	    length_64 = n >> (format - 3);                       \
	    n = length_64 << (format - 3);                       \
	    what2.two_int.int0 = what2.two_int.int1 =            \
		    (align8 & 1) ? what_1 : what_0;              \
	    mlib_c_set_64((mlib_d64 *)dst, (mlib_d64 *)&what2,   \
		    length_64);                                  \
	    for (i = 0; i < rest_64; i++) {                      \
		dst[n + i] = ((align8 ^ i) & 1) ? c_i : c_r;     \
	    }                                                    \
	    return (MLIB_SUCCESS);                               \
	}

#else

#define	SET_LARGE_C(format)                                      \
	{                                                        \
	    type_union_mlib_d64 what2;                           \
	                                                         \
	    while ((mlib_addr)dst & 7) {                         \
		(*dst++) = (i & 1) ? c_i : c_r;                  \
		i++;                                             \
	    }                                                    \
	    n -= i;                                              \
	    rest_64 = n & ((1 << (format - 3)) - 1);             \
	    length_64 = n >> (format - 3);                       \
	    n = length_64 << (format - 3);                       \
	    what2.two_int.int0 = what2.two_int.int1 =            \
		    (align8 & 1) ? what_0 : what_1;              \
	    mlib_c_set_64((mlib_d64 *)dst, (mlib_d64 *)&what2,   \
		    length_64);                                  \
	    for (i = 0; i < rest_64; i++) {                      \
		dst[n + i] = ((align8 ^ i) & 1) ? c_i : c_r;     \
	    }                                                    \
	    return (MLIB_SUCCESS);                               \
	}

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

static void
mlib_c_set_64(
	mlib_d64 *dst,
	const mlib_d64 *src,
	mlib_s32 n)
{
	mlib_s32 i;
	TYPE_64BIT what22 = ((TYPE_64BIT *) src)[0];

	if (i = n & 1)
		((TYPE_64BIT *) dst)[0] = what22;
	for (; i < n; i += 2) {
		((TYPE_64BIT *) dst)[i] = what22;
		((TYPE_64BIT *) dst)[i + 1] = what22;
	}
}

/* ****************************************************** */

#define	SET_VALUE(TYPE)							\
	if (n < 1)							\
		return (MLIB_FAILURE);					\
									\
	mlib_s32 i, nstep, n1, n2, n3;					\
	TYPE *pdst = z;							\
									\
	nstep = 16 / sizeof (TYPE);					\
	n1 = ((16 - ((mlib_addr)z & 15)) & 15) / sizeof (TYPE);		\
	n2 = (n - n1) / nstep;						\
	n3 = n - n1 - n2 * nstep;					\
									\
	if (n2 < 1) { 							\
		for (i = 0; i < n; i++)					\
			*pdst++ = c0;					\
	} else {							\
		for (i = 0; i < n1; i++)				\
			*pdst++ = c0;					\
		for (i = 0; i < n2; i++) {				\
			_mm_store_si128((__m128i *)pdst, val);		\
			pdst += nstep;					\
		}							\
		for (i = 0; i < n3; i++)				\
			*pdst++ = c0;					\
	}								\
									\
	return (MLIB_SUCCESS)
/* *********************************************************** */

mlib_status
__mlib_VectorSet_U8(
	mlib_u8 *z,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_u8 c0 = *c;
	__m128i val = _mm_set1_epi8(c0);
	SET_VALUE(mlib_u8);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSet_S8(
	mlib_s8 *z,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_s8 c0 = *c;
	__m128i val = _mm_set1_epi8(c0);
	SET_VALUE(mlib_s8);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSet_S16(
	mlib_s16 *z,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s16 c0 = *c;
	__m128i val = _mm_set1_epi16(c0);
	SET_VALUE(mlib_s16);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSet_S32(
	mlib_s32 *z,
	const mlib_s32 *c,
	mlib_s32 n)
{
	mlib_s32 c0 = *c;
	__m128i val = _mm_set1_epi32(c0);
	SET_VALUE(mlib_s32);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSet_U8C(
	mlib_u8 *z,
	const mlib_u8 *c,
	mlib_s32 n)
{
	if ((mlib_addr)z & 1) {
		mlib_s32 i, nstep, n1, n2, n3;
		mlib_u8 c0, c1, *pdst = z;
		__m128i val;
		c0 = c[0];
		c1 = c[1];

		nstep = 16 / sizeof (mlib_u8);
		n1 = ((16 - ((mlib_addr)z & 15)) & 15) / sizeof (mlib_u8);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				*pdst++ = c0;
				*pdst++ = c1;
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				*pdst++ = c0;
				*pdst++ = c1;
			}
			if (n1 & 1) {
				*pdst++ = c0;
				val = _mm_set1_epi16(((mlib_u16)c1 & 0xff) |
						((mlib_u16)c0 << 8));
			} else {
				val = _mm_set1_epi16(((mlib_u16)c0 & 0xff) |
						((mlib_s16)c1 << 8));
			}

			for (i = 0; i < n2; i++) {
				_mm_store_si128((__m128i *)pdst, val);
				pdst += nstep;
			}

			if (n1 & 1) {
				*pdst++ = c1;
				n3--;
			}
			for (i = 0; i < n3 / 2; i++) {
				*pdst++ = c0;
				*pdst++ = c1;
			}
			if (n3 & 1) {
				*pdst++ = c0;
			}
		}

		return (MLIB_SUCCESS);
	} else {
		return (__mlib_VectorSet_S16((mlib_s16 *)z, (mlib_s16 *)c, n));
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorSet_S8C(
	mlib_s8 *z,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSet_U8C((mlib_u8 *)z, (mlib_u8 *)c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSet_S16C(
	mlib_s16 *z,
	const mlib_s16 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	if ((mlib_addr)z & 3) {

		mlib_s32 i, nstep, n1, n2, n3;
		mlib_s16 c0, c1, *pdst = z;
		__m128i val;
		c0 = c[0];
		c1 = c[1];

		nstep = 16 / sizeof (mlib_s16);
		n1 = ((16 - ((mlib_addr)z & 15)) & 15) / sizeof (mlib_s16);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				*pdst++ = c0;
				*pdst++ = c1;
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				*pdst++ = c0;
				*pdst++ = c1;
			}
			if (n1 & 1) {
				*pdst++ = c0;
				val = _mm_set1_epi32(((mlib_u32)c1 & 0xffff) |
						((mlib_u32)c0 << 16));
			} else {
				val = _mm_set1_epi32(((mlib_u32)c0 & 0xffff) |
						((mlib_u32)c1 << 16));
			}

			for (i = 0; i < n2; i++) {
				_mm_store_si128((__m128i *)pdst, val);
				pdst += nstep;
			}

			if (n1 & 1) {
				*pdst++ = c1;
				n3--;
			}
			for (i = 0; i < n3 / 2; i++) {
				*pdst++ = c0;
				*pdst++ = c1;
			}
			if (n3 & 1) {
				*pdst++ = c0;
			}
		}

		return (MLIB_SUCCESS);

	} else {
		return (__mlib_VectorSet_S32((mlib_s32*)z, (mlib_s32*)c, n));
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorSet_S32C(
	mlib_s32 *z,
	const mlib_s32 *c,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, nstep, n1, n2, n3;
	mlib_s32 c0, c1, *pdst = z;
	__m128i val;
	c0 = c[0];
	c1 = c[1];

	nstep = 16 / sizeof (mlib_s32);
	n1 = ((16 - ((mlib_addr)z & 15)) & 15) / sizeof (mlib_s32);
	n2 = (n + n - n1) / nstep;
	n3 = n + n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			*pdst++ = c0;
			*pdst++ = c1;
		}
	} else {
		for (i = 0; i < n1 / 2; i++) {
			*pdst++ = c0;
			*pdst++ = c1;
		}
		if (n1 & 1) {
			*pdst++ = c0;
			val = _mm_setr_epi32(c1, c0, c1, c0);
		} else {
			val = _mm_setr_epi32(c0, c1, c0, c1);
		}

		for (i = 0; i < n2; i++) {
			_mm_store_si128((__m128i *)pdst, val);
			pdst += nstep;
		}

		if (n1 & 1) {
			*pdst++ = c1;
			n3--;
		}
		for (i = 0; i < n3 / 2; i++) {
			*pdst++ = c0;
			*pdst++ = c1;
		}
		if (n3 & 1) {
			*pdst++ = c0;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
