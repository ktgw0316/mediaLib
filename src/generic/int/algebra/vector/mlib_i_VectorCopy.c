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

#pragma ident	"@(#)mlib_i_VectorCopy.c	9.3	07/11/05 SMI"

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

#define	F_8	6
#define	F_16	5
#define	F_32	4

/*
 *
 * do not perform the copying by mlib_d64 data type for x86
 *
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

#define	CP_SRC	for (i = 0; i < n; i++) dst[i] = src[i]

/* *********************************************************** */

#define	CP_LARGE(format)                                                 \
	while ((mlib_addr)dst & 7) {                                     \
	    (*dst++) = (*src++);                                         \
	    n--;                                                         \
	}                                                                \
	                                                                 \
	rest_64 = n & ((1 << (format - 3)) - 1);                         \
	length_64 = n >> (format - 3);                                   \
	n = length_64 << (format - 3);                                   \
	if (((mlib_addr)src & 7) == 0) {                                 \
	    mlib_c_cp_64((mlib_d64 *)dst, (mlib_d64 *)src, length_64);   \
	} else {                                                         \
	    mlib_c_cp_64_na((mlib_u8 *)dst, (mlib_u8 *)src,              \
		    length_64 * 8);                                      \
	}                                                                \
	                                                                 \
	for (i = 0; i < rest_64; i++)                                    \
	    dst[n + i] = src[n + i]

/* *********************************************************** */

static void
mlib_c_cp_64(
	mlib_d64 *ddst,
	const mlib_d64 *dsrc,
	mlib_s32 n)
{
	mlib_s32 i;

	if (i = n & 1)
#ifdef _NO_LONGLONG
		((TYPE_64BIT *) ddst)[0] = ((TYPE_64BIT *) dsrc)[0];
#else
		((mlib_u64 *) ddst)[0] = ((mlib_u64 *) dsrc)[0];
#endif
	for (; i < n; i += 2) {
#ifdef _NO_LONGLONG
		((TYPE_64BIT *) ddst)[i] = ((TYPE_64BIT *) dsrc)[i];
		((TYPE_64BIT *) ddst)[i + 1] = ((TYPE_64BIT *) dsrc)[i + 1];
#else
		((mlib_u64 *) ddst)[i] = ((mlib_u64 *) dsrc)[i];
		((mlib_u64 *) ddst)[i + 1] = ((mlib_u64 *) dsrc)[i + 1];
#endif
	}
}

/* *********************************************************** */

