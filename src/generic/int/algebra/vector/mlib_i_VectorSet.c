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

#pragma ident	"@(#)mlib_i_VectorSet.c	9.3	07/11/05 SMI"

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
 * do not perform the copying by mlib_s64 data type for _NO_LONGLONG
 */
#ifdef _NO_LONGLONG

typedef struct
{
	mlib_s32 int0, int1;
} two_int;

#define	TYPE_64BIT	two_int

#else  /* _NO_LONGLONG */

#define	TYPE_64BIT	mlib_s64

#endif /* _NO_LONGLONG */

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
	    mlib_c_set_64((mlib_s64 *)dst, (mlib_s64 *)&what2,   \
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
	    mlib_c_set_64((mlib_s64 *)dst, (mlib_s64 *)&what2,   \
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
	    mlib_c_set_64((mlib_s64 *)dst, (mlib_s64 *)&what2,   \
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
	mlib_s64 *dst,
	const mlib_s64 *src,
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

/* *********************************************************** */

mlib_status
__mlib_VectorSet_U8(
	mlib_u8 *z,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_u8 cc = c[0], *dst = z;
	mlib_s32 i, rest_64, length_64;

	if (n < 16) {
		SET_BY_LOOP(dst, n, cc)
	} else {
		mlib_s32 what = (cc << 8) | cc;

		what = (what << 16) | what;
		SET_LARGE(F_8)
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorSet_S8(
	mlib_s8 *z,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorSet_U8((mlib_u8 *)z, (mlib_u8 *)c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSet_S16(
	mlib_s16 *z,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_u16 cc = ((mlib_u16 *)c)[0], *dst = (mlib_u16 *)z;
	mlib_s32 i, rest_64, length_64;

	if (n < 8) {
		SET_BY_LOOP(dst, n, cc)
	} else {
		mlib_s32 what = (cc << 16) | cc;

		SET_LARGE(F_16)
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorSet_S32(
	mlib_s32 *z,
	const mlib_s32 *c,
	mlib_s32 n)
{
	mlib_s32 cc = c[0], *dst = z;
	mlib_s32 i, rest_64, length_64;

	if (n < 4) {
		SET_BY_LOOP(dst, n, cc)
	} else {
		mlib_s32 what = cc;

		SET_LARGE(F_32)
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorSet_U8C(
	mlib_u8 *z,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_s32 i = 0, rest_64, length_64;
	mlib_u8 c_r = c[0], *dst = z, c_i = c[1];

	n = n + n;

	if (n < 16) {
		SET_BY_LOOP_C(dst, n)
	} else {
		mlib_s32 align8 = ((mlib_addr)dst) & 7;
		mlib_s32 what_0 = (c_r << 8) | c_i, what_1 = (c_i << 8) | c_r;

		what_0 = (what_0 << 16) | what_0;
		what_1 = (what_1 << 16) | what_1;
		SET_LARGE_C(F_8)
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
	mlib_s32 i = 0, rest_64, length_64;
	mlib_u16 c_r = ((mlib_u16 *)c)[0], *dst = (mlib_u16 *)z, c_i =
		((mlib_u16 *)c)[1];

	n = n + n;

	if (n < 8) {
		SET_BY_LOOP_C(dst, n)
	} else {
		mlib_s32 align8 = (((mlib_addr)dst) & 7) >> 1;
		mlib_s32 what_0 = (c_r << 16) | c_i, what_1 = (c_i << 16) | c_r;

		SET_LARGE_C(F_16)
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorSet_S32C(
	mlib_s32 *z,
	const mlib_s32 *c,
	mlib_s32 n)
{
	mlib_s32 i, rest_64, length_64;
	mlib_s32 c_r = c[0], *dst = z, c_i = c[1];

	n = n + n;

	if (n < 4) {
		SET_BY_LOOP_C(dst, n)
	} else {
		mlib_s32 align8 = (((mlib_addr)dst) & 7) >> 2;
		type_union_mlib_d64 what2;

#ifndef _LITTLE_ENDIAN

		if (align8) {
			(*dst++) = c_r;
			n--;
			what2.two_int.int0 = c_i;
			what2.two_int.int1 = c_r;
		} else {
			what2.two_int.int0 = c_r;
			what2.two_int.int1 = c_i;
		}

#else

		if (align8) {
			(*dst++) = c_r;
			n--;
			what2.two_int.int0 = c_r;
			what2.two_int.int1 = c_i;
		} else {
			what2.two_int.int0 = c_i;
			what2.two_int.int1 = c_r;
		}

#endif /* _LITTLE_ENDIAN */

		rest_64 = n & 1;
		length_64 = n >> 1;
		n = n & ~1;
		mlib_c_set_64((mlib_s64 *)dst, (mlib_s64 *)&what2, length_64);

		if (rest_64)
			dst[n] = (align8 & 1) ? c_i : c_r;
		return (MLIB_SUCCESS);
	}
}

/* *********************************************************** */
