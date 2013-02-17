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
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_c_VectorReverseByteOrder.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorReverseByteOrder_S16_S16 - change the endianness
 *      mlib_VectorReverseByteOrder_U16_U16 - change the endianness
 *      mlib_VectorReverseByteOrder_S32_S32 - change the endianness
 *      mlib_VectorReverseByteOrder_U32_U32 - change the endianness
 *      mlib_VectorReverseByteOrder_S64_S64 - change the endianness
 *      mlib_VectorReverseByteOrder_U64_U64 - change the endianness
 *      mlib_VectorReverseByteOrder_F32_F32 - change the endianness
 *      mlib_VectorReverseByteOrder_D64_D64 - change the endianness
 *      mlib_VectorReverseByteOrder         - change the endianness
 *      mlib_VectorReverseByteOrder_S16     - change the endianness
 *      mlib_VectorReverseByteOrder_U16     - change the endianness
 *      mlib_VectorReverseByteOrder_S32     - change the endianness
 *      mlib_VectorReverseByteOrder_U32     - change the endianness
 *      mlib_VectorReverseByteOrder_S64     - change the endianness
 *      mlib_VectorReverseByteOrder_U64     - change the endianness
 *      mlib_VectorReverseByteOrder_F32     - change the endianness
 *      mlib_VectorReverseByteOrder_D64     - change the endianness
 *      mlib_VectorReverseByteOrder_Inp     - change the endianness
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorReverseByteOrder_S16_S16(mlib_s16       *z,
 *                                                      const mlib_s16 *x,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorReverseByteOrder_U16_U16(mlib_u16       *z,
 *                                                      const mlib_u16 *x,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorReverseByteOrder_S32_S32(mlib_s32       *z,
 *                                                      const mlib_s32 *x,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorReverseByteOrder_U32_U32(mlib_u32       *z,
 *                                                      const mlib_u32 *x,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorReverseByteOrder_S64_S64(mlib_s64       *z,
 *                                                      const mlib_s64 *x,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorReverseByteOrder_U64_U64(mlib_u64       *z,
 *                                                      const mlib_u64 *x,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorReverseByteOrder_F32_F32(mlib_f32       *z,
 *                                                      const mlib_f32 *x,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorReverseByteOrder_D64_D64(mlib_d64       *z,
 *                                                      const mlib_d64 *x,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorReverseByteOrder(void       *z,
 *                                              const void *x,
 *                                              mlib_s32   n,
 *                                              mlib_s32   s);
 *      mlib_status mlib_VectorReverseByteOrder_S16(mlib_s16 *xz,
 *                                                  mlib_s32  n);
 *      mlib_status mlib_VectorReverseByteOrder_U16(mlib_u16 *xz,
 *                                                  mlib_s32  n);
 *      mlib_status mlib_VectorReverseByteOrder_S32(mlib_s32 *xz,
 *                                                  mlib_s32  n);
 *      mlib_status mlib_VectorReverseByteOrder_U32(mlib_u32 *xz,
 *                                                  mlib_s32  n);
 *      mlib_status mlib_VectorReverseByteOrder_S64(mlib_s64 *xz,
 *                                                  mlib_s32  n);
 *      mlib_status mlib_VectorReverseByteOrder_U64(mlib_u64 *xz,
 *                                                  mlib_s32  n);
 *      mlib_status mlib_VectorReverseByteOrder_F32(mlib_f32 *xz,
 *                                                  mlib_s32  n);
 *      mlib_status mlib_VectorReverseByteOrder_D64(mlib_d64 *xz,
 *                                                  mlib_s32  n);
 *      mlib_status mlib_VectorReverseByteOrder_Inp(void     *xz,
 *                                                  mlib_s32 n,
 *                                                  mlib_s32 s);
 *
 * ARGUMENT
 *      z    pointer to output vector
 *      x    pointer to input vector
 *      xz   pointer to input and output vector
 *      n    number of elements in the vectors
 *      s    size of elements in bytes
 *
 * DESCRIPTION
 *      Change the encoding of each element from big endian to
 *      little endian, or from little endian to big endian.
 *
 * RESTRICTION
 *      The vector element size can be from 2 to 8 bytes for
 *      mlib_VectorReverseByteOrder{_Inp} functions.
 */

#include <mlib_VectorReverseByteOrder.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorReverseByteOrder_Inp = \
	__mlib_VectorReverseByteOrder_Inp
#pragma weak mlib_VectorReverseByteOrder_U64 = \
	__mlib_VectorReverseByteOrder_U64
#pragma weak mlib_VectorReverseByteOrder_U16_U16 = \
	__mlib_VectorReverseByteOrder_U16_U16
#pragma weak mlib_VectorReverseByteOrder = __mlib_VectorReverseByteOrder
#pragma weak mlib_VectorReverseByteOrder_U32 = \
	__mlib_VectorReverseByteOrder_U32
#pragma weak mlib_VectorReverseByteOrder_U32_U32 = \
	__mlib_VectorReverseByteOrder_U32_U32
#pragma weak mlib_VectorReverseByteOrder_U16 = \
	__mlib_VectorReverseByteOrder_U16
#pragma weak mlib_VectorReverseByteOrder_U64_U64 = \
	__mlib_VectorReverseByteOrder_U64_U64

#elif defined(__GNUC__)

__typeof__(__mlib_VectorReverseByteOrder_Inp) mlib_VectorReverseByteOrder_Inp
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_Inp")));
__typeof__(__mlib_VectorReverseByteOrder_U64) mlib_VectorReverseByteOrder_U64
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_U64")));
__typeof__(__mlib_VectorReverseByteOrder_U16_U16)
	mlib_VectorReverseByteOrder_U16_U16
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_U16_U16")));
__typeof__(__mlib_VectorReverseByteOrder) mlib_VectorReverseByteOrder
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder")));
__typeof__(__mlib_VectorReverseByteOrder_U32) mlib_VectorReverseByteOrder_U32
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_U32")));
__typeof__(__mlib_VectorReverseByteOrder_U32_U32)
	mlib_VectorReverseByteOrder_U32_U32
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_U32_U32")));
__typeof__(__mlib_VectorReverseByteOrder_U16) mlib_VectorReverseByteOrder_U16
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_U16")));
__typeof__(__mlib_VectorReverseByteOrder_U64_U64)
	mlib_VectorReverseByteOrder_U64_U64
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_U64_U64")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VectorReverseByteOrder_U16_U16(
	mlib_u16 *z,
	const mlib_u16 *x,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_u16 *ux = (mlib_u16 *)x;

	if ((((mlib_addr)x | (mlib_addr)z) & 3) == 0) {

		mlib_u32 v;
		mlib_u32 *z32 = (mlib_u32 *)z;
		mlib_u32 *x32 = (mlib_u32 *)x;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < (n >> 1); i++) {
			v = x32[i];
			z32[i] = ((v >> 8) & ~0xff00) | ((v & ~0xff00) << 8);
		}

		if (n & 1) {
			z[n - 1] = (ux[n - 1] >> 8) | (ux[n - 1] << 8);
		}

	} else {

		mlib_u16 v;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			v = ux[i];
			z[i] = (v >> 8) | (v << 8);
		}
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorReverseByteOrder_U32_U32(
	mlib_u32 *z,
	const mlib_u32 *x,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_u16 *zs = (mlib_u16 *)z;

#ifdef _LITTLE_ENDIAN

	mlib_u16 *xs = (mlib_u16 *)x;
	mlib_u32 v0, v1;

	for (i = 0; i < n; i++) {
		v0 = xs[2 * i];
		v1 = xs[2 * i + 1];
		zs[2 * i] = (v1 >> 8) | (v1 << 8);
		zs[2 * i + 1] = (v0 >> 8) | (v0 << 8);
	}

#else

	mlib_u32 v;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		v = x[i];
		zs[2 * i] = ((v >> 8) & 0xff) | (v << 8);
		zs[2 * i + 1] = ((v >> 8) & 0xff00) | (v >> 24);
	}

#endif /* _LITTLE_ENDIAN */

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorReverseByteOrder_U64_U64(
	mlib_u64 *z,
	const mlib_u64 *x,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_u16 *xs = (mlib_u16 *)x;
	mlib_u16 *zs = (mlib_u16 *)z;

#ifdef _LITTLE_ENDIAN

	mlib_u32 v0, v1, v2, v3;

	for (i = 0; i < n; i++) {
		v0 = xs[4 * i];
		v1 = xs[4 * i + 1];
		v2 = xs[4 * i + 2];
		v3 = xs[4 * i + 3];
		zs[4 * i] = (v3 >> 8) | (v3 << 8);
		zs[4 * i + 1] = (v2 >> 8) | (v2 << 8);
		zs[4 * i + 2] = (v1 >> 8) | (v1 << 8);
		zs[4 * i + 3] = (v0 >> 8) | (v0 << 8);
	}

#else

	mlib_u32 s0, s1, v0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		v0 = *(mlib_u32 *)(&xs[4 * i]);
		s0 = xs[4 * i + 2];
		s1 = xs[4 * i + 3];
		zs[4 * i] = (s1 >> 8) | (s1 << 8);
		zs[4 * i + 1] = (s0 >> 8) | (s0 << 8);
		zs[4 * i + 2] = ((v0 >> 8) & 0xff) | (v0 << 8);
		zs[4 * i + 3] = ((v0 >> 8) & 0xff00) | (v0 >> 24);
	}

#endif /* _LITTLE_ENDIAN */

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

static mlib_status
mlib_VectorReverseByteOrder_U24_U24(
	mlib_u8 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_u32 v0, v1, v2, v3, v4, v5;

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((mlib_addr)z & 1) {
		v0 = x[0];
		v1 = x[1];
		v2 = x[2];
		z[2] = v0;
		z[1] = v1;
		z[0] = v2;
		x += 3;
		z += 3;
		n--;
	}

	if (((mlib_addr)x & 1) == 0) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n / 2; i++) {
			v0 = ((const mlib_u16 *)x)[3 * i];
			v1 = ((const mlib_u16 *)x)[3 * i + 1];
			v2 = ((const mlib_u16 *)x)[3 * i + 2];
#ifdef _LITTLE_ENDIAN
			((mlib_u16 *)z)[3 * i] = (v0 & ~0xff) | (v1 & 0xff);
			((mlib_u16 *)z)[3 * i + 1] = (v2 & ~0xff) | (v0 & 0xff);
			((mlib_u16 *)z)[3 * i + 2] = (v1 & ~0xff) | (v2 & 0xff);
#else
			((mlib_u16 *)z)[3 * i] = (v1 & ~0xff) | (v0 & 0xff);
			((mlib_u16 *)z)[3 * i + 1] = (v0 & ~0xff) | (v2 & 0xff);
			((mlib_u16 *)z)[3 * i + 2] = (v2 & ~0xff) | (v1 & 0xff);
#endif /* _LITTLE_ENDIAN */
		}
	} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n / 2; i++) {
			v0 = x[6 * i];
			v1 = x[6 * i + 1];
			v2 = x[6 * i + 2];
			v3 = x[6 * i + 3];
			v4 = x[6 * i + 4];
			v5 = x[6 * i + 5];
#ifdef _LITTLE_ENDIAN
			((mlib_u16 *)z)[3 * i] = (v1 << 8) | v2;
			((mlib_u16 *)z)[3 * i + 1] = (v5 << 8) | v0;
			((mlib_u16 *)z)[3 * i + 2] = (v3 << 8) | v4;
#else
			((mlib_u16 *)z)[3 * i] = (v2 << 8) | v1;
			((mlib_u16 *)z)[3 * i + 1] = (v0 << 8) | v5;
			((mlib_u16 *)z)[3 * i + 2] = (v4 << 8) | v3;
#endif /* _LITTLE_ENDIAN */
		}
	}

	if (n & 1) {
		v0 = x[3 * n - 3];
		v1 = x[3 * n - 2];
		v2 = x[3 * n - 1];
		z[3 * n - 1] = v0;
#ifdef _LITTLE_ENDIAN
		*((mlib_u16 *)(z + 3 * n - 3)) = (v1 << 8) | v2;
#else
		*((mlib_u16 *)(z + 3 * n - 3)) = (v2 << 8) | v1;
#endif /* _LITTLE_ENDIAN */
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorReverseByteOrder(
	void *z,
	const void *x,
	mlib_s32 n,
	mlib_s32 s)
{
	mlib_s32 i, v0, v1, v2, v3, v4, v5, v6, v7;

	switch (s) {
	    case 2:

		    if ((((mlib_addr)x | (mlib_addr)z) & 1) == 0)
			    return __mlib_VectorReverseByteOrder_U16_U16(
				    (mlib_u16 *)z, (const mlib_u16 *)x, n);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)x)[2 * i];
			    v1 = ((const mlib_u8 *)x)[2 * i + 1];
			    ((mlib_u8 *)z)[2 * i + 1] = v0;
			    ((mlib_u8 *)z)[2 * i] = v1;
		    }

		    break;

	    case 3:
		    return mlib_VectorReverseByteOrder_U24_U24((mlib_u8 *)z,
			    (const mlib_u8 *)x, n);

	    case 4:

		    if ((((mlib_addr)x | (mlib_addr)z) & 3) == 0)
			    return __mlib_VectorReverseByteOrder_U32_U32(
				    (mlib_u32 *)z, (const mlib_u32 *)x, n);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)x)[4 * i];
			    v1 = ((const mlib_u8 *)x)[4 * i + 1];
			    v2 = ((const mlib_u8 *)x)[4 * i + 2];
			    v3 = ((const mlib_u8 *)x)[4 * i + 3];
			    ((mlib_u8 *)z)[4 * i + 3] = v0;
			    ((mlib_u8 *)z)[4 * i + 2] = v1;
			    ((mlib_u8 *)z)[4 * i + 1] = v2;
			    ((mlib_u8 *)z)[4 * i] = v3;
		    }

		    break;

	    case 5:
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)x)[5 * i];
			    v1 = ((const mlib_u8 *)x)[5 * i + 1];
			    v2 = ((const mlib_u8 *)x)[5 * i + 2];
			    v3 = ((const mlib_u8 *)x)[5 * i + 3];
			    v4 = ((const mlib_u8 *)x)[5 * i + 4];
			    ((mlib_u8 *)z)[5 * i + 4] = v0;
			    ((mlib_u8 *)z)[5 * i + 3] = v1;
			    ((mlib_u8 *)z)[5 * i + 2] = v2;
			    ((mlib_u8 *)z)[5 * i + 1] = v3;
			    ((mlib_u8 *)z)[5 * i] = v4;
		    }

		    break;

	    case 6:
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)x)[6 * i];
			    v1 = ((const mlib_u8 *)x)[6 * i + 1];
			    v2 = ((const mlib_u8 *)x)[6 * i + 2];
			    v3 = ((const mlib_u8 *)x)[6 * i + 3];
			    v4 = ((const mlib_u8 *)x)[6 * i + 4];
			    v5 = ((const mlib_u8 *)x)[6 * i + 5];
			    ((mlib_u8 *)z)[6 * i + 5] = v0;
			    ((mlib_u8 *)z)[6 * i + 4] = v1;
			    ((mlib_u8 *)z)[6 * i + 3] = v2;
			    ((mlib_u8 *)z)[6 * i + 2] = v3;
			    ((mlib_u8 *)z)[6 * i + 1] = v4;
			    ((mlib_u8 *)z)[6 * i] = v5;
		    }

		    break;

	    case 7:
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)x)[7 * i];
			    v1 = ((const mlib_u8 *)x)[7 * i + 1];
			    v2 = ((const mlib_u8 *)x)[7 * i + 2];
			    v3 = ((const mlib_u8 *)x)[7 * i + 3];
			    v4 = ((const mlib_u8 *)x)[7 * i + 4];
			    v5 = ((const mlib_u8 *)x)[7 * i + 5];
			    v6 = ((const mlib_u8 *)x)[7 * i + 6];
			    ((mlib_u8 *)z)[7 * i + 6] = v0;
			    ((mlib_u8 *)z)[7 * i + 5] = v1;
			    ((mlib_u8 *)z)[7 * i + 4] = v2;
			    ((mlib_u8 *)z)[7 * i + 3] = v3;
			    ((mlib_u8 *)z)[7 * i + 2] = v4;
			    ((mlib_u8 *)z)[7 * i + 1] = v5;
			    ((mlib_u8 *)z)[7 * i] = v6;
		    }

		    break;

	    case 8:

		    if ((((mlib_addr)x | (mlib_addr)z) & 7) == 0)
			    return __mlib_VectorReverseByteOrder_U64_U64(
				    (mlib_u64 *)z, (const mlib_u64 *)x, n);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)x)[8 * i];
			    v1 = ((const mlib_u8 *)x)[8 * i + 1];
			    v2 = ((const mlib_u8 *)x)[8 * i + 2];
			    v3 = ((const mlib_u8 *)x)[8 * i + 3];
			    v4 = ((const mlib_u8 *)x)[8 * i + 4];
			    v5 = ((const mlib_u8 *)x)[8 * i + 5];
			    v6 = ((const mlib_u8 *)x)[8 * i + 6];
			    v7 = ((const mlib_u8 *)x)[8 * i + 7];
			    ((mlib_u8 *)z)[8 * i + 7] = v0;
			    ((mlib_u8 *)z)[8 * i + 6] = v1;
			    ((mlib_u8 *)z)[8 * i + 5] = v2;
			    ((mlib_u8 *)z)[8 * i + 4] = v3;
			    ((mlib_u8 *)z)[8 * i + 3] = v4;
			    ((mlib_u8 *)z)[8 * i + 2] = v5;
			    ((mlib_u8 *)z)[8 * i + 1] = v6;
			    ((mlib_u8 *)z)[8 * i] = v7;
		    }

		    break;

	    default:
		    return (MLIB_FAILURE);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorReverseByteOrder_U16(
	mlib_u16 *xz,
	mlib_s32 n)
{
	mlib_s32 i;

	if (((mlib_addr)xz & 3) == 0) {

		mlib_u32 v;
		mlib_u32 *xz32 = (mlib_u32 *)xz;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < (n >> 1); i++) {
			v = xz32[i];
			xz32[i] = ((v >> 8) & ~0xff00) | ((v & ~0xff00) << 8);
		}

		if (n & 1) {
			xz[n - 1] = (xz[n - 1] >> 8) | (xz[n - 1] << 8);
		}

	} else {

		mlib_u16 v;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			v = xz[i];
			xz[i] = (v >> 8) | (v << 8);
		}
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorReverseByteOrder_U32(
	mlib_u32 *xz,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_u16 *xzs = (mlib_u16 *)xz;

#ifdef _LITTLE_ENDIAN

	mlib_u32 v0, v1;

	for (i = 0; i < n; i++) {
		v0 = xzs[2 * i];
		v1 = xzs[2 * i + 1];
		xzs[2 * i] = (v1 >> 8) | (v1 << 8);
		xzs[2 * i + 1] = (v0 >> 8) | (v0 << 8);
	}

#else

	mlib_u32 v;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		v = xz[i];
		xzs[2 * i] = ((v >> 8) & 0xff) | (v << 8);
		xzs[2 * i + 1] = ((v >> 8) & 0xff00) | (v >> 24);
	}