static void
mlib_c_cp_64_na(
	mlib_u8 *dst,
	const mlib_u8 *src,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_u32 *ps, shl, shr, src0, src1, res;

	shl = (mlib_addr)src & 3;

	if (shl == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i += 4) {
			*((mlib_s32 *)(dst + i)) = *((mlib_s32 *)(src + i));
		}
	} else {
		ps = (mlib_u32 *)((mlib_addr)src - shl);
		shl <<= 3;
		shr = 32 - shl;
		src0 = ps[0];
		src1 = ps[1];
#ifndef _LITTLE_ENDIAN
		res = (src0 << shl) | (src1 >> shr);
#else
		res = (src0 >> shl) | (src1 << shr);
#endif /* _LITTLE_ENDIAN */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < (n - 4); i += 4) {
			src0 = src1;
			src1 = ps[2];
			*((mlib_s32 *)(dst + i)) = res;
#ifndef _LITTLE_ENDIAN
			res = (src0 << shl) | (src1 >> shr);
#else
			res = (src0 >> shl) | (src1 << shr);
#endif /* _LITTLE_ENDIAN */
			ps++;
		}
		*((mlib_s32 *)(dst + i)) = res;
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorCopy_U8(
	mlib_u8 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_u8 *dst = z;
	const mlib_u8 *src = x;
	mlib_s32 length_64, rest_64;

	if (n <= 12) {
		if (n <= 0)
			return (MLIB_FAILURE);
/*
 * For very mlib_s16 lengths we use copying by assignment statement
 * in case - operator.
 */

		switch (n) {
		    case 1:
			    dst[0] = src[0];
			    return (MLIB_SUCCESS);
		    case 2:
			    dst[0] = src[0];
			    dst[1] = src[1];
			    return (MLIB_SUCCESS);
		    case 3:
			    dst[0] = src[0];
			    dst[1] = src[1];
			    dst[2] = src[2];
			    return (MLIB_SUCCESS);
		    case 4:
			    dst[0] = src[0];
			    dst[1] = src[1];
			    dst[2] = src[2];
			    dst[3] = src[3];
			    return (MLIB_SUCCESS);
		    case 5:
			    dst[0] = src[0];
			    dst[1] = src[1];
			    dst[2] = src[2];
			    dst[3] = src[3];
			    dst[4] = src[4];
			    return (MLIB_SUCCESS);
		    case 6:
			    dst[0] = src[0];
			    dst[1] = src[1];
			    dst[2] = src[2];
			    dst[3] = src[3];
			    dst[4] = src[4];
			    dst[5] = src[5];
			    return (MLIB_SUCCESS);
		    case 7:
			    dst[0] = src[0];
			    dst[1] = src[1];
			    dst[2] = src[2];
			    dst[3] = src[3];
			    dst[4] = src[4];
			    dst[5] = src[5];
			    dst[6] = src[6];
			    return (MLIB_SUCCESS);
		    case 8:
			    dst[0] = src[0];
			    dst[1] = src[1];
			    dst[2] = src[2];
			    dst[3] = src[3];
			    dst[4] = src[4];
			    dst[5] = src[5];
			    dst[6] = src[6];
			    dst[7] = src[7];
			    return (MLIB_SUCCESS);
		    case 9:
			    dst[0] = src[0];
			    dst[1] = src[1];
			    dst[2] = src[2];
			    dst[3] = src[3];
			    dst[4] = src[4];
			    dst[5] = src[5];
			    dst[6] = src[6];
			    dst[7] = src[7];
			    dst[8] = src[8];
			    return (MLIB_SUCCESS);
		    case 10:
			    dst[0] = src[0];
			    dst[1] = src[1];
			    dst[2] = src[2];
			    dst[3] = src[3];
			    dst[4] = src[4];
			    dst[5] = src[5];
			    dst[6] = src[6];
			    dst[7] = src[7];
			    dst[8] = src[8];
			    dst[9] = src[9];
			    return (MLIB_SUCCESS);
		    case 11:
			    dst[0] = src[0];
			    dst[1] = src[1];
			    dst[2] = src[2];
			    dst[3] = src[3];
			    dst[4] = src[4];
			    dst[5] = src[5];
			    dst[6] = src[6];
			    dst[7] = src[7];
			    dst[8] = src[8];
			    dst[9] = src[9];
			    dst[10] = src[10];
			    return (MLIB_SUCCESS);
		    case 12:
			    dst[0] = src[0];
			    dst[1] = src[1];
			    dst[2] = src[2];
			    dst[3] = src[3];
			    dst[4] = src[4];
			    dst[5] = src[5];
			    dst[6] = src[6];
			    dst[7] = src[7];
			    dst[8] = src[8];
			    dst[9] = src[9];
			    dst[10] = src[10];
			    dst[11] = src[11];
			    return (MLIB_SUCCESS);
		}
	}

	if (n < 32) {
		CP_SRC;
	} else {
		CP_LARGE(F_8);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorCopy_S8(
	mlib_s8 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	return (__mlib_VectorCopy_U8((mlib_u8 *)z, (mlib_u8 *)x, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorCopy_S16(
	mlib_s16 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_u16 *dst = (mlib_u16 *)z;
	mlib_u16 *src = (mlib_u16 *)x;
	mlib_s32 length_64, rest_64;

	if (n <= 16) {
		if (n <= 0)
			return (MLIB_FAILURE);
		CP_SRC;
	} else {
		CP_LARGE(F_16);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorCopy_S32(
	mlib_s32 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_s32 *dst = z;
	const mlib_s32 *src = x;
	mlib_s32 length_64, rest_64;

	if (n <= 8) {
		if (n <= 0)
			return (MLIB_FAILURE);
		CP_SRC;
	} else {
		CP_LARGE(F_32);
	}

	return (MLIB_SUCCESS);
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
