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

#ifndef _MLIB_VECTORREVERSEBYTEORDER_H
#define	_MLIB_VECTORREVERSEBYTEORDER_H

#pragma ident	"@(#)mlib_VectorReverseByteOrder.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * FUNCTIONS
 *      Macros for mlib_VectorReverseByteOrder_* functions
 *
 */

#include <mlib_algebra.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorReverseByteOrder_S32_S32 = \
	__mlib_VectorReverseByteOrder_S32_S32
#pragma weak mlib_VectorReverseByteOrder_S16_S16 = \
	__mlib_VectorReverseByteOrder_S16_S16
#pragma weak mlib_VectorReverseByteOrder_F32_F32 = \
	__mlib_VectorReverseByteOrder_F32_F32
#pragma weak mlib_VectorReverseByteOrder_S32 = \
	__mlib_VectorReverseByteOrder_S32
#pragma weak mlib_VectorReverseByteOrder_S64_S64 = \
	__mlib_VectorReverseByteOrder_S64_S64
#pragma weak mlib_VectorReverseByteOrder_S16 = \
	__mlib_VectorReverseByteOrder_S16
#pragma weak mlib_VectorReverseByteOrder_D64_D64 = \
	__mlib_VectorReverseByteOrder_D64_D64
#pragma weak mlib_VectorReverseByteOrder_F32 = \
	__mlib_VectorReverseByteOrder_F32
#pragma weak mlib_VectorReverseByteOrder_S64 = \
	__mlib_VectorReverseByteOrder_S64
#pragma weak mlib_VectorReverseByteOrder_D64 = \
	__mlib_VectorReverseByteOrder_D64

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorReverseByteOrder_S32_S32)
	mlib_VectorReverseByteOrder_S32_S32
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_S32_S32")));
__typeof__(__mlib_VectorReverseByteOrder_S16_S16)
	mlib_VectorReverseByteOrder_S16_S16
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_S16_S16")));
__typeof__(__mlib_VectorReverseByteOrder_F32_F32)
	mlib_VectorReverseByteOrder_F32_F32
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_F32_F32")));
__typeof__(__mlib_VectorReverseByteOrder_S32) mlib_VectorReverseByteOrder_S32
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_S32")));
__typeof__(__mlib_VectorReverseByteOrder_S64_S64)
	mlib_VectorReverseByteOrder_S64_S64
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_S64_S64")));
__typeof__(__mlib_VectorReverseByteOrder_S16) mlib_VectorReverseByteOrder_S16
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_S16")));
__typeof__(__mlib_VectorReverseByteOrder_D64_D64)
	mlib_VectorReverseByteOrder_D64_D64
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_D64_D64")));
__typeof__(__mlib_VectorReverseByteOrder_F32) mlib_VectorReverseByteOrder_F32
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_F32")));
__typeof__(__mlib_VectorReverseByteOrder_S64) mlib_VectorReverseByteOrder_S64
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_S64")));
__typeof__(__mlib_VectorReverseByteOrder_D64) mlib_VectorReverseByteOrder_D64
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_D64")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VectorReverseByteOrder_S16_S16(
	mlib_s16 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	return __mlib_VectorReverseByteOrder_U16_U16((mlib_u16 *)z,
		(const mlib_u16 *)x, n);
}

mlib_status
__mlib_VectorReverseByteOrder_S32_S32(
	mlib_s32 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	return __mlib_VectorReverseByteOrder_U32_U32((mlib_u32 *)z,
		(const mlib_u32 *)x, n);
}

mlib_status
__mlib_VectorReverseByteOrder_S64_S64(
	mlib_s64 *z,
	const mlib_s64 *x,
	mlib_s32 n)
{
	return __mlib_VectorReverseByteOrder_U64_U64((mlib_u64 *)z,
		(const mlib_u64 *)x, n);
}

mlib_status
__mlib_VectorReverseByteOrder_F32_F32(
	mlib_f32 *z,
	const mlib_f32 *x,
	mlib_s32 n)
{
	return __mlib_VectorReverseByteOrder_U32_U32((mlib_u32 *)z,
		(const mlib_u32 *)x, n);
}

mlib_status
__mlib_VectorReverseByteOrder_D64_D64(
	mlib_d64 *z,
	const mlib_d64 *x,
	mlib_s32 n)
{
	return __mlib_VectorReverseByteOrder_U64_U64((mlib_u64 *)z,
		(const mlib_u64 *)x, n);
}

mlib_status
__mlib_VectorReverseByteOrder_S16(
	mlib_s16 *xz,
	mlib_s32 n)
{
	return (__mlib_VectorReverseByteOrder_U16((mlib_u16 *)xz, n));
}

mlib_status
__mlib_VectorReverseByteOrder_S32(
	mlib_s32 *xz,
	mlib_s32 n)
{
	return (__mlib_VectorReverseByteOrder_U32((mlib_u32 *)xz, n));
}

mlib_status
__mlib_VectorReverseByteOrder_S64(
	mlib_s64 *xz,
	mlib_s32 n)
{
	return (__mlib_VectorReverseByteOrder_U64((mlib_u64 *)xz, n));
}

mlib_status
__mlib_VectorReverseByteOrder_F32(
	mlib_f32 *xz,
	mlib_s32 n)
{
	return (__mlib_VectorReverseByteOrder_U32((mlib_u32 *)xz, n));
}

mlib_status
__mlib_VectorReverseByteOrder_D64(
	mlib_d64 *xz,
	mlib_s32 n)
{
	return (__mlib_VectorReverseByteOrder_U64((mlib_u64 *)xz, n));
}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_VECTORREVERSEBYTEORDER_H */