#endif /* _LITTLE_ENDIAN */

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorReverseByteOrder_U64(
	mlib_u64 *xz,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_u16 *xzs = (mlib_u16 *)xz;

#ifdef _LITTLE_ENDIAN

	mlib_u32 v0, v1, v2, v3;

	for (i = 0; i < n; i++) {
		v0 = xzs[4 * i];
		v1 = xzs[4 * i + 1];
		v2 = xzs[4 * i + 2];
		v3 = xzs[4 * i + 3];
		xzs[4 * i] = (v3 >> 8) | (v3 << 8);
		xzs[4 * i + 1] = (v2 >> 8) | (v2 << 8);
		xzs[4 * i + 2] = (v1 >> 8) | (v1 << 8);
		xzs[4 * i + 3] = (v0 >> 8) | (v0 << 8);
	}

#else

	mlib_u32 s0, s1, v0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		v0 = *(mlib_u32 *)(&xzs[4 * i]);
		s0 = xzs[4 * i + 2];
		s1 = xzs[4 * i + 3];
		xzs[4 * i] = (s1 >> 8) | (s1 << 8);
		xzs[4 * i + 1] = (s0 >> 8) | (s0 << 8);
		xzs[4 * i + 2] = ((v0 >> 8) & 0xff) | (v0 << 8);
		xzs[4 * i + 3] = ((v0 >> 8) & 0xff00) | (v0 >> 24);
	}

