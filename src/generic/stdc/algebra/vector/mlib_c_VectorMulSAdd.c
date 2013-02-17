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

#pragma ident	"@(#)mlib_c_VectorMulSAdd.c	9.3	07/10/09 SMI"

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

#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_s32 mul_add, val_c = *((mlib_u8 *)c);

	for (i = 0; i < n; i++) {
		mul_add = px[i] + val_c * py[i];

		if (mul_add > MLIB_U8_MAX)
			mul_add = MLIB_U8_MAX;
		pz[i] = mul_add;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_d64 d = 1 << 24, mul;
	const mlib_d64 *table = mlib_SU82D64 + 128;
	mlib_d64 dc = table[c[0]];
	mlib_d64 mul0, mul1, mul2, mul3;
	mlib_s32 sr0, sr1, sr2, sr3;
	mlib_s32 a = (4 - (mlib_addr)z & 3) & 3, *spz = (mlib_s32 *)(z + a);
	mlib_s32 b = ((mlib_addr)(z + n) & ~3) - (mlib_addr)z;

	if (n < a) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			mul = table[px[i]] + table[py[i]] * dc;
			pz[i] = (((mlib_s32)((mul - 128) * d)) >> 24) + 128;
		}

		return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < a; i++) {
		mul = table[px[i]] + table[py[i]] * dc;
		pz[i] = (((mlib_s32)((mul - 128) * d)) >> 24) + 128;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = a; i < b; i += 4) {
		mul0 = table[px[i + 0]] + table[py[i + 0]] * dc;
		mul1 = table[px[i + 1]] + table[py[i + 1]] * dc;
		mul2 = table[px[i + 2]] + table[py[i + 2]] * dc;
		mul3 = table[px[i + 3]] + table[py[i + 3]] * dc;

		sr0 = (mul0 - 128) * d;
		sr1 = (mul1 - 128) * d;
		sr2 = (mul2 - 128) * d;
		sr3 = (mul3 - 128) * d;

		sr0 = ((sr0 >> 24) + 128) << 24;
		sr1 = ((sr1 >> 8) + (128 << 16)) & 0x00FF0000;
		sr2 = ((sr2 >> 16) + (128 << 8)) & 0x0000FF00;
		sr3 = ((sr3 >> 24) + 128) & 0x000000FF;

		spz[i >> 2] = sr0 | sr1 | sr2 | sr3;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = b; i < n; i++) {
		mul = table[px[i]] + table[py[i]] * dc;
		pz[i] = (((mlib_s32)((mul - 128) * d)) >> 24) + 128;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* MLIB_USE_FTOI_CLAMPING */
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
#ifndef MLIB_USE_FTOI_CLAMPING
	MULSADD_MOD(mlib_u8,
		mlib_s32)
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 i;
	mlib_d64 d8 = 1 << 8, _d8 = 1 / d8, mul;
	mlib_d64 d16 = d8 * d8, d24 = d16 * d8;
	const mlib_d64 *table = mlib_SU82D64 + 128;
	mlib_d64 dc = table[c[0]];
	mlib_d64 mul0, mul1, mul2, mul3;
	mlib_s32 a = (4 - (mlib_addr)z & 3) & 3, *spz = (mlib_s32 *)(z + a);
	mlib_s32 b = ((mlib_addr)(z + n) & ~3) - (mlib_addr)z;

	if (n < a) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			mul = table[x[i]] + table[y[i]] * dc;
			z[i] = (mlib_s32)mul;
		}

		return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < a; i++) {
		mul = table[x[i]] + table[y[i]] * dc;
		z[i] = (mlib_s32)mul;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = a; i < b; i += 4) {
		mul0 = table[x[i + 0]] + table[y[i + 0]] * dc;
		mul1 = table[x[i + 1]] + table[y[i + 1]] * dc;
		mul2 = table[x[i + 2]] + table[y[i + 2]] * dc;
		mul3 = table[x[i + 3]] + table[y[i + 3]] * dc;

		mul1 = mul1 * d16 - (mlib_s32)(mul1 * _d8) * d24;
		mul2 = mul2 * d8 - (mlib_s32)(mul2 * _d8) * d16;
		mul3 = mul3 - (mlib_s32)(mul3 * _d8) * d8;
		mul1 += mul2 + mul3;

		spz[i >> 2] = (mlib_s32)mul1;
		((mlib_u8 *)(spz + (i >> 2)))[0] = (mlib_s32)mul0;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = b; i < n; i++) {
		mul = table[x[i]] + table[y[i]] * dc;
		z[i] = (mlib_s32)mul;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* MLIB_USE_FTOI_CLAMPING */
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
#ifndef MLIB_USE_FTOI_CLAMPING
	MULSADD_SAT(mlib_s8,
		mlib_s32,
		MLIB_S8_MAX,
		MLIB_S8_MIN)
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 i;
	mlib_d64 d = 1 << 24, mul;
	const mlib_d64 *table = mlib_SU82D64 + 128;
	mlib_d64 dc = table[c[0]];
	mlib_d64 mul0, mul1, mul2, mul3;
	mlib_s32 sr0, sr1, sr2, sr3;
	mlib_s32 a = (4 - (mlib_addr)z & 3) & 3, *spz = (mlib_s32 *)(z + a);
	mlib_s32 b = ((mlib_addr)(z + n) & ~3) - (mlib_addr)z;

	if (n < a) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			mul = table[x[i]] + table[y[i]] * dc;
			z[i] = (((mlib_s32)(mul * d)) >> 24);
		}

		return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < a; i++) {
		mul = table[x[i]] + table[y[i]] * dc;
		z[i] = (((mlib_s32)(mul * d)) >> 24);
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = a; i < b; i += 4) {
		mul0 = table[x[i + 0]] + table[y[i + 0]] * dc;
		mul1 = table[x[i + 1]] + table[y[i + 1]] * dc;
		mul2 = table[x[i + 2]] + table[y[i + 2]] * dc;
		mul3 = table[x[i + 3]] + table[y[i + 3]] * dc;

		sr0 = mul0 * d;
		sr1 = mul1 * d;
		sr2 = mul2 * d;
		sr3 = mul3 * d;

		sr0 = sr0 & 0xFF000000;
		sr1 = (sr1 >> 8) & 0x00FF0000;
		sr2 = (sr2 >> 16) & 0x0000FF00;
		sr3 = (mlib_s32)((mlib_u32)sr3 >> 24);

		spz[i >> 2] = sr0 | sr1 | sr2 | sr3;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = b; i < n; i++) {
		mul = table[x[i]] + table[y[i]] * dc;
		z[i] = (((mlib_s32)(mul * d)) >> 24);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* MLIB_USE_FTOI_CLAMPING */
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
#ifndef MLIB_USE_FTOI_CLAMPING
	MULSADD_SAT(mlib_s16,
		mlib_s32,
		MLIB_S16_MAX,
		MLIB_S16_MIN)
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 i;
	mlib_d64 d = 1 << 16;
	mlib_d64 dc = c[0];
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;
	mlib_d64 mul0, mul1, mul;
	mlib_s32 m = n;

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((mlib_addr)z & 2) {
		mul = (mlib_d64)px[0] + (mlib_d64)py[0] * dc;
		pz[0] = (mlib_s32)(mul * d) >> 16;
		pz++;
		px++;
		py++;
		m--;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < m - 1; i += 2) {
		mul0 = (mlib_d64)px[i + 0] + (mlib_d64)py[i + 0] * dc;
		mul1 = (mlib_d64)px[i + 1] + (mlib_d64)py[i + 1] * dc;
		mul0 *= d;
		mul1 *= d;
		((mlib_s32 *)pz)[i >> 1] = mul0;
		pz[i + 1] = (mlib_s32)mul1 >> 16;
	}

	if (m & 1) {
		mul = (mlib_d64)px[m - 1] + (mlib_d64)py[m - 1] * dc;
		pz[m - 1] = (mlib_s32)(mul * d) >> 16;
	}

	return (MLIB_SUCCESS);

#endif /* MLIB_USE_FTOI_CLAMPING */
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
#ifndef MLIB_USE_FTOI_CLAMPING
	MULSADD_MOD(mlib_s16,
		mlib_s32)
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 i;
	mlib_u16 *px = (mlib_u16 *)x, *py = (mlib_u16 *)y, *pz = (mlib_u16 *)z;
	mlib_d64 d16 = 1 << 16, _d16 = 1 / d16;
	mlib_d64 d32 = d16 * d16;
	mlib_d64 d52 = d32 * d16 * (1 << 4);
	mlib_d64 dc = ((mlib_u16 *)c)[0];
	mlib_d64 mul0, mul1, mul2, mul3, mul;
	mlib_s32 a = (4 - (((mlib_addr)z & 7) >> 1)) & 3;
	mlib_s32 b = (((mlib_addr)(z + n) & ~7) - (mlib_addr)z) >> 1;
	mlib_d64 *dpz = (void *)(z + a);

	if (n < a) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			mul = (mlib_d64)px[i] + (mlib_d64)py[i] * dc;
			pz[i] = mul - (mlib_s32)(mul * _d16) * d16;
		}

		return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < a; i++) {
		mul = (mlib_d64)px[i] + (mlib_d64)py[i] * dc;
		pz[i] = mul - (mlib_s32)(mul * _d16) * d16;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = a; i < b; i += 4) {

		mul0 = (mlib_d64)px[i + 0] + (mlib_d64)py[i + 0] * dc;
		mul1 = (mlib_d64)px[i + 1] + (mlib_d64)py[i + 1] * dc;
		mul2 = (mlib_d64)px[i + 2] + (mlib_d64)py[i + 2] * dc;
		mul3 = (mlib_d64)px[i + 3] + (mlib_d64)py[i + 3] * dc;

		mul1 = (mul1 - (mlib_s32)(mul1 * _d16) * d16) * d32;
		mul2 = (mul2 - (mlib_s32)(mul2 * _d16) * d16) * d16;
		mul3 = (mul3 - (mlib_s32)(mul3 * _d16) * d16);
		mul1 += mul2 + mul3;
		mul0 = mul0 - (mlib_s32)(mul0 * _d16) * d16;

		((mlib_d64 *)dpz)[i >> 2] = mul1 + d52;
		((mlib_u16 *)(dpz + (i >> 2)))[0] = mul0;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = b; i < n; i++) {
		mul = (mlib_d64)px[i] + (mlib_d64)py[i] * dc;
		pz[i] = mul - (mlib_s32)(mul * _d16) * d16;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* MLIB_USE_FTOI_CLAMPING */
}

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

