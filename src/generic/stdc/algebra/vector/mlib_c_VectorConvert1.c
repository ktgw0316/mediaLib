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

#pragma ident	"@(#)mlib_c_VectorConvert1.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorConvert_S16_U8_Mod  - copy with expand U8->S16
 *      mlib_VectorConvert_S32_U8_Mod  - copy with expand U8->S32
 *      mlib_VectorConvert_S16_S8_Mod  - copy with expand S8->S16
 *      mlib_VectorConvert_S32_S8_Mod  - copy with expand S8->S32
 *      mlib_VectorConvert_S32_S16_Mod - copy with expand S16->S32
 *
 *      mlib_VectorConvert_S16C_U8C_Mod  - copy with expand U8->S16
 *      mlib_VectorConvert_S32C_U8C_Mod  - copy with expand U8->S32
 *      mlib_VectorConvert_S16C_S8C_Mod  - copy with expand S8->S16
 *      mlib_VectorConvert_S32C_S8C_Mod  - copy with expand S8->S32
 *      mlib_VectorConvert_S32C_S16C_Mod - copy with expand S16->S32
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorConvert_S16_U8_Mod(mlib_s16      *z,
 *                                                const mlib_u8 *x,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorConvert_S32_U8_Mod(mlib_s32      *z,
 *                                                const mlib_u8 *x,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorConvert_S16_S8_Mod(mlib_s16      *z,
 *                                                const mlib_s8 *x,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorConvert_S32_S8_Mod(mlib_s32      *z,
 *                                                const mlib_s8 *x,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorConvert_S32_S16_Mod(mlib_s32       *z,
 *                                                 const mlib_s16 *x,
 *                                                 mlib_s32       n);
 *
 *      mlib_status mlib_VectorConvert_S16C_U8C_Mod(mlib_s16      *z,
 *                                                  const mlib_u8 *x,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorConvert_S32C_U8C_Mod(mlib_s32      *z,
 *                                                  const mlib_u8 *x,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorConvert_S16C_S8C_Mod(mlib_s16      *z,
 *                                                  const mlib_s8 *x,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorConvert_S32C_S8C_Mod(mlib_s32      *z,
 *                                                  const mlib_s8 *x,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorConvert_S32C_S16C_Mod(mlib_s32       *z,
 *                                                   const mlib_s16 *x,
 *                                                   mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      x    pointer to the first element of the input vector
 *      n    number of elements in the vectors
 *
 *           for complex - xx[2*i] hold the real parts, and
 *                         xx[2*i + 1] hold the imaginary parts
 *
 * DESCRIPTION
 *      z = x
 */

#include <mlib_algebra.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorConvert_S16C_U8C_Mod = \
	__mlib_VectorConvert_S16C_U8C_Mod
#pragma weak mlib_VectorConvert_S32_S16_Mod = \
	__mlib_VectorConvert_S32_S16_Mod
#pragma weak mlib_VectorConvert_S32_S8_Mod = __mlib_VectorConvert_S32_S8_Mod
#pragma weak mlib_VectorConvert_S32_U8_Mod = __mlib_VectorConvert_S32_U8_Mod
#pragma weak mlib_VectorConvert_S16_S8_Mod = __mlib_VectorConvert_S16_S8_Mod
#pragma weak mlib_VectorConvert_S16_U8_Mod = __mlib_VectorConvert_S16_U8_Mod
#pragma weak mlib_VectorConvert_S32C_S16C_Mod = \
	__mlib_VectorConvert_S32C_S16C_Mod
#pragma weak mlib_VectorConvert_S32C_S8C_Mod = \
	__mlib_VectorConvert_S32C_S8C_Mod
#pragma weak mlib_VectorConvert_S32C_U8C_Mod = \
	__mlib_VectorConvert_S32C_U8C_Mod
#pragma weak mlib_VectorConvert_S16C_S8C_Mod = \
	__mlib_VectorConvert_S16C_S8C_Mod

#elif defined(__GNUC__)

__typeof__(__mlib_VectorConvert_S16C_U8C_Mod) mlib_VectorConvert_S16C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S16C_U8C_Mod")));
__typeof__(__mlib_VectorConvert_S32_S16_Mod) mlib_VectorConvert_S32_S16_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S32_S16_Mod")));
__typeof__(__mlib_VectorConvert_S32_S8_Mod) mlib_VectorConvert_S32_S8_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S32_S8_Mod")));
__typeof__(__mlib_VectorConvert_S32_U8_Mod) mlib_VectorConvert_S32_U8_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S32_U8_Mod")));
__typeof__(__mlib_VectorConvert_S16_S8_Mod) mlib_VectorConvert_S16_S8_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S16_S8_Mod")));
__typeof__(__mlib_VectorConvert_S16_U8_Mod) mlib_VectorConvert_S16_U8_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S16_U8_Mod")));
__typeof__(__mlib_VectorConvert_S32C_S16C_Mod) mlib_VectorConvert_S32C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S32C_S16C_Mod")));
__typeof__(__mlib_VectorConvert_S32C_S8C_Mod) mlib_VectorConvert_S32C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S32C_S8C_Mod")));
__typeof__(__mlib_VectorConvert_S32C_U8C_Mod) mlib_VectorConvert_S32C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S32C_U8C_Mod")));
__typeof__(__mlib_VectorConvert_S16C_S8C_Mod) mlib_VectorConvert_S16C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S16C_S8C_Mod")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	EXPAND(form1, form2)                                    \
	form1 *src = (form1 *) x;                               \
	form2 *ddst = (form2 *) z;                              \
	mlib_s32 i;                                             \
	                                                        \
	for (i = 0; i < n; i++)                                 \
	    (*ddst++) = (*src++);                               \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/* *********************************************************** */

#define	EXPANDC(form1, form2)                                   \
	form1 *src = (form1 *) x;                               \
	form2 *ddst = (form2 *) z;                              \
	mlib_s32 i;                                             \
	                                                        \
	for (i = 0; i < n; i++) {                               \
	    (*ddst++) = (*src++);                               \
	    (*ddst++) = (*src++);                               \
	}                                                       \
	                                                        \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S16_U8_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	EXPAND(mlib_u8,
		mlib_s16);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S32_U8_Mod(
	mlib_s32 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	EXPAND(mlib_u8,
		mlib_s32);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S16_S8_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	EXPAND(mlib_s8,
		mlib_s16);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S32_S8_Mod(
	mlib_s32 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	EXPAND(mlib_s8,
		mlib_s32);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S32_S16_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	EXPAND(mlib_s16,
		mlib_s32);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S16C_U8C_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	EXPANDC(mlib_u8,
		mlib_s16);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S32C_U8C_Mod(
	mlib_s32 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	EXPANDC(mlib_u8,
		mlib_s32);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S16C_S8C_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	EXPANDC(mlib_s8,
		mlib_s16);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S32C_S8C_Mod(
	mlib_s32 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	EXPANDC(mlib_s8,
		mlib_s32);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	EXPANDC(mlib_s16,
		mlib_s32);
}

/* *********************************************************** */