#endif /* _LITTLE_ENDIAN */

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

static mlib_status
mlib_VectorReverseByteOrder_U24(
	mlib_u8 *xz,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_u32 v0, v1, v2;

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((mlib_addr)xz & 1) {
		v0 = xz[0];
		v2 = xz[2];
		xz[2] = v0;
		xz[0] = v2;
		xz += 3;
		n--;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n / 2; i++) {
		v0 = ((const mlib_u16 *)xz)[3 * i];
		v1 = ((const mlib_u16 *)xz)[3 * i + 1];
		v2 = ((const mlib_u16 *)xz)[3 * i + 2];
#ifdef _LITTLE_ENDIAN
		((mlib_u16 *)xz)[3 * i] = (v0 & ~0xff) | (v1 & 0xff);
		((mlib_u16 *)xz)[3 * i + 1] = (v2 & ~0xff) | (v0 & 0xff);
		((mlib_u16 *)xz)[3 * i + 2] = (v1 & ~0xff) | (v2 & 0xff);
#else
		((mlib_u16 *)xz)[3 * i] = (v1 & ~0xff) | (v0 & 0xff);
		((mlib_u16 *)xz)[3 * i + 1] = (v0 & ~0xff) | (v2 & 0xff);
		((mlib_u16 *)xz)[3 * i + 2] = (v2 & ~0xff) | (v1 & 0xff);
#endif /* _LITTLE_ENDIAN */
	}

	if (n & 1) {
		v0 = xz[3 * n - 3];
		v2 = xz[3 * n - 1];
		xz[3 * n - 1] = v0;
		xz[3 * n - 3] = v2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorReverseByteOrder_Inp(
	void *xz,
	mlib_s32 n,
	mlib_s32 s)
{
	mlib_s32 i, v0, v1, v2, v3, v4, v5, v6, v7;

	switch (s) {
	    case 2:

		    if (((mlib_addr)xz & 1) == 0)
			    return __mlib_VectorReverseByteOrder_U16((mlib_u16
				    *)xz, n);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)xz)[2 * i];
			    v1 = ((const mlib_u8 *)xz)[2 * i + 1];
			    ((mlib_u8 *)xz)[2 * i + 1] = v0;
			    ((mlib_u8 *)xz)[2 * i] = v1;
		    }

		    break;

	    case 3:
		    return (mlib_VectorReverseByteOrder_U24((mlib_u8 *)xz, n));

	    case 4:

		    if (((mlib_addr)xz & 3) == 0)
			    return __mlib_VectorReverseByteOrder_U32((mlib_u32
				    *)xz, n);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)xz)[4 * i];
			    v1 = ((const mlib_u8 *)xz)[4 * i + 1];
			    v2 = ((const mlib_u8 *)xz)[4 * i + 2];
			    v3 = ((const mlib_u8 *)xz)[4 * i + 3];
			    ((mlib_u8 *)xz)[4 * i + 3] = v0;
			    ((mlib_u8 *)xz)[4 * i + 2] = v1;
			    ((mlib_u8 *)xz)[4 * i + 1] = v2;
			    ((mlib_u8 *)xz)[4 * i] = v3;
		    }

		    break;

	    case 5:
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)xz)[5 * i];
			    v1 = ((const mlib_u8 *)xz)[5 * i + 1];
			    v3 = ((const mlib_u8 *)xz)[5 * i + 3];
			    v4 = ((const mlib_u8 *)xz)[5 * i + 4];
			    ((mlib_u8 *)xz)[5 * i + 4] = v0;
			    ((mlib_u8 *)xz)[5 * i + 3] = v1;
			    ((mlib_u8 *)xz)[5 * i + 1] = v3;
			    ((mlib_u8 *)xz)[5 * i] = v4;
		    }

		    break;

	    case 6:
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)xz)[6 * i];
			    v1 = ((const mlib_u8 *)xz)[6 * i + 1];
			    v2 = ((const mlib_u8 *)xz)[6 * i + 2];
			    v3 = ((const mlib_u8 *)xz)[6 * i + 3];
			    v4 = ((const mlib_u8 *)xz)[6 * i + 4];
			    v5 = ((const mlib_u8 *)xz)[6 * i + 5];
			    ((mlib_u8 *)xz)[6 * i + 5] = v0;
			    ((mlib_u8 *)xz)[6 * i + 4] = v1;
			    ((mlib_u8 *)xz)[6 * i + 3] = v2;
			    ((mlib_u8 *)xz)[6 * i + 2] = v3;
			    ((mlib_u8 *)xz)[6 * i + 1] = v4;
			    ((mlib_u8 *)xz)[6 * i] = v5;
		    }

		    break;

	    case 7:
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)xz)[7 * i];
			    v1 = ((const mlib_u8 *)xz)[7 * i + 1];
			    v2 = ((const mlib_u8 *)xz)[7 * i + 2];
			    v4 = ((const mlib_u8 *)xz)[7 * i + 4];
			    v5 = ((const mlib_u8 *)xz)[7 * i + 5];
			    v6 = ((const mlib_u8 *)xz)[7 * i + 6];
			    ((mlib_u8 *)xz)[7 * i + 6] = v0;
			    ((mlib_u8 *)xz)[7 * i + 5] = v1;
			    ((mlib_u8 *)xz)[7 * i + 4] = v2;
			    ((mlib_u8 *)xz)[7 * i + 2] = v4;
			    ((mlib_u8 *)xz)[7 * i + 1] = v5;
			    ((mlib_u8 *)xz)[7 * i] = v6;
		    }

		    break;

	    case 8:

		    if (((mlib_addr)xz & 7) == 0)
			    return __mlib_VectorReverseByteOrder_U64((mlib_u64
				    *)xz, n);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)xz)[8 * i];
			    v1 = ((const mlib_u8 *)xz)[8 * i + 1];
			    v2 = ((const mlib_u8 *)xz)[8 * i + 2];
			    v3 = ((const mlib_u8 *)xz)[8 * i + 3];
			    v4 = ((const mlib_u8 *)xz)[8 * i + 4];
			    v5 = ((const mlib_u8 *)xz)[8 * i + 5];
			    v6 = ((const mlib_u8 *)xz)[8 * i + 6];
			    v7 = ((const mlib_u8 *)xz)[8 * i + 7];
			    ((mlib_u8 *)xz)[8 * i + 7] = v0;
			    ((mlib_u8 *)xz)[8 * i + 6] = v1;
			    ((mlib_u8 *)xz)[8 * i + 5] = v2;
			    ((mlib_u8 *)xz)[8 * i + 4] = v3;
			    ((mlib_u8 *)xz)[8 * i + 3] = v4;
			    ((mlib_u8 *)xz)[8 * i + 2] = v5;
			    ((mlib_u8 *)xz)[8 * i + 1] = v6;
			    ((mlib_u8 *)xz)[8 * i] = v7;
		    }

		    break;

	    default:
		    return (MLIB_FAILURE);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */
