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

#pragma ident	"@(#)mlib_i_VectorMulSAdd.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorMulSAdd_U8_U8_Sat   - linear function on unsigned 8-bit
 *                                       format vector
 *      mlib_VectorMulSAdd_U8_U8_Mod   - linear function on unsigned 8-bit
 *                                       format vector
 *      mlib_VectorMulSAdd_S8_S8_Sat   - linear function on signed 8-bit
 *                                       format vector
 *      mlib_VectorMulSAdd_S8_S8_Mod   - linear function on signed 8-bit
 *                                       format vector
 *      mlib_VectorMulSAdd_S16_S16_Sat - linear function on signed 16-bit
 *                                       format vector
 *      mlib_VectorMulSAdd_S16_S16_Mod - linear function on signed 16-bit
 *                                       format vector
 *      mlib_VectorMulSAdd_S32_S32_Sat - linear function on signed 32-bit
 *                                       format vector
 *      mlib_VectorMulSAdd_S32_S32_Mod - linear function on signed 32-bit
 *                                       format vector
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMulSAdd_U8_U8_Sat(mlib_u8       *z,
 *                                               const mlib_u8 *x,
 *                                               const mlib_u8 *y,
 *                                               const mlib_u8 *c,
 *                                               mlib_s32      n);
 *      mlib_status mlib_VectorMulSAdd_S16_U8_Sat(mlib_s16      *z,
 *                                                const mlib_u8 *x,
 *                                                const mlib_u8 *y,
 *                                                const mlib_u8 *c,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorMulSAdd_U8_U8_Mod(mlib_u8       *z,
 *                                               const mlib_u8 *x,
 *                                               const mlib_u8 *y,
 *                                               const mlib_u8 *c,
 *                                               mlib_s32      n);
 *      mlib_status mlib_VectorMulSAdd_S16_U8_Mod(mlib_s16      *z,
 *                                                const mlib_u8 *x,
 *                                                const mlib_u8 *y,
 *                                                const mlib_u8 *c,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorMulSAdd_S8_S8_Sat(mlib_s8       *z,
 *                                               const mlib_s8 *x,
 *                                               const mlib_s8 *y,
 *                                               const mlib_s8 *c,
 *                                               mlib_s32      n);
 *      mlib_status mlib_VectorMulSAdd_S8_S8_Mod(mlib_s8       *z,
 *                                               const mlib_s8 *x,
 *                                               const mlib_s8 *y,
 *                                               const mlib_s8 *c,
 *                                               mlib_s32      n);
 *      mlib_status mlib_VectorMulSAdd_S16_S8_Mod(mlib_s16      *z,
 *                                                const mlib_s8 *x,
 *                                                const mlib_s8 *y,
 *                                                const mlib_s8 *c,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorMulSAdd_S16_S16_Sat(mlib_s16       *z,
 *                                                 const mlib_s16 *x,
 *                                                 const mlib_s16 *y,
 *                                                 const mlib_s16 *c,
 *                                                 mlib_s32       n);
 *      mlib_status mlib_VectorMulSAdd_S16_S16_Mod(mlib_s16       *z,
 *                                                 const mlib_s16 *x,
 *                                                 const mlib_s16 *y,
 *                                                 const mlib_s16 *c,
 *                                                 mlib_s32       n);
 *      mlib_status mlib_VectorMulSAdd_S32_S16_Mod(mlib_s32       *z,
 *                                                 const mlib_s16 *x,
 *                                                 const mlib_s16 *y,
 *                                                 const mlib_s16 *c,
 *                                                 mlib_s32       n);
 *      mlib_status mlib_VectorMulSAdd_S32_S32_Sat(mlib_s32       *z,
 *                                                 const mlib_s32 *x,
 *                                                 const mlib_s32 *y,
 *                                                 const mlib_s32 *c,
 *                                                 mlib_s32       n);
 *      mlib_status mlib_VectorMulSAdd_S32_S32_Mod(mlib_s32       *z,
 *                                                 const mlib_s32 *x,
 *                                                 const mlib_s32 *y,
 *                                                 const mlib_s32 *c,
 *                                                 mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      x    pointer to the first element of the first input vector
 *      y    pointer to the first element of the second input vector
 *      c    pointer to the scaling factor
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *      z = x + c * y
 */

#include <mlib_algebra.h>
#include <mlib_AlgebraUtil.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorMulSAdd_S16_Sat = __mlib_VectorMulSAdd_S16_Sat
#pragma weak mlib_VectorMulSAdd_S16_S8_Mod = __mlib_VectorMulSAdd_S16_S8_Mod
#pragma weak mlib_VectorMulSAdd_S16_U8_Mod = __mlib_VectorMulSAdd_S16_U8_Mod
#pragma weak mlib_VectorMulSAdd_S8_Mod = __mlib_VectorMulSAdd_S8_Mod
#pragma weak mlib_VectorMulSAdd_U8_Mod = __mlib_VectorMulSAdd_U8_Mod
#pragma weak mlib_VectorMulSAdd_S16_U8_Sat = __mlib_VectorMulSAdd_S16_U8_Sat
#pragma weak mlib_VectorMulSAdd_S32_S32_Mod = \
	__mlib_VectorMulSAdd_S32_S32_Mod
#pragma weak mlib_VectorMulSAdd_S32_S16_Mod = \
	__mlib_VectorMulSAdd_S32_S16_Mod
#pragma weak mlib_VectorMulSAdd_S16_S16_Mod = \
	__mlib_VectorMulSAdd_S16_S16_Mod
#pragma weak mlib_VectorMulSAdd_S8_Sat = __mlib_VectorMulSAdd_S8_Sat
#pragma weak mlib_VectorMulSAdd_S8_S8_Mod = __mlib_VectorMulSAdd_S8_S8_Mod
#pragma weak mlib_VectorMulSAdd_U8_Sat = __mlib_VectorMulSAdd_U8_Sat
#pragma weak mlib_VectorMulSAdd_S32_S32_Sat = \
	__mlib_VectorMulSAdd_S32_S32_Sat
#pragma weak mlib_VectorMulSAdd_S32_Mod = __mlib_VectorMulSAdd_S32_Mod
#pragma weak mlib_VectorMulSAdd_U8_U8_Mod = __mlib_VectorMulSAdd_U8_U8_Mod
#pragma weak mlib_VectorMulSAdd_S16_Mod = __mlib_VectorMulSAdd_S16_Mod
#pragma weak mlib_VectorMulSAdd_S16_S16_Sat = \
	__mlib_VectorMulSAdd_S16_S16_Sat
#pragma weak mlib_VectorMulSAdd_S8_S8_Sat = __mlib_VectorMulSAdd_S8_S8_Sat
#pragma weak mlib_VectorMulSAdd_S32_Sat = __mlib_VectorMulSAdd_S32_Sat
#pragma weak mlib_VectorMulSAdd_U8_U8_Sat = __mlib_VectorMulSAdd_U8_U8_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorMulSAdd_S16_Sat) mlib_VectorMulSAdd_S16_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16_Sat")));
__typeof__(__mlib_VectorMulSAdd_S16_S8_Mod) mlib_VectorMulSAdd_S16_S8_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16_S8_Mod")));
__typeof__(__mlib_VectorMulSAdd_S16_U8_Mod) mlib_VectorMulSAdd_S16_U8_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16_U8_Mod")));
__typeof__(__mlib_VectorMulSAdd_S8_Mod) mlib_VectorMulSAdd_S8_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S8_Mod")));
__typeof__(__mlib_VectorMulSAdd_U8_Mod) mlib_VectorMulSAdd_U8_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_U8_Mod")));
__typeof__(__mlib_VectorMulSAdd_S16_U8_Sat) mlib_VectorMulSAdd_S16_U8_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16_U8_Sat")));
__typeof__(__mlib_VectorMulSAdd_S32_S32_Mod) mlib_VectorMulSAdd_S32_S32_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S32_S32_Mod")));
__typeof__(__mlib_VectorMulSAdd_S32_S16_Mod) mlib_VectorMulSAdd_S32_S16_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S32_S16_Mod")));
__typeof__(__mlib_VectorMulSAdd_S16_S16_Mod) mlib_VectorMulSAdd_S16_S16_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16_S16_Mod")));
__typeof__(__mlib_VectorMulSAdd_S8_Sat) mlib_VectorMulSAdd_S8_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S8_Sat")));
__typeof__(__mlib_VectorMulSAdd_S8_S8_Mod) mlib_VectorMulSAdd_S8_S8_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S8_S8_Mod")));
__typeof__(__mlib_VectorMulSAdd_U8_Sat) mlib_VectorMulSAdd_U8_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_U8_Sat")));
__typeof__(__mlib_VectorMulSAdd_S32_S32_Sat) mlib_VectorMulSAdd_S32_S32_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S32_S32_Sat")));
__typeof__(__mlib_VectorMulSAdd_S32_Mod) mlib_VectorMulSAdd_S32_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S32_Mod")));
__typeof__(__mlib_VectorMulSAdd_U8_U8_Mod) mlib_VectorMulSAdd_U8_U8_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_U8_U8_Mod")));
__typeof__(__mlib_VectorMulSAdd_S16_Mod) mlib_VectorMulSAdd_S16_Mod
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16_Mod")));
__typeof__(__mlib_VectorMulSAdd_S16_S16_Sat) mlib_VectorMulSAdd_S16_S16_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S16_S16_Sat")));
__typeof__(__mlib_VectorMulSAdd_S8_S8_Sat) mlib_VectorMulSAdd_S8_S8_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S8_S8_Sat")));
__typeof__(__mlib_VectorMulSAdd_S32_Sat) mlib_VectorMulSAdd_S32_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_S32_Sat")));
__typeof__(__mlib_VectorMulSAdd_U8_U8_Sat) mlib_VectorMulSAdd_U8_U8_Sat
	__attribute__((weak, alias("__mlib_VectorMulSAdd_U8_U8_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MULSADD_SAT(form_in, form, MAX, MIN)                          \
	{                                                             \
	    mlib_s32 i;                                               \
	    form_in *px = (form_in *) x, *py = (form_in *) y, *pz =   \
		    (form_in *) z;                                    \
	    form mul_add, val_c = *((form_in *) c);                   \
	                                                              \
	    for (i = 0; i < n; i++) {                                 \
		mul_add = px[i] + val_c * py[i];                      \
		if (mul_add > MAX)                                    \
		    mul_add = MAX;                                    \
		else if (mul_add < MIN)                               \
		    mul_add = MIN;                                    \
		pz[i] = mul_add;                                      \
	    }                                                         \
	    return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);           \
	}

/* *********************************************************** */

#define	MULSADD_MOD(form_in, form)                                    \
	{                                                             \
	    mlib_s32 i;                                               \
	    form_in *px = (form_in *) x, *py = (form_in *) y, *pz =   \
		    (form_in *) z;                                    \
	    form val_c = *((form_in *) c);                            \
	                                                              \
	    for (i = 0; i < n; i++)                                   \
		pz[i] = px[i] + val_c * py[i];                        \
	    return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);           \
	}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;
	mlib_s32 i;

	mlib_s32 mul0, mul1, mul2, mul3;
	mlib_s32 sr0, sr1, sr2, sr3;
	mlib_s32 dc = c[0];
	mlib_s32 m = n;

	if ((mlib_addr)z & 2) {
		mul0 = (mlib_s32)px[0] + py[0] * dc;
		pz[0] = ((MLIB_U8_MAX - mul0) >> 31) | mul0;
		pz++;
		px++;
		py++;
		m--;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < m - 1; i += 2) {
		mul0 = (mlib_s32)px[i] + py[i] * dc;
		mul1 = (mlib_s32)px[i + 1] + py[i + 1] * dc;
		pz[i] = ((MLIB_U8_MAX - mul0) >> 31) | mul0;
		pz[i + 1] = ((MLIB_U8_MAX - mul1) >> 31) | mul1;
	}

	if (m & 1) {
		mul0 = (mlib_s32)px[m - 1] + py[m - 1] * dc;
		pz[m - 1] = ((MLIB_U8_MAX - mul0) >> 31) | mul0;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;
	mlib_s32 i;
	mlib_u8  dc = c[0];
	mlib_u8 mul0, mul1;
	mlib_s32 m = n;

	if ((mlib_addr)z & 2) {
		mul0 = px[0] + py[0] * dc;
		pz[0] = mul0;
		pz++;
		px++;
		py++;
		m--;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < m - 1; i += 2) {
		mul0 = px[i] + py[i] * dc;
		mul1 = px[i + 1] + py[i + 1] * dc;
		pz[i] = mul0;
		pz[i + 1] = mul1;
	}

	if (m & 1) {
		mul0 = px[m - 1] + py[m - 1] * dc;
		pz[m - 1] = mul0;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y, *pz = (mlib_s8 *)z;
	mlib_s32 i;
	mlib_s32 dc = c[0];
	mlib_s32 mul0, mul1, mul2, mul3;
	mlib_s32 m = n;

	if ((mlib_addr)z & 2) {
		mul0 = (mlib_s32)px[0] + py[0] * dc;
		pz[0] = (mul0 > MLIB_S8_MAX) ? MLIB_S8_MAX :
			(mul0 <	MLIB_S8_MIN ? MLIB_S8_MIN : mul0);
		pz++;
		px++;
		py++;
		m--;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < m - 1; i += 2) {
		mul0 = (mlib_s32)px[i] + py[i] * dc;
		mul1 = (mlib_s32)px[i + 1] + py[i + 1] * dc;
		pz[i] = (mul0 > MLIB_S8_MAX) ? MLIB_S8_MAX :
			(mul0 <	MLIB_S8_MIN ? MLIB_S8_MIN : mul0);
		pz[i + 1] = (mul1 > MLIB_S8_MAX) ? MLIB_S8_MAX :
			(mul1 <	MLIB_S8_MIN ? MLIB_S8_MIN : mul1);
	}

	if (m & 1) {
		mul0 = (mlib_s32)px[m - 1] + py[m - 1] * dc;
		pz[m - 1] = (mul0 > MLIB_S8_MAX) ? MLIB_S8_MAX :
			(mul0 <	MLIB_S8_MIN ? MLIB_S8_MIN : mul0);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S8_S8_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return __mlib_VectorMulSAdd_U8_U8_Mod((mlib_u8 *)z, (mlib_u8 *)x,
		(mlib_u8 *)y, (mlib_u8 *)c, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_s32 dc = c[0];
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;
	mlib_s32 mul0, mul1;
	mlib_s32 m = n;

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((mlib_addr)z & 2) {
		mul0 = (mlib_s32)px[0] + py[0] * dc;
		z[0] = (mul0 > MLIB_S16_MAX) ? MLIB_S16_MAX :
			(mul0 <	MLIB_S16_MIN ? MLIB_S16_MIN : mul0);
		pz++;
		px++;
		py++;
		m--;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < m - 1; i += 2) {
		mul0 = (mlib_s32)px[i] + py[i] * dc;
		mul1 = (mlib_s32)px[i + 1] + py[i + 1] * dc;
		mul0 = (mul0 > MLIB_S16_MAX) ? MLIB_S16_MAX :
			(mul0 <	MLIB_S16_MIN ? MLIB_S16_MIN : mul0);
		mul1 = (mul1 > MLIB_S16_MAX) ? MLIB_S16_MAX :
			(mul1 <	MLIB_S16_MIN ? MLIB_S16_MIN : mul1);
		pz[i] = mul0;
		pz[i + 1] = mul1;
	}

	if (m & 1) {
		mul0 = (mlib_s32)px[m - 1] + py[m - 1] * dc;
		pz[m - 1] = (mul0 > MLIB_S16_MAX) ? MLIB_S16_MAX :
			(mul0 <	MLIB_S16_MIN ? MLIB_S16_MIN : mul0);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_u16 *px = (mlib_u16 *)x, *py = (mlib_u16 *)y, *pz = (mlib_u16 *)z;
	mlib_s16 dc = ((mlib_u16 *)c)[0];
	mlib_s32 m = n;

	if ((mlib_addr)z & 2) {
		z[0] = px[0] + py[0] * dc;
		pz++;
		px++;
		py++;
		m--;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < m - 1; i += 2) {
		pz[i] = px[i] + py[i] * dc;
		pz[i + 1] = px[i + 1] + py[i + 1] * dc;
	}

	if (m & 1) {
		pz[m - 1] = px[m - 1] + py[m - 1] * dc;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}
#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32_S32_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	const mlib_s32 *c,
	mlib_s32 n)
{
#ifndef MLIB_USE_FTOI_CLAMPING

	MULSADD_SAT(mlib_s32,
		mlib_d64,
		MLIB_S32_MAX,
		MLIB_S32_MIN)
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 *pz = (mlib_s32 *)z;
	mlib_d64 dc = c[0];
	mlib_d64 mul, dx, dy;
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		dx = x[i];
		dy = y[i];
		mul = dx + dy * dc;
		pz[i] = mul;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* MLIB_USE_FTOI_CLAMPING */
}
#else
/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32_S32_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	const mlib_s32 *c,
	mlib_s32 n)
{
	mlib_s32 *pz = (mlib_s32 *)z, *px = (mlib_s32 *)x, *py = (mlib_s32 *)y;
	mlib_s64 dc = c[0];
	mlib_s64 mul, mul0, mul1, dx, dy;
	mlib_s32 i;
	mlib_s32 m = n;

	if ((mlib_addr)z & 2) {
		mul = (mlib_s64)px[0] + py[0] * dc;
		z[0] = (mul0 > MLIB_S32_MAX) ? MLIB_S32_MAX :
			(mul0 < MLIB_S32_MIN ? MLIB_S32_MIN : mul0);
		pz++;
		px++;
		py++;
		m--;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < m - 1; i += 2) {
		mul0 = (mlib_s64)px[i] + py[i] * dc;
		mul1 = (mlib_s64)px[i + 1] + py[i + 1] * dc;
		mul0 = (mul0 > MLIB_S32_MAX) ? MLIB_S32_MAX :
			(mul0 <	MLIB_S32_MIN ? MLIB_S32_MIN : mul0);
		mul1 = (mul1 > MLIB_S32_MAX) ? MLIB_S32_MAX :
			(mul1 <	MLIB_S32_MIN ? MLIB_S32_MIN : mul1);
		pz[i] = mul0;
		pz[i + 1] = mul1;
	}

	if (m & 1) {
		mul0 = (mlib_s64)px[m - 1] + py[m - 1] * dc;
		pz[m - 1] = (mul0 > MLIB_S32_MAX) ? MLIB_S32_MAX :
			(mul0 <	MLIB_S32_MIN ? MLIB_S32_MIN : mul0);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}
#endif

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32_S32_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	const mlib_s32 *c,
	mlib_s32 n)
{
#ifndef MLIB_USE_FTOI_CLAMPING
	MULSADD_MOD(mlib_s32,
		mlib_s32)
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 i;
	mlib_d64 d16 = 0x10000, _d16 = 1 / d16;
	mlib_d64 d32 = d16 * d16;
	mlib_d64 mul_hi, mul_lo;
	mlib_d64 dc_hi = ((mlib_s16 *)(c))[0], dy, dx;
	mlib_d64 dc_lo = ((mlib_u16 *)(c))[1];
	mlib_d64 cst = (1 << 30) * (mlib_d64)(3 << 21);
	type_union_mlib_d64 r;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		dy = y[i];
		dx = x[i];

		mul_hi = dc_hi * dy;
		mul_lo = dc_lo * dy;

		mul_hi = mul_hi * d16 - (mlib_s32)(mul_hi * _d16) * d32;
		r.db = mul_hi + mul_lo + dx + cst;
		z[i] = r.two_int.int1;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}
#else
/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32_S32_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	const mlib_s32 *c,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_s32 dy, dx;
	mlib_s32 mul0, mul1, dc = c[0];
	mlib_s32 m = n;

	if ((mlib_addr)z & 2) {
		z[0] = x[0] + y[0] * dc;
		z++;
		x++;
		y++;
		m--;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < m - 1; i += 2) {
		z[i] = x[i] + y[i] * dc;
		z[i + 1] = x[i + 1] + y[i + 1] * dc;
	}

	if (m & 1) {
		z[m - 1] = x[m - 1] + y[m - 1] * dc;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}
#endif
/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16_U8_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_s32 dc = c[0];
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	mlib_s32 mul0, mul1, mul;
	mlib_s32 m = n, index1, index2;

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((mlib_addr)z & 2) {
		mul = (mlib_s32)px[0] + py[0] * dc;
		pz[0] = ((((MLIB_S16_MAX - mul) >> 31) | mul) & MLIB_S16_MAX);
		pz++;
		px++;
		py++;
		m--;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (m >> 1); i++) {
		index1 = i << 1;
		index2 = index1 + 1;
		mul0 = (mlib_s32)px[index1] + py[index1] * dc;
		mul1 = (mlib_s32)px[index2] + py[index2] * dc;

		pz[index1] = ((((MLIB_S16_MAX - mul0) >> 31) |
					mul0) & MLIB_S16_MAX);
		pz[index2] = ((((MLIB_S16_MAX - mul1) >> 31) |
					mul1) & MLIB_S16_MAX);
	}

	if (m & 1) {
		index1 = m - 1;
		mul = (mlib_s32)px[index1] + py[index1] * dc;
		pz[index1] = ((((MLIB_S16_MAX - mul) >> 31) |
					mul) & MLIB_S16_MAX);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16_U8_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_s32 mul, dc = *((mlib_u8 *)c);
	mlib_s32 m = n, index1, index2, mul0, mul1;

	if ((mlib_addr)z & 2) {
		mul = (mlib_s32)x[0] + y[0] * dc;
		z[0] = mul;
		z++;
		x++;
		y++;
		m--;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (m >> 1); i++) {
		index1 = i << 1;
		index2 = index1 + 1;
		mul0 = (mlib_s32)x[index1] + y[index1] * dc;
		mul1 = (mlib_s32)x[index2] + y[index2] * dc;

		z[index2] = mul1;
		z[index1] = mul0;
	}

	if (m & 1) {
		index1 = m - 1;
		mul = (mlib_s32)x[index1] + y[index1] * dc;
		z[index1] = mul;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16_S8_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_s32 dc = c[0];
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	mlib_s32 mul0, mul1, mul;
	mlib_s32 m = n, index1, index2;

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((mlib_addr)z & 2) {
		mul = (mlib_s32)px[0] + py[0] * dc;
		pz[0] = mul;
		pz++;
		px++;
		py++;
		m--;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (m >> 1); i++) {
		index1 = i << 1;
		index2 = index1 + 1;
		mul0 = (mlib_s32)px[index1] + py[index1] * dc;
		mul1 = (mlib_s32)px[index2] + py[index2] * dc;

		pz[index2] = mul1;
		pz[index1] = mul0;
	}

	if (m & 1) {
		index1 = m - 1;
		mul = (mlib_s32)px[index1] + py[index1] * dc;
		pz[index1] = mul;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32_S16_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y;
	mlib_s32 *pz = (mlib_s32 *)z;
	mlib_s32 dc = c[0];
	mlib_s32 index1, index2;

	mlib_s32 m = n;
	if ((mlib_addr)z & 2) {
		pz[0] = (mlib_s32)px[0] + py[0] * dc;
		pz++;
		px++;
		py++;
		m--;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (m >> 1); i++) {
		index1 = i << 1;
		index2 = index1 + 1;
		pz[index1] = (mlib_s32)px[index1] + py[index1] * dc;
		pz[index2] = (mlib_s32)px[index2] + py[index2] * dc;
	}

	if (m & 1) {
		index1 = m - 1;
		pz[index1] = (mlib_s32)px[index1] + py[index1] * dc;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_U8_Sat(
	mlib_u8 *xz,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_U8_U8_Sat(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_U8_Mod(
	mlib_u8 *xz,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_U8_U8_Mod(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S8_Sat(
	mlib_s8 *xz,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S8_S8_Sat(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S8_Mod(
	mlib_s8 *xz,
	const mlib_s8 *y,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S8_S8_Mod(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16_Sat(
	mlib_s16 *xz,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S16_S16_Sat(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16_Mod(
	mlib_s16 *xz,
	const mlib_s16 *y,
	const mlib_s16 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S16_S16_Mod(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32_Sat(
	mlib_s32 *xz,
	const mlib_s32 *y,
	const mlib_s32 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S32_S32_Sat(xz, xz, y, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S32_Mod(
	mlib_s32 *xz,
	const mlib_s32 *y,
	const mlib_s32 *c,
	mlib_s32 n)
{
	return (__mlib_VectorMulSAdd_S32_S32_Mod(xz, xz, y, c, n));
}

/* *********************************************************** */
