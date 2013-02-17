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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_VectorReverseByteOrder.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorReverseByteOrder_S16_S16 - change the endianness
 *      mlib_VectorReverseByteOrder_U16_U16 - change the endianness
 *      mlib_VectorReverseByteOrder_S32_S32 - change the endianness
 *      mlib_VectorReverseByteOrder_U32_U32 - change the endianness
 *      mlib_VectorReverseByteOrder_S64_S64 - change the endianness
 *      mlib_VectorReverseByteOrder_U64_U64 - change the endianness
 *      mlib_VectorReverseByteOrder_F32_F32 - change the endianness
 *      mlib_VectorReverseByteOrder_D64_D64 - change the endianness
 *      mlib_VectorReverseByteOrder         - change the endianness
 *      mlib_VectorReverseByteOrder_S16     - change the endianness
 *      mlib_VectorReverseByteOrder_U16     - change the endianness
 *      mlib_VectorReverseByteOrder_S32     - change the endianness
 *      mlib_VectorReverseByteOrder_U32     - change the endianness
 *      mlib_VectorReverseByteOrder_S64     - change the endianness
 *      mlib_VectorReverseByteOrder_U64     - change the endianness
 *      mlib_VectorReverseByteOrder_F32     - change the endianness
 *      mlib_VectorReverseByteOrder_D64     - change the endianness
 *      mlib_VectorReverseByteOrder_Inp     - change the endianness
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorReverseByteOrder_S16_S16(mlib_s16       *z,
 *                                                      const mlib_s16 *x,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorReverseByteOrder_U16_U16(mlib_u16       *z,
 *                                                      const mlib_u16 *x,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorReverseByteOrder_S32_S32(mlib_s32       *z,
 *                                                      const mlib_s32 *x,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorReverseByteOrder_U32_U32(mlib_u32       *z,
 *                                                      const mlib_u32 *x,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorReverseByteOrder_S64_S64(mlib_s64       *z,
 *                                                      const mlib_s64 *x,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorReverseByteOrder_U64_U64(mlib_u64       *z,
 *                                                      const mlib_u64 *x,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorReverseByteOrder_F32_F32(mlib_f32       *z,
 *                                                      const mlib_f32 *x,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorReverseByteOrder_D64_D64(mlib_d64       *z,
 *                                                      const mlib_d64 *x,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorReverseByteOrder(void       *z,
 *                                              const void *x,
 *                                              mlib_s32   n,
 *                                              mlib_s32   s);
 *      mlib_status mlib_VectorReverseByteOrder_S16(mlib_s16 *xz,
 *                                                  mlib_s32  n);
 *      mlib_status mlib_VectorReverseByteOrder_U16(mlib_u16 *xz,
 *                                                  mlib_s32  n);
 *      mlib_status mlib_VectorReverseByteOrder_S32(mlib_s32 *xz,
 *                                                  mlib_s32  n);
 *      mlib_status mlib_VectorReverseByteOrder_U32(mlib_u32 *xz,
 *                                                  mlib_s32  n);
 *      mlib_status mlib_VectorReverseByteOrder_S64(mlib_s64 *xz,
 *                                                  mlib_s32  n);
 *      mlib_status mlib_VectorReverseByteOrder_U64(mlib_u64 *xz,
 *                                                  mlib_s32  n);
 *      mlib_status mlib_VectorReverseByteOrder_F32(mlib_f32 *xz,
 *                                                  mlib_s32  n);
 *      mlib_status mlib_VectorReverseByteOrder_D64(mlib_d64 *xz,
 *                                                  mlib_s32  n);
 *      mlib_status mlib_VectorReverseByteOrder_Inp(void     *xz,
 *                                                  mlib_s32 n,
 *                                                  mlib_s32 s);
 *
 * ARGUMENT
 *      z    pointer to output vector
 *      x    pointer to input vector
 *      xz   pointer to input and output vector
 *      n    number of elements in the vectors
 *      s    size of elements in bytes
 *
 * DESCRIPTION
 *      Change the encoding of each element from big endian to
 *      little endian, or from little endian to big endian.
 *
 * RESTRICTION
 *      The vector element size can be from 2 to 8 bytes for
 *      mlib_VectorReverseByteOrder{_Inp} functions.
 */

#include <mlib_VectorReverseByteOrder.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorReverseByteOrder_Inp = \
	__mlib_VectorReverseByteOrder_Inp
#pragma weak mlib_VectorReverseByteOrder_U64 = \
	__mlib_VectorReverseByteOrder_U64
#pragma weak mlib_VectorReverseByteOrder_U16_U16 = \
	__mlib_VectorReverseByteOrder_U16_U16
#pragma weak mlib_VectorReverseByteOrder = __mlib_VectorReverseByteOrder
#pragma weak mlib_VectorReverseByteOrder_U32 = \
	__mlib_VectorReverseByteOrder_U32
#pragma weak mlib_VectorReverseByteOrder_U32_U32 = \
	__mlib_VectorReverseByteOrder_U32_U32
#pragma weak mlib_VectorReverseByteOrder_U16 = \
	__mlib_VectorReverseByteOrder_U16
#pragma weak mlib_VectorReverseByteOrder_U64_U64 = \
	__mlib_VectorReverseByteOrder_U64_U64

#elif defined(__GNUC__)

__typeof__(__mlib_VectorReverseByteOrder_Inp) mlib_VectorReverseByteOrder_Inp
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_Inp")));
__typeof__(__mlib_VectorReverseByteOrder_U64) mlib_VectorReverseByteOrder_U64
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_U64")));
__typeof__(__mlib_VectorReverseByteOrder_U16_U16)
	mlib_VectorReverseByteOrder_U16_U16
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_U16_U16")));
__typeof__(__mlib_VectorReverseByteOrder) mlib_VectorReverseByteOrder
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder")));
__typeof__(__mlib_VectorReverseByteOrder_U32) mlib_VectorReverseByteOrder_U32
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_U32")));
__typeof__(__mlib_VectorReverseByteOrder_U32_U32)
	mlib_VectorReverseByteOrder_U32_U32
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_U32_U32")));
__typeof__(__mlib_VectorReverseByteOrder_U16) mlib_VectorReverseByteOrder_U16
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_U16")));
__typeof__(__mlib_VectorReverseByteOrder_U64_U64)
	mlib_VectorReverseByteOrder_U64_U64
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_U64_U64")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VectorReverseByteOrder_U16_U16(
	mlib_u16 *z,
	const mlib_u16 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	const mlib_u16 *px = x;
	mlib_u16 c, *pz = z;
	mlib_s32 i, ax, az, n1, n2, n3, nstep;
	__m128i xbuf, zbuf, buf1, buf2;

	ax = (mlib_addr)px & 15;
	az = (mlib_addr)pz & 15;
	nstep = 16 / sizeof (mlib_u16);

	n1 = ((16 - ax) & 15) / sizeof (mlib_u16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			c = *px++;
			c = (c << 8) | (c >> 8);
			*pz++ = c;
		}
	} else {
		for (i = 0; i < n1; i++) {
			c = *px++;
			c = (c << 8) | (c >> 8);
			*pz++ = c;
		}

		if (ax == az) {
			for (i = 0; i < n2; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				buf1 = _mm_slli_epi16(xbuf, 8);
				buf2 = _mm_srli_epi16(xbuf, 8);
				zbuf = _mm_or_si128(buf1, buf2);
				_mm_store_si128((__m128i *)pz, zbuf);
				px += nstep;
				pz += nstep;
			}
		} else {
			for (i = 0; i < n2; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				buf1 = _mm_slli_epi16(xbuf, 8);
				buf2 = _mm_srli_epi16(xbuf, 8);
				zbuf = _mm_or_si128(buf1, buf2);
				_mm_storeu_si128((__m128i *)pz, zbuf);
				px += nstep;
				pz += nstep;
			}
		}
		for (i = 0; i < n3; i++) {
			c = *px++;
			c = (c << 8) | (c >> 8);
			*pz++ = c;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorReverseByteOrder_U32_U32(
	mlib_u32 *z,
	const mlib_u32 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	const mlib_u32 *px = x;
	mlib_u32 c, *pz = z;
	mlib_s32 i, ax, az, n1, n2, n3, nstep;
	__m128i xbuf, zbuf, buf1, buf2;

	ax = (mlib_addr)px & 15;
	az = (mlib_addr)pz & 15;
	nstep = 16 / sizeof (mlib_u32);

	n1 = ((16 - ax) & 15) / sizeof (mlib_u32);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			c = *px++;
			c = (c << 24) | ((c << 8) & 0x00ff0000)
				| ((c >> 8) & 0x0000ff00) | (c >> 24);
			*pz++ = c;
		}
	} else {
		for (i = 0; i < n1; i++) {
			c = *px++;
			c = (c << 24) | ((c << 8) & 0x00ff0000)
				| ((c >> 8) & 0x0000ff00) | (c >> 24);
			*pz++ = c;
		}

		if (ax == az) {
			for (i = 0; i < n2; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				buf1 = _mm_slli_epi32(xbuf, 16);
				buf2 = _mm_srli_epi32(xbuf, 16);
				zbuf = _mm_or_si128(buf1, buf2);
				buf1 = _mm_slli_epi16(zbuf, 8);
				buf2 = _mm_srli_epi16(zbuf, 8);
				zbuf = _mm_or_si128(buf1, buf2);
				_mm_store_si128((__m128i *)pz, zbuf);
				px += nstep;
				pz += nstep;
			}
		} else {
			for (i = 0; i < n2; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				buf1 = _mm_slli_epi32(xbuf, 16);
				buf2 = _mm_srli_epi32(xbuf, 16);
				zbuf = _mm_or_si128(buf1, buf2);
				buf1 = _mm_slli_epi16(zbuf, 8);
				buf2 = _mm_srli_epi16(zbuf, 8);
				zbuf = _mm_or_si128(buf1, buf2);
				_mm_storeu_si128((__m128i *)pz, zbuf);
				px += nstep;
				pz += nstep;
			}
		}
		for (i = 0; i < n3; i++) {
			c = *px++;
			c = (c << 24) | ((c << 8) & 0x00ff0000)
				| ((c >> 8) & 0x0000ff00) | (c >> 24);
			*pz++ = c;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorReverseByteOrder_U64_U64(
	mlib_u64 *z,
	const mlib_u64 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	const mlib_u32 *px = (const mlib_u32 *)x;
	mlib_u32 c0, c1, *pz = (mlib_u32 *)z;
	mlib_s32 i, ax, az, n1, n2, n3, nstep;
	__m128i xbuf, zbuf, buf1, buf2;

	ax = (mlib_addr)px & 15;
	az = (mlib_addr)pz & 15;
	nstep = 16 / sizeof (mlib_u32);

	n1 = ((16 - ax) & 15) / sizeof (mlib_u32);
	n2 = (n + n - n1) / nstep;
	n3 = n + n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			c0 = *px++;
			c1 = *px++;
			c0 = (c0 << 24) | ((c0 << 8) & 0x00ff0000)
				| ((c0 >> 8) & 0x0000ff00) | (c0 >> 24);
			c1 = (c1 << 24) | ((c1 << 8) & 0x00ff0000)
				| ((c1 >> 8) & 0x0000ff00) | (c1 >> 24);
			*pz++ = c1;
			*pz++ = c0;
		}
	} else {
		for (i = 0; i < n1 / 2; i++) {
			c0 = *px++;
			c1 = *px++;
			c0 = (c0 << 24) | ((c0 << 8) & 0x00ff0000)
				| ((c0 >> 8) & 0x0000ff00) | (c0 >> 24);
			c1 = (c1 << 24) | ((c1 << 8) & 0x00ff0000)
				| ((c1 >> 8) & 0x0000ff00) | (c1 >> 24);
			*pz++ = c1;
			*pz++ = c0;
		}

		if (ax == az) {
			for (i = 0; i < n2; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				buf1 = _mm_slli_epi64(xbuf, 32);
				buf2 = _mm_srli_epi64(xbuf, 32);
				zbuf = _mm_or_si128(buf1, buf2);
				buf1 = _mm_slli_epi32(zbuf, 16);
				buf2 = _mm_srli_epi32(zbuf, 16);
				zbuf = _mm_or_si128(buf1, buf2);
				buf1 = _mm_slli_epi16(zbuf, 8);
				buf2 = _mm_srli_epi16(zbuf, 8);
				zbuf = _mm_or_si128(buf1, buf2);
				_mm_store_si128((__m128i *)pz, zbuf);
				px += nstep;
				pz += nstep;
			}
		} else {
			for (i = 0; i < n2; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				buf1 = _mm_slli_epi64(xbuf, 32);
				buf2 = _mm_srli_epi64(xbuf, 32);
				zbuf = _mm_or_si128(buf1, buf2);
				buf1 = _mm_slli_epi32(zbuf, 16);
				buf2 = _mm_srli_epi32(zbuf, 16);
				zbuf = _mm_or_si128(buf1, buf2);
				buf1 = _mm_slli_epi16(zbuf, 8);
				buf2 = _mm_srli_epi16(zbuf, 8);
				zbuf = _mm_or_si128(buf1, buf2);
				_mm_storeu_si128((__m128i *)pz, zbuf);
				px += nstep;
				pz += nstep;
			}
		}
		for (i = 0; i < n3 / 2; i++) {
			c0 = *px++;
			c1 = *px++;
			c0 = (c0 << 24) | ((c0 << 8) & 0x00ff0000)
				| ((c0 >> 8) & 0x0000ff00) | (c0 >> 24);
			c1 = (c1 << 24) | ((c1 << 8) & 0x00ff0000)
				| ((c1 >> 8) & 0x0000ff00) | (c1 >> 24);
			*pz++ = c1;
			*pz++ = c0;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

static mlib_status
mlib_VectorReverseByteOrder_U24_U24(
	mlib_u8 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	const mlib_u8 *px = x;
	mlib_u8 *pz = z;
	mlib_s32 i, c0, c1, c2, ntail, nblock;
	__m128i zbuf, buf1, buf2, buf3, mask1, mask2, mask3;
	mask1 = _mm_set_epi32(0x000000ff, 0x0000ff00, 0x00ff0000, 0xff0000ff);
	mask2 = _mm_set_epi32(0x0000ff00, 0x00ff0000, 0xff0000ff, 0x0000ff00);
	mask3 = _mm_set_epi32(0x00ff0000, 0xff0000ff, 0x0000ff00, 0x00ff0000);

	ntail = n % 5;
	ntail = (ntail == 0) ? 5 : ntail;
	nblock = (n - ntail) / 5;
	if (nblock > 0) {
		for (i = 0; i < nblock; i++) {
			buf2 = _mm_loadu_si128((__m128i *)px);
			buf1 = _mm_srli_si128(buf2, 2);
			buf3 = _mm_slli_si128(buf2, 2);
			buf1 = _mm_and_si128(buf1, mask1);
			buf3 = _mm_and_si128(buf3, mask3);
			zbuf = _mm_and_si128(buf2, mask2);
			zbuf = _mm_or_si128(zbuf, buf1);
			zbuf = _mm_or_si128(zbuf, buf3);

			_mm_storeu_si128((__m128i *)pz, zbuf);
			px += 15;
			pz += 15;
		}

		for (i = 0; i < ntail; i++) {
			c0 = *px++;
			c1 = *px++;
			c2 = *px++;
			*pz++ = c2;
			*pz++ = c1;
			*pz++ = c0;
		}
	} else {
		for (i = 0; i < n; i++) {
			c0 = *px++;
			c1 = *px++;
			c2 = *px++;
			*pz++ = c2;
			*pz++ = c1;
			*pz++ = c0;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorReverseByteOrder(
	void *z,
	const void *x,
	mlib_s32 n,
	mlib_s32 s)
{
	mlib_s32 i, v0, v1, v2, v3, v4, v5, v6, v7;

	switch (s) {
	    case 2:

		    if ((((mlib_addr)x | (mlib_addr)z) & 1) == 0)
			    return __mlib_VectorReverseByteOrder_U16_U16(
				    (mlib_u16 *)z, (const mlib_u16 *)x, n);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)x)[2 * i];
			    v1 = ((const mlib_u8 *)x)[2 * i + 1];
			    ((mlib_u8 *)z)[2 * i + 1] = v0;
			    ((mlib_u8 *)z)[2 * i] = v1;
		    }

		    break;

	    case 3:
		    return mlib_VectorReverseByteOrder_U24_U24((mlib_u8 *)z,
			    (const mlib_u8 *)x, n);

	    case 4:

		    if ((((mlib_addr)x | (mlib_addr)z) & 3) == 0)
			    return __mlib_VectorReverseByteOrder_U32_U32(
				    (mlib_u32 *)z, (const mlib_u32 *)x, n);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)x)[4 * i];
			    v1 = ((const mlib_u8 *)x)[4 * i + 1];
			    v2 = ((const mlib_u8 *)x)[4 * i + 2];
			    v3 = ((const mlib_u8 *)x)[4 * i + 3];
			    ((mlib_u8 *)z)[4 * i + 3] = v0;
			    ((mlib_u8 *)z)[4 * i + 2] = v1;
			    ((mlib_u8 *)z)[4 * i + 1] = v2;
			    ((mlib_u8 *)z)[4 * i] = v3;
		    }

		    break;

	    case 5:
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)x)[5 * i];
			    v1 = ((const mlib_u8 *)x)[5 * i + 1];
			    v2 = ((const mlib_u8 *)x)[5 * i + 2];
			    v3 = ((const mlib_u8 *)x)[5 * i + 3];
			    v4 = ((const mlib_u8 *)x)[5 * i + 4];
			    ((mlib_u8 *)z)[5 * i + 4] = v0;
			    ((mlib_u8 *)z)[5 * i + 3] = v1;
			    ((mlib_u8 *)z)[5 * i + 2] = v2;
			    ((mlib_u8 *)z)[5 * i + 1] = v3;
			    ((mlib_u8 *)z)[5 * i] = v4;
		    }

		    break;

	    case 6:
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)x)[6 * i];
			    v1 = ((const mlib_u8 *)x)[6 * i + 1];
			    v2 = ((const mlib_u8 *)x)[6 * i + 2];
			    v3 = ((const mlib_u8 *)x)[6 * i + 3];
			    v4 = ((const mlib_u8 *)x)[6 * i + 4];
			    v5 = ((const mlib_u8 *)x)[6 * i + 5];
			    ((mlib_u8 *)z)[6 * i + 5] = v0;
			    ((mlib_u8 *)z)[6 * i + 4] = v1;
			    ((mlib_u8 *)z)[6 * i + 3] = v2;
			    ((mlib_u8 *)z)[6 * i + 2] = v3;
			    ((mlib_u8 *)z)[6 * i + 1] = v4;
			    ((mlib_u8 *)z)[6 * i] = v5;
		    }

		    break;

	    case 7:
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)x)[7 * i];
			    v1 = ((const mlib_u8 *)x)[7 * i + 1];
			    v2 = ((const mlib_u8 *)x)[7 * i + 2];
			    v3 = ((const mlib_u8 *)x)[7 * i + 3];
			    v4 = ((const mlib_u8 *)x)[7 * i + 4];
			    v5 = ((const mlib_u8 *)x)[7 * i + 5];
			    v6 = ((const mlib_u8 *)x)[7 * i + 6];
			    ((mlib_u8 *)z)[7 * i + 6] = v0;
			    ((mlib_u8 *)z)[7 * i + 5] = v1;
			    ((mlib_u8 *)z)[7 * i + 4] = v2;
			    ((mlib_u8 *)z)[7 * i + 3] = v3;
			    ((mlib_u8 *)z)[7 * i + 2] = v4;
			    ((mlib_u8 *)z)[7 * i + 1] = v5;
			    ((mlib_u8 *)z)[7 * i] = v6;
		    }

		    break;

	    case 8:

		    if ((((mlib_addr)x | (mlib_addr)z) & 7) == 0)
			    return __mlib_VectorReverseByteOrder_U64_U64(
				    (mlib_u64 *)z, (const mlib_u64 *)x, n);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)x)[8 * i];
			    v1 = ((const mlib_u8 *)x)[8 * i + 1];
			    v2 = ((const mlib_u8 *)x)[8 * i + 2];
			    v3 = ((const mlib_u8 *)x)[8 * i + 3];
			    v4 = ((const mlib_u8 *)x)[8 * i + 4];
			    v5 = ((const mlib_u8 *)x)[8 * i + 5];
			    v6 = ((const mlib_u8 *)x)[8 * i + 6];
			    v7 = ((const mlib_u8 *)x)[8 * i + 7];
			    ((mlib_u8 *)z)[8 * i + 7] = v0;
			    ((mlib_u8 *)z)[8 * i + 6] = v1;
			    ((mlib_u8 *)z)[8 * i + 5] = v2;
			    ((mlib_u8 *)z)[8 * i + 4] = v3;
			    ((mlib_u8 *)z)[8 * i + 3] = v4;
			    ((mlib_u8 *)z)[8 * i + 2] = v5;
			    ((mlib_u8 *)z)[8 * i + 1] = v6;
			    ((mlib_u8 *)z)[8 * i] = v7;
		    }

		    break;

	    default:
		    return (MLIB_FAILURE);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorReverseByteOrder_U16(
	mlib_u16 *xz,
	mlib_s32 n)
{
	return (__mlib_VectorReverseByteOrder_U16_U16(xz, xz, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorReverseByteOrder_U32(
	mlib_u32 *xz,
	mlib_s32 n)
{
	return (__mlib_VectorReverseByteOrder_U32_U32(xz, xz, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorReverseByteOrder_U64(
	mlib_u64 *xz,
	mlib_s32 n)
{
	return (__mlib_VectorReverseByteOrder_U64_U64(xz, xz, n));
}

/* *********************************************************** */

static mlib_status
mlib_VectorReverseByteOrder_U24(
	mlib_u8 *xz,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_u8 *pxz = xz;
	mlib_s32 i, c0, c1, c2, ntail, nblock;
	__m128i zbuf, buf1, buf2, buf3, mask1, mask2, mask3;
	mask1 = _mm_set_epi32(0x000000ff, 0x0000ff00, 0x00ff0000, 0xff0000ff);
	mask2 = _mm_set_epi32(0xff00ff00, 0x00ff0000, 0xff0000ff, 0x0000ff00);
	mask3 = _mm_set_epi32(0x00ff0000, 0xff0000ff, 0x0000ff00, 0x00ff0000);

	nblock = n / 5;
	ntail = n - nblock * 5;

	if (nblock > 0) {
		for (i = 0; i < nblock; i++) {
			buf2 = _mm_loadu_si128((__m128i *)pxz);
			buf1 = _mm_srli_si128(buf2, 2);
			buf3 = _mm_slli_si128(buf2, 2);
			buf1 = _mm_and_si128(buf1, mask1);
			buf3 = _mm_and_si128(buf3, mask3);
			zbuf = _mm_and_si128(buf2, mask2);
			zbuf = _mm_or_si128(zbuf, buf1);
			zbuf = _mm_or_si128(zbuf, buf3);

			_mm_storeu_si128((__m128i *)pxz, zbuf);
			pxz += 15;
		}

		for (i = 0; i < ntail; i++) {
			c0 = pxz[0];
			c2 = pxz[2];
			pxz[0] = c2;
			pxz[2] = c0;
			pxz += 3;
		}
	} else {
		for (i = 0; i < n; i++) {
			c0 = pxz[0];
			c2 = pxz[2];
			pxz[0] = c2;
			pxz[2] = c0;
			pxz += 3;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorReverseByteOrder_Inp(
	void *xz,
	mlib_s32 n,
	mlib_s32 s)
{
	mlib_s32 i, v0, v1, v2, v3, v4, v5, v6, v7;

	switch (s) {
	    case 2:

		    if (((mlib_addr)xz & 1) == 0)
			    return __mlib_VectorReverseByteOrder_U16((mlib_u16
				    *)xz, n);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)xz)[2 * i];
			    v1 = ((const mlib_u8 *)xz)[2 * i + 1];
			    ((mlib_u8 *)xz)[2 * i + 1] = v0;
			    ((mlib_u8 *)xz)[2 * i] = v1;
		    }

		    break;

	    case 3:
		    return (mlib_VectorReverseByteOrder_U24((mlib_u8 *)xz, n));

	    case 4:

		    if (((mlib_addr)xz & 3) == 0)
			    return __mlib_VectorReverseByteOrder_U32((mlib_u32
				    *)xz, n);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)xz)[4 * i];
			    v1 = ((const mlib_u8 *)xz)[4 * i + 1];
			    v2 = ((const mlib_u8 *)xz)[4 * i + 2];
			    v3 = ((const mlib_u8 *)xz)[4 * i + 3];
			    ((mlib_u8 *)xz)[4 * i + 3] = v0;
			    ((mlib_u8 *)xz)[4 * i + 2] = v1;
			    ((mlib_u8 *)xz)[4 * i + 1] = v2;
			    ((mlib_u8 *)xz)[4 * i] = v3;
		    }

		    break;

	    case 5:
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)xz)[5 * i];
			    v1 = ((const mlib_u8 *)xz)[5 * i + 1];
			    v3 = ((const mlib_u8 *)xz)[5 * i + 3];
			    v4 = ((const mlib_u8 *)xz)[5 * i + 4];
			    ((mlib_u8 *)xz)[5 * i + 4] = v0;
			    ((mlib_u8 *)xz)[5 * i + 3] = v1;
			    ((mlib_u8 *)xz)[5 * i + 1] = v3;
			    ((mlib_u8 *)xz)[5 * i] = v4;
		    }

		    break;

	    case 6:
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)xz)[6 * i];
			    v1 = ((const mlib_u8 *)xz)[6 * i + 1];
			    v2 = ((const mlib_u8 *)xz)[6 * i + 2];
			    v3 = ((const mlib_u8 *)xz)[6 * i + 3];
			    v4 = ((const mlib_u8 *)xz)[6 * i + 4];
			    v5 = ((const mlib_u8 *)xz)[6 * i + 5];
			    ((mlib_u8 *)xz)[6 * i + 5] = v0;
			    ((mlib_u8 *)xz)[6 * i + 4] = v1;
			    ((mlib_u8 *)xz)[6 * i + 3] = v2;
			    ((mlib_u8 *)xz)[6 * i + 2] = v3;
			    ((mlib_u8 *)xz)[6 * i + 1] = v4;
			    ((mlib_u8 *)xz)[6 * i] = v5;
		    }

		    break;

	    case 7:
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)xz)[7 * i];
			    v1 = ((const mlib_u8 *)xz)[7 * i + 1];
			    v2 = ((const mlib_u8 *)xz)[7 * i + 2];
			    v4 = ((const mlib_u8 *)xz)[7 * i + 4];
			    v5 = ((const mlib_u8 *)xz)[7 * i + 5];
			    v6 = ((const mlib_u8 *)xz)[7 * i + 6];
			    ((mlib_u8 *)xz)[7 * i + 6] = v0;
			    ((mlib_u8 *)xz)[7 * i + 5] = v1;
			    ((mlib_u8 *)xz)[7 * i + 4] = v2;
			    ((mlib_u8 *)xz)[7 * i + 2] = v4;
			    ((mlib_u8 *)xz)[7 * i + 1] = v5;
			    ((mlib_u8 *)xz)[7 * i] = v6;
		    }

		    break;

	    case 8:

		    if (((mlib_addr)xz & 7) == 0)
			    return __mlib_VectorReverseByteOrder_U64((mlib_u64
				    *)xz, n);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)xz)[8 * i];
			    v1 = ((const mlib_u8 *)xz)[8 * i + 1];
			    v2 = ((const mlib_u8 *)xz)[8 * i + 2];
			    v3 = ((const mlib_u8 *)xz)[8 * i + 3];
			    v4 = ((const mlib_u8 *)xz)[8 * i + 4];
			    v5 = ((const mlib_u8 *)xz)[8 * i + 5];
			    v6 = ((const mlib_u8 *)xz)[8 * i + 6];
			    v7 = ((const mlib_u8 *)xz)[8 * i + 7];
			    ((mlib_u8 *)xz)[8 * i + 7] = v0;
			    ((mlib_u8 *)xz)[8 * i + 6] = v1;
			    ((mlib_u8 *)xz)[8 * i + 5] = v2;
			    ((mlib_u8 *)xz)[8 * i + 4] = v3;
			    ((mlib_u8 *)xz)[8 * i + 3] = v4;
			    ((mlib_u8 *)xz)[8 * i + 2] = v5;
			    ((mlib_u8 *)xz)[8 * i + 1] = v6;
			    ((mlib_u8 *)xz)[8 * i] = v7;
		    }

		    break;

	    default:
		    return (MLIB_FAILURE);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */
