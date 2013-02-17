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

#pragma ident	"@(#)mlib_MatrixUnit.c	9.4	07/10/08 SMI"

/*
 * FUNCTIONS
 *      mlib_MatrixUnit_U8 - generation of a unit 8-bit format element matrix
 *      mlib_MatrixUnit_S8 - generation of a unit 8-bit format element matrix
 *      mlib_MatrixUnit_S16 - generation of a unit 16-bit format element matrix
 *      mlib_MatrixUnit_S32 - generation of a unit 32-bit format element matrix
 *      mlib_MatrixUnit_U8C - generation of complex unit 8-bit
 *                            format element matrix
 *      mlib_MatrixUnit_S8C - generation of complex unit 8-bit
 *                            format element matrix
 *      mlib_MatrixUnit_S16C - generation of complex unit 16-bit
 *                             format element matrix
 *      mlib_MatrixUnit_S32C - generation of complex unit 32-bit
 *                             format element matrix
 *
 * SYNOPSIS
 *      mlib_status mlib_MatrixUnit_U8(mlib_u8  * z,
 *                                     mlib_s32 n);
 *      mlib_status mlib_MatrixUnit_S8(mlib_s8  * z,
 *                                     mlib_s32 n);
 *      mlib_status mlib_MatrixUnit_S16(mlib_s16 * z,
 *                                      mlib_s32 n);
 *      mlib_status mlib_MatrixUnit_S32(mlib_s32 * z,
 *                                      mlib_s32 n);
 *      mlib_status mlib_MatrixUnit_U8C(mlib_u8  * z,
 *                                      mlib_s32 n);
 *      mlib_status mlib_MatrixUnit_S8C(mlib_s8  * z,
 *                                      mlib_s32 n);
 *      mlib_status mlib_MatrixUnit_S16C(mlib_s16 * z,
 *                                       mlib_s32 n);
 *      mlib_status mlib_MatrixUnit_S32C(mlib_s32 * z,
 *                                       mlib_s32 n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result matrix.
 *           The matrix is in row major order
 *      n    number of columns and rows in the matrix
 *
 * DESCRIPTION
 *      z = E
 *
 * IMPLEMENTATION
 *
 *      First, Zero all matrix.
 *      Then for all diagonal elements make assignment
 *      z[i, i]=1
 */

#include <mlib_algebra.h>
#include <mlib_MatrixUnit.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_MatrixUnit_S16 = __mlib_MatrixUnit_S16
#pragma weak mlib_MatrixUnit_S32C = __mlib_MatrixUnit_S32C
#pragma weak mlib_MatrixUnit_S8 = __mlib_MatrixUnit_S8
#pragma weak mlib_MatrixUnit_S16C = __mlib_MatrixUnit_S16C
#pragma weak mlib_MatrixUnit_U8 = __mlib_MatrixUnit_U8
#pragma weak mlib_MatrixUnit_S8C = __mlib_MatrixUnit_S8C
#pragma weak mlib_MatrixUnit_U8C = __mlib_MatrixUnit_U8C
#pragma weak mlib_MatrixUnit_S32 = __mlib_MatrixUnit_S32

#elif defined(__GNUC__)

__typeof__(__mlib_MatrixUnit_S16) mlib_MatrixUnit_S16
	__attribute__((weak, alias("__mlib_MatrixUnit_S16")));
__typeof__(__mlib_MatrixUnit_S32C) mlib_MatrixUnit_S32C
	__attribute__((weak, alias("__mlib_MatrixUnit_S32C")));
__typeof__(__mlib_MatrixUnit_S8) mlib_MatrixUnit_S8
	__attribute__((weak, alias("__mlib_MatrixUnit_S8")));
__typeof__(__mlib_MatrixUnit_S16C) mlib_MatrixUnit_S16C
	__attribute__((weak, alias("__mlib_MatrixUnit_S16C")));
__typeof__(__mlib_MatrixUnit_U8) mlib_MatrixUnit_U8
	__attribute__((weak, alias("__mlib_MatrixUnit_U8")));
__typeof__(__mlib_MatrixUnit_S8C) mlib_MatrixUnit_S8C
	__attribute__((weak, alias("__mlib_MatrixUnit_S8C")));
__typeof__(__mlib_MatrixUnit_U8C) mlib_MatrixUnit_U8C
	__attribute__((weak, alias("__mlib_MatrixUnit_U8C")));
__typeof__(__mlib_MatrixUnit_S32) mlib_MatrixUnit_S32
	__attribute__((weak, alias("__mlib_MatrixUnit_S32")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_MatrixUnit_U8(
	mlib_u8 *z,
	mlib_s32 n)
{
	mlib_s32 i, shift = 0;
	mlib_u8 *dst = z;

	if (n <= 0)
		return (MLIB_FAILURE);

	__mlib_VectorZero_U8(z, n * n);

	for (i = 0; i < n; i++, shift += n)
		dst[(shift) + i] = 1;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_MatrixUnit_S8(
	mlib_s8 *z,
	mlib_s32 n)
{
	return (__mlib_MatrixUnit_U8((mlib_u8 *)z, n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixUnit_S16(
	mlib_s16 *z,
	mlib_s32 n)
{
	mlib_s32 i, shift = 0;
	mlib_s16 *dst = z;

	if (n <= 0)
		return (MLIB_FAILURE);

	__mlib_VectorZero_S16(z, n * n);

	for (i = 0; i < n; i++, shift += n)
		dst[(shift) + i] = 1;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_MatrixUnit_S32(
	mlib_s32 *z,
	mlib_s32 n)
{
	mlib_s32 i, shift = 0;
	mlib_s32 *dst = z;

	if (n <= 0)
		return (MLIB_FAILURE);

	__mlib_VectorZero_S32(z, n * n);

	for (i = 0; i < n; i++, shift += n)
		dst[(shift) + i] = 1;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_MatrixUnit_U8C(
	mlib_u8 *z,
	mlib_s32 n)
{
	mlib_s32 i, shift = 0;
	mlib_u8 *dst = z;

	if (n <= 0)
		return (MLIB_FAILURE);

	__mlib_VectorZero_U8(z, n * n * 2);

	for (i = 0; i < 2 * n; i += 2, shift += n << 1)
		dst[shift + i] = 1;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_MatrixUnit_S8C(
	mlib_s8 *z,
	mlib_s32 n)
{
	return (__mlib_MatrixUnit_U8C((mlib_u8 *)z, n));
}

/* *********************************************************** */

mlib_status
__mlib_MatrixUnit_S16C(
	mlib_s16 *z,
	mlib_s32 n)
{
	mlib_s32 i, shift = 0;
	mlib_s16 *dst = z;

	if (n <= 0)
		return (MLIB_FAILURE);

	__mlib_VectorZero_S16(z, n * n * 2);

	for (i = 0; i < 2 * n; i += 2, shift += n << 1)
		dst[shift + i] = 1;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_MatrixUnit_S32C(
	mlib_s32 *z,
	mlib_s32 n)
{
	mlib_s32 i, shift = 0;
	mlib_s32 *dst = z;

	if (n <= 0)
		return (MLIB_FAILURE);

	__mlib_VectorZero_S32(z, n * n * 2);

	for (i = 0; i < 2 * n; i += 2, shift += n << 1)
		dst[shift + i] = 1;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_MatrixUnit_S_U8(
	mlib_u8 *z,
	mlib_s32 n,
	mlib_s32 ns)
{
	mlib_s32 i, shift = 0;
	mlib_u8 *dst = z;

	if (n <= 0)
		return (MLIB_FAILURE);

	for (i = 0; i < n; i++, shift += ns)
		dst[(shift) + i] = 1;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_MatrixUnit_S_S8(
	mlib_s8 *z,
	mlib_s32 n,
	mlib_s32 ns)
{
	return (mlib_MatrixUnit_S_U8((mlib_u8 *)z, n, ns));
}

/* *********************************************************** */

mlib_status
mlib_MatrixUnit_S_S16(
	mlib_s16 *z,
	mlib_s32 n,
	mlib_s32 ns)
{
	mlib_s32 i, shift = 0;
	mlib_s16 *dst = z;

	if (n <= 0)
		return (MLIB_FAILURE);

	for (i = 0; i < n; i++, shift += ns)
		dst[(shift) + i] = 1;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_MatrixUnit_S_S32(
	mlib_s32 *z,
	mlib_s32 n,
	mlib_s32 ns)
{
	mlib_s32 i, shift = 0;
	mlib_s32 *dst = z;

	if (n <= 0)
		return (MLIB_FAILURE);

	for (i = 0; i < n; i++, shift += ns)
		dst[(shift) + i] = 1;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_MatrixUnit_S_U8C(
	mlib_u8 *z,
	mlib_s32 n,
	mlib_s32 ns)
{
	mlib_s32 i, shift = 0;
	mlib_u8 *dst = z;

	if (n <= 0)
		return (MLIB_FAILURE);

	for (i = 0; i < 2 * n; i += 2, shift += ns << 1)
		dst[shift + i] = 1;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_MatrixUnit_S_S8C(
	mlib_s8 *z,
	mlib_s32 n,
	mlib_s32 ns)
{
	return (mlib_MatrixUnit_S_U8C((mlib_u8 *)z, n, ns));
}

/* *********************************************************** */

mlib_status
mlib_MatrixUnit_S_S16C(
	mlib_s16 *z,
	mlib_s32 n,
	mlib_s32 ns)
{
	mlib_s32 i, shift = 0;
	mlib_s16 *dst = z;

	if (n <= 0)
		return (MLIB_FAILURE);

	for (i = 0; i < 2 * n; i += 2, shift += ns << 1)
		dst[shift + i] = 1;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_MatrixUnit_S_S32C(
	mlib_s32 *z,
	mlib_s32 n,
	mlib_s32 ns)
{
	mlib_s32 i, shift = 0;
	mlib_s32 *dst = z;

	if (n <= 0)
		return (MLIB_FAILURE);

	for (i = 0; i < 2 * n; i += 2, shift += ns << 1)
		dst[shift + i] = 1;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
