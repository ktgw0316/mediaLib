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

#pragma ident	"@(#)mlib_c_VectorNorm.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorNorm_U8_Sat  - unsigned 8-bit format vector norm
 *      mlib_VectorNorm_S8_Sat  - signed   8-bit format vector norm
 *      mlib_VectorNorm_S16_Sat - 16-bit format vector norm
 *      mlib_VectorNorm_S32_Sat - 32-bit format vector norm
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorNorm_U8_Sat(mlib_d64      *z,
 *                                         const mlib_u8 *x,
 *                                         mlib_s32      n);
 *      mlib_status mlib_VectorNorm_S8_Sat(mlib_d64      *z,
 *                                         const mlib_s8 *x,
 *                                         mlib_s32      n);
 *      mlib_status mlib_VectorNorm_S16_Sat(mlib_d64       *z,
 *                                          const mlib_s16 *x,
 *                                          mlib_s32       n);
 *      mlib_status mlib_VectorNorm_S32_Sat(mlib_d64       *z,
 *                                          const mlib_s32 *x,
 *                                          mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the norm of the vector
 *      x    pointer to the first element of the input vector
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *                 n
 *      z = sqrt( SUM(x[i] * x[i]) )
 *                i = 1
 */

#include <mlib_algebra.h>
#include <mlib_SysMath.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorNorm_S32_Sat = __mlib_VectorNorm_S32_Sat
#pragma weak mlib_VectorNorm_S16_Sat = __mlib_VectorNorm_S16_Sat
#pragma weak mlib_VectorNorm_S8_Sat = __mlib_VectorNorm_S8_Sat
#pragma weak mlib_VectorNorm_U8_Sat = __mlib_VectorNorm_U8_Sat

#elif defined(__GNUC__)

__typeof__(__mlib_VectorNorm_S32_Sat) mlib_VectorNorm_S32_Sat
	__attribute__((weak, alias("__mlib_VectorNorm_S32_Sat")));
__typeof__(__mlib_VectorNorm_S16_Sat) mlib_VectorNorm_S16_Sat
	__attribute__((weak, alias("__mlib_VectorNorm_S16_Sat")));
__typeof__(__mlib_VectorNorm_S8_Sat) mlib_VectorNorm_S8_Sat
	__attribute__((weak, alias("__mlib_VectorNorm_S8_Sat")));
__typeof__(__mlib_VectorNorm_U8_Sat) mlib_VectorNorm_U8_Sat
	__attribute__((weak, alias("__mlib_VectorNorm_U8_Sat")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VectorNorm_U8_Sat(
	mlib_d64 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x;
	mlib_s32 i, s0, s1, s2, s3, off, *wpx;
	mlib_d64 sum = 0;
	mlib_u32 ld;

	if (n <= 0)
		return (MLIB_FAILURE);

	off = 8 - ((mlib_addr)x & 7);
	for (i = 0; (i < n) && (i < off); i++) {
		ld = (*px++);
		sum += ld * (mlib_d64)ld;
	}

	wpx = (mlib_s32 *)px;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n - 3; i += 4) {
		ld = (*wpx++);
#ifdef _LITTLE_ENDIAN
		s0 = ld & 0xFF;
		s1 = (ld & 0xFF00) >> 8;
		s2 = (ld & 0xFF0000) >> 16;
		s3 = ld >> 24;
#else
		s0 = ld >> 24;
		s1 = (ld & 0xFF0000) >> 16;
		s2 = (ld & 0xFF00) >> 8;
		s3 = ld & 0xFF;
#endif /* _LITTLE_ENDIAN */
		sum += s0 * (mlib_d64)s0;
		sum += s1 * (mlib_d64)s1;
		sum += s2 * (mlib_d64)s2;
		sum += s3 * (mlib_d64)s3;
	}

	px = (mlib_u8 *)wpx;
	for (; i < n; i++) {
		ld = (*px++);
		sum += ld * (mlib_d64)ld;
	}

	z[0] = mlib_sqrt((mlib_d64)sum);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorNorm_S8_Sat(
	mlib_d64 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	mlib_s8 *px = (mlib_s8 *)x;
	mlib_s32 i, s0, s1, s2, s3, off, *wpx, ld;
	mlib_d64 sum = 0;

	if (n <= 0)
		return (MLIB_FAILURE);

	off = 8 - ((mlib_addr)x & 7);
	for (i = 0; (i < n) && (i < off); i++) {
		ld = (*px++);
		sum += ld * (mlib_d64)ld;
	}

	wpx = (mlib_s32 *)px;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n - 3; i += 4) {
		ld = (*wpx++);
#ifdef _LITTLE_ENDIAN
		s0 = (ld << 24) >> 24;
		s1 = (ld << 16) >> 24;
		s2 = (ld << 8) >> 24;
		s3 = ld >> 24;
#else
		s0 = ld >> 24;
		s1 = (ld << 8) >> 24;
		s2 = (ld << 16) >> 24;
		s3 = (ld << 24) >> 24;
#endif /* _LITTLE_ENDIAN */
		sum += s0 * (mlib_d64)s0;
		sum += s1 * (mlib_d64)s1;
		sum += s2 * (mlib_d64)s2;
		sum += s3 * (mlib_d64)s3;
	}

	px = (mlib_s8 *)wpx;
	for (; i < n; i++) {
		ld = (*px++);
		sum += ld * (mlib_d64)ld;
	}

	z[0] = mlib_sqrt((mlib_d64)sum);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorNorm_S16_Sat(
	mlib_d64 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	mlib_s16 *px = (mlib_s16 *)x;
	mlib_s32 i, s0, s1, off, *wpx, ld;
	mlib_d64 sum = 0;

	if (n <= 0)
		return (MLIB_FAILURE);

	off = (8 - ((mlib_addr)x & 7)) >> 1;
	for (i = 0; (i < n) && (i < off); i++) {
		ld = (*px++);
		sum += ld * (mlib_d64)ld;
	}

	wpx = (mlib_s32 *)px;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n - 1; i += 2) {
		ld = (*wpx++);
#ifdef _LITTLE_ENDIAN
		s0 = (ld << 16) >> 16;
		s1 = ld >> 16;
#else
		s0 = ld >> 16;
		s1 = (ld << 16) >> 16;
#endif /* _LITTLE_ENDIAN */
		sum += s0 * (mlib_d64)s0;
		sum += s1 * (mlib_d64)s1;
	}

	px = (mlib_s16 *)wpx;
	for (; i < n; i++) {
		ld = (*px++);
		sum += ld * (mlib_d64)ld;
	}

	z[0] = mlib_sqrt((mlib_d64)sum);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorNorm_S32_Sat(
	mlib_d64 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	mlib_s32 *px = (mlib_s32 *)x;
	mlib_s32 i;
	mlib_d64 sum = 0;

	if (n <= 0)
		return (MLIB_FAILURE);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++)
		sum += px[i] * (mlib_d64)px[i];
	z[0] = mlib_sqrt((mlib_d64)sum);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
