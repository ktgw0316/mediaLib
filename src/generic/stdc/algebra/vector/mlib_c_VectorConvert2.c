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

#pragma ident	"@(#)mlib_c_VectorConvert2.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorConvert_S8_U8_Mod     - packing of 8-bit format vector
 *      mlib_VectorConvert_U8_S8_Mod     - packing of 8-bit format vector
 *      mlib_VectorConvert_U8_S16_Mod    - packing of 16-bit into 8-bit
 *                                         format vector
 *      mlib_VectorConvert_S8_S16_Mod    - packing of 16-bit into 8-bit
 *                                         format vector
 *      mlib_VectorConvert_U8_S32_Mod    - packing of 32-bit into 8-bit
 *                                         format vector
 *      mlib_VectorConvert_S8_S32_Mod    - packing of 32-bit into 8-bit
 *                                         format vector
 *      mlib_VectorConvert_S16_S32_Mod   - packing of 32-bit into 16-bit
 *                                         format vector
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorConvert_S8_U8_Mod(mlib_s8        *z,
 *                                               const  mlib_u8 *x,
 *                                               mlib_s32       n);
 *      mlib_status mlib_VectorConvert_U8_S8_Mod(mlib_u8       *z,
 *                                               const mlib_s8 *x,
 *                                               mlib_s32      n);
 *      mlib_status mlib_VectorConvert_U8_S16_Mod(mlib_u8        *z,
 *                                                const mlib_s16 *x,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorConvert_S8_S16_Mod(mlib_s8        *z,
 *                                                const mlib_s16 *x,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorConvert_U8_S32_Mod(mlib_u8        *z,
 *                                                const mlib_s32 *x,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorConvert_S8_S32_Mod(mlib_s8        *z,
 *                                                const mlib_s32 *x,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorConvert_S16_S32_Mod(mlib_s16       *z,
 *                                                 const mlib_s32 *x,
 *                                                 mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      x    pointer to the first element of the input vector
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *      z = x
 *
 * IMPLEMENTATION
 *
 *      For 8->8 bits transformation we simply use mlib_Vector_Copy_U8_U8.
 *
 *      For 32->8 bits packing we use VIS instructions for composing of the
 *      resulting mlib_d64 word (8 elements).
 *      We read source vector 8 times by 1 byte
 *      and then form a result by fis_to_double(op1, op2), where op1 and op2
 *      are obtained by logical multiplication and shift.
 *
 *      For 32->16 bits packing we don't use VIS at all.
 *      The most quickly way is
 *      to read source vector 4 times by 1/2 word and
 *      to obtain 2*32-bit words by logical operations.
 *
 *      In all cases we've made unrolling of loops for 2 iterations.
 *
 *      All functions of Complex arguments were obtained from the corresponding
 *      functions of real arguments.
 */

/*
 * FUNCTIONS
 *      mlib_VectorConvert_S8C_U8C_Mod     - packing of 8-bit format vector
 *      mlib_VectorConvert_U8C_S8C_Mod     - packing of 8-bit format vector
 *      mlib_VectorConvert_U8C_S16C_Mod    - packing of 16-bit into 8-bit
 *                                           format vector
 *      mlib_VectorConvert_S8C_S16C_Mod    - packing of 16-bit into 8-bit
 *                                           format vector
 *      mlib_VectorConvert_U8C_S32C_Mod    - packing of 32-bit into 8-bit
 *                                           format vector
 *      mlib_VectorConvert_S8C_S32C_Mod    - packing of 32-bit into 8-bit
 *                                           format vector
 *      mlib_VectorConvert_S16C_S32C_Mod   - packing of 32-bit into 16-bit
 *                                           format vector
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorConvert_S8C_U8C_Mod(mlib_s8       *z,
 *                                                 const mlib_u8 *x,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_VectorConvert_U8C_S8C_Mod(mlib_u8       *z,
 *                                                 const mlib_s8 *x,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_VectorConvert_U8C_S16C_Mod(mlib_u8        *z,
 *                                                  const mlib_s16 *x,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_VectorConvert_S8C_S16C_Mod(mlib_s8        *z,
 *                                                  const mlib_s16 *x,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_VectorConvert_U8C_S32C_Mod(mlib_u8        *z,
 *                                                  const mlib_s32 *x,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_VectorConvert_S8C_S32C_Mod(mlib_s8        *z,
 *                                                  const mlib_s32 *x,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_VectorConvert_S16C_S32C_Mod(mlib_s16       *z,
 *                                                   const mlib_s32 *x,
 *                                                   mlib_s32       n);
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
 *
 * IMPLEMENTATION
 *
 *      For 8->8 bits transformation we simply use mlib_Vector_Copy_U8_U8.
 *
 *      For 32->8 bits packing we use VIS instructions for composing of the
 *      resulting mlib_d64 word (8 elements).
 *      We read source vector 8 times by 1 byte
 *      and then form a result by fis_to_double(op1, op2), where op1 and op2
 *      are obtained by logical multiplication and shift.
 *
 *      For 32->16 bits packing we don't use VIS at all.
 *      The most quickly way is
 *      to read source vector 4 times by 1/2 word and
 *      to obtain 2*32-bit words by logical operations.
 *
 *      In all cases we've made unrolling of loops for 2 iterations.
 *
 *      All functions of Complex arguments were obtained from the corresponding
 *      functions of real arguments.
 */

#include <mlib_algebra.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorConvert_U8C_S16C_Mod = \
	__mlib_VectorConvert_U8C_S16C_Mod
#pragma weak mlib_VectorConvert_U8C_S8C_Mod = \
	__mlib_VectorConvert_U8C_S8C_Mod
#pragma weak mlib_VectorConvert_S8C_U8C_Mod = \
	__mlib_VectorConvert_S8C_U8C_Mod
#pragma weak mlib_VectorConvert_S16_S32_Mod = \
	__mlib_VectorConvert_S16_S32_Mod
#pragma weak mlib_VectorConvert_S8_S32_Mod = __mlib_VectorConvert_S8_S32_Mod
#pragma weak mlib_VectorConvert_S8_S16_Mod = __mlib_VectorConvert_S8_S16_Mod
#pragma weak mlib_VectorConvert_U8_S32_Mod = __mlib_VectorConvert_U8_S32_Mod
#pragma weak mlib_VectorConvert_U8_S16_Mod = __mlib_VectorConvert_U8_S16_Mod
#pragma weak mlib_VectorConvert_S16C_S32C_Mod = \
	__mlib_VectorConvert_S16C_S32C_Mod
#pragma weak mlib_VectorConvert_S8_U8_Mod = __mlib_VectorConvert_S8_U8_Mod
#pragma weak mlib_VectorConvert_U8_S8_Mod = __mlib_VectorConvert_U8_S8_Mod
#pragma weak mlib_VectorConvert_S8C_S32C_Mod = \
	__mlib_VectorConvert_S8C_S32C_Mod
#pragma weak mlib_VectorConvert_U8C_S32C_Mod = \
	__mlib_VectorConvert_U8C_S32C_Mod
#pragma weak mlib_VectorConvert_S8C_S16C_Mod = \
	__mlib_VectorConvert_S8C_S16C_Mod

#elif defined(__GNUC__)

__typeof__(__mlib_VectorConvert_U8C_S16C_Mod) mlib_VectorConvert_U8C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_U8C_S16C_Mod")));
__typeof__(__mlib_VectorConvert_U8C_S8C_Mod) mlib_VectorConvert_U8C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_U8C_S8C_Mod")));
__typeof__(__mlib_VectorConvert_S8C_U8C_Mod) mlib_VectorConvert_S8C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S8C_U8C_Mod")));
__typeof__(__mlib_VectorConvert_S16_S32_Mod) mlib_VectorConvert_S16_S32_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S16_S32_Mod")));
__typeof__(__mlib_VectorConvert_S8_S32_Mod) mlib_VectorConvert_S8_S32_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S8_S32_Mod")));
__typeof__(__mlib_VectorConvert_S8_S16_Mod) mlib_VectorConvert_S8_S16_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S8_S16_Mod")));
__typeof__(__mlib_VectorConvert_U8_S32_Mod) mlib_VectorConvert_U8_S32_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_U8_S32_Mod")));
__typeof__(__mlib_VectorConvert_U8_S16_Mod) mlib_VectorConvert_U8_S16_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_U8_S16_Mod")));
__typeof__(__mlib_VectorConvert_S16C_S32C_Mod) mlib_VectorConvert_S16C_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S16C_S32C_Mod")));
__typeof__(__mlib_VectorConvert_S8_U8_Mod) mlib_VectorConvert_S8_U8_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S8_U8_Mod")));
__typeof__(__mlib_VectorConvert_U8_S8_Mod) mlib_VectorConvert_U8_S8_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_U8_S8_Mod")));
__typeof__(__mlib_VectorConvert_S8C_S32C_Mod) mlib_VectorConvert_S8C_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S8C_S32C_Mod")));
__typeof__(__mlib_VectorConvert_U8C_S32C_Mod) mlib_VectorConvert_U8C_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_U8C_S32C_Mod")));
__typeof__(__mlib_VectorConvert_S8C_S16C_Mod) mlib_VectorConvert_S8C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S8C_S16C_Mod")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

/*
 *
 * Simple defines for C implementation.
 * Assignment statement in loop for operands of different
 * formats.
 */

#define	PACK(form_src, form_dst)                                \
	const form_src *src = x;                                \
	form_dst *dst = z;                                      \
	mlib_s32 i;                                             \
	                                                        \
	for (i = 0; i < n; i++) {                               \
	    dst[i] = src[i];                                    \
	}                                                       \
	                                                        \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8_U8_Mod(
	mlib_s8 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	return (__mlib_VectorCopy_U8((mlib_u8 *)z, x, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8_S8_Mod(
	mlib_u8 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	return (__mlib_VectorCopy_U8(z, (mlib_u8 *)x, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8_S16_Mod(
	mlib_u8 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	PACK(mlib_s16,
		mlib_u8);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8_S16_Mod(
	mlib_s8 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	PACK(mlib_s16,
		mlib_s8);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8_S32_Mod(
	mlib_u8 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	const mlib_s32 *src = x;
	mlib_u8 *dst = z, *u8src;
	mlib_s32 *dst_32;
	mlib_s32 i1, i2, i3, i4, i5, i6, i7, i8;
	mlib_s32 len_64, even_length, rest_64, length = n, i;

	if (n < 16) {
		PACK(mlib_s32,
			mlib_u8);
	}

/* First try to align for 4 bytes destination address. */

	while ((mlib_addr)dst & 3) {
		(*dst++) = (*src++);
		length--;
	}

	rest_64 = length & 7;
	len_64 = length >> 3;
	even_length = len_64 << 3;
	dst_32 = (mlib_s32 *)dst;

	u8src = (mlib_u8 *)src;

#ifndef _LITTLE_ENDIAN

	u8src += 3;

#endif /* _LITTLE_ENDIAN */

	if (i = (len_64 & 1)) {
		i1 = *(u8src);
		i2 = *(u8src + 4);
		i3 = *(u8src + 8);
		i4 = *(u8src + 12);
		i5 = *(u8src + 16);
		i6 = *(u8src + 20);
		i7 = *(u8src + 24);
		i8 = *(u8src + 28);
#ifndef _LITTLE_ENDIAN
		(*dst_32++) = ((i1 << 24) | (i2 << 16) | (i3 << 8) | (i4));
		(*dst_32++) = ((i5 << 24) | (i6 << 16) | (i7 << 8) | (i8));
#else
		(*dst_32++) = ((i4 << 24) | (i3 << 16) | (i2 << 8) | (i1));
		(*dst_32++) = ((i8 << 24) | (i7 << 16) | (i6 << 8) | (i5));
#endif /* _LITTLE_ENDIAN */
		u8src += 32;
	}

	for (; i < len_64; i += 2) {
		i1 = *(u8src);
		i2 = *(u8src + 4);
		i3 = *(u8src + 8);
		i4 = *(u8src + 12);
		i5 = *(u8src + 16);
		i6 = *(u8src + 20);
		i7 = *(u8src + 24);
		i8 = *(u8src + 28);
#ifndef _LITTLE_ENDIAN
		(*dst_32++) = ((i1 << 24) | (i2 << 16) | (i3 << 8) | (i4));
		(*dst_32++) = ((i5 << 24) | (i6 << 16) | (i7 << 8) | (i8));
#else
		(*dst_32++) = ((i4 << 24) | (i3 << 16) | (i2 << 8) | (i1));
		(*dst_32++) = ((i8 << 24) | (i7 << 16) | (i6 << 8) | (i5));
#endif /* _LITTLE_ENDIAN */
		u8src += 32;
		i1 = *(u8src);
		i2 = *(u8src + 4);
		i3 = *(u8src + 8);
		i4 = *(u8src + 12);
		i5 = *(u8src + 16);
		i6 = *(u8src + 20);
		i7 = *(u8src + 24);
		i8 = *(u8src + 28);
#ifndef _LITTLE_ENDIAN
		(*dst_32++) = ((i1 << 24) | (i2 << 16) | (i3 << 8) | (i4));
		(*dst_32++) = ((i5 << 24) | (i6 << 16) | (i7 << 8) | (i8));
#else
		(*dst_32++) = ((i4 << 24) | (i3 << 16) | (i2 << 8) | (i1));
		(*dst_32++) = ((i8 << 24) | (i7 << 16) | (i6 << 8) | (i5));
#endif /* _LITTLE_ENDIAN */
		u8src += 32;
	}

	for (i = 0; i < rest_64; i++)
		dst[even_length + i] = *(u8src + (i << 2));

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8_S32_Mod(
	mlib_s8 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_U8_S32_Mod((mlib_u8 *)z, x, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S16_S32_Mod(
	mlib_s16 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	const mlib_s32 *src = x;
	mlib_s32 *idst;
	mlib_s16 *dst = z;
	mlib_u16 *u16src;
	mlib_s32 i1, i2, i3, i4, i5, i6, i7, i8;
	mlib_s32 len_64, even_length, rest_64, length = n, i;

	if (n < 16) {
		PACK(mlib_s32,
			mlib_s16);
	}

/* First try to align destination address for 4 bytes . */

	while ((mlib_addr)dst & 3) {
		(*dst++) = (*src++);
		length--;
	}

	rest_64 = length & 3;
	len_64 = length >> 2;
	even_length = len_64 << 2;
	u16src = (mlib_u16 *)src;

#ifndef _LITTLE_ENDIAN

	u16src++;

#endif /* _LITTLE_ENDIAN */

	idst = (mlib_s32 *)dst;

	if (i = (len_64 & 1)) {
		i1 = *(u16src);
		i2 = *(u16src + 2);
		i3 = *(u16src + 4);
		i4 = *(u16src + 6);
#ifndef _LITTLE_ENDIAN
		(*idst++) = ((i1 << 16) | (i2));
		(*idst++) = ((i3 << 16) | (i4));
#else
		(*idst++) = ((i2 << 16) | (i1));
		(*idst++) = ((i4 << 16) | (i3));
#endif /* _LITTLE_ENDIAN */
		u16src += 8;
	}

	for (; i < len_64; i += 2) {
		i1 = *(u16src);
		i2 = *(u16src + 2);
		i3 = *(u16src + 4);
		i4 = *(u16src + 6);
		i5 = *(u16src + 8);
		i6 = *(u16src + 10);
		i7 = *(u16src + 12);
		i8 = *(u16src + 14);
#ifndef _LITTLE_ENDIAN
		(*idst++) = ((i1 << 16) | (i2));
		(*idst++) = ((i3 << 16) | (i4));
		(*idst++) = ((i5 << 16) | (i6));
		(*idst++) = ((i7 << 16) | (i8));
#else
		(*idst++) = ((i2 << 16) | (i1));
		(*idst++) = ((i4 << 16) | (i3));
		(*idst++) = ((i6 << 16) | (i5));
		(*idst++) = ((i8 << 16) | (i7));
#endif /* _LITTLE_ENDIAN */
		u16src += 16;
	}

	for (i = 0; i < rest_64; i++)
		dst[even_length + i] = src[even_length + i];

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8C_U8C_Mod(
	mlib_s8 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	return (__mlib_VectorCopy_U8((mlib_u8 *)z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8C_S8C_Mod(
	mlib_u8 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	return (__mlib_VectorCopy_U8(z, (mlib_u8 *)x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8C_S16C_Mod(
	mlib_u8 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_U8_S16_Mod(z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8C_S16C_Mod(
	mlib_s8 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_U8_S16_Mod((mlib_u8 *)z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8C_S32C_Mod(
	mlib_u8 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_U8_S32_Mod(z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8C_S32C_Mod(
	mlib_s8 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_U8_S32_Mod((mlib_u8 *)z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S16C_S32C_Mod(
	mlib_s16 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_S16_S32_Mod(z, x, n + n));
}

/* *********************************************************** */
