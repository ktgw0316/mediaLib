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

#pragma ident	"@(#)mlib_s_VectorConvert2.c	9.3	07/11/05 SMI"

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

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

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
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, ax, az, nstep, n1, n2, n3;
	mlib_s16 *px = (mlib_s16 *)x;
	mlib_u8 *pz = (mlib_u8 *)z;
	__m128i zbuf, xlo, xhi, mask;
	mask = _mm_set1_epi16(0xff);

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			*pz++ = *px++;
		}
	} else {
		for (i = 0; i < n1; i++) {
			*pz++ = *px++;
		}

		for (i = 0; i < n2; i++) {
			xlo = _mm_load_si128((__m128i *)px);
			xhi = _mm_load_si128((__m128i *)px + 1);
			xlo = _mm_and_si128(xlo, mask);
			xhi = _mm_and_si128(xhi, mask);
			zbuf = _mm_packus_epi16(xlo, xhi);
			_mm_storeu_si128((__m128i *)pz, zbuf);
			px += nstep;
			pz += nstep;
		}

		for (i = 0; i < n3; i++) {
			*pz++ = *px++;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8_S16_Mod(
	mlib_s8 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_U8_S16_Mod((mlib_u8 *)z, x, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8_S32_Mod(
	mlib_u8 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, ax, az, nstep, n1, n2, n3;
	mlib_s32 *px = (mlib_s32 *)x;
	mlib_u8 *pz = (mlib_u8 *)z;
	__m128i zbuf, buf1, buf2, buf3, buf4, mask;
	mask = _mm_set1_epi32(0xff);

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s32);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			*pz++ = *px++;
		}
	} else {
		for (i = 0; i < n1; i++) {
			*pz++ = *px++;
		}

		for (i = 0; i < n2; i++) {
			buf1 = _mm_load_si128((__m128i *)px);
			buf2 = _mm_load_si128((__m128i *)px + 1);
			buf3 = _mm_load_si128((__m128i *)px + 2);
			buf4 = _mm_load_si128((__m128i *)px + 3);
			buf1 = _mm_and_si128(buf1, mask);
			buf2 = _mm_and_si128(buf2, mask);
			buf3 = _mm_and_si128(buf3, mask);
			buf4 = _mm_and_si128(buf4, mask);
			buf1 = _mm_packs_epi32(buf1, buf2);
			buf3 = _mm_packs_epi32(buf3, buf4);
			zbuf = _mm_packus_epi16(buf1, buf3);
			_mm_storeu_si128((__m128i *)pz, zbuf);
			px += nstep;
			pz += nstep;
		}

		for (i = 0; i < n3; i++) {
			*pz++ = *px++;
		}
	}

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
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, ax, az, nstep, n1, n2, n3;
	mlib_s32 *px = (mlib_s32 *)x;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i zbuf, xlo, xhi, mask;

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s32);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			*pz++ = *px++;
		}
	} else {
		for (i = 0; i < n1; i++) {
			*pz++ = *px++;
		}

		for (i = 0; i < n2; i++) {
			xlo = _mm_load_si128((__m128i *)px);
			xhi = _mm_load_si128((__m128i *)px + 1);
			xlo = _mm_slli_epi32(xlo, 16);
			xhi = _mm_slli_epi32(xhi, 16);
			xlo = _mm_srai_epi32(xlo, 16);
			xhi = _mm_srai_epi32(xhi, 16);
			zbuf = _mm_packs_epi32(xlo, xhi);
			_mm_storeu_si128((__m128i *)pz, zbuf);
			px += nstep;
			pz += nstep;
		}

		for (i = 0; i < n3; i++) {
			*pz++ = *px++;
		}
	}

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