/* *********************************************************** */

mlib_status
__mlib_VectorMulSAdd_S16_U8_Sat(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	const mlib_u8 *c,
	mlib_s32 n)
{
#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_s32 i;
	mlib_s32 mul_add, scal = *((mlib_u8 *)c);

	for (i = 0; i < n; i++) {
		mul_add = ((mlib_u8 *)y)[i] * scal + ((mlib_u8 *)x)[i];

		if (mul_add > MLIB_S16_MAX)
			mul_add = MLIB_S16_MAX;
		((mlib_s16 *)z)[i] = mul_add;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 i;
	const mlib_d64 *table = mlib_SU82D64 + 128;
	mlib_d64 d = 1 << 16;
	mlib_d64 dc = c[0];
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	mlib_d64 mul0, mul1, mul;
	mlib_s32 m = n;

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((mlib_addr)z & 2) {
		mul = table[px[0]] + table[py[0]] * dc;
		pz[0] = (mlib_s32)(mul * d) >> 16;
		pz++;
		px++;
		py++;
		m--;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (m >> 1); i++) {
		mul0 = table[px[2 * i + 0]] + table[py[2 * i + 0]] * dc;
		mul1 = table[px[2 * i + 1]] + table[py[2 * i + 1]] * dc;

		((mlib_s32 *)pz)[i] = mul0 * d;
		pz[2 * i + 1] = (mlib_s32)(mul1 * d) >> 16;
	}

	if (m & 1) {
		mul = table[px[m - 1]] + table[py[m - 1]] * dc;
		pz[m - 1] = (mlib_s32)(mul * d) >> 16;
	}

	return (MLIB_SUCCESS);
#endif /* MLIB_USE_FTOI_CLAMPING */
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
#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_s32 i;
	mlib_s32 mul_add, scal = *((mlib_u8 *)c);

	for (i = 0; i < n; i++) {
		mul_add = ((mlib_u8 *)y)[i] * scal + ((mlib_u8 *)x)[i];
		((mlib_s16 *)z)[i] = mul_add;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 i;
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_u16 *pz = (mlib_u16 *)z;
	mlib_d64 d16 = 1 << 16;
	mlib_d64 d32 = d16 * d16;
	mlib_d64 d52 = d32 * d16 * (1 << 4);
	mlib_d64 mul0, mul1, mul2, mul3, mul;
	mlib_s32 a = (4 - (((mlib_addr)z & 7) >> 1)) & 3;
	mlib_s32 b = (((mlib_addr)(z + n) & ~7) - (mlib_addr)z) >> 1;
	mlib_d64 *dpz = (void *)(z + a);
	const mlib_d64 *table = mlib_SU82D64 + 128;
	mlib_d64 dc = table[c[0]];

	if (n < a) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			mul = table[px[i]] + table[py[i]] * dc;
			pz[i] = (mlib_s32)mul;
		}

		return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < a; i++) {
		mul = table[px[i]] + table[py[i]] * dc;
		pz[i] = (mlib_s32)mul;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = a; i < b; i += 4) {
		mul0 = table[px[i + 0]] + table[py[i + 0]] * dc;
		mul1 = table[px[i + 1]] + table[py[i + 1]] * dc;
		mul2 = table[px[i + 2]] + table[py[i + 2]] * dc;
		mul3 = table[px[i + 3]] + table[py[i + 3]] * dc;

		mul1 *= d32;
		mul2 *= d16;
		mul3 += mul1 + mul2;

		((mlib_d64 *)dpz)[i >> 2] = mul3 + d52;
		((mlib_u16 *)(dpz + (i >> 2)))[0] = mul0;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = b; i < n; i++) {
		mul = table[px[i]] + table[py[i]] * dc;
		pz[i] = (mlib_s32)mul;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#endif /* MLIB_USE_FTOI_CLAMPING */
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
#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_s32 i;
	mlib_s32 mul_add, scal = *((mlib_s8 *)c);

	for (i = 0; i < n; i++) {
		mul_add = ((mlib_s8 *)y)[i] * scal + ((mlib_s8 *)x)[i];
		((mlib_s16 *)z)[i] = mul_add;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);

#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s32 i;
	const mlib_d64 *table = mlib_SU82D64 + 128;
	mlib_d64 dc = c[0];
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	mlib_d64 mul0, mul1, mul;
	mlib_s32 m = n;

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((mlib_addr)z & 2) {
		mul = table[px[0]] + table[py[0]] * dc;
		pz[0] = (mlib_s32)mul;
		pz++;
		px++;
		py++;
		m--;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (m >> 1); i++) {
		mul0 = table[px[2 * i + 0]] + table[py[2 * i + 0]] * dc;
		mul1 = table[px[2 * i + 1]] + table[py[2 * i + 1]] * dc;

		((mlib_s32 *)pz)[i] = mul1;
		pz[2 * i] = (mlib_s32)mul0;
	}

	if (m & 1) {
		mul = table[px[m - 1]] + table[py[m - 1]] * dc;
		pz[m - 1] = (mlib_s32)mul;
	}

	return (MLIB_SUCCESS);

#endif /* MLIB_USE_FTOI_CLAMPING */
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
#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_s32 i;
	mlib_s32 mul_add, scal = *((mlib_s16 *)c);

	for (i = 0; i < n; i++) {
		mul_add = ((mlib_s16 *)y)[i] * scal + ((mlib_s16 *)x)[i];
		((mlib_s32 *)z)[i] = mul_add;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y;
	mlib_s32 *pz = (mlib_s32 *)z;
	mlib_d64 mul;
	mlib_s32 i;
	mlib_d64 dc = c[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mul = px[i] + (mlib_d64)py[i] * dc;
		pz[i] = (mlib_s32)mul;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
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
