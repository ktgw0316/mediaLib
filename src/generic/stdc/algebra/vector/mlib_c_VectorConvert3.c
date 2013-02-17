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

#pragma ident	"@(#)mlib_c_VectorConvert3.c	9.5	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorConvert_S8_U8_Sat     - packing of 8-bit format vector
 *      mlib_VectorConvert_U8_S8_Sat     - packing of 8-bit format vector
 *      mlib_VectorConvert_U8_S16_Sat    - packing of 16-bit into 8-bit
 *                                         format vector
 *      mlib_VectorConvert_S8_S16_Sat    - packing of 16-bit into 8-bit
 *                                         format vector
 *      mlib_VectorConvert_U8_S32_Sat    - packing of 32-bit into 8-bit
 *                                         format vector
 *      mlib_VectorConvert_S8_S32_Sat    - packing of 32-bit into 8-bit
 *                                         format vector
 *      mlib_VectorConvert_S16_S32_Sat   - packing of 32-bit into 16-bit
 *                                         format vector
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorConvert_S8_U8_Sat(mlib_s8      *z,
 *                                               const mlib_u8 *x,
 *                                               mlib_s32      n);
 *      mlib_status mlib_VectorConvert_U8_S8_Sat(mlib_u8      *z,
 *                                               const mlib_s8 *x,
 *                                               mlib_s32      n);
 *      mlib_status mlib_VectorConvert_U8_S16_Sat(mlib_u8        *z,
 *                                                const mlib_s16 *x,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorConvert_S8_S16_Sat(mlib_s8        *z,
 *                                                const mlib_s16 *x,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorConvert_U8_S32_Sat(mlib_u8        *z,
 *                                                const mlib_s32 *x,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorConvert_S8_S32_Sat(mlib_s8        *z,
 *                                                const mlib_s32 *x,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorConvert_S16_S32_Sat(mlib_s16      *z,
 *                                                const mlib_s32 *x,
 *                                                mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      x    pointer to the first element of the input vector
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *      z = x
 */

/*
 * FUNCTIONS
 *  mlib_VectorConvert_S8C_U8C_Sat     - packing of 8-bit format vector
 *  mlib_VectorConvert_U8C_S8C_Sat     - packing of 8-bit format vector
 *  mlib_VectorConvert_U8C_S16C_Sat    - packing of 16-bit into 8-bit
 *                                       format vector
 *  mlib_VectorConvert_S8C_S16C_Sat    - packing of 16-bit into 8-bit
 *                                       format vector
 *  mlib_VectorConvert_U8C_S32C_Sat    - packing of 32-bit into 8-bit
 *                                       format vector
 *  mlib_VectorConvert_S8C_S32C_Sat    - packing of 32-bit into 8-bit
 *                                       format vector
 *  mlib_VectorConvert_S16C_S32C_Sat   - packing of 32-bit into 16-bit
 *                                       format vector
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorConvert_S8C_U8C_Sat(mlib_s8       *z,
 *                                                 const mlib_u8 *x,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_VectorConvert_U8C_S8C_Sat(mlib_u8       *z,
 *                                                 const mlib_s8 *x,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_VectorConvert_U8C_S16C_Sat(mlib_u8        *z,
 *                                                  const mlib_s16 *x,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_VectorConvert_S8C_S16C_Sat(mlib_s8        *z,
 *                                                  const mlib_s16 *x,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_VectorConvert_U8C_S32C_Sat(mlib_u8        *z,
 *                                                  const mlib_s32 *x,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_VectorConvert_S8C_S32C_Sat(mlib_s8        *z,
 *                                                  const mlib_s32 *x,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_VectorConvert_S16C_S32C_Sat(mlib_s16       *z,
 *                                                  const mlib_s32 *x,
 *                                                  mlib_s32       n);
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
 *      z = x
 */

#include <mlib_algebra.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorConvert_U8_S32_Sat = __mlib_VectorConvert_U8_S32_Sat
#pragma weak mlib_VectorConvert_S8_S16_Sat = __mlib_VectorConvert_S8_S16_Sat
#pragma weak mlib_VectorConvert_U8_S16_Sat = __mlib_VectorConvert_U8_S16_Sat
#pragma weak mlib_VectorConvert_S16C_S32C_Sat = \
	__mlib_VectorConvert_S16C_S32C_Sat
#pragma weak mlib_VectorConvert_U8_S8_Sat = __mlib_VectorConvert_U8_S8_Sat
#pragma weak mlib_VectorConvert_S8_U8_Sat = __mlib_VectorConvert_S8_U8_Sat
#pragma weak mlib_VectorConvert_S8C_S32C_Sat = \
	__mlib_VectorConvert_S8C_S32C_Sat
#pragma weak mlib_VectorConvert_U8C_S32C_Sat = \
	__mlib_VectorConvert_U8C_S32C_Sat
#pragma weak mlib_VectorConvert_S8C_S16C_Sat = \
	__mlib_VectorConvert_S8C_S16C_Sat
#pragma weak mlib_VectorConvert_U8C_S16C_Sat = \
	__mlib_VectorConvert_U8C_S16C_Sat
#pragma weak mlib_VectorConvert_U8C_S8C_Sat = \
	__mlib_VectorConvert_U8C_S8C_Sat
#pragma weak mlib_VectorConvert_S8C_U8C_Sat = \
	__mlib_VectorConvert_S8C_U8C_Sat
#pragma weak mlib_VectorConvert_S16_S32_Sat = \
	__mlib_VectorConvert_S16_S32_Sat
#pragma weak mlib_VectorConvert_S8_S32_Sat = __mlib_VectorConvert_S8_S32_Sat

#elif defined(__GNUC__)

__typeof__(__mlib_VectorConvert_U8_S32_Sat) mlib_VectorConvert_U8_S32_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_U8_S32_Sat")));
__typeof__(__mlib_VectorConvert_S8_S16_Sat) mlib_VectorConvert_S8_S16_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_S8_S16_Sat")));
__typeof__(__mlib_VectorConvert_U8_S16_Sat) mlib_VectorConvert_U8_S16_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_U8_S16_Sat")));
__typeof__(__mlib_VectorConvert_S16C_S32C_Sat) mlib_VectorConvert_S16C_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_S16C_S32C_Sat")));
__typeof__(__mlib_VectorConvert_U8_S8_Sat) mlib_VectorConvert_U8_S8_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_U8_S8_Sat")));
__typeof__(__mlib_VectorConvert_S8_U8_Sat) mlib_VectorConvert_S8_U8_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_S8_U8_Sat")));
__typeof__(__mlib_VectorConvert_S8C_S32C_Sat) mlib_VectorConvert_S8C_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_S8C_S32C_Sat")));
__typeof__(__mlib_VectorConvert_U8C_S32C_Sat) mlib_VectorConvert_U8C_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_U8C_S32C_Sat")));
__typeof__(__mlib_VectorConvert_S8C_S16C_Sat) mlib_VectorConvert_S8C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_S8C_S16C_Sat")));
__typeof__(__mlib_VectorConvert_U8C_S16C_Sat) mlib_VectorConvert_U8C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_U8C_S16C_Sat")));
__typeof__(__mlib_VectorConvert_U8C_S8C_Sat) mlib_VectorConvert_U8C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_U8C_S8C_Sat")));
__typeof__(__mlib_VectorConvert_S8C_U8C_Sat) mlib_VectorConvert_S8C_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_S8C_U8C_Sat")));
__typeof__(__mlib_VectorConvert_S16_S32_Sat) mlib_VectorConvert_S16_S32_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_S16_S32_Sat")));
__typeof__(__mlib_VectorConvert_S8_S32_Sat) mlib_VectorConvert_S8_S32_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_S8_S32_Sat")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

/*
 *
 * Copy with control for operands of the same format
 */
#define	PACK_S_U(form_src, form_dst)                            \
	const form_src *src = x;                                \
	form_dst *dst = z;                                      \
	mlib_s32 i;                                             \
	form_src c;                                             \
	                                                        \
	for (i = 0; i < n; i++) {                               \
	    dst[i] = (c = src[i]) < 0 ? 0 : c;                  \
	}                                                       \
	                                                        \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/*
 *
 * Copy with control for operands of the same format
 *
 */
#define	PACK_U_S(form_src, form_dst, max_dst)                   \
	const form_src *src = x;                                \
	form_dst *dst = z;                                      \
	form_src c;                                             \
	mlib_s32 i;                                             \
	                                                        \
	for (i = 0; i < n; i++) {                               \
	    dst[i] = (c = src[i]) > max_dst ? max_dst : c;      \
	}                                                       \
	                                                        \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/*
 *
 * Copy with control for operands of different format
 *
 */
#define	PACK_S_U_DF(form_src, form_dst, max_dst, min_dst)       \
	const form_src *src = x;                                \
	form_dst *dst = z;                                      \
	mlib_s32 i;                                             \
	form_src c;                                             \
	                                                        \
	for (i = 0; i < n; i++) {                               \
	    dst[i] = (c =                                       \
		    src[i]) < min_dst ? min_dst : (c >          \
		    max_dst ? max_dst : c);                     \
	}                                                       \
	                                                        \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/*
 *
 * Copy with control for operands of the same format
 *
 */
#define	PACK_S_S(form_src, form_dst, max_dst, min_dst)          \
	const form_src *src = x;                                \
	form_dst *dst = z;                                      \
	mlib_s32 i;                                             \
	form_src c;                                             \
	                                                        \
	for (i = 0; i < n; i++) {                               \
	    dst[i] = (c =                                       \
		    src[i]) < min_dst ? min_dst : (c >          \
		    max_dst ? max_dst : c);                     \
	}                                                       \
	                                                        \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/*
 *
 * The same macros for Complex operands
 *
 */

#define	PACKC_S_U(form_src, form_dst)                           \
	const form_src *src = x;                                \
	form_dst *dst = z;                                      \
	mlib_s32 i;                                             \
	form_src c;                                             \
	                                                        \
	for (i = 0; i < n * 2; i++) {                           \
	    dst[i] = (c = src[i]) < 0 ? 0 : c;                  \
	}                                                       \
	                                                        \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/* *********************************************************** */

#define	PACKC_U_S(form_src, form_dst, max_dst)                  \
	const form_src *src = x;                                \
	form_dst *dst = z;                                      \
	form_src c;                                             \
	mlib_s32 i;                                             \
	                                                        \
	for (i = 0; i < n * 2; i++) {                           \
	    dst[i] = (c = src[i]) > max_dst ? max_dst : c;      \
	}                                                       \
	                                                        \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/* *********************************************************** */

#define	PACKC_S_U_DF(form_src, form_dst, max_dst, min_dst)      \
	const form_src *src = x;                                \
	form_dst *dst = z;                                      \
	mlib_s32 i;                                             \
	form_src c;                                             \
	                                                        \
	for (i = 0; i < n * 2; i++) {                           \
	    dst[i] = (c =                                       \
		    src[i]) < min_dst ? min_dst : (c >          \
		    max_dst ? max_dst : c);                     \
	}                                                       \
	                                                        \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/* *********************************************************** */

#define	PACKC_S_S(form_src, form_dst, max_dst, min_dst)         \
	const form_src *src = x;                                \
	form_dst *dst = z;                                      \
	mlib_s32 i;                                             \
	form_src c;                                             \
	                                                        \
	for (i = 0; i < n * 2; i++) {                           \
	    dst[i] = (c =                                       \
		    src[i]) < min_dst ? min_dst : (c >          \
		    max_dst ? max_dst : c);                     \
	}                                                       \
	                                                        \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8_U8_Sat(
	mlib_s8 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	PACK_U_S(mlib_u8,
		mlib_s8,
		MLIB_S8_MAX);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8_S8_Sat(
	mlib_u8 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	PACK_S_U(mlib_s8,
		mlib_u8);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8_S16_Sat(
	mlib_u8 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	PACK_S_U_DF(mlib_s16,
		mlib_u8,
		MLIB_U8_MAX,
		0);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8_S16_Sat(
	mlib_s8 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	PACK_S_S(mlib_s16,
		mlib_s8,
		MLIB_S8_MAX,
		MLIB_S8_MIN);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8_S32_Sat(
	mlib_u8 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	PACK_S_U_DF(mlib_s32,
		mlib_u8,
		MLIB_U8_MAX,
		0);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8_S32_Sat(
	mlib_s8 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	PACK_S_S(mlib_s32,
		mlib_s8,
		MLIB_S8_MAX,
		MLIB_S8_MIN);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S16_S32_Sat(
	mlib_s16 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	PACK_S_S(mlib_s32,
		mlib_s16,
		MLIB_S16_MAX,
		MLIB_S16_MIN);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8C_U8C_Sat(
	mlib_s8 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	PACKC_U_S(mlib_u8,
		mlib_s8,
		MLIB_S8_MAX);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8C_S8C_Sat(
	mlib_u8 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	PACKC_S_U(mlib_s8,
		mlib_u8);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8C_S16C_Sat(
	mlib_u8 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	PACKC_S_U_DF(mlib_s16,
		mlib_u8,
		MLIB_U8_MAX,
		0);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8C_S16C_Sat(
	mlib_s8 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	PACKC_S_S(mlib_s16,
		mlib_s8,
		MLIB_S8_MAX,
		MLIB_S8_MIN);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8C_S32C_Sat(
	mlib_u8 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	PACKC_S_U_DF(mlib_s32,
		mlib_u8,
		MLIB_U8_MAX,
		0);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8C_S32C_Sat(
	mlib_s8 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	PACKC_S_S(mlib_s32,
		mlib_s8,
		MLIB_S8_MAX,
		MLIB_S8_MIN);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S16C_S32C_Sat(
	mlib_s16 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	PACKC_S_S(mlib_s32,
		mlib_s16,
		MLIB_S16_MAX,
		MLIB_S16_MIN);
}

/* *********************************************************** */
